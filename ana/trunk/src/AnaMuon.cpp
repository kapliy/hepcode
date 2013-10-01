#include "TrigFTKAna_config.h"

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/current_function.hpp>
#include "TrigFTKAna/Detector.hpp"
#include "TrigFTKAna/Conference.hpp"
#include "TrigFTKAna/AnaMuon.hpp"
#include "TrigFTKAna/AnaTrack.hpp"
#ifdef HAVE_COMMONANALYSIS
#include "MuonMomentumCorrections/SmearingClass.h"
#include "MuonEfficiencyCorrections/AnalysisMuonConfigurableScaleFactors.h"
#include "TrigMuonEfficiency/LeptonTriggerSF.h"
#include "MuonIsolationCorrection/CorrectCaloIso.h"
#include "ScaleFactorProvider/SFProvider.h"
#endif


// A wrapper around TrigMuonEfficiency::GetTriggerSF that computes total trigger scale factor
void
AnaMuon::GetTriggerSF_v17( const CONF::ConfType& conf, const detector::MCP_TYPE& mu_type , const detector::EGAMMA_TYPE& el_type ,
			   std::vector<TLorentzVector>& muons,
			   std::vector<TLorentzVector>& electrons,
			   const unsigned long& run_number , const int& mu_trig ,
			   double& eff, double& err , int replica) {
#ifdef HAVE_COMMONANALYSIS
  assert(conf==CONF::LATEST && "Unsupported --conf for GetTriggerSF_v17 scaling");
  static std::string datadir("CommonAnalysis/RootCore/data/TrigMuonEfficiency/");
  static LeptonTriggerSF tc_base(datadir);
  muon_quality mu_qual=combined;
  electron_quality el_qual=tightpp;
  LeptonTriggerSF *tc = &tc_base;
  if(mu_type==detector::MCP_STACO_COMBINED && el_type==detector::EGAMMA_LOOSEPP) {
    mu_qual=combined; el_qual=loosepp;
  }
  if(mu_type==detector::MCP_STACO_COMBINED && el_type==detector::EGAMMA_MEDIUMPP) {
    mu_qual=combined; el_qual=mediumpp;
  }
  if(mu_type==detector::MCP_STACO_COMBINED && el_type==detector::EGAMMA_TIGHTPP) {
    mu_qual=combined; el_qual=tightpp;
  }
  if((mu_type==detector::MCP_STACO_LOOSE || mu_type==detector::MCP_STACO_TIGHT) && el_type==detector::EGAMMA_LOOSEPP) {
    mu_qual=loose; el_qual=loosepp;
  }
  if((mu_type==detector::MCP_STACO_LOOSE || mu_type==detector::MCP_STACO_TIGHT) && el_type==detector::EGAMMA_MEDIUMPP) {
    mu_qual=loose; el_qual=mediumpp;
  }
  if((mu_type==detector::MCP_STACO_LOOSE || mu_type==detector::MCP_STACO_TIGHT) && el_type==detector::EGAMMA_TIGHTPP) {
    mu_qual=loose; el_qual=tightpp;
  }
  assert(tc);
  
  std::pair<double, double> res = tc->GetTriggerSF(run_number,true,muons,mu_qual,electrons,el_qual);
  eff = res.first;
  err = res.second;
#else
  assert(0&&NO_ANALYSIS_MSG);
#endif
}

// a custom version of trigger scale factors that can be used with the ScaleFactorProvider package
// this implementation provides these scale factors as a function of eta only, in bins of measurement.
void
AnaMuon::GetTriggerSF_v17_custom( const CONF::ConfType& conf, const detector::MCP_TYPE& mu_type , const detector::EGAMMA_TYPE& el_type ,
				  std::vector<TLorentzVector>& muons,
				  std::vector<TLorentzVector>& electrons,
				  std::vector<ftype>& muon_charges,
				  const unsigned long& run_number , const int& mu_trig ,
				  double& eff, double& err , int choice , int replica) {
#ifdef HAVE_COMMONANALYSIS
  assert(false && "DISABLED FOR NOW - THESE ARE OLD SFs FROM MAX");
  assert(conf==CONF::LATEST && "Unsupported --conf for GetTriggerSF_v17 scaling");
  static std::string datapath("data/AnalysisManager.TagAndProbe.data11_7TeV.Muons_SMWZ_p833_v0115_zvtx.root");
  static std::string mcpath("data/AnalysisManager.TagAndProbe.mc11c_7TeV_mc11c.p833_v0115_zvtx_part1.root");
  static std::string numD("eff_cbiso_trig/Numerator/"); static std::string denD("eff_cbiso_trig/Denominator/");
  static std::string numM("eff_cbiso_trig/pid_108304/Numerator/"); static std::string denM("eff_cbiso_trig/pid_108304/Denominator/");
  static std::string numD_p("eff_cbiso_trig/Numerator/mu_plus_probe_eta_Numerator");
  static std::string denD_p("eff_cbiso_trig/Denominator/mu_plus_probe_eta_Denominator");
  static std::string numM_p("eff_cbiso_trig/pid_108304/Numerator/mu_plus_probe_eta_Numerator");
  static std::string denM_p("eff_cbiso_trig/pid_108304/Denominator/mu_plus_probe_eta_Denominator");
  static std::string numD_n("eff_cbiso_trig/Numerator/mu_minus_probe_eta_Numerator");
  static std::string denD_n("eff_cbiso_trig/Denominator/mu_minus_probe_eta_Denominator");
  static std::string numM_n("eff_cbiso_trig/pid_108304/Numerator/mu_minus_probe_eta_Numerator");
  static std::string denM_n("eff_cbiso_trig/pid_108304/Denominator/mu_minus_probe_eta_Denominator");
  // separate tool instance for mu+ and mu-
  static SFProvider tcEta_p(datapath,numD+"mu_plus_probe_eta_Numerator",denD+"mu_plus_probe_eta_Denominator",
			    mcpath,numM+"mu_plus_probe_eta_Numerator",denM+"mu_plus_probe_eta_Denominator",
			    SFProvider::Eta,1.0);
  static SFProvider tcEta_n(datapath,numD+"mu_minus_probe_eta_Numerator",denD+"mu_minus_probe_eta_Denominator",
			    mcpath,numM+"mu_minus_probe_eta_Numerator",denM+"mu_minus_probe_eta_Denominator",
			    SFProvider::Eta,1.0);
  static SFProvider tcEtaPhi_p(datapath,numD+"map_plus_etaphi_Numerator",denD+"map_plus_etaphi_Denominator",
			       mcpath,numM+"map_plus_etaphi_Numerator",denM+"map_plus_etaphi_Denominator",
			       SFProvider::EtaPhi,1.0);
  static SFProvider tcEtaPhi_n(datapath,numD+"map_minus_etaphi_Numerator",denD+"map_minus_etaphi_Denominator",
			       mcpath,numM+"map_minus_etaphi_Numerator",denM+"map_minus_etaphi_Denominator",
			       SFProvider::EtaPhi,1.0);
  static SFProvider tcEtaPt_p(datapath,numD+"map_plus_etapt_Numerator",denD+"map_plus_etapt_Denominator",
			       mcpath,numM+"map_plus_etapt_Numerator",denM+"map_plus_etapt_Denominator",
			       SFProvider::EtaPt,1.0);
  static SFProvider tcEtaPt_n(datapath,numD+"map_minus_etapt_Numerator",denD+"map_minus_etapt_Denominator",
			       mcpath,numM+"map_minus_etapt_Numerator",denM+"map_minus_etapt_Denominator",
			       SFProvider::EtaPt,1.0);
  err = 0;
  eff = 1.0;
  SFProvider *tc_p = 0;
  SFProvider *tc_n = 0;
  // choices of binning for trigger scale factors:
  if(choice==0) { tc_p = &tcEta_p;tc_n = &tcEta_n; }
  else if(choice==1) { tc_p = &tcEtaPhi_p;tc_n = &tcEtaPhi_n; }
  else if(choice==2) { tc_p = &tcEtaPt_p;tc_n = &tcEtaPt_n; }
  assert(tc_p);
  assert(tc_n);
  for(unsigned i=0; i<muons.size(); i++) {
    eff *= muon_charges[i]>0 ? tc_p->getSF(muons[i]) : tc_n->getSF(muons[i]);
  }
#else
  assert(0&&NO_ANALYSIS_MSG);
#endif
}



