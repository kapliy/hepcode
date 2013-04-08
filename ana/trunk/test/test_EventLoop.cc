
#include <cassert>
#include <cmath>
#include <algorithm>
#include <functional>
#include <string>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <iostream>
#include <iomanip>
#include "TFile.h"
#include "TrigFTKAna/AnaEventMgr.hpp"
#include "TrigFTKAna/AnaTrack.hpp"
#include "TrigFTKAna/AnaConfiguration.hpp"
#include "DataGraphics/DataGraphics.hh"

using namespace std;
using namespace DataGraphics;
using boost::shared_ptr;

void standard_truth_comparison( const std::string& track_name , const std::vector<shared_ptr<const AnaTrack> >& truth_collection , const std::vector<shared_ptr<const AnaTrack> >& track_collection );
void standard_track_comparison( const std::string& track_name , const std::vector<shared_ptr<const AnaTrack> >& track_collection_ref ,
                                const std::vector<shared_ptr<const AnaTrack> >& track_collection_comp );
void plot_track_parameters( const std::string& track_name , shared_ptr<const AnaTrack>  track );
void plot_track_parameter_differences( const std::string& track_name , shared_ptr<const AnaTrack>  reference_Track , shared_ptr<const AnaTrack>  comparison_track );
void plot_tracking_efficiency( const std::string& algorithm_name , shared_ptr<const AnaTrack>  reference_track , const bool& success );

int
main( void )
{

  // define some job configuration
  const string sample_definition_file = "./test/test_EventLoop_definition.dat.bz2";

  // initialize plotting tool. this sets up a filename to which the
  // plots will eventually be saved. saving is automatically done when
  // the job finishes.
  dg::root()->set_save_filename( "./test/test_EventLoop.root" );

  // initialize data manager. this thing synchronizes all of the
  // underlying ntuples (wrapper text files, road finder/merger
  // output, track fitter output, athena ntuples) and provides a
  // simple interface (AnaEvent) to their data.
  AnaEventMgr* evtmgr = AnaEventMgr::instance(); assert(evtmgr);
   // each sample is described by a text file in this directory
  AnaConfiguration::full_sample_definition_path( sample_definition_file );

  // load a sample description and prepare to loop over events
  const bool ok = evtmgr->open_sample();
  if( !ok ) { cout << "open sample failed" << endl; return -1; }

  // loop over each event in the sample. by default, AnaEventMgr::begin and ::end 
  // give you a loop over the entire sample, but the same interface can iterate
  // over e.g. a short list of run/event pairs.

  for( AnaEventMgr::iterator i=evtmgr->begin(), f=evtmgr->end(); i!=f; ++i ) {

    shared_ptr<const AnaEvent> evt = *i;
    if( !evt ) { break; } // no AnaEvent means something went wrong, e.g. premature end of file

    // all the event data are stored as STL containers of Ana* objects. these function
    // calls retrieve each type of data. in principle, the data are not loaded from disk
    // until they are requested here (in practice, this is not yet implemented).
    const vector< shared_ptr<const AnaTrack> >& ftk_tracks( evt->ftk_tracks(AnaEventMgr::DEFAULT_SOURCE) );
    const vector< shared_ptr<const AnaTrack> >& trigftklib_tracks( evt->trigftklib_tracks(AnaEventMgr::DEFAULT_SOURCE) );
    const vector< shared_ptr<const AnaTrack> >& offline_tracks( evt->offline_tracks(AnaEventMgr::DEFAULT_SOURCE) );
    const vector< shared_ptr<const AnaTrack> >& truth_tracks( evt->truth_tracks(AnaEventMgr::DEFAULT_SOURCE) );
    if( truth_tracks.empty() ) { continue; }

    standard_truth_comparison( "ftk" , truth_tracks , ftk_tracks );
    standard_truth_comparison( "trigftklib" , truth_tracks , trigftklib_tracks );
    standard_truth_comparison( "offline" , truth_tracks , offline_tracks );
    standard_track_comparison( "ftk_wrt_offline" , offline_tracks , ftk_tracks );
    standard_track_comparison( "trigftklib_wrt_offline" , offline_tracks , trigftklib_tracks );
    standard_track_comparison( "ftk_wrt_trigftklib" , trigftklib_tracks , ftk_tracks );

    // count numbers of tracks with pT > 1 GeV
    const unsigned int n_truth = std::count_if( truth_tracks.begin() , truth_tracks.end() , boost::bind(&AnaTrack::pt,_1) > 1. );
    const unsigned int n_ftk = std::count_if( ftk_tracks.begin() , ftk_tracks.end() , boost::bind(&AnaTrack::pt,_1) > 1. );
    const unsigned int n_offline = std::count_if( offline_tracks.begin() , offline_tracks.end() , boost::bind(&AnaTrack::pt,_1) > 1. );
    // plot some track collection and matching statistics
    dg::fillh( "n_truth_tracks" , 600 , 0 , 600 , n_truth );
    dg::fillh( "n_ftk_tracks" , 600 , 0 , 600 , n_ftk );
    dg::fillh( "n_offline_tracks" , 600 , 0 , 600 , n_offline );

    // make standard plots of the tracks in each collection.
    // ================================================================
    // make standard track parameter plots for each truth track.
    for( vector<shared_ptr<const AnaTrack> >::const_iterator itrack=truth_tracks.begin(), 
           ftrack=truth_tracks.end(); itrack!=ftrack; ++itrack ) {
      shared_ptr<const AnaTrack>  track(*itrack);
      plot_track_parameters( "truth" , track );
      // see plot_track_parameters below for an explanation of how the following line works.
      const int particle_type = detector::particle_type( track->pdg_code() ) * detector::sign_of( track->pdg_code() );
      dg::fillh( "truth_type" , 13 , -6 , 7 , particle_type );
    }
    // make standard track parameter plots for each ftk track
    for( vector<shared_ptr<const AnaTrack> >::const_iterator itrack=ftk_tracks.begin(),
           ftrack=ftk_tracks.end(); itrack!=ftrack; ++itrack ) {
      shared_ptr<const AnaTrack>  track(*itrack);
      if( track->pt() < 1. ) { continue; }
      plot_track_parameters( "ftk" , track );
    }
    // make standard track parameter plots for each TrigFTKLib track
    for( vector<shared_ptr<const AnaTrack> >::const_iterator itrack=trigftklib_tracks.begin(),
           ftrack=trigftklib_tracks.end(); itrack!=ftrack; ++itrack ) {
      shared_ptr<const AnaTrack>  track(*itrack);
      if( track->pt() < 1. ) { continue; }
      plot_track_parameters( "trigftklib" , track );
    }
    // make standard track parameter plots for each offline track
    for( vector<shared_ptr<const AnaTrack> >::const_iterator itrack=offline_tracks.begin(),
           ftrack=offline_tracks.end(); itrack!=ftrack; ++itrack ) {
      shared_ptr<const AnaTrack>  track(*itrack);
      if( track->pt() < 1. ) { continue; }
      plot_track_parameters( "offline" , track );
    }

  } // end for each event
  
}

