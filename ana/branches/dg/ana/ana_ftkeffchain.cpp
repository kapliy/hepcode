
#include <algorithm>
#include <ext/algorithm>
#include <functional>
#include <iterator>
#include <cassert>
#include <cmath>
#include <string>
#include <bitset>
#include <set>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/scope_exit.hpp>
#include <boost/foreach.hpp>
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/assign/std/set.hpp>
#include <iostream>
#include <iomanip>
#include "TFile.h"
#include "TrigFTKAna/AnaConfiguration.hpp"
#include "TrigFTKAna/AnaEventMgr.hpp"
#include "TrigFTKAna/AnaTrack.hpp"
#include "TrigFTKAna/AnaJet.hpp"
#include "TrigFTKAna/AnaSiCluster.hpp"
#include "TrigFTKAna/AnaSuperstrip.hpp"
#include "TrigFTKAna/AnaBeamline.hpp"
#include "DataGraphics/DataGraphics.hh"
#include "TrigFTKAna/StudyTrack.hpp"
#include "TrigFTKAna/StudyTrackComparison.hpp"
#include "TrigFTKAna/StudyTrackingEfficiency.hpp"
#include "TrigFTKAna/StudyInRoadTrackingEfficiency.hpp"
#include "TrigFTKAna/StudyJet.hpp"
#ifdef HAVE_LIBFTKSIM
#include "TrigFTKSim/MultiTruth.h"
#include "TrigFTKSim/FTKPMap.h"
#endif

using namespace std;
using namespace DataGraphics;
using boost::bind;
using boost::shared_ptr;

int Guido=0;

// define some functions. each makes a set of plots for given track input.
void study_efficiency_configuration(const std::string &study_name, std::vector< shared_ptr<const AnaTrack> >& truth_collection, std::vector< shared_ptr<const AnaSiChannel> >& si_channels, vector<unsigned>&);


void study_trackfitter_efficiency( const std::string& study_name , const std::string& truth_name , const std::string& track_name ,
                           const std::vector< shared_ptr<const AnaTrack> >& truth_collection , const vector<int>&,
			   const std::vector< shared_ptr<const AnaTrack> >& track_collection, 
			   vector<int> &);

#ifdef HAVE_LIBFTKSIM
// plane map
FTKPlaneMap *PlaneMap(0x0);

void road_truth_association( const vector< boost::shared_ptr<const AnaRoad> >& roads, 
			     const vector< boost::shared_ptr<const AnaTrack> >& truth_tracks,
			     map< shared_ptr<const AnaRoad>, set< shared_ptr<const AnaTrack> > >& road_truth_map,
			     map< shared_ptr<const AnaTrack>, set< shared_ptr<const AnaRoad> > >& truth_road_map,
			     map< pair< shared_ptr<const AnaRoad>, shared_ptr<const AnaTrack> >, unsigned int >& n_ss_contributed);

void study_road_efficiency( const std::string& dir_name, 
		       const vector< boost::shared_ptr<const AnaTrack> >& truth_tracks_primary,  const vector<unsigned>&, 
		       const map< shared_ptr<const AnaRoad>, set< shared_ptr<const AnaTrack> > >& road_truth_map,
		       const map< shared_ptr<const AnaTrack>, set< shared_ptr<const AnaRoad> > >& truth_road_map,
		       const map< pair< shared_ptr<const AnaRoad>, shared_ptr<const AnaTrack> >, unsigned int >& n_ss_contributed,
		       vector<int>&);
#endif // HAVE_LIBFTKSIM