// a custom version of trigger scale factors that can be used with the ScaleFactorProvider package
// this implementation uses histograms from the MCP trigger group, but we are implementing all machinery by-hand.
// These particular histograms were made in October 2012, and should be used with 2011 data ONLY
// 05/10/2013: see Jan's email:
/*
  
maybe one (late) comment on this:
splitting up the data to periods etc should NOT be a problem for the
final statistical uncertainty, if you do the error propagation the right
way: you have to make the toys independent for all statistically
independent measurements. It somehow sounds like this is not done...
e.g. you should not fluctuate with the same random numbers measurements
from independent periods.

Or when averaging periods before doing the toys, this has to be done
with the usual formulas for weighted mean.

*/
Double_t AnaMuon::GetSFError( Double_t a, Double_t b, Double_t c, Double_t d) {
  // Function to calculate uncertainty on SF
  return TMath::Sqrt( ( TMath::Power( a, 2 ) / TMath::Power( 1 - b, 2 ) ) * c +
		      ( ( TMath::Power( b, 2 ) * TMath::Power( 1 - a, 2 ) ) /
			( TMath::Power( 1 - b, 4 ) ) ) * d );
}
void
AnaMuon::GetTriggerSF_v17_custom2( const CONF::ConfType& conf, const detector::MCP_TYPE& mu_type , const detector::EGAMMA_TYPE& el_type ,
				   std::vector<TLorentzVector>& muons,
				   std::vector<TLorentzVector>& electrons,
				   std::vector<ftype>& muon_charges,
				   const unsigned long& run_number , const int& mu_trig ,
				   double& eff, double& err , int replica) {
#ifdef HAVE_COMMONANALYSIS
  assert(conf==CONF::LATEST && "Unsupported --conf for GetTriggerSF_v17 scaling");
  static SFProvider* m_sfProvTrig_DG5_pos_barrel = 0;
  static SFProvider* m_sfProvTrig_G6I_pos_barrel = 0;
  static SFProvider* m_sfProvTrig_JM_pos_barrel = 0;
  static SFProvider* m_sfProvTrig_L3L4_pos_barrel = 0;
  static SFProvider* m_sfProvTrig_DG5_pos_endcap = 0;
  static SFProvider* m_sfProvTrig_G6I_pos_endcap = 0;
  static SFProvider* m_sfProvTrig_JM_pos_endcap = 0;
  static SFProvider* m_sfProvTrig_L3L4_pos_endcap = 0;
  static SFProvider* m_sfProvTrig_DG5_neg_barrel = 0;
  static SFProvider* m_sfProvTrig_G6I_neg_barrel = 0;
  static SFProvider* m_sfProvTrig_JM_neg_barrel = 0;
  static SFProvider* m_sfProvTrig_L3L4_neg_barrel = 0;
  static SFProvider* m_sfProvTrig_DG5_neg_endcap = 0;
  static SFProvider* m_sfProvTrig_G6I_neg_endcap = 0;
  static SFProvider* m_sfProvTrig_JM_neg_endcap = 0;
  static SFProvider* m_sfProvTrig_L3L4_neg_endcap = 0;

  // value for initial seed, with a unique offset for each period / region / charge
  // const int seed = 1721;
  
  // period base
  const int seed = 0;
  const int oposB = 1000;
  const int onegB = 2000;
  const int oposE = 3000;
  const int onegE = 4000;
  // period offset
  const int oDG5 = 0;
  const int oG6I = 1;
  const int oJM = 2;
  const int oL3L4 = 3;

  // init on first event
  if(!m_sfProvTrig_DG5_pos_barrel) {
    const std::string file="data/mu18_eta_pt_SF_hists.root";
    //const std::string file="data/muon_trigger_SF_hists.root"; // AK 04/03/2013: this was MG
    // positive muons
    m_sfProvTrig_DG5_pos_barrel  = new SFProvider(file, "charge_pos_data_matched_barrel", "charge_pos_data_probe_barrel",
						  file, "charge_pos_mc_matched_barrel", "charge_pos_mc_probe_barrel",
						  SFProvider::EtaPt,1.0);
    m_sfProvTrig_DG5_pos_barrel->generateReplicas( NREPLICASF,  seed+oDG5+oposB );
    m_sfProvTrig_G6I_pos_barrel  = new SFProvider(file, "mu18_pos_data_matched_barrel", "mu18_pos_data_probe_barrel",
						  file, "mu18_pos_mc_matched_barrel", "mu18_pos_mc_probe_barrel",
						  SFProvider::EtaPt,1.0);
    m_sfProvTrig_G6I_pos_barrel->generateReplicas( NREPLICASF,  seed+oG6I+oposB );
    m_sfProvTrig_JM_pos_barrel   = new SFProvider(file, "medium_pos_data_matched_barrel", "medium_pos_data_probe_barrel",
						  file, "medium_pos_mc_matched_barrel", "medium_pos_mc_probe_barrel",
						  SFProvider::EtaPt,1.0);
    m_sfProvTrig_JM_pos_barrel->generateReplicas( NREPLICASF,  seed+oJM+oposB );
    m_sfProvTrig_L3L4_pos_barrel = new SFProvider(file, "rpc_pos_data_matched_barrel", "rpc_pos_data_probe_barrel",
						  file, "rpc_pos_mc_matched_barrel", "rpc_pos_mc_probe_barrel",
						  SFProvider::EtaPt,1.0);
    m_sfProvTrig_L3L4_pos_barrel->generateReplicas( NREPLICASF,  seed+oL3L4+oposB );
    m_sfProvTrig_DG5_pos_endcap  = new SFProvider(file, "charge_pos_data_matched_endcap", "charge_pos_data_probe_endcap",
						  file, "charge_pos_mc_matched_endcap", "charge_pos_mc_probe_endcap",
						  SFProvider::EtaPt,1.0);
    m_sfProvTrig_DG5_pos_endcap->generateReplicas( NREPLICASF,  seed+oDG5+oposE );
    m_sfProvTrig_G6I_pos_endcap  = new SFProvider(file, "mu18_pos_data_matched_endcap", "mu18_pos_data_probe_endcap",
						  file, "mu18_pos_mc_matched_endcap", "mu18_pos_mc_probe_endcap",
						  SFProvider::EtaPt,1.0);
    m_sfProvTrig_G6I_pos_endcap->generateReplicas( NREPLICASF,  seed+oG6I+oposE );
    m_sfProvTrig_JM_pos_endcap   = new SFProvider(file, "medium_pos_data_matched_endcap", "medium_pos_data_probe_endcap",
						  file, "medium_pos_mc_matched_endcap", "medium_pos_mc_probe_endcap",
						  SFProvider::EtaPt,1.0);
    m_sfProvTrig_JM_pos_endcap->generateReplicas( NREPLICASF,  seed+oJM+oposE );
    m_sfProvTrig_L3L4_pos_endcap = new SFProvider(file, "rpc_pos_data_matched_endcap", "rpc_pos_data_probe_endcap",
						  file, "rpc_pos_mc_matched_endcap", "rpc_pos_mc_probe_endcap",
						  SFProvider::EtaPt,1.0);
    m_sfProvTrig_L3L4_pos_endcap->generateReplicas( NREPLICASF,  seed+oL3L4+oposE );
    // negative muons
    m_sfProvTrig_DG5_neg_barrel  = new SFProvider(file, "charge_neg_data_matched_barrel", "charge_neg_data_probe_barrel",
						  file, "charge_neg_mc_matched_barrel", "charge_neg_mc_probe_barrel",
						  SFProvider::EtaPt,1.0);
    m_sfProvTrig_DG5_neg_barrel->generateReplicas( NREPLICASF,  seed+oDG5+onegB );
    m_sfProvTrig_G6I_neg_barrel  = new SFProvider(file, "mu18_neg_data_matched_barrel", "mu18_neg_data_probe_barrel",
						  file, "mu18_neg_mc_matched_barrel", "mu18_neg_mc_probe_barrel",
						  SFProvider::EtaPt,1.0);
    m_sfProvTrig_G6I_neg_barrel->generateReplicas( NREPLICASF,  seed+oG6I+onegB );
    m_sfProvTrig_JM_neg_barrel   = new SFProvider(file, "medium_neg_data_matched_barrel", "medium_neg_data_probe_barrel",
						  file, "medium_neg_mc_matched_barrel", "medium_neg_mc_probe_barrel",
						  SFProvider::EtaPt,1.0);
    m_sfProvTrig_JM_neg_barrel->generateReplicas( NREPLICASF,  seed+oJM+onegB );
    m_sfProvTrig_L3L4_neg_barrel = new SFProvider(file, "rpc_neg_data_matched_barrel", "rpc_neg_data_probe_barrel",
						  file, "rpc_neg_mc_matched_barrel", "rpc_neg_mc_probe_barrel",
						  SFProvider::EtaPt,1.0);
    m_sfProvTrig_L3L4_neg_barrel->generateReplicas( NREPLICASF,  seed+oL3L4+onegB );
    m_sfProvTrig_DG5_neg_endcap  = new SFProvider(file, "charge_neg_data_matched_endcap", "charge_neg_data_probe_endcap",
						  file, "charge_neg_mc_matched_endcap", "charge_neg_mc_probe_endcap",
						  SFProvider::EtaPt,1.0);
    m_sfProvTrig_DG5_neg_endcap->generateReplicas( NREPLICASF,  seed+oDG5+onegE );
    m_sfProvTrig_G6I_neg_endcap  = new SFProvider(file, "mu18_neg_data_matched_endcap", "mu18_neg_data_probe_endcap",
						  file, "mu18_neg_mc_matched_endcap", "mu18_neg_mc_probe_endcap",
						  SFProvider::EtaPt,1.0);
    m_sfProvTrig_G6I_neg_endcap->generateReplicas( NREPLICASF,  seed+oG6I+onegE );
    m_sfProvTrig_JM_neg_endcap   = new SFProvider(file, "medium_neg_data_matched_endcap", "medium_neg_data_probe_endcap",
						  file, "medium_neg_mc_matched_endcap", "medium_neg_mc_probe_endcap",
						  SFProvider::EtaPt,1.0);
    m_sfProvTrig_JM_neg_endcap->generateReplicas( NREPLICASF,  seed+oJM+onegE );
    m_sfProvTrig_L3L4_neg_endcap = new SFProvider(file, "rpc_neg_data_matched_endcap", "rpc_neg_data_probe_endcap",
						  file, "rpc_neg_mc_matched_endcap", "rpc_neg_mc_probe_endcap",
						  SFProvider::EtaPt,1.0);
    m_sfProvTrig_L3L4_neg_endcap->generateReplicas( NREPLICASF,  seed+oL3L4+onegE );
  }
  
  const unsigned long myRunNumber = run_number;

  // code below adapted from Max Bellomo, and from TrigMuonEfficiency (to combine multiple muons)
  Double_t rate_not_fired_data = 1.;
  Double_t rate_not_fired_mc   = 1.;
  // needed for uncertainty calculation
  Double_t inv_sq_eff_data    = 0.;
  Double_t inv_sq_eff_mc      = 0.;
  Double_t inv_sq_eff_data_el    = 0.;
  Double_t inv_sq_eff_mc_el      = 0.;
  Double_t inv_sq_eff_data_mu    = 0.;
  Double_t inv_sq_eff_mc_mu      = 0.;
  for(unsigned i=0; i<muons.size(); i++) {
    const int q = muon_charges[i]>0 ? 1 : -1;
    SFProvider *m_sfProvTrig = 0;
    TLorentzVector& muon = muons[i];
    if(q>0) {
      if(fabs(muon.Eta())<1.05) {
	if(myRunNumber>=179710 && myRunNumber<=183347)
	  m_sfProvTrig=m_sfProvTrig_DG5_pos_barrel;
	else if(myRunNumber>=183391 && myRunNumber<=186493)
	  m_sfProvTrig=m_sfProvTrig_G6I_pos_barrel;
	else if( (myRunNumber>=186516 && myRunNumber<=189090) || (myRunNumber>=189639 && myRunNumber<=191933) )
	  m_sfProvTrig=m_sfProvTrig_JM_pos_barrel;
	else if(myRunNumber>=189184 && myRunNumber<=189610)
	  m_sfProvTrig=m_sfProvTrig_L3L4_pos_barrel;
	else assert(0&&" unknown run number "&&BOOST_CURRENT_FUNCTION);
      } else {
	if(myRunNumber>=179710 && myRunNumber<=183347)
	  m_sfProvTrig=m_sfProvTrig_DG5_pos_endcap;
	else if(myRunNumber>=183391 && myRunNumber<=186493)
	  m_sfProvTrig=m_sfProvTrig_G6I_pos_endcap;
	else if( (myRunNumber>=186516 && myRunNumber<=189090) || (myRunNumber>=189639 && myRunNumber<=191933) )
	  m_sfProvTrig=m_sfProvTrig_JM_pos_endcap;
	else if(myRunNumber>=189184 && myRunNumber<=189610)
	  m_sfProvTrig=m_sfProvTrig_L3L4_pos_endcap;
	else assert(0&&" unknown run number "&&BOOST_CURRENT_FUNCTION);
      }
    } else {    // q<0
      if(fabs(muon.Eta())<1.05) {
	if(myRunNumber>=179710 && myRunNumber<=183347)
	  m_sfProvTrig=m_sfProvTrig_DG5_neg_barrel;
	else if(myRunNumber>=183391 && myRunNumber<=186493)
	  m_sfProvTrig=m_sfProvTrig_G6I_neg_barrel;
	else if( (myRunNumber>=186516 && myRunNumber<=189090) || (myRunNumber>=189639 && myRunNumber<=191933) )
	  m_sfProvTrig=m_sfProvTrig_JM_neg_barrel;
	else if(myRunNumber>=189184 && myRunNumber<=189610)
	  m_sfProvTrig=m_sfProvTrig_L3L4_neg_barrel;
	else assert(0&&" unknown run number "&&BOOST_CURRENT_FUNCTION);
      } else {
	if(myRunNumber>=179710 && myRunNumber<=183347)
	  m_sfProvTrig=m_sfProvTrig_DG5_neg_endcap;
	else if(myRunNumber>=183391 && myRunNumber<=186493)
	  m_sfProvTrig=m_sfProvTrig_G6I_neg_endcap;
	else if( (myRunNumber>=186516 && myRunNumber<=189090) || (myRunNumber>=189639 && myRunNumber<=191933) )
	  m_sfProvTrig=m_sfProvTrig_JM_neg_endcap;
	else if(myRunNumber>=189184 && myRunNumber<=189610)
	  m_sfProvTrig=m_sfProvTrig_L3L4_neg_endcap;
	else assert(0&&" unknown run number "&&BOOST_CURRENT_FUNCTION);
      }
    }

    // calculate central values and uncertainties
    assert(m_sfProvTrig);
    const double eff_data = replica<0 ? m_sfProvTrig->getEffDATA(muon) : m_sfProvTrig->getEffDATAreplica(muon,replica);
    const double eff_mc = replica<0 ? m_sfProvTrig->getEffMC(muon) : m_sfProvTrig->getEffMCreplica(muon,replica);
    const double err_data = m_sfProvTrig->getEffDATAError(muon);
    const double err_mc = m_sfProvTrig->getEffMCError(muon);
    rate_not_fired_data *= ( 1 - eff_data );
    rate_not_fired_mc   *= ( 1 - eff_mc );
    if( eff_data ) {
      inv_sq_eff_data += TMath::Power( (err_data/( 1 - eff_data )), 2 );
    }
    if( eff_mc ) {
      inv_sq_eff_mc += TMath::Power( (err_mc/( 1 - eff_mc )), 2 );
    }

  } // loop over muons

  // calculate event-wide trigger weight
  eff = 1.0;
  err = 0;
  // prevent events with no triggered electrons or muons
  if( ( muons.size() ) && ( TMath::Abs( 1 - rate_not_fired_mc ) > 0.0001 ) ) {
    eff    = ( 1 - rate_not_fired_data ) / ( 1 - rate_not_fired_mc );
    err    = AnaMuon::GetSFError( rate_not_fired_data, rate_not_fired_mc,
				  inv_sq_eff_data, inv_sq_eff_mc );
  }

  return;
  
#else
  assert(0&&NO_ANALYSIS_MSG);
#endif
}

