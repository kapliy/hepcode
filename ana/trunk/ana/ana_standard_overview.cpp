
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
#include "TrigFTKAna/StudyJet.hpp"
#ifdef HAVE_LIBFTKSIM
#include "TrigFTKSim/MultiTruth.h"
#endif

using namespace std;
using namespace DataGraphics;
using boost::bind;
using boost::shared_ptr;

// define some functions. each makes a set of plots for given track input.
void standard_truth_comparison( const std::string& study_name , const std::string& truth_name , const std::string& track_name ,
                                const std::vector< shared_ptr<const AnaTrack> >& truth_collection , const std::vector< shared_ptr<const AnaTrack> >& track_collection ,
				float vx = 0. , float vy = 0. , float vz = 0. );
void standard_track_comparison( const std::string& ref_name , const std::string& track_name , 
                                const std::vector< shared_ptr<const AnaTrack> >& track_collection_ref ,
                                const std::vector< shared_ptr<const AnaTrack> >& track_collection_comp ,
				float vx = 0. , float vy = 0. , float vz = 0. );
void standard_fake_comparison( const std::string& ref_name , const std::string& track_name , 
			       const std::vector< shared_ptr<const AnaTrack> >& track_collection_ref ,
			       const std::vector< shared_ptr<const AnaTrack> >& track_collection_comp );
void old_track_comparison( const std::string& ref_name , const std::string& track_name ,
                           const std::vector< shared_ptr<const AnaTrack> >& track_collection_ref ,
                           const std::vector< shared_ptr<const AnaTrack> >& track_collection_comp );
void old_fake_comparison( const std::string& ref_name ,
			  const std::string& track_name ,
			  const std::vector< shared_ptr<const AnaTrack> >& track_collection_ref ,
			  const std::vector< shared_ptr<const AnaTrack> >& track_collection_comp );
void plot_track_parameter_differences( const std::string& label_name , const shared_ptr<const AnaTrack>& reference_track , 
                                       const shared_ptr<const AnaTrack>& comparison_track , float vx = 0. , float vy = 0. , float vz = 0. );
void plot_track_parameter_diff_detail( const std::string& label_name , const shared_ptr<const AnaTrack>& reference_track ,
                                       const shared_ptr<const AnaTrack>& comparison_track );

void study_jet_collection( const std::string& study_name , const std::vector< shared_ptr<const AnaJet> >& jets );

void study_si_channels( const std::string& study_name , const std::vector< shared_ptr<const AnaSiChannel> >& si_channels );

#ifdef HAVE_LIBFTKSIM
void road_truth_association( const vector< boost::shared_ptr<const AnaRoad> >& roads, 
			     const vector< boost::shared_ptr<const AnaTrack> >& truth_tracks,
			     map< shared_ptr<const AnaRoad>, set< shared_ptr<const AnaTrack> > >& road_truth_map,
			     map< shared_ptr<const AnaTrack>, set< shared_ptr<const AnaRoad> > >& truth_road_map,
			     map< pair< shared_ptr<const AnaRoad>, shared_ptr<const AnaTrack> >, unsigned int >& n_ss_contributed );

void road_truth_study( const std::string& dir_name, 
		       const vector< boost::shared_ptr<const AnaTrack> >& truth_tracks_primary, 
		       const map< shared_ptr<const AnaRoad>, set< shared_ptr<const AnaTrack> > >& road_truth_map,
		       const map< shared_ptr<const AnaTrack>, set< shared_ptr<const AnaRoad> > >& truth_road_map,
		       const map< pair< shared_ptr<const AnaRoad>, shared_ptr<const AnaTrack> >, unsigned int >& n_ss_contributed );

