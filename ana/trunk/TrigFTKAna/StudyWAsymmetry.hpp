 
#ifndef STUDYWASYMMETRY_HPP
#define STUDYWASYMMETRY_HPP

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
#include "DataGraphics/DgTypes.hh"
#include "DataGraphics/DgStudy.hh"
#include "DataGraphics/DgCollection.hh"
#include "DataGraphics/DgBin.hh"
#include "TrigFTKAna/AnaWCrossSectionCandidate.hpp"
#include "TrigFTKAna/AnaParticle.hpp"
#include "TrigFTKAna/AnaMET.hpp"
#include "TrigFTKAna/AnaW.hpp"
#include "TrigFTKAna/AnaMuon.hpp"
#include "TrigFTKAna/AnaElectron.hpp"
#include "TrigFTKAna/Detector.hpp"
#include <TH1D.h>
#include <TH2D.h>
#include <TH3D.h>

#include "egammaAnalysisUtils/BosonPtReweightingTool.h"

#define L(x) (x>120.0 ? 119.9999 : x)
class
StudyWAsymmetry : public DataGraphics::DgStudy< boost::shared_ptr<const AnaWCrossSectionCandidate> >
{
public:
  typedef boost::shared_ptr<const AnaWCrossSectionCandidate> type;

protected:

  virtual void _study( type const& w ) {
    using namespace DataGraphics;
    if(_do_save_ntuple && uncategorized()) {
      study_ntuple(w);
    }
    // at top level, always require a category [since we broadly pass very loose candidates]
    if(_do_save_histo && categorized()) {
      assert( std::abs(-999.0 - dg::global_weight()) > 0.000001 );
      study_histo_percharge(w);
    }
  }

  virtual void study_histo_percharge(type const& w , bool study_2d = true , bool minimal = false) {
    using namespace DataGraphics;
    study_histo(w,study_2d,true); // inclusive: don't make many plots
    if(_do_charge_separation) {
      if(w->scharge()>0) {
	dg::down( "POS" , "W+ candidates" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
	study_histo(w,study_2d,minimal);
      }
      if(w->scharge()<0) {
	dg::down( "NEG" , "W- candidates" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
	study_histo(w,study_2d,minimal);
      }
    }
  }

  // histograms [general]
  virtual void study_histo( type const& w , bool study_2d = false, bool minimal = false) {
    using namespace std;
    using namespace boost;
    using namespace DataGraphics;
    const boost::shared_ptr<const AnaMET>& met = w->met();
    const boost::shared_ptr<const AnaParticle>& lepton = w->lepton();
    const boost::shared_ptr<const AnaMuon> muon = boost::dynamic_pointer_cast<const AnaMuon,const AnaParticle>(lepton);

    if(_do_metonly) {
      if(_run_qcd_slices && met && lepton) {
	dg::fillvh( "d3_abseta_lpt_met" , dg::bin().D_abseta.size()-1, dg::bin().D_abseta , dg::bin().D_lpt.size()-1, dg::bin().D_lpt , dg::bin().D_pt.size()-1, dg::bin().D_pt , std::abs(lepton->eta()) , L(lepton->pt()) , met->pt() ,  "|leta|","lpt","met" );
	dg::fillvh( "d3_abseta_lpt_wmt" , dg::bin().D_abseta.size()-1, dg::bin().D_abseta , dg::bin().D_lpt.size()-1, dg::bin().D_lpt , dg::bin().D_pt.size()-1, dg::bin().D_pt , std::abs(lepton->eta()) , L(lepton->pt()) , w->transverse_mass() ,  "|leta|","lpt","wmt" );
      }
      return;
    }

    // minimal set of plots
    if(met && lepton) {
      dg::fillh( "wmt" , 200 , 0 , 200 , w->transverse_mass() , "W Transverse Mass (GeV/c^{2})" );
      dg::fillh( "met" , 200 , 0 , 200 , met->pt() , "W Missing ET (GeV)" );
      dg::fillh( "lpt" , 200 , 0 , 200 , L(lepton->pt()) , "W Lepton p_{T} (GeV/c)" );
      dg::fillvh( "lepton_absetav" , dg::bin().D_abseta.size()-1, dg::bin().D_abseta, std::abs(lepton->eta()) , "W Lepton |#eta|" );
      dg::fillvh( "lepton_etav" , dg::bin().D_eta.size()-1, dg::bin().D_eta, lepton->eta() , "W Lepton #eta" );
      dg::fillh( "uint"   , 1 , 0 , 1.0 , 0.50 , "Integrated event counts, pt20" );
      if(lepton->pt() > 25.0) { // for integrated measurement in pt>25 GeV region
	dg::fillh( "uint25" , 1 , 0 , 1.0 , 0.50 , "Integrated event counts, pt25" );
      }
    }
    if(minimal) return;
    // bootstrap histos
    if(bootstrap()) {
      dg::bfillvh( "b_lepton_absetav" , dg::bin().D_abseta.size()-1, dg::bin().D_abseta, std::abs(lepton->eta()) , "W Lepton |#eta|" );
      dg::bfillh( "b_uint"   , 1 , 0 , 1.0 , 0.50 , "Integrated event counts, pt20" );
      if(lepton->pt() > 25.0) { // for integrated measurement in pt>25 GeV region
	dg::bfillh( "b_uint25" , 1 , 0 , 1.0 , 0.50 , "Integrated event counts, pt25" );
      }
    }
    vector< boost::shared_ptr<const AnaJet> > jets_by_pt( w->begin_jets() , w->end_jets() );
    // make plots
    dg::fillh( "nvtxs_all" , 20 , 0 , 20 , w->nvtxs_all() , "N_{vtxs} (before cuts)" );
    dg::fillh( "njets" , 10 , 0 , 10 , w->njets() , "N_{jets} with pt>30 GeV" );
    dg::fillh( "wpt" , 200 , 0 , 200 , w->pt() , "W Transverse Momentum (GeV/c)" );
    if( false && lepton && met) {
      dg::fillh( "event_mt" , 500 , 0 , 500 , w->event_mt() , "Transverse Mass of Lepton, MET, and Jets (GeV/c^{2})" );
      dg::fillh( "event_m" , 500 , 0 , 500 , w->event_m() , "Invariant Mass of Lepton, MET, and Jets (GeV/c^{2})" );
      dg::fillh( "w_m" , 200 , 0 , 200 , w->w_m() , "Invariant Mass of W (GeV/c^{2})" );
      dg::fillh( "w_eta" , 100 , -3. , 3. , w->w_eta() , "W #eta" );
    }
    // final plots for unfolding
    if(study_2d && met && lepton) {
      dg::fillvh( "d2_abseta_lpt" , dg::bin().D_abseta.size()-1, dg::bin().D_abseta , dg::bin().D_lpt.size()-1, dg::bin().D_lpt , std::abs(lepton->eta()) , L(lepton->pt()),"|leta|","lpt" );
      dg::fillvh( "d2_eta_lpt" , dg::bin().D_eta.size()-1, dg::bin().D_eta , dg::bin().D_lpt.size()-1, dg::bin().D_lpt , lepton->eta() , L(lepton->pt()),"leta","lpt" );
      // bootstrap histos
      if(bootstrap()) {
	dg::bfillvh( "b_d2_abseta_lpt" , dg::bin().D_abseta.size()-1, dg::bin().D_abseta , dg::bin().D_lpt.size()-1, dg::bin().D_lpt , std::abs(lepton->eta()) , L(lepton->pt()),"|leta|","lpt" );
      }
      // QCD normalization in slices: utilize 3d histograms to store everything
      if(_run_qcd_slices) {
	dg::fillvh( "d3_abseta_lpt_met" , dg::bin().D_abseta.size()-1, dg::bin().D_abseta , dg::bin().D_lpt.size()-1, dg::bin().D_lpt , dg::bin().D_pt.size()-1, dg::bin().D_pt , std::abs(lepton->eta()) , L(lepton->pt()) , met->pt() ,  "|leta|","lpt","met" );
	dg::fillvh( "d3_abseta_lpt_wmt" , dg::bin().D_abseta.size()-1, dg::bin().D_abseta , dg::bin().D_lpt.size()-1, dg::bin().D_lpt , dg::bin().D_pt.size()-1, dg::bin().D_pt , std::abs(lepton->eta()) , L(lepton->pt()) , w->transverse_mass() ,  "|leta|","lpt","wmt" );
	dg::fillvh( "d3_eta_lpt_met" , dg::bin().D_eta.size()-1, dg::bin().D_eta , dg::bin().D_lpt.size()-1, dg::bin().D_lpt , dg::bin().D_pt.size()-1, dg::bin().D_pt , lepton->eta() , L(lepton->pt()) , met->pt() ,"leta","lpt","met" );
	if(false) {  //02/08/2013: disabled
	  dg::fillvh( "d3_eta_lpt_wmt" , dg::bin().D_eta.size()-1, dg::bin().D_eta , dg::bin().D_lpt.size()-1, dg::bin().D_lpt , dg::bin().D_pt.size()-1, dg::bin().D_pt , lepton->eta() , L(lepton->pt()) , w->transverse_mass() ,"leta","lpt","wmt" );
	}
	if(true) { // additional plots for systematic plots in eta/phi slices
	  dg::fillvh( "d3_abseta_lpt_wpt" , dg::bin().D_abseta.size()-1, dg::bin().D_abseta , dg::bin().D_lpt.size()-1, dg::bin().D_lpt , dg::bin().D_pt.size()-1, dg::bin().D_pt , std::abs(lepton->eta()) , L(lepton->pt()) , w->pt() ,  "|leta|","lpt","wpt" );
	  dg::fillvh( "d3_abseta_lpt_lpt" , dg::bin().D_abseta.size()-1, dg::bin().D_abseta , dg::bin().D_lpt.size()-1, dg::bin().D_lpt , dg::bin().D_pt.size()-1, dg::bin().D_pt , std::abs(lepton->eta()) , L(lepton->pt()) , lepton->pt() ,  "|leta|","lpt","lpt" );
	  dg::fillvh( "d3_abseta_lpt_phi" , dg::bin().D_abseta.size()-1, dg::bin().D_abseta , dg::bin().D_lpt.size()-1, dg::bin().D_lpt , dg::bin().D_lphi.size()-1, dg::bin().D_lphi , std::abs(lepton->eta()) , L(lepton->pt()) , lepton->phi() ,  "|leta|","lpt","phi" );
	  dg::fillvh( "d3_abseta_lpt_eta" , dg::bin().D_abseta.size()-1, dg::bin().D_abseta , dg::bin().D_lpt.size()-1, dg::bin().D_lpt , dg::bin().D_leta.size()-1, dg::bin().D_leta , std::abs(lepton->eta()) , L(lepton->pt()) , lepton->eta() ,  "|leta|","lpt","phi" );
	}
      }
    }
    // for Nominal/baseline selection: plot delta-phi between MET SoftTerms and all-but-SoftTerms
    // PS - SoftTerms means softjets + cellout + cellout_eflow. we also do separately for softjets ONLY.
    if(true && _run_debug_studies && met && is_baseline()) {
      TLorentzVector met_final,met_softjets,met_cellout,met_celloute;
      met_final.SetPtEtaPhiM( met->pt() , 0. , met->phi() , 0. );
      met_softjets.SetPtEtaPhiM( met->softjets() , 0. , met->softjets_phi() , 0. );
      met_cellout.SetPtEtaPhiM( met->cellout() , 0. , met->cellout_phi() , 0. );
      met_celloute.SetPtEtaPhiM( met->cellout_eflow() , 0. , met->cellout_eflow_phi() , 0. );
      TLorentzVector met_soft = met_softjets + met_cellout + met_celloute;
      TLorentzVector met_nosoft = met_final - met_soft;
      TLorentzVector met_nosoftjets = met_final - met_softjets;
      dg::fillh( "dphi_metsoft_metnosoft_Wsel", 32 , -3.15 , 3.15 , detector::canonical_phi(met_soft.Phi() - met_nosoft.Phi()) , "MET soft - nosoft (softjets+cellout+cellouteflow)" );
      dg::fillh( "dphi_metsjets_metnosjets_Wsel", 32 , -3.15 , 3.15 , detector::canonical_phi(met_softjets.Phi() - met_nosoftjets.Phi()) , "MET soft - nosoft (softjets only)" );
    }
    // general 1D plots
    if( false ) {  //02/08/2013: disabled
      if(false &&  met ) {
        dg::fillh( "met_phi", 100 , -M_PI , M_PI , met->phi() , "MET #phi (rad)" );
      } // end met plots
      if( lepton ) {
        dg::fillh( "lepton_phi" , 100 , -M_PI , M_PI , lepton->phi() , "W Lepton #phi (rad)" );
	//dg::fillh( "lepton_eta_fine" , 128 , -2.5 , 2.5 , lepton->eta() , "W Lepton #eta" );
	//dg::fillh( "lepton_abseta" , 10 , 0 , 2.5 , std::abs(lepton->eta()) , "W Lepton |#eta|" );
	//dg::fillh( "lepton_abseta_fine" , 128 , 0 , 2.5 , std::abs(lepton->eta()) , "W Lepton |#eta|" );
	//dg::fillvh( "lepton_ptv" , dg::bin().D_lpt.size()-1, dg::bin().D_lpt, L(lepton->pt()) , "W Lepton #pt" );
	//dg::fillvh( "w_wmtv" , dg::bin().D_wmt.size()-1 , dg::bin().D_wmt , w->transverse_mass() , "W Transverse Mass (GeV/c^{2})" );
	if(false &&  met) {
	  dg::fillh( "dphi_lepton_met" , 100 , 0 , M_PI , std::abs( lepton->dist_phi( met ) ) , "#Delta #phi Lepton - MET (rad)" );
	  dg::fillh( "deta_lepton_met" , 100 , -5 , 5 , lepton->dist_eta( w->met_eta( w->met_pz_vlq() ) ) , "#Delta #eta Lepton - MET" );
	  dg::fillh( "dr_lepton_met" , 100 , 0 , 5 , lepton->dist_eta_phi( w->met_eta( w->met_pz_vlq() ) , met->phi() ) , "#Delta r(#eta,#phi) Lepton - MET" );
	} // end lepton-met plots
	if( muon ) {
	  // asymmetry from ID or MS muons
	  //dg::fillvh( "muonID_absetav" , dg::bin().D_abseta.size()-1, dg::bin().D_abseta, std::abs(muon->id_eta()) , "W muon ID-only |#eta|" );
	  //dg::fillvh( "muonMS_absetav" , dg::bin().D_abseta.size()-1, dg::bin().D_abseta, std::abs(muon->exms_eta()) , "W muon MS-only |#eta|" );
	  // d0/z0/iso
	  if(false) {
	    dg::fillh( "exPV_z0" ,200, -1 , 1 , muon->exPV_z0() ,"W muon z0 wrt PV" );
	    dg::fillh( "exPV_d0" ,200, -0.1 , 0.1 , muon->exPV_d0() ,"W muon d0 signifiance wrt PV" );
	    dg::fillh( "exPV_d0_sig" ,200, -4 , 4 , muon->exPV_d0_signif() ,"W muon d0 signifiance wrt PV" );
	  }
	  dg::fillh( "lepton_ptiso40r" ,200, 0 , 0.3 , muon->pt_cone_ratio(AnaMuon::CONE40) ,"W Lepton ptcone40" );
	  //dg::fillh( "lepton_ptiso30r" ,200, 0 , 0.3 , muon->pt_cone_ratio(AnaMuon::CONE30) ,"W Lepton ptcone30" );
	  //dg::fillh( "lepton_ptiso20r" ,200, 0 , 0.3 , muon->pt_cone_ratio(AnaMuon::CONE20) ,"W Lepton ptcone20" );
	  //dg::fillh( "lepton_etiso30rcorr" ,200, 0 , 0.3 , muon->et_cone_corr_ratio(AnaMuon::CONE30,w->nvtxs_all()) ,"W Lepton etcone30 corr" );
	  //dg::fillh( "lepton_etiso40rcorr" ,200, 0 , 0.3 , muon->et_cone_corr_ratio(AnaMuon::CONE40,w->nvtxs_all()) ,"W Lepton etcone40 corr" );
	} // end muon plots
      } // end lepton plots
    }
    // plot the yield if flag is set to true
    if( _do_yield ) {
      dg::fillhw( "yield" , 
		  int( _yield_max - _yield_min ) , _yield_min , _yield_max ,
		  _yield_runnumber , dg::global_weight() / _yield_lumi , "run number" );
    }
    if( false && w->njets()>0 ) {
      dg::fillh( "jets_m" , 200 , 0 , 200 , w->jets_m() , "Invariant Mass of Jets (GeV/c^{2})" );
    }
    if(false) {
      std::sort( jets_by_pt.begin() , jets_by_pt.end() , bind(&AnaJet::pt,_1) > bind(&AnaJet::pt,_2) );
    }
    if( false && w->njets()>0 && lepton ) {
      //dg::fillh( "dphi_lepton_jet0" , 100 , 0 , M_PI , std::abs( lepton->dist_phi( *(jets_by_pt.begin()) ) ) , "#Delta #phi Lepton - Leading Jet (rad)" );
      //dg::fillh( "deta_lepton_jet0" , 100 , -5 , 5 , lepton->dist_eta( *(jets_by_pt.begin()) ) , "#Delta #eta Lepton - Leading Jet" );
      dg::fillh( "dr_lepton_jet0" , 50 , 0 , 5 , lepton->dist_eta_phi( *(jets_by_pt.begin()) ) , "#Delta r(#eta,#phi) Lepton - Leading Jet" );
    }
    if( false && w->njets()>0 && met ) {
      //dg::fillh( "dphi_met_jet0" , 100 , 0 , M_PI , std::abs( (*(jets_by_pt.begin()))->dist_phi( met ) ) , "#Delta #phi MET - Leading Jet (rad)" );
      //dg::fillh( "deta_met_jet0" , 100 , -5 , 5 , (*(jets_by_pt.begin()))->dist_eta( w->met_eta( w->met_pz_vlq() ) ) , "#Delta #eta MET - Leading Jet" );
      dg::fillh( "dr_met_jet0" , 50 , 0 , 5 , (*(jets_by_pt.begin()))->dist_eta_phi( w->met_eta( w->met_pz_vlq() ) , met->phi() ) , "#Delta r(#eta,#phi) MET - Leading Jet" );
    }
    if( false && w->njets()>1 ) {
      dg::fillh( "dijet_m" , 200 , 0  , 200 , w->dijet_m() , "Invariant Mass of 2 Hardest Jets (GeV/c^{2})" );
    }
    if( false ) { 
      // Get delta-R between lepton and nearest jet
      float min_dR = std::numeric_limits<float>::max();
      dg::down( "jet_pts" , "ith-highest pt jet(s)" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      // study ith-highest pt jet(s)
      unsigned int ithjet = 0u;
      for( vector< boost::shared_ptr<const AnaJet> >::iterator ijet=jets_by_pt.begin(), fjet=jets_by_pt.end(); ijet!=fjet; ++ijet, ++ithjet ) {
        const boost::shared_ptr<const AnaJet>& j( *ijet );
	if(ithjet<3) { // don't waste memory on fourth and higher pt jets
	  dg::fillh( (format("jet_%|d|thpt_pt") % ithjet ).str() , 200 , 0 , 200 , j->pt() );
	  //dg::fillh( (format("jet_%|d|thpt_phi") % ithjet ).str() , 100 , -M_PI , M_PI , j->phi() );
	  dg::fillh( (format("jet_%|d|thpt_eta") % ithjet ).str() , 100 , -8 , 8 , j->eta() );
	}
	float tmp_dR = lepton->dist_eta_phi( j );
	if( tmp_dR < min_dR ) { min_dR = tmp_dR; }
      }
      dg::fillh( "dr_lepton_jet" , 100 , 0 , 5 , min_dR , "#Delta r(#eta,#phi) Lepton - Nearest Jet" );
    }
  } // end study W cross section event candidate

  // unbinned data (DgNtuple)
  virtual void study_ntuple( type const& w ) {
    using namespace std;
    using namespace boost;
    using namespace DataGraphics;
    const boost::shared_ptr<const AnaParticle>& lepton = w->lepton();
    const boost::shared_ptr<const AnaMuon> muon = boost::dynamic_pointer_cast<const AnaMuon,const AnaParticle>(lepton);
    const boost::shared_ptr<const AnaMET>& met = w->met();
    // DgNtuple for unbinned likelihood
    if(_do_save_ntuple && uncategorized()) {
      using namespace boost::assign; // bring 'operator+=()' into scope
      const short baseline20_status = w->asym_baseline() ? 1 : 0;
      const short baseline25_status = w->asym_baseline25() ? 1 : 0;
      const short eta_trig_status = w->asym_eta_trig() ? 1 : 0;
      std::vector<std::string> names = list_of("avgmu")("l_q")("l_pt")("l_eta")("l_phi")("w_mt")("w_pt")("njets")("nvtxs")("nvtxs_all")("nmuons")("nmuons25")("mask20")("mask25")("l_eta_trig");
      std::vector<float> values = list_of(_average_mu)(w->scharge()*1.0)(lepton->pt())(lepton->eta())(lepton->phi())(w->transverse_mass())(w->pt())(w->njets()*1.0)(w->nvtxs()*1.0)(w->nvtxs_all()*1.0)(w->nleptons()*1.0)(w->nleptons25()*1.0)(baseline20_status*1.0)(baseline25_status*1.0)(eta_trig_status*1.0);
      if(muon) {
	// trigger matching
	names += "l_trigEF"; //,"l_trigEFb";
	const int end_of_periodI = 186493;
	if(_run_number <= end_of_periodI) {
	  values += muon->trigger_match_dr("EF_mu18_MG"); //,muon->trigger_match_dr("EF_mu40_MSonly_barrel");
	}
	else {
	  values += muon->trigger_match_dr("EF_mu18_MG_medium"); //,muon->trigger_match_dr("EF_mu40_MSonly_barrel_medium");
	}
	// Trigger debugging:
	if(true) {
	  names += "l_trigEF_mu18_MG","l_trigEF_mu18_MG_medium","l_trigEF_mu18","l_trigEF_mu18_medium";
	  values += muon->trigger_match_dr("EF_mu18_MG");
	  values += muon->trigger_match_dr("EF_mu18_MG_medium");
	  values += muon->trigger_match_dr("EF_mu18");
	  values += muon->trigger_match_dr("EF_mu18_medium");
	}
	// save trigger weight for each individual muon?
	float l_trigw = 1.0;
	if(_is_mc) {
	  std::vector<TLorentzVector> velectrons; // should be empty
	  std::vector<TLorentzVector> vmuons; vmuons.push_back(muon->four_vector());
	  std::vector<float> vmuons_charges; vmuons_charges.push_back(muon->charge());
	  double Aval = 1.0;
	  double Aerr = 0;
	  AnaMuon::GetTriggerSF_v17_custom2( AnaConfiguration::conf() , detector::MCP_STACO_COMBINED , detector::EGAMMA_MEDIUMPP , 
		 			     vmuons , velectrons , vmuons_charges, 
					     _run_number , 0 , Aval , Aerr , -1);
	  l_trigw = Aval;
	}
	names += "l_trigw";
	values += l_trigw;
	// isolation
	names += "ptiso40","ptiso30","ptiso20";
	values += muon->pt_cone(AnaMuon::CONE40),muon->pt_cone(AnaMuon::CONE30),muon->pt_cone(AnaMuon::CONE20);
	names += "etiso30corr"; // corrected et isolation from Peter
	values += muon->et_cone_corr(AnaMuon::CONE30 , w->nvtxs_all() );
	names += "etiso40corr"; // corrected et isolation from Peter
	values += muon->et_cone_corr(AnaMuon::CONE40 , w->nvtxs_all() );
	names += "etiso30"; // uncorrected et isolation
	values += muon->et_cone(AnaMuon::CONE30);
	names += "etiso40"; // uncorrected et isolation
	values += muon->et_cone(AnaMuon::CONE40);
	// vertex and ID variables
	names += "z0","d0","d0sig","idhits";
	values += muon->exPV_z0(),muon->exPV_d0(),muon->exPV_d0_signif(),muon->passes_hits_mcp_v17_moriond()?1.:0.0;
	// kinematics of ID and EXMS muons
	names += "l_q_id","l_pt_id","l_eta_id","l_phi_id";
	values += muon->id_charge()*1.0,muon->id_pt(),muon->id_eta(),muon->id_phi();
	names += "l_q_exms","l_pt_exms","l_eta_exms","l_phi_exms";
	values += muon->exms_charge()*1.0,muon->exms_pt(),muon->exms_eta(),muon->exms_phi();
      }
      if(met) {
	names += "met","met_phi";
	values += met->pt(),met->phi() ;
	// MET debugging:
	if(false) { // 04/23/2013: disabled
#define MT(x,y) detector::mtFromTLorentzVector<float>(x,y)
	  TLorentzVector mu_corr;
	  mu_corr.SetPtEtaPhiM( muon->pt() , 0. , muon->phi() , 0. );
	  TLorentzVector met_reffinal_corr;
	  met_reffinal_corr.SetPtEtaPhiM( met->pt() , 0. , met->phi() , 0. );
	  // raw version of muon and MET
	  TLorentzVector mu_raw = mu_corr;
	  if(muon->is_corrected()) mu_raw.SetPtEtaPhiM( muon->uncorrected_parent()->pt() , 0. , muon->uncorrected_parent()->phi() , 0. );
	  TLorentzVector met_reffinal_raw = met_reffinal_corr;
	  if(met->is_corrected()) met_reffinal_raw.SetPtEtaPhiM( met->uncorrected_parent()->pt() , 0. , met->uncorrected_parent()->phi() , 0. );
	  // save raw met, magnitude of its correction
	  names += "met_raw","wmt_raw","metcorr_total","metcorr_muon";
	  values += met_reffinal_raw.Pt(), MT(met_reffinal_raw,mu_raw),(met_reffinal_corr-met_reffinal_raw).Pt(), (mu_corr-mu_raw).Pt();
	  // additional METs; difference of muon term from actual muon
	  TLorentzVector met_muonboy,met_refmuon,met_lochad,met_ichep; // all are "RAW" (no MCP)
	  met_muonboy.SetPtEtaPhiM( met->muonboy() , 0. , met->muonboy_phi() , 0. );
	  met_refmuon.SetPtEtaPhiM( met->refmuon_track() , 0. , met->refmuon_track_phi() , 0. );
	  met_lochad.SetPtEtaPhiM( met->lochad_topo() , 0. , met->lochad_topo_phi() , 0. );
	  met_ichep.SetPtEtaPhiM( met->lochad_topo_ichep() , 0. , met->lochad_topo_ichep_phi() , 0. );
	  names += "met_muonboy","met_refmuon","met_lochad","met_ichep","wmt_ichep";
	  values += met_muonboy.Pt(),met_refmuon.Pt(),met_lochad.Pt(),met_ichep.Pt(),MT(met_ichep,mu_raw);
	  TLorentzVector met_ichep_corr = met_ichep-met_reffinal_raw+met_reffinal_corr;
	  names += "met_ichep_corr","wmt_ichep_corr"; // try to correct ichep met
	  values += met_ichep_corr.Pt(),MT(met_ichep_corr,mu_corr);
	  // CAREFUL: mixing met and mu - pay attention to the direction of muonboy vs muon 4vectors
	  names += "dmet_muonboy_muon","dmet_muonboy_muon_corr";
	  values += (met_muonboy+mu_raw).Pt(),(met_muonboy+mu_corr).Pt();
	  // LOCHADTOP + real muon
	  TLorentzVector met_locmuon = (met_lochad - mu_raw - met_refmuon);
	  TLorentzVector met_locmuon_corr = (met_lochad - mu_corr - met_refmuon);
	  names += "met_locmuon","wmt_locmuon","met_locmuon_corr","wmt_locmuon_corr";
	  values += met_locmuon.Pt(),MT(met_locmuon,mu_raw),met_locmuon_corr.Pt(),MT(met_locmuon_corr,mu_corr);
	}
#undef MT
// 	if( muon && met->is_corrected() ) { // correct met_muonboy term by the change in muon pt
// 	  const float dmx = muon->corrected_change_in_px(); // mcp - raw
// 	  const float dmy = muon->corrected_change_in_py();
// 	  const float rpx = met->muonboy_etx();
// 	  const float rpy = met->muonboy_ety();
// 	  const float cpx = rpx - dmx;
// 	  const float cpy = rpy - dmy;
// 	  met_muonboycorr = std::sqrt( cpx*cpx + cpy*cpy);
// 	  met_muonboy_minus_muon = std::sqrt( );
// 	  met_muonboy_minus_muoncorr = 0;
// 	  const float cx = met->px() - met->uncorrected_parent()->px();
// 	  const float cy = met->py() - met->uncorrected_parent()->py();
// 	  metcorr_total = std::sqrt(cx*cx + cy*cy);
// 	  metcorr_muon = std::sqrt(dmx*dmx + dmy*dmy);
// 	  float 
// 	  // re-calculate wmt for each met
// 	  // mT=sqrt( 2*mu.Pt()*_Tmet.Pt()*(1-cos(_Tmet.Phi()-mu.Phi())) )
// 	  float delphi = met->lochad_topo_ichep_phi() - muon->phi();
// 	  wmt_lochad_ichep = std::sqrt( 2.0 * muon->pt() * met->lochad_topo_ichep() * (1.0-cos(delphi)) );
// 	}
	if(lepton && false) {
	  // W boson m, eta, phi - using prefferred neutrino pz guess	
	  names += "w_m","w_eta","w_phi";
	  values += w->w_m(),w->w_eta(),w->phi();
	  // delta-phi between lepton and MET
	  names += "dphi";
	  values += std::abs( lepton->dist_phi( met ) );
	  // W polarization angle (cos-theta-3d)
	  names += "cos3d";
	  if (met->eta()!=0) { // only works when applied to truth!
	    values += BosonPtReweightingTool::CosThetaRestSystem( lepton->four_vector() , met->four_vector() );
	  } else {
	    values += 0.0;
	  }
	}
      }
      dg::fillnt("ntuple",values,names);
    }
  }

protected:
  bool _do_bootstrap;
  bool _do_save_ntuple;
  bool _do_2d_plots;
  bool _do_charge_separation;
  bool _do_save_histo;
  bool _run_qcd_slices;
  bool _run_debug_studies;
  bool _do_yield;
  bool _do_metonly;
  unsigned long _yield_min;
  unsigned long _yield_max;
  unsigned long _yield_runnumber;
  float _yield_lumi;
  unsigned long _run_number;
  bool _is_mc;
  float _average_mu;

public:
  bool is_baseline() const { return current_category()=="baseline"; }
  bool bootstrap() const {   // hardcoded to only run in "baseline" directory
    return _do_bootstrap && is_baseline();
  }
  void do_bootstrap(bool v = true) { _do_bootstrap = v; }
  void do_save_ntuple(bool v = true) { _do_save_ntuple = v; }
  void do_2d_plots( bool v = true ) { _do_2d_plots = v; }
  void do_charge_separation( bool v = true ) { _do_charge_separation = v; }
  void do_save_histo( bool v = true ) { _do_save_histo = v; }
  void run_qcd_slices( bool v = true ) { _run_qcd_slices = v; }
  void run_debug_studies( bool v = true ) { _run_debug_studies = v; }
  void do_metonly(bool v = true) { _do_metonly = v; }
  void do_yield(bool v = true) { _do_yield = v; }
  void do_yield( const unsigned long& min , const unsigned long& max , const unsigned long& runnumber , const float& lumi ) { 
    _do_yield = true; _yield_min = min ; _yield_max = max ; _yield_runnumber = runnumber; _yield_lumi = lumi; 
  }
  StudyWAsymmetry( const std::string& name , const std::string& description ) 
    : _do_bootstrap(false)
    , _do_save_ntuple(false)
    , _do_2d_plots(false)
    , _do_charge_separation(false)
    , _do_save_histo(true)
    , _do_yield(false)
    , _do_metonly(false)
    , _run_qcd_slices(false)
    , _run_debug_studies(false)
    , _yield_min( 0. )
    , _yield_max( 1. )
    , _yield_runnumber( 0. )
    , _yield_lumi( 1. )
    , _run_number(0)
    , _is_mc(false)
    , _average_mu(0)
    , DataGraphics::DgStudy<type>(name,description)
  {}
  virtual ~StudyWAsymmetry() {}
  void for_run( unsigned long rnum ) { _run_number = rnum; }
  void for_mc( bool ismc ) { _is_mc = ismc; }
  void for_average_mu(float avgmu) { _average_mu = avgmu; }
};
#undef L

#endif // STUDYWASYMMETRY_HPP