// a FOURTH custom version of trigger scale factors that can be used with the ScaleFactorProvider package
// this implementation uses histograms from the MCP trigger group, but we are implementing all machinery by-hand.
// These particular histograms were made in December 2012, and should be used with 2011 data ONLY
// compared to AnaMuon::GetTriggerSF_v17_custom2, AnaMuon::GetTriggerSF_v17_custom4 adds eta-phi corrections
// on top of eta-pt. This version works with multiple muons (e.g., for Z selection).
void
AnaMuon::GetTriggerSF_v17_custom4( const CONF::ConfType& conf, const detector::MCP_TYPE& mu_type , const detector::EGAMMA_TYPE& el_type ,
				   std::vector<TLorentzVector>& muons,
				   std::vector<TLorentzVector>& electrons,
				   std::vector<ftype>& muon_charges,
				   const unsigned long& run_number , const int& mu_trig ,
				   double& eff, double& err , int replica) {
#ifdef HAVE_COMMONANALYSIS
  assert(conf==CONF::LATEST && "Unsupported --conf for GetTriggerSF_v17 scaling");
  assert(replica<0);
  static SFProvider* m_sfProvTrig_DG5_pos_barrel = 0;
  static SFProvider* m_sfProvTrig_G6I_pos_barrel = 0;
  static SFProvider* m_sfProvTrig_JM_pos_barrel = 0;
  static SFProvider* m_sfProvTrig_L3L4_pos_barrel = 0;
  static SFProvider* m_sfProvTrig_DG5_pos_endcap = 0;
  static SFProvider* m_sfProvTrig_G6I_pos_endcap = 0;
  static SFProvider* m_sfProvTrig_JM_pos_endcap = 0;
  static SFProvider* m_sfProvTrig_L3L4_pos_endcap = 0;
  static SFProvider* m_sfProvTrig_DG5_neg_barrel = 0;
  static SFProvider* m_sfProvTrig_G6I_neg_barrel = 0;
  static SFProvider* m_sfProvTrig_JM_neg_barrel = 0;
  static SFProvider* m_sfProvTrig_L3L4_neg_barrel = 0;
  static SFProvider* m_sfProvTrig_DG5_neg_endcap = 0;
  static SFProvider* m_sfProvTrig_G6I_neg_endcap = 0;
  static SFProvider* m_sfProvTrig_JM_neg_endcap = 0;
  static SFProvider* m_sfProvTrig_L3L4_neg_endcap = 0;
  
  // init on first event
  if(!m_sfProvTrig_DG5_pos_barrel) {
    const std::string file="data/mu18_eta_phi_SF_hists.root";
    //const std::string file="data/muon_trigger_SF_phi_averaging_syst_v4.root"; // AK 04/03/2013: this was MG
    // positive muons
    m_sfProvTrig_DG5_pos_barrel  = new SFProvider(file, "charge_pos_data_matched_barrel", "charge_pos_data_probe_barrel",
						  file, "charge_pos_mc_matched_barrel", "charge_pos_mc_probe_barrel",
						  SFProvider::EtaPhi,1.0);
    m_sfProvTrig_G6I_pos_barrel  = new SFProvider(file, "mu18_pos_data_matched_barrel", "mu18_pos_data_probe_barrel",
						  file, "mu18_pos_mc_matched_barrel", "mu18_pos_mc_probe_barrel",
						  SFProvider::EtaPhi,1.0);
    m_sfProvTrig_JM_pos_barrel   = new SFProvider(file, "medium_pos_data_matched_barrel", "medium_pos_data_probe_barrel",
						  file, "medium_pos_mc_matched_barrel", "medium_pos_mc_probe_barrel",
						  SFProvider::EtaPhi,1.0);
    m_sfProvTrig_L3L4_pos_barrel = new SFProvider(file, "rpc_pos_data_matched_barrel", "rpc_pos_data_probe_barrel",
						  file, "rpc_pos_mc_matched_barrel", "rpc_pos_mc_probe_barrel",
						  SFProvider::EtaPhi,1.0);
    m_sfProvTrig_DG5_pos_endcap  = new SFProvider(file, "charge_pos_data_matched_endcap", "charge_pos_data_probe_endcap",
						  file, "charge_pos_mc_matched_endcap", "charge_pos_mc_probe_endcap",
						  SFProvider::EtaPhi,1.0);
    m_sfProvTrig_G6I_pos_endcap  = new SFProvider(file, "mu18_pos_data_matched_endcap", "mu18_pos_data_probe_endcap",
						  file, "mu18_pos_mc_matched_endcap", "mu18_pos_mc_probe_endcap",
						  SFProvider::EtaPhi,1.0);
    m_sfProvTrig_JM_pos_endcap   = new SFProvider(file, "medium_pos_data_matched_endcap", "medium_pos_data_probe_endcap",
						  file, "medium_pos_mc_matched_endcap", "medium_pos_mc_probe_endcap",
						  SFProvider::EtaPhi,1.0);
    m_sfProvTrig_L3L4_pos_endcap = new SFProvider(file, "rpc_pos_data_matched_endcap", "rpc_pos_data_probe_endcap",
						  file, "rpc_pos_mc_matched_endcap", "rpc_pos_mc_probe_endcap",
						  SFProvider::EtaPhi,1.0);
    // negative muons
    m_sfProvTrig_DG5_neg_barrel  = new SFProvider(file, "charge_neg_data_matched_barrel", "charge_neg_data_probe_barrel",
						  file, "charge_neg_mc_matched_barrel", "charge_neg_mc_probe_barrel",
						  SFProvider::EtaPhi,1.0);
    m_sfProvTrig_G6I_neg_barrel  = new SFProvider(file, "mu18_neg_data_matched_barrel", "mu18_neg_data_probe_barrel",
						  file, "mu18_neg_mc_matched_barrel", "mu18_neg_mc_probe_barrel",
						  SFProvider::EtaPhi,1.0);
    m_sfProvTrig_JM_neg_barrel   = new SFProvider(file, "medium_neg_data_matched_barrel", "medium_neg_data_probe_barrel",
						  file, "medium_neg_mc_matched_barrel", "medium_neg_mc_probe_barrel",
						  SFProvider::EtaPhi,1.0);
    m_sfProvTrig_L3L4_neg_barrel = new SFProvider(file, "rpc_neg_data_matched_barrel", "rpc_neg_data_probe_barrel",
						  file, "rpc_neg_mc_matched_barrel", "rpc_neg_mc_probe_barrel",
						  SFProvider::EtaPhi,1.0);
    m_sfProvTrig_DG5_neg_endcap  = new SFProvider(file, "charge_neg_data_matched_endcap", "charge_neg_data_probe_endcap",
						  file, "charge_neg_mc_matched_endcap", "charge_neg_mc_probe_endcap",
						  SFProvider::EtaPhi,1.0);
    m_sfProvTrig_G6I_neg_endcap  = new SFProvider(file, "mu18_neg_data_matched_endcap", "mu18_neg_data_probe_endcap",
						  file, "mu18_neg_mc_matched_endcap", "mu18_neg_mc_probe_endcap",
						  SFProvider::EtaPhi,1.0);
    m_sfProvTrig_JM_neg_endcap   = new SFProvider(file, "medium_neg_data_matched_endcap", "medium_neg_data_probe_endcap",
						  file, "medium_neg_mc_matched_endcap", "medium_neg_mc_probe_endcap",
						  SFProvider::EtaPhi,1.0);
    m_sfProvTrig_L3L4_neg_endcap = new SFProvider(file, "rpc_neg_data_matched_endcap", "rpc_neg_data_probe_endcap",
						  file, "rpc_neg_mc_matched_endcap", "rpc_neg_mc_probe_endcap",
						  SFProvider::EtaPhi,1.0);
  }
  
  const unsigned long myRunNumber = run_number;

  // code below adapted from Max Bellomo, and from TrigMuonEfficiency (to combine multiple muons)
  Double_t rate_not_fired_data = 1.;
  Double_t rate_not_fired_mc   = 1.;
  // needed for uncertainty calculation
  Double_t inv_sq_eff_data    = 0.;
  Double_t inv_sq_eff_mc      = 0.;
  Double_t inv_sq_eff_data_el    = 0.;
  Double_t inv_sq_eff_mc_el      = 0.;
  Double_t inv_sq_eff_data_mu    = 0.;
  Double_t inv_sq_eff_mc_mu      = 0.;
  for(unsigned i=0; i<muons.size(); i++) {
    const int q = muon_charges[i]>0 ? 1 : -1;
    SFProvider *m_sfProvTrig = 0;
    TLorentzVector& muon = muons[i];
    if(q>0) {
      if(fabs(muon.Eta())<1.05) {
	if(myRunNumber>=179710 && myRunNumber<=183347)
	  m_sfProvTrig=m_sfProvTrig_DG5_pos_barrel;
	else if(myRunNumber>=183391 && myRunNumber<=186493)
	  m_sfProvTrig=m_sfProvTrig_G6I_pos_barrel;
	else if( (myRunNumber>=186516 && myRunNumber<=189090) || (myRunNumber>=189639 && myRunNumber<=191933) )
	  m_sfProvTrig=m_sfProvTrig_JM_pos_barrel;
	else if(myRunNumber>=189184 && myRunNumber<=189610)
	  m_sfProvTrig=m_sfProvTrig_L3L4_pos_barrel;
	else assert(0&&" unknown run number "&&BOOST_CURRENT_FUNCTION);
      } else {
	if(myRunNumber>=179710 && myRunNumber<=183347)
	  m_sfProvTrig=m_sfProvTrig_DG5_pos_endcap;
	else if(myRunNumber>=183391 && myRunNumber<=186493)
	  m_sfProvTrig=m_sfProvTrig_G6I_pos_endcap;
	else if( (myRunNumber>=186516 && myRunNumber<=189090) || (myRunNumber>=189639 && myRunNumber<=191933) )
	  m_sfProvTrig=m_sfProvTrig_JM_pos_endcap;
	else if(myRunNumber>=189184 && myRunNumber<=189610)
	  m_sfProvTrig=m_sfProvTrig_L3L4_pos_endcap;
	else assert(0&&" unknown run number "&&BOOST_CURRENT_FUNCTION);
      }
    } else {    // q<0
      if(fabs(muon.Eta())<1.05) {
	if(myRunNumber>=179710 && myRunNumber<=183347)
	  m_sfProvTrig=m_sfProvTrig_DG5_neg_barrel;
	else if(myRunNumber>=183391 && myRunNumber<=186493)
	  m_sfProvTrig=m_sfProvTrig_G6I_neg_barrel;
	else if( (myRunNumber>=186516 && myRunNumber<=189090) || (myRunNumber>=189639 && myRunNumber<=191933) )
	  m_sfProvTrig=m_sfProvTrig_JM_neg_barrel;
	else if(myRunNumber>=189184 && myRunNumber<=189610)
	  m_sfProvTrig=m_sfProvTrig_L3L4_neg_barrel;
	else assert(0&&" unknown run number "&&BOOST_CURRENT_FUNCTION);
      } else {
	if(myRunNumber>=179710 && myRunNumber<=183347)
	  m_sfProvTrig=m_sfProvTrig_DG5_neg_endcap;
	else if(myRunNumber>=183391 && myRunNumber<=186493)
	  m_sfProvTrig=m_sfProvTrig_G6I_neg_endcap;
	else if( (myRunNumber>=186516 && myRunNumber<=189090) || (myRunNumber>=189639 && myRunNumber<=191933) )
	  m_sfProvTrig=m_sfProvTrig_JM_neg_endcap;
	else if(myRunNumber>=189184 && myRunNumber<=189610)
	  m_sfProvTrig=m_sfProvTrig_L3L4_neg_endcap;
	else assert(0&&" unknown run number "&&BOOST_CURRENT_FUNCTION);
      }
    }

    // calculate central values and uncertainties
    assert(m_sfProvTrig);
    const double eff_data = m_sfProvTrig->getEffDATA(muon);
    const double eff_mc = m_sfProvTrig->getEffMC(muon);
    const double err_data = m_sfProvTrig->getEffDATAError(muon);
    const double err_mc = m_sfProvTrig->getEffMCError(muon);
    if(false) {
      std::cerr << "DEBUG: eff_data = " << eff_data << std::endl;
      std::cerr << "DEBUG: eff_mc = " << eff_mc << std::endl;
      std::cerr << "DEBUG: eff_data_mc = " << eff_data/eff_mc << std::endl;
    }
    rate_not_fired_data *= ( 1 - eff_data );
    rate_not_fired_mc   *= ( 1 - eff_mc );
    if( eff_data ) {
      inv_sq_eff_data += TMath::Power( (err_data/( 1 - eff_data )), 2 );
    }
    if( eff_mc ) {
      inv_sq_eff_mc += TMath::Power( (err_mc/( 1 - eff_mc )), 2 );
    }

  } // loop over muons

  // calculate event-wide trigger weight
  eff = 1.0;
  err = 0;
  // prevent events with no triggered electrons or muons
  if( ( muons.size() ) && ( TMath::Abs( 1 - rate_not_fired_mc ) > 0.0001 ) ) {
    eff    = ( 1 - rate_not_fired_data ) / ( 1 - rate_not_fired_mc );
    err    = AnaMuon::GetSFError( rate_not_fired_data, rate_not_fired_mc,
				  inv_sq_eff_data, inv_sq_eff_mc );
  }

  return;
  
#else
  assert(0&&NO_ANALYSIS_MSG);
#endif
}

