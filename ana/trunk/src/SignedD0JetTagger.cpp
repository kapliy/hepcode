
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <utility>
#include <algorithm>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/format.hpp>
#include <boost/foreach.hpp>
#include <boost/scope_exit.hpp>
#include "TrigFTKAna/AnaJet.hpp"
#include "TrigFTKAna/AnaTrack.hpp"
#include "TrigFTKAna/SignedD0JetTagger.hpp"
#include "TrigFTKAna/StudyTrack.hpp"
#include "TrigFTKAna/StudyJetTag.hpp"
#include "DataGraphics/DgCollection.hh"
#include "TrigFTKAna/AnaConfiguration.hpp"
#include "TrigFTKAna/AnaVertexFit.hpp"

using namespace std;
using namespace DataGraphics;

// 20100101 - monica-like binning for 40k
// const bool raw_use_trif = false;
// const unsigned int raw_nbins_trif[3] = { 6 , 10 , 6 };
// const double raw_boundaries_trif[4] = { -2 , -0.2 , 0.2 , 2 };
// const unsigned int raw_nbins_even = 108;
// const double raw_boundaries_even[2] = { -2.5 , 2.5 };
// const bool sig_use_trif = false;
// const unsigned int sig_nbins_trif[3] = { 6 , 10 , 6 };
// const double sig_boundaries_trif[4] = { -25 , -3 , 3 , 25 };
// const unsigned int sig_nbins_even = 50;
// const double sig_boundaries_even[2] = { -25 , 25 };

// const bool raw_use_trif = true;
// const unsigned int raw_nbins_trif[3] = { 2 , 10 , 6 };
// const double raw_boundaries_trif[4] = { -1 , -0.1 , 0.1 , 1 };
// const unsigned int raw_nbins_even = 108;
// const double raw_boundaries_even[2] = { -2.5 , 2.5 };
// const bool sig_use_trif = true;
// const unsigned int sig_nbins_trif[3] = { 2 , 8 , 6 };
// const double sig_boundaries_trif[4] = { -10 , -2.5 , 2.5 , 32.5 };
// const unsigned int sig_nbins_even = 50;
// const double sig_boundaries_even[2] = { -25 , 25 };

// const bool raw_use_trif = true;
// const unsigned int raw_nbins_trif[3] = { 1 , 5 , 3 };
// const double raw_boundaries_trif[4] = { -1 , -0.1 , 0.1 , 1 };
// const unsigned int raw_nbins_even = 108;
// const double raw_boundaries_even[2] = { -2.5 , 2.5 };
// const bool sig_use_trif = true;
// const unsigned int sig_nbins_trif[3] = { 1 , 4 , 3 };
// const double sig_boundaries_trif[4] = { -10 , -2.5 , 2.5 , 32.5 };
// const unsigned int sig_nbins_even = 50;
// const double sig_boundaries_even[2] = { -25 , 25 };

const bool raw_use_trif = true;
const unsigned int raw_nbins_trif[3] = { 2 , 10 , 6 };
const double raw_boundaries_trif[4] = { -1 , -0.1 , 0.1 , 1 };
const unsigned int raw_nbins_even = 108;
const double raw_boundaries_even[2] = { -2.5 , 2.5 };
const bool sig_use_trif = true;
const unsigned int sig_nbins_trif[3] = { 2 , 8 , 6 };
const double sig_boundaries_trif[4] = { -5 , -2.5 , 2.5 , 32.5 };
const unsigned int sig_nbins_even = 50;
const double sig_boundaries_even[2] = { -25 , 25 };

SignedD0JetTagger::SignedD0JetTagger() 
  : JetTagger()
  , _use_significance( false )
  , _use_sign( true )
  , _use_track_jet_direction( false )
  , _use_impure_tracks( false )
  , _use_d0( true )
  , _use_z0( false )
  , _use_z0_significance( false )
  , _use_sv_variables( false )
{
  // _lik_d0_b.trifold_uniform_binning( 6 , 10 , 6 , 
  //                                 -2. , -0.2 , 0.2 , 2. );
  // _lik_d0_notb.trifold_uniform_binning( 6 , 10 , 6 , 
  //                                    -2. , -0.2 , 0.2 , 2. );
    // _lik_d0_b.uniform_binning( 80 , -2 , 2 );
    // _lik_d0_notb.uniform_binning( 80 , -2 , 2 );
    // _lik_d0_b.uniform_binning( 80 , 0 , 2 );
    // _lik_d0_notb.uniform_binning( 80 , 0 , 2 );
  // _lik_d0_b.uniform_binning( 100 , -2.5 , 2.5 );
  // _lik_d0_notb.uniform_binning( 100 , -2.5 , 2.5 );

  // binning up until 20100114
  if( raw_use_trif ) { 
    _lik_d0_b.trifold_uniform_binning( raw_nbins_trif[0] , raw_nbins_trif[1] , raw_nbins_trif[2] ,
                            raw_boundaries_trif[0] , raw_boundaries_trif[1] , raw_boundaries_trif[2] , raw_boundaries_trif[3] );
    _lik_d0_notb.trifold_uniform_binning( raw_nbins_trif[0] , raw_nbins_trif[1] , raw_nbins_trif[2] ,
                               raw_boundaries_trif[0] , raw_boundaries_trif[1] , raw_boundaries_trif[2] , raw_boundaries_trif[3] );
  } else { 
    _lik_d0_b.uniform_binning( raw_nbins_even , raw_boundaries_even[0] , raw_boundaries_even[1] );
    _lik_d0_notb.uniform_binning( raw_nbins_even , raw_boundaries_even[0] , raw_boundaries_even[1] );
  }

  // _lik_d0_b.trifold_uniform_binning( 3 , 10 , 3 , 
  //                                 -1. , -0.4 , 0.4 , 1. );
  // _lik_d0_notb.trifold_uniform_binning( 3 , 10 , 3 , 
  //                                    -1. , -0.4 , 0.4 , 1. );

  // _lik_d0_b.uniform_binning( 32 , -0.75 , 0.75 );
  // _lik_d0_notb.uniform_binning( 32 , -0.75 , 0.75 );
}

SignedD0JetTagger::SignedD0JetTagger( DgCollection* dgcoll )
  : JetTagger( dgcoll )
  , _use_significance( false )
  , _use_sign( true )
  , _use_track_jet_direction( false )
  , _use_impure_tracks( false )
  , _use_d0( true )
  , _use_z0( false )
  , _use_z0_significance( false )
  , _use_sv_variables( false )
{
  // _lik_d0_b.trifold_uniform_binning( 6 , 10 , 6 , 
  //                                 -2. , -0.2 , 0.2 , 2. );
  // _lik_d0_notb.trifold_uniform_binning( 6 , 10 , 6 , 
  //                                    -2. , -0.2 , 0.2 , 2. );
    // _lik_d0_b.uniform_binning( 80 , -2 , 2 );
    // _lik_d0_notb.uniform_binning( 80 , -2 , 2 );
  // _lik_d0_b.uniform_binning( 100 , -2.5 , 2.5 );
  // _lik_d0_notb.uniform_binning( 100 , -2.5 , 2.5 );

  // binning up until 20100114
  if( raw_use_trif ) { 
    _lik_d0_b.trifold_uniform_binning( raw_nbins_trif[0] , raw_nbins_trif[1] , raw_nbins_trif[2] ,
                                    raw_boundaries_trif[0] , raw_boundaries_trif[1] , raw_boundaries_trif[2] , raw_boundaries_trif[3] );
    _lik_d0_notb.trifold_uniform_binning( raw_nbins_trif[0] , raw_nbins_trif[1] , raw_nbins_trif[2] ,
                                       raw_boundaries_trif[0] , raw_boundaries_trif[1] , raw_boundaries_trif[2] , raw_boundaries_trif[3] );
  } else { 
    _lik_d0_b.uniform_binning( raw_nbins_even , raw_boundaries_even[0] , raw_boundaries_even[1] );
    _lik_d0_notb.uniform_binning( raw_nbins_even , raw_boundaries_even[0] , raw_boundaries_even[1] );
  }
  // _lik_d0_b.trifold_uniform_binning( 3 , 10 , 3 , 
  //                                 -1. , -0.4 , 0.4 , 1. );
  // _lik_d0_notb.trifold_uniform_binning( 3 , 10 , 3 , 
  //                                    -1. , -0.4 , 0.4 , 1. );
}

