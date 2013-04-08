
#include <algorithm>
#include <functional>
#include <cassert>
#include <cmath>
#include <string>
#include <bitset>
#include <set>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/scope_exit.hpp>
#include <boost/foreach.hpp>
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/assign/std/set.hpp>
#include <ctime>
#include <TStopwatch.h>
#include <iostream>
#include <iomanip>
#include "TFile.h"
#include "TH1F.h"
#include "TrigFTKAna/AnaConfiguration.hpp"
#include "TrigFTKAna/AnaEventMgr.hpp"
#include "TrigFTKAna/AnaTrack.hpp"
#include "TrigFTKAna/AnaJet.hpp"
#include "DataGraphics/DataGraphics.hh"
#include "TrigFTKAna/StudyTrack.hpp"
#include "TrigFTKAna/StudyTrackingEfficiency.hpp"
#include "TrigFTKAna/StudyTrackDisplacement.hpp"
#include "TrigFTKAna/StudyTrackComparison.hpp"
#include "TrigFTKAna/StudyVertexFit.hpp"
#include "TrigFTKAna/SignedD0JetTagger.hpp"
#include "TrigFTKAna/SignedD0ParameterizedJetTagger.hpp"
#include "TrigFTKAna/LikelihoodParameterization1D.hpp"
#include "TrigFTKAna/AnaTaggerMgr.hpp"
#include "TrigFTKAna/AnaVertexFit.hpp"

using namespace std;
using namespace DataGraphics;
using boost::bind;

void study_tagger( const std::string& name ,  const boost::shared_ptr<const AnaJet>& jet , 
                   const std::vector< boost::shared_ptr<const AnaTrack> >& associated_tracks ,
                   const std::vector< boost::shared_ptr<const AnaTrack> >& associated_truth_tracks ,
                   const std::string dgcoll = std::string() );

void study_jet_tracks( const std::string& name , const boost::shared_ptr<const AnaJet>& jet , 
                       const std::vector< boost::shared_ptr<const AnaTrack> >& associated_tracks ,
                       const std::vector< boost::shared_ptr<const AnaTrack> >& associated_truth_tracks );

void study_vertexing( const std::string& name , 
                      vector< boost::shared_ptr<const AnaTrack> > associated_tracks , 
                      const boost::shared_ptr<const AnaJet>& jet 
                      );

// global pointer to current event (a shortcut used only in this file)
boost::shared_ptr<const AnaEvent> global_evt;

// define method to associate tracks with a jet
template<typename trackCollT>
class TrackAssociator {
private:
  const trackCollT& _coll;
public:
  TrackAssociator(trackCollT& coll) : _coll(coll) {}
  const std::vector< boost::shared_ptr<const AnaTrack> > operator()( const boost::shared_ptr<const AnaJet>& jet , const double radius = 0.4 ) const {
    std::vector< boost::shared_ptr<const AnaTrack> > result;
    BOOST_FOREACH( const boost::shared_ptr<const AnaTrack>& track , _coll ) {
      const double DELTA_ETA_ASSOCIATION = radius;
      const double DELTA_PHI_ASSOCIATION = radius;
      if( std::abs( track->d0() ) > 1. ) { continue; }
      // if( std::abs( track->d0() ) > 2. ) { continue; }
      if( std::abs( track->pt() ) < AnaConfiguration::minimum_track_pt_cut() ) { continue; }
      if( std::abs( track->eta() ) > AnaConfiguration::maximum_track_abseta_cut() ) { continue; }
      //if( track->is_offline() && std::abs( track->nhits()<5 ) ) { continue; }
      const double deta = detector::delta_eta( jet->eta() , track->eta() );
      const double dphi = detector::delta_phi( jet->phi() , track->phi0() );
      if( std::abs(deta)>DELTA_ETA_ASSOCIATION ) { continue; }
      if( std::abs(dphi)>DELTA_PHI_ASSOCIATION ) { continue; }
      // added 20090123
      const double dr = detector::quadrature( deta , dphi );
      if( dr > radius ) { continue; }
      result.push_back( track );
    }
    return result;
  }
};

// prune tracks with large delta z0
class TrackPruner {
private:
public:
  TrackPruner() {}
  const unsigned int prune( std::vector< boost::shared_ptr<const AnaTrack> >& tracks , const double& max_dz0 ) const {
    if( tracks.empty() ) { return 0; }
    // find highest pt track
    vector< boost::shared_ptr<const AnaTrack> >::iterator imaxpt = std::max_element( tracks.begin() , tracks.end() , AnaTrack::ptLessThanV() );
    // loop over all other tracks. copy only those which pass the dz0 cut.
    vector< boost::shared_ptr<const AnaTrack> > result;
    result.reserve( tracks.size() );
    unsigned int npruned = 0u;
    BOOST_FOREACH( const boost::shared_ptr<const AnaTrack>& track , tracks ) {
      if( std::abs( track->z0() - (*imaxpt)->z0() ) > max_dz0 ) { ++npruned; continue; }
      result.push_back( track );
    }
    result.swap( tracks );
    return npruned;
  }
};

// set of possible taggers
AnaTaggerMgr tagger_mgr;

void standard_truth_comparison( const std::string& study_name , const std::string& truth_name , 
                                const std::string& track_name ,
                                const std::vector< boost::shared_ptr<const AnaTrack> >& truth_collection , 
                                const std::vector< boost::shared_ptr<const AnaTrack> >& track_collection ,
                                const boost::shared_ptr<const AnaJet> jet = boost::shared_ptr<const AnaJet>() );
void standard_jet_tracks( const std::string& study_name , const std::string& track_name ,
                          const std::vector< boost::shared_ptr<const AnaTrack> >& track_collection ,
                          const boost::shared_ptr<const AnaJet> jet = boost::shared_ptr<const AnaJet>() );

void plot_track_parameter_differences( const std::string& label_name , 
                                       const boost::shared_ptr<const AnaTrack>& reference_track , 
                                       const boost::shared_ptr<const AnaTrack>& comparison_track );


