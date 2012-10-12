//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu May 12 17:22:52 2011 by ROOT version 5.14/00i
// from TChain qcd/
//////////////////////////////////////////////////////////

#ifndef Example_h
#define Example_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1D.h>
#include <iostream>

class Example {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   TH1D* h_refFinal_D3PD;
   TH1D* h_refFinal_METUtil;
   TH1D* h_refFinal_Diff;
   TH1D* h_refFinal_Delta;

   TH1D* h_refFinal_SUSYmu;
   TH1D* h_refFinal_SUSYMU;

   TH1D* h_refFinal_JERUp;

   TH1D* h_refFinal_JESUp;
   TH1D* h_refFinal_JESDown;

   TH1D* h_refFinal_EESUp;
   TH1D* h_refFinal_EESDown;

   TH1D* h_refFinal_MESUp;
   TH1D* h_refFinal_MESDown;

   TH1D* h_refFinal_MERIDUp;
   TH1D* h_refFinal_MERIDDown;

   TH1D* h_refFinal_MERMSUp;
   TH1D* h_refFinal_MERMSDown;

   TH1D* h_refFinal_COUp;
   TH1D* h_refFinal_CODown;

   TFile *dataout;

   // Declaration of leaf types
   vector<string>  *SkimDecision_NTPSUSY_name;
   vector<int>     *SkimDecision_NTPSUSY_accepted;
   vector<string>  *SkimDecision_DAODJETS_name;
   vector<int>     *SkimDecision_DAODJETS_accepted;
   vector<string>  *SkimDecision_DAODMUONS_name;
   vector<int>     *SkimDecision_DAODMUONS_accepted;
   vector<string>  *SkimDecision_DAODEGAMMA_name;
   vector<int>     *SkimDecision_DAODEGAMMA_accepted;
   UInt_t          RunNumber;
   UInt_t          EventNumber;
   UInt_t          timestamp;
   UInt_t          timestamp_ns;
   UInt_t          lbn;
   UInt_t          bcid;
   UInt_t          detmask0;
   UInt_t          detmask1;
   Float_t         actualIntPerXing;
   Float_t         averageIntPerXing;
   UInt_t          pixelFlags;
   UInt_t          sctFlags;
   UInt_t          trtFlags;
   UInt_t          larFlags;
   UInt_t          tileFlags;
   UInt_t          muonFlags;
   UInt_t          fwdFlags;
   UInt_t          coreFlags;
   UInt_t          pixelError;
   UInt_t          sctError;
   UInt_t          trtError;
   UInt_t          larError;
   UInt_t          tileError;
   UInt_t          muonError;
   UInt_t          fwdError;
   UInt_t          coreError;
   Int_t           el_n;
   vector<float>   *el_E;
   vector<float>   *el_Et;
   vector<float>   *el_pt;
   vector<float>   *el_m;
   vector<float>   *el_eta;
   vector<float>   *el_phi;
   vector<float>   *el_px;
   vector<float>   *el_py;
   vector<float>   *el_pz;
   vector<float>   *el_charge;
   vector<int>     *el_author;
   vector<unsigned int> *el_isEM;
   vector<unsigned int> *el_OQ;
   vector<int>     *el_convFlag;
   vector<int>     *el_isConv;
   vector<int>     *el_nConv;
   vector<int>     *el_nSingleTrackConv;
   vector<int>     *el_nDoubleTrackConv;
   vector<unsigned int> *el_OQRecalc;
   vector<float>   *el_truth_E;
   vector<float>   *el_truth_pt;
   vector<float>   *el_truth_eta;
   vector<float>   *el_truth_phi;
   vector<int>     *el_truth_type;
   vector<int>     *el_truth_status;
   vector<int>     *el_truth_barcode;
   vector<int>     *el_truth_mothertype;
   vector<int>     *el_truth_motherbarcode;
   vector<int>     *el_truth_hasHardBrem;
   vector<int>     *el_truth_matched;
   vector<int>     *el_loose;
   vector<int>     *el_medium;
   vector<int>     *el_mediumIso;
   vector<int>     *el_tight;
   vector<int>     *el_tightIso;
   vector<int>     *el_mediumWithTrack;
   vector<int>     *el_tightWithTrack;
   vector<int>     *el_goodOQ;
   vector<float>   *el_rphiallcalo;
   vector<float>   *el_Etcone45;
   vector<float>   *el_Etcone15;
   vector<float>   *el_Etcone20;
   vector<float>   *el_Etcone25;
   vector<float>   *el_Etcone30;
   vector<float>   *el_Etcone35;
   vector<float>   *el_Etcone40;
   vector<float>   *el_ptcone20;
   vector<float>   *el_ptcone30;
   vector<float>   *el_ptcone40;
   vector<float>   *el_nucone20;
   vector<float>   *el_nucone30;
   vector<float>   *el_nucone40;
   vector<float>   *el_pos7;
   vector<float>   *el_etacorrmag;
   vector<float>   *el_deltaeta1;
   vector<float>   *el_deltaeta2;
   vector<float>   *el_deltaphi2;
   vector<float>   *el_deltaphiRescaled;
   vector<float>   *el_deltaPhiRot;
   vector<float>   *el_expectHitInBLayer;
   vector<float>   *el_trackd0_physics;
   vector<float>   *el_reta;
   vector<float>   *el_rphi;
   vector<float>   *el_zvertex;
   vector<float>   *el_errz;
   vector<float>   *el_etap;
   vector<float>   *el_depth;
   vector<int>     *el_refittedTrack_n;
   vector<float>   *el_cl_E;
   vector<float>   *el_cl_pt;
   vector<float>   *el_cl_eta;
   vector<float>   *el_cl_phi;
   vector<float>   *el_cl_time;
   vector<float>   *el_trackd0;
   vector<float>   *el_trackz0;
   vector<float>   *el_trackphi;
   vector<float>   *el_tracktheta;
   vector<float>   *el_trackqoverp;
   vector<float>   *el_trackpt;
   vector<float>   *el_tracketa;
   vector<int>     *el_nBLHits;
   vector<int>     *el_nPixHits;
   vector<int>     *el_nSCTHits;
   vector<int>     *el_nTRTHits;
   vector<int>     *el_nTRTHighTHits;
   vector<int>     *el_nPixHoles;
   vector<int>     *el_nSCTHoles;
   vector<int>     *el_nTRTHoles;
   vector<int>     *el_nBLSharedHits;
   vector<int>     *el_nPixSharedHits;
   vector<int>     *el_nSCTSharedHits;
   vector<int>     *el_nBLayerOutliers;
   vector<int>     *el_nPixelOutliers;
   vector<int>     *el_nSCTOutliers;
   vector<int>     *el_nTRTOutliers;
   vector<int>     *el_nTRTHighTOutliers;
   vector<int>     *el_expectBLayerHit;
   vector<int>     *el_nSiHits;
   vector<float>   *el_TRTHighTHitsRatio;
   vector<float>   *el_TRTHighTOutliersRatio;
   vector<float>   *el_pixeldEdx;
   vector<int>     *el_nGoodHitsPixeldEdx;
   vector<float>   *el_massPixeldEdx;
   vector<vector<float> > *el_likelihoodsPixeldEdx;
   vector<float>   *el_vertweight;
   vector<int>     *el_hastrack;
   vector<float>   *el_Etcone40_ED_corrected;
   vector<float>   *el_Etcone40_corrected;
   vector<float>   *el_EF_dr;
   Bool_t          EF_2e5_medium;
   Bool_t          EF_2j35_jetNoEF_xe40_noMu;
   Bool_t          EF_2mu10;
   Bool_t          EF_2mu4;
   Bool_t          EF_e10_medium;
   Bool_t          EF_e15_medium_xe30_noMu;
   Bool_t          EF_e15_medium_xe35_noMu;
   Bool_t          EF_e15_medium_xe40_noMu_loose;
   Bool_t          EF_e15_tight_xe30_noMu;
   Bool_t          EF_e20_loose;
   Bool_t          EF_e20_loose_xe20;
   Bool_t          EF_e20_loose_xe20_noMu;
   Bool_t          EF_e20_loose_xe30;
   Bool_t          EF_e20_loose_xe30_noMu;
   Bool_t          EF_e20_medium;
   Bool_t          EF_em105_passHLT;
   Bool_t          EF_g20_etcut_xe30_noMu;
   Bool_t          EF_g20_loose;
   Bool_t          EF_g20_loose_xe20_noMu;
   Bool_t          EF_g20_loose_xe30_noMu;
   Bool_t          EF_g25_loose_xe30_noMu;
   Bool_t          EF_g40_loose_xe40_noMu;
   Bool_t          EF_j55_a4tc_EFFS_xe55_medium_noMu_dphi2j30xe10;
   Bool_t          EF_j75_a4tc_EFFS_xe40_loose_noMu;
   Bool_t          EF_j75_a4tc_EFFS_xe40_loose_noMu_dphijxe03;
   Bool_t          EF_j75_a4tc_EFFS_xe45_loose_noMu;
   Bool_t          EF_j75_a4tc_EFFS_xe55_loose_noMu;
   Bool_t          EF_mu10;
   Bool_t          EF_mu13;
   Bool_t          EF_mu18;
   Bool_t          EF_mu20;
   Bool_t          EF_mu20_MSonly;
   Bool_t          EF_mu4;
   Bool_t          EF_mu6;
   Bool_t          EF_tau16_j75_a4tc_EFFS_xe40_loose_noMu;
   Bool_t          EF_tau16_loose_xe20_noMu;
   Bool_t          EF_tau16_loose_xe25_noMu;
   Bool_t          EF_tau16_loose_xe30_noMu;
   Bool_t          EF_tau16_medium_xe22_noMu;
   Bool_t          EF_tau16_medium_xe25_noMu;
   Bool_t          EF_tau16_medium_xe25_tight_noMu;
   Bool_t          EF_tau29_medium_xe35_noMu;
   Bool_t          EF_tau29_medium_xe40_loose_noMu;
   Bool_t          EF_xe20_noMu;
   Bool_t          EF_xe30_noMu;
   Bool_t          EF_xe35_noMu;
   Bool_t          EF_xe40_noMu;
   Bool_t          EF_xe50_noMu;
   Bool_t          EF_xe60_noMu;
   Bool_t          EF_xe70_noMu;
   Bool_t          EF_xe80_noMu;
   Bool_t          EF_xe90_noMu;
   Bool_t          L1_2J15_XE25;
   Bool_t          L1_EM10_XE20;
   Bool_t          L1_EM14_XE10;
   Bool_t          L1_EM14_XE20;
   Bool_t          L1_J30_XE35;
   Bool_t          L1_J50_XE20;
   Bool_t          L1_J50_XE25;
   Bool_t          L1_MU0;
   Bool_t          L1_MU10;
   Bool_t          L1_MU20;
   Bool_t          L1_MU6;
   Bool_t          L1_TAU11_XE10_3J10;
   Bool_t          L1_TAU11_XE20;
   Bool_t          L1_TAU15_XE20;
   Bool_t          L1_TAU6_J50_XE20;
   Bool_t          L1_TAU6_XE10;
   Bool_t          L1_XE10;
   Bool_t          L1_XE20;
   Bool_t          L1_XE25;
   Bool_t          L1_XE30;
   Bool_t          L1_XE35;
   Bool_t          L1_XE40;
   Bool_t          L1_XE50;
   Bool_t          L1_XE60;
   Bool_t          L2_2j35_jetNoEF_xe40_noMu;
   Bool_t          L2_2mu4;
   Bool_t          L2_e15_medium_xe30_noMu;
   Bool_t          L2_e15_medium_xe35_noMu;
   Bool_t          L2_e15_medium_xe40_noMu_loose;
   Bool_t          L2_e15_tight_xe30_noMu;
   Bool_t          L2_e20_loose_xe20;
   Bool_t          L2_e20_loose_xe20_noMu;
   Bool_t          L2_e20_loose_xe30;
   Bool_t          L2_e20_loose_xe30_noMu;
   Bool_t          L2_g20_etcut_xe30_noMu;
   Bool_t          L2_g20_loose_xe20_noMu;
   Bool_t          L2_g20_loose_xe30_noMu;
   Bool_t          L2_g25_loose_xe30_noMu;
   Bool_t          L2_g40_loose_xe40_noMu;
   Bool_t          L2_j50_xe35_medium_noMu;
   Bool_t          L2_j70_xe20_loose_noMu;
   Bool_t          L2_j70_xe25_loose_noMu;
   Bool_t          L2_mu10;
   Bool_t          L2_mu13;
   Bool_t          L2_mu20;
   Bool_t          L2_mu20_MSonly;
   Bool_t          L2_mu4;
   Bool_t          L2_mu6;
   Bool_t          L2_tau16_j70_xe20_loose_noMu;
   Bool_t          L2_tau16_loose_xe20_noMu;
   Bool_t          L2_tau16_loose_xe25_noMu;
   Bool_t          L2_tau16_loose_xe30_noMu;
   Bool_t          L2_tau16_medium_xe22_noMu;
   Bool_t          L2_tau16_medium_xe25_noMu;
   Bool_t          L2_tau16_medium_xe25_tight_noMu;
   Bool_t          L2_tau29_medium_xe25_noMu;
   Bool_t          L2_tau29_medium_xe30_loose_noMu;
   Bool_t          L2_xe10_noMu;
   Bool_t          L2_xe20_noMu;
   Bool_t          L2_xe25_noMu;
   Bool_t          L2_xe30_noMu;
   Bool_t          L2_xe35_noMu;
   Bool_t          L2_xe40_noMu;
   Bool_t          L2_xe50_noMu;
   Bool_t          L2_xe60_noMu;
   Bool_t          L2_xe70_noMu;
   Int_t           ph_n;
   vector<float>   *ph_E;
   vector<float>   *ph_Et;
   vector<float>   *ph_pt;
   vector<float>   *ph_m;
   vector<float>   *ph_eta;
   vector<float>   *ph_phi;
   vector<float>   *ph_px;
   vector<float>   *ph_py;
   vector<float>   *ph_pz;
   vector<int>     *ph_author;
   vector<int>     *ph_isRecovered;
   vector<unsigned int> *ph_isEM;
   vector<unsigned int> *ph_OQ;
   vector<unsigned int> *ph_OQRecalc;
   vector<int>     *ph_convFlag;
   vector<int>     *ph_isConv;
   vector<int>     *ph_nConv;
   vector<int>     *ph_nSingleTrackConv;
   vector<int>     *ph_nDoubleTrackConv;
   vector<float>   *ph_truth_deltaRRecPhoton;
   vector<float>   *ph_truth_E;
   vector<float>   *ph_truth_pt;
   vector<float>   *ph_truth_eta;
   vector<float>   *ph_truth_phi;
   vector<int>     *ph_truth_type;
   vector<int>     *ph_truth_status;
   vector<int>     *ph_truth_barcode;
   vector<int>     *ph_truth_mothertype;
   vector<int>     *ph_truth_motherbarcode;
   vector<int>     *ph_truth_matched;
   vector<int>     *ph_loose;
   vector<int>     *ph_tight;
   vector<int>     *ph_tightIso;
   vector<int>     *ph_goodOQ;
   vector<float>   *ph_rphiallcalo;
   vector<float>   *ph_Etcone45;
   vector<float>   *ph_Etcone15;
   vector<float>   *ph_Etcone20;
   vector<float>   *ph_Etcone25;
   vector<float>   *ph_Etcone30;
   vector<float>   *ph_Etcone35;
   vector<float>   *ph_Etcone40;
   vector<float>   *ph_ptcone20;
   vector<float>   *ph_ptcone30;
   vector<float>   *ph_ptcone40;
   vector<float>   *ph_nucone20;
   vector<float>   *ph_nucone30;
   vector<float>   *ph_nucone40;
   vector<float>   *ph_convanglematch;
   vector<float>   *ph_convtrackmatch;
   vector<int>     *ph_hasconv;
   vector<float>   *ph_convvtxx;
   vector<float>   *ph_convvtxy;
   vector<float>   *ph_convvtxz;
   vector<float>   *ph_Rconv;
   vector<float>   *ph_zconv;
   vector<float>   *ph_convvtxchi2;
   vector<float>   *ph_pt1conv;
   vector<int>     *ph_convtrk1nBLHits;
   vector<int>     *ph_convtrk1nPixHits;
   vector<int>     *ph_convtrk1nSCTHits;
   vector<int>     *ph_convtrk1nTRTHits;
   vector<float>   *ph_pt2conv;
   vector<int>     *ph_convtrk2nBLHits;
   vector<int>     *ph_convtrk2nPixHits;
   vector<int>     *ph_convtrk2nSCTHits;
   vector<int>     *ph_convtrk2nTRTHits;
   vector<float>   *ph_ptconv;
   vector<float>   *ph_pzconv;
   vector<float>   *ph_reta;
   vector<float>   *ph_rphi;
   vector<float>   *ph_zvertex;
   vector<float>   *ph_errz;
   vector<float>   *ph_etap;
   vector<float>   *ph_depth;
   vector<float>   *ph_cl_E;
   vector<float>   *ph_cl_pt;
   vector<float>   *ph_cl_eta;
   vector<float>   *ph_cl_phi;
   vector<float>   *ph_cl_time;
   vector<int>     *ph_truth_isConv;
   vector<int>     *ph_truth_isBrem;
   vector<int>     *ph_truth_isFromHardProc;
   vector<int>     *ph_truth_isPhotonFromHardProc;
   vector<float>   *ph_truth_Rconv;
   vector<float>   *ph_truth_zconv;
   vector<float>   *ph_deltaEmax2;
   vector<float>   *ph_jet_dr;
   vector<int>     *ph_jet_matched;
   vector<float>   *ph_convIP;
   vector<float>   *ph_convIPRev;
   vector<float>   *ph_ptIsolationCone;
   vector<float>   *ph_ptIsolationConePhAngle;
   vector<float>   *ph_Etcone40_ED_corrected;
   vector<float>   *ph_Etcone40_corrected;
   Int_t           mu_staco_n;
   vector<float>   *mu_staco_E;
   vector<float>   *mu_staco_pt;
   vector<float>   *mu_staco_m;
   vector<float>   *mu_staco_eta;
   vector<float>   *mu_staco_phi;
   vector<float>   *mu_staco_px;
   vector<float>   *mu_staco_py;
   vector<float>   *mu_staco_pz;
   vector<float>   *mu_staco_charge;
   vector<unsigned short> *mu_staco_allauthor;
   vector<int>     *mu_staco_author;
   vector<float>   *mu_staco_matchchi2;
   vector<int>     *mu_staco_matchndof;
   vector<float>   *mu_staco_etcone20;
   vector<float>   *mu_staco_etcone30;
   vector<float>   *mu_staco_etcone40;
   vector<float>   *mu_staco_nucone20;
   vector<float>   *mu_staco_nucone30;
   vector<float>   *mu_staco_nucone40;
   vector<float>   *mu_staco_ptcone20;
   vector<float>   *mu_staco_ptcone30;
   vector<float>   *mu_staco_ptcone40;
   vector<float>   *mu_staco_energyLossPar;
   vector<float>   *mu_staco_energyLossErr;
   vector<float>   *mu_staco_etCore;
   vector<float>   *mu_staco_energyLossType;
   vector<unsigned short> *mu_staco_caloMuonIdTag;
   vector<double>  *mu_staco_caloLRLikelihood;
   vector<int>     *mu_staco_bestMatch;
   vector<int>     *mu_staco_isStandAloneMuon;
   vector<int>     *mu_staco_isCombinedMuon;
   vector<int>     *mu_staco_isLowPtReconstructedMuon;
   vector<int>     *mu_staco_isSegmentTaggedMuon;
   vector<int>     *mu_staco_isCaloMuonId;
   vector<int>     *mu_staco_alsoFoundByLowPt;
   vector<int>     *mu_staco_alsoFoundByCaloMuonId;
   vector<int>     *mu_staco_loose;
   vector<int>     *mu_staco_medium;
   vector<int>     *mu_staco_tight;
   vector<float>   *mu_staco_d0_exPV;
   vector<float>   *mu_staco_z0_exPV;
   vector<float>   *mu_staco_phi_exPV;
   vector<float>   *mu_staco_theta_exPV;
   vector<float>   *mu_staco_qoverp_exPV;
   vector<float>   *mu_staco_cb_d0_exPV;
   vector<float>   *mu_staco_cb_z0_exPV;
   vector<float>   *mu_staco_cb_phi_exPV;
   vector<float>   *mu_staco_cb_theta_exPV;
   vector<float>   *mu_staco_cb_qoverp_exPV;
   vector<float>   *mu_staco_id_d0_exPV;
   vector<float>   *mu_staco_id_z0_exPV;
   vector<float>   *mu_staco_id_phi_exPV;
   vector<float>   *mu_staco_id_theta_exPV;
   vector<float>   *mu_staco_id_qoverp_exPV;
   vector<float>   *mu_staco_me_d0_exPV;
   vector<float>   *mu_staco_me_z0_exPV;
   vector<float>   *mu_staco_me_phi_exPV;
   vector<float>   *mu_staco_me_theta_exPV;
   vector<float>   *mu_staco_me_qoverp_exPV;
   vector<float>   *mu_staco_ie_d0_exPV;
   vector<float>   *mu_staco_ie_z0_exPV;
   vector<float>   *mu_staco_ie_phi_exPV;
   vector<float>   *mu_staco_ie_theta_exPV;
   vector<float>   *mu_staco_ie_qoverp_exPV;
   vector<float>   *mu_staco_ms_d0;
   vector<float>   *mu_staco_ms_z0;
   vector<float>   *mu_staco_ms_phi;
   vector<float>   *mu_staco_ms_theta;
   vector<float>   *mu_staco_ms_qoverp;
   vector<float>   *mu_staco_id_d0;
   vector<float>   *mu_staco_id_z0;
   vector<float>   *mu_staco_id_phi;
   vector<float>   *mu_staco_id_theta;
   vector<float>   *mu_staco_id_qoverp;
   vector<float>   *mu_staco_me_d0;
   vector<float>   *mu_staco_me_z0;
   vector<float>   *mu_staco_me_phi;
   vector<float>   *mu_staco_me_theta;
   vector<float>   *mu_staco_me_qoverp;
   vector<float>   *mu_staco_ie_d0;
   vector<float>   *mu_staco_ie_z0;
   vector<float>   *mu_staco_ie_phi;
   vector<float>   *mu_staco_ie_theta;
   vector<float>   *mu_staco_ie_qoverp;
   vector<int>     *mu_staco_nOutliersOnTrack;
   vector<int>     *mu_staco_nBLHits;
   vector<int>     *mu_staco_nPixHits;
   vector<int>     *mu_staco_nSCTHits;
   vector<int>     *mu_staco_nTRTHits;
   vector<int>     *mu_staco_nTRTHighTHits;
   vector<int>     *mu_staco_nBLSharedHits;
   vector<int>     *mu_staco_nPixSharedHits;
   vector<int>     *mu_staco_nPixHoles;
   vector<int>     *mu_staco_nSCTSharedHits;
   vector<int>     *mu_staco_nSCTHoles;
   vector<int>     *mu_staco_nTRTOutliers;
   vector<int>     *mu_staco_nTRTHighTOutliers;
   vector<int>     *mu_staco_nGangedPixels;
   vector<int>     *mu_staco_nPixelDeadSensors;
   vector<int>     *mu_staco_nSCTDeadSensors;
   vector<int>     *mu_staco_nTRTDeadStraws;
   vector<int>     *mu_staco_expectBLayerHit;
   vector<int>     *mu_staco_nMDTHits;
   vector<int>     *mu_staco_nMDTHoles;
   vector<int>     *mu_staco_nCSCEtaHits;
   vector<int>     *mu_staco_nCSCEtaHoles;
   vector<int>     *mu_staco_nCSCPhiHits;
   vector<int>     *mu_staco_nCSCPhiHoles;
   vector<int>     *mu_staco_nRPCEtaHits;
   vector<int>     *mu_staco_nRPCEtaHoles;
   vector<int>     *mu_staco_nRPCPhiHits;
   vector<int>     *mu_staco_nRPCPhiHoles;
   vector<int>     *mu_staco_nTGCEtaHits;
   vector<int>     *mu_staco_nTGCEtaHoles;
   vector<int>     *mu_staco_nTGCPhiHits;
   vector<int>     *mu_staco_nTGCPhiHoles;
   vector<int>     *mu_staco_nMDTBIHits;
   vector<int>     *mu_staco_nMDTBMHits;
   vector<int>     *mu_staco_nMDTBOHits;
   vector<int>     *mu_staco_nMDTBEEHits;
   vector<int>     *mu_staco_nMDTBIS78Hits;
   vector<int>     *mu_staco_nMDTEIHits;
   vector<int>     *mu_staco_nMDTEMHits;
   vector<int>     *mu_staco_nMDTEOHits;
   vector<int>     *mu_staco_nMDTEEHits;
   vector<int>     *mu_staco_nRPCLayer1EtaHits;
   vector<int>     *mu_staco_nRPCLayer2EtaHits;
   vector<int>     *mu_staco_nRPCLayer3EtaHits;
   vector<int>     *mu_staco_nRPCLayer1PhiHits;
   vector<int>     *mu_staco_nRPCLayer2PhiHits;
   vector<int>     *mu_staco_nRPCLayer3PhiHits;
   vector<int>     *mu_staco_nTGCLayer1EtaHits;
   vector<int>     *mu_staco_nTGCLayer2EtaHits;
   vector<int>     *mu_staco_nTGCLayer3EtaHits;
   vector<int>     *mu_staco_nTGCLayer4EtaHits;
   vector<int>     *mu_staco_nTGCLayer1PhiHits;
   vector<int>     *mu_staco_nTGCLayer2PhiHits;
   vector<int>     *mu_staco_nTGCLayer3PhiHits;
   vector<int>     *mu_staco_nTGCLayer4PhiHits;
   vector<int>     *mu_staco_barrelSectors;
   vector<int>     *mu_staco_endcapSectors;
   vector<float>   *mu_staco_trackd0;
   vector<float>   *mu_staco_trackz0;
   vector<float>   *mu_staco_trackphi;
   vector<float>   *mu_staco_tracktheta;
   vector<float>   *mu_staco_trackqoverp;
   vector<float>   *mu_staco_trackcov_d0;
   vector<float>   *mu_staco_trackcov_z0;
   vector<float>   *mu_staco_trackcov_phi;
   vector<float>   *mu_staco_trackcov_theta;
   vector<float>   *mu_staco_trackcov_qoverp;
   vector<float>   *mu_staco_trackcov_d0_z0;
   vector<float>   *mu_staco_trackcov_d0_phi;
   vector<float>   *mu_staco_trackcov_d0_theta;
   vector<float>   *mu_staco_trackcov_d0_qoverp;
   vector<float>   *mu_staco_trackcov_z0_phi;
   vector<float>   *mu_staco_trackcov_z0_theta;
   vector<float>   *mu_staco_trackcov_z0_qoverp;
   vector<float>   *mu_staco_trackcov_phi_theta;
   vector<float>   *mu_staco_trackcov_phi_qoverp;
   vector<float>   *mu_staco_trackcov_theta_qoverp;
   vector<int>     *mu_staco_hastrack;
   vector<float>   *mu_staco_truth_dr;
   vector<float>   *mu_staco_truth_E;
   vector<float>   *mu_staco_truth_pt;
   vector<float>   *mu_staco_truth_eta;
   vector<float>   *mu_staco_truth_phi;
   vector<int>     *mu_staco_truth_type;
   vector<int>     *mu_staco_truth_status;
   vector<int>     *mu_staco_truth_barcode;
   vector<int>     *mu_staco_truth_mothertype;
   vector<int>     *mu_staco_truth_motherbarcode;
   vector<int>     *mu_staco_truth_matched;
   vector<float>   *mu_staco_EFCB_dr;
   vector<int>     *mu_staco_EFCB_index;
   vector<float>   *mu_staco_EFMG_dr;
   vector<float>   *mu_staco_EFME_dr;
   vector<int>     *mu_staco_EFME_index;
   Float_t         MET_Simplified20_RefGamma_etx;
   Float_t         MET_Simplified20_RefGamma_ety;
   Float_t         MET_Simplified20_RefGamma_sumet;
   Float_t         MET_Simplified20_RefGamma_etx_CentralReg;
   Float_t         MET_Simplified20_RefGamma_ety_CentralReg;
   Float_t         MET_Simplified20_RefGamma_sumet_CentralReg;
   Float_t         MET_Simplified20_RefGamma_phi_CentralReg;
   Float_t         MET_Simplified20_RefGamma_etx_EndcapRegion;
   Float_t         MET_Simplified20_RefGamma_ety_EndcapRegion;
   Float_t         MET_Simplified20_RefGamma_sumet_EndcapRegion;
   Float_t         MET_Simplified20_RefGamma_phi_EndcapRegion;
   Float_t         MET_Simplified20_RefGamma_etx_ForwardReg;
   Float_t         MET_Simplified20_RefGamma_ety_ForwardReg;
   Float_t         MET_Simplified20_RefGamma_sumet_ForwardReg;
   Float_t         MET_Simplified20_RefGamma_phi_ForwardReg;
   Float_t         MET_Simplified20_RefFinal_etx;
   Float_t         MET_Simplified20_RefFinal_ety;
   Float_t         MET_Simplified20_RefFinal_sumet;
   Float_t         MET_Simplified20_RefFinal_etx_CentralReg;
   Float_t         MET_Simplified20_RefFinal_ety_CentralReg;
   Float_t         MET_Simplified20_RefFinal_sumet_CentralReg;
   Float_t         MET_Simplified20_RefFinal_phi_CentralReg;
   Float_t         MET_Simplified20_RefFinal_etx_EndcapRegion;
   Float_t         MET_Simplified20_RefFinal_ety_EndcapRegion;
   Float_t         MET_Simplified20_RefFinal_sumet_EndcapRegion;
   Float_t         MET_Simplified20_RefFinal_phi_EndcapRegion;
   Float_t         MET_Simplified20_RefFinal_etx_ForwardReg;
   Float_t         MET_Simplified20_RefFinal_ety_ForwardReg;
   Float_t         MET_Simplified20_RefFinal_sumet_ForwardReg;
   Float_t         MET_Simplified20_RefFinal_phi_ForwardReg;
   Float_t         MET_Simplified20_RefJet_etx;
   Float_t         MET_Simplified20_RefJet_ety;
   Float_t         MET_Simplified20_RefJet_sumet;
   Float_t         MET_Simplified20_RefJet_etx_CentralReg;
   Float_t         MET_Simplified20_RefJet_ety_CentralReg;
   Float_t         MET_Simplified20_RefJet_sumet_CentralReg;
   Float_t         MET_Simplified20_RefJet_phi_CentralReg;
   Float_t         MET_Simplified20_RefJet_etx_EndcapRegion;
   Float_t         MET_Simplified20_RefJet_ety_EndcapRegion;
   Float_t         MET_Simplified20_RefJet_sumet_EndcapRegion;
   Float_t         MET_Simplified20_RefJet_phi_EndcapRegion;
   Float_t         MET_Simplified20_RefJet_etx_ForwardReg;
   Float_t         MET_Simplified20_RefJet_ety_ForwardReg;
   Float_t         MET_Simplified20_RefJet_sumet_ForwardReg;
   Float_t         MET_Simplified20_RefJet_phi_ForwardReg;
   Float_t         MET_Simplified20_RefEle_etx;
   Float_t         MET_Simplified20_RefEle_ety;
   Float_t         MET_Simplified20_RefEle_sumet;
   Float_t         MET_Simplified20_RefEle_etx_CentralReg;
   Float_t         MET_Simplified20_RefEle_ety_CentralReg;
   Float_t         MET_Simplified20_RefEle_sumet_CentralReg;
   Float_t         MET_Simplified20_RefEle_phi_CentralReg;
   Float_t         MET_Simplified20_RefEle_etx_EndcapRegion;
   Float_t         MET_Simplified20_RefEle_ety_EndcapRegion;
   Float_t         MET_Simplified20_RefEle_sumet_EndcapRegion;
   Float_t         MET_Simplified20_RefEle_phi_EndcapRegion;
   Float_t         MET_Simplified20_RefEle_etx_ForwardReg;
   Float_t         MET_Simplified20_RefEle_ety_ForwardReg;
   Float_t         MET_Simplified20_RefEle_sumet_ForwardReg;
   Float_t         MET_Simplified20_RefEle_phi_ForwardReg;
   Float_t         MET_Simplified20_Muon_Total_Staco_etx;
   Float_t         MET_Simplified20_Muon_Total_Staco_ety;
   Float_t         MET_Simplified20_Muon_Total_Staco_sumet;
   Float_t         MET_Simplified20_Muon_Total_Staco_etx_CentralReg;
   Float_t         MET_Simplified20_Muon_Total_Staco_ety_CentralReg;
   Float_t         MET_Simplified20_Muon_Total_Staco_sumet_CentralReg;
   Float_t         MET_Simplified20_Muon_Total_Staco_phi_CentralReg;
   Float_t         MET_Simplified20_Muon_Total_Staco_etx_EndcapRegion;
   Float_t         MET_Simplified20_Muon_Total_Staco_ety_EndcapRegion;
   Float_t         MET_Simplified20_Muon_Total_Staco_sumet_EndcapRegion;
   Float_t         MET_Simplified20_Muon_Total_Staco_phi_EndcapRegion;
   Float_t         MET_Simplified20_Muon_Total_Staco_etx_ForwardReg;
   Float_t         MET_Simplified20_Muon_Total_Staco_ety_ForwardReg;
   Float_t         MET_Simplified20_Muon_Total_Staco_sumet_ForwardReg;
   Float_t         MET_Simplified20_Muon_Total_Staco_phi_ForwardReg;
   Float_t         MET_Simplified20_Muon_Isol_Staco_etx;
   Float_t         MET_Simplified20_Muon_Isol_Staco_ety;
   Float_t         MET_Simplified20_Muon_Isol_Staco_sumet;
   Float_t         MET_Simplified20_Muon_Isol_Staco_etx_CentralReg;
   Float_t         MET_Simplified20_Muon_Isol_Staco_ety_CentralReg;
   Float_t         MET_Simplified20_Muon_Isol_Staco_sumet_CentralReg;
   Float_t         MET_Simplified20_Muon_Isol_Staco_phi_CentralReg;
   Float_t         MET_Simplified20_Muon_Isol_Staco_etx_EndcapRegion;
   Float_t         MET_Simplified20_Muon_Isol_Staco_ety_EndcapRegion;
   Float_t         MET_Simplified20_Muon_Isol_Staco_sumet_EndcapRegion;
   Float_t         MET_Simplified20_Muon_Isol_Staco_phi_EndcapRegion;
   Float_t         MET_Simplified20_Muon_Isol_Staco_etx_ForwardReg;
   Float_t         MET_Simplified20_Muon_Isol_Staco_ety_ForwardReg;
   Float_t         MET_Simplified20_Muon_Isol_Staco_sumet_ForwardReg;
   Float_t         MET_Simplified20_Muon_Isol_Staco_phi_ForwardReg;
   Float_t         MET_Simplified20_Muon_NonIsol_Staco_etx;
   Float_t         MET_Simplified20_Muon_NonIsol_Staco_ety;
   Float_t         MET_Simplified20_Muon_NonIsol_Staco_sumet;
   Float_t         MET_Simplified20_Muon_NonIsol_Staco_etx_CentralReg;
   Float_t         MET_Simplified20_Muon_NonIsol_Staco_ety_CentralReg;
   Float_t         MET_Simplified20_Muon_NonIsol_Staco_sumet_CentralReg;
   Float_t         MET_Simplified20_Muon_NonIsol_Staco_phi_CentralReg;
   Float_t         MET_Simplified20_Muon_NonIsol_Staco_etx_EndcapRegion;
   Float_t         MET_Simplified20_Muon_NonIsol_Staco_ety_EndcapRegion;
   Float_t         MET_Simplified20_Muon_NonIsol_Staco_sumet_EndcapRegion;
   Float_t         MET_Simplified20_Muon_NonIsol_Staco_phi_EndcapRegion;
   Float_t         MET_Simplified20_Muon_NonIsol_Staco_etx_ForwardReg;
   Float_t         MET_Simplified20_Muon_NonIsol_Staco_ety_ForwardReg;
   Float_t         MET_Simplified20_Muon_NonIsol_Staco_sumet_ForwardReg;
   Float_t         MET_Simplified20_Muon_NonIsol_Staco_phi_ForwardReg;
   Float_t         MET_Simplified20_CellOut_etx;
   Float_t         MET_Simplified20_CellOut_ety;
   Float_t         MET_Simplified20_CellOut_sumet;
   Float_t         MET_Simplified20_CellOut_etx_CentralReg;
   Float_t         MET_Simplified20_CellOut_ety_CentralReg;
   Float_t         MET_Simplified20_CellOut_sumet_CentralReg;
   Float_t         MET_Simplified20_CellOut_phi_CentralReg;
   Float_t         MET_Simplified20_CellOut_etx_EndcapRegion;
   Float_t         MET_Simplified20_CellOut_ety_EndcapRegion;
   Float_t         MET_Simplified20_CellOut_sumet_EndcapRegion;
   Float_t         MET_Simplified20_CellOut_phi_EndcapRegion;
   Float_t         MET_Simplified20_CellOut_etx_ForwardReg;
   Float_t         MET_Simplified20_CellOut_ety_ForwardReg;
   Float_t         MET_Simplified20_CellOut_sumet_ForwardReg;
   Float_t         MET_Simplified20_CellOut_phi_ForwardReg;
   Float_t         MET_Simplified20_CellOut_Eflow_etx;
   Float_t         MET_Simplified20_CellOut_Eflow_ety;
   Float_t         MET_Simplified20_CellOut_Eflow_sumet;
   Float_t         MET_Simplified20_CellOut_Eflow_etx_CentralReg;
   Float_t         MET_Simplified20_CellOut_Eflow_ety_CentralReg;
   Float_t         MET_Simplified20_CellOut_Eflow_sumet_CentralReg;
   Float_t         MET_Simplified20_CellOut_Eflow_phi_CentralReg;
   Float_t         MET_Simplified20_CellOut_Eflow_etx_EndcapRegion;
   Float_t         MET_Simplified20_CellOut_Eflow_ety_EndcapRegion;
   Float_t         MET_Simplified20_CellOut_Eflow_sumet_EndcapRegion;
   Float_t         MET_Simplified20_CellOut_Eflow_phi_EndcapRegion;
   Float_t         MET_Simplified20_CellOut_Eflow_etx_ForwardReg;
   Float_t         MET_Simplified20_CellOut_Eflow_ety_ForwardReg;
   Float_t         MET_Simplified20_CellOut_Eflow_sumet_ForwardReg;
   Float_t         MET_Simplified20_CellOut_Eflow_phi_ForwardReg;
   Int_t           el_MET_Simplified20_n;
   vector<vector<float> > *el_MET_Simplified20_wpx;
   vector<vector<float> > *el_MET_Simplified20_wpy;
   vector<vector<float> > *el_MET_Simplified20_wet;
   vector<vector<unsigned int> > *el_MET_Simplified20_statusWord;
   Int_t           mu_staco_MET_Simplified20_n;
   vector<vector<float> > *mu_staco_MET_Simplified20_wpx;
   vector<vector<float> > *mu_staco_MET_Simplified20_wpy;
   vector<vector<float> > *mu_staco_MET_Simplified20_wet;
   vector<vector<unsigned int> > *mu_staco_MET_Simplified20_statusWord;
   Int_t           mu_muid_MET_Simplified20_n;
   vector<vector<float> > *mu_muid_MET_Simplified20_wpx;
   vector<vector<float> > *mu_muid_MET_Simplified20_wpy;
   vector<vector<float> > *mu_muid_MET_Simplified20_wet;
   vector<vector<unsigned int> > *mu_muid_MET_Simplified20_statusWord;
   Int_t           jet_AntiKt4TopoNewEM_MET_Simplified20_n;
   vector<vector<float> > *jet_AntiKt4TopoNewEM_MET_Simplified20_wpx;
   vector<vector<float> > *jet_AntiKt4TopoNewEM_MET_Simplified20_wpy;
   vector<vector<float> > *jet_AntiKt4TopoNewEM_MET_Simplified20_wet;
   vector<vector<unsigned int> > *jet_AntiKt4TopoNewEM_MET_Simplified20_statusWord;
   Int_t           el_MET_n;
   vector<vector<float> > *el_MET_wpx;
   vector<vector<float> > *el_MET_wpy;
   vector<vector<float> > *el_MET_wet;
   vector<vector<unsigned int> > *el_MET_statusWord;
   Int_t           ph_MET_n;
   vector<vector<float> > *ph_MET_wpx;
   vector<vector<float> > *ph_MET_wpy;
   vector<vector<float> > *ph_MET_wet;
   vector<vector<unsigned int> > *ph_MET_statusWord;
   Int_t           mu_staco_MET_n;
   vector<vector<float> > *mu_staco_MET_wpx;
   vector<vector<float> > *mu_staco_MET_wpy;
   vector<vector<float> > *mu_staco_MET_wet;
   vector<vector<unsigned int> > *mu_staco_MET_statusWord;
   Int_t           mu_muid_MET_n;
   vector<vector<float> > *mu_muid_MET_wpx;
   vector<vector<float> > *mu_muid_MET_wpy;
   vector<vector<float> > *mu_muid_MET_wet;
   vector<vector<unsigned int> > *mu_muid_MET_statusWord;
   Int_t           tau_MET_n;
   vector<vector<float> > *tau_MET_wpx;
   vector<vector<float> > *tau_MET_wpy;
   vector<vector<float> > *tau_MET_wet;
   vector<vector<unsigned int> > *tau_MET_statusWord;
   Int_t           jet_AntiKt6LCTopo_MET_n;
   vector<vector<float> > *jet_AntiKt6LCTopo_MET_wpx;
   vector<vector<float> > *jet_AntiKt6LCTopo_MET_wpy;
   vector<vector<float> > *jet_AntiKt6LCTopo_MET_wet;
   vector<vector<unsigned int> > *jet_AntiKt6LCTopo_MET_statusWord;
   Int_t           cl_MET_n;
   vector<vector<float> > *cl_MET_wpx;
   vector<vector<float> > *cl_MET_wpy;
   vector<vector<float> > *cl_MET_wet;
   vector<vector<unsigned int> > *cl_MET_statusWord;
   Int_t           trk_MET_n;
   vector<vector<float> > *trk_MET_wpx;
   vector<vector<float> > *trk_MET_wpy;
   vector<vector<float> > *trk_MET_wet;
   vector<vector<unsigned int> > *trk_MET_statusWord;
   Int_t           jet_AntiKt4TopoNewEM_n;
   vector<float>   *jet_AntiKt4TopoNewEM_E;
   vector<float>   *jet_AntiKt4TopoNewEM_pt;
   vector<float>   *jet_AntiKt4TopoNewEM_m;
   vector<float>   *jet_AntiKt4TopoNewEM_eta;
   vector<float>   *jet_AntiKt4TopoNewEM_phi;
   vector<float>   *jet_AntiKt4TopoNewEM_EtaOrigin;
   vector<float>   *jet_AntiKt4TopoNewEM_PhiOrigin;
   vector<float>   *jet_AntiKt4TopoNewEM_MOrigin;
   vector<float>   *jet_AntiKt4TopoNewEM_EtaOriginEM;
   vector<float>   *jet_AntiKt4TopoNewEM_PhiOriginEM;
   vector<float>   *jet_AntiKt4TopoNewEM_MOriginEM;
   vector<float>   *jet_AntiKt4TopoNewEM_WIDTH;
   vector<float>   *jet_AntiKt4TopoNewEM_n90;
   vector<float>   *jet_AntiKt4TopoNewEM_Timing;
   vector<float>   *jet_AntiKt4TopoNewEM_LArQuality;
   vector<float>   *jet_AntiKt4TopoNewEM_nTrk;
   vector<float>   *jet_AntiKt4TopoNewEM_sumPtTrk;
   vector<float>   *jet_AntiKt4TopoNewEM_OriginIndex;
   vector<float>   *jet_AntiKt4TopoNewEM_HECQuality;
   vector<float>   *jet_AntiKt4TopoNewEM_NegativeE;
   vector<float>   *jet_AntiKt4TopoNewEM_AverageLArQF;
   vector<float>   *jet_AntiKt4TopoNewEM_YFlip12;
   vector<float>   *jet_AntiKt4TopoNewEM_YFlip23;
   vector<float>   *jet_AntiKt4TopoNewEM_BCH_CORR_CELL;
   vector<float>   *jet_AntiKt4TopoNewEM_BCH_CORR_DOTX;
   vector<float>   *jet_AntiKt4TopoNewEM_BCH_CORR_JET;
   vector<float>   *jet_AntiKt4TopoNewEM_BCH_CORR_JET_FORCELL;
   vector<float>   *jet_AntiKt4TopoNewEM_ENG_BAD_CELLS;
   vector<float>   *jet_AntiKt4TopoNewEM_N_BAD_CELLS;
   vector<float>   *jet_AntiKt4TopoNewEM_N_BAD_CELLS_CORR;
   vector<float>   *jet_AntiKt4TopoNewEM_BAD_CELLS_CORR_E;
   vector<float>   *jet_AntiKt4TopoNewEM_NumTowers;
   vector<int>     *jet_AntiKt4TopoNewEM_SamplingMax;
   vector<float>   *jet_AntiKt4TopoNewEM_fracSamplingMax;
   vector<float>   *jet_AntiKt4TopoNewEM_hecf;
   vector<float>   *jet_AntiKt4TopoNewEM_tgap3f;
   vector<int>     *jet_AntiKt4TopoNewEM_isUgly;
   vector<int>     *jet_AntiKt4TopoNewEM_isBadLoose;
   vector<int>     *jet_AntiKt4TopoNewEM_isBadMedium;
   vector<int>     *jet_AntiKt4TopoNewEM_isBadTight;
   vector<float>   *jet_AntiKt4TopoNewEM_emfrac;
   vector<float>   *jet_AntiKt4TopoNewEM_Offset;
   vector<float>   *jet_AntiKt4TopoNewEM_EMJES;
   vector<float>   *jet_AntiKt4TopoNewEM_EMJES_EtaCorr;
   vector<float>   *jet_AntiKt4TopoNewEM_EMJESnooffset;
   vector<float>   *jet_AntiKt4TopoNewEM_GCWJES;
   vector<float>   *jet_AntiKt4TopoNewEM_GCWJES_EtaCorr;
   vector<float>   *jet_AntiKt4TopoNewEM_CB;
   vector<float>   *jet_AntiKt4TopoNewEM_emscale_E;
   vector<float>   *jet_AntiKt4TopoNewEM_emscale_pt;
   vector<float>   *jet_AntiKt4TopoNewEM_emscale_m;
   vector<float>   *jet_AntiKt4TopoNewEM_emscale_eta;
   vector<float>   *jet_AntiKt4TopoNewEM_emscale_phi;
   vector<float>   *jet_AntiKt4TopoNewEM_jvtxf;
   vector<float>   *jet_AntiKt4TopoNewEM_jvtx_x;
   vector<float>   *jet_AntiKt4TopoNewEM_jvtx_y;
   vector<float>   *jet_AntiKt4TopoNewEM_jvtx_z;
   vector<float>   *jet_AntiKt4TopoNewEM_GSCFactorF;
   vector<float>   *jet_AntiKt4TopoNewEM_WidthFraction;
   vector<float>   *jet_AntiKt4TopoNewEM_flavor_weight_Comb;
   vector<float>   *jet_AntiKt4TopoNewEM_flavor_weight_IP2D;
   vector<float>   *jet_AntiKt4TopoNewEM_flavor_weight_IP3D;
   vector<float>   *jet_AntiKt4TopoNewEM_flavor_weight_SV0;
   vector<float>   *jet_AntiKt4TopoNewEM_flavor_weight_SV1;
   vector<float>   *jet_AntiKt4TopoNewEM_flavor_weight_SV2;
   vector<float>   *jet_AntiKt4TopoNewEM_flavor_weight_JetProb;
   vector<float>   *jet_AntiKt4TopoNewEM_flavor_weight_TrackCounting2D;
   vector<float>   *jet_AntiKt4TopoNewEM_flavor_weight_SoftMuonTag;
   vector<float>   *jet_AntiKt4TopoNewEM_flavor_weight_SoftElectronTag;
   vector<float>   *jet_AntiKt4TopoNewEM_flavor_weight_JetFitterTagNN;
   vector<float>   *jet_AntiKt4TopoNewEM_flavor_weight_JetFitterCOMBNN;
   vector<int>     *jet_AntiKt4TopoNewEM_flavor_truth_label;
   vector<float>   *jet_AntiKt4TopoNewEM_flavor_truth_dRminToB;
   vector<float>   *jet_AntiKt4TopoNewEM_flavor_truth_dRminToC;
   vector<float>   *jet_AntiKt4TopoNewEM_flavor_truth_dRminToT;
   vector<int>     *jet_AntiKt4TopoNewEM_flavor_truth_BHadronpdg;
   vector<float>   *jet_AntiKt4TopoNewEM_flavor_truth_vx_x;
   vector<float>   *jet_AntiKt4TopoNewEM_flavor_truth_vx_y;
   vector<float>   *jet_AntiKt4TopoNewEM_flavor_truth_vx_z;
   vector<float>   *jet_AntiKt4TopoNewEM_el_dr;
   vector<int>     *jet_AntiKt4TopoNewEM_el_matched;
   vector<float>   *jet_AntiKt4TopoNewEM_mu_dr;
   vector<int>     *jet_AntiKt4TopoNewEM_mu_matched;
   vector<float>   *jet_AntiKt4TopoNewEM_L1_dr;
   vector<int>     *jet_AntiKt4TopoNewEM_L1_matched;
   vector<float>   *jet_AntiKt4TopoNewEM_L2_dr;
   vector<int>     *jet_AntiKt4TopoNewEM_L2_matched;
   vector<float>   *jet_AntiKt4TopoNewEM_EF_dr;
   vector<int>     *jet_AntiKt4TopoNewEM_EF_matched;
   vector<float>   *jet_AntiKt4TopoNewEM_SmearingFactor;
   Int_t           vx_n;
   vector<float>   *vx_x;
   vector<float>   *vx_y;
   vector<float>   *vx_z;
   vector<float>   *vx_px;
   vector<float>   *vx_py;
   vector<float>   *vx_pz;
   vector<float>   *vx_E;
   vector<float>   *vx_m;
   vector<int>     *vx_nTracks;
   vector<float>   *vx_sumPt;
   vector<int>     *vx_type;
   Int_t           top_hfor_type;
   Int_t           mcevt_n;
   vector<int>     *mcevt_signal_process_id;
   vector<int>     *mcevt_event_number;
   vector<double>  *mcevt_event_scale;
   vector<double>  *mcevt_alphaQCD;
   vector<double>  *mcevt_alphaQED;
   vector<int>     *mcevt_pdf_id1;
   vector<int>     *mcevt_pdf_id2;
   vector<double>  *mcevt_pdf_x1;
   vector<double>  *mcevt_pdf_x2;
   vector<double>  *mcevt_pdf_scale;
   vector<double>  *mcevt_pdf1;
   vector<double>  *mcevt_pdf2;
   vector<vector<double> > *mcevt_weight;
   vector<int>     *mcevt_nparticle;
   vector<short>   *mcevt_pileUpType;
   Int_t           trig_EF_trigmuonef_n;
   vector<int>     *trig_EF_trigmuonef_track_n;
   vector<vector<int> > *trig_EF_trigmuonef_track_MuonType;
   vector<vector<float> > *trig_EF_trigmuonef_track_MS_pt;
   vector<vector<float> > *trig_EF_trigmuonef_track_MS_eta;
   vector<vector<float> > *trig_EF_trigmuonef_track_MS_phi;
   vector<vector<int> > *trig_EF_trigmuonef_track_MS_hasMS;
   vector<vector<float> > *trig_EF_trigmuonef_track_SA_pt;
   vector<vector<float> > *trig_EF_trigmuonef_track_SA_eta;
   vector<vector<float> > *trig_EF_trigmuonef_track_SA_phi;
   vector<vector<int> > *trig_EF_trigmuonef_track_SA_hasSA;
   vector<vector<float> > *trig_EF_trigmuonef_track_CB_pt;
   vector<vector<float> > *trig_EF_trigmuonef_track_CB_eta;
   vector<vector<float> > *trig_EF_trigmuonef_track_CB_phi;
   vector<vector<int> > *trig_EF_trigmuonef_track_CB_hasCB;
   UInt_t          bunch_configID;

