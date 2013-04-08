#include "TrigFTKAna_config.h"

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include "TrigFTKAna/Detector.hpp"
#include "TrigFTKAna/AnaJet.hpp"
#include "TrigFTKAna/AnaTrack.hpp"
#include "TrigFTKAna/Conference.hpp"
#ifdef HAVE_COMMONANALYSIS
#include "JetUncertainties/JESUncertaintyProvider.h"
#include "JetUncertainties/MultijetJESUncertaintyProvider.h"
#include "JetResolution/JERProvider.h"
#include "ApplyJetCalibration/ApplyJetCalibration.h"
#include "ApplyJetResolutionSmearing/ApplyJetSmearing.h"
#endif
#include <TRandom3.h>
#include <TMath.h>


// A wrapper function that selects the correct calibrating function to call
boost::shared_ptr<const AnaJet>
AnaJet::jetetmiss_calibrate( const CONF::ConfType& conf ,
			     const boost::shared_ptr<const AnaJet>& jet ,
			     const unsigned long& event_number , const int& jet_number ,
			     const double& mu , const int& npv , const bool& is_mc ,
			     const bool& calibrate, const bool& smear)
{
  assert( is_mc || !smear ); // do not want to smear data
  switch(conf) {
  case CONF::SUSI:
  case CONF::LATEST:
    return AnaJet::jet_CalibrationTools_v17(jet,event_number,jet_number,mu,npv,is_mc,calibrate,smear,conf);
    break;
  default:
    assert(0&&"Unsupported --conf");
  }
}

// Alternative version that only differs in the way JER systematics are applied (as done for 2011 w/z inclusive measurement)
boost::shared_ptr<const AnaJet>
AnaJet::jetetmiss_calibrate2( const CONF::ConfType& conf ,
			      const boost::shared_ptr<const AnaJet>& jet ,
			      const unsigned long& event_number , const int& jet_number ,
			      const double& mu , const int& npv , const bool& is_mc ,
			      const bool& calibrate, const int& smear)
{
  assert( is_mc || !smear ); // do not want to smear data
  assert(smear==0 || smear==1 || smear==-1);
  switch(conf) {
  case CONF::SUSI:
  case CONF::LATEST:
    return AnaJet::jet_CalibrationTools_v17_v2(jet,event_number,jet_number,mu,npv,is_mc,calibrate,smear,conf);
    break;
  default:
    assert(0&&"Unsupported --conf");
  }
}

// A wrapper function for JES shifts
boost::shared_ptr<const AnaJet>
AnaJet::jetetmiss_scale( const CONF::ConfType& conf ,
			 const boost::shared_ptr<const AnaJet>& jet ,
			 const unsigned long& event_number , const int& njets , const double& drmin , const int& jet_number ,
			 const double& mu , const int& npv , const std::string& fun , bool isbjet)
{
  // possibilities for fun:
  //  JESUP , JESDOWN
  double direction;
  JES_VARIATION jvar = JESVAR_JES;
  if( fun=="JESUP" ) { direction = 1.; }
  else if( fun=="JESDOWN" ) { direction = -1.; }
  else if( fun=="NPVUP" ) { direction = 1.; jvar = JESVAR_NPV; }
  else if( fun=="NPVDOWN" ) { direction = -1.; jvar = JESVAR_NPV; }
  else if( fun=="MUUP" ) { direction = 1.; jvar = JESVAR_MU; }
  else if( fun=="MUDOWN" ) { direction = -1.; jvar = JESVAR_MU; }
  else if( fun=="NPVMUUP" ) { direction = 1.; jvar = JESVAR_NPVMU; }
  else if( fun=="NPVMUDOWN" ) { direction = -1.; jvar = JESVAR_NPVMU; }
  else assert(0&&"Unsupported fun");
  switch(conf) {
  case CONF::SUSI:
  case CONF::LATEST:
    if( njets > 1 ) return jet_multijetJESpileupUncertainty_v17(jet,mu,npv,drmin,direction,conf, isbjet);
    else return jet_JESpileupUncertainty_v17(jet,mu,npv,direction,conf,jvar);
    break;
  default:
    assert(0&&"Unsupported --conf");
  }
}