void
standard_truth_comparison( const std::string& track_name , const std::vector<shared_ptr<const AnaTrack> >& truth_collection , const std::vector<shared_ptr<const AnaTrack> >& track_collection )
{
  // loop over all truth tracks in truth_collection and look for matching reconstructed tracks in track_collection.
  unsigned int n_matching = 0u;
  unsigned int n_multiple_matching = 0u;
  for( vector<shared_ptr<const AnaTrack> >::const_iterator itruth=truth_collection.begin(), ftruth=truth_collection.end(); itruth!=ftruth; ++itruth ) {
    shared_ptr<const AnaTrack>  truth( *itruth );
    // apply some cuts to the truth tracks used for the comparison/efficiencies.
    if( truth->pt() < 1. ) { continue; }
    if( std::abs( truth->eta() ) > 1. ) { continue; }
    // get the "type" of truth track./ detector::particle_type(
    // pdg_code ) returns detector::PROTON , ::KAON , ::PION , ::MUON ,
    // ::ELECTRON , etc.  convert these to a integer signed by the
    // charge of the particle.
    const int particle_type = detector::particle_type( truth->pdg_code() ) * detector::sign_of( truth->pdg_code() );
    // retrieve reconstructed tracks assigned the same barcode as this truth track and store them in a vector.
    // the remove_copy_if function copies tracks from "track_collection" into "matches" as long as the fourth
    // argument to the function is false. That argument,
    //    boost::bind(&AnaTrack::unique_barcode,_1) != truth->unique_barcode()
    // calls the AnaTrack::unique_barcode function for the track and compares the barcode with truth->unique_barcode().
    vector<shared_ptr<const AnaTrack> > matches;
    std::remove_copy_if( track_collection.begin() , track_collection.end() , back_inserter(matches) , 
                         boost::bind(&AnaTrack::barcode,_1) != truth->barcode() );
    // count the number of matches
    const unsigned int n_matches = matches.size();
    if( n_matches>0 ) { ++n_matching; }
    if( n_matches>1 ) { ++n_multiple_matching; }
    // histogram the number of reconstructed tracks matching this truth track
    dg::fillh( "n_matches"+track_name , 10 , 0 , 10 , n_matches );
    // fill some standard efficiency plots. efficiency is 100% if all tracks have n_matches>0.
    plot_tracking_efficiency( track_name , truth , n_matches>0 );
    // see plot_tracking_efficiency for an explanation of the following line.
    dg::filleff( track_name+"_eff_vs_type" , 13 , -6 , 7 , particle_type , n_matches>0 );
    if( n_matches>0 ) {
      // plot truth track parameters when a match is found
      plot_track_parameters( "truth_matched_"+track_name+"_true" , truth );
      // see plot_track_parameters for an explanation of the following line.
      dg::fillh( "truth_matched_"+track_name+"_true_type" , 13 , -6 , 7 , particle_type );
      // get "best" matched track. the figure of merit is the matching fraction. 
      // if the matching fraction is the same, matchingFracNHitsLessThan uses the 
      // largest number of hits instead.
      vector<shared_ptr<const AnaTrack> >::iterator ibest = 
        std::max_element( matches.begin() , matches.end() , AnaTrack::matchingFracNHitsLessThanV() );
      assert( ibest != matches.end() ); // since n_matches>0, there must be a best match.
      shared_ptr<const AnaTrack>  btrack( *ibest );
      // reject best matches with fewer then 80% matching hits
      if( btrack->barcode_matching_fraction()<0.8 ) { continue; } 
      // plot parameters for the track matching the truth.
      plot_track_parameters( "truth_matched_"+track_name , btrack );
      plot_track_parameter_differences( "truth_matched_"+track_name , truth , btrack );
    } // end if there is at least one match, look at the matches
  } // end for each good truth track
  dg::fillh( "n_matching_"+track_name+"_tracks" , 20 , 0 , 20 , n_matching );
  dg::fillh( "n_multiple_matching_"+track_name+"_tracks" , 20 , 0 , 20 , n_multiple_matching );
} // end standard_truth_comparison