SignedD0JetTagger*
SignedD0JetTagger::use_significance( const bool& yes ) { 
  _use_significance = yes; 
  if( yes ) {
//     _lik_d0_b.trifold_uniform_binning( 23 , 8 , 23 , 
//                                     -25. , -2 , 2 , 25. );
//     _lik_d0_notb.trifold_uniform_binning( 23 , 8 , 23 , 
//                                        -25. , -2 , 2 , 25. );
    // until 20100117
    if( sig_use_trif ) {
      _lik_d0_b.trifold_uniform_binning( sig_nbins_trif[0] , sig_nbins_trif[1] , sig_nbins_trif[2] ,
                                      sig_boundaries_trif[0] , sig_boundaries_trif[1] , sig_boundaries_trif[2] , sig_boundaries_trif[3] );
      _lik_d0_notb.trifold_uniform_binning( sig_nbins_trif[0] , sig_nbins_trif[1] , sig_nbins_trif[2] ,
                                         sig_boundaries_trif[0] , sig_boundaries_trif[1] , sig_boundaries_trif[2] , sig_boundaries_trif[3] );
    } else {
      _lik_d0_b.uniform_binning( sig_nbins_even , sig_boundaries_even[0] , sig_boundaries_even[1] );
      _lik_d0_notb.uniform_binning( sig_nbins_even , sig_boundaries_even[0] , sig_boundaries_even[1] );
    }

    // _lik_d0_b.trifold_uniform_binning(  3, 16 , 3 , 
    //                                 -25. , -4 , 4 , 25. );
    // _lik_d0_notb.trifold_uniform_binning( 3 , 16 , 3 , 
    //                                    -25. , -4 , 4 , 25. );
  }
  return this; 
}

SignedD0JetTagger*
SignedD0JetTagger::use_sign( const bool& yes ) { 
  _use_sign = yes;
  if( !_use_sign ) {
    // binning up until 20100114
    if( raw_use_trif ) { 
      _lik_d0_b.trifold_uniform_binning( raw_nbins_trif[0] , raw_nbins_trif[1] , raw_nbins_trif[2] ,
                                      raw_boundaries_trif[0] , raw_boundaries_trif[1] , raw_boundaries_trif[2] , raw_boundaries_trif[3] );
      _lik_d0_notb.trifold_uniform_binning( raw_nbins_trif[0] , raw_nbins_trif[1] , raw_nbins_trif[2] ,
                                         raw_boundaries_trif[0] , raw_boundaries_trif[1] , raw_boundaries_trif[2] , raw_boundaries_trif[3] );
    } else { 
      _lik_d0_b.uniform_binning( raw_nbins_even , raw_boundaries_even[0] , raw_boundaries_even[1] );
      _lik_d0_notb.uniform_binning( raw_nbins_even , raw_boundaries_even[0] , raw_boundaries_even[1] );
    }
    // _lik_d0_b.trifold_uniform_binning( 3 , 10 , 3 , 
    //                                 -1. , -0.4 , 0.4 , 1. );
    // _lik_d0_notb.trifold_uniform_binning( 3 , 10 , 3 , 
    //                                    -1. , -0.4 , 0.4 , 1. );
    if( _use_significance ) { use_significance(true); } // modify binning if requested.
  }
  return this;
}

SignedD0JetTagger*
SignedD0JetTagger::use_track_jet_direction( const bool& yes ) { 
  _use_track_jet_direction = yes;
  _jet_centroid_option = JetTagger::TRACK_CENTROID;
  return this;
}

SignedD0JetTagger*
SignedD0JetTagger::use_impure_tracks( const bool& yes ) { 
  _use_impure_tracks = yes; 
  if( yes ) {
    use_track_jet_direction( _use_track_jet_direction );
    use_sign( _use_sign );
    _lik_d0_b_impure = _lik_d0_b;
    _lik_d0_notb_impure = _lik_d0_notb;
  }
  return this; 
}

SignedD0JetTagger*
SignedD0JetTagger::use_z0( const bool& yes , const bool& yes_significance ) { 
  _use_z0 = yes; 
  _use_z0_significance = yes_significance; 
  if( yes ) {
    if( yes_significance ) { 
      _lik_z0_b.trifold_uniform_binning( 1 , 10 , 1 , -25 , -5 , 5 , 25 );
      _lik_z0_notb.trifold_uniform_binning( 1 , 10 , 1 , -25 , -5 , 5 , 25 );
    } else {
      _lik_z0_b.trifold_uniform_binning( 4 , 1 , 1 , 0 , 1 , 2 , 6 );
      _lik_z0_notb.trifold_uniform_binning( 4 , 1 , 1 , 0 , 1 , 2 , 6 );
    }
  }
  return this; 
}

SignedD0JetTagger*
SignedD0JetTagger::use_sv_variables( const bool& yes ) { 
  _use_sv_variables = yes; 
  if( yes ) {
    _lik_vertex_mass_b.trifold_uniform_binning( 4 , 1 , 1 , 0 , 1 , 2 , 6 );
    _lik_vertex_mass_notb.trifold_uniform_binning( 4 , 1 , 1 , 0 , 1 , 2 , 6 );
    _lik_pt_ratio_b.uniform_binning( 3 , 0 , 1 );
    _lik_pt_ratio_notb.uniform_binning( 3 , 0 , 1 );
    _lik_n_two_track_b.trifold_uniform_binning( 2 , 2 , 1 , 0 , 2 , 4 , 50 );
    _lik_n_two_track_notb.trifold_uniform_binning( 2 , 2 , 1 , 0 , 2 , 4 , 50 );
    _lik_has_vertex_b.uniform_binning( 2 , 0 , 2 );
    _lik_has_vertex_notb.uniform_binning( 2 , 0 , 2 );
  }
  return this; 
}