// release 17 Functions
// ===========================

// details:https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/JESUncertaintyProvider
const boost::shared_ptr<const AnaJet>
AnaJet::jet_JESpileupUncertainty_v17( const boost::shared_ptr<const AnaJet>& jet , const double& mu ,const int& nGoodPV  , const double& direction,const CONF::ConfType& conf , const JES_VARIATION& jvar )
{
#ifdef HAVE_COMMONANALYSIS
  static JESUncertaintyProvider *jes_calc_em4 = 0;
  static JESUncertaintyProvider *jes_calc_em6 = 0;
  static JESUncertaintyProvider *jes_calc_lc4 = 0;
  static JESUncertaintyProvider *jes_calc_lc6 = 0;
  JESUncertaintyProvider *jes_calc = 0;
   std::string jespath;
   std::string mctype;
   switch(conf) {
   case CONF::LATEST:   
     mctype= "mc11b";
     jespath= "CommonAnalysis/RootCore/data/JetUncertainties/JES_2011/Final/InsituJES2011_AllNuisanceParameters.config";
     break;
   case CONF::SUSI:
     mctype="Pythia8";
     jespath="CommonAnalysis/RootCore/data/JetUncertainties/JESUncertainty2012_FirstData.config" ;
     break;
   
   default:
     assert(0&&"Unsupported --conf");

   }



  if( jes_calc_em4==0 ) {
    jes_calc_em4 = new JESUncertaintyProvider(jespath, "AntiKt4EMJESTopoJets" , mctype );
    jes_calc_em4->initJES();
    jes_calc_em6 = new JESUncertaintyProvider( jespath,"AntiKt6EMJESTopoJets" , mctype );
    jes_calc_em6->initJES();
    jes_calc_lc4 = new JESUncertaintyProvider( jespath,"AntiKt4LCTopoJets" , mctype );
    jes_calc_lc4->initJES();
    jes_calc_lc6 = new JESUncertaintyProvider(jespath, "AntiKt6LCTopoJets" , mctype);
    jes_calc_lc6->initJES();
  }
  if(jet->is_atlas_anti_kt_cone_4_h1_topo()) jes_calc = jes_calc_em4;
  else if(jet->is_atlas_anti_kt_cone_6_h1_topo()) jes_calc = jes_calc_em6;
  else if(jet->is_atlas_anti_kt_cone_4_h1_lc_topo()) jes_calc = jes_calc_lc4;
  else if(jet->is_atlas_anti_kt_cone_6_h1_lc_topo()) jes_calc = jes_calc_lc6;
  else assert("Jet not supported by JESUncertaintyProvider");
  // make sure the pT >= 15 GeV, since otherwise JESuncertaintyCalc will complain
  // jets with pT < 15 GeV should be rejected anyway
  double uncert = 0.;
  if( jet->pt() >= 15. && std::fabs(jet->eta()) <= 4.5 && nGoodPV > 0 && mu > 0 ) {
    //std::cout << "DEBUG :: jet->pt() = " << jet->pt() << " , jet->eta() = " << jet->eta() << " , nGoodPV = " << nGoodPV << " , mu = " << mu << std::endl;
    /*  Summer 2012:  
        // Get total, default uncertainty EXCLUDING offset term 
	// (uncertainty components added in quadrature)
	double getRelUncert(double pT, double eta);
	// Get relative offset uncertainty only: each term separatly, and their sum in quad.
	double getRelNPVOffsetTerm(double pT, double eta, double NPV);
	double getRelMuOffsetTerm(double pT, double eta, double mu);
	double getRelOffsetUncert(double pT, double eta, double NPV, double mu);
    */
    switch(jvar) {
    case JESVAR_JES:
      uncert = jes_calc->getRelUncert( jet->pt() * 1000. , // This needs to be in MeV
				       jet->eta() );
      break;
    case JESVAR_NPV:
      uncert = jes_calc->getRelNPVOffsetTerm(jet->pt() * 1000. , jet->eta() , nGoodPV);
      break;
    case JESVAR_MU:
      uncert = jes_calc->getRelNPVOffsetTerm(jet->pt() * 1000. , jet->eta() , mu);
      break;
    case JESVAR_NPVMU:
      uncert = jes_calc->getRelOffsetUncert(jet->pt() * 1000. , jet->eta() , nGoodPV , mu);
      break;
    }
  }
  boost::shared_ptr<AnaJet> new_jet( boost::make_shared<AnaJet>(*jet) );
  new_jet->_momentum.SetPtEtaPhiM( jet->pt() + (direction*uncert*jet->pt()) , 
				   jet->eta() , jet->phi() , 
				   jet->m() + (direction*uncert*jet->m()) );
  
  // define parent
  new_jet->_uncorrected_parent = jet;
  return boost::const_pointer_cast<const AnaJet>( new_jet );
#else
  assert(0&&NO_ANALYSIS_MSG);
  return jet;
#endif
}