#ifdef __cplusplus
extern "C"
#endif
int
F77_MAIN( int argc , char* argv[] )
{
  // initialize configuration and event data manager. 
  //
  // run this program with "--help" to see a list of command line
  // options set by the following line.
  //
  // The AnaEventMgr synchronizes all of the underlying
  // ntuples (wrapper text files, road finder/merger output, track
  // fitter output, athena ntuples) and provides a simple interface
  // (AnaEvent) to their data. each sample is described by a text file in
  // the "sample-def-dir" directory.
  AnaConfiguration::configure("ana_ftkeffchain",argc,argv);
  if( !AnaConfiguration::configured() ) { exit(0); }
  dg::root()->no_save_at_exit();
  BOOST_SCOPE_EXIT() { dg::root()->do_save_at_exit(); } BOOST_SCOPE_EXIT_END;


  // open the plain ascii summary file
  ofstream plain_summary("anaeff_summary.out");
  // print the header
  plain_summary << "Truth ConfFull ConfMajority ConfBad RoadFull RoadFullNF RoadFullNM RoadMajority TrackerFull TrackFullNF TrackFullNM TrackMajority" << endl;

  // loop over each event in the requested sample. by default,
  // AnaEventMgr::begin and ::end give you a loop over the entire
  // sample, but the same interface can iterate over e.g. a short list
  // of run/event pairs.
  //
  // in the case below, the end iterator with a numerical argument
  // points to either the last event, or the user_max_nevents-th
  // event, whichever comes first.
  AnaEventMgr* evtmgr = AnaEventMgr::instance(); assert(evtmgr);
  evtmgr->copy_ftk_si_clusters( AnaConfiguration::do_si_cluster_studies() ); // attempts to access road/superstrip information and retrieve clusters. may be slow.

  // create a create a plane-map object
  PlaneMap = new FTKPlaneMap(AnaConfiguration::ftk_pmap().c_str());

  for( AnaEventMgr::iterator i=evtmgr->begin(), f=evtmgr->end(AnaConfiguration::user_max_nevents()); i!=f; ++i ) {
    
    Guido++; 

    const shared_ptr<const AnaEvent>& evt( *i );
    if( !evt ) { break; } // no AnaEvent means something went wrong, e.g. premature end of file

    // Workaround to skip athena/CBNT problems
    if( evt->inconsistentCBNT ) { continue; }

    // all the event data are stored as STL containers of Ana* objects. these function
    // calls retrieve each type of data. in principle, the data are not loaded from disk
    // unless/until they are requested here (in practice, this is not yet implemented).
    const vector< shared_ptr<const AnaTrack> >& ftk_tracks_all( evt->ftk_tracks(AnaEventMgr::DEFAULT_SOURCE) );
    const vector< shared_ptr<const AnaTrack> >& offline_tracks_all( evt->offline_tracks(AnaEventMgr::DEFAULT_SOURCE) );
    const vector< shared_ptr<const AnaTrack> >& truth_tracks_all( evt->truth_tracks(AnaEventMgr::DEFAULT_SOURCE) );

    // filter out truth tracks with wild impact parameters, no charge,
    // infinite momentum, etc.  by requiring AnaTrack::detpaper() == true.
    // also, remove pileup tracks (UniqueBarcode::event_index!=0) from consideration.
    // limit truth tracks |eta| and pt with configurable cuts.
    vector< shared_ptr<const AnaTrack> > truth_tracks_primary;
    truth_tracks_primary.reserve( truth_tracks_all.size() );
    std::remove_copy_if( truth_tracks_all.begin() , truth_tracks_all.end() , back_inserter(truth_tracks_primary) ,
                         bind(&AnaTrack::detpaper,_1) == false ||
                         bind(&AnaTrack::pt,_1) < AnaConfiguration::minimum_track_pt_cut() ||
                         bind( &AnaConfiguration::phi_fails_cut , bind( &AnaTrack::phi0 , _1 ) ) == true ||
                         bind( std::ptr_fun<AnaTrack::ftype,AnaTrack::ftype>(std::abs) , bind(&AnaTrack::d0,_1)) > AnaConfiguration::maximum_track_absd0_cut() ||
                         bind( std::ptr_fun<AnaTrack::ftype,AnaTrack::ftype>(std::abs) , bind(&AnaTrack::eta,_1)) > AnaConfiguration::maximum_track_abseta_cut() ||
                         bind(&UniqueBarcode::event_index,bind(&AnaTrack::barcode,_1))!=0 || 
			 bind( std::ptr_fun<AnaTrack::ftype,AnaTrack::ftype>(std::abs) , bind(&AnaTrack::z0,_1)) > 120.);

    // Get the z0 of the highest-pT primary track
    float primary_vertex_z0(0);
    vector< shared_ptr<const AnaTrack> >::iterator highest_pt_truth(std::max_element( truth_tracks_primary.begin(), truth_tracks_primary.end(), 
										      bind( &AnaTrack::pt, _1 ) < bind( &AnaTrack::pt, _2 )));
    if( highest_pt_truth != truth_tracks_primary.end() )
      primary_vertex_z0 = (*highest_pt_truth)->z0();
    
    // apply quality cuts to offline tracks if requested
    vector< shared_ptr<const AnaTrack> > offline_tracks;
    offline_tracks.reserve( offline_tracks_all.size() );
    std::remove_copy_if( offline_tracks_all.begin() , offline_tracks_all.end() , back_inserter(offline_tracks) ,
			 bind( std::ptr_fun<AnaTrack::ftype,AnaTrack::ftype>(std::abs),bind(&AnaTrack::z0cL,_1,primary_vertex_z0)) > 1.5 ||
			 bind(&AnaTrack::chi2ndof,_1)  > 10 || // FIXME: restore this cut when not running on wrapper offline tracks
			 bind(&AnaTrack::nhits,_1) < 9 ||
			 bind(&AnaTrack::pixel_nholes,_1) > 0 ||
			 bind(&AnaTrack::sct_nholes,_1) > 2 ||
                         //bind( &AnaConfiguration::phi_fails_cut , bind( &AnaTrack::phi0 , _1 ) ) == true ||
                         bind(&AnaTrack::pt,_1) < AnaConfiguration::minimum_track_pt_cut() ||
                         bind( std::ptr_fun<AnaTrack::ftype,AnaTrack::ftype>(std::abs) , bind(&AnaTrack::d0,_1)) > AnaConfiguration::maximum_track_absd0_cut() ||
                         bind( std::ptr_fun<AnaTrack::ftype,AnaTrack::ftype>(std::abs) , bind(&AnaTrack::eta,_1)) > AnaConfiguration::maximum_track_abseta_cut() );
    
    vector< shared_ptr<const AnaTrack> > offline_ftklike_tracks;
    offline_ftklike_tracks.reserve( offline_tracks_all.size() );
    std::remove_copy_if( offline_tracks_all.begin() , offline_tracks_all.end() , back_inserter(offline_ftklike_tracks) ,
			 bind(&AnaTrack::chi2ndof,_1)  > 4.25 || // Based on comparing chi2/ndof shape between FTK & offline
                         //bind( &AnaConfiguration::phi_fails_cut , bind( &AnaTrack::phi0 , _1 ) ) == true ||
                         bind(&AnaTrack::pt,_1) < AnaConfiguration::minimum_track_pt_cut() ||
                         bind( std::ptr_fun<AnaTrack::ftype,AnaTrack::ftype>(std::abs) , bind(&AnaTrack::d0,_1)) > AnaConfiguration::maximum_track_absd0_cut() ||
                         bind( std::ptr_fun<AnaTrack::ftype,AnaTrack::ftype>(std::abs) , bind(&AnaTrack::eta,_1)) > AnaConfiguration::maximum_track_abseta_cut() ||
			 bind(&AnaTrack::nhits,_1) < 9 || bind(&AnaTrack::sct_nholes,_1) > 2 || bind(&AnaTrack::pixel_nholes,_1) > 1 ||
			 ( bind( std::ptr_fun<AnaTrack::ftype,AnaTrack::ftype>(std::abs) , bind(&AnaTrack::eta,_1)) < 1.0 &&
			   bind(&AnaTrack::nhits,_1) < 10 || bind(&AnaTrack::nholes,_1) > 1 ) ||
			 ( bind( std::ptr_fun<AnaTrack::ftype,AnaTrack::ftype>(std::abs) , bind(&AnaTrack::eta,_1)) > 1.3 &&
			   bind(&AnaTrack::nhits,_1) < 10 || bind(&AnaTrack::nholes,_1) > 1 )
			 );
 
    // filter FTK tracks with reasonable parameters
    vector< shared_ptr<const AnaTrack> > ftk_tracks;
    ftk_tracks.reserve( ftk_tracks_all.size() );
    std::remove_copy_if( ftk_tracks_all.begin() , ftk_tracks_all.end() , back_inserter(ftk_tracks) ,
                         //                         bind(&AnaTrack::ftk_is_guido_category_3,_1) == true ||
                         // bind(&AnaTrack::nhits,_1)<10 ||
                         // (AnaConfiguration::use_strict_majority() && bind(&AnaTrack::nhits,_1)<10)) || 
                         //bind( &AnaConfiguration::phi_fails_cut , bind( &AnaTrack::phi0 , _1 ) ) == true ||
                         bind(&AnaTrack::pt,_1) < AnaConfiguration::minimum_track_pt_cut() ||
                         bind( std::ptr_fun<AnaTrack::ftype,AnaTrack::ftype>(std::abs) , bind(&AnaTrack::d0,_1)) > AnaConfiguration::maximum_track_absd0_cut() ||
                         bind( std::ptr_fun<AnaTrack::ftype,AnaTrack::ftype>(std::abs) , bind(&AnaTrack::z0,_1)) > 120. ||

                         bind( std::ptr_fun<AnaTrack::ftype,AnaTrack::ftype>(std::abs) , bind(&AnaTrack::eta,_1)) > AnaConfiguration::maximum_track_abseta_cut() );

    vector< shared_ptr<const AnaTrack> > ftk_tracks_final;
    ftk_tracks_final.reserve( ftk_tracks_final.size() );
    std::remove_copy_if( ftk_tracks_all.begin() , ftk_tracks_all.end() , back_inserter(ftk_tracks_final) ,
                         //                         bind(&AnaTrack::ftk_is_guido_category_3,_1) == true ||
                         // bind(&AnaTrack::nhits,_1)<10 ||
                         // (AnaConfiguration::use_strict_majority() && bind(&AnaTrack::nhits,_1)<10)) || 
                         //bind( &AnaConfiguration::phi_fails_cut , bind( &AnaTrack::phi0 , _1 ) ) == true ||
                         bind(&AnaTrack::pt,_1) < AnaConfiguration::minimum_track_pt_cut() ||
                         bind( std::ptr_fun<AnaTrack::ftype,AnaTrack::ftype>(std::abs) , bind(&AnaTrack::d0,_1)) > AnaConfiguration::maximum_track_absd0_cut() ||
                         bind( std::ptr_fun<AnaTrack::ftype,AnaTrack::ftype>(std::abs) , bind(&AnaTrack::z0,_1)) > 120. ||

                         bind( std::ptr_fun<AnaTrack::ftype,AnaTrack::ftype>(std::abs) , bind(&AnaTrack::eta,_1)) > AnaConfiguration::maximum_track_abseta_cut()|| 
			 bind(&AnaTrack::ftk_hw_rejected,_1)>0 );


    // separate truth tracks which represent muons
    vector< shared_ptr<const AnaTrack> > truth_muon_tracks;
    truth_muon_tracks.reserve( truth_tracks_primary.size() );
    std::remove_copy_if( truth_tracks_primary.begin() , truth_tracks_primary.end() , back_inserter(truth_muon_tracks) ,
                         bind( std::ptr_fun<int,int>(std::abs) , bind(&AnaTrack::pdg_code,_1)) != 13 );
    
    // example: determine whether or not the truth_tracks collection contains a track with pT>1 GeV and |eta|<=configurable cut,
    const bool has_fiducial_truth = std::count_if( truth_tracks_primary.begin() , truth_tracks_primary.end() , 
                                                   bind(&AnaTrack::pt,_1) > AnaConfiguration::minimum_track_pt_cut() &&
                                                   bind( ptr_fun<AnaTrack::ftype,AnaTrack::ftype>(std::abs) , 
                                                         bind(&AnaTrack::eta,_1) ) <= AnaConfiguration::maximum_track_abseta_cut() ) > 0u;
    // the filtered truth track collection doesn't contain any good tracks, skip this event.
    if( truth_tracks_primary.empty() ) { continue; }
    

    vector< shared_ptr<const AnaSiChannel> > silicon_hits_all = evt->si_channels(AnaEventMgr::DEFAULT_SOURCE);
    // remove hits from pileup
    vector< shared_ptr<const AnaSiChannel> > silicon_hits_primary;
    std::remove_copy_if( silicon_hits_all.begin() , silicon_hits_all.end() , back_inserter(silicon_hits_primary) ,
                         bind(&UniqueBarcode::event_index,bind(&AnaSiChannel::barcode,_1))!=0);


    // vector of the truth tracks with the right configuration of hits
    vector<unsigned> truth_tracks_primary_nhits; // number of layers with hits
    truth_tracks_primary_nhits.reserve(truth_tracks_primary.size());

    // build study on efficiency loss in the different steps of FTK simulation
    if (true) {
      // create the FTK directory for the efficiency chain
      dg::down( "Configuration" , "Layer configuration efficiency" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;

      study_efficiency_configuration("Truth_configuration",truth_tracks_primary,silicon_hits_primary,truth_tracks_primary_nhits);
    }
    // if desired, fill plots comparing each tracking algorithm with truth tracks.

    const vector< shared_ptr<const AnaRoad> >& ftk_roads(evt->ftk_roads(AnaEventMgr::DEFAULT_SOURCE));

    // count numbers of tracks with pT > xx (default 1 GeV) for each collection.
    const unsigned int n_truth = truth_tracks_primary.size();
    const unsigned int n_ftk = ftk_tracks.size();
    const unsigned int n_offline = offline_tracks.size();
    // And some extra categories for offline:
    const unsigned int n_offline_hardscatter = std::count_if( offline_tracks.begin() , offline_tracks.end() , 
							     bind(&UniqueBarcode::event_index,bind(&AnaTrack::barcode,_1))==0 &&
							     bind(&UniqueBarcode::barcode,bind(&AnaTrack::barcode,_1))>0 &&
							     bind(&AnaTrack::barcode_matching_fraction,_1) >= AnaConfiguration::barcode_matching_minimum_cut());
    const unsigned int n_offline_pileup = std::count_if( offline_tracks.begin() , offline_tracks.end() , 
							 bind(&UniqueBarcode::event_index,bind(&AnaTrack::barcode,_1))>0 &&
							 bind(&UniqueBarcode::barcode,bind(&AnaTrack::barcode,_1))>0 &&
							 bind(&AnaTrack::barcode_matching_fraction,_1) >= AnaConfiguration::barcode_matching_minimum_cut());
    const unsigned int n_offline_fake = std::count_if( offline_tracks.begin() , offline_tracks.end() , 
						       bind(&UniqueBarcode::event_index,bind(&AnaTrack::barcode,_1))<0 ||
						       bind(&UniqueBarcode::barcode,bind(&AnaTrack::barcode,_1))<=0 ||
						       bind(&AnaTrack::barcode_matching_fraction,_1) < AnaConfiguration::barcode_matching_minimum_cut());


    // vector of the truth tracks with the right configuration of hits
    vector<int> truth_tracks_primary_matchtype;
    truth_tracks_primary_matchtype.reserve(truth_tracks_primary.size());
    vector<int> truth_tracks_primary_AMmatchtype;
    truth_tracks_primary_AMmatchtype.reserve(truth_tracks_primary.size());

    if( true ) {
      // plot some crude road and superstrip properties.
      dg::down( "Bank" , "Bank effiency" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
            
      // Prepare road studies
      map< shared_ptr<const AnaRoad>, set< shared_ptr<const AnaTrack> > > road_truth_map;
      map< shared_ptr<const AnaTrack>, set< shared_ptr<const AnaRoad> > > truth_road_map;
      map< pair< shared_ptr<const AnaRoad>, shared_ptr<const AnaTrack> >, unsigned int > n_ss_contributed;
      map< pair< shared_ptr<const AnaRoad>, shared_ptr<const AnaTrack> >, unsigned int > bitmask_ss_contributed;
      road_truth_association( ftk_roads, truth_tracks_all, road_truth_map, truth_road_map, n_ss_contributed );
    
      study_road_efficiency( "ftk_roads", truth_tracks_primary, truth_tracks_primary_nhits, 
			road_truth_map, truth_road_map, n_ss_contributed ,  
			truth_tracks_primary_matchtype);
      
      road_truth_map.clear(); truth_road_map.clear(); n_ss_contributed.clear(); bitmask_ss_contributed.clear();
      road_truth_association(evt->ftk_roads(AnaEventMgr::DEFAULT_SOURCE+"_amroads"), truth_tracks_all, road_truth_map, truth_road_map, n_ss_contributed);
      
      study_road_efficiency( "ftk_am_roads", truth_tracks_primary, truth_tracks_primary_nhits, 
			road_truth_map, truth_road_map, n_ss_contributed , 
			truth_tracks_primary_AMmatchtype);
    }


    vector<int> truth_tracks_primary_nfittermatches;
    truth_tracks_primary_nfittermatches.reserve(truth_tracks_primary.size());
    vector<int> truth_tracks_primary_nmergermatches;
    truth_tracks_primary_nmergermatches.reserve(truth_tracks_primary.size());

    if( true ) { 
      dg::down( "Fitter" , "comparing tracking algorithms with truth tracks" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      study_trackfitter_efficiency( "tracks_ftkroad" , "TRUTH" , "FTK" , truth_tracks_primary , truth_tracks_primary_matchtype, ftk_tracks, 
				 truth_tracks_primary_nfittermatches );
      dg::up();

      dg::down( "Merger" , "merging algorithms with truth tracks" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      study_trackfitter_efficiency( "tracks_ftkroad" , "TRUTH" , "FTK" , truth_tracks_primary , truth_tracks_primary_nfittermatches, ftk_tracks_final,
				 truth_tracks_primary_nmergermatches);
      dg::up();
    }

    /* This block prints a summary for the event, the summary is saved in an
       ascii file and for each event reports:
       * the number of truth tracks
       * the number of full and majority tracks (configuation)
       * the number of full tracks with a full, noisy full and majority roads,
         the numberof majority tracks with noisy full or majority roads
    */
    if (true) {
    

    }

  } // end for each event


  AnaEventMgr::instance()->close_sample();

}

void study_efficiency_configuration(const std::string &study_name, std::vector< shared_ptr<const AnaTrack> >& truth_collection, std::vector< shared_ptr<const AnaSiChannel> >& si_channels, vector<unsigned> &filtered_nhits)
{
  // number of planes
  int nplanes = PlaneMap->getNPlanes();
  bool *hasHitOnPlane = new bool[nplanes];

  // cluster the hits according the barcode
  map< UniqueBarcode, vector< shared_ptr<const AnaSiChannel> > > map_hits;
  BOOST_FOREACH( shared_ptr<const AnaSiChannel>& ihit , si_channels ) {
    map_hits[ihit->barcode()].push_back(ihit);
  }

  // loop over the truth tracks
  BOOST_FOREACH( shared_ptr<const AnaTrack>& truth , truth_collection ) {
    dg::down( study_name , "Configuration" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
  
    // reset the layer flag and counter
    for (int i=0;i!=nplanes;++i) hasHitOnPlane[i] = false;
    unsigned nFiredPlanes(0);

    // retrieve the hits associated with this track
    vector< shared_ptr<const AnaSiChannel> > &truth_hits = map_hits[truth->barcode()];

    
    vector< shared_ptr<const AnaSiChannel> >::iterator ihit = truth_hits.begin();
    vector< shared_ptr<const AnaSiChannel> >::iterator ihit_end = truth_hits.end();
    for (;ihit!=ihit_end;++ihit) { // begin hit loop
      
      int ilayer = (*ihit)->ftk_layerid();
      FTKPlaneSection &pinfo = PlaneMap->getMap((*ihit)->channel_type()==AnaSiChannel::PIXEL ? 1 : 0,
						(*ihit)->is_barrel() ? 0 : 1,
						ilayer);
      int ihit_plane = pinfo.getPlane();
      assert(ihit_plane>=-1&&ihit_plane<nplanes);
      if (ihit_plane==-1) continue; // skip unused layers
      else if (!hasHitOnPlane[ihit_plane]) {
	hasHitOnPlane[ihit_plane] = true;
	++nFiredPlanes;
      }

    } // end hit loop


    dg::fillh( "n_layers", nplanes+1 , 0 , nplanes+1 , nFiredPlanes , "N Layers" );
    for (int i=0;i!=nplanes;++i) {
      dg::filleff("eff_layer",nplanes,0,nplanes,i,hasHitOnPlane[i],"Layer ID");
    }

    if (std::abs(truth->eta()) < 1.0) {
      dg::fillh( "n_layers_barrel", nplanes+1 , 0 , nplanes+1 , nFiredPlanes , "N Layers" );
      for (int i=0;i!=nplanes;++i) {
	dg::filleff("eff_layer_barrel",nplanes,0,nplanes,i,hasHitOnPlane[i],"Layer ID");
      }
    }
    else if (std::abs(truth->eta()) > 1.0) {
      dg::fillh( "n_layers_endcap", nplanes+1 , 0 , nplanes+1 , nFiredPlanes , "N Layers" );
      for (int i=0;i!=nplanes;++i) {
	dg::filleff("eff_layer_endcap",nplanes,0,nplanes,i,hasHitOnPlane[i],"Layer ID");
      }
    }    

    if (nFiredPlanes==(nplanes-1)){
      int miss_layer_id=-1;
      for (int i=0;i!=nplanes;++i) {
	dg::filleff("eff_layer_maj",nplanes,0,nplanes,i,hasHitOnPlane[i],"eff_layer_maj");
        if (!hasHitOnPlane[i]) miss_layer_id = i;
        if (miss_layer_id>=0)
	  dg::fillh("n_misslayer_maj",nplanes+1,0,nplanes+1,miss_layer_id,"n_misslayer_maj");
        //if (miss_layer_id==3)
	//cout<<"funnybeginningforgrep track with miss in 3rd layer "<<Guido<<endl;
      }
    }
    
     if (nFiredPlanes==(nplanes-1)){
      int miss_layer_id=-1;
      if (std::abs(truth->eta()) < 1.0) {
      for (int i=0;i!=nplanes;++i) {
	dg::filleff("eff_layer_maj_barrel",nplanes,0,nplanes,i,hasHitOnPlane[i],"eff_layer_maj_barrel");
        if (!hasHitOnPlane[i]) miss_layer_id = i;
        if (miss_layer_id>=0)
	  dg::fillh("n_misslayer_maj_barrel",nplanes+1,0,nplanes+1,miss_layer_id,"n_misslayer_maj_barrel");
      }
      }
     else if (std::abs(truth->eta()) > 1.0) {
      for (int i=0;i!=nplanes;++i) {
      dg::filleff("eff_layer_maj_endcap",nplanes,0,nplanes,i,hasHitOnPlane[i],"eff_layer_maj_endcap");
        if (!hasHitOnPlane[i]) miss_layer_id = i;
        if (miss_layer_id>=0)
	  dg::fillh("n_misslayer_maj_endcap",nplanes+1,0,nplanes+1,miss_layer_id,"n_misslayer_maj_endcap");
      }
     }
     }


    // propagate the information on the number of hits
    filtered_nhits.push_back(nFiredPlanes);


    // Implement the object that control the efficiency study
    StudyTrackingEfficiency st_eff( study_name+"_all" , "Configuration efficiency w.r.t. track paramters");
    st_eff.for_event( AnaEventMgr::current_event() );
    st_eff.add_constant_category( "muon" , abs(truth->pdg_code()) == 13 );
    st_eff.add_constant_category( "pion" , abs(truth->pdg_code()) == 211 );
    st_eff.add_constant_category( "electron" , abs(truth->pdg_code()) == 11 );
    st_eff.add_constant_category( "barrel" , std::abs(truth->eta()) < 1.0 );
    st_eff.add_constant_category( "endcap" , std::abs(truth->eta()) > 1.5 );
    st_eff.for_only( StudyTrackingEfficiency::type( truth , nFiredPlanes>=(nplanes-1) ) );

    // create a similar set of plots for the full tracks
    StudyTrackingEfficiency st_eff_full( study_name+"_full" , "Configuration efficiency w.r.t. track paramters (full)");
    st_eff_full.for_event( AnaEventMgr::current_event() );
    st_eff_full.add_constant_category( "muon" , abs(truth->pdg_code()) == 13 );
    st_eff_full.add_constant_category( "pion" , abs(truth->pdg_code()) == 211 );
    st_eff_full.add_constant_category( "electron" , abs(truth->pdg_code()) == 11 );
    st_eff_full.add_constant_category( "barrel" , std::abs(truth->eta()) < 1.0 );
    st_eff_full.add_constant_category( "endcap" , std::abs(truth->eta()) > 1.5 );
    st_eff_full.for_only( StudyTrackingEfficiency::type( truth , nFiredPlanes==nplanes ) );

    // create a similar set of plots for the majority tracks
    // this category is just the difference between the previous two categories but can be useful
    StudyTrackingEfficiency st_eff_maj( study_name+"_maj" , "Configuration efficiency w.r.t. track paramters (maj)");
    st_eff_maj.for_event( AnaEventMgr::current_event() );
    st_eff_maj.add_constant_category( "muon" , abs(truth->pdg_code()) == 13 );
    st_eff_maj.add_constant_category( "pion" , abs(truth->pdg_code()) == 211 );
    st_eff_maj.add_constant_category( "electron" , abs(truth->pdg_code()) == 11 );
    st_eff_maj.add_constant_category( "barrel" , std::abs(truth->eta()) < 1.0 );
    st_eff_maj.add_constant_category( "endcap" , std::abs(truth->eta()) > 1.5 );
    st_eff_maj.for_only( StudyTrackingEfficiency::type( truth , nFiredPlanes==nplanes-1 ) );

    // create a similar set of plots for the tracks with 5 out of 7 hits
    // the historic reference is as of the time the code is written, 4 out of the past 7 governors of IL are serving time for felony convictions

    StudyTrackingEfficiency st_eff_govplus( study_name+"_govplus" , "Configuration efficiency w.r.t. track paramters (maj minus one)");
    st_eff_govplus.for_event( AnaEventMgr::current_event() );
    st_eff_govplus.add_constant_category( "muon" , abs(truth->pdg_code()) == 13 );
    st_eff_govplus.add_constant_category( "pion" , abs(truth->pdg_code()) == 211 );
    st_eff_govplus.add_constant_category( "electron" , abs(truth->pdg_code()) == 11 );
    st_eff_govplus.add_constant_category( "barrel" , std::abs(truth->eta()) < 1.0 );
    st_eff_govplus.add_constant_category( "endcap" , std::abs(truth->eta()) > 1.5 );
    st_eff_govplus.for_only( StudyTrackingEfficiency::type( truth , nFiredPlanes==nplanes-2 ) );

    // make plot for the tracks that doesn't have the a good configuration
    StudyTrackingEfficiency st_eff_bad( study_name+"_bad" , "Configuration efficiency w.r.t. track paramters");
    st_eff_bad.for_event( AnaEventMgr::current_event() );
    st_eff_bad.add_constant_category( "muon" , abs(truth->pdg_code()) == 13 );
    st_eff_bad.add_constant_category( "pion" , abs(truth->pdg_code()) == 211 );
    st_eff_bad.add_constant_category( "electron" , abs(truth->pdg_code()) == 11 );
    st_eff_bad.add_constant_category( "barrel" , std::abs(truth->eta()) < 1.0 );
    st_eff_bad.add_constant_category( "endcap" , std::abs(truth->eta()) > 1.5 );
    st_eff_bad.for_only( StudyTrackingEfficiency::type( truth , nFiredPlanes<(nplanes-1) ) );

  } // end truth loop

  delete [] hasHitOnPlane;
}


#ifdef HAVE_LIBFTKSIM
void road_truth_association( const vector< boost::shared_ptr<const AnaRoad> >& roads, 
			     const vector< boost::shared_ptr<const AnaTrack> >& truth_tracks,
			     map< shared_ptr<const AnaRoad>, set< shared_ptr<const AnaTrack> > >& road_truth_map,
			     map< shared_ptr<const AnaTrack>, set< shared_ptr<const AnaRoad> > >& truth_road_map,
			     map< pair< shared_ptr<const AnaRoad>, shared_ptr<const AnaTrack> >, unsigned int >& n_ss_contributed) {

  // Keep a map of UniqueBarcodes --> truth tracks to assist lookup.
  map<UniqueBarcode, boost::shared_ptr<const AnaTrack> > truth_lookup;
  
  // Keep track of which barcodes have contributed to this ss
  set< UniqueBarcode > truth_in_ss;

  BOOST_FOREACH( const boost::shared_ptr<const AnaRoad>& road , roads ) {
    for(unsigned int plane = 0, nplanes = road->n_planes();
	plane != nplanes; plane++) {
      truth_in_ss.clear();

      // Loop through the hits in the SS
      const AnaRoad::TruthVector& tv = road->truth(plane);
      for(unsigned int ihit = 0; ihit < tv.size(); ihit++) {
	// Skip masked hits
	if( ihit < road->getHitsMaskByPlane(plane).size() && 
	    road->getHitsMaskByPlane(plane)[ihit] )
	  continue;

	// Get the barcode of this hit
	MultiTruth::Barcode currTruth;
	MultiTruth::Weight weight;
	UniqueBarcode code;
	if( tv[ihit].best(currTruth,weight) )
	  code = UniqueBarcode(currTruth.first, currTruth.second);
	else
	  code = UniqueBarcode(-1,-1);
	
	if( code.barcode() < 0 )
	  code = UniqueBarcode(-1,-1);
	
	// If we already saw this barcode in this SS, move on
	if( truth_in_ss.count( code ) )
	  continue;
	truth_in_ss.insert(code);
	  
	boost::shared_ptr<const AnaTrack> theTruth;
	// Get the truth track associated with this barcode

	map<UniqueBarcode, boost::shared_ptr<const AnaTrack> >::iterator theTruthIter =
	  truth_lookup.find(code);
	if( theTruthIter == truth_lookup.end() ) {
	  // We don't know about it yet - search
	  if( code.event_index() > -1 ) {
	    vector< boost::shared_ptr<const AnaTrack> >::const_iterator it = 
	      find_if(truth_tracks.begin(), truth_tracks.end(),
		      bind(&AnaTrack::barcode,_1)==code);
	    if( it != truth_tracks.end() ) {
	      // Found the truth track
	      theTruth = *it;
	      truth_lookup[code] = theTruth;
	    }
	  }
	}
	else
	  theTruth = theTruthIter->second;

	if( !theTruth ) {
	  // It wasn't found in the truth record of the event
	  theTruth.reset(new AnaTrack(code));
	  // This "truth" track will have the "unknown" algorithm associated with it. Feature.
	  truth_lookup[code] = theTruth;
	}
	  
	if( theTruth->barcode().event_index() > -1 ) {
	  road_truth_map[road].insert(theTruth);
	  truth_road_map[theTruth].insert(road);
	}
	// Make a link between this road and the truth track
	pair< shared_ptr<const AnaRoad>, shared_ptr<const AnaTrack> > road_and_truth = 
	  make_pair< shared_ptr<const AnaRoad>, shared_ptr<const AnaTrack> >(road,theTruth);
	  
	// Have we seen these two before?
	map< pair< shared_ptr<const AnaRoad>, shared_ptr<const AnaTrack> >, unsigned int >::iterator pairIter =
	  n_ss_contributed.find(road_and_truth);
	if( pairIter != n_ss_contributed.end() ) {
	  n_ss_contributed[road_and_truth]++;
	}
	else {
	  n_ss_contributed[road_and_truth] = 1;	
	}

      } // end loop over hits
    } // end loop over planes
  } // end loop over roads
}

void study_road_efficiency( const std::string& dir_name, 
		       const vector< boost::shared_ptr<const AnaTrack> >& truth_tracks_primary, const vector<unsigned> &primary_nhits, 
		       const map< shared_ptr<const AnaRoad>, set< shared_ptr<const AnaTrack> > >& road_truth_map,
		       const map< shared_ptr<const AnaTrack>, set< shared_ptr<const AnaRoad> > >& truth_road_map,
		       const map< pair< shared_ptr<const AnaRoad>, shared_ptr<const AnaTrack> >, unsigned int >& n_ss_contributed,
		       vector<int>& filtered_type) {

  dg::down(dir_name, "FTK roads vs truth");
  BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;

  vector<unsigned>::const_iterator iTruthNHits = primary_nhits.begin();

  // Loop over truth to do efficiency studies
  BOOST_FOREACH( const boost::shared_ptr<const AnaTrack>& truth , truth_tracks_primary ) {    
    if (*iTruthNHits<PlaneMap->getNPlanes()-1) {
      filtered_type.push_back(-1);
      ++iTruthNHits;
      continue; // the track doens't have enough hits, skip
    }
    // Get the set of roads associated with this truth track
    map< shared_ptr<const AnaTrack>, set< shared_ptr<const AnaRoad> > >::const_iterator trmapiter = 
      truth_road_map.find(truth);

    // type of road: 1 MJ, 2 MJ+noise, 4 FULL, 8 MJ-1
    int max_nContributed=0;
    int match_type(0);
    int road_type(0);

    vector<unsigned int> roadsWithAtLeastNHits(AnaEventMgr::current_event()->ftk_road_nplanes(),0);
    unsigned int min_hits(AnaEventMgr::current_event()->ftk_road_nplanes()), max_hits(0);

    if( trmapiter != truth_road_map.end() ) {
      const set< shared_ptr<const AnaRoad> >& any_matched_roads(trmapiter->second);
      BOOST_FOREACH( shared_ptr<const AnaRoad> aRoad, any_matched_roads ) {
	unsigned int totNHits( aRoad->n_hits() );
	std::pair< shared_ptr<const AnaRoad>, shared_ptr<const AnaTrack> > mp(aRoad,truth);
	map< pair< shared_ptr<const AnaRoad>, shared_ptr<const AnaTrack> >, unsigned int >::const_iterator it = 
	  n_ss_contributed.find(mp);
	assert(it != n_ss_contributed.end());
	//unsigned int nContributed = n_ss_contributed[make_pair< shared_ptr<const AnaRoad>, shared_ptr<const AnaTrack> >(aRoad,truth)];
	unsigned int nContributed = it->second;
	assert( nContributed != 0 );
	assert( nContributed <= totNHits );
        if (max_nContributed < nContributed)
	  max_nContributed = nContributed;

        //check if the road has 5/7 layers

        /* if (nContributed==(PlaneMap->getNPlanes()-2)){
          road_type |=8;
	  match_type |=16;
	  }*/

	// check if this road has enough layers
	if (nContributed<(PlaneMap->getNPlanes()-1)) continue;

	//check the type of road (full or majority)
	if (aRoad->n_hits()==PlaneMap->getNPlanes()) { // full
	  road_type |= 2;
	  if (nContributed==PlaneMap->getNPlanes()) {
	    match_type |= 8; // full road with all hits from the truth
	  }
	  else if ((*iTruthNHits)==PlaneMap->getNPlanes()) {
	    match_type |= 4; // full road but with noise from full combination
	  }
	  else {
	    match_type |= 2; // full road from maj combination + noisy hit(s)
	  }
	}
	else { // majority
	  road_type |= 1;
	  match_type |= 1; // majority roads
	}
	// end check of the road type

      }
      
    }

    // search the highest bit set
    int best_match(0);
    for (unsigned vbit=8;vbit;vbit>>=1) {
      if (match_type & vbit) {
	// this is the highest bit set
	best_match = vbit;
	break;
      }
    }

    if (match_type) {
      // if any of the match category is verified add the track to the list of
      // tracks to use in the next step and save the type
      filtered_type.push_back(match_type);
    }
    
    
    // Efficiency studies
    StudyTrackingEfficiency road_eff( "Any_road" , "FTK ROAD efficiency w.r.t. good truth" );
    road_eff.for_event( AnaEventMgr::current_event() );
    //road_eff.set_match_type(match_type);
    road_eff.add_constant_category( "muon" , abs(truth->pdg_code()) == 13 );
    road_eff.add_constant_category( "pion" , abs(truth->pdg_code()) == 211 );
    road_eff.add_constant_category( "electron" , abs(truth->pdg_code()) == 11 );
    road_eff.add_constant_category( "barrel" , std::abs(truth->eta()) < 1.0 );
    road_eff.add_constant_category( "endcap" , std::abs(truth->eta()) > 1.5 );
    road_eff.add_constant_category( "full" , (*iTruthNHits)==PlaneMap->getNPlanes());
    road_eff.add_constant_category( "maj" , (*iTruthNHits)==PlaneMap->getNPlanes()-1);
    road_eff.for_only( StudyTrackingEfficiency::type( truth , best_match ) );


    StudyTrackingEfficiency road_eff_full( "Full_road" , "FTK ROAD efficiency w.r.t. good truth" );
    road_eff_full.for_event( AnaEventMgr::current_event() );
    //road_eff_full.set_match_type(match_type);
    road_eff_full.add_constant_category( "muon" , abs(truth->pdg_code()) == 13 );
    road_eff_full.add_constant_category( "pion" , abs(truth->pdg_code()) == 211 );
    road_eff_full.add_constant_category( "electron" , abs(truth->pdg_code()) == 11 );
    road_eff_full.add_constant_category( "barrel" , std::abs(truth->eta()) < 1.0 );
    road_eff_full.add_constant_category( "endcap" , std::abs(truth->eta()) > 1.5 );
    road_eff_full.add_constant_category( "full" , (*iTruthNHits)==PlaneMap->getNPlanes());
    road_eff_full.add_constant_category( "maj" , (*iTruthNHits)==PlaneMap->getNPlanes()-1);
    road_eff_full.for_only( StudyTrackingEfficiency::type( truth ,  best_match & 8) );

    StudyTrackingEfficiency road_eff_maj( "Maj_road" , "FTK ROAD efficiency w.r.t. good truth" );
    road_eff_maj.for_event( AnaEventMgr::current_event() );
    //road_eff_maj.set_match_type(match_type);
    road_eff_maj.add_constant_category( "muon" , abs(truth->pdg_code()) == 13 );
    road_eff_maj.add_constant_category( "pion" , abs(truth->pdg_code()) == 211 );
    road_eff_maj.add_constant_category( "electron" , abs(truth->pdg_code()) == 11 );
    road_eff_maj.add_constant_category( "barrel" , std::abs(truth->eta()) < 1.0 );
    road_eff_maj.add_constant_category( "endcap" , std::abs(truth->eta()) > 1.5 );
    road_eff_maj.add_constant_category( "full" , (*iTruthNHits)==PlaneMap->getNPlanes());
    road_eff_maj.add_constant_category( "maj" , (*iTruthNHits)==PlaneMap->getNPlanes()-1);
    road_eff_maj.for_only( StudyTrackingEfficiency::type( truth , best_match&1 ) );

    StudyTrackingEfficiency road_eff_submaj( "Sub_Maj_road" , "FTK ROAD efficiency w.r.t. good truth" );
    road_eff_submaj.for_event( AnaEventMgr::current_event() );
    //road_eff_maj.set_match_type(match_type);
    road_eff_submaj.add_constant_category( "muon" , abs(truth->pdg_code()) == 13 );
    road_eff_submaj.add_constant_category( "pion" , abs(truth->pdg_code()) == 211 );
    road_eff_submaj.add_constant_category( "electron" , abs(truth->pdg_code()) == 11 );
    road_eff_submaj.add_constant_category( "barrel" , std::abs(truth->eta()) < 1.0 );
    road_eff_submaj.add_constant_category( "endcap" , std::abs(truth->eta()) > 1.5 );
    //road_eff_submaj.add_constant_category( "full" , (*iTruthNHits)==PlaneMap->getNPlanes());
    //road_eff_submaj.add_constant_category( "maj" , (*iTruthNHits)==PlaneMap->getNPlanes()-1);
    road_eff_submaj.for_only( StudyTrackingEfficiency::type( truth , max_nContributed==(PlaneMap->getNPlanes()-2) ) );

    StudyTrackingEfficiency road_eff_fullN( "Maj_fullN" , "FTK ROAD efficiency w.r.t. good truth" );
    road_eff_fullN.for_event( AnaEventMgr::current_event() );
    //road_eff_fullN.set_match_type(match_type);
    road_eff_fullN.add_constant_category( "muon" , abs(truth->pdg_code()) == 13 );
    road_eff_fullN.add_constant_category( "pion" , abs(truth->pdg_code()) == 211 );
    road_eff_fullN.add_constant_category( "electron" , abs(truth->pdg_code()) == 11 );
    road_eff_fullN.add_constant_category( "barrel" , std::abs(truth->eta()) < 1.0 );
    road_eff_fullN.add_constant_category( "endcap" , std::abs(truth->eta()) > 1.5 );
    road_eff_fullN.add_constant_category( "full" , (*iTruthNHits)==PlaneMap->getNPlanes());
    road_eff_fullN.add_constant_category( "maj" , (*iTruthNHits)==PlaneMap->getNPlanes()-1);
    road_eff_fullN.for_only( StudyTrackingEfficiency::type( truth , best_match&(2|4)  ) );

    StudyTrackingEfficiency road_eff_fullN_maj( "Maj_fullN_maj" , "FTK ROAD efficiency w.r.t. good truth" );
    road_eff_fullN_maj.for_event( AnaEventMgr::current_event() );
    //road_eff_fullN.set_match_type(match_type);
    road_eff_fullN_maj.add_constant_category( "muon" , abs(truth->pdg_code()) == 13 );
    road_eff_fullN_maj.add_constant_category( "pion" , abs(truth->pdg_code()) == 211 );
    road_eff_fullN_maj.add_constant_category( "electron" , abs(truth->pdg_code()) == 11 );
    road_eff_fullN_maj.add_constant_category( "barrel" , std::abs(truth->eta()) < 1.0 );
    road_eff_fullN_maj.add_constant_category( "endcap" , std::abs(truth->eta()) > 1.5 );
    road_eff_fullN_maj.add_constant_category( "full" , (*iTruthNHits)==PlaneMap->getNPlanes());
    road_eff_fullN_maj.add_constant_category( "maj" , (*iTruthNHits)==PlaneMap->getNPlanes()-1);
    road_eff_fullN_maj.for_only( StudyTrackingEfficiency::type( truth , best_match&2  ) );

    StudyTrackingEfficiency road_eff_fullN_full( "Maj_fullN_full" , "FTK ROAD efficiency w.r.t. good truth" );
    road_eff_fullN_full.for_event( AnaEventMgr::current_event() );
    //road_eff_fullN.set_match_type(match_type);
    road_eff_fullN_full.add_constant_category( "muon" , abs(truth->pdg_code()) == 13 );
    road_eff_fullN_full.add_constant_category( "pion" , abs(truth->pdg_code()) == 211 );
    road_eff_fullN_full.add_constant_category( "electron" , abs(truth->pdg_code()) == 11 );
    road_eff_fullN_full.add_constant_category( "barrel" , std::abs(truth->eta()) < 1.0 );
    road_eff_fullN_full.add_constant_category( "endcap" , std::abs(truth->eta()) > 1.5 );
    road_eff_fullN_full.add_constant_category( "full" , (*iTruthNHits)==PlaneMap->getNPlanes());
    road_eff_fullN_full.add_constant_category( "maj" , (*iTruthNHits)==PlaneMap->getNPlanes()-1);
    road_eff_fullN_full.for_only( StudyTrackingEfficiency::type( truth , best_match&4  ) );

    StudyTrackingEfficiency road_eff_noroad( "No_road" , "FTK ROAD efficiency w.r.t. good truth" );
    road_eff_noroad.for_event( AnaEventMgr::current_event() );
    //road_eff_noroad.set_match_type(match_type);
    road_eff_noroad.add_constant_category( "muon" , abs(truth->pdg_code()) == 13 );
    road_eff_noroad.add_constant_category( "pion" , abs(truth->pdg_code()) == 211 );
    road_eff_noroad.add_constant_category( "electron" , abs(truth->pdg_code()) == 11 );
    road_eff_noroad.add_constant_category( "barrel" , std::abs(truth->eta()) < 1.0 );
    road_eff_noroad.add_constant_category( "endcap" , std::abs(truth->eta()) > 1.5 );
    road_eff_noroad.add_constant_category( "full" , (*iTruthNHits)==PlaneMap->getNPlanes());
    road_eff_noroad.add_constant_category( "maj" , (*iTruthNHits)==PlaneMap->getNPlanes()-1);
    road_eff_noroad.for_only( StudyTrackingEfficiency::type( truth , !best_match ) );

    ++iTruthNHits;
  } // End loop over truth
}
#endif // HAVE_LIBFTKSIM


unsigned count_real_hits(const shared_ptr<const AnaTrack> &ftktrack)
{
  unsigned nhits(0);
  const unsigned int bank_id = ftktrack->ftk_bank_id();
  const unsigned long road_id = ftktrack->ftk_road_id();
  const shared_ptr<const AnaEvent> &evt = AnaEventMgr::current_event();

  const shared_ptr<const AnaRoad>& road( evt->ftk_road_lookup( AnaEventMgr::DEFAULT_SOURCE , bank_id , road_id ) );
  if( !road ) {
    cerr << (boost::format("Road # %2% with bank_id %1% not found. Possible issue in the road merger.")%bank_id%road_id).str() << endl;
    assert(road); // the must should exist
  }

  for (int iplane=0;iplane!=PlaneMap->getNPlanes();++iplane) {
    // Keep track of which barcodes have contributed to this ss

    // Loop through the hits in the SS
    const AnaRoad::TruthVector& tv = road->truth(iplane);
    for(unsigned int ihit = 0; ihit < tv.size(); ihit++) {
      // Skip masked hits
      if( ihit < road->getHitsMaskByPlane(iplane).size() && 
	  road->getHitsMaskByPlane(iplane)[ihit] )
	continue;
  
      // Get the barcode of this hit
      MultiTruth::Barcode currTruth;
      MultiTruth::Weight weight;
      UniqueBarcode code;
      if( tv[ihit].best(currTruth,weight) )
	code = UniqueBarcode(currTruth.first, currTruth.second);
      else
	code = UniqueBarcode(-1,-1);
      
      if( code.barcode() < 0 )
	code = UniqueBarcode(-1,-1);
    
      if (code==ftktrack->barcode()) {
	// this hit has the same barcode of the truth, count and go to the next plane
	// the test doesn't look for any weight
	nhits += 1;
	break;
      }
    }
  }
  return nhits;
}

void
study_trackfitter_efficiency( const std::string& study_name , const std::string& truth_name , const std::string& track_name ,
			      const std::vector< shared_ptr<const AnaTrack> >& truth_collection , const vector<int> &roads_matchtype, const std::vector< shared_ptr<const AnaTrack> >& track_collection, vector<int> &truth_flags )
{
  // histogram the number of truth tracks processed.
  // {
  //   dg::down( study_name + "_st_" + track_name + "_wrt_" + truth_name , track_name + " efficiency w.r.t. " + truth_name );
  //   dg::fillh( "n_truth_tracks_input" , 500 , 0 , 500 , truth_collection.size() );
  //   dg::up();
  // }
  // loop over all truth tracks in truth_collection and look for matching reconstructed tracks in track_collection.
  unsigned int n_matching = 0u;
  unsigned int n_multiple_matching = 0u;  

  vector<int>::const_iterator iMatchType = roads_matchtype.begin();

  BOOST_FOREACH( const shared_ptr<const AnaTrack>& truth , truth_collection ) {

    if (*iMatchType<=0) {
      // no associated road, skip
      truth_flags.push_back(-1);
      ++iMatchType;
      continue;
    }

    // apply some cuts to the truth tracks used for the comparison/efficiencies.
    // get the "type" of truth track./ detector::particle_type(
    // pdg_code ) returns detector::PROTON , ::KAON , ::PION , ::MUON ,
    // ::ELECTRON , etc.  convert these to a integer signed by the
    // charge of the particle.
    const int particle_type = truth->signed_truth_particle_type();
    // retrieve reconstructed tracks assigned the same barcode as this truth track and store them in a vector.
    // the remove_copy_if function copies tracks from "track_collection" into "matches" as long as the fourth
    // argument to the function is false. That argument,
    //    bind(&AnaTrack::unique_barcode,_1) != truth->unique_barcode()
    // calls the AnaTrack::unique_barcode function for the track and compares the barcode with truth->unique_barcode().
    // also calls the AnaTrack::barcode_matching_fraction and compares with AnaConfiguration::barcode_matching_minimum_cut().

    // make a match with a small threshold, just to cleanup the next match
    vector< shared_ptr<const AnaTrack> > preliminary_matches;
    std::remove_copy_if( track_collection.begin() , track_collection.end() , back_inserter(preliminary_matches) , 
                         bind(&AnaTrack::barcode,_1) != truth->barcode()
                         || bind(&AnaTrack::barcode_matching_fraction,_1) < .5
                         );

    vector< shared_ptr<const AnaTrack> > matches;    
    int ftktype(0);
    int max_ntruth_hits=0;

    int best_iMatchType(0);
      for (unsigned vbit=8;vbit;vbit>>=1) {
       if (*iMatchType & vbit) {
	// this is the highest bit set
	best_iMatchType = vbit;
	break;
      }
     }

    BOOST_FOREACH( const shared_ptr<const AnaTrack>& ftktrack , preliminary_matches ) {    
      unsigned ntruth_hits = count_real_hits(ftktrack);
      if (max_ntruth_hits < ntruth_hits)
        max_ntruth_hits = ntruth_hits;

      // if (ntruth_hits==PlaneMap->getNPlanes()-2)
      //ftktype |=8;

      if (ntruth_hits<PlaneMap->getNPlanes()-1) continue; // not enough
      // add the matched track
      matches.push_back(ftktrack);
     
      // check the category
      //if (ftktrack->ftk_is_majority()) {
      if (ftktrack->nmisses()) {
	// majority
	ftktype |= 1; // majority tracks
      }
      else {
	// full track
	if (ntruth_hits==PlaneMap->getNPlanes()) {
	  ftktype |= 8;
	}
	else {
          if (best_iMatchType&(4|8))
	    ftktype |= 4;
          if (best_iMatchType&(2|1))
	    ftktype |=2;
	}
      }
    }

    int best_ftkmatch(0);
    for (unsigned wbit=8;wbit;wbit>>=1) {
      if (ftktype & wbit) {
	// this is the highest bit set
	best_ftkmatch = wbit;
	break;
      }
    }

    // count the number of matches
    const unsigned int n_matches = matches.size();
    if( n_matches>0 ) { ++n_matching; }
    if( n_matches>1 ) { ++n_multiple_matching; }

    // update the number of matches for the next step
    truth_flags.push_back(n_matches);

    StudyTrackingEfficiency st_eff( study_name , track_name + " efficiency w.r.t. " + truth_name );
    st_eff.for_event( AnaEventMgr::current_event() );
    // if the reconstructed tracks are supposed to come from FTK, plot the tracking efficiency in 
    // a couple of categories separately: "full_fit" where AnaTrack::nmisses()==0 and 
    st_eff.add_constant_category( "b_daughters" , truth->is_truth_b_decay() );
    st_eff.add_constant_category( "tau_daughters" , truth->is_truth_tau_decay() );
    st_eff.add_constant_category( "z0_lt_21mm" , std::abs(truth->z0()) < 21. );
    st_eff.add_constant_category( "muon" , abs(truth->pdg_code()) == 13 );
    st_eff.add_constant_category( "pion" , abs(truth->pdg_code()) == 211 );
    st_eff.add_constant_category( "electron" , abs(truth->pdg_code()) == 11 );
    st_eff.add_constant_category( "barrel" , std::abs(truth->eta()) < 1.0 );
    // if desired, plot a single region separately for the barrel
    // tracks. this uses a crude geometric definition of a region: the
    // quadrant containing the phi of the intersection with the layer.
    //     if( false ) {
    //       const unsigned int region = 0;
    //       const unsigned int nlayers = 7;
    //       // the following code creates a category functor that is true if
    //       // the input track intersects all 7 of the layers of region
    //       // "region" in the barrel.  again, this uses a crude
    //       // approximation of the region, not a calculation of the strip
    //       // intersection and comparison with the map definition.
    //       using bind;
    //       using boost::format;
    //       st_eff.add_category( (format("region_%1%_only") % region).str() ,
    //                            bind( &std::bitset<8>::test , bind( &detector::ftkTestIntersectionCountInRegions , 
    //                                                                bind( StudyTrackingEfficiency::getTrack , _1 ) ,
    //                                                                nlayers ) , region ) );
    //     }
    // make plots for this track. fills plots in this directory plus
    // subdirectories for the "full"fit" and "majority_fit"
    // categories.
    st_eff.for_only( StudyTrackingEfficiency::type( truth , n_matches>0 ) );


    StudyTrackingEfficiency st_eff_any( "Any_track" , "FTK track efficiency w.r.t. good roads" );
    st_eff_any.for_event( AnaEventMgr::current_event() );
    st_eff_any.add_constant_category( "b_daughters" , truth->is_truth_b_decay() );
    st_eff_any.add_constant_category( "tau_daughters" , truth->is_truth_tau_decay() );
    st_eff_any.add_constant_category( "z0_lt_21mm" , std::abs(truth->z0()) < 21. );
    st_eff_any.add_constant_category( "muon" , abs(truth->pdg_code()) == 13 );
    st_eff_any.add_constant_category( "pion" , abs(truth->pdg_code()) == 211 );
    st_eff_any.add_constant_category( "electron" , abs(truth->pdg_code()) == 11 );
    st_eff_any.add_constant_category( "barrel" , std::abs(truth->eta()) < 1.0 );
    st_eff_any.add_constant_category( "endcap" , std::abs(truth->eta()) > 1.5 );
    st_eff_any.add_constant_category( "full" , (*iMatchType)==PlaneMap->getNPlanes());
    st_eff_any.add_constant_category( "maj" , (*iMatchType)==PlaneMap->getNPlanes()-1);
    st_eff_any.for_only( StudyTrackingEfficiency::type( truth , best_ftkmatch ) );

    StudyTrackingEfficiency st_eff_maj( "Maj_track" , "FTK track efficiency w.r.t. good roads" );
    st_eff_maj.for_event( AnaEventMgr::current_event() );
    st_eff_maj.add_constant_category( "b_daughters" , truth->is_truth_b_decay() );
    st_eff_maj.add_constant_category( "tau_daughters" , truth->is_truth_tau_decay() );
    st_eff_maj.add_constant_category( "z0_lt_21mm" , std::abs(truth->z0()) < 21. );
    st_eff_maj.add_constant_category( "muon" , abs(truth->pdg_code()) == 13 );
    st_eff_maj.add_constant_category( "pion" , abs(truth->pdg_code()) == 211 );
    st_eff_maj.add_constant_category( "electron" , abs(truth->pdg_code()) == 11 );
    st_eff_maj.add_constant_category( "barrel" , std::abs(truth->eta()) < 1.0 );
    st_eff_maj.add_constant_category( "endcap" , std::abs(truth->eta()) > 1.5 );
    st_eff_maj.add_constant_category( "full" , (*iMatchType)==PlaneMap->getNPlanes());
    st_eff_maj.add_constant_category( "maj" , (*iMatchType)==PlaneMap->getNPlanes()-1);
    st_eff_maj.for_only( StudyTrackingEfficiency::type( truth , best_ftkmatch&1 ) );

    StudyTrackingEfficiency st_eff_maj_full( "Maj_track_full" , "FTK track efficiency w.r.t. good roads" );
    st_eff_maj_full.for_event( AnaEventMgr::current_event() );
    st_eff_maj_full.add_constant_category( "b_daughters" , truth->is_truth_b_decay() );
    st_eff_maj_full.add_constant_category( "tau_daughters" , truth->is_truth_tau_decay() );
    st_eff_maj_full.add_constant_category( "z0_lt_21mm" , std::abs(truth->z0()) < 21. );
    st_eff_maj_full.add_constant_category( "muon" , abs(truth->pdg_code()) == 13 );
    st_eff_maj_full.add_constant_category( "pion" , abs(truth->pdg_code()) == 211 );
    st_eff_maj_full.add_constant_category( "electron" , abs(truth->pdg_code()) == 11 );
    st_eff_maj_full.add_constant_category( "barrel" , std::abs(truth->eta()) < 1.0 );
    st_eff_maj_full.add_constant_category( "endcap" , std::abs(truth->eta()) > 1.5 );
    //st_eff_maj_full.add_constant_category( "full" , (*iMatchType)==PlaneMap->getNPlanes());
    //st_eff_maj_full.add_constant_category( "maj" , (*iMatchType)==PlaneMap->getNPlanes()-1);
    st_eff_maj_full.for_only( StudyTrackingEfficiency::type( truth , (best_ftkmatch&1) && (best_iMatchType&8) ) );

    StudyTrackingEfficiency st_eff_maj_fullN( "Maj_track_fullN" , "FTK track efficiency w.r.t. good roads" );
    st_eff_maj_fullN.for_event( AnaEventMgr::current_event() );
    st_eff_maj_fullN.add_constant_category( "b_daughters" , truth->is_truth_b_decay() );
    st_eff_maj_fullN.add_constant_category( "tau_daughters" , truth->is_truth_tau_decay() );
    st_eff_maj_fullN.add_constant_category( "z0_lt_21mm" , std::abs(truth->z0()) < 21. );
    st_eff_maj_fullN.add_constant_category( "muon" , abs(truth->pdg_code()) == 13 );
    st_eff_maj_fullN.add_constant_category( "pion" , abs(truth->pdg_code()) == 211 );
    st_eff_maj_fullN.add_constant_category( "electron" , abs(truth->pdg_code()) == 11 );
    st_eff_maj_fullN.add_constant_category( "barrel" , std::abs(truth->eta()) < 1.0 );
    st_eff_maj_fullN.add_constant_category( "endcap" , std::abs(truth->eta()) > 1.5 );
    //st_eff_maj_fullN.add_constant_category( "full" , (*iMatchType)==PlaneMap->getNPlanes());
    //st_eff_maj_fullN.add_constant_category( "maj" , (*iMatchType)==PlaneMap->getNPlanes()-1);
    st_eff_maj_fullN.for_only( StudyTrackingEfficiency::type( truth , (best_ftkmatch&1) && (best_iMatchType&4) ) );

    StudyTrackingEfficiency st_eff_maj_majN( "Maj_track_majN" , "FTK track efficiency w.r.t. good roads" );
    st_eff_maj_majN.for_event( AnaEventMgr::current_event() );
    st_eff_maj_majN.add_constant_category( "b_daughters" , truth->is_truth_b_decay() );
    st_eff_maj_majN.add_constant_category( "tau_daughters" , truth->is_truth_tau_decay() );
    st_eff_maj_majN.add_constant_category( "z0_lt_21mm" , std::abs(truth->z0()) < 21. );
    st_eff_maj_majN.add_constant_category( "muon" , abs(truth->pdg_code()) == 13 );
    st_eff_maj_majN.add_constant_category( "pion" , abs(truth->pdg_code()) == 211 );
    st_eff_maj_majN.add_constant_category( "electron" , abs(truth->pdg_code()) == 11 );
    st_eff_maj_majN.add_constant_category( "barrel" , std::abs(truth->eta()) < 1.0 );
    st_eff_maj_majN.add_constant_category( "endcap" , std::abs(truth->eta()) > 1.5 );
    //st_eff_maj_majN.add_constant_category( "full" , (*iMatchType)==PlaneMap->getNPlanes());
    //st_eff_maj_majN.add_constant_category( "maj" , (*iMatchType)==PlaneMap->getNPlanes()-1);
    st_eff_maj_majN.for_only( StudyTrackingEfficiency::type( truth , (best_ftkmatch&1) && (best_iMatchType&2) ) );

    StudyTrackingEfficiency st_eff_maj_maj( "Maj_track_maj" , "FTK track efficiency w.r.t. good roads" );
    st_eff_maj_maj.for_event( AnaEventMgr::current_event() );
    st_eff_maj_maj.add_constant_category( "b_daughters" , truth->is_truth_b_decay() );
    st_eff_maj_maj.add_constant_category( "tau_daughters" , truth->is_truth_tau_decay() );
    st_eff_maj_maj.add_constant_category( "z0_lt_21mm" , std::abs(truth->z0()) < 21. );
    st_eff_maj_maj.add_constant_category( "muon" , abs(truth->pdg_code()) == 13 );
    st_eff_maj_maj.add_constant_category( "pion" , abs(truth->pdg_code()) == 211 );
    st_eff_maj_maj.add_constant_category( "electron" , abs(truth->pdg_code()) == 11 );
    st_eff_maj_maj.add_constant_category( "barrel" , std::abs(truth->eta()) < 1.0 );
    st_eff_maj_maj.add_constant_category( "endcap" , std::abs(truth->eta()) > 1.5 );
    st_eff_maj_maj.add_constant_category( "full" , (*iMatchType)==PlaneMap->getNPlanes());
    st_eff_maj_maj.add_constant_category( "maj" , (*iMatchType)==PlaneMap->getNPlanes()-1);
    st_eff_maj_maj.for_only( StudyTrackingEfficiency::type( truth , (best_ftkmatch&1) && (best_iMatchType&1) ) );

    StudyTrackingEfficiency st_eff_submaj( "SubMaj_track" , "FTK track efficiency w.r.t. good roads" );
    st_eff_submaj.for_event( AnaEventMgr::current_event() );
    st_eff_submaj.add_constant_category( "b_daughters" , truth->is_truth_b_decay() );
    st_eff_submaj.add_constant_category( "tau_daughters" , truth->is_truth_tau_decay() );
    st_eff_submaj.add_constant_category( "z0_lt_21mm" , std::abs(truth->z0()) < 21. );
    st_eff_submaj.add_constant_category( "muon" , abs(truth->pdg_code()) == 13 );
    st_eff_submaj.add_constant_category( "pion" , abs(truth->pdg_code()) == 211 );
    st_eff_submaj.add_constant_category( "electron" , abs(truth->pdg_code()) == 11 );
    st_eff_submaj.add_constant_category( "barrel" , std::abs(truth->eta()) < 1.0 );
    st_eff_submaj.add_constant_category( "endcap" , std::abs(truth->eta()) > 1.5 );
    //st_eff_submaj.add_constant_category( "full" , (*iMatchType)==PlaneMap->getNPlanes());
    //st_eff_submaj.add_constant_category( "maj" , (*iMatchType)==PlaneMap->getNPlanes()-1);
    st_eff_submaj.for_only( StudyTrackingEfficiency::type( truth , max_ntruth_hits==PlaneMap->getNPlanes()-2 ) );

    StudyTrackingEfficiency st_eff_fullRoadN( "Maj_fullRoadN" , "FTK track efficiency w.r.t. good roads" );
    st_eff_fullRoadN.for_event( AnaEventMgr::current_event() );
    st_eff_fullRoadN.add_constant_category( "b_daughters" , truth->is_truth_b_decay() );
    st_eff_fullRoadN.add_constant_category( "tau_daughters" , truth->is_truth_tau_decay() );
    st_eff_fullRoadN.add_constant_category( "z0_lt_21mm" , std::abs(truth->z0()) < 21. );
    st_eff_fullRoadN.add_constant_category( "muon" , abs(truth->pdg_code()) == 13 );
    st_eff_fullRoadN.add_constant_category( "pion" , abs(truth->pdg_code()) == 211 );
    st_eff_fullRoadN.add_constant_category( "electron" , abs(truth->pdg_code()) == 11 );
    st_eff_fullRoadN.add_constant_category( "barrel" , std::abs(truth->eta()) < 1.0 );
    st_eff_fullRoadN.add_constant_category( "endcap" , std::abs(truth->eta()) > 1.5 );
    st_eff_fullRoadN.add_constant_category( "full" , (*iMatchType)==PlaneMap->getNPlanes());
    st_eff_fullRoadN.add_constant_category( "maj" , (*iMatchType)==PlaneMap->getNPlanes()-1);
    st_eff_fullRoadN.for_only( StudyTrackingEfficiency::type( truth , best_ftkmatch&(2|4) ) );

    StudyTrackingEfficiency st_eff_fullRoadN_full( "Maj_fullRoadN_full" , "FTK track efficiency w.r.t. good roads" );
    st_eff_fullRoadN_full.for_event( AnaEventMgr::current_event() );
    st_eff_fullRoadN_full.add_constant_category( "b_daughters" , truth->is_truth_b_decay() );
    st_eff_fullRoadN_full.add_constant_category( "tau_daughters" , truth->is_truth_tau_decay() );
    st_eff_fullRoadN_full.add_constant_category( "z0_lt_21mm" , std::abs(truth->z0()) < 21. );
    st_eff_fullRoadN_full.add_constant_category( "muon" , abs(truth->pdg_code()) == 13 );
    st_eff_fullRoadN_full.add_constant_category( "pion" , abs(truth->pdg_code()) == 211 );
    st_eff_fullRoadN_full.add_constant_category( "electron" , abs(truth->pdg_code()) == 11 );
    st_eff_fullRoadN_full.add_constant_category( "barrel" , std::abs(truth->eta()) < 1.0 );
    st_eff_fullRoadN_full.add_constant_category( "endcap" , std::abs(truth->eta()) > 1.5 );
    //st_eff_fullRoadN_full.add_constant_category( "full" , (*iMatchType)==PlaneMap->getNPlanes());
    //st_eff_fullRoadN_full.add_constant_category( "maj" , (*iMatchType)==PlaneMap->getNPlanes()-1);
    st_eff_fullRoadN_full.for_only( StudyTrackingEfficiency::type( truth , (best_ftkmatch&4) && (best_iMatchType&8) ) );

    StudyTrackingEfficiency st_eff_fullRoadN_fullN( "Maj_fullRoadN_fullN" , "FTK track efficiency w.r.t. good roads" );
    st_eff_fullRoadN_fullN.for_event( AnaEventMgr::current_event() );
    st_eff_fullRoadN_fullN.add_constant_category( "b_daughters" , truth->is_truth_b_decay() );
    st_eff_fullRoadN_fullN.add_constant_category( "tau_daughters" , truth->is_truth_tau_decay() );
    st_eff_fullRoadN_fullN.add_constant_category( "z0_lt_21mm" , std::abs(truth->z0()) < 21. );
    st_eff_fullRoadN_fullN.add_constant_category( "muon" , abs(truth->pdg_code()) == 13 );
    st_eff_fullRoadN_fullN.add_constant_category( "pion" , abs(truth->pdg_code()) == 211 );
    st_eff_fullRoadN_fullN.add_constant_category( "electron" , abs(truth->pdg_code()) == 11 );
    st_eff_fullRoadN_fullN.add_constant_category( "barrel" , std::abs(truth->eta()) < 1.0 );
    st_eff_fullRoadN_fullN.add_constant_category( "endcap" , std::abs(truth->eta()) > 1.5 );
    //st_eff_fullRoadN_full.add_constant_category( "full" , (*iMatchType)==PlaneMap->getNPlanes());
    //st_eff_fullRoadN_full.add_constant_category( "maj" , (*iMatchType)==PlaneMap->getNPlanes()-1);
    st_eff_fullRoadN_fullN.for_only( StudyTrackingEfficiency::type( truth , (best_ftkmatch&4) && (best_iMatchType&4) ) );

    StudyTrackingEfficiency st_eff_fullRoadN_majN( "Maj_fullRoadN_majN" , "FTK track efficiency w.r.t. good roads" );
    st_eff_fullRoadN_majN.for_event( AnaEventMgr::current_event() );
    st_eff_fullRoadN_majN.add_constant_category( "b_daughters" , truth->is_truth_b_decay() );
    st_eff_fullRoadN_majN.add_constant_category( "tau_daughters" , truth->is_truth_tau_decay() );
    st_eff_fullRoadN_majN.add_constant_category( "z0_lt_21mm" , std::abs(truth->z0()) < 21. );
    st_eff_fullRoadN_majN.add_constant_category( "muon" , abs(truth->pdg_code()) == 13 );
    st_eff_fullRoadN_majN.add_constant_category( "pion" , abs(truth->pdg_code()) == 211 );
    st_eff_fullRoadN_majN.add_constant_category( "electron" , abs(truth->pdg_code()) == 11 );
    st_eff_fullRoadN_majN.add_constant_category( "barrel" , std::abs(truth->eta()) < 1.0 );
    st_eff_fullRoadN_majN.add_constant_category( "endcap" , std::abs(truth->eta()) > 1.5 );
    //st_eff_fullRoadN_full.add_constant_category( "full" , (*iMatchType)==PlaneMap->getNPlanes());
    //st_eff_fullRoadN_full.add_constant_category( "maj" , (*iMatchType)==PlaneMap->getNPlanes()-1);
    st_eff_fullRoadN_majN.for_only( StudyTrackingEfficiency::type( truth , (best_ftkmatch&2) && (best_iMatchType&2) ) ); 

    StudyTrackingEfficiency st_eff_fullRoadN_maj( "Maj_fullRoadN_maj" , "FTK track efficiency w.r.t. good roads" );
    st_eff_fullRoadN_maj.for_event( AnaEventMgr::current_event() );
    st_eff_fullRoadN_maj.add_constant_category( "b_daughters" , truth->is_truth_b_decay() );
    st_eff_fullRoadN_maj.add_constant_category( "tau_daughters" , truth->is_truth_tau_decay() );
    st_eff_fullRoadN_maj.add_constant_category( "z0_lt_21mm" , std::abs(truth->z0()) < 21. );
    st_eff_fullRoadN_maj.add_constant_category( "muon" , abs(truth->pdg_code()) == 13 );
    st_eff_fullRoadN_maj.add_constant_category( "pion" , abs(truth->pdg_code()) == 211 );
    st_eff_fullRoadN_maj.add_constant_category( "electron" , abs(truth->pdg_code()) == 11 );
    st_eff_fullRoadN_maj.add_constant_category( "barrel" , std::abs(truth->eta()) < 1.0 );
    st_eff_fullRoadN_maj.add_constant_category( "endcap" , std::abs(truth->eta()) > 1.5 );
    //st_eff_fullRoadN_full.add_constant_category( "full" , (*iMatchType)==PlaneMap->getNPlanes());
    //st_eff_fullRoadN_full.add_constant_category( "maj" , (*iMatchType)==PlaneMap->getNPlanes()-1);
    st_eff_fullRoadN_maj.for_only( StudyTrackingEfficiency::type( truth , (best_ftkmatch&2) && (best_iMatchType&1) ) );       

    StudyTrackingEfficiency st_eff_full( "Full_track" , "FTK track efficiency w.r.t. good roads" );
    st_eff_full.for_event( AnaEventMgr::current_event() );
    st_eff_full.add_constant_category( "b_daughters" , truth->is_truth_b_decay() );
    st_eff_full.add_constant_category( "tau_daughters" , truth->is_truth_tau_decay() );
    st_eff_full.add_constant_category( "z0_lt_21mm" , std::abs(truth->z0()) < 21. );
    st_eff_full.add_constant_category( "muon" , abs(truth->pdg_code()) == 13 );
    st_eff_full.add_constant_category( "pion" , abs(truth->pdg_code()) == 211 );
    st_eff_full.add_constant_category( "electron" , abs(truth->pdg_code()) == 11 );
    st_eff_full.add_constant_category( "barrel" , std::abs(truth->eta()) < 1.0 );
    st_eff_full.add_constant_category( "endcap" , std::abs(truth->eta()) > 1.5 );
    st_eff_full.add_constant_category( "full" , (*iMatchType)==PlaneMap->getNPlanes());
    st_eff_full.add_constant_category( "maj" , (*iMatchType)==PlaneMap->getNPlanes()-1);
    st_eff_full.for_only( StudyTrackingEfficiency::type( truth , best_ftkmatch&8 ) );

    StudyTrackingEfficiency st_eff_full_full( "Full_track_full" , "FTK track efficiency w.r.t. good roads" );
    st_eff_full_full.for_event( AnaEventMgr::current_event() );
    st_eff_full_full.add_constant_category( "b_daughters" , truth->is_truth_b_decay() );
    st_eff_full_full.add_constant_category( "tau_daughters" , truth->is_truth_tau_decay() );
    st_eff_full_full.add_constant_category( "z0_lt_21mm" , std::abs(truth->z0()) < 21. );
    st_eff_full_full.add_constant_category( "muon" , abs(truth->pdg_code()) == 13 );
    st_eff_full_full.add_constant_category( "pion" , abs(truth->pdg_code()) == 211 );
    st_eff_full_full.add_constant_category( "electron" , abs(truth->pdg_code()) == 11 );
    st_eff_full_full.add_constant_category( "barrel" , std::abs(truth->eta()) < 1.0 );
    st_eff_full_full.add_constant_category( "endcap" , std::abs(truth->eta()) > 1.5 );
    //st_eff_full_full.add_constant_category( "full" , (*iMatchType)==PlaneMap->getNPlanes());
    //st_eff_full_full.add_constant_category( "maj" , (*iMatchType)==PlaneMap->getNPlanes()-1);
    st_eff_full_full.for_only( StudyTrackingEfficiency::type( truth , (best_ftkmatch&8) && (best_iMatchType&8) ) );

    StudyTrackingEfficiency st_eff_full_fullN( "Full_track_fullN" , "FTK track efficiency w.r.t. good roads" );
    st_eff_full_fullN.for_event( AnaEventMgr::current_event() );
    st_eff_full_fullN.add_constant_category( "b_daughters" , truth->is_truth_b_decay() );
    st_eff_full_fullN.add_constant_category( "tau_daughters" , truth->is_truth_tau_decay() );
    st_eff_full_fullN.add_constant_category( "z0_lt_21mm" , std::abs(truth->z0()) < 21. );
    st_eff_full_fullN.add_constant_category( "muon" , abs(truth->pdg_code()) == 13 );
    st_eff_full_fullN.add_constant_category( "pion" , abs(truth->pdg_code()) == 211 );
    st_eff_full_fullN.add_constant_category( "electron" , abs(truth->pdg_code()) == 11 );
    st_eff_full_fullN.add_constant_category( "barrel" , std::abs(truth->eta()) < 1.0 );
    st_eff_full_fullN.add_constant_category( "endcap" , std::abs(truth->eta()) > 1.5 );
    //st_eff_full_full_fullN.add_constant_category( "full" , (*iMatchType)==PlaneMap->getNPlanes());
    //st_eff_full_full_fullN.add_constant_category( "maj" , (*iMatchType)==PlaneMap->getNPlanes()-1);
    st_eff_full_fullN.for_only( StudyTrackingEfficiency::type( truth , (best_ftkmatch&8) && (best_iMatchType&4) ) );

    StudyTrackingEfficiency st_eff_full_majN( "Full_track_majN" , "FTK track efficiency w.r.t. good roads" );
    st_eff_full_majN.for_event( AnaEventMgr::current_event() );
    st_eff_full_majN.add_constant_category( "b_daughters" , truth->is_truth_b_decay() );
    st_eff_full_majN.add_constant_category( "tau_daughters" , truth->is_truth_tau_decay() );
    st_eff_full_majN.add_constant_category( "z0_lt_21mm" , std::abs(truth->z0()) < 21. );
    st_eff_full_majN.add_constant_category( "muon" , abs(truth->pdg_code()) == 13 );
    st_eff_full_majN.add_constant_category( "pion" , abs(truth->pdg_code()) == 211 );
    st_eff_full_majN.add_constant_category( "electron" , abs(truth->pdg_code()) == 11 );
    st_eff_full_majN.add_constant_category( "barrel" , std::abs(truth->eta()) < 1.0 );
    st_eff_full_majN.add_constant_category( "endcap" , std::abs(truth->eta()) > 1.5 );
    st_eff_full_majN.add_constant_category( "full" , (*iMatchType)==PlaneMap->getNPlanes());
    st_eff_full_majN.add_constant_category( "maj" , (*iMatchType)==PlaneMap->getNPlanes()-1);
    st_eff_full_majN.for_only( StudyTrackingEfficiency::type( truth , (best_ftkmatch&8) && (best_iMatchType&2) ) );

    StudyTrackingEfficiency st_eff_notrack( "No track" , "FTK track efficiency w.r.t. good roads" );
    st_eff_notrack.for_event( AnaEventMgr::current_event() );
    st_eff_notrack.add_constant_category( "b_daughters" , truth->is_truth_b_decay() );
    st_eff_notrack.add_constant_category( "tau_daughters" , truth->is_truth_tau_decay() );
    st_eff_notrack.add_constant_category( "z0_lt_21mm" , std::abs(truth->z0()) < 21. );
    st_eff_notrack.add_constant_category( "muon" , abs(truth->pdg_code()) == 13 );
    st_eff_notrack.add_constant_category( "pion" , abs(truth->pdg_code()) == 211 );
    st_eff_notrack.add_constant_category( "electron" , abs(truth->pdg_code()) == 11 );
    st_eff_notrack.add_constant_category( "barrel" , std::abs(truth->eta()) < 1.0 );
    st_eff_notrack.add_constant_category( "endcap" , std::abs(truth->eta()) > 1.5 );
    st_eff_notrack.add_constant_category( "full" , (*iMatchType)==PlaneMap->getNPlanes());
    st_eff_notrack.add_constant_category( "maj" , (*iMatchType)==PlaneMap->getNPlanes()-1);
    st_eff_notrack.for_only( StudyTrackingEfficiency::type( truth , !best_ftkmatch ) );    


    ++iMatchType;
  } // end for each good truth track
  dg::fillh( "n_matching_"+study_name+"_tracks" , 20 , 0 , 20 , 
             n_matching , "N " + truth_name + " WITH " + track_name + " MATCH" );
  dg::fillh( "n_multiple_"+study_name+"_tracks" , 20 , 0 , 20 , 
             n_multiple_matching , "N " + truth_name + " WITH MULTIPLE " + track_name + " MATCHES" );
  
} // end study_trackfitter_efficiency