// temporary variations to reduce statistical uncertainty
void
AnaMuon::GetTriggerSF_v17_custom2_ptindep( const CONF::ConfType& conf, const detector::MCP_TYPE& mu_type , const detector::EGAMMA_TYPE& el_type ,
				   std::vector<TLorentzVector>& muons,
				   std::vector<TLorentzVector>& electrons,
				   std::vector<ftype>& muon_charges,
				   const unsigned long& run_number , const int& mu_trig ,
				   double& eff, double& err , int replica) {
#ifdef HAVE_COMMONANALYSIS
  assert(conf==CONF::LATEST && "Unsupported --conf for GetTriggerSF_v17 scaling");
  static SFProvider* m_sfProvTrig_DG5_pos_barrel = 0;
  static SFProvider* m_sfProvTrig_G6I_pos_barrel = 0;
  static SFProvider* m_sfProvTrig_JM_pos_barrel = 0;
  static SFProvider* m_sfProvTrig_L3L4_pos_barrel = 0;
  static SFProvider* m_sfProvTrig_DG5_pos_endcap = 0;
  static SFProvider* m_sfProvTrig_G6I_pos_endcap = 0;
  static SFProvider* m_sfProvTrig_JM_pos_endcap = 0;
  static SFProvider* m_sfProvTrig_L3L4_pos_endcap = 0;
  static SFProvider* m_sfProvTrig_DG5_neg_barrel = 0;
  static SFProvider* m_sfProvTrig_G6I_neg_barrel = 0;
  static SFProvider* m_sfProvTrig_JM_neg_barrel = 0;
  static SFProvider* m_sfProvTrig_L3L4_neg_barrel = 0;
  static SFProvider* m_sfProvTrig_DG5_neg_endcap = 0;
  static SFProvider* m_sfProvTrig_G6I_neg_endcap = 0;
  static SFProvider* m_sfProvTrig_JM_neg_endcap = 0;
  static SFProvider* m_sfProvTrig_L3L4_neg_endcap = 0;

  // value for initial seed, with a unique offset for each period / region / charge
  // const int seed = 1721;
  
  // period base
  const int seed = 0;
  const int oposB = 1000;
  const int onegB = 2000;
  const int oposE = 3000;
  const int onegE = 4000;
  // period offset
  const int oDG5 = 0;
  const int oG6I = 1;
  const int oJM = 2;
  const int oL3L4 = 3;

  // init on first event
  if(!m_sfProvTrig_DG5_pos_barrel) {
    const std::string file="data/pt_independent_eta.root";
    //const std::string file="data/muon_trigger_SF_hists.root"; // AK 04/03/2013: this was MG
    // positive muons
    m_sfProvTrig_DG5_pos_barrel  = new SFProvider(file, "charge_pos_data_matched_barrel", "charge_pos_data_probe_barrel",
						  file, "charge_pos_mc_matched_barrel", "charge_pos_mc_probe_barrel",
						  SFProvider::Eta,1.0);
    m_sfProvTrig_DG5_pos_barrel->generateReplicas( NREPLICASF,  seed+oDG5+oposB );
    m_sfProvTrig_G6I_pos_barrel  = new SFProvider(file, "mu18_pos_data_matched_barrel", "mu18_pos_data_probe_barrel",
						  file, "mu18_pos_mc_matched_barrel", "mu18_pos_mc_probe_barrel",
						  SFProvider::Eta,1.0);
    m_sfProvTrig_G6I_pos_barrel->generateReplicas( NREPLICASF,  seed+oG6I+oposB );
    m_sfProvTrig_JM_pos_barrel   = new SFProvider(file, "medium_pos_data_matched_barrel", "medium_pos_data_probe_barrel",
						  file, "medium_pos_mc_matched_barrel", "medium_pos_mc_probe_barrel",
						  SFProvider::Eta,1.0);
    m_sfProvTrig_JM_pos_barrel->generateReplicas( NREPLICASF,  seed+oJM+oposB );
    m_sfProvTrig_L3L4_pos_barrel = new SFProvider(file, "rpc_pos_data_matched_barrel", "rpc_pos_data_probe_barrel",
						  file, "rpc_pos_mc_matched_barrel", "rpc_pos_mc_probe_barrel",
						  SFProvider::Eta,1.0);
    m_sfProvTrig_L3L4_pos_barrel->generateReplicas( NREPLICASF,  seed+oL3L4+oposB );
    m_sfProvTrig_DG5_pos_endcap  = new SFProvider(file, "charge_pos_data_matched_endcap", "charge_pos_data_probe_endcap",
						  file, "charge_pos_mc_matched_endcap", "charge_pos_mc_probe_endcap",
						  SFProvider::Eta,1.0);
    m_sfProvTrig_DG5_pos_endcap->generateReplicas( NREPLICASF,  seed+oDG5+oposE );
    m_sfProvTrig_G6I_pos_endcap  = new SFProvider(file, "mu18_pos_data_matched_endcap", "mu18_pos_data_probe_endcap",
						  file, "mu18_pos_mc_matched_endcap", "mu18_pos_mc_probe_endcap",
						  SFProvider::Eta,1.0);
    m_sfProvTrig_G6I_pos_endcap->generateReplicas( NREPLICASF,  seed+oG6I+oposE );
    m_sfProvTrig_JM_pos_endcap   = new SFProvider(file, "medium_pos_data_matched_endcap", "medium_pos_data_probe_endcap",
						  file, "medium_pos_mc_matched_endcap", "medium_pos_mc_probe_endcap",
						  SFProvider::Eta,1.0);
    m_sfProvTrig_JM_pos_endcap->generateReplicas( NREPLICASF,  seed+oJM+oposE );
    m_sfProvTrig_L3L4_pos_endcap = new SFProvider(file, "rpc_pos_data_matched_endcap", "rpc_pos_data_probe_endcap",
						  file, "rpc_pos_mc_matched_endcap", "rpc_pos_mc_probe_endcap",
						  SFProvider::Eta,1.0);
    m_sfProvTrig_L3L4_pos_endcap->generateReplicas( NREPLICASF,  seed+oL3L4+oposE );
    // negative muons
    m_sfProvTrig_DG5_neg_barrel  = new SFProvider(file, "charge_neg_data_matched_barrel", "charge_neg_data_probe_barrel",
						  file, "charge_neg_mc_matched_barrel", "charge_neg_mc_probe_barrel",
						  SFProvider::Eta,1.0);
    m_sfProvTrig_DG5_neg_barrel->generateReplicas( NREPLICASF,  seed+oDG5+onegB );
    m_sfProvTrig_G6I_neg_barrel  = new SFProvider(file, "mu18_neg_data_matched_barrel", "mu18_neg_data_probe_barrel",
						  file, "mu18_neg_mc_matched_barrel", "mu18_neg_mc_probe_barrel",
						  SFProvider::Eta,1.0);
    m_sfProvTrig_G6I_neg_barrel->generateReplicas( NREPLICASF,  seed+oG6I+onegB );
    m_sfProvTrig_JM_neg_barrel   = new SFProvider(file, "medium_neg_data_matched_barrel", "medium_neg_data_probe_barrel",
						  file, "medium_neg_mc_matched_barrel", "medium_neg_mc_probe_barrel",
						  SFProvider::Eta,1.0);
    m_sfProvTrig_JM_neg_barrel->generateReplicas( NREPLICASF,  seed+oJM+onegB );
    m_sfProvTrig_L3L4_neg_barrel = new SFProvider(file, "rpc_neg_data_matched_barrel", "rpc_neg_data_probe_barrel",
						  file, "rpc_neg_mc_matched_barrel", "rpc_neg_mc_probe_barrel",
						  SFProvider::Eta,1.0);
    m_sfProvTrig_L3L4_neg_barrel->generateReplicas( NREPLICASF,  seed+oL3L4+onegB );
    m_sfProvTrig_DG5_neg_endcap  = new SFProvider(file, "charge_neg_data_matched_endcap", "charge_neg_data_probe_endcap",
						  file, "charge_neg_mc_matched_endcap", "charge_neg_mc_probe_endcap",
						  SFProvider::Eta,1.0);
    m_sfProvTrig_DG5_neg_endcap->generateReplicas( NREPLICASF,  seed+oDG5+onegE );
    m_sfProvTrig_G6I_neg_endcap  = new SFProvider(file, "mu18_neg_data_matched_endcap", "mu18_neg_data_probe_endcap",
						  file, "mu18_neg_mc_matched_endcap", "mu18_neg_mc_probe_endcap",
						  SFProvider::Eta,1.0);
    m_sfProvTrig_G6I_neg_endcap->generateReplicas( NREPLICASF,  seed+oG6I+onegE );
    m_sfProvTrig_JM_neg_endcap   = new SFProvider(file, "medium_neg_data_matched_endcap", "medium_neg_data_probe_endcap",
						  file, "medium_neg_mc_matched_endcap", "medium_neg_mc_probe_endcap",
						  SFProvider::Eta,1.0);
    m_sfProvTrig_JM_neg_endcap->generateReplicas( NREPLICASF,  seed+oJM+onegE );
    m_sfProvTrig_L3L4_neg_endcap = new SFProvider(file, "rpc_neg_data_matched_endcap", "rpc_neg_data_probe_endcap",
						  file, "rpc_neg_mc_matched_endcap", "rpc_neg_mc_probe_endcap",
						  SFProvider::Eta,1.0);
    m_sfProvTrig_L3L4_neg_endcap->generateReplicas( NREPLICASF,  seed+oL3L4+onegE );
  }
  
  const unsigned long myRunNumber = run_number;

  // code below adapted from Max Bellomo, and from TrigMuonEfficiency (to combine multiple muons)
  Double_t rate_not_fired_data = 1.;
  Double_t rate_not_fired_mc   = 1.;
  // needed for uncertainty calculation
  Double_t inv_sq_eff_data    = 0.;
  Double_t inv_sq_eff_mc      = 0.;
  Double_t inv_sq_eff_data_el    = 0.;
  Double_t inv_sq_eff_mc_el      = 0.;
  Double_t inv_sq_eff_data_mu    = 0.;
  Double_t inv_sq_eff_mc_mu      = 0.;
  for(unsigned i=0; i<muons.size(); i++) {
    const int q = muon_charges[i]>0 ? 1 : -1;
    SFProvider *m_sfProvTrig = 0;
    TLorentzVector& muon = muons[i];
    if(q>0) {
      if(fabs(muon.Eta())<1.05) {
	if(myRunNumber>=179710 && myRunNumber<=183347)
	  m_sfProvTrig=m_sfProvTrig_DG5_pos_barrel;
	else if(myRunNumber>=183391 && myRunNumber<=186493)
	  m_sfProvTrig=m_sfProvTrig_G6I_pos_barrel;
	else if( (myRunNumber>=186516 && myRunNumber<=189090) || (myRunNumber>=189639 && myRunNumber<=191933) )
	  m_sfProvTrig=m_sfProvTrig_JM_pos_barrel;
	else if(myRunNumber>=189184 && myRunNumber<=189610)
	  m_sfProvTrig=m_sfProvTrig_L3L4_pos_barrel;
	else assert(0&&" unknown run number "&&BOOST_CURRENT_FUNCTION);
      } else {
	if(myRunNumber>=179710 && myRunNumber<=183347)
	  m_sfProvTrig=m_sfProvTrig_DG5_pos_endcap;
	else if(myRunNumber>=183391 && myRunNumber<=186493)
	  m_sfProvTrig=m_sfProvTrig_G6I_pos_endcap;
	else if( (myRunNumber>=186516 && myRunNumber<=189090) || (myRunNumber>=189639 && myRunNumber<=191933) )
	  m_sfProvTrig=m_sfProvTrig_JM_pos_endcap;
	else if(myRunNumber>=189184 && myRunNumber<=189610)
	  m_sfProvTrig=m_sfProvTrig_L3L4_pos_endcap;
	else assert(0&&" unknown run number "&&BOOST_CURRENT_FUNCTION);
      }
    } else {    // q<0
      if(fabs(muon.Eta())<1.05) {
	if(myRunNumber>=179710 && myRunNumber<=183347)
	  m_sfProvTrig=m_sfProvTrig_DG5_neg_barrel;
	else if(myRunNumber>=183391 && myRunNumber<=186493)
	  m_sfProvTrig=m_sfProvTrig_G6I_neg_barrel;
	else if( (myRunNumber>=186516 && myRunNumber<=189090) || (myRunNumber>=189639 && myRunNumber<=191933) )
	  m_sfProvTrig=m_sfProvTrig_JM_neg_barrel;
	else if(myRunNumber>=189184 && myRunNumber<=189610)
	  m_sfProvTrig=m_sfProvTrig_L3L4_neg_barrel;
	else assert(0&&" unknown run number "&&BOOST_CURRENT_FUNCTION);
      } else {
	if(myRunNumber>=179710 && myRunNumber<=183347)
	  m_sfProvTrig=m_sfProvTrig_DG5_neg_endcap;
	else if(myRunNumber>=183391 && myRunNumber<=186493)
	  m_sfProvTrig=m_sfProvTrig_G6I_neg_endcap;
	else if( (myRunNumber>=186516 && myRunNumber<=189090) || (myRunNumber>=189639 && myRunNumber<=191933) )
	  m_sfProvTrig=m_sfProvTrig_JM_neg_endcap;
	else if(myRunNumber>=189184 && myRunNumber<=189610)
	  m_sfProvTrig=m_sfProvTrig_L3L4_neg_endcap;
	else assert(0&&" unknown run number "&&BOOST_CURRENT_FUNCTION);
      }
    }

    // calculate central values and uncertainties
    assert(m_sfProvTrig);
    const double eff_data = replica<0 ? m_sfProvTrig->getEffDATA(muon) : m_sfProvTrig->getEffDATAreplica(muon,replica);
    const double eff_mc = replica<0 ? m_sfProvTrig->getEffMC(muon) : m_sfProvTrig->getEffMCreplica(muon,replica);
    const double err_data = m_sfProvTrig->getEffDATAError(muon);
    const double err_mc = m_sfProvTrig->getEffMCError(muon);
    rate_not_fired_data *= ( 1 - eff_data );
    rate_not_fired_mc   *= ( 1 - eff_mc );
    if( eff_data ) {
      inv_sq_eff_data += TMath::Power( (err_data/( 1 - eff_data )), 2 );
    }
    if( eff_mc ) {
      inv_sq_eff_mc += TMath::Power( (err_mc/( 1 - eff_mc )), 2 );
    }

  } // loop over muons

  // calculate event-wide trigger weight
  eff = 1.0;
  err = 0;
  // prevent events with no triggered electrons or muons
  if( ( muons.size() ) && ( TMath::Abs( 1 - rate_not_fired_mc ) > 0.0001 ) ) {
    eff    = ( 1 - rate_not_fired_data ) / ( 1 - rate_not_fired_mc );
    err    = AnaMuon::GetSFError( rate_not_fired_data, rate_not_fired_mc,
				  inv_sq_eff_data, inv_sq_eff_mc );
  }

  return;
  
#else
  assert(0&&NO_ANALYSIS_MSG);
#endif
}