//details: https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/MultijetJESUncertaintyProvider
const boost::shared_ptr<const AnaJet>
AnaJet::jet_multijetJESpileupUncertainty_v17( const boost::shared_ptr<const AnaJet>& jet , const double& mu, const int& nGoodPV , const double& dRmin , const double& direction ,const CONF::ConfType& conf, bool isbjet)
{
#ifdef HAVE_COMMONANALYSIS
  static MultijetJESUncertaintyProvider *multijes_calc_em4 = 0;
  static MultijetJESUncertaintyProvider *multijes_calc_em6 = 0;
  static MultijetJESUncertaintyProvider *multijes_calc_lc4 = 0;
  static MultijetJESUncertaintyProvider *multijes_calc_lc6 = 0;
  MultijetJESUncertaintyProvider *multijes_calc = 0;

   switch(conf) {
  case CONF::LATEST:   
    break;
  case CONF::SUSI:
    //we dont' calibrate these jets yet.
    return jet;
   default:
     assert(0&&"Unsupported --conf");
  }
   std::string multijespath;
   std::string multijespathM;
   std::string mctype;
   switch(conf) {
   case CONF::LATEST:   
     mctype= "mc11b";
     multijespath="CommonAnalysis/RootCore/data/JetUncertainties/JES_2011/Final/MultijetJES_2011.config";
     multijespathM= "CommonAnalysis/RootCore/data/JetUncertainties/JES_2011/Final/InsituJES2011_AllNuisanceParameters.config";
     break;
   case CONF::SUSI:
     mctype="Pythia8";
     assert(false && "Please update the 2012 MultiJES paths");
     multijespath="CommonAnalysis/RootCore/data/JetUncertainties/JES_2011/Final/MultijetJES_2011.config";
     multijespathM="CommonAnalysis/RootCore/data/JetUncertainties/JESUncertainty2012_FirstData.config" ;
     break;
   
   default: assert(0&&"Unsupported --conf");

   }

   if( multijes_calc_em4==0 ) {
     multijes_calc_em4 = new MultijetJESUncertaintyProvider(multijespath, multijespathM,"AntiKt4EMJESTopoJets",mctype );
     multijes_calc_em4->initMultijet();
     multijes_calc_em6 = new MultijetJESUncertaintyProvider(multijespath, multijespathM,"AntiKt6EMJESTopoJets",mctype );
     multijes_calc_em6->initMultijet();
     multijes_calc_lc4 = new MultijetJESUncertaintyProvider(multijespath, multijespathM,"AntiKt4LCTopoJets",mctype );
     multijes_calc_lc4->initMultijet();
     multijes_calc_lc6 = new MultijetJESUncertaintyProvider(multijespath, multijespathM,"AntiKt6LCTopoJets",mctype );
     multijes_calc_lc6->initMultijet();
   }

  if(jet->is_atlas_anti_kt_cone_4_h1_topo()) multijes_calc = multijes_calc_em4;
  else if(jet->is_atlas_anti_kt_cone_6_h1_topo()) multijes_calc = multijes_calc_em6;
  else if(jet->is_atlas_anti_kt_cone_4_h1_lc_topo()) multijes_calc = multijes_calc_lc4;
  else if(jet->is_atlas_anti_kt_cone_6_h1_lc_topo()) multijes_calc = multijes_calc_lc6;
  else assert("Jet not supported by MultijetJESUncertaintyProvider");

  // make sure the pT >= 15 GeV, since otherwise multiJESuncertaintyCalc will complain
  // jets with pT < 15 GeV should be rejected anyway
  double uncert = 0.;
  if( jet->pt() >= 15. && std::abs(jet->eta()) <= 4.5 ) {
    uncert = multijes_calc->getRelUncert( jet->pt() * 1000. , // in MeV
					  jet->eta() , //fabs(jet->eta())<2.9 ? jet->eta() : copysign(2.8999,jet->eta()) ,
					  dRmin ,
					  direction>0. ? true : false , // true for up, false for down
					  nGoodPV ,mu,isbjet);
  }

  boost::shared_ptr<AnaJet> new_jet( boost::make_shared<AnaJet>(*jet) );
  new_jet->_momentum.SetPtEtaPhiM( jet->pt() + (direction*uncert*jet->pt()) ,
                                   jet->eta() , jet->phi() ,
                                   jet->m() + (direction*uncert*jet->m()) );

  // define parent
  new_jet->_uncorrected_parent = jet;
  return boost::const_pointer_cast<const AnaJet>( new_jet );
#else
  assert(0&&NO_ANALYSIS_MSG);
  return jet;
#endif
}


