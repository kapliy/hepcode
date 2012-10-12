#define Example_cxx
#include "Example.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

#include "../MissingETUtility/METUtility.h"

#include "JetUncertainties/JetUncertainties/MultijetJESUncertaintyProvider.h"
#include "JetResolution/JERProvider/JERProvider.h"
#include "EnergyRescalerTool/EnergyRescaler.h"
#include "MuonMomentumCorrections/root/SmearingClass.h"

#include "../MissingETUtility/TESUncertaintyProvider.h"


void Example::Loop()
{

  bool verbose = true;
  bool isData = true;

  MultijetJESUncertaintyProvider _JES("AntiKt4EMJESTopoJets",
				      "JetUncertainties/share/MJESUncertainty.root",
				      "JetUncertainties/share/MJESUncertainty.root",
				      "JetUncertainties/share/JESUncertainty.root");
  _JES.init();

  JERProvider _JER("AntiKt4LCTopoJets", "Truth", "JetResolution/share/JERProviderPlots.root");
  _JER.init();

  EnergyRescaler _egammaTool;
  _egammaTool.useDefaultCalibConstants("2011");

  MuonSmear::SmearingClass _muonTool("Data11","staco","q_pT","Rel17");
  _muonTool.UseScale(1);

  TESUncertaintyProvider _TES;

  // Default settings are in the code, and replicate MET_RefFinal with no modifications.
  METUtility *metCheck = new METUtility;
  METUtility *metSys = new METUtility;

  //start of loop/ntuple stuff
  if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntriesFast();

  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    // if (Cut(ientry) < 0) continue;



    // if(jentry%1000==0) 
    if(verbose) cout << "Event " << jentry << endl;
    
    ///note, these are just examples of how to get jet, electron, and muon uncertainties and resolutions. The MET tool won't do it for you, because how you calibrate the tools supplied by the performance group is analysis specific. So you have to get them and feed them to METUtlity, and it will make sure they are used in the correct manner for MET
      

    //you, the user, should get updated tools for yourself.
    //and it is up to you to decide how to use them, these are only examples


    //First, we get the jet energy scale uncertainties and the resolutions of the jets
    vector<float> jesUp;
    vector<float> jesDown;
    vector<float> jetResUp;
    vector<float> jetResDown;

    TRandom *jetRandom = new TRandom;
    jetRandom->SetSeed(EventNumber);
    for(unsigned int iJet = 0; iJet < jet_AntiKt4LCTopo_pt->size(); ++iJet){
      float jesShiftUp = 0.14;
      float jesShiftDown = 0.14;
      float jerShift = 0.0;

      if(jet_AntiKt4LCTopo_pt->at(iJet) > 15000.0
	 && jet_AntiKt4LCTopo_pt->at(iJet) < 7000000.0
	 && fabs(jet_AntiKt4LCTopo_eta->at(iJet)) < 4.5){
	// delta R cut needed to apply close-by jets uncertainty
	float drmin=9999;
	if( jet_AntiKt4LCTopo_pt->at(iJet)>20000) {
	  for ( unsigned int ii = 0; ii < jet_AntiKt4LCTopo_n->size(); ii++ ) {
	    if(jet_AntiKt4LCTopo_emscale_pt->at(ii)>7000) {
	      if(i!=ii) {
		double dr = deltaR(jet_AntiKt4LCTopo_eta->at(iJet), 
				   jet_AntiKt4LCTopo_phi->at(iJet), 
				   jet_AntiKt4LCTopo_eta->at(ii),
				   jet_AntiKt4LCTopo_phi->at(ii));
		if(dr<drmin) drmin=dr;
	      }
	    }
	  }
	}

	jesShiftUp = _JES.getRelPosUncert(jet_AntiKt4LCTopo_pt->at(iJet),
					  jet_AntiKt4LCTopo_eta->at(iJet),drmin);
	jesShiftDown = -1*_JES.getRelNegUncert(jet_AntiKt4LCTopo_pt->at(iJet),
					       jet_AntiKt4LCTopo_eta->at(iJet),drmin);
      }
      jesUp.push_back(jesShiftUp);
      jesDown.push_back(jesShiftDown);

      // Allowable range is 10 GeV plus, but anything below 20 enters SoftJets
      if(jet_AntiKt4LCTopo_pt->at(iJet) > 20000.0 && jet_AntiKt4LCTopo_pt->at(iJet) < 5000.0*1000.0){
	double pt = jet_AntiKt4LCTopo_pt->at(iJet);
	double eta = jet_AntiKt4LCTopo_eta->at(iJet);
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
    
    //now we get the same for electrons.
    vector<float> eesUp;
    vector<float> eesDown;
    vector<float> eerUp;
    vector<float> eerDown;
    vector<float> *el_smeared_pt = new vector<float>;
      
    for (unsigned int iEl = 0; iEl < el_pt->size(); ++iEl){

      _egammaTool.SetRandomSeed(int(1.e+5*fabs(el_phi))); //set the seed however you like, I'm following the SUSYTools example

      float smear = _egammaTool.getSmearingCorrection(el_cl_eta->at(iEl), el_E->at(iEl)/1000.0, 0, true);
      float smearUp = _egammaTool.getSmearingCorrection(el_cl_eta->at(iEl), el_E->at(iEl)/1000.0, 2, true);
      float smearDown = _egammaTool.getSmearingCorrection(el_cl_eta->at(iEl), el_E->at(iEl)/1000.0, 1, true);

      el_smeared_pt->push_back(smear*el_pt->at(iEl));
      eerUp.push_back((smearUp - smear)/smear);
      eerDown.push_back((smearDown - smear)/smear);

      // Energy correction for data only, but following two calls get systematics on data or MC
      float correction = 1.;
      if(isData)
	correction = m_eRescale.applyEnergyCorrectionMeV(el_cl_eta->at(iEl),el_cl_phi->at(iEl),
							 el_E->at(iEl),el_cl_pt->at(iEl),0,"ELECTRON") / el_E->at(iEl);
      el_smeared_pt->at(i)*= correction;
      double energyUp = m_eRescale.applyEnergyCorrectionMeV(el_cl_eta->at(iEl),el_cl_phi->at(iEl),
							    el_E->at(iEl),el_cl_pt->at(iEl),2,"ELECTRON") / (correction*el_E->at(iEl)) - 1;
      double energyDown = m_eRescale.applyEnergyCorrectionMeV(el_cl_eta->at(iEl),el_cl_phi->at(iEl),
							      el_E->at(iEl),el_cl_pt->at(iEl),1,"ELECTRON") / (correction*el_E->at(iEl)) - 1;

      eesUp.push_back(energyUp);
      eesDown.push_back(energyDown);
    }//end of electron loop


    //now we get the same for photons
    vector<float> pesUp;
    vector<float> pesDown;
    vector<float> perUp;
    vector<float> perDown;
    vector<float> *ph_smeared_pt = new vector<float>;

    for (unsigned int iPh = 0; iPh < ph_pt->size(); ++iPh){

      _egammaTool.SetRandomSeed(int(1.e+5*fabs(ph_phi))); //set the seed however you like, I'm following the SUSYTools example

      float smear = _egammaTool.getSmearingCorrection(ph_cl_eta->at(iPh), ph_E->at(iPh)/1000.0, 0, true);
      float smearUp = _egammaTool.getSmearingCorrection(ph_cl_eta->at(iPh), ph_E->at(iPh)/1000.0, 2, true);
      float smearDown = _egammaTool.getSmearingCorrection(ph_cl_eta->at(iPh), ph_E->at(iPh)/1000.0, 1, true);

      ph_smeared_pt->push_back(smear*ph_pt->at(iPh));
      perUp.push_back((smearUp - smear)/smear);
      perDown.push_back((smearDown - smear)/smear);

      float correction = 1.;
      string photontype = ph_isConv->at(iPh) ? "CONVERTED_PHOTON" : "UNCONVERTED_PHOTON";
      if(isData)
	correction = m_eRescale.applyEnergyCorrectionMeV(ph_cl_eta,ph_cl_phi,ph_E,ph_cl_pt,0,photontype) / ph_E;
      ph_smeared_pt->at(i)*= correction;
      correction = m_eRescale.applyEnergyCorrectionMeV(ph_cl_eta->at(iPh),ph_cl_phi->at(iPh),
						       ph_E->at(iPh),ph_cl_pt->at(iPh),2,photontype) / (correction*ph_E->at(iPh)) - 1;
      correction = m_eRescale.applyEnergyCorrectionMeV(ph_cl_eta->at(iPh),ph_cl_phi->at(iPh),
						       ph_E->at(iPh),ph_cl_pt->at(iPh),1,photontype) / (correction*ph_E->at(iPh)) - 1;

      pesUp.push_back(static_cast<float>(energyUp)); 
      pesDown.push_back(static_cast<float>(energyDown));
    }//end of photon loop


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
      double combinedPt = ptcb;
      double ptid = (mu_staco_id_qoverp_exPV->at(iMu) != 0.) ? fabs(sin(mu_staco_id_theta_exPV->at(iMu))/mu_staco_id_qoverp_exPV->at(iMu)) : 0.;
      double ptms = (mu_staco_ms_qoverp_exPV->at(iMu) != 0.) ? fabs(sin(mu_staco_ms_theta_exPV->at(iMu))/mu_staco_ms_qoverp_exPV->at(iMu)) : 0.;
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
	
      int detRegion = m_muonTool->DetRegion();
      if(detRegion==-1) detRegion = 3;
      double scalesyst = _muonTool->getScaleSyst_CB().at(detRegion);    
      mesUp.push_back(scalesyst);
      mesDown.push_back(-scalesyst);

    }//end of muon loop

    vector<float> tesUp;
    vector<float> tesDown;

    // and for taus -- preliminary, TESUncertaintyProvider is still being tested.
    for(int iTau=0; iTau<tau_n; iTau++) {
      double pt = tau_pt->at(iTau);
      double eta = tau_eta->at(iTau);
      int nProng = tau_nProng->at(iTau);
      double uncert = _TES.GetTESUncertainty(pt/1e3, eta, nProng);
      //	if(verbose) cout << "RefTau TES uncert = " << uncert << endl;

      if(uncert < 0) uncert = 0;
      tesUp.push_back(uncert);
      tesDown.push_back(-1*uncert);
    }
  
    if(verbose)   cout << "Checking Rebuilding from METComposition Map Works:" << endl;

    //this is an example of how to simply rebuild MET from available objects
    metCheck->reset();
    metCheck->setObjects("jets", jet_AntiKt4LCTopo_pt, jet_AntiKt4LCTopo_eta, jet_AntiKt4LCTopo_phi, jet_AntiKt4LCTopo_E, jet_AntiKt4LCTopo_MET_wet, jet_AntiKt4LCTopo_MET_wpx, jet_AntiKt4LCTopo_MET_wpy, jet_AntiKt4LCTopo_MET_statusWord); //there is also a setJetParameters function like electrons below. Or this function can be used with electrons as setObjects("electrons", ...)
     
      
    metCheck->setElectronParameters(el_pt, el_eta, el_phi, el_MET_wet, el_MET_wpx, el_MET_wpy, el_MET_statusWord);
    metCheck->setPhotonParameters(ph_pt, ph_eta, ph_phi, ph_MET_wet, ph_MET_wpx, ph_MET_wpy, ph_MET_statusWord);
    metCheck->setTauParameters(tau_pt, tau_eta, tau_phi, tau_MET_wet, tau_MET_wpx, tau_MET_wpy, tau_MET_statusWord);
      

    metCheck->setMuonParameters(mu_staco_pt, mu_staco_eta, mu_staco_phi, mu_staco_MET_wet, mu_staco_MET_wpx, mu_staco_MET_wpy, mu_staco_MET_statusWord);
    metCheck->setExtraMuonParameters(mu_staco_ms_qoverp, mu_staco_ms_theta, mu_staco_ms_phi, mu_staco_charge);
    //muons require more info for spectro. There is also a setObjectMomenta(type, pt, eta, phi, E) that will fill these using the type argument as "spectromuons"
    //this accepts the qoverp variables to simplify things for the users. There is an overloaded function that takes pt instead (and leaves off charge),
    //to simplify things when smearing. See below in smearing and scaling example
    //note that the extra parameters are MS, not ME. While the MCP recommendation uses the ME variables as an example,
    //the uncertainties are derived on MS variables, which are used in the MET calculation.

 

    //these objects are either not in the D3PD, or it isn't trustworthy to rebuild the term. Any term can be put in and used in place of being rebuilt, but only the SoftJets and CellOut_Eflow systematics can be returned in that form
    //metCheck->setMETTerm("RefTau", MET_RefTau_etx, MET_RefTau_ety, MET_RefTau_sumet);  
    //metCheck->setMETTerm("RefGamma", MET_RefGamma_etx, MET_RefGamma_ety, MET_RefGamma_sumet);
    metCheck->setMETTerm("RefMuon", MET_RefMuon_etx, MET_RefMuon_ety, MET_RefMuon_sumet);
    //I didn't feel it was a good idea to rebuilt RefMuon from smeared and scaled jets, since it is a calorimeter term and the smearings and scaling from MCP are spectrometer and tracker based.
    metCheck->setMETTerm("CellOut", MET_CellOut_etx, MET_CellOut_ety, MET_CellOut_sumet);
    metCheck->setMETTerm("CellOutEflow", MET_CellOut_Eflow_etx, MET_CellOut_Eflow_ety, MET_CellOut_Eflow_sumet);
    // Only one of these should be set - whichever is activated in the flags. This is CellOut_Eflow for MET_RefFinal.

    float refEle_et = metCheck->getMissingET("RefEle").et();
    float refGamma_et = metCheck->getMissingET("RefGamma").et();
    float refTau_et = metCheck->getMissingET("RefTau").et();
    float refMuon_et = metCheck->getMissingET("RefMuon").et();
    float refJet_et = metCheck->getMissingET("RefJet").et();
    float refSoftJet_et = metCheck->getMissingET("SoftJets").et();
    float refCellOut_et = metCheck->getMissingET("CellOut").et();
    float refCellOutEflow_et = metCheck->getMissingET("CellOutEflow").et();
    float muonTotal_et = metCheck->getMissingET("MuonTotal").et();
    float refFinal_et = metCheck->getMissingET("RefFinal").et();
    
     
    if(verbose)  {
      cout << "+++++++++++++++++++++++++++++" << endl;
      cout << "Term:    Original       Tool output" << endl;
      cout << "Ref Ele et: " << MET_RefEle_et << " vs " << refEle_et << endl;
      cout << "Ref Gamma et: " << MET_RefGamma_et << " vs " << refGamma_et << endl;
      cout << "Ref Tau et: " << MET_RefTau_et << " vs " << refTau_et << endl;
      cout << "Ref Muon et: " << MET_RefMuon_et << " vs " << refMuon_et << endl;
      cout << "Ref Jet et: " << MET_RefJet_et << " vs " << refJet_et << endl;
      cout << "SoftJet et: " << MET_SoftJets_et << " vs " << refSoftJet_et << endl;
      cout << "MuonBoy et: " << MET_MuonBoy_et << " vs " << muonTotal_et << endl;
      cout << "CellOut_eflow et: " << MET_CellOut_Eflow_et << " vs " << refCellOutEflow_et << endl;
      cout << "Ref Final et: " << MET_RefFinal_et << " vs " << refFinal_et << endl;
    }
      
    ////Note, everytime you call MissingET(term), that term or RefFinal gets reconstructed. I haven't added any logic against that or to make it intelligent, but you can simplify things by not doing it for different elements of MET, I.E., etx, ety, et, sumet, or phi 

    //Part of METUtility is an METObject that holds MET. 
    //Declare is like so:
    METObject myMET;
    //set it to the output of METUtility:
    myMET = metCheck->getMissingET("RefFinal");
    if(verbose)   cout << "RefFinal components, etx = " << myMET.etx() << ", ety = " << myMET.ety() << ", sumet = " << myMET.sumet() << ", phi = " << myMET.phi() << ", et = " << myMET.et() << endl;
    //and note, there is an additional helper function, sig(), that returns MET/.5*SQRT(SUMET)
    if(verbose)    cout << "Significance of RefFinal is " << myMET.sig() << endl;
    cout << "+++++++++++++++++++++++++++++" << endl;
     
    delete metCheck;

    if(verbose)  cout << "Demonstration of smearing and systematics" << endl;

    //this demonstration is for doing smearing and systematics
    metSys->reset();
    metSys->setObjects("jets", jet_AntiKt4LCTopo_pt, jet_AntiKt4LCTopo_eta, jet_AntiKt4LCTopo_phi, jet_AntiKt4LCTopo_E, jet_AntiKt4LCTopo_MET_wet, jet_AntiKt4LCTopo_MET_wpx, jet_AntiKt4LCTopo_MET_wpy, jet_AntiKt4LCTopo_MET_statusWord);
    metSys->setExtraJetParameters(jet_AntiKt4LCTopo_GCWJES, jet_AntiKt4LCTopo_m, jet_AntiKt4LCTopo_eta, jet_AntiKt4LCTopo_phi);//Origin, jet_AntiKt4LCTopo_PhiOrigin);//this is necessary for systematics. When MET does overlap a cluster might be applied to an electron and a tau. If that cluster becomes an electron there may be some cells still associated with a tau and these get an MET weight for taus, but below .5. The same occurs for jets. The objects systematics should not be applied to the detritus, so when a systematic is called, these loose cells get shunted into CellOut/CellOut_Eflow. In the case of jets, these loose cells need the EMJES moment removed. So we divide by it when we shunt the cells.
    //note, the function metSys->setCellFix(bool) can override this. Set it to false and it will not happen at all
    //CellFix should not be needed in r17

    //note, you could also correct the regular eta with the GCWJES_etaCorr and feed that in. I opted for the eta that points at the beamspot

    //putting in smeared and/or scaled objects will cause that to be reflected in MET
    metSys->setElectronParameters(el_smeared_pt, el_eta, el_phi, el_MET_wet, el_MET_wpx, el_MET_wpy, el_MET_statusWord);
    metSys->setPhotonParameters(ph_smeared_pt, ph_eta, ph_phi, ph_MET_wet, ph_MET_wpx, ph_MET_wpy, ph_MET_statusWord);
    metSys->setTauParameters(tau_pt, tau_eta, tau_phi, tau_MET_wet, tau_MET_wpx, tau_MET_wpy, tau_MET_statusWord);
    metSys->setMuonParameters(mu_staco_smeared_pt, mu_staco_eta, mu_staco_phi, mu_staco_MET_wet, mu_staco_MET_wpx, mu_staco_MET_wpy, mu_staco_MET_statusWord);
    //in this instance there is an overloaded version of setExtraMuonParameters that accepts smeared pTs for track and spectro
    metSys->setExtraMuonParameters(mu_staco_smeared_ms_pt, mu_staco_ms_theta, mu_staco_ms_phi);
	
    //metSys->setMETTerm("RefTau", MET_RefTau_etx, MET_RefTau_ety, MET_RefTau_sumet);
    metSys->setMETTerm("RefMuon", MET_RefMuon_etx, MET_RefMuon_ety, MET_RefMuon_sumet);
    metSys->setMETTerm("CellOut", MET_CellOut_etx, MET_CellOut_ety, MET_CellOut_sumet);
    metSys->setMETTerm("CellOutEflow", MET_CellOut_Eflow_etx, MET_CellOut_Eflow_ety, MET_CellOut_Eflow_sumet);
	

    metSys->setObjectEnergyUncertainties("jets", jesUp, jesDown);
    metSys->setObjectResolutionShift("jets", jetResUp, jetResDown);

    metSys->setObjectEnergyUncertainties("electrons", eesUp, eesDown);
    metSys->setObjectResolutionShift("electrons", eerUp, eerDown);

    metSys->setObjectEnergyUncertainties("photons", pesUp, pesDown);
    metSys->setObjectResolutionShift("photons", perUp, perDown);

    //the muons are nasty.
    metSys->setObjectResolutionShift("comboms", cb_mermsUp, cb_mermsDown); //muons are complicated, and MET makes use of track, spectro, and combined quantites, so we need all off their resolutions. comboms reflects that it is the combined muon res affected by shifting ms res up and down. comboid is for shifting the id res up and down
    metSys->setObjectResolutionShift("comboid", cb_meridUp, cb_meridDown);
    metSys->setObjectResolutionShift("spectromuons", mermsUp, mermsDown);
    //metSys->setObjectResolutionShift("trackmuons", meridUp, meridDown);

    metSys->setObjectEnergyUncertainties("muons", mesUp, mesDown); //for now the mes only affects combined

    metSys->setObjectEnergyUncertainties("taus", tesUp, tesDown);

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
	
    delete el_smeared_pt;
    delete ph_smeared_pt;
    delete mu_staco_smeared_pt;
    delete mu_staco_smeared_ms_pt;
    delete metSys;

  }//end of loop
   
   
}//end of macro

