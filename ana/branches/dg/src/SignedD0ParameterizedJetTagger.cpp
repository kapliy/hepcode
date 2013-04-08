
#include <iostream>
#include <iomanip>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/format.hpp>
#include <TH1.h>
#include "TrigFTKAna/AnaJet.hpp"
#include "TrigFTKAna/AnaTrack.hpp"
#include "TrigFTKAna/SignedD0ParameterizedJetTagger.hpp"
#include "TrigFTKAna/SignedD0JetTagger.hpp"
#include "DataGraphics/DgCollection.hh"

using namespace std;
using namespace DataGraphics;

void
SignedD0ParameterizedJetTagger::fit( const SignedD0JetTagger& tagger )
{
  _lik_d0_b.fit( LikelihoodParameterization1D::LONG_LIVED , dynamic_cast<const TH1*>(tagger.b_d0_likelihood().th1()) , _name+"_b_lik" );
  _lik_d0_notb.fit( LikelihoodParameterization1D::PROMPT , dynamic_cast<const TH1*>(tagger.notb_d0_likelihood().th1()) , _name+"_notb_lik" );
}

SignedD0ParameterizedJetTagger::SignedD0ParameterizedJetTagger() 
  : JetTagger()
  , _use_significance( false )
  , _use_sign( true )
  , _use_track_jet_direction( true )
{
}

SignedD0ParameterizedJetTagger::SignedD0ParameterizedJetTagger( DgCollection* dgcoll )
  : JetTagger( dgcoll )
  , _use_significance( false )
  , _use_sign( true )
  , _use_track_jet_direction( true )
{
}

SignedD0ParameterizedJetTagger*
SignedD0ParameterizedJetTagger::use_significance( const bool& yes ) { 
  _use_significance = yes; 
  return this; 
}

SignedD0ParameterizedJetTagger*
SignedD0ParameterizedJetTagger::use_sign( const bool& yes ) { 
  _use_sign = yes;
  return this;
}

SignedD0ParameterizedJetTagger*
SignedD0ParameterizedJetTagger::use_track_jet_direction( const bool& yes ) { 
  _use_track_jet_direction = yes;
  _jet_centroid_option = JetTagger::TRACK_CENTROID;
  return this;
}

const bool
SignedD0ParameterizedJetTagger::_train( const boost::shared_ptr<const AnaJet>& jet , const std::vector< boost::shared_ptr<const AnaTrack> >& tracks )
{
  return false;
}