/* details: https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/JetCalibrationToolsWinter2011
   NOTE: a good vertex has to have at least two tracks
*/
const boost::shared_ptr<const AnaJet>
AnaJet::jet_CalibrationTools_v17( const boost::shared_ptr<const AnaJet>& jet , const unsigned long& evnum , const int& jetnum , 
				  const double& mu, const int& nGoodPV , const bool& is_mc , const bool& calibrate, const bool& smear ,const CONF::ConfType& conf)
{
#ifdef HAVE_COMMONANALYSIS
  switch(conf) {
  case CONF::LATEST:   
    break;
  case CONF::SUSI:
    //we dont' calibrate these jets yet.
    return jet;
   default:
     assert(0&&"Unsupported --conf");
  }

  static std::string calpath("CommonAnalysis/RootCore/data/ApplyJetCalibration/CalibrationConfigs/InsituJES_2011_Preliminary.config");
  static std::string smearpath( "CommonAnalysis/RootCore/data/JetResolution/JERProviderPlots.root" );
  static JetCalibrationTool *jetcal_em4 = 0;
  static JetCalibrationTool *jetcal_em6 = 0;
  static JetCalibrationTool *jetcal_lc4 = 0;
  static JetCalibrationTool *jetcal_lc6 = 0;
  static JetSmearingTool *jetsmear_em4 = 0;
  static JetSmearingTool *jetsmear_em6 = 0;
  static JetSmearingTool *jetsmear_lc4 = 0;
  static JetSmearingTool *jetsmear_lc6 = 0;
  
  JetCalibrationTool *jetcal = 0;
  JetSmearingTool *jetsmear = 0;

  if( jetcal_em4==0 ) {
    jetcal_em4 = new JetCalibrationTool( "AntiKt4TopoEM" , calpath , !is_mc );
    jetcal_em6 = new JetCalibrationTool( "AntiKt6TopoEM" , calpath , !is_mc );
    jetcal_lc4 = new JetCalibrationTool( "AntiKt4LCTopo" , calpath , !is_mc );
    jetcal_lc6 = new JetCalibrationTool( "AntiKt6LCTopo" , calpath , !is_mc );
    jetcal_em4->UseGeV(true);
    jetcal_em6->UseGeV(true);
    jetcal_lc4->UseGeV(true);
    jetcal_lc6->UseGeV(true);
  }

  if( jetsmear_em4==0 ) {
    jetsmear_em4 = new JetSmearingTool( "AntiKt4TopoEM" , smearpath );
    jetsmear_em6 = new JetSmearingTool( "AntiKt6TopoEM" , smearpath );
    jetsmear_lc4 = new JetSmearingTool( "AntiKt4LCTopo" , smearpath );
    jetsmear_lc6 = new JetSmearingTool( "AntiKt6LCTopo" , smearpath );
    jetsmear_em4->UseGeV(true);
    jetsmear_em6->UseGeV(true);
    jetsmear_lc4->UseGeV(true);
    jetsmear_lc6->UseGeV(true);
  }

  if(jet->is_atlas_anti_kt_cone_4_h1_topo()) {
    jetcal = jetcal_em4;
    jetsmear = jetsmear_em4;
  }
  else if(jet->is_atlas_anti_kt_cone_6_h1_topo()) {
    jetcal = jetcal_em6;
    jetsmear = jetsmear_em6;
  }
  else if(jet->is_atlas_anti_kt_cone_4_h1_lc_topo()) {
    jetcal = jetcal_lc4;
    jetsmear = jetsmear_lc4;
  }
  else if(jet->is_atlas_anti_kt_cone_6_h1_lc_topo()) {
    jetcal = jetcal_lc6;
    jetsmear = jetsmear_lc6;
  }
  else assert("Jet not supported by JESCalibrationTool");

  const bool is_lc = jet->is_atlas_anti_kt_cone_4_h1_lc_topo() || jet->is_atlas_anti_kt_cone_6_h1_lc_topo();

  // seed per WZ cutflow
  // source: https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/WZCommonMETUtilityHowTo
  // jetsmear->SetSeed( evnum*100+20110706*jetnum - 3);

  TLorentzVector calib_jet;
  // calibrate jet, including origin correction - this is used for actual analysis-level jets
  if(calibrate) {
    if( false ) {
      // for debugging...
      std::cout << "AnaJet: " 
		<< jet->_em_scale_momentum.E() << " " 
		<< jet->_em_scale_momentum.Eta() << " " 
		<< jet->EtaOrigin() << " " 
		<< jet->PhiOrigin() << " " 
		<< jet->MOrigin() << " " 
		<< mu << " " 
		<< nGoodPV << endl;
    }
    if(false) {
      // more debugging...
      std::cout << "AnaJet const_scale: " << (jet->has_cs_scale()?"YES":"NO") << " " << jet->_cs_scale_momentum.E()  << " " << jet->_em_scale_momentum.E() << std::endl;
    }
    calib_jet = jetcal->ApplyOffsetEtaJES( is_lc && jet->has_cs_scale() ? jet->_cs_scale_momentum.E() : jet->_em_scale_momentum.E(),
					   jet->_em_scale_momentum.Eta() ,
					   jet->EtaOrigin() ,
					   jet->PhiOrigin() ,
					   jet->MOrigin() ,
	 				   mu, nGoodPV );
  }
  // just copy over 4vector from AnaJet - this is used for LC jets used in MET recomputation
  // This follow the simplified 2011 W/Z recommendation for MET uncertainty due to JES
  else {
    assert(smear); // if we're not calibrating, we must be smearing for MET recomputation
    calib_jet = jet->four_vector();
  }

  // MC oversmearing for systematic studies
  // details: https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/JetEnergyResolutionProviderForPhysicsAnalysis
  if( smear ) jetsmear->SmearJet_SystRel17( calib_jet );

  boost::shared_ptr<AnaJet> new_jet( boost::make_shared<AnaJet>(*jet) );
  new_jet->_momentum = calib_jet;
  //new_jet->print();
  //jet->print();

  // define parent
  new_jet->_uncorrected_parent = jet;
  
  return boost::const_pointer_cast<const AnaJet>( new_jet );
#else
  assert(0&&NO_ANALYSIS_MSG);
  return jet;
#endif
}