////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C"
#endif
int
F77_MAIN( int argc , char* argv[] )
{

  argc = std::distance( argv , std::find( argv , argv+256 , (char*)0 ) ); // HACK: fortran is passing a nonsense argc.
  AnaConfiguration::configure("ana_b_tagging",argc,argv);
  if( !AnaConfiguration::configured() ) { exit(0); }
  dg::root()->no_save_at_exit();
  BOOST_SCOPE_EXIT() { dg::root()->do_save_at_exit(); } BOOST_SCOPE_EXIT_END;

  unsigned long n_events = 0ul;

  // initialize possilble taggers
  if( AnaConfiguration::train_tagger() ) {
    tagger_mgr.add<SignedD0JetTagger>( static_cast<const string&>("FTK_D0_TAGGER") );
    tagger_mgr.add<SignedD0JetTagger>( static_cast<const string&>("FTK_D0_SIGNIFICANCE_TAGGER") );
    tagger_mgr.add<SignedD0JetTagger>( static_cast<const string&>("FTK_SIGNEDD0_TAGGER") );
    tagger_mgr.add<SignedD0JetTagger>( static_cast<const string&>("FTK_SIGNEDD0_SIGNIFICANCE_TAGGER") );
    tagger_mgr.add<SignedD0JetTagger>( static_cast<const string&>("FTK_SIGNEDD0_TRACKCONE_TAGGER") );
    tagger_mgr.add<SignedD0JetTagger>( static_cast<const string&>("FTK_SIGNEDD0_SIGNIFICANCE_TRACKCONE_TAGGER") );
    tagger_mgr.add<SignedD0JetTagger>( static_cast<const string&>("FTK_SIGNEDD0_SIGNIFICANCE_PURITY_TRACKCONE_TAGGER") );
    tagger_mgr.add<SignedD0JetTagger>( static_cast<const string&>("FTK_SIGNEDD0_SIGNIFICANCE_TRACKCONE_TAGGER.CHI2LT2") );
    tagger_mgr.add<SignedD0JetTagger>( static_cast<const string&>("FTK_SIGNEDD0_SIGNIFICANCE_TRACKCONE_TAGGER.FULLFIT") );
    tagger_mgr.add<SignedD0JetTagger>( static_cast<const string&>("OFFLINE_D0_TAGGER") );
    tagger_mgr.add<SignedD0JetTagger>( static_cast<const string&>("OFFLINE_D0_SIGNIFICANCE_TAGGER") );
    tagger_mgr.add<SignedD0JetTagger>( static_cast<const string&>("OFFLINE_SIGNEDD0_TAGGER") );
    tagger_mgr.add<SignedD0JetTagger>( static_cast<const string&>("OFFLINE_SIGNEDD0_SIGNIFICANCE_TAGGER") );
    tagger_mgr.add<SignedD0JetTagger>( static_cast<const string&>("OFFLINE_SIGNEDD0_TRACKCONE_TAGGER_ALL") );
    tagger_mgr.add<SignedD0JetTagger>( static_cast<const string&>("OFFLINE_SIGNEDD0_TRACKCONE_TAGGER") );
    tagger_mgr.add<SignedD0JetTagger>( static_cast<const string&>("OFFLINE_SIGNEDD0_TRACKCONE_TAGGER_FTKLIKE") );
    tagger_mgr.add<SignedD0JetTagger>( static_cast<const string&>("OFFLINE_SIGNEDD0_SIGNIFICANCE_TRACKCONE_TAGGER") );
    tagger_mgr.add<SignedD0JetTagger>( static_cast<const string&>("OFFLINE_SIGNEDD0_SIGNIFICANCE_TRACKCONE_SV1_TAGGER_ALL") );
    tagger_mgr.add<SignedD0JetTagger>( static_cast<const string&>("OFFLINE_SIGNEDD0_Z0_SIGNIFICANCE_TRACKCONE_SV1_TAGGER_ALL") );
    tagger_mgr.add<SignedD0JetTagger>( static_cast<const string&>("OFFLINE_SIGNED_Z0_SIGNIFICANCE_TRACKCONE_SV1_TAGGER_ALL") );
    tagger_mgr.add<SignedD0JetTagger>( static_cast<const string&>("OFFLINE_SIGNEDD0_SIGNIFICANCE_TRACKCONE_TAGGER") );
    tagger_mgr.add<SignedD0JetTagger>( static_cast<const string&>("OFFLINE_SIGNEDD0_SIGNIFICANCE_PURITY_TRACKCONE_TAGGER") );
    tagger_mgr.add<SignedD0JetTagger>( static_cast<const string&>("OFFLINE_SIGNEDD0_SIGNIFICANCE_TRACKCONE_TAGGER_ALL") );
    tagger_mgr.add<SignedD0JetTagger>( static_cast<const string&>("OFFLINE_SIGNEDD0_Z0_SIGNIFICANCE_TRACKCONE_TAGGER_ALL") );
    tagger_mgr.add<SignedD0JetTagger>( static_cast<const string&>("OFFLINE_SIGNED_Z0_SIGNIFICANCE_TRACKCONE_TAGGER_ALL") );
    tagger_mgr.add<SignedD0JetTagger>( static_cast<const string&>("IDSCAN_SIGNEDD0_TAGGER") );
    tagger_mgr.add<SignedD0JetTagger>( static_cast<const string&>("IDSCAN_SIGNEDD0_SIGNIFICANCE_TAGGER") );
    tagger_mgr.add<SignedD0JetTagger>( static_cast<const string&>("TRUTH_SIGNEDD0_TAGGER") );
    tagger_mgr.add<SignedD0JetTagger>( static_cast<const string&>("TRUTH_SIGNEDD0_SIGNIFICANCE_TAGGER") );
  } else {
    cout << " loading tagger training from " << AnaConfiguration::trained_tagger_file() << "..." << endl;
    tagger_mgr.load( AnaConfiguration::trained_tagger_file() );
    // dump the loaded taggers.
    if( true ) { 
      for( AnaTaggerMgr::const_iterator i=tagger_mgr.begin(), f=tagger_mgr.end(); i!=f; ++i ) {
        const boost::shared_ptr<JetTagger>& tagger( tagger_mgr.get_tagger(i) );
        const SignedD0JetTagger* sd0_tagger( dynamic_cast<const SignedD0JetTagger*>( tagger.get() ) );
        const SignedD0ParameterizedJetTagger* sd0_param_tagger( dynamic_cast<const SignedD0ParameterizedJetTagger*>( tagger.get() ) );
        if( sd0_tagger ) {
          cout << " Loaded tagger: " << tagger_mgr.get_name(i) << endl;
          const Likelihood1D& blik( sd0_tagger->b_d0_likelihood() );
          const Likelihood1D& notblik( sd0_tagger->notb_d0_likelihood() );
          const unsigned int btot = std::accumulate( blik.begin() , blik.end() , 0u );
          const unsigned int notbtot = std::accumulate( notblik.begin() , notblik.end() , 0u );
          cout << " b likelihood: " << endl;
          for( Likelihood1D::const_iterator ibin=blik.begin(), fbin=blik.end(); ibin!=fbin; ++ibin ) {
            cout << boost::format( "%5d  [%8f]" ) % (*ibin) % ( (*ibin)/((float)btot) ) << " ";
          }
          cout << endl;
          cout << " not b likelihood: " << endl;
          for( Likelihood1D::const_iterator ibin=notblik.begin(), fbin=notblik.end(); ibin!=fbin; ++ibin ) {
            cout << boost::format( "%5d  [%8f]" ) % (*ibin) % ( (*ibin)/((float)notbtot) ) << " ";
          }
          cout << endl;
        } else if( sd0_param_tagger ) {
          cout << " Loaded tagger: " << tagger_mgr.get_name(i) << endl;
          const LikelihoodParameterization1D& blik( sd0_param_tagger->b_d0_likelihood() );
          const LikelihoodParameterization1D& notblik( sd0_param_tagger->notb_d0_likelihood() );
          cout << " b likelihood: " << endl;
          blik.print( cout );
          cout << " not b likelihood: " << endl;
          notblik.print( cout );
        }
      } // end for each taqger, dump it to cout
    } // end dump possible taggers
  } // end else load trained taggers from a a file


  // loop

  AnaEventMgr* evtmgr = AnaEventMgr::instance(); assert(evtmgr);
  for( AnaEventMgr::iterator i=evtmgr->begin(), f=evtmgr->end(AnaConfiguration::user_max_nevents()); i!=f; ++i, ++n_events ) {

    const boost::shared_ptr<const AnaEvent> evt( *i );
    if( !evt ) { break; } // no AnaEvent means something went wrong, e.g. premature end of file
    global_evt = evt;

    const bool maintain_stat_independence = ( ( AnaConfiguration::train_tagger() && (n_events%2)==1 ) ||
                                              ( !AnaConfiguration::train_tagger() && (n_events%2)==0 ) );

    dg::filleff( "reject_event" , maintain_stat_independence );

    if( maintain_stat_independence ) { continue; }

    // split dataset into training and evaluation subsets.
    // if( AnaConfiguration::train_tagger() && (n_events%3)!=0 ) { continue; }
    // if( !AnaConfiguration::train_tagger() && (n_events%3)==0 ) { continue; }

    const vector< boost::shared_ptr<const AnaTrack> >& ftk_tracks_all( evt->ftk_tracks() );
    const vector< boost::shared_ptr<const AnaTrack> >& idscan_tracks( evt->l2_idscan_tracks() );
    const vector< boost::shared_ptr<const AnaTrack> >& offline_tracks_all( evt->offline_tracks() );
    const vector< boost::shared_ptr<const AnaJet> >& roi_jets( evt->l1_roi_jets() );
    const vector< boost::shared_ptr<const AnaJet> >& cone_jets( evt->atlas_cone_4_truth_jets() ); 
    
    // use truth jets for high lumi (avoid clustering problems)

    const vector< boost::shared_ptr<const AnaTrack> >& truth_tracks_all( evt->truth_tracks() );

    vector< boost::shared_ptr<const AnaTrack> > truth_tracks;
    truth_tracks.reserve( truth_tracks_all.size() );
    std::remove_copy_if( truth_tracks_all.begin() , truth_tracks_all.end() , back_inserter(truth_tracks) ,
                         bind(&AnaTrack::detpaper,_1) == false || // implies a 2mm cut on the primary-corrected d0
                         bind( std::ptr_fun<AnaTrack::ftype,AnaTrack::ftype>(std::abs) , bind(&AnaTrack::eta,_1)) > AnaConfiguration::maximum_track_abseta_cut() ||
                         bind(&UniqueBarcode::event_index,bind(&AnaTrack::barcode,_1))!=0 );

    // apply quality cuts to offline tracks if requested
    vector< boost::shared_ptr<const AnaTrack> > offline_tracks;
    vector< boost::shared_ptr<const AnaTrack> > offline_tracks_tight;
    vector< boost::shared_ptr<const AnaTrack> > offline_tracks_ftklike;
    offline_tracks.reserve( offline_tracks_all.size() );
    std::remove_copy_if( offline_tracks_all.begin() , offline_tracks_all.end() , back_inserter(offline_tracks) ,
                         bind(&AnaTrack::nhits,_1) < 7 || 
                         bind(&AnaTrack::pixel_nhits,_1) < 2 || 
                         bind(&AnaTrack::b_layer_nhits,_1) < 1 || 
                         bind( std::ptr_fun<AnaTrack::ftype,AnaTrack::ftype>(std::abs) , bind(&AnaTrack::d0,_1)) > 1 ||
                         bind( std::ptr_fun<AnaTrack::ftype,AnaTrack::ftype>(std::abs) , bind(&AnaTrack::eta,_1)) > AnaConfiguration::maximum_track_abseta_cut() );
    offline_tracks_tight.reserve( offline_tracks.size() );
    std::remove_copy_if( offline_tracks.begin() , offline_tracks.end() , back_inserter(offline_tracks_tight) ,
                         bind(&AnaTrack::nhits,_1) < 9 || 
                         bind(&AnaTrack::nholes,_1) > 2 || 
                         bind(&AnaTrack::pixel_nholes,_1) > 0 || 
                         // bind(&AnaTrack::chi2ndof,_1)  > 5 || // FIXME: test for mel, 20100215
                         bind( std::ptr_fun<AnaTrack::ftype,AnaTrack::ftype>(std::abs) , bind(&AnaTrack::d0,_1)) > 1 ||
                         bind( std::ptr_fun<AnaTrack::ftype,AnaTrack::ftype>(std::abs) , bind(&AnaTrack::z0,_1)) > 150 ||
                         bind( std::ptr_fun<AnaTrack::ftype,AnaTrack::ftype>(std::abs) , bind(&AnaTrack::eta,_1)) > AnaConfiguration::maximum_track_abseta_cut() );
    offline_tracks_ftklike.reserve( offline_tracks.size() );
    std::remove_copy_if( offline_tracks.begin() , offline_tracks.end() , back_inserter(offline_tracks_ftklike) ,
                         bind(&AnaTrack::nhits,_1) < 10 || 
                         bind(&AnaTrack::b_layer_nhits,_1) < 1 || 
                         bind(&AnaTrack::nholes,_1) > 1 || 
                         // bind(&AnaTrack::chi2ndof,_1) > 4 ||
                         bind( std::ptr_fun<AnaTrack::ftype,AnaTrack::ftype>(std::abs) , bind(&AnaTrack::eta,_1)) > AnaConfiguration::maximum_track_abseta_cut() );
    
    // limit ftk tracks to 0 or 1 missed hit
    vector< boost::shared_ptr<const AnaTrack> > ftk_tracks;
    ftk_tracks.reserve( ftk_tracks_all.size() );
    //ftk_tracks.assign( ftk_tracks_all.begin() , ftk_tracks_all.end() );
    std::remove_copy_if( ftk_tracks_all.begin() , ftk_tracks_all.end() , back_inserter(ftk_tracks) ,
                         bind(&AnaTrack::ftk_is_guido_category_3,_1) == true ||
                         // (bind(&AnaTrack::nhits,_1)<10) ||
                         // ! (!AnaConfiguration::use_strict_majority() || (bind(&AnaTrack::nhits,_1)>=10) ) ||
                         bind(&AnaTrack::b_layer_nhits,_1) < 1 );
    
    if( false ) { 
      cout << " dumping event: " << endl;
      BOOST_FOREACH( const  boost::shared_ptr<const AnaJet>& jet , cone_jets ) {
        jet->print();
      }
      BOOST_FOREACH( const boost::shared_ptr<const AnaTrack>& track , ftk_tracks ) {
        if( track->pt() < AnaConfiguration::minimum_track_pt_cut() ) { continue; }
        track->print();
      }
      BOOST_FOREACH( const boost::shared_ptr<const AnaTrack>& track , offline_tracks ) {
        if( track->pt() < AnaConfiguration::minimum_track_pt_cut() ) { continue; }
        track->print();
      }
      BOOST_FOREACH( const boost::shared_ptr<const AnaTrack>& track , truth_tracks ) {
        if( track->pt() < AnaConfiguration::minimum_track_pt_cut() ) { continue; }
        track->print();
      }
    }

    if( false ) { // study tracks in event
      const unsigned int nftk_pt1 = std::count_if( ftk_tracks.begin() , ftk_tracks.end() , boost::bind(&AnaTrack::pt,_1)>AnaConfiguration::minimum_track_pt_cut() );
      const unsigned int nidscan_pt1 = std::count_if( idscan_tracks.begin() , idscan_tracks.end() , boost::bind(&AnaTrack::pt,_1)>AnaConfiguration::minimum_track_pt_cut() );
      const unsigned int noffline_pt1 = std::count_if( offline_tracks.begin() , offline_tracks.end() , boost::bind(&AnaTrack::pt,_1)>AnaConfiguration::minimum_track_pt_cut() );
      const unsigned int ntruth_pt1 = std::count_if( truth_tracks.begin() , truth_tracks.end() , boost::bind(&AnaTrack::pt,_1)>AnaConfiguration::minimum_track_pt_cut() );

      dg::fillh( "n_ftk_tracks_all" , 50 , 0 , 50 , ftk_tracks.size() , "N FTK TRACKS IN EVENT" );
      dg::fillh( "n_idscan_tracks_all" , 50 , 0 , 50 , idscan_tracks.size() , "N IDSCAN TRACKS IN EVENT" );
      dg::fillh( "n_offline_tracks_all" , 50 , 0 , 50 , offline_tracks.size() , "N OFFLINE TRACKS IN EVENT" );
      dg::fillh( "n_truth_tracks" , 50 , 0 , 50 , truth_tracks.size() , "N TRUTH TRACKS IN EVENT" );

      dg::fillh( "n_ftk_tracks_pt1" , 50 , 0 , 50 , nftk_pt1 , "N FTK TRACKS IN EVENT (p_{T}> x GeV)" );
      dg::fillh( "n_idscan_tracks_pt1" , 50 , 0 , 50 , nidscan_pt1 , "N IDSCAN TRACKS IN EVENT (p_{T}> x GeV)" );
      dg::fillh( "n_offline_tracks_pt1" , 50 , 0 , 50 , noffline_pt1 , "N OFFLINE TRACKS IN EVENT (p_{T}> x GeV)" );
      dg::fillh( "n_truth_tracks_pt1" , 50 , 0 , 50 , ntruth_pt1 , "N TRUTH TRACKS IN EVENT (p_{T}> x GeV)" );

      // dg::fillh( "n_ftk_tracks_vs_n_truth_tracks_pt1" , 50 , 0 , 50 , 50 , 0 , 50 , 
      //            ntruth_pt1 , nftk_pt1 , "N TRUTH TRACKS" , "N FTK TRACKS" );
      // dg::fillh( "n_idscan_tracks_vs_n_truth_tracks_pt1" , 50 , 0 , 50 , 50 , 0 , 50 , 
      //            ntruth_pt1 , nidscan_pt1 , "N TRUTH TRACKS" , "N IDSCAN TRACKS" );
      // dg::fillh( "n_offline_tracks_vs_n_truth_tracks_pt1" , 50 , 0 , 50 , 50 , 0 , 50 , 
      //            ntruth_pt1 , noffline_pt1 , "N TRUTH TRACKS" , "N OFFLINE TRACKS" );
      // dg::fillh( "n_ftk_tracks_vs_n_offline_tracks_pt1" , 50 , 0 , 50 , 50 , 0 , 50 , 
      //            noffline_pt1 , nftk_pt1 , "N OFFLINE TRACKS" , "N FTK TRACKS" );
      // dg::fillh( "n_idscan_tracks_vs_n_offline_tracks_pt1" , 50 , 0 , 50 , 50 , 0 , 50 , 
      //            noffline_pt1 , nidscan_pt1 , "N OFFLINE TRACKS" , "N IDSCAN TRACKS" );

      StudyTrack study_track( "FTK_TRACKS" , "plots of TrigFTKSim tracks" );
      study_track.for_each( ftk_tracks.begin() , ftk_tracks.end() );
      // study_track.repurpose( "IDSCAN_TRACKS" , "plots of idscan tracks" );
      // study_track.for_each( idscan_tracks.begin() , idscan_tracks.end() );      
      study_track.repurpose( "OFFLINE_TRACKS" , "plots of offline tracks" );
      study_track.for_each( offline_tracks.begin() , offline_tracks.end() );      
      study_track.repurpose( "TRUTH_TRACKS" , "plots of truth tracks" );
      study_track.for_each( truth_tracks.begin() , truth_tracks.end() );      
    }

    // construct list of taggable jets. use the same requirements for tagging and training.
    vector< boost::shared_ptr<const AnaJet> > taggable_cone_jets;
    BOOST_FOREACH( const boost::shared_ptr<const AnaJet>& jet , cone_jets ) {
      if( jet->et()<20. ) { continue; }
      //if( std::abs(jet->eta()) > 0.6 ) { continue; }
      if( std::abs(jet->eta()) > std::max(0.,AnaConfiguration::maximum_track_abseta_cut()-0.4) ) { continue; }
      taggable_cone_jets.push_back( jet );
    }

    dg::filleff( "event_has_taggable_cone_jet" , !(taggable_cone_jets.empty()) );


    BOOST_FOREACH( const boost::shared_ptr<const AnaJet>& roi , roi_jets ) {
      dg::fillh( "roi_et" , 100 , 0 , 100 , roi->et() , "L1 ROI E_{T} (GeV)" );
      dg::fillh( "roi_eta" , 40 , -4 , 4 , roi->eta() , "L1 ROI #eta" );
      dg::fillh( "roi_phi" , 96 , -M_PI , M_PI , roi->phi() , "L1 ROI #phi (rad)" );
      dg::fillh( "roi_dr_b" , 100 , 0 , 1. , roi->dr_b_parton() , "#Delta r (b parton)" );
      dg::fillh( "roi_dr_c" , 100 , 0 , 1. , roi->dr_c_parton() , "#Delta r (c parton)" );
      dg::fillh( "roi_dr_light" , 100 , 0 , 1. , roi->dr_light_parton() , "#Delta r (light parton)" );
    }
    BOOST_FOREACH( const boost::shared_ptr<const AnaJet>& jet , cone_jets ) {
      dg::fillh( "cone_et" , 100 , 0 , 100 , jet->et() , "CONE E_{T} (GeV)" );
      dg::fillh( "cone_eta" , 40 , -4 , 4 , jet->eta() , "CONE #eta" );
      dg::fillh( "cone_phi" , 96 , -M_PI , M_PI , jet->phi() , "CONE #phi (rad)" );
      dg::fillh( "cone_dr_b" , 100 , 0 , 1. , jet->dr_b_parton() , "#Delta r (b parton)" );
      dg::fillh( "cone_dr_c" , 100 , 0 , 1. , jet->dr_c_parton() , "#Delta r (c parton)" );
      dg::fillh( "cone_dr_light" , 100 , 0 , 1. , jet->dr_light_parton() , "#Delta r (light parton)" );
    }
    BOOST_FOREACH( const boost::shared_ptr<const AnaJet>& jet , taggable_cone_jets ) {
      dg::fillh( "taggable_cone_et" , 100 , 0 , 100 , jet->et() , "TAGGABLE CONE E_{T} (GeV)" );
      dg::fillh( "taggable_cone_eta" , 40 , -4 , 4 , jet->eta() , "TAGGABLE CONE #eta" );
      dg::fillh( "taggable_cone_phi" , 96 , -M_PI , M_PI , jet->phi() , "TAGGABLE CONE #phi (rad)" );
      dg::fillh( "taggable_cone_dr_b" , 100 , 0 , 1. , jet->dr_b_parton() , "#Delta r (b parton)" );
      dg::fillh( "taggable_cone_dr_c" , 100 , 0 , 1. , jet->dr_c_parton() , "#Delta r (c parton)" );
      dg::fillh( "taggable_cone_dr_light" , 100 , 0 , 1. , jet->dr_light_parton() , "#Delta r (light parton)" );
    }


    // tracking efficiency in jets
    if( false ) { 
      dg::down( "tracking_eff" , "tracking efficiency plots" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      {
        StudyTrack study_track( "truth" , "truth" );
        study_track.for_each( truth_tracks.begin() , truth_tracks.end() );
      }
      standard_truth_comparison( "offline" , "all_primary" , "OFFLINE" , truth_tracks , offline_tracks );
      standard_truth_comparison( "offline_tight" , "all_primary" , "OFFLINE" , truth_tracks , offline_tracks_tight );
      standard_truth_comparison( "offline_ftklike" , "all_primary" , "OFFLINE" , truth_tracks , offline_tracks_ftklike );
      standard_truth_comparison( "FTK" , "all_primary" , "FTK" , truth_tracks , ftk_tracks );
      standard_jet_tracks( "offline" , "OFFLINE" , offline_tracks );
      standard_jet_tracks( "offline_tight" , "OFFLINE" , offline_tracks_tight );
      standard_jet_tracks( "offline_ftklike" , "OFFLINE" , offline_tracks_ftklike );
      standard_jet_tracks( "FTK" , "FTK" , ftk_tracks );
    }

    // tagging
    
    const bool train_not_tag = AnaConfiguration::train_tagger();
    
    for( vector< boost::shared_ptr<const AnaJet> >::iterator ijet=taggable_cone_jets.begin(), fjet=taggable_cone_jets.end(); ijet!=fjet; ++ijet ) {
      const boost::shared_ptr<const AnaJet>& jet( *ijet );
      //unsigned int i = std::distance( taggable_cone_jets.begin() , ijet );
      TrackAssociator< const vector< boost::shared_ptr<const AnaTrack> > > assoc_ftk( ftk_tracks );
      TrackAssociator< const vector< boost::shared_ptr<const AnaTrack> > > assoc_idscan( idscan_tracks );
      TrackAssociator< const vector< boost::shared_ptr<const AnaTrack> > > assoc_offline( offline_tracks );
      TrackAssociator< const vector< boost::shared_ptr<const AnaTrack> > > assoc_offline_tight( offline_tracks_tight );
      TrackAssociator< const vector< boost::shared_ptr<const AnaTrack> > > assoc_offline_ftklike( offline_tracks_ftklike );
      TrackAssociator< const vector< boost::shared_ptr<const AnaTrack> > > assoc_truth( truth_tracks );
      vector< boost::shared_ptr<const AnaTrack> > associated_ftk_tracks( assoc_ftk(jet) );
      vector< boost::shared_ptr<const AnaTrack> > associated_offline_tracks( assoc_offline(jet) );
      vector< boost::shared_ptr<const AnaTrack> > associated_offline_tracks_tight( assoc_offline_tight(jet) );
      vector< boost::shared_ptr<const AnaTrack> > associated_offline_tracks_ftklike( assoc_offline_ftklike(jet) );
      vector< boost::shared_ptr<const AnaTrack> > associated_idscan_tracks( assoc_idscan(jet) );
      vector< boost::shared_ptr<const AnaTrack> > associated_truth_tracks( assoc_truth(jet) );
      vector< boost::shared_ptr<const AnaTrack> > associated_b_truth_tracks;
      std::remove_copy_if( associated_truth_tracks.begin() , associated_truth_tracks.end() , back_inserter(associated_b_truth_tracks) ,
                           bind( &AnaTrack::is_truth_b_decay , _1 ) == false );
      
      dg::fillh( "nassoc_ftk" , 50 , 0 , 50 , associated_ftk_tracks.size() );
      dg::fillh( "nassoc_offline" , 50 , 0 , 50 , associated_offline_tracks.size() );
      dg::fillh( "nassoc_offline" , 50 , 0 , 50 , associated_offline_tracks_tight.size() );
      dg::fillh( "nassoc_offline" , 50 , 0 , 50 , associated_offline_tracks_ftklike.size() );
      dg::fillh( "nassoc_idscan" , 50 , 0 , 50 , associated_idscan_tracks.size() );
      dg::fillh( "nassoc_truth" , 50 , 0 , 50 , associated_truth_tracks.size() );
      dg::fillh( "nassoc_truth_b" , 50 , 0 , 50 , associated_b_truth_tracks.size() );
      dg::fillh( "nassoc_truth_primary" , 50 , 0 , 50 , std::count_if( associated_truth_tracks.begin() , associated_truth_tracks.end() , 
                                                                       boost::bind( &UniqueBarcode::is_primary , boost::bind( &AnaTrack::barcode , _1 ) ) ) );

      TrackPruner pruner;
      const double prune_dz0_cut = AnaConfiguration::jet_dz0_prune(); // e.g. 5 or 9999 mm
      pruner.prune( associated_ftk_tracks , prune_dz0_cut ); // remove all tracks with |dz0|>prune_dz0_cutmm around highest pt track
      pruner.prune( associated_offline_tracks , prune_dz0_cut ); // remove all tracks with |dz0|>prune_dz0_cutmm around highest pt track
      pruner.prune( associated_offline_tracks_tight , prune_dz0_cut ); // remove all tracks with |dz0|>prune_dz0_cutmm around highest pt track
      pruner.prune( associated_offline_tracks_ftklike , prune_dz0_cut ); // remove all tracks with |dz0|>prune_dz0_cutmm around highest pt track
      pruner.prune( associated_idscan_tracks , prune_dz0_cut ); // remove all tracks with |dz0|>prune_dz0_cutmm around highest pt track
      pruner.prune( associated_truth_tracks , prune_dz0_cut ); // remove all tracks with |dz0|>prune_dz0_cutmm around highest pt track

      dg::fillh( "nassoc_ftk_pruned" , 50 , 0 , 50 , associated_ftk_tracks.size() );
      dg::fillh( "nassoc_offline_pruned" , 50 , 0 , 50 , associated_offline_tracks.size() );
      dg::fillh( "nassoc_offline_pruned" , 50 , 0 , 50 , associated_offline_tracks_tight.size() );
      dg::fillh( "nassoc_offline_pruned" , 50 , 0 , 50 , associated_offline_tracks_ftklike.size() );
      dg::fillh( "nassoc_idscan_pruned" , 50 , 0 , 50 , associated_idscan_tracks.size() );
      dg::fillh( "nassoc_truth_pruned" , 50 , 0 , 50 , associated_truth_tracks.size() );
      dg::fillh( "nassoc_truth_primary_pruned" , 50 , 0 , 50 , std::count_if( associated_truth_tracks.begin() , associated_truth_tracks.end() , 
                                                                              boost::bind( &UniqueBarcode::is_primary , boost::bind( &AnaTrack::barcode , _1 ) ) ) );
      // apply some quality cuts to the associated tracks
      // class bad_tracks
      // {
      // public:
      //   static void remove ( vector< boost::shared_ptr<const AnaTrack> >& tracks ) {
      //     vector< boost::shared_ptr<const AnaTrack> >::iterator new_end = std::remove_if( tracks.begin() , tracks.end() , 
      //                                                                 boost::bind( &AnaTrack::pt , _1 ) < AnaConfiguration::minimum_track_pt_cut() );
      //     tracks.erase( tracks.begin() , new_end );
      //   }
      // };
      // bad_tracks::remove( associated_ftk_tracks );
      // bad_tracks::remove( associated_offline_tracks );
      // bad_tracks::remove( associated_idscan_tracks );
      // bad_tracks::remove( associated_truth_tracks );
      // //
      if( false ) {
        // dump jet and associated tracks.
        cout << " Run Event " << evt->run_number() << " " << evt->event_number() << endl;
        cout << " got an offline jet: eta=" << jet->eta() << " phi=" << jet->phi()
             << " et=" << jet->et() << " type=" << jet->dr_b_parton() << " " << jet->dr_light_parton() << endl;
        BOOST_FOREACH( const boost::shared_ptr<const AnaTrack>&  track , associated_offline_tracks ) { 
          cout << " associated track with jet: pt=" << track->pt() << " eta=" << track->eta() << " d0=" << track->d0()
               << " d0/sig=" << track->d0_sig_monica()
               << " sign=" << (track->signed_d0_sig_monica( jet->phi() )>0. ? 1. : -1. )
               << " z0=" << track->z0()
               << endl;
        }
        cout << " finished associating " << associated_offline_tracks.size() << " tracks  with  jets " << endl;
      }
      dg::fillh( "n_ftk_assoc" , 50 , 0 , 50 , associated_ftk_tracks.size() , "N FTK ASSOCIATED" );
      dg::fillh( "n_idscan_assoc" , 50 , 0 , 50 , associated_idscan_tracks.size() , "N IDSCAN ASSOCIATED" );
      dg::fillh( "n_offline_assoc" , 50 , 0 , 50 , associated_offline_tracks.size() , "N OFFLINE ASSOCIATED" );
      dg::fillh( "n_offline_tight_assoc" , 50 , 0 , 50 , associated_offline_tracks_tight.size() , "N OFFLINE ASSOCIATED" );
      dg::fillh( "n_offline_ftklike_assoc" , 50 , 0 , 50 , associated_offline_tracks_ftklike.size() , "N OFFLINE ASSOCIATED" );
      dg::fillh( "n_truth_assoc" , 50 , 0 , 50 , associated_truth_tracks.size() , "N TRUTH ASSOCIATED" );
      // number of tracks with prompt impact parameter in u jets
      const bool ok_dr_b = jet->dr_b_parton() < AnaConfiguration::jet_label_dr_cut();
      const bool ok_dr_notb = jet->dr_light_parton() < AnaConfiguration::jet_label_dr_cut();
      // tracking efficiency
      if( true ) { 
        dg::down( "tracking_eff" , "tracking efficiency plots" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
        standard_truth_comparison( "offline_jets" , "jets" , "OFFLINE" , associated_truth_tracks , associated_offline_tracks , jet );
        standard_truth_comparison( "offline_tight_jets" , "jets" , "TIGHT OFFLINE" , associated_truth_tracks , associated_offline_tracks_tight , jet );
        standard_truth_comparison( "offline_ftklike_jets" , "jets" , "FTKLIKE OFFLINE" , associated_truth_tracks , associated_offline_tracks_ftklike , jet );
        standard_truth_comparison( "FTK_jets" , "jets" , "FTK" , associated_truth_tracks , associated_ftk_tracks , jet );
        standard_jet_tracks( "offline_jets" , "OFFLINE" , associated_offline_tracks );
        standard_jet_tracks( "offline_tight_jets" , "OFFLINE" , associated_offline_tracks_tight );
        standard_jet_tracks( "offline_ftklike_jets" , "OFFLINE" , associated_offline_tracks_ftklike );
        standard_jet_tracks( "FTK_jets" , "FTK" , associated_ftk_tracks );
        if( ok_dr_b && !ok_dr_notb ) { 
          standard_truth_comparison( "offline_b_jets" , "b_jets" , "OFFLINE" , associated_truth_tracks , associated_offline_tracks , jet );
          standard_truth_comparison( "offline_tight_b_jets" , "b_jets" , "TIGHT OFFLINE" , associated_truth_tracks , associated_offline_tracks_tight , jet );
          standard_truth_comparison( "offline_ftklike_b_jets" , "b_jets" , "FTKLIKE OFFLINE" , associated_truth_tracks , associated_offline_tracks_ftklike , jet );
          standard_truth_comparison( "FTK_b_jets" , "u_jets" , "FTK" , associated_truth_tracks , associated_ftk_tracks , jet );
          standard_jet_tracks( "offline_b_jets" , "OFFLINE" , associated_offline_tracks );
          standard_jet_tracks( "offline_tight_b_jets" , "OFFLINE" , associated_offline_tracks_tight );
          standard_jet_tracks( "offline_ftklike_b_jets" , "OFFLINE" , associated_offline_tracks_ftklike );
          standard_jet_tracks( "FTK_b_jets" , "FTK" , associated_ftk_tracks );
        }
        if( !ok_dr_b && ok_dr_notb ) { 
          standard_truth_comparison( "offline_u_jets" , "b_jets" , "OFFLINE" , associated_truth_tracks , associated_offline_tracks , jet );
          standard_truth_comparison( "offline_tight_u_jets" , "b_jets" , "TIGHT OFFLINE" , associated_truth_tracks , associated_offline_tracks_tight , jet );
          standard_truth_comparison( "offline_ftklike_u_jets" , "b_jets" , "FTKLIKE OFFLINE" , associated_truth_tracks , associated_offline_tracks_ftklike , jet );
          standard_truth_comparison( "FTK_u_jets" , "u_jets" , "FTK" , associated_truth_tracks , associated_ftk_tracks , jet );
          standard_jet_tracks( "offline_u_jets" , "OFFLINE" , associated_offline_tracks );
          standard_jet_tracks( "offline_tight_u_jets" , "OFFLINE" , associated_offline_tracks_tight );
          standard_jet_tracks( "offline_ftklike_u_jets" , "OFFLINE" , associated_offline_tracks_ftklike );
          standard_jet_tracks( "FTK_u_jets" , "FTK" , associated_ftk_tracks );
        }
      }
      // make some track subsets to look at separately
      vector< boost::shared_ptr<const AnaTrack> > associated_ftk_tracks_chi2lt2;
      std::remove_copy_if( associated_ftk_tracks.begin() , associated_ftk_tracks.end() , back_inserter(associated_ftk_tracks_chi2lt2) ,
                           boost::bind( &AnaTrack::chi2ndof , _1 ) > 2. );
      vector< boost::shared_ptr<const AnaTrack> > associated_ftk_tracks_fullfits;
      std::remove_copy_if( associated_ftk_tracks.begin() , associated_ftk_tracks.end() , back_inserter(associated_ftk_tracks_fullfits) ,
                           boost::bind( &AnaTrack::nmisses , _1 ) > 0 );
      // study secondary vertexing algorithm.
      if( true ) {
        study_vertexing( "VERTEX_FIT" , associated_offline_tracks , jet );
        // get minimum dr b hadron, if it exists.
        AnaJet::Hadron mini;
        AnaTrack::ftype mini_rad = 0.;
        AnaTrack::ftype mini_phi = 0.;
        const AnaJet::ftype mindr = jet->min_dr_hadron( AnaJet::Hadron::B_QUARK , mini );
        if( mindr < 0.3 ) {
          mini_phi = mini.phi();
          mini_rad = detector::quadrature( mini.x() , mini.y() );
        }
        {
          dg::down( "d0_vs_phi" , "tagging d0 vs phi studies" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
          // find offline tracks from b decay
          StudyTrackDisplacement< vector< boost::shared_ptr<const AnaTrack> > >  study_disp( "D0_VS_PHI_b_truth" , "d0 vs phi studies" , jet->phi() , mini_rad , mini_phi );
          study_disp.add_constant_category( "bjet" , jet->dr_b_parton() < 0.3 );
          study_disp.add_constant_category( "ujet" , jet->dr_b_parton() > 0.3 );
          study_disp.for_only( associated_b_truth_tracks );
          study_disp.repurpose( "D0_VS_PHI_truth" , "d0 vs phi studies" );
          study_disp.for_only( associated_truth_tracks );
          study_disp.repurpose( "D0_VS_PHI_offline" , "d0 vs phi studies" );
          study_disp.for_only( associated_offline_tracks );
          study_disp.repurpose( "D0_VS_PHI_ftk" , "d0 vs phi studies" );
          study_disp.for_only( associated_ftk_tracks );
        }
      }
      //
      if( true ) { 
        if( train_not_tag ) {
          tagger_mgr.train( "FTK_D0_TAGGER" , jet , associated_ftk_tracks );
          tagger_mgr.train( "FTK_SIGNEDD0_TAGGER" , jet , associated_ftk_tracks );
          tagger_mgr.train( "FTK_SIGNEDD0_TRACKCONE_TAGGER" , jet , associated_ftk_tracks );
          tagger_mgr.train( "IDSCAN_SIGNEDD0_TAGGER" , jet , associated_idscan_tracks );
          tagger_mgr.train( "OFFLINE_D0_TAGGER" , jet , associated_offline_tracks_tight );
          tagger_mgr.train( "OFFLINE_SIGNEDD0_TAGGER" , jet , associated_offline_tracks_tight );
          tagger_mgr.train( "OFFLINE_SIGNEDD0_TRACKCONE_TAGGER_ALL" , jet , associated_offline_tracks );
          tagger_mgr.train( "OFFLINE_SIGNEDD0_TRACKCONE_TAGGER" , jet , associated_offline_tracks_tight );
          tagger_mgr.train( "OFFLINE_SIGNEDD0_TRACKCONE_TAGGER_FTKLIKE" , jet , associated_offline_tracks_ftklike );
          tagger_mgr.train( "TRUTH_SIGNEDD0_TAGGER" , jet , associated_truth_tracks );
          tagger_mgr.train( "FTK_D0_SIGNIFICANCE_TAGGER" , jet , associated_ftk_tracks );
          tagger_mgr.train( "FTK_SIGNEDD0_SIGNIFICANCE_TAGGER" , jet , associated_ftk_tracks );
          tagger_mgr.train( "FTK_SIGNEDD0_SIGNIFICANCE_TRACKCONE_TAGGER" , jet , associated_ftk_tracks );
          tagger_mgr.train( "FTK_SIGNEDD0_SIGNIFICANCE_PURITY_TRACKCONE_TAGGER" , jet , associated_ftk_tracks );
          tagger_mgr.train( "FTK_SIGNEDD0_SIGNIFICANCE_TRACKCONE_TAGGER.CHI2LT2" , jet , associated_ftk_tracks_chi2lt2 );
          tagger_mgr.train( "FTK_SIGNEDD0_SIGNIFICANCE_TRACKCONE_TAGGER.FULLFIT" , jet , associated_ftk_tracks_fullfits );
          tagger_mgr.train( "IDSCAN_SIGNEDD0_SIGNIFICANCE_TAGGER" , jet , associated_idscan_tracks );
          tagger_mgr.train( "OFFLINE_D0_SIGNIFICANCE_TAGGER" , jet , associated_offline_tracks_tight );
          tagger_mgr.train( "OFFLINE_SIGNEDD0_SIGNIFICANCE_TAGGER" , jet , associated_offline_tracks_tight );
          tagger_mgr.train( "OFFLINE_SIGNEDD0_SIGNIFICANCE_TRACKCONE_SV1_TAGGER_ALL" , jet , associated_offline_tracks );
          tagger_mgr.train( "OFFLINE_SIGNEDD0_Z0_SIGNIFICANCE_TRACKCONE_SV1_TAGGER_ALL" , jet , associated_offline_tracks );
          tagger_mgr.train( "OFFLINE_SIGNED_Z0_SIGNIFICANCE_TRACKCONE_SV1_TAGGER_ALL" , jet , associated_offline_tracks );
          tagger_mgr.train( "OFFLINE_SIGNEDD0_SIGNIFICANCE_TRACKCONE_TAGGER" , jet , associated_offline_tracks_tight );
          tagger_mgr.train( "OFFLINE_SIGNEDD0_SIGNIFICANCE_PURITY_TRACKCONE_TAGGER" , jet , associated_offline_tracks );
          tagger_mgr.train( "OFFLINE_SIGNEDD0_SIGNIFICANCE_TRACKCONE_TAGGER_ALL" , jet , associated_offline_tracks );
          tagger_mgr.train( "OFFLINE_SIGNEDD0_Z0_SIGNIFICANCE_TRACKCONE_TAGGER_ALL" , jet , associated_offline_tracks );
          tagger_mgr.train( "OFFLINE_SIGNED_Z0_SIGNIFICANCE_TRACKCONE_TAGGER_ALL" , jet , associated_offline_tracks );
          tagger_mgr.train( "TRUTH_SIGNEDD0_SIGNIFICANCE_TAGGER" , jet , associated_truth_tracks );
        } else {
          study_tagger( "FTK_D0_TAGGER" , jet , associated_ftk_tracks , associated_truth_tracks );
          study_tagger( "FTK_SIGNEDD0_TAGGER" , jet , associated_ftk_tracks , associated_truth_tracks );
          study_tagger( "FTK_SIGNEDD0_TRACKCONE_TAGGER" , jet , associated_ftk_tracks , associated_truth_tracks );
          study_tagger( "IDSCAN_SIGNEDD0_TAGGER" , jet , associated_idscan_tracks , associated_truth_tracks );
          study_tagger( "OFFLINE_D0_TAGGER" , jet , associated_offline_tracks_tight , associated_truth_tracks );
          study_tagger( "OFFLINE_SIGNEDD0_TAGGER" , jet , associated_offline_tracks_tight , associated_truth_tracks );
          study_tagger( "OFFLINE_SIGNEDD0_TRACKCONE_TAGGER_ALL" , jet , associated_offline_tracks , associated_truth_tracks );
          study_tagger( "OFFLINE_SIGNEDD0_TRACKCONE_TAGGER" , jet , associated_offline_tracks_tight , associated_truth_tracks );
          study_tagger( "OFFLINE_SIGNEDD0_TRACKCONE_TAGGER_FTKLIKE" , jet , associated_offline_tracks_ftklike , associated_truth_tracks );
          study_tagger( "TRUTH_SIGNEDD0_TAGGER" , jet , associated_truth_tracks , associated_truth_tracks );
          study_tagger( "FTK_D0_SIGNIFICANCE_TAGGER" , jet , associated_ftk_tracks , associated_truth_tracks );
          study_tagger( "FTK_SIGNEDD0_SIGNIFICANCE_TAGGER" , jet , associated_ftk_tracks , associated_truth_tracks );
          study_tagger( "FTK_SIGNEDD0_SIGNIFICANCE_TRACKCONE_TAGGER" , jet , associated_ftk_tracks , associated_truth_tracks );
          study_tagger( "FTK_SIGNEDD0_SIGNIFICANCE_PURITY_TRACKCONE_TAGGER" , jet , associated_ftk_tracks , associated_truth_tracks );
          study_tagger( "FTK_SIGNEDD0_SIGNIFICANCE_TRACKCONE_TAGGER.CHI2LT2" , jet , associated_ftk_tracks_chi2lt2 , associated_truth_tracks );
          study_tagger( "FTK_SIGNEDD0_SIGNIFICANCE_TRACKCONE_TAGGER.FULLFIT" , jet , associated_ftk_tracks_fullfits , associated_truth_tracks );
          study_tagger( "IDSCAN_SIGNEDD0_SIGNIFICANCE_TAGGER" , jet , associated_idscan_tracks , associated_truth_tracks );
          study_tagger( "OFFLINE_D0_SIGNIFICANCE_TAGGER" , jet , associated_offline_tracks_tight , associated_truth_tracks );
          study_tagger( "OFFLINE_SIGNEDD0_SIGNIFICANCE_TAGGER" , jet , associated_offline_tracks_tight , associated_truth_tracks );
          study_tagger( "OFFLINE_SIGNEDD0_SIGNIFICANCE_TRACKCONE_SV1_TAGGER_ALL" , jet , associated_offline_tracks , associated_truth_tracks );
          study_tagger( "OFFLINE_SIGNEDD0_Z0_SIGNIFICANCE_TRACKCONE_SV1_TAGGER_ALL" , jet , associated_offline_tracks , associated_truth_tracks );
          study_tagger( "OFFLINE_SIGNED_Z0_SIGNIFICANCE_TRACKCONE_SV1_TAGGER_ALL" , jet , associated_offline_tracks , associated_truth_tracks );
          study_tagger( "OFFLINE_SIGNEDD0_SIGNIFICANCE_TRACKCONE_TAGGER" , jet , associated_offline_tracks_tight , associated_truth_tracks );
          study_tagger( "OFFLINE_SIGNEDD0_SIGNIFICANCE_PURITY_TRACKCONE_TAGGER" , jet , associated_offline_tracks , associated_truth_tracks );
          study_tagger( "OFFLINE_SIGNEDD0_SIGNIFICANCE_TRACKCONE_TAGGER_ALL" , jet , associated_offline_tracks , associated_truth_tracks );
          study_tagger( "OFFLINE_SIGNEDD0_Z0_SIGNIFICANCE_TRACKCONE_TAGGER_ALL" , jet , associated_offline_tracks , associated_truth_tracks );
          study_tagger( "OFFLINE_SIGNED_Z0_SIGNIFICANCE_TRACKCONE_TAGGER_ALL" , jet , associated_offline_tracks , associated_truth_tracks );
          study_tagger( "TRUTH_SIGNEDD0_SIGNIFICANCE_TAGGER" , jet , associated_truth_tracks , associated_truth_tracks );

          // study_tagger( "FTK_D0_TAGGER_PARAMETERIZED" , jet , associated_ftk_tracks , associated_truth_tracks );
          // study_tagger( "FTK_SIGNEDD0_TAGGER_PARAMETERIZED" , jet , associated_ftk_tracks , associated_truth_tracks );
          // study_tagger( "FTK_SIGNEDD0_TRACKCONE_TAGGER_PARAMETERIZED" , jet , associated_ftk_tracks , associated_truth_tracks );
          // study_tagger( "OFFLINE_D0_TAGGER_PARAMETERIZED" , jet , associated_offline_tracks_tight , associated_truth_tracks );
          // study_tagger( "OFFLINE_SIGNEDD0_TAGGER_PARAMETERIZED" , jet , associated_offline_tracks_tight , associated_truth_tracks );
          // study_tagger( "OFFLINE_SIGNEDD0_TRACKCONE_TAGGER_PARAMETERIZED" , jet , associated_offline_tracks_tight , associated_truth_tracks );
          // //study_tagger( "OFFLINE_SIGNEDD0_TRACKCONE_TAGGER_PARAMETERIZED_FTKLIKE" , jet , associated_offline_tracks_ftklike , associated_truth_tracks );
          // study_tagger( "FTK_D0_SIGNIFICANCE_TAGGER_PARAMETERIZED" , jet , associated_ftk_tracks , associated_truth_tracks );
          // study_tagger( "FTK_SIGNEDD0_SIGNIFICANCE_TAGGER_PARAMETERIZED" , jet , associated_ftk_tracks , associated_truth_tracks );
          // study_tagger( "FTK_SIGNEDD0_SIGNIFICANCE_TRACKCONE_TAGGER_PARAMETERIZED" , jet , associated_ftk_tracks , associated_truth_tracks );
          // study_tagger( "OFFLINE_D0_SIGNIFICANCE_TAGGER_PARAMETERIZED" , jet , associated_offline_tracks_tight , associated_truth_tracks );
          // study_tagger( "OFFLINE_SIGNEDD0_SIGNIFICANCE_TAGGER_PARAMETERIZED" , jet , associated_offline_tracks_tight , associated_truth_tracks );
          // study_tagger( "OFFLINE_SIGNEDD0_SIGNIFICANCE_TRACKCONE_TAGGER_PARAMETERIZED" , jet , associated_offline_tracks_tight , associated_truth_tracks );
        }
      }
    }

  } // end for each event

  if( AnaConfiguration::train_tagger() ) {
    cout << " saving tagger training to " << AnaConfiguration::trained_tagger_file() << "..." << endl;
    tagger_mgr.save( AnaConfiguration::trained_tagger_file() );
  }

  if( false ) {
    TFile* f = new TFile( "tagger.root" , "RECREATE" );
    TH1F* hb = tagger_mgr.likelihood( "FTK_SIGNEDD0_TAGGER" , true );
    if( hb ) { 
      hb->SetName("hb"); 
      hb->SetDirectory( f );
    }
    TH1F* hnotb = tagger_mgr.likelihood( "FTK_SIGNEDD0_TAGGER" , false );
    if( hnotb ) { 
      hnotb->SetName("hnotb");
      hnotb->SetDirectory( f );
    }
    f->Write();
    f->Close();
  }


}
void
study_tagger( const std::string& name , const boost::shared_ptr<const AnaJet>& jet , const std::vector< boost::shared_ptr<const AnaTrack> >& associated_tracks ,
              const std::vector< boost::shared_ptr<const AnaTrack> >& associated_truth_tracks ,
              const std::string dgcoll )
{
  if( !tagger_mgr.get_tagger( name ) ) { return; }

  // require a jet clearly identified as either a b or a light jet, but not both.
  // const bool is_b = (jet->dr_b_parton()<0.1) && (jet->dr_light_parton()>0.1);
  // const bool is_light = (jet->dr_light_parton()<0.1) && (jet->dr_b_parton()>0.1);
  // if( !(is_b ^ is_light) ) { continue; }

  // 20100123
  // const bool is_b = (jet->dr_b_parton()<AnaConfiguration::jet_label_dr_cut()) && (jet->dr_light_parton()>AnaConfiguration::jet_label_dr_cut());
  // const bool is_light = (jet->dr_light_parton()<AnaConfiguration::jet_label_dr_cut()) && (jet->dr_b_parton()>AnaConfiguration::jet_label_dr_cut());
  const bool is_b = (jet->dr_b_parton()<AnaConfiguration::jet_label_dr_cut());
  const bool is_light = !is_b;

  dg::down( name , "taggable jets" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
  dg::fillh( "n_track_assoc" , 50 , 0 , 50 , associated_tracks.size() , "N ASSOCIATED TRACKS" );
  dg::fillh( "taggable_et" , 100 , 0 , 100 , jet->et() );
  dg::fillh( "taggable_eta" , 160 , -4 , 4 , jet->eta() );
  dg::fillh( "taggable_phi" , 96 , -M_PI , M_PI , jet->phi() );
  dg::fillh( "taggable_dr_b" , 100 , 0 , 1. , jet->dr_b_parton() , "#Delta r (b parton)" );
  dg::fillh( "taggable_dr_c" , 100 , 0 , 1. , jet->dr_c_parton() , "#Delta r (c parton)" );
  dg::fillh( "taggable_dr_light" , 100 , 0 , 1. , jet->dr_light_parton() , "#Delta r (light parton)" );
  dg::filleff( "taggable_is_b" , is_b );
  dg::filleff( "taggable_is_light" , is_light );
  if( jet->dr_b_parton()<AnaConfiguration::jet_label_dr_cut() ) { 
    dg::fillh( "taggable_is_b_dr_b" , 100 , 0 , 1. , jet->dr_b_parton() , "#Delta r (b parton)" ); 
    dg::fillh( "taggable_is_b_dr_light" , 100 , 0 , 1. , jet->dr_light_parton() , "#Delta r (light parton)" ); 
    dg::fillh( "taggable_is_b_dr_b_any" , 100 , 0 , 1. , jet->dr_b_parton_any() , "#Delta r (b parton)" ); 
    dg::fillh( "taggable_is_b_dr_light_any" , 100 , 0 , 1. , jet->dr_light_parton_any() , "#Delta r (light parton)" ); 
  }
  if( jet->dr_light_parton()<AnaConfiguration::jet_label_dr_cut() ) { 
    dg::fillh( "taggable_is_light_dr_b" , 100 , 0 , 1. , jet->dr_b_parton() , "#Delta r (b parton)" ); 
    dg::fillh( "taggable_is_light_dr_b" , 100 , 0 , 1. , jet->dr_b_parton() , "#Delta r (b parton)" ); 
    dg::fillh( "taggable_is_light_dr_light" , 100 , 0 , 1. , jet->dr_light_parton() , "#Delta r (light parton)" ); 
    dg::fillh( "taggable_is_light_dr_b_any" , 100 , 0 , 1. , jet->dr_b_parton_any() , "#Delta r (b parton)" ); 
    dg::fillh( "taggable_is_light_dr_light_any" , 100 , 0 , 1. , jet->dr_light_parton_any() , "#Delta r (light parton)" ); 
  }

  study_jet_tracks( "all_jets" , jet , associated_tracks , associated_truth_tracks );

  if( is_b || is_light ) { 
    study_jet_tracks( is_b ? "bjets" : "lightjets" , jet , associated_tracks ,
                      associated_truth_tracks );
  }

  if( !(is_b ^ is_light) ) { return; }

  const double tag = tagger_mgr.tag( name , jet , associated_tracks , dgcoll );
  for( unsigned int icut=0; icut!=20; ++icut ) {
    const double cutval = (icut+0.5)*1/20.;
    dg::filleff( "tagged_eff_vs_cut" , 20 , 0 , 1 , cutval , tag>cutval );
  }

  // count number of hits shared with another track
  if( false ) { 
    BOOST_FOREACH( const boost::shared_ptr<const AnaTrack>& track , associated_tracks ) {
      const unsigned int bank_id = track->ftk_bank_id();
      const unsigned long road_id = track->ftk_road_id();
      const boost::shared_ptr<const AnaRoad>& road( global_evt->ftk_road_lookup( AnaEventMgr::DEFAULT_SOURCE , bank_id , road_id ) );
      if( !road ) { continue; }
      // get the ssid and coord for each hit in the road
      typedef map<AnaSuperstrip::SuperstripID,AnaSuperstrip::ftype> SSIDMap;
      typedef map<unsigned int,SSIDMap> HitMap;
      HitMap hitmap;
      int ilayer = 0;
      int iid = -1;
      BOOST_FOREACH( const unsigned long ssid , road->superstrip_ids() ) {
        ++iid;
        while( !detector::ftkHasPlane(track->ftk_bitmask(),ilayer) ) {
          ++ilayer; 
          if( ilayer>=detector::ftk_nplanes() ) { break; }
          continue;
        }
        assert( ilayer<detector::ftk_nplanes() );
        // const boost::shared_ptr<const AnaSuperstrip>& ss = global_evt->ftk_ss_lookup(AnaEventMgr::DEFAULT_SOURCE,ilayer,ssid);
        assert( track->ftk_coords().size() <= iid );
        hitmap[ ilayer ][ ssid ] = track->ftk_coords()[iid];
      }
      // 
      unsigned int nshared = 0u;
      BOOST_FOREACH( const boost::shared_ptr<const AnaTrack>& other_track , associated_tracks ) {
        if( other_track == track ) { continue; }
        const unsigned int other_bank_id = other_track->ftk_bank_id();
        const unsigned long other_road_id = other_track->ftk_road_id();
        const boost::shared_ptr<const AnaRoad>& other_road( global_evt->ftk_road_lookup( AnaEventMgr::DEFAULT_SOURCE , other_bank_id , other_road_id ) );
        if( !other_road ) { continue; }
        int ilayer = 0;
        int iid = -1;
        BOOST_FOREACH( const unsigned long ssid , other_road->superstrip_ids() ) {
          ++iid;
          while( !detector::ftkHasPlane(other_track->ftk_bitmask(),ilayer) ) {
            ++ilayer; 
            if( ilayer>=detector::ftk_nplanes() ) { break; }
            continue;
          }
          assert( ilayer<detector::ftk_nplanes() );
          assert( other_track->ftk_coords().size() <= iid );
          HitMap::const_iterator imap = hitmap.find( ilayer );
          if( imap == hitmap.end() ) { continue; }
          SSIDMap::const_iterator jmap = imap->second.find( ssid );
          if( jmap == imap->second.end() ) { continue; }
          if( jmap->second == iid ) {
            ++nshared;
          }
        }
      }
      // histogram nshared
      dg::fillh( "trk_nshared" , 14 , 0 , 14 , nshared , "Number of Shared Hits" );
    }
  }

}


void study_jet_tracks( const std::string& name , const boost::shared_ptr<const AnaJet>& jet , 
                       const std::vector< boost::shared_ptr<const AnaTrack> >& associated_tracks ,
                       const std::vector< boost::shared_ptr<const AnaTrack> >& associated_truth_tracks )
{
  using namespace boost;

  dg::down( name , "jets tracks" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;

  TrackAssociator< const vector< boost::shared_ptr<const AnaTrack> > > assoc( associated_tracks );
  // match all tracks to truth, if possible. determine "b- or non-b-ness"
  typedef map< boost::shared_ptr<const AnaTrack> , boost::shared_ptr<const AnaTrack> > TrackToTruthMap;
  TrackToTruthMap track_to_truth;
  BOOST_FOREACH( const boost::shared_ptr<const AnaTrack>& track , associated_tracks ) {
    assert( track );
    if( track->barcode_matching_fraction() < AnaConfiguration::barcode_matching_minimum_cut() ) { continue; }
    vector< boost::shared_ptr<const AnaTrack> >::const_iterator itruth = std::find_if( associated_truth_tracks.begin() , associated_truth_tracks.end() , 
                                                                   bind(&AnaTrack::barcode,_1) == track->barcode() );
    if( itruth == associated_truth_tracks.end() ) { continue; }
    track_to_truth[ track ] = (*itruth);
  }

  // study b / u jet classification
  for( double drcut = 0.1; drcut<=0.4; drcut+=0.1 ) {
    const char* dname = "default";
    if( drcut<0.12 ) { dname = "jets_0.1"; }
    else if( drcut<0.22 ) { dname = "jets_0.2"; }
    else if( drcut<0.32 ) { dname = "jets_0.3"; }
    else if( drcut<0.42 ) { dname = "jets_0.4"; }
    dg::down( dname , "jets tracks" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
    const bool ok_dr_b = jet->dr_b_parton() < drcut;
    const bool ok_dr_notb = jet->dr_light_parton() < drcut;
    dg::fillh( "taggable_et" , 100 , 0 , 100 , jet->et() );
    dg::fillh( "taggable_eta" , 40 , -4 , 4 , jet->eta() );
    dg::fillh( "taggable_phi" , 96 , -M_PI , M_PI , jet->phi() );
    dg::fillh( "taggable_dr_b" , 100 , 0 , 1. , jet->dr_b_parton() , "#Delta r (b parton)" );
    dg::fillh( "taggable_dr_c" , 100 , 0 , 1. , jet->dr_c_parton() , "#Delta r (c parton)" );
    dg::fillh( "taggable_dr_light" , 100 , 0 , 1. , jet->dr_light_parton() , "#Delta r (light parton)" );
    dg::fillh( "taggable_dr_b_any" , 100 , 0 , 1. , jet->dr_b_parton_any() , "#Delta r (b parton)" );
    dg::fillh( "taggable_dr_c_any" , 100 , 0 , 1. , jet->dr_c_parton_any() , "#Delta r (c parton)" );
    dg::fillh( "taggable_dr_light_any" , 100 , 0 , 1. , jet->dr_light_parton_any() , "#Delta r (light parton)" );
    vector< boost::shared_ptr<const AnaTrack> > tracks( assoc( jet , drcut ) );
    dg::fillh( "ntracks" , 20 , 0 , 20 , tracks.size() );
    double jet_eta , jet_phi;
    const bool jet_dir_ok = JetTagger::compute_jet_eta_phi( jet , tracks , JetTagger::TRACK_CENTROID , jet_eta , jet_phi );
    // compare direction obtained from jet and track clustering vs truth. 
    // retrieve truth direction.
    dg::fillh( "deta_jet" , 100 , -0.5 , 0.5 , jet->eta() - jet->eta_highest_pt_parton() , "#Delta #eta" );
    dg::fillh( "dphi_jet" , 100 , -0.5 , 0.5 , detector::delta_phi( jet->phi() , jet->phi_highest_pt_parton() ), "#Delta #phi" );
    dg::filleff( "taggable_jetdirok" , jet_dir_ok );
    if( jet_dir_ok ) {
      dg::fillh( "deta_track" , 100 , -0.5 , 0.5 , jet_eta - jet->eta_highest_pt_parton() , "#Delta #eta" );
      dg::fillh( "dphi_track" , 100 , -0.5 , 0.5 , detector::delta_phi( jet_phi , static_cast<double>(jet->phi_highest_pt_parton()) ), "#Delta #phi" );
      dg::fillh( "deta_track_jet" , 100 , -0.5 , 0.5 , jet_eta - jet->eta() , "#Delta #eta" );
      dg::fillh( "dphi_track_jet" , 100 , -0.5 , 0.5 , detector::delta_phi( jet_phi , static_cast<double>(jet->phi()) ), "#Delta #phi" );
    }
    // number of (unmatched,nonb,b) tracks.
    unsigned int nunmatched = 0u;
    unsigned int nnonb = 0u;
    unsigned int nb = 0u;
    BOOST_FOREACH( const boost::shared_ptr<const AnaTrack>& track , tracks ) {
      TrackToTruthMap::const_iterator itruth = track_to_truth.find( track );
      if( itruth == track_to_truth.end() ) {
        ++nunmatched;
        continue;
      }
      const boost::shared_ptr<const AnaTrack>& truth = itruth->second;
      if( truth->is_truth_b_decay() ) { ++nb; } else { ++nnonb; }
    }
    const double frac_nunmatched = detector::safe_divide( static_cast<double>(nunmatched) , static_cast<double>(tracks.size()) );
    const double frac_nnonb = detector::safe_divide( static_cast<double>(nnonb) , static_cast<double>(tracks.size()) );
    const double frac_nb = detector::safe_divide( static_cast<double>(nb) , static_cast<double>(tracks.size()) );
    dg::fillh( "n_unmatched_fakeorooc" , 20 , 0 , 20 , nunmatched );
    dg::fillh( "n_nonb" , 20 , 0 , 20 , nnonb );
    dg::fillh( "n_b" , 20 , 0 , 20 , nb );
    dg::fillh( "frac_unmatched_fakeorooc" , 20 , 0 , 1.001 , frac_nunmatched );
    dg::fillh( "frac_nonb" , 20 , 0 , 1.001 , frac_nnonb );
    dg::fillh( "frac_b" , 20 , 0 , 1.001 , frac_nb );
    dg::fillh( "frac_b_vs_dr_b" , 100 , 0 , 1. , 20 , 0 , 1.001 , jet->dr_b_parton() , frac_nb , "dr_b" , "frac b");
    dg::fillh( "frac_light_vs_dr_light" , 100 , 0 , 1. , 20 , 0 , 1.001 , jet->dr_light_parton() , frac_nnonb , "dr_light" , "frac light" );
    dg::fillh( "frac_b_vs_dr_light" , 100 , 0 , 1. , 20 , 0 , 1.001 , jet->dr_light_parton() , frac_nb , "dr light" , "frac b" );
    dg::fillh( "frac_light_vs_dr_b" , 100 , 0 , 1. , 20 , 0 , 1.001 , jet->dr_b_parton() , frac_nnonb , "dr b" , "frac light" );

    dg::fillh( "frac_b_vs_dr_b_any" , 100 , 0 , 1. , 20 , 0 , 1.001 , jet->dr_b_parton_any() , frac_nb , "dr_b" , "frac b");
    dg::fillh( "frac_light_vs_dr_light_any" , 100 , 0 , 1. , 20 , 0 , 1.001 , jet->dr_light_parton_any() , frac_nnonb , "dr_light" , "frac light" );
    dg::fillh( "frac_b_vs_dr_light_any" , 100 , 0 , 1. , 20 , 0 , 1.001 , jet->dr_light_parton_any() , frac_nb , "dr light" , "frac b" );
    dg::fillh( "frac_light_vs_dr_b_any" , 100 , 0 , 1. , 20 , 0 , 1.001 , jet->dr_b_parton_any() , frac_nnonb , "dr b" , "frac light" );
  }
 
  // study track association
  if( true ) {
    // find highest pt track
  }


}

void
study_vertexing( const std::string& name , 
                 vector< boost::shared_ptr<const AnaTrack> > associated_tracks , 
                 const boost::shared_ptr<const AnaJet>& jet)
{
  using boost::format;
  using boost::bind;
  if( associated_tracks.empty() ) { return; }

  StudyVertexFit< vector< boost::shared_ptr<const AnaTrack> > > study( name , "vertex fit studies" , jet );
  study.add_constant_category( "bjet" , jet->dr_b_parton() < 0.3 );
  study.add_constant_category( "ujet" , jet->dr_b_parton() > 0.3 );
  study.for_only( associated_tracks );
}


void
standard_truth_comparison( const std::string& study_name , const std::string& truth_name , const std::string& track_name ,
                           const std::vector< boost::shared_ptr<const AnaTrack> >& truth_collection , 
                           const std::vector< boost::shared_ptr<const AnaTrack> >& track_collection ,
                           const boost::shared_ptr<const AnaJet> jet )
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
  BOOST_FOREACH( const boost::shared_ptr<const AnaTrack>& truth , truth_collection ) {
    // apply some cuts to the truth tracks used for the comparison/efficiencies.
    if( truth->pt() < AnaConfiguration::minimum_track_pt_cut() ) { continue; }
    if( std::abs(truth->eta()) > AnaConfiguration::maximum_track_abseta_cut() ) { continue; }
    if( std::abs(truth->z0()) > 120. ) { continue; }
    if( std::abs(truth->d0()) > 1./*FIXME was 2.2, switch to 1 for new b tagging cut 20100112*/ ) { continue; }
    vector< boost::shared_ptr<const AnaTrack> > matches;
    std::remove_copy_if( track_collection.begin() , track_collection.end() , back_inserter(matches) , 
                         bind(&AnaTrack::barcode,_1) != truth->barcode()
                         || bind(&AnaTrack::barcode_matching_fraction,_1) < AnaConfiguration::barcode_matching_minimum_cut()
                         );
    // count the number of matches
    const unsigned int n_matches = matches.size();
    // get the match with the highest matching fraction (highest number of hits if same fraction)
    const boost::shared_ptr<const AnaTrack> best_match = ( matches.empty() ? 
                                                           boost::shared_ptr<const AnaTrack>() :
                                                           *std::max_element( matches.begin() , matches.end() , AnaTrack::matchingFracNHitsLessThanV() ) );
    // determine if best match has 'good' residuals (reconstructed - generated track parameters)
    const bool best_has_good_parameters = best_match && ( best_match->satisfies_old_parameter_matching_with(truth) );
    dg::fillh( "n_matches_"+study_name , 10 , 0 , 10 , n_matches , "N "  + track_name + " MATCHING " + truth_name );
    // fill some standard efficiency plots vs truth track kinematics. efficiency is 100% if all tracks have n_matches>0.
    StudyTrackingEfficiency st_eff( study_name , track_name + " efficiency w.r.t. " + truth_name );
    st_eff.for_only( StudyTrackingEfficiency::type( truth , n_matches>0 ) );
    // look at efficiency only for tracks passing Guido parameter matching
    st_eff.repurpose( study_name+"_GOOD_PARAMETERS" , track_name + " efficiency w.r.t. " + truth_name + " with good parameter residuals" );
    st_eff.for_only( StudyTrackingEfficiency::type( truth , n_matches>0 && best_has_good_parameters ) );
    st_eff.repurpose( study_name+"_wblayer" , track_name + " efficiency w.r.t. " + truth_name + " with blayer" );
    st_eff.for_only( StudyTrackingEfficiency::type( truth , n_matches>0 && best_match->b_layer_nhits()>0 ) );
    // track parameter differences
    if( best_match ) { 
      plot_track_parameter_differences( study_name , truth , best_match ); 
      dg::down( study_name , track_name + " efficiency w.r.t. " + truth_name ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      if( truth->is_truth_b_decay() ) { 
        plot_track_parameter_differences( "b_decay" , truth , best_match ); 
        StudyTrack study_track( "b_decay" , "match" );
        st_eff.add_constant_category( "high_d0" , std::abs(best_match->d0()) > 0.2 );
        study_track.for_only( best_match );
      }
      if( !truth->is_truth_b_decay() ) { 
        plot_track_parameter_differences( "notb_decay" , truth , best_match ); 
        StudyTrack study_track( "notb_decay" , "match" );
        st_eff.add_constant_category( "high_d0" , std::abs(best_match->d0()) > 0.2 );
        study_track.for_only( best_match );
      }
    }
    // covariance studies
    if( best_match ) {
      TrackIdentification pairing( truth , best_match );
      dg::down( "covariance" , "track covariance studies" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      StudyTrackComparison st( study_name , track_name + " covariance w.r.t. " + truth_name );
      st.for_only( pairing );
    }
    // efficiency vs jet dr.
    if( jet ) {
      dg::down( study_name , track_name + " efficiency w.r.t. " + truth_name ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      const double jet_deta = detector::delta_eta( jet->eta() , truth->eta() );
      const double jet_dphi = detector::delta_phi( jet->phi() , truth->phi0() );
      const double jet_dr = detector::quadrature(jet_deta,jet_dphi);
      // double jet_mindr = std::numeric_limits<double>::max();
      // BOOST_FOREACH( const boost::shared_ptr<const AnaJet>& jet , global_evt->atlas_cone_4_truth_jets() ) {
      //   const double deta = detector::delta_eta( jet->eta() , truth->eta() );
      //   const double dphi = detector::delta_phi( jet->phi() , truth->phi0() );
      //   jet_mindr = std::min( jet_mindr , detector::quadrature(deta,dphi) );
      // }
      dg::filleff( "eff_vs_jet_dr" , 100 , 0 , 0.5 , jet_dr , n_matches>0 , "JET #Delta R" );
      dg::filleff( "dupeff_vs_jet_dr" , 100 , 0 , 0.5 , jet_dr , n_matches>1 , "JET #Delta R" );
      dg::fillh( "nmatches_vs_jet_dr" , 100 , 0 , 0.5 , 10 , 0 , 10 , jet_dr , n_matches , "JET #Delta R" , "Number of matches" );
      // study track resolutions inside jets
      if( jet_dr < 0.2) {
        dg::down( "resolutions_inside_jets" , "track reconsturcted resolutions w.r.t. truth for tracks within 0.2 of jet centroid" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
        BOOST_FOREACH( const boost::shared_ptr<const AnaTrack>& btrack , matches ) { 
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
    // // parameters, rate and properties of multiple matches.
    // if( n_matches>0 ) {
    //   // see plot_track_parameters for an explanation of the following line.
    //   dg::fillh( study_name+"_matched_true_type" , 13 , -6 , 7 , particle_type , "PARTICLE TYPE CODE" );
    //   // get "best" matched track. the figure of merit is the matching fraction. 
    //   // if the matching fraction is the same, matchingFracNHitsLessThan uses the 
    //   // largest number of hits instead.
    //   vector< boost::shared_ptr<const AnaTrack> >::iterator ibest = 
    //     std::max_element( matches.begin() , matches.end() , AnaTrack::matchingFracNHitsLessThanV() );
    //   assert( ibest != matches.end() ); // since n_matches>0, there must be a best match.
    //   const boost::shared_ptr<const AnaTrack>& btrack( *ibest );
    //   // plot parameters for the track matching the truth.
    //   StudyTrack st_track( study_name+"_matched" , "reconstructed "+track_name+" best matching truth" );
    //   st_track.for_only( btrack );
    //   plot_track_parameter_differences( study_name , truth , btrack );
    //   if( truth->is_truth_b_decay() ) { plot_track_parameter_differences( study_name+"_bjet" , truth , btrack ); }
    //   if( truth->is_truth_charged_pion_or_secondary() ) { plot_track_parameter_differences( study_name + "_pion" , truth , btrack ); }
    //   if( n_matches==2 ) { plot_track_parameter_differences( study_name+"_two_duplicates" , matches.front() , matches.back() ); }
    //   if( true ) {
    //     dg::down( "duplicates" , "multiple matches" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
    //     StudyTrackingEfficiency st_eff( study_name , track_name + " multiple match efficiency w.r.t. " + truth_name );
    //     st_eff.for_only( StudyTrackingEfficiency::type( truth , n_matches>=2 ) );
    //   } // end study duplicates
    // } // end if there is at least one match, look at the matches

    //   if( n_matches==0 ) {
    //     dg::down( study_name , track_name + " efficiency w.r.t. " + truth_name ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
    //     dg::down( "efficiency_loss_studies" , "matchless tracks" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
    //     // investigate properties of events in which no comparison track is found
    //     // for the reference. e.g. what are the properties of offline tracks for which no 
    //     // ftksim track is found?
    //     using boost::format;
    //     if( truth->barcode().event_index()!=0 ) { continue; } // skip pileup truth
    //     // cout << " " << " truth does not have a good " << track_name << " match: " << endl
    //     //      << "     " << ( boost::format( "event %1% %2% %3% %4% %|25t|" ) 
    //     //                      % global_evt->run_number() % global_evt->event_number() % truth->barcode().event_index() % truth->barcode().barcode() );
    //     // truth->print( cout );
    //     // find closest matches in barcode and/or phi if any
    //     // vector< boost::shared_ptr<const AnaTrack> > barcode_matches;
    //     // vector< boost::shared_ptr<const AnaTrack> > parameter_matches;
    //     // std::remove_copy_if( track_collection.begin() , track_collection.end() , back_inserter(barcode_matches) ,
    //     //                      bind(&AnaTrack::barcode,_1) != truth->barcode() );
    //     // std::remove_copy_if( track_collection.begin() , track_collection.end() , back_inserter(parameter_matches) ,
    //     //                      bind( logical_not<bool>() , bind( AnaTrack::parametersEqualV(truth) , _1 ) ) );
    //     // if( true ) { 
    //     //   sort( barcode_matches.begin() , barcode_matches.end() , AnaTrack::matchingFracGreaterThanV() );
    //     //   cout << "     barcode matches: " << barcode_matches.size() << " (showing first 5)" << endl;
    //     //   vector< boost::shared_ptr<const AnaTrack> > first_n_barcode_matches;
    //     //   __gnu_cxx::copy_n( barcode_matches.begin() , std::min(static_cast<size_t>(5),barcode_matches.size()) , back_inserter(first_n_barcode_matches) );
    //     //   for_each( first_n_barcode_matches.begin() , first_n_barcode_matches.end() , bind( &AnaTrack::print , _1 ) );
    //     //   sort( parameter_matches.begin() , parameter_matches.end() , AnaTrack::matchingFracGreaterThanV() );
    //     //   cout << "     parameter matches: " << parameter_matches.size() << endl;
    //     //   for_each( parameter_matches.begin() , parameter_matches.end() , bind( &AnaTrack::print , _1 ) );
    //     // }
    //     // dg::fillh( "barcode_matches" , 50 , 0 , 50 , barcode_matches.size() );
    //     // dg::fillh( "parameter_matches" , 50 , 0 , 50 , parameter_matches.size() );
    //     // find highest matching fraction for barcode and parameter match.
    //     //  boost::shared_ptr<const AnaTrack>  best_barcode_match = ( barcode_matches.empty() ? 0 : *std::max_element( barcode_matches.begin() , barcode_matches.end() , AnaTrack::matchingFracNHitsLessThanV() ) );
    //     //  boost::shared_ptr<const AnaTrack>  best_parameter_match = ( parameter_matches.empty() ? 0 : *std::max_element( parameter_matches.begin() , parameter_matches.end() , AnaTrack::matchingFracNHitsLessThanV() ) );
    //     // if( best_barcode_match ) { plot_track_parameter_differences( "best_barcode_match" , track , best_barcode_match ); }
    //     // if( best_parameter_match ) { plot_track_parameter_differences( "best_parameter_match" , track , best_parameter_match ); }
    //   } // end if matchless tracks, study them
    // }
    dg::fillh( "n_matching_"+study_name+"_tracks" , 20 , 0 , 20 , n_matching , "N " + truth_name + " WITH " + track_name + " MATCH" );
    dg::fillh( "n_multiple_"+study_name+"_tracks" , 20 , 0 , 20 , n_multiple_matching , "N " + truth_name + " WITH MULTIPLE " + track_name + " MATCHES" );
    // fake rate
    if( true ) {
      dg::down( study_name , track_name + " efficiency w.r.t. " + truth_name ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      BOOST_FOREACH( const boost::shared_ptr<const AnaTrack>& track , track_collection ) {
        if( track->pt() < AnaConfiguration::minimum_track_pt_cut() ) { continue; }
        if( std::abs(track->eta()) > AnaConfiguration::maximum_track_abseta_cut() ) { continue; }
        dg::down( "fakes" , "fake rate" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
        // fake rate vs kinematic variables.
        StudyTrackingEfficiency st_eff( study_name + "_fakes" , track_name + " fake rate w.r.t. " + truth_name );
        const bool is_fake = ( track->barcode().barcode()==-1 || track->barcode().barcode()==0 || (track->barcode_matching_fraction() < AnaConfiguration::barcode_matching_minimum_cut()) );
        st_eff.for_only( StudyTrackingEfficiency::type( track , is_fake ) );
        if( std::abs(track->d0())>0.2 ) {
          if( is_fake ) { 
            // properties of fake tracks with large d0
            StudyTrack study_track( "large_d0_fakes" , "fakes" );
            study_track.for_only( track );
          } else {
            // properties of fake tracks with large d0
            StudyTrack study_track( "large_d0_notfakes" , "not fakes" );
            study_track.for_only( track );
          }
        }
        // fake rate vs jet dr
        if( jet ) { 
          const double jet_deta = detector::delta_eta( jet->eta() , track->eta() );
          const double jet_dphi = detector::delta_phi( jet->phi() , track->phi0() );
          const double jet_dr = detector::quadrature(jet_deta,jet_dphi);
          dg::filleff( "fake_vs_jet_dr" , 100 , 0 , 0.5 , jet_dr , is_fake , "JET #Delta R" );
        }
      }
    }
  }
} // end standard_truth_comparison

  
void
standard_jet_tracks( const std::string& study_name , const std::string& track_name ,
                     const std::vector< boost::shared_ptr<const AnaTrack> >& track_collection ,
                     const boost::shared_ptr<const AnaJet> jet )
{
  StudyTrack study_track( study_name , "plots of tracks associated with jets" );

  study_track.for_each_if( track_collection.begin() , track_collection.end() , 
                           bind(&AnaTrack::pt,_1) >= AnaConfiguration::minimum_track_pt_cut() &&
                           bind( std::ptr_fun<AnaTrack::ftype,AnaTrack::ftype>(std::abs) , bind(&AnaTrack::eta,_1)) <= AnaConfiguration::maximum_track_abseta_cut() &&
                           bind( std::ptr_fun<AnaTrack::ftype,AnaTrack::ftype>(std::abs) , bind(&AnaTrack::d0,_1)) <= 120. &&
                           bind( std::ptr_fun<AnaTrack::ftype,AnaTrack::ftype>(std::abs) , bind(&AnaTrack::d0,_1)) <= 2.2 );

  //  dg::down( study_name , "" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
}

void
plot_track_parameter_differences( const std::string& label_name , 
                                  const boost::shared_ptr<const AnaTrack>& reference_track , 
                                  const boost::shared_ptr<const AnaTrack>& comparison_track )
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
  dg::fillh( "dd0" , 200 , -0.1 , 0.1 , (comparison_track->d0()-reference_track->d0())/10. ,
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
  dg::fillh( "deta" , 100 , -0.05 , 0.05 , (comparison_track->eta()-reference_track->eta()) ,
             comp_name + " #eta - " + ref_name + " #eta" );
  dg::fillh( "dphi" , 100 , -0.05 , 0.05 , (comparison_track->phi0()-reference_track->phi0()) ,
             comp_name + " #phi_{0} - " + ref_name + " #phi_{0} (rad)" );
  dg::fillh( "dphi_narrow" , 200 , -0.01 , 0.01 , (comparison_track->phi0()-reference_track->phi0()) ,
             comp_name + " #phi_{0} - " + ref_name + " #phi_{0} (rad)" );
  dg::fillh( "dchi2_narrow" , 100 , -1 , 1 , (comparison_track->chi2()-reference_track->chi2()) ,
             comp_name + " #chi^{2} - " + ref_name + " #chi^{2}" );
  dg::fillh( "dchi2_wide" , 100 , -25 , 25 , (comparison_track->chi2()-reference_track->chi2()) ,
             comp_name + " #chi^{2} - " + ref_name + " #chi^{2}" );
  dg::fillh( "dnhit" , 5 , -2 , 2 , (comparison_track->nhits()-reference_track->nhits()) ,
             comp_name + " N HIT - " + ref_name + " N HIT" );
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


#ifdef F77_DUMMY_MAIN
#ifdef __cplusplus
extern "C"
#endif
int F77_DUMMY_MAIN() { abort(); return 1; }
#endif