// now charge-independent version
void
AnaMuon::GetTriggerSF_v17_custom2_qindep( const CONF::ConfType& conf, const detector::MCP_TYPE& mu_type , const detector::EGAMMA_TYPE& el_type ,
					  std::vector<TLorentzVector>& muons,
					  std::vector<TLorentzVector>& electrons,
					  std::vector<ftype>& muon_charges,
					  const unsigned long& run_number , const int& mu_trig ,
					  double& eff, double& err , int replica) {
#ifdef HAVE_COMMONANALYSIS
  assert(conf==CONF::LATEST && "Unsupported --conf for GetTriggerSF_v17 scaling");
  static SFProvider* m_sfProvTrig_DG5_barrel = 0;
  static SFProvider* m_sfProvTrig_G6I_barrel = 0;
  static SFProvider* m_sfProvTrig_JM_barrel = 0;
  static SFProvider* m_sfProvTrig_L3L4_barrel = 0;
  static SFProvider* m_sfProvTrig_DG5_endcap = 0;
  static SFProvider* m_sfProvTrig_G6I_endcap = 0;
  static SFProvider* m_sfProvTrig_JM_endcap = 0;
  static SFProvider* m_sfProvTrig_L3L4_endcap = 0;
  
  // init on first event
  if(!m_sfProvTrig_DG5_barrel) {
    const std::string file="data/charge_independent_eta_pt.root";
    // positive muons
    m_sfProvTrig_DG5_barrel  = new SFProvider(file, "charge_data_matched_barrel", "charge_data_probe_barrel",
						  file, "charge_mc_matched_barrel", "charge_mc_probe_barrel",
						  SFProvider::EtaPt,1.0);
    m_sfProvTrig_G6I_barrel  = new SFProvider(file, "mu18_data_matched_barrel", "mu18_data_probe_barrel",
						  file, "mu18_mc_matched_barrel", "mu18_mc_probe_barrel",
						  SFProvider::EtaPt,1.0);
    m_sfProvTrig_JM_barrel   = new SFProvider(file, "medium_data_matched_barrel", "medium_data_probe_barrel",
						  file, "medium_mc_matched_barrel", "medium_mc_probe_barrel",
						  SFProvider::EtaPt,1.0);
    m_sfProvTrig_L3L4_barrel = new SFProvider(file, "rpc_data_matched_barrel", "rpc_data_probe_barrel",
						  file, "rpc_mc_matched_barrel", "rpc_mc_probe_barrel",
						  SFProvider::EtaPt,1.0);
    m_sfProvTrig_DG5_endcap  = new SFProvider(file, "charge_data_matched_endcap", "charge_data_probe_endcap",
						  file, "charge_mc_matched_endcap", "charge_mc_probe_endcap",
						  SFProvider::EtaPt,1.0);
    m_sfProvTrig_G6I_endcap  = new SFProvider(file, "mu18_data_matched_endcap", "mu18_data_probe_endcap",
						  file, "mu18_mc_matched_endcap", "mu18_mc_probe_endcap",
						  SFProvider::EtaPt,1.0);
    m_sfProvTrig_JM_endcap   = new SFProvider(file, "medium_data_matched_endcap", "medium_data_probe_endcap",
						  file, "medium_mc_matched_endcap", "medium_mc_probe_endcap",
						  SFProvider::EtaPt,1.0);
    m_sfProvTrig_L3L4_endcap = new SFProvider(file, "rpc_data_matched_endcap", "rpc_data_probe_endcap",
						  file, "rpc_mc_matched_endcap", "rpc_mc_probe_endcap",
						  SFProvider::EtaPt,1.0);
  }
  
  const unsigned long myRunNumber = run_number;

  // code below adapted from Max Bellomo, and from TrigMuonEfficiency (to combine multiple muons)
  Double_t rate_not_fired_data = 1.;
  Double_t rate_not_fired_mc   = 1.;
  // needed for uncertainty calculation
  Double_t inv_sq_eff_data    = 0.;
  Double_t inv_sq_eff_mc      = 0.;
  Double_t inv_sq_eff_data_el    = 0.;
  Double_t inv_sq_eff_mc_el      = 0.;
  Double_t inv_sq_eff_data_mu    = 0.;
  Double_t inv_sq_eff_mc_mu      = 0.;
  for(unsigned i=0; i<muons.size(); i++) {
    const int q = muon_charges[i]>0 ? 1 : -1;
    SFProvider *m_sfProvTrig = 0;
    TLorentzVector& muon = muons[i];
    if(true) {
      if(fabs(muon.Eta())<1.05) {
	if(myRunNumber>=179710 && myRunNumber<=183347)
	  m_sfProvTrig=m_sfProvTrig_DG5_barrel;
	else if(myRunNumber>=183391 && myRunNumber<=186493)
	  m_sfProvTrig=m_sfProvTrig_G6I_barrel;
	else if( (myRunNumber>=186516 && myRunNumber<=189090) || (myRunNumber>=189639 && myRunNumber<=191933) )
	  m_sfProvTrig=m_sfProvTrig_JM_barrel;
	else if(myRunNumber>=189184 && myRunNumber<=189610)
	  m_sfProvTrig=m_sfProvTrig_L3L4_barrel;
	else assert(0&&" unknown run number "&&BOOST_CURRENT_FUNCTION);
      } else {
	if(myRunNumber>=179710 && myRunNumber<=183347)
	  m_sfProvTrig=m_sfProvTrig_DG5_endcap;
	else if(myRunNumber>=183391 && myRunNumber<=186493)
	  m_sfProvTrig=m_sfProvTrig_G6I_endcap;
	else if( (myRunNumber>=186516 && myRunNumber<=189090) || (myRunNumber>=189639 && myRunNumber<=191933) )
	  m_sfProvTrig=m_sfProvTrig_JM_endcap;
	else if(myRunNumber>=189184 && myRunNumber<=189610)
	  m_sfProvTrig=m_sfProvTrig_L3L4_endcap;
	else assert(0&&" unknown run number "&&BOOST_CURRENT_FUNCTION);
      }
    }

    // calculate central values and uncertainties
    assert(m_sfProvTrig);
    const double eff_data = m_sfProvTrig->getEffDATA(muon);
    const double eff_mc = m_sfProvTrig->getEffMC(muon);
    const double err_data = m_sfProvTrig->getEffDATAError(muon);
    const double err_mc = m_sfProvTrig->getEffMCError(muon);
    if(false) {
      std::cerr << "DEBUG: eff_data = " << eff_data << std::endl;
      std::cerr << "DEBUG: eff_mc = " << eff_mc << std::endl;
      std::cerr << "DEBUG: eff_data_mc = " << eff_data/eff_mc << std::endl;
    }
    rate_not_fired_data *= ( 1 - eff_data );
    rate_not_fired_mc   *= ( 1 - eff_mc );
    if( eff_data ) {
      inv_sq_eff_data += TMath::Power( (err_data/( 1 - eff_data )), 2 );
    }
    if( eff_mc ) {
      inv_sq_eff_mc += TMath::Power( (err_mc/( 1 - eff_mc )), 2 );
    }

  } // loop over muons

  // calculate event-wide trigger weight
  eff = 1.0;
  err = 0;
  // prevent events with no triggered electrons or muons
  if( ( muons.size() ) && ( TMath::Abs( 1 - rate_not_fired_mc ) > 0.0001 ) ) {
    eff    = ( 1 - rate_not_fired_data ) / ( 1 - rate_not_fired_mc );
    err    = AnaMuon::GetSFError( rate_not_fired_data, rate_not_fired_mc,
				  inv_sq_eff_data, inv_sq_eff_mc );
  }

  return;
  
#else
  assert(0&&NO_ANALYSIS_MSG);
#endif
}