const double
SignedD0ParameterizedJetTagger::_tag( const boost::shared_ptr<const AnaJet>& jet , const std::vector< boost::shared_ptr<const AnaTrack> >& tracks ) const
{
  if( tracks.empty() ) { return -1; }
  cout << " tagger: " << name() << endl;
  const double drcut = 0.1;
  // use trained signed d0 pdfs to construct product likelihoods
  //
  // get jet centroid phi
  double jet_phi;
  if( _use_track_jet_direction ) { 
    const bool jet_dir_ok = compute_jet_phi( jet , tracks , JetTagger::TRACK_CENTROID , jet_phi );
    if( _dgcoll ) {
      dg::filleff( "jet_dir_ok" , jet_dir_ok );
    }
    if( !jet_dir_ok ) { return 0.; }
  } else {
    jet_phi = jet->phi();
  }
  // 
  if( _dgcoll ) {
    if( jet->dr_b_parton() < drcut ) { dg::down( "bjets" , "b jets" ); }
    else if( jet->dr_light_parton() < drcut ) { dg::down( "lightjets" , "light jets" ); }
    else { dg::down( "unknownjets" , "unknown jets" ); }
  }
  // 
  if( _dgcoll ) {
    dg::fillh( "jet_et" , 50 , 0 , 50 , jet->et() , "jet et" );
    dg::fillh( "jet_eta" , 40 , -4 , 4 , jet->eta() , "jet eta" );
    dg::fillh( "jet_ntracks" , 100 , 0 , 100 , tracks.size() , "number of tracks associated with jet" );
    dg::fillh( "jet_dr_b" , 100 , 0 , 0.5 , jet->dr_b_parton() );
    dg::fillh( "jet_dr_c" , 100 , 0 , 0.5 , jet->dr_c_parton() );
    dg::fillh( "jet_dr_light" , 100 , 0 , 0.5 , jet->dr_light_parton() );
  }
  //
  const bool debug = true;
  if( debug ) {
    cout << boost::format(" jet: %|10g|(GeV) %|10g| %|10g| %|6g| %|6g| %|6g|")
      % jet->et() % jet->eta() % jet->phi()
      % jet->dr_b_parton() % jet->dr_c_parton() % jet->dr_light_parton()
         << endl;
  }
  // loop over each track and construct product likelihood
  double product = 1.;
  double product_b_lik = 1.;
  double product_notb_lik = 1.;
  double sum_b_lik = 1.;
  double sum_notb_lik = 1.;
  unsigned int ntracks = 0u;
  for( vector< boost::shared_ptr<const AnaTrack> >::const_iterator i=tracks.begin(), f=tracks.end(); i!=f; ++i ) {
    const boost::shared_ptr<const AnaTrack>& track( *i ); assert(track);
    const double d0 = track->d0();
    const double d0_signif = track->d0_signif();
    const double signed_d0 = track->signed_d0( jet_phi );
    const double signed_d0_signif = track->signed_d0_signif_monica( jet_phi );
    const double d0_signif_csc = std::log( 1. + std::abs(d0_signif) );
    const double signed_d0_signif_csc = std::log( 1. + std::abs(signed_d0_signif) );
    double x = 0.;
    if( _use_sign ) { 
      x = _use_significance ? signed_d0_signif : signed_d0;
    } else {
      x = _use_significance ? d0_signif : d0;
    }
    double track_b_lik = _lik_d0_b.lik( x );
    double track_notb_lik = _lik_d0_notb.lik( x );
    const double track_lik_r = detector::safe_divide( track_b_lik , track_notb_lik );
    if( debug ) {
      cout << boost::format(" associated track: %|10s| %|10g| %|10g| %|10g| %|10g| %|10g| likr %|10g|") % track->algorithm().name() % 
        jet->dr_b_parton() % jet->dr_light_parton() % x % track_b_lik % track_notb_lik % track_lik_r 
           << endl;
    }
    if( _dgcoll ) {
      dg::down( "tracks" , "per track tagger results" );
      dg::fillh( "d0" , 100 , -1 , 1 , track->d0() , "d_{0}" );
      dg::fillh( "pt" , 100 , 0 , 20 , track->pt() , "p_{T}" );
      dg::fillh( "signed_d0" , 100 , -1 , 1 , signed_d0 , "signed d_{0}" );
      dg::fillh( "signed_d0_high" , 20 , -5 , 5 , signed_d0 , "signed d_{0}" );
      dg::fillh( "signed_d0_signif" , 50 , -25 , 25 , signed_d0_signif , "signed d_{0} significance" );
      dg::fillh( "b_classifier" , 100 , 0 , 1 , track_b_lik , "track b likelihood" );
      dg::fillh( "b_classifier_x" , 100 , 0 , 1 , track_b_lik/(track_b_lik+1.) , "monica x for track b" );
      dg::fillh( "notb_classifier" , 100 , 0 , 1 , track_notb_lik , "track !b likelihood" );
      dg::fillh( "notb_classifier_x" , 100 , 0 , 1 , track_notb_lik/(track_notb_lik+1.) , "monica x for track !b" );
      dg::fillh( "b_classifier_x_vs_signed_d0" , 40 , -1 , 1 , 25 , 0 , 1 , signed_d0 , track_b_lik/(1.+track_b_lik) , "signed d_{0}" , "monica x for track b" );
      dg::fillh( "notb_classifier_x_vs_signed_d0" , 40 , -1 , 1 , 25 , 0 , 1 , signed_d0 , track_notb_lik/(1.+track_notb_lik) , "signed d_{0}" , "monica x for track !b" );
      dg::fillh( "classifier_r_vs_signed_d0" , 100 , -2 , 2 , 100 , -10 , 10 , signed_d0 , detector::safe_log10(track_lik_r) , "signed d_{0}" , "log10 ratio of likelihoods" );
      if( std::abs( track_b_lik/(track_b_lik+1.) ) < 0.01 ) {
        dg::fillh( "signed_d0_low_b_lik" , 500 , -5 , 5 , signed_d0 , "signed d_{0}" );
      }
      if( std::abs( track_notb_lik/(track_notb_lik+1.) ) < 0.01 ) {
        dg::fillh( "signed_d0_low_notb_lik" , 500 , -5 , 5 , signed_d0 , "signed d_{0}"  );
      }
      if( std::abs( track_b_lik/(track_b_lik+1.) ) > 0.75 ) { 
        dg::fillh( "signed_d0_high_b_lik" , 500 , -5 , 5 , signed_d0 , "signed d_{0}"  );
      }
      if( std::abs( track_notb_lik/(track_notb_lik+1.) ) > 0.75 ) {
        dg::fillh( "signed_d0_high_notb_lik" , 500 , -5 , 5 , signed_d0 , "signed d_{0}"  );
      }
      // pt-dependent resolution measurement
      if( jet->dr_light_parton() < drcut ) { 
        dg::down( "resolution_measurement" , "pt-dependent resolution measurement" );
        dg::fillh( "d0" , 100 , -0.5 , 0.5 , track->d0() , "track d_{0}" );
        dg::fillh( "d0_err" , 100 , 0. , 1 , track->d0_err() , "track d_{0} error" );
        dg::fillh( "d0_err_m" , 100 , 0. , 1 , track->monica_d0_err() , "track d_{0} error, Monica's parameterization" );
        dg::fillh( "d0_vs_pt" , 100 , 0 , 50 , 100 , -0.5 , 0.5 , track->pt() , track->d0() , "p_{T}" , "d_{0}" );
        dg::fillh( "d0_neg_vs_pt" , 100 , 0 , 50 , 100 , -0.5 , 0. , track->pt() , track->d0() , "p_{T}" , "d_{0}" );
        dg::fillh( "signed_d0_vs_pt" , 100 , 0 , 50 , 100 , -0.5 , 0.5 , track->pt() , signed_d0 , "p_{T}" , "signed d_{0}" );
        dg::fillh( "signed_d0_neg_vs_pt" , 100 , 0 , 50 , 100 , -0.5 , 0. , track->pt() , signed_d0 , "p_{T}" , "signed d_{0}" );
        if( signed_d0 < 0. ) { 
          dg::fillh( "abs_signed_d0_neg" , 100 , 0. , 0.5 , std::abs(signed_d0) );
          if( track->pt() < 2 ) { dg::fillh( "abs_signed_d0_neg_pt2gev" , 100 , 0. , 0.5 , std::abs(signed_d0) ); }
          if( track->pt() >= 2 && track->pt() <= 4 ) { dg::fillh( "abs_signed_d0_neg_pt2_4gev" , 100 , 0. , 0.5 , std::abs(signed_d0) , "|signed d_{0}|" ); }
          if( track->pt() >= 4 && track->pt() <= 6 ) { dg::fillh( "abs_signed_d0_neg_pt4_6gev" , 100 , 0. , 0.5 , std::abs(signed_d0) , "|signed d_{0}|" ); }
          if( track->pt() >= 6 && track->pt() <= 8 ) { dg::fillh( "abs_signed_d0_neg_pt6_8gev" , 100 , 0. , 0.5 , std::abs(signed_d0) , "|signed d_{0}|" ); }
          if( track->pt() >= 8 && track->pt() <= 10 ) { dg::fillh( "abs_signed_d0_neg_pt8_10gev" , 100 , 0. , 0.5 , std::abs(signed_d0) , "|signed d_{0}|" ); }
          if( track->pt() >= 10 && track->pt() <= 15 ) { dg::fillh( "abs_signed_d0_neg_pt10_15gev" , 100 , 0. , 0.5 , std::abs(signed_d0) , "|signed d_{0}|" ); }
          if( track->pt() >= 15 && track->pt() <= 20 ) { dg::fillh( "abs_signed_d0_neg_pt15_20gev" , 100 , 0. , 0.5 , std::abs(signed_d0) , "|signed d_{0}|" ); }
          if( track->pt() >= 20 && track->pt() <= 40 ) { dg::fillh( "abs_signed_d0_neg_pt20_40gev" , 100 , 0. , 0.5 , std::abs(signed_d0) , "|signed d_{0}|" ); }
          if( track->pt() >= 40 ) { dg::fillh( "abs_signed_d0_neg_pt40gev" , 100 , 0. , 0.5 , std::abs(signed_d0) , "|signed d_{0}|" ); }
        }
        if( signed_d0 > 0. ) { 
          dg::fillh( "abs_signed_d0_pos" , 100 , 0. , 0.5 , std::abs(signed_d0) , "|signed d_{0|}" ); 
        }
        dg::up();
      }
      dg::up();
    }
    //if( track_b_lik<(0.05*tracks.size()) ) { track_b_lik = 0; }
    if( track_notb_lik<1E-50 ) { continue; }
    //track_notb_lik = std::max( track_notb_lik , 0.005 );
    product_b_lik *= track_b_lik;
    product_notb_lik *= track_notb_lik;
    const double ratio = track_b_lik/track_notb_lik;
    product *= ratio;
    ++ntracks;
    sum_b_lik += track_b_lik;
    sum_notb_lik += track_notb_lik;
    if( _dgcoll ) {
      dg::down( "tracks" , "per track tagger results" );
      dg::fillh( "r_classifier" , 100 , -10 , 10 , ratio , "r classifier" );
      dg::fillh( "r_classifier_x" , 100 , 0 , 1 , ratio/(1.+ratio) , "r_classifier_x" );
      dg::fillh( "r_classifier_vs_signed_d0" , 10 , -1 , 1 , 50 , -10 , 10 , signed_d0 , ratio , "signed_d0" , "r_classifier" );
      dg::fillh( "r_classifier_x_vs_signed_d0" , 10 , -1 , 1 , 100 , 0 , 1 , signed_d0 , ratio/(1.+ratio) , "signed_d0" , "r_classifier_x" );
      if( std::abs(ratio/(1.+ratio))<0.01 ) { dg::fillh( "signed_d0_low_r_x" , 500 , -5 , 5 , signed_d0 , "signed_d0_low_r_x" ); }
      if( std::abs(ratio/(1.+ratio))>0.75 ) { dg::fillh( "signed_d0_high_r_x" , 500 , -5 , 5 , signed_d0 , "signed_d0_high_r_x" ); }
      dg::up();
    }
  }

  if( debug ) {
    cout << boost::format(" tag result: %|10g| %|10g| %|10g| %|10g| %|10g| %|10g|") % 
      product_b_lik % product_notb_lik % sum_b_lik % sum_notb_lik % product % (product/(1.+product))
         << endl;
  }

  //   if( ntracks>0u && sum_notb_lik >0. ) {
  //     sum_b_lik /= ntracks;
  //     sum_notb_lik /= ntracks;
  //     product *= sum_b_lik/sum_notb_lik;
  //   }
  if( ntracks==0u ) {
    product = 0.;
  }

  if( _dgcoll ) {
    dg::down( "jets" , "per jet tagger results" );
    dg::fillh( "r_classifier" , 100 , -10 , 10 , product , "r_classifier" );
    dg::fillh( "r_classifier_x" , 100 , 0 , 1 , product/(1.+product) , "r_classifier_x" );
    dg::fillh( "r_classifier_x_monica" , 15 , 0 , 1 , product/(1.+product) , "r_classifier_x_monica" );
    dg::up();
  }
  //
  if( _dgcoll ) { dg::up(); }
  // 
  // debug
  if( false && ((product/(1.+product))>0.85 || (product/(1.+product))<0.1) ) {
//     cout << (true_b ? "is_b" : "is_notb") << " " << (( (product/(1.+product))>0.5 ) ? "pass_b" : "pass_notb") << " track dump: " << endl;
    double b_product = 1.;
    double notb_product = 1.;
    double b_sum = 0.;
    double notb_sum = 0.;
    unsigned int ntracks = 0u;
    for( vector< boost::shared_ptr<const AnaTrack> >::const_iterator i=tracks.begin(), f=tracks.end(); i!=f; ++i ) {
      const boost::shared_ptr<const AnaTrack>& track( *i ); assert(track);
      //if( track->alg()<5 ) { continue; }
      const double d0 = track->d0();
      const double d0_signif = track->d0_signif();
      const double signed_d0 = track->signed_d0( jet_phi );
      const double signed_d0_signif = track->signed_d0_signif_monica( jet_phi );
      const double d0_signif_csc = std::log( 1 + std::abs(d0_signif) );
      const double signed_d0_signif_csc = std::log( 1 + std::abs(signed_d0_signif) );
      double x = 0.;
      if( _use_sign ) { 
        x = _use_significance ? signed_d0_signif : signed_d0;
      } else {
        x = _use_significance ? d0_signif : d0;
      }
      double track_b_lik = _lik_d0_b.lik( x );
      const double track_notb_lik = _lik_d0_notb.lik( x );
      //if( track_notb_lik<1E-20 ) { continue; }
      b_product *= track_b_lik;
      notb_product *= track_notb_lik;
      b_sum += track_b_lik;
      notb_sum += track_notb_lik;
      ++ntracks;
      cout << " track: "
           << setw(4) << track->algorithm().name() << " " 
           << setw(8) << track->charge() << " " 
           << setw(8) << track->phi0() << " "  
           << setw(8) << track->eta() << " " 
           << setw(12) << track->pt() << " " 
           << setw(8) << track->d0() << " " 
           << setw(8) << track->d0_err() << " " 
           << setw(8) << track->d0_signif() << " " 
           << setw(8) << signed_d0 << " " 
           << setw(8) << x << " z0: "
           << setw(8) << track->z0() << " "
           << setw(8) << track->z0_err() << " "
           << setw(8) << track->z0_signif() << " chi2:"
           << setw(8) << track->chi2() << " "
           << setw(4) << track->nhits() << " "
           << setw(4) << track->ndof() << " "
           << setw(4) << track->nmisses() << " "
           << setw(8) << track->chi2ndof() << " "
           << setw(8) << track->chi2prob() << " lik: "
           << setw(8) << track_b_lik << " "
           << setw(8) << track_notb_lik << " "
           << setw(8) << (track_b_lik/track_notb_lik) << " "
           << endl;
    }
    if( ntracks>0u ) { 
      b_sum /= ntracks;
      notb_sum /= ntracks;
      cout << " product: " << product << " " << b_product << " " << notb_product << " "
           << b_sum << " " << notb_sum << " " << (product/(1.+product)) << endl;
    }
 }
  //
  return product;
}