/* details: https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/JetCalibrationToolsWinter2011
   NOTE: a good vertex has to have at least two tracks
   THIS VERSION DIFFERS FROM THE ABOVE IN ONLY ONE WAY:
   it uses JERProvider *directly*, rather than through JetSmearingTool.
   This allows to fine-tune the application of SigmaUp and SigmaDown variations, per 2011 inclusive recommendation
*/
const boost::shared_ptr<const AnaJet>
AnaJet::jet_CalibrationTools_v17_v2( const boost::shared_ptr<const AnaJet>& jet , const unsigned long& evnum , const int& jetnum , 
				     const double& mu, const int& nGoodPV , const bool& is_mc , const bool& calibrate, 
				     const int& smear ,const CONF::ConfType& conf)
{
#ifdef HAVE_COMMONANALYSIS
  switch(conf) {
  case CONF::LATEST:   
    break;
  case CONF::SUSI:
    //we dont' calibrate these jets yet.
    return jet;
   default:
     assert(0&&"Unsupported --conf");
  }

  static TRandom3 *m_random;
  static std::string calpath("CommonAnalysis/RootCore/data/ApplyJetCalibration/CalibrationConfigs/InsituJES_2011_Preliminary.config");
  static std::string smearpath( "CommonAnalysis/RootCore/data/JetResolution/JERProviderPlots.root" );
  static JetCalibrationTool *jetcal_em4 = 0;
  static JetCalibrationTool *jetcal_em6 = 0;
  static JetCalibrationTool *jetcal_lc4 = 0;
  static JetCalibrationTool *jetcal_lc6 = 0;
  static JERProvider *jetsmear_em4 = 0;
  static JERProvider *jetsmear_em6 = 0;
  static JERProvider *jetsmear_lc4 = 0;
  static JERProvider *jetsmear_lc6 = 0;
  
  JetCalibrationTool *jetcal = 0;
  JERProvider *jetsmear = 0;

  if( jetcal_em4==0 ) {
    jetcal_em4 = new JetCalibrationTool( "AntiKt4TopoEM" , calpath , !is_mc );
    jetcal_em6 = new JetCalibrationTool( "AntiKt6TopoEM" , calpath , !is_mc );
    jetcal_lc4 = new JetCalibrationTool( "AntiKt4LCTopo" , calpath , !is_mc );
    jetcal_lc6 = new JetCalibrationTool( "AntiKt6LCTopo" , calpath , !is_mc );
    jetcal_em4->UseGeV(true);
    jetcal_em6->UseGeV(true);
    jetcal_lc4->UseGeV(true);
    jetcal_lc6->UseGeV(true);
  }

  if( jetsmear_em4==0 ) {
    m_random = new TRandom3();
    jetsmear_em4 = new JERProvider( "AntiKt4TopoJES" , "Truth", smearpath );
    jetsmear_em6 = new JERProvider( "AntiKt6TopoJES" , "Truth", smearpath );
    jetsmear_lc4 = new JERProvider( "AntiKt4LCTopoJES" , "Truth", smearpath );
    jetsmear_lc6 = new JERProvider( "AntiKt6LCTopoJES" , "Truth", smearpath );
    jetsmear_em4->init();
    jetsmear_em6->init();
    jetsmear_lc4->init();
    jetsmear_lc6->init();
  }

  if(jet->is_atlas_anti_kt_cone_4_h1_topo()) {
    jetcal = jetcal_em4;
    jetsmear = jetsmear_em4;
  }
  else if(jet->is_atlas_anti_kt_cone_6_h1_topo()) {
    jetcal = jetcal_em6;
    jetsmear = jetsmear_em6;
  }
  else if(jet->is_atlas_anti_kt_cone_4_h1_lc_topo()) {
    jetcal = jetcal_lc4;
    jetsmear = jetsmear_lc4;
  }
  else if(jet->is_atlas_anti_kt_cone_6_h1_lc_topo()) {
    jetcal = jetcal_lc6;
    jetsmear = jetsmear_lc6;
  }
  else assert("Jet not supported by JESCalibrationTool");

  const bool is_lc = jet->is_atlas_anti_kt_cone_4_h1_lc_topo() || jet->is_atlas_anti_kt_cone_6_h1_lc_topo();

  // seed per WZ cutflow
  // source: https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/WZCommonMETUtilityHowTo
  m_random->SetSeed( evnum*100 + 20110706*jetnum);

  TLorentzVector calib_jet;
  // calibrate jet, including origin correction - this is used for actual analysis-level jets
  if(calibrate) {
    if( false ) {
      // for debugging...
      std::cout << "AnaJet: " 
		<< jet->_em_scale_momentum.E() << " " 
		<< jet->_em_scale_momentum.Eta() << " " 
		<< jet->EtaOrigin() << " " 
		<< jet->PhiOrigin() << " " 
		<< jet->MOrigin() << " " 
		<< mu << " " 
		<< nGoodPV << endl;
    }
    if(false) {
      // more debugging...
      std::cout << "AnaJet const_scale: " << (jet->has_cs_scale()?"YES":"NO") << " " << jet->_cs_scale_momentum.E()  << " " << jet->_em_scale_momentum.E() << std::endl;
    }
    calib_jet = jetcal->ApplyOffsetEtaJES( is_lc && jet->has_cs_scale() ? jet->_cs_scale_momentum.E() : jet->_em_scale_momentum.E(),
					   jet->_em_scale_momentum.Eta() ,
					   jet->EtaOrigin() ,
					   jet->PhiOrigin() ,
					   jet->MOrigin() ,
	 				   mu, nGoodPV );
  }
  // just copy over 4vector from AnaJet - this is used for LC jets used in MET recomputation
  // This follow the simplified 2011 W/Z recommendation for MET uncertainty due to JES
  else {
    assert(smear); // if we're not calibrating, we must be smearing for MET recomputation
    calib_jet = jet->four_vector();
  }

  // MC oversmearing for systematic studies
  // details: https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/JetEnergyResolutionProviderForPhysicsAnalysis
  double jer_factor = 1.0;
  if( smear ) {
    double y = calib_jet.Rapidity();
    double pt = calib_jet.Pt();
    if(std::abs(y)>4.5) y = (y>0) ? 4.5 : -4.5; // this is to get rid of JER package, it does the same things outside the range                            
    double jerShift         = 1.0; // EWK meeting 3/2/2011                                                                                             

    // now factor for JER systematics
    double deltaJerShift = 1.0;
    if(true) {
      // see https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/JetEnergyResolutionProviderForPhysicsAnalysis
      double resolutionMC     = jetsmear->getRelResolutionMC(pt, y); // S
      double resolutionUncert = jetsmear->getResolutionUncert(pt, y); // U	
      
      // get the shift for JER uncertainty
      double SplusU2 = pow(resolutionMC + resolutionUncert,2);
      deltaJerShift = m_random->Gaus(0.0, TMath::Sqrt(SplusU2 - resolutionMC*resolutionMC));
      
      // get the shift for -1 sigma
      if(smear<0) {
	deltaJerShift*=-1.0;
      }
      
      // translate this into a multiplicative factor
      deltaJerShift = 1.0 + deltaJerShift;
    }    
    
    // add it together
    jer_factor = jerShift*deltaJerShift;
  }

  boost::shared_ptr<AnaJet> new_jet( boost::make_shared<AnaJet>(*jet) );
  new_jet->_momentum.SetPtEtaPhiE(calib_jet.Pt()*jer_factor,
				  calib_jet.Eta(), 
				  calib_jet.Phi(),
				  calib_jet.E()*jer_factor);
  //new_jet->print();
  //jet->print();

  // define parent
  new_jet->_uncorrected_parent = jet;
  
  return boost::const_pointer_cast<const AnaJet>( new_jet );
#else
  assert(0&&NO_ANALYSIS_MSG);
  return jet;
#endif
}