// ISOLATION SCALE FACTORS
// 08/28/2013: separate SF or MCNLO (mctype==1) and PowhegHerwig (mctype==4)
void
AnaMuon::GetIsolationSF_v17( const CONF::ConfType& conf, const detector::MCP_TYPE& mu_type , 
			     std::vector<TLorentzVector>& muons,
			     const unsigned long& run_number ,
			     double& eff, double& err , 
			     int choice, int mctype, int replica) {
#ifdef HAVE_COMMONANALYSIS
  assert(conf==CONF::LATEST && "Unsupported --conf for GetTriggerSF_v17 scaling");
  static SFProvider *tcnom = 0;
  static SFProvider *tcup = 0;
  static SFProvider *tcdown = 0;
  const int seed = 6000; // 1721
  if(!tcnom) {
    assert(mctype==5 || mctype==4 || mctype==1);
    SFProvider::Metric metric = SFProvider::Pt; //SFProvider::EtaPt; // 09/19/2013: testing 2d SF
    const bool is2d = (metric == SFProvider::EtaPt);
    std::string namenom = mctype==5 ? "data/IsolationSF_vsPt.root" : (mctype==1 ? "data/Iso_MCAtNLO_SF_vsPt.root" : "data/Iso_PowhegJimmy_SF_vsPt.root");
    if(is2d) {
      namenom = mctype==5 ? "data/Iso_PowhegPythia_SF_vsEtaPt.root" : (mctype==1 ? "data/Iso_MCAtNLO_SF_vsEtaPt.root" : "data/Iso_PowhegJimmy_SF_vsEtaPt.root");
    }
    tcnom = new SFProvider(namenom,"SF","EffMC","EffData",metric,1.0);
    tcnom->generateReplicas(NREPLICASF,seed);
    //09/19/2013: tcnom only available for 2d reweighting
    tcup = is2d ? tcnom : new SFProvider("data/IsolationSF_vsPtup.root","SF","","",metric,1.0);
    tcdown = is2d ? tcnom : new SFProvider("data/IsolationSF_vsPtdown.root","SF","","",metric,1.0);
  }
  if(choice>0) assert(replica<0 && "AnaMuon::GetIsolationSF_v17 replicas only available for choice==0");
  SFProvider *tc = choice==0 ? tcnom : (choice==1 ? tcdown : tcup);
  err = 0; // not used
  eff = 1.0;
  for(unsigned i=0; i<muons.size(); i++) {
    const double ceff = replica<0 ? tc->getSF(muons[i]) : tc->getSFreplica(muons[i],replica);
    eff *=  ceff;
  }
#else
  assert(0&&NO_ANALYSIS_MSG);
#endif
}

