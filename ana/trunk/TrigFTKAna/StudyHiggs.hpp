 
#ifndef STUDYHIGGS_HPP
#define STUDYHIGGS_HPP

#include <cmath>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/format.hpp>
#include <boost/scope_exit.hpp>
#include <boost/assign/list_of.hpp>
#include "DataGraphics/DgStudy.hh"
#include "DataGraphics/DgCollection.hh"
#include "TrigFTKAna/AnaHiggs.hpp"
#include "TrigFTKAna/AnaParticle.hpp"
#include "TrigFTKAna/AnaElectron.hpp"
#include "TrigFTKAna/AnaMuon.hpp"
#include "TrigFTKAna/AnaMET.hpp"
#include "TrigFTKAna/AnaW.hpp"
#include "TrigFTKAna/AnaCandidateEvent.hpp"
#include "TrigFTKAna/StudyComposite.hpp"
#include "TrigFTKAna/StudyParticle.hpp"
#include "TrigFTKAna/StudyElectron.hpp"
#include "TrigFTKAna/StudyMuon.hpp"
#include "TrigFTKAna/Detector.hpp"


class
StudyHiggs : public DataGraphics::DgStudy< boost::shared_ptr<const AnaHiggs> >
{
public:
  typedef boost::shared_ptr<const AnaHiggs> type; // must be same type as the template argument for DgStudy.
protected:
  virtual void _study( type const& higgs ) { // the second const is important---matches the base class.
    using namespace std;
    using namespace boost;
    using namespace DataGraphics;
    shared_ptr< const AnaComposite > cop( boost::dynamic_pointer_cast<const AnaComposite>(higgs) ); assert( cop );
    shared_ptr< const AnaParticle > leading_pt_lepton;
    shared_ptr< const AnaParticle > subleading_pt_lepton;
    shared_ptr< const AnaComposite > dilepton_pair( higgs->dilepton_pair() );
    shared_ptr< const AnaMET > met;
    // TODO: generalize this to loop over all cop final states instead of direct constituents
    for( AnaComposite::const_iterator i=cop->begin_constituent(), f=cop->end_constituent(); i!=f; ++i ) {
      shared_ptr<const AnaParticle> p( *i );
      if( p->type() != AnaParticle::MET ) { // could be GENERIC, i.e. track lepton; AnaParticle::ELECTRON || p->type() == AnaParticle::MUON ) {
        if( !leading_pt_lepton || p->pt() > leading_pt_lepton->pt() ) { 
          subleading_pt_lepton = leading_pt_lepton;
          leading_pt_lepton = p;
        } else if( !subleading_pt_lepton || p->pt() > subleading_pt_lepton->pt() ) {
          subleading_pt_lepton = p;
        }
      } else if( p->type() == AnaParticle::MET ) {
        assert( !met );
        met = dynamic_pointer_cast<const AnaMET>(p);
        assert( met );
      }
    } // end for each constituent of the composite.
    assert( leading_pt_lepton );
    assert( subleading_pt_lepton );
    assert( met );
    // make plots
    StudyComposite::study( cop );
    if( true ) { 
      dg::down( "met" , "met" ); BOOST_SCOPE_EXIT(void) { dg::up(); } BOOST_SCOPE_EXIT_END;
      dg::fillh( "met" , 100 , 0 , 200 , met->pt() , "Missing ET (GeV)" );
      dg::fillh( "met_phi", 64 , 0 , M_PI , fabs( met->phi() ) , "MET #phi (rad)" );
      dg::fillh( "met_eta", 60 , -3 , 3 , met->eta() , "MET #eta" );
      StudyParticle::study( met );
    }
    if( true ) { 
      dg::down( "leading_pt_lepton" , "leading pt lepton" ); BOOST_SCOPE_EXIT(void) { dg::up(); } BOOST_SCOPE_EXIT_END;
      StudyParticle::study( leading_pt_lepton );
    }
    if( true ) { 
      dg::down( "subleading_pt_lepton" , "subleading pt lepton" ); BOOST_SCOPE_EXIT(void) { dg::up(); } BOOST_SCOPE_EXIT_END;
      StudyParticle::study( subleading_pt_lepton );
    }
    dg::fillh( "dphi_between_leptons" , 100 , -M_PI , M_PI , leading_pt_lepton->dist_phi( subleading_pt_lepton ) );
    dg::fillh( "dphi_between_leptons_zoom" , 100 , -0.4 , 0.4 , leading_pt_lepton->dist_phi( subleading_pt_lepton ) );
    dg::fillh( "dphi_between_leptons_vs_mll_zoom" , 50 , 0 , 100 , 100 , -0.4 , 0.4 , dilepton_pair->mass() , 
               leading_pt_lepton->dist_phi( subleading_pt_lepton ) , "M_{ll} (GeV)" , "#Delta #phi (rad)");
    dg::fillh( "dphi_between_leptons_vs_llpt_zoom" , 50 , 0 , 100 , 100 , -0.4 , 0.4 , dilepton_pair->pt() , 
               leading_pt_lepton->dist_phi( subleading_pt_lepton ) , "p_{T}^{ll} (GeV)" , "#Delta #phi (rad)");
    dg::fillh( "dr_between_leptons" , 100 , -0.5 , 0.5 , leading_pt_lepton->dist_eta( subleading_pt_lepton ) );
    dg::fillh( "dr_between_leptons" , 100 , 0 , M_PI , leading_pt_lepton->dist_eta_phi( subleading_pt_lepton ) );

    dg::fillh( "leading_pt_lepton_pt_vs_met" , 100 , 0, 200, 100, 0 , 200 , met->pt(), leading_pt_lepton->pt() , "MET", "W Leading_Pt_Lepton #pt" );
    dg::fillh( "dphi_leading_pt_lepton_met" , 100 , -M_PI , M_PI , leading_pt_lepton->dist_phi( met ) , "#Delta #phi Leading_Pt_Lepton - MET (rad)" );
    dg::fillh( "deta_leading_pt_lepton_met" , 100 , -5 , 5 , leading_pt_lepton->dist_eta( met ) , "#Delta #eta Leading_Pt_Lepton - MET" );
    dg::fillh( "dr_leading_pt_lepton_met" , 100 , -5 , 5 , leading_pt_lepton->dist_eta_phi( met ) , "#Delta r(#eta,#phi) Leading_Pt_Lepton - MET" );
    // dg::fillh( "balance_vecsumphi_leading_pt_lepton" , 100 , -M_PI , M_PI , 
    //            detector::delta_phi(event_vecsumphi,leading_pt_lepton->phi()) , "Event Vector #Sigma p_{T} #phi - Leading_Pt_Lepton #phi (rad)" );
    // dg::fillh( "balance_vecsumphi_met" , 100 , -M_PI , M_PI , detector::delta_phi(event_vecsumphi,met->phi()) , "Event Vector #Sigma p_{T} #phi - MET #phi (rad)" );
    dg::fillh( "met_vs_dphi_leading_pt_lepton_met", 48, -M_PI, M_PI, 100, 0 , 200 , leading_pt_lepton->dist_phi( met ),
               met->pt() , "#Delta #phi Leading_Pt_Lepton - MET (rad)" , "MET [GeV]" );

    dg::fillh( "subleading_pt_lepton_pt_vs_met" , 100 , 0, 200, 100, 0 , 200 , met->pt(), subleading_pt_lepton->pt() , "MET", "W Subleading_Pt_Lepton #pt" );
    dg::fillh( "dphi_subleading_pt_lepton_met" , 100 , -M_PI , M_PI , subleading_pt_lepton->dist_phi( met ) , "#Delta #phi Subleading_Pt_Lepton - MET (rad)" );
    dg::fillh( "deta_subleading_pt_lepton_met" , 100 , -5 , 5 , subleading_pt_lepton->dist_eta( met ) , "#Delta #eta Subleading_Pt_Lepton - MET" );
    dg::fillh( "dr_subleading_pt_lepton_met" , 100 , -5 , 5 , subleading_pt_lepton->dist_eta_phi( met ) , "#Delta r(#eta,#phi) Subleading_Pt_Lepton - MET" );
    // dg::fillh( "balance_vecsumphi_subleading_pt_lepton" , 100 , -M_PI , M_PI , 
    //            detector::delta_phi(event_vecsumphi,subleading_pt_lepton->phi()) , "Event Vector #Sigma p_{T} #phi - Subleading_Pt_Lepton #phi (rad)" );
    // dg::fillh( "balance_vecsumphi_met" , 100 , -M_PI , M_PI , detector::delta_phi(event_vecsumphi,met->phi()) , "Event Vector #Sigma p_{T} #phi - MET #phi (rad)" );
    dg::fillh( "met_vs_dphi_subleading_pt_lepton_met", 48, -M_PI, M_PI, 100, 0 , 200 , subleading_pt_lepton->dist_phi( met ),
               met->pt() , "#Delta #phi Subleading_Pt_Lepton - MET (rad)" , "MET [GeV]" );


    dg::fillh( "dilepton_pair_pt" , 100 , 0, 200 , dilepton_pair->pt() , "Dilepton Pair p_{T} (Gev)" );
    dg::fillh( "dilepton_pair_mass" , 100 , 0, 200 , dilepton_pair->mass() , "Dilepton Pair Mass (GeV)t" );
    dg::fillh( "dilepton_pair_pt_vs_met" , 100 , 0, 200, 100, 0 , 200 , met->pt(), dilepton_pair->pt() , "MET", "W Dilepton_Pair #pt" );
    dg::fillh( "dphi_dilepton_pair_met" , 100 , -M_PI , M_PI , dilepton_pair->dist_phi( met ) , "#Delta #phi Dilepton_Pair - MET (rad)" );
    dg::fillh( "deta_dilepton_pair_met" , 100 , -5 , 5 , dilepton_pair->dist_eta( met ) , "#Delta #eta Dilepton_Pair - MET" );
    dg::fillh( "dr_dilepton_pair_met" , 100 , -5 , 5 , dilepton_pair->dist_eta_phi( met ) , "#Delta r(#eta,#phi) Dilepton_Pair - MET" );
    // dg::fillh( "balance_vecsumphi_dilepton_pair" , 100 , -M_PI , M_PI , 
    //            detector::delta_phi(event_vecsumphi,dilepton_pair->phi()) , "Event Vector #Sigma p_{T} #phi - Dilepton_Pair #phi (rad)" );
    // dg::fillh( "balance_vecsumphi_met" , 100 , -M_PI , M_PI , detector::delta_phi(event_vecsumphi,met->phi()) , "Event Vector #Sigma p_{T} #phi - MET #phi (rad)" );
    dg::fillh( "met_vs_dphi_dilepton_pair_met", 48, -M_PI, M_PI, 100, 0 , 200 , dilepton_pair->dist_phi( met ),
               met->pt() , "#Delta #phi Dilepton_Pair - MET (rad)" , "MET [GeV]" );

    // relative MET.
    const AnaMET::ftype met_rel = met->pt() * std::sin( dilepton_pair->signed_min_amongst_delta_phi_wrt( met ) );
    dg::fillh( "met_rel" , 100 , 0 , 100 , met_rel , "W Missing ET Projection (GeV)" );
    
    // higgs transverse mass
    dg::fillh( "mt_composite" , 250 , 0 , 250 , higgs->transverse_mass() , "Higgs M_{T} (GeV/c^{2})" ); 
    dg::fillh( "mt_self" , 250 , 0 , 500 , higgs->mt_self() , "Higgs M_{T} (GeV/c^{2})" );

    // lepton type studies
    if( leading_pt_lepton->type() == AnaParticle::ELECTRON ) {
      dg::down( "leading_ele" , "leading lepton is electron" ); BOOST_SCOPE_EXIT(void) { dg::up(); } BOOST_SCOPE_EXIT_END;
      StudyElectron::study( boost::dynamic_pointer_cast<const AnaElectron>(leading_pt_lepton) );
    }
    if( leading_pt_lepton->type() == AnaParticle::MUON ) {
      dg::down( "leading_muon" , "leading lepton is muon" ); BOOST_SCOPE_EXIT(void) { dg::up(); } BOOST_SCOPE_EXIT_END;
      StudyMuon::study( boost::dynamic_pointer_cast<const AnaMuon>(leading_pt_lepton) );
    }
    if( subleading_pt_lepton->type() == AnaParticle::ELECTRON ) {
      dg::down( "subleading_ele" , "subleading lepton is electron" ); BOOST_SCOPE_EXIT(void) { dg::up(); } BOOST_SCOPE_EXIT_END;
      StudyElectron::study( boost::dynamic_pointer_cast<const AnaElectron>(subleading_pt_lepton) );
    }
    if( subleading_pt_lepton->type() == AnaParticle::MUON ) {
      dg::down( "subleading_muon" , "subleading lepton is muon" ); BOOST_SCOPE_EXIT(void) { dg::up(); } BOOST_SCOPE_EXIT_END;
      StudyMuon::study( boost::dynamic_pointer_cast<const AnaMuon>(subleading_pt_lepton) );
    }
    if( leading_pt_lepton->type() == AnaParticle::ELECTRON && subleading_pt_lepton->type() == AnaParticle::ELECTRON ) {
      boost::shared_ptr<const AnaElectron> leading_ele( boost::dynamic_pointer_cast<const AnaElectron>(leading_pt_lepton) );
      boost::shared_ptr<const AnaElectron> subleading_ele( boost::dynamic_pointer_cast<const AnaElectron>(subleading_pt_lepton) );
      dg::filleff( "frac_ee_with_otx" , leading_ele->pass_otx() && subleading_ele->pass_otx() );
      dg::filleff( "frac_ee_with_subleading_otx" , subleading_ele->pass_otx() );
    }

    if( (leading_pt_lepton->type() == AnaParticle::ELECTRON && subleading_pt_lepton->type() == AnaParticle::MUON) ||
        (subleading_pt_lepton->type() == AnaParticle::ELECTRON && leading_pt_lepton->type() == AnaParticle::MUON) ) {
      boost::shared_ptr<const AnaElectron> ele;
      boost::shared_ptr<const AnaMuon> mu;
      if( leading_pt_lepton->type() == AnaParticle::ELECTRON ) {
        ele = boost::dynamic_pointer_cast<const AnaElectron>(leading_pt_lepton);
        mu = boost::dynamic_pointer_cast<const AnaMuon>(subleading_pt_lepton);
      } else {
        ele = boost::dynamic_pointer_cast<const AnaElectron>(subleading_pt_lepton);
        mu = boost::dynamic_pointer_cast<const AnaMuon>(leading_pt_lepton);
      }
      dg::filleff( "frac_emu_with_otx" , ele->pass_otx() );
      if( subleading_pt_lepton->type() == AnaParticle::ELECTRON ) { 
        dg::filleff( "frac_emu_with_subleading_otx" , ele->pass_otx() );
      }
    }

    if( leading_pt_lepton->type() == AnaParticle::MUON && subleading_pt_lepton->type() == AnaParticle::MUON ) {
      boost::shared_ptr<const AnaMuon> leading_mu( boost::dynamic_pointer_cast<const AnaMuon>(leading_pt_lepton) );
      boost::shared_ptr<const AnaMuon> subleading_mu( boost::dynamic_pointer_cast<const AnaMuon>(subleading_pt_lepton) );
      dg::filleff( "frac_mm_with_combined" , leading_mu->is_staco_combined() && subleading_mu->is_staco_combined() );
      dg::filleff( "frac_mm_with_combined_or_mutag" , 
                   (leading_mu->is_staco_combined()||leading_mu->is_mutag()) && (subleading_mu->is_staco_combined()||subleading_mu->is_mutag()) );
    }
    
    // trigger efficiencies
    if( _event ) {
      const bool any_jets_bad_for_met = std::count_if( _event->begin_jets() , _event->end_jets() ,
                                                       bind(&AnaJet::is_good_loose_for_met_jetetmiss_v16,_1)==false ) > 0;
      const bool ok_clean_met = !any_jets_bad_for_met && _event->met();
      const bool is_triggered_e15_medium = _event->trigger() ? _event->trigger()->passes("EF_e15_medium") : false;
      const bool is_triggered_e20_medium = _event->trigger() ? _event->trigger()->passes("EF_e20_medium") : false;
      const bool is_triggered_e25_medium = _event->trigger() ? _event->trigger()->passes("EF_e25_medium") : false;
      const bool is_triggered_e30_loose = _event->trigger() ? _event->trigger()->passes("EF_e30_loose") : false;
      const bool is_triggered_mu13_mg_tight = _event->trigger() ? _event->trigger()->passes("EF_MU13_MG_TIGHT") : false;
      const bool is_triggered_mu15_mg = _event->trigger() ? _event->trigger()->passes("EF_mu15_mg") : false;
      const bool is_triggered_mu20 = _event->trigger() ? _event->trigger()->passes("EF_mu20") : false;
      const bool is_triggered_2mu10 = _event->trigger() ? _event->trigger()->passes("EF_2mu10") : false;
      const bool is_triggered_2e10_medium = _event->trigger() ? _event->trigger()->passes("EF_2e10_medium") : false;
      const bool is_triggered_e10_loose_mu6 = _event->trigger() ? _event->trigger()->passes("EF_e10_loose_mu6") : false;
      //
      dg::filleff( "eff_trig_mc_ele" , is_triggered_e15_medium );
      dg::filleff( "eff_trig_mc_mu" , is_triggered_mu13_mg_tight );
      dg::filleff( "eff_trig_mc_elemu" , is_triggered_e15_medium && is_triggered_mu13_mg_tight );
      dg::filleff( "eff_trig_mc_elemu_not" , !is_triggered_e15_medium && !is_triggered_mu13_mg_tight );
      //
      dg::filleff( "eff_trig_e15_medium" , is_triggered_e15_medium );
      if( leading_pt_lepton->type() == AnaParticle::ELECTRON ) { 
        dg::filleff( "eff_trig_e15_medium_vs_leading_pt" , 100 , 0 , 100 , leading_pt_lepton->pt() , is_triggered_e15_medium , "Leading Lepton p_{T} (GeV)" );
        dg::filleff( "eff_trig_e15_medium_vs_leading_eta" , 80 , -4 , 4 , leading_pt_lepton->eta() , is_triggered_e15_medium , "Leading Lepton #eta" );
      }
      dg::filleff( "eff_trig_e15_medium" , is_triggered_e15_medium );
      dg::filleff( "eff_trig_e20_medium" , is_triggered_e20_medium );
      dg::filleff( "eff_trig_e25_medium" , is_triggered_e25_medium );
      dg::filleff( "eff_trig_e30_loose" , is_triggered_e30_loose );
      dg::filleff( "eff_trig_mu13_mg_tight" , is_triggered_mu13_mg_tight );
      if( leading_pt_lepton->type() == AnaParticle::MUON ) { 
        dg::filleff( "eff_trig_mu13_mg_tight_vs_leading_pt" , 100 , 0 , 100 , leading_pt_lepton->pt() , is_triggered_mu13_mg_tight , "Leading Lepton p_{T} (GeV)" );
        dg::filleff( "eff_trig_mu13_mg_tight_vs_leading_eta" , 80 , -4 , 4 , leading_pt_lepton->eta() , is_triggered_mu13_mg_tight , "Leading Lepton #eta" );
      }
      dg::filleff( "eff_trig_mu15_mg" , is_triggered_mu15_mg );
      dg::filleff( "eff_trig_mu20" , is_triggered_mu20 );
      dg::filleff( "eff_trig_2mu10" , is_triggered_2mu10 );
      dg::filleff( "eff_trig_2e10_medium" , is_triggered_2e10_medium );
      dg::filleff( "eff_trig_e10_loose_mu6" , is_triggered_e10_loose_mu6 );
      if( !is_triggered_e20_medium && !is_triggered_mu20 ) {
        dg::filleff( "eff_trig_combined_nots20" , is_triggered_2mu10 || is_triggered_2e10_medium ||
                     is_triggered_e10_loose_mu6 );
      }
      dg::filleff( "eff_clean_met" , ok_clean_met );
      dg::filleff( "eff_privertex" , _event->empty_vertices() );
    }

    
  } // end study higgs
protected:
  boost::shared_ptr<const AnaCandidateEvent> _event;
public:
  StudyHiggs( const std::string& name , const std::string& description ) 
    : _event()
    , DataGraphics::DgStudy<type>(name,description)
  {}
  virtual ~StudyHiggs() {}
  void for_event( const boost::shared_ptr<const AnaCandidateEvent>& event ) { _event = event; }
};

#endif // STUDYHIGGS_HPP