   // List of branches
   TBranch        *b_SkimDecision_NTPSUSY_name;   //!
   TBranch        *b_SkimDecision_NTPSUSY_accepted;   //!
   TBranch        *b_SkimDecision_DAODJETS_name;   //!
   TBranch        *b_SkimDecision_DAODJETS_accepted;   //!
   TBranch        *b_SkimDecision_DAODMUONS_name;   //!
   TBranch        *b_SkimDecision_DAODMUONS_accepted;   //!
   TBranch        *b_SkimDecision_DAODEGAMMA_name;   //!
   TBranch        *b_SkimDecision_DAODEGAMMA_accepted;   //!
   TBranch        *b_RunNumber;   //!
   TBranch        *b_EventNumber;   //!
   TBranch        *b_timestamp;   //!
   TBranch        *b_timestamp_ns;   //!
   TBranch        *b_lbn;   //!
   TBranch        *b_bcid;   //!
   TBranch        *b_detmask0;   //!
   TBranch        *b_detmask1;   //!
   TBranch        *b_actualIntPerXing;   //!
   TBranch        *b_averageIntPerXing;   //!
   TBranch        *b_pixelFlags;   //!
   TBranch        *b_sctFlags;   //!
   TBranch        *b_trtFlags;   //!
   TBranch        *b_larFlags;   //!
   TBranch        *b_tileFlags;   //!
   TBranch        *b_muonFlags;   //!
   TBranch        *b_fwdFlags;   //!
   TBranch        *b_coreFlags;   //!
   TBranch        *b_pixelError;   //!
   TBranch        *b_sctError;   //!
   TBranch        *b_trtError;   //!
   TBranch        *b_larError;   //!
   TBranch        *b_tileError;   //!
   TBranch        *b_muonError;   //!
   TBranch        *b_fwdError;   //!
   TBranch        *b_coreError;   //!
   TBranch        *b_el_n;   //!
   TBranch        *b_el_E;   //!
   TBranch        *b_el_Et;   //!
   TBranch        *b_el_pt;   //!
   TBranch        *b_el_m;   //!
   TBranch        *b_el_eta;   //!
   TBranch        *b_el_phi;   //!
   TBranch        *b_el_px;   //!
   TBranch        *b_el_py;   //!
   TBranch        *b_el_pz;   //!
   TBranch        *b_el_charge;   //!
   TBranch        *b_el_author;   //!
   TBranch        *b_el_isEM;   //!
   TBranch        *b_el_OQ;   //!
   TBranch        *b_el_convFlag;   //!
   TBranch        *b_el_isConv;   //!
   TBranch        *b_el_nConv;   //!
   TBranch        *b_el_nSingleTrackConv;   //!
   TBranch        *b_el_nDoubleTrackConv;   //!
   TBranch        *b_el_OQRecalc;   //!
   TBranch        *b_el_truth_E;   //!
   TBranch        *b_el_truth_pt;   //!
   TBranch        *b_el_truth_eta;   //!
   TBranch        *b_el_truth_phi;   //!
   TBranch        *b_el_truth_type;   //!
   TBranch        *b_el_truth_status;   //!
   TBranch        *b_el_truth_barcode;   //!
   TBranch        *b_el_truth_mothertype;   //!
   TBranch        *b_el_truth_motherbarcode;   //!
   TBranch        *b_el_truth_hasHardBrem;   //!
   TBranch        *b_el_truth_matched;   //!
   TBranch        *b_el_loose;   //!
   TBranch        *b_el_medium;   //!
   TBranch        *b_el_mediumIso;   //!
   TBranch        *b_el_tight;   //!
   TBranch        *b_el_tightIso;   //!
   TBranch        *b_el_mediumWithTrack;   //!
   TBranch        *b_el_tightWithTrack;   //!
   TBranch        *b_el_goodOQ;   //!
   TBranch        *b_el_rphiallcalo;   //!
   TBranch        *b_el_Etcone45;   //!
   TBranch        *b_el_Etcone15;   //!
   TBranch        *b_el_Etcone20;   //!
   TBranch        *b_el_Etcone25;   //!
   TBranch        *b_el_Etcone30;   //!
   TBranch        *b_el_Etcone35;   //!
   TBranch        *b_el_Etcone40;   //!
   TBranch        *b_el_ptcone20;   //!
   TBranch        *b_el_ptcone30;   //!
   TBranch        *b_el_ptcone40;   //!
   TBranch        *b_el_nucone20;   //!
   TBranch        *b_el_nucone30;   //!
   TBranch        *b_el_nucone40;   //!
   TBranch        *b_el_pos7;   //!
   TBranch        *b_el_etacorrmag;   //!
   TBranch        *b_el_deltaeta1;   //!
   TBranch        *b_el_deltaeta2;   //!
   TBranch        *b_el_deltaphi2;   //!
   TBranch        *b_el_deltaphiRescaled;   //!
   TBranch        *b_el_deltaPhiRot;   //!
   TBranch        *b_el_expectHitInBLayer;   //!
   TBranch        *b_el_trackd0_physics;   //!
   TBranch        *b_el_reta;   //!
   TBranch        *b_el_rphi;   //!
   TBranch        *b_el_zvertex;   //!
   TBranch        *b_el_errz;   //!
   TBranch        *b_el_etap;   //!
   TBranch        *b_el_depth;   //!
   TBranch        *b_el_refittedTrack_n;   //!
   TBranch        *b_el_cl_E;   //!
   TBranch        *b_el_cl_pt;   //!
   TBranch        *b_el_cl_eta;   //!
   TBranch        *b_el_cl_phi;   //!
   TBranch        *b_el_cl_time;   //!
   TBranch        *b_el_trackd0;   //!
   TBranch        *b_el_trackz0;   //!
   TBranch        *b_el_trackphi;   //!
   TBranch        *b_el_tracktheta;   //!
   TBranch        *b_el_trackqoverp;   //!
   TBranch        *b_el_trackpt;   //!
   TBranch        *b_el_tracketa;   //!
   TBranch        *b_el_nBLHits;   //!
   TBranch        *b_el_nPixHits;   //!
   TBranch        *b_el_nSCTHits;   //!
   TBranch        *b_el_nTRTHits;   //!
   TBranch        *b_el_nTRTHighTHits;   //!
   TBranch        *b_el_nPixHoles;   //!
   TBranch        *b_el_nSCTHoles;   //!
   TBranch        *b_el_nTRTHoles;   //!
   TBranch        *b_el_nBLSharedHits;   //!
   TBranch        *b_el_nPixSharedHits;   //!
   TBranch        *b_el_nSCTSharedHits;   //!
   TBranch        *b_el_nBLayerOutliers;   //!
   TBranch        *b_el_nPixelOutliers;   //!
   TBranch        *b_el_nSCTOutliers;   //!
   TBranch        *b_el_nTRTOutliers;   //!
   TBranch        *b_el_nTRTHighTOutliers;   //!
   TBranch        *b_el_expectBLayerHit;   //!
   TBranch        *b_el_nSiHits;   //!
   TBranch        *b_el_TRTHighTHitsRatio;   //!
   TBranch        *b_el_TRTHighTOutliersRatio;   //!
   TBranch        *b_el_pixeldEdx;   //!
   TBranch        *b_el_nGoodHitsPixeldEdx;   //!
   TBranch        *b_el_massPixeldEdx;   //!
   TBranch        *b_el_likelihoodsPixeldEdx;   //!
   TBranch        *b_el_vertweight;   //!
   TBranch        *b_el_hastrack;   //!
   TBranch        *b_el_Etcone40_ED_corrected;   //!
   TBranch        *b_el_Etcone40_corrected;   //!
   TBranch        *b_el_EF_dr;   //!
   TBranch        *b_EF_2e5_medium;   //!
   TBranch        *b_EF_2j35_jetNoEF_xe40_noMu;   //!
   TBranch        *b_EF_2mu10;   //!
   TBranch        *b_EF_2mu4;   //!
   TBranch        *b_EF_e10_medium;   //!
   TBranch        *b_EF_e15_medium_xe30_noMu;   //!
   TBranch        *b_EF_e15_medium_xe35_noMu;   //!
   TBranch        *b_EF_e15_medium_xe40_noMu_loose;   //!
   TBranch        *b_EF_e15_tight_xe30_noMu;   //!
   TBranch        *b_EF_e20_loose;   //!
   TBranch        *b_EF_e20_loose_xe20;   //!
   TBranch        *b_EF_e20_loose_xe20_noMu;   //!
   TBranch        *b_EF_e20_loose_xe30;   //!
   TBranch        *b_EF_e20_loose_xe30_noMu;   //!
   TBranch        *b_EF_e20_medium;   //!
   TBranch        *b_EF_em105_passHLT;   //!
   TBranch        *b_EF_g20_etcut_xe30_noMu;   //!
   TBranch        *b_EF_g20_loose;   //!
   TBranch        *b_EF_g20_loose_xe20_noMu;   //!
   TBranch        *b_EF_g20_loose_xe30_noMu;   //!
   TBranch        *b_EF_g25_loose_xe30_noMu;   //!
   TBranch        *b_EF_g40_loose_xe40_noMu;   //!
   TBranch        *b_EF_j55_a4tc_EFFS_xe55_medium_noMu_dphi2j30xe10;   //!
   TBranch        *b_EF_j75_a4tc_EFFS_xe40_loose_noMu;   //!
   TBranch        *b_EF_j75_a4tc_EFFS_xe40_loose_noMu_dphijxe03;   //!
   TBranch        *b_EF_j75_a4tc_EFFS_xe45_loose_noMu;   //!
   TBranch        *b_EF_j75_a4tc_EFFS_xe55_loose_noMu;   //!
   TBranch        *b_EF_mu10;   //!
   TBranch        *b_EF_mu13;   //!
   TBranch        *b_EF_mu18;   //!
   TBranch        *b_EF_mu20;   //!
   TBranch        *b_EF_mu20_MSonly;   //!
   TBranch        *b_EF_mu4;   //!
   TBranch        *b_EF_mu6;   //!
   TBranch        *b_EF_tau16_j75_a4tc_EFFS_xe40_loose_noMu;   //!
   TBranch        *b_EF_tau16_loose_xe20_noMu;   //!
   TBranch        *b_EF_tau16_loose_xe25_noMu;   //!
   TBranch        *b_EF_tau16_loose_xe30_noMu;   //!
   TBranch        *b_EF_tau16_medium_xe22_noMu;   //!
   TBranch        *b_EF_tau16_medium_xe25_noMu;   //!
   TBranch        *b_EF_tau16_medium_xe25_tight_noMu;   //!
   TBranch        *b_EF_tau29_medium_xe35_noMu;   //!
   TBranch        *b_EF_tau29_medium_xe40_loose_noMu;   //!
   TBranch        *b_EF_xe20_noMu;   //!
   TBranch        *b_EF_xe30_noMu;   //!
   TBranch        *b_EF_xe35_noMu;   //!
   TBranch        *b_EF_xe40_noMu;   //!
   TBranch        *b_EF_xe50_noMu;   //!
   TBranch        *b_EF_xe60_noMu;   //!
   TBranch        *b_EF_xe70_noMu;   //!
   TBranch        *b_EF_xe80_noMu;   //!
   TBranch        *b_EF_xe90_noMu;   //!
   TBranch        *b_L1_2J15_XE25;   //!
   TBranch        *b_L1_EM10_XE20;   //!
   TBranch        *b_L1_EM14_XE10;   //!
   TBranch        *b_L1_EM14_XE20;   //!
   TBranch        *b_L1_J30_XE35;   //!
   TBranch        *b_L1_J50_XE20;   //!
   TBranch        *b_L1_J50_XE25;   //!
   TBranch        *b_L1_MU0;   //!
   TBranch        *b_L1_MU10;   //!
   TBranch        *b_L1_MU20;   //!
   TBranch        *b_L1_MU6;   //!
   TBranch        *b_L1_TAU11_XE10_3J10;   //!
   TBranch        *b_L1_TAU11_XE20;   //!
   TBranch        *b_L1_TAU15_XE20;   //!
   TBranch        *b_L1_TAU6_J50_XE20;   //!
   TBranch        *b_L1_TAU6_XE10;   //!
   TBranch        *b_L1_XE10;   //!
   TBranch        *b_L1_XE20;   //!
   TBranch        *b_L1_XE25;   //!
   TBranch        *b_L1_XE30;   //!
   TBranch        *b_L1_XE35;   //!
   TBranch        *b_L1_XE40;   //!
   TBranch        *b_L1_XE50;   //!
   TBranch        *b_L1_XE60;   //!
   TBranch        *b_L2_2j35_jetNoEF_xe40_noMu;   //!
   TBranch        *b_L2_2mu4;   //!
   TBranch        *b_L2_e15_medium_xe30_noMu;   //!
   TBranch        *b_L2_e15_medium_xe35_noMu;   //!
   TBranch        *b_L2_e15_medium_xe40_noMu_loose;   //!
   TBranch        *b_L2_e15_tight_xe30_noMu;   //!
   TBranch        *b_L2_e20_loose_xe20;   //!
   TBranch        *b_L2_e20_loose_xe20_noMu;   //!
   TBranch        *b_L2_e20_loose_xe30;   //!
   TBranch        *b_L2_e20_loose_xe30_noMu;   //!
   TBranch        *b_L2_g20_etcut_xe30_noMu;   //!
   TBranch        *b_L2_g20_loose_xe20_noMu;   //!
   TBranch        *b_L2_g20_loose_xe30_noMu;   //!
   TBranch        *b_L2_g25_loose_xe30_noMu;   //!
   TBranch        *b_L2_g40_loose_xe40_noMu;   //!
   TBranch        *b_L2_j50_xe35_medium_noMu;   //!
   TBranch        *b_L2_j70_xe20_loose_noMu;   //!
   TBranch        *b_L2_j70_xe25_loose_noMu;   //!
   TBranch        *b_L2_mu10;   //!
   TBranch        *b_L2_mu13;   //!
   TBranch        *b_L2_mu20;   //!
   TBranch        *b_L2_mu20_MSonly;   //!
   TBranch        *b_L2_mu4;   //!
   TBranch        *b_L2_mu6;   //!
   TBranch        *b_L2_tau16_j70_xe20_loose_noMu;   //!
   TBranch        *b_L2_tau16_loose_xe20_noMu;   //!
   TBranch        *b_L2_tau16_loose_xe25_noMu;   //!
   TBranch        *b_L2_tau16_loose_xe30_noMu;   //!
   TBranch        *b_L2_tau16_medium_xe22_noMu;   //!
   TBranch        *b_L2_tau16_medium_xe25_noMu;   //!
   TBranch        *b_L2_tau16_medium_xe25_tight_noMu;   //!
   TBranch        *b_L2_tau29_medium_xe25_noMu;   //!
   TBranch        *b_L2_tau29_medium_xe30_loose_noMu;   //!
   TBranch        *b_L2_xe10_noMu;   //!
   TBranch        *b_L2_xe20_noMu;   //!
   TBranch        *b_L2_xe25_noMu;   //!
   TBranch        *b_L2_xe30_noMu;   //!
   TBranch        *b_L2_xe35_noMu;   //!
   TBranch        *b_L2_xe40_noMu;   //!
   TBranch        *b_L2_xe50_noMu;   //!
   TBranch        *b_L2_xe60_noMu;   //!
   TBranch        *b_L2_xe70_noMu;   //!
   TBranch        *b_ph_n;   //!
   TBranch        *b_ph_E;   //!
   TBranch        *b_ph_Et;   //!
   TBranch        *b_ph_pt;   //!
   TBranch        *b_ph_m;   //!
   TBranch        *b_ph_eta;   //!
   TBranch        *b_ph_phi;   //!
   TBranch        *b_ph_px;   //!
   TBranch        *b_ph_py;   //!
   TBranch        *b_ph_pz;   //!
   TBranch        *b_ph_author;   //!
   TBranch        *b_ph_isRecovered;   //!
   TBranch        *b_ph_isEM;   //!
   TBranch        *b_ph_OQ;   //!
   TBranch        *b_ph_OQRecalc;   //!
   TBranch        *b_ph_convFlag;   //!
   TBranch        *b_ph_isConv;   //!
   TBranch        *b_ph_nConv;   //!
   TBranch        *b_ph_nSingleTrackConv;   //!
   TBranch        *b_ph_nDoubleTrackConv;   //!
   TBranch        *b_ph_truth_deltaRRecPhoton;   //!
   TBranch        *b_ph_truth_E;   //!
   TBranch        *b_ph_truth_pt;   //!
   TBranch        *b_ph_truth_eta;   //!
   TBranch        *b_ph_truth_phi;   //!
   TBranch        *b_ph_truth_type;   //!
   TBranch        *b_ph_truth_status;   //!
   TBranch        *b_ph_truth_barcode;   //!
   TBranch        *b_ph_truth_mothertype;   //!
   TBranch        *b_ph_truth_motherbarcode;   //!
   TBranch        *b_ph_truth_matched;   //!
   TBranch        *b_ph_loose;   //!
   TBranch        *b_ph_tight;   //!
   TBranch        *b_ph_tightIso;   //!
   TBranch        *b_ph_goodOQ;   //!
   TBranch        *b_ph_rphiallcalo;   //!
   TBranch        *b_ph_Etcone45;   //!
   TBranch        *b_ph_Etcone15;   //!
   TBranch        *b_ph_Etcone20;   //!
   TBranch        *b_ph_Etcone25;   //!
   TBranch        *b_ph_Etcone30;   //!
   TBranch        *b_ph_Etcone35;   //!
   TBranch        *b_ph_Etcone40;   //!
   TBranch        *b_ph_ptcone20;   //!
   TBranch        *b_ph_ptcone30;   //!
   TBranch        *b_ph_ptcone40;   //!
   TBranch        *b_ph_nucone20;   //!
   TBranch        *b_ph_nucone30;   //!
   TBranch        *b_ph_nucone40;   //!
   TBranch        *b_ph_convanglematch;   //!
   TBranch        *b_ph_convtrackmatch;   //!
   TBranch        *b_ph_hasconv;   //!
   TBranch        *b_ph_convvtxx;   //!
   TBranch        *b_ph_convvtxy;   //!
   TBranch        *b_ph_convvtxz;   //!
   TBranch        *b_ph_Rconv;   //!
   TBranch        *b_ph_zconv;   //!
   TBranch        *b_ph_convvtxchi2;   //!
   TBranch        *b_ph_pt1conv;   //!
   TBranch        *b_ph_convtrk1nBLHits;   //!
   TBranch        *b_ph_convtrk1nPixHits;   //!
   TBranch        *b_ph_convtrk1nSCTHits;   //!
   TBranch        *b_ph_convtrk1nTRTHits;   //!
   TBranch        *b_ph_pt2conv;   //!
   TBranch        *b_ph_convtrk2nBLHits;   //!
   TBranch        *b_ph_convtrk2nPixHits;   //!
   TBranch        *b_ph_convtrk2nSCTHits;   //!
   TBranch        *b_ph_convtrk2nTRTHits;   //!
   TBranch        *b_ph_ptconv;   //!
   TBranch        *b_ph_pzconv;   //!
   TBranch        *b_ph_reta;   //!
   TBranch        *b_ph_rphi;   //!
   TBranch        *b_ph_zvertex;   //!
   TBranch        *b_ph_errz;   //!
   TBranch        *b_ph_etap;   //!
   TBranch        *b_ph_depth;   //!
   TBranch        *b_ph_cl_E;   //!
   TBranch        *b_ph_cl_pt;   //!
   TBranch        *b_ph_cl_eta;   //!
   TBranch        *b_ph_cl_phi;   //!
   TBranch        *b_ph_cl_time;   //!
   TBranch        *b_ph_truth_isConv;   //!
   TBranch        *b_ph_truth_isBrem;   //!
   TBranch        *b_ph_truth_isFromHardProc;   //!
   TBranch        *b_ph_truth_isPhotonFromHardProc;   //!
   TBranch        *b_ph_truth_Rconv;   //!
   TBranch        *b_ph_truth_zconv;   //!
   TBranch        *b_ph_deltaEmax2;   //!
   TBranch        *b_ph_jet_dr;   //!
   TBranch        *b_ph_jet_matched;   //!
   TBranch        *b_ph_convIP;   //!
   TBranch        *b_ph_convIPRev;   //!
   TBranch        *b_ph_ptIsolationCone;   //!
   TBranch        *b_ph_ptIsolationConePhAngle;   //!
   TBranch        *b_ph_Etcone40_ED_corrected;   //!
   TBranch        *b_ph_Etcone40_corrected;   //!
   TBranch        *b_mu_staco_n;   //!
   TBranch        *b_mu_staco_E;   //!
   TBranch        *b_mu_staco_pt;   //!
   TBranch        *b_mu_staco_m;   //!
   TBranch        *b_mu_staco_eta;   //!
   TBranch        *b_mu_staco_phi;   //!
   TBranch        *b_mu_staco_px;   //!
   TBranch        *b_mu_staco_py;   //!
   TBranch        *b_mu_staco_pz;   //!
   TBranch        *b_mu_staco_charge;   //!
   TBranch        *b_mu_staco_allauthor;   //!
   TBranch        *b_mu_staco_author;   //!
   TBranch        *b_mu_staco_matchchi2;   //!
   TBranch        *b_mu_staco_matchndof;   //!
   TBranch        *b_mu_staco_etcone20;   //!
   TBranch        *b_mu_staco_etcone30;   //!
   TBranch        *b_mu_staco_etcone40;   //!
   TBranch        *b_mu_staco_nucone20;   //!
   TBranch        *b_mu_staco_nucone30;   //!
   TBranch        *b_mu_staco_nucone40;   //!
   TBranch        *b_mu_staco_ptcone20;   //!
   TBranch        *b_mu_staco_ptcone30;   //!
   TBranch        *b_mu_staco_ptcone40;   //!
   TBranch        *b_mu_staco_energyLossPar;   //!
   TBranch        *b_mu_staco_energyLossErr;   //!
   TBranch        *b_mu_staco_etCore;   //!
   TBranch        *b_mu_staco_energyLossType;   //!
   TBranch        *b_mu_staco_caloMuonIdTag;   //!
   TBranch        *b_mu_staco_caloLRLikelihood;   //!
   TBranch        *b_mu_staco_bestMatch;   //!
   TBranch        *b_mu_staco_isStandAloneMuon;   //!
   TBranch        *b_mu_staco_isCombinedMuon;   //!
   TBranch        *b_mu_staco_isLowPtReconstructedMuon;   //!
   TBranch        *b_mu_staco_isSegmentTaggedMuon;   //!
   TBranch        *b_mu_staco_isCaloMuonId;   //!
   TBranch        *b_mu_staco_alsoFoundByLowPt;   //!
   TBranch        *b_mu_staco_alsoFoundByCaloMuonId;   //!
   TBranch        *b_mu_staco_loose;   //!
   TBranch        *b_mu_staco_medium;   //!
   TBranch        *b_mu_staco_tight;   //!
   TBranch        *b_mu_staco_d0_exPV;   //!
   TBranch        *b_mu_staco_z0_exPV;   //!
   TBranch        *b_mu_staco_phi_exPV;   //!
   TBranch        *b_mu_staco_theta_exPV;   //!
   TBranch        *b_mu_staco_qoverp_exPV;   //!
   TBranch        *b_mu_staco_cb_d0_exPV;   //!
   TBranch        *b_mu_staco_cb_z0_exPV;   //!
   TBranch        *b_mu_staco_cb_phi_exPV;   //!
   TBranch        *b_mu_staco_cb_theta_exPV;   //!
   TBranch        *b_mu_staco_cb_qoverp_exPV;   //!
   TBranch        *b_mu_staco_id_d0_exPV;   //!
   TBranch        *b_mu_staco_id_z0_exPV;   //!
   TBranch        *b_mu_staco_id_phi_exPV;   //!
   TBranch        *b_mu_staco_id_theta_exPV;   //!
   TBranch        *b_mu_staco_id_qoverp_exPV;   //!
   TBranch        *b_mu_staco_me_d0_exPV;   //!
   TBranch        *b_mu_staco_me_z0_exPV;   //!
   TBranch        *b_mu_staco_me_phi_exPV;   //!
   TBranch        *b_mu_staco_me_theta_exPV;   //!
   TBranch        *b_mu_staco_me_qoverp_exPV;   //!
   TBranch        *b_mu_staco_ie_d0_exPV;   //!
   TBranch        *b_mu_staco_ie_z0_exPV;   //!
   TBranch        *b_mu_staco_ie_phi_exPV;   //!
   TBranch        *b_mu_staco_ie_theta_exPV;   //!
   TBranch        *b_mu_staco_ie_qoverp_exPV;   //!
   TBranch        *b_mu_staco_ms_d0;   //!
   TBranch        *b_mu_staco_ms_z0;   //!
   TBranch        *b_mu_staco_ms_phi;   //!
   TBranch        *b_mu_staco_ms_theta;   //!
   TBranch        *b_mu_staco_ms_qoverp;   //!
   TBranch        *b_mu_staco_id_d0;   //!
   TBranch        *b_mu_staco_id_z0;   //!
   TBranch        *b_mu_staco_id_phi;   //!
   TBranch        *b_mu_staco_id_theta;   //!
   TBranch        *b_mu_staco_id_qoverp;   //!
   TBranch        *b_mu_staco_me_d0;   //!
   TBranch        *b_mu_staco_me_z0;   //!
   TBranch        *b_mu_staco_me_phi;   //!
   TBranch        *b_mu_staco_me_theta;   //!
   TBranch        *b_mu_staco_me_qoverp;   //!
   TBranch        *b_mu_staco_ie_d0;   //!
   TBranch        *b_mu_staco_ie_z0;   //!
   TBranch        *b_mu_staco_ie_phi;   //!
   TBranch        *b_mu_staco_ie_theta;   //!
   TBranch        *b_mu_staco_ie_qoverp;   //!
   TBranch        *b_mu_staco_nOutliersOnTrack;   //!
   TBranch        *b_mu_staco_nBLHits;   //!
   TBranch        *b_mu_staco_nPixHits;   //!
   TBranch        *b_mu_staco_nSCTHits;   //!
   TBranch        *b_mu_staco_nTRTHits;   //!
   TBranch        *b_mu_staco_nTRTHighTHits;   //!
   TBranch        *b_mu_staco_nBLSharedHits;   //!
   TBranch        *b_mu_staco_nPixSharedHits;   //!
   TBranch        *b_mu_staco_nPixHoles;   //!
   TBranch        *b_mu_staco_nSCTSharedHits;   //!
   TBranch        *b_mu_staco_nSCTHoles;   //!
   TBranch        *b_mu_staco_nTRTOutliers;   //!
   TBranch        *b_mu_staco_nTRTHighTOutliers;   //!
   TBranch        *b_mu_staco_nGangedPixels;   //!
   TBranch        *b_mu_staco_nPixelDeadSensors;   //!
   TBranch        *b_mu_staco_nSCTDeadSensors;   //!
   TBranch        *b_mu_staco_nTRTDeadStraws;   //!
   TBranch        *b_mu_staco_expectBLayerHit;   //!
   TBranch        *b_mu_staco_nMDTHits;   //!
   TBranch        *b_mu_staco_nMDTHoles;   //!
   TBranch        *b_mu_staco_nCSCEtaHits;   //!
   TBranch        *b_mu_staco_nCSCEtaHoles;   //!
   TBranch        *b_mu_staco_nCSCPhiHits;   //!
   TBranch        *b_mu_staco_nCSCPhiHoles;   //!
   TBranch        *b_mu_staco_nRPCEtaHits;   //!
   TBranch        *b_mu_staco_nRPCEtaHoles;   //!
   TBranch        *b_mu_staco_nRPCPhiHits;   //!
   TBranch        *b_mu_staco_nRPCPhiHoles;   //!
   TBranch        *b_mu_staco_nTGCEtaHits;   //!
   TBranch        *b_mu_staco_nTGCEtaHoles;   //!
   TBranch        *b_mu_staco_nTGCPhiHits;   //!
   TBranch        *b_mu_staco_nTGCPhiHoles;   //!
   TBranch        *b_mu_staco_nMDTBIHits;   //!
   TBranch        *b_mu_staco_nMDTBMHits;   //!
   TBranch        *b_mu_staco_nMDTBOHits;   //!
   TBranch        *b_mu_staco_nMDTBEEHits;   //!
   TBranch        *b_mu_staco_nMDTBIS78Hits;   //!
   TBranch        *b_mu_staco_nMDTEIHits;   //!
   TBranch        *b_mu_staco_nMDTEMHits;   //!
   TBranch        *b_mu_staco_nMDTEOHits;   //!
   TBranch        *b_mu_staco_nMDTEEHits;   //!
   TBranch        *b_mu_staco_nRPCLayer1EtaHits;   //!
   TBranch        *b_mu_staco_nRPCLayer2EtaHits;   //!
   TBranch        *b_mu_staco_nRPCLayer3EtaHits;   //!
   TBranch        *b_mu_staco_nRPCLayer1PhiHits;   //!
   TBranch        *b_mu_staco_nRPCLayer2PhiHits;   //!
   TBranch        *b_mu_staco_nRPCLayer3PhiHits;   //!
   TBranch        *b_mu_staco_nTGCLayer1EtaHits;   //!
   TBranch        *b_mu_staco_nTGCLayer2EtaHits;   //!
   TBranch        *b_mu_staco_nTGCLayer3EtaHits;   //!
   TBranch        *b_mu_staco_nTGCLayer4EtaHits;   //!
   TBranch        *b_mu_staco_nTGCLayer1PhiHits;   //!
   TBranch        *b_mu_staco_nTGCLayer2PhiHits;   //!
   TBranch        *b_mu_staco_nTGCLayer3PhiHits;   //!
   TBranch        *b_mu_staco_nTGCLayer4PhiHits;   //!
   TBranch        *b_mu_staco_barrelSectors;   //!
   TBranch        *b_mu_staco_endcapSectors;   //!
   TBranch        *b_mu_staco_trackd0;   //!
   TBranch        *b_mu_staco_trackz0;   //!
   TBranch        *b_mu_staco_trackphi;   //!
   TBranch        *b_mu_staco_tracktheta;   //!
   TBranch        *b_mu_staco_trackqoverp;   //!
   TBranch        *b_mu_staco_trackcov_d0;   //!
   TBranch        *b_mu_staco_trackcov_z0;   //!
   TBranch        *b_mu_staco_trackcov_phi;   //!
   TBranch        *b_mu_staco_trackcov_theta;   //!
   TBranch        *b_mu_staco_trackcov_qoverp;   //!
   TBranch        *b_mu_staco_trackcov_d0_z0;   //!
   TBranch        *b_mu_staco_trackcov_d0_phi;   //!
   TBranch        *b_mu_staco_trackcov_d0_theta;   //!
   TBranch        *b_mu_staco_trackcov_d0_qoverp;   //!
   TBranch        *b_mu_staco_trackcov_z0_phi;   //!
   TBranch        *b_mu_staco_trackcov_z0_theta;   //!
   TBranch        *b_mu_staco_trackcov_z0_qoverp;   //!
   TBranch        *b_mu_staco_trackcov_phi_theta;   //!
   TBranch        *b_mu_staco_trackcov_phi_qoverp;   //!
   TBranch        *b_mu_staco_trackcov_theta_qoverp;   //!
   TBranch        *b_mu_staco_hastrack;   //!
   TBranch        *b_mu_staco_truth_dr;   //!
   TBranch        *b_mu_staco_truth_E;   //!
   TBranch        *b_mu_staco_truth_pt;   //!
   TBranch        *b_mu_staco_truth_eta;   //!
   TBranch        *b_mu_staco_truth_phi;   //!
   TBranch        *b_mu_staco_truth_type;   //!
   TBranch        *b_mu_staco_truth_status;   //!
   TBranch        *b_mu_staco_truth_barcode;   //!
   TBranch        *b_mu_staco_truth_mothertype;   //!
   TBranch        *b_mu_staco_truth_motherbarcode;   //!
   TBranch        *b_mu_staco_truth_matched;   //!
   TBranch        *b_mu_staco_EFCB_dr;   //!
   TBranch        *b_mu_staco_EFCB_index;   //!
   TBranch        *b_mu_staco_EFMG_dr;   //!
   TBranch        *b_mu_staco_EFME_dr;   //!
   TBranch        *b_mu_staco_EFME_index;   //!
   TBranch        *b_MET_Simplified20_RefGamma_etx;   //!
   TBranch        *b_MET_Simplified20_RefGamma_ety;   //!
   TBranch        *b_MET_Simplified20_RefGamma_sumet;   //!
   TBranch        *b_MET_Simplified20_RefGamma_etx_CentralReg;   //!
   TBranch        *b_MET_Simplified20_RefGamma_ety_CentralReg;   //!
   TBranch        *b_MET_Simplified20_RefGamma_sumet_CentralReg;   //!
   TBranch        *b_MET_Simplified20_RefGamma_phi_CentralReg;   //!
   TBranch        *b_MET_Simplified20_RefGamma_etx_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_RefGamma_ety_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_RefGamma_sumet_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_RefGamma_phi_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_RefGamma_etx_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_RefGamma_ety_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_RefGamma_sumet_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_RefGamma_phi_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_RefFinal_etx;   //!
   TBranch        *b_MET_Simplified20_RefFinal_ety;   //!
   TBranch        *b_MET_Simplified20_RefFinal_sumet;   //!
   TBranch        *b_MET_Simplified20_RefFinal_etx_CentralReg;   //!
   TBranch        *b_MET_Simplified20_RefFinal_ety_CentralReg;   //!
   TBranch        *b_MET_Simplified20_RefFinal_sumet_CentralReg;   //!
   TBranch        *b_MET_Simplified20_RefFinal_phi_CentralReg;   //!
   TBranch        *b_MET_Simplified20_RefFinal_etx_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_RefFinal_ety_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_RefFinal_sumet_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_RefFinal_phi_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_RefFinal_etx_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_RefFinal_ety_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_RefFinal_sumet_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_RefFinal_phi_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_RefJet_etx;   //!
   TBranch        *b_MET_Simplified20_RefJet_ety;   //!
   TBranch        *b_MET_Simplified20_RefJet_sumet;   //!
   TBranch        *b_MET_Simplified20_RefJet_etx_CentralReg;   //!
   TBranch        *b_MET_Simplified20_RefJet_ety_CentralReg;   //!
   TBranch        *b_MET_Simplified20_RefJet_sumet_CentralReg;   //!
   TBranch        *b_MET_Simplified20_RefJet_phi_CentralReg;   //!
   TBranch        *b_MET_Simplified20_RefJet_etx_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_RefJet_ety_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_RefJet_sumet_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_RefJet_phi_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_RefJet_etx_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_RefJet_ety_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_RefJet_sumet_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_RefJet_phi_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_RefEle_etx;   //!
   TBranch        *b_MET_Simplified20_RefEle_ety;   //!
   TBranch        *b_MET_Simplified20_RefEle_sumet;   //!
   TBranch        *b_MET_Simplified20_RefEle_etx_CentralReg;   //!
   TBranch        *b_MET_Simplified20_RefEle_ety_CentralReg;   //!
   TBranch        *b_MET_Simplified20_RefEle_sumet_CentralReg;   //!
   TBranch        *b_MET_Simplified20_RefEle_phi_CentralReg;   //!
   TBranch        *b_MET_Simplified20_RefEle_etx_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_RefEle_ety_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_RefEle_sumet_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_RefEle_phi_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_RefEle_etx_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_RefEle_ety_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_RefEle_sumet_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_RefEle_phi_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_Muon_Total_Staco_etx;   //!
   TBranch        *b_MET_Simplified20_Muon_Total_Staco_ety;   //!
   TBranch        *b_MET_Simplified20_Muon_Total_Staco_sumet;   //!
   TBranch        *b_MET_Simplified20_Muon_Total_Staco_etx_CentralReg;   //!
   TBranch        *b_MET_Simplified20_Muon_Total_Staco_ety_CentralReg;   //!
   TBranch        *b_MET_Simplified20_Muon_Total_Staco_sumet_CentralReg;   //!
   TBranch        *b_MET_Simplified20_Muon_Total_Staco_phi_CentralReg;   //!
   TBranch        *b_MET_Simplified20_Muon_Total_Staco_etx_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_Muon_Total_Staco_ety_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_Muon_Total_Staco_sumet_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_Muon_Total_Staco_phi_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_Muon_Total_Staco_etx_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_Muon_Total_Staco_ety_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_Muon_Total_Staco_sumet_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_Muon_Total_Staco_phi_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_Muon_Isol_Staco_etx;   //!
   TBranch        *b_MET_Simplified20_Muon_Isol_Staco_ety;   //!
   TBranch        *b_MET_Simplified20_Muon_Isol_Staco_sumet;   //!
   TBranch        *b_MET_Simplified20_Muon_Isol_Staco_etx_CentralReg;   //!
   TBranch        *b_MET_Simplified20_Muon_Isol_Staco_ety_CentralReg;   //!
   TBranch        *b_MET_Simplified20_Muon_Isol_Staco_sumet_CentralReg;   //!
   TBranch        *b_MET_Simplified20_Muon_Isol_Staco_phi_CentralReg;   //!
   TBranch        *b_MET_Simplified20_Muon_Isol_Staco_etx_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_Muon_Isol_Staco_ety_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_Muon_Isol_Staco_sumet_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_Muon_Isol_Staco_phi_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_Muon_Isol_Staco_etx_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_Muon_Isol_Staco_ety_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_Muon_Isol_Staco_sumet_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_Muon_Isol_Staco_phi_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_Muon_NonIsol_Staco_etx;   //!
   TBranch        *b_MET_Simplified20_Muon_NonIsol_Staco_ety;   //!
   TBranch        *b_MET_Simplified20_Muon_NonIsol_Staco_sumet;   //!
   TBranch        *b_MET_Simplified20_Muon_NonIsol_Staco_etx_CentralReg;   //!
   TBranch        *b_MET_Simplified20_Muon_NonIsol_Staco_ety_CentralReg;   //!
   TBranch        *b_MET_Simplified20_Muon_NonIsol_Staco_sumet_CentralReg;   //!
   TBranch        *b_MET_Simplified20_Muon_NonIsol_Staco_phi_CentralReg;   //!
   TBranch        *b_MET_Simplified20_Muon_NonIsol_Staco_etx_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_Muon_NonIsol_Staco_ety_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_Muon_NonIsol_Staco_sumet_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_Muon_NonIsol_Staco_phi_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_Muon_NonIsol_Staco_etx_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_Muon_NonIsol_Staco_ety_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_Muon_NonIsol_Staco_sumet_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_Muon_NonIsol_Staco_phi_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_CellOut_etx;   //!
   TBranch        *b_MET_Simplified20_CellOut_ety;   //!
   TBranch        *b_MET_Simplified20_CellOut_sumet;   //!
   TBranch        *b_MET_Simplified20_CellOut_etx_CentralReg;   //!
   TBranch        *b_MET_Simplified20_CellOut_ety_CentralReg;   //!
   TBranch        *b_MET_Simplified20_CellOut_sumet_CentralReg;   //!
   TBranch        *b_MET_Simplified20_CellOut_phi_CentralReg;   //!
   TBranch        *b_MET_Simplified20_CellOut_etx_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_CellOut_ety_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_CellOut_sumet_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_CellOut_phi_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_CellOut_etx_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_CellOut_ety_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_CellOut_sumet_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_CellOut_phi_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_CellOut_Eflow_etx;   //!
   TBranch        *b_MET_Simplified20_CellOut_Eflow_ety;   //!
   TBranch        *b_MET_Simplified20_CellOut_Eflow_sumet;   //!
   TBranch        *b_MET_Simplified20_CellOut_Eflow_etx_CentralReg;   //!
   TBranch        *b_MET_Simplified20_CellOut_Eflow_ety_CentralReg;   //!
   TBranch        *b_MET_Simplified20_CellOut_Eflow_sumet_CentralReg;   //!
   TBranch        *b_MET_Simplified20_CellOut_Eflow_phi_CentralReg;   //!
   TBranch        *b_MET_Simplified20_CellOut_Eflow_etx_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_CellOut_Eflow_ety_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_CellOut_Eflow_sumet_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_CellOut_Eflow_phi_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_CellOut_Eflow_etx_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_CellOut_Eflow_ety_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_CellOut_Eflow_sumet_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_CellOut_Eflow_phi_ForwardReg;   //!
   TBranch        *b_el_MET_Simplified20_n;   //!
   TBranch        *b_el_MET_Simplified20_wpx;   //!
   TBranch        *b_el_MET_Simplified20_wpy;   //!
   TBranch        *b_el_MET_Simplified20_wet;   //!
   TBranch        *b_el_MET_Simplified20_statusWord;   //!
   TBranch        *b_mu_staco_MET_Simplified20_n;   //!
   TBranch        *b_mu_staco_MET_Simplified20_wpx;   //!
   TBranch        *b_mu_staco_MET_Simplified20_wpy;   //!
   TBranch        *b_mu_staco_MET_Simplified20_wet;   //!
   TBranch        *b_mu_staco_MET_Simplified20_statusWord;   //!
   TBranch        *b_mu_muid_MET_Simplified20_n;   //!
   TBranch        *b_mu_muid_MET_Simplified20_wpx;   //!
   TBranch        *b_mu_muid_MET_Simplified20_wpy;   //!
   TBranch        *b_mu_muid_MET_Simplified20_wet;   //!
   TBranch        *b_mu_muid_MET_Simplified20_statusWord;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_MET_Simplified20_n;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_MET_Simplified20_wpx;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_MET_Simplified20_wpy;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_MET_Simplified20_wet;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_MET_Simplified20_statusWord;   //!
   TBranch        *b_el_MET_n;   //!
   TBranch        *b_el_MET_wpx;   //!
   TBranch        *b_el_MET_wpy;   //!
   TBranch        *b_el_MET_wet;   //!
   TBranch        *b_el_MET_statusWord;   //!
   TBranch        *b_ph_MET_n;   //!
   TBranch        *b_ph_MET_wpx;   //!
   TBranch        *b_ph_MET_wpy;   //!
   TBranch        *b_ph_MET_wet;   //!
   TBranch        *b_ph_MET_statusWord;   //!
   TBranch        *b_mu_staco_MET_n;   //!
   TBranch        *b_mu_staco_MET_wpx;   //!
   TBranch        *b_mu_staco_MET_wpy;   //!
   TBranch        *b_mu_staco_MET_wet;   //!
   TBranch        *b_mu_staco_MET_statusWord;   //!
   TBranch        *b_mu_muid_MET_n;   //!
   TBranch        *b_mu_muid_MET_wpx;   //!
   TBranch        *b_mu_muid_MET_wpy;   //!
   TBranch        *b_mu_muid_MET_wet;   //!
   TBranch        *b_mu_muid_MET_statusWord;   //!
   TBranch        *b_tau_MET_n;   //!
   TBranch        *b_tau_MET_wpx;   //!
   TBranch        *b_tau_MET_wpy;   //!
   TBranch        *b_tau_MET_wet;   //!
   TBranch        *b_tau_MET_statusWord;   //!
   TBranch        *b_jet_AntiKt6LCTopo_MET_n;   //!
   TBranch        *b_jet_AntiKt6LCTopo_MET_wpx;   //!
   TBranch        *b_jet_AntiKt6LCTopo_MET_wpy;   //!
   TBranch        *b_jet_AntiKt6LCTopo_MET_wet;   //!
   TBranch        *b_jet_AntiKt6LCTopo_MET_statusWord;   //!
   TBranch        *b_cl_MET_n;   //!
   TBranch        *b_cl_MET_wpx;   //!
   TBranch        *b_cl_MET_wpy;   //!
   TBranch        *b_cl_MET_wet;   //!
   TBranch        *b_cl_MET_statusWord;   //!
   TBranch        *b_trk_MET_n;   //!
   TBranch        *b_trk_MET_wpx;   //!
   TBranch        *b_trk_MET_wpy;   //!
   TBranch        *b_trk_MET_wet;   //!
   TBranch        *b_trk_MET_statusWord;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_n;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_E;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_pt;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_m;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_eta;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_phi;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_EtaOrigin;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_PhiOrigin;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_MOrigin;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_EtaOriginEM;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_PhiOriginEM;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_MOriginEM;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_WIDTH;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_n90;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_Timing;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_LArQuality;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_nTrk;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_sumPtTrk;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_OriginIndex;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_HECQuality;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_NegativeE;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_AverageLArQF;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_YFlip12;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_YFlip23;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_BCH_CORR_CELL;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_BCH_CORR_DOTX;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_BCH_CORR_JET;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_BCH_CORR_JET_FORCELL;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_ENG_BAD_CELLS;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_N_BAD_CELLS;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_N_BAD_CELLS_CORR;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_BAD_CELLS_CORR_E;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_NumTowers;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_SamplingMax;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_fracSamplingMax;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_hecf;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_tgap3f;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_isUgly;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_isBadLoose;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_isBadMedium;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_isBadTight;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_emfrac;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_Offset;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_EMJES;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_EMJES_EtaCorr;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_EMJESnooffset;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_GCWJES;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_GCWJES_EtaCorr;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_CB;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_emscale_E;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_emscale_pt;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_emscale_m;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_emscale_eta;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_emscale_phi;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_jvtxf;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_jvtx_x;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_jvtx_y;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_jvtx_z;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_GSCFactorF;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_WidthFraction;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_weight_Comb;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_weight_IP2D;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_weight_IP3D;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_weight_SV0;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_weight_SV1;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_weight_SV2;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_weight_JetProb;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_weight_TrackCounting2D;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_weight_SoftMuonTag;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_weight_SoftElectronTag;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_weight_JetFitterTagNN;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_weight_JetFitterCOMBNN;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_truth_label;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_truth_dRminToB;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_truth_dRminToC;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_truth_dRminToT;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_truth_BHadronpdg;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_truth_vx_x;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_truth_vx_y;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_truth_vx_z;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_el_dr;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_el_matched;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_mu_dr;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_mu_matched;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_L1_dr;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_L1_matched;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_L2_dr;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_L2_matched;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_EF_dr;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_EF_matched;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_SmearingFactor;   //!
   TBranch        *b_vx_n;   //!
   TBranch        *b_vx_x;   //!
   TBranch        *b_vx_y;   //!
   TBranch        *b_vx_z;   //!
   TBranch        *b_vx_px;   //!
   TBranch        *b_vx_py;   //!
   TBranch        *b_vx_pz;   //!
   TBranch        *b_vx_E;   //!
   TBranch        *b_vx_m;   //!
   TBranch        *b_vx_nTracks;   //!
   TBranch        *b_vx_sumPt;   //!
   TBranch        *b_vx_type;   //!
   TBranch        *b_top_hfor_type;   //!
   TBranch        *b_mcevt_n;   //!
   TBranch        *b_mcevt_signal_process_id;   //!
   TBranch        *b_mcevt_event_number;   //!
   TBranch        *b_mcevt_event_scale;   //!
   TBranch        *b_mcevt_alphaQCD;   //!
   TBranch        *b_mcevt_alphaQED;   //!
   TBranch        *b_mcevt_pdf_id1;   //!
   TBranch        *b_mcevt_pdf_id2;   //!
   TBranch        *b_mcevt_pdf_x1;   //!
   TBranch        *b_mcevt_pdf_x2;   //!
   TBranch        *b_mcevt_pdf_scale;   //!
   TBranch        *b_mcevt_pdf1;   //!
   TBranch        *b_mcevt_pdf2;   //!
   TBranch        *b_mcevt_weight;   //!
   TBranch        *b_mcevt_nparticle;   //!
   TBranch        *b_mcevt_pileUpType;   //!
   TBranch        *b_trig_EF_trigmuonef_n;   //!
   TBranch        *b_trig_EF_trigmuonef_track_n;   //!
   TBranch        *b_trig_EF_trigmuonef_track_MuonType;   //!
   TBranch        *b_trig_EF_trigmuonef_track_MS_pt;   //!
   TBranch        *b_trig_EF_trigmuonef_track_MS_eta;   //!
   TBranch        *b_trig_EF_trigmuonef_track_MS_phi;   //!
   TBranch        *b_trig_EF_trigmuonef_track_MS_hasMS;   //!
   TBranch        *b_trig_EF_trigmuonef_track_SA_pt;   //!
   TBranch        *b_trig_EF_trigmuonef_track_SA_eta;   //!
   TBranch        *b_trig_EF_trigmuonef_track_SA_phi;   //!
   TBranch        *b_trig_EF_trigmuonef_track_SA_hasSA;   //!
   TBranch        *b_trig_EF_trigmuonef_track_CB_pt;   //!
   TBranch        *b_trig_EF_trigmuonef_track_CB_eta;   //!
   TBranch        *b_trig_EF_trigmuonef_track_CB_phi;   //!
   TBranch        *b_trig_EF_trigmuonef_track_CB_hasCB;   //!
   TBranch        *b_bunch_configID;   //!

