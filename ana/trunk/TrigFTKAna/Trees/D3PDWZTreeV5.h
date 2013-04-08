//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Jan 31 17:58:55 2011 by ROOT version 5.22/00d
// from TChain physics/
//////////////////////////////////////////////////////////

#ifndef D3PDWZTreeV5_h
#define D3PDWZTreeV5_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <vector>
#include <string>
#include "TrigFTKAna/Trees/D3PDWZTreeBase.h"

class D3PDWZTreeV5 : public D3PDWZTreeBase {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   UInt_t          RunNumber;
   UInt_t          EventNumber;
   UInt_t          timestamp;
   UInt_t          timestamp_ns;
   UInt_t          lbn;
   UInt_t          bcid;
   UInt_t          detmask0;
   UInt_t          detmask1;
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
   std::vector<float>   *el_E;
   std::vector<float>   *el_Et;
   std::vector<float>   *el_pt;
   std::vector<float>   *el_m;
   std::vector<float>   *el_eta;
   std::vector<float>   *el_phi;
   std::vector<float>   *el_px;
   std::vector<float>   *el_py;
   std::vector<float>   *el_pz;
   std::vector<float>   *el_charge;
   std::vector<int>     *el_author;
   std::vector<unsigned int> *el_isEM;
   std::vector<unsigned int> *el_OQ;
   std::vector<int>     *el_convFlag;
   std::vector<int>     *el_isConv;
   std::vector<int>     *el_nConv;
   std::vector<int>     *el_nSingleTrackConv;
   std::vector<int>     *el_nDoubleTrackConv;
   std::vector<int>     *el_type;
   std::vector<int>     *el_origin;
   std::vector<float>   *el_truth_E;
   std::vector<float>   *el_truth_pt;
   std::vector<float>   *el_truth_eta;
   std::vector<float>   *el_truth_phi;
   std::vector<int>     *el_truth_type;
   std::vector<int>     *el_truth_status;
   std::vector<int>     *el_truth_barcode;
   std::vector<int>     *el_truth_mothertype;
   std::vector<int>     *el_truth_motherbarcode;
   std::vector<int>     *el_truth_hasHardBrem;
   std::vector<int>     *el_truth_index;
   std::vector<int>     *el_truth_matched;
   std::vector<int>     *el_loose;
   std::vector<int>     *el_medium;
   std::vector<int>     *el_mediumIso;
   std::vector<int>     *el_tight;
   std::vector<int>     *el_tightIso;
   std::vector<int>     *el_goodOQ;
   std::vector<float>   *el_Ethad;
   std::vector<float>   *el_Ethad1;
   std::vector<float>   *el_f1;
   std::vector<float>   *el_f1core;
   std::vector<float>   *el_Emins1;
   std::vector<float>   *el_fside;
   std::vector<float>   *el_Emax2;
   std::vector<float>   *el_ws3;
   std::vector<float>   *el_wstot;
   std::vector<float>   *el_emaxs1;
   std::vector<float>   *el_deltaEs;
   std::vector<float>   *el_E233;
   std::vector<float>   *el_E237;
   std::vector<float>   *el_E277;
   std::vector<float>   *el_weta2;
   std::vector<float>   *el_f3;
   std::vector<float>   *el_f3core;
   std::vector<float>   *el_rphiallcalo;
   std::vector<float>   *el_Etcone45;
   std::vector<float>   *el_Etcone20;
   std::vector<float>   *el_Etcone30;
   std::vector<float>   *el_Etcone40;
   std::vector<float>   *el_ptcone20;
   std::vector<float>   *el_ptcone30;
   std::vector<float>   *el_ptcone40;
   std::vector<float>   *el_nucone20;
   std::vector<float>   *el_nucone30;
   std::vector<float>   *el_nucone40;
   std::vector<float>   *el_convanglematch;
   std::vector<float>   *el_convtrackmatch;
   std::vector<int>     *el_hasconv;
   std::vector<float>   *el_convvtxx;
   std::vector<float>   *el_convvtxy;
   std::vector<float>   *el_convvtxz;
   std::vector<float>   *el_Rconv;
   std::vector<float>   *el_zconv;
   std::vector<float>   *el_convvtxchi2;
   std::vector<float>   *el_pt1conv;
   std::vector<int>     *el_convtrk1nBLHits;
   std::vector<int>     *el_convtrk1nPixHits;
   std::vector<int>     *el_convtrk1nSCTHits;
   std::vector<int>     *el_convtrk1nTRTHits;
   std::vector<float>   *el_pt2conv;
   std::vector<int>     *el_convtrk2nBLHits;
   std::vector<int>     *el_convtrk2nPixHits;
   std::vector<int>     *el_convtrk2nSCTHits;
   std::vector<int>     *el_convtrk2nTRTHits;
   std::vector<float>   *el_ptconv;
   std::vector<float>   *el_pzconv;
   std::vector<float>   *el_pos7;
   std::vector<float>   *el_etacorrmag;
   std::vector<float>   *el_deltaeta1;
   std::vector<float>   *el_deltaeta2;
   std::vector<float>   *el_deltaphi2;
   std::vector<float>   *el_deltaphiRescaled;
   std::vector<float>   *el_deltaPhiRot;
   std::vector<float>   *el_expectHitInBLayer;
   std::vector<float>   *el_reta;
   std::vector<float>   *el_rphi;
   std::vector<float>   *el_EtringnoisedR03sig2;
   std::vector<float>   *el_EtringnoisedR03sig3;
   std::vector<float>   *el_EtringnoisedR03sig4;
   std::vector<double>  *el_isolationlikelihoodjets;
   std::vector<double>  *el_isolationlikelihoodhqelectrons;
   std::vector<double>  *el_electronweight;
   std::vector<double>  *el_electronbgweight;
   std::vector<double>  *el_softeweight;
   std::vector<double>  *el_softebgweight;
   std::vector<double>  *el_neuralnet;
   std::vector<double>  *el_Hmatrix;
   std::vector<double>  *el_Hmatrix5;
   std::vector<double>  *el_adaboost;
   std::vector<double>  *el_softeneuralnet;
   std::vector<float>   *el_zvertex;
   std::vector<float>   *el_errz;
   std::vector<float>   *el_etap;
   std::vector<float>   *el_depth;
   std::vector<int>     *el_refittedTrack_n;
   std::vector<float>   *el_Es0;
   std::vector<float>   *el_etas0;
   std::vector<float>   *el_phis0;
   std::vector<float>   *el_Es1;
   std::vector<float>   *el_etas1;
   std::vector<float>   *el_phis1;
   std::vector<float>   *el_Es2;
   std::vector<float>   *el_etas2;
   std::vector<float>   *el_phis2;
   std::vector<float>   *el_Es3;
   std::vector<float>   *el_etas3;
   std::vector<float>   *el_phis3;
   std::vector<float>   *el_E_PreSamplerB;
   std::vector<float>   *el_E_EMB1;
   std::vector<float>   *el_E_EMB2;
   std::vector<float>   *el_E_EMB3;
   std::vector<float>   *el_E_PreSamplerE;
   std::vector<float>   *el_E_EME1;
   std::vector<float>   *el_E_EME2;
   std::vector<float>   *el_E_EME3;
   std::vector<float>   *el_E_HEC0;
   std::vector<float>   *el_E_HEC1;
   std::vector<float>   *el_E_HEC2;
   std::vector<float>   *el_E_HEC3;
   std::vector<float>   *el_E_TileBar0;
   std::vector<float>   *el_E_TileBar1;
   std::vector<float>   *el_E_TileBar2;
   std::vector<float>   *el_E_TileGap1;
   std::vector<float>   *el_E_TileGap2;
   std::vector<float>   *el_E_TileGap3;
   std::vector<float>   *el_E_TileExt0;
   std::vector<float>   *el_E_TileExt1;
   std::vector<float>   *el_E_TileExt2;
   std::vector<float>   *el_E_FCAL0;
   std::vector<float>   *el_E_FCAL1;
   std::vector<float>   *el_E_FCAL2;
   std::vector<float>   *el_cl_E;
   std::vector<float>   *el_cl_pt;
   std::vector<float>   *el_cl_eta;
   std::vector<float>   *el_cl_phi;
   std::vector<float>   *el_firstEdens;
   std::vector<float>   *el_cellmaxfrac;
   std::vector<float>   *el_longitudinal;
   std::vector<float>   *el_secondlambda;
   std::vector<float>   *el_lateral;
   std::vector<float>   *el_secondR;
   std::vector<float>   *el_centerlambda;
   std::vector<float>   *el_rawcl_Es0;
   std::vector<float>   *el_rawcl_etas0;
   std::vector<float>   *el_rawcl_phis0;
   std::vector<float>   *el_rawcl_Es1;
   std::vector<float>   *el_rawcl_etas1;
   std::vector<float>   *el_rawcl_phis1;
   std::vector<float>   *el_rawcl_Es2;
   std::vector<float>   *el_rawcl_etas2;
   std::vector<float>   *el_rawcl_phis2;
   std::vector<float>   *el_rawcl_Es3;
   std::vector<float>   *el_rawcl_etas3;
   std::vector<float>   *el_rawcl_phis3;
   std::vector<float>   *el_rawcl_E;
   std::vector<float>   *el_rawcl_pt;
   std::vector<float>   *el_rawcl_eta;
   std::vector<float>   *el_rawcl_phi;
   std::vector<float>   *el_refittedtrackcovd0;
   std::vector<float>   *el_refittedtrackcovz0;
   std::vector<float>   *el_refittedtrackcovphi;
   std::vector<float>   *el_refittedtrackcovtheta;
   std::vector<float>   *el_refittedtrackcovqoverp;
   std::vector<float>   *el_refittedtrackcovd0z0;
   std::vector<float>   *el_refittedtrackcovz0phi;
   std::vector<float>   *el_refittedtrackcovz0theta;
   std::vector<float>   *el_refittedtrackcovz0qoverp;
   std::vector<float>   *el_refittedtrackcovd0phi;
   std::vector<float>   *el_refittedtrackcovd0theta;
   std::vector<float>   *el_refittedtrackcovd0qoverp;
   std::vector<float>   *el_refittedtrackcovphitheta;
   std::vector<float>   *el_refittedtrackcovphiqoverp;
   std::vector<float>   *el_refittedtrackcovthetaqoverp;
   std::vector<float>   *el_trackd0;
   std::vector<float>   *el_trackz0;
   std::vector<float>   *el_trackphi;
   std::vector<float>   *el_tracktheta;
   std::vector<float>   *el_trackqoverp;
   std::vector<float>   *el_trackpt;
   std::vector<float>   *el_tracketa;
   std::vector<float>   *el_trackcov_d0;
   std::vector<float>   *el_trackcov_z0;
   std::vector<float>   *el_trackcov_phi;
   std::vector<float>   *el_trackcov_theta;
   std::vector<float>   *el_trackcov_qoverp;
   std::vector<float>   *el_trackcov_d0_z0;
   std::vector<float>   *el_trackcov_d0_phi;
   std::vector<float>   *el_trackcov_d0_theta;
   std::vector<float>   *el_trackcov_d0_qoverp;
   std::vector<float>   *el_trackcov_z0_phi;
   std::vector<float>   *el_trackcov_z0_theta;
   std::vector<float>   *el_trackcov_z0_qoverp;
   std::vector<float>   *el_trackcov_phi_theta;
   std::vector<float>   *el_trackcov_phi_qoverp;
   std::vector<float>   *el_trackcov_theta_qoverp;
   std::vector<float>   *el_trackfitchi2;
   std::vector<int>     *el_trackfitndof;
   std::vector<int>     *el_nBLHits;
   std::vector<int>     *el_nPixHits;
   std::vector<int>     *el_nSCTHits;
   std::vector<int>     *el_nTRTHits;
   std::vector<int>     *el_nPixHoles;
   std::vector<int>     *el_nSCTHoles;
   std::vector<int>     *el_nBLSharedHits;
   std::vector<int>     *el_nPixSharedHits;
   std::vector<int>     *el_nSCTSharedHits;
   std::vector<int>     *el_nTRTHighTHits;
   std::vector<int>     *el_nTRTOutliers;
   std::vector<int>     *el_nTRTHighTOutliers;
   std::vector<int>     *el_nSiHits;
   std::vector<float>   *el_TRTHighTHitsRatio;
   std::vector<float>   *el_pixeldEdx;
   std::vector<float>   *el_eProbabilityComb;
   std::vector<float>   *el_eProbabilityHT;
   std::vector<float>   *el_eProbabilityToT;
   std::vector<float>   *el_eProbabilityBrem;
   std::vector<float>   *el_vertx;
   std::vector<float>   *el_verty;
   std::vector<float>   *el_vertz;
   std::vector<int>     *el_hastrack;
   std::vector<float>   *el_deltaEmax2;
   std::vector<float>   *el_calibHitsShowerDepth;
   std::vector<float>   *el_trackd0beam;
   std::vector<float>   *el_tracksigd0beam;
   std::vector<float>   *el_trackd0pv;
   std::vector<float>   *el_tracksigd0pv;
   std::vector<float>   *el_trackz0pv;
   std::vector<float>   *el_tracksigz0pv;
   std::vector<float>   *el_trackd0pvunbiased;
   std::vector<float>   *el_tracksigd0pvunbiased;
   std::vector<float>   *el_trackz0pvunbiased;
   std::vector<float>   *el_tracksigz0pvunbiased;
   std::vector<unsigned int> *el_isIso;
   std::vector<float>   *el_mvaptcone20;
   std::vector<float>   *el_mvaptcone30;
   std::vector<float>   *el_mvaptcone40;
   std::vector<float>   *el_jet_dr;
   std::vector<float>   *el_jet_E;
   std::vector<float>   *el_jet_pt;
   std::vector<float>   *el_jet_m;
   std::vector<float>   *el_jet_eta;
   std::vector<float>   *el_jet_phi;
   std::vector<float>   *el_jet_truth_dr;
   std::vector<float>   *el_jet_truth_E;
   std::vector<float>   *el_jet_truth_pt;
   std::vector<float>   *el_jet_truth_m;
   std::vector<float>   *el_jet_truth_eta;
   std::vector<float>   *el_jet_truth_phi;
   std::vector<int>     *el_jet_truth_matched;
   std::vector<int>     *el_jet_matched;
   std::vector<float>   *el_Etcone40_pt_corrected;
   std::vector<float>   *el_Etcone40_ED_corrected;
   std::vector<float>   *el_Etcone40_corrected;
   std::vector<float>   *el_EF_dr;
   std::vector<int>     *el_EF_index;
   std::vector<float>   *el_L2_dr;
   std::vector<int>     *el_L2_index;
   std::vector<float>   *el_L1_dr;
   std::vector<int>     *el_L1_index;
   Bool_t          EF_2e10_loose;
   Bool_t          EF_2e10_medium;
   Bool_t          EF_2e15_loose;
   Bool_t          EF_2e3_loose;
   Bool_t          EF_2e3_loose_SiTrk;
   Bool_t          EF_2e3_loose_TRT;
   Bool_t          EF_2e3_medium;
   Bool_t          EF_2e3_medium_SiTrk;
   Bool_t          EF_2e3_medium_TRT;
   Bool_t          EF_2e3_tight;
   Bool_t          EF_2e5_medium;
   Bool_t          EF_2e5_medium_SiTrk;
   Bool_t          EF_2e5_medium_TRT;
   Bool_t          EF_2e5_tight;
   Bool_t          EF_2g10_loose;
   Bool_t          EF_2g15_loose;
   Bool_t          EF_2g20_loose;
   Bool_t          EF_2g5_loose;
   Bool_t          EF_2g7_loose;
   Bool_t          EF_2j35_jetNoEF_xe20_noMu;
   Bool_t          EF_2j35_jetNoEF_xe30_noMu;
   Bool_t          EF_2j35_jetNoEF_xe40_noMu;
   Bool_t          EF_2j35_xe20_noMu;
   Bool_t          EF_2j35_xe30_noMu;
   Bool_t          EF_2j35_xe40_noMu;
   Bool_t          EF_2mu0_NoAlg;
   Bool_t          EF_2mu10;
   Bool_t          EF_2mu10_NoAlg;
   Bool_t          EF_2mu4;
   Bool_t          EF_2mu4_Bmumu;
   Bool_t          EF_2mu4_Bmumux;
   Bool_t          EF_2mu4_DiMu;
   Bool_t          EF_2mu4_DiMu_SiTrk;
   Bool_t          EF_2mu4_DiMu_noVtx;
   Bool_t          EF_2mu4_DiMu_noVtx_noOS;
   Bool_t          EF_2mu4_Jpsimumu;
   Bool_t          EF_2mu4_Jpsimumu_IDTrkNoCut;
   Bool_t          EF_2mu4_Upsimumu;
   Bool_t          EF_2mu6;
   Bool_t          EF_2mu6_Bmumu;
   Bool_t          EF_2mu6_Bmumux;
   Bool_t          EF_2mu6_DiMu;
   Bool_t          EF_2mu6_Jpsimumu;
   Bool_t          EF_2mu6_MG;
   Bool_t          EF_2mu6_NoAlg;
   Bool_t          EF_2mu6_Upsimumu;
   Bool_t          EF_2mu6_g10_loose;
   Bool_t          EF_2tau12_loose;
   Bool_t          EF_2tau16_loose;
   Bool_t          EF_2tau20_loose;
   Bool_t          EF_2tau29_loose;
   Bool_t          EF_2tau29_loose1;
   Bool_t          EF_2tau38_loose;
   Bool_t          EF_e10_NoCut;
   Bool_t          EF_e10_loose;
   Bool_t          EF_e10_loose_mu10;
   Bool_t          EF_e10_loose_mu6;
   Bool_t          EF_e10_medium;
   Bool_t          EF_e10_medium_IDTrkNoCut;
   Bool_t          EF_e10_medium_SiTrk;
   Bool_t          EF_e10_medium_TRT;
   Bool_t          EF_e10_tight;
   Bool_t          EF_e10_tight_TRT;
   Bool_t          EF_e15_loose;
   Bool_t          EF_e15_loose_IDTrkNoCut;
   Bool_t          EF_e15_medium;
   Bool_t          EF_e15_medium_SiTrk;
   Bool_t          EF_e15_medium_TRT;
   Bool_t          EF_e15_tight;
   Bool_t          EF_e15_tight_TRT;
   Bool_t          EF_e18_medium;
   Bool_t          EF_e20_loose;
   Bool_t          EF_e20_loose_IDTrkNoCut;
   Bool_t          EF_e20_loose_TRT;
   Bool_t          EF_e20_loose_passEF;
   Bool_t          EF_e20_loose_passL2;
   Bool_t          EF_e20_loose_xe20;
   Bool_t          EF_e20_loose_xe20_noMu;
   Bool_t          EF_e20_loose_xe30;
   Bool_t          EF_e20_loose_xe30_noMu;
   Bool_t          EF_e20_medium;
   Bool_t          EF_e25_loose;
   Bool_t          EF_e25_medium;
   Bool_t          EF_e30_loose;
   Bool_t          EF_e3_loose;
   Bool_t          EF_e3_loose_SiTrk;
   Bool_t          EF_e3_loose_TRT;
   Bool_t          EF_e3_medium;
   Bool_t          EF_e3_medium_SiTrk;
   Bool_t          EF_e3_medium_TRT;
   Bool_t          EF_e5_NoCut_L2SW;
   Bool_t          EF_e5_NoCut_Ringer;
   Bool_t          EF_e5_NoCut_firstempty;
   Bool_t          EF_e5_loose_mu4;
   Bool_t          EF_e5_medium;
   Bool_t          EF_e5_medium_MV;
   Bool_t          EF_e5_medium_SiTrk;
   Bool_t          EF_e5_medium_TRT;
   Bool_t          EF_e5_medium_mu4;
   Bool_t          EF_e5_tight;
   Bool_t          EF_e5_tight_SiTrk;
   Bool_t          EF_e5_tight_TRT;
   Bool_t          EF_e5_tight_e5_NoCut;
   Bool_t          EF_em105_passHLT;
   Bool_t          EF_g10_loose;
   Bool_t          EF_g10_loose_larcalib;
   Bool_t          EF_g11_etcut;
   Bool_t          EF_g15_loose;
   Bool_t          EF_g17_etcut;
   Bool_t          EF_g17_etcut_EFxe20_noMu;
   Bool_t          EF_g17_etcut_EFxe30_noMu;
   Bool_t          EF_g20_loose;
   Bool_t          EF_g20_loose_cnv;
   Bool_t          EF_g20_loose_larcalib;
   Bool_t          EF_g20_loose_xe20_noMu;
   Bool_t          EF_g20_loose_xe30_noMu;
   Bool_t          EF_g20_tight;
   Bool_t          EF_g25_loose_xe30_noMu;
   Bool_t          EF_g30_loose;
   Bool_t          EF_g30_tight;
   Bool_t          EF_g3_NoCut_unpaired_iso;
   Bool_t          EF_g3_NoCut_unpaired_noniso;
   Bool_t          EF_g40_loose;
   Bool_t          EF_g40_loose_larcalib;
   Bool_t          EF_g40_tight;
   Bool_t          EF_g50_loose;
   Bool_t          EF_g50_loose_larcalib;
   Bool_t          EF_g5_NoCut_cosmic;
   Bool_t          EF_g5_loose;
   Bool_t          EF_g5_loose_cnv;
   Bool_t          EF_g5_loose_larcalib;
   Bool_t          EF_g7_loose;
   Bool_t          EF_j115_jetNoCut_larcalib;
   Bool_t          EF_j20;
   Bool_t          EF_j20_jetNoEF;
   Bool_t          EF_j20_jetNoEF_larcalib;
   Bool_t          EF_j20a4;
   Bool_t          EF_j20a4fs;
   Bool_t          EF_j20a6;
   Bool_t          EF_j20a6fs;
   Bool_t          EF_j30;
   Bool_t          EF_j30_cosmic;
   Bool_t          EF_j30_firstempty;
   Bool_t          EF_j30_fj30_jetNoEF;
   Bool_t          EF_j30_jetNoCut_cosmic;
   Bool_t          EF_j30_jetNoCut_firstempty;
   Bool_t          EF_j30_jetNoCut_unpaired;
   Bool_t          EF_j30_jetNoEF;
   Bool_t          EF_j30_jetNoEF_cosmic;
   Bool_t          EF_j30_jetNoEF_firstempty;
   Bool_t          EF_j30_jetNoEF_unpaired;
   Bool_t          EF_j30_unpaired;
   Bool_t          EF_j35;
   Bool_t          EF_j35_L1TAU_HV;
   Bool_t          EF_j35_L1TAU_HV_jetNoEF;
   Bool_t          EF_j35_jetNoCut_xe30_e15_medium;
   Bool_t          EF_j35_jetNoCut_xe30_mu15;
   Bool_t          EF_j35_jetNoEF;
   Bool_t          EF_j35_xe30_e15_medium;
   Bool_t          EF_j35_xe30_mu15;
   Bool_t          EF_j50;
   Bool_t          EF_j50_cosmic;
   Bool_t          EF_j50_firstempty;
   Bool_t          EF_j50_jetNoCut_cosmic;
   Bool_t          EF_j50_jetNoCut_firstempty;
   Bool_t          EF_j50_jetNoCut_unpaired;
   Bool_t          EF_j50_jetNoEF;
   Bool_t          EF_j50_jetNoEF_cosmic;
   Bool_t          EF_j50_jetNoEF_firstempty;
   Bool_t          EF_j50_jetNoEF_larcalib;
   Bool_t          EF_j50_jetNoEF_unpaired;
   Bool_t          EF_j50_jetNoEF_xe20_noMu;
   Bool_t          EF_j50_jetNoEF_xe30_noMu;
   Bool_t          EF_j50_jetNoEF_xe40_noMu;
   Bool_t          EF_j50_unpaired;
   Bool_t          EF_j50_xe20_noMu;
   Bool_t          EF_j50_xe30_noMu;
   Bool_t          EF_j50_xe40_noMu;
   Bool_t          EF_j5_empty_NoAlg;
   Bool_t          EF_j5_unpaired_iso_NoAlg;
   Bool_t          EF_j5_unpaired_noniso_NoAlg;
   Bool_t          EF_j75;
   Bool_t          EF_j75_jetNoEF;
   Bool_t          EF_j75_jetNoEF_EFxe20_noMu;
   Bool_t          EF_j75_jetNoEF_EFxe25_noMu;
   Bool_t          EF_j75_jetNoEF_EFxe30_noMu;
   Bool_t          EF_j75_jetNoEF_EFxe35_noMu;
   Bool_t          EF_j75_jetNoEF_EFxe40_noMu;
   Bool_t          EF_j95;
   Bool_t          EF_j95_jetNoEF;
   Bool_t          EF_j95_jetNoEF_larcalib;
   Bool_t          EF_je135_NoEF;
   Bool_t          EF_je195_NoEF;
   Bool_t          EF_je255_NoEF;
   Bool_t          EF_je300_NoEF;
   Bool_t          EF_je350_NoEF;
   Bool_t          EF_mu0_NoAlg;
   Bool_t          EF_mu0_comm_NoAlg;
   Bool_t          EF_mu0_comm_empty_NoAlg;
   Bool_t          EF_mu0_comm_firstempty_NoAlg;
   Bool_t          EF_mu0_comm_unpaired_iso_NoAlg;
   Bool_t          EF_mu0_comm_unpaired_noniso_NoAlg;
   Bool_t          EF_mu0_empty_NoAlg;
   Bool_t          EF_mu0_firstempty_NoAlg;
   Bool_t          EF_mu0_missingRoi;
   Bool_t          EF_mu0_outOfTime1;
   Bool_t          EF_mu0_outOfTime2;
   Bool_t          EF_mu0_rpcOnly;
   Bool_t          EF_mu0_unpaired_iso_NoAlg;
   Bool_t          EF_mu0_unpaired_noniso_NoAlg;
   Bool_t          EF_mu10;
   Bool_t          EF_mu10_Ecut12;
   Bool_t          EF_mu10_Ecut123;
   Bool_t          EF_mu10_Ecut13;
   Bool_t          EF_mu10_IDTrkNoCut;
   Bool_t          EF_mu10_MG;
   Bool_t          EF_mu10_MSonly;
   Bool_t          EF_mu10_MSonly_Ecut12;
   Bool_t          EF_mu10_MSonly_Ecut123;
   Bool_t          EF_mu10_MSonly_Ecut13;
   Bool_t          EF_mu10_MSonly_tight;
   Bool_t          EF_mu10_NoAlg;
   Bool_t          EF_mu10_SiTrk;
   Bool_t          EF_mu10_j30;
   Bool_t          EF_mu10_tight;
   Bool_t          EF_mu10i_loose;
   Bool_t          EF_mu13;
   Bool_t          EF_mu13_MG;
   Bool_t          EF_mu13_MG_tight;
   Bool_t          EF_mu13_tight;
   Bool_t          EF_mu15;
   Bool_t          EF_mu15_MG;
   Bool_t          EF_mu15_NoAlg;
   Bool_t          EF_mu18;
   Bool_t          EF_mu18_MG;
   Bool_t          EF_mu20;
   Bool_t          EF_mu20_MSonly;
   Bool_t          EF_mu20_NoAlg;
   Bool_t          EF_mu20_passHLT;
   Bool_t          EF_mu20_slow;
   Bool_t          EF_mu30_MSonly;
   Bool_t          EF_mu4;
   Bool_t          EF_mu40_MSonly;
   Bool_t          EF_mu40_MSonly_barrel;
   Bool_t          EF_mu4_Bmumu;
   Bool_t          EF_mu4_BmumuX;
   Bool_t          EF_mu4_DiMu;
   Bool_t          EF_mu4_DiMu_FS;
   Bool_t          EF_mu4_DiMu_FS_noOS;
   Bool_t          EF_mu4_DiMu_MG;
   Bool_t          EF_mu4_DiMu_MG_FS;
   Bool_t          EF_mu4_DiMu_SiTrk;
   Bool_t          EF_mu4_DiMu_SiTrk_FS;
   Bool_t          EF_mu4_DiMu_noOS;
   Bool_t          EF_mu4_IDTrkNoCut;
   Bool_t          EF_mu4_Jpsie5e3;
   Bool_t          EF_mu4_Jpsie5e3_FS;
   Bool_t          EF_mu4_Jpsie5e3_SiTrk;
   Bool_t          EF_mu4_Jpsie5e3_SiTrk_FS;
   Bool_t          EF_mu4_Jpsimumu;
   Bool_t          EF_mu4_Jpsimumu_FS;
   Bool_t          EF_mu4_Jpsimumu_SiTrk_FS;
   Bool_t          EF_mu4_L1J10_matched;
   Bool_t          EF_mu4_L1J15_matched;
   Bool_t          EF_mu4_L1J30_matched;
   Bool_t          EF_mu4_L1J55_matched;
   Bool_t          EF_mu4_L1J5_matched;
   Bool_t          EF_mu4_L2MSonly_EFFS_passL2;
   Bool_t          EF_mu4_MG;
   Bool_t          EF_mu4_MSonly;
   Bool_t          EF_mu4_MSonly_EFFS_passL2;
   Bool_t          EF_mu4_MSonly_MB2_noL2_EFFS;
   Bool_t          EF_mu4_MSonly_cosmic;
   Bool_t          EF_mu4_MSonly_outOfTime;
   Bool_t          EF_mu4_MV;
   Bool_t          EF_mu4_SiTrk;
   Bool_t          EF_mu4_Trk_Jpsi;
   Bool_t          EF_mu4_Trk_Jpsi_FS;
   Bool_t          EF_mu4_Trk_Jpsi_loose;
   Bool_t          EF_mu4_Trk_Upsi_FS;
   Bool_t          EF_mu4_Trk_Upsi_loose_FS;
   Bool_t          EF_mu4_Upsimumu_FS;
   Bool_t          EF_mu4_Upsimumu_SiTrk_FS;
   Bool_t          EF_mu4_comm_MSonly_cosmic;
   Bool_t          EF_mu4_comm_cosmic;
   Bool_t          EF_mu4_comm_firstempty;
   Bool_t          EF_mu4_comm_unpaired_iso;
   Bool_t          EF_mu4_cosmic;
   Bool_t          EF_mu4_firstempty;
   Bool_t          EF_mu4_j20;
   Bool_t          EF_mu4_j20_jetNoEF;
   Bool_t          EF_mu4_j30;
   Bool_t          EF_mu4_j30_jetNoEF;
   Bool_t          EF_mu4_mu6;
   Bool_t          EF_mu4_muCombTag;
   Bool_t          EF_mu4_tile;
   Bool_t          EF_mu4_tile_cosmic;
   Bool_t          EF_mu4_unpaired_iso;
   Bool_t          EF_mu4mu6_Bmumu;
   Bool_t          EF_mu4mu6_BmumuX;
   Bool_t          EF_mu4mu6_DiMu;
   Bool_t          EF_mu4mu6_Jpsimumu;
   Bool_t          EF_mu4mu6_Jpsimumu_IDTrkNoCut;
   Bool_t          EF_mu4mu6_Upsimumu;
   Bool_t          EF_mu6;
   Bool_t          EF_mu6_Bmumu;
   Bool_t          EF_mu6_BmumuX;
   Bool_t          EF_mu6_DiMu;
   Bool_t          EF_mu6_Ecut12;
   Bool_t          EF_mu6_Ecut123;
   Bool_t          EF_mu6_Ecut13;
   Bool_t          EF_mu6_Ecut2;
   Bool_t          EF_mu6_Ecut3;
   Bool_t          EF_mu6_IDTrkNoCut;
   Bool_t          EF_mu6_Jpsie5e3;
   Bool_t          EF_mu6_Jpsie5e3_FS;
   Bool_t          EF_mu6_Jpsie5e3_SiTrk;
   Bool_t          EF_mu6_Jpsie5e3_SiTrk_FS;
   Bool_t          EF_mu6_Jpsimumu;
   Bool_t          EF_mu6_MG;
   Bool_t          EF_mu6_MSonly;
   Bool_t          EF_mu6_MSonly_Ecut12;
   Bool_t          EF_mu6_MSonly_Ecut123;
   Bool_t          EF_mu6_MSonly_Ecut13;
   Bool_t          EF_mu6_MSonly_Ecut2;
   Bool_t          EF_mu6_MSonly_Ecut3;
   Bool_t          EF_mu6_MSonly_outOfTime;
   Bool_t          EF_mu6_NoAlg;
   Bool_t          EF_mu6_SiTrk;
   Bool_t          EF_mu6_Trk_Jpsi;
   Bool_t          EF_mu6_Upsimumu_FS;
   Bool_t          EF_mu6_muCombTag;
   Bool_t          EF_tau125_loose;
   Bool_t          EF_tau125_medium;
   Bool_t          EF_tau12_IDTrkNoCut;
   Bool_t          EF_tau12_loose;
   Bool_t          EF_tau12_loose_2b15;
   Bool_t          EF_tau12_loose_3j35;
   Bool_t          EF_tau12_loose_3j35_jetNoEF;
   Bool_t          EF_tau12_loose_EFxe12_noMu;
   Bool_t          EF_tau12_loose_IdScan_EFxe12_noMu;
   Bool_t          EF_tau12_loose_IdScan_xe15_noMu;
   Bool_t          EF_tau12_loose_e10_loose;
   Bool_t          EF_tau12_loose_e10_medium;
   Bool_t          EF_tau12_loose_e10_tight;
   Bool_t          EF_tau12_loose_mu10;
   Bool_t          EF_tau12_loose_xe15_noMu;
   Bool_t          EF_tau12_loose_xe20_noMu;
   Bool_t          EF_tau16_loose;
   Bool_t          EF_tau16_loose_2b15;
   Bool_t          EF_tau16_loose_3j35;
   Bool_t          EF_tau16_loose_3j35_jetNoEF;
   Bool_t          EF_tau16_loose_e10_loose;
   Bool_t          EF_tau16_loose_e15_loose;
   Bool_t          EF_tau16_loose_mu10;
   Bool_t          EF_tau16_loose_mu15;
   Bool_t          EF_tau16_loose_xe20_noMu;
   Bool_t          EF_tau16_loose_xe25_noMu;
   Bool_t          EF_tau16_loose_xe25_tight_noMu;
   Bool_t          EF_tau16_loose_xe30_noMu;
   Bool_t          EF_tau16_medium;
   Bool_t          EF_tau16_medium_xe22_noMu;
   Bool_t          EF_tau16_medium_xe25_noMu;
   Bool_t          EF_tau16_medium_xe25_tight_noMu;
   Bool_t          EF_tau20_loose;
   Bool_t          EF_tau20_loose_xe25_noMu;
   Bool_t          EF_tau29_loose;
   Bool_t          EF_tau29_loose1;
   Bool_t          EF_tau38_loose;
   Bool_t          EF_tau38_medium;
   Bool_t          EF_tau50_IDTrkNoCut;
   Bool_t          EF_tau50_loose;
   Bool_t          EF_tau50_loose_IdScan;
   Bool_t          EF_tau50_medium;
   Bool_t          EF_tau84_loose;
   Bool_t          EF_tau84_medium;
   Bool_t          EF_tauNoCut;
   Bool_t          EF_tauNoCut_IdScan;
   Bool_t          EF_tauNoCut_cosmic;
   Bool_t          EF_tauNoCut_firstempty;
   Bool_t          EF_tauNoCut_hasTrk6_EFxe15_noMu;
   Bool_t          EF_tauNoCut_hasTrk6_IdScan_EFxe15_noMu;
   Bool_t          EF_tauNoCut_hasTrk6_IdScan_xe20_noMu;
   Bool_t          EF_tauNoCut_hasTrk6_xe20_noMu;
   Bool_t          EF_tauNoCut_hasTrk9_xe20_noMu;
   Bool_t          EF_tauNoCut_hasTrk_MV;
   Bool_t          EF_tauNoCut_hasTrk_e10_tight;
   Bool_t          EF_tauNoCut_hasTrk_xe20_noMu;
   Bool_t          EF_tauNoCut_unpaired_iso;
   Bool_t          EF_tauNoCut_unpaired_noniso;
   Bool_t          EF_xe15;
   Bool_t          EF_xe15_noMu;
   Bool_t          EF_xe15_unbiased_noMu;
   Bool_t          EF_xe20;
   Bool_t          EF_xe20_noMu;
   Bool_t          EF_xe20_tight_noMu;
   Bool_t          EF_xe20_tight_vfj_noMu;
   Bool_t          EF_xe25;
   Bool_t          EF_xe25_medium;
   Bool_t          EF_xe25_medium_noMu;
   Bool_t          EF_xe25_noMu;
   Bool_t          EF_xe25_tight_noMu;
   Bool_t          EF_xe25_tight_vfj_noMu;
   Bool_t          EF_xe25_vfj_noMu;
   Bool_t          EF_xe30;
   Bool_t          EF_xe30_allL1;
   Bool_t          EF_xe30_allL1_FEB;
   Bool_t          EF_xe30_allL1_allCells;
   Bool_t          EF_xe30_allL1_noMu;
   Bool_t          EF_xe30_loose;
   Bool_t          EF_xe30_loose_noMu;
   Bool_t          EF_xe30_medium;
   Bool_t          EF_xe30_medium_noMu;
   Bool_t          EF_xe30_medium_vfj_noMu;
   Bool_t          EF_xe30_noMu;
   Bool_t          EF_xe30_tight_noMu;
   Bool_t          EF_xe30_tight_vfj_noMu;
   Bool_t          EF_xe30_vfj_noMu;
   Bool_t          EF_xe35;
   Bool_t          EF_xe35_loose_noMu;
   Bool_t          EF_xe35_noMu;
   Bool_t          EF_xe35_tight_noMu;
   Bool_t          EF_xe40;
   Bool_t          EF_xe40_noMu;
   Bool_t          EF_xe40_tight_noMu;
   Bool_t          EF_xe45;
   Bool_t          EF_xe45_noMu;
   Bool_t          EF_xe55;
   Bool_t          EF_xe55_noMu;
   Bool_t          EF_xe60_medium;
   Bool_t          EF_xe60_medium_noMu;
   Bool_t          EF_xe80_medium;
   Bool_t          EF_xe80_medium_noMu;
   Bool_t          L1_2EM10;
   Bool_t          L1_2EM14;
   Bool_t          L1_2EM2;
   Bool_t          L1_2EM3;
   Bool_t          L1_2EM5;
   Bool_t          L1_2J15_XE10;
   Bool_t          L1_2J15_XE15;
   Bool_t          L1_2J15_XE25;
   Bool_t          L1_2MU0;
   Bool_t          L1_2MU0_FIRSTEMPTY;
   Bool_t          L1_2MU0_MU6;
   Bool_t          L1_2MU10;
   Bool_t          L1_2MU20;
   Bool_t          L1_2MU6;
   Bool_t          L1_2MU6_EM5;
   Bool_t          L1_2TAU11;
   Bool_t          L1_2TAU5;
   Bool_t          L1_2TAU5_EM5;
   Bool_t          L1_2TAU6;
   Bool_t          L1_2TAU6_EM10;
   Bool_t          L1_2TAU6_EM5;
   Bool_t          L1_EM10;
   Bool_t          L1_EM10I;
   Bool_t          L1_EM14;
   Bool_t          L1_EM14I;
   Bool_t          L1_EM14_XE10;
   Bool_t          L1_EM14_XE15;
   Bool_t          L1_EM2;
   Bool_t          L1_EM2_UNPAIRED_ISO;
   Bool_t          L1_EM2_UNPAIRED_NONISO;
   Bool_t          L1_EM3;
   Bool_t          L1_EM3_EMPTY;
   Bool_t          L1_EM3_FIRSTEMPTY;
   Bool_t          L1_EM3_MV;
   Bool_t          L1_EM5;
   Bool_t          L1_EM5_MU10;
   Bool_t          L1_EM5_MU6;
   Bool_t          L1_EM85;
   Bool_t          L1_J15_XE15_EM10;
   Bool_t          L1_J15_XE15_MU15;
   Bool_t          L1_J30_XE10;
   Bool_t          L1_J30_XE15;
   Bool_t          L1_J30_XE25;
   Bool_t          L1_MU0;
   Bool_t          L1_MU0_COMM;
   Bool_t          L1_MU0_COMM_EMPTY;
   Bool_t          L1_MU0_COMM_FIRSTEMPTY;
   Bool_t          L1_MU0_COMM_UNPAIRED_ISO;
   Bool_t          L1_MU0_COMM_UNPAIRED_NONISO;
   Bool_t          L1_MU0_EM3;
   Bool_t          L1_MU0_EMPTY;
   Bool_t          L1_MU0_FIRSTEMPTY;
   Bool_t          L1_MU0_J10;
   Bool_t          L1_MU0_J15;
   Bool_t          L1_MU0_J30;
   Bool_t          L1_MU0_J5;
   Bool_t          L1_MU0_J55;
   Bool_t          L1_MU0_MV;
   Bool_t          L1_MU0_UNPAIRED_ISO;
   Bool_t          L1_MU0_UNPAIRED_NONISO;
   Bool_t          L1_MU10;
   Bool_t          L1_MU10_FIRSTEMPTY;
   Bool_t          L1_MU10_J10;
   Bool_t          L1_MU15;
   Bool_t          L1_MU20;
   Bool_t          L1_MU6;
   Bool_t          L1_MU6_EM3;
   Bool_t          L1_MU6_FIRSTEMPTY;
   Bool_t          L1_MU6_J5;
   Bool_t          L1_TAU11;
   Bool_t          L1_TAU11I;
   Bool_t          L1_TAU20;
   Bool_t          L1_TAU30;
   Bool_t          L1_TAU5;
   Bool_t          L1_TAU50;
   Bool_t          L1_TAU5_3J5_2J15;
   Bool_t          L1_TAU5_4J5_3J15;
   Bool_t          L1_TAU5_EMPTY;
   Bool_t          L1_TAU5_FIRSTEMPTY;
   Bool_t          L1_TAU5_MU6;
   Bool_t          L1_TAU5_MV;
   Bool_t          L1_TAU5_UNPAIRED_ISO;
   Bool_t          L1_TAU5_UNPAIRED_NONISO;
   Bool_t          L1_TAU5_XE10;
   Bool_t          L1_TAU6;
   Bool_t          L1_TAU6I;
   Bool_t          L1_TAU6_3J5_2J15;
   Bool_t          L1_TAU6_4J5_3J15;
   Bool_t          L1_TAU6_MU10;
   Bool_t          L1_TAU6_MU15;
   Bool_t          L1_TAU6_XE10;
   Bool_t          L1_XE10;
   Bool_t          L1_XE15;
   Bool_t          L1_XE20;
   Bool_t          L1_XE25;
   Bool_t          L1_XE30;
   Bool_t          L1_XE35;
   Bool_t          L1_XE40;
   Bool_t          L1_XE50;
   Bool_t          L2_2e10_loose;
   Bool_t          L2_2e10_medium;
   Bool_t          L2_2e15_loose;
   Bool_t          L2_2e3_loose;
   Bool_t          L2_2e3_loose_SiTrk;
   Bool_t          L2_2e3_loose_TRT;
   Bool_t          L2_2e3_medium;
   Bool_t          L2_2e3_medium_SiTrk;
   Bool_t          L2_2e3_medium_TRT;
   Bool_t          L2_2e3_tight;
   Bool_t          L2_2e5_medium;
   Bool_t          L2_2e5_medium_SiTrk;
   Bool_t          L2_2e5_medium_TRT;
   Bool_t          L2_2e5_tight;
   Bool_t          L2_2g10_loose;
   Bool_t          L2_2g15_loose;
   Bool_t          L2_2g20_loose;
   Bool_t          L2_2g5_loose;
   Bool_t          L2_2g7_loose;
   Bool_t          L2_2j30_xe12_noMu;
   Bool_t          L2_2j30_xe20_noMu;
   Bool_t          L2_2j30_xe30_noMu;
   Bool_t          L2_2j35_jetNoEF_xe20_noMu;
   Bool_t          L2_2j35_jetNoEF_xe30_noMu;
   Bool_t          L2_2j35_jetNoEF_xe40_noMu;
   Bool_t          L2_2mu0_NoAlg;
   Bool_t          L2_2mu10;
   Bool_t          L2_2mu10_NoAlg;
   Bool_t          L2_2mu4;
   Bool_t          L2_2mu4_Bmumu;
   Bool_t          L2_2mu4_Bmumux;
   Bool_t          L2_2mu4_DiMu;
   Bool_t          L2_2mu4_DiMu_SiTrk;
   Bool_t          L2_2mu4_DiMu_noVtx;
   Bool_t          L2_2mu4_DiMu_noVtx_noOS;
   Bool_t          L2_2mu4_Jpsimumu;
   Bool_t          L2_2mu4_Jpsimumu_IDTrkNoCut;
   Bool_t          L2_2mu4_Upsimumu;
   Bool_t          L2_2mu6;
   Bool_t          L2_2mu6_Bmumu;
   Bool_t          L2_2mu6_Bmumux;
   Bool_t          L2_2mu6_DiMu;
   Bool_t          L2_2mu6_Jpsimumu;
   Bool_t          L2_2mu6_MG;
   Bool_t          L2_2mu6_NoAlg;
   Bool_t          L2_2mu6_Upsimumu;
   Bool_t          L2_2mu6_g10_loose;
   Bool_t          L2_2tau12_loose;
   Bool_t          L2_2tau16_loose;
   Bool_t          L2_2tau20_loose;
   Bool_t          L2_2tau29_loose;
   Bool_t          L2_2tau29_loose1;
   Bool_t          L2_2tau38_loose;
   Bool_t          L2_e10_NoCut;
   Bool_t          L2_e10_loose;
   Bool_t          L2_e10_loose_mu10;
   Bool_t          L2_e10_loose_mu6;
   Bool_t          L2_e10_medium;
   Bool_t          L2_e10_medium_IDTrkNoCut;
   Bool_t          L2_e10_medium_SiTrk;
   Bool_t          L2_e10_medium_TRT;
   Bool_t          L2_e10_tight;
   Bool_t          L2_e10_tight_TRT;
   Bool_t          L2_e15_loose;
   Bool_t          L2_e15_loose_IDTrkNoCut;
   Bool_t          L2_e15_medium;
   Bool_t          L2_e15_medium_SiTrk;
   Bool_t          L2_e15_medium_TRT;
   Bool_t          L2_e15_tight;
   Bool_t          L2_e15_tight_TRT;
   Bool_t          L2_e18_medium;
   Bool_t          L2_e20_loose;
   Bool_t          L2_e20_loose_IDTrkNoCut;
   Bool_t          L2_e20_loose_TRT;
   Bool_t          L2_e20_loose_passEF;
   Bool_t          L2_e20_loose_passL2;
   Bool_t          L2_e20_loose_xe20;
   Bool_t          L2_e20_loose_xe20_noMu;
   Bool_t          L2_e20_loose_xe30;
   Bool_t          L2_e20_loose_xe30_noMu;
   Bool_t          L2_e20_medium;
   Bool_t          L2_e25_loose;
   Bool_t          L2_e25_medium;
   Bool_t          L2_e30_loose;
   Bool_t          L2_e3_loose;
   Bool_t          L2_e3_loose_SiTrk;
   Bool_t          L2_e3_loose_TRT;
   Bool_t          L2_e3_medium;
   Bool_t          L2_e3_medium_SiTrk;
   Bool_t          L2_e3_medium_TRT;
   Bool_t          L2_e5_NoCut_L2SW;
   Bool_t          L2_e5_NoCut_Ringer;
   Bool_t          L2_e5_NoCut_firstempty;
   Bool_t          L2_e5_loose_mu4;
   Bool_t          L2_e5_medium;
   Bool_t          L2_e5_medium_MV;
   Bool_t          L2_e5_medium_SiTrk;
   Bool_t          L2_e5_medium_TRT;
   Bool_t          L2_e5_medium_mu4;
   Bool_t          L2_e5_tight;
   Bool_t          L2_e5_tight_SiTrk;
   Bool_t          L2_e5_tight_TRT;
   Bool_t          L2_e5_tight_e5_NoCut;
   Bool_t          L2_em105_passHLT;
   Bool_t          L2_em3_empty_larcalib;
   Bool_t          L2_g10_loose;
   Bool_t          L2_g11_etcut;
   Bool_t          L2_g15_loose;
   Bool_t          L2_g17_etcut;
   Bool_t          L2_g17_etcut_EFxe20_noMu;
   Bool_t          L2_g17_etcut_EFxe30_noMu;
   Bool_t          L2_g20_loose;
   Bool_t          L2_g20_loose_cnv;
   Bool_t          L2_g20_loose_xe20_noMu;
   Bool_t          L2_g20_loose_xe30_noMu;
   Bool_t          L2_g20_tight;
   Bool_t          L2_g25_loose_xe30_noMu;
   Bool_t          L2_g30_loose;
   Bool_t          L2_g30_tight;
   Bool_t          L2_g3_NoCut_unpaired_iso;
   Bool_t          L2_g3_NoCut_unpaired_noniso;
   Bool_t          L2_g40_loose;
   Bool_t          L2_g40_tight;
   Bool_t          L2_g50_loose;
   Bool_t          L2_g5_NoCut_cosmic;
   Bool_t          L2_g5_loose;
   Bool_t          L2_g5_loose_cnv;
   Bool_t          L2_g7_loose;
   Bool_t          L2_j30_jetNoCut_xe20_e15_medium;
   Bool_t          L2_j30_xe20_e15_medium;
   Bool_t          L2_j35_jetNoCut_xe30_mu15;
   Bool_t          L2_j35_xe30_mu15;
   Bool_t          L2_j45_xe12_noMu;
   Bool_t          L2_j45_xe20_noMu;
   Bool_t          L2_j45_xe30_noMu;
   Bool_t          L2_mu0_NoAlg;
   Bool_t          L2_mu0_comm_NoAlg;
   Bool_t          L2_mu0_comm_empty_NoAlg;
   Bool_t          L2_mu0_comm_firstempty_NoAlg;
   Bool_t          L2_mu0_comm_unpaired_iso_NoAlg;
   Bool_t          L2_mu0_comm_unpaired_noniso_NoAlg;
   Bool_t          L2_mu0_empty_NoAlg;
   Bool_t          L2_mu0_firstempty_NoAlg;
   Bool_t          L2_mu0_missingRoi;
   Bool_t          L2_mu0_outOfTime1;
   Bool_t          L2_mu0_outOfTime2;
   Bool_t          L2_mu0_rpcOnly;
   Bool_t          L2_mu0_unpaired_iso_NoAlg;
   Bool_t          L2_mu0_unpaired_noniso_NoAlg;
   Bool_t          L2_mu10;
   Bool_t          L2_mu10_Ecut12;
   Bool_t          L2_mu10_Ecut123;
   Bool_t          L2_mu10_Ecut13;
   Bool_t          L2_mu10_IDTrkNoCut;
   Bool_t          L2_mu10_MG;
   Bool_t          L2_mu10_MSonly;
   Bool_t          L2_mu10_MSonly_Ecut12;
   Bool_t          L2_mu10_MSonly_Ecut123;
   Bool_t          L2_mu10_MSonly_Ecut13;
   Bool_t          L2_mu10_MSonly_tight;
   Bool_t          L2_mu10_NoAlg;
   Bool_t          L2_mu10_SiTrk;
   Bool_t          L2_mu10_j30;
   Bool_t          L2_mu10_tight;
   Bool_t          L2_mu10i_loose;
   Bool_t          L2_mu13;
   Bool_t          L2_mu13_MG;
   Bool_t          L2_mu13_MG_tight;
   Bool_t          L2_mu13_tight;
   Bool_t          L2_mu15;
   Bool_t          L2_mu15_MG;
   Bool_t          L2_mu15_NoAlg;
   Bool_t          L2_mu20;
   Bool_t          L2_mu20_MSonly;
   Bool_t          L2_mu20_NoAlg;
   Bool_t          L2_mu20_passHLT;
   Bool_t          L2_mu20_slow;
   Bool_t          L2_mu30_MSonly;
   Bool_t          L2_mu4;
   Bool_t          L2_mu40_MSonly;
   Bool_t          L2_mu4_Bmumu;
   Bool_t          L2_mu4_BmumuX;
   Bool_t          L2_mu4_DiMu;
   Bool_t          L2_mu4_DiMu_FS;
   Bool_t          L2_mu4_DiMu_FS_noOS;
   Bool_t          L2_mu4_DiMu_MG;
   Bool_t          L2_mu4_DiMu_MG_FS;
   Bool_t          L2_mu4_DiMu_SiTrk;
   Bool_t          L2_mu4_DiMu_SiTrk_FS;
   Bool_t          L2_mu4_DiMu_noOS;
   Bool_t          L2_mu4_IDTrkNoCut;
   Bool_t          L2_mu4_Jpsie5e3;
   Bool_t          L2_mu4_Jpsie5e3_FS;
   Bool_t          L2_mu4_Jpsie5e3_SiTrk;
   Bool_t          L2_mu4_Jpsie5e3_SiTrk_FS;
   Bool_t          L2_mu4_Jpsimumu;
   Bool_t          L2_mu4_Jpsimumu_FS;
   Bool_t          L2_mu4_Jpsimumu_SiTrk_FS;
   Bool_t          L2_mu4_L1J10_matched;
   Bool_t          L2_mu4_L1J15_matched;
   Bool_t          L2_mu4_L1J30_matched;
   Bool_t          L2_mu4_L1J55_matched;
   Bool_t          L2_mu4_L1J5_matched;
   Bool_t          L2_mu4_L2MSonly_EFFS_passL2;
   Bool_t          L2_mu4_MG;
   Bool_t          L2_mu4_MSonly;
   Bool_t          L2_mu4_MSonly_EFFS_passL2;
   Bool_t          L2_mu4_MSonly_MB2_noL2_EFFS;
   Bool_t          L2_mu4_MSonly_cosmic;
   Bool_t          L2_mu4_MSonly_outOfTime;
   Bool_t          L2_mu4_MV;
   Bool_t          L2_mu4_SiTrk;
   Bool_t          L2_mu4_Trk_Jpsi;
   Bool_t          L2_mu4_Trk_Jpsi_FS;
   Bool_t          L2_mu4_Trk_Jpsi_loose;
   Bool_t          L2_mu4_Trk_Upsi_FS;
   Bool_t          L2_mu4_Trk_Upsi_loose_FS;
   Bool_t          L2_mu4_Upsimumu_FS;
   Bool_t          L2_mu4_Upsimumu_SiTrk_FS;
   Bool_t          L2_mu4_comm_MSonly_cosmic;
   Bool_t          L2_mu4_comm_cosmic;
   Bool_t          L2_mu4_comm_firstempty;
   Bool_t          L2_mu4_comm_unpaired_iso;
   Bool_t          L2_mu4_cosmic;
   Bool_t          L2_mu4_firstempty;
   Bool_t          L2_mu4_j20;
   Bool_t          L2_mu4_j20_jetNoEF;
   Bool_t          L2_mu4_j25;
   Bool_t          L2_mu4_mu6;
   Bool_t          L2_mu4_muCombTag;
   Bool_t          L2_mu4_tile;
   Bool_t          L2_mu4_tile_cosmic;
   Bool_t          L2_mu4_unpaired_iso;
   Bool_t          L2_mu4mu6_Bmumu;
   Bool_t          L2_mu4mu6_BmumuX;
   Bool_t          L2_mu4mu6_DiMu;
   Bool_t          L2_mu4mu6_Jpsimumu;
   Bool_t          L2_mu4mu6_Jpsimumu_IDTrkNoCut;
   Bool_t          L2_mu4mu6_Upsimumu;
   Bool_t          L2_mu6;
   Bool_t          L2_mu6_Bmumu;
   Bool_t          L2_mu6_BmumuX;
   Bool_t          L2_mu6_DiMu;
   Bool_t          L2_mu6_Ecut12;
   Bool_t          L2_mu6_Ecut123;
   Bool_t          L2_mu6_Ecut13;
   Bool_t          L2_mu6_Ecut2;
   Bool_t          L2_mu6_Ecut3;
   Bool_t          L2_mu6_IDTrkNoCut;
   Bool_t          L2_mu6_Jpsie5e3;
   Bool_t          L2_mu6_Jpsie5e3_FS;
   Bool_t          L2_mu6_Jpsie5e3_SiTrk;
   Bool_t          L2_mu6_Jpsie5e3_SiTrk_FS;
   Bool_t          L2_mu6_Jpsimumu;
   Bool_t          L2_mu6_MG;
   Bool_t          L2_mu6_MSonly;
   Bool_t          L2_mu6_MSonly_Ecut12;
   Bool_t          L2_mu6_MSonly_Ecut123;
   Bool_t          L2_mu6_MSonly_Ecut13;
   Bool_t          L2_mu6_MSonly_Ecut2;
   Bool_t          L2_mu6_MSonly_Ecut3;
   Bool_t          L2_mu6_MSonly_outOfTime;
   Bool_t          L2_mu6_NoAlg;
   Bool_t          L2_mu6_SiTrk;
   Bool_t          L2_mu6_Trk_Jpsi;
   Bool_t          L2_mu6_Upsimumu_FS;
   Bool_t          L2_mu6_muCombTag;
   Bool_t          L2_tau125_loose;
   Bool_t          L2_tau125_medium;
   Bool_t          L2_tau12_IDTrkNoCut;
   Bool_t          L2_tau12_loose;
   Bool_t          L2_tau12_loose_2b15;
   Bool_t          L2_tau12_loose_3j30;
   Bool_t          L2_tau12_loose_EFxe12_noMu;
   Bool_t          L2_tau12_loose_IdScan_EFxe12_noMu;
   Bool_t          L2_tau12_loose_IdScan_xe15_noMu;
   Bool_t          L2_tau12_loose_e10_loose;
   Bool_t          L2_tau12_loose_e10_medium;
   Bool_t          L2_tau12_loose_e10_tight;
   Bool_t          L2_tau12_loose_mu10;
   Bool_t          L2_tau12_loose_xe15_noMu;
   Bool_t          L2_tau12_loose_xe20_noMu;
   Bool_t          L2_tau16_loose;
   Bool_t          L2_tau16_loose_2b15;
   Bool_t          L2_tau16_loose_3j30;
   Bool_t          L2_tau16_loose_e10_loose;
   Bool_t          L2_tau16_loose_e15_loose;
   Bool_t          L2_tau16_loose_mu10;
   Bool_t          L2_tau16_loose_mu15;
   Bool_t          L2_tau16_loose_xe20_noMu;
   Bool_t          L2_tau16_loose_xe25_noMu;
   Bool_t          L2_tau16_loose_xe25_tight_noMu;
   Bool_t          L2_tau16_loose_xe30_noMu;
   Bool_t          L2_tau16_medium;
   Bool_t          L2_tau16_medium_xe22_noMu;
   Bool_t          L2_tau16_medium_xe25_noMu;
   Bool_t          L2_tau16_medium_xe25_tight_noMu;
   Bool_t          L2_tau20_loose;
   Bool_t          L2_tau20_loose_xe25_noMu;
   Bool_t          L2_tau29_loose;
   Bool_t          L2_tau29_loose1;
   Bool_t          L2_tau38_loose;
   Bool_t          L2_tau38_medium;
   Bool_t          L2_tau50_IDTrkNoCut;
   Bool_t          L2_tau50_loose;
   Bool_t          L2_tau50_loose_IdScan;
   Bool_t          L2_tau50_medium;
   Bool_t          L2_tau5_empty_larcalib;
   Bool_t          L2_tau84_loose;
   Bool_t          L2_tau84_medium;
   Bool_t          L2_tauNoCut;
   Bool_t          L2_tauNoCut_IdScan;
   Bool_t          L2_tauNoCut_cosmic;
   Bool_t          L2_tauNoCut_firstempty;
   Bool_t          L2_tauNoCut_hasTrk6_EFxe15_noMu;
   Bool_t          L2_tauNoCut_hasTrk6_IdScan_EFxe15_noMu;
   Bool_t          L2_tauNoCut_hasTrk6_IdScan_xe20_noMu;
   Bool_t          L2_tauNoCut_hasTrk6_xe20_noMu;
   Bool_t          L2_tauNoCut_hasTrk9_xe20_noMu;
   Bool_t          L2_tauNoCut_hasTrk_MV;
   Bool_t          L2_tauNoCut_hasTrk_e10_tight;
   Bool_t          L2_tauNoCut_hasTrk_xe20_noMu;
   Bool_t          L2_tauNoCut_unpaired_iso;
   Bool_t          L2_tauNoCut_unpaired_noniso;
   Bool_t          L2_xe12;
   Bool_t          L2_xe12_loose;
   Bool_t          L2_xe12_loose_noMu;
   Bool_t          L2_xe12_medium;
   Bool_t          L2_xe12_medium_noMu;
   Bool_t          L2_xe12_noMu;
   Bool_t          L2_xe15;
   Bool_t          L2_xe15_medium;
   Bool_t          L2_xe15_medium_noMu;
   Bool_t          L2_xe15_medium_vfj_noMu;
   Bool_t          L2_xe15_noMu;
   Bool_t          L2_xe15_tight_noMu;
   Bool_t          L2_xe15_tight_vfj_noMu;
   Bool_t          L2_xe15_unbiased_noMu;
   Bool_t          L2_xe15_vfj_noMu;
   Bool_t          L2_xe17_tight_noMu;
   Bool_t          L2_xe17_tight_vfj_noMu;
   Bool_t          L2_xe20;
   Bool_t          L2_xe20_noMu;
   Bool_t          L2_xe20_vfj_noMu;
   Bool_t          L2_xe22_loose_noMu;
   Bool_t          L2_xe22_tight_noMu;
   Bool_t          L2_xe22_tight_vfj_noMu;
   Bool_t          L2_xe25;
   Bool_t          L2_xe25_noMu;
   Bool_t          L2_xe27_tight_noMu;
   Bool_t          L2_xe30;
   Bool_t          L2_xe30_allL1;
   Bool_t          L2_xe30_allL1_FEB;
   Bool_t          L2_xe30_allL1_allCells;
   Bool_t          L2_xe30_allL1_noMu;
   Bool_t          L2_xe30_noMu;
   Bool_t          L2_xe32_tight_noMu;
   Bool_t          L2_xe35;
   Bool_t          L2_xe35_noMu;
   Bool_t          L2_xe40_medium;
   Bool_t          L2_xe40_medium_noMu;
   Bool_t          L2_xe45;
   Bool_t          L2_xe45_noMu;
   Bool_t          L2_xe60_medium;
   Bool_t          L2_xe60_medium_noMu;
   Int_t           ph_n;
   std::vector<float>   *ph_E;
   std::vector<float>   *ph_Et;
   std::vector<float>   *ph_pt;
   std::vector<float>   *ph_m;
   std::vector<float>   *ph_eta;
   std::vector<float>   *ph_phi;
   std::vector<float>   *ph_px;
   std::vector<float>   *ph_py;
   std::vector<float>   *ph_pz;
   std::vector<int>     *ph_author;
   std::vector<int>     *ph_isRecovered;
   std::vector<unsigned int> *ph_isEM;
   std::vector<unsigned int> *ph_OQ;
   std::vector<int>     *ph_convFlag;
   std::vector<int>     *ph_isConv;
   std::vector<int>     *ph_nConv;
   std::vector<int>     *ph_nSingleTrackConv;
   std::vector<int>     *ph_nDoubleTrackConv;
   std::vector<int>     *ph_type;
   std::vector<int>     *ph_origin;
   std::vector<float>   *ph_truth_deltaRRecPhoton;
   std::vector<float>   *ph_truth_E;
   std::vector<float>   *ph_truth_pt;
   std::vector<float>   *ph_truth_eta;
   std::vector<float>   *ph_truth_phi;
   std::vector<int>     *ph_truth_type;
   std::vector<int>     *ph_truth_status;
   std::vector<int>     *ph_truth_barcode;
   std::vector<int>     *ph_truth_mothertype;
   std::vector<int>     *ph_truth_motherbarcode;
   std::vector<int>     *ph_truth_index;
   std::vector<int>     *ph_truth_matched;
   std::vector<int>     *ph_loose;
   std::vector<int>     *ph_tight;
   std::vector<int>     *ph_tightIso;
   std::vector<int>     *ph_goodOQ;
   std::vector<float>   *ph_Ethad;
   std::vector<float>   *ph_Ethad1;
   std::vector<float>   *ph_E033;
   std::vector<float>   *ph_f1;
   std::vector<float>   *ph_f1core;
   std::vector<float>   *ph_Emins1;
   std::vector<float>   *ph_fside;
   std::vector<float>   *ph_Emax2;
   std::vector<float>   *ph_ws3;
   std::vector<float>   *ph_wstot;
   std::vector<float>   *ph_E132;
   std::vector<float>   *ph_E1152;
   std::vector<float>   *ph_emaxs1;
   std::vector<float>   *ph_deltaEs;
   std::vector<float>   *ph_E233;
   std::vector<float>   *ph_E237;
   std::vector<float>   *ph_E277;
   std::vector<float>   *ph_weta2;
   std::vector<float>   *ph_f3;
   std::vector<float>   *ph_f3core;
   std::vector<float>   *ph_rphiallcalo;
   std::vector<float>   *ph_Etcone45;
   std::vector<float>   *ph_Etcone20;
   std::vector<float>   *ph_Etcone30;
   std::vector<float>   *ph_Etcone40;
   std::vector<float>   *ph_ptcone20;
   std::vector<float>   *ph_ptcone30;
   std::vector<float>   *ph_ptcone40;
   std::vector<float>   *ph_nucone20;
   std::vector<float>   *ph_nucone30;
   std::vector<float>   *ph_nucone40;
   std::vector<float>   *ph_convanglematch;
   std::vector<float>   *ph_convtrackmatch;
   std::vector<int>     *ph_hasconv;
   std::vector<float>   *ph_convvtxx;
   std::vector<float>   *ph_convvtxy;
   std::vector<float>   *ph_convvtxz;
   std::vector<float>   *ph_Rconv;
   std::vector<float>   *ph_zconv;
   std::vector<float>   *ph_convvtxchi2;
   std::vector<float>   *ph_pt1conv;
   std::vector<int>     *ph_convtrk1nBLHits;
   std::vector<int>     *ph_convtrk1nPixHits;
   std::vector<int>     *ph_convtrk1nSCTHits;
   std::vector<int>     *ph_convtrk1nTRTHits;
   std::vector<float>   *ph_pt2conv;
   std::vector<int>     *ph_convtrk2nBLHits;
   std::vector<int>     *ph_convtrk2nPixHits;
   std::vector<int>     *ph_convtrk2nSCTHits;
   std::vector<int>     *ph_convtrk2nTRTHits;
   std::vector<float>   *ph_ptconv;
   std::vector<float>   *ph_pzconv;
   std::vector<float>   *ph_reta;
   std::vector<float>   *ph_rphi;
   std::vector<float>   *ph_EtringnoisedR03sig2;
   std::vector<float>   *ph_EtringnoisedR03sig3;
   std::vector<float>   *ph_EtringnoisedR03sig4;
   std::vector<double>  *ph_isolationlikelihoodjets;
   std::vector<double>  *ph_isolationlikelihoodhqelectrons;
   std::vector<double>  *ph_loglikelihood;
   std::vector<double>  *ph_photonweight;
   std::vector<double>  *ph_photonbgweight;
   std::vector<double>  *ph_neuralnet;
   std::vector<double>  *ph_Hmatrix;
   std::vector<double>  *ph_Hmatrix5;
   std::vector<double>  *ph_adaboost;
   std::vector<float>   *ph_zvertex;
   std::vector<float>   *ph_errz;
   std::vector<float>   *ph_etap;
   std::vector<float>   *ph_depth;
   std::vector<float>   *ph_cl_E;
   std::vector<float>   *ph_cl_pt;
   std::vector<float>   *ph_cl_eta;
   std::vector<float>   *ph_cl_phi;
   std::vector<float>   *ph_Es0;
   std::vector<float>   *ph_etas0;
   std::vector<float>   *ph_phis0;
   std::vector<float>   *ph_Es1;
   std::vector<float>   *ph_etas1;
   std::vector<float>   *ph_phis1;
   std::vector<float>   *ph_Es2;
   std::vector<float>   *ph_etas2;
   std::vector<float>   *ph_phis2;
   std::vector<float>   *ph_Es3;
   std::vector<float>   *ph_etas3;
   std::vector<float>   *ph_phis3;
   std::vector<float>   *ph_rawcl_Es0;
   std::vector<float>   *ph_rawcl_etas0;
   std::vector<float>   *ph_rawcl_phis0;
   std::vector<float>   *ph_rawcl_Es1;
   std::vector<float>   *ph_rawcl_etas1;
   std::vector<float>   *ph_rawcl_phis1;
   std::vector<float>   *ph_rawcl_Es2;
   std::vector<float>   *ph_rawcl_etas2;
   std::vector<float>   *ph_rawcl_phis2;
   std::vector<float>   *ph_rawcl_Es3;
   std::vector<float>   *ph_rawcl_etas3;
   std::vector<float>   *ph_rawcl_phis3;
   std::vector<float>   *ph_rawcl_E;
   std::vector<float>   *ph_rawcl_pt;
   std::vector<float>   *ph_rawcl_eta;
   std::vector<float>   *ph_rawcl_phi;
   std::vector<int>     *ph_truth_isConv;
   std::vector<int>     *ph_truth_isBrem;
   std::vector<int>     *ph_truth_isFromHardProc;
   std::vector<int>     *ph_truth_isPhotonFromHardProc;
   std::vector<float>   *ph_truth_Rconv;
   std::vector<float>   *ph_truth_zconv;
   std::vector<float>   *ph_deltaEmax2;
   std::vector<float>   *ph_calibHitsShowerDepth;
   std::vector<unsigned int> *ph_isIso;
   std::vector<float>   *ph_mvaptcone20;
   std::vector<float>   *ph_mvaptcone30;
   std::vector<float>   *ph_mvaptcone40;
   std::vector<float>   *ph_topoEtcone20;
   std::vector<float>   *ph_topoEtcone40;
   std::vector<float>   *ph_topoEtcone60;
   std::vector<float>   *ph_jet_dr;
   std::vector<float>   *ph_jet_E;
   std::vector<float>   *ph_jet_pt;
   std::vector<float>   *ph_jet_m;
   std::vector<float>   *ph_jet_eta;
   std::vector<float>   *ph_jet_phi;
   std::vector<float>   *ph_jet_truth_dr;
   std::vector<float>   *ph_jet_truth_E;
   std::vector<float>   *ph_jet_truth_pt;
   std::vector<float>   *ph_jet_truth_m;
   std::vector<float>   *ph_jet_truth_eta;
   std::vector<float>   *ph_jet_truth_phi;
   std::vector<int>     *ph_jet_truth_matched;
   std::vector<int>     *ph_jet_matched;
   std::vector<float>   *ph_convIP;
   std::vector<float>   *ph_convIPRev;
   std::vector<float>   *ph_ptIsolationCone;
   std::vector<float>   *ph_ptIsolationConePhAngle;
   std::vector<float>   *ph_Etcone40_pt_corrected;
   std::vector<float>   *ph_Etcone40_ED_corrected;
   std::vector<float>   *ph_Etcone40_corrected;
   std::vector<float>   *ph_topodr;
   std::vector<float>   *ph_topopt;
   std::vector<float>   *ph_topoeta;
   std::vector<float>   *ph_topophi;
   std::vector<int>     *ph_topomatched;
   std::vector<float>   *ph_topoEMdr;
   std::vector<float>   *ph_topoEMpt;
   std::vector<float>   *ph_topoEMeta;
   std::vector<float>   *ph_topoEMphi;
   std::vector<int>     *ph_topoEMmatched;
   std::vector<float>   *ph_EF_dr;
   std::vector<int>     *ph_EF_index;
   std::vector<float>   *ph_L2_dr;
   std::vector<int>     *ph_L2_index;
   std::vector<float>   *ph_L1_dr;
   std::vector<int>     *ph_L1_index;
   Int_t           mu_muid_n;
   std::vector<float>   *mu_muid_E;
   std::vector<float>   *mu_muid_pt;
   std::vector<float>   *mu_muid_m;
   std::vector<float>   *mu_muid_eta;
   std::vector<float>   *mu_muid_phi;
   std::vector<float>   *mu_muid_px;
   std::vector<float>   *mu_muid_py;
   std::vector<float>   *mu_muid_pz;
   std::vector<float>   *mu_muid_charge;
   std::vector<unsigned short> *mu_muid_allauthor;
   std::vector<int>     *mu_muid_author;
   std::vector<float>   *mu_muid_beta;
   std::vector<float>   *mu_muid_isMuonLikelihood;
   std::vector<float>   *mu_muid_matchchi2;
   std::vector<int>     *mu_muid_matchndof;
   std::vector<float>   *mu_muid_etcone20;
   std::vector<float>   *mu_muid_etcone30;
   std::vector<float>   *mu_muid_etcone40;
   std::vector<float>   *mu_muid_nucone20;
   std::vector<float>   *mu_muid_nucone30;
   std::vector<float>   *mu_muid_nucone40;
   std::vector<float>   *mu_muid_ptcone20;
   std::vector<float>   *mu_muid_ptcone30;
   std::vector<float>   *mu_muid_ptcone40;
   std::vector<float>   *mu_muid_energyLossPar;
   std::vector<float>   *mu_muid_energyLossErr;
   std::vector<float>   *mu_muid_etCore;
   std::vector<float>   *mu_muid_energyLossType;
   std::vector<unsigned short> *mu_muid_caloMuonIdTag;
   std::vector<double>  *mu_muid_caloLRLikelihood;
   std::vector<int>     *mu_muid_bestMatch;
   std::vector<int>     *mu_muid_isStandAloneMuon;
   std::vector<int>     *mu_muid_isCombinedMuon;
   std::vector<int>     *mu_muid_isLowPtReconstructedMuon;
   std::vector<int>     *mu_muid_loose;
   std::vector<int>     *mu_muid_medium;
   std::vector<int>     *mu_muid_tight;
   std::vector<float>   *mu_muid_d0_exPV;
   std::vector<float>   *mu_muid_z0_exPV;
   std::vector<float>   *mu_muid_phi_exPV;
   std::vector<float>   *mu_muid_theta_exPV;
   std::vector<float>   *mu_muid_qoverp_exPV;
   std::vector<float>   *mu_muid_cb_d0_exPV;
   std::vector<float>   *mu_muid_cb_z0_exPV;
   std::vector<float>   *mu_muid_cb_phi_exPV;
   std::vector<float>   *mu_muid_cb_theta_exPV;
   std::vector<float>   *mu_muid_cb_qoverp_exPV;
   std::vector<float>   *mu_muid_id_d0_exPV;
   std::vector<float>   *mu_muid_id_z0_exPV;
   std::vector<float>   *mu_muid_id_phi_exPV;
   std::vector<float>   *mu_muid_id_theta_exPV;
   std::vector<float>   *mu_muid_id_qoverp_exPV;
   std::vector<float>   *mu_muid_me_d0_exPV;
   std::vector<float>   *mu_muid_me_z0_exPV;
   std::vector<float>   *mu_muid_me_phi_exPV;
   std::vector<float>   *mu_muid_me_theta_exPV;
   std::vector<float>   *mu_muid_me_qoverp_exPV;
   std::vector<float>   *mu_muid_ie_d0_exPV;
   std::vector<float>   *mu_muid_ie_z0_exPV;
   std::vector<float>   *mu_muid_ie_phi_exPV;
   std::vector<float>   *mu_muid_ie_theta_exPV;
   std::vector<float>   *mu_muid_ie_qoverp_exPV;
   std::vector<std::vector<int> > *mu_muid_SpaceTime_detID;
   std::vector<std::vector<float> > *mu_muid_SpaceTime_t;
   std::vector<std::vector<float> > *mu_muid_SpaceTime_tError;
   std::vector<std::vector<float> > *mu_muid_SpaceTime_weight;
   std::vector<std::vector<float> > *mu_muid_SpaceTime_x;
   std::vector<std::vector<float> > *mu_muid_SpaceTime_y;
   std::vector<std::vector<float> > *mu_muid_SpaceTime_z;
   std::vector<std::vector<float> > *mu_muid_SpaceTime_t_Tile;
   std::vector<std::vector<float> > *mu_muid_SpaceTime_tError_Tile;
   std::vector<std::vector<float> > *mu_muid_SpaceTime_weight_Tile;
   std::vector<std::vector<float> > *mu_muid_SpaceTime_x_Tile;
   std::vector<std::vector<float> > *mu_muid_SpaceTime_y_Tile;
   std::vector<std::vector<float> > *mu_muid_SpaceTime_z_Tile;
   std::vector<std::vector<float> > *mu_muid_SpaceTime_t_TRT;
   std::vector<std::vector<float> > *mu_muid_SpaceTime_tError_TRT;
   std::vector<std::vector<float> > *mu_muid_SpaceTime_weight_TRT;
   std::vector<std::vector<float> > *mu_muid_SpaceTime_x_TRT;
   std::vector<std::vector<float> > *mu_muid_SpaceTime_y_TRT;
   std::vector<std::vector<float> > *mu_muid_SpaceTime_z_TRT;
   std::vector<std::vector<float> > *mu_muid_SpaceTime_t_MDT;
   std::vector<std::vector<float> > *mu_muid_SpaceTime_tError_MDT;
   std::vector<std::vector<float> > *mu_muid_SpaceTime_weight_MDT;
   std::vector<std::vector<float> > *mu_muid_SpaceTime_x_MDT;
   std::vector<std::vector<float> > *mu_muid_SpaceTime_y_MDT;
   std::vector<std::vector<float> > *mu_muid_SpaceTime_z_MDT;
   std::vector<float>   *mu_muid_TileCellEnergyLayer1;
   std::vector<float>   *mu_muid_TileTimeLayer1;
   std::vector<float>   *mu_muid_TileCellRmsNoiseLayer1;
   std::vector<float>   *mu_muid_TileCellSignLayer1;
   std::vector<float>   *mu_muid_TileCellEnergyLayer2;
   std::vector<float>   *mu_muid_TileTimeLayer2;
   std::vector<float>   *mu_muid_TileCellRmsNoiseLayer2;
   std::vector<float>   *mu_muid_TileCellSignLayer2;
   std::vector<float>   *mu_muid_TileCellEnergyLayer3;
   std::vector<float>   *mu_muid_TileTimeLayer3;
   std::vector<float>   *mu_muid_TileCellRmsNoiseLayer3;
   std::vector<float>   *mu_muid_TileCellSignLayer3;
   std::vector<float>   *mu_muid_MSTrkT0_1;
   std::vector<float>   *mu_muid_MSTrkT0_2;
   std::vector<float>   *mu_muid_MSTrkT0_3;
   std::vector<float>   *mu_muid_cov_d0_exPV;
   std::vector<float>   *mu_muid_cov_z0_exPV;
   std::vector<float>   *mu_muid_cov_phi_exPV;
   std::vector<float>   *mu_muid_cov_theta_exPV;
   std::vector<float>   *mu_muid_cov_qoverp_exPV;
   std::vector<float>   *mu_muid_cov_d0_z0_exPV;
   std::vector<float>   *mu_muid_cov_d0_phi_exPV;
   std::vector<float>   *mu_muid_cov_d0_theta_exPV;
   std::vector<float>   *mu_muid_cov_d0_qoverp_exPV;
   std::vector<float>   *mu_muid_cov_z0_phi_exPV;
   std::vector<float>   *mu_muid_cov_z0_theta_exPV;
   std::vector<float>   *mu_muid_cov_z0_qoverp_exPV;
   std::vector<float>   *mu_muid_cov_phi_theta_exPV;
   std::vector<float>   *mu_muid_cov_phi_qoverp_exPV;
   std::vector<float>   *mu_muid_cov_theta_qoverp_exPV;
   std::vector<float>   *mu_muid_id_cov_d0_exPV;
   std::vector<float>   *mu_muid_id_cov_z0_exPV;
   std::vector<float>   *mu_muid_id_cov_phi_exPV;
   std::vector<float>   *mu_muid_id_cov_theta_exPV;
   std::vector<float>   *mu_muid_id_cov_qoverp_exPV;
   std::vector<float>   *mu_muid_id_cov_d0_z0_exPV;
   std::vector<float>   *mu_muid_id_cov_d0_phi_exPV;
   std::vector<float>   *mu_muid_id_cov_d0_theta_exPV;
   std::vector<float>   *mu_muid_id_cov_d0_qoverp_exPV;
   std::vector<float>   *mu_muid_id_cov_z0_phi_exPV;
   std::vector<float>   *mu_muid_id_cov_z0_theta_exPV;
   std::vector<float>   *mu_muid_id_cov_z0_qoverp_exPV;
   std::vector<float>   *mu_muid_id_cov_phi_theta_exPV;
   std::vector<float>   *mu_muid_id_cov_phi_qoverp_exPV;
   std::vector<float>   *mu_muid_id_cov_theta_qoverp_exPV;
   std::vector<float>   *mu_muid_me_cov_d0_exPV;
   std::vector<float>   *mu_muid_me_cov_z0_exPV;
   std::vector<float>   *mu_muid_me_cov_phi_exPV;
   std::vector<float>   *mu_muid_me_cov_theta_exPV;
   std::vector<float>   *mu_muid_me_cov_qoverp_exPV;
   std::vector<float>   *mu_muid_me_cov_d0_z0_exPV;
   std::vector<float>   *mu_muid_me_cov_d0_phi_exPV;
   std::vector<float>   *mu_muid_me_cov_d0_theta_exPV;
   std::vector<float>   *mu_muid_me_cov_d0_qoverp_exPV;
   std::vector<float>   *mu_muid_me_cov_z0_phi_exPV;
   std::vector<float>   *mu_muid_me_cov_z0_theta_exPV;
   std::vector<float>   *mu_muid_me_cov_z0_qoverp_exPV;
   std::vector<float>   *mu_muid_me_cov_phi_theta_exPV;
   std::vector<float>   *mu_muid_me_cov_phi_qoverp_exPV;
   std::vector<float>   *mu_muid_me_cov_theta_qoverp_exPV;
   std::vector<float>   *mu_muid_ms_d0;
   std::vector<float>   *mu_muid_ms_z0;
   std::vector<float>   *mu_muid_ms_phi;
   std::vector<float>   *mu_muid_ms_theta;
   std::vector<float>   *mu_muid_ms_qoverp;
   std::vector<float>   *mu_muid_id_d0;
   std::vector<float>   *mu_muid_id_z0;
   std::vector<float>   *mu_muid_id_phi;
   std::vector<float>   *mu_muid_id_theta;
   std::vector<float>   *mu_muid_id_qoverp;
   std::vector<float>   *mu_muid_me_d0;
   std::vector<float>   *mu_muid_me_z0;
   std::vector<float>   *mu_muid_me_phi;
   std::vector<float>   *mu_muid_me_theta;
   std::vector<float>   *mu_muid_me_qoverp;
   std::vector<float>   *mu_muid_ie_d0;
   std::vector<float>   *mu_muid_ie_z0;
   std::vector<float>   *mu_muid_ie_phi;
   std::vector<float>   *mu_muid_ie_theta;
   std::vector<float>   *mu_muid_ie_qoverp;
   std::vector<int>     *mu_muid_nOutliersOnTrack;
   std::vector<int>     *mu_muid_nBLHits;
   std::vector<int>     *mu_muid_nPixHits;
   std::vector<int>     *mu_muid_nSCTHits;
   std::vector<int>     *mu_muid_nTRTHits;
   std::vector<int>     *mu_muid_nTRTHighTHits;
   std::vector<int>     *mu_muid_nBLSharedHits;
   std::vector<int>     *mu_muid_nPixSharedHits;
   std::vector<int>     *mu_muid_nPixHoles;
   std::vector<int>     *mu_muid_nSCTSharedHits;
   std::vector<int>     *mu_muid_nSCTHoles;
   std::vector<int>     *mu_muid_nTRTOutliers;
   std::vector<int>     *mu_muid_nTRTHighTOutliers;
   std::vector<int>     *mu_muid_nGangedPixels;
   std::vector<int>     *mu_muid_nPixelDeadSensors;
   std::vector<int>     *mu_muid_nSCTDeadSensors;
   std::vector<int>     *mu_muid_nTRTDeadStraws;
   std::vector<int>     *mu_muid_expectBLayerHit;
   std::vector<int>     *mu_muid_nMDTHits;
   std::vector<int>     *mu_muid_nMDTHoles;
   std::vector<int>     *mu_muid_nCSCEtaHits;
   std::vector<int>     *mu_muid_nCSCEtaHoles;
   std::vector<int>     *mu_muid_nCSCPhiHits;
   std::vector<int>     *mu_muid_nCSCPhiHoles;
   std::vector<int>     *mu_muid_nRPCEtaHits;
   std::vector<int>     *mu_muid_nRPCEtaHoles;
   std::vector<int>     *mu_muid_nRPCPhiHits;
   std::vector<int>     *mu_muid_nRPCPhiHoles;
   std::vector<int>     *mu_muid_nTGCEtaHits;
   std::vector<int>     *mu_muid_nTGCEtaHoles;
   std::vector<int>     *mu_muid_nTGCPhiHits;
   std::vector<int>     *mu_muid_nTGCPhiHoles;
   std::vector<int>     *mu_muid_nMDTBIHits;
   std::vector<int>     *mu_muid_nMDTBMHits;
   std::vector<int>     *mu_muid_nMDTBOHits;
   std::vector<int>     *mu_muid_nMDTBEEHits;
   std::vector<int>     *mu_muid_nMDTBIS78Hits;
   std::vector<int>     *mu_muid_nMDTEIHits;
   std::vector<int>     *mu_muid_nMDTEMHits;
   std::vector<int>     *mu_muid_nMDTEOHits;
   std::vector<int>     *mu_muid_nMDTEEHits;
   std::vector<int>     *mu_muid_nRPCLayer1EtaHits;
   std::vector<int>     *mu_muid_nRPCLayer2EtaHits;
   std::vector<int>     *mu_muid_nRPCLayer3EtaHits;
   std::vector<int>     *mu_muid_nRPCLayer1PhiHits;
   std::vector<int>     *mu_muid_nRPCLayer2PhiHits;
   std::vector<int>     *mu_muid_nRPCLayer3PhiHits;
   std::vector<int>     *mu_muid_nTGCLayer1EtaHits;
   std::vector<int>     *mu_muid_nTGCLayer2EtaHits;
   std::vector<int>     *mu_muid_nTGCLayer3EtaHits;
   std::vector<int>     *mu_muid_nTGCLayer4EtaHits;
   std::vector<int>     *mu_muid_nTGCLayer1PhiHits;
   std::vector<int>     *mu_muid_nTGCLayer2PhiHits;
   std::vector<int>     *mu_muid_nTGCLayer3PhiHits;
   std::vector<int>     *mu_muid_nTGCLayer4PhiHits;
   std::vector<int>     *mu_muid_barrelSectors;
   std::vector<int>     *mu_muid_endcapSectors;
   std::vector<float>   *mu_muid_trackd0;
   std::vector<float>   *mu_muid_trackz0;
   std::vector<float>   *mu_muid_trackphi;
   std::vector<float>   *mu_muid_tracktheta;
   std::vector<float>   *mu_muid_trackqoverp;
   std::vector<float>   *mu_muid_trackcov_d0;
   std::vector<float>   *mu_muid_trackcov_z0;
   std::vector<float>   *mu_muid_trackcov_phi;
   std::vector<float>   *mu_muid_trackcov_theta;
   std::vector<float>   *mu_muid_trackcov_qoverp;
   std::vector<float>   *mu_muid_trackcov_d0_z0;
   std::vector<float>   *mu_muid_trackcov_d0_phi;
   std::vector<float>   *mu_muid_trackcov_d0_theta;
   std::vector<float>   *mu_muid_trackcov_d0_qoverp;
   std::vector<float>   *mu_muid_trackcov_z0_phi;
   std::vector<float>   *mu_muid_trackcov_z0_theta;
   std::vector<float>   *mu_muid_trackcov_z0_qoverp;
   std::vector<float>   *mu_muid_trackcov_phi_theta;
   std::vector<float>   *mu_muid_trackcov_phi_qoverp;
   std::vector<float>   *mu_muid_trackcov_theta_qoverp;
   std::vector<float>   *mu_muid_trackfitchi2;
   std::vector<int>     *mu_muid_trackfitndof;
   std::vector<int>     *mu_muid_hastrack;
   std::vector<float>   *mu_muid_truth_dr;
   std::vector<float>   *mu_muid_truth_E;
   std::vector<float>   *mu_muid_truth_pt;
   std::vector<float>   *mu_muid_truth_eta;
   std::vector<float>   *mu_muid_truth_phi;
   std::vector<int>     *mu_muid_truth_type;
   std::vector<int>     *mu_muid_truth_status;
   std::vector<int>     *mu_muid_truth_barcode;
   std::vector<int>     *mu_muid_truth_mothertype;
   std::vector<int>     *mu_muid_truth_motherbarcode;
   std::vector<int>     *mu_muid_truth_matched;
   std::vector<float>   *mu_muid_EFCB_dr;
   std::vector<int>     *mu_muid_EFCB_index;
   std::vector<float>   *mu_muid_EFMG_dr;
   std::vector<int>     *mu_muid_EFMG_index;
   std::vector<float>   *mu_muid_EFME_dr;
   std::vector<int>     *mu_muid_EFME_index;
   std::vector<float>   *mu_muid_L2CB_dr;
   std::vector<int>     *mu_muid_L2CB_index;
   std::vector<float>   *mu_muid_L1_dr;
   std::vector<int>     *mu_muid_L1_index;
   Int_t           mu_staco_n;
   std::vector<float>   *mu_staco_E;
   std::vector<float>   *mu_staco_pt;
   std::vector<float>   *mu_staco_m;
   std::vector<float>   *mu_staco_eta;
   std::vector<float>   *mu_staco_phi;
   std::vector<float>   *mu_staco_px;
   std::vector<float>   *mu_staco_py;
   std::vector<float>   *mu_staco_pz;
   std::vector<float>   *mu_staco_charge;
   std::vector<unsigned short> *mu_staco_allauthor;
   std::vector<int>     *mu_staco_author;
   std::vector<float>   *mu_staco_beta;
   std::vector<float>   *mu_staco_isMuonLikelihood;
   std::vector<float>   *mu_staco_matchchi2;
   std::vector<int>     *mu_staco_matchndof;
   std::vector<float>   *mu_staco_etcone20;
   std::vector<float>   *mu_staco_etcone30;
   std::vector<float>   *mu_staco_etcone40;
   std::vector<float>   *mu_staco_nucone20;
   std::vector<float>   *mu_staco_nucone30;
   std::vector<float>   *mu_staco_nucone40;
   std::vector<float>   *mu_staco_ptcone20;
   std::vector<float>   *mu_staco_ptcone30;
   std::vector<float>   *mu_staco_ptcone40;
   std::vector<float>   *mu_staco_energyLossPar;
   std::vector<float>   *mu_staco_energyLossErr;
   std::vector<float>   *mu_staco_etCore;
   std::vector<float>   *mu_staco_energyLossType;
   std::vector<unsigned short> *mu_staco_caloMuonIdTag;
   std::vector<double>  *mu_staco_caloLRLikelihood;
   std::vector<int>     *mu_staco_bestMatch;
   std::vector<int>     *mu_staco_isStandAloneMuon;
   std::vector<int>     *mu_staco_isCombinedMuon;
   std::vector<int>     *mu_staco_isLowPtReconstructedMuon;
   std::vector<int>     *mu_staco_loose;
   std::vector<int>     *mu_staco_medium;
   std::vector<int>     *mu_staco_tight;
   std::vector<float>   *mu_staco_d0_exPV;
   std::vector<float>   *mu_staco_z0_exPV;
   std::vector<float>   *mu_staco_phi_exPV;
   std::vector<float>   *mu_staco_theta_exPV;
   std::vector<float>   *mu_staco_qoverp_exPV;
   std::vector<float>   *mu_staco_cb_d0_exPV;
   std::vector<float>   *mu_staco_cb_z0_exPV;
   std::vector<float>   *mu_staco_cb_phi_exPV;
   std::vector<float>   *mu_staco_cb_theta_exPV;
   std::vector<float>   *mu_staco_cb_qoverp_exPV;
   std::vector<float>   *mu_staco_id_d0_exPV;
   std::vector<float>   *mu_staco_id_z0_exPV;
   std::vector<float>   *mu_staco_id_phi_exPV;
   std::vector<float>   *mu_staco_id_theta_exPV;
   std::vector<float>   *mu_staco_id_qoverp_exPV;
   std::vector<float>   *mu_staco_me_d0_exPV;
   std::vector<float>   *mu_staco_me_z0_exPV;
   std::vector<float>   *mu_staco_me_phi_exPV;
   std::vector<float>   *mu_staco_me_theta_exPV;
   std::vector<float>   *mu_staco_me_qoverp_exPV;
   std::vector<float>   *mu_staco_ie_d0_exPV;
   std::vector<float>   *mu_staco_ie_z0_exPV;
   std::vector<float>   *mu_staco_ie_phi_exPV;
   std::vector<float>   *mu_staco_ie_theta_exPV;
   std::vector<float>   *mu_staco_ie_qoverp_exPV;
   std::vector<std::vector<int> > *mu_staco_SpaceTime_detID;
   std::vector<std::vector<float> > *mu_staco_SpaceTime_t;
   std::vector<std::vector<float> > *mu_staco_SpaceTime_tError;
   std::vector<std::vector<float> > *mu_staco_SpaceTime_weight;
   std::vector<std::vector<float> > *mu_staco_SpaceTime_x;
   std::vector<std::vector<float> > *mu_staco_SpaceTime_y;
   std::vector<std::vector<float> > *mu_staco_SpaceTime_z;
   std::vector<std::vector<float> > *mu_staco_SpaceTime_t_Tile;
   std::vector<std::vector<float> > *mu_staco_SpaceTime_tError_Tile;
   std::vector<std::vector<float> > *mu_staco_SpaceTime_weight_Tile;
   std::vector<std::vector<float> > *mu_staco_SpaceTime_x_Tile;
   std::vector<std::vector<float> > *mu_staco_SpaceTime_y_Tile;
   std::vector<std::vector<float> > *mu_staco_SpaceTime_z_Tile;
   std::vector<std::vector<float> > *mu_staco_SpaceTime_t_TRT;
   std::vector<std::vector<float> > *mu_staco_SpaceTime_tError_TRT;
   std::vector<std::vector<float> > *mu_staco_SpaceTime_weight_TRT;
   std::vector<std::vector<float> > *mu_staco_SpaceTime_x_TRT;
   std::vector<std::vector<float> > *mu_staco_SpaceTime_y_TRT;
   std::vector<std::vector<float> > *mu_staco_SpaceTime_z_TRT;
   std::vector<std::vector<float> > *mu_staco_SpaceTime_t_MDT;
   std::vector<std::vector<float> > *mu_staco_SpaceTime_tError_MDT;
   std::vector<std::vector<float> > *mu_staco_SpaceTime_weight_MDT;
   std::vector<std::vector<float> > *mu_staco_SpaceTime_x_MDT;
   std::vector<std::vector<float> > *mu_staco_SpaceTime_y_MDT;
   std::vector<std::vector<float> > *mu_staco_SpaceTime_z_MDT;
   std::vector<float>   *mu_staco_TileCellEnergyLayer1;
   std::vector<float>   *mu_staco_TileTimeLayer1;
   std::vector<float>   *mu_staco_TileCellRmsNoiseLayer1;
   std::vector<float>   *mu_staco_TileCellSignLayer1;
   std::vector<float>   *mu_staco_TileCellEnergyLayer2;
   std::vector<float>   *mu_staco_TileTimeLayer2;
   std::vector<float>   *mu_staco_TileCellRmsNoiseLayer2;
   std::vector<float>   *mu_staco_TileCellSignLayer2;
   std::vector<float>   *mu_staco_TileCellEnergyLayer3;
   std::vector<float>   *mu_staco_TileTimeLayer3;
   std::vector<float>   *mu_staco_TileCellRmsNoiseLayer3;
   std::vector<float>   *mu_staco_TileCellSignLayer3;
   std::vector<float>   *mu_staco_MSTrkT0_1;
   std::vector<float>   *mu_staco_MSTrkT0_2;
   std::vector<float>   *mu_staco_MSTrkT0_3;
   std::vector<float>   *mu_staco_cov_d0_exPV;
   std::vector<float>   *mu_staco_cov_z0_exPV;
   std::vector<float>   *mu_staco_cov_phi_exPV;
   std::vector<float>   *mu_staco_cov_theta_exPV;
   std::vector<float>   *mu_staco_cov_qoverp_exPV;
   std::vector<float>   *mu_staco_cov_d0_z0_exPV;
   std::vector<float>   *mu_staco_cov_d0_phi_exPV;
   std::vector<float>   *mu_staco_cov_d0_theta_exPV;
   std::vector<float>   *mu_staco_cov_d0_qoverp_exPV;
   std::vector<float>   *mu_staco_cov_z0_phi_exPV;
   std::vector<float>   *mu_staco_cov_z0_theta_exPV;
   std::vector<float>   *mu_staco_cov_z0_qoverp_exPV;
   std::vector<float>   *mu_staco_cov_phi_theta_exPV;
   std::vector<float>   *mu_staco_cov_phi_qoverp_exPV;
   std::vector<float>   *mu_staco_cov_theta_qoverp_exPV;
   std::vector<float>   *mu_staco_id_cov_d0_exPV;
   std::vector<float>   *mu_staco_id_cov_z0_exPV;
   std::vector<float>   *mu_staco_id_cov_phi_exPV;
   std::vector<float>   *mu_staco_id_cov_theta_exPV;
   std::vector<float>   *mu_staco_id_cov_qoverp_exPV;
   std::vector<float>   *mu_staco_id_cov_d0_z0_exPV;
   std::vector<float>   *mu_staco_id_cov_d0_phi_exPV;
   std::vector<float>   *mu_staco_id_cov_d0_theta_exPV;
   std::vector<float>   *mu_staco_id_cov_d0_qoverp_exPV;
   std::vector<float>   *mu_staco_id_cov_z0_phi_exPV;
   std::vector<float>   *mu_staco_id_cov_z0_theta_exPV;
   std::vector<float>   *mu_staco_id_cov_z0_qoverp_exPV;
   std::vector<float>   *mu_staco_id_cov_phi_theta_exPV;
   std::vector<float>   *mu_staco_id_cov_phi_qoverp_exPV;
   std::vector<float>   *mu_staco_id_cov_theta_qoverp_exPV;
   std::vector<float>   *mu_staco_me_cov_d0_exPV;
   std::vector<float>   *mu_staco_me_cov_z0_exPV;
   std::vector<float>   *mu_staco_me_cov_phi_exPV;
   std::vector<float>   *mu_staco_me_cov_theta_exPV;
   std::vector<float>   *mu_staco_me_cov_qoverp_exPV;
   std::vector<float>   *mu_staco_me_cov_d0_z0_exPV;
   std::vector<float>   *mu_staco_me_cov_d0_phi_exPV;
   std::vector<float>   *mu_staco_me_cov_d0_theta_exPV;
   std::vector<float>   *mu_staco_me_cov_d0_qoverp_exPV;
   std::vector<float>   *mu_staco_me_cov_z0_phi_exPV;
   std::vector<float>   *mu_staco_me_cov_z0_theta_exPV;
   std::vector<float>   *mu_staco_me_cov_z0_qoverp_exPV;
   std::vector<float>   *mu_staco_me_cov_phi_theta_exPV;
   std::vector<float>   *mu_staco_me_cov_phi_qoverp_exPV;
   std::vector<float>   *mu_staco_me_cov_theta_qoverp_exPV;
   std::vector<float>   *mu_staco_ms_d0;
   std::vector<float>   *mu_staco_ms_z0;
   std::vector<float>   *mu_staco_ms_phi;
   std::vector<float>   *mu_staco_ms_theta;
   std::vector<float>   *mu_staco_ms_qoverp;
   std::vector<float>   *mu_staco_id_d0;
   std::vector<float>   *mu_staco_id_z0;
   std::vector<float>   *mu_staco_id_phi;
   std::vector<float>   *mu_staco_id_theta;
   std::vector<float>   *mu_staco_id_qoverp;
   std::vector<float>   *mu_staco_me_d0;
   std::vector<float>   *mu_staco_me_z0;
   std::vector<float>   *mu_staco_me_phi;
   std::vector<float>   *mu_staco_me_theta;
   std::vector<float>   *mu_staco_me_qoverp;
   std::vector<float>   *mu_staco_ie_d0;
   std::vector<float>   *mu_staco_ie_z0;
   std::vector<float>   *mu_staco_ie_phi;
   std::vector<float>   *mu_staco_ie_theta;
   std::vector<float>   *mu_staco_ie_qoverp;
   std::vector<int>     *mu_staco_nOutliersOnTrack;
   std::vector<int>     *mu_staco_nBLHits;
   std::vector<int>     *mu_staco_nPixHits;
   std::vector<int>     *mu_staco_nSCTHits;
   std::vector<int>     *mu_staco_nTRTHits;
   std::vector<int>     *mu_staco_nTRTHighTHits;
   std::vector<int>     *mu_staco_nBLSharedHits;
   std::vector<int>     *mu_staco_nPixSharedHits;
   std::vector<int>     *mu_staco_nPixHoles;
   std::vector<int>     *mu_staco_nSCTSharedHits;
   std::vector<int>     *mu_staco_nSCTHoles;
   std::vector<int>     *mu_staco_nTRTOutliers;
   std::vector<int>     *mu_staco_nTRTHighTOutliers;
   std::vector<int>     *mu_staco_nGangedPixels;
   std::vector<int>     *mu_staco_nPixelDeadSensors;
   std::vector<int>     *mu_staco_nSCTDeadSensors;
   std::vector<int>     *mu_staco_nTRTDeadStraws;
   std::vector<int>     *mu_staco_expectBLayerHit;
   std::vector<int>     *mu_staco_nMDTHits;
   std::vector<int>     *mu_staco_nMDTHoles;
   std::vector<int>     *mu_staco_nCSCEtaHits;
   std::vector<int>     *mu_staco_nCSCEtaHoles;
   std::vector<int>     *mu_staco_nCSCPhiHits;
   std::vector<int>     *mu_staco_nCSCPhiHoles;
   std::vector<int>     *mu_staco_nRPCEtaHits;
   std::vector<int>     *mu_staco_nRPCEtaHoles;
   std::vector<int>     *mu_staco_nRPCPhiHits;
   std::vector<int>     *mu_staco_nRPCPhiHoles;
   std::vector<int>     *mu_staco_nTGCEtaHits;
   std::vector<int>     *mu_staco_nTGCEtaHoles;
   std::vector<int>     *mu_staco_nTGCPhiHits;
   std::vector<int>     *mu_staco_nTGCPhiHoles;
   std::vector<int>     *mu_staco_nMDTBIHits;
   std::vector<int>     *mu_staco_nMDTBMHits;
   std::vector<int>     *mu_staco_nMDTBOHits;
   std::vector<int>     *mu_staco_nMDTBEEHits;
   std::vector<int>     *mu_staco_nMDTBIS78Hits;
   std::vector<int>     *mu_staco_nMDTEIHits;
   std::vector<int>     *mu_staco_nMDTEMHits;
   std::vector<int>     *mu_staco_nMDTEOHits;
   std::vector<int>     *mu_staco_nMDTEEHits;
   std::vector<int>     *mu_staco_nRPCLayer1EtaHits;
   std::vector<int>     *mu_staco_nRPCLayer2EtaHits;
   std::vector<int>     *mu_staco_nRPCLayer3EtaHits;
   std::vector<int>     *mu_staco_nRPCLayer1PhiHits;
   std::vector<int>     *mu_staco_nRPCLayer2PhiHits;
   std::vector<int>     *mu_staco_nRPCLayer3PhiHits;
   std::vector<int>     *mu_staco_nTGCLayer1EtaHits;
   std::vector<int>     *mu_staco_nTGCLayer2EtaHits;
   std::vector<int>     *mu_staco_nTGCLayer3EtaHits;
   std::vector<int>     *mu_staco_nTGCLayer4EtaHits;
   std::vector<int>     *mu_staco_nTGCLayer1PhiHits;
   std::vector<int>     *mu_staco_nTGCLayer2PhiHits;
   std::vector<int>     *mu_staco_nTGCLayer3PhiHits;
   std::vector<int>     *mu_staco_nTGCLayer4PhiHits;
   std::vector<int>     *mu_staco_barrelSectors;
   std::vector<int>     *mu_staco_endcapSectors;
   std::vector<float>   *mu_staco_trackd0;
   std::vector<float>   *mu_staco_trackz0;
   std::vector<float>   *mu_staco_trackphi;
   std::vector<float>   *mu_staco_tracktheta;
   std::vector<float>   *mu_staco_trackqoverp;
   std::vector<float>   *mu_staco_trackcov_d0;
   std::vector<float>   *mu_staco_trackcov_z0;
   std::vector<float>   *mu_staco_trackcov_phi;
   std::vector<float>   *mu_staco_trackcov_theta;
   std::vector<float>   *mu_staco_trackcov_qoverp;
   std::vector<float>   *mu_staco_trackcov_d0_z0;
   std::vector<float>   *mu_staco_trackcov_d0_phi;
   std::vector<float>   *mu_staco_trackcov_d0_theta;
   std::vector<float>   *mu_staco_trackcov_d0_qoverp;
   std::vector<float>   *mu_staco_trackcov_z0_phi;
   std::vector<float>   *mu_staco_trackcov_z0_theta;
   std::vector<float>   *mu_staco_trackcov_z0_qoverp;
   std::vector<float>   *mu_staco_trackcov_phi_theta;
   std::vector<float>   *mu_staco_trackcov_phi_qoverp;
   std::vector<float>   *mu_staco_trackcov_theta_qoverp;
   std::vector<float>   *mu_staco_trackfitchi2;
   std::vector<int>     *mu_staco_trackfitndof;
   std::vector<int>     *mu_staco_hastrack;
   std::vector<float>   *mu_staco_truth_dr;
   std::vector<float>   *mu_staco_truth_E;
   std::vector<float>   *mu_staco_truth_pt;
   std::vector<float>   *mu_staco_truth_eta;
   std::vector<float>   *mu_staco_truth_phi;
   std::vector<int>     *mu_staco_truth_type;
   std::vector<int>     *mu_staco_truth_status;
   std::vector<int>     *mu_staco_truth_barcode;
   std::vector<int>     *mu_staco_truth_mothertype;
   std::vector<int>     *mu_staco_truth_motherbarcode;
   std::vector<int>     *mu_staco_truth_matched;
   std::vector<float>   *mu_staco_EFCB_dr;
   std::vector<int>     *mu_staco_EFCB_index;
   std::vector<float>   *mu_staco_EFMG_dr;
   std::vector<int>     *mu_staco_EFMG_index;
   std::vector<float>   *mu_staco_EFME_dr;
   std::vector<int>     *mu_staco_EFME_index;
   std::vector<float>   *mu_staco_L2CB_dr;
   std::vector<int>     *mu_staco_L2CB_index;
   std::vector<float>   *mu_staco_L1_dr;
   std::vector<int>     *mu_staco_L1_index;
   Int_t           mu_calo_n;
   std::vector<float>   *mu_calo_E;
   std::vector<float>   *mu_calo_pt;
   std::vector<float>   *mu_calo_m;
   std::vector<float>   *mu_calo_eta;
   std::vector<float>   *mu_calo_phi;
   std::vector<float>   *mu_calo_px;
   std::vector<float>   *mu_calo_py;
   std::vector<float>   *mu_calo_pz;
   std::vector<float>   *mu_calo_charge;
   std::vector<unsigned short> *mu_calo_allauthor;
   std::vector<int>     *mu_calo_hastrack;
   std::vector<float>   *mu_calo_truth_dr;
   std::vector<float>   *mu_calo_truth_E;
   std::vector<float>   *mu_calo_truth_pt;
   std::vector<float>   *mu_calo_truth_eta;
   std::vector<float>   *mu_calo_truth_phi;
   std::vector<int>     *mu_calo_truth_type;
   std::vector<int>     *mu_calo_truth_status;
   std::vector<int>     *mu_calo_truth_barcode;
   std::vector<int>     *mu_calo_truth_mothertype;
   std::vector<int>     *mu_calo_truth_motherbarcode;
   std::vector<int>     *mu_calo_truth_matched;
   Int_t           tau_n;
   std::vector<float>   *tau_Et;
   std::vector<float>   *tau_pt;
   std::vector<float>   *tau_m;
   std::vector<float>   *tau_eta;
   std::vector<float>   *tau_phi;
   std::vector<float>   *tau_charge;
   std::vector<float>   *tau_BDTEleScore;
   std::vector<float>   *tau_BDTJetScore;
   std::vector<float>   *tau_discCut;
   std::vector<float>   *tau_discCutTMVA;
   std::vector<float>   *tau_discLL;
   std::vector<float>   *tau_discNN;
   std::vector<float>   *tau_efficLL;
   std::vector<float>   *tau_efficNN;
   std::vector<float>   *tau_likelihood;
   std::vector<float>   *tau_tauJetNeuralNetwork;
   std::vector<float>   *tau_tauENeuralNetwork;
   std::vector<float>   *tau_tauElTauLikelihood;
   std::vector<float>   *tau_SafeLikelihood;
   std::vector<int>     *tau_electronVetoLoose;
   std::vector<int>     *tau_electronVetoMedium;
   std::vector<int>     *tau_electronVetoTight;
   std::vector<int>     *tau_muonVeto;
   std::vector<int>     *tau_tauCutLoose;
   std::vector<int>     *tau_tauCutMedium;
   std::vector<int>     *tau_tauCutTight;
   std::vector<int>     *tau_tauCutSafeLoose;
   std::vector<int>     *tau_tauCutSafeMedium;
   std::vector<int>     *tau_tauCutSafeTight;
   std::vector<int>     *tau_tauCutSafeCaloLoose;
   std::vector<int>     *tau_tauCutSafeCaloMedium;
   std::vector<int>     *tau_tauCutSafeCaloTight;
   std::vector<int>     *tau_tauLlhLoose;
   std::vector<int>     *tau_tauLlhMedium;
   std::vector<int>     *tau_tauLlhTight;
   std::vector<int>     *tau_JetBDTLoose;
   std::vector<int>     *tau_JetBDTMedium;
   std::vector<int>     *tau_JetBDTTight;
   std::vector<int>     *tau_EleBDTLoose;
   std::vector<int>     *tau_EleBDTMedium;
   std::vector<int>     *tau_EleBDTTight;
   std::vector<int>     *tau_SafeLlhLoose;
   std::vector<int>     *tau_SafeLlhMedium;
   std::vector<int>     *tau_SafeLlhTight;
   std::vector<int>     *tau_author;
   std::vector<int>     *tau_ROIword;
   std::vector<int>     *tau_nProng;
   std::vector<int>     *tau_numTrack;
   std::vector<int>     *tau_seedCalo_numTrack;
   std::vector<float>   *tau_etOverPtLeadTrk;
   std::vector<float>   *tau_ipZ0SinThetaSigLeadTrk;
   std::vector<float>   *tau_leadTrkPt;
   std::vector<int>     *tau_nLooseTrk;
   std::vector<int>     *tau_nLooseConvTrk;
   std::vector<int>     *tau_nProngLoose;
   std::vector<float>   *tau_ipSigLeadTrk;
   std::vector<float>   *tau_ipSigLeadLooseTrk;
   std::vector<float>   *tau_etOverPtLeadLooseTrk;
   std::vector<float>   *tau_leadLooseTrkPt;
   std::vector<float>   *tau_chrgLooseTrk;
   std::vector<float>   *tau_massTrkSys;
   std::vector<float>   *tau_trkWidth2;
   std::vector<float>   *tau_trFlightPathSig;
   std::vector<float>   *tau_etEflow;
   std::vector<float>   *tau_mEflow;
   std::vector<int>     *tau_nPi0;
   std::vector<float>   *tau_ele_E237E277;
   std::vector<float>   *tau_ele_PresamplerFraction;
   std::vector<float>   *tau_ele_ECALFirstFraction;
   std::vector<float>   *tau_seedCalo_EMRadius;
   std::vector<float>   *tau_seedCalo_hadRadius;
   std::vector<float>   *tau_seedCalo_etEMAtEMScale;
   std::vector<float>   *tau_seedCalo_etHadAtEMScale;
   std::vector<float>   *tau_seedCalo_isolFrac;
   std::vector<float>   *tau_seedCalo_centFrac;
   std::vector<float>   *tau_seedCalo_stripWidth2;
   std::vector<int>     *tau_seedCalo_nStrip;
   std::vector<float>   *tau_seedCalo_etEMCalib;
   std::vector<float>   *tau_seedCalo_etHadCalib;
   std::vector<float>   *tau_seedCalo_eta;
   std::vector<float>   *tau_seedCalo_phi;
   std::vector<float>   *tau_seedCalo_nIsolLooseTrk;
   std::vector<float>   *tau_seedCalo_trkAvgDist;
   std::vector<float>   *tau_seedCalo_trkRmsDist;
   std::vector<int>     *tau_numTopoClusters;
   std::vector<float>   *tau_numEffTopoClusters;
   std::vector<float>   *tau_topoInvMass;
   std::vector<float>   *tau_effTopoInvMass;
   std::vector<float>   *tau_topoMeanDeltaR;
   std::vector<float>   *tau_effTopoMeanDeltaR;
   std::vector<float>   *tau_numCells;
   std::vector<float>   *tau_seedTrk_EMRadius;
   std::vector<float>   *tau_seedTrk_isolFrac;
   std::vector<float>   *tau_seedTrk_etChrgHadOverSumTrkPt;
   std::vector<float>   *tau_seedTrk_isolFracWide;
   std::vector<float>   *tau_seedTrk_etHadAtEMScale;
   std::vector<float>   *tau_seedTrk_etEMAtEMScale;
   std::vector<float>   *tau_seedTrk_etEMCL;
   std::vector<float>   *tau_seedTrk_etChrgEM;
   std::vector<float>   *tau_seedTrk_etNeuEM;
   std::vector<float>   *tau_seedTrk_etResNeuEM;
   std::vector<float>   *tau_seedTrk_hadLeakEt;
   std::vector<float>   *tau_seedTrk_sumEMCellEtOverLeadTrkPt;
   std::vector<float>   *tau_seedTrk_secMaxStripEt;
   std::vector<float>   *tau_seedTrk_stripWidth2;
   std::vector<int>     *tau_seedTrk_nStrip;
   std::vector<float>   *tau_seedTrk_etChrgHad;
   std::vector<int>     *tau_seedTrk_nOtherCoreTrk;
   std::vector<int>     *tau_seedTrk_nIsolTrk;
   std::vector<float>   *tau_seedTrk_etIsolEM;
   std::vector<float>   *tau_seedTrk_etIsolHad;
   std::vector<float>   *tau_calcVars_sumTrkPt;
   std::vector<float>   *tau_calcVars_etHadSumPtTracks;
   std::vector<float>   *tau_calcVars_etEMSumPtTracks;
   std::vector<float>   *tau_calcVars_etHad_EMScale_Pt3Trks;
   std::vector<float>   *tau_calcVars_etEM_EMScale_Pt3Trks;
   std::vector<float>   *tau_calcVars_mass;
   std::vector<float>   *tau_calcVars_ipSigLeadLooseTrk;
   std::vector<float>   *tau_calcVars_drMax;
   std::vector<float>   *tau_calcVars_drMin;
   std::vector<float>   *tau_calcVars_emFracCalib;
   std::vector<float>   *tau_calcVars_TRTHTOverLT_LeadTrk;
   std::vector<float>   *tau_calcVars_calRadius;
   std::vector<float>   *tau_calcVars_EMFractionAtEMScale;
   std::vector<int>     *tau_calcVars_BDTLooseBkg;
   std::vector<int>     *tau_calcVars_BDTMediumBkg;
   std::vector<int>     *tau_calcVars_BDTTightBkg;
   std::vector<int>     *tau_seedCalo_track_n;
   std::vector<int>     *tau_seedCalo_wideTrk_n;
   std::vector<int>     *tau_otherTrk_n;
   std::vector<float>   *tau_EF_dr;
   std::vector<float>   *tau_EF_E;
   std::vector<float>   *tau_EF_Et;
   std::vector<float>   *tau_EF_pt;
   std::vector<float>   *tau_EF_eta;
   std::vector<float>   *tau_EF_phi;
   std::vector<int>     *tau_EF_matched;
   std::vector<float>   *tau_L2_dr;
   std::vector<float>   *tau_L2_E;
   std::vector<float>   *tau_L2_Et;
   std::vector<float>   *tau_L2_pt;
   std::vector<float>   *tau_L2_eta;
   std::vector<float>   *tau_L2_phi;
   std::vector<int>     *tau_L2_matched;
   std::vector<float>   *tau_L1_dr;
   std::vector<float>   *tau_L1_Et;
   std::vector<float>   *tau_L1_pt;
   std::vector<float>   *tau_L1_eta;
   std::vector<float>   *tau_L1_phi;
   std::vector<int>     *tau_L1_matched;
   Int_t           jet_AntiKt4TopoEM_n;
   std::vector<float>   *jet_AntiKt4TopoEM_E;
   std::vector<float>   *jet_AntiKt4TopoEM_pt;
   std::vector<float>   *jet_AntiKt4TopoEM_m;
   std::vector<float>   *jet_AntiKt4TopoEM_eta;
   std::vector<float>   *jet_AntiKt4TopoEM_phi;
   std::vector<float>   *jet_AntiKt4TopoEM_EtaOrigin;
   std::vector<float>   *jet_AntiKt4TopoEM_PhiOrigin;
   std::vector<float>   *jet_AntiKt4TopoEM_MOrigin;
   std::vector<float>   *jet_AntiKt4TopoEM_EtaOriginEM;
   std::vector<float>   *jet_AntiKt4TopoEM_PhiOriginEM;
   std::vector<float>   *jet_AntiKt4TopoEM_MOriginEM;
   std::vector<float>   *jet_AntiKt4TopoEM_WIDTH;
   std::vector<float>   *jet_AntiKt4TopoEM_n90;
   std::vector<float>   *jet_AntiKt4TopoEM_Timing;
   std::vector<float>   *jet_AntiKt4TopoEM_LArQuality;
   std::vector<float>   *jet_AntiKt4TopoEM_nTrk;
   std::vector<float>   *jet_AntiKt4TopoEM_sumPtTrk;
   std::vector<float>   *jet_AntiKt4TopoEM_OriginIndex;
   std::vector<float>   *jet_AntiKt4TopoEM_HECQuality;
   std::vector<float>   *jet_AntiKt4TopoEM_NegativeE;
   std::vector<float>   *jet_AntiKt4TopoEM_BCH_CORR_CELL;
   std::vector<float>   *jet_AntiKt4TopoEM_BCH_CORR_DOTX;
   std::vector<float>   *jet_AntiKt4TopoEM_BCH_CORR_JET;
   std::vector<float>   *jet_AntiKt4TopoEM_BCH_CORR_JET_FORCELL;
   std::vector<float>   *jet_AntiKt4TopoEM_ENG_BAD_CELLS;
   std::vector<float>   *jet_AntiKt4TopoEM_N_BAD_CELLS;
   std::vector<float>   *jet_AntiKt4TopoEM_N_BAD_CELLS_CORR;
   std::vector<float>   *jet_AntiKt4TopoEM_BAD_CELLS_CORR_E;
   std::vector<int>     *jet_AntiKt4TopoEM_SamplingMax;
   std::vector<double>  *jet_AntiKt4TopoEM_fracSamplingMax;
   std::vector<float>   *jet_AntiKt4TopoEM_hecf;
   std::vector<float>   *jet_AntiKt4TopoEM_tgap3f;
   std::vector<int>     *jet_AntiKt4TopoEM_isUgly;
   std::vector<int>     *jet_AntiKt4TopoEM_isBadLoose;
   std::vector<int>     *jet_AntiKt4TopoEM_isBadTight;
   std::vector<float>   *jet_AntiKt4TopoEM_emfrac;
   std::vector<float>   *jet_AntiKt4TopoEM_Offset;
   std::vector<float>   *jet_AntiKt4TopoEM_EMJES;
   std::vector<float>   *jet_AntiKt4TopoEM_EMJES_EtaCorr;
   std::vector<float>   *jet_AntiKt4TopoEM_EMJESnooffset;
   std::vector<float>   *jet_AntiKt4TopoEM_GCWJES;
   std::vector<float>   *jet_AntiKt4TopoEM_GCWJES_EtaCorr;
   std::vector<float>   *jet_AntiKt4TopoEM_CB;
   std::vector<float>   *jet_AntiKt4TopoEM_emscale_E;
   std::vector<float>   *jet_AntiKt4TopoEM_emscale_pt;
   std::vector<float>   *jet_AntiKt4TopoEM_emscale_m;
   std::vector<float>   *jet_AntiKt4TopoEM_emscale_eta;
   std::vector<float>   *jet_AntiKt4TopoEM_emscale_phi;
   std::vector<float>   *jet_AntiKt4TopoEM_jvtxf;
   std::vector<float>   *jet_AntiKt4TopoEM_jvtx_x;
   std::vector<float>   *jet_AntiKt4TopoEM_jvtx_y;
   std::vector<float>   *jet_AntiKt4TopoEM_jvtx_z;
   std::vector<float>   *jet_AntiKt4TopoEM_GSCFactorF;
   std::vector<float>   *jet_AntiKt4TopoEM_WidthFraction;
   std::vector<double>  *jet_AntiKt4TopoEM_flavor_component_ip2d_pu;
   std::vector<double>  *jet_AntiKt4TopoEM_flavor_component_ip2d_pb;
   std::vector<double>  *jet_AntiKt4TopoEM_flavor_component_ip2d_ntrk;
   std::vector<double>  *jet_AntiKt4TopoEM_flavor_component_ip3d_pu;
   std::vector<double>  *jet_AntiKt4TopoEM_flavor_component_ip3d_pb;
   std::vector<double>  *jet_AntiKt4TopoEM_flavor_component_ip3d_ntrk;
   std::vector<double>  *jet_AntiKt4TopoEM_flavor_component_sv1_pu;
   std::vector<double>  *jet_AntiKt4TopoEM_flavor_component_sv1_pb;
   std::vector<double>  *jet_AntiKt4TopoEM_flavor_component_sv2_pu;
   std::vector<double>  *jet_AntiKt4TopoEM_flavor_component_sv2_pb;
   std::vector<double>  *jet_AntiKt4TopoEM_flavor_component_svp_mass;
   std::vector<double>  *jet_AntiKt4TopoEM_flavor_component_svp_n2t;
   std::vector<double>  *jet_AntiKt4TopoEM_flavor_component_svp_svok;
   std::vector<double>  *jet_AntiKt4TopoEM_flavor_component_svp_ntrk;
   std::vector<double>  *jet_AntiKt4TopoEM_flavor_component_svp_ntrkv;
   std::vector<double>  *jet_AntiKt4TopoEM_flavor_component_svp_ntrkj;
   std::vector<double>  *jet_AntiKt4TopoEM_flavor_component_svp_efrc;
   std::vector<double>  *jet_AntiKt4TopoEM_flavor_component_sv0p_mass;
   std::vector<double>  *jet_AntiKt4TopoEM_flavor_component_sv0p_n2t;
   std::vector<double>  *jet_AntiKt4TopoEM_flavor_component_sv0p_svok;
   std::vector<double>  *jet_AntiKt4TopoEM_flavor_component_sv0p_ntrk;
   std::vector<double>  *jet_AntiKt4TopoEM_flavor_component_sv0p_ntrkv;
   std::vector<double>  *jet_AntiKt4TopoEM_flavor_component_sv0p_ntrkj;
   std::vector<double>  *jet_AntiKt4TopoEM_flavor_component_sv0p_efrc;
   std::vector<double>  *jet_AntiKt4TopoEM_flavor_component_jfit_pu;
   std::vector<double>  *jet_AntiKt4TopoEM_flavor_component_jfit_pb;
   std::vector<double>  *jet_AntiKt4TopoEM_flavor_component_jfit_nvtxnt;
   std::vector<double>  *jet_AntiKt4TopoEM_flavor_component_jfit_nvtx1t;
   std::vector<double>  *jet_AntiKt4TopoEM_flavor_component_jfit_ntrk;
   std::vector<double>  *jet_AntiKt4TopoEM_flavor_component_jfit_efrc;
   std::vector<double>  *jet_AntiKt4TopoEM_flavor_component_jfit_mass;
   std::vector<double>  *jet_AntiKt4TopoEM_flavor_component_jfit_sig3d;
   std::vector<double>  *jet_AntiKt4TopoEM_flavor_weight_TrackCounting2D;
   std::vector<double>  *jet_AntiKt4TopoEM_flavor_weight_JetProb;
   std::vector<double>  *jet_AntiKt4TopoEM_flavor_weight_IP1D;
   std::vector<double>  *jet_AntiKt4TopoEM_flavor_weight_IP2D;
   std::vector<double>  *jet_AntiKt4TopoEM_flavor_weight_IP3D;
   std::vector<double>  *jet_AntiKt4TopoEM_flavor_weight_SV0;
   std::vector<double>  *jet_AntiKt4TopoEM_flavor_weight_SV1;
   std::vector<double>  *jet_AntiKt4TopoEM_flavor_weight_SV2;
   std::vector<double>  *jet_AntiKt4TopoEM_flavor_weight_JetFitterTag;
   std::vector<double>  *jet_AntiKt4TopoEM_flavor_weight_JetFitterCOMB;
   std::vector<double>  *jet_AntiKt4TopoEM_flavor_weight_JetFitterTagNN;
   std::vector<double>  *jet_AntiKt4TopoEM_flavor_weight_JetFitterCOMBNN;
   std::vector<double>  *jet_AntiKt4TopoEM_flavor_weight_SoftMuonTag;
   std::vector<double>  *jet_AntiKt4TopoEM_flavor_weight_SoftElectronTag;
   std::vector<double>  *jet_AntiKt4TopoEM_flavor_truth_dRminToB;
   std::vector<double>  *jet_AntiKt4TopoEM_flavor_truth_dRminToC;
   std::vector<double>  *jet_AntiKt4TopoEM_flavor_truth_dRminToT;
   std::vector<int>     *jet_AntiKt4TopoEM_flavor_truth_BHadronpdg;
   std::vector<int>     *jet_AntiKt4TopoEM_flavor_truth_trueflav;
   std::vector<float>   *jet_AntiKt4TopoEM_el_dr;
   std::vector<int>     *jet_AntiKt4TopoEM_el_matched;
   std::vector<float>   *jet_AntiKt4TopoEM_mu_dr;
   std::vector<int>     *jet_AntiKt4TopoEM_mu_matched;
   std::vector<float>   *jet_AntiKt4TopoEM_L1_dr;
   std::vector<int>     *jet_AntiKt4TopoEM_L1_matched;
   std::vector<float>   *jet_AntiKt4TopoEM_L2_dr;
   std::vector<int>     *jet_AntiKt4TopoEM_L2_matched;
   std::vector<float>   *jet_AntiKt4TopoEM_EF_dr;
   std::vector<int>     *jet_AntiKt4TopoEM_EF_matched;
   Int_t           jet_akt6topoem_n;
   std::vector<float>   *jet_akt6topoem_E;
   std::vector<float>   *jet_akt6topoem_pt;
   std::vector<float>   *jet_akt6topoem_m;
   std::vector<float>   *jet_akt6topoem_eta;
   std::vector<float>   *jet_akt6topoem_phi;
   std::vector<float>   *jet_akt6topoem_EtaOrigin;
   std::vector<float>   *jet_akt6topoem_PhiOrigin;
   std::vector<float>   *jet_akt6topoem_MOrigin;
   std::vector<float>   *jet_akt6topoem_EtaOriginEM;
   std::vector<float>   *jet_akt6topoem_PhiOriginEM;
   std::vector<float>   *jet_akt6topoem_MOriginEM;
   std::vector<float>   *jet_akt6topoem_WIDTH;
   std::vector<float>   *jet_akt6topoem_n90;
   std::vector<float>   *jet_akt6topoem_Timing;
   std::vector<float>   *jet_akt6topoem_LArQuality;
   std::vector<float>   *jet_akt6topoem_nTrk;
   std::vector<float>   *jet_akt6topoem_sumPtTrk;
   std::vector<float>   *jet_akt6topoem_OriginIndex;
   std::vector<float>   *jet_akt6topoem_HECQuality;
   std::vector<float>   *jet_akt6topoem_NegativeE;
   std::vector<float>   *jet_akt6topoem_BCH_CORR_CELL;
   std::vector<float>   *jet_akt6topoem_BCH_CORR_DOTX;
   std::vector<float>   *jet_akt6topoem_BCH_CORR_JET;
   std::vector<float>   *jet_akt6topoem_BCH_CORR_JET_FORCELL;
   std::vector<float>   *jet_akt6topoem_ENG_BAD_CELLS;
   std::vector<float>   *jet_akt6topoem_N_BAD_CELLS;
   std::vector<float>   *jet_akt6topoem_N_BAD_CELLS_CORR;
   std::vector<float>   *jet_akt6topoem_BAD_CELLS_CORR_E;
   std::vector<int>     *jet_akt6topoem_SamplingMax;
   std::vector<double>  *jet_akt6topoem_fracSamplingMax;
   std::vector<float>   *jet_akt6topoem_hecf;
   std::vector<float>   *jet_akt6topoem_tgap3f;
   std::vector<int>     *jet_akt6topoem_isUgly;
   std::vector<int>     *jet_akt6topoem_isBadLoose;
   std::vector<int>     *jet_akt6topoem_isBadTight;
   std::vector<float>   *jet_akt6topoem_emfrac;
   std::vector<float>   *jet_akt6topoem_Offset;
   std::vector<float>   *jet_akt6topoem_EMJES;
   std::vector<float>   *jet_akt6topoem_EMJES_EtaCorr;
   std::vector<float>   *jet_akt6topoem_EMJESnooffset;
   std::vector<float>   *jet_akt6topoem_GCWJES;
   std::vector<float>   *jet_akt6topoem_GCWJES_EtaCorr;
   std::vector<float>   *jet_akt6topoem_CB;
   std::vector<float>   *jet_akt6topoem_emscale_E;
   std::vector<float>   *jet_akt6topoem_emscale_pt;
   std::vector<float>   *jet_akt6topoem_emscale_m;
   std::vector<float>   *jet_akt6topoem_emscale_eta;
   std::vector<float>   *jet_akt6topoem_emscale_phi;
   std::vector<float>   *jet_akt6topoem_jvtxf;
   std::vector<float>   *jet_akt6topoem_jvtx_x;
   std::vector<float>   *jet_akt6topoem_jvtx_y;
   std::vector<float>   *jet_akt6topoem_jvtx_z;
   std::vector<float>   *jet_akt6topoem_GSCFactorF;
   std::vector<float>   *jet_akt6topoem_WidthFraction;
   std::vector<double>  *jet_akt6topoem_flavor_component_ip2d_pu;
   std::vector<double>  *jet_akt6topoem_flavor_component_ip2d_pb;
   std::vector<double>  *jet_akt6topoem_flavor_component_ip2d_ntrk;
   std::vector<double>  *jet_akt6topoem_flavor_component_ip3d_pu;
   std::vector<double>  *jet_akt6topoem_flavor_component_ip3d_pb;
   std::vector<double>  *jet_akt6topoem_flavor_component_ip3d_ntrk;
   std::vector<double>  *jet_akt6topoem_flavor_component_sv1_pu;
   std::vector<double>  *jet_akt6topoem_flavor_component_sv1_pb;
   std::vector<double>  *jet_akt6topoem_flavor_component_sv2_pu;
   std::vector<double>  *jet_akt6topoem_flavor_component_sv2_pb;
   std::vector<double>  *jet_akt6topoem_flavor_component_svp_mass;
   std::vector<double>  *jet_akt6topoem_flavor_component_svp_n2t;
   std::vector<double>  *jet_akt6topoem_flavor_component_svp_svok;
   std::vector<double>  *jet_akt6topoem_flavor_component_svp_ntrk;
   std::vector<double>  *jet_akt6topoem_flavor_component_svp_ntrkv;
   std::vector<double>  *jet_akt6topoem_flavor_component_svp_ntrkj;
   std::vector<double>  *jet_akt6topoem_flavor_component_svp_efrc;
   std::vector<double>  *jet_akt6topoem_flavor_component_sv0p_mass;
   std::vector<double>  *jet_akt6topoem_flavor_component_sv0p_n2t;
   std::vector<double>  *jet_akt6topoem_flavor_component_sv0p_svok;
   std::vector<double>  *jet_akt6topoem_flavor_component_sv0p_ntrk;
   std::vector<double>  *jet_akt6topoem_flavor_component_sv0p_ntrkv;
   std::vector<double>  *jet_akt6topoem_flavor_component_sv0p_ntrkj;
   std::vector<double>  *jet_akt6topoem_flavor_component_sv0p_efrc;
   std::vector<double>  *jet_akt6topoem_flavor_component_jfit_pu;
   std::vector<double>  *jet_akt6topoem_flavor_component_jfit_pb;
   std::vector<double>  *jet_akt6topoem_flavor_component_jfit_nvtxnt;
   std::vector<double>  *jet_akt6topoem_flavor_component_jfit_nvtx1t;
   std::vector<double>  *jet_akt6topoem_flavor_component_jfit_ntrk;
   std::vector<double>  *jet_akt6topoem_flavor_component_jfit_efrc;
   std::vector<double>  *jet_akt6topoem_flavor_component_jfit_mass;
   std::vector<double>  *jet_akt6topoem_flavor_component_jfit_sig3d;
   std::vector<double>  *jet_akt6topoem_flavor_weight_TrackCounting2D;
   std::vector<double>  *jet_akt6topoem_flavor_weight_JetProb;
   std::vector<double>  *jet_akt6topoem_flavor_weight_IP1D;
   std::vector<double>  *jet_akt6topoem_flavor_weight_IP2D;
   std::vector<double>  *jet_akt6topoem_flavor_weight_IP3D;
   std::vector<double>  *jet_akt6topoem_flavor_weight_SV0;
   std::vector<double>  *jet_akt6topoem_flavor_weight_SV1;
   std::vector<double>  *jet_akt6topoem_flavor_weight_SV2;
   std::vector<double>  *jet_akt6topoem_flavor_weight_JetFitterTag;
   std::vector<double>  *jet_akt6topoem_flavor_weight_JetFitterCOMB;
   std::vector<double>  *jet_akt6topoem_flavor_weight_JetFitterTagNN;
   std::vector<double>  *jet_akt6topoem_flavor_weight_JetFitterCOMBNN;
   std::vector<double>  *jet_akt6topoem_flavor_weight_SoftMuonTag;
   std::vector<double>  *jet_akt6topoem_flavor_weight_SoftElectronTag;
   std::vector<double>  *jet_akt6topoem_flavor_truth_dRminToB;
   std::vector<double>  *jet_akt6topoem_flavor_truth_dRminToC;
   std::vector<double>  *jet_akt6topoem_flavor_truth_dRminToT;
   std::vector<int>     *jet_akt6topoem_flavor_truth_BHadronpdg;
   std::vector<int>     *jet_akt6topoem_flavor_truth_trueflav;
   std::vector<float>   *jet_akt6topoem_el_dr;
   std::vector<int>     *jet_akt6topoem_el_matched;
   std::vector<float>   *jet_akt6topoem_mu_dr;
   std::vector<int>     *jet_akt6topoem_mu_matched;
   std::vector<float>   *jet_akt6topoem_L1_dr;
   std::vector<int>     *jet_akt6topoem_L1_matched;
   std::vector<float>   *jet_akt6topoem_L2_dr;
   std::vector<int>     *jet_akt6topoem_L2_matched;
   std::vector<float>   *jet_akt6topoem_EF_dr;
   std::vector<int>     *jet_akt6topoem_EF_matched;
   Float_t         MET_RefFinal_phi;
   Float_t         MET_RefFinal_et;
   Float_t         MET_RefFinal_sumet;
   Float_t         MET_Cryo_phi;
   Float_t         MET_Cryo_et;
   Float_t         MET_Cryo_sumet;
   Float_t         MET_RefEle_phi;
   Float_t         MET_RefEle_et;
   Float_t         MET_RefEle_sumet;
   Float_t         MET_RefJet_phi;
   Float_t         MET_RefJet_et;
   Float_t         MET_RefJet_sumet;
   Float_t         MET_SoftJets_phi;
   Float_t         MET_SoftJets_et;
   Float_t         MET_SoftJets_sumet;
   Float_t         MET_RefMuon_phi;
   Float_t         MET_RefMuon_et;
   Float_t         MET_RefMuon_sumet;
   Float_t         MET_RefMuon_Staco_phi;
   Float_t         MET_RefMuon_Staco_et;
   Float_t         MET_RefMuon_Staco_sumet;
   Float_t         MET_RefMuon_Muid_phi;
   Float_t         MET_RefMuon_Muid_et;
   Float_t         MET_RefMuon_Muid_sumet;
   Float_t         MET_RefGamma_phi;
   Float_t         MET_RefGamma_et;
   Float_t         MET_RefGamma_sumet;
   Float_t         MET_RefTau_phi;
   Float_t         MET_RefTau_et;
   Float_t         MET_RefTau_sumet;
   Float_t         MET_CellOut_phi;
   Float_t         MET_CellOut_et;
   Float_t         MET_CellOut_sumet;
   Float_t         MET_Track_phi;
   Float_t         MET_Track_et;
   Float_t         MET_Track_sumet;
   Float_t         MET_RefFinal_em_etx;
   Float_t         MET_RefFinal_em_ety;
   Float_t         MET_RefFinal_em_phi;
   Float_t         MET_RefFinal_em_et;
   Float_t         MET_RefFinal_em_sumet;
   Float_t         MET_RefEle_em_phi;
   Float_t         MET_RefEle_em_et;
   Float_t         MET_RefEle_em_sumet;
   Float_t         MET_RefJet_em_phi;
   Float_t         MET_RefJet_em_et;
   Float_t         MET_RefJet_em_sumet;
   Float_t         MET_SoftJets_em_phi;
   Float_t         MET_SoftJets_em_et;
   Float_t         MET_SoftJets_em_sumet;
   Float_t         MET_RefMuon_em_phi;
   Float_t         MET_RefMuon_em_et;
   Float_t         MET_RefMuon_em_sumet;
   Float_t         MET_RefMuon_Track_em_phi;
   Float_t         MET_RefMuon_Track_em_et;
   Float_t         MET_RefMuon_Track_em_sumet;
   Float_t         MET_RefGamma_em_phi;
   Float_t         MET_RefGamma_em_et;
   Float_t         MET_RefGamma_em_sumet;
   Float_t         MET_RefTau_em_phi;
   Float_t         MET_RefTau_em_et;
   Float_t         MET_RefTau_em_sumet;
   Float_t         MET_CellOut_em_etx;
   Float_t         MET_CellOut_em_ety;
   Float_t         MET_CellOut_em_phi;
   Float_t         MET_CellOut_em_et;
   Float_t         MET_CellOut_em_sumet;
   Float_t         MET_Muon_Isol_Staco_phi;
   Float_t         MET_Muon_Isol_Staco_et;
   Float_t         MET_Muon_Isol_Staco_sumet;
   Float_t         MET_Muon_NonIsol_Staco_phi;
   Float_t         MET_Muon_NonIsol_Staco_et;
   Float_t         MET_Muon_NonIsol_Staco_sumet;
   Float_t         MET_Muon_Total_Staco_phi;
   Float_t         MET_Muon_Total_Staco_et;
   Float_t         MET_Muon_Total_Staco_sumet;
   Float_t         MET_Muon_Isol_Muid_phi;
   Float_t         MET_Muon_Isol_Muid_et;
   Float_t         MET_Muon_Isol_Muid_sumet;
   Float_t         MET_Muon_NonIsol_Muid_phi;
   Float_t         MET_Muon_NonIsol_Muid_et;
   Float_t         MET_Muon_NonIsol_Muid_sumet;
   Float_t         MET_Muon_Total_Muid_phi;
   Float_t         MET_Muon_Total_Muid_et;
   Float_t         MET_Muon_Total_Muid_sumet;
   Float_t         MET_MuonBoy_phi;
   Float_t         MET_MuonBoy_et;
   Float_t         MET_MuonBoy_sumet;
   Float_t         MET_RefMuon_Track_phi;
   Float_t         MET_RefMuon_Track_et;
   Float_t         MET_RefMuon_Track_sumet;
   Float_t         MET_RefMuon_Track_Staco_phi;
   Float_t         MET_RefMuon_Track_Staco_et;
   Float_t         MET_RefMuon_Track_Staco_sumet;
   Float_t         MET_RefMuon_Track_Muid_phi;
   Float_t         MET_RefMuon_Track_Muid_et;
   Float_t         MET_RefMuon_Track_Muid_sumet;
   Float_t         MET_CryoCone_phi;
   Float_t         MET_CryoCone_et;
   Float_t         MET_CryoCone_sumet;
   Float_t         MET_Final_phi;
   Float_t         MET_Final_et;
   Float_t         MET_Final_sumet;
   Float_t         MET_MuonBoy_Spectro_phi;
   Float_t         MET_MuonBoy_Spectro_et;
   Float_t         MET_MuonBoy_Spectro_sumet;
   Float_t         MET_MuonBoy_Track_phi;
   Float_t         MET_MuonBoy_Track_et;
   Float_t         MET_MuonBoy_Track_sumet;
   Float_t         MET_MuonMuid_phi;
   Float_t         MET_MuonMuid_et;
   Float_t         MET_MuonMuid_sumet;
   Float_t         MET_Muid_phi;
   Float_t         MET_Muid_et;
   Float_t         MET_Muid_sumet;
   Float_t         MET_Muid_Spectro_phi;
   Float_t         MET_Muid_Spectro_et;
   Float_t         MET_Muid_Spectro_sumet;
   Float_t         MET_Muid_Track_phi;
   Float_t         MET_Muid_Track_et;
   Float_t         MET_Muid_Track_sumet;
   Float_t         MET_Muon_phi;
   Float_t         MET_Muon_et;
   Float_t         MET_Muon_sumet;
   Float_t         MET_TopoObj_phi;
   Float_t         MET_TopoObj_et;
   Float_t         MET_TopoObj_sumet;
   Float_t         MET_LocHadTopoObj_phi;
   Float_t         MET_LocHadTopoObj_et;
   Float_t         MET_LocHadTopoObj_sumet;
   Float_t         MET_Topo_phi;
   Float_t         MET_Topo_et;
   Float_t         MET_Topo_sumet;
   Float_t         MET_Topo_SUMET_EMFrac;
   Float_t         MET_Topo_etx_PEMB;
   Float_t         MET_Topo_ety_PEMB;
   Float_t         MET_Topo_sumet_PEMB;
   Float_t         MET_Topo_phi_PEMB;
   Float_t         MET_Topo_etx_EMB;
   Float_t         MET_Topo_ety_EMB;
   Float_t         MET_Topo_sumet_EMB;
   Float_t         MET_Topo_phi_EMB;
   Float_t         MET_Topo_etx_PEMEC;
   Float_t         MET_Topo_ety_PEMEC;
   Float_t         MET_Topo_sumet_PEMEC;
   Float_t         MET_Topo_phi_PEMEC;
   Float_t         MET_Topo_etx_EME;
   Float_t         MET_Topo_ety_EME;
   Float_t         MET_Topo_sumet_EME;
   Float_t         MET_Topo_phi_EME;
   Float_t         MET_Topo_etx_TILE;
   Float_t         MET_Topo_ety_TILE;
   Float_t         MET_Topo_sumet_TILE;
   Float_t         MET_Topo_phi_TILE;
   Float_t         MET_Topo_etx_HEC;
   Float_t         MET_Topo_ety_HEC;
   Float_t         MET_Topo_sumet_HEC;
   Float_t         MET_Topo_phi_HEC;
   Float_t         MET_Topo_etx_FCAL;
   Float_t         MET_Topo_ety_FCAL;
   Float_t         MET_Topo_sumet_FCAL;
   Float_t         MET_Topo_phi_FCAL;
   Float_t         MET_Topo_nCell_PEMB;
   Float_t         MET_Topo_nCell_EMB;
   Float_t         MET_Topo_nCell_PEMEC;
   Float_t         MET_Topo_nCell_EME;
   Float_t         MET_Topo_nCell_TILE;
   Float_t         MET_Topo_nCell_HEC;
   Float_t         MET_Topo_nCell_FCAL;
   Float_t         MET_Topo_etx_CentralReg;
   Float_t         MET_Topo_ety_CentralReg;
   Float_t         MET_Topo_sumet_CentralReg;
   Float_t         MET_Topo_phi_CentralReg;
   Float_t         MET_Topo_etx_EndcapRegion;
   Float_t         MET_Topo_ety_EndcapRegion;
   Float_t         MET_Topo_sumet_EndcapRegion;
   Float_t         MET_Topo_phi_EndcapRegion;
   Float_t         MET_Topo_etx_ForwardReg;
   Float_t         MET_Topo_ety_ForwardReg;
   Float_t         MET_Topo_sumet_ForwardReg;
   Float_t         MET_Topo_phi_ForwardReg;
   Float_t         MET_CorrTopo_phi;
   Float_t         MET_CorrTopo_et;
   Float_t         MET_CorrTopo_sumet;
   Float_t         MET_CorrTopo_SUMET_EMFrac;
   Float_t         MET_CorrTopo_etx_PEMB;
   Float_t         MET_CorrTopo_ety_PEMB;
   Float_t         MET_CorrTopo_sumet_PEMB;
   Float_t         MET_CorrTopo_phi_PEMB;
   Float_t         MET_CorrTopo_etx_EMB;
   Float_t         MET_CorrTopo_ety_EMB;
   Float_t         MET_CorrTopo_sumet_EMB;
   Float_t         MET_CorrTopo_phi_EMB;
   Float_t         MET_CorrTopo_etx_PEMEC;
   Float_t         MET_CorrTopo_ety_PEMEC;
   Float_t         MET_CorrTopo_sumet_PEMEC;
   Float_t         MET_CorrTopo_phi_PEMEC;
   Float_t         MET_CorrTopo_etx_EME;
   Float_t         MET_CorrTopo_ety_EME;
   Float_t         MET_CorrTopo_sumet_EME;
   Float_t         MET_CorrTopo_phi_EME;
   Float_t         MET_CorrTopo_etx_TILE;
   Float_t         MET_CorrTopo_ety_TILE;
   Float_t         MET_CorrTopo_sumet_TILE;
   Float_t         MET_CorrTopo_phi_TILE;
   Float_t         MET_CorrTopo_etx_HEC;
   Float_t         MET_CorrTopo_ety_HEC;
   Float_t         MET_CorrTopo_sumet_HEC;
   Float_t         MET_CorrTopo_phi_HEC;
   Float_t         MET_CorrTopo_etx_FCAL;
   Float_t         MET_CorrTopo_ety_FCAL;
   Float_t         MET_CorrTopo_sumet_FCAL;
   Float_t         MET_CorrTopo_phi_FCAL;
   Float_t         MET_CorrTopo_nCell_PEMB;
   Float_t         MET_CorrTopo_nCell_EMB;
   Float_t         MET_CorrTopo_nCell_PEMEC;
   Float_t         MET_CorrTopo_nCell_EME;
   Float_t         MET_CorrTopo_nCell_TILE;
   Float_t         MET_CorrTopo_nCell_HEC;
   Float_t         MET_CorrTopo_nCell_FCAL;
   Float_t         MET_CorrTopo_etx_CentralReg;
   Float_t         MET_CorrTopo_ety_CentralReg;
   Float_t         MET_CorrTopo_sumet_CentralReg;
   Float_t         MET_CorrTopo_phi_CentralReg;
   Float_t         MET_CorrTopo_etx_EndcapRegion;
   Float_t         MET_CorrTopo_ety_EndcapRegion;
   Float_t         MET_CorrTopo_sumet_EndcapRegion;
   Float_t         MET_CorrTopo_phi_EndcapRegion;
   Float_t         MET_CorrTopo_etx_ForwardReg;
   Float_t         MET_CorrTopo_ety_ForwardReg;
   Float_t         MET_CorrTopo_sumet_ForwardReg;
   Float_t         MET_CorrTopo_phi_ForwardReg;
   Float_t         MET_LocHadTopo_phi;
   Float_t         MET_LocHadTopo_et;
   Float_t         MET_LocHadTopo_sumet;
   Float_t         MET_LocHadTopo_SUMET_EMFrac;
   Float_t         MET_LocHadTopo_etx_PEMB;
   Float_t         MET_LocHadTopo_ety_PEMB;
   Float_t         MET_LocHadTopo_sumet_PEMB;
   Float_t         MET_LocHadTopo_phi_PEMB;
   Float_t         MET_LocHadTopo_etx_EMB;
   Float_t         MET_LocHadTopo_ety_EMB;
   Float_t         MET_LocHadTopo_sumet_EMB;
   Float_t         MET_LocHadTopo_phi_EMB;
   Float_t         MET_LocHadTopo_etx_PEMEC;
   Float_t         MET_LocHadTopo_ety_PEMEC;
   Float_t         MET_LocHadTopo_sumet_PEMEC;
   Float_t         MET_LocHadTopo_phi_PEMEC;
   Float_t         MET_LocHadTopo_etx_EME;
   Float_t         MET_LocHadTopo_ety_EME;
   Float_t         MET_LocHadTopo_sumet_EME;
   Float_t         MET_LocHadTopo_phi_EME;
   Float_t         MET_LocHadTopo_etx_TILE;
   Float_t         MET_LocHadTopo_ety_TILE;
   Float_t         MET_LocHadTopo_sumet_TILE;
   Float_t         MET_LocHadTopo_phi_TILE;
   Float_t         MET_LocHadTopo_etx_HEC;
   Float_t         MET_LocHadTopo_ety_HEC;
   Float_t         MET_LocHadTopo_sumet_HEC;
   Float_t         MET_LocHadTopo_phi_HEC;
   Float_t         MET_LocHadTopo_etx_FCAL;
   Float_t         MET_LocHadTopo_ety_FCAL;
   Float_t         MET_LocHadTopo_sumet_FCAL;
   Float_t         MET_LocHadTopo_phi_FCAL;
   Float_t         MET_LocHadTopo_nCell_PEMB;
   Float_t         MET_LocHadTopo_nCell_EMB;
   Float_t         MET_LocHadTopo_nCell_PEMEC;
   Float_t         MET_LocHadTopo_nCell_EME;
   Float_t         MET_LocHadTopo_nCell_TILE;
   Float_t         MET_LocHadTopo_nCell_HEC;
   Float_t         MET_LocHadTopo_nCell_FCAL;
   Float_t         MET_LocHadTopo_etx_CentralReg;
   Float_t         MET_LocHadTopo_ety_CentralReg;
   Float_t         MET_LocHadTopo_sumet_CentralReg;
   Float_t         MET_LocHadTopo_phi_CentralReg;
   Float_t         MET_LocHadTopo_etx_EndcapRegion;
   Float_t         MET_LocHadTopo_ety_EndcapRegion;
   Float_t         MET_LocHadTopo_sumet_EndcapRegion;
   Float_t         MET_LocHadTopo_phi_EndcapRegion;
   Float_t         MET_LocHadTopo_etx_ForwardReg;
   Float_t         MET_LocHadTopo_ety_ForwardReg;
   Float_t         MET_LocHadTopo_sumet_ForwardReg;
   Float_t         MET_LocHadTopo_phi_ForwardReg;
   Float_t         MET_Calib_phi;
   Float_t         MET_Calib_et;
   Float_t         MET_Calib_sumet;
   Float_t         MET_Calib_SUMET_EMFrac;
   Float_t         MET_Calib_etx_PEMB;
   Float_t         MET_Calib_ety_PEMB;
   Float_t         MET_Calib_sumet_PEMB;
   Float_t         MET_Calib_phi_PEMB;
   Float_t         MET_Calib_etx_EMB;
   Float_t         MET_Calib_ety_EMB;
   Float_t         MET_Calib_sumet_EMB;
   Float_t         MET_Calib_phi_EMB;
   Float_t         MET_Calib_etx_PEMEC;
   Float_t         MET_Calib_ety_PEMEC;
   Float_t         MET_Calib_sumet_PEMEC;
   Float_t         MET_Calib_phi_PEMEC;
   Float_t         MET_Calib_etx_EME;
   Float_t         MET_Calib_ety_EME;
   Float_t         MET_Calib_sumet_EME;
   Float_t         MET_Calib_phi_EME;
   Float_t         MET_Calib_etx_TILE;
   Float_t         MET_Calib_ety_TILE;
   Float_t         MET_Calib_sumet_TILE;
   Float_t         MET_Calib_phi_TILE;
   Float_t         MET_Calib_etx_HEC;
   Float_t         MET_Calib_ety_HEC;
   Float_t         MET_Calib_sumet_HEC;
   Float_t         MET_Calib_phi_HEC;
   Float_t         MET_Calib_etx_FCAL;
   Float_t         MET_Calib_ety_FCAL;
   Float_t         MET_Calib_sumet_FCAL;
   Float_t         MET_Calib_phi_FCAL;
   Float_t         MET_Calib_nCell_PEMB;
   Float_t         MET_Calib_nCell_EMB;
   Float_t         MET_Calib_nCell_PEMEC;
   Float_t         MET_Calib_nCell_EME;
   Float_t         MET_Calib_nCell_TILE;
   Float_t         MET_Calib_nCell_HEC;
   Float_t         MET_Calib_nCell_FCAL;
   Float_t         MET_Calib_etx_CentralReg;
   Float_t         MET_Calib_ety_CentralReg;
   Float_t         MET_Calib_sumet_CentralReg;
   Float_t         MET_Calib_phi_CentralReg;
   Float_t         MET_Calib_etx_EndcapRegion;
   Float_t         MET_Calib_ety_EndcapRegion;
   Float_t         MET_Calib_sumet_EndcapRegion;
   Float_t         MET_Calib_phi_EndcapRegion;
   Float_t         MET_Calib_etx_ForwardReg;
   Float_t         MET_Calib_ety_ForwardReg;
   Float_t         MET_Calib_sumet_ForwardReg;
   Float_t         MET_Calib_phi_ForwardReg;
   Float_t         MET_Truth_NonInt_etx;
   Float_t         MET_Truth_NonInt_ety;
   Float_t         MET_Truth_NonInt_phi;
   Float_t         MET_Truth_NonInt_et;
   Float_t         MET_Truth_NonInt_sumet;
   Float_t         MET_Truth_Int_phi;
   Float_t         MET_Truth_IntCentral_phi;
   Float_t         MET_Truth_IntFwd_phi;
   Float_t         MET_Truth_IntOutCover_phi;
   Float_t         MET_Truth_IntMuons_phi;
   Float_t         MET_Truth_Int_et;
   Float_t         MET_Truth_IntCentral_et;
   Float_t         MET_Truth_IntFwd_et;
   Float_t         MET_Truth_IntOutCover_et;
   Float_t         MET_Truth_IntMuons_et;
   Float_t         MET_Truth_Int_sumet;
   Float_t         MET_Truth_IntCentral_sumet;
   Float_t         MET_Truth_IntFwd_sumet;
   Float_t         MET_Truth_IntOutCover_sumet;
   Float_t         MET_Truth_IntMuons_sumet;
   Float_t         MET_Truth_PileUp_NonInt_etx;
   Float_t         MET_Truth_PileUp_NonInt_ety;
   Float_t         MET_Truth_PileUp_NonInt_phi;
   Float_t         MET_Truth_PileUp_NonInt_et;
   Float_t         MET_Truth_PileUp_NonInt_sumet;
   Float_t         MET_Truth_PileUp_Int_etx;
   Float_t         MET_Truth_PileUp_Int_ety;
   Float_t         MET_Truth_PileUp_IntCentral_etx;
   Float_t         MET_Truth_PileUp_IntCentral_ety;
   Float_t         MET_Truth_PileUp_IntFwd_etx;
   Float_t         MET_Truth_PileUp_IntFwd_ety;
   Float_t         MET_Truth_PileUp_IntOutCover_etx;
   Float_t         MET_Truth_PileUp_IntOutCover_ety;
   Float_t         MET_Truth_PileUp_IntMuons_etx;
   Float_t         MET_Truth_PileUp_IntMuons_ety;
   Float_t         MET_Truth_PileUp_Int_phi;
   Float_t         MET_Truth_PileUp_IntCentral_phi;
   Float_t         MET_Truth_PileUp_IntFwd_phi;
   Float_t         MET_Truth_PileUp_IntOutCover_phi;
   Float_t         MET_Truth_PileUp_IntMuons_phi;
   Float_t         MET_Truth_PileUp_Int_et;
   Float_t         MET_Truth_PileUp_IntCentral_et;
   Float_t         MET_Truth_PileUp_IntFwd_et;
   Float_t         MET_Truth_PileUp_IntOutCover_et;
   Float_t         MET_Truth_PileUp_IntMuons_et;
   Float_t         MET_Truth_PileUp_Int_sumet;
   Float_t         MET_Truth_PileUp_IntCentral_sumet;
   Float_t         MET_Truth_PileUp_IntFwd_sumet;
   Float_t         MET_Truth_PileUp_IntOutCover_sumet;
   Float_t         MET_Truth_PileUp_IntMuons_sumet;
   Float_t         MET_DM_Crack1_etx;
   Float_t         MET_DM_Crack1_ety;
   Float_t         MET_DM_Crack1_phi;
   Float_t         MET_DM_Crack1_et;
   Float_t         MET_DM_Crack1_sumet;
   Float_t         MET_DM_Crack2_etx;
   Float_t         MET_DM_Crack2_ety;
   Float_t         MET_DM_Crack2_phi;
   Float_t         MET_DM_Crack2_et;
   Float_t         MET_DM_Crack2_sumet;
   Float_t         MET_DM_All_etx;
   Float_t         MET_DM_All_ety;
   Float_t         MET_DM_All_phi;
   Float_t         MET_DM_All_et;
   Float_t         MET_DM_All_sumet;
   Float_t         MET_DM_Cryo_etx;
   Float_t         MET_DM_Cryo_ety;
   Float_t         MET_DM_Cryo_phi;
   Float_t         MET_DM_Cryo_et;
   Float_t         MET_DM_Cryo_sumet;
   Float_t         METJetsInfo_JetPtWeightedEventEMFraction;
   Float_t         METJetsInfo_JetPtWeightedNumAssociatedTracks;
   Float_t         METJetsInfo_JetPtWeightedSize;
   Float_t         METJetsInfo_LeadingJetEt;
   Float_t         METJetsInfo_LeadingJetEta;
   Int_t           cl_n;
   std::vector<float>   *cl_pt;
   std::vector<float>   *cl_eta;
   std::vector<float>   *cl_phi;
   Int_t           clpt10_n;
   std::vector<float>   *clpt10_pt;
   std::vector<float>   *clpt10_eta;
   std::vector<float>   *clpt10_phi;
   std::vector<float>   *clpt10_E_em;
   std::vector<float>   *clpt10_E_had;
   std::vector<float>   *clpt10_firstEdens;
   std::vector<float>   *clpt10_cellmaxfrac;
   std::vector<float>   *clpt10_longitudinal;
   std::vector<float>   *clpt10_secondlambda;
   std::vector<float>   *clpt10_lateral;
   std::vector<float>   *clpt10_secondR;
   std::vector<float>   *clpt10_centerlambda;
   std::vector<float>   *clpt10_deltaTheta;
   std::vector<float>   *clpt10_deltaPhi;
   std::vector<float>   *clpt10_time;
   std::vector<float>   *clpt10_E_PreSamplerB;
   std::vector<float>   *clpt10_E_EMB1;
   std::vector<float>   *clpt10_E_EMB2;
   std::vector<float>   *clpt10_E_EMB3;
   std::vector<float>   *clpt10_E_PreSamplerE;
   std::vector<float>   *clpt10_E_EME1;
   std::vector<float>   *clpt10_E_EME2;
   std::vector<float>   *clpt10_E_EME3;
   std::vector<float>   *clpt10_E_HEC0;
   std::vector<float>   *clpt10_E_HEC1;
   std::vector<float>   *clpt10_E_HEC2;
   std::vector<float>   *clpt10_E_HEC3;
   std::vector<float>   *clpt10_E_TileBar0;
   std::vector<float>   *clpt10_E_TileBar1;
   std::vector<float>   *clpt10_E_TileBar2;
   std::vector<float>   *clpt10_E_TileGap1;
   std::vector<float>   *clpt10_E_TileGap2;
   std::vector<float>   *clpt10_E_TileGap3;
   std::vector<float>   *clpt10_E_TileExt0;
   std::vector<float>   *clpt10_E_TileExt1;
   std::vector<float>   *clpt10_E_TileExt2;
   std::vector<float>   *clpt10_E_FCAL0;
   std::vector<float>   *clpt10_E_FCAL1;
   std::vector<float>   *clpt10_E_FCAL2;
   Int_t           emclpt10_n;
   std::vector<float>   *emclpt10_pt;
   std::vector<float>   *emclpt10_eta;
   std::vector<float>   *emclpt10_phi;
   std::vector<float>   *emclpt10_E_em;
   std::vector<float>   *emclpt10_E_had;
   std::vector<float>   *emclpt10_firstEdens;
   std::vector<float>   *emclpt10_cellmaxfrac;
   std::vector<float>   *emclpt10_longitudinal;
   std::vector<float>   *emclpt10_secondlambda;
   std::vector<float>   *emclpt10_lateral;
   std::vector<float>   *emclpt10_secondR;
   std::vector<float>   *emclpt10_centerlambda;
   std::vector<float>   *emclpt10_deltaTheta;
   std::vector<float>   *emclpt10_deltaPhi;
   std::vector<float>   *emclpt10_time;
   std::vector<float>   *emclpt10_E_PreSamplerB;
   std::vector<float>   *emclpt10_E_EMB1;
   std::vector<float>   *emclpt10_E_EMB2;
   std::vector<float>   *emclpt10_E_EMB3;
   std::vector<float>   *emclpt10_E_PreSamplerE;
   std::vector<float>   *emclpt10_E_EME1;
   std::vector<float>   *emclpt10_E_EME2;
   std::vector<float>   *emclpt10_E_EME3;
   std::vector<float>   *emclpt10_E_HEC0;
   std::vector<float>   *emclpt10_E_HEC1;
   std::vector<float>   *emclpt10_E_HEC2;
   std::vector<float>   *emclpt10_E_HEC3;
   std::vector<float>   *emclpt10_E_TileBar0;
   std::vector<float>   *emclpt10_E_TileBar1;
   std::vector<float>   *emclpt10_E_TileBar2;
   std::vector<float>   *emclpt10_E_TileGap1;
   std::vector<float>   *emclpt10_E_TileGap2;
   std::vector<float>   *emclpt10_E_TileGap3;
   std::vector<float>   *emclpt10_E_TileExt0;
   std::vector<float>   *emclpt10_E_TileExt1;
   std::vector<float>   *emclpt10_E_TileExt2;
   std::vector<float>   *emclpt10_E_FCAL0;
   std::vector<float>   *emclpt10_E_FCAL1;
   std::vector<float>   *emclpt10_E_FCAL2;
   std::vector<float>   *emclpt10_eta_PreSamplerB;
   std::vector<float>   *emclpt10_phi_PreSamplerB;
   std::vector<float>   *emclpt10_eta_EMB1;
   std::vector<float>   *emclpt10_phi_EMB1;
   std::vector<float>   *emclpt10_eta_EMB2;
   std::vector<float>   *emclpt10_phi_EMB2;
   std::vector<float>   *emclpt10_eta_EMB3;
   std::vector<float>   *emclpt10_phi_EMB3;
   std::vector<float>   *emclpt10_eta_PreSamplerE;
   std::vector<float>   *emclpt10_phi_PreSamplerE;
   std::vector<float>   *emclpt10_eta_EME1;
   std::vector<float>   *emclpt10_phi_EME1;
   std::vector<float>   *emclpt10_eta_EME2;
   std::vector<float>   *emclpt10_phi_EME2;
   std::vector<float>   *emclpt10_eta_EME3;
   std::vector<float>   *emclpt10_phi_EME3;
   std::vector<float>   *emclpt10_eta_HEC0;
   std::vector<float>   *emclpt10_phi_HEC0;
   std::vector<float>   *emclpt10_eta_HEC1;
   std::vector<float>   *emclpt10_phi_HEC1;
   std::vector<float>   *emclpt10_eta_HEC2;
   std::vector<float>   *emclpt10_phi_HEC2;
   std::vector<float>   *emclpt10_eta_HEC3;
   std::vector<float>   *emclpt10_phi_HEC3;
   std::vector<float>   *emclpt10_eta_TileBar0;
   std::vector<float>   *emclpt10_phi_TileBar0;
   std::vector<float>   *emclpt10_eta_TileBar1;
   std::vector<float>   *emclpt10_phi_TileBar1;
   std::vector<float>   *emclpt10_eta_TileBar2;
   std::vector<float>   *emclpt10_phi_TileBar2;
   std::vector<float>   *emclpt10_eta_TileGap1;
   std::vector<float>   *emclpt10_phi_TileGap1;
   std::vector<float>   *emclpt10_eta_TileGap2;
   std::vector<float>   *emclpt10_phi_TileGap2;
   std::vector<float>   *emclpt10_eta_TileGap3;
   std::vector<float>   *emclpt10_phi_TileGap3;
   std::vector<float>   *emclpt10_eta_TileExt0;
   std::vector<float>   *emclpt10_phi_TileExt0;
   std::vector<float>   *emclpt10_eta_TileExt1;
   std::vector<float>   *emclpt10_phi_TileExt1;
   std::vector<float>   *emclpt10_eta_TileExt2;
   std::vector<float>   *emclpt10_phi_TileExt2;
   std::vector<float>   *emclpt10_eta_FCAL0;
   std::vector<float>   *emclpt10_phi_FCAL0;
   std::vector<float>   *emclpt10_eta_FCAL1;
   std::vector<float>   *emclpt10_phi_FCAL1;
   std::vector<float>   *emclpt10_eta_FCAL2;
   std::vector<float>   *emclpt10_phi_FCAL2;
   Int_t           trk_n;
   std::vector<float>   *trk_pt;
   std::vector<float>   *trk_eta;
   std::vector<float>   *trk_d0_wrtPV;
   std::vector<float>   *trk_z0_wrtPV;
   std::vector<float>   *trk_phi_wrtPV;
   std::vector<float>   *trk_theta_wrtPV;
   std::vector<float>   *trk_qoverp_wrtPV;
   std::vector<float>   *trk_chi2;
   std::vector<int>     *trk_ndof;
   Int_t           trkpt5_n;
   std::vector<float>   *trkpt5_d0;
   std::vector<float>   *trkpt5_z0;
   std::vector<float>   *trkpt5_phi;
   std::vector<float>   *trkpt5_theta;
   std::vector<float>   *trkpt5_qoverp;
   std::vector<float>   *trkpt5_pt;
   std::vector<float>   *trkpt5_eta;
   std::vector<float>   *trkpt5_err_d0;
   std::vector<float>   *trkpt5_err_z0;
   std::vector<float>   *trkpt5_err_phi;
   std::vector<float>   *trkpt5_err_theta;
   std::vector<float>   *trkpt5_err_qoverp;
   std::vector<float>   *trkpt5_d0_wrtPV;
   std::vector<float>   *trkpt5_z0_wrtPV;
   std::vector<float>   *trkpt5_phi_wrtPV;
   std::vector<float>   *trkpt5_err_d0_wrtPV;
   std::vector<float>   *trkpt5_err_z0_wrtPV;
   std::vector<float>   *trkpt5_err_phi_wrtPV;
   std::vector<float>   *trkpt5_err_theta_wrtPV;
   std::vector<float>   *trkpt5_err_qoverp_wrtPV;
   std::vector<float>   *trkpt5_cov_d0_z0_wrtPV;
   std::vector<float>   *trkpt5_cov_d0_phi_wrtPV;
   std::vector<float>   *trkpt5_cov_d0_theta_wrtPV;
   std::vector<float>   *trkpt5_cov_d0_qoverp_wrtPV;
   std::vector<float>   *trkpt5_cov_z0_phi_wrtPV;
   std::vector<float>   *trkpt5_cov_z0_theta_wrtPV;
   std::vector<float>   *trkpt5_cov_z0_qoverp_wrtPV;
   std::vector<float>   *trkpt5_cov_phi_theta_wrtPV;
   std::vector<float>   *trkpt5_cov_phi_qoverp_wrtPV;
   std::vector<float>   *trkpt5_cov_theta_qoverp_wrtPV;
   std::vector<float>   *trkpt5_chi2;
   std::vector<int>     *trkpt5_ndof;
   std::vector<int>     *trkpt5_nBLHits;
   std::vector<int>     *trkpt5_nPixHits;
   std::vector<int>     *trkpt5_nSCTHits;
   std::vector<int>     *trkpt5_nTRTHits;
   std::vector<int>     *trkpt5_nPixHoles;
   std::vector<int>     *trkpt5_nSCTHoles;
   std::vector<int>     *trkpt5_nMDTHits;
   std::vector<int>     *trkpt5_nCSCEtaHits;
   std::vector<int>     *trkpt5_nCSCPhiHits;
   std::vector<int>     *trkpt5_nRPCEtaHits;
   std::vector<int>     *trkpt5_nRPCPhiHits;
   std::vector<int>     *trkpt5_nTGCEtaHits;
   std::vector<int>     *trkpt5_nTGCPhiHits;
   std::vector<int>     *trkpt5_nHits;
   std::vector<int>     *trkpt5_nHoles;
   std::vector<int>     *trkpt5_hitPattern;
   std::vector<float>   *trkpt5_TRTHighTHitsRatio;
   std::vector<float>   *trkpt5_pixeldEdx;
   std::vector<float>   *trkpt5_mc_probability;
   std::vector<int>     *trkpt5_mc_barcode;
   Int_t           vxp_n;
   std::vector<float>   *vxp_x;
   std::vector<float>   *vxp_y;
   std::vector<float>   *vxp_z;
   std::vector<float>   *vxp_cov_x;
   std::vector<float>   *vxp_cov_y;
   std::vector<float>   *vxp_cov_z;
   std::vector<float>   *vxp_chi2;
   std::vector<int>     *vxp_ndof;
   std::vector<float>   *vxp_px;
   std::vector<float>   *vxp_py;
   std::vector<float>   *vxp_pz;
   std::vector<float>   *vxp_E;
   std::vector<float>   *vxp_m;
   std::vector<int>     *vxp_nTracks;
   std::vector<float>   *vxp_sumPt;
   std::vector<int>     *vxp_type;
   std::vector<int>     *vxp_trk_n;
   std::vector<std::vector<int> > *vxp_trk_index;
   Int_t           mb_n;
   std::vector<float>   *mb_E;
   std::vector<float>   *mb_eta;
   std::vector<float>   *mb_phi;
   std::vector<float>   *mb_time;
   std::vector<int>     *mb_quality;
   std::vector<int>     *mb_type;
   std::vector<int>     *mb_module;
   std::vector<int>     *mb_channel;
   Float_t         mbtime_timeDiff;
   Float_t         mbtime_timeA;
   Float_t         mbtime_timeC;
   Int_t           mbtime_countA;
   Int_t           mbtime_countC;
   Bool_t          L1_MBTS_1;
   Bool_t          L1_MBTS_1_1;
   Bool_t          L1_MBTS_1_1_EMPTY;
   Bool_t          L1_MBTS_1_1_UNPAIRED_ISO;
   Bool_t          L1_MBTS_1_EMPTY;
   Bool_t          L1_MBTS_1_UNPAIRED_ISO;
   Bool_t          L1_MBTS_2;
   Bool_t          L1_MBTS_2_EMPTY;
   Bool_t          L1_MBTS_2_UNPAIRED_ISO;
   Bool_t          L1_MBTS_2_UNPAIRED_NONISO;
   Bool_t          L1_MBTS_4_4;
   Bool_t          L1_MBTS_4_4_UNPAIRED_ISO;
   Bool_t          collcand_passCaloTime;
   Bool_t          collcand_passMBTSTime;
   Bool_t          collcand_passTrigger;
   Bool_t          collcand_pass;
   Int_t           muonTruth_n;
   std::vector<float>   *muonTruth_pt;
   std::vector<float>   *muonTruth_m;
   std::vector<float>   *muonTruth_eta;
   std::vector<float>   *muonTruth_phi;
   std::vector<float>   *muonTruth_charge;
   std::vector<int>     *muonTruth_PDGID;
   std::vector<int>     *muonTruth_barcode;
   std::vector<int>     *muonTruth_type;
   std::vector<int>     *muonTruth_origin;
   Int_t           mcevt_n;
   std::vector<int>     *mcevt_signal_process_id;
   std::vector<int>     *mcevt_event_number;
   std::vector<double>  *mcevt_event_scale;
   std::vector<double>  *mcevt_alphaQCD;
   std::vector<double>  *mcevt_alphaQED;
   std::vector<int>     *mcevt_pdf_id1;
   std::vector<int>     *mcevt_pdf_id2;
   std::vector<double>  *mcevt_pdf_x1;
   std::vector<double>  *mcevt_pdf_x2;
   std::vector<double>  *mcevt_pdf_scale;
   std::vector<double>  *mcevt_pdf1;
   std::vector<double>  *mcevt_pdf2;
   std::vector<std::vector<double> >  *mcevt_weight;
   Int_t           mc_n;
   std::vector<float>   *mc_pt;
   std::vector<float>   *mc_m;
   std::vector<float>   *mc_eta;
   std::vector<float>   *mc_phi;
   std::vector<int>     *mc_status;
   std::vector<int>     *mc_barcode;
   std::vector<std::vector<int> > *mc_parents;
   std::vector<std::vector<int> > *mc_children;
   std::vector<int>     *mc_pdgId;
   std::vector<float>   *mc_charge;
   std::vector<float>   *mc_vx_x;
   std::vector<float>   *mc_vx_y;
   std::vector<float>   *mc_vx_z;
   std::vector<std::vector<int> > *mc_child_index;
   std::vector<std::vector<int> > *mc_parent_index;
   std::vector<int>     *trig_L2_combmuonfeature_L2_mu18_MG;
   Int_t           trig_L2_combmuonfeature_n;
   std::vector<float>   *trig_L2_combmuonfeature_pt;
   std::vector<float>   *trig_L2_combmuonfeature_eta;
   std::vector<float>   *trig_L2_combmuonfeature_phi;
   Int_t           trig_EF_trigmugirl_n;
   std::vector<int>     *trig_EF_trigmugirl_track_n;
   std::vector<std::vector<float> > *trig_EF_trigmugirl_track_CB_pt;
   std::vector<std::vector<float> > *trig_EF_trigmugirl_track_CB_eta;
   std::vector<std::vector<float> > *trig_EF_trigmugirl_track_CB_phi;
   std::vector<int>     *trig_L2_muonfeature_L2_mu40_MSonly_barrel;
   Int_t           trig_L2_muonfeature_n;
   std::vector<float>   *trig_L2_muonfeature_pt;
   std::vector<float>   *trig_L2_muonfeature_eta;
   std::vector<float>   *trig_L2_muonfeature_phi;
   Int_t           trig_EF_trigmuonef_n;
   std::vector<int>     *trig_EF_trigmuonef_track_n;
   std::vector<std::vector<float> > *trig_EF_trigmuonef_track_SA_pt;
   std::vector<std::vector<float> > *trig_EF_trigmuonef_track_SA_eta;
   std::vector<std::vector<float> > *trig_EF_trigmuonef_track_SA_phi;
   Int_t           trig_EF_el_n;
   std::vector<int>     *trig_EF_el_EF_e20_medium;
   std::vector<float>   *trig_EF_el_eta;
   std::vector<float>   *trig_EF_el_phi;

   // List of branches
   TBranch        *b_RunNumber;   //!
   TBranch        *b_EventNumber;   //!
   TBranch        *b_timestamp;   //!
   TBranch        *b_timestamp_ns;   //!
   TBranch        *b_lbn;   //!
   TBranch        *b_bcid;   //!
   TBranch        *b_detmask0;   //!
   TBranch        *b_detmask1;   //!
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
   TBranch        *b_el_type;   //!
   TBranch        *b_el_origin;   //!
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
   TBranch        *b_el_truth_index;   //!
   TBranch        *b_el_truth_matched;   //!
   TBranch        *b_el_loose;   //!
   TBranch        *b_el_medium;   //!
   TBranch        *b_el_mediumIso;   //!
   TBranch        *b_el_tight;   //!
   TBranch        *b_el_tightIso;   //!
   TBranch        *b_el_goodOQ;   //!
   TBranch        *b_el_Ethad;   //!
   TBranch        *b_el_Ethad1;   //!
   TBranch        *b_el_f1;   //!
   TBranch        *b_el_f1core;   //!
   TBranch        *b_el_Emins1;   //!
   TBranch        *b_el_fside;   //!
   TBranch        *b_el_Emax2;   //!
   TBranch        *b_el_ws3;   //!
   TBranch        *b_el_wstot;   //!
   TBranch        *b_el_emaxs1;   //!
   TBranch        *b_el_deltaEs;   //!
   TBranch        *b_el_E233;   //!
   TBranch        *b_el_E237;   //!
   TBranch        *b_el_E277;   //!
   TBranch        *b_el_weta2;   //!
   TBranch        *b_el_f3;   //!
   TBranch        *b_el_f3core;   //!
   TBranch        *b_el_rphiallcalo;   //!
   TBranch        *b_el_Etcone45;   //!
   TBranch        *b_el_Etcone20;   //!
   TBranch        *b_el_Etcone30;   //!
   TBranch        *b_el_Etcone40;   //!
   TBranch        *b_el_ptcone20;   //!
   TBranch        *b_el_ptcone30;   //!
   TBranch        *b_el_ptcone40;   //!
   TBranch        *b_el_nucone20;   //!
   TBranch        *b_el_nucone30;   //!
   TBranch        *b_el_nucone40;   //!
   TBranch        *b_el_convanglematch;   //!
   TBranch        *b_el_convtrackmatch;   //!
   TBranch        *b_el_hasconv;   //!
   TBranch        *b_el_convvtxx;   //!
   TBranch        *b_el_convvtxy;   //!
   TBranch        *b_el_convvtxz;   //!
   TBranch        *b_el_Rconv;   //!
   TBranch        *b_el_zconv;   //!
   TBranch        *b_el_convvtxchi2;   //!
   TBranch        *b_el_pt1conv;   //!
   TBranch        *b_el_convtrk1nBLHits;   //!
   TBranch        *b_el_convtrk1nPixHits;   //!
   TBranch        *b_el_convtrk1nSCTHits;   //!
   TBranch        *b_el_convtrk1nTRTHits;   //!
   TBranch        *b_el_pt2conv;   //!
   TBranch        *b_el_convtrk2nBLHits;   //!
   TBranch        *b_el_convtrk2nPixHits;   //!
   TBranch        *b_el_convtrk2nSCTHits;   //!
   TBranch        *b_el_convtrk2nTRTHits;   //!
   TBranch        *b_el_ptconv;   //!
   TBranch        *b_el_pzconv;   //!
   TBranch        *b_el_pos7;   //!
   TBranch        *b_el_etacorrmag;   //!
   TBranch        *b_el_deltaeta1;   //!
   TBranch        *b_el_deltaeta2;   //!
   TBranch        *b_el_deltaphi2;   //!
   TBranch        *b_el_deltaphiRescaled;   //!
   TBranch        *b_el_deltaPhiRot;   //!
   TBranch        *b_el_expectHitInBLayer;   //!
   TBranch        *b_el_reta;   //!
   TBranch        *b_el_rphi;   //!
   TBranch        *b_el_EtringnoisedR03sig2;   //!
   TBranch        *b_el_EtringnoisedR03sig3;   //!
   TBranch        *b_el_EtringnoisedR03sig4;   //!
   TBranch        *b_el_isolationlikelihoodjets;   //!
   TBranch        *b_el_isolationlikelihoodhqelectrons;   //!
   TBranch        *b_el_electronweight;   //!
   TBranch        *b_el_electronbgweight;   //!
   TBranch        *b_el_softeweight;   //!
   TBranch        *b_el_softebgweight;   //!
   TBranch        *b_el_neuralnet;   //!
   TBranch        *b_el_Hmatrix;   //!
   TBranch        *b_el_Hmatrix5;   //!
   TBranch        *b_el_adaboost;   //!
   TBranch        *b_el_softeneuralnet;   //!
   TBranch        *b_el_zvertex;   //!
   TBranch        *b_el_errz;   //!
   TBranch        *b_el_etap;   //!
   TBranch        *b_el_depth;   //!
   TBranch        *b_el_refittedTrack_n;   //!
   TBranch        *b_el_Es0;   //!
   TBranch        *b_el_etas0;   //!
   TBranch        *b_el_phis0;   //!
   TBranch        *b_el_Es1;   //!
   TBranch        *b_el_etas1;   //!
   TBranch        *b_el_phis1;   //!
   TBranch        *b_el_Es2;   //!
   TBranch        *b_el_etas2;   //!
   TBranch        *b_el_phis2;   //!
   TBranch        *b_el_Es3;   //!
   TBranch        *b_el_etas3;   //!
   TBranch        *b_el_phis3;   //!
   TBranch        *b_el_E_PreSamplerB;   //!
   TBranch        *b_el_E_EMB1;   //!
   TBranch        *b_el_E_EMB2;   //!
   TBranch        *b_el_E_EMB3;   //!
   TBranch        *b_el_E_PreSamplerE;   //!
   TBranch        *b_el_E_EME1;   //!
   TBranch        *b_el_E_EME2;   //!
   TBranch        *b_el_E_EME3;   //!
   TBranch        *b_el_E_HEC0;   //!
   TBranch        *b_el_E_HEC1;   //!
   TBranch        *b_el_E_HEC2;   //!
   TBranch        *b_el_E_HEC3;   //!
   TBranch        *b_el_E_TileBar0;   //!
   TBranch        *b_el_E_TileBar1;   //!
   TBranch        *b_el_E_TileBar2;   //!
   TBranch        *b_el_E_TileGap1;   //!
   TBranch        *b_el_E_TileGap2;   //!
   TBranch        *b_el_E_TileGap3;   //!
   TBranch        *b_el_E_TileExt0;   //!
   TBranch        *b_el_E_TileExt1;   //!
   TBranch        *b_el_E_TileExt2;   //!
   TBranch        *b_el_E_FCAL0;   //!
   TBranch        *b_el_E_FCAL1;   //!
   TBranch        *b_el_E_FCAL2;   //!
   TBranch        *b_el_cl_E;   //!
   TBranch        *b_el_cl_pt;   //!
   TBranch        *b_el_cl_eta;   //!
   TBranch        *b_el_cl_phi;   //!
   TBranch        *b_el_firstEdens;   //!
   TBranch        *b_el_cellmaxfrac;   //!
   TBranch        *b_el_longitudinal;   //!
   TBranch        *b_el_secondlambda;   //!
   TBranch        *b_el_lateral;   //!
   TBranch        *b_el_secondR;   //!
   TBranch        *b_el_centerlambda;   //!
   TBranch        *b_el_rawcl_Es0;   //!
   TBranch        *b_el_rawcl_etas0;   //!
   TBranch        *b_el_rawcl_phis0;   //!
   TBranch        *b_el_rawcl_Es1;   //!
   TBranch        *b_el_rawcl_etas1;   //!
   TBranch        *b_el_rawcl_phis1;   //!
   TBranch        *b_el_rawcl_Es2;   //!
   TBranch        *b_el_rawcl_etas2;   //!
   TBranch        *b_el_rawcl_phis2;   //!
   TBranch        *b_el_rawcl_Es3;   //!
   TBranch        *b_el_rawcl_etas3;   //!
   TBranch        *b_el_rawcl_phis3;   //!
   TBranch        *b_el_rawcl_E;   //!
   TBranch        *b_el_rawcl_pt;   //!
   TBranch        *b_el_rawcl_eta;   //!
   TBranch        *b_el_rawcl_phi;   //!
   TBranch        *b_el_refittedtrackcovd0;   //!
   TBranch        *b_el_refittedtrackcovz0;   //!
   TBranch        *b_el_refittedtrackcovphi;   //!
   TBranch        *b_el_refittedtrackcovtheta;   //!
   TBranch        *b_el_refittedtrackcovqoverp;   //!
   TBranch        *b_el_refittedtrackcovd0z0;   //!
   TBranch        *b_el_refittedtrackcovz0phi;   //!
   TBranch        *b_el_refittedtrackcovz0theta;   //!
   TBranch        *b_el_refittedtrackcovz0qoverp;   //!
   TBranch        *b_el_refittedtrackcovd0phi;   //!
   TBranch        *b_el_refittedtrackcovd0theta;   //!
   TBranch        *b_el_refittedtrackcovd0qoverp;   //!
   TBranch        *b_el_refittedtrackcovphitheta;   //!
   TBranch        *b_el_refittedtrackcovphiqoverp;   //!
   TBranch        *b_el_refittedtrackcovthetaqoverp;   //!
   TBranch        *b_el_trackd0;   //!
   TBranch        *b_el_trackz0;   //!
   TBranch        *b_el_trackphi;   //!
   TBranch        *b_el_tracktheta;   //!
   TBranch        *b_el_trackqoverp;   //!
   TBranch        *b_el_trackpt;   //!
   TBranch        *b_el_tracketa;   //!
   TBranch        *b_el_trackcov_d0;   //!
   TBranch        *b_el_trackcov_z0;   //!
   TBranch        *b_el_trackcov_phi;   //!
   TBranch        *b_el_trackcov_theta;   //!
   TBranch        *b_el_trackcov_qoverp;   //!
   TBranch        *b_el_trackcov_d0_z0;   //!
   TBranch        *b_el_trackcov_d0_phi;   //!
   TBranch        *b_el_trackcov_d0_theta;   //!
   TBranch        *b_el_trackcov_d0_qoverp;   //!
   TBranch        *b_el_trackcov_z0_phi;   //!
   TBranch        *b_el_trackcov_z0_theta;   //!
   TBranch        *b_el_trackcov_z0_qoverp;   //!
   TBranch        *b_el_trackcov_phi_theta;   //!
   TBranch        *b_el_trackcov_phi_qoverp;   //!
   TBranch        *b_el_trackcov_theta_qoverp;   //!
   TBranch        *b_el_trackfitchi2;   //!
   TBranch        *b_el_trackfitndof;   //!
   TBranch        *b_el_nBLHits;   //!
   TBranch        *b_el_nPixHits;   //!
   TBranch        *b_el_nSCTHits;   //!
   TBranch        *b_el_nTRTHits;   //!
   TBranch        *b_el_nPixHoles;   //!
   TBranch        *b_el_nSCTHoles;   //!
   TBranch        *b_el_nBLSharedHits;   //!
   TBranch        *b_el_nPixSharedHits;   //!
   TBranch        *b_el_nSCTSharedHits;   //!
   TBranch        *b_el_nTRTHighTHits;   //!
   TBranch        *b_el_nTRTOutliers;   //!
   TBranch        *b_el_nTRTHighTOutliers;   //!
   TBranch        *b_el_nSiHits;   //!
   TBranch        *b_el_TRTHighTHitsRatio;   //!
   TBranch        *b_el_pixeldEdx;   //!
   TBranch        *b_el_eProbabilityComb;   //!
   TBranch        *b_el_eProbabilityHT;   //!
   TBranch        *b_el_eProbabilityToT;   //!
   TBranch        *b_el_eProbabilityBrem;   //!
   TBranch        *b_el_vertx;   //!
   TBranch        *b_el_verty;   //!
   TBranch        *b_el_vertz;   //!
   TBranch        *b_el_hastrack;   //!
   TBranch        *b_el_deltaEmax2;   //!
   TBranch        *b_el_calibHitsShowerDepth;   //!
   TBranch        *b_el_trackd0beam;   //!
   TBranch        *b_el_tracksigd0beam;   //!
   TBranch        *b_el_trackd0pv;   //!
   TBranch        *b_el_tracksigd0pv;   //!
   TBranch        *b_el_trackz0pv;   //!
   TBranch        *b_el_tracksigz0pv;   //!
   TBranch        *b_el_trackd0pvunbiased;   //!
   TBranch        *b_el_tracksigd0pvunbiased;   //!
   TBranch        *b_el_trackz0pvunbiased;   //!
   TBranch        *b_el_tracksigz0pvunbiased;   //!
   TBranch        *b_el_isIso;   //!
   TBranch        *b_el_mvaptcone20;   //!
   TBranch        *b_el_mvaptcone30;   //!
   TBranch        *b_el_mvaptcone40;   //!
   TBranch        *b_el_jet_dr;   //!
   TBranch        *b_el_jet_E;   //!
   TBranch        *b_el_jet_pt;   //!
   TBranch        *b_el_jet_m;   //!
   TBranch        *b_el_jet_eta;   //!
   TBranch        *b_el_jet_phi;   //!
   TBranch        *b_el_jet_truth_dr;   //!
   TBranch        *b_el_jet_truth_E;   //!
   TBranch        *b_el_jet_truth_pt;   //!
   TBranch        *b_el_jet_truth_m;   //!
   TBranch        *b_el_jet_truth_eta;   //!
   TBranch        *b_el_jet_truth_phi;   //!
   TBranch        *b_el_jet_truth_matched;   //!
   TBranch        *b_el_jet_matched;   //!
   TBranch        *b_el_Etcone40_pt_corrected;   //!
   TBranch        *b_el_Etcone40_ED_corrected;   //!
   TBranch        *b_el_Etcone40_corrected;   //!
   TBranch        *b_el_EF_dr;   //!
   TBranch        *b_el_EF_index;   //!
   TBranch        *b_el_L2_dr;   //!
   TBranch        *b_el_L2_index;   //!
   TBranch        *b_el_L1_dr;   //!
   TBranch        *b_el_L1_index;   //!
   TBranch        *b_EF_2e10_loose;   //!
   TBranch        *b_EF_2e10_medium;   //!
   TBranch        *b_EF_2e15_loose;   //!
   TBranch        *b_EF_2e3_loose;   //!
   TBranch        *b_EF_2e3_loose_SiTrk;   //!
   TBranch        *b_EF_2e3_loose_TRT;   //!
   TBranch        *b_EF_2e3_medium;   //!
   TBranch        *b_EF_2e3_medium_SiTrk;   //!
   TBranch        *b_EF_2e3_medium_TRT;   //!
   TBranch        *b_EF_2e3_tight;   //!
   TBranch        *b_EF_2e5_medium;   //!
   TBranch        *b_EF_2e5_medium_SiTrk;   //!
   TBranch        *b_EF_2e5_medium_TRT;   //!
   TBranch        *b_EF_2e5_tight;   //!
   TBranch        *b_EF_2g10_loose;   //!
   TBranch        *b_EF_2g15_loose;   //!
   TBranch        *b_EF_2g20_loose;   //!
   TBranch        *b_EF_2g5_loose;   //!
   TBranch        *b_EF_2g7_loose;   //!
   TBranch        *b_EF_2j35_jetNoEF_xe20_noMu;   //!
   TBranch        *b_EF_2j35_jetNoEF_xe30_noMu;   //!
   TBranch        *b_EF_2j35_jetNoEF_xe40_noMu;   //!
   TBranch        *b_EF_2j35_xe20_noMu;   //!
   TBranch        *b_EF_2j35_xe30_noMu;   //!
   TBranch        *b_EF_2j35_xe40_noMu;   //!
   TBranch        *b_EF_2mu0_NoAlg;   //!
   TBranch        *b_EF_2mu10;   //!
   TBranch        *b_EF_2mu10_NoAlg;   //!
   TBranch        *b_EF_2mu4;   //!
   TBranch        *b_EF_2mu4_Bmumu;   //!
   TBranch        *b_EF_2mu4_Bmumux;   //!
   TBranch        *b_EF_2mu4_DiMu;   //!
   TBranch        *b_EF_2mu4_DiMu_SiTrk;   //!
   TBranch        *b_EF_2mu4_DiMu_noVtx;   //!
   TBranch        *b_EF_2mu4_DiMu_noVtx_noOS;   //!
   TBranch        *b_EF_2mu4_Jpsimumu;   //!
   TBranch        *b_EF_2mu4_Jpsimumu_IDTrkNoCut;   //!
   TBranch        *b_EF_2mu4_Upsimumu;   //!
   TBranch        *b_EF_2mu6;   //!
   TBranch        *b_EF_2mu6_Bmumu;   //!
   TBranch        *b_EF_2mu6_Bmumux;   //!
   TBranch        *b_EF_2mu6_DiMu;   //!
   TBranch        *b_EF_2mu6_Jpsimumu;   //!
   TBranch        *b_EF_2mu6_MG;   //!
   TBranch        *b_EF_2mu6_NoAlg;   //!
   TBranch        *b_EF_2mu6_Upsimumu;   //!
   TBranch        *b_EF_2mu6_g10_loose;   //!
   TBranch        *b_EF_2tau12_loose;   //!
   TBranch        *b_EF_2tau16_loose;   //!
   TBranch        *b_EF_2tau20_loose;   //!
   TBranch        *b_EF_2tau29_loose;   //!
   TBranch        *b_EF_2tau29_loose1;   //!
   TBranch        *b_EF_2tau38_loose;   //!
   TBranch        *b_EF_e10_NoCut;   //!
   TBranch        *b_EF_e10_loose;   //!
   TBranch        *b_EF_e10_loose_mu10;   //!
   TBranch        *b_EF_e10_loose_mu6;   //!
   TBranch        *b_EF_e10_medium;   //!
   TBranch        *b_EF_e10_medium_IDTrkNoCut;   //!
   TBranch        *b_EF_e10_medium_SiTrk;   //!
   TBranch        *b_EF_e10_medium_TRT;   //!
   TBranch        *b_EF_e10_tight;   //!
   TBranch        *b_EF_e10_tight_TRT;   //!
   TBranch        *b_EF_e15_loose;   //!
   TBranch        *b_EF_e15_loose_IDTrkNoCut;   //!
   TBranch        *b_EF_e15_medium;   //!
   TBranch        *b_EF_e15_medium_SiTrk;   //!
   TBranch        *b_EF_e15_medium_TRT;   //!
   TBranch        *b_EF_e15_tight;   //!
   TBranch        *b_EF_e15_tight_TRT;   //!
   TBranch        *b_EF_e18_medium;   //!
   TBranch        *b_EF_e20_loose;   //!
   TBranch        *b_EF_e20_loose_IDTrkNoCut;   //!
   TBranch        *b_EF_e20_loose_TRT;   //!
   TBranch        *b_EF_e20_loose_passEF;   //!
   TBranch        *b_EF_e20_loose_passL2;   //!
   TBranch        *b_EF_e20_loose_xe20;   //!
   TBranch        *b_EF_e20_loose_xe20_noMu;   //!
   TBranch        *b_EF_e20_loose_xe30;   //!
   TBranch        *b_EF_e20_loose_xe30_noMu;   //!
   TBranch        *b_EF_e20_medium;   //!
   TBranch        *b_EF_e25_loose;   //!
   TBranch        *b_EF_e25_medium;   //!
   TBranch        *b_EF_e30_loose;   //!
   TBranch        *b_EF_e3_loose;   //!
   TBranch        *b_EF_e3_loose_SiTrk;   //!
   TBranch        *b_EF_e3_loose_TRT;   //!
   TBranch        *b_EF_e3_medium;   //!
   TBranch        *b_EF_e3_medium_SiTrk;   //!
   TBranch        *b_EF_e3_medium_TRT;   //!
   TBranch        *b_EF_e5_NoCut_L2SW;   //!
   TBranch        *b_EF_e5_NoCut_Ringer;   //!
   TBranch        *b_EF_e5_NoCut_firstempty;   //!
   TBranch        *b_EF_e5_loose_mu4;   //!
   TBranch        *b_EF_e5_medium;   //!
   TBranch        *b_EF_e5_medium_MV;   //!
   TBranch        *b_EF_e5_medium_SiTrk;   //!
   TBranch        *b_EF_e5_medium_TRT;   //!
   TBranch        *b_EF_e5_medium_mu4;   //!
   TBranch        *b_EF_e5_tight;   //!
   TBranch        *b_EF_e5_tight_SiTrk;   //!
   TBranch        *b_EF_e5_tight_TRT;   //!
   TBranch        *b_EF_e5_tight_e5_NoCut;   //!
   TBranch        *b_EF_em105_passHLT;   //!
   TBranch        *b_EF_g10_loose;   //!
   TBranch        *b_EF_g10_loose_larcalib;   //!
   TBranch        *b_EF_g11_etcut;   //!
   TBranch        *b_EF_g15_loose;   //!
   TBranch        *b_EF_g17_etcut;   //!
   TBranch        *b_EF_g17_etcut_EFxe20_noMu;   //!
   TBranch        *b_EF_g17_etcut_EFxe30_noMu;   //!
   TBranch        *b_EF_g20_loose;   //!
   TBranch        *b_EF_g20_loose_cnv;   //!
   TBranch        *b_EF_g20_loose_larcalib;   //!
   TBranch        *b_EF_g20_loose_xe20_noMu;   //!
   TBranch        *b_EF_g20_loose_xe30_noMu;   //!
   TBranch        *b_EF_g20_tight;   //!
   TBranch        *b_EF_g25_loose_xe30_noMu;   //!
   TBranch        *b_EF_g30_loose;   //!
   TBranch        *b_EF_g30_tight;   //!
   TBranch        *b_EF_g3_NoCut_unpaired_iso;   //!
   TBranch        *b_EF_g3_NoCut_unpaired_noniso;   //!
   TBranch        *b_EF_g40_loose;   //!
   TBranch        *b_EF_g40_loose_larcalib;   //!
   TBranch        *b_EF_g40_tight;   //!
   TBranch        *b_EF_g50_loose;   //!
   TBranch        *b_EF_g50_loose_larcalib;   //!
   TBranch        *b_EF_g5_NoCut_cosmic;   //!
   TBranch        *b_EF_g5_loose;   //!
   TBranch        *b_EF_g5_loose_cnv;   //!
   TBranch        *b_EF_g5_loose_larcalib;   //!
   TBranch        *b_EF_g7_loose;   //!
   TBranch        *b_EF_j115_jetNoCut_larcalib;   //!
   TBranch        *b_EF_j20;   //!
   TBranch        *b_EF_j20_jetNoEF;   //!
   TBranch        *b_EF_j20_jetNoEF_larcalib;   //!
   TBranch        *b_EF_j20a4;   //!
   TBranch        *b_EF_j20a4fs;   //!
   TBranch        *b_EF_j20a6;   //!
   TBranch        *b_EF_j20a6fs;   //!
   TBranch        *b_EF_j30;   //!
   TBranch        *b_EF_j30_cosmic;   //!
   TBranch        *b_EF_j30_firstempty;   //!
   TBranch        *b_EF_j30_fj30_jetNoEF;   //!
   TBranch        *b_EF_j30_jetNoCut_cosmic;   //!
   TBranch        *b_EF_j30_jetNoCut_firstempty;   //!
   TBranch        *b_EF_j30_jetNoCut_unpaired;   //!
   TBranch        *b_EF_j30_jetNoEF;   //!
   TBranch        *b_EF_j30_jetNoEF_cosmic;   //!
   TBranch        *b_EF_j30_jetNoEF_firstempty;   //!
   TBranch        *b_EF_j30_jetNoEF_unpaired;   //!
   TBranch        *b_EF_j30_unpaired;   //!
   TBranch        *b_EF_j35;   //!
   TBranch        *b_EF_j35_L1TAU_HV;   //!
   TBranch        *b_EF_j35_L1TAU_HV_jetNoEF;   //!
   TBranch        *b_EF_j35_jetNoCut_xe30_e15_medium;   //!
   TBranch        *b_EF_j35_jetNoCut_xe30_mu15;   //!
   TBranch        *b_EF_j35_jetNoEF;   //!
   TBranch        *b_EF_j35_xe30_e15_medium;   //!
   TBranch        *b_EF_j35_xe30_mu15;   //!
   TBranch        *b_EF_j50;   //!
   TBranch        *b_EF_j50_cosmic;   //!
   TBranch        *b_EF_j50_firstempty;   //!
   TBranch        *b_EF_j50_jetNoCut_cosmic;   //!
   TBranch        *b_EF_j50_jetNoCut_firstempty;   //!
   TBranch        *b_EF_j50_jetNoCut_unpaired;   //!
   TBranch        *b_EF_j50_jetNoEF;   //!
   TBranch        *b_EF_j50_jetNoEF_cosmic;   //!
   TBranch        *b_EF_j50_jetNoEF_firstempty;   //!
   TBranch        *b_EF_j50_jetNoEF_larcalib;   //!
   TBranch        *b_EF_j50_jetNoEF_unpaired;   //!
   TBranch        *b_EF_j50_jetNoEF_xe20_noMu;   //!
   TBranch        *b_EF_j50_jetNoEF_xe30_noMu;   //!
   TBranch        *b_EF_j50_jetNoEF_xe40_noMu;   //!
   TBranch        *b_EF_j50_unpaired;   //!
   TBranch        *b_EF_j50_xe20_noMu;   //!
   TBranch        *b_EF_j50_xe30_noMu;   //!
   TBranch        *b_EF_j50_xe40_noMu;   //!
   TBranch        *b_EF_j5_empty_NoAlg;   //!
   TBranch        *b_EF_j5_unpaired_iso_NoAlg;   //!
   TBranch        *b_EF_j5_unpaired_noniso_NoAlg;   //!
   TBranch        *b_EF_j75;   //!
   TBranch        *b_EF_j75_jetNoEF;   //!
   TBranch        *b_EF_j75_jetNoEF_EFxe20_noMu;   //!
   TBranch        *b_EF_j75_jetNoEF_EFxe25_noMu;   //!
   TBranch        *b_EF_j75_jetNoEF_EFxe30_noMu;   //!
   TBranch        *b_EF_j75_jetNoEF_EFxe35_noMu;   //!
   TBranch        *b_EF_j75_jetNoEF_EFxe40_noMu;   //!
   TBranch        *b_EF_j95;   //!
   TBranch        *b_EF_j95_jetNoEF;   //!
   TBranch        *b_EF_j95_jetNoEF_larcalib;   //!
   TBranch        *b_EF_je135_NoEF;   //!
   TBranch        *b_EF_je195_NoEF;   //!
   TBranch        *b_EF_je255_NoEF;   //!
   TBranch        *b_EF_je300_NoEF;   //!
   TBranch        *b_EF_je350_NoEF;   //!
   TBranch        *b_EF_mu0_NoAlg;   //!
   TBranch        *b_EF_mu0_comm_NoAlg;   //!
   TBranch        *b_EF_mu0_comm_empty_NoAlg;   //!
   TBranch        *b_EF_mu0_comm_firstempty_NoAlg;   //!
   TBranch        *b_EF_mu0_comm_unpaired_iso_NoAlg;   //!
   TBranch        *b_EF_mu0_comm_unpaired_noniso_NoAlg;   //!
   TBranch        *b_EF_mu0_empty_NoAlg;   //!
   TBranch        *b_EF_mu0_firstempty_NoAlg;   //!
   TBranch        *b_EF_mu0_missingRoi;   //!
   TBranch        *b_EF_mu0_outOfTime1;   //!
   TBranch        *b_EF_mu0_outOfTime2;   //!
   TBranch        *b_EF_mu0_rpcOnly;   //!
   TBranch        *b_EF_mu0_unpaired_iso_NoAlg;   //!
   TBranch        *b_EF_mu0_unpaired_noniso_NoAlg;   //!
   TBranch        *b_EF_mu10;   //!
   TBranch        *b_EF_mu10_Ecut12;   //!
   TBranch        *b_EF_mu10_Ecut123;   //!
   TBranch        *b_EF_mu10_Ecut13;   //!
   TBranch        *b_EF_mu10_IDTrkNoCut;   //!
   TBranch        *b_EF_mu10_MG;   //!
   TBranch        *b_EF_mu10_MSonly;   //!
   TBranch        *b_EF_mu10_MSonly_Ecut12;   //!
   TBranch        *b_EF_mu10_MSonly_Ecut123;   //!
   TBranch        *b_EF_mu10_MSonly_Ecut13;   //!
   TBranch        *b_EF_mu10_MSonly_tight;   //!
   TBranch        *b_EF_mu10_NoAlg;   //!
   TBranch        *b_EF_mu10_SiTrk;   //!
   TBranch        *b_EF_mu10_j30;   //!
   TBranch        *b_EF_mu10_tight;   //!
   TBranch        *b_EF_mu10i_loose;   //!
   TBranch        *b_EF_mu13;   //!
   TBranch        *b_EF_mu13_MG;   //!
   TBranch        *b_EF_mu13_MG_tight;   //!
   TBranch        *b_EF_mu13_tight;   //!
   TBranch        *b_EF_mu15;   //!
   TBranch        *b_EF_mu15_MG;   //!
   TBranch        *b_EF_mu15_NoAlg;   //!
   TBranch        *b_EF_mu18;   //!
   TBranch        *b_EF_mu18_MG;   //!
   TBranch        *b_EF_mu20;   //!
   TBranch        *b_EF_mu20_MSonly;   //!
   TBranch        *b_EF_mu20_NoAlg;   //!
   TBranch        *b_EF_mu20_passHLT;   //!
   TBranch        *b_EF_mu20_slow;   //!
   TBranch        *b_EF_mu30_MSonly;   //!
   TBranch        *b_EF_mu4;   //!
   TBranch        *b_EF_mu40_MSonly;   //!
   TBranch        *b_EF_mu40_MSonly_barrel;   //!
   TBranch        *b_EF_mu4_Bmumu;   //!
   TBranch        *b_EF_mu4_BmumuX;   //!
   TBranch        *b_EF_mu4_DiMu;   //!
   TBranch        *b_EF_mu4_DiMu_FS;   //!
   TBranch        *b_EF_mu4_DiMu_FS_noOS;   //!
   TBranch        *b_EF_mu4_DiMu_MG;   //!
   TBranch        *b_EF_mu4_DiMu_MG_FS;   //!
   TBranch        *b_EF_mu4_DiMu_SiTrk;   //!
   TBranch        *b_EF_mu4_DiMu_SiTrk_FS;   //!
   TBranch        *b_EF_mu4_DiMu_noOS;   //!
   TBranch        *b_EF_mu4_IDTrkNoCut;   //!
   TBranch        *b_EF_mu4_Jpsie5e3;   //!
   TBranch        *b_EF_mu4_Jpsie5e3_FS;   //!
   TBranch        *b_EF_mu4_Jpsie5e3_SiTrk;   //!
   TBranch        *b_EF_mu4_Jpsie5e3_SiTrk_FS;   //!
   TBranch        *b_EF_mu4_Jpsimumu;   //!
   TBranch        *b_EF_mu4_Jpsimumu_FS;   //!
   TBranch        *b_EF_mu4_Jpsimumu_SiTrk_FS;   //!
   TBranch        *b_EF_mu4_L1J10_matched;   //!
   TBranch        *b_EF_mu4_L1J15_matched;   //!
   TBranch        *b_EF_mu4_L1J30_matched;   //!
   TBranch        *b_EF_mu4_L1J55_matched;   //!
   TBranch        *b_EF_mu4_L1J5_matched;   //!
   TBranch        *b_EF_mu4_L2MSonly_EFFS_passL2;   //!
   TBranch        *b_EF_mu4_MG;   //!
   TBranch        *b_EF_mu4_MSonly;   //!
   TBranch        *b_EF_mu4_MSonly_EFFS_passL2;   //!
   TBranch        *b_EF_mu4_MSonly_MB2_noL2_EFFS;   //!
   TBranch        *b_EF_mu4_MSonly_cosmic;   //!
   TBranch        *b_EF_mu4_MSonly_outOfTime;   //!
   TBranch        *b_EF_mu4_MV;   //!
   TBranch        *b_EF_mu4_SiTrk;   //!
   TBranch        *b_EF_mu4_Trk_Jpsi;   //!
   TBranch        *b_EF_mu4_Trk_Jpsi_FS;   //!
   TBranch        *b_EF_mu4_Trk_Jpsi_loose;   //!
   TBranch        *b_EF_mu4_Trk_Upsi_FS;   //!
   TBranch        *b_EF_mu4_Trk_Upsi_loose_FS;   //!
   TBranch        *b_EF_mu4_Upsimumu_FS;   //!
   TBranch        *b_EF_mu4_Upsimumu_SiTrk_FS;   //!
   TBranch        *b_EF_mu4_comm_MSonly_cosmic;   //!
   TBranch        *b_EF_mu4_comm_cosmic;   //!
   TBranch        *b_EF_mu4_comm_firstempty;   //!
   TBranch        *b_EF_mu4_comm_unpaired_iso;   //!
   TBranch        *b_EF_mu4_cosmic;   //!
   TBranch        *b_EF_mu4_firstempty;   //!
   TBranch        *b_EF_mu4_j20;   //!
   TBranch        *b_EF_mu4_j20_jetNoEF;   //!
   TBranch        *b_EF_mu4_j30;   //!
   TBranch        *b_EF_mu4_j30_jetNoEF;   //!
   TBranch        *b_EF_mu4_mu6;   //!
   TBranch        *b_EF_mu4_muCombTag;   //!
   TBranch        *b_EF_mu4_tile;   //!
   TBranch        *b_EF_mu4_tile_cosmic;   //!
   TBranch        *b_EF_mu4_unpaired_iso;   //!
   TBranch        *b_EF_mu4mu6_Bmumu;   //!
   TBranch        *b_EF_mu4mu6_BmumuX;   //!
   TBranch        *b_EF_mu4mu6_DiMu;   //!
   TBranch        *b_EF_mu4mu6_Jpsimumu;   //!
   TBranch        *b_EF_mu4mu6_Jpsimumu_IDTrkNoCut;   //!
   TBranch        *b_EF_mu4mu6_Upsimumu;   //!
   TBranch        *b_EF_mu6;   //!
   TBranch        *b_EF_mu6_Bmumu;   //!
   TBranch        *b_EF_mu6_BmumuX;   //!
   TBranch        *b_EF_mu6_DiMu;   //!
   TBranch        *b_EF_mu6_Ecut12;   //!
   TBranch        *b_EF_mu6_Ecut123;   //!
   TBranch        *b_EF_mu6_Ecut13;   //!
   TBranch        *b_EF_mu6_Ecut2;   //!
   TBranch        *b_EF_mu6_Ecut3;   //!
   TBranch        *b_EF_mu6_IDTrkNoCut;   //!
   TBranch        *b_EF_mu6_Jpsie5e3;   //!
   TBranch        *b_EF_mu6_Jpsie5e3_FS;   //!
   TBranch        *b_EF_mu6_Jpsie5e3_SiTrk;   //!
   TBranch        *b_EF_mu6_Jpsie5e3_SiTrk_FS;   //!
   TBranch        *b_EF_mu6_Jpsimumu;   //!
   TBranch        *b_EF_mu6_MG;   //!
   TBranch        *b_EF_mu6_MSonly;   //!
   TBranch        *b_EF_mu6_MSonly_Ecut12;   //!
   TBranch        *b_EF_mu6_MSonly_Ecut123;   //!
   TBranch        *b_EF_mu6_MSonly_Ecut13;   //!
   TBranch        *b_EF_mu6_MSonly_Ecut2;   //!
   TBranch        *b_EF_mu6_MSonly_Ecut3;   //!
   TBranch        *b_EF_mu6_MSonly_outOfTime;   //!
   TBranch        *b_EF_mu6_NoAlg;   //!
   TBranch        *b_EF_mu6_SiTrk;   //!
   TBranch        *b_EF_mu6_Trk_Jpsi;   //!
   TBranch        *b_EF_mu6_Upsimumu_FS;   //!
   TBranch        *b_EF_mu6_muCombTag;   //!
   TBranch        *b_EF_tau125_loose;   //!
   TBranch        *b_EF_tau125_medium;   //!
   TBranch        *b_EF_tau12_IDTrkNoCut;   //!
   TBranch        *b_EF_tau12_loose;   //!
   TBranch        *b_EF_tau12_loose_2b15;   //!
   TBranch        *b_EF_tau12_loose_3j35;   //!
   TBranch        *b_EF_tau12_loose_3j35_jetNoEF;   //!
   TBranch        *b_EF_tau12_loose_EFxe12_noMu;   //!
   TBranch        *b_EF_tau12_loose_IdScan_EFxe12_noMu;   //!
   TBranch        *b_EF_tau12_loose_IdScan_xe15_noMu;   //!
   TBranch        *b_EF_tau12_loose_e10_loose;   //!
   TBranch        *b_EF_tau12_loose_e10_medium;   //!
   TBranch        *b_EF_tau12_loose_e10_tight;   //!
   TBranch        *b_EF_tau12_loose_mu10;   //!
   TBranch        *b_EF_tau12_loose_xe15_noMu;   //!
   TBranch        *b_EF_tau12_loose_xe20_noMu;   //!
   TBranch        *b_EF_tau16_loose;   //!
   TBranch        *b_EF_tau16_loose_2b15;   //!
   TBranch        *b_EF_tau16_loose_3j35;   //!
   TBranch        *b_EF_tau16_loose_3j35_jetNoEF;   //!
   TBranch        *b_EF_tau16_loose_e10_loose;   //!
   TBranch        *b_EF_tau16_loose_e15_loose;   //!
   TBranch        *b_EF_tau16_loose_mu10;   //!
   TBranch        *b_EF_tau16_loose_mu15;   //!
   TBranch        *b_EF_tau16_loose_xe20_noMu;   //!
   TBranch        *b_EF_tau16_loose_xe25_noMu;   //!
   TBranch        *b_EF_tau16_loose_xe25_tight_noMu;   //!
   TBranch        *b_EF_tau16_loose_xe30_noMu;   //!
   TBranch        *b_EF_tau16_medium;   //!
   TBranch        *b_EF_tau16_medium_xe22_noMu;   //!
   TBranch        *b_EF_tau16_medium_xe25_noMu;   //!
   TBranch        *b_EF_tau16_medium_xe25_tight_noMu;   //!
   TBranch        *b_EF_tau20_loose;   //!
   TBranch        *b_EF_tau20_loose_xe25_noMu;   //!
   TBranch        *b_EF_tau29_loose;   //!
   TBranch        *b_EF_tau29_loose1;   //!
   TBranch        *b_EF_tau38_loose;   //!
   TBranch        *b_EF_tau38_medium;   //!
   TBranch        *b_EF_tau50_IDTrkNoCut;   //!
   TBranch        *b_EF_tau50_loose;   //!
   TBranch        *b_EF_tau50_loose_IdScan;   //!
   TBranch        *b_EF_tau50_medium;   //!
   TBranch        *b_EF_tau84_loose;   //!
   TBranch        *b_EF_tau84_medium;   //!
   TBranch        *b_EF_tauNoCut;   //!
   TBranch        *b_EF_tauNoCut_IdScan;   //!
   TBranch        *b_EF_tauNoCut_cosmic;   //!
   TBranch        *b_EF_tauNoCut_firstempty;   //!
   TBranch        *b_EF_tauNoCut_hasTrk6_EFxe15_noMu;   //!
   TBranch        *b_EF_tauNoCut_hasTrk6_IdScan_EFxe15_noMu;   //!
   TBranch        *b_EF_tauNoCut_hasTrk6_IdScan_xe20_noMu;   //!
   TBranch        *b_EF_tauNoCut_hasTrk6_xe20_noMu;   //!
   TBranch        *b_EF_tauNoCut_hasTrk9_xe20_noMu;   //!
   TBranch        *b_EF_tauNoCut_hasTrk_MV;   //!
   TBranch        *b_EF_tauNoCut_hasTrk_e10_tight;   //!
   TBranch        *b_EF_tauNoCut_hasTrk_xe20_noMu;   //!
   TBranch        *b_EF_tauNoCut_unpaired_iso;   //!
   TBranch        *b_EF_tauNoCut_unpaired_noniso;   //!
   TBranch        *b_EF_xe15;   //!
   TBranch        *b_EF_xe15_noMu;   //!
   TBranch        *b_EF_xe15_unbiased_noMu;   //!
   TBranch        *b_EF_xe20;   //!
   TBranch        *b_EF_xe20_noMu;   //!
   TBranch        *b_EF_xe20_tight_noMu;   //!
   TBranch        *b_EF_xe20_tight_vfj_noMu;   //!
   TBranch        *b_EF_xe25;   //!
   TBranch        *b_EF_xe25_medium;   //!
   TBranch        *b_EF_xe25_medium_noMu;   //!
   TBranch        *b_EF_xe25_noMu;   //!
   TBranch        *b_EF_xe25_tight_noMu;   //!
   TBranch        *b_EF_xe25_tight_vfj_noMu;   //!
   TBranch        *b_EF_xe25_vfj_noMu;   //!
   TBranch        *b_EF_xe30;   //!
   TBranch        *b_EF_xe30_allL1;   //!
   TBranch        *b_EF_xe30_allL1_FEB;   //!
   TBranch        *b_EF_xe30_allL1_allCells;   //!
   TBranch        *b_EF_xe30_allL1_noMu;   //!
   TBranch        *b_EF_xe30_loose;   //!
   TBranch        *b_EF_xe30_loose_noMu;   //!
   TBranch        *b_EF_xe30_medium;   //!
   TBranch        *b_EF_xe30_medium_noMu;   //!
   TBranch        *b_EF_xe30_medium_vfj_noMu;   //!
   TBranch        *b_EF_xe30_noMu;   //!
   TBranch        *b_EF_xe30_tight_noMu;   //!
   TBranch        *b_EF_xe30_tight_vfj_noMu;   //!
   TBranch        *b_EF_xe30_vfj_noMu;   //!
   TBranch        *b_EF_xe35;   //!
   TBranch        *b_EF_xe35_loose_noMu;   //!
   TBranch        *b_EF_xe35_noMu;   //!
   TBranch        *b_EF_xe35_tight_noMu;   //!
   TBranch        *b_EF_xe40;   //!
   TBranch        *b_EF_xe40_noMu;   //!
   TBranch        *b_EF_xe40_tight_noMu;   //!
   TBranch        *b_EF_xe45;   //!
   TBranch        *b_EF_xe45_noMu;   //!
   TBranch        *b_EF_xe55;   //!
   TBranch        *b_EF_xe55_noMu;   //!
   TBranch        *b_EF_xe60_medium;   //!
   TBranch        *b_EF_xe60_medium_noMu;   //!
   TBranch        *b_EF_xe80_medium;   //!
   TBranch        *b_EF_xe80_medium_noMu;   //!
   TBranch        *b_L1_2EM10;   //!
   TBranch        *b_L1_2EM14;   //!
   TBranch        *b_L1_2EM2;   //!
   TBranch        *b_L1_2EM3;   //!
   TBranch        *b_L1_2EM5;   //!
   TBranch        *b_L1_2J15_XE10;   //!
   TBranch        *b_L1_2J15_XE15;   //!
   TBranch        *b_L1_2J15_XE25;   //!
   TBranch        *b_L1_2MU0;   //!
   TBranch        *b_L1_2MU0_FIRSTEMPTY;   //!
   TBranch        *b_L1_2MU0_MU6;   //!
   TBranch        *b_L1_2MU10;   //!
   TBranch        *b_L1_2MU20;   //!
   TBranch        *b_L1_2MU6;   //!
   TBranch        *b_L1_2MU6_EM5;   //!
   TBranch        *b_L1_2TAU11;   //!
   TBranch        *b_L1_2TAU5;   //!
   TBranch        *b_L1_2TAU5_EM5;   //!
   TBranch        *b_L1_2TAU6;   //!
   TBranch        *b_L1_2TAU6_EM10;   //!
   TBranch        *b_L1_2TAU6_EM5;   //!
   TBranch        *b_L1_EM10;   //!
   TBranch        *b_L1_EM10I;   //!
   TBranch        *b_L1_EM14;   //!
   TBranch        *b_L1_EM14I;   //!
   TBranch        *b_L1_EM14_XE10;   //!
   TBranch        *b_L1_EM14_XE15;   //!
   TBranch        *b_L1_EM2;   //!
   TBranch        *b_L1_EM2_UNPAIRED_ISO;   //!
   TBranch        *b_L1_EM2_UNPAIRED_NONISO;   //!
   TBranch        *b_L1_EM3;   //!
   TBranch        *b_L1_EM3_EMPTY;   //!
   TBranch        *b_L1_EM3_FIRSTEMPTY;   //!
   TBranch        *b_L1_EM3_MV;   //!
   TBranch        *b_L1_EM5;   //!
   TBranch        *b_L1_EM5_MU10;   //!
   TBranch        *b_L1_EM5_MU6;   //!
   TBranch        *b_L1_EM85;   //!
   TBranch        *b_L1_J15_XE15_EM10;   //!
   TBranch        *b_L1_J15_XE15_MU15;   //!
   TBranch        *b_L1_J30_XE10;   //!
   TBranch        *b_L1_J30_XE15;   //!
   TBranch        *b_L1_J30_XE25;   //!
   TBranch        *b_L1_MU0;   //!
   TBranch        *b_L1_MU0_COMM;   //!
   TBranch        *b_L1_MU0_COMM_EMPTY;   //!
   TBranch        *b_L1_MU0_COMM_FIRSTEMPTY;   //!
   TBranch        *b_L1_MU0_COMM_UNPAIRED_ISO;   //!
   TBranch        *b_L1_MU0_COMM_UNPAIRED_NONISO;   //!
   TBranch        *b_L1_MU0_EM3;   //!
   TBranch        *b_L1_MU0_EMPTY;   //!
   TBranch        *b_L1_MU0_FIRSTEMPTY;   //!
   TBranch        *b_L1_MU0_J10;   //!
   TBranch        *b_L1_MU0_J15;   //!
   TBranch        *b_L1_MU0_J30;   //!
   TBranch        *b_L1_MU0_J5;   //!
   TBranch        *b_L1_MU0_J55;   //!
   TBranch        *b_L1_MU0_MV;   //!
   TBranch        *b_L1_MU0_UNPAIRED_ISO;   //!
   TBranch        *b_L1_MU0_UNPAIRED_NONISO;   //!
   TBranch        *b_L1_MU10;   //!
   TBranch        *b_L1_MU10_FIRSTEMPTY;   //!
   TBranch        *b_L1_MU10_J10;   //!
   TBranch        *b_L1_MU15;   //!
   TBranch        *b_L1_MU20;   //!
   TBranch        *b_L1_MU6;   //!
   TBranch        *b_L1_MU6_EM3;   //!
   TBranch        *b_L1_MU6_FIRSTEMPTY;   //!
   TBranch        *b_L1_MU6_J5;   //!
   TBranch        *b_L1_TAU11;   //!
   TBranch        *b_L1_TAU11I;   //!
   TBranch        *b_L1_TAU20;   //!
   TBranch        *b_L1_TAU30;   //!
   TBranch        *b_L1_TAU5;   //!
   TBranch        *b_L1_TAU50;   //!
   TBranch        *b_L1_TAU5_3J5_2J15;   //!
   TBranch        *b_L1_TAU5_4J5_3J15;   //!
   TBranch        *b_L1_TAU5_EMPTY;   //!
   TBranch        *b_L1_TAU5_FIRSTEMPTY;   //!
   TBranch        *b_L1_TAU5_MU6;   //!
   TBranch        *b_L1_TAU5_MV;   //!
   TBranch        *b_L1_TAU5_UNPAIRED_ISO;   //!
   TBranch        *b_L1_TAU5_UNPAIRED_NONISO;   //!
   TBranch        *b_L1_TAU5_XE10;   //!
   TBranch        *b_L1_TAU6;   //!
   TBranch        *b_L1_TAU6I;   //!
   TBranch        *b_L1_TAU6_3J5_2J15;   //!
   TBranch        *b_L1_TAU6_4J5_3J15;   //!
   TBranch        *b_L1_TAU6_MU10;   //!
   TBranch        *b_L1_TAU6_MU15;   //!
   TBranch        *b_L1_TAU6_XE10;   //!
   TBranch        *b_L1_XE10;   //!
   TBranch        *b_L1_XE15;   //!
   TBranch        *b_L1_XE20;   //!
   TBranch        *b_L1_XE25;   //!
   TBranch        *b_L1_XE30;   //!
   TBranch        *b_L1_XE35;   //!
   TBranch        *b_L1_XE40;   //!
   TBranch        *b_L1_XE50;   //!
   TBranch        *b_L2_2e10_loose;   //!
   TBranch        *b_L2_2e10_medium;   //!
   TBranch        *b_L2_2e15_loose;   //!
   TBranch        *b_L2_2e3_loose;   //!
   TBranch        *b_L2_2e3_loose_SiTrk;   //!
   TBranch        *b_L2_2e3_loose_TRT;   //!
   TBranch        *b_L2_2e3_medium;   //!
   TBranch        *b_L2_2e3_medium_SiTrk;   //!
   TBranch        *b_L2_2e3_medium_TRT;   //!
   TBranch        *b_L2_2e3_tight;   //!
   TBranch        *b_L2_2e5_medium;   //!
   TBranch        *b_L2_2e5_medium_SiTrk;   //!
   TBranch        *b_L2_2e5_medium_TRT;   //!
   TBranch        *b_L2_2e5_tight;   //!
   TBranch        *b_L2_2g10_loose;   //!
   TBranch        *b_L2_2g15_loose;   //!
   TBranch        *b_L2_2g20_loose;   //!
   TBranch        *b_L2_2g5_loose;   //!
   TBranch        *b_L2_2g7_loose;   //!
   TBranch        *b_L2_2j30_xe12_noMu;   //!
   TBranch        *b_L2_2j30_xe20_noMu;   //!
   TBranch        *b_L2_2j30_xe30_noMu;   //!
   TBranch        *b_L2_2j35_jetNoEF_xe20_noMu;   //!
   TBranch        *b_L2_2j35_jetNoEF_xe30_noMu;   //!
   TBranch        *b_L2_2j35_jetNoEF_xe40_noMu;   //!
   TBranch        *b_L2_2mu0_NoAlg;   //!
   TBranch        *b_L2_2mu10;   //!
   TBranch        *b_L2_2mu10_NoAlg;   //!
   TBranch        *b_L2_2mu4;   //!
   TBranch        *b_L2_2mu4_Bmumu;   //!
   TBranch        *b_L2_2mu4_Bmumux;   //!
   TBranch        *b_L2_2mu4_DiMu;   //!
   TBranch        *b_L2_2mu4_DiMu_SiTrk;   //!
   TBranch        *b_L2_2mu4_DiMu_noVtx;   //!
   TBranch        *b_L2_2mu4_DiMu_noVtx_noOS;   //!
   TBranch        *b_L2_2mu4_Jpsimumu;   //!
   TBranch        *b_L2_2mu4_Jpsimumu_IDTrkNoCut;   //!
   TBranch        *b_L2_2mu4_Upsimumu;   //!
   TBranch        *b_L2_2mu6;   //!
   TBranch        *b_L2_2mu6_Bmumu;   //!
   TBranch        *b_L2_2mu6_Bmumux;   //!
   TBranch        *b_L2_2mu6_DiMu;   //!
   TBranch        *b_L2_2mu6_Jpsimumu;   //!
   TBranch        *b_L2_2mu6_MG;   //!
   TBranch        *b_L2_2mu6_NoAlg;   //!
   TBranch        *b_L2_2mu6_Upsimumu;   //!
   TBranch        *b_L2_2mu6_g10_loose;   //!
   TBranch        *b_L2_2tau12_loose;   //!
   TBranch        *b_L2_2tau16_loose;   //!
   TBranch        *b_L2_2tau20_loose;   //!
   TBranch        *b_L2_2tau29_loose;   //!
   TBranch        *b_L2_2tau29_loose1;   //!
   TBranch        *b_L2_2tau38_loose;   //!
   TBranch        *b_L2_e10_NoCut;   //!
   TBranch        *b_L2_e10_loose;   //!
   TBranch        *b_L2_e10_loose_mu10;   //!
   TBranch        *b_L2_e10_loose_mu6;   //!
   TBranch        *b_L2_e10_medium;   //!
   TBranch        *b_L2_e10_medium_IDTrkNoCut;   //!
   TBranch        *b_L2_e10_medium_SiTrk;   //!
   TBranch        *b_L2_e10_medium_TRT;   //!
   TBranch        *b_L2_e10_tight;   //!
   TBranch        *b_L2_e10_tight_TRT;   //!
   TBranch        *b_L2_e15_loose;   //!
   TBranch        *b_L2_e15_loose_IDTrkNoCut;   //!
   TBranch        *b_L2_e15_medium;   //!
   TBranch        *b_L2_e15_medium_SiTrk;   //!
   TBranch        *b_L2_e15_medium_TRT;   //!
   TBranch        *b_L2_e15_tight;   //!
   TBranch        *b_L2_e15_tight_TRT;   //!
   TBranch        *b_L2_e18_medium;   //!
   TBranch        *b_L2_e20_loose;   //!
   TBranch        *b_L2_e20_loose_IDTrkNoCut;   //!
   TBranch        *b_L2_e20_loose_TRT;   //!
   TBranch        *b_L2_e20_loose_passEF;   //!
   TBranch        *b_L2_e20_loose_passL2;   //!
   TBranch        *b_L2_e20_loose_xe20;   //!
   TBranch        *b_L2_e20_loose_xe20_noMu;   //!
   TBranch        *b_L2_e20_loose_xe30;   //!
   TBranch        *b_L2_e20_loose_xe30_noMu;   //!
   TBranch        *b_L2_e20_medium;   //!
   TBranch        *b_L2_e25_loose;   //!
   TBranch        *b_L2_e25_medium;   //!
   TBranch        *b_L2_e30_loose;   //!
   TBranch        *b_L2_e3_loose;   //!
   TBranch        *b_L2_e3_loose_SiTrk;   //!
   TBranch        *b_L2_e3_loose_TRT;   //!
   TBranch        *b_L2_e3_medium;   //!
   TBranch        *b_L2_e3_medium_SiTrk;   //!
   TBranch        *b_L2_e3_medium_TRT;   //!
   TBranch        *b_L2_e5_NoCut_L2SW;   //!
   TBranch        *b_L2_e5_NoCut_Ringer;   //!
   TBranch        *b_L2_e5_NoCut_firstempty;   //!
   TBranch        *b_L2_e5_loose_mu4;   //!
   TBranch        *b_L2_e5_medium;   //!
   TBranch        *b_L2_e5_medium_MV;   //!
   TBranch        *b_L2_e5_medium_SiTrk;   //!
   TBranch        *b_L2_e5_medium_TRT;   //!
   TBranch        *b_L2_e5_medium_mu4;   //!
   TBranch        *b_L2_e5_tight;   //!
   TBranch        *b_L2_e5_tight_SiTrk;   //!
   TBranch        *b_L2_e5_tight_TRT;   //!
   TBranch        *b_L2_e5_tight_e5_NoCut;   //!
   TBranch        *b_L2_em105_passHLT;   //!
   TBranch        *b_L2_em3_empty_larcalib;   //!
   TBranch        *b_L2_g10_loose;   //!
   TBranch        *b_L2_g11_etcut;   //!
   TBranch        *b_L2_g15_loose;   //!
   TBranch        *b_L2_g17_etcut;   //!
   TBranch        *b_L2_g17_etcut_EFxe20_noMu;   //!
   TBranch        *b_L2_g17_etcut_EFxe30_noMu;   //!
   TBranch        *b_L2_g20_loose;   //!
   TBranch        *b_L2_g20_loose_cnv;   //!
   TBranch        *b_L2_g20_loose_xe20_noMu;   //!
   TBranch        *b_L2_g20_loose_xe30_noMu;   //!
   TBranch        *b_L2_g20_tight;   //!
   TBranch        *b_L2_g25_loose_xe30_noMu;   //!
   TBranch        *b_L2_g30_loose;   //!
   TBranch        *b_L2_g30_tight;   //!
   TBranch        *b_L2_g3_NoCut_unpaired_iso;   //!
   TBranch        *b_L2_g3_NoCut_unpaired_noniso;   //!
   TBranch        *b_L2_g40_loose;   //!
   TBranch        *b_L2_g40_tight;   //!
   TBranch        *b_L2_g50_loose;   //!
   TBranch        *b_L2_g5_NoCut_cosmic;   //!
   TBranch        *b_L2_g5_loose;   //!
   TBranch        *b_L2_g5_loose_cnv;   //!
   TBranch        *b_L2_g7_loose;   //!
   TBranch        *b_L2_j30_jetNoCut_xe20_e15_medium;   //!
   TBranch        *b_L2_j30_xe20_e15_medium;   //!
   TBranch        *b_L2_j35_jetNoCut_xe30_mu15;   //!
   TBranch        *b_L2_j35_xe30_mu15;   //!
   TBranch        *b_L2_j45_xe12_noMu;   //!
   TBranch        *b_L2_j45_xe20_noMu;   //!
   TBranch        *b_L2_j45_xe30_noMu;   //!
   TBranch        *b_L2_mu0_NoAlg;   //!
   TBranch        *b_L2_mu0_comm_NoAlg;   //!
   TBranch        *b_L2_mu0_comm_empty_NoAlg;   //!
   TBranch        *b_L2_mu0_comm_firstempty_NoAlg;   //!
   TBranch        *b_L2_mu0_comm_unpaired_iso_NoAlg;   //!
   TBranch        *b_L2_mu0_comm_unpaired_noniso_NoAlg;   //!
   TBranch        *b_L2_mu0_empty_NoAlg;   //!
   TBranch        *b_L2_mu0_firstempty_NoAlg;   //!
   TBranch        *b_L2_mu0_missingRoi;   //!
   TBranch        *b_L2_mu0_outOfTime1;   //!
   TBranch        *b_L2_mu0_outOfTime2;   //!
   TBranch        *b_L2_mu0_rpcOnly;   //!
   TBranch        *b_L2_mu0_unpaired_iso_NoAlg;   //!
   TBranch        *b_L2_mu0_unpaired_noniso_NoAlg;   //!
   TBranch        *b_L2_mu10;   //!
   TBranch        *b_L2_mu10_Ecut12;   //!
   TBranch        *b_L2_mu10_Ecut123;   //!
   TBranch        *b_L2_mu10_Ecut13;   //!
   TBranch        *b_L2_mu10_IDTrkNoCut;   //!
   TBranch        *b_L2_mu10_MG;   //!
   TBranch        *b_L2_mu10_MSonly;   //!
   TBranch        *b_L2_mu10_MSonly_Ecut12;   //!
   TBranch        *b_L2_mu10_MSonly_Ecut123;   //!
   TBranch        *b_L2_mu10_MSonly_Ecut13;   //!
   TBranch        *b_L2_mu10_MSonly_tight;   //!
   TBranch        *b_L2_mu10_NoAlg;   //!
   TBranch        *b_L2_mu10_SiTrk;   //!
   TBranch        *b_L2_mu10_j30;   //!
   TBranch        *b_L2_mu10_tight;   //!
   TBranch        *b_L2_mu10i_loose;   //!
   TBranch        *b_L2_mu13;   //!
   TBranch        *b_L2_mu13_MG;   //!
   TBranch        *b_L2_mu13_MG_tight;   //!
   TBranch        *b_L2_mu13_tight;   //!
   TBranch        *b_L2_mu15;   //!
   TBranch        *b_L2_mu15_MG;   //!
   TBranch        *b_L2_mu15_NoAlg;   //!
   TBranch        *b_L2_mu20;   //!
   TBranch        *b_L2_mu20_MSonly;   //!
   TBranch        *b_L2_mu20_NoAlg;   //!
   TBranch        *b_L2_mu20_passHLT;   //!
   TBranch        *b_L2_mu20_slow;   //!
   TBranch        *b_L2_mu30_MSonly;   //!
   TBranch        *b_L2_mu4;   //!
   TBranch        *b_L2_mu40_MSonly;   //!
   TBranch        *b_L2_mu4_Bmumu;   //!
   TBranch        *b_L2_mu4_BmumuX;   //!
   TBranch        *b_L2_mu4_DiMu;   //!
   TBranch        *b_L2_mu4_DiMu_FS;   //!
   TBranch        *b_L2_mu4_DiMu_FS_noOS;   //!
   TBranch        *b_L2_mu4_DiMu_MG;   //!
   TBranch        *b_L2_mu4_DiMu_MG_FS;   //!
   TBranch        *b_L2_mu4_DiMu_SiTrk;   //!
   TBranch        *b_L2_mu4_DiMu_SiTrk_FS;   //!
   TBranch        *b_L2_mu4_DiMu_noOS;   //!
   TBranch        *b_L2_mu4_IDTrkNoCut;   //!
   TBranch        *b_L2_mu4_Jpsie5e3;   //!
   TBranch        *b_L2_mu4_Jpsie5e3_FS;   //!
   TBranch        *b_L2_mu4_Jpsie5e3_SiTrk;   //!
   TBranch        *b_L2_mu4_Jpsie5e3_SiTrk_FS;   //!
   TBranch        *b_L2_mu4_Jpsimumu;   //!
   TBranch        *b_L2_mu4_Jpsimumu_FS;   //!
   TBranch        *b_L2_mu4_Jpsimumu_SiTrk_FS;   //!
   TBranch        *b_L2_mu4_L1J10_matched;   //!
   TBranch        *b_L2_mu4_L1J15_matched;   //!
   TBranch        *b_L2_mu4_L1J30_matched;   //!
   TBranch        *b_L2_mu4_L1J55_matched;   //!
   TBranch        *b_L2_mu4_L1J5_matched;   //!
   TBranch        *b_L2_mu4_L2MSonly_EFFS_passL2;   //!
   TBranch        *b_L2_mu4_MG;   //!
   TBranch        *b_L2_mu4_MSonly;   //!
   TBranch        *b_L2_mu4_MSonly_EFFS_passL2;   //!
   TBranch        *b_L2_mu4_MSonly_MB2_noL2_EFFS;   //!
   TBranch        *b_L2_mu4_MSonly_cosmic;   //!
   TBranch        *b_L2_mu4_MSonly_outOfTime;   //!
   TBranch        *b_L2_mu4_MV;   //!
   TBranch        *b_L2_mu4_SiTrk;   //!
   TBranch        *b_L2_mu4_Trk_Jpsi;   //!
   TBranch        *b_L2_mu4_Trk_Jpsi_FS;   //!
   TBranch        *b_L2_mu4_Trk_Jpsi_loose;   //!
   TBranch        *b_L2_mu4_Trk_Upsi_FS;   //!
   TBranch        *b_L2_mu4_Trk_Upsi_loose_FS;   //!
   TBranch        *b_L2_mu4_Upsimumu_FS;   //!
   TBranch        *b_L2_mu4_Upsimumu_SiTrk_FS;   //!
   TBranch        *b_L2_mu4_comm_MSonly_cosmic;   //!
   TBranch        *b_L2_mu4_comm_cosmic;   //!
   TBranch        *b_L2_mu4_comm_firstempty;   //!
   TBranch        *b_L2_mu4_comm_unpaired_iso;   //!
   TBranch        *b_L2_mu4_cosmic;   //!
   TBranch        *b_L2_mu4_firstempty;   //!
   TBranch        *b_L2_mu4_j20;   //!
   TBranch        *b_L2_mu4_j20_jetNoEF;   //!
   TBranch        *b_L2_mu4_j25;   //!
   TBranch        *b_L2_mu4_mu6;   //!
   TBranch        *b_L2_mu4_muCombTag;   //!
   TBranch        *b_L2_mu4_tile;   //!
   TBranch        *b_L2_mu4_tile_cosmic;   //!
   TBranch        *b_L2_mu4_unpaired_iso;   //!
   TBranch        *b_L2_mu4mu6_Bmumu;   //!
   TBranch        *b_L2_mu4mu6_BmumuX;   //!
   TBranch        *b_L2_mu4mu6_DiMu;   //!
   TBranch        *b_L2_mu4mu6_Jpsimumu;   //!
   TBranch        *b_L2_mu4mu6_Jpsimumu_IDTrkNoCut;   //!
   TBranch        *b_L2_mu4mu6_Upsimumu;   //!
   TBranch        *b_L2_mu6;   //!
   TBranch        *b_L2_mu6_Bmumu;   //!
   TBranch        *b_L2_mu6_BmumuX;   //!
   TBranch        *b_L2_mu6_DiMu;   //!
   TBranch        *b_L2_mu6_Ecut12;   //!
   TBranch        *b_L2_mu6_Ecut123;   //!
   TBranch        *b_L2_mu6_Ecut13;   //!
   TBranch        *b_L2_mu6_Ecut2;   //!
   TBranch        *b_L2_mu6_Ecut3;   //!
   TBranch        *b_L2_mu6_IDTrkNoCut;   //!
   TBranch        *b_L2_mu6_Jpsie5e3;   //!
   TBranch        *b_L2_mu6_Jpsie5e3_FS;   //!
   TBranch        *b_L2_mu6_Jpsie5e3_SiTrk;   //!
   TBranch        *b_L2_mu6_Jpsie5e3_SiTrk_FS;   //!
   TBranch        *b_L2_mu6_Jpsimumu;   //!
   TBranch        *b_L2_mu6_MG;   //!
   TBranch        *b_L2_mu6_MSonly;   //!
   TBranch        *b_L2_mu6_MSonly_Ecut12;   //!
   TBranch        *b_L2_mu6_MSonly_Ecut123;   //!
   TBranch        *b_L2_mu6_MSonly_Ecut13;   //!
   TBranch        *b_L2_mu6_MSonly_Ecut2;   //!
   TBranch        *b_L2_mu6_MSonly_Ecut3;   //!
   TBranch        *b_L2_mu6_MSonly_outOfTime;   //!
   TBranch        *b_L2_mu6_NoAlg;   //!
   TBranch        *b_L2_mu6_SiTrk;   //!
   TBranch        *b_L2_mu6_Trk_Jpsi;   //!
   TBranch        *b_L2_mu6_Upsimumu_FS;   //!
   TBranch        *b_L2_mu6_muCombTag;   //!
   TBranch        *b_L2_tau125_loose;   //!
   TBranch        *b_L2_tau125_medium;   //!
   TBranch        *b_L2_tau12_IDTrkNoCut;   //!
   TBranch        *b_L2_tau12_loose;   //!
   TBranch        *b_L2_tau12_loose_2b15;   //!
   TBranch        *b_L2_tau12_loose_3j30;   //!
   TBranch        *b_L2_tau12_loose_EFxe12_noMu;   //!
   TBranch        *b_L2_tau12_loose_IdScan_EFxe12_noMu;   //!
   TBranch        *b_L2_tau12_loose_IdScan_xe15_noMu;   //!
   TBranch        *b_L2_tau12_loose_e10_loose;   //!
   TBranch        *b_L2_tau12_loose_e10_medium;   //!
   TBranch        *b_L2_tau12_loose_e10_tight;   //!
   TBranch        *b_L2_tau12_loose_mu10;   //!
   TBranch        *b_L2_tau12_loose_xe15_noMu;   //!
   TBranch        *b_L2_tau12_loose_xe20_noMu;   //!
   TBranch        *b_L2_tau16_loose;   //!
   TBranch        *b_L2_tau16_loose_2b15;   //!
   TBranch        *b_L2_tau16_loose_3j30;   //!
   TBranch        *b_L2_tau16_loose_e10_loose;   //!
   TBranch        *b_L2_tau16_loose_e15_loose;   //!
   TBranch        *b_L2_tau16_loose_mu10;   //!
   TBranch        *b_L2_tau16_loose_mu15;   //!
   TBranch        *b_L2_tau16_loose_xe20_noMu;   //!
   TBranch        *b_L2_tau16_loose_xe25_noMu;   //!
   TBranch        *b_L2_tau16_loose_xe25_tight_noMu;   //!
   TBranch        *b_L2_tau16_loose_xe30_noMu;   //!
   TBranch        *b_L2_tau16_medium;   //!
   TBranch        *b_L2_tau16_medium_xe22_noMu;   //!
   TBranch        *b_L2_tau16_medium_xe25_noMu;   //!
   TBranch        *b_L2_tau16_medium_xe25_tight_noMu;   //!
   TBranch        *b_L2_tau20_loose;   //!
   TBranch        *b_L2_tau20_loose_xe25_noMu;   //!
   TBranch        *b_L2_tau29_loose;   //!
   TBranch        *b_L2_tau29_loose1;   //!
   TBranch        *b_L2_tau38_loose;   //!
   TBranch        *b_L2_tau38_medium;   //!
   TBranch        *b_L2_tau50_IDTrkNoCut;   //!
   TBranch        *b_L2_tau50_loose;   //!
   TBranch        *b_L2_tau50_loose_IdScan;   //!
   TBranch        *b_L2_tau50_medium;   //!
   TBranch        *b_L2_tau5_empty_larcalib;   //!
   TBranch        *b_L2_tau84_loose;   //!
   TBranch        *b_L2_tau84_medium;   //!
   TBranch        *b_L2_tauNoCut;   //!
   TBranch        *b_L2_tauNoCut_IdScan;   //!
   TBranch        *b_L2_tauNoCut_cosmic;   //!
   TBranch        *b_L2_tauNoCut_firstempty;   //!
   TBranch        *b_L2_tauNoCut_hasTrk6_EFxe15_noMu;   //!
   TBranch        *b_L2_tauNoCut_hasTrk6_IdScan_EFxe15_noMu;   //!
   TBranch        *b_L2_tauNoCut_hasTrk6_IdScan_xe20_noMu;   //!
   TBranch        *b_L2_tauNoCut_hasTrk6_xe20_noMu;   //!
   TBranch        *b_L2_tauNoCut_hasTrk9_xe20_noMu;   //!
   TBranch        *b_L2_tauNoCut_hasTrk_MV;   //!
   TBranch        *b_L2_tauNoCut_hasTrk_e10_tight;   //!
   TBranch        *b_L2_tauNoCut_hasTrk_xe20_noMu;   //!
   TBranch        *b_L2_tauNoCut_unpaired_iso;   //!
   TBranch        *b_L2_tauNoCut_unpaired_noniso;   //!
   TBranch        *b_L2_xe12;   //!
   TBranch        *b_L2_xe12_loose;   //!
   TBranch        *b_L2_xe12_loose_noMu;   //!
   TBranch        *b_L2_xe12_medium;   //!
   TBranch        *b_L2_xe12_medium_noMu;   //!
   TBranch        *b_L2_xe12_noMu;   //!
   TBranch        *b_L2_xe15;   //!
   TBranch        *b_L2_xe15_medium;   //!
   TBranch        *b_L2_xe15_medium_noMu;   //!
   TBranch        *b_L2_xe15_medium_vfj_noMu;   //!
   TBranch        *b_L2_xe15_noMu;   //!
   TBranch        *b_L2_xe15_tight_noMu;   //!
   TBranch        *b_L2_xe15_tight_vfj_noMu;   //!
   TBranch        *b_L2_xe15_unbiased_noMu;   //!
   TBranch        *b_L2_xe15_vfj_noMu;   //!
   TBranch        *b_L2_xe17_tight_noMu;   //!
   TBranch        *b_L2_xe17_tight_vfj_noMu;   //!
   TBranch        *b_L2_xe20;   //!
   TBranch        *b_L2_xe20_noMu;   //!
   TBranch        *b_L2_xe20_vfj_noMu;   //!
   TBranch        *b_L2_xe22_loose_noMu;   //!
   TBranch        *b_L2_xe22_tight_noMu;   //!
   TBranch        *b_L2_xe22_tight_vfj_noMu;   //!
   TBranch        *b_L2_xe25;   //!
   TBranch        *b_L2_xe25_noMu;   //!
   TBranch        *b_L2_xe27_tight_noMu;   //!
   TBranch        *b_L2_xe30;   //!
   TBranch        *b_L2_xe30_allL1;   //!
   TBranch        *b_L2_xe30_allL1_FEB;   //!
   TBranch        *b_L2_xe30_allL1_allCells;   //!
   TBranch        *b_L2_xe30_allL1_noMu;   //!
   TBranch        *b_L2_xe30_noMu;   //!
   TBranch        *b_L2_xe32_tight_noMu;   //!
   TBranch        *b_L2_xe35;   //!
   TBranch        *b_L2_xe35_noMu;   //!
   TBranch        *b_L2_xe40_medium;   //!
   TBranch        *b_L2_xe40_medium_noMu;   //!
   TBranch        *b_L2_xe45;   //!
   TBranch        *b_L2_xe45_noMu;   //!
   TBranch        *b_L2_xe60_medium;   //!
   TBranch        *b_L2_xe60_medium_noMu;   //!
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
   TBranch        *b_ph_convFlag;   //!
   TBranch        *b_ph_isConv;   //!
   TBranch        *b_ph_nConv;   //!
   TBranch        *b_ph_nSingleTrackConv;   //!
   TBranch        *b_ph_nDoubleTrackConv;   //!
   TBranch        *b_ph_type;   //!
   TBranch        *b_ph_origin;   //!
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
   TBranch        *b_ph_truth_index;   //!
   TBranch        *b_ph_truth_matched;   //!
   TBranch        *b_ph_loose;   //!
   TBranch        *b_ph_tight;   //!
   TBranch        *b_ph_tightIso;   //!
   TBranch        *b_ph_goodOQ;   //!
   TBranch        *b_ph_Ethad;   //!
   TBranch        *b_ph_Ethad1;   //!
   TBranch        *b_ph_E033;   //!
   TBranch        *b_ph_f1;   //!
   TBranch        *b_ph_f1core;   //!
   TBranch        *b_ph_Emins1;   //!
   TBranch        *b_ph_fside;   //!
   TBranch        *b_ph_Emax2;   //!
   TBranch        *b_ph_ws3;   //!
   TBranch        *b_ph_wstot;   //!
   TBranch        *b_ph_E132;   //!
   TBranch        *b_ph_E1152;   //!
   TBranch        *b_ph_emaxs1;   //!
   TBranch        *b_ph_deltaEs;   //!
   TBranch        *b_ph_E233;   //!
   TBranch        *b_ph_E237;   //!
   TBranch        *b_ph_E277;   //!
   TBranch        *b_ph_weta2;   //!
   TBranch        *b_ph_f3;   //!
   TBranch        *b_ph_f3core;   //!
   TBranch        *b_ph_rphiallcalo;   //!
   TBranch        *b_ph_Etcone45;   //!
   TBranch        *b_ph_Etcone20;   //!
   TBranch        *b_ph_Etcone30;   //!
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
   TBranch        *b_ph_EtringnoisedR03sig2;   //!
   TBranch        *b_ph_EtringnoisedR03sig3;   //!
   TBranch        *b_ph_EtringnoisedR03sig4;   //!
   TBranch        *b_ph_isolationlikelihoodjets;   //!
   TBranch        *b_ph_isolationlikelihoodhqelectrons;   //!
   TBranch        *b_ph_loglikelihood;   //!
   TBranch        *b_ph_photonweight;   //!
   TBranch        *b_ph_photonbgweight;   //!
   TBranch        *b_ph_neuralnet;   //!
   TBranch        *b_ph_Hmatrix;   //!
   TBranch        *b_ph_Hmatrix5;   //!
   TBranch        *b_ph_adaboost;   //!
   TBranch        *b_ph_zvertex;   //!
   TBranch        *b_ph_errz;   //!
   TBranch        *b_ph_etap;   //!
   TBranch        *b_ph_depth;   //!
   TBranch        *b_ph_cl_E;   //!
   TBranch        *b_ph_cl_pt;   //!
   TBranch        *b_ph_cl_eta;   //!
   TBranch        *b_ph_cl_phi;   //!
   TBranch        *b_ph_Es0;   //!
   TBranch        *b_ph_etas0;   //!
   TBranch        *b_ph_phis0;   //!
   TBranch        *b_ph_Es1;   //!
   TBranch        *b_ph_etas1;   //!
   TBranch        *b_ph_phis1;   //!
   TBranch        *b_ph_Es2;   //!
   TBranch        *b_ph_etas2;   //!
   TBranch        *b_ph_phis2;   //!
   TBranch        *b_ph_Es3;   //!
   TBranch        *b_ph_etas3;   //!
   TBranch        *b_ph_phis3;   //!
   TBranch        *b_ph_rawcl_Es0;   //!
   TBranch        *b_ph_rawcl_etas0;   //!
   TBranch        *b_ph_rawcl_phis0;   //!
   TBranch        *b_ph_rawcl_Es1;   //!
   TBranch        *b_ph_rawcl_etas1;   //!
   TBranch        *b_ph_rawcl_phis1;   //!
   TBranch        *b_ph_rawcl_Es2;   //!
   TBranch        *b_ph_rawcl_etas2;   //!
   TBranch        *b_ph_rawcl_phis2;   //!
   TBranch        *b_ph_rawcl_Es3;   //!
   TBranch        *b_ph_rawcl_etas3;   //!
   TBranch        *b_ph_rawcl_phis3;   //!
   TBranch        *b_ph_rawcl_E;   //!
   TBranch        *b_ph_rawcl_pt;   //!
   TBranch        *b_ph_rawcl_eta;   //!
   TBranch        *b_ph_rawcl_phi;   //!
   TBranch        *b_ph_truth_isConv;   //!
   TBranch        *b_ph_truth_isBrem;   //!
   TBranch        *b_ph_truth_isFromHardProc;   //!
   TBranch        *b_ph_truth_isPhotonFromHardProc;   //!
   TBranch        *b_ph_truth_Rconv;   //!
   TBranch        *b_ph_truth_zconv;   //!
   TBranch        *b_ph_deltaEmax2;   //!
   TBranch        *b_ph_calibHitsShowerDepth;   //!
   TBranch        *b_ph_isIso;   //!
   TBranch        *b_ph_mvaptcone20;   //!
   TBranch        *b_ph_mvaptcone30;   //!
   TBranch        *b_ph_mvaptcone40;   //!
   TBranch        *b_ph_topoEtcone20;   //!
   TBranch        *b_ph_topoEtcone40;   //!
   TBranch        *b_ph_topoEtcone60;   //!
   TBranch        *b_ph_jet_dr;   //!
   TBranch        *b_ph_jet_E;   //!
   TBranch        *b_ph_jet_pt;   //!
   TBranch        *b_ph_jet_m;   //!
   TBranch        *b_ph_jet_eta;   //!
   TBranch        *b_ph_jet_phi;   //!
   TBranch        *b_ph_jet_truth_dr;   //!
   TBranch        *b_ph_jet_truth_E;   //!
   TBranch        *b_ph_jet_truth_pt;   //!
   TBranch        *b_ph_jet_truth_m;   //!
   TBranch        *b_ph_jet_truth_eta;   //!
   TBranch        *b_ph_jet_truth_phi;   //!
   TBranch        *b_ph_jet_truth_matched;   //!
   TBranch        *b_ph_jet_matched;   //!
   TBranch        *b_ph_convIP;   //!
   TBranch        *b_ph_convIPRev;   //!
   TBranch        *b_ph_ptIsolationCone;   //!
   TBranch        *b_ph_ptIsolationConePhAngle;   //!
   TBranch        *b_ph_Etcone40_pt_corrected;   //!
   TBranch        *b_ph_Etcone40_ED_corrected;   //!
   TBranch        *b_ph_Etcone40_corrected;   //!
   TBranch        *b_ph_topodr;   //!
   TBranch        *b_ph_topopt;   //!
   TBranch        *b_ph_topoeta;   //!
   TBranch        *b_ph_topophi;   //!
   TBranch        *b_ph_topomatched;   //!
   TBranch        *b_ph_topoEMdr;   //!
   TBranch        *b_ph_topoEMpt;   //!
   TBranch        *b_ph_topoEMeta;   //!
   TBranch        *b_ph_topoEMphi;   //!
   TBranch        *b_ph_topoEMmatched;   //!
   TBranch        *b_ph_EF_dr;   //!
   TBranch        *b_ph_EF_index;   //!
   TBranch        *b_ph_L2_dr;   //!
   TBranch        *b_ph_L2_index;   //!
   TBranch        *b_ph_L1_dr;   //!
   TBranch        *b_ph_L1_index;   //!
   TBranch        *b_mu_muid_n;   //!
   TBranch        *b_mu_muid_E;   //!
   TBranch        *b_mu_muid_pt;   //!
   TBranch        *b_mu_muid_m;   //!
   TBranch        *b_mu_muid_eta;   //!
   TBranch        *b_mu_muid_phi;   //!
   TBranch        *b_mu_muid_px;   //!
   TBranch        *b_mu_muid_py;   //!
   TBranch        *b_mu_muid_pz;   //!
   TBranch        *b_mu_muid_charge;   //!
   TBranch        *b_mu_muid_allauthor;   //!
   TBranch        *b_mu_muid_author;   //!
   TBranch        *b_mu_muid_beta;   //!
   TBranch        *b_mu_muid_isMuonLikelihood;   //!
   TBranch        *b_mu_muid_matchchi2;   //!
   TBranch        *b_mu_muid_matchndof;   //!
   TBranch        *b_mu_muid_etcone20;   //!
   TBranch        *b_mu_muid_etcone30;   //!
   TBranch        *b_mu_muid_etcone40;   //!
   TBranch        *b_mu_muid_nucone20;   //!
   TBranch        *b_mu_muid_nucone30;   //!
   TBranch        *b_mu_muid_nucone40;   //!
   TBranch        *b_mu_muid_ptcone20;   //!
   TBranch        *b_mu_muid_ptcone30;   //!
   TBranch        *b_mu_muid_ptcone40;   //!
   TBranch        *b_mu_muid_energyLossPar;   //!
   TBranch        *b_mu_muid_energyLossErr;   //!
   TBranch        *b_mu_muid_etCore;   //!
   TBranch        *b_mu_muid_energyLossType;   //!
   TBranch        *b_mu_muid_caloMuonIdTag;   //!
   TBranch        *b_mu_muid_caloLRLikelihood;   //!
   TBranch        *b_mu_muid_bestMatch;   //!
   TBranch        *b_mu_muid_isStandAloneMuon;   //!
   TBranch        *b_mu_muid_isCombinedMuon;   //!
   TBranch        *b_mu_muid_isLowPtReconstructedMuon;   //!
   TBranch        *b_mu_muid_loose;   //!
   TBranch        *b_mu_muid_medium;   //!
   TBranch        *b_mu_muid_tight;   //!
   TBranch        *b_mu_muid_d0_exPV;   //!
   TBranch        *b_mu_muid_z0_exPV;   //!
   TBranch        *b_mu_muid_phi_exPV;   //!
   TBranch        *b_mu_muid_theta_exPV;   //!
   TBranch        *b_mu_muid_qoverp_exPV;   //!
   TBranch        *b_mu_muid_cb_d0_exPV;   //!
   TBranch        *b_mu_muid_cb_z0_exPV;   //!
   TBranch        *b_mu_muid_cb_phi_exPV;   //!
   TBranch        *b_mu_muid_cb_theta_exPV;   //!
   TBranch        *b_mu_muid_cb_qoverp_exPV;   //!
   TBranch        *b_mu_muid_id_d0_exPV;   //!
   TBranch        *b_mu_muid_id_z0_exPV;   //!
   TBranch        *b_mu_muid_id_phi_exPV;   //!
   TBranch        *b_mu_muid_id_theta_exPV;   //!
   TBranch        *b_mu_muid_id_qoverp_exPV;   //!
   TBranch        *b_mu_muid_me_d0_exPV;   //!
   TBranch        *b_mu_muid_me_z0_exPV;   //!
   TBranch        *b_mu_muid_me_phi_exPV;   //!
   TBranch        *b_mu_muid_me_theta_exPV;   //!
   TBranch        *b_mu_muid_me_qoverp_exPV;   //!
   TBranch        *b_mu_muid_ie_d0_exPV;   //!
   TBranch        *b_mu_muid_ie_z0_exPV;   //!
   TBranch        *b_mu_muid_ie_phi_exPV;   //!
   TBranch        *b_mu_muid_ie_theta_exPV;   //!
   TBranch        *b_mu_muid_ie_qoverp_exPV;   //!
   TBranch        *b_mu_muid_SpaceTime_detID;   //!
   TBranch        *b_mu_muid_SpaceTime_t;   //!
   TBranch        *b_mu_muid_SpaceTime_tError;   //!
   TBranch        *b_mu_muid_SpaceTime_weight;   //!
   TBranch        *b_mu_muid_SpaceTime_x;   //!
   TBranch        *b_mu_muid_SpaceTime_y;   //!
   TBranch        *b_mu_muid_SpaceTime_z;   //!
   TBranch        *b_mu_muid_SpaceTime_t_Tile;   //!
   TBranch        *b_mu_muid_SpaceTime_tError_Tile;   //!
   TBranch        *b_mu_muid_SpaceTime_weight_Tile;   //!
   TBranch        *b_mu_muid_SpaceTime_x_Tile;   //!
   TBranch        *b_mu_muid_SpaceTime_y_Tile;   //!
   TBranch        *b_mu_muid_SpaceTime_z_Tile;   //!
   TBranch        *b_mu_muid_SpaceTime_t_TRT;   //!
   TBranch        *b_mu_muid_SpaceTime_tError_TRT;   //!
   TBranch        *b_mu_muid_SpaceTime_weight_TRT;   //!
   TBranch        *b_mu_muid_SpaceTime_x_TRT;   //!
   TBranch        *b_mu_muid_SpaceTime_y_TRT;   //!
   TBranch        *b_mu_muid_SpaceTime_z_TRT;   //!
   TBranch        *b_mu_muid_SpaceTime_t_MDT;   //!
   TBranch        *b_mu_muid_SpaceTime_tError_MDT;   //!
   TBranch        *b_mu_muid_SpaceTime_weight_MDT;   //!
   TBranch        *b_mu_muid_SpaceTime_x_MDT;   //!
   TBranch        *b_mu_muid_SpaceTime_y_MDT;   //!
   TBranch        *b_mu_muid_SpaceTime_z_MDT;   //!
   TBranch        *b_mu_muid_TileCellEnergyLayer1;   //!
   TBranch        *b_mu_muid_TileTimeLayer1;   //!
   TBranch        *b_mu_muid_TileCellRmsNoiseLayer1;   //!
   TBranch        *b_mu_muid_TileCellSignLayer1;   //!
   TBranch        *b_mu_muid_TileCellEnergyLayer2;   //!
   TBranch        *b_mu_muid_TileTimeLayer2;   //!
   TBranch        *b_mu_muid_TileCellRmsNoiseLayer2;   //!
   TBranch        *b_mu_muid_TileCellSignLayer2;   //!
   TBranch        *b_mu_muid_TileCellEnergyLayer3;   //!
   TBranch        *b_mu_muid_TileTimeLayer3;   //!
   TBranch        *b_mu_muid_TileCellRmsNoiseLayer3;   //!
   TBranch        *b_mu_muid_TileCellSignLayer3;   //!
   TBranch        *b_mu_muid_MSTrkT0_1;   //!
   TBranch        *b_mu_muid_MSTrkT0_2;   //!
   TBranch        *b_mu_muid_MSTrkT0_3;   //!
   TBranch        *b_mu_muid_cov_d0_exPV;   //!
   TBranch        *b_mu_muid_cov_z0_exPV;   //!
   TBranch        *b_mu_muid_cov_phi_exPV;   //!
   TBranch        *b_mu_muid_cov_theta_exPV;   //!
   TBranch        *b_mu_muid_cov_qoverp_exPV;   //!
   TBranch        *b_mu_muid_cov_d0_z0_exPV;   //!
   TBranch        *b_mu_muid_cov_d0_phi_exPV;   //!
   TBranch        *b_mu_muid_cov_d0_theta_exPV;   //!
   TBranch        *b_mu_muid_cov_d0_qoverp_exPV;   //!
   TBranch        *b_mu_muid_cov_z0_phi_exPV;   //!
   TBranch        *b_mu_muid_cov_z0_theta_exPV;   //!
   TBranch        *b_mu_muid_cov_z0_qoverp_exPV;   //!
   TBranch        *b_mu_muid_cov_phi_theta_exPV;   //!
   TBranch        *b_mu_muid_cov_phi_qoverp_exPV;   //!
   TBranch        *b_mu_muid_cov_theta_qoverp_exPV;   //!
   TBranch        *b_mu_muid_id_cov_d0_exPV;   //!
   TBranch        *b_mu_muid_id_cov_z0_exPV;   //!
   TBranch        *b_mu_muid_id_cov_phi_exPV;   //!
   TBranch        *b_mu_muid_id_cov_theta_exPV;   //!
   TBranch        *b_mu_muid_id_cov_qoverp_exPV;   //!
   TBranch        *b_mu_muid_id_cov_d0_z0_exPV;   //!
   TBranch        *b_mu_muid_id_cov_d0_phi_exPV;   //!
   TBranch        *b_mu_muid_id_cov_d0_theta_exPV;   //!
   TBranch        *b_mu_muid_id_cov_d0_qoverp_exPV;   //!
   TBranch        *b_mu_muid_id_cov_z0_phi_exPV;   //!
   TBranch        *b_mu_muid_id_cov_z0_theta_exPV;   //!
   TBranch        *b_mu_muid_id_cov_z0_qoverp_exPV;   //!
   TBranch        *b_mu_muid_id_cov_phi_theta_exPV;   //!
   TBranch        *b_mu_muid_id_cov_phi_qoverp_exPV;   //!
   TBranch        *b_mu_muid_id_cov_theta_qoverp_exPV;   //!
   TBranch        *b_mu_muid_me_cov_d0_exPV;   //!
   TBranch        *b_mu_muid_me_cov_z0_exPV;   //!
   TBranch        *b_mu_muid_me_cov_phi_exPV;   //!
   TBranch        *b_mu_muid_me_cov_theta_exPV;   //!
   TBranch        *b_mu_muid_me_cov_qoverp_exPV;   //!
   TBranch        *b_mu_muid_me_cov_d0_z0_exPV;   //!
   TBranch        *b_mu_muid_me_cov_d0_phi_exPV;   //!
   TBranch        *b_mu_muid_me_cov_d0_theta_exPV;   //!
   TBranch        *b_mu_muid_me_cov_d0_qoverp_exPV;   //!
   TBranch        *b_mu_muid_me_cov_z0_phi_exPV;   //!
   TBranch        *b_mu_muid_me_cov_z0_theta_exPV;   //!
   TBranch        *b_mu_muid_me_cov_z0_qoverp_exPV;   //!
   TBranch        *b_mu_muid_me_cov_phi_theta_exPV;   //!
   TBranch        *b_mu_muid_me_cov_phi_qoverp_exPV;   //!
   TBranch        *b_mu_muid_me_cov_theta_qoverp_exPV;   //!
   TBranch        *b_mu_muid_ms_d0;   //!
   TBranch        *b_mu_muid_ms_z0;   //!
   TBranch        *b_mu_muid_ms_phi;   //!
   TBranch        *b_mu_muid_ms_theta;   //!
   TBranch        *b_mu_muid_ms_qoverp;   //!
   TBranch        *b_mu_muid_id_d0;   //!
   TBranch        *b_mu_muid_id_z0;   //!
   TBranch        *b_mu_muid_id_phi;   //!
   TBranch        *b_mu_muid_id_theta;   //!
   TBranch        *b_mu_muid_id_qoverp;   //!
   TBranch        *b_mu_muid_me_d0;   //!
   TBranch        *b_mu_muid_me_z0;   //!
   TBranch        *b_mu_muid_me_phi;   //!
   TBranch        *b_mu_muid_me_theta;   //!
   TBranch        *b_mu_muid_me_qoverp;   //!
   TBranch        *b_mu_muid_ie_d0;   //!
   TBranch        *b_mu_muid_ie_z0;   //!
   TBranch        *b_mu_muid_ie_phi;   //!
   TBranch        *b_mu_muid_ie_theta;   //!
   TBranch        *b_mu_muid_ie_qoverp;   //!
   TBranch        *b_mu_muid_nOutliersOnTrack;   //!
   TBranch        *b_mu_muid_nBLHits;   //!
   TBranch        *b_mu_muid_nPixHits;   //!
   TBranch        *b_mu_muid_nSCTHits;   //!
   TBranch        *b_mu_muid_nTRTHits;   //!
   TBranch        *b_mu_muid_nTRTHighTHits;   //!
   TBranch        *b_mu_muid_nBLSharedHits;   //!
   TBranch        *b_mu_muid_nPixSharedHits;   //!
   TBranch        *b_mu_muid_nPixHoles;   //!
   TBranch        *b_mu_muid_nSCTSharedHits;   //!
   TBranch        *b_mu_muid_nSCTHoles;   //!
   TBranch        *b_mu_muid_nTRTOutliers;   //!
   TBranch        *b_mu_muid_nTRTHighTOutliers;   //!
   TBranch        *b_mu_muid_nGangedPixels;   //!
   TBranch        *b_mu_muid_nPixelDeadSensors;   //!
   TBranch        *b_mu_muid_nSCTDeadSensors;   //!
   TBranch        *b_mu_muid_nTRTDeadStraws;   //!
   TBranch        *b_mu_muid_expectBLayerHit;   //!
   TBranch        *b_mu_muid_nMDTHits;   //!
   TBranch        *b_mu_muid_nMDTHoles;   //!
   TBranch        *b_mu_muid_nCSCEtaHits;   //!
   TBranch        *b_mu_muid_nCSCEtaHoles;   //!
   TBranch        *b_mu_muid_nCSCPhiHits;   //!
   TBranch        *b_mu_muid_nCSCPhiHoles;   //!
   TBranch        *b_mu_muid_nRPCEtaHits;   //!
   TBranch        *b_mu_muid_nRPCEtaHoles;   //!
   TBranch        *b_mu_muid_nRPCPhiHits;   //!
   TBranch        *b_mu_muid_nRPCPhiHoles;   //!
   TBranch        *b_mu_muid_nTGCEtaHits;   //!
   TBranch        *b_mu_muid_nTGCEtaHoles;   //!
   TBranch        *b_mu_muid_nTGCPhiHits;   //!
   TBranch        *b_mu_muid_nTGCPhiHoles;   //!
   TBranch        *b_mu_muid_nMDTBIHits;   //!
   TBranch        *b_mu_muid_nMDTBMHits;   //!
   TBranch        *b_mu_muid_nMDTBOHits;   //!
   TBranch        *b_mu_muid_nMDTBEEHits;   //!
   TBranch        *b_mu_muid_nMDTBIS78Hits;   //!
   TBranch        *b_mu_muid_nMDTEIHits;   //!
   TBranch        *b_mu_muid_nMDTEMHits;   //!
   TBranch        *b_mu_muid_nMDTEOHits;   //!
   TBranch        *b_mu_muid_nMDTEEHits;   //!
   TBranch        *b_mu_muid_nRPCLayer1EtaHits;   //!
   TBranch        *b_mu_muid_nRPCLayer2EtaHits;   //!
   TBranch        *b_mu_muid_nRPCLayer3EtaHits;   //!
   TBranch        *b_mu_muid_nRPCLayer1PhiHits;   //!
   TBranch        *b_mu_muid_nRPCLayer2PhiHits;   //!
   TBranch        *b_mu_muid_nRPCLayer3PhiHits;   //!
   TBranch        *b_mu_muid_nTGCLayer1EtaHits;   //!
   TBranch        *b_mu_muid_nTGCLayer2EtaHits;   //!
   TBranch        *b_mu_muid_nTGCLayer3EtaHits;   //!
   TBranch        *b_mu_muid_nTGCLayer4EtaHits;   //!
   TBranch        *b_mu_muid_nTGCLayer1PhiHits;   //!
   TBranch        *b_mu_muid_nTGCLayer2PhiHits;   //!
   TBranch        *b_mu_muid_nTGCLayer3PhiHits;   //!
   TBranch        *b_mu_muid_nTGCLayer4PhiHits;   //!
   TBranch        *b_mu_muid_barrelSectors;   //!
   TBranch        *b_mu_muid_endcapSectors;   //!
   TBranch        *b_mu_muid_trackd0;   //!
   TBranch        *b_mu_muid_trackz0;   //!
   TBranch        *b_mu_muid_trackphi;   //!
   TBranch        *b_mu_muid_tracktheta;   //!
   TBranch        *b_mu_muid_trackqoverp;   //!
   TBranch        *b_mu_muid_trackcov_d0;   //!
   TBranch        *b_mu_muid_trackcov_z0;   //!
   TBranch        *b_mu_muid_trackcov_phi;   //!
   TBranch        *b_mu_muid_trackcov_theta;   //!
   TBranch        *b_mu_muid_trackcov_qoverp;   //!
   TBranch        *b_mu_muid_trackcov_d0_z0;   //!
   TBranch        *b_mu_muid_trackcov_d0_phi;   //!
   TBranch        *b_mu_muid_trackcov_d0_theta;   //!
   TBranch        *b_mu_muid_trackcov_d0_qoverp;   //!
   TBranch        *b_mu_muid_trackcov_z0_phi;   //!
   TBranch        *b_mu_muid_trackcov_z0_theta;   //!
   TBranch        *b_mu_muid_trackcov_z0_qoverp;   //!
   TBranch        *b_mu_muid_trackcov_phi_theta;   //!
   TBranch        *b_mu_muid_trackcov_phi_qoverp;   //!
   TBranch        *b_mu_muid_trackcov_theta_qoverp;   //!
   TBranch        *b_mu_muid_trackfitchi2;   //!
   TBranch        *b_mu_muid_trackfitndof;   //!
   TBranch        *b_mu_muid_hastrack;   //!
   TBranch        *b_mu_muid_truth_dr;   //!
   TBranch        *b_mu_muid_truth_E;   //!
   TBranch        *b_mu_muid_truth_pt;   //!
   TBranch        *b_mu_muid_truth_eta;   //!
   TBranch        *b_mu_muid_truth_phi;   //!
   TBranch        *b_mu_muid_truth_type;   //!
   TBranch        *b_mu_muid_truth_status;   //!
   TBranch        *b_mu_muid_truth_barcode;   //!
   TBranch        *b_mu_muid_truth_mothertype;   //!
   TBranch        *b_mu_muid_truth_motherbarcode;   //!
   TBranch        *b_mu_muid_truth_matched;   //!
   TBranch        *b_mu_muid_EFCB_dr;   //!
   TBranch        *b_mu_muid_EFCB_index;   //!
   TBranch        *b_mu_muid_EFMG_dr;   //!
   TBranch        *b_mu_muid_EFMG_index;   //!
   TBranch        *b_mu_muid_EFME_dr;   //!
   TBranch        *b_mu_muid_EFME_index;   //!
   TBranch        *b_mu_muid_L2CB_dr;   //!
   TBranch        *b_mu_muid_L2CB_index;   //!
   TBranch        *b_mu_muid_L1_dr;   //!
   TBranch        *b_mu_muid_L1_index;   //!
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
   TBranch        *b_mu_staco_beta;   //!
   TBranch        *b_mu_staco_isMuonLikelihood;   //!
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
   TBranch        *b_mu_staco_SpaceTime_detID;   //!
   TBranch        *b_mu_staco_SpaceTime_t;   //!
   TBranch        *b_mu_staco_SpaceTime_tError;   //!
   TBranch        *b_mu_staco_SpaceTime_weight;   //!
   TBranch        *b_mu_staco_SpaceTime_x;   //!
   TBranch        *b_mu_staco_SpaceTime_y;   //!
   TBranch        *b_mu_staco_SpaceTime_z;   //!
   TBranch        *b_mu_staco_SpaceTime_t_Tile;   //!
   TBranch        *b_mu_staco_SpaceTime_tError_Tile;   //!
   TBranch        *b_mu_staco_SpaceTime_weight_Tile;   //!
   TBranch        *b_mu_staco_SpaceTime_x_Tile;   //!
   TBranch        *b_mu_staco_SpaceTime_y_Tile;   //!
   TBranch        *b_mu_staco_SpaceTime_z_Tile;   //!
   TBranch        *b_mu_staco_SpaceTime_t_TRT;   //!
   TBranch        *b_mu_staco_SpaceTime_tError_TRT;   //!
   TBranch        *b_mu_staco_SpaceTime_weight_TRT;   //!
   TBranch        *b_mu_staco_SpaceTime_x_TRT;   //!
   TBranch        *b_mu_staco_SpaceTime_y_TRT;   //!
   TBranch        *b_mu_staco_SpaceTime_z_TRT;   //!
   TBranch        *b_mu_staco_SpaceTime_t_MDT;   //!
   TBranch        *b_mu_staco_SpaceTime_tError_MDT;   //!
   TBranch        *b_mu_staco_SpaceTime_weight_MDT;   //!
   TBranch        *b_mu_staco_SpaceTime_x_MDT;   //!
   TBranch        *b_mu_staco_SpaceTime_y_MDT;   //!
   TBranch        *b_mu_staco_SpaceTime_z_MDT;   //!
   TBranch        *b_mu_staco_TileCellEnergyLayer1;   //!
   TBranch        *b_mu_staco_TileTimeLayer1;   //!
   TBranch        *b_mu_staco_TileCellRmsNoiseLayer1;   //!
   TBranch        *b_mu_staco_TileCellSignLayer1;   //!
   TBranch        *b_mu_staco_TileCellEnergyLayer2;   //!
   TBranch        *b_mu_staco_TileTimeLayer2;   //!
   TBranch        *b_mu_staco_TileCellRmsNoiseLayer2;   //!
   TBranch        *b_mu_staco_TileCellSignLayer2;   //!
   TBranch        *b_mu_staco_TileCellEnergyLayer3;   //!
   TBranch        *b_mu_staco_TileTimeLayer3;   //!
   TBranch        *b_mu_staco_TileCellRmsNoiseLayer3;   //!
   TBranch        *b_mu_staco_TileCellSignLayer3;   //!
   TBranch        *b_mu_staco_MSTrkT0_1;   //!
   TBranch        *b_mu_staco_MSTrkT0_2;   //!
   TBranch        *b_mu_staco_MSTrkT0_3;   //!
   TBranch        *b_mu_staco_cov_d0_exPV;   //!
   TBranch        *b_mu_staco_cov_z0_exPV;   //!
   TBranch        *b_mu_staco_cov_phi_exPV;   //!
   TBranch        *b_mu_staco_cov_theta_exPV;   //!
   TBranch        *b_mu_staco_cov_qoverp_exPV;   //!
   TBranch        *b_mu_staco_cov_d0_z0_exPV;   //!
   TBranch        *b_mu_staco_cov_d0_phi_exPV;   //!
   TBranch        *b_mu_staco_cov_d0_theta_exPV;   //!
   TBranch        *b_mu_staco_cov_d0_qoverp_exPV;   //!
   TBranch        *b_mu_staco_cov_z0_phi_exPV;   //!
   TBranch        *b_mu_staco_cov_z0_theta_exPV;   //!
   TBranch        *b_mu_staco_cov_z0_qoverp_exPV;   //!
   TBranch        *b_mu_staco_cov_phi_theta_exPV;   //!
   TBranch        *b_mu_staco_cov_phi_qoverp_exPV;   //!
   TBranch        *b_mu_staco_cov_theta_qoverp_exPV;   //!
   TBranch        *b_mu_staco_id_cov_d0_exPV;   //!
   TBranch        *b_mu_staco_id_cov_z0_exPV;   //!
   TBranch        *b_mu_staco_id_cov_phi_exPV;   //!
   TBranch        *b_mu_staco_id_cov_theta_exPV;   //!
   TBranch        *b_mu_staco_id_cov_qoverp_exPV;   //!
   TBranch        *b_mu_staco_id_cov_d0_z0_exPV;   //!
   TBranch        *b_mu_staco_id_cov_d0_phi_exPV;   //!
   TBranch        *b_mu_staco_id_cov_d0_theta_exPV;   //!
   TBranch        *b_mu_staco_id_cov_d0_qoverp_exPV;   //!
   TBranch        *b_mu_staco_id_cov_z0_phi_exPV;   //!
   TBranch        *b_mu_staco_id_cov_z0_theta_exPV;   //!
   TBranch        *b_mu_staco_id_cov_z0_qoverp_exPV;   //!
   TBranch        *b_mu_staco_id_cov_phi_theta_exPV;   //!
   TBranch        *b_mu_staco_id_cov_phi_qoverp_exPV;   //!
   TBranch        *b_mu_staco_id_cov_theta_qoverp_exPV;   //!
   TBranch        *b_mu_staco_me_cov_d0_exPV;   //!
   TBranch        *b_mu_staco_me_cov_z0_exPV;   //!
   TBranch        *b_mu_staco_me_cov_phi_exPV;   //!
   TBranch        *b_mu_staco_me_cov_theta_exPV;   //!
   TBranch        *b_mu_staco_me_cov_qoverp_exPV;   //!
   TBranch        *b_mu_staco_me_cov_d0_z0_exPV;   //!
   TBranch        *b_mu_staco_me_cov_d0_phi_exPV;   //!
   TBranch        *b_mu_staco_me_cov_d0_theta_exPV;   //!
   TBranch        *b_mu_staco_me_cov_d0_qoverp_exPV;   //!
   TBranch        *b_mu_staco_me_cov_z0_phi_exPV;   //!
   TBranch        *b_mu_staco_me_cov_z0_theta_exPV;   //!
   TBranch        *b_mu_staco_me_cov_z0_qoverp_exPV;   //!
   TBranch        *b_mu_staco_me_cov_phi_theta_exPV;   //!
   TBranch        *b_mu_staco_me_cov_phi_qoverp_exPV;   //!
   TBranch        *b_mu_staco_me_cov_theta_qoverp_exPV;   //!
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
   TBranch        *b_mu_staco_trackfitchi2;   //!
   TBranch        *b_mu_staco_trackfitndof;   //!
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
   TBranch        *b_mu_staco_EFMG_index;   //!
   TBranch        *b_mu_staco_EFME_dr;   //!
   TBranch        *b_mu_staco_EFME_index;   //!
   TBranch        *b_mu_staco_L2CB_dr;   //!
   TBranch        *b_mu_staco_L2CB_index;   //!
   TBranch        *b_mu_staco_L1_dr;   //!
   TBranch        *b_mu_staco_L1_index;   //!
   TBranch        *b_mu_calo_n;   //!
   TBranch        *b_mu_calo_E;   //!
   TBranch        *b_mu_calo_pt;   //!
   TBranch        *b_mu_calo_m;   //!
   TBranch        *b_mu_calo_eta;   //!
   TBranch        *b_mu_calo_phi;   //!
   TBranch        *b_mu_calo_px;   //!
   TBranch        *b_mu_calo_py;   //!
   TBranch        *b_mu_calo_pz;   //!
   TBranch        *b_mu_calo_charge;   //!
   TBranch        *b_mu_calo_allauthor;   //!
   TBranch        *b_mu_calo_hastrack;   //!
   TBranch        *b_mu_calo_truth_dr;   //!
   TBranch        *b_mu_calo_truth_E;   //!
   TBranch        *b_mu_calo_truth_pt;   //!
   TBranch        *b_mu_calo_truth_eta;   //!
   TBranch        *b_mu_calo_truth_phi;   //!
   TBranch        *b_mu_calo_truth_type;   //!
   TBranch        *b_mu_calo_truth_status;   //!
   TBranch        *b_mu_calo_truth_barcode;   //!
   TBranch        *b_mu_calo_truth_mothertype;   //!
   TBranch        *b_mu_calo_truth_motherbarcode;   //!
   TBranch        *b_mu_calo_truth_matched;   //!
   TBranch        *b_tau_n;   //!
   TBranch        *b_tau_Et;   //!
   TBranch        *b_tau_pt;   //!
   TBranch        *b_tau_m;   //!
   TBranch        *b_tau_eta;   //!
   TBranch        *b_tau_phi;   //!
   TBranch        *b_tau_charge;   //!
   TBranch        *b_tau_BDTEleScore;   //!
   TBranch        *b_tau_BDTJetScore;   //!
   TBranch        *b_tau_discCut;   //!
   TBranch        *b_tau_discCutTMVA;   //!
   TBranch        *b_tau_discLL;   //!
   TBranch        *b_tau_discNN;   //!
   TBranch        *b_tau_efficLL;   //!
   TBranch        *b_tau_efficNN;   //!
   TBranch        *b_tau_likelihood;   //!
   TBranch        *b_tau_tauJetNeuralNetwork;   //!
   TBranch        *b_tau_tauENeuralNetwork;   //!
   TBranch        *b_tau_tauElTauLikelihood;   //!
   TBranch        *b_tau_SafeLikelihood;   //!
   TBranch        *b_tau_electronVetoLoose;   //!
   TBranch        *b_tau_electronVetoMedium;   //!
   TBranch        *b_tau_electronVetoTight;   //!
   TBranch        *b_tau_muonVeto;   //!
   TBranch        *b_tau_tauCutLoose;   //!
   TBranch        *b_tau_tauCutMedium;   //!
   TBranch        *b_tau_tauCutTight;   //!
   TBranch        *b_tau_tauCutSafeLoose;   //!
   TBranch        *b_tau_tauCutSafeMedium;   //!
   TBranch        *b_tau_tauCutSafeTight;   //!
   TBranch        *b_tau_tauCutSafeCaloLoose;   //!
   TBranch        *b_tau_tauCutSafeCaloMedium;   //!
   TBranch        *b_tau_tauCutSafeCaloTight;   //!
   TBranch        *b_tau_tauLlhLoose;   //!
   TBranch        *b_tau_tauLlhMedium;   //!
   TBranch        *b_tau_tauLlhTight;   //!
   TBranch        *b_tau_JetBDTLoose;   //!
   TBranch        *b_tau_JetBDTMedium;   //!
   TBranch        *b_tau_JetBDTTight;   //!
   TBranch        *b_tau_EleBDTLoose;   //!
   TBranch        *b_tau_EleBDTMedium;   //!
   TBranch        *b_tau_EleBDTTight;   //!
   TBranch        *b_tau_SafeLlhLoose;   //!
   TBranch        *b_tau_SafeLlhMedium;   //!
   TBranch        *b_tau_SafeLlhTight;   //!
   TBranch        *b_tau_author;   //!
   TBranch        *b_tau_ROIword;   //!
   TBranch        *b_tau_nProng;   //!
   TBranch        *b_tau_numTrack;   //!
   TBranch        *b_tau_seedCalo_numTrack;   //!
   TBranch        *b_tau_etOverPtLeadTrk;   //!
   TBranch        *b_tau_ipZ0SinThetaSigLeadTrk;   //!
   TBranch        *b_tau_leadTrkPt;   //!
   TBranch        *b_tau_nLooseTrk;   //!
   TBranch        *b_tau_nLooseConvTrk;   //!
   TBranch        *b_tau_nProngLoose;   //!
   TBranch        *b_tau_ipSigLeadTrk;   //!
   TBranch        *b_tau_ipSigLeadLooseTrk;   //!
   TBranch        *b_tau_etOverPtLeadLooseTrk;   //!
   TBranch        *b_tau_leadLooseTrkPt;   //!
   TBranch        *b_tau_chrgLooseTrk;   //!
   TBranch        *b_tau_massTrkSys;   //!
   TBranch        *b_tau_trkWidth2;   //!
   TBranch        *b_tau_trFlightPathSig;   //!
   TBranch        *b_tau_etEflow;   //!
   TBranch        *b_tau_mEflow;   //!
   TBranch        *b_tau_nPi0;   //!
   TBranch        *b_tau_ele_E237E277;   //!
   TBranch        *b_tau_ele_PresamplerFraction;   //!
   TBranch        *b_tau_ele_ECALFirstFraction;   //!
   TBranch        *b_tau_seedCalo_EMRadius;   //!
   TBranch        *b_tau_seedCalo_hadRadius;   //!
   TBranch        *b_tau_seedCalo_etEMAtEMScale;   //!
   TBranch        *b_tau_seedCalo_etHadAtEMScale;   //!
   TBranch        *b_tau_seedCalo_isolFrac;   //!
   TBranch        *b_tau_seedCalo_centFrac;   //!
   TBranch        *b_tau_seedCalo_stripWidth2;   //!
   TBranch        *b_tau_seedCalo_nStrip;   //!
   TBranch        *b_tau_seedCalo_etEMCalib;   //!
   TBranch        *b_tau_seedCalo_etHadCalib;   //!
   TBranch        *b_tau_seedCalo_eta;   //!
   TBranch        *b_tau_seedCalo_phi;   //!
   TBranch        *b_tau_seedCalo_nIsolLooseTrk;   //!
   TBranch        *b_tau_seedCalo_trkAvgDist;   //!
   TBranch        *b_tau_seedCalo_trkRmsDist;   //!
   TBranch        *b_tau_numTopoClusters;   //!
   TBranch        *b_tau_numEffTopoClusters;   //!
   TBranch        *b_tau_topoInvMass;   //!
   TBranch        *b_tau_effTopoInvMass;   //!
   TBranch        *b_tau_topoMeanDeltaR;   //!
   TBranch        *b_tau_effTopoMeanDeltaR;   //!
   TBranch        *b_tau_numCells;   //!
   TBranch        *b_tau_seedTrk_EMRadius;   //!
   TBranch        *b_tau_seedTrk_isolFrac;   //!
   TBranch        *b_tau_seedTrk_etChrgHadOverSumTrkPt;   //!
   TBranch        *b_tau_seedTrk_isolFracWide;   //!
   TBranch        *b_tau_seedTrk_etHadAtEMScale;   //!
   TBranch        *b_tau_seedTrk_etEMAtEMScale;   //!
   TBranch        *b_tau_seedTrk_etEMCL;   //!
   TBranch        *b_tau_seedTrk_etChrgEM;   //!
   TBranch        *b_tau_seedTrk_etNeuEM;   //!
   TBranch        *b_tau_seedTrk_etResNeuEM;   //!
   TBranch        *b_tau_seedTrk_hadLeakEt;   //!
   TBranch        *b_tau_seedTrk_sumEMCellEtOverLeadTrkPt;   //!
   TBranch        *b_tau_seedTrk_secMaxStripEt;   //!
   TBranch        *b_tau_seedTrk_stripWidth2;   //!
   TBranch        *b_tau_seedTrk_nStrip;   //!
   TBranch        *b_tau_seedTrk_etChrgHad;   //!
   TBranch        *b_tau_seedTrk_nOtherCoreTrk;   //!
   TBranch        *b_tau_seedTrk_nIsolTrk;   //!
   TBranch        *b_tau_seedTrk_etIsolEM;   //!
   TBranch        *b_tau_seedTrk_etIsolHad;   //!
   TBranch        *b_tau_calcVars_sumTrkPt;   //!
   TBranch        *b_tau_calcVars_etHadSumPtTracks;   //!
   TBranch        *b_tau_calcVars_etEMSumPtTracks;   //!
   TBranch        *b_tau_calcVars_etHad_EMScale_Pt3Trks;   //!
   TBranch        *b_tau_calcVars_etEM_EMScale_Pt3Trks;   //!
   TBranch        *b_tau_calcVars_mass;   //!
   TBranch        *b_tau_calcVars_ipSigLeadLooseTrk;   //!
   TBranch        *b_tau_calcVars_drMax;   //!
   TBranch        *b_tau_calcVars_drMin;   //!
   TBranch        *b_tau_calcVars_emFracCalib;   //!
   TBranch        *b_tau_calcVars_TRTHTOverLT_LeadTrk;   //!
   TBranch        *b_tau_calcVars_calRadius;   //!
   TBranch        *b_tau_calcVars_EMFractionAtEMScale;   //!
   TBranch        *b_tau_calcVars_BDTLooseBkg;   //!
   TBranch        *b_tau_calcVars_BDTMediumBkg;   //!
   TBranch        *b_tau_calcVars_BDTTightBkg;   //!
   TBranch        *b_tau_seedCalo_track_n;   //!
   TBranch        *b_tau_seedCalo_wideTrk_n;   //!
   TBranch        *b_tau_otherTrk_n;   //!
   TBranch        *b_tau_EF_dr;   //!
   TBranch        *b_tau_EF_E;   //!
   TBranch        *b_tau_EF_Et;   //!
   TBranch        *b_tau_EF_pt;   //!
   TBranch        *b_tau_EF_eta;   //!
   TBranch        *b_tau_EF_phi;   //!
   TBranch        *b_tau_EF_matched;   //!
   TBranch        *b_tau_L2_dr;   //!
   TBranch        *b_tau_L2_E;   //!
   TBranch        *b_tau_L2_Et;   //!
   TBranch        *b_tau_L2_pt;   //!
   TBranch        *b_tau_L2_eta;   //!
   TBranch        *b_tau_L2_phi;   //!
   TBranch        *b_tau_L2_matched;   //!
   TBranch        *b_tau_L1_dr;   //!
   TBranch        *b_tau_L1_Et;   //!
   TBranch        *b_tau_L1_pt;   //!
   TBranch        *b_tau_L1_eta;   //!
   TBranch        *b_tau_L1_phi;   //!
   TBranch        *b_tau_L1_matched;   //!
   TBranch        *b_jet_AntiKt4TopoEM_n;   //!
   TBranch        *b_jet_AntiKt4TopoEM_E;   //!
   TBranch        *b_jet_AntiKt4TopoEM_pt;   //!
   TBranch        *b_jet_AntiKt4TopoEM_m;   //!
   TBranch        *b_jet_AntiKt4TopoEM_eta;   //!
   TBranch        *b_jet_AntiKt4TopoEM_phi;   //!
   TBranch        *b_jet_AntiKt4TopoEM_EtaOrigin;   //!
   TBranch        *b_jet_AntiKt4TopoEM_PhiOrigin;   //!
   TBranch        *b_jet_AntiKt4TopoEM_MOrigin;   //!
   TBranch        *b_jet_AntiKt4TopoEM_EtaOriginEM;   //!
   TBranch        *b_jet_AntiKt4TopoEM_PhiOriginEM;   //!
   TBranch        *b_jet_AntiKt4TopoEM_MOriginEM;   //!
   TBranch        *b_jet_AntiKt4TopoEM_WIDTH;   //!
   TBranch        *b_jet_AntiKt4TopoEM_n90;   //!
   TBranch        *b_jet_AntiKt4TopoEM_Timing;   //!
   TBranch        *b_jet_AntiKt4TopoEM_LArQuality;   //!
   TBranch        *b_jet_AntiKt4TopoEM_nTrk;   //!
   TBranch        *b_jet_AntiKt4TopoEM_sumPtTrk;   //!
   TBranch        *b_jet_AntiKt4TopoEM_OriginIndex;   //!
   TBranch        *b_jet_AntiKt4TopoEM_HECQuality;   //!
   TBranch        *b_jet_AntiKt4TopoEM_NegativeE;   //!
   TBranch        *b_jet_AntiKt4TopoEM_BCH_CORR_CELL;   //!
   TBranch        *b_jet_AntiKt4TopoEM_BCH_CORR_DOTX;   //!
   TBranch        *b_jet_AntiKt4TopoEM_BCH_CORR_JET;   //!
   TBranch        *b_jet_AntiKt4TopoEM_BCH_CORR_JET_FORCELL;   //!
   TBranch        *b_jet_AntiKt4TopoEM_ENG_BAD_CELLS;   //!
   TBranch        *b_jet_AntiKt4TopoEM_N_BAD_CELLS;   //!
   TBranch        *b_jet_AntiKt4TopoEM_N_BAD_CELLS_CORR;   //!
   TBranch        *b_jet_AntiKt4TopoEM_BAD_CELLS_CORR_E;   //!
   TBranch        *b_jet_AntiKt4TopoEM_SamplingMax;   //!
   TBranch        *b_jet_AntiKt4TopoEM_fracSamplingMax;   //!
   TBranch        *b_jet_AntiKt4TopoEM_hecf;   //!
   TBranch        *b_jet_AntiKt4TopoEM_tgap3f;   //!
   TBranch        *b_jet_AntiKt4TopoEM_isUgly;   //!
   TBranch        *b_jet_AntiKt4TopoEM_isBadLoose;   //!
   TBranch        *b_jet_AntiKt4TopoEM_isBadTight;   //!
   TBranch        *b_jet_AntiKt4TopoEM_emfrac;   //!
   TBranch        *b_jet_AntiKt4TopoEM_Offset;   //!
   TBranch        *b_jet_AntiKt4TopoEM_EMJES;   //!
   TBranch        *b_jet_AntiKt4TopoEM_EMJES_EtaCorr;   //!
   TBranch        *b_jet_AntiKt4TopoEM_EMJESnooffset;   //!
   TBranch        *b_jet_AntiKt4TopoEM_GCWJES;   //!
   TBranch        *b_jet_AntiKt4TopoEM_GCWJES_EtaCorr;   //!
   TBranch        *b_jet_AntiKt4TopoEM_CB;   //!
   TBranch        *b_jet_AntiKt4TopoEM_emscale_E;   //!
   TBranch        *b_jet_AntiKt4TopoEM_emscale_pt;   //!
   TBranch        *b_jet_AntiKt4TopoEM_emscale_m;   //!
   TBranch        *b_jet_AntiKt4TopoEM_emscale_eta;   //!
   TBranch        *b_jet_AntiKt4TopoEM_emscale_phi;   //!
   TBranch        *b_jet_AntiKt4TopoEM_jvtxf;   //!
   TBranch        *b_jet_AntiKt4TopoEM_jvtx_x;   //!
   TBranch        *b_jet_AntiKt4TopoEM_jvtx_y;   //!
   TBranch        *b_jet_AntiKt4TopoEM_jvtx_z;   //!
   TBranch        *b_jet_AntiKt4TopoEM_GSCFactorF;   //!
   TBranch        *b_jet_AntiKt4TopoEM_WidthFraction;   //!
   TBranch        *b_jet_AntiKt4TopoEM_flavor_component_ip2d_pu;   //!
   TBranch        *b_jet_AntiKt4TopoEM_flavor_component_ip2d_pb;   //!
   TBranch        *b_jet_AntiKt4TopoEM_flavor_component_ip2d_ntrk;   //!
   TBranch        *b_jet_AntiKt4TopoEM_flavor_component_ip3d_pu;   //!
   TBranch        *b_jet_AntiKt4TopoEM_flavor_component_ip3d_pb;   //!
   TBranch        *b_jet_AntiKt4TopoEM_flavor_component_ip3d_ntrk;   //!
   TBranch        *b_jet_AntiKt4TopoEM_flavor_component_sv1_pu;   //!
   TBranch        *b_jet_AntiKt4TopoEM_flavor_component_sv1_pb;   //!
   TBranch        *b_jet_AntiKt4TopoEM_flavor_component_sv2_pu;   //!
   TBranch        *b_jet_AntiKt4TopoEM_flavor_component_sv2_pb;   //!
   TBranch        *b_jet_AntiKt4TopoEM_flavor_component_svp_mass;   //!
   TBranch        *b_jet_AntiKt4TopoEM_flavor_component_svp_n2t;   //!
   TBranch        *b_jet_AntiKt4TopoEM_flavor_component_svp_svok;   //!
   TBranch        *b_jet_AntiKt4TopoEM_flavor_component_svp_ntrk;   //!
   TBranch        *b_jet_AntiKt4TopoEM_flavor_component_svp_ntrkv;   //!
   TBranch        *b_jet_AntiKt4TopoEM_flavor_component_svp_ntrkj;   //!
   TBranch        *b_jet_AntiKt4TopoEM_flavor_component_svp_efrc;   //!
   TBranch        *b_jet_AntiKt4TopoEM_flavor_component_sv0p_mass;   //!
   TBranch        *b_jet_AntiKt4TopoEM_flavor_component_sv0p_n2t;   //!
   TBranch        *b_jet_AntiKt4TopoEM_flavor_component_sv0p_svok;   //!
   TBranch        *b_jet_AntiKt4TopoEM_flavor_component_sv0p_ntrk;   //!
   TBranch        *b_jet_AntiKt4TopoEM_flavor_component_sv0p_ntrkv;   //!
   TBranch        *b_jet_AntiKt4TopoEM_flavor_component_sv0p_ntrkj;   //!
   TBranch        *b_jet_AntiKt4TopoEM_flavor_component_sv0p_efrc;   //!
   TBranch        *b_jet_AntiKt4TopoEM_flavor_component_jfit_pu;   //!
   TBranch        *b_jet_AntiKt4TopoEM_flavor_component_jfit_pb;   //!
   TBranch        *b_jet_AntiKt4TopoEM_flavor_component_jfit_nvtxnt;   //!
   TBranch        *b_jet_AntiKt4TopoEM_flavor_component_jfit_nvtx1t;   //!
   TBranch        *b_jet_AntiKt4TopoEM_flavor_component_jfit_ntrk;   //!
   TBranch        *b_jet_AntiKt4TopoEM_flavor_component_jfit_efrc;   //!
   TBranch        *b_jet_AntiKt4TopoEM_flavor_component_jfit_mass;   //!
   TBranch        *b_jet_AntiKt4TopoEM_flavor_component_jfit_sig3d;   //!
   TBranch        *b_jet_AntiKt4TopoEM_flavor_weight_TrackCounting2D;   //!
   TBranch        *b_jet_AntiKt4TopoEM_flavor_weight_JetProb;   //!
   TBranch        *b_jet_AntiKt4TopoEM_flavor_weight_IP1D;   //!
   TBranch        *b_jet_AntiKt4TopoEM_flavor_weight_IP2D;   //!
   TBranch        *b_jet_AntiKt4TopoEM_flavor_weight_IP3D;   //!
   TBranch        *b_jet_AntiKt4TopoEM_flavor_weight_SV0;   //!
   TBranch        *b_jet_AntiKt4TopoEM_flavor_weight_SV1;   //!
   TBranch        *b_jet_AntiKt4TopoEM_flavor_weight_SV2;   //!
   TBranch        *b_jet_AntiKt4TopoEM_flavor_weight_JetFitterTag;   //!
   TBranch        *b_jet_AntiKt4TopoEM_flavor_weight_JetFitterCOMB;   //!
   TBranch        *b_jet_AntiKt4TopoEM_flavor_weight_JetFitterTagNN;   //!
   TBranch        *b_jet_AntiKt4TopoEM_flavor_weight_JetFitterCOMBNN;   //!
   TBranch        *b_jet_AntiKt4TopoEM_flavor_weight_SoftMuonTag;   //!
   TBranch        *b_jet_AntiKt4TopoEM_flavor_weight_SoftElectronTag;   //!
   TBranch        *b_jet_AntiKt4TopoEM_flavor_truth_dRminToB;   //!
   TBranch        *b_jet_AntiKt4TopoEM_flavor_truth_dRminToC;   //!
   TBranch        *b_jet_AntiKt4TopoEM_flavor_truth_dRminToT;   //!
   TBranch        *b_jet_AntiKt4TopoEM_flavor_truth_BHadronpdg;   //!
   TBranch        *b_jet_AntiKt4TopoEM_flavor_truth_trueflav;   //!
   TBranch        *b_jet_AntiKt4TopoEM_el_dr;   //!
   TBranch        *b_jet_AntiKt4TopoEM_el_matched;   //!
   TBranch        *b_jet_AntiKt4TopoEM_mu_dr;   //!
   TBranch        *b_jet_AntiKt4TopoEM_mu_matched;   //!
   TBranch        *b_jet_AntiKt4TopoEM_L1_dr;   //!
   TBranch        *b_jet_AntiKt4TopoEM_L1_matched;   //!
   TBranch        *b_jet_AntiKt4TopoEM_L2_dr;   //!
   TBranch        *b_jet_AntiKt4TopoEM_L2_matched;   //!
   TBranch        *b_jet_AntiKt4TopoEM_EF_dr;   //!
   TBranch        *b_jet_AntiKt4TopoEM_EF_matched;   //!
   TBranch        *b_jet_akt6topoem_n;   //!
   TBranch        *b_jet_akt6topoem_E;   //!
   TBranch        *b_jet_akt6topoem_pt;   //!
   TBranch        *b_jet_akt6topoem_m;   //!
   TBranch        *b_jet_akt6topoem_eta;   //!
   TBranch        *b_jet_akt6topoem_phi;   //!
   TBranch        *b_jet_akt6topoem_EtaOrigin;   //!
   TBranch        *b_jet_akt6topoem_PhiOrigin;   //!
   TBranch        *b_jet_akt6topoem_MOrigin;   //!
   TBranch        *b_jet_akt6topoem_EtaOriginEM;   //!
   TBranch        *b_jet_akt6topoem_PhiOriginEM;   //!
   TBranch        *b_jet_akt6topoem_MOriginEM;   //!
   TBranch        *b_jet_akt6topoem_WIDTH;   //!
   TBranch        *b_jet_akt6topoem_n90;   //!
   TBranch        *b_jet_akt6topoem_Timing;   //!
   TBranch        *b_jet_akt6topoem_LArQuality;   //!
   TBranch        *b_jet_akt6topoem_nTrk;   //!
   TBranch        *b_jet_akt6topoem_sumPtTrk;   //!
   TBranch        *b_jet_akt6topoem_OriginIndex;   //!
   TBranch        *b_jet_akt6topoem_HECQuality;   //!
   TBranch        *b_jet_akt6topoem_NegativeE;   //!
   TBranch        *b_jet_akt6topoem_BCH_CORR_CELL;   //!
   TBranch        *b_jet_akt6topoem_BCH_CORR_DOTX;   //!
   TBranch        *b_jet_akt6topoem_BCH_CORR_JET;   //!
   TBranch        *b_jet_akt6topoem_BCH_CORR_JET_FORCELL;   //!
   TBranch        *b_jet_akt6topoem_ENG_BAD_CELLS;   //!
   TBranch        *b_jet_akt6topoem_N_BAD_CELLS;   //!
   TBranch        *b_jet_akt6topoem_N_BAD_CELLS_CORR;   //!
   TBranch        *b_jet_akt6topoem_BAD_CELLS_CORR_E;   //!
   TBranch        *b_jet_akt6topoem_SamplingMax;   //!
   TBranch        *b_jet_akt6topoem_fracSamplingMax;   //!
   TBranch        *b_jet_akt6topoem_hecf;   //!
   TBranch        *b_jet_akt6topoem_tgap3f;   //!
   TBranch        *b_jet_akt6topoem_isUgly;   //!
   TBranch        *b_jet_akt6topoem_isBadLoose;   //!
   TBranch        *b_jet_akt6topoem_isBadTight;   //!
   TBranch        *b_jet_akt6topoem_emfrac;   //!
   TBranch        *b_jet_akt6topoem_Offset;   //!
   TBranch        *b_jet_akt6topoem_EMJES;   //!
   TBranch        *b_jet_akt6topoem_EMJES_EtaCorr;   //!
   TBranch        *b_jet_akt6topoem_EMJESnooffset;   //!
   TBranch        *b_jet_akt6topoem_GCWJES;   //!
   TBranch        *b_jet_akt6topoem_GCWJES_EtaCorr;   //!
   TBranch        *b_jet_akt6topoem_CB;   //!
   TBranch        *b_jet_akt6topoem_emscale_E;   //!
   TBranch        *b_jet_akt6topoem_emscale_pt;   //!
   TBranch        *b_jet_akt6topoem_emscale_m;   //!
   TBranch        *b_jet_akt6topoem_emscale_eta;   //!
   TBranch        *b_jet_akt6topoem_emscale_phi;   //!
   TBranch        *b_jet_akt6topoem_jvtxf;   //!
   TBranch        *b_jet_akt6topoem_jvtx_x;   //!
   TBranch        *b_jet_akt6topoem_jvtx_y;   //!
   TBranch        *b_jet_akt6topoem_jvtx_z;   //!
   TBranch        *b_jet_akt6topoem_GSCFactorF;   //!
   TBranch        *b_jet_akt6topoem_WidthFraction;   //!
   TBranch        *b_jet_akt6topoem_flavor_component_ip2d_pu;   //!
   TBranch        *b_jet_akt6topoem_flavor_component_ip2d_pb;   //!
   TBranch        *b_jet_akt6topoem_flavor_component_ip2d_ntrk;   //!
   TBranch        *b_jet_akt6topoem_flavor_component_ip3d_pu;   //!
   TBranch        *b_jet_akt6topoem_flavor_component_ip3d_pb;   //!
   TBranch        *b_jet_akt6topoem_flavor_component_ip3d_ntrk;   //!
   TBranch        *b_jet_akt6topoem_flavor_component_sv1_pu;   //!
   TBranch        *b_jet_akt6topoem_flavor_component_sv1_pb;   //!
   TBranch        *b_jet_akt6topoem_flavor_component_sv2_pu;   //!
   TBranch        *b_jet_akt6topoem_flavor_component_sv2_pb;   //!
   TBranch        *b_jet_akt6topoem_flavor_component_svp_mass;   //!
   TBranch        *b_jet_akt6topoem_flavor_component_svp_n2t;   //!
   TBranch        *b_jet_akt6topoem_flavor_component_svp_svok;   //!
   TBranch        *b_jet_akt6topoem_flavor_component_svp_ntrk;   //!
   TBranch        *b_jet_akt6topoem_flavor_component_svp_ntrkv;   //!
   TBranch        *b_jet_akt6topoem_flavor_component_svp_ntrkj;   //!
   TBranch        *b_jet_akt6topoem_flavor_component_svp_efrc;   //!
   TBranch        *b_jet_akt6topoem_flavor_component_sv0p_mass;   //!
   TBranch        *b_jet_akt6topoem_flavor_component_sv0p_n2t;   //!
   TBranch        *b_jet_akt6topoem_flavor_component_sv0p_svok;   //!
   TBranch        *b_jet_akt6topoem_flavor_component_sv0p_ntrk;   //!
   TBranch        *b_jet_akt6topoem_flavor_component_sv0p_ntrkv;   //!
   TBranch        *b_jet_akt6topoem_flavor_component_sv0p_ntrkj;   //!
   TBranch        *b_jet_akt6topoem_flavor_component_sv0p_efrc;   //!
   TBranch        *b_jet_akt6topoem_flavor_component_jfit_pu;   //!
   TBranch        *b_jet_akt6topoem_flavor_component_jfit_pb;   //!
   TBranch        *b_jet_akt6topoem_flavor_component_jfit_nvtxnt;   //!
   TBranch        *b_jet_akt6topoem_flavor_component_jfit_nvtx1t;   //!
   TBranch        *b_jet_akt6topoem_flavor_component_jfit_ntrk;   //!
   TBranch        *b_jet_akt6topoem_flavor_component_jfit_efrc;   //!
   TBranch        *b_jet_akt6topoem_flavor_component_jfit_mass;   //!
   TBranch        *b_jet_akt6topoem_flavor_component_jfit_sig3d;   //!
   TBranch        *b_jet_akt6topoem_flavor_weight_TrackCounting2D;   //!
   TBranch        *b_jet_akt6topoem_flavor_weight_JetProb;   //!
   TBranch        *b_jet_akt6topoem_flavor_weight_IP1D;   //!
   TBranch        *b_jet_akt6topoem_flavor_weight_IP2D;   //!
   TBranch        *b_jet_akt6topoem_flavor_weight_IP3D;   //!
   TBranch        *b_jet_akt6topoem_flavor_weight_SV0;   //!
   TBranch        *b_jet_akt6topoem_flavor_weight_SV1;   //!
   TBranch        *b_jet_akt6topoem_flavor_weight_SV2;   //!
   TBranch        *b_jet_akt6topoem_flavor_weight_JetFitterTag;   //!
   TBranch        *b_jet_akt6topoem_flavor_weight_JetFitterCOMB;   //!
   TBranch        *b_jet_akt6topoem_flavor_weight_JetFitterTagNN;   //!
   TBranch        *b_jet_akt6topoem_flavor_weight_JetFitterCOMBNN;   //!
   TBranch        *b_jet_akt6topoem_flavor_weight_SoftMuonTag;   //!
   TBranch        *b_jet_akt6topoem_flavor_weight_SoftElectronTag;   //!
   TBranch        *b_jet_akt6topoem_flavor_truth_dRminToB;   //!
   TBranch        *b_jet_akt6topoem_flavor_truth_dRminToC;   //!
   TBranch        *b_jet_akt6topoem_flavor_truth_dRminToT;   //!
   TBranch        *b_jet_akt6topoem_flavor_truth_BHadronpdg;   //!
   TBranch        *b_jet_akt6topoem_flavor_truth_trueflav;   //!
   TBranch        *b_jet_akt6topoem_el_dr;   //!
   TBranch        *b_jet_akt6topoem_el_matched;   //!
   TBranch        *b_jet_akt6topoem_mu_dr;   //!
   TBranch        *b_jet_akt6topoem_mu_matched;   //!
   TBranch        *b_jet_akt6topoem_L1_dr;   //!
   TBranch        *b_jet_akt6topoem_L1_matched;   //!
   TBranch        *b_jet_akt6topoem_L2_dr;   //!
   TBranch        *b_jet_akt6topoem_L2_matched;   //!
   TBranch        *b_jet_akt6topoem_EF_dr;   //!
   TBranch        *b_jet_akt6topoem_EF_matched;   //!
   TBranch        *b_MET_RefFinal_phi;   //!
   TBranch        *b_MET_RefFinal_et;   //!
   TBranch        *b_MET_RefFinal_sumet;   //!
   TBranch        *b_MET_Cryo_phi;   //!
   TBranch        *b_MET_Cryo_et;   //!
   TBranch        *b_MET_Cryo_sumet;   //!
   TBranch        *b_MET_RefEle_phi;   //!
   TBranch        *b_MET_RefEle_et;   //!
   TBranch        *b_MET_RefEle_sumet;   //!
   TBranch        *b_MET_RefJet_phi;   //!
   TBranch        *b_MET_RefJet_et;   //!
   TBranch        *b_MET_RefJet_sumet;   //!
   TBranch        *b_MET_SoftJets_phi;   //!
   TBranch        *b_MET_SoftJets_et;   //!
   TBranch        *b_MET_SoftJets_sumet;   //!
   TBranch        *b_MET_RefMuon_phi;   //!
   TBranch        *b_MET_RefMuon_et;   //!
   TBranch        *b_MET_RefMuon_sumet;   //!
   TBranch        *b_MET_RefMuon_Staco_phi;   //!
   TBranch        *b_MET_RefMuon_Staco_et;   //!
   TBranch        *b_MET_RefMuon_Staco_sumet;   //!
   TBranch        *b_MET_RefMuon_Muid_phi;   //!
   TBranch        *b_MET_RefMuon_Muid_et;   //!
   TBranch        *b_MET_RefMuon_Muid_sumet;   //!
   TBranch        *b_MET_RefGamma_phi;   //!
   TBranch        *b_MET_RefGamma_et;   //!
   TBranch        *b_MET_RefGamma_sumet;   //!
   TBranch        *b_MET_RefTau_phi;   //!
   TBranch        *b_MET_RefTau_et;   //!
   TBranch        *b_MET_RefTau_sumet;   //!
   TBranch        *b_MET_CellOut_phi;   //!
   TBranch        *b_MET_CellOut_et;   //!
   TBranch        *b_MET_CellOut_sumet;   //!
   TBranch        *b_MET_Track_phi;   //!
   TBranch        *b_MET_Track_et;   //!
   TBranch        *b_MET_Track_sumet;   //!
   TBranch        *b_MET_RefFinal_em_etx;   //!
   TBranch        *b_MET_RefFinal_em_ety;   //!
   TBranch        *b_MET_RefFinal_em_phi;   //!
   TBranch        *b_MET_RefFinal_em_et;   //!
   TBranch        *b_MET_RefFinal_em_sumet;   //!
   TBranch        *b_MET_RefEle_em_phi;   //!
   TBranch        *b_MET_RefEle_em_et;   //!
   TBranch        *b_MET_RefEle_em_sumet;   //!
   TBranch        *b_MET_RefJet_em_phi;   //!
   TBranch        *b_MET_RefJet_em_et;   //!
   TBranch        *b_MET_RefJet_em_sumet;   //!
   TBranch        *b_MET_SoftJets_em_phi;   //!
   TBranch        *b_MET_SoftJets_em_et;   //!
   TBranch        *b_MET_SoftJets_em_sumet;   //!
   TBranch        *b_MET_RefMuon_em_phi;   //!
   TBranch        *b_MET_RefMuon_em_et;   //!
   TBranch        *b_MET_RefMuon_em_sumet;   //!
   TBranch        *b_MET_RefMuon_Track_em_phi;   //!
   TBranch        *b_MET_RefMuon_Track_em_et;   //!
   TBranch        *b_MET_RefMuon_Track_em_sumet;   //!
   TBranch        *b_MET_RefGamma_em_phi;   //!
   TBranch        *b_MET_RefGamma_em_et;   //!
   TBranch        *b_MET_RefGamma_em_sumet;   //!
   TBranch        *b_MET_RefTau_em_phi;   //!
   TBranch        *b_MET_RefTau_em_et;   //!
   TBranch        *b_MET_RefTau_em_sumet;   //!
   TBranch        *b_MET_CellOut_em_etx;   //!
   TBranch        *b_MET_CellOut_em_ety;   //!
   TBranch        *b_MET_CellOut_em_phi;   //!
   TBranch        *b_MET_CellOut_em_et;   //!
   TBranch        *b_MET_CellOut_em_sumet;   //!
   TBranch        *b_MET_Muon_Isol_Staco_phi;   //!
   TBranch        *b_MET_Muon_Isol_Staco_et;   //!
   TBranch        *b_MET_Muon_Isol_Staco_sumet;   //!
   TBranch        *b_MET_Muon_NonIsol_Staco_phi;   //!
   TBranch        *b_MET_Muon_NonIsol_Staco_et;   //!
   TBranch        *b_MET_Muon_NonIsol_Staco_sumet;   //!
   TBranch        *b_MET_Muon_Total_Staco_phi;   //!
   TBranch        *b_MET_Muon_Total_Staco_et;   //!
   TBranch        *b_MET_Muon_Total_Staco_sumet;   //!
   TBranch        *b_MET_Muon_Isol_Muid_phi;   //!
   TBranch        *b_MET_Muon_Isol_Muid_et;   //!
   TBranch        *b_MET_Muon_Isol_Muid_sumet;   //!
   TBranch        *b_MET_Muon_NonIsol_Muid_phi;   //!
   TBranch        *b_MET_Muon_NonIsol_Muid_et;   //!
   TBranch        *b_MET_Muon_NonIsol_Muid_sumet;   //!
   TBranch        *b_MET_Muon_Total_Muid_phi;   //!
   TBranch        *b_MET_Muon_Total_Muid_et;   //!
   TBranch        *b_MET_Muon_Total_Muid_sumet;   //!
   TBranch        *b_MET_MuonBoy_phi;   //!
   TBranch        *b_MET_MuonBoy_et;   //!
   TBranch        *b_MET_MuonBoy_sumet;   //!
   TBranch        *b_MET_RefMuon_Track_phi;   //!
   TBranch        *b_MET_RefMuon_Track_et;   //!
   TBranch        *b_MET_RefMuon_Track_sumet;   //!
   TBranch        *b_MET_RefMuon_Track_Staco_phi;   //!
   TBranch        *b_MET_RefMuon_Track_Staco_et;   //!
   TBranch        *b_MET_RefMuon_Track_Staco_sumet;   //!
   TBranch        *b_MET_RefMuon_Track_Muid_phi;   //!
   TBranch        *b_MET_RefMuon_Track_Muid_et;   //!
   TBranch        *b_MET_RefMuon_Track_Muid_sumet;   //!
   TBranch        *b_MET_CryoCone_phi;   //!
   TBranch        *b_MET_CryoCone_et;   //!
   TBranch        *b_MET_CryoCone_sumet;   //!
   TBranch        *b_MET_Final_phi;   //!
   TBranch        *b_MET_Final_et;   //!
   TBranch        *b_MET_Final_sumet;   //!
   TBranch        *b_MET_MuonBoy_Spectro_phi;   //!
   TBranch        *b_MET_MuonBoy_Spectro_et;   //!
   TBranch        *b_MET_MuonBoy_Spectro_sumet;   //!
   TBranch        *b_MET_MuonBoy_Track_phi;   //!
   TBranch        *b_MET_MuonBoy_Track_et;   //!
   TBranch        *b_MET_MuonBoy_Track_sumet;   //!
   TBranch        *b_MET_MuonMuid_phi;   //!
   TBranch        *b_MET_MuonMuid_et;   //!
   TBranch        *b_MET_MuonMuid_sumet;   //!
   TBranch        *b_MET_Muid_phi;   //!
   TBranch        *b_MET_Muid_et;   //!
   TBranch        *b_MET_Muid_sumet;   //!
   TBranch        *b_MET_Muid_Spectro_phi;   //!
   TBranch        *b_MET_Muid_Spectro_et;   //!
   TBranch        *b_MET_Muid_Spectro_sumet;   //!
   TBranch        *b_MET_Muid_Track_phi;   //!
   TBranch        *b_MET_Muid_Track_et;   //!
   TBranch        *b_MET_Muid_Track_sumet;   //!
   TBranch        *b_MET_Muon_phi;   //!
   TBranch        *b_MET_Muon_et;   //!
   TBranch        *b_MET_Muon_sumet;   //!
   TBranch        *b_MET_TopoObj_phi;   //!
   TBranch        *b_MET_TopoObj_et;   //!
   TBranch        *b_MET_TopoObj_sumet;   //!
   TBranch        *b_MET_LocHadTopoObj_phi;   //!
   TBranch        *b_MET_LocHadTopoObj_et;   //!
   TBranch        *b_MET_LocHadTopoObj_sumet;   //!
   TBranch        *b_MET_Topo_phi;   //!
   TBranch        *b_MET_Topo_et;   //!
   TBranch        *b_MET_Topo_sumet;   //!
   TBranch        *b_MET_Topo_SUMET_EMFrac;   //!
   TBranch        *b_MET_Topo_etx_PEMB;   //!
   TBranch        *b_MET_Topo_ety_PEMB;   //!
   TBranch        *b_MET_Topo_sumet_PEMB;   //!
   TBranch        *b_MET_Topo_phi_PEMB;   //!
   TBranch        *b_MET_Topo_etx_EMB;   //!
   TBranch        *b_MET_Topo_ety_EMB;   //!
   TBranch        *b_MET_Topo_sumet_EMB;   //!
   TBranch        *b_MET_Topo_phi_EMB;   //!
   TBranch        *b_MET_Topo_etx_PEMEC;   //!
   TBranch        *b_MET_Topo_ety_PEMEC;   //!
   TBranch        *b_MET_Topo_sumet_PEMEC;   //!
   TBranch        *b_MET_Topo_phi_PEMEC;   //!
   TBranch        *b_MET_Topo_etx_EME;   //!
   TBranch        *b_MET_Topo_ety_EME;   //!
   TBranch        *b_MET_Topo_sumet_EME;   //!
   TBranch        *b_MET_Topo_phi_EME;   //!
   TBranch        *b_MET_Topo_etx_TILE;   //!
   TBranch        *b_MET_Topo_ety_TILE;   //!
   TBranch        *b_MET_Topo_sumet_TILE;   //!
   TBranch        *b_MET_Topo_phi_TILE;   //!
   TBranch        *b_MET_Topo_etx_HEC;   //!
   TBranch        *b_MET_Topo_ety_HEC;   //!
   TBranch        *b_MET_Topo_sumet_HEC;   //!
   TBranch        *b_MET_Topo_phi_HEC;   //!
   TBranch        *b_MET_Topo_etx_FCAL;   //!
   TBranch        *b_MET_Topo_ety_FCAL;   //!
   TBranch        *b_MET_Topo_sumet_FCAL;   //!
   TBranch        *b_MET_Topo_phi_FCAL;   //!
   TBranch        *b_MET_Topo_nCell_PEMB;   //!
   TBranch        *b_MET_Topo_nCell_EMB;   //!
   TBranch        *b_MET_Topo_nCell_PEMEC;   //!
   TBranch        *b_MET_Topo_nCell_EME;   //!
   TBranch        *b_MET_Topo_nCell_TILE;   //!
   TBranch        *b_MET_Topo_nCell_HEC;   //!
   TBranch        *b_MET_Topo_nCell_FCAL;   //!
   TBranch        *b_MET_Topo_etx_CentralReg;   //!
   TBranch        *b_MET_Topo_ety_CentralReg;   //!
   TBranch        *b_MET_Topo_sumet_CentralReg;   //!
   TBranch        *b_MET_Topo_phi_CentralReg;   //!
   TBranch        *b_MET_Topo_etx_EndcapRegion;   //!
   TBranch        *b_MET_Topo_ety_EndcapRegion;   //!
   TBranch        *b_MET_Topo_sumet_EndcapRegion;   //!
   TBranch        *b_MET_Topo_phi_EndcapRegion;   //!
   TBranch        *b_MET_Topo_etx_ForwardReg;   //!
   TBranch        *b_MET_Topo_ety_ForwardReg;   //!
   TBranch        *b_MET_Topo_sumet_ForwardReg;   //!
   TBranch        *b_MET_Topo_phi_ForwardReg;   //!
   TBranch        *b_MET_CorrTopo_phi;   //!
   TBranch        *b_MET_CorrTopo_et;   //!
   TBranch        *b_MET_CorrTopo_sumet;   //!
   TBranch        *b_MET_CorrTopo_SUMET_EMFrac;   //!
   TBranch        *b_MET_CorrTopo_etx_PEMB;   //!
   TBranch        *b_MET_CorrTopo_ety_PEMB;   //!
   TBranch        *b_MET_CorrTopo_sumet_PEMB;   //!
   TBranch        *b_MET_CorrTopo_phi_PEMB;   //!
   TBranch        *b_MET_CorrTopo_etx_EMB;   //!
   TBranch        *b_MET_CorrTopo_ety_EMB;   //!
   TBranch        *b_MET_CorrTopo_sumet_EMB;   //!
   TBranch        *b_MET_CorrTopo_phi_EMB;   //!
   TBranch        *b_MET_CorrTopo_etx_PEMEC;   //!
   TBranch        *b_MET_CorrTopo_ety_PEMEC;   //!
   TBranch        *b_MET_CorrTopo_sumet_PEMEC;   //!
   TBranch        *b_MET_CorrTopo_phi_PEMEC;   //!
   TBranch        *b_MET_CorrTopo_etx_EME;   //!
   TBranch        *b_MET_CorrTopo_ety_EME;   //!
   TBranch        *b_MET_CorrTopo_sumet_EME;   //!
   TBranch        *b_MET_CorrTopo_phi_EME;   //!
   TBranch        *b_MET_CorrTopo_etx_TILE;   //!
   TBranch        *b_MET_CorrTopo_ety_TILE;   //!
   TBranch        *b_MET_CorrTopo_sumet_TILE;   //!
   TBranch        *b_MET_CorrTopo_phi_TILE;   //!
   TBranch        *b_MET_CorrTopo_etx_HEC;   //!
   TBranch        *b_MET_CorrTopo_ety_HEC;   //!
   TBranch        *b_MET_CorrTopo_sumet_HEC;   //!
   TBranch        *b_MET_CorrTopo_phi_HEC;   //!
   TBranch        *b_MET_CorrTopo_etx_FCAL;   //!
   TBranch        *b_MET_CorrTopo_ety_FCAL;   //!
   TBranch        *b_MET_CorrTopo_sumet_FCAL;   //!
   TBranch        *b_MET_CorrTopo_phi_FCAL;   //!
   TBranch        *b_MET_CorrTopo_nCell_PEMB;   //!
   TBranch        *b_MET_CorrTopo_nCell_EMB;   //!
   TBranch        *b_MET_CorrTopo_nCell_PEMEC;   //!
   TBranch        *b_MET_CorrTopo_nCell_EME;   //!
   TBranch        *b_MET_CorrTopo_nCell_TILE;   //!
   TBranch        *b_MET_CorrTopo_nCell_HEC;   //!
   TBranch        *b_MET_CorrTopo_nCell_FCAL;   //!
   TBranch        *b_MET_CorrTopo_etx_CentralReg;   //!
   TBranch        *b_MET_CorrTopo_ety_CentralReg;   //!
   TBranch        *b_MET_CorrTopo_sumet_CentralReg;   //!
   TBranch        *b_MET_CorrTopo_phi_CentralReg;   //!
   TBranch        *b_MET_CorrTopo_etx_EndcapRegion;   //!
   TBranch        *b_MET_CorrTopo_ety_EndcapRegion;   //!
   TBranch        *b_MET_CorrTopo_sumet_EndcapRegion;   //!
   TBranch        *b_MET_CorrTopo_phi_EndcapRegion;   //!
   TBranch        *b_MET_CorrTopo_etx_ForwardReg;   //!
   TBranch        *b_MET_CorrTopo_ety_ForwardReg;   //!
   TBranch        *b_MET_CorrTopo_sumet_ForwardReg;   //!
   TBranch        *b_MET_CorrTopo_phi_ForwardReg;   //!
   TBranch        *b_MET_LocHadTopo_phi;   //!
   TBranch        *b_MET_LocHadTopo_et;   //!
   TBranch        *b_MET_LocHadTopo_sumet;   //!
   TBranch        *b_MET_LocHadTopo_SUMET_EMFrac;   //!
   TBranch        *b_MET_LocHadTopo_etx_PEMB;   //!
   TBranch        *b_MET_LocHadTopo_ety_PEMB;   //!
   TBranch        *b_MET_LocHadTopo_sumet_PEMB;   //!
   TBranch        *b_MET_LocHadTopo_phi_PEMB;   //!
   TBranch        *b_MET_LocHadTopo_etx_EMB;   //!
   TBranch        *b_MET_LocHadTopo_ety_EMB;   //!
   TBranch        *b_MET_LocHadTopo_sumet_EMB;   //!
   TBranch        *b_MET_LocHadTopo_phi_EMB;   //!
   TBranch        *b_MET_LocHadTopo_etx_PEMEC;   //!
   TBranch        *b_MET_LocHadTopo_ety_PEMEC;   //!
   TBranch        *b_MET_LocHadTopo_sumet_PEMEC;   //!
   TBranch        *b_MET_LocHadTopo_phi_PEMEC;   //!
   TBranch        *b_MET_LocHadTopo_etx_EME;   //!
   TBranch        *b_MET_LocHadTopo_ety_EME;   //!
   TBranch        *b_MET_LocHadTopo_sumet_EME;   //!
   TBranch        *b_MET_LocHadTopo_phi_EME;   //!
   TBranch        *b_MET_LocHadTopo_etx_TILE;   //!
   TBranch        *b_MET_LocHadTopo_ety_TILE;   //!
   TBranch        *b_MET_LocHadTopo_sumet_TILE;   //!
   TBranch        *b_MET_LocHadTopo_phi_TILE;   //!
   TBranch        *b_MET_LocHadTopo_etx_HEC;   //!
   TBranch        *b_MET_LocHadTopo_ety_HEC;   //!
   TBranch        *b_MET_LocHadTopo_sumet_HEC;   //!
   TBranch        *b_MET_LocHadTopo_phi_HEC;   //!
   TBranch        *b_MET_LocHadTopo_etx_FCAL;   //!
   TBranch        *b_MET_LocHadTopo_ety_FCAL;   //!
   TBranch        *b_MET_LocHadTopo_sumet_FCAL;   //!
   TBranch        *b_MET_LocHadTopo_phi_FCAL;   //!
   TBranch        *b_MET_LocHadTopo_nCell_PEMB;   //!
   TBranch        *b_MET_LocHadTopo_nCell_EMB;   //!
   TBranch        *b_MET_LocHadTopo_nCell_PEMEC;   //!
   TBranch        *b_MET_LocHadTopo_nCell_EME;   //!
   TBranch        *b_MET_LocHadTopo_nCell_TILE;   //!
   TBranch        *b_MET_LocHadTopo_nCell_HEC;   //!
   TBranch        *b_MET_LocHadTopo_nCell_FCAL;   //!
   TBranch        *b_MET_LocHadTopo_etx_CentralReg;   //!
   TBranch        *b_MET_LocHadTopo_ety_CentralReg;   //!
   TBranch        *b_MET_LocHadTopo_sumet_CentralReg;   //!
   TBranch        *b_MET_LocHadTopo_phi_CentralReg;   //!
   TBranch        *b_MET_LocHadTopo_etx_EndcapRegion;   //!
   TBranch        *b_MET_LocHadTopo_ety_EndcapRegion;   //!
   TBranch        *b_MET_LocHadTopo_sumet_EndcapRegion;   //!
   TBranch        *b_MET_LocHadTopo_phi_EndcapRegion;   //!
   TBranch        *b_MET_LocHadTopo_etx_ForwardReg;   //!
   TBranch        *b_MET_LocHadTopo_ety_ForwardReg;   //!
   TBranch        *b_MET_LocHadTopo_sumet_ForwardReg;   //!
   TBranch        *b_MET_LocHadTopo_phi_ForwardReg;   //!
   TBranch        *b_MET_Calib_phi;   //!
   TBranch        *b_MET_Calib_et;   //!
   TBranch        *b_MET_Calib_sumet;   //!
   TBranch        *b_MET_Calib_SUMET_EMFrac;   //!
   TBranch        *b_MET_Calib_etx_PEMB;   //!
   TBranch        *b_MET_Calib_ety_PEMB;   //!
   TBranch        *b_MET_Calib_sumet_PEMB;   //!
   TBranch        *b_MET_Calib_phi_PEMB;   //!
   TBranch        *b_MET_Calib_etx_EMB;   //!
   TBranch        *b_MET_Calib_ety_EMB;   //!
   TBranch        *b_MET_Calib_sumet_EMB;   //!
   TBranch        *b_MET_Calib_phi_EMB;   //!
   TBranch        *b_MET_Calib_etx_PEMEC;   //!
   TBranch        *b_MET_Calib_ety_PEMEC;   //!
   TBranch        *b_MET_Calib_sumet_PEMEC;   //!
   TBranch        *b_MET_Calib_phi_PEMEC;   //!
   TBranch        *b_MET_Calib_etx_EME;   //!
   TBranch        *b_MET_Calib_ety_EME;   //!
   TBranch        *b_MET_Calib_sumet_EME;   //!
   TBranch        *b_MET_Calib_phi_EME;   //!
   TBranch        *b_MET_Calib_etx_TILE;   //!
   TBranch        *b_MET_Calib_ety_TILE;   //!
   TBranch        *b_MET_Calib_sumet_TILE;   //!
   TBranch        *b_MET_Calib_phi_TILE;   //!
   TBranch        *b_MET_Calib_etx_HEC;   //!
   TBranch        *b_MET_Calib_ety_HEC;   //!
   TBranch        *b_MET_Calib_sumet_HEC;   //!
   TBranch        *b_MET_Calib_phi_HEC;   //!
   TBranch        *b_MET_Calib_etx_FCAL;   //!
   TBranch        *b_MET_Calib_ety_FCAL;   //!
   TBranch        *b_MET_Calib_sumet_FCAL;   //!
   TBranch        *b_MET_Calib_phi_FCAL;   //!
   TBranch        *b_MET_Calib_nCell_PEMB;   //!
   TBranch        *b_MET_Calib_nCell_EMB;   //!
   TBranch        *b_MET_Calib_nCell_PEMEC;   //!
   TBranch        *b_MET_Calib_nCell_EME;   //!
   TBranch        *b_MET_Calib_nCell_TILE;   //!
   TBranch        *b_MET_Calib_nCell_HEC;   //!
   TBranch        *b_MET_Calib_nCell_FCAL;   //!
   TBranch        *b_MET_Calib_etx_CentralReg;   //!
   TBranch        *b_MET_Calib_ety_CentralReg;   //!
   TBranch        *b_MET_Calib_sumet_CentralReg;   //!
   TBranch        *b_MET_Calib_phi_CentralReg;   //!
   TBranch        *b_MET_Calib_etx_EndcapRegion;   //!
   TBranch        *b_MET_Calib_ety_EndcapRegion;   //!
   TBranch        *b_MET_Calib_sumet_EndcapRegion;   //!
   TBranch        *b_MET_Calib_phi_EndcapRegion;   //!
   TBranch        *b_MET_Calib_etx_ForwardReg;   //!
   TBranch        *b_MET_Calib_ety_ForwardReg;   //!
   TBranch        *b_MET_Calib_sumet_ForwardReg;   //!
   TBranch        *b_MET_Calib_phi_ForwardReg;   //!
   TBranch        *b_MET_Truth_NonInt_etx;   //!
   TBranch        *b_MET_Truth_NonInt_ety;   //!
   TBranch        *b_MET_Truth_NonInt_phi;   //!
   TBranch        *b_MET_Truth_NonInt_et;   //!
   TBranch        *b_MET_Truth_NonInt_sumet;   //!
   TBranch        *b_MET_Truth_Int_phi;   //!
   TBranch        *b_MET_Truth_IntCentral_phi;   //!
   TBranch        *b_MET_Truth_IntFwd_phi;   //!
   TBranch        *b_MET_Truth_IntOutCover_phi;   //!
   TBranch        *b_MET_Truth_IntMuons_phi;   //!
   TBranch        *b_MET_Truth_Int_et;   //!
   TBranch        *b_MET_Truth_IntCentral_et;   //!
   TBranch        *b_MET_Truth_IntFwd_et;   //!
   TBranch        *b_MET_Truth_IntOutCover_et;   //!
   TBranch        *b_MET_Truth_IntMuons_et;   //!
   TBranch        *b_MET_Truth_Int_sumet;   //!
   TBranch        *b_MET_Truth_IntCentral_sumet;   //!
   TBranch        *b_MET_Truth_IntFwd_sumet;   //!
   TBranch        *b_MET_Truth_IntOutCover_sumet;   //!
   TBranch        *b_MET_Truth_IntMuons_sumet;   //!
   TBranch        *b_MET_Truth_PileUp_NonInt_etx;   //!
   TBranch        *b_MET_Truth_PileUp_NonInt_ety;   //!
   TBranch        *b_MET_Truth_PileUp_NonInt_phi;   //!
   TBranch        *b_MET_Truth_PileUp_NonInt_et;   //!
   TBranch        *b_MET_Truth_PileUp_NonInt_sumet;   //!
   TBranch        *b_MET_Truth_PileUp_Int_etx;   //!
   TBranch        *b_MET_Truth_PileUp_Int_ety;   //!
   TBranch        *b_MET_Truth_PileUp_IntCentral_etx;   //!
   TBranch        *b_MET_Truth_PileUp_IntCentral_ety;   //!
   TBranch        *b_MET_Truth_PileUp_IntFwd_etx;   //!
   TBranch        *b_MET_Truth_PileUp_IntFwd_ety;   //!
   TBranch        *b_MET_Truth_PileUp_IntOutCover_etx;   //!
   TBranch        *b_MET_Truth_PileUp_IntOutCover_ety;   //!
   TBranch        *b_MET_Truth_PileUp_IntMuons_etx;   //!
   TBranch        *b_MET_Truth_PileUp_IntMuons_ety;   //!
   TBranch        *b_MET_Truth_PileUp_Int_phi;   //!
   TBranch        *b_MET_Truth_PileUp_IntCentral_phi;   //!
   TBranch        *b_MET_Truth_PileUp_IntFwd_phi;   //!
   TBranch        *b_MET_Truth_PileUp_IntOutCover_phi;   //!
   TBranch        *b_MET_Truth_PileUp_IntMuons_phi;   //!
   TBranch        *b_MET_Truth_PileUp_Int_et;   //!
   TBranch        *b_MET_Truth_PileUp_IntCentral_et;   //!
   TBranch        *b_MET_Truth_PileUp_IntFwd_et;   //!
   TBranch        *b_MET_Truth_PileUp_IntOutCover_et;   //!
   TBranch        *b_MET_Truth_PileUp_IntMuons_et;   //!
   TBranch        *b_MET_Truth_PileUp_Int_sumet;   //!
   TBranch        *b_MET_Truth_PileUp_IntCentral_sumet;   //!
   TBranch        *b_MET_Truth_PileUp_IntFwd_sumet;   //!
   TBranch        *b_MET_Truth_PileUp_IntOutCover_sumet;   //!
   TBranch        *b_MET_Truth_PileUp_IntMuons_sumet;   //!
   TBranch        *b_MET_DM_Crack1_etx;   //!
   TBranch        *b_MET_DM_Crack1_ety;   //!
   TBranch        *b_MET_DM_Crack1_phi;   //!
   TBranch        *b_MET_DM_Crack1_et;   //!
   TBranch        *b_MET_DM_Crack1_sumet;   //!
   TBranch        *b_MET_DM_Crack2_etx;   //!
   TBranch        *b_MET_DM_Crack2_ety;   //!
   TBranch        *b_MET_DM_Crack2_phi;   //!
   TBranch        *b_MET_DM_Crack2_et;   //!
   TBranch        *b_MET_DM_Crack2_sumet;   //!
   TBranch        *b_MET_DM_All_etx;   //!
   TBranch        *b_MET_DM_All_ety;   //!
   TBranch        *b_MET_DM_All_phi;   //!
   TBranch        *b_MET_DM_All_et;   //!
   TBranch        *b_MET_DM_All_sumet;   //!
   TBranch        *b_MET_DM_Cryo_etx;   //!
   TBranch        *b_MET_DM_Cryo_ety;   //!
   TBranch        *b_MET_DM_Cryo_phi;   //!
   TBranch        *b_MET_DM_Cryo_et;   //!
   TBranch        *b_MET_DM_Cryo_sumet;   //!
   TBranch        *b_METJetsInfo_JetPtWeightedEventEMFraction;   //!
   TBranch        *b_METJetsInfo_JetPtWeightedNumAssociatedTracks;   //!
   TBranch        *b_METJetsInfo_JetPtWeightedSize;   //!
   TBranch        *b_METJetsInfo_LeadingJetEt;   //!
   TBranch        *b_METJetsInfo_LeadingJetEta;   //!
   TBranch        *b_cl_n;   //!
   TBranch        *b_cl_pt;   //!
   TBranch        *b_cl_eta;   //!
   TBranch        *b_cl_phi;   //!
   TBranch        *b_clpt10_n;   //!
   TBranch        *b_clpt10_pt;   //!
   TBranch        *b_clpt10_eta;   //!
   TBranch        *b_clpt10_phi;   //!
   TBranch        *b_clpt10_E_em;   //!
   TBranch        *b_clpt10_E_had;   //!
   TBranch        *b_clpt10_firstEdens;   //!
   TBranch        *b_clpt10_cellmaxfrac;   //!
   TBranch        *b_clpt10_longitudinal;   //!
   TBranch        *b_clpt10_secondlambda;   //!
   TBranch        *b_clpt10_lateral;   //!
   TBranch        *b_clpt10_secondR;   //!
   TBranch        *b_clpt10_centerlambda;   //!
   TBranch        *b_clpt10_deltaTheta;   //!
   TBranch        *b_clpt10_deltaPhi;   //!
   TBranch        *b_clpt10_time;   //!
   TBranch        *b_clpt10_E_PreSamplerB;   //!
   TBranch        *b_clpt10_E_EMB1;   //!
   TBranch        *b_clpt10_E_EMB2;   //!
   TBranch        *b_clpt10_E_EMB3;   //!
   TBranch        *b_clpt10_E_PreSamplerE;   //!
   TBranch        *b_clpt10_E_EME1;   //!
   TBranch        *b_clpt10_E_EME2;   //!
   TBranch        *b_clpt10_E_EME3;   //!
   TBranch        *b_clpt10_E_HEC0;   //!
   TBranch        *b_clpt10_E_HEC1;   //!
   TBranch        *b_clpt10_E_HEC2;   //!
   TBranch        *b_clpt10_E_HEC3;   //!
   TBranch        *b_clpt10_E_TileBar0;   //!
   TBranch        *b_clpt10_E_TileBar1;   //!
   TBranch        *b_clpt10_E_TileBar2;   //!
   TBranch        *b_clpt10_E_TileGap1;   //!
   TBranch        *b_clpt10_E_TileGap2;   //!
   TBranch        *b_clpt10_E_TileGap3;   //!
   TBranch        *b_clpt10_E_TileExt0;   //!
   TBranch        *b_clpt10_E_TileExt1;   //!
   TBranch        *b_clpt10_E_TileExt2;   //!
   TBranch        *b_clpt10_E_FCAL0;   //!
   TBranch        *b_clpt10_E_FCAL1;   //!
   TBranch        *b_clpt10_E_FCAL2;   //!
   TBranch        *b_emclpt10_n;   //!
   TBranch        *b_emclpt10_pt;   //!
   TBranch        *b_emclpt10_eta;   //!
   TBranch        *b_emclpt10_phi;   //!
   TBranch        *b_emclpt10_E_em;   //!
   TBranch        *b_emclpt10_E_had;   //!
   TBranch        *b_emclpt10_firstEdens;   //!
   TBranch        *b_emclpt10_cellmaxfrac;   //!
   TBranch        *b_emclpt10_longitudinal;   //!
   TBranch        *b_emclpt10_secondlambda;   //!
   TBranch        *b_emclpt10_lateral;   //!
   TBranch        *b_emclpt10_secondR;   //!
   TBranch        *b_emclpt10_centerlambda;   //!
   TBranch        *b_emclpt10_deltaTheta;   //!
   TBranch        *b_emclpt10_deltaPhi;   //!
   TBranch        *b_emclpt10_time;   //!
   TBranch        *b_emclpt10_E_PreSamplerB;   //!
   TBranch        *b_emclpt10_E_EMB1;   //!
   TBranch        *b_emclpt10_E_EMB2;   //!
   TBranch        *b_emclpt10_E_EMB3;   //!
   TBranch        *b_emclpt10_E_PreSamplerE;   //!
   TBranch        *b_emclpt10_E_EME1;   //!
   TBranch        *b_emclpt10_E_EME2;   //!
   TBranch        *b_emclpt10_E_EME3;   //!
   TBranch        *b_emclpt10_E_HEC0;   //!
   TBranch        *b_emclpt10_E_HEC1;   //!
   TBranch        *b_emclpt10_E_HEC2;   //!
   TBranch        *b_emclpt10_E_HEC3;   //!
   TBranch        *b_emclpt10_E_TileBar0;   //!
   TBranch        *b_emclpt10_E_TileBar1;   //!
   TBranch        *b_emclpt10_E_TileBar2;   //!
   TBranch        *b_emclpt10_E_TileGap1;   //!
   TBranch        *b_emclpt10_E_TileGap2;   //!
   TBranch        *b_emclpt10_E_TileGap3;   //!
   TBranch        *b_emclpt10_E_TileExt0;   //!
   TBranch        *b_emclpt10_E_TileExt1;   //!
   TBranch        *b_emclpt10_E_TileExt2;   //!
   TBranch        *b_emclpt10_E_FCAL0;   //!
   TBranch        *b_emclpt10_E_FCAL1;   //!
   TBranch        *b_emclpt10_E_FCAL2;   //!
   TBranch        *b_emclpt10_eta_PreSamplerB;   //!
   TBranch        *b_emclpt10_phi_PreSamplerB;   //!
   TBranch        *b_emclpt10_eta_EMB1;   //!
   TBranch        *b_emclpt10_phi_EMB1;   //!
   TBranch        *b_emclpt10_eta_EMB2;   //!
   TBranch        *b_emclpt10_phi_EMB2;   //!
   TBranch        *b_emclpt10_eta_EMB3;   //!
   TBranch        *b_emclpt10_phi_EMB3;   //!
   TBranch        *b_emclpt10_eta_PreSamplerE;   //!
   TBranch        *b_emclpt10_phi_PreSamplerE;   //!
   TBranch        *b_emclpt10_eta_EME1;   //!
   TBranch        *b_emclpt10_phi_EME1;   //!
   TBranch        *b_emclpt10_eta_EME2;   //!
   TBranch        *b_emclpt10_phi_EME2;   //!
   TBranch        *b_emclpt10_eta_EME3;   //!
   TBranch        *b_emclpt10_phi_EME3;   //!
   TBranch        *b_emclpt10_eta_HEC0;   //!
   TBranch        *b_emclpt10_phi_HEC0;   //!
   TBranch        *b_emclpt10_eta_HEC1;   //!
   TBranch        *b_emclpt10_phi_HEC1;   //!
   TBranch        *b_emclpt10_eta_HEC2;   //!
   TBranch        *b_emclpt10_phi_HEC2;   //!
   TBranch        *b_emclpt10_eta_HEC3;   //!
   TBranch        *b_emclpt10_phi_HEC3;   //!
   TBranch        *b_emclpt10_eta_TileBar0;   //!
   TBranch        *b_emclpt10_phi_TileBar0;   //!
   TBranch        *b_emclpt10_eta_TileBar1;   //!
   TBranch        *b_emclpt10_phi_TileBar1;   //!
   TBranch        *b_emclpt10_eta_TileBar2;   //!
   TBranch        *b_emclpt10_phi_TileBar2;   //!
   TBranch        *b_emclpt10_eta_TileGap1;   //!
   TBranch        *b_emclpt10_phi_TileGap1;   //!
   TBranch        *b_emclpt10_eta_TileGap2;   //!
   TBranch        *b_emclpt10_phi_TileGap2;   //!
   TBranch        *b_emclpt10_eta_TileGap3;   //!
   TBranch        *b_emclpt10_phi_TileGap3;   //!
   TBranch        *b_emclpt10_eta_TileExt0;   //!
   TBranch        *b_emclpt10_phi_TileExt0;   //!
   TBranch        *b_emclpt10_eta_TileExt1;   //!
   TBranch        *b_emclpt10_phi_TileExt1;   //!
   TBranch        *b_emclpt10_eta_TileExt2;   //!
   TBranch        *b_emclpt10_phi_TileExt2;   //!
   TBranch        *b_emclpt10_eta_FCAL0;   //!
   TBranch        *b_emclpt10_phi_FCAL0;   //!
   TBranch        *b_emclpt10_eta_FCAL1;   //!
   TBranch        *b_emclpt10_phi_FCAL1;   //!
   TBranch        *b_emclpt10_eta_FCAL2;   //!
   TBranch        *b_emclpt10_phi_FCAL2;   //!
   TBranch        *b_trk_n;   //!
   TBranch        *b_trk_pt;   //!
   TBranch        *b_trk_eta;   //!
   TBranch        *b_trk_d0_wrtPV;   //!
   TBranch        *b_trk_z0_wrtPV;   //!
   TBranch        *b_trk_phi_wrtPV;   //!
   TBranch        *b_trk_theta_wrtPV;   //!
   TBranch        *b_trk_qoverp_wrtPV;   //!
   TBranch        *b_trk_chi2;   //!
   TBranch        *b_trk_ndof;   //!
   TBranch        *b_trkpt5_n;   //!
   TBranch        *b_trkpt5_d0;   //!
   TBranch        *b_trkpt5_z0;   //!
   TBranch        *b_trkpt5_phi;   //!
   TBranch        *b_trkpt5_theta;   //!
   TBranch        *b_trkpt5_qoverp;   //!
   TBranch        *b_trkpt5_pt;   //!
   TBranch        *b_trkpt5_eta;   //!
   TBranch        *b_trkpt5_err_d0;   //!
   TBranch        *b_trkpt5_err_z0;   //!
   TBranch        *b_trkpt5_err_phi;   //!
   TBranch        *b_trkpt5_err_theta;   //!
   TBranch        *b_trkpt5_err_qoverp;   //!
   TBranch        *b_trkpt5_d0_wrtPV;   //!
   TBranch        *b_trkpt5_z0_wrtPV;   //!
   TBranch        *b_trkpt5_phi_wrtPV;   //!
   TBranch        *b_trkpt5_err_d0_wrtPV;   //!
   TBranch        *b_trkpt5_err_z0_wrtPV;   //!
   TBranch        *b_trkpt5_err_phi_wrtPV;   //!
   TBranch        *b_trkpt5_err_theta_wrtPV;   //!
   TBranch        *b_trkpt5_err_qoverp_wrtPV;   //!
   TBranch        *b_trkpt5_cov_d0_z0_wrtPV;   //!
   TBranch        *b_trkpt5_cov_d0_phi_wrtPV;   //!
   TBranch        *b_trkpt5_cov_d0_theta_wrtPV;   //!
   TBranch        *b_trkpt5_cov_d0_qoverp_wrtPV;   //!
   TBranch        *b_trkpt5_cov_z0_phi_wrtPV;   //!
   TBranch        *b_trkpt5_cov_z0_theta_wrtPV;   //!
   TBranch        *b_trkpt5_cov_z0_qoverp_wrtPV;   //!
   TBranch        *b_trkpt5_cov_phi_theta_wrtPV;   //!
   TBranch        *b_trkpt5_cov_phi_qoverp_wrtPV;   //!
   TBranch        *b_trkpt5_cov_theta_qoverp_wrtPV;   //!
   TBranch        *b_trkpt5_chi2;   //!
   TBranch        *b_trkpt5_ndof;   //!
   TBranch        *b_trkpt5_nBLHits;   //!
   TBranch        *b_trkpt5_nPixHits;   //!
   TBranch        *b_trkpt5_nSCTHits;   //!
   TBranch        *b_trkpt5_nTRTHits;   //!
   TBranch        *b_trkpt5_nPixHoles;   //!
   TBranch        *b_trkpt5_nSCTHoles;   //!
   TBranch        *b_trkpt5_nMDTHits;   //!
   TBranch        *b_trkpt5_nCSCEtaHits;   //!
   TBranch        *b_trkpt5_nCSCPhiHits;   //!
   TBranch        *b_trkpt5_nRPCEtaHits;   //!
   TBranch        *b_trkpt5_nRPCPhiHits;   //!
   TBranch        *b_trkpt5_nTGCEtaHits;   //!
   TBranch        *b_trkpt5_nTGCPhiHits;   //!
   TBranch        *b_trkpt5_nHits;   //!
   TBranch        *b_trkpt5_nHoles;   //!
   TBranch        *b_trkpt5_hitPattern;   //!
   TBranch        *b_trkpt5_TRTHighTHitsRatio;   //!
   TBranch        *b_trkpt5_pixeldEdx;   //!
   TBranch        *b_trkpt5_mc_probability;   //!
   TBranch        *b_trkpt5_mc_barcode;   //!
   TBranch        *b_vxp_n;   //!
   TBranch        *b_vxp_x;   //!
   TBranch        *b_vxp_y;   //!
   TBranch        *b_vxp_z;   //!
   TBranch        *b_vxp_cov_x;   //!
   TBranch        *b_vxp_cov_y;   //!
   TBranch        *b_vxp_cov_z;   //!
   TBranch        *b_vxp_chi2;   //!
   TBranch        *b_vxp_ndof;   //!
   TBranch        *b_vxp_px;   //!
   TBranch        *b_vxp_py;   //!
   TBranch        *b_vxp_pz;   //!
   TBranch        *b_vxp_E;   //!
   TBranch        *b_vxp_m;   //!
   TBranch        *b_vxp_nTracks;   //!
   TBranch        *b_vxp_sumPt;   //!
   TBranch        *b_vxp_type;   //!
   TBranch        *b_vxp_trk_n;   //!
   TBranch        *b_vxp_trk_index;   //!
   TBranch        *b_mb_n;   //!
   TBranch        *b_mb_E;   //!
   TBranch        *b_mb_eta;   //!
   TBranch        *b_mb_phi;   //!
   TBranch        *b_mb_time;   //!
   TBranch        *b_mb_quality;   //!
   TBranch        *b_mb_type;   //!
   TBranch        *b_mb_module;   //!
   TBranch        *b_mb_channel;   //!
   TBranch        *b_mbtime_timeDiff;   //!
   TBranch        *b_mbtime_timeA;   //!
   TBranch        *b_mbtime_timeC;   //!
   TBranch        *b_mbtime_countA;   //!
   TBranch        *b_mbtime_countC;   //!
   TBranch        *b_L1_MBTS_1;   //!
   TBranch        *b_L1_MBTS_1_1;   //!
   TBranch        *b_L1_MBTS_1_1_EMPTY;   //!
   TBranch        *b_L1_MBTS_1_1_UNPAIRED_ISO;   //!
   TBranch        *b_L1_MBTS_1_EMPTY;   //!
   TBranch        *b_L1_MBTS_1_UNPAIRED_ISO;   //!
   TBranch        *b_L1_MBTS_2;   //!
   TBranch        *b_L1_MBTS_2_EMPTY;   //!
   TBranch        *b_L1_MBTS_2_UNPAIRED_ISO;   //!
   TBranch        *b_L1_MBTS_2_UNPAIRED_NONISO;   //!
   TBranch        *b_L1_MBTS_4_4;   //!
   TBranch        *b_L1_MBTS_4_4_UNPAIRED_ISO;   //!
   TBranch        *b_collcand_passCaloTime;   //!
   TBranch        *b_collcand_passMBTSTime;   //!
   TBranch        *b_collcand_passTrigger;   //!
   TBranch        *b_collcand_pass;   //!
   TBranch        *b_muonTruth_n;   //!
   TBranch        *b_muonTruth_pt;   //!
   TBranch        *b_muonTruth_m;   //!
   TBranch        *b_muonTruth_eta;   //!
   TBranch        *b_muonTruth_phi;   //!
   TBranch        *b_muonTruth_charge;   //!
   TBranch        *b_muonTruth_PDGID;   //!
   TBranch        *b_muonTruth_barcode;   //!
   TBranch        *b_muonTruth_type;   //!
   TBranch        *b_muonTruth_origin;   //!
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
   TBranch        *b_mc_n;   //!
   TBranch        *b_mc_pt;   //!
   TBranch        *b_mc_m;   //!
   TBranch        *b_mc_eta;   //!
   TBranch        *b_mc_phi;   //!
   TBranch        *b_mc_status;   //!
   TBranch        *b_mc_barcode;   //!
   TBranch        *b_mc_parents;   //!
   TBranch        *b_mc_children;   //!
   TBranch        *b_mc_pdgId;   //!
   TBranch        *b_mc_charge;   //!
   TBranch        *b_mc_vx_x;   //!
   TBranch        *b_mc_vx_y;   //!
   TBranch        *b_mc_vx_z;   //!
   TBranch        *b_mc_child_index;   //!
   TBranch        *b_mc_parent_index;   //!
   TBranch        *b_trig_L2_combmuonfeature_L2_mu18_MG;   //!
   TBranch        *b_trig_L2_combmuonfeature_n;   //!
   TBranch        *b_trig_L2_combmuonfeature_pt;   //!
   TBranch        *b_trig_L2_combmuonfeature_eta;   //!
   TBranch        *b_trig_L2_combmuonfeature_phi;   //!
   TBranch        *b_trig_EF_trigmugirl_n;   //!
   TBranch        *b_trig_EF_trigmugirl_track_n;   //!
   TBranch        *b_trig_EF_trigmugirl_track_CB_pt;   //!
   TBranch        *b_trig_EF_trigmugirl_track_CB_eta;   //!
   TBranch        *b_trig_EF_trigmugirl_track_CB_phi;   //!
   TBranch        *b_trig_L2_muonfeature_n;   //!
   TBranch        *b_trig_L2_muonfeature_pt;   //!
   TBranch        *b_trig_L2_muonfeature_eta;   //!
   TBranch        *b_trig_L2_muonfeature_phi;   //!
   TBranch        *b_trig_L2_muonfeature_L2_mu40_MSonly_barrel;   //!
   TBranch        *b_trig_EF_trigmuonef_n;   //!
   TBranch        *b_trig_EF_trigmuonef_track_n;   //!
   TBranch        *b_trig_EF_trigmuonef_track_SA_pt;   //!
   TBranch        *b_trig_EF_trigmuonef_track_SA_eta;   //!
   TBranch        *b_trig_EF_trigmuonef_track_SA_phi;   //!
   TBranch        *b_trig_EF_el_n;   //!
   TBranch        *b_trig_EF_el_EF_e20_medium;   //!
   TBranch        *b_trig_EF_el_eta;   //!
   TBranch        *b_trig_EF_el_phi;   //!



   D3PDWZTreeV5(TTree *tree=0);
   virtual ~D3PDWZTreeV5();
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
};

#endif
