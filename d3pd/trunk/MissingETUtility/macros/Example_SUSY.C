#define Example_cxx
#include "Example.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

#include "../MissingETUtility/METUtility.h"

#include "TLorentzVector.h"
#include "TMath.h"

#include "JetUncertainties/MultijetJESUncertaintyProvider.h"
#include "JetResolution/JERProvider.h"
#include "egammaAnalysisUtils/EnergyRescaler.h"
#include "MuonMomentumCorrections/SmearingClass.h"

void Example::Loop(int nfirst, int Nentries)
{

  //start of loop/ntuple stuff
  if (fChain == 0) return;

  if(Nentries<0) Nentries = fChain->GetEntries();
  
  bool verbose = true;
  bool isData = false;

  dataout = new TFile("METUtil_test_SUSY.root","RECREATE");

  h_refFinal_D3PD    = new TH1D("h_refFinal_D3PD","RefFinal from D3PD;E_{T}^{miss} [GeV]; Events / 5 GeV",50,0,250);
  h_refFinal_METUtil = new TH1D("h_refFinal_METUtil","RefFinal from METUtility;E_{T}^{miss} [GeV]; Events / 5 GeV",50,0,250);
  h_refFinal_Diff    = new TH1D("h_refFinal_Diff","RefFinal: Relative difference between D3PD and METUtility;E_{T}^{miss}; Events / 0.02",50,-0.5,0.5);
  h_refFinal_Delta   = new TH1D("h_refFinal_Delta","Uncertainty on RefFinal;E_{T}^{miss} [GeV]; Events / GeV",50,0,50);

  h_refFinal_SUSYmu  = new TH1D("h_refFinal_SUSYmu","RefFinal with recalculated muon term;E_{T}^{miss} [GeV]; Events / 5 GeV",50,0,250);
  h_refFinal_SUSYMU  = new TH1D("h_refFinal_SUSYMU","RefFinal with recalculated muon term in METUtility;E_{T}^{miss} [GeV]; Events / 5 GeV",50,0,250);

  h_refFinal_JERUp   = new TH1D("h_refFinal_JERUp","RefFinal with JER up;E_{T}^{miss} [GeV]; Events / 5 GeV",50,0,250);

  h_refFinal_JESUp   = new TH1D("h_refFinal_JESUp","RefFinal with JES up;E_{T}^{miss} [GeV]; Events / 5 GeV",50,0,250);
  h_refFinal_JESDown = new TH1D("h_refFinal_JESDown","RefFinal with JES down;E_{T}^{miss} [GeV]; Events / 5 GeV",50,0,250);

  h_refFinal_EESUp   = new TH1D("h_refFinal_EESUp","RefFinal with EES up;E_{T}^{miss} [GeV]; Events / 5 GeV",50,0,250);
  h_refFinal_EESDown = new TH1D("h_refFinal_EESDown","RefFinal with EES down;E_{T}^{miss} [GeV]; Events / 5 GeV",50,0,250);

  h_refFinal_MESUp   = new TH1D("h_refFinal_MESUp","RefFinal with MES up;E_{T}^{miss} [GeV]; Events / 5 GeV",50,0,250);
  h_refFinal_MESDown = new TH1D("h_refFinal_MESDown","RefFinal with MES down;E_{T}^{miss} [GeV]; Events / 5 GeV",50,0,250);

  h_refFinal_MERIDUp   = new TH1D("h_refFinal_MERIDUp","RefFinal with MERID up;E_{T}^{miss} [GeV]; Events / 5 GeV",50,0,250);
  h_refFinal_MERIDDown = new TH1D("h_refFinal_MERIDDown","RefFinal with MERID down;E_{T}^{miss} [GeV]; Events / 5 GeV",50,0,250);

  h_refFinal_MERMSUp   = new TH1D("h_refFinal_MERMSUp","RefFinal with MERMS up;E_{T}^{miss} [GeV]; Events / 5 GeV",50,0,250);
  h_refFinal_MERMSDown = new TH1D("h_refFinal_MERMSDown","RefFinal with MERMS down;E_{T}^{miss} [GeV]; Events / 5 GeV",50,0,250);

  h_refFinal_COUp    = new TH1D("h_refFinal_COUp","RefFinal with CellOut up;E_{T}^{miss} [GeV]; Events / 5 GeV",50,0,250);
  h_refFinal_CODown  = new TH1D("h_refFinal_CODown","RefFinal with CellOut down;E_{T}^{miss} [GeV]; Events / 5 GeV",50,0,250);
 
  MultijetJESUncertaintyProvider _JES("AntiKt4EMJESTopoJets",
				      "../../Jet/JetUncertainties/share/MJESUncertainty.root",
				      "../../Jet/JetUncertainties/share/MJESUncertainty.root",
				      "../../Jet/JetUncertainties/share/JESUncertainty.root");
  _JES.init();

  JERProvider _JER("AntiKt4TopoJES", "Truth", "../../Jet/JetResolution/share/JERProviderPlots.root");
  _JER.init();

  eg2011::EnergyRescaler _egammaTool;
  _egammaTool.useDefaultCalibConstants("2011");

  MuonSmear::SmearingClass _muonTool("Data11","staco","q_pT","Rel17");
  _muonTool.UseScale(1);

  METUtility *metCheck = new METUtility;
  metCheck->defineMissingET(true, false, true, true, false, false, true, true, false);
  // Turn on (off) the relevant MET terms
  // RefEle, (RefGamma), (RefTau), RefJet, (SoftJets), (RefMuon), MuonTotal, CellOut, (CellOut_Eflow)
  metCheck->setCellFix(false);
  metCheck->setIsMuid(false);
  metCheck->doForwardEtaCut(false);

  METUtility *metSys = new METUtility;
  metSys->defineMissingET(true, false, false, true, false, false, true, true, false);
  metSys->setCellFix(true);
  metSys->setIsMuid(false);
  metSys->doForwardEtaCut(false);
  metSys->setPileUpUncertainty(0.10);

  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=nfirst; jentry<Nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    // if (Cut(ientry) < 0) continue;


    double MET_Simplified20_RefEle_et = sqrt(MET_Simplified20_RefEle_etx*MET_Simplified20_RefEle_etx
					     + MET_Simplified20_RefEle_ety*MET_Simplified20_RefEle_ety);
    double MET_Simplified20_RefGamma_et = sqrt(MET_Simplified20_RefGamma_etx*MET_Simplified20_RefGamma_etx
					       + MET_Simplified20_RefGamma_ety*MET_Simplified20_RefGamma_ety);
    double MET_Simplified20_RefJet_et = sqrt(MET_Simplified20_RefJet_etx*MET_Simplified20_RefJet_etx
					     + MET_Simplified20_RefJet_ety*MET_Simplified20_RefJet_ety);
    double MET_Simplified20_Muon_Total_Staco_et = sqrt(MET_Simplified20_Muon_Total_Staco_etx*MET_Simplified20_Muon_Total_Staco_etx
						       + MET_Simplified20_Muon_Total_Staco_ety*MET_Simplified20_Muon_Total_Staco_ety);
    double MET_Simplified20_CellOut_et = sqrt(MET_Simplified20_CellOut_etx*MET_Simplified20_CellOut_etx
					      + MET_Simplified20_CellOut_ety*MET_Simplified20_CellOut_ety);
    double MET_Simplified20_RefFinal_et = sqrt(MET_Simplified20_RefFinal_etx*MET_Simplified20_RefFinal_etx
					       + MET_Simplified20_RefFinal_ety*MET_Simplified20_RefFinal_ety);


    if(verbose || jentry%1000==0) cout << "Entry " << jentry
				       << ", run " << RunNumber << ", event " << EventNumber << endl;
    

    //First, we get the jet energy scale uncertainties and the resolutions of the jets
    vector<float> jesUp;
    vector<float> jesDown;
    vector<float> jetResUp;
    vector<float> jetResDown;

    TRandom *jetRandom = new TRandom;
    jetRandom->SetSeed(EventNumber);

    for(unsigned int iJet = 0; iJet < jet_AntiKt4TopoNewEM_pt->size(); ++iJet){
      float jesShiftUp = 0.14;
      float jesShiftDown = 0.14;
      //this is just a default value for out of bound jets. Chosen from the value for eta > 2.1 to be conservative.
      float jerShift = 1.0;
      float jerShiftUp = 1.0;
      float jerShiftDown = 1.0;
      if(jet_AntiKt4TopoNewEM_pt->at(iJet) > 15000.0
	 && jet_AntiKt4TopoNewEM_pt->at(iJet) < 7000000.0
	 && fabs(jet_AntiKt4TopoNewEM_eta->at(iJet)) < 4.5){

	double pi = TMath::Pi();

	// delta R cut needed to apply close-by jets uncertainty
	float drmin = 9999;
	if( jet_AntiKt4TopoNewEM_pt->at(iJet)>20000) {
	  for ( unsigned int ii = 0; ii < jet_AntiKt4TopoNewEM_n; ii++ ) {
	    if(jet_AntiKt4TopoNewEM_emscale_pt->at(ii)>7000) {
	      if(iJet!=ii) {
		double deta = jet_AntiKt4TopoNewEM_eta->at(iJet) - jet_AntiKt4TopoNewEM_eta->at(ii);
		double dphi = fabs(fmod((jet_AntiKt4TopoNewEM_phi->at(iJet)
					 - jet_AntiKt4TopoNewEM_phi->at(ii))+3*pi,2*pi)-pi);
		double dr = sqrt(deta*deta+dphi*dphi);
		if(dr<drmin) drmin=dr;
	      }
	    }
	  }
	}

	jesShiftUp = _JES.getRelPosUncert(jet_AntiKt4TopoNewEM_pt->at(iJet),
					  jet_AntiKt4TopoNewEM_eta->at(iJet),drmin);
	jesShiftDown = -1*_JES.getRelNegUncert(jet_AntiKt4TopoNewEM_pt->at(iJet),
					       jet_AntiKt4TopoNewEM_eta->at(iJet),drmin);
      }
      jesUp.push_back(jesShiftUp);
      jesDown.push_back(jesShiftDown);


      // Allowable range is > 10 GeV, but anything below 20 enters SoftJets
      if(jet_AntiKt4TopoNewEM_pt->at(iJet) > 20000.0 && jet_AntiKt4TopoNewEM_pt->at(iJet) < 5000.0*1000.0){
	double pt = jet_AntiKt4TopoNewEM_pt->at(iJet);
	double eta = jet_AntiKt4TopoNewEM_eta->at(iJet);
	if(fabs(eta)>4.5) eta = eta>0 ? 4.49 : -4.49;

	double S = _JER.getRelResolutionMC(pt/1e3,eta);
	double U = _JER.getResolutionUncert(pt/1e3,eta);
	double smearingFactorSyst = sqrt(pow(S+U,2)-pow(S,2));

	jerShift = jetRandom->Gaus(0, smearingFactorSyst);
      }

      jetResUp.push_back(jerShift);
      jetResDown.push_back(-1*jerShift); // Usually not used, see below.

      //////////////////////////////////////////////////////////////////////
      // Note: The JERDown shift is essentially meaningless.
      // If one is smearing central values, then there is an alternate
      // definition, i.e. from r16:
      //
      // S = m_JERProvider.getRelResolutionData(pt/1e3,eta);
      // SMC = m_JERProvider.getRelResolutionMC(pt/1e3,eta);
      // U = m_JERProvider.getResolutionUncert(pt/1e3,eta);
      // smearingFactorMC = sqrt( S*S - SMC*SMC );
      // smearingFactorSystUp = sqrt( (S+U)*(S+U) - SMC*SMC );
      // smearingFactorSystDown = (S-U > SMC) ? sqrt( (S+U)*(S+U) - SMC*SMC ) : 0;
      // 
      // float jerShift = jetRandom->Gaus(1,smearingFactorMC);
      // float jerShiftUp = jetRandom->Gaus(1,smearingFactorSystUp)/jerShift;
      // float jerShiftDown = jetRandom->Gaus(1,smearingFactorSystDown)/jerShift;
      //
      // jet_smeared_pt = pt*jerShift;
      // jetResUp.push_back(jerShiftUp-1);
      // jetResDown.push_back(jerShiftDown-1);
      //
      // This means that we smear the MC jets to match the resolution in data
      // for central values, or the resolution +/- uncertainty.
      // The standard practice is only to use res + uncertainty.
      //
      //////////////////////////////////////////////////////////////////////

    }//end of jet loop

    if(verbose)  cout << endl;

    delete jetRandom;

    //now we get the same for electrons. We also smear the electrons -- the method should be similar for photons
    vector<float> eesUp;
    vector<float> eesDown;
    vector<float> eerUp;
    vector<float> eerDown;
    vector<float> *el_smeared_pt = new vector<float>;
      
    for (unsigned int iEl = 0; iEl < el_pt->size(); ++iEl){

      _egammaTool.SetRandomSeed(int(1.e+5*fabs(el_phi->at(iEl))));
      //set the seed however you like, I'm following the SUSYTools example

      float smear = _egammaTool.getSmearingCorrectionMeV(el_cl_eta->at(iEl), el_E->at(iEl), 0, true);
      float smearUp = _egammaTool.getSmearingCorrectionMeV(el_cl_eta->at(iEl), el_E->at(iEl), 2, true);
      float smearDown = _egammaTool.getSmearingCorrectionMeV(el_cl_eta->at(iEl), el_E->at(iEl), 1, true);

      el_smeared_pt->push_back(smear*el_pt->at(iEl));
      eerUp.push_back((smearUp - smear)/smear);
      eerDown.push_back((smearDown - smear)/smear);

      float correction = 1.;
      if(isData)
	correction = _egammaTool.applyEnergyCorrectionMeV(el_cl_eta->at(iEl),el_cl_phi->at(iEl),
							 el_E->at(iEl),el_cl_pt->at(iEl),0,"ELECTRON") / el_E->at(iEl);
      el_smeared_pt->at(iEl)*= correction;
      double energyUp = _egammaTool.applyEnergyCorrectionMeV(el_cl_eta->at(iEl),el_cl_phi->at(iEl),
							    el_E->at(iEl),el_cl_pt->at(iEl),2,"ELECTRON") / (correction*el_E->at(iEl)) - 1;
      double energyDown = _egammaTool.applyEnergyCorrectionMeV(el_cl_eta->at(iEl),el_cl_phi->at(iEl),
							      el_E->at(iEl),el_cl_pt->at(iEl),1,"ELECTRON") / (correction*el_E->at(iEl)) - 1;

      eesUp.push_back(static_cast<float>(energyUp)); 
      eesDown.push_back(static_cast<float>(energyDown));
    }//end of electron loop


    //and now the same for muons
    vector<float> *mu_staco_smeared_pt = new vector<float>;
    vector<float> *mu_staco_smeared_ms_pt = new vector<float>;
      
    vector<float> cb_meridUp;
    vector<float> cb_meridDown;
    vector<float> cb_mermsUp;
    vector<float> cb_mermsDown;
    vector<float> mermsUp;
    vector<float> mermsDown;

    vector<float> mesUp;
    vector<float> mesDown;
      
    for(unsigned int iMu = 0; iMu < mu_staco_pt->size(); ++iMu){

      double ptcb = mu_staco_pt->at(iMu);
      double ptid = (mu_staco_id_qoverp_exPV->at(iMu) != 0.) ? fabs(sin(mu_staco_id_theta_exPV->at(iMu))/mu_staco_id_qoverp_exPV->at(iMu)) : 0.;
      double ptms = (mu_staco_ms_qoverp->at(iMu) != 0.) ? fabs(sin(mu_staco_ms_theta->at(iMu))/mu_staco_ms_qoverp->at(iMu)) : 0.;
      _muonTool.SetSeed(int(1.e+5*fabs(mu_staco_phi->at(iMu))));
      double etaMu = mu_staco_eta->at(iMu);
      _muonTool.Event(ptms, ptid, ptcb, etaMu);


      Float_t smearedCombinedPt = _muonTool.pTCB();
      if(!mu_staco_isCombinedMuon->at(iMu)) smearedCombinedPt = _muonTool.pTMS() + _muonTool.pTID();

      Float_t smearedMSPt = _muonTool.pTMS();


      mu_staco_smeared_ms_pt->push_back(ptms);
      mu_staco_smeared_pt->push_back(smearedCombinedPt);

      double ptMS_smeared, ptID_smeared, ptCB_smeared;
      float smearedpTMS, smearedpTID, smearedpTCB;
      smearedpTMS = 0.1; smearedpTID = 0.1; smearedpTCB = 0.1;
      _muonTool.PTVar(ptMS_smeared, ptID_smeared, ptCB_smeared, "MSLOW");
      smearedpTMS = ptMS_smeared/smearedMSPt - 1.0;
      smearedpTCB = ptCB_smeared/smearedCombinedPt - 1.0;
      mermsDown.push_back(smearedpTMS);
      cb_mermsDown.push_back(smearedpTCB);
      _muonTool.PTVar(ptMS_smeared, ptID_smeared, ptCB_smeared, "MSUP");
      smearedpTMS = ptMS_smeared/smearedMSPt - 1.0;
      smearedpTCB = ptCB_smeared/smearedCombinedPt - 1.0;
      mermsUp.push_back(smearedpTMS);
      cb_mermsUp.push_back(smearedpTCB);
      _muonTool.PTVar(ptMS_smeared, ptID_smeared, ptCB_smeared, "IDUP");
      smearedpTCB = ptCB_smeared/smearedCombinedPt - 1.0;
      cb_meridUp.push_back(smearedpTCB);
      _muonTool.PTVar(ptMS_smeared, ptID_smeared, ptCB_smeared, "IDLOW");
      smearedpTCB = ptCB_smeared/smearedCombinedPt - 1.0;
      cb_meridDown.push_back(smearedpTCB);

      // This is usable (as of METUtility 00-02-04) with the MuonMomentumUtils trunk
      int detRegion = _muonTool.DetRegion();
      if(detRegion==-1) detRegion = 3;
      double scalesyst = _muonTool.getScaleSyst_CB().at(detRegion);    
      mesUp.push_back(scalesyst);
      mesDown.push_back(-scalesyst);


    }//end of muon loop

      
    if(verbose)   cout << "Checking Rebuilding from METComposition Map Works:" << endl;
    metCheck->reset();

    //this is an example of how to simply rebuild MET from available objects
    //       metCheck = new METUtility;
    metCheck->setObjects("jets", jet_AntiKt4TopoNewEM_pt, jet_AntiKt4TopoNewEM_eta, jet_AntiKt4TopoNewEM_phi, jet_AntiKt4TopoNewEM_E, jet_AntiKt4TopoNewEM_MET_Simplified20_wet, jet_AntiKt4TopoNewEM_MET_Simplified20_wpx, jet_AntiKt4TopoNewEM_MET_Simplified20_wpy, jet_AntiKt4TopoNewEM_MET_Simplified20_statusWord);

    metCheck->setElectronParameters(el_pt, el_eta, el_phi, el_MET_Simplified20_wet, el_MET_Simplified20_wpx, el_MET_Simplified20_wpy, el_MET_Simplified20_statusWord);

    metCheck->setMuonParameters(mu_staco_pt, mu_staco_eta, mu_staco_phi, mu_staco_MET_Simplified20_wet, mu_staco_MET_Simplified20_wpx, mu_staco_MET_Simplified20_wpy, mu_staco_MET_Simplified20_statusWord);
    metCheck->setExtraMuonParameters(mu_staco_ms_qoverp, mu_staco_ms_theta, mu_staco_ms_phi, mu_staco_charge);
    //    metCheck->setPhotonParameters(ph_pt, ph_eta, ph_phi, ph_MET_wet, ph_MET_wpx, ph_MET_wpy, ph_MET_statusWord);
    //    metCheck->setMETTerm("RefGamma", MET_Simplified20_RefGamma_etx, MET_Simplified20_RefGamma_ety, MET_Simplified20_RefGamma_sumet);
    metCheck->setMETTerm("CellOut", MET_Simplified20_CellOut_etx, MET_Simplified20_CellOut_ety, MET_Simplified20_CellOut_sumet);

//     metCheck->setTauParameters(tau_pt, tau_eta, tau_phi, tau_MET_Simplified20_wet, tau_MET_Simplified20_wpx, tau_MET_Simplified20_wpy, tau_MET_Simplified20_statusWord);

    float refEle_et = metCheck->getMissingET("RefEle").et();
    float refGamma_et = metCheck->getMissingET("RefGamma").et();
    float refJet_et = metCheck->getMissingET("RefJet").et();
    float refCellOut_et = metCheck->getMissingET("CellOut").et();
    float muonTotal_et = metCheck->getMissingET("MuonTotal").et();
    float refFinal_et = metCheck->getMissingET("RefFinal").et();

    float refEle_etx = metCheck->getMissingET("RefEle").etx();
    float refGamma_etx = metCheck->getMissingET("RefGamma").etx();
    float refJet_etx = metCheck->getMissingET("RefJet").etx();
    float refCellOut_etx = metCheck->getMissingET("CellOut").etx();
    float muonTotal_etx = metCheck->getMissingET("MuonTotal").etx();
    float refFinal_etx = metCheck->getMissingET("RefFinal").etx();
    
    if(verbose) {
      cout << "+++++++++++++++++++++++++++++" << endl;
      cout << "Term:    Original       Tool output" << endl;
      cout << "Ref Ele etx: " << MET_Simplified20_RefEle_etx << " vs " << refEle_etx << endl;
      cout << "Ref Gamma etx: " << MET_Simplified20_RefGamma_etx << " vs " << refGamma_etx << endl;
      cout << "Ref Jet etx: " << MET_Simplified20_RefJet_etx << " vs " << refJet_etx << endl;
      cout << "Muon_Total_Staco etx: " << MET_Simplified20_Muon_Total_Staco_etx << " vs " << muonTotal_etx << endl;
      cout << "CellOut etx: " << MET_Simplified20_CellOut_etx << " vs " << refCellOut_etx << endl;
      cout << "Ref Final etx: " << MET_Simplified20_RefFinal_etx << " vs " << refFinal_etx << endl;
    }
      
    METObject myMET;
    myMET = metCheck->getMissingET("RefFinal");
    if(verbose) {
      cout << "RefFinal components, etx = " << myMET.etx() << ", ety = " << myMET.ety() << ", sumet = " << myMET.sumet() << ", phi = " << myMET.phi() << ", et = " << myMET.et() << endl;
      cout << "Significance of RefFinal is " << myMET.sig() << endl;
      cout << "+++++++++++++++++++++++++++++" << endl;
    }
     
    if(verbose)  cout << "Demonstration of smearing and systematics" << endl;

    //this demonstration is for doing smearing and systematics

    metSys->reset();

    metSys->setObjects("jets", jet_AntiKt4TopoNewEM_pt, jet_AntiKt4TopoNewEM_eta, jet_AntiKt4TopoNewEM_phi, jet_AntiKt4TopoNewEM_E, jet_AntiKt4TopoNewEM_MET_Simplified20_wet, jet_AntiKt4TopoNewEM_MET_Simplified20_wpx, jet_AntiKt4TopoNewEM_MET_Simplified20_wpy, jet_AntiKt4TopoNewEM_MET_Simplified20_statusWord);
    metSys->setExtraJetParameters(jet_AntiKt4TopoNewEM_EMJES, jet_AntiKt4TopoNewEM_m, jet_AntiKt4TopoNewEM_eta, jet_AntiKt4TopoNewEM_phi);
    metSys->setElectronParameters(el_smeared_pt, el_eta, el_phi, el_MET_wet, el_MET_wpx, el_MET_wpy, el_MET_statusWord);
    metSys->setMuonParameters(mu_staco_smeared_pt, mu_staco_eta, mu_staco_phi, mu_staco_MET_wet, mu_staco_MET_wpx, mu_staco_MET_wpy, mu_staco_MET_statusWord);
    metSys->setExtraMuonParameters(mu_staco_smeared_ms_pt, mu_staco_ms_theta, mu_staco_ms_phi);
	
    //    metSys->setMETTerm("RefGamma", MET_Simplified20_RefGamma_etx, MET_Simplified20_RefGamma_ety, MET_Simplified20_RefGamma_sumet);
    metSys->setMETTerm("CellOut", MET_Simplified20_CellOut_etx, MET_Simplified20_CellOut_ety, MET_Simplified20_CellOut_sumet);

    metSys->setObjectEnergyUncertainties("jets", jesUp, jesDown);
    metSys->setObjectResolutionShift("jets", jetResUp, jetResDown);

    metSys->setObjectEnergyUncertainties("electrons", eesUp, eesDown);
    metSys->setObjectResolutionShift("electrons", eerUp, eerDown);

    metSys->setObjectResolutionShift("comboms", cb_mermsUp, cb_mermsDown);
    metSys->setObjectResolutionShift("comboid", cb_meridUp, cb_meridDown);
    metSys->setObjectResolutionShift("spectromuons", mermsUp, mermsDown);

    metSys->setObjectEnergyUncertainties("muons", mesUp, mesDown);
	
    h_refFinal_D3PD->Fill(MET_Simplified20_RefFinal_et/1e3);
    h_refFinal_METUtil->Fill(metSys->getMissingET("RefFinal").et()/1e3);
    h_refFinal_Diff->Fill(metSys->getMissingET("RefFinal").et()/MET_Simplified20_RefFinal_et-1);
    h_refFinal_Delta->Fill(metSys->absDeltaMissingET("RefFinal").et()/1e3);

    h_refFinal_JERUp->Fill(metSys->getMissingET("RefFinal","JERUp").et()/1e3);

    h_refFinal_JESUp->Fill(metSys->getMissingET("RefFinal","JESUp").et()/1e3);
    h_refFinal_JESDown->Fill(metSys->getMissingET("RefFinal","JESDown").et()/1e3);

    h_refFinal_EESUp->Fill(metSys->getMissingET("RefFinal","EESUp").et()/1e3);
    h_refFinal_EESDown->Fill(metSys->getMissingET("RefFinal","EESDown").et()/1e3);

    h_refFinal_MERIDUp->Fill(metSys->getMissingET("RefFinal","MERIDUp").et()/1e3);
    h_refFinal_MERIDDown->Fill(metSys->getMissingET("RefFinal","MERIDDown").et()/1e3);

    h_refFinal_MERMSUp->Fill(metSys->getMissingET("RefFinal","MERMSUp").et()/1e3);
    h_refFinal_MERMSDown->Fill(metSys->getMissingET("RefFinal","MERMSDown").et()/1e3);

    h_refFinal_MESUp->Fill(metSys->getMissingET("RefFinal","MESUp").et()/1e3);
    h_refFinal_MESDown->Fill(metSys->getMissingET("RefFinal","MESDown").et()/1e3);

    h_refFinal_COUp->Fill(metSys->getMissingET("RefFinal","AllClustersUp").et()/1e3);
    h_refFinal_CODown->Fill(metSys->getMissingET("RefFinal","AllClustersDown").et()/1e3);

    if(verbose) {
      cout << "+++++++++++++++++++++++++++++" << endl;
      cout << "All these are the scalar MET term" << endl;
      cout << "RefEle (smeared): " << metSys->getMissingET("RefEle").et() << endl;
      cout << "RefGamma: " << metSys->getMissingET("RefGamma").et() << endl;
      cout << "RefTau: " << metSys->getMissingET("RefTau").et() << endl;
      cout << "RefJet: " << metSys->getMissingET("RefJet").et() << endl;
      cout << "SoftJets: " << metSys->getMissingET("SoftJets").et() << endl;
      cout << "RefMuon: " << metSys->getMissingET("RefMuon").et() << endl;
      cout << "MuonBoy (smeared and scaled): " << metSys->getMissingET("MuonTotal").et() << endl;
      cout << "CellOut_eflow: " << metSys->getMissingET("CellOutEflow").et() << endl;
      cout << "RefFinal: " << metSys->getMissingET("RefFinal").et() << endl;
      cout << "+++++++++++++++++++++++++++++" << endl;
      cout << endl;
      cout << "+++++++++++++++++++++++++++++" << endl;
      cout << "RefJet JESUp: " << metSys->getMissingET("RefJet", "JESUp").et() << ",  JESDown: " << metSys->getMissingET("RefJet", "JESDown").et() << endl;
      cout << "RefJet JES Diff (up - down)/none : " << metSys->getMissingET("RefJet", "JESDiff").et() << endl;
      cout << "RefFinal JESUp: " << metSys->getMissingET("RefFinal", "JESUp").et() << ", JESDown: " << metSys->getMissingET("RefFinal", "JESDown").et() << endl;
      cout << "RefFinal JES Diff (up - down)/none : " << metSys->getMissingET("RefFinal", "JESDiff").et() << endl;
      cout << "RefJet JERUp: " << metSys->getMissingET("RefJet", "JERUp").et() << ", JERDown: " << metSys->getMissingET("RefJet", "JERDown").et() << endl;
      cout << "RefJet JER Diff (up - down)/none : " << metSys->getMissingET("RefJet", "JERDiff").et() << endl;
      cout << "RefFinal JERUp: " << metSys->getMissingET("RefFinal", "JERUp").et() << ", JERDown: " << metSys->getMissingET("RefFinal", "JERDown").et() << endl;
      cout << "RefFinal JER Diff (up - down)/none : " << metSys->getMissingET("RefFinal", "JERDiff").et() << endl;
      cout << "+++++++++++++++++++++++++++++" << endl;
      // Use SoftJetsUp and SoftJetsDown
      cout << "SoftJets Up: " << metSys->getMissingET("SoftJets", "SoftJetsUp").et() << ", Down: " << metSys->getMissingET("SoftJets", "SoftJetsDown").et() << endl;
      cout << "SoftJets Diff (up - down)/none: " << metSys->getMissingET("SoftJets", "SoftJetsDiff").et() << endl;
      cout << "RefFinal SoftJets up: " << metSys->getMissingET("RefFinal", "SoftJetsUp").et() << ", SoftJets down: " << metSys->getMissingET("RefFinal", "SoftJetsDown").et()  << endl;
      cout << "RefFinal SoftJets Diff (up - down)/none: " << metSys->getMissingET("RefFinal", "SoftJetsDiff").et() << endl;
      cout << "+++++++++++++++++++++++++++++" << endl;
      cout << "RefEle EESUp: " << metSys->getMissingET("RefEle", "EESUp").et() << ",  EESDown: " << metSys->getMissingET("RefEle", "EESDown").et() << endl;
      cout << "RefFinal EESUp: " << metSys->getMissingET("RefFinal", "EESUp").et() << ",  EESDown: " << metSys->getMissingET("RefFinal", "EESDown").et() << endl;
      cout << "RefFinal EES Diff (up - down)/none : " << metSys->getMissingET("RefFinal", "EESDiff").et() << endl;
      cout << "RefEle EERUp: " << metSys->getMissingET("RefEle", "EERUp").et() << ",  EERDown: " << metSys->getMissingET("RefEle", "EERDown").et() << endl;
      cout << "RefFinal EERUp: " << metSys->getMissingET("RefFinal", "EERUp").et() << ",  EERDown: " << metSys->getMissingET("RefFinal", "EERDown").et() << endl;
      cout << "RefFinal EER Diff (up - down)/none : " << metSys->getMissingET("RefFinal", "EERDiff").et() << endl;
      cout << "+++++++++++++++++++++++++++++" << endl;
      cout << "MuonBoy MESUp: " << metSys->getMissingET("MuonTotal", "MESUp").et() << ",  MESDown: " << metSys->getMissingET("MuonTotal", "MESDown").et() << endl;
      cout << "RefFinal MESUp: " << metSys->getMissingET("RefFinal", "MESUp").et() << ",  MESDown: " << metSys->getMissingET("RefFinal", "MESDown").et() << endl;
      cout << "RefFinal MES Diff (up - down)/none : " << metSys->getMissingET("RefFinal", "MESDiff").et() << endl;
      cout << "MuonBoy MERIDUp: " << metSys->getMissingET("MuonTotal", "MERIDUp").et() << ",  MERIDDown: " << metSys->getMissingET("MuonTotal", "MERIDDown").et() << endl;
      cout << "RefFinal MERIDUp: " << metSys->getMissingET("RefFinal", "MERIDUp").et() << ",  MERIDDown: " << metSys->getMissingET("RefFinal", "MERIDDown").et() << endl;
      cout << "RefFinal MERID Diff (up - down)/none : " << metSys->getMissingET("RefFinal", "MERIDDiff").et() << endl;
      cout << "MuonBoy MERMSUp: " << metSys->getMissingET("MuonTotal", "MERMSUp").et() << ",  MERMSDown: " << metSys->getMissingET("MuonTotal", "MERMSDown").et() << endl;
      cout << "RefFinal MERMSUp: " << metSys->getMissingET("RefFinal", "MERMSUp").et() << ",  MERMSDown: " << metSys->getMissingET("RefFinal", "MERMSDown").et() << endl;
      cout << "RefFinal MERMS Diff (up - down)/none : " << metSys->getMissingET("RefFinal", "MERMSDiff").et() << endl;
      cout << "+++++++++++++++++++++++++++++" << endl;
      cout << "RefTau TESUp: " << metSys->getMissingET("RefTau", "TESUp").et() << ",  TESDown: " << metSys->getMissingET("RefTau", "TESDown").et() << endl;
      cout << "RefFinal TESUp: " << metSys->getMissingET("RefFinal", "TESUp").et() << ",  TESDown: " << metSys->getMissingET("RefFinal", "TESDown").et() << endl;
      cout << "RefFinal TES Diff (up - down)/none : " << metSys->getMissingET("RefFinal", "TESDiff").et() << endl;
      cout << "RefTau TERUp: " << metSys->getMissingET("RefTau", "TERUp").et() << ",  TERDown: " << metSys->getMissingET("RefTau", "TERDown").et() << endl;
      cout << "RefFinal TERUp: " << metSys->getMissingET("RefFinal", "TERUp").et() << ",  TERDown: " << metSys->getMissingET("RefFinal", "TERDown").et() << endl;
      cout << "RefFinal TER Diff (up - down)/none : " << metSys->getMissingET("RefFinal", "TERDiff").et() << endl;
      cout << "+++++++++++++++++++++++++++++" << endl;
      ///the cellout systematic is based on sumet of cellout, so the objects aren't needed. the numbers are from the May 2011 MET PLHC paper and include the affects predicted by different MC (Professor vs Perugia, etc) and cluster energy scale uncertainty
      cout << "CellOut_Eflow Up: " << metSys->getMissingET("CellOutEflow", "CellOutEflowUp").et() << " CellOutEflow Down: " << metSys->getMissingET("CellOutEflow", "CellOutEflowDown").et() << endl;
      cout << "RefFinal, CellOut_Eflow up: " << metSys->getMissingET("RefFinal", "CellOutEflowUp").et() << " CellOut_Eflow down: " << metSys->getMissingET("RefFinal", "CellOutEflowDown").et() << endl;
      cout << "+++++++++++++++++++++++++++++" << endl;
      ///the following systematic argument gets all of the PLHC systematics at once, for SoftsJets and CellOut_Eflow. Note it does not include RefTau
      cout << "RefFinal PLHC Up: " << metSys->getMissingET("RefFinal", "AllClustersUp").et() << ", RefFinal PLHC down: " << metSys->getMissingET("RefFinal", "AllClustersDown").et() << endl;

      cout << "+++++++++++++++++++++++++++++" << endl;
      cout << "Pile Up, RefFinal Up: " << metSys->getMissingET("RefFinal", "PileUpUp").et() << ", PileUpDown: " << metSys->getMissingET("RefFinal", "PileUpDown").et() << endl;
      cout << "+++++++++++++++++++++++++++++" << endl;
      cout << "+++++++++++++++++++++++++++++" << endl;
      cout << "Combined errors, giving an uncertainty on MET" << endl;
      METObject smearedMET = metSys->getMissingET("RefFinal");
      cout << "RefFinal MET = " << smearedMET.et() << " +- " << metSys->absDeltaMissingET("RefFinal").et() << " (" << 100*metSys->deltaMissingET("RefFinal").et() << "%)" << endl;
      cout << "+++++++++++++++++++++++++++++" << endl;
      cout << endl;
    }

    // Do computation following the SUSY prescription of recomputing the muon MET term

    metCheck->reset();

    double MET_Simp20_etx = MET_Simplified20_RefFinal_etx - MET_Simplified20_Muon_Total_Staco_etx;
    double MET_Simp20_ety = MET_Simplified20_RefFinal_ety - MET_Simplified20_Muon_Total_Staco_ety;

    double MET_susyMu_etx = 0;
    double MET_susyMu_ety = 0;

    vector<float> new_pt;
    vector<vector<unsigned int> > new_statusWord;
    vector<vector<float> > new_wet, new_wpx, new_wpy;
    for(int imu=0; imu<mu_staco_n; imu++) {
      TLorentzVector mu_TLV;
      double pt, phi, eta, m;
      m = 105.66;
      eta = mu_staco_eta->at(imu);
      phi = mu_staco_phi->at(imu);

      if(mu_staco_isCombinedMuon->at(imu)) {
	pt  = (mu_staco_id_qoverp_exPV->at(imu) != 0) ? 
	  fabs(sin(mu_staco_id_theta_exPV->at(imu))/mu_staco_id_qoverp_exPV->at(imu)) : 0.;
      } else {
	pt = mu_staco_pt->at(imu);
      }
      mu_TLV.SetPtEtaPhiM(pt,eta,phi,m);

      bool accept = mu_TLV.Pt()>10e3 && fabs(mu_TLV.Eta())<2.4;

      // Muon quality cuts
      bool quality = ( (mu_staco_isCombinedMuon->at(imu) || 
			mu_staco_isLowPtReconstructedMuon->at(imu)) &&
		       mu_staco_loose->at(imu) );

      // Track quality requirements
      int nTRTOutliers = mu_staco_nTRTOutliers->at(imu);
      int nTRTTotal    = nTRTOutliers + mu_staco_nTRTHits->at(imu);
      float trackEta   = -log(tan(mu_staco_id_theta->at(imu)/2));
      bool trackisgood = 
	( !mu_staco_expectBLayerHit->at(imu) || mu_staco_nBLHits->at(imu) > 0 ) &&
	( mu_staco_nPixHits->at(imu) + mu_staco_nPixelDeadSensors->at(imu) > 1 ) &&
	( mu_staco_nSCTHits->at(imu) + mu_staco_nSCTDeadSensors->at(imu) >= 6 ) &&
	( mu_staco_nPixHoles->at(imu) + mu_staco_nSCTHoles->at(imu) < 3 ) &&
	( fabs(trackEta) > 1.9 || nTRTTotal > 5 ) &&
	( nTRTTotal <= 5 || nTRTOutliers < 0.9*nTRTTotal );

      bool select = accept && quality && trackisgood;

      new_pt.push_back(mu_TLV.Pt());

      if(select) {
	MET_susyMu_etx -= mu_TLV.Px();
	MET_susyMu_ety -= mu_TLV.Py();
	vector<float> univec;
	univec.push_back(1.);
	new_wet.push_back(univec);
	new_wpx.push_back(univec);
	new_wpy.push_back(univec);
      } else {
	vector<float> zerovec;
	zerovec.push_back(0.);
	new_wet.push_back(zerovec);
	new_wpx.push_back(zerovec);
	new_wpy.push_back(zerovec);
      }
      vector<unsigned int> defvec;
      defvec.push_back(MissingETTags::DEFAULT);
      new_statusWord.push_back(defvec);
    }

    double MET_Simp20_et = sqrt(MET_Simp20_etx*MET_Simp20_etx + MET_Simp20_ety*MET_Simp20_ety);

    metCheck->setObjects("jets", jet_AntiKt4TopoNewEM_pt, jet_AntiKt4TopoNewEM_eta, jet_AntiKt4TopoNewEM_phi, jet_AntiKt4TopoNewEM_E, 
			 jet_AntiKt4TopoNewEM_MET_Simplified20_wet, jet_AntiKt4TopoNewEM_MET_Simplified20_wpx,
			 jet_AntiKt4TopoNewEM_MET_Simplified20_wpy, jet_AntiKt4TopoNewEM_MET_Simplified20_statusWord);

    metCheck->setElectronParameters(el_pt, el_eta, el_phi, el_MET_wet, el_MET_wpx, el_MET_wpy, el_MET_statusWord);
    //    metCheck->setPhotonParameters(ph_pt, ph_eta, ph_phi, ph_MET_wet, ph_MET_wpx, ph_MET_wpy, ph_MET_statusWord);
    metCheck->setMETTerm("RefGamma", MET_Simplified20_RefGamma_etx, MET_Simplified20_RefGamma_ety, MET_Simplified20_RefGamma_sumet);

    metCheck->setMETTerm("CellOut", MET_Simplified20_CellOut_etx, MET_Simplified20_CellOut_ety, MET_Simplified20_CellOut_sumet);

    if(verbose && (abs(MET_susyMu_etx)>0 || abs(MET_susyMu_ety)>0)) {
      METObject noMu_susy = metCheck->getMissingET("RefFinal");
      cout << "Muonless MET computed by hand: etx = " << MET_Simp20_etx << ", ety = " << MET_Simp20_ety << endl;
      cout << "Muonless MET computed by METUtil: etx = " << noMu_susy.etx() << ", ety = " << noMu_susy.ety() << endl;
    }

    MET_Simp20_etx += MET_susyMu_etx;
    MET_Simp20_ety += MET_susyMu_ety;

    metCheck->setMuonParameters(&new_pt, mu_staco_eta, mu_staco_phi, &new_wet, &new_wpx, &new_wpy, &new_statusWord);
    metCheck->setExtraMuonParameters(mu_staco_ms_qoverp, mu_staco_ms_theta, mu_staco_ms_phi, mu_staco_charge);

    if(verbose && (abs(MET_susyMu_etx)>0 || abs(MET_susyMu_ety)>0)) {
      METObject muonTotal_susy = metCheck->getMissingET("MuonTotal");
      cout << "Muon MET computed by hand: etx = " << MET_susyMu_etx << ", ety = " << MET_susyMu_ety << endl;
      cout << "Muon MET computed by METUtil: etx = " << muonTotal_susy.etx() << ", ety = " << muonTotal_susy.ety() << endl;
    }

    h_refFinal_SUSYmu->Fill(MET_Simp20_et/1e3);
    h_refFinal_SUSYMU->Fill(metCheck->getMissingET("RefFinal").et()/1e3);

    delete el_smeared_pt;
    delete mu_staco_smeared_pt;
    delete mu_staco_smeared_ms_pt;

  }//end of loop

  dataout->Print();
  dataout->Write();
  dataout->Close();

  delete metSys;
  delete metCheck;
   
}//end of macro

