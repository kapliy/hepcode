 
#ifndef STUDYWCROSSECTION_HPP
#define STUDYWCROSSECTION_HPP

#include <cmath>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <numeric>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/format.hpp>
#include <boost/scope_exit.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/assign/std/vector.hpp>
#include "DataGraphics/DgStudy.hh"
#include "DataGraphics/DgCollection.hh"
#include "TrigFTKAna/AnaWCrossSectionCandidate.hpp"
#include "TrigFTKAna/AnaParticle.hpp"
#include "TrigFTKAna/AnaMET.hpp"
#include "TrigFTKAna/AnaW.hpp"
#include "TrigFTKAna/AnaMuon.hpp"
#include "TrigFTKAna/AnaElectron.hpp"
#include "TrigFTKAna/Detector.hpp"

class
StudyWCrossSection : public DataGraphics::DgStudy< boost::shared_ptr<const AnaWCrossSectionCandidate> >
{
public:
  typedef boost::shared_ptr<const AnaWCrossSectionCandidate> type; // must be same type as the template argument for DgStudy.
protected:
  // a wrapper implementing (optional) charge separation
  virtual void _study( type const& wevent ) {
    using namespace DataGraphics;
    study_do(wevent);
    if(_do_charge_separation) {
      if(wevent->scharge()>0) {
	dg::down( "POS" , "W+ candidates" ); BOOST_SCOPE_EXIT(void) { dg::up(); } BOOST_SCOPE_EXIT_END;
	study_do(wevent);
      }
      if(wevent->scharge()<0) {
	dg::down( "NEG" , "W- candidates" ); BOOST_SCOPE_EXIT(void) { dg::up(); } BOOST_SCOPE_EXIT_END;
	study_do(wevent);
      }
    }
    if( _do_eta_separation ) {
      if( fabs(wevent->lepton_eta()) >= 1.9 ) {
	dg::down( "ENDCAP" , "leptons with |eta| >= 1.9" ); BOOST_SCOPE_EXIT(void) { dg::up(); } BOOST_SCOPE_EXIT_END;
	study_do(wevent);
      }
      if( fabs(wevent->lepton_eta()) < 1.9 ) {
	dg::down( "BARREL" , "leptons with |eta| < 1.9" ); BOOST_SCOPE_EXIT(void) { dg::up(); } BOOST_SCOPE_EXIT_END;
	study_do(wevent);
      }
    }
  }
  // actual study
  virtual void study_do( type const& wevent ) {
    using namespace std;
    using namespace boost;
    using namespace DataGraphics;
    const boost::shared_ptr<const AnaWCrossSectionCandidate>& w = wevent;
    const boost::shared_ptr<const AnaParticle>& lepton = w->lepton();
    const boost::shared_ptr<const AnaMuon> muon = boost::dynamic_pointer_cast<const AnaMuon,const AnaParticle>(lepton);
    const boost::shared_ptr<const AnaElectron> electron = boost::dynamic_pointer_cast<const AnaElectron,const AnaParticle>(lepton);
    const boost::shared_ptr<const AnaPhoton> photon = boost::dynamic_pointer_cast<const AnaPhoton,const AnaParticle>(lepton);
    const boost::shared_ptr<const AnaMET>& met = w->met();
    const AnaParticle::ftype jet_sumpt =  std::accumulate( wevent->begin_jets() , wevent->end_jets() , 0. , 
                                                           lambda::bind(&AnaJet::pt,lambda::bind(&shared_ptr<const AnaJet>::get,lambda::_2)) + lambda::_1 );
    const AnaParticle::ftype jet_sumpx =  std::accumulate( wevent->begin_jets() , wevent->end_jets() , 0. , 
                                                           lambda::bind(&AnaJet::px,lambda::bind(&shared_ptr<const AnaJet>::get,lambda::_2)) + lambda::_1 );
    const AnaParticle::ftype jet_sumpy =  std::accumulate( wevent->begin_jets() , wevent->end_jets() , 0. , 
                                                           lambda::bind(&AnaJet::py,lambda::bind(&shared_ptr<const AnaJet>::get,lambda::_2)) + lambda::_1 );
    const AnaParticle::ftype jet_vecsumpt = detector::quadrature( jet_sumpx , jet_sumpy );
    const AnaParticle::ftype event_sumpx = w->px() + jet_sumpx;
    const AnaParticle::ftype event_sumpy = w->py() + jet_sumpy;
    const AnaParticle::ftype event_vecsumpt = detector::quadrature( event_sumpx , event_sumpy );
    const AnaParticle::ftype event_vecsumphi = std::atan2( event_sumpy , event_sumpx );
    const AnaParticle::ftype wjets_sumpt = w->pt() + jet_sumpt;
    // jet ordering
    vector< boost::shared_ptr<const AnaJet> > jets_by_pt( wevent->begin_jets() , wevent->end_jets() );
    std::sort( jets_by_pt.begin() , jets_by_pt.end() , bind(&AnaJet::pt,_1) > bind(&AnaJet::pt,_2) );
    // make plots
    dg::fillh( "w_mt" , 500 , 0 , 500 , w->transverse_mass() , "W Transverse Mass (GeV/c^{2})" );
    dg::fillh( "w_pt" , 500 , 0 , 500 , w->pt() , "W Transverse Momentum (GeV/c)" );
    dg::fillh( "w_pt2" , 1000 , 0 , 200 , w->pt()*w->pt() , "W Transverse Momentum^2 (GeV/c)" );
    dg::fillh( "njets" , 10 , 0 , 10 , wevent->njets() , "N_{jets}" );
    dg::fillh( "ntruth" , 10 , 0 , 10 , wevent->ntruth() , "N_{truth}" );
    dg::fillh( "nvlqtruth" , 10 , 0 , 10 , wevent->nvlqtruth() , "N_{truth} VLQs" );
    dg::fillh( "nwtruth" , 10 , 0 , 10 , wevent->nwtruth() , "N_{truth} Ws" );
    dg::fillh( "nnutruth" , 10 , 0 , 10 , wevent->nnutruth() , "N_{truth} #nu" );
    dg::fillh( "nfjtruth" , 10 , 0 , 10 , wevent->nfjtruth() , "N_{truth} forward jets" );
    // vector and scalar sums
    dg::fillh( "jet_sumpt" , 200 , 0 , 200 , jet_sumpt , "Jet #Sigma p_{T} (GeV)" );
    dg::fillh( "jet_vecsumpt" , 100 , 0 , 100 , jet_vecsumpt , "Jet Vector #Sigma p_{T} Magnitude (GeV)" );
    dg::fillh( "wjets_vecsumpt" , 500 , 0 , 500 , event_vecsumpt , "Event Vector #Sigma p_{T} Magnitude (GeV)" );
    dg::fillh( "wjets_sumpt" , 2000 , 0 , 2000 , wjets_sumpt , "Event Scaler #Sigma p_{T} (GeV)" );
    if( lepton ) {
      const AnaParticle::ftype ljets_sumpt = lepton->pt() + jet_sumpt;
      const AnaParticle::ftype ljets_vecsumpt = detector::quadrature( lepton->px() + jet_sumpx ,
                                                                      lepton->py() + jet_sumpy );
      dg::fillh( "ljets_sumpt" , 2000 , 0 , 2000 , ljets_sumpt , "Lepton and Jets Scaler #Sigma p_{T} (GeV)" );
      dg::fillh( "ljets_vecsumpt" , 1000 , 0 , 1000 , ljets_vecsumpt , "Lepton and Jets Vector #Sigma p_{T} Magnitude (GeV)" );
      if( met ) {
        const AnaParticle::ftype lvjets_sumpt = lepton->pt() + met->pt() + jet_sumpt;
        const AnaParticle::ftype lvjets_vecsumpt = detector::quadrature( lepton->px() + met->px() + jet_sumpx ,
  								         lepton->py() + met->py() + jet_sumpy );
        dg::fillh( "lvjets_sumpt" , 2000 , 0 , 2000 , lvjets_sumpt , "Lepton, MET, and Jet Scaler #Sigma p_{T} (GeV)" );
        dg::fillh( "lvjets_vecsumpt" , 1000 , 0 , 1000 , lvjets_vecsumpt , "Lepton, MET, and Jet Vector #Sigma p_{T} Magnitude (GeV)" );
      }
    }
    if( true ) { 
      if( met ) {
	dg::fillh( "met" , 500 , 0 , 500 , met->pt() , "W Missing ET (GeV)" );
        dg::fillh( "met_phi", 100 , -M_PI , M_PI , met->phi() , "MET #phi (rad)" );
	dg::fillh( "met_absphi", 100 , 0 , M_PI , fabs(met->phi()) , "MET |#phi| (rad)" );
        dg::fillh( "met_eta", 100 , -8 , 8 , wevent->met_eta( wevent->met_pz_vlq() ) , "MET #eta" );
	if( _do_2d_plots ) dg::fillh( "wmt_vs_met" , 200,0,200,200,0,200,met->pt(),w->transverse_mass(),"MET", "W Transverse Mass (GeV/c^{2})" );
      } // end met plots
      if( lepton ) {
	dg::fillh( "lepton_p" , 500 , 0 , 500 , lepton->p() , "W Lepton p (GeV/c)" );
        dg::fillh( "lepton_pt" , 500 , 0 , 500 , lepton->pt() , "W Lepton p_{T} (GeV/c)" );
	dg::fillh( "lepton_invpt" , 200 , 5e-3 , 5e-1 , 1.0/lepton->pt() , "W Lepton 1/p_{T} (GeV^{-1})" ); 
        dg::fillh( "lepton_absphi" , 100 , 0 , M_PI , fabs( lepton->phi() ) , "W Lepton |#phi| (rad)" );
        dg::fillh( "lepton_phi" , 100 , -M_PI , M_PI , lepton->phi() , "W Lepton #phi (rad)" );
	dg::fillh( "lepton_eta" , 100 , -8 , 8 , lepton->eta() , "W Lepton #eta" );
	dg::fillh( "lepton_abseta" , 100 , 0 , 8 , fabs(lepton->eta()) , "W Lepton |#eta|" );
	if( _do_2d_plots ) dg::fillh( "lepton_phi_vs_eta" , 100 , -3 , 3 , 100, -M_PI, M_PI, lepton->eta() , lepton->phi(), "W Lepton #eta", "W Lepton #phi");
	if( met ) {
	  if( _do_2d_plots ) dg::fillh( "lepton_pt_vs_met" , 200 , 0, 200, 200, 0 , 200 , met->pt(), lepton->pt() , "MET", "W Lepton #pt" );
	  dg::fillh( "dphi_lepton_met" , 100 , 0 , M_PI , fabs( lepton->dist_phi( met ) ) , "#Delta #phi Lepton - MET (rad)" );
	  dg::fillh( "deta_lepton_met" , 100 , -5 , 5 , lepton->dist_eta( wevent->met_eta( wevent->met_pz_vlq() ) ) , "#Delta #eta Lepton - MET" );
	  dg::fillh( "dr_lepton_met" , 100 , 0 , 5 , lepton->dist_eta_phi( wevent->met_eta( wevent->met_pz_vlq() ) , met->phi() ) , "#Delta r(#eta,#phi) Lepton - MET" );
	  dg::fillh( "balance_vecsumphi_lepton" , 100 , -M_PI , M_PI , detector::delta_phi(event_vecsumphi,lepton->phi()) , "Event Vector #Sigma p_{T} #phi - Lepton #phi (rad)" );
	  dg::fillh( "balance_vecsumphi_met" , 100 , -M_PI , M_PI , detector::delta_phi(event_vecsumphi,met->phi()) , "Event Vector #Sigma p_{T} #phi - MET #phi (rad)" );
          if( _do_2d_plots ) dg::fillh( "met_vs_dphi_lepton_met", 100, -M_PI, M_PI, 200, 0 , 200 , lepton->dist_phi( met ),
					met->pt() , "#Delta #phi Lepton - MET (rad)" , "MET [GeV]" );
	} // end lepton-met plots
	if( muon && !_do_heavyQ_studies ) {
	  dg::fillh( "exPV_z0" ,200, -100 , 100 , muon->exPV_z0() ,"W muon z0 wrt PV" );
	  dg::fillh( "exPV_d0" ,200, -100 , 100 , muon->exPV_d0() ,"W muon d0 signifiance wrt PV" );
	  dg::fillh( "exPV_d0_sig" ,200, -100 , 100 , muon->exPV_d0_signif() ,"W muon d0 signifiance wrt PV" );
	  dg::fillh( "lepton_ptiso40r" ,200, 0 , 1.0 , muon->pt_cone_ratio(AnaMuon::CONE40) ,"W Lepton ptcone40" );
	  dg::fillh( "lepton_ptiso30r" ,200, 0 , 1.0 , muon->pt_cone_ratio(AnaMuon::CONE30) ,"W Lepton ptcone30" );
	  dg::fillh( "lepton_ptiso20r" ,200, 0 , 1.0 , muon->pt_cone_ratio(AnaMuon::CONE20) ,"W Lepton ptcone20" );
	  dg::fillh( "lepton_etiso40r" ,200, 0 , 1.0 , muon->et_cone_ratio(AnaMuon::CONE40) ,"W Lepton ptcone40" );
	  dg::fillh( "lepton_etiso30r" ,200, 0 , 1.0 , muon->et_cone_ratio(AnaMuon::CONE30) ,"W Lepton ptcone30" );
	  dg::fillh( "lepton_etiso20r" ,200, 0 , 1.0 , muon->et_cone_ratio(AnaMuon::CONE20) ,"W Lepton ptcone20" );
	  dg::fillh( "lepton_ptiso40" ,200, 0 , 20.0 , muon->pt_cone(AnaMuon::CONE40) ,"W Lepton ptcone40" );
	  dg::fillh( "lepton_ptiso30" ,200, 0 , 20.0 , muon->pt_cone(AnaMuon::CONE30) ,"W Lepton ptcone30" );
	  dg::fillh( "lepton_ptiso20" ,200, 0 , 20.0 , muon->pt_cone(AnaMuon::CONE20) ,"W Lepton ptcone20" );
	  dg::fillh( "lepton_etiso40" ,200, 0 , 20.0 , muon->et_cone(AnaMuon::CONE40) ,"W Lepton ptcone40" );
	  dg::fillh( "lepton_etiso30" ,200, 0 , 20.0 , muon->et_cone(AnaMuon::CONE30) ,"W Lepton ptcone30" );
	  dg::fillh( "lepton_etiso20" ,200, 0 , 20.0 , muon->et_cone(AnaMuon::CONE20) ,"W Lepton ptcone20" );
	  if( _do_2d_plots ) 
	    dg::fillh( "lepton_iso40_vs_pt", 200, 0, 200, 200, 0, 1, muon->pt(), muon->pt_cone_ratio(AnaMuon::CONE40), "W Lepton pt", "W Lepton ptcone40" );
	  if( met ) {
	    if( _do_2d_plots )
	      dg::fillh( "lepton_iso40_vs_met" , 200 , 0 , 200 , 200, 0 , 1.0 , met->pt(), muon->pt_cone_ratio(AnaMuon::CONE40) , "MET", "W Lepton ptcone40" );
	  } // end muon-met plots
	} // end muon plots
      } // end lepton plots
    }
    // plot the yield if flag is set to true
    if( _do_yield ) {
      dg::fillhw( "yield" , 
		  int( _yield_max - _yield_min ) , _yield_min , _yield_max ,
		  _yield_runnumber , dg::global_weight() / _yield_lumi , "run number" );
    }
    // plot each of the scale factors that are initialized
    std::vector< std::string >::iterator iname = _scalefactor_names.begin() , fname = _scalefactor_names.end();
    std::vector< std::pair<float,float> >::iterator ipair = _scalefactor_pairs.begin();
    std::vector< float >::iterator iweight = _scalefactor_weights.begin();
    for( ; iname!=fname ; ++iname , ++ipair , ++iweight ) {
      if( false ) std::cout << "DEBUG scale factor: " << (*iname) << " , " << (*ipair).first << " , " << (*ipair).second << std::endl;
      dg::fillhw( (*iname) , 500 , 0 , 5 , (*ipair).first , (*iweight) );
      std::string name_uncert = (*iname);
      name_uncert.append( "_uncert" );
      dg::fillhw( name_uncert , 1000 , 0 , 1 , (*ipair).second , (*iweight) );
    }
    // plot heavy Q transverse mass (including leading jet pt)
    if( _do_heavyQ_studies ) {
      //dg::fillhw( "event_number" , 20000 , 0 , 20000 , wevent->event_number , 1. );
      dg::fillhw( "global_weight" , 1000 , 0 , 10 , dg::global_weight() , 1. );
      dg::fillh( "total" , 1 , 1 , 2 , 1 );
      dg::fillhw( "total_raw" , 1 , 1 , 2 , 1 , 1. );
      //dg::fillh( "bdt_response_400" , 200 , -2 , 2 , wevent->bdt_response("BDT method W 400") , "BDT response 400" );
      //dg::fillh( "bdt_response_900" , 200 , -2 , 2 , wevent->bdt_response("BDT method W 900") , "BDT response 900" );
      //dg::fillh( "bdt_response_1200" , 200 , -2 , 2 , wevent->bdt_response("BDT method W 1200") , "BDT response 1200" );
      //dg::fillh( "w_bdt_eta" , 100 , -8. , 8. , wevent->w_bdt_eta() , "W #eta (BDT)" );
      dg::fillh( "w_eta" , 100 , -8. , 8. , wevent->w_eta() , "W #eta" );
      dg::fillh( "nvtxs" , 50 , 0 , 50 , wevent->nvtxs() , "N_{vtxs}" );
      dg::fillhw( "nvtxs_unweighted" , 20 , 0 , 20 , wevent->nvtxs() , 1. , "N_{vtxs}" );
      if( wevent->njets()>0 ) {
        dg::fillh( "jets_m" , 500 , 0 , 500 , wevent->jets_m() , "Invariant Mass of Jets (GeV/c^{2})" );
      }
      if( wevent->njets()>1 ) {
	dg::fillh( "associated_jet_id" , 10 , 0 , 10 , wevent->associated_jet_id() );
	dg::fillh( "far_jet_id" , 10 , 0 , 10 , wevent->far_jet_id() );
	if( wevent->nfjtruth()==1 ) {
	  dg::fillh( "truth_matched_jet_id" , 10 , 0 , 10 , wevent->truth_matched_jet_id() );
	  dg::fillh( "farjet_eta_resolution" , 500 , -10 , 10 , wevent->far_jet()->eta() - wevent->truth_forwardjet()->eta() );
	  dg::fillh( "farjet_pt_resolution" , 500 , -100 , 100 , wevent->far_jet()->pt() - wevent->truth_forwardjet()->pt() );
	  dg::fillh( "associatedjet_eta_resolution" , 500 , -10 , 10 , wevent->associated_jet()->eta() - wevent->truth_forwardjet()->eta() );
	  dg::fillh( "associatedjet_pt_resolution" , 500 , -100 , 100 , wevent->associated_jet()->pt() - wevent->truth_forwardjet()->pt() );
	  dg::fillh( "deta_leadingjet_truthforwardjet" , 200 , -8 , 8 , wevent->leading_jet()->eta() - wevent->truth_forwardjet()->eta() );
	  dg::fillh( "total_jet" , 1 , 0 , 1 , 0 );
	  if( wevent->associatedjet_is_correct() ) dg::fillh( "total_jet_associated_correct" , 1 , 0 , 1 , 0 );
	  if( wevent->farjet_is_correct() ) dg::fillh( "total_jet_far_correct" , 1 , 0 , 1 , 0 );
	}
      }
      if( lepton && met ) {
        dg::fillh( "event_mt" , 2000 , 0 , 2000 , wevent->event_mt() , "Transverse Mass of Lepton, MET, and Jets (GeV/c^{2})" );
        dg::fillh( "event_m" , 2000 , 0 , 2000 , wevent->event_m() , "Invariant Mass of Lepton, MET, and Jets (GeV/c^{2})" );
        dg::fillh( "w_m" , 500 , 0 , 500 , wevent->w_m() , "Invariant Mass of W (GeV/c^{2})" );
	dg::fillh( "npzsolutions" , 2 , 0 , 2 , wevent->npzsolutions() );
      }
      if( wevent->njets()>0 && lepton && met ) {
        dg::fillh( "Q_mt" , 2500 , 0 , 2500 , wevent->heavyQ_mt() , "Heavy Q Transverse Mass (GeV/c^{2})" );
	dg::fillh( "Q_m" , 2500 , 0 , 2500 , wevent->heavyQ_m() , "Heavy Q Invariant Mass (GeV/c^{2})" );
	dg::fillh( "Q_m_low" , 2500 , 0 , 2500 , wevent->heavyQ_m_low() , "Heavy Q Invariant Mass (low solution)" );
	dg::fillh( "Q_m_high" , 2500 , 0 , 2500 , wevent->heavyQ_m_high() , "Heavy Q Invariant Mass (high solution)" );
	dg::fillh( "deta_w_leadingjet" , 200 , -8 , 8 , wevent->deta_w_leadingjet() );
	dg::fillh( "dphi_w_leadingjet" , 200 , -M_PI , M_PI , wevent->dphi_w_leadingjet() );
	if( wevent->nvlqtruth()==1 ) {
	  dg::fillh( "truth_Q_m" , 2500 , 0 , 2500 , wevent->truth_vlq_m() , "TRUTH VLQ mass" );
	}
	if( wevent->nwtruth()==1 ) {
	  dg::fillh( "Q_m_matched" , 2500 , 0 , 2500 , wevent->heavyQ_m_matched() , "Heavy Q Invariant Mass (matched solution)" );
	  dg::fillh( "Q_m_resolution_matched" , 1000 , -500 , 500 , wevent->heavyQ_m_matched() - wevent->truth_vlq_m() , "RECO - TRUTH VLQ Mass (matched) [GeV]" );
	  dg::fillh( "Q_m_resolution" , 1000 , -500 , 500 , wevent->heavyQ_m() - wevent->truth_vlq_m() , "RECO - TRUTH VLQ Mass (#Delta#eta solution) [GeV]" );
	  dg::fillh( "Q_m_resolution_low" , 1000 , -500 , 500 , wevent->heavyQ_m_low() - wevent->truth_vlq_m() , "RECO - TRUTH VLQ Mass (low solution) [GeV]" );
	  dg::fillh( "Q_m_resolution_high" , 1000 , -500 , 500 , wevent->heavyQ_m_high() - wevent->truth_vlq_m() , "RECO - TRUTH VLQ Mass (low solution) [GeV]" );  
	  if( wevent->npzsolutions()==2 ) {
	    dg::fillh( "total_2pz" , 1 , 0 , 1 , 0 );
	    if( wevent->low_pz_solution_is_correct() ) dg::fillh( "total_2pz_low_correct" , 1 , 0 , 1 , 0 );
	    if( wevent->high_pz_solution_is_correct() ) dg::fillh( "total_2pz_high_correct" , 1 , 0 , 1 , 0 );
	    if( wevent->vlq_pz_solution_is_correct() ) dg::fillh( "total_2pz_vlq_correct" , 1 , 0 , 1 , 0 );
	  }
	}
      }
      if( wevent->njets()>1 && lepton && met ) {
        dg::fillh( "wjj_mt" , 100 , 0 , 500 , wevent->wjj_mt() , "Transverse Mass of W + 2 Hardest Jets (GeV/c^{2})" );
        dg::fillh( "wjj_m" , 100 , 0 , 500 , wevent->wjj_m() , "Invariant Mass of W + 2 Hardest Jets (GeV/c^{2})" );
	dg::fillh( "deta_w_associatedjet" , 200 , -8 , 8 , wevent->deta_w_associatedjet() );
	dg::fillh( "deta_w_farjet" , 200 , -8 , 8 , wevent->deta_w_farjet() );
      }
      if( wevent->njets()>0 && lepton ) {
        dg::fillh( "dphi_lepton_jet0" , 100 , 0 , M_PI , fabs( lepton->dist_phi( *(jets_by_pt.begin()) ) ) , "#Delta #phi Lepton - Leading Jet (rad)" );
        dg::fillh( "deta_lepton_jet0" , 100 , -8 , 8 , lepton->dist_eta( *(jets_by_pt.begin()) ) , "#Delta #eta Lepton - Leading Jet" );
        dg::fillh( "dr_lepton_jet0" , 50 , 0 , 5 , lepton->dist_eta_phi( *(jets_by_pt.begin()) ) , "#Delta r(#eta,#phi) Lepton - Leading Jet" );
      }
      if( wevent->njets()>0 && met ) {
        dg::fillh( "dphi_met_jet0" , 100 , 0 , M_PI , fabs( (*(jets_by_pt.begin()))->dist_phi( met ) ) , "#Delta #phi MET - Leading Jet (rad)" );
        dg::fillh( "deta_met_jet0" , 100 , -8 , 8 , wevent->leading_jet()->eta() - wevent->met_eta( wevent->met_pz_vlq() ) , "#Delta #eta MET - Leading Jet" );
        dg::fillh( "dr_met_jet0" , 50 , 0 , 5 , (wevent->leading_jet())->dist_eta_phi( wevent->met_eta( wevent->met_pz_vlq() ) , met->phi() ) , "#Delta r(#eta,#phi) MET - Leading Jet" );
        if( _do_2d_plots ) {
	  dg::fillh( "jet0_pt_vs_met" , 100 , 0, 200, 100, 0 , 200 , met->pt(), (*(jets_by_pt.begin()))->pt() , "MET [GeV]", "Leading Jet p_{T} [GeV]" );
	  if( lepton ) {
	    dg::fillh( "jet0_pt_vs_dphi_lepton_met", 48, -M_PI, M_PI, 100, 0 , 200 , lepton->dist_phi( met ),
		       (*(jets_by_pt.begin()))->pt() , "#Delta #phi Lepton - MET (rad)" , "Leading Jet p_{T} [GeV]" );
	  }
	}
      }
      if( wevent->njets()>1 ) {
        dg::fillh( "dijet_m" , 500 , 0  , 500 , wevent->dijet_m() , "Invariant Mass of 2 Hardest Jets (GeV/c^{2})" );
	dg::fillh( "dphi_leadingjet_associatedjet" , 200 , -M_PI , M_PI , wevent->dphi_leadingjet_associatedjet() );
	dg::fillh( "deta_leadingjet_associatedjet" , 200 , -8 , 8 , wevent->deta_leadingjet_associatedjet() );
	dg::fillh( "dphi_leadingjet_farjet" , 200 , -M_PI , M_PI , wevent->dphi_leadingjet_farjet() );
	dg::fillh( "deta_leadingjet_farjet" , 200 , -8 , 8 , wevent->deta_leadingjet_farjet() );
	dg::fillh( "associated_jet_pt" , 500 , 0 , 500 , wevent->associated_jet()->pt() );
	dg::fillh( "associated_jet_eta" , 100 , -8 , 8 , wevent->associated_jet()->eta() );
	dg::fillh( "associated_jet_phi" , 100 , -M_PI , M_PI , wevent->associated_jet()->phi() );
	dg::fillh( "far_jet_pt" , 500 , 0 , 500 , wevent->far_jet()->pt() );
	dg::fillh( "far_jet_eta" , 100 , -8 , 8 , wevent->far_jet()->eta() );
	dg::fillh( "far_jet_phi" , 100 , -M_PI , M_PI , wevent->far_jet()->phi() );
      }
      if( _do_xcheck_studies && wevent->njets()>1 ) {
        dg::fillh( "xcheck_softjet_pt" , 500 , 0 , 500 , wevent->subleading_jet()->pt() );
      }
    }
    if( true ) {
      float min_dR = std::numeric_limits<float>::max();
      for( short i = 0 ; i < wevent->njets() ; ++i ) {
	std::string istr = boost::lexical_cast<std::string>(i);
	dg::fillh( "jet"+istr+"_pt" , 500 , 0 , 500 , wevent->jet(i)->pt() );
	dg::fillh( "jet"+istr+"_phi" , 100 , -M_PI , M_PI , wevent->jet(i)->phi() );
	dg::fillh( "jet"+istr+"_eta" , 100 , -5 , 5 , wevent->jet(i)->eta() );
	if( !_do_heavyQ_studies ) {
	  if( i==0 ) { dg::fillh( "balance_vecsumphi_leading_jet" , 100 , -M_PI , M_PI , 
				  detector::delta_phi(event_vecsumphi,wevent->jet(i)->phi()) , "Event Vector #Sigma p_{T} #phi - Leading Jet #phi (rad)" ); }
	  float tmp_dR = lepton->dist_eta_phi( wevent->jet(i) );
	  if( tmp_dR < min_dR ) { min_dR = tmp_dR; }
	}
      }
      // plot delta R between lepton and nearest jet
      if( !_do_heavyQ_studies ) dg::fillh( "dr_lepton_jet" , 100 , 0 , 5 , min_dR , "#Delta r(#eta,#phi) Lepton - Nearest Jet" );
    }
    if( true && met && lepton && !_do_heavyQ_studies ) { 
      // Get delta-phi between met and nearest jet/lepton
      float min_dPhi = std::numeric_limits<float>::max();
      // study ith-highest pt jet(s)
      unsigned int ithjet = 0u;
      for( vector< boost::shared_ptr<const AnaJet> >::iterator ijet=jets_by_pt.begin(), fjet=jets_by_pt.end(); ijet!=fjet; ++ijet, ++ithjet ) {
        const boost::shared_ptr<const AnaJet>& j( *ijet );
	float tmp_dPhi = fabs(met->dist_phi( j ));
	if( tmp_dPhi < min_dPhi ) { min_dPhi = tmp_dPhi; }
      }
      float tmp_dPhi = fabs(met->dist_phi( lepton ));
      if( tmp_dPhi < min_dPhi ) { min_dPhi = tmp_dPhi; }
      dg::fillh( "dphi_met_leptonjet" , 100 , 0 , M_PI , min_dPhi , "#Delta #phi MET - Nearest Lepton/Jet" );
      float metrel=met->pt();
      if (min_dPhi < M_PI/2) { metrel = met->pt() * sin(min_dPhi); }
      dg::fillh( "met_rel" , 200 , 0 , 200 , metrel , "W Missing ET Projection(GeV)" );
    }
    if( false ) { 
      dg::down( "jet_loweta" , "ith-lowest eta jet(s)" ); BOOST_SCOPE_EXIT(void) { dg::up(); } BOOST_SCOPE_EXIT_END;
      // study ith-lowest eta jet(s)
      vector< boost::shared_ptr<const AnaJet> > jets_by_eta( wevent->begin_jets() , wevent->end_jets() );
      std::sort( jets_by_eta.begin() , jets_by_eta.end() , 
                 bind( ptr_fun<AnaJet::ftype,AnaJet::ftype>(abs) , bind(&AnaJet::eta,_1) ) <
                 bind( ptr_fun<AnaJet::ftype,AnaJet::ftype>(abs) , bind(&AnaJet::eta,_2) ) );
      unsigned int ithjet = 0u;
      for( vector< boost::shared_ptr<const AnaJet> >::iterator ijet=jets_by_eta.begin(), fjet=jets_by_eta.end(); ijet!=fjet; ++ijet, ++ithjet ) {
        const boost::shared_ptr<const AnaJet>& j( *ijet );
        dg::fillh( (format("jet_%|d|theta_pt") % ithjet ).str() , 500 , 0 , 500 , j->pt() );
        dg::fillh( (format("jet_%|d|theta_eta") % ithjet ).str() , 100 , -8 , 8 , j->eta() );
      }
    }
  } // end study W cross section event candidate
protected:
  boost::shared_ptr<const AnaEvent> _event;
  bool _do_heavyQ_studies;
  bool _do_xcheck_studies;
  bool _do_2d_plots;
  bool _do_charge_separation;
  bool _do_eta_separation;
  bool _do_yield;
  unsigned long _yield_min;
  unsigned long _yield_max;
  unsigned long _yield_runnumber;
  float _yield_lumi;
  unsigned long _run_number;
  std::vector< std::string > _scalefactor_names;
  std::vector< std::pair<float,float> > _scalefactor_pairs;
  std::vector< float > _scalefactor_weights;
public:
  void do_heavyQ_studies( bool v = true ) { _do_heavyQ_studies = v; }
  void do_xcheck_studies( bool v = true ) { _do_xcheck_studies = v; }
  void do_2d_plots( bool v = true ) { _do_2d_plots = v; }
  void do_charge_separation( bool v = true ) { _do_charge_separation = v; }
  void do_eta_separation( bool v = true ) { _do_eta_separation = v; }
  void do_yield(bool v = true) { _do_yield = v; }
  void do_yield( const unsigned long& min , const unsigned long& max , const unsigned long& runnumber , const float& lumi ) { 
    _do_yield = true; _yield_min = min ; _yield_max = max ; _yield_runnumber = runnumber; _yield_lumi = lumi; 
  }
  void plot_scale_factor( std::string name , std::pair<float,float> value , float weight = 1. ) { 
    _scalefactor_names.push_back( name ); _scalefactor_pairs.push_back( value ); _scalefactor_weights.push_back( weight ); 
  }
  StudyWCrossSection( const std::string& name , const std::string& description ) 
    : _event()
    , _do_heavyQ_studies(false)
    , _do_xcheck_studies(false)
    , _do_2d_plots(true)
    , _do_charge_separation(false)
    , _do_eta_separation(false)
    , _do_yield(false)
    , _yield_min( 0. )
    , _yield_max( 1. )
    , _yield_runnumber( 0. )
    , _yield_lumi( 1. )
    , _run_number(0)
    , DataGraphics::DgStudy<type>(name,description)
  {
  }
  virtual ~StudyWCrossSection() {}
  void for_event( const boost::shared_ptr<const AnaEvent>& event ) { _event = event; }
  void for_run( unsigned long rnum ) { _run_number = rnum; }
};

#endif // STUDYWCROSSECTION_HPP