   Example(TTree *tree=0);
   virtual ~Example();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop(int nfirst=0, int Nentries=-1);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef Example_cxx
Example::Example(TTree *tree)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {

#ifdef SINGLE_TREE
      // The following code should be used if you want this class to access
      // a single tree instead of a chain
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("Memory Directory");
      if (!f) {
         f = new TFile("Memory Directory");
         f->cd("Rint:/");
      }
      tree = (TTree*)gDirectory->Get("qcd");

#else // SINGLE_TREE

      // The following code should be used if you want this class to access a chain
      // of trees.
      TChain * chain = new TChain("qcd","");
      chain->Add("/scratch/jgoodson/mc10_7TeV.105200.T1_McAtNlo_Jimmy.merge.NTUP_JETMET.e598_s933_s946_r2179_r2176_p515_tid333864_00/NTUP_JETMET.333864._000028.root/qcd");
      tree = chain;
#endif // SINGLE_TREE

   }
   Init(tree);

}

Example::~Example()
{
   if (!fChain) return;
   //   delete fChain->GetCurrentFile();
}

Int_t Example::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t Example::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (!fChain->InheritsFrom(TChain::Class()))  return centry;
   TChain *chain = (TChain*)fChain;
   if (chain->GetTreeNumber() != fCurrent) {
      fCurrent = chain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void Example::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normaly not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   SkimDecision_NTPSUSY_name = 0;
   SkimDecision_NTPSUSY_accepted = 0;
   SkimDecision_DAODJETS_name = 0;
   SkimDecision_DAODJETS_accepted = 0;
   SkimDecision_DAODMUONS_name = 0;
   SkimDecision_DAODMUONS_accepted = 0;
   SkimDecision_DAODEGAMMA_name = 0;
   SkimDecision_DAODEGAMMA_accepted = 0;
   el_E = 0;
   el_Et = 0;
   el_pt = 0;
   el_m = 0;
   el_eta = 0;
   el_phi = 0;
   el_px = 0;
   el_py = 0;
   el_pz = 0;
   el_charge = 0;
   el_author = 0;
   el_isEM = 0;
   el_OQ = 0;
   el_convFlag = 0;
   el_isConv = 0;
   el_nConv = 0;
   el_nSingleTrackConv = 0;
   el_nDoubleTrackConv = 0;
   el_OQRecalc = 0;
   el_truth_E = 0;
   el_truth_pt = 0;
   el_truth_eta = 0;
   el_truth_phi = 0;
   el_truth_type = 0;
   el_truth_status = 0;
   el_truth_barcode = 0;
   el_truth_mothertype = 0;
   el_truth_motherbarcode = 0;
   el_truth_hasHardBrem = 0;
   el_truth_matched = 0;
   el_loose = 0;
   el_medium = 0;
   el_mediumIso = 0;
   el_tight = 0;
   el_tightIso = 0;
   el_mediumWithTrack = 0;
   el_tightWithTrack = 0;
   el_goodOQ = 0;
   el_rphiallcalo = 0;
   el_Etcone45 = 0;
   el_Etcone15 = 0;
   el_Etcone20 = 0;
   el_Etcone25 = 0;
   el_Etcone30 = 0;
   el_Etcone35 = 0;
   el_Etcone40 = 0;
   el_ptcone20 = 0;
   el_ptcone30 = 0;
   el_ptcone40 = 0;
   el_nucone20 = 0;
   el_nucone30 = 0;
   el_nucone40 = 0;
   el_pos7 = 0;
   el_etacorrmag = 0;
   el_deltaeta1 = 0;
   el_deltaeta2 = 0;
   el_deltaphi2 = 0;
   el_deltaphiRescaled = 0;
   el_deltaPhiRot = 0;
   el_expectHitInBLayer = 0;
   el_trackd0_physics = 0;
   el_reta = 0;
   el_rphi = 0;
   el_zvertex = 0;
   el_errz = 0;
   el_etap = 0;
   el_depth = 0;
   el_refittedTrack_n = 0;
   el_cl_E = 0;
   el_cl_pt = 0;
   el_cl_eta = 0;
   el_cl_phi = 0;
   el_cl_time = 0;
   el_trackd0 = 0;
   el_trackz0 = 0;
   el_trackphi = 0;
   el_tracktheta = 0;
   el_trackqoverp = 0;
   el_trackpt = 0;
   el_tracketa = 0;
   el_nBLHits = 0;
   el_nPixHits = 0;
   el_nSCTHits = 0;
   el_nTRTHits = 0;
   el_nTRTHighTHits = 0;
   el_nPixHoles = 0;
   el_nSCTHoles = 0;
   el_nTRTHoles = 0;
   el_nBLSharedHits = 0;
   el_nPixSharedHits = 0;
   el_nSCTSharedHits = 0;
   el_nBLayerOutliers = 0;
   el_nPixelOutliers = 0;
   el_nSCTOutliers = 0;
   el_nTRTOutliers = 0;
   el_nTRTHighTOutliers = 0;
   el_expectBLayerHit = 0;
   el_nSiHits = 0;
   el_TRTHighTHitsRatio = 0;
   el_TRTHighTOutliersRatio = 0;
   el_pixeldEdx = 0;
   el_nGoodHitsPixeldEdx = 0;
   el_massPixeldEdx = 0;
   el_likelihoodsPixeldEdx = 0;
   el_vertweight = 0;
   el_hastrack = 0;
   el_Etcone40_ED_corrected = 0;
   el_Etcone40_corrected = 0;
   el_EF_dr = 0;
   ph_E = 0;
   ph_Et = 0;
   ph_pt = 0;
   ph_m = 0;
   ph_eta = 0;
   ph_phi = 0;
   ph_px = 0;
   ph_py = 0;
   ph_pz = 0;
   ph_author = 0;
   ph_isRecovered = 0;
   ph_isEM = 0;
   ph_OQ = 0;
   ph_OQRecalc = 0;
   ph_convFlag = 0;
   ph_isConv = 0;
   ph_nConv = 0;
   ph_nSingleTrackConv = 0;
   ph_nDoubleTrackConv = 0;
   ph_truth_deltaRRecPhoton = 0;
   ph_truth_E = 0;
   ph_truth_pt = 0;
   ph_truth_eta = 0;
   ph_truth_phi = 0;
   ph_truth_type = 0;
   ph_truth_status = 0;
   ph_truth_barcode = 0;
   ph_truth_mothertype = 0;
   ph_truth_motherbarcode = 0;
   ph_truth_matched = 0;
   ph_loose = 0;
   ph_tight = 0;
   ph_tightIso = 0;
   ph_goodOQ = 0;
   ph_rphiallcalo = 0;
   ph_Etcone45 = 0;
   ph_Etcone15 = 0;
   ph_Etcone20 = 0;
   ph_Etcone25 = 0;
   ph_Etcone30 = 0;
   ph_Etcone35 = 0;
   ph_Etcone40 = 0;
   ph_ptcone20 = 0;
   ph_ptcone30 = 0;
   ph_ptcone40 = 0;
   ph_nucone20 = 0;
   ph_nucone30 = 0;
   ph_nucone40 = 0;
   ph_convanglematch = 0;
   ph_convtrackmatch = 0;
   ph_hasconv = 0;
   ph_convvtxx = 0;
   ph_convvtxy = 0;
   ph_convvtxz = 0;
   ph_Rconv = 0;
   ph_zconv = 0;
   ph_convvtxchi2 = 0;
   ph_pt1conv = 0;
   ph_convtrk1nBLHits = 0;
   ph_convtrk1nPixHits = 0;
   ph_convtrk1nSCTHits = 0;
   ph_convtrk1nTRTHits = 0;
   ph_pt2conv = 0;
   ph_convtrk2nBLHits = 0;
   ph_convtrk2nPixHits = 0;
   ph_convtrk2nSCTHits = 0;
   ph_convtrk2nTRTHits = 0;
   ph_ptconv = 0;
   ph_pzconv = 0;
   ph_reta = 0;
   ph_rphi = 0;
   ph_zvertex = 0;
   ph_errz = 0;
   ph_etap = 0;
   ph_depth = 0;
   ph_cl_E = 0;
   ph_cl_pt = 0;
   ph_cl_eta = 0;
   ph_cl_phi = 0;
   ph_cl_time = 0;
   ph_truth_isConv = 0;
   ph_truth_isBrem = 0;
   ph_truth_isFromHardProc = 0;
   ph_truth_isPhotonFromHardProc = 0;
   ph_truth_Rconv = 0;
   ph_truth_zconv = 0;
   ph_deltaEmax2 = 0;
   ph_jet_dr = 0;
   ph_jet_matched = 0;
   ph_convIP = 0;
   ph_convIPRev = 0;
   ph_ptIsolationCone = 0;
   ph_ptIsolationConePhAngle = 0;
   ph_Etcone40_ED_corrected = 0;
   ph_Etcone40_corrected = 0;
   mu_staco_E = 0;
   mu_staco_pt = 0;
   mu_staco_m = 0;
   mu_staco_eta = 0;
   mu_staco_phi = 0;
   mu_staco_px = 0;
   mu_staco_py = 0;
   mu_staco_pz = 0;
   mu_staco_charge = 0;
   mu_staco_allauthor = 0;
   mu_staco_author = 0;
   mu_staco_matchchi2 = 0;
   mu_staco_matchndof = 0;
   mu_staco_etcone20 = 0;
   mu_staco_etcone30 = 0;
   mu_staco_etcone40 = 0;
   mu_staco_nucone20 = 0;
   mu_staco_nucone30 = 0;
   mu_staco_nucone40 = 0;
   mu_staco_ptcone20 = 0;
   mu_staco_ptcone30 = 0;
   mu_staco_ptcone40 = 0;
   mu_staco_energyLossPar = 0;
   mu_staco_energyLossErr = 0;
   mu_staco_etCore = 0;
   mu_staco_energyLossType = 0;
   mu_staco_caloMuonIdTag = 0;
   mu_staco_caloLRLikelihood = 0;
   mu_staco_bestMatch = 0;
   mu_staco_isStandAloneMuon = 0;
   mu_staco_isCombinedMuon = 0;
   mu_staco_isLowPtReconstructedMuon = 0;
   mu_staco_isSegmentTaggedMuon = 0;
   mu_staco_isCaloMuonId = 0;
   mu_staco_alsoFoundByLowPt = 0;
   mu_staco_alsoFoundByCaloMuonId = 0;
   mu_staco_loose = 0;
   mu_staco_medium = 0;
   mu_staco_tight = 0;
   mu_staco_d0_exPV = 0;
   mu_staco_z0_exPV = 0;
   mu_staco_phi_exPV = 0;
   mu_staco_theta_exPV = 0;
   mu_staco_qoverp_exPV = 0;
   mu_staco_cb_d0_exPV = 0;
   mu_staco_cb_z0_exPV = 0;
   mu_staco_cb_phi_exPV = 0;
   mu_staco_cb_theta_exPV = 0;
   mu_staco_cb_qoverp_exPV = 0;
   mu_staco_id_d0_exPV = 0;
   mu_staco_id_z0_exPV = 0;
   mu_staco_id_phi_exPV = 0;
   mu_staco_id_theta_exPV = 0;
   mu_staco_id_qoverp_exPV = 0;
   mu_staco_me_d0_exPV = 0;
   mu_staco_me_z0_exPV = 0;
   mu_staco_me_phi_exPV = 0;
   mu_staco_me_theta_exPV = 0;
   mu_staco_me_qoverp_exPV = 0;
   mu_staco_ie_d0_exPV = 0;
   mu_staco_ie_z0_exPV = 0;
   mu_staco_ie_phi_exPV = 0;
   mu_staco_ie_theta_exPV = 0;
   mu_staco_ie_qoverp_exPV = 0;
   mu_staco_ms_d0 = 0;
   mu_staco_ms_z0 = 0;
   mu_staco_ms_phi = 0;
   mu_staco_ms_theta = 0;
   mu_staco_ms_qoverp = 0;
   mu_staco_id_d0 = 0;
   mu_staco_id_z0 = 0;
   mu_staco_id_phi = 0;
   mu_staco_id_theta = 0;
   mu_staco_id_qoverp = 0;
   mu_staco_me_d0 = 0;
   mu_staco_me_z0 = 0;
   mu_staco_me_phi = 0;
   mu_staco_me_theta = 0;
   mu_staco_me_qoverp = 0;
   mu_staco_ie_d0 = 0;
   mu_staco_ie_z0 = 0;
   mu_staco_ie_phi = 0;
   mu_staco_ie_theta = 0;
   mu_staco_ie_qoverp = 0;
   mu_staco_nOutliersOnTrack = 0;
   mu_staco_nBLHits = 0;
   mu_staco_nPixHits = 0;
   mu_staco_nSCTHits = 0;
   mu_staco_nTRTHits = 0;
   mu_staco_nTRTHighTHits = 0;
   mu_staco_nBLSharedHits = 0;
   mu_staco_nPixSharedHits = 0;
   mu_staco_nPixHoles = 0;
   mu_staco_nSCTSharedHits = 0;
   mu_staco_nSCTHoles = 0;
   mu_staco_nTRTOutliers = 0;
   mu_staco_nTRTHighTOutliers = 0;
   mu_staco_nGangedPixels = 0;
   mu_staco_nPixelDeadSensors = 0;
   mu_staco_nSCTDeadSensors = 0;
   mu_staco_nTRTDeadStraws = 0;
   mu_staco_expectBLayerHit = 0;
   mu_staco_nMDTHits = 0;
   mu_staco_nMDTHoles = 0;
   mu_staco_nCSCEtaHits = 0;
   mu_staco_nCSCEtaHoles = 0;
   mu_staco_nCSCPhiHits = 0;
   mu_staco_nCSCPhiHoles = 0;
   mu_staco_nRPCEtaHits = 0;
   mu_staco_nRPCEtaHoles = 0;
   mu_staco_nRPCPhiHits = 0;
   mu_staco_nRPCPhiHoles = 0;
   mu_staco_nTGCEtaHits = 0;
   mu_staco_nTGCEtaHoles = 0;
   mu_staco_nTGCPhiHits = 0;
   mu_staco_nTGCPhiHoles = 0;
   mu_staco_nMDTBIHits = 0;
   mu_staco_nMDTBMHits = 0;
   mu_staco_nMDTBOHits = 0;
   mu_staco_nMDTBEEHits = 0;
   mu_staco_nMDTBIS78Hits = 0;
   mu_staco_nMDTEIHits = 0;
   mu_staco_nMDTEMHits = 0;
   mu_staco_nMDTEOHits = 0;
   mu_staco_nMDTEEHits = 0;
   mu_staco_nRPCLayer1EtaHits = 0;
   mu_staco_nRPCLayer2EtaHits = 0;
   mu_staco_nRPCLayer3EtaHits = 0;
   mu_staco_nRPCLayer1PhiHits = 0;
   mu_staco_nRPCLayer2PhiHits = 0;
   mu_staco_nRPCLayer3PhiHits = 0;
   mu_staco_nTGCLayer1EtaHits = 0;
   mu_staco_nTGCLayer2EtaHits = 0;
   mu_staco_nTGCLayer3EtaHits = 0;
   mu_staco_nTGCLayer4EtaHits = 0;
   mu_staco_nTGCLayer1PhiHits = 0;
   mu_staco_nTGCLayer2PhiHits = 0;
   mu_staco_nTGCLayer3PhiHits = 0;
   mu_staco_nTGCLayer4PhiHits = 0;
   mu_staco_barrelSectors = 0;
   mu_staco_endcapSectors = 0;
   mu_staco_trackd0 = 0;
   mu_staco_trackz0 = 0;
   mu_staco_trackphi = 0;
   mu_staco_tracktheta = 0;
   mu_staco_trackqoverp = 0;
   mu_staco_trackcov_d0 = 0;
   mu_staco_trackcov_z0 = 0;
   mu_staco_trackcov_phi = 0;
   mu_staco_trackcov_theta = 0;
   mu_staco_trackcov_qoverp = 0;
   mu_staco_trackcov_d0_z0 = 0;
   mu_staco_trackcov_d0_phi = 0;
   mu_staco_trackcov_d0_theta = 0;
   mu_staco_trackcov_d0_qoverp = 0;
   mu_staco_trackcov_z0_phi = 0;
   mu_staco_trackcov_z0_theta = 0;
   mu_staco_trackcov_z0_qoverp = 0;
   mu_staco_trackcov_phi_theta = 0;
   mu_staco_trackcov_phi_qoverp = 0;
   mu_staco_trackcov_theta_qoverp = 0;
   mu_staco_hastrack = 0;
   mu_staco_truth_dr = 0;
   mu_staco_truth_E = 0;
   mu_staco_truth_pt = 0;
   mu_staco_truth_eta = 0;
   mu_staco_truth_phi = 0;
   mu_staco_truth_type = 0;
   mu_staco_truth_status = 0;
   mu_staco_truth_barcode = 0;
   mu_staco_truth_mothertype = 0;
   mu_staco_truth_motherbarcode = 0;
   mu_staco_truth_matched = 0;
   mu_staco_EFCB_dr = 0;
   mu_staco_EFCB_index = 0;
   mu_staco_EFMG_dr = 0;
   mu_staco_EFME_dr = 0;
   mu_staco_EFME_index = 0;
   el_MET_Simplified20_wpx = 0;
   el_MET_Simplified20_wpy = 0;
   el_MET_Simplified20_wet = 0;
   el_MET_Simplified20_statusWord = 0;
   mu_staco_MET_Simplified20_wpx = 0;
   mu_staco_MET_Simplified20_wpy = 0;
   mu_staco_MET_Simplified20_wet = 0;
   mu_staco_MET_Simplified20_statusWord = 0;
   mu_muid_MET_Simplified20_wpx = 0;
   mu_muid_MET_Simplified20_wpy = 0;
   mu_muid_MET_Simplified20_wet = 0;
   mu_muid_MET_Simplified20_statusWord = 0;
   jet_AntiKt4TopoNewEM_MET_Simplified20_wpx = 0;
   jet_AntiKt4TopoNewEM_MET_Simplified20_wpy = 0;
   jet_AntiKt4TopoNewEM_MET_Simplified20_wet = 0;
   jet_AntiKt4TopoNewEM_MET_Simplified20_statusWord = 0;
   el_MET_wpx = 0;
   el_MET_wpy = 0;
   el_MET_wet = 0;
   el_MET_statusWord = 0;
   ph_MET_wpx = 0;
   ph_MET_wpy = 0;
   ph_MET_wet = 0;
   ph_MET_statusWord = 0;
   mu_staco_MET_wpx = 0;
   mu_staco_MET_wpy = 0;
   mu_staco_MET_wet = 0;
   mu_staco_MET_statusWord = 0;
   mu_muid_MET_wpx = 0;
   mu_muid_MET_wpy = 0;
   mu_muid_MET_wet = 0;
   mu_muid_MET_statusWord = 0;
   tau_MET_wpx = 0;
   tau_MET_wpy = 0;
   tau_MET_wet = 0;
   tau_MET_statusWord = 0;
   jet_AntiKt6LCTopo_MET_wpx = 0;
   jet_AntiKt6LCTopo_MET_wpy = 0;
   jet_AntiKt6LCTopo_MET_wet = 0;
   jet_AntiKt6LCTopo_MET_statusWord = 0;
   cl_MET_wpx = 0;
   cl_MET_wpy = 0;
   cl_MET_wet = 0;
   cl_MET_statusWord = 0;
   trk_MET_wpx = 0;
   trk_MET_wpy = 0;
   trk_MET_wet = 0;
   trk_MET_statusWord = 0;
   jet_AntiKt4TopoNewEM_E = 0;
   jet_AntiKt4TopoNewEM_pt = 0;
   jet_AntiKt4TopoNewEM_m = 0;
   jet_AntiKt4TopoNewEM_eta = 0;
   jet_AntiKt4TopoNewEM_phi = 0;
   jet_AntiKt4TopoNewEM_EtaOrigin = 0;
   jet_AntiKt4TopoNewEM_PhiOrigin = 0;
   jet_AntiKt4TopoNewEM_MOrigin = 0;
   jet_AntiKt4TopoNewEM_EtaOriginEM = 0;
   jet_AntiKt4TopoNewEM_PhiOriginEM = 0;
   jet_AntiKt4TopoNewEM_MOriginEM = 0;
   jet_AntiKt4TopoNewEM_WIDTH = 0;
   jet_AntiKt4TopoNewEM_n90 = 0;
   jet_AntiKt4TopoNewEM_Timing = 0;
   jet_AntiKt4TopoNewEM_LArQuality = 0;
   jet_AntiKt4TopoNewEM_nTrk = 0;
   jet_AntiKt4TopoNewEM_sumPtTrk = 0;
   jet_AntiKt4TopoNewEM_OriginIndex = 0;
   jet_AntiKt4TopoNewEM_HECQuality = 0;
   jet_AntiKt4TopoNewEM_NegativeE = 0;
   jet_AntiKt4TopoNewEM_AverageLArQF = 0;
   jet_AntiKt4TopoNewEM_YFlip12 = 0;
   jet_AntiKt4TopoNewEM_YFlip23 = 0;
   jet_AntiKt4TopoNewEM_BCH_CORR_CELL = 0;
   jet_AntiKt4TopoNewEM_BCH_CORR_DOTX = 0;
   jet_AntiKt4TopoNewEM_BCH_CORR_JET = 0;
   jet_AntiKt4TopoNewEM_BCH_CORR_JET_FORCELL = 0;
   jet_AntiKt4TopoNewEM_ENG_BAD_CELLS = 0;
   jet_AntiKt4TopoNewEM_N_BAD_CELLS = 0;
   jet_AntiKt4TopoNewEM_N_BAD_CELLS_CORR = 0;
   jet_AntiKt4TopoNewEM_BAD_CELLS_CORR_E = 0;
   jet_AntiKt4TopoNewEM_NumTowers = 0;
   jet_AntiKt4TopoNewEM_SamplingMax = 0;
   jet_AntiKt4TopoNewEM_fracSamplingMax = 0;
   jet_AntiKt4TopoNewEM_hecf = 0;
   jet_AntiKt4TopoNewEM_tgap3f = 0;
   jet_AntiKt4TopoNewEM_isUgly = 0;
   jet_AntiKt4TopoNewEM_isBadLoose = 0;
   jet_AntiKt4TopoNewEM_isBadMedium = 0;
   jet_AntiKt4TopoNewEM_isBadTight = 0;
   jet_AntiKt4TopoNewEM_emfrac = 0;
   jet_AntiKt4TopoNewEM_Offset = 0;
   jet_AntiKt4TopoNewEM_EMJES = 0;
   jet_AntiKt4TopoNewEM_EMJES_EtaCorr = 0;
   jet_AntiKt4TopoNewEM_EMJESnooffset = 0;
   jet_AntiKt4TopoNewEM_GCWJES = 0;
   jet_AntiKt4TopoNewEM_GCWJES_EtaCorr = 0;
   jet_AntiKt4TopoNewEM_CB = 0;
   jet_AntiKt4TopoNewEM_emscale_E = 0;
   jet_AntiKt4TopoNewEM_emscale_pt = 0;
   jet_AntiKt4TopoNewEM_emscale_m = 0;
   jet_AntiKt4TopoNewEM_emscale_eta = 0;
   jet_AntiKt4TopoNewEM_emscale_phi = 0;
   jet_AntiKt4TopoNewEM_jvtxf = 0;
   jet_AntiKt4TopoNewEM_jvtx_x = 0;
   jet_AntiKt4TopoNewEM_jvtx_y = 0;
   jet_AntiKt4TopoNewEM_jvtx_z = 0;
   jet_AntiKt4TopoNewEM_GSCFactorF = 0;
   jet_AntiKt4TopoNewEM_WidthFraction = 0;
   jet_AntiKt4TopoNewEM_flavor_weight_Comb = 0;
   jet_AntiKt4TopoNewEM_flavor_weight_IP2D = 0;
   jet_AntiKt4TopoNewEM_flavor_weight_IP3D = 0;
   jet_AntiKt4TopoNewEM_flavor_weight_SV0 = 0;
   jet_AntiKt4TopoNewEM_flavor_weight_SV1 = 0;
   jet_AntiKt4TopoNewEM_flavor_weight_SV2 = 0;
   jet_AntiKt4TopoNewEM_flavor_weight_JetProb = 0;
   jet_AntiKt4TopoNewEM_flavor_weight_TrackCounting2D = 0;
   jet_AntiKt4TopoNewEM_flavor_weight_SoftMuonTag = 0;
   jet_AntiKt4TopoNewEM_flavor_weight_SoftElectronTag = 0;
   jet_AntiKt4TopoNewEM_flavor_weight_JetFitterTagNN = 0;
   jet_AntiKt4TopoNewEM_flavor_weight_JetFitterCOMBNN = 0;
   jet_AntiKt4TopoNewEM_flavor_truth_label = 0;
   jet_AntiKt4TopoNewEM_flavor_truth_dRminToB = 0;
   jet_AntiKt4TopoNewEM_flavor_truth_dRminToC = 0;
   jet_AntiKt4TopoNewEM_flavor_truth_dRminToT = 0;
   jet_AntiKt4TopoNewEM_flavor_truth_BHadronpdg = 0;
   jet_AntiKt4TopoNewEM_flavor_truth_vx_x = 0;
   jet_AntiKt4TopoNewEM_flavor_truth_vx_y = 0;
   jet_AntiKt4TopoNewEM_flavor_truth_vx_z = 0;
   jet_AntiKt4TopoNewEM_el_dr = 0;
   jet_AntiKt4TopoNewEM_el_matched = 0;
   jet_AntiKt4TopoNewEM_mu_dr = 0;
   jet_AntiKt4TopoNewEM_mu_matched = 0;
   jet_AntiKt4TopoNewEM_L1_dr = 0;
   jet_AntiKt4TopoNewEM_L1_matched = 0;
   jet_AntiKt4TopoNewEM_L2_dr = 0;
   jet_AntiKt4TopoNewEM_L2_matched = 0;
   jet_AntiKt4TopoNewEM_EF_dr = 0;
   jet_AntiKt4TopoNewEM_EF_matched = 0;
   jet_AntiKt4TopoNewEM_SmearingFactor = 0;
   vx_x = 0;
   vx_y = 0;
   vx_z = 0;
   vx_px = 0;
   vx_py = 0;
   vx_pz = 0;
   vx_E = 0;
   vx_m = 0;
   vx_nTracks = 0;
   vx_sumPt = 0;
   vx_type = 0;
   mcevt_signal_process_id = 0;
   mcevt_event_number = 0;
   mcevt_event_scale = 0;
   mcevt_alphaQCD = 0;
   mcevt_alphaQED = 0;
   mcevt_pdf_id1 = 0;
   mcevt_pdf_id2 = 0;
   mcevt_pdf_x1 = 0;
   mcevt_pdf_x2 = 0;
   mcevt_pdf_scale = 0;
   mcevt_pdf1 = 0;
   mcevt_pdf2 = 0;
   mcevt_weight = 0;
   mcevt_nparticle = 0;
   mcevt_pileUpType = 0;
   trig_EF_trigmuonef_track_n = 0;
   trig_EF_trigmuonef_track_MuonType = 0;
   trig_EF_trigmuonef_track_MS_pt = 0;
   trig_EF_trigmuonef_track_MS_eta = 0;
   trig_EF_trigmuonef_track_MS_phi = 0;
   trig_EF_trigmuonef_track_MS_hasMS = 0;
   trig_EF_trigmuonef_track_SA_pt = 0;
   trig_EF_trigmuonef_track_SA_eta = 0;
   trig_EF_trigmuonef_track_SA_phi = 0;
   trig_EF_trigmuonef_track_SA_hasSA = 0;
   trig_EF_trigmuonef_track_CB_pt = 0;
   trig_EF_trigmuonef_track_CB_eta = 0;
   trig_EF_trigmuonef_track_CB_phi = 0;
   trig_EF_trigmuonef_track_CB_hasCB = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("SkimDecision_NTPSUSY_name", &SkimDecision_NTPSUSY_name, &b_SkimDecision_NTPSUSY_name);
   fChain->SetBranchAddress("SkimDecision_NTPSUSY_accepted", &SkimDecision_NTPSUSY_accepted, &b_SkimDecision_NTPSUSY_accepted);
   fChain->SetBranchAddress("SkimDecision_DAODJETS_name", &SkimDecision_DAODJETS_name, &b_SkimDecision_DAODJETS_name);
   fChain->SetBranchAddress("SkimDecision_DAODJETS_accepted", &SkimDecision_DAODJETS_accepted, &b_SkimDecision_DAODJETS_accepted);
   fChain->SetBranchAddress("SkimDecision_DAODMUONS_name", &SkimDecision_DAODMUONS_name, &b_SkimDecision_DAODMUONS_name);
   fChain->SetBranchAddress("SkimDecision_DAODMUONS_accepted", &SkimDecision_DAODMUONS_accepted, &b_SkimDecision_DAODMUONS_accepted);
   fChain->SetBranchAddress("SkimDecision_DAODEGAMMA_name", &SkimDecision_DAODEGAMMA_name, &b_SkimDecision_DAODEGAMMA_name);
   fChain->SetBranchAddress("SkimDecision_DAODEGAMMA_accepted", &SkimDecision_DAODEGAMMA_accepted, &b_SkimDecision_DAODEGAMMA_accepted);
   fChain->SetBranchAddress("RunNumber", &RunNumber, &b_RunNumber);
   fChain->SetBranchAddress("EventNumber", &EventNumber, &b_EventNumber);
   fChain->SetBranchAddress("timestamp", &timestamp, &b_timestamp);
   fChain->SetBranchAddress("timestamp_ns", &timestamp_ns, &b_timestamp_ns);
   fChain->SetBranchAddress("lbn", &lbn, &b_lbn);
   fChain->SetBranchAddress("bcid", &bcid, &b_bcid);
   fChain->SetBranchAddress("detmask0", &detmask0, &b_detmask0);
   fChain->SetBranchAddress("detmask1", &detmask1, &b_detmask1);
   fChain->SetBranchAddress("actualIntPerXing", &actualIntPerXing, &b_actualIntPerXing);
   fChain->SetBranchAddress("averageIntPerXing", &averageIntPerXing, &b_averageIntPerXing);
   fChain->SetBranchAddress("pixelFlags", &pixelFlags, &b_pixelFlags);
   fChain->SetBranchAddress("sctFlags", &sctFlags, &b_sctFlags);
   fChain->SetBranchAddress("trtFlags", &trtFlags, &b_trtFlags);
   fChain->SetBranchAddress("larFlags", &larFlags, &b_larFlags);
   fChain->SetBranchAddress("tileFlags", &tileFlags, &b_tileFlags);
   fChain->SetBranchAddress("muonFlags", &muonFlags, &b_muonFlags);
   fChain->SetBranchAddress("fwdFlags", &fwdFlags, &b_fwdFlags);
   fChain->SetBranchAddress("coreFlags", &coreFlags, &b_coreFlags);
   fChain->SetBranchAddress("pixelError", &pixelError, &b_pixelError);
   fChain->SetBranchAddress("sctError", &sctError, &b_sctError);
   fChain->SetBranchAddress("trtError", &trtError, &b_trtError);
   fChain->SetBranchAddress("larError", &larError, &b_larError);
   fChain->SetBranchAddress("tileError", &tileError, &b_tileError);
   fChain->SetBranchAddress("muonError", &muonError, &b_muonError);
   fChain->SetBranchAddress("fwdError", &fwdError, &b_fwdError);
   fChain->SetBranchAddress("coreError", &coreError, &b_coreError);
   fChain->SetBranchAddress("el_n", &el_n, &b_el_n);
   fChain->SetBranchAddress("el_E", &el_E, &b_el_E);
   fChain->SetBranchAddress("el_Et", &el_Et, &b_el_Et);
   fChain->SetBranchAddress("el_pt", &el_pt, &b_el_pt);
   fChain->SetBranchAddress("el_m", &el_m, &b_el_m);
   fChain->SetBranchAddress("el_eta", &el_eta, &b_el_eta);
   fChain->SetBranchAddress("el_phi", &el_phi, &b_el_phi);
   fChain->SetBranchAddress("el_px", &el_px, &b_el_px);
   fChain->SetBranchAddress("el_py", &el_py, &b_el_py);
   fChain->SetBranchAddress("el_pz", &el_pz, &b_el_pz);
   fChain->SetBranchAddress("el_charge", &el_charge, &b_el_charge);
   fChain->SetBranchAddress("el_author", &el_author, &b_el_author);
   fChain->SetBranchAddress("el_isEM", &el_isEM, &b_el_isEM);
   fChain->SetBranchAddress("el_OQ", &el_OQ, &b_el_OQ);
   fChain->SetBranchAddress("el_convFlag", &el_convFlag, &b_el_convFlag);
   fChain->SetBranchAddress("el_isConv", &el_isConv, &b_el_isConv);
   fChain->SetBranchAddress("el_nConv", &el_nConv, &b_el_nConv);
   fChain->SetBranchAddress("el_nSingleTrackConv", &el_nSingleTrackConv, &b_el_nSingleTrackConv);
   fChain->SetBranchAddress("el_nDoubleTrackConv", &el_nDoubleTrackConv, &b_el_nDoubleTrackConv);
   fChain->SetBranchAddress("el_OQRecalc", &el_OQRecalc, &b_el_OQRecalc);
   fChain->SetBranchAddress("el_truth_E", &el_truth_E, &b_el_truth_E);
   fChain->SetBranchAddress("el_truth_pt", &el_truth_pt, &b_el_truth_pt);
   fChain->SetBranchAddress("el_truth_eta", &el_truth_eta, &b_el_truth_eta);
   fChain->SetBranchAddress("el_truth_phi", &el_truth_phi, &b_el_truth_phi);
   fChain->SetBranchAddress("el_truth_type", &el_truth_type, &b_el_truth_type);
   fChain->SetBranchAddress("el_truth_status", &el_truth_status, &b_el_truth_status);
   fChain->SetBranchAddress("el_truth_barcode", &el_truth_barcode, &b_el_truth_barcode);
   fChain->SetBranchAddress("el_truth_mothertype", &el_truth_mothertype, &b_el_truth_mothertype);
   fChain->SetBranchAddress("el_truth_motherbarcode", &el_truth_motherbarcode, &b_el_truth_motherbarcode);
   fChain->SetBranchAddress("el_truth_hasHardBrem", &el_truth_hasHardBrem, &b_el_truth_hasHardBrem);
   fChain->SetBranchAddress("el_truth_matched", &el_truth_matched, &b_el_truth_matched);
   fChain->SetBranchAddress("el_loose", &el_loose, &b_el_loose);
   fChain->SetBranchAddress("el_medium", &el_medium, &b_el_medium);
   fChain->SetBranchAddress("el_mediumIso", &el_mediumIso, &b_el_mediumIso);
   fChain->SetBranchAddress("el_tight", &el_tight, &b_el_tight);
   fChain->SetBranchAddress("el_tightIso", &el_tightIso, &b_el_tightIso);
   fChain->SetBranchAddress("el_mediumWithTrack", &el_mediumWithTrack, &b_el_mediumWithTrack);
   fChain->SetBranchAddress("el_tightWithTrack", &el_tightWithTrack, &b_el_tightWithTrack);
   fChain->SetBranchAddress("el_goodOQ", &el_goodOQ, &b_el_goodOQ);
   fChain->SetBranchAddress("el_rphiallcalo", &el_rphiallcalo, &b_el_rphiallcalo);
   fChain->SetBranchAddress("el_Etcone45", &el_Etcone45, &b_el_Etcone45);
   fChain->SetBranchAddress("el_Etcone15", &el_Etcone15, &b_el_Etcone15);
   fChain->SetBranchAddress("el_Etcone20", &el_Etcone20, &b_el_Etcone20);
   fChain->SetBranchAddress("el_Etcone25", &el_Etcone25, &b_el_Etcone25);
   fChain->SetBranchAddress("el_Etcone30", &el_Etcone30, &b_el_Etcone30);
   fChain->SetBranchAddress("el_Etcone35", &el_Etcone35, &b_el_Etcone35);
   fChain->SetBranchAddress("el_Etcone40", &el_Etcone40, &b_el_Etcone40);
   fChain->SetBranchAddress("el_ptcone20", &el_ptcone20, &b_el_ptcone20);
   fChain->SetBranchAddress("el_ptcone30", &el_ptcone30, &b_el_ptcone30);
   fChain->SetBranchAddress("el_ptcone40", &el_ptcone40, &b_el_ptcone40);
   fChain->SetBranchAddress("el_nucone20", &el_nucone20, &b_el_nucone20);
   fChain->SetBranchAddress("el_nucone30", &el_nucone30, &b_el_nucone30);
   fChain->SetBranchAddress("el_nucone40", &el_nucone40, &b_el_nucone40);
   fChain->SetBranchAddress("el_pos7", &el_pos7, &b_el_pos7);
   fChain->SetBranchAddress("el_etacorrmag", &el_etacorrmag, &b_el_etacorrmag);
   fChain->SetBranchAddress("el_deltaeta1", &el_deltaeta1, &b_el_deltaeta1);
   fChain->SetBranchAddress("el_deltaeta2", &el_deltaeta2, &b_el_deltaeta2);
   fChain->SetBranchAddress("el_deltaphi2", &el_deltaphi2, &b_el_deltaphi2);
   fChain->SetBranchAddress("el_deltaphiRescaled", &el_deltaphiRescaled, &b_el_deltaphiRescaled);
   fChain->SetBranchAddress("el_deltaPhiRot", &el_deltaPhiRot, &b_el_deltaPhiRot);
   fChain->SetBranchAddress("el_expectHitInBLayer", &el_expectHitInBLayer, &b_el_expectHitInBLayer);
   fChain->SetBranchAddress("el_trackd0_physics", &el_trackd0_physics, &b_el_trackd0_physics);
   fChain->SetBranchAddress("el_reta", &el_reta, &b_el_reta);
   fChain->SetBranchAddress("el_rphi", &el_rphi, &b_el_rphi);
   fChain->SetBranchAddress("el_zvertex", &el_zvertex, &b_el_zvertex);
   fChain->SetBranchAddress("el_errz", &el_errz, &b_el_errz);
   fChain->SetBranchAddress("el_etap", &el_etap, &b_el_etap);
   fChain->SetBranchAddress("el_depth", &el_depth, &b_el_depth);
   fChain->SetBranchAddress("el_refittedTrack_n", &el_refittedTrack_n, &b_el_refittedTrack_n);
   fChain->SetBranchAddress("el_cl_E", &el_cl_E, &b_el_cl_E);
   fChain->SetBranchAddress("el_cl_pt", &el_cl_pt, &b_el_cl_pt);
   fChain->SetBranchAddress("el_cl_eta", &el_cl_eta, &b_el_cl_eta);
   fChain->SetBranchAddress("el_cl_phi", &el_cl_phi, &b_el_cl_phi);
   fChain->SetBranchAddress("el_cl_time", &el_cl_time, &b_el_cl_time);
   fChain->SetBranchAddress("el_trackd0", &el_trackd0, &b_el_trackd0);
   fChain->SetBranchAddress("el_trackz0", &el_trackz0, &b_el_trackz0);
   fChain->SetBranchAddress("el_trackphi", &el_trackphi, &b_el_trackphi);
   fChain->SetBranchAddress("el_tracktheta", &el_tracktheta, &b_el_tracktheta);
   fChain->SetBranchAddress("el_trackqoverp", &el_trackqoverp, &b_el_trackqoverp);
   fChain->SetBranchAddress("el_trackpt", &el_trackpt, &b_el_trackpt);
   fChain->SetBranchAddress("el_tracketa", &el_tracketa, &b_el_tracketa);
   fChain->SetBranchAddress("el_nBLHits", &el_nBLHits, &b_el_nBLHits);
   fChain->SetBranchAddress("el_nPixHits", &el_nPixHits, &b_el_nPixHits);
   fChain->SetBranchAddress("el_nSCTHits", &el_nSCTHits, &b_el_nSCTHits);
   fChain->SetBranchAddress("el_nTRTHits", &el_nTRTHits, &b_el_nTRTHits);
   fChain->SetBranchAddress("el_nTRTHighTHits", &el_nTRTHighTHits, &b_el_nTRTHighTHits);
   fChain->SetBranchAddress("el_nPixHoles", &el_nPixHoles, &b_el_nPixHoles);
   fChain->SetBranchAddress("el_nSCTHoles", &el_nSCTHoles, &b_el_nSCTHoles);
   fChain->SetBranchAddress("el_nTRTHoles", &el_nTRTHoles, &b_el_nTRTHoles);
   fChain->SetBranchAddress("el_nBLSharedHits", &el_nBLSharedHits, &b_el_nBLSharedHits);
   fChain->SetBranchAddress("el_nPixSharedHits", &el_nPixSharedHits, &b_el_nPixSharedHits);
   fChain->SetBranchAddress("el_nSCTSharedHits", &el_nSCTSharedHits, &b_el_nSCTSharedHits);
   fChain->SetBranchAddress("el_nBLayerOutliers", &el_nBLayerOutliers, &b_el_nBLayerOutliers);
   fChain->SetBranchAddress("el_nPixelOutliers", &el_nPixelOutliers, &b_el_nPixelOutliers);
   fChain->SetBranchAddress("el_nSCTOutliers", &el_nSCTOutliers, &b_el_nSCTOutliers);
   fChain->SetBranchAddress("el_nTRTOutliers", &el_nTRTOutliers, &b_el_nTRTOutliers);
   fChain->SetBranchAddress("el_nTRTHighTOutliers", &el_nTRTHighTOutliers, &b_el_nTRTHighTOutliers);
   fChain->SetBranchAddress("el_expectBLayerHit", &el_expectBLayerHit, &b_el_expectBLayerHit);
   fChain->SetBranchAddress("el_nSiHits", &el_nSiHits, &b_el_nSiHits);
   fChain->SetBranchAddress("el_TRTHighTHitsRatio", &el_TRTHighTHitsRatio, &b_el_TRTHighTHitsRatio);
   fChain->SetBranchAddress("el_TRTHighTOutliersRatio", &el_TRTHighTOutliersRatio, &b_el_TRTHighTOutliersRatio);
   fChain->SetBranchAddress("el_pixeldEdx", &el_pixeldEdx, &b_el_pixeldEdx);
   fChain->SetBranchAddress("el_nGoodHitsPixeldEdx", &el_nGoodHitsPixeldEdx, &b_el_nGoodHitsPixeldEdx);
   fChain->SetBranchAddress("el_massPixeldEdx", &el_massPixeldEdx, &b_el_massPixeldEdx);
   fChain->SetBranchAddress("el_likelihoodsPixeldEdx", &el_likelihoodsPixeldEdx, &b_el_likelihoodsPixeldEdx);
   fChain->SetBranchAddress("el_vertweight", &el_vertweight, &b_el_vertweight);
   fChain->SetBranchAddress("el_hastrack", &el_hastrack, &b_el_hastrack);
   fChain->SetBranchAddress("el_Etcone40_ED_corrected", &el_Etcone40_ED_corrected, &b_el_Etcone40_ED_corrected);
   fChain->SetBranchAddress("el_Etcone40_corrected", &el_Etcone40_corrected, &b_el_Etcone40_corrected);
   fChain->SetBranchAddress("el_EF_dr", &el_EF_dr, &b_el_EF_dr);
   fChain->SetBranchAddress("EF_2e5_medium", &EF_2e5_medium, &b_EF_2e5_medium);
   fChain->SetBranchAddress("EF_2j35_jetNoEF_xe40_noMu", &EF_2j35_jetNoEF_xe40_noMu, &b_EF_2j35_jetNoEF_xe40_noMu);
   fChain->SetBranchAddress("EF_2mu10", &EF_2mu10, &b_EF_2mu10);
   fChain->SetBranchAddress("EF_2mu4", &EF_2mu4, &b_EF_2mu4);
   fChain->SetBranchAddress("EF_e10_medium", &EF_e10_medium, &b_EF_e10_medium);
   fChain->SetBranchAddress("EF_e15_medium_xe30_noMu", &EF_e15_medium_xe30_noMu, &b_EF_e15_medium_xe30_noMu);
   fChain->SetBranchAddress("EF_e15_medium_xe35_noMu", &EF_e15_medium_xe35_noMu, &b_EF_e15_medium_xe35_noMu);
   fChain->SetBranchAddress("EF_e15_medium_xe40_noMu_loose", &EF_e15_medium_xe40_noMu_loose, &b_EF_e15_medium_xe40_noMu_loose);
   fChain->SetBranchAddress("EF_e15_tight_xe30_noMu", &EF_e15_tight_xe30_noMu, &b_EF_e15_tight_xe30_noMu);
   fChain->SetBranchAddress("EF_e20_loose", &EF_e20_loose, &b_EF_e20_loose);
   fChain->SetBranchAddress("EF_e20_loose_xe20", &EF_e20_loose_xe20, &b_EF_e20_loose_xe20);
   fChain->SetBranchAddress("EF_e20_loose_xe20_noMu", &EF_e20_loose_xe20_noMu, &b_EF_e20_loose_xe20_noMu);
   fChain->SetBranchAddress("EF_e20_loose_xe30", &EF_e20_loose_xe30, &b_EF_e20_loose_xe30);
   fChain->SetBranchAddress("EF_e20_loose_xe30_noMu", &EF_e20_loose_xe30_noMu, &b_EF_e20_loose_xe30_noMu);
   fChain->SetBranchAddress("EF_e20_medium", &EF_e20_medium, &b_EF_e20_medium);
   fChain->SetBranchAddress("EF_em105_passHLT", &EF_em105_passHLT, &b_EF_em105_passHLT);
   fChain->SetBranchAddress("EF_g20_etcut_xe30_noMu", &EF_g20_etcut_xe30_noMu, &b_EF_g20_etcut_xe30_noMu);
   fChain->SetBranchAddress("EF_g20_loose", &EF_g20_loose, &b_EF_g20_loose);
   fChain->SetBranchAddress("EF_g20_loose_xe20_noMu", &EF_g20_loose_xe20_noMu, &b_EF_g20_loose_xe20_noMu);
   fChain->SetBranchAddress("EF_g20_loose_xe30_noMu", &EF_g20_loose_xe30_noMu, &b_EF_g20_loose_xe30_noMu);
   fChain->SetBranchAddress("EF_g25_loose_xe30_noMu", &EF_g25_loose_xe30_noMu, &b_EF_g25_loose_xe30_noMu);
   fChain->SetBranchAddress("EF_g40_loose_xe40_noMu", &EF_g40_loose_xe40_noMu, &b_EF_g40_loose_xe40_noMu);
   fChain->SetBranchAddress("EF_j55_a4tc_EFFS_xe55_medium_noMu_dphi2j30xe10", &EF_j55_a4tc_EFFS_xe55_medium_noMu_dphi2j30xe10, &b_EF_j55_a4tc_EFFS_xe55_medium_noMu_dphi2j30xe10);
   fChain->SetBranchAddress("EF_j75_a4tc_EFFS_xe40_loose_noMu", &EF_j75_a4tc_EFFS_xe40_loose_noMu, &b_EF_j75_a4tc_EFFS_xe40_loose_noMu);
   fChain->SetBranchAddress("EF_j75_a4tc_EFFS_xe40_loose_noMu_dphijxe03", &EF_j75_a4tc_EFFS_xe40_loose_noMu_dphijxe03, &b_EF_j75_a4tc_EFFS_xe40_loose_noMu_dphijxe03);
   fChain->SetBranchAddress("EF_j75_a4tc_EFFS_xe45_loose_noMu", &EF_j75_a4tc_EFFS_xe45_loose_noMu, &b_EF_j75_a4tc_EFFS_xe45_loose_noMu);
   fChain->SetBranchAddress("EF_j75_a4tc_EFFS_xe55_loose_noMu", &EF_j75_a4tc_EFFS_xe55_loose_noMu, &b_EF_j75_a4tc_EFFS_xe55_loose_noMu);
   fChain->SetBranchAddress("EF_mu10", &EF_mu10, &b_EF_mu10);
   fChain->SetBranchAddress("EF_mu13", &EF_mu13, &b_EF_mu13);
   fChain->SetBranchAddress("EF_mu18", &EF_mu18, &b_EF_mu18);
   fChain->SetBranchAddress("EF_mu20", &EF_mu20, &b_EF_mu20);
   fChain->SetBranchAddress("EF_mu20_MSonly", &EF_mu20_MSonly, &b_EF_mu20_MSonly);
   fChain->SetBranchAddress("EF_mu4", &EF_mu4, &b_EF_mu4);
   fChain->SetBranchAddress("EF_mu6", &EF_mu6, &b_EF_mu6);
   fChain->SetBranchAddress("EF_tau16_j75_a4tc_EFFS_xe40_loose_noMu", &EF_tau16_j75_a4tc_EFFS_xe40_loose_noMu, &b_EF_tau16_j75_a4tc_EFFS_xe40_loose_noMu);
   fChain->SetBranchAddress("EF_tau16_loose_xe20_noMu", &EF_tau16_loose_xe20_noMu, &b_EF_tau16_loose_xe20_noMu);
   fChain->SetBranchAddress("EF_tau16_loose_xe25_noMu", &EF_tau16_loose_xe25_noMu, &b_EF_tau16_loose_xe25_noMu);
   fChain->SetBranchAddress("EF_tau16_loose_xe30_noMu", &EF_tau16_loose_xe30_noMu, &b_EF_tau16_loose_xe30_noMu);
   fChain->SetBranchAddress("EF_tau16_medium_xe22_noMu", &EF_tau16_medium_xe22_noMu, &b_EF_tau16_medium_xe22_noMu);
   fChain->SetBranchAddress("EF_tau16_medium_xe25_noMu", &EF_tau16_medium_xe25_noMu, &b_EF_tau16_medium_xe25_noMu);
   fChain->SetBranchAddress("EF_tau16_medium_xe25_tight_noMu", &EF_tau16_medium_xe25_tight_noMu, &b_EF_tau16_medium_xe25_tight_noMu);
   fChain->SetBranchAddress("EF_tau29_medium_xe35_noMu", &EF_tau29_medium_xe35_noMu, &b_EF_tau29_medium_xe35_noMu);
   fChain->SetBranchAddress("EF_tau29_medium_xe40_loose_noMu", &EF_tau29_medium_xe40_loose_noMu, &b_EF_tau29_medium_xe40_loose_noMu);
   fChain->SetBranchAddress("EF_xe20_noMu", &EF_xe20_noMu, &b_EF_xe20_noMu);
   fChain->SetBranchAddress("EF_xe30_noMu", &EF_xe30_noMu, &b_EF_xe30_noMu);
   fChain->SetBranchAddress("EF_xe35_noMu", &EF_xe35_noMu, &b_EF_xe35_noMu);
   fChain->SetBranchAddress("EF_xe40_noMu", &EF_xe40_noMu, &b_EF_xe40_noMu);
   fChain->SetBranchAddress("EF_xe50_noMu", &EF_xe50_noMu, &b_EF_xe50_noMu);
   fChain->SetBranchAddress("EF_xe60_noMu", &EF_xe60_noMu, &b_EF_xe60_noMu);
   fChain->SetBranchAddress("EF_xe70_noMu", &EF_xe70_noMu, &b_EF_xe70_noMu);
   fChain->SetBranchAddress("EF_xe80_noMu", &EF_xe80_noMu, &b_EF_xe80_noMu);
   fChain->SetBranchAddress("EF_xe90_noMu", &EF_xe90_noMu, &b_EF_xe90_noMu);
   fChain->SetBranchAddress("L1_2J15_XE25", &L1_2J15_XE25, &b_L1_2J15_XE25);
   fChain->SetBranchAddress("L1_EM10_XE20", &L1_EM10_XE20, &b_L1_EM10_XE20);
   fChain->SetBranchAddress("L1_EM14_XE10", &L1_EM14_XE10, &b_L1_EM14_XE10);
   fChain->SetBranchAddress("L1_EM14_XE20", &L1_EM14_XE20, &b_L1_EM14_XE20);
   fChain->SetBranchAddress("L1_J30_XE35", &L1_J30_XE35, &b_L1_J30_XE35);
   fChain->SetBranchAddress("L1_J50_XE20", &L1_J50_XE20, &b_L1_J50_XE20);
   fChain->SetBranchAddress("L1_J50_XE25", &L1_J50_XE25, &b_L1_J50_XE25);
   fChain->SetBranchAddress("L1_MU0", &L1_MU0, &b_L1_MU0);
   fChain->SetBranchAddress("L1_MU10", &L1_MU10, &b_L1_MU10);
   fChain->SetBranchAddress("L1_MU20", &L1_MU20, &b_L1_MU20);
   fChain->SetBranchAddress("L1_MU6", &L1_MU6, &b_L1_MU6);
   fChain->SetBranchAddress("L1_TAU11_XE10_3J10", &L1_TAU11_XE10_3J10, &b_L1_TAU11_XE10_3J10);
   fChain->SetBranchAddress("L1_TAU11_XE20", &L1_TAU11_XE20, &b_L1_TAU11_XE20);
   fChain->SetBranchAddress("L1_TAU15_XE20", &L1_TAU15_XE20, &b_L1_TAU15_XE20);
   fChain->SetBranchAddress("L1_TAU6_J50_XE20", &L1_TAU6_J50_XE20, &b_L1_TAU6_J50_XE20);
   fChain->SetBranchAddress("L1_TAU6_XE10", &L1_TAU6_XE10, &b_L1_TAU6_XE10);
   fChain->SetBranchAddress("L1_XE10", &L1_XE10, &b_L1_XE10);
   fChain->SetBranchAddress("L1_XE20", &L1_XE20, &b_L1_XE20);
   fChain->SetBranchAddress("L1_XE25", &L1_XE25, &b_L1_XE25);
   fChain->SetBranchAddress("L1_XE30", &L1_XE30, &b_L1_XE30);
   fChain->SetBranchAddress("L1_XE35", &L1_XE35, &b_L1_XE35);
   fChain->SetBranchAddress("L1_XE40", &L1_XE40, &b_L1_XE40);
   fChain->SetBranchAddress("L1_XE50", &L1_XE50, &b_L1_XE50);
   fChain->SetBranchAddress("L1_XE60", &L1_XE60, &b_L1_XE60);
   fChain->SetBranchAddress("L2_2j35_jetNoEF_xe40_noMu", &L2_2j35_jetNoEF_xe40_noMu, &b_L2_2j35_jetNoEF_xe40_noMu);
   fChain->SetBranchAddress("L2_2mu4", &L2_2mu4, &b_L2_2mu4);
   fChain->SetBranchAddress("L2_e15_medium_xe30_noMu", &L2_e15_medium_xe30_noMu, &b_L2_e15_medium_xe30_noMu);
   fChain->SetBranchAddress("L2_e15_medium_xe35_noMu", &L2_e15_medium_xe35_noMu, &b_L2_e15_medium_xe35_noMu);
   fChain->SetBranchAddress("L2_e15_medium_xe40_noMu_loose", &L2_e15_medium_xe40_noMu_loose, &b_L2_e15_medium_xe40_noMu_loose);
   fChain->SetBranchAddress("L2_e15_tight_xe30_noMu", &L2_e15_tight_xe30_noMu, &b_L2_e15_tight_xe30_noMu);
   fChain->SetBranchAddress("L2_e20_loose_xe20", &L2_e20_loose_xe20, &b_L2_e20_loose_xe20);
   fChain->SetBranchAddress("L2_e20_loose_xe20_noMu", &L2_e20_loose_xe20_noMu, &b_L2_e20_loose_xe20_noMu);
   fChain->SetBranchAddress("L2_e20_loose_xe30", &L2_e20_loose_xe30, &b_L2_e20_loose_xe30);
   fChain->SetBranchAddress("L2_e20_loose_xe30_noMu", &L2_e20_loose_xe30_noMu, &b_L2_e20_loose_xe30_noMu);
   fChain->SetBranchAddress("L2_g20_etcut_xe30_noMu", &L2_g20_etcut_xe30_noMu, &b_L2_g20_etcut_xe30_noMu);
   fChain->SetBranchAddress("L2_g20_loose_xe20_noMu", &L2_g20_loose_xe20_noMu, &b_L2_g20_loose_xe20_noMu);
   fChain->SetBranchAddress("L2_g20_loose_xe30_noMu", &L2_g20_loose_xe30_noMu, &b_L2_g20_loose_xe30_noMu);
   fChain->SetBranchAddress("L2_g25_loose_xe30_noMu", &L2_g25_loose_xe30_noMu, &b_L2_g25_loose_xe30_noMu);
   fChain->SetBranchAddress("L2_g40_loose_xe40_noMu", &L2_g40_loose_xe40_noMu, &b_L2_g40_loose_xe40_noMu);
   fChain->SetBranchAddress("L2_j50_xe35_medium_noMu", &L2_j50_xe35_medium_noMu, &b_L2_j50_xe35_medium_noMu);
   fChain->SetBranchAddress("L2_j70_xe20_loose_noMu", &L2_j70_xe20_loose_noMu, &b_L2_j70_xe20_loose_noMu);
   fChain->SetBranchAddress("L2_j70_xe25_loose_noMu", &L2_j70_xe25_loose_noMu, &b_L2_j70_xe25_loose_noMu);
   fChain->SetBranchAddress("L2_mu10", &L2_mu10, &b_L2_mu10);
   fChain->SetBranchAddress("L2_mu13", &L2_mu13, &b_L2_mu13);
   fChain->SetBranchAddress("L2_mu20", &L2_mu20, &b_L2_mu20);
   fChain->SetBranchAddress("L2_mu20_MSonly", &L2_mu20_MSonly, &b_L2_mu20_MSonly);
   fChain->SetBranchAddress("L2_mu4", &L2_mu4, &b_L2_mu4);
   fChain->SetBranchAddress("L2_mu6", &L2_mu6, &b_L2_mu6);
   fChain->SetBranchAddress("L2_tau16_j70_xe20_loose_noMu", &L2_tau16_j70_xe20_loose_noMu, &b_L2_tau16_j70_xe20_loose_noMu);
   fChain->SetBranchAddress("L2_tau16_loose_xe20_noMu", &L2_tau16_loose_xe20_noMu, &b_L2_tau16_loose_xe20_noMu);
   fChain->SetBranchAddress("L2_tau16_loose_xe25_noMu", &L2_tau16_loose_xe25_noMu, &b_L2_tau16_loose_xe25_noMu);
   fChain->SetBranchAddress("L2_tau16_loose_xe30_noMu", &L2_tau16_loose_xe30_noMu, &b_L2_tau16_loose_xe30_noMu);
   fChain->SetBranchAddress("L2_tau16_medium_xe22_noMu", &L2_tau16_medium_xe22_noMu, &b_L2_tau16_medium_xe22_noMu);
   fChain->SetBranchAddress("L2_tau16_medium_xe25_noMu", &L2_tau16_medium_xe25_noMu, &b_L2_tau16_medium_xe25_noMu);
   fChain->SetBranchAddress("L2_tau16_medium_xe25_tight_noMu", &L2_tau16_medium_xe25_tight_noMu, &b_L2_tau16_medium_xe25_tight_noMu);
   fChain->SetBranchAddress("L2_tau29_medium_xe25_noMu", &L2_tau29_medium_xe25_noMu, &b_L2_tau29_medium_xe25_noMu);
   fChain->SetBranchAddress("L2_tau29_medium_xe30_loose_noMu", &L2_tau29_medium_xe30_loose_noMu, &b_L2_tau29_medium_xe30_loose_noMu);
   fChain->SetBranchAddress("L2_xe10_noMu", &L2_xe10_noMu, &b_L2_xe10_noMu);
   fChain->SetBranchAddress("L2_xe20_noMu", &L2_xe20_noMu, &b_L2_xe20_noMu);
   fChain->SetBranchAddress("L2_xe25_noMu", &L2_xe25_noMu, &b_L2_xe25_noMu);
   fChain->SetBranchAddress("L2_xe30_noMu", &L2_xe30_noMu, &b_L2_xe30_noMu);
   fChain->SetBranchAddress("L2_xe35_noMu", &L2_xe35_noMu, &b_L2_xe35_noMu);
   fChain->SetBranchAddress("L2_xe40_noMu", &L2_xe40_noMu, &b_L2_xe40_noMu);
   fChain->SetBranchAddress("L2_xe50_noMu", &L2_xe50_noMu, &b_L2_xe50_noMu);
   fChain->SetBranchAddress("L2_xe60_noMu", &L2_xe60_noMu, &b_L2_xe60_noMu);
   fChain->SetBranchAddress("L2_xe70_noMu", &L2_xe70_noMu, &b_L2_xe70_noMu);
   fChain->SetBranchAddress("ph_n", &ph_n, &b_ph_n);
   fChain->SetBranchAddress("ph_E", &ph_E, &b_ph_E);
   fChain->SetBranchAddress("ph_Et", &ph_Et, &b_ph_Et);
   fChain->SetBranchAddress("ph_pt", &ph_pt, &b_ph_pt);
   fChain->SetBranchAddress("ph_m", &ph_m, &b_ph_m);
   fChain->SetBranchAddress("ph_eta", &ph_eta, &b_ph_eta);
   fChain->SetBranchAddress("ph_phi", &ph_phi, &b_ph_phi);
   fChain->SetBranchAddress("ph_px", &ph_px, &b_ph_px);
   fChain->SetBranchAddress("ph_py", &ph_py, &b_ph_py);
   fChain->SetBranchAddress("ph_pz", &ph_pz, &b_ph_pz);
   fChain->SetBranchAddress("ph_author", &ph_author, &b_ph_author);
   fChain->SetBranchAddress("ph_isRecovered", &ph_isRecovered, &b_ph_isRecovered);
   fChain->SetBranchAddress("ph_isEM", &ph_isEM, &b_ph_isEM);
   fChain->SetBranchAddress("ph_OQ", &ph_OQ, &b_ph_OQ);
   fChain->SetBranchAddress("ph_OQRecalc", &ph_OQRecalc, &b_ph_OQRecalc);
   fChain->SetBranchAddress("ph_convFlag", &ph_convFlag, &b_ph_convFlag);
   fChain->SetBranchAddress("ph_isConv", &ph_isConv, &b_ph_isConv);
   fChain->SetBranchAddress("ph_nConv", &ph_nConv, &b_ph_nConv);
   fChain->SetBranchAddress("ph_nSingleTrackConv", &ph_nSingleTrackConv, &b_ph_nSingleTrackConv);
   fChain->SetBranchAddress("ph_nDoubleTrackConv", &ph_nDoubleTrackConv, &b_ph_nDoubleTrackConv);
   fChain->SetBranchAddress("ph_truth_deltaRRecPhoton", &ph_truth_deltaRRecPhoton, &b_ph_truth_deltaRRecPhoton);
   fChain->SetBranchAddress("ph_truth_E", &ph_truth_E, &b_ph_truth_E);
   fChain->SetBranchAddress("ph_truth_pt", &ph_truth_pt, &b_ph_truth_pt);
   fChain->SetBranchAddress("ph_truth_eta", &ph_truth_eta, &b_ph_truth_eta);
   fChain->SetBranchAddress("ph_truth_phi", &ph_truth_phi, &b_ph_truth_phi);
   fChain->SetBranchAddress("ph_truth_type", &ph_truth_type, &b_ph_truth_type);
   fChain->SetBranchAddress("ph_truth_status", &ph_truth_status, &b_ph_truth_status);
   fChain->SetBranchAddress("ph_truth_barcode", &ph_truth_barcode, &b_ph_truth_barcode);
   fChain->SetBranchAddress("ph_truth_mothertype", &ph_truth_mothertype, &b_ph_truth_mothertype);
   fChain->SetBranchAddress("ph_truth_motherbarcode", &ph_truth_motherbarcode, &b_ph_truth_motherbarcode);
   fChain->SetBranchAddress("ph_truth_matched", &ph_truth_matched, &b_ph_truth_matched);
   fChain->SetBranchAddress("ph_loose", &ph_loose, &b_ph_loose);
   fChain->SetBranchAddress("ph_tight", &ph_tight, &b_ph_tight);
   fChain->SetBranchAddress("ph_tightIso", &ph_tightIso, &b_ph_tightIso);
   fChain->SetBranchAddress("ph_goodOQ", &ph_goodOQ, &b_ph_goodOQ);
   fChain->SetBranchAddress("ph_rphiallcalo", &ph_rphiallcalo, &b_ph_rphiallcalo);
   fChain->SetBranchAddress("ph_Etcone45", &ph_Etcone45, &b_ph_Etcone45);
   fChain->SetBranchAddress("ph_Etcone15", &ph_Etcone15, &b_ph_Etcone15);
   fChain->SetBranchAddress("ph_Etcone20", &ph_Etcone20, &b_ph_Etcone20);
   fChain->SetBranchAddress("ph_Etcone25", &ph_Etcone25, &b_ph_Etcone25);
   fChain->SetBranchAddress("ph_Etcone30", &ph_Etcone30, &b_ph_Etcone30);
   fChain->SetBranchAddress("ph_Etcone35", &ph_Etcone35, &b_ph_Etcone35);
   fChain->SetBranchAddress("ph_Etcone40", &ph_Etcone40, &b_ph_Etcone40);
   fChain->SetBranchAddress("ph_ptcone20", &ph_ptcone20, &b_ph_ptcone20);
   fChain->SetBranchAddress("ph_ptcone30", &ph_ptcone30, &b_ph_ptcone30);
   fChain->SetBranchAddress("ph_ptcone40", &ph_ptcone40, &b_ph_ptcone40);
   fChain->SetBranchAddress("ph_nucone20", &ph_nucone20, &b_ph_nucone20);
   fChain->SetBranchAddress("ph_nucone30", &ph_nucone30, &b_ph_nucone30);
   fChain->SetBranchAddress("ph_nucone40", &ph_nucone40, &b_ph_nucone40);
   fChain->SetBranchAddress("ph_convanglematch", &ph_convanglematch, &b_ph_convanglematch);
   fChain->SetBranchAddress("ph_convtrackmatch", &ph_convtrackmatch, &b_ph_convtrackmatch);
   fChain->SetBranchAddress("ph_hasconv", &ph_hasconv, &b_ph_hasconv);
   fChain->SetBranchAddress("ph_convvtxx", &ph_convvtxx, &b_ph_convvtxx);
   fChain->SetBranchAddress("ph_convvtxy", &ph_convvtxy, &b_ph_convvtxy);
   fChain->SetBranchAddress("ph_convvtxz", &ph_convvtxz, &b_ph_convvtxz);
   fChain->SetBranchAddress("ph_Rconv", &ph_Rconv, &b_ph_Rconv);
   fChain->SetBranchAddress("ph_zconv", &ph_zconv, &b_ph_zconv);
   fChain->SetBranchAddress("ph_convvtxchi2", &ph_convvtxchi2, &b_ph_convvtxchi2);
   fChain->SetBranchAddress("ph_pt1conv", &ph_pt1conv, &b_ph_pt1conv);
   fChain->SetBranchAddress("ph_convtrk1nBLHits", &ph_convtrk1nBLHits, &b_ph_convtrk1nBLHits);
   fChain->SetBranchAddress("ph_convtrk1nPixHits", &ph_convtrk1nPixHits, &b_ph_convtrk1nPixHits);
   fChain->SetBranchAddress("ph_convtrk1nSCTHits", &ph_convtrk1nSCTHits, &b_ph_convtrk1nSCTHits);
   fChain->SetBranchAddress("ph_convtrk1nTRTHits", &ph_convtrk1nTRTHits, &b_ph_convtrk1nTRTHits);
   fChain->SetBranchAddress("ph_pt2conv", &ph_pt2conv, &b_ph_pt2conv);
   fChain->SetBranchAddress("ph_convtrk2nBLHits", &ph_convtrk2nBLHits, &b_ph_convtrk2nBLHits);
   fChain->SetBranchAddress("ph_convtrk2nPixHits", &ph_convtrk2nPixHits, &b_ph_convtrk2nPixHits);
   fChain->SetBranchAddress("ph_convtrk2nSCTHits", &ph_convtrk2nSCTHits, &b_ph_convtrk2nSCTHits);
   fChain->SetBranchAddress("ph_convtrk2nTRTHits", &ph_convtrk2nTRTHits, &b_ph_convtrk2nTRTHits);
   fChain->SetBranchAddress("ph_ptconv", &ph_ptconv, &b_ph_ptconv);
   fChain->SetBranchAddress("ph_pzconv", &ph_pzconv, &b_ph_pzconv);
   fChain->SetBranchAddress("ph_reta", &ph_reta, &b_ph_reta);
   fChain->SetBranchAddress("ph_rphi", &ph_rphi, &b_ph_rphi);
   fChain->SetBranchAddress("ph_zvertex", &ph_zvertex, &b_ph_zvertex);
   fChain->SetBranchAddress("ph_errz", &ph_errz, &b_ph_errz);
   fChain->SetBranchAddress("ph_etap", &ph_etap, &b_ph_etap);
   fChain->SetBranchAddress("ph_depth", &ph_depth, &b_ph_depth);
   fChain->SetBranchAddress("ph_cl_E", &ph_cl_E, &b_ph_cl_E);
   fChain->SetBranchAddress("ph_cl_pt", &ph_cl_pt, &b_ph_cl_pt);
   fChain->SetBranchAddress("ph_cl_eta", &ph_cl_eta, &b_ph_cl_eta);
   fChain->SetBranchAddress("ph_cl_phi", &ph_cl_phi, &b_ph_cl_phi);
   fChain->SetBranchAddress("ph_cl_time", &ph_cl_time, &b_ph_cl_time);
   fChain->SetBranchAddress("ph_truth_isConv", &ph_truth_isConv, &b_ph_truth_isConv);
   fChain->SetBranchAddress("ph_truth_isBrem", &ph_truth_isBrem, &b_ph_truth_isBrem);
   fChain->SetBranchAddress("ph_truth_isFromHardProc", &ph_truth_isFromHardProc, &b_ph_truth_isFromHardProc);
   fChain->SetBranchAddress("ph_truth_isPhotonFromHardProc", &ph_truth_isPhotonFromHardProc, &b_ph_truth_isPhotonFromHardProc);
   fChain->SetBranchAddress("ph_truth_Rconv", &ph_truth_Rconv, &b_ph_truth_Rconv);
   fChain->SetBranchAddress("ph_truth_zconv", &ph_truth_zconv, &b_ph_truth_zconv);
   fChain->SetBranchAddress("ph_deltaEmax2", &ph_deltaEmax2, &b_ph_deltaEmax2);
   fChain->SetBranchAddress("ph_jet_dr", &ph_jet_dr, &b_ph_jet_dr);
   fChain->SetBranchAddress("ph_jet_matched", &ph_jet_matched, &b_ph_jet_matched);
   fChain->SetBranchAddress("ph_convIP", &ph_convIP, &b_ph_convIP);
   fChain->SetBranchAddress("ph_convIPRev", &ph_convIPRev, &b_ph_convIPRev);
   fChain->SetBranchAddress("ph_ptIsolationCone", &ph_ptIsolationCone, &b_ph_ptIsolationCone);
   fChain->SetBranchAddress("ph_ptIsolationConePhAngle", &ph_ptIsolationConePhAngle, &b_ph_ptIsolationConePhAngle);
   fChain->SetBranchAddress("ph_Etcone40_ED_corrected", &ph_Etcone40_ED_corrected, &b_ph_Etcone40_ED_corrected);
   fChain->SetBranchAddress("ph_Etcone40_corrected", &ph_Etcone40_corrected, &b_ph_Etcone40_corrected);
   fChain->SetBranchAddress("mu_staco_n", &mu_staco_n, &b_mu_staco_n);
   fChain->SetBranchAddress("mu_staco_E", &mu_staco_E, &b_mu_staco_E);
   fChain->SetBranchAddress("mu_staco_pt", &mu_staco_pt, &b_mu_staco_pt);
   fChain->SetBranchAddress("mu_staco_m", &mu_staco_m, &b_mu_staco_m);
   fChain->SetBranchAddress("mu_staco_eta", &mu_staco_eta, &b_mu_staco_eta);
   fChain->SetBranchAddress("mu_staco_phi", &mu_staco_phi, &b_mu_staco_phi);
   fChain->SetBranchAddress("mu_staco_px", &mu_staco_px, &b_mu_staco_px);
   fChain->SetBranchAddress("mu_staco_py", &mu_staco_py, &b_mu_staco_py);
   fChain->SetBranchAddress("mu_staco_pz", &mu_staco_pz, &b_mu_staco_pz);
   fChain->SetBranchAddress("mu_staco_charge", &mu_staco_charge, &b_mu_staco_charge);
   fChain->SetBranchAddress("mu_staco_allauthor", &mu_staco_allauthor, &b_mu_staco_allauthor);
   fChain->SetBranchAddress("mu_staco_author", &mu_staco_author, &b_mu_staco_author);
   fChain->SetBranchAddress("mu_staco_matchchi2", &mu_staco_matchchi2, &b_mu_staco_matchchi2);
   fChain->SetBranchAddress("mu_staco_matchndof", &mu_staco_matchndof, &b_mu_staco_matchndof);
   fChain->SetBranchAddress("mu_staco_etcone20", &mu_staco_etcone20, &b_mu_staco_etcone20);
   fChain->SetBranchAddress("mu_staco_etcone30", &mu_staco_etcone30, &b_mu_staco_etcone30);
   fChain->SetBranchAddress("mu_staco_etcone40", &mu_staco_etcone40, &b_mu_staco_etcone40);
   fChain->SetBranchAddress("mu_staco_nucone20", &mu_staco_nucone20, &b_mu_staco_nucone20);
   fChain->SetBranchAddress("mu_staco_nucone30", &mu_staco_nucone30, &b_mu_staco_nucone30);
   fChain->SetBranchAddress("mu_staco_nucone40", &mu_staco_nucone40, &b_mu_staco_nucone40);
   fChain->SetBranchAddress("mu_staco_ptcone20", &mu_staco_ptcone20, &b_mu_staco_ptcone20);
   fChain->SetBranchAddress("mu_staco_ptcone30", &mu_staco_ptcone30, &b_mu_staco_ptcone30);
   fChain->SetBranchAddress("mu_staco_ptcone40", &mu_staco_ptcone40, &b_mu_staco_ptcone40);
   fChain->SetBranchAddress("mu_staco_energyLossPar", &mu_staco_energyLossPar, &b_mu_staco_energyLossPar);
   fChain->SetBranchAddress("mu_staco_energyLossErr", &mu_staco_energyLossErr, &b_mu_staco_energyLossErr);
   fChain->SetBranchAddress("mu_staco_etCore", &mu_staco_etCore, &b_mu_staco_etCore);
   fChain->SetBranchAddress("mu_staco_energyLossType", &mu_staco_energyLossType, &b_mu_staco_energyLossType);
   fChain->SetBranchAddress("mu_staco_caloMuonIdTag", &mu_staco_caloMuonIdTag, &b_mu_staco_caloMuonIdTag);
   fChain->SetBranchAddress("mu_staco_caloLRLikelihood", &mu_staco_caloLRLikelihood, &b_mu_staco_caloLRLikelihood);
   fChain->SetBranchAddress("mu_staco_bestMatch", &mu_staco_bestMatch, &b_mu_staco_bestMatch);
   fChain->SetBranchAddress("mu_staco_isStandAloneMuon", &mu_staco_isStandAloneMuon, &b_mu_staco_isStandAloneMuon);
   fChain->SetBranchAddress("mu_staco_isCombinedMuon", &mu_staco_isCombinedMuon, &b_mu_staco_isCombinedMuon);
   fChain->SetBranchAddress("mu_staco_isLowPtReconstructedMuon", &mu_staco_isLowPtReconstructedMuon, &b_mu_staco_isLowPtReconstructedMuon);
   fChain->SetBranchAddress("mu_staco_isSegmentTaggedMuon", &mu_staco_isSegmentTaggedMuon, &b_mu_staco_isSegmentTaggedMuon);
   fChain->SetBranchAddress("mu_staco_isCaloMuonId", &mu_staco_isCaloMuonId, &b_mu_staco_isCaloMuonId);
   fChain->SetBranchAddress("mu_staco_alsoFoundByLowPt", &mu_staco_alsoFoundByLowPt, &b_mu_staco_alsoFoundByLowPt);
   fChain->SetBranchAddress("mu_staco_alsoFoundByCaloMuonId", &mu_staco_alsoFoundByCaloMuonId, &b_mu_staco_alsoFoundByCaloMuonId);
   fChain->SetBranchAddress("mu_staco_loose", &mu_staco_loose, &b_mu_staco_loose);
   fChain->SetBranchAddress("mu_staco_medium", &mu_staco_medium, &b_mu_staco_medium);
   fChain->SetBranchAddress("mu_staco_tight", &mu_staco_tight, &b_mu_staco_tight);
   fChain->SetBranchAddress("mu_staco_d0_exPV", &mu_staco_d0_exPV, &b_mu_staco_d0_exPV);
   fChain->SetBranchAddress("mu_staco_z0_exPV", &mu_staco_z0_exPV, &b_mu_staco_z0_exPV);
   fChain->SetBranchAddress("mu_staco_phi_exPV", &mu_staco_phi_exPV, &b_mu_staco_phi_exPV);
   fChain->SetBranchAddress("mu_staco_theta_exPV", &mu_staco_theta_exPV, &b_mu_staco_theta_exPV);
   fChain->SetBranchAddress("mu_staco_qoverp_exPV", &mu_staco_qoverp_exPV, &b_mu_staco_qoverp_exPV);
   fChain->SetBranchAddress("mu_staco_cb_d0_exPV", &mu_staco_cb_d0_exPV, &b_mu_staco_cb_d0_exPV);
   fChain->SetBranchAddress("mu_staco_cb_z0_exPV", &mu_staco_cb_z0_exPV, &b_mu_staco_cb_z0_exPV);
   fChain->SetBranchAddress("mu_staco_cb_phi_exPV", &mu_staco_cb_phi_exPV, &b_mu_staco_cb_phi_exPV);
   fChain->SetBranchAddress("mu_staco_cb_theta_exPV", &mu_staco_cb_theta_exPV, &b_mu_staco_cb_theta_exPV);
   fChain->SetBranchAddress("mu_staco_cb_qoverp_exPV", &mu_staco_cb_qoverp_exPV, &b_mu_staco_cb_qoverp_exPV);
   fChain->SetBranchAddress("mu_staco_id_d0_exPV", &mu_staco_id_d0_exPV, &b_mu_staco_id_d0_exPV);
   fChain->SetBranchAddress("mu_staco_id_z0_exPV", &mu_staco_id_z0_exPV, &b_mu_staco_id_z0_exPV);
   fChain->SetBranchAddress("mu_staco_id_phi_exPV", &mu_staco_id_phi_exPV, &b_mu_staco_id_phi_exPV);
   fChain->SetBranchAddress("mu_staco_id_theta_exPV", &mu_staco_id_theta_exPV, &b_mu_staco_id_theta_exPV);
   fChain->SetBranchAddress("mu_staco_id_qoverp_exPV", &mu_staco_id_qoverp_exPV, &b_mu_staco_id_qoverp_exPV);
   fChain->SetBranchAddress("mu_staco_me_d0_exPV", &mu_staco_me_d0_exPV, &b_mu_staco_me_d0_exPV);
   fChain->SetBranchAddress("mu_staco_me_z0_exPV", &mu_staco_me_z0_exPV, &b_mu_staco_me_z0_exPV);
   fChain->SetBranchAddress("mu_staco_me_phi_exPV", &mu_staco_me_phi_exPV, &b_mu_staco_me_phi_exPV);
   fChain->SetBranchAddress("mu_staco_me_theta_exPV", &mu_staco_me_theta_exPV, &b_mu_staco_me_theta_exPV);
   fChain->SetBranchAddress("mu_staco_me_qoverp_exPV", &mu_staco_me_qoverp_exPV, &b_mu_staco_me_qoverp_exPV);
   fChain->SetBranchAddress("mu_staco_ie_d0_exPV", &mu_staco_ie_d0_exPV, &b_mu_staco_ie_d0_exPV);
   fChain->SetBranchAddress("mu_staco_ie_z0_exPV", &mu_staco_ie_z0_exPV, &b_mu_staco_ie_z0_exPV);
   fChain->SetBranchAddress("mu_staco_ie_phi_exPV", &mu_staco_ie_phi_exPV, &b_mu_staco_ie_phi_exPV);
   fChain->SetBranchAddress("mu_staco_ie_theta_exPV", &mu_staco_ie_theta_exPV, &b_mu_staco_ie_theta_exPV);
   fChain->SetBranchAddress("mu_staco_ie_qoverp_exPV", &mu_staco_ie_qoverp_exPV, &b_mu_staco_ie_qoverp_exPV);
   fChain->SetBranchAddress("mu_staco_ms_d0", &mu_staco_ms_d0, &b_mu_staco_ms_d0);
   fChain->SetBranchAddress("mu_staco_ms_z0", &mu_staco_ms_z0, &b_mu_staco_ms_z0);
   fChain->SetBranchAddress("mu_staco_ms_phi", &mu_staco_ms_phi, &b_mu_staco_ms_phi);
   fChain->SetBranchAddress("mu_staco_ms_theta", &mu_staco_ms_theta, &b_mu_staco_ms_theta);
   fChain->SetBranchAddress("mu_staco_ms_qoverp", &mu_staco_ms_qoverp, &b_mu_staco_ms_qoverp);
   fChain->SetBranchAddress("mu_staco_id_d0", &mu_staco_id_d0, &b_mu_staco_id_d0);
   fChain->SetBranchAddress("mu_staco_id_z0", &mu_staco_id_z0, &b_mu_staco_id_z0);
   fChain->SetBranchAddress("mu_staco_id_phi", &mu_staco_id_phi, &b_mu_staco_id_phi);
   fChain->SetBranchAddress("mu_staco_id_theta", &mu_staco_id_theta, &b_mu_staco_id_theta);
   fChain->SetBranchAddress("mu_staco_id_qoverp", &mu_staco_id_qoverp, &b_mu_staco_id_qoverp);
   fChain->SetBranchAddress("mu_staco_me_d0", &mu_staco_me_d0, &b_mu_staco_me_d0);
   fChain->SetBranchAddress("mu_staco_me_z0", &mu_staco_me_z0, &b_mu_staco_me_z0);
   fChain->SetBranchAddress("mu_staco_me_phi", &mu_staco_me_phi, &b_mu_staco_me_phi);
   fChain->SetBranchAddress("mu_staco_me_theta", &mu_staco_me_theta, &b_mu_staco_me_theta);
   fChain->SetBranchAddress("mu_staco_me_qoverp", &mu_staco_me_qoverp, &b_mu_staco_me_qoverp);
   fChain->SetBranchAddress("mu_staco_ie_d0", &mu_staco_ie_d0, &b_mu_staco_ie_d0);
   fChain->SetBranchAddress("mu_staco_ie_z0", &mu_staco_ie_z0, &b_mu_staco_ie_z0);
   fChain->SetBranchAddress("mu_staco_ie_phi", &mu_staco_ie_phi, &b_mu_staco_ie_phi);
   fChain->SetBranchAddress("mu_staco_ie_theta", &mu_staco_ie_theta, &b_mu_staco_ie_theta);
   fChain->SetBranchAddress("mu_staco_ie_qoverp", &mu_staco_ie_qoverp, &b_mu_staco_ie_qoverp);
   fChain->SetBranchAddress("mu_staco_nOutliersOnTrack", &mu_staco_nOutliersOnTrack, &b_mu_staco_nOutliersOnTrack);
   fChain->SetBranchAddress("mu_staco_nBLHits", &mu_staco_nBLHits, &b_mu_staco_nBLHits);
   fChain->SetBranchAddress("mu_staco_nPixHits", &mu_staco_nPixHits, &b_mu_staco_nPixHits);
   fChain->SetBranchAddress("mu_staco_nSCTHits", &mu_staco_nSCTHits, &b_mu_staco_nSCTHits);
   fChain->SetBranchAddress("mu_staco_nTRTHits", &mu_staco_nTRTHits, &b_mu_staco_nTRTHits);
   fChain->SetBranchAddress("mu_staco_nTRTHighTHits", &mu_staco_nTRTHighTHits, &b_mu_staco_nTRTHighTHits);
   fChain->SetBranchAddress("mu_staco_nBLSharedHits", &mu_staco_nBLSharedHits, &b_mu_staco_nBLSharedHits);
   fChain->SetBranchAddress("mu_staco_nPixSharedHits", &mu_staco_nPixSharedHits, &b_mu_staco_nPixSharedHits);
   fChain->SetBranchAddress("mu_staco_nPixHoles", &mu_staco_nPixHoles, &b_mu_staco_nPixHoles);
   fChain->SetBranchAddress("mu_staco_nSCTSharedHits", &mu_staco_nSCTSharedHits, &b_mu_staco_nSCTSharedHits);
   fChain->SetBranchAddress("mu_staco_nSCTHoles", &mu_staco_nSCTHoles, &b_mu_staco_nSCTHoles);
   fChain->SetBranchAddress("mu_staco_nTRTOutliers", &mu_staco_nTRTOutliers, &b_mu_staco_nTRTOutliers);
   fChain->SetBranchAddress("mu_staco_nTRTHighTOutliers", &mu_staco_nTRTHighTOutliers, &b_mu_staco_nTRTHighTOutliers);
   fChain->SetBranchAddress("mu_staco_nGangedPixels", &mu_staco_nGangedPixels, &b_mu_staco_nGangedPixels);
   fChain->SetBranchAddress("mu_staco_nPixelDeadSensors", &mu_staco_nPixelDeadSensors, &b_mu_staco_nPixelDeadSensors);
   fChain->SetBranchAddress("mu_staco_nSCTDeadSensors", &mu_staco_nSCTDeadSensors, &b_mu_staco_nSCTDeadSensors);
   fChain->SetBranchAddress("mu_staco_nTRTDeadStraws", &mu_staco_nTRTDeadStraws, &b_mu_staco_nTRTDeadStraws);
   fChain->SetBranchAddress("mu_staco_expectBLayerHit", &mu_staco_expectBLayerHit, &b_mu_staco_expectBLayerHit);
   fChain->SetBranchAddress("mu_staco_nMDTHits", &mu_staco_nMDTHits, &b_mu_staco_nMDTHits);
   fChain->SetBranchAddress("mu_staco_nMDTHoles", &mu_staco_nMDTHoles, &b_mu_staco_nMDTHoles);
   fChain->SetBranchAddress("mu_staco_nCSCEtaHits", &mu_staco_nCSCEtaHits, &b_mu_staco_nCSCEtaHits);
   fChain->SetBranchAddress("mu_staco_nCSCEtaHoles", &mu_staco_nCSCEtaHoles, &b_mu_staco_nCSCEtaHoles);
   fChain->SetBranchAddress("mu_staco_nCSCPhiHits", &mu_staco_nCSCPhiHits, &b_mu_staco_nCSCPhiHits);
   fChain->SetBranchAddress("mu_staco_nCSCPhiHoles", &mu_staco_nCSCPhiHoles, &b_mu_staco_nCSCPhiHoles);
   fChain->SetBranchAddress("mu_staco_nRPCEtaHits", &mu_staco_nRPCEtaHits, &b_mu_staco_nRPCEtaHits);
   fChain->SetBranchAddress("mu_staco_nRPCEtaHoles", &mu_staco_nRPCEtaHoles, &b_mu_staco_nRPCEtaHoles);
   fChain->SetBranchAddress("mu_staco_nRPCPhiHits", &mu_staco_nRPCPhiHits, &b_mu_staco_nRPCPhiHits);
   fChain->SetBranchAddress("mu_staco_nRPCPhiHoles", &mu_staco_nRPCPhiHoles, &b_mu_staco_nRPCPhiHoles);
   fChain->SetBranchAddress("mu_staco_nTGCEtaHits", &mu_staco_nTGCEtaHits, &b_mu_staco_nTGCEtaHits);
   fChain->SetBranchAddress("mu_staco_nTGCEtaHoles", &mu_staco_nTGCEtaHoles, &b_mu_staco_nTGCEtaHoles);
   fChain->SetBranchAddress("mu_staco_nTGCPhiHits", &mu_staco_nTGCPhiHits, &b_mu_staco_nTGCPhiHits);
   fChain->SetBranchAddress("mu_staco_nTGCPhiHoles", &mu_staco_nTGCPhiHoles, &b_mu_staco_nTGCPhiHoles);
   fChain->SetBranchAddress("mu_staco_nMDTBIHits", &mu_staco_nMDTBIHits, &b_mu_staco_nMDTBIHits);
   fChain->SetBranchAddress("mu_staco_nMDTBMHits", &mu_staco_nMDTBMHits, &b_mu_staco_nMDTBMHits);
   fChain->SetBranchAddress("mu_staco_nMDTBOHits", &mu_staco_nMDTBOHits, &b_mu_staco_nMDTBOHits);
   fChain->SetBranchAddress("mu_staco_nMDTBEEHits", &mu_staco_nMDTBEEHits, &b_mu_staco_nMDTBEEHits);
   fChain->SetBranchAddress("mu_staco_nMDTBIS78Hits", &mu_staco_nMDTBIS78Hits, &b_mu_staco_nMDTBIS78Hits);
   fChain->SetBranchAddress("mu_staco_nMDTEIHits", &mu_staco_nMDTEIHits, &b_mu_staco_nMDTEIHits);
   fChain->SetBranchAddress("mu_staco_nMDTEMHits", &mu_staco_nMDTEMHits, &b_mu_staco_nMDTEMHits);
   fChain->SetBranchAddress("mu_staco_nMDTEOHits", &mu_staco_nMDTEOHits, &b_mu_staco_nMDTEOHits);
   fChain->SetBranchAddress("mu_staco_nMDTEEHits", &mu_staco_nMDTEEHits, &b_mu_staco_nMDTEEHits);
   fChain->SetBranchAddress("mu_staco_nRPCLayer1EtaHits", &mu_staco_nRPCLayer1EtaHits, &b_mu_staco_nRPCLayer1EtaHits);
   fChain->SetBranchAddress("mu_staco_nRPCLayer2EtaHits", &mu_staco_nRPCLayer2EtaHits, &b_mu_staco_nRPCLayer2EtaHits);
   fChain->SetBranchAddress("mu_staco_nRPCLayer3EtaHits", &mu_staco_nRPCLayer3EtaHits, &b_mu_staco_nRPCLayer3EtaHits);
   fChain->SetBranchAddress("mu_staco_nRPCLayer1PhiHits", &mu_staco_nRPCLayer1PhiHits, &b_mu_staco_nRPCLayer1PhiHits);
   fChain->SetBranchAddress("mu_staco_nRPCLayer2PhiHits", &mu_staco_nRPCLayer2PhiHits, &b_mu_staco_nRPCLayer2PhiHits);
   fChain->SetBranchAddress("mu_staco_nRPCLayer3PhiHits", &mu_staco_nRPCLayer3PhiHits, &b_mu_staco_nRPCLayer3PhiHits);
   fChain->SetBranchAddress("mu_staco_nTGCLayer1EtaHits", &mu_staco_nTGCLayer1EtaHits, &b_mu_staco_nTGCLayer1EtaHits);
   fChain->SetBranchAddress("mu_staco_nTGCLayer2EtaHits", &mu_staco_nTGCLayer2EtaHits, &b_mu_staco_nTGCLayer2EtaHits);
   fChain->SetBranchAddress("mu_staco_nTGCLayer3EtaHits", &mu_staco_nTGCLayer3EtaHits, &b_mu_staco_nTGCLayer3EtaHits);
   fChain->SetBranchAddress("mu_staco_nTGCLayer4EtaHits", &mu_staco_nTGCLayer4EtaHits, &b_mu_staco_nTGCLayer4EtaHits);
   fChain->SetBranchAddress("mu_staco_nTGCLayer1PhiHits", &mu_staco_nTGCLayer1PhiHits, &b_mu_staco_nTGCLayer1PhiHits);
   fChain->SetBranchAddress("mu_staco_nTGCLayer2PhiHits", &mu_staco_nTGCLayer2PhiHits, &b_mu_staco_nTGCLayer2PhiHits);
   fChain->SetBranchAddress("mu_staco_nTGCLayer3PhiHits", &mu_staco_nTGCLayer3PhiHits, &b_mu_staco_nTGCLayer3PhiHits);
   fChain->SetBranchAddress("mu_staco_nTGCLayer4PhiHits", &mu_staco_nTGCLayer4PhiHits, &b_mu_staco_nTGCLayer4PhiHits);
   fChain->SetBranchAddress("mu_staco_barrelSectors", &mu_staco_barrelSectors, &b_mu_staco_barrelSectors);
   fChain->SetBranchAddress("mu_staco_endcapSectors", &mu_staco_endcapSectors, &b_mu_staco_endcapSectors);
   fChain->SetBranchAddress("mu_staco_trackd0", &mu_staco_trackd0, &b_mu_staco_trackd0);
   fChain->SetBranchAddress("mu_staco_trackz0", &mu_staco_trackz0, &b_mu_staco_trackz0);
   fChain->SetBranchAddress("mu_staco_trackphi", &mu_staco_trackphi, &b_mu_staco_trackphi);
   fChain->SetBranchAddress("mu_staco_tracktheta", &mu_staco_tracktheta, &b_mu_staco_tracktheta);
   fChain->SetBranchAddress("mu_staco_trackqoverp", &mu_staco_trackqoverp, &b_mu_staco_trackqoverp);
   fChain->SetBranchAddress("mu_staco_trackcov_d0", &mu_staco_trackcov_d0, &b_mu_staco_trackcov_d0);
   fChain->SetBranchAddress("mu_staco_trackcov_z0", &mu_staco_trackcov_z0, &b_mu_staco_trackcov_z0);
   fChain->SetBranchAddress("mu_staco_trackcov_phi", &mu_staco_trackcov_phi, &b_mu_staco_trackcov_phi);
   fChain->SetBranchAddress("mu_staco_trackcov_theta", &mu_staco_trackcov_theta, &b_mu_staco_trackcov_theta);
   fChain->SetBranchAddress("mu_staco_trackcov_qoverp", &mu_staco_trackcov_qoverp, &b_mu_staco_trackcov_qoverp);
   fChain->SetBranchAddress("mu_staco_trackcov_d0_z0", &mu_staco_trackcov_d0_z0, &b_mu_staco_trackcov_d0_z0);
   fChain->SetBranchAddress("mu_staco_trackcov_d0_phi", &mu_staco_trackcov_d0_phi, &b_mu_staco_trackcov_d0_phi);
   fChain->SetBranchAddress("mu_staco_trackcov_d0_theta", &mu_staco_trackcov_d0_theta, &b_mu_staco_trackcov_d0_theta);
   fChain->SetBranchAddress("mu_staco_trackcov_d0_qoverp", &mu_staco_trackcov_d0_qoverp, &b_mu_staco_trackcov_d0_qoverp);
   fChain->SetBranchAddress("mu_staco_trackcov_z0_phi", &mu_staco_trackcov_z0_phi, &b_mu_staco_trackcov_z0_phi);
   fChain->SetBranchAddress("mu_staco_trackcov_z0_theta", &mu_staco_trackcov_z0_theta, &b_mu_staco_trackcov_z0_theta);
   fChain->SetBranchAddress("mu_staco_trackcov_z0_qoverp", &mu_staco_trackcov_z0_qoverp, &b_mu_staco_trackcov_z0_qoverp);
   fChain->SetBranchAddress("mu_staco_trackcov_phi_theta", &mu_staco_trackcov_phi_theta, &b_mu_staco_trackcov_phi_theta);
   fChain->SetBranchAddress("mu_staco_trackcov_phi_qoverp", &mu_staco_trackcov_phi_qoverp, &b_mu_staco_trackcov_phi_qoverp);
   fChain->SetBranchAddress("mu_staco_trackcov_theta_qoverp", &mu_staco_trackcov_theta_qoverp, &b_mu_staco_trackcov_theta_qoverp);
   fChain->SetBranchAddress("mu_staco_hastrack", &mu_staco_hastrack, &b_mu_staco_hastrack);
   fChain->SetBranchAddress("mu_staco_truth_dr", &mu_staco_truth_dr, &b_mu_staco_truth_dr);
   fChain->SetBranchAddress("mu_staco_truth_E", &mu_staco_truth_E, &b_mu_staco_truth_E);
   fChain->SetBranchAddress("mu_staco_truth_pt", &mu_staco_truth_pt, &b_mu_staco_truth_pt);
   fChain->SetBranchAddress("mu_staco_truth_eta", &mu_staco_truth_eta, &b_mu_staco_truth_eta);
   fChain->SetBranchAddress("mu_staco_truth_phi", &mu_staco_truth_phi, &b_mu_staco_truth_phi);
   fChain->SetBranchAddress("mu_staco_truth_type", &mu_staco_truth_type, &b_mu_staco_truth_type);
   fChain->SetBranchAddress("mu_staco_truth_status", &mu_staco_truth_status, &b_mu_staco_truth_status);
   fChain->SetBranchAddress("mu_staco_truth_barcode", &mu_staco_truth_barcode, &b_mu_staco_truth_barcode);
   fChain->SetBranchAddress("mu_staco_truth_mothertype", &mu_staco_truth_mothertype, &b_mu_staco_truth_mothertype);
   fChain->SetBranchAddress("mu_staco_truth_motherbarcode", &mu_staco_truth_motherbarcode, &b_mu_staco_truth_motherbarcode);
   fChain->SetBranchAddress("mu_staco_truth_matched", &mu_staco_truth_matched, &b_mu_staco_truth_matched);
   fChain->SetBranchAddress("mu_staco_EFCB_dr", &mu_staco_EFCB_dr, &b_mu_staco_EFCB_dr);
   fChain->SetBranchAddress("mu_staco_EFCB_index", &mu_staco_EFCB_index, &b_mu_staco_EFCB_index);
   fChain->SetBranchAddress("mu_staco_EFMG_dr", &mu_staco_EFMG_dr, &b_mu_staco_EFMG_dr);
   fChain->SetBranchAddress("mu_staco_EFME_dr", &mu_staco_EFME_dr, &b_mu_staco_EFME_dr);
   fChain->SetBranchAddress("mu_staco_EFME_index", &mu_staco_EFME_index, &b_mu_staco_EFME_index);
   fChain->SetBranchAddress("MET_Simplified20_RefGamma_etx", &MET_Simplified20_RefGamma_etx, &b_MET_Simplified20_RefGamma_etx);
   fChain->SetBranchAddress("MET_Simplified20_RefGamma_ety", &MET_Simplified20_RefGamma_ety, &b_MET_Simplified20_RefGamma_ety);
   fChain->SetBranchAddress("MET_Simplified20_RefGamma_sumet", &MET_Simplified20_RefGamma_sumet, &b_MET_Simplified20_RefGamma_sumet);
   fChain->SetBranchAddress("MET_Simplified20_RefGamma_etx_CentralReg", &MET_Simplified20_RefGamma_etx_CentralReg, &b_MET_Simplified20_RefGamma_etx_CentralReg);
   fChain->SetBranchAddress("MET_Simplified20_RefGamma_ety_CentralReg", &MET_Simplified20_RefGamma_ety_CentralReg, &b_MET_Simplified20_RefGamma_ety_CentralReg);
   fChain->SetBranchAddress("MET_Simplified20_RefGamma_sumet_CentralReg", &MET_Simplified20_RefGamma_sumet_CentralReg, &b_MET_Simplified20_RefGamma_sumet_CentralReg);
   fChain->SetBranchAddress("MET_Simplified20_RefGamma_phi_CentralReg", &MET_Simplified20_RefGamma_phi_CentralReg, &b_MET_Simplified20_RefGamma_phi_CentralReg);
   fChain->SetBranchAddress("MET_Simplified20_RefGamma_etx_EndcapRegion", &MET_Simplified20_RefGamma_etx_EndcapRegion, &b_MET_Simplified20_RefGamma_etx_EndcapRegion);
   fChain->SetBranchAddress("MET_Simplified20_RefGamma_ety_EndcapRegion", &MET_Simplified20_RefGamma_ety_EndcapRegion, &b_MET_Simplified20_RefGamma_ety_EndcapRegion);
   fChain->SetBranchAddress("MET_Simplified20_RefGamma_sumet_EndcapRegion", &MET_Simplified20_RefGamma_sumet_EndcapRegion, &b_MET_Simplified20_RefGamma_sumet_EndcapRegion);
   fChain->SetBranchAddress("MET_Simplified20_RefGamma_phi_EndcapRegion", &MET_Simplified20_RefGamma_phi_EndcapRegion, &b_MET_Simplified20_RefGamma_phi_EndcapRegion);
   fChain->SetBranchAddress("MET_Simplified20_RefGamma_etx_ForwardReg", &MET_Simplified20_RefGamma_etx_ForwardReg, &b_MET_Simplified20_RefGamma_etx_ForwardReg);
   fChain->SetBranchAddress("MET_Simplified20_RefGamma_ety_ForwardReg", &MET_Simplified20_RefGamma_ety_ForwardReg, &b_MET_Simplified20_RefGamma_ety_ForwardReg);
   fChain->SetBranchAddress("MET_Simplified20_RefGamma_sumet_ForwardReg", &MET_Simplified20_RefGamma_sumet_ForwardReg, &b_MET_Simplified20_RefGamma_sumet_ForwardReg);
   fChain->SetBranchAddress("MET_Simplified20_RefGamma_phi_ForwardReg", &MET_Simplified20_RefGamma_phi_ForwardReg, &b_MET_Simplified20_RefGamma_phi_ForwardReg);
   fChain->SetBranchAddress("MET_Simplified20_RefFinal_etx", &MET_Simplified20_RefFinal_etx, &b_MET_Simplified20_RefFinal_etx);
   fChain->SetBranchAddress("MET_Simplified20_RefFinal_ety", &MET_Simplified20_RefFinal_ety, &b_MET_Simplified20_RefFinal_ety);
   fChain->SetBranchAddress("MET_Simplified20_RefFinal_sumet", &MET_Simplified20_RefFinal_sumet, &b_MET_Simplified20_RefFinal_sumet);
   fChain->SetBranchAddress("MET_Simplified20_RefFinal_etx_CentralReg", &MET_Simplified20_RefFinal_etx_CentralReg, &b_MET_Simplified20_RefFinal_etx_CentralReg);
   fChain->SetBranchAddress("MET_Simplified20_RefFinal_ety_CentralReg", &MET_Simplified20_RefFinal_ety_CentralReg, &b_MET_Simplified20_RefFinal_ety_CentralReg);
   fChain->SetBranchAddress("MET_Simplified20_RefFinal_sumet_CentralReg", &MET_Simplified20_RefFinal_sumet_CentralReg, &b_MET_Simplified20_RefFinal_sumet_CentralReg);
   fChain->SetBranchAddress("MET_Simplified20_RefFinal_phi_CentralReg", &MET_Simplified20_RefFinal_phi_CentralReg, &b_MET_Simplified20_RefFinal_phi_CentralReg);
   fChain->SetBranchAddress("MET_Simplified20_RefFinal_etx_EndcapRegion", &MET_Simplified20_RefFinal_etx_EndcapRegion, &b_MET_Simplified20_RefFinal_etx_EndcapRegion);
   fChain->SetBranchAddress("MET_Simplified20_RefFinal_ety_EndcapRegion", &MET_Simplified20_RefFinal_ety_EndcapRegion, &b_MET_Simplified20_RefFinal_ety_EndcapRegion);
   fChain->SetBranchAddress("MET_Simplified20_RefFinal_sumet_EndcapRegion", &MET_Simplified20_RefFinal_sumet_EndcapRegion, &b_MET_Simplified20_RefFinal_sumet_EndcapRegion);
   fChain->SetBranchAddress("MET_Simplified20_RefFinal_phi_EndcapRegion", &MET_Simplified20_RefFinal_phi_EndcapRegion, &b_MET_Simplified20_RefFinal_phi_EndcapRegion);
   fChain->SetBranchAddress("MET_Simplified20_RefFinal_etx_ForwardReg", &MET_Simplified20_RefFinal_etx_ForwardReg, &b_MET_Simplified20_RefFinal_etx_ForwardReg);
   fChain->SetBranchAddress("MET_Simplified20_RefFinal_ety_ForwardReg", &MET_Simplified20_RefFinal_ety_ForwardReg, &b_MET_Simplified20_RefFinal_ety_ForwardReg);
   fChain->SetBranchAddress("MET_Simplified20_RefFinal_sumet_ForwardReg", &MET_Simplified20_RefFinal_sumet_ForwardReg, &b_MET_Simplified20_RefFinal_sumet_ForwardReg);
   fChain->SetBranchAddress("MET_Simplified20_RefFinal_phi_ForwardReg", &MET_Simplified20_RefFinal_phi_ForwardReg, &b_MET_Simplified20_RefFinal_phi_ForwardReg);
   fChain->SetBranchAddress("MET_Simplified20_RefJet_etx", &MET_Simplified20_RefJet_etx, &b_MET_Simplified20_RefJet_etx);
   fChain->SetBranchAddress("MET_Simplified20_RefJet_ety", &MET_Simplified20_RefJet_ety, &b_MET_Simplified20_RefJet_ety);
   fChain->SetBranchAddress("MET_Simplified20_RefJet_sumet", &MET_Simplified20_RefJet_sumet, &b_MET_Simplified20_RefJet_sumet);
   fChain->SetBranchAddress("MET_Simplified20_RefJet_etx_CentralReg", &MET_Simplified20_RefJet_etx_CentralReg, &b_MET_Simplified20_RefJet_etx_CentralReg);
   fChain->SetBranchAddress("MET_Simplified20_RefJet_ety_CentralReg", &MET_Simplified20_RefJet_ety_CentralReg, &b_MET_Simplified20_RefJet_ety_CentralReg);
   fChain->SetBranchAddress("MET_Simplified20_RefJet_sumet_CentralReg", &MET_Simplified20_RefJet_sumet_CentralReg, &b_MET_Simplified20_RefJet_sumet_CentralReg);
   fChain->SetBranchAddress("MET_Simplified20_RefJet_phi_CentralReg", &MET_Simplified20_RefJet_phi_CentralReg, &b_MET_Simplified20_RefJet_phi_CentralReg);
   fChain->SetBranchAddress("MET_Simplified20_RefJet_etx_EndcapRegion", &MET_Simplified20_RefJet_etx_EndcapRegion, &b_MET_Simplified20_RefJet_etx_EndcapRegion);
   fChain->SetBranchAddress("MET_Simplified20_RefJet_ety_EndcapRegion", &MET_Simplified20_RefJet_ety_EndcapRegion, &b_MET_Simplified20_RefJet_ety_EndcapRegion);
   fChain->SetBranchAddress("MET_Simplified20_RefJet_sumet_EndcapRegion", &MET_Simplified20_RefJet_sumet_EndcapRegion, &b_MET_Simplified20_RefJet_sumet_EndcapRegion);
   fChain->SetBranchAddress("MET_Simplified20_RefJet_phi_EndcapRegion", &MET_Simplified20_RefJet_phi_EndcapRegion, &b_MET_Simplified20_RefJet_phi_EndcapRegion);
   fChain->SetBranchAddress("MET_Simplified20_RefJet_etx_ForwardReg", &MET_Simplified20_RefJet_etx_ForwardReg, &b_MET_Simplified20_RefJet_etx_ForwardReg);
   fChain->SetBranchAddress("MET_Simplified20_RefJet_ety_ForwardReg", &MET_Simplified20_RefJet_ety_ForwardReg, &b_MET_Simplified20_RefJet_ety_ForwardReg);
   fChain->SetBranchAddress("MET_Simplified20_RefJet_sumet_ForwardReg", &MET_Simplified20_RefJet_sumet_ForwardReg, &b_MET_Simplified20_RefJet_sumet_ForwardReg);
   fChain->SetBranchAddress("MET_Simplified20_RefJet_phi_ForwardReg", &MET_Simplified20_RefJet_phi_ForwardReg, &b_MET_Simplified20_RefJet_phi_ForwardReg);
   fChain->SetBranchAddress("MET_Simplified20_RefEle_etx", &MET_Simplified20_RefEle_etx, &b_MET_Simplified20_RefEle_etx);
   fChain->SetBranchAddress("MET_Simplified20_RefEle_ety", &MET_Simplified20_RefEle_ety, &b_MET_Simplified20_RefEle_ety);
   fChain->SetBranchAddress("MET_Simplified20_RefEle_sumet", &MET_Simplified20_RefEle_sumet, &b_MET_Simplified20_RefEle_sumet);
   fChain->SetBranchAddress("MET_Simplified20_RefEle_etx_CentralReg", &MET_Simplified20_RefEle_etx_CentralReg, &b_MET_Simplified20_RefEle_etx_CentralReg);
   fChain->SetBranchAddress("MET_Simplified20_RefEle_ety_CentralReg", &MET_Simplified20_RefEle_ety_CentralReg, &b_MET_Simplified20_RefEle_ety_CentralReg);
   fChain->SetBranchAddress("MET_Simplified20_RefEle_sumet_CentralReg", &MET_Simplified20_RefEle_sumet_CentralReg, &b_MET_Simplified20_RefEle_sumet_CentralReg);
   fChain->SetBranchAddress("MET_Simplified20_RefEle_phi_CentralReg", &MET_Simplified20_RefEle_phi_CentralReg, &b_MET_Simplified20_RefEle_phi_CentralReg);
   fChain->SetBranchAddress("MET_Simplified20_RefEle_etx_EndcapRegion", &MET_Simplified20_RefEle_etx_EndcapRegion, &b_MET_Simplified20_RefEle_etx_EndcapRegion);
   fChain->SetBranchAddress("MET_Simplified20_RefEle_ety_EndcapRegion", &MET_Simplified20_RefEle_ety_EndcapRegion, &b_MET_Simplified20_RefEle_ety_EndcapRegion);
   fChain->SetBranchAddress("MET_Simplified20_RefEle_sumet_EndcapRegion", &MET_Simplified20_RefEle_sumet_EndcapRegion, &b_MET_Simplified20_RefEle_sumet_EndcapRegion);
   fChain->SetBranchAddress("MET_Simplified20_RefEle_phi_EndcapRegion", &MET_Simplified20_RefEle_phi_EndcapRegion, &b_MET_Simplified20_RefEle_phi_EndcapRegion);
   fChain->SetBranchAddress("MET_Simplified20_RefEle_etx_ForwardReg", &MET_Simplified20_RefEle_etx_ForwardReg, &b_MET_Simplified20_RefEle_etx_ForwardReg);
   fChain->SetBranchAddress("MET_Simplified20_RefEle_ety_ForwardReg", &MET_Simplified20_RefEle_ety_ForwardReg, &b_MET_Simplified20_RefEle_ety_ForwardReg);
   fChain->SetBranchAddress("MET_Simplified20_RefEle_sumet_ForwardReg", &MET_Simplified20_RefEle_sumet_ForwardReg, &b_MET_Simplified20_RefEle_sumet_ForwardReg);
   fChain->SetBranchAddress("MET_Simplified20_RefEle_phi_ForwardReg", &MET_Simplified20_RefEle_phi_ForwardReg, &b_MET_Simplified20_RefEle_phi_ForwardReg);
   fChain->SetBranchAddress("MET_Simplified20_Muon_Total_Staco_etx", &MET_Simplified20_Muon_Total_Staco_etx, &b_MET_Simplified20_Muon_Total_Staco_etx);
   fChain->SetBranchAddress("MET_Simplified20_Muon_Total_Staco_ety", &MET_Simplified20_Muon_Total_Staco_ety, &b_MET_Simplified20_Muon_Total_Staco_ety);
   fChain->SetBranchAddress("MET_Simplified20_Muon_Total_Staco_sumet", &MET_Simplified20_Muon_Total_Staco_sumet, &b_MET_Simplified20_Muon_Total_Staco_sumet);
   fChain->SetBranchAddress("MET_Simplified20_Muon_Total_Staco_etx_CentralReg", &MET_Simplified20_Muon_Total_Staco_etx_CentralReg, &b_MET_Simplified20_Muon_Total_Staco_etx_CentralReg);
   fChain->SetBranchAddress("MET_Simplified20_Muon_Total_Staco_ety_CentralReg", &MET_Simplified20_Muon_Total_Staco_ety_CentralReg, &b_MET_Simplified20_Muon_Total_Staco_ety_CentralReg);
   fChain->SetBranchAddress("MET_Simplified20_Muon_Total_Staco_sumet_CentralReg", &MET_Simplified20_Muon_Total_Staco_sumet_CentralReg, &b_MET_Simplified20_Muon_Total_Staco_sumet_CentralReg);
   fChain->SetBranchAddress("MET_Simplified20_Muon_Total_Staco_phi_CentralReg", &MET_Simplified20_Muon_Total_Staco_phi_CentralReg, &b_MET_Simplified20_Muon_Total_Staco_phi_CentralReg);
   fChain->SetBranchAddress("MET_Simplified20_Muon_Total_Staco_etx_EndcapRegion", &MET_Simplified20_Muon_Total_Staco_etx_EndcapRegion, &b_MET_Simplified20_Muon_Total_Staco_etx_EndcapRegion);
   fChain->SetBranchAddress("MET_Simplified20_Muon_Total_Staco_ety_EndcapRegion", &MET_Simplified20_Muon_Total_Staco_ety_EndcapRegion, &b_MET_Simplified20_Muon_Total_Staco_ety_EndcapRegion);
   fChain->SetBranchAddress("MET_Simplified20_Muon_Total_Staco_sumet_EndcapRegion", &MET_Simplified20_Muon_Total_Staco_sumet_EndcapRegion, &b_MET_Simplified20_Muon_Total_Staco_sumet_EndcapRegion);
   fChain->SetBranchAddress("MET_Simplified20_Muon_Total_Staco_phi_EndcapRegion", &MET_Simplified20_Muon_Total_Staco_phi_EndcapRegion, &b_MET_Simplified20_Muon_Total_Staco_phi_EndcapRegion);
   fChain->SetBranchAddress("MET_Simplified20_Muon_Total_Staco_etx_ForwardReg", &MET_Simplified20_Muon_Total_Staco_etx_ForwardReg, &b_MET_Simplified20_Muon_Total_Staco_etx_ForwardReg);
   fChain->SetBranchAddress("MET_Simplified20_Muon_Total_Staco_ety_ForwardReg", &MET_Simplified20_Muon_Total_Staco_ety_ForwardReg, &b_MET_Simplified20_Muon_Total_Staco_ety_ForwardReg);
   fChain->SetBranchAddress("MET_Simplified20_Muon_Total_Staco_sumet_ForwardReg", &MET_Simplified20_Muon_Total_Staco_sumet_ForwardReg, &b_MET_Simplified20_Muon_Total_Staco_sumet_ForwardReg);
   fChain->SetBranchAddress("MET_Simplified20_Muon_Total_Staco_phi_ForwardReg", &MET_Simplified20_Muon_Total_Staco_phi_ForwardReg, &b_MET_Simplified20_Muon_Total_Staco_phi_ForwardReg);
   fChain->SetBranchAddress("MET_Simplified20_Muon_Isol_Staco_etx", &MET_Simplified20_Muon_Isol_Staco_etx, &b_MET_Simplified20_Muon_Isol_Staco_etx);
   fChain->SetBranchAddress("MET_Simplified20_Muon_Isol_Staco_ety", &MET_Simplified20_Muon_Isol_Staco_ety, &b_MET_Simplified20_Muon_Isol_Staco_ety);
   fChain->SetBranchAddress("MET_Simplified20_Muon_Isol_Staco_sumet", &MET_Simplified20_Muon_Isol_Staco_sumet, &b_MET_Simplified20_Muon_Isol_Staco_sumet);
   fChain->SetBranchAddress("MET_Simplified20_Muon_Isol_Staco_etx_CentralReg", &MET_Simplified20_Muon_Isol_Staco_etx_CentralReg, &b_MET_Simplified20_Muon_Isol_Staco_etx_CentralReg);
   fChain->SetBranchAddress("MET_Simplified20_Muon_Isol_Staco_ety_CentralReg", &MET_Simplified20_Muon_Isol_Staco_ety_CentralReg, &b_MET_Simplified20_Muon_Isol_Staco_ety_CentralReg);
   fChain->SetBranchAddress("MET_Simplified20_Muon_Isol_Staco_sumet_CentralReg", &MET_Simplified20_Muon_Isol_Staco_sumet_CentralReg, &b_MET_Simplified20_Muon_Isol_Staco_sumet_CentralReg);
   fChain->SetBranchAddress("MET_Simplified20_Muon_Isol_Staco_phi_CentralReg", &MET_Simplified20_Muon_Isol_Staco_phi_CentralReg, &b_MET_Simplified20_Muon_Isol_Staco_phi_CentralReg);
   fChain->SetBranchAddress("MET_Simplified20_Muon_Isol_Staco_etx_EndcapRegion", &MET_Simplified20_Muon_Isol_Staco_etx_EndcapRegion, &b_MET_Simplified20_Muon_Isol_Staco_etx_EndcapRegion);
   fChain->SetBranchAddress("MET_Simplified20_Muon_Isol_Staco_ety_EndcapRegion", &MET_Simplified20_Muon_Isol_Staco_ety_EndcapRegion, &b_MET_Simplified20_Muon_Isol_Staco_ety_EndcapRegion);
   fChain->SetBranchAddress("MET_Simplified20_Muon_Isol_Staco_sumet_EndcapRegion", &MET_Simplified20_Muon_Isol_Staco_sumet_EndcapRegion, &b_MET_Simplified20_Muon_Isol_Staco_sumet_EndcapRegion);
   fChain->SetBranchAddress("MET_Simplified20_Muon_Isol_Staco_phi_EndcapRegion", &MET_Simplified20_Muon_Isol_Staco_phi_EndcapRegion, &b_MET_Simplified20_Muon_Isol_Staco_phi_EndcapRegion);
   fChain->SetBranchAddress("MET_Simplified20_Muon_Isol_Staco_etx_ForwardReg", &MET_Simplified20_Muon_Isol_Staco_etx_ForwardReg, &b_MET_Simplified20_Muon_Isol_Staco_etx_ForwardReg);
   fChain->SetBranchAddress("MET_Simplified20_Muon_Isol_Staco_ety_ForwardReg", &MET_Simplified20_Muon_Isol_Staco_ety_ForwardReg, &b_MET_Simplified20_Muon_Isol_Staco_ety_ForwardReg);
   fChain->SetBranchAddress("MET_Simplified20_Muon_Isol_Staco_sumet_ForwardReg", &MET_Simplified20_Muon_Isol_Staco_sumet_ForwardReg, &b_MET_Simplified20_Muon_Isol_Staco_sumet_ForwardReg);
   fChain->SetBranchAddress("MET_Simplified20_Muon_Isol_Staco_phi_ForwardReg", &MET_Simplified20_Muon_Isol_Staco_phi_ForwardReg, &b_MET_Simplified20_Muon_Isol_Staco_phi_ForwardReg);
   fChain->SetBranchAddress("MET_Simplified20_Muon_NonIsol_Staco_etx", &MET_Simplified20_Muon_NonIsol_Staco_etx, &b_MET_Simplified20_Muon_NonIsol_Staco_etx);
   fChain->SetBranchAddress("MET_Simplified20_Muon_NonIsol_Staco_ety", &MET_Simplified20_Muon_NonIsol_Staco_ety, &b_MET_Simplified20_Muon_NonIsol_Staco_ety);
   fChain->SetBranchAddress("MET_Simplified20_Muon_NonIsol_Staco_sumet", &MET_Simplified20_Muon_NonIsol_Staco_sumet, &b_MET_Simplified20_Muon_NonIsol_Staco_sumet);
   fChain->SetBranchAddress("MET_Simplified20_Muon_NonIsol_Staco_etx_CentralReg", &MET_Simplified20_Muon_NonIsol_Staco_etx_CentralReg, &b_MET_Simplified20_Muon_NonIsol_Staco_etx_CentralReg);
   fChain->SetBranchAddress("MET_Simplified20_Muon_NonIsol_Staco_ety_CentralReg", &MET_Simplified20_Muon_NonIsol_Staco_ety_CentralReg, &b_MET_Simplified20_Muon_NonIsol_Staco_ety_CentralReg);
   fChain->SetBranchAddress("MET_Simplified20_Muon_NonIsol_Staco_sumet_CentralReg", &MET_Simplified20_Muon_NonIsol_Staco_sumet_CentralReg, &b_MET_Simplified20_Muon_NonIsol_Staco_sumet_CentralReg);
   fChain->SetBranchAddress("MET_Simplified20_Muon_NonIsol_Staco_phi_CentralReg", &MET_Simplified20_Muon_NonIsol_Staco_phi_CentralReg, &b_MET_Simplified20_Muon_NonIsol_Staco_phi_CentralReg);
   fChain->SetBranchAddress("MET_Simplified20_Muon_NonIsol_Staco_etx_EndcapRegion", &MET_Simplified20_Muon_NonIsol_Staco_etx_EndcapRegion, &b_MET_Simplified20_Muon_NonIsol_Staco_etx_EndcapRegion);
   fChain->SetBranchAddress("MET_Simplified20_Muon_NonIsol_Staco_ety_EndcapRegion", &MET_Simplified20_Muon_NonIsol_Staco_ety_EndcapRegion, &b_MET_Simplified20_Muon_NonIsol_Staco_ety_EndcapRegion);
   fChain->SetBranchAddress("MET_Simplified20_Muon_NonIsol_Staco_sumet_EndcapRegion", &MET_Simplified20_Muon_NonIsol_Staco_sumet_EndcapRegion, &b_MET_Simplified20_Muon_NonIsol_Staco_sumet_EndcapRegion);
   fChain->SetBranchAddress("MET_Simplified20_Muon_NonIsol_Staco_phi_EndcapRegion", &MET_Simplified20_Muon_NonIsol_Staco_phi_EndcapRegion, &b_MET_Simplified20_Muon_NonIsol_Staco_phi_EndcapRegion);
   fChain->SetBranchAddress("MET_Simplified20_Muon_NonIsol_Staco_etx_ForwardReg", &MET_Simplified20_Muon_NonIsol_Staco_etx_ForwardReg, &b_MET_Simplified20_Muon_NonIsol_Staco_etx_ForwardReg);
   fChain->SetBranchAddress("MET_Simplified20_Muon_NonIsol_Staco_ety_ForwardReg", &MET_Simplified20_Muon_NonIsol_Staco_ety_ForwardReg, &b_MET_Simplified20_Muon_NonIsol_Staco_ety_ForwardReg);
   fChain->SetBranchAddress("MET_Simplified20_Muon_NonIsol_Staco_sumet_ForwardReg", &MET_Simplified20_Muon_NonIsol_Staco_sumet_ForwardReg, &b_MET_Simplified20_Muon_NonIsol_Staco_sumet_ForwardReg);
   fChain->SetBranchAddress("MET_Simplified20_Muon_NonIsol_Staco_phi_ForwardReg", &MET_Simplified20_Muon_NonIsol_Staco_phi_ForwardReg, &b_MET_Simplified20_Muon_NonIsol_Staco_phi_ForwardReg);
   fChain->SetBranchAddress("MET_Simplified20_CellOut_etx", &MET_Simplified20_CellOut_etx, &b_MET_Simplified20_CellOut_etx);
   fChain->SetBranchAddress("MET_Simplified20_CellOut_ety", &MET_Simplified20_CellOut_ety, &b_MET_Simplified20_CellOut_ety);
   fChain->SetBranchAddress("MET_Simplified20_CellOut_sumet", &MET_Simplified20_CellOut_sumet, &b_MET_Simplified20_CellOut_sumet);
   fChain->SetBranchAddress("MET_Simplified20_CellOut_etx_CentralReg", &MET_Simplified20_CellOut_etx_CentralReg, &b_MET_Simplified20_CellOut_etx_CentralReg);
   fChain->SetBranchAddress("MET_Simplified20_CellOut_ety_CentralReg", &MET_Simplified20_CellOut_ety_CentralReg, &b_MET_Simplified20_CellOut_ety_CentralReg);
   fChain->SetBranchAddress("MET_Simplified20_CellOut_sumet_CentralReg", &MET_Simplified20_CellOut_sumet_CentralReg, &b_MET_Simplified20_CellOut_sumet_CentralReg);
   fChain->SetBranchAddress("MET_Simplified20_CellOut_phi_CentralReg", &MET_Simplified20_CellOut_phi_CentralReg, &b_MET_Simplified20_CellOut_phi_CentralReg);
   fChain->SetBranchAddress("MET_Simplified20_CellOut_etx_EndcapRegion", &MET_Simplified20_CellOut_etx_EndcapRegion, &b_MET_Simplified20_CellOut_etx_EndcapRegion);
   fChain->SetBranchAddress("MET_Simplified20_CellOut_ety_EndcapRegion", &MET_Simplified20_CellOut_ety_EndcapRegion, &b_MET_Simplified20_CellOut_ety_EndcapRegion);
   fChain->SetBranchAddress("MET_Simplified20_CellOut_sumet_EndcapRegion", &MET_Simplified20_CellOut_sumet_EndcapRegion, &b_MET_Simplified20_CellOut_sumet_EndcapRegion);
   fChain->SetBranchAddress("MET_Simplified20_CellOut_phi_EndcapRegion", &MET_Simplified20_CellOut_phi_EndcapRegion, &b_MET_Simplified20_CellOut_phi_EndcapRegion);
   fChain->SetBranchAddress("MET_Simplified20_CellOut_etx_ForwardReg", &MET_Simplified20_CellOut_etx_ForwardReg, &b_MET_Simplified20_CellOut_etx_ForwardReg);
   fChain->SetBranchAddress("MET_Simplified20_CellOut_ety_ForwardReg", &MET_Simplified20_CellOut_ety_ForwardReg, &b_MET_Simplified20_CellOut_ety_ForwardReg);
   fChain->SetBranchAddress("MET_Simplified20_CellOut_sumet_ForwardReg", &MET_Simplified20_CellOut_sumet_ForwardReg, &b_MET_Simplified20_CellOut_sumet_ForwardReg);
   fChain->SetBranchAddress("MET_Simplified20_CellOut_phi_ForwardReg", &MET_Simplified20_CellOut_phi_ForwardReg, &b_MET_Simplified20_CellOut_phi_ForwardReg);
   fChain->SetBranchAddress("MET_Simplified20_CellOut_Eflow_etx", &MET_Simplified20_CellOut_Eflow_etx, &b_MET_Simplified20_CellOut_Eflow_etx);
   fChain->SetBranchAddress("MET_Simplified20_CellOut_Eflow_ety", &MET_Simplified20_CellOut_Eflow_ety, &b_MET_Simplified20_CellOut_Eflow_ety);
   fChain->SetBranchAddress("MET_Simplified20_CellOut_Eflow_sumet", &MET_Simplified20_CellOut_Eflow_sumet, &b_MET_Simplified20_CellOut_Eflow_sumet);
   fChain->SetBranchAddress("MET_Simplified20_CellOut_Eflow_etx_CentralReg", &MET_Simplified20_CellOut_Eflow_etx_CentralReg, &b_MET_Simplified20_CellOut_Eflow_etx_CentralReg);
   fChain->SetBranchAddress("MET_Simplified20_CellOut_Eflow_ety_CentralReg", &MET_Simplified20_CellOut_Eflow_ety_CentralReg, &b_MET_Simplified20_CellOut_Eflow_ety_CentralReg);
   fChain->SetBranchAddress("MET_Simplified20_CellOut_Eflow_sumet_CentralReg", &MET_Simplified20_CellOut_Eflow_sumet_CentralReg, &b_MET_Simplified20_CellOut_Eflow_sumet_CentralReg);
   fChain->SetBranchAddress("MET_Simplified20_CellOut_Eflow_phi_CentralReg", &MET_Simplified20_CellOut_Eflow_phi_CentralReg, &b_MET_Simplified20_CellOut_Eflow_phi_CentralReg);
   fChain->SetBranchAddress("MET_Simplified20_CellOut_Eflow_etx_EndcapRegion", &MET_Simplified20_CellOut_Eflow_etx_EndcapRegion, &b_MET_Simplified20_CellOut_Eflow_etx_EndcapRegion);
   fChain->SetBranchAddress("MET_Simplified20_CellOut_Eflow_ety_EndcapRegion", &MET_Simplified20_CellOut_Eflow_ety_EndcapRegion, &b_MET_Simplified20_CellOut_Eflow_ety_EndcapRegion);
   fChain->SetBranchAddress("MET_Simplified20_CellOut_Eflow_sumet_EndcapRegion", &MET_Simplified20_CellOut_Eflow_sumet_EndcapRegion, &b_MET_Simplified20_CellOut_Eflow_sumet_EndcapRegion);
   fChain->SetBranchAddress("MET_Simplified20_CellOut_Eflow_phi_EndcapRegion", &MET_Simplified20_CellOut_Eflow_phi_EndcapRegion, &b_MET_Simplified20_CellOut_Eflow_phi_EndcapRegion);
   fChain->SetBranchAddress("MET_Simplified20_CellOut_Eflow_etx_ForwardReg", &MET_Simplified20_CellOut_Eflow_etx_ForwardReg, &b_MET_Simplified20_CellOut_Eflow_etx_ForwardReg);
   fChain->SetBranchAddress("MET_Simplified20_CellOut_Eflow_ety_ForwardReg", &MET_Simplified20_CellOut_Eflow_ety_ForwardReg, &b_MET_Simplified20_CellOut_Eflow_ety_ForwardReg);
   fChain->SetBranchAddress("MET_Simplified20_CellOut_Eflow_sumet_ForwardReg", &MET_Simplified20_CellOut_Eflow_sumet_ForwardReg, &b_MET_Simplified20_CellOut_Eflow_sumet_ForwardReg);
   fChain->SetBranchAddress("MET_Simplified20_CellOut_Eflow_phi_ForwardReg", &MET_Simplified20_CellOut_Eflow_phi_ForwardReg, &b_MET_Simplified20_CellOut_Eflow_phi_ForwardReg);
   fChain->SetBranchAddress("el_MET_Simplified20_n", &el_MET_Simplified20_n, &b_el_MET_Simplified20_n);
   fChain->SetBranchAddress("el_MET_Simplified20_wpx", &el_MET_Simplified20_wpx, &b_el_MET_Simplified20_wpx);
   fChain->SetBranchAddress("el_MET_Simplified20_wpy", &el_MET_Simplified20_wpy, &b_el_MET_Simplified20_wpy);
   fChain->SetBranchAddress("el_MET_Simplified20_wet", &el_MET_Simplified20_wet, &b_el_MET_Simplified20_wet);
   fChain->SetBranchAddress("el_MET_Simplified20_statusWord", &el_MET_Simplified20_statusWord, &b_el_MET_Simplified20_statusWord);
   fChain->SetBranchAddress("mu_staco_MET_Simplified20_n", &mu_staco_MET_Simplified20_n, &b_mu_staco_MET_Simplified20_n);
   fChain->SetBranchAddress("mu_staco_MET_Simplified20_wpx", &mu_staco_MET_Simplified20_wpx, &b_mu_staco_MET_Simplified20_wpx);
   fChain->SetBranchAddress("mu_staco_MET_Simplified20_wpy", &mu_staco_MET_Simplified20_wpy, &b_mu_staco_MET_Simplified20_wpy);
   fChain->SetBranchAddress("mu_staco_MET_Simplified20_wet", &mu_staco_MET_Simplified20_wet, &b_mu_staco_MET_Simplified20_wet);
   fChain->SetBranchAddress("mu_staco_MET_Simplified20_statusWord", &mu_staco_MET_Simplified20_statusWord, &b_mu_staco_MET_Simplified20_statusWord);
   fChain->SetBranchAddress("mu_muid_MET_Simplified20_n", &mu_muid_MET_Simplified20_n, &b_mu_muid_MET_Simplified20_n);
   fChain->SetBranchAddress("mu_muid_MET_Simplified20_wpx", &mu_muid_MET_Simplified20_wpx, &b_mu_muid_MET_Simplified20_wpx);
   fChain->SetBranchAddress("mu_muid_MET_Simplified20_wpy", &mu_muid_MET_Simplified20_wpy, &b_mu_muid_MET_Simplified20_wpy);
   fChain->SetBranchAddress("mu_muid_MET_Simplified20_wet", &mu_muid_MET_Simplified20_wet, &b_mu_muid_MET_Simplified20_wet);
   fChain->SetBranchAddress("mu_muid_MET_Simplified20_statusWord", &mu_muid_MET_Simplified20_statusWord, &b_mu_muid_MET_Simplified20_statusWord);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_MET_Simplified20_n", &jet_AntiKt4TopoNewEM_MET_Simplified20_n, &b_jet_AntiKt4TopoNewEM_MET_Simplified20_n);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_MET_Simplified20_wpx", &jet_AntiKt4TopoNewEM_MET_Simplified20_wpx, &b_jet_AntiKt4TopoNewEM_MET_Simplified20_wpx);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_MET_Simplified20_wpy", &jet_AntiKt4TopoNewEM_MET_Simplified20_wpy, &b_jet_AntiKt4TopoNewEM_MET_Simplified20_wpy);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_MET_Simplified20_wet", &jet_AntiKt4TopoNewEM_MET_Simplified20_wet, &b_jet_AntiKt4TopoNewEM_MET_Simplified20_wet);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_MET_Simplified20_statusWord", &jet_AntiKt4TopoNewEM_MET_Simplified20_statusWord, &b_jet_AntiKt4TopoNewEM_MET_Simplified20_statusWord);
   fChain->SetBranchAddress("el_MET_n", &el_MET_n, &b_el_MET_n);
   fChain->SetBranchAddress("el_MET_wpx", &el_MET_wpx, &b_el_MET_wpx);
   fChain->SetBranchAddress("el_MET_wpy", &el_MET_wpy, &b_el_MET_wpy);
   fChain->SetBranchAddress("el_MET_wet", &el_MET_wet, &b_el_MET_wet);
   fChain->SetBranchAddress("el_MET_statusWord", &el_MET_statusWord, &b_el_MET_statusWord);
   fChain->SetBranchAddress("ph_MET_n", &ph_MET_n, &b_ph_MET_n);
   fChain->SetBranchAddress("ph_MET_wpx", &ph_MET_wpx, &b_ph_MET_wpx);
   fChain->SetBranchAddress("ph_MET_wpy", &ph_MET_wpy, &b_ph_MET_wpy);
   fChain->SetBranchAddress("ph_MET_wet", &ph_MET_wet, &b_ph_MET_wet);
   fChain->SetBranchAddress("ph_MET_statusWord", &ph_MET_statusWord, &b_ph_MET_statusWord);
   fChain->SetBranchAddress("mu_staco_MET_n", &mu_staco_MET_n, &b_mu_staco_MET_n);
   fChain->SetBranchAddress("mu_staco_MET_wpx", &mu_staco_MET_wpx, &b_mu_staco_MET_wpx);
   fChain->SetBranchAddress("mu_staco_MET_wpy", &mu_staco_MET_wpy, &b_mu_staco_MET_wpy);
   fChain->SetBranchAddress("mu_staco_MET_wet", &mu_staco_MET_wet, &b_mu_staco_MET_wet);
   fChain->SetBranchAddress("mu_staco_MET_statusWord", &mu_staco_MET_statusWord, &b_mu_staco_MET_statusWord);
   fChain->SetBranchAddress("mu_muid_MET_n", &mu_muid_MET_n, &b_mu_muid_MET_n);
   fChain->SetBranchAddress("mu_muid_MET_wpx", &mu_muid_MET_wpx, &b_mu_muid_MET_wpx);
   fChain->SetBranchAddress("mu_muid_MET_wpy", &mu_muid_MET_wpy, &b_mu_muid_MET_wpy);
   fChain->SetBranchAddress("mu_muid_MET_wet", &mu_muid_MET_wet, &b_mu_muid_MET_wet);
   fChain->SetBranchAddress("mu_muid_MET_statusWord", &mu_muid_MET_statusWord, &b_mu_muid_MET_statusWord);
   fChain->SetBranchAddress("tau_MET_n", &tau_MET_n, &b_tau_MET_n);
   fChain->SetBranchAddress("tau_MET_wpx", &tau_MET_wpx, &b_tau_MET_wpx);
   fChain->SetBranchAddress("tau_MET_wpy", &tau_MET_wpy, &b_tau_MET_wpy);
   fChain->SetBranchAddress("tau_MET_wet", &tau_MET_wet, &b_tau_MET_wet);
   fChain->SetBranchAddress("tau_MET_statusWord", &tau_MET_statusWord, &b_tau_MET_statusWord);
   fChain->SetBranchAddress("jet_AntiKt6LCTopo_MET_n", &jet_AntiKt6LCTopo_MET_n, &b_jet_AntiKt6LCTopo_MET_n);
   fChain->SetBranchAddress("jet_AntiKt6LCTopo_MET_wpx", &jet_AntiKt6LCTopo_MET_wpx, &b_jet_AntiKt6LCTopo_MET_wpx);
   fChain->SetBranchAddress("jet_AntiKt6LCTopo_MET_wpy", &jet_AntiKt6LCTopo_MET_wpy, &b_jet_AntiKt6LCTopo_MET_wpy);
   fChain->SetBranchAddress("jet_AntiKt6LCTopo_MET_wet", &jet_AntiKt6LCTopo_MET_wet, &b_jet_AntiKt6LCTopo_MET_wet);
   fChain->SetBranchAddress("jet_AntiKt6LCTopo_MET_statusWord", &jet_AntiKt6LCTopo_MET_statusWord, &b_jet_AntiKt6LCTopo_MET_statusWord);
   fChain->SetBranchAddress("cl_MET_n", &cl_MET_n, &b_cl_MET_n);
   fChain->SetBranchAddress("cl_MET_wpx", &cl_MET_wpx, &b_cl_MET_wpx);
   fChain->SetBranchAddress("cl_MET_wpy", &cl_MET_wpy, &b_cl_MET_wpy);
   fChain->SetBranchAddress("cl_MET_wet", &cl_MET_wet, &b_cl_MET_wet);
   fChain->SetBranchAddress("cl_MET_statusWord", &cl_MET_statusWord, &b_cl_MET_statusWord);
   fChain->SetBranchAddress("trk_MET_n", &trk_MET_n, &b_trk_MET_n);
   fChain->SetBranchAddress("trk_MET_wpx", &trk_MET_wpx, &b_trk_MET_wpx);
   fChain->SetBranchAddress("trk_MET_wpy", &trk_MET_wpy, &b_trk_MET_wpy);
   fChain->SetBranchAddress("trk_MET_wet", &trk_MET_wet, &b_trk_MET_wet);
   fChain->SetBranchAddress("trk_MET_statusWord", &trk_MET_statusWord, &b_trk_MET_statusWord);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_n", &jet_AntiKt4TopoNewEM_n, &b_jet_AntiKt4TopoNewEM_n);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_E", &jet_AntiKt4TopoNewEM_E, &b_jet_AntiKt4TopoNewEM_E);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_pt", &jet_AntiKt4TopoNewEM_pt, &b_jet_AntiKt4TopoNewEM_pt);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_m", &jet_AntiKt4TopoNewEM_m, &b_jet_AntiKt4TopoNewEM_m);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_eta", &jet_AntiKt4TopoNewEM_eta, &b_jet_AntiKt4TopoNewEM_eta);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_phi", &jet_AntiKt4TopoNewEM_phi, &b_jet_AntiKt4TopoNewEM_phi);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_EtaOrigin", &jet_AntiKt4TopoNewEM_EtaOrigin, &b_jet_AntiKt4TopoNewEM_EtaOrigin);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_PhiOrigin", &jet_AntiKt4TopoNewEM_PhiOrigin, &b_jet_AntiKt4TopoNewEM_PhiOrigin);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_MOrigin", &jet_AntiKt4TopoNewEM_MOrigin, &b_jet_AntiKt4TopoNewEM_MOrigin);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_EtaOriginEM", &jet_AntiKt4TopoNewEM_EtaOriginEM, &b_jet_AntiKt4TopoNewEM_EtaOriginEM);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_PhiOriginEM", &jet_AntiKt4TopoNewEM_PhiOriginEM, &b_jet_AntiKt4TopoNewEM_PhiOriginEM);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_MOriginEM", &jet_AntiKt4TopoNewEM_MOriginEM, &b_jet_AntiKt4TopoNewEM_MOriginEM);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_WIDTH", &jet_AntiKt4TopoNewEM_WIDTH, &b_jet_AntiKt4TopoNewEM_WIDTH);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_n90", &jet_AntiKt4TopoNewEM_n90, &b_jet_AntiKt4TopoNewEM_n90);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_Timing", &jet_AntiKt4TopoNewEM_Timing, &b_jet_AntiKt4TopoNewEM_Timing);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_LArQuality", &jet_AntiKt4TopoNewEM_LArQuality, &b_jet_AntiKt4TopoNewEM_LArQuality);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_nTrk", &jet_AntiKt4TopoNewEM_nTrk, &b_jet_AntiKt4TopoNewEM_nTrk);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_sumPtTrk", &jet_AntiKt4TopoNewEM_sumPtTrk, &b_jet_AntiKt4TopoNewEM_sumPtTrk);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_OriginIndex", &jet_AntiKt4TopoNewEM_OriginIndex, &b_jet_AntiKt4TopoNewEM_OriginIndex);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_HECQuality", &jet_AntiKt4TopoNewEM_HECQuality, &b_jet_AntiKt4TopoNewEM_HECQuality);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_NegativeE", &jet_AntiKt4TopoNewEM_NegativeE, &b_jet_AntiKt4TopoNewEM_NegativeE);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_AverageLArQF", &jet_AntiKt4TopoNewEM_AverageLArQF, &b_jet_AntiKt4TopoNewEM_AverageLArQF);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_YFlip12", &jet_AntiKt4TopoNewEM_YFlip12, &b_jet_AntiKt4TopoNewEM_YFlip12);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_YFlip23", &jet_AntiKt4TopoNewEM_YFlip23, &b_jet_AntiKt4TopoNewEM_YFlip23);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_BCH_CORR_CELL", &jet_AntiKt4TopoNewEM_BCH_CORR_CELL, &b_jet_AntiKt4TopoNewEM_BCH_CORR_CELL);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_BCH_CORR_DOTX", &jet_AntiKt4TopoNewEM_BCH_CORR_DOTX, &b_jet_AntiKt4TopoNewEM_BCH_CORR_DOTX);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_BCH_CORR_JET", &jet_AntiKt4TopoNewEM_BCH_CORR_JET, &b_jet_AntiKt4TopoNewEM_BCH_CORR_JET);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_BCH_CORR_JET_FORCELL", &jet_AntiKt4TopoNewEM_BCH_CORR_JET_FORCELL, &b_jet_AntiKt4TopoNewEM_BCH_CORR_JET_FORCELL);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_ENG_BAD_CELLS", &jet_AntiKt4TopoNewEM_ENG_BAD_CELLS, &b_jet_AntiKt4TopoNewEM_ENG_BAD_CELLS);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_N_BAD_CELLS", &jet_AntiKt4TopoNewEM_N_BAD_CELLS, &b_jet_AntiKt4TopoNewEM_N_BAD_CELLS);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_N_BAD_CELLS_CORR", &jet_AntiKt4TopoNewEM_N_BAD_CELLS_CORR, &b_jet_AntiKt4TopoNewEM_N_BAD_CELLS_CORR);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_BAD_CELLS_CORR_E", &jet_AntiKt4TopoNewEM_BAD_CELLS_CORR_E, &b_jet_AntiKt4TopoNewEM_BAD_CELLS_CORR_E);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_NumTowers", &jet_AntiKt4TopoNewEM_NumTowers, &b_jet_AntiKt4TopoNewEM_NumTowers);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_SamplingMax", &jet_AntiKt4TopoNewEM_SamplingMax, &b_jet_AntiKt4TopoNewEM_SamplingMax);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_fracSamplingMax", &jet_AntiKt4TopoNewEM_fracSamplingMax, &b_jet_AntiKt4TopoNewEM_fracSamplingMax);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_hecf", &jet_AntiKt4TopoNewEM_hecf, &b_jet_AntiKt4TopoNewEM_hecf);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_tgap3f", &jet_AntiKt4TopoNewEM_tgap3f, &b_jet_AntiKt4TopoNewEM_tgap3f);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_isUgly", &jet_AntiKt4TopoNewEM_isUgly, &b_jet_AntiKt4TopoNewEM_isUgly);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_isBadLoose", &jet_AntiKt4TopoNewEM_isBadLoose, &b_jet_AntiKt4TopoNewEM_isBadLoose);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_isBadMedium", &jet_AntiKt4TopoNewEM_isBadMedium, &b_jet_AntiKt4TopoNewEM_isBadMedium);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_isBadTight", &jet_AntiKt4TopoNewEM_isBadTight, &b_jet_AntiKt4TopoNewEM_isBadTight);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_emfrac", &jet_AntiKt4TopoNewEM_emfrac, &b_jet_AntiKt4TopoNewEM_emfrac);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_Offset", &jet_AntiKt4TopoNewEM_Offset, &b_jet_AntiKt4TopoNewEM_Offset);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_EMJES", &jet_AntiKt4TopoNewEM_EMJES, &b_jet_AntiKt4TopoNewEM_EMJES);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_EMJES_EtaCorr", &jet_AntiKt4TopoNewEM_EMJES_EtaCorr, &b_jet_AntiKt4TopoNewEM_EMJES_EtaCorr);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_EMJESnooffset", &jet_AntiKt4TopoNewEM_EMJESnooffset, &b_jet_AntiKt4TopoNewEM_EMJESnooffset);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_GCWJES", &jet_AntiKt4TopoNewEM_GCWJES, &b_jet_AntiKt4TopoNewEM_GCWJES);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_GCWJES_EtaCorr", &jet_AntiKt4TopoNewEM_GCWJES_EtaCorr, &b_jet_AntiKt4TopoNewEM_GCWJES_EtaCorr);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_CB", &jet_AntiKt4TopoNewEM_CB, &b_jet_AntiKt4TopoNewEM_CB);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_emscale_E", &jet_AntiKt4TopoNewEM_emscale_E, &b_jet_AntiKt4TopoNewEM_emscale_E);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_emscale_pt", &jet_AntiKt4TopoNewEM_emscale_pt, &b_jet_AntiKt4TopoNewEM_emscale_pt);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_emscale_m", &jet_AntiKt4TopoNewEM_emscale_m, &b_jet_AntiKt4TopoNewEM_emscale_m);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_emscale_eta", &jet_AntiKt4TopoNewEM_emscale_eta, &b_jet_AntiKt4TopoNewEM_emscale_eta);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_emscale_phi", &jet_AntiKt4TopoNewEM_emscale_phi, &b_jet_AntiKt4TopoNewEM_emscale_phi);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_jvtxf", &jet_AntiKt4TopoNewEM_jvtxf, &b_jet_AntiKt4TopoNewEM_jvtxf);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_jvtx_x", &jet_AntiKt4TopoNewEM_jvtx_x, &b_jet_AntiKt4TopoNewEM_jvtx_x);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_jvtx_y", &jet_AntiKt4TopoNewEM_jvtx_y, &b_jet_AntiKt4TopoNewEM_jvtx_y);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_jvtx_z", &jet_AntiKt4TopoNewEM_jvtx_z, &b_jet_AntiKt4TopoNewEM_jvtx_z);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_GSCFactorF", &jet_AntiKt4TopoNewEM_GSCFactorF, &b_jet_AntiKt4TopoNewEM_GSCFactorF);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_WidthFraction", &jet_AntiKt4TopoNewEM_WidthFraction, &b_jet_AntiKt4TopoNewEM_WidthFraction);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_flavor_weight_Comb", &jet_AntiKt4TopoNewEM_flavor_weight_Comb, &b_jet_AntiKt4TopoNewEM_flavor_weight_Comb);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_flavor_weight_IP2D", &jet_AntiKt4TopoNewEM_flavor_weight_IP2D, &b_jet_AntiKt4TopoNewEM_flavor_weight_IP2D);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_flavor_weight_IP3D", &jet_AntiKt4TopoNewEM_flavor_weight_IP3D, &b_jet_AntiKt4TopoNewEM_flavor_weight_IP3D);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_flavor_weight_SV0", &jet_AntiKt4TopoNewEM_flavor_weight_SV0, &b_jet_AntiKt4TopoNewEM_flavor_weight_SV0);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_flavor_weight_SV1", &jet_AntiKt4TopoNewEM_flavor_weight_SV1, &b_jet_AntiKt4TopoNewEM_flavor_weight_SV1);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_flavor_weight_SV2", &jet_AntiKt4TopoNewEM_flavor_weight_SV2, &b_jet_AntiKt4TopoNewEM_flavor_weight_SV2);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_flavor_weight_JetProb", &jet_AntiKt4TopoNewEM_flavor_weight_JetProb, &b_jet_AntiKt4TopoNewEM_flavor_weight_JetProb);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_flavor_weight_TrackCounting2D", &jet_AntiKt4TopoNewEM_flavor_weight_TrackCounting2D, &b_jet_AntiKt4TopoNewEM_flavor_weight_TrackCounting2D);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_flavor_weight_SoftMuonTag", &jet_AntiKt4TopoNewEM_flavor_weight_SoftMuonTag, &b_jet_AntiKt4TopoNewEM_flavor_weight_SoftMuonTag);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_flavor_weight_SoftElectronTag", &jet_AntiKt4TopoNewEM_flavor_weight_SoftElectronTag, &b_jet_AntiKt4TopoNewEM_flavor_weight_SoftElectronTag);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_flavor_weight_JetFitterTagNN", &jet_AntiKt4TopoNewEM_flavor_weight_JetFitterTagNN, &b_jet_AntiKt4TopoNewEM_flavor_weight_JetFitterTagNN);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_flavor_weight_JetFitterCOMBNN", &jet_AntiKt4TopoNewEM_flavor_weight_JetFitterCOMBNN, &b_jet_AntiKt4TopoNewEM_flavor_weight_JetFitterCOMBNN);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_flavor_truth_label", &jet_AntiKt4TopoNewEM_flavor_truth_label, &b_jet_AntiKt4TopoNewEM_flavor_truth_label);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_flavor_truth_dRminToB", &jet_AntiKt4TopoNewEM_flavor_truth_dRminToB, &b_jet_AntiKt4TopoNewEM_flavor_truth_dRminToB);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_flavor_truth_dRminToC", &jet_AntiKt4TopoNewEM_flavor_truth_dRminToC, &b_jet_AntiKt4TopoNewEM_flavor_truth_dRminToC);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_flavor_truth_dRminToT", &jet_AntiKt4TopoNewEM_flavor_truth_dRminToT, &b_jet_AntiKt4TopoNewEM_flavor_truth_dRminToT);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_flavor_truth_BHadronpdg", &jet_AntiKt4TopoNewEM_flavor_truth_BHadronpdg, &b_jet_AntiKt4TopoNewEM_flavor_truth_BHadronpdg);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_flavor_truth_vx_x", &jet_AntiKt4TopoNewEM_flavor_truth_vx_x, &b_jet_AntiKt4TopoNewEM_flavor_truth_vx_x);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_flavor_truth_vx_y", &jet_AntiKt4TopoNewEM_flavor_truth_vx_y, &b_jet_AntiKt4TopoNewEM_flavor_truth_vx_y);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_flavor_truth_vx_z", &jet_AntiKt4TopoNewEM_flavor_truth_vx_z, &b_jet_AntiKt4TopoNewEM_flavor_truth_vx_z);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_el_dr", &jet_AntiKt4TopoNewEM_el_dr, &b_jet_AntiKt4TopoNewEM_el_dr);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_el_matched", &jet_AntiKt4TopoNewEM_el_matched, &b_jet_AntiKt4TopoNewEM_el_matched);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_mu_dr", &jet_AntiKt4TopoNewEM_mu_dr, &b_jet_AntiKt4TopoNewEM_mu_dr);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_mu_matched", &jet_AntiKt4TopoNewEM_mu_matched, &b_jet_AntiKt4TopoNewEM_mu_matched);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_L1_dr", &jet_AntiKt4TopoNewEM_L1_dr, &b_jet_AntiKt4TopoNewEM_L1_dr);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_L1_matched", &jet_AntiKt4TopoNewEM_L1_matched, &b_jet_AntiKt4TopoNewEM_L1_matched);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_L2_dr", &jet_AntiKt4TopoNewEM_L2_dr, &b_jet_AntiKt4TopoNewEM_L2_dr);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_L2_matched", &jet_AntiKt4TopoNewEM_L2_matched, &b_jet_AntiKt4TopoNewEM_L2_matched);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_EF_dr", &jet_AntiKt4TopoNewEM_EF_dr, &b_jet_AntiKt4TopoNewEM_EF_dr);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_EF_matched", &jet_AntiKt4TopoNewEM_EF_matched, &b_jet_AntiKt4TopoNewEM_EF_matched);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_SmearingFactor", &jet_AntiKt4TopoNewEM_SmearingFactor, &b_jet_AntiKt4TopoNewEM_SmearingFactor);
   fChain->SetBranchAddress("vx_n", &vx_n, &b_vx_n);
   fChain->SetBranchAddress("vx_x", &vx_x, &b_vx_x);
   fChain->SetBranchAddress("vx_y", &vx_y, &b_vx_y);
   fChain->SetBranchAddress("vx_z", &vx_z, &b_vx_z);
   fChain->SetBranchAddress("vx_px", &vx_px, &b_vx_px);
   fChain->SetBranchAddress("vx_py", &vx_py, &b_vx_py);
   fChain->SetBranchAddress("vx_pz", &vx_pz, &b_vx_pz);
   fChain->SetBranchAddress("vx_E", &vx_E, &b_vx_E);
   fChain->SetBranchAddress("vx_m", &vx_m, &b_vx_m);
   fChain->SetBranchAddress("vx_nTracks", &vx_nTracks, &b_vx_nTracks);
   fChain->SetBranchAddress("vx_sumPt", &vx_sumPt, &b_vx_sumPt);
   fChain->SetBranchAddress("vx_type", &vx_type, &b_vx_type);
   fChain->SetBranchAddress("top_hfor_type", &top_hfor_type, &b_top_hfor_type);
   fChain->SetBranchAddress("mcevt_n", &mcevt_n, &b_mcevt_n);
   fChain->SetBranchAddress("mcevt_signal_process_id", &mcevt_signal_process_id, &b_mcevt_signal_process_id);
   fChain->SetBranchAddress("mcevt_event_number", &mcevt_event_number, &b_mcevt_event_number);
   fChain->SetBranchAddress("mcevt_event_scale", &mcevt_event_scale, &b_mcevt_event_scale);
   fChain->SetBranchAddress("mcevt_alphaQCD", &mcevt_alphaQCD, &b_mcevt_alphaQCD);
   fChain->SetBranchAddress("mcevt_alphaQED", &mcevt_alphaQED, &b_mcevt_alphaQED);
   fChain->SetBranchAddress("mcevt_pdf_id1", &mcevt_pdf_id1, &b_mcevt_pdf_id1);
   fChain->SetBranchAddress("mcevt_pdf_id2", &mcevt_pdf_id2, &b_mcevt_pdf_id2);
   fChain->SetBranchAddress("mcevt_pdf_x1", &mcevt_pdf_x1, &b_mcevt_pdf_x1);
   fChain->SetBranchAddress("mcevt_pdf_x2", &mcevt_pdf_x2, &b_mcevt_pdf_x2);
   fChain->SetBranchAddress("mcevt_pdf_scale", &mcevt_pdf_scale, &b_mcevt_pdf_scale);
   fChain->SetBranchAddress("mcevt_pdf1", &mcevt_pdf1, &b_mcevt_pdf1);
   fChain->SetBranchAddress("mcevt_pdf2", &mcevt_pdf2, &b_mcevt_pdf2);
   fChain->SetBranchAddress("mcevt_weight", &mcevt_weight, &b_mcevt_weight);
   fChain->SetBranchAddress("mcevt_nparticle", &mcevt_nparticle, &b_mcevt_nparticle);
   fChain->SetBranchAddress("mcevt_pileUpType", &mcevt_pileUpType, &b_mcevt_pileUpType);
   fChain->SetBranchAddress("trig_EF_trigmuonef_n", &trig_EF_trigmuonef_n, &b_trig_EF_trigmuonef_n);
   fChain->SetBranchAddress("trig_EF_trigmuonef_track_n", &trig_EF_trigmuonef_track_n, &b_trig_EF_trigmuonef_track_n);
   fChain->SetBranchAddress("trig_EF_trigmuonef_track_MuonType", &trig_EF_trigmuonef_track_MuonType, &b_trig_EF_trigmuonef_track_MuonType);
   fChain->SetBranchAddress("trig_EF_trigmuonef_track_MS_pt", &trig_EF_trigmuonef_track_MS_pt, &b_trig_EF_trigmuonef_track_MS_pt);
   fChain->SetBranchAddress("trig_EF_trigmuonef_track_MS_eta", &trig_EF_trigmuonef_track_MS_eta, &b_trig_EF_trigmuonef_track_MS_eta);
   fChain->SetBranchAddress("trig_EF_trigmuonef_track_MS_phi", &trig_EF_trigmuonef_track_MS_phi, &b_trig_EF_trigmuonef_track_MS_phi);
   fChain->SetBranchAddress("trig_EF_trigmuonef_track_MS_hasMS", &trig_EF_trigmuonef_track_MS_hasMS, &b_trig_EF_trigmuonef_track_MS_hasMS);
   fChain->SetBranchAddress("trig_EF_trigmuonef_track_SA_pt", &trig_EF_trigmuonef_track_SA_pt, &b_trig_EF_trigmuonef_track_SA_pt);
   fChain->SetBranchAddress("trig_EF_trigmuonef_track_SA_eta", &trig_EF_trigmuonef_track_SA_eta, &b_trig_EF_trigmuonef_track_SA_eta);
   fChain->SetBranchAddress("trig_EF_trigmuonef_track_SA_phi", &trig_EF_trigmuonef_track_SA_phi, &b_trig_EF_trigmuonef_track_SA_phi);
   fChain->SetBranchAddress("trig_EF_trigmuonef_track_SA_hasSA", &trig_EF_trigmuonef_track_SA_hasSA, &b_trig_EF_trigmuonef_track_SA_hasSA);
   fChain->SetBranchAddress("trig_EF_trigmuonef_track_CB_pt", &trig_EF_trigmuonef_track_CB_pt, &b_trig_EF_trigmuonef_track_CB_pt);
   fChain->SetBranchAddress("trig_EF_trigmuonef_track_CB_eta", &trig_EF_trigmuonef_track_CB_eta, &b_trig_EF_trigmuonef_track_CB_eta);
   fChain->SetBranchAddress("trig_EF_trigmuonef_track_CB_phi", &trig_EF_trigmuonef_track_CB_phi, &b_trig_EF_trigmuonef_track_CB_phi);
   fChain->SetBranchAddress("trig_EF_trigmuonef_track_CB_hasCB", &trig_EF_trigmuonef_track_CB_hasCB, &b_trig_EF_trigmuonef_track_CB_hasCB);
   fChain->SetBranchAddress("bunch_configID", &bunch_configID, &b_bunch_configID);

   Notify();
}

Bool_t Example::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normaly not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void Example::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t Example::Cut(Long64_t /*entry*/)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef Example_cxx