// old version using 2 GeV binning, used for 2011 W/Z cutflow comparison
void
AnaMuon::GetIsolationSF_v17_old( const CONF::ConfType& conf, const detector::MCP_TYPE& mu_type , 
				 std::vector<TLorentzVector>& muons,
				 const unsigned long& run_number ,
				 double& eff, double& err , int replica) {
#ifdef HAVE_COMMONANALYSIS
  assert(conf==CONF::LATEST && "Unsupported --conf for GetTriggerSF_v17 scaling");
  static std::string datapath("data/eff_cbiso40rel01_mu_probe_pt_TP.root");
  static SFProvider *tc = 0;
  if(!tc) {
    tc = new SFProvider(datapath,"SF","EffMC","EffData",SFProvider::Pt,1.0);
    tc->generateReplicas(NREPLICASF);
  }
  err = 0;
  eff = 1.0;
  for(unsigned i=0; i<muons.size(); i++) {
    eff *=  replica<0 ? tc->getSF(muons[i]) : tc->getSFreplica(muons[i],replica);
  }
#else
  assert(0&&NO_ANALYSIS_MSG);
#endif
}


// A wrapper function that selects the correct efficiency scaling class
// conf selects an SVN release version; mu_type selects between staco/muid/loose/tight etc
void
AnaMuon::mcp_effscale( const CONF::ConfType& conf, const DATARANGE::DataRange& data_range, 
		       const boost::shared_ptr<const AnaMuon>& muon , const detector::MCP_TYPE& mu_type,
		       const unsigned long& run_number, const int runRange,
		       const std::vector<double>& int_lumi, const std::vector<std::string>& run_periods,
		       const int& flag,
		       double *effs,
		       double& eff, double& errstat, double& errsys ) {
#ifdef HAVE_COMMONANALYSIS
  using namespace detector;
  assert(conf==CONF::LATEST);
  return AnaMuon::mcp_effscale_apply_v17<Analysis::AnalysisMuonConfigurableScaleFactors>(muon,mu_type,run_number,runRange,int_lumi,run_periods,flag,effs,eff,errstat,errsys);
#else
  assert(0&&NO_ANALYSIS_MSG);
#endif
}