void
standard_track_comparison( const std::string& track_name , 
                           const std::vector<shared_ptr<const AnaTrack> >& track_collection_ref , 
                           const std::vector<shared_ptr<const AnaTrack> >& track_collection_comp )
{
  // loop over all track tracks in track_collection_ref and look for matching reconstructed tracks in track_collection_comp.
  unsigned int n_matching = 0u;
  unsigned int n_multiple_matching = 0u;
  for( vector<shared_ptr<const AnaTrack> >::const_iterator itrack=track_collection_ref.begin(), ftrack=track_collection_ref.end(); itrack!=ftrack; ++itrack ) {
    shared_ptr<const AnaTrack>  track( *itrack );
    // apply some cuts to the track tracks used for the comparison/efficiencies.
    if( track->pt() < 1. ) { continue; }
    if( std::abs( track->eta() ) > 1. ) { continue; }
    if( track->barcode_matching_fraction() < 0.8 ) { continue; }
    // retrieve reconstructed tracks assigned the same barcode as this track track and store them in a vector.
    vector<shared_ptr<const AnaTrack> > matches;
    std::remove_copy_if( track_collection_comp.begin() , track_collection_comp.end() , back_inserter(matches) , 
                         boost::bind(&AnaTrack::barcode,_1) != track->barcode() );
    // count the number of matches
    const unsigned int n_matches = matches.size();
    if( n_matches>0 ) { ++n_matching; }
    if( n_matches>1 ) { ++n_multiple_matching; }
    // histogram the number of reconstructed tracks matching this ref track
    dg::fillh( "n_matches"+track_name , 10 , 0 , 10 , n_matches );
    // fill some standard efficiency plots. efficiency is 100% if all tracks have n_matches>0.
    plot_tracking_efficiency( track_name , track , n_matches>0 );
    if( n_matches>0 ) {
      // plot ref track parameters when a match is found
      plot_track_parameters( "track_matched_"+track_name+"_ref" , track );
      // get "best" matched track. the figure of merit is the matching fraction. 
      // if the matching fraction is the same, matchingFracNHitsLessThan uses the 
      // largest number of hits instead.
      vector<shared_ptr<const AnaTrack> >::iterator ibest = 
        std::max_element( matches.begin() , matches.end() , AnaTrack::matchingFracNHitsLessThanV() );
      assert( ibest != matches.end() ); // since n_matches>0, there must be a best match.
      shared_ptr<const AnaTrack>  btrack( *ibest );
      // reject best matches with fewer then 80% matching hits
      if( btrack->barcode_matching_fraction()<0.8 ) { continue; } 
      // plot parameters for the track matching the track.
      plot_track_parameters( "track_matched_"+track_name , btrack );
      plot_track_parameter_differences( "track_matched_"+track_name , track , btrack );
    } // end if there is at least one match, look at the matches
  } // end for each good track track
  dg::fillh( "n_matching_"+track_name+"_tracks" , 20 , 0 , 20 , n_matching );
  dg::fillh( "n_multiple_matching_"+track_name+"_tracks" , 20 , 0 , 20 , n_multiple_matching );
} // end standard_track_comparison

