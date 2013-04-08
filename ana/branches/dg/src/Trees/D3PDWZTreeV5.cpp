#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <vector>
#include "TrigFTKAna/Trees/D3PDWZTreeV5.h"

D3PDWZTreeV5::D3PDWZTreeV5(TTree *tree)
{    
  Init(tree);
} 

D3PDWZTreeV5::~D3PDWZTreeV5() 
{
  if (!fChain) return;
  // delete fChain->GetCurrentFile();
}

Int_t D3PDWZTreeV5::GetEntry(Long64_t entry) 
{
  // Read contents of entry.   
  if (!fChain) return 0;
  mc_n = 0;
  return fChain->GetEntry(entry); 
}

Long64_t D3PDWZTreeV5::LoadTree(Long64_t entry)
{
  // Set the environment to read one entry
  if (!fChain) return -5;
  Long64_t centry = fChain->LoadTree(entry);
  if (centry < 0) return centry;
  if (!fChain->InheritsFrom(TChain::Class()))  return centry;
  TChain *chain = (TChain*)fChain;
  if (chain->GetTreeNumber() != fCurrent) {
    fCurrent = chain->GetTreeNumber();
  }
  return centry;
}


void D3PDWZTreeV5::Init(TTree *tree)
{
  // The Init() function is called when the selector needs to initialize
  // a new tree or chain. Typically here the branch addresses and branch
  // pointers of the tree will be set.
  // It is normally not necessary to make changes to the generated
  // code, but the routine can be extended by the user if needed.
  // Init() will be called many times when running on PROOF
  // (once per file to be processed).

  fChain = 0;

  // Set object pointer
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
  el_type = 0;
  el_origin = 0;
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
  el_truth_index = 0;
  el_truth_matched = 0;
  el_loose = 0;
  el_medium = 0;
  el_mediumIso = 0;
  el_tight = 0;
  el_tightIso = 0;
  el_goodOQ = 0;
  el_Ethad = 0;
  el_Ethad1 = 0;
  el_f1 = 0;
  el_f1core = 0;
  el_Emins1 = 0;
  el_fside = 0;
  el_Emax2 = 0;
  el_ws3 = 0;
  el_wstot = 0;
  el_emaxs1 = 0;
  el_deltaEs = 0;
  el_E233 = 0;
  el_E237 = 0;
  el_E277 = 0;
  el_weta2 = 0;
  el_f3 = 0;
  el_f3core = 0;
  el_rphiallcalo = 0;
  el_Etcone45 = 0;
  el_Etcone20 = 0;
  el_Etcone30 = 0;
  el_Etcone40 = 0;
  el_ptcone20 = 0;
  el_ptcone30 = 0;
  el_ptcone40 = 0;
  el_nucone20 = 0;
  el_nucone30 = 0;
  el_nucone40 = 0;
  el_convanglematch = 0;
  el_convtrackmatch = 0;
  el_hasconv = 0;
  el_convvtxx = 0;
  el_convvtxy = 0;
  el_convvtxz = 0;
  el_Rconv = 0;
  el_zconv = 0;
  el_convvtxchi2 = 0;
  el_pt1conv = 0;
  el_convtrk1nBLHits = 0;
  el_convtrk1nPixHits = 0;
  el_convtrk1nSCTHits = 0;
  el_convtrk1nTRTHits = 0;
  el_pt2conv = 0;
  el_convtrk2nBLHits = 0;
  el_convtrk2nPixHits = 0;
  el_convtrk2nSCTHits = 0;
  el_convtrk2nTRTHits = 0;
  el_ptconv = 0;
  el_pzconv = 0;
  el_pos7 = 0;
  el_etacorrmag = 0;
  el_deltaeta1 = 0;
  el_deltaeta2 = 0;
  el_deltaphi2 = 0;
  el_deltaphiRescaled = 0;
  el_deltaPhiRot = 0;
  el_expectHitInBLayer = 0;
  el_reta = 0;
  el_rphi = 0;
  el_EtringnoisedR03sig2 = 0;
  el_EtringnoisedR03sig3 = 0;
  el_EtringnoisedR03sig4 = 0;
  el_isolationlikelihoodjets = 0;
  el_isolationlikelihoodhqelectrons = 0;
  el_electronweight = 0;
  el_electronbgweight = 0;
  el_softeweight = 0;
  el_softebgweight = 0;
  el_neuralnet = 0;
  el_Hmatrix = 0;
  el_Hmatrix5 = 0;
  el_adaboost = 0;
  el_softeneuralnet = 0;
  el_zvertex = 0;
  el_errz = 0;
  el_etap = 0;
  el_depth = 0;
  el_refittedTrack_n = 0;
  el_Es0 = 0;
  el_etas0 = 0;
  el_phis0 = 0;
  el_Es1 = 0;
  el_etas1 = 0;
  el_phis1 = 0;
  el_Es2 = 0;
  el_etas2 = 0;
  el_phis2 = 0;
  el_Es3 = 0;
  el_etas3 = 0;
  el_phis3 = 0;
  el_E_PreSamplerB = 0;
  el_E_EMB1 = 0;
  el_E_EMB2 = 0;
  el_E_EMB3 = 0;
  el_E_PreSamplerE = 0;
  el_E_EME1 = 0;
  el_E_EME2 = 0;
  el_E_EME3 = 0;
  el_E_HEC0 = 0;
  el_E_HEC1 = 0;
  el_E_HEC2 = 0;
  el_E_HEC3 = 0;
  el_E_TileBar0 = 0;
  el_E_TileBar1 = 0;
  el_E_TileBar2 = 0;
  el_E_TileGap1 = 0;
  el_E_TileGap2 = 0;
  el_E_TileGap3 = 0;
  el_E_TileExt0 = 0;
  el_E_TileExt1 = 0;
  el_E_TileExt2 = 0;
  el_E_FCAL0 = 0;
  el_E_FCAL1 = 0;
  el_E_FCAL2 = 0;
  el_cl_E = 0;
  el_cl_pt = 0;
  el_cl_eta = 0;
  el_cl_phi = 0;
  el_firstEdens = 0;
  el_cellmaxfrac = 0;
  el_longitudinal = 0;
  el_secondlambda = 0;
  el_lateral = 0;
  el_secondR = 0;
  el_centerlambda = 0;
  el_rawcl_Es0 = 0;
  el_rawcl_etas0 = 0;
  el_rawcl_phis0 = 0;
  el_rawcl_Es1 = 0;
  el_rawcl_etas1 = 0;
  el_rawcl_phis1 = 0;
  el_rawcl_Es2 = 0;
  el_rawcl_etas2 = 0;
  el_rawcl_phis2 = 0;
  el_rawcl_Es3 = 0;
  el_rawcl_etas3 = 0;
  el_rawcl_phis3 = 0;
  el_rawcl_E = 0;
  el_rawcl_pt = 0;
  el_rawcl_eta = 0;
  el_rawcl_phi = 0;
  el_refittedtrackcovd0 = 0;
  el_refittedtrackcovz0 = 0;
  el_refittedtrackcovphi = 0;
  el_refittedtrackcovtheta = 0;
  el_refittedtrackcovqoverp = 0;
  el_refittedtrackcovd0z0 = 0;
  el_refittedtrackcovz0phi = 0;
  el_refittedtrackcovz0theta = 0;
  el_refittedtrackcovz0qoverp = 0;
  el_refittedtrackcovd0phi = 0;
  el_refittedtrackcovd0theta = 0;
  el_refittedtrackcovd0qoverp = 0;
  el_refittedtrackcovphitheta = 0;
  el_refittedtrackcovphiqoverp = 0;
  el_refittedtrackcovthetaqoverp = 0;
  el_trackd0 = 0;
  el_trackz0 = 0;
  el_trackphi = 0;
  el_tracktheta = 0;
  el_trackqoverp = 0;
  el_trackpt = 0;
  el_tracketa = 0;
  el_trackcov_d0 = 0;
  el_trackcov_z0 = 0;
  el_trackcov_phi = 0;
  el_trackcov_theta = 0;
  el_trackcov_qoverp = 0;
  el_trackcov_d0_z0 = 0;
  el_trackcov_d0_phi = 0;
  el_trackcov_d0_theta = 0;
  el_trackcov_d0_qoverp = 0;
  el_trackcov_z0_phi = 0;
  el_trackcov_z0_theta = 0;
  el_trackcov_z0_qoverp = 0;
  el_trackcov_phi_theta = 0;
  el_trackcov_phi_qoverp = 0;
  el_trackcov_theta_qoverp = 0;
  el_trackfitchi2 = 0;
  el_trackfitndof = 0;
  el_nBLHits = 0;
  el_nPixHits = 0;
  el_nSCTHits = 0;
  el_nTRTHits = 0;
  el_nPixHoles = 0;
  el_nSCTHoles = 0;
  el_nBLSharedHits = 0;
  el_nPixSharedHits = 0;
  el_nSCTSharedHits = 0;
  el_nTRTHighTHits = 0;
  el_nTRTOutliers = 0;
  el_nTRTHighTOutliers = 0;
  el_nSiHits = 0;
  el_TRTHighTHitsRatio = 0;
  el_pixeldEdx = 0;
  el_eProbabilityComb = 0;
  el_eProbabilityHT = 0;
  el_eProbabilityToT = 0;
  el_eProbabilityBrem = 0;
  el_vertx = 0;
  el_verty = 0;
  el_vertz = 0;
  el_hastrack = 0;
  el_deltaEmax2 = 0;
  el_calibHitsShowerDepth = 0;
  el_trackd0beam = 0;
  el_tracksigd0beam = 0;
  el_trackd0pv = 0;
  el_tracksigd0pv = 0;
  el_trackz0pv = 0;
  el_tracksigz0pv = 0;
  el_trackd0pvunbiased = 0;
  el_tracksigd0pvunbiased = 0;
  el_trackz0pvunbiased = 0;
  el_tracksigz0pvunbiased = 0;
  el_isIso = 0;
  el_mvaptcone20 = 0;
  el_mvaptcone30 = 0;
  el_mvaptcone40 = 0;
  el_jet_dr = 0;
  el_jet_E = 0;
  el_jet_pt = 0;
  el_jet_m = 0;
  el_jet_eta = 0;
  el_jet_phi = 0;
  el_jet_truth_dr = 0;
  el_jet_truth_E = 0;
  el_jet_truth_pt = 0;
  el_jet_truth_m = 0;
  el_jet_truth_eta = 0;
  el_jet_truth_phi = 0;
  el_jet_truth_matched = 0;
  el_jet_matched = 0;
  el_Etcone40_pt_corrected = 0;
  el_Etcone40_ED_corrected = 0;
  el_Etcone40_corrected = 0;
  el_EF_dr = 0;
  el_EF_index = 0;
  el_L2_dr = 0;
  el_L2_index = 0;
  el_L1_dr = 0;
  el_L1_index = 0;
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
  ph_convFlag = 0;
  ph_isConv = 0;
  ph_nConv = 0;
  ph_nSingleTrackConv = 0;
  ph_nDoubleTrackConv = 0;
  ph_type = 0;
  ph_origin = 0;
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
  ph_truth_index = 0;
  ph_truth_matched = 0;
  ph_loose = 0;
  ph_tight = 0;
  ph_tightIso = 0;
  ph_goodOQ = 0;
  ph_Ethad = 0;
  ph_Ethad1 = 0;
  ph_E033 = 0;
  ph_f1 = 0;
  ph_f1core = 0;
  ph_Emins1 = 0;
  ph_fside = 0;
  ph_Emax2 = 0;
  ph_ws3 = 0;
  ph_wstot = 0;
  ph_E132 = 0;
  ph_E1152 = 0;
  ph_emaxs1 = 0;
  ph_deltaEs = 0;
  ph_E233 = 0;
  ph_E237 = 0;
  ph_E277 = 0;
  ph_weta2 = 0;
  ph_f3 = 0;
  ph_f3core = 0;
  ph_rphiallcalo = 0;
  ph_Etcone45 = 0;
  ph_Etcone20 = 0;
  ph_Etcone30 = 0;
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
  ph_EtringnoisedR03sig2 = 0;
  ph_EtringnoisedR03sig3 = 0;
  ph_EtringnoisedR03sig4 = 0;
  ph_isolationlikelihoodjets = 0;
  ph_isolationlikelihoodhqelectrons = 0;
  ph_loglikelihood = 0;
  ph_photonweight = 0;
  ph_photonbgweight = 0;
  ph_neuralnet = 0;
  ph_Hmatrix = 0;
  ph_Hmatrix5 = 0;
  ph_adaboost = 0;
  ph_zvertex = 0;
  ph_errz = 0;
  ph_etap = 0;
  ph_depth = 0;
  ph_cl_E = 0;
  ph_cl_pt = 0;
  ph_cl_eta = 0;
  ph_cl_phi = 0;
  ph_Es0 = 0;
  ph_etas0 = 0;
  ph_phis0 = 0;
  ph_Es1 = 0;
  ph_etas1 = 0;
  ph_phis1 = 0;
  ph_Es2 = 0;
  ph_etas2 = 0;
  ph_phis2 = 0;
  ph_Es3 = 0;
  ph_etas3 = 0;
  ph_phis3 = 0;
  ph_rawcl_Es0 = 0;
  ph_rawcl_etas0 = 0;
  ph_rawcl_phis0 = 0;
  ph_rawcl_Es1 = 0;
  ph_rawcl_etas1 = 0;
  ph_rawcl_phis1 = 0;
  ph_rawcl_Es2 = 0;
  ph_rawcl_etas2 = 0;
  ph_rawcl_phis2 = 0;
  ph_rawcl_Es3 = 0;
  ph_rawcl_etas3 = 0;
  ph_rawcl_phis3 = 0;
  ph_rawcl_E = 0;
  ph_rawcl_pt = 0;
  ph_rawcl_eta = 0;
  ph_rawcl_phi = 0;
  ph_truth_isConv = 0;
  ph_truth_isBrem = 0;
  ph_truth_isFromHardProc = 0;
  ph_truth_isPhotonFromHardProc = 0;
  ph_truth_Rconv = 0;
  ph_truth_zconv = 0;
  ph_deltaEmax2 = 0;
  ph_calibHitsShowerDepth = 0;
  ph_isIso = 0;
  ph_mvaptcone20 = 0;
  ph_mvaptcone30 = 0;
  ph_mvaptcone40 = 0;
  ph_topoEtcone20 = 0;
  ph_topoEtcone40 = 0;
  ph_topoEtcone60 = 0;
  ph_jet_dr = 0;
  ph_jet_E = 0;
  ph_jet_pt = 0;
  ph_jet_m = 0;
  ph_jet_eta = 0;
  ph_jet_phi = 0;
  ph_jet_truth_dr = 0;
  ph_jet_truth_E = 0;
  ph_jet_truth_pt = 0;
  ph_jet_truth_m = 0;
  ph_jet_truth_eta = 0;
  ph_jet_truth_phi = 0;
  ph_jet_truth_matched = 0;
  ph_jet_matched = 0;
  ph_convIP = 0;
  ph_convIPRev = 0;
  ph_ptIsolationCone = 0;
  ph_ptIsolationConePhAngle = 0;
  ph_Etcone40_pt_corrected = 0;
  ph_Etcone40_ED_corrected = 0;
  ph_Etcone40_corrected = 0;
  ph_topodr = 0;
  ph_topopt = 0;
  ph_topoeta = 0;
  ph_topophi = 0;
  ph_topomatched = 0;
  ph_topoEMdr = 0;
  ph_topoEMpt = 0;
  ph_topoEMeta = 0;
  ph_topoEMphi = 0;
  ph_topoEMmatched = 0;
  ph_EF_dr = 0;
  ph_EF_index = 0;
  ph_L2_dr = 0;
  ph_L2_index = 0;
  ph_L1_dr = 0;
  ph_L1_index = 0;
  mu_muid_E = 0;
  mu_muid_pt = 0;
  mu_muid_m = 0;
  mu_muid_eta = 0;
  mu_muid_phi = 0;
  mu_muid_px = 0;
  mu_muid_py = 0;
  mu_muid_pz = 0;
  mu_muid_charge = 0;
  mu_muid_allauthor = 0;
  mu_muid_author = 0;
  mu_muid_beta = 0;
  mu_muid_isMuonLikelihood = 0;
  mu_muid_matchchi2 = 0;
  mu_muid_matchndof = 0;
  mu_muid_etcone20 = 0;
  mu_muid_etcone30 = 0;
  mu_muid_etcone40 = 0;
  mu_muid_nucone20 = 0;
  mu_muid_nucone30 = 0;
  mu_muid_nucone40 = 0;
  mu_muid_ptcone20 = 0;
  mu_muid_ptcone30 = 0;
  mu_muid_ptcone40 = 0;
  mu_muid_energyLossPar = 0;
  mu_muid_energyLossErr = 0;
  mu_muid_etCore = 0;
  mu_muid_energyLossType = 0;
  mu_muid_caloMuonIdTag = 0;
  mu_muid_caloLRLikelihood = 0;
  mu_muid_bestMatch = 0;
  mu_muid_isStandAloneMuon = 0;
  mu_muid_isCombinedMuon = 0;
  mu_muid_isLowPtReconstructedMuon = 0;
  mu_muid_loose = 0;
  mu_muid_medium = 0;
  mu_muid_tight = 0;
  mu_muid_d0_exPV = 0;
  mu_muid_z0_exPV = 0;
  mu_muid_phi_exPV = 0;
  mu_muid_theta_exPV = 0;
  mu_muid_qoverp_exPV = 0;
  mu_muid_cb_d0_exPV = 0;
  mu_muid_cb_z0_exPV = 0;
  mu_muid_cb_phi_exPV = 0;
  mu_muid_cb_theta_exPV = 0;
  mu_muid_cb_qoverp_exPV = 0;
  mu_muid_id_d0_exPV = 0;
  mu_muid_id_z0_exPV = 0;
  mu_muid_id_phi_exPV = 0;
  mu_muid_id_theta_exPV = 0;
  mu_muid_id_qoverp_exPV = 0;
  mu_muid_me_d0_exPV = 0;
  mu_muid_me_z0_exPV = 0;
  mu_muid_me_phi_exPV = 0;
  mu_muid_me_theta_exPV = 0;
  mu_muid_me_qoverp_exPV = 0;
  mu_muid_ie_d0_exPV = 0;
  mu_muid_ie_z0_exPV = 0;
  mu_muid_ie_phi_exPV = 0;
  mu_muid_ie_theta_exPV = 0;
  mu_muid_ie_qoverp_exPV = 0;
  mu_muid_SpaceTime_detID = 0;
  mu_muid_SpaceTime_t = 0;
  mu_muid_SpaceTime_tError = 0;
  mu_muid_SpaceTime_weight = 0;
  mu_muid_SpaceTime_x = 0;
  mu_muid_SpaceTime_y = 0;
  mu_muid_SpaceTime_z = 0;
  mu_muid_SpaceTime_t_Tile = 0;
  mu_muid_SpaceTime_tError_Tile = 0;
  mu_muid_SpaceTime_weight_Tile = 0;
  mu_muid_SpaceTime_x_Tile = 0;
  mu_muid_SpaceTime_y_Tile = 0;
  mu_muid_SpaceTime_z_Tile = 0;
  mu_muid_SpaceTime_t_TRT = 0;
  mu_muid_SpaceTime_tError_TRT = 0;
  mu_muid_SpaceTime_weight_TRT = 0;
  mu_muid_SpaceTime_x_TRT = 0;
  mu_muid_SpaceTime_y_TRT = 0;
  mu_muid_SpaceTime_z_TRT = 0;
  mu_muid_SpaceTime_t_MDT = 0;
  mu_muid_SpaceTime_tError_MDT = 0;
  mu_muid_SpaceTime_weight_MDT = 0;
  mu_muid_SpaceTime_x_MDT = 0;
  mu_muid_SpaceTime_y_MDT = 0;
  mu_muid_SpaceTime_z_MDT = 0;
  mu_muid_TileCellEnergyLayer1 = 0;
  mu_muid_TileTimeLayer1 = 0;
  mu_muid_TileCellRmsNoiseLayer1 = 0;
  mu_muid_TileCellSignLayer1 = 0;
  mu_muid_TileCellEnergyLayer2 = 0;
  mu_muid_TileTimeLayer2 = 0;
  mu_muid_TileCellRmsNoiseLayer2 = 0;
  mu_muid_TileCellSignLayer2 = 0;
  mu_muid_TileCellEnergyLayer3 = 0;
  mu_muid_TileTimeLayer3 = 0;
  mu_muid_TileCellRmsNoiseLayer3 = 0;
  mu_muid_TileCellSignLayer3 = 0;
  mu_muid_MSTrkT0_1 = 0;
  mu_muid_MSTrkT0_2 = 0;
  mu_muid_MSTrkT0_3 = 0;
  mu_muid_cov_d0_exPV = 0;
  mu_muid_cov_z0_exPV = 0;
  mu_muid_cov_phi_exPV = 0;
  mu_muid_cov_theta_exPV = 0;
  mu_muid_cov_qoverp_exPV = 0;
  mu_muid_cov_d0_z0_exPV = 0;
  mu_muid_cov_d0_phi_exPV = 0;
  mu_muid_cov_d0_theta_exPV = 0;
  mu_muid_cov_d0_qoverp_exPV = 0;
  mu_muid_cov_z0_phi_exPV = 0;
  mu_muid_cov_z0_theta_exPV = 0;
  mu_muid_cov_z0_qoverp_exPV = 0;
  mu_muid_cov_phi_theta_exPV = 0;
  mu_muid_cov_phi_qoverp_exPV = 0;
  mu_muid_cov_theta_qoverp_exPV = 0;
  mu_muid_id_cov_d0_exPV = 0;
  mu_muid_id_cov_z0_exPV = 0;
  mu_muid_id_cov_phi_exPV = 0;
  mu_muid_id_cov_theta_exPV = 0;
  mu_muid_id_cov_qoverp_exPV = 0;
  mu_muid_id_cov_d0_z0_exPV = 0;
  mu_muid_id_cov_d0_phi_exPV = 0;
  mu_muid_id_cov_d0_theta_exPV = 0;
  mu_muid_id_cov_d0_qoverp_exPV = 0;
  mu_muid_id_cov_z0_phi_exPV = 0;
  mu_muid_id_cov_z0_theta_exPV = 0;
  mu_muid_id_cov_z0_qoverp_exPV = 0;
  mu_muid_id_cov_phi_theta_exPV = 0;
  mu_muid_id_cov_phi_qoverp_exPV = 0;
  mu_muid_id_cov_theta_qoverp_exPV = 0;
  mu_muid_me_cov_d0_exPV = 0;
  mu_muid_me_cov_z0_exPV = 0;
  mu_muid_me_cov_phi_exPV = 0;
  mu_muid_me_cov_theta_exPV = 0;
  mu_muid_me_cov_qoverp_exPV = 0;
  mu_muid_me_cov_d0_z0_exPV = 0;
  mu_muid_me_cov_d0_phi_exPV = 0;
  mu_muid_me_cov_d0_theta_exPV = 0;
  mu_muid_me_cov_d0_qoverp_exPV = 0;
  mu_muid_me_cov_z0_phi_exPV = 0;
  mu_muid_me_cov_z0_theta_exPV = 0;
  mu_muid_me_cov_z0_qoverp_exPV = 0;
  mu_muid_me_cov_phi_theta_exPV = 0;
  mu_muid_me_cov_phi_qoverp_exPV = 0;
  mu_muid_me_cov_theta_qoverp_exPV = 0;
  mu_muid_ms_d0 = 0;
  mu_muid_ms_z0 = 0;
  mu_muid_ms_phi = 0;
  mu_muid_ms_theta = 0;
  mu_muid_ms_qoverp = 0;
  mu_muid_id_d0 = 0;
  mu_muid_id_z0 = 0;
  mu_muid_id_phi = 0;
  mu_muid_id_theta = 0;
  mu_muid_id_qoverp = 0;
  mu_muid_me_d0 = 0;
  mu_muid_me_z0 = 0;
  mu_muid_me_phi = 0;
  mu_muid_me_theta = 0;
  mu_muid_me_qoverp = 0;
  mu_muid_ie_d0 = 0;
  mu_muid_ie_z0 = 0;
  mu_muid_ie_phi = 0;
  mu_muid_ie_theta = 0;
  mu_muid_ie_qoverp = 0;
  mu_muid_nOutliersOnTrack = 0;
  mu_muid_nBLHits = 0;
  mu_muid_nPixHits = 0;
  mu_muid_nSCTHits = 0;
  mu_muid_nTRTHits = 0;
  mu_muid_nTRTHighTHits = 0;
  mu_muid_nBLSharedHits = 0;
  mu_muid_nPixSharedHits = 0;
  mu_muid_nPixHoles = 0;
  mu_muid_nSCTSharedHits = 0;
  mu_muid_nSCTHoles = 0;
  mu_muid_nTRTOutliers = 0;
  mu_muid_nTRTHighTOutliers = 0;
  mu_muid_nGangedPixels = 0;
  mu_muid_nPixelDeadSensors = 0;
  mu_muid_nSCTDeadSensors = 0;
  mu_muid_nTRTDeadStraws = 0;
  mu_muid_expectBLayerHit = 0;
  mu_muid_nMDTHits = 0;
  mu_muid_nMDTHoles = 0;
  mu_muid_nCSCEtaHits = 0;
  mu_muid_nCSCEtaHoles = 0;
  mu_muid_nCSCPhiHits = 0;
  mu_muid_nCSCPhiHoles = 0;
  mu_muid_nRPCEtaHits = 0;
  mu_muid_nRPCEtaHoles = 0;
  mu_muid_nRPCPhiHits = 0;
  mu_muid_nRPCPhiHoles = 0;
  mu_muid_nTGCEtaHits = 0;
  mu_muid_nTGCEtaHoles = 0;
  mu_muid_nTGCPhiHits = 0;
  mu_muid_nTGCPhiHoles = 0;
  mu_muid_nMDTBIHits = 0;
  mu_muid_nMDTBMHits = 0;
  mu_muid_nMDTBOHits = 0;
  mu_muid_nMDTBEEHits = 0;
  mu_muid_nMDTBIS78Hits = 0;
  mu_muid_nMDTEIHits = 0;
  mu_muid_nMDTEMHits = 0;
  mu_muid_nMDTEOHits = 0;
  mu_muid_nMDTEEHits = 0;
  mu_muid_nRPCLayer1EtaHits = 0;
  mu_muid_nRPCLayer2EtaHits = 0;
  mu_muid_nRPCLayer3EtaHits = 0;
  mu_muid_nRPCLayer1PhiHits = 0;
  mu_muid_nRPCLayer2PhiHits = 0;
  mu_muid_nRPCLayer3PhiHits = 0;
  mu_muid_nTGCLayer1EtaHits = 0;
  mu_muid_nTGCLayer2EtaHits = 0;
  mu_muid_nTGCLayer3EtaHits = 0;
  mu_muid_nTGCLayer4EtaHits = 0;
  mu_muid_nTGCLayer1PhiHits = 0;
  mu_muid_nTGCLayer2PhiHits = 0;
  mu_muid_nTGCLayer3PhiHits = 0;
  mu_muid_nTGCLayer4PhiHits = 0;
  mu_muid_barrelSectors = 0;
  mu_muid_endcapSectors = 0;
  mu_muid_trackd0 = 0;
  mu_muid_trackz0 = 0;
  mu_muid_trackphi = 0;
  mu_muid_tracktheta = 0;
  mu_muid_trackqoverp = 0;
  mu_muid_trackcov_d0 = 0;
  mu_muid_trackcov_z0 = 0;
  mu_muid_trackcov_phi = 0;
  mu_muid_trackcov_theta = 0;
  mu_muid_trackcov_qoverp = 0;
  mu_muid_trackcov_d0_z0 = 0;
  mu_muid_trackcov_d0_phi = 0;
  mu_muid_trackcov_d0_theta = 0;
  mu_muid_trackcov_d0_qoverp = 0;
  mu_muid_trackcov_z0_phi = 0;
  mu_muid_trackcov_z0_theta = 0;
  mu_muid_trackcov_z0_qoverp = 0;
  mu_muid_trackcov_phi_theta = 0;
  mu_muid_trackcov_phi_qoverp = 0;
  mu_muid_trackcov_theta_qoverp = 0;
  mu_muid_trackfitchi2 = 0;
  mu_muid_trackfitndof = 0;
  mu_muid_hastrack = 0;
  mu_muid_truth_dr = 0;
  mu_muid_truth_E = 0;
  mu_muid_truth_pt = 0;
  mu_muid_truth_eta = 0;
  mu_muid_truth_phi = 0;
  mu_muid_truth_type = 0;
  mu_muid_truth_status = 0;
  mu_muid_truth_barcode = 0;
  mu_muid_truth_mothertype = 0;
  mu_muid_truth_motherbarcode = 0;
  mu_muid_truth_matched = 0;
  mu_muid_EFCB_dr = 0;
  mu_muid_EFCB_index = 0;
  mu_muid_EFMG_dr = 0;
  mu_muid_EFMG_index = 0;
  mu_muid_EFME_dr = 0;
  mu_muid_EFME_index = 0;
  mu_muid_L2CB_dr = 0;
  mu_muid_L2CB_index = 0;
  mu_muid_L1_dr = 0;
  mu_muid_L1_index = 0;
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
  mu_staco_beta = 0;
  mu_staco_isMuonLikelihood = 0;
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
  mu_staco_SpaceTime_detID = 0;
  mu_staco_SpaceTime_t = 0;
  mu_staco_SpaceTime_tError = 0;
  mu_staco_SpaceTime_weight = 0;
  mu_staco_SpaceTime_x = 0;
  mu_staco_SpaceTime_y = 0;
  mu_staco_SpaceTime_z = 0;
  mu_staco_SpaceTime_t_Tile = 0;
  mu_staco_SpaceTime_tError_Tile = 0;
  mu_staco_SpaceTime_weight_Tile = 0;
  mu_staco_SpaceTime_x_Tile = 0;
  mu_staco_SpaceTime_y_Tile = 0;
  mu_staco_SpaceTime_z_Tile = 0;
  mu_staco_SpaceTime_t_TRT = 0;
  mu_staco_SpaceTime_tError_TRT = 0;
  mu_staco_SpaceTime_weight_TRT = 0;
  mu_staco_SpaceTime_x_TRT = 0;
  mu_staco_SpaceTime_y_TRT = 0;
  mu_staco_SpaceTime_z_TRT = 0;
  mu_staco_SpaceTime_t_MDT = 0;
  mu_staco_SpaceTime_tError_MDT = 0;
  mu_staco_SpaceTime_weight_MDT = 0;
  mu_staco_SpaceTime_x_MDT = 0;
  mu_staco_SpaceTime_y_MDT = 0;
  mu_staco_SpaceTime_z_MDT = 0;
  mu_staco_TileCellEnergyLayer1 = 0;
  mu_staco_TileTimeLayer1 = 0;
  mu_staco_TileCellRmsNoiseLayer1 = 0;
  mu_staco_TileCellSignLayer1 = 0;
  mu_staco_TileCellEnergyLayer2 = 0;
  mu_staco_TileTimeLayer2 = 0;
  mu_staco_TileCellRmsNoiseLayer2 = 0;
  mu_staco_TileCellSignLayer2 = 0;
  mu_staco_TileCellEnergyLayer3 = 0;
  mu_staco_TileTimeLayer3 = 0;
  mu_staco_TileCellRmsNoiseLayer3 = 0;
  mu_staco_TileCellSignLayer3 = 0;
  mu_staco_MSTrkT0_1 = 0;
  mu_staco_MSTrkT0_2 = 0;
  mu_staco_MSTrkT0_3 = 0;
  mu_staco_cov_d0_exPV = 0;
  mu_staco_cov_z0_exPV = 0;
  mu_staco_cov_phi_exPV = 0;
  mu_staco_cov_theta_exPV = 0;
  mu_staco_cov_qoverp_exPV = 0;
  mu_staco_cov_d0_z0_exPV = 0;
  mu_staco_cov_d0_phi_exPV = 0;
  mu_staco_cov_d0_theta_exPV = 0;
  mu_staco_cov_d0_qoverp_exPV = 0;
  mu_staco_cov_z0_phi_exPV = 0;
  mu_staco_cov_z0_theta_exPV = 0;
  mu_staco_cov_z0_qoverp_exPV = 0;
  mu_staco_cov_phi_theta_exPV = 0;
  mu_staco_cov_phi_qoverp_exPV = 0;
  mu_staco_cov_theta_qoverp_exPV = 0;
  mu_staco_id_cov_d0_exPV = 0;
  mu_staco_id_cov_z0_exPV = 0;
  mu_staco_id_cov_phi_exPV = 0;
  mu_staco_id_cov_theta_exPV = 0;
  mu_staco_id_cov_qoverp_exPV = 0;
  mu_staco_id_cov_d0_z0_exPV = 0;
  mu_staco_id_cov_d0_phi_exPV = 0;
  mu_staco_id_cov_d0_theta_exPV = 0;
  mu_staco_id_cov_d0_qoverp_exPV = 0;
  mu_staco_id_cov_z0_phi_exPV = 0;
  mu_staco_id_cov_z0_theta_exPV = 0;
  mu_staco_id_cov_z0_qoverp_exPV = 0;
  mu_staco_id_cov_phi_theta_exPV = 0;
  mu_staco_id_cov_phi_qoverp_exPV = 0;
  mu_staco_id_cov_theta_qoverp_exPV = 0;
  mu_staco_me_cov_d0_exPV = 0;
  mu_staco_me_cov_z0_exPV = 0;
  mu_staco_me_cov_phi_exPV = 0;
  mu_staco_me_cov_theta_exPV = 0;
  mu_staco_me_cov_qoverp_exPV = 0;
  mu_staco_me_cov_d0_z0_exPV = 0;
  mu_staco_me_cov_d0_phi_exPV = 0;
  mu_staco_me_cov_d0_theta_exPV = 0;
  mu_staco_me_cov_d0_qoverp_exPV = 0;
  mu_staco_me_cov_z0_phi_exPV = 0;
  mu_staco_me_cov_z0_theta_exPV = 0;
  mu_staco_me_cov_z0_qoverp_exPV = 0;
  mu_staco_me_cov_phi_theta_exPV = 0;
  mu_staco_me_cov_phi_qoverp_exPV = 0;
  mu_staco_me_cov_theta_qoverp_exPV = 0;
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
  mu_staco_trackfitchi2 = 0;
  mu_staco_trackfitndof = 0;
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
  mu_staco_EFMG_index = 0;
  mu_staco_EFME_dr = 0;
  mu_staco_EFME_index = 0;
  mu_staco_L2CB_dr = 0;
  mu_staco_L2CB_index = 0;
  mu_staco_L1_dr = 0;
  mu_staco_L1_index = 0;
  mu_calo_E = 0;
  mu_calo_pt = 0;
  mu_calo_m = 0;
  mu_calo_eta = 0;
  mu_calo_phi = 0;
  mu_calo_px = 0;
  mu_calo_py = 0;
  mu_calo_pz = 0;
  mu_calo_charge = 0;
  mu_calo_allauthor = 0;
  mu_calo_hastrack = 0;
  mu_calo_truth_dr = 0;
  mu_calo_truth_E = 0;
  mu_calo_truth_pt = 0;
  mu_calo_truth_eta = 0;
  mu_calo_truth_phi = 0;
  mu_calo_truth_type = 0;
  mu_calo_truth_status = 0;
  mu_calo_truth_barcode = 0;
  mu_calo_truth_mothertype = 0;
  mu_calo_truth_motherbarcode = 0;
  mu_calo_truth_matched = 0;
  tau_Et = 0;
  tau_pt = 0;
  tau_m = 0;
  tau_eta = 0;
  tau_phi = 0;
  tau_charge = 0;
  tau_BDTEleScore = 0;
  tau_BDTJetScore = 0;
  tau_discCut = 0;
  tau_discCutTMVA = 0;
  tau_discLL = 0;
  tau_discNN = 0;
  tau_efficLL = 0;
  tau_efficNN = 0;
  tau_likelihood = 0;
  tau_tauJetNeuralNetwork = 0;
  tau_tauENeuralNetwork = 0;
  tau_tauElTauLikelihood = 0;
  tau_SafeLikelihood = 0;
  tau_electronVetoLoose = 0;
  tau_electronVetoMedium = 0;
  tau_electronVetoTight = 0;
  tau_muonVeto = 0;
  tau_tauCutLoose = 0;
  tau_tauCutMedium = 0;
  tau_tauCutTight = 0;
  tau_tauCutSafeLoose = 0;
  tau_tauCutSafeMedium = 0;
  tau_tauCutSafeTight = 0;
  tau_tauCutSafeCaloLoose = 0;
  tau_tauCutSafeCaloMedium = 0;
  tau_tauCutSafeCaloTight = 0;
  tau_tauLlhLoose = 0;
  tau_tauLlhMedium = 0;
  tau_tauLlhTight = 0;
  tau_JetBDTLoose = 0;
  tau_JetBDTMedium = 0;
  tau_JetBDTTight = 0;
  tau_EleBDTLoose = 0;
  tau_EleBDTMedium = 0;
  tau_EleBDTTight = 0;
  tau_SafeLlhLoose = 0;
  tau_SafeLlhMedium = 0;
  tau_SafeLlhTight = 0;
  tau_author = 0;
  tau_ROIword = 0;
  tau_nProng = 0;
  tau_numTrack = 0;
  tau_seedCalo_numTrack = 0;
  tau_etOverPtLeadTrk = 0;
  tau_ipZ0SinThetaSigLeadTrk = 0;
  tau_leadTrkPt = 0;
  tau_nLooseTrk = 0;
  tau_nLooseConvTrk = 0;
  tau_nProngLoose = 0;
  tau_ipSigLeadTrk = 0;
  tau_ipSigLeadLooseTrk = 0;
  tau_etOverPtLeadLooseTrk = 0;
  tau_leadLooseTrkPt = 0;
  tau_chrgLooseTrk = 0;
  tau_massTrkSys = 0;
  tau_trkWidth2 = 0;
  tau_trFlightPathSig = 0;
  tau_etEflow = 0;
  tau_mEflow = 0;
  tau_nPi0 = 0;
  tau_ele_E237E277 = 0;
  tau_ele_PresamplerFraction = 0;
  tau_ele_ECALFirstFraction = 0;
  tau_seedCalo_EMRadius = 0;
  tau_seedCalo_hadRadius = 0;
  tau_seedCalo_etEMAtEMScale = 0;
  tau_seedCalo_etHadAtEMScale = 0;
  tau_seedCalo_isolFrac = 0;
  tau_seedCalo_centFrac = 0;
  tau_seedCalo_stripWidth2 = 0;
  tau_seedCalo_nStrip = 0;
  tau_seedCalo_etEMCalib = 0;
  tau_seedCalo_etHadCalib = 0;
  tau_seedCalo_eta = 0;
  tau_seedCalo_phi = 0;
  tau_seedCalo_nIsolLooseTrk = 0;
  tau_seedCalo_trkAvgDist = 0;
  tau_seedCalo_trkRmsDist = 0;
  tau_numTopoClusters = 0;
  tau_numEffTopoClusters = 0;
  tau_topoInvMass = 0;
  tau_effTopoInvMass = 0;
  tau_topoMeanDeltaR = 0;
  tau_effTopoMeanDeltaR = 0;
  tau_numCells = 0;
  tau_seedTrk_EMRadius = 0;
  tau_seedTrk_isolFrac = 0;
  tau_seedTrk_etChrgHadOverSumTrkPt = 0;
  tau_seedTrk_isolFracWide = 0;
  tau_seedTrk_etHadAtEMScale = 0;
  tau_seedTrk_etEMAtEMScale = 0;
  tau_seedTrk_etEMCL = 0;
  tau_seedTrk_etChrgEM = 0;
  tau_seedTrk_etNeuEM = 0;
  tau_seedTrk_etResNeuEM = 0;
  tau_seedTrk_hadLeakEt = 0;
  tau_seedTrk_sumEMCellEtOverLeadTrkPt = 0;
  tau_seedTrk_secMaxStripEt = 0;
  tau_seedTrk_stripWidth2 = 0;
  tau_seedTrk_nStrip = 0;
  tau_seedTrk_etChrgHad = 0;
  tau_seedTrk_nOtherCoreTrk = 0;
  tau_seedTrk_nIsolTrk = 0;
  tau_seedTrk_etIsolEM = 0;
  tau_seedTrk_etIsolHad = 0;
  tau_calcVars_sumTrkPt = 0;
  tau_calcVars_etHadSumPtTracks = 0;
  tau_calcVars_etEMSumPtTracks = 0;
  tau_calcVars_etHad_EMScale_Pt3Trks = 0;
  tau_calcVars_etEM_EMScale_Pt3Trks = 0;
  tau_calcVars_mass = 0;
  tau_calcVars_ipSigLeadLooseTrk = 0;
  tau_calcVars_drMax = 0;
  tau_calcVars_drMin = 0;
  tau_calcVars_emFracCalib = 0;
  tau_calcVars_TRTHTOverLT_LeadTrk = 0;
  tau_calcVars_calRadius = 0;
  tau_calcVars_EMFractionAtEMScale = 0;
  tau_calcVars_BDTLooseBkg = 0;
  tau_calcVars_BDTMediumBkg = 0;
  tau_calcVars_BDTTightBkg = 0;
  tau_seedCalo_track_n = 0;
  tau_seedCalo_wideTrk_n = 0;
  tau_otherTrk_n = 0;
  tau_EF_dr = 0;
  tau_EF_E = 0;
  tau_EF_Et = 0;
  tau_EF_pt = 0;
  tau_EF_eta = 0;
  tau_EF_phi = 0;
  tau_EF_matched = 0;
  tau_L2_dr = 0;
  tau_L2_E = 0;
  tau_L2_Et = 0;
  tau_L2_pt = 0;
  tau_L2_eta = 0;
  tau_L2_phi = 0;
  tau_L2_matched = 0;
  tau_L1_dr = 0;
  tau_L1_Et = 0;
  tau_L1_pt = 0;
  tau_L1_eta = 0;
  tau_L1_phi = 0;
  tau_L1_matched = 0;
  //AntiKt4TopoEM
  jet_AntiKt4TopoEM_E = 0;
  jet_AntiKt4TopoEM_pt = 0;
  jet_AntiKt4TopoEM_m = 0;
  jet_AntiKt4TopoEM_eta = 0;
  jet_AntiKt4TopoEM_phi = 0;
  jet_AntiKt4TopoEM_EtaOrigin = 0;
  jet_AntiKt4TopoEM_PhiOrigin = 0;
  jet_AntiKt4TopoEM_MOrigin = 0;
  jet_AntiKt4TopoEM_EtaOriginEM = 0;
  jet_AntiKt4TopoEM_PhiOriginEM = 0;
  jet_AntiKt4TopoEM_MOriginEM = 0;
  jet_AntiKt4TopoEM_WIDTH = 0;
  jet_AntiKt4TopoEM_n90 = 0;
  jet_AntiKt4TopoEM_Timing = 0;
  jet_AntiKt4TopoEM_LArQuality = 0;
  jet_AntiKt4TopoEM_nTrk = 0;
  jet_AntiKt4TopoEM_sumPtTrk = 0;
  jet_AntiKt4TopoEM_OriginIndex = 0;
  jet_AntiKt4TopoEM_HECQuality = 0;
  jet_AntiKt4TopoEM_NegativeE = 0;
  jet_AntiKt4TopoEM_BCH_CORR_CELL = 0;
  jet_AntiKt4TopoEM_BCH_CORR_DOTX = 0;
  jet_AntiKt4TopoEM_BCH_CORR_JET = 0;
  jet_AntiKt4TopoEM_BCH_CORR_JET_FORCELL = 0;
  jet_AntiKt4TopoEM_ENG_BAD_CELLS = 0;
  jet_AntiKt4TopoEM_N_BAD_CELLS = 0;
  jet_AntiKt4TopoEM_N_BAD_CELLS_CORR = 0;
  jet_AntiKt4TopoEM_BAD_CELLS_CORR_E = 0;
  jet_AntiKt4TopoEM_SamplingMax = 0;
  jet_AntiKt4TopoEM_fracSamplingMax = 0;
  jet_AntiKt4TopoEM_hecf = 0;
  jet_AntiKt4TopoEM_tgap3f = 0;
  jet_AntiKt4TopoEM_isUgly = 0;
  jet_AntiKt4TopoEM_isBadLoose = 0;
  jet_AntiKt4TopoEM_isBadTight = 0;
  jet_AntiKt4TopoEM_emfrac = 0;
  jet_AntiKt4TopoEM_Offset = 0;
  jet_AntiKt4TopoEM_EMJES = 0;
  jet_AntiKt4TopoEM_EMJES_EtaCorr = 0;
  jet_AntiKt4TopoEM_EMJESnooffset = 0;
  jet_AntiKt4TopoEM_GCWJES = 0;
  jet_AntiKt4TopoEM_GCWJES_EtaCorr = 0;
  jet_AntiKt4TopoEM_CB = 0;
  jet_AntiKt4TopoEM_emscale_E = 0;
  jet_AntiKt4TopoEM_emscale_pt = 0;
  jet_AntiKt4TopoEM_emscale_m = 0;
  jet_AntiKt4TopoEM_emscale_eta = 0;
  jet_AntiKt4TopoEM_emscale_phi = 0;
  jet_AntiKt4TopoEM_jvtxf = 0;
  jet_AntiKt4TopoEM_jvtx_x = 0;
  jet_AntiKt4TopoEM_jvtx_y = 0;
  jet_AntiKt4TopoEM_jvtx_z = 0;
  jet_AntiKt4TopoEM_GSCFactorF = 0;
  jet_AntiKt4TopoEM_WidthFraction = 0;
  jet_AntiKt4TopoEM_flavor_component_ip2d_pu = 0;
  jet_AntiKt4TopoEM_flavor_component_ip2d_pb = 0;
  jet_AntiKt4TopoEM_flavor_component_ip2d_ntrk = 0;
  jet_AntiKt4TopoEM_flavor_component_ip3d_pu = 0;
  jet_AntiKt4TopoEM_flavor_component_ip3d_pb = 0;
  jet_AntiKt4TopoEM_flavor_component_ip3d_ntrk = 0;
  jet_AntiKt4TopoEM_flavor_component_sv1_pu = 0;
  jet_AntiKt4TopoEM_flavor_component_sv1_pb = 0;
  jet_AntiKt4TopoEM_flavor_component_sv2_pu = 0;
  jet_AntiKt4TopoEM_flavor_component_sv2_pb = 0;
  jet_AntiKt4TopoEM_flavor_component_svp_mass = 0;
  jet_AntiKt4TopoEM_flavor_component_svp_n2t = 0;
  jet_AntiKt4TopoEM_flavor_component_svp_svok = 0;
  jet_AntiKt4TopoEM_flavor_component_svp_ntrk = 0;
  jet_AntiKt4TopoEM_flavor_component_svp_ntrkv = 0;
  jet_AntiKt4TopoEM_flavor_component_svp_ntrkj = 0;
  jet_AntiKt4TopoEM_flavor_component_svp_efrc = 0;
  jet_AntiKt4TopoEM_flavor_component_sv0p_mass = 0;
  jet_AntiKt4TopoEM_flavor_component_sv0p_n2t = 0;
  jet_AntiKt4TopoEM_flavor_component_sv0p_svok = 0;
  jet_AntiKt4TopoEM_flavor_component_sv0p_ntrk = 0;
  jet_AntiKt4TopoEM_flavor_component_sv0p_ntrkv = 0;
  jet_AntiKt4TopoEM_flavor_component_sv0p_ntrkj = 0;
  jet_AntiKt4TopoEM_flavor_component_sv0p_efrc = 0;
  jet_AntiKt4TopoEM_flavor_component_jfit_pu = 0;
  jet_AntiKt4TopoEM_flavor_component_jfit_pb = 0;
  jet_AntiKt4TopoEM_flavor_component_jfit_nvtxnt = 0;
  jet_AntiKt4TopoEM_flavor_component_jfit_nvtx1t = 0;
  jet_AntiKt4TopoEM_flavor_component_jfit_ntrk = 0;
  jet_AntiKt4TopoEM_flavor_component_jfit_efrc = 0;
  jet_AntiKt4TopoEM_flavor_component_jfit_mass = 0;
  jet_AntiKt4TopoEM_flavor_component_jfit_sig3d = 0;
  jet_AntiKt4TopoEM_flavor_weight_TrackCounting2D = 0;
  jet_AntiKt4TopoEM_flavor_weight_JetProb = 0;
  jet_AntiKt4TopoEM_flavor_weight_IP1D = 0;
  jet_AntiKt4TopoEM_flavor_weight_IP2D = 0;
  jet_AntiKt4TopoEM_flavor_weight_IP3D = 0;
  jet_AntiKt4TopoEM_flavor_weight_SV0 = 0;
  jet_AntiKt4TopoEM_flavor_weight_SV1 = 0;
  jet_AntiKt4TopoEM_flavor_weight_SV2 = 0;
  jet_AntiKt4TopoEM_flavor_weight_JetFitterTag = 0;
  jet_AntiKt4TopoEM_flavor_weight_JetFitterCOMB = 0;
  jet_AntiKt4TopoEM_flavor_weight_JetFitterTagNN = 0;
  jet_AntiKt4TopoEM_flavor_weight_JetFitterCOMBNN = 0;
  jet_AntiKt4TopoEM_flavor_weight_SoftMuonTag = 0;
  jet_AntiKt4TopoEM_flavor_weight_SoftElectronTag = 0;
  jet_AntiKt4TopoEM_flavor_truth_dRminToB = 0;
  jet_AntiKt4TopoEM_flavor_truth_dRminToC = 0;
  jet_AntiKt4TopoEM_flavor_truth_dRminToT = 0;
  jet_AntiKt4TopoEM_flavor_truth_BHadronpdg = 0;
  jet_AntiKt4TopoEM_flavor_truth_trueflav = 0;
  jet_AntiKt4TopoEM_el_dr = 0;
  jet_AntiKt4TopoEM_el_matched = 0;
  jet_AntiKt4TopoEM_mu_dr = 0;
  jet_AntiKt4TopoEM_mu_matched = 0;
  jet_AntiKt4TopoEM_L1_dr = 0;
  jet_AntiKt4TopoEM_L1_matched = 0;
  jet_AntiKt4TopoEM_L2_dr = 0;
  jet_AntiKt4TopoEM_L2_matched = 0;
  jet_AntiKt4TopoEM_EF_dr = 0;
  jet_AntiKt4TopoEM_EF_matched = 0;
  jet_akt6topoem_E = 0;
  jet_akt6topoem_pt = 0;
  jet_akt6topoem_m = 0;
  jet_akt6topoem_eta = 0;
  jet_akt6topoem_phi = 0;
  jet_akt6topoem_EtaOrigin = 0;
  jet_akt6topoem_PhiOrigin = 0;
  jet_akt6topoem_MOrigin = 0;
  jet_akt6topoem_EtaOriginEM = 0;
  jet_akt6topoem_PhiOriginEM = 0;
  jet_akt6topoem_MOriginEM = 0;
  jet_akt6topoem_WIDTH = 0;
  jet_akt6topoem_n90 = 0;
  jet_akt6topoem_Timing = 0;
  jet_akt6topoem_LArQuality = 0;
  jet_akt6topoem_nTrk = 0;
  jet_akt6topoem_sumPtTrk = 0;
  jet_akt6topoem_OriginIndex = 0;
  jet_akt6topoem_HECQuality = 0;
  jet_akt6topoem_NegativeE = 0;
  jet_akt6topoem_BCH_CORR_CELL = 0;
  jet_akt6topoem_BCH_CORR_DOTX = 0;
  jet_akt6topoem_BCH_CORR_JET = 0;
  jet_akt6topoem_BCH_CORR_JET_FORCELL = 0;
  jet_akt6topoem_ENG_BAD_CELLS = 0;
  jet_akt6topoem_N_BAD_CELLS = 0;
  jet_akt6topoem_N_BAD_CELLS_CORR = 0;
  jet_akt6topoem_BAD_CELLS_CORR_E = 0;
  jet_akt6topoem_SamplingMax = 0;
  jet_akt6topoem_fracSamplingMax = 0;
  jet_akt6topoem_hecf = 0;
  jet_akt6topoem_tgap3f = 0;
  jet_akt6topoem_isUgly = 0;
  jet_akt6topoem_isBadLoose = 0;
  jet_akt6topoem_isBadTight = 0;
  jet_akt6topoem_emfrac = 0;
  jet_akt6topoem_Offset = 0;
  jet_akt6topoem_EMJES = 0;
  jet_akt6topoem_EMJES_EtaCorr = 0;
  jet_akt6topoem_EMJESnooffset = 0;
  jet_akt6topoem_GCWJES = 0;
  jet_akt6topoem_GCWJES_EtaCorr = 0;
  jet_akt6topoem_CB = 0;
  jet_akt6topoem_emscale_E = 0;
  jet_akt6topoem_emscale_pt = 0;
  jet_akt6topoem_emscale_m = 0;
  jet_akt6topoem_emscale_eta = 0;
  jet_akt6topoem_emscale_phi = 0;
  jet_akt6topoem_jvtxf = 0;
  jet_akt6topoem_jvtx_x = 0;
  jet_akt6topoem_jvtx_y = 0;
  jet_akt6topoem_jvtx_z = 0;
  jet_akt6topoem_GSCFactorF = 0;
  jet_akt6topoem_WidthFraction = 0;
  jet_akt6topoem_flavor_component_ip2d_pu = 0;
  jet_akt6topoem_flavor_component_ip2d_pb = 0;
  jet_akt6topoem_flavor_component_ip2d_ntrk = 0;
  jet_akt6topoem_flavor_component_ip3d_pu = 0;
  jet_akt6topoem_flavor_component_ip3d_pb = 0;
  jet_akt6topoem_flavor_component_ip3d_ntrk = 0;
  jet_akt6topoem_flavor_component_sv1_pu = 0;
  jet_akt6topoem_flavor_component_sv1_pb = 0;
  jet_akt6topoem_flavor_component_sv2_pu = 0;
  jet_akt6topoem_flavor_component_sv2_pb = 0;
  jet_akt6topoem_flavor_component_svp_mass = 0;
  jet_akt6topoem_flavor_component_svp_n2t = 0;
  jet_akt6topoem_flavor_component_svp_svok = 0;
  jet_akt6topoem_flavor_component_svp_ntrk = 0;
  jet_akt6topoem_flavor_component_svp_ntrkv = 0;
  jet_akt6topoem_flavor_component_svp_ntrkj = 0;
  jet_akt6topoem_flavor_component_svp_efrc = 0;
  jet_akt6topoem_flavor_component_sv0p_mass = 0;
  jet_akt6topoem_flavor_component_sv0p_n2t = 0;
  jet_akt6topoem_flavor_component_sv0p_svok = 0;
  jet_akt6topoem_flavor_component_sv0p_ntrk = 0;
  jet_akt6topoem_flavor_component_sv0p_ntrkv = 0;
  jet_akt6topoem_flavor_component_sv0p_ntrkj = 0;
  jet_akt6topoem_flavor_component_sv0p_efrc = 0;
  jet_akt6topoem_flavor_component_jfit_pu = 0;
  jet_akt6topoem_flavor_component_jfit_pb = 0;
  jet_akt6topoem_flavor_component_jfit_nvtxnt = 0;
  jet_akt6topoem_flavor_component_jfit_nvtx1t = 0;
  jet_akt6topoem_flavor_component_jfit_ntrk = 0;
  jet_akt6topoem_flavor_component_jfit_efrc = 0;
  jet_akt6topoem_flavor_component_jfit_mass = 0;
  jet_akt6topoem_flavor_component_jfit_sig3d = 0;
  jet_akt6topoem_flavor_weight_TrackCounting2D = 0;
  jet_akt6topoem_flavor_weight_JetProb = 0;
  jet_akt6topoem_flavor_weight_IP1D = 0;
  jet_akt6topoem_flavor_weight_IP2D = 0;
  jet_akt6topoem_flavor_weight_IP3D = 0;
  jet_akt6topoem_flavor_weight_SV0 = 0;
  jet_akt6topoem_flavor_weight_SV1 = 0;
  jet_akt6topoem_flavor_weight_SV2 = 0;
  jet_akt6topoem_flavor_weight_JetFitterTag = 0;
  jet_akt6topoem_flavor_weight_JetFitterCOMB = 0;
  jet_akt6topoem_flavor_weight_JetFitterTagNN = 0;
  jet_akt6topoem_flavor_weight_JetFitterCOMBNN = 0;
  jet_akt6topoem_flavor_weight_SoftMuonTag = 0;
  jet_akt6topoem_flavor_weight_SoftElectronTag = 0;
  jet_akt6topoem_flavor_truth_dRminToB = 0;
  jet_akt6topoem_flavor_truth_dRminToC = 0;
  jet_akt6topoem_flavor_truth_dRminToT = 0;
  jet_akt6topoem_flavor_truth_BHadronpdg = 0;
  jet_akt6topoem_flavor_truth_trueflav = 0;
  jet_akt6topoem_el_dr = 0;
  jet_akt6topoem_el_matched = 0;
  jet_akt6topoem_mu_dr = 0;
  jet_akt6topoem_mu_matched = 0;
  jet_akt6topoem_L1_dr = 0;
  jet_akt6topoem_L1_matched = 0;
  jet_akt6topoem_L2_dr = 0;
  jet_akt6topoem_L2_matched = 0;
  jet_akt6topoem_EF_dr = 0;
  jet_akt6topoem_EF_matched = 0;
  cl_pt = 0;
  cl_eta = 0;
  cl_phi = 0;
  clpt10_pt = 0;
  clpt10_eta = 0;
  clpt10_phi = 0;
  clpt10_E_em = 0;
  clpt10_E_had = 0;
  clpt10_firstEdens = 0;
  clpt10_cellmaxfrac = 0;
  clpt10_longitudinal = 0;
  clpt10_secondlambda = 0;
  clpt10_lateral = 0;
  clpt10_secondR = 0;
  clpt10_centerlambda = 0;
  clpt10_deltaTheta = 0;
  clpt10_deltaPhi = 0;
  clpt10_time = 0;
  clpt10_E_PreSamplerB = 0;
  clpt10_E_EMB1 = 0;
  clpt10_E_EMB2 = 0;
  clpt10_E_EMB3 = 0;
  clpt10_E_PreSamplerE = 0;
  clpt10_E_EME1 = 0;
  clpt10_E_EME2 = 0;
  clpt10_E_EME3 = 0;
  clpt10_E_HEC0 = 0;
  clpt10_E_HEC1 = 0;
  clpt10_E_HEC2 = 0;
  clpt10_E_HEC3 = 0;
  clpt10_E_TileBar0 = 0;
  clpt10_E_TileBar1 = 0;
  clpt10_E_TileBar2 = 0;
  clpt10_E_TileGap1 = 0;
  clpt10_E_TileGap2 = 0;
  clpt10_E_TileGap3 = 0;
  clpt10_E_TileExt0 = 0;
  clpt10_E_TileExt1 = 0;
  clpt10_E_TileExt2 = 0;
  clpt10_E_FCAL0 = 0;
  clpt10_E_FCAL1 = 0;
  clpt10_E_FCAL2 = 0;
  emclpt10_pt = 0;
  emclpt10_eta = 0;
  emclpt10_phi = 0;
  emclpt10_E_em = 0;
  emclpt10_E_had = 0;
  emclpt10_firstEdens = 0;
  emclpt10_cellmaxfrac = 0;
  emclpt10_longitudinal = 0;
  emclpt10_secondlambda = 0;
  emclpt10_lateral = 0;
  emclpt10_secondR = 0;
  emclpt10_centerlambda = 0;
  emclpt10_deltaTheta = 0;
  emclpt10_deltaPhi = 0;
  emclpt10_time = 0;
  emclpt10_E_PreSamplerB = 0;
  emclpt10_E_EMB1 = 0;
  emclpt10_E_EMB2 = 0;
  emclpt10_E_EMB3 = 0;
  emclpt10_E_PreSamplerE = 0;
  emclpt10_E_EME1 = 0;
  emclpt10_E_EME2 = 0;
  emclpt10_E_EME3 = 0;
  emclpt10_E_HEC0 = 0;
  emclpt10_E_HEC1 = 0;
  emclpt10_E_HEC2 = 0;
  emclpt10_E_HEC3 = 0;
  emclpt10_E_TileBar0 = 0;
  emclpt10_E_TileBar1 = 0;
  emclpt10_E_TileBar2 = 0;
  emclpt10_E_TileGap1 = 0;
  emclpt10_E_TileGap2 = 0;
  emclpt10_E_TileGap3 = 0;
  emclpt10_E_TileExt0 = 0;
  emclpt10_E_TileExt1 = 0;
  emclpt10_E_TileExt2 = 0;
  emclpt10_E_FCAL0 = 0;
  emclpt10_E_FCAL1 = 0;
  emclpt10_E_FCAL2 = 0;
  emclpt10_eta_PreSamplerB = 0;
  emclpt10_phi_PreSamplerB = 0;
  emclpt10_eta_EMB1 = 0;
  emclpt10_phi_EMB1 = 0;
  emclpt10_eta_EMB2 = 0;
  emclpt10_phi_EMB2 = 0;
  emclpt10_eta_EMB3 = 0;
  emclpt10_phi_EMB3 = 0;
  emclpt10_eta_PreSamplerE = 0;
  emclpt10_phi_PreSamplerE = 0;
  emclpt10_eta_EME1 = 0;
  emclpt10_phi_EME1 = 0;
  emclpt10_eta_EME2 = 0;
  emclpt10_phi_EME2 = 0;
  emclpt10_eta_EME3 = 0;
  emclpt10_phi_EME3 = 0;
  emclpt10_eta_HEC0 = 0;
  emclpt10_phi_HEC0 = 0;
  emclpt10_eta_HEC1 = 0;
  emclpt10_phi_HEC1 = 0;
  emclpt10_eta_HEC2 = 0;
  emclpt10_phi_HEC2 = 0;
  emclpt10_eta_HEC3 = 0;
  emclpt10_phi_HEC3 = 0;
  emclpt10_eta_TileBar0 = 0;
  emclpt10_phi_TileBar0 = 0;
  emclpt10_eta_TileBar1 = 0;
  emclpt10_phi_TileBar1 = 0;
  emclpt10_eta_TileBar2 = 0;
  emclpt10_phi_TileBar2 = 0;
  emclpt10_eta_TileGap1 = 0;
  emclpt10_phi_TileGap1 = 0;
  emclpt10_eta_TileGap2 = 0;
  emclpt10_phi_TileGap2 = 0;
  emclpt10_eta_TileGap3 = 0;
  emclpt10_phi_TileGap3 = 0;
  emclpt10_eta_TileExt0 = 0;
  emclpt10_phi_TileExt0 = 0;
  emclpt10_eta_TileExt1 = 0;
  emclpt10_phi_TileExt1 = 0;
  emclpt10_eta_TileExt2 = 0;
  emclpt10_phi_TileExt2 = 0;
  emclpt10_eta_FCAL0 = 0;
  emclpt10_phi_FCAL0 = 0;
  emclpt10_eta_FCAL1 = 0;
  emclpt10_phi_FCAL1 = 0;
  emclpt10_eta_FCAL2 = 0;
  emclpt10_phi_FCAL2 = 0;
  trk_pt = 0;
  trk_eta = 0;
  trk_d0_wrtPV = 0;
  trk_z0_wrtPV = 0;
  trk_phi_wrtPV = 0;
  trk_theta_wrtPV = 0;
  trk_qoverp_wrtPV = 0;
  trk_chi2 = 0;
  trk_ndof = 0;
  trkpt5_d0 = 0;
  trkpt5_z0 = 0;
  trkpt5_phi = 0;
  trkpt5_theta = 0;
  trkpt5_qoverp = 0;
  trkpt5_pt = 0;
  trkpt5_eta = 0;
  trkpt5_err_d0 = 0;
  trkpt5_err_z0 = 0;
  trkpt5_err_phi = 0;
  trkpt5_err_theta = 0;
  trkpt5_err_qoverp = 0;
  trkpt5_d0_wrtPV = 0;
  trkpt5_z0_wrtPV = 0;
  trkpt5_phi_wrtPV = 0;
  trkpt5_err_d0_wrtPV = 0;
  trkpt5_err_z0_wrtPV = 0;
  trkpt5_err_phi_wrtPV = 0;
  trkpt5_err_theta_wrtPV = 0;
  trkpt5_err_qoverp_wrtPV = 0;
  trkpt5_cov_d0_z0_wrtPV = 0;
  trkpt5_cov_d0_phi_wrtPV = 0;
  trkpt5_cov_d0_theta_wrtPV = 0;
  trkpt5_cov_d0_qoverp_wrtPV = 0;
  trkpt5_cov_z0_phi_wrtPV = 0;
  trkpt5_cov_z0_theta_wrtPV = 0;
  trkpt5_cov_z0_qoverp_wrtPV = 0;
  trkpt5_cov_phi_theta_wrtPV = 0;
  trkpt5_cov_phi_qoverp_wrtPV = 0;
  trkpt5_cov_theta_qoverp_wrtPV = 0;
  trkpt5_chi2 = 0;
  trkpt5_ndof = 0;
  trkpt5_nBLHits = 0;
  trkpt5_nPixHits = 0;
  trkpt5_nSCTHits = 0;
  trkpt5_nTRTHits = 0;
  trkpt5_nPixHoles = 0;
  trkpt5_nSCTHoles = 0;
  trkpt5_nMDTHits = 0;
  trkpt5_nCSCEtaHits = 0;
  trkpt5_nCSCPhiHits = 0;
  trkpt5_nRPCEtaHits = 0;
  trkpt5_nRPCPhiHits = 0;
  trkpt5_nTGCEtaHits = 0;
  trkpt5_nTGCPhiHits = 0;
  trkpt5_nHits = 0;
  trkpt5_nHoles = 0;
  trkpt5_hitPattern = 0;
  trkpt5_TRTHighTHitsRatio = 0;
  trkpt5_pixeldEdx = 0;
  trkpt5_mc_probability = 0;
  trkpt5_mc_barcode = 0;
  vxp_x = 0;
  vxp_y = 0;
  vxp_z = 0;
  vxp_cov_x = 0;
  vxp_cov_y = 0;
  vxp_cov_z = 0;
  vxp_chi2 = 0;
  vxp_ndof = 0;
  vxp_px = 0;
  vxp_py = 0;
  vxp_pz = 0;
  vxp_E = 0;
  vxp_m = 0;
  vxp_nTracks = 0;
  vxp_sumPt = 0;
  vxp_type = 0;
  vxp_trk_n = 0;
  vxp_trk_index = 0;
  mb_E = 0;
  mb_eta = 0;
  mb_phi = 0;
  mb_time = 0;
  mb_quality = 0;
  mb_type = 0;
  mb_module = 0;
  mb_channel = 0;
  muonTruth_pt = 0;
  muonTruth_m = 0;
  muonTruth_eta = 0;
  muonTruth_phi = 0;
  muonTruth_charge = 0;
  muonTruth_PDGID = 0;
  muonTruth_barcode = 0;
  muonTruth_type = 0;
  muonTruth_origin = 0;
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
  mc_pt = 0;
  mc_m = 0;
  mc_eta = 0;
  mc_phi = 0;
  mc_status = 0;
  mc_barcode = 0;
  mc_parents = 0;
  mc_children = 0;
  mc_pdgId = 0;
  mc_charge = 0;
  mc_vx_x = 0;
  mc_vx_y = 0;
  mc_vx_z = 0;
  mc_child_index = 0;
  mc_parent_index = 0;
  trig_L2_combmuonfeature_L2_mu18_MG=0;
  trig_L2_combmuonfeature_n=0;
  trig_L2_combmuonfeature_pt=0;
  trig_L2_combmuonfeature_eta=0;
  trig_L2_combmuonfeature_phi=0;
  trig_EF_trigmugirl_n=0;
  trig_EF_trigmugirl_track_n=0;
  trig_EF_trigmugirl_track_CB_pt=0;
  trig_EF_trigmugirl_track_CB_eta=0;
  trig_EF_trigmugirl_track_CB_phi=0;
  trig_L2_muonfeature_L2_mu40_MSonly_barrel=0;
  trig_L2_muonfeature_n=0;
  trig_L2_muonfeature_pt=0;
  trig_L2_muonfeature_eta=0;
  trig_L2_muonfeature_phi=0;
  trig_EF_trigmuonef_n=0;
  trig_EF_trigmuonef_track_n=0;
  trig_EF_trigmuonef_track_SA_pt=0;
  trig_EF_trigmuonef_track_SA_eta=0;
  trig_EF_trigmuonef_track_SA_phi=0;
  trig_EF_el_n=0;
  trig_EF_el_EF_e20_medium=0;
  trig_EF_el_eta=0;
  trig_EF_el_phi=0;


  // Set branch addresses and branch pointers
  if (!tree) return;
  fChain = tree;
  fCurrent = -1;
  fChain->SetMakeClass(1);

  fChain->SetBranchStatus("*",0);
  fChain->SetBranchStatus("RunNumber*",1);
  fChain->SetBranchStatus("EventNumber*",1);
  // trigger
  fChain->SetBranchStatus("EF_e20_loose*",1);
  fChain->SetBranchStatus("EF_e10_medium*",1);
  fChain->SetBranchStatus("EF_e10_loose*",1);
  fChain->SetBranchStatus("EF_e15_medium*",1);
  fChain->SetBranchStatus("EF_mu13*",1);
  fChain->SetBranchStatus("EF_mu10*",1);
  fChain->SetBranchStatus("EF_mu6*",1);
  fChain->SetBranchStatus("EF_mu4*",1);
  fChain->SetBranchStatus("EF_g*",1);
  fChain->SetBranchStatus("L1_EM3*",1);
  fChain->SetBranchStatus("L1_MU6*",1);
  fChain->SetBranchStatus("L1_EM5*",1);
  fChain->SetBranchStatus("L1_EM10*",1);
  fChain->SetBranchStatus("L1_EM14*",1);
  fChain->SetBranchStatus("L1_MU0*",1);
  fChain->SetBranchStatus("L1_MU10*",1);
  fChain->SetBranchStatus("EF_e20_medium*",1);
  fChain->SetBranchStatus("EF_e25_medium*",1);
  fChain->SetBranchStatus("EF_e30_loose*",1);
  fChain->SetBranchStatus("EF_2e10_medium*",1);
  fChain->SetBranchStatus("EF_e10_loose_mu6*",1);
  fChain->SetBranchStatus("EF_mu15_MG*",1);
  fChain->SetBranchStatus("EF_mu18*",1);
  fChain->SetBranchStatus("EF_mu20*",1);
  fChain->SetBranchStatus("EF_mu40*",1);
  fChain->SetBranchStatus("EF_2mu10*",1);
  fChain->SetBranchStatus("lbn*",1);
  // electrons/photons
  fChain->SetBranchStatus("el_*",1);
  fChain->SetBranchStatus("ph_*",1);
  // fChain->SetBranchStatus("emclpt10_*",1); // temp
  // muons
  fChain->SetBranchStatus("mu_staco*",1);
  fChain->SetBranchStatus("mu_muid*",1);
  fChain->SetBranchStatus("mu_calo*",1);
  // jets
  fChain->SetBranchStatus("jet_AntiKt4TopoEM_n",1);
  fChain->SetBranchStatus("jet_AntiKt4TopoEM_eta*",1);
  fChain->SetBranchStatus("jet_AntiKt4TopoEM_phi*",1);
  fChain->SetBranchStatus("jet_AntiKt4TopoEM_pt*",1);
  fChain->SetBranchStatus("jet_AntiKt4TopoEM_m*",1);
  fChain->SetBranchStatus("jet_AntiKt4TopoEM_E*",1);
  fChain->SetBranchStatus("jet_AntiKt4TopoEM_emscale_eta*",1);
  fChain->SetBranchStatus("jet_AntiKt4TopoEM_emscale_phi*",1);
  fChain->SetBranchStatus("jet_AntiKt4TopoEM_emscale_pt*",1);
  fChain->SetBranchStatus("jet_AntiKt4TopoEM_emscale_m*",1);
  fChain->SetBranchStatus("jet_AntiKt4TopoEM_emscale_E*",1);
  fChain->SetBranchStatus("jet_AntiKt4TopoEM_jvtxf*",1);
  fChain->SetBranchStatus("jet_AntiKt4TopoEM_n90*",1);
  fChain->SetBranchStatus("jet_AntiKt4TopoEM_emfrac*",1);
  fChain->SetBranchStatus("jet_AntiKt4TopoEM_LArQuality*",1);
  fChain->SetBranchStatus("jet_AntiKt4TopoEM_HECQuality*",1);
  fChain->SetBranchStatus("jet_AntiKt4TopoEM_sumPtTrk*",1);
  fChain->SetBranchStatus("jet_AntiKt4TopoEM_NegativeE*",1);
  fChain->SetBranchStatus("jet_AntiKt4TopoEM_EtaOrigin*",1);
  fChain->SetBranchStatus("jet_AntiKt4TopoEM_PhiOrigin*",1);
  fChain->SetBranchStatus("jet_AntiKt4TopoEM_MOrigin*",1);
  fChain->SetBranchStatus("jet_AntiKt4TopoEM_Timing*",1);
  fChain->SetBranchStatus("jet_AntiKt4TopoEM_hecf*",1);
  fChain->SetBranchStatus("jet_AntiKt4TopoEM_tgap3f*",1);
  fChain->SetBranchStatus("jet_AntiKt4TopoEM_BCH_CORR_CELL*",1);
  fChain->SetBranchStatus("jet_AntiKt4TopoEM_EMJES*",1);
  fChain->SetBranchStatus("jet_AntiKt4TopoEM_fracSamplingMax*",1);
  fChain->SetBranchStatus("jet_AntiKt4TopoEM_flavor_weight_SV0*",1);
  fChain->SetBranchStatus("jet_AntiKt4TopoEM_isBadLoose*",1);
  // fChain->SetBranchStatus("jet_akt6topoem_n",1);
  // fChain->SetBranchStatus("jet_akt6topoem_eta*",1);
  // fChain->SetBranchStatus("jet_akt6topoem_phi*",1);
  // fChain->SetBranchStatus("jet_akt6topoem_pt*",1);
  // fChain->SetBranchStatus("jet_akt6topoem_m*",1);
  // fChain->SetBranchStatus("jet_akt6topoem_E*",1);
  // fChain->SetBranchStatus("jet_akt6topoem_emscale_eta*",1);
  // fChain->SetBranchStatus("jet_akt6topoem_emscale_phi*",1);
  // fChain->SetBranchStatus("jet_akt6topoem_emscale_pt*",1);
  // fChain->SetBranchStatus("jet_akt6topoem_emscale_m*",1);
  // fChain->SetBranchStatus("jet_akt6topoem_emscale_E*",1);
  // fChain->SetBranchStatus("jet_akt6topoem_jvtxf*",1);
  // fChain->SetBranchStatus("jet_akt6topoem_n90*",1);
  // fChain->SetBranchStatus("jet_akt6topoem_emfrac*",1);
  // fChain->SetBranchStatus("jet_akt6topoem_LArQuality*",1);
  // fChain->SetBranchStatus("jet_akt6topoem_HECQuality*",1);
  // fChain->SetBranchStatus("jet_akt6topoem_sumPtTrk*",1);
  // fChain->SetBranchStatus("jet_akt6topoem_NegativeE*",1);
  // fChain->SetBranchStatus("jet_akt6topoem_Timing*",1);
  // fChain->SetBranchStatus("jet_akt6topoem_EtaOrigin*",1);
  // fChain->SetBranchStatus("jet_akt6topoem_PhiOrigin*",1);
  // fChain->SetBranchStatus("jet_akt6topoem_MOrigin*",1);
  // fChain->SetBranchStatus("jet_akt6topoem_BCH_CORR_CELL*",1);
  // fChain->SetBranchStatus("jet_akt6topoem_hecf*",1);
  // fChain->SetBranchStatus("jet_akt6topoem_tgap3f*",1);
  // fChain->SetBranchStatus("jet_akt6topoem_EMJES*",1);
  // fChain->SetBranchStatus("jet_akt6topoem_fracSamplingMax*",1);
  // fChain->SetBranchStatus("jet_akt6topoem_isBadLoose*",1);
   // MET
  fChain->SetBranchStatus("MET_Final_*",1);
  fChain->SetBranchStatus("MET_RefFinal_*",1);
  fChain->SetBranchStatus("MET_Topo_*",1);
  fChain->SetBranchStatus("MET_LocHadTopo_*",1);
  fChain->SetBranchStatus("MET_MuonBoy_*",1);
  fChain->SetBranchStatus("MET_RefMuon_*",1);
  // mc truth tree
  fChain->SetBranchStatus("mc_n*",1);
  fChain->SetBranchStatus("mc_pt*",1);
  fChain->SetBranchStatus("mc_m*",1);
  fChain->SetBranchStatus("mc_eta*",1);
  fChain->SetBranchStatus("mc_phi*",1);
  fChain->SetBranchStatus("mc_status*",1);
  fChain->SetBranchStatus("mc_barcode*",1);
  fChain->SetBranchStatus("mc_parents*",1);
  fChain->SetBranchStatus("mc_parent_index*",1);
  fChain->SetBranchStatus("mc_child_index*",1);
  fChain->SetBranchStatus("mc_children*",1);
  fChain->SetBranchStatus("mc_pdgId*",1);
  fChain->SetBranchStatus("mc_charge*",1);
  // mc evt tree
  fChain->SetBranchStatus("mcevt_*",1);
  // vertices
  fChain->SetBranchStatus("vxp*",1);
  // tracks
  fChain->SetBranchStatus("trkpt5*",1);
  // taus
  // fChain->SetBranchStatus("tau*",1);
  // trigger 
  fChain->SetBranchStatus("trig*",1);

  if( fChain->GetBranchStatus("RunNumber") ) { fChain->SetBranchAddress("RunNumber", &RunNumber, &b_RunNumber); }
  if( fChain->GetBranchStatus("EventNumber") ) { fChain->SetBranchAddress("EventNumber", &EventNumber, &b_EventNumber); }
  if( fChain->GetBranchStatus("timestamp") ) { fChain->SetBranchAddress("timestamp", &timestamp, &b_timestamp); }
  if( fChain->GetBranchStatus("timestamp_ns") ) { fChain->SetBranchAddress("timestamp_ns", &timestamp_ns, &b_timestamp_ns); }
  if( fChain->GetBranchStatus("lbn") ) { fChain->SetBranchAddress("lbn", &lbn, &b_lbn); }
  if( fChain->GetBranchStatus("bcid") ) { fChain->SetBranchAddress("bcid", &bcid, &b_bcid); }
  if( fChain->GetBranchStatus("detmask0") ) { fChain->SetBranchAddress("detmask0", &detmask0, &b_detmask0); }
  if( fChain->GetBranchStatus("detmask1") ) { fChain->SetBranchAddress("detmask1", &detmask1, &b_detmask1); }
  if( fChain->GetBranchStatus("pixelFlags") ) { fChain->SetBranchAddress("pixelFlags", &pixelFlags, &b_pixelFlags); }
  if( fChain->GetBranchStatus("sctFlags") ) { fChain->SetBranchAddress("sctFlags", &sctFlags, &b_sctFlags); }
  if( fChain->GetBranchStatus("trtFlags") ) { fChain->SetBranchAddress("trtFlags", &trtFlags, &b_trtFlags); }
  if( fChain->GetBranchStatus("larFlags") ) { fChain->SetBranchAddress("larFlags", &larFlags, &b_larFlags); }
  if( fChain->GetBranchStatus("tileFlags") ) { fChain->SetBranchAddress("tileFlags", &tileFlags, &b_tileFlags); }
  if( fChain->GetBranchStatus("muonFlags") ) { fChain->SetBranchAddress("muonFlags", &muonFlags, &b_muonFlags); }
  if( fChain->GetBranchStatus("fwdFlags") ) { fChain->SetBranchAddress("fwdFlags", &fwdFlags, &b_fwdFlags); }
  if( fChain->GetBranchStatus("coreFlags") ) { fChain->SetBranchAddress("coreFlags", &coreFlags, &b_coreFlags); }
  if( fChain->GetBranchStatus("pixelError") ) { fChain->SetBranchAddress("pixelError", &pixelError, &b_pixelError); }
  if( fChain->GetBranchStatus("sctError") ) { fChain->SetBranchAddress("sctError", &sctError, &b_sctError); }
  if( fChain->GetBranchStatus("trtError") ) { fChain->SetBranchAddress("trtError", &trtError, &b_trtError); }
  if( fChain->GetBranchStatus("larError") ) { fChain->SetBranchAddress("larError", &larError, &b_larError); }
  if( fChain->GetBranchStatus("tileError") ) { fChain->SetBranchAddress("tileError", &tileError, &b_tileError); }
  if( fChain->GetBranchStatus("muonError") ) { fChain->SetBranchAddress("muonError", &muonError, &b_muonError); }
  if( fChain->GetBranchStatus("fwdError") ) { fChain->SetBranchAddress("fwdError", &fwdError, &b_fwdError); }
  if( fChain->GetBranchStatus("coreError") ) { fChain->SetBranchAddress("coreError", &coreError, &b_coreError); }
  if( fChain->GetBranchStatus("el_n") ) { fChain->SetBranchAddress("el_n", &el_n, &b_el_n); }
  if( fChain->GetBranchStatus("el_E") ) { fChain->SetBranchAddress("el_E", &el_E, &b_el_E); }
  if( fChain->GetBranchStatus("el_Et") ) { fChain->SetBranchAddress("el_Et", &el_Et, &b_el_Et); }
  if( fChain->GetBranchStatus("el_pt") ) { fChain->SetBranchAddress("el_pt", &el_pt, &b_el_pt); }
  if( fChain->GetBranchStatus("el_m") ) { fChain->SetBranchAddress("el_m", &el_m, &b_el_m); }
  if( fChain->GetBranchStatus("el_eta") ) { fChain->SetBranchAddress("el_eta", &el_eta, &b_el_eta); }
  if( fChain->GetBranchStatus("el_phi") ) { fChain->SetBranchAddress("el_phi", &el_phi, &b_el_phi); }
  if( fChain->GetBranchStatus("el_px") ) { fChain->SetBranchAddress("el_px", &el_px, &b_el_px); }
  if( fChain->GetBranchStatus("el_py") ) { fChain->SetBranchAddress("el_py", &el_py, &b_el_py); }
  if( fChain->GetBranchStatus("el_pz") ) { fChain->SetBranchAddress("el_pz", &el_pz, &b_el_pz); }
  if( fChain->GetBranchStatus("el_charge") ) { fChain->SetBranchAddress("el_charge", &el_charge, &b_el_charge); }
  if( fChain->GetBranchStatus("el_author") ) { fChain->SetBranchAddress("el_author", &el_author, &b_el_author); }
  if( fChain->GetBranchStatus("el_isEM") ) { fChain->SetBranchAddress("el_isEM", &el_isEM, &b_el_isEM); }
  if( fChain->GetBranchStatus("el_OQ") ) { fChain->SetBranchAddress("el_OQ", &el_OQ, &b_el_OQ); }
  if( fChain->GetBranchStatus("el_convFlag") ) { fChain->SetBranchAddress("el_convFlag", &el_convFlag, &b_el_convFlag); }
  if( fChain->GetBranchStatus("el_isConv") ) { fChain->SetBranchAddress("el_isConv", &el_isConv, &b_el_isConv); }
  if( fChain->GetBranchStatus("el_nConv") ) { fChain->SetBranchAddress("el_nConv", &el_nConv, &b_el_nConv); }
  if( fChain->GetBranchStatus("el_nSingleTrackConv") ) { fChain->SetBranchAddress("el_nSingleTrackConv", &el_nSingleTrackConv, &b_el_nSingleTrackConv); }
  if( fChain->GetBranchStatus("el_nDoubleTrackConv") ) { fChain->SetBranchAddress("el_nDoubleTrackConv", &el_nDoubleTrackConv, &b_el_nDoubleTrackConv); }
  if( fChain->GetBranchStatus("el_type") ) { fChain->SetBranchAddress("el_type", &el_type, &b_el_type); }
  if( fChain->GetBranchStatus("el_origin") ) { fChain->SetBranchAddress("el_origin", &el_origin, &b_el_origin); }
  if( fChain->GetBranchStatus("el_truth_E") ) { fChain->SetBranchAddress("el_truth_E", &el_truth_E, &b_el_truth_E); }
  if( fChain->GetBranchStatus("el_truth_pt") ) { fChain->SetBranchAddress("el_truth_pt", &el_truth_pt, &b_el_truth_pt); }
  if( fChain->GetBranchStatus("el_truth_eta") ) { fChain->SetBranchAddress("el_truth_eta", &el_truth_eta, &b_el_truth_eta); }
  if( fChain->GetBranchStatus("el_truth_phi") ) { fChain->SetBranchAddress("el_truth_phi", &el_truth_phi, &b_el_truth_phi); }
  if( fChain->GetBranchStatus("el_truth_type") ) { fChain->SetBranchAddress("el_truth_type", &el_truth_type, &b_el_truth_type); }
  if( fChain->GetBranchStatus("el_truth_status") ) { fChain->SetBranchAddress("el_truth_status", &el_truth_status, &b_el_truth_status); }
  if( fChain->GetBranchStatus("el_truth_barcode") ) { fChain->SetBranchAddress("el_truth_barcode", &el_truth_barcode, &b_el_truth_barcode); }
  if( fChain->GetBranchStatus("el_truth_mothertype") ) { fChain->SetBranchAddress("el_truth_mothertype", &el_truth_mothertype, &b_el_truth_mothertype); }
  if( fChain->GetBranchStatus("el_truth_motherbarcode") ) { fChain->SetBranchAddress("el_truth_motherbarcode", &el_truth_motherbarcode, &b_el_truth_motherbarcode); }
  if( fChain->GetBranchStatus("el_truth_hasHardBrem") ) { fChain->SetBranchAddress("el_truth_hasHardBrem", &el_truth_hasHardBrem, &b_el_truth_hasHardBrem); }
  if( fChain->GetBranchStatus("el_truth_index") ) { fChain->SetBranchAddress("el_truth_index", &el_truth_index, &b_el_truth_index); }
  if( fChain->GetBranchStatus("el_truth_matched") ) { fChain->SetBranchAddress("el_truth_matched", &el_truth_matched, &b_el_truth_matched); }
  if( fChain->GetBranchStatus("el_loose") ) { fChain->SetBranchAddress("el_loose", &el_loose, &b_el_loose); }
  if( fChain->GetBranchStatus("el_medium") ) { fChain->SetBranchAddress("el_medium", &el_medium, &b_el_medium); }
  if( fChain->GetBranchStatus("el_mediumIso") ) { fChain->SetBranchAddress("el_mediumIso", &el_mediumIso, &b_el_mediumIso); }
  if( fChain->GetBranchStatus("el_tight") ) { fChain->SetBranchAddress("el_tight", &el_tight, &b_el_tight); }
  if( fChain->GetBranchStatus("el_tightIso") ) { fChain->SetBranchAddress("el_tightIso", &el_tightIso, &b_el_tightIso); }
  if( fChain->GetBranchStatus("el_goodOQ") ) { fChain->SetBranchAddress("el_goodOQ", &el_goodOQ, &b_el_goodOQ); }
  if( fChain->GetBranchStatus("el_Ethad") ) { fChain->SetBranchAddress("el_Ethad", &el_Ethad, &b_el_Ethad); }
  if( fChain->GetBranchStatus("el_Ethad1") ) { fChain->SetBranchAddress("el_Ethad1", &el_Ethad1, &b_el_Ethad1); }
  if( fChain->GetBranchStatus("el_f1") ) { fChain->SetBranchAddress("el_f1", &el_f1, &b_el_f1); }
  if( fChain->GetBranchStatus("el_f1core") ) { fChain->SetBranchAddress("el_f1core", &el_f1core, &b_el_f1core); }
  if( fChain->GetBranchStatus("el_Emins1") ) { fChain->SetBranchAddress("el_Emins1", &el_Emins1, &b_el_Emins1); }
  if( fChain->GetBranchStatus("el_fside") ) { fChain->SetBranchAddress("el_fside", &el_fside, &b_el_fside); }
  if( fChain->GetBranchStatus("el_Emax2") ) { fChain->SetBranchAddress("el_Emax2", &el_Emax2, &b_el_Emax2); }
  if( fChain->GetBranchStatus("el_ws3") ) { fChain->SetBranchAddress("el_ws3", &el_ws3, &b_el_ws3); }
  if( fChain->GetBranchStatus("el_wstot") ) { fChain->SetBranchAddress("el_wstot", &el_wstot, &b_el_wstot); }
  if( fChain->GetBranchStatus("el_emaxs1") ) { fChain->SetBranchAddress("el_emaxs1", &el_emaxs1, &b_el_emaxs1); }
  if( fChain->GetBranchStatus("el_deltaEs") ) { fChain->SetBranchAddress("el_deltaEs", &el_deltaEs, &b_el_deltaEs); }
  if( fChain->GetBranchStatus("el_E233") ) { fChain->SetBranchAddress("el_E233", &el_E233, &b_el_E233); }
  if( fChain->GetBranchStatus("el_E237") ) { fChain->SetBranchAddress("el_E237", &el_E237, &b_el_E237); }
  if( fChain->GetBranchStatus("el_E277") ) { fChain->SetBranchAddress("el_E277", &el_E277, &b_el_E277); }
  if( fChain->GetBranchStatus("el_weta2") ) { fChain->SetBranchAddress("el_weta2", &el_weta2, &b_el_weta2); }
  if( fChain->GetBranchStatus("el_f3") ) { fChain->SetBranchAddress("el_f3", &el_f3, &b_el_f3); }
  if( fChain->GetBranchStatus("el_f3core") ) { fChain->SetBranchAddress("el_f3core", &el_f3core, &b_el_f3core); }
  if( fChain->GetBranchStatus("el_rphiallcalo") ) { fChain->SetBranchAddress("el_rphiallcalo", &el_rphiallcalo, &b_el_rphiallcalo); }
  if( fChain->GetBranchStatus("el_Etcone45") ) { fChain->SetBranchAddress("el_Etcone45", &el_Etcone45, &b_el_Etcone45); }
  if( fChain->GetBranchStatus("el_Etcone20") ) { fChain->SetBranchAddress("el_Etcone20", &el_Etcone20, &b_el_Etcone20); }
  if( fChain->GetBranchStatus("el_Etcone30") ) { fChain->SetBranchAddress("el_Etcone30", &el_Etcone30, &b_el_Etcone30); }
  if( fChain->GetBranchStatus("el_Etcone40") ) { fChain->SetBranchAddress("el_Etcone40", &el_Etcone40, &b_el_Etcone40); }
  if( fChain->GetBranchStatus("el_ptcone20") ) { fChain->SetBranchAddress("el_ptcone20", &el_ptcone20, &b_el_ptcone20); }
  if( fChain->GetBranchStatus("el_ptcone30") ) { fChain->SetBranchAddress("el_ptcone30", &el_ptcone30, &b_el_ptcone30); }
  if( fChain->GetBranchStatus("el_ptcone40") ) { fChain->SetBranchAddress("el_ptcone40", &el_ptcone40, &b_el_ptcone40); }
  if( fChain->GetBranchStatus("el_nucone20") ) { fChain->SetBranchAddress("el_nucone20", &el_nucone20, &b_el_nucone20); }
  if( fChain->GetBranchStatus("el_nucone30") ) { fChain->SetBranchAddress("el_nucone30", &el_nucone30, &b_el_nucone30); }
  if( fChain->GetBranchStatus("el_nucone40") ) { fChain->SetBranchAddress("el_nucone40", &el_nucone40, &b_el_nucone40); }
  if( fChain->GetBranchStatus("el_convanglematch") ) { fChain->SetBranchAddress("el_convanglematch", &el_convanglematch, &b_el_convanglematch); }
  if( fChain->GetBranchStatus("el_convtrackmatch") ) { fChain->SetBranchAddress("el_convtrackmatch", &el_convtrackmatch, &b_el_convtrackmatch); }
  if( fChain->GetBranchStatus("el_hasconv") ) { fChain->SetBranchAddress("el_hasconv", &el_hasconv, &b_el_hasconv); }
  if( fChain->GetBranchStatus("el_convvtxx") ) { fChain->SetBranchAddress("el_convvtxx", &el_convvtxx, &b_el_convvtxx); }
  if( fChain->GetBranchStatus("el_convvtxy") ) { fChain->SetBranchAddress("el_convvtxy", &el_convvtxy, &b_el_convvtxy); }
  if( fChain->GetBranchStatus("el_convvtxz") ) { fChain->SetBranchAddress("el_convvtxz", &el_convvtxz, &b_el_convvtxz); }
  if( fChain->GetBranchStatus("el_Rconv") ) { fChain->SetBranchAddress("el_Rconv", &el_Rconv, &b_el_Rconv); }
  if( fChain->GetBranchStatus("el_zconv") ) { fChain->SetBranchAddress("el_zconv", &el_zconv, &b_el_zconv); }
  if( fChain->GetBranchStatus("el_convvtxchi2") ) { fChain->SetBranchAddress("el_convvtxchi2", &el_convvtxchi2, &b_el_convvtxchi2); }
  if( fChain->GetBranchStatus("el_pt1conv") ) { fChain->SetBranchAddress("el_pt1conv", &el_pt1conv, &b_el_pt1conv); }
  if( fChain->GetBranchStatus("el_convtrk1nBLHits") ) { fChain->SetBranchAddress("el_convtrk1nBLHits", &el_convtrk1nBLHits, &b_el_convtrk1nBLHits); }
  if( fChain->GetBranchStatus("el_convtrk1nPixHits") ) { fChain->SetBranchAddress("el_convtrk1nPixHits", &el_convtrk1nPixHits, &b_el_convtrk1nPixHits); }
  if( fChain->GetBranchStatus("el_convtrk1nSCTHits") ) { fChain->SetBranchAddress("el_convtrk1nSCTHits", &el_convtrk1nSCTHits, &b_el_convtrk1nSCTHits); }
  if( fChain->GetBranchStatus("el_convtrk1nTRTHits") ) { fChain->SetBranchAddress("el_convtrk1nTRTHits", &el_convtrk1nTRTHits, &b_el_convtrk1nTRTHits); }
  if( fChain->GetBranchStatus("el_pt2conv") ) { fChain->SetBranchAddress("el_pt2conv", &el_pt2conv, &b_el_pt2conv); }
  if( fChain->GetBranchStatus("el_convtrk2nBLHits") ) { fChain->SetBranchAddress("el_convtrk2nBLHits", &el_convtrk2nBLHits, &b_el_convtrk2nBLHits); }
  if( fChain->GetBranchStatus("el_convtrk2nPixHits") ) { fChain->SetBranchAddress("el_convtrk2nPixHits", &el_convtrk2nPixHits, &b_el_convtrk2nPixHits); }
  if( fChain->GetBranchStatus("el_convtrk2nSCTHits") ) { fChain->SetBranchAddress("el_convtrk2nSCTHits", &el_convtrk2nSCTHits, &b_el_convtrk2nSCTHits); }
  if( fChain->GetBranchStatus("el_convtrk2nTRTHits") ) { fChain->SetBranchAddress("el_convtrk2nTRTHits", &el_convtrk2nTRTHits, &b_el_convtrk2nTRTHits); }
  if( fChain->GetBranchStatus("el_ptconv") ) { fChain->SetBranchAddress("el_ptconv", &el_ptconv, &b_el_ptconv); }
  if( fChain->GetBranchStatus("el_pzconv") ) { fChain->SetBranchAddress("el_pzconv", &el_pzconv, &b_el_pzconv); }
  if( fChain->GetBranchStatus("el_pos7") ) { fChain->SetBranchAddress("el_pos7", &el_pos7, &b_el_pos7); }
  if( fChain->GetBranchStatus("el_etacorrmag") ) { fChain->SetBranchAddress("el_etacorrmag", &el_etacorrmag, &b_el_etacorrmag); }
  if( fChain->GetBranchStatus("el_deltaeta1") ) { fChain->SetBranchAddress("el_deltaeta1", &el_deltaeta1, &b_el_deltaeta1); }
  if( fChain->GetBranchStatus("el_deltaeta2") ) { fChain->SetBranchAddress("el_deltaeta2", &el_deltaeta2, &b_el_deltaeta2); }
  if( fChain->GetBranchStatus("el_deltaphi2") ) { fChain->SetBranchAddress("el_deltaphi2", &el_deltaphi2, &b_el_deltaphi2); }
  if( fChain->GetBranchStatus("el_deltaphiRescaled") ) { fChain->SetBranchAddress("el_deltaphiRescaled", &el_deltaphiRescaled, &b_el_deltaphiRescaled); }
  if( fChain->GetBranchStatus("el_deltaPhiRot") ) { fChain->SetBranchAddress("el_deltaPhiRot", &el_deltaPhiRot, &b_el_deltaPhiRot); }
  if( fChain->GetBranchStatus("el_expectHitInBLayer") ) { fChain->SetBranchAddress("el_expectHitInBLayer", &el_expectHitInBLayer, &b_el_expectHitInBLayer); }
  if( fChain->GetBranchStatus("el_reta") ) { fChain->SetBranchAddress("el_reta", &el_reta, &b_el_reta); }
  if( fChain->GetBranchStatus("el_rphi") ) { fChain->SetBranchAddress("el_rphi", &el_rphi, &b_el_rphi); }
  if( fChain->GetBranchStatus("el_EtringnoisedR03sig2") ) { fChain->SetBranchAddress("el_EtringnoisedR03sig2", &el_EtringnoisedR03sig2, &b_el_EtringnoisedR03sig2); }
  if( fChain->GetBranchStatus("el_EtringnoisedR03sig3") ) { fChain->SetBranchAddress("el_EtringnoisedR03sig3", &el_EtringnoisedR03sig3, &b_el_EtringnoisedR03sig3); }
  if( fChain->GetBranchStatus("el_EtringnoisedR03sig4") ) { fChain->SetBranchAddress("el_EtringnoisedR03sig4", &el_EtringnoisedR03sig4, &b_el_EtringnoisedR03sig4); }
  if( fChain->GetBranchStatus("el_isolationlikelihoodjets") ) { fChain->SetBranchAddress("el_isolationlikelihoodjets", &el_isolationlikelihoodjets, &b_el_isolationlikelihoodjets); }
  if( fChain->GetBranchStatus("el_isolationlikelihoodhqelectrons") ) { fChain->SetBranchAddress("el_isolationlikelihoodhqelectrons", &el_isolationlikelihoodhqelectrons, &b_el_isolationlikelihoodhqelectrons); }
  if( fChain->GetBranchStatus("el_electronweight") ) { fChain->SetBranchAddress("el_electronweight", &el_electronweight, &b_el_electronweight); }
  if( fChain->GetBranchStatus("el_electronbgweight") ) { fChain->SetBranchAddress("el_electronbgweight", &el_electronbgweight, &b_el_electronbgweight); }
  if( fChain->GetBranchStatus("el_softeweight") ) { fChain->SetBranchAddress("el_softeweight", &el_softeweight, &b_el_softeweight); }
  if( fChain->GetBranchStatus("el_softebgweight") ) { fChain->SetBranchAddress("el_softebgweight", &el_softebgweight, &b_el_softebgweight); }
  if( fChain->GetBranchStatus("el_neuralnet") ) { fChain->SetBranchAddress("el_neuralnet", &el_neuralnet, &b_el_neuralnet); }
  if( fChain->GetBranchStatus("el_Hmatrix") ) { fChain->SetBranchAddress("el_Hmatrix", &el_Hmatrix, &b_el_Hmatrix); }
  if( fChain->GetBranchStatus("el_Hmatrix5") ) { fChain->SetBranchAddress("el_Hmatrix5", &el_Hmatrix5, &b_el_Hmatrix5); }
  if( fChain->GetBranchStatus("el_adaboost") ) { fChain->SetBranchAddress("el_adaboost", &el_adaboost, &b_el_adaboost); }
  if( fChain->GetBranchStatus("el_softeneuralnet") ) { fChain->SetBranchAddress("el_softeneuralnet", &el_softeneuralnet, &b_el_softeneuralnet); }
  if( fChain->GetBranchStatus("el_zvertex") ) { fChain->SetBranchAddress("el_zvertex", &el_zvertex, &b_el_zvertex); }
  if( fChain->GetBranchStatus("el_errz") ) { fChain->SetBranchAddress("el_errz", &el_errz, &b_el_errz); }
  if( fChain->GetBranchStatus("el_etap") ) { fChain->SetBranchAddress("el_etap", &el_etap, &b_el_etap); }
  if( fChain->GetBranchStatus("el_depth") ) { fChain->SetBranchAddress("el_depth", &el_depth, &b_el_depth); }
  if( fChain->GetBranchStatus("el_refittedTrack_n") ) { fChain->SetBranchAddress("el_refittedTrack_n", &el_refittedTrack_n, &b_el_refittedTrack_n); }
  if( fChain->GetBranchStatus("el_Es0") ) { fChain->SetBranchAddress("el_Es0", &el_Es0, &b_el_Es0); }
  if( fChain->GetBranchStatus("el_etas0") ) { fChain->SetBranchAddress("el_etas0", &el_etas0, &b_el_etas0); }
  if( fChain->GetBranchStatus("el_phis0") ) { fChain->SetBranchAddress("el_phis0", &el_phis0, &b_el_phis0); }
  if( fChain->GetBranchStatus("el_Es1") ) { fChain->SetBranchAddress("el_Es1", &el_Es1, &b_el_Es1); }
  if( fChain->GetBranchStatus("el_etas1") ) { fChain->SetBranchAddress("el_etas1", &el_etas1, &b_el_etas1); }
  if( fChain->GetBranchStatus("el_phis1") ) { fChain->SetBranchAddress("el_phis1", &el_phis1, &b_el_phis1); }
  if( fChain->GetBranchStatus("el_Es2") ) { fChain->SetBranchAddress("el_Es2", &el_Es2, &b_el_Es2); }
  if( fChain->GetBranchStatus("el_etas2") ) { fChain->SetBranchAddress("el_etas2", &el_etas2, &b_el_etas2); }
  if( fChain->GetBranchStatus("el_phis2") ) { fChain->SetBranchAddress("el_phis2", &el_phis2, &b_el_phis2); }
  if( fChain->GetBranchStatus("el_Es3") ) { fChain->SetBranchAddress("el_Es3", &el_Es3, &b_el_Es3); }
  if( fChain->GetBranchStatus("el_etas3") ) { fChain->SetBranchAddress("el_etas3", &el_etas3, &b_el_etas3); }
  if( fChain->GetBranchStatus("el_phis3") ) { fChain->SetBranchAddress("el_phis3", &el_phis3, &b_el_phis3); }
  if( fChain->GetBranchStatus("el_E_PreSamplerB") ) { fChain->SetBranchAddress("el_E_PreSamplerB", &el_E_PreSamplerB, &b_el_E_PreSamplerB); }
  if( fChain->GetBranchStatus("el_E_EMB1") ) { fChain->SetBranchAddress("el_E_EMB1", &el_E_EMB1, &b_el_E_EMB1); }
  if( fChain->GetBranchStatus("el_E_EMB2") ) { fChain->SetBranchAddress("el_E_EMB2", &el_E_EMB2, &b_el_E_EMB2); }
  if( fChain->GetBranchStatus("el_E_EMB3") ) { fChain->SetBranchAddress("el_E_EMB3", &el_E_EMB3, &b_el_E_EMB3); }
  if( fChain->GetBranchStatus("el_E_PreSamplerE") ) { fChain->SetBranchAddress("el_E_PreSamplerE", &el_E_PreSamplerE, &b_el_E_PreSamplerE); }
  if( fChain->GetBranchStatus("el_E_EME1") ) { fChain->SetBranchAddress("el_E_EME1", &el_E_EME1, &b_el_E_EME1); }
  if( fChain->GetBranchStatus("el_E_EME2") ) { fChain->SetBranchAddress("el_E_EME2", &el_E_EME2, &b_el_E_EME2); }
  if( fChain->GetBranchStatus("el_E_EME3") ) { fChain->SetBranchAddress("el_E_EME3", &el_E_EME3, &b_el_E_EME3); }
  if( fChain->GetBranchStatus("el_E_HEC0") ) { fChain->SetBranchAddress("el_E_HEC0", &el_E_HEC0, &b_el_E_HEC0); }
  if( fChain->GetBranchStatus("el_E_HEC1") ) { fChain->SetBranchAddress("el_E_HEC1", &el_E_HEC1, &b_el_E_HEC1); }
  if( fChain->GetBranchStatus("el_E_HEC2") ) { fChain->SetBranchAddress("el_E_HEC2", &el_E_HEC2, &b_el_E_HEC2); }
  if( fChain->GetBranchStatus("el_E_HEC3") ) { fChain->SetBranchAddress("el_E_HEC3", &el_E_HEC3, &b_el_E_HEC3); }
  if( fChain->GetBranchStatus("el_E_TileBar0") ) { fChain->SetBranchAddress("el_E_TileBar0", &el_E_TileBar0, &b_el_E_TileBar0); }
  if( fChain->GetBranchStatus("el_E_TileBar1") ) { fChain->SetBranchAddress("el_E_TileBar1", &el_E_TileBar1, &b_el_E_TileBar1); }
  if( fChain->GetBranchStatus("el_E_TileBar2") ) { fChain->SetBranchAddress("el_E_TileBar2", &el_E_TileBar2, &b_el_E_TileBar2); }
  if( fChain->GetBranchStatus("el_E_TileGap1") ) { fChain->SetBranchAddress("el_E_TileGap1", &el_E_TileGap1, &b_el_E_TileGap1); }
  if( fChain->GetBranchStatus("el_E_TileGap2") ) { fChain->SetBranchAddress("el_E_TileGap2", &el_E_TileGap2, &b_el_E_TileGap2); }
  if( fChain->GetBranchStatus("el_E_TileGap3") ) { fChain->SetBranchAddress("el_E_TileGap3", &el_E_TileGap3, &b_el_E_TileGap3); }
  if( fChain->GetBranchStatus("el_E_TileExt0") ) { fChain->SetBranchAddress("el_E_TileExt0", &el_E_TileExt0, &b_el_E_TileExt0); }
  if( fChain->GetBranchStatus("el_E_TileExt1") ) { fChain->SetBranchAddress("el_E_TileExt1", &el_E_TileExt1, &b_el_E_TileExt1); }
  if( fChain->GetBranchStatus("el_E_TileExt2") ) { fChain->SetBranchAddress("el_E_TileExt2", &el_E_TileExt2, &b_el_E_TileExt2); }
  if( fChain->GetBranchStatus("el_E_FCAL0") ) { fChain->SetBranchAddress("el_E_FCAL0", &el_E_FCAL0, &b_el_E_FCAL0); }
  if( fChain->GetBranchStatus("el_E_FCAL1") ) { fChain->SetBranchAddress("el_E_FCAL1", &el_E_FCAL1, &b_el_E_FCAL1); }
  if( fChain->GetBranchStatus("el_E_FCAL2") ) { fChain->SetBranchAddress("el_E_FCAL2", &el_E_FCAL2, &b_el_E_FCAL2); }
  if( fChain->GetBranchStatus("el_cl_E") ) { fChain->SetBranchAddress("el_cl_E", &el_cl_E, &b_el_cl_E); }
  if( fChain->GetBranchStatus("el_cl_pt") ) { fChain->SetBranchAddress("el_cl_pt", &el_cl_pt, &b_el_cl_pt); }
  if( fChain->GetBranchStatus("el_cl_eta") ) { fChain->SetBranchAddress("el_cl_eta", &el_cl_eta, &b_el_cl_eta); }
  if( fChain->GetBranchStatus("el_cl_phi") ) { fChain->SetBranchAddress("el_cl_phi", &el_cl_phi, &b_el_cl_phi); }
  if( fChain->GetBranchStatus("el_firstEdens") ) { fChain->SetBranchAddress("el_firstEdens", &el_firstEdens, &b_el_firstEdens); }
  if( fChain->GetBranchStatus("el_cellmaxfrac") ) { fChain->SetBranchAddress("el_cellmaxfrac", &el_cellmaxfrac, &b_el_cellmaxfrac); }
  if( fChain->GetBranchStatus("el_longitudinal") ) { fChain->SetBranchAddress("el_longitudinal", &el_longitudinal, &b_el_longitudinal); }
  if( fChain->GetBranchStatus("el_secondlambda") ) { fChain->SetBranchAddress("el_secondlambda", &el_secondlambda, &b_el_secondlambda); }
  if( fChain->GetBranchStatus("el_lateral") ) { fChain->SetBranchAddress("el_lateral", &el_lateral, &b_el_lateral); }
  if( fChain->GetBranchStatus("el_secondR") ) { fChain->SetBranchAddress("el_secondR", &el_secondR, &b_el_secondR); }
  if( fChain->GetBranchStatus("el_centerlambda") ) { fChain->SetBranchAddress("el_centerlambda", &el_centerlambda, &b_el_centerlambda); }
  if( fChain->GetBranchStatus("el_rawcl_Es0") ) { fChain->SetBranchAddress("el_rawcl_Es0", &el_rawcl_Es0, &b_el_rawcl_Es0); }
  if( fChain->GetBranchStatus("el_rawcl_etas0") ) { fChain->SetBranchAddress("el_rawcl_etas0", &el_rawcl_etas0, &b_el_rawcl_etas0); }
  if( fChain->GetBranchStatus("el_rawcl_phis0") ) { fChain->SetBranchAddress("el_rawcl_phis0", &el_rawcl_phis0, &b_el_rawcl_phis0); }
  if( fChain->GetBranchStatus("el_rawcl_Es1") ) { fChain->SetBranchAddress("el_rawcl_Es1", &el_rawcl_Es1, &b_el_rawcl_Es1); }
  if( fChain->GetBranchStatus("el_rawcl_etas1") ) { fChain->SetBranchAddress("el_rawcl_etas1", &el_rawcl_etas1, &b_el_rawcl_etas1); }
  if( fChain->GetBranchStatus("el_rawcl_phis1") ) { fChain->SetBranchAddress("el_rawcl_phis1", &el_rawcl_phis1, &b_el_rawcl_phis1); }
  if( fChain->GetBranchStatus("el_rawcl_Es2") ) { fChain->SetBranchAddress("el_rawcl_Es2", &el_rawcl_Es2, &b_el_rawcl_Es2); }
  if( fChain->GetBranchStatus("el_rawcl_etas2") ) { fChain->SetBranchAddress("el_rawcl_etas2", &el_rawcl_etas2, &b_el_rawcl_etas2); }
  if( fChain->GetBranchStatus("el_rawcl_phis2") ) { fChain->SetBranchAddress("el_rawcl_phis2", &el_rawcl_phis2, &b_el_rawcl_phis2); }
  if( fChain->GetBranchStatus("el_rawcl_Es3") ) { fChain->SetBranchAddress("el_rawcl_Es3", &el_rawcl_Es3, &b_el_rawcl_Es3); }
  if( fChain->GetBranchStatus("el_rawcl_etas3") ) { fChain->SetBranchAddress("el_rawcl_etas3", &el_rawcl_etas3, &b_el_rawcl_etas3); }
  if( fChain->GetBranchStatus("el_rawcl_phis3") ) { fChain->SetBranchAddress("el_rawcl_phis3", &el_rawcl_phis3, &b_el_rawcl_phis3); }
  if( fChain->GetBranchStatus("el_rawcl_E") ) { fChain->SetBranchAddress("el_rawcl_E", &el_rawcl_E, &b_el_rawcl_E); }
  if( fChain->GetBranchStatus("el_rawcl_pt") ) { fChain->SetBranchAddress("el_rawcl_pt", &el_rawcl_pt, &b_el_rawcl_pt); }
  if( fChain->GetBranchStatus("el_rawcl_eta") ) { fChain->SetBranchAddress("el_rawcl_eta", &el_rawcl_eta, &b_el_rawcl_eta); }
  if( fChain->GetBranchStatus("el_rawcl_phi") ) { fChain->SetBranchAddress("el_rawcl_phi", &el_rawcl_phi, &b_el_rawcl_phi); }
  if( fChain->GetBranchStatus("el_refittedtrackcovd0") ) { fChain->SetBranchAddress("el_refittedtrackcovd0", &el_refittedtrackcovd0, &b_el_refittedtrackcovd0); }
  if( fChain->GetBranchStatus("el_refittedtrackcovz0") ) { fChain->SetBranchAddress("el_refittedtrackcovz0", &el_refittedtrackcovz0, &b_el_refittedtrackcovz0); }
  if( fChain->GetBranchStatus("el_refittedtrackcovphi") ) { fChain->SetBranchAddress("el_refittedtrackcovphi", &el_refittedtrackcovphi, &b_el_refittedtrackcovphi); }
  if( fChain->GetBranchStatus("el_refittedtrackcovtheta") ) { fChain->SetBranchAddress("el_refittedtrackcovtheta", &el_refittedtrackcovtheta, &b_el_refittedtrackcovtheta); }
  if( fChain->GetBranchStatus("el_refittedtrackcovqoverp") ) { fChain->SetBranchAddress("el_refittedtrackcovqoverp", &el_refittedtrackcovqoverp, &b_el_refittedtrackcovqoverp); }
  if( fChain->GetBranchStatus("el_refittedtrackcovd0z0") ) { fChain->SetBranchAddress("el_refittedtrackcovd0z0", &el_refittedtrackcovd0z0, &b_el_refittedtrackcovd0z0); }
  if( fChain->GetBranchStatus("el_refittedtrackcovz0phi") ) { fChain->SetBranchAddress("el_refittedtrackcovz0phi", &el_refittedtrackcovz0phi, &b_el_refittedtrackcovz0phi); }
  if( fChain->GetBranchStatus("el_refittedtrackcovz0theta") ) { fChain->SetBranchAddress("el_refittedtrackcovz0theta", &el_refittedtrackcovz0theta, &b_el_refittedtrackcovz0theta); }
  if( fChain->GetBranchStatus("el_refittedtrackcovz0qoverp") ) { fChain->SetBranchAddress("el_refittedtrackcovz0qoverp", &el_refittedtrackcovz0qoverp, &b_el_refittedtrackcovz0qoverp); }
  if( fChain->GetBranchStatus("el_refittedtrackcovd0phi") ) { fChain->SetBranchAddress("el_refittedtrackcovd0phi", &el_refittedtrackcovd0phi, &b_el_refittedtrackcovd0phi); }
  if( fChain->GetBranchStatus("el_refittedtrackcovd0theta") ) { fChain->SetBranchAddress("el_refittedtrackcovd0theta", &el_refittedtrackcovd0theta, &b_el_refittedtrackcovd0theta); }
  if( fChain->GetBranchStatus("el_refittedtrackcovd0qoverp") ) { fChain->SetBranchAddress("el_refittedtrackcovd0qoverp", &el_refittedtrackcovd0qoverp, &b_el_refittedtrackcovd0qoverp); }
  if( fChain->GetBranchStatus("el_refittedtrackcovphitheta") ) { fChain->SetBranchAddress("el_refittedtrackcovphitheta", &el_refittedtrackcovphitheta, &b_el_refittedtrackcovphitheta); }
  if( fChain->GetBranchStatus("el_refittedtrackcovphiqoverp") ) { fChain->SetBranchAddress("el_refittedtrackcovphiqoverp", &el_refittedtrackcovphiqoverp, &b_el_refittedtrackcovphiqoverp); }
  if( fChain->GetBranchStatus("el_refittedtrackcovthetaqoverp") ) { fChain->SetBranchAddress("el_refittedtrackcovthetaqoverp", &el_refittedtrackcovthetaqoverp, &b_el_refittedtrackcovthetaqoverp); }
  if( fChain->GetBranchStatus("el_trackd0") ) { fChain->SetBranchAddress("el_trackd0", &el_trackd0, &b_el_trackd0); }
  if( fChain->GetBranchStatus("el_trackz0") ) { fChain->SetBranchAddress("el_trackz0", &el_trackz0, &b_el_trackz0); }
  if( fChain->GetBranchStatus("el_trackphi") ) { fChain->SetBranchAddress("el_trackphi", &el_trackphi, &b_el_trackphi); }
  if( fChain->GetBranchStatus("el_tracktheta") ) { fChain->SetBranchAddress("el_tracktheta", &el_tracktheta, &b_el_tracktheta); }
  if( fChain->GetBranchStatus("el_trackqoverp") ) { fChain->SetBranchAddress("el_trackqoverp", &el_trackqoverp, &b_el_trackqoverp); }
  if( fChain->GetBranchStatus("el_trackpt") ) { fChain->SetBranchAddress("el_trackpt", &el_trackpt, &b_el_trackpt); }
  if( fChain->GetBranchStatus("el_tracketa") ) { fChain->SetBranchAddress("el_tracketa", &el_tracketa, &b_el_tracketa); }
  if( fChain->GetBranchStatus("el_trackcov_d0") ) { fChain->SetBranchAddress("el_trackcov_d0", &el_trackcov_d0, &b_el_trackcov_d0); }
  if( fChain->GetBranchStatus("el_trackcov_z0") ) { fChain->SetBranchAddress("el_trackcov_z0", &el_trackcov_z0, &b_el_trackcov_z0); }
  if( fChain->GetBranchStatus("el_trackcov_phi") ) { fChain->SetBranchAddress("el_trackcov_phi", &el_trackcov_phi, &b_el_trackcov_phi); }
  if( fChain->GetBranchStatus("el_trackcov_theta") ) { fChain->SetBranchAddress("el_trackcov_theta", &el_trackcov_theta, &b_el_trackcov_theta); }
  if( fChain->GetBranchStatus("el_trackcov_qoverp") ) { fChain->SetBranchAddress("el_trackcov_qoverp", &el_trackcov_qoverp, &b_el_trackcov_qoverp); }
  if( fChain->GetBranchStatus("el_trackcov_d0_z0") ) { fChain->SetBranchAddress("el_trackcov_d0_z0", &el_trackcov_d0_z0, &b_el_trackcov_d0_z0); }
  if( fChain->GetBranchStatus("el_trackcov_d0_phi") ) { fChain->SetBranchAddress("el_trackcov_d0_phi", &el_trackcov_d0_phi, &b_el_trackcov_d0_phi); }
  if( fChain->GetBranchStatus("el_trackcov_d0_theta") ) { fChain->SetBranchAddress("el_trackcov_d0_theta", &el_trackcov_d0_theta, &b_el_trackcov_d0_theta); }
  if( fChain->GetBranchStatus("el_trackcov_d0_qoverp") ) { fChain->SetBranchAddress("el_trackcov_d0_qoverp", &el_trackcov_d0_qoverp, &b_el_trackcov_d0_qoverp); }
  if( fChain->GetBranchStatus("el_trackcov_z0_phi") ) { fChain->SetBranchAddress("el_trackcov_z0_phi", &el_trackcov_z0_phi, &b_el_trackcov_z0_phi); }
  if( fChain->GetBranchStatus("el_trackcov_z0_theta") ) { fChain->SetBranchAddress("el_trackcov_z0_theta", &el_trackcov_z0_theta, &b_el_trackcov_z0_theta); }
  if( fChain->GetBranchStatus("el_trackcov_z0_qoverp") ) { fChain->SetBranchAddress("el_trackcov_z0_qoverp", &el_trackcov_z0_qoverp, &b_el_trackcov_z0_qoverp); }
  if( fChain->GetBranchStatus("el_trackcov_phi_theta") ) { fChain->SetBranchAddress("el_trackcov_phi_theta", &el_trackcov_phi_theta, &b_el_trackcov_phi_theta); }
  if( fChain->GetBranchStatus("el_trackcov_phi_qoverp") ) { fChain->SetBranchAddress("el_trackcov_phi_qoverp", &el_trackcov_phi_qoverp, &b_el_trackcov_phi_qoverp); }
  if( fChain->GetBranchStatus("el_trackcov_theta_qoverp") ) { fChain->SetBranchAddress("el_trackcov_theta_qoverp", &el_trackcov_theta_qoverp, &b_el_trackcov_theta_qoverp); }
  if( fChain->GetBranchStatus("el_trackfitchi2") ) { fChain->SetBranchAddress("el_trackfitchi2", &el_trackfitchi2, &b_el_trackfitchi2); }
  if( fChain->GetBranchStatus("el_trackfitndof") ) { fChain->SetBranchAddress("el_trackfitndof", &el_trackfitndof, &b_el_trackfitndof); }
  if( fChain->GetBranchStatus("el_nBLHits") ) { fChain->SetBranchAddress("el_nBLHits", &el_nBLHits, &b_el_nBLHits); }
  if( fChain->GetBranchStatus("el_nPixHits") ) { fChain->SetBranchAddress("el_nPixHits", &el_nPixHits, &b_el_nPixHits); }
  if( fChain->GetBranchStatus("el_nSCTHits") ) { fChain->SetBranchAddress("el_nSCTHits", &el_nSCTHits, &b_el_nSCTHits); }
  if( fChain->GetBranchStatus("el_nTRTHits") ) { fChain->SetBranchAddress("el_nTRTHits", &el_nTRTHits, &b_el_nTRTHits); }
  if( fChain->GetBranchStatus("el_nPixHoles") ) { fChain->SetBranchAddress("el_nPixHoles", &el_nPixHoles, &b_el_nPixHoles); }
  if( fChain->GetBranchStatus("el_nSCTHoles") ) { fChain->SetBranchAddress("el_nSCTHoles", &el_nSCTHoles, &b_el_nSCTHoles); }
  if( fChain->GetBranchStatus("el_nBLSharedHits") ) { fChain->SetBranchAddress("el_nBLSharedHits", &el_nBLSharedHits, &b_el_nBLSharedHits); }
  if( fChain->GetBranchStatus("el_nPixSharedHits") ) { fChain->SetBranchAddress("el_nPixSharedHits", &el_nPixSharedHits, &b_el_nPixSharedHits); }
  if( fChain->GetBranchStatus("el_nSCTSharedHits") ) { fChain->SetBranchAddress("el_nSCTSharedHits", &el_nSCTSharedHits, &b_el_nSCTSharedHits); }
  if( fChain->GetBranchStatus("el_nTRTHighTHits") ) { fChain->SetBranchAddress("el_nTRTHighTHits", &el_nTRTHighTHits, &b_el_nTRTHighTHits); }
  if( fChain->GetBranchStatus("el_nTRTOutliers") ) { fChain->SetBranchAddress("el_nTRTOutliers", &el_nTRTOutliers, &b_el_nTRTOutliers); }
  if( fChain->GetBranchStatus("el_nTRTHighTOutliers") ) { fChain->SetBranchAddress("el_nTRTHighTOutliers", &el_nTRTHighTOutliers, &b_el_nTRTHighTOutliers); }
  if( fChain->GetBranchStatus("el_nSiHits") ) { fChain->SetBranchAddress("el_nSiHits", &el_nSiHits, &b_el_nSiHits); }
  if( fChain->GetBranchStatus("el_TRTHighTHitsRatio") ) { fChain->SetBranchAddress("el_TRTHighTHitsRatio", &el_TRTHighTHitsRatio, &b_el_TRTHighTHitsRatio); }
  if( fChain->GetBranchStatus("el_pixeldEdx") ) { fChain->SetBranchAddress("el_pixeldEdx", &el_pixeldEdx, &b_el_pixeldEdx); }
  if( fChain->GetBranchStatus("el_eProbabilityComb") ) { fChain->SetBranchAddress("el_eProbabilityComb", &el_eProbabilityComb, &b_el_eProbabilityComb); }
  if( fChain->GetBranchStatus("el_eProbabilityHT") ) { fChain->SetBranchAddress("el_eProbabilityHT", &el_eProbabilityHT, &b_el_eProbabilityHT); }
  if( fChain->GetBranchStatus("el_eProbabilityToT") ) { fChain->SetBranchAddress("el_eProbabilityToT", &el_eProbabilityToT, &b_el_eProbabilityToT); }
  if( fChain->GetBranchStatus("el_eProbabilityBrem") ) { fChain->SetBranchAddress("el_eProbabilityBrem", &el_eProbabilityBrem, &b_el_eProbabilityBrem); }
  if( fChain->GetBranchStatus("el_vertx") ) { fChain->SetBranchAddress("el_vertx", &el_vertx, &b_el_vertx); }
  if( fChain->GetBranchStatus("el_verty") ) { fChain->SetBranchAddress("el_verty", &el_verty, &b_el_verty); }
  if( fChain->GetBranchStatus("el_vertz") ) { fChain->SetBranchAddress("el_vertz", &el_vertz, &b_el_vertz); }
  if( fChain->GetBranchStatus("el_hastrack") ) { fChain->SetBranchAddress("el_hastrack", &el_hastrack, &b_el_hastrack); }
  if( fChain->GetBranchStatus("el_deltaEmax2") ) { fChain->SetBranchAddress("el_deltaEmax2", &el_deltaEmax2, &b_el_deltaEmax2); }
  if( fChain->GetBranchStatus("el_calibHitsShowerDepth") ) { fChain->SetBranchAddress("el_calibHitsShowerDepth", &el_calibHitsShowerDepth, &b_el_calibHitsShowerDepth); }
  if( fChain->GetBranchStatus("el_trackd0beam") ) { fChain->SetBranchAddress("el_trackd0beam", &el_trackd0beam, &b_el_trackd0beam); }
  if( fChain->GetBranchStatus("el_tracksigd0beam") ) { fChain->SetBranchAddress("el_tracksigd0beam", &el_tracksigd0beam, &b_el_tracksigd0beam); }
  if( fChain->GetBranchStatus("el_trackd0pv") ) { fChain->SetBranchAddress("el_trackd0pv", &el_trackd0pv, &b_el_trackd0pv); }
  if( fChain->GetBranchStatus("el_tracksigd0pv") ) { fChain->SetBranchAddress("el_tracksigd0pv", &el_tracksigd0pv, &b_el_tracksigd0pv); }
  if( fChain->GetBranchStatus("el_trackz0pv") ) { fChain->SetBranchAddress("el_trackz0pv", &el_trackz0pv, &b_el_trackz0pv); }
  if( fChain->GetBranchStatus("el_tracksigz0pv") ) { fChain->SetBranchAddress("el_tracksigz0pv", &el_tracksigz0pv, &b_el_tracksigz0pv); }
  if( fChain->GetBranchStatus("el_trackd0pvunbiased") ) { fChain->SetBranchAddress("el_trackd0pvunbiased", &el_trackd0pvunbiased, &b_el_trackd0pvunbiased); }
  if( fChain->GetBranchStatus("el_tracksigd0pvunbiased") ) { fChain->SetBranchAddress("el_tracksigd0pvunbiased", &el_tracksigd0pvunbiased, &b_el_tracksigd0pvunbiased); }
  if( fChain->GetBranchStatus("el_trackz0pvunbiased") ) { fChain->SetBranchAddress("el_trackz0pvunbiased", &el_trackz0pvunbiased, &b_el_trackz0pvunbiased); }
  if( fChain->GetBranchStatus("el_tracksigz0pvunbiased") ) { fChain->SetBranchAddress("el_tracksigz0pvunbiased", &el_tracksigz0pvunbiased, &b_el_tracksigz0pvunbiased); }
  if( fChain->GetBranchStatus("el_isIso") ) { fChain->SetBranchAddress("el_isIso", &el_isIso, &b_el_isIso); }
  if( fChain->GetBranchStatus("el_mvaptcone20") ) { fChain->SetBranchAddress("el_mvaptcone20", &el_mvaptcone20, &b_el_mvaptcone20); }
  if( fChain->GetBranchStatus("el_mvaptcone30") ) { fChain->SetBranchAddress("el_mvaptcone30", &el_mvaptcone30, &b_el_mvaptcone30); }
  if( fChain->GetBranchStatus("el_mvaptcone40") ) { fChain->SetBranchAddress("el_mvaptcone40", &el_mvaptcone40, &b_el_mvaptcone40); }
  if( fChain->GetBranchStatus("el_jet_dr") ) { fChain->SetBranchAddress("el_jet_dr", &el_jet_dr, &b_el_jet_dr); }
  if( fChain->GetBranchStatus("el_jet_E") ) { fChain->SetBranchAddress("el_jet_E", &el_jet_E, &b_el_jet_E); }
  if( fChain->GetBranchStatus("el_jet_pt") ) { fChain->SetBranchAddress("el_jet_pt", &el_jet_pt, &b_el_jet_pt); }
  if( fChain->GetBranchStatus("el_jet_m") ) { fChain->SetBranchAddress("el_jet_m", &el_jet_m, &b_el_jet_m); }
  if( fChain->GetBranchStatus("el_jet_eta") ) { fChain->SetBranchAddress("el_jet_eta", &el_jet_eta, &b_el_jet_eta); }
  if( fChain->GetBranchStatus("el_jet_phi") ) { fChain->SetBranchAddress("el_jet_phi", &el_jet_phi, &b_el_jet_phi); }
  if( fChain->GetBranchStatus("el_jet_truth_dr") ) { fChain->SetBranchAddress("el_jet_truth_dr", &el_jet_truth_dr, &b_el_jet_truth_dr); }
  if( fChain->GetBranchStatus("el_jet_truth_E") ) { fChain->SetBranchAddress("el_jet_truth_E", &el_jet_truth_E, &b_el_jet_truth_E); }
  if( fChain->GetBranchStatus("el_jet_truth_pt") ) { fChain->SetBranchAddress("el_jet_truth_pt", &el_jet_truth_pt, &b_el_jet_truth_pt); }
  if( fChain->GetBranchStatus("el_jet_truth_m") ) { fChain->SetBranchAddress("el_jet_truth_m", &el_jet_truth_m, &b_el_jet_truth_m); }
  if( fChain->GetBranchStatus("el_jet_truth_eta") ) { fChain->SetBranchAddress("el_jet_truth_eta", &el_jet_truth_eta, &b_el_jet_truth_eta); }
  if( fChain->GetBranchStatus("el_jet_truth_phi") ) { fChain->SetBranchAddress("el_jet_truth_phi", &el_jet_truth_phi, &b_el_jet_truth_phi); }
  if( fChain->GetBranchStatus("el_jet_truth_matched") ) { fChain->SetBranchAddress("el_jet_truth_matched", &el_jet_truth_matched, &b_el_jet_truth_matched); }
  if( fChain->GetBranchStatus("el_jet_matched") ) { fChain->SetBranchAddress("el_jet_matched", &el_jet_matched, &b_el_jet_matched); }
  if( fChain->GetBranchStatus("el_Etcone40_pt_corrected") ) { fChain->SetBranchAddress("el_Etcone40_pt_corrected", &el_Etcone40_pt_corrected, &b_el_Etcone40_pt_corrected); }
  if( fChain->GetBranchStatus("el_Etcone40_ED_corrected") ) { fChain->SetBranchAddress("el_Etcone40_ED_corrected", &el_Etcone40_ED_corrected, &b_el_Etcone40_ED_corrected); }
  if( fChain->GetBranchStatus("el_Etcone40_corrected") ) { fChain->SetBranchAddress("el_Etcone40_corrected", &el_Etcone40_corrected, &b_el_Etcone40_corrected); }
  if( fChain->GetBranchStatus("el_EF_dr") ) { fChain->SetBranchAddress("el_EF_dr", &el_EF_dr, &b_el_EF_dr); }
  if( fChain->GetBranchStatus("el_EF_index") ) { fChain->SetBranchAddress("el_EF_index", &el_EF_index, &b_el_EF_index); }
  if( fChain->GetBranchStatus("el_L2_dr") ) { fChain->SetBranchAddress("el_L2_dr", &el_L2_dr, &b_el_L2_dr); }
  if( fChain->GetBranchStatus("el_L2_index") ) { fChain->SetBranchAddress("el_L2_index", &el_L2_index, &b_el_L2_index); }
  if( fChain->GetBranchStatus("el_L1_dr") ) { fChain->SetBranchAddress("el_L1_dr", &el_L1_dr, &b_el_L1_dr); }
  if( fChain->GetBranchStatus("el_L1_index") ) { fChain->SetBranchAddress("el_L1_index", &el_L1_index, &b_el_L1_index); }
  if( fChain->GetBranchStatus("EF_2e10_loose") ) { fChain->SetBranchAddress("EF_2e10_loose", &EF_2e10_loose, &b_EF_2e10_loose); }
  if( fChain->GetBranchStatus("EF_2e10_medium") ) { fChain->SetBranchAddress("EF_2e10_medium", &EF_2e10_medium, &b_EF_2e10_medium); }
  if( fChain->GetBranchStatus("EF_2e15_loose") ) { fChain->SetBranchAddress("EF_2e15_loose", &EF_2e15_loose, &b_EF_2e15_loose); }
  if( fChain->GetBranchStatus("EF_2e3_loose") ) { fChain->SetBranchAddress("EF_2e3_loose", &EF_2e3_loose, &b_EF_2e3_loose); }
  if( fChain->GetBranchStatus("EF_2e3_loose_SiTrk") ) { fChain->SetBranchAddress("EF_2e3_loose_SiTrk", &EF_2e3_loose_SiTrk, &b_EF_2e3_loose_SiTrk); }
  if( fChain->GetBranchStatus("EF_2e3_loose_TRT") ) { fChain->SetBranchAddress("EF_2e3_loose_TRT", &EF_2e3_loose_TRT, &b_EF_2e3_loose_TRT); }
  if( fChain->GetBranchStatus("EF_2e3_medium") ) { fChain->SetBranchAddress("EF_2e3_medium", &EF_2e3_medium, &b_EF_2e3_medium); }
  if( fChain->GetBranchStatus("EF_2e3_medium_SiTrk") ) { fChain->SetBranchAddress("EF_2e3_medium_SiTrk", &EF_2e3_medium_SiTrk, &b_EF_2e3_medium_SiTrk); }
  if( fChain->GetBranchStatus("EF_2e3_medium_TRT") ) { fChain->SetBranchAddress("EF_2e3_medium_TRT", &EF_2e3_medium_TRT, &b_EF_2e3_medium_TRT); }
  if( fChain->GetBranchStatus("EF_2e3_tight") ) { fChain->SetBranchAddress("EF_2e3_tight", &EF_2e3_tight, &b_EF_2e3_tight); }
  if( fChain->GetBranchStatus("EF_2e5_medium") ) { fChain->SetBranchAddress("EF_2e5_medium", &EF_2e5_medium, &b_EF_2e5_medium); }
  if( fChain->GetBranchStatus("EF_2e5_medium_SiTrk") ) { fChain->SetBranchAddress("EF_2e5_medium_SiTrk", &EF_2e5_medium_SiTrk, &b_EF_2e5_medium_SiTrk); }
  if( fChain->GetBranchStatus("EF_2e5_medium_TRT") ) { fChain->SetBranchAddress("EF_2e5_medium_TRT", &EF_2e5_medium_TRT, &b_EF_2e5_medium_TRT); }
  if( fChain->GetBranchStatus("EF_2e5_tight") ) { fChain->SetBranchAddress("EF_2e5_tight", &EF_2e5_tight, &b_EF_2e5_tight); }
  if( fChain->GetBranchStatus("EF_2g10_loose") ) { fChain->SetBranchAddress("EF_2g10_loose", &EF_2g10_loose, &b_EF_2g10_loose); }
  if( fChain->GetBranchStatus("EF_2g15_loose") ) { fChain->SetBranchAddress("EF_2g15_loose", &EF_2g15_loose, &b_EF_2g15_loose); }
  if( fChain->GetBranchStatus("EF_2g20_loose") ) { fChain->SetBranchAddress("EF_2g20_loose", &EF_2g20_loose, &b_EF_2g20_loose); }
  if( fChain->GetBranchStatus("EF_2g5_loose") ) { fChain->SetBranchAddress("EF_2g5_loose", &EF_2g5_loose, &b_EF_2g5_loose); }
  if( fChain->GetBranchStatus("EF_2g7_loose") ) { fChain->SetBranchAddress("EF_2g7_loose", &EF_2g7_loose, &b_EF_2g7_loose); }
  if( fChain->GetBranchStatus("EF_2j35_jetNoEF_xe20_noMu") ) { fChain->SetBranchAddress("EF_2j35_jetNoEF_xe20_noMu", &EF_2j35_jetNoEF_xe20_noMu, &b_EF_2j35_jetNoEF_xe20_noMu); }
  if( fChain->GetBranchStatus("EF_2j35_jetNoEF_xe30_noMu") ) { fChain->SetBranchAddress("EF_2j35_jetNoEF_xe30_noMu", &EF_2j35_jetNoEF_xe30_noMu, &b_EF_2j35_jetNoEF_xe30_noMu); }
  if( fChain->GetBranchStatus("EF_2j35_jetNoEF_xe40_noMu") ) { fChain->SetBranchAddress("EF_2j35_jetNoEF_xe40_noMu", &EF_2j35_jetNoEF_xe40_noMu, &b_EF_2j35_jetNoEF_xe40_noMu); }
  if( fChain->GetBranchStatus("EF_2j35_xe20_noMu") ) { fChain->SetBranchAddress("EF_2j35_xe20_noMu", &EF_2j35_xe20_noMu, &b_EF_2j35_xe20_noMu); }
  if( fChain->GetBranchStatus("EF_2j35_xe30_noMu") ) { fChain->SetBranchAddress("EF_2j35_xe30_noMu", &EF_2j35_xe30_noMu, &b_EF_2j35_xe30_noMu); }
  if( fChain->GetBranchStatus("EF_2j35_xe40_noMu") ) { fChain->SetBranchAddress("EF_2j35_xe40_noMu", &EF_2j35_xe40_noMu, &b_EF_2j35_xe40_noMu); }
  if( fChain->GetBranchStatus("EF_2mu0_NoAlg") ) { fChain->SetBranchAddress("EF_2mu0_NoAlg", &EF_2mu0_NoAlg, &b_EF_2mu0_NoAlg); }
  if( fChain->GetBranchStatus("EF_2mu10") ) { fChain->SetBranchAddress("EF_2mu10", &EF_2mu10, &b_EF_2mu10); }
  if( fChain->GetBranchStatus("EF_2mu10_NoAlg") ) { fChain->SetBranchAddress("EF_2mu10_NoAlg", &EF_2mu10_NoAlg, &b_EF_2mu10_NoAlg); }
  if( fChain->GetBranchStatus("EF_2mu4") ) { fChain->SetBranchAddress("EF_2mu4", &EF_2mu4, &b_EF_2mu4); }
  if( fChain->GetBranchStatus("EF_2mu4_Bmumu") ) { fChain->SetBranchAddress("EF_2mu4_Bmumu", &EF_2mu4_Bmumu, &b_EF_2mu4_Bmumu); }
  if( fChain->GetBranchStatus("EF_2mu4_Bmumux") ) { fChain->SetBranchAddress("EF_2mu4_Bmumux", &EF_2mu4_Bmumux, &b_EF_2mu4_Bmumux); }
  if( fChain->GetBranchStatus("EF_2mu4_DiMu") ) { fChain->SetBranchAddress("EF_2mu4_DiMu", &EF_2mu4_DiMu, &b_EF_2mu4_DiMu); }
  if( fChain->GetBranchStatus("EF_2mu4_DiMu_SiTrk") ) { fChain->SetBranchAddress("EF_2mu4_DiMu_SiTrk", &EF_2mu4_DiMu_SiTrk, &b_EF_2mu4_DiMu_SiTrk); }
  if( fChain->GetBranchStatus("EF_2mu4_DiMu_noVtx") ) { fChain->SetBranchAddress("EF_2mu4_DiMu_noVtx", &EF_2mu4_DiMu_noVtx, &b_EF_2mu4_DiMu_noVtx); }
  if( fChain->GetBranchStatus("EF_2mu4_DiMu_noVtx_noOS") ) { fChain->SetBranchAddress("EF_2mu4_DiMu_noVtx_noOS", &EF_2mu4_DiMu_noVtx_noOS, &b_EF_2mu4_DiMu_noVtx_noOS); }
  if( fChain->GetBranchStatus("EF_2mu4_Jpsimumu") ) { fChain->SetBranchAddress("EF_2mu4_Jpsimumu", &EF_2mu4_Jpsimumu, &b_EF_2mu4_Jpsimumu); }
  if( fChain->GetBranchStatus("EF_2mu4_Jpsimumu_IDTrkNoCut") ) { fChain->SetBranchAddress("EF_2mu4_Jpsimumu_IDTrkNoCut", &EF_2mu4_Jpsimumu_IDTrkNoCut, &b_EF_2mu4_Jpsimumu_IDTrkNoCut); }
  if( fChain->GetBranchStatus("EF_2mu4_Upsimumu") ) { fChain->SetBranchAddress("EF_2mu4_Upsimumu", &EF_2mu4_Upsimumu, &b_EF_2mu4_Upsimumu); }
  if( fChain->GetBranchStatus("EF_2mu6") ) { fChain->SetBranchAddress("EF_2mu6", &EF_2mu6, &b_EF_2mu6); }
  if( fChain->GetBranchStatus("EF_2mu6_Bmumu") ) { fChain->SetBranchAddress("EF_2mu6_Bmumu", &EF_2mu6_Bmumu, &b_EF_2mu6_Bmumu); }
  if( fChain->GetBranchStatus("EF_2mu6_Bmumux") ) { fChain->SetBranchAddress("EF_2mu6_Bmumux", &EF_2mu6_Bmumux, &b_EF_2mu6_Bmumux); }
  if( fChain->GetBranchStatus("EF_2mu6_DiMu") ) { fChain->SetBranchAddress("EF_2mu6_DiMu", &EF_2mu6_DiMu, &b_EF_2mu6_DiMu); }
  if( fChain->GetBranchStatus("EF_2mu6_Jpsimumu") ) { fChain->SetBranchAddress("EF_2mu6_Jpsimumu", &EF_2mu6_Jpsimumu, &b_EF_2mu6_Jpsimumu); }
  if( fChain->GetBranchStatus("EF_2mu6_MG") ) { fChain->SetBranchAddress("EF_2mu6_MG", &EF_2mu6_MG, &b_EF_2mu6_MG); }
  if( fChain->GetBranchStatus("EF_2mu6_NoAlg") ) { fChain->SetBranchAddress("EF_2mu6_NoAlg", &EF_2mu6_NoAlg, &b_EF_2mu6_NoAlg); }
  if( fChain->GetBranchStatus("EF_2mu6_Upsimumu") ) { fChain->SetBranchAddress("EF_2mu6_Upsimumu", &EF_2mu6_Upsimumu, &b_EF_2mu6_Upsimumu); }
  if( fChain->GetBranchStatus("EF_2mu6_g10_loose") ) { fChain->SetBranchAddress("EF_2mu6_g10_loose", &EF_2mu6_g10_loose, &b_EF_2mu6_g10_loose); }
  if( fChain->GetBranchStatus("EF_2tau12_loose") ) { fChain->SetBranchAddress("EF_2tau12_loose", &EF_2tau12_loose, &b_EF_2tau12_loose); }
  if( fChain->GetBranchStatus("EF_2tau16_loose") ) { fChain->SetBranchAddress("EF_2tau16_loose", &EF_2tau16_loose, &b_EF_2tau16_loose); }
  if( fChain->GetBranchStatus("EF_2tau20_loose") ) { fChain->SetBranchAddress("EF_2tau20_loose", &EF_2tau20_loose, &b_EF_2tau20_loose); }
  if( fChain->GetBranchStatus("EF_2tau29_loose") ) { fChain->SetBranchAddress("EF_2tau29_loose", &EF_2tau29_loose, &b_EF_2tau29_loose); }
  if( fChain->GetBranchStatus("EF_2tau29_loose1") ) { fChain->SetBranchAddress("EF_2tau29_loose1", &EF_2tau29_loose1, &b_EF_2tau29_loose1); }
  if( fChain->GetBranchStatus("EF_2tau38_loose") ) { fChain->SetBranchAddress("EF_2tau38_loose", &EF_2tau38_loose, &b_EF_2tau38_loose); }
  if( fChain->GetBranchStatus("EF_e10_NoCut") ) { fChain->SetBranchAddress("EF_e10_NoCut", &EF_e10_NoCut, &b_EF_e10_NoCut); }
  if( fChain->GetBranchStatus("EF_e10_loose") ) { fChain->SetBranchAddress("EF_e10_loose", &EF_e10_loose, &b_EF_e10_loose); }
  if( fChain->GetBranchStatus("EF_e10_loose_mu10") ) { fChain->SetBranchAddress("EF_e10_loose_mu10", &EF_e10_loose_mu10, &b_EF_e10_loose_mu10); }
  if( fChain->GetBranchStatus("EF_e10_loose_mu6") ) { fChain->SetBranchAddress("EF_e10_loose_mu6", &EF_e10_loose_mu6, &b_EF_e10_loose_mu6); }
  if( fChain->GetBranchStatus("EF_e10_medium") ) { fChain->SetBranchAddress("EF_e10_medium", &EF_e10_medium, &b_EF_e10_medium); }
  if( fChain->GetBranchStatus("EF_e10_medium_IDTrkNoCut") ) { fChain->SetBranchAddress("EF_e10_medium_IDTrkNoCut", &EF_e10_medium_IDTrkNoCut, &b_EF_e10_medium_IDTrkNoCut); }
  if( fChain->GetBranchStatus("EF_e10_medium_SiTrk") ) { fChain->SetBranchAddress("EF_e10_medium_SiTrk", &EF_e10_medium_SiTrk, &b_EF_e10_medium_SiTrk); }
  if( fChain->GetBranchStatus("EF_e10_medium_TRT") ) { fChain->SetBranchAddress("EF_e10_medium_TRT", &EF_e10_medium_TRT, &b_EF_e10_medium_TRT); }
  if( fChain->GetBranchStatus("EF_e10_tight") ) { fChain->SetBranchAddress("EF_e10_tight", &EF_e10_tight, &b_EF_e10_tight); }
  if( fChain->GetBranchStatus("EF_e10_tight_TRT") ) { fChain->SetBranchAddress("EF_e10_tight_TRT", &EF_e10_tight_TRT, &b_EF_e10_tight_TRT); }
  if( fChain->GetBranchStatus("EF_e15_loose") ) { fChain->SetBranchAddress("EF_e15_loose", &EF_e15_loose, &b_EF_e15_loose); }
  if( fChain->GetBranchStatus("EF_e15_loose_IDTrkNoCut") ) { fChain->SetBranchAddress("EF_e15_loose_IDTrkNoCut", &EF_e15_loose_IDTrkNoCut, &b_EF_e15_loose_IDTrkNoCut); }
  if( fChain->GetBranchStatus("EF_e15_medium") ) { fChain->SetBranchAddress("EF_e15_medium", &EF_e15_medium, &b_EF_e15_medium); }
  if( fChain->GetBranchStatus("EF_e15_medium_SiTrk") ) { fChain->SetBranchAddress("EF_e15_medium_SiTrk", &EF_e15_medium_SiTrk, &b_EF_e15_medium_SiTrk); }
  if( fChain->GetBranchStatus("EF_e15_medium_TRT") ) { fChain->SetBranchAddress("EF_e15_medium_TRT", &EF_e15_medium_TRT, &b_EF_e15_medium_TRT); }
  if( fChain->GetBranchStatus("EF_e15_tight") ) { fChain->SetBranchAddress("EF_e15_tight", &EF_e15_tight, &b_EF_e15_tight); }
  if( fChain->GetBranchStatus("EF_e15_tight_TRT") ) { fChain->SetBranchAddress("EF_e15_tight_TRT", &EF_e15_tight_TRT, &b_EF_e15_tight_TRT); }
  if( fChain->GetBranchStatus("EF_e18_medium") ) { fChain->SetBranchAddress("EF_e18_medium", &EF_e18_medium, &b_EF_e18_medium); }
  if( fChain->GetBranchStatus("EF_e20_loose") ) { fChain->SetBranchAddress("EF_e20_loose", &EF_e20_loose, &b_EF_e20_loose); }
  if( fChain->GetBranchStatus("EF_e20_loose_IDTrkNoCut") ) { fChain->SetBranchAddress("EF_e20_loose_IDTrkNoCut", &EF_e20_loose_IDTrkNoCut, &b_EF_e20_loose_IDTrkNoCut); }
  if( fChain->GetBranchStatus("EF_e20_loose_TRT") ) { fChain->SetBranchAddress("EF_e20_loose_TRT", &EF_e20_loose_TRT, &b_EF_e20_loose_TRT); }
  if( fChain->GetBranchStatus("EF_e20_loose_passEF") ) { fChain->SetBranchAddress("EF_e20_loose_passEF", &EF_e20_loose_passEF, &b_EF_e20_loose_passEF); }
  if( fChain->GetBranchStatus("EF_e20_loose_passL2") ) { fChain->SetBranchAddress("EF_e20_loose_passL2", &EF_e20_loose_passL2, &b_EF_e20_loose_passL2); }
  if( fChain->GetBranchStatus("EF_e20_loose_xe20") ) { fChain->SetBranchAddress("EF_e20_loose_xe20", &EF_e20_loose_xe20, &b_EF_e20_loose_xe20); }
  if( fChain->GetBranchStatus("EF_e20_loose_xe20_noMu") ) { fChain->SetBranchAddress("EF_e20_loose_xe20_noMu", &EF_e20_loose_xe20_noMu, &b_EF_e20_loose_xe20_noMu); }
  if( fChain->GetBranchStatus("EF_e20_loose_xe30") ) { fChain->SetBranchAddress("EF_e20_loose_xe30", &EF_e20_loose_xe30, &b_EF_e20_loose_xe30); }
  if( fChain->GetBranchStatus("EF_e20_loose_xe30_noMu") ) { fChain->SetBranchAddress("EF_e20_loose_xe30_noMu", &EF_e20_loose_xe30_noMu, &b_EF_e20_loose_xe30_noMu); }
  if( fChain->GetBranchStatus("EF_e20_medium") ) { fChain->SetBranchAddress("EF_e20_medium", &EF_e20_medium, &b_EF_e20_medium); }
  if( fChain->GetBranchStatus("EF_e25_loose") ) { fChain->SetBranchAddress("EF_e25_loose", &EF_e25_loose, &b_EF_e25_loose); }
  if( fChain->GetBranchStatus("EF_e25_medium") ) { fChain->SetBranchAddress("EF_e25_medium", &EF_e25_medium, &b_EF_e25_medium); }
  if( fChain->GetBranchStatus("EF_e30_loose") ) { fChain->SetBranchAddress("EF_e30_loose", &EF_e30_loose, &b_EF_e30_loose); }
  if( fChain->GetBranchStatus("EF_e3_loose") ) { fChain->SetBranchAddress("EF_e3_loose", &EF_e3_loose, &b_EF_e3_loose); }
  if( fChain->GetBranchStatus("EF_e3_loose_SiTrk") ) { fChain->SetBranchAddress("EF_e3_loose_SiTrk", &EF_e3_loose_SiTrk, &b_EF_e3_loose_SiTrk); }
  if( fChain->GetBranchStatus("EF_e3_loose_TRT") ) { fChain->SetBranchAddress("EF_e3_loose_TRT", &EF_e3_loose_TRT, &b_EF_e3_loose_TRT); }
  if( fChain->GetBranchStatus("EF_e3_medium") ) { fChain->SetBranchAddress("EF_e3_medium", &EF_e3_medium, &b_EF_e3_medium); }
  if( fChain->GetBranchStatus("EF_e3_medium_SiTrk") ) { fChain->SetBranchAddress("EF_e3_medium_SiTrk", &EF_e3_medium_SiTrk, &b_EF_e3_medium_SiTrk); }
  if( fChain->GetBranchStatus("EF_e3_medium_TRT") ) { fChain->SetBranchAddress("EF_e3_medium_TRT", &EF_e3_medium_TRT, &b_EF_e3_medium_TRT); }
  if( fChain->GetBranchStatus("EF_e5_NoCut_L2SW") ) { fChain->SetBranchAddress("EF_e5_NoCut_L2SW", &EF_e5_NoCut_L2SW, &b_EF_e5_NoCut_L2SW); }
  if( fChain->GetBranchStatus("EF_e5_NoCut_Ringer") ) { fChain->SetBranchAddress("EF_e5_NoCut_Ringer", &EF_e5_NoCut_Ringer, &b_EF_e5_NoCut_Ringer); }
  if( fChain->GetBranchStatus("EF_e5_NoCut_firstempty") ) { fChain->SetBranchAddress("EF_e5_NoCut_firstempty", &EF_e5_NoCut_firstempty, &b_EF_e5_NoCut_firstempty); }
  if( fChain->GetBranchStatus("EF_e5_loose_mu4") ) { fChain->SetBranchAddress("EF_e5_loose_mu4", &EF_e5_loose_mu4, &b_EF_e5_loose_mu4); }
  if( fChain->GetBranchStatus("EF_e5_medium") ) { fChain->SetBranchAddress("EF_e5_medium", &EF_e5_medium, &b_EF_e5_medium); }
  if( fChain->GetBranchStatus("EF_e5_medium_MV") ) { fChain->SetBranchAddress("EF_e5_medium_MV", &EF_e5_medium_MV, &b_EF_e5_medium_MV); }
  if( fChain->GetBranchStatus("EF_e5_medium_SiTrk") ) { fChain->SetBranchAddress("EF_e5_medium_SiTrk", &EF_e5_medium_SiTrk, &b_EF_e5_medium_SiTrk); }
  if( fChain->GetBranchStatus("EF_e5_medium_TRT") ) { fChain->SetBranchAddress("EF_e5_medium_TRT", &EF_e5_medium_TRT, &b_EF_e5_medium_TRT); }
  if( fChain->GetBranchStatus("EF_e5_medium_mu4") ) { fChain->SetBranchAddress("EF_e5_medium_mu4", &EF_e5_medium_mu4, &b_EF_e5_medium_mu4); }
  if( fChain->GetBranchStatus("EF_e5_tight") ) { fChain->SetBranchAddress("EF_e5_tight", &EF_e5_tight, &b_EF_e5_tight); }
  if( fChain->GetBranchStatus("EF_e5_tight_SiTrk") ) { fChain->SetBranchAddress("EF_e5_tight_SiTrk", &EF_e5_tight_SiTrk, &b_EF_e5_tight_SiTrk); }
  if( fChain->GetBranchStatus("EF_e5_tight_TRT") ) { fChain->SetBranchAddress("EF_e5_tight_TRT", &EF_e5_tight_TRT, &b_EF_e5_tight_TRT); }
  if( fChain->GetBranchStatus("EF_e5_tight_e5_NoCut") ) { fChain->SetBranchAddress("EF_e5_tight_e5_NoCut", &EF_e5_tight_e5_NoCut, &b_EF_e5_tight_e5_NoCut); }
  if( fChain->GetBranchStatus("EF_em105_passHLT") ) { fChain->SetBranchAddress("EF_em105_passHLT", &EF_em105_passHLT, &b_EF_em105_passHLT); }
  if( fChain->GetBranchStatus("EF_g10_loose") ) { fChain->SetBranchAddress("EF_g10_loose", &EF_g10_loose, &b_EF_g10_loose); }
  if( fChain->GetBranchStatus("EF_g10_loose_larcalib") ) { fChain->SetBranchAddress("EF_g10_loose_larcalib", &EF_g10_loose_larcalib, &b_EF_g10_loose_larcalib); }
  if( fChain->GetBranchStatus("EF_g11_etcut") ) { fChain->SetBranchAddress("EF_g11_etcut", &EF_g11_etcut, &b_EF_g11_etcut); }
  if( fChain->GetBranchStatus("EF_g15_loose") ) { fChain->SetBranchAddress("EF_g15_loose", &EF_g15_loose, &b_EF_g15_loose); }
  if( fChain->GetBranchStatus("EF_g17_etcut") ) { fChain->SetBranchAddress("EF_g17_etcut", &EF_g17_etcut, &b_EF_g17_etcut); }
  if( fChain->GetBranchStatus("EF_g17_etcut_EFxe20_noMu") ) { fChain->SetBranchAddress("EF_g17_etcut_EFxe20_noMu", &EF_g17_etcut_EFxe20_noMu, &b_EF_g17_etcut_EFxe20_noMu); }
  if( fChain->GetBranchStatus("EF_g17_etcut_EFxe30_noMu") ) { fChain->SetBranchAddress("EF_g17_etcut_EFxe30_noMu", &EF_g17_etcut_EFxe30_noMu, &b_EF_g17_etcut_EFxe30_noMu); }
  if( fChain->GetBranchStatus("EF_g20_loose") ) { fChain->SetBranchAddress("EF_g20_loose", &EF_g20_loose, &b_EF_g20_loose); }
  if( fChain->GetBranchStatus("EF_g20_loose_cnv") ) { fChain->SetBranchAddress("EF_g20_loose_cnv", &EF_g20_loose_cnv, &b_EF_g20_loose_cnv); }
  if( fChain->GetBranchStatus("EF_g20_loose_larcalib") ) { fChain->SetBranchAddress("EF_g20_loose_larcalib", &EF_g20_loose_larcalib, &b_EF_g20_loose_larcalib); }
  if( fChain->GetBranchStatus("EF_g20_loose_xe20_noMu") ) { fChain->SetBranchAddress("EF_g20_loose_xe20_noMu", &EF_g20_loose_xe20_noMu, &b_EF_g20_loose_xe20_noMu); }
  if( fChain->GetBranchStatus("EF_g20_loose_xe30_noMu") ) { fChain->SetBranchAddress("EF_g20_loose_xe30_noMu", &EF_g20_loose_xe30_noMu, &b_EF_g20_loose_xe30_noMu); }
  if( fChain->GetBranchStatus("EF_g20_tight") ) { fChain->SetBranchAddress("EF_g20_tight", &EF_g20_tight, &b_EF_g20_tight); }
  if( fChain->GetBranchStatus("EF_g25_loose_xe30_noMu") ) { fChain->SetBranchAddress("EF_g25_loose_xe30_noMu", &EF_g25_loose_xe30_noMu, &b_EF_g25_loose_xe30_noMu); }
  if( fChain->GetBranchStatus("EF_g30_loose") ) { fChain->SetBranchAddress("EF_g30_loose", &EF_g30_loose, &b_EF_g30_loose); }
  if( fChain->GetBranchStatus("EF_g30_tight") ) { fChain->SetBranchAddress("EF_g30_tight", &EF_g30_tight, &b_EF_g30_tight); }
  if( fChain->GetBranchStatus("EF_g3_NoCut_unpaired_iso") ) { fChain->SetBranchAddress("EF_g3_NoCut_unpaired_iso", &EF_g3_NoCut_unpaired_iso, &b_EF_g3_NoCut_unpaired_iso); }
  if( fChain->GetBranchStatus("EF_g3_NoCut_unpaired_noniso") ) { fChain->SetBranchAddress("EF_g3_NoCut_unpaired_noniso", &EF_g3_NoCut_unpaired_noniso, &b_EF_g3_NoCut_unpaired_noniso); }
  if( fChain->GetBranchStatus("EF_g40_loose") ) { fChain->SetBranchAddress("EF_g40_loose", &EF_g40_loose, &b_EF_g40_loose); }
  if( fChain->GetBranchStatus("EF_g40_loose_larcalib") ) { fChain->SetBranchAddress("EF_g40_loose_larcalib", &EF_g40_loose_larcalib, &b_EF_g40_loose_larcalib); }
  if( fChain->GetBranchStatus("EF_g40_tight") ) { fChain->SetBranchAddress("EF_g40_tight", &EF_g40_tight, &b_EF_g40_tight); }
  if( fChain->GetBranchStatus("EF_g50_loose") ) { fChain->SetBranchAddress("EF_g50_loose", &EF_g50_loose, &b_EF_g50_loose); }
  if( fChain->GetBranchStatus("EF_g50_loose_larcalib") ) { fChain->SetBranchAddress("EF_g50_loose_larcalib", &EF_g50_loose_larcalib, &b_EF_g50_loose_larcalib); }
  if( fChain->GetBranchStatus("EF_g5_NoCut_cosmic") ) { fChain->SetBranchAddress("EF_g5_NoCut_cosmic", &EF_g5_NoCut_cosmic, &b_EF_g5_NoCut_cosmic); }
  if( fChain->GetBranchStatus("EF_g5_loose") ) { fChain->SetBranchAddress("EF_g5_loose", &EF_g5_loose, &b_EF_g5_loose); }
  if( fChain->GetBranchStatus("EF_g5_loose_cnv") ) { fChain->SetBranchAddress("EF_g5_loose_cnv", &EF_g5_loose_cnv, &b_EF_g5_loose_cnv); }
  if( fChain->GetBranchStatus("EF_g5_loose_larcalib") ) { fChain->SetBranchAddress("EF_g5_loose_larcalib", &EF_g5_loose_larcalib, &b_EF_g5_loose_larcalib); }
  if( fChain->GetBranchStatus("EF_g7_loose") ) { fChain->SetBranchAddress("EF_g7_loose", &EF_g7_loose, &b_EF_g7_loose); }
  if( fChain->GetBranchStatus("EF_j115_jetNoCut_larcalib") ) { fChain->SetBranchAddress("EF_j115_jetNoCut_larcalib", &EF_j115_jetNoCut_larcalib, &b_EF_j115_jetNoCut_larcalib); }
  if( fChain->GetBranchStatus("EF_j20") ) { fChain->SetBranchAddress("EF_j20", &EF_j20, &b_EF_j20); }
  if( fChain->GetBranchStatus("EF_j20_jetNoEF") ) { fChain->SetBranchAddress("EF_j20_jetNoEF", &EF_j20_jetNoEF, &b_EF_j20_jetNoEF); }
  if( fChain->GetBranchStatus("EF_j20_jetNoEF_larcalib") ) { fChain->SetBranchAddress("EF_j20_jetNoEF_larcalib", &EF_j20_jetNoEF_larcalib, &b_EF_j20_jetNoEF_larcalib); }
  if( fChain->GetBranchStatus("EF_j20a4") ) { fChain->SetBranchAddress("EF_j20a4", &EF_j20a4, &b_EF_j20a4); }
  if( fChain->GetBranchStatus("EF_j20a4fs") ) { fChain->SetBranchAddress("EF_j20a4fs", &EF_j20a4fs, &b_EF_j20a4fs); }
  if( fChain->GetBranchStatus("EF_j20a6") ) { fChain->SetBranchAddress("EF_j20a6", &EF_j20a6, &b_EF_j20a6); }
  if( fChain->GetBranchStatus("EF_j20a6fs") ) { fChain->SetBranchAddress("EF_j20a6fs", &EF_j20a6fs, &b_EF_j20a6fs); }
  if( fChain->GetBranchStatus("EF_j30") ) { fChain->SetBranchAddress("EF_j30", &EF_j30, &b_EF_j30); }
  if( fChain->GetBranchStatus("EF_j30_cosmic") ) { fChain->SetBranchAddress("EF_j30_cosmic", &EF_j30_cosmic, &b_EF_j30_cosmic); }
  if( fChain->GetBranchStatus("EF_j30_firstempty") ) { fChain->SetBranchAddress("EF_j30_firstempty", &EF_j30_firstempty, &b_EF_j30_firstempty); }
  if( fChain->GetBranchStatus("EF_j30_fj30_jetNoEF") ) { fChain->SetBranchAddress("EF_j30_fj30_jetNoEF", &EF_j30_fj30_jetNoEF, &b_EF_j30_fj30_jetNoEF); }
  if( fChain->GetBranchStatus("EF_j30_jetNoCut_cosmic") ) { fChain->SetBranchAddress("EF_j30_jetNoCut_cosmic", &EF_j30_jetNoCut_cosmic, &b_EF_j30_jetNoCut_cosmic); }
  if( fChain->GetBranchStatus("EF_j30_jetNoCut_firstempty") ) { fChain->SetBranchAddress("EF_j30_jetNoCut_firstempty", &EF_j30_jetNoCut_firstempty, &b_EF_j30_jetNoCut_firstempty); }
  if( fChain->GetBranchStatus("EF_j30_jetNoCut_unpaired") ) { fChain->SetBranchAddress("EF_j30_jetNoCut_unpaired", &EF_j30_jetNoCut_unpaired, &b_EF_j30_jetNoCut_unpaired); }
  if( fChain->GetBranchStatus("EF_j30_jetNoEF") ) { fChain->SetBranchAddress("EF_j30_jetNoEF", &EF_j30_jetNoEF, &b_EF_j30_jetNoEF); }
  if( fChain->GetBranchStatus("EF_j30_jetNoEF_cosmic") ) { fChain->SetBranchAddress("EF_j30_jetNoEF_cosmic", &EF_j30_jetNoEF_cosmic, &b_EF_j30_jetNoEF_cosmic); }
  if( fChain->GetBranchStatus("EF_j30_jetNoEF_firstempty") ) { fChain->SetBranchAddress("EF_j30_jetNoEF_firstempty", &EF_j30_jetNoEF_firstempty, &b_EF_j30_jetNoEF_firstempty); }
  if( fChain->GetBranchStatus("EF_j30_jetNoEF_unpaired") ) { fChain->SetBranchAddress("EF_j30_jetNoEF_unpaired", &EF_j30_jetNoEF_unpaired, &b_EF_j30_jetNoEF_unpaired); }
  if( fChain->GetBranchStatus("EF_j30_unpaired") ) { fChain->SetBranchAddress("EF_j30_unpaired", &EF_j30_unpaired, &b_EF_j30_unpaired); }
  if( fChain->GetBranchStatus("EF_j35") ) { fChain->SetBranchAddress("EF_j35", &EF_j35, &b_EF_j35); }
  if( fChain->GetBranchStatus("EF_j35_L1TAU_HV") ) { fChain->SetBranchAddress("EF_j35_L1TAU_HV", &EF_j35_L1TAU_HV, &b_EF_j35_L1TAU_HV); }
  if( fChain->GetBranchStatus("EF_j35_L1TAU_HV_jetNoEF") ) { fChain->SetBranchAddress("EF_j35_L1TAU_HV_jetNoEF", &EF_j35_L1TAU_HV_jetNoEF, &b_EF_j35_L1TAU_HV_jetNoEF); }
  if( fChain->GetBranchStatus("EF_j35_jetNoCut_xe30_e15_medium") ) { fChain->SetBranchAddress("EF_j35_jetNoCut_xe30_e15_medium", &EF_j35_jetNoCut_xe30_e15_medium, &b_EF_j35_jetNoCut_xe30_e15_medium); }
  if( fChain->GetBranchStatus("EF_j35_jetNoCut_xe30_mu15") ) { fChain->SetBranchAddress("EF_j35_jetNoCut_xe30_mu15", &EF_j35_jetNoCut_xe30_mu15, &b_EF_j35_jetNoCut_xe30_mu15); }
  if( fChain->GetBranchStatus("EF_j35_jetNoEF") ) { fChain->SetBranchAddress("EF_j35_jetNoEF", &EF_j35_jetNoEF, &b_EF_j35_jetNoEF); }
  if( fChain->GetBranchStatus("EF_j35_xe30_e15_medium") ) { fChain->SetBranchAddress("EF_j35_xe30_e15_medium", &EF_j35_xe30_e15_medium, &b_EF_j35_xe30_e15_medium); }
  if( fChain->GetBranchStatus("EF_j35_xe30_mu15") ) { fChain->SetBranchAddress("EF_j35_xe30_mu15", &EF_j35_xe30_mu15, &b_EF_j35_xe30_mu15); }
  if( fChain->GetBranchStatus("EF_j50") ) { fChain->SetBranchAddress("EF_j50", &EF_j50, &b_EF_j50); }
  if( fChain->GetBranchStatus("EF_j50_cosmic") ) { fChain->SetBranchAddress("EF_j50_cosmic", &EF_j50_cosmic, &b_EF_j50_cosmic); }
  if( fChain->GetBranchStatus("EF_j50_firstempty") ) { fChain->SetBranchAddress("EF_j50_firstempty", &EF_j50_firstempty, &b_EF_j50_firstempty); }
  if( fChain->GetBranchStatus("EF_j50_jetNoCut_cosmic") ) { fChain->SetBranchAddress("EF_j50_jetNoCut_cosmic", &EF_j50_jetNoCut_cosmic, &b_EF_j50_jetNoCut_cosmic); }
  if( fChain->GetBranchStatus("EF_j50_jetNoCut_firstempty") ) { fChain->SetBranchAddress("EF_j50_jetNoCut_firstempty", &EF_j50_jetNoCut_firstempty, &b_EF_j50_jetNoCut_firstempty); }
  if( fChain->GetBranchStatus("EF_j50_jetNoCut_unpaired") ) { fChain->SetBranchAddress("EF_j50_jetNoCut_unpaired", &EF_j50_jetNoCut_unpaired, &b_EF_j50_jetNoCut_unpaired); }
  if( fChain->GetBranchStatus("EF_j50_jetNoEF") ) { fChain->SetBranchAddress("EF_j50_jetNoEF", &EF_j50_jetNoEF, &b_EF_j50_jetNoEF); }
  if( fChain->GetBranchStatus("EF_j50_jetNoEF_cosmic") ) { fChain->SetBranchAddress("EF_j50_jetNoEF_cosmic", &EF_j50_jetNoEF_cosmic, &b_EF_j50_jetNoEF_cosmic); }
  if( fChain->GetBranchStatus("EF_j50_jetNoEF_firstempty") ) { fChain->SetBranchAddress("EF_j50_jetNoEF_firstempty", &EF_j50_jetNoEF_firstempty, &b_EF_j50_jetNoEF_firstempty); }
  if( fChain->GetBranchStatus("EF_j50_jetNoEF_larcalib") ) { fChain->SetBranchAddress("EF_j50_jetNoEF_larcalib", &EF_j50_jetNoEF_larcalib, &b_EF_j50_jetNoEF_larcalib); }
  if( fChain->GetBranchStatus("EF_j50_jetNoEF_unpaired") ) { fChain->SetBranchAddress("EF_j50_jetNoEF_unpaired", &EF_j50_jetNoEF_unpaired, &b_EF_j50_jetNoEF_unpaired); }
  if( fChain->GetBranchStatus("EF_j50_jetNoEF_xe20_noMu") ) { fChain->SetBranchAddress("EF_j50_jetNoEF_xe20_noMu", &EF_j50_jetNoEF_xe20_noMu, &b_EF_j50_jetNoEF_xe20_noMu); }
  if( fChain->GetBranchStatus("EF_j50_jetNoEF_xe30_noMu") ) { fChain->SetBranchAddress("EF_j50_jetNoEF_xe30_noMu", &EF_j50_jetNoEF_xe30_noMu, &b_EF_j50_jetNoEF_xe30_noMu); }
  if( fChain->GetBranchStatus("EF_j50_jetNoEF_xe40_noMu") ) { fChain->SetBranchAddress("EF_j50_jetNoEF_xe40_noMu", &EF_j50_jetNoEF_xe40_noMu, &b_EF_j50_jetNoEF_xe40_noMu); }
  if( fChain->GetBranchStatus("EF_j50_unpaired") ) { fChain->SetBranchAddress("EF_j50_unpaired", &EF_j50_unpaired, &b_EF_j50_unpaired); }
  if( fChain->GetBranchStatus("EF_j50_xe20_noMu") ) { fChain->SetBranchAddress("EF_j50_xe20_noMu", &EF_j50_xe20_noMu, &b_EF_j50_xe20_noMu); }
  if( fChain->GetBranchStatus("EF_j50_xe30_noMu") ) { fChain->SetBranchAddress("EF_j50_xe30_noMu", &EF_j50_xe30_noMu, &b_EF_j50_xe30_noMu); }
  if( fChain->GetBranchStatus("EF_j50_xe40_noMu") ) { fChain->SetBranchAddress("EF_j50_xe40_noMu", &EF_j50_xe40_noMu, &b_EF_j50_xe40_noMu); }
  if( fChain->GetBranchStatus("EF_j5_empty_NoAlg") ) { fChain->SetBranchAddress("EF_j5_empty_NoAlg", &EF_j5_empty_NoAlg, &b_EF_j5_empty_NoAlg); }
  if( fChain->GetBranchStatus("EF_j5_unpaired_iso_NoAlg") ) { fChain->SetBranchAddress("EF_j5_unpaired_iso_NoAlg", &EF_j5_unpaired_iso_NoAlg, &b_EF_j5_unpaired_iso_NoAlg); }
  if( fChain->GetBranchStatus("EF_j5_unpaired_noniso_NoAlg") ) { fChain->SetBranchAddress("EF_j5_unpaired_noniso_NoAlg", &EF_j5_unpaired_noniso_NoAlg, &b_EF_j5_unpaired_noniso_NoAlg); }
  if( fChain->GetBranchStatus("EF_j75") ) { fChain->SetBranchAddress("EF_j75", &EF_j75, &b_EF_j75); }
  if( fChain->GetBranchStatus("EF_j75_jetNoEF") ) { fChain->SetBranchAddress("EF_j75_jetNoEF", &EF_j75_jetNoEF, &b_EF_j75_jetNoEF); }
  if( fChain->GetBranchStatus("EF_j75_jetNoEF_EFxe20_noMu") ) { fChain->SetBranchAddress("EF_j75_jetNoEF_EFxe20_noMu", &EF_j75_jetNoEF_EFxe20_noMu, &b_EF_j75_jetNoEF_EFxe20_noMu); }
  if( fChain->GetBranchStatus("EF_j75_jetNoEF_EFxe25_noMu") ) { fChain->SetBranchAddress("EF_j75_jetNoEF_EFxe25_noMu", &EF_j75_jetNoEF_EFxe25_noMu, &b_EF_j75_jetNoEF_EFxe25_noMu); }
  if( fChain->GetBranchStatus("EF_j75_jetNoEF_EFxe30_noMu") ) { fChain->SetBranchAddress("EF_j75_jetNoEF_EFxe30_noMu", &EF_j75_jetNoEF_EFxe30_noMu, &b_EF_j75_jetNoEF_EFxe30_noMu); }
  if( fChain->GetBranchStatus("EF_j75_jetNoEF_EFxe35_noMu") ) { fChain->SetBranchAddress("EF_j75_jetNoEF_EFxe35_noMu", &EF_j75_jetNoEF_EFxe35_noMu, &b_EF_j75_jetNoEF_EFxe35_noMu); }
  if( fChain->GetBranchStatus("EF_j75_jetNoEF_EFxe40_noMu") ) { fChain->SetBranchAddress("EF_j75_jetNoEF_EFxe40_noMu", &EF_j75_jetNoEF_EFxe40_noMu, &b_EF_j75_jetNoEF_EFxe40_noMu); }
  if( fChain->GetBranchStatus("EF_j95") ) { fChain->SetBranchAddress("EF_j95", &EF_j95, &b_EF_j95); }
  if( fChain->GetBranchStatus("EF_j95_jetNoEF") ) { fChain->SetBranchAddress("EF_j95_jetNoEF", &EF_j95_jetNoEF, &b_EF_j95_jetNoEF); }
  if( fChain->GetBranchStatus("EF_j95_jetNoEF_larcalib") ) { fChain->SetBranchAddress("EF_j95_jetNoEF_larcalib", &EF_j95_jetNoEF_larcalib, &b_EF_j95_jetNoEF_larcalib); }
  if( fChain->GetBranchStatus("EF_je135_NoEF") ) { fChain->SetBranchAddress("EF_je135_NoEF", &EF_je135_NoEF, &b_EF_je135_NoEF); }
  if( fChain->GetBranchStatus("EF_je195_NoEF") ) { fChain->SetBranchAddress("EF_je195_NoEF", &EF_je195_NoEF, &b_EF_je195_NoEF); }
  if( fChain->GetBranchStatus("EF_je255_NoEF") ) { fChain->SetBranchAddress("EF_je255_NoEF", &EF_je255_NoEF, &b_EF_je255_NoEF); }
  if( fChain->GetBranchStatus("EF_je300_NoEF") ) { fChain->SetBranchAddress("EF_je300_NoEF", &EF_je300_NoEF, &b_EF_je300_NoEF); }
  if( fChain->GetBranchStatus("EF_je350_NoEF") ) { fChain->SetBranchAddress("EF_je350_NoEF", &EF_je350_NoEF, &b_EF_je350_NoEF); }
  if( fChain->GetBranchStatus("EF_mu0_NoAlg") ) { fChain->SetBranchAddress("EF_mu0_NoAlg", &EF_mu0_NoAlg, &b_EF_mu0_NoAlg); }
  if( fChain->GetBranchStatus("EF_mu0_comm_NoAlg") ) { fChain->SetBranchAddress("EF_mu0_comm_NoAlg", &EF_mu0_comm_NoAlg, &b_EF_mu0_comm_NoAlg); }
  if( fChain->GetBranchStatus("EF_mu0_comm_empty_NoAlg") ) { fChain->SetBranchAddress("EF_mu0_comm_empty_NoAlg", &EF_mu0_comm_empty_NoAlg, &b_EF_mu0_comm_empty_NoAlg); }
  if( fChain->GetBranchStatus("EF_mu0_comm_firstempty_NoAlg") ) { fChain->SetBranchAddress("EF_mu0_comm_firstempty_NoAlg", &EF_mu0_comm_firstempty_NoAlg, &b_EF_mu0_comm_firstempty_NoAlg); }
  if( fChain->GetBranchStatus("EF_mu0_comm_unpaired_iso_NoAlg") ) { fChain->SetBranchAddress("EF_mu0_comm_unpaired_iso_NoAlg", &EF_mu0_comm_unpaired_iso_NoAlg, &b_EF_mu0_comm_unpaired_iso_NoAlg); }
  if( fChain->GetBranchStatus("EF_mu0_comm_unpaired_noniso_NoAlg") ) { fChain->SetBranchAddress("EF_mu0_comm_unpaired_noniso_NoAlg", &EF_mu0_comm_unpaired_noniso_NoAlg, &b_EF_mu0_comm_unpaired_noniso_NoAlg); }
  if( fChain->GetBranchStatus("EF_mu0_empty_NoAlg") ) { fChain->SetBranchAddress("EF_mu0_empty_NoAlg", &EF_mu0_empty_NoAlg, &b_EF_mu0_empty_NoAlg); }
  if( fChain->GetBranchStatus("EF_mu0_firstempty_NoAlg") ) { fChain->SetBranchAddress("EF_mu0_firstempty_NoAlg", &EF_mu0_firstempty_NoAlg, &b_EF_mu0_firstempty_NoAlg); }
  if( fChain->GetBranchStatus("EF_mu0_missingRoi") ) { fChain->SetBranchAddress("EF_mu0_missingRoi", &EF_mu0_missingRoi, &b_EF_mu0_missingRoi); }
  if( fChain->GetBranchStatus("EF_mu0_outOfTime1") ) { fChain->SetBranchAddress("EF_mu0_outOfTime1", &EF_mu0_outOfTime1, &b_EF_mu0_outOfTime1); }
  if( fChain->GetBranchStatus("EF_mu0_outOfTime2") ) { fChain->SetBranchAddress("EF_mu0_outOfTime2", &EF_mu0_outOfTime2, &b_EF_mu0_outOfTime2); }
  if( fChain->GetBranchStatus("EF_mu0_rpcOnly") ) { fChain->SetBranchAddress("EF_mu0_rpcOnly", &EF_mu0_rpcOnly, &b_EF_mu0_rpcOnly); }
  if( fChain->GetBranchStatus("EF_mu0_unpaired_iso_NoAlg") ) { fChain->SetBranchAddress("EF_mu0_unpaired_iso_NoAlg", &EF_mu0_unpaired_iso_NoAlg, &b_EF_mu0_unpaired_iso_NoAlg); }
  if( fChain->GetBranchStatus("EF_mu0_unpaired_noniso_NoAlg") ) { fChain->SetBranchAddress("EF_mu0_unpaired_noniso_NoAlg", &EF_mu0_unpaired_noniso_NoAlg, &b_EF_mu0_unpaired_noniso_NoAlg); }
  if( fChain->GetBranchStatus("EF_mu10") ) { fChain->SetBranchAddress("EF_mu10", &EF_mu10, &b_EF_mu10); }
  if( fChain->GetBranchStatus("EF_mu10_Ecut12") ) { fChain->SetBranchAddress("EF_mu10_Ecut12", &EF_mu10_Ecut12, &b_EF_mu10_Ecut12); }
  if( fChain->GetBranchStatus("EF_mu10_Ecut123") ) { fChain->SetBranchAddress("EF_mu10_Ecut123", &EF_mu10_Ecut123, &b_EF_mu10_Ecut123); }
  if( fChain->GetBranchStatus("EF_mu10_Ecut13") ) { fChain->SetBranchAddress("EF_mu10_Ecut13", &EF_mu10_Ecut13, &b_EF_mu10_Ecut13); }
  if( fChain->GetBranchStatus("EF_mu10_IDTrkNoCut") ) { fChain->SetBranchAddress("EF_mu10_IDTrkNoCut", &EF_mu10_IDTrkNoCut, &b_EF_mu10_IDTrkNoCut); }
  if( fChain->GetBranchStatus("EF_mu10_MG") ) { fChain->SetBranchAddress("EF_mu10_MG", &EF_mu10_MG, &b_EF_mu10_MG); }
  if( fChain->GetBranchStatus("EF_mu10_MSonly") ) { fChain->SetBranchAddress("EF_mu10_MSonly", &EF_mu10_MSonly, &b_EF_mu10_MSonly); }
  if( fChain->GetBranchStatus("EF_mu10_MSonly_Ecut12") ) { fChain->SetBranchAddress("EF_mu10_MSonly_Ecut12", &EF_mu10_MSonly_Ecut12, &b_EF_mu10_MSonly_Ecut12); }
  if( fChain->GetBranchStatus("EF_mu10_MSonly_Ecut123") ) { fChain->SetBranchAddress("EF_mu10_MSonly_Ecut123", &EF_mu10_MSonly_Ecut123, &b_EF_mu10_MSonly_Ecut123); }
  if( fChain->GetBranchStatus("EF_mu10_MSonly_Ecut13") ) { fChain->SetBranchAddress("EF_mu10_MSonly_Ecut13", &EF_mu10_MSonly_Ecut13, &b_EF_mu10_MSonly_Ecut13); }
  if( fChain->GetBranchStatus("EF_mu10_MSonly_tight") ) { fChain->SetBranchAddress("EF_mu10_MSonly_tight", &EF_mu10_MSonly_tight, &b_EF_mu10_MSonly_tight); }
  if( fChain->GetBranchStatus("EF_mu10_NoAlg") ) { fChain->SetBranchAddress("EF_mu10_NoAlg", &EF_mu10_NoAlg, &b_EF_mu10_NoAlg); }
  if( fChain->GetBranchStatus("EF_mu10_SiTrk") ) { fChain->SetBranchAddress("EF_mu10_SiTrk", &EF_mu10_SiTrk, &b_EF_mu10_SiTrk); }
  if( fChain->GetBranchStatus("EF_mu10_j30") ) { fChain->SetBranchAddress("EF_mu10_j30", &EF_mu10_j30, &b_EF_mu10_j30); }
  if( fChain->GetBranchStatus("EF_mu10_tight") ) { fChain->SetBranchAddress("EF_mu10_tight", &EF_mu10_tight, &b_EF_mu10_tight); }
  if( fChain->GetBranchStatus("EF_mu10i_loose") ) { fChain->SetBranchAddress("EF_mu10i_loose", &EF_mu10i_loose, &b_EF_mu10i_loose); }
  if( fChain->GetBranchStatus("EF_mu13") ) { fChain->SetBranchAddress("EF_mu13", &EF_mu13, &b_EF_mu13); }
  if( fChain->GetBranchStatus("EF_mu13_MG") ) { fChain->SetBranchAddress("EF_mu13_MG", &EF_mu13_MG, &b_EF_mu13_MG); }
  if( fChain->GetBranchStatus("EF_mu13_MG_tight") ) { fChain->SetBranchAddress("EF_mu13_MG_tight", &EF_mu13_MG_tight, &b_EF_mu13_MG_tight); }
  if( fChain->GetBranchStatus("EF_mu13_tight") ) { fChain->SetBranchAddress("EF_mu13_tight", &EF_mu13_tight, &b_EF_mu13_tight); }
  if( fChain->GetBranchStatus("EF_mu15") ) { fChain->SetBranchAddress("EF_mu15", &EF_mu15, &b_EF_mu15); }
  if( fChain->GetBranchStatus("EF_mu15_MG") ) { fChain->SetBranchAddress("EF_mu15_MG", &EF_mu15_MG, &b_EF_mu15_MG); }
  if( fChain->GetBranchStatus("EF_mu15_NoAlg") ) { fChain->SetBranchAddress("EF_mu15_NoAlg", &EF_mu15_NoAlg, &b_EF_mu15_NoAlg); }
  if( fChain->GetBranchStatus("EF_mu18_MG") ) { fChain->SetBranchAddress("EF_mu18_MG", &EF_mu18_MG, &b_EF_mu18_MG); }
  if( fChain->GetBranchStatus("EF_mu20") ) { fChain->SetBranchAddress("EF_mu20", &EF_mu20, &b_EF_mu20); }
  if( fChain->GetBranchStatus("EF_mu20_MSonly") ) { fChain->SetBranchAddress("EF_mu20_MSonly", &EF_mu20_MSonly, &b_EF_mu20_MSonly); }
  if( fChain->GetBranchStatus("EF_mu20_NoAlg") ) { fChain->SetBranchAddress("EF_mu20_NoAlg", &EF_mu20_NoAlg, &b_EF_mu20_NoAlg); }
  if( fChain->GetBranchStatus("EF_mu20_passHLT") ) { fChain->SetBranchAddress("EF_mu20_passHLT", &EF_mu20_passHLT, &b_EF_mu20_passHLT); }
  if( fChain->GetBranchStatus("EF_mu20_slow") ) { fChain->SetBranchAddress("EF_mu20_slow", &EF_mu20_slow, &b_EF_mu20_slow); }
  if( fChain->GetBranchStatus("EF_mu30_MSonly") ) { fChain->SetBranchAddress("EF_mu30_MSonly", &EF_mu30_MSonly, &b_EF_mu30_MSonly); }
  if( fChain->GetBranchStatus("EF_mu40_MSonly_barrel") ) { fChain->SetBranchAddress("EF_mu40_MSonly_barrel", &EF_mu40_MSonly_barrel, &b_EF_mu40_MSonly_barrel); }
  if( fChain->GetBranchStatus("EF_mu4") ) { fChain->SetBranchAddress("EF_mu4", &EF_mu4, &b_EF_mu4); }
  if( fChain->GetBranchStatus("EF_mu40_MSonly") ) { fChain->SetBranchAddress("EF_mu40_MSonly", &EF_mu40_MSonly, &b_EF_mu40_MSonly); }
  if( fChain->GetBranchStatus("EF_mu4_Bmumu") ) { fChain->SetBranchAddress("EF_mu4_Bmumu", &EF_mu4_Bmumu, &b_EF_mu4_Bmumu); }
  if( fChain->GetBranchStatus("EF_mu4_BmumuX") ) { fChain->SetBranchAddress("EF_mu4_BmumuX", &EF_mu4_BmumuX, &b_EF_mu4_BmumuX); }
  if( fChain->GetBranchStatus("EF_mu4_DiMu") ) { fChain->SetBranchAddress("EF_mu4_DiMu", &EF_mu4_DiMu, &b_EF_mu4_DiMu); }
  if( fChain->GetBranchStatus("EF_mu4_DiMu_FS") ) { fChain->SetBranchAddress("EF_mu4_DiMu_FS", &EF_mu4_DiMu_FS, &b_EF_mu4_DiMu_FS); }
  if( fChain->GetBranchStatus("EF_mu4_DiMu_FS_noOS") ) { fChain->SetBranchAddress("EF_mu4_DiMu_FS_noOS", &EF_mu4_DiMu_FS_noOS, &b_EF_mu4_DiMu_FS_noOS); }
  if( fChain->GetBranchStatus("EF_mu4_DiMu_MG") ) { fChain->SetBranchAddress("EF_mu4_DiMu_MG", &EF_mu4_DiMu_MG, &b_EF_mu4_DiMu_MG); }
  if( fChain->GetBranchStatus("EF_mu4_DiMu_MG_FS") ) { fChain->SetBranchAddress("EF_mu4_DiMu_MG_FS", &EF_mu4_DiMu_MG_FS, &b_EF_mu4_DiMu_MG_FS); }
  if( fChain->GetBranchStatus("EF_mu4_DiMu_SiTrk") ) { fChain->SetBranchAddress("EF_mu4_DiMu_SiTrk", &EF_mu4_DiMu_SiTrk, &b_EF_mu4_DiMu_SiTrk); }
  if( fChain->GetBranchStatus("EF_mu4_DiMu_SiTrk_FS") ) { fChain->SetBranchAddress("EF_mu4_DiMu_SiTrk_FS", &EF_mu4_DiMu_SiTrk_FS, &b_EF_mu4_DiMu_SiTrk_FS); }
  if( fChain->GetBranchStatus("EF_mu4_DiMu_noOS") ) { fChain->SetBranchAddress("EF_mu4_DiMu_noOS", &EF_mu4_DiMu_noOS, &b_EF_mu4_DiMu_noOS); }
  if( fChain->GetBranchStatus("EF_mu4_IDTrkNoCut") ) { fChain->SetBranchAddress("EF_mu4_IDTrkNoCut", &EF_mu4_IDTrkNoCut, &b_EF_mu4_IDTrkNoCut); }
  if( fChain->GetBranchStatus("EF_mu4_Jpsie5e3") ) { fChain->SetBranchAddress("EF_mu4_Jpsie5e3", &EF_mu4_Jpsie5e3, &b_EF_mu4_Jpsie5e3); }
  if( fChain->GetBranchStatus("EF_mu4_Jpsie5e3_FS") ) { fChain->SetBranchAddress("EF_mu4_Jpsie5e3_FS", &EF_mu4_Jpsie5e3_FS, &b_EF_mu4_Jpsie5e3_FS); }
  if( fChain->GetBranchStatus("EF_mu4_Jpsie5e3_SiTrk") ) { fChain->SetBranchAddress("EF_mu4_Jpsie5e3_SiTrk", &EF_mu4_Jpsie5e3_SiTrk, &b_EF_mu4_Jpsie5e3_SiTrk); }
  if( fChain->GetBranchStatus("EF_mu4_Jpsie5e3_SiTrk_FS") ) { fChain->SetBranchAddress("EF_mu4_Jpsie5e3_SiTrk_FS", &EF_mu4_Jpsie5e3_SiTrk_FS, &b_EF_mu4_Jpsie5e3_SiTrk_FS); }
  if( fChain->GetBranchStatus("EF_mu4_Jpsimumu") ) { fChain->SetBranchAddress("EF_mu4_Jpsimumu", &EF_mu4_Jpsimumu, &b_EF_mu4_Jpsimumu); }
  if( fChain->GetBranchStatus("EF_mu4_Jpsimumu_FS") ) { fChain->SetBranchAddress("EF_mu4_Jpsimumu_FS", &EF_mu4_Jpsimumu_FS, &b_EF_mu4_Jpsimumu_FS); }
  if( fChain->GetBranchStatus("EF_mu4_Jpsimumu_SiTrk_FS") ) { fChain->SetBranchAddress("EF_mu4_Jpsimumu_SiTrk_FS", &EF_mu4_Jpsimumu_SiTrk_FS, &b_EF_mu4_Jpsimumu_SiTrk_FS); }
  if( fChain->GetBranchStatus("EF_mu4_L1J10_matched") ) { fChain->SetBranchAddress("EF_mu4_L1J10_matched", &EF_mu4_L1J10_matched, &b_EF_mu4_L1J10_matched); }
  if( fChain->GetBranchStatus("EF_mu4_L1J15_matched") ) { fChain->SetBranchAddress("EF_mu4_L1J15_matched", &EF_mu4_L1J15_matched, &b_EF_mu4_L1J15_matched); }
  if( fChain->GetBranchStatus("EF_mu4_L1J30_matched") ) { fChain->SetBranchAddress("EF_mu4_L1J30_matched", &EF_mu4_L1J30_matched, &b_EF_mu4_L1J30_matched); }
  if( fChain->GetBranchStatus("EF_mu4_L1J55_matched") ) { fChain->SetBranchAddress("EF_mu4_L1J55_matched", &EF_mu4_L1J55_matched, &b_EF_mu4_L1J55_matched); }
  if( fChain->GetBranchStatus("EF_mu4_L1J5_matched") ) { fChain->SetBranchAddress("EF_mu4_L1J5_matched", &EF_mu4_L1J5_matched, &b_EF_mu4_L1J5_matched); }
  if( fChain->GetBranchStatus("EF_mu4_L2MSonly_EFFS_passL2") ) { fChain->SetBranchAddress("EF_mu4_L2MSonly_EFFS_passL2", &EF_mu4_L2MSonly_EFFS_passL2, &b_EF_mu4_L2MSonly_EFFS_passL2); }
  if( fChain->GetBranchStatus("EF_mu4_MG") ) { fChain->SetBranchAddress("EF_mu4_MG", &EF_mu4_MG, &b_EF_mu4_MG); }
  if( fChain->GetBranchStatus("EF_mu4_MSonly") ) { fChain->SetBranchAddress("EF_mu4_MSonly", &EF_mu4_MSonly, &b_EF_mu4_MSonly); }
  if( fChain->GetBranchStatus("EF_mu4_MSonly_EFFS_passL2") ) { fChain->SetBranchAddress("EF_mu4_MSonly_EFFS_passL2", &EF_mu4_MSonly_EFFS_passL2, &b_EF_mu4_MSonly_EFFS_passL2); }
  if( fChain->GetBranchStatus("EF_mu4_MSonly_MB2_noL2_EFFS") ) { fChain->SetBranchAddress("EF_mu4_MSonly_MB2_noL2_EFFS", &EF_mu4_MSonly_MB2_noL2_EFFS, &b_EF_mu4_MSonly_MB2_noL2_EFFS); }
  if( fChain->GetBranchStatus("EF_mu4_MSonly_cosmic") ) { fChain->SetBranchAddress("EF_mu4_MSonly_cosmic", &EF_mu4_MSonly_cosmic, &b_EF_mu4_MSonly_cosmic); }
  if( fChain->GetBranchStatus("EF_mu4_MSonly_outOfTime") ) { fChain->SetBranchAddress("EF_mu4_MSonly_outOfTime", &EF_mu4_MSonly_outOfTime, &b_EF_mu4_MSonly_outOfTime); }
  if( fChain->GetBranchStatus("EF_mu4_MV") ) { fChain->SetBranchAddress("EF_mu4_MV", &EF_mu4_MV, &b_EF_mu4_MV); }
  if( fChain->GetBranchStatus("EF_mu4_SiTrk") ) { fChain->SetBranchAddress("EF_mu4_SiTrk", &EF_mu4_SiTrk, &b_EF_mu4_SiTrk); }
  if( fChain->GetBranchStatus("EF_mu4_Trk_Jpsi") ) { fChain->SetBranchAddress("EF_mu4_Trk_Jpsi", &EF_mu4_Trk_Jpsi, &b_EF_mu4_Trk_Jpsi); }
  if( fChain->GetBranchStatus("EF_mu4_Trk_Jpsi_FS") ) { fChain->SetBranchAddress("EF_mu4_Trk_Jpsi_FS", &EF_mu4_Trk_Jpsi_FS, &b_EF_mu4_Trk_Jpsi_FS); }
  if( fChain->GetBranchStatus("EF_mu4_Trk_Jpsi_loose") ) { fChain->SetBranchAddress("EF_mu4_Trk_Jpsi_loose", &EF_mu4_Trk_Jpsi_loose, &b_EF_mu4_Trk_Jpsi_loose); }
  if( fChain->GetBranchStatus("EF_mu4_Trk_Upsi_FS") ) { fChain->SetBranchAddress("EF_mu4_Trk_Upsi_FS", &EF_mu4_Trk_Upsi_FS, &b_EF_mu4_Trk_Upsi_FS); }
  if( fChain->GetBranchStatus("EF_mu4_Trk_Upsi_loose_FS") ) { fChain->SetBranchAddress("EF_mu4_Trk_Upsi_loose_FS", &EF_mu4_Trk_Upsi_loose_FS, &b_EF_mu4_Trk_Upsi_loose_FS); }
  if( fChain->GetBranchStatus("EF_mu4_Upsimumu_FS") ) { fChain->SetBranchAddress("EF_mu4_Upsimumu_FS", &EF_mu4_Upsimumu_FS, &b_EF_mu4_Upsimumu_FS); }
  if( fChain->GetBranchStatus("EF_mu4_Upsimumu_SiTrk_FS") ) { fChain->SetBranchAddress("EF_mu4_Upsimumu_SiTrk_FS", &EF_mu4_Upsimumu_SiTrk_FS, &b_EF_mu4_Upsimumu_SiTrk_FS); }
  if( fChain->GetBranchStatus("EF_mu4_comm_MSonly_cosmic") ) { fChain->SetBranchAddress("EF_mu4_comm_MSonly_cosmic", &EF_mu4_comm_MSonly_cosmic, &b_EF_mu4_comm_MSonly_cosmic); }
  if( fChain->GetBranchStatus("EF_mu4_comm_cosmic") ) { fChain->SetBranchAddress("EF_mu4_comm_cosmic", &EF_mu4_comm_cosmic, &b_EF_mu4_comm_cosmic); }
  if( fChain->GetBranchStatus("EF_mu4_comm_firstempty") ) { fChain->SetBranchAddress("EF_mu4_comm_firstempty", &EF_mu4_comm_firstempty, &b_EF_mu4_comm_firstempty); }
  if( fChain->GetBranchStatus("EF_mu4_comm_unpaired_iso") ) { fChain->SetBranchAddress("EF_mu4_comm_unpaired_iso", &EF_mu4_comm_unpaired_iso, &b_EF_mu4_comm_unpaired_iso); }
  if( fChain->GetBranchStatus("EF_mu4_cosmic") ) { fChain->SetBranchAddress("EF_mu4_cosmic", &EF_mu4_cosmic, &b_EF_mu4_cosmic); }
  if( fChain->GetBranchStatus("EF_mu4_firstempty") ) { fChain->SetBranchAddress("EF_mu4_firstempty", &EF_mu4_firstempty, &b_EF_mu4_firstempty); }
  if( fChain->GetBranchStatus("EF_mu4_j20") ) { fChain->SetBranchAddress("EF_mu4_j20", &EF_mu4_j20, &b_EF_mu4_j20); }
  if( fChain->GetBranchStatus("EF_mu4_j20_jetNoEF") ) { fChain->SetBranchAddress("EF_mu4_j20_jetNoEF", &EF_mu4_j20_jetNoEF, &b_EF_mu4_j20_jetNoEF); }
  if( fChain->GetBranchStatus("EF_mu4_j30") ) { fChain->SetBranchAddress("EF_mu4_j30", &EF_mu4_j30, &b_EF_mu4_j30); }
  if( fChain->GetBranchStatus("EF_mu4_j30_jetNoEF") ) { fChain->SetBranchAddress("EF_mu4_j30_jetNoEF", &EF_mu4_j30_jetNoEF, &b_EF_mu4_j30_jetNoEF); }
  if( fChain->GetBranchStatus("EF_mu4_mu6") ) { fChain->SetBranchAddress("EF_mu4_mu6", &EF_mu4_mu6, &b_EF_mu4_mu6); }
  if( fChain->GetBranchStatus("EF_mu4_muCombTag") ) { fChain->SetBranchAddress("EF_mu4_muCombTag", &EF_mu4_muCombTag, &b_EF_mu4_muCombTag); }
  if( fChain->GetBranchStatus("EF_mu4_tile") ) { fChain->SetBranchAddress("EF_mu4_tile", &EF_mu4_tile, &b_EF_mu4_tile); }
  if( fChain->GetBranchStatus("EF_mu4_tile_cosmic") ) { fChain->SetBranchAddress("EF_mu4_tile_cosmic", &EF_mu4_tile_cosmic, &b_EF_mu4_tile_cosmic); }
  if( fChain->GetBranchStatus("EF_mu4_unpaired_iso") ) { fChain->SetBranchAddress("EF_mu4_unpaired_iso", &EF_mu4_unpaired_iso, &b_EF_mu4_unpaired_iso); }
  if( fChain->GetBranchStatus("EF_mu4mu6_Bmumu") ) { fChain->SetBranchAddress("EF_mu4mu6_Bmumu", &EF_mu4mu6_Bmumu, &b_EF_mu4mu6_Bmumu); }
  if( fChain->GetBranchStatus("EF_mu4mu6_BmumuX") ) { fChain->SetBranchAddress("EF_mu4mu6_BmumuX", &EF_mu4mu6_BmumuX, &b_EF_mu4mu6_BmumuX); }
  if( fChain->GetBranchStatus("EF_mu4mu6_DiMu") ) { fChain->SetBranchAddress("EF_mu4mu6_DiMu", &EF_mu4mu6_DiMu, &b_EF_mu4mu6_DiMu); }
  if( fChain->GetBranchStatus("EF_mu4mu6_Jpsimumu") ) { fChain->SetBranchAddress("EF_mu4mu6_Jpsimumu", &EF_mu4mu6_Jpsimumu, &b_EF_mu4mu6_Jpsimumu); }
  if( fChain->GetBranchStatus("EF_mu4mu6_Jpsimumu_IDTrkNoCut") ) { fChain->SetBranchAddress("EF_mu4mu6_Jpsimumu_IDTrkNoCut", &EF_mu4mu6_Jpsimumu_IDTrkNoCut, &b_EF_mu4mu6_Jpsimumu_IDTrkNoCut); }
  if( fChain->GetBranchStatus("EF_mu4mu6_Upsimumu") ) { fChain->SetBranchAddress("EF_mu4mu6_Upsimumu", &EF_mu4mu6_Upsimumu, &b_EF_mu4mu6_Upsimumu); }
  if( fChain->GetBranchStatus("EF_mu6") ) { fChain->SetBranchAddress("EF_mu6", &EF_mu6, &b_EF_mu6); }
  if( fChain->GetBranchStatus("EF_mu6_Bmumu") ) { fChain->SetBranchAddress("EF_mu6_Bmumu", &EF_mu6_Bmumu, &b_EF_mu6_Bmumu); }
  if( fChain->GetBranchStatus("EF_mu6_BmumuX") ) { fChain->SetBranchAddress("EF_mu6_BmumuX", &EF_mu6_BmumuX, &b_EF_mu6_BmumuX); }
  if( fChain->GetBranchStatus("EF_mu6_DiMu") ) { fChain->SetBranchAddress("EF_mu6_DiMu", &EF_mu6_DiMu, &b_EF_mu6_DiMu); }
  if( fChain->GetBranchStatus("EF_mu6_Ecut12") ) { fChain->SetBranchAddress("EF_mu6_Ecut12", &EF_mu6_Ecut12, &b_EF_mu6_Ecut12); }
  if( fChain->GetBranchStatus("EF_mu6_Ecut123") ) { fChain->SetBranchAddress("EF_mu6_Ecut123", &EF_mu6_Ecut123, &b_EF_mu6_Ecut123); }
  if( fChain->GetBranchStatus("EF_mu6_Ecut13") ) { fChain->SetBranchAddress("EF_mu6_Ecut13", &EF_mu6_Ecut13, &b_EF_mu6_Ecut13); }
  if( fChain->GetBranchStatus("EF_mu6_Ecut2") ) { fChain->SetBranchAddress("EF_mu6_Ecut2", &EF_mu6_Ecut2, &b_EF_mu6_Ecut2); }
  if( fChain->GetBranchStatus("EF_mu6_Ecut3") ) { fChain->SetBranchAddress("EF_mu6_Ecut3", &EF_mu6_Ecut3, &b_EF_mu6_Ecut3); }
  if( fChain->GetBranchStatus("EF_mu6_IDTrkNoCut") ) { fChain->SetBranchAddress("EF_mu6_IDTrkNoCut", &EF_mu6_IDTrkNoCut, &b_EF_mu6_IDTrkNoCut); }
  if( fChain->GetBranchStatus("EF_mu6_Jpsie5e3") ) { fChain->SetBranchAddress("EF_mu6_Jpsie5e3", &EF_mu6_Jpsie5e3, &b_EF_mu6_Jpsie5e3); }
  if( fChain->GetBranchStatus("EF_mu6_Jpsie5e3_FS") ) { fChain->SetBranchAddress("EF_mu6_Jpsie5e3_FS", &EF_mu6_Jpsie5e3_FS, &b_EF_mu6_Jpsie5e3_FS); }
  if( fChain->GetBranchStatus("EF_mu6_Jpsie5e3_SiTrk") ) { fChain->SetBranchAddress("EF_mu6_Jpsie5e3_SiTrk", &EF_mu6_Jpsie5e3_SiTrk, &b_EF_mu6_Jpsie5e3_SiTrk); }
  if( fChain->GetBranchStatus("EF_mu6_Jpsie5e3_SiTrk_FS") ) { fChain->SetBranchAddress("EF_mu6_Jpsie5e3_SiTrk_FS", &EF_mu6_Jpsie5e3_SiTrk_FS, &b_EF_mu6_Jpsie5e3_SiTrk_FS); }
  if( fChain->GetBranchStatus("EF_mu6_Jpsimumu") ) { fChain->SetBranchAddress("EF_mu6_Jpsimumu", &EF_mu6_Jpsimumu, &b_EF_mu6_Jpsimumu); }
  if( fChain->GetBranchStatus("EF_mu6_MG") ) { fChain->SetBranchAddress("EF_mu6_MG", &EF_mu6_MG, &b_EF_mu6_MG); }
  if( fChain->GetBranchStatus("EF_mu6_MSonly") ) { fChain->SetBranchAddress("EF_mu6_MSonly", &EF_mu6_MSonly, &b_EF_mu6_MSonly); }
  if( fChain->GetBranchStatus("EF_mu6_MSonly_Ecut12") ) { fChain->SetBranchAddress("EF_mu6_MSonly_Ecut12", &EF_mu6_MSonly_Ecut12, &b_EF_mu6_MSonly_Ecut12); }
  if( fChain->GetBranchStatus("EF_mu6_MSonly_Ecut123") ) { fChain->SetBranchAddress("EF_mu6_MSonly_Ecut123", &EF_mu6_MSonly_Ecut123, &b_EF_mu6_MSonly_Ecut123); }
  if( fChain->GetBranchStatus("EF_mu6_MSonly_Ecut13") ) { fChain->SetBranchAddress("EF_mu6_MSonly_Ecut13", &EF_mu6_MSonly_Ecut13, &b_EF_mu6_MSonly_Ecut13); }
  if( fChain->GetBranchStatus("EF_mu6_MSonly_Ecut2") ) { fChain->SetBranchAddress("EF_mu6_MSonly_Ecut2", &EF_mu6_MSonly_Ecut2, &b_EF_mu6_MSonly_Ecut2); }
  if( fChain->GetBranchStatus("EF_mu6_MSonly_Ecut3") ) { fChain->SetBranchAddress("EF_mu6_MSonly_Ecut3", &EF_mu6_MSonly_Ecut3, &b_EF_mu6_MSonly_Ecut3); }
  if( fChain->GetBranchStatus("EF_mu6_MSonly_outOfTime") ) { fChain->SetBranchAddress("EF_mu6_MSonly_outOfTime", &EF_mu6_MSonly_outOfTime, &b_EF_mu6_MSonly_outOfTime); }
  if( fChain->GetBranchStatus("EF_mu6_NoAlg") ) { fChain->SetBranchAddress("EF_mu6_NoAlg", &EF_mu6_NoAlg, &b_EF_mu6_NoAlg); }
  if( fChain->GetBranchStatus("EF_mu6_SiTrk") ) { fChain->SetBranchAddress("EF_mu6_SiTrk", &EF_mu6_SiTrk, &b_EF_mu6_SiTrk); }
  if( fChain->GetBranchStatus("EF_mu6_Trk_Jpsi") ) { fChain->SetBranchAddress("EF_mu6_Trk_Jpsi", &EF_mu6_Trk_Jpsi, &b_EF_mu6_Trk_Jpsi); }
  if( fChain->GetBranchStatus("EF_mu6_Upsimumu_FS") ) { fChain->SetBranchAddress("EF_mu6_Upsimumu_FS", &EF_mu6_Upsimumu_FS, &b_EF_mu6_Upsimumu_FS); }
  if( fChain->GetBranchStatus("EF_mu6_muCombTag") ) { fChain->SetBranchAddress("EF_mu6_muCombTag", &EF_mu6_muCombTag, &b_EF_mu6_muCombTag); }
  if( fChain->GetBranchStatus("EF_tau125_loose") ) { fChain->SetBranchAddress("EF_tau125_loose", &EF_tau125_loose, &b_EF_tau125_loose); }
  if( fChain->GetBranchStatus("EF_tau125_medium") ) { fChain->SetBranchAddress("EF_tau125_medium", &EF_tau125_medium, &b_EF_tau125_medium); }
  if( fChain->GetBranchStatus("EF_tau12_IDTrkNoCut") ) { fChain->SetBranchAddress("EF_tau12_IDTrkNoCut", &EF_tau12_IDTrkNoCut, &b_EF_tau12_IDTrkNoCut); }
  if( fChain->GetBranchStatus("EF_tau12_loose") ) { fChain->SetBranchAddress("EF_tau12_loose", &EF_tau12_loose, &b_EF_tau12_loose); }
  if( fChain->GetBranchStatus("EF_tau12_loose_2b15") ) { fChain->SetBranchAddress("EF_tau12_loose_2b15", &EF_tau12_loose_2b15, &b_EF_tau12_loose_2b15); }
  if( fChain->GetBranchStatus("EF_tau12_loose_3j35") ) { fChain->SetBranchAddress("EF_tau12_loose_3j35", &EF_tau12_loose_3j35, &b_EF_tau12_loose_3j35); }
  if( fChain->GetBranchStatus("EF_tau12_loose_3j35_jetNoEF") ) { fChain->SetBranchAddress("EF_tau12_loose_3j35_jetNoEF", &EF_tau12_loose_3j35_jetNoEF, &b_EF_tau12_loose_3j35_jetNoEF); }
  if( fChain->GetBranchStatus("EF_tau12_loose_EFxe12_noMu") ) { fChain->SetBranchAddress("EF_tau12_loose_EFxe12_noMu", &EF_tau12_loose_EFxe12_noMu, &b_EF_tau12_loose_EFxe12_noMu); }
  if( fChain->GetBranchStatus("EF_tau12_loose_IdScan_EFxe12_noMu") ) { fChain->SetBranchAddress("EF_tau12_loose_IdScan_EFxe12_noMu", &EF_tau12_loose_IdScan_EFxe12_noMu, &b_EF_tau12_loose_IdScan_EFxe12_noMu); }
  if( fChain->GetBranchStatus("EF_tau12_loose_IdScan_xe15_noMu") ) { fChain->SetBranchAddress("EF_tau12_loose_IdScan_xe15_noMu", &EF_tau12_loose_IdScan_xe15_noMu, &b_EF_tau12_loose_IdScan_xe15_noMu); }
  if( fChain->GetBranchStatus("EF_tau12_loose_e10_loose") ) { fChain->SetBranchAddress("EF_tau12_loose_e10_loose", &EF_tau12_loose_e10_loose, &b_EF_tau12_loose_e10_loose); }
  if( fChain->GetBranchStatus("EF_tau12_loose_e10_medium") ) { fChain->SetBranchAddress("EF_tau12_loose_e10_medium", &EF_tau12_loose_e10_medium, &b_EF_tau12_loose_e10_medium); }
  if( fChain->GetBranchStatus("EF_tau12_loose_e10_tight") ) { fChain->SetBranchAddress("EF_tau12_loose_e10_tight", &EF_tau12_loose_e10_tight, &b_EF_tau12_loose_e10_tight); }
  if( fChain->GetBranchStatus("EF_tau12_loose_mu10") ) { fChain->SetBranchAddress("EF_tau12_loose_mu10", &EF_tau12_loose_mu10, &b_EF_tau12_loose_mu10); }
  if( fChain->GetBranchStatus("EF_tau12_loose_xe15_noMu") ) { fChain->SetBranchAddress("EF_tau12_loose_xe15_noMu", &EF_tau12_loose_xe15_noMu, &b_EF_tau12_loose_xe15_noMu); }
  if( fChain->GetBranchStatus("EF_tau12_loose_xe20_noMu") ) { fChain->SetBranchAddress("EF_tau12_loose_xe20_noMu", &EF_tau12_loose_xe20_noMu, &b_EF_tau12_loose_xe20_noMu); }
  if( fChain->GetBranchStatus("EF_tau16_loose") ) { fChain->SetBranchAddress("EF_tau16_loose", &EF_tau16_loose, &b_EF_tau16_loose); }
  if( fChain->GetBranchStatus("EF_tau16_loose_2b15") ) { fChain->SetBranchAddress("EF_tau16_loose_2b15", &EF_tau16_loose_2b15, &b_EF_tau16_loose_2b15); }
  if( fChain->GetBranchStatus("EF_tau16_loose_3j35") ) { fChain->SetBranchAddress("EF_tau16_loose_3j35", &EF_tau16_loose_3j35, &b_EF_tau16_loose_3j35); }
  if( fChain->GetBranchStatus("EF_tau16_loose_3j35_jetNoEF") ) { fChain->SetBranchAddress("EF_tau16_loose_3j35_jetNoEF", &EF_tau16_loose_3j35_jetNoEF, &b_EF_tau16_loose_3j35_jetNoEF); }
  if( fChain->GetBranchStatus("EF_tau16_loose_e10_loose") ) { fChain->SetBranchAddress("EF_tau16_loose_e10_loose", &EF_tau16_loose_e10_loose, &b_EF_tau16_loose_e10_loose); }
  if( fChain->GetBranchStatus("EF_tau16_loose_e15_loose") ) { fChain->SetBranchAddress("EF_tau16_loose_e15_loose", &EF_tau16_loose_e15_loose, &b_EF_tau16_loose_e15_loose); }
  if( fChain->GetBranchStatus("EF_tau16_loose_mu10") ) { fChain->SetBranchAddress("EF_tau16_loose_mu10", &EF_tau16_loose_mu10, &b_EF_tau16_loose_mu10); }
  if( fChain->GetBranchStatus("EF_tau16_loose_mu15") ) { fChain->SetBranchAddress("EF_tau16_loose_mu15", &EF_tau16_loose_mu15, &b_EF_tau16_loose_mu15); }
  if( fChain->GetBranchStatus("EF_tau16_loose_xe20_noMu") ) { fChain->SetBranchAddress("EF_tau16_loose_xe20_noMu", &EF_tau16_loose_xe20_noMu, &b_EF_tau16_loose_xe20_noMu); }
  if( fChain->GetBranchStatus("EF_tau16_loose_xe25_noMu") ) { fChain->SetBranchAddress("EF_tau16_loose_xe25_noMu", &EF_tau16_loose_xe25_noMu, &b_EF_tau16_loose_xe25_noMu); }
  if( fChain->GetBranchStatus("EF_tau16_loose_xe25_tight_noMu") ) { fChain->SetBranchAddress("EF_tau16_loose_xe25_tight_noMu", &EF_tau16_loose_xe25_tight_noMu, &b_EF_tau16_loose_xe25_tight_noMu); }
  if( fChain->GetBranchStatus("EF_tau16_loose_xe30_noMu") ) { fChain->SetBranchAddress("EF_tau16_loose_xe30_noMu", &EF_tau16_loose_xe30_noMu, &b_EF_tau16_loose_xe30_noMu); }
  if( fChain->GetBranchStatus("EF_tau16_medium") ) { fChain->SetBranchAddress("EF_tau16_medium", &EF_tau16_medium, &b_EF_tau16_medium); }
  if( fChain->GetBranchStatus("EF_tau16_medium_xe22_noMu") ) { fChain->SetBranchAddress("EF_tau16_medium_xe22_noMu", &EF_tau16_medium_xe22_noMu, &b_EF_tau16_medium_xe22_noMu); }
  if( fChain->GetBranchStatus("EF_tau16_medium_xe25_noMu") ) { fChain->SetBranchAddress("EF_tau16_medium_xe25_noMu", &EF_tau16_medium_xe25_noMu, &b_EF_tau16_medium_xe25_noMu); }
  if( fChain->GetBranchStatus("EF_tau16_medium_xe25_tight_noMu") ) { fChain->SetBranchAddress("EF_tau16_medium_xe25_tight_noMu", &EF_tau16_medium_xe25_tight_noMu, &b_EF_tau16_medium_xe25_tight_noMu); }
  if( fChain->GetBranchStatus("EF_tau20_loose") ) { fChain->SetBranchAddress("EF_tau20_loose", &EF_tau20_loose, &b_EF_tau20_loose); }
  if( fChain->GetBranchStatus("EF_tau20_loose_xe25_noMu") ) { fChain->SetBranchAddress("EF_tau20_loose_xe25_noMu", &EF_tau20_loose_xe25_noMu, &b_EF_tau20_loose_xe25_noMu); }
  if( fChain->GetBranchStatus("EF_tau29_loose") ) { fChain->SetBranchAddress("EF_tau29_loose", &EF_tau29_loose, &b_EF_tau29_loose); }
  if( fChain->GetBranchStatus("EF_tau29_loose1") ) { fChain->SetBranchAddress("EF_tau29_loose1", &EF_tau29_loose1, &b_EF_tau29_loose1); }
  if( fChain->GetBranchStatus("EF_tau38_loose") ) { fChain->SetBranchAddress("EF_tau38_loose", &EF_tau38_loose, &b_EF_tau38_loose); }
  if( fChain->GetBranchStatus("EF_tau38_medium") ) { fChain->SetBranchAddress("EF_tau38_medium", &EF_tau38_medium, &b_EF_tau38_medium); }
  if( fChain->GetBranchStatus("EF_tau50_IDTrkNoCut") ) { fChain->SetBranchAddress("EF_tau50_IDTrkNoCut", &EF_tau50_IDTrkNoCut, &b_EF_tau50_IDTrkNoCut); }
  if( fChain->GetBranchStatus("EF_tau50_loose") ) { fChain->SetBranchAddress("EF_tau50_loose", &EF_tau50_loose, &b_EF_tau50_loose); }
  if( fChain->GetBranchStatus("EF_tau50_loose_IdScan") ) { fChain->SetBranchAddress("EF_tau50_loose_IdScan", &EF_tau50_loose_IdScan, &b_EF_tau50_loose_IdScan); }
  if( fChain->GetBranchStatus("EF_tau50_medium") ) { fChain->SetBranchAddress("EF_tau50_medium", &EF_tau50_medium, &b_EF_tau50_medium); }
  if( fChain->GetBranchStatus("EF_tau84_loose") ) { fChain->SetBranchAddress("EF_tau84_loose", &EF_tau84_loose, &b_EF_tau84_loose); }
  if( fChain->GetBranchStatus("EF_tau84_medium") ) { fChain->SetBranchAddress("EF_tau84_medium", &EF_tau84_medium, &b_EF_tau84_medium); }
  if( fChain->GetBranchStatus("EF_tauNoCut") ) { fChain->SetBranchAddress("EF_tauNoCut", &EF_tauNoCut, &b_EF_tauNoCut); }
  if( fChain->GetBranchStatus("EF_tauNoCut_IdScan") ) { fChain->SetBranchAddress("EF_tauNoCut_IdScan", &EF_tauNoCut_IdScan, &b_EF_tauNoCut_IdScan); }
  if( fChain->GetBranchStatus("EF_tauNoCut_cosmic") ) { fChain->SetBranchAddress("EF_tauNoCut_cosmic", &EF_tauNoCut_cosmic, &b_EF_tauNoCut_cosmic); }
  if( fChain->GetBranchStatus("EF_tauNoCut_firstempty") ) { fChain->SetBranchAddress("EF_tauNoCut_firstempty", &EF_tauNoCut_firstempty, &b_EF_tauNoCut_firstempty); }
  if( fChain->GetBranchStatus("EF_tauNoCut_hasTrk6_EFxe15_noMu") ) { fChain->SetBranchAddress("EF_tauNoCut_hasTrk6_EFxe15_noMu", &EF_tauNoCut_hasTrk6_EFxe15_noMu, &b_EF_tauNoCut_hasTrk6_EFxe15_noMu); }
  if( fChain->GetBranchStatus("EF_tauNoCut_hasTrk6_IdScan_EFxe15_noMu") ) { fChain->SetBranchAddress("EF_tauNoCut_hasTrk6_IdScan_EFxe15_noMu", &EF_tauNoCut_hasTrk6_IdScan_EFxe15_noMu, &b_EF_tauNoCut_hasTrk6_IdScan_EFxe15_noMu); }
  if( fChain->GetBranchStatus("EF_tauNoCut_hasTrk6_IdScan_xe20_noMu") ) { fChain->SetBranchAddress("EF_tauNoCut_hasTrk6_IdScan_xe20_noMu", &EF_tauNoCut_hasTrk6_IdScan_xe20_noMu, &b_EF_tauNoCut_hasTrk6_IdScan_xe20_noMu); }
  if( fChain->GetBranchStatus("EF_tauNoCut_hasTrk6_xe20_noMu") ) { fChain->SetBranchAddress("EF_tauNoCut_hasTrk6_xe20_noMu", &EF_tauNoCut_hasTrk6_xe20_noMu, &b_EF_tauNoCut_hasTrk6_xe20_noMu); }
  if( fChain->GetBranchStatus("EF_tauNoCut_hasTrk9_xe20_noMu") ) { fChain->SetBranchAddress("EF_tauNoCut_hasTrk9_xe20_noMu", &EF_tauNoCut_hasTrk9_xe20_noMu, &b_EF_tauNoCut_hasTrk9_xe20_noMu); }
  if( fChain->GetBranchStatus("EF_tauNoCut_hasTrk_MV") ) { fChain->SetBranchAddress("EF_tauNoCut_hasTrk_MV", &EF_tauNoCut_hasTrk_MV, &b_EF_tauNoCut_hasTrk_MV); }
  if( fChain->GetBranchStatus("EF_tauNoCut_hasTrk_e10_tight") ) { fChain->SetBranchAddress("EF_tauNoCut_hasTrk_e10_tight", &EF_tauNoCut_hasTrk_e10_tight, &b_EF_tauNoCut_hasTrk_e10_tight); }
  if( fChain->GetBranchStatus("EF_tauNoCut_hasTrk_xe20_noMu") ) { fChain->SetBranchAddress("EF_tauNoCut_hasTrk_xe20_noMu", &EF_tauNoCut_hasTrk_xe20_noMu, &b_EF_tauNoCut_hasTrk_xe20_noMu); }
  if( fChain->GetBranchStatus("EF_tauNoCut_unpaired_iso") ) { fChain->SetBranchAddress("EF_tauNoCut_unpaired_iso", &EF_tauNoCut_unpaired_iso, &b_EF_tauNoCut_unpaired_iso); }
  if( fChain->GetBranchStatus("EF_tauNoCut_unpaired_noniso") ) { fChain->SetBranchAddress("EF_tauNoCut_unpaired_noniso", &EF_tauNoCut_unpaired_noniso, &b_EF_tauNoCut_unpaired_noniso); }
  if( fChain->GetBranchStatus("EF_xe15") ) { fChain->SetBranchAddress("EF_xe15", &EF_xe15, &b_EF_xe15); }
  if( fChain->GetBranchStatus("EF_xe15_noMu") ) { fChain->SetBranchAddress("EF_xe15_noMu", &EF_xe15_noMu, &b_EF_xe15_noMu); }
  if( fChain->GetBranchStatus("EF_xe15_unbiased_noMu") ) { fChain->SetBranchAddress("EF_xe15_unbiased_noMu", &EF_xe15_unbiased_noMu, &b_EF_xe15_unbiased_noMu); }
  if( fChain->GetBranchStatus("EF_xe20") ) { fChain->SetBranchAddress("EF_xe20", &EF_xe20, &b_EF_xe20); }
  if( fChain->GetBranchStatus("EF_xe20_noMu") ) { fChain->SetBranchAddress("EF_xe20_noMu", &EF_xe20_noMu, &b_EF_xe20_noMu); }
  if( fChain->GetBranchStatus("EF_xe20_tight_noMu") ) { fChain->SetBranchAddress("EF_xe20_tight_noMu", &EF_xe20_tight_noMu, &b_EF_xe20_tight_noMu); }
  if( fChain->GetBranchStatus("EF_xe20_tight_vfj_noMu") ) { fChain->SetBranchAddress("EF_xe20_tight_vfj_noMu", &EF_xe20_tight_vfj_noMu, &b_EF_xe20_tight_vfj_noMu); }
  if( fChain->GetBranchStatus("EF_xe25") ) { fChain->SetBranchAddress("EF_xe25", &EF_xe25, &b_EF_xe25); }
  if( fChain->GetBranchStatus("EF_xe25_medium") ) { fChain->SetBranchAddress("EF_xe25_medium", &EF_xe25_medium, &b_EF_xe25_medium); }
  if( fChain->GetBranchStatus("EF_xe25_medium_noMu") ) { fChain->SetBranchAddress("EF_xe25_medium_noMu", &EF_xe25_medium_noMu, &b_EF_xe25_medium_noMu); }
  if( fChain->GetBranchStatus("EF_xe25_noMu") ) { fChain->SetBranchAddress("EF_xe25_noMu", &EF_xe25_noMu, &b_EF_xe25_noMu); }
  if( fChain->GetBranchStatus("EF_xe25_tight_noMu") ) { fChain->SetBranchAddress("EF_xe25_tight_noMu", &EF_xe25_tight_noMu, &b_EF_xe25_tight_noMu); }
  if( fChain->GetBranchStatus("EF_xe25_tight_vfj_noMu") ) { fChain->SetBranchAddress("EF_xe25_tight_vfj_noMu", &EF_xe25_tight_vfj_noMu, &b_EF_xe25_tight_vfj_noMu); }
  if( fChain->GetBranchStatus("EF_xe25_vfj_noMu") ) { fChain->SetBranchAddress("EF_xe25_vfj_noMu", &EF_xe25_vfj_noMu, &b_EF_xe25_vfj_noMu); }
  if( fChain->GetBranchStatus("EF_xe30") ) { fChain->SetBranchAddress("EF_xe30", &EF_xe30, &b_EF_xe30); }
  if( fChain->GetBranchStatus("EF_xe30_allL1") ) { fChain->SetBranchAddress("EF_xe30_allL1", &EF_xe30_allL1, &b_EF_xe30_allL1); }
  if( fChain->GetBranchStatus("EF_xe30_allL1_FEB") ) { fChain->SetBranchAddress("EF_xe30_allL1_FEB", &EF_xe30_allL1_FEB, &b_EF_xe30_allL1_FEB); }
  if( fChain->GetBranchStatus("EF_xe30_allL1_allCells") ) { fChain->SetBranchAddress("EF_xe30_allL1_allCells", &EF_xe30_allL1_allCells, &b_EF_xe30_allL1_allCells); }
  if( fChain->GetBranchStatus("EF_xe30_allL1_noMu") ) { fChain->SetBranchAddress("EF_xe30_allL1_noMu", &EF_xe30_allL1_noMu, &b_EF_xe30_allL1_noMu); }
  if( fChain->GetBranchStatus("EF_xe30_loose") ) { fChain->SetBranchAddress("EF_xe30_loose", &EF_xe30_loose, &b_EF_xe30_loose); }
  if( fChain->GetBranchStatus("EF_xe30_loose_noMu") ) { fChain->SetBranchAddress("EF_xe30_loose_noMu", &EF_xe30_loose_noMu, &b_EF_xe30_loose_noMu); }
  if( fChain->GetBranchStatus("EF_xe30_medium") ) { fChain->SetBranchAddress("EF_xe30_medium", &EF_xe30_medium, &b_EF_xe30_medium); }
  if( fChain->GetBranchStatus("EF_xe30_medium_noMu") ) { fChain->SetBranchAddress("EF_xe30_medium_noMu", &EF_xe30_medium_noMu, &b_EF_xe30_medium_noMu); }
  if( fChain->GetBranchStatus("EF_xe30_medium_vfj_noMu") ) { fChain->SetBranchAddress("EF_xe30_medium_vfj_noMu", &EF_xe30_medium_vfj_noMu, &b_EF_xe30_medium_vfj_noMu); }
  if( fChain->GetBranchStatus("EF_xe30_noMu") ) { fChain->SetBranchAddress("EF_xe30_noMu", &EF_xe30_noMu, &b_EF_xe30_noMu); }
  if( fChain->GetBranchStatus("EF_xe30_tight_noMu") ) { fChain->SetBranchAddress("EF_xe30_tight_noMu", &EF_xe30_tight_noMu, &b_EF_xe30_tight_noMu); }
  if( fChain->GetBranchStatus("EF_xe30_tight_vfj_noMu") ) { fChain->SetBranchAddress("EF_xe30_tight_vfj_noMu", &EF_xe30_tight_vfj_noMu, &b_EF_xe30_tight_vfj_noMu); }
  if( fChain->GetBranchStatus("EF_xe30_vfj_noMu") ) { fChain->SetBranchAddress("EF_xe30_vfj_noMu", &EF_xe30_vfj_noMu, &b_EF_xe30_vfj_noMu); }
  if( fChain->GetBranchStatus("EF_xe35") ) { fChain->SetBranchAddress("EF_xe35", &EF_xe35, &b_EF_xe35); }
  if( fChain->GetBranchStatus("EF_xe35_loose_noMu") ) { fChain->SetBranchAddress("EF_xe35_loose_noMu", &EF_xe35_loose_noMu, &b_EF_xe35_loose_noMu); }
  if( fChain->GetBranchStatus("EF_xe35_noMu") ) { fChain->SetBranchAddress("EF_xe35_noMu", &EF_xe35_noMu, &b_EF_xe35_noMu); }
  if( fChain->GetBranchStatus("EF_xe35_tight_noMu") ) { fChain->SetBranchAddress("EF_xe35_tight_noMu", &EF_xe35_tight_noMu, &b_EF_xe35_tight_noMu); }
  if( fChain->GetBranchStatus("EF_xe40") ) { fChain->SetBranchAddress("EF_xe40", &EF_xe40, &b_EF_xe40); }
  if( fChain->GetBranchStatus("EF_xe40_noMu") ) { fChain->SetBranchAddress("EF_xe40_noMu", &EF_xe40_noMu, &b_EF_xe40_noMu); }
  if( fChain->GetBranchStatus("EF_xe40_tight_noMu") ) { fChain->SetBranchAddress("EF_xe40_tight_noMu", &EF_xe40_tight_noMu, &b_EF_xe40_tight_noMu); }
  if( fChain->GetBranchStatus("EF_xe45") ) { fChain->SetBranchAddress("EF_xe45", &EF_xe45, &b_EF_xe45); }
  if( fChain->GetBranchStatus("EF_xe45_noMu") ) { fChain->SetBranchAddress("EF_xe45_noMu", &EF_xe45_noMu, &b_EF_xe45_noMu); }
  if( fChain->GetBranchStatus("EF_xe55") ) { fChain->SetBranchAddress("EF_xe55", &EF_xe55, &b_EF_xe55); }
  if( fChain->GetBranchStatus("EF_xe55_noMu") ) { fChain->SetBranchAddress("EF_xe55_noMu", &EF_xe55_noMu, &b_EF_xe55_noMu); }
  if( fChain->GetBranchStatus("EF_xe60_medium") ) { fChain->SetBranchAddress("EF_xe60_medium", &EF_xe60_medium, &b_EF_xe60_medium); }
  if( fChain->GetBranchStatus("EF_xe60_medium_noMu") ) { fChain->SetBranchAddress("EF_xe60_medium_noMu", &EF_xe60_medium_noMu, &b_EF_xe60_medium_noMu); }
  if( fChain->GetBranchStatus("EF_xe80_medium") ) { fChain->SetBranchAddress("EF_xe80_medium", &EF_xe80_medium, &b_EF_xe80_medium); }
  if( fChain->GetBranchStatus("EF_xe80_medium_noMu") ) { fChain->SetBranchAddress("EF_xe80_medium_noMu", &EF_xe80_medium_noMu, &b_EF_xe80_medium_noMu); }
  if( fChain->GetBranchStatus("L1_2EM10") ) { fChain->SetBranchAddress("L1_2EM10", &L1_2EM10, &b_L1_2EM10); }
  if( fChain->GetBranchStatus("L1_2EM14") ) { fChain->SetBranchAddress("L1_2EM14", &L1_2EM14, &b_L1_2EM14); }
  if( fChain->GetBranchStatus("L1_2EM2") ) { fChain->SetBranchAddress("L1_2EM2", &L1_2EM2, &b_L1_2EM2); }
  if( fChain->GetBranchStatus("L1_2EM3") ) { fChain->SetBranchAddress("L1_2EM3", &L1_2EM3, &b_L1_2EM3); }
  if( fChain->GetBranchStatus("L1_2EM5") ) { fChain->SetBranchAddress("L1_2EM5", &L1_2EM5, &b_L1_2EM5); }
  if( fChain->GetBranchStatus("L1_2J15_XE10") ) { fChain->SetBranchAddress("L1_2J15_XE10", &L1_2J15_XE10, &b_L1_2J15_XE10); }
  if( fChain->GetBranchStatus("L1_2J15_XE15") ) { fChain->SetBranchAddress("L1_2J15_XE15", &L1_2J15_XE15, &b_L1_2J15_XE15); }
  if( fChain->GetBranchStatus("L1_2J15_XE25") ) { fChain->SetBranchAddress("L1_2J15_XE25", &L1_2J15_XE25, &b_L1_2J15_XE25); }
  if( fChain->GetBranchStatus("L1_2MU0") ) { fChain->SetBranchAddress("L1_2MU0", &L1_2MU0, &b_L1_2MU0); }
  if( fChain->GetBranchStatus("L1_2MU0_FIRSTEMPTY") ) { fChain->SetBranchAddress("L1_2MU0_FIRSTEMPTY", &L1_2MU0_FIRSTEMPTY, &b_L1_2MU0_FIRSTEMPTY); }
  if( fChain->GetBranchStatus("L1_2MU0_MU6") ) { fChain->SetBranchAddress("L1_2MU0_MU6", &L1_2MU0_MU6, &b_L1_2MU0_MU6); }
  if( fChain->GetBranchStatus("L1_2MU10") ) { fChain->SetBranchAddress("L1_2MU10", &L1_2MU10, &b_L1_2MU10); }
  if( fChain->GetBranchStatus("L1_2MU20") ) { fChain->SetBranchAddress("L1_2MU20", &L1_2MU20, &b_L1_2MU20); }
  if( fChain->GetBranchStatus("L1_2MU6") ) { fChain->SetBranchAddress("L1_2MU6", &L1_2MU6, &b_L1_2MU6); }
  if( fChain->GetBranchStatus("L1_2MU6_EM5") ) { fChain->SetBranchAddress("L1_2MU6_EM5", &L1_2MU6_EM5, &b_L1_2MU6_EM5); }
  if( fChain->GetBranchStatus("L1_2TAU11") ) { fChain->SetBranchAddress("L1_2TAU11", &L1_2TAU11, &b_L1_2TAU11); }
  if( fChain->GetBranchStatus("L1_2TAU5") ) { fChain->SetBranchAddress("L1_2TAU5", &L1_2TAU5, &b_L1_2TAU5); }
  if( fChain->GetBranchStatus("L1_2TAU5_EM5") ) { fChain->SetBranchAddress("L1_2TAU5_EM5", &L1_2TAU5_EM5, &b_L1_2TAU5_EM5); }
  if( fChain->GetBranchStatus("L1_2TAU6") ) { fChain->SetBranchAddress("L1_2TAU6", &L1_2TAU6, &b_L1_2TAU6); }
  if( fChain->GetBranchStatus("L1_2TAU6_EM10") ) { fChain->SetBranchAddress("L1_2TAU6_EM10", &L1_2TAU6_EM10, &b_L1_2TAU6_EM10); }
  if( fChain->GetBranchStatus("L1_2TAU6_EM5") ) { fChain->SetBranchAddress("L1_2TAU6_EM5", &L1_2TAU6_EM5, &b_L1_2TAU6_EM5); }
  if( fChain->GetBranchStatus("L1_EM10") ) { fChain->SetBranchAddress("L1_EM10", &L1_EM10, &b_L1_EM10); }
  if( fChain->GetBranchStatus("L1_EM10I") ) { fChain->SetBranchAddress("L1_EM10I", &L1_EM10I, &b_L1_EM10I); }
  if( fChain->GetBranchStatus("L1_EM14") ) { fChain->SetBranchAddress("L1_EM14", &L1_EM14, &b_L1_EM14); }
  if( fChain->GetBranchStatus("L1_EM14I") ) { fChain->SetBranchAddress("L1_EM14I", &L1_EM14I, &b_L1_EM14I); }
  if( fChain->GetBranchStatus("L1_EM14_XE10") ) { fChain->SetBranchAddress("L1_EM14_XE10", &L1_EM14_XE10, &b_L1_EM14_XE10); }
  if( fChain->GetBranchStatus("L1_EM14_XE15") ) { fChain->SetBranchAddress("L1_EM14_XE15", &L1_EM14_XE15, &b_L1_EM14_XE15); }
  if( fChain->GetBranchStatus("L1_EM2") ) { fChain->SetBranchAddress("L1_EM2", &L1_EM2, &b_L1_EM2); }
  if( fChain->GetBranchStatus("L1_EM2_UNPAIRED_ISO") ) { fChain->SetBranchAddress("L1_EM2_UNPAIRED_ISO", &L1_EM2_UNPAIRED_ISO, &b_L1_EM2_UNPAIRED_ISO); }
  if( fChain->GetBranchStatus("L1_EM2_UNPAIRED_NONISO") ) { fChain->SetBranchAddress("L1_EM2_UNPAIRED_NONISO", &L1_EM2_UNPAIRED_NONISO, &b_L1_EM2_UNPAIRED_NONISO); }
  if( fChain->GetBranchStatus("L1_EM3") ) { fChain->SetBranchAddress("L1_EM3", &L1_EM3, &b_L1_EM3); }
  if( fChain->GetBranchStatus("L1_EM3_EMPTY") ) { fChain->SetBranchAddress("L1_EM3_EMPTY", &L1_EM3_EMPTY, &b_L1_EM3_EMPTY); }
  if( fChain->GetBranchStatus("L1_EM3_FIRSTEMPTY") ) { fChain->SetBranchAddress("L1_EM3_FIRSTEMPTY", &L1_EM3_FIRSTEMPTY, &b_L1_EM3_FIRSTEMPTY); }
  if( fChain->GetBranchStatus("L1_EM3_MV") ) { fChain->SetBranchAddress("L1_EM3_MV", &L1_EM3_MV, &b_L1_EM3_MV); }
  if( fChain->GetBranchStatus("L1_EM5") ) { fChain->SetBranchAddress("L1_EM5", &L1_EM5, &b_L1_EM5); }
  if( fChain->GetBranchStatus("L1_EM5_MU10") ) { fChain->SetBranchAddress("L1_EM5_MU10", &L1_EM5_MU10, &b_L1_EM5_MU10); }
  if( fChain->GetBranchStatus("L1_EM5_MU6") ) { fChain->SetBranchAddress("L1_EM5_MU6", &L1_EM5_MU6, &b_L1_EM5_MU6); }
  if( fChain->GetBranchStatus("L1_EM85") ) { fChain->SetBranchAddress("L1_EM85", &L1_EM85, &b_L1_EM85); }
  if( fChain->GetBranchStatus("L1_J15_XE15_EM10") ) { fChain->SetBranchAddress("L1_J15_XE15_EM10", &L1_J15_XE15_EM10, &b_L1_J15_XE15_EM10); }
  if( fChain->GetBranchStatus("L1_J15_XE15_MU15") ) { fChain->SetBranchAddress("L1_J15_XE15_MU15", &L1_J15_XE15_MU15, &b_L1_J15_XE15_MU15); }
  if( fChain->GetBranchStatus("L1_J30_XE10") ) { fChain->SetBranchAddress("L1_J30_XE10", &L1_J30_XE10, &b_L1_J30_XE10); }
  if( fChain->GetBranchStatus("L1_J30_XE15") ) { fChain->SetBranchAddress("L1_J30_XE15", &L1_J30_XE15, &b_L1_J30_XE15); }
  if( fChain->GetBranchStatus("L1_J30_XE25") ) { fChain->SetBranchAddress("L1_J30_XE25", &L1_J30_XE25, &b_L1_J30_XE25); }
  if( fChain->GetBranchStatus("L1_MU0") ) { fChain->SetBranchAddress("L1_MU0", &L1_MU0, &b_L1_MU0); }
  if( fChain->GetBranchStatus("L1_MU0_COMM") ) { fChain->SetBranchAddress("L1_MU0_COMM", &L1_MU0_COMM, &b_L1_MU0_COMM); }
  if( fChain->GetBranchStatus("L1_MU0_COMM_EMPTY") ) { fChain->SetBranchAddress("L1_MU0_COMM_EMPTY", &L1_MU0_COMM_EMPTY, &b_L1_MU0_COMM_EMPTY); }
  if( fChain->GetBranchStatus("L1_MU0_COMM_FIRSTEMPTY") ) { fChain->SetBranchAddress("L1_MU0_COMM_FIRSTEMPTY", &L1_MU0_COMM_FIRSTEMPTY, &b_L1_MU0_COMM_FIRSTEMPTY); }
  if( fChain->GetBranchStatus("L1_MU0_COMM_UNPAIRED_ISO") ) { fChain->SetBranchAddress("L1_MU0_COMM_UNPAIRED_ISO", &L1_MU0_COMM_UNPAIRED_ISO, &b_L1_MU0_COMM_UNPAIRED_ISO); }
  if( fChain->GetBranchStatus("L1_MU0_COMM_UNPAIRED_NONISO") ) { fChain->SetBranchAddress("L1_MU0_COMM_UNPAIRED_NONISO", &L1_MU0_COMM_UNPAIRED_NONISO, &b_L1_MU0_COMM_UNPAIRED_NONISO); }
  if( fChain->GetBranchStatus("L1_MU0_EM3") ) { fChain->SetBranchAddress("L1_MU0_EM3", &L1_MU0_EM3, &b_L1_MU0_EM3); }
  if( fChain->GetBranchStatus("L1_MU0_EMPTY") ) { fChain->SetBranchAddress("L1_MU0_EMPTY", &L1_MU0_EMPTY, &b_L1_MU0_EMPTY); }
  if( fChain->GetBranchStatus("L1_MU0_FIRSTEMPTY") ) { fChain->SetBranchAddress("L1_MU0_FIRSTEMPTY", &L1_MU0_FIRSTEMPTY, &b_L1_MU0_FIRSTEMPTY); }
  if( fChain->GetBranchStatus("L1_MU0_J10") ) { fChain->SetBranchAddress("L1_MU0_J10", &L1_MU0_J10, &b_L1_MU0_J10); }
  if( fChain->GetBranchStatus("L1_MU0_J15") ) { fChain->SetBranchAddress("L1_MU0_J15", &L1_MU0_J15, &b_L1_MU0_J15); }
  if( fChain->GetBranchStatus("L1_MU0_J30") ) { fChain->SetBranchAddress("L1_MU0_J30", &L1_MU0_J30, &b_L1_MU0_J30); }
  if( fChain->GetBranchStatus("L1_MU0_J5") ) { fChain->SetBranchAddress("L1_MU0_J5", &L1_MU0_J5, &b_L1_MU0_J5); }
  if( fChain->GetBranchStatus("L1_MU0_J55") ) { fChain->SetBranchAddress("L1_MU0_J55", &L1_MU0_J55, &b_L1_MU0_J55); }
  if( fChain->GetBranchStatus("L1_MU0_MV") ) { fChain->SetBranchAddress("L1_MU0_MV", &L1_MU0_MV, &b_L1_MU0_MV); }
  if( fChain->GetBranchStatus("L1_MU0_UNPAIRED_ISO") ) { fChain->SetBranchAddress("L1_MU0_UNPAIRED_ISO", &L1_MU0_UNPAIRED_ISO, &b_L1_MU0_UNPAIRED_ISO); }
  if( fChain->GetBranchStatus("L1_MU0_UNPAIRED_NONISO") ) { fChain->SetBranchAddress("L1_MU0_UNPAIRED_NONISO", &L1_MU0_UNPAIRED_NONISO, &b_L1_MU0_UNPAIRED_NONISO); }
  if( fChain->GetBranchStatus("L1_MU10") ) { fChain->SetBranchAddress("L1_MU10", &L1_MU10, &b_L1_MU10); }
  if( fChain->GetBranchStatus("L1_MU10_FIRSTEMPTY") ) { fChain->SetBranchAddress("L1_MU10_FIRSTEMPTY", &L1_MU10_FIRSTEMPTY, &b_L1_MU10_FIRSTEMPTY); }
  if( fChain->GetBranchStatus("L1_MU10_J10") ) { fChain->SetBranchAddress("L1_MU10_J10", &L1_MU10_J10, &b_L1_MU10_J10); }
  if( fChain->GetBranchStatus("L1_MU15") ) { fChain->SetBranchAddress("L1_MU15", &L1_MU15, &b_L1_MU15); }
  if( fChain->GetBranchStatus("L1_MU20") ) { fChain->SetBranchAddress("L1_MU20", &L1_MU20, &b_L1_MU20); }
  if( fChain->GetBranchStatus("L1_MU6") ) { fChain->SetBranchAddress("L1_MU6", &L1_MU6, &b_L1_MU6); }
  if( fChain->GetBranchStatus("L1_MU6_EM3") ) { fChain->SetBranchAddress("L1_MU6_EM3", &L1_MU6_EM3, &b_L1_MU6_EM3); }
  if( fChain->GetBranchStatus("L1_MU6_FIRSTEMPTY") ) { fChain->SetBranchAddress("L1_MU6_FIRSTEMPTY", &L1_MU6_FIRSTEMPTY, &b_L1_MU6_FIRSTEMPTY); }
  if( fChain->GetBranchStatus("L1_MU6_J5") ) { fChain->SetBranchAddress("L1_MU6_J5", &L1_MU6_J5, &b_L1_MU6_J5); }
  if( fChain->GetBranchStatus("L1_TAU11") ) { fChain->SetBranchAddress("L1_TAU11", &L1_TAU11, &b_L1_TAU11); }
  if( fChain->GetBranchStatus("L1_TAU11I") ) { fChain->SetBranchAddress("L1_TAU11I", &L1_TAU11I, &b_L1_TAU11I); }
  if( fChain->GetBranchStatus("L1_TAU20") ) { fChain->SetBranchAddress("L1_TAU20", &L1_TAU20, &b_L1_TAU20); }
  if( fChain->GetBranchStatus("L1_TAU30") ) { fChain->SetBranchAddress("L1_TAU30", &L1_TAU30, &b_L1_TAU30); }
  if( fChain->GetBranchStatus("L1_TAU5") ) { fChain->SetBranchAddress("L1_TAU5", &L1_TAU5, &b_L1_TAU5); }
  if( fChain->GetBranchStatus("L1_TAU50") ) { fChain->SetBranchAddress("L1_TAU50", &L1_TAU50, &b_L1_TAU50); }
  if( fChain->GetBranchStatus("L1_TAU5_3J5_2J15") ) { fChain->SetBranchAddress("L1_TAU5_3J5_2J15", &L1_TAU5_3J5_2J15, &b_L1_TAU5_3J5_2J15); }
  if( fChain->GetBranchStatus("L1_TAU5_4J5_3J15") ) { fChain->SetBranchAddress("L1_TAU5_4J5_3J15", &L1_TAU5_4J5_3J15, &b_L1_TAU5_4J5_3J15); }
  if( fChain->GetBranchStatus("L1_TAU5_EMPTY") ) { fChain->SetBranchAddress("L1_TAU5_EMPTY", &L1_TAU5_EMPTY, &b_L1_TAU5_EMPTY); }
  if( fChain->GetBranchStatus("L1_TAU5_FIRSTEMPTY") ) { fChain->SetBranchAddress("L1_TAU5_FIRSTEMPTY", &L1_TAU5_FIRSTEMPTY, &b_L1_TAU5_FIRSTEMPTY); }
  if( fChain->GetBranchStatus("L1_TAU5_MU6") ) { fChain->SetBranchAddress("L1_TAU5_MU6", &L1_TAU5_MU6, &b_L1_TAU5_MU6); }
  if( fChain->GetBranchStatus("L1_TAU5_MV") ) { fChain->SetBranchAddress("L1_TAU5_MV", &L1_TAU5_MV, &b_L1_TAU5_MV); }
  if( fChain->GetBranchStatus("L1_TAU5_UNPAIRED_ISO") ) { fChain->SetBranchAddress("L1_TAU5_UNPAIRED_ISO", &L1_TAU5_UNPAIRED_ISO, &b_L1_TAU5_UNPAIRED_ISO); }
  if( fChain->GetBranchStatus("L1_TAU5_UNPAIRED_NONISO") ) { fChain->SetBranchAddress("L1_TAU5_UNPAIRED_NONISO", &L1_TAU5_UNPAIRED_NONISO, &b_L1_TAU5_UNPAIRED_NONISO); }
  if( fChain->GetBranchStatus("L1_TAU5_XE10") ) { fChain->SetBranchAddress("L1_TAU5_XE10", &L1_TAU5_XE10, &b_L1_TAU5_XE10); }
  if( fChain->GetBranchStatus("L1_TAU6") ) { fChain->SetBranchAddress("L1_TAU6", &L1_TAU6, &b_L1_TAU6); }
  if( fChain->GetBranchStatus("L1_TAU6I") ) { fChain->SetBranchAddress("L1_TAU6I", &L1_TAU6I, &b_L1_TAU6I); }
  if( fChain->GetBranchStatus("L1_TAU6_3J5_2J15") ) { fChain->SetBranchAddress("L1_TAU6_3J5_2J15", &L1_TAU6_3J5_2J15, &b_L1_TAU6_3J5_2J15); }
  if( fChain->GetBranchStatus("L1_TAU6_4J5_3J15") ) { fChain->SetBranchAddress("L1_TAU6_4J5_3J15", &L1_TAU6_4J5_3J15, &b_L1_TAU6_4J5_3J15); }
  if( fChain->GetBranchStatus("L1_TAU6_MU10") ) { fChain->SetBranchAddress("L1_TAU6_MU10", &L1_TAU6_MU10, &b_L1_TAU6_MU10); }
  if( fChain->GetBranchStatus("L1_TAU6_MU15") ) { fChain->SetBranchAddress("L1_TAU6_MU15", &L1_TAU6_MU15, &b_L1_TAU6_MU15); }
  if( fChain->GetBranchStatus("L1_TAU6_XE10") ) { fChain->SetBranchAddress("L1_TAU6_XE10", &L1_TAU6_XE10, &b_L1_TAU6_XE10); }
  if( fChain->GetBranchStatus("L1_XE10") ) { fChain->SetBranchAddress("L1_XE10", &L1_XE10, &b_L1_XE10); }
  if( fChain->GetBranchStatus("L1_XE15") ) { fChain->SetBranchAddress("L1_XE15", &L1_XE15, &b_L1_XE15); }
  if( fChain->GetBranchStatus("L1_XE20") ) { fChain->SetBranchAddress("L1_XE20", &L1_XE20, &b_L1_XE20); }
  if( fChain->GetBranchStatus("L1_XE25") ) { fChain->SetBranchAddress("L1_XE25", &L1_XE25, &b_L1_XE25); }
  if( fChain->GetBranchStatus("L1_XE30") ) { fChain->SetBranchAddress("L1_XE30", &L1_XE30, &b_L1_XE30); }
  if( fChain->GetBranchStatus("L1_XE35") ) { fChain->SetBranchAddress("L1_XE35", &L1_XE35, &b_L1_XE35); }
  if( fChain->GetBranchStatus("L1_XE40") ) { fChain->SetBranchAddress("L1_XE40", &L1_XE40, &b_L1_XE40); }
  if( fChain->GetBranchStatus("L1_XE50") ) { fChain->SetBranchAddress("L1_XE50", &L1_XE50, &b_L1_XE50); }
  if( fChain->GetBranchStatus("L2_2e10_loose") ) { fChain->SetBranchAddress("L2_2e10_loose", &L2_2e10_loose, &b_L2_2e10_loose); }
  if( fChain->GetBranchStatus("L2_2e10_medium") ) { fChain->SetBranchAddress("L2_2e10_medium", &L2_2e10_medium, &b_L2_2e10_medium); }
  if( fChain->GetBranchStatus("L2_2e15_loose") ) { fChain->SetBranchAddress("L2_2e15_loose", &L2_2e15_loose, &b_L2_2e15_loose); }
  if( fChain->GetBranchStatus("L2_2e3_loose") ) { fChain->SetBranchAddress("L2_2e3_loose", &L2_2e3_loose, &b_L2_2e3_loose); }
  if( fChain->GetBranchStatus("L2_2e3_loose_SiTrk") ) { fChain->SetBranchAddress("L2_2e3_loose_SiTrk", &L2_2e3_loose_SiTrk, &b_L2_2e3_loose_SiTrk); }
  if( fChain->GetBranchStatus("L2_2e3_loose_TRT") ) { fChain->SetBranchAddress("L2_2e3_loose_TRT", &L2_2e3_loose_TRT, &b_L2_2e3_loose_TRT); }
  if( fChain->GetBranchStatus("L2_2e3_medium") ) { fChain->SetBranchAddress("L2_2e3_medium", &L2_2e3_medium, &b_L2_2e3_medium); }
  if( fChain->GetBranchStatus("L2_2e3_medium_SiTrk") ) { fChain->SetBranchAddress("L2_2e3_medium_SiTrk", &L2_2e3_medium_SiTrk, &b_L2_2e3_medium_SiTrk); }
  if( fChain->GetBranchStatus("L2_2e3_medium_TRT") ) { fChain->SetBranchAddress("L2_2e3_medium_TRT", &L2_2e3_medium_TRT, &b_L2_2e3_medium_TRT); }
  if( fChain->GetBranchStatus("L2_2e3_tight") ) { fChain->SetBranchAddress("L2_2e3_tight", &L2_2e3_tight, &b_L2_2e3_tight); }
  if( fChain->GetBranchStatus("L2_2e5_medium") ) { fChain->SetBranchAddress("L2_2e5_medium", &L2_2e5_medium, &b_L2_2e5_medium); }
  if( fChain->GetBranchStatus("L2_2e5_medium_SiTrk") ) { fChain->SetBranchAddress("L2_2e5_medium_SiTrk", &L2_2e5_medium_SiTrk, &b_L2_2e5_medium_SiTrk); }
  if( fChain->GetBranchStatus("L2_2e5_medium_TRT") ) { fChain->SetBranchAddress("L2_2e5_medium_TRT", &L2_2e5_medium_TRT, &b_L2_2e5_medium_TRT); }
  if( fChain->GetBranchStatus("L2_2e5_tight") ) { fChain->SetBranchAddress("L2_2e5_tight", &L2_2e5_tight, &b_L2_2e5_tight); }
  if( fChain->GetBranchStatus("L2_2g10_loose") ) { fChain->SetBranchAddress("L2_2g10_loose", &L2_2g10_loose, &b_L2_2g10_loose); }
  if( fChain->GetBranchStatus("L2_2g15_loose") ) { fChain->SetBranchAddress("L2_2g15_loose", &L2_2g15_loose, &b_L2_2g15_loose); }
  if( fChain->GetBranchStatus("L2_2g20_loose") ) { fChain->SetBranchAddress("L2_2g20_loose", &L2_2g20_loose, &b_L2_2g20_loose); }
  if( fChain->GetBranchStatus("L2_2g5_loose") ) { fChain->SetBranchAddress("L2_2g5_loose", &L2_2g5_loose, &b_L2_2g5_loose); }
  if( fChain->GetBranchStatus("L2_2g7_loose") ) { fChain->SetBranchAddress("L2_2g7_loose", &L2_2g7_loose, &b_L2_2g7_loose); }
  if( fChain->GetBranchStatus("L2_2j30_xe12_noMu") ) { fChain->SetBranchAddress("L2_2j30_xe12_noMu", &L2_2j30_xe12_noMu, &b_L2_2j30_xe12_noMu); }
  if( fChain->GetBranchStatus("L2_2j30_xe20_noMu") ) { fChain->SetBranchAddress("L2_2j30_xe20_noMu", &L2_2j30_xe20_noMu, &b_L2_2j30_xe20_noMu); }
  if( fChain->GetBranchStatus("L2_2j30_xe30_noMu") ) { fChain->SetBranchAddress("L2_2j30_xe30_noMu", &L2_2j30_xe30_noMu, &b_L2_2j30_xe30_noMu); }
  if( fChain->GetBranchStatus("L2_2j35_jetNoEF_xe20_noMu") ) { fChain->SetBranchAddress("L2_2j35_jetNoEF_xe20_noMu", &L2_2j35_jetNoEF_xe20_noMu, &b_L2_2j35_jetNoEF_xe20_noMu); }
  if( fChain->GetBranchStatus("L2_2j35_jetNoEF_xe30_noMu") ) { fChain->SetBranchAddress("L2_2j35_jetNoEF_xe30_noMu", &L2_2j35_jetNoEF_xe30_noMu, &b_L2_2j35_jetNoEF_xe30_noMu); }
  if( fChain->GetBranchStatus("L2_2j35_jetNoEF_xe40_noMu") ) { fChain->SetBranchAddress("L2_2j35_jetNoEF_xe40_noMu", &L2_2j35_jetNoEF_xe40_noMu, &b_L2_2j35_jetNoEF_xe40_noMu); }
  if( fChain->GetBranchStatus("L2_2mu0_NoAlg") ) { fChain->SetBranchAddress("L2_2mu0_NoAlg", &L2_2mu0_NoAlg, &b_L2_2mu0_NoAlg); }
  if( fChain->GetBranchStatus("L2_2mu10") ) { fChain->SetBranchAddress("L2_2mu10", &L2_2mu10, &b_L2_2mu10); }
  if( fChain->GetBranchStatus("L2_2mu10_NoAlg") ) { fChain->SetBranchAddress("L2_2mu10_NoAlg", &L2_2mu10_NoAlg, &b_L2_2mu10_NoAlg); }
  if( fChain->GetBranchStatus("L2_2mu4") ) { fChain->SetBranchAddress("L2_2mu4", &L2_2mu4, &b_L2_2mu4); }
  if( fChain->GetBranchStatus("L2_2mu4_Bmumu") ) { fChain->SetBranchAddress("L2_2mu4_Bmumu", &L2_2mu4_Bmumu, &b_L2_2mu4_Bmumu); }
  if( fChain->GetBranchStatus("L2_2mu4_Bmumux") ) { fChain->SetBranchAddress("L2_2mu4_Bmumux", &L2_2mu4_Bmumux, &b_L2_2mu4_Bmumux); }
  if( fChain->GetBranchStatus("L2_2mu4_DiMu") ) { fChain->SetBranchAddress("L2_2mu4_DiMu", &L2_2mu4_DiMu, &b_L2_2mu4_DiMu); }
  if( fChain->GetBranchStatus("L2_2mu4_DiMu_SiTrk") ) { fChain->SetBranchAddress("L2_2mu4_DiMu_SiTrk", &L2_2mu4_DiMu_SiTrk, &b_L2_2mu4_DiMu_SiTrk); }
  if( fChain->GetBranchStatus("L2_2mu4_DiMu_noVtx") ) { fChain->SetBranchAddress("L2_2mu4_DiMu_noVtx", &L2_2mu4_DiMu_noVtx, &b_L2_2mu4_DiMu_noVtx); }
  if( fChain->GetBranchStatus("L2_2mu4_DiMu_noVtx_noOS") ) { fChain->SetBranchAddress("L2_2mu4_DiMu_noVtx_noOS", &L2_2mu4_DiMu_noVtx_noOS, &b_L2_2mu4_DiMu_noVtx_noOS); }
  if( fChain->GetBranchStatus("L2_2mu4_Jpsimumu") ) { fChain->SetBranchAddress("L2_2mu4_Jpsimumu", &L2_2mu4_Jpsimumu, &b_L2_2mu4_Jpsimumu); }
  if( fChain->GetBranchStatus("L2_2mu4_Jpsimumu_IDTrkNoCut") ) { fChain->SetBranchAddress("L2_2mu4_Jpsimumu_IDTrkNoCut", &L2_2mu4_Jpsimumu_IDTrkNoCut, &b_L2_2mu4_Jpsimumu_IDTrkNoCut); }
  if( fChain->GetBranchStatus("L2_2mu4_Upsimumu") ) { fChain->SetBranchAddress("L2_2mu4_Upsimumu", &L2_2mu4_Upsimumu, &b_L2_2mu4_Upsimumu); }
  if( fChain->GetBranchStatus("L2_2mu6") ) { fChain->SetBranchAddress("L2_2mu6", &L2_2mu6, &b_L2_2mu6); }
  if( fChain->GetBranchStatus("L2_2mu6_Bmumu") ) { fChain->SetBranchAddress("L2_2mu6_Bmumu", &L2_2mu6_Bmumu, &b_L2_2mu6_Bmumu); }
  if( fChain->GetBranchStatus("L2_2mu6_Bmumux") ) { fChain->SetBranchAddress("L2_2mu6_Bmumux", &L2_2mu6_Bmumux, &b_L2_2mu6_Bmumux); }
  if( fChain->GetBranchStatus("L2_2mu6_DiMu") ) { fChain->SetBranchAddress("L2_2mu6_DiMu", &L2_2mu6_DiMu, &b_L2_2mu6_DiMu); }
  if( fChain->GetBranchStatus("L2_2mu6_Jpsimumu") ) { fChain->SetBranchAddress("L2_2mu6_Jpsimumu", &L2_2mu6_Jpsimumu, &b_L2_2mu6_Jpsimumu); }
  if( fChain->GetBranchStatus("L2_2mu6_MG") ) { fChain->SetBranchAddress("L2_2mu6_MG", &L2_2mu6_MG, &b_L2_2mu6_MG); }
  if( fChain->GetBranchStatus("L2_2mu6_NoAlg") ) { fChain->SetBranchAddress("L2_2mu6_NoAlg", &L2_2mu6_NoAlg, &b_L2_2mu6_NoAlg); }
  if( fChain->GetBranchStatus("L2_2mu6_Upsimumu") ) { fChain->SetBranchAddress("L2_2mu6_Upsimumu", &L2_2mu6_Upsimumu, &b_L2_2mu6_Upsimumu); }
  if( fChain->GetBranchStatus("L2_2mu6_g10_loose") ) { fChain->SetBranchAddress("L2_2mu6_g10_loose", &L2_2mu6_g10_loose, &b_L2_2mu6_g10_loose); }
  if( fChain->GetBranchStatus("L2_2tau12_loose") ) { fChain->SetBranchAddress("L2_2tau12_loose", &L2_2tau12_loose, &b_L2_2tau12_loose); }
  if( fChain->GetBranchStatus("L2_2tau16_loose") ) { fChain->SetBranchAddress("L2_2tau16_loose", &L2_2tau16_loose, &b_L2_2tau16_loose); }
  if( fChain->GetBranchStatus("L2_2tau20_loose") ) { fChain->SetBranchAddress("L2_2tau20_loose", &L2_2tau20_loose, &b_L2_2tau20_loose); }
  if( fChain->GetBranchStatus("L2_2tau29_loose") ) { fChain->SetBranchAddress("L2_2tau29_loose", &L2_2tau29_loose, &b_L2_2tau29_loose); }
  if( fChain->GetBranchStatus("L2_2tau29_loose1") ) { fChain->SetBranchAddress("L2_2tau29_loose1", &L2_2tau29_loose1, &b_L2_2tau29_loose1); }
  if( fChain->GetBranchStatus("L2_2tau38_loose") ) { fChain->SetBranchAddress("L2_2tau38_loose", &L2_2tau38_loose, &b_L2_2tau38_loose); }
  if( fChain->GetBranchStatus("L2_e10_NoCut") ) { fChain->SetBranchAddress("L2_e10_NoCut", &L2_e10_NoCut, &b_L2_e10_NoCut); }
  if( fChain->GetBranchStatus("L2_e10_loose") ) { fChain->SetBranchAddress("L2_e10_loose", &L2_e10_loose, &b_L2_e10_loose); }
  if( fChain->GetBranchStatus("L2_e10_loose_mu10") ) { fChain->SetBranchAddress("L2_e10_loose_mu10", &L2_e10_loose_mu10, &b_L2_e10_loose_mu10); }
  if( fChain->GetBranchStatus("L2_e10_loose_mu6") ) { fChain->SetBranchAddress("L2_e10_loose_mu6", &L2_e10_loose_mu6, &b_L2_e10_loose_mu6); }
  if( fChain->GetBranchStatus("L2_e10_medium") ) { fChain->SetBranchAddress("L2_e10_medium", &L2_e10_medium, &b_L2_e10_medium); }
  if( fChain->GetBranchStatus("L2_e10_medium_IDTrkNoCut") ) { fChain->SetBranchAddress("L2_e10_medium_IDTrkNoCut", &L2_e10_medium_IDTrkNoCut, &b_L2_e10_medium_IDTrkNoCut); }
  if( fChain->GetBranchStatus("L2_e10_medium_SiTrk") ) { fChain->SetBranchAddress("L2_e10_medium_SiTrk", &L2_e10_medium_SiTrk, &b_L2_e10_medium_SiTrk); }
  if( fChain->GetBranchStatus("L2_e10_medium_TRT") ) { fChain->SetBranchAddress("L2_e10_medium_TRT", &L2_e10_medium_TRT, &b_L2_e10_medium_TRT); }
  if( fChain->GetBranchStatus("L2_e10_tight") ) { fChain->SetBranchAddress("L2_e10_tight", &L2_e10_tight, &b_L2_e10_tight); }
  if( fChain->GetBranchStatus("L2_e10_tight_TRT") ) { fChain->SetBranchAddress("L2_e10_tight_TRT", &L2_e10_tight_TRT, &b_L2_e10_tight_TRT); }
  if( fChain->GetBranchStatus("L2_e15_loose") ) { fChain->SetBranchAddress("L2_e15_loose", &L2_e15_loose, &b_L2_e15_loose); }
  if( fChain->GetBranchStatus("L2_e15_loose_IDTrkNoCut") ) { fChain->SetBranchAddress("L2_e15_loose_IDTrkNoCut", &L2_e15_loose_IDTrkNoCut, &b_L2_e15_loose_IDTrkNoCut); }
  if( fChain->GetBranchStatus("L2_e15_medium") ) { fChain->SetBranchAddress("L2_e15_medium", &L2_e15_medium, &b_L2_e15_medium); }
  if( fChain->GetBranchStatus("L2_e15_medium_SiTrk") ) { fChain->SetBranchAddress("L2_e15_medium_SiTrk", &L2_e15_medium_SiTrk, &b_L2_e15_medium_SiTrk); }
  if( fChain->GetBranchStatus("L2_e15_medium_TRT") ) { fChain->SetBranchAddress("L2_e15_medium_TRT", &L2_e15_medium_TRT, &b_L2_e15_medium_TRT); }
  if( fChain->GetBranchStatus("L2_e15_tight") ) { fChain->SetBranchAddress("L2_e15_tight", &L2_e15_tight, &b_L2_e15_tight); }
  if( fChain->GetBranchStatus("L2_e15_tight_TRT") ) { fChain->SetBranchAddress("L2_e15_tight_TRT", &L2_e15_tight_TRT, &b_L2_e15_tight_TRT); }
  if( fChain->GetBranchStatus("L2_e18_medium") ) { fChain->SetBranchAddress("L2_e18_medium", &L2_e18_medium, &b_L2_e18_medium); }
  if( fChain->GetBranchStatus("L2_e20_loose") ) { fChain->SetBranchAddress("L2_e20_loose", &L2_e20_loose, &b_L2_e20_loose); }
  if( fChain->GetBranchStatus("L2_e20_loose_IDTrkNoCut") ) { fChain->SetBranchAddress("L2_e20_loose_IDTrkNoCut", &L2_e20_loose_IDTrkNoCut, &b_L2_e20_loose_IDTrkNoCut); }
  if( fChain->GetBranchStatus("L2_e20_loose_TRT") ) { fChain->SetBranchAddress("L2_e20_loose_TRT", &L2_e20_loose_TRT, &b_L2_e20_loose_TRT); }
  if( fChain->GetBranchStatus("L2_e20_loose_passEF") ) { fChain->SetBranchAddress("L2_e20_loose_passEF", &L2_e20_loose_passEF, &b_L2_e20_loose_passEF); }
  if( fChain->GetBranchStatus("L2_e20_loose_passL2") ) { fChain->SetBranchAddress("L2_e20_loose_passL2", &L2_e20_loose_passL2, &b_L2_e20_loose_passL2); }
  if( fChain->GetBranchStatus("L2_e20_loose_xe20") ) { fChain->SetBranchAddress("L2_e20_loose_xe20", &L2_e20_loose_xe20, &b_L2_e20_loose_xe20); }
  if( fChain->GetBranchStatus("L2_e20_loose_xe20_noMu") ) { fChain->SetBranchAddress("L2_e20_loose_xe20_noMu", &L2_e20_loose_xe20_noMu, &b_L2_e20_loose_xe20_noMu); }
  if( fChain->GetBranchStatus("L2_e20_loose_xe30") ) { fChain->SetBranchAddress("L2_e20_loose_xe30", &L2_e20_loose_xe30, &b_L2_e20_loose_xe30); }
  if( fChain->GetBranchStatus("L2_e20_loose_xe30_noMu") ) { fChain->SetBranchAddress("L2_e20_loose_xe30_noMu", &L2_e20_loose_xe30_noMu, &b_L2_e20_loose_xe30_noMu); }
  if( fChain->GetBranchStatus("L2_e20_medium") ) { fChain->SetBranchAddress("L2_e20_medium", &L2_e20_medium, &b_L2_e20_medium); }
  if( fChain->GetBranchStatus("L2_e25_loose") ) { fChain->SetBranchAddress("L2_e25_loose", &L2_e25_loose, &b_L2_e25_loose); }
  if( fChain->GetBranchStatus("L2_e25_medium") ) { fChain->SetBranchAddress("L2_e25_medium", &L2_e25_medium, &b_L2_e25_medium); }
  if( fChain->GetBranchStatus("L2_e30_loose") ) { fChain->SetBranchAddress("L2_e30_loose", &L2_e30_loose, &b_L2_e30_loose); }
  if( fChain->GetBranchStatus("L2_e3_loose") ) { fChain->SetBranchAddress("L2_e3_loose", &L2_e3_loose, &b_L2_e3_loose); }
  if( fChain->GetBranchStatus("L2_e3_loose_SiTrk") ) { fChain->SetBranchAddress("L2_e3_loose_SiTrk", &L2_e3_loose_SiTrk, &b_L2_e3_loose_SiTrk); }
  if( fChain->GetBranchStatus("L2_e3_loose_TRT") ) { fChain->SetBranchAddress("L2_e3_loose_TRT", &L2_e3_loose_TRT, &b_L2_e3_loose_TRT); }
  if( fChain->GetBranchStatus("L2_e3_medium") ) { fChain->SetBranchAddress("L2_e3_medium", &L2_e3_medium, &b_L2_e3_medium); }
  if( fChain->GetBranchStatus("L2_e3_medium_SiTrk") ) { fChain->SetBranchAddress("L2_e3_medium_SiTrk", &L2_e3_medium_SiTrk, &b_L2_e3_medium_SiTrk); }
  if( fChain->GetBranchStatus("L2_e3_medium_TRT") ) { fChain->SetBranchAddress("L2_e3_medium_TRT", &L2_e3_medium_TRT, &b_L2_e3_medium_TRT); }
  if( fChain->GetBranchStatus("L2_e5_NoCut_L2SW") ) { fChain->SetBranchAddress("L2_e5_NoCut_L2SW", &L2_e5_NoCut_L2SW, &b_L2_e5_NoCut_L2SW); }
  if( fChain->GetBranchStatus("L2_e5_NoCut_Ringer") ) { fChain->SetBranchAddress("L2_e5_NoCut_Ringer", &L2_e5_NoCut_Ringer, &b_L2_e5_NoCut_Ringer); }
  if( fChain->GetBranchStatus("L2_e5_NoCut_firstempty") ) { fChain->SetBranchAddress("L2_e5_NoCut_firstempty", &L2_e5_NoCut_firstempty, &b_L2_e5_NoCut_firstempty); }
  if( fChain->GetBranchStatus("L2_e5_loose_mu4") ) { fChain->SetBranchAddress("L2_e5_loose_mu4", &L2_e5_loose_mu4, &b_L2_e5_loose_mu4); }
  if( fChain->GetBranchStatus("L2_e5_medium") ) { fChain->SetBranchAddress("L2_e5_medium", &L2_e5_medium, &b_L2_e5_medium); }
  if( fChain->GetBranchStatus("L2_e5_medium_MV") ) { fChain->SetBranchAddress("L2_e5_medium_MV", &L2_e5_medium_MV, &b_L2_e5_medium_MV); }
  if( fChain->GetBranchStatus("L2_e5_medium_SiTrk") ) { fChain->SetBranchAddress("L2_e5_medium_SiTrk", &L2_e5_medium_SiTrk, &b_L2_e5_medium_SiTrk); }
  if( fChain->GetBranchStatus("L2_e5_medium_TRT") ) { fChain->SetBranchAddress("L2_e5_medium_TRT", &L2_e5_medium_TRT, &b_L2_e5_medium_TRT); }
  if( fChain->GetBranchStatus("L2_e5_medium_mu4") ) { fChain->SetBranchAddress("L2_e5_medium_mu4", &L2_e5_medium_mu4, &b_L2_e5_medium_mu4); }
  if( fChain->GetBranchStatus("L2_e5_tight") ) { fChain->SetBranchAddress("L2_e5_tight", &L2_e5_tight, &b_L2_e5_tight); }
  if( fChain->GetBranchStatus("L2_e5_tight_SiTrk") ) { fChain->SetBranchAddress("L2_e5_tight_SiTrk", &L2_e5_tight_SiTrk, &b_L2_e5_tight_SiTrk); }
  if( fChain->GetBranchStatus("L2_e5_tight_TRT") ) { fChain->SetBranchAddress("L2_e5_tight_TRT", &L2_e5_tight_TRT, &b_L2_e5_tight_TRT); }
  if( fChain->GetBranchStatus("L2_e5_tight_e5_NoCut") ) { fChain->SetBranchAddress("L2_e5_tight_e5_NoCut", &L2_e5_tight_e5_NoCut, &b_L2_e5_tight_e5_NoCut); }
  if( fChain->GetBranchStatus("L2_em105_passHLT") ) { fChain->SetBranchAddress("L2_em105_passHLT", &L2_em105_passHLT, &b_L2_em105_passHLT); }
  if( fChain->GetBranchStatus("L2_em3_empty_larcalib") ) { fChain->SetBranchAddress("L2_em3_empty_larcalib", &L2_em3_empty_larcalib, &b_L2_em3_empty_larcalib); }
  if( fChain->GetBranchStatus("L2_g10_loose") ) { fChain->SetBranchAddress("L2_g10_loose", &L2_g10_loose, &b_L2_g10_loose); }
  if( fChain->GetBranchStatus("L2_g11_etcut") ) { fChain->SetBranchAddress("L2_g11_etcut", &L2_g11_etcut, &b_L2_g11_etcut); }
  if( fChain->GetBranchStatus("L2_g15_loose") ) { fChain->SetBranchAddress("L2_g15_loose", &L2_g15_loose, &b_L2_g15_loose); }
  if( fChain->GetBranchStatus("L2_g17_etcut") ) { fChain->SetBranchAddress("L2_g17_etcut", &L2_g17_etcut, &b_L2_g17_etcut); }
  if( fChain->GetBranchStatus("L2_g17_etcut_EFxe20_noMu") ) { fChain->SetBranchAddress("L2_g17_etcut_EFxe20_noMu", &L2_g17_etcut_EFxe20_noMu, &b_L2_g17_etcut_EFxe20_noMu); }
  if( fChain->GetBranchStatus("L2_g17_etcut_EFxe30_noMu") ) { fChain->SetBranchAddress("L2_g17_etcut_EFxe30_noMu", &L2_g17_etcut_EFxe30_noMu, &b_L2_g17_etcut_EFxe30_noMu); }
  if( fChain->GetBranchStatus("L2_g20_loose") ) { fChain->SetBranchAddress("L2_g20_loose", &L2_g20_loose, &b_L2_g20_loose); }
  if( fChain->GetBranchStatus("L2_g20_loose_cnv") ) { fChain->SetBranchAddress("L2_g20_loose_cnv", &L2_g20_loose_cnv, &b_L2_g20_loose_cnv); }
  if( fChain->GetBranchStatus("L2_g20_loose_xe20_noMu") ) { fChain->SetBranchAddress("L2_g20_loose_xe20_noMu", &L2_g20_loose_xe20_noMu, &b_L2_g20_loose_xe20_noMu); }
  if( fChain->GetBranchStatus("L2_g20_loose_xe30_noMu") ) { fChain->SetBranchAddress("L2_g20_loose_xe30_noMu", &L2_g20_loose_xe30_noMu, &b_L2_g20_loose_xe30_noMu); }
  if( fChain->GetBranchStatus("L2_g20_tight") ) { fChain->SetBranchAddress("L2_g20_tight", &L2_g20_tight, &b_L2_g20_tight); }
  if( fChain->GetBranchStatus("L2_g25_loose_xe30_noMu") ) { fChain->SetBranchAddress("L2_g25_loose_xe30_noMu", &L2_g25_loose_xe30_noMu, &b_L2_g25_loose_xe30_noMu); }
  if( fChain->GetBranchStatus("L2_g30_loose") ) { fChain->SetBranchAddress("L2_g30_loose", &L2_g30_loose, &b_L2_g30_loose); }
  if( fChain->GetBranchStatus("L2_g30_tight") ) { fChain->SetBranchAddress("L2_g30_tight", &L2_g30_tight, &b_L2_g30_tight); }
  if( fChain->GetBranchStatus("L2_g3_NoCut_unpaired_iso") ) { fChain->SetBranchAddress("L2_g3_NoCut_unpaired_iso", &L2_g3_NoCut_unpaired_iso, &b_L2_g3_NoCut_unpaired_iso); }
  if( fChain->GetBranchStatus("L2_g3_NoCut_unpaired_noniso") ) { fChain->SetBranchAddress("L2_g3_NoCut_unpaired_noniso", &L2_g3_NoCut_unpaired_noniso, &b_L2_g3_NoCut_unpaired_noniso); }
  if( fChain->GetBranchStatus("L2_g40_loose") ) { fChain->SetBranchAddress("L2_g40_loose", &L2_g40_loose, &b_L2_g40_loose); }
  if( fChain->GetBranchStatus("L2_g40_tight") ) { fChain->SetBranchAddress("L2_g40_tight", &L2_g40_tight, &b_L2_g40_tight); }
  if( fChain->GetBranchStatus("L2_g50_loose") ) { fChain->SetBranchAddress("L2_g50_loose", &L2_g50_loose, &b_L2_g50_loose); }
  if( fChain->GetBranchStatus("L2_g5_NoCut_cosmic") ) { fChain->SetBranchAddress("L2_g5_NoCut_cosmic", &L2_g5_NoCut_cosmic, &b_L2_g5_NoCut_cosmic); }
  if( fChain->GetBranchStatus("L2_g5_loose") ) { fChain->SetBranchAddress("L2_g5_loose", &L2_g5_loose, &b_L2_g5_loose); }
  if( fChain->GetBranchStatus("L2_g5_loose_cnv") ) { fChain->SetBranchAddress("L2_g5_loose_cnv", &L2_g5_loose_cnv, &b_L2_g5_loose_cnv); }
  if( fChain->GetBranchStatus("L2_g7_loose") ) { fChain->SetBranchAddress("L2_g7_loose", &L2_g7_loose, &b_L2_g7_loose); }
  if( fChain->GetBranchStatus("L2_j30_jetNoCut_xe20_e15_medium") ) { fChain->SetBranchAddress("L2_j30_jetNoCut_xe20_e15_medium", &L2_j30_jetNoCut_xe20_e15_medium, &b_L2_j30_jetNoCut_xe20_e15_medium); }
  if( fChain->GetBranchStatus("L2_j30_xe20_e15_medium") ) { fChain->SetBranchAddress("L2_j30_xe20_e15_medium", &L2_j30_xe20_e15_medium, &b_L2_j30_xe20_e15_medium); }
  if( fChain->GetBranchStatus("L2_j35_jetNoCut_xe30_mu15") ) { fChain->SetBranchAddress("L2_j35_jetNoCut_xe30_mu15", &L2_j35_jetNoCut_xe30_mu15, &b_L2_j35_jetNoCut_xe30_mu15); }
  if( fChain->GetBranchStatus("L2_j35_xe30_mu15") ) { fChain->SetBranchAddress("L2_j35_xe30_mu15", &L2_j35_xe30_mu15, &b_L2_j35_xe30_mu15); }
  if( fChain->GetBranchStatus("L2_j45_xe12_noMu") ) { fChain->SetBranchAddress("L2_j45_xe12_noMu", &L2_j45_xe12_noMu, &b_L2_j45_xe12_noMu); }
  if( fChain->GetBranchStatus("L2_j45_xe20_noMu") ) { fChain->SetBranchAddress("L2_j45_xe20_noMu", &L2_j45_xe20_noMu, &b_L2_j45_xe20_noMu); }
  if( fChain->GetBranchStatus("L2_j45_xe30_noMu") ) { fChain->SetBranchAddress("L2_j45_xe30_noMu", &L2_j45_xe30_noMu, &b_L2_j45_xe30_noMu); }
  if( fChain->GetBranchStatus("L2_mu0_NoAlg") ) { fChain->SetBranchAddress("L2_mu0_NoAlg", &L2_mu0_NoAlg, &b_L2_mu0_NoAlg); }
  if( fChain->GetBranchStatus("L2_mu0_comm_NoAlg") ) { fChain->SetBranchAddress("L2_mu0_comm_NoAlg", &L2_mu0_comm_NoAlg, &b_L2_mu0_comm_NoAlg); }
  if( fChain->GetBranchStatus("L2_mu0_comm_empty_NoAlg") ) { fChain->SetBranchAddress("L2_mu0_comm_empty_NoAlg", &L2_mu0_comm_empty_NoAlg, &b_L2_mu0_comm_empty_NoAlg); }
  if( fChain->GetBranchStatus("L2_mu0_comm_firstempty_NoAlg") ) { fChain->SetBranchAddress("L2_mu0_comm_firstempty_NoAlg", &L2_mu0_comm_firstempty_NoAlg, &b_L2_mu0_comm_firstempty_NoAlg); }
  if( fChain->GetBranchStatus("L2_mu0_comm_unpaired_iso_NoAlg") ) { fChain->SetBranchAddress("L2_mu0_comm_unpaired_iso_NoAlg", &L2_mu0_comm_unpaired_iso_NoAlg, &b_L2_mu0_comm_unpaired_iso_NoAlg); }
  if( fChain->GetBranchStatus("L2_mu0_comm_unpaired_noniso_NoAlg") ) { fChain->SetBranchAddress("L2_mu0_comm_unpaired_noniso_NoAlg", &L2_mu0_comm_unpaired_noniso_NoAlg, &b_L2_mu0_comm_unpaired_noniso_NoAlg); }
  if( fChain->GetBranchStatus("L2_mu0_empty_NoAlg") ) { fChain->SetBranchAddress("L2_mu0_empty_NoAlg", &L2_mu0_empty_NoAlg, &b_L2_mu0_empty_NoAlg); }
  if( fChain->GetBranchStatus("L2_mu0_firstempty_NoAlg") ) { fChain->SetBranchAddress("L2_mu0_firstempty_NoAlg", &L2_mu0_firstempty_NoAlg, &b_L2_mu0_firstempty_NoAlg); }
  if( fChain->GetBranchStatus("L2_mu0_missingRoi") ) { fChain->SetBranchAddress("L2_mu0_missingRoi", &L2_mu0_missingRoi, &b_L2_mu0_missingRoi); }
  if( fChain->GetBranchStatus("L2_mu0_outOfTime1") ) { fChain->SetBranchAddress("L2_mu0_outOfTime1", &L2_mu0_outOfTime1, &b_L2_mu0_outOfTime1); }
  if( fChain->GetBranchStatus("L2_mu0_outOfTime2") ) { fChain->SetBranchAddress("L2_mu0_outOfTime2", &L2_mu0_outOfTime2, &b_L2_mu0_outOfTime2); }
  if( fChain->GetBranchStatus("L2_mu0_rpcOnly") ) { fChain->SetBranchAddress("L2_mu0_rpcOnly", &L2_mu0_rpcOnly, &b_L2_mu0_rpcOnly); }
  if( fChain->GetBranchStatus("L2_mu0_unpaired_iso_NoAlg") ) { fChain->SetBranchAddress("L2_mu0_unpaired_iso_NoAlg", &L2_mu0_unpaired_iso_NoAlg, &b_L2_mu0_unpaired_iso_NoAlg); }
  if( fChain->GetBranchStatus("L2_mu0_unpaired_noniso_NoAlg") ) { fChain->SetBranchAddress("L2_mu0_unpaired_noniso_NoAlg", &L2_mu0_unpaired_noniso_NoAlg, &b_L2_mu0_unpaired_noniso_NoAlg); }
  if( fChain->GetBranchStatus("L2_mu10") ) { fChain->SetBranchAddress("L2_mu10", &L2_mu10, &b_L2_mu10); }
  if( fChain->GetBranchStatus("L2_mu10_Ecut12") ) { fChain->SetBranchAddress("L2_mu10_Ecut12", &L2_mu10_Ecut12, &b_L2_mu10_Ecut12); }
  if( fChain->GetBranchStatus("L2_mu10_Ecut123") ) { fChain->SetBranchAddress("L2_mu10_Ecut123", &L2_mu10_Ecut123, &b_L2_mu10_Ecut123); }
  if( fChain->GetBranchStatus("L2_mu10_Ecut13") ) { fChain->SetBranchAddress("L2_mu10_Ecut13", &L2_mu10_Ecut13, &b_L2_mu10_Ecut13); }
  if( fChain->GetBranchStatus("L2_mu10_IDTrkNoCut") ) { fChain->SetBranchAddress("L2_mu10_IDTrkNoCut", &L2_mu10_IDTrkNoCut, &b_L2_mu10_IDTrkNoCut); }
  if( fChain->GetBranchStatus("L2_mu10_MG") ) { fChain->SetBranchAddress("L2_mu10_MG", &L2_mu10_MG, &b_L2_mu10_MG); }
  if( fChain->GetBranchStatus("L2_mu10_MSonly") ) { fChain->SetBranchAddress("L2_mu10_MSonly", &L2_mu10_MSonly, &b_L2_mu10_MSonly); }
  if( fChain->GetBranchStatus("L2_mu10_MSonly_Ecut12") ) { fChain->SetBranchAddress("L2_mu10_MSonly_Ecut12", &L2_mu10_MSonly_Ecut12, &b_L2_mu10_MSonly_Ecut12); }
  if( fChain->GetBranchStatus("L2_mu10_MSonly_Ecut123") ) { fChain->SetBranchAddress("L2_mu10_MSonly_Ecut123", &L2_mu10_MSonly_Ecut123, &b_L2_mu10_MSonly_Ecut123); }
  if( fChain->GetBranchStatus("L2_mu10_MSonly_Ecut13") ) { fChain->SetBranchAddress("L2_mu10_MSonly_Ecut13", &L2_mu10_MSonly_Ecut13, &b_L2_mu10_MSonly_Ecut13); }
  if( fChain->GetBranchStatus("L2_mu10_MSonly_tight") ) { fChain->SetBranchAddress("L2_mu10_MSonly_tight", &L2_mu10_MSonly_tight, &b_L2_mu10_MSonly_tight); }
  if( fChain->GetBranchStatus("L2_mu10_NoAlg") ) { fChain->SetBranchAddress("L2_mu10_NoAlg", &L2_mu10_NoAlg, &b_L2_mu10_NoAlg); }
  if( fChain->GetBranchStatus("L2_mu10_SiTrk") ) { fChain->SetBranchAddress("L2_mu10_SiTrk", &L2_mu10_SiTrk, &b_L2_mu10_SiTrk); }
  if( fChain->GetBranchStatus("L2_mu10_j30") ) { fChain->SetBranchAddress("L2_mu10_j30", &L2_mu10_j30, &b_L2_mu10_j30); }
  if( fChain->GetBranchStatus("L2_mu10_tight") ) { fChain->SetBranchAddress("L2_mu10_tight", &L2_mu10_tight, &b_L2_mu10_tight); }
  if( fChain->GetBranchStatus("L2_mu10i_loose") ) { fChain->SetBranchAddress("L2_mu10i_loose", &L2_mu10i_loose, &b_L2_mu10i_loose); }
  if( fChain->GetBranchStatus("L2_mu13") ) { fChain->SetBranchAddress("L2_mu13", &L2_mu13, &b_L2_mu13); }
  if( fChain->GetBranchStatus("L2_mu13_MG") ) { fChain->SetBranchAddress("L2_mu13_MG", &L2_mu13_MG, &b_L2_mu13_MG); }
  if( fChain->GetBranchStatus("L2_mu13_MG_tight") ) { fChain->SetBranchAddress("L2_mu13_MG_tight", &L2_mu13_MG_tight, &b_L2_mu13_MG_tight); }
  if( fChain->GetBranchStatus("L2_mu13_tight") ) { fChain->SetBranchAddress("L2_mu13_tight", &L2_mu13_tight, &b_L2_mu13_tight); }
  if( fChain->GetBranchStatus("L2_mu15") ) { fChain->SetBranchAddress("L2_mu15", &L2_mu15, &b_L2_mu15); }
  if( fChain->GetBranchStatus("L2_mu15_MG") ) { fChain->SetBranchAddress("L2_mu15_MG", &L2_mu15_MG, &b_L2_mu15_MG); }
  if( fChain->GetBranchStatus("L2_mu15_NoAlg") ) { fChain->SetBranchAddress("L2_mu15_NoAlg", &L2_mu15_NoAlg, &b_L2_mu15_NoAlg); }
  if( fChain->GetBranchStatus("L2_mu20") ) { fChain->SetBranchAddress("L2_mu20", &L2_mu20, &b_L2_mu20); }
  if( fChain->GetBranchStatus("L2_mu20_MSonly") ) { fChain->SetBranchAddress("L2_mu20_MSonly", &L2_mu20_MSonly, &b_L2_mu20_MSonly); }
  if( fChain->GetBranchStatus("L2_mu20_NoAlg") ) { fChain->SetBranchAddress("L2_mu20_NoAlg", &L2_mu20_NoAlg, &b_L2_mu20_NoAlg); }
  if( fChain->GetBranchStatus("L2_mu20_passHLT") ) { fChain->SetBranchAddress("L2_mu20_passHLT", &L2_mu20_passHLT, &b_L2_mu20_passHLT); }
  if( fChain->GetBranchStatus("L2_mu20_slow") ) { fChain->SetBranchAddress("L2_mu20_slow", &L2_mu20_slow, &b_L2_mu20_slow); }
  if( fChain->GetBranchStatus("L2_mu30_MSonly") ) { fChain->SetBranchAddress("L2_mu30_MSonly", &L2_mu30_MSonly, &b_L2_mu30_MSonly); }
  if( fChain->GetBranchStatus("L2_mu4") ) { fChain->SetBranchAddress("L2_mu4", &L2_mu4, &b_L2_mu4); }
  if( fChain->GetBranchStatus("L2_mu40_MSonly") ) { fChain->SetBranchAddress("L2_mu40_MSonly", &L2_mu40_MSonly, &b_L2_mu40_MSonly); }
  if( fChain->GetBranchStatus("L2_mu4_Bmumu") ) { fChain->SetBranchAddress("L2_mu4_Bmumu", &L2_mu4_Bmumu, &b_L2_mu4_Bmumu); }
  if( fChain->GetBranchStatus("L2_mu4_BmumuX") ) { fChain->SetBranchAddress("L2_mu4_BmumuX", &L2_mu4_BmumuX, &b_L2_mu4_BmumuX); }
  if( fChain->GetBranchStatus("L2_mu4_DiMu") ) { fChain->SetBranchAddress("L2_mu4_DiMu", &L2_mu4_DiMu, &b_L2_mu4_DiMu); }
  if( fChain->GetBranchStatus("L2_mu4_DiMu_FS") ) { fChain->SetBranchAddress("L2_mu4_DiMu_FS", &L2_mu4_DiMu_FS, &b_L2_mu4_DiMu_FS); }
  if( fChain->GetBranchStatus("L2_mu4_DiMu_FS_noOS") ) { fChain->SetBranchAddress("L2_mu4_DiMu_FS_noOS", &L2_mu4_DiMu_FS_noOS, &b_L2_mu4_DiMu_FS_noOS); }
  if( fChain->GetBranchStatus("L2_mu4_DiMu_MG") ) { fChain->SetBranchAddress("L2_mu4_DiMu_MG", &L2_mu4_DiMu_MG, &b_L2_mu4_DiMu_MG); }
  if( fChain->GetBranchStatus("L2_mu4_DiMu_MG_FS") ) { fChain->SetBranchAddress("L2_mu4_DiMu_MG_FS", &L2_mu4_DiMu_MG_FS, &b_L2_mu4_DiMu_MG_FS); }
  if( fChain->GetBranchStatus("L2_mu4_DiMu_SiTrk") ) { fChain->SetBranchAddress("L2_mu4_DiMu_SiTrk", &L2_mu4_DiMu_SiTrk, &b_L2_mu4_DiMu_SiTrk); }
  if( fChain->GetBranchStatus("L2_mu4_DiMu_SiTrk_FS") ) { fChain->SetBranchAddress("L2_mu4_DiMu_SiTrk_FS", &L2_mu4_DiMu_SiTrk_FS, &b_L2_mu4_DiMu_SiTrk_FS); }
  if( fChain->GetBranchStatus("L2_mu4_DiMu_noOS") ) { fChain->SetBranchAddress("L2_mu4_DiMu_noOS", &L2_mu4_DiMu_noOS, &b_L2_mu4_DiMu_noOS); }
  if( fChain->GetBranchStatus("L2_mu4_IDTrkNoCut") ) { fChain->SetBranchAddress("L2_mu4_IDTrkNoCut", &L2_mu4_IDTrkNoCut, &b_L2_mu4_IDTrkNoCut); }
  if( fChain->GetBranchStatus("L2_mu4_Jpsie5e3") ) { fChain->SetBranchAddress("L2_mu4_Jpsie5e3", &L2_mu4_Jpsie5e3, &b_L2_mu4_Jpsie5e3); }
  if( fChain->GetBranchStatus("L2_mu4_Jpsie5e3_FS") ) { fChain->SetBranchAddress("L2_mu4_Jpsie5e3_FS", &L2_mu4_Jpsie5e3_FS, &b_L2_mu4_Jpsie5e3_FS); }
  if( fChain->GetBranchStatus("L2_mu4_Jpsie5e3_SiTrk") ) { fChain->SetBranchAddress("L2_mu4_Jpsie5e3_SiTrk", &L2_mu4_Jpsie5e3_SiTrk, &b_L2_mu4_Jpsie5e3_SiTrk); }
  if( fChain->GetBranchStatus("L2_mu4_Jpsie5e3_SiTrk_FS") ) { fChain->SetBranchAddress("L2_mu4_Jpsie5e3_SiTrk_FS", &L2_mu4_Jpsie5e3_SiTrk_FS, &b_L2_mu4_Jpsie5e3_SiTrk_FS); }
  if( fChain->GetBranchStatus("L2_mu4_Jpsimumu") ) { fChain->SetBranchAddress("L2_mu4_Jpsimumu", &L2_mu4_Jpsimumu, &b_L2_mu4_Jpsimumu); }
  if( fChain->GetBranchStatus("L2_mu4_Jpsimumu_FS") ) { fChain->SetBranchAddress("L2_mu4_Jpsimumu_FS", &L2_mu4_Jpsimumu_FS, &b_L2_mu4_Jpsimumu_FS); }
  if( fChain->GetBranchStatus("L2_mu4_Jpsimumu_SiTrk_FS") ) { fChain->SetBranchAddress("L2_mu4_Jpsimumu_SiTrk_FS", &L2_mu4_Jpsimumu_SiTrk_FS, &b_L2_mu4_Jpsimumu_SiTrk_FS); }
  if( fChain->GetBranchStatus("L2_mu4_L1J10_matched") ) { fChain->SetBranchAddress("L2_mu4_L1J10_matched", &L2_mu4_L1J10_matched, &b_L2_mu4_L1J10_matched); }
  if( fChain->GetBranchStatus("L2_mu4_L1J15_matched") ) { fChain->SetBranchAddress("L2_mu4_L1J15_matched", &L2_mu4_L1J15_matched, &b_L2_mu4_L1J15_matched); }
  if( fChain->GetBranchStatus("L2_mu4_L1J30_matched") ) { fChain->SetBranchAddress("L2_mu4_L1J30_matched", &L2_mu4_L1J30_matched, &b_L2_mu4_L1J30_matched); }
  if( fChain->GetBranchStatus("L2_mu4_L1J55_matched") ) { fChain->SetBranchAddress("L2_mu4_L1J55_matched", &L2_mu4_L1J55_matched, &b_L2_mu4_L1J55_matched); }
  if( fChain->GetBranchStatus("L2_mu4_L1J5_matched") ) { fChain->SetBranchAddress("L2_mu4_L1J5_matched", &L2_mu4_L1J5_matched, &b_L2_mu4_L1J5_matched); }
  if( fChain->GetBranchStatus("L2_mu4_L2MSonly_EFFS_passL2") ) { fChain->SetBranchAddress("L2_mu4_L2MSonly_EFFS_passL2", &L2_mu4_L2MSonly_EFFS_passL2, &b_L2_mu4_L2MSonly_EFFS_passL2); }
  if( fChain->GetBranchStatus("L2_mu4_MG") ) { fChain->SetBranchAddress("L2_mu4_MG", &L2_mu4_MG, &b_L2_mu4_MG); }
  if( fChain->GetBranchStatus("L2_mu4_MSonly") ) { fChain->SetBranchAddress("L2_mu4_MSonly", &L2_mu4_MSonly, &b_L2_mu4_MSonly); }
  if( fChain->GetBranchStatus("L2_mu4_MSonly_EFFS_passL2") ) { fChain->SetBranchAddress("L2_mu4_MSonly_EFFS_passL2", &L2_mu4_MSonly_EFFS_passL2, &b_L2_mu4_MSonly_EFFS_passL2); }
  if( fChain->GetBranchStatus("L2_mu4_MSonly_MB2_noL2_EFFS") ) { fChain->SetBranchAddress("L2_mu4_MSonly_MB2_noL2_EFFS", &L2_mu4_MSonly_MB2_noL2_EFFS, &b_L2_mu4_MSonly_MB2_noL2_EFFS); }
  if( fChain->GetBranchStatus("L2_mu4_MSonly_cosmic") ) { fChain->SetBranchAddress("L2_mu4_MSonly_cosmic", &L2_mu4_MSonly_cosmic, &b_L2_mu4_MSonly_cosmic); }
  if( fChain->GetBranchStatus("L2_mu4_MSonly_outOfTime") ) { fChain->SetBranchAddress("L2_mu4_MSonly_outOfTime", &L2_mu4_MSonly_outOfTime, &b_L2_mu4_MSonly_outOfTime); }
  if( fChain->GetBranchStatus("L2_mu4_MV") ) { fChain->SetBranchAddress("L2_mu4_MV", &L2_mu4_MV, &b_L2_mu4_MV); }
  if( fChain->GetBranchStatus("L2_mu4_SiTrk") ) { fChain->SetBranchAddress("L2_mu4_SiTrk", &L2_mu4_SiTrk, &b_L2_mu4_SiTrk); }
  if( fChain->GetBranchStatus("L2_mu4_Trk_Jpsi") ) { fChain->SetBranchAddress("L2_mu4_Trk_Jpsi", &L2_mu4_Trk_Jpsi, &b_L2_mu4_Trk_Jpsi); }
  if( fChain->GetBranchStatus("L2_mu4_Trk_Jpsi_FS") ) { fChain->SetBranchAddress("L2_mu4_Trk_Jpsi_FS", &L2_mu4_Trk_Jpsi_FS, &b_L2_mu4_Trk_Jpsi_FS); }
  if( fChain->GetBranchStatus("L2_mu4_Trk_Jpsi_loose") ) { fChain->SetBranchAddress("L2_mu4_Trk_Jpsi_loose", &L2_mu4_Trk_Jpsi_loose, &b_L2_mu4_Trk_Jpsi_loose); }
  if( fChain->GetBranchStatus("L2_mu4_Trk_Upsi_FS") ) { fChain->SetBranchAddress("L2_mu4_Trk_Upsi_FS", &L2_mu4_Trk_Upsi_FS, &b_L2_mu4_Trk_Upsi_FS); }
  if( fChain->GetBranchStatus("L2_mu4_Trk_Upsi_loose_FS") ) { fChain->SetBranchAddress("L2_mu4_Trk_Upsi_loose_FS", &L2_mu4_Trk_Upsi_loose_FS, &b_L2_mu4_Trk_Upsi_loose_FS); }
  if( fChain->GetBranchStatus("L2_mu4_Upsimumu_FS") ) { fChain->SetBranchAddress("L2_mu4_Upsimumu_FS", &L2_mu4_Upsimumu_FS, &b_L2_mu4_Upsimumu_FS); }
  if( fChain->GetBranchStatus("L2_mu4_Upsimumu_SiTrk_FS") ) { fChain->SetBranchAddress("L2_mu4_Upsimumu_SiTrk_FS", &L2_mu4_Upsimumu_SiTrk_FS, &b_L2_mu4_Upsimumu_SiTrk_FS); }
  if( fChain->GetBranchStatus("L2_mu4_comm_MSonly_cosmic") ) { fChain->SetBranchAddress("L2_mu4_comm_MSonly_cosmic", &L2_mu4_comm_MSonly_cosmic, &b_L2_mu4_comm_MSonly_cosmic); }
  if( fChain->GetBranchStatus("L2_mu4_comm_cosmic") ) { fChain->SetBranchAddress("L2_mu4_comm_cosmic", &L2_mu4_comm_cosmic, &b_L2_mu4_comm_cosmic); }
  if( fChain->GetBranchStatus("L2_mu4_comm_firstempty") ) { fChain->SetBranchAddress("L2_mu4_comm_firstempty", &L2_mu4_comm_firstempty, &b_L2_mu4_comm_firstempty); }
  if( fChain->GetBranchStatus("L2_mu4_comm_unpaired_iso") ) { fChain->SetBranchAddress("L2_mu4_comm_unpaired_iso", &L2_mu4_comm_unpaired_iso, &b_L2_mu4_comm_unpaired_iso); }
  if( fChain->GetBranchStatus("L2_mu4_cosmic") ) { fChain->SetBranchAddress("L2_mu4_cosmic", &L2_mu4_cosmic, &b_L2_mu4_cosmic); }
  if( fChain->GetBranchStatus("L2_mu4_firstempty") ) { fChain->SetBranchAddress("L2_mu4_firstempty", &L2_mu4_firstempty, &b_L2_mu4_firstempty); }
  if( fChain->GetBranchStatus("L2_mu4_j20") ) { fChain->SetBranchAddress("L2_mu4_j20", &L2_mu4_j20, &b_L2_mu4_j20); }
  if( fChain->GetBranchStatus("L2_mu4_j20_jetNoEF") ) { fChain->SetBranchAddress("L2_mu4_j20_jetNoEF", &L2_mu4_j20_jetNoEF, &b_L2_mu4_j20_jetNoEF); }
  if( fChain->GetBranchStatus("L2_mu4_j25") ) { fChain->SetBranchAddress("L2_mu4_j25", &L2_mu4_j25, &b_L2_mu4_j25); }
  if( fChain->GetBranchStatus("L2_mu4_mu6") ) { fChain->SetBranchAddress("L2_mu4_mu6", &L2_mu4_mu6, &b_L2_mu4_mu6); }
  if( fChain->GetBranchStatus("L2_mu4_muCombTag") ) { fChain->SetBranchAddress("L2_mu4_muCombTag", &L2_mu4_muCombTag, &b_L2_mu4_muCombTag); }
  if( fChain->GetBranchStatus("L2_mu4_tile") ) { fChain->SetBranchAddress("L2_mu4_tile", &L2_mu4_tile, &b_L2_mu4_tile); }
  if( fChain->GetBranchStatus("L2_mu4_tile_cosmic") ) { fChain->SetBranchAddress("L2_mu4_tile_cosmic", &L2_mu4_tile_cosmic, &b_L2_mu4_tile_cosmic); }
  if( fChain->GetBranchStatus("L2_mu4_unpaired_iso") ) { fChain->SetBranchAddress("L2_mu4_unpaired_iso", &L2_mu4_unpaired_iso, &b_L2_mu4_unpaired_iso); }
  if( fChain->GetBranchStatus("L2_mu4mu6_Bmumu") ) { fChain->SetBranchAddress("L2_mu4mu6_Bmumu", &L2_mu4mu6_Bmumu, &b_L2_mu4mu6_Bmumu); }
  if( fChain->GetBranchStatus("L2_mu4mu6_BmumuX") ) { fChain->SetBranchAddress("L2_mu4mu6_BmumuX", &L2_mu4mu6_BmumuX, &b_L2_mu4mu6_BmumuX); }
  if( fChain->GetBranchStatus("L2_mu4mu6_DiMu") ) { fChain->SetBranchAddress("L2_mu4mu6_DiMu", &L2_mu4mu6_DiMu, &b_L2_mu4mu6_DiMu); }
  if( fChain->GetBranchStatus("L2_mu4mu6_Jpsimumu") ) { fChain->SetBranchAddress("L2_mu4mu6_Jpsimumu", &L2_mu4mu6_Jpsimumu, &b_L2_mu4mu6_Jpsimumu); }
  if( fChain->GetBranchStatus("L2_mu4mu6_Jpsimumu_IDTrkNoCut") ) { fChain->SetBranchAddress("L2_mu4mu6_Jpsimumu_IDTrkNoCut", &L2_mu4mu6_Jpsimumu_IDTrkNoCut, &b_L2_mu4mu6_Jpsimumu_IDTrkNoCut); }
  if( fChain->GetBranchStatus("L2_mu4mu6_Upsimumu") ) { fChain->SetBranchAddress("L2_mu4mu6_Upsimumu", &L2_mu4mu6_Upsimumu, &b_L2_mu4mu6_Upsimumu); }
  if( fChain->GetBranchStatus("L2_mu6") ) { fChain->SetBranchAddress("L2_mu6", &L2_mu6, &b_L2_mu6); }
  if( fChain->GetBranchStatus("L2_mu6_Bmumu") ) { fChain->SetBranchAddress("L2_mu6_Bmumu", &L2_mu6_Bmumu, &b_L2_mu6_Bmumu); }
  if( fChain->GetBranchStatus("L2_mu6_BmumuX") ) { fChain->SetBranchAddress("L2_mu6_BmumuX", &L2_mu6_BmumuX, &b_L2_mu6_BmumuX); }
  if( fChain->GetBranchStatus("L2_mu6_DiMu") ) { fChain->SetBranchAddress("L2_mu6_DiMu", &L2_mu6_DiMu, &b_L2_mu6_DiMu); }
  if( fChain->GetBranchStatus("L2_mu6_Ecut12") ) { fChain->SetBranchAddress("L2_mu6_Ecut12", &L2_mu6_Ecut12, &b_L2_mu6_Ecut12); }
  if( fChain->GetBranchStatus("L2_mu6_Ecut123") ) { fChain->SetBranchAddress("L2_mu6_Ecut123", &L2_mu6_Ecut123, &b_L2_mu6_Ecut123); }
  if( fChain->GetBranchStatus("L2_mu6_Ecut13") ) { fChain->SetBranchAddress("L2_mu6_Ecut13", &L2_mu6_Ecut13, &b_L2_mu6_Ecut13); }
  if( fChain->GetBranchStatus("L2_mu6_Ecut2") ) { fChain->SetBranchAddress("L2_mu6_Ecut2", &L2_mu6_Ecut2, &b_L2_mu6_Ecut2); }
  if( fChain->GetBranchStatus("L2_mu6_Ecut3") ) { fChain->SetBranchAddress("L2_mu6_Ecut3", &L2_mu6_Ecut3, &b_L2_mu6_Ecut3); }
  if( fChain->GetBranchStatus("L2_mu6_IDTrkNoCut") ) { fChain->SetBranchAddress("L2_mu6_IDTrkNoCut", &L2_mu6_IDTrkNoCut, &b_L2_mu6_IDTrkNoCut); }
  if( fChain->GetBranchStatus("L2_mu6_Jpsie5e3") ) { fChain->SetBranchAddress("L2_mu6_Jpsie5e3", &L2_mu6_Jpsie5e3, &b_L2_mu6_Jpsie5e3); }
  if( fChain->GetBranchStatus("L2_mu6_Jpsie5e3_FS") ) { fChain->SetBranchAddress("L2_mu6_Jpsie5e3_FS", &L2_mu6_Jpsie5e3_FS, &b_L2_mu6_Jpsie5e3_FS); }
  if( fChain->GetBranchStatus("L2_mu6_Jpsie5e3_SiTrk") ) { fChain->SetBranchAddress("L2_mu6_Jpsie5e3_SiTrk", &L2_mu6_Jpsie5e3_SiTrk, &b_L2_mu6_Jpsie5e3_SiTrk); }
  if( fChain->GetBranchStatus("L2_mu6_Jpsie5e3_SiTrk_FS") ) { fChain->SetBranchAddress("L2_mu6_Jpsie5e3_SiTrk_FS", &L2_mu6_Jpsie5e3_SiTrk_FS, &b_L2_mu6_Jpsie5e3_SiTrk_FS); }
  if( fChain->GetBranchStatus("L2_mu6_Jpsimumu") ) { fChain->SetBranchAddress("L2_mu6_Jpsimumu", &L2_mu6_Jpsimumu, &b_L2_mu6_Jpsimumu); }
  if( fChain->GetBranchStatus("L2_mu6_MG") ) { fChain->SetBranchAddress("L2_mu6_MG", &L2_mu6_MG, &b_L2_mu6_MG); }
  if( fChain->GetBranchStatus("L2_mu6_MSonly") ) { fChain->SetBranchAddress("L2_mu6_MSonly", &L2_mu6_MSonly, &b_L2_mu6_MSonly); }
  if( fChain->GetBranchStatus("L2_mu6_MSonly_Ecut12") ) { fChain->SetBranchAddress("L2_mu6_MSonly_Ecut12", &L2_mu6_MSonly_Ecut12, &b_L2_mu6_MSonly_Ecut12); }
  if( fChain->GetBranchStatus("L2_mu6_MSonly_Ecut123") ) { fChain->SetBranchAddress("L2_mu6_MSonly_Ecut123", &L2_mu6_MSonly_Ecut123, &b_L2_mu6_MSonly_Ecut123); }
  if( fChain->GetBranchStatus("L2_mu6_MSonly_Ecut13") ) { fChain->SetBranchAddress("L2_mu6_MSonly_Ecut13", &L2_mu6_MSonly_Ecut13, &b_L2_mu6_MSonly_Ecut13); }
  if( fChain->GetBranchStatus("L2_mu6_MSonly_Ecut2") ) { fChain->SetBranchAddress("L2_mu6_MSonly_Ecut2", &L2_mu6_MSonly_Ecut2, &b_L2_mu6_MSonly_Ecut2); }
  if( fChain->GetBranchStatus("L2_mu6_MSonly_Ecut3") ) { fChain->SetBranchAddress("L2_mu6_MSonly_Ecut3", &L2_mu6_MSonly_Ecut3, &b_L2_mu6_MSonly_Ecut3); }
  if( fChain->GetBranchStatus("L2_mu6_MSonly_outOfTime") ) { fChain->SetBranchAddress("L2_mu6_MSonly_outOfTime", &L2_mu6_MSonly_outOfTime, &b_L2_mu6_MSonly_outOfTime); }
  if( fChain->GetBranchStatus("L2_mu6_NoAlg") ) { fChain->SetBranchAddress("L2_mu6_NoAlg", &L2_mu6_NoAlg, &b_L2_mu6_NoAlg); }
  if( fChain->GetBranchStatus("L2_mu6_SiTrk") ) { fChain->SetBranchAddress("L2_mu6_SiTrk", &L2_mu6_SiTrk, &b_L2_mu6_SiTrk); }
  if( fChain->GetBranchStatus("L2_mu6_Trk_Jpsi") ) { fChain->SetBranchAddress("L2_mu6_Trk_Jpsi", &L2_mu6_Trk_Jpsi, &b_L2_mu6_Trk_Jpsi); }
  if( fChain->GetBranchStatus("L2_mu6_Upsimumu_FS") ) { fChain->SetBranchAddress("L2_mu6_Upsimumu_FS", &L2_mu6_Upsimumu_FS, &b_L2_mu6_Upsimumu_FS); }
  if( fChain->GetBranchStatus("L2_mu6_muCombTag") ) { fChain->SetBranchAddress("L2_mu6_muCombTag", &L2_mu6_muCombTag, &b_L2_mu6_muCombTag); }
  if( fChain->GetBranchStatus("L2_tau125_loose") ) { fChain->SetBranchAddress("L2_tau125_loose", &L2_tau125_loose, &b_L2_tau125_loose); }
  if( fChain->GetBranchStatus("L2_tau125_medium") ) { fChain->SetBranchAddress("L2_tau125_medium", &L2_tau125_medium, &b_L2_tau125_medium); }
  if( fChain->GetBranchStatus("L2_tau12_IDTrkNoCut") ) { fChain->SetBranchAddress("L2_tau12_IDTrkNoCut", &L2_tau12_IDTrkNoCut, &b_L2_tau12_IDTrkNoCut); }
  if( fChain->GetBranchStatus("L2_tau12_loose") ) { fChain->SetBranchAddress("L2_tau12_loose", &L2_tau12_loose, &b_L2_tau12_loose); }
  if( fChain->GetBranchStatus("L2_tau12_loose_2b15") ) { fChain->SetBranchAddress("L2_tau12_loose_2b15", &L2_tau12_loose_2b15, &b_L2_tau12_loose_2b15); }
  if( fChain->GetBranchStatus("L2_tau12_loose_3j30") ) { fChain->SetBranchAddress("L2_tau12_loose_3j30", &L2_tau12_loose_3j30, &b_L2_tau12_loose_3j30); }
  if( fChain->GetBranchStatus("L2_tau12_loose_EFxe12_noMu") ) { fChain->SetBranchAddress("L2_tau12_loose_EFxe12_noMu", &L2_tau12_loose_EFxe12_noMu, &b_L2_tau12_loose_EFxe12_noMu); }
  if( fChain->GetBranchStatus("L2_tau12_loose_IdScan_EFxe12_noMu") ) { fChain->SetBranchAddress("L2_tau12_loose_IdScan_EFxe12_noMu", &L2_tau12_loose_IdScan_EFxe12_noMu, &b_L2_tau12_loose_IdScan_EFxe12_noMu); }
  if( fChain->GetBranchStatus("L2_tau12_loose_IdScan_xe15_noMu") ) { fChain->SetBranchAddress("L2_tau12_loose_IdScan_xe15_noMu", &L2_tau12_loose_IdScan_xe15_noMu, &b_L2_tau12_loose_IdScan_xe15_noMu); }
  if( fChain->GetBranchStatus("L2_tau12_loose_e10_loose") ) { fChain->SetBranchAddress("L2_tau12_loose_e10_loose", &L2_tau12_loose_e10_loose, &b_L2_tau12_loose_e10_loose); }
  if( fChain->GetBranchStatus("L2_tau12_loose_e10_medium") ) { fChain->SetBranchAddress("L2_tau12_loose_e10_medium", &L2_tau12_loose_e10_medium, &b_L2_tau12_loose_e10_medium); }
  if( fChain->GetBranchStatus("L2_tau12_loose_e10_tight") ) { fChain->SetBranchAddress("L2_tau12_loose_e10_tight", &L2_tau12_loose_e10_tight, &b_L2_tau12_loose_e10_tight); }
  if( fChain->GetBranchStatus("L2_tau12_loose_mu10") ) { fChain->SetBranchAddress("L2_tau12_loose_mu10", &L2_tau12_loose_mu10, &b_L2_tau12_loose_mu10); }
  if( fChain->GetBranchStatus("L2_tau12_loose_xe15_noMu") ) { fChain->SetBranchAddress("L2_tau12_loose_xe15_noMu", &L2_tau12_loose_xe15_noMu, &b_L2_tau12_loose_xe15_noMu); }
  if( fChain->GetBranchStatus("L2_tau12_loose_xe20_noMu") ) { fChain->SetBranchAddress("L2_tau12_loose_xe20_noMu", &L2_tau12_loose_xe20_noMu, &b_L2_tau12_loose_xe20_noMu); }
  if( fChain->GetBranchStatus("L2_tau16_loose") ) { fChain->SetBranchAddress("L2_tau16_loose", &L2_tau16_loose, &b_L2_tau16_loose); }
  if( fChain->GetBranchStatus("L2_tau16_loose_2b15") ) { fChain->SetBranchAddress("L2_tau16_loose_2b15", &L2_tau16_loose_2b15, &b_L2_tau16_loose_2b15); }
  if( fChain->GetBranchStatus("L2_tau16_loose_3j30") ) { fChain->SetBranchAddress("L2_tau16_loose_3j30", &L2_tau16_loose_3j30, &b_L2_tau16_loose_3j30); }
  if( fChain->GetBranchStatus("L2_tau16_loose_e10_loose") ) { fChain->SetBranchAddress("L2_tau16_loose_e10_loose", &L2_tau16_loose_e10_loose, &b_L2_tau16_loose_e10_loose); }
  if( fChain->GetBranchStatus("L2_tau16_loose_e15_loose") ) { fChain->SetBranchAddress("L2_tau16_loose_e15_loose", &L2_tau16_loose_e15_loose, &b_L2_tau16_loose_e15_loose); }
  if( fChain->GetBranchStatus("L2_tau16_loose_mu10") ) { fChain->SetBranchAddress("L2_tau16_loose_mu10", &L2_tau16_loose_mu10, &b_L2_tau16_loose_mu10); }
  if( fChain->GetBranchStatus("L2_tau16_loose_mu15") ) { fChain->SetBranchAddress("L2_tau16_loose_mu15", &L2_tau16_loose_mu15, &b_L2_tau16_loose_mu15); }
  if( fChain->GetBranchStatus("L2_tau16_loose_xe20_noMu") ) { fChain->SetBranchAddress("L2_tau16_loose_xe20_noMu", &L2_tau16_loose_xe20_noMu, &b_L2_tau16_loose_xe20_noMu); }
  if( fChain->GetBranchStatus("L2_tau16_loose_xe25_noMu") ) { fChain->SetBranchAddress("L2_tau16_loose_xe25_noMu", &L2_tau16_loose_xe25_noMu, &b_L2_tau16_loose_xe25_noMu); }
  if( fChain->GetBranchStatus("L2_tau16_loose_xe25_tight_noMu") ) { fChain->SetBranchAddress("L2_tau16_loose_xe25_tight_noMu", &L2_tau16_loose_xe25_tight_noMu, &b_L2_tau16_loose_xe25_tight_noMu); }
  if( fChain->GetBranchStatus("L2_tau16_loose_xe30_noMu") ) { fChain->SetBranchAddress("L2_tau16_loose_xe30_noMu", &L2_tau16_loose_xe30_noMu, &b_L2_tau16_loose_xe30_noMu); }
  if( fChain->GetBranchStatus("L2_tau16_medium") ) { fChain->SetBranchAddress("L2_tau16_medium", &L2_tau16_medium, &b_L2_tau16_medium); }
  if( fChain->GetBranchStatus("L2_tau16_medium_xe22_noMu") ) { fChain->SetBranchAddress("L2_tau16_medium_xe22_noMu", &L2_tau16_medium_xe22_noMu, &b_L2_tau16_medium_xe22_noMu); }
  if( fChain->GetBranchStatus("L2_tau16_medium_xe25_noMu") ) { fChain->SetBranchAddress("L2_tau16_medium_xe25_noMu", &L2_tau16_medium_xe25_noMu, &b_L2_tau16_medium_xe25_noMu); }
  if( fChain->GetBranchStatus("L2_tau16_medium_xe25_tight_noMu") ) { fChain->SetBranchAddress("L2_tau16_medium_xe25_tight_noMu", &L2_tau16_medium_xe25_tight_noMu, &b_L2_tau16_medium_xe25_tight_noMu); }
  if( fChain->GetBranchStatus("L2_tau20_loose") ) { fChain->SetBranchAddress("L2_tau20_loose", &L2_tau20_loose, &b_L2_tau20_loose); }
  if( fChain->GetBranchStatus("L2_tau20_loose_xe25_noMu") ) { fChain->SetBranchAddress("L2_tau20_loose_xe25_noMu", &L2_tau20_loose_xe25_noMu, &b_L2_tau20_loose_xe25_noMu); }
  if( fChain->GetBranchStatus("L2_tau29_loose") ) { fChain->SetBranchAddress("L2_tau29_loose", &L2_tau29_loose, &b_L2_tau29_loose); }
  if( fChain->GetBranchStatus("L2_tau29_loose1") ) { fChain->SetBranchAddress("L2_tau29_loose1", &L2_tau29_loose1, &b_L2_tau29_loose1); }
  if( fChain->GetBranchStatus("L2_tau38_loose") ) { fChain->SetBranchAddress("L2_tau38_loose", &L2_tau38_loose, &b_L2_tau38_loose); }
  if( fChain->GetBranchStatus("L2_tau38_medium") ) { fChain->SetBranchAddress("L2_tau38_medium", &L2_tau38_medium, &b_L2_tau38_medium); }
  if( fChain->GetBranchStatus("L2_tau50_IDTrkNoCut") ) { fChain->SetBranchAddress("L2_tau50_IDTrkNoCut", &L2_tau50_IDTrkNoCut, &b_L2_tau50_IDTrkNoCut); }
  if( fChain->GetBranchStatus("L2_tau50_loose") ) { fChain->SetBranchAddress("L2_tau50_loose", &L2_tau50_loose, &b_L2_tau50_loose); }
  if( fChain->GetBranchStatus("L2_tau50_loose_IdScan") ) { fChain->SetBranchAddress("L2_tau50_loose_IdScan", &L2_tau50_loose_IdScan, &b_L2_tau50_loose_IdScan); }
  if( fChain->GetBranchStatus("L2_tau50_medium") ) { fChain->SetBranchAddress("L2_tau50_medium", &L2_tau50_medium, &b_L2_tau50_medium); }
  if( fChain->GetBranchStatus("L2_tau5_empty_larcalib") ) { fChain->SetBranchAddress("L2_tau5_empty_larcalib", &L2_tau5_empty_larcalib, &b_L2_tau5_empty_larcalib); }
  if( fChain->GetBranchStatus("L2_tau84_loose") ) { fChain->SetBranchAddress("L2_tau84_loose", &L2_tau84_loose, &b_L2_tau84_loose); }
  if( fChain->GetBranchStatus("L2_tau84_medium") ) { fChain->SetBranchAddress("L2_tau84_medium", &L2_tau84_medium, &b_L2_tau84_medium); }
  if( fChain->GetBranchStatus("L2_tauNoCut") ) { fChain->SetBranchAddress("L2_tauNoCut", &L2_tauNoCut, &b_L2_tauNoCut); }
  if( fChain->GetBranchStatus("L2_tauNoCut_IdScan") ) { fChain->SetBranchAddress("L2_tauNoCut_IdScan", &L2_tauNoCut_IdScan, &b_L2_tauNoCut_IdScan); }
  if( fChain->GetBranchStatus("L2_tauNoCut_cosmic") ) { fChain->SetBranchAddress("L2_tauNoCut_cosmic", &L2_tauNoCut_cosmic, &b_L2_tauNoCut_cosmic); }
  if( fChain->GetBranchStatus("L2_tauNoCut_firstempty") ) { fChain->SetBranchAddress("L2_tauNoCut_firstempty", &L2_tauNoCut_firstempty, &b_L2_tauNoCut_firstempty); }
  if( fChain->GetBranchStatus("L2_tauNoCut_hasTrk6_EFxe15_noMu") ) { fChain->SetBranchAddress("L2_tauNoCut_hasTrk6_EFxe15_noMu", &L2_tauNoCut_hasTrk6_EFxe15_noMu, &b_L2_tauNoCut_hasTrk6_EFxe15_noMu); }
  if( fChain->GetBranchStatus("L2_tauNoCut_hasTrk6_IdScan_EFxe15_noMu") ) { fChain->SetBranchAddress("L2_tauNoCut_hasTrk6_IdScan_EFxe15_noMu", &L2_tauNoCut_hasTrk6_IdScan_EFxe15_noMu, &b_L2_tauNoCut_hasTrk6_IdScan_EFxe15_noMu); }
  if( fChain->GetBranchStatus("L2_tauNoCut_hasTrk6_IdScan_xe20_noMu") ) { fChain->SetBranchAddress("L2_tauNoCut_hasTrk6_IdScan_xe20_noMu", &L2_tauNoCut_hasTrk6_IdScan_xe20_noMu, &b_L2_tauNoCut_hasTrk6_IdScan_xe20_noMu); }
  if( fChain->GetBranchStatus("L2_tauNoCut_hasTrk6_xe20_noMu") ) { fChain->SetBranchAddress("L2_tauNoCut_hasTrk6_xe20_noMu", &L2_tauNoCut_hasTrk6_xe20_noMu, &b_L2_tauNoCut_hasTrk6_xe20_noMu); }
  if( fChain->GetBranchStatus("L2_tauNoCut_hasTrk9_xe20_noMu") ) { fChain->SetBranchAddress("L2_tauNoCut_hasTrk9_xe20_noMu", &L2_tauNoCut_hasTrk9_xe20_noMu, &b_L2_tauNoCut_hasTrk9_xe20_noMu); }
  if( fChain->GetBranchStatus("L2_tauNoCut_hasTrk_MV") ) { fChain->SetBranchAddress("L2_tauNoCut_hasTrk_MV", &L2_tauNoCut_hasTrk_MV, &b_L2_tauNoCut_hasTrk_MV); }
  if( fChain->GetBranchStatus("L2_tauNoCut_hasTrk_e10_tight") ) { fChain->SetBranchAddress("L2_tauNoCut_hasTrk_e10_tight", &L2_tauNoCut_hasTrk_e10_tight, &b_L2_tauNoCut_hasTrk_e10_tight); }
  if( fChain->GetBranchStatus("L2_tauNoCut_hasTrk_xe20_noMu") ) { fChain->SetBranchAddress("L2_tauNoCut_hasTrk_xe20_noMu", &L2_tauNoCut_hasTrk_xe20_noMu, &b_L2_tauNoCut_hasTrk_xe20_noMu); }
  if( fChain->GetBranchStatus("L2_tauNoCut_unpaired_iso") ) { fChain->SetBranchAddress("L2_tauNoCut_unpaired_iso", &L2_tauNoCut_unpaired_iso, &b_L2_tauNoCut_unpaired_iso); }
  if( fChain->GetBranchStatus("L2_tauNoCut_unpaired_noniso") ) { fChain->SetBranchAddress("L2_tauNoCut_unpaired_noniso", &L2_tauNoCut_unpaired_noniso, &b_L2_tauNoCut_unpaired_noniso); }
  if( fChain->GetBranchStatus("L2_xe12") ) { fChain->SetBranchAddress("L2_xe12", &L2_xe12, &b_L2_xe12); }
  if( fChain->GetBranchStatus("L2_xe12_loose") ) { fChain->SetBranchAddress("L2_xe12_loose", &L2_xe12_loose, &b_L2_xe12_loose); }
  if( fChain->GetBranchStatus("L2_xe12_loose_noMu") ) { fChain->SetBranchAddress("L2_xe12_loose_noMu", &L2_xe12_loose_noMu, &b_L2_xe12_loose_noMu); }
  if( fChain->GetBranchStatus("L2_xe12_medium") ) { fChain->SetBranchAddress("L2_xe12_medium", &L2_xe12_medium, &b_L2_xe12_medium); }
  if( fChain->GetBranchStatus("L2_xe12_medium_noMu") ) { fChain->SetBranchAddress("L2_xe12_medium_noMu", &L2_xe12_medium_noMu, &b_L2_xe12_medium_noMu); }
  if( fChain->GetBranchStatus("L2_xe12_noMu") ) { fChain->SetBranchAddress("L2_xe12_noMu", &L2_xe12_noMu, &b_L2_xe12_noMu); }
  if( fChain->GetBranchStatus("L2_xe15") ) { fChain->SetBranchAddress("L2_xe15", &L2_xe15, &b_L2_xe15); }
  if( fChain->GetBranchStatus("L2_xe15_medium") ) { fChain->SetBranchAddress("L2_xe15_medium", &L2_xe15_medium, &b_L2_xe15_medium); }
  if( fChain->GetBranchStatus("L2_xe15_medium_noMu") ) { fChain->SetBranchAddress("L2_xe15_medium_noMu", &L2_xe15_medium_noMu, &b_L2_xe15_medium_noMu); }
  if( fChain->GetBranchStatus("L2_xe15_medium_vfj_noMu") ) { fChain->SetBranchAddress("L2_xe15_medium_vfj_noMu", &L2_xe15_medium_vfj_noMu, &b_L2_xe15_medium_vfj_noMu); }
  if( fChain->GetBranchStatus("L2_xe15_noMu") ) { fChain->SetBranchAddress("L2_xe15_noMu", &L2_xe15_noMu, &b_L2_xe15_noMu); }
  if( fChain->GetBranchStatus("L2_xe15_tight_noMu") ) { fChain->SetBranchAddress("L2_xe15_tight_noMu", &L2_xe15_tight_noMu, &b_L2_xe15_tight_noMu); }
  if( fChain->GetBranchStatus("L2_xe15_tight_vfj_noMu") ) { fChain->SetBranchAddress("L2_xe15_tight_vfj_noMu", &L2_xe15_tight_vfj_noMu, &b_L2_xe15_tight_vfj_noMu); }
  if( fChain->GetBranchStatus("L2_xe15_unbiased_noMu") ) { fChain->SetBranchAddress("L2_xe15_unbiased_noMu", &L2_xe15_unbiased_noMu, &b_L2_xe15_unbiased_noMu); }
  if( fChain->GetBranchStatus("L2_xe15_vfj_noMu") ) { fChain->SetBranchAddress("L2_xe15_vfj_noMu", &L2_xe15_vfj_noMu, &b_L2_xe15_vfj_noMu); }
  if( fChain->GetBranchStatus("L2_xe17_tight_noMu") ) { fChain->SetBranchAddress("L2_xe17_tight_noMu", &L2_xe17_tight_noMu, &b_L2_xe17_tight_noMu); }
  if( fChain->GetBranchStatus("L2_xe17_tight_vfj_noMu") ) { fChain->SetBranchAddress("L2_xe17_tight_vfj_noMu", &L2_xe17_tight_vfj_noMu, &b_L2_xe17_tight_vfj_noMu); }
  if( fChain->GetBranchStatus("L2_xe20") ) { fChain->SetBranchAddress("L2_xe20", &L2_xe20, &b_L2_xe20); }
  if( fChain->GetBranchStatus("L2_xe20_noMu") ) { fChain->SetBranchAddress("L2_xe20_noMu", &L2_xe20_noMu, &b_L2_xe20_noMu); }
  if( fChain->GetBranchStatus("L2_xe20_vfj_noMu") ) { fChain->SetBranchAddress("L2_xe20_vfj_noMu", &L2_xe20_vfj_noMu, &b_L2_xe20_vfj_noMu); }
  if( fChain->GetBranchStatus("L2_xe22_loose_noMu") ) { fChain->SetBranchAddress("L2_xe22_loose_noMu", &L2_xe22_loose_noMu, &b_L2_xe22_loose_noMu); }
  if( fChain->GetBranchStatus("L2_xe22_tight_noMu") ) { fChain->SetBranchAddress("L2_xe22_tight_noMu", &L2_xe22_tight_noMu, &b_L2_xe22_tight_noMu); }
  if( fChain->GetBranchStatus("L2_xe22_tight_vfj_noMu") ) { fChain->SetBranchAddress("L2_xe22_tight_vfj_noMu", &L2_xe22_tight_vfj_noMu, &b_L2_xe22_tight_vfj_noMu); }
  if( fChain->GetBranchStatus("L2_xe25") ) { fChain->SetBranchAddress("L2_xe25", &L2_xe25, &b_L2_xe25); }
  if( fChain->GetBranchStatus("L2_xe25_noMu") ) { fChain->SetBranchAddress("L2_xe25_noMu", &L2_xe25_noMu, &b_L2_xe25_noMu); }
  if( fChain->GetBranchStatus("L2_xe27_tight_noMu") ) { fChain->SetBranchAddress("L2_xe27_tight_noMu", &L2_xe27_tight_noMu, &b_L2_xe27_tight_noMu); }
  if( fChain->GetBranchStatus("L2_xe30") ) { fChain->SetBranchAddress("L2_xe30", &L2_xe30, &b_L2_xe30); }
  if( fChain->GetBranchStatus("L2_xe30_allL1") ) { fChain->SetBranchAddress("L2_xe30_allL1", &L2_xe30_allL1, &b_L2_xe30_allL1); }
  if( fChain->GetBranchStatus("L2_xe30_allL1_FEB") ) { fChain->SetBranchAddress("L2_xe30_allL1_FEB", &L2_xe30_allL1_FEB, &b_L2_xe30_allL1_FEB); }
  if( fChain->GetBranchStatus("L2_xe30_allL1_allCells") ) { fChain->SetBranchAddress("L2_xe30_allL1_allCells", &L2_xe30_allL1_allCells, &b_L2_xe30_allL1_allCells); }
  if( fChain->GetBranchStatus("L2_xe30_allL1_noMu") ) { fChain->SetBranchAddress("L2_xe30_allL1_noMu", &L2_xe30_allL1_noMu, &b_L2_xe30_allL1_noMu); }
  if( fChain->GetBranchStatus("L2_xe30_noMu") ) { fChain->SetBranchAddress("L2_xe30_noMu", &L2_xe30_noMu, &b_L2_xe30_noMu); }
  if( fChain->GetBranchStatus("L2_xe32_tight_noMu") ) { fChain->SetBranchAddress("L2_xe32_tight_noMu", &L2_xe32_tight_noMu, &b_L2_xe32_tight_noMu); }
  if( fChain->GetBranchStatus("L2_xe35") ) { fChain->SetBranchAddress("L2_xe35", &L2_xe35, &b_L2_xe35); }
  if( fChain->GetBranchStatus("L2_xe35_noMu") ) { fChain->SetBranchAddress("L2_xe35_noMu", &L2_xe35_noMu, &b_L2_xe35_noMu); }
  if( fChain->GetBranchStatus("L2_xe40_medium") ) { fChain->SetBranchAddress("L2_xe40_medium", &L2_xe40_medium, &b_L2_xe40_medium); }
  if( fChain->GetBranchStatus("L2_xe40_medium_noMu") ) { fChain->SetBranchAddress("L2_xe40_medium_noMu", &L2_xe40_medium_noMu, &b_L2_xe40_medium_noMu); }
  if( fChain->GetBranchStatus("L2_xe45") ) { fChain->SetBranchAddress("L2_xe45", &L2_xe45, &b_L2_xe45); }
  if( fChain->GetBranchStatus("L2_xe45_noMu") ) { fChain->SetBranchAddress("L2_xe45_noMu", &L2_xe45_noMu, &b_L2_xe45_noMu); }
  if( fChain->GetBranchStatus("L2_xe60_medium") ) { fChain->SetBranchAddress("L2_xe60_medium", &L2_xe60_medium, &b_L2_xe60_medium); }
  if( fChain->GetBranchStatus("L2_xe60_medium_noMu") ) { fChain->SetBranchAddress("L2_xe60_medium_noMu", &L2_xe60_medium_noMu, &b_L2_xe60_medium_noMu); }
  if( fChain->GetBranchStatus("ph_n") ) { fChain->SetBranchAddress("ph_n", &ph_n, &b_ph_n); }
  if( fChain->GetBranchStatus("ph_E") ) { fChain->SetBranchAddress("ph_E", &ph_E, &b_ph_E); }
  if( fChain->GetBranchStatus("ph_Et") ) { fChain->SetBranchAddress("ph_Et", &ph_Et, &b_ph_Et); }
  if( fChain->GetBranchStatus("ph_pt") ) { fChain->SetBranchAddress("ph_pt", &ph_pt, &b_ph_pt); }
  if( fChain->GetBranchStatus("ph_m") ) { fChain->SetBranchAddress("ph_m", &ph_m, &b_ph_m); }
  if( fChain->GetBranchStatus("ph_eta") ) { fChain->SetBranchAddress("ph_eta", &ph_eta, &b_ph_eta); }
  if( fChain->GetBranchStatus("ph_phi") ) { fChain->SetBranchAddress("ph_phi", &ph_phi, &b_ph_phi); }
  if( fChain->GetBranchStatus("ph_px") ) { fChain->SetBranchAddress("ph_px", &ph_px, &b_ph_px); }
  if( fChain->GetBranchStatus("ph_py") ) { fChain->SetBranchAddress("ph_py", &ph_py, &b_ph_py); }
  if( fChain->GetBranchStatus("ph_pz") ) { fChain->SetBranchAddress("ph_pz", &ph_pz, &b_ph_pz); }
  if( fChain->GetBranchStatus("ph_author") ) { fChain->SetBranchAddress("ph_author", &ph_author, &b_ph_author); }
  if( fChain->GetBranchStatus("ph_isRecovered") ) { fChain->SetBranchAddress("ph_isRecovered", &ph_isRecovered, &b_ph_isRecovered); }
  if( fChain->GetBranchStatus("ph_isEM") ) { fChain->SetBranchAddress("ph_isEM", &ph_isEM, &b_ph_isEM); }
  if( fChain->GetBranchStatus("ph_OQ") ) { fChain->SetBranchAddress("ph_OQ", &ph_OQ, &b_ph_OQ); }
  if( fChain->GetBranchStatus("ph_convFlag") ) { fChain->SetBranchAddress("ph_convFlag", &ph_convFlag, &b_ph_convFlag); }
  if( fChain->GetBranchStatus("ph_isConv") ) { fChain->SetBranchAddress("ph_isConv", &ph_isConv, &b_ph_isConv); }
  if( fChain->GetBranchStatus("ph_nConv") ) { fChain->SetBranchAddress("ph_nConv", &ph_nConv, &b_ph_nConv); }
  if( fChain->GetBranchStatus("ph_nSingleTrackConv") ) { fChain->SetBranchAddress("ph_nSingleTrackConv", &ph_nSingleTrackConv, &b_ph_nSingleTrackConv); }
  if( fChain->GetBranchStatus("ph_nDoubleTrackConv") ) { fChain->SetBranchAddress("ph_nDoubleTrackConv", &ph_nDoubleTrackConv, &b_ph_nDoubleTrackConv); }
  if( fChain->GetBranchStatus("ph_type") ) { fChain->SetBranchAddress("ph_type", &ph_type, &b_ph_type); }
  if( fChain->GetBranchStatus("ph_origin") ) { fChain->SetBranchAddress("ph_origin", &ph_origin, &b_ph_origin); }
  if( fChain->GetBranchStatus("ph_truth_deltaRRecPhoton") ) { fChain->SetBranchAddress("ph_truth_deltaRRecPhoton", &ph_truth_deltaRRecPhoton, &b_ph_truth_deltaRRecPhoton); }
  if( fChain->GetBranchStatus("ph_truth_E") ) { fChain->SetBranchAddress("ph_truth_E", &ph_truth_E, &b_ph_truth_E); }
  if( fChain->GetBranchStatus("ph_truth_pt") ) { fChain->SetBranchAddress("ph_truth_pt", &ph_truth_pt, &b_ph_truth_pt); }
  if( fChain->GetBranchStatus("ph_truth_eta") ) { fChain->SetBranchAddress("ph_truth_eta", &ph_truth_eta, &b_ph_truth_eta); }
  if( fChain->GetBranchStatus("ph_truth_phi") ) { fChain->SetBranchAddress("ph_truth_phi", &ph_truth_phi, &b_ph_truth_phi); }
  if( fChain->GetBranchStatus("ph_truth_type") ) { fChain->SetBranchAddress("ph_truth_type", &ph_truth_type, &b_ph_truth_type); }
  if( fChain->GetBranchStatus("ph_truth_status") ) { fChain->SetBranchAddress("ph_truth_status", &ph_truth_status, &b_ph_truth_status); }
  if( fChain->GetBranchStatus("ph_truth_barcode") ) { fChain->SetBranchAddress("ph_truth_barcode", &ph_truth_barcode, &b_ph_truth_barcode); }
  if( fChain->GetBranchStatus("ph_truth_mothertype") ) { fChain->SetBranchAddress("ph_truth_mothertype", &ph_truth_mothertype, &b_ph_truth_mothertype); }
  if( fChain->GetBranchStatus("ph_truth_motherbarcode") ) { fChain->SetBranchAddress("ph_truth_motherbarcode", &ph_truth_motherbarcode, &b_ph_truth_motherbarcode); }
  if( fChain->GetBranchStatus("ph_truth_index") ) { fChain->SetBranchAddress("ph_truth_index", &ph_truth_index, &b_ph_truth_index); }
  if( fChain->GetBranchStatus("ph_truth_matched") ) { fChain->SetBranchAddress("ph_truth_matched", &ph_truth_matched, &b_ph_truth_matched); }
  if( fChain->GetBranchStatus("ph_loose") ) { fChain->SetBranchAddress("ph_loose", &ph_loose, &b_ph_loose); }
  if( fChain->GetBranchStatus("ph_tight") ) { fChain->SetBranchAddress("ph_tight", &ph_tight, &b_ph_tight); }
  if( fChain->GetBranchStatus("ph_tightIso") ) { fChain->SetBranchAddress("ph_tightIso", &ph_tightIso, &b_ph_tightIso); }
  if( fChain->GetBranchStatus("ph_goodOQ") ) { fChain->SetBranchAddress("ph_goodOQ", &ph_goodOQ, &b_ph_goodOQ); }
  if( fChain->GetBranchStatus("ph_Ethad") ) { fChain->SetBranchAddress("ph_Ethad", &ph_Ethad, &b_ph_Ethad); }
  if( fChain->GetBranchStatus("ph_Ethad1") ) { fChain->SetBranchAddress("ph_Ethad1", &ph_Ethad1, &b_ph_Ethad1); }
  if( fChain->GetBranchStatus("ph_E033") ) { fChain->SetBranchAddress("ph_E033", &ph_E033, &b_ph_E033); }
  if( fChain->GetBranchStatus("ph_f1") ) { fChain->SetBranchAddress("ph_f1", &ph_f1, &b_ph_f1); }
  if( fChain->GetBranchStatus("ph_f1core") ) { fChain->SetBranchAddress("ph_f1core", &ph_f1core, &b_ph_f1core); }
  if( fChain->GetBranchStatus("ph_Emins1") ) { fChain->SetBranchAddress("ph_Emins1", &ph_Emins1, &b_ph_Emins1); }
  if( fChain->GetBranchStatus("ph_fside") ) { fChain->SetBranchAddress("ph_fside", &ph_fside, &b_ph_fside); }
  if( fChain->GetBranchStatus("ph_Emax2") ) { fChain->SetBranchAddress("ph_Emax2", &ph_Emax2, &b_ph_Emax2); }
  if( fChain->GetBranchStatus("ph_ws3") ) { fChain->SetBranchAddress("ph_ws3", &ph_ws3, &b_ph_ws3); }
  if( fChain->GetBranchStatus("ph_wstot") ) { fChain->SetBranchAddress("ph_wstot", &ph_wstot, &b_ph_wstot); }
  if( fChain->GetBranchStatus("ph_E132") ) { fChain->SetBranchAddress("ph_E132", &ph_E132, &b_ph_E132); }
  if( fChain->GetBranchStatus("ph_E1152") ) { fChain->SetBranchAddress("ph_E1152", &ph_E1152, &b_ph_E1152); }
  if( fChain->GetBranchStatus("ph_emaxs1") ) { fChain->SetBranchAddress("ph_emaxs1", &ph_emaxs1, &b_ph_emaxs1); }
  if( fChain->GetBranchStatus("ph_deltaEs") ) { fChain->SetBranchAddress("ph_deltaEs", &ph_deltaEs, &b_ph_deltaEs); }
  if( fChain->GetBranchStatus("ph_E233") ) { fChain->SetBranchAddress("ph_E233", &ph_E233, &b_ph_E233); }
  if( fChain->GetBranchStatus("ph_E237") ) { fChain->SetBranchAddress("ph_E237", &ph_E237, &b_ph_E237); }
  if( fChain->GetBranchStatus("ph_E277") ) { fChain->SetBranchAddress("ph_E277", &ph_E277, &b_ph_E277); }
  if( fChain->GetBranchStatus("ph_weta2") ) { fChain->SetBranchAddress("ph_weta2", &ph_weta2, &b_ph_weta2); }
  if( fChain->GetBranchStatus("ph_f3") ) { fChain->SetBranchAddress("ph_f3", &ph_f3, &b_ph_f3); }
  if( fChain->GetBranchStatus("ph_f3core") ) { fChain->SetBranchAddress("ph_f3core", &ph_f3core, &b_ph_f3core); }
  if( fChain->GetBranchStatus("ph_rphiallcalo") ) { fChain->SetBranchAddress("ph_rphiallcalo", &ph_rphiallcalo, &b_ph_rphiallcalo); }
  if( fChain->GetBranchStatus("ph_Etcone45") ) { fChain->SetBranchAddress("ph_Etcone45", &ph_Etcone45, &b_ph_Etcone45); }
  if( fChain->GetBranchStatus("ph_Etcone20") ) { fChain->SetBranchAddress("ph_Etcone20", &ph_Etcone20, &b_ph_Etcone20); }
  if( fChain->GetBranchStatus("ph_Etcone30") ) { fChain->SetBranchAddress("ph_Etcone30", &ph_Etcone30, &b_ph_Etcone30); }
  if( fChain->GetBranchStatus("ph_Etcone40") ) { fChain->SetBranchAddress("ph_Etcone40", &ph_Etcone40, &b_ph_Etcone40); }
  if( fChain->GetBranchStatus("ph_ptcone20") ) { fChain->SetBranchAddress("ph_ptcone20", &ph_ptcone20, &b_ph_ptcone20); }
  if( fChain->GetBranchStatus("ph_ptcone30") ) { fChain->SetBranchAddress("ph_ptcone30", &ph_ptcone30, &b_ph_ptcone30); }
  if( fChain->GetBranchStatus("ph_ptcone40") ) { fChain->SetBranchAddress("ph_ptcone40", &ph_ptcone40, &b_ph_ptcone40); }
  if( fChain->GetBranchStatus("ph_nucone20") ) { fChain->SetBranchAddress("ph_nucone20", &ph_nucone20, &b_ph_nucone20); }
  if( fChain->GetBranchStatus("ph_nucone30") ) { fChain->SetBranchAddress("ph_nucone30", &ph_nucone30, &b_ph_nucone30); }
  if( fChain->GetBranchStatus("ph_nucone40") ) { fChain->SetBranchAddress("ph_nucone40", &ph_nucone40, &b_ph_nucone40); }
  if( fChain->GetBranchStatus("ph_convanglematch") ) { fChain->SetBranchAddress("ph_convanglematch", &ph_convanglematch, &b_ph_convanglematch); }
  if( fChain->GetBranchStatus("ph_convtrackmatch") ) { fChain->SetBranchAddress("ph_convtrackmatch", &ph_convtrackmatch, &b_ph_convtrackmatch); }
  if( fChain->GetBranchStatus("ph_hasconv") ) { fChain->SetBranchAddress("ph_hasconv", &ph_hasconv, &b_ph_hasconv); }
  if( fChain->GetBranchStatus("ph_convvtxx") ) { fChain->SetBranchAddress("ph_convvtxx", &ph_convvtxx, &b_ph_convvtxx); }
  if( fChain->GetBranchStatus("ph_convvtxy") ) { fChain->SetBranchAddress("ph_convvtxy", &ph_convvtxy, &b_ph_convvtxy); }
  if( fChain->GetBranchStatus("ph_convvtxz") ) { fChain->SetBranchAddress("ph_convvtxz", &ph_convvtxz, &b_ph_convvtxz); }
  if( fChain->GetBranchStatus("ph_Rconv") ) { fChain->SetBranchAddress("ph_Rconv", &ph_Rconv, &b_ph_Rconv); }
  if( fChain->GetBranchStatus("ph_zconv") ) { fChain->SetBranchAddress("ph_zconv", &ph_zconv, &b_ph_zconv); }
  if( fChain->GetBranchStatus("ph_convvtxchi2") ) { fChain->SetBranchAddress("ph_convvtxchi2", &ph_convvtxchi2, &b_ph_convvtxchi2); }
  if( fChain->GetBranchStatus("ph_pt1conv") ) { fChain->SetBranchAddress("ph_pt1conv", &ph_pt1conv, &b_ph_pt1conv); }
  if( fChain->GetBranchStatus("ph_convtrk1nBLHits") ) { fChain->SetBranchAddress("ph_convtrk1nBLHits", &ph_convtrk1nBLHits, &b_ph_convtrk1nBLHits); }
  if( fChain->GetBranchStatus("ph_convtrk1nPixHits") ) { fChain->SetBranchAddress("ph_convtrk1nPixHits", &ph_convtrk1nPixHits, &b_ph_convtrk1nPixHits); }
  if( fChain->GetBranchStatus("ph_convtrk1nSCTHits") ) { fChain->SetBranchAddress("ph_convtrk1nSCTHits", &ph_convtrk1nSCTHits, &b_ph_convtrk1nSCTHits); }
  if( fChain->GetBranchStatus("ph_convtrk1nTRTHits") ) { fChain->SetBranchAddress("ph_convtrk1nTRTHits", &ph_convtrk1nTRTHits, &b_ph_convtrk1nTRTHits); }
  if( fChain->GetBranchStatus("ph_pt2conv") ) { fChain->SetBranchAddress("ph_pt2conv", &ph_pt2conv, &b_ph_pt2conv); }
  if( fChain->GetBranchStatus("ph_convtrk2nBLHits") ) { fChain->SetBranchAddress("ph_convtrk2nBLHits", &ph_convtrk2nBLHits, &b_ph_convtrk2nBLHits); }
  if( fChain->GetBranchStatus("ph_convtrk2nPixHits") ) { fChain->SetBranchAddress("ph_convtrk2nPixHits", &ph_convtrk2nPixHits, &b_ph_convtrk2nPixHits); }
  if( fChain->GetBranchStatus("ph_convtrk2nSCTHits") ) { fChain->SetBranchAddress("ph_convtrk2nSCTHits", &ph_convtrk2nSCTHits, &b_ph_convtrk2nSCTHits); }
  if( fChain->GetBranchStatus("ph_convtrk2nTRTHits") ) { fChain->SetBranchAddress("ph_convtrk2nTRTHits", &ph_convtrk2nTRTHits, &b_ph_convtrk2nTRTHits); }
  if( fChain->GetBranchStatus("ph_ptconv") ) { fChain->SetBranchAddress("ph_ptconv", &ph_ptconv, &b_ph_ptconv); }
  if( fChain->GetBranchStatus("ph_pzconv") ) { fChain->SetBranchAddress("ph_pzconv", &ph_pzconv, &b_ph_pzconv); }
  if( fChain->GetBranchStatus("ph_reta") ) { fChain->SetBranchAddress("ph_reta", &ph_reta, &b_ph_reta); }
  if( fChain->GetBranchStatus("ph_rphi") ) { fChain->SetBranchAddress("ph_rphi", &ph_rphi, &b_ph_rphi); }
  if( fChain->GetBranchStatus("ph_EtringnoisedR03sig2") ) { fChain->SetBranchAddress("ph_EtringnoisedR03sig2", &ph_EtringnoisedR03sig2, &b_ph_EtringnoisedR03sig2); }
  if( fChain->GetBranchStatus("ph_EtringnoisedR03sig3") ) { fChain->SetBranchAddress("ph_EtringnoisedR03sig3", &ph_EtringnoisedR03sig3, &b_ph_EtringnoisedR03sig3); }
  if( fChain->GetBranchStatus("ph_EtringnoisedR03sig4") ) { fChain->SetBranchAddress("ph_EtringnoisedR03sig4", &ph_EtringnoisedR03sig4, &b_ph_EtringnoisedR03sig4); }
  if( fChain->GetBranchStatus("ph_isolationlikelihoodjets") ) { fChain->SetBranchAddress("ph_isolationlikelihoodjets", &ph_isolationlikelihoodjets, &b_ph_isolationlikelihoodjets); }
  if( fChain->GetBranchStatus("ph_isolationlikelihoodhqelectrons") ) { fChain->SetBranchAddress("ph_isolationlikelihoodhqelectrons", &ph_isolationlikelihoodhqelectrons, &b_ph_isolationlikelihoodhqelectrons); }
  if( fChain->GetBranchStatus("ph_loglikelihood") ) { fChain->SetBranchAddress("ph_loglikelihood", &ph_loglikelihood, &b_ph_loglikelihood); }
  if( fChain->GetBranchStatus("ph_photonweight") ) { fChain->SetBranchAddress("ph_photonweight", &ph_photonweight, &b_ph_photonweight); }
  if( fChain->GetBranchStatus("ph_photonbgweight") ) { fChain->SetBranchAddress("ph_photonbgweight", &ph_photonbgweight, &b_ph_photonbgweight); }
  if( fChain->GetBranchStatus("ph_neuralnet") ) { fChain->SetBranchAddress("ph_neuralnet", &ph_neuralnet, &b_ph_neuralnet); }
  if( fChain->GetBranchStatus("ph_Hmatrix") ) { fChain->SetBranchAddress("ph_Hmatrix", &ph_Hmatrix, &b_ph_Hmatrix); }
  if( fChain->GetBranchStatus("ph_Hmatrix5") ) { fChain->SetBranchAddress("ph_Hmatrix5", &ph_Hmatrix5, &b_ph_Hmatrix5); }
  if( fChain->GetBranchStatus("ph_adaboost") ) { fChain->SetBranchAddress("ph_adaboost", &ph_adaboost, &b_ph_adaboost); }
  if( fChain->GetBranchStatus("ph_zvertex") ) { fChain->SetBranchAddress("ph_zvertex", &ph_zvertex, &b_ph_zvertex); }
  if( fChain->GetBranchStatus("ph_errz") ) { fChain->SetBranchAddress("ph_errz", &ph_errz, &b_ph_errz); }
  if( fChain->GetBranchStatus("ph_etap") ) { fChain->SetBranchAddress("ph_etap", &ph_etap, &b_ph_etap); }
  if( fChain->GetBranchStatus("ph_depth") ) { fChain->SetBranchAddress("ph_depth", &ph_depth, &b_ph_depth); }
  if( fChain->GetBranchStatus("ph_cl_E") ) { fChain->SetBranchAddress("ph_cl_E", &ph_cl_E, &b_ph_cl_E); }
  if( fChain->GetBranchStatus("ph_cl_pt") ) { fChain->SetBranchAddress("ph_cl_pt", &ph_cl_pt, &b_ph_cl_pt); }
  if( fChain->GetBranchStatus("ph_cl_eta") ) { fChain->SetBranchAddress("ph_cl_eta", &ph_cl_eta, &b_ph_cl_eta); }
  if( fChain->GetBranchStatus("ph_cl_phi") ) { fChain->SetBranchAddress("ph_cl_phi", &ph_cl_phi, &b_ph_cl_phi); }
  if( fChain->GetBranchStatus("ph_Es0") ) { fChain->SetBranchAddress("ph_Es0", &ph_Es0, &b_ph_Es0); }
  if( fChain->GetBranchStatus("ph_etas0") ) { fChain->SetBranchAddress("ph_etas0", &ph_etas0, &b_ph_etas0); }
  if( fChain->GetBranchStatus("ph_phis0") ) { fChain->SetBranchAddress("ph_phis0", &ph_phis0, &b_ph_phis0); }
  if( fChain->GetBranchStatus("ph_Es1") ) { fChain->SetBranchAddress("ph_Es1", &ph_Es1, &b_ph_Es1); }
  if( fChain->GetBranchStatus("ph_etas1") ) { fChain->SetBranchAddress("ph_etas1", &ph_etas1, &b_ph_etas1); }
  if( fChain->GetBranchStatus("ph_phis1") ) { fChain->SetBranchAddress("ph_phis1", &ph_phis1, &b_ph_phis1); }
  if( fChain->GetBranchStatus("ph_Es2") ) { fChain->SetBranchAddress("ph_Es2", &ph_Es2, &b_ph_Es2); }
  if( fChain->GetBranchStatus("ph_etas2") ) { fChain->SetBranchAddress("ph_etas2", &ph_etas2, &b_ph_etas2); }
  if( fChain->GetBranchStatus("ph_phis2") ) { fChain->SetBranchAddress("ph_phis2", &ph_phis2, &b_ph_phis2); }
  if( fChain->GetBranchStatus("ph_Es3") ) { fChain->SetBranchAddress("ph_Es3", &ph_Es3, &b_ph_Es3); }
  if( fChain->GetBranchStatus("ph_etas3") ) { fChain->SetBranchAddress("ph_etas3", &ph_etas3, &b_ph_etas3); }
  if( fChain->GetBranchStatus("ph_phis3") ) { fChain->SetBranchAddress("ph_phis3", &ph_phis3, &b_ph_phis3); }
  if( fChain->GetBranchStatus("ph_rawcl_Es0") ) { fChain->SetBranchAddress("ph_rawcl_Es0", &ph_rawcl_Es0, &b_ph_rawcl_Es0); }
  if( fChain->GetBranchStatus("ph_rawcl_etas0") ) { fChain->SetBranchAddress("ph_rawcl_etas0", &ph_rawcl_etas0, &b_ph_rawcl_etas0); }
  if( fChain->GetBranchStatus("ph_rawcl_phis0") ) { fChain->SetBranchAddress("ph_rawcl_phis0", &ph_rawcl_phis0, &b_ph_rawcl_phis0); }
  if( fChain->GetBranchStatus("ph_rawcl_Es1") ) { fChain->SetBranchAddress("ph_rawcl_Es1", &ph_rawcl_Es1, &b_ph_rawcl_Es1); }
  if( fChain->GetBranchStatus("ph_rawcl_etas1") ) { fChain->SetBranchAddress("ph_rawcl_etas1", &ph_rawcl_etas1, &b_ph_rawcl_etas1); }
  if( fChain->GetBranchStatus("ph_rawcl_phis1") ) { fChain->SetBranchAddress("ph_rawcl_phis1", &ph_rawcl_phis1, &b_ph_rawcl_phis1); }
  if( fChain->GetBranchStatus("ph_rawcl_Es2") ) { fChain->SetBranchAddress("ph_rawcl_Es2", &ph_rawcl_Es2, &b_ph_rawcl_Es2); }
  if( fChain->GetBranchStatus("ph_rawcl_etas2") ) { fChain->SetBranchAddress("ph_rawcl_etas2", &ph_rawcl_etas2, &b_ph_rawcl_etas2); }
  if( fChain->GetBranchStatus("ph_rawcl_phis2") ) { fChain->SetBranchAddress("ph_rawcl_phis2", &ph_rawcl_phis2, &b_ph_rawcl_phis2); }
  if( fChain->GetBranchStatus("ph_rawcl_Es3") ) { fChain->SetBranchAddress("ph_rawcl_Es3", &ph_rawcl_Es3, &b_ph_rawcl_Es3); }
  if( fChain->GetBranchStatus("ph_rawcl_etas3") ) { fChain->SetBranchAddress("ph_rawcl_etas3", &ph_rawcl_etas3, &b_ph_rawcl_etas3); }
  if( fChain->GetBranchStatus("ph_rawcl_phis3") ) { fChain->SetBranchAddress("ph_rawcl_phis3", &ph_rawcl_phis3, &b_ph_rawcl_phis3); }
  if( fChain->GetBranchStatus("ph_rawcl_E") ) { fChain->SetBranchAddress("ph_rawcl_E", &ph_rawcl_E, &b_ph_rawcl_E); }
  if( fChain->GetBranchStatus("ph_rawcl_pt") ) { fChain->SetBranchAddress("ph_rawcl_pt", &ph_rawcl_pt, &b_ph_rawcl_pt); }
  if( fChain->GetBranchStatus("ph_rawcl_eta") ) { fChain->SetBranchAddress("ph_rawcl_eta", &ph_rawcl_eta, &b_ph_rawcl_eta); }
  if( fChain->GetBranchStatus("ph_rawcl_phi") ) { fChain->SetBranchAddress("ph_rawcl_phi", &ph_rawcl_phi, &b_ph_rawcl_phi); }
  if( fChain->GetBranchStatus("ph_truth_isConv") ) { fChain->SetBranchAddress("ph_truth_isConv", &ph_truth_isConv, &b_ph_truth_isConv); }
  if( fChain->GetBranchStatus("ph_truth_isBrem") ) { fChain->SetBranchAddress("ph_truth_isBrem", &ph_truth_isBrem, &b_ph_truth_isBrem); }
  if( fChain->GetBranchStatus("ph_truth_isFromHardProc") ) { fChain->SetBranchAddress("ph_truth_isFromHardProc", &ph_truth_isFromHardProc, &b_ph_truth_isFromHardProc); }
  if( fChain->GetBranchStatus("ph_truth_isPhotonFromHardProc") ) { fChain->SetBranchAddress("ph_truth_isPhotonFromHardProc", &ph_truth_isPhotonFromHardProc, &b_ph_truth_isPhotonFromHardProc); }
  if( fChain->GetBranchStatus("ph_truth_Rconv") ) { fChain->SetBranchAddress("ph_truth_Rconv", &ph_truth_Rconv, &b_ph_truth_Rconv); }
  if( fChain->GetBranchStatus("ph_truth_zconv") ) { fChain->SetBranchAddress("ph_truth_zconv", &ph_truth_zconv, &b_ph_truth_zconv); }
  if( fChain->GetBranchStatus("ph_deltaEmax2") ) { fChain->SetBranchAddress("ph_deltaEmax2", &ph_deltaEmax2, &b_ph_deltaEmax2); }
  if( fChain->GetBranchStatus("ph_calibHitsShowerDepth") ) { fChain->SetBranchAddress("ph_calibHitsShowerDepth", &ph_calibHitsShowerDepth, &b_ph_calibHitsShowerDepth); }
  if( fChain->GetBranchStatus("ph_isIso") ) { fChain->SetBranchAddress("ph_isIso", &ph_isIso, &b_ph_isIso); }
  if( fChain->GetBranchStatus("ph_mvaptcone20") ) { fChain->SetBranchAddress("ph_mvaptcone20", &ph_mvaptcone20, &b_ph_mvaptcone20); }
  if( fChain->GetBranchStatus("ph_mvaptcone30") ) { fChain->SetBranchAddress("ph_mvaptcone30", &ph_mvaptcone30, &b_ph_mvaptcone30); }
  if( fChain->GetBranchStatus("ph_mvaptcone40") ) { fChain->SetBranchAddress("ph_mvaptcone40", &ph_mvaptcone40, &b_ph_mvaptcone40); }
  if( fChain->GetBranchStatus("ph_topoEtcone20") ) { fChain->SetBranchAddress("ph_topoEtcone20", &ph_topoEtcone20, &b_ph_topoEtcone20); }
  if( fChain->GetBranchStatus("ph_topoEtcone40") ) { fChain->SetBranchAddress("ph_topoEtcone40", &ph_topoEtcone40, &b_ph_topoEtcone40); }
  if( fChain->GetBranchStatus("ph_topoEtcone60") ) { fChain->SetBranchAddress("ph_topoEtcone60", &ph_topoEtcone60, &b_ph_topoEtcone60); }
  if( fChain->GetBranchStatus("ph_jet_dr") ) { fChain->SetBranchAddress("ph_jet_dr", &ph_jet_dr, &b_ph_jet_dr); }
  if( fChain->GetBranchStatus("ph_jet_E") ) { fChain->SetBranchAddress("ph_jet_E", &ph_jet_E, &b_ph_jet_E); }
  if( fChain->GetBranchStatus("ph_jet_pt") ) { fChain->SetBranchAddress("ph_jet_pt", &ph_jet_pt, &b_ph_jet_pt); }
  if( fChain->GetBranchStatus("ph_jet_m") ) { fChain->SetBranchAddress("ph_jet_m", &ph_jet_m, &b_ph_jet_m); }
  if( fChain->GetBranchStatus("ph_jet_eta") ) { fChain->SetBranchAddress("ph_jet_eta", &ph_jet_eta, &b_ph_jet_eta); }
  if( fChain->GetBranchStatus("ph_jet_phi") ) { fChain->SetBranchAddress("ph_jet_phi", &ph_jet_phi, &b_ph_jet_phi); }
  if( fChain->GetBranchStatus("ph_jet_truth_dr") ) { fChain->SetBranchAddress("ph_jet_truth_dr", &ph_jet_truth_dr, &b_ph_jet_truth_dr); }
  if( fChain->GetBranchStatus("ph_jet_truth_E") ) { fChain->SetBranchAddress("ph_jet_truth_E", &ph_jet_truth_E, &b_ph_jet_truth_E); }
  if( fChain->GetBranchStatus("ph_jet_truth_pt") ) { fChain->SetBranchAddress("ph_jet_truth_pt", &ph_jet_truth_pt, &b_ph_jet_truth_pt); }
  if( fChain->GetBranchStatus("ph_jet_truth_m") ) { fChain->SetBranchAddress("ph_jet_truth_m", &ph_jet_truth_m, &b_ph_jet_truth_m); }
  if( fChain->GetBranchStatus("ph_jet_truth_eta") ) { fChain->SetBranchAddress("ph_jet_truth_eta", &ph_jet_truth_eta, &b_ph_jet_truth_eta); }
  if( fChain->GetBranchStatus("ph_jet_truth_phi") ) { fChain->SetBranchAddress("ph_jet_truth_phi", &ph_jet_truth_phi, &b_ph_jet_truth_phi); }
  if( fChain->GetBranchStatus("ph_jet_truth_matched") ) { fChain->SetBranchAddress("ph_jet_truth_matched", &ph_jet_truth_matched, &b_ph_jet_truth_matched); }
  if( fChain->GetBranchStatus("ph_jet_matched") ) { fChain->SetBranchAddress("ph_jet_matched", &ph_jet_matched, &b_ph_jet_matched); }
  if( fChain->GetBranchStatus("ph_convIP") ) { fChain->SetBranchAddress("ph_convIP", &ph_convIP, &b_ph_convIP); }
  if( fChain->GetBranchStatus("ph_convIPRev") ) { fChain->SetBranchAddress("ph_convIPRev", &ph_convIPRev, &b_ph_convIPRev); }
  if( fChain->GetBranchStatus("ph_ptIsolationCone") ) { fChain->SetBranchAddress("ph_ptIsolationCone", &ph_ptIsolationCone, &b_ph_ptIsolationCone); }
  if( fChain->GetBranchStatus("ph_ptIsolationConePhAngle") ) { fChain->SetBranchAddress("ph_ptIsolationConePhAngle", &ph_ptIsolationConePhAngle, &b_ph_ptIsolationConePhAngle); }
  if( fChain->GetBranchStatus("ph_Etcone40_pt_corrected") ) { fChain->SetBranchAddress("ph_Etcone40_pt_corrected", &ph_Etcone40_pt_corrected, &b_ph_Etcone40_pt_corrected); }
  if( fChain->GetBranchStatus("ph_Etcone40_ED_corrected") ) { fChain->SetBranchAddress("ph_Etcone40_ED_corrected", &ph_Etcone40_ED_corrected, &b_ph_Etcone40_ED_corrected); }
  if( fChain->GetBranchStatus("ph_Etcone40_corrected") ) { fChain->SetBranchAddress("ph_Etcone40_corrected", &ph_Etcone40_corrected, &b_ph_Etcone40_corrected); }
  if( fChain->GetBranchStatus("ph_topodr") ) { fChain->SetBranchAddress("ph_topodr", &ph_topodr, &b_ph_topodr); }
  if( fChain->GetBranchStatus("ph_topopt") ) { fChain->SetBranchAddress("ph_topopt", &ph_topopt, &b_ph_topopt); }
  if( fChain->GetBranchStatus("ph_topoeta") ) { fChain->SetBranchAddress("ph_topoeta", &ph_topoeta, &b_ph_topoeta); }
  if( fChain->GetBranchStatus("ph_topophi") ) { fChain->SetBranchAddress("ph_topophi", &ph_topophi, &b_ph_topophi); }
  if( fChain->GetBranchStatus("ph_topomatched") ) { fChain->SetBranchAddress("ph_topomatched", &ph_topomatched, &b_ph_topomatched); }
  if( fChain->GetBranchStatus("ph_topoEMdr") ) { fChain->SetBranchAddress("ph_topoEMdr", &ph_topoEMdr, &b_ph_topoEMdr); }
  if( fChain->GetBranchStatus("ph_topoEMpt") ) { fChain->SetBranchAddress("ph_topoEMpt", &ph_topoEMpt, &b_ph_topoEMpt); }
  if( fChain->GetBranchStatus("ph_topoEMeta") ) { fChain->SetBranchAddress("ph_topoEMeta", &ph_topoEMeta, &b_ph_topoEMeta); }
  if( fChain->GetBranchStatus("ph_topoEMphi") ) { fChain->SetBranchAddress("ph_topoEMphi", &ph_topoEMphi, &b_ph_topoEMphi); }
  if( fChain->GetBranchStatus("ph_topoEMmatched") ) { fChain->SetBranchAddress("ph_topoEMmatched", &ph_topoEMmatched, &b_ph_topoEMmatched); }
  if( fChain->GetBranchStatus("ph_EF_dr") ) { fChain->SetBranchAddress("ph_EF_dr", &ph_EF_dr, &b_ph_EF_dr); }
  if( fChain->GetBranchStatus("ph_EF_index") ) { fChain->SetBranchAddress("ph_EF_index", &ph_EF_index, &b_ph_EF_index); }
  if( fChain->GetBranchStatus("ph_L2_dr") ) { fChain->SetBranchAddress("ph_L2_dr", &ph_L2_dr, &b_ph_L2_dr); }
  if( fChain->GetBranchStatus("ph_L2_index") ) { fChain->SetBranchAddress("ph_L2_index", &ph_L2_index, &b_ph_L2_index); }
  if( fChain->GetBranchStatus("ph_L1_dr") ) { fChain->SetBranchAddress("ph_L1_dr", &ph_L1_dr, &b_ph_L1_dr); }
  if( fChain->GetBranchStatus("ph_L1_index") ) { fChain->SetBranchAddress("ph_L1_index", &ph_L1_index, &b_ph_L1_index); }
  if( fChain->GetBranchStatus("mu_muid_n") ) { fChain->SetBranchAddress("mu_muid_n", &mu_muid_n, &b_mu_muid_n); }
  if( fChain->GetBranchStatus("mu_muid_E") ) { fChain->SetBranchAddress("mu_muid_E", &mu_muid_E, &b_mu_muid_E); }
  if( fChain->GetBranchStatus("mu_muid_pt") ) { fChain->SetBranchAddress("mu_muid_pt", &mu_muid_pt, &b_mu_muid_pt); }
  if( fChain->GetBranchStatus("mu_muid_m") ) { fChain->SetBranchAddress("mu_muid_m", &mu_muid_m, &b_mu_muid_m); }
  if( fChain->GetBranchStatus("mu_muid_eta") ) { fChain->SetBranchAddress("mu_muid_eta", &mu_muid_eta, &b_mu_muid_eta); }
  if( fChain->GetBranchStatus("mu_muid_phi") ) { fChain->SetBranchAddress("mu_muid_phi", &mu_muid_phi, &b_mu_muid_phi); }
  if( fChain->GetBranchStatus("mu_muid_px") ) { fChain->SetBranchAddress("mu_muid_px", &mu_muid_px, &b_mu_muid_px); }
  if( fChain->GetBranchStatus("mu_muid_py") ) { fChain->SetBranchAddress("mu_muid_py", &mu_muid_py, &b_mu_muid_py); }
  if( fChain->GetBranchStatus("mu_muid_pz") ) { fChain->SetBranchAddress("mu_muid_pz", &mu_muid_pz, &b_mu_muid_pz); }
  if( fChain->GetBranchStatus("mu_muid_charge") ) { fChain->SetBranchAddress("mu_muid_charge", &mu_muid_charge, &b_mu_muid_charge); }
  if( fChain->GetBranchStatus("mu_muid_allauthor") ) { fChain->SetBranchAddress("mu_muid_allauthor", &mu_muid_allauthor, &b_mu_muid_allauthor); }
  if( fChain->GetBranchStatus("mu_muid_author") ) { fChain->SetBranchAddress("mu_muid_author", &mu_muid_author, &b_mu_muid_author); }
  if( fChain->GetBranchStatus("mu_muid_beta") ) { fChain->SetBranchAddress("mu_muid_beta", &mu_muid_beta, &b_mu_muid_beta); }
  if( fChain->GetBranchStatus("mu_muid_isMuonLikelihood") ) { fChain->SetBranchAddress("mu_muid_isMuonLikelihood", &mu_muid_isMuonLikelihood, &b_mu_muid_isMuonLikelihood); }
  if( fChain->GetBranchStatus("mu_muid_matchchi2") ) { fChain->SetBranchAddress("mu_muid_matchchi2", &mu_muid_matchchi2, &b_mu_muid_matchchi2); }
  if( fChain->GetBranchStatus("mu_muid_matchndof") ) { fChain->SetBranchAddress("mu_muid_matchndof", &mu_muid_matchndof, &b_mu_muid_matchndof); }
  if( fChain->GetBranchStatus("mu_muid_etcone20") ) { fChain->SetBranchAddress("mu_muid_etcone20", &mu_muid_etcone20, &b_mu_muid_etcone20); }
  if( fChain->GetBranchStatus("mu_muid_etcone30") ) { fChain->SetBranchAddress("mu_muid_etcone30", &mu_muid_etcone30, &b_mu_muid_etcone30); }
  if( fChain->GetBranchStatus("mu_muid_etcone40") ) { fChain->SetBranchAddress("mu_muid_etcone40", &mu_muid_etcone40, &b_mu_muid_etcone40); }
  if( fChain->GetBranchStatus("mu_muid_nucone20") ) { fChain->SetBranchAddress("mu_muid_nucone20", &mu_muid_nucone20, &b_mu_muid_nucone20); }
  if( fChain->GetBranchStatus("mu_muid_nucone30") ) { fChain->SetBranchAddress("mu_muid_nucone30", &mu_muid_nucone30, &b_mu_muid_nucone30); }
  if( fChain->GetBranchStatus("mu_muid_nucone40") ) { fChain->SetBranchAddress("mu_muid_nucone40", &mu_muid_nucone40, &b_mu_muid_nucone40); }
  if( fChain->GetBranchStatus("mu_muid_ptcone20") ) { fChain->SetBranchAddress("mu_muid_ptcone20", &mu_muid_ptcone20, &b_mu_muid_ptcone20); }
  if( fChain->GetBranchStatus("mu_muid_ptcone30") ) { fChain->SetBranchAddress("mu_muid_ptcone30", &mu_muid_ptcone30, &b_mu_muid_ptcone30); }
  if( fChain->GetBranchStatus("mu_muid_ptcone40") ) { fChain->SetBranchAddress("mu_muid_ptcone40", &mu_muid_ptcone40, &b_mu_muid_ptcone40); }
  if( fChain->GetBranchStatus("mu_muid_energyLossPar") ) { fChain->SetBranchAddress("mu_muid_energyLossPar", &mu_muid_energyLossPar, &b_mu_muid_energyLossPar); }
  if( fChain->GetBranchStatus("mu_muid_energyLossErr") ) { fChain->SetBranchAddress("mu_muid_energyLossErr", &mu_muid_energyLossErr, &b_mu_muid_energyLossErr); }
  if( fChain->GetBranchStatus("mu_muid_etCore") ) { fChain->SetBranchAddress("mu_muid_etCore", &mu_muid_etCore, &b_mu_muid_etCore); }
  if( fChain->GetBranchStatus("mu_muid_energyLossType") ) { fChain->SetBranchAddress("mu_muid_energyLossType", &mu_muid_energyLossType, &b_mu_muid_energyLossType); }
  if( fChain->GetBranchStatus("mu_muid_caloMuonIdTag") ) { fChain->SetBranchAddress("mu_muid_caloMuonIdTag", &mu_muid_caloMuonIdTag, &b_mu_muid_caloMuonIdTag); }
  if( fChain->GetBranchStatus("mu_muid_caloLRLikelihood") ) { fChain->SetBranchAddress("mu_muid_caloLRLikelihood", &mu_muid_caloLRLikelihood, &b_mu_muid_caloLRLikelihood); }
  if( fChain->GetBranchStatus("mu_muid_bestMatch") ) { fChain->SetBranchAddress("mu_muid_bestMatch", &mu_muid_bestMatch, &b_mu_muid_bestMatch); }
  if( fChain->GetBranchStatus("mu_muid_isStandAloneMuon") ) { fChain->SetBranchAddress("mu_muid_isStandAloneMuon", &mu_muid_isStandAloneMuon, &b_mu_muid_isStandAloneMuon); }
  if( fChain->GetBranchStatus("mu_muid_isCombinedMuon") ) { fChain->SetBranchAddress("mu_muid_isCombinedMuon", &mu_muid_isCombinedMuon, &b_mu_muid_isCombinedMuon); }
  if( fChain->GetBranchStatus("mu_muid_isLowPtReconstructedMuon") ) { fChain->SetBranchAddress("mu_muid_isLowPtReconstructedMuon", &mu_muid_isLowPtReconstructedMuon, &b_mu_muid_isLowPtReconstructedMuon); }
  if( fChain->GetBranchStatus("mu_muid_loose") ) { fChain->SetBranchAddress("mu_muid_loose", &mu_muid_loose, &b_mu_muid_loose); }
  if( fChain->GetBranchStatus("mu_muid_medium") ) { fChain->SetBranchAddress("mu_muid_medium", &mu_muid_medium, &b_mu_muid_medium); }
  if( fChain->GetBranchStatus("mu_muid_tight") ) { fChain->SetBranchAddress("mu_muid_tight", &mu_muid_tight, &b_mu_muid_tight); }
  if( fChain->GetBranchStatus("mu_muid_d0_exPV") ) { fChain->SetBranchAddress("mu_muid_d0_exPV", &mu_muid_d0_exPV, &b_mu_muid_d0_exPV); }
  if( fChain->GetBranchStatus("mu_muid_z0_exPV") ) { fChain->SetBranchAddress("mu_muid_z0_exPV", &mu_muid_z0_exPV, &b_mu_muid_z0_exPV); }
  if( fChain->GetBranchStatus("mu_muid_phi_exPV") ) { fChain->SetBranchAddress("mu_muid_phi_exPV", &mu_muid_phi_exPV, &b_mu_muid_phi_exPV); }
  if( fChain->GetBranchStatus("mu_muid_theta_exPV") ) { fChain->SetBranchAddress("mu_muid_theta_exPV", &mu_muid_theta_exPV, &b_mu_muid_theta_exPV); }
  if( fChain->GetBranchStatus("mu_muid_qoverp_exPV") ) { fChain->SetBranchAddress("mu_muid_qoverp_exPV", &mu_muid_qoverp_exPV, &b_mu_muid_qoverp_exPV); }
  if( fChain->GetBranchStatus("mu_muid_cb_d0_exPV") ) { fChain->SetBranchAddress("mu_muid_cb_d0_exPV", &mu_muid_cb_d0_exPV, &b_mu_muid_cb_d0_exPV); }
  if( fChain->GetBranchStatus("mu_muid_cb_z0_exPV") ) { fChain->SetBranchAddress("mu_muid_cb_z0_exPV", &mu_muid_cb_z0_exPV, &b_mu_muid_cb_z0_exPV); }
  if( fChain->GetBranchStatus("mu_muid_cb_phi_exPV") ) { fChain->SetBranchAddress("mu_muid_cb_phi_exPV", &mu_muid_cb_phi_exPV, &b_mu_muid_cb_phi_exPV); }
  if( fChain->GetBranchStatus("mu_muid_cb_theta_exPV") ) { fChain->SetBranchAddress("mu_muid_cb_theta_exPV", &mu_muid_cb_theta_exPV, &b_mu_muid_cb_theta_exPV); }
  if( fChain->GetBranchStatus("mu_muid_cb_qoverp_exPV") ) { fChain->SetBranchAddress("mu_muid_cb_qoverp_exPV", &mu_muid_cb_qoverp_exPV, &b_mu_muid_cb_qoverp_exPV); }
  if( fChain->GetBranchStatus("mu_muid_id_d0_exPV") ) { fChain->SetBranchAddress("mu_muid_id_d0_exPV", &mu_muid_id_d0_exPV, &b_mu_muid_id_d0_exPV); }
  if( fChain->GetBranchStatus("mu_muid_id_z0_exPV") ) { fChain->SetBranchAddress("mu_muid_id_z0_exPV", &mu_muid_id_z0_exPV, &b_mu_muid_id_z0_exPV); }
  if( fChain->GetBranchStatus("mu_muid_id_phi_exPV") ) { fChain->SetBranchAddress("mu_muid_id_phi_exPV", &mu_muid_id_phi_exPV, &b_mu_muid_id_phi_exPV); }
  if( fChain->GetBranchStatus("mu_muid_id_theta_exPV") ) { fChain->SetBranchAddress("mu_muid_id_theta_exPV", &mu_muid_id_theta_exPV, &b_mu_muid_id_theta_exPV); }
  if( fChain->GetBranchStatus("mu_muid_id_qoverp_exPV") ) { fChain->SetBranchAddress("mu_muid_id_qoverp_exPV", &mu_muid_id_qoverp_exPV, &b_mu_muid_id_qoverp_exPV); }
  if( fChain->GetBranchStatus("mu_muid_me_d0_exPV") ) { fChain->SetBranchAddress("mu_muid_me_d0_exPV", &mu_muid_me_d0_exPV, &b_mu_muid_me_d0_exPV); }
  if( fChain->GetBranchStatus("mu_muid_me_z0_exPV") ) { fChain->SetBranchAddress("mu_muid_me_z0_exPV", &mu_muid_me_z0_exPV, &b_mu_muid_me_z0_exPV); }
  if( fChain->GetBranchStatus("mu_muid_me_phi_exPV") ) { fChain->SetBranchAddress("mu_muid_me_phi_exPV", &mu_muid_me_phi_exPV, &b_mu_muid_me_phi_exPV); }
  if( fChain->GetBranchStatus("mu_muid_me_theta_exPV") ) { fChain->SetBranchAddress("mu_muid_me_theta_exPV", &mu_muid_me_theta_exPV, &b_mu_muid_me_theta_exPV); }
  if( fChain->GetBranchStatus("mu_muid_me_qoverp_exPV") ) { fChain->SetBranchAddress("mu_muid_me_qoverp_exPV", &mu_muid_me_qoverp_exPV, &b_mu_muid_me_qoverp_exPV); }
  if( fChain->GetBranchStatus("mu_muid_ie_d0_exPV") ) { fChain->SetBranchAddress("mu_muid_ie_d0_exPV", &mu_muid_ie_d0_exPV, &b_mu_muid_ie_d0_exPV); }
  if( fChain->GetBranchStatus("mu_muid_ie_z0_exPV") ) { fChain->SetBranchAddress("mu_muid_ie_z0_exPV", &mu_muid_ie_z0_exPV, &b_mu_muid_ie_z0_exPV); }
  if( fChain->GetBranchStatus("mu_muid_ie_phi_exPV") ) { fChain->SetBranchAddress("mu_muid_ie_phi_exPV", &mu_muid_ie_phi_exPV, &b_mu_muid_ie_phi_exPV); }
  if( fChain->GetBranchStatus("mu_muid_ie_theta_exPV") ) { fChain->SetBranchAddress("mu_muid_ie_theta_exPV", &mu_muid_ie_theta_exPV, &b_mu_muid_ie_theta_exPV); }
  if( fChain->GetBranchStatus("mu_muid_ie_qoverp_exPV") ) { fChain->SetBranchAddress("mu_muid_ie_qoverp_exPV", &mu_muid_ie_qoverp_exPV, &b_mu_muid_ie_qoverp_exPV); }
  if( fChain->GetBranchStatus("mu_muid_SpaceTime_detID") ) { fChain->SetBranchAddress("mu_muid_SpaceTime_detID", &mu_muid_SpaceTime_detID, &b_mu_muid_SpaceTime_detID); }
  if( fChain->GetBranchStatus("mu_muid_SpaceTime_t") ) { fChain->SetBranchAddress("mu_muid_SpaceTime_t", &mu_muid_SpaceTime_t, &b_mu_muid_SpaceTime_t); }
  if( fChain->GetBranchStatus("mu_muid_SpaceTime_tError") ) { fChain->SetBranchAddress("mu_muid_SpaceTime_tError", &mu_muid_SpaceTime_tError, &b_mu_muid_SpaceTime_tError); }
  if( fChain->GetBranchStatus("mu_muid_SpaceTime_weight") ) { fChain->SetBranchAddress("mu_muid_SpaceTime_weight", &mu_muid_SpaceTime_weight, &b_mu_muid_SpaceTime_weight); }
  if( fChain->GetBranchStatus("mu_muid_SpaceTime_x") ) { fChain->SetBranchAddress("mu_muid_SpaceTime_x", &mu_muid_SpaceTime_x, &b_mu_muid_SpaceTime_x); }
  if( fChain->GetBranchStatus("mu_muid_SpaceTime_y") ) { fChain->SetBranchAddress("mu_muid_SpaceTime_y", &mu_muid_SpaceTime_y, &b_mu_muid_SpaceTime_y); }
  if( fChain->GetBranchStatus("mu_muid_SpaceTime_z") ) { fChain->SetBranchAddress("mu_muid_SpaceTime_z", &mu_muid_SpaceTime_z, &b_mu_muid_SpaceTime_z); }
  if( fChain->GetBranchStatus("mu_muid_SpaceTime_t_Tile") ) { fChain->SetBranchAddress("mu_muid_SpaceTime_t_Tile", &mu_muid_SpaceTime_t_Tile, &b_mu_muid_SpaceTime_t_Tile); }
  if( fChain->GetBranchStatus("mu_muid_SpaceTime_tError_Tile") ) { fChain->SetBranchAddress("mu_muid_SpaceTime_tError_Tile", &mu_muid_SpaceTime_tError_Tile, &b_mu_muid_SpaceTime_tError_Tile); }
  if( fChain->GetBranchStatus("mu_muid_SpaceTime_weight_Tile") ) { fChain->SetBranchAddress("mu_muid_SpaceTime_weight_Tile", &mu_muid_SpaceTime_weight_Tile, &b_mu_muid_SpaceTime_weight_Tile); }
  if( fChain->GetBranchStatus("mu_muid_SpaceTime_x_Tile") ) { fChain->SetBranchAddress("mu_muid_SpaceTime_x_Tile", &mu_muid_SpaceTime_x_Tile, &b_mu_muid_SpaceTime_x_Tile); }
  if( fChain->GetBranchStatus("mu_muid_SpaceTime_y_Tile") ) { fChain->SetBranchAddress("mu_muid_SpaceTime_y_Tile", &mu_muid_SpaceTime_y_Tile, &b_mu_muid_SpaceTime_y_Tile); }
  if( fChain->GetBranchStatus("mu_muid_SpaceTime_z_Tile") ) { fChain->SetBranchAddress("mu_muid_SpaceTime_z_Tile", &mu_muid_SpaceTime_z_Tile, &b_mu_muid_SpaceTime_z_Tile); }
  if( fChain->GetBranchStatus("mu_muid_SpaceTime_t_TRT") ) { fChain->SetBranchAddress("mu_muid_SpaceTime_t_TRT", &mu_muid_SpaceTime_t_TRT, &b_mu_muid_SpaceTime_t_TRT); }
  if( fChain->GetBranchStatus("mu_muid_SpaceTime_tError_TRT") ) { fChain->SetBranchAddress("mu_muid_SpaceTime_tError_TRT", &mu_muid_SpaceTime_tError_TRT, &b_mu_muid_SpaceTime_tError_TRT); }
  if( fChain->GetBranchStatus("mu_muid_SpaceTime_weight_TRT") ) { fChain->SetBranchAddress("mu_muid_SpaceTime_weight_TRT", &mu_muid_SpaceTime_weight_TRT, &b_mu_muid_SpaceTime_weight_TRT); }
  if( fChain->GetBranchStatus("mu_muid_SpaceTime_x_TRT") ) { fChain->SetBranchAddress("mu_muid_SpaceTime_x_TRT", &mu_muid_SpaceTime_x_TRT, &b_mu_muid_SpaceTime_x_TRT); }
  if( fChain->GetBranchStatus("mu_muid_SpaceTime_y_TRT") ) { fChain->SetBranchAddress("mu_muid_SpaceTime_y_TRT", &mu_muid_SpaceTime_y_TRT, &b_mu_muid_SpaceTime_y_TRT); }
  if( fChain->GetBranchStatus("mu_muid_SpaceTime_z_TRT") ) { fChain->SetBranchAddress("mu_muid_SpaceTime_z_TRT", &mu_muid_SpaceTime_z_TRT, &b_mu_muid_SpaceTime_z_TRT); }
  if( fChain->GetBranchStatus("mu_muid_SpaceTime_t_MDT") ) { fChain->SetBranchAddress("mu_muid_SpaceTime_t_MDT", &mu_muid_SpaceTime_t_MDT, &b_mu_muid_SpaceTime_t_MDT); }
  if( fChain->GetBranchStatus("mu_muid_SpaceTime_tError_MDT") ) { fChain->SetBranchAddress("mu_muid_SpaceTime_tError_MDT", &mu_muid_SpaceTime_tError_MDT, &b_mu_muid_SpaceTime_tError_MDT); }
  if( fChain->GetBranchStatus("mu_muid_SpaceTime_weight_MDT") ) { fChain->SetBranchAddress("mu_muid_SpaceTime_weight_MDT", &mu_muid_SpaceTime_weight_MDT, &b_mu_muid_SpaceTime_weight_MDT); }
  if( fChain->GetBranchStatus("mu_muid_SpaceTime_x_MDT") ) { fChain->SetBranchAddress("mu_muid_SpaceTime_x_MDT", &mu_muid_SpaceTime_x_MDT, &b_mu_muid_SpaceTime_x_MDT); }
  if( fChain->GetBranchStatus("mu_muid_SpaceTime_y_MDT") ) { fChain->SetBranchAddress("mu_muid_SpaceTime_y_MDT", &mu_muid_SpaceTime_y_MDT, &b_mu_muid_SpaceTime_y_MDT); }
  if( fChain->GetBranchStatus("mu_muid_SpaceTime_z_MDT") ) { fChain->SetBranchAddress("mu_muid_SpaceTime_z_MDT", &mu_muid_SpaceTime_z_MDT, &b_mu_muid_SpaceTime_z_MDT); }
  if( fChain->GetBranchStatus("mu_muid_TileCellEnergyLayer1") ) { fChain->SetBranchAddress("mu_muid_TileCellEnergyLayer1", &mu_muid_TileCellEnergyLayer1, &b_mu_muid_TileCellEnergyLayer1); }
  if( fChain->GetBranchStatus("mu_muid_TileTimeLayer1") ) { fChain->SetBranchAddress("mu_muid_TileTimeLayer1", &mu_muid_TileTimeLayer1, &b_mu_muid_TileTimeLayer1); }
  if( fChain->GetBranchStatus("mu_muid_TileCellRmsNoiseLayer1") ) { fChain->SetBranchAddress("mu_muid_TileCellRmsNoiseLayer1", &mu_muid_TileCellRmsNoiseLayer1, &b_mu_muid_TileCellRmsNoiseLayer1); }
  if( fChain->GetBranchStatus("mu_muid_TileCellSignLayer1") ) { fChain->SetBranchAddress("mu_muid_TileCellSignLayer1", &mu_muid_TileCellSignLayer1, &b_mu_muid_TileCellSignLayer1); }
  if( fChain->GetBranchStatus("mu_muid_TileCellEnergyLayer2") ) { fChain->SetBranchAddress("mu_muid_TileCellEnergyLayer2", &mu_muid_TileCellEnergyLayer2, &b_mu_muid_TileCellEnergyLayer2); }
  if( fChain->GetBranchStatus("mu_muid_TileTimeLayer2") ) { fChain->SetBranchAddress("mu_muid_TileTimeLayer2", &mu_muid_TileTimeLayer2, &b_mu_muid_TileTimeLayer2); }
  if( fChain->GetBranchStatus("mu_muid_TileCellRmsNoiseLayer2") ) { fChain->SetBranchAddress("mu_muid_TileCellRmsNoiseLayer2", &mu_muid_TileCellRmsNoiseLayer2, &b_mu_muid_TileCellRmsNoiseLayer2); }
  if( fChain->GetBranchStatus("mu_muid_TileCellSignLayer2") ) { fChain->SetBranchAddress("mu_muid_TileCellSignLayer2", &mu_muid_TileCellSignLayer2, &b_mu_muid_TileCellSignLayer2); }
  if( fChain->GetBranchStatus("mu_muid_TileCellEnergyLayer3") ) { fChain->SetBranchAddress("mu_muid_TileCellEnergyLayer3", &mu_muid_TileCellEnergyLayer3, &b_mu_muid_TileCellEnergyLayer3); }
  if( fChain->GetBranchStatus("mu_muid_TileTimeLayer3") ) { fChain->SetBranchAddress("mu_muid_TileTimeLayer3", &mu_muid_TileTimeLayer3, &b_mu_muid_TileTimeLayer3); }
  if( fChain->GetBranchStatus("mu_muid_TileCellRmsNoiseLayer3") ) { fChain->SetBranchAddress("mu_muid_TileCellRmsNoiseLayer3", &mu_muid_TileCellRmsNoiseLayer3, &b_mu_muid_TileCellRmsNoiseLayer3); }
  if( fChain->GetBranchStatus("mu_muid_TileCellSignLayer3") ) { fChain->SetBranchAddress("mu_muid_TileCellSignLayer3", &mu_muid_TileCellSignLayer3, &b_mu_muid_TileCellSignLayer3); }
  if( fChain->GetBranchStatus("mu_muid_MSTrkT0_1") ) { fChain->SetBranchAddress("mu_muid_MSTrkT0_1", &mu_muid_MSTrkT0_1, &b_mu_muid_MSTrkT0_1); }
  if( fChain->GetBranchStatus("mu_muid_MSTrkT0_2") ) { fChain->SetBranchAddress("mu_muid_MSTrkT0_2", &mu_muid_MSTrkT0_2, &b_mu_muid_MSTrkT0_2); }
  if( fChain->GetBranchStatus("mu_muid_MSTrkT0_3") ) { fChain->SetBranchAddress("mu_muid_MSTrkT0_3", &mu_muid_MSTrkT0_3, &b_mu_muid_MSTrkT0_3); }
  if( fChain->GetBranchStatus("mu_muid_cov_d0_exPV") ) { fChain->SetBranchAddress("mu_muid_cov_d0_exPV", &mu_muid_cov_d0_exPV, &b_mu_muid_cov_d0_exPV); }
  if( fChain->GetBranchStatus("mu_muid_cov_z0_exPV") ) { fChain->SetBranchAddress("mu_muid_cov_z0_exPV", &mu_muid_cov_z0_exPV, &b_mu_muid_cov_z0_exPV); }
  if( fChain->GetBranchStatus("mu_muid_cov_phi_exPV") ) { fChain->SetBranchAddress("mu_muid_cov_phi_exPV", &mu_muid_cov_phi_exPV, &b_mu_muid_cov_phi_exPV); }
  if( fChain->GetBranchStatus("mu_muid_cov_theta_exPV") ) { fChain->SetBranchAddress("mu_muid_cov_theta_exPV", &mu_muid_cov_theta_exPV, &b_mu_muid_cov_theta_exPV); }
  if( fChain->GetBranchStatus("mu_muid_cov_qoverp_exPV") ) { fChain->SetBranchAddress("mu_muid_cov_qoverp_exPV", &mu_muid_cov_qoverp_exPV, &b_mu_muid_cov_qoverp_exPV); }
  if( fChain->GetBranchStatus("mu_muid_cov_d0_z0_exPV") ) { fChain->SetBranchAddress("mu_muid_cov_d0_z0_exPV", &mu_muid_cov_d0_z0_exPV, &b_mu_muid_cov_d0_z0_exPV); }
  if( fChain->GetBranchStatus("mu_muid_cov_d0_phi_exPV") ) { fChain->SetBranchAddress("mu_muid_cov_d0_phi_exPV", &mu_muid_cov_d0_phi_exPV, &b_mu_muid_cov_d0_phi_exPV); }
  if( fChain->GetBranchStatus("mu_muid_cov_d0_theta_exPV") ) { fChain->SetBranchAddress("mu_muid_cov_d0_theta_exPV", &mu_muid_cov_d0_theta_exPV, &b_mu_muid_cov_d0_theta_exPV); }
  if( fChain->GetBranchStatus("mu_muid_cov_d0_qoverp_exPV") ) { fChain->SetBranchAddress("mu_muid_cov_d0_qoverp_exPV", &mu_muid_cov_d0_qoverp_exPV, &b_mu_muid_cov_d0_qoverp_exPV); }
  if( fChain->GetBranchStatus("mu_muid_cov_z0_phi_exPV") ) { fChain->SetBranchAddress("mu_muid_cov_z0_phi_exPV", &mu_muid_cov_z0_phi_exPV, &b_mu_muid_cov_z0_phi_exPV); }
  if( fChain->GetBranchStatus("mu_muid_cov_z0_theta_exPV") ) { fChain->SetBranchAddress("mu_muid_cov_z0_theta_exPV", &mu_muid_cov_z0_theta_exPV, &b_mu_muid_cov_z0_theta_exPV); }
  if( fChain->GetBranchStatus("mu_muid_cov_z0_qoverp_exPV") ) { fChain->SetBranchAddress("mu_muid_cov_z0_qoverp_exPV", &mu_muid_cov_z0_qoverp_exPV, &b_mu_muid_cov_z0_qoverp_exPV); }
  if( fChain->GetBranchStatus("mu_muid_cov_phi_theta_exPV") ) { fChain->SetBranchAddress("mu_muid_cov_phi_theta_exPV", &mu_muid_cov_phi_theta_exPV, &b_mu_muid_cov_phi_theta_exPV); }
  if( fChain->GetBranchStatus("mu_muid_cov_phi_qoverp_exPV") ) { fChain->SetBranchAddress("mu_muid_cov_phi_qoverp_exPV", &mu_muid_cov_phi_qoverp_exPV, &b_mu_muid_cov_phi_qoverp_exPV); }
  if( fChain->GetBranchStatus("mu_muid_cov_theta_qoverp_exPV") ) { fChain->SetBranchAddress("mu_muid_cov_theta_qoverp_exPV", &mu_muid_cov_theta_qoverp_exPV, &b_mu_muid_cov_theta_qoverp_exPV); }
  if( fChain->GetBranchStatus("mu_muid_id_cov_d0_exPV") ) { fChain->SetBranchAddress("mu_muid_id_cov_d0_exPV", &mu_muid_id_cov_d0_exPV, &b_mu_muid_id_cov_d0_exPV); }
  if( fChain->GetBranchStatus("mu_muid_id_cov_z0_exPV") ) { fChain->SetBranchAddress("mu_muid_id_cov_z0_exPV", &mu_muid_id_cov_z0_exPV, &b_mu_muid_id_cov_z0_exPV); }
  if( fChain->GetBranchStatus("mu_muid_id_cov_phi_exPV") ) { fChain->SetBranchAddress("mu_muid_id_cov_phi_exPV", &mu_muid_id_cov_phi_exPV, &b_mu_muid_id_cov_phi_exPV); }
  if( fChain->GetBranchStatus("mu_muid_id_cov_theta_exPV") ) { fChain->SetBranchAddress("mu_muid_id_cov_theta_exPV", &mu_muid_id_cov_theta_exPV, &b_mu_muid_id_cov_theta_exPV); }
  if( fChain->GetBranchStatus("mu_muid_id_cov_qoverp_exPV") ) { fChain->SetBranchAddress("mu_muid_id_cov_qoverp_exPV", &mu_muid_id_cov_qoverp_exPV, &b_mu_muid_id_cov_qoverp_exPV); }
  if( fChain->GetBranchStatus("mu_muid_id_cov_d0_z0_exPV") ) { fChain->SetBranchAddress("mu_muid_id_cov_d0_z0_exPV", &mu_muid_id_cov_d0_z0_exPV, &b_mu_muid_id_cov_d0_z0_exPV); }
  if( fChain->GetBranchStatus("mu_muid_id_cov_d0_phi_exPV") ) { fChain->SetBranchAddress("mu_muid_id_cov_d0_phi_exPV", &mu_muid_id_cov_d0_phi_exPV, &b_mu_muid_id_cov_d0_phi_exPV); }
  if( fChain->GetBranchStatus("mu_muid_id_cov_d0_theta_exPV") ) { fChain->SetBranchAddress("mu_muid_id_cov_d0_theta_exPV", &mu_muid_id_cov_d0_theta_exPV, &b_mu_muid_id_cov_d0_theta_exPV); }
  if( fChain->GetBranchStatus("mu_muid_id_cov_d0_qoverp_exPV") ) { fChain->SetBranchAddress("mu_muid_id_cov_d0_qoverp_exPV", &mu_muid_id_cov_d0_qoverp_exPV, &b_mu_muid_id_cov_d0_qoverp_exPV); }
  if( fChain->GetBranchStatus("mu_muid_id_cov_z0_phi_exPV") ) { fChain->SetBranchAddress("mu_muid_id_cov_z0_phi_exPV", &mu_muid_id_cov_z0_phi_exPV, &b_mu_muid_id_cov_z0_phi_exPV); }
  if( fChain->GetBranchStatus("mu_muid_id_cov_z0_theta_exPV") ) { fChain->SetBranchAddress("mu_muid_id_cov_z0_theta_exPV", &mu_muid_id_cov_z0_theta_exPV, &b_mu_muid_id_cov_z0_theta_exPV); }
  if( fChain->GetBranchStatus("mu_muid_id_cov_z0_qoverp_exPV") ) { fChain->SetBranchAddress("mu_muid_id_cov_z0_qoverp_exPV", &mu_muid_id_cov_z0_qoverp_exPV, &b_mu_muid_id_cov_z0_qoverp_exPV); }
  if( fChain->GetBranchStatus("mu_muid_id_cov_phi_theta_exPV") ) { fChain->SetBranchAddress("mu_muid_id_cov_phi_theta_exPV", &mu_muid_id_cov_phi_theta_exPV, &b_mu_muid_id_cov_phi_theta_exPV); }
  if( fChain->GetBranchStatus("mu_muid_id_cov_phi_qoverp_exPV") ) { fChain->SetBranchAddress("mu_muid_id_cov_phi_qoverp_exPV", &mu_muid_id_cov_phi_qoverp_exPV, &b_mu_muid_id_cov_phi_qoverp_exPV); }
  if( fChain->GetBranchStatus("mu_muid_id_cov_theta_qoverp_exPV") ) { fChain->SetBranchAddress("mu_muid_id_cov_theta_qoverp_exPV", &mu_muid_id_cov_theta_qoverp_exPV, &b_mu_muid_id_cov_theta_qoverp_exPV); }
  if( fChain->GetBranchStatus("mu_muid_me_cov_d0_exPV") ) { fChain->SetBranchAddress("mu_muid_me_cov_d0_exPV", &mu_muid_me_cov_d0_exPV, &b_mu_muid_me_cov_d0_exPV); }
  if( fChain->GetBranchStatus("mu_muid_me_cov_z0_exPV") ) { fChain->SetBranchAddress("mu_muid_me_cov_z0_exPV", &mu_muid_me_cov_z0_exPV, &b_mu_muid_me_cov_z0_exPV); }
  if( fChain->GetBranchStatus("mu_muid_me_cov_phi_exPV") ) { fChain->SetBranchAddress("mu_muid_me_cov_phi_exPV", &mu_muid_me_cov_phi_exPV, &b_mu_muid_me_cov_phi_exPV); }
  if( fChain->GetBranchStatus("mu_muid_me_cov_theta_exPV") ) { fChain->SetBranchAddress("mu_muid_me_cov_theta_exPV", &mu_muid_me_cov_theta_exPV, &b_mu_muid_me_cov_theta_exPV); }
  if( fChain->GetBranchStatus("mu_muid_me_cov_qoverp_exPV") ) { fChain->SetBranchAddress("mu_muid_me_cov_qoverp_exPV", &mu_muid_me_cov_qoverp_exPV, &b_mu_muid_me_cov_qoverp_exPV); }
  if( fChain->GetBranchStatus("mu_muid_me_cov_d0_z0_exPV") ) { fChain->SetBranchAddress("mu_muid_me_cov_d0_z0_exPV", &mu_muid_me_cov_d0_z0_exPV, &b_mu_muid_me_cov_d0_z0_exPV); }
  if( fChain->GetBranchStatus("mu_muid_me_cov_d0_phi_exPV") ) { fChain->SetBranchAddress("mu_muid_me_cov_d0_phi_exPV", &mu_muid_me_cov_d0_phi_exPV, &b_mu_muid_me_cov_d0_phi_exPV); }
  if( fChain->GetBranchStatus("mu_muid_me_cov_d0_theta_exPV") ) { fChain->SetBranchAddress("mu_muid_me_cov_d0_theta_exPV", &mu_muid_me_cov_d0_theta_exPV, &b_mu_muid_me_cov_d0_theta_exPV); }
  if( fChain->GetBranchStatus("mu_muid_me_cov_d0_qoverp_exPV") ) { fChain->SetBranchAddress("mu_muid_me_cov_d0_qoverp_exPV", &mu_muid_me_cov_d0_qoverp_exPV, &b_mu_muid_me_cov_d0_qoverp_exPV); }
  if( fChain->GetBranchStatus("mu_muid_me_cov_z0_phi_exPV") ) { fChain->SetBranchAddress("mu_muid_me_cov_z0_phi_exPV", &mu_muid_me_cov_z0_phi_exPV, &b_mu_muid_me_cov_z0_phi_exPV); }
  if( fChain->GetBranchStatus("mu_muid_me_cov_z0_theta_exPV") ) { fChain->SetBranchAddress("mu_muid_me_cov_z0_theta_exPV", &mu_muid_me_cov_z0_theta_exPV, &b_mu_muid_me_cov_z0_theta_exPV); }
  if( fChain->GetBranchStatus("mu_muid_me_cov_z0_qoverp_exPV") ) { fChain->SetBranchAddress("mu_muid_me_cov_z0_qoverp_exPV", &mu_muid_me_cov_z0_qoverp_exPV, &b_mu_muid_me_cov_z0_qoverp_exPV); }
  if( fChain->GetBranchStatus("mu_muid_me_cov_phi_theta_exPV") ) { fChain->SetBranchAddress("mu_muid_me_cov_phi_theta_exPV", &mu_muid_me_cov_phi_theta_exPV, &b_mu_muid_me_cov_phi_theta_exPV); }
  if( fChain->GetBranchStatus("mu_muid_me_cov_phi_qoverp_exPV") ) { fChain->SetBranchAddress("mu_muid_me_cov_phi_qoverp_exPV", &mu_muid_me_cov_phi_qoverp_exPV, &b_mu_muid_me_cov_phi_qoverp_exPV); }
  if( fChain->GetBranchStatus("mu_muid_me_cov_theta_qoverp_exPV") ) { fChain->SetBranchAddress("mu_muid_me_cov_theta_qoverp_exPV", &mu_muid_me_cov_theta_qoverp_exPV, &b_mu_muid_me_cov_theta_qoverp_exPV); }
  if( fChain->GetBranchStatus("mu_muid_ms_d0") ) { fChain->SetBranchAddress("mu_muid_ms_d0", &mu_muid_ms_d0, &b_mu_muid_ms_d0); }
  if( fChain->GetBranchStatus("mu_muid_ms_z0") ) { fChain->SetBranchAddress("mu_muid_ms_z0", &mu_muid_ms_z0, &b_mu_muid_ms_z0); }
  if( fChain->GetBranchStatus("mu_muid_ms_phi") ) { fChain->SetBranchAddress("mu_muid_ms_phi", &mu_muid_ms_phi, &b_mu_muid_ms_phi); }
  if( fChain->GetBranchStatus("mu_muid_ms_theta") ) { fChain->SetBranchAddress("mu_muid_ms_theta", &mu_muid_ms_theta, &b_mu_muid_ms_theta); }
  if( fChain->GetBranchStatus("mu_muid_ms_qoverp") ) { fChain->SetBranchAddress("mu_muid_ms_qoverp", &mu_muid_ms_qoverp, &b_mu_muid_ms_qoverp); }
  if( fChain->GetBranchStatus("mu_muid_id_d0") ) { fChain->SetBranchAddress("mu_muid_id_d0", &mu_muid_id_d0, &b_mu_muid_id_d0); }
  if( fChain->GetBranchStatus("mu_muid_id_z0") ) { fChain->SetBranchAddress("mu_muid_id_z0", &mu_muid_id_z0, &b_mu_muid_id_z0); }
  if( fChain->GetBranchStatus("mu_muid_id_phi") ) { fChain->SetBranchAddress("mu_muid_id_phi", &mu_muid_id_phi, &b_mu_muid_id_phi); }
  if( fChain->GetBranchStatus("mu_muid_id_theta") ) { fChain->SetBranchAddress("mu_muid_id_theta", &mu_muid_id_theta, &b_mu_muid_id_theta); }
  if( fChain->GetBranchStatus("mu_muid_id_qoverp") ) { fChain->SetBranchAddress("mu_muid_id_qoverp", &mu_muid_id_qoverp, &b_mu_muid_id_qoverp); }
  if( fChain->GetBranchStatus("mu_muid_me_d0") ) { fChain->SetBranchAddress("mu_muid_me_d0", &mu_muid_me_d0, &b_mu_muid_me_d0); }
  if( fChain->GetBranchStatus("mu_muid_me_z0") ) { fChain->SetBranchAddress("mu_muid_me_z0", &mu_muid_me_z0, &b_mu_muid_me_z0); }
  if( fChain->GetBranchStatus("mu_muid_me_phi") ) { fChain->SetBranchAddress("mu_muid_me_phi", &mu_muid_me_phi, &b_mu_muid_me_phi); }
  if( fChain->GetBranchStatus("mu_muid_me_theta") ) { fChain->SetBranchAddress("mu_muid_me_theta", &mu_muid_me_theta, &b_mu_muid_me_theta); }
  if( fChain->GetBranchStatus("mu_muid_me_qoverp") ) { fChain->SetBranchAddress("mu_muid_me_qoverp", &mu_muid_me_qoverp, &b_mu_muid_me_qoverp); }
  if( fChain->GetBranchStatus("mu_muid_ie_d0") ) { fChain->SetBranchAddress("mu_muid_ie_d0", &mu_muid_ie_d0, &b_mu_muid_ie_d0); }
  if( fChain->GetBranchStatus("mu_muid_ie_z0") ) { fChain->SetBranchAddress("mu_muid_ie_z0", &mu_muid_ie_z0, &b_mu_muid_ie_z0); }
  if( fChain->GetBranchStatus("mu_muid_ie_phi") ) { fChain->SetBranchAddress("mu_muid_ie_phi", &mu_muid_ie_phi, &b_mu_muid_ie_phi); }
  if( fChain->GetBranchStatus("mu_muid_ie_theta") ) { fChain->SetBranchAddress("mu_muid_ie_theta", &mu_muid_ie_theta, &b_mu_muid_ie_theta); }
  if( fChain->GetBranchStatus("mu_muid_ie_qoverp") ) { fChain->SetBranchAddress("mu_muid_ie_qoverp", &mu_muid_ie_qoverp, &b_mu_muid_ie_qoverp); }
  if( fChain->GetBranchStatus("mu_muid_nOutliersOnTrack") ) { fChain->SetBranchAddress("mu_muid_nOutliersOnTrack", &mu_muid_nOutliersOnTrack, &b_mu_muid_nOutliersOnTrack); }
  if( fChain->GetBranchStatus("mu_muid_nBLHits") ) { fChain->SetBranchAddress("mu_muid_nBLHits", &mu_muid_nBLHits, &b_mu_muid_nBLHits); }
  if( fChain->GetBranchStatus("mu_muid_nPixHits") ) { fChain->SetBranchAddress("mu_muid_nPixHits", &mu_muid_nPixHits, &b_mu_muid_nPixHits); }
  if( fChain->GetBranchStatus("mu_muid_nSCTHits") ) { fChain->SetBranchAddress("mu_muid_nSCTHits", &mu_muid_nSCTHits, &b_mu_muid_nSCTHits); }
  if( fChain->GetBranchStatus("mu_muid_nTRTHits") ) { fChain->SetBranchAddress("mu_muid_nTRTHits", &mu_muid_nTRTHits, &b_mu_muid_nTRTHits); }
  if( fChain->GetBranchStatus("mu_muid_nTRTHighTHits") ) { fChain->SetBranchAddress("mu_muid_nTRTHighTHits", &mu_muid_nTRTHighTHits, &b_mu_muid_nTRTHighTHits); }
  if( fChain->GetBranchStatus("mu_muid_nBLSharedHits") ) { fChain->SetBranchAddress("mu_muid_nBLSharedHits", &mu_muid_nBLSharedHits, &b_mu_muid_nBLSharedHits); }
  if( fChain->GetBranchStatus("mu_muid_nPixSharedHits") ) { fChain->SetBranchAddress("mu_muid_nPixSharedHits", &mu_muid_nPixSharedHits, &b_mu_muid_nPixSharedHits); }
  if( fChain->GetBranchStatus("mu_muid_nPixHoles") ) { fChain->SetBranchAddress("mu_muid_nPixHoles", &mu_muid_nPixHoles, &b_mu_muid_nPixHoles); }
  if( fChain->GetBranchStatus("mu_muid_nSCTSharedHits") ) { fChain->SetBranchAddress("mu_muid_nSCTSharedHits", &mu_muid_nSCTSharedHits, &b_mu_muid_nSCTSharedHits); }
  if( fChain->GetBranchStatus("mu_muid_nSCTHoles") ) { fChain->SetBranchAddress("mu_muid_nSCTHoles", &mu_muid_nSCTHoles, &b_mu_muid_nSCTHoles); }
  if( fChain->GetBranchStatus("mu_muid_nTRTOutliers") ) { fChain->SetBranchAddress("mu_muid_nTRTOutliers", &mu_muid_nTRTOutliers, &b_mu_muid_nTRTOutliers); }
  if( fChain->GetBranchStatus("mu_muid_nTRTHighTOutliers") ) { fChain->SetBranchAddress("mu_muid_nTRTHighTOutliers", &mu_muid_nTRTHighTOutliers, &b_mu_muid_nTRTHighTOutliers); }
  if( fChain->GetBranchStatus("mu_muid_nGangedPixels") ) { fChain->SetBranchAddress("mu_muid_nGangedPixels", &mu_muid_nGangedPixels, &b_mu_muid_nGangedPixels); }
  if( fChain->GetBranchStatus("mu_muid_nPixelDeadSensors") ) { fChain->SetBranchAddress("mu_muid_nPixelDeadSensors", &mu_muid_nPixelDeadSensors, &b_mu_muid_nPixelDeadSensors); }
  if( fChain->GetBranchStatus("mu_muid_nSCTDeadSensors") ) { fChain->SetBranchAddress("mu_muid_nSCTDeadSensors", &mu_muid_nSCTDeadSensors, &b_mu_muid_nSCTDeadSensors); }
  if( fChain->GetBranchStatus("mu_muid_nTRTDeadStraws") ) { fChain->SetBranchAddress("mu_muid_nTRTDeadStraws", &mu_muid_nTRTDeadStraws, &b_mu_muid_nTRTDeadStraws); }
  if( fChain->GetBranchStatus("mu_muid_expectBLayerHit") ) { fChain->SetBranchAddress("mu_muid_expectBLayerHit", &mu_muid_expectBLayerHit, &b_mu_muid_expectBLayerHit); }
  if( fChain->GetBranchStatus("mu_muid_nMDTHits") ) { fChain->SetBranchAddress("mu_muid_nMDTHits", &mu_muid_nMDTHits, &b_mu_muid_nMDTHits); }
  if( fChain->GetBranchStatus("mu_muid_nMDTHoles") ) { fChain->SetBranchAddress("mu_muid_nMDTHoles", &mu_muid_nMDTHoles, &b_mu_muid_nMDTHoles); }
  if( fChain->GetBranchStatus("mu_muid_nCSCEtaHits") ) { fChain->SetBranchAddress("mu_muid_nCSCEtaHits", &mu_muid_nCSCEtaHits, &b_mu_muid_nCSCEtaHits); }
  if( fChain->GetBranchStatus("mu_muid_nCSCEtaHoles") ) { fChain->SetBranchAddress("mu_muid_nCSCEtaHoles", &mu_muid_nCSCEtaHoles, &b_mu_muid_nCSCEtaHoles); }
  if( fChain->GetBranchStatus("mu_muid_nCSCPhiHits") ) { fChain->SetBranchAddress("mu_muid_nCSCPhiHits", &mu_muid_nCSCPhiHits, &b_mu_muid_nCSCPhiHits); }
  if( fChain->GetBranchStatus("mu_muid_nCSCPhiHoles") ) { fChain->SetBranchAddress("mu_muid_nCSCPhiHoles", &mu_muid_nCSCPhiHoles, &b_mu_muid_nCSCPhiHoles); }
  if( fChain->GetBranchStatus("mu_muid_nRPCEtaHits") ) { fChain->SetBranchAddress("mu_muid_nRPCEtaHits", &mu_muid_nRPCEtaHits, &b_mu_muid_nRPCEtaHits); }
  if( fChain->GetBranchStatus("mu_muid_nRPCEtaHoles") ) { fChain->SetBranchAddress("mu_muid_nRPCEtaHoles", &mu_muid_nRPCEtaHoles, &b_mu_muid_nRPCEtaHoles); }
  if( fChain->GetBranchStatus("mu_muid_nRPCPhiHits") ) { fChain->SetBranchAddress("mu_muid_nRPCPhiHits", &mu_muid_nRPCPhiHits, &b_mu_muid_nRPCPhiHits); }
  if( fChain->GetBranchStatus("mu_muid_nRPCPhiHoles") ) { fChain->SetBranchAddress("mu_muid_nRPCPhiHoles", &mu_muid_nRPCPhiHoles, &b_mu_muid_nRPCPhiHoles); }
  if( fChain->GetBranchStatus("mu_muid_nTGCEtaHits") ) { fChain->SetBranchAddress("mu_muid_nTGCEtaHits", &mu_muid_nTGCEtaHits, &b_mu_muid_nTGCEtaHits); }
  if( fChain->GetBranchStatus("mu_muid_nTGCEtaHoles") ) { fChain->SetBranchAddress("mu_muid_nTGCEtaHoles", &mu_muid_nTGCEtaHoles, &b_mu_muid_nTGCEtaHoles); }
  if( fChain->GetBranchStatus("mu_muid_nTGCPhiHits") ) { fChain->SetBranchAddress("mu_muid_nTGCPhiHits", &mu_muid_nTGCPhiHits, &b_mu_muid_nTGCPhiHits); }
  if( fChain->GetBranchStatus("mu_muid_nTGCPhiHoles") ) { fChain->SetBranchAddress("mu_muid_nTGCPhiHoles", &mu_muid_nTGCPhiHoles, &b_mu_muid_nTGCPhiHoles); }
  if( fChain->GetBranchStatus("mu_muid_nMDTBIHits") ) { fChain->SetBranchAddress("mu_muid_nMDTBIHits", &mu_muid_nMDTBIHits, &b_mu_muid_nMDTBIHits); }
  if( fChain->GetBranchStatus("mu_muid_nMDTBMHits") ) { fChain->SetBranchAddress("mu_muid_nMDTBMHits", &mu_muid_nMDTBMHits, &b_mu_muid_nMDTBMHits); }
  if( fChain->GetBranchStatus("mu_muid_nMDTBOHits") ) { fChain->SetBranchAddress("mu_muid_nMDTBOHits", &mu_muid_nMDTBOHits, &b_mu_muid_nMDTBOHits); }
  if( fChain->GetBranchStatus("mu_muid_nMDTBEEHits") ) { fChain->SetBranchAddress("mu_muid_nMDTBEEHits", &mu_muid_nMDTBEEHits, &b_mu_muid_nMDTBEEHits); }
  if( fChain->GetBranchStatus("mu_muid_nMDTBIS78Hits") ) { fChain->SetBranchAddress("mu_muid_nMDTBIS78Hits", &mu_muid_nMDTBIS78Hits, &b_mu_muid_nMDTBIS78Hits); }
  if( fChain->GetBranchStatus("mu_muid_nMDTEIHits") ) { fChain->SetBranchAddress("mu_muid_nMDTEIHits", &mu_muid_nMDTEIHits, &b_mu_muid_nMDTEIHits); }
  if( fChain->GetBranchStatus("mu_muid_nMDTEMHits") ) { fChain->SetBranchAddress("mu_muid_nMDTEMHits", &mu_muid_nMDTEMHits, &b_mu_muid_nMDTEMHits); }
  if( fChain->GetBranchStatus("mu_muid_nMDTEOHits") ) { fChain->SetBranchAddress("mu_muid_nMDTEOHits", &mu_muid_nMDTEOHits, &b_mu_muid_nMDTEOHits); }
  if( fChain->GetBranchStatus("mu_muid_nMDTEEHits") ) { fChain->SetBranchAddress("mu_muid_nMDTEEHits", &mu_muid_nMDTEEHits, &b_mu_muid_nMDTEEHits); }
  if( fChain->GetBranchStatus("mu_muid_nRPCLayer1EtaHits") ) { fChain->SetBranchAddress("mu_muid_nRPCLayer1EtaHits", &mu_muid_nRPCLayer1EtaHits, &b_mu_muid_nRPCLayer1EtaHits); }
  if( fChain->GetBranchStatus("mu_muid_nRPCLayer2EtaHits") ) { fChain->SetBranchAddress("mu_muid_nRPCLayer2EtaHits", &mu_muid_nRPCLayer2EtaHits, &b_mu_muid_nRPCLayer2EtaHits); }
  if( fChain->GetBranchStatus("mu_muid_nRPCLayer3EtaHits") ) { fChain->SetBranchAddress("mu_muid_nRPCLayer3EtaHits", &mu_muid_nRPCLayer3EtaHits, &b_mu_muid_nRPCLayer3EtaHits); }
  if( fChain->GetBranchStatus("mu_muid_nRPCLayer1PhiHits") ) { fChain->SetBranchAddress("mu_muid_nRPCLayer1PhiHits", &mu_muid_nRPCLayer1PhiHits, &b_mu_muid_nRPCLayer1PhiHits); }
  if( fChain->GetBranchStatus("mu_muid_nRPCLayer2PhiHits") ) { fChain->SetBranchAddress("mu_muid_nRPCLayer2PhiHits", &mu_muid_nRPCLayer2PhiHits, &b_mu_muid_nRPCLayer2PhiHits); }
  if( fChain->GetBranchStatus("mu_muid_nRPCLayer3PhiHits") ) { fChain->SetBranchAddress("mu_muid_nRPCLayer3PhiHits", &mu_muid_nRPCLayer3PhiHits, &b_mu_muid_nRPCLayer3PhiHits); }
  if( fChain->GetBranchStatus("mu_muid_nTGCLayer1EtaHits") ) { fChain->SetBranchAddress("mu_muid_nTGCLayer1EtaHits", &mu_muid_nTGCLayer1EtaHits, &b_mu_muid_nTGCLayer1EtaHits); }
  if( fChain->GetBranchStatus("mu_muid_nTGCLayer2EtaHits") ) { fChain->SetBranchAddress("mu_muid_nTGCLayer2EtaHits", &mu_muid_nTGCLayer2EtaHits, &b_mu_muid_nTGCLayer2EtaHits); }
  if( fChain->GetBranchStatus("mu_muid_nTGCLayer3EtaHits") ) { fChain->SetBranchAddress("mu_muid_nTGCLayer3EtaHits", &mu_muid_nTGCLayer3EtaHits, &b_mu_muid_nTGCLayer3EtaHits); }
  if( fChain->GetBranchStatus("mu_muid_nTGCLayer4EtaHits") ) { fChain->SetBranchAddress("mu_muid_nTGCLayer4EtaHits", &mu_muid_nTGCLayer4EtaHits, &b_mu_muid_nTGCLayer4EtaHits); }
  if( fChain->GetBranchStatus("mu_muid_nTGCLayer1PhiHits") ) { fChain->SetBranchAddress("mu_muid_nTGCLayer1PhiHits", &mu_muid_nTGCLayer1PhiHits, &b_mu_muid_nTGCLayer1PhiHits); }
  if( fChain->GetBranchStatus("mu_muid_nTGCLayer2PhiHits") ) { fChain->SetBranchAddress("mu_muid_nTGCLayer2PhiHits", &mu_muid_nTGCLayer2PhiHits, &b_mu_muid_nTGCLayer2PhiHits); }
  if( fChain->GetBranchStatus("mu_muid_nTGCLayer3PhiHits") ) { fChain->SetBranchAddress("mu_muid_nTGCLayer3PhiHits", &mu_muid_nTGCLayer3PhiHits, &b_mu_muid_nTGCLayer3PhiHits); }
  if( fChain->GetBranchStatus("mu_muid_nTGCLayer4PhiHits") ) { fChain->SetBranchAddress("mu_muid_nTGCLayer4PhiHits", &mu_muid_nTGCLayer4PhiHits, &b_mu_muid_nTGCLayer4PhiHits); }
  if( fChain->GetBranchStatus("mu_muid_barrelSectors") ) { fChain->SetBranchAddress("mu_muid_barrelSectors", &mu_muid_barrelSectors, &b_mu_muid_barrelSectors); }
  if( fChain->GetBranchStatus("mu_muid_endcapSectors") ) { fChain->SetBranchAddress("mu_muid_endcapSectors", &mu_muid_endcapSectors, &b_mu_muid_endcapSectors); }
  if( fChain->GetBranchStatus("mu_muid_trackd0") ) { fChain->SetBranchAddress("mu_muid_trackd0", &mu_muid_trackd0, &b_mu_muid_trackd0); }
  if( fChain->GetBranchStatus("mu_muid_trackz0") ) { fChain->SetBranchAddress("mu_muid_trackz0", &mu_muid_trackz0, &b_mu_muid_trackz0); }
  if( fChain->GetBranchStatus("mu_muid_trackphi") ) { fChain->SetBranchAddress("mu_muid_trackphi", &mu_muid_trackphi, &b_mu_muid_trackphi); }
  if( fChain->GetBranchStatus("mu_muid_tracktheta") ) { fChain->SetBranchAddress("mu_muid_tracktheta", &mu_muid_tracktheta, &b_mu_muid_tracktheta); }
  if( fChain->GetBranchStatus("mu_muid_trackqoverp") ) { fChain->SetBranchAddress("mu_muid_trackqoverp", &mu_muid_trackqoverp, &b_mu_muid_trackqoverp); }
  if( fChain->GetBranchStatus("mu_muid_trackcov_d0") ) { fChain->SetBranchAddress("mu_muid_trackcov_d0", &mu_muid_trackcov_d0, &b_mu_muid_trackcov_d0); }
  if( fChain->GetBranchStatus("mu_muid_trackcov_z0") ) { fChain->SetBranchAddress("mu_muid_trackcov_z0", &mu_muid_trackcov_z0, &b_mu_muid_trackcov_z0); }
  if( fChain->GetBranchStatus("mu_muid_trackcov_phi") ) { fChain->SetBranchAddress("mu_muid_trackcov_phi", &mu_muid_trackcov_phi, &b_mu_muid_trackcov_phi); }
  if( fChain->GetBranchStatus("mu_muid_trackcov_theta") ) { fChain->SetBranchAddress("mu_muid_trackcov_theta", &mu_muid_trackcov_theta, &b_mu_muid_trackcov_theta); }
  if( fChain->GetBranchStatus("mu_muid_trackcov_qoverp") ) { fChain->SetBranchAddress("mu_muid_trackcov_qoverp", &mu_muid_trackcov_qoverp, &b_mu_muid_trackcov_qoverp); }
  if( fChain->GetBranchStatus("mu_muid_trackcov_d0_z0") ) { fChain->SetBranchAddress("mu_muid_trackcov_d0_z0", &mu_muid_trackcov_d0_z0, &b_mu_muid_trackcov_d0_z0); }
  if( fChain->GetBranchStatus("mu_muid_trackcov_d0_phi") ) { fChain->SetBranchAddress("mu_muid_trackcov_d0_phi", &mu_muid_trackcov_d0_phi, &b_mu_muid_trackcov_d0_phi); }
  if( fChain->GetBranchStatus("mu_muid_trackcov_d0_theta") ) { fChain->SetBranchAddress("mu_muid_trackcov_d0_theta", &mu_muid_trackcov_d0_theta, &b_mu_muid_trackcov_d0_theta); }
  if( fChain->GetBranchStatus("mu_muid_trackcov_d0_qoverp") ) { fChain->SetBranchAddress("mu_muid_trackcov_d0_qoverp", &mu_muid_trackcov_d0_qoverp, &b_mu_muid_trackcov_d0_qoverp); }
  if( fChain->GetBranchStatus("mu_muid_trackcov_z0_phi") ) { fChain->SetBranchAddress("mu_muid_trackcov_z0_phi", &mu_muid_trackcov_z0_phi, &b_mu_muid_trackcov_z0_phi); }
  if( fChain->GetBranchStatus("mu_muid_trackcov_z0_theta") ) { fChain->SetBranchAddress("mu_muid_trackcov_z0_theta", &mu_muid_trackcov_z0_theta, &b_mu_muid_trackcov_z0_theta); }
  if( fChain->GetBranchStatus("mu_muid_trackcov_z0_qoverp") ) { fChain->SetBranchAddress("mu_muid_trackcov_z0_qoverp", &mu_muid_trackcov_z0_qoverp, &b_mu_muid_trackcov_z0_qoverp); }
  if( fChain->GetBranchStatus("mu_muid_trackcov_phi_theta") ) { fChain->SetBranchAddress("mu_muid_trackcov_phi_theta", &mu_muid_trackcov_phi_theta, &b_mu_muid_trackcov_phi_theta); }
  if( fChain->GetBranchStatus("mu_muid_trackcov_phi_qoverp") ) { fChain->SetBranchAddress("mu_muid_trackcov_phi_qoverp", &mu_muid_trackcov_phi_qoverp, &b_mu_muid_trackcov_phi_qoverp); }
  if( fChain->GetBranchStatus("mu_muid_trackcov_theta_qoverp") ) { fChain->SetBranchAddress("mu_muid_trackcov_theta_qoverp", &mu_muid_trackcov_theta_qoverp, &b_mu_muid_trackcov_theta_qoverp); }
  if( fChain->GetBranchStatus("mu_muid_trackfitchi2") ) { fChain->SetBranchAddress("mu_muid_trackfitchi2", &mu_muid_trackfitchi2, &b_mu_muid_trackfitchi2); }
  if( fChain->GetBranchStatus("mu_muid_trackfitndof") ) { fChain->SetBranchAddress("mu_muid_trackfitndof", &mu_muid_trackfitndof, &b_mu_muid_trackfitndof); }
  if( fChain->GetBranchStatus("mu_muid_hastrack") ) { fChain->SetBranchAddress("mu_muid_hastrack", &mu_muid_hastrack, &b_mu_muid_hastrack); }
  if( fChain->GetBranchStatus("mu_muid_truth_dr") ) { fChain->SetBranchAddress("mu_muid_truth_dr", &mu_muid_truth_dr, &b_mu_muid_truth_dr); }
  if( fChain->GetBranchStatus("mu_muid_truth_E") ) { fChain->SetBranchAddress("mu_muid_truth_E", &mu_muid_truth_E, &b_mu_muid_truth_E); }
  if( fChain->GetBranchStatus("mu_muid_truth_pt") ) { fChain->SetBranchAddress("mu_muid_truth_pt", &mu_muid_truth_pt, &b_mu_muid_truth_pt); }
  if( fChain->GetBranchStatus("mu_muid_truth_eta") ) { fChain->SetBranchAddress("mu_muid_truth_eta", &mu_muid_truth_eta, &b_mu_muid_truth_eta); }
  if( fChain->GetBranchStatus("mu_muid_truth_phi") ) { fChain->SetBranchAddress("mu_muid_truth_phi", &mu_muid_truth_phi, &b_mu_muid_truth_phi); }
  if( fChain->GetBranchStatus("mu_muid_truth_type") ) { fChain->SetBranchAddress("mu_muid_truth_type", &mu_muid_truth_type, &b_mu_muid_truth_type); }
  if( fChain->GetBranchStatus("mu_muid_truth_status") ) { fChain->SetBranchAddress("mu_muid_truth_status", &mu_muid_truth_status, &b_mu_muid_truth_status); }
  if( fChain->GetBranchStatus("mu_muid_truth_barcode") ) { fChain->SetBranchAddress("mu_muid_truth_barcode", &mu_muid_truth_barcode, &b_mu_muid_truth_barcode); }
  if( fChain->GetBranchStatus("mu_muid_truth_mothertype") ) { fChain->SetBranchAddress("mu_muid_truth_mothertype", &mu_muid_truth_mothertype, &b_mu_muid_truth_mothertype); }
  if( fChain->GetBranchStatus("mu_muid_truth_motherbarcode") ) { fChain->SetBranchAddress("mu_muid_truth_motherbarcode", &mu_muid_truth_motherbarcode, &b_mu_muid_truth_motherbarcode); }
  if( fChain->GetBranchStatus("mu_muid_truth_matched") ) { fChain->SetBranchAddress("mu_muid_truth_matched", &mu_muid_truth_matched, &b_mu_muid_truth_matched); }
  if( fChain->GetBranchStatus("mu_muid_EFCB_dr") ) { fChain->SetBranchAddress("mu_muid_EFCB_dr", &mu_muid_EFCB_dr, &b_mu_muid_EFCB_dr); }
  if( fChain->GetBranchStatus("mu_muid_EFCB_index") ) { fChain->SetBranchAddress("mu_muid_EFCB_index", &mu_muid_EFCB_index, &b_mu_muid_EFCB_index); }
  if( fChain->GetBranchStatus("mu_muid_EFMG_dr") ) { fChain->SetBranchAddress("mu_muid_EFMG_dr", &mu_muid_EFMG_dr, &b_mu_muid_EFMG_dr); }
  if( fChain->GetBranchStatus("mu_muid_EFMG_index") ) { fChain->SetBranchAddress("mu_muid_EFMG_index", &mu_muid_EFMG_index, &b_mu_muid_EFMG_index); }
  if( fChain->GetBranchStatus("mu_muid_EFME_dr") ) { fChain->SetBranchAddress("mu_muid_EFME_dr", &mu_muid_EFME_dr, &b_mu_muid_EFME_dr); }
  if( fChain->GetBranchStatus("mu_muid_EFME_index") ) { fChain->SetBranchAddress("mu_muid_EFME_index", &mu_muid_EFME_index, &b_mu_muid_EFME_index); }
  if( fChain->GetBranchStatus("mu_muid_L2CB_dr") ) { fChain->SetBranchAddress("mu_muid_L2CB_dr", &mu_muid_L2CB_dr, &b_mu_muid_L2CB_dr); }
  if( fChain->GetBranchStatus("mu_muid_L2CB_index") ) { fChain->SetBranchAddress("mu_muid_L2CB_index", &mu_muid_L2CB_index, &b_mu_muid_L2CB_index); }
  if( fChain->GetBranchStatus("mu_muid_L1_dr") ) { fChain->SetBranchAddress("mu_muid_L1_dr", &mu_muid_L1_dr, &b_mu_muid_L1_dr); }
  if( fChain->GetBranchStatus("mu_muid_L1_index") ) { fChain->SetBranchAddress("mu_muid_L1_index", &mu_muid_L1_index, &b_mu_muid_L1_index); }
  if( fChain->GetBranchStatus("mu_staco_n") ) { fChain->SetBranchAddress("mu_staco_n", &mu_staco_n, &b_mu_staco_n); }
  if( fChain->GetBranchStatus("mu_staco_E") ) { fChain->SetBranchAddress("mu_staco_E", &mu_staco_E, &b_mu_staco_E); }
  if( fChain->GetBranchStatus("mu_staco_pt") ) { fChain->SetBranchAddress("mu_staco_pt", &mu_staco_pt, &b_mu_staco_pt); }
  if( fChain->GetBranchStatus("mu_staco_m") ) { fChain->SetBranchAddress("mu_staco_m", &mu_staco_m, &b_mu_staco_m); }
  if( fChain->GetBranchStatus("mu_staco_eta") ) { fChain->SetBranchAddress("mu_staco_eta", &mu_staco_eta, &b_mu_staco_eta); }
  if( fChain->GetBranchStatus("mu_staco_phi") ) { fChain->SetBranchAddress("mu_staco_phi", &mu_staco_phi, &b_mu_staco_phi); }
  if( fChain->GetBranchStatus("mu_staco_px") ) { fChain->SetBranchAddress("mu_staco_px", &mu_staco_px, &b_mu_staco_px); }
  if( fChain->GetBranchStatus("mu_staco_py") ) { fChain->SetBranchAddress("mu_staco_py", &mu_staco_py, &b_mu_staco_py); }
  if( fChain->GetBranchStatus("mu_staco_pz") ) { fChain->SetBranchAddress("mu_staco_pz", &mu_staco_pz, &b_mu_staco_pz); }
  if( fChain->GetBranchStatus("mu_staco_charge") ) { fChain->SetBranchAddress("mu_staco_charge", &mu_staco_charge, &b_mu_staco_charge); }
  if( fChain->GetBranchStatus("mu_staco_allauthor") ) { fChain->SetBranchAddress("mu_staco_allauthor", &mu_staco_allauthor, &b_mu_staco_allauthor); }
  if( fChain->GetBranchStatus("mu_staco_author") ) { fChain->SetBranchAddress("mu_staco_author", &mu_staco_author, &b_mu_staco_author); }
  if( fChain->GetBranchStatus("mu_staco_beta") ) { fChain->SetBranchAddress("mu_staco_beta", &mu_staco_beta, &b_mu_staco_beta); }
  if( fChain->GetBranchStatus("mu_staco_isMuonLikelihood") ) { fChain->SetBranchAddress("mu_staco_isMuonLikelihood", &mu_staco_isMuonLikelihood, &b_mu_staco_isMuonLikelihood); }
  if( fChain->GetBranchStatus("mu_staco_matchchi2") ) { fChain->SetBranchAddress("mu_staco_matchchi2", &mu_staco_matchchi2, &b_mu_staco_matchchi2); }
  if( fChain->GetBranchStatus("mu_staco_matchndof") ) { fChain->SetBranchAddress("mu_staco_matchndof", &mu_staco_matchndof, &b_mu_staco_matchndof); }
  if( fChain->GetBranchStatus("mu_staco_etcone20") ) { fChain->SetBranchAddress("mu_staco_etcone20", &mu_staco_etcone20, &b_mu_staco_etcone20); }
  if( fChain->GetBranchStatus("mu_staco_etcone30") ) { fChain->SetBranchAddress("mu_staco_etcone30", &mu_staco_etcone30, &b_mu_staco_etcone30); }
  if( fChain->GetBranchStatus("mu_staco_etcone40") ) { fChain->SetBranchAddress("mu_staco_etcone40", &mu_staco_etcone40, &b_mu_staco_etcone40); }
  if( fChain->GetBranchStatus("mu_staco_nucone20") ) { fChain->SetBranchAddress("mu_staco_nucone20", &mu_staco_nucone20, &b_mu_staco_nucone20); }
  if( fChain->GetBranchStatus("mu_staco_nucone30") ) { fChain->SetBranchAddress("mu_staco_nucone30", &mu_staco_nucone30, &b_mu_staco_nucone30); }
  if( fChain->GetBranchStatus("mu_staco_nucone40") ) { fChain->SetBranchAddress("mu_staco_nucone40", &mu_staco_nucone40, &b_mu_staco_nucone40); }
  if( fChain->GetBranchStatus("mu_staco_ptcone20") ) { fChain->SetBranchAddress("mu_staco_ptcone20", &mu_staco_ptcone20, &b_mu_staco_ptcone20); }
  if( fChain->GetBranchStatus("mu_staco_ptcone30") ) { fChain->SetBranchAddress("mu_staco_ptcone30", &mu_staco_ptcone30, &b_mu_staco_ptcone30); }
  if( fChain->GetBranchStatus("mu_staco_ptcone40") ) { fChain->SetBranchAddress("mu_staco_ptcone40", &mu_staco_ptcone40, &b_mu_staco_ptcone40); }
  if( fChain->GetBranchStatus("mu_staco_energyLossPar") ) { fChain->SetBranchAddress("mu_staco_energyLossPar", &mu_staco_energyLossPar, &b_mu_staco_energyLossPar); }
  if( fChain->GetBranchStatus("mu_staco_energyLossErr") ) { fChain->SetBranchAddress("mu_staco_energyLossErr", &mu_staco_energyLossErr, &b_mu_staco_energyLossErr); }
  if( fChain->GetBranchStatus("mu_staco_etCore") ) { fChain->SetBranchAddress("mu_staco_etCore", &mu_staco_etCore, &b_mu_staco_etCore); }
  if( fChain->GetBranchStatus("mu_staco_energyLossType") ) { fChain->SetBranchAddress("mu_staco_energyLossType", &mu_staco_energyLossType, &b_mu_staco_energyLossType); }
  if( fChain->GetBranchStatus("mu_staco_caloMuonIdTag") ) { fChain->SetBranchAddress("mu_staco_caloMuonIdTag", &mu_staco_caloMuonIdTag, &b_mu_staco_caloMuonIdTag); }
  if( fChain->GetBranchStatus("mu_staco_caloLRLikelihood") ) { fChain->SetBranchAddress("mu_staco_caloLRLikelihood", &mu_staco_caloLRLikelihood, &b_mu_staco_caloLRLikelihood); }
  if( fChain->GetBranchStatus("mu_staco_bestMatch") ) { fChain->SetBranchAddress("mu_staco_bestMatch", &mu_staco_bestMatch, &b_mu_staco_bestMatch); }
  if( fChain->GetBranchStatus("mu_staco_isStandAloneMuon") ) { fChain->SetBranchAddress("mu_staco_isStandAloneMuon", &mu_staco_isStandAloneMuon, &b_mu_staco_isStandAloneMuon); }
  if( fChain->GetBranchStatus("mu_staco_isCombinedMuon") ) { fChain->SetBranchAddress("mu_staco_isCombinedMuon", &mu_staco_isCombinedMuon, &b_mu_staco_isCombinedMuon); }
  if( fChain->GetBranchStatus("mu_staco_isLowPtReconstructedMuon") ) { fChain->SetBranchAddress("mu_staco_isLowPtReconstructedMuon", &mu_staco_isLowPtReconstructedMuon, &b_mu_staco_isLowPtReconstructedMuon); }
  if( fChain->GetBranchStatus("mu_staco_loose") ) { fChain->SetBranchAddress("mu_staco_loose", &mu_staco_loose, &b_mu_staco_loose); }
  if( fChain->GetBranchStatus("mu_staco_medium") ) { fChain->SetBranchAddress("mu_staco_medium", &mu_staco_medium, &b_mu_staco_medium); }
  if( fChain->GetBranchStatus("mu_staco_tight") ) { fChain->SetBranchAddress("mu_staco_tight", &mu_staco_tight, &b_mu_staco_tight); }
  if( fChain->GetBranchStatus("mu_staco_d0_exPV") ) { fChain->SetBranchAddress("mu_staco_d0_exPV", &mu_staco_d0_exPV, &b_mu_staco_d0_exPV); }
  if( fChain->GetBranchStatus("mu_staco_z0_exPV") ) { fChain->SetBranchAddress("mu_staco_z0_exPV", &mu_staco_z0_exPV, &b_mu_staco_z0_exPV); }
  if( fChain->GetBranchStatus("mu_staco_phi_exPV") ) { fChain->SetBranchAddress("mu_staco_phi_exPV", &mu_staco_phi_exPV, &b_mu_staco_phi_exPV); }
  if( fChain->GetBranchStatus("mu_staco_theta_exPV") ) { fChain->SetBranchAddress("mu_staco_theta_exPV", &mu_staco_theta_exPV, &b_mu_staco_theta_exPV); }
  if( fChain->GetBranchStatus("mu_staco_qoverp_exPV") ) { fChain->SetBranchAddress("mu_staco_qoverp_exPV", &mu_staco_qoverp_exPV, &b_mu_staco_qoverp_exPV); }
  if( fChain->GetBranchStatus("mu_staco_cb_d0_exPV") ) { fChain->SetBranchAddress("mu_staco_cb_d0_exPV", &mu_staco_cb_d0_exPV, &b_mu_staco_cb_d0_exPV); }
  if( fChain->GetBranchStatus("mu_staco_cb_z0_exPV") ) { fChain->SetBranchAddress("mu_staco_cb_z0_exPV", &mu_staco_cb_z0_exPV, &b_mu_staco_cb_z0_exPV); }
  if( fChain->GetBranchStatus("mu_staco_cb_phi_exPV") ) { fChain->SetBranchAddress("mu_staco_cb_phi_exPV", &mu_staco_cb_phi_exPV, &b_mu_staco_cb_phi_exPV); }
  if( fChain->GetBranchStatus("mu_staco_cb_theta_exPV") ) { fChain->SetBranchAddress("mu_staco_cb_theta_exPV", &mu_staco_cb_theta_exPV, &b_mu_staco_cb_theta_exPV); }
  if( fChain->GetBranchStatus("mu_staco_cb_qoverp_exPV") ) { fChain->SetBranchAddress("mu_staco_cb_qoverp_exPV", &mu_staco_cb_qoverp_exPV, &b_mu_staco_cb_qoverp_exPV); }
  if( fChain->GetBranchStatus("mu_staco_id_d0_exPV") ) { fChain->SetBranchAddress("mu_staco_id_d0_exPV", &mu_staco_id_d0_exPV, &b_mu_staco_id_d0_exPV); }
  if( fChain->GetBranchStatus("mu_staco_id_z0_exPV") ) { fChain->SetBranchAddress("mu_staco_id_z0_exPV", &mu_staco_id_z0_exPV, &b_mu_staco_id_z0_exPV); }
  if( fChain->GetBranchStatus("mu_staco_id_phi_exPV") ) { fChain->SetBranchAddress("mu_staco_id_phi_exPV", &mu_staco_id_phi_exPV, &b_mu_staco_id_phi_exPV); }
  if( fChain->GetBranchStatus("mu_staco_id_theta_exPV") ) { fChain->SetBranchAddress("mu_staco_id_theta_exPV", &mu_staco_id_theta_exPV, &b_mu_staco_id_theta_exPV); }
  if( fChain->GetBranchStatus("mu_staco_id_qoverp_exPV") ) { fChain->SetBranchAddress("mu_staco_id_qoverp_exPV", &mu_staco_id_qoverp_exPV, &b_mu_staco_id_qoverp_exPV); }
  if( fChain->GetBranchStatus("mu_staco_me_d0_exPV") ) { fChain->SetBranchAddress("mu_staco_me_d0_exPV", &mu_staco_me_d0_exPV, &b_mu_staco_me_d0_exPV); }
  if( fChain->GetBranchStatus("mu_staco_me_z0_exPV") ) { fChain->SetBranchAddress("mu_staco_me_z0_exPV", &mu_staco_me_z0_exPV, &b_mu_staco_me_z0_exPV); }
  if( fChain->GetBranchStatus("mu_staco_me_phi_exPV") ) { fChain->SetBranchAddress("mu_staco_me_phi_exPV", &mu_staco_me_phi_exPV, &b_mu_staco_me_phi_exPV); }
  if( fChain->GetBranchStatus("mu_staco_me_theta_exPV") ) { fChain->SetBranchAddress("mu_staco_me_theta_exPV", &mu_staco_me_theta_exPV, &b_mu_staco_me_theta_exPV); }
  if( fChain->GetBranchStatus("mu_staco_me_qoverp_exPV") ) { fChain->SetBranchAddress("mu_staco_me_qoverp_exPV", &mu_staco_me_qoverp_exPV, &b_mu_staco_me_qoverp_exPV); }
  if( fChain->GetBranchStatus("mu_staco_ie_d0_exPV") ) { fChain->SetBranchAddress("mu_staco_ie_d0_exPV", &mu_staco_ie_d0_exPV, &b_mu_staco_ie_d0_exPV); }
  if( fChain->GetBranchStatus("mu_staco_ie_z0_exPV") ) { fChain->SetBranchAddress("mu_staco_ie_z0_exPV", &mu_staco_ie_z0_exPV, &b_mu_staco_ie_z0_exPV); }
  if( fChain->GetBranchStatus("mu_staco_ie_phi_exPV") ) { fChain->SetBranchAddress("mu_staco_ie_phi_exPV", &mu_staco_ie_phi_exPV, &b_mu_staco_ie_phi_exPV); }
  if( fChain->GetBranchStatus("mu_staco_ie_theta_exPV") ) { fChain->SetBranchAddress("mu_staco_ie_theta_exPV", &mu_staco_ie_theta_exPV, &b_mu_staco_ie_theta_exPV); }
  if( fChain->GetBranchStatus("mu_staco_ie_qoverp_exPV") ) { fChain->SetBranchAddress("mu_staco_ie_qoverp_exPV", &mu_staco_ie_qoverp_exPV, &b_mu_staco_ie_qoverp_exPV); }
  if( fChain->GetBranchStatus("mu_staco_SpaceTime_detID") ) { fChain->SetBranchAddress("mu_staco_SpaceTime_detID", &mu_staco_SpaceTime_detID, &b_mu_staco_SpaceTime_detID); }
  if( fChain->GetBranchStatus("mu_staco_SpaceTime_t") ) { fChain->SetBranchAddress("mu_staco_SpaceTime_t", &mu_staco_SpaceTime_t, &b_mu_staco_SpaceTime_t); }
  if( fChain->GetBranchStatus("mu_staco_SpaceTime_tError") ) { fChain->SetBranchAddress("mu_staco_SpaceTime_tError", &mu_staco_SpaceTime_tError, &b_mu_staco_SpaceTime_tError); }
  if( fChain->GetBranchStatus("mu_staco_SpaceTime_weight") ) { fChain->SetBranchAddress("mu_staco_SpaceTime_weight", &mu_staco_SpaceTime_weight, &b_mu_staco_SpaceTime_weight); }
  if( fChain->GetBranchStatus("mu_staco_SpaceTime_x") ) { fChain->SetBranchAddress("mu_staco_SpaceTime_x", &mu_staco_SpaceTime_x, &b_mu_staco_SpaceTime_x); }
  if( fChain->GetBranchStatus("mu_staco_SpaceTime_y") ) { fChain->SetBranchAddress("mu_staco_SpaceTime_y", &mu_staco_SpaceTime_y, &b_mu_staco_SpaceTime_y); }
  if( fChain->GetBranchStatus("mu_staco_SpaceTime_z") ) { fChain->SetBranchAddress("mu_staco_SpaceTime_z", &mu_staco_SpaceTime_z, &b_mu_staco_SpaceTime_z); }
  if( fChain->GetBranchStatus("mu_staco_SpaceTime_t_Tile") ) { fChain->SetBranchAddress("mu_staco_SpaceTime_t_Tile", &mu_staco_SpaceTime_t_Tile, &b_mu_staco_SpaceTime_t_Tile); }
  if( fChain->GetBranchStatus("mu_staco_SpaceTime_tError_Tile") ) { fChain->SetBranchAddress("mu_staco_SpaceTime_tError_Tile", &mu_staco_SpaceTime_tError_Tile, &b_mu_staco_SpaceTime_tError_Tile); }
  if( fChain->GetBranchStatus("mu_staco_SpaceTime_weight_Tile") ) { fChain->SetBranchAddress("mu_staco_SpaceTime_weight_Tile", &mu_staco_SpaceTime_weight_Tile, &b_mu_staco_SpaceTime_weight_Tile); }
  if( fChain->GetBranchStatus("mu_staco_SpaceTime_x_Tile") ) { fChain->SetBranchAddress("mu_staco_SpaceTime_x_Tile", &mu_staco_SpaceTime_x_Tile, &b_mu_staco_SpaceTime_x_Tile); }
  if( fChain->GetBranchStatus("mu_staco_SpaceTime_y_Tile") ) { fChain->SetBranchAddress("mu_staco_SpaceTime_y_Tile", &mu_staco_SpaceTime_y_Tile, &b_mu_staco_SpaceTime_y_Tile); }
  if( fChain->GetBranchStatus("mu_staco_SpaceTime_z_Tile") ) { fChain->SetBranchAddress("mu_staco_SpaceTime_z_Tile", &mu_staco_SpaceTime_z_Tile, &b_mu_staco_SpaceTime_z_Tile); }
  if( fChain->GetBranchStatus("mu_staco_SpaceTime_t_TRT") ) { fChain->SetBranchAddress("mu_staco_SpaceTime_t_TRT", &mu_staco_SpaceTime_t_TRT, &b_mu_staco_SpaceTime_t_TRT); }
  if( fChain->GetBranchStatus("mu_staco_SpaceTime_tError_TRT") ) { fChain->SetBranchAddress("mu_staco_SpaceTime_tError_TRT", &mu_staco_SpaceTime_tError_TRT, &b_mu_staco_SpaceTime_tError_TRT); }
  if( fChain->GetBranchStatus("mu_staco_SpaceTime_weight_TRT") ) { fChain->SetBranchAddress("mu_staco_SpaceTime_weight_TRT", &mu_staco_SpaceTime_weight_TRT, &b_mu_staco_SpaceTime_weight_TRT); }
  if( fChain->GetBranchStatus("mu_staco_SpaceTime_x_TRT") ) { fChain->SetBranchAddress("mu_staco_SpaceTime_x_TRT", &mu_staco_SpaceTime_x_TRT, &b_mu_staco_SpaceTime_x_TRT); }
  if( fChain->GetBranchStatus("mu_staco_SpaceTime_y_TRT") ) { fChain->SetBranchAddress("mu_staco_SpaceTime_y_TRT", &mu_staco_SpaceTime_y_TRT, &b_mu_staco_SpaceTime_y_TRT); }
  if( fChain->GetBranchStatus("mu_staco_SpaceTime_z_TRT") ) { fChain->SetBranchAddress("mu_staco_SpaceTime_z_TRT", &mu_staco_SpaceTime_z_TRT, &b_mu_staco_SpaceTime_z_TRT); }
  if( fChain->GetBranchStatus("mu_staco_SpaceTime_t_MDT") ) { fChain->SetBranchAddress("mu_staco_SpaceTime_t_MDT", &mu_staco_SpaceTime_t_MDT, &b_mu_staco_SpaceTime_t_MDT); }
  if( fChain->GetBranchStatus("mu_staco_SpaceTime_tError_MDT") ) { fChain->SetBranchAddress("mu_staco_SpaceTime_tError_MDT", &mu_staco_SpaceTime_tError_MDT, &b_mu_staco_SpaceTime_tError_MDT); }
  if( fChain->GetBranchStatus("mu_staco_SpaceTime_weight_MDT") ) { fChain->SetBranchAddress("mu_staco_SpaceTime_weight_MDT", &mu_staco_SpaceTime_weight_MDT, &b_mu_staco_SpaceTime_weight_MDT); }
  if( fChain->GetBranchStatus("mu_staco_SpaceTime_x_MDT") ) { fChain->SetBranchAddress("mu_staco_SpaceTime_x_MDT", &mu_staco_SpaceTime_x_MDT, &b_mu_staco_SpaceTime_x_MDT); }
  if( fChain->GetBranchStatus("mu_staco_SpaceTime_y_MDT") ) { fChain->SetBranchAddress("mu_staco_SpaceTime_y_MDT", &mu_staco_SpaceTime_y_MDT, &b_mu_staco_SpaceTime_y_MDT); }
  if( fChain->GetBranchStatus("mu_staco_SpaceTime_z_MDT") ) { fChain->SetBranchAddress("mu_staco_SpaceTime_z_MDT", &mu_staco_SpaceTime_z_MDT, &b_mu_staco_SpaceTime_z_MDT); }
  if( fChain->GetBranchStatus("mu_staco_TileCellEnergyLayer1") ) { fChain->SetBranchAddress("mu_staco_TileCellEnergyLayer1", &mu_staco_TileCellEnergyLayer1, &b_mu_staco_TileCellEnergyLayer1); }
  if( fChain->GetBranchStatus("mu_staco_TileTimeLayer1") ) { fChain->SetBranchAddress("mu_staco_TileTimeLayer1", &mu_staco_TileTimeLayer1, &b_mu_staco_TileTimeLayer1); }
  if( fChain->GetBranchStatus("mu_staco_TileCellRmsNoiseLayer1") ) { fChain->SetBranchAddress("mu_staco_TileCellRmsNoiseLayer1", &mu_staco_TileCellRmsNoiseLayer1, &b_mu_staco_TileCellRmsNoiseLayer1); }
  if( fChain->GetBranchStatus("mu_staco_TileCellSignLayer1") ) { fChain->SetBranchAddress("mu_staco_TileCellSignLayer1", &mu_staco_TileCellSignLayer1, &b_mu_staco_TileCellSignLayer1); }
  if( fChain->GetBranchStatus("mu_staco_TileCellEnergyLayer2") ) { fChain->SetBranchAddress("mu_staco_TileCellEnergyLayer2", &mu_staco_TileCellEnergyLayer2, &b_mu_staco_TileCellEnergyLayer2); }
  if( fChain->GetBranchStatus("mu_staco_TileTimeLayer2") ) { fChain->SetBranchAddress("mu_staco_TileTimeLayer2", &mu_staco_TileTimeLayer2, &b_mu_staco_TileTimeLayer2); }
  if( fChain->GetBranchStatus("mu_staco_TileCellRmsNoiseLayer2") ) { fChain->SetBranchAddress("mu_staco_TileCellRmsNoiseLayer2", &mu_staco_TileCellRmsNoiseLayer2, &b_mu_staco_TileCellRmsNoiseLayer2); }
  if( fChain->GetBranchStatus("mu_staco_TileCellSignLayer2") ) { fChain->SetBranchAddress("mu_staco_TileCellSignLayer2", &mu_staco_TileCellSignLayer2, &b_mu_staco_TileCellSignLayer2); }
  if( fChain->GetBranchStatus("mu_staco_TileCellEnergyLayer3") ) { fChain->SetBranchAddress("mu_staco_TileCellEnergyLayer3", &mu_staco_TileCellEnergyLayer3, &b_mu_staco_TileCellEnergyLayer3); }
  if( fChain->GetBranchStatus("mu_staco_TileTimeLayer3") ) { fChain->SetBranchAddress("mu_staco_TileTimeLayer3", &mu_staco_TileTimeLayer3, &b_mu_staco_TileTimeLayer3); }
  if( fChain->GetBranchStatus("mu_staco_TileCellRmsNoiseLayer3") ) { fChain->SetBranchAddress("mu_staco_TileCellRmsNoiseLayer3", &mu_staco_TileCellRmsNoiseLayer3, &b_mu_staco_TileCellRmsNoiseLayer3); }
  if( fChain->GetBranchStatus("mu_staco_TileCellSignLayer3") ) { fChain->SetBranchAddress("mu_staco_TileCellSignLayer3", &mu_staco_TileCellSignLayer3, &b_mu_staco_TileCellSignLayer3); }
  if( fChain->GetBranchStatus("mu_staco_MSTrkT0_1") ) { fChain->SetBranchAddress("mu_staco_MSTrkT0_1", &mu_staco_MSTrkT0_1, &b_mu_staco_MSTrkT0_1); }
  if( fChain->GetBranchStatus("mu_staco_MSTrkT0_2") ) { fChain->SetBranchAddress("mu_staco_MSTrkT0_2", &mu_staco_MSTrkT0_2, &b_mu_staco_MSTrkT0_2); }
  if( fChain->GetBranchStatus("mu_staco_MSTrkT0_3") ) { fChain->SetBranchAddress("mu_staco_MSTrkT0_3", &mu_staco_MSTrkT0_3, &b_mu_staco_MSTrkT0_3); }
  if( fChain->GetBranchStatus("mu_staco_cov_d0_exPV") ) { fChain->SetBranchAddress("mu_staco_cov_d0_exPV", &mu_staco_cov_d0_exPV, &b_mu_staco_cov_d0_exPV); }
  if( fChain->GetBranchStatus("mu_staco_cov_z0_exPV") ) { fChain->SetBranchAddress("mu_staco_cov_z0_exPV", &mu_staco_cov_z0_exPV, &b_mu_staco_cov_z0_exPV); }
  if( fChain->GetBranchStatus("mu_staco_cov_phi_exPV") ) { fChain->SetBranchAddress("mu_staco_cov_phi_exPV", &mu_staco_cov_phi_exPV, &b_mu_staco_cov_phi_exPV); }
  if( fChain->GetBranchStatus("mu_staco_cov_theta_exPV") ) { fChain->SetBranchAddress("mu_staco_cov_theta_exPV", &mu_staco_cov_theta_exPV, &b_mu_staco_cov_theta_exPV); }
  if( fChain->GetBranchStatus("mu_staco_cov_qoverp_exPV") ) { fChain->SetBranchAddress("mu_staco_cov_qoverp_exPV", &mu_staco_cov_qoverp_exPV, &b_mu_staco_cov_qoverp_exPV); }
  if( fChain->GetBranchStatus("mu_staco_cov_d0_z0_exPV") ) { fChain->SetBranchAddress("mu_staco_cov_d0_z0_exPV", &mu_staco_cov_d0_z0_exPV, &b_mu_staco_cov_d0_z0_exPV); }
  if( fChain->GetBranchStatus("mu_staco_cov_d0_phi_exPV") ) { fChain->SetBranchAddress("mu_staco_cov_d0_phi_exPV", &mu_staco_cov_d0_phi_exPV, &b_mu_staco_cov_d0_phi_exPV); }
  if( fChain->GetBranchStatus("mu_staco_cov_d0_theta_exPV") ) { fChain->SetBranchAddress("mu_staco_cov_d0_theta_exPV", &mu_staco_cov_d0_theta_exPV, &b_mu_staco_cov_d0_theta_exPV); }
  if( fChain->GetBranchStatus("mu_staco_cov_d0_qoverp_exPV") ) { fChain->SetBranchAddress("mu_staco_cov_d0_qoverp_exPV", &mu_staco_cov_d0_qoverp_exPV, &b_mu_staco_cov_d0_qoverp_exPV); }
  if( fChain->GetBranchStatus("mu_staco_cov_z0_phi_exPV") ) { fChain->SetBranchAddress("mu_staco_cov_z0_phi_exPV", &mu_staco_cov_z0_phi_exPV, &b_mu_staco_cov_z0_phi_exPV); }
  if( fChain->GetBranchStatus("mu_staco_cov_z0_theta_exPV") ) { fChain->SetBranchAddress("mu_staco_cov_z0_theta_exPV", &mu_staco_cov_z0_theta_exPV, &b_mu_staco_cov_z0_theta_exPV); }
  if( fChain->GetBranchStatus("mu_staco_cov_z0_qoverp_exPV") ) { fChain->SetBranchAddress("mu_staco_cov_z0_qoverp_exPV", &mu_staco_cov_z0_qoverp_exPV, &b_mu_staco_cov_z0_qoverp_exPV); }
  if( fChain->GetBranchStatus("mu_staco_cov_phi_theta_exPV") ) { fChain->SetBranchAddress("mu_staco_cov_phi_theta_exPV", &mu_staco_cov_phi_theta_exPV, &b_mu_staco_cov_phi_theta_exPV); }
  if( fChain->GetBranchStatus("mu_staco_cov_phi_qoverp_exPV") ) { fChain->SetBranchAddress("mu_staco_cov_phi_qoverp_exPV", &mu_staco_cov_phi_qoverp_exPV, &b_mu_staco_cov_phi_qoverp_exPV); }
  if( fChain->GetBranchStatus("mu_staco_cov_theta_qoverp_exPV") ) { fChain->SetBranchAddress("mu_staco_cov_theta_qoverp_exPV", &mu_staco_cov_theta_qoverp_exPV, &b_mu_staco_cov_theta_qoverp_exPV); }
  if( fChain->GetBranchStatus("mu_staco_id_cov_d0_exPV") ) { fChain->SetBranchAddress("mu_staco_id_cov_d0_exPV", &mu_staco_id_cov_d0_exPV, &b_mu_staco_id_cov_d0_exPV); }
  if( fChain->GetBranchStatus("mu_staco_id_cov_z0_exPV") ) { fChain->SetBranchAddress("mu_staco_id_cov_z0_exPV", &mu_staco_id_cov_z0_exPV, &b_mu_staco_id_cov_z0_exPV); }
  if( fChain->GetBranchStatus("mu_staco_id_cov_phi_exPV") ) { fChain->SetBranchAddress("mu_staco_id_cov_phi_exPV", &mu_staco_id_cov_phi_exPV, &b_mu_staco_id_cov_phi_exPV); }
  if( fChain->GetBranchStatus("mu_staco_id_cov_theta_exPV") ) { fChain->SetBranchAddress("mu_staco_id_cov_theta_exPV", &mu_staco_id_cov_theta_exPV, &b_mu_staco_id_cov_theta_exPV); }
  if( fChain->GetBranchStatus("mu_staco_id_cov_qoverp_exPV") ) { fChain->SetBranchAddress("mu_staco_id_cov_qoverp_exPV", &mu_staco_id_cov_qoverp_exPV, &b_mu_staco_id_cov_qoverp_exPV); }
  if( fChain->GetBranchStatus("mu_staco_id_cov_d0_z0_exPV") ) { fChain->SetBranchAddress("mu_staco_id_cov_d0_z0_exPV", &mu_staco_id_cov_d0_z0_exPV, &b_mu_staco_id_cov_d0_z0_exPV); }
  if( fChain->GetBranchStatus("mu_staco_id_cov_d0_phi_exPV") ) { fChain->SetBranchAddress("mu_staco_id_cov_d0_phi_exPV", &mu_staco_id_cov_d0_phi_exPV, &b_mu_staco_id_cov_d0_phi_exPV); }
  if( fChain->GetBranchStatus("mu_staco_id_cov_d0_theta_exPV") ) { fChain->SetBranchAddress("mu_staco_id_cov_d0_theta_exPV", &mu_staco_id_cov_d0_theta_exPV, &b_mu_staco_id_cov_d0_theta_exPV); }
  if( fChain->GetBranchStatus("mu_staco_id_cov_d0_qoverp_exPV") ) { fChain->SetBranchAddress("mu_staco_id_cov_d0_qoverp_exPV", &mu_staco_id_cov_d0_qoverp_exPV, &b_mu_staco_id_cov_d0_qoverp_exPV); }
  if( fChain->GetBranchStatus("mu_staco_id_cov_z0_phi_exPV") ) { fChain->SetBranchAddress("mu_staco_id_cov_z0_phi_exPV", &mu_staco_id_cov_z0_phi_exPV, &b_mu_staco_id_cov_z0_phi_exPV); }
  if( fChain->GetBranchStatus("mu_staco_id_cov_z0_theta_exPV") ) { fChain->SetBranchAddress("mu_staco_id_cov_z0_theta_exPV", &mu_staco_id_cov_z0_theta_exPV, &b_mu_staco_id_cov_z0_theta_exPV); }
  if( fChain->GetBranchStatus("mu_staco_id_cov_z0_qoverp_exPV") ) { fChain->SetBranchAddress("mu_staco_id_cov_z0_qoverp_exPV", &mu_staco_id_cov_z0_qoverp_exPV, &b_mu_staco_id_cov_z0_qoverp_exPV); }
  if( fChain->GetBranchStatus("mu_staco_id_cov_phi_theta_exPV") ) { fChain->SetBranchAddress("mu_staco_id_cov_phi_theta_exPV", &mu_staco_id_cov_phi_theta_exPV, &b_mu_staco_id_cov_phi_theta_exPV); }
  if( fChain->GetBranchStatus("mu_staco_id_cov_phi_qoverp_exPV") ) { fChain->SetBranchAddress("mu_staco_id_cov_phi_qoverp_exPV", &mu_staco_id_cov_phi_qoverp_exPV, &b_mu_staco_id_cov_phi_qoverp_exPV); }
  if( fChain->GetBranchStatus("mu_staco_id_cov_theta_qoverp_exPV") ) { fChain->SetBranchAddress("mu_staco_id_cov_theta_qoverp_exPV", &mu_staco_id_cov_theta_qoverp_exPV, &b_mu_staco_id_cov_theta_qoverp_exPV); }
  if( fChain->GetBranchStatus("mu_staco_me_cov_d0_exPV") ) { fChain->SetBranchAddress("mu_staco_me_cov_d0_exPV", &mu_staco_me_cov_d0_exPV, &b_mu_staco_me_cov_d0_exPV); }
  if( fChain->GetBranchStatus("mu_staco_me_cov_z0_exPV") ) { fChain->SetBranchAddress("mu_staco_me_cov_z0_exPV", &mu_staco_me_cov_z0_exPV, &b_mu_staco_me_cov_z0_exPV); }
  if( fChain->GetBranchStatus("mu_staco_me_cov_phi_exPV") ) { fChain->SetBranchAddress("mu_staco_me_cov_phi_exPV", &mu_staco_me_cov_phi_exPV, &b_mu_staco_me_cov_phi_exPV); }
  if( fChain->GetBranchStatus("mu_staco_me_cov_theta_exPV") ) { fChain->SetBranchAddress("mu_staco_me_cov_theta_exPV", &mu_staco_me_cov_theta_exPV, &b_mu_staco_me_cov_theta_exPV); }
  if( fChain->GetBranchStatus("mu_staco_me_cov_qoverp_exPV") ) { fChain->SetBranchAddress("mu_staco_me_cov_qoverp_exPV", &mu_staco_me_cov_qoverp_exPV, &b_mu_staco_me_cov_qoverp_exPV); }
  if( fChain->GetBranchStatus("mu_staco_me_cov_d0_z0_exPV") ) { fChain->SetBranchAddress("mu_staco_me_cov_d0_z0_exPV", &mu_staco_me_cov_d0_z0_exPV, &b_mu_staco_me_cov_d0_z0_exPV); }
  if( fChain->GetBranchStatus("mu_staco_me_cov_d0_phi_exPV") ) { fChain->SetBranchAddress("mu_staco_me_cov_d0_phi_exPV", &mu_staco_me_cov_d0_phi_exPV, &b_mu_staco_me_cov_d0_phi_exPV); }
  if( fChain->GetBranchStatus("mu_staco_me_cov_d0_theta_exPV") ) { fChain->SetBranchAddress("mu_staco_me_cov_d0_theta_exPV", &mu_staco_me_cov_d0_theta_exPV, &b_mu_staco_me_cov_d0_theta_exPV); }
  if( fChain->GetBranchStatus("mu_staco_me_cov_d0_qoverp_exPV") ) { fChain->SetBranchAddress("mu_staco_me_cov_d0_qoverp_exPV", &mu_staco_me_cov_d0_qoverp_exPV, &b_mu_staco_me_cov_d0_qoverp_exPV); }
  if( fChain->GetBranchStatus("mu_staco_me_cov_z0_phi_exPV") ) { fChain->SetBranchAddress("mu_staco_me_cov_z0_phi_exPV", &mu_staco_me_cov_z0_phi_exPV, &b_mu_staco_me_cov_z0_phi_exPV); }
  if( fChain->GetBranchStatus("mu_staco_me_cov_z0_theta_exPV") ) { fChain->SetBranchAddress("mu_staco_me_cov_z0_theta_exPV", &mu_staco_me_cov_z0_theta_exPV, &b_mu_staco_me_cov_z0_theta_exPV); }
  if( fChain->GetBranchStatus("mu_staco_me_cov_z0_qoverp_exPV") ) { fChain->SetBranchAddress("mu_staco_me_cov_z0_qoverp_exPV", &mu_staco_me_cov_z0_qoverp_exPV, &b_mu_staco_me_cov_z0_qoverp_exPV); }
  if( fChain->GetBranchStatus("mu_staco_me_cov_phi_theta_exPV") ) { fChain->SetBranchAddress("mu_staco_me_cov_phi_theta_exPV", &mu_staco_me_cov_phi_theta_exPV, &b_mu_staco_me_cov_phi_theta_exPV); }
  if( fChain->GetBranchStatus("mu_staco_me_cov_phi_qoverp_exPV") ) { fChain->SetBranchAddress("mu_staco_me_cov_phi_qoverp_exPV", &mu_staco_me_cov_phi_qoverp_exPV, &b_mu_staco_me_cov_phi_qoverp_exPV); }
  if( fChain->GetBranchStatus("mu_staco_me_cov_theta_qoverp_exPV") ) { fChain->SetBranchAddress("mu_staco_me_cov_theta_qoverp_exPV", &mu_staco_me_cov_theta_qoverp_exPV, &b_mu_staco_me_cov_theta_qoverp_exPV); }
  if( fChain->GetBranchStatus("mu_staco_ms_d0") ) { fChain->SetBranchAddress("mu_staco_ms_d0", &mu_staco_ms_d0, &b_mu_staco_ms_d0); }
  if( fChain->GetBranchStatus("mu_staco_ms_z0") ) { fChain->SetBranchAddress("mu_staco_ms_z0", &mu_staco_ms_z0, &b_mu_staco_ms_z0); }
  if( fChain->GetBranchStatus("mu_staco_ms_phi") ) { fChain->SetBranchAddress("mu_staco_ms_phi", &mu_staco_ms_phi, &b_mu_staco_ms_phi); }
  if( fChain->GetBranchStatus("mu_staco_ms_theta") ) { fChain->SetBranchAddress("mu_staco_ms_theta", &mu_staco_ms_theta, &b_mu_staco_ms_theta); }
  if( fChain->GetBranchStatus("mu_staco_ms_qoverp") ) { fChain->SetBranchAddress("mu_staco_ms_qoverp", &mu_staco_ms_qoverp, &b_mu_staco_ms_qoverp); }
  if( fChain->GetBranchStatus("mu_staco_id_d0") ) { fChain->SetBranchAddress("mu_staco_id_d0", &mu_staco_id_d0, &b_mu_staco_id_d0); }
  if( fChain->GetBranchStatus("mu_staco_id_z0") ) { fChain->SetBranchAddress("mu_staco_id_z0", &mu_staco_id_z0, &b_mu_staco_id_z0); }
  if( fChain->GetBranchStatus("mu_staco_id_phi") ) { fChain->SetBranchAddress("mu_staco_id_phi", &mu_staco_id_phi, &b_mu_staco_id_phi); }
  if( fChain->GetBranchStatus("mu_staco_id_theta") ) { fChain->SetBranchAddress("mu_staco_id_theta", &mu_staco_id_theta, &b_mu_staco_id_theta); }
  if( fChain->GetBranchStatus("mu_staco_id_qoverp") ) { fChain->SetBranchAddress("mu_staco_id_qoverp", &mu_staco_id_qoverp, &b_mu_staco_id_qoverp); }
  if( fChain->GetBranchStatus("mu_staco_me_d0") ) { fChain->SetBranchAddress("mu_staco_me_d0", &mu_staco_me_d0, &b_mu_staco_me_d0); }
  if( fChain->GetBranchStatus("mu_staco_me_z0") ) { fChain->SetBranchAddress("mu_staco_me_z0", &mu_staco_me_z0, &b_mu_staco_me_z0); }
  if( fChain->GetBranchStatus("mu_staco_me_phi") ) { fChain->SetBranchAddress("mu_staco_me_phi", &mu_staco_me_phi, &b_mu_staco_me_phi); }
  if( fChain->GetBranchStatus("mu_staco_me_theta") ) { fChain->SetBranchAddress("mu_staco_me_theta", &mu_staco_me_theta, &b_mu_staco_me_theta); }
  if( fChain->GetBranchStatus("mu_staco_me_qoverp") ) { fChain->SetBranchAddress("mu_staco_me_qoverp", &mu_staco_me_qoverp, &b_mu_staco_me_qoverp); }
  if( fChain->GetBranchStatus("mu_staco_ie_d0") ) { fChain->SetBranchAddress("mu_staco_ie_d0", &mu_staco_ie_d0, &b_mu_staco_ie_d0); }
  if( fChain->GetBranchStatus("mu_staco_ie_z0") ) { fChain->SetBranchAddress("mu_staco_ie_z0", &mu_staco_ie_z0, &b_mu_staco_ie_z0); }
  if( fChain->GetBranchStatus("mu_staco_ie_phi") ) { fChain->SetBranchAddress("mu_staco_ie_phi", &mu_staco_ie_phi, &b_mu_staco_ie_phi); }
  if( fChain->GetBranchStatus("mu_staco_ie_theta") ) { fChain->SetBranchAddress("mu_staco_ie_theta", &mu_staco_ie_theta, &b_mu_staco_ie_theta); }
  if( fChain->GetBranchStatus("mu_staco_ie_qoverp") ) { fChain->SetBranchAddress("mu_staco_ie_qoverp", &mu_staco_ie_qoverp, &b_mu_staco_ie_qoverp); }
  if( fChain->GetBranchStatus("mu_staco_nOutliersOnTrack") ) { fChain->SetBranchAddress("mu_staco_nOutliersOnTrack", &mu_staco_nOutliersOnTrack, &b_mu_staco_nOutliersOnTrack); }
  if( fChain->GetBranchStatus("mu_staco_nBLHits") ) { fChain->SetBranchAddress("mu_staco_nBLHits", &mu_staco_nBLHits, &b_mu_staco_nBLHits); }
  if( fChain->GetBranchStatus("mu_staco_nPixHits") ) { fChain->SetBranchAddress("mu_staco_nPixHits", &mu_staco_nPixHits, &b_mu_staco_nPixHits); }
  if( fChain->GetBranchStatus("mu_staco_nSCTHits") ) { fChain->SetBranchAddress("mu_staco_nSCTHits", &mu_staco_nSCTHits, &b_mu_staco_nSCTHits); }
  if( fChain->GetBranchStatus("mu_staco_nTRTHits") ) { fChain->SetBranchAddress("mu_staco_nTRTHits", &mu_staco_nTRTHits, &b_mu_staco_nTRTHits); }
  if( fChain->GetBranchStatus("mu_staco_nTRTHighTHits") ) { fChain->SetBranchAddress("mu_staco_nTRTHighTHits", &mu_staco_nTRTHighTHits, &b_mu_staco_nTRTHighTHits); }
  if( fChain->GetBranchStatus("mu_staco_nBLSharedHits") ) { fChain->SetBranchAddress("mu_staco_nBLSharedHits", &mu_staco_nBLSharedHits, &b_mu_staco_nBLSharedHits); }
  if( fChain->GetBranchStatus("mu_staco_nPixSharedHits") ) { fChain->SetBranchAddress("mu_staco_nPixSharedHits", &mu_staco_nPixSharedHits, &b_mu_staco_nPixSharedHits); }
  if( fChain->GetBranchStatus("mu_staco_nPixHoles") ) { fChain->SetBranchAddress("mu_staco_nPixHoles", &mu_staco_nPixHoles, &b_mu_staco_nPixHoles); }
  if( fChain->GetBranchStatus("mu_staco_nSCTSharedHits") ) { fChain->SetBranchAddress("mu_staco_nSCTSharedHits", &mu_staco_nSCTSharedHits, &b_mu_staco_nSCTSharedHits); }
  if( fChain->GetBranchStatus("mu_staco_nSCTHoles") ) { fChain->SetBranchAddress("mu_staco_nSCTHoles", &mu_staco_nSCTHoles, &b_mu_staco_nSCTHoles); }
  if( fChain->GetBranchStatus("mu_staco_nTRTOutliers") ) { fChain->SetBranchAddress("mu_staco_nTRTOutliers", &mu_staco_nTRTOutliers, &b_mu_staco_nTRTOutliers); }
  if( fChain->GetBranchStatus("mu_staco_nTRTHighTOutliers") ) { fChain->SetBranchAddress("mu_staco_nTRTHighTOutliers", &mu_staco_nTRTHighTOutliers, &b_mu_staco_nTRTHighTOutliers); }
  if( fChain->GetBranchStatus("mu_staco_nGangedPixels") ) { fChain->SetBranchAddress("mu_staco_nGangedPixels", &mu_staco_nGangedPixels, &b_mu_staco_nGangedPixels); }
  if( fChain->GetBranchStatus("mu_staco_nPixelDeadSensors") ) { fChain->SetBranchAddress("mu_staco_nPixelDeadSensors", &mu_staco_nPixelDeadSensors, &b_mu_staco_nPixelDeadSensors); }
  if( fChain->GetBranchStatus("mu_staco_nSCTDeadSensors") ) { fChain->SetBranchAddress("mu_staco_nSCTDeadSensors", &mu_staco_nSCTDeadSensors, &b_mu_staco_nSCTDeadSensors); }
  if( fChain->GetBranchStatus("mu_staco_nTRTDeadStraws") ) { fChain->SetBranchAddress("mu_staco_nTRTDeadStraws", &mu_staco_nTRTDeadStraws, &b_mu_staco_nTRTDeadStraws); }
  if( fChain->GetBranchStatus("mu_staco_expectBLayerHit") ) { fChain->SetBranchAddress("mu_staco_expectBLayerHit", &mu_staco_expectBLayerHit, &b_mu_staco_expectBLayerHit); }
  if( fChain->GetBranchStatus("mu_staco_nMDTHits") ) { fChain->SetBranchAddress("mu_staco_nMDTHits", &mu_staco_nMDTHits, &b_mu_staco_nMDTHits); }
  if( fChain->GetBranchStatus("mu_staco_nMDTHoles") ) { fChain->SetBranchAddress("mu_staco_nMDTHoles", &mu_staco_nMDTHoles, &b_mu_staco_nMDTHoles); }
  if( fChain->GetBranchStatus("mu_staco_nCSCEtaHits") ) { fChain->SetBranchAddress("mu_staco_nCSCEtaHits", &mu_staco_nCSCEtaHits, &b_mu_staco_nCSCEtaHits); }
  if( fChain->GetBranchStatus("mu_staco_nCSCEtaHoles") ) { fChain->SetBranchAddress("mu_staco_nCSCEtaHoles", &mu_staco_nCSCEtaHoles, &b_mu_staco_nCSCEtaHoles); }
  if( fChain->GetBranchStatus("mu_staco_nCSCPhiHits") ) { fChain->SetBranchAddress("mu_staco_nCSCPhiHits", &mu_staco_nCSCPhiHits, &b_mu_staco_nCSCPhiHits); }
  if( fChain->GetBranchStatus("mu_staco_nCSCPhiHoles") ) { fChain->SetBranchAddress("mu_staco_nCSCPhiHoles", &mu_staco_nCSCPhiHoles, &b_mu_staco_nCSCPhiHoles); }
  if( fChain->GetBranchStatus("mu_staco_nRPCEtaHits") ) { fChain->SetBranchAddress("mu_staco_nRPCEtaHits", &mu_staco_nRPCEtaHits, &b_mu_staco_nRPCEtaHits); }
  if( fChain->GetBranchStatus("mu_staco_nRPCEtaHoles") ) { fChain->SetBranchAddress("mu_staco_nRPCEtaHoles", &mu_staco_nRPCEtaHoles, &b_mu_staco_nRPCEtaHoles); }
  if( fChain->GetBranchStatus("mu_staco_nRPCPhiHits") ) { fChain->SetBranchAddress("mu_staco_nRPCPhiHits", &mu_staco_nRPCPhiHits, &b_mu_staco_nRPCPhiHits); }
  if( fChain->GetBranchStatus("mu_staco_nRPCPhiHoles") ) { fChain->SetBranchAddress("mu_staco_nRPCPhiHoles", &mu_staco_nRPCPhiHoles, &b_mu_staco_nRPCPhiHoles); }
  if( fChain->GetBranchStatus("mu_staco_nTGCEtaHits") ) { fChain->SetBranchAddress("mu_staco_nTGCEtaHits", &mu_staco_nTGCEtaHits, &b_mu_staco_nTGCEtaHits); }
  if( fChain->GetBranchStatus("mu_staco_nTGCEtaHoles") ) { fChain->SetBranchAddress("mu_staco_nTGCEtaHoles", &mu_staco_nTGCEtaHoles, &b_mu_staco_nTGCEtaHoles); }
  if( fChain->GetBranchStatus("mu_staco_nTGCPhiHits") ) { fChain->SetBranchAddress("mu_staco_nTGCPhiHits", &mu_staco_nTGCPhiHits, &b_mu_staco_nTGCPhiHits); }
  if( fChain->GetBranchStatus("mu_staco_nTGCPhiHoles") ) { fChain->SetBranchAddress("mu_staco_nTGCPhiHoles", &mu_staco_nTGCPhiHoles, &b_mu_staco_nTGCPhiHoles); }
  if( fChain->GetBranchStatus("mu_staco_nMDTBIHits") ) { fChain->SetBranchAddress("mu_staco_nMDTBIHits", &mu_staco_nMDTBIHits, &b_mu_staco_nMDTBIHits); }
  if( fChain->GetBranchStatus("mu_staco_nMDTBMHits") ) { fChain->SetBranchAddress("mu_staco_nMDTBMHits", &mu_staco_nMDTBMHits, &b_mu_staco_nMDTBMHits); }
  if( fChain->GetBranchStatus("mu_staco_nMDTBOHits") ) { fChain->SetBranchAddress("mu_staco_nMDTBOHits", &mu_staco_nMDTBOHits, &b_mu_staco_nMDTBOHits); }
  if( fChain->GetBranchStatus("mu_staco_nMDTBEEHits") ) { fChain->SetBranchAddress("mu_staco_nMDTBEEHits", &mu_staco_nMDTBEEHits, &b_mu_staco_nMDTBEEHits); }
  if( fChain->GetBranchStatus("mu_staco_nMDTBIS78Hits") ) { fChain->SetBranchAddress("mu_staco_nMDTBIS78Hits", &mu_staco_nMDTBIS78Hits, &b_mu_staco_nMDTBIS78Hits); }
  if( fChain->GetBranchStatus("mu_staco_nMDTEIHits") ) { fChain->SetBranchAddress("mu_staco_nMDTEIHits", &mu_staco_nMDTEIHits, &b_mu_staco_nMDTEIHits); }
  if( fChain->GetBranchStatus("mu_staco_nMDTEMHits") ) { fChain->SetBranchAddress("mu_staco_nMDTEMHits", &mu_staco_nMDTEMHits, &b_mu_staco_nMDTEMHits); }
  if( fChain->GetBranchStatus("mu_staco_nMDTEOHits") ) { fChain->SetBranchAddress("mu_staco_nMDTEOHits", &mu_staco_nMDTEOHits, &b_mu_staco_nMDTEOHits); }
  if( fChain->GetBranchStatus("mu_staco_nMDTEEHits") ) { fChain->SetBranchAddress("mu_staco_nMDTEEHits", &mu_staco_nMDTEEHits, &b_mu_staco_nMDTEEHits); }
  if( fChain->GetBranchStatus("mu_staco_nRPCLayer1EtaHits") ) { fChain->SetBranchAddress("mu_staco_nRPCLayer1EtaHits", &mu_staco_nRPCLayer1EtaHits, &b_mu_staco_nRPCLayer1EtaHits); }
  if( fChain->GetBranchStatus("mu_staco_nRPCLayer2EtaHits") ) { fChain->SetBranchAddress("mu_staco_nRPCLayer2EtaHits", &mu_staco_nRPCLayer2EtaHits, &b_mu_staco_nRPCLayer2EtaHits); }
  if( fChain->GetBranchStatus("mu_staco_nRPCLayer3EtaHits") ) { fChain->SetBranchAddress("mu_staco_nRPCLayer3EtaHits", &mu_staco_nRPCLayer3EtaHits, &b_mu_staco_nRPCLayer3EtaHits); }
  if( fChain->GetBranchStatus("mu_staco_nRPCLayer1PhiHits") ) { fChain->SetBranchAddress("mu_staco_nRPCLayer1PhiHits", &mu_staco_nRPCLayer1PhiHits, &b_mu_staco_nRPCLayer1PhiHits); }
  if( fChain->GetBranchStatus("mu_staco_nRPCLayer2PhiHits") ) { fChain->SetBranchAddress("mu_staco_nRPCLayer2PhiHits", &mu_staco_nRPCLayer2PhiHits, &b_mu_staco_nRPCLayer2PhiHits); }
  if( fChain->GetBranchStatus("mu_staco_nRPCLayer3PhiHits") ) { fChain->SetBranchAddress("mu_staco_nRPCLayer3PhiHits", &mu_staco_nRPCLayer3PhiHits, &b_mu_staco_nRPCLayer3PhiHits); }
  if( fChain->GetBranchStatus("mu_staco_nTGCLayer1EtaHits") ) { fChain->SetBranchAddress("mu_staco_nTGCLayer1EtaHits", &mu_staco_nTGCLayer1EtaHits, &b_mu_staco_nTGCLayer1EtaHits); }
  if( fChain->GetBranchStatus("mu_staco_nTGCLayer2EtaHits") ) { fChain->SetBranchAddress("mu_staco_nTGCLayer2EtaHits", &mu_staco_nTGCLayer2EtaHits, &b_mu_staco_nTGCLayer2EtaHits); }
  if( fChain->GetBranchStatus("mu_staco_nTGCLayer3EtaHits") ) { fChain->SetBranchAddress("mu_staco_nTGCLayer3EtaHits", &mu_staco_nTGCLayer3EtaHits, &b_mu_staco_nTGCLayer3EtaHits); }
  if( fChain->GetBranchStatus("mu_staco_nTGCLayer4EtaHits") ) { fChain->SetBranchAddress("mu_staco_nTGCLayer4EtaHits", &mu_staco_nTGCLayer4EtaHits, &b_mu_staco_nTGCLayer4EtaHits); }
  if( fChain->GetBranchStatus("mu_staco_nTGCLayer1PhiHits") ) { fChain->SetBranchAddress("mu_staco_nTGCLayer1PhiHits", &mu_staco_nTGCLayer1PhiHits, &b_mu_staco_nTGCLayer1PhiHits); }
  if( fChain->GetBranchStatus("mu_staco_nTGCLayer2PhiHits") ) { fChain->SetBranchAddress("mu_staco_nTGCLayer2PhiHits", &mu_staco_nTGCLayer2PhiHits, &b_mu_staco_nTGCLayer2PhiHits); }
  if( fChain->GetBranchStatus("mu_staco_nTGCLayer3PhiHits") ) { fChain->SetBranchAddress("mu_staco_nTGCLayer3PhiHits", &mu_staco_nTGCLayer3PhiHits, &b_mu_staco_nTGCLayer3PhiHits); }
  if( fChain->GetBranchStatus("mu_staco_nTGCLayer4PhiHits") ) { fChain->SetBranchAddress("mu_staco_nTGCLayer4PhiHits", &mu_staco_nTGCLayer4PhiHits, &b_mu_staco_nTGCLayer4PhiHits); }
  if( fChain->GetBranchStatus("mu_staco_barrelSectors") ) { fChain->SetBranchAddress("mu_staco_barrelSectors", &mu_staco_barrelSectors, &b_mu_staco_barrelSectors); }
  if( fChain->GetBranchStatus("mu_staco_endcapSectors") ) { fChain->SetBranchAddress("mu_staco_endcapSectors", &mu_staco_endcapSectors, &b_mu_staco_endcapSectors); }
  if( fChain->GetBranchStatus("mu_staco_trackd0") ) { fChain->SetBranchAddress("mu_staco_trackd0", &mu_staco_trackd0, &b_mu_staco_trackd0); }
  if( fChain->GetBranchStatus("mu_staco_trackz0") ) { fChain->SetBranchAddress("mu_staco_trackz0", &mu_staco_trackz0, &b_mu_staco_trackz0); }
  if( fChain->GetBranchStatus("mu_staco_trackphi") ) { fChain->SetBranchAddress("mu_staco_trackphi", &mu_staco_trackphi, &b_mu_staco_trackphi); }
  if( fChain->GetBranchStatus("mu_staco_tracktheta") ) { fChain->SetBranchAddress("mu_staco_tracktheta", &mu_staco_tracktheta, &b_mu_staco_tracktheta); }
  if( fChain->GetBranchStatus("mu_staco_trackqoverp") ) { fChain->SetBranchAddress("mu_staco_trackqoverp", &mu_staco_trackqoverp, &b_mu_staco_trackqoverp); }
  if( fChain->GetBranchStatus("mu_staco_trackcov_d0") ) { fChain->SetBranchAddress("mu_staco_trackcov_d0", &mu_staco_trackcov_d0, &b_mu_staco_trackcov_d0); }
  if( fChain->GetBranchStatus("mu_staco_trackcov_z0") ) { fChain->SetBranchAddress("mu_staco_trackcov_z0", &mu_staco_trackcov_z0, &b_mu_staco_trackcov_z0); }
  if( fChain->GetBranchStatus("mu_staco_trackcov_phi") ) { fChain->SetBranchAddress("mu_staco_trackcov_phi", &mu_staco_trackcov_phi, &b_mu_staco_trackcov_phi); }
  if( fChain->GetBranchStatus("mu_staco_trackcov_theta") ) { fChain->SetBranchAddress("mu_staco_trackcov_theta", &mu_staco_trackcov_theta, &b_mu_staco_trackcov_theta); }
  if( fChain->GetBranchStatus("mu_staco_trackcov_qoverp") ) { fChain->SetBranchAddress("mu_staco_trackcov_qoverp", &mu_staco_trackcov_qoverp, &b_mu_staco_trackcov_qoverp); }
  if( fChain->GetBranchStatus("mu_staco_trackcov_d0_z0") ) { fChain->SetBranchAddress("mu_staco_trackcov_d0_z0", &mu_staco_trackcov_d0_z0, &b_mu_staco_trackcov_d0_z0); }
  if( fChain->GetBranchStatus("mu_staco_trackcov_d0_phi") ) { fChain->SetBranchAddress("mu_staco_trackcov_d0_phi", &mu_staco_trackcov_d0_phi, &b_mu_staco_trackcov_d0_phi); }
  if( fChain->GetBranchStatus("mu_staco_trackcov_d0_theta") ) { fChain->SetBranchAddress("mu_staco_trackcov_d0_theta", &mu_staco_trackcov_d0_theta, &b_mu_staco_trackcov_d0_theta); }
  if( fChain->GetBranchStatus("mu_staco_trackcov_d0_qoverp") ) { fChain->SetBranchAddress("mu_staco_trackcov_d0_qoverp", &mu_staco_trackcov_d0_qoverp, &b_mu_staco_trackcov_d0_qoverp); }
  if( fChain->GetBranchStatus("mu_staco_trackcov_z0_phi") ) { fChain->SetBranchAddress("mu_staco_trackcov_z0_phi", &mu_staco_trackcov_z0_phi, &b_mu_staco_trackcov_z0_phi); }
  if( fChain->GetBranchStatus("mu_staco_trackcov_z0_theta") ) { fChain->SetBranchAddress("mu_staco_trackcov_z0_theta", &mu_staco_trackcov_z0_theta, &b_mu_staco_trackcov_z0_theta); }
  if( fChain->GetBranchStatus("mu_staco_trackcov_z0_qoverp") ) { fChain->SetBranchAddress("mu_staco_trackcov_z0_qoverp", &mu_staco_trackcov_z0_qoverp, &b_mu_staco_trackcov_z0_qoverp); }
  if( fChain->GetBranchStatus("mu_staco_trackcov_phi_theta") ) { fChain->SetBranchAddress("mu_staco_trackcov_phi_theta", &mu_staco_trackcov_phi_theta, &b_mu_staco_trackcov_phi_theta); }
  if( fChain->GetBranchStatus("mu_staco_trackcov_phi_qoverp") ) { fChain->SetBranchAddress("mu_staco_trackcov_phi_qoverp", &mu_staco_trackcov_phi_qoverp, &b_mu_staco_trackcov_phi_qoverp); }
  if( fChain->GetBranchStatus("mu_staco_trackcov_theta_qoverp") ) { fChain->SetBranchAddress("mu_staco_trackcov_theta_qoverp", &mu_staco_trackcov_theta_qoverp, &b_mu_staco_trackcov_theta_qoverp); }
  if( fChain->GetBranchStatus("mu_staco_trackfitchi2") ) { fChain->SetBranchAddress("mu_staco_trackfitchi2", &mu_staco_trackfitchi2, &b_mu_staco_trackfitchi2); }
  if( fChain->GetBranchStatus("mu_staco_trackfitndof") ) { fChain->SetBranchAddress("mu_staco_trackfitndof", &mu_staco_trackfitndof, &b_mu_staco_trackfitndof); }
  if( fChain->GetBranchStatus("mu_staco_hastrack") ) { fChain->SetBranchAddress("mu_staco_hastrack", &mu_staco_hastrack, &b_mu_staco_hastrack); }
  if( fChain->GetBranchStatus("mu_staco_truth_dr") ) { fChain->SetBranchAddress("mu_staco_truth_dr", &mu_staco_truth_dr, &b_mu_staco_truth_dr); }
  if( fChain->GetBranchStatus("mu_staco_truth_E") ) { fChain->SetBranchAddress("mu_staco_truth_E", &mu_staco_truth_E, &b_mu_staco_truth_E); }
  if( fChain->GetBranchStatus("mu_staco_truth_pt") ) { fChain->SetBranchAddress("mu_staco_truth_pt", &mu_staco_truth_pt, &b_mu_staco_truth_pt); }
  if( fChain->GetBranchStatus("mu_staco_truth_eta") ) { fChain->SetBranchAddress("mu_staco_truth_eta", &mu_staco_truth_eta, &b_mu_staco_truth_eta); }
  if( fChain->GetBranchStatus("mu_staco_truth_phi") ) { fChain->SetBranchAddress("mu_staco_truth_phi", &mu_staco_truth_phi, &b_mu_staco_truth_phi); }
  if( fChain->GetBranchStatus("mu_staco_truth_type") ) { fChain->SetBranchAddress("mu_staco_truth_type", &mu_staco_truth_type, &b_mu_staco_truth_type); }
  if( fChain->GetBranchStatus("mu_staco_truth_status") ) { fChain->SetBranchAddress("mu_staco_truth_status", &mu_staco_truth_status, &b_mu_staco_truth_status); }
  if( fChain->GetBranchStatus("mu_staco_truth_barcode") ) { fChain->SetBranchAddress("mu_staco_truth_barcode", &mu_staco_truth_barcode, &b_mu_staco_truth_barcode); }
  if( fChain->GetBranchStatus("mu_staco_truth_mothertype") ) { fChain->SetBranchAddress("mu_staco_truth_mothertype", &mu_staco_truth_mothertype, &b_mu_staco_truth_mothertype); }
  if( fChain->GetBranchStatus("mu_staco_truth_motherbarcode") ) { fChain->SetBranchAddress("mu_staco_truth_motherbarcode", &mu_staco_truth_motherbarcode, &b_mu_staco_truth_motherbarcode); }
  if( fChain->GetBranchStatus("mu_staco_truth_matched") ) { fChain->SetBranchAddress("mu_staco_truth_matched", &mu_staco_truth_matched, &b_mu_staco_truth_matched); }
  if( fChain->GetBranchStatus("mu_staco_EFCB_dr") ) { fChain->SetBranchAddress("mu_staco_EFCB_dr", &mu_staco_EFCB_dr, &b_mu_staco_EFCB_dr); }
  if( fChain->GetBranchStatus("mu_staco_EFCB_index") ) { fChain->SetBranchAddress("mu_staco_EFCB_index", &mu_staco_EFCB_index, &b_mu_staco_EFCB_index); }
  if( fChain->GetBranchStatus("mu_staco_EFMG_dr") ) { fChain->SetBranchAddress("mu_staco_EFMG_dr", &mu_staco_EFMG_dr, &b_mu_staco_EFMG_dr); }
  if( fChain->GetBranchStatus("mu_staco_EFMG_index") ) { fChain->SetBranchAddress("mu_staco_EFMG_index", &mu_staco_EFMG_index, &b_mu_staco_EFMG_index); }
  if( fChain->GetBranchStatus("mu_staco_EFME_dr") ) { fChain->SetBranchAddress("mu_staco_EFME_dr", &mu_staco_EFME_dr, &b_mu_staco_EFME_dr); }
  if( fChain->GetBranchStatus("mu_staco_EFME_index") ) { fChain->SetBranchAddress("mu_staco_EFME_index", &mu_staco_EFME_index, &b_mu_staco_EFME_index); }
  if( fChain->GetBranchStatus("mu_staco_L2CB_dr") ) { fChain->SetBranchAddress("mu_staco_L2CB_dr", &mu_staco_L2CB_dr, &b_mu_staco_L2CB_dr); }
  if( fChain->GetBranchStatus("mu_staco_L2CB_index") ) { fChain->SetBranchAddress("mu_staco_L2CB_index", &mu_staco_L2CB_index, &b_mu_staco_L2CB_index); }
  if( fChain->GetBranchStatus("mu_staco_L1_dr") ) { fChain->SetBranchAddress("mu_staco_L1_dr", &mu_staco_L1_dr, &b_mu_staco_L1_dr); }
  if( fChain->GetBranchStatus("mu_staco_L1_index") ) { fChain->SetBranchAddress("mu_staco_L1_index", &mu_staco_L1_index, &b_mu_staco_L1_index); }
  if( fChain->GetBranchStatus("mu_calo_n") ) { fChain->SetBranchAddress("mu_calo_n", &mu_calo_n, &b_mu_calo_n); }
  if( fChain->GetBranchStatus("mu_calo_E") ) { fChain->SetBranchAddress("mu_calo_E", &mu_calo_E, &b_mu_calo_E); }
  if( fChain->GetBranchStatus("mu_calo_pt") ) { fChain->SetBranchAddress("mu_calo_pt", &mu_calo_pt, &b_mu_calo_pt); }
  if( fChain->GetBranchStatus("mu_calo_m") ) { fChain->SetBranchAddress("mu_calo_m", &mu_calo_m, &b_mu_calo_m); }
  if( fChain->GetBranchStatus("mu_calo_eta") ) { fChain->SetBranchAddress("mu_calo_eta", &mu_calo_eta, &b_mu_calo_eta); }
  if( fChain->GetBranchStatus("mu_calo_phi") ) { fChain->SetBranchAddress("mu_calo_phi", &mu_calo_phi, &b_mu_calo_phi); }
  if( fChain->GetBranchStatus("mu_calo_px") ) { fChain->SetBranchAddress("mu_calo_px", &mu_calo_px, &b_mu_calo_px); }
  if( fChain->GetBranchStatus("mu_calo_py") ) { fChain->SetBranchAddress("mu_calo_py", &mu_calo_py, &b_mu_calo_py); }
  if( fChain->GetBranchStatus("mu_calo_pz") ) { fChain->SetBranchAddress("mu_calo_pz", &mu_calo_pz, &b_mu_calo_pz); }
  if( fChain->GetBranchStatus("mu_calo_charge") ) { fChain->SetBranchAddress("mu_calo_charge", &mu_calo_charge, &b_mu_calo_charge); }
  if( fChain->GetBranchStatus("mu_calo_allauthor") ) { fChain->SetBranchAddress("mu_calo_allauthor", &mu_calo_allauthor, &b_mu_calo_allauthor); }
  if( fChain->GetBranchStatus("mu_calo_hastrack") ) { fChain->SetBranchAddress("mu_calo_hastrack", &mu_calo_hastrack, &b_mu_calo_hastrack); }
  if( fChain->GetBranchStatus("mu_calo_truth_dr") ) { fChain->SetBranchAddress("mu_calo_truth_dr", &mu_calo_truth_dr, &b_mu_calo_truth_dr); }
  if( fChain->GetBranchStatus("mu_calo_truth_E") ) { fChain->SetBranchAddress("mu_calo_truth_E", &mu_calo_truth_E, &b_mu_calo_truth_E); }
  if( fChain->GetBranchStatus("mu_calo_truth_pt") ) { fChain->SetBranchAddress("mu_calo_truth_pt", &mu_calo_truth_pt, &b_mu_calo_truth_pt); }
  if( fChain->GetBranchStatus("mu_calo_truth_eta") ) { fChain->SetBranchAddress("mu_calo_truth_eta", &mu_calo_truth_eta, &b_mu_calo_truth_eta); }
  if( fChain->GetBranchStatus("mu_calo_truth_phi") ) { fChain->SetBranchAddress("mu_calo_truth_phi", &mu_calo_truth_phi, &b_mu_calo_truth_phi); }
  if( fChain->GetBranchStatus("mu_calo_truth_type") ) { fChain->SetBranchAddress("mu_calo_truth_type", &mu_calo_truth_type, &b_mu_calo_truth_type); }
  if( fChain->GetBranchStatus("mu_calo_truth_status") ) { fChain->SetBranchAddress("mu_calo_truth_status", &mu_calo_truth_status, &b_mu_calo_truth_status); }
  if( fChain->GetBranchStatus("mu_calo_truth_barcode") ) { fChain->SetBranchAddress("mu_calo_truth_barcode", &mu_calo_truth_barcode, &b_mu_calo_truth_barcode); }
  if( fChain->GetBranchStatus("mu_calo_truth_mothertype") ) { fChain->SetBranchAddress("mu_calo_truth_mothertype", &mu_calo_truth_mothertype, &b_mu_calo_truth_mothertype); }
  if( fChain->GetBranchStatus("mu_calo_truth_motherbarcode") ) { fChain->SetBranchAddress("mu_calo_truth_motherbarcode", &mu_calo_truth_motherbarcode, &b_mu_calo_truth_motherbarcode); }
  if( fChain->GetBranchStatus("mu_calo_truth_matched") ) { fChain->SetBranchAddress("mu_calo_truth_matched", &mu_calo_truth_matched, &b_mu_calo_truth_matched); }
  if( fChain->GetBranchStatus("tau_n") ) { fChain->SetBranchAddress("tau_n", &tau_n, &b_tau_n); }
  if( fChain->GetBranchStatus("tau_Et") ) { fChain->SetBranchAddress("tau_Et", &tau_Et, &b_tau_Et); }
  if( fChain->GetBranchStatus("tau_pt") ) { fChain->SetBranchAddress("tau_pt", &tau_pt, &b_tau_pt); }
  if( fChain->GetBranchStatus("tau_m") ) { fChain->SetBranchAddress("tau_m", &tau_m, &b_tau_m); }
  if( fChain->GetBranchStatus("tau_eta") ) { fChain->SetBranchAddress("tau_eta", &tau_eta, &b_tau_eta); }
  if( fChain->GetBranchStatus("tau_phi") ) { fChain->SetBranchAddress("tau_phi", &tau_phi, &b_tau_phi); }
  if( fChain->GetBranchStatus("tau_charge") ) { fChain->SetBranchAddress("tau_charge", &tau_charge, &b_tau_charge); }
  if( fChain->GetBranchStatus("tau_BDTEleScore") ) { fChain->SetBranchAddress("tau_BDTEleScore", &tau_BDTEleScore, &b_tau_BDTEleScore); }
  if( fChain->GetBranchStatus("tau_BDTJetScore") ) { fChain->SetBranchAddress("tau_BDTJetScore", &tau_BDTJetScore, &b_tau_BDTJetScore); }
  if( fChain->GetBranchStatus("tau_discCut") ) { fChain->SetBranchAddress("tau_discCut", &tau_discCut, &b_tau_discCut); }
  if( fChain->GetBranchStatus("tau_discCutTMVA") ) { fChain->SetBranchAddress("tau_discCutTMVA", &tau_discCutTMVA, &b_tau_discCutTMVA); }
  if( fChain->GetBranchStatus("tau_discLL") ) { fChain->SetBranchAddress("tau_discLL", &tau_discLL, &b_tau_discLL); }
  if( fChain->GetBranchStatus("tau_discNN") ) { fChain->SetBranchAddress("tau_discNN", &tau_discNN, &b_tau_discNN); }
  if( fChain->GetBranchStatus("tau_efficLL") ) { fChain->SetBranchAddress("tau_efficLL", &tau_efficLL, &b_tau_efficLL); }
  if( fChain->GetBranchStatus("tau_efficNN") ) { fChain->SetBranchAddress("tau_efficNN", &tau_efficNN, &b_tau_efficNN); }
  if( fChain->GetBranchStatus("tau_likelihood") ) { fChain->SetBranchAddress("tau_likelihood", &tau_likelihood, &b_tau_likelihood); }
  if( fChain->GetBranchStatus("tau_tauJetNeuralNetwork") ) { fChain->SetBranchAddress("tau_tauJetNeuralNetwork", &tau_tauJetNeuralNetwork, &b_tau_tauJetNeuralNetwork); }
  if( fChain->GetBranchStatus("tau_tauENeuralNetwork") ) { fChain->SetBranchAddress("tau_tauENeuralNetwork", &tau_tauENeuralNetwork, &b_tau_tauENeuralNetwork); }
  if( fChain->GetBranchStatus("tau_tauElTauLikelihood") ) { fChain->SetBranchAddress("tau_tauElTauLikelihood", &tau_tauElTauLikelihood, &b_tau_tauElTauLikelihood); }
  if( fChain->GetBranchStatus("tau_SafeLikelihood") ) { fChain->SetBranchAddress("tau_SafeLikelihood", &tau_SafeLikelihood, &b_tau_SafeLikelihood); }
  if( fChain->GetBranchStatus("tau_electronVetoLoose") ) { fChain->SetBranchAddress("tau_electronVetoLoose", &tau_electronVetoLoose, &b_tau_electronVetoLoose); }
  if( fChain->GetBranchStatus("tau_electronVetoMedium") ) { fChain->SetBranchAddress("tau_electronVetoMedium", &tau_electronVetoMedium, &b_tau_electronVetoMedium); }
  if( fChain->GetBranchStatus("tau_electronVetoTight") ) { fChain->SetBranchAddress("tau_electronVetoTight", &tau_electronVetoTight, &b_tau_electronVetoTight); }
  if( fChain->GetBranchStatus("tau_muonVeto") ) { fChain->SetBranchAddress("tau_muonVeto", &tau_muonVeto, &b_tau_muonVeto); }
  if( fChain->GetBranchStatus("tau_tauCutLoose") ) { fChain->SetBranchAddress("tau_tauCutLoose", &tau_tauCutLoose, &b_tau_tauCutLoose); }
  if( fChain->GetBranchStatus("tau_tauCutMedium") ) { fChain->SetBranchAddress("tau_tauCutMedium", &tau_tauCutMedium, &b_tau_tauCutMedium); }
  if( fChain->GetBranchStatus("tau_tauCutTight") ) { fChain->SetBranchAddress("tau_tauCutTight", &tau_tauCutTight, &b_tau_tauCutTight); }
  if( fChain->GetBranchStatus("tau_tauCutSafeLoose") ) { fChain->SetBranchAddress("tau_tauCutSafeLoose", &tau_tauCutSafeLoose, &b_tau_tauCutSafeLoose); }
  if( fChain->GetBranchStatus("tau_tauCutSafeMedium") ) { fChain->SetBranchAddress("tau_tauCutSafeMedium", &tau_tauCutSafeMedium, &b_tau_tauCutSafeMedium); }
  if( fChain->GetBranchStatus("tau_tauCutSafeTight") ) { fChain->SetBranchAddress("tau_tauCutSafeTight", &tau_tauCutSafeTight, &b_tau_tauCutSafeTight); }
  if( fChain->GetBranchStatus("tau_tauCutSafeCaloLoose") ) { fChain->SetBranchAddress("tau_tauCutSafeCaloLoose", &tau_tauCutSafeCaloLoose, &b_tau_tauCutSafeCaloLoose); }
  if( fChain->GetBranchStatus("tau_tauCutSafeCaloMedium") ) { fChain->SetBranchAddress("tau_tauCutSafeCaloMedium", &tau_tauCutSafeCaloMedium, &b_tau_tauCutSafeCaloMedium); }
  if( fChain->GetBranchStatus("tau_tauCutSafeCaloTight") ) { fChain->SetBranchAddress("tau_tauCutSafeCaloTight", &tau_tauCutSafeCaloTight, &b_tau_tauCutSafeCaloTight); }
  if( fChain->GetBranchStatus("tau_tauLlhLoose") ) { fChain->SetBranchAddress("tau_tauLlhLoose", &tau_tauLlhLoose, &b_tau_tauLlhLoose); }
  if( fChain->GetBranchStatus("tau_tauLlhMedium") ) { fChain->SetBranchAddress("tau_tauLlhMedium", &tau_tauLlhMedium, &b_tau_tauLlhMedium); }
  if( fChain->GetBranchStatus("tau_tauLlhTight") ) { fChain->SetBranchAddress("tau_tauLlhTight", &tau_tauLlhTight, &b_tau_tauLlhTight); }
  if( fChain->GetBranchStatus("tau_JetBDTLoose") ) { fChain->SetBranchAddress("tau_JetBDTLoose", &tau_JetBDTLoose, &b_tau_JetBDTLoose); }
  if( fChain->GetBranchStatus("tau_JetBDTMedium") ) { fChain->SetBranchAddress("tau_JetBDTMedium", &tau_JetBDTMedium, &b_tau_JetBDTMedium); }
  if( fChain->GetBranchStatus("tau_JetBDTTight") ) { fChain->SetBranchAddress("tau_JetBDTTight", &tau_JetBDTTight, &b_tau_JetBDTTight); }
  if( fChain->GetBranchStatus("tau_EleBDTLoose") ) { fChain->SetBranchAddress("tau_EleBDTLoose", &tau_EleBDTLoose, &b_tau_EleBDTLoose); }
  if( fChain->GetBranchStatus("tau_EleBDTMedium") ) { fChain->SetBranchAddress("tau_EleBDTMedium", &tau_EleBDTMedium, &b_tau_EleBDTMedium); }
  if( fChain->GetBranchStatus("tau_EleBDTTight") ) { fChain->SetBranchAddress("tau_EleBDTTight", &tau_EleBDTTight, &b_tau_EleBDTTight); }
  if( fChain->GetBranchStatus("tau_SafeLlhLoose") ) { fChain->SetBranchAddress("tau_SafeLlhLoose", &tau_SafeLlhLoose, &b_tau_SafeLlhLoose); }
  if( fChain->GetBranchStatus("tau_SafeLlhMedium") ) { fChain->SetBranchAddress("tau_SafeLlhMedium", &tau_SafeLlhMedium, &b_tau_SafeLlhMedium); }
  if( fChain->GetBranchStatus("tau_SafeLlhTight") ) { fChain->SetBranchAddress("tau_SafeLlhTight", &tau_SafeLlhTight, &b_tau_SafeLlhTight); }
  if( fChain->GetBranchStatus("tau_author") ) { fChain->SetBranchAddress("tau_author", &tau_author, &b_tau_author); }
  if( fChain->GetBranchStatus("tau_ROIword") ) { fChain->SetBranchAddress("tau_ROIword", &tau_ROIword, &b_tau_ROIword); }
  if( fChain->GetBranchStatus("tau_nProng") ) { fChain->SetBranchAddress("tau_nProng", &tau_nProng, &b_tau_nProng); }
  if( fChain->GetBranchStatus("tau_numTrack") ) { fChain->SetBranchAddress("tau_numTrack", &tau_numTrack, &b_tau_numTrack); }
  if( fChain->GetBranchStatus("tau_seedCalo_numTrack") ) { fChain->SetBranchAddress("tau_seedCalo_numTrack", &tau_seedCalo_numTrack, &b_tau_seedCalo_numTrack); }
  if( fChain->GetBranchStatus("tau_etOverPtLeadTrk") ) { fChain->SetBranchAddress("tau_etOverPtLeadTrk", &tau_etOverPtLeadTrk, &b_tau_etOverPtLeadTrk); }
  if( fChain->GetBranchStatus("tau_ipZ0SinThetaSigLeadTrk") ) { fChain->SetBranchAddress("tau_ipZ0SinThetaSigLeadTrk", &tau_ipZ0SinThetaSigLeadTrk, &b_tau_ipZ0SinThetaSigLeadTrk); }
  if( fChain->GetBranchStatus("tau_leadTrkPt") ) { fChain->SetBranchAddress("tau_leadTrkPt", &tau_leadTrkPt, &b_tau_leadTrkPt); }
  if( fChain->GetBranchStatus("tau_nLooseTrk") ) { fChain->SetBranchAddress("tau_nLooseTrk", &tau_nLooseTrk, &b_tau_nLooseTrk); }
  if( fChain->GetBranchStatus("tau_nLooseConvTrk") ) { fChain->SetBranchAddress("tau_nLooseConvTrk", &tau_nLooseConvTrk, &b_tau_nLooseConvTrk); }
  if( fChain->GetBranchStatus("tau_nProngLoose") ) { fChain->SetBranchAddress("tau_nProngLoose", &tau_nProngLoose, &b_tau_nProngLoose); }
  if( fChain->GetBranchStatus("tau_ipSigLeadTrk") ) { fChain->SetBranchAddress("tau_ipSigLeadTrk", &tau_ipSigLeadTrk, &b_tau_ipSigLeadTrk); }
  if( fChain->GetBranchStatus("tau_ipSigLeadLooseTrk") ) { fChain->SetBranchAddress("tau_ipSigLeadLooseTrk", &tau_ipSigLeadLooseTrk, &b_tau_ipSigLeadLooseTrk); }
  if( fChain->GetBranchStatus("tau_etOverPtLeadLooseTrk") ) { fChain->SetBranchAddress("tau_etOverPtLeadLooseTrk", &tau_etOverPtLeadLooseTrk, &b_tau_etOverPtLeadLooseTrk); }
  if( fChain->GetBranchStatus("tau_leadLooseTrkPt") ) { fChain->SetBranchAddress("tau_leadLooseTrkPt", &tau_leadLooseTrkPt, &b_tau_leadLooseTrkPt); }
  if( fChain->GetBranchStatus("tau_chrgLooseTrk") ) { fChain->SetBranchAddress("tau_chrgLooseTrk", &tau_chrgLooseTrk, &b_tau_chrgLooseTrk); }
  if( fChain->GetBranchStatus("tau_massTrkSys") ) { fChain->SetBranchAddress("tau_massTrkSys", &tau_massTrkSys, &b_tau_massTrkSys); }
  if( fChain->GetBranchStatus("tau_trkWidth2") ) { fChain->SetBranchAddress("tau_trkWidth2", &tau_trkWidth2, &b_tau_trkWidth2); }
  if( fChain->GetBranchStatus("tau_trFlightPathSig") ) { fChain->SetBranchAddress("tau_trFlightPathSig", &tau_trFlightPathSig, &b_tau_trFlightPathSig); }
  if( fChain->GetBranchStatus("tau_etEflow") ) { fChain->SetBranchAddress("tau_etEflow", &tau_etEflow, &b_tau_etEflow); }
  if( fChain->GetBranchStatus("tau_mEflow") ) { fChain->SetBranchAddress("tau_mEflow", &tau_mEflow, &b_tau_mEflow); }
  if( fChain->GetBranchStatus("tau_nPi0") ) { fChain->SetBranchAddress("tau_nPi0", &tau_nPi0, &b_tau_nPi0); }
  if( fChain->GetBranchStatus("tau_ele_E237E277") ) { fChain->SetBranchAddress("tau_ele_E237E277", &tau_ele_E237E277, &b_tau_ele_E237E277); }
  if( fChain->GetBranchStatus("tau_ele_PresamplerFraction") ) { fChain->SetBranchAddress("tau_ele_PresamplerFraction", &tau_ele_PresamplerFraction, &b_tau_ele_PresamplerFraction); }
  if( fChain->GetBranchStatus("tau_ele_ECALFirstFraction") ) { fChain->SetBranchAddress("tau_ele_ECALFirstFraction", &tau_ele_ECALFirstFraction, &b_tau_ele_ECALFirstFraction); }
  if( fChain->GetBranchStatus("tau_seedCalo_EMRadius") ) { fChain->SetBranchAddress("tau_seedCalo_EMRadius", &tau_seedCalo_EMRadius, &b_tau_seedCalo_EMRadius); }
  if( fChain->GetBranchStatus("tau_seedCalo_hadRadius") ) { fChain->SetBranchAddress("tau_seedCalo_hadRadius", &tau_seedCalo_hadRadius, &b_tau_seedCalo_hadRadius); }
  if( fChain->GetBranchStatus("tau_seedCalo_etEMAtEMScale") ) { fChain->SetBranchAddress("tau_seedCalo_etEMAtEMScale", &tau_seedCalo_etEMAtEMScale, &b_tau_seedCalo_etEMAtEMScale); }
  if( fChain->GetBranchStatus("tau_seedCalo_etHadAtEMScale") ) { fChain->SetBranchAddress("tau_seedCalo_etHadAtEMScale", &tau_seedCalo_etHadAtEMScale, &b_tau_seedCalo_etHadAtEMScale); }
  if( fChain->GetBranchStatus("tau_seedCalo_isolFrac") ) { fChain->SetBranchAddress("tau_seedCalo_isolFrac", &tau_seedCalo_isolFrac, &b_tau_seedCalo_isolFrac); }
  if( fChain->GetBranchStatus("tau_seedCalo_centFrac") ) { fChain->SetBranchAddress("tau_seedCalo_centFrac", &tau_seedCalo_centFrac, &b_tau_seedCalo_centFrac); }
  if( fChain->GetBranchStatus("tau_seedCalo_stripWidth2") ) { fChain->SetBranchAddress("tau_seedCalo_stripWidth2", &tau_seedCalo_stripWidth2, &b_tau_seedCalo_stripWidth2); }
  if( fChain->GetBranchStatus("tau_seedCalo_nStrip") ) { fChain->SetBranchAddress("tau_seedCalo_nStrip", &tau_seedCalo_nStrip, &b_tau_seedCalo_nStrip); }
  if( fChain->GetBranchStatus("tau_seedCalo_etEMCalib") ) { fChain->SetBranchAddress("tau_seedCalo_etEMCalib", &tau_seedCalo_etEMCalib, &b_tau_seedCalo_etEMCalib); }
  if( fChain->GetBranchStatus("tau_seedCalo_etHadCalib") ) { fChain->SetBranchAddress("tau_seedCalo_etHadCalib", &tau_seedCalo_etHadCalib, &b_tau_seedCalo_etHadCalib); }
  if( fChain->GetBranchStatus("tau_seedCalo_eta") ) { fChain->SetBranchAddress("tau_seedCalo_eta", &tau_seedCalo_eta, &b_tau_seedCalo_eta); }
  if( fChain->GetBranchStatus("tau_seedCalo_phi") ) { fChain->SetBranchAddress("tau_seedCalo_phi", &tau_seedCalo_phi, &b_tau_seedCalo_phi); }
  if( fChain->GetBranchStatus("tau_seedCalo_nIsolLooseTrk") ) { fChain->SetBranchAddress("tau_seedCalo_nIsolLooseTrk", &tau_seedCalo_nIsolLooseTrk, &b_tau_seedCalo_nIsolLooseTrk); }
  if( fChain->GetBranchStatus("tau_seedCalo_trkAvgDist") ) { fChain->SetBranchAddress("tau_seedCalo_trkAvgDist", &tau_seedCalo_trkAvgDist, &b_tau_seedCalo_trkAvgDist); }
  if( fChain->GetBranchStatus("tau_seedCalo_trkRmsDist") ) { fChain->SetBranchAddress("tau_seedCalo_trkRmsDist", &tau_seedCalo_trkRmsDist, &b_tau_seedCalo_trkRmsDist); }
  if( fChain->GetBranchStatus("tau_numTopoClusters") ) { fChain->SetBranchAddress("tau_numTopoClusters", &tau_numTopoClusters, &b_tau_numTopoClusters); }
  if( fChain->GetBranchStatus("tau_numEffTopoClusters") ) { fChain->SetBranchAddress("tau_numEffTopoClusters", &tau_numEffTopoClusters, &b_tau_numEffTopoClusters); }
  if( fChain->GetBranchStatus("tau_topoInvMass") ) { fChain->SetBranchAddress("tau_topoInvMass", &tau_topoInvMass, &b_tau_topoInvMass); }
  if( fChain->GetBranchStatus("tau_effTopoInvMass") ) { fChain->SetBranchAddress("tau_effTopoInvMass", &tau_effTopoInvMass, &b_tau_effTopoInvMass); }
  if( fChain->GetBranchStatus("tau_topoMeanDeltaR") ) { fChain->SetBranchAddress("tau_topoMeanDeltaR", &tau_topoMeanDeltaR, &b_tau_topoMeanDeltaR); }
  if( fChain->GetBranchStatus("tau_effTopoMeanDeltaR") ) { fChain->SetBranchAddress("tau_effTopoMeanDeltaR", &tau_effTopoMeanDeltaR, &b_tau_effTopoMeanDeltaR); }
  if( fChain->GetBranchStatus("tau_numCells") ) { fChain->SetBranchAddress("tau_numCells", &tau_numCells, &b_tau_numCells); }
  if( fChain->GetBranchStatus("tau_seedTrk_EMRadius") ) { fChain->SetBranchAddress("tau_seedTrk_EMRadius", &tau_seedTrk_EMRadius, &b_tau_seedTrk_EMRadius); }
  if( fChain->GetBranchStatus("tau_seedTrk_isolFrac") ) { fChain->SetBranchAddress("tau_seedTrk_isolFrac", &tau_seedTrk_isolFrac, &b_tau_seedTrk_isolFrac); }
  if( fChain->GetBranchStatus("tau_seedTrk_etChrgHadOverSumTrkPt") ) { fChain->SetBranchAddress("tau_seedTrk_etChrgHadOverSumTrkPt", &tau_seedTrk_etChrgHadOverSumTrkPt, &b_tau_seedTrk_etChrgHadOverSumTrkPt); }
  if( fChain->GetBranchStatus("tau_seedTrk_isolFracWide") ) { fChain->SetBranchAddress("tau_seedTrk_isolFracWide", &tau_seedTrk_isolFracWide, &b_tau_seedTrk_isolFracWide); }
  if( fChain->GetBranchStatus("tau_seedTrk_etHadAtEMScale") ) { fChain->SetBranchAddress("tau_seedTrk_etHadAtEMScale", &tau_seedTrk_etHadAtEMScale, &b_tau_seedTrk_etHadAtEMScale); }
  if( fChain->GetBranchStatus("tau_seedTrk_etEMAtEMScale") ) { fChain->SetBranchAddress("tau_seedTrk_etEMAtEMScale", &tau_seedTrk_etEMAtEMScale, &b_tau_seedTrk_etEMAtEMScale); }
  if( fChain->GetBranchStatus("tau_seedTrk_etEMCL") ) { fChain->SetBranchAddress("tau_seedTrk_etEMCL", &tau_seedTrk_etEMCL, &b_tau_seedTrk_etEMCL); }
  if( fChain->GetBranchStatus("tau_seedTrk_etChrgEM") ) { fChain->SetBranchAddress("tau_seedTrk_etChrgEM", &tau_seedTrk_etChrgEM, &b_tau_seedTrk_etChrgEM); }
  if( fChain->GetBranchStatus("tau_seedTrk_etNeuEM") ) { fChain->SetBranchAddress("tau_seedTrk_etNeuEM", &tau_seedTrk_etNeuEM, &b_tau_seedTrk_etNeuEM); }
  if( fChain->GetBranchStatus("tau_seedTrk_etResNeuEM") ) { fChain->SetBranchAddress("tau_seedTrk_etResNeuEM", &tau_seedTrk_etResNeuEM, &b_tau_seedTrk_etResNeuEM); }
  if( fChain->GetBranchStatus("tau_seedTrk_hadLeakEt") ) { fChain->SetBranchAddress("tau_seedTrk_hadLeakEt", &tau_seedTrk_hadLeakEt, &b_tau_seedTrk_hadLeakEt); }
  if( fChain->GetBranchStatus("tau_seedTrk_sumEMCellEtOverLeadTrkPt") ) { fChain->SetBranchAddress("tau_seedTrk_sumEMCellEtOverLeadTrkPt", &tau_seedTrk_sumEMCellEtOverLeadTrkPt, &b_tau_seedTrk_sumEMCellEtOverLeadTrkPt); }
  if( fChain->GetBranchStatus("tau_seedTrk_secMaxStripEt") ) { fChain->SetBranchAddress("tau_seedTrk_secMaxStripEt", &tau_seedTrk_secMaxStripEt, &b_tau_seedTrk_secMaxStripEt); }
  if( fChain->GetBranchStatus("tau_seedTrk_stripWidth2") ) { fChain->SetBranchAddress("tau_seedTrk_stripWidth2", &tau_seedTrk_stripWidth2, &b_tau_seedTrk_stripWidth2); }
  if( fChain->GetBranchStatus("tau_seedTrk_nStrip") ) { fChain->SetBranchAddress("tau_seedTrk_nStrip", &tau_seedTrk_nStrip, &b_tau_seedTrk_nStrip); }
  if( fChain->GetBranchStatus("tau_seedTrk_etChrgHad") ) { fChain->SetBranchAddress("tau_seedTrk_etChrgHad", &tau_seedTrk_etChrgHad, &b_tau_seedTrk_etChrgHad); }
  if( fChain->GetBranchStatus("tau_seedTrk_nOtherCoreTrk") ) { fChain->SetBranchAddress("tau_seedTrk_nOtherCoreTrk", &tau_seedTrk_nOtherCoreTrk, &b_tau_seedTrk_nOtherCoreTrk); }
  if( fChain->GetBranchStatus("tau_seedTrk_nIsolTrk") ) { fChain->SetBranchAddress("tau_seedTrk_nIsolTrk", &tau_seedTrk_nIsolTrk, &b_tau_seedTrk_nIsolTrk); }
  if( fChain->GetBranchStatus("tau_seedTrk_etIsolEM") ) { fChain->SetBranchAddress("tau_seedTrk_etIsolEM", &tau_seedTrk_etIsolEM, &b_tau_seedTrk_etIsolEM); }
  if( fChain->GetBranchStatus("tau_seedTrk_etIsolHad") ) { fChain->SetBranchAddress("tau_seedTrk_etIsolHad", &tau_seedTrk_etIsolHad, &b_tau_seedTrk_etIsolHad); }
  if( fChain->GetBranchStatus("tau_calcVars_sumTrkPt") ) { fChain->SetBranchAddress("tau_calcVars_sumTrkPt", &tau_calcVars_sumTrkPt, &b_tau_calcVars_sumTrkPt); }
  if( fChain->GetBranchStatus("tau_calcVars_etHadSumPtTracks") ) { fChain->SetBranchAddress("tau_calcVars_etHadSumPtTracks", &tau_calcVars_etHadSumPtTracks, &b_tau_calcVars_etHadSumPtTracks); }
  if( fChain->GetBranchStatus("tau_calcVars_etEMSumPtTracks") ) { fChain->SetBranchAddress("tau_calcVars_etEMSumPtTracks", &tau_calcVars_etEMSumPtTracks, &b_tau_calcVars_etEMSumPtTracks); }
  if( fChain->GetBranchStatus("tau_calcVars_etHad_EMScale_Pt3Trks") ) { fChain->SetBranchAddress("tau_calcVars_etHad_EMScale_Pt3Trks", &tau_calcVars_etHad_EMScale_Pt3Trks, &b_tau_calcVars_etHad_EMScale_Pt3Trks); }
  if( fChain->GetBranchStatus("tau_calcVars_etEM_EMScale_Pt3Trks") ) { fChain->SetBranchAddress("tau_calcVars_etEM_EMScale_Pt3Trks", &tau_calcVars_etEM_EMScale_Pt3Trks, &b_tau_calcVars_etEM_EMScale_Pt3Trks); }
  if( fChain->GetBranchStatus("tau_calcVars_mass") ) { fChain->SetBranchAddress("tau_calcVars_mass", &tau_calcVars_mass, &b_tau_calcVars_mass); }
  if( fChain->GetBranchStatus("tau_calcVars_ipSigLeadLooseTrk") ) { fChain->SetBranchAddress("tau_calcVars_ipSigLeadLooseTrk", &tau_calcVars_ipSigLeadLooseTrk, &b_tau_calcVars_ipSigLeadLooseTrk); }
  if( fChain->GetBranchStatus("tau_calcVars_drMax") ) { fChain->SetBranchAddress("tau_calcVars_drMax", &tau_calcVars_drMax, &b_tau_calcVars_drMax); }
  if( fChain->GetBranchStatus("tau_calcVars_drMin") ) { fChain->SetBranchAddress("tau_calcVars_drMin", &tau_calcVars_drMin, &b_tau_calcVars_drMin); }
  if( fChain->GetBranchStatus("tau_calcVars_emFracCalib") ) { fChain->SetBranchAddress("tau_calcVars_emFracCalib", &tau_calcVars_emFracCalib, &b_tau_calcVars_emFracCalib); }
  if( fChain->GetBranchStatus("tau_calcVars_TRTHTOverLT_LeadTrk") ) { fChain->SetBranchAddress("tau_calcVars_TRTHTOverLT_LeadTrk", &tau_calcVars_TRTHTOverLT_LeadTrk, &b_tau_calcVars_TRTHTOverLT_LeadTrk); }
  if( fChain->GetBranchStatus("tau_calcVars_calRadius") ) { fChain->SetBranchAddress("tau_calcVars_calRadius", &tau_calcVars_calRadius, &b_tau_calcVars_calRadius); }
  if( fChain->GetBranchStatus("tau_calcVars_EMFractionAtEMScale") ) { fChain->SetBranchAddress("tau_calcVars_EMFractionAtEMScale", &tau_calcVars_EMFractionAtEMScale, &b_tau_calcVars_EMFractionAtEMScale); }
  if( fChain->GetBranchStatus("tau_calcVars_BDTLooseBkg") ) { fChain->SetBranchAddress("tau_calcVars_BDTLooseBkg", &tau_calcVars_BDTLooseBkg, &b_tau_calcVars_BDTLooseBkg); }
  if( fChain->GetBranchStatus("tau_calcVars_BDTMediumBkg") ) { fChain->SetBranchAddress("tau_calcVars_BDTMediumBkg", &tau_calcVars_BDTMediumBkg, &b_tau_calcVars_BDTMediumBkg); }
  if( fChain->GetBranchStatus("tau_calcVars_BDTTightBkg") ) { fChain->SetBranchAddress("tau_calcVars_BDTTightBkg", &tau_calcVars_BDTTightBkg, &b_tau_calcVars_BDTTightBkg); }
  if( fChain->GetBranchStatus("tau_seedCalo_track_n") ) { fChain->SetBranchAddress("tau_seedCalo_track_n", &tau_seedCalo_track_n, &b_tau_seedCalo_track_n); }
  if( fChain->GetBranchStatus("tau_seedCalo_wideTrk_n") ) { fChain->SetBranchAddress("tau_seedCalo_wideTrk_n", &tau_seedCalo_wideTrk_n, &b_tau_seedCalo_wideTrk_n); }
  if( fChain->GetBranchStatus("tau_otherTrk_n") ) { fChain->SetBranchAddress("tau_otherTrk_n", &tau_otherTrk_n, &b_tau_otherTrk_n); }
  if( fChain->GetBranchStatus("tau_EF_dr") ) { fChain->SetBranchAddress("tau_EF_dr", &tau_EF_dr, &b_tau_EF_dr); }
  if( fChain->GetBranchStatus("tau_EF_E") ) { fChain->SetBranchAddress("tau_EF_E", &tau_EF_E, &b_tau_EF_E); }
  if( fChain->GetBranchStatus("tau_EF_Et") ) { fChain->SetBranchAddress("tau_EF_Et", &tau_EF_Et, &b_tau_EF_Et); }
  if( fChain->GetBranchStatus("tau_EF_pt") ) { fChain->SetBranchAddress("tau_EF_pt", &tau_EF_pt, &b_tau_EF_pt); }
  if( fChain->GetBranchStatus("tau_EF_eta") ) { fChain->SetBranchAddress("tau_EF_eta", &tau_EF_eta, &b_tau_EF_eta); }
  if( fChain->GetBranchStatus("tau_EF_phi") ) { fChain->SetBranchAddress("tau_EF_phi", &tau_EF_phi, &b_tau_EF_phi); }
  if( fChain->GetBranchStatus("tau_EF_matched") ) { fChain->SetBranchAddress("tau_EF_matched", &tau_EF_matched, &b_tau_EF_matched); }
  if( fChain->GetBranchStatus("tau_L2_dr") ) { fChain->SetBranchAddress("tau_L2_dr", &tau_L2_dr, &b_tau_L2_dr); }
  if( fChain->GetBranchStatus("tau_L2_E") ) { fChain->SetBranchAddress("tau_L2_E", &tau_L2_E, &b_tau_L2_E); }
  if( fChain->GetBranchStatus("tau_L2_Et") ) { fChain->SetBranchAddress("tau_L2_Et", &tau_L2_Et, &b_tau_L2_Et); }
  if( fChain->GetBranchStatus("tau_L2_pt") ) { fChain->SetBranchAddress("tau_L2_pt", &tau_L2_pt, &b_tau_L2_pt); }
  if( fChain->GetBranchStatus("tau_L2_eta") ) { fChain->SetBranchAddress("tau_L2_eta", &tau_L2_eta, &b_tau_L2_eta); }
  if( fChain->GetBranchStatus("tau_L2_phi") ) { fChain->SetBranchAddress("tau_L2_phi", &tau_L2_phi, &b_tau_L2_phi); }
  if( fChain->GetBranchStatus("tau_L2_matched") ) { fChain->SetBranchAddress("tau_L2_matched", &tau_L2_matched, &b_tau_L2_matched); }
  if( fChain->GetBranchStatus("tau_L1_dr") ) { fChain->SetBranchAddress("tau_L1_dr", &tau_L1_dr, &b_tau_L1_dr); }
  if( fChain->GetBranchStatus("tau_L1_Et") ) { fChain->SetBranchAddress("tau_L1_Et", &tau_L1_Et, &b_tau_L1_Et); }
  if( fChain->GetBranchStatus("tau_L1_pt") ) { fChain->SetBranchAddress("tau_L1_pt", &tau_L1_pt, &b_tau_L1_pt); }
  if( fChain->GetBranchStatus("tau_L1_eta") ) { fChain->SetBranchAddress("tau_L1_eta", &tau_L1_eta, &b_tau_L1_eta); }
  if( fChain->GetBranchStatus("tau_L1_phi") ) { fChain->SetBranchAddress("tau_L1_phi", &tau_L1_phi, &b_tau_L1_phi); }
  if( fChain->GetBranchStatus("tau_L1_matched") ) { fChain->SetBranchAddress("tau_L1_matched", &tau_L1_matched, &b_tau_L1_matched); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_n") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_n", &jet_AntiKt4TopoEM_n, &b_jet_AntiKt4TopoEM_n); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_E") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_E", &jet_AntiKt4TopoEM_E, &b_jet_AntiKt4TopoEM_E); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_pt") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_pt", &jet_AntiKt4TopoEM_pt, &b_jet_AntiKt4TopoEM_pt); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_m") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_m", &jet_AntiKt4TopoEM_m, &b_jet_AntiKt4TopoEM_m); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_eta") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_eta", &jet_AntiKt4TopoEM_eta, &b_jet_AntiKt4TopoEM_eta); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_phi") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_phi", &jet_AntiKt4TopoEM_phi, &b_jet_AntiKt4TopoEM_phi); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_EtaOrigin") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_EtaOrigin", &jet_AntiKt4TopoEM_EtaOrigin, &b_jet_AntiKt4TopoEM_EtaOrigin); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_PhiOrigin") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_PhiOrigin", &jet_AntiKt4TopoEM_PhiOrigin, &b_jet_AntiKt4TopoEM_PhiOrigin); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_MOrigin") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_MOrigin", &jet_AntiKt4TopoEM_MOrigin, &b_jet_AntiKt4TopoEM_MOrigin); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_EtaOriginEM") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_EtaOriginEM", &jet_AntiKt4TopoEM_EtaOriginEM, &b_jet_AntiKt4TopoEM_EtaOriginEM); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_PhiOriginEM") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_PhiOriginEM", &jet_AntiKt4TopoEM_PhiOriginEM, &b_jet_AntiKt4TopoEM_PhiOriginEM); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_MOriginEM") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_MOriginEM", &jet_AntiKt4TopoEM_MOriginEM, &b_jet_AntiKt4TopoEM_MOriginEM); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_WIDTH") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_WIDTH", &jet_AntiKt4TopoEM_WIDTH, &b_jet_AntiKt4TopoEM_WIDTH); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_n90") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_n90", &jet_AntiKt4TopoEM_n90, &b_jet_AntiKt4TopoEM_n90); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_Timing") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_Timing", &jet_AntiKt4TopoEM_Timing, &b_jet_AntiKt4TopoEM_Timing); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_LArQuality") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_LArQuality", &jet_AntiKt4TopoEM_LArQuality, &b_jet_AntiKt4TopoEM_LArQuality); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_nTrk") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_nTrk", &jet_AntiKt4TopoEM_nTrk, &b_jet_AntiKt4TopoEM_nTrk); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_sumPtTrk") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_sumPtTrk", &jet_AntiKt4TopoEM_sumPtTrk, &b_jet_AntiKt4TopoEM_sumPtTrk); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_OriginIndex") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_OriginIndex", &jet_AntiKt4TopoEM_OriginIndex, &b_jet_AntiKt4TopoEM_OriginIndex); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_HECQuality") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_HECQuality", &jet_AntiKt4TopoEM_HECQuality, &b_jet_AntiKt4TopoEM_HECQuality); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_NegativeE") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_NegativeE", &jet_AntiKt4TopoEM_NegativeE, &b_jet_AntiKt4TopoEM_NegativeE); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_BCH_CORR_CELL") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_BCH_CORR_CELL", &jet_AntiKt4TopoEM_BCH_CORR_CELL, &b_jet_AntiKt4TopoEM_BCH_CORR_CELL); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_BCH_CORR_DOTX") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_BCH_CORR_DOTX", &jet_AntiKt4TopoEM_BCH_CORR_DOTX, &b_jet_AntiKt4TopoEM_BCH_CORR_DOTX); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_BCH_CORR_JET") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_BCH_CORR_JET", &jet_AntiKt4TopoEM_BCH_CORR_JET, &b_jet_AntiKt4TopoEM_BCH_CORR_JET); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_BCH_CORR_JET_FORCELL") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_BCH_CORR_JET_FORCELL", &jet_AntiKt4TopoEM_BCH_CORR_JET_FORCELL, &b_jet_AntiKt4TopoEM_BCH_CORR_JET_FORCELL); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_ENG_BAD_CELLS") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_ENG_BAD_CELLS", &jet_AntiKt4TopoEM_ENG_BAD_CELLS, &b_jet_AntiKt4TopoEM_ENG_BAD_CELLS); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_N_BAD_CELLS") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_N_BAD_CELLS", &jet_AntiKt4TopoEM_N_BAD_CELLS, &b_jet_AntiKt4TopoEM_N_BAD_CELLS); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_N_BAD_CELLS_CORR") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_N_BAD_CELLS_CORR", &jet_AntiKt4TopoEM_N_BAD_CELLS_CORR, &b_jet_AntiKt4TopoEM_N_BAD_CELLS_CORR); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_BAD_CELLS_CORR_E") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_BAD_CELLS_CORR_E", &jet_AntiKt4TopoEM_BAD_CELLS_CORR_E, &b_jet_AntiKt4TopoEM_BAD_CELLS_CORR_E); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_SamplingMax") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_SamplingMax", &jet_AntiKt4TopoEM_SamplingMax, &b_jet_AntiKt4TopoEM_SamplingMax); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_fracSamplingMax") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_fracSamplingMax", &jet_AntiKt4TopoEM_fracSamplingMax, &b_jet_AntiKt4TopoEM_fracSamplingMax); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_hecf") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_hecf", &jet_AntiKt4TopoEM_hecf, &b_jet_AntiKt4TopoEM_hecf); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_tgap3f") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_tgap3f", &jet_AntiKt4TopoEM_tgap3f, &b_jet_AntiKt4TopoEM_tgap3f); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_isUgly") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_isUgly", &jet_AntiKt4TopoEM_isUgly, &b_jet_AntiKt4TopoEM_isUgly); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_isBadLoose") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_isBadLoose", &jet_AntiKt4TopoEM_isBadLoose, &b_jet_AntiKt4TopoEM_isBadLoose); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_isBadTight") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_isBadTight", &jet_AntiKt4TopoEM_isBadTight, &b_jet_AntiKt4TopoEM_isBadTight); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_emfrac") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_emfrac", &jet_AntiKt4TopoEM_emfrac, &b_jet_AntiKt4TopoEM_emfrac); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_Offset") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_Offset", &jet_AntiKt4TopoEM_Offset, &b_jet_AntiKt4TopoEM_Offset); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_EMJES") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_EMJES", &jet_AntiKt4TopoEM_EMJES, &b_jet_AntiKt4TopoEM_EMJES); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_EMJES_EtaCorr") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_EMJES_EtaCorr", &jet_AntiKt4TopoEM_EMJES_EtaCorr, &b_jet_AntiKt4TopoEM_EMJES_EtaCorr); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_EMJESnooffset") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_EMJESnooffset", &jet_AntiKt4TopoEM_EMJESnooffset, &b_jet_AntiKt4TopoEM_EMJESnooffset); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_GCWJES") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_GCWJES", &jet_AntiKt4TopoEM_GCWJES, &b_jet_AntiKt4TopoEM_GCWJES); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_GCWJES_EtaCorr") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_GCWJES_EtaCorr", &jet_AntiKt4TopoEM_GCWJES_EtaCorr, &b_jet_AntiKt4TopoEM_GCWJES_EtaCorr); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_CB") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_CB", &jet_AntiKt4TopoEM_CB, &b_jet_AntiKt4TopoEM_CB); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_emscale_E") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_emscale_E", &jet_AntiKt4TopoEM_emscale_E, &b_jet_AntiKt4TopoEM_emscale_E); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_emscale_pt") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_emscale_pt", &jet_AntiKt4TopoEM_emscale_pt, &b_jet_AntiKt4TopoEM_emscale_pt); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_emscale_m") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_emscale_m", &jet_AntiKt4TopoEM_emscale_m, &b_jet_AntiKt4TopoEM_emscale_m); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_emscale_eta") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_emscale_eta", &jet_AntiKt4TopoEM_emscale_eta, &b_jet_AntiKt4TopoEM_emscale_eta); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_emscale_phi") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_emscale_phi", &jet_AntiKt4TopoEM_emscale_phi, &b_jet_AntiKt4TopoEM_emscale_phi); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_jvtxf") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_jvtxf", &jet_AntiKt4TopoEM_jvtxf, &b_jet_AntiKt4TopoEM_jvtxf); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_jvtx_x") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_jvtx_x", &jet_AntiKt4TopoEM_jvtx_x, &b_jet_AntiKt4TopoEM_jvtx_x); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_jvtx_y") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_jvtx_y", &jet_AntiKt4TopoEM_jvtx_y, &b_jet_AntiKt4TopoEM_jvtx_y); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_jvtx_z") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_jvtx_z", &jet_AntiKt4TopoEM_jvtx_z, &b_jet_AntiKt4TopoEM_jvtx_z); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_GSCFactorF") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_GSCFactorF", &jet_AntiKt4TopoEM_GSCFactorF, &b_jet_AntiKt4TopoEM_GSCFactorF); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_WidthFraction") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_WidthFraction", &jet_AntiKt4TopoEM_WidthFraction, &b_jet_AntiKt4TopoEM_WidthFraction); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_flavor_component_ip2d_pu") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_flavor_component_ip2d_pu", &jet_AntiKt4TopoEM_flavor_component_ip2d_pu, &b_jet_AntiKt4TopoEM_flavor_component_ip2d_pu); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_flavor_component_ip2d_pb") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_flavor_component_ip2d_pb", &jet_AntiKt4TopoEM_flavor_component_ip2d_pb, &b_jet_AntiKt4TopoEM_flavor_component_ip2d_pb); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_flavor_component_ip2d_ntrk") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_flavor_component_ip2d_ntrk", &jet_AntiKt4TopoEM_flavor_component_ip2d_ntrk, &b_jet_AntiKt4TopoEM_flavor_component_ip2d_ntrk); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_flavor_component_ip3d_pu") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_flavor_component_ip3d_pu", &jet_AntiKt4TopoEM_flavor_component_ip3d_pu, &b_jet_AntiKt4TopoEM_flavor_component_ip3d_pu); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_flavor_component_ip3d_pb") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_flavor_component_ip3d_pb", &jet_AntiKt4TopoEM_flavor_component_ip3d_pb, &b_jet_AntiKt4TopoEM_flavor_component_ip3d_pb); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_flavor_component_ip3d_ntrk") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_flavor_component_ip3d_ntrk", &jet_AntiKt4TopoEM_flavor_component_ip3d_ntrk, &b_jet_AntiKt4TopoEM_flavor_component_ip3d_ntrk); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_flavor_component_sv1_pu") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_flavor_component_sv1_pu", &jet_AntiKt4TopoEM_flavor_component_sv1_pu, &b_jet_AntiKt4TopoEM_flavor_component_sv1_pu); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_flavor_component_sv1_pb") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_flavor_component_sv1_pb", &jet_AntiKt4TopoEM_flavor_component_sv1_pb, &b_jet_AntiKt4TopoEM_flavor_component_sv1_pb); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_flavor_component_sv2_pu") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_flavor_component_sv2_pu", &jet_AntiKt4TopoEM_flavor_component_sv2_pu, &b_jet_AntiKt4TopoEM_flavor_component_sv2_pu); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_flavor_component_sv2_pb") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_flavor_component_sv2_pb", &jet_AntiKt4TopoEM_flavor_component_sv2_pb, &b_jet_AntiKt4TopoEM_flavor_component_sv2_pb); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_flavor_component_svp_mass") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_flavor_component_svp_mass", &jet_AntiKt4TopoEM_flavor_component_svp_mass, &b_jet_AntiKt4TopoEM_flavor_component_svp_mass); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_flavor_component_svp_n2t") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_flavor_component_svp_n2t", &jet_AntiKt4TopoEM_flavor_component_svp_n2t, &b_jet_AntiKt4TopoEM_flavor_component_svp_n2t); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_flavor_component_svp_svok") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_flavor_component_svp_svok", &jet_AntiKt4TopoEM_flavor_component_svp_svok, &b_jet_AntiKt4TopoEM_flavor_component_svp_svok); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_flavor_component_svp_ntrk") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_flavor_component_svp_ntrk", &jet_AntiKt4TopoEM_flavor_component_svp_ntrk, &b_jet_AntiKt4TopoEM_flavor_component_svp_ntrk); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_flavor_component_svp_ntrkv") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_flavor_component_svp_ntrkv", &jet_AntiKt4TopoEM_flavor_component_svp_ntrkv, &b_jet_AntiKt4TopoEM_flavor_component_svp_ntrkv); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_flavor_component_svp_ntrkj") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_flavor_component_svp_ntrkj", &jet_AntiKt4TopoEM_flavor_component_svp_ntrkj, &b_jet_AntiKt4TopoEM_flavor_component_svp_ntrkj); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_flavor_component_svp_efrc") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_flavor_component_svp_efrc", &jet_AntiKt4TopoEM_flavor_component_svp_efrc, &b_jet_AntiKt4TopoEM_flavor_component_svp_efrc); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_flavor_component_sv0p_mass") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_flavor_component_sv0p_mass", &jet_AntiKt4TopoEM_flavor_component_sv0p_mass, &b_jet_AntiKt4TopoEM_flavor_component_sv0p_mass); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_flavor_component_sv0p_n2t") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_flavor_component_sv0p_n2t", &jet_AntiKt4TopoEM_flavor_component_sv0p_n2t, &b_jet_AntiKt4TopoEM_flavor_component_sv0p_n2t); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_flavor_component_sv0p_svok") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_flavor_component_sv0p_svok", &jet_AntiKt4TopoEM_flavor_component_sv0p_svok, &b_jet_AntiKt4TopoEM_flavor_component_sv0p_svok); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_flavor_component_sv0p_ntrk") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_flavor_component_sv0p_ntrk", &jet_AntiKt4TopoEM_flavor_component_sv0p_ntrk, &b_jet_AntiKt4TopoEM_flavor_component_sv0p_ntrk); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_flavor_component_sv0p_ntrkv") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_flavor_component_sv0p_ntrkv", &jet_AntiKt4TopoEM_flavor_component_sv0p_ntrkv, &b_jet_AntiKt4TopoEM_flavor_component_sv0p_ntrkv); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_flavor_component_sv0p_ntrkj") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_flavor_component_sv0p_ntrkj", &jet_AntiKt4TopoEM_flavor_component_sv0p_ntrkj, &b_jet_AntiKt4TopoEM_flavor_component_sv0p_ntrkj); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_flavor_component_sv0p_efrc") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_flavor_component_sv0p_efrc", &jet_AntiKt4TopoEM_flavor_component_sv0p_efrc, &b_jet_AntiKt4TopoEM_flavor_component_sv0p_efrc); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_flavor_component_jfit_pu") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_flavor_component_jfit_pu", &jet_AntiKt4TopoEM_flavor_component_jfit_pu, &b_jet_AntiKt4TopoEM_flavor_component_jfit_pu); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_flavor_component_jfit_pb") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_flavor_component_jfit_pb", &jet_AntiKt4TopoEM_flavor_component_jfit_pb, &b_jet_AntiKt4TopoEM_flavor_component_jfit_pb); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_flavor_component_jfit_nvtxnt") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_flavor_component_jfit_nvtxnt", &jet_AntiKt4TopoEM_flavor_component_jfit_nvtxnt, &b_jet_AntiKt4TopoEM_flavor_component_jfit_nvtxnt); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_flavor_component_jfit_nvtx1t") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_flavor_component_jfit_nvtx1t", &jet_AntiKt4TopoEM_flavor_component_jfit_nvtx1t, &b_jet_AntiKt4TopoEM_flavor_component_jfit_nvtx1t); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_flavor_component_jfit_ntrk") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_flavor_component_jfit_ntrk", &jet_AntiKt4TopoEM_flavor_component_jfit_ntrk, &b_jet_AntiKt4TopoEM_flavor_component_jfit_ntrk); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_flavor_component_jfit_efrc") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_flavor_component_jfit_efrc", &jet_AntiKt4TopoEM_flavor_component_jfit_efrc, &b_jet_AntiKt4TopoEM_flavor_component_jfit_efrc); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_flavor_component_jfit_mass") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_flavor_component_jfit_mass", &jet_AntiKt4TopoEM_flavor_component_jfit_mass, &b_jet_AntiKt4TopoEM_flavor_component_jfit_mass); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_flavor_component_jfit_sig3d") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_flavor_component_jfit_sig3d", &jet_AntiKt4TopoEM_flavor_component_jfit_sig3d, &b_jet_AntiKt4TopoEM_flavor_component_jfit_sig3d); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_flavor_weight_TrackCounting2D") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_flavor_weight_TrackCounting2D", &jet_AntiKt4TopoEM_flavor_weight_TrackCounting2D, &b_jet_AntiKt4TopoEM_flavor_weight_TrackCounting2D); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_flavor_weight_JetProb") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_flavor_weight_JetProb", &jet_AntiKt4TopoEM_flavor_weight_JetProb, &b_jet_AntiKt4TopoEM_flavor_weight_JetProb); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_flavor_weight_IP1D") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_flavor_weight_IP1D", &jet_AntiKt4TopoEM_flavor_weight_IP1D, &b_jet_AntiKt4TopoEM_flavor_weight_IP1D); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_flavor_weight_IP2D") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_flavor_weight_IP2D", &jet_AntiKt4TopoEM_flavor_weight_IP2D, &b_jet_AntiKt4TopoEM_flavor_weight_IP2D); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_flavor_weight_IP3D") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_flavor_weight_IP3D", &jet_AntiKt4TopoEM_flavor_weight_IP3D, &b_jet_AntiKt4TopoEM_flavor_weight_IP3D); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_flavor_weight_SV0") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_flavor_weight_SV0", &jet_AntiKt4TopoEM_flavor_weight_SV0, &b_jet_AntiKt4TopoEM_flavor_weight_SV0); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_flavor_weight_SV1") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_flavor_weight_SV1", &jet_AntiKt4TopoEM_flavor_weight_SV1, &b_jet_AntiKt4TopoEM_flavor_weight_SV1); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_flavor_weight_SV2") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_flavor_weight_SV2", &jet_AntiKt4TopoEM_flavor_weight_SV2, &b_jet_AntiKt4TopoEM_flavor_weight_SV2); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_flavor_weight_JetFitterTag") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_flavor_weight_JetFitterTag", &jet_AntiKt4TopoEM_flavor_weight_JetFitterTag, &b_jet_AntiKt4TopoEM_flavor_weight_JetFitterTag); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_flavor_weight_JetFitterCOMB") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_flavor_weight_JetFitterCOMB", &jet_AntiKt4TopoEM_flavor_weight_JetFitterCOMB, &b_jet_AntiKt4TopoEM_flavor_weight_JetFitterCOMB); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_flavor_weight_JetFitterTagNN") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_flavor_weight_JetFitterTagNN", &jet_AntiKt4TopoEM_flavor_weight_JetFitterTagNN, &b_jet_AntiKt4TopoEM_flavor_weight_JetFitterTagNN); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_flavor_weight_JetFitterCOMBNN") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_flavor_weight_JetFitterCOMBNN", &jet_AntiKt4TopoEM_flavor_weight_JetFitterCOMBNN, &b_jet_AntiKt4TopoEM_flavor_weight_JetFitterCOMBNN); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_flavor_weight_SoftMuonTag") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_flavor_weight_SoftMuonTag", &jet_AntiKt4TopoEM_flavor_weight_SoftMuonTag, &b_jet_AntiKt4TopoEM_flavor_weight_SoftMuonTag); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_flavor_weight_SoftElectronTag") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_flavor_weight_SoftElectronTag", &jet_AntiKt4TopoEM_flavor_weight_SoftElectronTag, &b_jet_AntiKt4TopoEM_flavor_weight_SoftElectronTag); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_flavor_truth_dRminToB") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_flavor_truth_dRminToB", &jet_AntiKt4TopoEM_flavor_truth_dRminToB, &b_jet_AntiKt4TopoEM_flavor_truth_dRminToB); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_flavor_truth_dRminToC") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_flavor_truth_dRminToC", &jet_AntiKt4TopoEM_flavor_truth_dRminToC, &b_jet_AntiKt4TopoEM_flavor_truth_dRminToC); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_flavor_truth_dRminToT") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_flavor_truth_dRminToT", &jet_AntiKt4TopoEM_flavor_truth_dRminToT, &b_jet_AntiKt4TopoEM_flavor_truth_dRminToT); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_flavor_truth_BHadronpdg") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_flavor_truth_BHadronpdg", &jet_AntiKt4TopoEM_flavor_truth_BHadronpdg, &b_jet_AntiKt4TopoEM_flavor_truth_BHadronpdg); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_flavor_truth_trueflav") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_flavor_truth_trueflav", &jet_AntiKt4TopoEM_flavor_truth_trueflav, &b_jet_AntiKt4TopoEM_flavor_truth_trueflav); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_el_dr") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_el_dr", &jet_AntiKt4TopoEM_el_dr, &b_jet_AntiKt4TopoEM_el_dr); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_el_matched") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_el_matched", &jet_AntiKt4TopoEM_el_matched, &b_jet_AntiKt4TopoEM_el_matched); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_mu_dr") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_mu_dr", &jet_AntiKt4TopoEM_mu_dr, &b_jet_AntiKt4TopoEM_mu_dr); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_mu_matched") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_mu_matched", &jet_AntiKt4TopoEM_mu_matched, &b_jet_AntiKt4TopoEM_mu_matched); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_L1_dr") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_L1_dr", &jet_AntiKt4TopoEM_L1_dr, &b_jet_AntiKt4TopoEM_L1_dr); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_L1_matched") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_L1_matched", &jet_AntiKt4TopoEM_L1_matched, &b_jet_AntiKt4TopoEM_L1_matched); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_L2_dr") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_L2_dr", &jet_AntiKt4TopoEM_L2_dr, &b_jet_AntiKt4TopoEM_L2_dr); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_L2_matched") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_L2_matched", &jet_AntiKt4TopoEM_L2_matched, &b_jet_AntiKt4TopoEM_L2_matched); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_EF_dr") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_EF_dr", &jet_AntiKt4TopoEM_EF_dr, &b_jet_AntiKt4TopoEM_EF_dr); }
  if( fChain->GetBranchStatus("jet_AntiKt4TopoEM_EF_matched") ) { fChain->SetBranchAddress("jet_AntiKt4TopoEM_EF_matched", &jet_AntiKt4TopoEM_EF_matched, &b_jet_AntiKt4TopoEM_EF_matched); }
  if( fChain->GetBranchStatus("jet_akt6topoem_n") ) { fChain->SetBranchAddress("jet_akt6topoem_n", &jet_akt6topoem_n, &b_jet_akt6topoem_n); }
  if( fChain->GetBranchStatus("jet_akt6topoem_E") ) { fChain->SetBranchAddress("jet_akt6topoem_E", &jet_akt6topoem_E, &b_jet_akt6topoem_E); }
  if( fChain->GetBranchStatus("jet_akt6topoem_pt") ) { fChain->SetBranchAddress("jet_akt6topoem_pt", &jet_akt6topoem_pt, &b_jet_akt6topoem_pt); }
  if( fChain->GetBranchStatus("jet_akt6topoem_m") ) { fChain->SetBranchAddress("jet_akt6topoem_m", &jet_akt6topoem_m, &b_jet_akt6topoem_m); }
  if( fChain->GetBranchStatus("jet_akt6topoem_eta") ) { fChain->SetBranchAddress("jet_akt6topoem_eta", &jet_akt6topoem_eta, &b_jet_akt6topoem_eta); }
  if( fChain->GetBranchStatus("jet_akt6topoem_phi") ) { fChain->SetBranchAddress("jet_akt6topoem_phi", &jet_akt6topoem_phi, &b_jet_akt6topoem_phi); }
  if( fChain->GetBranchStatus("jet_akt6topoem_EtaOrigin") ) { fChain->SetBranchAddress("jet_akt6topoem_EtaOrigin", &jet_akt6topoem_EtaOrigin, &b_jet_akt6topoem_EtaOrigin); }
  if( fChain->GetBranchStatus("jet_akt6topoem_PhiOrigin") ) { fChain->SetBranchAddress("jet_akt6topoem_PhiOrigin", &jet_akt6topoem_PhiOrigin, &b_jet_akt6topoem_PhiOrigin); }
  if( fChain->GetBranchStatus("jet_akt6topoem_MOrigin") ) { fChain->SetBranchAddress("jet_akt6topoem_MOrigin", &jet_akt6topoem_MOrigin, &b_jet_akt6topoem_MOrigin); }
  if( fChain->GetBranchStatus("jet_akt6topoem_EtaOriginEM") ) { fChain->SetBranchAddress("jet_akt6topoem_EtaOriginEM", &jet_akt6topoem_EtaOriginEM, &b_jet_akt6topoem_EtaOriginEM); }
  if( fChain->GetBranchStatus("jet_akt6topoem_PhiOriginEM") ) { fChain->SetBranchAddress("jet_akt6topoem_PhiOriginEM", &jet_akt6topoem_PhiOriginEM, &b_jet_akt6topoem_PhiOriginEM); }
  if( fChain->GetBranchStatus("jet_akt6topoem_MOriginEM") ) { fChain->SetBranchAddress("jet_akt6topoem_MOriginEM", &jet_akt6topoem_MOriginEM, &b_jet_akt6topoem_MOriginEM); }
  if( fChain->GetBranchStatus("jet_akt6topoem_WIDTH") ) { fChain->SetBranchAddress("jet_akt6topoem_WIDTH", &jet_akt6topoem_WIDTH, &b_jet_akt6topoem_WIDTH); }
  if( fChain->GetBranchStatus("jet_akt6topoem_n90") ) { fChain->SetBranchAddress("jet_akt6topoem_n90", &jet_akt6topoem_n90, &b_jet_akt6topoem_n90); }
  if( fChain->GetBranchStatus("jet_akt6topoem_Timing") ) { fChain->SetBranchAddress("jet_akt6topoem_Timing", &jet_akt6topoem_Timing, &b_jet_akt6topoem_Timing); }
  if( fChain->GetBranchStatus("jet_akt6topoem_LArQuality") ) { fChain->SetBranchAddress("jet_akt6topoem_LArQuality", &jet_akt6topoem_LArQuality, &b_jet_akt6topoem_LArQuality); }
  if( fChain->GetBranchStatus("jet_akt6topoem_nTrk") ) { fChain->SetBranchAddress("jet_akt6topoem_nTrk", &jet_akt6topoem_nTrk, &b_jet_akt6topoem_nTrk); }
  if( fChain->GetBranchStatus("jet_akt6topoem_sumPtTrk") ) { fChain->SetBranchAddress("jet_akt6topoem_sumPtTrk", &jet_akt6topoem_sumPtTrk, &b_jet_akt6topoem_sumPtTrk); }
  if( fChain->GetBranchStatus("jet_akt6topoem_OriginIndex") ) { fChain->SetBranchAddress("jet_akt6topoem_OriginIndex", &jet_akt6topoem_OriginIndex, &b_jet_akt6topoem_OriginIndex); }
  if( fChain->GetBranchStatus("jet_akt6topoem_HECQuality") ) { fChain->SetBranchAddress("jet_akt6topoem_HECQuality", &jet_akt6topoem_HECQuality, &b_jet_akt6topoem_HECQuality); }
  if( fChain->GetBranchStatus("jet_akt6topoem_NegativeE") ) { fChain->SetBranchAddress("jet_akt6topoem_NegativeE", &jet_akt6topoem_NegativeE, &b_jet_akt6topoem_NegativeE); }
  if( fChain->GetBranchStatus("jet_akt6topoem_BCH_CORR_CELL") ) { fChain->SetBranchAddress("jet_akt6topoem_BCH_CORR_CELL", &jet_akt6topoem_BCH_CORR_CELL, &b_jet_akt6topoem_BCH_CORR_CELL); }
  if( fChain->GetBranchStatus("jet_akt6topoem_BCH_CORR_DOTX") ) { fChain->SetBranchAddress("jet_akt6topoem_BCH_CORR_DOTX", &jet_akt6topoem_BCH_CORR_DOTX, &b_jet_akt6topoem_BCH_CORR_DOTX); }
  if( fChain->GetBranchStatus("jet_akt6topoem_BCH_CORR_JET") ) { fChain->SetBranchAddress("jet_akt6topoem_BCH_CORR_JET", &jet_akt6topoem_BCH_CORR_JET, &b_jet_akt6topoem_BCH_CORR_JET); }
  if( fChain->GetBranchStatus("jet_akt6topoem_BCH_CORR_JET_FORCELL") ) { fChain->SetBranchAddress("jet_akt6topoem_BCH_CORR_JET_FORCELL", &jet_akt6topoem_BCH_CORR_JET_FORCELL, &b_jet_akt6topoem_BCH_CORR_JET_FORCELL); }
  if( fChain->GetBranchStatus("jet_akt6topoem_ENG_BAD_CELLS") ) { fChain->SetBranchAddress("jet_akt6topoem_ENG_BAD_CELLS", &jet_akt6topoem_ENG_BAD_CELLS, &b_jet_akt6topoem_ENG_BAD_CELLS); }
  if( fChain->GetBranchStatus("jet_akt6topoem_N_BAD_CELLS") ) { fChain->SetBranchAddress("jet_akt6topoem_N_BAD_CELLS", &jet_akt6topoem_N_BAD_CELLS, &b_jet_akt6topoem_N_BAD_CELLS); }
  if( fChain->GetBranchStatus("jet_akt6topoem_N_BAD_CELLS_CORR") ) { fChain->SetBranchAddress("jet_akt6topoem_N_BAD_CELLS_CORR", &jet_akt6topoem_N_BAD_CELLS_CORR, &b_jet_akt6topoem_N_BAD_CELLS_CORR); }
  if( fChain->GetBranchStatus("jet_akt6topoem_BAD_CELLS_CORR_E") ) { fChain->SetBranchAddress("jet_akt6topoem_BAD_CELLS_CORR_E", &jet_akt6topoem_BAD_CELLS_CORR_E, &b_jet_akt6topoem_BAD_CELLS_CORR_E); }
  if( fChain->GetBranchStatus("jet_akt6topoem_SamplingMax") ) { fChain->SetBranchAddress("jet_akt6topoem_SamplingMax", &jet_akt6topoem_SamplingMax, &b_jet_akt6topoem_SamplingMax); }
  if( fChain->GetBranchStatus("jet_akt6topoem_fracSamplingMax") ) { fChain->SetBranchAddress("jet_akt6topoem_fracSamplingMax", &jet_akt6topoem_fracSamplingMax, &b_jet_akt6topoem_fracSamplingMax); }
  if( fChain->GetBranchStatus("jet_akt6topoem_hecf") ) { fChain->SetBranchAddress("jet_akt6topoem_hecf", &jet_akt6topoem_hecf, &b_jet_akt6topoem_hecf); }
  if( fChain->GetBranchStatus("jet_akt6topoem_tgap3f") ) { fChain->SetBranchAddress("jet_akt6topoem_tgap3f", &jet_akt6topoem_tgap3f, &b_jet_akt6topoem_tgap3f); }
  if( fChain->GetBranchStatus("jet_akt6topoem_isUgly") ) { fChain->SetBranchAddress("jet_akt6topoem_isUgly", &jet_akt6topoem_isUgly, &b_jet_akt6topoem_isUgly); }
  if( fChain->GetBranchStatus("jet_akt6topoem_isBadLoose") ) { fChain->SetBranchAddress("jet_akt6topoem_isBadLoose", &jet_akt6topoem_isBadLoose, &b_jet_akt6topoem_isBadLoose); }
  if( fChain->GetBranchStatus("jet_akt6topoem_isBadTight") ) { fChain->SetBranchAddress("jet_akt6topoem_isBadTight", &jet_akt6topoem_isBadTight, &b_jet_akt6topoem_isBadTight); }
  if( fChain->GetBranchStatus("jet_akt6topoem_emfrac") ) { fChain->SetBranchAddress("jet_akt6topoem_emfrac", &jet_akt6topoem_emfrac, &b_jet_akt6topoem_emfrac); }
  if( fChain->GetBranchStatus("jet_akt6topoem_Offset") ) { fChain->SetBranchAddress("jet_akt6topoem_Offset", &jet_akt6topoem_Offset, &b_jet_akt6topoem_Offset); }
  if( fChain->GetBranchStatus("jet_akt6topoem_EMJES") ) { fChain->SetBranchAddress("jet_akt6topoem_EMJES", &jet_akt6topoem_EMJES, &b_jet_akt6topoem_EMJES); }
  if( fChain->GetBranchStatus("jet_akt6topoem_EMJES_EtaCorr") ) { fChain->SetBranchAddress("jet_akt6topoem_EMJES_EtaCorr", &jet_akt6topoem_EMJES_EtaCorr, &b_jet_akt6topoem_EMJES_EtaCorr); }
  if( fChain->GetBranchStatus("jet_akt6topoem_EMJESnooffset") ) { fChain->SetBranchAddress("jet_akt6topoem_EMJESnooffset", &jet_akt6topoem_EMJESnooffset, &b_jet_akt6topoem_EMJESnooffset); }
  if( fChain->GetBranchStatus("jet_akt6topoem_GCWJES") ) { fChain->SetBranchAddress("jet_akt6topoem_GCWJES", &jet_akt6topoem_GCWJES, &b_jet_akt6topoem_GCWJES); }
  if( fChain->GetBranchStatus("jet_akt6topoem_GCWJES_EtaCorr") ) { fChain->SetBranchAddress("jet_akt6topoem_GCWJES_EtaCorr", &jet_akt6topoem_GCWJES_EtaCorr, &b_jet_akt6topoem_GCWJES_EtaCorr); }
  if( fChain->GetBranchStatus("jet_akt6topoem_CB") ) { fChain->SetBranchAddress("jet_akt6topoem_CB", &jet_akt6topoem_CB, &b_jet_akt6topoem_CB); }
  if( fChain->GetBranchStatus("jet_akt6topoem_emscale_E") ) { fChain->SetBranchAddress("jet_akt6topoem_emscale_E", &jet_akt6topoem_emscale_E, &b_jet_akt6topoem_emscale_E); }
  if( fChain->GetBranchStatus("jet_akt6topoem_emscale_pt") ) { fChain->SetBranchAddress("jet_akt6topoem_emscale_pt", &jet_akt6topoem_emscale_pt, &b_jet_akt6topoem_emscale_pt); }
  if( fChain->GetBranchStatus("jet_akt6topoem_emscale_m") ) { fChain->SetBranchAddress("jet_akt6topoem_emscale_m", &jet_akt6topoem_emscale_m, &b_jet_akt6topoem_emscale_m); }
  if( fChain->GetBranchStatus("jet_akt6topoem_emscale_eta") ) { fChain->SetBranchAddress("jet_akt6topoem_emscale_eta", &jet_akt6topoem_emscale_eta, &b_jet_akt6topoem_emscale_eta); }
  if( fChain->GetBranchStatus("jet_akt6topoem_emscale_phi") ) { fChain->SetBranchAddress("jet_akt6topoem_emscale_phi", &jet_akt6topoem_emscale_phi, &b_jet_akt6topoem_emscale_phi); }
  if( fChain->GetBranchStatus("jet_akt6topoem_jvtxf") ) { fChain->SetBranchAddress("jet_akt6topoem_jvtxf", &jet_akt6topoem_jvtxf, &b_jet_akt6topoem_jvtxf); }
  if( fChain->GetBranchStatus("jet_akt6topoem_jvtx_x") ) { fChain->SetBranchAddress("jet_akt6topoem_jvtx_x", &jet_akt6topoem_jvtx_x, &b_jet_akt6topoem_jvtx_x); }
  if( fChain->GetBranchStatus("jet_akt6topoem_jvtx_y") ) { fChain->SetBranchAddress("jet_akt6topoem_jvtx_y", &jet_akt6topoem_jvtx_y, &b_jet_akt6topoem_jvtx_y); }
  if( fChain->GetBranchStatus("jet_akt6topoem_jvtx_z") ) { fChain->SetBranchAddress("jet_akt6topoem_jvtx_z", &jet_akt6topoem_jvtx_z, &b_jet_akt6topoem_jvtx_z); }
  if( fChain->GetBranchStatus("jet_akt6topoem_GSCFactorF") ) { fChain->SetBranchAddress("jet_akt6topoem_GSCFactorF", &jet_akt6topoem_GSCFactorF, &b_jet_akt6topoem_GSCFactorF); }
  if( fChain->GetBranchStatus("jet_akt6topoem_WidthFraction") ) { fChain->SetBranchAddress("jet_akt6topoem_WidthFraction", &jet_akt6topoem_WidthFraction, &b_jet_akt6topoem_WidthFraction); }
  if( fChain->GetBranchStatus("jet_akt6topoem_flavor_component_ip2d_pu") ) { fChain->SetBranchAddress("jet_akt6topoem_flavor_component_ip2d_pu", &jet_akt6topoem_flavor_component_ip2d_pu, &b_jet_akt6topoem_flavor_component_ip2d_pu); }
  if( fChain->GetBranchStatus("jet_akt6topoem_flavor_component_ip2d_pb") ) { fChain->SetBranchAddress("jet_akt6topoem_flavor_component_ip2d_pb", &jet_akt6topoem_flavor_component_ip2d_pb, &b_jet_akt6topoem_flavor_component_ip2d_pb); }
  if( fChain->GetBranchStatus("jet_akt6topoem_flavor_component_ip2d_ntrk") ) { fChain->SetBranchAddress("jet_akt6topoem_flavor_component_ip2d_ntrk", &jet_akt6topoem_flavor_component_ip2d_ntrk, &b_jet_akt6topoem_flavor_component_ip2d_ntrk); }
  if( fChain->GetBranchStatus("jet_akt6topoem_flavor_component_ip3d_pu") ) { fChain->SetBranchAddress("jet_akt6topoem_flavor_component_ip3d_pu", &jet_akt6topoem_flavor_component_ip3d_pu, &b_jet_akt6topoem_flavor_component_ip3d_pu); }
  if( fChain->GetBranchStatus("jet_akt6topoem_flavor_component_ip3d_pb") ) { fChain->SetBranchAddress("jet_akt6topoem_flavor_component_ip3d_pb", &jet_akt6topoem_flavor_component_ip3d_pb, &b_jet_akt6topoem_flavor_component_ip3d_pb); }
  if( fChain->GetBranchStatus("jet_akt6topoem_flavor_component_ip3d_ntrk") ) { fChain->SetBranchAddress("jet_akt6topoem_flavor_component_ip3d_ntrk", &jet_akt6topoem_flavor_component_ip3d_ntrk, &b_jet_akt6topoem_flavor_component_ip3d_ntrk); }
  if( fChain->GetBranchStatus("jet_akt6topoem_flavor_component_sv1_pu") ) { fChain->SetBranchAddress("jet_akt6topoem_flavor_component_sv1_pu", &jet_akt6topoem_flavor_component_sv1_pu, &b_jet_akt6topoem_flavor_component_sv1_pu); }
  if( fChain->GetBranchStatus("jet_akt6topoem_flavor_component_sv1_pb") ) { fChain->SetBranchAddress("jet_akt6topoem_flavor_component_sv1_pb", &jet_akt6topoem_flavor_component_sv1_pb, &b_jet_akt6topoem_flavor_component_sv1_pb); }
  if( fChain->GetBranchStatus("jet_akt6topoem_flavor_component_sv2_pu") ) { fChain->SetBranchAddress("jet_akt6topoem_flavor_component_sv2_pu", &jet_akt6topoem_flavor_component_sv2_pu, &b_jet_akt6topoem_flavor_component_sv2_pu); }
  if( fChain->GetBranchStatus("jet_akt6topoem_flavor_component_sv2_pb") ) { fChain->SetBranchAddress("jet_akt6topoem_flavor_component_sv2_pb", &jet_akt6topoem_flavor_component_sv2_pb, &b_jet_akt6topoem_flavor_component_sv2_pb); }
  if( fChain->GetBranchStatus("jet_akt6topoem_flavor_component_svp_mass") ) { fChain->SetBranchAddress("jet_akt6topoem_flavor_component_svp_mass", &jet_akt6topoem_flavor_component_svp_mass, &b_jet_akt6topoem_flavor_component_svp_mass); }
  if( fChain->GetBranchStatus("jet_akt6topoem_flavor_component_svp_n2t") ) { fChain->SetBranchAddress("jet_akt6topoem_flavor_component_svp_n2t", &jet_akt6topoem_flavor_component_svp_n2t, &b_jet_akt6topoem_flavor_component_svp_n2t); }
  if( fChain->GetBranchStatus("jet_akt6topoem_flavor_component_svp_svok") ) { fChain->SetBranchAddress("jet_akt6topoem_flavor_component_svp_svok", &jet_akt6topoem_flavor_component_svp_svok, &b_jet_akt6topoem_flavor_component_svp_svok); }
  if( fChain->GetBranchStatus("jet_akt6topoem_flavor_component_svp_ntrk") ) { fChain->SetBranchAddress("jet_akt6topoem_flavor_component_svp_ntrk", &jet_akt6topoem_flavor_component_svp_ntrk, &b_jet_akt6topoem_flavor_component_svp_ntrk); }
  if( fChain->GetBranchStatus("jet_akt6topoem_flavor_component_svp_ntrkv") ) { fChain->SetBranchAddress("jet_akt6topoem_flavor_component_svp_ntrkv", &jet_akt6topoem_flavor_component_svp_ntrkv, &b_jet_akt6topoem_flavor_component_svp_ntrkv); }
  if( fChain->GetBranchStatus("jet_akt6topoem_flavor_component_svp_ntrkj") ) { fChain->SetBranchAddress("jet_akt6topoem_flavor_component_svp_ntrkj", &jet_akt6topoem_flavor_component_svp_ntrkj, &b_jet_akt6topoem_flavor_component_svp_ntrkj); }
  if( fChain->GetBranchStatus("jet_akt6topoem_flavor_component_svp_efrc") ) { fChain->SetBranchAddress("jet_akt6topoem_flavor_component_svp_efrc", &jet_akt6topoem_flavor_component_svp_efrc, &b_jet_akt6topoem_flavor_component_svp_efrc); }
  if( fChain->GetBranchStatus("jet_akt6topoem_flavor_component_sv0p_mass") ) { fChain->SetBranchAddress("jet_akt6topoem_flavor_component_sv0p_mass", &jet_akt6topoem_flavor_component_sv0p_mass, &b_jet_akt6topoem_flavor_component_sv0p_mass); }
  if( fChain->GetBranchStatus("jet_akt6topoem_flavor_component_sv0p_n2t") ) { fChain->SetBranchAddress("jet_akt6topoem_flavor_component_sv0p_n2t", &jet_akt6topoem_flavor_component_sv0p_n2t, &b_jet_akt6topoem_flavor_component_sv0p_n2t); }
  if( fChain->GetBranchStatus("jet_akt6topoem_flavor_component_sv0p_svok") ) { fChain->SetBranchAddress("jet_akt6topoem_flavor_component_sv0p_svok", &jet_akt6topoem_flavor_component_sv0p_svok, &b_jet_akt6topoem_flavor_component_sv0p_svok); }
  if( fChain->GetBranchStatus("jet_akt6topoem_flavor_component_sv0p_ntrk") ) { fChain->SetBranchAddress("jet_akt6topoem_flavor_component_sv0p_ntrk", &jet_akt6topoem_flavor_component_sv0p_ntrk, &b_jet_akt6topoem_flavor_component_sv0p_ntrk); }
  if( fChain->GetBranchStatus("jet_akt6topoem_flavor_component_sv0p_ntrkv") ) { fChain->SetBranchAddress("jet_akt6topoem_flavor_component_sv0p_ntrkv", &jet_akt6topoem_flavor_component_sv0p_ntrkv, &b_jet_akt6topoem_flavor_component_sv0p_ntrkv); }
  if( fChain->GetBranchStatus("jet_akt6topoem_flavor_component_sv0p_ntrkj") ) { fChain->SetBranchAddress("jet_akt6topoem_flavor_component_sv0p_ntrkj", &jet_akt6topoem_flavor_component_sv0p_ntrkj, &b_jet_akt6topoem_flavor_component_sv0p_ntrkj); }
  if( fChain->GetBranchStatus("jet_akt6topoem_flavor_component_sv0p_efrc") ) { fChain->SetBranchAddress("jet_akt6topoem_flavor_component_sv0p_efrc", &jet_akt6topoem_flavor_component_sv0p_efrc, &b_jet_akt6topoem_flavor_component_sv0p_efrc); }
  if( fChain->GetBranchStatus("jet_akt6topoem_flavor_component_jfit_pu") ) { fChain->SetBranchAddress("jet_akt6topoem_flavor_component_jfit_pu", &jet_akt6topoem_flavor_component_jfit_pu, &b_jet_akt6topoem_flavor_component_jfit_pu); }
  if( fChain->GetBranchStatus("jet_akt6topoem_flavor_component_jfit_pb") ) { fChain->SetBranchAddress("jet_akt6topoem_flavor_component_jfit_pb", &jet_akt6topoem_flavor_component_jfit_pb, &b_jet_akt6topoem_flavor_component_jfit_pb); }
  if( fChain->GetBranchStatus("jet_akt6topoem_flavor_component_jfit_nvtxnt") ) { fChain->SetBranchAddress("jet_akt6topoem_flavor_component_jfit_nvtxnt", &jet_akt6topoem_flavor_component_jfit_nvtxnt, &b_jet_akt6topoem_flavor_component_jfit_nvtxnt); }
  if( fChain->GetBranchStatus("jet_akt6topoem_flavor_component_jfit_nvtx1t") ) { fChain->SetBranchAddress("jet_akt6topoem_flavor_component_jfit_nvtx1t", &jet_akt6topoem_flavor_component_jfit_nvtx1t, &b_jet_akt6topoem_flavor_component_jfit_nvtx1t); }
  if( fChain->GetBranchStatus("jet_akt6topoem_flavor_component_jfit_ntrk") ) { fChain->SetBranchAddress("jet_akt6topoem_flavor_component_jfit_ntrk", &jet_akt6topoem_flavor_component_jfit_ntrk, &b_jet_akt6topoem_flavor_component_jfit_ntrk); }
  if( fChain->GetBranchStatus("jet_akt6topoem_flavor_component_jfit_efrc") ) { fChain->SetBranchAddress("jet_akt6topoem_flavor_component_jfit_efrc", &jet_akt6topoem_flavor_component_jfit_efrc, &b_jet_akt6topoem_flavor_component_jfit_efrc); }
  if( fChain->GetBranchStatus("jet_akt6topoem_flavor_component_jfit_mass") ) { fChain->SetBranchAddress("jet_akt6topoem_flavor_component_jfit_mass", &jet_akt6topoem_flavor_component_jfit_mass, &b_jet_akt6topoem_flavor_component_jfit_mass); }
  if( fChain->GetBranchStatus("jet_akt6topoem_flavor_component_jfit_sig3d") ) { fChain->SetBranchAddress("jet_akt6topoem_flavor_component_jfit_sig3d", &jet_akt6topoem_flavor_component_jfit_sig3d, &b_jet_akt6topoem_flavor_component_jfit_sig3d); }
  if( fChain->GetBranchStatus("jet_akt6topoem_flavor_weight_TrackCounting2D") ) { fChain->SetBranchAddress("jet_akt6topoem_flavor_weight_TrackCounting2D", &jet_akt6topoem_flavor_weight_TrackCounting2D, &b_jet_akt6topoem_flavor_weight_TrackCounting2D); }
  if( fChain->GetBranchStatus("jet_akt6topoem_flavor_weight_JetProb") ) { fChain->SetBranchAddress("jet_akt6topoem_flavor_weight_JetProb", &jet_akt6topoem_flavor_weight_JetProb, &b_jet_akt6topoem_flavor_weight_JetProb); }
  if( fChain->GetBranchStatus("jet_akt6topoem_flavor_weight_IP1D") ) { fChain->SetBranchAddress("jet_akt6topoem_flavor_weight_IP1D", &jet_akt6topoem_flavor_weight_IP1D, &b_jet_akt6topoem_flavor_weight_IP1D); }
  if( fChain->GetBranchStatus("jet_akt6topoem_flavor_weight_IP2D") ) { fChain->SetBranchAddress("jet_akt6topoem_flavor_weight_IP2D", &jet_akt6topoem_flavor_weight_IP2D, &b_jet_akt6topoem_flavor_weight_IP2D); }
  if( fChain->GetBranchStatus("jet_akt6topoem_flavor_weight_IP3D") ) { fChain->SetBranchAddress("jet_akt6topoem_flavor_weight_IP3D", &jet_akt6topoem_flavor_weight_IP3D, &b_jet_akt6topoem_flavor_weight_IP3D); }
  if( fChain->GetBranchStatus("jet_akt6topoem_flavor_weight_SV0") ) { fChain->SetBranchAddress("jet_akt6topoem_flavor_weight_SV0", &jet_akt6topoem_flavor_weight_SV0, &b_jet_akt6topoem_flavor_weight_SV0); }
  if( fChain->GetBranchStatus("jet_akt6topoem_flavor_weight_SV1") ) { fChain->SetBranchAddress("jet_akt6topoem_flavor_weight_SV1", &jet_akt6topoem_flavor_weight_SV1, &b_jet_akt6topoem_flavor_weight_SV1); }
  if( fChain->GetBranchStatus("jet_akt6topoem_flavor_weight_SV2") ) { fChain->SetBranchAddress("jet_akt6topoem_flavor_weight_SV2", &jet_akt6topoem_flavor_weight_SV2, &b_jet_akt6topoem_flavor_weight_SV2); }
  if( fChain->GetBranchStatus("jet_akt6topoem_flavor_weight_JetFitterTag") ) { fChain->SetBranchAddress("jet_akt6topoem_flavor_weight_JetFitterTag", &jet_akt6topoem_flavor_weight_JetFitterTag, &b_jet_akt6topoem_flavor_weight_JetFitterTag); }
  if( fChain->GetBranchStatus("jet_akt6topoem_flavor_weight_JetFitterCOMB") ) { fChain->SetBranchAddress("jet_akt6topoem_flavor_weight_JetFitterCOMB", &jet_akt6topoem_flavor_weight_JetFitterCOMB, &b_jet_akt6topoem_flavor_weight_JetFitterCOMB); }
  if( fChain->GetBranchStatus("jet_akt6topoem_flavor_weight_JetFitterTagNN") ) { fChain->SetBranchAddress("jet_akt6topoem_flavor_weight_JetFitterTagNN", &jet_akt6topoem_flavor_weight_JetFitterTagNN, &b_jet_akt6topoem_flavor_weight_JetFitterTagNN); }
  if( fChain->GetBranchStatus("jet_akt6topoem_flavor_weight_JetFitterCOMBNN") ) { fChain->SetBranchAddress("jet_akt6topoem_flavor_weight_JetFitterCOMBNN", &jet_akt6topoem_flavor_weight_JetFitterCOMBNN, &b_jet_akt6topoem_flavor_weight_JetFitterCOMBNN); }
  if( fChain->GetBranchStatus("jet_akt6topoem_flavor_weight_SoftMuonTag") ) { fChain->SetBranchAddress("jet_akt6topoem_flavor_weight_SoftMuonTag", &jet_akt6topoem_flavor_weight_SoftMuonTag, &b_jet_akt6topoem_flavor_weight_SoftMuonTag); }
  if( fChain->GetBranchStatus("jet_akt6topoem_flavor_weight_SoftElectronTag") ) { fChain->SetBranchAddress("jet_akt6topoem_flavor_weight_SoftElectronTag", &jet_akt6topoem_flavor_weight_SoftElectronTag, &b_jet_akt6topoem_flavor_weight_SoftElectronTag); }
  if( fChain->GetBranchStatus("jet_akt6topoem_flavor_truth_dRminToB") ) { fChain->SetBranchAddress("jet_akt6topoem_flavor_truth_dRminToB", &jet_akt6topoem_flavor_truth_dRminToB, &b_jet_akt6topoem_flavor_truth_dRminToB); }
  if( fChain->GetBranchStatus("jet_akt6topoem_flavor_truth_dRminToC") ) { fChain->SetBranchAddress("jet_akt6topoem_flavor_truth_dRminToC", &jet_akt6topoem_flavor_truth_dRminToC, &b_jet_akt6topoem_flavor_truth_dRminToC); }
  if( fChain->GetBranchStatus("jet_akt6topoem_flavor_truth_dRminToT") ) { fChain->SetBranchAddress("jet_akt6topoem_flavor_truth_dRminToT", &jet_akt6topoem_flavor_truth_dRminToT, &b_jet_akt6topoem_flavor_truth_dRminToT); }
  if( fChain->GetBranchStatus("jet_akt6topoem_flavor_truth_BHadronpdg") ) { fChain->SetBranchAddress("jet_akt6topoem_flavor_truth_BHadronpdg", &jet_akt6topoem_flavor_truth_BHadronpdg, &b_jet_akt6topoem_flavor_truth_BHadronpdg); }
  if( fChain->GetBranchStatus("jet_akt6topoem_flavor_truth_trueflav") ) { fChain->SetBranchAddress("jet_akt6topoem_flavor_truth_trueflav", &jet_akt6topoem_flavor_truth_trueflav, &b_jet_akt6topoem_flavor_truth_trueflav); }
  if( fChain->GetBranchStatus("jet_akt6topoem_el_dr") ) { fChain->SetBranchAddress("jet_akt6topoem_el_dr", &jet_akt6topoem_el_dr, &b_jet_akt6topoem_el_dr); }
  if( fChain->GetBranchStatus("jet_akt6topoem_el_matched") ) { fChain->SetBranchAddress("jet_akt6topoem_el_matched", &jet_akt6topoem_el_matched, &b_jet_akt6topoem_el_matched); }
  if( fChain->GetBranchStatus("jet_akt6topoem_mu_dr") ) { fChain->SetBranchAddress("jet_akt6topoem_mu_dr", &jet_akt6topoem_mu_dr, &b_jet_akt6topoem_mu_dr); }
  if( fChain->GetBranchStatus("jet_akt6topoem_mu_matched") ) { fChain->SetBranchAddress("jet_akt6topoem_mu_matched", &jet_akt6topoem_mu_matched, &b_jet_akt6topoem_mu_matched); }
  if( fChain->GetBranchStatus("jet_akt6topoem_L1_dr") ) { fChain->SetBranchAddress("jet_akt6topoem_L1_dr", &jet_akt6topoem_L1_dr, &b_jet_akt6topoem_L1_dr); }
  if( fChain->GetBranchStatus("jet_akt6topoem_L1_matched") ) { fChain->SetBranchAddress("jet_akt6topoem_L1_matched", &jet_akt6topoem_L1_matched, &b_jet_akt6topoem_L1_matched); }
  if( fChain->GetBranchStatus("jet_akt6topoem_L2_dr") ) { fChain->SetBranchAddress("jet_akt6topoem_L2_dr", &jet_akt6topoem_L2_dr, &b_jet_akt6topoem_L2_dr); }
  if( fChain->GetBranchStatus("jet_akt6topoem_L2_matched") ) { fChain->SetBranchAddress("jet_akt6topoem_L2_matched", &jet_akt6topoem_L2_matched, &b_jet_akt6topoem_L2_matched); }
  if( fChain->GetBranchStatus("jet_akt6topoem_EF_dr") ) { fChain->SetBranchAddress("jet_akt6topoem_EF_dr", &jet_akt6topoem_EF_dr, &b_jet_akt6topoem_EF_dr); }
  if( fChain->GetBranchStatus("jet_akt6topoem_EF_matched") ) { fChain->SetBranchAddress("jet_akt6topoem_EF_matched", &jet_akt6topoem_EF_matched, &b_jet_akt6topoem_EF_matched); }
  if( fChain->GetBranchStatus("MET_RefFinal_phi") ) { fChain->SetBranchAddress("MET_RefFinal_phi", &MET_RefFinal_phi, &b_MET_RefFinal_phi); }
  if( fChain->GetBranchStatus("MET_RefFinal_et") ) { fChain->SetBranchAddress("MET_RefFinal_et", &MET_RefFinal_et, &b_MET_RefFinal_et); }
  if( fChain->GetBranchStatus("MET_RefFinal_sumet") ) { fChain->SetBranchAddress("MET_RefFinal_sumet", &MET_RefFinal_sumet, &b_MET_RefFinal_sumet); }
  if( fChain->GetBranchStatus("MET_Cryo_phi") ) { fChain->SetBranchAddress("MET_Cryo_phi", &MET_Cryo_phi, &b_MET_Cryo_phi); }
  if( fChain->GetBranchStatus("MET_Cryo_et") ) { fChain->SetBranchAddress("MET_Cryo_et", &MET_Cryo_et, &b_MET_Cryo_et); }
  if( fChain->GetBranchStatus("MET_Cryo_sumet") ) { fChain->SetBranchAddress("MET_Cryo_sumet", &MET_Cryo_sumet, &b_MET_Cryo_sumet); }
  if( fChain->GetBranchStatus("MET_RefEle_phi") ) { fChain->SetBranchAddress("MET_RefEle_phi", &MET_RefEle_phi, &b_MET_RefEle_phi); }
  if( fChain->GetBranchStatus("MET_RefEle_et") ) { fChain->SetBranchAddress("MET_RefEle_et", &MET_RefEle_et, &b_MET_RefEle_et); }
  if( fChain->GetBranchStatus("MET_RefEle_sumet") ) { fChain->SetBranchAddress("MET_RefEle_sumet", &MET_RefEle_sumet, &b_MET_RefEle_sumet); }
  if( fChain->GetBranchStatus("MET_RefJet_phi") ) { fChain->SetBranchAddress("MET_RefJet_phi", &MET_RefJet_phi, &b_MET_RefJet_phi); }
  if( fChain->GetBranchStatus("MET_RefJet_et") ) { fChain->SetBranchAddress("MET_RefJet_et", &MET_RefJet_et, &b_MET_RefJet_et); }
  if( fChain->GetBranchStatus("MET_RefJet_sumet") ) { fChain->SetBranchAddress("MET_RefJet_sumet", &MET_RefJet_sumet, &b_MET_RefJet_sumet); }
  if( fChain->GetBranchStatus("MET_SoftJets_phi") ) { fChain->SetBranchAddress("MET_SoftJets_phi", &MET_SoftJets_phi, &b_MET_SoftJets_phi); }
  if( fChain->GetBranchStatus("MET_SoftJets_et") ) { fChain->SetBranchAddress("MET_SoftJets_et", &MET_SoftJets_et, &b_MET_SoftJets_et); }
  if( fChain->GetBranchStatus("MET_SoftJets_sumet") ) { fChain->SetBranchAddress("MET_SoftJets_sumet", &MET_SoftJets_sumet, &b_MET_SoftJets_sumet); }
  if( fChain->GetBranchStatus("MET_RefMuon_phi") ) { fChain->SetBranchAddress("MET_RefMuon_phi", &MET_RefMuon_phi, &b_MET_RefMuon_phi); }
  if( fChain->GetBranchStatus("MET_RefMuon_et") ) { fChain->SetBranchAddress("MET_RefMuon_et", &MET_RefMuon_et, &b_MET_RefMuon_et); }
  if( fChain->GetBranchStatus("MET_RefMuon_sumet") ) { fChain->SetBranchAddress("MET_RefMuon_sumet", &MET_RefMuon_sumet, &b_MET_RefMuon_sumet); }
  if( fChain->GetBranchStatus("MET_RefMuon_Staco_phi") ) { fChain->SetBranchAddress("MET_RefMuon_Staco_phi", &MET_RefMuon_Staco_phi, &b_MET_RefMuon_Staco_phi); }
  if( fChain->GetBranchStatus("MET_RefMuon_Staco_et") ) { fChain->SetBranchAddress("MET_RefMuon_Staco_et", &MET_RefMuon_Staco_et, &b_MET_RefMuon_Staco_et); }
  if( fChain->GetBranchStatus("MET_RefMuon_Staco_sumet") ) { fChain->SetBranchAddress("MET_RefMuon_Staco_sumet", &MET_RefMuon_Staco_sumet, &b_MET_RefMuon_Staco_sumet); }
  if( fChain->GetBranchStatus("MET_RefMuon_Muid_phi") ) { fChain->SetBranchAddress("MET_RefMuon_Muid_phi", &MET_RefMuon_Muid_phi, &b_MET_RefMuon_Muid_phi); }
  if( fChain->GetBranchStatus("MET_RefMuon_Muid_et") ) { fChain->SetBranchAddress("MET_RefMuon_Muid_et", &MET_RefMuon_Muid_et, &b_MET_RefMuon_Muid_et); }
  if( fChain->GetBranchStatus("MET_RefMuon_Muid_sumet") ) { fChain->SetBranchAddress("MET_RefMuon_Muid_sumet", &MET_RefMuon_Muid_sumet, &b_MET_RefMuon_Muid_sumet); }
  if( fChain->GetBranchStatus("MET_RefGamma_phi") ) { fChain->SetBranchAddress("MET_RefGamma_phi", &MET_RefGamma_phi, &b_MET_RefGamma_phi); }
  if( fChain->GetBranchStatus("MET_RefGamma_et") ) { fChain->SetBranchAddress("MET_RefGamma_et", &MET_RefGamma_et, &b_MET_RefGamma_et); }
  if( fChain->GetBranchStatus("MET_RefGamma_sumet") ) { fChain->SetBranchAddress("MET_RefGamma_sumet", &MET_RefGamma_sumet, &b_MET_RefGamma_sumet); }
  if( fChain->GetBranchStatus("MET_RefTau_phi") ) { fChain->SetBranchAddress("MET_RefTau_phi", &MET_RefTau_phi, &b_MET_RefTau_phi); }
  if( fChain->GetBranchStatus("MET_RefTau_et") ) { fChain->SetBranchAddress("MET_RefTau_et", &MET_RefTau_et, &b_MET_RefTau_et); }
  if( fChain->GetBranchStatus("MET_RefTau_sumet") ) { fChain->SetBranchAddress("MET_RefTau_sumet", &MET_RefTau_sumet, &b_MET_RefTau_sumet); }
  if( fChain->GetBranchStatus("MET_CellOut_phi") ) { fChain->SetBranchAddress("MET_CellOut_phi", &MET_CellOut_phi, &b_MET_CellOut_phi); }
  if( fChain->GetBranchStatus("MET_CellOut_et") ) { fChain->SetBranchAddress("MET_CellOut_et", &MET_CellOut_et, &b_MET_CellOut_et); }
  if( fChain->GetBranchStatus("MET_CellOut_sumet") ) { fChain->SetBranchAddress("MET_CellOut_sumet", &MET_CellOut_sumet, &b_MET_CellOut_sumet); }
  if( fChain->GetBranchStatus("MET_Track_phi") ) { fChain->SetBranchAddress("MET_Track_phi", &MET_Track_phi, &b_MET_Track_phi); }
  if( fChain->GetBranchStatus("MET_Track_et") ) { fChain->SetBranchAddress("MET_Track_et", &MET_Track_et, &b_MET_Track_et); }
  if( fChain->GetBranchStatus("MET_Track_sumet") ) { fChain->SetBranchAddress("MET_Track_sumet", &MET_Track_sumet, &b_MET_Track_sumet); }
  if( fChain->GetBranchStatus("MET_RefFinal_em_etx") ) { fChain->SetBranchAddress("MET_RefFinal_em_etx", &MET_RefFinal_em_etx, &b_MET_RefFinal_em_etx); }
  if( fChain->GetBranchStatus("MET_RefFinal_em_ety") ) { fChain->SetBranchAddress("MET_RefFinal_em_ety", &MET_RefFinal_em_ety, &b_MET_RefFinal_em_ety); }
  if( fChain->GetBranchStatus("MET_RefFinal_em_phi") ) { fChain->SetBranchAddress("MET_RefFinal_em_phi", &MET_RefFinal_em_phi, &b_MET_RefFinal_em_phi); }
  if( fChain->GetBranchStatus("MET_RefFinal_em_et") ) { fChain->SetBranchAddress("MET_RefFinal_em_et", &MET_RefFinal_em_et, &b_MET_RefFinal_em_et); }
  if( fChain->GetBranchStatus("MET_RefFinal_em_sumet") ) { fChain->SetBranchAddress("MET_RefFinal_em_sumet", &MET_RefFinal_em_sumet, &b_MET_RefFinal_em_sumet); }
  if( fChain->GetBranchStatus("MET_RefEle_em_phi") ) { fChain->SetBranchAddress("MET_RefEle_em_phi", &MET_RefEle_em_phi, &b_MET_RefEle_em_phi); }
  if( fChain->GetBranchStatus("MET_RefEle_em_et") ) { fChain->SetBranchAddress("MET_RefEle_em_et", &MET_RefEle_em_et, &b_MET_RefEle_em_et); }
  if( fChain->GetBranchStatus("MET_RefEle_em_sumet") ) { fChain->SetBranchAddress("MET_RefEle_em_sumet", &MET_RefEle_em_sumet, &b_MET_RefEle_em_sumet); }
  if( fChain->GetBranchStatus("MET_RefJet_em_phi") ) { fChain->SetBranchAddress("MET_RefJet_em_phi", &MET_RefJet_em_phi, &b_MET_RefJet_em_phi); }
  if( fChain->GetBranchStatus("MET_RefJet_em_et") ) { fChain->SetBranchAddress("MET_RefJet_em_et", &MET_RefJet_em_et, &b_MET_RefJet_em_et); }
  if( fChain->GetBranchStatus("MET_RefJet_em_sumet") ) { fChain->SetBranchAddress("MET_RefJet_em_sumet", &MET_RefJet_em_sumet, &b_MET_RefJet_em_sumet); }
  if( fChain->GetBranchStatus("MET_SoftJets_em_phi") ) { fChain->SetBranchAddress("MET_SoftJets_em_phi", &MET_SoftJets_em_phi, &b_MET_SoftJets_em_phi); }
  if( fChain->GetBranchStatus("MET_SoftJets_em_et") ) { fChain->SetBranchAddress("MET_SoftJets_em_et", &MET_SoftJets_em_et, &b_MET_SoftJets_em_et); }
  if( fChain->GetBranchStatus("MET_SoftJets_em_sumet") ) { fChain->SetBranchAddress("MET_SoftJets_em_sumet", &MET_SoftJets_em_sumet, &b_MET_SoftJets_em_sumet); }
  if( fChain->GetBranchStatus("MET_RefMuon_em_phi") ) { fChain->SetBranchAddress("MET_RefMuon_em_phi", &MET_RefMuon_em_phi, &b_MET_RefMuon_em_phi); }
  if( fChain->GetBranchStatus("MET_RefMuon_em_et") ) { fChain->SetBranchAddress("MET_RefMuon_em_et", &MET_RefMuon_em_et, &b_MET_RefMuon_em_et); }
  if( fChain->GetBranchStatus("MET_RefMuon_em_sumet") ) { fChain->SetBranchAddress("MET_RefMuon_em_sumet", &MET_RefMuon_em_sumet, &b_MET_RefMuon_em_sumet); }
  if( fChain->GetBranchStatus("MET_RefMuon_Track_em_phi") ) { fChain->SetBranchAddress("MET_RefMuon_Track_em_phi", &MET_RefMuon_Track_em_phi, &b_MET_RefMuon_Track_em_phi); }
  if( fChain->GetBranchStatus("MET_RefMuon_Track_em_et") ) { fChain->SetBranchAddress("MET_RefMuon_Track_em_et", &MET_RefMuon_Track_em_et, &b_MET_RefMuon_Track_em_et); }
  if( fChain->GetBranchStatus("MET_RefMuon_Track_em_sumet") ) { fChain->SetBranchAddress("MET_RefMuon_Track_em_sumet", &MET_RefMuon_Track_em_sumet, &b_MET_RefMuon_Track_em_sumet); }
  if( fChain->GetBranchStatus("MET_RefGamma_em_phi") ) { fChain->SetBranchAddress("MET_RefGamma_em_phi", &MET_RefGamma_em_phi, &b_MET_RefGamma_em_phi); }
  if( fChain->GetBranchStatus("MET_RefGamma_em_et") ) { fChain->SetBranchAddress("MET_RefGamma_em_et", &MET_RefGamma_em_et, &b_MET_RefGamma_em_et); }
  if( fChain->GetBranchStatus("MET_RefGamma_em_sumet") ) { fChain->SetBranchAddress("MET_RefGamma_em_sumet", &MET_RefGamma_em_sumet, &b_MET_RefGamma_em_sumet); }
  if( fChain->GetBranchStatus("MET_RefTau_em_phi") ) { fChain->SetBranchAddress("MET_RefTau_em_phi", &MET_RefTau_em_phi, &b_MET_RefTau_em_phi); }
  if( fChain->GetBranchStatus("MET_RefTau_em_et") ) { fChain->SetBranchAddress("MET_RefTau_em_et", &MET_RefTau_em_et, &b_MET_RefTau_em_et); }
  if( fChain->GetBranchStatus("MET_RefTau_em_sumet") ) { fChain->SetBranchAddress("MET_RefTau_em_sumet", &MET_RefTau_em_sumet, &b_MET_RefTau_em_sumet); }
  if( fChain->GetBranchStatus("MET_CellOut_em_etx") ) { fChain->SetBranchAddress("MET_CellOut_em_etx", &MET_CellOut_em_etx, &b_MET_CellOut_em_etx); }
  if( fChain->GetBranchStatus("MET_CellOut_em_ety") ) { fChain->SetBranchAddress("MET_CellOut_em_ety", &MET_CellOut_em_ety, &b_MET_CellOut_em_ety); }
  if( fChain->GetBranchStatus("MET_CellOut_em_phi") ) { fChain->SetBranchAddress("MET_CellOut_em_phi", &MET_CellOut_em_phi, &b_MET_CellOut_em_phi); }
  if( fChain->GetBranchStatus("MET_CellOut_em_et") ) { fChain->SetBranchAddress("MET_CellOut_em_et", &MET_CellOut_em_et, &b_MET_CellOut_em_et); }
  if( fChain->GetBranchStatus("MET_CellOut_em_sumet") ) { fChain->SetBranchAddress("MET_CellOut_em_sumet", &MET_CellOut_em_sumet, &b_MET_CellOut_em_sumet); }
  if( fChain->GetBranchStatus("MET_Muon_Isol_Staco_phi") ) { fChain->SetBranchAddress("MET_Muon_Isol_Staco_phi", &MET_Muon_Isol_Staco_phi, &b_MET_Muon_Isol_Staco_phi); }
  if( fChain->GetBranchStatus("MET_Muon_Isol_Staco_et") ) { fChain->SetBranchAddress("MET_Muon_Isol_Staco_et", &MET_Muon_Isol_Staco_et, &b_MET_Muon_Isol_Staco_et); }
  if( fChain->GetBranchStatus("MET_Muon_Isol_Staco_sumet") ) { fChain->SetBranchAddress("MET_Muon_Isol_Staco_sumet", &MET_Muon_Isol_Staco_sumet, &b_MET_Muon_Isol_Staco_sumet); }
  if( fChain->GetBranchStatus("MET_Muon_NonIsol_Staco_phi") ) { fChain->SetBranchAddress("MET_Muon_NonIsol_Staco_phi", &MET_Muon_NonIsol_Staco_phi, &b_MET_Muon_NonIsol_Staco_phi); }
  if( fChain->GetBranchStatus("MET_Muon_NonIsol_Staco_et") ) { fChain->SetBranchAddress("MET_Muon_NonIsol_Staco_et", &MET_Muon_NonIsol_Staco_et, &b_MET_Muon_NonIsol_Staco_et); }
  if( fChain->GetBranchStatus("MET_Muon_NonIsol_Staco_sumet") ) { fChain->SetBranchAddress("MET_Muon_NonIsol_Staco_sumet", &MET_Muon_NonIsol_Staco_sumet, &b_MET_Muon_NonIsol_Staco_sumet); }
  if( fChain->GetBranchStatus("MET_Muon_Total_Staco_phi") ) { fChain->SetBranchAddress("MET_Muon_Total_Staco_phi", &MET_Muon_Total_Staco_phi, &b_MET_Muon_Total_Staco_phi); }
  if( fChain->GetBranchStatus("MET_Muon_Total_Staco_et") ) { fChain->SetBranchAddress("MET_Muon_Total_Staco_et", &MET_Muon_Total_Staco_et, &b_MET_Muon_Total_Staco_et); }
  if( fChain->GetBranchStatus("MET_Muon_Total_Staco_sumet") ) { fChain->SetBranchAddress("MET_Muon_Total_Staco_sumet", &MET_Muon_Total_Staco_sumet, &b_MET_Muon_Total_Staco_sumet); }
  if( fChain->GetBranchStatus("MET_Muon_Isol_Muid_phi") ) { fChain->SetBranchAddress("MET_Muon_Isol_Muid_phi", &MET_Muon_Isol_Muid_phi, &b_MET_Muon_Isol_Muid_phi); }
  if( fChain->GetBranchStatus("MET_Muon_Isol_Muid_et") ) { fChain->SetBranchAddress("MET_Muon_Isol_Muid_et", &MET_Muon_Isol_Muid_et, &b_MET_Muon_Isol_Muid_et); }
  if( fChain->GetBranchStatus("MET_Muon_Isol_Muid_sumet") ) { fChain->SetBranchAddress("MET_Muon_Isol_Muid_sumet", &MET_Muon_Isol_Muid_sumet, &b_MET_Muon_Isol_Muid_sumet); }
  if( fChain->GetBranchStatus("MET_Muon_NonIsol_Muid_phi") ) { fChain->SetBranchAddress("MET_Muon_NonIsol_Muid_phi", &MET_Muon_NonIsol_Muid_phi, &b_MET_Muon_NonIsol_Muid_phi); }
  if( fChain->GetBranchStatus("MET_Muon_NonIsol_Muid_et") ) { fChain->SetBranchAddress("MET_Muon_NonIsol_Muid_et", &MET_Muon_NonIsol_Muid_et, &b_MET_Muon_NonIsol_Muid_et); }
  if( fChain->GetBranchStatus("MET_Muon_NonIsol_Muid_sumet") ) { fChain->SetBranchAddress("MET_Muon_NonIsol_Muid_sumet", &MET_Muon_NonIsol_Muid_sumet, &b_MET_Muon_NonIsol_Muid_sumet); }
  if( fChain->GetBranchStatus("MET_Muon_Total_Muid_phi") ) { fChain->SetBranchAddress("MET_Muon_Total_Muid_phi", &MET_Muon_Total_Muid_phi, &b_MET_Muon_Total_Muid_phi); }
  if( fChain->GetBranchStatus("MET_Muon_Total_Muid_et") ) { fChain->SetBranchAddress("MET_Muon_Total_Muid_et", &MET_Muon_Total_Muid_et, &b_MET_Muon_Total_Muid_et); }
  if( fChain->GetBranchStatus("MET_Muon_Total_Muid_sumet") ) { fChain->SetBranchAddress("MET_Muon_Total_Muid_sumet", &MET_Muon_Total_Muid_sumet, &b_MET_Muon_Total_Muid_sumet); }
  if( fChain->GetBranchStatus("MET_MuonBoy_phi") ) { fChain->SetBranchAddress("MET_MuonBoy_phi", &MET_MuonBoy_phi, &b_MET_MuonBoy_phi); }
  if( fChain->GetBranchStatus("MET_MuonBoy_et") ) { fChain->SetBranchAddress("MET_MuonBoy_et", &MET_MuonBoy_et, &b_MET_MuonBoy_et); }
  if( fChain->GetBranchStatus("MET_MuonBoy_sumet") ) { fChain->SetBranchAddress("MET_MuonBoy_sumet", &MET_MuonBoy_sumet, &b_MET_MuonBoy_sumet); }
  if( fChain->GetBranchStatus("MET_RefMuon_Track_phi") ) { fChain->SetBranchAddress("MET_RefMuon_Track_phi", &MET_RefMuon_Track_phi, &b_MET_RefMuon_Track_phi); }
  if( fChain->GetBranchStatus("MET_RefMuon_Track_et") ) { fChain->SetBranchAddress("MET_RefMuon_Track_et", &MET_RefMuon_Track_et, &b_MET_RefMuon_Track_et); }
  if( fChain->GetBranchStatus("MET_RefMuon_Track_sumet") ) { fChain->SetBranchAddress("MET_RefMuon_Track_sumet", &MET_RefMuon_Track_sumet, &b_MET_RefMuon_Track_sumet); }
  if( fChain->GetBranchStatus("MET_RefMuon_Track_Staco_phi") ) { fChain->SetBranchAddress("MET_RefMuon_Track_Staco_phi", &MET_RefMuon_Track_Staco_phi, &b_MET_RefMuon_Track_Staco_phi); }
  if( fChain->GetBranchStatus("MET_RefMuon_Track_Staco_et") ) { fChain->SetBranchAddress("MET_RefMuon_Track_Staco_et", &MET_RefMuon_Track_Staco_et, &b_MET_RefMuon_Track_Staco_et); }
  if( fChain->GetBranchStatus("MET_RefMuon_Track_Staco_sumet") ) { fChain->SetBranchAddress("MET_RefMuon_Track_Staco_sumet", &MET_RefMuon_Track_Staco_sumet, &b_MET_RefMuon_Track_Staco_sumet); }
  if( fChain->GetBranchStatus("MET_RefMuon_Track_Muid_phi") ) { fChain->SetBranchAddress("MET_RefMuon_Track_Muid_phi", &MET_RefMuon_Track_Muid_phi, &b_MET_RefMuon_Track_Muid_phi); }
  if( fChain->GetBranchStatus("MET_RefMuon_Track_Muid_et") ) { fChain->SetBranchAddress("MET_RefMuon_Track_Muid_et", &MET_RefMuon_Track_Muid_et, &b_MET_RefMuon_Track_Muid_et); }
  if( fChain->GetBranchStatus("MET_RefMuon_Track_Muid_sumet") ) { fChain->SetBranchAddress("MET_RefMuon_Track_Muid_sumet", &MET_RefMuon_Track_Muid_sumet, &b_MET_RefMuon_Track_Muid_sumet); }
  if( fChain->GetBranchStatus("MET_CryoCone_phi") ) { fChain->SetBranchAddress("MET_CryoCone_phi", &MET_CryoCone_phi, &b_MET_CryoCone_phi); }
  if( fChain->GetBranchStatus("MET_CryoCone_et") ) { fChain->SetBranchAddress("MET_CryoCone_et", &MET_CryoCone_et, &b_MET_CryoCone_et); }
  if( fChain->GetBranchStatus("MET_CryoCone_sumet") ) { fChain->SetBranchAddress("MET_CryoCone_sumet", &MET_CryoCone_sumet, &b_MET_CryoCone_sumet); }
  if( fChain->GetBranchStatus("MET_Final_phi") ) { fChain->SetBranchAddress("MET_Final_phi", &MET_Final_phi, &b_MET_Final_phi); }
  if( fChain->GetBranchStatus("MET_Final_et") ) { fChain->SetBranchAddress("MET_Final_et", &MET_Final_et, &b_MET_Final_et); }
  if( fChain->GetBranchStatus("MET_Final_sumet") ) { fChain->SetBranchAddress("MET_Final_sumet", &MET_Final_sumet, &b_MET_Final_sumet); }
  if( fChain->GetBranchStatus("MET_MuonBoy_Spectro_phi") ) { fChain->SetBranchAddress("MET_MuonBoy_Spectro_phi", &MET_MuonBoy_Spectro_phi, &b_MET_MuonBoy_Spectro_phi); }
  if( fChain->GetBranchStatus("MET_MuonBoy_Spectro_et") ) { fChain->SetBranchAddress("MET_MuonBoy_Spectro_et", &MET_MuonBoy_Spectro_et, &b_MET_MuonBoy_Spectro_et); }
  if( fChain->GetBranchStatus("MET_MuonBoy_Spectro_sumet") ) { fChain->SetBranchAddress("MET_MuonBoy_Spectro_sumet", &MET_MuonBoy_Spectro_sumet, &b_MET_MuonBoy_Spectro_sumet); }
  if( fChain->GetBranchStatus("MET_MuonBoy_Track_phi") ) { fChain->SetBranchAddress("MET_MuonBoy_Track_phi", &MET_MuonBoy_Track_phi, &b_MET_MuonBoy_Track_phi); }
  if( fChain->GetBranchStatus("MET_MuonBoy_Track_et") ) { fChain->SetBranchAddress("MET_MuonBoy_Track_et", &MET_MuonBoy_Track_et, &b_MET_MuonBoy_Track_et); }
  if( fChain->GetBranchStatus("MET_MuonBoy_Track_sumet") ) { fChain->SetBranchAddress("MET_MuonBoy_Track_sumet", &MET_MuonBoy_Track_sumet, &b_MET_MuonBoy_Track_sumet); }
  if( fChain->GetBranchStatus("MET_MuonMuid_phi") ) { fChain->SetBranchAddress("MET_MuonMuid_phi", &MET_MuonMuid_phi, &b_MET_MuonMuid_phi); }
  if( fChain->GetBranchStatus("MET_MuonMuid_et") ) { fChain->SetBranchAddress("MET_MuonMuid_et", &MET_MuonMuid_et, &b_MET_MuonMuid_et); }
  if( fChain->GetBranchStatus("MET_MuonMuid_sumet") ) { fChain->SetBranchAddress("MET_MuonMuid_sumet", &MET_MuonMuid_sumet, &b_MET_MuonMuid_sumet); }
  if( fChain->GetBranchStatus("MET_Muid_phi") ) { fChain->SetBranchAddress("MET_Muid_phi", &MET_Muid_phi, &b_MET_Muid_phi); }
  if( fChain->GetBranchStatus("MET_Muid_et") ) { fChain->SetBranchAddress("MET_Muid_et", &MET_Muid_et, &b_MET_Muid_et); }
  if( fChain->GetBranchStatus("MET_Muid_sumet") ) { fChain->SetBranchAddress("MET_Muid_sumet", &MET_Muid_sumet, &b_MET_Muid_sumet); }
  if( fChain->GetBranchStatus("MET_Muid_Spectro_phi") ) { fChain->SetBranchAddress("MET_Muid_Spectro_phi", &MET_Muid_Spectro_phi, &b_MET_Muid_Spectro_phi); }
  if( fChain->GetBranchStatus("MET_Muid_Spectro_et") ) { fChain->SetBranchAddress("MET_Muid_Spectro_et", &MET_Muid_Spectro_et, &b_MET_Muid_Spectro_et); }
  if( fChain->GetBranchStatus("MET_Muid_Spectro_sumet") ) { fChain->SetBranchAddress("MET_Muid_Spectro_sumet", &MET_Muid_Spectro_sumet, &b_MET_Muid_Spectro_sumet); }
  if( fChain->GetBranchStatus("MET_Muid_Track_phi") ) { fChain->SetBranchAddress("MET_Muid_Track_phi", &MET_Muid_Track_phi, &b_MET_Muid_Track_phi); }
  if( fChain->GetBranchStatus("MET_Muid_Track_et") ) { fChain->SetBranchAddress("MET_Muid_Track_et", &MET_Muid_Track_et, &b_MET_Muid_Track_et); }
  if( fChain->GetBranchStatus("MET_Muid_Track_sumet") ) { fChain->SetBranchAddress("MET_Muid_Track_sumet", &MET_Muid_Track_sumet, &b_MET_Muid_Track_sumet); }
  if( fChain->GetBranchStatus("MET_Muon_phi") ) { fChain->SetBranchAddress("MET_Muon_phi", &MET_Muon_phi, &b_MET_Muon_phi); }
  if( fChain->GetBranchStatus("MET_Muon_et") ) { fChain->SetBranchAddress("MET_Muon_et", &MET_Muon_et, &b_MET_Muon_et); }
  if( fChain->GetBranchStatus("MET_Muon_sumet") ) { fChain->SetBranchAddress("MET_Muon_sumet", &MET_Muon_sumet, &b_MET_Muon_sumet); }
  if( fChain->GetBranchStatus("MET_TopoObj_phi") ) { fChain->SetBranchAddress("MET_TopoObj_phi", &MET_TopoObj_phi, &b_MET_TopoObj_phi); }
  if( fChain->GetBranchStatus("MET_TopoObj_et") ) { fChain->SetBranchAddress("MET_TopoObj_et", &MET_TopoObj_et, &b_MET_TopoObj_et); }
  if( fChain->GetBranchStatus("MET_TopoObj_sumet") ) { fChain->SetBranchAddress("MET_TopoObj_sumet", &MET_TopoObj_sumet, &b_MET_TopoObj_sumet); }
  if( fChain->GetBranchStatus("MET_LocHadTopoObj_phi") ) { fChain->SetBranchAddress("MET_LocHadTopoObj_phi", &MET_LocHadTopoObj_phi, &b_MET_LocHadTopoObj_phi); }
  if( fChain->GetBranchStatus("MET_LocHadTopoObj_et") ) { fChain->SetBranchAddress("MET_LocHadTopoObj_et", &MET_LocHadTopoObj_et, &b_MET_LocHadTopoObj_et); }
  if( fChain->GetBranchStatus("MET_LocHadTopoObj_sumet") ) { fChain->SetBranchAddress("MET_LocHadTopoObj_sumet", &MET_LocHadTopoObj_sumet, &b_MET_LocHadTopoObj_sumet); }
  if( fChain->GetBranchStatus("MET_Topo_phi") ) { fChain->SetBranchAddress("MET_Topo_phi", &MET_Topo_phi, &b_MET_Topo_phi); }
  if( fChain->GetBranchStatus("MET_Topo_et") ) { fChain->SetBranchAddress("MET_Topo_et", &MET_Topo_et, &b_MET_Topo_et); }
  if( fChain->GetBranchStatus("MET_Topo_sumet") ) { fChain->SetBranchAddress("MET_Topo_sumet", &MET_Topo_sumet, &b_MET_Topo_sumet); }
  if( fChain->GetBranchStatus("MET_Topo_SUMET_EMFrac") ) { fChain->SetBranchAddress("MET_Topo_SUMET_EMFrac", &MET_Topo_SUMET_EMFrac, &b_MET_Topo_SUMET_EMFrac); }
  if( fChain->GetBranchStatus("MET_Topo_etx_PEMB") ) { fChain->SetBranchAddress("MET_Topo_etx_PEMB", &MET_Topo_etx_PEMB, &b_MET_Topo_etx_PEMB); }
  if( fChain->GetBranchStatus("MET_Topo_ety_PEMB") ) { fChain->SetBranchAddress("MET_Topo_ety_PEMB", &MET_Topo_ety_PEMB, &b_MET_Topo_ety_PEMB); }
  if( fChain->GetBranchStatus("MET_Topo_sumet_PEMB") ) { fChain->SetBranchAddress("MET_Topo_sumet_PEMB", &MET_Topo_sumet_PEMB, &b_MET_Topo_sumet_PEMB); }
  if( fChain->GetBranchStatus("MET_Topo_phi_PEMB") ) { fChain->SetBranchAddress("MET_Topo_phi_PEMB", &MET_Topo_phi_PEMB, &b_MET_Topo_phi_PEMB); }
  if( fChain->GetBranchStatus("MET_Topo_etx_EMB") ) { fChain->SetBranchAddress("MET_Topo_etx_EMB", &MET_Topo_etx_EMB, &b_MET_Topo_etx_EMB); }
  if( fChain->GetBranchStatus("MET_Topo_ety_EMB") ) { fChain->SetBranchAddress("MET_Topo_ety_EMB", &MET_Topo_ety_EMB, &b_MET_Topo_ety_EMB); }
  if( fChain->GetBranchStatus("MET_Topo_sumet_EMB") ) { fChain->SetBranchAddress("MET_Topo_sumet_EMB", &MET_Topo_sumet_EMB, &b_MET_Topo_sumet_EMB); }
  if( fChain->GetBranchStatus("MET_Topo_phi_EMB") ) { fChain->SetBranchAddress("MET_Topo_phi_EMB", &MET_Topo_phi_EMB, &b_MET_Topo_phi_EMB); }
  if( fChain->GetBranchStatus("MET_Topo_etx_PEMEC") ) { fChain->SetBranchAddress("MET_Topo_etx_PEMEC", &MET_Topo_etx_PEMEC, &b_MET_Topo_etx_PEMEC); }
  if( fChain->GetBranchStatus("MET_Topo_ety_PEMEC") ) { fChain->SetBranchAddress("MET_Topo_ety_PEMEC", &MET_Topo_ety_PEMEC, &b_MET_Topo_ety_PEMEC); }
  if( fChain->GetBranchStatus("MET_Topo_sumet_PEMEC") ) { fChain->SetBranchAddress("MET_Topo_sumet_PEMEC", &MET_Topo_sumet_PEMEC, &b_MET_Topo_sumet_PEMEC); }
  if( fChain->GetBranchStatus("MET_Topo_phi_PEMEC") ) { fChain->SetBranchAddress("MET_Topo_phi_PEMEC", &MET_Topo_phi_PEMEC, &b_MET_Topo_phi_PEMEC); }
  if( fChain->GetBranchStatus("MET_Topo_etx_EME") ) { fChain->SetBranchAddress("MET_Topo_etx_EME", &MET_Topo_etx_EME, &b_MET_Topo_etx_EME); }
  if( fChain->GetBranchStatus("MET_Topo_ety_EME") ) { fChain->SetBranchAddress("MET_Topo_ety_EME", &MET_Topo_ety_EME, &b_MET_Topo_ety_EME); }
  if( fChain->GetBranchStatus("MET_Topo_sumet_EME") ) { fChain->SetBranchAddress("MET_Topo_sumet_EME", &MET_Topo_sumet_EME, &b_MET_Topo_sumet_EME); }
  if( fChain->GetBranchStatus("MET_Topo_phi_EME") ) { fChain->SetBranchAddress("MET_Topo_phi_EME", &MET_Topo_phi_EME, &b_MET_Topo_phi_EME); }
  if( fChain->GetBranchStatus("MET_Topo_etx_TILE") ) { fChain->SetBranchAddress("MET_Topo_etx_TILE", &MET_Topo_etx_TILE, &b_MET_Topo_etx_TILE); }
  if( fChain->GetBranchStatus("MET_Topo_ety_TILE") ) { fChain->SetBranchAddress("MET_Topo_ety_TILE", &MET_Topo_ety_TILE, &b_MET_Topo_ety_TILE); }
  if( fChain->GetBranchStatus("MET_Topo_sumet_TILE") ) { fChain->SetBranchAddress("MET_Topo_sumet_TILE", &MET_Topo_sumet_TILE, &b_MET_Topo_sumet_TILE); }
  if( fChain->GetBranchStatus("MET_Topo_phi_TILE") ) { fChain->SetBranchAddress("MET_Topo_phi_TILE", &MET_Topo_phi_TILE, &b_MET_Topo_phi_TILE); }
  if( fChain->GetBranchStatus("MET_Topo_etx_HEC") ) { fChain->SetBranchAddress("MET_Topo_etx_HEC", &MET_Topo_etx_HEC, &b_MET_Topo_etx_HEC); }
  if( fChain->GetBranchStatus("MET_Topo_ety_HEC") ) { fChain->SetBranchAddress("MET_Topo_ety_HEC", &MET_Topo_ety_HEC, &b_MET_Topo_ety_HEC); }
  if( fChain->GetBranchStatus("MET_Topo_sumet_HEC") ) { fChain->SetBranchAddress("MET_Topo_sumet_HEC", &MET_Topo_sumet_HEC, &b_MET_Topo_sumet_HEC); }
  if( fChain->GetBranchStatus("MET_Topo_phi_HEC") ) { fChain->SetBranchAddress("MET_Topo_phi_HEC", &MET_Topo_phi_HEC, &b_MET_Topo_phi_HEC); }
  if( fChain->GetBranchStatus("MET_Topo_etx_FCAL") ) { fChain->SetBranchAddress("MET_Topo_etx_FCAL", &MET_Topo_etx_FCAL, &b_MET_Topo_etx_FCAL); }
  if( fChain->GetBranchStatus("MET_Topo_ety_FCAL") ) { fChain->SetBranchAddress("MET_Topo_ety_FCAL", &MET_Topo_ety_FCAL, &b_MET_Topo_ety_FCAL); }
  if( fChain->GetBranchStatus("MET_Topo_sumet_FCAL") ) { fChain->SetBranchAddress("MET_Topo_sumet_FCAL", &MET_Topo_sumet_FCAL, &b_MET_Topo_sumet_FCAL); }
  if( fChain->GetBranchStatus("MET_Topo_phi_FCAL") ) { fChain->SetBranchAddress("MET_Topo_phi_FCAL", &MET_Topo_phi_FCAL, &b_MET_Topo_phi_FCAL); }
  if( fChain->GetBranchStatus("MET_Topo_nCell_PEMB") ) { fChain->SetBranchAddress("MET_Topo_nCell_PEMB", &MET_Topo_nCell_PEMB, &b_MET_Topo_nCell_PEMB); }
  if( fChain->GetBranchStatus("MET_Topo_nCell_EMB") ) { fChain->SetBranchAddress("MET_Topo_nCell_EMB", &MET_Topo_nCell_EMB, &b_MET_Topo_nCell_EMB); }
  if( fChain->GetBranchStatus("MET_Topo_nCell_PEMEC") ) { fChain->SetBranchAddress("MET_Topo_nCell_PEMEC", &MET_Topo_nCell_PEMEC, &b_MET_Topo_nCell_PEMEC); }
  if( fChain->GetBranchStatus("MET_Topo_nCell_EME") ) { fChain->SetBranchAddress("MET_Topo_nCell_EME", &MET_Topo_nCell_EME, &b_MET_Topo_nCell_EME); }
  if( fChain->GetBranchStatus("MET_Topo_nCell_TILE") ) { fChain->SetBranchAddress("MET_Topo_nCell_TILE", &MET_Topo_nCell_TILE, &b_MET_Topo_nCell_TILE); }
  if( fChain->GetBranchStatus("MET_Topo_nCell_HEC") ) { fChain->SetBranchAddress("MET_Topo_nCell_HEC", &MET_Topo_nCell_HEC, &b_MET_Topo_nCell_HEC); }
  if( fChain->GetBranchStatus("MET_Topo_nCell_FCAL") ) { fChain->SetBranchAddress("MET_Topo_nCell_FCAL", &MET_Topo_nCell_FCAL, &b_MET_Topo_nCell_FCAL); }
  if( fChain->GetBranchStatus("MET_Topo_etx_CentralReg") ) { fChain->SetBranchAddress("MET_Topo_etx_CentralReg", &MET_Topo_etx_CentralReg, &b_MET_Topo_etx_CentralReg); }
  if( fChain->GetBranchStatus("MET_Topo_ety_CentralReg") ) { fChain->SetBranchAddress("MET_Topo_ety_CentralReg", &MET_Topo_ety_CentralReg, &b_MET_Topo_ety_CentralReg); }
  if( fChain->GetBranchStatus("MET_Topo_sumet_CentralReg") ) { fChain->SetBranchAddress("MET_Topo_sumet_CentralReg", &MET_Topo_sumet_CentralReg, &b_MET_Topo_sumet_CentralReg); }
  if( fChain->GetBranchStatus("MET_Topo_phi_CentralReg") ) { fChain->SetBranchAddress("MET_Topo_phi_CentralReg", &MET_Topo_phi_CentralReg, &b_MET_Topo_phi_CentralReg); }
  if( fChain->GetBranchStatus("MET_Topo_etx_EndcapRegion") ) { fChain->SetBranchAddress("MET_Topo_etx_EndcapRegion", &MET_Topo_etx_EndcapRegion, &b_MET_Topo_etx_EndcapRegion); }
  if( fChain->GetBranchStatus("MET_Topo_ety_EndcapRegion") ) { fChain->SetBranchAddress("MET_Topo_ety_EndcapRegion", &MET_Topo_ety_EndcapRegion, &b_MET_Topo_ety_EndcapRegion); }
  if( fChain->GetBranchStatus("MET_Topo_sumet_EndcapRegion") ) { fChain->SetBranchAddress("MET_Topo_sumet_EndcapRegion", &MET_Topo_sumet_EndcapRegion, &b_MET_Topo_sumet_EndcapRegion); }
  if( fChain->GetBranchStatus("MET_Topo_phi_EndcapRegion") ) { fChain->SetBranchAddress("MET_Topo_phi_EndcapRegion", &MET_Topo_phi_EndcapRegion, &b_MET_Topo_phi_EndcapRegion); }
  if( fChain->GetBranchStatus("MET_Topo_etx_ForwardReg") ) { fChain->SetBranchAddress("MET_Topo_etx_ForwardReg", &MET_Topo_etx_ForwardReg, &b_MET_Topo_etx_ForwardReg); }
  if( fChain->GetBranchStatus("MET_Topo_ety_ForwardReg") ) { fChain->SetBranchAddress("MET_Topo_ety_ForwardReg", &MET_Topo_ety_ForwardReg, &b_MET_Topo_ety_ForwardReg); }
  if( fChain->GetBranchStatus("MET_Topo_sumet_ForwardReg") ) { fChain->SetBranchAddress("MET_Topo_sumet_ForwardReg", &MET_Topo_sumet_ForwardReg, &b_MET_Topo_sumet_ForwardReg); }
  if( fChain->GetBranchStatus("MET_Topo_phi_ForwardReg") ) { fChain->SetBranchAddress("MET_Topo_phi_ForwardReg", &MET_Topo_phi_ForwardReg, &b_MET_Topo_phi_ForwardReg); }
  if( fChain->GetBranchStatus("MET_CorrTopo_phi") ) { fChain->SetBranchAddress("MET_CorrTopo_phi", &MET_CorrTopo_phi, &b_MET_CorrTopo_phi); }
  if( fChain->GetBranchStatus("MET_CorrTopo_et") ) { fChain->SetBranchAddress("MET_CorrTopo_et", &MET_CorrTopo_et, &b_MET_CorrTopo_et); }
  if( fChain->GetBranchStatus("MET_CorrTopo_sumet") ) { fChain->SetBranchAddress("MET_CorrTopo_sumet", &MET_CorrTopo_sumet, &b_MET_CorrTopo_sumet); }
  if( fChain->GetBranchStatus("MET_CorrTopo_SUMET_EMFrac") ) { fChain->SetBranchAddress("MET_CorrTopo_SUMET_EMFrac", &MET_CorrTopo_SUMET_EMFrac, &b_MET_CorrTopo_SUMET_EMFrac); }
  if( fChain->GetBranchStatus("MET_CorrTopo_etx_PEMB") ) { fChain->SetBranchAddress("MET_CorrTopo_etx_PEMB", &MET_CorrTopo_etx_PEMB, &b_MET_CorrTopo_etx_PEMB); }
  if( fChain->GetBranchStatus("MET_CorrTopo_ety_PEMB") ) { fChain->SetBranchAddress("MET_CorrTopo_ety_PEMB", &MET_CorrTopo_ety_PEMB, &b_MET_CorrTopo_ety_PEMB); }
  if( fChain->GetBranchStatus("MET_CorrTopo_sumet_PEMB") ) { fChain->SetBranchAddress("MET_CorrTopo_sumet_PEMB", &MET_CorrTopo_sumet_PEMB, &b_MET_CorrTopo_sumet_PEMB); }
  if( fChain->GetBranchStatus("MET_CorrTopo_phi_PEMB") ) { fChain->SetBranchAddress("MET_CorrTopo_phi_PEMB", &MET_CorrTopo_phi_PEMB, &b_MET_CorrTopo_phi_PEMB); }
  if( fChain->GetBranchStatus("MET_CorrTopo_etx_EMB") ) { fChain->SetBranchAddress("MET_CorrTopo_etx_EMB", &MET_CorrTopo_etx_EMB, &b_MET_CorrTopo_etx_EMB); }
  if( fChain->GetBranchStatus("MET_CorrTopo_ety_EMB") ) { fChain->SetBranchAddress("MET_CorrTopo_ety_EMB", &MET_CorrTopo_ety_EMB, &b_MET_CorrTopo_ety_EMB); }
  if( fChain->GetBranchStatus("MET_CorrTopo_sumet_EMB") ) { fChain->SetBranchAddress("MET_CorrTopo_sumet_EMB", &MET_CorrTopo_sumet_EMB, &b_MET_CorrTopo_sumet_EMB); }
  if( fChain->GetBranchStatus("MET_CorrTopo_phi_EMB") ) { fChain->SetBranchAddress("MET_CorrTopo_phi_EMB", &MET_CorrTopo_phi_EMB, &b_MET_CorrTopo_phi_EMB); }
  if( fChain->GetBranchStatus("MET_CorrTopo_etx_PEMEC") ) { fChain->SetBranchAddress("MET_CorrTopo_etx_PEMEC", &MET_CorrTopo_etx_PEMEC, &b_MET_CorrTopo_etx_PEMEC); }
  if( fChain->GetBranchStatus("MET_CorrTopo_ety_PEMEC") ) { fChain->SetBranchAddress("MET_CorrTopo_ety_PEMEC", &MET_CorrTopo_ety_PEMEC, &b_MET_CorrTopo_ety_PEMEC); }
  if( fChain->GetBranchStatus("MET_CorrTopo_sumet_PEMEC") ) { fChain->SetBranchAddress("MET_CorrTopo_sumet_PEMEC", &MET_CorrTopo_sumet_PEMEC, &b_MET_CorrTopo_sumet_PEMEC); }
  if( fChain->GetBranchStatus("MET_CorrTopo_phi_PEMEC") ) { fChain->SetBranchAddress("MET_CorrTopo_phi_PEMEC", &MET_CorrTopo_phi_PEMEC, &b_MET_CorrTopo_phi_PEMEC); }
  if( fChain->GetBranchStatus("MET_CorrTopo_etx_EME") ) { fChain->SetBranchAddress("MET_CorrTopo_etx_EME", &MET_CorrTopo_etx_EME, &b_MET_CorrTopo_etx_EME); }
  if( fChain->GetBranchStatus("MET_CorrTopo_ety_EME") ) { fChain->SetBranchAddress("MET_CorrTopo_ety_EME", &MET_CorrTopo_ety_EME, &b_MET_CorrTopo_ety_EME); }
  if( fChain->GetBranchStatus("MET_CorrTopo_sumet_EME") ) { fChain->SetBranchAddress("MET_CorrTopo_sumet_EME", &MET_CorrTopo_sumet_EME, &b_MET_CorrTopo_sumet_EME); }
  if( fChain->GetBranchStatus("MET_CorrTopo_phi_EME") ) { fChain->SetBranchAddress("MET_CorrTopo_phi_EME", &MET_CorrTopo_phi_EME, &b_MET_CorrTopo_phi_EME); }
  if( fChain->GetBranchStatus("MET_CorrTopo_etx_TILE") ) { fChain->SetBranchAddress("MET_CorrTopo_etx_TILE", &MET_CorrTopo_etx_TILE, &b_MET_CorrTopo_etx_TILE); }
  if( fChain->GetBranchStatus("MET_CorrTopo_ety_TILE") ) { fChain->SetBranchAddress("MET_CorrTopo_ety_TILE", &MET_CorrTopo_ety_TILE, &b_MET_CorrTopo_ety_TILE); }
  if( fChain->GetBranchStatus("MET_CorrTopo_sumet_TILE") ) { fChain->SetBranchAddress("MET_CorrTopo_sumet_TILE", &MET_CorrTopo_sumet_TILE, &b_MET_CorrTopo_sumet_TILE); }
  if( fChain->GetBranchStatus("MET_CorrTopo_phi_TILE") ) { fChain->SetBranchAddress("MET_CorrTopo_phi_TILE", &MET_CorrTopo_phi_TILE, &b_MET_CorrTopo_phi_TILE); }
  if( fChain->GetBranchStatus("MET_CorrTopo_etx_HEC") ) { fChain->SetBranchAddress("MET_CorrTopo_etx_HEC", &MET_CorrTopo_etx_HEC, &b_MET_CorrTopo_etx_HEC); }
  if( fChain->GetBranchStatus("MET_CorrTopo_ety_HEC") ) { fChain->SetBranchAddress("MET_CorrTopo_ety_HEC", &MET_CorrTopo_ety_HEC, &b_MET_CorrTopo_ety_HEC); }
  if( fChain->GetBranchStatus("MET_CorrTopo_sumet_HEC") ) { fChain->SetBranchAddress("MET_CorrTopo_sumet_HEC", &MET_CorrTopo_sumet_HEC, &b_MET_CorrTopo_sumet_HEC); }
  if( fChain->GetBranchStatus("MET_CorrTopo_phi_HEC") ) { fChain->SetBranchAddress("MET_CorrTopo_phi_HEC", &MET_CorrTopo_phi_HEC, &b_MET_CorrTopo_phi_HEC); }
  if( fChain->GetBranchStatus("MET_CorrTopo_etx_FCAL") ) { fChain->SetBranchAddress("MET_CorrTopo_etx_FCAL", &MET_CorrTopo_etx_FCAL, &b_MET_CorrTopo_etx_FCAL); }
  if( fChain->GetBranchStatus("MET_CorrTopo_ety_FCAL") ) { fChain->SetBranchAddress("MET_CorrTopo_ety_FCAL", &MET_CorrTopo_ety_FCAL, &b_MET_CorrTopo_ety_FCAL); }
  if( fChain->GetBranchStatus("MET_CorrTopo_sumet_FCAL") ) { fChain->SetBranchAddress("MET_CorrTopo_sumet_FCAL", &MET_CorrTopo_sumet_FCAL, &b_MET_CorrTopo_sumet_FCAL); }
  if( fChain->GetBranchStatus("MET_CorrTopo_phi_FCAL") ) { fChain->SetBranchAddress("MET_CorrTopo_phi_FCAL", &MET_CorrTopo_phi_FCAL, &b_MET_CorrTopo_phi_FCAL); }
  if( fChain->GetBranchStatus("MET_CorrTopo_nCell_PEMB") ) { fChain->SetBranchAddress("MET_CorrTopo_nCell_PEMB", &MET_CorrTopo_nCell_PEMB, &b_MET_CorrTopo_nCell_PEMB); }
  if( fChain->GetBranchStatus("MET_CorrTopo_nCell_EMB") ) { fChain->SetBranchAddress("MET_CorrTopo_nCell_EMB", &MET_CorrTopo_nCell_EMB, &b_MET_CorrTopo_nCell_EMB); }
  if( fChain->GetBranchStatus("MET_CorrTopo_nCell_PEMEC") ) { fChain->SetBranchAddress("MET_CorrTopo_nCell_PEMEC", &MET_CorrTopo_nCell_PEMEC, &b_MET_CorrTopo_nCell_PEMEC); }
  if( fChain->GetBranchStatus("MET_CorrTopo_nCell_EME") ) { fChain->SetBranchAddress("MET_CorrTopo_nCell_EME", &MET_CorrTopo_nCell_EME, &b_MET_CorrTopo_nCell_EME); }
  if( fChain->GetBranchStatus("MET_CorrTopo_nCell_TILE") ) { fChain->SetBranchAddress("MET_CorrTopo_nCell_TILE", &MET_CorrTopo_nCell_TILE, &b_MET_CorrTopo_nCell_TILE); }
  if( fChain->GetBranchStatus("MET_CorrTopo_nCell_HEC") ) { fChain->SetBranchAddress("MET_CorrTopo_nCell_HEC", &MET_CorrTopo_nCell_HEC, &b_MET_CorrTopo_nCell_HEC); }
  if( fChain->GetBranchStatus("MET_CorrTopo_nCell_FCAL") ) { fChain->SetBranchAddress("MET_CorrTopo_nCell_FCAL", &MET_CorrTopo_nCell_FCAL, &b_MET_CorrTopo_nCell_FCAL); }
  if( fChain->GetBranchStatus("MET_CorrTopo_etx_CentralReg") ) { fChain->SetBranchAddress("MET_CorrTopo_etx_CentralReg", &MET_CorrTopo_etx_CentralReg, &b_MET_CorrTopo_etx_CentralReg); }
  if( fChain->GetBranchStatus("MET_CorrTopo_ety_CentralReg") ) { fChain->SetBranchAddress("MET_CorrTopo_ety_CentralReg", &MET_CorrTopo_ety_CentralReg, &b_MET_CorrTopo_ety_CentralReg); }
  if( fChain->GetBranchStatus("MET_CorrTopo_sumet_CentralReg") ) { fChain->SetBranchAddress("MET_CorrTopo_sumet_CentralReg", &MET_CorrTopo_sumet_CentralReg, &b_MET_CorrTopo_sumet_CentralReg); }
  if( fChain->GetBranchStatus("MET_CorrTopo_phi_CentralReg") ) { fChain->SetBranchAddress("MET_CorrTopo_phi_CentralReg", &MET_CorrTopo_phi_CentralReg, &b_MET_CorrTopo_phi_CentralReg); }
  if( fChain->GetBranchStatus("MET_CorrTopo_etx_EndcapRegion") ) { fChain->SetBranchAddress("MET_CorrTopo_etx_EndcapRegion", &MET_CorrTopo_etx_EndcapRegion, &b_MET_CorrTopo_etx_EndcapRegion); }
  if( fChain->GetBranchStatus("MET_CorrTopo_ety_EndcapRegion") ) { fChain->SetBranchAddress("MET_CorrTopo_ety_EndcapRegion", &MET_CorrTopo_ety_EndcapRegion, &b_MET_CorrTopo_ety_EndcapRegion); }
  if( fChain->GetBranchStatus("MET_CorrTopo_sumet_EndcapRegion") ) { fChain->SetBranchAddress("MET_CorrTopo_sumet_EndcapRegion", &MET_CorrTopo_sumet_EndcapRegion, &b_MET_CorrTopo_sumet_EndcapRegion); }
  if( fChain->GetBranchStatus("MET_CorrTopo_phi_EndcapRegion") ) { fChain->SetBranchAddress("MET_CorrTopo_phi_EndcapRegion", &MET_CorrTopo_phi_EndcapRegion, &b_MET_CorrTopo_phi_EndcapRegion); }
  if( fChain->GetBranchStatus("MET_CorrTopo_etx_ForwardReg") ) { fChain->SetBranchAddress("MET_CorrTopo_etx_ForwardReg", &MET_CorrTopo_etx_ForwardReg, &b_MET_CorrTopo_etx_ForwardReg); }
  if( fChain->GetBranchStatus("MET_CorrTopo_ety_ForwardReg") ) { fChain->SetBranchAddress("MET_CorrTopo_ety_ForwardReg", &MET_CorrTopo_ety_ForwardReg, &b_MET_CorrTopo_ety_ForwardReg); }
  if( fChain->GetBranchStatus("MET_CorrTopo_sumet_ForwardReg") ) { fChain->SetBranchAddress("MET_CorrTopo_sumet_ForwardReg", &MET_CorrTopo_sumet_ForwardReg, &b_MET_CorrTopo_sumet_ForwardReg); }
  if( fChain->GetBranchStatus("MET_CorrTopo_phi_ForwardReg") ) { fChain->SetBranchAddress("MET_CorrTopo_phi_ForwardReg", &MET_CorrTopo_phi_ForwardReg, &b_MET_CorrTopo_phi_ForwardReg); }
  if( fChain->GetBranchStatus("MET_LocHadTopo_phi") ) { fChain->SetBranchAddress("MET_LocHadTopo_phi", &MET_LocHadTopo_phi, &b_MET_LocHadTopo_phi); }
  if( fChain->GetBranchStatus("MET_LocHadTopo_et") ) { fChain->SetBranchAddress("MET_LocHadTopo_et", &MET_LocHadTopo_et, &b_MET_LocHadTopo_et); }
  if( fChain->GetBranchStatus("MET_LocHadTopo_sumet") ) { fChain->SetBranchAddress("MET_LocHadTopo_sumet", &MET_LocHadTopo_sumet, &b_MET_LocHadTopo_sumet); }
  if( fChain->GetBranchStatus("MET_LocHadTopo_SUMET_EMFrac") ) { fChain->SetBranchAddress("MET_LocHadTopo_SUMET_EMFrac", &MET_LocHadTopo_SUMET_EMFrac, &b_MET_LocHadTopo_SUMET_EMFrac); }
  if( fChain->GetBranchStatus("MET_LocHadTopo_etx_PEMB") ) { fChain->SetBranchAddress("MET_LocHadTopo_etx_PEMB", &MET_LocHadTopo_etx_PEMB, &b_MET_LocHadTopo_etx_PEMB); }
  if( fChain->GetBranchStatus("MET_LocHadTopo_ety_PEMB") ) { fChain->SetBranchAddress("MET_LocHadTopo_ety_PEMB", &MET_LocHadTopo_ety_PEMB, &b_MET_LocHadTopo_ety_PEMB); }
  if( fChain->GetBranchStatus("MET_LocHadTopo_sumet_PEMB") ) { fChain->SetBranchAddress("MET_LocHadTopo_sumet_PEMB", &MET_LocHadTopo_sumet_PEMB, &b_MET_LocHadTopo_sumet_PEMB); }
  if( fChain->GetBranchStatus("MET_LocHadTopo_phi_PEMB") ) { fChain->SetBranchAddress("MET_LocHadTopo_phi_PEMB", &MET_LocHadTopo_phi_PEMB, &b_MET_LocHadTopo_phi_PEMB); }
  if( fChain->GetBranchStatus("MET_LocHadTopo_etx_EMB") ) { fChain->SetBranchAddress("MET_LocHadTopo_etx_EMB", &MET_LocHadTopo_etx_EMB, &b_MET_LocHadTopo_etx_EMB); }
  if( fChain->GetBranchStatus("MET_LocHadTopo_ety_EMB") ) { fChain->SetBranchAddress("MET_LocHadTopo_ety_EMB", &MET_LocHadTopo_ety_EMB, &b_MET_LocHadTopo_ety_EMB); }
  if( fChain->GetBranchStatus("MET_LocHadTopo_sumet_EMB") ) { fChain->SetBranchAddress("MET_LocHadTopo_sumet_EMB", &MET_LocHadTopo_sumet_EMB, &b_MET_LocHadTopo_sumet_EMB); }
  if( fChain->GetBranchStatus("MET_LocHadTopo_phi_EMB") ) { fChain->SetBranchAddress("MET_LocHadTopo_phi_EMB", &MET_LocHadTopo_phi_EMB, &b_MET_LocHadTopo_phi_EMB); }
  if( fChain->GetBranchStatus("MET_LocHadTopo_etx_PEMEC") ) { fChain->SetBranchAddress("MET_LocHadTopo_etx_PEMEC", &MET_LocHadTopo_etx_PEMEC, &b_MET_LocHadTopo_etx_PEMEC); }
  if( fChain->GetBranchStatus("MET_LocHadTopo_ety_PEMEC") ) { fChain->SetBranchAddress("MET_LocHadTopo_ety_PEMEC", &MET_LocHadTopo_ety_PEMEC, &b_MET_LocHadTopo_ety_PEMEC); }
  if( fChain->GetBranchStatus("MET_LocHadTopo_sumet_PEMEC") ) { fChain->SetBranchAddress("MET_LocHadTopo_sumet_PEMEC", &MET_LocHadTopo_sumet_PEMEC, &b_MET_LocHadTopo_sumet_PEMEC); }
  if( fChain->GetBranchStatus("MET_LocHadTopo_phi_PEMEC") ) { fChain->SetBranchAddress("MET_LocHadTopo_phi_PEMEC", &MET_LocHadTopo_phi_PEMEC, &b_MET_LocHadTopo_phi_PEMEC); }
  if( fChain->GetBranchStatus("MET_LocHadTopo_etx_EME") ) { fChain->SetBranchAddress("MET_LocHadTopo_etx_EME", &MET_LocHadTopo_etx_EME, &b_MET_LocHadTopo_etx_EME); }
  if( fChain->GetBranchStatus("MET_LocHadTopo_ety_EME") ) { fChain->SetBranchAddress("MET_LocHadTopo_ety_EME", &MET_LocHadTopo_ety_EME, &b_MET_LocHadTopo_ety_EME); }
  if( fChain->GetBranchStatus("MET_LocHadTopo_sumet_EME") ) { fChain->SetBranchAddress("MET_LocHadTopo_sumet_EME", &MET_LocHadTopo_sumet_EME, &b_MET_LocHadTopo_sumet_EME); }
  if( fChain->GetBranchStatus("MET_LocHadTopo_phi_EME") ) { fChain->SetBranchAddress("MET_LocHadTopo_phi_EME", &MET_LocHadTopo_phi_EME, &b_MET_LocHadTopo_phi_EME); }
  if( fChain->GetBranchStatus("MET_LocHadTopo_etx_TILE") ) { fChain->SetBranchAddress("MET_LocHadTopo_etx_TILE", &MET_LocHadTopo_etx_TILE, &b_MET_LocHadTopo_etx_TILE); }
  if( fChain->GetBranchStatus("MET_LocHadTopo_ety_TILE") ) { fChain->SetBranchAddress("MET_LocHadTopo_ety_TILE", &MET_LocHadTopo_ety_TILE, &b_MET_LocHadTopo_ety_TILE); }
  if( fChain->GetBranchStatus("MET_LocHadTopo_sumet_TILE") ) { fChain->SetBranchAddress("MET_LocHadTopo_sumet_TILE", &MET_LocHadTopo_sumet_TILE, &b_MET_LocHadTopo_sumet_TILE); }
  if( fChain->GetBranchStatus("MET_LocHadTopo_phi_TILE") ) { fChain->SetBranchAddress("MET_LocHadTopo_phi_TILE", &MET_LocHadTopo_phi_TILE, &b_MET_LocHadTopo_phi_TILE); }
  if( fChain->GetBranchStatus("MET_LocHadTopo_etx_HEC") ) { fChain->SetBranchAddress("MET_LocHadTopo_etx_HEC", &MET_LocHadTopo_etx_HEC, &b_MET_LocHadTopo_etx_HEC); }
  if( fChain->GetBranchStatus("MET_LocHadTopo_ety_HEC") ) { fChain->SetBranchAddress("MET_LocHadTopo_ety_HEC", &MET_LocHadTopo_ety_HEC, &b_MET_LocHadTopo_ety_HEC); }
  if( fChain->GetBranchStatus("MET_LocHadTopo_sumet_HEC") ) { fChain->SetBranchAddress("MET_LocHadTopo_sumet_HEC", &MET_LocHadTopo_sumet_HEC, &b_MET_LocHadTopo_sumet_HEC); }
  if( fChain->GetBranchStatus("MET_LocHadTopo_phi_HEC") ) { fChain->SetBranchAddress("MET_LocHadTopo_phi_HEC", &MET_LocHadTopo_phi_HEC, &b_MET_LocHadTopo_phi_HEC); }
  if( fChain->GetBranchStatus("MET_LocHadTopo_etx_FCAL") ) { fChain->SetBranchAddress("MET_LocHadTopo_etx_FCAL", &MET_LocHadTopo_etx_FCAL, &b_MET_LocHadTopo_etx_FCAL); }
  if( fChain->GetBranchStatus("MET_LocHadTopo_ety_FCAL") ) { fChain->SetBranchAddress("MET_LocHadTopo_ety_FCAL", &MET_LocHadTopo_ety_FCAL, &b_MET_LocHadTopo_ety_FCAL); }
  if( fChain->GetBranchStatus("MET_LocHadTopo_sumet_FCAL") ) { fChain->SetBranchAddress("MET_LocHadTopo_sumet_FCAL", &MET_LocHadTopo_sumet_FCAL, &b_MET_LocHadTopo_sumet_FCAL); }
  if( fChain->GetBranchStatus("MET_LocHadTopo_phi_FCAL") ) { fChain->SetBranchAddress("MET_LocHadTopo_phi_FCAL", &MET_LocHadTopo_phi_FCAL, &b_MET_LocHadTopo_phi_FCAL); }
  if( fChain->GetBranchStatus("MET_LocHadTopo_nCell_PEMB") ) { fChain->SetBranchAddress("MET_LocHadTopo_nCell_PEMB", &MET_LocHadTopo_nCell_PEMB, &b_MET_LocHadTopo_nCell_PEMB); }
  if( fChain->GetBranchStatus("MET_LocHadTopo_nCell_EMB") ) { fChain->SetBranchAddress("MET_LocHadTopo_nCell_EMB", &MET_LocHadTopo_nCell_EMB, &b_MET_LocHadTopo_nCell_EMB); }
  if( fChain->GetBranchStatus("MET_LocHadTopo_nCell_PEMEC") ) { fChain->SetBranchAddress("MET_LocHadTopo_nCell_PEMEC", &MET_LocHadTopo_nCell_PEMEC, &b_MET_LocHadTopo_nCell_PEMEC); }
  if( fChain->GetBranchStatus("MET_LocHadTopo_nCell_EME") ) { fChain->SetBranchAddress("MET_LocHadTopo_nCell_EME", &MET_LocHadTopo_nCell_EME, &b_MET_LocHadTopo_nCell_EME); }
  if( fChain->GetBranchStatus("MET_LocHadTopo_nCell_TILE") ) { fChain->SetBranchAddress("MET_LocHadTopo_nCell_TILE", &MET_LocHadTopo_nCell_TILE, &b_MET_LocHadTopo_nCell_TILE); }
  if( fChain->GetBranchStatus("MET_LocHadTopo_nCell_HEC") ) { fChain->SetBranchAddress("MET_LocHadTopo_nCell_HEC", &MET_LocHadTopo_nCell_HEC, &b_MET_LocHadTopo_nCell_HEC); }
  if( fChain->GetBranchStatus("MET_LocHadTopo_nCell_FCAL") ) { fChain->SetBranchAddress("MET_LocHadTopo_nCell_FCAL", &MET_LocHadTopo_nCell_FCAL, &b_MET_LocHadTopo_nCell_FCAL); }
  if( fChain->GetBranchStatus("MET_LocHadTopo_etx_CentralReg") ) { fChain->SetBranchAddress("MET_LocHadTopo_etx_CentralReg", &MET_LocHadTopo_etx_CentralReg, &b_MET_LocHadTopo_etx_CentralReg); }
  if( fChain->GetBranchStatus("MET_LocHadTopo_ety_CentralReg") ) { fChain->SetBranchAddress("MET_LocHadTopo_ety_CentralReg", &MET_LocHadTopo_ety_CentralReg, &b_MET_LocHadTopo_ety_CentralReg); }
  if( fChain->GetBranchStatus("MET_LocHadTopo_sumet_CentralReg") ) { fChain->SetBranchAddress("MET_LocHadTopo_sumet_CentralReg", &MET_LocHadTopo_sumet_CentralReg, &b_MET_LocHadTopo_sumet_CentralReg); }
  if( fChain->GetBranchStatus("MET_LocHadTopo_phi_CentralReg") ) { fChain->SetBranchAddress("MET_LocHadTopo_phi_CentralReg", &MET_LocHadTopo_phi_CentralReg, &b_MET_LocHadTopo_phi_CentralReg); }
  if( fChain->GetBranchStatus("MET_LocHadTopo_etx_EndcapRegion") ) { fChain->SetBranchAddress("MET_LocHadTopo_etx_EndcapRegion", &MET_LocHadTopo_etx_EndcapRegion, &b_MET_LocHadTopo_etx_EndcapRegion); }
  if( fChain->GetBranchStatus("MET_LocHadTopo_ety_EndcapRegion") ) { fChain->SetBranchAddress("MET_LocHadTopo_ety_EndcapRegion", &MET_LocHadTopo_ety_EndcapRegion, &b_MET_LocHadTopo_ety_EndcapRegion); }
  if( fChain->GetBranchStatus("MET_LocHadTopo_sumet_EndcapRegion") ) { fChain->SetBranchAddress("MET_LocHadTopo_sumet_EndcapRegion", &MET_LocHadTopo_sumet_EndcapRegion, &b_MET_LocHadTopo_sumet_EndcapRegion); }
  if( fChain->GetBranchStatus("MET_LocHadTopo_phi_EndcapRegion") ) { fChain->SetBranchAddress("MET_LocHadTopo_phi_EndcapRegion", &MET_LocHadTopo_phi_EndcapRegion, &b_MET_LocHadTopo_phi_EndcapRegion); }
  if( fChain->GetBranchStatus("MET_LocHadTopo_etx_ForwardReg") ) { fChain->SetBranchAddress("MET_LocHadTopo_etx_ForwardReg", &MET_LocHadTopo_etx_ForwardReg, &b_MET_LocHadTopo_etx_ForwardReg); }
  if( fChain->GetBranchStatus("MET_LocHadTopo_ety_ForwardReg") ) { fChain->SetBranchAddress("MET_LocHadTopo_ety_ForwardReg", &MET_LocHadTopo_ety_ForwardReg, &b_MET_LocHadTopo_ety_ForwardReg); }
  if( fChain->GetBranchStatus("MET_LocHadTopo_sumet_ForwardReg") ) { fChain->SetBranchAddress("MET_LocHadTopo_sumet_ForwardReg", &MET_LocHadTopo_sumet_ForwardReg, &b_MET_LocHadTopo_sumet_ForwardReg); }
  if( fChain->GetBranchStatus("MET_LocHadTopo_phi_ForwardReg") ) { fChain->SetBranchAddress("MET_LocHadTopo_phi_ForwardReg", &MET_LocHadTopo_phi_ForwardReg, &b_MET_LocHadTopo_phi_ForwardReg); }
  if( fChain->GetBranchStatus("MET_Calib_phi") ) { fChain->SetBranchAddress("MET_Calib_phi", &MET_Calib_phi, &b_MET_Calib_phi); }
  if( fChain->GetBranchStatus("MET_Calib_et") ) { fChain->SetBranchAddress("MET_Calib_et", &MET_Calib_et, &b_MET_Calib_et); }
  if( fChain->GetBranchStatus("MET_Calib_sumet") ) { fChain->SetBranchAddress("MET_Calib_sumet", &MET_Calib_sumet, &b_MET_Calib_sumet); }
  if( fChain->GetBranchStatus("MET_Calib_SUMET_EMFrac") ) { fChain->SetBranchAddress("MET_Calib_SUMET_EMFrac", &MET_Calib_SUMET_EMFrac, &b_MET_Calib_SUMET_EMFrac); }
  if( fChain->GetBranchStatus("MET_Calib_etx_PEMB") ) { fChain->SetBranchAddress("MET_Calib_etx_PEMB", &MET_Calib_etx_PEMB, &b_MET_Calib_etx_PEMB); }
  if( fChain->GetBranchStatus("MET_Calib_ety_PEMB") ) { fChain->SetBranchAddress("MET_Calib_ety_PEMB", &MET_Calib_ety_PEMB, &b_MET_Calib_ety_PEMB); }
  if( fChain->GetBranchStatus("MET_Calib_sumet_PEMB") ) { fChain->SetBranchAddress("MET_Calib_sumet_PEMB", &MET_Calib_sumet_PEMB, &b_MET_Calib_sumet_PEMB); }
  if( fChain->GetBranchStatus("MET_Calib_phi_PEMB") ) { fChain->SetBranchAddress("MET_Calib_phi_PEMB", &MET_Calib_phi_PEMB, &b_MET_Calib_phi_PEMB); }
  if( fChain->GetBranchStatus("MET_Calib_etx_EMB") ) { fChain->SetBranchAddress("MET_Calib_etx_EMB", &MET_Calib_etx_EMB, &b_MET_Calib_etx_EMB); }
  if( fChain->GetBranchStatus("MET_Calib_ety_EMB") ) { fChain->SetBranchAddress("MET_Calib_ety_EMB", &MET_Calib_ety_EMB, &b_MET_Calib_ety_EMB); }
  if( fChain->GetBranchStatus("MET_Calib_sumet_EMB") ) { fChain->SetBranchAddress("MET_Calib_sumet_EMB", &MET_Calib_sumet_EMB, &b_MET_Calib_sumet_EMB); }
  if( fChain->GetBranchStatus("MET_Calib_phi_EMB") ) { fChain->SetBranchAddress("MET_Calib_phi_EMB", &MET_Calib_phi_EMB, &b_MET_Calib_phi_EMB); }
  if( fChain->GetBranchStatus("MET_Calib_etx_PEMEC") ) { fChain->SetBranchAddress("MET_Calib_etx_PEMEC", &MET_Calib_etx_PEMEC, &b_MET_Calib_etx_PEMEC); }
  if( fChain->GetBranchStatus("MET_Calib_ety_PEMEC") ) { fChain->SetBranchAddress("MET_Calib_ety_PEMEC", &MET_Calib_ety_PEMEC, &b_MET_Calib_ety_PEMEC); }
  if( fChain->GetBranchStatus("MET_Calib_sumet_PEMEC") ) { fChain->SetBranchAddress("MET_Calib_sumet_PEMEC", &MET_Calib_sumet_PEMEC, &b_MET_Calib_sumet_PEMEC); }
  if( fChain->GetBranchStatus("MET_Calib_phi_PEMEC") ) { fChain->SetBranchAddress("MET_Calib_phi_PEMEC", &MET_Calib_phi_PEMEC, &b_MET_Calib_phi_PEMEC); }
  if( fChain->GetBranchStatus("MET_Calib_etx_EME") ) { fChain->SetBranchAddress("MET_Calib_etx_EME", &MET_Calib_etx_EME, &b_MET_Calib_etx_EME); }
  if( fChain->GetBranchStatus("MET_Calib_ety_EME") ) { fChain->SetBranchAddress("MET_Calib_ety_EME", &MET_Calib_ety_EME, &b_MET_Calib_ety_EME); }
  if( fChain->GetBranchStatus("MET_Calib_sumet_EME") ) { fChain->SetBranchAddress("MET_Calib_sumet_EME", &MET_Calib_sumet_EME, &b_MET_Calib_sumet_EME); }
  if( fChain->GetBranchStatus("MET_Calib_phi_EME") ) { fChain->SetBranchAddress("MET_Calib_phi_EME", &MET_Calib_phi_EME, &b_MET_Calib_phi_EME); }
  if( fChain->GetBranchStatus("MET_Calib_etx_TILE") ) { fChain->SetBranchAddress("MET_Calib_etx_TILE", &MET_Calib_etx_TILE, &b_MET_Calib_etx_TILE); }
  if( fChain->GetBranchStatus("MET_Calib_ety_TILE") ) { fChain->SetBranchAddress("MET_Calib_ety_TILE", &MET_Calib_ety_TILE, &b_MET_Calib_ety_TILE); }
  if( fChain->GetBranchStatus("MET_Calib_sumet_TILE") ) { fChain->SetBranchAddress("MET_Calib_sumet_TILE", &MET_Calib_sumet_TILE, &b_MET_Calib_sumet_TILE); }
  if( fChain->GetBranchStatus("MET_Calib_phi_TILE") ) { fChain->SetBranchAddress("MET_Calib_phi_TILE", &MET_Calib_phi_TILE, &b_MET_Calib_phi_TILE); }
  if( fChain->GetBranchStatus("MET_Calib_etx_HEC") ) { fChain->SetBranchAddress("MET_Calib_etx_HEC", &MET_Calib_etx_HEC, &b_MET_Calib_etx_HEC); }
  if( fChain->GetBranchStatus("MET_Calib_ety_HEC") ) { fChain->SetBranchAddress("MET_Calib_ety_HEC", &MET_Calib_ety_HEC, &b_MET_Calib_ety_HEC); }
  if( fChain->GetBranchStatus("MET_Calib_sumet_HEC") ) { fChain->SetBranchAddress("MET_Calib_sumet_HEC", &MET_Calib_sumet_HEC, &b_MET_Calib_sumet_HEC); }
  if( fChain->GetBranchStatus("MET_Calib_phi_HEC") ) { fChain->SetBranchAddress("MET_Calib_phi_HEC", &MET_Calib_phi_HEC, &b_MET_Calib_phi_HEC); }
  if( fChain->GetBranchStatus("MET_Calib_etx_FCAL") ) { fChain->SetBranchAddress("MET_Calib_etx_FCAL", &MET_Calib_etx_FCAL, &b_MET_Calib_etx_FCAL); }
  if( fChain->GetBranchStatus("MET_Calib_ety_FCAL") ) { fChain->SetBranchAddress("MET_Calib_ety_FCAL", &MET_Calib_ety_FCAL, &b_MET_Calib_ety_FCAL); }
  if( fChain->GetBranchStatus("MET_Calib_sumet_FCAL") ) { fChain->SetBranchAddress("MET_Calib_sumet_FCAL", &MET_Calib_sumet_FCAL, &b_MET_Calib_sumet_FCAL); }
  if( fChain->GetBranchStatus("MET_Calib_phi_FCAL") ) { fChain->SetBranchAddress("MET_Calib_phi_FCAL", &MET_Calib_phi_FCAL, &b_MET_Calib_phi_FCAL); }
  if( fChain->GetBranchStatus("MET_Calib_nCell_PEMB") ) { fChain->SetBranchAddress("MET_Calib_nCell_PEMB", &MET_Calib_nCell_PEMB, &b_MET_Calib_nCell_PEMB); }
  if( fChain->GetBranchStatus("MET_Calib_nCell_EMB") ) { fChain->SetBranchAddress("MET_Calib_nCell_EMB", &MET_Calib_nCell_EMB, &b_MET_Calib_nCell_EMB); }
  if( fChain->GetBranchStatus("MET_Calib_nCell_PEMEC") ) { fChain->SetBranchAddress("MET_Calib_nCell_PEMEC", &MET_Calib_nCell_PEMEC, &b_MET_Calib_nCell_PEMEC); }
  if( fChain->GetBranchStatus("MET_Calib_nCell_EME") ) { fChain->SetBranchAddress("MET_Calib_nCell_EME", &MET_Calib_nCell_EME, &b_MET_Calib_nCell_EME); }
  if( fChain->GetBranchStatus("MET_Calib_nCell_TILE") ) { fChain->SetBranchAddress("MET_Calib_nCell_TILE", &MET_Calib_nCell_TILE, &b_MET_Calib_nCell_TILE); }
  if( fChain->GetBranchStatus("MET_Calib_nCell_HEC") ) { fChain->SetBranchAddress("MET_Calib_nCell_HEC", &MET_Calib_nCell_HEC, &b_MET_Calib_nCell_HEC); }
  if( fChain->GetBranchStatus("MET_Calib_nCell_FCAL") ) { fChain->SetBranchAddress("MET_Calib_nCell_FCAL", &MET_Calib_nCell_FCAL, &b_MET_Calib_nCell_FCAL); }
  if( fChain->GetBranchStatus("MET_Calib_etx_CentralReg") ) { fChain->SetBranchAddress("MET_Calib_etx_CentralReg", &MET_Calib_etx_CentralReg, &b_MET_Calib_etx_CentralReg); }
  if( fChain->GetBranchStatus("MET_Calib_ety_CentralReg") ) { fChain->SetBranchAddress("MET_Calib_ety_CentralReg", &MET_Calib_ety_CentralReg, &b_MET_Calib_ety_CentralReg); }
  if( fChain->GetBranchStatus("MET_Calib_sumet_CentralReg") ) { fChain->SetBranchAddress("MET_Calib_sumet_CentralReg", &MET_Calib_sumet_CentralReg, &b_MET_Calib_sumet_CentralReg); }
  if( fChain->GetBranchStatus("MET_Calib_phi_CentralReg") ) { fChain->SetBranchAddress("MET_Calib_phi_CentralReg", &MET_Calib_phi_CentralReg, &b_MET_Calib_phi_CentralReg); }
  if( fChain->GetBranchStatus("MET_Calib_etx_EndcapRegion") ) { fChain->SetBranchAddress("MET_Calib_etx_EndcapRegion", &MET_Calib_etx_EndcapRegion, &b_MET_Calib_etx_EndcapRegion); }
  if( fChain->GetBranchStatus("MET_Calib_ety_EndcapRegion") ) { fChain->SetBranchAddress("MET_Calib_ety_EndcapRegion", &MET_Calib_ety_EndcapRegion, &b_MET_Calib_ety_EndcapRegion); }
  if( fChain->GetBranchStatus("MET_Calib_sumet_EndcapRegion") ) { fChain->SetBranchAddress("MET_Calib_sumet_EndcapRegion", &MET_Calib_sumet_EndcapRegion, &b_MET_Calib_sumet_EndcapRegion); }
  if( fChain->GetBranchStatus("MET_Calib_phi_EndcapRegion") ) { fChain->SetBranchAddress("MET_Calib_phi_EndcapRegion", &MET_Calib_phi_EndcapRegion, &b_MET_Calib_phi_EndcapRegion); }
  if( fChain->GetBranchStatus("MET_Calib_etx_ForwardReg") ) { fChain->SetBranchAddress("MET_Calib_etx_ForwardReg", &MET_Calib_etx_ForwardReg, &b_MET_Calib_etx_ForwardReg); }
  if( fChain->GetBranchStatus("MET_Calib_ety_ForwardReg") ) { fChain->SetBranchAddress("MET_Calib_ety_ForwardReg", &MET_Calib_ety_ForwardReg, &b_MET_Calib_ety_ForwardReg); }
  if( fChain->GetBranchStatus("MET_Calib_sumet_ForwardReg") ) { fChain->SetBranchAddress("MET_Calib_sumet_ForwardReg", &MET_Calib_sumet_ForwardReg, &b_MET_Calib_sumet_ForwardReg); }
  if( fChain->GetBranchStatus("MET_Calib_phi_ForwardReg") ) { fChain->SetBranchAddress("MET_Calib_phi_ForwardReg", &MET_Calib_phi_ForwardReg, &b_MET_Calib_phi_ForwardReg); }
  if( fChain->GetBranchStatus("MET_Truth_NonInt_etx") ) { fChain->SetBranchAddress("MET_Truth_NonInt_etx", &MET_Truth_NonInt_etx, &b_MET_Truth_NonInt_etx); }
  if( fChain->GetBranchStatus("MET_Truth_NonInt_ety") ) { fChain->SetBranchAddress("MET_Truth_NonInt_ety", &MET_Truth_NonInt_ety, &b_MET_Truth_NonInt_ety); }
  if( fChain->GetBranchStatus("MET_Truth_NonInt_phi") ) { fChain->SetBranchAddress("MET_Truth_NonInt_phi", &MET_Truth_NonInt_phi, &b_MET_Truth_NonInt_phi); }
  if( fChain->GetBranchStatus("MET_Truth_NonInt_et") ) { fChain->SetBranchAddress("MET_Truth_NonInt_et", &MET_Truth_NonInt_et, &b_MET_Truth_NonInt_et); }
  if( fChain->GetBranchStatus("MET_Truth_NonInt_sumet") ) { fChain->SetBranchAddress("MET_Truth_NonInt_sumet", &MET_Truth_NonInt_sumet, &b_MET_Truth_NonInt_sumet); }
  if( fChain->GetBranchStatus("MET_Truth_Int_phi") ) { fChain->SetBranchAddress("MET_Truth_Int_phi", &MET_Truth_Int_phi, &b_MET_Truth_Int_phi); }
  if( fChain->GetBranchStatus("MET_Truth_IntCentral_phi") ) { fChain->SetBranchAddress("MET_Truth_IntCentral_phi", &MET_Truth_IntCentral_phi, &b_MET_Truth_IntCentral_phi); }
  if( fChain->GetBranchStatus("MET_Truth_IntFwd_phi") ) { fChain->SetBranchAddress("MET_Truth_IntFwd_phi", &MET_Truth_IntFwd_phi, &b_MET_Truth_IntFwd_phi); }
  if( fChain->GetBranchStatus("MET_Truth_IntOutCover_phi") ) { fChain->SetBranchAddress("MET_Truth_IntOutCover_phi", &MET_Truth_IntOutCover_phi, &b_MET_Truth_IntOutCover_phi); }
  if( fChain->GetBranchStatus("MET_Truth_IntMuons_phi") ) { fChain->SetBranchAddress("MET_Truth_IntMuons_phi", &MET_Truth_IntMuons_phi, &b_MET_Truth_IntMuons_phi); }
  if( fChain->GetBranchStatus("MET_Truth_Int_et") ) { fChain->SetBranchAddress("MET_Truth_Int_et", &MET_Truth_Int_et, &b_MET_Truth_Int_et); }
  if( fChain->GetBranchStatus("MET_Truth_IntCentral_et") ) { fChain->SetBranchAddress("MET_Truth_IntCentral_et", &MET_Truth_IntCentral_et, &b_MET_Truth_IntCentral_et); }
  if( fChain->GetBranchStatus("MET_Truth_IntFwd_et") ) { fChain->SetBranchAddress("MET_Truth_IntFwd_et", &MET_Truth_IntFwd_et, &b_MET_Truth_IntFwd_et); }
  if( fChain->GetBranchStatus("MET_Truth_IntOutCover_et") ) { fChain->SetBranchAddress("MET_Truth_IntOutCover_et", &MET_Truth_IntOutCover_et, &b_MET_Truth_IntOutCover_et); }
  if( fChain->GetBranchStatus("MET_Truth_IntMuons_et") ) { fChain->SetBranchAddress("MET_Truth_IntMuons_et", &MET_Truth_IntMuons_et, &b_MET_Truth_IntMuons_et); }
  if( fChain->GetBranchStatus("MET_Truth_Int_sumet") ) { fChain->SetBranchAddress("MET_Truth_Int_sumet", &MET_Truth_Int_sumet, &b_MET_Truth_Int_sumet); }
  if( fChain->GetBranchStatus("MET_Truth_IntCentral_sumet") ) { fChain->SetBranchAddress("MET_Truth_IntCentral_sumet", &MET_Truth_IntCentral_sumet, &b_MET_Truth_IntCentral_sumet); }
  if( fChain->GetBranchStatus("MET_Truth_IntFwd_sumet") ) { fChain->SetBranchAddress("MET_Truth_IntFwd_sumet", &MET_Truth_IntFwd_sumet, &b_MET_Truth_IntFwd_sumet); }
  if( fChain->GetBranchStatus("MET_Truth_IntOutCover_sumet") ) { fChain->SetBranchAddress("MET_Truth_IntOutCover_sumet", &MET_Truth_IntOutCover_sumet, &b_MET_Truth_IntOutCover_sumet); }
  if( fChain->GetBranchStatus("MET_Truth_IntMuons_sumet") ) { fChain->SetBranchAddress("MET_Truth_IntMuons_sumet", &MET_Truth_IntMuons_sumet, &b_MET_Truth_IntMuons_sumet); }
  if( fChain->GetBranchStatus("MET_Truth_PileUp_NonInt_etx") ) { fChain->SetBranchAddress("MET_Truth_PileUp_NonInt_etx", &MET_Truth_PileUp_NonInt_etx, &b_MET_Truth_PileUp_NonInt_etx); }
  if( fChain->GetBranchStatus("MET_Truth_PileUp_NonInt_ety") ) { fChain->SetBranchAddress("MET_Truth_PileUp_NonInt_ety", &MET_Truth_PileUp_NonInt_ety, &b_MET_Truth_PileUp_NonInt_ety); }
  if( fChain->GetBranchStatus("MET_Truth_PileUp_NonInt_phi") ) { fChain->SetBranchAddress("MET_Truth_PileUp_NonInt_phi", &MET_Truth_PileUp_NonInt_phi, &b_MET_Truth_PileUp_NonInt_phi); }
  if( fChain->GetBranchStatus("MET_Truth_PileUp_NonInt_et") ) { fChain->SetBranchAddress("MET_Truth_PileUp_NonInt_et", &MET_Truth_PileUp_NonInt_et, &b_MET_Truth_PileUp_NonInt_et); }
  if( fChain->GetBranchStatus("MET_Truth_PileUp_NonInt_sumet") ) { fChain->SetBranchAddress("MET_Truth_PileUp_NonInt_sumet", &MET_Truth_PileUp_NonInt_sumet, &b_MET_Truth_PileUp_NonInt_sumet); }
  if( fChain->GetBranchStatus("MET_Truth_PileUp_Int_etx") ) { fChain->SetBranchAddress("MET_Truth_PileUp_Int_etx", &MET_Truth_PileUp_Int_etx, &b_MET_Truth_PileUp_Int_etx); }
  if( fChain->GetBranchStatus("MET_Truth_PileUp_Int_ety") ) { fChain->SetBranchAddress("MET_Truth_PileUp_Int_ety", &MET_Truth_PileUp_Int_ety, &b_MET_Truth_PileUp_Int_ety); }
  if( fChain->GetBranchStatus("MET_Truth_PileUp_IntCentral_etx") ) { fChain->SetBranchAddress("MET_Truth_PileUp_IntCentral_etx", &MET_Truth_PileUp_IntCentral_etx, &b_MET_Truth_PileUp_IntCentral_etx); }
  if( fChain->GetBranchStatus("MET_Truth_PileUp_IntCentral_ety") ) { fChain->SetBranchAddress("MET_Truth_PileUp_IntCentral_ety", &MET_Truth_PileUp_IntCentral_ety, &b_MET_Truth_PileUp_IntCentral_ety); }
  if( fChain->GetBranchStatus("MET_Truth_PileUp_IntFwd_etx") ) { fChain->SetBranchAddress("MET_Truth_PileUp_IntFwd_etx", &MET_Truth_PileUp_IntFwd_etx, &b_MET_Truth_PileUp_IntFwd_etx); }
  if( fChain->GetBranchStatus("MET_Truth_PileUp_IntFwd_ety") ) { fChain->SetBranchAddress("MET_Truth_PileUp_IntFwd_ety", &MET_Truth_PileUp_IntFwd_ety, &b_MET_Truth_PileUp_IntFwd_ety); }
  if( fChain->GetBranchStatus("MET_Truth_PileUp_IntOutCover_etx") ) { fChain->SetBranchAddress("MET_Truth_PileUp_IntOutCover_etx", &MET_Truth_PileUp_IntOutCover_etx, &b_MET_Truth_PileUp_IntOutCover_etx); }
  if( fChain->GetBranchStatus("MET_Truth_PileUp_IntOutCover_ety") ) { fChain->SetBranchAddress("MET_Truth_PileUp_IntOutCover_ety", &MET_Truth_PileUp_IntOutCover_ety, &b_MET_Truth_PileUp_IntOutCover_ety); }
  if( fChain->GetBranchStatus("MET_Truth_PileUp_IntMuons_etx") ) { fChain->SetBranchAddress("MET_Truth_PileUp_IntMuons_etx", &MET_Truth_PileUp_IntMuons_etx, &b_MET_Truth_PileUp_IntMuons_etx); }
  if( fChain->GetBranchStatus("MET_Truth_PileUp_IntMuons_ety") ) { fChain->SetBranchAddress("MET_Truth_PileUp_IntMuons_ety", &MET_Truth_PileUp_IntMuons_ety, &b_MET_Truth_PileUp_IntMuons_ety); }
  if( fChain->GetBranchStatus("MET_Truth_PileUp_Int_phi") ) { fChain->SetBranchAddress("MET_Truth_PileUp_Int_phi", &MET_Truth_PileUp_Int_phi, &b_MET_Truth_PileUp_Int_phi); }
  if( fChain->GetBranchStatus("MET_Truth_PileUp_IntCentral_phi") ) { fChain->SetBranchAddress("MET_Truth_PileUp_IntCentral_phi", &MET_Truth_PileUp_IntCentral_phi, &b_MET_Truth_PileUp_IntCentral_phi); }
  if( fChain->GetBranchStatus("MET_Truth_PileUp_IntFwd_phi") ) { fChain->SetBranchAddress("MET_Truth_PileUp_IntFwd_phi", &MET_Truth_PileUp_IntFwd_phi, &b_MET_Truth_PileUp_IntFwd_phi); }
  if( fChain->GetBranchStatus("MET_Truth_PileUp_IntOutCover_phi") ) { fChain->SetBranchAddress("MET_Truth_PileUp_IntOutCover_phi", &MET_Truth_PileUp_IntOutCover_phi, &b_MET_Truth_PileUp_IntOutCover_phi); }
  if( fChain->GetBranchStatus("MET_Truth_PileUp_IntMuons_phi") ) { fChain->SetBranchAddress("MET_Truth_PileUp_IntMuons_phi", &MET_Truth_PileUp_IntMuons_phi, &b_MET_Truth_PileUp_IntMuons_phi); }
  if( fChain->GetBranchStatus("MET_Truth_PileUp_Int_et") ) { fChain->SetBranchAddress("MET_Truth_PileUp_Int_et", &MET_Truth_PileUp_Int_et, &b_MET_Truth_PileUp_Int_et); }
  if( fChain->GetBranchStatus("MET_Truth_PileUp_IntCentral_et") ) { fChain->SetBranchAddress("MET_Truth_PileUp_IntCentral_et", &MET_Truth_PileUp_IntCentral_et, &b_MET_Truth_PileUp_IntCentral_et); }
  if( fChain->GetBranchStatus("MET_Truth_PileUp_IntFwd_et") ) { fChain->SetBranchAddress("MET_Truth_PileUp_IntFwd_et", &MET_Truth_PileUp_IntFwd_et, &b_MET_Truth_PileUp_IntFwd_et); }
  if( fChain->GetBranchStatus("MET_Truth_PileUp_IntOutCover_et") ) { fChain->SetBranchAddress("MET_Truth_PileUp_IntOutCover_et", &MET_Truth_PileUp_IntOutCover_et, &b_MET_Truth_PileUp_IntOutCover_et); }
  if( fChain->GetBranchStatus("MET_Truth_PileUp_IntMuons_et") ) { fChain->SetBranchAddress("MET_Truth_PileUp_IntMuons_et", &MET_Truth_PileUp_IntMuons_et, &b_MET_Truth_PileUp_IntMuons_et); }
  if( fChain->GetBranchStatus("MET_Truth_PileUp_Int_sumet") ) { fChain->SetBranchAddress("MET_Truth_PileUp_Int_sumet", &MET_Truth_PileUp_Int_sumet, &b_MET_Truth_PileUp_Int_sumet); }
  if( fChain->GetBranchStatus("MET_Truth_PileUp_IntCentral_sumet") ) { fChain->SetBranchAddress("MET_Truth_PileUp_IntCentral_sumet", &MET_Truth_PileUp_IntCentral_sumet, &b_MET_Truth_PileUp_IntCentral_sumet); }
  if( fChain->GetBranchStatus("MET_Truth_PileUp_IntFwd_sumet") ) { fChain->SetBranchAddress("MET_Truth_PileUp_IntFwd_sumet", &MET_Truth_PileUp_IntFwd_sumet, &b_MET_Truth_PileUp_IntFwd_sumet); }
  if( fChain->GetBranchStatus("MET_Truth_PileUp_IntOutCover_sumet") ) { fChain->SetBranchAddress("MET_Truth_PileUp_IntOutCover_sumet", &MET_Truth_PileUp_IntOutCover_sumet, &b_MET_Truth_PileUp_IntOutCover_sumet); }
  if( fChain->GetBranchStatus("MET_Truth_PileUp_IntMuons_sumet") ) { fChain->SetBranchAddress("MET_Truth_PileUp_IntMuons_sumet", &MET_Truth_PileUp_IntMuons_sumet, &b_MET_Truth_PileUp_IntMuons_sumet); }
  if( fChain->GetBranchStatus("MET_DM_Crack1_etx") ) { fChain->SetBranchAddress("MET_DM_Crack1_etx", &MET_DM_Crack1_etx, &b_MET_DM_Crack1_etx); }
  if( fChain->GetBranchStatus("MET_DM_Crack1_ety") ) { fChain->SetBranchAddress("MET_DM_Crack1_ety", &MET_DM_Crack1_ety, &b_MET_DM_Crack1_ety); }
  if( fChain->GetBranchStatus("MET_DM_Crack1_phi") ) { fChain->SetBranchAddress("MET_DM_Crack1_phi", &MET_DM_Crack1_phi, &b_MET_DM_Crack1_phi); }
  if( fChain->GetBranchStatus("MET_DM_Crack1_et") ) { fChain->SetBranchAddress("MET_DM_Crack1_et", &MET_DM_Crack1_et, &b_MET_DM_Crack1_et); }
  if( fChain->GetBranchStatus("MET_DM_Crack1_sumet") ) { fChain->SetBranchAddress("MET_DM_Crack1_sumet", &MET_DM_Crack1_sumet, &b_MET_DM_Crack1_sumet); }
  if( fChain->GetBranchStatus("MET_DM_Crack2_etx") ) { fChain->SetBranchAddress("MET_DM_Crack2_etx", &MET_DM_Crack2_etx, &b_MET_DM_Crack2_etx); }
  if( fChain->GetBranchStatus("MET_DM_Crack2_ety") ) { fChain->SetBranchAddress("MET_DM_Crack2_ety", &MET_DM_Crack2_ety, &b_MET_DM_Crack2_ety); }
  if( fChain->GetBranchStatus("MET_DM_Crack2_phi") ) { fChain->SetBranchAddress("MET_DM_Crack2_phi", &MET_DM_Crack2_phi, &b_MET_DM_Crack2_phi); }
  if( fChain->GetBranchStatus("MET_DM_Crack2_et") ) { fChain->SetBranchAddress("MET_DM_Crack2_et", &MET_DM_Crack2_et, &b_MET_DM_Crack2_et); }
  if( fChain->GetBranchStatus("MET_DM_Crack2_sumet") ) { fChain->SetBranchAddress("MET_DM_Crack2_sumet", &MET_DM_Crack2_sumet, &b_MET_DM_Crack2_sumet); }
  if( fChain->GetBranchStatus("MET_DM_All_etx") ) { fChain->SetBranchAddress("MET_DM_All_etx", &MET_DM_All_etx, &b_MET_DM_All_etx); }
  if( fChain->GetBranchStatus("MET_DM_All_ety") ) { fChain->SetBranchAddress("MET_DM_All_ety", &MET_DM_All_ety, &b_MET_DM_All_ety); }
  if( fChain->GetBranchStatus("MET_DM_All_phi") ) { fChain->SetBranchAddress("MET_DM_All_phi", &MET_DM_All_phi, &b_MET_DM_All_phi); }
  if( fChain->GetBranchStatus("MET_DM_All_et") ) { fChain->SetBranchAddress("MET_DM_All_et", &MET_DM_All_et, &b_MET_DM_All_et); }
  if( fChain->GetBranchStatus("MET_DM_All_sumet") ) { fChain->SetBranchAddress("MET_DM_All_sumet", &MET_DM_All_sumet, &b_MET_DM_All_sumet); }
  if( fChain->GetBranchStatus("MET_DM_Cryo_etx") ) { fChain->SetBranchAddress("MET_DM_Cryo_etx", &MET_DM_Cryo_etx, &b_MET_DM_Cryo_etx); }
  if( fChain->GetBranchStatus("MET_DM_Cryo_ety") ) { fChain->SetBranchAddress("MET_DM_Cryo_ety", &MET_DM_Cryo_ety, &b_MET_DM_Cryo_ety); }
  if( fChain->GetBranchStatus("MET_DM_Cryo_phi") ) { fChain->SetBranchAddress("MET_DM_Cryo_phi", &MET_DM_Cryo_phi, &b_MET_DM_Cryo_phi); }
  if( fChain->GetBranchStatus("MET_DM_Cryo_et") ) { fChain->SetBranchAddress("MET_DM_Cryo_et", &MET_DM_Cryo_et, &b_MET_DM_Cryo_et); }
  if( fChain->GetBranchStatus("MET_DM_Cryo_sumet") ) { fChain->SetBranchAddress("MET_DM_Cryo_sumet", &MET_DM_Cryo_sumet, &b_MET_DM_Cryo_sumet); }
  if( fChain->GetBranchStatus("METJetsInfo_JetPtWeightedEventEMFraction") ) { fChain->SetBranchAddress("METJetsInfo_JetPtWeightedEventEMFraction", &METJetsInfo_JetPtWeightedEventEMFraction, &b_METJetsInfo_JetPtWeightedEventEMFraction); }
  if( fChain->GetBranchStatus("METJetsInfo_JetPtWeightedNumAssociatedTracks") ) { fChain->SetBranchAddress("METJetsInfo_JetPtWeightedNumAssociatedTracks", &METJetsInfo_JetPtWeightedNumAssociatedTracks, &b_METJetsInfo_JetPtWeightedNumAssociatedTracks); }
  if( fChain->GetBranchStatus("METJetsInfo_JetPtWeightedSize") ) { fChain->SetBranchAddress("METJetsInfo_JetPtWeightedSize", &METJetsInfo_JetPtWeightedSize, &b_METJetsInfo_JetPtWeightedSize); }
  if( fChain->GetBranchStatus("METJetsInfo_LeadingJetEt") ) { fChain->SetBranchAddress("METJetsInfo_LeadingJetEt", &METJetsInfo_LeadingJetEt, &b_METJetsInfo_LeadingJetEt); }
  if( fChain->GetBranchStatus("METJetsInfo_LeadingJetEta") ) { fChain->SetBranchAddress("METJetsInfo_LeadingJetEta", &METJetsInfo_LeadingJetEta, &b_METJetsInfo_LeadingJetEta); }
  if( fChain->GetBranchStatus("cl_n") ) { fChain->SetBranchAddress("cl_n", &cl_n, &b_cl_n); }
  if( fChain->GetBranchStatus("cl_pt") ) { fChain->SetBranchAddress("cl_pt", &cl_pt, &b_cl_pt); }
  if( fChain->GetBranchStatus("cl_eta") ) { fChain->SetBranchAddress("cl_eta", &cl_eta, &b_cl_eta); }
  if( fChain->GetBranchStatus("cl_phi") ) { fChain->SetBranchAddress("cl_phi", &cl_phi, &b_cl_phi); }
  if( fChain->GetBranchStatus("clpt10_n") ) { fChain->SetBranchAddress("clpt10_n", &clpt10_n, &b_clpt10_n); }
  if( fChain->GetBranchStatus("clpt10_pt") ) { fChain->SetBranchAddress("clpt10_pt", &clpt10_pt, &b_clpt10_pt); }
  if( fChain->GetBranchStatus("clpt10_eta") ) { fChain->SetBranchAddress("clpt10_eta", &clpt10_eta, &b_clpt10_eta); }
  if( fChain->GetBranchStatus("clpt10_phi") ) { fChain->SetBranchAddress("clpt10_phi", &clpt10_phi, &b_clpt10_phi); }
  if( fChain->GetBranchStatus("clpt10_E_em") ) { fChain->SetBranchAddress("clpt10_E_em", &clpt10_E_em, &b_clpt10_E_em); }
  if( fChain->GetBranchStatus("clpt10_E_had") ) { fChain->SetBranchAddress("clpt10_E_had", &clpt10_E_had, &b_clpt10_E_had); }
  if( fChain->GetBranchStatus("clpt10_firstEdens") ) { fChain->SetBranchAddress("clpt10_firstEdens", &clpt10_firstEdens, &b_clpt10_firstEdens); }
  if( fChain->GetBranchStatus("clpt10_cellmaxfrac") ) { fChain->SetBranchAddress("clpt10_cellmaxfrac", &clpt10_cellmaxfrac, &b_clpt10_cellmaxfrac); }
  if( fChain->GetBranchStatus("clpt10_longitudinal") ) { fChain->SetBranchAddress("clpt10_longitudinal", &clpt10_longitudinal, &b_clpt10_longitudinal); }
  if( fChain->GetBranchStatus("clpt10_secondlambda") ) { fChain->SetBranchAddress("clpt10_secondlambda", &clpt10_secondlambda, &b_clpt10_secondlambda); }
  if( fChain->GetBranchStatus("clpt10_lateral") ) { fChain->SetBranchAddress("clpt10_lateral", &clpt10_lateral, &b_clpt10_lateral); }
  if( fChain->GetBranchStatus("clpt10_secondR") ) { fChain->SetBranchAddress("clpt10_secondR", &clpt10_secondR, &b_clpt10_secondR); }
  if( fChain->GetBranchStatus("clpt10_centerlambda") ) { fChain->SetBranchAddress("clpt10_centerlambda", &clpt10_centerlambda, &b_clpt10_centerlambda); }
  if( fChain->GetBranchStatus("clpt10_deltaTheta") ) { fChain->SetBranchAddress("clpt10_deltaTheta", &clpt10_deltaTheta, &b_clpt10_deltaTheta); }
  if( fChain->GetBranchStatus("clpt10_deltaPhi") ) { fChain->SetBranchAddress("clpt10_deltaPhi", &clpt10_deltaPhi, &b_clpt10_deltaPhi); }
  if( fChain->GetBranchStatus("clpt10_time") ) { fChain->SetBranchAddress("clpt10_time", &clpt10_time, &b_clpt10_time); }
  if( fChain->GetBranchStatus("clpt10_E_PreSamplerB") ) { fChain->SetBranchAddress("clpt10_E_PreSamplerB", &clpt10_E_PreSamplerB, &b_clpt10_E_PreSamplerB); }
  if( fChain->GetBranchStatus("clpt10_E_EMB1") ) { fChain->SetBranchAddress("clpt10_E_EMB1", &clpt10_E_EMB1, &b_clpt10_E_EMB1); }
  if( fChain->GetBranchStatus("clpt10_E_EMB2") ) { fChain->SetBranchAddress("clpt10_E_EMB2", &clpt10_E_EMB2, &b_clpt10_E_EMB2); }
  if( fChain->GetBranchStatus("clpt10_E_EMB3") ) { fChain->SetBranchAddress("clpt10_E_EMB3", &clpt10_E_EMB3, &b_clpt10_E_EMB3); }
  if( fChain->GetBranchStatus("clpt10_E_PreSamplerE") ) { fChain->SetBranchAddress("clpt10_E_PreSamplerE", &clpt10_E_PreSamplerE, &b_clpt10_E_PreSamplerE); }
  if( fChain->GetBranchStatus("clpt10_E_EME1") ) { fChain->SetBranchAddress("clpt10_E_EME1", &clpt10_E_EME1, &b_clpt10_E_EME1); }
  if( fChain->GetBranchStatus("clpt10_E_EME2") ) { fChain->SetBranchAddress("clpt10_E_EME2", &clpt10_E_EME2, &b_clpt10_E_EME2); }
  if( fChain->GetBranchStatus("clpt10_E_EME3") ) { fChain->SetBranchAddress("clpt10_E_EME3", &clpt10_E_EME3, &b_clpt10_E_EME3); }
  if( fChain->GetBranchStatus("clpt10_E_HEC0") ) { fChain->SetBranchAddress("clpt10_E_HEC0", &clpt10_E_HEC0, &b_clpt10_E_HEC0); }
  if( fChain->GetBranchStatus("clpt10_E_HEC1") ) { fChain->SetBranchAddress("clpt10_E_HEC1", &clpt10_E_HEC1, &b_clpt10_E_HEC1); }
  if( fChain->GetBranchStatus("clpt10_E_HEC2") ) { fChain->SetBranchAddress("clpt10_E_HEC2", &clpt10_E_HEC2, &b_clpt10_E_HEC2); }
  if( fChain->GetBranchStatus("clpt10_E_HEC3") ) { fChain->SetBranchAddress("clpt10_E_HEC3", &clpt10_E_HEC3, &b_clpt10_E_HEC3); }
  if( fChain->GetBranchStatus("clpt10_E_TileBar0") ) { fChain->SetBranchAddress("clpt10_E_TileBar0", &clpt10_E_TileBar0, &b_clpt10_E_TileBar0); }
  if( fChain->GetBranchStatus("clpt10_E_TileBar1") ) { fChain->SetBranchAddress("clpt10_E_TileBar1", &clpt10_E_TileBar1, &b_clpt10_E_TileBar1); }
  if( fChain->GetBranchStatus("clpt10_E_TileBar2") ) { fChain->SetBranchAddress("clpt10_E_TileBar2", &clpt10_E_TileBar2, &b_clpt10_E_TileBar2); }
  if( fChain->GetBranchStatus("clpt10_E_TileGap1") ) { fChain->SetBranchAddress("clpt10_E_TileGap1", &clpt10_E_TileGap1, &b_clpt10_E_TileGap1); }
  if( fChain->GetBranchStatus("clpt10_E_TileGap2") ) { fChain->SetBranchAddress("clpt10_E_TileGap2", &clpt10_E_TileGap2, &b_clpt10_E_TileGap2); }
  if( fChain->GetBranchStatus("clpt10_E_TileGap3") ) { fChain->SetBranchAddress("clpt10_E_TileGap3", &clpt10_E_TileGap3, &b_clpt10_E_TileGap3); }
  if( fChain->GetBranchStatus("clpt10_E_TileExt0") ) { fChain->SetBranchAddress("clpt10_E_TileExt0", &clpt10_E_TileExt0, &b_clpt10_E_TileExt0); }
  if( fChain->GetBranchStatus("clpt10_E_TileExt1") ) { fChain->SetBranchAddress("clpt10_E_TileExt1", &clpt10_E_TileExt1, &b_clpt10_E_TileExt1); }
  if( fChain->GetBranchStatus("clpt10_E_TileExt2") ) { fChain->SetBranchAddress("clpt10_E_TileExt2", &clpt10_E_TileExt2, &b_clpt10_E_TileExt2); }
  if( fChain->GetBranchStatus("clpt10_E_FCAL0") ) { fChain->SetBranchAddress("clpt10_E_FCAL0", &clpt10_E_FCAL0, &b_clpt10_E_FCAL0); }
  if( fChain->GetBranchStatus("clpt10_E_FCAL1") ) { fChain->SetBranchAddress("clpt10_E_FCAL1", &clpt10_E_FCAL1, &b_clpt10_E_FCAL1); }
  if( fChain->GetBranchStatus("clpt10_E_FCAL2") ) { fChain->SetBranchAddress("clpt10_E_FCAL2", &clpt10_E_FCAL2, &b_clpt10_E_FCAL2); }
  if( fChain->GetBranchStatus("emclpt10_n") ) { fChain->SetBranchAddress("emclpt10_n", &emclpt10_n, &b_emclpt10_n); }
  if( fChain->GetBranchStatus("emclpt10_pt") ) { fChain->SetBranchAddress("emclpt10_pt", &emclpt10_pt, &b_emclpt10_pt); }
  if( fChain->GetBranchStatus("emclpt10_eta") ) { fChain->SetBranchAddress("emclpt10_eta", &emclpt10_eta, &b_emclpt10_eta); }
  if( fChain->GetBranchStatus("emclpt10_phi") ) { fChain->SetBranchAddress("emclpt10_phi", &emclpt10_phi, &b_emclpt10_phi); }
  if( fChain->GetBranchStatus("emclpt10_E_em") ) { fChain->SetBranchAddress("emclpt10_E_em", &emclpt10_E_em, &b_emclpt10_E_em); }
  if( fChain->GetBranchStatus("emclpt10_E_had") ) { fChain->SetBranchAddress("emclpt10_E_had", &emclpt10_E_had, &b_emclpt10_E_had); }
  if( fChain->GetBranchStatus("emclpt10_firstEdens") ) { fChain->SetBranchAddress("emclpt10_firstEdens", &emclpt10_firstEdens, &b_emclpt10_firstEdens); }
  if( fChain->GetBranchStatus("emclpt10_cellmaxfrac") ) { fChain->SetBranchAddress("emclpt10_cellmaxfrac", &emclpt10_cellmaxfrac, &b_emclpt10_cellmaxfrac); }
  if( fChain->GetBranchStatus("emclpt10_longitudinal") ) { fChain->SetBranchAddress("emclpt10_longitudinal", &emclpt10_longitudinal, &b_emclpt10_longitudinal); }
  if( fChain->GetBranchStatus("emclpt10_secondlambda") ) { fChain->SetBranchAddress("emclpt10_secondlambda", &emclpt10_secondlambda, &b_emclpt10_secondlambda); }
  if( fChain->GetBranchStatus("emclpt10_lateral") ) { fChain->SetBranchAddress("emclpt10_lateral", &emclpt10_lateral, &b_emclpt10_lateral); }
  if( fChain->GetBranchStatus("emclpt10_secondR") ) { fChain->SetBranchAddress("emclpt10_secondR", &emclpt10_secondR, &b_emclpt10_secondR); }
  if( fChain->GetBranchStatus("emclpt10_centerlambda") ) { fChain->SetBranchAddress("emclpt10_centerlambda", &emclpt10_centerlambda, &b_emclpt10_centerlambda); }
  if( fChain->GetBranchStatus("emclpt10_deltaTheta") ) { fChain->SetBranchAddress("emclpt10_deltaTheta", &emclpt10_deltaTheta, &b_emclpt10_deltaTheta); }
  if( fChain->GetBranchStatus("emclpt10_deltaPhi") ) { fChain->SetBranchAddress("emclpt10_deltaPhi", &emclpt10_deltaPhi, &b_emclpt10_deltaPhi); }
  if( fChain->GetBranchStatus("emclpt10_time") ) { fChain->SetBranchAddress("emclpt10_time", &emclpt10_time, &b_emclpt10_time); }
  if( fChain->GetBranchStatus("emclpt10_E_PreSamplerB") ) { fChain->SetBranchAddress("emclpt10_E_PreSamplerB", &emclpt10_E_PreSamplerB, &b_emclpt10_E_PreSamplerB); }
  if( fChain->GetBranchStatus("emclpt10_E_EMB1") ) { fChain->SetBranchAddress("emclpt10_E_EMB1", &emclpt10_E_EMB1, &b_emclpt10_E_EMB1); }
  if( fChain->GetBranchStatus("emclpt10_E_EMB2") ) { fChain->SetBranchAddress("emclpt10_E_EMB2", &emclpt10_E_EMB2, &b_emclpt10_E_EMB2); }
  if( fChain->GetBranchStatus("emclpt10_E_EMB3") ) { fChain->SetBranchAddress("emclpt10_E_EMB3", &emclpt10_E_EMB3, &b_emclpt10_E_EMB3); }
  if( fChain->GetBranchStatus("emclpt10_E_PreSamplerE") ) { fChain->SetBranchAddress("emclpt10_E_PreSamplerE", &emclpt10_E_PreSamplerE, &b_emclpt10_E_PreSamplerE); }
  if( fChain->GetBranchStatus("emclpt10_E_EME1") ) { fChain->SetBranchAddress("emclpt10_E_EME1", &emclpt10_E_EME1, &b_emclpt10_E_EME1); }
  if( fChain->GetBranchStatus("emclpt10_E_EME2") ) { fChain->SetBranchAddress("emclpt10_E_EME2", &emclpt10_E_EME2, &b_emclpt10_E_EME2); }
  if( fChain->GetBranchStatus("emclpt10_E_EME3") ) { fChain->SetBranchAddress("emclpt10_E_EME3", &emclpt10_E_EME3, &b_emclpt10_E_EME3); }
  if( fChain->GetBranchStatus("emclpt10_E_HEC0") ) { fChain->SetBranchAddress("emclpt10_E_HEC0", &emclpt10_E_HEC0, &b_emclpt10_E_HEC0); }
  if( fChain->GetBranchStatus("emclpt10_E_HEC1") ) { fChain->SetBranchAddress("emclpt10_E_HEC1", &emclpt10_E_HEC1, &b_emclpt10_E_HEC1); }
  if( fChain->GetBranchStatus("emclpt10_E_HEC2") ) { fChain->SetBranchAddress("emclpt10_E_HEC2", &emclpt10_E_HEC2, &b_emclpt10_E_HEC2); }
  if( fChain->GetBranchStatus("emclpt10_E_HEC3") ) { fChain->SetBranchAddress("emclpt10_E_HEC3", &emclpt10_E_HEC3, &b_emclpt10_E_HEC3); }
  if( fChain->GetBranchStatus("emclpt10_E_TileBar0") ) { fChain->SetBranchAddress("emclpt10_E_TileBar0", &emclpt10_E_TileBar0, &b_emclpt10_E_TileBar0); }
  if( fChain->GetBranchStatus("emclpt10_E_TileBar1") ) { fChain->SetBranchAddress("emclpt10_E_TileBar1", &emclpt10_E_TileBar1, &b_emclpt10_E_TileBar1); }
  if( fChain->GetBranchStatus("emclpt10_E_TileBar2") ) { fChain->SetBranchAddress("emclpt10_E_TileBar2", &emclpt10_E_TileBar2, &b_emclpt10_E_TileBar2); }
  if( fChain->GetBranchStatus("emclpt10_E_TileGap1") ) { fChain->SetBranchAddress("emclpt10_E_TileGap1", &emclpt10_E_TileGap1, &b_emclpt10_E_TileGap1); }
  if( fChain->GetBranchStatus("emclpt10_E_TileGap2") ) { fChain->SetBranchAddress("emclpt10_E_TileGap2", &emclpt10_E_TileGap2, &b_emclpt10_E_TileGap2); }
  if( fChain->GetBranchStatus("emclpt10_E_TileGap3") ) { fChain->SetBranchAddress("emclpt10_E_TileGap3", &emclpt10_E_TileGap3, &b_emclpt10_E_TileGap3); }
  if( fChain->GetBranchStatus("emclpt10_E_TileExt0") ) { fChain->SetBranchAddress("emclpt10_E_TileExt0", &emclpt10_E_TileExt0, &b_emclpt10_E_TileExt0); }
  if( fChain->GetBranchStatus("emclpt10_E_TileExt1") ) { fChain->SetBranchAddress("emclpt10_E_TileExt1", &emclpt10_E_TileExt1, &b_emclpt10_E_TileExt1); }
  if( fChain->GetBranchStatus("emclpt10_E_TileExt2") ) { fChain->SetBranchAddress("emclpt10_E_TileExt2", &emclpt10_E_TileExt2, &b_emclpt10_E_TileExt2); }
  if( fChain->GetBranchStatus("emclpt10_E_FCAL0") ) { fChain->SetBranchAddress("emclpt10_E_FCAL0", &emclpt10_E_FCAL0, &b_emclpt10_E_FCAL0); }
  if( fChain->GetBranchStatus("emclpt10_E_FCAL1") ) { fChain->SetBranchAddress("emclpt10_E_FCAL1", &emclpt10_E_FCAL1, &b_emclpt10_E_FCAL1); }
  if( fChain->GetBranchStatus("emclpt10_E_FCAL2") ) { fChain->SetBranchAddress("emclpt10_E_FCAL2", &emclpt10_E_FCAL2, &b_emclpt10_E_FCAL2); }
  if( fChain->GetBranchStatus("emclpt10_eta_PreSamplerB") ) { fChain->SetBranchAddress("emclpt10_eta_PreSamplerB", &emclpt10_eta_PreSamplerB, &b_emclpt10_eta_PreSamplerB); }
  if( fChain->GetBranchStatus("emclpt10_phi_PreSamplerB") ) { fChain->SetBranchAddress("emclpt10_phi_PreSamplerB", &emclpt10_phi_PreSamplerB, &b_emclpt10_phi_PreSamplerB); }
  if( fChain->GetBranchStatus("emclpt10_eta_EMB1") ) { fChain->SetBranchAddress("emclpt10_eta_EMB1", &emclpt10_eta_EMB1, &b_emclpt10_eta_EMB1); }
  if( fChain->GetBranchStatus("emclpt10_phi_EMB1") ) { fChain->SetBranchAddress("emclpt10_phi_EMB1", &emclpt10_phi_EMB1, &b_emclpt10_phi_EMB1); }
  if( fChain->GetBranchStatus("emclpt10_eta_EMB2") ) { fChain->SetBranchAddress("emclpt10_eta_EMB2", &emclpt10_eta_EMB2, &b_emclpt10_eta_EMB2); }
  if( fChain->GetBranchStatus("emclpt10_phi_EMB2") ) { fChain->SetBranchAddress("emclpt10_phi_EMB2", &emclpt10_phi_EMB2, &b_emclpt10_phi_EMB2); }
  if( fChain->GetBranchStatus("emclpt10_eta_EMB3") ) { fChain->SetBranchAddress("emclpt10_eta_EMB3", &emclpt10_eta_EMB3, &b_emclpt10_eta_EMB3); }
  if( fChain->GetBranchStatus("emclpt10_phi_EMB3") ) { fChain->SetBranchAddress("emclpt10_phi_EMB3", &emclpt10_phi_EMB3, &b_emclpt10_phi_EMB3); }
  if( fChain->GetBranchStatus("emclpt10_eta_PreSamplerE") ) { fChain->SetBranchAddress("emclpt10_eta_PreSamplerE", &emclpt10_eta_PreSamplerE, &b_emclpt10_eta_PreSamplerE); }
  if( fChain->GetBranchStatus("emclpt10_phi_PreSamplerE") ) { fChain->SetBranchAddress("emclpt10_phi_PreSamplerE", &emclpt10_phi_PreSamplerE, &b_emclpt10_phi_PreSamplerE); }
  if( fChain->GetBranchStatus("emclpt10_eta_EME1") ) { fChain->SetBranchAddress("emclpt10_eta_EME1", &emclpt10_eta_EME1, &b_emclpt10_eta_EME1); }
  if( fChain->GetBranchStatus("emclpt10_phi_EME1") ) { fChain->SetBranchAddress("emclpt10_phi_EME1", &emclpt10_phi_EME1, &b_emclpt10_phi_EME1); }
  if( fChain->GetBranchStatus("emclpt10_eta_EME2") ) { fChain->SetBranchAddress("emclpt10_eta_EME2", &emclpt10_eta_EME2, &b_emclpt10_eta_EME2); }
  if( fChain->GetBranchStatus("emclpt10_phi_EME2") ) { fChain->SetBranchAddress("emclpt10_phi_EME2", &emclpt10_phi_EME2, &b_emclpt10_phi_EME2); }
  if( fChain->GetBranchStatus("emclpt10_eta_EME3") ) { fChain->SetBranchAddress("emclpt10_eta_EME3", &emclpt10_eta_EME3, &b_emclpt10_eta_EME3); }
  if( fChain->GetBranchStatus("emclpt10_phi_EME3") ) { fChain->SetBranchAddress("emclpt10_phi_EME3", &emclpt10_phi_EME3, &b_emclpt10_phi_EME3); }
  if( fChain->GetBranchStatus("emclpt10_eta_HEC0") ) { fChain->SetBranchAddress("emclpt10_eta_HEC0", &emclpt10_eta_HEC0, &b_emclpt10_eta_HEC0); }
  if( fChain->GetBranchStatus("emclpt10_phi_HEC0") ) { fChain->SetBranchAddress("emclpt10_phi_HEC0", &emclpt10_phi_HEC0, &b_emclpt10_phi_HEC0); }
  if( fChain->GetBranchStatus("emclpt10_eta_HEC1") ) { fChain->SetBranchAddress("emclpt10_eta_HEC1", &emclpt10_eta_HEC1, &b_emclpt10_eta_HEC1); }
  if( fChain->GetBranchStatus("emclpt10_phi_HEC1") ) { fChain->SetBranchAddress("emclpt10_phi_HEC1", &emclpt10_phi_HEC1, &b_emclpt10_phi_HEC1); }
  if( fChain->GetBranchStatus("emclpt10_eta_HEC2") ) { fChain->SetBranchAddress("emclpt10_eta_HEC2", &emclpt10_eta_HEC2, &b_emclpt10_eta_HEC2); }
  if( fChain->GetBranchStatus("emclpt10_phi_HEC2") ) { fChain->SetBranchAddress("emclpt10_phi_HEC2", &emclpt10_phi_HEC2, &b_emclpt10_phi_HEC2); }
  if( fChain->GetBranchStatus("emclpt10_eta_HEC3") ) { fChain->SetBranchAddress("emclpt10_eta_HEC3", &emclpt10_eta_HEC3, &b_emclpt10_eta_HEC3); }
  if( fChain->GetBranchStatus("emclpt10_phi_HEC3") ) { fChain->SetBranchAddress("emclpt10_phi_HEC3", &emclpt10_phi_HEC3, &b_emclpt10_phi_HEC3); }
  if( fChain->GetBranchStatus("emclpt10_eta_TileBar0") ) { fChain->SetBranchAddress("emclpt10_eta_TileBar0", &emclpt10_eta_TileBar0, &b_emclpt10_eta_TileBar0); }
  if( fChain->GetBranchStatus("emclpt10_phi_TileBar0") ) { fChain->SetBranchAddress("emclpt10_phi_TileBar0", &emclpt10_phi_TileBar0, &b_emclpt10_phi_TileBar0); }
  if( fChain->GetBranchStatus("emclpt10_eta_TileBar1") ) { fChain->SetBranchAddress("emclpt10_eta_TileBar1", &emclpt10_eta_TileBar1, &b_emclpt10_eta_TileBar1); }
  if( fChain->GetBranchStatus("emclpt10_phi_TileBar1") ) { fChain->SetBranchAddress("emclpt10_phi_TileBar1", &emclpt10_phi_TileBar1, &b_emclpt10_phi_TileBar1); }
  if( fChain->GetBranchStatus("emclpt10_eta_TileBar2") ) { fChain->SetBranchAddress("emclpt10_eta_TileBar2", &emclpt10_eta_TileBar2, &b_emclpt10_eta_TileBar2); }
  if( fChain->GetBranchStatus("emclpt10_phi_TileBar2") ) { fChain->SetBranchAddress("emclpt10_phi_TileBar2", &emclpt10_phi_TileBar2, &b_emclpt10_phi_TileBar2); }
  if( fChain->GetBranchStatus("emclpt10_eta_TileGap1") ) { fChain->SetBranchAddress("emclpt10_eta_TileGap1", &emclpt10_eta_TileGap1, &b_emclpt10_eta_TileGap1); }
  if( fChain->GetBranchStatus("emclpt10_phi_TileGap1") ) { fChain->SetBranchAddress("emclpt10_phi_TileGap1", &emclpt10_phi_TileGap1, &b_emclpt10_phi_TileGap1); }
  if( fChain->GetBranchStatus("emclpt10_eta_TileGap2") ) { fChain->SetBranchAddress("emclpt10_eta_TileGap2", &emclpt10_eta_TileGap2, &b_emclpt10_eta_TileGap2); }
  if( fChain->GetBranchStatus("emclpt10_phi_TileGap2") ) { fChain->SetBranchAddress("emclpt10_phi_TileGap2", &emclpt10_phi_TileGap2, &b_emclpt10_phi_TileGap2); }
  if( fChain->GetBranchStatus("emclpt10_eta_TileGap3") ) { fChain->SetBranchAddress("emclpt10_eta_TileGap3", &emclpt10_eta_TileGap3, &b_emclpt10_eta_TileGap3); }
  if( fChain->GetBranchStatus("emclpt10_phi_TileGap3") ) { fChain->SetBranchAddress("emclpt10_phi_TileGap3", &emclpt10_phi_TileGap3, &b_emclpt10_phi_TileGap3); }
  if( fChain->GetBranchStatus("emclpt10_eta_TileExt0") ) { fChain->SetBranchAddress("emclpt10_eta_TileExt0", &emclpt10_eta_TileExt0, &b_emclpt10_eta_TileExt0); }
  if( fChain->GetBranchStatus("emclpt10_phi_TileExt0") ) { fChain->SetBranchAddress("emclpt10_phi_TileExt0", &emclpt10_phi_TileExt0, &b_emclpt10_phi_TileExt0); }
  if( fChain->GetBranchStatus("emclpt10_eta_TileExt1") ) { fChain->SetBranchAddress("emclpt10_eta_TileExt1", &emclpt10_eta_TileExt1, &b_emclpt10_eta_TileExt1); }
  if( fChain->GetBranchStatus("emclpt10_phi_TileExt1") ) { fChain->SetBranchAddress("emclpt10_phi_TileExt1", &emclpt10_phi_TileExt1, &b_emclpt10_phi_TileExt1); }
  if( fChain->GetBranchStatus("emclpt10_eta_TileExt2") ) { fChain->SetBranchAddress("emclpt10_eta_TileExt2", &emclpt10_eta_TileExt2, &b_emclpt10_eta_TileExt2); }
  if( fChain->GetBranchStatus("emclpt10_phi_TileExt2") ) { fChain->SetBranchAddress("emclpt10_phi_TileExt2", &emclpt10_phi_TileExt2, &b_emclpt10_phi_TileExt2); }
  if( fChain->GetBranchStatus("emclpt10_eta_FCAL0") ) { fChain->SetBranchAddress("emclpt10_eta_FCAL0", &emclpt10_eta_FCAL0, &b_emclpt10_eta_FCAL0); }
  if( fChain->GetBranchStatus("emclpt10_phi_FCAL0") ) { fChain->SetBranchAddress("emclpt10_phi_FCAL0", &emclpt10_phi_FCAL0, &b_emclpt10_phi_FCAL0); }
  if( fChain->GetBranchStatus("emclpt10_eta_FCAL1") ) { fChain->SetBranchAddress("emclpt10_eta_FCAL1", &emclpt10_eta_FCAL1, &b_emclpt10_eta_FCAL1); }
  if( fChain->GetBranchStatus("emclpt10_phi_FCAL1") ) { fChain->SetBranchAddress("emclpt10_phi_FCAL1", &emclpt10_phi_FCAL1, &b_emclpt10_phi_FCAL1); }
  if( fChain->GetBranchStatus("emclpt10_eta_FCAL2") ) { fChain->SetBranchAddress("emclpt10_eta_FCAL2", &emclpt10_eta_FCAL2, &b_emclpt10_eta_FCAL2); }
  if( fChain->GetBranchStatus("emclpt10_phi_FCAL2") ) { fChain->SetBranchAddress("emclpt10_phi_FCAL2", &emclpt10_phi_FCAL2, &b_emclpt10_phi_FCAL2); }
  if( fChain->GetBranchStatus("trk_n") ) { fChain->SetBranchAddress("trk_n", &trk_n, &b_trk_n); }
  if( fChain->GetBranchStatus("trk_pt") ) { fChain->SetBranchAddress("trk_pt", &trk_pt, &b_trk_pt); }
  if( fChain->GetBranchStatus("trk_eta") ) { fChain->SetBranchAddress("trk_eta", &trk_eta, &b_trk_eta); }
  if( fChain->GetBranchStatus("trk_d0_wrtPV") ) { fChain->SetBranchAddress("trk_d0_wrtPV", &trk_d0_wrtPV, &b_trk_d0_wrtPV); }
  if( fChain->GetBranchStatus("trk_z0_wrtPV") ) { fChain->SetBranchAddress("trk_z0_wrtPV", &trk_z0_wrtPV, &b_trk_z0_wrtPV); }
  if( fChain->GetBranchStatus("trk_phi_wrtPV") ) { fChain->SetBranchAddress("trk_phi_wrtPV", &trk_phi_wrtPV, &b_trk_phi_wrtPV); }
  if( fChain->GetBranchStatus("trk_theta_wrtPV") ) { fChain->SetBranchAddress("trk_theta_wrtPV", &trk_theta_wrtPV, &b_trk_theta_wrtPV); }
  if( fChain->GetBranchStatus("trk_qoverp_wrtPV") ) { fChain->SetBranchAddress("trk_qoverp_wrtPV", &trk_qoverp_wrtPV, &b_trk_qoverp_wrtPV); }
  if( fChain->GetBranchStatus("trk_chi2") ) { fChain->SetBranchAddress("trk_chi2", &trk_chi2, &b_trk_chi2); }
  if( fChain->GetBranchStatus("trk_ndof") ) { fChain->SetBranchAddress("trk_ndof", &trk_ndof, &b_trk_ndof); }
  if( fChain->GetBranchStatus("trkpt5_n") ) { fChain->SetBranchAddress("trkpt5_n", &trkpt5_n, &b_trkpt5_n); }
  if( fChain->GetBranchStatus("trkpt5_d0") ) { fChain->SetBranchAddress("trkpt5_d0", &trkpt5_d0, &b_trkpt5_d0); }
  if( fChain->GetBranchStatus("trkpt5_z0") ) { fChain->SetBranchAddress("trkpt5_z0", &trkpt5_z0, &b_trkpt5_z0); }
  if( fChain->GetBranchStatus("trkpt5_phi") ) { fChain->SetBranchAddress("trkpt5_phi", &trkpt5_phi, &b_trkpt5_phi); }
  if( fChain->GetBranchStatus("trkpt5_theta") ) { fChain->SetBranchAddress("trkpt5_theta", &trkpt5_theta, &b_trkpt5_theta); }
  if( fChain->GetBranchStatus("trkpt5_qoverp") ) { fChain->SetBranchAddress("trkpt5_qoverp", &trkpt5_qoverp, &b_trkpt5_qoverp); }
  if( fChain->GetBranchStatus("trkpt5_pt") ) { fChain->SetBranchAddress("trkpt5_pt", &trkpt5_pt, &b_trkpt5_pt); }
  if( fChain->GetBranchStatus("trkpt5_eta") ) { fChain->SetBranchAddress("trkpt5_eta", &trkpt5_eta, &b_trkpt5_eta); }
  if( fChain->GetBranchStatus("trkpt5_err_d0") ) { fChain->SetBranchAddress("trkpt5_err_d0", &trkpt5_err_d0, &b_trkpt5_err_d0); }
  if( fChain->GetBranchStatus("trkpt5_err_z0") ) { fChain->SetBranchAddress("trkpt5_err_z0", &trkpt5_err_z0, &b_trkpt5_err_z0); }
  if( fChain->GetBranchStatus("trkpt5_err_phi") ) { fChain->SetBranchAddress("trkpt5_err_phi", &trkpt5_err_phi, &b_trkpt5_err_phi); }
  if( fChain->GetBranchStatus("trkpt5_err_theta") ) { fChain->SetBranchAddress("trkpt5_err_theta", &trkpt5_err_theta, &b_trkpt5_err_theta); }
  if( fChain->GetBranchStatus("trkpt5_err_qoverp") ) { fChain->SetBranchAddress("trkpt5_err_qoverp", &trkpt5_err_qoverp, &b_trkpt5_err_qoverp); }
  if( fChain->GetBranchStatus("trkpt5_d0_wrtPV") ) { fChain->SetBranchAddress("trkpt5_d0_wrtPV", &trkpt5_d0_wrtPV, &b_trkpt5_d0_wrtPV); }
  if( fChain->GetBranchStatus("trkpt5_z0_wrtPV") ) { fChain->SetBranchAddress("trkpt5_z0_wrtPV", &trkpt5_z0_wrtPV, &b_trkpt5_z0_wrtPV); }
  if( fChain->GetBranchStatus("trkpt5_phi_wrtPV") ) { fChain->SetBranchAddress("trkpt5_phi_wrtPV", &trkpt5_phi_wrtPV, &b_trkpt5_phi_wrtPV); }
  if( fChain->GetBranchStatus("trkpt5_err_d0_wrtPV") ) { fChain->SetBranchAddress("trkpt5_err_d0_wrtPV", &trkpt5_err_d0_wrtPV, &b_trkpt5_err_d0_wrtPV); }
  if( fChain->GetBranchStatus("trkpt5_err_z0_wrtPV") ) { fChain->SetBranchAddress("trkpt5_err_z0_wrtPV", &trkpt5_err_z0_wrtPV, &b_trkpt5_err_z0_wrtPV); }
  if( fChain->GetBranchStatus("trkpt5_err_phi_wrtPV") ) { fChain->SetBranchAddress("trkpt5_err_phi_wrtPV", &trkpt5_err_phi_wrtPV, &b_trkpt5_err_phi_wrtPV); }
  if( fChain->GetBranchStatus("trkpt5_err_theta_wrtPV") ) { fChain->SetBranchAddress("trkpt5_err_theta_wrtPV", &trkpt5_err_theta_wrtPV, &b_trkpt5_err_theta_wrtPV); }
  if( fChain->GetBranchStatus("trkpt5_err_qoverp_wrtPV") ) { fChain->SetBranchAddress("trkpt5_err_qoverp_wrtPV", &trkpt5_err_qoverp_wrtPV, &b_trkpt5_err_qoverp_wrtPV); }
  if( fChain->GetBranchStatus("trkpt5_cov_d0_z0_wrtPV") ) { fChain->SetBranchAddress("trkpt5_cov_d0_z0_wrtPV", &trkpt5_cov_d0_z0_wrtPV, &b_trkpt5_cov_d0_z0_wrtPV); }
  if( fChain->GetBranchStatus("trkpt5_cov_d0_phi_wrtPV") ) { fChain->SetBranchAddress("trkpt5_cov_d0_phi_wrtPV", &trkpt5_cov_d0_phi_wrtPV, &b_trkpt5_cov_d0_phi_wrtPV); }
  if( fChain->GetBranchStatus("trkpt5_cov_d0_theta_wrtPV") ) { fChain->SetBranchAddress("trkpt5_cov_d0_theta_wrtPV", &trkpt5_cov_d0_theta_wrtPV, &b_trkpt5_cov_d0_theta_wrtPV); }
  if( fChain->GetBranchStatus("trkpt5_cov_d0_qoverp_wrtPV") ) { fChain->SetBranchAddress("trkpt5_cov_d0_qoverp_wrtPV", &trkpt5_cov_d0_qoverp_wrtPV, &b_trkpt5_cov_d0_qoverp_wrtPV); }
  if( fChain->GetBranchStatus("trkpt5_cov_z0_phi_wrtPV") ) { fChain->SetBranchAddress("trkpt5_cov_z0_phi_wrtPV", &trkpt5_cov_z0_phi_wrtPV, &b_trkpt5_cov_z0_phi_wrtPV); }
  if( fChain->GetBranchStatus("trkpt5_cov_z0_theta_wrtPV") ) { fChain->SetBranchAddress("trkpt5_cov_z0_theta_wrtPV", &trkpt5_cov_z0_theta_wrtPV, &b_trkpt5_cov_z0_theta_wrtPV); }
  if( fChain->GetBranchStatus("trkpt5_cov_z0_qoverp_wrtPV") ) { fChain->SetBranchAddress("trkpt5_cov_z0_qoverp_wrtPV", &trkpt5_cov_z0_qoverp_wrtPV, &b_trkpt5_cov_z0_qoverp_wrtPV); }
  if( fChain->GetBranchStatus("trkpt5_cov_phi_theta_wrtPV") ) { fChain->SetBranchAddress("trkpt5_cov_phi_theta_wrtPV", &trkpt5_cov_phi_theta_wrtPV, &b_trkpt5_cov_phi_theta_wrtPV); }
  if( fChain->GetBranchStatus("trkpt5_cov_phi_qoverp_wrtPV") ) { fChain->SetBranchAddress("trkpt5_cov_phi_qoverp_wrtPV", &trkpt5_cov_phi_qoverp_wrtPV, &b_trkpt5_cov_phi_qoverp_wrtPV); }
  if( fChain->GetBranchStatus("trkpt5_cov_theta_qoverp_wrtPV") ) { fChain->SetBranchAddress("trkpt5_cov_theta_qoverp_wrtPV", &trkpt5_cov_theta_qoverp_wrtPV, &b_trkpt5_cov_theta_qoverp_wrtPV); }
  if( fChain->GetBranchStatus("trkpt5_chi2") ) { fChain->SetBranchAddress("trkpt5_chi2", &trkpt5_chi2, &b_trkpt5_chi2); }
  if( fChain->GetBranchStatus("trkpt5_ndof") ) { fChain->SetBranchAddress("trkpt5_ndof", &trkpt5_ndof, &b_trkpt5_ndof); }
  if( fChain->GetBranchStatus("trkpt5_nBLHits") ) { fChain->SetBranchAddress("trkpt5_nBLHits", &trkpt5_nBLHits, &b_trkpt5_nBLHits); }
  if( fChain->GetBranchStatus("trkpt5_nPixHits") ) { fChain->SetBranchAddress("trkpt5_nPixHits", &trkpt5_nPixHits, &b_trkpt5_nPixHits); }
  if( fChain->GetBranchStatus("trkpt5_nSCTHits") ) { fChain->SetBranchAddress("trkpt5_nSCTHits", &trkpt5_nSCTHits, &b_trkpt5_nSCTHits); }
  if( fChain->GetBranchStatus("trkpt5_nTRTHits") ) { fChain->SetBranchAddress("trkpt5_nTRTHits", &trkpt5_nTRTHits, &b_trkpt5_nTRTHits); }
  if( fChain->GetBranchStatus("trkpt5_nPixHoles") ) { fChain->SetBranchAddress("trkpt5_nPixHoles", &trkpt5_nPixHoles, &b_trkpt5_nPixHoles); }
  if( fChain->GetBranchStatus("trkpt5_nSCTHoles") ) { fChain->SetBranchAddress("trkpt5_nSCTHoles", &trkpt5_nSCTHoles, &b_trkpt5_nSCTHoles); }
  if( fChain->GetBranchStatus("trkpt5_nMDTHits") ) { fChain->SetBranchAddress("trkpt5_nMDTHits", &trkpt5_nMDTHits, &b_trkpt5_nMDTHits); }
  if( fChain->GetBranchStatus("trkpt5_nCSCEtaHits") ) { fChain->SetBranchAddress("trkpt5_nCSCEtaHits", &trkpt5_nCSCEtaHits, &b_trkpt5_nCSCEtaHits); }
  if( fChain->GetBranchStatus("trkpt5_nCSCPhiHits") ) { fChain->SetBranchAddress("trkpt5_nCSCPhiHits", &trkpt5_nCSCPhiHits, &b_trkpt5_nCSCPhiHits); }
  if( fChain->GetBranchStatus("trkpt5_nRPCEtaHits") ) { fChain->SetBranchAddress("trkpt5_nRPCEtaHits", &trkpt5_nRPCEtaHits, &b_trkpt5_nRPCEtaHits); }
  if( fChain->GetBranchStatus("trkpt5_nRPCPhiHits") ) { fChain->SetBranchAddress("trkpt5_nRPCPhiHits", &trkpt5_nRPCPhiHits, &b_trkpt5_nRPCPhiHits); }
  if( fChain->GetBranchStatus("trkpt5_nTGCEtaHits") ) { fChain->SetBranchAddress("trkpt5_nTGCEtaHits", &trkpt5_nTGCEtaHits, &b_trkpt5_nTGCEtaHits); }
  if( fChain->GetBranchStatus("trkpt5_nTGCPhiHits") ) { fChain->SetBranchAddress("trkpt5_nTGCPhiHits", &trkpt5_nTGCPhiHits, &b_trkpt5_nTGCPhiHits); }
  if( fChain->GetBranchStatus("trkpt5_nHits") ) { fChain->SetBranchAddress("trkpt5_nHits", &trkpt5_nHits, &b_trkpt5_nHits); }
  if( fChain->GetBranchStatus("trkpt5_nHoles") ) { fChain->SetBranchAddress("trkpt5_nHoles", &trkpt5_nHoles, &b_trkpt5_nHoles); }
  if( fChain->GetBranchStatus("trkpt5_hitPattern") ) { fChain->SetBranchAddress("trkpt5_hitPattern", &trkpt5_hitPattern, &b_trkpt5_hitPattern); }
  if( fChain->GetBranchStatus("trkpt5_TRTHighTHitsRatio") ) { fChain->SetBranchAddress("trkpt5_TRTHighTHitsRatio", &trkpt5_TRTHighTHitsRatio, &b_trkpt5_TRTHighTHitsRatio); }
  if( fChain->GetBranchStatus("trkpt5_pixeldEdx") ) { fChain->SetBranchAddress("trkpt5_pixeldEdx", &trkpt5_pixeldEdx, &b_trkpt5_pixeldEdx); }
  if( fChain->GetBranchStatus("trkpt5_mc_probability") ) { fChain->SetBranchAddress("trkpt5_mc_probability", &trkpt5_mc_probability, &b_trkpt5_mc_probability); }
  if( fChain->GetBranchStatus("trkpt5_mc_barcode") ) { fChain->SetBranchAddress("trkpt5_mc_barcode", &trkpt5_mc_barcode, &b_trkpt5_mc_barcode); }
  if( fChain->GetBranchStatus("vxp_n") ) { fChain->SetBranchAddress("vxp_n", &vxp_n, &b_vxp_n); }
  if( fChain->GetBranchStatus("vxp_x") ) { fChain->SetBranchAddress("vxp_x", &vxp_x, &b_vxp_x); }
  if( fChain->GetBranchStatus("vxp_y") ) { fChain->SetBranchAddress("vxp_y", &vxp_y, &b_vxp_y); }
  if( fChain->GetBranchStatus("vxp_z") ) { fChain->SetBranchAddress("vxp_z", &vxp_z, &b_vxp_z); }
  if( fChain->GetBranchStatus("vxp_cov_x") ) { fChain->SetBranchAddress("vxp_cov_x", &vxp_cov_x, &b_vxp_cov_x); }
  if( fChain->GetBranchStatus("vxp_cov_y") ) { fChain->SetBranchAddress("vxp_cov_y", &vxp_cov_y, &b_vxp_cov_y); }
  if( fChain->GetBranchStatus("vxp_cov_z") ) { fChain->SetBranchAddress("vxp_cov_z", &vxp_cov_z, &b_vxp_cov_z); }
  if( fChain->GetBranchStatus("vxp_chi2") ) { fChain->SetBranchAddress("vxp_chi2", &vxp_chi2, &b_vxp_chi2); }
  if( fChain->GetBranchStatus("vxp_ndof") ) { fChain->SetBranchAddress("vxp_ndof", &vxp_ndof, &b_vxp_ndof); }
  if( fChain->GetBranchStatus("vxp_px") ) { fChain->SetBranchAddress("vxp_px", &vxp_px, &b_vxp_px); }
  if( fChain->GetBranchStatus("vxp_py") ) { fChain->SetBranchAddress("vxp_py", &vxp_py, &b_vxp_py); }
  if( fChain->GetBranchStatus("vxp_pz") ) { fChain->SetBranchAddress("vxp_pz", &vxp_pz, &b_vxp_pz); }
  if( fChain->GetBranchStatus("vxp_E") ) { fChain->SetBranchAddress("vxp_E", &vxp_E, &b_vxp_E); }
  if( fChain->GetBranchStatus("vxp_m") ) { fChain->SetBranchAddress("vxp_m", &vxp_m, &b_vxp_m); }
  if( fChain->GetBranchStatus("vxp_nTracks") ) { fChain->SetBranchAddress("vxp_nTracks", &vxp_nTracks, &b_vxp_nTracks); }
  if( fChain->GetBranchStatus("vxp_sumPt") ) { fChain->SetBranchAddress("vxp_sumPt", &vxp_sumPt, &b_vxp_sumPt); }
  if( fChain->GetBranchStatus("vxp_type") ) { fChain->SetBranchAddress("vxp_type", &vxp_type, &b_vxp_type); }
  if( fChain->GetBranchStatus("vxp_trk_n") ) { fChain->SetBranchAddress("vxp_trk_n", &vxp_trk_n, &b_vxp_trk_n); }
  if( fChain->GetBranchStatus("vxp_trk_index") ) { fChain->SetBranchAddress("vxp_trk_index", &vxp_trk_index, &b_vxp_trk_index); }
  if( fChain->GetBranchStatus("mb_n") ) { fChain->SetBranchAddress("mb_n", &mb_n, &b_mb_n); }
  if( fChain->GetBranchStatus("mb_E") ) { fChain->SetBranchAddress("mb_E", &mb_E, &b_mb_E); }
  if( fChain->GetBranchStatus("mb_eta") ) { fChain->SetBranchAddress("mb_eta", &mb_eta, &b_mb_eta); }
  if( fChain->GetBranchStatus("mb_phi") ) { fChain->SetBranchAddress("mb_phi", &mb_phi, &b_mb_phi); }
  if( fChain->GetBranchStatus("mb_time") ) { fChain->SetBranchAddress("mb_time", &mb_time, &b_mb_time); }
  if( fChain->GetBranchStatus("mb_quality") ) { fChain->SetBranchAddress("mb_quality", &mb_quality, &b_mb_quality); }
  if( fChain->GetBranchStatus("mb_type") ) { fChain->SetBranchAddress("mb_type", &mb_type, &b_mb_type); }
  if( fChain->GetBranchStatus("mb_module") ) { fChain->SetBranchAddress("mb_module", &mb_module, &b_mb_module); }
  if( fChain->GetBranchStatus("mb_channel") ) { fChain->SetBranchAddress("mb_channel", &mb_channel, &b_mb_channel); }
  if( fChain->GetBranchStatus("mbtime_timeDiff") ) { fChain->SetBranchAddress("mbtime_timeDiff", &mbtime_timeDiff, &b_mbtime_timeDiff); }
  if( fChain->GetBranchStatus("mbtime_timeA") ) { fChain->SetBranchAddress("mbtime_timeA", &mbtime_timeA, &b_mbtime_timeA); }
  if( fChain->GetBranchStatus("mbtime_timeC") ) { fChain->SetBranchAddress("mbtime_timeC", &mbtime_timeC, &b_mbtime_timeC); }
  if( fChain->GetBranchStatus("mbtime_countA") ) { fChain->SetBranchAddress("mbtime_countA", &mbtime_countA, &b_mbtime_countA); }
  if( fChain->GetBranchStatus("mbtime_countC") ) { fChain->SetBranchAddress("mbtime_countC", &mbtime_countC, &b_mbtime_countC); }
  if( fChain->GetBranchStatus("L1_MBTS_1") ) { fChain->SetBranchAddress("L1_MBTS_1", &L1_MBTS_1, &b_L1_MBTS_1); }
  if( fChain->GetBranchStatus("L1_MBTS_1_1") ) { fChain->SetBranchAddress("L1_MBTS_1_1", &L1_MBTS_1_1, &b_L1_MBTS_1_1); }
  if( fChain->GetBranchStatus("L1_MBTS_1_1_EMPTY") ) { fChain->SetBranchAddress("L1_MBTS_1_1_EMPTY", &L1_MBTS_1_1_EMPTY, &b_L1_MBTS_1_1_EMPTY); }
  if( fChain->GetBranchStatus("L1_MBTS_1_1_UNPAIRED_ISO") ) { fChain->SetBranchAddress("L1_MBTS_1_1_UNPAIRED_ISO", &L1_MBTS_1_1_UNPAIRED_ISO, &b_L1_MBTS_1_1_UNPAIRED_ISO); }
  if( fChain->GetBranchStatus("L1_MBTS_1_EMPTY") ) { fChain->SetBranchAddress("L1_MBTS_1_EMPTY", &L1_MBTS_1_EMPTY, &b_L1_MBTS_1_EMPTY); }
  if( fChain->GetBranchStatus("L1_MBTS_1_UNPAIRED_ISO") ) { fChain->SetBranchAddress("L1_MBTS_1_UNPAIRED_ISO", &L1_MBTS_1_UNPAIRED_ISO, &b_L1_MBTS_1_UNPAIRED_ISO); }
  if( fChain->GetBranchStatus("L1_MBTS_2") ) { fChain->SetBranchAddress("L1_MBTS_2", &L1_MBTS_2, &b_L1_MBTS_2); }
  if( fChain->GetBranchStatus("L1_MBTS_2_EMPTY") ) { fChain->SetBranchAddress("L1_MBTS_2_EMPTY", &L1_MBTS_2_EMPTY, &b_L1_MBTS_2_EMPTY); }
  if( fChain->GetBranchStatus("L1_MBTS_2_UNPAIRED_ISO") ) { fChain->SetBranchAddress("L1_MBTS_2_UNPAIRED_ISO", &L1_MBTS_2_UNPAIRED_ISO, &b_L1_MBTS_2_UNPAIRED_ISO); }
  if( fChain->GetBranchStatus("L1_MBTS_2_UNPAIRED_NONISO") ) { fChain->SetBranchAddress("L1_MBTS_2_UNPAIRED_NONISO", &L1_MBTS_2_UNPAIRED_NONISO, &b_L1_MBTS_2_UNPAIRED_NONISO); }
  if( fChain->GetBranchStatus("L1_MBTS_4_4") ) { fChain->SetBranchAddress("L1_MBTS_4_4", &L1_MBTS_4_4, &b_L1_MBTS_4_4); }
  if( fChain->GetBranchStatus("L1_MBTS_4_4_UNPAIRED_ISO") ) { fChain->SetBranchAddress("L1_MBTS_4_4_UNPAIRED_ISO", &L1_MBTS_4_4_UNPAIRED_ISO, &b_L1_MBTS_4_4_UNPAIRED_ISO); }
  if( fChain->GetBranchStatus("collcand_passCaloTime") ) { fChain->SetBranchAddress("collcand_passCaloTime", &collcand_passCaloTime, &b_collcand_passCaloTime); }
  if( fChain->GetBranchStatus("collcand_passMBTSTime") ) { fChain->SetBranchAddress("collcand_passMBTSTime", &collcand_passMBTSTime, &b_collcand_passMBTSTime); }
  if( fChain->GetBranchStatus("collcand_passTrigger") ) { fChain->SetBranchAddress("collcand_passTrigger", &collcand_passTrigger, &b_collcand_passTrigger); }
  if( fChain->GetBranchStatus("collcand_pass") ) { fChain->SetBranchAddress("collcand_pass", &collcand_pass, &b_collcand_pass); }
  if( fChain->GetBranchStatus("muonTruth_n") ) { fChain->SetBranchAddress("muonTruth_n", &muonTruth_n, &b_muonTruth_n); }
  if( fChain->GetBranchStatus("muonTruth_pt") ) { fChain->SetBranchAddress("muonTruth_pt", &muonTruth_pt, &b_muonTruth_pt); }
  if( fChain->GetBranchStatus("muonTruth_m") ) { fChain->SetBranchAddress("muonTruth_m", &muonTruth_m, &b_muonTruth_m); }
  if( fChain->GetBranchStatus("muonTruth_eta") ) { fChain->SetBranchAddress("muonTruth_eta", &muonTruth_eta, &b_muonTruth_eta); }
  if( fChain->GetBranchStatus("muonTruth_phi") ) { fChain->SetBranchAddress("muonTruth_phi", &muonTruth_phi, &b_muonTruth_phi); }
  if( fChain->GetBranchStatus("muonTruth_charge") ) { fChain->SetBranchAddress("muonTruth_charge", &muonTruth_charge, &b_muonTruth_charge); }
  if( fChain->GetBranchStatus("muonTruth_PDGID") ) { fChain->SetBranchAddress("muonTruth_PDGID", &muonTruth_PDGID, &b_muonTruth_PDGID); }
  if( fChain->GetBranchStatus("muonTruth_barcode") ) { fChain->SetBranchAddress("muonTruth_barcode", &muonTruth_barcode, &b_muonTruth_barcode); }
  if( fChain->GetBranchStatus("muonTruth_type") ) { fChain->SetBranchAddress("muonTruth_type", &muonTruth_type, &b_muonTruth_type); }
  if( fChain->GetBranchStatus("muonTruth_origin") ) { fChain->SetBranchAddress("muonTruth_origin", &muonTruth_origin, &b_muonTruth_origin); }
  if( fChain->GetBranchStatus("mcevt_n") ) { fChain->SetBranchAddress("mcevt_n", &mcevt_n, &b_mcevt_n); }
  if( fChain->GetBranchStatus("mcevt_signal_process_id") ) { fChain->SetBranchAddress("mcevt_signal_process_id", &mcevt_signal_process_id, &b_mcevt_signal_process_id); }
  if( fChain->GetBranchStatus("mcevt_event_number") ) { fChain->SetBranchAddress("mcevt_event_number", &mcevt_event_number, &b_mcevt_event_number); }
  if( fChain->GetBranchStatus("mcevt_event_scale") ) { fChain->SetBranchAddress("mcevt_event_scale", &mcevt_event_scale, &b_mcevt_event_scale); }
  if( fChain->GetBranchStatus("mcevt_alphaQCD") ) { fChain->SetBranchAddress("mcevt_alphaQCD", &mcevt_alphaQCD, &b_mcevt_alphaQCD); }
  if( fChain->GetBranchStatus("mcevt_alphaQED") ) { fChain->SetBranchAddress("mcevt_alphaQED", &mcevt_alphaQED, &b_mcevt_alphaQED); }
  if( fChain->GetBranchStatus("mcevt_pdf_id1") ) { fChain->SetBranchAddress("mcevt_pdf_id1", &mcevt_pdf_id1, &b_mcevt_pdf_id1); }
  if( fChain->GetBranchStatus("mcevt_pdf_id2") ) { fChain->SetBranchAddress("mcevt_pdf_id2", &mcevt_pdf_id2, &b_mcevt_pdf_id2); }
  if( fChain->GetBranchStatus("mcevt_pdf_x1") ) { fChain->SetBranchAddress("mcevt_pdf_x1", &mcevt_pdf_x1, &b_mcevt_pdf_x1); }
  if( fChain->GetBranchStatus("mcevt_pdf_x2") ) { fChain->SetBranchAddress("mcevt_pdf_x2", &mcevt_pdf_x2, &b_mcevt_pdf_x2); }
  if( fChain->GetBranchStatus("mcevt_pdf_scale") ) { fChain->SetBranchAddress("mcevt_pdf_scale", &mcevt_pdf_scale, &b_mcevt_pdf_scale); }
  if( fChain->GetBranchStatus("mcevt_pdf1") ) { fChain->SetBranchAddress("mcevt_pdf1", &mcevt_pdf1, &b_mcevt_pdf1); }
  if( fChain->GetBranchStatus("mcevt_pdf2") ) { fChain->SetBranchAddress("mcevt_pdf2", &mcevt_pdf2, &b_mcevt_pdf2); }
  if( fChain->GetBranchStatus("mcevt_weight") ) { fChain->SetBranchAddress("mcevt_weight", &mcevt_weight, &b_mcevt_weight); }
  if( fChain->GetBranchStatus("mc_n") ) { fChain->SetBranchAddress("mc_n", &mc_n, &b_mc_n); }
  if( fChain->GetBranchStatus("mc_pt") ) { fChain->SetBranchAddress("mc_pt", &mc_pt, &b_mc_pt); }
  if( fChain->GetBranchStatus("mc_m") ) { fChain->SetBranchAddress("mc_m", &mc_m, &b_mc_m); }
  if( fChain->GetBranchStatus("mc_eta") ) { fChain->SetBranchAddress("mc_eta", &mc_eta, &b_mc_eta); }
  if( fChain->GetBranchStatus("mc_phi") ) { fChain->SetBranchAddress("mc_phi", &mc_phi, &b_mc_phi); }
  if( fChain->GetBranchStatus("mc_status") ) { fChain->SetBranchAddress("mc_status", &mc_status, &b_mc_status); }
  if( fChain->GetBranchStatus("mc_barcode") ) { fChain->SetBranchAddress("mc_barcode", &mc_barcode, &b_mc_barcode); }
  if( fChain->GetBranchStatus("mc_parents") ) { fChain->SetBranchAddress("mc_parents", &mc_parents, &b_mc_parents); }
  if( fChain->GetBranchStatus("mc_children") ) { fChain->SetBranchAddress("mc_children", &mc_children, &b_mc_children); }
  if( fChain->GetBranchStatus("mc_pdgId") ) { fChain->SetBranchAddress("mc_pdgId", &mc_pdgId, &b_mc_pdgId); }
  if( fChain->GetBranchStatus("mc_charge") ) { fChain->SetBranchAddress("mc_charge", &mc_charge, &b_mc_charge); }
  if( fChain->GetBranchStatus("mc_vx_x") ) { fChain->SetBranchAddress("mc_vx_x", &mc_vx_x, &b_mc_vx_x); }
  if( fChain->GetBranchStatus("mc_vx_y") ) { fChain->SetBranchAddress("mc_vx_y", &mc_vx_y, &b_mc_vx_y); }
  if( fChain->GetBranchStatus("mc_vx_z") ) { fChain->SetBranchAddress("mc_vx_z", &mc_vx_z, &b_mc_vx_z); }
  if( fChain->GetBranchStatus("mc_child_index") ) { fChain->SetBranchAddress("mc_child_index", &mc_child_index, &b_mc_child_index); }
  if( fChain->GetBranchStatus("mc_parent_index") ) { fChain->SetBranchAddress("mc_parent_index", &mc_parent_index, &b_mc_parent_index); }

  if( fChain->GetBranchStatus("trig_L2_combmuonfeature_L2_mu18_MG") ) { fChain->SetBranchAddress("trig_L2_combmuonfeature_L2_mu18_MG", &trig_L2_combmuonfeature_L2_mu18_MG, &b_trig_L2_combmuonfeature_L2_mu18_MG); }
  if( fChain->GetBranchStatus("trig_L2_combmuonfeature_n") ) { fChain->SetBranchAddress("trig_L2_combmuonfeature_n", &trig_L2_combmuonfeature_n, &b_trig_L2_combmuonfeature_n); }
  if( fChain->GetBranchStatus("trig_L2_combmuonfeature_pt") ) { fChain->SetBranchAddress("trig_L2_combmuonfeature_pt", &trig_L2_combmuonfeature_pt, &b_trig_L2_combmuonfeature_pt); }
  if( fChain->GetBranchStatus("trig_L2_combmuonfeature_eta") ) { fChain->SetBranchAddress("trig_L2_combmuonfeature_eta", &trig_L2_combmuonfeature_eta, &b_trig_L2_combmuonfeature_eta); }
  if( fChain->GetBranchStatus("trig_L2_combmuonfeature_phi") ) { fChain->SetBranchAddress("trig_L2_combmuonfeature_phi", &trig_L2_combmuonfeature_phi, &b_trig_L2_combmuonfeature_phi); }
  if( fChain->GetBranchStatus("trig_EF_trigmugirl_n") ) { fChain->SetBranchAddress("trig_EF_trigmugirl_n", &trig_EF_trigmugirl_n, &b_trig_EF_trigmugirl_n); }
  if( fChain->GetBranchStatus("trig_EF_trigmugirl_track_n") ) { fChain->SetBranchAddress("trig_EF_trigmugirl_track_n", &trig_EF_trigmugirl_track_n, &b_trig_EF_trigmugirl_track_n); }
  if( fChain->GetBranchStatus("trig_EF_trigmugirl_track_CB_pt") ) { fChain->SetBranchAddress("trig_EF_trigmugirl_track_CB_pt", &trig_EF_trigmugirl_track_CB_pt, &b_trig_EF_trigmugirl_track_CB_pt); }
  if( fChain->GetBranchStatus("trig_EF_trigmugirl_track_CB_eta") ) { fChain->SetBranchAddress("trig_EF_trigmugirl_track_CB_eta", &trig_EF_trigmugirl_track_CB_eta, &b_trig_EF_trigmugirl_track_CB_eta); }
  if( fChain->GetBranchStatus("trig_EF_trigmugirl_track_CB_phi") ) { fChain->SetBranchAddress("trig_EF_trigmugirl_track_CB_phi", &trig_EF_trigmugirl_track_CB_phi, &b_trig_EF_trigmugirl_track_CB_phi); }
  if( fChain->GetBranchStatus("trig_L2_muonfeature_L2_mu40_MSonly_barrel") ) { fChain->SetBranchAddress("trig_L2_muonfeature_L2_mu40_MSonly_barrel", &trig_L2_muonfeature_L2_mu40_MSonly_barrel, &b_trig_L2_muonfeature_L2_mu40_MSonly_barrel); }
  if( fChain->GetBranchStatus("trig_L2_muonfeature_n") ) { fChain->SetBranchAddress("trig_L2_muonfeature_n", &trig_L2_muonfeature_n, &b_trig_L2_muonfeature_n); }
  if( fChain->GetBranchStatus("trig_L2_muonfeature_pt") ) { fChain->SetBranchAddress("trig_L2_muonfeature_pt", &trig_L2_muonfeature_pt, &b_trig_L2_muonfeature_pt); }
  if( fChain->GetBranchStatus("trig_L2_muonfeature_eta") ) { fChain->SetBranchAddress("trig_L2_muonfeature_eta", &trig_L2_muonfeature_eta, &b_trig_L2_muonfeature_eta); }
  if( fChain->GetBranchStatus("trig_L2_muonfeature_phi") ) { fChain->SetBranchAddress("trig_L2_muonfeature_phi", &trig_L2_muonfeature_phi, &b_trig_L2_muonfeature_phi); }
  if( fChain->GetBranchStatus("trig_EF_trigmuonef_n") ) { fChain->SetBranchAddress("trig_EF_trigmuonef_n", &trig_EF_trigmuonef_n, &b_trig_EF_trigmuonef_n); }
  if( fChain->GetBranchStatus("trig_EF_trigmuonef_track_n") ) { fChain->SetBranchAddress("trig_EF_trigmuonef_track_n", &trig_EF_trigmuonef_track_n, &b_trig_EF_trigmuonef_track_n); }
  if( fChain->GetBranchStatus("trig_EF_trigmuonef_track_SA_pt") ) { fChain->SetBranchAddress("trig_EF_trigmuonef_track_SA_pt", &trig_EF_trigmuonef_track_SA_pt, &b_trig_EF_trigmuonef_track_SA_pt); }
  if( fChain->GetBranchStatus("trig_EF_trigmuonef_track_SA_eta") ) { fChain->SetBranchAddress("trig_EF_trigmuonef_track_SA_eta", &trig_EF_trigmuonef_track_SA_eta, &b_trig_EF_trigmuonef_track_SA_eta); }
  if( fChain->GetBranchStatus("trig_EF_trigmuonef_track_SA_phi") ) { fChain->SetBranchAddress("trig_EF_trigmuonef_track_SA_phi", &trig_EF_trigmuonef_track_SA_phi, &b_trig_EF_trigmuonef_track_SA_phi); }
  if( fChain->GetBranchStatus("trig_EF_el_n") ) { fChain->SetBranchAddress("trig_EF_el_n", &trig_EF_el_n, &b_trig_EF_el_n); }
  if( fChain->GetBranchStatus("trig_EF_el_EF_e20_medium") ) { fChain->SetBranchAddress("trig_EF_el_EF_e20_medium", &trig_EF_el_EF_e20_medium, &b_trig_EF_el_EF_e20_medium); }
  if( fChain->GetBranchStatus("trig_EF_el_eta") ) { fChain->SetBranchAddress("trig_EF_el_eta", &trig_EF_el_eta, &b_trig_EF_el_eta); }
  if( fChain->GetBranchStatus("trig_EF_el_phi") ) { fChain->SetBranchAddress("trig_EF_el_phi", &trig_EF_el_phi, &b_trig_EF_el_phi); }

}