const bool
SignedD0JetTagger::_train( const boost::shared_ptr<const AnaJet>& jet , const std::vector< boost::shared_ptr<const AnaTrack> >& tracks )
{
  if( tracks.empty() ) { return false; }
  // train signed d0 likelihoods. called with flag telling us whether or not this jet is supposed to be signal.
  //
  // get jet centroid eta and phi
  double jet_phi, jet_eta;
  if( _use_track_jet_direction ) { 
    const bool jet_dir_ok = compute_jet_eta_phi( jet , tracks , JetTagger::TRACK_CENTROID , jet_eta , jet_phi );
    if( _dgcoll ) {
      dg::filleff( "jet_dir_ok" , jet_dir_ok );
    }
    if( !jet_dir_ok ) { return false; } // don't use jets with unknown direction for signed d0 training.
  } else {
    jet_eta = jet->eta();
    jet_phi = jet->phi();
  }

  // loop over tracks and identify z vertex as z0 of highest pT track.
  double zvert_pt = 0.;
  double zvert_z = 0.;
  double zvert_zerr = 0.;
  for( vector< boost::shared_ptr<const AnaTrack> >::const_iterator itrack=tracks.begin(), ftrack=tracks.end(); itrack!=ftrack; ++itrack ) {
    const boost::shared_ptr<const AnaTrack>& track( *itrack );
    if( track->pt() > zvert_pt ) { 
      zvert_pt = track->pt();
      zvert_z = track->z0();
      zvert_zerr = track->z0_err();
    }
  }
  // 20100228 - override vertex z with "primary vertex z" associated with the jet at ntuple read time. 
  // at present this is the truth z position of the first vertex in the event, but that may change.
  zvert_z = jet->primary_vertex_z();

  // use hadron content to reclassify jet. cross check of quark dr
  // matching method and provides secondary vertex information.
  AnaJet::Hadron b_hadron;
  AnaJet::Hadron light_hadron;
  const AnaJet::ftype b_hadron_dr = jet->min_dr_hadron( AnaJet::Hadron::B_QUARK , b_hadron );
  const AnaJet::ftype light_hadron_dr = jet->min_dr_hadron( AnaJet::Hadron::LIGHT_QUARK , light_hadron );

  if( _dgcoll ) {
    dg::fillh( "jet_et" , 50 , 0 , 50 , jet->et() );
    dg::fillh( "jet_eta" , 160 , -4 , 4 , jet->eta() );
    dg::fillh( "jet_ntracks" , 100 , 0 , 100 , tracks.size() );
    dg::fillh( "jet_dr_b" , 100 , 0 , 1.0 , jet->dr_b_parton() );
    dg::fillh( "jet_dr_c" , 100 , 0 , 1.0 , jet->dr_c_parton() );
    dg::fillh( "jet_dr_light" , 100 , 0 , 1.0 , jet->dr_light_parton() );
    dg::fillh( "jet_dr_bhad" , 100 , 0 , 1.0 , b_hadron_dr );
    dg::fillh( "jet_dr_lighthad" , 100 , 0 , 1.0 , light_hadron_dr );
    dg::fillh( "jet_dr_bhad" , 50 , 0 , 1.0 , 50 , 0 , 1.0 , jet->dr_b_parton() , b_hadron_dr , "B PARTON DR" , "B HADRON DR" );
    dg::fillh( "jet_dr_lighthad" , 50 , 0 , 1.0 , 50 , 0 , 1.0 , jet->dr_light_parton() , light_hadron_dr , "U PARTON DR" , "U HADRON DR" );
    dg::fillh( "jet_dr_any_b" , 100 , 0 , 1.0 , jet->dr_b_parton_any() );
    dg::fillh( "jet_dr_any_c" , 100 , 0 , 1.0 , jet->dr_c_parton_any() );
    dg::fillh( "jet_dr_any_light" , 100 , 0 , 1.0 , jet->dr_light_parton_any() );
  }
  //
  const bool debug = false;
  if( debug ) {
    cout << boost::format(" jet: %|10g|(GeV) %|10g| %|10g| %|6g| %|6g| %|6g|")
      % jet->et() % jet->eta() % jet->phi()
      % jet->dr_b_parton() % jet->dr_c_parton() % jet->dr_light_parton()
         << endl;
  }

  // 20100123
  // const bool ok_dr_b = jet->dr_b_parton() < AnaConfiguration::jet_label_dr_cut();
  // const bool ok_dr_notb = jet->dr_light_parton() < AnaConfiguration::jet_label_dr_cut();
  const bool ok_dr_b = jet->dr_b_parton() < AnaConfiguration::jet_label_dr_cut();
  const bool ok_dr_notb = !ok_dr_b;

  if( _dgcoll ) {
    dg::filleff( "jet_ok_dr_b" , ok_dr_b );
    if( ok_dr_notb ) { dg::filleff( "jet_notb_ok_dr_b" , ok_dr_b ); }
    dg::filleff( "jet_ok_dr_notb" , ok_dr_notb );
    if( ok_dr_b ) { dg::filleff( "jet_b_ok_dr_notb" , ok_dr_notb ); }
    if( b_hadron_dr < AnaConfiguration::jet_label_dr_cut() ) { 
      dg::filleff( "jet_bhad_ok_dr_b" , ok_dr_b ); 
      dg::filleff( "jet_bhad_ok_dr_notb" , ok_dr_notb ); 
    }
    if( light_hadron_dr < AnaConfiguration::jet_label_dr_cut() ) { 
      dg::filleff( "jet_lighthad_ok_dr_b" , ok_dr_b ); 
      dg::filleff( "jet_lighthad_ok_dr_notb" , ok_dr_notb ); 
    }
  }

  // loop over each track and fill the appropriate likelihood pdf.
  double max_signed_d0 = -std::numeric_limits<double>::max();
  double max_signed_d0_signif = -std::numeric_limits<double>::max();
  unsigned int ntracks_signed_d0_pt1 = 0u;
  unsigned int ntracks_signed_d0_pt2 = 0u;
  for( vector< boost::shared_ptr<const AnaTrack> >::const_iterator i=tracks.begin(), f=tracks.end(); i!=f; ++i ) {
    const boost::shared_ptr<const AnaTrack>& track( *i ); assert(track);
    const double d0 = track->d0();
    const double d0_signif = track->d0_signif();
    const double signed_d0 = track->signed_d0( jet_phi );
    const double signed_d0_signif = track->signed_d0_signif_monica( jet_phi );
    const double d0_signif_csc = std::log( 1. + std::abs(d0_signif) );
    const double signed_d0_signif_csc = std::log( 1. + std::abs(signed_d0_signif) );
    // if( track->is_ftk() && (track->chi2prob()<0.05) ) { continue; }
    const double z0c = track->z0c( zvert_z );
    const double z0cL = track->z0cL( zvert_z );
    const double z0cL_signif = track->z0cL_signif( zvert_z , zvert_zerr );
    const double signed_z0cL = track->signed_z0cL( jet_eta , zvert_z );
    const double signed_z0cL_signif = track->signed_z0cL_signif( jet_eta , zvert_z , zvert_zerr );
    //
    bool is_impure = false;
    if( track->is_ftk() ) {
      // CHANGE IN TWO PLACES: training and tagging
      //is_impure = detector::ftkNumLayersMissedPixels(track->ftk_bitmask())>0;
      //is_impure = track->nmisses() > 0;
      // is_impure = ( (track->chi2ndof()>2.) || track->pixel_nhits()<3 );
      // is_impure = (track->pixel_nhits()<3) || (track->chi2prob()<0.1);
      //is_impure = (track->pixel_nhits()<3);

      is_impure = track->chi2prob()<0.5;
      // is_impure = track->pixel_nshared()>=1 || track->sct_nshared()>=2;
    } else if( track->is_offline() ) {
      is_impure = track->pixel_nshared()>=1 || track->sct_nshared()>=2;
    }
    double x = 0.;
    if( _use_sign ) { 
      x = _use_significance ? signed_d0_signif : signed_d0;
    } else {
      x = _use_significance ? d0_signif : d0;
    }
    // fill likelihood pdf
    if( !_use_impure_tracks ) { 
      if( ok_dr_b && !ok_dr_notb ) { 
        _lik_d0_b.fill( x );
      } else if( ok_dr_notb && !ok_dr_b ) { 
        _lik_d0_notb.fill( x );
      }
    } else {
      // divide likelihood into "pure" and "impure" pieces
      if( ok_dr_b && !ok_dr_notb ) { 
        if( is_impure ) { _lik_d0_b_impure.fill( x ); } else { _lik_d0_b.fill( x ); }
      } else if( ok_dr_notb && !ok_dr_b ) { 
        if( is_impure ) { _lik_d0_notb_impure.fill( x ); } else { _lik_d0_notb.fill( x ); }
      }
    }
    if( _use_z0 ) { 
      double xz0 = _use_z0_significance ? signed_z0cL_signif : signed_z0cL;
      if( ok_dr_b && !ok_dr_notb ) { 
        _lik_z0_b.fill( xz0 );
      } else if( ok_dr_notb && !ok_dr_b ) { 
        _lik_z0_notb.fill( xz0 );
      }
    }
    // end fill likelihood pdfs
    if( debug ) {
      cout << ( boost::format(" associated track: %|10s| %|10g| %|10g| %|10g| %|10g| %|10g| %|10g| %|10g|") 
                % track->algorithm().name() % track->d0() % track->d0_signif() % signed_d0 % signed_d0_signif % jet->dr_b_parton() % jet->dr_light_parton() % x 
                )
           << endl;
    }
    if( _dgcoll ) {
      max_signed_d0 = std::max( max_signed_d0 , signed_d0 );
      max_signed_d0_signif = std::max( max_signed_d0_signif , signed_d0_signif );
      if( signed_d0 > 0.1 ) { ++ntracks_signed_d0_pt1; }
      if( signed_d0 > 0.2 ) { ++ntracks_signed_d0_pt2; }
      if( ok_dr_b && !ok_dr_notb ) { dg::down( "bjets" , "b jets" ); }
      else if( ok_dr_notb && !ok_dr_b ) { dg::down( "lightjets" , "light jets" ); }
      else if( ok_dr_b && ok_dr_notb ) { dg::down( "bothjets" , "b and light jets" ); }
      else { dg::down( "unknownjets" , "unknown jets" ); }
      dg::fillh( "d0" , 100 , -1 , 1 , track->d0() , "d_{0}" );
      dg::fillh( "d0_vs_phi" , 32 , -M_PI , M_PI , 100 , -0.5 , 0.5 , track->phi0() , track->d0() , "#phi_{0}" , "d_{0}" );
      dg::fillh( "signed_d0" , 100 , -1 , 1 , signed_d0 , "d_{0}" );
      dg::fillh( "signed_d0_vs_phi" , 32 , -M_PI , M_PI , 100 , -1 , 1 , track->phi0() , signed_d0 , "#phi_{0}" , "signed d_{0}" );
      dg::fillh( "signed_d0_lik" , 100 , -1 , 2.04 , signed_d0 , "signed d_{0}" );
      dg::fillh( "signed_d0_high" , 20 , -5 , 5 , signed_d0 , "signed d_{0}" );
      dg::fillh( "signed_d0_signif" , 100 , -25 , 25 , signed_d0_signif , "signed d_{0} significance" );
      dg::fillh( "signed_d0_signif_high" , 100 , -100 , 100 , signed_d0_signif , "signed d_{0} significance" );
      if( _use_impure_tracks ) {
        if( is_impure ) { dg::fillh( "d0_impure" , 100 , -1 , 1 , track->d0() , "d_{0}" ); }
        else { dg::fillh( "d0_pure" , 100 , -1 , 1 , track->d0() , "d_{0}" ); }
        if( is_impure ) { dg::fillh( "signed_d0_impure" , 100 , -1 , 1 , signed_d0 , "d_{0}" ); }
        else { dg::fillh( "signed_d0_pure" , 100 , -1 , 1 , signed_d0 , "d_{0}" ); }
      }
      dg::up();
    }
  }
  if( _use_sv_variables ) { 
    // compute secondary vertex variables
    AnaVertexFit vf;
    float vertex_mass = -1.;
    float vertex_pt_ratio = -1.;
    unsigned int n_two_track_vertices( 0u );
    const bool ok_vf = vf.compute_sv_variables( tracks.begin() , tracks.end() , vertex_mass , vertex_pt_ratio , n_two_track_vertices );
    if( ok_vf ) {
      if( ok_dr_b && !ok_dr_notb ) { 
        _lik_vertex_mass_b.fill( vertex_mass );
        _lik_pt_ratio_b.fill( vertex_pt_ratio );
        _lik_n_two_track_b.fill( n_two_track_vertices );
      } else {
        _lik_vertex_mass_notb.fill( vertex_mass );
        _lik_pt_ratio_notb.fill( vertex_pt_ratio );
        _lik_n_two_track_notb.fill( n_two_track_vertices );
      }
    }
    if( ok_dr_b && !ok_dr_notb ) { 
      _lik_has_vertex_b.fill( ok_vf );
    }  else {
      _lik_has_vertex_notb.fill( ok_vf );
    }
  }      
  if( _dgcoll ) {
    if( ok_dr_b && !ok_dr_notb ) { dg::down( "bjets" , "b jets" ); }
    else if( ok_dr_notb && !ok_dr_b ) { dg::down( "lightjets" , "light jets" ); }
    else if( ok_dr_b && ok_dr_notb ) { dg::down( "bothjets" , "b and light jets" ); }
    else { dg::down( "unknownjets" , "unknown jets" ); }
    dg::fillh( "max_signed_d0" , 110 , -1 , 10 , max_signed_d0 , "max signed d_{0}" );
    dg::fillh( "max_signed_d0_signif" , 110 , -10 , 100 , max_signed_d0_signif , "max signed d_{0} significance" );
    dg::fillh( "n_signed_d0_pt1" , 50 , 0 , 50 , ntracks_signed_d0_pt1 , "# signed d_{0} p_{T}^{(1)}" );
    dg::fillh( "n_signed_d0_pt2" , 50 , 0 , 50 , ntracks_signed_d0_pt2 , "# signed d_{0} p_{T}^{(2)}");
    dg::up();
  }
  return true;
}

