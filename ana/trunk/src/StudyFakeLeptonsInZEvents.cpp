#include <cmath>
#include <string>
#include <algorithm>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/make_shared.hpp>
#include <boost/foreach.hpp>
#include <boost/scope_exit.hpp>
#include "DataGraphics/DgStudy.hh"
#include "DataGraphics/DataGraphics.hh"
#include "TrigFTKAna/AnaParticle.hpp"
#include "TrigFTKAna/AnaElectron.hpp"
#include "TrigFTKAna/AnaMuon.hpp"
#include "TrigFTKAna/AnaTrack.hpp"
#include "TrigFTKAna/AnaZ.hpp"
#include "TrigFTKAna/StudyComposite.hpp"
#include "TrigFTKAna/StudyTrackingEfficiency.hpp"
#include "TrigFTKAna/StudyFakeLeptonsInZEvents.hpp"

using namespace std;
using namespace boost;
using namespace DataGraphics;

void
StudyFakeLeptonsInZEvents::study( StudyFakeLeptonsInZEvents::type const& zboson )
{
  // plot number of leptons and number of additional leptons in z
  // events.
  if( zboson->num_constituents() != 2 ) { return; }
  shared_ptr<const AnaParticle> leading_z_lepton( *(zboson->begin_constituent()) );
  shared_ptr<const AnaParticle> subleading_z_lepton( *(++(zboson->begin_constituent())) );
  if( leading_z_lepton->pt() < subleading_z_lepton->pt() ) { std::swap( leading_z_lepton , subleading_z_lepton ); }
  
  StudyComposite stcop( "z" , "dilepton pair" );
  stcop.for_only( zboson );
  
  if( std::abs(zboson->mass()) < 76. ) { return; }
  if( std::abs(zboson->mass()) > 106. ) { return; }
  
  dg::fillh( "accepted_dilepton_mass" , 50 , 66 , 116 , zboson->mass() , "m_{ll} (GeV)");

  // count number of additional leptons, using several measures of
  // equivalence.  note that the 'track' comparison still compares
  // AnaTrack pointers for the ID track and thus, since these are
  // typically created separately for each lepton during ntuple
  // reading, the track pointer test amounts to the same thing as the
  // lepton pointer test. the third test compares track parameters
  // using the FTK parameter matching criteria.

  unsigned int n_extra_leptons_pointer = 0u;
  unsigned int n_extra_electrons_pointer = 0u;
  unsigned int n_extra_muons_pointer = 0u;
  unsigned int n_extra_leptons_track = 0u;
  unsigned int n_extra_electrons_track = 0u;
  unsigned int n_extra_muons_track = 0u;
  unsigned int n_extra_leptons_track_param = 0u;
  unsigned int n_extra_electrons_track_param = 0u;
  unsigned int n_extra_muons_track_param = 0u;

  static vector< shared_ptr<const AnaParticle> > fakes;
  fakes.reserve( _leptons.size() );
  fakes.clear();

  AnaTrack::parametersEqualV paramV;
  
  BOOST_FOREACH( const shared_ptr<const AnaParticle>& lepton , _leptons ) {
    const bool same_leading_pointer = (lepton == leading_z_lepton);
    const bool same_subleading_pointer = (lepton == subleading_z_lepton);
    const bool same_pointer = (same_leading_pointer || same_subleading_pointer);
    const bool same_leading_track = (lepton->has_track() && (leading_z_lepton->has_track() && lepton->track() == leading_z_lepton->track()));
    const bool same_subleading_track = (lepton->has_track() && (subleading_z_lepton->has_track() && lepton->track() == subleading_z_lepton->track()));
    const bool same_track = (same_leading_track || same_subleading_track);
    const bool same_leading_track_param = (lepton->has_track() && leading_z_lepton->has_track() && paramV(lepton->track(),leading_z_lepton->track()));
    const bool same_subleading_track_param = (lepton->has_track() && subleading_z_lepton->has_track() && paramV(lepton->track(),subleading_z_lepton->track()));
    const bool same_track_param = (same_leading_track_param || same_subleading_track_param);
    //
    if( !same_pointer ) { 
      ++n_extra_leptons_pointer;
      if( lepton->type()==AnaParticle::ELECTRON ) { ++n_extra_electrons_pointer; }
      if( lepton->type()==AnaParticle::MUON ) { ++n_extra_muons_pointer; }
    }
    if( !same_track ) { 
      ++n_extra_leptons_track;
      if( lepton->type()==AnaParticle::ELECTRON ) { ++n_extra_electrons_track; }
      if( lepton->type()==AnaParticle::MUON ) { ++n_extra_muons_track; }
    }
    if( !same_track_param ) { 
      ++n_extra_leptons_track_param;
      if( lepton->type()==AnaParticle::ELECTRON ) { ++n_extra_electrons_track_param; }
      if( lepton->type()==AnaParticle::MUON ) { ++n_extra_muons_track_param; }
    }
    // plot delta 4-vector and delta parameter distributions for the above
    AnaParticle::ftype deta_4v_leading = lepton->dist_eta( leading_z_lepton );
    AnaParticle::ftype deta_4v_subleading = lepton->dist_eta( subleading_z_lepton );
    AnaParticle::ftype deta_4v = detector::min_abs( deta_4v_leading , deta_4v_subleading );
    //
    AnaParticle::ftype dphi_4v_leading = lepton->dist_phi( leading_z_lepton );
    AnaParticle::ftype dphi_4v_subleading = lepton->dist_phi( subleading_z_lepton );
    AnaParticle::ftype dphi_4v = detector::min_abs( dphi_4v_leading , dphi_4v_subleading );
    //
    AnaParticle::ftype dr_4v_leading = lepton->dist_eta_phi( leading_z_lepton );
    AnaParticle::ftype dr_4v_subleading = lepton->dist_eta_phi( subleading_z_lepton );
    AnaParticle::ftype dr_4v = detector::min_abs( dr_4v_leading , dr_4v_subleading );
    //
    {
      dg::down( "all" , "all (no matching)" ); BOOST_SCOPE_EXIT(void) { dg::up(); } BOOST_SCOPE_EXIT_END;
      dg::fillh( "deta_4v_leading" , 100 , -5 , 5 , deta_4v_leading );
      dg::fillh( "deta_4v_subleading" , 100 , -5 , 5 , deta_4v_subleading );
      dg::fillh( "deta_4v" , 100 , -1 , 1 , deta_4v );
      dg::fillh( "dphi_4v_leading" , 100 , -M_PI , M_PI , dphi_4v_leading );
      dg::fillh( "dphi_4v_subleading" , 100 , -M_PI , M_PI , dphi_4v_subleading );
      dg::fillh( "dphi_4v" , 100 , -0.5 , 0.5 , dphi_4v );
      dg::fillh( "dr_4v_leading" , 100 , -5 , 5 , dr_4v_leading );
      dg::fillh( "dr_4v_subleading" , 100 , -5 , 5 , dr_4v_subleading );
      dg::fillh( "dr_4v" , 100 , -1 , 1 , dr_4v );
    }
    if( same_pointer ) { 
      dg::down( "same_pointer" , "lepton has the same pointer" ); BOOST_SCOPE_EXIT(void) { dg::up(); } BOOST_SCOPE_EXIT_END;
      dg::fillh( "deta_4v_leading" , 100 , -5 , 5 , deta_4v_leading );
      dg::fillh( "deta_4v_subleading" , 100 , -5 , 5 , deta_4v_subleading );
      dg::fillh( "deta_4v" , 100 , -1 , 1 , deta_4v );
      dg::fillh( "dphi_4v_leading" , 100 , -M_PI , M_PI , dphi_4v_leading );
      dg::fillh( "dphi_4v_subleading" , 100 , -M_PI , M_PI , dphi_4v_subleading );
      dg::fillh( "dphi_4v" , 100 , -0.5 , 0.5 , dphi_4v );
      dg::fillh( "dr_4v_leading" , 100 , -5 , 5 , dr_4v_leading );
      dg::fillh( "dr_4v_subleading" , 100 , -5 , 5 , dr_4v_subleading );
      dg::fillh( "dr_4v" , 100 , -1 , 1 , dr_4v );
    } else {
      dg::down( "not_same_pointer" , "lepton does not have the same pointer as a z leg" ); BOOST_SCOPE_EXIT(void) { dg::up(); } BOOST_SCOPE_EXIT_END;
      dg::fillh( "deta_4v_leading" , 100 , -5 , 5 , deta_4v_leading );
      dg::fillh( "deta_4v_subleading" , 100 , -5 , 5 , deta_4v_subleading );
      dg::fillh( "deta_4v" , 100 , -1 , 1 , deta_4v );
      dg::fillh( "dphi_4v_leading" , 100 , -M_PI , M_PI , dphi_4v_leading );
      dg::fillh( "dphi_4v_subleading" , 100 , -M_PI , M_PI , dphi_4v_subleading );
      dg::fillh( "dphi_4v" , 100 , -0.5 , 0.5 , dphi_4v );
      dg::fillh( "dr_4v_leading" , 100 , -5 , 5 , dr_4v_leading );
      dg::fillh( "dr_4v_subleading" , 100 , -5 , 5 , dr_4v_subleading );
      dg::fillh( "dr_4v" , 100 , -1 , 1 , dr_4v );
    }
    if( same_track_param ) { 
      dg::down( "same_track_param" , "lepton has the same track_param" ); BOOST_SCOPE_EXIT(void) { dg::up(); } BOOST_SCOPE_EXIT_END;
      dg::fillh( "deta_4v_leading" , 100 , -5 , 5 , deta_4v_leading );
      dg::fillh( "deta_4v_subleading" , 100 , -5 , 5 , deta_4v_subleading );
      dg::fillh( "deta_4v" , 100 , -1 , 1 , deta_4v );
      dg::fillh( "dphi_4v_leading" , 100 , -M_PI , M_PI , dphi_4v_leading );
      dg::fillh( "dphi_4v_subleading" , 100 , -M_PI , M_PI , dphi_4v_subleading );
      dg::fillh( "dphi_4v" , 100 , -0.5 , 0.5 , dphi_4v );
      dg::fillh( "dr_4v_leading" , 100 , -5 , 5 , dr_4v_leading );
      dg::fillh( "dr_4v_subleading" , 100 , -5 , 5 , dr_4v_subleading );
      dg::fillh( "dr_4v" , 100 , -1 , 1 , dr_4v );
    } else {
      dg::down( "not_same_track_param" , "lepton does not have the same track_param as a z leg" ); BOOST_SCOPE_EXIT(void) { dg::up(); } BOOST_SCOPE_EXIT_END;
      dg::fillh( "deta_4v_leading" , 100 , -5 , 5 , deta_4v_leading );
      dg::fillh( "deta_4v_subleading" , 100 , -5 , 5 , deta_4v_subleading );
      dg::fillh( "deta_4v" , 100 , -1 , 1 , deta_4v );
      dg::fillh( "dphi_4v_leading" , 100 , -M_PI , M_PI , dphi_4v_leading );
      dg::fillh( "dphi_4v_subleading" , 100 , -M_PI , M_PI , dphi_4v_subleading );
      dg::fillh( "dphi_4v" , 100 , -0.5 , 0.5 , dphi_4v );
      dg::fillh( "dr_4v_leading" , 100 , -5 , 5 , dr_4v_leading );
      dg::fillh( "dr_4v_subleading" , 100 , -5 , 5 , dr_4v_subleading );
      dg::fillh( "dr_4v" , 100 , -1 , 1 , dr_4v );
    }
    // define 'fakes'
    if( !same_track_param && 
        lepton->pt() > 10. ) {
      fakes.push_back( lepton ); 
    }
  } // end for each lepton
  
  dg::fillh( "n_extra_leptons_pointer" , 10 , 0 , 10 , n_extra_leptons_pointer );
  dg::fillh( "n_extra_electrons_pointer" , 10 , 0 , 10 , n_extra_electrons_pointer );
  dg::fillh( "n_extra_muons_pointer" , 10 , 0 , 10 , n_extra_muons_pointer );
  dg::fillh( "n_extra_leptons_track" , 10 , 0 , 10 , n_extra_leptons_track );
  dg::fillh( "n_extra_electrons_track" , 10 , 0 , 10 , n_extra_electrons_track );
  dg::fillh( "n_extra_muons_track" , 10 , 0 , 10 , n_extra_muons_track );
  dg::fillh( "n_extra_leptons_track_param" , 10 , 0 , 10 , n_extra_leptons_track_param );
  dg::fillh( "n_extra_electrons_track_param" , 10 , 0 , 10 , n_extra_electrons_track_param );
  dg::fillh( "n_extra_muons_track_param" , 10 , 0 , 10 , n_extra_muons_track_param );

  // study fakes
  StudyParticle stp( "fakes" , "fake leptons" );
  stp.for_each( fakes.begin() , fakes.end() );

  dg::fillh( "n_fakes" , 20 , 0 , 20 , fakes.size() );
  dg::fillh( "n_fakes_pt15" , 20 , 0 , 20 , std::count_if( fakes.begin() , fakes.end() , bind(&AnaParticle::pt,_1) > 15. ) );
  dg::fillh( "n_fakes_pt20" , 20 , 0 , 20 , std::count_if( fakes.begin() , fakes.end() , bind(&AnaParticle::pt,_1) > 20. ) );
  dg::fillh( "n_fakes_pt25" , 20 , 0 , 20 , std::count_if( fakes.begin() , fakes.end() , bind(&AnaParticle::pt,_1) > 25. ) );
  dg::fillh( "n_fakes_pt50" , 20 , 0 , 20 , std::count_if( fakes.begin() , fakes.end() , bind(&AnaParticle::pt,_1) > 50. ) );
  dg::fillh( "n_fakes_eta_low" , 20 , 0 , 20 , std::count_if( fakes.begin() , fakes.end() , bind(&AnaParticle::abseta,_1) < 1. ) );
  dg::fillh( "n_fakes_eta_mid" , 20 , 0 , 20 , std::count_if( fakes.begin() , fakes.end() ,
                                                              bind(&AnaParticle::abseta,_1) > 1. && bind(&AnaParticle::abseta,_1) < 2. ) );
  dg::fillh( "n_fakes_eta_high" , 20 , 0 , 20 , std::count_if( fakes.begin() , fakes.end() , bind(&AnaParticle::abseta,_1) > 2. ) );

  // study fake rate vs some denominators
  // w+jets estimate uses wrt ?

  // possible denominators:
  //   number of tracks in event
  //   number of reconstructed muons in the event
  //   number of reconstructed jets in the event
  //   etc.

  // so, given a 'denominator' collection, look for fakes matching the denominator. remove 
  // z legs from the denominator collection first.
  vector< shared_ptr<const AnaParticle> > denoms;
  vector< boost::shared_ptr<const AnaParticle> >::const_iterator iden_leg0 = _denominators.end();
  vector< boost::shared_ptr<const AnaParticle> >::const_iterator iden_leg1 = _denominators.end();
  AnaTrack::ftype min_delta_r_leg0 = 9999;
  AnaTrack::ftype min_delta_r_leg1 = 9999;
  // _denominators
  for( vector< shared_ptr<const AnaParticle> >::const_iterator idenom=_denominators.begin(), fdenom=_denominators.end(); idenom!=fdenom; ++idenom ) {
    const shared_ptr<const AnaParticle>& denom( *idenom );
    if( denom->has_track() && leading_z_lepton->has_track() && paramV(denom->track(),leading_z_lepton->track()) ) { 
      const AnaTrack::ftype dr = leading_z_lepton->dist_eta_phi( denom );
      if( dr < min_delta_r_leg0 ) { 
        min_delta_r_leg0 = dr;
        iden_leg0 = idenom;
      }
    }
    if( denom->has_track() && subleading_z_lepton->has_track() && paramV(denom->track(),subleading_z_lepton->track()) ) { 
      const AnaTrack::ftype dr = subleading_z_lepton->dist_eta_phi( denom );
      if( dr < min_delta_r_leg1 ) { 
        min_delta_r_leg1 = dr;
        iden_leg1 = idenom;
      }
    }
  } // end look for the single best match to each z lepton in the denominator collection.
  // copy everything but the <=two z lepton matches into the denominator collection.
  for( vector< shared_ptr<const AnaParticle> >::const_iterator idenom=_denominators.begin(), fdenom=_denominators.end(); idenom!=fdenom; ++idenom ) {
    const shared_ptr<const AnaParticle>& denom( *idenom );
    if( idenom == iden_leg0 && min_delta_r_leg0<0.01 ) { continue; }
    if( idenom == iden_leg1 && min_delta_r_leg1<0.01 ) { continue; }
    denoms.push_back( denom );
  }
  // look for fakes amongst the remaining denominator collection
  BOOST_FOREACH( const shared_ptr<const AnaParticle>& denom , denoms ) {
    bool same_pointer = false;
    bool same_track = false;
    bool same_track_param = false;
    // look for fake matching the denominator
    BOOST_FOREACH( const shared_ptr<const AnaParticle>& fake , fakes ) {
      same_pointer = same_pointer || (fake == denom);
      same_track = same_track || (fake->has_track() && (denom->has_track() && fake->track() == denom->track()));
      same_track_param = same_track_param || (fake->has_track() && denom->has_track() && paramV(fake->track(),denom->track()));
    } // end for each fake
    //
    dg::filleff( "fake_pointer_eff_vs_pt" , 100 , 0 , 100 , denom->pt() , same_pointer , "p_{T} (GeV)" );
    dg::filleff( "fake_pointer_eff_vs_eta" , 64 , -4 , 4 , denom->eta() , same_pointer , "#eta" );
    dg::filleff( "fake_pointer_eff_vs_phi" , 48 , -M_PI , M_PI , denom->phi() , same_pointer , "#phi (rad)" );
    //
    dg::filleff( "fake_track_eff_vs_pt" , 100 , 0 , 100 , denom->pt() , same_track , "p_{T} (GeV)" );
    dg::filleff( "fake_track_eff_vs_eta" , 64 , -4 , 4 , denom->eta() , same_track , "#eta" );
    dg::filleff( "fake_track_eff_vs_phi" , 48 , -M_PI , M_PI , denom->phi() , same_track , "#phi (rad)" );
    //
    dg::filleff( "fake_track_param_eff_vs_pt" , 100 , 0 , 100 , denom->pt() , same_track_param , "p_{T} (GeV)" );
    dg::filleff( "fake_track_param_eff_vs_eta" , 64 , -4 , 4 , denom->eta() , same_track_param , "#eta" );
    dg::filleff( "fake_track_param_eff_vs_phi" , 48 , -M_PI , M_PI , denom->phi() , same_track_param , "#phi (rad)" );
    //
  } // end for each denominator object
  // what about fakes not matching a denominator?
  BOOST_FOREACH( const shared_ptr<const AnaParticle>& fake , fakes ) {
    bool same_pointer = false;
    bool same_track = false;
    bool same_track_param = false;
    BOOST_FOREACH( const shared_ptr<const AnaParticle>& denom , _denominators ) {
      AnaTrack::parametersEqualV paramV;
      same_pointer = same_pointer || (fake == denom);
      same_track = same_track || (fake->has_track() && (denom->has_track() && fake->track() == denom->track()));
      same_track_param = same_track_param || (fake->has_track() && denom->has_track() && paramV(fake->track(),denom->track()));
    }
    //
    dg::filleff( "unmatched_fake_pointer_eff_vs_pt" , 100 , 0 , 100 , fake->pt() , same_pointer , "p_{T} (GeV)" );
    dg::filleff( "unmatched_fake_pointer_eff_vs_eta" , 64 , -4 , 4 , fake->eta() , same_pointer , "#eta" );
    dg::filleff( "unmatched_fake_pointer_eff_vs_phi" , 48 , -M_PI , M_PI , fake->phi() , same_pointer , "#phi (rad)" );
    //
    dg::filleff( "unmatched_fake_track_eff_vs_pt" , 100 , 0 , 100 , fake->pt() , same_track , "p_{T} (GeV)" );
    dg::filleff( "unmatched_fake_track_eff_vs_eta" , 64 , -4 , 4 , fake->eta() , same_track , "#eta" );
    dg::filleff( "unmatched_fake_track_eff_vs_phi" , 48 , -M_PI , M_PI , fake->phi() , same_track , "#phi (rad)" );
    //
    dg::filleff( "unmatched_fake_track_param_eff_vs_pt" , 100 , 0 , 100 , fake->pt() , same_track_param , "p_{T} (GeV)" );
    dg::filleff( "unmatched_fake_track_param_eff_vs_eta" , 64 , -4 , 4 , fake->eta() , same_track_param , "#eta" );
    dg::filleff( "unmatched_fake_track_param_eff_vs_phi" , 48 , -M_PI , M_PI , fake->phi() , same_track_param , "#phi (rad)" );
    //
  }
}