void road_study( const vector< boost::shared_ptr<const AnaRoad> >& ftk_roads, const std::string& dir_name, 
		 const map< shared_ptr<const AnaRoad>, set< shared_ptr<const AnaTrack> > >& road_truth_map,
		 const map< shared_ptr<const AnaTrack>, set< shared_ptr<const AnaRoad> > >& truth_road_map,
		 const map< pair< shared_ptr<const AnaRoad>, shared_ptr<const AnaTrack> >, unsigned int >& n_ss_contributed );
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
  AnaConfiguration::configure("ana_standard_overview",argc,argv);
  if( !AnaConfiguration::configured() ) { exit(0); }
  dg::root()->no_save_at_exit();
  BOOST_SCOPE_EXIT() { dg::root()->do_save_at_exit(); } BOOST_SCOPE_EXIT_END;

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
  for( AnaEventMgr::iterator i=evtmgr->begin(), f=evtmgr->end(AnaConfiguration::user_max_nevents()); i!=f; ++i ) {

    const shared_ptr<const AnaEvent>& evt( *i );
    if( !evt ) { break; } // no AnaEvent means something went wrong, e.g. premature end of file

    // Workaround to skip athena/CBNT problems
    if( evt->inconsistentCBNT ) { continue; }

    // if( false ) { // make some plots for mel and exit
    //   study_jet_collection( "cone_4_tower" , evt->atlas_cone_4_tower_jets() );
    //   study_jet_collection( "cone_7_tower" , evt->atlas_cone_7_tower_jets() );
    //   study_jet_collection( "cone_4_truth" , evt->atlas_cone_4_truth_jets() );
    //   study_jet_collection( "cone_4_truth_pileup" , evt->atlas_cone_4_truth_pileup_jets() );
    //   continue;
    // }

    if( AnaConfiguration::do_si_channel_studies() ) { study_si_channels( "si_channels" , evt->si_channels(AnaEventMgr::DEFAULT_SOURCE) ); }

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
 
    // Used to study the FTK fake rate relative to offline (get rid of the primary vtx z0 cut)
    vector< shared_ptr<const AnaTrack> > offline_tracks_forfakes;
    offline_tracks.reserve( offline_tracks_all.size() );
    std::remove_copy_if( offline_tracks_all.begin() , offline_tracks_all.end() , back_inserter(offline_tracks_forfakes) ,
			 bind(&AnaTrack::chi2ndof,_1)  > 10 || // FIXME: restore this cut when not running on wrapper offline tracks
			 bind(&AnaTrack::nhits,_1) < 9 ||
			 bind(&AnaTrack::pixel_nholes,_1) > 0 ||
			 bind(&AnaTrack::sct_nholes,_1) > 2 ||
                         //bind( &AnaConfiguration::phi_fails_cut , bind( &AnaTrack::phi0 , _1 ) ) == true ||
                         bind(&AnaTrack::pt,_1) < AnaConfiguration::minimum_track_pt_cut() ||
                         bind( std::ptr_fun<AnaTrack::ftype,AnaTrack::ftype>(std::abs) , bind(&AnaTrack::d0,_1)) > AnaConfiguration::maximum_track_absd0_cut() ||
                         bind( std::ptr_fun<AnaTrack::ftype,AnaTrack::ftype>(std::abs) , bind(&AnaTrack::eta,_1)) > AnaConfiguration::maximum_track_abseta_cut() );
 
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

    vector< shared_ptr<const AnaTrack> > pileup_tracks;
    std::remove_copy_if( truth_tracks_all.begin() , truth_tracks_all.end() , back_inserter(pileup_tracks) ,
                         bind(&AnaTrack::detpaper,_1) == false ||
                         bind( &AnaConfiguration::phi_fails_cut , bind( &AnaTrack::phi0 , _1 ) ) == true ||
                         bind( std::ptr_fun<AnaTrack::ftype,AnaTrack::ftype>(std::abs) , bind(&AnaTrack::eta,_1)) > AnaConfiguration::maximum_track_abseta_cut() ||
                         bind(&UniqueBarcode::event_index,bind(&AnaTrack::barcode,_1))==0 );
    vector< shared_ptr<const AnaTrack> > bdaughter_tracks;
    std::remove_copy_if( truth_tracks_all.begin() , truth_tracks_all.end() , back_inserter(bdaughter_tracks) ,
                         bind( std::ptr_fun<AnaTrack::ftype,AnaTrack::ftype>(std::abs) , bind(&AnaTrack::eta,_1)) > AnaConfiguration::maximum_track_abseta_cut() ||
                         bind( &AnaConfiguration::phi_fails_cut , bind( &AnaTrack::phi0 , _1 ) ) == true ||
                         bind(&AnaTrack::is_truth_b_decay,_1)==false );

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
    dg::filleff( "tracks_truth_empty" , truth_tracks_primary.empty() );
    dg::filleff( "tracks_truth_fiducial" , has_fiducial_truth );
    if( truth_tracks_primary.empty() ) { continue; }
    
    // make some standard plots of track quantities for each track.

    if( true ) { 
      dg::down( "all_tracks" , "standard plots for all tracks passing standard cuts" );  BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      // make standard track parameter plots for each FTK track with pT>=1 GeV
      StudyTrack ftk_study_track( "FTK" , "plots of TrigFTKSim tracks" );
      ftk_study_track.for_event( evt );
      ftk_study_track.do_cluster_study( AnaConfiguration::do_si_cluster_studies() );
      ftk_study_track.add_category("full_fits", bind(&AnaTrack::nmisses,_1) == 0 );
      ftk_study_track.add_category("single_miss", bind(&AnaTrack::nmisses,_1) == 1 );
      ftk_study_track.add_category("single_miss_maj", bind(&AnaTrack::ftk_is_majority,_1) );
      ftk_study_track.add_category("single_miss_maj_pix", bind(&AnaTrack::ftk_is_majority,_1) && bind(&AnaTrack::pixel_nhits,_1) == 2 );
      ftk_study_track.add_category("single_miss_maj_pix0", bind(&AnaTrack::ftk_is_majority,_1) && bind(&AnaTrack::b_layer_nhits,_1) == 0 );
      ftk_study_track.add_category("single_miss_maj_pix1", bind(&AnaTrack::ftk_is_majority,_1) &&
				   bind(&detector::ftkMissedPlane, bind(&AnaTrack::ftk_bitmask,_1),1));
      ftk_study_track.add_category("single_miss_maj_pix2", bind(&AnaTrack::ftk_is_majority,_1) &&
				   bind(&detector::ftkMissedPlane, bind(&AnaTrack::ftk_bitmask,_1),2));
      ftk_study_track.add_category("single_miss_maj_sct", bind(&AnaTrack::ftk_is_majority,_1) && bind(&AnaTrack::sct_nhits,_1) == 7 );
      ftk_study_track.add_category("single_miss_road", !bind(&AnaTrack::ftk_is_majority,_1) && bind(&AnaTrack::nmisses,_1) == 1 );
      ftk_study_track.add_category("single_miss_road_pix", !bind(&AnaTrack::ftk_is_majority,_1) && bind(&AnaTrack::pixel_nhits,_1) == 2 );
      ftk_study_track.add_category("single_miss_road_pix0", !bind(&AnaTrack::ftk_is_majority,_1) && bind(&AnaTrack::b_layer_nhits,_1) == 0 );
      ftk_study_track.add_category("single_miss_road_pix1", !bind(&AnaTrack::ftk_is_majority,_1) &&
				   bind(&detector::ftkMissedPlane, bind(&AnaTrack::ftk_bitmask,_1),1));
      ftk_study_track.add_category("single_miss_road_pix2", !bind(&AnaTrack::ftk_is_majority,_1) &&
				   bind(&detector::ftkMissedPlane, bind(&AnaTrack::ftk_bitmask,_1),2));
      ftk_study_track.add_category("single_miss_road_sct", !bind(&AnaTrack::ftk_is_majority,_1) && bind(&AnaTrack::sct_nhits,_1) == 7 );
      ftk_study_track.add_category("double_miss", bind(&AnaTrack::nmisses,_1) == 2 );
      ftk_study_track.add_category("double_miss0", bind(&AnaTrack::nmisses,_1) == 2 &&
				   bind(&detector::ftkMissedPlane, bind(&AnaTrack::ftk_bitmask,_1),6));
      ftk_study_track.add_category("double_miss1", bind(&AnaTrack::nmisses,_1) == 2 &&
				   bind(&detector::ftkMissedPlane, bind(&AnaTrack::ftk_bitmask,_1),8));
      ftk_study_track.add_category("double_miss2", bind(&AnaTrack::nmisses,_1) == 2 &&
				   bind(&detector::ftkMissedPlane, bind(&AnaTrack::ftk_bitmask,_1),10));
      ftk_study_track.add_category("double_miss3", bind(&AnaTrack::nmisses,_1) == 2 &&
				   bind(&detector::ftkMissedPlane, bind(&AnaTrack::ftk_bitmask,_1),12));
      ftk_study_track.for_each( ftk_tracks.begin() , ftk_tracks.end() );
      
      // make standard track parameter plots for each offline track
      StudyTrack study_track("OFFLINE" , "plots of offline tracks" );
      study_track.for_event( evt );
      study_track.do_cluster_study( AnaConfiguration::do_si_cluster_studies() );
      study_track.for_each( offline_tracks.begin() , offline_tracks.end() );
      study_track.repurpose( "OFFLINE_FSTKLIKE" , "plots of offline FTK-like tracks" ); // Extra S is so that this string doesn't contain FTK
      study_track.for_each( offline_ftklike_tracks.begin() , offline_ftklike_tracks.end() );
      // make standard track parameter plots for each truth track
      study_track.repurpose( "TRUTH" , "plots of truth tracks" );
      study_track.for_each( truth_tracks_primary.begin() , truth_tracks_primary.end() );
      // make standard track parameter plots for each truth track
      study_track.repurpose( "TRUTH_PILEUP" , "plots of truth pileup tracks" );
      study_track.for_each_if( pileup_tracks.begin() , pileup_tracks.end() , bind(&AnaTrack::pt,_1) >= AnaConfiguration::minimum_track_pt_cut() );
      // make standard track parameter plots for each b decay truth track
      study_track.repurpose( "TRUTH_BDECAY" , "plots of truth b daughter tracks" );
      study_track.for_each_if( bdaughter_tracks.begin() , bdaughter_tracks.end() , bind(&AnaTrack::pt,_1) >= AnaConfiguration::minimum_track_pt_cut() );

      // FIXME:
      // ALL
      /*
        study_track.repurpose( "TRUTH_ALL" , "plots of ALL truth tracks" );
        study_track.for_each_if( truth_tracks_all.begin() , truth_tracks_all.end() , bind(&AnaTrack::pt,_1) >= AnaConfiguration::minimum_track_pt_cut() );
        study_track.repurpose( "OFFLINE_ALL" , "plots of ALL offline tracks" );
        study_track.for_each_if( offline_tracks_all.begin() , offline_tracks_all.end() , bind(&AnaTrack::pt,_1) >= AnaConfiguration::minimum_track_pt_cut() );
      */

    }

    // plot beamline position
    float x0beamline = 0.;
    float y0beamline = 0.;
    float z0beamline = 0.;
    if( true && evt->beamline() ) {
      const shared_ptr<const AnaBeamline>& beamline = evt->beamline();
      x0beamline = beamline->x0();
      y0beamline = beamline->y0();
      z0beamline = beamline->z0();
    }
    dg::fillh( "beamspot_x" , 100 , -1 , 1 , x0beamline/10. , "BEAMSPOT X (cm)" );
    dg::fillh( "beamspot_y" , 100 , -1 , 1 , y0beamline/10. , "BEAMSPOT Y (cm)" );
    dg::fillh( "beamspot_z" , 100 , -5 , 5 , z0beamline/10. , "BEAMSPOT Z (cm)" );

    // if desired, print each track to stdout for closer inspection.
    if( false ) { 
      cout << "================================================================" << endl;
      cout << "                      NEW EVENT" << endl;
      cout << "      " << AnaEventMgr::current_event()->run_number() << " " << AnaEventMgr::current_event()->event_number() << endl;
      cout << "================================================================" << endl;
      cout << " PRIMARY TRUTH TRACKS " << endl;
      std::for_each( truth_tracks_primary.begin() , truth_tracks_primary.end() , bind(&AnaTrack::print,_1) );
      cout << " OFFLINE TRACKS " << endl;
      std::for_each( offline_tracks.begin() , offline_tracks.end() , bind(&AnaTrack::print,_1) );
      cout << " FTK TRACKS " << endl;
      std::for_each( ftk_tracks.begin() , ftk_tracks.end() , bind(&AnaTrack::print,_1) );
    }

    // Get the 10/11 and 11/11 coverage for roads - this is only
    // accurate if the true tracks are single muons.
    const vector< shared_ptr<const AnaRoad> >& ftk_roads(evt->ftk_roads(AnaEventMgr::DEFAULT_SOURCE));
    if( truth_tracks_primary.size() == 1 ) {
      dg::down( "coverage", "coverage plots" );
      const shared_ptr<const AnaTrack>& truth(*truth_tracks_primary.begin());
      {
        dg::filleff( "coverage_full" , std::count_if( ftk_roads.begin() , ftk_roads.end() , bind(&AnaRoad::n_hits,_1) == evt->ftk_road_nplanes() ) );
        dg::filleff ("coverage" , ftk_roads.size() != 0 );
        dg::filleff( "coverage_full_eta" , 60, -3, 3, truth->eta(), std::count_if( ftk_roads.begin() , ftk_roads.end() , bind(&AnaRoad::n_hits,_1) == evt->ftk_road_nplanes() ), "TRUTH #eta" );
        dg::filleff ("coverage_eta" , 60, -3, 3, truth->eta(), ftk_roads.size() != 0, "TRUTH #eta" );
	
        // if(std::abs(truth->z0()) < 21. ) { 
        //   dg::filleff ("coverage_z0_lt_21mm" , 60, -3, 3, truth->eta(), ftk_roads.size() != 0, "TRUTH #eta" ); 
        // }
	if( !ftk_roads.empty() ) {
	  // What is the probability for the roads with the greatest
	  // number of hits to be missing given layers?
	  // First, sort by number of hits
	  vector< shared_ptr<const AnaRoad> > sorted_ftk_roads(ftk_roads);
	  std::sort(sorted_ftk_roads.begin(),sorted_ftk_roads.end(), bind(&AnaRoad::n_hits,_1) > bind(&AnaRoad::n_hits,_2) );
	  const unsigned int maxnhits = (*sorted_ftk_roads.begin())->n_hits();
	  BOOST_FOREACH( const shared_ptr<const AnaRoad>& road , sorted_ftk_roads ) {
	    if( road->n_hits() < maxnhits ) break;
	    // Check how often a given layer is missing. For 11L and
	    // Option B config, the bitmask is always 11L.
	    for( unsigned int i = 0; i < (evt->ftk_road_nplanes()==7 ? 7 : 11); i++) {
	      dg::filleff( (boost::format("miss_layer_%d") % i).str(), 60, -3, 3, truth->eta(),
			   ((road->bitmask() & (1<<i)) == 0),
			   "TRUTH #eta" );
	      if( evt->ftk_road_nplanes() != 7 && 
		  i > 2 &&
		  i%2 == 0 ) {
		// Check for SCT missing pairs
		dg::filleff( (boost::format("sct_miss_pair_%d") % ((i-4)/2)).str(), 60, -3, 3, truth->eta(),
			     ((road->bitmask() & (1<<i)) == 0) && ((road->bitmask() & (1<<(i-1))) == 0), 
			     "TRUTH #eta" );
	      }
	    }
	  }
	}
      }
      dg::up();
    }
    
    // if desired, fill plots comparing each tracking algorithm with truth tracks.
    if( true ) { 
      dg::down( "comparison_with_truth" , "comparing tracking algorithms with truth tracks" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      standard_truth_comparison( "FTK_VS_TRUTH" , "TRUTH" , "FTK" , truth_tracks_primary , ftk_tracks , x0beamline , y0beamline , z0beamline );
      standard_truth_comparison( "OFFLINE_VS_TRUTH" , "TRUTH" , "OFFLINE" , truth_tracks_primary , offline_tracks );
      standard_truth_comparison( "OFFLINE_FTKLIKE_VS_TRUTH" , "TRUTH" , "OFFLINE_FSTKLIKE" , truth_tracks_primary , offline_ftklike_tracks );
      if( AnaConfiguration::has_reference_source() ) { 
        const string refname( AnaConfiguration::source_to_treat_as_reference() );
        standard_truth_comparison( refname+"_VS_TRUTH" , "TRUTH" , refname , truth_tracks_primary , evt->ftk_tracks(refname) );
      }
      if( true ){ // do parameter-based matching. Resolution numbers from Guido.
        old_track_comparison( "TRUTH" , "FTK", truth_tracks_primary , ftk_tracks );
        old_track_comparison( "TRUTH" , "OFFLINE", truth_tracks_primary , offline_tracks );
        old_track_comparison( "TRUTH" , "OFFLINE_FSTKLIKE", truth_tracks_primary , offline_ftklike_tracks );
        old_fake_comparison( "TRUTH" , "FTK", truth_tracks_all , ftk_tracks );
        old_fake_comparison( "TRUTH" , "OFFLINE", truth_tracks_all , offline_tracks );
        old_fake_comparison( "TRUTH" , "OFFLINE_FSTKLIKE", truth_tracks_all , offline_ftklike_tracks );

        old_track_comparison( "OFFLINE" , "FTK", offline_tracks , ftk_tracks );
        old_track_comparison( "OFFLINE_FSTKLIKE" , "FTK", offline_ftklike_tracks , ftk_tracks );
        old_fake_comparison( "OFFLINE" , "FTK", offline_tracks_forfakes , ftk_tracks );
        old_fake_comparison( "OFFLINE_FSTKLIKE" , "FTK", offline_ftklike_tracks , ftk_tracks );
      }
    }

    if( true ) {
      // fill plots comparing each tracking algorithm with a reference reconstruction algorithm
      dg::down( "comparison_with_other_alg" , "comparing tracking algorithm with a reference reconstruction algorithm" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      if( true ) {
        dg::down( "ftk_new_vs_offline" , "new ftk tracks" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
        standard_track_comparison( "OFFLINE" , "FTK", offline_tracks , ftk_tracks );
        standard_track_comparison( "OFFLINE_FSTKLIKE" , "FTK", offline_ftklike_tracks , ftk_tracks );
      }
      if( AnaConfiguration::has_reference_source() ) { 
        const string& refname( AnaConfiguration::source_to_treat_as_reference() );
        standard_track_comparison( refname , "FTK" , evt->ftk_tracks(refname) , ftk_tracks );
      }
      // sanity checks
      if( false ) { 
        dg::down( "sanity_checks" , "track comparison sanity checks" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
        standard_track_comparison( "FTK" , "FTK" , ftk_tracks , ftk_tracks );
        standard_track_comparison( "OFFLINE" , "OFFLINE" , offline_tracks , offline_tracks );
        standard_track_comparison( "OFFLINE" , "TRUTH", offline_tracks , truth_tracks_primary );
      }
    }

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

    if( true ) {
      // make standard plots of road and fit multiplicities
      dg::down( "ftk_stats" , "FTK statistics" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      dg::fillh( "n_truth_tracks" , 600 , 0 , 600 , n_truth , "N TRUTH TRACKS" );
      dg::fillh( "n_offline_tracks" , 600 , 0 , 600 , n_offline , "N OFFLINE TRACKS" );
      dg::fillh( "n_offline_tracks_hardscatter" , 600 , 0 , 600 , n_offline_hardscatter , "N HARDSCATTER OFFLINE TRACKS" );
      dg::fillh( "n_offline_tracks_pileup" , 600 , 0 , 600 , n_offline_pileup , "N PILEUP OFFLINE TRACKS" );
      dg::fillh( "n_offline_tracks_fake" , 600 , 0 , 600 , n_offline_fake , "N FAKE OFFLINE TRACKS" );
      using namespace boost::assign;
      set<string> sources;
      sources += AnaEventMgr::DEFAULT_SOURCE;
      if( AnaConfiguration::has_reference_source() ) { sources += AnaConfiguration::source_to_treat_as_reference(); }
      const bool multiple_sources = sources.size()>1;
      for( set<string>::const_iterator is=sources.begin(), fs=sources.end(); is!=fs; ++is ) {
        if( multiple_sources ) { dg::down( *is , "FTK source" ); }
        const string& source( *is );
        dg::fillh( "n_ftk_tracks" , 500 , 0 , 100000 , n_ftk , "N (NEW) FTK TRACKS" );
        dg::fillh( "n_ftk_tracks_crate" , 500 , 0 , 12500 , n_ftk/8.0 , "N (NEW) FTK TRACKS PER CRATE" );
        dg::fillh( "n_ftk_tracks_incomplete" , 500 , 0 , 100000 , evt->ftk_tracks(source+"_incomplete").size() , "N STAGE1 FTK TRACKS" );
        dg::fillh( "n_ftk_tracks_crate_incomplete" , 500 , 0 , 12500 , evt->ftk_tracks(source+"_incomplete").size()/8.0 , "N STAGE1 FTK TRACKS PER CRATE" );
        dg::fillh( "n_roads_crate" , 500 , 0 , 75000 , evt->ftk_roads(source).size()/8.0 , "N ROADS PER CRATE");
        dg::fillh( "n_am_roads_crate" , 500 , 0 , 75000 , evt->ftk_roads(source+"_amroads").size()/8.0 , "N AM ROADS PER CRATE");
        dg::fillh( "n_roads" , 500 , 0 , 600000 , evt->ftk_roads(source).size() , "N ROADS");
        dg::fillh( "n_am_roads" , 500 , 0 , 600000 , evt->ftk_roads(source+"_amroads").size() , "N AM ROADS");
        dg::fillh( "n_fits" , 500 , 0 , 20000000 , evt->ftk_n_combinations(source) , "N FITS" );
        dg::fillh( "n_fits_crate" , 500 , 0 , 2500000 , evt->ftk_n_combinations(source)/8.0 , "N FITS PER CRATE" );
        dg::fillh( "n_fits_majority" , 500 , 0 , 20000000 , evt->ftk_n_fits_majority(source) , "N MAJORITY FITS" );
        dg::fillh( "n_fits_bad" , 500 , 0 , 20000000 , evt->ftk_n_fits_bad(source) , "N BAD FITS" );
        dg::fillh( "n_fits_pass_chi2" , 500 , 0 , 10000000 , (evt->ftk_n_fits(source)-evt->ftk_n_fits_bad(source)) , "N GOOD CHI2 FITS" );
        dg::fillh( "n_fits_pass_chi2_crate" , 500 , 0 , 1250000 , (evt->ftk_n_fits(source)-evt->ftk_n_fits_bad(source))/8.0 , "N GOOD CHI2 FITS PER CRATE" );
        dg::fillh( "n_fits_majority_bad" , 500 , 0 , 20000000 , evt->ftk_n_fits_majority_bad(source) , "N BAD MAJORITY FITS" );
        dg::fillh( "n_fits_hitwarrior_rejected" , 500 , 0 , 10000000 , evt->ftk_n_fits_hitwarrior_rejected(source) , "N FITS REJECTED BY HITWARRIOR" );
        dg::fillh( "n_fits_incomplete" , 500 , 0 , 20000000 , evt->ftk_n_combinations(source+"_incomplete") , "N STAGE1 FITS" );
        dg::fillh( "n_fits_crate_incomplete" , 500 , 0 , 2500000 , evt->ftk_n_combinations(source+"_incomplete")/8.0 , "N STAGE1 FITS PER CRATE" );
        dg::fillh( "n_fits_majority_incomplete" , 500 , 0 , 20000000 , evt->ftk_n_fits_majority(source+"_incomplete") , "N STAGE1 MAJORITY FITS" );
        dg::fillh( "n_fits_bad_incomplete" , 500 , 0 , 20000000 , evt->ftk_n_fits_bad(source+"_incomplete") , "N STAGE1 BAD FITS" );
        dg::fillh( "n_fits_pass_chi2_incomplete" , 500 , 0 , 10000000 , (evt->ftk_n_fits(source+"_incomplete")-evt->ftk_n_fits_bad(source)) , "N STAGE1 GOOD CHI2 FITS" );
        dg::fillh( "n_fits_pass_chi2_crate_incomplete" , 500 , 0 , 1250000 , (evt->ftk_n_fits(source+"_incomplete")-evt->ftk_n_fits_bad(source))/8.0 , "N STAGE1 GOOD CHI2 FITS PER CRATE" );
        dg::fillh( "n_fits_majority_bad_incomplete" , 500 , 0 , 20000000 , evt->ftk_n_fits_majority_bad(source+"_incomplete") , "N STAGE1 BAD MAJORITY FITS" );
        dg::fillh( "n_fits_hitwarrior_rejected_incomplete" , 500 , 0 , 10000000 , evt->ftk_n_fits_hitwarrior_rejected(source+"_incomplete") , "N STAGE1 FITS REJECTED BY HITWARRIOR" );

	// Clusters per layer:
	for(unsigned int ipl = 0; ipl < evt->ftk_road_nplanes(); ipl++ ) {
	  // Note: assuming first 3 planes are pixels
	  dg::fillh( boost::str(boost::format("n_ftk_clusters_layer%d")%ipl), 500, 0, ipl<3?40000:10000, evt->ftk_n_clusters(source)[ipl], 
		     boost::str(boost::format("N CLUSTERS LAYER %d")%ipl) );
	  dg::fillh( boost::str(boost::format("n_ftk_clusters_layer%d_crate")%ipl), 500, 0, ipl<3?5000:2000, evt->ftk_n_clusters(source)[ipl]/8.0, 
		     boost::str(boost::format("N CLUSTERS LAYER %d PER CRATE")%ipl) );
	}
        if( multiple_sources ) { dg::up(); }
      }
    } // end plot some statistics

#ifdef HAVE_LIBFTKSIM
    if( !(evt->ftk_roads(AnaEventMgr::DEFAULT_SOURCE).empty()) ) {
      // study relationship between tracks and roads.
      if( false ) {
	dg::down( "roads_for_tracks" , "roads for TrigFTKSim tracks" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
	BOOST_FOREACH( const shared_ptr<const AnaTrack>& track , ftk_tracks ) {
	  // if( track->barcode().barcode()!=-1 ) { continue; }
	  // cout << " track without correct truth assignment: " << endl;
	  // track->print();
	  // retrieve the road for this track
	  const unsigned int bank_id = track->ftk_bank_id();
	  const unsigned long road_id = track->ftk_road_id();
	  // cout << boost::format("%|5t| track bank %1% road %2% ids") % bank_id % road_id << endl;
	  // cout << (boost::format("%|5t| total nroads %1% nss %2%") % 
	  //          evt->ftk_roads(AnaEventMgr::DEFAULT_SOURCE).size() % evt->ftk_superstrips(AnaEventMgr::DEFAULT_SOURCE).size())
	  //      << endl;
	  // unsigned int nroads = 0u;
	  // BOOST_FOREACH( const shared_ptr<const AnaRoad>& road , evt->ftk_roads(AnaEventMgr::DEFAULT_SOURCE) ) {
	  //   cout << boost::format("%|5t| road #%|4d| %|6d| road %|6d| ids") % (nroads++) % road->bank_id() % road->road_id() << endl;
	  // }
	  // use a lookup table to find the right road, rather than loop over all roads. worst case, this takes the same amount of
	  // time as the single loop, but typically this is a faster constant time operation.
	  const shared_ptr<const AnaRoad>& road( evt->ftk_road_lookup( AnaEventMgr::DEFAULT_SOURCE , bank_id , road_id ) );
	  if( !road ) {
	    //cerr << (boost::format("Road lookup failed for bank_id %1% and road_id %2%.")%bank_id%road_id).str() << endl;
	    continue;
	  }
	  // if( road ) { 
	  //   cout << boost::format("%|5t| road from lookup bank %1% road %2% ids") % road->bank_id() % road->road_id() << endl;
	  // } else {
	  //   cout << boost::format("%|5t| road from lookup bank -- not found --") << endl;
	  // }
	  // loop over all ss for the road
	  // for( int i=0, f=detector::ftk_nlayers(); i!=f; ++i ) {
	  //   // if( !detector::ftkHasPlane(track->ftk_bitmask(),i) ) { continue; }
	  //   const shared_ptr<const AnaSuperstrip>& ss = evt->ftk_ss_lookup( AnaEvent::DEFAULT_SOURCE , i , road->superstrip_ids()[i] );
	  //   cout << " i: " << i << " ssid: " << road->superstrip_ids()[i]
	  //        << " bit: " << detector::ftkHasPlane(track->ftk_bitmask(),i)
	  //        << " has ss: " << (bool)ss
	  //        << endl;
	  // }
	  for( int i=0, f=detector::ftk_nlayers(), ilayer=0; i!=f; ++i ) {
	    if( !detector::ftkHasPlane(track->ftk_bitmask(),i) ) { continue; }
	    // get ss for this layer
	    //cout << " i: " << i << " ssid: " << road->superstrip_ids()[i] << endl;
	    const shared_ptr<const AnaSuperstrip>& ss = evt->ftk_ss_lookup( AnaEvent::DEFAULT_SOURCE , i , road->superstrip_ids()[i] );
	    ++ilayer;
	    if( !ss ) {
	      dg::fillh( "missing_ss_layer" , 14 , 0 , 14 , i );
	      dg::fillh( "missing_ss_id" , 20 , 0 , 40000 , road->superstrip_ids()[i] );
	      continue;
	    }
	    // cout << boost::format("%|10t| event ss %1% layer %2% nhits %3%") % ss->id() % ss->layer() % ss->nhits() << endl;
	    bool wrong_hit = false;
	    // find the hit attached from this superstrip
	    bool used_hit_correct_truth = false;
	    bool any_hit_correct_truth = false;
	    if( true ) { // get hit attached from this superstrip. does it have the correct truth?
	      const bool is_pixel_layer = i<detector::ftk_nlayers_pixels();
	      AnaTrack::ftype hitfx = track->ftk_coords()[detector::ftk_1st_coordinate_for_layer(i)];
	      AnaTrack::ftype hitfy = ( is_pixel_layer ? 
					track->ftk_coords()[detector::ftk_2nd_coordinate_for_layer(i)] :
					-9999. );
	      assert( std::distance( ss->begin_hits() , ss->end_hits() ) == ss->truth().size() );
	      // does any hit in the superstrip have the correct truth?
	      int cc = 0;
	      BOOST_FOREACH( const MultiTruth& mt , ss->truth() ) {
		MultiTruth::Barcode code;
		MultiTruth::Weight weight;
		const bool ok = mt.best( code , weight );
		if( ok && (code.first == track->barcode().event_index()) && 
		    (code.second == track->barcode().barcode()) ) {
		  any_hit_correct_truth = true;
		  // is this the hit used on the track?
		  AnaSuperstrip::HitVector::const_iterator iss = ss->begin_hits();
		  std::advance( iss , cc );
		  AnaSuperstrip::ftype ihitfx = (*iss).front();
		  AnaSuperstrip::ftype ihitfy = is_pixel_layer ? (*iss).back() : 99999.;
		  if( std::abs( ihitfx - hitfx ) < 0.5 && (!is_pixel_layer || (std::abs( ihitfy - hitfy ) < 0.5) ) ) { 
		    used_hit_correct_truth = true;
		  }
		}
		++cc;
	      }
	    }
	    // look at truth multiplicity for each hit in superstrip
	    BOOST_FOREACH( const MultiTruth& mt , ss->truth() ) {
	      // mt.display();
	      dg::fillh( "superstrip_mt_mult" , 20 , 0 , 20 , mt.multiplicity() , "Superstrip Truth Multiplicity" );
	      dg::fillh( "superstrip_mt_mult_vs_layer" , 11 , 0 , 11 , 20 , 0 , 20 , 
			 ss->layer() , mt.multiplicity() , "Superstrip Layer" , "Superstrip Truth Multiplicity" );
	    } // end for each hit truth on the superstrip
	    //dg::fillh( "superstrip_nhits" , 20 , 0 , 20 , ss->nhits() , "Superstrip Truth Multiplicity" );
	    dg::fillh( "ss_nhits_vs_layer" , 11 , 0 , 11 , 20 , 0 , 20 , ss->layer() , ss->nhits() , "Superstrip Layer" , "Superstrip Truth Multiplicity" );
	    dg::filleff( "superstrip_used_true_hit_vs_layer" , 11 , 0 , 11 , ss->layer() , used_hit_correct_truth , "Superstrip Layer" );//, "Eff. for Using Correct Truth" );
	    dg::filleff( "superstrip_any_true_hit_vs_layer" , 11 , 0 , 11 , ss->layer() , any_hit_correct_truth , "Superstrip Layer" );//, "Eff. for SS to Contain Truth Hit" );
	    dg::filleff( "superstrip_wrong_true_hit_vs_layer" , 11 , 0 , 11 , ss->layer() , any_hit_correct_truth && !used_hit_correct_truth , 
			 "Superstrip Layer" );//, "Rate for SS to Contain Truth Hit, Other Hit Used" );
	    if( ss->nhits()>1 ) { 
	      dg::filleff( "superstrip_used_true_hit_vs_layer_mult" , 11 , 0 , 11 , ss->layer() , used_hit_correct_truth , "Superstrip Layer" );//, "Eff. for Using Correct Truth" );
	      dg::filleff( "superstrip_any_true_hit_vs_layer_mult" , 11 , 0 , 11 , ss->layer() , any_hit_correct_truth , "Superstrip Layer" );//, "Eff. for SS to Contain Truth Hit" );
	      dg::filleff( "superstrip_wrong_true_hit_vs_layer_mult" , 11 , 0 , 11 , ss->layer() , any_hit_correct_truth && !used_hit_correct_truth , 
			   "Superstrip Layer" ); //, "Rate for SS to Contain Truth Hit, Other Hit Used" );
	    }
	  }
	  // cout << " road ss: " << endl;
	  // loop through all superstrips in the road and dump all hit truth.
	  // int ilayer=-1;
	  // BOOST_FOREACH( const unsigned long ssid , road->superstrip_ids() ) {
	  //   if( !detector::ftkHasPlane(track->ftk_bitmask(),ilayer) ) { continue; }
	  //   ++ilayer;
	  //   //cout << boost::format("%|15t| ss %1%") % ssid << endl;
	  //   const shared_ptr<const AnaSuperstrip>& ss = evt->ftk_ss_lookup(AnaEventMgr::DEFAULT_SOURCE,ilayer,ssid);
	  //   if( ss ) { 
	  //     cout << boost::format("%|15t| lookup ss %1% layer %2% nhits %3%") % ss->id() % ss->layer() % ss->nhits() << endl;
	  //   } else {
	  //     cout << boost::format("%|15t| lookup ss not found") << endl;
	  //   }
	  //   BOOST_FOREACH( const MultiTruth& mt , ss->truth() ) {
	  //     mt.display();
	  //   } // end for each hit truth on the superstrip
	  // } // end for each superstrip
	  //assert( !"stop!" );
	} // end for each track
      }
    } // If we have FTK Roads

    // Further road studies
    // Do standard road study

    if( true ) {
      // plot some crude road and superstrip properties.
      dg::down( "roads" , "FTK roads" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      
      // plot number of hits in each superstrip vs plane
      vector<unsigned int> nhits_per_layer;
      std::fill_n( back_inserter(nhits_per_layer) , 14 , 0 );
      BOOST_FOREACH( const shared_ptr<const AnaSuperstrip>& ss , evt->ftk_superstrips(AnaEventMgr::DEFAULT_SOURCE) ) {
	nhits_per_layer[ ss->layer() ] += ss->nhits();
	dg::fillh( "superstrip_nhits_vs_layer" , 14 , 0 , 14 , 5 , 0 , 5 , ss->layer() , ss->nhits() ,
		   "LAYER" , "HITS PER SUPERSTRIP" );
	dg::fillh( "superstrip_nhits" , 5 , 0 , 5 , ss->nhits() , "HITS PER SUPERSTRIP" );
      }
      for( unsigned int ilayer=0; ilayer!=14; ++ilayer ) {
	dg::fillh( "total_nhits_vs_layer_2d" , 14 , 0 , 14 , 100 , 0 , 5000 , ilayer , nhits_per_layer[ilayer] ,
		   "LAYER" , "TOTAL NUMBER OF HITS" );
	dg::fillf( "total_nhits_vs_layer_pf" , 14 , 0 , 14 , ilayer , nhits_per_layer[ilayer] , "LAYER" , "TOTAL NUMBER OF HITS" );
      }
      
      // Prepare road studies
      map< shared_ptr<const AnaRoad>, set< shared_ptr<const AnaTrack> > > road_truth_map;
      map< shared_ptr<const AnaTrack>, set< shared_ptr<const AnaRoad> > > truth_road_map;
      map< pair< shared_ptr<const AnaRoad>, shared_ptr<const AnaTrack> >, unsigned int > n_ss_contributed;
      road_truth_association( ftk_roads, truth_tracks_all, road_truth_map, truth_road_map, n_ss_contributed );

      road_truth_study( "ftk_roads", truth_tracks_primary, road_truth_map, truth_road_map, n_ss_contributed );
      road_study( ftk_roads, "ftk_roads", road_truth_map, truth_road_map, n_ss_contributed );

      road_truth_map.clear(); truth_road_map.clear(); n_ss_contributed.clear();
      road_truth_association(evt->ftk_roads(AnaEventMgr::DEFAULT_SOURCE+"_amroads"), truth_tracks_all, road_truth_map, truth_road_map, n_ss_contributed );

      road_truth_study( "ftk_am_roads", truth_tracks_primary, road_truth_map, truth_road_map, n_ss_contributed );
      road_study( evt->ftk_roads(AnaEventMgr::DEFAULT_SOURCE+"_amroads"), "ftk_am_roads", road_truth_map, truth_road_map, n_ss_contributed );
    }
#endif // HAVE_LIBFTKSIM
  } // end for each event

  AnaEventMgr::instance()->close_sample();

}

void
standard_truth_comparison( const std::string& study_name , const std::string& truth_name , const std::string& track_name ,
                           const std::vector< shared_ptr<const AnaTrack> >& truth_collection , const std::vector< shared_ptr<const AnaTrack> >& track_collection ,
			   float vx , float vy , float vz )
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
  BOOST_FOREACH( const shared_ptr<const AnaTrack>& truth , truth_collection ) {
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
    vector< shared_ptr<const AnaTrack> > matches;
    std::remove_copy_if( track_collection.begin() , track_collection.end() , back_inserter(matches) , 
                         bind(&AnaTrack::barcode,_1) != truth->barcode()
                         || bind(&AnaTrack::barcode_matching_fraction,_1) < AnaConfiguration::barcode_matching_minimum_cut()
                         );
    // count the number of matches
    const unsigned int n_matches = matches.size();
    if( n_matches>0 ) { ++n_matching; }
    if( n_matches>1 ) { ++n_multiple_matching; }

    // get the match with the highest matching fraction (highest number of hits if same fraction)
    const shared_ptr<const AnaTrack>  best_match = ( matches.empty() ? 
                                                            shared_ptr<const AnaTrack>() : 
                                                            *std::max_element( matches.begin() , matches.end() , AnaTrack::matchingFracNHitsLessThanV() ) );
    // determine if best match has 'good' residuals (reconstructed - generated track parameters)
    const bool best_has_good_parameters = best_match && ( best_match->satisfies_old_parameter_matching_with(truth) );
    // to modify the definition of "good parameters", replace the above line with something like
    // const bool best_has_good_parameters = best_match && ( std::abs(comp->phi0()-track->phi0()) < 6e-3 &&
    //                                                       std::abs(comp->z0()-track->z0()) < 0.8 &&
    //                                                       std::abs(comp->ct()-track->ct()) < 8e-3 &&
    //                                                       std::abs(comp->curvature()-track->curvature())/2.0 < 2e-5 );
    // histogram the number of reconstructed tracks matching this truth track
    dg::fillh( "n_matches_"+study_name , 10 , 0 , 10 , n_matches , "N "  + track_name + " MATCHING " + truth_name );
    // fill some standard efficiency plots vs truth track kinematics. efficiency is 100% if all tracks have n_matches>0.
    StudyTrackingEfficiency st_eff( study_name , track_name + " efficiency w.r.t. " + truth_name );
    st_eff.for_event( AnaEventMgr::current_event() );
    // if the reconstructed tracks are supposed to come from FTK, plot the tracking efficiency in 
    // a couple of categories separately: "full_fit" where AnaTrack::nmisses()==0 and 
    // "majority_fit" where AnaTrack::nmisses()!=0
    if( boost::find_first( track_name , "FTK" ) ) {
      st_eff.add_constant_category( "full_fit_best" , matches.empty() || !(best_match->ftk_is_majority()) );
      st_eff.add_constant_category( "majority_fit_best" , matches.empty() || (best_match->ftk_is_majority()) );
    }
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

    // Histogram quantities for matched tracks
    StudyTrack study_track(track_name , "plots of matched tracks" );
    study_track.for_event( AnaEventMgr::current_event() );
    study_track.do_cluster_study( false );
    study_track.for_each( matches.begin() , matches.end() );

    if( boost::find_first( track_name , "FTK" ) ) {
      BOOST_FOREACH( const shared_ptr<const AnaTrack>& btrack , matches ) { 
        if( btrack->nmisses()==0 ) {
          plot_track_parameter_differences( study_name+"_nomisses" , truth , btrack );
        } else {
          plot_track_parameter_differences( study_name+"_misses" , truth , btrack );
        }
      }
    }
    if( true && best_match ) { // plot track parameter differences for tracks with/without hits in barrel phi overlaps
      dg::down( study_name , track_name + " efficiency w.r.t. " + truth_name); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      dg::down( "multiple_clusters_per_layer" , "tracks with/without multiple clusters per FTK plane" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      // check whether btrack has multiple clusters in a given logical FTK layer.
      std::vector<unsigned int> nclu_in_layer(11,0);
      BOOST_FOREACH( const shared_ptr<const AnaSiCluster>& clu , best_match->si_clusters() ) {
        const int ll = clu->ftk_logical_layer();
        if( ll<0 || ll>11 ) { continue; }
        ++nclu_in_layer[ll];
      }
      for( std::vector<unsigned int>::iterator i=nclu_in_layer.begin(), f=nclu_in_layer.end(); i!=f; ++i ) {
        dg::fillh( (boost::format("nhits_for_layer_%d") % std::distance(nclu_in_layer.begin(),i)).str() , 5 , 0 , 5 , *i );
        dg::fillh( (boost::format("nhits_for_layer_%d_vs_phi") % std::distance(nclu_in_layer.begin(),i)).str() , 
                   208 , 0 , 52 , 3 , 0 , 3 , 
                   detector::fmodd(static_cast<double>(best_match->phi0()), 2*M_PI/52.,0.) , *i ,
                   "TRACK #phi mod (2#pi/52)" , "NUMBER OF HITS IN LAYER" );
      }
      const unsigned int nmult_layers = std::count_if( nclu_in_layer.begin() , nclu_in_layer.end() , boost::bind(greater_equal<unsigned int>(),_1,2) );
      dg::fillh( "nmult_layers" , 12 , 0 , 12 , nmult_layers );
      dg::fillh( "nmult_layers_vs_phi" , 208 , 0 , 52 , 12 , 0 , 12 , 
                 detector::fmodd(static_cast<double>(best_match->phi0()), 2*M_PI/52.,0.) , nmult_layers , "TRACK #phi mod (2#pi/52)" , "N LOGICAL LAYERS WITH MULTIPLE HITS" );      
      if( nmult_layers>0 ) {
        plot_track_parameter_differences( study_name+"_ge1overlap" , truth , best_match );
        if( nmult_layers>1 ) {
          plot_track_parameter_differences( study_name+"_ge2overlap" , truth , best_match );
        }
      } else {
        plot_track_parameter_differences( study_name+"_nooverlap" , truth , best_match );
      }
      if( nclu_in_layer[0]>1 ) {
        plot_track_parameter_differences( study_name+"_blayeroverlap" , truth , best_match );
      } else { 
        plot_track_parameter_differences( study_name+"_noblayeroverlap" , truth , best_match );
      }
    }
    st_eff.repurpose( study_name+"_GOOD_PARAMETERS" , track_name + " efficiency w.r.t. " + truth_name + " with good parameter residuals" );
    st_eff.for_only( StudyTrackingEfficiency::type( truth , n_matches>0 && best_has_good_parameters ) );

    // plot separate efficiencies for ftk full track fits and majority fits.
    if( boost::find_first( track_name , "FTK" ) ) {
      dg::down( study_name, track_name + " efficiency w.r.t. " + truth_name); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      st_eff.repurpose( "full_fits", "full fit " + track_name + " efficiency w.r.t. " + truth_name );
      st_eff.for_only( StudyTrackingEfficiency::type( truth , count_if(matches.begin(),matches.end(),bind(&AnaTrack::nmisses,_1)==0)>0));
      st_eff.repurpose( "single_miss", "single miss " + track_name + " efficiency w.r.t. " + truth_name );
      st_eff.for_only( StudyTrackingEfficiency::type( truth , count_if(matches.begin(),matches.end(),bind(&AnaTrack::nmisses,_1)==1)>0));
      st_eff.repurpose( "single_miss_maj", "single_miss_maj " + track_name + " efficiency w.r.t. " + truth_name );
      st_eff.for_only( StudyTrackingEfficiency::type( truth , count_if(matches.begin(),matches.end(),bind(&AnaTrack::ftk_is_majority,_1))>0));
      st_eff.repurpose( "single_miss_maj_pix", "single_miss_maj_pix " + track_name + " efficiency w.r.t. " + truth_name );
      st_eff.for_only( StudyTrackingEfficiency::type( truth , count_if(matches.begin(),matches.end(),bind(&AnaTrack::ftk_is_majority,_1) && bind(&AnaTrack::pixel_nhits,_1) == 2)>0));
      st_eff.repurpose( "single_miss_maj_pix0", "single_miss_maj_pix0 " + track_name + " efficiency w.r.t. " + truth_name );
      st_eff.for_only( StudyTrackingEfficiency::type( truth , count_if(matches.begin(),matches.end(),bind(&AnaTrack::ftk_is_majority,_1) && bind(&AnaTrack::b_layer_nhits,_1) == 0)>0));
      st_eff.repurpose( "single_miss_maj_pix1", "single_miss_maj_pix1 " + track_name + " efficiency w.r.t. " + truth_name );
      st_eff.for_only( StudyTrackingEfficiency::type( truth , count_if(matches.begin(),matches.end(),bind(&AnaTrack::ftk_is_majority,_1) && bind(&detector::ftkMissedPlane, bind(&AnaTrack::ftk_bitmask,_1),1))>0));
      st_eff.repurpose( "single_miss_maj_pix2", "single_miss_maj_pix2 " + track_name + " efficiency w.r.t. " + truth_name );
      st_eff.for_only( StudyTrackingEfficiency::type( truth , count_if(matches.begin(),matches.end(),bind(&AnaTrack::ftk_is_majority,_1) && bind(&detector::ftkMissedPlane, bind(&AnaTrack::ftk_bitmask,_1),2))>0));
      st_eff.repurpose( "single_miss_maj_sct", "single_miss_maj_sct " + track_name + " efficiency w.r.t. " + truth_name );
      st_eff.for_only( StudyTrackingEfficiency::type( truth , count_if(matches.begin(),matches.end(),bind(&AnaTrack::ftk_is_majority,_1) && bind(&AnaTrack::sct_nhits,_1) == 7)>0));
      st_eff.repurpose( "single_miss_road", "single_miss_road " + track_name + " efficiency w.r.t. " + truth_name );
      st_eff.for_only( StudyTrackingEfficiency::type( truth , count_if(matches.begin(),matches.end(),!bind(&AnaTrack::ftk_is_majority,_1) && bind(&AnaTrack::nmisses,_1) == 1)>0));
      st_eff.repurpose( "single_miss_road_pix", "single_miss_road_pix " + track_name + " efficiency w.r.t. " + truth_name );
      st_eff.for_only( StudyTrackingEfficiency::type( truth , count_if(matches.begin(),matches.end(),!bind(&AnaTrack::ftk_is_majority,_1) && bind(&AnaTrack::pixel_nhits,_1) == 2)>0));
      st_eff.repurpose( "single_miss_road_pix0", "single_miss_road_pix0 " + track_name + " efficiency w.r.t. " + truth_name );
      st_eff.for_only( StudyTrackingEfficiency::type( truth , count_if(matches.begin(),matches.end(),!bind(&AnaTrack::ftk_is_majority,_1) && bind(&AnaTrack::b_layer_nhits,_1) == 0)>0));
      st_eff.repurpose( "single_miss_road_pix1", "single_miss_road_pix1 " + track_name + " efficiency w.r.t. " + truth_name );
      st_eff.for_only( StudyTrackingEfficiency::type( truth , count_if(matches.begin(),matches.end(),!bind(&AnaTrack::ftk_is_majority,_1) && bind(&detector::ftkMissedPlane, bind(&AnaTrack::ftk_bitmask,_1),1))>0));
      st_eff.repurpose( "single_miss_road_pix2", "single_miss_road_pix2 " + track_name + " efficiency w.r.t. " + truth_name );
      st_eff.for_only( StudyTrackingEfficiency::type( truth , count_if(matches.begin(),matches.end(),!bind(&AnaTrack::ftk_is_majority,_1) && bind(&detector::ftkMissedPlane, bind(&AnaTrack::ftk_bitmask,_1),2))>0));
      st_eff.repurpose( "single_miss_road_sct", "single_miss_road_sct " + track_name + " efficiency w.r.t. " + truth_name );
      st_eff.for_only( StudyTrackingEfficiency::type( truth , count_if(matches.begin(),matches.end(),!bind(&AnaTrack::ftk_is_majority,_1) && bind(&AnaTrack::sct_nhits,_1) == 7)>0));
      st_eff.repurpose( "double_miss", "double_miss " + track_name + " efficiency w.r.t. " + truth_name );
      st_eff.for_only( StudyTrackingEfficiency::type( truth , count_if(matches.begin(),matches.end(),bind(&AnaTrack::nmisses,_1) == 2)>0));
      st_eff.repurpose( "double_miss0", "double_miss0 " + track_name + " efficiency w.r.t. " + truth_name );
      st_eff.for_only( StudyTrackingEfficiency::type( truth , count_if(matches.begin(),matches.end(),bind(&AnaTrack::nmisses,_1) == 2 && bind(&detector::ftkMissedPlane, bind(&AnaTrack::ftk_bitmask,_1),6))>0));
      st_eff.repurpose( "double_miss1", "double_miss1 " + track_name + " efficiency w.r.t. " + truth_name );
      st_eff.for_only( StudyTrackingEfficiency::type( truth , count_if(matches.begin(),matches.end(),bind(&AnaTrack::nmisses,_1) == 2 && bind(&detector::ftkMissedPlane, bind(&AnaTrack::ftk_bitmask,_1),8))>0));
      st_eff.repurpose( "double_miss2", "double_miss2 " + track_name + " efficiency w.r.t. " + truth_name );
      st_eff.for_only( StudyTrackingEfficiency::type( truth , count_if(matches.begin(),matches.end(),bind(&AnaTrack::nmisses,_1) == 2 && bind(&detector::ftkMissedPlane, bind(&AnaTrack::ftk_bitmask,_1),10))>0));
      st_eff.repurpose( "double_miss3", "double_miss3 " + track_name + " efficiency w.r.t. " + truth_name );
      st_eff.for_only( StudyTrackingEfficiency::type( truth , count_if(matches.begin(),matches.end(),bind(&AnaTrack::nmisses,_1) == 2 && bind(&detector::ftkMissedPlane, bind(&AnaTrack::ftk_bitmask,_1),12))>0));
    }

//     // plot efficiencies and track parameters for each region
//     // separately. this uses a crude geometric definition of a region:
//     // the quadrant containing the phi of the intersection with the
//     // layer.
//     if( false ) {
//       dg::down( "region" , "FTK region-specific plots (region defined by rough calculation of intersection with layer geometry)" );
//       const std::bitset<8> track_hits_region( detector::ftkTestIntersectionsCountInRegions( detector::intersections_with_silicon_barrels<AnaTrack::ftype>(truth) , 7 ) );
//       for( unsigned int iregion=0; iregion!=8; ++iregion ) {
//         if( !track_hits_region.test(iregion) ) { continue; }
//         dg::down( (boost::format("region_%1%") % iregion).str() , "plots for approximate region" );
//         StudyTrack st_track( "truth" , (boost::format("truth track parameters for region %1%")%iregion).str() );
//         st_track.for_only( truth );
//         StudyTrackingEfficiency st_eff( track_name , track_name + " efficiency w.r.t. truth" );
//         st_eff.for_only( StudyTrackingEfficiency::type( truth , n_matches>0 ) );
//         //plot_tracking_efficiency( track_name , truth , n_matches>0 );
//         dg::up();
//       }
//       dg::up();
//     }
    // see plot_tracking_efficiency for an explanation of the following line.
    dg::filleff( study_name+"_vs_type" , 13 , -6 , 7 , particle_type , n_matches>0 , "PARTICLE TYPE CODE" );
    // efficiency vs jet dr.
    if( false ) {
      dg::down( study_name , track_name + " efficiency w.r.t. " + truth_name ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      double jet_mindr = std::numeric_limits<double>::max();
      BOOST_FOREACH( const shared_ptr<const AnaJet>& jet , AnaEventMgr::current_event()->atlas_cone_4_truth_jets() ) {
        const double deta = detector::delta_eta( jet->eta() , truth->eta() );
        const double dphi = detector::delta_phi( jet->phi() , truth->phi0() );
        jet_mindr = std::min( jet_mindr , detector::quadrature(deta,dphi) );
      }
      dg::filleff( "eff_vs_jet_dr" , 100 , 0 , 0.5 , jet_mindr , n_matches>0 , "JET #Delta R" );
      dg::filleff( "dupeff_vs_jet_dr" , 100 , 0 , 0.5 , jet_mindr , n_matches>1 , "JET #Delta R" );

      dg::fillh( "nmatches_vs_jet_dr" , 100 , 0 , 0.5 , 10 , 0 , 10 , jet_mindr , n_matches , "JET #Delta R" , "Number of matches" );
      // BOOST_FOREACH( const shared_ptr<const AnaTrack>& match , matches ) { 
      //   dg::fillh( "matching_vs_jet_dr" , 100 , 0 , 0.5 , 50 , 0 , 1 , jet_mindr , match->barcode_matching_fraction() ,
      //              "JET #Delta R" , "Barcode Matching Fraction" );
      // }
      // study track resolutions inside jets
      if( jet_mindr < 0.2) {
        dg::down( "resolutions_inside_jets" , "track reconsturcted resolutions w.r.t. truth for tracks within 0.2 of jet centroid" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
        BOOST_FOREACH( const shared_ptr<const AnaTrack>& btrack , matches ) { 
          plot_track_parameter_differences( study_name , truth , btrack );
          if( boost::find_first( track_name , "FTK" ) && btrack->nmisses()==0 ) {
            plot_track_parameter_differences( study_name+"_fullfit" , truth , btrack );
          } else if( boost::find_first( track_name , "FTK" ) && btrack->nmisses()>0 ) {
            plot_track_parameter_differences( study_name+"_majorityfit" , truth , btrack );
          }
          if( truth->is_truth_b_decay() ) {
            plot_track_parameter_differences( study_name+"_bdaughters" , truth , btrack );
          }
        }
      }
    }
    // study properties of the tracks matching the truth: parameters,
    // differences between the track parameters and the truth
    // parameters, rate and properties of multiple matches.
    if( n_matches>0 ) {
      // see plot_track_parameters for an explanation of the following line.
      dg::fillh( study_name+"_matched_true_type" , 13 , -6 , 7 , particle_type , "PARTICLE TYPE CODE" );
      // get "best" matched track. the figure of merit is the matching fraction. 
      // if the matching fraction is the same, matchingFracNHitsLessThan uses the 
      // largest number of hits instead.
      vector< shared_ptr<const AnaTrack> >::iterator ibest = 
        std::max_element( matches.begin() , matches.end() , AnaTrack::matchingFracNHitsLessThanV() );
      assert( ibest != matches.end() ); // since n_matches>0, there must be a best match.
      const shared_ptr<const AnaTrack>& btrack( *ibest );
      // plot parameters for the track matching the truth.
      StudyTrack st_track( study_name+"_matched" , "reconstructed "+track_name+" best matching truth" );
      st_track.for_only( btrack );
      plot_track_parameter_differences( study_name , truth , btrack , vx , vy , vz );
      dg::down( study_name ,"");
      if( btrack->is_ftk() ) {
	if(btrack->nmisses() == 0) { plot_track_parameter_diff_detail( "full_fits" , truth , btrack ); }
	if(btrack->nmisses() == 1) { plot_track_parameter_diff_detail( "single_miss" , truth , btrack ); }
	if(btrack->ftk_is_majority()) { plot_track_parameter_diff_detail( "single_miss_maj" , truth , btrack ); }
	if(btrack->ftk_is_majority() && btrack->pixel_nhits() == 2 ) { plot_track_parameter_diff_detail( "single_miss_maj_pix" , truth , btrack ); }
	if(btrack->ftk_is_majority() && btrack->b_layer_nhits() == 0) { plot_track_parameter_diff_detail( "single_miss_maj_pix0" , truth , btrack ); }
	if(btrack->ftk_is_majority() && detector::ftkMissedPlane(btrack->ftk_bitmask(),1)) { plot_track_parameter_diff_detail( "single_miss_maj_pix1" , truth , btrack ); }
	if(btrack->ftk_is_majority() && detector::ftkMissedPlane(btrack->ftk_bitmask(),2)) { plot_track_parameter_diff_detail( "single_miss_maj_pix2" , truth , btrack ); }
	if(btrack->ftk_is_majority() && btrack->sct_nhits() == 7) { plot_track_parameter_diff_detail( "single_miss_maj_sct" , truth , btrack ); }
	if(btrack->nmisses() == 1 && !btrack->ftk_is_majority()) { plot_track_parameter_diff_detail( "single_miss_road" , truth , btrack ); }
	if(btrack->pixel_nhits() == 2 && !btrack->ftk_is_majority()) { plot_track_parameter_diff_detail( "single_miss_road_pix" , truth , btrack) ; }
	if(btrack->b_layer_nhits() == 0 && !btrack->ftk_is_majority() ) { plot_track_parameter_diff_detail( "single_miss_road_pix0" , truth , btrack ); }
	if(!btrack->ftk_is_majority() && detector::ftkMissedPlane(btrack->ftk_bitmask(),1)) { plot_track_parameter_diff_detail( "single_miss_road_pix1" , truth , btrack ); }
	if(!btrack->ftk_is_majority() && detector::ftkMissedPlane(btrack->ftk_bitmask(),2)) { plot_track_parameter_diff_detail( "single_miss_road_pix2" , truth , btrack ); }
	if(btrack->sct_nhits() == 7 && !btrack->ftk_is_majority() ) { plot_track_parameter_diff_detail( "single_miss_road_sct" , truth , btrack ); }
	if(btrack->nmisses() == 2) { plot_track_parameter_diff_detail( "double_miss" , truth , btrack ); }
	if(btrack->nmisses() == 2 && detector::ftkMissedPlane(btrack->ftk_bitmask(),6)) { plot_track_parameter_diff_detail( "double_miss0" , truth , btrack ); }
	if(btrack->nmisses() == 2 && detector::ftkMissedPlane(btrack->ftk_bitmask(),8)) { plot_track_parameter_diff_detail( "double_miss1" , truth , btrack ); }
	if(btrack->nmisses() == 2 && detector::ftkMissedPlane(btrack->ftk_bitmask(),10)) { plot_track_parameter_diff_detail( "double_miss2" , truth , btrack ); }
	if(btrack->nmisses() == 2 && detector::ftkMissedPlane(btrack->ftk_bitmask(),12)) { plot_track_parameter_diff_detail( "double_miss3" , truth , btrack ); }
      }
      if( truth->is_truth_b_decay() ) { plot_track_parameter_diff_detail( "b_daughters" , truth , btrack ); }
      if( truth->is_truth_charged_pion_or_secondary() ) { plot_track_parameter_diff_detail( "pion" , truth , btrack ); }
      dg::up();
      
      if( n_matches==2 ) { plot_track_parameter_differences( study_name+"_two_duplicates" , matches.front() , matches.back() ); }
      if( AnaConfiguration::do_si_cluster_studies() && btrack->is_offline() && !btrack->si_clusters().empty() ) {
        dg::down( "offline_by_hitusage" , "offline track residuals vs hit usage" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
        // count number of hits on each FTK logical layer
        unsigned int nhits_ll[11];
        for( int il=0; il!=11; ++il ) { nhits_ll[il] = 0; }
        BOOST_FOREACH( const shared_ptr<const AnaSiCluster>& cluster , btrack->si_clusters() ) {
          assert( cluster );
          if( cluster->ftk_logical_layer()<0 || cluster->ftk_logical_layer()>=11 ) { continue; }
          ++(nhits_ll[cluster->ftk_logical_layer()]);
        }
        const int npix_mult = ((int)(nhits_ll[0]>1) + (int)(nhits_ll[1]>1) + (int)(nhits_ll[2] > 1));
        const int nsct_mult = ((int)(nhits_ll[3]>1) + (int)(nhits_ll[4]>1) + (int)(nhits_ll[5] > 1) + 
                               (int)(nhits_ll[6]>1) + (int)(nhits_ll[7]>1) + (int)(nhits_ll[8] > 1) +
                               (int)(nhits_ll[9]>1) + (int)(nhits_ll[10]>1));
        dg::fillh( "nhits_l0" , 3 , 0  , 3 , nhits_ll[0] );
        dg::fillh( "nhits_l1" , 3 , 0  , 3 , nhits_ll[1] );
        dg::fillh( "nhits_l2" , 3 , 0  , 3 , nhits_ll[2] );
        dg::fillh( "nhits_l3" , 3 , 0  , 3 , nhits_ll[3] );
        dg::fillh( "nhits_l4" , 3 , 0  , 3 , nhits_ll[4] );
        dg::fillh( "nhits_l5" , 3 , 0  , 3 , nhits_ll[5] );
        dg::fillh( "nhits_l6" , 3 , 0  , 3 , nhits_ll[6] );
        dg::fillh( "nhits_l7" , 3 , 0  , 3 , nhits_ll[7] );
        dg::fillh( "nhits_l8" , 3 , 0  , 3 , nhits_ll[8] );
        dg::fillh( "nhits_l9" , 3 , 0  , 3 , nhits_ll[9] );
        dg::fillh( "nhits_l10" , 3 , 0  , 3 , nhits_ll[10] );
        dg::fillh( "npix_mult" , 4 , 0 , 4 , npix_mult );
        dg::fillh( "nsct_mult" , 4 , 0 , 4 , nsct_mult );
        if( npix_mult >= 1 ) { plot_track_parameter_differences( study_name+"_multll_pixel" , truth , btrack ); }
        if( npix_mult >= 2 ) { plot_track_parameter_differences( study_name+"_multll_pixel_2" , truth , btrack ); }
        if( npix_mult >= 3 ) { plot_track_parameter_differences( study_name+"_multll_pixel_3" , truth , btrack ); }
        if( nsct_mult >= 1 ) { plot_track_parameter_differences( study_name+"_multll_sct" , truth , btrack ); }
        if( nsct_mult >= 2 ) { plot_track_parameter_differences( study_name+"_multll_sct_2" , truth , btrack ); }
        if( nsct_mult >= 3 ) { plot_track_parameter_differences( study_name+"_multll_sct_3" , truth , btrack ); }
        if( nhits_ll[0] >= 2 ) { plot_track_parameter_differences( study_name+"_multll_blayer" , truth , btrack ); }
      }
      if( true ) {
        dg::down( "duplicates" , "multiple matches" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
        StudyTrackingEfficiency st_eff( study_name , track_name + " multiple match efficiency w.r.t. " + truth_name );
	st_eff.add_constant_category( "barrel" , std::abs(truth->eta()) < 1.0 );
        st_eff.for_event( AnaEventMgr::current_event() );
        st_eff.for_only( StudyTrackingEfficiency::type( truth , n_matches>=2 ) );

	if( boost::find_first( track_name , "FTK" ) ) {
	  dg::down( study_name, "multiple matches"); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
	  st_eff.repurpose( "full_fits", "full fit " + track_name + " duplicate w.r.t. " + truth_name );
	  st_eff.for_only( StudyTrackingEfficiency::type( truth , count_if(matches.begin(),matches.end(),bind(&AnaTrack::nmisses,_1)==0)>1));
	  st_eff.repurpose( "single_miss", "single miss " + track_name + " duplicate w.r.t. " + truth_name );
	  st_eff.for_only( StudyTrackingEfficiency::type( truth , count_if(matches.begin(),matches.end(),bind(&AnaTrack::nmisses,_1)==1)>1));
	  st_eff.repurpose( "single_miss_maj", "single_miss_maj " + track_name + " duplicate w.r.t. " + truth_name );
	  st_eff.for_only( StudyTrackingEfficiency::type( truth , count_if(matches.begin(),matches.end(),bind(&AnaTrack::ftk_is_majority,_1))>1));
	  st_eff.repurpose( "single_miss_maj_pix", "single_miss_maj_pix " + track_name + " duplicate w.r.t. " + truth_name );
	  st_eff.for_only( StudyTrackingEfficiency::type( truth , count_if(matches.begin(),matches.end(),bind(&AnaTrack::ftk_is_majority,_1) && bind(&AnaTrack::pixel_nhits,_1) == 2)>1));
	  st_eff.repurpose( "single_miss_maj_pix0", "single_miss_maj_pix0 " + track_name + " duplicate w.r.t. " + truth_name );
	  st_eff.for_only( StudyTrackingEfficiency::type( truth , count_if(matches.begin(),matches.end(),bind(&AnaTrack::ftk_is_majority,_1) && bind(&AnaTrack::b_layer_nhits,_1) == 0)>1));
	  st_eff.repurpose( "single_miss_maj_pix1", "single_miss_maj_pix1 " + track_name + " duplicate w.r.t. " + truth_name );
	  st_eff.for_only( StudyTrackingEfficiency::type( truth , count_if(matches.begin(),matches.end(),bind(&AnaTrack::ftk_is_majority,_1) && bind(&detector::ftkMissedPlane, bind(&AnaTrack::ftk_bitmask,_1),1))>1));
	  st_eff.repurpose( "single_miss_maj_pix2", "single_miss_maj_pix2 " + track_name + " duplicate w.r.t. " + truth_name );
	  st_eff.for_only( StudyTrackingEfficiency::type( truth , count_if(matches.begin(),matches.end(),bind(&AnaTrack::ftk_is_majority,_1) && bind(&detector::ftkMissedPlane, bind(&AnaTrack::ftk_bitmask,_1),2))>1));
	  st_eff.repurpose( "single_miss_maj_sct", "single_miss_maj_sct " + track_name + " duplicate w.r.t. " + truth_name );
	  st_eff.for_only( StudyTrackingEfficiency::type( truth , count_if(matches.begin(),matches.end(),bind(&AnaTrack::ftk_is_majority,_1) && bind(&AnaTrack::sct_nhits,_1) == 7)>1));
	  st_eff.repurpose( "single_miss_road", "single_miss_road " + track_name + " duplicate w.r.t. " + truth_name );
	  st_eff.for_only( StudyTrackingEfficiency::type( truth , count_if(matches.begin(),matches.end(),!bind(&AnaTrack::ftk_is_majority,_1) && bind(&AnaTrack::nmisses,_1) == 1)>1));
	  st_eff.repurpose( "single_miss_road_pix", "single_miss_road_pix " + track_name + " duplicate w.r.t. " + truth_name );
	  st_eff.for_only( StudyTrackingEfficiency::type( truth , count_if(matches.begin(),matches.end(),!bind(&AnaTrack::ftk_is_majority,_1) && bind(&AnaTrack::pixel_nhits,_1) == 2)>1));
	  st_eff.repurpose( "single_miss_road_pix0", "single_miss_road_pix0 " + track_name + " duplicate w.r.t. " + truth_name );
	  st_eff.for_only( StudyTrackingEfficiency::type( truth , count_if(matches.begin(),matches.end(),!bind(&AnaTrack::ftk_is_majority,_1) && bind(&AnaTrack::b_layer_nhits,_1) == 0)>1));
	  st_eff.repurpose( "single_miss_road_pix1", "single_miss_road_pix1 " + track_name + " duplicate w.r.t. " + truth_name );
	  st_eff.for_only( StudyTrackingEfficiency::type( truth , count_if(matches.begin(),matches.end(),!bind(&AnaTrack::ftk_is_majority,_1) && bind(&detector::ftkMissedPlane, bind(&AnaTrack::ftk_bitmask,_1),1))>1));
	  st_eff.repurpose( "single_miss_road_pix2", "single_miss_road_pix2 " + track_name + " duplicate w.r.t. " + truth_name );
	  st_eff.for_only( StudyTrackingEfficiency::type( truth , count_if(matches.begin(),matches.end(),!bind(&AnaTrack::ftk_is_majority,_1) && bind(&detector::ftkMissedPlane, bind(&AnaTrack::ftk_bitmask,_1),2))>1));
	  st_eff.repurpose( "single_miss_road_sct", "single_miss_road_sct " + track_name + " duplicate w.r.t. " + truth_name );
	  st_eff.for_only( StudyTrackingEfficiency::type( truth , count_if(matches.begin(),matches.end(),!bind(&AnaTrack::ftk_is_majority,_1) && bind(&AnaTrack::sct_nhits,_1) == 7)>1));
	  st_eff.repurpose( "double_miss", "double_miss " + track_name + " duplicate w.r.t. " + truth_name );
	  st_eff.for_only( StudyTrackingEfficiency::type( truth , count_if(matches.begin(),matches.end(),bind(&AnaTrack::nmisses,_1) == 2)>1));
	  st_eff.repurpose( "double_miss0", "double_miss0 " + track_name + " duplicate w.r.t. " + truth_name );
	  st_eff.for_only( StudyTrackingEfficiency::type( truth , count_if(matches.begin(),matches.end(),bind(&AnaTrack::nmisses,_1) == 2 && bind(&detector::ftkMissedPlane, bind(&AnaTrack::ftk_bitmask,_1),6))>1));
	  st_eff.repurpose( "double_miss1", "double_miss1 " + track_name + " duplicate w.r.t. " + truth_name );
	  st_eff.for_only( StudyTrackingEfficiency::type( truth , count_if(matches.begin(),matches.end(),bind(&AnaTrack::nmisses,_1) == 2 && bind(&detector::ftkMissedPlane, bind(&AnaTrack::ftk_bitmask,_1),8))>1));
	  st_eff.repurpose( "double_miss2", "double_miss2 " + track_name + " duplicate w.r.t. " + truth_name );
	  st_eff.for_only( StudyTrackingEfficiency::type( truth , count_if(matches.begin(),matches.end(),bind(&AnaTrack::nmisses,_1) == 2 && bind(&detector::ftkMissedPlane, bind(&AnaTrack::ftk_bitmask,_1),10))>1));
	  st_eff.repurpose( "double_miss3", "double_miss3 " + track_name + " duplicate w.r.t. " + truth_name );
	  st_eff.for_only( StudyTrackingEfficiency::type( truth , count_if(matches.begin(),matches.end(),bind(&AnaTrack::nmisses,_1) == 2 && bind(&detector::ftkMissedPlane, bind(&AnaTrack::ftk_bitmask,_1),12))>1));
	}
      } // end study duplicates
    } // end if there is at least one match, look at the matches
    
    if( false && n_matches==0 ) {
      dg::down( "efficiency_loss_studies" , "matchless tracks" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      // investigate properties of events in which no comparison track is found
      // for the reference. e.g. what are the properties of offline tracks for which no 
      // ftksim track is found?
      using boost::format;
      if( truth->barcode().event_index()!=0 ) { continue; } // skip pileup truth
      cout << " " << " truth does not have a good " << track_name << " match: " << endl
           << "     " << ( boost::format( "event %1% %2% %3% %4% %|25t|" ) 
                           % AnaEventMgr::current_event()->run_number() % AnaEventMgr::current_event()->event_number() % truth->barcode().event_index() % truth->barcode().barcode() );
      truth->print( cout );
      // find closest matches in barcode and/or phi if any
      vector< shared_ptr<const AnaTrack> > barcode_matches;
      vector< shared_ptr<const AnaTrack> > parameter_matches;
      std::remove_copy_if( track_collection.begin() , track_collection.end() , back_inserter(barcode_matches) ,
                           bind(&AnaTrack::barcode,_1) != truth->barcode() );
      std::remove_copy_if( track_collection.begin() , track_collection.end() , back_inserter(parameter_matches) ,
                           bind( logical_not<bool>() , bind( AnaTrack::parametersEqualV(truth) , _1 ) ) );
      if( true ) { 
        sort( barcode_matches.begin() , barcode_matches.end() , AnaTrack::matchingFracGreaterThanV() );
        cout << "     barcode matches: " << barcode_matches.size() << " (showing first 5)" << endl;
        vector< shared_ptr<const AnaTrack> > first_n_barcode_matches;
        __gnu_cxx::copy_n( barcode_matches.begin() , std::min(static_cast<size_t>(5),barcode_matches.size()) , back_inserter(first_n_barcode_matches) );
        for_each( first_n_barcode_matches.begin() , first_n_barcode_matches.end() , bind( &AnaTrack::print , _1 ) );
        sort( parameter_matches.begin() , parameter_matches.end() , AnaTrack::matchingFracGreaterThanV() );
        cout << "     parameter matches: " << parameter_matches.size() << endl;
        for_each( parameter_matches.begin() , parameter_matches.end() , bind( &AnaTrack::print , _1 ) );
      }
      // dg::fillh( "barcode_matches" , 50 , 0 , 50 , barcode_matches.size() );
      // dg::fillh( "parameter_matches" , 50 , 0 , 50 , parameter_matches.size() );
      // find highest matching fraction for barcode and parameter match.
      //  shared_ptr<const AnaTrack>  best_barcode_match = ( barcode_matches.empty() ? 0 : *std::max_element( barcode_matches.begin() , barcode_matches.end() , AnaTrack::matchingFracNHitsLessThanV() ) );
      //  shared_ptr<const AnaTrack>  best_parameter_match = ( parameter_matches.empty() ? 0 : *std::max_element( parameter_matches.begin() , parameter_matches.end() , AnaTrack::matchingFracNHitsLessThanV() ) );
      // if( best_barcode_match ) { plot_track_parameter_differences( "best_barcode_match" , track , best_barcode_match ); }
      // if( best_parameter_match ) { plot_track_parameter_differences( "best_parameter_match" , track , best_parameter_match ); }
    } // end if matchless tracks, study them


    // look at the failures. are there any good tracks present?
    vector< shared_ptr<const AnaTrack> > bad_matches;
    std::remove_copy_if( track_collection.begin() , track_collection.end() , back_inserter(bad_matches) ,
                         bind(&AnaTrack::barcode,_1) != truth->barcode()
                         || bind(&AnaTrack::barcode_matching_fraction,_1) > 0.4
                         // || bind(&AnaTrack::barcode_matching_fraction,_1) > AnaConfiguration::barcode_matching_minimum_cut()
                         || bind(&AnaTrack::barcode_matching_fraction,_1) < 0.0001
                         );
    if( !bad_matches.empty() ) {
      // check out residuals
      dg::down( study_name+"_BADMATCHED" , track_name + " geant matching failures (small barcode matching fraction)" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      BOOST_FOREACH( const shared_ptr<const AnaTrack>& track , bad_matches ) {
        plot_track_parameter_differences( "delta_parameters" , truth , track );
        dg::fillh( "barcode_matching_fraction" , 100 , 0 , 1 , track->barcode_matching_fraction() );
      }
      // find best matched track
      if( true ) { 
        const shared_ptr<const AnaTrack> best_bad_match = ( bad_matches.empty() ? 
                                                                   shared_ptr<const AnaTrack>() : 
                                                                   *std::max_element( bad_matches.begin() , bad_matches.end() , AnaTrack::matchingFracNHitsLessThanV() ) );
        assert( best_bad_match );
        dg::down( "single" , "single best barcode (still bad) for each truth track" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
        plot_track_parameter_differences( "delta_parameters" , truth , best_bad_match );
        dg::fillh( "barcode_matching_fraction" , 100 , 0 , 1 , best_bad_match->barcode_matching_fraction() );
      }
    }
  } // end for each good truth track
  dg::fillh( "n_matching_"+study_name+"_tracks" , 20 , 0 , 20 , 
             n_matching , "N " + truth_name + " WITH " + track_name + " MATCH" );
  dg::fillh( "n_multiple_"+study_name+"_tracks" , 20 , 0 , 20 , 
             n_multiple_matching , "N " + truth_name + " WITH MULTIPLE " + track_name + " MATCHES" );
  // fake rate
  if( true ) {
    BOOST_FOREACH( const shared_ptr<const AnaTrack>& track , track_collection ) {
      if( AnaConfiguration::phi_fails_cut(track->phi0()) ) continue;
      // vector< shared_ptr<const AnaTrack> > matches;
      // std::remove_copy_if( truth_collection.begin() , truth_collection.end() , back_inserter(matches) , 
      //                      bind(&AnaTrack::barcode,_1) != track->barcode()
      //                      || bind(&AnaTrack::barcode_matching_fraction,_1) < 
      //                      AnaConfiguration::barcode_matching_minimum_cut()
      //                      );
      dg::down( "fakes" , "fake rate" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      // fake rate vs kinematic variables.
      StudyTrackingEfficiency st_eff( study_name + "_fakes" , 
                                      track_name + " fake rate w.r.t. " + truth_name );
      st_eff.add_constant_category( "barrel" , std::abs(track->eta()) < 1.0 );
      st_eff.for_event( AnaEventMgr::current_event() );
      // st_eff.for_only( StudyTrackingEfficiency::type( track , matches.empty() ) );
      const bool is_fake = ( track->barcode().barcode() < 1 || 
			     track->barcode().event_index() < 0 ||
                             track->barcode_matching_fraction() < AnaConfiguration::barcode_matching_minimum_cut() );
      if( track->is_ftk() ) {
	st_eff.add_constant_category("full_fits", track->nmisses() == 0 );
	st_eff.add_constant_category("single_miss", track->nmisses() == 1 );
	st_eff.add_constant_category("single_miss_maj", track->ftk_is_majority() );
	st_eff.add_constant_category("single_miss_maj_pix", track->ftk_is_majority() && track->pixel_nhits() == 2 );
	st_eff.add_constant_category("single_miss_maj_pix0", track->ftk_is_majority() && track->b_layer_nhits() == 0 );
	st_eff.add_constant_category("single_miss_maj_pix1", track->ftk_is_majority() &&
				     detector::ftkMissedPlane(track->ftk_bitmask(),1));
	st_eff.add_constant_category("single_miss_maj_pix2", track->ftk_is_majority() &&
				     detector::ftkMissedPlane(track->ftk_bitmask(),2));
	st_eff.add_constant_category("single_miss_maj_sct", track->ftk_is_majority() && track->sct_nhits() == 7 );
	st_eff.add_constant_category("single_miss_road", !track->ftk_is_majority() && track->nmisses() == 1 );
	st_eff.add_constant_category("single_miss_road_pix", !track->ftk_is_majority() && track->pixel_nhits() == 2 );
	st_eff.add_constant_category("single_miss_road_pix0", !track->ftk_is_majority() && track->b_layer_nhits() == 0 );
	st_eff.add_constant_category("single_miss_road_pix1", !track->ftk_is_majority() &&
				     detector::ftkMissedPlane(track->ftk_bitmask(),1));
	st_eff.add_constant_category("single_miss_road_pix2", !track->ftk_is_majority() &&
				     detector::ftkMissedPlane(track->ftk_bitmask(),2));
	st_eff.add_constant_category("single_miss_road_sct", !track->ftk_is_majority() && track->sct_nhits() == 7 );
	st_eff.add_constant_category("double_miss", track->nmisses() == 2 );
	st_eff.add_constant_category("double_miss0", track->nmisses() == 2 &&
				     detector::ftkMissedPlane(track->ftk_bitmask(),6));
	st_eff.add_constant_category("double_miss1", track->nmisses() == 2 &&
				     detector::ftkMissedPlane(track->ftk_bitmask(),8));
	st_eff.add_constant_category("double_miss2", track->nmisses() == 2 &&
				     detector::ftkMissedPlane(track->ftk_bitmask(),10));
	st_eff.add_constant_category("double_miss3", track->nmisses() == 2 &&
				     detector::ftkMissedPlane(track->ftk_bitmask(),12));
      }
      st_eff.for_only( StudyTrackingEfficiency::type( track , is_fake ) );
      if( false ) { 
        cout << " FAKES " << track_name << endl;
        track->print();
        cout << " all truth " << endl;
        std::for_each( truth_collection.begin() , truth_collection.end() , bind(&AnaTrack::print,_1) );
        // cout << " matches " << endl;
        // std::for_each( matches.begin() , matches.end() , bind(&AnaTrack::print,_1) );
      }
      // fake rate vs jet dr
      {
        dg::down( study_name+"_fakes" , track_name + " fake rate w.r.t. " + truth_name ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
        double jet_mindr = std::numeric_limits<double>::max();
        BOOST_FOREACH( const shared_ptr<const AnaJet>& jet , AnaEventMgr::current_event()->atlas_cone_4_truth_jets() ) {
          const double deta = detector::delta_eta( jet->eta() , track->eta() );
          const double dphi = detector::delta_phi( jet->phi() , track->phi0() );
          jet_mindr = std::min( jet_mindr , detector::quadrature(deta,dphi) );
        }
        dg::filleff( "fake_vs_jet_dr" , 100 , 0 , 0.5 , jet_mindr , is_fake , "JET #Delta R" );
      }
    }
  }
} // end standard_truth_comparison

void
standard_track_comparison( const std::string& ref_name , 
                           const std::string& track_name , 
                           const std::vector< shared_ptr<const AnaTrack> >& track_collection_ref , 
                           const std::vector< shared_ptr<const AnaTrack> >& track_collection_comp ,
			   float vx , float vy , float vz )
{
  // loop over all track tracks in track_collection_ref and look for matching reconstructed tracks in track_collection_comp.
  unsigned int n_matching = 0u;
  unsigned int n_multiple_matching = 0u;
  vector< shared_ptr<const AnaTrack> > matchless_tracks; matchless_tracks.reserve( track_collection_ref.size() );
  BOOST_FOREACH( const shared_ptr<const AnaTrack>& track , track_collection_ref ) { 
    // apply some cuts to the reference tracks used for the comparison/efficiencies.
    if( track->barcode_matching_fraction() < AnaConfiguration::barcode_matching_minimum_cut() || 
	track->barcode().event_index() != 0 ) { continue; }
    // if( track->nhits() < 10 ) { continue; } // veto tracks without enough hits for FTK to find FIXME for now!
    // Get truth track's pdg code
    int truth_pdg(0);
    vector< shared_ptr<const AnaTrack> > truth_matches;
    std::remove_copy_if( AnaEventMgr::current_event()->truth_tracks().begin(), 
			 AnaEventMgr::current_event()->truth_tracks().end(),
			 back_inserter(truth_matches),
                         bind(&AnaTrack::barcode,_1) != track->barcode()
                         );
    if( truth_matches.size() ) {
      truth_pdg = (*truth_matches.begin())->pdg_code();
    }
    // retrieve reconstructed tracks assigned the same barcode as this track track and store them in a vector.
    vector< shared_ptr<const AnaTrack> > matches;
    std::remove_copy_if( track_collection_comp.begin() , track_collection_comp.end() , back_inserter(matches) , 
                         bind(&AnaTrack::barcode,_1) != track->barcode() ||
                         bind(&AnaTrack::barcode_matching_fraction,_1) < AnaConfiguration::barcode_matching_minimum_cut()
                         );
    // count the number of matches
    const unsigned int n_matches = matches.size();
    if( n_matches>0 ) { ++n_matching; }
    if( n_matches>1 ) { ++n_multiple_matching; }
    // histogram the number of reconstructed tracks matching this ref track
    dg::fillh( "n_matches_"+track_name+"_wrt_"+ref_name , 10 , 0 , 10 , 
               n_matches , "N "+track_name+" TRACKS MATCHING " + track->algorithm().name() );
    // fill some standard efficiency plots. efficiency is 100% if all tracks have n_matches>0.
    StudyTrackingEfficiency st_eff( track_name + "_wrt_" + ref_name , track_name + " efficiency w.r.t. " + ref_name);
    st_eff.add_constant_category( "barrel" , std::abs(track->eta()) < 1.0 );
    st_eff.add_constant_category( "muon" , abs(truth_pdg) == 13 );
    st_eff.add_constant_category( "pion" , abs(truth_pdg) == 211 );
    st_eff.add_constant_category( "electron" , abs(truth_pdg) == 11 );
    st_eff.for_event( AnaEventMgr::current_event() );
    st_eff.for_only( StudyTrackingEfficiency::type( track , n_matches>0 ) );
    st_eff.repurpose(track_name + "_wrt_" + ref_name + "_dup" , track_name + " duplicate rate w.r.t. " + ref_name);
    st_eff.for_only( StudyTrackingEfficiency::type( track , n_matches>1 ) );
    if( n_matches>0 ) {
      // plot ref track parameters when a match is found
      dg::down( "matching_tracks" , "matched comparison vs reference tracks" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      StudyTrack st_track( track_name+"_wrt_"+ref_name+"_ref" , "reference tracks with a good match" );
      st_track.for_only( track );
      // get "best" matched track. the figure of merit is the matching fraction. 
      // if the matching fraction is the same, matchingFracNHitsLessThan uses the 
      // largest number of hits instead.
      vector< shared_ptr<const AnaTrack> >::iterator ibest = std::max_element( matches.begin() , matches.end() , AnaTrack::matchingFracNHitsLessThanV() );
      assert( ibest != matches.end() ); // since n_matches>0, there must be a best match.
      const shared_ptr<const AnaTrack>& btrack( *ibest );
      // reject best matches with fewer then 80% matching hits
      //if( btrack->barcode_matching_fraction()<AnaConfiguration::barcode_matching_minimum_cut() ) { continue; } 
      // plot parameters for the track matching the track.
      st_track.repurpose( track_name+"_wrt_"+ref_name , "track parameters for "+track_name+" tracks matching a "+ref_name+" track" );
      st_track.for_only( btrack );
      plot_track_parameter_differences( track_name+"_wrt_"+ref_name , track , btrack , vx , vy , vz );
      if( false && track->barcode().event_index()==0 ) {
        cout << " " << ref_name << " track has " << matches.size() << "  good " << track_name << " matches: " << endl
             << "     " << ( boost::format( "event %1% %2% %3% %4% %|25t|" ) 
                             % AnaEventMgr::current_event()->run_number() % AnaEventMgr::current_event()->event_number() % track->barcode().event_index() % track->barcode().barcode() );
        track->print( cout );
        sort( matches.begin() , matches.end() , AnaTrack::matchingFracGreaterThanV() );
        {
          cout << "     matches are (best 5 shown): " << endl;
          vector< shared_ptr<const AnaTrack> > first_n_matches;
          __gnu_cxx::copy_n( matches.begin() , std::min(static_cast<size_t>(5),matches.size()) , back_inserter(first_n_matches) );
          for_each( first_n_matches.begin() , first_n_matches.end() , bind( &AnaTrack::print , _1 ) );
        }
        // find closest matches in barcode and/or phi if any
        vector< shared_ptr<const AnaTrack> > barcode_matches;
        vector< shared_ptr<const AnaTrack> > parameter_matches;
        std::remove_copy_if( track_collection_comp.begin() , track_collection_comp.end() , back_inserter(barcode_matches) ,
                             bind(&AnaTrack::barcode,_1) != track->barcode() );
        std::remove_copy_if( track_collection_comp.begin() , track_collection_comp.end() , back_inserter(parameter_matches) ,
                             bind( logical_not<bool>() , bind( AnaTrack::parametersEqualV(track) , _1 ) ) );
        if( true ) { 
          sort( barcode_matches.begin() , barcode_matches.end() , AnaTrack::matchingFracGreaterThanV() );
          cout << "     barcode matches: " << barcode_matches.size() << " (showing first 5)" << endl;
          vector< shared_ptr<const AnaTrack> > first_n_barcode_matches;
          __gnu_cxx::copy_n( barcode_matches.begin() , std::min(static_cast<size_t>(5),barcode_matches.size()) , back_inserter(first_n_barcode_matches) );
          for_each( first_n_barcode_matches.begin() , first_n_barcode_matches.end() , bind( &AnaTrack::print , _1 ) );
          sort( parameter_matches.begin() , parameter_matches.end() , AnaTrack::matchingFracGreaterThanV() );
          cout << "     parameter matches: " << parameter_matches.size() << endl;
          for_each( parameter_matches.begin() , parameter_matches.end() , bind( &AnaTrack::print , _1 ) );
        }
      } // end if print some debugging info
      if( true ) {
        StudyTrackComparison stcomp( "comparison" , "track comparison study" );
        stcomp.do_cluster_study( AnaConfiguration::do_si_cluster_studies() );
        TrackIdentification ident( track , btrack );
        stcomp.for_only( ident );
      }
    } // end if there is at least one match, look at the matches
    if( n_matches==0 ) { matchless_tracks.push_back( track ); }
  } // end for each good track track
  // dg::fillh( "n_" + ref_name + "_with_matching_"+track_name+"_tracks" , 20 , 0 , 20 , n_matching , "N "+ref_name+" WITH MATCHING "+track_name );
  // dg::fillh( "n_" + ref_name + "_with_multiple_matching_"+track_name+"_tracks" , 20 , 0 , 20 , n_multiple_matching , "N "+ref_name+" WITH MULTIPLE MATCHING "+track_name );

  if( false ) {
    dg::down( "efficiency_loss_studies" , "matchless tracks" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
    // investigate properties of events in which no comparison track is found
    // for the reference. e.g. what are the properties of offline tracks for which no 
    // ftksim track is found?
    dg::down( string("properties_") + string(matchless_tracks.empty() ? "matches" : "no_matches")  , "some event properties" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
    using boost::format;
    if( !matchless_tracks.empty() )  {
      BOOST_FOREACH( const shared_ptr<const AnaTrack>& track , matchless_tracks ) {
        if( track->barcode().event_index()!=0 ) { continue; } // skip pileup truth
        cout << " " << ref_name << " track does not have a good " << track_name << " match: " << endl
             << "     " << ( boost::format( "event %1% %2% %3% %4% %|25t|" ) 
                             % AnaEventMgr::current_event()->run_number() % AnaEventMgr::current_event()->event_number() % track->barcode().event_index() % track->barcode().barcode() );
        track->print( cout );
        // StudyTrack st_track( "unmatched_"+track_name+"_"+ref_name , ref_name + " tracks without a " + track_name + " match" );
        // st_track.for_only( track );
        // find closest matches in barcode and/or phi if any
        vector< shared_ptr<const AnaTrack> > barcode_matches;
        vector< shared_ptr<const AnaTrack> > parameter_matches;
        std::remove_copy_if( track_collection_comp.begin() , track_collection_comp.end() , back_inserter(barcode_matches) ,
                             bind(&AnaTrack::barcode,_1) != track->barcode() );
        std::remove_copy_if( track_collection_comp.begin() , track_collection_comp.end() , back_inserter(parameter_matches) ,
                             bind( logical_not<bool>() , bind( AnaTrack::parametersEqualV(track) , _1 ) ) );
        if( true ) { 
          sort( barcode_matches.begin() , barcode_matches.end() , AnaTrack::matchingFracGreaterThanV() );
          cout << "     barcode matches: " << barcode_matches.size() << " (showing first 5)" << endl;
          vector< shared_ptr<const AnaTrack> > first_n_barcode_matches;
          __gnu_cxx::copy_n( barcode_matches.begin() , std::min(static_cast<size_t>(5),barcode_matches.size()) , back_inserter(first_n_barcode_matches) );
          for_each( first_n_barcode_matches.begin() , first_n_barcode_matches.end() , bind( &AnaTrack::print , _1 ) );
          sort( parameter_matches.begin() , parameter_matches.end() , AnaTrack::matchingFracGreaterThanV() );
          cout << "     parameter matches: " << parameter_matches.size() << endl;
          for_each( parameter_matches.begin() , parameter_matches.end() , bind( &AnaTrack::print , _1 ) );
        }
        // dg::fillh( "barcode_matches" , 50 , 0 , 50 , barcode_matches.size() );
        // dg::fillh( "parameter_matches" , 50 , 0 , 50 , parameter_matches.size() );
        // find highest matching fraction for barcode and parameter match.
        //  shared_ptr<const AnaTrack>  best_barcode_match = ( barcode_matches.empty() ? 0 : *std::max_element( barcode_matches.begin() , barcode_matches.end() , AnaTrack::matchingFracNHitsLessThanV() ) );
        //  shared_ptr<const AnaTrack>  best_parameter_match = ( parameter_matches.empty() ? 0 : *std::max_element( parameter_matches.begin() , parameter_matches.end() , AnaTrack::matchingFracNHitsLessThanV() ) );
        // if( best_barcode_match ) { plot_track_parameter_differences( "best_barcode_match" , track , best_barcode_match ); }
        // if( best_parameter_match ) { plot_track_parameter_differences( "best_parameter_match" , track , best_parameter_match ); }
      } // end for each matchless reference track
    } // end if matchless tracks, study them
  }
  // if( true ) {
  //   dg::down( "efficiency_loss_studies" , "matchless tracks" );
  //     // investigate properties of events in which no comparison track is found
  //     // for the reference. e.g. what are the properties of events with no TrigFTKSim
  //     // track but a TrigFTKLib track? what are the properties of the missing track?
  //   dg::down( string("event_properties_") + string(matchless_tracks.empty() ? "matches" : "no_matches")  , "some event properties" );
  //   if( !matchless_tracks.empty() )  {
  //     BOOST_FOREACH(  shared_ptr<const AnaTrack>  const track , matchless_tracks ) {
  //       if( ref_name != "FTK_OLD" || track_name != "FTK_NEW" ) { continue; }
  //       cout << " unmatched TRIGFTKLIB track: "
  //            << boost::format( "event %1% %2% %|25t|" ) % AnaEventMgr::current_event()->run_number() % AnaEventMgr::current_event()->event_number();
  //       track->print( cout );
  //       StudyTrack st_track( "unmatched_"+track_name+"_"+ref_name , ref_name + " tracks without a " + track_name + " match" );
  //       st_track.for_only( track );
  //       for( unsigned int iplane=0; iplane!=14; ++iplane ) { 
  //         using boost::format;
  //         if( !track->is_ftk() ) { continue; }
  //         dg::filleff( (format("unmatched_%1%_hit_pl_%2%") % ref_name % iplane).str() , detector::ftkHasPlane(track->ftk_bitmask(),iplane) );
  //       }
  //     }
  //   }
  //   dg::up();
  //   dg::up();
  // }

} // end standard_track_comparison

void
plot_track_parameter_differences( const std::string& label_name , 
                                  const shared_ptr<const AnaTrack>& reference_track , 
                                  const shared_ptr<const AnaTrack>& comparison_track ,
				  const float vx , const float vy , const float vz )
{
  // plot differences between e.g. reconstruced and true track parameters.
  string comp_name = comparison_track->algorithm().name();
  string ref_name = reference_track->algorithm().name();
  dg::down( label_name , "track parameters" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
  dg::fillh( "dcurv" , 100 , -0.05 , 0.05 , (comparison_track->curvature()-reference_track->curvature())*1000. , 
             comp_name + " CURVATURE - " + ref_name + " CURVATURE (1/GeV)" );
  dg::fillh( "dcurv_vs_chi2ndof" , 20 , 0 , 20 , 100 , -0.05 , 0.05 , 
             comparison_track->chi2ndof() , 
             (comparison_track->curvature()-reference_track->curvature())*1000. , 
             comp_name + " #chi^{2} / n_{dof}" ,
             comp_name + " CURVATURE - " + ref_name + " CURVATURE (1/GeV)" );
  dg::fillh( "dd0" , 100 , -0.02 , 0.02 , (comparison_track->d0()-reference_track->d0())/10. ,
             comp_name + " d_{0} - " + ref_name + " d_{0} (cm)" );
  dg::fillh( "dd0_wide" , 100 , -0.2 , 0.2 , (comparison_track->d0()-reference_track->d0())/10. ,
             comp_name + " d_{0} - " + ref_name + " d_{0} (cm)" );
  dg::fillh( "dd0_vs_chi2ndof" , 20 , 0 , 10 , 100 , -0.02 , 0.02 , 
             comparison_track->chi2ndof() , 
             (comparison_track->d0()-reference_track->d0())/10. , 
             comp_name + " #chi^{2} / n_{dof}" ,
             comp_name + " d_{0} - " + ref_name + " d_{0} (cm)" );
  dg::fillh( "dz0" , 100 , -0.1 , 0.1 , (comparison_track->z0()-reference_track->z0())/10. ,
             comp_name + " z_{0} - " + ref_name + " z_{0} (cm)" );
  dg::fillh( "dz0_vs_chi2ndof" , 20 , 0 , 10 , 100 , -0.1 , 0.1 , 
             comparison_track->chi2ndof() , 
             (comparison_track->z0()-reference_track->z0())/10. , 
             comp_name + " #chi^{2} / n_{dof}" ,
             comp_name + " z_{0} - " + ref_name + " z_{0} (cm)" );
  //dg::fillh( "dd0_raw" , 100 , -0.02 , 0.02 , (comparison_track->d0_raw(vx,vy)-reference_track->d0_raw(vx,vy))/10. ,
  //           comp_name + " d_{0} - " + ref_name + " d_{0} (cm)" );
  //dg::fillh( "dz0_raw" , 100 , -0.1 , 0.1 , (comparison_track->z0_raw(vz)-reference_track->z0_raw(vz))/10. ,
  //           comp_name + " z_{0} - " + ref_name + " z_{0} (cm)" );
  dg::fillh( "deta" , 100 , -0.05 , 0.05 , (comparison_track->eta()-reference_track->eta()) ,
             comp_name + " #eta - " + ref_name + " #eta" );
  dg::fillh( "dphi" , 100 , -0.05 , 0.05 , (comparison_track->phi0()-reference_track->phi0()) ,
             comp_name + " #phi_{0} - " + ref_name + " #phi_{0} (rad)" );
  dg::fillh( "dphi_wide" , 100 , -M_PI , M_PI , (comparison_track->phi0()-reference_track->phi0()) ,
             comp_name + " #phi_{0} - " + ref_name + " #phi_{0} (rad)" );
  dg::fillh( "dphi_corr" , 100 , -0.05 , 0.05 , detector::delta_phi(comparison_track->phi0(),reference_track->phi0()) ,
	     comp_name + " #phi_{0} - " + ref_name + " #phi_{0} (rad) [canonical]" );
  dg::fillh( "dphi_narrow" , 200 , -0.01 , 0.01 , (comparison_track->phi0()-reference_track->phi0()) ,
             comp_name + " #phi_{0} - " + ref_name + " #phi_{0} (rad)" );
  dg::fillh( "dchi2_narrow" , 100 , -1 , 1 , (comparison_track->chi2()-reference_track->chi2()) ,
             comp_name + " #chi^{2} - " + ref_name + " #chi^{2}" );
  dg::fillh( "dchi2_wide" , 100 , -25 , 25 , (comparison_track->chi2()-reference_track->chi2()) ,
             comp_name + " #chi^{2} - " + ref_name + " #chi^{2}" );
  dg::fillh( "dnhit" , 5 , -2 , 2 , (comparison_track->nhits()-reference_track->nhits()) ,
             comp_name + " N HIT - " + ref_name + " N HIT" );
  dg::fillgraph( "dd0_vs_pt" , reference_track->pt() , (comparison_track->d0()-reference_track->d0())/10. ,
                 ref_name + " p_{T} (GeV)" , comp_name + " d_{0} - " + ref_name + " d_{0} (cm)" );
  dg::fillgraph( "dz0_vs_pt" , reference_track->pt() , (comparison_track->z0()-reference_track->z0())/10. ,
                 ref_name + " p_{T} (GeV)" , comp_name + " z_{0} - " + ref_name + " z_{0} (cm)" );
  dg::fillgraph( "dcurv_vs_pt" , reference_track->pt() , (comparison_track->curvature()-reference_track->curvature())*1000. , 
                 ref_name + " p_{T} (GeV)" , comp_name + " CURVATURE - " + ref_name + " CURVATURE (1/GeV)" );
  dg::fillgraph( "deta_vs_pt" , reference_track->pt() , (comparison_track->eta()-reference_track->eta()) ,
                 ref_name + " p_{T} (GeV)" , comp_name + " #eta - " + ref_name + " #eta" );
  dg::fillgraph( "dphi_vs_pt" , reference_track->pt() , (comparison_track->phi0()-reference_track->phi0()) ,
                 ref_name + " p_{T} (GeV)" ,comp_name + " #phi_{0} - " + ref_name + " #phi_{0} (rad)" );
  // efficiency per hit on each plane
  for( unsigned int iplane=0; iplane!=14 && reference_track->is_ftk() && comparison_track->is_ftk(); ++iplane ) { 
    using boost::format;
    if( !detector::ftkHasPlane(reference_track->ftk_bitmask(),iplane) ) { continue; }
    dg::filleff( (format("dhit_%1%_wrt_%2%") % comp_name % ref_name).str() , 15 , 0 , 15 , iplane , 
                 detector::ftkHasPlane(comparison_track->ftk_bitmask(),iplane) , "HIT PLANE" );
  }
  // plot matching fraction for non-truth comparison tracks
  if( !(comparison_track->is_truth()) && !(reference_track->is_truth())) {
    dg::fillh( "barcode_matching_fraction" , 50 , -0.01 , 1.0001 , 50 , -0.01 , 1.0001 , 
               reference_track->barcode_matching_fraction() , comparison_track->barcode_matching_fraction() , 
               ref_name + " BARCODE MATCHING FRACTION" , comp_name + " BARCODE MATCHING FRACTION" );
  }

  // relationship between barcode matching fractions, if neither track is truth
  //   if( !(reference_track->is_truth()) && !(comparison_track->is_truth()) ) {
  //     dg::filleff( "same_barcode_rate" , comparison_track->unique_barcode()==reference_track->unique_barcode() );
  //     dg::fillh( "comparison_vs_reference_barcode_fraction" , 50 , 0 , 1.001 , 50 , 0 , 1.001 ,
  //                reference_track->barcode_matching_fraction() , comparison_track->barcode_matching_fraction() , 
  //                ref_name + " MATCHING FRACTION" , comp_name + " MATCHING FRACTION" );
  //   }


  // correlation between reconstruction differences (covariance matrix estimation)
  dg::fillh( "dphi_vs_dcurv" ,
             100 , -0.05 , 0.05 , 
             100 , -0.01 , 0.01 ,
             (comparison_track->curvature()-reference_track->curvature())*1000. , 
             comparison_track->phi0()-reference_track->phi0() , 
             comp_name + " CURVATURE - " + ref_name + " CURVATURE (1/GeV)" ,
             comp_name + " #phi_{0} - " + ref_name + " $phi_{0} (rad)" );
  dg::fillh( "dphi_vs_dd0" ,
             100 , -0.02 , 0.02 , 
             100 , -0.01 , 0.01 ,
             (comparison_track->d0()-reference_track->d0())/10. , 
             comparison_track->phi0()-reference_track->phi0() , 
             comp_name + " d_{0} - " + ref_name + " d_{0} (cm)" ,
             comp_name + " #phi_{0} - " + ref_name + " $phi_{0} (rad)" );
  dg::fillh( "dphi_vs_deta" ,
             100 , -0.02 , 0.02 , 
             100 , -0.01 , 0.01 ,
             comparison_track->eta()-reference_track->eta() , 
             comparison_track->phi0()-reference_track->phi0() , 
             comp_name + " #eta - " + ref_name + " #eta" ,
             comp_name + " #phi_{0} - " + ref_name + " $phi_{0} (rad)" );
  dg::fillh( "dphi_vs_dz0" ,
             100 , -0.1 , 0.1 , 
             100 , -0.01 , 0.01 ,
             comparison_track->z0()-reference_track->z0() , 
             comparison_track->phi0()-reference_track->phi0() , 
             comp_name + " z_{0} - " + ref_name + " z_{0}" ,
             comp_name + " #phi_{0} - " + ref_name + " $phi_{0} (rad)" );

  dg::fillh( "dd0_vs_dcurv" ,
             100 , -0.05 , 0.05 , 
             100 , -0.02 , 0.02 ,
             (comparison_track->curvature()-reference_track->curvature())*1000. , 
             (comparison_track->d0()-reference_track->d0())/10. , 
             comp_name + " CURVATURE - " + ref_name + " CURVATURE (1/GeV)" ,
             comp_name + " d_{0} - " + ref_name + " d_{0} (cm)" );
  dg::fillh( "dd0_vs_deta" ,
             100 , -0.02 , 0.02 , 
             100 , -0.02 , 0.02 ,
             comparison_track->eta()-reference_track->eta() , 
             (comparison_track->d0()-reference_track->d0())/10. , 
             comp_name + " #eta - " + ref_name + " #eta" ,
             comp_name + " d_{0} - " + ref_name + " d_{0} (cm)" );
  dg::fillh( "dd0_vs_dz0" ,
             100 , -0.1 , 0.1 , 
             100 , -0.02 , 0.02 ,
             comparison_track->z0()-reference_track->z0() , 
             (comparison_track->d0()-reference_track->d0())/10. , 
             comp_name + " z_{0} - " + ref_name + " z_{0}" ,
             comp_name + " d_{0} - " + ref_name + " d_{0} (cm)" );

  dg::fillh( "dcurv_vs_deta" ,
             100 , -0.02 , 0.02 , 
             100 , -0.05 , 0.05 ,
             comparison_track->eta()-reference_track->eta() , 
             (comparison_track->curvature()-reference_track->curvature())*1000. , 
             comp_name + " #eta - " + ref_name + " #eta" ,
             comp_name + " CURVATURE - " + ref_name + " CURVATURE (1/GeV)" );
  dg::fillh( "dcurv_vs_dz0" ,
             100 , -0.1 , 0.1 , 
             100 , -0.05 , 0.05 ,
             comparison_track->z0()-reference_track->z0() , 
             (comparison_track->curvature()-reference_track->curvature())*1000. , 
             comp_name + " z_{0} - " + ref_name + " z_{0}" ,
             comp_name + " CURVATURE - " + ref_name + " CURVATURE (1/GeV)" );


  dg::fillh( "dcurv_vs_deta" ,
             100 , -0.02 , 0.02 , 
             100 , -0.05 , 0.05 ,
             comparison_track->eta()-reference_track->eta() , 
             (comparison_track->curvature()-reference_track->curvature())*1000. , 
             comp_name + " #eta - " + ref_name + " #eta" ,
             comp_name + " CURVATURE - " + ref_name + " CURVATURE (1/GeV)" );
  dg::fillh( "deta_vs_dz0" ,
             100 , -0.02 , 0.02 , 
             100 , -0.05 , 0.05 ,
             comparison_track->z0()-reference_track->z0() , 
             comparison_track->eta()-reference_track->eta() ,
             comp_name + " z_{0} - " + ref_name + " z_{0}" ,
             comp_name + " #eta - " + ref_name + " #eta" );


}

void
old_track_comparison( const std::string& ref_name ,
                      const std::string& track_name ,
                      const std::vector< shared_ptr<const AnaTrack> >& track_collection_ref ,
                      const std::vector< shared_ptr<const AnaTrack> >& track_collection_comp )
{
  // This function does the matching based on Guido's criteria
  // (optimized for the barrel)
  unsigned int n_matching = 0u;
  unsigned int n_multiple_matching = 0u;
  BOOST_FOREACH( const shared_ptr<const AnaTrack>& track , track_collection_ref ) {
    // apply some cuts to the reference tracks used for the comparison/efficiencies.
    if( std::abs(track->eta()) > 1.0 ) { continue; }
    if( !track->is_truth() &&
	( track->barcode_matching_fraction() < AnaConfiguration::barcode_matching_minimum_cut() || 
	  track->barcode().event_index() != 0 ) ) { continue; }

    vector< shared_ptr<const AnaTrack> > matches;
    std::remove_copy_if( track_collection_comp.begin() , track_collection_comp.end() , back_inserter(matches) , 
                         bind( &AnaTrack::fails_old_parameter_matching_with , _1 ,track ) );
    // count the number of matches
    const unsigned int n_matches = matches.size();
    if( n_matches>0 ) { ++n_matching; }
    if( n_matches>1 ) { ++n_multiple_matching; }
    // histogram the number of reconstructed tracks matching this ref track
    dg::fillh( "n_old_matches_"+track_name , 10 , 0 , 10 ,
               n_matches , "N "+track_name+" TRACKS MATCHING " + track->algorithm().name() );

    StudyTrackingEfficiency st_eff( "old_" + track_name + "_wrt_" + ref_name  , "old " + track_name + " efficiency w.r.t. " + ref_name);
    st_eff.for_event( AnaEventMgr::current_event() );
    st_eff.for_only( StudyTrackingEfficiency::type( track , n_matches>0 ) );
    st_eff.repurpose( "old_" + track_name + "_wrt_" + ref_name + "_dup"  , "old " + track_name + " duplicates w.r.t. " + ref_name );
    st_eff.for_only( StudyTrackingEfficiency::type( track , n_matches>1 ) );

    if( n_matches == 1 ) {
      dg::down( "old_matching_tracks" , "matched comparison vs reference tracks" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      StudyTrack st_track( "old_"+track_name+"_wrt_"+ref_name+"_ref" , "reference tracks with a good match (old)" );
      st_track.for_only( track );
      const shared_ptr<const AnaTrack>& btrack( *(matches.begin()) );
      st_track.repurpose( "old_"+track_name+"_wrt_"+ref_name , "track parameters for "+track_name+" tracks matching (old) a "+ref_name+" track" );
      st_track.for_only( btrack );
      plot_track_parameter_differences( "old_"+track_name+"_wrt_"+ref_name , track , btrack );
    }
  }

  dg::fillh( "n_" + ref_name + "_with_old_matching_"+track_name+"_tracks" , 20 , 0 , 20 ,
             n_matching , "N "+ref_name+" WITH MATCHING "+track_name );
  dg::fillh( "n_" + ref_name + "_with_multiple_old_matching_"+track_name+"_tracks" , 20 , 0 , 20 ,
             n_multiple_matching , "N "+ref_name+" WITH MULTIPLE MATCHING "+track_name );
}

void
old_fake_comparison( const std::string& ref_name ,
		     const std::string& track_name ,
		     const std::vector< shared_ptr<const AnaTrack> >& track_collection_ref ,
		     const std::vector< shared_ptr<const AnaTrack> >& track_collection_comp ) {

  // This function does the matching based on Guido's criteria
  // (optimized for the barrel)
  BOOST_FOREACH( const shared_ptr<const AnaTrack>& track , track_collection_comp ) {
    if( AnaConfiguration::phi_fails_cut(track->phi0()) || 
	std::abs(track->eta()) > 1.0 ) continue;
    dg::down( "fakes" , "fake rate" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
    // fake rate vs kinematic variables.
    StudyTrackingEfficiency st_eff( "old_" + track_name + "_wrt_" + ref_name  , "old " + track_name + " fake rate w.r.t. " + ref_name );
    st_eff.for_event( AnaEventMgr::current_event() );
    vector< shared_ptr<const AnaTrack> > matches;
    std::remove_copy_if( track_collection_ref.begin() , track_collection_ref.end() , back_inserter(matches) , 
                         bind( &AnaTrack::fails_old_parameter_matching_with , _1 ,track ) );
    st_eff.for_only( StudyTrackingEfficiency::type( track , matches.size()==0 ) );
  }
}


void
study_jet_collection( const std::string& study_name , const std::vector< shared_ptr<const AnaJet> >& jets )
{
  dg::down( study_name , "jet collection studies for mel" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;

  StudyJet st( "jets" , "jets" );
  st.add_category( "jets_10gev" , bind( &AnaJet::et , _1 ) > 10 );
  st.add_category( "jets_20gev" , bind( &AnaJet::et , _1 ) > 20 );
  st.add_category( "jets_40gev" , bind( &AnaJet::et , _1 ) > 40 );
  st.add_category( "jets_70gev" , bind( &AnaJet::et , _1 ) > 70 );
  st.add_category( "jets_100gev" , bind( &AnaJet::et , _1 ) > 100 );

  st.for_each( jets.begin() , jets.end() );
  dg::fillh( "n_jets_10gev" , 500 , 0 , 500 , std::count_if( jets.begin() , jets.end() , bind( &AnaJet::et , _1 ) > 10 ) );
  dg::fillh( "n_jets_20gev" , 500 , 0 , 500 , std::count_if( jets.begin() , jets.end() , bind( &AnaJet::et , _1 ) > 20 ) );
  dg::fillh( "n_jets_40gev" , 500 , 0 , 500 , std::count_if( jets.begin() , jets.end() , bind( &AnaJet::et , _1 ) > 40 ) );
  dg::fillh( "n_jets_70gev" , 500 , 0 , 500 , std::count_if( jets.begin() , jets.end() , bind( &AnaJet::et , _1 ) > 70 ) );
  dg::fillh( "n_jets_100gev" , 500 , 0 , 500 , std::count_if( jets.begin() , jets.end() , bind( &AnaJet::et , _1 ) > 100 ) );
}

void
study_si_channels( const std::string& study_name , const std::vector< shared_ptr<const AnaSiChannel> >& si_channels )
{
  dg::down( study_name , "silicon channel studies for high luminosity" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
  
  for( unsigned int il=0; il!=11; ++il ) { 
    vector< shared_ptr<const AnaSiChannel> > lc;
    vector< shared_ptr<const AnaSiChannel> > lc_hard;
    // vector< shared_ptr<const AnaSiChannel> > lc_pileup;
    std::remove_copy_if( si_channels.begin() , si_channels.end() , back_inserter(lc) , bind( &AnaSiChannel::ftk_logical_layer , _1 ) != il );
    std::remove_copy_if( lc.begin() , lc.end() , back_inserter(lc_hard) , bind( &UniqueBarcode::is_primary , bind( &AnaSiChannel::barcode , _1 ) ) != true );
    // std::remove_copy_if( lc.begin() , lc.end() , back_inserter(lc_pileup) , bind( &UniqueBarcode::is_primary , bind( &AnaSiChannel::barcode , _1 ) ) == false );
    // count number of channels hit on this logical layer
    {
      dg::down( study_name+(boost::format("_layer%|d|")%il).str() , "individual layer plots" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      const unsigned int n_ch = lc.size();
      const unsigned int n_ch_barrel = std::count_if( lc.begin() , lc.end() , bind( &AnaSiChannel::barrel_ec , _1 ) == 0 );
      const unsigned int n_ch_endcap = n_ch - n_ch_barrel;
      const unsigned int n_ch_barrel_hard_scatter = std::count_if( lc_hard.begin() , lc_hard.end() , bind( &AnaSiChannel::barrel_ec , _1 ) == 0 );
      const unsigned int n_ch_endcap_hard_scatter = std::count_if( lc_hard.begin() , lc_hard.end() , bind( &AnaSiChannel::barrel_ec , _1 ) != 0 );
      const unsigned int n_ch_barrel_pileup_scatter = n_ch_barrel - n_ch_barrel_hard_scatter;
      const unsigned int n_ch_endcap_pileup_scatter = n_ch_endcap - n_ch_endcap_hard_scatter;
      const unsigned int n_ch_hard_scatter = n_ch_barrel_hard_scatter + n_ch_endcap_hard_scatter;
      const unsigned int n_ch_pileup_scatter = n_ch_barrel_pileup_scatter + n_ch_endcap_pileup_scatter;
      dg::fillh( "n_ch" , 100 , 0 , 1000 , n_ch );
      dg::fillh( "n_ch_wide" , 100 , 0 , 30000. , n_ch );
      dg::fillh( "n_ch_narrow" , 100 , 0 , 100. , n_ch );
      dg::fillh( "n_ch_barrel" , 100 , 0 , 1000 , n_ch_barrel );
      dg::fillh( "n_ch_barrel_wide" , 100 , 0 , 30000. , n_ch_barrel );
      dg::fillh( "n_ch_barrel_narrow" , 100 , 0 , 100. , n_ch_barrel );
      dg::fillh( "n_ch_endcap" , 100 , 0 , 1000 , n_ch_endcap );
      dg::fillh( "n_ch_endcap_wide" , 100 , 0 , 30000. , n_ch_endcap );
      dg::fillh( "n_ch_endcap_narrow" , 100 , 0 , 100. , n_ch_endcap );
      dg::fillh( "n_ch_hard" , 100 , 0 , 2000 , n_ch_hard_scatter );
      dg::fillh( "n_ch_hard_narrow" , 100 , 0 , 100 , n_ch_hard_scatter );
      dg::fillh( "n_ch_pileup" , 100 , 0 , 30000 , n_ch_pileup_scatter );
      dg::fillh( "n_ch_barrel_hard" , 100 , 0 , 2000 , n_ch_barrel_hard_scatter );
      dg::fillh( "n_ch_endcap_hard" , 100 , 0 , 2000 , n_ch_endcap_hard_scatter );
      dg::fillh( "n_ch_barrel_pileup" , 100 , 0 , 30000. , n_ch_barrel_pileup_scatter );
      dg::fillh( "n_ch_endcap_pileup" , 100 , 0 , 30000. , n_ch_endcap_pileup_scatter );
      // pixel tot
      if( il<3) { 
        BOOST_FOREACH( const shared_ptr<const AnaSiChannel>& ch , lc ) {
          if( ch->channel_type()!=AnaSiChannel::PIXEL ) { continue; }
          dg::down( ( ch->barcode().is_primary() ) ? "hard" : "pileup"  , "tot plots" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
          AnaSiChannel::ftype tot_corr = ch->tot_corr();
          dg::fillh( "tot" , 256 , 0 , 256 , ch->tot() );
          dg::fillh( "tot_corr" , 256 , 0 , 256 , tot_corr );
          if( ch->barrel_ec()==0 ) { 
            dg::fillh( "tot_barrel" , 256 , 0 , 256 , ch->tot() ); 
            dg::fillh( "tot_corr_barrel" , 256 , 0 , 256 , tot_corr ); 
          }
          if( ch->barrel_ec()!=0 ) { 
            dg::fillh( "tot_endcap" , 256 , 0 , 256 , ch->tot() ); 
            dg::fillh( "tot_corr_endcap" , 256 , 0 , 256 , tot_corr ); 
          }
          if( ch->highest_pt()>=1. ) {
            dg::fillh( "1gev_tot" , 256 , 0 , 256 , ch->tot() );
            dg::fillh( "1gev_tot_corr" , 256 , 0 , 256 , tot_corr );
            if( ch->barrel_ec()==0 ) { 
              dg::fillh( "1gev_tot_barrel" , 256 , 0 , 256 , ch->tot() ); 
              dg::fillh( "1gev_tot_corr_barrel" , 256 , 0 , 256 , tot_corr ); 
            }
            if( ch->barrel_ec()!=0 ) { 
              dg::fillh( "1gev_tot_endcap" , 256 , 0 , 256 , ch->tot() ); 
              dg::fillh( "1gev_tot_corr_endcap" , 256 , 0 , 256 , tot_corr ); 
            }
          }
          // try to find truth track
          vector< shared_ptr<const AnaTrack> >::const_iterator itruth = std::find_if( AnaEventMgr::current_event()->truth_tracks(AnaEventMgr::DEFAULT_SOURCE).begin() , 
                                                                                             AnaEventMgr::current_event()->truth_tracks(AnaEventMgr::DEFAULT_SOURCE).end() ,
                                                                                             bind( &AnaTrack::barcode , _1 ) == ch->barcode() );
          if( itruth != AnaEventMgr::current_event()->truth_tracks(AnaEventMgr::DEFAULT_SOURCE).end() ) { 
            const shared_ptr<const AnaTrack>& truth_track( *itruth );
            const double theta = truth_track->theta();
            dg::fillh( "tot" , 256 , 0 , 256 , ch->tot() );
            dg::fillh( "tot_corr" , 256 , 0 , 256 , tot_corr );
            dg::fillh( "tot_vs_lambda" , 50 , 0 , 5.0 , 100 , 0 , 100 , std::abs(truth_track->lambda()) , ch->tot() , "|#Lambda|" , "TOT" );
            dg::fillh( "tot_corr_vs_lambda" , 50 , 0 , 5.0 , 100 , 0 , 100 , std::abs(truth_track->lambda()) , tot_corr , "|#Lambda|" , "Corrected TOT" );
            if( ch->barrel_ec()==0 ) { 
              dg::fillh( "tot_barrel" , 256 , 0 , 256 , ch->tot() ); 
              dg::fillh( "tot_corr_barrel" , 256 , 0 , 256 , tot_corr ); 
              dg::fillh( "tot_norm_barrel" , 256 , 0 , 256 , ch->tot() * std::sin(theta) );
              dg::fillh( "tot_barrel_vs_eta" , 30 , 0 , 3 , 100 , 0 , 100 , std::abs(truth_track->eta()) , ch->tot() , "#eta" , "tot" ); 
              if( std::abs(truth_track->eta()) < 2.5 ) {
                dg::fillh( "tot_barrel_eta2.5" , 256 , 0 , 256 , ch->tot() ); 
                dg::fillh( "tot_corr_barrel_eta2.5" , 256 , 0 , 256 , tot_corr ); 
                dg::fillh( "tot_norm_barrel_eta2.5" , 256 , 0 , 256 , ch->tot() * std::sin(theta) );
              }
            }
            if( ch->barrel_ec()!=0 ) { 
              dg::fillh( "tot_endcap" , 256 , 0 , 256 , ch->tot() ); 
              dg::fillh( "tot_corr_endcap" , 256 , 0 , 256 , tot_corr ); 
              dg::fillh( "tot_norm_endcap" , 256 , 0 , 256 , ch->tot() * std::cos(theta) );
              if( std::abs(truth_track->eta()) < 2.5 ) {
                dg::fillh( "tot_endcap_eta2.5" , 256 , 0 , 256 , ch->tot() ); 
                dg::fillh( "tot_corr_endcap_eta2.5" , 256 , 0 , 256 , tot_corr ); 
                dg::fillh( "tot_norm_endcap_eta2.5" , 256 , 0 , 256 , ch->tot() * std::cos(theta) );
              }
            }            
          }
        }
      } // end plot pixel tot
    } // per layer plots
  } // for each layer
} // end do channel study

void
plot_track_parameter_diff_detail( const std::string& label_name ,
                                  const shared_ptr<const AnaTrack>& reference_track ,
                                  const shared_ptr<const AnaTrack>& comparison_track ) {
  dg::down( label_name , "track parameters" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
  plot_track_parameter_differences( "total" , reference_track , comparison_track );
  if( std::abs(reference_track->eta()) < 1.0 ) { plot_track_parameter_differences( "eta_lt_1" , reference_track , comparison_track ); }
  if( std::abs(reference_track->eta()) > 1.0 && std::abs(reference_track->eta()) < 1.3 ) { plot_track_parameter_differences( "eta_1_13" , reference_track , comparison_track );}
  if( std::abs(reference_track->eta()) > 1.3 && std::abs(reference_track->eta()) < 2.5 ) { plot_track_parameter_differences( "eta_13_25" , reference_track , comparison_track ); }
  if( std::abs(reference_track->z0()) < 21. ) { plot_track_parameter_differences( "z0_lt_21mm", reference_track , comparison_track ); }
}

#ifdef HAVE_LIBFTKSIM
void road_truth_association( const vector< boost::shared_ptr<const AnaRoad> >& roads, 
			     const vector< boost::shared_ptr<const AnaTrack> >& truth_tracks,
			     map< shared_ptr<const AnaRoad>, set< shared_ptr<const AnaTrack> > >& road_truth_map,
			     map< shared_ptr<const AnaTrack>, set< shared_ptr<const AnaRoad> > >& truth_road_map,
			     map< pair< shared_ptr<const AnaRoad>, shared_ptr<const AnaTrack> >, unsigned int >& n_ss_contributed ) {

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
	if( pairIter != n_ss_contributed.end() )
	  n_ss_contributed[road_and_truth]++;
	else
	  n_ss_contributed[road_and_truth] = 1;	

      } // end loop over hits
    } // end loop over planes
  } // end loop over roads
}

void road_truth_study( const std::string& dir_name, 
		       const vector< boost::shared_ptr<const AnaTrack> >& truth_tracks_primary, 
		       const map< shared_ptr<const AnaRoad>, set< shared_ptr<const AnaTrack> > >& road_truth_map,
		       const map< shared_ptr<const AnaTrack>, set< shared_ptr<const AnaRoad> > >& truth_road_map,
		       const map< pair< shared_ptr<const AnaRoad>, shared_ptr<const AnaTrack> >, unsigned int >& n_ss_contributed ) {

  dg::down(dir_name, "FTK roads vs truth");
  BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;

  // Loop over truth to do efficiency studies
  BOOST_FOREACH( const boost::shared_ptr<const AnaTrack>& truth , truth_tracks_primary ) {
    // Get the set of roads associated with this truth track
    map< shared_ptr<const AnaTrack>, set< shared_ptr<const AnaRoad> > >::const_iterator trmapiter = 
      truth_road_map.find(truth);
	
    vector<unsigned int> roadsWithAtLeastNHits(AnaEventMgr::current_event()->ftk_road_nplanes(),0);
    unsigned int min_hits(AnaEventMgr::current_event()->ftk_road_nplanes()), max_hits(0);
    set< shared_ptr<const AnaRoad> > matched_roads;
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
	if( nContributed < min_hits ) min_hits = nContributed;
	if( nContributed > max_hits ) max_hits = nContributed;
	
	for(unsigned int nhits = nContributed; nhits > 0; nhits--)
	  roadsWithAtLeastNHits[nhits-1]++;
	
	if( (float)nContributed / totNHits >= AnaConfiguration::barcode_matching_minimum_cut() )
	  matched_roads.insert(aRoad);
      }
    }
	  
    // Number of matched roads vs parameters
    double tmpPhi = detector::fmodd(static_cast<double>(truth->phi0()), 2*M_PI/52.,0.);
    unsigned int nRoads = matched_roads.size();
    dg::fillh("nroads_vs_phi", 200, -M_PI, M_PI, 30, 1, 31, truth->phi0(), nRoads,
	      "truth phi", "# FTK Roads" );
    dg::fillh("nroads_vs_phiM52", 200, 0, M_PI/26., 30, 1, 31, tmpPhi, nRoads,
	      "truth phi mod 2pi/52", "# FTK Roads" );
    if( truth->pt() > 5 ) {
      dg::fillh("nroads_vs_phi_hipt", 200, -M_PI, M_PI, 30, 1, 31, truth->phi0(), nRoads,
		"truth phi", "# FTK Roads" );
      dg::fillh("nroads_vs_phiM52_hipt", 200, 0, M_PI/26., 30, 1, 31,  tmpPhi, nRoads,
		"truth phi mod 2pi/52", "# FTK Roads" );
    }
    if( std::abs(truth->eta()) < 1.0 ) {
      dg::fillh("nroads_vs_phi_central", 200, -M_PI, M_PI, 30, 1, 31, truth->phi0(), nRoads,
		"truth phi", "# FTK Roads" );
      dg::fillh("nroads_vs_phiM52_central", 200, 0, M_PI/26., 30, 1, 31,  tmpPhi, nRoads,
		"truth phi mod 2pi/52", "# FTK Roads" );
      if( truth->pt() > 5 ) {
	dg::fillh("nroads_vs_phi_central_hipt", 200, -M_PI, M_PI, 30, 1, 31, truth->phi0(), nRoads,
		  "truth phi", "# FTK Roads" );
	dg::fillh("nroads_vs_phiM52_central_hipt", 200, 0, M_PI/26., 30, 1, 31,  tmpPhi, nRoads,
		  "truth phi mod 2pi/52", "# FTK Roads" );
      }
    }
    else {
      dg::fillh("nroads_vs_phi_forward", 200, -M_PI, M_PI, 30, 1, 31, truth->phi0(), nRoads,
		"truth phi", "# FTK Roads" );
      dg::fillh("nroads_vs_phiM52_forward", 200, 0, M_PI/26., 30, 1, 31,  tmpPhi, nRoads,
		"truth phi mod 2pi/52", "# FTK Roads" );
      if( truth->pt() > 5 ) {
	dg::fillh("nroads_vs_phi_forward_hipt", 200, -M_PI, M_PI, 30, 1, 31, truth->phi0(), nRoads,
		  "truth phi", "# FTK Roads" );
	dg::fillh("nroads_vs_phiM52_forward_hipt", 200, 0, M_PI/26., 30, 1, 31,  tmpPhi, nRoads,
		  "truth phi mod 2pi/52", "# FTK Roads" );
      }
    }
    dg::fillh("nroads_vs_eta", 25, -2.5, 2.5, 30, 1, 31, truth->eta(), nRoads,
	      "truth eta", "# FTK Roads" );
    dg::fillh("nroads_vs_qoverpt", 25, -1, 1, 30, 0, 30, truth->curvature()*1000., nRoads,
	      "truth q/pT (1/GeV)", "# FTK Roads" );
    if( std::abs(truth->eta()) < 1.0 ) {
      dg::fillh("nroads_vs_qoverpt_central", 25, -1, 1, 30, 0, 30, truth->curvature()*1000., nRoads,
		"truth q/pT (1/GeV)", "# FTK Roads" );
    }
    else {
      dg::fillh("nroads_vs_qoverpt_forward", 25, -1, 1, 30, 0, 30, truth->curvature()*1000., nRoads,
		"truth q/pT (1/GeV)", "# FTK Roads" );
    }
    
    // Maximum number of hits contributed to a road vs params
    dg::fillh("maxhits_vs_eta", 25, -2.5, 2.5, 11, 1, 12, truth->eta(), max_hits,
	      "truth #eta", "Maxmimum number of hits contributed to a road" );
    dg::fillh("maxhits_vs_phi", 50, -M_PI, M_PI, 11, 1, 12, truth->phi0(), max_hits,
	      "truth #phi", "Maxmimum number of hits contributed to a road" );
    dg::fillh("maxhits_vs_curv", 50, -1, 1, 11, 1, 12, truth->curvature()*1000., max_hits,
	      "truth q/p_{T} (1/GeV)", "Maxmimum number of hits contributed to a road" );
    dg::fillh("maxhits_vs_pT", 25, 0, 25, 11, 1, 12, truth->pt(), max_hits,
	      "truth p_{T}", "Maxmimum number of hits contributed to a road" );
    dg::fillh("maxhits_vs_z0", 24, -120, 120, 11, 1, 12, truth->z0(), max_hits,
	      "truth z_{0}", "Maxmimum number of hits contributed to a road" );
    
    // Efficiency studies
    StudyTrackingEfficiency road_eff( "FTK_ROADS_VS_TRUTH" , "FTK ROAD efficiency w.r.t. truth" );
    road_eff.for_event( AnaEventMgr::current_event() );
    road_eff.add_constant_category( "muon" , abs(truth->pdg_code()) == 13 );
    road_eff.add_constant_category( "pion" , abs(truth->pdg_code()) == 211 );
    road_eff.add_constant_category( "electron" , abs(truth->pdg_code()) == 11 );
    road_eff.add_constant_category( "barrel" , std::abs(truth->eta()) < 1.0 );
    road_eff.for_only( StudyTrackingEfficiency::type( truth , matched_roads.size()>0 ) );
    // Check the ones that are only missing one hit
    set< shared_ptr<const AnaRoad> > subset_matched_roads;
    std::remove_copy_if( matched_roads.begin() , matched_roads.end() , inserter(subset_matched_roads,subset_matched_roads.begin()) ,
			 bind(&AnaRoad::n_hits,_1) < (AnaEventMgr::current_event()->ftk_road_nplanes()-1) );
    road_eff.repurpose( "FTK_ROADS_VS_TRUTH_miss1" , "FTK ROAD efficiency w.r.t. truth; only 1 missing hit" );
    road_eff.for_only( StudyTrackingEfficiency::type( truth , subset_matched_roads.size()>0 ) );
    if( AnaEventMgr::current_event()->ftk_road_nplanes() > 7 ) { // option B first stage or 11L
      // Now check the ones that have a missing innermost sct pair
      BOOST_FOREACH( shared_ptr<const AnaRoad> aRoad, matched_roads ) {
	if( (aRoad->bitmask() & 0x18) == 0  ) {
	  subset_matched_roads.insert(aRoad);
	}
      }
      road_eff.repurpose( "FTK_ROADS_VS_TRUTH_sct_miss_inner_pair" , "FTK ROAD efficiency w.r.t. truth; missing 1 hit or inner sct pair" );
      road_eff.for_only( StudyTrackingEfficiency::type( truth , subset_matched_roads.size()>0 ) );
      // Now check the ones that have a missing outermost sct pair
      subset_matched_roads.clear();
      std::remove_copy_if( matched_roads.begin() , matched_roads.end() , inserter(subset_matched_roads,subset_matched_roads.begin()) ,
			   bind(&AnaRoad::n_hits,_1) < (AnaEventMgr::current_event()->ftk_road_nplanes()-1) );
      BOOST_FOREACH( shared_ptr<const AnaRoad> aRoad, matched_roads ) {
	if( (aRoad->bitmask() & 0x600) == 0 ) {
	  subset_matched_roads.insert(aRoad);
	}
      }
      road_eff.repurpose( "FTK_ROADS_VS_TRUTH_sct_miss_outer_pair" , "FTK ROAD efficiency w.r.t. truth; missing 1 hit or outer sct pair" );
      road_eff.for_only( StudyTrackingEfficiency::type( truth , subset_matched_roads.size()>0 ) );
    }
    
    // Distributions where the matching fails:
    if( matched_roads.size() == 0 ) {
      dg::fillh("no_roads_phi", 100, -M_PI, M_PI, truth->phi0(),  "#phi (radians)");
      dg::fillh("no_roads_d0", 88 , -2.2 , 2.2 , truth->d0() ,  "d_{0} (mm)");
      dg::fillh("no_roads_z0" , 96 , -12 , 12 , truth->z0()/10. ,  "z_{0} (cm)");
      dg::fillh("no_roads_eta", 100 , -2.5 , 2.5 , truth->eta() , "#eta" );
      dg::fillh("no_roads_pt" , 100, 0, 25, truth->pt(),  "pT (GeV)");
      dg::fillh("no_roads_curv" , 100, -1., 1, 1000.*truth->curvature(), "q/pT (1/GeV)");
      
      // FlagJT output event number and truth track barcode if there a missing match in the barrel
      if( false && std::abs(truth->eta()) < 1.0 ) {
	cout << "FLAGJT\tEvent " << AnaEventMgr::current_event()->event_manager_index()
	     << "\tBarcode " << truth->barcode().barcode() << endl;
      }
    }
    // Now do efficiency versus number of hits in the roads
    for(unsigned int i = 0; i < AnaEventMgr::current_event()->ftk_road_nplanes(); i++) {
      road_eff.repurpose( (boost::format("FTK_ROADS_VS_TRUTH_contrib_%1%")%(i+1)).str() , 
			  (boost::format("Efficiency to contribute at least %1% hit(s) to a road")%(i+1)).str() );
      road_eff.for_only( StudyTrackingEfficiency::type( truth , roadsWithAtLeastNHits[i]>0 ) );
    }
	
    // Duplicate rate
    road_eff.repurpose("DUP_FTK_ROADS_VS_TRUTH" , "FTK ROAD duplicate rate w.r.t. truth" );
    road_eff.for_only( StudyTrackingEfficiency::type( truth , matched_roads.size()>1 ) );
    
    // Do efficiency relative to offline too
    // vector< boost::shared_ptr<const AnaTrack> > offline_matches;
    // std::remove_copy_if( offline_tracks.begin() , offline_tracks.end() , back_inserter(offline_matches) , 
    // 			 bind(&AnaTrack::barcode,_1) != truth->barcode() ||
    // 			 bind(&AnaTrack::barcode_matching_fraction,_1) < AnaConfiguration::barcode_matching_minimum_cut() );
    // if( offline_matches.size()>0 ) { // Don't care if offline didn't match
    //   road_eff.repurpose( "ftk_roads_wrt_offline" , "FTK Road efficiency w.r.t. offline" );
    //   road_eff.for_only( StudyTrackingEfficiency::type( truth , nRoads>0 ) );
    //   road_eff.repurpose( "dup_ftk_roads_wrt_offline" , "FTK Road duplicates w.r.t. offline" );
    //   road_eff.for_only( StudyTrackingEfficiency::type( truth , nRoads>1 ) );
    //   if( nRoads == 0 ) {
    // 	StudyTrack study_track( "no_offline_match" , "Properties of truth tracks found by offline but not matched to FTK roads" );
    // 	study_track.for_only( truth );
    //   }
    // }
    
    // offline_matches.clear();
    // std::remove_copy_if( offline_ftklike_tracks.begin() , offline_ftklike_tracks.end() , back_inserter(offline_matches) , 
    // 			 bind(&AnaTrack::barcode,_1) != truth->barcode() ||
    // 			 bind(&AnaTrack::barcode_matching_fraction,_1) < AnaConfiguration::barcode_matching_minimum_cut() );
    // if( offline_matches.size()>0 ) { // Don't care if offline didn't match
    //   road_eff.repurpose( "ftk_roads_wrt_ftklike_offline" , "FTK Road efficiency w.r.t. ftklike offline" );
    //   road_eff.for_only( StudyTrackingEfficiency::type( truth , nRoads>0 ) );
    //   road_eff.repurpose( "dup_ftk_roads_wrt_ftklike_offline" , "FTK Road duplicates w.r.t. ftklike offline" );
    //   road_eff.for_only( StudyTrackingEfficiency::type( truth , nRoads>1 ) );
    //   if( nRoads == 0 ) {
    // 	StudyTrack study_track( "no_ftklike_offline_match" , "Properties of truth tracks found by ftklike offline but not matched to FTK roads" );
    // 	    study_track.for_only( truth );
    //   }
    // }
  } // End loop over truth
}

void road_study( const vector< boost::shared_ptr<const AnaRoad> >& ftk_roads, const std::string& dir_name, 
		 const map< shared_ptr<const AnaRoad>, set< shared_ptr<const AnaTrack> > >& road_truth_map,
		 const map< shared_ptr<const AnaTrack>, set< shared_ptr<const AnaRoad> > >& truth_road_map,
		 const map< pair< shared_ptr<const AnaRoad>, shared_ptr<const AnaTrack> >, unsigned int >& n_ss_contributed ) {

  dg::down(dir_name, "FTK roads/fakes");
  BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;

  // Basic road properties and fake rate studies
  long int n_words_per_event(0);
  BOOST_FOREACH( boost::shared_ptr<const AnaRoad> road, ftk_roads ) {
    dg::filleff( "rw_rejection_rate" , road->rw_rejected() );
    dg::fillh( "n_hits_in_road" , 100 , 0 , 100 , road->n_hits() , "N HITS IN ROAD" );
    dg::fillh( "n_words_per_road", 100, 0, 100, (1+road->n_hits()), "N WORDS PER ROAD" );
    dg::fillh( "n_comb_per_road", 100, 0, 100, (road->n_comb()), "N COMBINATIONS PER ROAD" );
    n_words_per_event += (1+road->n_hits());
    for( unsigned int iplane=0, fplane=road->n_planes(); iplane!=fplane; ++iplane ) {
      dg::filleff( "hiteff_vs_plane" , 14 , 0 , 14 , iplane , detector::ftkHasPlane(road->bitmask(),iplane) , "PLANE" );
    }
    
    dg::down("fake_rate_studies", "fake roads");
    BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
	
    // Get the set of truth tracks associated with this road
    map< shared_ptr<const AnaRoad>, set< shared_ptr<const AnaTrack> > >::const_iterator rtmapiter = 
      road_truth_map.find(road);
	
    bool is_fake(true);
    unsigned int totNHits( road->n_hits() );
    if( rtmapiter != road_truth_map.end() ) {
      const set< shared_ptr<const AnaTrack> >& truth_contributors(rtmapiter->second);
      BOOST_FOREACH( shared_ptr<const AnaTrack> aTruth, truth_contributors ) {
	std::pair< shared_ptr<const AnaRoad>, shared_ptr<const AnaTrack> > mp(road,aTruth);
	map< pair< shared_ptr<const AnaRoad>, shared_ptr<const AnaTrack> >, unsigned int >::const_iterator it = 
	  n_ss_contributed.find(mp);
	assert(it != n_ss_contributed.end());
	//unsigned int nContributed = n_ss_contributed[make_pair< shared_ptr<const AnaRoad>, shared_ptr<const AnaTrack> >(road,aTruth)];
	unsigned int nContributed = it->second;
	assert( nContributed != 0 );
	assert( nContributed <= AnaEventMgr::current_event()->ftk_road_nplanes() );
	    
	if( (float)nContributed / totNHits >= AnaConfiguration::barcode_matching_minimum_cut() ) {
	  is_fake = false;
	  break;
	}
      }
    }
    dg::filleff("fake_rate", is_fake);
    dg::filleff("fakes_vs_sector_id", 250, 0, 125000, road->sector_id(), is_fake, "Sector ID" );
  } // end loop over roads
  dg::fillh("n_words_per_event", 500, 0, 10000000, n_words_per_event, "N WORDS PER EVENT" );
  dg::fillh("n_words_per_event_by_crate", 500, 0, 2500000, n_words_per_event/8.0, "N WORDS PER EVENT (PER CRATE)" );
}

#endif // HAVE_LIBFTKSIM