const double
SignedD0JetTagger::_tag( const boost::shared_ptr<const AnaJet>& jet , const std::vector< boost::shared_ptr<const AnaTrack> >& tracks ) const
{
  if( tracks.empty() ) { return -1; }

  const bool debug = true;
  if( debug ) {
    cout << boost::format(" jet: %|10g|(GeV) %|10g| %|10g| %|6g| %|6g| %|6g|")
      % jet->et() % jet->eta() % jet->phi()
      % jet->dr_b_parton() % jet->dr_c_parton() % jet->dr_light_parton()
         << endl;
  }

  vector<AnaTrack::ftype> likb_track;
  vector<AnaTrack::ftype> liknotb_track;

  // get jet centroid eta and phi
  double jet_phi, jet_eta;
  if( _use_track_jet_direction ) { 
    const bool jet_dir_ok = compute_jet_eta_phi( jet , tracks , JetTagger::TRACK_CENTROID , jet_eta , jet_phi );
    if( _dgcoll ) {
      dg::filleff( "jet_dir_ok" , jet_dir_ok );
    }
    if( !jet_dir_ok ) { return false; } // don't use jets with unknown direction for signed d0 training.
  } else {
    jet_eta = jet->eta();
    jet_phi = jet->phi();
  }

  // loop over tracks and identify z vertex as z0 of highest pT track.
  double zvert_pt = 0.;
  double zvert_z = 0.;
  double zvert_zerr = 0.;
  for( vector< boost::shared_ptr<const AnaTrack> >::const_iterator itrack=tracks.begin(), ftrack=tracks.end(); itrack!=ftrack; ++itrack ) {
    const boost::shared_ptr<const AnaTrack>& track( *itrack );
    if( track->pt() > zvert_pt ) { 
      zvert_pt = track->pt();
      zvert_z = track->z0();
      zvert_zerr = track->z0_err();
    }
  }
  // 20100228 - override vertex z with "primary vertex z" associated with the jet at ntuple read time. 
  // at present this is the truth z position of the first vertex in the event, but that may change.
  zvert_z = jet->primary_vertex_z();

  // 
  // 20100123
  // const bool ok_dr_b = jet->dr_b_parton() < AnaConfiguration::jet_label_dr_cut();
  // const bool ok_dr_notb = jet->dr_light_parton() < AnaConfiguration::jet_label_dr_cut();
  const bool ok_dr_b = jet->dr_b_parton() < AnaConfiguration::jet_label_dr_cut();
  const bool ok_dr_notb = !ok_dr_b;

  // loop over each track and construct product likelihood
  double product = 1.;
  double product_b_lik = 1.;
  double product_notb_lik = 1.;
  double sum_b_lik = 1.;
  double sum_notb_lik = 1.;
  unsigned int ntracks = 0u;
  vector<double> xes;

  vector< boost::shared_ptr<const AnaTrack> > tracks_used;
  vector<AnaTrack::ftype> track_b;
  vector<AnaTrack::ftype> track_notb;
  tracks_used.reserve( tracks.size() );
  track_b.reserve( tracks.size() );
  track_notb.reserve( tracks.size() );

  for( vector< boost::shared_ptr<const AnaTrack> >::const_iterator i=tracks.begin(), f=tracks.end(); i!=f; ++i ) {
    const boost::shared_ptr<const AnaTrack>& track( *i ); assert(track);
    const double d0 = track->d0();
    const double d0_signif = track->d0_signif();
    const double signed_d0 = track->signed_d0( jet_phi );
    const double signed_d0_signif = track->signed_d0_signif_monica( jet_phi );
    const double d0_signif_csc = std::log( 1. + std::abs(d0_signif) );
    const double signed_d0_signif_csc = std::log( 1. + std::abs(signed_d0_signif) );
    //if( track->is_ftk() && (track->pixel_nhits()<3) ) { continue; }
    // if( track->is_ftk() && (track->chi2prob()<0.05) ) { continue; }
    const double z0c = track->z0c( zvert_z );
    const double z0cL = track->z0cL( zvert_z );
    const double z0cL_signif = track->z0cL_signif( zvert_z , zvert_zerr );
    const double signed_z0cL = track->signed_z0cL( jet_eta , zvert_z );
    const double signed_z0cL_signif = track->signed_z0cL_signif( jet_eta , zvert_z , zvert_zerr );
    bool is_impure = false;
    if( track->is_ftk() ) {
      // CHANGE IN TWO PLACES: training and tagging
      // is_impure = (track->pixel_nhits()<3) || (track->chi2prob()<0.1);
      // is_impure = (track->pixel_nhits()<3);
      //is_impure = (track->pixel_nhits()<3) || (track->chi2prob()<0.1);
      //is_impure = track->nmisses() > 0;
      // is_impure = detector::ftkNumLayersMissedPixels(track->ftk_bitmask())>0;

      is_impure = track->chi2prob()<0.5;
      // is_impure = track->pixel_nshared()>=1 || track->sct_nshared()>=2;
    } else if( track->is_offline() ) {
      is_impure = track->pixel_nshared()>=1 || track->sct_nshared()>=2;
    }
    double x = 0.;
    if( _use_sign ) { 
      x = _use_significance ? signed_d0_signif : signed_d0;
    } else {
      x = _use_significance ? d0_signif : d0;
    }
    xes.push_back( x );    
    double track_b_lik;
    double track_notb_lik;
    if( !_use_impure_tracks ) { 
      track_b_lik = _lik_d0_b.lik( x );
      track_notb_lik = _lik_d0_notb.lik( x );
    } else {
      if( is_impure ) { 
        track_b_lik = _lik_d0_b_impure.lik( x );
        track_notb_lik = _lik_d0_notb_impure.lik( x );
      } else {
        track_b_lik = _lik_d0_b.lik( x );
        track_notb_lik = _lik_d0_notb.lik( x );
      }
    }
    const double ratio = detector::safe_divide( track_b_lik , track_notb_lik );
    if( debug ) {
      cout << boost::format(" associated track: %|10s| %|10g| %|10g| %|10g| %|10g| %|10g|") % track->algorithm().name() % 
        jet->dr_b_parton() % jet->dr_light_parton() % x % track_b_lik % track_notb_lik 
           << endl;
    }

    tracks_used.push_back( track );
    likb_track.push_back( track_b_lik );
    liknotb_track.push_back( track_notb_lik );
    
    if( track_notb_lik>1E-50 ) { 
      // 20100228 - disable d0 tagger to test z0 and sv1 pieces
      if( _use_d0 ) { 
        product_b_lik *= track_b_lik;
        product_notb_lik *= track_notb_lik;
        product *= ratio;
      }
      ++ntracks;
      sum_b_lik += track_b_lik;
      sum_notb_lik += track_notb_lik;
    }
    if( _use_z0 ) { 
      double xz0 = _use_z0_significance ? signed_z0cL_signif : signed_z0cL;
      const double lik_z0_b = _lik_z0_b.lik( xz0 );
      const double lik_z0_notb = _lik_z0_notb.lik( xz0 );
      if( lik_z0_notb > 1E-50 ) {
        product_b_lik *=  lik_z0_b;
        product_notb_lik *=  lik_z0_notb;
        product *= detector::safe_divide( lik_z0_b , lik_z0_notb );
      }
      dg::fillf("lik_z0_b", 50 , -25 , 25  , xz0 , _lik_z0_b.lik( xz0 ) , "VERTEX MASS (GeV)" , "LIKELIHOOD" );
      dg::fillf("lik_z0_notb", 50 , -25 , 25  , xz0 , _lik_z0_notb.lik( xz0 ) , "VERTEX MASS (GeV)" , "LIKELIHOOD" );
      dg::fillf("lik_z0_ratio", 50 , -25 ,25  , xz0 , 
                detector::safe_log10( detector::safe_divide( _lik_z0_b.lik( xz0 ) , _lik_z0_notb.lik( xz0 ) ) ) , 
                "XZ0 (mm)" , "LOG LIKELIHOOD RATIO" );
    }

    // look at correlation between z0 and d0 likelihoods
    if( _use_z0 ) {
      const double xd0 = _use_significance ? signed_d0_signif : signed_d0;
      const double xz0 = _use_z0_significance ? signed_z0cL_signif : signed_z0cL;
      const double lik_d0_b = _lik_d0_b.lik( xd0 );
      const double lik_d0_notb = _lik_d0_notb.lik( xd0 );
      const double lik_z0_b = _lik_z0_b.lik( xz0 );
      const double lik_z0_notb = _lik_z0_notb.lik( xz0 );
      dg::fillh( "correl_lik_z0_vs<_d0_b" , 100 , 0 , 1 , 100 , 0 , 1, detector::likx(lik_d0_b) , detector::likx(lik_z0_b) , "D0 LIK" , "Z0 LIK" );
      dg::fillh( "correl_lik_z0_vs_d0_notb" , 100 , 0 , 1 , 100 , 0 , 1, detector::likx(lik_d0_notb) , detector::likx(lik_z0_notb) , "D0 LIK" , "Z0 LIK" );
      dg::fillh( "correl_lik_z0_vs_d0_ratio" , 100 , -5 , 5 , 100 , -5 , 5, 
                 detector::safe_log10(detector::likx(detector::safe_divide(lik_d0_b,lik_d0_notb))) , 
                 detector::safe_log10(detector::likx(detector::safe_divide(lik_z0_b,lik_z0_notb)))  , "LOG D0 LIK RATIO" , "LOG Z0 LIK RATIO" );
    }

  } // end for each track associated with the jet

  if( ntracks==0u ) {
    product = 0.;
  }

  if( _use_sv_variables ) { 
    // compute secondary vertex variables
    AnaVertexFit vf;
    float vertex_mass = -1.;
    float vertex_pt_ratio = -1.;
    unsigned int n_two_track_vertices( 0u );
    const bool ok_vf = vf.compute_sv_variables( tracks.begin() , tracks.end() , vertex_mass , vertex_pt_ratio , n_two_track_vertices );
    if( ok_vf ) {
      const double lik_vm_b = _lik_vertex_mass_b.lik( vertex_mass );
      const double lik_vp_b = _lik_pt_ratio_b.lik( vertex_pt_ratio );
      const double lik_ntwo_b = _lik_n_two_track_b.lik( n_two_track_vertices );
      const double lik_vm_notb = _lik_vertex_mass_notb.lik( vertex_mass );
      const double lik_vp_notb = _lik_pt_ratio_notb.lik( vertex_pt_ratio );
      const double lik_ntwo_notb = _lik_n_two_track_notb.lik( n_two_track_vertices );
      {
        dg::fillf("lik_vertex_mass_b", 24 , 0 , 6 , vertex_mass , _lik_vertex_mass_b.lik( vertex_mass ) , "VERTEX MASS (GeV)" , "LIKELIHOOD" );
        dg::fillf("lik_vertex_mass_notb", 24 , 0 , 6 , vertex_mass , _lik_vertex_mass_notb.lik( vertex_mass ) , "VERTEX MASS (GeV)" , "LIKELIHOOD" );
        dg::fillf("lik_vertex_mass_ratio", 24 , 0 , 6 , vertex_mass , 
                  detector::safe_log10( detector::safe_divide( _lik_vertex_mass_b.lik( vertex_mass ) , _lik_vertex_mass_notb.lik( vertex_mass ) ) ) , 
                  "VERTEX MASS (GeV)" , "LOG LIKELIHOOD RATIO" );
        dg::fillf("lik_pt_ratio_b", 10 , 0 , 1 , vertex_pt_ratio , _lik_pt_ratio_b.lik( vertex_pt_ratio ) , "VERTEX MASS (GeV)" , "LIKELIHOOD" );
        dg::fillf("lik_pt_ratio_notb", 10 , 0 , 1 , vertex_pt_ratio , _lik_pt_ratio_notb.lik( vertex_pt_ratio ) , "VERTEX MASS (GeV)" , "LIKELIHOOD" );
        dg::fillf("lik_pt_ratio_ratio", 10 , 0 , 1 , vertex_pt_ratio , 
                  detector::safe_log10( detector::safe_divide( _lik_pt_ratio_b.lik( vertex_pt_ratio ) , _lik_pt_ratio_notb.lik( vertex_pt_ratio ) ) ) , 
                  "PT RATIO" , "LOG LIKELIHOOD RATIO" );
        dg::fillf("lik_n_two_track_b", 50 , 0 , 50 , n_two_track_vertices , _lik_n_two_track_b.lik( n_two_track_vertices ) , "N TWO TRACKS" , "LIKELIHOOD" );
        dg::fillf("lik_n_two_track_notb", 50 , 0 , 50 , n_two_track_vertices , _lik_n_two_track_notb.lik( n_two_track_vertices ) , "N TWO TRACKS" , "LIKELIHOOD" );
        dg::fillf("lik_n_two_track_ratio", 50 , 0 , 50 , n_two_track_vertices , 
                  detector::safe_log10( detector::safe_divide( _lik_n_two_track_b.lik( n_two_track_vertices ) , _lik_n_two_track_notb.lik( n_two_track_vertices ) ) ) , 
                  "N TWO TRACKS" , "LOG LIKELIHOOD RATIO" );
      }
      if( lik_vm_notb>1E-50 ) { 
        product_b_lik *= lik_vm_b;
        product_notb_lik *= lik_vm_notb;
        product *= detector::safe_divide( lik_vm_b , lik_vm_notb );
      }
      if( lik_vp_notb>1E-50 ) { 
        product_b_lik *= lik_vp_b;
        product_notb_lik *= lik_vp_notb;
        product *= detector::safe_divide( lik_vp_b , lik_vp_notb );
      }
      if( lik_ntwo_notb>1E-50 ) { 
        product_b_lik *= lik_ntwo_b;
        product_notb_lik *= lik_ntwo_notb;
        product *= detector::safe_divide( lik_ntwo_b , lik_ntwo_notb );
      }
    }
    const double lik_has_b = _lik_has_vertex_b.lik( ok_vf );
    const double lik_has_notb = _lik_has_vertex_notb.lik( ok_vf );
    if( lik_has_notb>1E-50 ) { 
      product_b_lik *= lik_has_b;
      product_notb_lik *= lik_has_notb;
      product *= detector::safe_divide( lik_has_b , lik_has_notb );
    }
  }

  const double likx = detector::likx(product);

  JetTrackTag tag( jet , product , jet->eta() , jet_phi , tracks_used , likb_track , liknotb_track );

  StudyJetTag study( "tags" , "tagging study" );
  study.add_constant_category( "bjets" , ok_dr_b && !ok_dr_notb );
  study.add_constant_category( "lightjets" , ok_dr_notb && !ok_dr_b );
  study.add_constant_category( "bothjets" , ok_dr_notb && ok_dr_b );
  study.add_constant_category( "unmatchedjets" , !ok_dr_notb && !ok_dr_b );
  study.add_constant_category( "lightjets_mistagged" , ok_dr_notb && !ok_dr_b && (likx>0.6) );
  study.add_constant_category( "lightjets_middle" , ok_dr_notb && !ok_dr_b && (likx>0.35) && (likx<0.65) );
  study.add_constant_category( "lightjets_hump" , ok_dr_notb && !ok_dr_b && (likx>0.1) && (likx<0.35) );
  study.add_constant_category( "bjets_mistagged" , ok_dr_b && !ok_dr_notb && (likx<0.5) );
  study.for_only( tag );

  if( debug ) {
    cout << boost::format(" tag result: %|10g| %|10g| %|10g| %|10g| %|10g| %|10g|") % 
      product_b_lik % product_notb_lik % sum_b_lik % sum_notb_lik % product % likx
         << endl;
  }

  // study classification error
  // sort( xes.begin() , xes.end() , greater<double>() );
  // if( false ) {
  //   typedef pair<bool,string> ClassCat;
  //   vector< ClassCat > classes;
  //   // study light jets misclassified as heavy
  //   classes.push_back( ClassCat((product/(1.+product))>0.725 && ok_dr_notb && !ok_dr_b,"light_as_heavy" ) );
  //   classes.push_back( ClassCat((product/(1.+product))<0.25 && ok_dr_notb && !ok_dr_b,"light_as_light" ) );
  //   // study heavy jets misclassified as light
  //   classes.push_back( ClassCat((product/(1.+product))<0.25 && ok_dr_b && !ok_dr_notb,"heavy_as_light" ) );
  //   classes.push_back( ClassCat((product/(1.+product))>0.75 && ok_dr_b && !ok_dr_notb,"heavy_as_heavy" ) );
  //   // do classification error studies
  //   const double lowquat = xmin + (xmax-xmin)*0.25;
  //   const double hiquat = xmin + (xmax-xmin)*0.75;
  //   BOOST_FOREACH( ClassCat cat , classes ) {
  //     if( !cat.first ) { continue; }
  //     dg::down( cat.second , "classification error studies" ); BOOST_SCOPE_EXIT(void) { dg::up(); } BOOST_SCOPE_EXIT_END;
  //     double max_x = -9999;
  //     double min_x = 9999;
  //     unsigned int nlowquat = 0u;
  //     unsigned int nhiquat = 0u;
  //     unsigned int nneg = 0u;
  //     BOOST_FOREACH( double x , xes ) {
  //       dg::fillh( "x" , nbinsx , xmin , xmax , x );
  //       min_x = std::min( min_x , x );
  //       max_x = std::max( max_x , x );
  //       if( x<lowquat ) { ++nlowquat; }
  //       if( x>hiquat ) { ++nhiquat; }
  //       if( x<0 ) { ++nneg; }
  //     }
  //     dg::fillh( "n" , 20 , 0 , 20 , xes.size() );
  //     dg::fillh( "nlow" , 20 , 0 , 20 , nlowquat );
  //     dg::fillh( "nhi" , 20 , 0 , 20 , nhiquat );
  //     dg::fillh( "nmid" , 20 , 0 , 20 , xes.size() - nlowquat - nhiquat );
  //     dg::fillh( "nneg" , 20 , 0 , 20 , nneg );
  //     dg::fillh( "npos" , 20 , 0 , 20 , xes.size() - nneg );
  //     dg::fillh( "flow" , 20 , 0 , 20 , nlowquat / static_cast<double>(xes.size()) );
  //     dg::fillh( "fhi" , 20 , 0 , 20 , nhiquat / static_cast<double>(xes.size()) );
  //     dg::fillh( "fmid" , 20 , 0 , 20 , (xes.size() - nlowquat - nhiquat)/static_cast<double>(xes.size()) );
  //     dg::fillh( "fneg" , 20 , 0 , 20 , nneg / static_cast<double>(xes.size()) );
  //     dg::fillh( "fpos" , 20 , 0 , 20 , (xes.size() - nneg)/static_cast<double>(xes.size()) );
  //     dg::fillh( "xmin" , nbinsx , xmin , xmax , min_x );
  //     dg::fillh( "xmax" , nbinsx , xmin , xmax , max_x );
  //     StudyTrack study_track( "tracks" , "plots of associated tracks" );
  //     study_track.for_each( tracks.begin() , tracks.end() );
  //     unsigned int n1 = 0u;
  //     unsigned int n2 = 0u;
  //     unsigned int n3 = 0u;
  //     unsigned int n4 = 0u;
  //     unsigned int m1 = 0u;
  //     unsigned int m2 = 0u;
  //     unsigned int m3 = 0u;
  //     unsigned int m4 = 0u;
  //     unsigned int o1 = 0u;
  //     unsigned int o2 = 0u;
  //     unsigned int o3 = 0u;
  //     unsigned int o4 = 0u;
  //     double sumpt = 0.;
  //     BOOST_FOREACH( const boost::shared_ptr<const AnaTrack>& track , tracks ) {
  //       const double deta = detector::delta_eta( track->eta() , jet->eta() );
  //       const double dphi = detector::delta_phi( track->phi0() , jet->phi() );
  //       const double dr = detector::quadrature( deta , dphi );
  //       if( std::abs(deta) < 0.1 ) { ++m1; }
  //       if( std::abs(deta) < 0.2 ) { ++m2; }
  //       if( std::abs(deta) < 0.3 ) { ++m3; }
  //       if( std::abs(deta) < 0.4 ) { ++m4; }
  //       if( std::abs(dphi) < 0.1 ) { ++o1; }
  //       if( std::abs(dphi) < 0.2 ) { ++o2; }
  //       if( std::abs(dphi) < 0.3 ) { ++o3; }
  //       if( std::abs(dphi) < 0.4 ) { ++o4; }
  //       if( dr < 0.1 ) { ++n1; }
  //       if( dr < 0.2 ) { ++n2; }
  //       if( dr < 0.3 ) { ++n3; }
  //       if( dr < 0.4 ) { ++n4; }
  //       sumpt += track->pt();
  //     }
  //     dg::fillf( "ninc_vs_jet_dr" , 10 , 0 , 0.4 , 0.1 , n1 , "jet_dr" , "N Tracks Inside" );
  //     dg::fillf( "ninc_vs_jet_dr" , 10 , 0 , 0.4 , 0.2 , n2 , "jet_dr" , "N Tracks Inside" );
  //     dg::fillf( "ninc_vs_jet_dr" , 10 , 0 , 0.4 , 0.3 , n3 , "jet_dr" , "N Tracks Inside" );
  //     dg::fillf( "ninc_vs_jet_dr" , 10 , 0 , 0.4 , 0.4 , n4 , "jet_dr" , "N Tracks Inside" );
  //     dg::fillf( "ninc_vs_jet_deta" , 10 , 0 , 0.4 , 0.1 , m1 , "jet_deta" , "N Tracks Inside" );
  //     dg::fillf( "ninc_vs_jet_deta" , 10 , 0 , 0.4 , 0.2 , m2 , "jet_deta" , "N Tracks Inside" );
  //     dg::fillf( "ninc_vs_jet_deta" , 10 , 0 , 0.4 , 0.3 , m3 , "jet_deta" , "N Tracks Inside" );
  //     dg::fillf( "ninc_vs_jet_deta" , 10 , 0 , 0.4 , 0.4 , m4 , "jet_deta" , "N Tracks Inside" );
  //     dg::fillf( "ninc_vs_jet_dphi" , 10 , 0 , 0.4 , 0.1 , o1 , "jet_dphi" , "N Tracks Inside" );
  //     dg::fillf( "ninc_vs_jet_dphi" , 10 , 0 , 0.4 , 0.2 , o2 , "jet_dphi" , "N Tracks Inside" );
  //     dg::fillf( "ninc_vs_jet_dphi" , 10 , 0 , 0.4 , 0.3 , o3 , "jet_dphi" , "N Tracks Inside" );
  //     dg::fillf( "ninc_vs_jet_dphi" , 10 , 0 , 0.4 , 0.4 , o4 , "jet_dphi" , "N Tracks Inside" );

  //     dg::fillf( "finc_vs_jet_dr" , 10 , 0 , 0.4 , 0.1 , n1/static_cast<double>(xes.size()) , "jet_dr" , "Fraction of Tracks Inside" );
  //     dg::fillf( "finc_vs_jet_dr" , 10 , 0 , 0.4 , 0.2 , n2/static_cast<double>(xes.size()) , "jet_dr" , "Fraction of Tracks Inside" );
  //     dg::fillf( "finc_vs_jet_dr" , 10 , 0 , 0.4 , 0.3 , n3/static_cast<double>(xes.size()) , "jet_dr" , "Fraction of Tracks Inside" );
  //     dg::fillf( "finc_vs_jet_dr" , 10 , 0 , 0.4 , 0.4 , n4/static_cast<double>(xes.size()) , "jet_dr" , "Fraction of Tracks Inside" );
  //     dg::fillf( "finc_vs_jet_deta" , 10 , 0 , 0.4 , 0.1 , m1/static_cast<double>(xes.size()) , "jet_deta" , "Fraction of Tracks Inside" );
  //     dg::fillf( "finc_vs_jet_deta" , 10 , 0 , 0.4 , 0.2 , m2/static_cast<double>(xes.size()) , "jet_deta" , "Fraction of Tracks Inside" );
  //     dg::fillf( "finc_vs_jet_deta" , 10 , 0 , 0.4 , 0.3 , m3/static_cast<double>(xes.size()) , "jet_deta" , "Fraction of Tracks Inside" );
  //     dg::fillf( "finc_vs_jet_deta" , 10 , 0 , 0.4 , 0.4 , m4/static_cast<double>(xes.size()) , "jet_deta" , "Fraction of Tracks Inside" );
  //     dg::fillf( "finc_vs_jet_dphi" , 10 , 0 , 0.4 , 0.1 , o1/static_cast<double>(xes.size()) , "jet_dphi" , "Fraction of Tracks Inside" );
  //     dg::fillf( "finc_vs_jet_dphi" , 10 , 0 , 0.4 , 0.2 , o2/static_cast<double>(xes.size()) , "jet_dphi" , "Fraction of Tracks Inside" );
  //     dg::fillf( "finc_vs_jet_dphi" , 10 , 0 , 0.4 , 0.3 , o3/static_cast<double>(xes.size()) , "jet_dphi" , "Fraction of Tracks Inside" );
  //     dg::fillf( "finc_vs_jet_dphi" , 10 , 0 , 0.4 , 0.4 , o4/static_cast<double>(xes.size()) , "jet_dphi" , "Fraction of Tracks Inside" );

  //     dg::fillh( "sumpt" , 100 , 0 , 50 , sumpt , " Sum p_{T} (GeV)" );
  //   }
  // }

  //
  // 
  // debug
//   if( false && ((product/(1.+product))>0.85 || (product/(1.+product))<0.1) ) {
// //     cout << (true_b ? "is_b" : "is_notb") << " " << (( (product/(1.+product))>0.5 ) ? "pass_b" : "pass_notb") << " track dump: " << endl;
//     double b_product = 1.;
//     double notb_product = 1.;
//     double b_sum = 0.;
//     double notb_sum = 0.;
//     unsigned int ntracks = 0u;
//     for( vector< boost::shared_ptr<const AnaTrack> >::const_iterator i=tracks.begin(), f=tracks.end(); i!=f; ++i ) {
//       const boost::shared_ptr<const AnaTrack>& track( *i ); assert(track);
//       //if( track->alg()<5 ) { continue; }
//       const double d0 = track->d0();
//       const double d0_signif = track->d0_signif();
//       const double signed_d0 = track->signed_d0( jet_phi );
//       const double signed_d0_signif = track->signed_d0_signif_monica( jet_phi );
//       const double d0_signif_csc = std::log( 1 + std::abs(d0_signif) );
//       const double signed_d0_signif_csc = std::log( 1 + std::abs(signed_d0_signif) );
//       double x = 0.;
//       if( _use_sign ) { 
//         x = _use_significance ? signed_d0_signif : signed_d0;
//       } else {
//         x = _use_significance ? d0_signif : d0;
//       }
//       double track_b_lik = _lik_d0_b.lik( x );
//       const double track_notb_lik = _lik_d0_notb.lik( x );
//       //if( track_b_lik<(0.05*tracks.size()) ) { track_b_lik = 0; }
//       if( track_notb_lik<1E-50 ) { continue; }
//       b_product *= track_b_lik;
//       notb_product *= track_notb_lik;
//       b_sum += track_b_lik;
//       notb_sum += track_notb_lik;
//       ++ntracks;
//       cout << " track: "
//            << setw(4) << track->algorithm().name() << " " 
//            << setw(8) << track->charge() << " " 
//            << setw(8) << track->phi0() << " "  
//            << setw(8) << track->eta() << " " 
//            << setw(12) << track->pt() << " " 
//            << setw(8) << track->d0() << " " 
//            << setw(8) << track->d0_err() << " " 
//            << setw(8) << track->d0_signif() << " " 
//            << setw(8) << signed_d0 << " " 
//            << setw(8) << x << " z0: "
//            << setw(8) << track->z0() << " "
//            << setw(8) << track->z0_err() << " "
//            << setw(8) << track->z0_signif() << " chi2:"
//            << setw(8) << track->chi2() << " "
//            << setw(4) << track->nhits() << " "
//            << setw(4) << track->ndof() << " "
//            << setw(4) << track->nmisses() << " "
//            << setw(8) << track->chi2ndof() << " "
//            << setw(8) << track->chi2prob() << " lik: "
//            << setw(8) << track_b_lik << " "
//            << setw(8) << track_notb_lik << " "
//            << setw(8) << (track_b_lik/track_notb_lik) << " "
//            << endl;
//     }
//     if( ntracks>0u ) { 
//       b_sum /= ntracks;
//       notb_sum /= ntracks;
//       cout << " product: " << product << " " << b_product << " " << notb_product << " "
//            << b_sum << " " << notb_sum << " " << (product/(1.+product)) << endl;
//     }
//   }
  //
  //if( do_plots ) { dg::up(); }

  return product;
}



const bool
SignedD0JetTagger::save( const std::string& filename )
{
  const bool b_ok = _lik_d0_b.save( (filename+"_b.dat").c_str() );
  const bool u_ok = _lik_d0_notb.save( (filename+"_notb.dat").c_str() );
  return b_ok && u_ok;
}

const bool
SignedD0JetTagger::load( const std::string& filename )
{
  const bool b_ok = _lik_d0_b.load( (filename+"_b.dat").c_str() );
  const bool u_ok = _lik_d0_notb.load( (filename+"_notb.dat").c_str() );
  return b_ok && u_ok;
}