// A wrapper function that selects the correct smearing class
boost::shared_ptr<const AnaMuon>
AnaMuon::mcp_smeared( const CONF::ConfType& conf, const DATARANGE::DataRange& data_range,
		      const boost::shared_ptr<const AnaMuon>& muon , const detector::MCP_TYPE& mu_type ,
		      const unsigned long& event_number , const size_t& muon_number ,
		      const int& scale, const std::string& fun ) {
#ifdef HAVE_COMMONANALYSIS
  assert(conf==CONF::LATEST);
  return AnaMuon::mcp_smeared_apply_v17<MuonSmear::SmearingClass>(muon,mu_type,event_number,muon_number,scale,"Data11","Rel17",fun);
#else
  assert(0&&NO_ANALYSIS_MSG);
#endif
}

const AnaParticle::ftype AnaMuon::et_cone_corr( const ConeSize& cone , unsigned int nvtxs ) const {
#ifdef HAVE_COMMONANALYSIS
  static CorrectCaloIso mcp20;
  static CorrectCaloIso mcp30;
  static CorrectCaloIso mcp40;
  // note: CorrectCaloIso is hardcoded to use MeV
  // note2: including "2011" from the end of every correction string uses new quadratic corrections!
  switch (cone) {
  case CONE20:
    return _et_cone[cone] - mcp20.GetCorrectionEtCone(nvtxs,eta(),"cone20Comb2011")/1000.0;
    break;
  case CONE30:
    return _et_cone[cone] - mcp30.GetCorrectionEtCone(nvtxs,eta(),"cone30Comb2011")/1000.0;
    break;
  case CONE40:
    return _et_cone[cone] - mcp40.GetCorrectionEtCone(nvtxs,eta(),"cone40Comb2011")/1000.0;
    break;
  default:
    assert(0 && "Unsupported et cone for MuonIsolationCorrection");
  }
  assert(false);
  return 0;
#else
  assert(0&&NO_ANALYSIS_MSG);
  return 0;
#endif
}

#include "DataGraphics/DgCollection.hh"
const bool AnaMuon::passes_trigger_matching_v17() const {
  const unsigned int rnum = DataGraphics::dg::event_info().get_run_number();
  if(rnum <= 186493) {
    return trigger_match_dr("EF_mu18_MG")<0.2;
  }
  else if(rnum < 200804){
    return trigger_match_dr("EF_mu18_MG_medium")<0.2;
  }
  else { // 2012 data
    return trigger_match_dr("EF_mu18_medium")<0.2;
  }
}

int AnaMuon::NREPLICASF = 0;
