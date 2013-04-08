 
#ifndef STUDYWPRIME_HPP
#define STUDYWPRIME_HPP

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
#include "TrigFTKAna/AnaWprimeCandidate.hpp"
#include "TrigFTKAna/AnaParticle.hpp"
#include "TrigFTKAna/AnaMET.hpp"
#include "TrigFTKAna/AnaW.hpp"
#include "TrigFTKAna/AnaMuon.hpp"
#include "TrigFTKAna/AnaElectron.hpp"
#include "TrigFTKAna/Detector.hpp"


class
StudyWprime : public DataGraphics::DgStudy< boost::shared_ptr<const AnaWprimeCandidate> >
{
public:
  typedef boost::shared_ptr<const AnaWprimeCandidate> type; // must be same type as the template argument for DgStudy.
protected:
  // a wrapper implementing (optional) charge separation
  virtual void _study( type const& wevent ) {
    using namespace DataGraphics;
    study_do(wevent);
    if(_do_charge_separation) {
      const bool save_ntuple = _do_save_ntuple;
      _do_save_ntuple = false;
      if(wevent->scharge()>0) {
	dg::down( "POS" , "W+ candidates" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
	study_do(wevent);
      }
      if(wevent->scharge()<0) {
	dg::down( "NEG" , "W- candidates" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
	study_do(wevent);
      }
      _do_save_ntuple = save_ntuple;
    }
  }
  // actual study
  virtual void study_do( type const& wevent ) {
    using namespace std;
    using namespace boost;
    using namespace DataGraphics;
    const boost::shared_ptr<const AnaWprimeCandidate>& w = wevent;
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
    dg::fillh( "w_mt" , 40 , 0 , 200 , w->transverse_mass() , "W Transverse Mass (GeV/c^{2})" );
    dg::fillh( "w_pt" , 160 , 0 , 800 , w->pt() , "W Transverse Momentum (GeV/c)" );
    dg::fillh( "njets" , 10 , 0 , 10 , wevent->njets() , "N_{jets}" );
    dg::fillh( "nbjets" , 10 , 0 , 10 , wevent->nbjets() , "N_{b jets}" );
    // vector and scaler sums
    //dg::fillh( "jet_sumpt" , 200 , 0 , 200 , jet_sumpt , "Jet #Sigma p_{T} (GeV)" );
    //dg::fillh( "jet_vecsumpt" , 100 , 0 , 100 , jet_vecsumpt , "Jet Vector #Sigma p_{T} Magnitude (GeV)" );
    //dg::fillh( "wjets_vecsumpt" , 500 , 0 , 500 , event_vecsumpt , "Event Vector #Sigma p_{T} Magnitude (GeV)" );
    //dg::fillh( "wjets_sumpt" , 2000 , 0 , 2000 , wjets_sumpt , "Event Scaler #Sigma p_{T} (GeV)" );
    if( lepton ) {
      const AnaParticle::ftype ljets_sumpt = lepton->pt() + jet_sumpt;
      const AnaParticle::ftype ljets_vecsumpt = detector::quadrature( lepton->px() + jet_sumpx ,
                                                                      lepton->py() + jet_sumpy );
      //dg::fillh( "ljets_sumpt" , 2000 , 0 , 2000 , ljets_sumpt , "Lepton and Jets Scaler #Sigma p_{T} (GeV)" );
      //dg::fillh( "ljets_vecsumpt" , 1000 , 0 , 1000 , ljets_vecsumpt , "Lepton and Jets Vector #Sigma p_{T} Magnitude (GeV)" );
      if( met ) {
        const AnaParticle::ftype lvjets_sumpt = lepton->pt() + met->pt() + jet_sumpt;
        const AnaParticle::ftype lvjets_vecsumpt = detector::quadrature( lepton->px() + met->px() + jet_sumpx ,
  								         lepton->py() + met->py() + jet_sumpy );
        //dg::fillh( "lvjets_sumpt" , 2000 , 0 , 2000 , lvjets_sumpt , "Lepton, MET, and Jet Scaler #Sigma p_{T} (GeV)" );
        //dg::fillh( "lvjets_vecsumpt" , 1000 , 0 , 1000 , lvjets_vecsumpt , "Lepton, MET, and Jet Vector #Sigma p_{T} Magnitude (GeV)" );
      }
      //if( (_do_2d_plots || _do_qcd_2d) && (_do_save_ntuple==0) ) dg::fillh( "lepton_pt_vs_eta" , 100,0,500,200,-3,3,lepton->pt(),lepton->eta(),"lepton P_{T} (GeV)", "lepton #eta" );
      //if( _do_2d_plots ) dg::fillh( "lepton_pt_vs_abs_eta" , 100,0,500,100,0,3,lepton->pt(),fabs(lepton->eta()),"lepton P_{T} (GeV)", "lepton |#eta|" );
    }
    if( true ) { 
      if( met ) {
	dg::fillh( "met" , 100 , 0 , 500 , met->pt() , "W Missing ET (GeV)" );
        dg::fillh( "met_phi", 200 , -M_PI , M_PI , met->phi() , "MET #phi (rad)" );
        dg::fillh( "met_eta", 200 , -3 , 3 , wevent->met_eta( wevent->met_preferred_pz() ) , "MET #eta" );
	dg::fillh( "met_plus_wmt", 100 , 0 , 500 , (met->pt())+(w->transverse_mass()) , "MET + W tranverse mass (GeV/c^{2})" );
	if( _do_2d_plots ) dg::fillh( "wmt_vs_met" , 200,0,200,200,0,200,met->pt(),w->transverse_mass(),"MET", "W Transverse Mass (GeV/c^{2})" );
      } // end met plots
      if( lepton ) {
	dg::fillh( "lepton_p" , 500 , 0 , 500 , lepton->p() , "W Lepton p (GeV/c)" );
        dg::fillh( "lepton_pt" , 100 , 0 , 500 , lepton->pt() , "W Lepton p_{T} (GeV/c)" );
	dg::fillh( "lepton_invpt" , 200 , 5e-3 , 5e-1 , 1.0/lepton->pt() , "W Lepton 1/p_{T} (GeV^{-1})" ); 
        dg::fillh( "lepton_phi" , 200 , -M_PI , M_PI , lepton->phi() , "W Lepton #phi (rad)" );
	dg::fillh( "lepton_eta" , 200 , -3 , 3 , lepton->eta() , "W Lepton #eta" );

	// TNtuple for unbinned likelihood
	if(muon && met && _do_save_ntuple && uncategorized()) {
	  std::vector<std::string> names = boost::assign::list_of("l_q")("l_pt")("l_eta")("l_phi")("w_mt")("njets")("nbjets");
	  std::vector<float> values = boost::assign::list_of(w->scharge()*1.0)(lepton->pt())(lepton->eta())(lepton->phi())(w->transverse_mass())(w->njets()*1.0)(w->nbjets()*1.0);
	  names.push_back("ptiso40");
	  names.push_back("ptiso30");
	  names.push_back("ptiso20");
	  names.push_back("etiso40");
	  names.push_back("etiso30");
	  names.push_back("etiso20");
	  values.push_back( muon->pt_cone(AnaMuon::CONE40) );
	  values.push_back( muon->pt_cone(AnaMuon::CONE30) );
	  values.push_back( muon->pt_cone(AnaMuon::CONE20) );
	  values.push_back( muon->et_cone(AnaMuon::CONE40) );
	  values.push_back( muon->et_cone(AnaMuon::CONE30) );
	  values.push_back( muon->et_cone(AnaMuon::CONE20) );

	  names.push_back("met");
	  names.push_back("met_phi");
	  names.push_back("dphi_lepton_met");
	  values.push_back(met->pt());
	  values.push_back(met->phi());
	  values.push_back(fabs( lepton->dist_phi(met) ));
	  // W boson m, eta, phi - using prefferred neutrino pz guess
	  if(true) {
	    names.push_back("w_m");
	    names.push_back("w_eta");
	    names.push_back("w_phi");
	    values.push_back(w->w_m());
	    values.push_back(w->w_eta());
	    values.push_back(w->w_phi());
	  }
	  names.push_back("st");
	  names.push_back("ht");
	  names.push_back("wprime_mt");
	  names.push_back("wprime_m");
	  if(w->njets()>1) {
	    values.push_back(w->st());
	    values.push_back(w->ht());
	    values.push_back(w->wjj_mt());
	    values.push_back(w->wjj_m());
	  }
	  else {
	    values.push_back(-999);
	    values.push_back(-999);
	    values.push_back(-999);
	    values.push_back(-999);
	  }
	  names.push_back("0thjet_pt");
	  names.push_back("0thjet_eta");
	  names.push_back("0thjet_phi");
	  if(w->njets()>0) {
	    values.push_back(w->leading_jet()->pt());
	    values.push_back(w->leading_jet()->eta());
	    values.push_back(w->leading_jet()->phi());
	  }
	  else {
	    values.push_back(-999);
	    values.push_back(-999);
	    values.push_back(-999);
	  }
	  names.push_back("1thjet_pt");
	  names.push_back("1thjet_eta");
	  names.push_back("1thjet_phi");
	  if(w->njets()>1) {
	    values.push_back(w->subleading_jet()->pt());
	    values.push_back(w->subleading_jet()->eta());
	    values.push_back(w->subleading_jet()->phi());
	  }
	  else {
	    values.push_back(-999);
	    values.push_back(-999);
	    values.push_back(-999);
	  }
	  dg::fillnt("ntuple_mu",values,names);
	}


	if( _do_2d_plots ) dg::fillh( "lepton_phi_vs_eta" , 100 , -3 , 3 , 200, -M_PI, M_PI, lepton->eta() , lepton->phi(), "W Lepton #eta", "W Lepton #phi");
	if(_do_asymmetry_studies) {
	  dg::fillh( "asym_eta" , 200 , -3 , 3 , lepton->eta() , "W Lepton #eta" );
	  dg::fillh( "asym_abseta" , 100 , 0 , 3 , fabs(lepton->eta()) , "W Lepton |#eta|" );
	  if(fabs(lepton->eta())<1.05) {
	    dg::fillh( "lepton_phi_barrel" , 200 , -M_PI , M_PI , lepton->phi() , "W Lepton barrel #phi (rad)" );
	  }
	  else {
	    dg::fillh( "lepton_phi_endcap" , 200 , -M_PI , M_PI , lepton->phi() , "W Lepton endcap #phi (rad)" );
	  }
	}
	if( _do_2d_plots ) {
	  if( lepton->dist_eta_phi( *(jets_by_pt.begin())) > 0.5) {
	    dg::fillh( "dr05_lepton_pt" , 100 , 0 , 500 , lepton->pt() , "lepton P_{T} (GeV)" );
	    dg::fillh( "dr05_lepton_eta" , 200 , -3 , 3 , lepton->eta() , "lepton #eta" );
	  }
	}
	if( lepton && met ) {
	  if( (_do_2d_plots||_do_qcd_2d) && (_do_save_ntuple==0) ) {
	    dg::fillh( "lepton_pt_vs_met" , 100 , 0, 500, 100, 0 , 500 , met->pt(), lepton->pt() , "MET", "W Lepton #pt" );
	    dg::fillh( "lepton_eta_vs_met" , 200 , -3 , 3 , 100, 0 , 500 , lepton->eta() , met->pt() , "lepton #eta" , "MET (GeV)" );
	  }
          if( _do_heavyQ_studies ) dg::fillh( "dphi_lepton_met" , 100 , 0 , M_PI , fabs( lepton->dist_phi( met ) ) , "#Delta #phi Lepton - MET (rad)" );
	  else dg::fillh( "dphi_lepton_met" , 200 , -M_PI , M_PI , lepton->dist_phi( met ) , "#Delta #phi Lepton - MET (rad)" );
	  dg::fillh( "deta_lepton_met" , 100 , -5 , 5 , lepton->dist_eta( wevent->met_eta( wevent->met_preferred_pz() ) ) , "#Delta #eta Lepton - MET" );
	  //dg::fillh( "dr_lepton_met" , 100 , 0 , 5 , lepton->dist_eta_phi( wevent->met_eta( wevent->met_preferred_pz() ) , met->phi() ) , "#Delta r(#eta,#phi) Lepton - MET" );
	  //dg::fillh( "balance_vecsumphi_lepton" , 200 , -M_PI , M_PI , detector::delta_phi(event_vecsumphi,lepton->phi()) , "Event Vector #Sigma p_{T} #phi - Lepton #phi (rad)" );
	  //dg::fillh( "balance_vecsumphi_met" , 200 , -M_PI , M_PI , detector::delta_phi(event_vecsumphi,met->phi()) , "Event Vector #Sigma p_{T} #phi - MET #phi (rad)" );
          if( _do_2d_plots ) dg::fillh( "met_vs_dphi_lepton_met", 200, -M_PI, M_PI, 200, 0 , 200 , lepton->dist_phi( met ),
					met->pt() , "#Delta #phi Lepton - MET (rad)" , "MET [GeV]" );
	} // end lepton-met plots
	if( muon ) {
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
    // plot W' transverse mass (including leading jet pt)
    if( _do_heavyQ_studies ) {
      if( _do_yield ) {
	dg::fillhw( "yield" , int( _yield_max - _yield_min ) , _yield_min , _yield_max , _yield_runnumber , dg::global_weight() / _yield_lumi , "run number" );
      }
      dg::fillh( "w_eta" , 200 , -M_PI , M_PI , wevent->w_eta() , "W #eta" );
      dg::fillh( "nvtxs" , 20 , 0 , 20 , wevent->nvtxs() , "N_{vtxs}" );
      if( wevent->njets()>0 ) {
        dg::fillh( "jets_m" , 500 , 0 , 500 , wevent->jets_m() , "Invariant Mass of Jets (GeV/c^{2})" );
      }
      if( _do_qcd_2d && (_do_save_ntuple==0) ) {
	if( wevent->njets()>0 ) {
	  //dg::fillh( "njets_vs_leading_jet_pt" , 10,0,10, 300,0,1500, wevent->njets(),wevent->leading_jet()->pt(),"number of jets","leading jet P_{T} (GeV)");
	}
	if( wevent->njets()>0 && lepton ) {
	  //dg::fillh( "lepton_pt_vs_leading_jet_pt" , 100,0,500, 300,0,1500,lepton->pt(),wevent->leading_jet()->pt(),"lepton P_{T} (GeV)","leading jet P_{T} (GeV)");
	  dg::fillh( "lepton_eta_vs_leading_jet_pt" , 200,-3,3, 300,0,1500,lepton->eta(),wevent->leading_jet()->pt(),"lepton #eta","leading jet P_{T} (GeV)");
	  //dg::fillh( "lepton_pt_vs_leading_jet_eta" , 100,0,500, 200,-3,3,lepton->pt(),wevent->leading_jet()->eta(),"lepton P_{T} (GeV)","leading jet #eta");
	  dg::fillh( "lepton_eta_vs_leading_jet_eta" , 200,-3,3, 200,-3,3,lepton->eta(),wevent->leading_jet()->eta(),"lepton #eta","leading jet #eta");
	  //dg::fillh( "lepton_pt_vs_leading_jet_phi" , 100,0,500, 200,-M_PI,M_PI,lepton->pt(),wevent->leading_jet()->phi(),"lepton P_{T} (GeV)","leading jet #phi (rad)");
	  //dg::fillh( "lepton_eta_vs_leading_jet_phi" , 200,-3,3, 200,-M_PI,M_PI,lepton->eta(),wevent->leading_jet()->phi(),"lepton #eta","leading jet #phi (rad)");
	}
	if( wevent->njets()>1 && lepton ) {
	  //dg::fillh( "lepton_pt_vs_subleading_jet_pt" , 100,0,500, 300,0,1500,lepton->pt(),wevent->subleading_jet()->pt(),"lepton P_{T} (GeV)","subleading jet P_{T} (GeV)");
	  //dg::fillh( "lepton_eta_vs_subleading_jet_pt" , 200,-3,3, 300,0,1500,lepton->eta(),wevent->subleading_jet()->pt(),"lepton #eta","subleading jet P_{T} (GeV)");
	  //dg::fillh( "lepton_pt_vs_subleading_jet_eta" , 100,0,500, 200,-3,3,lepton->pt(),wevent->subleading_jet()->eta(),"lepton P_{T} (GeV)","subleading jet #eta");
	  // dg::fillh( "lepton_eta_vs_subleading_jet_eta" , 200,-3,3, 200,-3,3,lepton->eta(),wevent->subleading_jet()->eta(),"lepton #eta","subleading jet #eta");
	  //dg::fillh( "lepton_pt_vs_subleading_jet_phi" , 100,0,500, 200,-M_PI,M_PI,lepton->pt(),wevent->subleading_jet()->phi(),"lepton P_{T} (GeV)","subleading jet #phi (rad)");
	  //dg::fillh( "lepton_eta_vs_subleading_jet_phi" , 200,-3,3, 200,-M_PI,M_PI,lepton->eta(),wevent->subleading_jet()->phi(),"lepton #eta","subleading jet #phi (rad)");
	}
      }
      if( lepton && met ) {
        dg::fillh( "event_mt" , 2000 , 0 , 2000 , wevent->event_mt() , "Transverse Mass of Lepton, MET, and Jets (GeV/c^{2})" );
        dg::fillh( "event_m" , 2000 , 0 , 2000 , wevent->event_m() , "Invariant Mass of Lepton, MET, and Jets (GeV/c^{2})" );
        dg::fillh( "w_m" , 500 , 0 , 500 , wevent->w_m() , "Invariant Mass of W (GeV/c^{2})" );
      }
      if( wevent->njets()>0 && lepton && met ) {
        dg::fillh( "top_mt" , 300 , 0 , 300 , wevent->top_mt() , "Top transverse mass (GeV/c^{2})" );
	dg::fillh( "top_m" , 300 , 0 , 300 , wevent->top_m() , "Top invariant mass (GeV/c^{2})" );
        if( _do_xcheck_studies ) {
          boost::shared_ptr<const AnaW> heavyQ( new AnaW( lepton , met , wevent->leading_jet() ) );
          boost::shared_ptr<const AnaW> heavyQ2( new AnaW( lepton , met , *(jets_by_pt.begin()) ) );
          dg::fillh( "xcheck_Q_mt1" , 100 , 0 , 500 , heavyQ->mt() , "Heavy Q Transverse Mass (GeV/c^{2})" );
          dg::fillh( "xcheck_Q_mt2" , 100 , 0 , 500 , heavyQ2->mt() , "Heavy Q Transverse Mass (GeV/c^{2})" );
        }
      }
      if( wevent->njets()>1 && lepton && met ) {
        dg::fillh( "wprime_mt" , 600 , 0 , 3000 , wevent->wjj_mt() , "W' transverse mass (GeV/c^{2})" );
        dg::fillh( "wprime_m" , 600 , 0 , 3000 , wevent->wjj_m() , "W' invariant mass (GeV/c^{2})" );
	dg::fillh( "ht" , 600 , 0 , 3000 , wevent->ht() , "H_{T} (GeV)" );
	dg::fillh( "st" , 100 , 0.0 , 1.0 , wevent->st() , "S_{T}" );
	if( (_do_2d_plots||_do_qcd_2d) && (_do_save_ntuple==0) ) {
	  //dg::fillh( "lepton_pt_vs_wprime_mt" , 100,0,500, 600,0,3000,lepton->pt(),wevent->wjj_mt(),"lepton P_{T} (GeV)","W' transverse mass (GeV/c^{2})");
          dg::fillh( "lepton_eta_vs_wprime_mt" , 200,-3,3, 600,0,3000,lepton->eta(),wevent->wjj_mt(),"lepton #eta","W' transverse mass (GeV/c^{2})");
	  //dg::fillh( "lepton_pt_vs_wprime_m" , 100,0,500, 600,0,3000,lepton->pt(),wevent->wjj_m(),"lepton P_{T} (GeV)","W' mass (GeV/c^{2})");
          dg::fillh( "lepton_eta_vs_wprime_m" , 200,-3,3, 600,0,3000,lepton->eta(),wevent->wjj_m(),"lepton #eta","W' mass (GeV/c^{2})");	  
	  //dg::fillh( "lepton_pt_vs_st" , 100,0,500, 100,0.0,1.0,lepton->pt(),wevent->st(),"lepton P_{T} (GeV)","S_{T}" );
	  dg::fillh( "lepton_eta_vs_st" , 200,-3,3, 100,0.0,1.0,lepton->eta(),wevent->st(),"lepton #eta","S_{T}" );
	  //dg::fillh( "lepton_pt_vs_ht" , 100,0,500, 600,0,3000,lepton->pt(),wevent->ht(),"lepton P_{T} (GeV)","H_{T} (GeV)");
	  //dg::fillh( "lepton_eta_vs_ht" , 200,-3,3, 600,0,3000,lepton->eta(),wevent->ht(), "lepton #eta","H_{T} (GeV)");
	  //dg::fillh( "dphi_lepton_met_vs_st" , 100,0,M_PI, 100,0.0,1.0, fabs( lepton->dist_phi( met ) ),wevent->st(),"#Delta #phi Lepton - MET (rad)","S_{T}" );
	}
      }
      if( wevent->njets()>0 && lepton ) {
        dg::fillh( "dphi_lepton_jet0" , 100 , 0 , M_PI , fabs( lepton->dist_phi( *(jets_by_pt.begin()) ) ) , "#Delta #phi Lepton - Leading Jet (rad)" );
        //dg::fillh( "deta_lepton_jet0" , 200 , -M_PI , M_PI , lepton->dist_eta( *(jets_by_pt.begin()) ) , "#Delta #eta Lepton - Leading Jet" );
        //dg::fillh( "dr_lepton_jet0" , 50 , 0 , 5 , lepton->dist_eta_phi( *(jets_by_pt.begin()) ) , "#Delta r(#eta,#phi) Lepton - Leading Jet" );
      }
      if( wevent->njets()>1 && lepton ) {
	dg::fillh( "dphi_lepton_jet1" , 100 , 0 , M_PI , fabs( lepton->dist_phi( *(jets_by_pt.begin()+1) ) ) , "#Delta #phi Lepton - Leading Jet (rad)" );
	//dg::fillh( "deta_lepton_jet1" , 200 , -M_PI , M_PI , lepton->dist_eta( *(jets_by_pt.begin()+1) ) , "#Delta #eta Lepton - Leading Jet" );
	//dg::fillh( "dr_lepton_jet1" , 50 , 0 , 5 , lepton->dist_eta_phi( *(jets_by_pt.begin()+1) ) , "#Delta r(#eta,#phi) Lepton - Leading Jet" );
      }
      if( wevent->njets()>0 && met ) {
        dg::fillh( "dphi_met_jet0" , 100 , 0 , M_PI , fabs( (*(jets_by_pt.begin()))->dist_phi( met ) ) , "#Delta #phi MET - Leading Jet (rad)" );
        //dg::fillh( "deta_met_jet0" , 200 , -M_PI , M_PI , (*(jets_by_pt.begin()))->dist_eta( wevent->met_eta( wevent->met_preferred_pz() ) ) , "#Delta #eta MET - Leading Jet" );
        //dg::fillh( "dr_met_jet0" , 50 , 0 , 5 , (*(jets_by_pt.begin()))->dist_eta_phi( wevent->met_eta( wevent->met_preferred_pz() ) , met->phi() ) , "#Delta r(#eta,#phi) MET - Leading Jet" );
        dg::fillh( "jet0_pt_vs_met" , 100 , 0, 500, 300, 0 , 1500 , met->pt(), (*(jets_by_pt.begin()))->pt() , "MET [GeV]", "Leading Jet p_{T} [GeV]" );
        if( lepton )
          if( _do_2d_plots ) dg::fillh( "jet0_pt_vs_dphi_lepton_met", 200, -M_PI, M_PI, 100, 0 , 200 , lepton->dist_phi( met ),
					(*(jets_by_pt.begin()))->pt() , "#Delta #phi Lepton - MET (rad)" , "Leading Jet p_{T} [GeV]" );
      }
      if( wevent->njets()>1 && met ) {
	dg::fillh( "dphi_met_jet1" , 100 , 0 , M_PI , fabs( (*(jets_by_pt.begin()+1))->dist_phi( met ) ) , "#Delta #phi MET - Leading Jet (rad)" );
	//dg::fillh( "deta_met_jet1" , 200 , -M_PI , M_PI , (*(jets_by_pt.begin()+1))->dist_eta( wevent->met_eta( wevent->met_preferred_pz() ) ) , "#Delta #eta MET - Leading Jet" );
	//dg::fillh( "dr_met_jet1" , 50 , 0 , 5 , (*(jets_by_pt.begin()+1))->dist_eta_phi( wevent->met_eta( wevent->met_preferred_pz() ) , met->phi() ) , "#Delta r(#eta,#phi) MET - Leading Jet" );
	dg::fillh( "jet1_pt_vs_met" , 100 , 0, 200, 100, 0 , 200 , met->pt(), (*(jets_by_pt.begin()+1))->pt() , "MET [GeV]", "Leading Jet p_{T} [GeV]" );
        if( lepton )
          if( _do_2d_plots ) dg::fillh( "jet1_pt_vs_dphi_lepton_met", 200, -M_PI, M_PI, 100, 0 , 200 , lepton->dist_phi( met ),
                                        (*(jets_by_pt.begin()+1))->pt() , "#Delta #phi Lepton - MET (rad)" , "Leading Jet p_{T} [GeV]" );
      }
      if( wevent->njets()>1 ) {
        dg::fillh( "dijet_m" , 500 , 0  , 500 , wevent->dijet_m() , "Invariant Mass of 2 Hardest Jets (GeV/c^{2})" );
        dg::fillh( "dphi_leading_jets"  , 100 , 0 , M_PI , fabs((*(jets_by_pt.begin()))->dist_phi(*(jets_by_pt.begin()+1))) , "#Delta #phi Leading Jets (rad)");
        dg::fillh( "deta_leading_jets"      , 200 , -M_PI , M_PI , wevent->deta_leading_jets()        , "#Delta #eta Leading Jets" );
        dg::fillh( "abs_deta_leading_jets"  , 100 ,  0 , M_PI , fabs(wevent->deta_leading_jets())  , "|#Delta #eta| Leading Jets" );
	dg::fillh( "max_deta_jets"          , 200 , -M_PI , M_PI , wevent->max_deta_jets()            , "max #Delta #eta leading jet - hardest three" );
        dg::fillh( "abs_max_deta_jets"      , 100 ,  0 , M_PI , fabs(wevent->max_deta_jets())      , "|max #Delta #eta| leading jet - hardest three" );
	dg::fillh( "true_max_deta_jets"     , 200 , -M_PI , M_PI , wevent->true_max_deta_jets()       , "max #Delta #eta leading jet - any other jet" );
        dg::fillh( "abs_true_max_deta_jets" , 100 ,  0 , M_PI , fabs(wevent->true_max_deta_jets()) , "|max #Delta #eta| leading jet - any other jet" );
        dg::fillh( "dr_leading_jets"        , 50 , 0 , 5 , (*(jets_by_pt.begin()))->dist_eta_phi( *(jets_by_pt.begin()+1) ) , "#Delta r(#eta,#phi) Leading Jets" );
      }
      if( _do_xcheck_studies && wevent->njets()>1 ) {
        dg::fillh( "xcheck_softjet_pt" , 500 , 0 , 500 , wevent->subleading_jet()->pt() );
      }
    }
    if( true ) { 
      // Get delta-R between lepton and nearest jet
      float min_dR = std::numeric_limits<float>::max();
      dg::down( "jet_pts" , "ith-highest pt jet(s)" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      // study ith-highest pt jet(s)
      unsigned int ithjet = 0u;
      for( vector< boost::shared_ptr<const AnaJet> >::iterator ijet=jets_by_pt.begin(), fjet=jets_by_pt.end(); ijet!=fjet; ++ijet, ++ithjet ) {
        const boost::shared_ptr<const AnaJet>& j( *ijet );
        dg::fillh( (format("jet_%|d|thpt_pt") % ithjet ).str() , 300 , 0 , 1500 , j->pt() );
	if((j->tag_sv0_w())>5.85) {
	  dg::fillh( (format("jet_%|d|thpt_pt_btag") % ithjet ).str() , 300 , 0 , 1500 , j->pt() );
	  dg::fillh( "jet_btag" , 10, 0, 10, ithjet, "i-th jet" );
	}	
        dg::fillh( (format("jet_%|d|thpt_absphi") % ithjet ).str() , 100 , 0 , M_PI , fabs( j->phi() ) );
        dg::fillh( (format("jet_%|d|thpt_phi") % ithjet ).str() , 200 , -M_PI , M_PI , j->phi() );
        dg::fillh( (format("jet_%|d|thpt_eta") % ithjet ).str() , 200 , -3 , 3 , j->eta() );
        if( ithjet==0 ) { dg::fillh( "balance_vecsumphi_leading_jet" , 200 , -M_PI , M_PI , 
                                     detector::delta_phi(event_vecsumphi,j->phi()) , "Event Vector #Sigma p_{T} #phi - Leading Jet #phi (rad)" ); }
	float tmp_dR = lepton->dist_eta_phi( j );
	if( tmp_dR < min_dR ) { min_dR = tmp_dR; }
      }
      dg::fillh( "dr_lepton_jet" , 100 , 0 , 5 , min_dR , "#Delta r(#eta,#phi) Lepton - Nearest Jet" );
    }
    if( true && met && lepton ) { 
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
      dg::down( "jet_loweta" , "ith-lowest eta jet(s)" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      // study ith-lowest eta jet(s)
      vector< boost::shared_ptr<const AnaJet> > jets_by_eta( wevent->begin_jets() , wevent->end_jets() );
      std::sort( jets_by_eta.begin() , jets_by_eta.end() , 
                 bind( ptr_fun<AnaJet::ftype,AnaJet::ftype>(abs) , bind(&AnaJet::eta,_1) ) <
                 bind( ptr_fun<AnaJet::ftype,AnaJet::ftype>(abs) , bind(&AnaJet::eta,_2) ) );
      unsigned int ithjet = 0u;
      for( vector< boost::shared_ptr<const AnaJet> >::iterator ijet=jets_by_eta.begin(), fjet=jets_by_eta.end(); ijet!=fjet; ++ijet, ++ithjet ) {
        const boost::shared_ptr<const AnaJet>& j( *ijet );
        dg::fillh( (format("jet_%|d|theta_pt") % ithjet ).str() , 500 , 0 , 500 , j->pt() );
        dg::fillh( (format("jet_%|d|theta_eta") % ithjet ).str() , 200 , -M_PI , M_PI , j->eta() );
      }
    }
  } // end study W cross section event candidate
protected:
  boost::shared_ptr<const AnaEvent> _event;
  bool _do_asymmetry_studies;
  bool _do_save_ntuple;
  bool _do_heavyQ_studies;
  bool _do_xcheck_studies;
  bool _do_2d_plots;
  bool _do_qcd_2d;
  bool _do_charge_separation;
  bool _do_yield;
  float _yield_min;
  float _yield_max;
  float _yield_runnumber;
  float _yield_lumi;
  std::vector<DataGraphics::Value> _wasymAbsEtaBins;
  std::vector<DataGraphics::Value> _wasymEtaBins;
  std::vector<DataGraphics::Value> _wasymPtBins;
public:
  void do_asymmetry_studies(bool v) { _do_asymmetry_studies = v; if(v) allocate_asymmetry_bins(); }
  void do_asymmetry_studies() { do_asymmetry_studies(true); }
  void do_save_ntuple(bool v) { _do_save_ntuple = v; }
  void do_save_ntuple() { do_save_ntuple(true); }
  void do_heavyQ_studies( bool v = true ) { _do_heavyQ_studies = v; }
  void do_xcheck_studies( bool v = true ) { _do_xcheck_studies = v; }
  void do_2d_plots( bool v = true ) { _do_2d_plots = v; }
  void do_qcd_2d( bool v = true ) { _do_qcd_2d = v; }
  void do_charge_separation( bool v = true ) { _do_charge_separation = v; }
  void do_yield( float min , float max , float runnumber , float lumi ) { _do_yield = true; _yield_min = min ; _yield_max = max ; _yield_runnumber = runnumber; ; _yield_lumi = lumi; }
  void allocate_asymmetry_bins() {
    if(!_wasymPtBins.empty()) return;
    _wasymPtBins = boost::assign::list_of(0.0)(20.0)(40.0)(60.0)(80.0)(100.0)(120.0)(140.0);
    _wasymAbsEtaBins = boost::assign::list_of(0.0)(0.21)(0.42)(0.63)(0.84)(1.05)(1.37)(1.52)(1.74)(1.95)(2.18)(2.4);
    for(unsigned int ii=_wasymAbsEtaBins.size()-1;ii>=1;ii--) _wasymEtaBins.push_back(-_wasymAbsEtaBins[ii]);
    for(unsigned int ii=0;ii<_wasymAbsEtaBins.size();ii++) _wasymEtaBins.push_back(_wasymAbsEtaBins[ii]);
  }
  StudyWprime( const std::string& name , const std::string& description ) 
    : _event()
    , _do_asymmetry_studies(false)
    , _do_save_ntuple(false)
    , _do_heavyQ_studies(false)
    , _do_xcheck_studies(false)
    , _do_2d_plots(true)
    , _do_qcd_2d(true)
    , _do_charge_separation(false)
    , _do_yield(false)
    , _yield_min( 0. )
    , _yield_max( 1. )
    , _yield_runnumber( 0. )
    , _yield_lumi( 1. )
    , DataGraphics::DgStudy<type>(name,description)
  {
  }
  virtual ~StudyWprime() {}
  void for_event( const boost::shared_ptr<const AnaEvent>& event ) { _event = event; }
};

#endif // STUDYWPRIME_HPP