void
plot_track_parameters( const std::string& track_name , shared_ptr<const AnaTrack>  track )
{
  // fill some one-dimensional histograms. the parameter order is: name , nbins , xmin , xmax , x.
  // note that one never needs to declare the histogram at some earlier point, or save it at some later point.
  // those things are taken care of automatically.
  dg::down( track_name , "track parameters" );
  dg::fillh( "pt" , 100 , 0 , 25. , track->pt() );
  dg::fillh( "curv" , 100 , -0.5 , 0.5 , track->curvature()*1000. );
  dg::fillh( "d0" , 100 , -0.2 , 0.2 , track->d0() );
  dg::fillh( "z0" , 100 , -10 , 10 , track->z0() );
  dg::fillh( "eta" , 60 , -3 , 3. , track->eta() );
  dg::fillh( "phi" , 48 , -M_PI , M_PI , track->phi0() );
  dg::up();
}

void
plot_track_parameter_differences( const std::string& track_name , shared_ptr<const AnaTrack>  reference_track , shared_ptr<const AnaTrack>  comparison_track )
{
  // plot differences between e.g. reconstruced and true track parameters.
  dg::down( track_name , "track parameters" );
  dg::fillh( "dcurv" , 100 , -0.05 , 0.05 , (comparison_track->curvature()-reference_track->curvature())*1000. );
  dg::fillh( "dd0" , 100 , -0.02 , 0.02 , (comparison_track->d0()-reference_track->d0()) );
  dg::fillh( "dz0" , 100 , -0.1 , 0.1 , (comparison_track->z0()-reference_track->z0()) );
  dg::fillh( "deta" , 100 , -0.05 , 0.05 , (comparison_track->eta()-reference_track->eta()) );
  dg::fillh( "dphi" , 100 , -0.05 , 0.05 , (comparison_track->phi0()-reference_track->phi0()) );
  dg::up();
}

void
plot_tracking_efficiency( const std::string& algorithm_name , shared_ptr<const AnaTrack>  reference_track , const bool& success )
{
  // fill some efficiency plots. the first just records an overall efficiency number.
  // the remaining filleff plots record the efficiency in bins of pt, curvature, etc. with
  // a syntax similar to the one-dimensional histogram fillh function.
  dg::down( algorithm_name+"_efficiency" , "tracking algorithm efficiency" );
  dg::filleff( "eff" , success );
  dg::filleff( "eff_vs_pt" , 100 , 0 , 25. , reference_track->pt() , success );
  dg::filleff( "eff_vs_curv" , 100 , -0.5 , 0.5 , reference_track->curvature()*1000. , success );
  dg::filleff( "eff_vs_d0" , 100 , -0.2 , 0.2 , reference_track->d0() , success );
  dg::filleff( "eff_vs_z0" , 100 , -10 , 10 , reference_track->z0() , success );
  dg::filleff( "eff_vs_eta" , 60 , -3 , 3 , reference_track->eta() , success );
  dg::filleff( "eff_vs_phi" , 48 , -M_PI , M_PI , reference_track->phi0() , success );
  dg::up();
}
