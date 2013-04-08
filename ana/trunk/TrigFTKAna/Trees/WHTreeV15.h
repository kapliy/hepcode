//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sun Jul 26 16:51:57 2009 by ROOT version 5.22/00a
// from TTree CollectionTree/CollectionTree
// found on file: WHbb120_reco_trig_10_ntuple.root
//////////////////////////////////////////////////////////

#ifndef WHTreeV15_h
#define WHTreeV15_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <vector>
#include <string>

class WHTreeV15 {
public :
  TTree          *fChain;   //!pointer to the analyzed TTree or TChain
  Int_t           fCurrent; //!current Tree number in a TChain

  // Declaration of leaf types
  Int_t           RunNumber;
  Int_t           EventNumber;
  Char_t          StreamESD_ref[153];
  Char_t          StreamRDO_ref;
  Char_t          Token[153];
  Int_t           Run;
  Int_t           Event;
  Int_t           Time;
  Double_t        Weight;
  Int_t           IEvent;
  UInt_t          NPar;
  std::vector<long>    *Type;
  std::vector<float>   *PtGen;
  std::vector<float>   *PhiGen;
  std::vector<float>   *EtaGen;
  std::vector<float>   *MGen;
  std::vector<float>   *Charge;
  std::vector<long>    *GenStat;
  std::vector<long>    *GenRef;
  std::vector<long>    *KMothNt;
  std::vector<long>    *KFDauNt;
  std::vector<long>    *KLDauNt;
  std::vector<long>    *KOriVNt;
  std::vector<long>    *KEndVNt;
  std::vector<long>    *BunchNum;
  std::vector<float>   *RVGen;
  std::vector<float>   *PhiVGen;
  std::vector<float>   *ZVGen;
  UInt_t          NVer;
  std::vector<long>    *BunchV;
  std::vector<float>   *RV;
  std::vector<float>   *PhiV;
  std::vector<float>   *ZV;
  std::vector<long>    *GenRefV;
  std::vector<long>    *KVMothNt;
  std::vector<long>    *KVFDauNt;
  std::vector<long>    *KVLDauNt;
  UInt_t          IVPrimary;
  Int_t           Spcl_Num;
  std::vector<long>    *Spcl_ID;
  std::vector<float>   *Spcl_Pt;
  std::vector<float>   *Spcl_Eta;
  std::vector<float>   *Spcl_Phi;
  std::vector<float>   *Spcl_M;
  std::vector<long>    *Spcl_Stat;
  std::vector<float>   *Spcl_Isol;
  Float_t         Spcl_sumx;
  Float_t         Spcl_sumy;
  Float_t         Spcl_sumz;
  Float_t         Spcl_sume;
  Float_t         Spcl_x1;
  Float_t         Spcl_x2;
  Int_t           Spcl_id1;
  Int_t           Spcl_id2;
  Int_t           MuonEntRec_Num;
  std::vector<long>    *MuonEntRec_ID;
  std::vector<float>   *MuonEntRec_Ene;
  std::vector<float>   *MuonEntRec_Eta;
  std::vector<float>   *MuonEntRec_Phi;
  std::vector<float>   *MuonEntRec_PosX;
  std::vector<float>   *MuonEntRec_PosY;
  std::vector<float>   *MuonEntRec_PosZ;
  std::vector<float>   *MuonEntRec_Time;
  std::vector<long>    *MuonEntRec_BarCode;
  UInt_t          vxp_numVertices;
  std::vector<float>   *vxp_vtx_x;
  std::vector<float>   *vxp_vtx_y;
  std::vector<float>   *vxp_vtx_z;
  UInt_t          vxp_numOfTruthVertices;
  std::vector<float>   *vxp_vtx_x_truth;
  std::vector<float>   *vxp_vtx_y_truth;
  std::vector<float>   *vxp_vtx_z_truth;
  std::vector<float>   *vxp_vtx_x_res;
  std::vector<float>   *vxp_vtx_y_res;
  std::vector<float>   *vxp_vtx_z_res;
  std::vector<float>   *vxp_sigvtx_x;
  std::vector<float>   *vxp_sigvtx_y;
  std::vector<float>   *vxp_sigvtx_z;
  std::vector<float>   *vxp_vtxchi2;
  std::vector<float>   *vxp_vtxndf;
  std::vector<float>   *vxp_pt;
  std::vector<float>   *vxp_vtx_constraint_x;
  std::vector<float>   *vxp_vtx_constraint_y;
  std::vector<float>   *vxp_vtx_constraint_z;
  std::vector<float>   *vxp_sigvtx_constraint_x;
  std::vector<float>   *vxp_sigvtx_constraint_y;
  std::vector<float>   *vxp_sigvtx_constraint_z;
  std::vector<float>   *vxp_vtx_linearization_x;
  std::vector<float>   *vxp_vtx_linearization_y;
  std::vector<float>   *vxp_vtx_linearization_z;
  std::vector<float>   *vxp_vtx_seed_x;
  std::vector<float>   *vxp_vtx_seed_y;
  std::vector<float>   *vxp_vtx_seed_z;
  std::vector<float>   *vxp_vtxchi2prob;
  std::vector<long>    *vxp_numTracksPerVertex;
  UInt_t          vxp_totalNumTracks;
  std::vector<float>   *vxp_chi2;
  std::vector<float>   *vxp_ndf;
  std::vector<float>   *vxp_vertexCompatibility;
  std::vector<float>   *vxp_d0;
  std::vector<float>   *vxp_sigd0;
  std::vector<float>   *vxp_z0;
  std::vector<float>   *vxp_sigz0;
  std::vector<float>   *vxp_phi;
  std::vector<float>   *vxp_sigphi;
  std::vector<float>   *vxp_theta;
  std::vector<float>   *vxp_sigtheta;
  std::vector<float>   *vxp_qOverP;
  std::vector<float>   *vxp_sigqOverP;
  std::vector<float>   *vxp_d0g;
  std::vector<float>   *vxp_sigd0g;
  std::vector<float>   *vxp_z0g;
  std::vector<float>   *vxp_sigz0g;
  std::vector<float>   *vxp_refitted_d0;
  std::vector<float>   *vxp_refitted_sigd0;
  std::vector<float>   *vxp_refitted_z0;
  std::vector<float>   *vxp_refitted_sigz0;
  std::vector<float>   *vxp_refitted_phi;
  std::vector<float>   *vxp_refitted_sigphi;
  std::vector<float>   *vxp_refitted_theta;
  std::vector<float>   *vxp_refitted_sigtheta;
  std::vector<float>   *vxp_refitted_qOverP;
  std::vector<float>   *vxp_refitted_sigqOverP;
  UInt_t          Trk_totalNumTracks;
  std::vector<float>   *Trk_d0;
  std::vector<float>   *Trk_z0;
  std::vector<float>   *Trk_phi;
  std::vector<float>   *Trk_theta;
  std::vector<float>   *Trk_qOverP;
  std::vector<float>   *Trk_Chi2;
  std::vector<long>    *Trk_Ndf;
  std::vector<float>   *Trk_sigd0;
  std::vector<float>   *Trk_sigz0;
  std::vector<float>   *Trk_sigphi;
  std::vector<float>   *Trk_sigtheta;
  std::vector<float>   *Trk_sigqOverP;
  std::vector<float>   *Trk_covVert21;
  std::vector<float>   *Trk_covVert31;
  std::vector<float>   *Trk_covVert32;
  std::vector<float>   *Trk_covVert41;
  std::vector<float>   *Trk_covVert42;
  std::vector<float>   *Trk_covVert43;
  std::vector<float>   *Trk_covVert51;
  std::vector<float>   *Trk_covVert52;
  std::vector<float>   *Trk_covVert53;
  std::vector<float>   *Trk_covVert54;
  std::vector<float>   *Trk_px;
  std::vector<float>   *Trk_py;
  std::vector<float>   *Trk_pz;
  std::vector<float>   *Trk_pt;
  std::vector<long>    *Trk_numberOfBLayerHits;
  std::vector<long>    *Trk_numberOfPixelHits;
  std::vector<long>    *Trk_numberOfSCTHits;
  std::vector<long>    *Trk_numberOfTRTHits;
  std::vector<long>    *Trk_numberOfTRTHighThresholdHits;
  std::vector<long>    *Trk_numberOfBLayerSharedHits;
  std::vector<long>    *Trk_numberOfPixelSharedHits;
  std::vector<long>    *Trk_numberOfPixelHoles;
  std::vector<long>    *Trk_numberOfSCTSharedHits;
  std::vector<long>    *Trk_numberOfSCTHoles;
  std::vector<long>    *Trk_numberOfTRTOutliers;
  std::vector<long>    *Trk_numberOfTRTHighThresholdOutliers;
  std::vector<long>    *Trk_numberOfMdtHits;
  std::vector<long>    *Trk_numberOfTgcPhiHits;
  std::vector<long>    *Trk_numberOfRpcPhiHits;
  std::vector<long>    *Trk_numberOfCscPhiHits;
  std::vector<long>    *Trk_numberOfTgcEtaHits;
  std::vector<long>    *Trk_numberOfRpcEtaHits;
  std::vector<long>    *Trk_numberOfCscEtaHits;
  std::vector<long>    *Trk_numberOfGangedPixels;
  std::vector<long>    *Trk_numberOfOutliersOnTrack;
  std::vector<long>    *Trk_numberOfTrackSummaryTypes;
  std::vector<long>    *Trk_truthBarcode;
  std::vector<long>    *Trk_truthNt;
  std::vector<long>    *Trk_ParticlePdg;
  std::vector<long>    *Trk_ParentPdg;
  Int_t           Nh_Calo;
  Float_t         Eh_Calo;
  Int_t           Nh_EM;
  Float_t         Eh_EM;
  Int_t           Nh_HAD;
  Float_t         Eh_HAD;
  Int_t           Nh_PresB;
  std::vector<float>   *Eh_PresB;
  Int_t           Nh_EMB;
  std::vector<float>   *Eh_EMB;
  Int_t           Nh_EMEC;
  std::vector<float>   *Eh_EMEC;
  Int_t           Nh_Tile;
  std::vector<float>   *Eh_Tile;
  Int_t           Nh_TileGap;
  std::vector<float>   *Eh_TileGap;
  Int_t           Nh_HEC;
  std::vector<float>   *Eh_HEC;
  Int_t           Nh_FCal;
  std::vector<float>   *Eh_FCal;
  Int_t           Nh_PresE;
  std::vector<float>   *Eh_PresE;
  Int_t           Nh_Scint;
  std::vector<float>   *Eh_Scint;
  Int_t           nhit_em;
  Float_t         ecell_em;
  Int_t           nhit_hec;
  Float_t         ecell_hec;
  Int_t           nhit_fcal;
  Float_t         ecell_fcal;
  Int_t           nhit_tile;
  Float_t         ecell_tile;
  Float_t         cl_ecluster;
  Int_t           cl_nctotal;
  Int_t           cl_nc;
  std::vector<float>   *cl_et;
  std::vector<float>   *cl_e;
  std::vector<float>   *cl_eemb0;
  std::vector<float>   *cl_eemb1;
  std::vector<float>   *cl_eemb2;
  std::vector<float>   *cl_eemb3;
  std::vector<long>    *cl_nemb0;
  std::vector<long>    *cl_nemb1;
  std::vector<long>    *cl_nemb2;
  std::vector<long>    *cl_nemb3;
  std::vector<float>   *cl_phi2;
  std::vector<float>   *cl_eta0;
  std::vector<float>   *cl_eta1;
  std::vector<float>   *cl_eta2;
  std::vector<float>   *cl_eta3;
  std::vector<float>   *cl_eeme0;
  std::vector<float>   *cl_eeme1;
  std::vector<float>   *cl_eeme2;
  std::vector<float>   *cl_eeme3;
  std::vector<long>    *cl_neme0;
  std::vector<long>    *cl_neme1;
  std::vector<long>    *cl_neme2;
  std::vector<long>    *cl_neme3;
  std::vector<float>   *cl_etileg1;
  std::vector<float>   *cl_etileg2;
  std::vector<float>   *cl_etileg3;
  std::vector<long>    *cl_ntileg1;
  std::vector<long>    *cl_ntileg2;
  std::vector<long>    *cl_ntileg3;
  std::vector<float>   *cl_eta;
  std::vector<float>   *cl_phi;
  std::vector<float>   *cl_time;
  std::vector<long>    *cl_reco_stat;
  std::vector<float>   *cl_m1_eta;
  std::vector<float>   *cl_m1_phi;
  std::vector<float>   *cl_m2_r;
  std::vector<float>   *cl_m2_lambda;
  std::vector<float>   *cl_delta_phi;
  std::vector<float>   *cl_delta_theta;
  std::vector<float>   *cl_delta_alpha;
  std::vector<float>   *cl_center_x;
  std::vector<float>   *cl_center_y;
  std::vector<float>   *cl_center_z;
  std::vector<float>   *cl_center_lambda;
  std::vector<float>   *cl_lateral;
  std::vector<float>   *cl_longitudinal;
  std::vector<float>   *cl_eng_frac_em;
  std::vector<float>   *cl_eng_frac_max;
  std::vector<float>   *cl_eng_frac_core;
  std::vector<float>   *cl_m1_dens;
  std::vector<float>   *cl_m2_dens;
  std::vector<long>    *cl_ntotcells;
  Float_t         cl_ecluster_gam;
  Int_t           cl_nctotal_gam;
  Int_t           cl_nc_gam;
  std::vector<float>   *cl_et_gam;
  std::vector<float>   *cl_e_gam;
  std::vector<float>   *cl_eemb0_gam;
  std::vector<float>   *cl_eemb1_gam;
  std::vector<float>   *cl_eemb2_gam;
  std::vector<float>   *cl_eemb3_gam;
  std::vector<long>    *cl_nemb0_gam;
  std::vector<long>    *cl_nemb1_gam;
  std::vector<long>    *cl_nemb2_gam;
  std::vector<long>    *cl_nemb3_gam;
  std::vector<float>   *cl_phi2_gam;
  std::vector<float>   *cl_eta0_gam;
  std::vector<float>   *cl_eta1_gam;
  std::vector<float>   *cl_eta2_gam;
  std::vector<float>   *cl_eta3_gam;
  std::vector<float>   *cl_eeme0_gam;
  std::vector<float>   *cl_eeme1_gam;
  std::vector<float>   *cl_eeme2_gam;
  std::vector<float>   *cl_eeme3_gam;
  std::vector<long>    *cl_neme0_gam;
  std::vector<long>    *cl_neme1_gam;
  std::vector<long>    *cl_neme2_gam;
  std::vector<long>    *cl_neme3_gam;
  std::vector<float>   *cl_etileg1_gam;
  std::vector<float>   *cl_etileg2_gam;
  std::vector<float>   *cl_etileg3_gam;
  std::vector<long>    *cl_ntileg1_gam;
  std::vector<long>    *cl_ntileg2_gam;
  std::vector<long>    *cl_ntileg3_gam;
  std::vector<float>   *cl_eta_gam;
  std::vector<float>   *cl_phi_gam;
  std::vector<float>   *cl_time_gam;
  std::vector<long>    *cl_reco_stat_gam;
  std::vector<float>   *cl_m1_eta_gam;
  std::vector<float>   *cl_m1_phi_gam;
  std::vector<float>   *cl_m2_r_gam;
  std::vector<float>   *cl_m2_lambda_gam;
  std::vector<float>   *cl_delta_phi_gam;
  std::vector<float>   *cl_delta_theta_gam;
  std::vector<float>   *cl_delta_alpha_gam;
  std::vector<float>   *cl_center_x_gam;
  std::vector<float>   *cl_center_y_gam;
  std::vector<float>   *cl_center_z_gam;
  std::vector<float>   *cl_center_lambda_gam;
  std::vector<float>   *cl_lateral_gam;
  std::vector<float>   *cl_longitudinal_gam;
  std::vector<float>   *cl_eng_frac_em_gam;
  std::vector<float>   *cl_eng_frac_max_gam;
  std::vector<float>   *cl_eng_frac_core_gam;
  std::vector<float>   *cl_m1_dens_gam;
  std::vector<float>   *cl_m2_dens_gam;
  std::vector<long>    *cl_ntotcells_gam;
  Float_t         cl_ecluster_35;
  Int_t           cl_nctotal_35;
  Int_t           cl_nc_35;
  std::vector<float>   *cl_et_35;
  std::vector<float>   *cl_e_35;
  std::vector<float>   *cl_eemb0_35;
  std::vector<float>   *cl_eemb1_35;
  std::vector<float>   *cl_eemb2_35;
  std::vector<float>   *cl_eemb3_35;
  std::vector<long>    *cl_nemb0_35;
  std::vector<long>    *cl_nemb1_35;
  std::vector<long>    *cl_nemb2_35;
  std::vector<long>    *cl_nemb3_35;
  std::vector<float>   *cl_phi2_35;
  std::vector<float>   *cl_eta0_35;
  std::vector<float>   *cl_eta1_35;
  std::vector<float>   *cl_eta2_35;
  std::vector<float>   *cl_eta3_35;
  std::vector<float>   *cl_eeme0_35;
  std::vector<float>   *cl_eeme1_35;
  std::vector<float>   *cl_eeme2_35;
  std::vector<float>   *cl_eeme3_35;
  std::vector<long>    *cl_neme0_35;
  std::vector<long>    *cl_neme1_35;
  std::vector<long>    *cl_neme2_35;
  std::vector<long>    *cl_neme3_35;
  std::vector<float>   *cl_etileg1_35;
  std::vector<float>   *cl_etileg2_35;
  std::vector<float>   *cl_etileg3_35;
  std::vector<long>    *cl_ntileg1_35;
  std::vector<long>    *cl_ntileg2_35;
  std::vector<long>    *cl_ntileg3_35;
  std::vector<float>   *cl_eta_35;
  std::vector<float>   *cl_phi_35;
  std::vector<float>   *cl_time_35;
  std::vector<long>    *cl_reco_stat_35;
  std::vector<float>   *cl_m1_eta_35;
  std::vector<float>   *cl_m1_phi_35;
  std::vector<float>   *cl_m2_r_35;
  std::vector<float>   *cl_m2_lambda_35;
  std::vector<float>   *cl_delta_phi_35;
  std::vector<float>   *cl_delta_theta_35;
  std::vector<float>   *cl_delta_alpha_35;
  std::vector<float>   *cl_center_x_35;
  std::vector<float>   *cl_center_y_35;
  std::vector<float>   *cl_center_z_35;
  std::vector<float>   *cl_center_lambda_35;
  std::vector<float>   *cl_lateral_35;
  std::vector<float>   *cl_longitudinal_35;
  std::vector<float>   *cl_eng_frac_em_35;
  std::vector<float>   *cl_eng_frac_max_35;
  std::vector<float>   *cl_eng_frac_core_35;
  std::vector<float>   *cl_m1_dens_35;
  std::vector<float>   *cl_m2_dens_35;
  std::vector<long>    *cl_ntotcells_35;
  Float_t         cl_ecluster_35gam;
  Int_t           cl_nctotal_35gam;
  Int_t           cl_nc_35gam;
  std::vector<float>   *cl_et_35gam;
  std::vector<float>   *cl_e_35gam;
  std::vector<float>   *cl_eemb0_35gam;
  std::vector<float>   *cl_eemb1_35gam;
  std::vector<float>   *cl_eemb2_35gam;
  std::vector<float>   *cl_eemb3_35gam;
  std::vector<long>    *cl_nemb0_35gam;
  std::vector<long>    *cl_nemb1_35gam;
  std::vector<long>    *cl_nemb2_35gam;
  std::vector<long>    *cl_nemb3_35gam;
  std::vector<float>   *cl_phi2_35gam;
  std::vector<float>   *cl_eta0_35gam;
  std::vector<float>   *cl_eta1_35gam;
  std::vector<float>   *cl_eta2_35gam;
  std::vector<float>   *cl_eta3_35gam;
  std::vector<float>   *cl_eeme0_35gam;
  std::vector<float>   *cl_eeme1_35gam;
  std::vector<float>   *cl_eeme2_35gam;
  std::vector<float>   *cl_eeme3_35gam;
  std::vector<long>    *cl_neme0_35gam;
  std::vector<long>    *cl_neme1_35gam;
  std::vector<long>    *cl_neme2_35gam;
  std::vector<long>    *cl_neme3_35gam;
  std::vector<float>   *cl_etileg1_35gam;
  std::vector<float>   *cl_etileg2_35gam;
  std::vector<float>   *cl_etileg3_35gam;
  std::vector<long>    *cl_ntileg1_35gam;
  std::vector<long>    *cl_ntileg2_35gam;
  std::vector<long>    *cl_ntileg3_35gam;
  std::vector<float>   *cl_eta_35gam;
  std::vector<float>   *cl_phi_35gam;
  std::vector<float>   *cl_time_35gam;
  std::vector<long>    *cl_reco_stat_35gam;
  std::vector<float>   *cl_m1_eta_35gam;
  std::vector<float>   *cl_m1_phi_35gam;
  std::vector<float>   *cl_m2_r_35gam;
  std::vector<float>   *cl_m2_lambda_35gam;
  std::vector<float>   *cl_delta_phi_35gam;
  std::vector<float>   *cl_delta_theta_35gam;
  std::vector<float>   *cl_delta_alpha_35gam;
  std::vector<float>   *cl_center_x_35gam;
  std::vector<float>   *cl_center_y_35gam;
  std::vector<float>   *cl_center_z_35gam;
  std::vector<float>   *cl_center_lambda_35gam;
  std::vector<float>   *cl_lateral_35gam;
  std::vector<float>   *cl_longitudinal_35gam;
  std::vector<float>   *cl_eng_frac_em_35gam;
  std::vector<float>   *cl_eng_frac_max_35gam;
  std::vector<float>   *cl_eng_frac_core_35gam;
  std::vector<float>   *cl_m1_dens_35gam;
  std::vector<float>   *cl_m2_dens_35gam;
  std::vector<long>    *cl_ntotcells_35gam;
  Float_t         cl_ecluster_37;
  Int_t           cl_nctotal_37;
  Int_t           cl_nc_37;
  std::vector<float>   *cl_et_37;
  std::vector<float>   *cl_e_37;
  std::vector<float>   *cl_eemb0_37;
  std::vector<float>   *cl_eemb1_37;
  std::vector<float>   *cl_eemb2_37;
  std::vector<float>   *cl_eemb3_37;
  std::vector<long>    *cl_nemb0_37;
  std::vector<long>    *cl_nemb1_37;
  std::vector<long>    *cl_nemb2_37;
  std::vector<long>    *cl_nemb3_37;
  std::vector<float>   *cl_phi2_37;
  std::vector<float>   *cl_eta0_37;
  std::vector<float>   *cl_eta1_37;
  std::vector<float>   *cl_eta2_37;
  std::vector<float>   *cl_eta3_37;
  std::vector<float>   *cl_eeme0_37;
  std::vector<float>   *cl_eeme1_37;
  std::vector<float>   *cl_eeme2_37;
  std::vector<float>   *cl_eeme3_37;
  std::vector<long>    *cl_neme0_37;
  std::vector<long>    *cl_neme1_37;
  std::vector<long>    *cl_neme2_37;
  std::vector<long>    *cl_neme3_37;
  std::vector<float>   *cl_etileg1_37;
  std::vector<float>   *cl_etileg2_37;
  std::vector<float>   *cl_etileg3_37;
  std::vector<long>    *cl_ntileg1_37;
  std::vector<long>    *cl_ntileg2_37;
  std::vector<long>    *cl_ntileg3_37;
  std::vector<float>   *cl_eta_37;
  std::vector<float>   *cl_phi_37;
  std::vector<float>   *cl_time_37;
  std::vector<long>    *cl_reco_stat_37;
  std::vector<float>   *cl_m1_eta_37;
  std::vector<float>   *cl_m1_phi_37;
  std::vector<float>   *cl_m2_r_37;
  std::vector<float>   *cl_m2_lambda_37;
  std::vector<float>   *cl_delta_phi_37;
  std::vector<float>   *cl_delta_theta_37;
  std::vector<float>   *cl_delta_alpha_37;
  std::vector<float>   *cl_center_x_37;
  std::vector<float>   *cl_center_y_37;
  std::vector<float>   *cl_center_z_37;
  std::vector<float>   *cl_center_lambda_37;
  std::vector<float>   *cl_lateral_37;
  std::vector<float>   *cl_longitudinal_37;
  std::vector<float>   *cl_eng_frac_em_37;
  std::vector<float>   *cl_eng_frac_max_37;
  std::vector<float>   *cl_eng_frac_core_37;
  std::vector<float>   *cl_m1_dens_37;
  std::vector<float>   *cl_m2_dens_37;
  std::vector<long>    *cl_ntotcells_37;
  Float_t         cl_ecluster_37gam;
  Int_t           cl_nctotal_37gam;
  Int_t           cl_nc_37gam;
  std::vector<float>   *cl_et_37gam;
  std::vector<float>   *cl_e_37gam;
  std::vector<float>   *cl_eemb0_37gam;
  std::vector<float>   *cl_eemb1_37gam;
  std::vector<float>   *cl_eemb2_37gam;
  std::vector<float>   *cl_eemb3_37gam;
  std::vector<long>    *cl_nemb0_37gam;
  std::vector<long>    *cl_nemb1_37gam;
  std::vector<long>    *cl_nemb2_37gam;
  std::vector<long>    *cl_nemb3_37gam;
  std::vector<float>   *cl_phi2_37gam;
  std::vector<float>   *cl_eta0_37gam;
  std::vector<float>   *cl_eta1_37gam;
  std::vector<float>   *cl_eta2_37gam;
  std::vector<float>   *cl_eta3_37gam;
  std::vector<float>   *cl_eeme0_37gam;
  std::vector<float>   *cl_eeme1_37gam;
  std::vector<float>   *cl_eeme2_37gam;
  std::vector<float>   *cl_eeme3_37gam;
  std::vector<long>    *cl_neme0_37gam;
  std::vector<long>    *cl_neme1_37gam;
  std::vector<long>    *cl_neme2_37gam;
  std::vector<long>    *cl_neme3_37gam;
  std::vector<float>   *cl_etileg1_37gam;
  std::vector<float>   *cl_etileg2_37gam;
  std::vector<float>   *cl_etileg3_37gam;
  std::vector<long>    *cl_ntileg1_37gam;
  std::vector<long>    *cl_ntileg2_37gam;
  std::vector<long>    *cl_ntileg3_37gam;
  std::vector<float>   *cl_eta_37gam;
  std::vector<float>   *cl_phi_37gam;
  std::vector<float>   *cl_time_37gam;
  std::vector<long>    *cl_reco_stat_37gam;
  std::vector<float>   *cl_m1_eta_37gam;
  std::vector<float>   *cl_m1_phi_37gam;
  std::vector<float>   *cl_m2_r_37gam;
  std::vector<float>   *cl_m2_lambda_37gam;
  std::vector<float>   *cl_delta_phi_37gam;
  std::vector<float>   *cl_delta_theta_37gam;
  std::vector<float>   *cl_delta_alpha_37gam;
  std::vector<float>   *cl_center_x_37gam;
  std::vector<float>   *cl_center_y_37gam;
  std::vector<float>   *cl_center_z_37gam;
  std::vector<float>   *cl_center_lambda_37gam;
  std::vector<float>   *cl_lateral_37gam;
  std::vector<float>   *cl_longitudinal_37gam;
  std::vector<float>   *cl_eng_frac_em_37gam;
  std::vector<float>   *cl_eng_frac_max_37gam;
  std::vector<float>   *cl_eng_frac_core_37gam;
  std::vector<float>   *cl_m1_dens_37gam;
  std::vector<float>   *cl_m2_dens_37gam;
  std::vector<long>    *cl_ntotcells_37gam;
  Float_t         cl_ecluster_Softe;
  Int_t           cl_nctotal_Softe;
  Int_t           cl_nc_Softe;
  std::vector<float>   *cl_et_Softe;
  std::vector<float>   *cl_e_Softe;
  std::vector<float>   *cl_eemb0_Softe;
  std::vector<float>   *cl_eemb1_Softe;
  std::vector<float>   *cl_eemb2_Softe;
  std::vector<float>   *cl_eemb3_Softe;
  std::vector<long>    *cl_nemb0_Softe;
  std::vector<long>    *cl_nemb1_Softe;
  std::vector<long>    *cl_nemb2_Softe;
  std::vector<long>    *cl_nemb3_Softe;
  std::vector<float>   *cl_phi2_Softe;
  std::vector<float>   *cl_eta0_Softe;
  std::vector<float>   *cl_eta1_Softe;
  std::vector<float>   *cl_eta2_Softe;
  std::vector<float>   *cl_eta3_Softe;
  std::vector<float>   *cl_eeme0_Softe;
  std::vector<float>   *cl_eeme1_Softe;
  std::vector<float>   *cl_eeme2_Softe;
  std::vector<float>   *cl_eeme3_Softe;
  std::vector<long>    *cl_neme0_Softe;
  std::vector<long>    *cl_neme1_Softe;
  std::vector<long>    *cl_neme2_Softe;
  std::vector<long>    *cl_neme3_Softe;
  std::vector<float>   *cl_etileg1_Softe;
  std::vector<float>   *cl_etileg2_Softe;
  std::vector<float>   *cl_etileg3_Softe;
  std::vector<long>    *cl_ntileg1_Softe;
  std::vector<long>    *cl_ntileg2_Softe;
  std::vector<long>    *cl_ntileg3_Softe;
  std::vector<float>   *cl_eta_Softe;
  std::vector<float>   *cl_phi_Softe;
  std::vector<float>   *cl_time_Softe;
  std::vector<long>    *cl_reco_stat_Softe;
  std::vector<float>   *cl_m1_eta_Softe;
  std::vector<float>   *cl_m1_phi_Softe;
  std::vector<float>   *cl_m2_r_Softe;
  std::vector<float>   *cl_m2_lambda_Softe;
  std::vector<float>   *cl_delta_phi_Softe;
  std::vector<float>   *cl_delta_theta_Softe;
  std::vector<float>   *cl_delta_alpha_Softe;
  std::vector<float>   *cl_center_x_Softe;
  std::vector<float>   *cl_center_y_Softe;
  std::vector<float>   *cl_center_z_Softe;
  std::vector<float>   *cl_center_lambda_Softe;
  std::vector<float>   *cl_lateral_Softe;
  std::vector<float>   *cl_longitudinal_Softe;
  std::vector<float>   *cl_eng_frac_em_Softe;
  std::vector<float>   *cl_eng_frac_max_Softe;
  std::vector<float>   *cl_eng_frac_core_Softe;
  std::vector<float>   *cl_m1_dens_Softe;
  std::vector<float>   *cl_m2_dens_Softe;
  std::vector<long>    *cl_ntotcells_Softe;
  Float_t         cl_ecluster_comb;
  Int_t           cl_nctotal_comb;
  Int_t           cl_nc_comb;
  std::vector<float>   *cl_et_comb;
  std::vector<float>   *cl_e_comb;
  std::vector<float>   *cl_eemb0_comb;
  std::vector<float>   *cl_eemb1_comb;
  std::vector<float>   *cl_eemb2_comb;
  std::vector<float>   *cl_eemb3_comb;
  std::vector<long>    *cl_nemb0_comb;
  std::vector<long>    *cl_nemb1_comb;
  std::vector<long>    *cl_nemb2_comb;
  std::vector<long>    *cl_nemb3_comb;
  std::vector<float>   *cl_phi2_comb;
  std::vector<float>   *cl_eta0_comb;
  std::vector<float>   *cl_eta1_comb;
  std::vector<float>   *cl_eta2_comb;
  std::vector<float>   *cl_eta3_comb;
  std::vector<float>   *cl_eeme0_comb;
  std::vector<float>   *cl_eeme1_comb;
  std::vector<float>   *cl_eeme2_comb;
  std::vector<float>   *cl_eeme3_comb;
  std::vector<long>    *cl_neme0_comb;
  std::vector<long>    *cl_neme1_comb;
  std::vector<long>    *cl_neme2_comb;
  std::vector<long>    *cl_neme3_comb;
  std::vector<float>   *cl_etileg1_comb;
  std::vector<float>   *cl_etileg2_comb;
  std::vector<float>   *cl_etileg3_comb;
  std::vector<long>    *cl_ntileg1_comb;
  std::vector<long>    *cl_ntileg2_comb;
  std::vector<long>    *cl_ntileg3_comb;
  std::vector<float>   *cl_eta_comb;
  std::vector<float>   *cl_phi_comb;
  std::vector<float>   *cl_time_comb;
  std::vector<long>    *cl_reco_stat_comb;
  std::vector<float>   *cl_m1_eta_comb;
  std::vector<float>   *cl_m1_phi_comb;
  std::vector<float>   *cl_m2_r_comb;
  std::vector<float>   *cl_m2_lambda_comb;
  std::vector<float>   *cl_delta_phi_comb;
  std::vector<float>   *cl_delta_theta_comb;
  std::vector<float>   *cl_delta_alpha_comb;
  std::vector<float>   *cl_center_x_comb;
  std::vector<float>   *cl_center_y_comb;
  std::vector<float>   *cl_center_z_comb;
  std::vector<float>   *cl_center_lambda_comb;
  std::vector<float>   *cl_lateral_comb;
  std::vector<float>   *cl_longitudinal_comb;
  std::vector<float>   *cl_eng_frac_em_comb;
  std::vector<float>   *cl_eng_frac_max_comb;
  std::vector<float>   *cl_eng_frac_core_comb;
  std::vector<float>   *cl_m1_dens_comb;
  std::vector<float>   *cl_m2_dens_comb;
  std::vector<float>   *cl_ehec0_comb;
  std::vector<float>   *cl_ehec1_comb;
  std::vector<float>   *cl_ehec2_comb;
  std::vector<float>   *cl_ehec3_comb;
  std::vector<long>    *cl_nhec0_comb;
  std::vector<long>    *cl_nhec1_comb;
  std::vector<long>    *cl_nhec2_comb;
  std::vector<long>    *cl_nhec3_comb;
  std::vector<float>   *cl_etileb0_comb;
  std::vector<float>   *cl_etileb1_comb;
  std::vector<float>   *cl_etileb2_comb;
  std::vector<long>    *cl_ntileb0_comb;
  std::vector<long>    *cl_ntileb1_comb;
  std::vector<long>    *cl_ntileb2_comb;
  std::vector<float>   *cl_etilee0_comb;
  std::vector<float>   *cl_etilee1_comb;
  std::vector<float>   *cl_etilee2_comb;
  std::vector<float>   *cl_efcal0_comb;
  std::vector<float>   *cl_efcal1_comb;
  std::vector<float>   *cl_efcal2_comb;
  std::vector<long>    *cl_ntilee0_comb;
  std::vector<long>    *cl_ntilee1_comb;
  std::vector<long>    *cl_ntilee2_comb;
  std::vector<long>    *cl_nfcal0_comb;
  std::vector<long>    *cl_nfcal1_comb;
  std::vector<long>    *cl_nfcal2_comb;
  std::vector<long>    *cl_ntotcells_comb;
  Float_t         cl_ecluster_topoEM;
  Int_t           cl_nctotal_topoEM;
  Int_t           cl_nc_topoEM;
  std::vector<float>   *cl_et_topoEM;
  std::vector<float>   *cl_e_topoEM;
  std::vector<float>   *cl_eemb0_topoEM;
  std::vector<float>   *cl_eemb1_topoEM;
  std::vector<float>   *cl_eemb2_topoEM;
  std::vector<float>   *cl_eemb3_topoEM;
  std::vector<long>    *cl_nemb0_topoEM;
  std::vector<long>    *cl_nemb1_topoEM;
  std::vector<long>    *cl_nemb2_topoEM;
  std::vector<long>    *cl_nemb3_topoEM;
  std::vector<float>   *cl_phi2_topoEM;
  std::vector<float>   *cl_eta0_topoEM;
  std::vector<float>   *cl_eta1_topoEM;
  std::vector<float>   *cl_eta2_topoEM;
  std::vector<float>   *cl_eta3_topoEM;
  std::vector<float>   *cl_eeme0_topoEM;
  std::vector<float>   *cl_eeme1_topoEM;
  std::vector<float>   *cl_eeme2_topoEM;
  std::vector<float>   *cl_eeme3_topoEM;
  std::vector<long>    *cl_neme0_topoEM;
  std::vector<long>    *cl_neme1_topoEM;
  std::vector<long>    *cl_neme2_topoEM;
  std::vector<long>    *cl_neme3_topoEM;
  std::vector<float>   *cl_etileg1_topoEM;
  std::vector<float>   *cl_etileg2_topoEM;
  std::vector<float>   *cl_etileg3_topoEM;
  std::vector<long>    *cl_ntileg1_topoEM;
  std::vector<long>    *cl_ntileg2_topoEM;
  std::vector<long>    *cl_ntileg3_topoEM;
  std::vector<float>   *cl_eta_topoEM;
  std::vector<float>   *cl_phi_topoEM;
  std::vector<float>   *cl_time_topoEM;
  std::vector<long>    *cl_reco_stat_topoEM;
  std::vector<float>   *cl_m1_eta_topoEM;
  std::vector<float>   *cl_m1_phi_topoEM;
  std::vector<float>   *cl_m2_r_topoEM;
  std::vector<float>   *cl_m2_lambda_topoEM;
  std::vector<float>   *cl_delta_phi_topoEM;
  std::vector<float>   *cl_delta_theta_topoEM;
  std::vector<float>   *cl_delta_alpha_topoEM;
  std::vector<float>   *cl_center_x_topoEM;
  std::vector<float>   *cl_center_y_topoEM;
  std::vector<float>   *cl_center_z_topoEM;
  std::vector<float>   *cl_center_lambda_topoEM;
  std::vector<float>   *cl_lateral_topoEM;
  std::vector<float>   *cl_longitudinal_topoEM;
  std::vector<float>   *cl_eng_frac_em_topoEM;
  std::vector<float>   *cl_eng_frac_max_topoEM;
  std::vector<float>   *cl_eng_frac_core_topoEM;
  std::vector<float>   *cl_m1_dens_topoEM;
  std::vector<float>   *cl_m2_dens_topoEM;
  std::vector<long>    *cl_ntotcells_topoEM;
  UInt_t          IS_Nsgins;
  std::vector<float>   *IS_Xpt;
  std::vector<float>   *IS_Ypt;
  std::vector<float>   *IS_Zpt;
  std::vector<float>   *IS_Xvec;
  std::vector<float>   *IS_Yvec;
  std::vector<float>   *IS_Zvec;
  std::vector<float>   *IS_QualityFactor;
  std::vector<long>    *IS_NbreOfMultiLayer;
  std::vector<long>    *IS_TrackSharing;
  std::vector<long>    *IS_StationName;
  std::vector<long>    *IS_Ndigits;
  UInt_t          MB_NTrk;
  UInt_t          VT_NTrk;
  std::vector<float>   *VT_A0;
  std::vector<float>   *VT_Z;
  std::vector<float>   *VT_Phi;
  std::vector<float>   *VT_CotTh;
  std::vector<float>   *VT_PTInv;
  std::vector<float>   *VT_Cov11;
  std::vector<float>   *VT_Cov21;
  std::vector<float>   *VT_Cov22;
  std::vector<float>   *VT_Cov31;
  std::vector<float>   *VT_Cov32;
  std::vector<float>   *VT_Cov33;
  std::vector<float>   *VT_Cov41;
  std::vector<float>   *VT_Cov42;
  std::vector<float>   *VT_Cov43;
  std::vector<float>   *VT_Cov44;
  std::vector<float>   *VT_Cov51;
  std::vector<float>   *VT_Cov52;
  std::vector<float>   *VT_Cov53;
  std::vector<float>   *VT_Cov54;
  std::vector<float>   *VT_Cov55;
  std::vector<float>   *VT_Xpt;
  std::vector<float>   *VT_Ypt;
  std::vector<float>   *VT_Zpt;
  std::vector<float>   *VT_Xvec;
  std::vector<float>   *VT_Yvec;
  std::vector<float>   *VT_Zvec;
  std::vector<float>   *VT_Mom;
  std::vector<long>    *VT_digits;
  UInt_t          KE_NTrk;
  std::vector<float>   *KE_Thept;
  std::vector<float>   *KE_Phipt;
  std::vector<float>   *KE_Thevc;
  std::vector<float>   *KE_Phivc;
  std::vector<float>   *KE_PInv;
  std::vector<float>   *KE_Cov11;
  std::vector<float>   *KE_Cov21;
  std::vector<float>   *KE_Cov22;
  std::vector<float>   *KE_Cov31;
  std::vector<float>   *KE_Cov32;
  std::vector<float>   *KE_Cov33;
  std::vector<float>   *KE_Cov41;
  std::vector<float>   *KE_Cov42;
  std::vector<float>   *KE_Cov43;
  std::vector<float>   *KE_Cov44;
  std::vector<float>   *KE_Cov51;
  std::vector<float>   *KE_Cov52;
  std::vector<float>   *KE_Cov53;
  std::vector<float>   *KE_Cov54;
  std::vector<float>   *KE_Cov55;
  std::vector<float>   *KE_Xpt;
  std::vector<float>   *KE_Ypt;
  std::vector<float>   *KE_Zpt;
  std::vector<float>   *KE_Xvec;
  std::vector<float>   *KE_Yvec;
  std::vector<float>   *KE_Zvec;
  std::vector<float>   *KE_Mom;
  std::vector<float>   *KE_ZCyl;
  std::vector<float>   *KE_RCyl;
  UInt_t          MS_NTrk;
  std::vector<float>   *MS_Thept;
  std::vector<float>   *MS_Phipt;
  std::vector<float>   *MS_Thevc;
  std::vector<float>   *MS_Phivc;
  std::vector<float>   *MS_PInv;
  std::vector<float>   *MS_Cov11;
  std::vector<float>   *MS_Cov21;
  std::vector<float>   *MS_Cov22;
  std::vector<float>   *MS_Cov31;
  std::vector<float>   *MS_Cov32;
  std::vector<float>   *MS_Cov33;
  std::vector<float>   *MS_Cov41;
  std::vector<float>   *MS_Cov42;
  std::vector<float>   *MS_Cov43;
  std::vector<float>   *MS_Cov44;
  std::vector<float>   *MS_Cov51;
  std::vector<float>   *MS_Cov52;
  std::vector<float>   *MS_Cov53;
  std::vector<float>   *MS_Cov54;
  std::vector<float>   *MS_Cov55;
  std::vector<float>   *MS_Xpt;
  std::vector<float>   *MS_Ypt;
  std::vector<float>   *MS_Zpt;
  std::vector<float>   *MS_Xvec;
  std::vector<float>   *MS_Yvec;
  std::vector<float>   *MS_Zvec;
  std::vector<float>   *MS_Mom;
  std::vector<float>   *MS_ZCyl;
  std::vector<float>   *MS_RCyl;
  UInt_t          DR1_NTrk;
  std::vector<float>   *MbKE_Calo_En_dr01;
  std::vector<float>   *MbKE_Calo_Et_dr01;
  std::vector<float>   *MbKE_HAD_Calo_En_dr01;
  std::vector<float>   *MbKE_HAD_Calo_Et_dr01;
  std::vector<float>   *MbKE_EM_Calo_En_dr01;
  std::vector<float>   *MbKE_EM_Calo_Et_dr01;
  UInt_t          Mboy_nmuon;
  std::vector<float>   *Mboy_A0;
  std::vector<float>   *Mboy_Z;
  std::vector<float>   *Mboy_Phi;
  std::vector<float>   *Mboy_Theta;
  std::vector<float>   *Mboy_qOverP;
  std::vector<float>   *Mboy_covr11;
  std::vector<float>   *Mboy_covr21;
  std::vector<float>   *Mboy_covr22;
  std::vector<float>   *Mboy_covr31;
  std::vector<float>   *Mboy_covr32;
  std::vector<float>   *Mboy_covr33;
  std::vector<float>   *Mboy_covr41;
  std::vector<float>   *Mboy_covr42;
  std::vector<float>   *Mboy_covr43;
  std::vector<float>   *Mboy_covr44;
  std::vector<float>   *Mboy_covr51;
  std::vector<float>   *Mboy_covr52;
  std::vector<float>   *Mboy_covr53;
  std::vector<float>   *Mboy_covr54;
  std::vector<float>   *Mboy_covr55;
  std::vector<float>   *Mboy_etcone10;
  std::vector<float>   *Mboy_etcone20;
  std::vector<float>   *Mboy_etcone30;
  std::vector<float>   *Mboy_etcone40;
  std::vector<float>   *Mboy_etcone;
  std::vector<float>   *Mboy_etcone50;
  std::vector<float>   *Mboy_etcone60;
  std::vector<float>   *Mboy_etcone70;
  std::vector<float>   *Mboy_nucone10;
  std::vector<float>   *Mboy_nucone20;
  std::vector<float>   *Mboy_nucone30;
  std::vector<float>   *Mboy_nucone40;
  std::vector<float>   *Mboy_nucone;
  std::vector<float>   *Mboy_nucone50;
  std::vector<float>   *Mboy_nucone60;
  std::vector<float>   *Mboy_nucone70;
  std::vector<float>   *Mboy_Eloss;
  std::vector<float>   *Mboy_ElossError;
  Int_t           Moore_nkine;
  std::vector<float>   *Moore_xvtxg;
  std::vector<float>   *Moore_yvtxg;
  std::vector<float>   *Moore_zvtxg;
  std::vector<float>   *Moore_a0g;
  std::vector<float>   *Moore_z0g;
  std::vector<float>   *Moore_phig;
  std::vector<float>   *Moore_cotthg;
  std::vector<float>   *Moore_ptig;
  std::vector<float>   *Moore_etag;
  std::vector<int>     *Moore_rassociated;
  std::vector<float>   *Moore_matchdg;
  std::vector<float>   *Moore_xextrg;
  std::vector<float>   *Moore_yextrg;
  std::vector<float>   *Moore_zextrg;
  std::vector<float>   *Moore_a0extrg;
  std::vector<float>   *Moore_z0extrg;
  std::vector<float>   *Moore_phiextrg;
  std::vector<float>   *Moore_cthextrg;
  std::vector<float>   *Moore_ptiextrg;
  std::vector<float>   *Moore_etaextrg;
  std::vector<float>   *Moore_barcodeg;
  std::vector<float>   *Moore_statusg;
  std::vector<float>   *Moore_elossg;
  std::vector<float>   *Moore_drphig;
  std::vector<float>   *Moore_drzg;
  std::vector<float>   *Moore_dphig;
  std::vector<float>   *Moore_dthetag;
  std::vector<float>   *Moore_a0pullg;
  std::vector<float>   *Moore_z0pullg;
  std::vector<float>   *Moore_phipullg;
  std::vector<float>   *Moore_cotthpullg;
  std::vector<float>   *Moore_ptipullg;
  Int_t           Moore_nmuon;
  std::vector<int>     *Moore_phih;
  std::vector<int>     *Moore_etah;
  std::vector<int>     *Moore_mdth;
  std::vector<int>     *Moore_cscetah;
  std::vector<int>     *Moore_cscphih;
  std::vector<int>     *Moore_rpcetah;
  std::vector<int>     *Moore_rpcphih;
  std::vector<int>     *Moore_tgcetah;
  std::vector<int>     *Moore_tgcphih;
  std::vector<int>     *Moore_stati;
  std::vector<int>     *Moore_statr;
  std::vector<float>   *Moore_xvtxr;
  std::vector<float>   *Moore_yvtxr;
  std::vector<float>   *Moore_zvtxr;
  std::vector<float>   *Moore_a0r;
  std::vector<float>   *Moore_z0r;
  std::vector<float>   *Moore_phir;
  std::vector<float>   *Moore_cotthr;
  std::vector<float>   *Moore_ptir;
  std::vector<float>   *Moore_etar;
  std::vector<float>   *Moore_chi2;
  std::vector<float>   *Moore_chi2pr;
  std::vector<float>   *Moore_origx;
  std::vector<float>   *Moore_origy;
  std::vector<float>   *Moore_origz;
  std::vector<float>   *Moore_covr11;
  std::vector<float>   *Moore_covr12;
  std::vector<float>   *Moore_covr22;
  std::vector<float>   *Moore_covr13;
  std::vector<float>   *Moore_covr23;
  std::vector<float>   *Moore_covr33;
  std::vector<float>   *Moore_covr14;
  std::vector<float>   *Moore_covr24;
  std::vector<float>   *Moore_covr34;
  std::vector<float>   *Moore_covr44;
  std::vector<float>   *Moore_covr15;
  std::vector<float>   *Moore_covr25;
  std::vector<float>   *Moore_covr35;
  std::vector<float>   *Moore_covr45;
  std::vector<float>   *Moore_covr55;
  UInt_t          conv_numVertices;
  std::vector<float>   *conv_vtx_x;
  std::vector<float>   *conv_vtx_y;
  std::vector<float>   *conv_vtx_z;
  std::vector<float>   *conv_is_conv;
  std::vector<float>   *conv_vtx_x_truth;
  std::vector<float>   *conv_vtx_y_truth;
  std::vector<float>   *conv_vtx_z_truth;
  std::vector<float>   *conv_sigvtx_x;
  std::vector<float>   *conv_sigvtx_y;
  std::vector<float>   *conv_sigvtx_z;
  std::vector<float>   *conv_vtxchi2;
  std::vector<float>   *conv_vtxndf;
  std::vector<float>   *conv_vtxchi2prob;
  std::vector<long>    *conv_numTracksPerVertex;
  std::vector<float>   *conv_track1_chi2;
  std::vector<float>   *conv_track2_chi2;
  std::vector<float>   *conv_track1_d0;
  std::vector<float>   *conv_track1_sigd0;
  std::vector<float>   *conv_track1_z0;
  std::vector<float>   *conv_track1_sigz0;
  std::vector<float>   *conv_track1_phi;
  std::vector<float>   *conv_track1_sigphi;
  std::vector<float>   *conv_track1_theta;
  std::vector<float>   *conv_track1_sigtheta;
  std::vector<float>   *conv_track1_qOverP;
  std::vector<float>   *conv_track1_sigqOverP;
  std::vector<float>   *conv_track1_momentum_x;
  std::vector<float>   *conv_track1_momentum_y;
  std::vector<float>   *conv_track1_momentum_z;
  std::vector<float>   *conv_track1_d0g;
  std::vector<float>   *conv_track1_sigd0g;
  std::vector<float>   *conv_track1_z0g;
  std::vector<float>   *conv_track1_sigz0g;
  std::vector<float>   *conv_track2_d0;
  std::vector<float>   *conv_track2_sigd0;
  std::vector<float>   *conv_track2_z0;
  std::vector<float>   *conv_track2_sigz0;
  std::vector<float>   *conv_track2_phi;
  std::vector<float>   *conv_track2_sigphi;
  std::vector<float>   *conv_track2_theta;
  std::vector<float>   *conv_track2_sigtheta;
  std::vector<float>   *conv_track2_qOverP;
  std::vector<float>   *conv_track2_sigqOverP;
  std::vector<float>   *conv_track2_momentum_x;
  std::vector<float>   *conv_track2_momentum_y;
  std::vector<float>   *conv_track2_momentum_z;
  std::vector<float>   *conv_track2_d0g;
  std::vector<float>   *conv_track2_sigd0g;
  std::vector<float>   *conv_track2_z0g;
  std::vector<float>   *conv_track2_sigz0g;
  UInt_t          conv_numOfTruthConversions;
  std::vector<float>   *conv_true_vtx_x;
  std::vector<float>   *conv_true_vtx_y;
  std::vector<float>   *conv_true_vtx_z;
  UInt_t          jetNumCJets;
  std::vector<double>  *jetEtaCJets;
  std::vector<double>  *jetPhiCJets;
  std::vector<double>  *jetECJets;
  std::vector<double>  *jetEemCJets;
  std::vector<double>  *jetEtCJets;
  std::vector<double>  *jetMCJets;
  std::vector<double>  *jetPxCJets;
  std::vector<double>  *jetPyCJets;
  std::vector<double>  *jetPzCJets;
  std::vector<long>    *jetSizeCJets;
  UInt_t          jetNumC4Jets;
  std::vector<double>  *jetEtaC4Jets;
  std::vector<double>  *jetPhiC4Jets;
  std::vector<double>  *jetEC4Jets;
  std::vector<double>  *jetEemC4Jets;
  std::vector<double>  *jetEtC4Jets;
  std::vector<double>  *jetMC4Jets;
  std::vector<double>  *jetPxC4Jets;
  std::vector<double>  *jetPyC4Jets;
  std::vector<double>  *jetPzC4Jets;
  std::vector<long>    *jetSizeC4Jets;
  UInt_t          jetNumKt6Jets;
  std::vector<double>  *jetEtaKt6Jets;
  std::vector<double>  *jetPhiKt6Jets;
  std::vector<double>  *jetEKt6Jets;
  std::vector<double>  *jetEemKt6Jets;
  std::vector<double>  *jetEtKt6Jets;
  std::vector<double>  *jetMKt6Jets;
  std::vector<double>  *jetPxKt6Jets;
  std::vector<double>  *jetPyKt6Jets;
  std::vector<double>  *jetPzKt6Jets;
  std::vector<long>    *jetSizeKt6Jets;
  UInt_t          jetNumKt4Jets;
  std::vector<double>  *jetEtaKt4Jets;
  std::vector<double>  *jetPhiKt4Jets;
  std::vector<double>  *jetEKt4Jets;
  std::vector<double>  *jetEemKt4Jets;
  std::vector<double>  *jetEtKt4Jets;
  std::vector<double>  *jetMKt4Jets;
  std::vector<double>  *jetPxKt4Jets;
  std::vector<double>  *jetPyKt4Jets;
  std::vector<double>  *jetPzKt4Jets;
  std::vector<long>    *jetSizeKt4Jets;
  UInt_t          jetNumCTopoJets;
  std::vector<double>  *jetEtaCTopoJets;
  std::vector<double>  *jetPhiCTopoJets;
  std::vector<double>  *jetECTopoJets;
  std::vector<double>  *jetEemCTopoJets;
  std::vector<double>  *jetEtCTopoJets;
  std::vector<double>  *jetMCTopoJets;
  std::vector<double>  *jetPxCTopoJets;
  std::vector<double>  *jetPyCTopoJets;
  std::vector<double>  *jetPzCTopoJets;
  std::vector<long>    *jetSizeCTopoJets;
  UInt_t          jetNumC4TopoJets;
  std::vector<double>  *jetEtaC4TopoJets;
  std::vector<double>  *jetPhiC4TopoJets;
  std::vector<double>  *jetEC4TopoJets;
  std::vector<double>  *jetEemC4TopoJets;
  std::vector<double>  *jetEtC4TopoJets;
  std::vector<double>  *jetMC4TopoJets;
  std::vector<double>  *jetPxC4TopoJets;
  std::vector<double>  *jetPyC4TopoJets;
  std::vector<double>  *jetPzC4TopoJets;
  std::vector<long>    *jetSizeC4TopoJets;
  UInt_t          jetNumKt6TopoJets;
  std::vector<double>  *jetEtaKt6TopoJets;
  std::vector<double>  *jetPhiKt6TopoJets;
  std::vector<double>  *jetEKt6TopoJets;
  std::vector<double>  *jetEemKt6TopoJets;
  std::vector<double>  *jetEtKt6TopoJets;
  std::vector<double>  *jetMKt6TopoJets;
  std::vector<double>  *jetPxKt6TopoJets;
  std::vector<double>  *jetPyKt6TopoJets;
  std::vector<double>  *jetPzKt6TopoJets;
  std::vector<long>    *jetSizeKt6TopoJets;
  UInt_t          jetNumKt4TopoJets;
  std::vector<double>  *jetEtaKt4TopoJets;
  std::vector<double>  *jetPhiKt4TopoJets;
  std::vector<double>  *jetEKt4TopoJets;
  std::vector<double>  *jetEemKt4TopoJets;
  std::vector<double>  *jetEtKt4TopoJets;
  std::vector<double>  *jetMKt4TopoJets;
  std::vector<double>  *jetPxKt4TopoJets;
  std::vector<double>  *jetPyKt4TopoJets;
  std::vector<double>  *jetPzKt4TopoJets;
  std::vector<long>    *jetSizeKt4TopoJets;
  UInt_t          jetNumCMcJets;
  std::vector<double>  *jetEtaCMcJets;
  std::vector<double>  *jetPhiCMcJets;
  std::vector<double>  *jetECMcJets;
  std::vector<double>  *jetEemCMcJets;
  std::vector<double>  *jetEtCMcJets;
  std::vector<double>  *jetMCMcJets;
  std::vector<double>  *jetPxCMcJets;
  std::vector<double>  *jetPyCMcJets;
  std::vector<double>  *jetPzCMcJets;
  std::vector<long>    *jetSizeCMcJets;
  UInt_t          jetNumC4McJets;
  std::vector<double>  *jetEtaC4McJets;
  std::vector<double>  *jetPhiC4McJets;
  std::vector<double>  *jetEC4McJets;
  std::vector<double>  *jetEemC4McJets;
  std::vector<double>  *jetEtC4McJets;
  std::vector<double>  *jetMC4McJets;
  std::vector<double>  *jetPxC4McJets;
  std::vector<double>  *jetPyC4McJets;
  std::vector<double>  *jetPzC4McJets;
  std::vector<long>    *jetSizeC4McJets;
  UInt_t          jetNumKt4McJets;
  std::vector<double>  *jetEtaKt4McJets;
  std::vector<double>  *jetPhiKt4McJets;
  std::vector<double>  *jetEKt4McJets;
  std::vector<double>  *jetEemKt4McJets;
  std::vector<double>  *jetEtKt4McJets;
  std::vector<double>  *jetMKt4McJets;
  std::vector<double>  *jetPxKt4McJets;
  std::vector<double>  *jetPyKt4McJets;
  std::vector<double>  *jetPzKt4McJets;
  std::vector<long>    *jetSizeKt4McJets;
  UInt_t          jetNumKt6McJets;
  std::vector<double>  *jetEtaKt6McJets;
  std::vector<double>  *jetPhiKt6McJets;
  std::vector<double>  *jetEKt6McJets;
  std::vector<double>  *jetEemKt6McJets;
  std::vector<double>  *jetEtKt6McJets;
  std::vector<double>  *jetMKt6McJets;
  std::vector<double>  *jetPxKt6McJets;
  std::vector<double>  *jetPyKt6McJets;
  std::vector<double>  *jetPzKt6McJets;
  std::vector<long>    *jetSizeKt6McJets;
  Int_t           tau_NCand;
  std::vector<float>   *tau_ET;
  std::vector<float>   *tau_eta;
  std::vector<float>   *tau_phi;
  std::vector<long>    *tau_nStrip;
  std::vector<float>   *tau_stripWidth2;
  std::vector<float>   *tau_EMRadius;
  std::vector<float>   *tau_IsoFrac;
  std::vector<long>    *tau_ntrack;
  std::vector<long>    *tau_charge;
  std::vector<float>   *tau_secVtxX;
  std::vector<float>   *tau_secVtxY;
  std::vector<float>   *tau_secVtxZ;
  std::vector<float>   *tau_secVtxXError;
  std::vector<float>   *tau_secVtxYError;
  std::vector<float>   *tau_secVtxZError;
  std::vector<float>   *tau_sumEMe;
  std::vector<float>   *tau_sumEMpx;
  std::vector<float>   *tau_sumEMpy;
  std::vector<float>   *tau_sumEMpz;
  std::vector<float>   *tau_energy;
  std::vector<float>   *tau_EMenergy;
  std::vector<long>    *tau_nEMCell;
  std::vector<float>   *tau_stripET;
  std::vector<float>   *tau_ptTrack1;
  std::vector<float>   *tau_etaTrack1;
  std::vector<float>   *tau_phiTrack1;
  std::vector<float>   *tau_distTrack1;
  std::vector<float>   *tau_ptTrack2;
  std::vector<float>   *tau_etaTrack2;
  std::vector<float>   *tau_phiTrack2;
  std::vector<float>   *tau_distTrack2;
  std::vector<float>   *tau_ptTrack3;
  std::vector<float>   *tau_etaTrack3;
  std::vector<float>   *tau_phiTrack3;
  std::vector<float>   *tau_distTrack3;
  std::vector<float>   *tau_etEMCalib;
  std::vector<float>   *tau_etHadCalib;
  std::vector<float>   *tau_Likelihood;
  std::vector<float>   *tau_tauJetLL;
  std::vector<float>   *tau_tauELL;
  std::vector<float>   *tau_tauJetNN;
  std::vector<float>   *tau_tauENN;
  std::vector<long>    *tau_nTracksdrdR;
  std::vector<long>    *tau_accept;
  Int_t           tau1p3p_NCand;
  std::vector<float>   *tau1p3p_ET;
  std::vector<float>   *tau1p3p_eta;
  std::vector<float>   *tau1p3p_phi;
  std::vector<long>    *tau1p3p_nStrip;
  std::vector<float>   *tau1p3p_stripWidth2;
  std::vector<float>   *tau1p3p_EMRadius;
  std::vector<float>   *tau1p3p_IsoFrac;
  std::vector<long>    *tau1p3p_ntrack;
  std::vector<long>    *tau1p3p_charge;
  std::vector<float>   *tau1p3p_secVtxX;
  std::vector<float>   *tau1p3p_secVtxY;
  std::vector<float>   *tau1p3p_secVtxZ;
  std::vector<float>   *tau1p3p_secVtxXError;
  std::vector<float>   *tau1p3p_secVtxYError;
  std::vector<float>   *tau1p3p_secVtxZError;
  std::vector<float>   *tau1p3p_sumEMe;
  std::vector<float>   *tau1p3p_sumEMpx;
  std::vector<float>   *tau1p3p_sumEMpy;
  std::vector<float>   *tau1p3p_sumEMpz;
  std::vector<long>    *tau1p3p_track1Nt;
  std::vector<long>    *tau1p3p_track2Nt;
  std::vector<long>    *tau1p3p_track3Nt;
  std::vector<float>   *tau1p3p_ptTrack1;
  std::vector<float>   *tau1p3p_ptTrack2;
  std::vector<float>   *tau1p3p_ptTrack3;
  std::vector<float>   *tau1p3p_track1ETChrgEM01;
  std::vector<float>   *tau1p3p_track2ETChrgEM01;
  std::vector<float>   *tau1p3p_track3ETChrgEM01;
  std::vector<float>   *tau1p3p_track1ETResChrgEMTrk;
  std::vector<float>   *tau1p3p_track2ETResChrgEMTrk;
  std::vector<float>   *tau1p3p_track3ETResChrgEMTrk;
  std::vector<float>   *tau1p3p_etEMCL;
  std::vector<float>   *tau1p3p_etNeuEM;
  std::vector<float>   *tau1p3p_etChrgEM;
  std::vector<float>   *tau1p3p_etResNeuEM;
  std::vector<float>   *tau1p3p_cellsChrgHAD;
  std::vector<float>   *tau1p3p_cellsOtherEM;
  std::vector<float>   *tau1p3p_cellsOtherHAD;
  std::vector<float>   *tau1p3p_ETeflow;
  std::vector<float>   *tau1p3p_ETcalo;
  std::vector<float>   *tau1p3p_MVisEflow;
  std::vector<float>   *tau1p3p_discriCut;
  std::vector<float>   *tau1p3p_discriNN;
  std::vector<float>   *tau1p3p_discriPDRS;
  std::vector<float>   *tau1p3p_MTrk3P;
  std::vector<long>    *tau1p3p_NAssocTracksCore;
  std::vector<long>    *tau1p3p_NAssocTracksIsol;
  std::vector<float>   *tau1p3p_RWidth2Trk3P;
  std::vector<float>   *tau1p3p_SignD0Trk3P;
  Int_t           eg_nc;
  std::vector<float>   *eg_e;
  std::vector<float>   *eg_eta;
  std::vector<float>   *eg_phi;
  std::vector<float>   *eg_cl_et;
  std::vector<float>   *eg_cl_eta;
  std::vector<float>   *eg_cl_phi;
  std::vector<float>   *eg_etap;
  std::vector<float>   *eg_zvertex;
  std::vector<float>   *eg_errz;
  std::vector<float>   *eg_depth;
  std::vector<float>   *eg_e0;
  std::vector<float>   *eg_e1;
  std::vector<float>   *eg_e2;
  std::vector<float>   *eg_e3;
  std::vector<float>   *eg_eta0;
  std::vector<float>   *eg_eta1;
  std::vector<float>   *eg_eta2;
  std::vector<float>   *eg_eta3;
  std::vector<float>   *eg_phi0;
  std::vector<float>   *eg_phi1;
  std::vector<float>   *eg_phi2;
  std::vector<float>   *eg_phi3;
  std::vector<float>   *eg_Etha1;
  std::vector<float>   *eg_Etha;
  std::vector<float>   *eg_Eha1;
  std::vector<float>   *eg_F1;
  std::vector<float>   *eg_F3;
  std::vector<float>   *eg_E233;
  std::vector<float>   *eg_E237;
  std::vector<float>   *eg_E277;
  std::vector<float>   *eg_Weta1;
  std::vector<float>   *eg_Weta2;
  std::vector<float>   *eg_E2ts1;
  std::vector<float>   *eg_E2tsts1;
  std::vector<float>   *eg_Widths1;
  std::vector<float>   *eg_Widths2;
  std::vector<float>   *eg_poscs1;
  std::vector<float>   *eg_poscs2;
  std::vector<float>   *eg_Barys1;
  std::vector<float>   *eg_Wtots1;
  std::vector<float>   *eg_Emins1;
  std::vector<float>   *eg_Emaxs1;
  std::vector<float>   *eg_Fracs1;
  std::vector<float>   *eg_EtCone45;
  std::vector<float>   *eg_EtCone20;
  std::vector<float>   *eg_EtCone30;
  std::vector<float>   *eg_EtCone40;
  std::vector<long>    *eg_IsEM;
  std::vector<float>   *eg_epiNN;
  std::vector<double>  *eg_EMWeight;
  std::vector<double>  *eg_PionWeight;
  std::vector<double>  *eg_Hmatrix;
  std::vector<long>    *eg_IsEMse;
  std::vector<float>   *eg_epiNNse;
  std::vector<double>  *eg_EMWeightse;
  std::vector<double>  *eg_PionWeightse;
  std::vector<float>   *eg_F1core;
  std::vector<float>   *eg_F3core;
  std::vector<float>   *eg_Asy1;
  std::vector<float>   *eg_Pos7;
  std::vector<float>   *eg_Isol;
  std::vector<float>   *eg_Ecore;
  std::vector<float>   *eg_E011;
  std::vector<float>   *eg_E033;
  std::vector<float>   *eg_E131;
  std::vector<float>   *eg_E1153;
  std::vector<float>   *eg_E235;
  std::vector<float>   *eg_E255;
  std::vector<float>   *eg_E333;
  std::vector<float>   *eg_E335;
  std::vector<float>   *eg_E337;
  std::vector<float>   *eg_E377;
  std::vector<long>    *eg_trkmatchnt;
  std::vector<long>    *eg_primary;
  std::vector<long>    *eg_numtracks;
  std::vector<float>   *eg_eoverp;
  std::vector<float>   *eg_etacorr;
  std::vector<float>   *eg_deta0;
  std::vector<float>   *eg_dphi0;
  std::vector<float>   *eg_deta1;
  std::vector<float>   *eg_dphi1;
  std::vector<float>   *eg_deta2;
  std::vector<float>   *eg_dphi2;
  std::vector<float>   *eg_deta3;
  std::vector<float>   *eg_dphi3;
  std::vector<long>    *eg_trkopt;
  std::vector<float>   *eg_trkinvpTV;
  std::vector<float>   *eg_trkcotThV;
  std::vector<float>   *eg_trkphiV;
  std::vector<long>    *eg_convTrkMatch;
  std::vector<long>    *eg_convAngleMatch;
  Int_t           eg_nc_SE;
  std::vector<float>   *eg_e_SE;
  std::vector<float>   *eg_eta_SE;
  std::vector<float>   *eg_phi_SE;
  std::vector<float>   *eg_cl_et_SE;
  std::vector<float>   *eg_cl_eta_SE;
  std::vector<float>   *eg_cl_phi_SE;
  std::vector<float>   *eg_etap_SE;
  std::vector<float>   *eg_zvertex_SE;
  std::vector<float>   *eg_errz_SE;
  std::vector<float>   *eg_depth_SE;
  std::vector<float>   *eg_e0_SE;
  std::vector<float>   *eg_e1_SE;
  std::vector<float>   *eg_e2_SE;
  std::vector<float>   *eg_e3_SE;
  std::vector<float>   *eg_eta0_SE;
  std::vector<float>   *eg_eta1_SE;
  std::vector<float>   *eg_eta2_SE;
  std::vector<float>   *eg_eta3_SE;
  std::vector<float>   *eg_phi0_SE;
  std::vector<float>   *eg_phi1_SE;
  std::vector<float>   *eg_phi2_SE;
  std::vector<float>   *eg_phi3_SE;
  std::vector<float>   *eg_Etha1_SE;
  std::vector<float>   *eg_Etha_SE;
  std::vector<float>   *eg_Eha1_SE;
  std::vector<float>   *eg_F1_SE;
  std::vector<float>   *eg_F3_SE;
  std::vector<float>   *eg_E233_SE;
  std::vector<float>   *eg_E237_SE;
  std::vector<float>   *eg_E277_SE;
  std::vector<float>   *eg_Weta1_SE;
  std::vector<float>   *eg_Weta2_SE;
  std::vector<float>   *eg_E2ts1_SE;
  std::vector<float>   *eg_E2tsts1_SE;
  std::vector<float>   *eg_Widths1_SE;
  std::vector<float>   *eg_Widths2_SE;
  std::vector<float>   *eg_poscs1_SE;
  std::vector<float>   *eg_poscs2_SE;
  std::vector<float>   *eg_Barys1_SE;
  std::vector<float>   *eg_Wtots1_SE;
  std::vector<float>   *eg_Emins1_SE;
  std::vector<float>   *eg_Emaxs1_SE;
  std::vector<float>   *eg_Fracs1_SE;
  std::vector<float>   *eg_EtCone45_SE;
  std::vector<float>   *eg_EtCone20_SE;
  std::vector<float>   *eg_EtCone30_SE;
  std::vector<float>   *eg_EtCone40_SE;
  std::vector<long>    *eg_IsEM_SE;
  std::vector<float>   *eg_epiNN_SE;
  std::vector<double>  *eg_EMWeight_SE;
  std::vector<double>  *eg_PionWeight_SE;
  std::vector<double>  *eg_Hmatrix_SE;
  std::vector<long>    *eg_IsEMse_SE;
  std::vector<float>   *eg_epiNNse_SE;
  std::vector<double>  *eg_EMWeightse_SE;
  std::vector<double>  *eg_PionWeightse_SE;
  std::vector<float>   *eg_F1core_SE;
  std::vector<float>   *eg_F3core_SE;
  std::vector<float>   *eg_Asy1_SE;
  std::vector<float>   *eg_Pos7_SE;
  std::vector<float>   *eg_Isol_SE;
  std::vector<float>   *eg_Ecore_SE;
  std::vector<float>   *eg_E011_SE;
  std::vector<float>   *eg_E033_SE;
  std::vector<float>   *eg_E131_SE;
  std::vector<float>   *eg_E1153_SE;
  std::vector<float>   *eg_E235_SE;
  std::vector<float>   *eg_E255_SE;
  std::vector<float>   *eg_E333_SE;
  std::vector<float>   *eg_E335_SE;
  std::vector<float>   *eg_E337_SE;
  std::vector<float>   *eg_E377_SE;
  std::vector<long>    *eg_trkmatchnt_SE;
  std::vector<long>    *eg_primary_SE;
  std::vector<long>    *eg_numtracks_SE;
  std::vector<float>   *eg_eoverp_SE;
  std::vector<float>   *eg_etacorr_SE;
  std::vector<float>   *eg_deta0_SE;
  std::vector<float>   *eg_dphi0_SE;
  std::vector<float>   *eg_deta1_SE;
  std::vector<float>   *eg_dphi1_SE;
  std::vector<float>   *eg_deta2_SE;
  std::vector<float>   *eg_dphi2_SE;
  std::vector<float>   *eg_deta3_SE;
  std::vector<float>   *eg_dphi3_SE;
  std::vector<long>    *eg_trkopt_SE;
  std::vector<float>   *eg_trkinvpTV_SE;
  std::vector<float>   *eg_trkcotThV_SE;
  std::vector<float>   *eg_trkphiV_SE;
  std::vector<long>    *eg_convTrkMatch_SE;
  std::vector<long>    *eg_convAngleMatch_SE;
  Int_t           egtruth_nc;
  std::vector<float>   *egtruth_et;
  std::vector<float>   *egtruth_eta;
  std::vector<float>   *egtruth_phi;
  std::vector<long>    *egtruth_id;
  std::vector<long>    *egtruth_barcode;
  std::vector<long>    *egtruth_truthnt;
  std::vector<float>   *egtruth_etIsol;
  std::vector<float>   *egtruth_etaCalo;
  std::vector<float>   *egtruth_phiCalo;
  Int_t           NTileMuonTag;
  std::vector<float>   *EtaTileMuonTag;
  std::vector<float>   *PhiTileMuonTag;
  std::vector<std::vector<float> > *EnergyVecTileMuonTag;
  std::vector<float>   *LabelTileMuonTag;
  Int_t           Muid_nmuonms;
  std::vector<float>   *Muid_a0rms;
  std::vector<float>   *Muid_z0rms;
  std::vector<float>   *Muid_phirms;
  std::vector<float>   *Muid_thetarms;
  std::vector<float>   *Muid_etarms;
  std::vector<float>   *Muid_pirms;
  std::vector<float>   *Muid_chi2ms;
  std::vector<long>    *Muid_mdtms;
  std::vector<long>    *Muid_cscetams;
  std::vector<long>    *Muid_cscphims;
  std::vector<long>    *Muid_rpcetams;
  std::vector<long>    *Muid_rpcphims;
  std::vector<long>    *Muid_tgcetams;
  std::vector<long>    *Muid_tgcphims;
  std::vector<float>   *Muid_covr11ms;
  std::vector<float>   *Muid_covr12ms;
  std::vector<float>   *Muid_covr22ms;
  std::vector<float>   *Muid_covr13ms;
  std::vector<float>   *Muid_covr23ms;
  std::vector<float>   *Muid_covr33ms;
  std::vector<float>   *Muid_covr14ms;
  std::vector<float>   *Muid_covr24ms;
  std::vector<float>   *Muid_covr34ms;
  std::vector<float>   *Muid_covr44ms;
  std::vector<float>   *Muid_covr15ms;
  std::vector<float>   *Muid_covr25ms;
  std::vector<float>   *Muid_covr35ms;
  std::vector<float>   *Muid_covr45ms;
  std::vector<float>   *Muid_covr55ms;
  Int_t           Muid_nmuonmu;
  std::vector<float>   *Muid_prelossmu;
  std::vector<float>   *Muid_erelossmu;
  std::vector<long>    *Muid_isparelossmu;
  std::vector<float>   *Muid_a0rmu;
  std::vector<float>   *Muid_z0rmu;
  std::vector<float>   *Muid_phirmu;
  std::vector<float>   *Muid_thetarmu;
  std::vector<float>   *Muid_etarmu;
  std::vector<float>   *Muid_pirmu;
  std::vector<float>   *Muid_chi2mu;
  std::vector<float>   *Muid_covr11mu;
  std::vector<float>   *Muid_covr12mu;
  std::vector<float>   *Muid_covr22mu;
  std::vector<float>   *Muid_covr13mu;
  std::vector<float>   *Muid_covr23mu;
  std::vector<float>   *Muid_covr33mu;
  std::vector<float>   *Muid_covr14mu;
  std::vector<float>   *Muid_covr24mu;
  std::vector<float>   *Muid_covr34mu;
  std::vector<float>   *Muid_covr44mu;
  std::vector<float>   *Muid_covr15mu;
  std::vector<float>   *Muid_covr25mu;
  std::vector<float>   *Muid_covr35mu;
  std::vector<float>   *Muid_covr45mu;
  std::vector<float>   *Muid_covr55mu;
  std::vector<long>    *Muid_mslinkmu;
  Int_t           Muid_nmuoncb;
  std::vector<float>   *Muid_chi2mcb;
  std::vector<long>    *Muid_loosecb;
  std::vector<float>   *Muid_a0rcb;
  std::vector<float>   *Muid_z0rcb;
  std::vector<float>   *Muid_phircb;
  std::vector<float>   *Muid_thetarcb;
  std::vector<float>   *Muid_etarcb;
  std::vector<float>   *Muid_pircb;
  std::vector<float>   *Muid_chi2cb;
  std::vector<float>   *Muid_chi2prcb;
  std::vector<float>   *Muid_covr11cb;
  std::vector<float>   *Muid_covr12cb;
  std::vector<float>   *Muid_covr22cb;
  std::vector<float>   *Muid_covr13cb;
  std::vector<float>   *Muid_covr23cb;
  std::vector<float>   *Muid_covr33cb;
  std::vector<float>   *Muid_covr14cb;
  std::vector<float>   *Muid_covr24cb;
  std::vector<float>   *Muid_covr34cb;
  std::vector<float>   *Muid_covr44cb;
  std::vector<float>   *Muid_covr15cb;
  std::vector<float>   *Muid_covr25cb;
  std::vector<float>   *Muid_covr35cb;
  std::vector<float>   *Muid_covr45cb;
  std::vector<float>   *Muid_covr55cb;
  std::vector<long>    *Muid_mslinkcb;
  std::vector<long>    *Muid_exlinkcb;
  std::vector<long>    *Muid_blayercb;
  std::vector<long>    *Muid_pixelcb;
  std::vector<long>    *Muid_sctcb;
  std::vector<long>    *Muid_trtcb;
  std::vector<long>    *Muid_trthighcb;
  std::vector<float>   *Muid_a0rid;
  std::vector<float>   *Muid_z0rid;
  std::vector<float>   *Muid_phirid;
  std::vector<float>   *Muid_thetarid;
  std::vector<float>   *Muid_etarid;
  std::vector<float>   *Muid_pirid;
  std::vector<float>   *Muid_chi2id;
  std::vector<float>   *Muid_pulla0cb;
  std::vector<float>   *Muid_pullz0cb;
  std::vector<float>   *Muid_pullphicb;
  std::vector<float>   *Muid_pullthecb;
  std::vector<float>   *Muid_pullmomcb;
  UInt_t          staco_nmuon;
  std::vector<float>   *staco_A0;
  std::vector<float>   *staco_Z;
  std::vector<float>   *staco_Phi;
  std::vector<float>   *staco_Theta;
  std::vector<float>   *staco_qOverP;
  std::vector<float>   *staco_chi2;
  std::vector<float>   *staco_covr11;
  std::vector<float>   *staco_covr21;
  std::vector<float>   *staco_covr22;
  std::vector<float>   *staco_covr31;
  std::vector<float>   *staco_covr32;
  std::vector<float>   *staco_covr33;
  std::vector<float>   *staco_covr41;
  std::vector<float>   *staco_covr42;
  std::vector<float>   *staco_covr43;
  std::vector<float>   *staco_covr44;
  std::vector<float>   *staco_covr51;
  std::vector<float>   *staco_covr52;
  std::vector<float>   *staco_covr53;
  std::vector<float>   *staco_covr54;
  std::vector<float>   *staco_covr55;
  std::vector<float>   *staco_A0MS;
  std::vector<float>   *staco_ZMS;
  std::vector<float>   *staco_PhiMS;
  std::vector<float>   *staco_ThetaMS;
  std::vector<float>   *staco_qOverPMS;
  std::vector<float>   *staco_A0ID;
  std::vector<float>   *staco_ZID;
  std::vector<float>   *staco_PhiID;
  std::vector<float>   *staco_ThetaID;
  std::vector<float>   *staco_qOverPID;
  UInt_t          MuTag_nmuon;
  std::vector<float>   *MuTag_A0;
  std::vector<float>   *MuTag_Z;
  std::vector<float>   *MuTag_Phi;
  std::vector<float>   *MuTag_Theta;
  std::vector<float>   *MuTag_qOverP;
  std::vector<float>   *MuTag_covr11;
  std::vector<float>   *MuTag_covr21;
  std::vector<float>   *MuTag_covr22;
  std::vector<float>   *MuTag_covr31;
  std::vector<float>   *MuTag_covr32;
  std::vector<float>   *MuTag_covr33;
  std::vector<float>   *MuTag_covr41;
  std::vector<float>   *MuTag_covr42;
  std::vector<float>   *MuTag_covr43;
  std::vector<float>   *MuTag_covr44;
  std::vector<float>   *MuTag_covr51;
  std::vector<float>   *MuTag_covr52;
  std::vector<float>   *MuTag_covr53;
  std::vector<float>   *MuTag_covr54;
  std::vector<float>   *MuTag_covr55;
  Float_t         MET_ExMiss;
  Float_t         MET_EyMiss;
  Float_t         MET_EtSum;
  Float_t         MET_ExMissTopo;
  Float_t         MET_EyMissTopo;
  Float_t         MET_EtSumTopo;
  Float_t         MET_ExMissCorr;
  Float_t         MET_EyMissCorr;
  Float_t         MET_EtSumCorr;
  Float_t         MET_ExMissLocHadTopo;
  Float_t         MET_EyMissLocHadTopo;
  Float_t         MET_EtSumLocHadTopo;
  Float_t         MET_ExMissCorrTopo;
  Float_t         MET_EyMissCorrTopo;
  Float_t         MET_EtSumCorrTopo;
  Float_t         MET_ExMissMu;
  Float_t         MET_EyMissMu;
  Float_t         MET_EtSumMu;
  Float_t         MET_ExMissMuBoy;
  Float_t         MET_EyMissMuBoy;
  Float_t         MET_EtSumMuBoy;
  Float_t         MET_ExMissFinal;
  Float_t         MET_EyMissFinal;
  Float_t         MET_EtSumFinal;
  Float_t         MET_ExMissRefFinal;
  Float_t         MET_EyMissRefFinal;
  Float_t         MET_EtSumRefFinal;
  Float_t         MET_ExMissCryo;
  Float_t         MET_EyMissCryo;
  Float_t         MET_EtSumCryo;
  Float_t         MET_ExMissCryoCone;
  Float_t         MET_EyMissCryoCone;
  Float_t         MET_EtSumCryoCone;
  Float_t         MET_ExMissLocHadTopoObj;
  Float_t         MET_EyMissLocHadTopoObj;
  Float_t         MET_EtSumLocHadTopoObj;
  Float_t         MET_ExMissTopoObj;
  Float_t         MET_EyMissTopoObj;
  Float_t         MET_EtSumTopoObj;
  Float_t         MET_ExMissRefEle;
  Float_t         MET_EyMissRefEle;
  Float_t         MET_EtSumRefEle;
  Float_t         MET_ExMissRefMuo;
  Float_t         MET_EyMissRefMuo;
  Float_t         MET_EtSumRefMuo;
  Float_t         MET_ExMissRefJet;
  Float_t         MET_EyMissRefJet;
  Float_t         MET_EtSumRefJet;
  Float_t         MET_ExMissCellOut;
  Float_t         MET_EyMissCellOut;
  Float_t         MET_EtSumCellOut;
  Float_t         MET_ExFCALCorrTopo;
  Float_t         MET_EyFCALCorrTopo;
  Float_t         MET_EtSumFCALCorrTopo;
  Float_t         MET_ExFCALlocHadTopo;
  Float_t         MET_EyFCALlocHadTopo;
  Float_t         MET_EtSumFCALlocHadTopo;
  Float_t         MET_ExPEMB;
  Float_t         MET_EyPEMB;
  Float_t         MET_EtSumPEMB;
  Int_t           MET_nCellPEMB;
  Float_t         MET_ExEMB;
  Float_t         MET_EyEMB;
  Float_t         MET_EtSumEMB;
  Int_t           MET_nCellEMB;
  Float_t         MET_ExPEMEC;
  Float_t         MET_EyPEMEC;
  Float_t         MET_EtSumPEMEC;
  Int_t           MET_nCellPEMEC;
  Float_t         MET_ExEMEC;
  Float_t         MET_EyEMEC;
  Float_t         MET_EtSumEMEC;
  Int_t           MET_nCellEMEC;
  Float_t         MET_ExTILE;
  Float_t         MET_EyTILE;
  Float_t         MET_EtSumTILE;
  Int_t           MET_nCellTILE;
  Float_t         MET_ExHEC;
  Float_t         MET_EyHEC;
  Float_t         MET_EtSumHEC;
  Int_t           MET_nCellHEC;
  Float_t         MET_ExFCAL;
  Float_t         MET_EyFCAL;
  Float_t         MET_EtSumFCAL;
  Int_t           MET_nCellFCAL;
  Float_t         MET_ExBARReg;
  Float_t         MET_EyBARReg;
  Float_t         MET_EtSumBARReg;
  Float_t         MET_ExECReg;
  Float_t         MET_EyECReg;
  Float_t         MET_EtSumECReg;
  Float_t         MET_ExFCALReg;
  Float_t         MET_EyFCALReg;
  Float_t         MET_EtSumFCALReg;
  Float_t         MET_ExTruthInt;
  Float_t         MET_EyTruthInt;
  Float_t         MET_EtSumTruthInt;
  Float_t         MET_ExTruthNonInt;
  Float_t         MET_EyTruthNonInt;
  Float_t         MET_EtSumTruthNonInt;
  Float_t         MET_ExTruthIntCentral;
  Float_t         MET_EyTruthIntCentral;
  Float_t         MET_EtSumTruthIntCentral;
  Float_t         MET_ExTruthIntFwd;
  Float_t         MET_EyTruthIntFwd;
  Float_t         MET_EtSumTruthIntFwd;
  Float_t         MET_ExTruthMuons;
  Float_t         MET_EyTruthMuons;
  Float_t         MET_EtSumTruthMuons;
  Float_t         Obj_ExMissEle;
  Float_t         Obj_EyMissEle;
  Float_t         Obj_EtSumEle;
  Float_t         Obj_ExMissMu;
  Float_t         Obj_EyMissMu;
  Float_t         Obj_EtSumMu;
  Float_t         Obj_ExMissJet;
  Float_t         Obj_EyMissJet;
  Float_t         Obj_EtSumJet;
  Float_t         Obj_ExMissIdTrk;
  Float_t         Obj_EyMissIdTrk;
  Float_t         Obj_EtSumIdTrk;
  Float_t         Obj_ExMissMJet;
  Float_t         Obj_EyMissMJet;
  Float_t         Obj_EtSumMJet;
  Float_t         Obj_ExMissRest;
  Float_t         Obj_EyMissRest;
  Float_t         Obj_EtSumRest;
  Float_t         Obj_ExMissFinal;
  Float_t         Obj_EyMissFinal;
  Float_t         Obj_EtSumFinal;
  Float_t         L1EM_Scale;
  Int_t           L1Em_nRoI;
  std::vector<long>    *L1Em_RoIWord;
  std::vector<float>   *L1Em_Core;
  std::vector<float>   *L1Em_EmClus;
  std::vector<float>   *L1Em_TauClus;
  std::vector<float>   *L1Em_EmIsol;
  std::vector<float>   *L1Em_HdIsol;
  std::vector<float>   *L1Em_HdCore;
  std::vector<long>    *L1Em_EmTauThresh;
  std::vector<float>   *L1Em_eta;
  std::vector<float>   *L1Em_phi;
  Int_t           L1Jet_nRoI;
  std::vector<long>    *L1Jet_JetRoIWord;
  std::vector<float>   *L1Jet_ET4x4;
  std::vector<float>   *L1Jet_ET6x6;
  std::vector<float>   *L1Jet_ET8x8;
  std::vector<long>    *L1Jet_Thresh;
  std::vector<float>   *L1Jet_eta;
  std::vector<float>   *L1Jet_phi;
  Long64_t        L1ET_EtMissHits;
  Long64_t        L1ET_EtSumHits;
  Float_t         L1ET_Ex;
  Float_t         L1ET_Ey;
  Float_t         L1ET_EtMiss;
  Float_t         L1ET_EtSum;
  Long64_t        L1ET_JetEtHits;
  Float_t         L1ET_JetEtSum;
  UInt_t          canMulti;
  UInt_t          nDataWord;
  std::vector<unsigned long> *dataWord;
  UInt_t          nCTP_ROI;
  std::vector<unsigned long> *CTP_ROI;
  UInt_t          nMuCTPI_ROI;
  std::vector<unsigned long> *muCTPI_ROI;
  UInt_t          nEMTau_ROI;
  std::vector<unsigned long> *EMTau_ROI;
  UInt_t          nJetEnergy_ROI;
  std::vector<unsigned long> *JetEnergy_ROI;
  UInt_t          T2CaNclus;
  UInt_t          T2CaTauNclus;
  std::vector<float>   *T2CaEmE;
  std::vector<float>   *T2CaRawEmE;
  std::vector<float>   *T2CaEmES0;
  std::vector<float>   *T2CaEmES1;
  std::vector<float>   *T2CaEmES2;
  std::vector<float>   *T2CaEmES3;
  std::vector<float>   *T2CaHadE;
  std::vector<float>   *T2CaRawHadE;
  std::vector<float>   *T2CaHadES0;
  std::vector<float>   *T2CaHadES1;
  std::vector<float>   *T2CaHadES2;
  std::vector<float>   *T2CaHadES3;
  std::vector<float>   *T2CaRcore;
  std::vector<float>   *T2CaEratio;
  std::vector<float>   *T2CaWidth;
  std::vector<float>   *T2CaF73;
  std::vector<float>   *T2CaEta;
  std::vector<float>   *T2CaPhi;
  std::vector<float>   *T2CaRawEta;
  std::vector<float>   *T2CaRawPhi;
  std::vector<float>   *T2CaL1Sim_Eta;
  std::vector<float>   *T2CaL1Sim_Phi;
  std::vector<float>   *T2CaL1Sim_EmClus;
  std::vector<float>   *T2CaL1Sim_EmIsol;
  std::vector<float>   *T2CaL1Sim_HdCore;
  std::vector<float>   *T2CaL1Sim_HdIsol;
  std::vector<long>    *T2CaL1Sim_nRoIperRegion;
  std::vector<long>    *T2CaRoIword;
  std::vector<float>   *T2CaTauEta;
  std::vector<float>   *T2CaTauPhi;
  std::vector<float>   *T2CaEMES0_nar;
  std::vector<float>   *T2CaEMES0_wid;
  std::vector<float>   *T2CaEMES1_nar;
  std::vector<float>   *T2CaEMES1_wid;
  std::vector<float>   *T2CaEMES2_nar;
  std::vector<float>   *T2CaEMES2_wid;
  std::vector<float>   *T2CaEMES3_nar;
  std::vector<float>   *T2CaEMES3_wid;
  std::vector<float>   *T2CaHADES1_nar;
  std::vector<float>   *T2CaEHADS1_wid;
  std::vector<float>   *T2CaHADES2_nar;
  std::vector<float>   *T2CaEHADS2_wid;
  std::vector<float>   *T2CaHADES3_nar;
  std::vector<float>   *T2CaEHADS3_wid;
  std::vector<float>   *T2CaEMrad0;
  std::vector<float>   *T2CaEMrad1;
  std::vector<float>   *T2CaEMrad2;
  std::vector<float>   *T2CaEMrad3;
  std::vector<float>   *T2CaEMwid0;
  std::vector<float>   *T2CaEMwid1;
  std::vector<float>   *T2CaEMwid2;
  std::vector<float>   *T2CaEMwid3;
  std::vector<float>   *T2CaHADwid1;
  std::vector<float>   *T2CaHADwid2;
  std::vector<float>   *T2CaHADwid3;
  std::vector<float>   *T2CaEMenorm0;
  std::vector<float>   *T2CaEMenorm1;
  std::vector<float>   *T2CaEMenorm2;
  std::vector<float>   *T2CaEMenorm3;
  std::vector<float>   *T2CaHADenorm1;
  std::vector<float>   *T2CaHADenorm2;
  std::vector<float>   *T2CaHADenorm3;
  std::vector<float>   *T2CaNumStripCells;
  std::vector<float>   *T2CaEnergyCalib;
  std::vector<float>   *T2CaEMEnergyCalib;
  std::vector<float>   *T2CaIsoFrac;
  UInt_t          TrigNClus;
  std::vector<float>   *TrigClusEta;
  std::vector<float>   *TrigClusPhi;
  std::vector<float>   *TrigClusEtaRef;
  std::vector<float>   *TrigClusPhiRef;
  std::vector<float>   *TrigClusEt;
  std::vector<float>   *TrigClusE;
  std::vector<float>   *TrigClusEtaPresh;
  std::vector<float>   *TrigClusPhiPresh;
  std::vector<float>   *TrigClusEPresh;
  std::vector<float>   *TrigClusEta1stS;
  std::vector<float>   *TrigClusPhi1stS;
  std::vector<float>   *TrigClusE1stS;
  std::vector<float>   *TrigClusEta2ndS;
  std::vector<float>   *TrigClusPhi2ndS;
  std::vector<float>   *TrigClusE2ndS;
  std::vector<float>   *TrigClusEta3rdS;
  std::vector<float>   *TrigClusPhi3rdS;
  std::vector<float>   *TrigClusE3rdS;
  std::vector<std::string>  *TrigClusMaker;
  UInt_t          EFTauNCand;
  std::vector<float>   *EFTauEnergy;
  std::vector<float>   *EFTauNRoI;
  std::vector<float>   *EFTauEMenergy;
  std::vector<float>   *EFTauET;
  std::vector<float>   *EFTauEta;
  std::vector<float>   *EFTauPhi;
  std::vector<float>   *EFTauCaloEta;
  std::vector<float>   *EFTauCaloPhi;
  std::vector<long>    *EFTauNStrip;
  std::vector<long>    *EFTauNEMCell;
  std::vector<float>   *EFTauStripET;
  std::vector<float>   *EFTauStripWidth2;
  std::vector<float>   *EFTauEMRadius;
  std::vector<float>   *EFTauIsoFrac;
  std::vector<long>    *EFTauNtrack;
  std::vector<long>    *EFTauCharge;
  std::vector<float>   *EFTauPtTrack1;
  std::vector<float>   *EFTauEtaTrack1;
  std::vector<float>   *EFTauPhiTrack1;
  std::vector<float>   *EFTauDistTrack1;
  std::vector<float>   *EFTauPtTrack2;
  std::vector<float>   *EFTauEtaTrack2;
  std::vector<float>   *EFTauPhiTrack2;
  std::vector<float>   *EFTauDistTrack2;
  std::vector<float>   *EFTauPtTrack3;
  std::vector<float>   *EFTauEtaTrack3;
  std::vector<float>   *EFTauPhiTrack3;
  std::vector<float>   *EFTauDistTrack3;
  std::vector<float>   *EFTauSumEMe;
  std::vector<float>   *EFTauSumEMpx;
  std::vector<float>   *EFTauSumEMpy;
  std::vector<float>   *EFTauSumEMpz;
  std::vector<float>   *EFTauEtEMCalib;
  std::vector<float>   *EFTauEtHadCalib;
  UInt_t          T2CaNjets;
  std::vector<float>   *T2CaJetE;
  std::vector<float>   *T2CaJeteta;
  std::vector<float>   *T2CaJetphi;
  std::vector<float>   *T2CaJetConeR;
  std::vector<float>   *T2CaJetroi;
  UInt_t          TrigNJet;
  std::vector<float>   *TrigJetEt;
  std::vector<float>   *TrigJetEta;
  std::vector<float>   *TrigJetPhi;
  std::vector<float>   *TrigJetE;
  std::vector<float>   *TrigJetM;
  std::vector<std::string>  *TrigJetMaker;
  UInt_t          T2IdNtracks;
  std::vector<long>    *T2IdRoiID;
  std::vector<long>    *T2IdAlgo;
  std::vector<float>   *T2IdPt;
  std::vector<float>   *T2IdPhi0;
  std::vector<float>   *T2IdZ0;
  std::vector<float>   *T2IdD0;
  std::vector<float>   *T2IdPhic;
  std::vector<float>   *T2IdEta;
  std::vector<float>   *T2IdErrPt;
  std::vector<float>   *T2IdErrPhi0;
  std::vector<float>   *T2IdErrEta;
  std::vector<float>   *T2IdErrD0;
  std::vector<float>   *T2IdErrZ0;
  std::vector<float>   *T2IdChi2;
  std::vector<long>    *T2IdNDoF;
  std::vector<long>    *T2IdNSihits;
  std::vector<long>    *T2IdNTrthits;
  std::vector<long>    *T2IdHPatt;
  std::vector<long>    *T2IdNstraw;
  std::vector<long>    *T2IdNtime;
  std::vector<long>    *T2IdNtr;
  std::vector<long>    *T2IdLastPlane;
  std::vector<long>    *T2IdFirstPlane;
  std::vector<long>    *T2IdNkineHit;
  std::vector<long>    *T2IdKineRef;
  std::vector<long>    *T2IdKineEnt;
  std::vector<long>    *T2IdNkineHitTRT;
  std::vector<long>    *T2IdKineRefTRT;
  std::vector<long>    *T2IdKineEntTRT;
  UInt_t          T2NVtx;
  std::vector<float>   *T2zVertex;
  Int_t           TrkEF_totalNumTracks;
  std::vector<long>    *TrkEF_RoIId;
  std::vector<float>   *TrkEF_d0;
  std::vector<float>   *TrkEF_z0;
  std::vector<float>   *TrkEF_phi;
  std::vector<float>   *TrkEF_eta;
  std::vector<float>   *TrkEF_qOverP;
  std::vector<float>   *TrkEF_Chi2;
  std::vector<long>    *TrkEF_Ndf;
  std::vector<float>   *TrkEF_sigd0;
  std::vector<float>   *TrkEF_sigz0;
  std::vector<float>   *TrkEF_sigpt;
  std::vector<float>   *TrkEF_sigphi;
  std::vector<float>   *TrkEF_sigeta;
  std::vector<float>   *TrkEF_sigqOverP;
  std::vector<float>   *TrkEF_covVert21;
  std::vector<float>   *TrkEF_covVert31;
  std::vector<float>   *TrkEF_covVert32;
  std::vector<float>   *TrkEF_covVert41;
  std::vector<float>   *TrkEF_covVert42;
  std::vector<float>   *TrkEF_covVert43;
  std::vector<float>   *TrkEF_covVert51;
  std::vector<float>   *TrkEF_covVert52;
  std::vector<float>   *TrkEF_covVert53;
  std::vector<float>   *TrkEF_covVert54;
  std::vector<float>   *TrkEF_px;
  std::vector<float>   *TrkEF_py;
  std::vector<float>   *TrkEF_pz;
  std::vector<float>   *TrkEF_pt;
  std::vector<long>    *TrkEF_numberOfBLayerHits;
  std::vector<long>    *TrkEF_numberOfPixelHits;
  std::vector<long>    *TrkEF_numberOfSCTHits;
  std::vector<long>    *TrkEF_numberOfTRTHits;
  std::vector<long>    *TrkEF_numberOfTRTHighThresholdHits;
  std::vector<long>    *TrkEF_numberOfBLayerSharedHits;
  std::vector<long>    *TrkEF_numberOfPixelSharedHits;
  std::vector<long>    *TrkEF_numberOfPixelHoles;
  std::vector<long>    *TrkEF_numberOfSCTSharedHits;
  std::vector<long>    *TrkEF_numberOfSCTHoles;
  std::vector<long>    *TrkEF_numberOfTRTOutliers;
  std::vector<long>    *TrkEF_numberOfTRTHighThresholdOutliers;
  std::vector<long>    *TrkEF_numberOfMdtHits;
  std::vector<long>    *TrkEF_numberOfTgcPhiHits;
  std::vector<long>    *TrkEF_numberOfRpcPhiHits;
  std::vector<long>    *TrkEF_numberOfCscPhiHits;
  std::vector<long>    *TrkEF_numberOfTgcEtaHits;
  std::vector<long>    *TrkEF_numberOfRpcEtaHits;
  std::vector<long>    *TrkEF_numberOfCscEtaHits;
  std::vector<long>    *TrkEF_numberOfGangedPixels;
  std::vector<long>    *TrkEF_numberOfOutliersOnTrack;
  std::vector<long>    *TrkEF_numberOfTrackSummaryTypes;
  std::vector<long>    *TrkEF_truthBarcode;
  std::vector<long>    *TrkEF_truthNt;
  std::vector<long>    *TrkEF_ParticlePdg;
  std::vector<long>    *TrkEF_ParentPdg;
  std::vector<long>    *TrkEF_NumKineHits;
  Int_t           VxEF_vxp_numVertices;
  std::vector<float>   *VxEF_vxp_vtx_x;
  std::vector<float>   *VxEF_vxp_vtx_y;
  std::vector<float>   *VxEF_vxp_vtx_z;
  Int_t           VxEF_vxp_numOfTruthVertices;
  std::vector<float>   *VxEF_vxp_vtx_x_truth;
  std::vector<float>   *VxEF_vxp_vtx_y_truth;
  std::vector<float>   *VxEF_vxp_vtx_z_truth;
  std::vector<float>   *VxEF_vxp_vtx_x_res;
  std::vector<float>   *VxEF_vxp_vtx_y_res;
  std::vector<float>   *VxEF_vxp_vtx_z_res;
  std::vector<float>   *VxEF_vxp_sigvtx_x;
  std::vector<float>   *VxEF_vxp_sigvtx_y;
  std::vector<float>   *VxEF_vxp_sigvtx_z;
  std::vector<float>   *VxEF_vxp_vtxchi2;
  std::vector<long>    *VxEF_vxp_vtxndf;
  std::vector<float>   *VxEF_vxp_pt;
  std::vector<float>   *VxEF_vxp_vtxchi2prob;
  std::vector<long>    *VxEF_vxp_numTracksPerVertex;
  Int_t           VxEF_vxp_totalNumTracks;
  std::vector<float>   *VxEF_vxp_chi2;
  std::vector<float>   *VxEF_vxp_d0;
  std::vector<float>   *VxEF_vxp_sigd0;
  std::vector<float>   *VxEF_vxp_z0;
  std::vector<float>   *VxEF_vxp_sigz0;
  std::vector<float>   *VxEF_vxp_phi;
  std::vector<float>   *VxEF_vxp_sigphi;
  std::vector<float>   *VxEF_vxp_theta;
  std::vector<float>   *VxEF_vxp_sigtheta;
  std::vector<float>   *VxEF_vxp_qOverP;
  std::vector<float>   *VxEF_vxp_sigqOverP;
  std::vector<float>   *VxEF_vxp_d0g;
  std::vector<float>   *VxEF_vxp_sigd0g;
  std::vector<float>   *VxEF_vxp_z0g;
  std::vector<float>   *VxEF_vxp_sigz0g;
  Int_t           eg_nc_EF;
  std::vector<float>   *eg_e_EF;
  std::vector<float>   *eg_eta_EF;
  std::vector<float>   *eg_phi_EF;
  std::vector<float>   *eg_cl_et_EF;
  std::vector<float>   *eg_cl_eta_EF;
  std::vector<float>   *eg_cl_phi_EF;
  std::vector<float>   *eg_etap_EF;
  std::vector<float>   *eg_zvertex_EF;
  std::vector<float>   *eg_errz_EF;
  std::vector<float>   *eg_depth_EF;
  std::vector<float>   *eg_e0_EF;
  std::vector<float>   *eg_e1_EF;
  std::vector<float>   *eg_e2_EF;
  std::vector<float>   *eg_e3_EF;
  std::vector<float>   *eg_eta0_EF;
  std::vector<float>   *eg_eta1_EF;
  std::vector<float>   *eg_eta2_EF;
  std::vector<float>   *eg_eta3_EF;
  std::vector<float>   *eg_phi0_EF;
  std::vector<float>   *eg_phi1_EF;
  std::vector<float>   *eg_phi2_EF;
  std::vector<float>   *eg_phi3_EF;
  std::vector<float>   *eg_Etha1_EF;
  std::vector<float>   *eg_Etha_EF;
  std::vector<float>   *eg_Eha1_EF;
  std::vector<float>   *eg_F1_EF;
  std::vector<float>   *eg_F3_EF;
  std::vector<float>   *eg_E233_EF;
  std::vector<float>   *eg_E237_EF;
  std::vector<float>   *eg_E277_EF;
  std::vector<float>   *eg_Weta1_EF;
  std::vector<float>   *eg_Weta2_EF;
  std::vector<float>   *eg_E2ts1_EF;
  std::vector<float>   *eg_E2tsts1_EF;
  std::vector<float>   *eg_Widths1_EF;
  std::vector<float>   *eg_Widths2_EF;
  std::vector<float>   *eg_poscs1_EF;
  std::vector<float>   *eg_poscs2_EF;
  std::vector<float>   *eg_Barys1_EF;
  std::vector<float>   *eg_Wtots1_EF;
  std::vector<float>   *eg_Emins1_EF;
  std::vector<float>   *eg_Emaxs1_EF;
  std::vector<float>   *eg_Fracs1_EF;
  std::vector<float>   *eg_EtCone45_EF;
  std::vector<float>   *eg_EtCone20_EF;
  std::vector<float>   *eg_EtCone30_EF;
  std::vector<float>   *eg_EtCone40_EF;
  std::vector<long>    *eg_IsEM_EF;
  std::vector<float>   *eg_epiNN_EF;
  std::vector<double>  *eg_EMWeight_EF;
  std::vector<double>  *eg_PionWeight_EF;
  std::vector<double>  *eg_Hmatrix_EF;
  std::vector<long>    *eg_IsEMse_EF;
  std::vector<float>   *eg_epiNNse_EF;
  std::vector<double>  *eg_EMWeightse_EF;
  std::vector<double>  *eg_PionWeightse_EF;
  std::vector<float>   *eg_E011_EF;
  std::vector<float>   *eg_E033_EF;
  std::vector<float>   *eg_E131_EF;
  std::vector<float>   *eg_E1153_EF;
  std::vector<float>   *eg_E235_EF;
  std::vector<float>   *eg_E255_EF;
  std::vector<float>   *eg_E333_EF;
  std::vector<float>   *eg_E335_EF;
  std::vector<float>   *eg_E337_EF;
  std::vector<float>   *eg_E377_EF;
  std::vector<long>    *eg_trkmatchnt_EF;
  std::vector<long>    *eg_primary_EF;
  std::vector<long>    *eg_numtracks_EF;
  std::vector<float>   *eg_eoverp_EF;
  std::vector<float>   *eg_etacorr_EF;
  std::vector<float>   *eg_deta0_EF;
  std::vector<float>   *eg_dphi0_EF;
  std::vector<float>   *eg_deta1_EF;
  std::vector<float>   *eg_dphi1_EF;
  std::vector<float>   *eg_deta2_EF;
  std::vector<float>   *eg_dphi2_EF;
  std::vector<float>   *eg_deta3_EF;
  std::vector<float>   *eg_dphi3_EF;
  std::vector<long>    *eg_trkopt_EF;
  std::vector<float>   *eg_trkinvpTV_EF;
  std::vector<float>   *eg_trkcotThV_EF;
  std::vector<float>   *eg_trkphiV_EF;
  std::vector<int>     *egRoiId_EF;
  std::vector<int>     *egKey_EF;
  UInt_t          T2Bphys_NPar;
  std::vector<int>     *T2Bphys_roiId;
  std::vector<float>   *T2Bphys_eta;
  std::vector<float>   *T2Bphys_phi;
  std::vector<int>     *T2Bphys_pType;
  std::vector<float>   *T2Bphys_mass;
  std::vector<float>   *T2Bphys_fitmass;
  std::vector<float>   *T2Bphys_fitchis;
  std::vector<float>   *T2Bphys_fitprob;
  std::vector<float>   *T2Bphys_secmass;
  std::vector<float>   *T2Bphys_secfitmass;
  std::vector<float>   *T2Bphys_secfitchis;
  std::vector<float>   *T2Bphys_secfitprob;
  std::vector<long>    *T2Bphys_pdgid_tr1;
  std::vector<long>    *T2Bphys_pdgid_tr2;
  std::vector<long>    *T2Bphys_pdgid_tr3;
  std::vector<long>    *T2Bphys_NtEnt1;
  std::vector<long>    *T2Bphys_NtEnt2;
  std::vector<long>    *T2Bphys_NtEnt3;
  UInt_t          T2BjN;
  std::vector<float>   *T2BjEta;
  std::vector<float>   *T2BjPhi0;
  std::vector<float>   *T2BjX2d;
  std::vector<float>   *T2BjXd0;
  std::vector<float>   *T2BjXz0;
  std::vector<float>   *T2BjX3d;
  std::vector<float>   *T2BjXmvtx;
  std::vector<float>   *T2BjXevtx;
  std::vector<float>   *T2BjXnvtx;
  std::vector<float>   *T2BjPvtx;
  UInt_t          EFBjN;
  std::vector<float>   *EFBjEta;
  std::vector<float>   *EFBjPhi0;
  std::vector<float>   *EFBjX2d;
  std::vector<float>   *EFBjXd0;
  std::vector<float>   *EFBjXz0;
  std::vector<float>   *EFBjX3d;
  std::vector<float>   *EFBjXmvtx;
  std::vector<float>   *EFBjXevtx;
  std::vector<float>   *EFBjXnvtx;
  std::vector<float>   *EFBjPvtx;
  Int_t           tau_L2ntaus;
  std::vector<float>   *tau_L2eta;
  std::vector<float>   *tau_L2phi;
  std::vector<float>   *tau_L2pt;
  std::vector<float>   *tau_L2Zvtx;
  std::vector<float>   *tau_L2nMatchTracks;
  std::vector<float>   *tau_L2roiId;
  UChar_t         Trig_DecisionL1;
  UChar_t         Trig_DecisionL2;
  UChar_t         Trig_DecisionEF;
  UChar_t         Trig_EM01;
  UChar_t         Trig_L1_2EM15;
  UChar_t         Trig_L1_2EM15I;
  UChar_t         Trig_L1_EM25;
  UChar_t         Trig_L1_EM25I;
  UChar_t         Trig_L1_EM60;
  UChar_t         Trig_L1_XE20;
  UChar_t         Trig_L1_XE30;
  UChar_t         Trig_L1_XE40;
  UChar_t         Trig_L1_XE50;
  UChar_t         Trig_L1_XE100;
  UChar_t         Trig_L1_XE200;
  UChar_t         Trig_L1_TAU05;
  UChar_t         Trig_L1_TAU10;
  UChar_t         Trig_L1_TAU10I;
  UChar_t         Trig_L1_TAU15;
  UChar_t         Trig_L1_TAU15I;
  UChar_t         Trig_L1_TAU20I;
  UChar_t         Trig_L1_TAU25I;
  UChar_t         Trig_L1_TAU35I;
  UChar_t         Trig_L1_MU06;
  UChar_t         Trig_L1_2MU06;
  UChar_t         Trig_L1_MU08;
  UChar_t         Trig_L1_MU10;
  UChar_t         Trig_L1_MU11;
  UChar_t         Trig_L1_MU20;
  UChar_t         Trig_L1_MU40;
  UChar_t         Trig_L1_J35;
  UChar_t         Trig_L1_J45;
  UChar_t         Trig_L1_2J45;
  UChar_t         Trig_L1_3J45;
  UChar_t         Trig_L1_4J45;
  UChar_t         Trig_L1_FJ30;
  UChar_t         Trig_L1_J60;
  UChar_t         Trig_L1_J80;
  UChar_t         Trig_L1_J170;
  UChar_t         Trig_L1_J300;
  UChar_t         Trig_L1_BJT15;
  UChar_t         Trig_L1_EM5;
  UChar_t         Trig_L2_met10f;
  UChar_t         Trig_L2_g10;
  UChar_t         Trig_L2_g20iL2_g20i;
  UChar_t         Trig_L2_g60;
  UChar_t         Trig_L2_jet20a;
  UChar_t         Trig_L2_jet20bL2_jet20b;
  UChar_t         Trig_L2_jet20cL2_jet20cL2_jet20c;
  UChar_t         Trig_L2_jet20dL2_jet20dL2_jet20dL2_jet20d;
  UChar_t         Trig_L2_jet20kt;
  UChar_t         Trig_L2_jet160;
  UChar_t         Trig_L2_jet120L2_jet120;
  UChar_t         Trig_L2_jet65L2_jet65L2_jet65;
  UChar_t         Trig_L2_jet50L2_jet50L2_jet50L2_jet50;
  UChar_t         Trig_L2_frjet10;
  UChar_t         Trig_L2_fljet10;
  UChar_t         Trig_L2_BsDsPhiPi;
  UChar_t         Trig_L2_BsDsPhiPiFullSCan;
  UChar_t         Trig_L2_BJpsiMU6mu;
  UChar_t         Trig_L2_b35;
  UChar_t         Trig_L2_e10TRTxK;
  UChar_t         Trig_L2_e15iL2_e15i;
  UChar_t         Trig_L2_e25i;
  UChar_t         Trig_L2_e60;
  UChar_t         Trig_L2_e10;
  UChar_t         Trig_L2_e10L2_e10;
  UChar_t         Trig_L2_mu6l;
  UChar_t         Trig_L2_mu20i;
  UChar_t         Trig_L2_mu6lL2_mu6l;
  UChar_t         Trig_L2_mu6L2_mu6;
  UChar_t         Trig_L2_mu6;
  UChar_t         Trig_L2_Ze10e10;
  UChar_t         Trig_L2_tau10;
  UChar_t         Trig_L2_tau10i;
  UChar_t         Trig_L2_tau15;
  UChar_t         Trig_L2_tau15i;
  UChar_t         Trig_L2_tau20i;
  UChar_t         Trig_L2_tau25i;
  UChar_t         Trig_L2_tau35i;
  UChar_t         Trig_L2_tauNoCut;
  UChar_t         Trig_L2_BJpsimu6lmu6l;
  UChar_t         Trig_L2_BJpsimu6mu6;
  UChar_t         Trig_L2_BJpsimu6mu;
  UChar_t         Trig_EF_met10;
  UChar_t         Trig_EF_jet20aEt;
  UChar_t         Trig_EF_jet20bEtEF_jet20bEt;
  UChar_t         Trig_EF_jet20cEtEF_jet20cEtEF_jet20cEt;
  UChar_t         Trig_EF_jet20dEtEF_jet20dEtEF_jet20dEtEF_jet20dEt;
  UChar_t         Trig_EF_jet20kt;
  UChar_t         Trig_EF_jet160;
  UChar_t         Trig_EF_jet120EF_jet120;
  UChar_t         Trig_EF_jet65EF_jet65EF_jet65;
  UChar_t         Trig_EF_jet50EF_jet50EF_jet50EF_jet50;
  UChar_t         Trig_EF_frjet10;
  UChar_t         Trig_EF_fljet10;
  UChar_t         Trig_EF_MuonTRTExt_mu6l;
  UChar_t         Trig_EF_BsDsPhiPi;
  UChar_t         Trig_EF_b35;
  UChar_t         Trig_EF_tau10;
  UChar_t         Trig_EF_tau10i;
  UChar_t         Trig_EF_tau15;
  UChar_t         Trig_EF_tau15i;
  UChar_t         Trig_EF_tau20i;
  UChar_t         Trig_EF_tau25i;
  UChar_t         Trig_EF_tau35i;
  UChar_t         Trig_EF_tauNoCut;
  UChar_t         Trig_EF_mu6l;
  UChar_t         Trig_EF_mu6;
  UChar_t         Trig_EF_mu20i;
  UChar_t         Trig_EF_g10;
  UChar_t         Trig_EF_g20iEF_g20i;
  UChar_t         Trig_EF_g60;
  UChar_t         Trig_EF_e10;
  UChar_t         Trig_EF_e10TRTxK;
  UChar_t         Trig_EF_e15iEF_e15i;
  UChar_t         Trig_EF_e25i;
  UChar_t         Trig_EF_e60;
  Int_t           nMC;
  std::vector<double>  *mcPx;
  std::vector<double>  *mcPy;
  std::vector<double>  *mcPz;
  std::vector<double>  *mcEne;
  std::vector<std::vector<double> > *mcEtIsol;
  std::vector<long>    *mcID;
  std::vector<int>     *mcStatus;
  std::vector<int>     *mcBarcode;
  std::vector<double>  *mcCharge;
  std::vector<long>    *mcNMother;
  std::vector<std::vector<double> > *mcMotherNt;
  std::vector<long>    *mcNChild;
  std::vector<std::vector<double> > *mcChildNt;
  Int_t           EleN;
  std::vector<long>    *EleAuthor;
  std::vector<long>    *EleTrkNt;
  std::vector<float>   *EleEta;
  std::vector<float>   *ElePhi;
  std::vector<float>   *ElePt;
  std::vector<long>    *EleIsEM;
  std::vector<float>   *EleZ0;
  std::vector<float>   *EleD0;
  std::vector<long>    *EleNBHits;
  std::vector<long>    *EleNPixelHits;
  std::vector<long>    *EleNSCTHits;
  std::vector<long>    *EleNTRTHits;
  std::vector<long>    *EleNTRHits;
  std::vector<float>   *EleEOverP;
  std::vector<float>   *EleEtaBE2;
  std::vector<float>   *EleEt37;
  std::vector<float>   *EleE237;
  std::vector<float>   *EleE277;
  std::vector<float>   *EleEthad1;
  std::vector<float>   *EleWeta1;
  std::vector<float>   *EleWeta2;
  std::vector<float>   *EleF1;
  std::vector<float>   *EleE2tsts1;
  std::vector<float>   *EleEmins1;
  std::vector<float>   *EleWtots1;
  std::vector<float>   *EleFracs1;
  std::vector<float>   *EleSoftEtaCorrMag;
  std::vector<float>   *EleSoftF1Core;
  std::vector<float>   *EleSoftF3Core;
  std::vector<float>   *EleSoftPos7;
  std::vector<float>   *EleSoftIso;
  std::vector<float>   *EleSoftWidths2;
  Int_t           PhoN;
  std::vector<long>    *PhoAuthor;
  std::vector<float>   *PhoEta;
  std::vector<float>   *PhoPhi;
  std::vector<float>   *PhoPt;
  std::vector<long>    *PhoIsEM;
  std::vector<float>   *PhoEtaBE2;
  std::vector<float>   *PhoEt37;
  std::vector<float>   *PhoE237;
  std::vector<float>   *PhoE277;
  std::vector<float>   *PhoEthad1;
  std::vector<float>   *PhoWeta1;
  std::vector<float>   *PhoWeta2;
  std::vector<float>   *PhoF1;
  std::vector<float>   *PhoE2tsts1;
  std::vector<float>   *PhoEmins1;
  std::vector<float>   *PhoWtots1;
  std::vector<float>   *PhoFracs1;
  UInt_t          MuonN;
  std::vector<double>  *MuonAuthor;
  std::vector<double>  *MuonCombTrkNt;
  std::vector<double>  *MuonInDetTrkNt;
  std::vector<double>  *MuonExtrNt;
  std::vector<double>  *MuonEta;
  std::vector<double>  *MuonPhi;
  std::vector<double>  *MuonPt;
  std::vector<double>  *MuonZ0;
  std::vector<double>  *MuonD0;
  std::vector<double>  *MuonCharge;
  std::vector<double>  *MuonNMDTHits;
  std::vector<double>  *MuonNRPCEtaHits;
  std::vector<double>  *MuonNRPCPhiHits;
  std::vector<double>  *MuonNCSCEtaHits;
  std::vector<double>  *MuonNCSCPhiHits;
  std::vector<double>  *MuonNTGCEtaHits;
  std::vector<double>  *MuonNTGCPhiHits;
  std::vector<double>  *MuonLowPtNMDTHits;
  std::vector<double>  *MuonLowPtNCSCEtaHits;
  std::vector<double>  *MuonLowPtNCSCPhiHits;
  std::vector<double>  *MuonLowPtNRPCEtaHits;
  std::vector<double>  *MuonLowPtNRPCPhiHits;
  std::vector<double>  *MuonLowPtNTGCEtaHits;
  std::vector<double>  *MuonLowPtNTGCPhiHits;
  Int_t           TauJet_N;
  std::vector<long>    *TauJet_Author;
  std::vector<long>    *TauJet_numTrack;
  std::vector<long>    *TauJet_decayVxNt;
  std::vector<long>    *TauJet_clusterNt;
  std::vector<float>   *TauJet_eta;
  std::vector<float>   *TauJet_phi;
  std::vector<float>   *TauJet_pt;
  std::vector<float>   *TauJet_m;
  std::vector<float>   *TauJet_et;
  std::vector<double>  *TauJet_charge;
  std::vector<float>   *TauJet_emRadius;
  std::vector<float>   *TauJet_stripWidth2;
  std::vector<float>   *TauJet_nStripCells;
  Int_t           TauJet_NumTauRecParams;
  std::vector<float>   *TauJet_pEM_px;
  std::vector<float>   *TauJet_pEM_py;
  std::vector<float>   *TauJet_pEM_pz;
  std::vector<float>   *TauJet_pEM_e;
  std::vector<float>   *TauJet_etHadCalib;
  std::vector<float>   *TauJet_etEMCalib;
  std::vector<float>   *TauJet_isolationFraction;
  std::vector<float>   *TauJet_centralityFraction;
  std::vector<float>   *TauJet_logLikelihoodRatio;
  std::vector<float>   *TauJet_lowPtTauJetDiscriminant;
  std::vector<float>   *TauJet_lowPtTauEleDiscriminant;
  std::vector<float>   *TauJet_tauJetNeuralnetwork;
  std::vector<float>   *TauJet_tauENeuralNetwork;
  std::vector<std::vector<double> > *TauJet_TrkNts;
  Int_t           TauJet1p3p_N;
  std::vector<long>    *TauJet1p3p_Author;
  std::vector<long>    *TauJet1p3p_numTrack;
  std::vector<long>    *TauJet1p3p_decayVxNt;
  std::vector<long>    *TauJet1p3p_clusterNt;
  std::vector<float>   *TauJet1p3p_eta;
  std::vector<float>   *TauJet1p3p_phi;
  std::vector<float>   *TauJet1p3p_pt;
  std::vector<float>   *TauJet1p3p_m;
  std::vector<float>   *TauJet1p3p_et;
  std::vector<double>  *TauJet1p3p_charge;
  std::vector<float>   *TauJet1p3p_emRadius;
  std::vector<float>   *TauJet1p3p_stripWidth2;
  std::vector<float>   *TauJet1p3p_nStripCells;
  Int_t           TauJet1p3p_NumTau1p3pParams;
  std::vector<float>   *TauJet1p3p_annularIsolationFraction;
  std::vector<float>   *TauJet1p3p_etCaloAtEMScale;
  std::vector<float>   *TauJet1p3p_etChargedHadCells;
  std::vector<float>   *TauJet1p3p_etOtherEMCells;
  std::vector<float>   *TauJet1p3p_etOtherHadCells;
  std::vector<float>   *TauJet1p3p_discriminant;
  std::vector<float>   *TauJet1p3p_discPDERS;
  std::vector<float>   *TauJet1p3p_discNN;
  std::vector<float>   *TauJet1p3p_nAssocTracksCore;
  std::vector<float>   *TauJet1p3p_nAssocTracksIsol;
  std::vector<float>   *TauJet1p3p_sumPTtrack;
  std::vector<float>   *TauJet1p3p_rWidth2Trk3P;
  std::vector<float>   *TauJet1p3p_massTrk3P;
  std::vector<std::vector<double> > *TauJet1p3p_TrkNts;
  UInt_t          CTPWord0;
  UInt_t          CTPWord1;
  UInt_t          CTPWord2;
  UInt_t          MuROI_N;
  std::vector<float>   *MuROI_Eta;
  std::vector<float>   *MuROI_Phi;
  std::vector<long>    *MuROI_Thr;
  UInt_t          EmROI_N;
  std::vector<float>   *EmROI_Eta;
  std::vector<float>   *EmROI_Phi;
  std::vector<long>    *EmROI_Thr;
  UInt_t          JetROI_N;
  std::vector<float>   *JetROI_Eta;
  std::vector<float>   *JetROI_Phi;
  std::vector<long>    *JetROI_Thr;
  UInt_t          JetEtROI_N;
  std::vector<long>    *JetEtROI_Word;
  std::vector<long>    *JetEtROI_Thr;
  UInt_t          ESumROI_N;
  std::vector<float>   *ESumROI_Ex;
  std::vector<float>   *ESumROI_Ey;
  std::vector<float>   *ESumROI_Et;
  std::vector<long>    *ESumROI_ThrS;
  std::vector<long>    *ESumROI_ThrM;
  Float_t         cl_ecluster_topo;
  Int_t           cl_nctotal_topo;
  Int_t           cl_nc_topo;
  std::vector<float>   *cl_et_topo;
  std::vector<float>   *cl_e_topo;
  std::vector<float>   *cl_eemb0_topo;
  std::vector<float>   *cl_eemb1_topo;
  std::vector<float>   *cl_eemb2_topo;
  std::vector<float>   *cl_eemb3_topo;
  std::vector<long>    *cl_nemb0_topo;
  std::vector<long>    *cl_nemb1_topo;
  std::vector<long>    *cl_nemb2_topo;
  std::vector<long>    *cl_nemb3_topo;
  std::vector<float>   *cl_phi2_topo;
  std::vector<float>   *cl_eta0_topo;
  std::vector<float>   *cl_eta1_topo;
  std::vector<float>   *cl_eta2_topo;
  std::vector<float>   *cl_eta3_topo;
  std::vector<float>   *cl_eeme0_topo;
  std::vector<float>   *cl_eeme1_topo;
  std::vector<float>   *cl_eeme2_topo;
  std::vector<float>   *cl_eeme3_topo;
  std::vector<long>    *cl_neme0_topo;
  std::vector<long>    *cl_neme1_topo;
  std::vector<long>    *cl_neme2_topo;
  std::vector<long>    *cl_neme3_topo;
  std::vector<float>   *cl_etileg1_topo;
  std::vector<float>   *cl_etileg2_topo;
  std::vector<float>   *cl_etileg3_topo;
  std::vector<long>    *cl_ntileg1_topo;
  std::vector<long>    *cl_ntileg2_topo;
  std::vector<long>    *cl_ntileg3_topo;
  std::vector<float>   *cl_eta_topo;
  std::vector<float>   *cl_phi_topo;
  std::vector<float>   *cl_time_topo;
  std::vector<long>    *cl_reco_stat_topo;
  std::vector<float>   *cl_m1_eta_topo;
  std::vector<float>   *cl_m1_phi_topo;
  std::vector<float>   *cl_m2_r_topo;
  std::vector<float>   *cl_m2_lambda_topo;
  std::vector<float>   *cl_delta_phi_topo;
  std::vector<float>   *cl_delta_theta_topo;
  std::vector<float>   *cl_delta_alpha_topo;
  std::vector<float>   *cl_center_x_topo;
  std::vector<float>   *cl_center_y_topo;
  std::vector<float>   *cl_center_z_topo;
  std::vector<float>   *cl_center_lambda_topo;
  std::vector<float>   *cl_lateral_topo;
  std::vector<float>   *cl_longitudinal_topo;
  std::vector<float>   *cl_eng_frac_em_topo;
  std::vector<float>   *cl_eng_frac_max_topo;
  std::vector<float>   *cl_eng_frac_core_topo;
  std::vector<float>   *cl_m1_dens_topo;
  std::vector<float>   *cl_m2_dens_topo;
  std::vector<float>   *cl_ehec0_topo;
  std::vector<float>   *cl_ehec1_topo;
  std::vector<float>   *cl_ehec2_topo;
  std::vector<float>   *cl_ehec3_topo;
  std::vector<long>    *cl_nhec0_topo;
  std::vector<long>    *cl_nhec1_topo;
  std::vector<long>    *cl_nhec2_topo;
  std::vector<long>    *cl_nhec3_topo;
  std::vector<float>   *cl_etileb0_topo;
  std::vector<float>   *cl_etileb1_topo;
  std::vector<float>   *cl_etileb2_topo;
  std::vector<long>    *cl_ntileb0_topo;
  std::vector<long>    *cl_ntileb1_topo;
  std::vector<long>    *cl_ntileb2_topo;
  std::vector<float>   *cl_etilee0_topo;
  std::vector<float>   *cl_etilee1_topo;
  std::vector<float>   *cl_etilee2_topo;
  std::vector<float>   *cl_efcal0_topo;
  std::vector<float>   *cl_efcal1_topo;
  std::vector<float>   *cl_efcal2_topo;
  std::vector<long>    *cl_ntilee0_topo;
  std::vector<long>    *cl_ntilee1_topo;
  std::vector<long>    *cl_ntilee2_topo;
  std::vector<long>    *cl_nfcal0_topo;
  std::vector<long>    *cl_nfcal1_topo;
  std::vector<long>    *cl_nfcal2_topo;
  std::vector<long>    *cl_ntotcells_topo;
  UInt_t          nPJ;
  std::vector<std::string>  *pjAuthor;
  std::vector<double>  *pjPx;
  std::vector<double>  *pjPy;
  std::vector<double>  *pjPz;
  std::vector<double>  *pjEne;
  std::vector<double>  *pjEta;
  std::vector<double>  *pjPhi;
  std::vector<double>  *pjPt;
  std::vector<int>     *pjNtrk;
  std::vector<int>     *pjNmuo;
  std::vector<int>     *pjNele;
  std::vector<double>  *pjWeight;
  std::vector<double>  *pjW2D;
  std::vector<double>  *pjW3D;
  std::vector<double>  *pjWSV2;
  std::vector<double>  *pjWlhSig;
  std::vector<int>     *pjLabel;
  std::vector<double>  *pjdRb;
  std::vector<double>  *pjdRc;
  std::vector<double>  *pjdRt;
  Float_t         MemRSS;
  Float_t         MemVSize;
  Float_t         TimeTotal;
  Float_t         TimeSum;
  Float_t         TimeEventCounter;
  Float_t         TimePixelRegionSelectorTable;
  Float_t         TimeSCT_RegionSelectorTable;
  Float_t         TimeTRT_RegionSelectorTable;
  Float_t         TimePixelClusterization;
  Float_t         TimeSCT_Clusterization;
  Float_t         TimeTRT_RIO_Maker;
  Float_t         TimePRD_MultiTruthMaker;
  Float_t         TimeSiTrackerSpacePointFinder;
  Float_t         TimeiPatRec;
  Float_t         TimeiPatTrackTruthAssociator;
  Float_t         TimeiPatStatistics;
  Float_t         TimeXKaSeedsManager;
  Float_t         TimeXKaMField;
  Float_t         TimeXKaDetectorBuilder;
  Float_t         TimeXKalMan;
  Float_t         TimeInDetLegacyCnvAlg;
  Float_t         TimeConvertedXKalmanTracksDetailedTruthMaker;
  Float_t         TimeConvertedXKalmanTracksTruthSelector;
  Float_t         TimeConvertedIPatTracksDetailedTruthMaker;
  Float_t         TimeConvertedIPatTracksTruthSelector;
  Float_t         TimeInDetSiSPTrackFinder;
  Float_t         TimeSiSPSeededTracksDetailedTruthMaker;
  Float_t         TimeSiSPSeededTracksTruthCollectionSelector;
  Float_t         TimeInDetAmbiguitySolver;
  Float_t         TimeResolvedTracksDetailedTruthMaker;
  Float_t         TimeResolvedTracksTruthCollectionSelector;
  Float_t         TimeInDetTRTExtension;
  Float_t         TimeInDetExtensionProcessor;
  Float_t         TimeExtendedTracksDetailedTruthMaker;
  Float_t         TimeExtendedTracksTruthCollectionSelector;
  Float_t         TimeInDetTRTTrackSegmentsFinder;
  Float_t         TimeInDetOutputCopyAlg;
  Float_t         TimeDetailedTracksTruthMaker;
  Float_t         TimeTrackTruthCollectionSelector;
  Float_t         TimeInDetPriVxFinder;
  Float_t         TimeInDetIPatPriVxFinder;
  Float_t         TimeInDetXKalPriVxFinder;
  Float_t         TimeInDetParticleCreation;
  Float_t         TimeInDetIPatParticleCreation;
  Float_t         TimeInDetXKalParticleCreation;
  Float_t         TimeInDetTrackParticleTruthMaker;
  Float_t         TimeInDetIPatTrackParticleTruthMaker;
  Float_t         TimeInDetXKalTrackParticleTruthMaker;
  Float_t         TimeInDetRecStatistics;
  Float_t         TimeCaloCellMaker;
  Float_t         TimeCmbTowerBldr;
  Float_t         TimeCaloSWClusterMaker;
  Float_t         TimeTowerMaker;
  Float_t         TimeLArSWClusterMaker;
  Float_t         TimeLArgamClusterMaker;
  Float_t         TimeLAr35ClusterMaker;
  Float_t         TimeLAr37ClusterMaker;
  Float_t         TimeLArgam35ClusterMaker;
  Float_t         TimeLArgam37ClusterMaker;
  Float_t         TimeCaloTopoClusterMaker;
  Float_t         TimeCaloCell2TopoClusterMapper;
  Float_t         TimeEMTopoClusterMaker;
  Float_t         TimeEMCell2TopoClusterMapper;
  Float_t         TimeMuonRdoToMuonDigit;
  Float_t         TimeCscRdoToCscPrepData;
  Float_t         TimeMdtRdoToMdtPrepData;
  Float_t         TimeRpcRdoToRpcPrepData;
  Float_t         TimeTgcRdoToTgcPrepData;
  Float_t         TimeCscThresholdClusterBuilder;
  Float_t         TimeTrackRecordFilter;
  Float_t         TimeMboyDigiEmptyLoop;
  Float_t         TimeMboyDigiEmptyLoop2;
  Float_t         TimeMboyRec;
  Float_t         TimeMooMakePhiPatterns;
  Float_t         TimeMooMakeRzPatterns;
  Float_t         TimeMooCombinePatterns;
  Float_t         TimeMooCalibratedSegmentMaker;
  Float_t         TimePhiPatternToPhiSeg;
  Float_t         TimeCscPatternToCrudeSeg;
  Float_t         TimeMdtPatternToCrudeSeg;
  Float_t         TimeMuonSegmentComboToMooRzSegmentCombo;
  Float_t         TimeMuonSegmentToMooRzSegment;
  Float_t         TimeMooRoadMaker;
  Float_t         TimeMooMakeTracks;
  Float_t         TimeMooCnvAlg;
  Float_t         TimeMuidMooreTPCreator;
  Float_t         TimeConversionFinder;
  Float_t         Timeegamma;
  Float_t         Timesofte;
  Float_t         TimeConeTowerJets;
  Float_t         TimeCone4TowerJets;
  Float_t         TimeKtTowerJets;
  Float_t         TimeKt4TowerJets;
  Float_t         TimeConeTopoJetAlg;
  Float_t         TimeCone4TopoJetAlg;
  Float_t         TimeKt6TopoJets;
  Float_t         TimeKt4TopoJets;
  Float_t         TimeConeTruthJets;
  Float_t         TimeCone4TruthJets;
  Float_t         TimeKt6TruthJets;
  Float_t         TimeKt4TruthJets;
  Float_t         TimetauSequence;
  Float_t         Timetau1p3pSequence;
  Float_t         TimeTileLookForMuAlg;
  Float_t         TimeMuidStandAlone;
  Float_t         TimeMuidExtrCnvAlg;
  Float_t         TimeMuidCombined;
  Float_t         TimeMuidCnvAlg;
  Float_t         TimeMuGirl;
  Float_t         TimeMuGirlToCombinedMuon;
  Float_t         TimeStacoMaster;
  Float_t         TimeMuTagMaster;
  Float_t         TimeMuonPrepRawDataTruthMaker;
  Float_t         TimeMuonTrackTruthMaker;
  Float_t         TimeMETRefAlg;
  Float_t         TimeMETAlg;
  Float_t         TimeLeptonAlg;
  Float_t         TimeHadJets;
  Float_t         TimeHadronAlg;
  Float_t         TimeTriggerTowerMaker;
  Float_t         TimeJetElementMaker;
  Float_t         TimeEmTauTrigger;
  Float_t         TimeJetTrigger;
  Float_t         TimeEnergyTrigger;
  Float_t         TimeROD;
  Float_t         TimeTrigT1RPC;
  Float_t         TimeLVL1TGCTrigger;
  Float_t         TimeL1Muctpi;
  Float_t         TimeCTPSim;
  Float_t         TimeRoIBuilder;
  Float_t         TimeZVertexFromKine;
  Float_t         TimeStreamBS;
  Float_t         TimeTriggerConfig;
  Float_t         TimeStepController_L2;
  Float_t         TimeStepController_EF;
  Float_t         TimeTrigMooCnvAlg;
  Float_t         TimeTrigMuidMooreTPCreator;
  Float_t         TimeTrigMuidExtrCnvAlg;
  Float_t         TimeTrigMuidCnvAlg;
  Float_t         TimeTrigMuonBuilder;
  Float_t         TimeTrigInDetTrackTruthMaker;
  Float_t         TimeTriggerDecisionMaker;
  Float_t         TimeL1CaloObjectsToESD;
  Float_t         TimeRoIBResultToAOD;
  Float_t         TimephotonBuilder;
  Float_t         TimeelectronBuilder;
  Float_t         TimeMuonBuilder;
  Float_t         TimetauJetBuilder;
  Float_t         TimetauJetBuilder1p3p;
  Float_t         TimeKt4TowerParticleJetBuilder;
  Float_t         TimeKt6TowerParticleJetBuilder;
  Float_t         TimeCone4TowerParticleJetBuilder;
  Float_t         TimeConeTowerParticleJetBuilder;
  Float_t         TimeKt4TopoParticleJetBuilder;
  Float_t         TimeKt6TopoParticleJetBuilder;
  Float_t         TimeCone4TopoParticleJetBuilder;
  Float_t         TimeConeTopoParticleJetBuilder;
  Float_t         TimeSharedHitMapper;
  Float_t         TimeBJetBuilderKt4Tower;
  Float_t         TimeBJetBuilderKt6Tower;
  Float_t         TimeBJetBuilderCone4Tower;
  Float_t         TimeBJetBuilderConeTower;
  Float_t         TimeBJetBuilderKt4Topo;
  Float_t         TimeBJetBuilderKt6Topo;
  Float_t         TimeBJetBuilderCone4Topo;
  Float_t         TimeBJetBuilderConeTopo;
  Float_t         TimeKt4TruthParticleJetBuilder;
  Float_t         TimeKt6TruthParticleJetBuilder;
  Float_t         TimeCone4TruthParticleJetBuilder;
  Float_t         TimeConeTruthParticleJetBuilder;
  Float_t         TimeMcAodBuilder;
  Float_t         TimeTopSequence;
  Float_t         TimeAtlfastAodBuilder;
  Float_t         TimeFastAANT;
  Float_t         TimeFastAANTupleStream;
  Float_t         TimeStreamESD;
  Float_t         TimeMakeInputDataHeaderESD;
  Float_t         TimeStreamAOD;
  Float_t         TimeCBNT_AthenaAware;
  Float_t         TimeAANTupleStream;
  Float_t         TimeDumpSp;

  // List of branches
  TBranch        *b_RunNumber;   //!
  TBranch        *b_EventNumber;   //!
  TBranch        *b_StreamESD_ref;   //!
  TBranch        *b_StreamRDO_ref;   //!
  TBranch        *b_Token;   //!
  TBranch        *b_Run;   //!
  TBranch        *b_Event;   //!
  TBranch        *b_Time;   //!
  TBranch        *b_Weight;   //!
  TBranch        *b_IEvent;   //!
  TBranch        *b_NPar;   //!
  TBranch        *b_Type;   //!
  TBranch        *b_PtGen;   //!
  TBranch        *b_PhiGen;   //!
  TBranch        *b_EtaGen;   //!
  TBranch        *b_MGen;   //!
  TBranch        *b_Charge;   //!
  TBranch        *b_GenStat;   //!
  TBranch        *b_GenRef;   //!
  TBranch        *b_KMothNt;   //!
  TBranch        *b_KFDauNt;   //!
  TBranch        *b_KLDauNt;   //!
  TBranch        *b_KOriVNt;   //!
  TBranch        *b_KEndVNt;   //!
  TBranch        *b_BunchNum;   //!
  TBranch        *b_RVGen;   //!
  TBranch        *b_PhiVGen;   //!
  TBranch        *b_ZVGen;   //!
  TBranch        *b_NVer;   //!
  TBranch        *b_BunchV;   //!
  TBranch        *b_RV;   //!
  TBranch        *b_PhiV;   //!
  TBranch        *b_ZV;   //!
  TBranch        *b_GenRefV;   //!
  TBranch        *b_KVMothNt;   //!
  TBranch        *b_KVFDauNt;   //!
  TBranch        *b_KVLDauNt;   //!
  TBranch        *b_IVPrimary;   //!
  TBranch        *b_Spcl_Num;   //!
  TBranch        *b_Spcl_ID;   //!
  TBranch        *b_Spcl_Pt;   //!
  TBranch        *b_Spcl_Eta;   //!
  TBranch        *b_Spcl_Phi;   //!
  TBranch        *b_Spcl_M;   //!
  TBranch        *b_Spcl_Stat;   //!
  TBranch        *b_Spcl_Isol;   //!
  TBranch        *b_Spcl_sumx;   //!
  TBranch        *b_Spcl_sumy;   //!
  TBranch        *b_Spcl_sumz;   //!
  TBranch        *b_Spcl_sume;   //!
  TBranch        *b_Spcl_x1;   //!
  TBranch        *b_Spcl_x2;   //!
  TBranch        *b_Spcl_id1;   //!
  TBranch        *b_Spcl_id2;   //!
  TBranch        *b_MuonEntRec_Num;   //!
  TBranch        *b_MuonEntRec_ID;   //!
  TBranch        *b_MuonEntRec_Ene;   //!
  TBranch        *b_MuonEntRec_Eta;   //!
  TBranch        *b_MuonEntRec_Phi;   //!
  TBranch        *b_MuonEntRec_PosX;   //!
  TBranch        *b_MuonEntRec_PosY;   //!
  TBranch        *b_MuonEntRec_PosZ;   //!
  TBranch        *b_MuonEntRec_Time;   //!
  TBranch        *b_MuonEntRec_BarCode;   //!
  TBranch        *b_vxp_numVertices;   //!
  TBranch        *b_vxp_vtx_x;   //!
  TBranch        *b_vxp_vtx_y;   //!
  TBranch        *b_vxp_vtx_z;   //!
  TBranch        *b_vxp_numOfTruthVertices;   //!
  TBranch        *b_vxp_vtx_x_truth;   //!
  TBranch        *b_vxp_vtx_y_truth;   //!
  TBranch        *b_vxp_vtx_z_truth;   //!
  TBranch        *b_vxp_vtx_x_res;   //!
  TBranch        *b_vxp_vtx_y_res;   //!
  TBranch        *b_vxp_vtx_z_res;   //!
  TBranch        *b_vxp_sigvtx_x;   //!
  TBranch        *b_vxp_sigvtx_y;   //!
  TBranch        *b_vxp_sigvtx_z;   //!
  TBranch        *b_vxp_vtxchi2;   //!
  TBranch        *b_vxp_vtxndf;   //!
  TBranch        *b_vxp_pt;   //!
  TBranch        *b_vxp_vtx_constraint_x;   //!
  TBranch        *b_vxp_vtx_constraint_y;   //!
  TBranch        *b_vxp_vtx_constraint_z;   //!
  TBranch        *b_vxp_sigvtx_constraint_x;   //!
  TBranch        *b_vxp_sigvtx_constraint_y;   //!
  TBranch        *b_vxp_sigvtx_constraint_z;   //!
  TBranch        *b_vxp_vtx_linearization_x;   //!
  TBranch        *b_vxp_vtx_linearization_y;   //!
  TBranch        *b_vxp_vtx_linearization_z;   //!
  TBranch        *b_vxp_vtx_seed_x;   //!
  TBranch        *b_vxp_vtx_seed_y;   //!
  TBranch        *b_vxp_vtx_seed_z;   //!
  TBranch        *b_vxp_vtxchi2prob;   //!
  TBranch        *b_vxp_numTracksPerVertex;   //!
  TBranch        *b_vxp_totalNumTracks;   //!
  TBranch        *b_vxp_chi2;   //!
  TBranch        *b_vxp_ndf;   //!
  TBranch        *b_vxp_vertexCompatibility;   //!
  TBranch        *b_vxp_d0;   //!
  TBranch        *b_vxp_sigd0;   //!
  TBranch        *b_vxp_z0;   //!
  TBranch        *b_vxp_sigz0;   //!
  TBranch        *b_vxp_phi;   //!
  TBranch        *b_vxp_sigphi;   //!
  TBranch        *b_vxp_theta;   //!
  TBranch        *b_vxp_sigtheta;   //!
  TBranch        *b_vxp_qOverP;   //!
  TBranch        *b_vxp_sigqOverP;   //!
  TBranch        *b_vxp_d0g;   //!
  TBranch        *b_vxp_sigd0g;   //!
  TBranch        *b_vxp_z0g;   //!
  TBranch        *b_vxp_sigz0g;   //!
  TBranch        *b_vxp_refitted_d0;   //!
  TBranch        *b_vxp_refitted_sigd0;   //!
  TBranch        *b_vxp_refitted_z0;   //!
  TBranch        *b_vxp_refitted_sigz0;   //!
  TBranch        *b_vxp_refitted_phi;   //!
  TBranch        *b_vxp_refitted_sigphi;   //!
  TBranch        *b_vxp_refitted_theta;   //!
  TBranch        *b_vxp_refitted_sigtheta;   //!
  TBranch        *b_vxp_refitted_qOverP;   //!
  TBranch        *b_vxp_refitted_sigqOverP;   //!
  TBranch        *b_Trk_totalNumTracks;   //!
  TBranch        *b_Trk_d0;   //!
  TBranch        *b_Trk_z0;   //!
  TBranch        *b_Trk_phi;   //!
  TBranch        *b_Trk_theta;   //!
  TBranch        *b_Trk_qOverP;   //!
  TBranch        *b_Trk_Chi2;   //!
  TBranch        *b_Trk_Ndf;   //!
  TBranch        *b_Trk_sigd0;   //!
  TBranch        *b_Trk_sigz0;   //!
  TBranch        *b_Trk_sigphi;   //!
  TBranch        *b_Trk_sigtheta;   //!
  TBranch        *b_Trk_sigqOverP;   //!
  TBranch        *b_Trk_covVert21;   //!
  TBranch        *b_Trk_covVert31;   //!
  TBranch        *b_Trk_covVert32;   //!
  TBranch        *b_Trk_covVert41;   //!
  TBranch        *b_Trk_covVert42;   //!
  TBranch        *b_Trk_covVert43;   //!
  TBranch        *b_Trk_covVert51;   //!
  TBranch        *b_Trk_covVert52;   //!
  TBranch        *b_Trk_covVert53;   //!
  TBranch        *b_Trk_covVert54;   //!
  TBranch        *b_Trk_px;   //!
  TBranch        *b_Trk_py;   //!
  TBranch        *b_Trk_pz;   //!
  TBranch        *b_Trk_pt;   //!
  TBranch        *b_Trk_numberOfBLayerHits;   //!
  TBranch        *b_Trk_numberOfPixelHits;   //!
  TBranch        *b_Trk_numberOfSCTHits;   //!
  TBranch        *b_Trk_numberOfTRTHits;   //!
  TBranch        *b_Trk_numberOfTRTHighThresholdHits;   //!
  TBranch        *b_Trk_numberOfBLayerSharedHits;   //!
  TBranch        *b_Trk_numberOfPixelSharedHits;   //!
  TBranch        *b_Trk_numberOfPixelHoles;   //!
  TBranch        *b_Trk_numberOfSCTSharedHits;   //!
  TBranch        *b_Trk_numberOfSCTHoles;   //!
  TBranch        *b_Trk_numberOfTRTOutliers;   //!
  TBranch        *b_Trk_numberOfTRTHighThresholdOutliers;   //!
  TBranch        *b_Trk_numberOfMdtHits;   //!
  TBranch        *b_Trk_numberOfTgcPhiHits;   //!
  TBranch        *b_Trk_numberOfRpcPhiHits;   //!
  TBranch        *b_Trk_numberOfCscPhiHits;   //!
  TBranch        *b_Trk_numberOfTgcEtaHits;   //!
  TBranch        *b_Trk_numberOfRpcEtaHits;   //!
  TBranch        *b_Trk_numberOfCscEtaHits;   //!
  TBranch        *b_Trk_numberOfGangedPixels;   //!
  TBranch        *b_Trk_numberOfOutliersOnTrack;   //!
  TBranch        *b_Trk_numberOfTrackSummaryTypes;   //!
  TBranch        *b_Trk_truthBarcode;   //!
  TBranch        *b_Trk_truthNt;   //!
  TBranch        *b_Trk_ParticlePdg;   //!
  TBranch        *b_Trk_ParentPdg;   //!
  TBranch        *b_Nh_Calo;   //!
  TBranch        *b_Eh_Calo;   //!
  TBranch        *b_Nh_EM;   //!
  TBranch        *b_Eh_EM;   //!
  TBranch        *b_Nh_HAD;   //!
  TBranch        *b_Eh_HAD;   //!
  TBranch        *b_Nh_PresB;   //!
  TBranch        *b_Eh_PresB;   //!
  TBranch        *b_Nh_EMB;   //!
  TBranch        *b_Eh_EMB;   //!
  TBranch        *b_Nh_EMEC;   //!
  TBranch        *b_Eh_EMEC;   //!
  TBranch        *b_Nh_Tile;   //!
  TBranch        *b_Eh_Tile;   //!
  TBranch        *b_Nh_TileGap;   //!
  TBranch        *b_Eh_TileGap;   //!
  TBranch        *b_Nh_HEC;   //!
  TBranch        *b_Eh_HEC;   //!
  TBranch        *b_Nh_FCal;   //!
  TBranch        *b_Eh_FCal;   //!
  TBranch        *b_Nh_PresE;   //!
  TBranch        *b_Eh_PresE;   //!
  TBranch        *b_Nh_Scint;   //!
  TBranch        *b_Eh_Scint;   //!
  TBranch        *b_nhit_em;   //!
  TBranch        *b_ecell_em;   //!
  TBranch        *b_nhit_hec;   //!
  TBranch        *b_ecell_hec;   //!
  TBranch        *b_nhit_fcal;   //!
  TBranch        *b_ecell_fcal;   //!
  TBranch        *b_nhit_tile;   //!
  TBranch        *b_ecell_tile;   //!
  TBranch        *b_cl_ecluster;   //!
  TBranch        *b_cl_nctotal;   //!
  TBranch        *b_cl_nc;   //!
  TBranch        *b_cl_et;   //!
  TBranch        *b_cl_e;   //!
  TBranch        *b_cl_eemb0;   //!
  TBranch        *b_cl_eemb1;   //!
  TBranch        *b_cl_eemb2;   //!
  TBranch        *b_cl_eemb3;   //!
  TBranch        *b_cl_nemb0;   //!
  TBranch        *b_cl_nemb1;   //!
  TBranch        *b_cl_nemb2;   //!
  TBranch        *b_cl_nemb3;   //!
  TBranch        *b_cl_phi2;   //!
  TBranch        *b_cl_eta0;   //!
  TBranch        *b_cl_eta1;   //!
  TBranch        *b_cl_eta2;   //!
  TBranch        *b_cl_eta3;   //!
  TBranch        *b_cl_eeme0;   //!
  TBranch        *b_cl_eeme1;   //!
  TBranch        *b_cl_eeme2;   //!
  TBranch        *b_cl_eeme3;   //!
  TBranch        *b_cl_neme0;   //!
  TBranch        *b_cl_neme1;   //!
  TBranch        *b_cl_neme2;   //!
  TBranch        *b_cl_neme3;   //!
  TBranch        *b_cl_etileg1;   //!
  TBranch        *b_cl_etileg2;   //!
  TBranch        *b_cl_etileg3;   //!
  TBranch        *b_cl_ntileg1;   //!
  TBranch        *b_cl_ntileg2;   //!
  TBranch        *b_cl_ntileg3;   //!
  TBranch        *b_cl_eta;   //!
  TBranch        *b_cl_phi;   //!
  TBranch        *b_cl_time;   //!
  TBranch        *b_cl_reco_stat;   //!
  TBranch        *b_cl_m1_eta;   //!
  TBranch        *b_cl_m1_phi;   //!
  TBranch        *b_cl_m2_r;   //!
  TBranch        *b_cl_m2_lambda;   //!
  TBranch        *b_cl_delta_phi;   //!
  TBranch        *b_cl_delta_theta;   //!
  TBranch        *b_cl_delta_alpha;   //!
  TBranch        *b_cl_center_x;   //!
  TBranch        *b_cl_center_y;   //!
  TBranch        *b_cl_center_z;   //!
  TBranch        *b_cl_center_lambda;   //!
  TBranch        *b_cl_lateral;   //!
  TBranch        *b_cl_longitudinal;   //!
  TBranch        *b_cl_eng_frac_em;   //!
  TBranch        *b_cl_eng_frac_max;   //!
  TBranch        *b_cl_eng_frac_core;   //!
  TBranch        *b_cl_m1_dens;   //!
  TBranch        *b_cl_m2_dens;   //!
  TBranch        *b_cl_ntotcells;   //!
  TBranch        *b_cl_ecluster_gam;   //!
  TBranch        *b_cl_nctotal_gam;   //!
  TBranch        *b_cl_nc_gam;   //!
  TBranch        *b_cl_et_gam;   //!
  TBranch        *b_cl_e_gam;   //!
  TBranch        *b_cl_eemb0_gam;   //!
  TBranch        *b_cl_eemb1_gam;   //!
  TBranch        *b_cl_eemb2_gam;   //!
  TBranch        *b_cl_eemb3_gam;   //!
  TBranch        *b_cl_nemb0_gam;   //!
  TBranch        *b_cl_nemb1_gam;   //!
  TBranch        *b_cl_nemb2_gam;   //!
  TBranch        *b_cl_nemb3_gam;   //!
  TBranch        *b_cl_phi2_gam;   //!
  TBranch        *b_cl_eta0_gam;   //!
  TBranch        *b_cl_eta1_gam;   //!
  TBranch        *b_cl_eta2_gam;   //!
  TBranch        *b_cl_eta3_gam;   //!
  TBranch        *b_cl_eeme0_gam;   //!
  TBranch        *b_cl_eeme1_gam;   //!
  TBranch        *b_cl_eeme2_gam;   //!
  TBranch        *b_cl_eeme3_gam;   //!
  TBranch        *b_cl_neme0_gam;   //!
  TBranch        *b_cl_neme1_gam;   //!
  TBranch        *b_cl_neme2_gam;   //!
  TBranch        *b_cl_neme3_gam;   //!
  TBranch        *b_cl_etileg1_gam;   //!
  TBranch        *b_cl_etileg2_gam;   //!
  TBranch        *b_cl_etileg3_gam;   //!
  TBranch        *b_cl_ntileg1_gam;   //!
  TBranch        *b_cl_ntileg2_gam;   //!
  TBranch        *b_cl_ntileg3_gam;   //!
  TBranch        *b_cl_eta_gam;   //!
  TBranch        *b_cl_phi_gam;   //!
  TBranch        *b_cl_time_gam;   //!
  TBranch        *b_cl_reco_stat_gam;   //!
  TBranch        *b_cl_m1_eta_gam;   //!
  TBranch        *b_cl_m1_phi_gam;   //!
  TBranch        *b_cl_m2_r_gam;   //!
  TBranch        *b_cl_m2_lambda_gam;   //!
  TBranch        *b_cl_delta_phi_gam;   //!
  TBranch        *b_cl_delta_theta_gam;   //!
  TBranch        *b_cl_delta_alpha_gam;   //!
  TBranch        *b_cl_center_x_gam;   //!
  TBranch        *b_cl_center_y_gam;   //!
  TBranch        *b_cl_center_z_gam;   //!
  TBranch        *b_cl_center_lambda_gam;   //!
  TBranch        *b_cl_lateral_gam;   //!
  TBranch        *b_cl_longitudinal_gam;   //!
  TBranch        *b_cl_eng_frac_em_gam;   //!
  TBranch        *b_cl_eng_frac_max_gam;   //!
  TBranch        *b_cl_eng_frac_core_gam;   //!
  TBranch        *b_cl_m1_dens_gam;   //!
  TBranch        *b_cl_m2_dens_gam;   //!
  TBranch        *b_cl_ntotcells_gam;   //!
  TBranch        *b_cl_ecluster_35;   //!
  TBranch        *b_cl_nctotal_35;   //!
  TBranch        *b_cl_nc_35;   //!
  TBranch        *b_cl_et_35;   //!
  TBranch        *b_cl_e_35;   //!
  TBranch        *b_cl_eemb0_35;   //!
  TBranch        *b_cl_eemb1_35;   //!
  TBranch        *b_cl_eemb2_35;   //!
  TBranch        *b_cl_eemb3_35;   //!
  TBranch        *b_cl_nemb0_35;   //!
  TBranch        *b_cl_nemb1_35;   //!
  TBranch        *b_cl_nemb2_35;   //!
  TBranch        *b_cl_nemb3_35;   //!
  TBranch        *b_cl_phi2_35;   //!
  TBranch        *b_cl_eta0_35;   //!
  TBranch        *b_cl_eta1_35;   //!
  TBranch        *b_cl_eta2_35;   //!
  TBranch        *b_cl_eta3_35;   //!
  TBranch        *b_cl_eeme0_35;   //!
  TBranch        *b_cl_eeme1_35;   //!
  TBranch        *b_cl_eeme2_35;   //!
  TBranch        *b_cl_eeme3_35;   //!
  TBranch        *b_cl_neme0_35;   //!
  TBranch        *b_cl_neme1_35;   //!
  TBranch        *b_cl_neme2_35;   //!
  TBranch        *b_cl_neme3_35;   //!
  TBranch        *b_cl_etileg1_35;   //!
  TBranch        *b_cl_etileg2_35;   //!
  TBranch        *b_cl_etileg3_35;   //!
  TBranch        *b_cl_ntileg1_35;   //!
  TBranch        *b_cl_ntileg2_35;   //!
  TBranch        *b_cl_ntileg3_35;   //!
  TBranch        *b_cl_eta_35;   //!
  TBranch        *b_cl_phi_35;   //!
  TBranch        *b_cl_time_35;   //!
  TBranch        *b_cl_reco_stat_35;   //!
  TBranch        *b_cl_m1_eta_35;   //!
  TBranch        *b_cl_m1_phi_35;   //!
  TBranch        *b_cl_m2_r_35;   //!
  TBranch        *b_cl_m2_lambda_35;   //!
  TBranch        *b_cl_delta_phi_35;   //!
  TBranch        *b_cl_delta_theta_35;   //!
  TBranch        *b_cl_delta_alpha_35;   //!
  TBranch        *b_cl_center_x_35;   //!
  TBranch        *b_cl_center_y_35;   //!
  TBranch        *b_cl_center_z_35;   //!
  TBranch        *b_cl_center_lambda_35;   //!
  TBranch        *b_cl_lateral_35;   //!
  TBranch        *b_cl_longitudinal_35;   //!
  TBranch        *b_cl_eng_frac_em_35;   //!
  TBranch        *b_cl_eng_frac_max_35;   //!
  TBranch        *b_cl_eng_frac_core_35;   //!
  TBranch        *b_cl_m1_dens_35;   //!
  TBranch        *b_cl_m2_dens_35;   //!
  TBranch        *b_cl_ntotcells_35;   //!
  TBranch        *b_cl_ecluster_35gam;   //!
  TBranch        *b_cl_nctotal_35gam;   //!
  TBranch        *b_cl_nc_35gam;   //!
  TBranch        *b_cl_et_35gam;   //!
  TBranch        *b_cl_e_35gam;   //!
  TBranch        *b_cl_eemb0_35gam;   //!
  TBranch        *b_cl_eemb1_35gam;   //!
  TBranch        *b_cl_eemb2_35gam;   //!
  TBranch        *b_cl_eemb3_35gam;   //!
  TBranch        *b_cl_nemb0_35gam;   //!
  TBranch        *b_cl_nemb1_35gam;   //!
  TBranch        *b_cl_nemb2_35gam;   //!
  TBranch        *b_cl_nemb3_35gam;   //!
  TBranch        *b_cl_phi2_35gam;   //!
  TBranch        *b_cl_eta0_35gam;   //!
  TBranch        *b_cl_eta1_35gam;   //!
  TBranch        *b_cl_eta2_35gam;   //!
  TBranch        *b_cl_eta3_35gam;   //!
  TBranch        *b_cl_eeme0_35gam;   //!
  TBranch        *b_cl_eeme1_35gam;   //!
  TBranch        *b_cl_eeme2_35gam;   //!
  TBranch        *b_cl_eeme3_35gam;   //!
  TBranch        *b_cl_neme0_35gam;   //!
  TBranch        *b_cl_neme1_35gam;   //!
  TBranch        *b_cl_neme2_35gam;   //!
  TBranch        *b_cl_neme3_35gam;   //!
  TBranch        *b_cl_etileg1_35gam;   //!
  TBranch        *b_cl_etileg2_35gam;   //!
  TBranch        *b_cl_etileg3_35gam;   //!
  TBranch        *b_cl_ntileg1_35gam;   //!
  TBranch        *b_cl_ntileg2_35gam;   //!
  TBranch        *b_cl_ntileg3_35gam;   //!
  TBranch        *b_cl_eta_35gam;   //!
  TBranch        *b_cl_phi_35gam;   //!
  TBranch        *b_cl_time_35gam;   //!
  TBranch        *b_cl_reco_stat_35gam;   //!
  TBranch        *b_cl_m1_eta_35gam;   //!
  TBranch        *b_cl_m1_phi_35gam;   //!
  TBranch        *b_cl_m2_r_35gam;   //!
  TBranch        *b_cl_m2_lambda_35gam;   //!
  TBranch        *b_cl_delta_phi_35gam;   //!
  TBranch        *b_cl_delta_theta_35gam;   //!
  TBranch        *b_cl_delta_alpha_35gam;   //!
  TBranch        *b_cl_center_x_35gam;   //!
  TBranch        *b_cl_center_y_35gam;   //!
  TBranch        *b_cl_center_z_35gam;   //!
  TBranch        *b_cl_center_lambda_35gam;   //!
  TBranch        *b_cl_lateral_35gam;   //!
  TBranch        *b_cl_longitudinal_35gam;   //!
  TBranch        *b_cl_eng_frac_em_35gam;   //!
  TBranch        *b_cl_eng_frac_max_35gam;   //!
  TBranch        *b_cl_eng_frac_core_35gam;   //!
  TBranch        *b_cl_m1_dens_35gam;   //!
  TBranch        *b_cl_m2_dens_35gam;   //!
  TBranch        *b_cl_ntotcells_35gam;   //!
  TBranch        *b_cl_ecluster_37;   //!
  TBranch        *b_cl_nctotal_37;   //!
  TBranch        *b_cl_nc_37;   //!
  TBranch        *b_cl_et_37;   //!
  TBranch        *b_cl_e_37;   //!
  TBranch        *b_cl_eemb0_37;   //!
  TBranch        *b_cl_eemb1_37;   //!
  TBranch        *b_cl_eemb2_37;   //!
  TBranch        *b_cl_eemb3_37;   //!
  TBranch        *b_cl_nemb0_37;   //!
  TBranch        *b_cl_nemb1_37;   //!
  TBranch        *b_cl_nemb2_37;   //!
  TBranch        *b_cl_nemb3_37;   //!
  TBranch        *b_cl_phi2_37;   //!
  TBranch        *b_cl_eta0_37;   //!
  TBranch        *b_cl_eta1_37;   //!
  TBranch        *b_cl_eta2_37;   //!
  TBranch        *b_cl_eta3_37;   //!
  TBranch        *b_cl_eeme0_37;   //!
  TBranch        *b_cl_eeme1_37;   //!
  TBranch        *b_cl_eeme2_37;   //!
  TBranch        *b_cl_eeme3_37;   //!
  TBranch        *b_cl_neme0_37;   //!
  TBranch        *b_cl_neme1_37;   //!
  TBranch        *b_cl_neme2_37;   //!
  TBranch        *b_cl_neme3_37;   //!
  TBranch        *b_cl_etileg1_37;   //!
  TBranch        *b_cl_etileg2_37;   //!
  TBranch        *b_cl_etileg3_37;   //!
  TBranch        *b_cl_ntileg1_37;   //!
  TBranch        *b_cl_ntileg2_37;   //!
  TBranch        *b_cl_ntileg3_37;   //!
  TBranch        *b_cl_eta_37;   //!
  TBranch        *b_cl_phi_37;   //!
  TBranch        *b_cl_time_37;   //!
  TBranch        *b_cl_reco_stat_37;   //!
  TBranch        *b_cl_m1_eta_37;   //!
  TBranch        *b_cl_m1_phi_37;   //!
  TBranch        *b_cl_m2_r_37;   //!
  TBranch        *b_cl_m2_lambda_37;   //!
  TBranch        *b_cl_delta_phi_37;   //!
  TBranch        *b_cl_delta_theta_37;   //!
  TBranch        *b_cl_delta_alpha_37;   //!
  TBranch        *b_cl_center_x_37;   //!
  TBranch        *b_cl_center_y_37;   //!
  TBranch        *b_cl_center_z_37;   //!
  TBranch        *b_cl_center_lambda_37;   //!
  TBranch        *b_cl_lateral_37;   //!
  TBranch        *b_cl_longitudinal_37;   //!
  TBranch        *b_cl_eng_frac_em_37;   //!
  TBranch        *b_cl_eng_frac_max_37;   //!
  TBranch        *b_cl_eng_frac_core_37;   //!
  TBranch        *b_cl_m1_dens_37;   //!
  TBranch        *b_cl_m2_dens_37;   //!
  TBranch        *b_cl_ntotcells_37;   //!
  TBranch        *b_cl_ecluster_37gam;   //!
  TBranch        *b_cl_nctotal_37gam;   //!
  TBranch        *b_cl_nc_37gam;   //!
  TBranch        *b_cl_et_37gam;   //!
  TBranch        *b_cl_e_37gam;   //!
  TBranch        *b_cl_eemb0_37gam;   //!
  TBranch        *b_cl_eemb1_37gam;   //!
  TBranch        *b_cl_eemb2_37gam;   //!
  TBranch        *b_cl_eemb3_37gam;   //!
  TBranch        *b_cl_nemb0_37gam;   //!
  TBranch        *b_cl_nemb1_37gam;   //!
  TBranch        *b_cl_nemb2_37gam;   //!
  TBranch        *b_cl_nemb3_37gam;   //!
  TBranch        *b_cl_phi2_37gam;   //!
  TBranch        *b_cl_eta0_37gam;   //!
  TBranch        *b_cl_eta1_37gam;   //!
  TBranch        *b_cl_eta2_37gam;   //!
  TBranch        *b_cl_eta3_37gam;   //!
  TBranch        *b_cl_eeme0_37gam;   //!
  TBranch        *b_cl_eeme1_37gam;   //!
  TBranch        *b_cl_eeme2_37gam;   //!
  TBranch        *b_cl_eeme3_37gam;   //!
  TBranch        *b_cl_neme0_37gam;   //!
  TBranch        *b_cl_neme1_37gam;   //!
  TBranch        *b_cl_neme2_37gam;   //!
  TBranch        *b_cl_neme3_37gam;   //!
  TBranch        *b_cl_etileg1_37gam;   //!
  TBranch        *b_cl_etileg2_37gam;   //!
  TBranch        *b_cl_etileg3_37gam;   //!
  TBranch        *b_cl_ntileg1_37gam;   //!
  TBranch        *b_cl_ntileg2_37gam;   //!
  TBranch        *b_cl_ntileg3_37gam;   //!
  TBranch        *b_cl_eta_37gam;   //!
  TBranch        *b_cl_phi_37gam;   //!
  TBranch        *b_cl_time_37gam;   //!
  TBranch        *b_cl_reco_stat_37gam;   //!
  TBranch        *b_cl_m1_eta_37gam;   //!
  TBranch        *b_cl_m1_phi_37gam;   //!
  TBranch        *b_cl_m2_r_37gam;   //!
  TBranch        *b_cl_m2_lambda_37gam;   //!
  TBranch        *b_cl_delta_phi_37gam;   //!
  TBranch        *b_cl_delta_theta_37gam;   //!
  TBranch        *b_cl_delta_alpha_37gam;   //!
  TBranch        *b_cl_center_x_37gam;   //!
  TBranch        *b_cl_center_y_37gam;   //!
  TBranch        *b_cl_center_z_37gam;   //!
  TBranch        *b_cl_center_lambda_37gam;   //!
  TBranch        *b_cl_lateral_37gam;   //!
  TBranch        *b_cl_longitudinal_37gam;   //!
  TBranch        *b_cl_eng_frac_em_37gam;   //!
  TBranch        *b_cl_eng_frac_max_37gam;   //!
  TBranch        *b_cl_eng_frac_core_37gam;   //!
  TBranch        *b_cl_m1_dens_37gam;   //!
  TBranch        *b_cl_m2_dens_37gam;   //!
  TBranch        *b_cl_ntotcells_37gam;   //!
  TBranch        *b_cl_ecluster_Softe;   //!
  TBranch        *b_cl_nctotal_Softe;   //!
  TBranch        *b_cl_nc_Softe;   //!
  TBranch        *b_cl_et_Softe;   //!
  TBranch        *b_cl_e_Softe;   //!
  TBranch        *b_cl_eemb0_Softe;   //!
  TBranch        *b_cl_eemb1_Softe;   //!
  TBranch        *b_cl_eemb2_Softe;   //!
  TBranch        *b_cl_eemb3_Softe;   //!
  TBranch        *b_cl_nemb0_Softe;   //!
  TBranch        *b_cl_nemb1_Softe;   //!
  TBranch        *b_cl_nemb2_Softe;   //!
  TBranch        *b_cl_nemb3_Softe;   //!
  TBranch        *b_cl_phi2_Softe;   //!
  TBranch        *b_cl_eta0_Softe;   //!
  TBranch        *b_cl_eta1_Softe;   //!
  TBranch        *b_cl_eta2_Softe;   //!
  TBranch        *b_cl_eta3_Softe;   //!
  TBranch        *b_cl_eeme0_Softe;   //!
  TBranch        *b_cl_eeme1_Softe;   //!
  TBranch        *b_cl_eeme2_Softe;   //!
  TBranch        *b_cl_eeme3_Softe;   //!
  TBranch        *b_cl_neme0_Softe;   //!
  TBranch        *b_cl_neme1_Softe;   //!
  TBranch        *b_cl_neme2_Softe;   //!
  TBranch        *b_cl_neme3_Softe;   //!
  TBranch        *b_cl_etileg1_Softe;   //!
  TBranch        *b_cl_etileg2_Softe;   //!
  TBranch        *b_cl_etileg3_Softe;   //!
  TBranch        *b_cl_ntileg1_Softe;   //!
  TBranch        *b_cl_ntileg2_Softe;   //!
  TBranch        *b_cl_ntileg3_Softe;   //!
  TBranch        *b_cl_eta_Softe;   //!
  TBranch        *b_cl_phi_Softe;   //!
  TBranch        *b_cl_time_Softe;   //!
  TBranch        *b_cl_reco_stat_Softe;   //!
  TBranch        *b_cl_m1_eta_Softe;   //!
  TBranch        *b_cl_m1_phi_Softe;   //!
  TBranch        *b_cl_m2_r_Softe;   //!
  TBranch        *b_cl_m2_lambda_Softe;   //!
  TBranch        *b_cl_delta_phi_Softe;   //!
  TBranch        *b_cl_delta_theta_Softe;   //!
  TBranch        *b_cl_delta_alpha_Softe;   //!
  TBranch        *b_cl_center_x_Softe;   //!
  TBranch        *b_cl_center_y_Softe;   //!
  TBranch        *b_cl_center_z_Softe;   //!
  TBranch        *b_cl_center_lambda_Softe;   //!
  TBranch        *b_cl_lateral_Softe;   //!
  TBranch        *b_cl_longitudinal_Softe;   //!
  TBranch        *b_cl_eng_frac_em_Softe;   //!
  TBranch        *b_cl_eng_frac_max_Softe;   //!
  TBranch        *b_cl_eng_frac_core_Softe;   //!
  TBranch        *b_cl_m1_dens_Softe;   //!
  TBranch        *b_cl_m2_dens_Softe;   //!
  TBranch        *b_cl_ntotcells_Softe;   //!
  TBranch        *b_cl_ecluster_comb;   //!
  TBranch        *b_cl_nctotal_comb;   //!
  TBranch        *b_cl_nc_comb;   //!
  TBranch        *b_cl_et_comb;   //!
  TBranch        *b_cl_e_comb;   //!
  TBranch        *b_cl_eemb0_comb;   //!
  TBranch        *b_cl_eemb1_comb;   //!
  TBranch        *b_cl_eemb2_comb;   //!
  TBranch        *b_cl_eemb3_comb;   //!
  TBranch        *b_cl_nemb0_comb;   //!
  TBranch        *b_cl_nemb1_comb;   //!
  TBranch        *b_cl_nemb2_comb;   //!
  TBranch        *b_cl_nemb3_comb;   //!
  TBranch        *b_cl_phi2_comb;   //!
  TBranch        *b_cl_eta0_comb;   //!
  TBranch        *b_cl_eta1_comb;   //!
  TBranch        *b_cl_eta2_comb;   //!
  TBranch        *b_cl_eta3_comb;   //!
  TBranch        *b_cl_eeme0_comb;   //!
  TBranch        *b_cl_eeme1_comb;   //!
  TBranch        *b_cl_eeme2_comb;   //!
  TBranch        *b_cl_eeme3_comb;   //!
  TBranch        *b_cl_neme0_comb;   //!
  TBranch        *b_cl_neme1_comb;   //!
  TBranch        *b_cl_neme2_comb;   //!
  TBranch        *b_cl_neme3_comb;   //!
  TBranch        *b_cl_etileg1_comb;   //!
  TBranch        *b_cl_etileg2_comb;   //!
  TBranch        *b_cl_etileg3_comb;   //!
  TBranch        *b_cl_ntileg1_comb;   //!
  TBranch        *b_cl_ntileg2_comb;   //!
  TBranch        *b_cl_ntileg3_comb;   //!
  TBranch        *b_cl_eta_comb;   //!
  TBranch        *b_cl_phi_comb;   //!
  TBranch        *b_cl_time_comb;   //!
  TBranch        *b_cl_reco_stat_comb;   //!
  TBranch        *b_cl_m1_eta_comb;   //!
  TBranch        *b_cl_m1_phi_comb;   //!
  TBranch        *b_cl_m2_r_comb;   //!
  TBranch        *b_cl_m2_lambda_comb;   //!
  TBranch        *b_cl_delta_phi_comb;   //!
  TBranch        *b_cl_delta_theta_comb;   //!
  TBranch        *b_cl_delta_alpha_comb;   //!
  TBranch        *b_cl_center_x_comb;   //!
  TBranch        *b_cl_center_y_comb;   //!
  TBranch        *b_cl_center_z_comb;   //!
  TBranch        *b_cl_center_lambda_comb;   //!
  TBranch        *b_cl_lateral_comb;   //!
  TBranch        *b_cl_longitudinal_comb;   //!
  TBranch        *b_cl_eng_frac_em_comb;   //!
  TBranch        *b_cl_eng_frac_max_comb;   //!
  TBranch        *b_cl_eng_frac_core_comb;   //!
  TBranch        *b_cl_m1_dens_comb;   //!
  TBranch        *b_cl_m2_dens_comb;   //!
  TBranch        *b_cl_ehec0_comb;   //!
  TBranch        *b_cl_ehec1_comb;   //!
  TBranch        *b_cl_ehec2_comb;   //!
  TBranch        *b_cl_ehec3_comb;   //!
  TBranch        *b_cl_nhec0_comb;   //!
  TBranch        *b_cl_nhec1_comb;   //!
  TBranch        *b_cl_nhec2_comb;   //!
  TBranch        *b_cl_nhec3_comb;   //!
  TBranch        *b_cl_etileb0_comb;   //!
  TBranch        *b_cl_etileb1_comb;   //!
  TBranch        *b_cl_etileb2_comb;   //!
  TBranch        *b_cl_ntileb0_comb;   //!
  TBranch        *b_cl_ntileb1_comb;   //!
  TBranch        *b_cl_ntileb2_comb;   //!
  TBranch        *b_cl_etilee0_comb;   //!
  TBranch        *b_cl_etilee1_comb;   //!
  TBranch        *b_cl_etilee2_comb;   //!
  TBranch        *b_cl_efcal0_comb;   //!
  TBranch        *b_cl_efcal1_comb;   //!
  TBranch        *b_cl_efcal2_comb;   //!
  TBranch        *b_cl_ntilee0_comb;   //!
  TBranch        *b_cl_ntilee1_comb;   //!
  TBranch        *b_cl_ntilee2_comb;   //!
  TBranch        *b_cl_nfcal0_comb;   //!
  TBranch        *b_cl_nfcal1_comb;   //!
  TBranch        *b_cl_nfcal2_comb;   //!
  TBranch        *b_cl_ntotcells_comb;   //!
  TBranch        *b_cl_ecluster_topoEM;   //!
  TBranch        *b_cl_nctotal_topoEM;   //!
  TBranch        *b_cl_nc_topoEM;   //!
  TBranch        *b_cl_et_topoEM;   //!
  TBranch        *b_cl_e_topoEM;   //!
  TBranch        *b_cl_eemb0_topoEM;   //!
  TBranch        *b_cl_eemb1_topoEM;   //!
  TBranch        *b_cl_eemb2_topoEM;   //!
  TBranch        *b_cl_eemb3_topoEM;   //!
  TBranch        *b_cl_nemb0_topoEM;   //!
  TBranch        *b_cl_nemb1_topoEM;   //!
  TBranch        *b_cl_nemb2_topoEM;   //!
  TBranch        *b_cl_nemb3_topoEM;   //!
  TBranch        *b_cl_phi2_topoEM;   //!
  TBranch        *b_cl_eta0_topoEM;   //!
  TBranch        *b_cl_eta1_topoEM;   //!
  TBranch        *b_cl_eta2_topoEM;   //!
  TBranch        *b_cl_eta3_topoEM;   //!
  TBranch        *b_cl_eeme0_topoEM;   //!
  TBranch        *b_cl_eeme1_topoEM;   //!
  TBranch        *b_cl_eeme2_topoEM;   //!
  TBranch        *b_cl_eeme3_topoEM;   //!
  TBranch        *b_cl_neme0_topoEM;   //!
  TBranch        *b_cl_neme1_topoEM;   //!
  TBranch        *b_cl_neme2_topoEM;   //!
  TBranch        *b_cl_neme3_topoEM;   //!
  TBranch        *b_cl_etileg1_topoEM;   //!
  TBranch        *b_cl_etileg2_topoEM;   //!
  TBranch        *b_cl_etileg3_topoEM;   //!
  TBranch        *b_cl_ntileg1_topoEM;   //!
  TBranch        *b_cl_ntileg2_topoEM;   //!
  TBranch        *b_cl_ntileg3_topoEM;   //!
  TBranch        *b_cl_eta_topoEM;   //!
  TBranch        *b_cl_phi_topoEM;   //!
  TBranch        *b_cl_time_topoEM;   //!
  TBranch        *b_cl_reco_stat_topoEM;   //!
  TBranch        *b_cl_m1_eta_topoEM;   //!
  TBranch        *b_cl_m1_phi_topoEM;   //!
  TBranch        *b_cl_m2_r_topoEM;   //!
  TBranch        *b_cl_m2_lambda_topoEM;   //!
  TBranch        *b_cl_delta_phi_topoEM;   //!
  TBranch        *b_cl_delta_theta_topoEM;   //!
  TBranch        *b_cl_delta_alpha_topoEM;   //!
  TBranch        *b_cl_center_x_topoEM;   //!
  TBranch        *b_cl_center_y_topoEM;   //!
  TBranch        *b_cl_center_z_topoEM;   //!
  TBranch        *b_cl_center_lambda_topoEM;   //!
  TBranch        *b_cl_lateral_topoEM;   //!
  TBranch        *b_cl_longitudinal_topoEM;   //!
  TBranch        *b_cl_eng_frac_em_topoEM;   //!
  TBranch        *b_cl_eng_frac_max_topoEM;   //!
  TBranch        *b_cl_eng_frac_core_topoEM;   //!
  TBranch        *b_cl_m1_dens_topoEM;   //!
  TBranch        *b_cl_m2_dens_topoEM;   //!
  TBranch        *b_cl_ntotcells_topoEM;   //!
  TBranch        *b_IS_Nsgins;   //!
  TBranch        *b_IS_Xpt;   //!
  TBranch        *b_IS_Ypt;   //!
  TBranch        *b_IS_Zpt;   //!
  TBranch        *b_IS_Xvec;   //!
  TBranch        *b_IS_Yvec;   //!
  TBranch        *b_IS_Zvec;   //!
  TBranch        *b_IS_QualityFactor;   //!
  TBranch        *b_IS_NbreOfMultiLayer;   //!
  TBranch        *b_IS_TrackSharing;   //!
  TBranch        *b_IS_StationName;   //!
  TBranch        *b_IS_Ndigits;   //!
  TBranch        *b_MB_NTrk;   //!
  TBranch        *b_VT_NTrk;   //!
  TBranch        *b_VT_A0;   //!
  TBranch        *b_VT_Z;   //!
  TBranch        *b_VT_Phi;   //!
  TBranch        *b_VT_CotTh;   //!
  TBranch        *b_VT_PTInv;   //!
  TBranch        *b_VT_Cov11;   //!
  TBranch        *b_VT_Cov21;   //!
  TBranch        *b_VT_Cov22;   //!
  TBranch        *b_VT_Cov31;   //!
  TBranch        *b_VT_Cov32;   //!
  TBranch        *b_VT_Cov33;   //!
  TBranch        *b_VT_Cov41;   //!
  TBranch        *b_VT_Cov42;   //!
  TBranch        *b_VT_Cov43;   //!
  TBranch        *b_VT_Cov44;   //!
  TBranch        *b_VT_Cov51;   //!
  TBranch        *b_VT_Cov52;   //!
  TBranch        *b_VT_Cov53;   //!
  TBranch        *b_VT_Cov54;   //!
  TBranch        *b_VT_Cov55;   //!
  TBranch        *b_VT_Xpt;   //!
  TBranch        *b_VT_Ypt;   //!
  TBranch        *b_VT_Zpt;   //!
  TBranch        *b_VT_Xvec;   //!
  TBranch        *b_VT_Yvec;   //!
  TBranch        *b_VT_Zvec;   //!
  TBranch        *b_VT_Mom;   //!
  TBranch        *b_VT_digits;   //!
  TBranch        *b_KE_NTrk;   //!
  TBranch        *b_KE_Thept;   //!
  TBranch        *b_KE_Phipt;   //!
  TBranch        *b_KE_Thevc;   //!
  TBranch        *b_KE_Phivc;   //!
  TBranch        *b_KE_PInv;   //!
  TBranch        *b_KE_Cov11;   //!
  TBranch        *b_KE_Cov21;   //!
  TBranch        *b_KE_Cov22;   //!
  TBranch        *b_KE_Cov31;   //!
  TBranch        *b_KE_Cov32;   //!
  TBranch        *b_KE_Cov33;   //!
  TBranch        *b_KE_Cov41;   //!
  TBranch        *b_KE_Cov42;   //!
  TBranch        *b_KE_Cov43;   //!
  TBranch        *b_KE_Cov44;   //!
  TBranch        *b_KE_Cov51;   //!
  TBranch        *b_KE_Cov52;   //!
  TBranch        *b_KE_Cov53;   //!
  TBranch        *b_KE_Cov54;   //!
  TBranch        *b_KE_Cov55;   //!
  TBranch        *b_KE_Xpt;   //!
  TBranch        *b_KE_Ypt;   //!
  TBranch        *b_KE_Zpt;   //!
  TBranch        *b_KE_Xvec;   //!
  TBranch        *b_KE_Yvec;   //!
  TBranch        *b_KE_Zvec;   //!
  TBranch        *b_KE_Mom;   //!
  TBranch        *b_KE_ZCyl;   //!
  TBranch        *b_KE_RCyl;   //!
  TBranch        *b_MS_NTrk;   //!
  TBranch        *b_MS_Thept;   //!
  TBranch        *b_MS_Phipt;   //!
  TBranch        *b_MS_Thevc;   //!
  TBranch        *b_MS_Phivc;   //!
  TBranch        *b_MS_PInv;   //!
  TBranch        *b_MS_Cov11;   //!
  TBranch        *b_MS_Cov21;   //!
  TBranch        *b_MS_Cov22;   //!
  TBranch        *b_MS_Cov31;   //!
  TBranch        *b_MS_Cov32;   //!
  TBranch        *b_MS_Cov33;   //!
  TBranch        *b_MS_Cov41;   //!
  TBranch        *b_MS_Cov42;   //!
  TBranch        *b_MS_Cov43;   //!
  TBranch        *b_MS_Cov44;   //!
  TBranch        *b_MS_Cov51;   //!
  TBranch        *b_MS_Cov52;   //!
  TBranch        *b_MS_Cov53;   //!
  TBranch        *b_MS_Cov54;   //!
  TBranch        *b_MS_Cov55;   //!
  TBranch        *b_MS_Xpt;   //!
  TBranch        *b_MS_Ypt;   //!
  TBranch        *b_MS_Zpt;   //!
  TBranch        *b_MS_Xvec;   //!
  TBranch        *b_MS_Yvec;   //!
  TBranch        *b_MS_Zvec;   //!
  TBranch        *b_MS_Mom;   //!
  TBranch        *b_MS_ZCyl;   //!
  TBranch        *b_MS_RCyl;   //!
  TBranch        *b_DR1_NTrk;   //!
  TBranch        *b_MbKE_Calo_En_dr01;   //!
  TBranch        *b_MbKE_Calo_Et_dr01;   //!
  TBranch        *b_MbKE_HAD_Calo_En_dr01;   //!
  TBranch        *b_MbKE_HAD_Calo_Et_dr01;   //!
  TBranch        *b_MbKE_EM_Calo_En_dr01;   //!
  TBranch        *b_MbKE_EM_Calo_Et_dr01;   //!
  TBranch        *b_Mboy_nmuon;   //!
  TBranch        *b_Mboy_A0;   //!
  TBranch        *b_Mboy_Z;   //!
  TBranch        *b_Mboy_Phi;   //!
  TBranch        *b_Mboy_Theta;   //!
  TBranch        *b_Mboy_qOverP;   //!
  TBranch        *b_Mboy_covr11;   //!
  TBranch        *b_Mboy_covr21;   //!
  TBranch        *b_Mboy_covr22;   //!
  TBranch        *b_Mboy_covr31;   //!
  TBranch        *b_Mboy_covr32;   //!
  TBranch        *b_Mboy_covr33;   //!
  TBranch        *b_Mboy_covr41;   //!
  TBranch        *b_Mboy_covr42;   //!
  TBranch        *b_Mboy_covr43;   //!
  TBranch        *b_Mboy_covr44;   //!
  TBranch        *b_Mboy_covr51;   //!
  TBranch        *b_Mboy_covr52;   //!
  TBranch        *b_Mboy_covr53;   //!
  TBranch        *b_Mboy_covr54;   //!
  TBranch        *b_Mboy_covr55;   //!
  TBranch        *b_Mboy_etcone10;   //!
  TBranch        *b_Mboy_etcone20;   //!
  TBranch        *b_Mboy_etcone30;   //!
  TBranch        *b_Mboy_etcone40;   //!
  TBranch        *b_Mboy_etcone;   //!
  TBranch        *b_Mboy_etcone50;   //!
  TBranch        *b_Mboy_etcone60;   //!
  TBranch        *b_Mboy_etcone70;   //!
  TBranch        *b_Mboy_nucone10;   //!
  TBranch        *b_Mboy_nucone20;   //!
  TBranch        *b_Mboy_nucone30;   //!
  TBranch        *b_Mboy_nucone40;   //!
  TBranch        *b_Mboy_nucone;   //!
  TBranch        *b_Mboy_nucone50;   //!
  TBranch        *b_Mboy_nucone60;   //!
  TBranch        *b_Mboy_nucone70;   //!
  TBranch        *b_Mboy_Eloss;   //!
  TBranch        *b_Mboy_ElossError;   //!
  TBranch        *b_Moore_nkine;   //!
  TBranch        *b_Moore_xvtxg;   //!
  TBranch        *b_Moore_yvtxg;   //!
  TBranch        *b_Moore_zvtxg;   //!
  TBranch        *b_Moore_a0g;   //!
  TBranch        *b_Moore_z0g;   //!
  TBranch        *b_Moore_phig;   //!
  TBranch        *b_Moore_cotthg;   //!
  TBranch        *b_Moore_ptig;   //!
  TBranch        *b_Moore_etag;   //!
  TBranch        *b_Moore_rassociated;   //!
  TBranch        *b_Moore_matchdg;   //!
  TBranch        *b_Moore_xextrg;   //!
  TBranch        *b_Moore_yextrg;   //!
  TBranch        *b_Moore_zextrg;   //!
  TBranch        *b_Moore_a0extrg;   //!
  TBranch        *b_Moore_z0extrg;   //!
  TBranch        *b_Moore_phiextrg;   //!
  TBranch        *b_Moore_cthextrg;   //!
  TBranch        *b_Moore_ptiextrg;   //!
  TBranch        *b_Moore_etaextrg;   //!
  TBranch        *b_Moore_barcodeg;   //!
  TBranch        *b_Moore_statusg;   //!
  TBranch        *b_Moore_elossg;   //!
  TBranch        *b_Moore_drphig;   //!
  TBranch        *b_Moore_drzg;   //!
  TBranch        *b_Moore_dphig;   //!
  TBranch        *b_Moore_dthetag;   //!
  TBranch        *b_Moore_a0pullg;   //!
  TBranch        *b_Moore_z0pullg;   //!
  TBranch        *b_Moore_phipullg;   //!
  TBranch        *b_Moore_cotthpullg;   //!
  TBranch        *b_Moore_ptipullg;   //!
  TBranch        *b_Moore_nmuon;   //!
  TBranch        *b_Moore_phih;   //!
  TBranch        *b_Moore_etah;   //!
  TBranch        *b_Moore_mdth;   //!
  TBranch        *b_Moore_cscetah;   //!
  TBranch        *b_Moore_cscphih;   //!
  TBranch        *b_Moore_rpcetah;   //!
  TBranch        *b_Moore_rpcphih;   //!
  TBranch        *b_Moore_tgcetah;   //!
  TBranch        *b_Moore_tgcphih;   //!
  TBranch        *b_Moore_stati;   //!
  TBranch        *b_Moore_statr;   //!
  TBranch        *b_Moore_xvtxr;   //!
  TBranch        *b_Moore_yvtxr;   //!
  TBranch        *b_Moore_zvtxr;   //!
  TBranch        *b_Moore_a0r;   //!
  TBranch        *b_Moore_z0r;   //!
  TBranch        *b_Moore_phir;   //!
  TBranch        *b_Moore_cotthr;   //!
  TBranch        *b_Moore_ptir;   //!
  TBranch        *b_Moore_etar;   //!
  TBranch        *b_Moore_chi2;   //!
  TBranch        *b_Moore_chi2pr;   //!
  TBranch        *b_Moore_origx;   //!
  TBranch        *b_Moore_origy;   //!
  TBranch        *b_Moore_origz;   //!
  TBranch        *b_Moore_covr11;   //!
  TBranch        *b_Moore_covr12;   //!
  TBranch        *b_Moore_covr22;   //!
  TBranch        *b_Moore_covr13;   //!
  TBranch        *b_Moore_covr23;   //!
  TBranch        *b_Moore_covr33;   //!
  TBranch        *b_Moore_covr14;   //!
  TBranch        *b_Moore_covr24;   //!
  TBranch        *b_Moore_covr34;   //!
  TBranch        *b_Moore_covr44;   //!
  TBranch        *b_Moore_covr15;   //!
  TBranch        *b_Moore_covr25;   //!
  TBranch        *b_Moore_covr35;   //!
  TBranch        *b_Moore_covr45;   //!
  TBranch        *b_Moore_covr55;   //!
  TBranch        *b_conv_numVertices;   //!
  TBranch        *b_conv_vtx_x;   //!
  TBranch        *b_conv_vtx_y;   //!
  TBranch        *b_conv_vtx_z;   //!
  TBranch        *b_conv_is_conv;   //!
  TBranch        *b_conv_vtx_x_truth;   //!
  TBranch        *b_conv_vtx_y_truth;   //!
  TBranch        *b_conv_vtx_z_truth;   //!
  TBranch        *b_conv_sigvtx_x;   //!
  TBranch        *b_conv_sigvtx_y;   //!
  TBranch        *b_conv_sigvtx_z;   //!
  TBranch        *b_conv_vtxchi2;   //!
  TBranch        *b_conv_vtxndf;   //!
  TBranch        *b_conv_vtxchi2prob;   //!
  TBranch        *b_conv_numTracksPerVertex;   //!
  TBranch        *b_conv_track1_chi2;   //!
  TBranch        *b_conv_track2_chi2;   //!
  TBranch        *b_conv_track1_d0;   //!
  TBranch        *b_conv_track1_sigd0;   //!
  TBranch        *b_conv_track1_z0;   //!
  TBranch        *b_conv_track1_sigz0;   //!
  TBranch        *b_conv_track1_phi;   //!
  TBranch        *b_conv_track1_sigphi;   //!
  TBranch        *b_conv_track1_theta;   //!
  TBranch        *b_conv_track1_sigtheta;   //!
  TBranch        *b_conv_track1_qOverP;   //!
  TBranch        *b_conv_track1_sigqOverP;   //!
  TBranch        *b_conv_track1_momentum_x;   //!
  TBranch        *b_conv_track1_momentum_y;   //!
  TBranch        *b_conv_track1_momentum_z;   //!
  TBranch        *b_conv_track1_d0g;   //!
  TBranch        *b_conv_track1_sigd0g;   //!
  TBranch        *b_conv_track1_z0g;   //!
  TBranch        *b_conv_track1_sigz0g;   //!
  TBranch        *b_conv_track2_d0;   //!
  TBranch        *b_conv_track2_sigd0;   //!
  TBranch        *b_conv_track2_z0;   //!
  TBranch        *b_conv_track2_sigz0;   //!
  TBranch        *b_conv_track2_phi;   //!
  TBranch        *b_conv_track2_sigphi;   //!
  TBranch        *b_conv_track2_theta;   //!
  TBranch        *b_conv_track2_sigtheta;   //!
  TBranch        *b_conv_track2_qOverP;   //!
  TBranch        *b_conv_track2_sigqOverP;   //!
  TBranch        *b_conv_track2_momentum_x;   //!
  TBranch        *b_conv_track2_momentum_y;   //!
  TBranch        *b_conv_track2_momentum_z;   //!
  TBranch        *b_conv_track2_d0g;   //!
  TBranch        *b_conv_track2_sigd0g;   //!
  TBranch        *b_conv_track2_z0g;   //!
  TBranch        *b_conv_track2_sigz0g;   //!
  TBranch        *b_conv_numOfTruthConversions;   //!
  TBranch        *b_conv_true_vtx_x;   //!
  TBranch        *b_conv_true_vtx_y;   //!
  TBranch        *b_conv_true_vtx_z;   //!
  TBranch        *b_CJetsjetNum;   //!
  TBranch        *b_jetEtaCJets;   //!
  TBranch        *b_jetPhiCJets;   //!
  TBranch        *b_jetECJets;   //!
  TBranch        *b_jetEemCJets;   //!
  TBranch        *b_jetEtCJets;   //!
  TBranch        *b_jetMCJets;   //!
  TBranch        *b_jetPxCJets;   //!
  TBranch        *b_jetPyCJets;   //!
  TBranch        *b_jetPzCJets;   //!
  TBranch        *b_jetSizeCJets;   //!
  TBranch        *b_C4JetsjetNum;   //!
  TBranch        *b_jetEtaC4Jets;   //!
  TBranch        *b_jetPhiC4Jets;   //!
  TBranch        *b_jetEC4Jets;   //!
  TBranch        *b_jetEemC4Jets;   //!
  TBranch        *b_jetEtC4Jets;   //!
  TBranch        *b_jetMC4Jets;   //!
  TBranch        *b_jetPxC4Jets;   //!
  TBranch        *b_jetPyC4Jets;   //!
  TBranch        *b_jetPzC4Jets;   //!
  TBranch        *b_jetSizeC4Jets;   //!
  TBranch        *b_Kt6JetsjetNum;   //!
  TBranch        *b_jetEtaKt6Jets;   //!
  TBranch        *b_jetPhiKt6Jets;   //!
  TBranch        *b_jetEKt6Jets;   //!
  TBranch        *b_jetEemKt6Jets;   //!
  TBranch        *b_jetEtKt6Jets;   //!
  TBranch        *b_jetMKt6Jets;   //!
  TBranch        *b_jetPxKt6Jets;   //!
  TBranch        *b_jetPyKt6Jets;   //!
  TBranch        *b_jetPzKt6Jets;   //!
  TBranch        *b_jetSizeKt6Jets;   //!
  TBranch        *b_Kt4JetsjetNum;   //!
  TBranch        *b_jetEtaKt4Jets;   //!
  TBranch        *b_jetPhiKt4Jets;   //!
  TBranch        *b_jetEKt4Jets;   //!
  TBranch        *b_jetEemKt4Jets;   //!
  TBranch        *b_jetEtKt4Jets;   //!
  TBranch        *b_jetMKt4Jets;   //!
  TBranch        *b_jetPxKt4Jets;   //!
  TBranch        *b_jetPyKt4Jets;   //!
  TBranch        *b_jetPzKt4Jets;   //!
  TBranch        *b_jetSizeKt4Jets;   //!
  TBranch        *b_CTopoJetsjetNum;   //!
  TBranch        *b_jetEtaCTopoJets;   //!
  TBranch        *b_jetPhiCTopoJets;   //!
  TBranch        *b_jetECTopoJets;   //!
  TBranch        *b_jetEemCTopoJets;   //!
  TBranch        *b_jetEtCTopoJets;   //!
  TBranch        *b_jetMCTopoJets;   //!
  TBranch        *b_jetPxCTopoJets;   //!
  TBranch        *b_jetPyCTopoJets;   //!
  TBranch        *b_jetPzCTopoJets;   //!
  TBranch        *b_jetSizeCTopoJets;   //!
  TBranch        *b_C4TopoJetsjetNum;   //!
  TBranch        *b_jetEtaC4TopoJets;   //!
  TBranch        *b_jetPhiC4TopoJets;   //!
  TBranch        *b_jetEC4TopoJets;   //!
  TBranch        *b_jetEemC4TopoJets;   //!
  TBranch        *b_jetEtC4TopoJets;   //!
  TBranch        *b_jetMC4TopoJets;   //!
  TBranch        *b_jetPxC4TopoJets;   //!
  TBranch        *b_jetPyC4TopoJets;   //!
  TBranch        *b_jetPzC4TopoJets;   //!
  TBranch        *b_jetSizeC4TopoJets;   //!
  TBranch        *b_Kt6TopoJetsjetNum;   //!
  TBranch        *b_jetEtaKt6TopoJets;   //!
  TBranch        *b_jetPhiKt6TopoJets;   //!
  TBranch        *b_jetEKt6TopoJets;   //!
  TBranch        *b_jetEemKt6TopoJets;   //!
  TBranch        *b_jetEtKt6TopoJets;   //!
  TBranch        *b_jetMKt6TopoJets;   //!
  TBranch        *b_jetPxKt6TopoJets;   //!
  TBranch        *b_jetPyKt6TopoJets;   //!
  TBranch        *b_jetPzKt6TopoJets;   //!
  TBranch        *b_jetSizeKt6TopoJets;   //!
  TBranch        *b_Kt4TopoJetsjetNum;   //!
  TBranch        *b_jetEtaKt4TopoJets;   //!
  TBranch        *b_jetPhiKt4TopoJets;   //!
  TBranch        *b_jetEKt4TopoJets;   //!
  TBranch        *b_jetEemKt4TopoJets;   //!
  TBranch        *b_jetEtKt4TopoJets;   //!
  TBranch        *b_jetMKt4TopoJets;   //!
  TBranch        *b_jetPxKt4TopoJets;   //!
  TBranch        *b_jetPyKt4TopoJets;   //!
  TBranch        *b_jetPzKt4TopoJets;   //!
  TBranch        *b_jetSizeKt4TopoJets;   //!
  TBranch        *b_CMcJetsjetNum;   //!
  TBranch        *b_jetEtaCMcJets;   //!
  TBranch        *b_jetPhiCMcJets;   //!
  TBranch        *b_jetECMcJets;   //!
  TBranch        *b_jetEemCMcJets;   //!
  TBranch        *b_jetEtCMcJets;   //!
  TBranch        *b_jetMCMcJets;   //!
  TBranch        *b_jetPxCMcJets;   //!
  TBranch        *b_jetPyCMcJets;   //!
  TBranch        *b_jetPzCMcJets;   //!
  TBranch        *b_jetSizeCMcJets;   //!
  TBranch        *b_C4McJetsjetNum;   //!
  TBranch        *b_jetEtaC4McJets;   //!
  TBranch        *b_jetPhiC4McJets;   //!
  TBranch        *b_jetEC4McJets;   //!
  TBranch        *b_jetEemC4McJets;   //!
  TBranch        *b_jetEtC4McJets;   //!
  TBranch        *b_jetMC4McJets;   //!
  TBranch        *b_jetPxC4McJets;   //!
  TBranch        *b_jetPyC4McJets;   //!
  TBranch        *b_jetPzC4McJets;   //!
  TBranch        *b_jetSizeC4McJets;   //!
  TBranch        *b_Kt4McJetsjetNum;   //!
  TBranch        *b_jetEtaKt4McJets;   //!
  TBranch        *b_jetPhiKt4McJets;   //!
  TBranch        *b_jetEKt4McJets;   //!
  TBranch        *b_jetEemKt4McJets;   //!
  TBranch        *b_jetEtKt4McJets;   //!
  TBranch        *b_jetMKt4McJets;   //!
  TBranch        *b_jetPxKt4McJets;   //!
  TBranch        *b_jetPyKt4McJets;   //!
  TBranch        *b_jetPzKt4McJets;   //!
  TBranch        *b_jetSizeKt4McJets;   //!
  TBranch        *b_Kt6McJetsjetNum;   //!
  TBranch        *b_jetEtaKt6McJets;   //!
  TBranch        *b_jetPhiKt6McJets;   //!
  TBranch        *b_jetEKt6McJets;   //!
  TBranch        *b_jetEemKt6McJets;   //!
  TBranch        *b_jetEtKt6McJets;   //!
  TBranch        *b_jetMKt6McJets;   //!
  TBranch        *b_jetPxKt6McJets;   //!
  TBranch        *b_jetPyKt6McJets;   //!
  TBranch        *b_jetPzKt6McJets;   //!
  TBranch        *b_jetSizeKt6McJets;   //!
  TBranch        *b_tau_NCand;   //!
  TBranch        *b_tau_ET;   //!
  TBranch        *b_tau_eta;   //!
  TBranch        *b_tau_phi;   //!
  TBranch        *b_tau_nStrip;   //!
  TBranch        *b_tau_stripWidth2;   //!
  TBranch        *b_tau_EMRadius;   //!
  TBranch        *b_tau_IsoFrac;   //!
  TBranch        *b_tau_ntrack;   //!
  TBranch        *b_tau_charge;   //!
  TBranch        *b_tau_secVtxX;   //!
  TBranch        *b_tau_secVtxY;   //!
  TBranch        *b_tau_secVtxZ;   //!
  TBranch        *b_tau_secVtxXError;   //!
  TBranch        *b_tau_secVtxYError;   //!
  TBranch        *b_tau_secVtxZError;   //!
  TBranch        *b_tau_sumEMe;   //!
  TBranch        *b_tau_sumEMpx;   //!
  TBranch        *b_tau_sumEMpy;   //!
  TBranch        *b_tau_sumEMpz;   //!
  TBranch        *b_tau_energy;   //!
  TBranch        *b_tau_EMenergy;   //!
  TBranch        *b_tau_nEMCell;   //!
  TBranch        *b_tau_stripET;   //!
  TBranch        *b_tau_ptTrack1;   //!
  TBranch        *b_tau_etaTrack1;   //!
  TBranch        *b_tau_phiTrack1;   //!
  TBranch        *b_tau_distTrack1;   //!
  TBranch        *b_tau_ptTrack2;   //!
  TBranch        *b_tau_etaTrack2;   //!
  TBranch        *b_tau_phiTrack2;   //!
  TBranch        *b_tau_distTrack2;   //!
  TBranch        *b_tau_ptTrack3;   //!
  TBranch        *b_tau_etaTrack3;   //!
  TBranch        *b_tau_phiTrack3;   //!
  TBranch        *b_tau_distTrack3;   //!
  TBranch        *b_tau_etEMCalib;   //!
  TBranch        *b_tau_etHadCalib;   //!
  TBranch        *b_tau_Likelihood;   //!
  TBranch        *b_tau_tauJetLL;   //!
  TBranch        *b_tau_tauELL;   //!
  TBranch        *b_tau_tauJetNN;   //!
  TBranch        *b_tau_tauENN;   //!
  TBranch        *b_tau_nTracksdrdR;   //!
  TBranch        *b_tau_accept;   //!
  TBranch        *b_tau1p3p_NCand;   //!
  TBranch        *b_tau1p3p_ET;   //!
  TBranch        *b_tau1p3p_eta;   //!
  TBranch        *b_tau1p3p_phi;   //!
  TBranch        *b_tau1p3p_nStrip;   //!
  TBranch        *b_tau1p3p_stripWidth2;   //!
  TBranch        *b_tau1p3p_EMRadius;   //!
  TBranch        *b_tau1p3p_IsoFrac;   //!
  TBranch        *b_tau1p3p_ntrack;   //!
  TBranch        *b_tau1p3p_charge;   //!
  TBranch        *b_tau1p3p_secVtxX;   //!
  TBranch        *b_tau1p3p_secVtxY;   //!
  TBranch        *b_tau1p3p_secVtxZ;   //!
  TBranch        *b_tau1p3p_secVtxXError;   //!
  TBranch        *b_tau1p3p_secVtxYError;   //!
  TBranch        *b_tau1p3p_secVtxZError;   //!
  TBranch        *b_tau1p3p_sumEMe;   //!
  TBranch        *b_tau1p3p_sumEMpx;   //!
  TBranch        *b_tau1p3p_sumEMpy;   //!
  TBranch        *b_tau1p3p_sumEMpz;   //!
  TBranch        *b_tau1p3p_track1Nt;   //!
  TBranch        *b_tau1p3p_track2Nt;   //!
  TBranch        *b_tau1p3p_track3Nt;   //!
  TBranch        *b_tau1p3p_ptTrack1;   //!
  TBranch        *b_tau1p3p_ptTrack2;   //!
  TBranch        *b_tau1p3p_ptTrack3;   //!
  TBranch        *b_tau1p3p_track1ETChrgEM01;   //!
  TBranch        *b_tau1p3p_track2ETChrgEM01;   //!
  TBranch        *b_tau1p3p_track3ETChrgEM01;   //!
  TBranch        *b_tau1p3p_track1ETResChrgEMTrk;   //!
  TBranch        *b_tau1p3p_track2ETResChrgEMTrk;   //!
  TBranch        *b_tau1p3p_track3ETResChrgEMTrk;   //!
  TBranch        *b_tau1p3p_etEMCL;   //!
  TBranch        *b_tau1p3p_etNeuEM;   //!
  TBranch        *b_tau1p3p_etChrgEM;   //!
  TBranch        *b_tau1p3p_etResNeuEM;   //!
  TBranch        *b_tau1p3p_cellsChrgHAD;   //!
  TBranch        *b_tau1p3p_cellsOtherEM;   //!
  TBranch        *b_tau1p3p_cellsOtherHAD;   //!
  TBranch        *b_tau1p3p_ETeflow;   //!
  TBranch        *b_tau1p3p_ETcalo;   //!
  TBranch        *b_tau1p3p_MVisEflow;   //!
  TBranch        *b_tau1p3p_discriCut;   //!
  TBranch        *b_tau1p3p_discriNN;   //!
  TBranch        *b_tau1p3p_discriPDRS;   //!
  TBranch        *b_tau1p3p_MTrk3P;   //!
  TBranch        *b_tau1p3p_NAssocTracksCore;   //!
  TBranch        *b_tau1p3p_NAssocTracksIsol;   //!
  TBranch        *b_tau1p3p_RWidth2Trk3P;   //!
  TBranch        *b_tau1p3p_SignD0Trk3P;   //!
  TBranch        *b_eg_nc;   //!
  TBranch        *b_eg_e;   //!
  TBranch        *b_eg_eta;   //!
  TBranch        *b_eg_phi;   //!
  TBranch        *b_eg_cl_et;   //!
  TBranch        *b_eg_cl_eta;   //!
  TBranch        *b_eg_cl_phi;   //!
  TBranch        *b_eg_etap;   //!
  TBranch        *b_eg_zvertex;   //!
  TBranch        *b_eg_errz;   //!
  TBranch        *b_eg_depth;   //!
  TBranch        *b_eg_e0;   //!
  TBranch        *b_eg_e1;   //!
  TBranch        *b_eg_e2;   //!
  TBranch        *b_eg_e3;   //!
  TBranch        *b_eg_eta0;   //!
  TBranch        *b_eg_eta1;   //!
  TBranch        *b_eg_eta2;   //!
  TBranch        *b_eg_eta3;   //!
  TBranch        *b_eg_phi0;   //!
  TBranch        *b_eg_phi1;   //!
  TBranch        *b_eg_phi2;   //!
  TBranch        *b_eg_phi3;   //!
  TBranch        *b_eg_Etha1;   //!
  TBranch        *b_eg_Etha;   //!
  TBranch        *b_eg_Eha1;   //!
  TBranch        *b_eg_F1;   //!
  TBranch        *b_eg_F3;   //!
  TBranch        *b_eg_E233;   //!
  TBranch        *b_eg_E237;   //!
  TBranch        *b_eg_E277;   //!
  TBranch        *b_eg_Weta1;   //!
  TBranch        *b_eg_Weta2;   //!
  TBranch        *b_eg_E2ts1;   //!
  TBranch        *b_eg_E2tsts1;   //!
  TBranch        *b_eg_Widths1;   //!
  TBranch        *b_eg_Widths2;   //!
  TBranch        *b_eg_poscs1;   //!
  TBranch        *b_eg_poscs2;   //!
  TBranch        *b_eg_Barys1;   //!
  TBranch        *b_eg_Wtots1;   //!
  TBranch        *b_eg_Emins1;   //!
  TBranch        *b_eg_Emaxs1;   //!
  TBranch        *b_eg_Fracs1;   //!
  TBranch        *b_eg_EtCone45;   //!
  TBranch        *b_eg_EtCone20;   //!
  TBranch        *b_eg_EtCone30;   //!
  TBranch        *b_eg_EtCone40;   //!
  TBranch        *b_eg_IsEM;   //!
  TBranch        *b_eg_epiNN;   //!
  TBranch        *b_eg_EMWeight;   //!
  TBranch        *b_eg_PionWeight;   //!
  TBranch        *b_eg_Hmatrix;   //!
  TBranch        *b_eg_IsEMse;   //!
  TBranch        *b_eg_epiNNse;   //!
  TBranch        *b_eg_EMWeightse;   //!
  TBranch        *b_eg_PionWeightse;   //!
  TBranch        *b_eg_F1core;   //!
  TBranch        *b_eg_F3core;   //!
  TBranch        *b_eg_Asy1;   //!
  TBranch        *b_eg_Pos7;   //!
  TBranch        *b_eg_Isol;   //!
  TBranch        *b_eg_Ecore;   //!
  TBranch        *b_eg_E011;   //!
  TBranch        *b_eg_E033;   //!
  TBranch        *b_eg_E131;   //!
  TBranch        *b_eg_E1153;   //!
  TBranch        *b_eg_E235;   //!
  TBranch        *b_eg_E255;   //!
  TBranch        *b_eg_E333;   //!
  TBranch        *b_eg_E335;   //!
  TBranch        *b_eg_E337;   //!
  TBranch        *b_eg_E377;   //!
  TBranch        *b_eg_trkmatchnt;   //!
  TBranch        *b_eg_primary;   //!
  TBranch        *b_eg_numtracks;   //!
  TBranch        *b_eg_eoverp;   //!
  TBranch        *b_eg_etacorr;   //!
  TBranch        *b_eg_deta0;   //!
  TBranch        *b_eg_dphi0;   //!
  TBranch        *b_eg_deta1;   //!
  TBranch        *b_eg_dphi1;   //!
  TBranch        *b_eg_deta2;   //!
  TBranch        *b_eg_dphi2;   //!
  TBranch        *b_eg_deta3;   //!
  TBranch        *b_eg_dphi3;   //!
  TBranch        *b_eg_trkopt;   //!
  TBranch        *b_eg_trkinvpTV;   //!
  TBranch        *b_eg_trkcotThV;   //!
  TBranch        *b_eg_trkphiV;   //!
  TBranch        *b_eg_convTrkMatch;   //!
  TBranch        *b_eg_convAngleMatch;   //!
  TBranch        *b_eg_nc_SE;   //!
  TBranch        *b_eg_e_SE;   //!
  TBranch        *b_eg_eta_SE;   //!
  TBranch        *b_eg_phi_SE;   //!
  TBranch        *b_eg_cl_et_SE;   //!
  TBranch        *b_eg_cl_eta_SE;   //!
  TBranch        *b_eg_cl_phi_SE;   //!
  TBranch        *b_eg_etap_SE;   //!
  TBranch        *b_eg_zvertex_SE;   //!
  TBranch        *b_eg_errz_SE;   //!
  TBranch        *b_eg_depth_SE;   //!
  TBranch        *b_eg_e0_SE;   //!
  TBranch        *b_eg_e1_SE;   //!
  TBranch        *b_eg_e2_SE;   //!
  TBranch        *b_eg_e3_SE;   //!
  TBranch        *b_eg_eta0_SE;   //!
  TBranch        *b_eg_eta1_SE;   //!
  TBranch        *b_eg_eta2_SE;   //!
  TBranch        *b_eg_eta3_SE;   //!
  TBranch        *b_eg_phi0_SE;   //!
  TBranch        *b_eg_phi1_SE;   //!
  TBranch        *b_eg_phi2_SE;   //!
  TBranch        *b_eg_phi3_SE;   //!
  TBranch        *b_eg_Etha1_SE;   //!
  TBranch        *b_eg_Etha_SE;   //!
  TBranch        *b_eg_Eha1_SE;   //!
  TBranch        *b_eg_F1_SE;   //!
  TBranch        *b_eg_F3_SE;   //!
  TBranch        *b_eg_E233_SE;   //!
  TBranch        *b_eg_E237_SE;   //!
  TBranch        *b_eg_E277_SE;   //!
  TBranch        *b_eg_Weta1_SE;   //!
  TBranch        *b_eg_Weta2_SE;   //!
  TBranch        *b_eg_E2ts1_SE;   //!
  TBranch        *b_eg_E2tsts1_SE;   //!
  TBranch        *b_eg_Widths1_SE;   //!
  TBranch        *b_eg_Widths2_SE;   //!
  TBranch        *b_eg_poscs1_SE;   //!
  TBranch        *b_eg_poscs2_SE;   //!
  TBranch        *b_eg_Barys1_SE;   //!
  TBranch        *b_eg_Wtots1_SE;   //!
  TBranch        *b_eg_Emins1_SE;   //!
  TBranch        *b_eg_Emaxs1_SE;   //!
  TBranch        *b_eg_Fracs1_SE;   //!
  TBranch        *b_eg_EtCone45_SE;   //!
  TBranch        *b_eg_EtCone20_SE;   //!
  TBranch        *b_eg_EtCone30_SE;   //!
  TBranch        *b_eg_EtCone40_SE;   //!
  TBranch        *b_eg_IsEM_SE;   //!
  TBranch        *b_eg_epiNN_SE;   //!
  TBranch        *b_eg_EMWeight_SE;   //!
  TBranch        *b_eg_PionWeight_SE;   //!
  TBranch        *b_eg_Hmatrix_SE;   //!
  TBranch        *b_eg_IsEMse_SE;   //!
  TBranch        *b_eg_epiNNse_SE;   //!
  TBranch        *b_eg_EMWeightse_SE;   //!
  TBranch        *b_eg_PionWeightse_SE;   //!
  TBranch        *b_eg_F1core_SE;   //!
  TBranch        *b_eg_F3core_SE;   //!
  TBranch        *b_eg_Asy1_SE;   //!
  TBranch        *b_eg_Pos7_SE;   //!
  TBranch        *b_eg_Isol_SE;   //!
  TBranch        *b_eg_Ecore_SE;   //!
  TBranch        *b_eg_E011_SE;   //!
  TBranch        *b_eg_E033_SE;   //!
  TBranch        *b_eg_E131_SE;   //!
  TBranch        *b_eg_E1153_SE;   //!
  TBranch        *b_eg_E235_SE;   //!
  TBranch        *b_eg_E255_SE;   //!
  TBranch        *b_eg_E333_SE;   //!
  TBranch        *b_eg_E335_SE;   //!
  TBranch        *b_eg_E337_SE;   //!
  TBranch        *b_eg_E377_SE;   //!
  TBranch        *b_eg_trkmatchnt_SE;   //!
  TBranch        *b_eg_primary_SE;   //!
  TBranch        *b_eg_numtracks_SE;   //!
  TBranch        *b_eg_eoverp_SE;   //!
  TBranch        *b_eg_etacorr_SE;   //!
  TBranch        *b_eg_deta0_SE;   //!
  TBranch        *b_eg_dphi0_SE;   //!
  TBranch        *b_eg_deta1_SE;   //!
  TBranch        *b_eg_dphi1_SE;   //!
  TBranch        *b_eg_deta2_SE;   //!
  TBranch        *b_eg_dphi2_SE;   //!
  TBranch        *b_eg_deta3_SE;   //!
  TBranch        *b_eg_dphi3_SE;   //!
  TBranch        *b_eg_trkopt_SE;   //!
  TBranch        *b_eg_trkinvpTV_SE;   //!
  TBranch        *b_eg_trkcotThV_SE;   //!
  TBranch        *b_eg_trkphiV_SE;   //!
  TBranch        *b_eg_convTrkMatch_SE;   //!
  TBranch        *b_eg_convAngleMatch_SE;   //!
  TBranch        *b_egtruth_nc;   //!
  TBranch        *b_egtruth_et;   //!
  TBranch        *b_egtruth_eta;   //!
  TBranch        *b_egtruth_phi;   //!
  TBranch        *b_egtruth_id;   //!
  TBranch        *b_egtruth_barcode;   //!
  TBranch        *b_egtruth_truthnt;   //!
  TBranch        *b_egtruth_etIsol;   //!
  TBranch        *b_egtruth_etaCalo;   //!
  TBranch        *b_egtruth_phiCalo;   //!
  TBranch        *b_NTileMuonTag;   //!
  TBranch        *b_EtaTileMuonTag;   //!
  TBranch        *b_PhiTileMuonTag;   //!
  TBranch        *b_EnergyVecTileMuonTag;   //!
  TBranch        *b_LabelTileMuonTag;   //!
  TBranch        *b_Muid_nmuonms;   //!
  TBranch        *b_Muid_a0rms;   //!
  TBranch        *b_Muid_z0rms;   //!
  TBranch        *b_Muid_phirms;   //!
  TBranch        *b_Muid_thetarms;   //!
  TBranch        *b_Muid_etarms;   //!
  TBranch        *b_Muid_pirms;   //!
  TBranch        *b_Muid_chi2ms;   //!
  TBranch        *b_Muid_mdtms;   //!
  TBranch        *b_Muid_cscetams;   //!
  TBranch        *b_Muid_cscphims;   //!
  TBranch        *b_Muid_rpcetams;   //!
  TBranch        *b_Muid_rpcphims;   //!
  TBranch        *b_Muid_tgcetams;   //!
  TBranch        *b_Muid_tgcphims;   //!
  TBranch        *b_Muid_covr11ms;   //!
  TBranch        *b_Muid_covr12ms;   //!
  TBranch        *b_Muid_covr22ms;   //!
  TBranch        *b_Muid_covr13ms;   //!
  TBranch        *b_Muid_covr23ms;   //!
  TBranch        *b_Muid_covr33ms;   //!
  TBranch        *b_Muid_covr14ms;   //!
  TBranch        *b_Muid_covr24ms;   //!
  TBranch        *b_Muid_covr34ms;   //!
  TBranch        *b_Muid_covr44ms;   //!
  TBranch        *b_Muid_covr15ms;   //!
  TBranch        *b_Muid_covr25ms;   //!
  TBranch        *b_Muid_covr35ms;   //!
  TBranch        *b_Muid_covr45ms;   //!
  TBranch        *b_Muid_covr55ms;   //!
  TBranch        *b_Muid_nmuonmu;   //!
  TBranch        *b_Muid_prelossmu;   //!
  TBranch        *b_Muid_erelossmu;   //!
  TBranch        *b_Muid_isparelossmu;   //!
  TBranch        *b_Muid_a0rmu;   //!
  TBranch        *b_Muid_z0rmu;   //!
  TBranch        *b_Muid_phirmu;   //!
  TBranch        *b_Muid_thetarmu;   //!
  TBranch        *b_Muid_etarmu;   //!
  TBranch        *b_Muid_pirmu;   //!
  TBranch        *b_Muid_chi2mu;   //!
  TBranch        *b_Muid_covr11mu;   //!
  TBranch        *b_Muid_covr12mu;   //!
  TBranch        *b_Muid_covr22mu;   //!
  TBranch        *b_Muid_covr13mu;   //!
  TBranch        *b_Muid_covr23mu;   //!
  TBranch        *b_Muid_covr33mu;   //!
  TBranch        *b_Muid_covr14mu;   //!
  TBranch        *b_Muid_covr24mu;   //!
  TBranch        *b_Muid_covr34mu;   //!
  TBranch        *b_Muid_covr44mu;   //!
  TBranch        *b_Muid_covr15mu;   //!
  TBranch        *b_Muid_covr25mu;   //!
  TBranch        *b_Muid_covr35mu;   //!
  TBranch        *b_Muid_covr45mu;   //!
  TBranch        *b_Muid_covr55mu;   //!
  TBranch        *b_Muid_mslinkmu;   //!
  TBranch        *b_Muid_nmuoncb;   //!
  TBranch        *b_Muid_chi2mcb;   //!
  TBranch        *b_Muid_loosecb;   //!
  TBranch        *b_Muid_a0rcb;   //!
  TBranch        *b_Muid_z0rcb;   //!
  TBranch        *b_Muid_phircb;   //!
  TBranch        *b_Muid_thetarcb;   //!
  TBranch        *b_Muid_etarcb;   //!
  TBranch        *b_Muid_pircb;   //!
  TBranch        *b_Muid_chi2cb;   //!
  TBranch        *b_Muid_chi2prcb;   //!
  TBranch        *b_Muid_covr11cb;   //!
  TBranch        *b_Muid_covr12cb;   //!
  TBranch        *b_Muid_covr22cb;   //!
  TBranch        *b_Muid_covr13cb;   //!
  TBranch        *b_Muid_covr23cb;   //!
  TBranch        *b_Muid_covr33cb;   //!
  TBranch        *b_Muid_covr14cb;   //!
  TBranch        *b_Muid_covr24cb;   //!
  TBranch        *b_Muid_covr34cb;   //!
  TBranch        *b_Muid_covr44cb;   //!
  TBranch        *b_Muid_covr15cb;   //!
  TBranch        *b_Muid_covr25cb;   //!
  TBranch        *b_Muid_covr35cb;   //!
  TBranch        *b_Muid_covr45cb;   //!
  TBranch        *b_Muid_covr55cb;   //!
  TBranch        *b_Muid_mslinkcb;   //!
  TBranch        *b_Muid_exlinkcb;   //!
  TBranch        *b_Muid_blayercb;   //!
  TBranch        *b_Muid_pixelcb;   //!
  TBranch        *b_Muid_sctcb;   //!
  TBranch        *b_Muid_trtcb;   //!
  TBranch        *b_Muid_trthighcb;   //!
  TBranch        *b_Muid_a0rid;   //!
  TBranch        *b_Muid_z0rid;   //!
  TBranch        *b_Muid_phirid;   //!
  TBranch        *b_Muid_thetarid;   //!
  TBranch        *b_Muid_etarid;   //!
  TBranch        *b_Muid_pirid;   //!
  TBranch        *b_Muid_chi2id;   //!
  TBranch        *b_Muid_pulla0cb;   //!
  TBranch        *b_Muid_pullz0cb;   //!
  TBranch        *b_Muid_pullphicb;   //!
  TBranch        *b_Muid_pullthecb;   //!
  TBranch        *b_Muid_pullmomcb;   //!
  TBranch        *b_staco_nmuon;   //!
  TBranch        *b_staco_A0;   //!
  TBranch        *b_staco_Z;   //!
  TBranch        *b_staco_Phi;   //!
  TBranch        *b_staco_Theta;   //!
  TBranch        *b_staco_qOverP;   //!
  TBranch        *b_staco_chi2;   //!
  TBranch        *b_staco_covr11;   //!
  TBranch        *b_staco_covr21;   //!
  TBranch        *b_staco_covr22;   //!
  TBranch        *b_staco_covr31;   //!
  TBranch        *b_staco_covr32;   //!
  TBranch        *b_staco_covr33;   //!
  TBranch        *b_staco_covr41;   //!
  TBranch        *b_staco_covr42;   //!
  TBranch        *b_staco_covr43;   //!
  TBranch        *b_staco_covr44;   //!
  TBranch        *b_staco_covr51;   //!
  TBranch        *b_staco_covr52;   //!
  TBranch        *b_staco_covr53;   //!
  TBranch        *b_staco_covr54;   //!
  TBranch        *b_staco_covr55;   //!
  TBranch        *b_staco_A0MS;   //!
  TBranch        *b_staco_ZMS;   //!
  TBranch        *b_staco_PhiMS;   //!
  TBranch        *b_staco_ThetaMS;   //!
  TBranch        *b_staco_qOverPMS;   //!
  TBranch        *b_staco_A0ID;   //!
  TBranch        *b_staco_ZID;   //!
  TBranch        *b_staco_PhiID;   //!
  TBranch        *b_staco_ThetaID;   //!
  TBranch        *b_staco_qOverPID;   //!
  TBranch        *b_MuTag_nmuon;   //!
  TBranch        *b_MuTag_A0;   //!
  TBranch        *b_MuTag_Z;   //!
  TBranch        *b_MuTag_Phi;   //!
  TBranch        *b_MuTag_Theta;   //!
  TBranch        *b_MuTag_qOverP;   //!
  TBranch        *b_MuTag_covr11;   //!
  TBranch        *b_MuTag_covr21;   //!
  TBranch        *b_MuTag_covr22;   //!
  TBranch        *b_MuTag_covr31;   //!
  TBranch        *b_MuTag_covr32;   //!
  TBranch        *b_MuTag_covr33;   //!
  TBranch        *b_MuTag_covr41;   //!
  TBranch        *b_MuTag_covr42;   //!
  TBranch        *b_MuTag_covr43;   //!
  TBranch        *b_MuTag_covr44;   //!
  TBranch        *b_MuTag_covr51;   //!
  TBranch        *b_MuTag_covr52;   //!
  TBranch        *b_MuTag_covr53;   //!
  TBranch        *b_MuTag_covr54;   //!
  TBranch        *b_MuTag_covr55;   //!
  TBranch        *b_MET_ExMiss;   //!
  TBranch        *b_MET_EyMiss;   //!
  TBranch        *b_MET_EtSum;   //!
  TBranch        *b_MET_ExMissTopo;   //!
  TBranch        *b_MET_EyMissTopo;   //!
  TBranch        *b_MET_EtSumTopo;   //!
  TBranch        *b_MET_ExMissCorr;   //!
  TBranch        *b_MET_EyMissCorr;   //!
  TBranch        *b_MET_EtSumCorr;   //!
  TBranch        *b_MET_ExMissLocHadTopo;   //!
  TBranch        *b_MET_EyMissLocHadTopo;   //!
  TBranch        *b_MET_EtSumLocHadTopo;   //!
  TBranch        *b_MET_ExMissCorrTopo;   //!
  TBranch        *b_MET_EyMissCorrTopo;   //!
  TBranch        *b_MET_EtSumCorrTopo;   //!
  TBranch        *b_MET_ExMissMu;   //!
  TBranch        *b_MET_EyMissMu;   //!
  TBranch        *b_MET_EtSumMu;   //!
  TBranch        *b_MET_ExMissMuBoy;   //!
  TBranch        *b_MET_EyMissMuBoy;   //!
  TBranch        *b_MET_EtSumMuBoy;   //!
  TBranch        *b_MET_ExMissFinal;   //!
  TBranch        *b_MET_EyMissFinal;   //!
  TBranch        *b_MET_EtSumFinal;   //!
  TBranch        *b_MET_ExMissRefFinal;   //!
  TBranch        *b_MET_EyMissRefFinal;   //!
  TBranch        *b_MET_EtSumRefFinal;   //!
  TBranch        *b_MET_ExMissCryo;   //!
  TBranch        *b_MET_EyMissCryo;   //!
  TBranch        *b_MET_EtSumCryo;   //!
  TBranch        *b_MET_ExMissCryoCone;   //!
  TBranch        *b_MET_EyMissCryoCone;   //!
  TBranch        *b_MET_EtSumCryoCone;   //!
  TBranch        *b_MET_ExMissLocHadTopoObj;   //!
  TBranch        *b_MET_EyMissLocHadTopoObj;   //!
  TBranch        *b_MET_EtSumLocHadTopoObj;   //!
  TBranch        *b_MET_ExMissTopoObj;   //!
  TBranch        *b_MET_EyMissTopoObj;   //!
  TBranch        *b_MET_EtSumTopoObj;   //!
  TBranch        *b_MET_ExMissRefEle;   //!
  TBranch        *b_MET_EyMissRefEle;   //!
  TBranch        *b_MET_EtSumRefEle;   //!
  TBranch        *b_MET_ExMissRefMuo;   //!
  TBranch        *b_MET_EyMissRefMuo;   //!
  TBranch        *b_MET_EtSumRefMuo;   //!
  TBranch        *b_MET_ExMissRefJet;   //!
  TBranch        *b_MET_EyMissRefJet;   //!
  TBranch        *b_MET_EtSumRefJet;   //!
  TBranch        *b_MET_ExMissCellOut;   //!
  TBranch        *b_MET_EyMissCellOut;   //!
  TBranch        *b_MET_EtSumCellOut;   //!
  TBranch        *b_MET_ExFCALCorrTopo;   //!
  TBranch        *b_MET_EyFCALCorrTopo;   //!
  TBranch        *b_MET_EtSumFCALCorrTopo;   //!
  TBranch        *b_MET_ExFCALlocHadTopo;   //!
  TBranch        *b_MET_EyFCALlocHadTopo;   //!
  TBranch        *b_MET_EtSumFCALlocHadTopo;   //!
  TBranch        *b_MET_ExPEMB;   //!
  TBranch        *b_MET_EyPEMB;   //!
  TBranch        *b_MET_EtSumPEMB;   //!
  TBranch        *b_MET_nCellPEMB;   //!
  TBranch        *b_MET_ExEMB;   //!
  TBranch        *b_MET_EyEMB;   //!
  TBranch        *b_MET_EtSumEMB;   //!
  TBranch        *b_MET_nCellEMB;   //!
  TBranch        *b_MET_ExPEMEC;   //!
  TBranch        *b_MET_EyPEMEC;   //!
  TBranch        *b_MET_EtSumPEMEC;   //!
  TBranch        *b_MET_nCellPEMEC;   //!
  TBranch        *b_MET_ExEMEC;   //!
  TBranch        *b_MET_EyEMEC;   //!
  TBranch        *b_MET_EtSumEMEC;   //!
  TBranch        *b_MET_nCellEMEC;   //!
  TBranch        *b_MET_ExTILE;   //!
  TBranch        *b_MET_EyTILE;   //!
  TBranch        *b_MET_EtSumTILE;   //!
  TBranch        *b_MET_nCellTILE;   //!
  TBranch        *b_MET_ExHEC;   //!
  TBranch        *b_MET_EyHEC;   //!
  TBranch        *b_MET_EtSumHEC;   //!
  TBranch        *b_MET_nCellHEC;   //!
  TBranch        *b_MET_ExFCAL;   //!
  TBranch        *b_MET_EyFCAL;   //!
  TBranch        *b_MET_EtSumFCAL;   //!
  TBranch        *b_MET_nCellFCAL;   //!
  TBranch        *b_MET_ExBARReg;   //!
  TBranch        *b_MET_EyBARReg;   //!
  TBranch        *b_MET_EtSumBARReg;   //!
  TBranch        *b_MET_ExECReg;   //!
  TBranch        *b_MET_EyECReg;   //!
  TBranch        *b_MET_EtSumECReg;   //!
  TBranch        *b_MET_ExFCALReg;   //!
  TBranch        *b_MET_EyFCALReg;   //!
  TBranch        *b_MET_EtSumFCALReg;   //!
  TBranch        *b_MET_ExTruthInt;   //!
  TBranch        *b_MET_EyTruthInt;   //!
  TBranch        *b_MET_EtSumTruthInt;   //!
  TBranch        *b_MET_ExTruthNonInt;   //!
  TBranch        *b_MET_EyTruthNonInt;   //!
  TBranch        *b_MET_EtSumTruthNonInt;   //!
  TBranch        *b_MET_ExTruthIntCentral;   //!
  TBranch        *b_MET_EyTruthIntCentral;   //!
  TBranch        *b_MET_EtSumTruthIntCentral;   //!
  TBranch        *b_MET_ExTruthIntFwd;   //!
  TBranch        *b_MET_EyTruthIntFwd;   //!
  TBranch        *b_MET_EtSumTruthIntFwd;   //!
  TBranch        *b_MET_ExTruthMuons;   //!
  TBranch        *b_MET_EyTruthMuons;   //!
  TBranch        *b_MET_EtSumTruthMuons;   //!
  TBranch        *b_Obj_ExMissEle;   //!
  TBranch        *b_Obj_EyMissEle;   //!
  TBranch        *b_Obj_EtSumEle;   //!
  TBranch        *b_Obj_ExMissMu;   //!
  TBranch        *b_Obj_EyMissMu;   //!
  TBranch        *b_Obj_EtSumMu;   //!
  TBranch        *b_Obj_ExMissJet;   //!
  TBranch        *b_Obj_EyMissJet;   //!
  TBranch        *b_Obj_EtSumJet;   //!
  TBranch        *b_Obj_ExMissIdTrk;   //!
  TBranch        *b_Obj_EyMissIdTrk;   //!
  TBranch        *b_Obj_EtSumIdTrk;   //!
  TBranch        *b_Obj_ExMissMJet;   //!
  TBranch        *b_Obj_EyMissMJet;   //!
  TBranch        *b_Obj_EtSumMJet;   //!
  TBranch        *b_Obj_ExMissRest;   //!
  TBranch        *b_Obj_EyMissRest;   //!
  TBranch        *b_Obj_EtSumRest;   //!
  TBranch        *b_Obj_ExMissFinal;   //!
  TBranch        *b_Obj_EyMissFinal;   //!
  TBranch        *b_Obj_EtSumFinal;   //!
  TBranch        *b_L1EM_Scale;   //!
  TBranch        *b_L1Em_nRoI;   //!
  TBranch        *b_L1Em_RoIWord;   //!
  TBranch        *b_L1Em_Core;   //!
  TBranch        *b_L1Em_EmClus;   //!
  TBranch        *b_L1Em_TauClus;   //!
  TBranch        *b_L1Em_EmIsol;   //!
  TBranch        *b_L1Em_HdIsol;   //!
  TBranch        *b_L1Em_HdCore;   //!
  TBranch        *b_L1Em_EmTauThresh;   //!
  TBranch        *b_L1Em_eta;   //!
  TBranch        *b_L1Em_phi;   //!
  TBranch        *b_L1Jet_nRoI;   //!
  TBranch        *b_L1Jet_JetRoIWord;   //!
  TBranch        *b_L1Jet_ET4x4;   //!
  TBranch        *b_L1Jet_ET6x6;   //!
  TBranch        *b_L1Jet_ET8x8;   //!
  TBranch        *b_L1Jet_Thresh;   //!
  TBranch        *b_L1Jet_eta;   //!
  TBranch        *b_L1Jet_phi;   //!
  TBranch        *b_L1ET_EtMissHits;   //!
  TBranch        *b_L1ET_EtSumHits;   //!
  TBranch        *b_L1ET_Ex;   //!
  TBranch        *b_L1ET_Ey;   //!
  TBranch        *b_L1ET_EtMiss;   //!
  TBranch        *b_L1ET_EtSum;   //!
  TBranch        *b_L1ET_JetEtHits;   //!
  TBranch        *b_L1ET_JetEtSum;   //!
  TBranch        *b_canMulti;   //!
  TBranch        *b_nDataWord;   //!
  TBranch        *b_dataWord;   //!
  TBranch        *b_nCTP_ROI;   //!
  TBranch        *b_CTP_ROI;   //!
  TBranch        *b_nMuCTPI_ROI;   //!
  TBranch        *b_muCTPI_ROI;   //!
  TBranch        *b_nEMTau_ROI;   //!
  TBranch        *b_EMTau_ROI;   //!
  TBranch        *b_nJetEnergy_ROI;   //!
  TBranch        *b_JetEnergy_ROI;   //!
  TBranch        *b_T2CaNclus;   //!
  TBranch        *b_T2CaTauNclus;   //!
  TBranch        *b_T2CaEmE;   //!
  TBranch        *b_T2CaRawEmE;   //!
  TBranch        *b_T2CaEmES0;   //!
  TBranch        *b_T2CaEmES1;   //!
  TBranch        *b_T2CaEmES2;   //!
  TBranch        *b_T2CaEmES3;   //!
  TBranch        *b_T2CaHadE;   //!
  TBranch        *b_T2CaRawHadE;   //!
  TBranch        *b_T2CaHadES0;   //!
  TBranch        *b_T2CaHadES1;   //!
  TBranch        *b_T2CaHadES2;   //!
  TBranch        *b_T2CaHadES3;   //!
  TBranch        *b_T2CaRcore;   //!
  TBranch        *b_T2CaEratio;   //!
  TBranch        *b_T2CaWidth;   //!
  TBranch        *b_T2CaF73;   //!
  TBranch        *b_T2CaEta;   //!
  TBranch        *b_T2CaPhi;   //!
  TBranch        *b_T2CaRawEta;   //!
  TBranch        *b_T2CaRawPhi;   //!
  TBranch        *b_T2CaL1Sim_Eta;   //!
  TBranch        *b_T2CaL1Sim_Phi;   //!
  TBranch        *b_T2CaL1Sim_EmClus;   //!
  TBranch        *b_T2CaL1Sim_EmIsol;   //!
  TBranch        *b_T2CaL1Sim_HdCore;   //!
  TBranch        *b_T2CaL1Sim_HdIsol;   //!
  TBranch        *b_T2CaL1Sim_nRoIperRegion;   //!
  TBranch        *b_T2CaRoIword;   //!
  TBranch        *b_T2CaTauEta;   //!
  TBranch        *b_T2CaTauPhi;   //!
  TBranch        *b_T2CaEMES0_nar;   //!
  TBranch        *b_T2CaEMES0_wid;   //!
  TBranch        *b_T2CaEMES1_nar;   //!
  TBranch        *b_T2CaEMES1_wid;   //!
  TBranch        *b_T2CaEMES2_nar;   //!
  TBranch        *b_T2CaEMES2_wid;   //!
  TBranch        *b_T2CaEMES3_nar;   //!
  TBranch        *b_T2CaEMES3_wid;   //!
  TBranch        *b_T2CaHADES1_nar;   //!
  TBranch        *b_T2CaEHADS1_wid;   //!
  TBranch        *b_T2CaHADES2_nar;   //!
  TBranch        *b_T2CaEHADS2_wid;   //!
  TBranch        *b_T2CaHADES3_nar;   //!
  TBranch        *b_T2CaEHADS3_wid;   //!
  TBranch        *b_T2CaEMrad0;   //!
  TBranch        *b_T2CaEMrad1;   //!
  TBranch        *b_T2CaEMrad2;   //!
  TBranch        *b_T2CaEMrad3;   //!
  TBranch        *b_T2CaEMwid0;   //!
  TBranch        *b_T2CaEMwid1;   //!
  TBranch        *b_T2CaEMwid2;   //!
  TBranch        *b_T2CaEMwid3;   //!
  TBranch        *b_T2CaHADwid1;   //!
  TBranch        *b_T2CaHADwid2;   //!
  TBranch        *b_T2CaHADwid3;   //!
  TBranch        *b_T2CaEMenorm0;   //!
  TBranch        *b_T2CaEMenorm1;   //!
  TBranch        *b_T2CaEMenorm2;   //!
  TBranch        *b_T2CaEMenorm3;   //!
  TBranch        *b_T2CaHADenorm1;   //!
  TBranch        *b_T2CaHADenorm2;   //!
  TBranch        *b_T2CaHADenorm3;   //!
  TBranch        *b_T2CaNumStripCells;   //!
  TBranch        *b_T2CaEnergyCalib;   //!
  TBranch        *b_T2CaEMEnergyCalib;   //!
  TBranch        *b_T2CaIsoFrac;   //!
  TBranch        *b_TrigNClus;   //!
  TBranch        *b_TrigClusEta;   //!
  TBranch        *b_TrigClusPhi;   //!
  TBranch        *b_TrigClusEtaRef;   //!
  TBranch        *b_TrigClusPhiRef;   //!
  TBranch        *b_TrigClusEt;   //!
  TBranch        *b_TrigClusE;   //!
  TBranch        *b_TrigClusEtaPresh;   //!
  TBranch        *b_TrigClusPhiPresh;   //!
  TBranch        *b_TrigClusEPresh;   //!
  TBranch        *b_TrigClusEta1stS;   //!
  TBranch        *b_TrigClusPhi1stS;   //!
  TBranch        *b_TrigClusE1stS;   //!
  TBranch        *b_TrigClusEta2ndS;   //!
  TBranch        *b_TrigClusPhi2ndS;   //!
  TBranch        *b_TrigClusE2ndS;   //!
  TBranch        *b_TrigClusEta3rdS;   //!
  TBranch        *b_TrigClusPhi3rdS;   //!
  TBranch        *b_TrigClusE3rdS;   //!
  TBranch        *b_TrigClusMaker;   //!
  TBranch        *b_EFTauNCand;   //!
  TBranch        *b_EFTauEnergy;   //!
  TBranch        *b_EFTauNRoI;   //!
  TBranch        *b_EFTauEMenergy;   //!
  TBranch        *b_EFTauET;   //!
  TBranch        *b_EFTauEta;   //!
  TBranch        *b_EFTauPhi;   //!
  TBranch        *b_EFTauCaloEta;   //!
  TBranch        *b_EFTauCaloPhi;   //!
  TBranch        *b_EFTauNStrip;   //!
  TBranch        *b_EFTauNEMCell;   //!
  TBranch        *b_EFTauStripET;   //!
  TBranch        *b_EFTauStripWidth2;   //!
  TBranch        *b_EFTauEMRadius;   //!
  TBranch        *b_EFTauIsoFrac;   //!
  TBranch        *b_EFTauNtrack;   //!
  TBranch        *b_EFTauCharge;   //!
  TBranch        *b_EFTauPtTrack1;   //!
  TBranch        *b_EFTauEtaTrack1;   //!
  TBranch        *b_EFTauPhiTrack1;   //!
  TBranch        *b_EFTauDistTrack1;   //!
  TBranch        *b_EFTauPtTrack2;   //!
  TBranch        *b_EFTauEtaTrack2;   //!
  TBranch        *b_EFTauPhiTrack2;   //!
  TBranch        *b_EFTauDistTrack2;   //!
  TBranch        *b_EFTauPtTrack3;   //!
  TBranch        *b_EFTauEtaTrack3;   //!
  TBranch        *b_EFTauPhiTrack3;   //!
  TBranch        *b_EFTauDistTrack3;   //!
  TBranch        *b_EFTauSumEMe;   //!
  TBranch        *b_EFTauSumEMpx;   //!
  TBranch        *b_EFTauSumEMpy;   //!
  TBranch        *b_EFTauSumEMpz;   //!
  TBranch        *b_EFTauEtEMCalib;   //!
  TBranch        *b_EFTauEtHadCalib;   //!
  TBranch        *b_T2CaNjets;   //!
  TBranch        *b_T2CaJetE;   //!
  TBranch        *b_T2CaJeteta;   //!
  TBranch        *b_T2CaJetphi;   //!
  TBranch        *b_T2CaJetConeR;   //!
  TBranch        *b_T2CaJetroi;   //!
  TBranch        *b_TrigNJet;   //!
  TBranch        *b_TrigJetEt;   //!
  TBranch        *b_TrigJetEta;   //!
  TBranch        *b_TrigJetPhi;   //!
  TBranch        *b_TrigJetE;   //!
  TBranch        *b_TrigJetM;   //!
  TBranch        *b_TrigJetMaker;   //!
  TBranch        *b_T2IdNtracks;   //!
  TBranch        *b_T2IdRoiID;   //!
  TBranch        *b_T2IdAlgo;   //!
  TBranch        *b_T2IdPt;   //!
  TBranch        *b_T2IdPhi0;   //!
  TBranch        *b_T2IdZ0;   //!
  TBranch        *b_T2IdD0;   //!
  TBranch        *b_T2IdPhic;   //!
  TBranch        *b_T2IdEta;   //!
  TBranch        *b_T2IdErrPt;   //!
  TBranch        *b_T2IdErrPhi0;   //!
  TBranch        *b_T2IdErrEta;   //!
  TBranch        *b_T2IdErrD0;   //!
  TBranch        *b_T2IdErrZ0;   //!
  TBranch        *b_T2IdChi2;   //!
  TBranch        *b_T2IdNDoF;   //!
  TBranch        *b_T2IdNSihits;   //!
  TBranch        *b_T2IdNTrthits;   //!
  TBranch        *b_T2IdHPatt;   //!
  TBranch        *b_T2IdNstraw;   //!
  TBranch        *b_T2IdNtime;   //!
  TBranch        *b_T2IdNtr;   //!
  TBranch        *b_T2IdLastPlane;   //!
  TBranch        *b_T2IdFirstPlane;   //!
  TBranch        *b_T2IdNkineHit;   //!
  TBranch        *b_T2IdKineRef;   //!
  TBranch        *b_T2IdKineEnt;   //!
  TBranch        *b_T2IdNkineHitTRT;   //!
  TBranch        *b_T2IdKineRefTRT;   //!
  TBranch        *b_T2IdKineEntTRT;   //!
  TBranch        *b_T2NVtx;   //!
  TBranch        *b_T2zVertex;   //!
  TBranch        *b_TrkEF_totalNumTracks;   //!
  TBranch        *b_TrkEF_RoIId;   //!
  TBranch        *b_TrkEF_d0;   //!
  TBranch        *b_TrkEF_z0;   //!
  TBranch        *b_TrkEF_phi;   //!
  TBranch        *b_TrkEF_eta;   //!
  TBranch        *b_TrkEF_qOverP;   //!
  TBranch        *b_TrkEF_Chi2;   //!
  TBranch        *b_TrkEF_Ndf;   //!
  TBranch        *b_TrkEF_sigd0;   //!
  TBranch        *b_TrkEF_sigz0;   //!
  TBranch        *b_TrkEF_sigpt;   //!
  TBranch        *b_TrkEF_sigphi;   //!
  TBranch        *b_TrkEF_sigeta;   //!
  TBranch        *b_TrkEF_sigqOverP;   //!
  TBranch        *b_TrkEF_covVert21;   //!
  TBranch        *b_TrkEF_covVert31;   //!
  TBranch        *b_TrkEF_covVert32;   //!
  TBranch        *b_TrkEF_covVert41;   //!
  TBranch        *b_TrkEF_covVert42;   //!
  TBranch        *b_TrkEF_covVert43;   //!
  TBranch        *b_TrkEF_covVert51;   //!
  TBranch        *b_TrkEF_covVert52;   //!
  TBranch        *b_TrkEF_covVert53;   //!
  TBranch        *b_TrkEF_covVert54;   //!
  TBranch        *b_TrkEF_px;   //!
  TBranch        *b_TrkEF_py;   //!
  TBranch        *b_TrkEF_pz;   //!
  TBranch        *b_TrkEF_pt;   //!
  TBranch        *b_TrkEF_numberOfBLayerHits;   //!
  TBranch        *b_TrkEF_numberOfPixelHits;   //!
  TBranch        *b_TrkEF_numberOfSCTHits;   //!
  TBranch        *b_TrkEF_numberOfTRTHits;   //!
  TBranch        *b_TrkEF_numberOfTRTHighThresholdHits;   //!
  TBranch        *b_TrkEF_numberOfBLayerSharedHits;   //!
  TBranch        *b_TrkEF_numberOfPixelSharedHits;   //!
  TBranch        *b_TrkEF_numberOfPixelHoles;   //!
  TBranch        *b_TrkEF_numberOfSCTSharedHits;   //!
  TBranch        *b_TrkEF_numberOfSCTHoles;   //!
  TBranch        *b_TrkEF_numberOfTRTOutliers;   //!
  TBranch        *b_TrkEF_numberOfTRTHighThresholdOutliers;   //!
  TBranch        *b_TrkEF_numberOfMdtHits;   //!
  TBranch        *b_TrkEF_numberOfTgcPhiHits;   //!
  TBranch        *b_TrkEF_numberOfRpcPhiHits;   //!
  TBranch        *b_TrkEF_numberOfCscPhiHits;   //!
  TBranch        *b_TrkEF_numberOfTgcEtaHits;   //!
  TBranch        *b_TrkEF_numberOfRpcEtaHits;   //!
  TBranch        *b_TrkEF_numberOfCscEtaHits;   //!
  TBranch        *b_TrkEF_numberOfGangedPixels;   //!
  TBranch        *b_TrkEF_numberOfOutliersOnTrack;   //!
  TBranch        *b_TrkEF_numberOfTrackSummaryTypes;   //!
  TBranch        *b_TrkEF_truthBarcode;   //!
  TBranch        *b_TrkEF_truthNt;   //!
  TBranch        *b_TrkEF_ParticlePdg;   //!
  TBranch        *b_TrkEF_ParentPdg;   //!
  TBranch        *b_TrkEF_NumKineHits;   //!
  TBranch        *b_VxEF_vxp_numVertices;   //!
  TBranch        *b_VxEF_vxp_vtx_x;   //!
  TBranch        *b_VxEF_vxp_vtx_y;   //!
  TBranch        *b_VxEF_vxp_vtx_z;   //!
  TBranch        *b_VxEF_vxp_numOfTruthVertices;   //!
  TBranch        *b_VxEF_vxp_vtx_x_truth;   //!
  TBranch        *b_VxEF_vxp_vtx_y_truth;   //!
  TBranch        *b_VxEF_vxp_vtx_z_truth;   //!
  TBranch        *b_VxEF_vxp_vtx_x_res;   //!
  TBranch        *b_VxEF_vxp_vtx_y_res;   //!
  TBranch        *b_VxEF_vxp_vtx_z_res;   //!
  TBranch        *b_VxEF_vxp_sigvtx_x;   //!
  TBranch        *b_VxEF_vxp_sigvtx_y;   //!
  TBranch        *b_VxEF_vxp_sigvtx_z;   //!
  TBranch        *b_VxEF_vxp_vtxchi2;   //!
  TBranch        *b_VxEF_vxp_vtxndf;   //!
  TBranch        *b_VxEF_vxp_pt;   //!
  TBranch        *b_VxEF_vxp_vtxchi2prob;   //!
  TBranch        *b_VxEF_vxp_numTracksPerVertex;   //!
  TBranch        *b_VxEF_vxp_totalNumTracks;   //!
  TBranch        *b_VxEF_vxp_chi2;   //!
  TBranch        *b_VxEF_vxp_d0;   //!
  TBranch        *b_VxEF_vxp_sigd0;   //!
  TBranch        *b_VxEF_vxp_z0;   //!
  TBranch        *b_VxEF_vxp_sigz0;   //!
  TBranch        *b_VxEF_vxp_phi;   //!
  TBranch        *b_VxEF_vxp_sigphi;   //!
  TBranch        *b_VxEF_vxp_theta;   //!
  TBranch        *b_VxEF_vxp_sigtheta;   //!
  TBranch        *b_VxEF_vxp_qOverP;   //!
  TBranch        *b_VxEF_vxp_sigqOverP;   //!
  TBranch        *b_VxEF_vxp_d0g;   //!
  TBranch        *b_VxEF_vxp_sigd0g;   //!
  TBranch        *b_VxEF_vxp_z0g;   //!
  TBranch        *b_VxEF_vxp_sigz0g;   //!
  TBranch        *b_eg_nc_EF;   //!
  TBranch        *b_eg_e_EF;   //!
  TBranch        *b_eg_eta_EF;   //!
  TBranch        *b_eg_phi_EF;   //!
  TBranch        *b_eg_cl_et_EF;   //!
  TBranch        *b_eg_cl_eta_EF;   //!
  TBranch        *b_eg_cl_phi_EF;   //!
  TBranch        *b_eg_etap_EF;   //!
  TBranch        *b_eg_zvertex_EF;   //!
  TBranch        *b_eg_errz_EF;   //!
  TBranch        *b_eg_depth_EF;   //!
  TBranch        *b_eg_e0_EF;   //!
  TBranch        *b_eg_e1_EF;   //!
  TBranch        *b_eg_e2_EF;   //!
  TBranch        *b_eg_e3_EF;   //!
  TBranch        *b_eg_eta0_EF;   //!
  TBranch        *b_eg_eta1_EF;   //!
  TBranch        *b_eg_eta2_EF;   //!
  TBranch        *b_eg_eta3_EF;   //!
  TBranch        *b_eg_phi0_EF;   //!
  TBranch        *b_eg_phi1_EF;   //!
  TBranch        *b_eg_phi2_EF;   //!
  TBranch        *b_eg_phi3_EF;   //!
  TBranch        *b_eg_Etha1_EF;   //!
  TBranch        *b_eg_Etha_EF;   //!
  TBranch        *b_eg_Eha1_EF;   //!
  TBranch        *b_eg_F1_EF;   //!
  TBranch        *b_eg_F3_EF;   //!
  TBranch        *b_eg_E233_EF;   //!
  TBranch        *b_eg_E237_EF;   //!
  TBranch        *b_eg_E277_EF;   //!
  TBranch        *b_eg_Weta1_EF;   //!
  TBranch        *b_eg_Weta2_EF;   //!
  TBranch        *b_eg_E2ts1_EF;   //!
  TBranch        *b_eg_E2tsts1_EF;   //!
  TBranch        *b_eg_Widths1_EF;   //!
  TBranch        *b_eg_Widths2_EF;   //!
  TBranch        *b_eg_poscs1_EF;   //!
  TBranch        *b_eg_poscs2_EF;   //!
  TBranch        *b_eg_Barys1_EF;   //!
  TBranch        *b_eg_Wtots1_EF;   //!
  TBranch        *b_eg_Emins1_EF;   //!
  TBranch        *b_eg_Emaxs1_EF;   //!
  TBranch        *b_eg_Fracs1_EF;   //!
  TBranch        *b_eg_EtCone45_EF;   //!
  TBranch        *b_eg_EtCone20_EF;   //!
  TBranch        *b_eg_EtCone30_EF;   //!
  TBranch        *b_eg_EtCone40_EF;   //!
  TBranch        *b_eg_IsEM_EF;   //!
  TBranch        *b_eg_epiNN_EF;   //!
  TBranch        *b_eg_EMWeight_EF;   //!
  TBranch        *b_eg_PionWeight_EF;   //!
  TBranch        *b_eg_Hmatrix_EF;   //!
  TBranch        *b_eg_IsEMse_EF;   //!
  TBranch        *b_eg_epiNNse_EF;   //!
  TBranch        *b_eg_EMWeightse_EF;   //!
  TBranch        *b_eg_PionWeightse_EF;   //!
  TBranch        *b_eg_E011_EF;   //!
  TBranch        *b_eg_E033_EF;   //!
  TBranch        *b_eg_E131_EF;   //!
  TBranch        *b_eg_E1153_EF;   //!
  TBranch        *b_eg_E235_EF;   //!
  TBranch        *b_eg_E255_EF;   //!
  TBranch        *b_eg_E333_EF;   //!
  TBranch        *b_eg_E335_EF;   //!
  TBranch        *b_eg_E337_EF;   //!
  TBranch        *b_eg_E377_EF;   //!
  TBranch        *b_eg_trkmatchnt_EF;   //!
  TBranch        *b_eg_primary_EF;   //!
  TBranch        *b_eg_numtracks_EF;   //!
  TBranch        *b_eg_eoverp_EF;   //!
  TBranch        *b_eg_etacorr_EF;   //!
  TBranch        *b_eg_deta0_EF;   //!
  TBranch        *b_eg_dphi0_EF;   //!
  TBranch        *b_eg_deta1_EF;   //!
  TBranch        *b_eg_dphi1_EF;   //!
  TBranch        *b_eg_deta2_EF;   //!
  TBranch        *b_eg_dphi2_EF;   //!
  TBranch        *b_eg_deta3_EF;   //!
  TBranch        *b_eg_dphi3_EF;   //!
  TBranch        *b_eg_trkopt_EF;   //!
  TBranch        *b_eg_trkinvpTV_EF;   //!
  TBranch        *b_eg_trkcotThV_EF;   //!
  TBranch        *b_eg_trkphiV_EF;   //!
  TBranch        *b_egRoiId_EF;   //!
  TBranch        *b_egKey_EF;   //!
  TBranch        *b_T2Bphys_NPar;   //!
  TBranch        *b_T2Bphys_roiId;   //!
  TBranch        *b_T2Bphys_eta;   //!
  TBranch        *b_T2Bphys_phi;   //!
  TBranch        *b_T2Bphys_pType;   //!
  TBranch        *b_T2Bphys_mass;   //!
  TBranch        *b_T2Bphys_fitmass;   //!
  TBranch        *b_T2Bphys_fitchis;   //!
  TBranch        *b_T2Bphys_fitprob;   //!
  TBranch        *b_T2Bphys_secmass;   //!
  TBranch        *b_T2Bphys_secfitmass;   //!
  TBranch        *b_T2Bphys_secfitchis;   //!
  TBranch        *b_T2Bphys_secfitprob;   //!
  TBranch        *b_T2Bphys_pdgid_tr1;   //!
  TBranch        *b_T2Bphys_pdgid_tr2;   //!
  TBranch        *b_T2Bphys_pdgid_tr3;   //!
  TBranch        *b_T2Bphys_NtEnt1;   //!
  TBranch        *b_T2Bphys_NtEnt2;   //!
  TBranch        *b_T2Bphys_NtEnt3;   //!
  TBranch        *b_T2BjN;   //!
  TBranch        *b_T2BjEta;   //!
  TBranch        *b_T2BjPhi0;   //!
  TBranch        *b_T2BjX2d;   //!
  TBranch        *b_T2BjXd0;   //!
  TBranch        *b_T2BjXz0;   //!
  TBranch        *b_T2BjX3d;   //!
  TBranch        *b_T2BjXmvtx;   //!
  TBranch        *b_T2BjXevtx;   //!
  TBranch        *b_T2BjXnvtx;   //!
  TBranch        *b_T2BjPvtx;   //!
  TBranch        *b_EFBjN;   //!
  TBranch        *b_EFBjEta;   //!
  TBranch        *b_EFBjPhi0;   //!
  TBranch        *b_EFBjX2d;   //!
  TBranch        *b_EFBjXd0;   //!
  TBranch        *b_EFBjXz0;   //!
  TBranch        *b_EFBjX3d;   //!
  TBranch        *b_EFBjXmvtx;   //!
  TBranch        *b_EFBjXevtx;   //!
  TBranch        *b_EFBjXnvtx;   //!
  TBranch        *b_EFBjPvtx;   //!
  TBranch        *b_tau_ntaus;   //!
  TBranch        *b_tau_L2eta;   //!
  TBranch        *b_tau_L2phi;   //!
  TBranch        *b_tau_L2pt;   //!
  TBranch        *b_tau_L2Zvtx;   //!
  TBranch        *b_tau_L2nMatchTracks;   //!
  TBranch        *b_tau_L2roiId;   //!
  TBranch        *b_Trig_DecisionL1;   //!
  TBranch        *b_Trig_DecisionL2;   //!
  TBranch        *b_Trig_DecisionEF;   //!
  TBranch        *b_Trig_EM01;   //!
  TBranch        *b_Trig_L1_2EM15;   //!
  TBranch        *b_Trig_L1_2EM15I;   //!
  TBranch        *b_Trig_L1_EM25;   //!
  TBranch        *b_Trig_L1_EM25I;   //!
  TBranch        *b_Trig_L1_EM60;   //!
  TBranch        *b_Trig_L1_XE20;   //!
  TBranch        *b_Trig_L1_XE30;   //!
  TBranch        *b_Trig_L1_XE40;   //!
  TBranch        *b_Trig_L1_XE50;   //!
  TBranch        *b_Trig_L1_XE100;   //!
  TBranch        *b_Trig_L1_XE200;   //!
  TBranch        *b_Trig_L1_TAU05;   //!
  TBranch        *b_Trig_L1_TAU10;   //!
  TBranch        *b_Trig_L1_TAU10I;   //!
  TBranch        *b_Trig_L1_TAU15;   //!
  TBranch        *b_Trig_L1_TAU15I;   //!
  TBranch        *b_Trig_L1_TAU20I;   //!
  TBranch        *b_Trig_L1_TAU25I;   //!
  TBranch        *b_Trig_L1_TAU35I;   //!
  TBranch        *b_Trig_L1_MU06;   //!
  TBranch        *b_Trig_L1_2MU06;   //!
  TBranch        *b_Trig_L1_MU08;   //!
  TBranch        *b_Trig_L1_MU10;   //!
  TBranch        *b_Trig_L1_MU11;   //!
  TBranch        *b_Trig_L1_MU20;   //!
  TBranch        *b_Trig_L1_MU40;   //!
  TBranch        *b_Trig_L1_J35;   //!
  TBranch        *b_Trig_L1_J45;   //!
  TBranch        *b_Trig_L1_2J45;   //!
  TBranch        *b_Trig_L1_3J45;   //!
  TBranch        *b_Trig_L1_4J45;   //!
  TBranch        *b_Trig_L1_FJ30;   //!
  TBranch        *b_Trig_L1_J60;   //!
  TBranch        *b_Trig_L1_J80;   //!
  TBranch        *b_Trig_L1_J170;   //!
  TBranch        *b_Trig_L1_J300;   //!
  TBranch        *b_Trig_L1_BJT15;   //!
  TBranch        *b_Trig_L1_EM5;   //!
  TBranch        *b_Trig_L2_met10f;   //!
  TBranch        *b_Trig_L2_g10;   //!
  TBranch        *b_Trig_L2_g20iL2_g20i;   //!
  TBranch        *b_Trig_L2_g60;   //!
  TBranch        *b_Trig_L2_jet20a;   //!
  TBranch        *b_Trig_L2_jet20bL2_jet20b;   //!
  TBranch        *b_Trig_L2_jet20cL2_jet20cL2_jet20c;   //!
  TBranch        *b_Trig_L2_jet20dL2_jet20dL2_jet20dL2_jet20d;   //!
  TBranch        *b_Trig_L2_jet20kt;   //!
  TBranch        *b_Trig_L2_jet160;   //!
  TBranch        *b_Trig_L2_jet120L2_jet120;   //!
  TBranch        *b_Trig_L2_jet65L2_jet65L2_jet65;   //!
  TBranch        *b_Trig_L2_jet50L2_jet50L2_jet50L2_jet50;   //!
  TBranch        *b_Trig_L2_frjet10;   //!
  TBranch        *b_Trig_L2_fljet10;   //!
  TBranch        *b_Trig_L2_BsDsPhiPi;   //!
  TBranch        *b_Trig_L2_BsDsPhiPiFullSCan;   //!
  TBranch        *b_Trig_L2_BJpsiMU6mu;   //!
  TBranch        *b_Trig_L2_b35;   //!
  TBranch        *b_Trig_L2_e10TRTxK;   //!
  TBranch        *b_Trig_L2_e15iL2_e15i;   //!
  TBranch        *b_Trig_L2_e25i;   //!
  TBranch        *b_Trig_L2_e60;   //!
  TBranch        *b_Trig_L2_e10;   //!
  TBranch        *b_Trig_L2_e10L2_e10;   //!
  TBranch        *b_Trig_L2_mu6l;   //!
  TBranch        *b_Trig_L2_mu20i;   //!
  TBranch        *b_Trig_L2_mu6lL2_mu6l;   //!
  TBranch        *b_Trig_L2_mu6L2_mu6;   //!
  TBranch        *b_Trig_L2_mu6;   //!
  TBranch        *b_Trig_L2_Ze10e10;   //!
  TBranch        *b_Trig_L2_tau10;   //!
  TBranch        *b_Trig_L2_tau10i;   //!
  TBranch        *b_Trig_L2_tau15;   //!
  TBranch        *b_Trig_L2_tau15i;   //!
  TBranch        *b_Trig_L2_tau20i;   //!
  TBranch        *b_Trig_L2_tau25i;   //!
  TBranch        *b_Trig_L2_tau35i;   //!
  TBranch        *b_Trig_L2_tauNoCut;   //!
  TBranch        *b_Trig_L2_BJpsimu6lmu6l;   //!
  TBranch        *b_Trig_L2_BJpsimu6mu6;   //!
  TBranch        *b_Trig_L2_BJpsimu6mu;   //!
  TBranch        *b_Trig_EF_met10;   //!
  TBranch        *b_Trig_EF_jet20aEt;   //!
  TBranch        *b_Trig_EF_jet20bEtEF_jet20bEt;   //!
  TBranch        *b_Trig_EF_jet20cEtEF_jet20cEtEF_jet20cEt;   //!
  TBranch        *b_Trig_EF_jet20dEtEF_jet20dEtEF_jet20dEtEF_jet20dEt;   //!
  TBranch        *b_Trig_EF_jet20kt;   //!
  TBranch        *b_Trig_EF_jet160;   //!
  TBranch        *b_Trig_EF_jet120EF_jet120;   //!
  TBranch        *b_Trig_EF_jet65EF_jet65EF_jet65;   //!
  TBranch        *b_Trig_EF_jet50EF_jet50EF_jet50EF_jet50;   //!
  TBranch        *b_Trig_EF_frjet10;   //!
  TBranch        *b_Trig_EF_fljet10;   //!
  TBranch        *b_Trig_EF_MuonTRTExt_mu6l;   //!
  TBranch        *b_Trig_EF_BsDsPhiPi;   //!
  TBranch        *b_Trig_EF_b35;   //!
  TBranch        *b_Trig_EF_tau10;   //!
  TBranch        *b_Trig_EF_tau10i;   //!
  TBranch        *b_Trig_EF_tau15;   //!
  TBranch        *b_Trig_EF_tau15i;   //!
  TBranch        *b_Trig_EF_tau20i;   //!
  TBranch        *b_Trig_EF_tau25i;   //!
  TBranch        *b_Trig_EF_tau35i;   //!
  TBranch        *b_Trig_EF_tauNoCut;   //!
  TBranch        *b_Trig_EF_mu6l;   //!
  TBranch        *b_Trig_EF_mu6;   //!
  TBranch        *b_Trig_EF_mu20i;   //!
  TBranch        *b_Trig_EF_g10;   //!
  TBranch        *b_Trig_EF_g20iEF_g20i;   //!
  TBranch        *b_Trig_EF_g60;   //!
  TBranch        *b_Trig_EF_e10;   //!
  TBranch        *b_Trig_EF_e10TRTxK;   //!
  TBranch        *b_Trig_EF_e15iEF_e15i;   //!
  TBranch        *b_Trig_EF_e25i;   //!
  TBranch        *b_Trig_EF_e60;   //!
  TBranch        *b_nMC;   //!
  TBranch        *b_mcPx;   //!
  TBranch        *b_mcPy;   //!
  TBranch        *b_mcPz;   //!
  TBranch        *b_mcEne;   //!
  TBranch        *b_mcEtIsol;   //!
  TBranch        *b_mcID;   //!
  TBranch        *b_mcStatus;   //!
  TBranch        *b_mcBarcode;   //!
  TBranch        *b_mcCharge;   //!
  TBranch        *b_mcNMother;   //!
  TBranch        *b_mcMotherNt;   //!
  TBranch        *b_mcNChild;   //!
  TBranch        *b_mcChildNt;   //!
  TBranch        *b_EleN;   //!
  TBranch        *b_EleAuthor;   //!
  TBranch        *b_EleTrkNt;   //!
  TBranch        *b_EleEta;   //!
  TBranch        *b_ElePhi;   //!
  TBranch        *b_ElePt;   //!
  TBranch        *b_EleIsEM;   //!
  TBranch        *b_EleZ0;   //!
  TBranch        *b_EleD0;   //!
  TBranch        *b_EleNBHits;   //!
  TBranch        *b_EleNPixelHits;   //!
  TBranch        *b_EleNSCTHits;   //!
  TBranch        *b_EleNTRTHits;   //!
  TBranch        *b_EleNTRHits;   //!
  TBranch        *b_EleEOverP;   //!
  TBranch        *b_EleEtaBE2;   //!
  TBranch        *b_EleEt37;   //!
  TBranch        *b_EleE237;   //!
  TBranch        *b_EleE277;   //!
  TBranch        *b_EleEthad1;   //!
  TBranch        *b_EleWeta1;   //!
  TBranch        *b_EleWeta2;   //!
  TBranch        *b_EleF1;   //!
  TBranch        *b_EleE2tsts1;   //!
  TBranch        *b_EleEmins1;   //!
  TBranch        *b_EleWtots1;   //!
  TBranch        *b_EleFracs1;   //!
  TBranch        *b_EleSoftEtaCorrMag;   //!
  TBranch        *b_EleSoftF1Core;   //!
  TBranch        *b_EleSoftF3Core;   //!
  TBranch        *b_EleSoftPos7;   //!
  TBranch        *b_EleSoftIso;   //!
  TBranch        *b_EleSoftWidths2;   //!
  TBranch        *b_PhoN;   //!
  TBranch        *b_PhoAuthor;   //!
  TBranch        *b_PhoEta;   //!
  TBranch        *b_PhoPhi;   //!
  TBranch        *b_PhoPt;   //!
  TBranch        *b_PhoIsEM;   //!
  TBranch        *b_PhoEtaBE2;   //!
  TBranch        *b_PhoEt37;   //!
  TBranch        *b_PhoE237;   //!
  TBranch        *b_PhoE277;   //!
  TBranch        *b_PhoEthad1;   //!
  TBranch        *b_PhoWeta1;   //!
  TBranch        *b_PhoWeta2;   //!
  TBranch        *b_PhoF1;   //!
  TBranch        *b_PhoE2tsts1;   //!
  TBranch        *b_PhoEmins1;   //!
  TBranch        *b_PhoWtots1;   //!
  TBranch        *b_PhoFracs1;   //!
  TBranch        *b_MuonN;   //!
  TBranch        *b_MuonAuthor;   //!
  TBranch        *b_MuonCombTrkNt;   //!
  TBranch        *b_MuonInDetTrkNt;   //!
  TBranch        *b_MuonExtrNt;   //!
  TBranch        *b_MuonEta;   //!
  TBranch        *b_MuonPhi;   //!
  TBranch        *b_MuonPt;   //!
  TBranch        *b_MuonZ0;   //!
  TBranch        *b_MuonD0;   //!
  TBranch        *b_MuonCharge;   //!
  TBranch        *b_MuonNMDTHits;   //!
  TBranch        *b_MuonNRPCEtaHits;   //!
  TBranch        *b_MuonNRPCPhiHits;   //!
  TBranch        *b_MuonNCSCEtaHits;   //!
  TBranch        *b_MuonNCSCPhiHits;   //!
  TBranch        *b_MuonNTGCEtaHits;   //!
  TBranch        *b_MuonNTGCPhiHits;   //!
  TBranch        *b_MuonLowPtNMDTHits;   //!
  TBranch        *b_MuonLowPtNCSCEtaHits;   //!
  TBranch        *b_MuonLowPtNCSCPhiHits;   //!
  TBranch        *b_MuonLowPtNRPCEtaHits;   //!
  TBranch        *b_MuonLowPtNRPCPhiHits;   //!
  TBranch        *b_MuonLowPtNTGCEtaHits;   //!
  TBranch        *b_MuonLowPtNTGCPhiHits;   //!
  TBranch        *b_TauJet_N;   //!
  TBranch        *b_TauJet_Author;   //!
  TBranch        *b_TauJet_numTrack;   //!
  TBranch        *b_TauJet_decayVxNt;   //!
  TBranch        *b_TauJet_clusterNt;   //!
  TBranch        *b_TauJet_eta;   //!
  TBranch        *b_TauJet_phi;   //!
  TBranch        *b_TauJet_pt;   //!
  TBranch        *b_TauJet_m;   //!
  TBranch        *b_TauJet_et;   //!
  TBranch        *b_TauJet_charge;   //!
  TBranch        *b_TauJet_emRadius;   //!
  TBranch        *b_TauJet_stripWidth2;   //!
  TBranch        *b_TauJet_nStripCells;   //!
  TBranch        *b_TauJet_NumTauRecParams;   //!
  TBranch        *b_TauJet_pEM_px;   //!
  TBranch        *b_TauJet_pEM_py;   //!
  TBranch        *b_TauJet_pEM_pz;   //!
  TBranch        *b_TauJet_pEM_e;   //!
  TBranch        *b_TauJet_etHadCalib;   //!
  TBranch        *b_TauJet_etEMCalib;   //!
  TBranch        *b_TauJet_isolationFraction;   //!
  TBranch        *b_TauJet_centralityFraction;   //!
  TBranch        *b_TauJet_logLikelihoodRatio;   //!
  TBranch        *b_TauJet_lowPtTauJetDiscriminant;   //!
  TBranch        *b_TauJet_lowPtTauEleDiscriminant;   //!
  TBranch        *b_TauJet_tauJetNeuralnetwork;   //!
  TBranch        *b_TauJet_tauENeuralNetwork;   //!
  TBranch        *b_TauJet_TrkNts;   //!
  TBranch        *b_TauJet1p3p_N;   //!
  TBranch        *b_TauJet1p3p_Author;   //!
  TBranch        *b_TauJet1p3p_numTrack;   //!
  TBranch        *b_TauJet1p3p_decayVxNt;   //!
  TBranch        *b_TauJet1p3p_clusterNt;   //!
  TBranch        *b_TauJet1p3p_eta;   //!
  TBranch        *b_TauJet1p3p_phi;   //!
  TBranch        *b_TauJet1p3p_pt;   //!
  TBranch        *b_TauJet1p3p_m;   //!
  TBranch        *b_TauJet1p3p_et;   //!
  TBranch        *b_TauJet1p3p_charge;   //!
  TBranch        *b_TauJet1p3p_emRadius;   //!
  TBranch        *b_TauJet1p3p_stripWidth2;   //!
  TBranch        *b_TauJet1p3p_nStripCells;   //!
  TBranch        *b_TauJet1p3p_NumTau1p3pParams;   //!
  TBranch        *b_TauJet1p3p_annularIsolationFraction;   //!
  TBranch        *b_TauJet1p3p_etCaloAtEMScale;   //!
  TBranch        *b_TauJet1p3p_etChargedHadCells;   //!
  TBranch        *b_TauJet1p3p_etOtherEMCells;   //!
  TBranch        *b_TauJet1p3p_etOtherHadCells;   //!
  TBranch        *b_TauJet1p3p_discriminant;   //!
  TBranch        *b_TauJet1p3p_discPDERS;   //!
  TBranch        *b_TauJet1p3p_discNN;   //!
  TBranch        *b_TauJet1p3p_nAssocTracksCore;   //!
  TBranch        *b_TauJet1p3p_nAssocTracksIsol;   //!
  TBranch        *b_TauJet1p3p_sumPTtrack;   //!
  TBranch        *b_TauJet1p3p_rWidth2Trk3P;   //!
  TBranch        *b_TauJet1p3p_massTrk3P;   //!
  TBranch        *b_TauJet1p3p_TrkNts;   //!
  TBranch        *b_CTPWord0;   //!
  TBranch        *b_CTPWord1;   //!
  TBranch        *b_CTPWord2;   //!
  TBranch        *b_MuROI_N;   //!
  TBranch        *b_MuROI_Eta;   //!
  TBranch        *b_MuROI_Phi;   //!
  TBranch        *b_MuROI_Thr;   //!
  TBranch        *b_EmROI_N;   //!
  TBranch        *b_EmROI_Eta;   //!
  TBranch        *b_EmROI_Phi;   //!
  TBranch        *b_EmROI_Thr;   //!
  TBranch        *b_JetROI_N;   //!
  TBranch        *b_JetROI_Eta;   //!
  TBranch        *b_JetROI_Phi;   //!
  TBranch        *b_JetROI_Thr;   //!
  TBranch        *b_JetEtROI_N;   //!
  TBranch        *b_JetEtROI_Word;   //!
  TBranch        *b_JetEtROI_Thr;   //!
  TBranch        *b_ESumROI_N;   //!
  TBranch        *b_ESumROI_Ex;   //!
  TBranch        *b_ESumROI_Ey;   //!
  TBranch        *b_ESumROI_Et;   //!
  TBranch        *b_ESumROI_ThrS;   //!
  TBranch        *b_ESumROI_ThrM;   //!
  TBranch        *b_cl_ecluster_topo;   //!
  TBranch        *b_cl_nctotal_topo;   //!
  TBranch        *b_cl_nc_topo;   //!
  TBranch        *b_cl_et_topo;   //!
  TBranch        *b_cl_e_topo;   //!
  TBranch        *b_cl_eemb0_topo;   //!
  TBranch        *b_cl_eemb1_topo;   //!
  TBranch        *b_cl_eemb2_topo;   //!
  TBranch        *b_cl_eemb3_topo;   //!
  TBranch        *b_cl_nemb0_topo;   //!
  TBranch        *b_cl_nemb1_topo;   //!
  TBranch        *b_cl_nemb2_topo;   //!
  TBranch        *b_cl_nemb3_topo;   //!
  TBranch        *b_cl_phi2_topo;   //!
  TBranch        *b_cl_eta0_topo;   //!
  TBranch        *b_cl_eta1_topo;   //!
  TBranch        *b_cl_eta2_topo;   //!
  TBranch        *b_cl_eta3_topo;   //!
  TBranch        *b_cl_eeme0_topo;   //!
  TBranch        *b_cl_eeme1_topo;   //!
  TBranch        *b_cl_eeme2_topo;   //!
  TBranch        *b_cl_eeme3_topo;   //!
  TBranch        *b_cl_neme0_topo;   //!
  TBranch        *b_cl_neme1_topo;   //!
  TBranch        *b_cl_neme2_topo;   //!
  TBranch        *b_cl_neme3_topo;   //!
  TBranch        *b_cl_etileg1_topo;   //!
  TBranch        *b_cl_etileg2_topo;   //!
  TBranch        *b_cl_etileg3_topo;   //!
  TBranch        *b_cl_ntileg1_topo;   //!
  TBranch        *b_cl_ntileg2_topo;   //!
  TBranch        *b_cl_ntileg3_topo;   //!
  TBranch        *b_cl_eta_topo;   //!
  TBranch        *b_cl_phi_topo;   //!
  TBranch        *b_cl_time_topo;   //!
  TBranch        *b_cl_reco_stat_topo;   //!
  TBranch        *b_cl_m1_eta_topo;   //!
  TBranch        *b_cl_m1_phi_topo;   //!
  TBranch        *b_cl_m2_r_topo;   //!
  TBranch        *b_cl_m2_lambda_topo;   //!
  TBranch        *b_cl_delta_phi_topo;   //!
  TBranch        *b_cl_delta_theta_topo;   //!
  TBranch        *b_cl_delta_alpha_topo;   //!
  TBranch        *b_cl_center_x_topo;   //!
  TBranch        *b_cl_center_y_topo;   //!
  TBranch        *b_cl_center_z_topo;   //!
  TBranch        *b_cl_center_lambda_topo;   //!
  TBranch        *b_cl_lateral_topo;   //!
  TBranch        *b_cl_longitudinal_topo;   //!
  TBranch        *b_cl_eng_frac_em_topo;   //!
  TBranch        *b_cl_eng_frac_max_topo;   //!
  TBranch        *b_cl_eng_frac_core_topo;   //!
  TBranch        *b_cl_m1_dens_topo;   //!
  TBranch        *b_cl_m2_dens_topo;   //!
  TBranch        *b_cl_ehec0_topo;   //!
  TBranch        *b_cl_ehec1_topo;   //!
  TBranch        *b_cl_ehec2_topo;   //!
  TBranch        *b_cl_ehec3_topo;   //!
  TBranch        *b_cl_nhec0_topo;   //!
  TBranch        *b_cl_nhec1_topo;   //!
  TBranch        *b_cl_nhec2_topo;   //!
  TBranch        *b_cl_nhec3_topo;   //!
  TBranch        *b_cl_etileb0_topo;   //!
  TBranch        *b_cl_etileb1_topo;   //!
  TBranch        *b_cl_etileb2_topo;   //!
  TBranch        *b_cl_ntileb0_topo;   //!
  TBranch        *b_cl_ntileb1_topo;   //!
  TBranch        *b_cl_ntileb2_topo;   //!
  TBranch        *b_cl_etilee0_topo;   //!
  TBranch        *b_cl_etilee1_topo;   //!
  TBranch        *b_cl_etilee2_topo;   //!
  TBranch        *b_cl_efcal0_topo;   //!
  TBranch        *b_cl_efcal1_topo;   //!
  TBranch        *b_cl_efcal2_topo;   //!
  TBranch        *b_cl_ntilee0_topo;   //!
  TBranch        *b_cl_ntilee1_topo;   //!
  TBranch        *b_cl_ntilee2_topo;   //!
  TBranch        *b_cl_nfcal0_topo;   //!
  TBranch        *b_cl_nfcal1_topo;   //!
  TBranch        *b_cl_nfcal2_topo;   //!
  TBranch        *b_cl_ntotcells_topo;   //!
  TBranch        *b_nPJ;   //!
  TBranch        *b_pjAuthor;   //!
  TBranch        *b_pjPx;   //!
  TBranch        *b_pjPy;   //!
  TBranch        *b_pjPz;   //!
  TBranch        *b_pjEne;   //!
  TBranch        *b_pjEta;   //!
  TBranch        *b_pjPhi;   //!
  TBranch        *b_pjPt;   //!
  TBranch        *b_pjNtrk;   //!
  TBranch        *b_pjNmuo;   //!
  TBranch        *b_pjNele;   //!
  TBranch        *b_pjWeight;   //!
  TBranch        *b_pjW2D;   //!
  TBranch        *b_pjW3D;   //!
  TBranch        *b_pjWSV2;   //!
  TBranch        *b_pjWlhSig;   //!
  TBranch        *b_pjLabel;   //!
  TBranch        *b_pjdRb;   //!
  TBranch        *b_pjdRc;   //!
  TBranch        *b_pjdRt;   //!
  TBranch        *b_MemRSS;   //!
  TBranch        *b_MemVSize;   //!
  TBranch        *b_TimeTotal;   //!
  TBranch        *b_TimeSum;   //!
  TBranch        *b_TimeEventCounter;   //!
  TBranch        *b_TimePixelRegionSelectorTable;   //!
  TBranch        *b_TimeSCT_RegionSelectorTable;   //!
  TBranch        *b_TimeTRT_RegionSelectorTable;   //!
  TBranch        *b_TimePixelClusterization;   //!
  TBranch        *b_TimeSCT_Clusterization;   //!
  TBranch        *b_TimeTRT_RIO_Maker;   //!
  TBranch        *b_TimePRD_MultiTruthMaker;   //!
  TBranch        *b_TimeSiTrackerSpacePointFinder;   //!
  TBranch        *b_TimeiPatRec;   //!
  TBranch        *b_TimeiPatTrackTruthAssociator;   //!
  TBranch        *b_TimeiPatStatistics;   //!
  TBranch        *b_TimeXKaSeedsManager;   //!
  TBranch        *b_TimeXKaMField;   //!
  TBranch        *b_TimeXKaDetectorBuilder;   //!
  TBranch        *b_TimeXKalMan;   //!
  TBranch        *b_TimeInDetLegacyCnvAlg;   //!
  TBranch        *b_TimeConvertedXKalmanTracksDetailedTruthMaker;   //!
  TBranch        *b_TimeConvertedXKalmanTracksTruthSelector;   //!
  TBranch        *b_TimeConvertedIPatTracksDetailedTruthMaker;   //!
  TBranch        *b_TimeConvertedIPatTracksTruthSelector;   //!
  TBranch        *b_TimeInDetSiSPTrackFinder;   //!
  TBranch        *b_TimeSiSPSeededTracksDetailedTruthMaker;   //!
  TBranch        *b_TimeSiSPSeededTracksTruthCollectionSelector;   //!
  TBranch        *b_TimeInDetAmbiguitySolver;   //!
  TBranch        *b_TimeResolvedTracksDetailedTruthMaker;   //!
  TBranch        *b_TimeResolvedTracksTruthCollectionSelector;   //!
  TBranch        *b_TimeInDetTRTExtension;   //!
  TBranch        *b_TimeInDetExtensionProcessor;   //!
  TBranch        *b_TimeExtendedTracksDetailedTruthMaker;   //!
  TBranch        *b_TimeExtendedTracksTruthCollectionSelector;   //!
  TBranch        *b_TimeInDetTRTTrackSegmentsFinder;   //!
  TBranch        *b_TimeInDetOutputCopyAlg;   //!
  TBranch        *b_TimeDetailedTracksTruthMaker;   //!
  TBranch        *b_TimeTrackTruthCollectionSelector;   //!
  TBranch        *b_TimeInDetPriVxFinder;   //!
  TBranch        *b_TimeInDetIPatPriVxFinder;   //!
  TBranch        *b_TimeInDetXKalPriVxFinder;   //!
  TBranch        *b_TimeInDetParticleCreation;   //!
  TBranch        *b_TimeInDetIPatParticleCreation;   //!
  TBranch        *b_TimeInDetXKalParticleCreation;   //!
  TBranch        *b_TimeInDetTrackParticleTruthMaker;   //!
  TBranch        *b_TimeInDetIPatTrackParticleTruthMaker;   //!
  TBranch        *b_TimeInDetXKalTrackParticleTruthMaker;   //!
  TBranch        *b_TimeInDetRecStatistics;   //!
  TBranch        *b_TimeCaloCellMaker;   //!
  TBranch        *b_TimeCmbTowerBldr;   //!
  TBranch        *b_TimeCaloSWClusterMaker;   //!
  TBranch        *b_TimeTowerMaker;   //!
  TBranch        *b_TimeLArSWClusterMaker;   //!
  TBranch        *b_TimeLArgamClusterMaker;   //!
  TBranch        *b_TimeLAr35ClusterMaker;   //!
  TBranch        *b_TimeLAr37ClusterMaker;   //!
  TBranch        *b_TimeLArgam35ClusterMaker;   //!
  TBranch        *b_TimeLArgam37ClusterMaker;   //!
  TBranch        *b_TimeCaloTopoClusterMaker;   //!
  TBranch        *b_TimeCaloCell2TopoClusterMapper;   //!
  TBranch        *b_TimeEMTopoClusterMaker;   //!
  TBranch        *b_TimeEMCell2TopoClusterMapper;   //!
  TBranch        *b_TimeMuonRdoToMuonDigit;   //!
  TBranch        *b_TimeCscRdoToCscPrepData;   //!
  TBranch        *b_TimeMdtRdoToMdtPrepData;   //!
  TBranch        *b_TimeRpcRdoToRpcPrepData;   //!
  TBranch        *b_TimeTgcRdoToTgcPrepData;   //!
  TBranch        *b_TimeCscThresholdClusterBuilder;   //!
  TBranch        *b_TimeTrackRecordFilter;   //!
  TBranch        *b_TimeMboyDigiEmptyLoop;   //!
  TBranch        *b_TimeMboyDigiEmptyLoop2;   //!
  TBranch        *b_TimeMboyRec;   //!
  TBranch        *b_TimeMooMakePhiPatterns;   //!
  TBranch        *b_TimeMooMakeRzPatterns;   //!
  TBranch        *b_TimeMooCombinePatterns;   //!
  TBranch        *b_TimeMooCalibratedSegmentMaker;   //!
  TBranch        *b_TimePhiPatternToPhiSeg;   //!
  TBranch        *b_TimeCscPatternToCrudeSeg;   //!
  TBranch        *b_TimeMdtPatternToCrudeSeg;   //!
  TBranch        *b_TimeMuonSegmentComboToMooRzSegmentCombo;   //!
  TBranch        *b_TimeMuonSegmentToMooRzSegment;   //!
  TBranch        *b_TimeMooRoadMaker;   //!
  TBranch        *b_TimeMooMakeTracks;   //!
  TBranch        *b_TimeMooCnvAlg;   //!
  TBranch        *b_TimeMuidMooreTPCreator;   //!
  TBranch        *b_TimeConversionFinder;   //!
  TBranch        *b_Timeegamma;   //!
  TBranch        *b_Timesofte;   //!
  TBranch        *b_TimeConeTowerJets;   //!
  TBranch        *b_TimeCone4TowerJets;   //!
  TBranch        *b_TimeKtTowerJets;   //!
  TBranch        *b_TimeKt4TowerJets;   //!
  TBranch        *b_TimeConeTopoJetAlg;   //!
  TBranch        *b_TimeCone4TopoJetAlg;   //!
  TBranch        *b_TimeKt6TopoJets;   //!
  TBranch        *b_TimeKt4TopoJets;   //!
  TBranch        *b_TimeConeTruthJets;   //!
  TBranch        *b_TimeCone4TruthJets;   //!
  TBranch        *b_TimeKt6TruthJets;   //!
  TBranch        *b_TimeKt4TruthJets;   //!
  TBranch        *b_TimetauSequence;   //!
  TBranch        *b_Timetau1p3pSequence;   //!
  TBranch        *b_TimeTileLookForMuAlg;   //!
  TBranch        *b_TimeMuidStandAlone;   //!
  TBranch        *b_TimeMuidExtrCnvAlg;   //!
  TBranch        *b_TimeMuidCombined;   //!
  TBranch        *b_TimeMuidCnvAlg;   //!
  TBranch        *b_TimeMuGirl;   //!
  TBranch        *b_TimeMuGirlToCombinedMuon;   //!
  TBranch        *b_TimeStacoMaster;   //!
  TBranch        *b_TimeMuTagMaster;   //!
  TBranch        *b_TimeMuonPrepRawDataTruthMaker;   //!
  TBranch        *b_TimeMuonTrackTruthMaker;   //!
  TBranch        *b_TimeMETRefAlg;   //!
  TBranch        *b_TimeMETAlg;   //!
  TBranch        *b_TimeLeptonAlg;   //!
  TBranch        *b_TimeHadJets;   //!
  TBranch        *b_TimeHadronAlg;   //!
  TBranch        *b_TimeTriggerTowerMaker;   //!
  TBranch        *b_TimeJetElementMaker;   //!
  TBranch        *b_TimeEmTauTrigger;   //!
  TBranch        *b_TimeJetTrigger;   //!
  TBranch        *b_TimeEnergyTrigger;   //!
  TBranch        *b_TimeROD;   //!
  TBranch        *b_TimeTrigT1RPC;   //!
  TBranch        *b_TimeLVL1TGCTrigger;   //!
  TBranch        *b_TimeL1Muctpi;   //!
  TBranch        *b_TimeCTPSim;   //!
  TBranch        *b_TimeRoIBuilder;   //!
  TBranch        *b_TimeZVertexFromKine;   //!
  TBranch        *b_TimeStreamBS;   //!
  TBranch        *b_TimeTriggerConfig;   //!
  TBranch        *b_TimeStepController_L2;   //!
  TBranch        *b_TimeStepController_EF;   //!
  TBranch        *b_TimeTrigMooCnvAlg;   //!
  TBranch        *b_TimeTrigMuidMooreTPCreator;   //!
  TBranch        *b_TimeTrigMuidExtrCnvAlg;   //!
  TBranch        *b_TimeTrigMuidCnvAlg;   //!
  TBranch        *b_TimeTrigMuonBuilder;   //!
  TBranch        *b_TimeTrigInDetTrackTruthMaker;   //!
  TBranch        *b_TimeTriggerDecisionMaker;   //!
  TBranch        *b_TimeL1CaloObjectsToESD;   //!
  TBranch        *b_TimeRoIBResultToAOD;   //!
  TBranch        *b_TimephotonBuilder;   //!
  TBranch        *b_TimeelectronBuilder;   //!
  TBranch        *b_TimeMuonBuilder;   //!
  TBranch        *b_TimetauJetBuilder;   //!
  TBranch        *b_TimetauJetBuilder1p3p;   //!
  TBranch        *b_TimeKt4TowerParticleJetBuilder;   //!
  TBranch        *b_TimeKt6TowerParticleJetBuilder;   //!
  TBranch        *b_TimeCone4TowerParticleJetBuilder;   //!
  TBranch        *b_TimeConeTowerParticleJetBuilder;   //!
  TBranch        *b_TimeKt4TopoParticleJetBuilder;   //!
  TBranch        *b_TimeKt6TopoParticleJetBuilder;   //!
  TBranch        *b_TimeCone4TopoParticleJetBuilder;   //!
  TBranch        *b_TimeConeTopoParticleJetBuilder;   //!
  TBranch        *b_TimeSharedHitMapper;   //!
  TBranch        *b_TimeBJetBuilderKt4Tower;   //!
  TBranch        *b_TimeBJetBuilderKt6Tower;   //!
  TBranch        *b_TimeBJetBuilderCone4Tower;   //!
  TBranch        *b_TimeBJetBuilderConeTower;   //!
  TBranch        *b_TimeBJetBuilderKt4Topo;   //!
  TBranch        *b_TimeBJetBuilderKt6Topo;   //!
  TBranch        *b_TimeBJetBuilderCone4Topo;   //!
  TBranch        *b_TimeBJetBuilderConeTopo;   //!
  TBranch        *b_TimeKt4TruthParticleJetBuilder;   //!
  TBranch        *b_TimeKt6TruthParticleJetBuilder;   //!
  TBranch        *b_TimeCone4TruthParticleJetBuilder;   //!
  TBranch        *b_TimeConeTruthParticleJetBuilder;   //!
  TBranch        *b_TimeMcAodBuilder;   //!
  TBranch        *b_TimeTopSequence;   //!
  TBranch        *b_TimeAtlfastAodBuilder;   //!
  TBranch        *b_TimeFastAANT;   //!
  TBranch        *b_TimeFastAANTupleStream;   //!
  TBranch        *b_TimeStreamESD;   //!
  TBranch        *b_TimeMakeInputDataHeaderESD;   //!
  TBranch        *b_TimeStreamAOD;   //!
  TBranch        *b_TimeCBNT_AthenaAware;   //!
  TBranch        *b_TimeAANTupleStream;   //!
  TBranch        *b_TimeDumpSp;   //!

  WHTreeV15(TTree *tree=0);
  virtual ~WHTreeV15();
  virtual Int_t    Cut(Long64_t entry);
  virtual Int_t    GetEntry(Long64_t entry);
  virtual Long64_t LoadTree(Long64_t entry);
  virtual void     Init(TTree *tree);
  virtual void     Loop();
  virtual Bool_t   Notify();
  virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef WHTreeV15_cxx
WHTreeV15::WHTreeV15(TTree *tree)
  : fChain(0)
{
  Init(tree);
}

WHTreeV15::~WHTreeV15()
{
  if( !fChain ) { return; }
  //if( fChain->GetCurrentFile() ) { delete fChain->GetCurrentFile(); }
}

Int_t WHTreeV15::GetEntry(Long64_t entry)
{
  // Read contents of entry.
  if (!fChain) return 0;
  return fChain->GetEntry(entry);
}
Long64_t WHTreeV15::LoadTree(Long64_t entry)
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

void WHTreeV15::Init(TTree *tree)
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
  Type = 0;
  PtGen = 0;
  PhiGen = 0;
  EtaGen = 0;
  MGen = 0;
  Charge = 0;
  GenStat = 0;
  GenRef = 0;
  KMothNt = 0;
  KFDauNt = 0;
  KLDauNt = 0;
  KOriVNt = 0;
  KEndVNt = 0;
  BunchNum = 0;
  RVGen = 0;
  PhiVGen = 0;
  ZVGen = 0;
  BunchV = 0;
  RV = 0;
  PhiV = 0;
  ZV = 0;
  GenRefV = 0;
  KVMothNt = 0;
  KVFDauNt = 0;
  KVLDauNt = 0;
  Spcl_ID = 0;
  Spcl_Pt = 0;
  Spcl_Eta = 0;
  Spcl_Phi = 0;
  Spcl_M = 0;
  Spcl_Stat = 0;
  Spcl_Isol = 0;
  MuonEntRec_ID = 0;
  MuonEntRec_Ene = 0;
  MuonEntRec_Eta = 0;
  MuonEntRec_Phi = 0;
  MuonEntRec_PosX = 0;
  MuonEntRec_PosY = 0;
  MuonEntRec_PosZ = 0;
  MuonEntRec_Time = 0;
  MuonEntRec_BarCode = 0;
  vxp_vtx_x = 0;
  vxp_vtx_y = 0;
  vxp_vtx_z = 0;
  vxp_vtx_x_truth = 0;
  vxp_vtx_y_truth = 0;
  vxp_vtx_z_truth = 0;
  vxp_vtx_x_res = 0;
  vxp_vtx_y_res = 0;
  vxp_vtx_z_res = 0;
  vxp_sigvtx_x = 0;
  vxp_sigvtx_y = 0;
  vxp_sigvtx_z = 0;
  vxp_vtxchi2 = 0;
  vxp_vtxndf = 0;
  vxp_pt = 0;
  vxp_vtx_constraint_x = 0;
  vxp_vtx_constraint_y = 0;
  vxp_vtx_constraint_z = 0;
  vxp_sigvtx_constraint_x = 0;
  vxp_sigvtx_constraint_y = 0;
  vxp_sigvtx_constraint_z = 0;
  vxp_vtx_linearization_x = 0;
  vxp_vtx_linearization_y = 0;
  vxp_vtx_linearization_z = 0;
  vxp_vtx_seed_x = 0;
  vxp_vtx_seed_y = 0;
  vxp_vtx_seed_z = 0;
  vxp_vtxchi2prob = 0;
  vxp_numTracksPerVertex = 0;
  vxp_chi2 = 0;
  vxp_ndf = 0;
  vxp_vertexCompatibility = 0;
  vxp_d0 = 0;
  vxp_sigd0 = 0;
  vxp_z0 = 0;
  vxp_sigz0 = 0;
  vxp_phi = 0;
  vxp_sigphi = 0;
  vxp_theta = 0;
  vxp_sigtheta = 0;
  vxp_qOverP = 0;
  vxp_sigqOverP = 0;
  vxp_d0g = 0;
  vxp_sigd0g = 0;
  vxp_z0g = 0;
  vxp_sigz0g = 0;
  vxp_refitted_d0 = 0;
  vxp_refitted_sigd0 = 0;
  vxp_refitted_z0 = 0;
  vxp_refitted_sigz0 = 0;
  vxp_refitted_phi = 0;
  vxp_refitted_sigphi = 0;
  vxp_refitted_theta = 0;
  vxp_refitted_sigtheta = 0;
  vxp_refitted_qOverP = 0;
  vxp_refitted_sigqOverP = 0;
  Trk_d0 = 0;
  Trk_z0 = 0;
  Trk_phi = 0;
  Trk_theta = 0;
  Trk_qOverP = 0;
  Trk_Chi2 = 0;
  Trk_Ndf = 0;
  Trk_sigd0 = 0;
  Trk_sigz0 = 0;
  Trk_sigphi = 0;
  Trk_sigtheta = 0;
  Trk_sigqOverP = 0;
  Trk_covVert21 = 0;
  Trk_covVert31 = 0;
  Trk_covVert32 = 0;
  Trk_covVert41 = 0;
  Trk_covVert42 = 0;
  Trk_covVert43 = 0;
  Trk_covVert51 = 0;
  Trk_covVert52 = 0;
  Trk_covVert53 = 0;
  Trk_covVert54 = 0;
  Trk_px = 0;
  Trk_py = 0;
  Trk_pz = 0;
  Trk_pt = 0;
  Trk_numberOfBLayerHits = 0;
  Trk_numberOfPixelHits = 0;
  Trk_numberOfSCTHits = 0;
  Trk_numberOfTRTHits = 0;
  Trk_numberOfTRTHighThresholdHits = 0;
  Trk_numberOfBLayerSharedHits = 0;
  Trk_numberOfPixelSharedHits = 0;
  Trk_numberOfPixelHoles = 0;
  Trk_numberOfSCTSharedHits = 0;
  Trk_numberOfSCTHoles = 0;
  Trk_numberOfTRTOutliers = 0;
  Trk_numberOfTRTHighThresholdOutliers = 0;
  Trk_numberOfMdtHits = 0;
  Trk_numberOfTgcPhiHits = 0;
  Trk_numberOfRpcPhiHits = 0;
  Trk_numberOfCscPhiHits = 0;
  Trk_numberOfTgcEtaHits = 0;
  Trk_numberOfRpcEtaHits = 0;
  Trk_numberOfCscEtaHits = 0;
  Trk_numberOfGangedPixels = 0;
  Trk_numberOfOutliersOnTrack = 0;
  Trk_numberOfTrackSummaryTypes = 0;
  Trk_truthBarcode = 0;
  Trk_truthNt = 0;
  Trk_ParticlePdg = 0;
  Trk_ParentPdg = 0;
  Eh_PresB = 0;
  Eh_EMB = 0;
  Eh_EMEC = 0;
  Eh_Tile = 0;
  Eh_TileGap = 0;
  Eh_HEC = 0;
  Eh_FCal = 0;
  Eh_PresE = 0;
  Eh_Scint = 0;
  cl_et = 0;
  cl_e = 0;
  cl_eemb0 = 0;
  cl_eemb1 = 0;
  cl_eemb2 = 0;
  cl_eemb3 = 0;
  cl_nemb0 = 0;
  cl_nemb1 = 0;
  cl_nemb2 = 0;
  cl_nemb3 = 0;
  cl_phi2 = 0;
  cl_eta0 = 0;
  cl_eta1 = 0;
  cl_eta2 = 0;
  cl_eta3 = 0;
  cl_eeme0 = 0;
  cl_eeme1 = 0;
  cl_eeme2 = 0;
  cl_eeme3 = 0;
  cl_neme0 = 0;
  cl_neme1 = 0;
  cl_neme2 = 0;
  cl_neme3 = 0;
  cl_etileg1 = 0;
  cl_etileg2 = 0;
  cl_etileg3 = 0;
  cl_ntileg1 = 0;
  cl_ntileg2 = 0;
  cl_ntileg3 = 0;
  cl_eta = 0;
  cl_phi = 0;
  cl_time = 0;
  cl_reco_stat = 0;
  cl_m1_eta = 0;
  cl_m1_phi = 0;
  cl_m2_r = 0;
  cl_m2_lambda = 0;
  cl_delta_phi = 0;
  cl_delta_theta = 0;
  cl_delta_alpha = 0;
  cl_center_x = 0;
  cl_center_y = 0;
  cl_center_z = 0;
  cl_center_lambda = 0;
  cl_lateral = 0;
  cl_longitudinal = 0;
  cl_eng_frac_em = 0;
  cl_eng_frac_max = 0;
  cl_eng_frac_core = 0;
  cl_m1_dens = 0;
  cl_m2_dens = 0;
  cl_ntotcells = 0;
  cl_et_gam = 0;
  cl_e_gam = 0;
  cl_eemb0_gam = 0;
  cl_eemb1_gam = 0;
  cl_eemb2_gam = 0;
  cl_eemb3_gam = 0;
  cl_nemb0_gam = 0;
  cl_nemb1_gam = 0;
  cl_nemb2_gam = 0;
  cl_nemb3_gam = 0;
  cl_phi2_gam = 0;
  cl_eta0_gam = 0;
  cl_eta1_gam = 0;
  cl_eta2_gam = 0;
  cl_eta3_gam = 0;
  cl_eeme0_gam = 0;
  cl_eeme1_gam = 0;
  cl_eeme2_gam = 0;
  cl_eeme3_gam = 0;
  cl_neme0_gam = 0;
  cl_neme1_gam = 0;
  cl_neme2_gam = 0;
  cl_neme3_gam = 0;
  cl_etileg1_gam = 0;
  cl_etileg2_gam = 0;
  cl_etileg3_gam = 0;
  cl_ntileg1_gam = 0;
  cl_ntileg2_gam = 0;
  cl_ntileg3_gam = 0;
  cl_eta_gam = 0;
  cl_phi_gam = 0;
  cl_time_gam = 0;
  cl_reco_stat_gam = 0;
  cl_m1_eta_gam = 0;
  cl_m1_phi_gam = 0;
  cl_m2_r_gam = 0;
  cl_m2_lambda_gam = 0;
  cl_delta_phi_gam = 0;
  cl_delta_theta_gam = 0;
  cl_delta_alpha_gam = 0;
  cl_center_x_gam = 0;
  cl_center_y_gam = 0;
  cl_center_z_gam = 0;
  cl_center_lambda_gam = 0;
  cl_lateral_gam = 0;
  cl_longitudinal_gam = 0;
  cl_eng_frac_em_gam = 0;
  cl_eng_frac_max_gam = 0;
  cl_eng_frac_core_gam = 0;
  cl_m1_dens_gam = 0;
  cl_m2_dens_gam = 0;
  cl_ntotcells_gam = 0;
  cl_et_35 = 0;
  cl_e_35 = 0;
  cl_eemb0_35 = 0;
  cl_eemb1_35 = 0;
  cl_eemb2_35 = 0;
  cl_eemb3_35 = 0;
  cl_nemb0_35 = 0;
  cl_nemb1_35 = 0;
  cl_nemb2_35 = 0;
  cl_nemb3_35 = 0;
  cl_phi2_35 = 0;
  cl_eta0_35 = 0;
  cl_eta1_35 = 0;
  cl_eta2_35 = 0;
  cl_eta3_35 = 0;
  cl_eeme0_35 = 0;
  cl_eeme1_35 = 0;
  cl_eeme2_35 = 0;
  cl_eeme3_35 = 0;
  cl_neme0_35 = 0;
  cl_neme1_35 = 0;
  cl_neme2_35 = 0;
  cl_neme3_35 = 0;
  cl_etileg1_35 = 0;
  cl_etileg2_35 = 0;
  cl_etileg3_35 = 0;
  cl_ntileg1_35 = 0;
  cl_ntileg2_35 = 0;
  cl_ntileg3_35 = 0;
  cl_eta_35 = 0;
  cl_phi_35 = 0;
  cl_time_35 = 0;
  cl_reco_stat_35 = 0;
  cl_m1_eta_35 = 0;
  cl_m1_phi_35 = 0;
  cl_m2_r_35 = 0;
  cl_m2_lambda_35 = 0;
  cl_delta_phi_35 = 0;
  cl_delta_theta_35 = 0;
  cl_delta_alpha_35 = 0;
  cl_center_x_35 = 0;
  cl_center_y_35 = 0;
  cl_center_z_35 = 0;
  cl_center_lambda_35 = 0;
  cl_lateral_35 = 0;
  cl_longitudinal_35 = 0;
  cl_eng_frac_em_35 = 0;
  cl_eng_frac_max_35 = 0;
  cl_eng_frac_core_35 = 0;
  cl_m1_dens_35 = 0;
  cl_m2_dens_35 = 0;
  cl_ntotcells_35 = 0;
  cl_et_35gam = 0;
  cl_e_35gam = 0;
  cl_eemb0_35gam = 0;
  cl_eemb1_35gam = 0;
  cl_eemb2_35gam = 0;
  cl_eemb3_35gam = 0;
  cl_nemb0_35gam = 0;
  cl_nemb1_35gam = 0;
  cl_nemb2_35gam = 0;
  cl_nemb3_35gam = 0;
  cl_phi2_35gam = 0;
  cl_eta0_35gam = 0;
  cl_eta1_35gam = 0;
  cl_eta2_35gam = 0;
  cl_eta3_35gam = 0;
  cl_eeme0_35gam = 0;
  cl_eeme1_35gam = 0;
  cl_eeme2_35gam = 0;
  cl_eeme3_35gam = 0;
  cl_neme0_35gam = 0;
  cl_neme1_35gam = 0;
  cl_neme2_35gam = 0;
  cl_neme3_35gam = 0;
  cl_etileg1_35gam = 0;
  cl_etileg2_35gam = 0;
  cl_etileg3_35gam = 0;
  cl_ntileg1_35gam = 0;
  cl_ntileg2_35gam = 0;
  cl_ntileg3_35gam = 0;
  cl_eta_35gam = 0;
  cl_phi_35gam = 0;
  cl_time_35gam = 0;
  cl_reco_stat_35gam = 0;
  cl_m1_eta_35gam = 0;
  cl_m1_phi_35gam = 0;
  cl_m2_r_35gam = 0;
  cl_m2_lambda_35gam = 0;
  cl_delta_phi_35gam = 0;
  cl_delta_theta_35gam = 0;
  cl_delta_alpha_35gam = 0;
  cl_center_x_35gam = 0;
  cl_center_y_35gam = 0;
  cl_center_z_35gam = 0;
  cl_center_lambda_35gam = 0;
  cl_lateral_35gam = 0;
  cl_longitudinal_35gam = 0;
  cl_eng_frac_em_35gam = 0;
  cl_eng_frac_max_35gam = 0;
  cl_eng_frac_core_35gam = 0;
  cl_m1_dens_35gam = 0;
  cl_m2_dens_35gam = 0;
  cl_ntotcells_35gam = 0;
  cl_et_37 = 0;
  cl_e_37 = 0;
  cl_eemb0_37 = 0;
  cl_eemb1_37 = 0;
  cl_eemb2_37 = 0;
  cl_eemb3_37 = 0;
  cl_nemb0_37 = 0;
  cl_nemb1_37 = 0;
  cl_nemb2_37 = 0;
  cl_nemb3_37 = 0;
  cl_phi2_37 = 0;
  cl_eta0_37 = 0;
  cl_eta1_37 = 0;
  cl_eta2_37 = 0;
  cl_eta3_37 = 0;
  cl_eeme0_37 = 0;
  cl_eeme1_37 = 0;
  cl_eeme2_37 = 0;
  cl_eeme3_37 = 0;
  cl_neme0_37 = 0;
  cl_neme1_37 = 0;
  cl_neme2_37 = 0;
  cl_neme3_37 = 0;
  cl_etileg1_37 = 0;
  cl_etileg2_37 = 0;
  cl_etileg3_37 = 0;
  cl_ntileg1_37 = 0;
  cl_ntileg2_37 = 0;
  cl_ntileg3_37 = 0;
  cl_eta_37 = 0;
  cl_phi_37 = 0;
  cl_time_37 = 0;
  cl_reco_stat_37 = 0;
  cl_m1_eta_37 = 0;
  cl_m1_phi_37 = 0;
  cl_m2_r_37 = 0;
  cl_m2_lambda_37 = 0;
  cl_delta_phi_37 = 0;
  cl_delta_theta_37 = 0;
  cl_delta_alpha_37 = 0;
  cl_center_x_37 = 0;
  cl_center_y_37 = 0;
  cl_center_z_37 = 0;
  cl_center_lambda_37 = 0;
  cl_lateral_37 = 0;
  cl_longitudinal_37 = 0;
  cl_eng_frac_em_37 = 0;
  cl_eng_frac_max_37 = 0;
  cl_eng_frac_core_37 = 0;
  cl_m1_dens_37 = 0;
  cl_m2_dens_37 = 0;
  cl_ntotcells_37 = 0;
  cl_et_37gam = 0;
  cl_e_37gam = 0;
  cl_eemb0_37gam = 0;
  cl_eemb1_37gam = 0;
  cl_eemb2_37gam = 0;
  cl_eemb3_37gam = 0;
  cl_nemb0_37gam = 0;
  cl_nemb1_37gam = 0;
  cl_nemb2_37gam = 0;
  cl_nemb3_37gam = 0;
  cl_phi2_37gam = 0;
  cl_eta0_37gam = 0;
  cl_eta1_37gam = 0;
  cl_eta2_37gam = 0;
  cl_eta3_37gam = 0;
  cl_eeme0_37gam = 0;
  cl_eeme1_37gam = 0;
  cl_eeme2_37gam = 0;
  cl_eeme3_37gam = 0;
  cl_neme0_37gam = 0;
  cl_neme1_37gam = 0;
  cl_neme2_37gam = 0;
  cl_neme3_37gam = 0;
  cl_etileg1_37gam = 0;
  cl_etileg2_37gam = 0;
  cl_etileg3_37gam = 0;
  cl_ntileg1_37gam = 0;
  cl_ntileg2_37gam = 0;
  cl_ntileg3_37gam = 0;
  cl_eta_37gam = 0;
  cl_phi_37gam = 0;
  cl_time_37gam = 0;
  cl_reco_stat_37gam = 0;
  cl_m1_eta_37gam = 0;
  cl_m1_phi_37gam = 0;
  cl_m2_r_37gam = 0;
  cl_m2_lambda_37gam = 0;
  cl_delta_phi_37gam = 0;
  cl_delta_theta_37gam = 0;
  cl_delta_alpha_37gam = 0;
  cl_center_x_37gam = 0;
  cl_center_y_37gam = 0;
  cl_center_z_37gam = 0;
  cl_center_lambda_37gam = 0;
  cl_lateral_37gam = 0;
  cl_longitudinal_37gam = 0;
  cl_eng_frac_em_37gam = 0;
  cl_eng_frac_max_37gam = 0;
  cl_eng_frac_core_37gam = 0;
  cl_m1_dens_37gam = 0;
  cl_m2_dens_37gam = 0;
  cl_ntotcells_37gam = 0;
  cl_et_Softe = 0;
  cl_e_Softe = 0;
  cl_eemb0_Softe = 0;
  cl_eemb1_Softe = 0;
  cl_eemb2_Softe = 0;
  cl_eemb3_Softe = 0;
  cl_nemb0_Softe = 0;
  cl_nemb1_Softe = 0;
  cl_nemb2_Softe = 0;
  cl_nemb3_Softe = 0;
  cl_phi2_Softe = 0;
  cl_eta0_Softe = 0;
  cl_eta1_Softe = 0;
  cl_eta2_Softe = 0;
  cl_eta3_Softe = 0;
  cl_eeme0_Softe = 0;
  cl_eeme1_Softe = 0;
  cl_eeme2_Softe = 0;
  cl_eeme3_Softe = 0;
  cl_neme0_Softe = 0;
  cl_neme1_Softe = 0;
  cl_neme2_Softe = 0;
  cl_neme3_Softe = 0;
  cl_etileg1_Softe = 0;
  cl_etileg2_Softe = 0;
  cl_etileg3_Softe = 0;
  cl_ntileg1_Softe = 0;
  cl_ntileg2_Softe = 0;
  cl_ntileg3_Softe = 0;
  cl_eta_Softe = 0;
  cl_phi_Softe = 0;
  cl_time_Softe = 0;
  cl_reco_stat_Softe = 0;
  cl_m1_eta_Softe = 0;
  cl_m1_phi_Softe = 0;
  cl_m2_r_Softe = 0;
  cl_m2_lambda_Softe = 0;
  cl_delta_phi_Softe = 0;
  cl_delta_theta_Softe = 0;
  cl_delta_alpha_Softe = 0;
  cl_center_x_Softe = 0;
  cl_center_y_Softe = 0;
  cl_center_z_Softe = 0;
  cl_center_lambda_Softe = 0;
  cl_lateral_Softe = 0;
  cl_longitudinal_Softe = 0;
  cl_eng_frac_em_Softe = 0;
  cl_eng_frac_max_Softe = 0;
  cl_eng_frac_core_Softe = 0;
  cl_m1_dens_Softe = 0;
  cl_m2_dens_Softe = 0;
  cl_ntotcells_Softe = 0;
  cl_et_comb = 0;
  cl_e_comb = 0;
  cl_eemb0_comb = 0;
  cl_eemb1_comb = 0;
  cl_eemb2_comb = 0;
  cl_eemb3_comb = 0;
  cl_nemb0_comb = 0;
  cl_nemb1_comb = 0;
  cl_nemb2_comb = 0;
  cl_nemb3_comb = 0;
  cl_phi2_comb = 0;
  cl_eta0_comb = 0;
  cl_eta1_comb = 0;
  cl_eta2_comb = 0;
  cl_eta3_comb = 0;
  cl_eeme0_comb = 0;
  cl_eeme1_comb = 0;
  cl_eeme2_comb = 0;
  cl_eeme3_comb = 0;
  cl_neme0_comb = 0;
  cl_neme1_comb = 0;
  cl_neme2_comb = 0;
  cl_neme3_comb = 0;
  cl_etileg1_comb = 0;
  cl_etileg2_comb = 0;
  cl_etileg3_comb = 0;
  cl_ntileg1_comb = 0;
  cl_ntileg2_comb = 0;
  cl_ntileg3_comb = 0;
  cl_eta_comb = 0;
  cl_phi_comb = 0;
  cl_time_comb = 0;
  cl_reco_stat_comb = 0;
  cl_m1_eta_comb = 0;
  cl_m1_phi_comb = 0;
  cl_m2_r_comb = 0;
  cl_m2_lambda_comb = 0;
  cl_delta_phi_comb = 0;
  cl_delta_theta_comb = 0;
  cl_delta_alpha_comb = 0;
  cl_center_x_comb = 0;
  cl_center_y_comb = 0;
  cl_center_z_comb = 0;
  cl_center_lambda_comb = 0;
  cl_lateral_comb = 0;
  cl_longitudinal_comb = 0;
  cl_eng_frac_em_comb = 0;
  cl_eng_frac_max_comb = 0;
  cl_eng_frac_core_comb = 0;
  cl_m1_dens_comb = 0;
  cl_m2_dens_comb = 0;
  cl_ehec0_comb = 0;
  cl_ehec1_comb = 0;
  cl_ehec2_comb = 0;
  cl_ehec3_comb = 0;
  cl_nhec0_comb = 0;
  cl_nhec1_comb = 0;
  cl_nhec2_comb = 0;
  cl_nhec3_comb = 0;
  cl_etileb0_comb = 0;
  cl_etileb1_comb = 0;
  cl_etileb2_comb = 0;
  cl_ntileb0_comb = 0;
  cl_ntileb1_comb = 0;
  cl_ntileb2_comb = 0;
  cl_etilee0_comb = 0;
  cl_etilee1_comb = 0;
  cl_etilee2_comb = 0;
  cl_efcal0_comb = 0;
  cl_efcal1_comb = 0;
  cl_efcal2_comb = 0;
  cl_ntilee0_comb = 0;
  cl_ntilee1_comb = 0;
  cl_ntilee2_comb = 0;
  cl_nfcal0_comb = 0;
  cl_nfcal1_comb = 0;
  cl_nfcal2_comb = 0;
  cl_ntotcells_comb = 0;
  cl_et_topoEM = 0;
  cl_e_topoEM = 0;
  cl_eemb0_topoEM = 0;
  cl_eemb1_topoEM = 0;
  cl_eemb2_topoEM = 0;
  cl_eemb3_topoEM = 0;
  cl_nemb0_topoEM = 0;
  cl_nemb1_topoEM = 0;
  cl_nemb2_topoEM = 0;
  cl_nemb3_topoEM = 0;
  cl_phi2_topoEM = 0;
  cl_eta0_topoEM = 0;
  cl_eta1_topoEM = 0;
  cl_eta2_topoEM = 0;
  cl_eta3_topoEM = 0;
  cl_eeme0_topoEM = 0;
  cl_eeme1_topoEM = 0;
  cl_eeme2_topoEM = 0;
  cl_eeme3_topoEM = 0;
  cl_neme0_topoEM = 0;
  cl_neme1_topoEM = 0;
  cl_neme2_topoEM = 0;
  cl_neme3_topoEM = 0;
  cl_etileg1_topoEM = 0;
  cl_etileg2_topoEM = 0;
  cl_etileg3_topoEM = 0;
  cl_ntileg1_topoEM = 0;
  cl_ntileg2_topoEM = 0;
  cl_ntileg3_topoEM = 0;
  cl_eta_topoEM = 0;
  cl_phi_topoEM = 0;
  cl_time_topoEM = 0;
  cl_reco_stat_topoEM = 0;
  cl_m1_eta_topoEM = 0;
  cl_m1_phi_topoEM = 0;
  cl_m2_r_topoEM = 0;
  cl_m2_lambda_topoEM = 0;
  cl_delta_phi_topoEM = 0;
  cl_delta_theta_topoEM = 0;
  cl_delta_alpha_topoEM = 0;
  cl_center_x_topoEM = 0;
  cl_center_y_topoEM = 0;
  cl_center_z_topoEM = 0;
  cl_center_lambda_topoEM = 0;
  cl_lateral_topoEM = 0;
  cl_longitudinal_topoEM = 0;
  cl_eng_frac_em_topoEM = 0;
  cl_eng_frac_max_topoEM = 0;
  cl_eng_frac_core_topoEM = 0;
  cl_m1_dens_topoEM = 0;
  cl_m2_dens_topoEM = 0;
  cl_ntotcells_topoEM = 0;
  IS_Xpt = 0;
  IS_Ypt = 0;
  IS_Zpt = 0;
  IS_Xvec = 0;
  IS_Yvec = 0;
  IS_Zvec = 0;
  IS_QualityFactor = 0;
  IS_NbreOfMultiLayer = 0;
  IS_TrackSharing = 0;
  IS_StationName = 0;
  IS_Ndigits = 0;
  VT_A0 = 0;
  VT_Z = 0;
  VT_Phi = 0;
  VT_CotTh = 0;
  VT_PTInv = 0;
  VT_Cov11 = 0;
  VT_Cov21 = 0;
  VT_Cov22 = 0;
  VT_Cov31 = 0;
  VT_Cov32 = 0;
  VT_Cov33 = 0;
  VT_Cov41 = 0;
  VT_Cov42 = 0;
  VT_Cov43 = 0;
  VT_Cov44 = 0;
  VT_Cov51 = 0;
  VT_Cov52 = 0;
  VT_Cov53 = 0;
  VT_Cov54 = 0;
  VT_Cov55 = 0;
  VT_Xpt = 0;
  VT_Ypt = 0;
  VT_Zpt = 0;
  VT_Xvec = 0;
  VT_Yvec = 0;
  VT_Zvec = 0;
  VT_Mom = 0;
  VT_digits = 0;
  KE_Thept = 0;
  KE_Phipt = 0;
  KE_Thevc = 0;
  KE_Phivc = 0;
  KE_PInv = 0;
  KE_Cov11 = 0;
  KE_Cov21 = 0;
  KE_Cov22 = 0;
  KE_Cov31 = 0;
  KE_Cov32 = 0;
  KE_Cov33 = 0;
  KE_Cov41 = 0;
  KE_Cov42 = 0;
  KE_Cov43 = 0;
  KE_Cov44 = 0;
  KE_Cov51 = 0;
  KE_Cov52 = 0;
  KE_Cov53 = 0;
  KE_Cov54 = 0;
  KE_Cov55 = 0;
  KE_Xpt = 0;
  KE_Ypt = 0;
  KE_Zpt = 0;
  KE_Xvec = 0;
  KE_Yvec = 0;
  KE_Zvec = 0;
  KE_Mom = 0;
  KE_ZCyl = 0;
  KE_RCyl = 0;
  MS_Thept = 0;
  MS_Phipt = 0;
  MS_Thevc = 0;
  MS_Phivc = 0;
  MS_PInv = 0;
  MS_Cov11 = 0;
  MS_Cov21 = 0;
  MS_Cov22 = 0;
  MS_Cov31 = 0;
  MS_Cov32 = 0;
  MS_Cov33 = 0;
  MS_Cov41 = 0;
  MS_Cov42 = 0;
  MS_Cov43 = 0;
  MS_Cov44 = 0;
  MS_Cov51 = 0;
  MS_Cov52 = 0;
  MS_Cov53 = 0;
  MS_Cov54 = 0;
  MS_Cov55 = 0;
  MS_Xpt = 0;
  MS_Ypt = 0;
  MS_Zpt = 0;
  MS_Xvec = 0;
  MS_Yvec = 0;
  MS_Zvec = 0;
  MS_Mom = 0;
  MS_ZCyl = 0;
  MS_RCyl = 0;
  MbKE_Calo_En_dr01 = 0;
  MbKE_Calo_Et_dr01 = 0;
  MbKE_HAD_Calo_En_dr01 = 0;
  MbKE_HAD_Calo_Et_dr01 = 0;
  MbKE_EM_Calo_En_dr01 = 0;
  MbKE_EM_Calo_Et_dr01 = 0;
  Mboy_A0 = 0;
  Mboy_Z = 0;
  Mboy_Phi = 0;
  Mboy_Theta = 0;
  Mboy_qOverP = 0;
  Mboy_covr11 = 0;
  Mboy_covr21 = 0;
  Mboy_covr22 = 0;
  Mboy_covr31 = 0;
  Mboy_covr32 = 0;
  Mboy_covr33 = 0;
  Mboy_covr41 = 0;
  Mboy_covr42 = 0;
  Mboy_covr43 = 0;
  Mboy_covr44 = 0;
  Mboy_covr51 = 0;
  Mboy_covr52 = 0;
  Mboy_covr53 = 0;
  Mboy_covr54 = 0;
  Mboy_covr55 = 0;
  Mboy_etcone10 = 0;
  Mboy_etcone20 = 0;
  Mboy_etcone30 = 0;
  Mboy_etcone40 = 0;
  Mboy_etcone = 0;
  Mboy_etcone50 = 0;
  Mboy_etcone60 = 0;
  Mboy_etcone70 = 0;
  Mboy_nucone10 = 0;
  Mboy_nucone20 = 0;
  Mboy_nucone30 = 0;
  Mboy_nucone40 = 0;
  Mboy_nucone = 0;
  Mboy_nucone50 = 0;
  Mboy_nucone60 = 0;
  Mboy_nucone70 = 0;
  Mboy_Eloss = 0;
  Mboy_ElossError = 0;
  Moore_xvtxg = 0;
  Moore_yvtxg = 0;
  Moore_zvtxg = 0;
  Moore_a0g = 0;
  Moore_z0g = 0;
  Moore_phig = 0;
  Moore_cotthg = 0;
  Moore_ptig = 0;
  Moore_etag = 0;
  Moore_rassociated = 0;
  Moore_matchdg = 0;
  Moore_xextrg = 0;
  Moore_yextrg = 0;
  Moore_zextrg = 0;
  Moore_a0extrg = 0;
  Moore_z0extrg = 0;
  Moore_phiextrg = 0;
  Moore_cthextrg = 0;
  Moore_ptiextrg = 0;
  Moore_etaextrg = 0;
  Moore_barcodeg = 0;
  Moore_statusg = 0;
  Moore_elossg = 0;
  Moore_drphig = 0;
  Moore_drzg = 0;
  Moore_dphig = 0;
  Moore_dthetag = 0;
  Moore_a0pullg = 0;
  Moore_z0pullg = 0;
  Moore_phipullg = 0;
  Moore_cotthpullg = 0;
  Moore_ptipullg = 0;
  Moore_phih = 0;
  Moore_etah = 0;
  Moore_mdth = 0;
  Moore_cscetah = 0;
  Moore_cscphih = 0;
  Moore_rpcetah = 0;
  Moore_rpcphih = 0;
  Moore_tgcetah = 0;
  Moore_tgcphih = 0;
  Moore_stati = 0;
  Moore_statr = 0;
  Moore_xvtxr = 0;
  Moore_yvtxr = 0;
  Moore_zvtxr = 0;
  Moore_a0r = 0;
  Moore_z0r = 0;
  Moore_phir = 0;
  Moore_cotthr = 0;
  Moore_ptir = 0;
  Moore_etar = 0;
  Moore_chi2 = 0;
  Moore_chi2pr = 0;
  Moore_origx = 0;
  Moore_origy = 0;
  Moore_origz = 0;
  Moore_covr11 = 0;
  Moore_covr12 = 0;
  Moore_covr22 = 0;
  Moore_covr13 = 0;
  Moore_covr23 = 0;
  Moore_covr33 = 0;
  Moore_covr14 = 0;
  Moore_covr24 = 0;
  Moore_covr34 = 0;
  Moore_covr44 = 0;
  Moore_covr15 = 0;
  Moore_covr25 = 0;
  Moore_covr35 = 0;
  Moore_covr45 = 0;
  Moore_covr55 = 0;
  conv_vtx_x = 0;
  conv_vtx_y = 0;
  conv_vtx_z = 0;
  conv_is_conv = 0;
  conv_vtx_x_truth = 0;
  conv_vtx_y_truth = 0;
  conv_vtx_z_truth = 0;
  conv_sigvtx_x = 0;
  conv_sigvtx_y = 0;
  conv_sigvtx_z = 0;
  conv_vtxchi2 = 0;
  conv_vtxndf = 0;
  conv_vtxchi2prob = 0;
  conv_numTracksPerVertex = 0;
  conv_track1_chi2 = 0;
  conv_track2_chi2 = 0;
  conv_track1_d0 = 0;
  conv_track1_sigd0 = 0;
  conv_track1_z0 = 0;
  conv_track1_sigz0 = 0;
  conv_track1_phi = 0;
  conv_track1_sigphi = 0;
  conv_track1_theta = 0;
  conv_track1_sigtheta = 0;
  conv_track1_qOverP = 0;
  conv_track1_sigqOverP = 0;
  conv_track1_momentum_x = 0;
  conv_track1_momentum_y = 0;
  conv_track1_momentum_z = 0;
  conv_track1_d0g = 0;
  conv_track1_sigd0g = 0;
  conv_track1_z0g = 0;
  conv_track1_sigz0g = 0;
  conv_track2_d0 = 0;
  conv_track2_sigd0 = 0;
  conv_track2_z0 = 0;
  conv_track2_sigz0 = 0;
  conv_track2_phi = 0;
  conv_track2_sigphi = 0;
  conv_track2_theta = 0;
  conv_track2_sigtheta = 0;
  conv_track2_qOverP = 0;
  conv_track2_sigqOverP = 0;
  conv_track2_momentum_x = 0;
  conv_track2_momentum_y = 0;
  conv_track2_momentum_z = 0;
  conv_track2_d0g = 0;
  conv_track2_sigd0g = 0;
  conv_track2_z0g = 0;
  conv_track2_sigz0g = 0;
  conv_true_vtx_x = 0;
  conv_true_vtx_y = 0;
  conv_true_vtx_z = 0;
  jetEtaCJets = 0;
  jetPhiCJets = 0;
  jetECJets = 0;
  jetEemCJets = 0;
  jetEtCJets = 0;
  jetMCJets = 0;
  jetPxCJets = 0;
  jetPyCJets = 0;
  jetPzCJets = 0;
  jetSizeCJets = 0;
  jetEtaC4Jets = 0;
  jetPhiC4Jets = 0;
  jetEC4Jets = 0;
  jetEemC4Jets = 0;
  jetEtC4Jets = 0;
  jetMC4Jets = 0;
  jetPxC4Jets = 0;
  jetPyC4Jets = 0;
  jetPzC4Jets = 0;
  jetSizeC4Jets = 0;
  jetEtaKt6Jets = 0;
  jetPhiKt6Jets = 0;
  jetEKt6Jets = 0;
  jetEemKt6Jets = 0;
  jetEtKt6Jets = 0;
  jetMKt6Jets = 0;
  jetPxKt6Jets = 0;
  jetPyKt6Jets = 0;
  jetPzKt6Jets = 0;
  jetSizeKt6Jets = 0;
  jetEtaKt4Jets = 0;
  jetPhiKt4Jets = 0;
  jetEKt4Jets = 0;
  jetEemKt4Jets = 0;
  jetEtKt4Jets = 0;
  jetMKt4Jets = 0;
  jetPxKt4Jets = 0;
  jetPyKt4Jets = 0;
  jetPzKt4Jets = 0;
  jetSizeKt4Jets = 0;
  jetEtaCTopoJets = 0;
  jetPhiCTopoJets = 0;
  jetECTopoJets = 0;
  jetEemCTopoJets = 0;
  jetEtCTopoJets = 0;
  jetMCTopoJets = 0;
  jetPxCTopoJets = 0;
  jetPyCTopoJets = 0;
  jetPzCTopoJets = 0;
  jetSizeCTopoJets = 0;
  jetEtaC4TopoJets = 0;
  jetPhiC4TopoJets = 0;
  jetEC4TopoJets = 0;
  jetEemC4TopoJets = 0;
  jetEtC4TopoJets = 0;
  jetMC4TopoJets = 0;
  jetPxC4TopoJets = 0;
  jetPyC4TopoJets = 0;
  jetPzC4TopoJets = 0;
  jetSizeC4TopoJets = 0;
  jetEtaKt6TopoJets = 0;
  jetPhiKt6TopoJets = 0;
  jetEKt6TopoJets = 0;
  jetEemKt6TopoJets = 0;
  jetEtKt6TopoJets = 0;
  jetMKt6TopoJets = 0;
  jetPxKt6TopoJets = 0;
  jetPyKt6TopoJets = 0;
  jetPzKt6TopoJets = 0;
  jetSizeKt6TopoJets = 0;
  jetEtaKt4TopoJets = 0;
  jetPhiKt4TopoJets = 0;
  jetEKt4TopoJets = 0;
  jetEemKt4TopoJets = 0;
  jetEtKt4TopoJets = 0;
  jetMKt4TopoJets = 0;
  jetPxKt4TopoJets = 0;
  jetPyKt4TopoJets = 0;
  jetPzKt4TopoJets = 0;
  jetSizeKt4TopoJets = 0;
  jetEtaCMcJets = 0;
  jetPhiCMcJets = 0;
  jetECMcJets = 0;
  jetEemCMcJets = 0;
  jetEtCMcJets = 0;
  jetMCMcJets = 0;
  jetPxCMcJets = 0;
  jetPyCMcJets = 0;
  jetPzCMcJets = 0;
  jetSizeCMcJets = 0;
  jetEtaC4McJets = 0;
  jetPhiC4McJets = 0;
  jetEC4McJets = 0;
  jetEemC4McJets = 0;
  jetEtC4McJets = 0;
  jetMC4McJets = 0;
  jetPxC4McJets = 0;
  jetPyC4McJets = 0;
  jetPzC4McJets = 0;
  jetSizeC4McJets = 0;
  jetEtaKt4McJets = 0;
  jetPhiKt4McJets = 0;
  jetEKt4McJets = 0;
  jetEemKt4McJets = 0;
  jetEtKt4McJets = 0;
  jetMKt4McJets = 0;
  jetPxKt4McJets = 0;
  jetPyKt4McJets = 0;
  jetPzKt4McJets = 0;
  jetSizeKt4McJets = 0;
  jetEtaKt6McJets = 0;
  jetPhiKt6McJets = 0;
  jetEKt6McJets = 0;
  jetEemKt6McJets = 0;
  jetEtKt6McJets = 0;
  jetMKt6McJets = 0;
  jetPxKt6McJets = 0;
  jetPyKt6McJets = 0;
  jetPzKt6McJets = 0;
  jetSizeKt6McJets = 0;
  tau_ET = 0;
  tau_eta = 0;
  tau_phi = 0;
  tau_nStrip = 0;
  tau_stripWidth2 = 0;
  tau_EMRadius = 0;
  tau_IsoFrac = 0;
  tau_ntrack = 0;
  tau_charge = 0;
  tau_secVtxX = 0;
  tau_secVtxY = 0;
  tau_secVtxZ = 0;
  tau_secVtxXError = 0;
  tau_secVtxYError = 0;
  tau_secVtxZError = 0;
  tau_sumEMe = 0;
  tau_sumEMpx = 0;
  tau_sumEMpy = 0;
  tau_sumEMpz = 0;
  tau_energy = 0;
  tau_EMenergy = 0;
  tau_nEMCell = 0;
  tau_stripET = 0;
  tau_ptTrack1 = 0;
  tau_etaTrack1 = 0;
  tau_phiTrack1 = 0;
  tau_distTrack1 = 0;
  tau_ptTrack2 = 0;
  tau_etaTrack2 = 0;
  tau_phiTrack2 = 0;
  tau_distTrack2 = 0;
  tau_ptTrack3 = 0;
  tau_etaTrack3 = 0;
  tau_phiTrack3 = 0;
  tau_distTrack3 = 0;
  tau_etEMCalib = 0;
  tau_etHadCalib = 0;
  tau_Likelihood = 0;
  tau_tauJetLL = 0;
  tau_tauELL = 0;
  tau_tauJetNN = 0;
  tau_tauENN = 0;
  tau_nTracksdrdR = 0;
  tau_accept = 0;
  tau1p3p_ET = 0;
  tau1p3p_eta = 0;
  tau1p3p_phi = 0;
  tau1p3p_nStrip = 0;
  tau1p3p_stripWidth2 = 0;
  tau1p3p_EMRadius = 0;
  tau1p3p_IsoFrac = 0;
  tau1p3p_ntrack = 0;
  tau1p3p_charge = 0;
  tau1p3p_secVtxX = 0;
  tau1p3p_secVtxY = 0;
  tau1p3p_secVtxZ = 0;
  tau1p3p_secVtxXError = 0;
  tau1p3p_secVtxYError = 0;
  tau1p3p_secVtxZError = 0;
  tau1p3p_sumEMe = 0;
  tau1p3p_sumEMpx = 0;
  tau1p3p_sumEMpy = 0;
  tau1p3p_sumEMpz = 0;
  tau1p3p_track1Nt = 0;
  tau1p3p_track2Nt = 0;
  tau1p3p_track3Nt = 0;
  tau1p3p_ptTrack1 = 0;
  tau1p3p_ptTrack2 = 0;
  tau1p3p_ptTrack3 = 0;
  tau1p3p_track1ETChrgEM01 = 0;
  tau1p3p_track2ETChrgEM01 = 0;
  tau1p3p_track3ETChrgEM01 = 0;
  tau1p3p_track1ETResChrgEMTrk = 0;
  tau1p3p_track2ETResChrgEMTrk = 0;
  tau1p3p_track3ETResChrgEMTrk = 0;
  tau1p3p_etEMCL = 0;
  tau1p3p_etNeuEM = 0;
  tau1p3p_etChrgEM = 0;
  tau1p3p_etResNeuEM = 0;
  tau1p3p_cellsChrgHAD = 0;
  tau1p3p_cellsOtherEM = 0;
  tau1p3p_cellsOtherHAD = 0;
  tau1p3p_ETeflow = 0;
  tau1p3p_ETcalo = 0;
  tau1p3p_MVisEflow = 0;
  tau1p3p_discriCut = 0;
  tau1p3p_discriNN = 0;
  tau1p3p_discriPDRS = 0;
  tau1p3p_MTrk3P = 0;
  tau1p3p_NAssocTracksCore = 0;
  tau1p3p_NAssocTracksIsol = 0;
  tau1p3p_RWidth2Trk3P = 0;
  tau1p3p_SignD0Trk3P = 0;
  eg_e = 0;
  eg_eta = 0;
  eg_phi = 0;
  eg_cl_et = 0;
  eg_cl_eta = 0;
  eg_cl_phi = 0;
  eg_etap = 0;
  eg_zvertex = 0;
  eg_errz = 0;
  eg_depth = 0;
  eg_e0 = 0;
  eg_e1 = 0;
  eg_e2 = 0;
  eg_e3 = 0;
  eg_eta0 = 0;
  eg_eta1 = 0;
  eg_eta2 = 0;
  eg_eta3 = 0;
  eg_phi0 = 0;
  eg_phi1 = 0;
  eg_phi2 = 0;
  eg_phi3 = 0;
  eg_Etha1 = 0;
  eg_Etha = 0;
  eg_Eha1 = 0;
  eg_F1 = 0;
  eg_F3 = 0;
  eg_E233 = 0;
  eg_E237 = 0;
  eg_E277 = 0;
  eg_Weta1 = 0;
  eg_Weta2 = 0;
  eg_E2ts1 = 0;
  eg_E2tsts1 = 0;
  eg_Widths1 = 0;
  eg_Widths2 = 0;
  eg_poscs1 = 0;
  eg_poscs2 = 0;
  eg_Barys1 = 0;
  eg_Wtots1 = 0;
  eg_Emins1 = 0;
  eg_Emaxs1 = 0;
  eg_Fracs1 = 0;
  eg_EtCone45 = 0;
  eg_EtCone20 = 0;
  eg_EtCone30 = 0;
  eg_EtCone40 = 0;
  eg_IsEM = 0;
  eg_epiNN = 0;
  eg_EMWeight = 0;
  eg_PionWeight = 0;
  eg_Hmatrix = 0;
  eg_IsEMse = 0;
  eg_epiNNse = 0;
  eg_EMWeightse = 0;
  eg_PionWeightse = 0;
  eg_F1core = 0;
  eg_F3core = 0;
  eg_Asy1 = 0;
  eg_Pos7 = 0;
  eg_Isol = 0;
  eg_Ecore = 0;
  eg_E011 = 0;
  eg_E033 = 0;
  eg_E131 = 0;
  eg_E1153 = 0;
  eg_E235 = 0;
  eg_E255 = 0;
  eg_E333 = 0;
  eg_E335 = 0;
  eg_E337 = 0;
  eg_E377 = 0;
  eg_trkmatchnt = 0;
  eg_primary = 0;
  eg_numtracks = 0;
  eg_eoverp = 0;
  eg_etacorr = 0;
  eg_deta0 = 0;
  eg_dphi0 = 0;
  eg_deta1 = 0;
  eg_dphi1 = 0;
  eg_deta2 = 0;
  eg_dphi2 = 0;
  eg_deta3 = 0;
  eg_dphi3 = 0;
  eg_trkopt = 0;
  eg_trkinvpTV = 0;
  eg_trkcotThV = 0;
  eg_trkphiV = 0;
  eg_convTrkMatch = 0;
  eg_convAngleMatch = 0;
  eg_e_SE = 0;
  eg_eta_SE = 0;
  eg_phi_SE = 0;
  eg_cl_et_SE = 0;
  eg_cl_eta_SE = 0;
  eg_cl_phi_SE = 0;
  eg_etap_SE = 0;
  eg_zvertex_SE = 0;
  eg_errz_SE = 0;
  eg_depth_SE = 0;
  eg_e0_SE = 0;
  eg_e1_SE = 0;
  eg_e2_SE = 0;
  eg_e3_SE = 0;
  eg_eta0_SE = 0;
  eg_eta1_SE = 0;
  eg_eta2_SE = 0;
  eg_eta3_SE = 0;
  eg_phi0_SE = 0;
  eg_phi1_SE = 0;
  eg_phi2_SE = 0;
  eg_phi3_SE = 0;
  eg_Etha1_SE = 0;
  eg_Etha_SE = 0;
  eg_Eha1_SE = 0;
  eg_F1_SE = 0;
  eg_F3_SE = 0;
  eg_E233_SE = 0;
  eg_E237_SE = 0;
  eg_E277_SE = 0;
  eg_Weta1_SE = 0;
  eg_Weta2_SE = 0;
  eg_E2ts1_SE = 0;
  eg_E2tsts1_SE = 0;
  eg_Widths1_SE = 0;
  eg_Widths2_SE = 0;
  eg_poscs1_SE = 0;
  eg_poscs2_SE = 0;
  eg_Barys1_SE = 0;
  eg_Wtots1_SE = 0;
  eg_Emins1_SE = 0;
  eg_Emaxs1_SE = 0;
  eg_Fracs1_SE = 0;
  eg_EtCone45_SE = 0;
  eg_EtCone20_SE = 0;
  eg_EtCone30_SE = 0;
  eg_EtCone40_SE = 0;
  eg_IsEM_SE = 0;
  eg_epiNN_SE = 0;
  eg_EMWeight_SE = 0;
  eg_PionWeight_SE = 0;
  eg_Hmatrix_SE = 0;
  eg_IsEMse_SE = 0;
  eg_epiNNse_SE = 0;
  eg_EMWeightse_SE = 0;
  eg_PionWeightse_SE = 0;
  eg_F1core_SE = 0;
  eg_F3core_SE = 0;
  eg_Asy1_SE = 0;
  eg_Pos7_SE = 0;
  eg_Isol_SE = 0;
  eg_Ecore_SE = 0;
  eg_E011_SE = 0;
  eg_E033_SE = 0;
  eg_E131_SE = 0;
  eg_E1153_SE = 0;
  eg_E235_SE = 0;
  eg_E255_SE = 0;
  eg_E333_SE = 0;
  eg_E335_SE = 0;
  eg_E337_SE = 0;
  eg_E377_SE = 0;
  eg_trkmatchnt_SE = 0;
  eg_primary_SE = 0;
  eg_numtracks_SE = 0;
  eg_eoverp_SE = 0;
  eg_etacorr_SE = 0;
  eg_deta0_SE = 0;
  eg_dphi0_SE = 0;
  eg_deta1_SE = 0;
  eg_dphi1_SE = 0;
  eg_deta2_SE = 0;
  eg_dphi2_SE = 0;
  eg_deta3_SE = 0;
  eg_dphi3_SE = 0;
  eg_trkopt_SE = 0;
  eg_trkinvpTV_SE = 0;
  eg_trkcotThV_SE = 0;
  eg_trkphiV_SE = 0;
  eg_convTrkMatch_SE = 0;
  eg_convAngleMatch_SE = 0;
  egtruth_et = 0;
  egtruth_eta = 0;
  egtruth_phi = 0;
  egtruth_id = 0;
  egtruth_barcode = 0;
  egtruth_truthnt = 0;
  egtruth_etIsol = 0;
  egtruth_etaCalo = 0;
  egtruth_phiCalo = 0;
  EtaTileMuonTag = 0;
  PhiTileMuonTag = 0;
  EnergyVecTileMuonTag = 0;
  LabelTileMuonTag = 0;
  Muid_a0rms = 0;
  Muid_z0rms = 0;
  Muid_phirms = 0;
  Muid_thetarms = 0;
  Muid_etarms = 0;
  Muid_pirms = 0;
  Muid_chi2ms = 0;
  Muid_mdtms = 0;
  Muid_cscetams = 0;
  Muid_cscphims = 0;
  Muid_rpcetams = 0;
  Muid_rpcphims = 0;
  Muid_tgcetams = 0;
  Muid_tgcphims = 0;
  Muid_covr11ms = 0;
  Muid_covr12ms = 0;
  Muid_covr22ms = 0;
  Muid_covr13ms = 0;
  Muid_covr23ms = 0;
  Muid_covr33ms = 0;
  Muid_covr14ms = 0;
  Muid_covr24ms = 0;
  Muid_covr34ms = 0;
  Muid_covr44ms = 0;
  Muid_covr15ms = 0;
  Muid_covr25ms = 0;
  Muid_covr35ms = 0;
  Muid_covr45ms = 0;
  Muid_covr55ms = 0;
  Muid_prelossmu = 0;
  Muid_erelossmu = 0;
  Muid_isparelossmu = 0;
  Muid_a0rmu = 0;
  Muid_z0rmu = 0;
  Muid_phirmu = 0;
  Muid_thetarmu = 0;
  Muid_etarmu = 0;
  Muid_pirmu = 0;
  Muid_chi2mu = 0;
  Muid_covr11mu = 0;
  Muid_covr12mu = 0;
  Muid_covr22mu = 0;
  Muid_covr13mu = 0;
  Muid_covr23mu = 0;
  Muid_covr33mu = 0;
  Muid_covr14mu = 0;
  Muid_covr24mu = 0;
  Muid_covr34mu = 0;
  Muid_covr44mu = 0;
  Muid_covr15mu = 0;
  Muid_covr25mu = 0;
  Muid_covr35mu = 0;
  Muid_covr45mu = 0;
  Muid_covr55mu = 0;
  Muid_mslinkmu = 0;
  Muid_chi2mcb = 0;
  Muid_loosecb = 0;
  Muid_a0rcb = 0;
  Muid_z0rcb = 0;
  Muid_phircb = 0;
  Muid_thetarcb = 0;
  Muid_etarcb = 0;
  Muid_pircb = 0;
  Muid_chi2cb = 0;
  Muid_chi2prcb = 0;
  Muid_covr11cb = 0;
  Muid_covr12cb = 0;
  Muid_covr22cb = 0;
  Muid_covr13cb = 0;
  Muid_covr23cb = 0;
  Muid_covr33cb = 0;
  Muid_covr14cb = 0;
  Muid_covr24cb = 0;
  Muid_covr34cb = 0;
  Muid_covr44cb = 0;
  Muid_covr15cb = 0;
  Muid_covr25cb = 0;
  Muid_covr35cb = 0;
  Muid_covr45cb = 0;
  Muid_covr55cb = 0;
  Muid_mslinkcb = 0;
  Muid_exlinkcb = 0;
  Muid_blayercb = 0;
  Muid_pixelcb = 0;
  Muid_sctcb = 0;
  Muid_trtcb = 0;
  Muid_trthighcb = 0;
  Muid_a0rid = 0;
  Muid_z0rid = 0;
  Muid_phirid = 0;
  Muid_thetarid = 0;
  Muid_etarid = 0;
  Muid_pirid = 0;
  Muid_chi2id = 0;
  Muid_pulla0cb = 0;
  Muid_pullz0cb = 0;
  Muid_pullphicb = 0;
  Muid_pullthecb = 0;
  Muid_pullmomcb = 0;
  staco_A0 = 0;
  staco_Z = 0;
  staco_Phi = 0;
  staco_Theta = 0;
  staco_qOverP = 0;
  staco_chi2 = 0;
  staco_covr11 = 0;
  staco_covr21 = 0;
  staco_covr22 = 0;
  staco_covr31 = 0;
  staco_covr32 = 0;
  staco_covr33 = 0;
  staco_covr41 = 0;
  staco_covr42 = 0;
  staco_covr43 = 0;
  staco_covr44 = 0;
  staco_covr51 = 0;
  staco_covr52 = 0;
  staco_covr53 = 0;
  staco_covr54 = 0;
  staco_covr55 = 0;
  staco_A0MS = 0;
  staco_ZMS = 0;
  staco_PhiMS = 0;
  staco_ThetaMS = 0;
  staco_qOverPMS = 0;
  staco_A0ID = 0;
  staco_ZID = 0;
  staco_PhiID = 0;
  staco_ThetaID = 0;
  staco_qOverPID = 0;
  MuTag_A0 = 0;
  MuTag_Z = 0;
  MuTag_Phi = 0;
  MuTag_Theta = 0;
  MuTag_qOverP = 0;
  MuTag_covr11 = 0;
  MuTag_covr21 = 0;
  MuTag_covr22 = 0;
  MuTag_covr31 = 0;
  MuTag_covr32 = 0;
  MuTag_covr33 = 0;
  MuTag_covr41 = 0;
  MuTag_covr42 = 0;
  MuTag_covr43 = 0;
  MuTag_covr44 = 0;
  MuTag_covr51 = 0;
  MuTag_covr52 = 0;
  MuTag_covr53 = 0;
  MuTag_covr54 = 0;
  MuTag_covr55 = 0;
  L1Em_RoIWord = 0;
  L1Em_Core = 0;
  L1Em_EmClus = 0;
  L1Em_TauClus = 0;
  L1Em_EmIsol = 0;
  L1Em_HdIsol = 0;
  L1Em_HdCore = 0;
  L1Em_EmTauThresh = 0;
  L1Em_EmTauThresh = 0;
  L1Em_eta = 0;
  L1Em_phi = 0;
  L1Jet_JetRoIWord = 0;
  L1Jet_ET4x4 = 0;
  L1Jet_ET6x6 = 0;
  L1Jet_ET8x8 = 0;
  L1Jet_Thresh = 0;
  L1Jet_eta = 0;
  L1Jet_phi = 0;
  dataWord = 0;
  CTP_ROI = 0;
  muCTPI_ROI = 0;
  EMTau_ROI = 0;
  JetEnergy_ROI = 0;
  T2CaEmE = 0;
  T2CaRawEmE = 0;
  T2CaEmES0 = 0;
  T2CaEmES1 = 0;
  T2CaEmES2 = 0;
  T2CaEmES3 = 0;
  T2CaHadE = 0;
  T2CaRawHadE = 0;
  T2CaHadES0 = 0;
  T2CaHadES1 = 0;
  T2CaHadES2 = 0;
  T2CaHadES3 = 0;
  T2CaRcore = 0;
  T2CaEratio = 0;
  T2CaWidth = 0;
  T2CaF73 = 0;
  T2CaEta = 0;
  T2CaPhi = 0;
  T2CaRawEta = 0;
  T2CaRawPhi = 0;
  T2CaL1Sim_Eta = 0;
  T2CaL1Sim_Phi = 0;
  T2CaL1Sim_EmClus = 0;
  T2CaL1Sim_EmIsol = 0;
  T2CaL1Sim_HdCore = 0;
  T2CaL1Sim_HdIsol = 0;
  T2CaL1Sim_nRoIperRegion = 0;
  T2CaRoIword = 0;
  T2CaTauEta = 0;
  T2CaTauPhi = 0;
  T2CaEMES0_nar = 0;
  T2CaEMES0_wid = 0;
  T2CaEMES1_nar = 0;
  T2CaEMES1_wid = 0;
  T2CaEMES2_nar = 0;
  T2CaEMES2_wid = 0;
  T2CaEMES3_nar = 0;
  T2CaEMES3_wid = 0;
  T2CaHADES1_nar = 0;
  T2CaEHADS1_wid = 0;
  T2CaHADES2_nar = 0;
  T2CaEHADS2_wid = 0;
  T2CaHADES3_nar = 0;
  T2CaEHADS3_wid = 0;
  T2CaEMrad0 = 0;
  T2CaEMrad1 = 0;
  T2CaEMrad2 = 0;
  T2CaEMrad3 = 0;
  T2CaEMwid0 = 0;
  T2CaEMwid1 = 0;
  T2CaEMwid2 = 0;
  T2CaEMwid3 = 0;
  T2CaHADwid1 = 0;
  T2CaHADwid2 = 0;
  T2CaHADwid3 = 0;
  T2CaEMenorm0 = 0;
  T2CaEMenorm1 = 0;
  T2CaEMenorm2 = 0;
  T2CaEMenorm3 = 0;
  T2CaHADenorm1 = 0;
  T2CaHADenorm2 = 0;
  T2CaHADenorm3 = 0;
  T2CaNumStripCells = 0;
  T2CaEnergyCalib = 0;
  T2CaEMEnergyCalib = 0;
  T2CaIsoFrac = 0;
  TrigClusEta = 0;
  TrigClusPhi = 0;
  TrigClusEtaRef = 0;
  TrigClusPhiRef = 0;
  TrigClusEt = 0;
  TrigClusE = 0;
  TrigClusEtaPresh = 0;
  TrigClusPhiPresh = 0;
  TrigClusEPresh = 0;
  TrigClusEta1stS = 0;
  TrigClusPhi1stS = 0;
  TrigClusE1stS = 0;
  TrigClusEta2ndS = 0;
  TrigClusPhi2ndS = 0;
  TrigClusE2ndS = 0;
  TrigClusEta3rdS = 0;
  TrigClusPhi3rdS = 0;
  TrigClusE3rdS = 0;
  TrigClusMaker = 0;
  EFTauEnergy = 0;
  EFTauNRoI = 0;
  EFTauEMenergy = 0;
  EFTauET = 0;
  EFTauEta = 0;
  EFTauPhi = 0;
  EFTauCaloEta = 0;
  EFTauCaloPhi = 0;
  EFTauNStrip = 0;
  EFTauNEMCell = 0;
  EFTauStripET = 0;
  EFTauStripWidth2 = 0;
  EFTauEMRadius = 0;
  EFTauIsoFrac = 0;
  EFTauNtrack = 0;
  EFTauCharge = 0;
  EFTauPtTrack1 = 0;
  EFTauEtaTrack1 = 0;
  EFTauPhiTrack1 = 0;
  EFTauDistTrack1 = 0;
  EFTauPtTrack2 = 0;
  EFTauEtaTrack2 = 0;
  EFTauPhiTrack2 = 0;
  EFTauDistTrack2 = 0;
  EFTauPtTrack3 = 0;
  EFTauEtaTrack3 = 0;
  EFTauPhiTrack3 = 0;
  EFTauDistTrack3 = 0;
  EFTauSumEMe = 0;
  EFTauSumEMpx = 0;
  EFTauSumEMpy = 0;
  EFTauSumEMpz = 0;
  EFTauEtEMCalib = 0;
  EFTauEtHadCalib = 0;
  T2CaJetE = 0;
  T2CaJeteta = 0;
  T2CaJetphi = 0;
  T2CaJetConeR = 0;
  T2CaJetroi = 0;
  TrigJetEt = 0;
  TrigJetEta = 0;
  TrigJetPhi = 0;
  TrigJetE = 0;
  TrigJetM = 0;
  TrigJetMaker = 0;
  T2IdRoiID = 0;
  T2IdAlgo = 0;
  T2IdPt = 0;
  T2IdPhi0 = 0;
  T2IdZ0 = 0;
  T2IdD0 = 0;
  T2IdPhic = 0;
  T2IdEta = 0;
  T2IdErrPt = 0;
  T2IdErrPhi0 = 0;
  T2IdErrEta = 0;
  T2IdErrD0 = 0;
  T2IdErrZ0 = 0;
  T2IdChi2 = 0;
  T2IdNDoF = 0;
  T2IdNSihits = 0;
  T2IdNTrthits = 0;
  T2IdHPatt = 0;
  T2IdNstraw = 0;
  T2IdNtime = 0;
  T2IdNtr = 0;
  T2IdLastPlane = 0;
  T2IdFirstPlane = 0;
  T2IdNkineHit = 0;
  T2IdKineRef = 0;
  T2IdKineEnt = 0;
  T2IdNkineHitTRT = 0;
  T2IdKineRefTRT = 0;
  T2IdKineEntTRT = 0;
  T2zVertex = 0;
  TrkEF_RoIId = 0;
  TrkEF_d0 = 0;
  TrkEF_z0 = 0;
  TrkEF_phi = 0;
  TrkEF_eta = 0;
  TrkEF_qOverP = 0;
  TrkEF_Chi2 = 0;
  TrkEF_Ndf = 0;
  TrkEF_sigd0 = 0;
  TrkEF_sigz0 = 0;
  TrkEF_sigpt = 0;
  TrkEF_sigphi = 0;
  TrkEF_sigeta = 0;
  TrkEF_sigqOverP = 0;
  TrkEF_covVert21 = 0;
  TrkEF_covVert31 = 0;
  TrkEF_covVert32 = 0;
  TrkEF_covVert41 = 0;
  TrkEF_covVert42 = 0;
  TrkEF_covVert43 = 0;
  TrkEF_covVert51 = 0;
  TrkEF_covVert52 = 0;
  TrkEF_covVert53 = 0;
  TrkEF_covVert54 = 0;
  TrkEF_px = 0;
  TrkEF_py = 0;
  TrkEF_pz = 0;
  TrkEF_pt = 0;
  TrkEF_numberOfBLayerHits = 0;
  TrkEF_numberOfPixelHits = 0;
  TrkEF_numberOfSCTHits = 0;
  TrkEF_numberOfTRTHits = 0;
  TrkEF_numberOfTRTHighThresholdHits = 0;
  TrkEF_numberOfBLayerSharedHits = 0;
  TrkEF_numberOfPixelSharedHits = 0;
  TrkEF_numberOfPixelHoles = 0;
  TrkEF_numberOfSCTSharedHits = 0;
  TrkEF_numberOfSCTHoles = 0;
  TrkEF_numberOfTRTOutliers = 0;
  TrkEF_numberOfTRTHighThresholdOutliers = 0;
  TrkEF_numberOfMdtHits = 0;
  TrkEF_numberOfTgcPhiHits = 0;
  TrkEF_numberOfRpcPhiHits = 0;
  TrkEF_numberOfCscPhiHits = 0;
  TrkEF_numberOfTgcEtaHits = 0;
  TrkEF_numberOfRpcEtaHits = 0;
  TrkEF_numberOfCscEtaHits = 0;
  TrkEF_numberOfGangedPixels = 0;
  TrkEF_numberOfOutliersOnTrack = 0;
  TrkEF_numberOfTrackSummaryTypes = 0;
  TrkEF_truthBarcode = 0;
  TrkEF_truthNt = 0;
  TrkEF_ParticlePdg = 0;
  TrkEF_ParentPdg = 0;
  TrkEF_NumKineHits = 0;
  VxEF_vxp_vtx_x = 0;
  VxEF_vxp_vtx_y = 0;
  VxEF_vxp_vtx_z = 0;
  VxEF_vxp_vtx_x_truth = 0;
  VxEF_vxp_vtx_y_truth = 0;
  VxEF_vxp_vtx_z_truth = 0;
  VxEF_vxp_vtx_x_res = 0;
  VxEF_vxp_vtx_y_res = 0;
  VxEF_vxp_vtx_z_res = 0;
  VxEF_vxp_sigvtx_x = 0;
  VxEF_vxp_sigvtx_y = 0;
  VxEF_vxp_sigvtx_z = 0;
  VxEF_vxp_vtxchi2 = 0;
  VxEF_vxp_vtxndf = 0;
  VxEF_vxp_pt = 0;
  VxEF_vxp_vtxchi2prob = 0;
  VxEF_vxp_numTracksPerVertex = 0;
  VxEF_vxp_chi2 = 0;
  VxEF_vxp_d0 = 0;
  VxEF_vxp_sigd0 = 0;
  VxEF_vxp_z0 = 0;
  VxEF_vxp_sigz0 = 0;
  VxEF_vxp_phi = 0;
  VxEF_vxp_sigphi = 0;
  VxEF_vxp_theta = 0;
  VxEF_vxp_sigtheta = 0;
  VxEF_vxp_qOverP = 0;
  VxEF_vxp_sigqOverP = 0;
  VxEF_vxp_d0g = 0;
  VxEF_vxp_sigd0g = 0;
  VxEF_vxp_z0g = 0;
  VxEF_vxp_sigz0g = 0;
  eg_e_EF = 0;
  eg_eta_EF = 0;
  eg_phi_EF = 0;
  eg_cl_et_EF = 0;
  eg_cl_eta_EF = 0;
  eg_cl_phi_EF = 0;
  eg_etap_EF = 0;
  eg_zvertex_EF = 0;
  eg_errz_EF = 0;
  eg_depth_EF = 0;
  eg_e0_EF = 0;
  eg_e1_EF = 0;
  eg_e2_EF = 0;
  eg_e3_EF = 0;
  eg_eta0_EF = 0;
  eg_eta1_EF = 0;
  eg_eta2_EF = 0;
  eg_eta3_EF = 0;
  eg_phi0_EF = 0;
  eg_phi1_EF = 0;
  eg_phi2_EF = 0;
  eg_phi3_EF = 0;
  eg_Etha1_EF = 0;
  eg_Etha_EF = 0;
  eg_Eha1_EF = 0;
  eg_F1_EF = 0;
  eg_F3_EF = 0;
  eg_E233_EF = 0;
  eg_E237_EF = 0;
  eg_E277_EF = 0;
  eg_Weta1_EF = 0;
  eg_Weta2_EF = 0;
  eg_E2ts1_EF = 0;
  eg_E2tsts1_EF = 0;
  eg_Widths1_EF = 0;
  eg_Widths2_EF = 0;
  eg_poscs1_EF = 0;
  eg_poscs2_EF = 0;
  eg_Barys1_EF = 0;
  eg_Wtots1_EF = 0;
  eg_Emins1_EF = 0;
  eg_Emaxs1_EF = 0;
  eg_Fracs1_EF = 0;
  eg_EtCone45_EF = 0;
  eg_EtCone20_EF = 0;
  eg_EtCone30_EF = 0;
  eg_EtCone40_EF = 0;
  eg_IsEM_EF = 0;
  eg_epiNN_EF = 0;
  eg_EMWeight_EF = 0;
  eg_PionWeight_EF = 0;
  eg_Hmatrix_EF = 0;
  eg_IsEMse_EF = 0;
  eg_epiNNse_EF = 0;
  eg_EMWeightse_EF = 0;
  eg_PionWeightse_EF = 0;
  eg_E011_EF = 0;
  eg_E033_EF = 0;
  eg_E131_EF = 0;
  eg_E1153_EF = 0;
  eg_E235_EF = 0;
  eg_E255_EF = 0;
  eg_E333_EF = 0;
  eg_E335_EF = 0;
  eg_E337_EF = 0;
  eg_E377_EF = 0;
  eg_trkmatchnt_EF = 0;
  eg_primary_EF = 0;
  eg_numtracks_EF = 0;
  eg_eoverp_EF = 0;
  eg_etacorr_EF = 0;
  eg_deta0_EF = 0;
  eg_dphi0_EF = 0;
  eg_deta1_EF = 0;
  eg_dphi1_EF = 0;
  eg_deta2_EF = 0;
  eg_dphi2_EF = 0;
  eg_deta3_EF = 0;
  eg_dphi3_EF = 0;
  eg_trkopt_EF = 0;
  eg_trkinvpTV_EF = 0;
  eg_trkcotThV_EF = 0;
  eg_trkphiV_EF = 0;
  egRoiId_EF = 0;
  egKey_EF = 0;
  T2Bphys_roiId = 0;
  T2Bphys_eta = 0;
  T2Bphys_phi = 0;
  T2Bphys_pType = 0;
  T2Bphys_mass = 0;
  T2Bphys_fitmass = 0;
  T2Bphys_fitchis = 0;
  T2Bphys_fitprob = 0;
  T2Bphys_secmass = 0;
  T2Bphys_secfitmass = 0;
  T2Bphys_secfitchis = 0;
  T2Bphys_secfitprob = 0;
  T2Bphys_pdgid_tr1 = 0;
  T2Bphys_pdgid_tr2 = 0;
  T2Bphys_pdgid_tr3 = 0;
  T2Bphys_NtEnt1 = 0;
  T2Bphys_NtEnt2 = 0;
  T2Bphys_NtEnt3 = 0;
  T2BjEta = 0;
  T2BjPhi0 = 0;
  T2BjX2d = 0;
  T2BjXd0 = 0;
  T2BjXz0 = 0;
  T2BjX3d = 0;
  T2BjXmvtx = 0;
  T2BjXevtx = 0;
  T2BjXnvtx = 0;
  T2BjPvtx = 0;
  EFBjEta = 0;
  EFBjPhi0 = 0;
  EFBjX2d = 0;
  EFBjXd0 = 0;
  EFBjXz0 = 0;
  EFBjX3d = 0;
  EFBjXmvtx = 0;
  EFBjXevtx = 0;
  EFBjXnvtx = 0;
  EFBjPvtx = 0;
  tau_L2eta = 0;
  tau_L2phi = 0;
  tau_L2pt = 0;
  tau_L2Zvtx = 0;
  tau_L2nMatchTracks = 0;
  tau_L2roiId = 0;
  mcPx = 0;
  mcPy = 0;
  mcPz = 0;
  mcEne = 0;
  mcEtIsol = 0;
  mcID = 0;
  mcStatus = 0;
  mcBarcode = 0;
  mcCharge = 0;
  mcNMother = 0;
  mcMotherNt = 0;
  mcNChild = 0;
  mcChildNt = 0;
  EleAuthor = 0;
  EleTrkNt = 0;
  EleEta = 0;
  ElePhi = 0;
  ElePt = 0;
  EleIsEM = 0;
  EleZ0 = 0;
  EleD0 = 0;
  EleNBHits = 0;
  EleNPixelHits = 0;
  EleNSCTHits = 0;
  EleNTRTHits = 0;
  EleNTRHits = 0;
  EleEOverP = 0;
  EleEtaBE2 = 0;
  EleEt37 = 0;
  EleE237 = 0;
  EleE277 = 0;
  EleEthad1 = 0;
  EleWeta1 = 0;
  EleWeta2 = 0;
  EleF1 = 0;
  EleE2tsts1 = 0;
  EleEmins1 = 0;
  EleWtots1 = 0;
  EleFracs1 = 0;
  EleSoftEtaCorrMag = 0;
  EleSoftF1Core = 0;
  EleSoftF3Core = 0;
  EleSoftPos7 = 0;
  EleSoftIso = 0;
  EleSoftWidths2 = 0;
  PhoAuthor = 0;
  PhoEta = 0;
  PhoPhi = 0;
  PhoPt = 0;
  PhoIsEM = 0;
  PhoEtaBE2 = 0;
  PhoEt37 = 0;
  PhoE237 = 0;
  PhoE277 = 0;
  PhoEthad1 = 0;
  PhoWeta1 = 0;
  PhoWeta2 = 0;
  PhoF1 = 0;
  PhoE2tsts1 = 0;
  PhoEmins1 = 0;
  PhoWtots1 = 0;
  PhoFracs1 = 0;
  MuonAuthor = 0;
  MuonCombTrkNt = 0;
  MuonInDetTrkNt = 0;
  MuonExtrNt = 0;
  MuonEta = 0;
  MuonPhi = 0;
  MuonPt = 0;
  MuonZ0 = 0;
  MuonD0 = 0;
  MuonCharge = 0;
  MuonNMDTHits = 0;
  MuonNRPCEtaHits = 0;
  MuonNRPCPhiHits = 0;
  MuonNCSCEtaHits = 0;
  MuonNCSCPhiHits = 0;
  MuonNTGCEtaHits = 0;
  MuonNTGCPhiHits = 0;
  MuonLowPtNMDTHits = 0;
  MuonLowPtNCSCEtaHits = 0;
  MuonLowPtNCSCPhiHits = 0;
  MuonLowPtNRPCEtaHits = 0;
  MuonLowPtNRPCPhiHits = 0;
  MuonLowPtNTGCEtaHits = 0;
  MuonLowPtNTGCPhiHits = 0;
  TauJet_Author = 0;
  TauJet_numTrack = 0;
  TauJet_decayVxNt = 0;
  TauJet_clusterNt = 0;
  TauJet_eta = 0;
  TauJet_phi = 0;
  TauJet_pt = 0;
  TauJet_m = 0;
  TauJet_et = 0;
  TauJet_charge = 0;
  TauJet_emRadius = 0;
  TauJet_stripWidth2 = 0;
  TauJet_nStripCells = 0;
  TauJet_pEM_px = 0;
  TauJet_pEM_py = 0;
  TauJet_pEM_pz = 0;
  TauJet_pEM_e = 0;
  TauJet_etHadCalib = 0;
  TauJet_etEMCalib = 0;
  TauJet_isolationFraction = 0;
  TauJet_centralityFraction = 0;
  TauJet_logLikelihoodRatio = 0;
  TauJet_lowPtTauJetDiscriminant = 0;
  TauJet_lowPtTauEleDiscriminant = 0;
  TauJet_tauJetNeuralnetwork = 0;
  TauJet_tauENeuralNetwork = 0;
  TauJet_TrkNts = 0;
  TauJet1p3p_Author = 0;
  TauJet1p3p_numTrack = 0;
  TauJet1p3p_decayVxNt = 0;
  TauJet1p3p_clusterNt = 0;
  TauJet1p3p_eta = 0;
  TauJet1p3p_phi = 0;
  TauJet1p3p_pt = 0;
  TauJet1p3p_m = 0;
  TauJet1p3p_et = 0;
  TauJet1p3p_charge = 0;
  TauJet1p3p_emRadius = 0;
  TauJet1p3p_stripWidth2 = 0;
  TauJet1p3p_nStripCells = 0;
  TauJet1p3p_annularIsolationFraction = 0;
  TauJet1p3p_etCaloAtEMScale = 0;
  TauJet1p3p_etChargedHadCells = 0;
  TauJet1p3p_etOtherEMCells = 0;
  TauJet1p3p_etOtherHadCells = 0;
  TauJet1p3p_discriminant = 0;
  TauJet1p3p_discPDERS = 0;
  TauJet1p3p_discNN = 0;
  TauJet1p3p_nAssocTracksCore = 0;
  TauJet1p3p_nAssocTracksIsol = 0;
  TauJet1p3p_sumPTtrack = 0;
  TauJet1p3p_rWidth2Trk3P = 0;
  TauJet1p3p_massTrk3P = 0;
  TauJet1p3p_TrkNts = 0;
  MuROI_Eta = 0;
  MuROI_Phi = 0;
  MuROI_Thr = 0;
  EmROI_Eta = 0;
  EmROI_Phi = 0;
  EmROI_Thr = 0;
  JetROI_Eta = 0;
  JetROI_Phi = 0;
  JetROI_Thr = 0;
  JetEtROI_Word = 0;
  JetEtROI_Thr = 0;
  ESumROI_Ex = 0;
  ESumROI_Ey = 0;
  ESumROI_Et = 0;
  ESumROI_ThrS = 0;
  ESumROI_ThrM = 0;
  cl_et_topo = 0;
  cl_e_topo = 0;
  cl_eemb0_topo = 0;
  cl_eemb1_topo = 0;
  cl_eemb2_topo = 0;
  cl_eemb3_topo = 0;
  cl_nemb0_topo = 0;
  cl_nemb1_topo = 0;
  cl_nemb2_topo = 0;
  cl_nemb3_topo = 0;
  cl_phi2_topo = 0;
  cl_eta0_topo = 0;
  cl_eta1_topo = 0;
  cl_eta2_topo = 0;
  cl_eta3_topo = 0;
  cl_eeme0_topo = 0;
  cl_eeme1_topo = 0;
  cl_eeme2_topo = 0;
  cl_eeme3_topo = 0;
  cl_neme0_topo = 0;
  cl_neme1_topo = 0;
  cl_neme2_topo = 0;
  cl_neme3_topo = 0;
  cl_etileg1_topo = 0;
  cl_etileg2_topo = 0;
  cl_etileg3_topo = 0;
  cl_ntileg1_topo = 0;
  cl_ntileg2_topo = 0;
  cl_ntileg3_topo = 0;
  cl_eta_topo = 0;
  cl_phi_topo = 0;
  cl_time_topo = 0;
  cl_reco_stat_topo = 0;
  cl_m1_eta_topo = 0;
  cl_m1_phi_topo = 0;
  cl_m2_r_topo = 0;
  cl_m2_lambda_topo = 0;
  cl_delta_phi_topo = 0;
  cl_delta_theta_topo = 0;
  cl_delta_alpha_topo = 0;
  cl_center_x_topo = 0;
  cl_center_y_topo = 0;
  cl_center_z_topo = 0;
  cl_center_lambda_topo = 0;
  cl_lateral_topo = 0;
  cl_longitudinal_topo = 0;
  cl_eng_frac_em_topo = 0;
  cl_eng_frac_max_topo = 0;
  cl_eng_frac_core_topo = 0;
  cl_m1_dens_topo = 0;
  cl_m2_dens_topo = 0;
  cl_ehec0_topo = 0;
  cl_ehec1_topo = 0;
  cl_ehec2_topo = 0;
  cl_ehec3_topo = 0;
  cl_nhec0_topo = 0;
  cl_nhec1_topo = 0;
  cl_nhec2_topo = 0;
  cl_nhec3_topo = 0;
  cl_etileb0_topo = 0;
  cl_etileb1_topo = 0;
  cl_etileb2_topo = 0;
  cl_ntileb0_topo = 0;
  cl_ntileb1_topo = 0;
  cl_ntileb2_topo = 0;
  cl_etilee0_topo = 0;
  cl_etilee1_topo = 0;
  cl_etilee2_topo = 0;
  cl_efcal0_topo = 0;
  cl_efcal1_topo = 0;
  cl_efcal2_topo = 0;
  cl_ntilee0_topo = 0;
  cl_ntilee1_topo = 0;
  cl_ntilee2_topo = 0;
  cl_nfcal0_topo = 0;
  cl_nfcal1_topo = 0;
  cl_nfcal2_topo = 0;
  cl_ntotcells_topo = 0;
  pjAuthor = 0;
  pjPx = 0;
  pjPy = 0;
  pjPz = 0;
  pjEne = 0;
  pjEta = 0;
  pjPhi = 0;
  pjPt = 0;
  pjNtrk = 0;
  pjNmuo = 0;
  pjNele = 0;
  pjWeight = 0;
  pjW2D = 0;
  pjW3D = 0;
  pjWSV2 = 0;
  pjWlhSig = 0;
  pjLabel = 0;
  pjdRb = 0;
  pjdRc = 0;
  pjdRt = 0;
  // Set branch addresses and branch pointers
  if (!tree) return;
  fChain = tree;
  fCurrent = -1;
  fChain->SetMakeClass(1);

  fChain->SetBranchStatus("*",0);
  fChain->SetBranchStatus("RunNumber",1);
  fChain->SetBranchStatus("EventNumber",1);
  fChain->SetBranchStatus("L1Jet_nRoI*",1);
  fChain->SetBranchStatus("L1Jet_ET8x8*",1);
  fChain->SetBranchStatus("L1Jet_eta*",1);
  fChain->SetBranchStatus("L1Jet_phi*",1);
  fChain->SetBranchStatus("jetNumC4Jets*",1);
  fChain->SetBranchStatus("jetEtaC4Jets*",1);
  fChain->SetBranchStatus("jetPhiC4Jets*",1);
  fChain->SetBranchStatus("jetPxC4Jets*",1);
  fChain->SetBranchStatus("jetPyC4Jets*",1);
  fChain->SetBranchStatus("jetEtC4Jets*",1);
  fChain->SetBranchStatus("T2IdNtracks*",1);
  fChain->SetBranchStatus("T2IdAlgo*",1);
  fChain->SetBranchStatus("T2IdPt*",1);
  fChain->SetBranchStatus("T2IdPhi0*",1);
  fChain->SetBranchStatus("T2IdZ0*",1);
  fChain->SetBranchStatus("T2IdD0*",1);
  fChain->SetBranchStatus("T2IdEta*",1);
  fChain->SetBranchStatus("T2IdErrD0*",1);
  fChain->SetBranchStatus("T2IdErrZ0*",1);
  fChain->SetBranchStatus("T2IdChi2*",1);
  fChain->SetBranchStatus("T2IdNDoF*",1);
  fChain->SetBranchStatus("T2IdNSihits*",1);
  fChain->SetBranchStatus("T2IdNTrthits*",1);
  fChain->SetBranchStatus("NPar*",1);
  fChain->SetBranchStatus("KMothNt*",1);
  fChain->SetBranchStatus("Type*",1);
  fChain->SetBranchStatus("PtGen*",1);
  fChain->SetBranchStatus("EtaGen*",1);
  fChain->SetBranchStatus("PhiGen*",1);
  fChain->SetBranchStatus("MGen*",1);
  fChain->SetBranchStatus("Trk_totalNumTracks",1);
  fChain->SetBranchStatus("Trk_d0",1);
  fChain->SetBranchStatus("Trk_z0",1);
  fChain->SetBranchStatus("Trk_px",1);
  fChain->SetBranchStatus("Trk_py",1);
  fChain->SetBranchStatus("Trk_pz",1);
  fChain->SetBranchStatus("Trk_pt",1);
  fChain->SetBranchStatus("Trk_phi",1);
  fChain->SetBranchStatus("Trk_qOverP",1);
  fChain->SetBranchStatus("Trk_theta",1);
  fChain->SetBranchStatus("Trk_sigd0",1);
  fChain->SetBranchStatus("Trk_sigz0",1);
  fChain->SetBranchStatus("Trk_Chi2",1);
  fChain->SetBranchStatus("Trk_Ndf",1);
  fChain->SetBranchStatus("Trk_numberOfPixelHits",1);
  fChain->SetBranchStatus("Trk_numberOfSCTHits",1);
  fChain->SetBranchStatus("Trk_numberOfTRTHits",1);
  fChain->SetBranchStatus("Trk_numberOfPixelHoles",1);
  fChain->SetBranchStatus("Trk_numberOfSCTHoles",1);
  fChain->SetBranchStatus("Trk_truthBarcode",1);
  fChain->SetBranchStatus("Trk_truthNt",1);
  fChain->SetBranchStatus("Trk_ParticlePdg",1);
  fChain->SetBranchStatus("Trk_ParentPdg",1);

  fChain->SetBranchStatus("T2BjN",1);
  fChain->SetBranchStatus("T2BjEta",1);
  fChain->SetBranchStatus("T2BjPhi0",1);
  fChain->SetBranchStatus("T2BjXd0",1);
  fChain->SetBranchStatus("T2BjXz0",1);
  fChain->SetBranchStatus("T2BjX2d",1);

  fChain->SetBranchStatus("EFBjN",1);
  fChain->SetBranchStatus("EFBjEta",1);
  fChain->SetBranchStatus("EFBjPhi0",1);
  fChain->SetBranchStatus("EFBjXd0",1);
  fChain->SetBranchStatus("EFBjXz0",1);
  fChain->SetBranchStatus("EFBjX2d",1);


  if( fChain->GetBranchStatus("RunNumber") ) { fChain->SetBranchAddress("RunNumber", &RunNumber, &b_RunNumber); }
  if( fChain->GetBranchStatus("EventNumber") ) { fChain->SetBranchAddress("EventNumber", &EventNumber, &b_EventNumber); }
  if( fChain->GetBranchStatus("StreamESD_ref") ) { fChain->SetBranchAddress("StreamESD_ref", StreamESD_ref, &b_StreamESD_ref); }
  if( fChain->GetBranchStatus("StreamRDO_ref") ) { fChain->SetBranchAddress("StreamRDO_ref", &StreamRDO_ref, &b_StreamRDO_ref); }
  if( fChain->GetBranchStatus("Token") ) { fChain->SetBranchAddress("Token", Token, &b_Token); }
  if( fChain->GetBranchStatus("Run") ) { fChain->SetBranchAddress("Run", &Run, &b_Run); }
  if( fChain->GetBranchStatus("Event") ) { fChain->SetBranchAddress("Event", &Event, &b_Event); }
  if( fChain->GetBranchStatus("Time") ) { fChain->SetBranchAddress("Time", &Time, &b_Time); }
  if( fChain->GetBranchStatus("Weight") ) { fChain->SetBranchAddress("Weight", &Weight, &b_Weight); }
  if( fChain->GetBranchStatus("IEvent") ) { fChain->SetBranchAddress("IEvent", &IEvent, &b_IEvent); }
  if( fChain->GetBranchStatus("NPar") ) { fChain->SetBranchAddress("NPar", &NPar, &b_NPar); }
  if( fChain->GetBranchStatus("Type") ) { fChain->SetBranchAddress("Type", &Type, &b_Type); }
  if( fChain->GetBranchStatus("PtGen") ) { fChain->SetBranchAddress("PtGen", &PtGen, &b_PtGen); }
  if( fChain->GetBranchStatus("PhiGen") ) { fChain->SetBranchAddress("PhiGen", &PhiGen, &b_PhiGen); }
  if( fChain->GetBranchStatus("EtaGen") ) { fChain->SetBranchAddress("EtaGen", &EtaGen, &b_EtaGen); }
  if( fChain->GetBranchStatus("MGen") ) { fChain->SetBranchAddress("MGen", &MGen, &b_MGen); }
  if( fChain->GetBranchStatus("Charge") ) { fChain->SetBranchAddress("Charge", &Charge, &b_Charge); }
  if( fChain->GetBranchStatus("GenStat") ) { fChain->SetBranchAddress("GenStat", &GenStat, &b_GenStat); }
  if( fChain->GetBranchStatus("GenRef") ) { fChain->SetBranchAddress("GenRef", &GenRef, &b_GenRef); }
  if( fChain->GetBranchStatus("KMothNt") ) { fChain->SetBranchAddress("KMothNt", &KMothNt, &b_KMothNt); }
  if( fChain->GetBranchStatus("KFDauNt") ) { fChain->SetBranchAddress("KFDauNt", &KFDauNt, &b_KFDauNt); }
  if( fChain->GetBranchStatus("KLDauNt") ) { fChain->SetBranchAddress("KLDauNt", &KLDauNt, &b_KLDauNt); }
  if( fChain->GetBranchStatus("KOriVNt") ) { fChain->SetBranchAddress("KOriVNt", &KOriVNt, &b_KOriVNt); }
  if( fChain->GetBranchStatus("KEndVNt") ) { fChain->SetBranchAddress("KEndVNt", &KEndVNt, &b_KEndVNt); }
  if( fChain->GetBranchStatus("BunchNum") ) { fChain->SetBranchAddress("BunchNum", &BunchNum, &b_BunchNum); }
  if( fChain->GetBranchStatus("RVGen") ) { fChain->SetBranchAddress("RVGen", &RVGen, &b_RVGen); }
  if( fChain->GetBranchStatus("PhiVGen") ) { fChain->SetBranchAddress("PhiVGen", &PhiVGen, &b_PhiVGen); }
  if( fChain->GetBranchStatus("ZVGen") ) { fChain->SetBranchAddress("ZVGen", &ZVGen, &b_ZVGen); }
  if( fChain->GetBranchStatus("NVer") ) { fChain->SetBranchAddress("NVer", &NVer, &b_NVer); }
  if( fChain->GetBranchStatus("BunchV") ) { fChain->SetBranchAddress("BunchV", &BunchV, &b_BunchV); }
  if( fChain->GetBranchStatus("RV") ) { fChain->SetBranchAddress("RV", &RV, &b_RV); }
  if( fChain->GetBranchStatus("PhiV") ) { fChain->SetBranchAddress("PhiV", &PhiV, &b_PhiV); }
  if( fChain->GetBranchStatus("ZV") ) { fChain->SetBranchAddress("ZV", &ZV, &b_ZV); }
  if( fChain->GetBranchStatus("GenRefV") ) { fChain->SetBranchAddress("GenRefV", &GenRefV, &b_GenRefV); }
  if( fChain->GetBranchStatus("KVMothNt") ) { fChain->SetBranchAddress("KVMothNt", &KVMothNt, &b_KVMothNt); }
  if( fChain->GetBranchStatus("KVFDauNt") ) { fChain->SetBranchAddress("KVFDauNt", &KVFDauNt, &b_KVFDauNt); }
  if( fChain->GetBranchStatus("KVLDauNt") ) { fChain->SetBranchAddress("KVLDauNt", &KVLDauNt, &b_KVLDauNt); }
  if( fChain->GetBranchStatus("IVPrimary") ) { fChain->SetBranchAddress("IVPrimary", &IVPrimary, &b_IVPrimary); }
  if( fChain->GetBranchStatus("Spcl_Num") ) { fChain->SetBranchAddress("Spcl_Num", &Spcl_Num, &b_Spcl_Num); }
  if( fChain->GetBranchStatus("Spcl_ID") ) { fChain->SetBranchAddress("Spcl_ID", &Spcl_ID, &b_Spcl_ID); }
  if( fChain->GetBranchStatus("Spcl_Pt") ) { fChain->SetBranchAddress("Spcl_Pt", &Spcl_Pt, &b_Spcl_Pt); }
  if( fChain->GetBranchStatus("Spcl_Eta") ) { fChain->SetBranchAddress("Spcl_Eta", &Spcl_Eta, &b_Spcl_Eta); }
  if( fChain->GetBranchStatus("Spcl_Phi") ) { fChain->SetBranchAddress("Spcl_Phi", &Spcl_Phi, &b_Spcl_Phi); }
  if( fChain->GetBranchStatus("Spcl_M") ) { fChain->SetBranchAddress("Spcl_M", &Spcl_M, &b_Spcl_M); }
  if( fChain->GetBranchStatus("Spcl_Stat") ) { fChain->SetBranchAddress("Spcl_Stat", &Spcl_Stat, &b_Spcl_Stat); }
  if( fChain->GetBranchStatus("Spcl_Isol") ) { fChain->SetBranchAddress("Spcl_Isol", &Spcl_Isol, &b_Spcl_Isol); }
  if( fChain->GetBranchStatus("Spcl_sumx") ) { fChain->SetBranchAddress("Spcl_sumx", &Spcl_sumx, &b_Spcl_sumx); }
  if( fChain->GetBranchStatus("Spcl_sumy") ) { fChain->SetBranchAddress("Spcl_sumy", &Spcl_sumy, &b_Spcl_sumy); }
  if( fChain->GetBranchStatus("Spcl_sumz") ) { fChain->SetBranchAddress("Spcl_sumz", &Spcl_sumz, &b_Spcl_sumz); }
  if( fChain->GetBranchStatus("Spcl_sume") ) { fChain->SetBranchAddress("Spcl_sume", &Spcl_sume, &b_Spcl_sume); }
  if( fChain->GetBranchStatus("Spcl_x1") ) { fChain->SetBranchAddress("Spcl_x1", &Spcl_x1, &b_Spcl_x1); }
  if( fChain->GetBranchStatus("Spcl_x2") ) { fChain->SetBranchAddress("Spcl_x2", &Spcl_x2, &b_Spcl_x2); }
  if( fChain->GetBranchStatus("Spcl_id1") ) { fChain->SetBranchAddress("Spcl_id1", &Spcl_id1, &b_Spcl_id1); }
  if( fChain->GetBranchStatus("Spcl_id2") ) { fChain->SetBranchAddress("Spcl_id2", &Spcl_id2, &b_Spcl_id2); }
  if( fChain->GetBranchStatus("MuonEntRec_Num") ) { fChain->SetBranchAddress("MuonEntRec_Num", &MuonEntRec_Num, &b_MuonEntRec_Num); }
  if( fChain->GetBranchStatus("MuonEntRec_ID") ) { fChain->SetBranchAddress("MuonEntRec_ID", &MuonEntRec_ID, &b_MuonEntRec_ID); }
  if( fChain->GetBranchStatus("MuonEntRec_Ene") ) { fChain->SetBranchAddress("MuonEntRec_Ene", &MuonEntRec_Ene, &b_MuonEntRec_Ene); }
  if( fChain->GetBranchStatus("MuonEntRec_Eta") ) { fChain->SetBranchAddress("MuonEntRec_Eta", &MuonEntRec_Eta, &b_MuonEntRec_Eta); }
  if( fChain->GetBranchStatus("MuonEntRec_Phi") ) { fChain->SetBranchAddress("MuonEntRec_Phi", &MuonEntRec_Phi, &b_MuonEntRec_Phi); }
  if( fChain->GetBranchStatus("MuonEntRec_PosX") ) { fChain->SetBranchAddress("MuonEntRec_PosX", &MuonEntRec_PosX, &b_MuonEntRec_PosX); }
  if( fChain->GetBranchStatus("MuonEntRec_PosY") ) { fChain->SetBranchAddress("MuonEntRec_PosY", &MuonEntRec_PosY, &b_MuonEntRec_PosY); }
  if( fChain->GetBranchStatus("MuonEntRec_PosZ") ) { fChain->SetBranchAddress("MuonEntRec_PosZ", &MuonEntRec_PosZ, &b_MuonEntRec_PosZ); }
  if( fChain->GetBranchStatus("MuonEntRec_Time") ) { fChain->SetBranchAddress("MuonEntRec_Time", &MuonEntRec_Time, &b_MuonEntRec_Time); }
  if( fChain->GetBranchStatus("MuonEntRec_BarCode") ) { fChain->SetBranchAddress("MuonEntRec_BarCode", &MuonEntRec_BarCode, &b_MuonEntRec_BarCode); }
  if( fChain->GetBranchStatus("vxp_numVertices") ) { fChain->SetBranchAddress("vxp_numVertices", &vxp_numVertices, &b_vxp_numVertices); }
  if( fChain->GetBranchStatus("vxp_vtx_x") ) { fChain->SetBranchAddress("vxp_vtx_x", &vxp_vtx_x, &b_vxp_vtx_x); }
  if( fChain->GetBranchStatus("vxp_vtx_y") ) { fChain->SetBranchAddress("vxp_vtx_y", &vxp_vtx_y, &b_vxp_vtx_y); }
  if( fChain->GetBranchStatus("vxp_vtx_z") ) { fChain->SetBranchAddress("vxp_vtx_z", &vxp_vtx_z, &b_vxp_vtx_z); }
  if( fChain->GetBranchStatus("vxp_numOfTruthVertices") ) { fChain->SetBranchAddress("vxp_numOfTruthVertices", &vxp_numOfTruthVertices, &b_vxp_numOfTruthVertices); }
  if( fChain->GetBranchStatus("vxp_vtx_x_truth") ) { fChain->SetBranchAddress("vxp_vtx_x_truth", &vxp_vtx_x_truth, &b_vxp_vtx_x_truth); }
  if( fChain->GetBranchStatus("vxp_vtx_y_truth") ) { fChain->SetBranchAddress("vxp_vtx_y_truth", &vxp_vtx_y_truth, &b_vxp_vtx_y_truth); }
  if( fChain->GetBranchStatus("vxp_vtx_z_truth") ) { fChain->SetBranchAddress("vxp_vtx_z_truth", &vxp_vtx_z_truth, &b_vxp_vtx_z_truth); }
  if( fChain->GetBranchStatus("vxp_vtx_x_res") ) { fChain->SetBranchAddress("vxp_vtx_x_res", &vxp_vtx_x_res, &b_vxp_vtx_x_res); }
  if( fChain->GetBranchStatus("vxp_vtx_y_res") ) { fChain->SetBranchAddress("vxp_vtx_y_res", &vxp_vtx_y_res, &b_vxp_vtx_y_res); }
  if( fChain->GetBranchStatus("vxp_vtx_z_res") ) { fChain->SetBranchAddress("vxp_vtx_z_res", &vxp_vtx_z_res, &b_vxp_vtx_z_res); }
  if( fChain->GetBranchStatus("vxp_sigvtx_x") ) { fChain->SetBranchAddress("vxp_sigvtx_x", &vxp_sigvtx_x, &b_vxp_sigvtx_x); }
  if( fChain->GetBranchStatus("vxp_sigvtx_y") ) { fChain->SetBranchAddress("vxp_sigvtx_y", &vxp_sigvtx_y, &b_vxp_sigvtx_y); }
  if( fChain->GetBranchStatus("vxp_sigvtx_z") ) { fChain->SetBranchAddress("vxp_sigvtx_z", &vxp_sigvtx_z, &b_vxp_sigvtx_z); }
  if( fChain->GetBranchStatus("vxp_vtxchi2") ) { fChain->SetBranchAddress("vxp_vtxchi2", &vxp_vtxchi2, &b_vxp_vtxchi2); }
  if( fChain->GetBranchStatus("vxp_vtxndf") ) { fChain->SetBranchAddress("vxp_vtxndf", &vxp_vtxndf, &b_vxp_vtxndf); }
  if( fChain->GetBranchStatus("vxp_pt") ) { fChain->SetBranchAddress("vxp_pt", &vxp_pt, &b_vxp_pt); }
  if( fChain->GetBranchStatus("vxp_vtx_constraint_x") ) { fChain->SetBranchAddress("vxp_vtx_constraint_x", &vxp_vtx_constraint_x, &b_vxp_vtx_constraint_x); }
  if( fChain->GetBranchStatus("vxp_vtx_constraint_y") ) { fChain->SetBranchAddress("vxp_vtx_constraint_y", &vxp_vtx_constraint_y, &b_vxp_vtx_constraint_y); }
  if( fChain->GetBranchStatus("vxp_vtx_constraint_z") ) { fChain->SetBranchAddress("vxp_vtx_constraint_z", &vxp_vtx_constraint_z, &b_vxp_vtx_constraint_z); }
  if( fChain->GetBranchStatus("vxp_sigvtx_constraint_x") ) { fChain->SetBranchAddress("vxp_sigvtx_constraint_x", &vxp_sigvtx_constraint_x, &b_vxp_sigvtx_constraint_x); }
  if( fChain->GetBranchStatus("vxp_sigvtx_constraint_y") ) { fChain->SetBranchAddress("vxp_sigvtx_constraint_y", &vxp_sigvtx_constraint_y, &b_vxp_sigvtx_constraint_y); }
  if( fChain->GetBranchStatus("vxp_sigvtx_constraint_z") ) { fChain->SetBranchAddress("vxp_sigvtx_constraint_z", &vxp_sigvtx_constraint_z, &b_vxp_sigvtx_constraint_z); }
  if( fChain->GetBranchStatus("vxp_vtx_linearization_x") ) { fChain->SetBranchAddress("vxp_vtx_linearization_x", &vxp_vtx_linearization_x, &b_vxp_vtx_linearization_x); }
  if( fChain->GetBranchStatus("vxp_vtx_linearization_y") ) { fChain->SetBranchAddress("vxp_vtx_linearization_y", &vxp_vtx_linearization_y, &b_vxp_vtx_linearization_y); }
  if( fChain->GetBranchStatus("vxp_vtx_linearization_z") ) { fChain->SetBranchAddress("vxp_vtx_linearization_z", &vxp_vtx_linearization_z, &b_vxp_vtx_linearization_z); }
  if( fChain->GetBranchStatus("vxp_vtx_seed_x") ) { fChain->SetBranchAddress("vxp_vtx_seed_x", &vxp_vtx_seed_x, &b_vxp_vtx_seed_x); }
  if( fChain->GetBranchStatus("vxp_vtx_seed_y") ) { fChain->SetBranchAddress("vxp_vtx_seed_y", &vxp_vtx_seed_y, &b_vxp_vtx_seed_y); }
  if( fChain->GetBranchStatus("vxp_vtx_seed_z") ) { fChain->SetBranchAddress("vxp_vtx_seed_z", &vxp_vtx_seed_z, &b_vxp_vtx_seed_z); }
  if( fChain->GetBranchStatus("vxp_vtxchi2prob") ) { fChain->SetBranchAddress("vxp_vtxchi2prob", &vxp_vtxchi2prob, &b_vxp_vtxchi2prob); }
  if( fChain->GetBranchStatus("vxp_numTracksPerVertex") ) { fChain->SetBranchAddress("vxp_numTracksPerVertex", &vxp_numTracksPerVertex, &b_vxp_numTracksPerVertex); }
  if( fChain->GetBranchStatus("vxp_totalNumTracks") ) { fChain->SetBranchAddress("vxp_totalNumTracks", &vxp_totalNumTracks, &b_vxp_totalNumTracks); }
  if( fChain->GetBranchStatus("vxp_chi2") ) { fChain->SetBranchAddress("vxp_chi2", &vxp_chi2, &b_vxp_chi2); }
  if( fChain->GetBranchStatus("vxp_ndf") ) { fChain->SetBranchAddress("vxp_ndf", &vxp_ndf, &b_vxp_ndf); }
  if( fChain->GetBranchStatus("vxp_vertexCompatibility") ) { fChain->SetBranchAddress("vxp_vertexCompatibility", &vxp_vertexCompatibility, &b_vxp_vertexCompatibility); }
  if( fChain->GetBranchStatus("vxp_d0") ) { fChain->SetBranchAddress("vxp_d0", &vxp_d0, &b_vxp_d0); }
  if( fChain->GetBranchStatus("vxp_sigd0") ) { fChain->SetBranchAddress("vxp_sigd0", &vxp_sigd0, &b_vxp_sigd0); }
  if( fChain->GetBranchStatus("vxp_z0") ) { fChain->SetBranchAddress("vxp_z0", &vxp_z0, &b_vxp_z0); }
  if( fChain->GetBranchStatus("vxp_sigz0") ) { fChain->SetBranchAddress("vxp_sigz0", &vxp_sigz0, &b_vxp_sigz0); }
  if( fChain->GetBranchStatus("vxp_phi") ) { fChain->SetBranchAddress("vxp_phi", &vxp_phi, &b_vxp_phi); }
  if( fChain->GetBranchStatus("vxp_sigphi") ) { fChain->SetBranchAddress("vxp_sigphi", &vxp_sigphi, &b_vxp_sigphi); }
  if( fChain->GetBranchStatus("vxp_theta") ) { fChain->SetBranchAddress("vxp_theta", &vxp_theta, &b_vxp_theta); }
  if( fChain->GetBranchStatus("vxp_sigtheta") ) { fChain->SetBranchAddress("vxp_sigtheta", &vxp_sigtheta, &b_vxp_sigtheta); }
  if( fChain->GetBranchStatus("vxp_qOverP") ) { fChain->SetBranchAddress("vxp_qOverP", &vxp_qOverP, &b_vxp_qOverP); }
  if( fChain->GetBranchStatus("vxp_sigqOverP") ) { fChain->SetBranchAddress("vxp_sigqOverP", &vxp_sigqOverP, &b_vxp_sigqOverP); }
  if( fChain->GetBranchStatus("vxp_d0g") ) { fChain->SetBranchAddress("vxp_d0g", &vxp_d0g, &b_vxp_d0g); }
  if( fChain->GetBranchStatus("vxp_sigd0g") ) { fChain->SetBranchAddress("vxp_sigd0g", &vxp_sigd0g, &b_vxp_sigd0g); }
  if( fChain->GetBranchStatus("vxp_z0g") ) { fChain->SetBranchAddress("vxp_z0g", &vxp_z0g, &b_vxp_z0g); }
  if( fChain->GetBranchStatus("vxp_sigz0g") ) { fChain->SetBranchAddress("vxp_sigz0g", &vxp_sigz0g, &b_vxp_sigz0g); }
  if( fChain->GetBranchStatus("vxp_refitted_d0") ) { fChain->SetBranchAddress("vxp_refitted_d0", &vxp_refitted_d0, &b_vxp_refitted_d0); }
  if( fChain->GetBranchStatus("vxp_refitted_sigd0") ) { fChain->SetBranchAddress("vxp_refitted_sigd0", &vxp_refitted_sigd0, &b_vxp_refitted_sigd0); }
  if( fChain->GetBranchStatus("vxp_refitted_z0") ) { fChain->SetBranchAddress("vxp_refitted_z0", &vxp_refitted_z0, &b_vxp_refitted_z0); }
  if( fChain->GetBranchStatus("vxp_refitted_sigz0") ) { fChain->SetBranchAddress("vxp_refitted_sigz0", &vxp_refitted_sigz0, &b_vxp_refitted_sigz0); }
  if( fChain->GetBranchStatus("vxp_refitted_phi") ) { fChain->SetBranchAddress("vxp_refitted_phi", &vxp_refitted_phi, &b_vxp_refitted_phi); }
  if( fChain->GetBranchStatus("vxp_refitted_sigphi") ) { fChain->SetBranchAddress("vxp_refitted_sigphi", &vxp_refitted_sigphi, &b_vxp_refitted_sigphi); }
  if( fChain->GetBranchStatus("vxp_refitted_theta") ) { fChain->SetBranchAddress("vxp_refitted_theta", &vxp_refitted_theta, &b_vxp_refitted_theta); }
  if( fChain->GetBranchStatus("vxp_refitted_sigtheta") ) { fChain->SetBranchAddress("vxp_refitted_sigtheta", &vxp_refitted_sigtheta, &b_vxp_refitted_sigtheta); }
  if( fChain->GetBranchStatus("vxp_refitted_qOverP") ) { fChain->SetBranchAddress("vxp_refitted_qOverP", &vxp_refitted_qOverP, &b_vxp_refitted_qOverP); }
  if( fChain->GetBranchStatus("vxp_refitted_sigqOverP") ) { fChain->SetBranchAddress("vxp_refitted_sigqOverP", &vxp_refitted_sigqOverP, &b_vxp_refitted_sigqOverP); }
  if( fChain->GetBranchStatus("Trk_totalNumTracks") ) { fChain->SetBranchAddress("Trk_totalNumTracks", &Trk_totalNumTracks, &b_Trk_totalNumTracks); }
  if( fChain->GetBranchStatus("Trk_d0") ) { fChain->SetBranchAddress("Trk_d0", &Trk_d0, &b_Trk_d0); }
  if( fChain->GetBranchStatus("Trk_z0") ) { fChain->SetBranchAddress("Trk_z0", &Trk_z0, &b_Trk_z0); }
  if( fChain->GetBranchStatus("Trk_phi") ) { fChain->SetBranchAddress("Trk_phi", &Trk_phi, &b_Trk_phi); }
  if( fChain->GetBranchStatus("Trk_theta") ) { fChain->SetBranchAddress("Trk_theta", &Trk_theta, &b_Trk_theta); }
  if( fChain->GetBranchStatus("Trk_qOverP") ) { fChain->SetBranchAddress("Trk_qOverP", &Trk_qOverP, &b_Trk_qOverP); }
  if( fChain->GetBranchStatus("Trk_Chi2") ) { fChain->SetBranchAddress("Trk_Chi2", &Trk_Chi2, &b_Trk_Chi2); }
  if( fChain->GetBranchStatus("Trk_Ndf") ) { fChain->SetBranchAddress("Trk_Ndf", &Trk_Ndf, &b_Trk_Ndf); }
  if( fChain->GetBranchStatus("Trk_sigd0") ) { fChain->SetBranchAddress("Trk_sigd0", &Trk_sigd0, &b_Trk_sigd0); }
  if( fChain->GetBranchStatus("Trk_sigz0") ) { fChain->SetBranchAddress("Trk_sigz0", &Trk_sigz0, &b_Trk_sigz0); }
  if( fChain->GetBranchStatus("Trk_sigphi") ) { fChain->SetBranchAddress("Trk_sigphi", &Trk_sigphi, &b_Trk_sigphi); }
  if( fChain->GetBranchStatus("Trk_sigtheta") ) { fChain->SetBranchAddress("Trk_sigtheta", &Trk_sigtheta, &b_Trk_sigtheta); }
  if( fChain->GetBranchStatus("Trk_sigqOverP") ) { fChain->SetBranchAddress("Trk_sigqOverP", &Trk_sigqOverP, &b_Trk_sigqOverP); }
  if( fChain->GetBranchStatus("Trk_covVert21") ) { fChain->SetBranchAddress("Trk_covVert21", &Trk_covVert21, &b_Trk_covVert21); }
  if( fChain->GetBranchStatus("Trk_covVert31") ) { fChain->SetBranchAddress("Trk_covVert31", &Trk_covVert31, &b_Trk_covVert31); }
  if( fChain->GetBranchStatus("Trk_covVert32") ) { fChain->SetBranchAddress("Trk_covVert32", &Trk_covVert32, &b_Trk_covVert32); }
  if( fChain->GetBranchStatus("Trk_covVert41") ) { fChain->SetBranchAddress("Trk_covVert41", &Trk_covVert41, &b_Trk_covVert41); }
  if( fChain->GetBranchStatus("Trk_covVert42") ) { fChain->SetBranchAddress("Trk_covVert42", &Trk_covVert42, &b_Trk_covVert42); }
  if( fChain->GetBranchStatus("Trk_covVert43") ) { fChain->SetBranchAddress("Trk_covVert43", &Trk_covVert43, &b_Trk_covVert43); }
  if( fChain->GetBranchStatus("Trk_covVert51") ) { fChain->SetBranchAddress("Trk_covVert51", &Trk_covVert51, &b_Trk_covVert51); }
  if( fChain->GetBranchStatus("Trk_covVert52") ) { fChain->SetBranchAddress("Trk_covVert52", &Trk_covVert52, &b_Trk_covVert52); }
  if( fChain->GetBranchStatus("Trk_covVert53") ) { fChain->SetBranchAddress("Trk_covVert53", &Trk_covVert53, &b_Trk_covVert53); }
  if( fChain->GetBranchStatus("Trk_covVert54") ) { fChain->SetBranchAddress("Trk_covVert54", &Trk_covVert54, &b_Trk_covVert54); }
  if( fChain->GetBranchStatus("Trk_px") ) { fChain->SetBranchAddress("Trk_px", &Trk_px, &b_Trk_px); }
  if( fChain->GetBranchStatus("Trk_py") ) { fChain->SetBranchAddress("Trk_py", &Trk_py, &b_Trk_py); }
  if( fChain->GetBranchStatus("Trk_pz") ) { fChain->SetBranchAddress("Trk_pz", &Trk_pz, &b_Trk_pz); }
  if( fChain->GetBranchStatus("Trk_pt") ) { fChain->SetBranchAddress("Trk_pt", &Trk_pt, &b_Trk_pt); }
  if( fChain->GetBranchStatus("Trk_numberOfBLayerHits") ) { fChain->SetBranchAddress("Trk_numberOfBLayerHits", &Trk_numberOfBLayerHits, &b_Trk_numberOfBLayerHits); }
  if( fChain->GetBranchStatus("Trk_numberOfPixelHits") ) { fChain->SetBranchAddress("Trk_numberOfPixelHits", &Trk_numberOfPixelHits, &b_Trk_numberOfPixelHits); }
  if( fChain->GetBranchStatus("Trk_numberOfSCTHits") ) { fChain->SetBranchAddress("Trk_numberOfSCTHits", &Trk_numberOfSCTHits, &b_Trk_numberOfSCTHits); }
  if( fChain->GetBranchStatus("Trk_numberOfTRTHits") ) { fChain->SetBranchAddress("Trk_numberOfTRTHits", &Trk_numberOfTRTHits, &b_Trk_numberOfTRTHits); }
  if( fChain->GetBranchStatus("Trk_numberOfTRTHighThresholdHits") ) { fChain->SetBranchAddress("Trk_numberOfTRTHighThresholdHits", &Trk_numberOfTRTHighThresholdHits, &b_Trk_numberOfTRTHighThresholdHits); }
  if( fChain->GetBranchStatus("Trk_numberOfBLayerSharedHits") ) { fChain->SetBranchAddress("Trk_numberOfBLayerSharedHits", &Trk_numberOfBLayerSharedHits, &b_Trk_numberOfBLayerSharedHits); }
  if( fChain->GetBranchStatus("Trk_numberOfPixelSharedHits") ) { fChain->SetBranchAddress("Trk_numberOfPixelSharedHits", &Trk_numberOfPixelSharedHits, &b_Trk_numberOfPixelSharedHits); }
  if( fChain->GetBranchStatus("Trk_numberOfPixelHoles") ) { fChain->SetBranchAddress("Trk_numberOfPixelHoles", &Trk_numberOfPixelHoles, &b_Trk_numberOfPixelHoles); }
  if( fChain->GetBranchStatus("Trk_numberOfSCTSharedHits") ) { fChain->SetBranchAddress("Trk_numberOfSCTSharedHits", &Trk_numberOfSCTSharedHits, &b_Trk_numberOfSCTSharedHits); }
  if( fChain->GetBranchStatus("Trk_numberOfSCTHoles") ) { fChain->SetBranchAddress("Trk_numberOfSCTHoles", &Trk_numberOfSCTHoles, &b_Trk_numberOfSCTHoles); }
  if( fChain->GetBranchStatus("Trk_numberOfTRTOutliers") ) { fChain->SetBranchAddress("Trk_numberOfTRTOutliers", &Trk_numberOfTRTOutliers, &b_Trk_numberOfTRTOutliers); }
  if( fChain->GetBranchStatus("Trk_numberOfTRTHighThresholdOutliers") ) { fChain->SetBranchAddress("Trk_numberOfTRTHighThresholdOutliers", &Trk_numberOfTRTHighThresholdOutliers, &b_Trk_numberOfTRTHighThresholdOutliers); }
  if( fChain->GetBranchStatus("Trk_numberOfMdtHits") ) { fChain->SetBranchAddress("Trk_numberOfMdtHits", &Trk_numberOfMdtHits, &b_Trk_numberOfMdtHits); }
  if( fChain->GetBranchStatus("Trk_numberOfTgcPhiHits") ) { fChain->SetBranchAddress("Trk_numberOfTgcPhiHits", &Trk_numberOfTgcPhiHits, &b_Trk_numberOfTgcPhiHits); }
  if( fChain->GetBranchStatus("Trk_numberOfRpcPhiHits") ) { fChain->SetBranchAddress("Trk_numberOfRpcPhiHits", &Trk_numberOfRpcPhiHits, &b_Trk_numberOfRpcPhiHits); }
  if( fChain->GetBranchStatus("Trk_numberOfCscPhiHits") ) { fChain->SetBranchAddress("Trk_numberOfCscPhiHits", &Trk_numberOfCscPhiHits, &b_Trk_numberOfCscPhiHits); }
  if( fChain->GetBranchStatus("Trk_numberOfTgcEtaHits") ) { fChain->SetBranchAddress("Trk_numberOfTgcEtaHits", &Trk_numberOfTgcEtaHits, &b_Trk_numberOfTgcEtaHits); }
  if( fChain->GetBranchStatus("Trk_numberOfRpcEtaHits") ) { fChain->SetBranchAddress("Trk_numberOfRpcEtaHits", &Trk_numberOfRpcEtaHits, &b_Trk_numberOfRpcEtaHits); }
  if( fChain->GetBranchStatus("Trk_numberOfCscEtaHits") ) { fChain->SetBranchAddress("Trk_numberOfCscEtaHits", &Trk_numberOfCscEtaHits, &b_Trk_numberOfCscEtaHits); }
  if( fChain->GetBranchStatus("Trk_numberOfGangedPixels") ) { fChain->SetBranchAddress("Trk_numberOfGangedPixels", &Trk_numberOfGangedPixels, &b_Trk_numberOfGangedPixels); }
  if( fChain->GetBranchStatus("Trk_numberOfOutliersOnTrack") ) { fChain->SetBranchAddress("Trk_numberOfOutliersOnTrack", &Trk_numberOfOutliersOnTrack, &b_Trk_numberOfOutliersOnTrack); }
  if( fChain->GetBranchStatus("Trk_numberOfTrackSummaryTypes") ) { fChain->SetBranchAddress("Trk_numberOfTrackSummaryTypes", &Trk_numberOfTrackSummaryTypes, &b_Trk_numberOfTrackSummaryTypes); }
  if( fChain->GetBranchStatus("Trk_truthBarcode") ) { fChain->SetBranchAddress("Trk_truthBarcode", &Trk_truthBarcode, &b_Trk_truthBarcode); }
  if( fChain->GetBranchStatus("Trk_truthNt") ) { fChain->SetBranchAddress("Trk_truthNt", &Trk_truthNt, &b_Trk_truthNt); }
  if( fChain->GetBranchStatus("Trk_ParticlePdg") ) { fChain->SetBranchAddress("Trk_ParticlePdg", &Trk_ParticlePdg, &b_Trk_ParticlePdg); }
  if( fChain->GetBranchStatus("Trk_ParentPdg") ) { fChain->SetBranchAddress("Trk_ParentPdg", &Trk_ParentPdg, &b_Trk_ParentPdg); }
  if( fChain->GetBranchStatus("Nh_Calo") ) { fChain->SetBranchAddress("Nh_Calo", &Nh_Calo, &b_Nh_Calo); }
  if( fChain->GetBranchStatus("Eh_Calo") ) { fChain->SetBranchAddress("Eh_Calo", &Eh_Calo, &b_Eh_Calo); }
  if( fChain->GetBranchStatus("Nh_EM") ) { fChain->SetBranchAddress("Nh_EM", &Nh_EM, &b_Nh_EM); }
  if( fChain->GetBranchStatus("Eh_EM") ) { fChain->SetBranchAddress("Eh_EM", &Eh_EM, &b_Eh_EM); }
  if( fChain->GetBranchStatus("Nh_HAD") ) { fChain->SetBranchAddress("Nh_HAD", &Nh_HAD, &b_Nh_HAD); }
  if( fChain->GetBranchStatus("Eh_HAD") ) { fChain->SetBranchAddress("Eh_HAD", &Eh_HAD, &b_Eh_HAD); }
  if( fChain->GetBranchStatus("Nh_PresB") ) { fChain->SetBranchAddress("Nh_PresB", &Nh_PresB, &b_Nh_PresB); }
  if( fChain->GetBranchStatus("Eh_PresB") ) { fChain->SetBranchAddress("Eh_PresB", &Eh_PresB, &b_Eh_PresB); }
  if( fChain->GetBranchStatus("Nh_EMB") ) { fChain->SetBranchAddress("Nh_EMB", &Nh_EMB, &b_Nh_EMB); }
  if( fChain->GetBranchStatus("Eh_EMB") ) { fChain->SetBranchAddress("Eh_EMB", &Eh_EMB, &b_Eh_EMB); }
  if( fChain->GetBranchStatus("Nh_EMEC") ) { fChain->SetBranchAddress("Nh_EMEC", &Nh_EMEC, &b_Nh_EMEC); }
  if( fChain->GetBranchStatus("Eh_EMEC") ) { fChain->SetBranchAddress("Eh_EMEC", &Eh_EMEC, &b_Eh_EMEC); }
  if( fChain->GetBranchStatus("Nh_Tile") ) { fChain->SetBranchAddress("Nh_Tile", &Nh_Tile, &b_Nh_Tile); }
  if( fChain->GetBranchStatus("Eh_Tile") ) { fChain->SetBranchAddress("Eh_Tile", &Eh_Tile, &b_Eh_Tile); }
  if( fChain->GetBranchStatus("Nh_TileGap") ) { fChain->SetBranchAddress("Nh_TileGap", &Nh_TileGap, &b_Nh_TileGap); }
  if( fChain->GetBranchStatus("Eh_TileGap") ) { fChain->SetBranchAddress("Eh_TileGap", &Eh_TileGap, &b_Eh_TileGap); }
  if( fChain->GetBranchStatus("Nh_HEC") ) { fChain->SetBranchAddress("Nh_HEC", &Nh_HEC, &b_Nh_HEC); }
  if( fChain->GetBranchStatus("Eh_HEC") ) { fChain->SetBranchAddress("Eh_HEC", &Eh_HEC, &b_Eh_HEC); }
  if( fChain->GetBranchStatus("Nh_FCal") ) { fChain->SetBranchAddress("Nh_FCal", &Nh_FCal, &b_Nh_FCal); }
  if( fChain->GetBranchStatus("Eh_FCal") ) { fChain->SetBranchAddress("Eh_FCal", &Eh_FCal, &b_Eh_FCal); }
  if( fChain->GetBranchStatus("Nh_PresE") ) { fChain->SetBranchAddress("Nh_PresE", &Nh_PresE, &b_Nh_PresE); }
  if( fChain->GetBranchStatus("Eh_PresE") ) { fChain->SetBranchAddress("Eh_PresE", &Eh_PresE, &b_Eh_PresE); }
  if( fChain->GetBranchStatus("Nh_Scint") ) { fChain->SetBranchAddress("Nh_Scint", &Nh_Scint, &b_Nh_Scint); }
  if( fChain->GetBranchStatus("Eh_Scint") ) { fChain->SetBranchAddress("Eh_Scint", &Eh_Scint, &b_Eh_Scint); }
  if( fChain->GetBranchStatus("nhit_em") ) { fChain->SetBranchAddress("nhit_em", &nhit_em, &b_nhit_em); }
  if( fChain->GetBranchStatus("ecell_em") ) { fChain->SetBranchAddress("ecell_em", &ecell_em, &b_ecell_em); }
  if( fChain->GetBranchStatus("nhit_hec") ) { fChain->SetBranchAddress("nhit_hec", &nhit_hec, &b_nhit_hec); }
  if( fChain->GetBranchStatus("ecell_hec") ) { fChain->SetBranchAddress("ecell_hec", &ecell_hec, &b_ecell_hec); }
  if( fChain->GetBranchStatus("nhit_fcal") ) { fChain->SetBranchAddress("nhit_fcal", &nhit_fcal, &b_nhit_fcal); }
  if( fChain->GetBranchStatus("ecell_fcal") ) { fChain->SetBranchAddress("ecell_fcal", &ecell_fcal, &b_ecell_fcal); }
  if( fChain->GetBranchStatus("nhit_tile") ) { fChain->SetBranchAddress("nhit_tile", &nhit_tile, &b_nhit_tile); }
  if( fChain->GetBranchStatus("ecell_tile") ) { fChain->SetBranchAddress("ecell_tile", &ecell_tile, &b_ecell_tile); }
  if( fChain->GetBranchStatus("cl_ecluster") ) { fChain->SetBranchAddress("cl_ecluster", &cl_ecluster, &b_cl_ecluster); }
  if( fChain->GetBranchStatus("cl_nctotal") ) { fChain->SetBranchAddress("cl_nctotal", &cl_nctotal, &b_cl_nctotal); }
  if( fChain->GetBranchStatus("cl_nc") ) { fChain->SetBranchAddress("cl_nc", &cl_nc, &b_cl_nc); }
  if( fChain->GetBranchStatus("cl_et") ) { fChain->SetBranchAddress("cl_et", &cl_et, &b_cl_et); }
  if( fChain->GetBranchStatus("cl_e") ) { fChain->SetBranchAddress("cl_e", &cl_e, &b_cl_e); }
  if( fChain->GetBranchStatus("cl_eemb0") ) { fChain->SetBranchAddress("cl_eemb0", &cl_eemb0, &b_cl_eemb0); }
  if( fChain->GetBranchStatus("cl_eemb1") ) { fChain->SetBranchAddress("cl_eemb1", &cl_eemb1, &b_cl_eemb1); }
  if( fChain->GetBranchStatus("cl_eemb2") ) { fChain->SetBranchAddress("cl_eemb2", &cl_eemb2, &b_cl_eemb2); }
  if( fChain->GetBranchStatus("cl_eemb3") ) { fChain->SetBranchAddress("cl_eemb3", &cl_eemb3, &b_cl_eemb3); }
  if( fChain->GetBranchStatus("cl_nemb0") ) { fChain->SetBranchAddress("cl_nemb0", &cl_nemb0, &b_cl_nemb0); }
  if( fChain->GetBranchStatus("cl_nemb1") ) { fChain->SetBranchAddress("cl_nemb1", &cl_nemb1, &b_cl_nemb1); }
  if( fChain->GetBranchStatus("cl_nemb2") ) { fChain->SetBranchAddress("cl_nemb2", &cl_nemb2, &b_cl_nemb2); }
  if( fChain->GetBranchStatus("cl_nemb3") ) { fChain->SetBranchAddress("cl_nemb3", &cl_nemb3, &b_cl_nemb3); }
  if( fChain->GetBranchStatus("cl_phi2") ) { fChain->SetBranchAddress("cl_phi2", &cl_phi2, &b_cl_phi2); }
  if( fChain->GetBranchStatus("cl_eta0") ) { fChain->SetBranchAddress("cl_eta0", &cl_eta0, &b_cl_eta0); }
  if( fChain->GetBranchStatus("cl_eta1") ) { fChain->SetBranchAddress("cl_eta1", &cl_eta1, &b_cl_eta1); }
  if( fChain->GetBranchStatus("cl_eta2") ) { fChain->SetBranchAddress("cl_eta2", &cl_eta2, &b_cl_eta2); }
  if( fChain->GetBranchStatus("cl_eta3") ) { fChain->SetBranchAddress("cl_eta3", &cl_eta3, &b_cl_eta3); }
  if( fChain->GetBranchStatus("cl_eeme0") ) { fChain->SetBranchAddress("cl_eeme0", &cl_eeme0, &b_cl_eeme0); }
  if( fChain->GetBranchStatus("cl_eeme1") ) { fChain->SetBranchAddress("cl_eeme1", &cl_eeme1, &b_cl_eeme1); }
  if( fChain->GetBranchStatus("cl_eeme2") ) { fChain->SetBranchAddress("cl_eeme2", &cl_eeme2, &b_cl_eeme2); }
  if( fChain->GetBranchStatus("cl_eeme3") ) { fChain->SetBranchAddress("cl_eeme3", &cl_eeme3, &b_cl_eeme3); }
  if( fChain->GetBranchStatus("cl_neme0") ) { fChain->SetBranchAddress("cl_neme0", &cl_neme0, &b_cl_neme0); }
  if( fChain->GetBranchStatus("cl_neme1") ) { fChain->SetBranchAddress("cl_neme1", &cl_neme1, &b_cl_neme1); }
  if( fChain->GetBranchStatus("cl_neme2") ) { fChain->SetBranchAddress("cl_neme2", &cl_neme2, &b_cl_neme2); }
  if( fChain->GetBranchStatus("cl_neme3") ) { fChain->SetBranchAddress("cl_neme3", &cl_neme3, &b_cl_neme3); }
  if( fChain->GetBranchStatus("cl_etileg1") ) { fChain->SetBranchAddress("cl_etileg1", &cl_etileg1, &b_cl_etileg1); }
  if( fChain->GetBranchStatus("cl_etileg2") ) { fChain->SetBranchAddress("cl_etileg2", &cl_etileg2, &b_cl_etileg2); }
  if( fChain->GetBranchStatus("cl_etileg3") ) { fChain->SetBranchAddress("cl_etileg3", &cl_etileg3, &b_cl_etileg3); }
  if( fChain->GetBranchStatus("cl_ntileg1") ) { fChain->SetBranchAddress("cl_ntileg1", &cl_ntileg1, &b_cl_ntileg1); }
  if( fChain->GetBranchStatus("cl_ntileg2") ) { fChain->SetBranchAddress("cl_ntileg2", &cl_ntileg2, &b_cl_ntileg2); }
  if( fChain->GetBranchStatus("cl_ntileg3") ) { fChain->SetBranchAddress("cl_ntileg3", &cl_ntileg3, &b_cl_ntileg3); }
  if( fChain->GetBranchStatus("cl_eta") ) { fChain->SetBranchAddress("cl_eta", &cl_eta, &b_cl_eta); }
  if( fChain->GetBranchStatus("cl_phi") ) { fChain->SetBranchAddress("cl_phi", &cl_phi, &b_cl_phi); }
  if( fChain->GetBranchStatus("cl_time") ) { fChain->SetBranchAddress("cl_time", &cl_time, &b_cl_time); }
  if( fChain->GetBranchStatus("cl_reco_stat") ) { fChain->SetBranchAddress("cl_reco_stat", &cl_reco_stat, &b_cl_reco_stat); }
  if( fChain->GetBranchStatus("cl_m1_eta") ) { fChain->SetBranchAddress("cl_m1_eta", &cl_m1_eta, &b_cl_m1_eta); }
  if( fChain->GetBranchStatus("cl_m1_phi") ) { fChain->SetBranchAddress("cl_m1_phi", &cl_m1_phi, &b_cl_m1_phi); }
  if( fChain->GetBranchStatus("cl_m2_r") ) { fChain->SetBranchAddress("cl_m2_r", &cl_m2_r, &b_cl_m2_r); }
  if( fChain->GetBranchStatus("cl_m2_lambda") ) { fChain->SetBranchAddress("cl_m2_lambda", &cl_m2_lambda, &b_cl_m2_lambda); }
  if( fChain->GetBranchStatus("cl_delta_phi") ) { fChain->SetBranchAddress("cl_delta_phi", &cl_delta_phi, &b_cl_delta_phi); }
  if( fChain->GetBranchStatus("cl_delta_theta") ) { fChain->SetBranchAddress("cl_delta_theta", &cl_delta_theta, &b_cl_delta_theta); }
  if( fChain->GetBranchStatus("cl_delta_alpha") ) { fChain->SetBranchAddress("cl_delta_alpha", &cl_delta_alpha, &b_cl_delta_alpha); }
  if( fChain->GetBranchStatus("cl_center_x") ) { fChain->SetBranchAddress("cl_center_x", &cl_center_x, &b_cl_center_x); }
  if( fChain->GetBranchStatus("cl_center_y") ) { fChain->SetBranchAddress("cl_center_y", &cl_center_y, &b_cl_center_y); }
  if( fChain->GetBranchStatus("cl_center_z") ) { fChain->SetBranchAddress("cl_center_z", &cl_center_z, &b_cl_center_z); }
  if( fChain->GetBranchStatus("cl_center_lambda") ) { fChain->SetBranchAddress("cl_center_lambda", &cl_center_lambda, &b_cl_center_lambda); }
  if( fChain->GetBranchStatus("cl_lateral") ) { fChain->SetBranchAddress("cl_lateral", &cl_lateral, &b_cl_lateral); }
  if( fChain->GetBranchStatus("cl_longitudinal") ) { fChain->SetBranchAddress("cl_longitudinal", &cl_longitudinal, &b_cl_longitudinal); }
  if( fChain->GetBranchStatus("cl_eng_frac_em") ) { fChain->SetBranchAddress("cl_eng_frac_em", &cl_eng_frac_em, &b_cl_eng_frac_em); }
  if( fChain->GetBranchStatus("cl_eng_frac_max") ) { fChain->SetBranchAddress("cl_eng_frac_max", &cl_eng_frac_max, &b_cl_eng_frac_max); }
  if( fChain->GetBranchStatus("cl_eng_frac_core") ) { fChain->SetBranchAddress("cl_eng_frac_core", &cl_eng_frac_core, &b_cl_eng_frac_core); }
  if( fChain->GetBranchStatus("cl_m1_dens") ) { fChain->SetBranchAddress("cl_m1_dens", &cl_m1_dens, &b_cl_m1_dens); }
  if( fChain->GetBranchStatus("cl_m2_dens") ) { fChain->SetBranchAddress("cl_m2_dens", &cl_m2_dens, &b_cl_m2_dens); }
  if( fChain->GetBranchStatus("cl_ntotcells") ) { fChain->SetBranchAddress("cl_ntotcells", &cl_ntotcells, &b_cl_ntotcells); }
  if( fChain->GetBranchStatus("cl_ecluster_gam") ) { fChain->SetBranchAddress("cl_ecluster_gam", &cl_ecluster_gam, &b_cl_ecluster_gam); }
  if( fChain->GetBranchStatus("cl_nctotal_gam") ) { fChain->SetBranchAddress("cl_nctotal_gam", &cl_nctotal_gam, &b_cl_nctotal_gam); }
  if( fChain->GetBranchStatus("cl_nc_gam") ) { fChain->SetBranchAddress("cl_nc_gam", &cl_nc_gam, &b_cl_nc_gam); }
  if( fChain->GetBranchStatus("cl_et_gam") ) { fChain->SetBranchAddress("cl_et_gam", &cl_et_gam, &b_cl_et_gam); }
  if( fChain->GetBranchStatus("cl_e_gam") ) { fChain->SetBranchAddress("cl_e_gam", &cl_e_gam, &b_cl_e_gam); }
  if( fChain->GetBranchStatus("cl_eemb0_gam") ) { fChain->SetBranchAddress("cl_eemb0_gam", &cl_eemb0_gam, &b_cl_eemb0_gam); }
  if( fChain->GetBranchStatus("cl_eemb1_gam") ) { fChain->SetBranchAddress("cl_eemb1_gam", &cl_eemb1_gam, &b_cl_eemb1_gam); }
  if( fChain->GetBranchStatus("cl_eemb2_gam") ) { fChain->SetBranchAddress("cl_eemb2_gam", &cl_eemb2_gam, &b_cl_eemb2_gam); }
  if( fChain->GetBranchStatus("cl_eemb3_gam") ) { fChain->SetBranchAddress("cl_eemb3_gam", &cl_eemb3_gam, &b_cl_eemb3_gam); }
  if( fChain->GetBranchStatus("cl_nemb0_gam") ) { fChain->SetBranchAddress("cl_nemb0_gam", &cl_nemb0_gam, &b_cl_nemb0_gam); }
  if( fChain->GetBranchStatus("cl_nemb1_gam") ) { fChain->SetBranchAddress("cl_nemb1_gam", &cl_nemb1_gam, &b_cl_nemb1_gam); }
  if( fChain->GetBranchStatus("cl_nemb2_gam") ) { fChain->SetBranchAddress("cl_nemb2_gam", &cl_nemb2_gam, &b_cl_nemb2_gam); }
  if( fChain->GetBranchStatus("cl_nemb3_gam") ) { fChain->SetBranchAddress("cl_nemb3_gam", &cl_nemb3_gam, &b_cl_nemb3_gam); }
  if( fChain->GetBranchStatus("cl_phi2_gam") ) { fChain->SetBranchAddress("cl_phi2_gam", &cl_phi2_gam, &b_cl_phi2_gam); }
  if( fChain->GetBranchStatus("cl_eta0_gam") ) { fChain->SetBranchAddress("cl_eta0_gam", &cl_eta0_gam, &b_cl_eta0_gam); }
  if( fChain->GetBranchStatus("cl_eta1_gam") ) { fChain->SetBranchAddress("cl_eta1_gam", &cl_eta1_gam, &b_cl_eta1_gam); }
  if( fChain->GetBranchStatus("cl_eta2_gam") ) { fChain->SetBranchAddress("cl_eta2_gam", &cl_eta2_gam, &b_cl_eta2_gam); }
  if( fChain->GetBranchStatus("cl_eta3_gam") ) { fChain->SetBranchAddress("cl_eta3_gam", &cl_eta3_gam, &b_cl_eta3_gam); }
  if( fChain->GetBranchStatus("cl_eeme0_gam") ) { fChain->SetBranchAddress("cl_eeme0_gam", &cl_eeme0_gam, &b_cl_eeme0_gam); }
  if( fChain->GetBranchStatus("cl_eeme1_gam") ) { fChain->SetBranchAddress("cl_eeme1_gam", &cl_eeme1_gam, &b_cl_eeme1_gam); }
  if( fChain->GetBranchStatus("cl_eeme2_gam") ) { fChain->SetBranchAddress("cl_eeme2_gam", &cl_eeme2_gam, &b_cl_eeme2_gam); }
  if( fChain->GetBranchStatus("cl_eeme3_gam") ) { fChain->SetBranchAddress("cl_eeme3_gam", &cl_eeme3_gam, &b_cl_eeme3_gam); }
  if( fChain->GetBranchStatus("cl_neme0_gam") ) { fChain->SetBranchAddress("cl_neme0_gam", &cl_neme0_gam, &b_cl_neme0_gam); }
  if( fChain->GetBranchStatus("cl_neme1_gam") ) { fChain->SetBranchAddress("cl_neme1_gam", &cl_neme1_gam, &b_cl_neme1_gam); }
  if( fChain->GetBranchStatus("cl_neme2_gam") ) { fChain->SetBranchAddress("cl_neme2_gam", &cl_neme2_gam, &b_cl_neme2_gam); }
  if( fChain->GetBranchStatus("cl_neme3_gam") ) { fChain->SetBranchAddress("cl_neme3_gam", &cl_neme3_gam, &b_cl_neme3_gam); }
  if( fChain->GetBranchStatus("cl_etileg1_gam") ) { fChain->SetBranchAddress("cl_etileg1_gam", &cl_etileg1_gam, &b_cl_etileg1_gam); }
  if( fChain->GetBranchStatus("cl_etileg2_gam") ) { fChain->SetBranchAddress("cl_etileg2_gam", &cl_etileg2_gam, &b_cl_etileg2_gam); }
  if( fChain->GetBranchStatus("cl_etileg3_gam") ) { fChain->SetBranchAddress("cl_etileg3_gam", &cl_etileg3_gam, &b_cl_etileg3_gam); }
  if( fChain->GetBranchStatus("cl_ntileg1_gam") ) { fChain->SetBranchAddress("cl_ntileg1_gam", &cl_ntileg1_gam, &b_cl_ntileg1_gam); }
  if( fChain->GetBranchStatus("cl_ntileg2_gam") ) { fChain->SetBranchAddress("cl_ntileg2_gam", &cl_ntileg2_gam, &b_cl_ntileg2_gam); }
  if( fChain->GetBranchStatus("cl_ntileg3_gam") ) { fChain->SetBranchAddress("cl_ntileg3_gam", &cl_ntileg3_gam, &b_cl_ntileg3_gam); }
  if( fChain->GetBranchStatus("cl_eta_gam") ) { fChain->SetBranchAddress("cl_eta_gam", &cl_eta_gam, &b_cl_eta_gam); }
  if( fChain->GetBranchStatus("cl_phi_gam") ) { fChain->SetBranchAddress("cl_phi_gam", &cl_phi_gam, &b_cl_phi_gam); }
  if( fChain->GetBranchStatus("cl_time_gam") ) { fChain->SetBranchAddress("cl_time_gam", &cl_time_gam, &b_cl_time_gam); }
  if( fChain->GetBranchStatus("cl_reco_stat_gam") ) { fChain->SetBranchAddress("cl_reco_stat_gam", &cl_reco_stat_gam, &b_cl_reco_stat_gam); }
  if( fChain->GetBranchStatus("cl_m1_eta_gam") ) { fChain->SetBranchAddress("cl_m1_eta_gam", &cl_m1_eta_gam, &b_cl_m1_eta_gam); }
  if( fChain->GetBranchStatus("cl_m1_phi_gam") ) { fChain->SetBranchAddress("cl_m1_phi_gam", &cl_m1_phi_gam, &b_cl_m1_phi_gam); }
  if( fChain->GetBranchStatus("cl_m2_r_gam") ) { fChain->SetBranchAddress("cl_m2_r_gam", &cl_m2_r_gam, &b_cl_m2_r_gam); }
  if( fChain->GetBranchStatus("cl_m2_lambda_gam") ) { fChain->SetBranchAddress("cl_m2_lambda_gam", &cl_m2_lambda_gam, &b_cl_m2_lambda_gam); }
  if( fChain->GetBranchStatus("cl_delta_phi_gam") ) { fChain->SetBranchAddress("cl_delta_phi_gam", &cl_delta_phi_gam, &b_cl_delta_phi_gam); }
  if( fChain->GetBranchStatus("cl_delta_theta_gam") ) { fChain->SetBranchAddress("cl_delta_theta_gam", &cl_delta_theta_gam, &b_cl_delta_theta_gam); }
  if( fChain->GetBranchStatus("cl_delta_alpha_gam") ) { fChain->SetBranchAddress("cl_delta_alpha_gam", &cl_delta_alpha_gam, &b_cl_delta_alpha_gam); }
  if( fChain->GetBranchStatus("cl_center_x_gam") ) { fChain->SetBranchAddress("cl_center_x_gam", &cl_center_x_gam, &b_cl_center_x_gam); }
  if( fChain->GetBranchStatus("cl_center_y_gam") ) { fChain->SetBranchAddress("cl_center_y_gam", &cl_center_y_gam, &b_cl_center_y_gam); }
  if( fChain->GetBranchStatus("cl_center_z_gam") ) { fChain->SetBranchAddress("cl_center_z_gam", &cl_center_z_gam, &b_cl_center_z_gam); }
  if( fChain->GetBranchStatus("cl_center_lambda_gam") ) { fChain->SetBranchAddress("cl_center_lambda_gam", &cl_center_lambda_gam, &b_cl_center_lambda_gam); }
  if( fChain->GetBranchStatus("cl_lateral_gam") ) { fChain->SetBranchAddress("cl_lateral_gam", &cl_lateral_gam, &b_cl_lateral_gam); }
  if( fChain->GetBranchStatus("cl_longitudinal_gam") ) { fChain->SetBranchAddress("cl_longitudinal_gam", &cl_longitudinal_gam, &b_cl_longitudinal_gam); }
  if( fChain->GetBranchStatus("cl_eng_frac_em_gam") ) { fChain->SetBranchAddress("cl_eng_frac_em_gam", &cl_eng_frac_em_gam, &b_cl_eng_frac_em_gam); }
  if( fChain->GetBranchStatus("cl_eng_frac_max_gam") ) { fChain->SetBranchAddress("cl_eng_frac_max_gam", &cl_eng_frac_max_gam, &b_cl_eng_frac_max_gam); }
  if( fChain->GetBranchStatus("cl_eng_frac_core_gam") ) { fChain->SetBranchAddress("cl_eng_frac_core_gam", &cl_eng_frac_core_gam, &b_cl_eng_frac_core_gam); }
  if( fChain->GetBranchStatus("cl_m1_dens_gam") ) { fChain->SetBranchAddress("cl_m1_dens_gam", &cl_m1_dens_gam, &b_cl_m1_dens_gam); }
  if( fChain->GetBranchStatus("cl_m2_dens_gam") ) { fChain->SetBranchAddress("cl_m2_dens_gam", &cl_m2_dens_gam, &b_cl_m2_dens_gam); }
  if( fChain->GetBranchStatus("cl_ntotcells_gam") ) { fChain->SetBranchAddress("cl_ntotcells_gam", &cl_ntotcells_gam, &b_cl_ntotcells_gam); }
  if( fChain->GetBranchStatus("cl_ecluster_35") ) { fChain->SetBranchAddress("cl_ecluster_35", &cl_ecluster_35, &b_cl_ecluster_35); }
  if( fChain->GetBranchStatus("cl_nctotal_35") ) { fChain->SetBranchAddress("cl_nctotal_35", &cl_nctotal_35, &b_cl_nctotal_35); }
  if( fChain->GetBranchStatus("cl_nc_35") ) { fChain->SetBranchAddress("cl_nc_35", &cl_nc_35, &b_cl_nc_35); }
  if( fChain->GetBranchStatus("cl_et_35") ) { fChain->SetBranchAddress("cl_et_35", &cl_et_35, &b_cl_et_35); }
  if( fChain->GetBranchStatus("cl_e_35") ) { fChain->SetBranchAddress("cl_e_35", &cl_e_35, &b_cl_e_35); }
  if( fChain->GetBranchStatus("cl_eemb0_35") ) { fChain->SetBranchAddress("cl_eemb0_35", &cl_eemb0_35, &b_cl_eemb0_35); }
  if( fChain->GetBranchStatus("cl_eemb1_35") ) { fChain->SetBranchAddress("cl_eemb1_35", &cl_eemb1_35, &b_cl_eemb1_35); }
  if( fChain->GetBranchStatus("cl_eemb2_35") ) { fChain->SetBranchAddress("cl_eemb2_35", &cl_eemb2_35, &b_cl_eemb2_35); }
  if( fChain->GetBranchStatus("cl_eemb3_35") ) { fChain->SetBranchAddress("cl_eemb3_35", &cl_eemb3_35, &b_cl_eemb3_35); }
  if( fChain->GetBranchStatus("cl_nemb0_35") ) { fChain->SetBranchAddress("cl_nemb0_35", &cl_nemb0_35, &b_cl_nemb0_35); }
  if( fChain->GetBranchStatus("cl_nemb1_35") ) { fChain->SetBranchAddress("cl_nemb1_35", &cl_nemb1_35, &b_cl_nemb1_35); }
  if( fChain->GetBranchStatus("cl_nemb2_35") ) { fChain->SetBranchAddress("cl_nemb2_35", &cl_nemb2_35, &b_cl_nemb2_35); }
  if( fChain->GetBranchStatus("cl_nemb3_35") ) { fChain->SetBranchAddress("cl_nemb3_35", &cl_nemb3_35, &b_cl_nemb3_35); }
  if( fChain->GetBranchStatus("cl_phi2_35") ) { fChain->SetBranchAddress("cl_phi2_35", &cl_phi2_35, &b_cl_phi2_35); }
  if( fChain->GetBranchStatus("cl_eta0_35") ) { fChain->SetBranchAddress("cl_eta0_35", &cl_eta0_35, &b_cl_eta0_35); }
  if( fChain->GetBranchStatus("cl_eta1_35") ) { fChain->SetBranchAddress("cl_eta1_35", &cl_eta1_35, &b_cl_eta1_35); }
  if( fChain->GetBranchStatus("cl_eta2_35") ) { fChain->SetBranchAddress("cl_eta2_35", &cl_eta2_35, &b_cl_eta2_35); }
  if( fChain->GetBranchStatus("cl_eta3_35") ) { fChain->SetBranchAddress("cl_eta3_35", &cl_eta3_35, &b_cl_eta3_35); }
  if( fChain->GetBranchStatus("cl_eeme0_35") ) { fChain->SetBranchAddress("cl_eeme0_35", &cl_eeme0_35, &b_cl_eeme0_35); }
  if( fChain->GetBranchStatus("cl_eeme1_35") ) { fChain->SetBranchAddress("cl_eeme1_35", &cl_eeme1_35, &b_cl_eeme1_35); }
  if( fChain->GetBranchStatus("cl_eeme2_35") ) { fChain->SetBranchAddress("cl_eeme2_35", &cl_eeme2_35, &b_cl_eeme2_35); }
  if( fChain->GetBranchStatus("cl_eeme3_35") ) { fChain->SetBranchAddress("cl_eeme3_35", &cl_eeme3_35, &b_cl_eeme3_35); }
  if( fChain->GetBranchStatus("cl_neme0_35") ) { fChain->SetBranchAddress("cl_neme0_35", &cl_neme0_35, &b_cl_neme0_35); }
  if( fChain->GetBranchStatus("cl_neme1_35") ) { fChain->SetBranchAddress("cl_neme1_35", &cl_neme1_35, &b_cl_neme1_35); }
  if( fChain->GetBranchStatus("cl_neme2_35") ) { fChain->SetBranchAddress("cl_neme2_35", &cl_neme2_35, &b_cl_neme2_35); }
  if( fChain->GetBranchStatus("cl_neme3_35") ) { fChain->SetBranchAddress("cl_neme3_35", &cl_neme3_35, &b_cl_neme3_35); }
  if( fChain->GetBranchStatus("cl_etileg1_35") ) { fChain->SetBranchAddress("cl_etileg1_35", &cl_etileg1_35, &b_cl_etileg1_35); }
  if( fChain->GetBranchStatus("cl_etileg2_35") ) { fChain->SetBranchAddress("cl_etileg2_35", &cl_etileg2_35, &b_cl_etileg2_35); }
  if( fChain->GetBranchStatus("cl_etileg3_35") ) { fChain->SetBranchAddress("cl_etileg3_35", &cl_etileg3_35, &b_cl_etileg3_35); }
  if( fChain->GetBranchStatus("cl_ntileg1_35") ) { fChain->SetBranchAddress("cl_ntileg1_35", &cl_ntileg1_35, &b_cl_ntileg1_35); }
  if( fChain->GetBranchStatus("cl_ntileg2_35") ) { fChain->SetBranchAddress("cl_ntileg2_35", &cl_ntileg2_35, &b_cl_ntileg2_35); }
  if( fChain->GetBranchStatus("cl_ntileg3_35") ) { fChain->SetBranchAddress("cl_ntileg3_35", &cl_ntileg3_35, &b_cl_ntileg3_35); }
  if( fChain->GetBranchStatus("cl_eta_35") ) { fChain->SetBranchAddress("cl_eta_35", &cl_eta_35, &b_cl_eta_35); }
  if( fChain->GetBranchStatus("cl_phi_35") ) { fChain->SetBranchAddress("cl_phi_35", &cl_phi_35, &b_cl_phi_35); }
  if( fChain->GetBranchStatus("cl_time_35") ) { fChain->SetBranchAddress("cl_time_35", &cl_time_35, &b_cl_time_35); }
  if( fChain->GetBranchStatus("cl_reco_stat_35") ) { fChain->SetBranchAddress("cl_reco_stat_35", &cl_reco_stat_35, &b_cl_reco_stat_35); }
  if( fChain->GetBranchStatus("cl_m1_eta_35") ) { fChain->SetBranchAddress("cl_m1_eta_35", &cl_m1_eta_35, &b_cl_m1_eta_35); }
  if( fChain->GetBranchStatus("cl_m1_phi_35") ) { fChain->SetBranchAddress("cl_m1_phi_35", &cl_m1_phi_35, &b_cl_m1_phi_35); }
  if( fChain->GetBranchStatus("cl_m2_r_35") ) { fChain->SetBranchAddress("cl_m2_r_35", &cl_m2_r_35, &b_cl_m2_r_35); }
  if( fChain->GetBranchStatus("cl_m2_lambda_35") ) { fChain->SetBranchAddress("cl_m2_lambda_35", &cl_m2_lambda_35, &b_cl_m2_lambda_35); }
  if( fChain->GetBranchStatus("cl_delta_phi_35") ) { fChain->SetBranchAddress("cl_delta_phi_35", &cl_delta_phi_35, &b_cl_delta_phi_35); }
  if( fChain->GetBranchStatus("cl_delta_theta_35") ) { fChain->SetBranchAddress("cl_delta_theta_35", &cl_delta_theta_35, &b_cl_delta_theta_35); }
  if( fChain->GetBranchStatus("cl_delta_alpha_35") ) { fChain->SetBranchAddress("cl_delta_alpha_35", &cl_delta_alpha_35, &b_cl_delta_alpha_35); }
  if( fChain->GetBranchStatus("cl_center_x_35") ) { fChain->SetBranchAddress("cl_center_x_35", &cl_center_x_35, &b_cl_center_x_35); }
  if( fChain->GetBranchStatus("cl_center_y_35") ) { fChain->SetBranchAddress("cl_center_y_35", &cl_center_y_35, &b_cl_center_y_35); }
  if( fChain->GetBranchStatus("cl_center_z_35") ) { fChain->SetBranchAddress("cl_center_z_35", &cl_center_z_35, &b_cl_center_z_35); }
  if( fChain->GetBranchStatus("cl_center_lambda_35") ) { fChain->SetBranchAddress("cl_center_lambda_35", &cl_center_lambda_35, &b_cl_center_lambda_35); }
  if( fChain->GetBranchStatus("cl_lateral_35") ) { fChain->SetBranchAddress("cl_lateral_35", &cl_lateral_35, &b_cl_lateral_35); }
  if( fChain->GetBranchStatus("cl_longitudinal_35") ) { fChain->SetBranchAddress("cl_longitudinal_35", &cl_longitudinal_35, &b_cl_longitudinal_35); }
  if( fChain->GetBranchStatus("cl_eng_frac_em_35") ) { fChain->SetBranchAddress("cl_eng_frac_em_35", &cl_eng_frac_em_35, &b_cl_eng_frac_em_35); }
  if( fChain->GetBranchStatus("cl_eng_frac_max_35") ) { fChain->SetBranchAddress("cl_eng_frac_max_35", &cl_eng_frac_max_35, &b_cl_eng_frac_max_35); }
  if( fChain->GetBranchStatus("cl_eng_frac_core_35") ) { fChain->SetBranchAddress("cl_eng_frac_core_35", &cl_eng_frac_core_35, &b_cl_eng_frac_core_35); }
  if( fChain->GetBranchStatus("cl_m1_dens_35") ) { fChain->SetBranchAddress("cl_m1_dens_35", &cl_m1_dens_35, &b_cl_m1_dens_35); }
  if( fChain->GetBranchStatus("cl_m2_dens_35") ) { fChain->SetBranchAddress("cl_m2_dens_35", &cl_m2_dens_35, &b_cl_m2_dens_35); }
  if( fChain->GetBranchStatus("cl_ntotcells_35") ) { fChain->SetBranchAddress("cl_ntotcells_35", &cl_ntotcells_35, &b_cl_ntotcells_35); }
  if( fChain->GetBranchStatus("cl_ecluster_35gam") ) { fChain->SetBranchAddress("cl_ecluster_35gam", &cl_ecluster_35gam, &b_cl_ecluster_35gam); }
  if( fChain->GetBranchStatus("cl_nctotal_35gam") ) { fChain->SetBranchAddress("cl_nctotal_35gam", &cl_nctotal_35gam, &b_cl_nctotal_35gam); }
  if( fChain->GetBranchStatus("cl_nc_35gam") ) { fChain->SetBranchAddress("cl_nc_35gam", &cl_nc_35gam, &b_cl_nc_35gam); }
  if( fChain->GetBranchStatus("cl_et_35gam") ) { fChain->SetBranchAddress("cl_et_35gam", &cl_et_35gam, &b_cl_et_35gam); }
  if( fChain->GetBranchStatus("cl_e_35gam") ) { fChain->SetBranchAddress("cl_e_35gam", &cl_e_35gam, &b_cl_e_35gam); }
  if( fChain->GetBranchStatus("cl_eemb0_35gam") ) { fChain->SetBranchAddress("cl_eemb0_35gam", &cl_eemb0_35gam, &b_cl_eemb0_35gam); }
  if( fChain->GetBranchStatus("cl_eemb1_35gam") ) { fChain->SetBranchAddress("cl_eemb1_35gam", &cl_eemb1_35gam, &b_cl_eemb1_35gam); }
  if( fChain->GetBranchStatus("cl_eemb2_35gam") ) { fChain->SetBranchAddress("cl_eemb2_35gam", &cl_eemb2_35gam, &b_cl_eemb2_35gam); }
  if( fChain->GetBranchStatus("cl_eemb3_35gam") ) { fChain->SetBranchAddress("cl_eemb3_35gam", &cl_eemb3_35gam, &b_cl_eemb3_35gam); }
  if( fChain->GetBranchStatus("cl_nemb0_35gam") ) { fChain->SetBranchAddress("cl_nemb0_35gam", &cl_nemb0_35gam, &b_cl_nemb0_35gam); }
  if( fChain->GetBranchStatus("cl_nemb1_35gam") ) { fChain->SetBranchAddress("cl_nemb1_35gam", &cl_nemb1_35gam, &b_cl_nemb1_35gam); }
  if( fChain->GetBranchStatus("cl_nemb2_35gam") ) { fChain->SetBranchAddress("cl_nemb2_35gam", &cl_nemb2_35gam, &b_cl_nemb2_35gam); }
  if( fChain->GetBranchStatus("cl_nemb3_35gam") ) { fChain->SetBranchAddress("cl_nemb3_35gam", &cl_nemb3_35gam, &b_cl_nemb3_35gam); }
  if( fChain->GetBranchStatus("cl_phi2_35gam") ) { fChain->SetBranchAddress("cl_phi2_35gam", &cl_phi2_35gam, &b_cl_phi2_35gam); }
  if( fChain->GetBranchStatus("cl_eta0_35gam") ) { fChain->SetBranchAddress("cl_eta0_35gam", &cl_eta0_35gam, &b_cl_eta0_35gam); }
  if( fChain->GetBranchStatus("cl_eta1_35gam") ) { fChain->SetBranchAddress("cl_eta1_35gam", &cl_eta1_35gam, &b_cl_eta1_35gam); }
  if( fChain->GetBranchStatus("cl_eta2_35gam") ) { fChain->SetBranchAddress("cl_eta2_35gam", &cl_eta2_35gam, &b_cl_eta2_35gam); }
  if( fChain->GetBranchStatus("cl_eta3_35gam") ) { fChain->SetBranchAddress("cl_eta3_35gam", &cl_eta3_35gam, &b_cl_eta3_35gam); }
  if( fChain->GetBranchStatus("cl_eeme0_35gam") ) { fChain->SetBranchAddress("cl_eeme0_35gam", &cl_eeme0_35gam, &b_cl_eeme0_35gam); }
  if( fChain->GetBranchStatus("cl_eeme1_35gam") ) { fChain->SetBranchAddress("cl_eeme1_35gam", &cl_eeme1_35gam, &b_cl_eeme1_35gam); }
  if( fChain->GetBranchStatus("cl_eeme2_35gam") ) { fChain->SetBranchAddress("cl_eeme2_35gam", &cl_eeme2_35gam, &b_cl_eeme2_35gam); }
  if( fChain->GetBranchStatus("cl_eeme3_35gam") ) { fChain->SetBranchAddress("cl_eeme3_35gam", &cl_eeme3_35gam, &b_cl_eeme3_35gam); }
  if( fChain->GetBranchStatus("cl_neme0_35gam") ) { fChain->SetBranchAddress("cl_neme0_35gam", &cl_neme0_35gam, &b_cl_neme0_35gam); }
  if( fChain->GetBranchStatus("cl_neme1_35gam") ) { fChain->SetBranchAddress("cl_neme1_35gam", &cl_neme1_35gam, &b_cl_neme1_35gam); }
  if( fChain->GetBranchStatus("cl_neme2_35gam") ) { fChain->SetBranchAddress("cl_neme2_35gam", &cl_neme2_35gam, &b_cl_neme2_35gam); }
  if( fChain->GetBranchStatus("cl_neme3_35gam") ) { fChain->SetBranchAddress("cl_neme3_35gam", &cl_neme3_35gam, &b_cl_neme3_35gam); }
  if( fChain->GetBranchStatus("cl_etileg1_35gam") ) { fChain->SetBranchAddress("cl_etileg1_35gam", &cl_etileg1_35gam, &b_cl_etileg1_35gam); }
  if( fChain->GetBranchStatus("cl_etileg2_35gam") ) { fChain->SetBranchAddress("cl_etileg2_35gam", &cl_etileg2_35gam, &b_cl_etileg2_35gam); }
  if( fChain->GetBranchStatus("cl_etileg3_35gam") ) { fChain->SetBranchAddress("cl_etileg3_35gam", &cl_etileg3_35gam, &b_cl_etileg3_35gam); }
  if( fChain->GetBranchStatus("cl_ntileg1_35gam") ) { fChain->SetBranchAddress("cl_ntileg1_35gam", &cl_ntileg1_35gam, &b_cl_ntileg1_35gam); }
  if( fChain->GetBranchStatus("cl_ntileg2_35gam") ) { fChain->SetBranchAddress("cl_ntileg2_35gam", &cl_ntileg2_35gam, &b_cl_ntileg2_35gam); }
  if( fChain->GetBranchStatus("cl_ntileg3_35gam") ) { fChain->SetBranchAddress("cl_ntileg3_35gam", &cl_ntileg3_35gam, &b_cl_ntileg3_35gam); }
  if( fChain->GetBranchStatus("cl_eta_35gam") ) { fChain->SetBranchAddress("cl_eta_35gam", &cl_eta_35gam, &b_cl_eta_35gam); }
  if( fChain->GetBranchStatus("cl_phi_35gam") ) { fChain->SetBranchAddress("cl_phi_35gam", &cl_phi_35gam, &b_cl_phi_35gam); }
  if( fChain->GetBranchStatus("cl_time_35gam") ) { fChain->SetBranchAddress("cl_time_35gam", &cl_time_35gam, &b_cl_time_35gam); }
  if( fChain->GetBranchStatus("cl_reco_stat_35gam") ) { fChain->SetBranchAddress("cl_reco_stat_35gam", &cl_reco_stat_35gam, &b_cl_reco_stat_35gam); }
  if( fChain->GetBranchStatus("cl_m1_eta_35gam") ) { fChain->SetBranchAddress("cl_m1_eta_35gam", &cl_m1_eta_35gam, &b_cl_m1_eta_35gam); }
  if( fChain->GetBranchStatus("cl_m1_phi_35gam") ) { fChain->SetBranchAddress("cl_m1_phi_35gam", &cl_m1_phi_35gam, &b_cl_m1_phi_35gam); }
  if( fChain->GetBranchStatus("cl_m2_r_35gam") ) { fChain->SetBranchAddress("cl_m2_r_35gam", &cl_m2_r_35gam, &b_cl_m2_r_35gam); }
  if( fChain->GetBranchStatus("cl_m2_lambda_35gam") ) { fChain->SetBranchAddress("cl_m2_lambda_35gam", &cl_m2_lambda_35gam, &b_cl_m2_lambda_35gam); }
  if( fChain->GetBranchStatus("cl_delta_phi_35gam") ) { fChain->SetBranchAddress("cl_delta_phi_35gam", &cl_delta_phi_35gam, &b_cl_delta_phi_35gam); }
  if( fChain->GetBranchStatus("cl_delta_theta_35gam") ) { fChain->SetBranchAddress("cl_delta_theta_35gam", &cl_delta_theta_35gam, &b_cl_delta_theta_35gam); }
  if( fChain->GetBranchStatus("cl_delta_alpha_35gam") ) { fChain->SetBranchAddress("cl_delta_alpha_35gam", &cl_delta_alpha_35gam, &b_cl_delta_alpha_35gam); }
  if( fChain->GetBranchStatus("cl_center_x_35gam") ) { fChain->SetBranchAddress("cl_center_x_35gam", &cl_center_x_35gam, &b_cl_center_x_35gam); }
  if( fChain->GetBranchStatus("cl_center_y_35gam") ) { fChain->SetBranchAddress("cl_center_y_35gam", &cl_center_y_35gam, &b_cl_center_y_35gam); }
  if( fChain->GetBranchStatus("cl_center_z_35gam") ) { fChain->SetBranchAddress("cl_center_z_35gam", &cl_center_z_35gam, &b_cl_center_z_35gam); }
  if( fChain->GetBranchStatus("cl_center_lambda_35gam") ) { fChain->SetBranchAddress("cl_center_lambda_35gam", &cl_center_lambda_35gam, &b_cl_center_lambda_35gam); }
  if( fChain->GetBranchStatus("cl_lateral_35gam") ) { fChain->SetBranchAddress("cl_lateral_35gam", &cl_lateral_35gam, &b_cl_lateral_35gam); }
  if( fChain->GetBranchStatus("cl_longitudinal_35gam") ) { fChain->SetBranchAddress("cl_longitudinal_35gam", &cl_longitudinal_35gam, &b_cl_longitudinal_35gam); }
  if( fChain->GetBranchStatus("cl_eng_frac_em_35gam") ) { fChain->SetBranchAddress("cl_eng_frac_em_35gam", &cl_eng_frac_em_35gam, &b_cl_eng_frac_em_35gam); }
  if( fChain->GetBranchStatus("cl_eng_frac_max_35gam") ) { fChain->SetBranchAddress("cl_eng_frac_max_35gam", &cl_eng_frac_max_35gam, &b_cl_eng_frac_max_35gam); }
  if( fChain->GetBranchStatus("cl_eng_frac_core_35gam") ) { fChain->SetBranchAddress("cl_eng_frac_core_35gam", &cl_eng_frac_core_35gam, &b_cl_eng_frac_core_35gam); }
  if( fChain->GetBranchStatus("cl_m1_dens_35gam") ) { fChain->SetBranchAddress("cl_m1_dens_35gam", &cl_m1_dens_35gam, &b_cl_m1_dens_35gam); }
  if( fChain->GetBranchStatus("cl_m2_dens_35gam") ) { fChain->SetBranchAddress("cl_m2_dens_35gam", &cl_m2_dens_35gam, &b_cl_m2_dens_35gam); }
  if( fChain->GetBranchStatus("cl_ntotcells_35gam") ) { fChain->SetBranchAddress("cl_ntotcells_35gam", &cl_ntotcells_35gam, &b_cl_ntotcells_35gam); }
  if( fChain->GetBranchStatus("cl_ecluster_37") ) { fChain->SetBranchAddress("cl_ecluster_37", &cl_ecluster_37, &b_cl_ecluster_37); }
  if( fChain->GetBranchStatus("cl_nctotal_37") ) { fChain->SetBranchAddress("cl_nctotal_37", &cl_nctotal_37, &b_cl_nctotal_37); }
  if( fChain->GetBranchStatus("cl_nc_37") ) { fChain->SetBranchAddress("cl_nc_37", &cl_nc_37, &b_cl_nc_37); }
  if( fChain->GetBranchStatus("cl_et_37") ) { fChain->SetBranchAddress("cl_et_37", &cl_et_37, &b_cl_et_37); }
  if( fChain->GetBranchStatus("cl_e_37") ) { fChain->SetBranchAddress("cl_e_37", &cl_e_37, &b_cl_e_37); }
  if( fChain->GetBranchStatus("cl_eemb0_37") ) { fChain->SetBranchAddress("cl_eemb0_37", &cl_eemb0_37, &b_cl_eemb0_37); }
  if( fChain->GetBranchStatus("cl_eemb1_37") ) { fChain->SetBranchAddress("cl_eemb1_37", &cl_eemb1_37, &b_cl_eemb1_37); }
  if( fChain->GetBranchStatus("cl_eemb2_37") ) { fChain->SetBranchAddress("cl_eemb2_37", &cl_eemb2_37, &b_cl_eemb2_37); }
  if( fChain->GetBranchStatus("cl_eemb3_37") ) { fChain->SetBranchAddress("cl_eemb3_37", &cl_eemb3_37, &b_cl_eemb3_37); }
  if( fChain->GetBranchStatus("cl_nemb0_37") ) { fChain->SetBranchAddress("cl_nemb0_37", &cl_nemb0_37, &b_cl_nemb0_37); }
  if( fChain->GetBranchStatus("cl_nemb1_37") ) { fChain->SetBranchAddress("cl_nemb1_37", &cl_nemb1_37, &b_cl_nemb1_37); }
  if( fChain->GetBranchStatus("cl_nemb2_37") ) { fChain->SetBranchAddress("cl_nemb2_37", &cl_nemb2_37, &b_cl_nemb2_37); }
  if( fChain->GetBranchStatus("cl_nemb3_37") ) { fChain->SetBranchAddress("cl_nemb3_37", &cl_nemb3_37, &b_cl_nemb3_37); }
  if( fChain->GetBranchStatus("cl_phi2_37") ) { fChain->SetBranchAddress("cl_phi2_37", &cl_phi2_37, &b_cl_phi2_37); }
  if( fChain->GetBranchStatus("cl_eta0_37") ) { fChain->SetBranchAddress("cl_eta0_37", &cl_eta0_37, &b_cl_eta0_37); }
  if( fChain->GetBranchStatus("cl_eta1_37") ) { fChain->SetBranchAddress("cl_eta1_37", &cl_eta1_37, &b_cl_eta1_37); }
  if( fChain->GetBranchStatus("cl_eta2_37") ) { fChain->SetBranchAddress("cl_eta2_37", &cl_eta2_37, &b_cl_eta2_37); }
  if( fChain->GetBranchStatus("cl_eta3_37") ) { fChain->SetBranchAddress("cl_eta3_37", &cl_eta3_37, &b_cl_eta3_37); }
  if( fChain->GetBranchStatus("cl_eeme0_37") ) { fChain->SetBranchAddress("cl_eeme0_37", &cl_eeme0_37, &b_cl_eeme0_37); }
  if( fChain->GetBranchStatus("cl_eeme1_37") ) { fChain->SetBranchAddress("cl_eeme1_37", &cl_eeme1_37, &b_cl_eeme1_37); }
  if( fChain->GetBranchStatus("cl_eeme2_37") ) { fChain->SetBranchAddress("cl_eeme2_37", &cl_eeme2_37, &b_cl_eeme2_37); }
  if( fChain->GetBranchStatus("cl_eeme3_37") ) { fChain->SetBranchAddress("cl_eeme3_37", &cl_eeme3_37, &b_cl_eeme3_37); }
  if( fChain->GetBranchStatus("cl_neme0_37") ) { fChain->SetBranchAddress("cl_neme0_37", &cl_neme0_37, &b_cl_neme0_37); }
  if( fChain->GetBranchStatus("cl_neme1_37") ) { fChain->SetBranchAddress("cl_neme1_37", &cl_neme1_37, &b_cl_neme1_37); }
  if( fChain->GetBranchStatus("cl_neme2_37") ) { fChain->SetBranchAddress("cl_neme2_37", &cl_neme2_37, &b_cl_neme2_37); }
  if( fChain->GetBranchStatus("cl_neme3_37") ) { fChain->SetBranchAddress("cl_neme3_37", &cl_neme3_37, &b_cl_neme3_37); }
  if( fChain->GetBranchStatus("cl_etileg1_37") ) { fChain->SetBranchAddress("cl_etileg1_37", &cl_etileg1_37, &b_cl_etileg1_37); }
  if( fChain->GetBranchStatus("cl_etileg2_37") ) { fChain->SetBranchAddress("cl_etileg2_37", &cl_etileg2_37, &b_cl_etileg2_37); }
  if( fChain->GetBranchStatus("cl_etileg3_37") ) { fChain->SetBranchAddress("cl_etileg3_37", &cl_etileg3_37, &b_cl_etileg3_37); }
  if( fChain->GetBranchStatus("cl_ntileg1_37") ) { fChain->SetBranchAddress("cl_ntileg1_37", &cl_ntileg1_37, &b_cl_ntileg1_37); }
  if( fChain->GetBranchStatus("cl_ntileg2_37") ) { fChain->SetBranchAddress("cl_ntileg2_37", &cl_ntileg2_37, &b_cl_ntileg2_37); }
  if( fChain->GetBranchStatus("cl_ntileg3_37") ) { fChain->SetBranchAddress("cl_ntileg3_37", &cl_ntileg3_37, &b_cl_ntileg3_37); }
  if( fChain->GetBranchStatus("cl_eta_37") ) { fChain->SetBranchAddress("cl_eta_37", &cl_eta_37, &b_cl_eta_37); }
  if( fChain->GetBranchStatus("cl_phi_37") ) { fChain->SetBranchAddress("cl_phi_37", &cl_phi_37, &b_cl_phi_37); }
  if( fChain->GetBranchStatus("cl_time_37") ) { fChain->SetBranchAddress("cl_time_37", &cl_time_37, &b_cl_time_37); }
  if( fChain->GetBranchStatus("cl_reco_stat_37") ) { fChain->SetBranchAddress("cl_reco_stat_37", &cl_reco_stat_37, &b_cl_reco_stat_37); }
  if( fChain->GetBranchStatus("cl_m1_eta_37") ) { fChain->SetBranchAddress("cl_m1_eta_37", &cl_m1_eta_37, &b_cl_m1_eta_37); }
  if( fChain->GetBranchStatus("cl_m1_phi_37") ) { fChain->SetBranchAddress("cl_m1_phi_37", &cl_m1_phi_37, &b_cl_m1_phi_37); }
  if( fChain->GetBranchStatus("cl_m2_r_37") ) { fChain->SetBranchAddress("cl_m2_r_37", &cl_m2_r_37, &b_cl_m2_r_37); }
  if( fChain->GetBranchStatus("cl_m2_lambda_37") ) { fChain->SetBranchAddress("cl_m2_lambda_37", &cl_m2_lambda_37, &b_cl_m2_lambda_37); }
  if( fChain->GetBranchStatus("cl_delta_phi_37") ) { fChain->SetBranchAddress("cl_delta_phi_37", &cl_delta_phi_37, &b_cl_delta_phi_37); }
  if( fChain->GetBranchStatus("cl_delta_theta_37") ) { fChain->SetBranchAddress("cl_delta_theta_37", &cl_delta_theta_37, &b_cl_delta_theta_37); }
  if( fChain->GetBranchStatus("cl_delta_alpha_37") ) { fChain->SetBranchAddress("cl_delta_alpha_37", &cl_delta_alpha_37, &b_cl_delta_alpha_37); }
  if( fChain->GetBranchStatus("cl_center_x_37") ) { fChain->SetBranchAddress("cl_center_x_37", &cl_center_x_37, &b_cl_center_x_37); }
  if( fChain->GetBranchStatus("cl_center_y_37") ) { fChain->SetBranchAddress("cl_center_y_37", &cl_center_y_37, &b_cl_center_y_37); }
  if( fChain->GetBranchStatus("cl_center_z_37") ) { fChain->SetBranchAddress("cl_center_z_37", &cl_center_z_37, &b_cl_center_z_37); }
  if( fChain->GetBranchStatus("cl_center_lambda_37") ) { fChain->SetBranchAddress("cl_center_lambda_37", &cl_center_lambda_37, &b_cl_center_lambda_37); }
  if( fChain->GetBranchStatus("cl_lateral_37") ) { fChain->SetBranchAddress("cl_lateral_37", &cl_lateral_37, &b_cl_lateral_37); }
  if( fChain->GetBranchStatus("cl_longitudinal_37") ) { fChain->SetBranchAddress("cl_longitudinal_37", &cl_longitudinal_37, &b_cl_longitudinal_37); }
  if( fChain->GetBranchStatus("cl_eng_frac_em_37") ) { fChain->SetBranchAddress("cl_eng_frac_em_37", &cl_eng_frac_em_37, &b_cl_eng_frac_em_37); }
  if( fChain->GetBranchStatus("cl_eng_frac_max_37") ) { fChain->SetBranchAddress("cl_eng_frac_max_37", &cl_eng_frac_max_37, &b_cl_eng_frac_max_37); }
  if( fChain->GetBranchStatus("cl_eng_frac_core_37") ) { fChain->SetBranchAddress("cl_eng_frac_core_37", &cl_eng_frac_core_37, &b_cl_eng_frac_core_37); }
  if( fChain->GetBranchStatus("cl_m1_dens_37") ) { fChain->SetBranchAddress("cl_m1_dens_37", &cl_m1_dens_37, &b_cl_m1_dens_37); }
  if( fChain->GetBranchStatus("cl_m2_dens_37") ) { fChain->SetBranchAddress("cl_m2_dens_37", &cl_m2_dens_37, &b_cl_m2_dens_37); }
  if( fChain->GetBranchStatus("cl_ntotcells_37") ) { fChain->SetBranchAddress("cl_ntotcells_37", &cl_ntotcells_37, &b_cl_ntotcells_37); }
  if( fChain->GetBranchStatus("cl_ecluster_37gam") ) { fChain->SetBranchAddress("cl_ecluster_37gam", &cl_ecluster_37gam, &b_cl_ecluster_37gam); }
  if( fChain->GetBranchStatus("cl_nctotal_37gam") ) { fChain->SetBranchAddress("cl_nctotal_37gam", &cl_nctotal_37gam, &b_cl_nctotal_37gam); }
  if( fChain->GetBranchStatus("cl_nc_37gam") ) { fChain->SetBranchAddress("cl_nc_37gam", &cl_nc_37gam, &b_cl_nc_37gam); }
  if( fChain->GetBranchStatus("cl_et_37gam") ) { fChain->SetBranchAddress("cl_et_37gam", &cl_et_37gam, &b_cl_et_37gam); }
  if( fChain->GetBranchStatus("cl_e_37gam") ) { fChain->SetBranchAddress("cl_e_37gam", &cl_e_37gam, &b_cl_e_37gam); }
  if( fChain->GetBranchStatus("cl_eemb0_37gam") ) { fChain->SetBranchAddress("cl_eemb0_37gam", &cl_eemb0_37gam, &b_cl_eemb0_37gam); }
  if( fChain->GetBranchStatus("cl_eemb1_37gam") ) { fChain->SetBranchAddress("cl_eemb1_37gam", &cl_eemb1_37gam, &b_cl_eemb1_37gam); }
  if( fChain->GetBranchStatus("cl_eemb2_37gam") ) { fChain->SetBranchAddress("cl_eemb2_37gam", &cl_eemb2_37gam, &b_cl_eemb2_37gam); }
  if( fChain->GetBranchStatus("cl_eemb3_37gam") ) { fChain->SetBranchAddress("cl_eemb3_37gam", &cl_eemb3_37gam, &b_cl_eemb3_37gam); }
  if( fChain->GetBranchStatus("cl_nemb0_37gam") ) { fChain->SetBranchAddress("cl_nemb0_37gam", &cl_nemb0_37gam, &b_cl_nemb0_37gam); }
  if( fChain->GetBranchStatus("cl_nemb1_37gam") ) { fChain->SetBranchAddress("cl_nemb1_37gam", &cl_nemb1_37gam, &b_cl_nemb1_37gam); }
  if( fChain->GetBranchStatus("cl_nemb2_37gam") ) { fChain->SetBranchAddress("cl_nemb2_37gam", &cl_nemb2_37gam, &b_cl_nemb2_37gam); }
  if( fChain->GetBranchStatus("cl_nemb3_37gam") ) { fChain->SetBranchAddress("cl_nemb3_37gam", &cl_nemb3_37gam, &b_cl_nemb3_37gam); }
  if( fChain->GetBranchStatus("cl_phi2_37gam") ) { fChain->SetBranchAddress("cl_phi2_37gam", &cl_phi2_37gam, &b_cl_phi2_37gam); }
  if( fChain->GetBranchStatus("cl_eta0_37gam") ) { fChain->SetBranchAddress("cl_eta0_37gam", &cl_eta0_37gam, &b_cl_eta0_37gam); }
  if( fChain->GetBranchStatus("cl_eta1_37gam") ) { fChain->SetBranchAddress("cl_eta1_37gam", &cl_eta1_37gam, &b_cl_eta1_37gam); }
  if( fChain->GetBranchStatus("cl_eta2_37gam") ) { fChain->SetBranchAddress("cl_eta2_37gam", &cl_eta2_37gam, &b_cl_eta2_37gam); }
  if( fChain->GetBranchStatus("cl_eta3_37gam") ) { fChain->SetBranchAddress("cl_eta3_37gam", &cl_eta3_37gam, &b_cl_eta3_37gam); }
  if( fChain->GetBranchStatus("cl_eeme0_37gam") ) { fChain->SetBranchAddress("cl_eeme0_37gam", &cl_eeme0_37gam, &b_cl_eeme0_37gam); }
  if( fChain->GetBranchStatus("cl_eeme1_37gam") ) { fChain->SetBranchAddress("cl_eeme1_37gam", &cl_eeme1_37gam, &b_cl_eeme1_37gam); }
  if( fChain->GetBranchStatus("cl_eeme2_37gam") ) { fChain->SetBranchAddress("cl_eeme2_37gam", &cl_eeme2_37gam, &b_cl_eeme2_37gam); }
  if( fChain->GetBranchStatus("cl_eeme3_37gam") ) { fChain->SetBranchAddress("cl_eeme3_37gam", &cl_eeme3_37gam, &b_cl_eeme3_37gam); }
  if( fChain->GetBranchStatus("cl_neme0_37gam") ) { fChain->SetBranchAddress("cl_neme0_37gam", &cl_neme0_37gam, &b_cl_neme0_37gam); }
  if( fChain->GetBranchStatus("cl_neme1_37gam") ) { fChain->SetBranchAddress("cl_neme1_37gam", &cl_neme1_37gam, &b_cl_neme1_37gam); }
  if( fChain->GetBranchStatus("cl_neme2_37gam") ) { fChain->SetBranchAddress("cl_neme2_37gam", &cl_neme2_37gam, &b_cl_neme2_37gam); }
  if( fChain->GetBranchStatus("cl_neme3_37gam") ) { fChain->SetBranchAddress("cl_neme3_37gam", &cl_neme3_37gam, &b_cl_neme3_37gam); }
  if( fChain->GetBranchStatus("cl_etileg1_37gam") ) { fChain->SetBranchAddress("cl_etileg1_37gam", &cl_etileg1_37gam, &b_cl_etileg1_37gam); }
  if( fChain->GetBranchStatus("cl_etileg2_37gam") ) { fChain->SetBranchAddress("cl_etileg2_37gam", &cl_etileg2_37gam, &b_cl_etileg2_37gam); }
  if( fChain->GetBranchStatus("cl_etileg3_37gam") ) { fChain->SetBranchAddress("cl_etileg3_37gam", &cl_etileg3_37gam, &b_cl_etileg3_37gam); }
  if( fChain->GetBranchStatus("cl_ntileg1_37gam") ) { fChain->SetBranchAddress("cl_ntileg1_37gam", &cl_ntileg1_37gam, &b_cl_ntileg1_37gam); }
  if( fChain->GetBranchStatus("cl_ntileg2_37gam") ) { fChain->SetBranchAddress("cl_ntileg2_37gam", &cl_ntileg2_37gam, &b_cl_ntileg2_37gam); }
  if( fChain->GetBranchStatus("cl_ntileg3_37gam") ) { fChain->SetBranchAddress("cl_ntileg3_37gam", &cl_ntileg3_37gam, &b_cl_ntileg3_37gam); }
  if( fChain->GetBranchStatus("cl_eta_37gam") ) { fChain->SetBranchAddress("cl_eta_37gam", &cl_eta_37gam, &b_cl_eta_37gam); }
  if( fChain->GetBranchStatus("cl_phi_37gam") ) { fChain->SetBranchAddress("cl_phi_37gam", &cl_phi_37gam, &b_cl_phi_37gam); }
  if( fChain->GetBranchStatus("cl_time_37gam") ) { fChain->SetBranchAddress("cl_time_37gam", &cl_time_37gam, &b_cl_time_37gam); }
  if( fChain->GetBranchStatus("cl_reco_stat_37gam") ) { fChain->SetBranchAddress("cl_reco_stat_37gam", &cl_reco_stat_37gam, &b_cl_reco_stat_37gam); }
  if( fChain->GetBranchStatus("cl_m1_eta_37gam") ) { fChain->SetBranchAddress("cl_m1_eta_37gam", &cl_m1_eta_37gam, &b_cl_m1_eta_37gam); }
  if( fChain->GetBranchStatus("cl_m1_phi_37gam") ) { fChain->SetBranchAddress("cl_m1_phi_37gam", &cl_m1_phi_37gam, &b_cl_m1_phi_37gam); }
  if( fChain->GetBranchStatus("cl_m2_r_37gam") ) { fChain->SetBranchAddress("cl_m2_r_37gam", &cl_m2_r_37gam, &b_cl_m2_r_37gam); }
  if( fChain->GetBranchStatus("cl_m2_lambda_37gam") ) { fChain->SetBranchAddress("cl_m2_lambda_37gam", &cl_m2_lambda_37gam, &b_cl_m2_lambda_37gam); }
  if( fChain->GetBranchStatus("cl_delta_phi_37gam") ) { fChain->SetBranchAddress("cl_delta_phi_37gam", &cl_delta_phi_37gam, &b_cl_delta_phi_37gam); }
  if( fChain->GetBranchStatus("cl_delta_theta_37gam") ) { fChain->SetBranchAddress("cl_delta_theta_37gam", &cl_delta_theta_37gam, &b_cl_delta_theta_37gam); }
  if( fChain->GetBranchStatus("cl_delta_alpha_37gam") ) { fChain->SetBranchAddress("cl_delta_alpha_37gam", &cl_delta_alpha_37gam, &b_cl_delta_alpha_37gam); }
  if( fChain->GetBranchStatus("cl_center_x_37gam") ) { fChain->SetBranchAddress("cl_center_x_37gam", &cl_center_x_37gam, &b_cl_center_x_37gam); }
  if( fChain->GetBranchStatus("cl_center_y_37gam") ) { fChain->SetBranchAddress("cl_center_y_37gam", &cl_center_y_37gam, &b_cl_center_y_37gam); }
  if( fChain->GetBranchStatus("cl_center_z_37gam") ) { fChain->SetBranchAddress("cl_center_z_37gam", &cl_center_z_37gam, &b_cl_center_z_37gam); }
  if( fChain->GetBranchStatus("cl_center_lambda_37gam") ) { fChain->SetBranchAddress("cl_center_lambda_37gam", &cl_center_lambda_37gam, &b_cl_center_lambda_37gam); }
  if( fChain->GetBranchStatus("cl_lateral_37gam") ) { fChain->SetBranchAddress("cl_lateral_37gam", &cl_lateral_37gam, &b_cl_lateral_37gam); }
  if( fChain->GetBranchStatus("cl_longitudinal_37gam") ) { fChain->SetBranchAddress("cl_longitudinal_37gam", &cl_longitudinal_37gam, &b_cl_longitudinal_37gam); }
  if( fChain->GetBranchStatus("cl_eng_frac_em_37gam") ) { fChain->SetBranchAddress("cl_eng_frac_em_37gam", &cl_eng_frac_em_37gam, &b_cl_eng_frac_em_37gam); }
  if( fChain->GetBranchStatus("cl_eng_frac_max_37gam") ) { fChain->SetBranchAddress("cl_eng_frac_max_37gam", &cl_eng_frac_max_37gam, &b_cl_eng_frac_max_37gam); }
  if( fChain->GetBranchStatus("cl_eng_frac_core_37gam") ) { fChain->SetBranchAddress("cl_eng_frac_core_37gam", &cl_eng_frac_core_37gam, &b_cl_eng_frac_core_37gam); }
  if( fChain->GetBranchStatus("cl_m1_dens_37gam") ) { fChain->SetBranchAddress("cl_m1_dens_37gam", &cl_m1_dens_37gam, &b_cl_m1_dens_37gam); }
  if( fChain->GetBranchStatus("cl_m2_dens_37gam") ) { fChain->SetBranchAddress("cl_m2_dens_37gam", &cl_m2_dens_37gam, &b_cl_m2_dens_37gam); }
  if( fChain->GetBranchStatus("cl_ntotcells_37gam") ) { fChain->SetBranchAddress("cl_ntotcells_37gam", &cl_ntotcells_37gam, &b_cl_ntotcells_37gam); }
  if( fChain->GetBranchStatus("cl_ecluster_Softe") ) { fChain->SetBranchAddress("cl_ecluster_Softe", &cl_ecluster_Softe, &b_cl_ecluster_Softe); }
  if( fChain->GetBranchStatus("cl_nctotal_Softe") ) { fChain->SetBranchAddress("cl_nctotal_Softe", &cl_nctotal_Softe, &b_cl_nctotal_Softe); }
  if( fChain->GetBranchStatus("cl_nc_Softe") ) { fChain->SetBranchAddress("cl_nc_Softe", &cl_nc_Softe, &b_cl_nc_Softe); }
  if( fChain->GetBranchStatus("cl_et_Softe") ) { fChain->SetBranchAddress("cl_et_Softe", &cl_et_Softe, &b_cl_et_Softe); }
  if( fChain->GetBranchStatus("cl_e_Softe") ) { fChain->SetBranchAddress("cl_e_Softe", &cl_e_Softe, &b_cl_e_Softe); }
  if( fChain->GetBranchStatus("cl_eemb0_Softe") ) { fChain->SetBranchAddress("cl_eemb0_Softe", &cl_eemb0_Softe, &b_cl_eemb0_Softe); }
  if( fChain->GetBranchStatus("cl_eemb1_Softe") ) { fChain->SetBranchAddress("cl_eemb1_Softe", &cl_eemb1_Softe, &b_cl_eemb1_Softe); }
  if( fChain->GetBranchStatus("cl_eemb2_Softe") ) { fChain->SetBranchAddress("cl_eemb2_Softe", &cl_eemb2_Softe, &b_cl_eemb2_Softe); }
  if( fChain->GetBranchStatus("cl_eemb3_Softe") ) { fChain->SetBranchAddress("cl_eemb3_Softe", &cl_eemb3_Softe, &b_cl_eemb3_Softe); }
  if( fChain->GetBranchStatus("cl_nemb0_Softe") ) { fChain->SetBranchAddress("cl_nemb0_Softe", &cl_nemb0_Softe, &b_cl_nemb0_Softe); }
  if( fChain->GetBranchStatus("cl_nemb1_Softe") ) { fChain->SetBranchAddress("cl_nemb1_Softe", &cl_nemb1_Softe, &b_cl_nemb1_Softe); }
  if( fChain->GetBranchStatus("cl_nemb2_Softe") ) { fChain->SetBranchAddress("cl_nemb2_Softe", &cl_nemb2_Softe, &b_cl_nemb2_Softe); }
  if( fChain->GetBranchStatus("cl_nemb3_Softe") ) { fChain->SetBranchAddress("cl_nemb3_Softe", &cl_nemb3_Softe, &b_cl_nemb3_Softe); }
  if( fChain->GetBranchStatus("cl_phi2_Softe") ) { fChain->SetBranchAddress("cl_phi2_Softe", &cl_phi2_Softe, &b_cl_phi2_Softe); }
  if( fChain->GetBranchStatus("cl_eta0_Softe") ) { fChain->SetBranchAddress("cl_eta0_Softe", &cl_eta0_Softe, &b_cl_eta0_Softe); }
  if( fChain->GetBranchStatus("cl_eta1_Softe") ) { fChain->SetBranchAddress("cl_eta1_Softe", &cl_eta1_Softe, &b_cl_eta1_Softe); }
  if( fChain->GetBranchStatus("cl_eta2_Softe") ) { fChain->SetBranchAddress("cl_eta2_Softe", &cl_eta2_Softe, &b_cl_eta2_Softe); }
  if( fChain->GetBranchStatus("cl_eta3_Softe") ) { fChain->SetBranchAddress("cl_eta3_Softe", &cl_eta3_Softe, &b_cl_eta3_Softe); }
  if( fChain->GetBranchStatus("cl_eeme0_Softe") ) { fChain->SetBranchAddress("cl_eeme0_Softe", &cl_eeme0_Softe, &b_cl_eeme0_Softe); }
  if( fChain->GetBranchStatus("cl_eeme1_Softe") ) { fChain->SetBranchAddress("cl_eeme1_Softe", &cl_eeme1_Softe, &b_cl_eeme1_Softe); }
  if( fChain->GetBranchStatus("cl_eeme2_Softe") ) { fChain->SetBranchAddress("cl_eeme2_Softe", &cl_eeme2_Softe, &b_cl_eeme2_Softe); }
  if( fChain->GetBranchStatus("cl_eeme3_Softe") ) { fChain->SetBranchAddress("cl_eeme3_Softe", &cl_eeme3_Softe, &b_cl_eeme3_Softe); }
  if( fChain->GetBranchStatus("cl_neme0_Softe") ) { fChain->SetBranchAddress("cl_neme0_Softe", &cl_neme0_Softe, &b_cl_neme0_Softe); }
  if( fChain->GetBranchStatus("cl_neme1_Softe") ) { fChain->SetBranchAddress("cl_neme1_Softe", &cl_neme1_Softe, &b_cl_neme1_Softe); }
  if( fChain->GetBranchStatus("cl_neme2_Softe") ) { fChain->SetBranchAddress("cl_neme2_Softe", &cl_neme2_Softe, &b_cl_neme2_Softe); }
  if( fChain->GetBranchStatus("cl_neme3_Softe") ) { fChain->SetBranchAddress("cl_neme3_Softe", &cl_neme3_Softe, &b_cl_neme3_Softe); }
  if( fChain->GetBranchStatus("cl_etileg1_Softe") ) { fChain->SetBranchAddress("cl_etileg1_Softe", &cl_etileg1_Softe, &b_cl_etileg1_Softe); }
  if( fChain->GetBranchStatus("cl_etileg2_Softe") ) { fChain->SetBranchAddress("cl_etileg2_Softe", &cl_etileg2_Softe, &b_cl_etileg2_Softe); }
  if( fChain->GetBranchStatus("cl_etileg3_Softe") ) { fChain->SetBranchAddress("cl_etileg3_Softe", &cl_etileg3_Softe, &b_cl_etileg3_Softe); }
  if( fChain->GetBranchStatus("cl_ntileg1_Softe") ) { fChain->SetBranchAddress("cl_ntileg1_Softe", &cl_ntileg1_Softe, &b_cl_ntileg1_Softe); }
  if( fChain->GetBranchStatus("cl_ntileg2_Softe") ) { fChain->SetBranchAddress("cl_ntileg2_Softe", &cl_ntileg2_Softe, &b_cl_ntileg2_Softe); }
  if( fChain->GetBranchStatus("cl_ntileg3_Softe") ) { fChain->SetBranchAddress("cl_ntileg3_Softe", &cl_ntileg3_Softe, &b_cl_ntileg3_Softe); }
  if( fChain->GetBranchStatus("cl_eta_Softe") ) { fChain->SetBranchAddress("cl_eta_Softe", &cl_eta_Softe, &b_cl_eta_Softe); }
  if( fChain->GetBranchStatus("cl_phi_Softe") ) { fChain->SetBranchAddress("cl_phi_Softe", &cl_phi_Softe, &b_cl_phi_Softe); }
  if( fChain->GetBranchStatus("cl_time_Softe") ) { fChain->SetBranchAddress("cl_time_Softe", &cl_time_Softe, &b_cl_time_Softe); }
  if( fChain->GetBranchStatus("cl_reco_stat_Softe") ) { fChain->SetBranchAddress("cl_reco_stat_Softe", &cl_reco_stat_Softe, &b_cl_reco_stat_Softe); }
  if( fChain->GetBranchStatus("cl_m1_eta_Softe") ) { fChain->SetBranchAddress("cl_m1_eta_Softe", &cl_m1_eta_Softe, &b_cl_m1_eta_Softe); }
  if( fChain->GetBranchStatus("cl_m1_phi_Softe") ) { fChain->SetBranchAddress("cl_m1_phi_Softe", &cl_m1_phi_Softe, &b_cl_m1_phi_Softe); }
  if( fChain->GetBranchStatus("cl_m2_r_Softe") ) { fChain->SetBranchAddress("cl_m2_r_Softe", &cl_m2_r_Softe, &b_cl_m2_r_Softe); }
  if( fChain->GetBranchStatus("cl_m2_lambda_Softe") ) { fChain->SetBranchAddress("cl_m2_lambda_Softe", &cl_m2_lambda_Softe, &b_cl_m2_lambda_Softe); }
  if( fChain->GetBranchStatus("cl_delta_phi_Softe") ) { fChain->SetBranchAddress("cl_delta_phi_Softe", &cl_delta_phi_Softe, &b_cl_delta_phi_Softe); }
  if( fChain->GetBranchStatus("cl_delta_theta_Softe") ) { fChain->SetBranchAddress("cl_delta_theta_Softe", &cl_delta_theta_Softe, &b_cl_delta_theta_Softe); }
  if( fChain->GetBranchStatus("cl_delta_alpha_Softe") ) { fChain->SetBranchAddress("cl_delta_alpha_Softe", &cl_delta_alpha_Softe, &b_cl_delta_alpha_Softe); }
  if( fChain->GetBranchStatus("cl_center_x_Softe") ) { fChain->SetBranchAddress("cl_center_x_Softe", &cl_center_x_Softe, &b_cl_center_x_Softe); }
  if( fChain->GetBranchStatus("cl_center_y_Softe") ) { fChain->SetBranchAddress("cl_center_y_Softe", &cl_center_y_Softe, &b_cl_center_y_Softe); }
  if( fChain->GetBranchStatus("cl_center_z_Softe") ) { fChain->SetBranchAddress("cl_center_z_Softe", &cl_center_z_Softe, &b_cl_center_z_Softe); }
  if( fChain->GetBranchStatus("cl_center_lambda_Softe") ) { fChain->SetBranchAddress("cl_center_lambda_Softe", &cl_center_lambda_Softe, &b_cl_center_lambda_Softe); }
  if( fChain->GetBranchStatus("cl_lateral_Softe") ) { fChain->SetBranchAddress("cl_lateral_Softe", &cl_lateral_Softe, &b_cl_lateral_Softe); }
  if( fChain->GetBranchStatus("cl_longitudinal_Softe") ) { fChain->SetBranchAddress("cl_longitudinal_Softe", &cl_longitudinal_Softe, &b_cl_longitudinal_Softe); }
  if( fChain->GetBranchStatus("cl_eng_frac_em_Softe") ) { fChain->SetBranchAddress("cl_eng_frac_em_Softe", &cl_eng_frac_em_Softe, &b_cl_eng_frac_em_Softe); }
  if( fChain->GetBranchStatus("cl_eng_frac_max_Softe") ) { fChain->SetBranchAddress("cl_eng_frac_max_Softe", &cl_eng_frac_max_Softe, &b_cl_eng_frac_max_Softe); }
  if( fChain->GetBranchStatus("cl_eng_frac_core_Softe") ) { fChain->SetBranchAddress("cl_eng_frac_core_Softe", &cl_eng_frac_core_Softe, &b_cl_eng_frac_core_Softe); }
  if( fChain->GetBranchStatus("cl_m1_dens_Softe") ) { fChain->SetBranchAddress("cl_m1_dens_Softe", &cl_m1_dens_Softe, &b_cl_m1_dens_Softe); }
  if( fChain->GetBranchStatus("cl_m2_dens_Softe") ) { fChain->SetBranchAddress("cl_m2_dens_Softe", &cl_m2_dens_Softe, &b_cl_m2_dens_Softe); }
  if( fChain->GetBranchStatus("cl_ntotcells_Softe") ) { fChain->SetBranchAddress("cl_ntotcells_Softe", &cl_ntotcells_Softe, &b_cl_ntotcells_Softe); }
  if( fChain->GetBranchStatus("cl_ecluster_comb") ) { fChain->SetBranchAddress("cl_ecluster_comb", &cl_ecluster_comb, &b_cl_ecluster_comb); }
  if( fChain->GetBranchStatus("cl_nctotal_comb") ) { fChain->SetBranchAddress("cl_nctotal_comb", &cl_nctotal_comb, &b_cl_nctotal_comb); }
  if( fChain->GetBranchStatus("cl_nc_comb") ) { fChain->SetBranchAddress("cl_nc_comb", &cl_nc_comb, &b_cl_nc_comb); }
  if( fChain->GetBranchStatus("cl_et_comb") ) { fChain->SetBranchAddress("cl_et_comb", &cl_et_comb, &b_cl_et_comb); }
  if( fChain->GetBranchStatus("cl_e_comb") ) { fChain->SetBranchAddress("cl_e_comb", &cl_e_comb, &b_cl_e_comb); }
  if( fChain->GetBranchStatus("cl_eemb0_comb") ) { fChain->SetBranchAddress("cl_eemb0_comb", &cl_eemb0_comb, &b_cl_eemb0_comb); }
  if( fChain->GetBranchStatus("cl_eemb1_comb") ) { fChain->SetBranchAddress("cl_eemb1_comb", &cl_eemb1_comb, &b_cl_eemb1_comb); }
  if( fChain->GetBranchStatus("cl_eemb2_comb") ) { fChain->SetBranchAddress("cl_eemb2_comb", &cl_eemb2_comb, &b_cl_eemb2_comb); }
  if( fChain->GetBranchStatus("cl_eemb3_comb") ) { fChain->SetBranchAddress("cl_eemb3_comb", &cl_eemb3_comb, &b_cl_eemb3_comb); }
  if( fChain->GetBranchStatus("cl_nemb0_comb") ) { fChain->SetBranchAddress("cl_nemb0_comb", &cl_nemb0_comb, &b_cl_nemb0_comb); }
  if( fChain->GetBranchStatus("cl_nemb1_comb") ) { fChain->SetBranchAddress("cl_nemb1_comb", &cl_nemb1_comb, &b_cl_nemb1_comb); }
  if( fChain->GetBranchStatus("cl_nemb2_comb") ) { fChain->SetBranchAddress("cl_nemb2_comb", &cl_nemb2_comb, &b_cl_nemb2_comb); }
  if( fChain->GetBranchStatus("cl_nemb3_comb") ) { fChain->SetBranchAddress("cl_nemb3_comb", &cl_nemb3_comb, &b_cl_nemb3_comb); }
  if( fChain->GetBranchStatus("cl_phi2_comb") ) { fChain->SetBranchAddress("cl_phi2_comb", &cl_phi2_comb, &b_cl_phi2_comb); }
  if( fChain->GetBranchStatus("cl_eta0_comb") ) { fChain->SetBranchAddress("cl_eta0_comb", &cl_eta0_comb, &b_cl_eta0_comb); }
  if( fChain->GetBranchStatus("cl_eta1_comb") ) { fChain->SetBranchAddress("cl_eta1_comb", &cl_eta1_comb, &b_cl_eta1_comb); }
  if( fChain->GetBranchStatus("cl_eta2_comb") ) { fChain->SetBranchAddress("cl_eta2_comb", &cl_eta2_comb, &b_cl_eta2_comb); }
  if( fChain->GetBranchStatus("cl_eta3_comb") ) { fChain->SetBranchAddress("cl_eta3_comb", &cl_eta3_comb, &b_cl_eta3_comb); }
  if( fChain->GetBranchStatus("cl_eeme0_comb") ) { fChain->SetBranchAddress("cl_eeme0_comb", &cl_eeme0_comb, &b_cl_eeme0_comb); }
  if( fChain->GetBranchStatus("cl_eeme1_comb") ) { fChain->SetBranchAddress("cl_eeme1_comb", &cl_eeme1_comb, &b_cl_eeme1_comb); }
  if( fChain->GetBranchStatus("cl_eeme2_comb") ) { fChain->SetBranchAddress("cl_eeme2_comb", &cl_eeme2_comb, &b_cl_eeme2_comb); }
  if( fChain->GetBranchStatus("cl_eeme3_comb") ) { fChain->SetBranchAddress("cl_eeme3_comb", &cl_eeme3_comb, &b_cl_eeme3_comb); }
  if( fChain->GetBranchStatus("cl_neme0_comb") ) { fChain->SetBranchAddress("cl_neme0_comb", &cl_neme0_comb, &b_cl_neme0_comb); }
  if( fChain->GetBranchStatus("cl_neme1_comb") ) { fChain->SetBranchAddress("cl_neme1_comb", &cl_neme1_comb, &b_cl_neme1_comb); }
  if( fChain->GetBranchStatus("cl_neme2_comb") ) { fChain->SetBranchAddress("cl_neme2_comb", &cl_neme2_comb, &b_cl_neme2_comb); }
  if( fChain->GetBranchStatus("cl_neme3_comb") ) { fChain->SetBranchAddress("cl_neme3_comb", &cl_neme3_comb, &b_cl_neme3_comb); }
  if( fChain->GetBranchStatus("cl_etileg1_comb") ) { fChain->SetBranchAddress("cl_etileg1_comb", &cl_etileg1_comb, &b_cl_etileg1_comb); }
  if( fChain->GetBranchStatus("cl_etileg2_comb") ) { fChain->SetBranchAddress("cl_etileg2_comb", &cl_etileg2_comb, &b_cl_etileg2_comb); }
  if( fChain->GetBranchStatus("cl_etileg3_comb") ) { fChain->SetBranchAddress("cl_etileg3_comb", &cl_etileg3_comb, &b_cl_etileg3_comb); }
  if( fChain->GetBranchStatus("cl_ntileg1_comb") ) { fChain->SetBranchAddress("cl_ntileg1_comb", &cl_ntileg1_comb, &b_cl_ntileg1_comb); }
  if( fChain->GetBranchStatus("cl_ntileg2_comb") ) { fChain->SetBranchAddress("cl_ntileg2_comb", &cl_ntileg2_comb, &b_cl_ntileg2_comb); }
  if( fChain->GetBranchStatus("cl_ntileg3_comb") ) { fChain->SetBranchAddress("cl_ntileg3_comb", &cl_ntileg3_comb, &b_cl_ntileg3_comb); }
  if( fChain->GetBranchStatus("cl_eta_comb") ) { fChain->SetBranchAddress("cl_eta_comb", &cl_eta_comb, &b_cl_eta_comb); }
  if( fChain->GetBranchStatus("cl_phi_comb") ) { fChain->SetBranchAddress("cl_phi_comb", &cl_phi_comb, &b_cl_phi_comb); }
  if( fChain->GetBranchStatus("cl_time_comb") ) { fChain->SetBranchAddress("cl_time_comb", &cl_time_comb, &b_cl_time_comb); }
  if( fChain->GetBranchStatus("cl_reco_stat_comb") ) { fChain->SetBranchAddress("cl_reco_stat_comb", &cl_reco_stat_comb, &b_cl_reco_stat_comb); }
  if( fChain->GetBranchStatus("cl_m1_eta_comb") ) { fChain->SetBranchAddress("cl_m1_eta_comb", &cl_m1_eta_comb, &b_cl_m1_eta_comb); }
  if( fChain->GetBranchStatus("cl_m1_phi_comb") ) { fChain->SetBranchAddress("cl_m1_phi_comb", &cl_m1_phi_comb, &b_cl_m1_phi_comb); }
  if( fChain->GetBranchStatus("cl_m2_r_comb") ) { fChain->SetBranchAddress("cl_m2_r_comb", &cl_m2_r_comb, &b_cl_m2_r_comb); }
  if( fChain->GetBranchStatus("cl_m2_lambda_comb") ) { fChain->SetBranchAddress("cl_m2_lambda_comb", &cl_m2_lambda_comb, &b_cl_m2_lambda_comb); }
  if( fChain->GetBranchStatus("cl_delta_phi_comb") ) { fChain->SetBranchAddress("cl_delta_phi_comb", &cl_delta_phi_comb, &b_cl_delta_phi_comb); }
  if( fChain->GetBranchStatus("cl_delta_theta_comb") ) { fChain->SetBranchAddress("cl_delta_theta_comb", &cl_delta_theta_comb, &b_cl_delta_theta_comb); }
  if( fChain->GetBranchStatus("cl_delta_alpha_comb") ) { fChain->SetBranchAddress("cl_delta_alpha_comb", &cl_delta_alpha_comb, &b_cl_delta_alpha_comb); }
  if( fChain->GetBranchStatus("cl_center_x_comb") ) { fChain->SetBranchAddress("cl_center_x_comb", &cl_center_x_comb, &b_cl_center_x_comb); }
  if( fChain->GetBranchStatus("cl_center_y_comb") ) { fChain->SetBranchAddress("cl_center_y_comb", &cl_center_y_comb, &b_cl_center_y_comb); }
  if( fChain->GetBranchStatus("cl_center_z_comb") ) { fChain->SetBranchAddress("cl_center_z_comb", &cl_center_z_comb, &b_cl_center_z_comb); }
  if( fChain->GetBranchStatus("cl_center_lambda_comb") ) { fChain->SetBranchAddress("cl_center_lambda_comb", &cl_center_lambda_comb, &b_cl_center_lambda_comb); }
  if( fChain->GetBranchStatus("cl_lateral_comb") ) { fChain->SetBranchAddress("cl_lateral_comb", &cl_lateral_comb, &b_cl_lateral_comb); }
  if( fChain->GetBranchStatus("cl_longitudinal_comb") ) { fChain->SetBranchAddress("cl_longitudinal_comb", &cl_longitudinal_comb, &b_cl_longitudinal_comb); }
  if( fChain->GetBranchStatus("cl_eng_frac_em_comb") ) { fChain->SetBranchAddress("cl_eng_frac_em_comb", &cl_eng_frac_em_comb, &b_cl_eng_frac_em_comb); }
  if( fChain->GetBranchStatus("cl_eng_frac_max_comb") ) { fChain->SetBranchAddress("cl_eng_frac_max_comb", &cl_eng_frac_max_comb, &b_cl_eng_frac_max_comb); }
  if( fChain->GetBranchStatus("cl_eng_frac_core_comb") ) { fChain->SetBranchAddress("cl_eng_frac_core_comb", &cl_eng_frac_core_comb, &b_cl_eng_frac_core_comb); }
  if( fChain->GetBranchStatus("cl_m1_dens_comb") ) { fChain->SetBranchAddress("cl_m1_dens_comb", &cl_m1_dens_comb, &b_cl_m1_dens_comb); }
  if( fChain->GetBranchStatus("cl_m2_dens_comb") ) { fChain->SetBranchAddress("cl_m2_dens_comb", &cl_m2_dens_comb, &b_cl_m2_dens_comb); }
  if( fChain->GetBranchStatus("cl_ehec0_comb") ) { fChain->SetBranchAddress("cl_ehec0_comb", &cl_ehec0_comb, &b_cl_ehec0_comb); }
  if( fChain->GetBranchStatus("cl_ehec1_comb") ) { fChain->SetBranchAddress("cl_ehec1_comb", &cl_ehec1_comb, &b_cl_ehec1_comb); }
  if( fChain->GetBranchStatus("cl_ehec2_comb") ) { fChain->SetBranchAddress("cl_ehec2_comb", &cl_ehec2_comb, &b_cl_ehec2_comb); }
  if( fChain->GetBranchStatus("cl_ehec3_comb") ) { fChain->SetBranchAddress("cl_ehec3_comb", &cl_ehec3_comb, &b_cl_ehec3_comb); }
  if( fChain->GetBranchStatus("cl_nhec0_comb") ) { fChain->SetBranchAddress("cl_nhec0_comb", &cl_nhec0_comb, &b_cl_nhec0_comb); }
  if( fChain->GetBranchStatus("cl_nhec1_comb") ) { fChain->SetBranchAddress("cl_nhec1_comb", &cl_nhec1_comb, &b_cl_nhec1_comb); }
  if( fChain->GetBranchStatus("cl_nhec2_comb") ) { fChain->SetBranchAddress("cl_nhec2_comb", &cl_nhec2_comb, &b_cl_nhec2_comb); }
  if( fChain->GetBranchStatus("cl_nhec3_comb") ) { fChain->SetBranchAddress("cl_nhec3_comb", &cl_nhec3_comb, &b_cl_nhec3_comb); }
  if( fChain->GetBranchStatus("cl_etileb0_comb") ) { fChain->SetBranchAddress("cl_etileb0_comb", &cl_etileb0_comb, &b_cl_etileb0_comb); }
  if( fChain->GetBranchStatus("cl_etileb1_comb") ) { fChain->SetBranchAddress("cl_etileb1_comb", &cl_etileb1_comb, &b_cl_etileb1_comb); }
  if( fChain->GetBranchStatus("cl_etileb2_comb") ) { fChain->SetBranchAddress("cl_etileb2_comb", &cl_etileb2_comb, &b_cl_etileb2_comb); }
  if( fChain->GetBranchStatus("cl_ntileb0_comb") ) { fChain->SetBranchAddress("cl_ntileb0_comb", &cl_ntileb0_comb, &b_cl_ntileb0_comb); }
  if( fChain->GetBranchStatus("cl_ntileb1_comb") ) { fChain->SetBranchAddress("cl_ntileb1_comb", &cl_ntileb1_comb, &b_cl_ntileb1_comb); }
  if( fChain->GetBranchStatus("cl_ntileb2_comb") ) { fChain->SetBranchAddress("cl_ntileb2_comb", &cl_ntileb2_comb, &b_cl_ntileb2_comb); }
  if( fChain->GetBranchStatus("cl_etilee0_comb") ) { fChain->SetBranchAddress("cl_etilee0_comb", &cl_etilee0_comb, &b_cl_etilee0_comb); }
  if( fChain->GetBranchStatus("cl_etilee1_comb") ) { fChain->SetBranchAddress("cl_etilee1_comb", &cl_etilee1_comb, &b_cl_etilee1_comb); }
  if( fChain->GetBranchStatus("cl_etilee2_comb") ) { fChain->SetBranchAddress("cl_etilee2_comb", &cl_etilee2_comb, &b_cl_etilee2_comb); }
  if( fChain->GetBranchStatus("cl_efcal0_comb") ) { fChain->SetBranchAddress("cl_efcal0_comb", &cl_efcal0_comb, &b_cl_efcal0_comb); }
  if( fChain->GetBranchStatus("cl_efcal1_comb") ) { fChain->SetBranchAddress("cl_efcal1_comb", &cl_efcal1_comb, &b_cl_efcal1_comb); }
  if( fChain->GetBranchStatus("cl_efcal2_comb") ) { fChain->SetBranchAddress("cl_efcal2_comb", &cl_efcal2_comb, &b_cl_efcal2_comb); }
  if( fChain->GetBranchStatus("cl_ntilee0_comb") ) { fChain->SetBranchAddress("cl_ntilee0_comb", &cl_ntilee0_comb, &b_cl_ntilee0_comb); }
  if( fChain->GetBranchStatus("cl_ntilee1_comb") ) { fChain->SetBranchAddress("cl_ntilee1_comb", &cl_ntilee1_comb, &b_cl_ntilee1_comb); }
  if( fChain->GetBranchStatus("cl_ntilee2_comb") ) { fChain->SetBranchAddress("cl_ntilee2_comb", &cl_ntilee2_comb, &b_cl_ntilee2_comb); }
  if( fChain->GetBranchStatus("cl_nfcal0_comb") ) { fChain->SetBranchAddress("cl_nfcal0_comb", &cl_nfcal0_comb, &b_cl_nfcal0_comb); }
  if( fChain->GetBranchStatus("cl_nfcal1_comb") ) { fChain->SetBranchAddress("cl_nfcal1_comb", &cl_nfcal1_comb, &b_cl_nfcal1_comb); }
  if( fChain->GetBranchStatus("cl_nfcal2_comb") ) { fChain->SetBranchAddress("cl_nfcal2_comb", &cl_nfcal2_comb, &b_cl_nfcal2_comb); }
  if( fChain->GetBranchStatus("cl_ntotcells_comb") ) { fChain->SetBranchAddress("cl_ntotcells_comb", &cl_ntotcells_comb, &b_cl_ntotcells_comb); }
  if( fChain->GetBranchStatus("cl_ecluster_topoEM") ) { fChain->SetBranchAddress("cl_ecluster_topoEM", &cl_ecluster_topoEM, &b_cl_ecluster_topoEM); }
  if( fChain->GetBranchStatus("cl_nctotal_topoEM") ) { fChain->SetBranchAddress("cl_nctotal_topoEM", &cl_nctotal_topoEM, &b_cl_nctotal_topoEM); }
  if( fChain->GetBranchStatus("cl_nc_topoEM") ) { fChain->SetBranchAddress("cl_nc_topoEM", &cl_nc_topoEM, &b_cl_nc_topoEM); }
  if( fChain->GetBranchStatus("cl_et_topoEM") ) { fChain->SetBranchAddress("cl_et_topoEM", &cl_et_topoEM, &b_cl_et_topoEM); }
  if( fChain->GetBranchStatus("cl_e_topoEM") ) { fChain->SetBranchAddress("cl_e_topoEM", &cl_e_topoEM, &b_cl_e_topoEM); }
  if( fChain->GetBranchStatus("cl_eemb0_topoEM") ) { fChain->SetBranchAddress("cl_eemb0_topoEM", &cl_eemb0_topoEM, &b_cl_eemb0_topoEM); }
  if( fChain->GetBranchStatus("cl_eemb1_topoEM") ) { fChain->SetBranchAddress("cl_eemb1_topoEM", &cl_eemb1_topoEM, &b_cl_eemb1_topoEM); }
  if( fChain->GetBranchStatus("cl_eemb2_topoEM") ) { fChain->SetBranchAddress("cl_eemb2_topoEM", &cl_eemb2_topoEM, &b_cl_eemb2_topoEM); }
  if( fChain->GetBranchStatus("cl_eemb3_topoEM") ) { fChain->SetBranchAddress("cl_eemb3_topoEM", &cl_eemb3_topoEM, &b_cl_eemb3_topoEM); }
  if( fChain->GetBranchStatus("cl_nemb0_topoEM") ) { fChain->SetBranchAddress("cl_nemb0_topoEM", &cl_nemb0_topoEM, &b_cl_nemb0_topoEM); }
  if( fChain->GetBranchStatus("cl_nemb1_topoEM") ) { fChain->SetBranchAddress("cl_nemb1_topoEM", &cl_nemb1_topoEM, &b_cl_nemb1_topoEM); }
  if( fChain->GetBranchStatus("cl_nemb2_topoEM") ) { fChain->SetBranchAddress("cl_nemb2_topoEM", &cl_nemb2_topoEM, &b_cl_nemb2_topoEM); }
  if( fChain->GetBranchStatus("cl_nemb3_topoEM") ) { fChain->SetBranchAddress("cl_nemb3_topoEM", &cl_nemb3_topoEM, &b_cl_nemb3_topoEM); }
  if( fChain->GetBranchStatus("cl_phi2_topoEM") ) { fChain->SetBranchAddress("cl_phi2_topoEM", &cl_phi2_topoEM, &b_cl_phi2_topoEM); }
  if( fChain->GetBranchStatus("cl_eta0_topoEM") ) { fChain->SetBranchAddress("cl_eta0_topoEM", &cl_eta0_topoEM, &b_cl_eta0_topoEM); }
  if( fChain->GetBranchStatus("cl_eta1_topoEM") ) { fChain->SetBranchAddress("cl_eta1_topoEM", &cl_eta1_topoEM, &b_cl_eta1_topoEM); }
  if( fChain->GetBranchStatus("cl_eta2_topoEM") ) { fChain->SetBranchAddress("cl_eta2_topoEM", &cl_eta2_topoEM, &b_cl_eta2_topoEM); }
  if( fChain->GetBranchStatus("cl_eta3_topoEM") ) { fChain->SetBranchAddress("cl_eta3_topoEM", &cl_eta3_topoEM, &b_cl_eta3_topoEM); }
  if( fChain->GetBranchStatus("cl_eeme0_topoEM") ) { fChain->SetBranchAddress("cl_eeme0_topoEM", &cl_eeme0_topoEM, &b_cl_eeme0_topoEM); }
  if( fChain->GetBranchStatus("cl_eeme1_topoEM") ) { fChain->SetBranchAddress("cl_eeme1_topoEM", &cl_eeme1_topoEM, &b_cl_eeme1_topoEM); }
  if( fChain->GetBranchStatus("cl_eeme2_topoEM") ) { fChain->SetBranchAddress("cl_eeme2_topoEM", &cl_eeme2_topoEM, &b_cl_eeme2_topoEM); }
  if( fChain->GetBranchStatus("cl_eeme3_topoEM") ) { fChain->SetBranchAddress("cl_eeme3_topoEM", &cl_eeme3_topoEM, &b_cl_eeme3_topoEM); }
  if( fChain->GetBranchStatus("cl_neme0_topoEM") ) { fChain->SetBranchAddress("cl_neme0_topoEM", &cl_neme0_topoEM, &b_cl_neme0_topoEM); }
  if( fChain->GetBranchStatus("cl_neme1_topoEM") ) { fChain->SetBranchAddress("cl_neme1_topoEM", &cl_neme1_topoEM, &b_cl_neme1_topoEM); }
  if( fChain->GetBranchStatus("cl_neme2_topoEM") ) { fChain->SetBranchAddress("cl_neme2_topoEM", &cl_neme2_topoEM, &b_cl_neme2_topoEM); }
  if( fChain->GetBranchStatus("cl_neme3_topoEM") ) { fChain->SetBranchAddress("cl_neme3_topoEM", &cl_neme3_topoEM, &b_cl_neme3_topoEM); }
  if( fChain->GetBranchStatus("cl_etileg1_topoEM") ) { fChain->SetBranchAddress("cl_etileg1_topoEM", &cl_etileg1_topoEM, &b_cl_etileg1_topoEM); }
  if( fChain->GetBranchStatus("cl_etileg2_topoEM") ) { fChain->SetBranchAddress("cl_etileg2_topoEM", &cl_etileg2_topoEM, &b_cl_etileg2_topoEM); }
  if( fChain->GetBranchStatus("cl_etileg3_topoEM") ) { fChain->SetBranchAddress("cl_etileg3_topoEM", &cl_etileg3_topoEM, &b_cl_etileg3_topoEM); }
  if( fChain->GetBranchStatus("cl_ntileg1_topoEM") ) { fChain->SetBranchAddress("cl_ntileg1_topoEM", &cl_ntileg1_topoEM, &b_cl_ntileg1_topoEM); }
  if( fChain->GetBranchStatus("cl_ntileg2_topoEM") ) { fChain->SetBranchAddress("cl_ntileg2_topoEM", &cl_ntileg2_topoEM, &b_cl_ntileg2_topoEM); }
  if( fChain->GetBranchStatus("cl_ntileg3_topoEM") ) { fChain->SetBranchAddress("cl_ntileg3_topoEM", &cl_ntileg3_topoEM, &b_cl_ntileg3_topoEM); }
  if( fChain->GetBranchStatus("cl_eta_topoEM") ) { fChain->SetBranchAddress("cl_eta_topoEM", &cl_eta_topoEM, &b_cl_eta_topoEM); }
  if( fChain->GetBranchStatus("cl_phi_topoEM") ) { fChain->SetBranchAddress("cl_phi_topoEM", &cl_phi_topoEM, &b_cl_phi_topoEM); }
  if( fChain->GetBranchStatus("cl_time_topoEM") ) { fChain->SetBranchAddress("cl_time_topoEM", &cl_time_topoEM, &b_cl_time_topoEM); }
  if( fChain->GetBranchStatus("cl_reco_stat_topoEM") ) { fChain->SetBranchAddress("cl_reco_stat_topoEM", &cl_reco_stat_topoEM, &b_cl_reco_stat_topoEM); }
  if( fChain->GetBranchStatus("cl_m1_eta_topoEM") ) { fChain->SetBranchAddress("cl_m1_eta_topoEM", &cl_m1_eta_topoEM, &b_cl_m1_eta_topoEM); }
  if( fChain->GetBranchStatus("cl_m1_phi_topoEM") ) { fChain->SetBranchAddress("cl_m1_phi_topoEM", &cl_m1_phi_topoEM, &b_cl_m1_phi_topoEM); }
  if( fChain->GetBranchStatus("cl_m2_r_topoEM") ) { fChain->SetBranchAddress("cl_m2_r_topoEM", &cl_m2_r_topoEM, &b_cl_m2_r_topoEM); }
  if( fChain->GetBranchStatus("cl_m2_lambda_topoEM") ) { fChain->SetBranchAddress("cl_m2_lambda_topoEM", &cl_m2_lambda_topoEM, &b_cl_m2_lambda_topoEM); }
  if( fChain->GetBranchStatus("cl_delta_phi_topoEM") ) { fChain->SetBranchAddress("cl_delta_phi_topoEM", &cl_delta_phi_topoEM, &b_cl_delta_phi_topoEM); }
  if( fChain->GetBranchStatus("cl_delta_theta_topoEM") ) { fChain->SetBranchAddress("cl_delta_theta_topoEM", &cl_delta_theta_topoEM, &b_cl_delta_theta_topoEM); }
  if( fChain->GetBranchStatus("cl_delta_alpha_topoEM") ) { fChain->SetBranchAddress("cl_delta_alpha_topoEM", &cl_delta_alpha_topoEM, &b_cl_delta_alpha_topoEM); }
  if( fChain->GetBranchStatus("cl_center_x_topoEM") ) { fChain->SetBranchAddress("cl_center_x_topoEM", &cl_center_x_topoEM, &b_cl_center_x_topoEM); }
  if( fChain->GetBranchStatus("cl_center_y_topoEM") ) { fChain->SetBranchAddress("cl_center_y_topoEM", &cl_center_y_topoEM, &b_cl_center_y_topoEM); }
  if( fChain->GetBranchStatus("cl_center_z_topoEM") ) { fChain->SetBranchAddress("cl_center_z_topoEM", &cl_center_z_topoEM, &b_cl_center_z_topoEM); }
  if( fChain->GetBranchStatus("cl_center_lambda_topoEM") ) { fChain->SetBranchAddress("cl_center_lambda_topoEM", &cl_center_lambda_topoEM, &b_cl_center_lambda_topoEM); }
  if( fChain->GetBranchStatus("cl_lateral_topoEM") ) { fChain->SetBranchAddress("cl_lateral_topoEM", &cl_lateral_topoEM, &b_cl_lateral_topoEM); }
  if( fChain->GetBranchStatus("cl_longitudinal_topoEM") ) { fChain->SetBranchAddress("cl_longitudinal_topoEM", &cl_longitudinal_topoEM, &b_cl_longitudinal_topoEM); }
  if( fChain->GetBranchStatus("cl_eng_frac_em_topoEM") ) { fChain->SetBranchAddress("cl_eng_frac_em_topoEM", &cl_eng_frac_em_topoEM, &b_cl_eng_frac_em_topoEM); }
  if( fChain->GetBranchStatus("cl_eng_frac_max_topoEM") ) { fChain->SetBranchAddress("cl_eng_frac_max_topoEM", &cl_eng_frac_max_topoEM, &b_cl_eng_frac_max_topoEM); }
  if( fChain->GetBranchStatus("cl_eng_frac_core_topoEM") ) { fChain->SetBranchAddress("cl_eng_frac_core_topoEM", &cl_eng_frac_core_topoEM, &b_cl_eng_frac_core_topoEM); }
  if( fChain->GetBranchStatus("cl_m1_dens_topoEM") ) { fChain->SetBranchAddress("cl_m1_dens_topoEM", &cl_m1_dens_topoEM, &b_cl_m1_dens_topoEM); }
  if( fChain->GetBranchStatus("cl_m2_dens_topoEM") ) { fChain->SetBranchAddress("cl_m2_dens_topoEM", &cl_m2_dens_topoEM, &b_cl_m2_dens_topoEM); }
  if( fChain->GetBranchStatus("cl_ntotcells_topoEM") ) { fChain->SetBranchAddress("cl_ntotcells_topoEM", &cl_ntotcells_topoEM, &b_cl_ntotcells_topoEM); }
  if( fChain->GetBranchStatus("IS_Nsgins") ) { fChain->SetBranchAddress("IS_Nsgins", &IS_Nsgins, &b_IS_Nsgins); }
  if( fChain->GetBranchStatus("IS_Xpt") ) { fChain->SetBranchAddress("IS_Xpt", &IS_Xpt, &b_IS_Xpt); }
  if( fChain->GetBranchStatus("IS_Ypt") ) { fChain->SetBranchAddress("IS_Ypt", &IS_Ypt, &b_IS_Ypt); }
  if( fChain->GetBranchStatus("IS_Zpt") ) { fChain->SetBranchAddress("IS_Zpt", &IS_Zpt, &b_IS_Zpt); }
  if( fChain->GetBranchStatus("IS_Xvec") ) { fChain->SetBranchAddress("IS_Xvec", &IS_Xvec, &b_IS_Xvec); }
  if( fChain->GetBranchStatus("IS_Yvec") ) { fChain->SetBranchAddress("IS_Yvec", &IS_Yvec, &b_IS_Yvec); }
  if( fChain->GetBranchStatus("IS_Zvec") ) { fChain->SetBranchAddress("IS_Zvec", &IS_Zvec, &b_IS_Zvec); }
  if( fChain->GetBranchStatus("IS_QualityFactor") ) { fChain->SetBranchAddress("IS_QualityFactor", &IS_QualityFactor, &b_IS_QualityFactor); }
  if( fChain->GetBranchStatus("IS_NbreOfMultiLayer") ) { fChain->SetBranchAddress("IS_NbreOfMultiLayer", &IS_NbreOfMultiLayer, &b_IS_NbreOfMultiLayer); }
  if( fChain->GetBranchStatus("IS_TrackSharing") ) { fChain->SetBranchAddress("IS_TrackSharing", &IS_TrackSharing, &b_IS_TrackSharing); }
  if( fChain->GetBranchStatus("IS_StationName") ) { fChain->SetBranchAddress("IS_StationName", &IS_StationName, &b_IS_StationName); }
  if( fChain->GetBranchStatus("IS_Ndigits") ) { fChain->SetBranchAddress("IS_Ndigits", &IS_Ndigits, &b_IS_Ndigits); }
  if( fChain->GetBranchStatus("MB_NTrk") ) { fChain->SetBranchAddress("MB_NTrk", &MB_NTrk, &b_MB_NTrk); }
  if( fChain->GetBranchStatus("VT_NTrk") ) { fChain->SetBranchAddress("VT_NTrk", &VT_NTrk, &b_VT_NTrk); }
  if( fChain->GetBranchStatus("VT_A0") ) { fChain->SetBranchAddress("VT_A0", &VT_A0, &b_VT_A0); }
  if( fChain->GetBranchStatus("VT_Z") ) { fChain->SetBranchAddress("VT_Z", &VT_Z, &b_VT_Z); }
  if( fChain->GetBranchStatus("VT_Phi") ) { fChain->SetBranchAddress("VT_Phi", &VT_Phi, &b_VT_Phi); }
  if( fChain->GetBranchStatus("VT_CotTh") ) { fChain->SetBranchAddress("VT_CotTh", &VT_CotTh, &b_VT_CotTh); }
  if( fChain->GetBranchStatus("VT_PTInv") ) { fChain->SetBranchAddress("VT_PTInv", &VT_PTInv, &b_VT_PTInv); }
  if( fChain->GetBranchStatus("VT_Cov11") ) { fChain->SetBranchAddress("VT_Cov11", &VT_Cov11, &b_VT_Cov11); }
  if( fChain->GetBranchStatus("VT_Cov21") ) { fChain->SetBranchAddress("VT_Cov21", &VT_Cov21, &b_VT_Cov21); }
  if( fChain->GetBranchStatus("VT_Cov22") ) { fChain->SetBranchAddress("VT_Cov22", &VT_Cov22, &b_VT_Cov22); }
  if( fChain->GetBranchStatus("VT_Cov31") ) { fChain->SetBranchAddress("VT_Cov31", &VT_Cov31, &b_VT_Cov31); }
  if( fChain->GetBranchStatus("VT_Cov32") ) { fChain->SetBranchAddress("VT_Cov32", &VT_Cov32, &b_VT_Cov32); }
  if( fChain->GetBranchStatus("VT_Cov33") ) { fChain->SetBranchAddress("VT_Cov33", &VT_Cov33, &b_VT_Cov33); }
  if( fChain->GetBranchStatus("VT_Cov41") ) { fChain->SetBranchAddress("VT_Cov41", &VT_Cov41, &b_VT_Cov41); }
  if( fChain->GetBranchStatus("VT_Cov42") ) { fChain->SetBranchAddress("VT_Cov42", &VT_Cov42, &b_VT_Cov42); }
  if( fChain->GetBranchStatus("VT_Cov43") ) { fChain->SetBranchAddress("VT_Cov43", &VT_Cov43, &b_VT_Cov43); }
  if( fChain->GetBranchStatus("VT_Cov44") ) { fChain->SetBranchAddress("VT_Cov44", &VT_Cov44, &b_VT_Cov44); }
  if( fChain->GetBranchStatus("VT_Cov51") ) { fChain->SetBranchAddress("VT_Cov51", &VT_Cov51, &b_VT_Cov51); }
  if( fChain->GetBranchStatus("VT_Cov52") ) { fChain->SetBranchAddress("VT_Cov52", &VT_Cov52, &b_VT_Cov52); }
  if( fChain->GetBranchStatus("VT_Cov53") ) { fChain->SetBranchAddress("VT_Cov53", &VT_Cov53, &b_VT_Cov53); }
  if( fChain->GetBranchStatus("VT_Cov54") ) { fChain->SetBranchAddress("VT_Cov54", &VT_Cov54, &b_VT_Cov54); }
  if( fChain->GetBranchStatus("VT_Cov55") ) { fChain->SetBranchAddress("VT_Cov55", &VT_Cov55, &b_VT_Cov55); }
  if( fChain->GetBranchStatus("VT_Xpt") ) { fChain->SetBranchAddress("VT_Xpt", &VT_Xpt, &b_VT_Xpt); }
  if( fChain->GetBranchStatus("VT_Ypt") ) { fChain->SetBranchAddress("VT_Ypt", &VT_Ypt, &b_VT_Ypt); }
  if( fChain->GetBranchStatus("VT_Zpt") ) { fChain->SetBranchAddress("VT_Zpt", &VT_Zpt, &b_VT_Zpt); }
  if( fChain->GetBranchStatus("VT_Xvec") ) { fChain->SetBranchAddress("VT_Xvec", &VT_Xvec, &b_VT_Xvec); }
  if( fChain->GetBranchStatus("VT_Yvec") ) { fChain->SetBranchAddress("VT_Yvec", &VT_Yvec, &b_VT_Yvec); }
  if( fChain->GetBranchStatus("VT_Zvec") ) { fChain->SetBranchAddress("VT_Zvec", &VT_Zvec, &b_VT_Zvec); }
  if( fChain->GetBranchStatus("VT_Mom") ) { fChain->SetBranchAddress("VT_Mom", &VT_Mom, &b_VT_Mom); }
  if( fChain->GetBranchStatus("VT_digits") ) { fChain->SetBranchAddress("VT_digits", &VT_digits, &b_VT_digits); }
  if( fChain->GetBranchStatus("KE_NTrk") ) { fChain->SetBranchAddress("KE_NTrk", &KE_NTrk, &b_KE_NTrk); }
  if( fChain->GetBranchStatus("KE_Thept") ) { fChain->SetBranchAddress("KE_Thept", &KE_Thept, &b_KE_Thept); }
  if( fChain->GetBranchStatus("KE_Phipt") ) { fChain->SetBranchAddress("KE_Phipt", &KE_Phipt, &b_KE_Phipt); }
  if( fChain->GetBranchStatus("KE_Thevc") ) { fChain->SetBranchAddress("KE_Thevc", &KE_Thevc, &b_KE_Thevc); }
  if( fChain->GetBranchStatus("KE_Phivc") ) { fChain->SetBranchAddress("KE_Phivc", &KE_Phivc, &b_KE_Phivc); }
  if( fChain->GetBranchStatus("KE_PInv") ) { fChain->SetBranchAddress("KE_PInv", &KE_PInv, &b_KE_PInv); }
  if( fChain->GetBranchStatus("KE_Cov11") ) { fChain->SetBranchAddress("KE_Cov11", &KE_Cov11, &b_KE_Cov11); }
  if( fChain->GetBranchStatus("KE_Cov21") ) { fChain->SetBranchAddress("KE_Cov21", &KE_Cov21, &b_KE_Cov21); }
  if( fChain->GetBranchStatus("KE_Cov22") ) { fChain->SetBranchAddress("KE_Cov22", &KE_Cov22, &b_KE_Cov22); }
  if( fChain->GetBranchStatus("KE_Cov31") ) { fChain->SetBranchAddress("KE_Cov31", &KE_Cov31, &b_KE_Cov31); }
  if( fChain->GetBranchStatus("KE_Cov32") ) { fChain->SetBranchAddress("KE_Cov32", &KE_Cov32, &b_KE_Cov32); }
  if( fChain->GetBranchStatus("KE_Cov33") ) { fChain->SetBranchAddress("KE_Cov33", &KE_Cov33, &b_KE_Cov33); }
  if( fChain->GetBranchStatus("KE_Cov41") ) { fChain->SetBranchAddress("KE_Cov41", &KE_Cov41, &b_KE_Cov41); }
  if( fChain->GetBranchStatus("KE_Cov42") ) { fChain->SetBranchAddress("KE_Cov42", &KE_Cov42, &b_KE_Cov42); }
  if( fChain->GetBranchStatus("KE_Cov43") ) { fChain->SetBranchAddress("KE_Cov43", &KE_Cov43, &b_KE_Cov43); }
  if( fChain->GetBranchStatus("KE_Cov44") ) { fChain->SetBranchAddress("KE_Cov44", &KE_Cov44, &b_KE_Cov44); }
  if( fChain->GetBranchStatus("KE_Cov51") ) { fChain->SetBranchAddress("KE_Cov51", &KE_Cov51, &b_KE_Cov51); }
  if( fChain->GetBranchStatus("KE_Cov52") ) { fChain->SetBranchAddress("KE_Cov52", &KE_Cov52, &b_KE_Cov52); }
  if( fChain->GetBranchStatus("KE_Cov53") ) { fChain->SetBranchAddress("KE_Cov53", &KE_Cov53, &b_KE_Cov53); }
  if( fChain->GetBranchStatus("KE_Cov54") ) { fChain->SetBranchAddress("KE_Cov54", &KE_Cov54, &b_KE_Cov54); }
  if( fChain->GetBranchStatus("KE_Cov55") ) { fChain->SetBranchAddress("KE_Cov55", &KE_Cov55, &b_KE_Cov55); }
  if( fChain->GetBranchStatus("KE_Xpt") ) { fChain->SetBranchAddress("KE_Xpt", &KE_Xpt, &b_KE_Xpt); }
  if( fChain->GetBranchStatus("KE_Ypt") ) { fChain->SetBranchAddress("KE_Ypt", &KE_Ypt, &b_KE_Ypt); }
  if( fChain->GetBranchStatus("KE_Zpt") ) { fChain->SetBranchAddress("KE_Zpt", &KE_Zpt, &b_KE_Zpt); }
  if( fChain->GetBranchStatus("KE_Xvec") ) { fChain->SetBranchAddress("KE_Xvec", &KE_Xvec, &b_KE_Xvec); }
  if( fChain->GetBranchStatus("KE_Yvec") ) { fChain->SetBranchAddress("KE_Yvec", &KE_Yvec, &b_KE_Yvec); }
  if( fChain->GetBranchStatus("KE_Zvec") ) { fChain->SetBranchAddress("KE_Zvec", &KE_Zvec, &b_KE_Zvec); }
  if( fChain->GetBranchStatus("KE_Mom") ) { fChain->SetBranchAddress("KE_Mom", &KE_Mom, &b_KE_Mom); }
  if( fChain->GetBranchStatus("KE_ZCyl") ) { fChain->SetBranchAddress("KE_ZCyl", &KE_ZCyl, &b_KE_ZCyl); }
  if( fChain->GetBranchStatus("KE_RCyl") ) { fChain->SetBranchAddress("KE_RCyl", &KE_RCyl, &b_KE_RCyl); }
  if( fChain->GetBranchStatus("MS_NTrk") ) { fChain->SetBranchAddress("MS_NTrk", &MS_NTrk, &b_MS_NTrk); }
  if( fChain->GetBranchStatus("MS_Thept") ) { fChain->SetBranchAddress("MS_Thept", &MS_Thept, &b_MS_Thept); }
  if( fChain->GetBranchStatus("MS_Phipt") ) { fChain->SetBranchAddress("MS_Phipt", &MS_Phipt, &b_MS_Phipt); }
  if( fChain->GetBranchStatus("MS_Thevc") ) { fChain->SetBranchAddress("MS_Thevc", &MS_Thevc, &b_MS_Thevc); }
  if( fChain->GetBranchStatus("MS_Phivc") ) { fChain->SetBranchAddress("MS_Phivc", &MS_Phivc, &b_MS_Phivc); }
  if( fChain->GetBranchStatus("MS_PInv") ) { fChain->SetBranchAddress("MS_PInv", &MS_PInv, &b_MS_PInv); }
  if( fChain->GetBranchStatus("MS_Cov11") ) { fChain->SetBranchAddress("MS_Cov11", &MS_Cov11, &b_MS_Cov11); }
  if( fChain->GetBranchStatus("MS_Cov21") ) { fChain->SetBranchAddress("MS_Cov21", &MS_Cov21, &b_MS_Cov21); }
  if( fChain->GetBranchStatus("MS_Cov22") ) { fChain->SetBranchAddress("MS_Cov22", &MS_Cov22, &b_MS_Cov22); }
  if( fChain->GetBranchStatus("MS_Cov31") ) { fChain->SetBranchAddress("MS_Cov31", &MS_Cov31, &b_MS_Cov31); }
  if( fChain->GetBranchStatus("MS_Cov32") ) { fChain->SetBranchAddress("MS_Cov32", &MS_Cov32, &b_MS_Cov32); }
  if( fChain->GetBranchStatus("MS_Cov33") ) { fChain->SetBranchAddress("MS_Cov33", &MS_Cov33, &b_MS_Cov33); }
  if( fChain->GetBranchStatus("MS_Cov41") ) { fChain->SetBranchAddress("MS_Cov41", &MS_Cov41, &b_MS_Cov41); }
  if( fChain->GetBranchStatus("MS_Cov42") ) { fChain->SetBranchAddress("MS_Cov42", &MS_Cov42, &b_MS_Cov42); }
  if( fChain->GetBranchStatus("MS_Cov43") ) { fChain->SetBranchAddress("MS_Cov43", &MS_Cov43, &b_MS_Cov43); }
  if( fChain->GetBranchStatus("MS_Cov44") ) { fChain->SetBranchAddress("MS_Cov44", &MS_Cov44, &b_MS_Cov44); }
  if( fChain->GetBranchStatus("MS_Cov51") ) { fChain->SetBranchAddress("MS_Cov51", &MS_Cov51, &b_MS_Cov51); }
  if( fChain->GetBranchStatus("MS_Cov52") ) { fChain->SetBranchAddress("MS_Cov52", &MS_Cov52, &b_MS_Cov52); }
  if( fChain->GetBranchStatus("MS_Cov53") ) { fChain->SetBranchAddress("MS_Cov53", &MS_Cov53, &b_MS_Cov53); }
  if( fChain->GetBranchStatus("MS_Cov54") ) { fChain->SetBranchAddress("MS_Cov54", &MS_Cov54, &b_MS_Cov54); }
  if( fChain->GetBranchStatus("MS_Cov55") ) { fChain->SetBranchAddress("MS_Cov55", &MS_Cov55, &b_MS_Cov55); }
  if( fChain->GetBranchStatus("MS_Xpt") ) { fChain->SetBranchAddress("MS_Xpt", &MS_Xpt, &b_MS_Xpt); }
  if( fChain->GetBranchStatus("MS_Ypt") ) { fChain->SetBranchAddress("MS_Ypt", &MS_Ypt, &b_MS_Ypt); }
  if( fChain->GetBranchStatus("MS_Zpt") ) { fChain->SetBranchAddress("MS_Zpt", &MS_Zpt, &b_MS_Zpt); }
  if( fChain->GetBranchStatus("MS_Xvec") ) { fChain->SetBranchAddress("MS_Xvec", &MS_Xvec, &b_MS_Xvec); }
  if( fChain->GetBranchStatus("MS_Yvec") ) { fChain->SetBranchAddress("MS_Yvec", &MS_Yvec, &b_MS_Yvec); }
  if( fChain->GetBranchStatus("MS_Zvec") ) { fChain->SetBranchAddress("MS_Zvec", &MS_Zvec, &b_MS_Zvec); }
  if( fChain->GetBranchStatus("MS_Mom") ) { fChain->SetBranchAddress("MS_Mom", &MS_Mom, &b_MS_Mom); }
  if( fChain->GetBranchStatus("MS_ZCyl") ) { fChain->SetBranchAddress("MS_ZCyl", &MS_ZCyl, &b_MS_ZCyl); }
  if( fChain->GetBranchStatus("MS_RCyl") ) { fChain->SetBranchAddress("MS_RCyl", &MS_RCyl, &b_MS_RCyl); }
  if( fChain->GetBranchStatus("DR1_NTrk") ) { fChain->SetBranchAddress("DR1_NTrk", &DR1_NTrk, &b_DR1_NTrk); }
  if( fChain->GetBranchStatus("MbKE_Calo_En_dr01") ) { fChain->SetBranchAddress("MbKE_Calo_En_dr01", &MbKE_Calo_En_dr01, &b_MbKE_Calo_En_dr01); }
  if( fChain->GetBranchStatus("MbKE_Calo_Et_dr01") ) { fChain->SetBranchAddress("MbKE_Calo_Et_dr01", &MbKE_Calo_Et_dr01, &b_MbKE_Calo_Et_dr01); }
  if( fChain->GetBranchStatus("MbKE_HAD_Calo_En_dr01") ) { fChain->SetBranchAddress("MbKE_HAD_Calo_En_dr01", &MbKE_HAD_Calo_En_dr01, &b_MbKE_HAD_Calo_En_dr01); }
  if( fChain->GetBranchStatus("MbKE_HAD_Calo_Et_dr01") ) { fChain->SetBranchAddress("MbKE_HAD_Calo_Et_dr01", &MbKE_HAD_Calo_Et_dr01, &b_MbKE_HAD_Calo_Et_dr01); }
  if( fChain->GetBranchStatus("MbKE_EM_Calo_En_dr01") ) { fChain->SetBranchAddress("MbKE_EM_Calo_En_dr01", &MbKE_EM_Calo_En_dr01, &b_MbKE_EM_Calo_En_dr01); }
  if( fChain->GetBranchStatus("MbKE_EM_Calo_Et_dr01") ) { fChain->SetBranchAddress("MbKE_EM_Calo_Et_dr01", &MbKE_EM_Calo_Et_dr01, &b_MbKE_EM_Calo_Et_dr01); }
  if( fChain->GetBranchStatus("Mboy_nmuon") ) { fChain->SetBranchAddress("Mboy_nmuon", &Mboy_nmuon, &b_Mboy_nmuon); }
  if( fChain->GetBranchStatus("Mboy_A0") ) { fChain->SetBranchAddress("Mboy_A0", &Mboy_A0, &b_Mboy_A0); }
  if( fChain->GetBranchStatus("Mboy_Z") ) { fChain->SetBranchAddress("Mboy_Z", &Mboy_Z, &b_Mboy_Z); }
  if( fChain->GetBranchStatus("Mboy_Phi") ) { fChain->SetBranchAddress("Mboy_Phi", &Mboy_Phi, &b_Mboy_Phi); }
  if( fChain->GetBranchStatus("Mboy_Theta") ) { fChain->SetBranchAddress("Mboy_Theta", &Mboy_Theta, &b_Mboy_Theta); }
  if( fChain->GetBranchStatus("Mboy_qOverP") ) { fChain->SetBranchAddress("Mboy_qOverP", &Mboy_qOverP, &b_Mboy_qOverP); }
  if( fChain->GetBranchStatus("Mboy_covr11") ) { fChain->SetBranchAddress("Mboy_covr11", &Mboy_covr11, &b_Mboy_covr11); }
  if( fChain->GetBranchStatus("Mboy_covr21") ) { fChain->SetBranchAddress("Mboy_covr21", &Mboy_covr21, &b_Mboy_covr21); }
  if( fChain->GetBranchStatus("Mboy_covr22") ) { fChain->SetBranchAddress("Mboy_covr22", &Mboy_covr22, &b_Mboy_covr22); }
  if( fChain->GetBranchStatus("Mboy_covr31") ) { fChain->SetBranchAddress("Mboy_covr31", &Mboy_covr31, &b_Mboy_covr31); }
  if( fChain->GetBranchStatus("Mboy_covr32") ) { fChain->SetBranchAddress("Mboy_covr32", &Mboy_covr32, &b_Mboy_covr32); }
  if( fChain->GetBranchStatus("Mboy_covr33") ) { fChain->SetBranchAddress("Mboy_covr33", &Mboy_covr33, &b_Mboy_covr33); }
  if( fChain->GetBranchStatus("Mboy_covr41") ) { fChain->SetBranchAddress("Mboy_covr41", &Mboy_covr41, &b_Mboy_covr41); }
  if( fChain->GetBranchStatus("Mboy_covr42") ) { fChain->SetBranchAddress("Mboy_covr42", &Mboy_covr42, &b_Mboy_covr42); }
  if( fChain->GetBranchStatus("Mboy_covr43") ) { fChain->SetBranchAddress("Mboy_covr43", &Mboy_covr43, &b_Mboy_covr43); }
  if( fChain->GetBranchStatus("Mboy_covr44") ) { fChain->SetBranchAddress("Mboy_covr44", &Mboy_covr44, &b_Mboy_covr44); }
  if( fChain->GetBranchStatus("Mboy_covr51") ) { fChain->SetBranchAddress("Mboy_covr51", &Mboy_covr51, &b_Mboy_covr51); }
  if( fChain->GetBranchStatus("Mboy_covr52") ) { fChain->SetBranchAddress("Mboy_covr52", &Mboy_covr52, &b_Mboy_covr52); }
  if( fChain->GetBranchStatus("Mboy_covr53") ) { fChain->SetBranchAddress("Mboy_covr53", &Mboy_covr53, &b_Mboy_covr53); }
  if( fChain->GetBranchStatus("Mboy_covr54") ) { fChain->SetBranchAddress("Mboy_covr54", &Mboy_covr54, &b_Mboy_covr54); }
  if( fChain->GetBranchStatus("Mboy_covr55") ) { fChain->SetBranchAddress("Mboy_covr55", &Mboy_covr55, &b_Mboy_covr55); }
  if( fChain->GetBranchStatus("Mboy_etcone10") ) { fChain->SetBranchAddress("Mboy_etcone10", &Mboy_etcone10, &b_Mboy_etcone10); }
  if( fChain->GetBranchStatus("Mboy_etcone20") ) { fChain->SetBranchAddress("Mboy_etcone20", &Mboy_etcone20, &b_Mboy_etcone20); }
  if( fChain->GetBranchStatus("Mboy_etcone30") ) { fChain->SetBranchAddress("Mboy_etcone30", &Mboy_etcone30, &b_Mboy_etcone30); }
  if( fChain->GetBranchStatus("Mboy_etcone40") ) { fChain->SetBranchAddress("Mboy_etcone40", &Mboy_etcone40, &b_Mboy_etcone40); }
  if( fChain->GetBranchStatus("Mboy_etcone") ) { fChain->SetBranchAddress("Mboy_etcone", &Mboy_etcone, &b_Mboy_etcone); }
  if( fChain->GetBranchStatus("Mboy_etcone50") ) { fChain->SetBranchAddress("Mboy_etcone50", &Mboy_etcone50, &b_Mboy_etcone50); }
  if( fChain->GetBranchStatus("Mboy_etcone60") ) { fChain->SetBranchAddress("Mboy_etcone60", &Mboy_etcone60, &b_Mboy_etcone60); }
  if( fChain->GetBranchStatus("Mboy_etcone70") ) { fChain->SetBranchAddress("Mboy_etcone70", &Mboy_etcone70, &b_Mboy_etcone70); }
  if( fChain->GetBranchStatus("Mboy_nucone10") ) { fChain->SetBranchAddress("Mboy_nucone10", &Mboy_nucone10, &b_Mboy_nucone10); }
  if( fChain->GetBranchStatus("Mboy_nucone20") ) { fChain->SetBranchAddress("Mboy_nucone20", &Mboy_nucone20, &b_Mboy_nucone20); }
  if( fChain->GetBranchStatus("Mboy_nucone30") ) { fChain->SetBranchAddress("Mboy_nucone30", &Mboy_nucone30, &b_Mboy_nucone30); }
  if( fChain->GetBranchStatus("Mboy_nucone40") ) { fChain->SetBranchAddress("Mboy_nucone40", &Mboy_nucone40, &b_Mboy_nucone40); }
  if( fChain->GetBranchStatus("Mboy_nucone") ) { fChain->SetBranchAddress("Mboy_nucone", &Mboy_nucone, &b_Mboy_nucone); }
  if( fChain->GetBranchStatus("Mboy_nucone50") ) { fChain->SetBranchAddress("Mboy_nucone50", &Mboy_nucone50, &b_Mboy_nucone50); }
  if( fChain->GetBranchStatus("Mboy_nucone60") ) { fChain->SetBranchAddress("Mboy_nucone60", &Mboy_nucone60, &b_Mboy_nucone60); }
  if( fChain->GetBranchStatus("Mboy_nucone70") ) { fChain->SetBranchAddress("Mboy_nucone70", &Mboy_nucone70, &b_Mboy_nucone70); }
  if( fChain->GetBranchStatus("Mboy_Eloss") ) { fChain->SetBranchAddress("Mboy_Eloss", &Mboy_Eloss, &b_Mboy_Eloss); }
  if( fChain->GetBranchStatus("Mboy_ElossError") ) { fChain->SetBranchAddress("Mboy_ElossError", &Mboy_ElossError, &b_Mboy_ElossError); }
  if( fChain->GetBranchStatus("Moore_nkine") ) { fChain->SetBranchAddress("Moore_nkine", &Moore_nkine, &b_Moore_nkine); }
  if( fChain->GetBranchStatus("Moore_xvtxg") ) { fChain->SetBranchAddress("Moore_xvtxg", &Moore_xvtxg, &b_Moore_xvtxg); }
  if( fChain->GetBranchStatus("Moore_yvtxg") ) { fChain->SetBranchAddress("Moore_yvtxg", &Moore_yvtxg, &b_Moore_yvtxg); }
  if( fChain->GetBranchStatus("Moore_zvtxg") ) { fChain->SetBranchAddress("Moore_zvtxg", &Moore_zvtxg, &b_Moore_zvtxg); }
  if( fChain->GetBranchStatus("Moore_a0g") ) { fChain->SetBranchAddress("Moore_a0g", &Moore_a0g, &b_Moore_a0g); }
  if( fChain->GetBranchStatus("Moore_z0g") ) { fChain->SetBranchAddress("Moore_z0g", &Moore_z0g, &b_Moore_z0g); }
  if( fChain->GetBranchStatus("Moore_phig") ) { fChain->SetBranchAddress("Moore_phig", &Moore_phig, &b_Moore_phig); }
  if( fChain->GetBranchStatus("Moore_cotthg") ) { fChain->SetBranchAddress("Moore_cotthg", &Moore_cotthg, &b_Moore_cotthg); }
  if( fChain->GetBranchStatus("Moore_ptig") ) { fChain->SetBranchAddress("Moore_ptig", &Moore_ptig, &b_Moore_ptig); }
  if( fChain->GetBranchStatus("Moore_etag") ) { fChain->SetBranchAddress("Moore_etag", &Moore_etag, &b_Moore_etag); }
  if( fChain->GetBranchStatus("Moore_rassociated") ) { fChain->SetBranchAddress("Moore_rassociated", &Moore_rassociated, &b_Moore_rassociated); }
  if( fChain->GetBranchStatus("Moore_matchdg") ) { fChain->SetBranchAddress("Moore_matchdg", &Moore_matchdg, &b_Moore_matchdg); }
  if( fChain->GetBranchStatus("Moore_xextrg") ) { fChain->SetBranchAddress("Moore_xextrg", &Moore_xextrg, &b_Moore_xextrg); }
  if( fChain->GetBranchStatus("Moore_yextrg") ) { fChain->SetBranchAddress("Moore_yextrg", &Moore_yextrg, &b_Moore_yextrg); }
  if( fChain->GetBranchStatus("Moore_zextrg") ) { fChain->SetBranchAddress("Moore_zextrg", &Moore_zextrg, &b_Moore_zextrg); }
  if( fChain->GetBranchStatus("Moore_a0extrg") ) { fChain->SetBranchAddress("Moore_a0extrg", &Moore_a0extrg, &b_Moore_a0extrg); }
  if( fChain->GetBranchStatus("Moore_z0extrg") ) { fChain->SetBranchAddress("Moore_z0extrg", &Moore_z0extrg, &b_Moore_z0extrg); }
  if( fChain->GetBranchStatus("Moore_phiextrg") ) { fChain->SetBranchAddress("Moore_phiextrg", &Moore_phiextrg, &b_Moore_phiextrg); }
  if( fChain->GetBranchStatus("Moore_cthextrg") ) { fChain->SetBranchAddress("Moore_cthextrg", &Moore_cthextrg, &b_Moore_cthextrg); }
  if( fChain->GetBranchStatus("Moore_ptiextrg") ) { fChain->SetBranchAddress("Moore_ptiextrg", &Moore_ptiextrg, &b_Moore_ptiextrg); }
  if( fChain->GetBranchStatus("Moore_etaextrg") ) { fChain->SetBranchAddress("Moore_etaextrg", &Moore_etaextrg, &b_Moore_etaextrg); }
  if( fChain->GetBranchStatus("Moore_barcodeg") ) { fChain->SetBranchAddress("Moore_barcodeg", &Moore_barcodeg, &b_Moore_barcodeg); }
  if( fChain->GetBranchStatus("Moore_statusg") ) { fChain->SetBranchAddress("Moore_statusg", &Moore_statusg, &b_Moore_statusg); }
  if( fChain->GetBranchStatus("Moore_elossg") ) { fChain->SetBranchAddress("Moore_elossg", &Moore_elossg, &b_Moore_elossg); }
  if( fChain->GetBranchStatus("Moore_drphig") ) { fChain->SetBranchAddress("Moore_drphig", &Moore_drphig, &b_Moore_drphig); }
  if( fChain->GetBranchStatus("Moore_drzg") ) { fChain->SetBranchAddress("Moore_drzg", &Moore_drzg, &b_Moore_drzg); }
  if( fChain->GetBranchStatus("Moore_dphig") ) { fChain->SetBranchAddress("Moore_dphig", &Moore_dphig, &b_Moore_dphig); }
  if( fChain->GetBranchStatus("Moore_dthetag") ) { fChain->SetBranchAddress("Moore_dthetag", &Moore_dthetag, &b_Moore_dthetag); }
  if( fChain->GetBranchStatus("Moore_a0pullg") ) { fChain->SetBranchAddress("Moore_a0pullg", &Moore_a0pullg, &b_Moore_a0pullg); }
  if( fChain->GetBranchStatus("Moore_z0pullg") ) { fChain->SetBranchAddress("Moore_z0pullg", &Moore_z0pullg, &b_Moore_z0pullg); }
  if( fChain->GetBranchStatus("Moore_phipullg") ) { fChain->SetBranchAddress("Moore_phipullg", &Moore_phipullg, &b_Moore_phipullg); }
  if( fChain->GetBranchStatus("Moore_cotthpullg") ) { fChain->SetBranchAddress("Moore_cotthpullg", &Moore_cotthpullg, &b_Moore_cotthpullg); }
  if( fChain->GetBranchStatus("Moore_ptipullg") ) { fChain->SetBranchAddress("Moore_ptipullg", &Moore_ptipullg, &b_Moore_ptipullg); }
  if( fChain->GetBranchStatus("Moore_nmuon") ) { fChain->SetBranchAddress("Moore_nmuon", &Moore_nmuon, &b_Moore_nmuon); }
  if( fChain->GetBranchStatus("Moore_phih") ) { fChain->SetBranchAddress("Moore_phih", &Moore_phih, &b_Moore_phih); }
  if( fChain->GetBranchStatus("Moore_etah") ) { fChain->SetBranchAddress("Moore_etah", &Moore_etah, &b_Moore_etah); }
  if( fChain->GetBranchStatus("Moore_mdth") ) { fChain->SetBranchAddress("Moore_mdth", &Moore_mdth, &b_Moore_mdth); }
  if( fChain->GetBranchStatus("Moore_cscetah") ) { fChain->SetBranchAddress("Moore_cscetah", &Moore_cscetah, &b_Moore_cscetah); }
  if( fChain->GetBranchStatus("Moore_cscphih") ) { fChain->SetBranchAddress("Moore_cscphih", &Moore_cscphih, &b_Moore_cscphih); }
  if( fChain->GetBranchStatus("Moore_rpcetah") ) { fChain->SetBranchAddress("Moore_rpcetah", &Moore_rpcetah, &b_Moore_rpcetah); }
  if( fChain->GetBranchStatus("Moore_rpcphih") ) { fChain->SetBranchAddress("Moore_rpcphih", &Moore_rpcphih, &b_Moore_rpcphih); }
  if( fChain->GetBranchStatus("Moore_tgcetah") ) { fChain->SetBranchAddress("Moore_tgcetah", &Moore_tgcetah, &b_Moore_tgcetah); }
  if( fChain->GetBranchStatus("Moore_tgcphih") ) { fChain->SetBranchAddress("Moore_tgcphih", &Moore_tgcphih, &b_Moore_tgcphih); }
  if( fChain->GetBranchStatus("Moore_stati") ) { fChain->SetBranchAddress("Moore_stati", &Moore_stati, &b_Moore_stati); }
  if( fChain->GetBranchStatus("Moore_statr") ) { fChain->SetBranchAddress("Moore_statr", &Moore_statr, &b_Moore_statr); }
  if( fChain->GetBranchStatus("Moore_xvtxr") ) { fChain->SetBranchAddress("Moore_xvtxr", &Moore_xvtxr, &b_Moore_xvtxr); }
  if( fChain->GetBranchStatus("Moore_yvtxr") ) { fChain->SetBranchAddress("Moore_yvtxr", &Moore_yvtxr, &b_Moore_yvtxr); }
  if( fChain->GetBranchStatus("Moore_zvtxr") ) { fChain->SetBranchAddress("Moore_zvtxr", &Moore_zvtxr, &b_Moore_zvtxr); }
  if( fChain->GetBranchStatus("Moore_a0r") ) { fChain->SetBranchAddress("Moore_a0r", &Moore_a0r, &b_Moore_a0r); }
  if( fChain->GetBranchStatus("Moore_z0r") ) { fChain->SetBranchAddress("Moore_z0r", &Moore_z0r, &b_Moore_z0r); }
  if( fChain->GetBranchStatus("Moore_phir") ) { fChain->SetBranchAddress("Moore_phir", &Moore_phir, &b_Moore_phir); }
  if( fChain->GetBranchStatus("Moore_cotthr") ) { fChain->SetBranchAddress("Moore_cotthr", &Moore_cotthr, &b_Moore_cotthr); }
  if( fChain->GetBranchStatus("Moore_ptir") ) { fChain->SetBranchAddress("Moore_ptir", &Moore_ptir, &b_Moore_ptir); }
  if( fChain->GetBranchStatus("Moore_etar") ) { fChain->SetBranchAddress("Moore_etar", &Moore_etar, &b_Moore_etar); }
  if( fChain->GetBranchStatus("Moore_chi2") ) { fChain->SetBranchAddress("Moore_chi2", &Moore_chi2, &b_Moore_chi2); }
  if( fChain->GetBranchStatus("Moore_chi2pr") ) { fChain->SetBranchAddress("Moore_chi2pr", &Moore_chi2pr, &b_Moore_chi2pr); }
  if( fChain->GetBranchStatus("Moore_origx") ) { fChain->SetBranchAddress("Moore_origx", &Moore_origx, &b_Moore_origx); }
  if( fChain->GetBranchStatus("Moore_origy") ) { fChain->SetBranchAddress("Moore_origy", &Moore_origy, &b_Moore_origy); }
  if( fChain->GetBranchStatus("Moore_origz") ) { fChain->SetBranchAddress("Moore_origz", &Moore_origz, &b_Moore_origz); }
  if( fChain->GetBranchStatus("Moore_covr11") ) { fChain->SetBranchAddress("Moore_covr11", &Moore_covr11, &b_Moore_covr11); }
  if( fChain->GetBranchStatus("Moore_covr12") ) { fChain->SetBranchAddress("Moore_covr12", &Moore_covr12, &b_Moore_covr12); }
  if( fChain->GetBranchStatus("Moore_covr22") ) { fChain->SetBranchAddress("Moore_covr22", &Moore_covr22, &b_Moore_covr22); }
  if( fChain->GetBranchStatus("Moore_covr13") ) { fChain->SetBranchAddress("Moore_covr13", &Moore_covr13, &b_Moore_covr13); }
  if( fChain->GetBranchStatus("Moore_covr23") ) { fChain->SetBranchAddress("Moore_covr23", &Moore_covr23, &b_Moore_covr23); }
  if( fChain->GetBranchStatus("Moore_covr33") ) { fChain->SetBranchAddress("Moore_covr33", &Moore_covr33, &b_Moore_covr33); }
  if( fChain->GetBranchStatus("Moore_covr14") ) { fChain->SetBranchAddress("Moore_covr14", &Moore_covr14, &b_Moore_covr14); }
  if( fChain->GetBranchStatus("Moore_covr24") ) { fChain->SetBranchAddress("Moore_covr24", &Moore_covr24, &b_Moore_covr24); }
  if( fChain->GetBranchStatus("Moore_covr34") ) { fChain->SetBranchAddress("Moore_covr34", &Moore_covr34, &b_Moore_covr34); }
  if( fChain->GetBranchStatus("Moore_covr44") ) { fChain->SetBranchAddress("Moore_covr44", &Moore_covr44, &b_Moore_covr44); }
  if( fChain->GetBranchStatus("Moore_covr15") ) { fChain->SetBranchAddress("Moore_covr15", &Moore_covr15, &b_Moore_covr15); }
  if( fChain->GetBranchStatus("Moore_covr25") ) { fChain->SetBranchAddress("Moore_covr25", &Moore_covr25, &b_Moore_covr25); }
  if( fChain->GetBranchStatus("Moore_covr35") ) { fChain->SetBranchAddress("Moore_covr35", &Moore_covr35, &b_Moore_covr35); }
  if( fChain->GetBranchStatus("Moore_covr45") ) { fChain->SetBranchAddress("Moore_covr45", &Moore_covr45, &b_Moore_covr45); }
  if( fChain->GetBranchStatus("Moore_covr55") ) { fChain->SetBranchAddress("Moore_covr55", &Moore_covr55, &b_Moore_covr55); }
  if( fChain->GetBranchStatus("conv_numVertices") ) { fChain->SetBranchAddress("conv_numVertices", &conv_numVertices, &b_conv_numVertices); }
  if( fChain->GetBranchStatus("conv_vtx_x") ) { fChain->SetBranchAddress("conv_vtx_x", &conv_vtx_x, &b_conv_vtx_x); }
  if( fChain->GetBranchStatus("conv_vtx_y") ) { fChain->SetBranchAddress("conv_vtx_y", &conv_vtx_y, &b_conv_vtx_y); }
  if( fChain->GetBranchStatus("conv_vtx_z") ) { fChain->SetBranchAddress("conv_vtx_z", &conv_vtx_z, &b_conv_vtx_z); }
  if( fChain->GetBranchStatus("conv_is_conv") ) { fChain->SetBranchAddress("conv_is_conv", &conv_is_conv, &b_conv_is_conv); }
  if( fChain->GetBranchStatus("conv_vtx_x_truth") ) { fChain->SetBranchAddress("conv_vtx_x_truth", &conv_vtx_x_truth, &b_conv_vtx_x_truth); }
  if( fChain->GetBranchStatus("conv_vtx_y_truth") ) { fChain->SetBranchAddress("conv_vtx_y_truth", &conv_vtx_y_truth, &b_conv_vtx_y_truth); }
  if( fChain->GetBranchStatus("conv_vtx_z_truth") ) { fChain->SetBranchAddress("conv_vtx_z_truth", &conv_vtx_z_truth, &b_conv_vtx_z_truth); }
  if( fChain->GetBranchStatus("conv_sigvtx_x") ) { fChain->SetBranchAddress("conv_sigvtx_x", &conv_sigvtx_x, &b_conv_sigvtx_x); }
  if( fChain->GetBranchStatus("conv_sigvtx_y") ) { fChain->SetBranchAddress("conv_sigvtx_y", &conv_sigvtx_y, &b_conv_sigvtx_y); }
  if( fChain->GetBranchStatus("conv_sigvtx_z") ) { fChain->SetBranchAddress("conv_sigvtx_z", &conv_sigvtx_z, &b_conv_sigvtx_z); }
  if( fChain->GetBranchStatus("conv_vtxchi2") ) { fChain->SetBranchAddress("conv_vtxchi2", &conv_vtxchi2, &b_conv_vtxchi2); }
  if( fChain->GetBranchStatus("conv_vtxndf") ) { fChain->SetBranchAddress("conv_vtxndf", &conv_vtxndf, &b_conv_vtxndf); }
  if( fChain->GetBranchStatus("conv_vtxchi2prob") ) { fChain->SetBranchAddress("conv_vtxchi2prob", &conv_vtxchi2prob, &b_conv_vtxchi2prob); }
  if( fChain->GetBranchStatus("conv_numTracksPerVertex") ) { fChain->SetBranchAddress("conv_numTracksPerVertex", &conv_numTracksPerVertex, &b_conv_numTracksPerVertex); }
  if( fChain->GetBranchStatus("conv_track1_chi2") ) { fChain->SetBranchAddress("conv_track1_chi2", &conv_track1_chi2, &b_conv_track1_chi2); }
  if( fChain->GetBranchStatus("conv_track2_chi2") ) { fChain->SetBranchAddress("conv_track2_chi2", &conv_track2_chi2, &b_conv_track2_chi2); }
  if( fChain->GetBranchStatus("conv_track1_d0") ) { fChain->SetBranchAddress("conv_track1_d0", &conv_track1_d0, &b_conv_track1_d0); }
  if( fChain->GetBranchStatus("conv_track1_sigd0") ) { fChain->SetBranchAddress("conv_track1_sigd0", &conv_track1_sigd0, &b_conv_track1_sigd0); }
  if( fChain->GetBranchStatus("conv_track1_z0") ) { fChain->SetBranchAddress("conv_track1_z0", &conv_track1_z0, &b_conv_track1_z0); }
  if( fChain->GetBranchStatus("conv_track1_sigz0") ) { fChain->SetBranchAddress("conv_track1_sigz0", &conv_track1_sigz0, &b_conv_track1_sigz0); }
  if( fChain->GetBranchStatus("conv_track1_phi") ) { fChain->SetBranchAddress("conv_track1_phi", &conv_track1_phi, &b_conv_track1_phi); }
  if( fChain->GetBranchStatus("conv_track1_sigphi") ) { fChain->SetBranchAddress("conv_track1_sigphi", &conv_track1_sigphi, &b_conv_track1_sigphi); }
  if( fChain->GetBranchStatus("conv_track1_theta") ) { fChain->SetBranchAddress("conv_track1_theta", &conv_track1_theta, &b_conv_track1_theta); }
  if( fChain->GetBranchStatus("conv_track1_sigtheta") ) { fChain->SetBranchAddress("conv_track1_sigtheta", &conv_track1_sigtheta, &b_conv_track1_sigtheta); }
  if( fChain->GetBranchStatus("conv_track1_qOverP") ) { fChain->SetBranchAddress("conv_track1_qOverP", &conv_track1_qOverP, &b_conv_track1_qOverP); }
  if( fChain->GetBranchStatus("conv_track1_sigqOverP") ) { fChain->SetBranchAddress("conv_track1_sigqOverP", &conv_track1_sigqOverP, &b_conv_track1_sigqOverP); }
  if( fChain->GetBranchStatus("conv_track1_momentum_x") ) { fChain->SetBranchAddress("conv_track1_momentum_x", &conv_track1_momentum_x, &b_conv_track1_momentum_x); }
  if( fChain->GetBranchStatus("conv_track1_momentum_y") ) { fChain->SetBranchAddress("conv_track1_momentum_y", &conv_track1_momentum_y, &b_conv_track1_momentum_y); }
  if( fChain->GetBranchStatus("conv_track1_momentum_z") ) { fChain->SetBranchAddress("conv_track1_momentum_z", &conv_track1_momentum_z, &b_conv_track1_momentum_z); }
  if( fChain->GetBranchStatus("conv_track1_d0g") ) { fChain->SetBranchAddress("conv_track1_d0g", &conv_track1_d0g, &b_conv_track1_d0g); }
  if( fChain->GetBranchStatus("conv_track1_sigd0g") ) { fChain->SetBranchAddress("conv_track1_sigd0g", &conv_track1_sigd0g, &b_conv_track1_sigd0g); }
  if( fChain->GetBranchStatus("conv_track1_z0g") ) { fChain->SetBranchAddress("conv_track1_z0g", &conv_track1_z0g, &b_conv_track1_z0g); }
  if( fChain->GetBranchStatus("conv_track1_sigz0g") ) { fChain->SetBranchAddress("conv_track1_sigz0g", &conv_track1_sigz0g, &b_conv_track1_sigz0g); }
  if( fChain->GetBranchStatus("conv_track2_d0") ) { fChain->SetBranchAddress("conv_track2_d0", &conv_track2_d0, &b_conv_track2_d0); }
  if( fChain->GetBranchStatus("conv_track2_sigd0") ) { fChain->SetBranchAddress("conv_track2_sigd0", &conv_track2_sigd0, &b_conv_track2_sigd0); }
  if( fChain->GetBranchStatus("conv_track2_z0") ) { fChain->SetBranchAddress("conv_track2_z0", &conv_track2_z0, &b_conv_track2_z0); }
  if( fChain->GetBranchStatus("conv_track2_sigz0") ) { fChain->SetBranchAddress("conv_track2_sigz0", &conv_track2_sigz0, &b_conv_track2_sigz0); }
  if( fChain->GetBranchStatus("conv_track2_phi") ) { fChain->SetBranchAddress("conv_track2_phi", &conv_track2_phi, &b_conv_track2_phi); }
  if( fChain->GetBranchStatus("conv_track2_sigphi") ) { fChain->SetBranchAddress("conv_track2_sigphi", &conv_track2_sigphi, &b_conv_track2_sigphi); }
  if( fChain->GetBranchStatus("conv_track2_theta") ) { fChain->SetBranchAddress("conv_track2_theta", &conv_track2_theta, &b_conv_track2_theta); }
  if( fChain->GetBranchStatus("conv_track2_sigtheta") ) { fChain->SetBranchAddress("conv_track2_sigtheta", &conv_track2_sigtheta, &b_conv_track2_sigtheta); }
  if( fChain->GetBranchStatus("conv_track2_qOverP") ) { fChain->SetBranchAddress("conv_track2_qOverP", &conv_track2_qOverP, &b_conv_track2_qOverP); }
  if( fChain->GetBranchStatus("conv_track2_sigqOverP") ) { fChain->SetBranchAddress("conv_track2_sigqOverP", &conv_track2_sigqOverP, &b_conv_track2_sigqOverP); }
  if( fChain->GetBranchStatus("conv_track2_momentum_x") ) { fChain->SetBranchAddress("conv_track2_momentum_x", &conv_track2_momentum_x, &b_conv_track2_momentum_x); }
  if( fChain->GetBranchStatus("conv_track2_momentum_y") ) { fChain->SetBranchAddress("conv_track2_momentum_y", &conv_track2_momentum_y, &b_conv_track2_momentum_y); }
  if( fChain->GetBranchStatus("conv_track2_momentum_z") ) { fChain->SetBranchAddress("conv_track2_momentum_z", &conv_track2_momentum_z, &b_conv_track2_momentum_z); }
  if( fChain->GetBranchStatus("conv_track2_d0g") ) { fChain->SetBranchAddress("conv_track2_d0g", &conv_track2_d0g, &b_conv_track2_d0g); }
  if( fChain->GetBranchStatus("conv_track2_sigd0g") ) { fChain->SetBranchAddress("conv_track2_sigd0g", &conv_track2_sigd0g, &b_conv_track2_sigd0g); }
  if( fChain->GetBranchStatus("conv_track2_z0g") ) { fChain->SetBranchAddress("conv_track2_z0g", &conv_track2_z0g, &b_conv_track2_z0g); }
  if( fChain->GetBranchStatus("conv_track2_sigz0g") ) { fChain->SetBranchAddress("conv_track2_sigz0g", &conv_track2_sigz0g, &b_conv_track2_sigz0g); }
  if( fChain->GetBranchStatus("conv_numOfTruthConversions") ) { fChain->SetBranchAddress("conv_numOfTruthConversions", &conv_numOfTruthConversions, &b_conv_numOfTruthConversions); }
  if( fChain->GetBranchStatus("conv_true_vtx_x") ) { fChain->SetBranchAddress("conv_true_vtx_x", &conv_true_vtx_x, &b_conv_true_vtx_x); }
  if( fChain->GetBranchStatus("conv_true_vtx_y") ) { fChain->SetBranchAddress("conv_true_vtx_y", &conv_true_vtx_y, &b_conv_true_vtx_y); }
  if( fChain->GetBranchStatus("conv_true_vtx_z") ) { fChain->SetBranchAddress("conv_true_vtx_z", &conv_true_vtx_z, &b_conv_true_vtx_z); }
  if( fChain->GetBranchStatus("jetNumCJets") ) { fChain->SetBranchAddress("jetNumCJets", &jetNumCJets, &b_CJetsjetNum); }
  if( fChain->GetBranchStatus("jetEtaCJets") ) { fChain->SetBranchAddress("jetEtaCJets", &jetEtaCJets, &b_jetEtaCJets); }
  if( fChain->GetBranchStatus("jetPhiCJets") ) { fChain->SetBranchAddress("jetPhiCJets", &jetPhiCJets, &b_jetPhiCJets); }
  if( fChain->GetBranchStatus("jetECJets") ) { fChain->SetBranchAddress("jetECJets", &jetECJets, &b_jetECJets); }
  if( fChain->GetBranchStatus("jetEemCJets") ) { fChain->SetBranchAddress("jetEemCJets", &jetEemCJets, &b_jetEemCJets); }
  if( fChain->GetBranchStatus("jetEtCJets") ) { fChain->SetBranchAddress("jetEtCJets", &jetEtCJets, &b_jetEtCJets); }
  if( fChain->GetBranchStatus("jetMCJets") ) { fChain->SetBranchAddress("jetMCJets", &jetMCJets, &b_jetMCJets); }
  if( fChain->GetBranchStatus("jetPxCJets") ) { fChain->SetBranchAddress("jetPxCJets", &jetPxCJets, &b_jetPxCJets); }
  if( fChain->GetBranchStatus("jetPyCJets") ) { fChain->SetBranchAddress("jetPyCJets", &jetPyCJets, &b_jetPyCJets); }
  if( fChain->GetBranchStatus("jetPzCJets") ) { fChain->SetBranchAddress("jetPzCJets", &jetPzCJets, &b_jetPzCJets); }
  if( fChain->GetBranchStatus("jetSizeCJets") ) { fChain->SetBranchAddress("jetSizeCJets", &jetSizeCJets, &b_jetSizeCJets); }
  if( fChain->GetBranchStatus("jetNumC4Jets") ) { fChain->SetBranchAddress("jetNumC4Jets", &jetNumC4Jets, &b_C4JetsjetNum); }
  if( fChain->GetBranchStatus("jetEtaC4Jets") ) { fChain->SetBranchAddress("jetEtaC4Jets", &jetEtaC4Jets, &b_jetEtaC4Jets); }
  if( fChain->GetBranchStatus("jetPhiC4Jets") ) { fChain->SetBranchAddress("jetPhiC4Jets", &jetPhiC4Jets, &b_jetPhiC4Jets); }
  if( fChain->GetBranchStatus("jetEC4Jets") ) { fChain->SetBranchAddress("jetEC4Jets", &jetEC4Jets, &b_jetEC4Jets); }
  if( fChain->GetBranchStatus("jetEemC4Jets") ) { fChain->SetBranchAddress("jetEemC4Jets", &jetEemC4Jets, &b_jetEemC4Jets); }
  if( fChain->GetBranchStatus("jetEtC4Jets") ) { fChain->SetBranchAddress("jetEtC4Jets", &jetEtC4Jets, &b_jetEtC4Jets); }
  if( fChain->GetBranchStatus("jetMC4Jets") ) { fChain->SetBranchAddress("jetMC4Jets", &jetMC4Jets, &b_jetMC4Jets); }
  if( fChain->GetBranchStatus("jetPxC4Jets") ) { fChain->SetBranchAddress("jetPxC4Jets", &jetPxC4Jets, &b_jetPxC4Jets); }
  if( fChain->GetBranchStatus("jetPyC4Jets") ) { fChain->SetBranchAddress("jetPyC4Jets", &jetPyC4Jets, &b_jetPyC4Jets); }
  if( fChain->GetBranchStatus("jetPzC4Jets") ) { fChain->SetBranchAddress("jetPzC4Jets", &jetPzC4Jets, &b_jetPzC4Jets); }
  if( fChain->GetBranchStatus("jetSizeC4Jets") ) { fChain->SetBranchAddress("jetSizeC4Jets", &jetSizeC4Jets, &b_jetSizeC4Jets); }
  if( fChain->GetBranchStatus("jetNumKt6Jets") ) { fChain->SetBranchAddress("jetNumKt6Jets", &jetNumKt6Jets, &b_Kt6JetsjetNum); }
  if( fChain->GetBranchStatus("jetEtaKt6Jets") ) { fChain->SetBranchAddress("jetEtaKt6Jets", &jetEtaKt6Jets, &b_jetEtaKt6Jets); }
  if( fChain->GetBranchStatus("jetPhiKt6Jets") ) { fChain->SetBranchAddress("jetPhiKt6Jets", &jetPhiKt6Jets, &b_jetPhiKt6Jets); }
  if( fChain->GetBranchStatus("jetEKt6Jets") ) { fChain->SetBranchAddress("jetEKt6Jets", &jetEKt6Jets, &b_jetEKt6Jets); }
  if( fChain->GetBranchStatus("jetEemKt6Jets") ) { fChain->SetBranchAddress("jetEemKt6Jets", &jetEemKt6Jets, &b_jetEemKt6Jets); }
  if( fChain->GetBranchStatus("jetEtKt6Jets") ) { fChain->SetBranchAddress("jetEtKt6Jets", &jetEtKt6Jets, &b_jetEtKt6Jets); }
  if( fChain->GetBranchStatus("jetMKt6Jets") ) { fChain->SetBranchAddress("jetMKt6Jets", &jetMKt6Jets, &b_jetMKt6Jets); }
  if( fChain->GetBranchStatus("jetPxKt6Jets") ) { fChain->SetBranchAddress("jetPxKt6Jets", &jetPxKt6Jets, &b_jetPxKt6Jets); }
  if( fChain->GetBranchStatus("jetPyKt6Jets") ) { fChain->SetBranchAddress("jetPyKt6Jets", &jetPyKt6Jets, &b_jetPyKt6Jets); }
  if( fChain->GetBranchStatus("jetPzKt6Jets") ) { fChain->SetBranchAddress("jetPzKt6Jets", &jetPzKt6Jets, &b_jetPzKt6Jets); }
  if( fChain->GetBranchStatus("jetSizeKt6Jets") ) { fChain->SetBranchAddress("jetSizeKt6Jets", &jetSizeKt6Jets, &b_jetSizeKt6Jets); }
  if( fChain->GetBranchStatus("jetNumKt4Jets") ) { fChain->SetBranchAddress("jetNumKt4Jets", &jetNumKt4Jets, &b_Kt4JetsjetNum); }
  if( fChain->GetBranchStatus("jetEtaKt4Jets") ) { fChain->SetBranchAddress("jetEtaKt4Jets", &jetEtaKt4Jets, &b_jetEtaKt4Jets); }
  if( fChain->GetBranchStatus("jetPhiKt4Jets") ) { fChain->SetBranchAddress("jetPhiKt4Jets", &jetPhiKt4Jets, &b_jetPhiKt4Jets); }
  if( fChain->GetBranchStatus("jetEKt4Jets") ) { fChain->SetBranchAddress("jetEKt4Jets", &jetEKt4Jets, &b_jetEKt4Jets); }
  if( fChain->GetBranchStatus("jetEemKt4Jets") ) { fChain->SetBranchAddress("jetEemKt4Jets", &jetEemKt4Jets, &b_jetEemKt4Jets); }
  if( fChain->GetBranchStatus("jetEtKt4Jets") ) { fChain->SetBranchAddress("jetEtKt4Jets", &jetEtKt4Jets, &b_jetEtKt4Jets); }
  if( fChain->GetBranchStatus("jetMKt4Jets") ) { fChain->SetBranchAddress("jetMKt4Jets", &jetMKt4Jets, &b_jetMKt4Jets); }
  if( fChain->GetBranchStatus("jetPxKt4Jets") ) { fChain->SetBranchAddress("jetPxKt4Jets", &jetPxKt4Jets, &b_jetPxKt4Jets); }
  if( fChain->GetBranchStatus("jetPyKt4Jets") ) { fChain->SetBranchAddress("jetPyKt4Jets", &jetPyKt4Jets, &b_jetPyKt4Jets); }
  if( fChain->GetBranchStatus("jetPzKt4Jets") ) { fChain->SetBranchAddress("jetPzKt4Jets", &jetPzKt4Jets, &b_jetPzKt4Jets); }
  if( fChain->GetBranchStatus("jetSizeKt4Jets") ) { fChain->SetBranchAddress("jetSizeKt4Jets", &jetSizeKt4Jets, &b_jetSizeKt4Jets); }
  if( fChain->GetBranchStatus("jetNumCTopoJets") ) { fChain->SetBranchAddress("jetNumCTopoJets", &jetNumCTopoJets, &b_CTopoJetsjetNum); }
  if( fChain->GetBranchStatus("jetEtaCTopoJets") ) { fChain->SetBranchAddress("jetEtaCTopoJets", &jetEtaCTopoJets, &b_jetEtaCTopoJets); }
  if( fChain->GetBranchStatus("jetPhiCTopoJets") ) { fChain->SetBranchAddress("jetPhiCTopoJets", &jetPhiCTopoJets, &b_jetPhiCTopoJets); }
  if( fChain->GetBranchStatus("jetECTopoJets") ) { fChain->SetBranchAddress("jetECTopoJets", &jetECTopoJets, &b_jetECTopoJets); }
  if( fChain->GetBranchStatus("jetEemCTopoJets") ) { fChain->SetBranchAddress("jetEemCTopoJets", &jetEemCTopoJets, &b_jetEemCTopoJets); }
  if( fChain->GetBranchStatus("jetEtCTopoJets") ) { fChain->SetBranchAddress("jetEtCTopoJets", &jetEtCTopoJets, &b_jetEtCTopoJets); }
  if( fChain->GetBranchStatus("jetMCTopoJets") ) { fChain->SetBranchAddress("jetMCTopoJets", &jetMCTopoJets, &b_jetMCTopoJets); }
  if( fChain->GetBranchStatus("jetPxCTopoJets") ) { fChain->SetBranchAddress("jetPxCTopoJets", &jetPxCTopoJets, &b_jetPxCTopoJets); }
  if( fChain->GetBranchStatus("jetPyCTopoJets") ) { fChain->SetBranchAddress("jetPyCTopoJets", &jetPyCTopoJets, &b_jetPyCTopoJets); }
  if( fChain->GetBranchStatus("jetPzCTopoJets") ) { fChain->SetBranchAddress("jetPzCTopoJets", &jetPzCTopoJets, &b_jetPzCTopoJets); }
  if( fChain->GetBranchStatus("jetSizeCTopoJets") ) { fChain->SetBranchAddress("jetSizeCTopoJets", &jetSizeCTopoJets, &b_jetSizeCTopoJets); }
  if( fChain->GetBranchStatus("jetNumC4TopoJets") ) { fChain->SetBranchAddress("jetNumC4TopoJets", &jetNumC4TopoJets, &b_C4TopoJetsjetNum); }
  if( fChain->GetBranchStatus("jetEtaC4TopoJets") ) { fChain->SetBranchAddress("jetEtaC4TopoJets", &jetEtaC4TopoJets, &b_jetEtaC4TopoJets); }
  if( fChain->GetBranchStatus("jetPhiC4TopoJets") ) { fChain->SetBranchAddress("jetPhiC4TopoJets", &jetPhiC4TopoJets, &b_jetPhiC4TopoJets); }
  if( fChain->GetBranchStatus("jetEC4TopoJets") ) { fChain->SetBranchAddress("jetEC4TopoJets", &jetEC4TopoJets, &b_jetEC4TopoJets); }
  if( fChain->GetBranchStatus("jetEemC4TopoJets") ) { fChain->SetBranchAddress("jetEemC4TopoJets", &jetEemC4TopoJets, &b_jetEemC4TopoJets); }
  if( fChain->GetBranchStatus("jetEtC4TopoJets") ) { fChain->SetBranchAddress("jetEtC4TopoJets", &jetEtC4TopoJets, &b_jetEtC4TopoJets); }
  if( fChain->GetBranchStatus("jetMC4TopoJets") ) { fChain->SetBranchAddress("jetMC4TopoJets", &jetMC4TopoJets, &b_jetMC4TopoJets); }
  if( fChain->GetBranchStatus("jetPxC4TopoJets") ) { fChain->SetBranchAddress("jetPxC4TopoJets", &jetPxC4TopoJets, &b_jetPxC4TopoJets); }
  if( fChain->GetBranchStatus("jetPyC4TopoJets") ) { fChain->SetBranchAddress("jetPyC4TopoJets", &jetPyC4TopoJets, &b_jetPyC4TopoJets); }
  if( fChain->GetBranchStatus("jetPzC4TopoJets") ) { fChain->SetBranchAddress("jetPzC4TopoJets", &jetPzC4TopoJets, &b_jetPzC4TopoJets); }
  if( fChain->GetBranchStatus("jetSizeC4TopoJets") ) { fChain->SetBranchAddress("jetSizeC4TopoJets", &jetSizeC4TopoJets, &b_jetSizeC4TopoJets); }
  if( fChain->GetBranchStatus("jetNumKt6TopoJets") ) { fChain->SetBranchAddress("jetNumKt6TopoJets", &jetNumKt6TopoJets, &b_Kt6TopoJetsjetNum); }
  if( fChain->GetBranchStatus("jetEtaKt6TopoJets") ) { fChain->SetBranchAddress("jetEtaKt6TopoJets", &jetEtaKt6TopoJets, &b_jetEtaKt6TopoJets); }
  if( fChain->GetBranchStatus("jetPhiKt6TopoJets") ) { fChain->SetBranchAddress("jetPhiKt6TopoJets", &jetPhiKt6TopoJets, &b_jetPhiKt6TopoJets); }
  if( fChain->GetBranchStatus("jetEKt6TopoJets") ) { fChain->SetBranchAddress("jetEKt6TopoJets", &jetEKt6TopoJets, &b_jetEKt6TopoJets); }
  if( fChain->GetBranchStatus("jetEemKt6TopoJets") ) { fChain->SetBranchAddress("jetEemKt6TopoJets", &jetEemKt6TopoJets, &b_jetEemKt6TopoJets); }
  if( fChain->GetBranchStatus("jetEtKt6TopoJets") ) { fChain->SetBranchAddress("jetEtKt6TopoJets", &jetEtKt6TopoJets, &b_jetEtKt6TopoJets); }
  if( fChain->GetBranchStatus("jetMKt6TopoJets") ) { fChain->SetBranchAddress("jetMKt6TopoJets", &jetMKt6TopoJets, &b_jetMKt6TopoJets); }
  if( fChain->GetBranchStatus("jetPxKt6TopoJets") ) { fChain->SetBranchAddress("jetPxKt6TopoJets", &jetPxKt6TopoJets, &b_jetPxKt6TopoJets); }
  if( fChain->GetBranchStatus("jetPyKt6TopoJets") ) { fChain->SetBranchAddress("jetPyKt6TopoJets", &jetPyKt6TopoJets, &b_jetPyKt6TopoJets); }
  if( fChain->GetBranchStatus("jetPzKt6TopoJets") ) { fChain->SetBranchAddress("jetPzKt6TopoJets", &jetPzKt6TopoJets, &b_jetPzKt6TopoJets); }
  if( fChain->GetBranchStatus("jetSizeKt6TopoJets") ) { fChain->SetBranchAddress("jetSizeKt6TopoJets", &jetSizeKt6TopoJets, &b_jetSizeKt6TopoJets); }
  if( fChain->GetBranchStatus("jetNumKt4TopoJets") ) { fChain->SetBranchAddress("jetNumKt4TopoJets", &jetNumKt4TopoJets, &b_Kt4TopoJetsjetNum); }
  if( fChain->GetBranchStatus("jetEtaKt4TopoJets") ) { fChain->SetBranchAddress("jetEtaKt4TopoJets", &jetEtaKt4TopoJets, &b_jetEtaKt4TopoJets); }
  if( fChain->GetBranchStatus("jetPhiKt4TopoJets") ) { fChain->SetBranchAddress("jetPhiKt4TopoJets", &jetPhiKt4TopoJets, &b_jetPhiKt4TopoJets); }
  if( fChain->GetBranchStatus("jetEKt4TopoJets") ) { fChain->SetBranchAddress("jetEKt4TopoJets", &jetEKt4TopoJets, &b_jetEKt4TopoJets); }
  if( fChain->GetBranchStatus("jetEemKt4TopoJets") ) { fChain->SetBranchAddress("jetEemKt4TopoJets", &jetEemKt4TopoJets, &b_jetEemKt4TopoJets); }
  if( fChain->GetBranchStatus("jetEtKt4TopoJets") ) { fChain->SetBranchAddress("jetEtKt4TopoJets", &jetEtKt4TopoJets, &b_jetEtKt4TopoJets); }
  if( fChain->GetBranchStatus("jetMKt4TopoJets") ) { fChain->SetBranchAddress("jetMKt4TopoJets", &jetMKt4TopoJets, &b_jetMKt4TopoJets); }
  if( fChain->GetBranchStatus("jetPxKt4TopoJets") ) { fChain->SetBranchAddress("jetPxKt4TopoJets", &jetPxKt4TopoJets, &b_jetPxKt4TopoJets); }
  if( fChain->GetBranchStatus("jetPyKt4TopoJets") ) { fChain->SetBranchAddress("jetPyKt4TopoJets", &jetPyKt4TopoJets, &b_jetPyKt4TopoJets); }
  if( fChain->GetBranchStatus("jetPzKt4TopoJets") ) { fChain->SetBranchAddress("jetPzKt4TopoJets", &jetPzKt4TopoJets, &b_jetPzKt4TopoJets); }
  if( fChain->GetBranchStatus("jetSizeKt4TopoJets") ) { fChain->SetBranchAddress("jetSizeKt4TopoJets", &jetSizeKt4TopoJets, &b_jetSizeKt4TopoJets); }
  if( fChain->GetBranchStatus("jetNumCMcJets") ) { fChain->SetBranchAddress("jetNumCMcJets", &jetNumCMcJets, &b_CMcJetsjetNum); }
  if( fChain->GetBranchStatus("jetEtaCMcJets") ) { fChain->SetBranchAddress("jetEtaCMcJets", &jetEtaCMcJets, &b_jetEtaCMcJets); }
  if( fChain->GetBranchStatus("jetPhiCMcJets") ) { fChain->SetBranchAddress("jetPhiCMcJets", &jetPhiCMcJets, &b_jetPhiCMcJets); }
  if( fChain->GetBranchStatus("jetECMcJets") ) { fChain->SetBranchAddress("jetECMcJets", &jetECMcJets, &b_jetECMcJets); }
  if( fChain->GetBranchStatus("jetEemCMcJets") ) { fChain->SetBranchAddress("jetEemCMcJets", &jetEemCMcJets, &b_jetEemCMcJets); }
  if( fChain->GetBranchStatus("jetEtCMcJets") ) { fChain->SetBranchAddress("jetEtCMcJets", &jetEtCMcJets, &b_jetEtCMcJets); }
  if( fChain->GetBranchStatus("jetMCMcJets") ) { fChain->SetBranchAddress("jetMCMcJets", &jetMCMcJets, &b_jetMCMcJets); }
  if( fChain->GetBranchStatus("jetPxCMcJets") ) { fChain->SetBranchAddress("jetPxCMcJets", &jetPxCMcJets, &b_jetPxCMcJets); }
  if( fChain->GetBranchStatus("jetPyCMcJets") ) { fChain->SetBranchAddress("jetPyCMcJets", &jetPyCMcJets, &b_jetPyCMcJets); }
  if( fChain->GetBranchStatus("jetPzCMcJets") ) { fChain->SetBranchAddress("jetPzCMcJets", &jetPzCMcJets, &b_jetPzCMcJets); }
  if( fChain->GetBranchStatus("jetSizeCMcJets") ) { fChain->SetBranchAddress("jetSizeCMcJets", &jetSizeCMcJets, &b_jetSizeCMcJets); }
  if( fChain->GetBranchStatus("jetNumC4McJets") ) { fChain->SetBranchAddress("jetNumC4McJets", &jetNumC4McJets, &b_C4McJetsjetNum); }
  if( fChain->GetBranchStatus("jetEtaC4McJets") ) { fChain->SetBranchAddress("jetEtaC4McJets", &jetEtaC4McJets, &b_jetEtaC4McJets); }
  if( fChain->GetBranchStatus("jetPhiC4McJets") ) { fChain->SetBranchAddress("jetPhiC4McJets", &jetPhiC4McJets, &b_jetPhiC4McJets); }
  if( fChain->GetBranchStatus("jetEC4McJets") ) { fChain->SetBranchAddress("jetEC4McJets", &jetEC4McJets, &b_jetEC4McJets); }
  if( fChain->GetBranchStatus("jetEemC4McJets") ) { fChain->SetBranchAddress("jetEemC4McJets", &jetEemC4McJets, &b_jetEemC4McJets); }
  if( fChain->GetBranchStatus("jetEtC4McJets") ) { fChain->SetBranchAddress("jetEtC4McJets", &jetEtC4McJets, &b_jetEtC4McJets); }
  if( fChain->GetBranchStatus("jetMC4McJets") ) { fChain->SetBranchAddress("jetMC4McJets", &jetMC4McJets, &b_jetMC4McJets); }
  if( fChain->GetBranchStatus("jetPxC4McJets") ) { fChain->SetBranchAddress("jetPxC4McJets", &jetPxC4McJets, &b_jetPxC4McJets); }
  if( fChain->GetBranchStatus("jetPyC4McJets") ) { fChain->SetBranchAddress("jetPyC4McJets", &jetPyC4McJets, &b_jetPyC4McJets); }
  if( fChain->GetBranchStatus("jetPzC4McJets") ) { fChain->SetBranchAddress("jetPzC4McJets", &jetPzC4McJets, &b_jetPzC4McJets); }
  if( fChain->GetBranchStatus("jetSizeC4McJets") ) { fChain->SetBranchAddress("jetSizeC4McJets", &jetSizeC4McJets, &b_jetSizeC4McJets); }
  if( fChain->GetBranchStatus("jetNumKt4McJets") ) { fChain->SetBranchAddress("jetNumKt4McJets", &jetNumKt4McJets, &b_Kt4McJetsjetNum); }
  if( fChain->GetBranchStatus("jetEtaKt4McJets") ) { fChain->SetBranchAddress("jetEtaKt4McJets", &jetEtaKt4McJets, &b_jetEtaKt4McJets); }
  if( fChain->GetBranchStatus("jetPhiKt4McJets") ) { fChain->SetBranchAddress("jetPhiKt4McJets", &jetPhiKt4McJets, &b_jetPhiKt4McJets); }
  if( fChain->GetBranchStatus("jetEKt4McJets") ) { fChain->SetBranchAddress("jetEKt4McJets", &jetEKt4McJets, &b_jetEKt4McJets); }
  if( fChain->GetBranchStatus("jetEemKt4McJets") ) { fChain->SetBranchAddress("jetEemKt4McJets", &jetEemKt4McJets, &b_jetEemKt4McJets); }
  if( fChain->GetBranchStatus("jetEtKt4McJets") ) { fChain->SetBranchAddress("jetEtKt4McJets", &jetEtKt4McJets, &b_jetEtKt4McJets); }
  if( fChain->GetBranchStatus("jetMKt4McJets") ) { fChain->SetBranchAddress("jetMKt4McJets", &jetMKt4McJets, &b_jetMKt4McJets); }
  if( fChain->GetBranchStatus("jetPxKt4McJets") ) { fChain->SetBranchAddress("jetPxKt4McJets", &jetPxKt4McJets, &b_jetPxKt4McJets); }
  if( fChain->GetBranchStatus("jetPyKt4McJets") ) { fChain->SetBranchAddress("jetPyKt4McJets", &jetPyKt4McJets, &b_jetPyKt4McJets); }
  if( fChain->GetBranchStatus("jetPzKt4McJets") ) { fChain->SetBranchAddress("jetPzKt4McJets", &jetPzKt4McJets, &b_jetPzKt4McJets); }
  if( fChain->GetBranchStatus("jetSizeKt4McJets") ) { fChain->SetBranchAddress("jetSizeKt4McJets", &jetSizeKt4McJets, &b_jetSizeKt4McJets); }
  if( fChain->GetBranchStatus("jetNumKt6McJets") ) { fChain->SetBranchAddress("jetNumKt6McJets", &jetNumKt6McJets, &b_Kt6McJetsjetNum); }
  if( fChain->GetBranchStatus("jetEtaKt6McJets") ) { fChain->SetBranchAddress("jetEtaKt6McJets", &jetEtaKt6McJets, &b_jetEtaKt6McJets); }
  if( fChain->GetBranchStatus("jetPhiKt6McJets") ) { fChain->SetBranchAddress("jetPhiKt6McJets", &jetPhiKt6McJets, &b_jetPhiKt6McJets); }
  if( fChain->GetBranchStatus("jetEKt6McJets") ) { fChain->SetBranchAddress("jetEKt6McJets", &jetEKt6McJets, &b_jetEKt6McJets); }
  if( fChain->GetBranchStatus("jetEemKt6McJets") ) { fChain->SetBranchAddress("jetEemKt6McJets", &jetEemKt6McJets, &b_jetEemKt6McJets); }
  if( fChain->GetBranchStatus("jetEtKt6McJets") ) { fChain->SetBranchAddress("jetEtKt6McJets", &jetEtKt6McJets, &b_jetEtKt6McJets); }
  if( fChain->GetBranchStatus("jetMKt6McJets") ) { fChain->SetBranchAddress("jetMKt6McJets", &jetMKt6McJets, &b_jetMKt6McJets); }
  if( fChain->GetBranchStatus("jetPxKt6McJets") ) { fChain->SetBranchAddress("jetPxKt6McJets", &jetPxKt6McJets, &b_jetPxKt6McJets); }
  if( fChain->GetBranchStatus("jetPyKt6McJets") ) { fChain->SetBranchAddress("jetPyKt6McJets", &jetPyKt6McJets, &b_jetPyKt6McJets); }
  if( fChain->GetBranchStatus("jetPzKt6McJets") ) { fChain->SetBranchAddress("jetPzKt6McJets", &jetPzKt6McJets, &b_jetPzKt6McJets); }
  if( fChain->GetBranchStatus("jetSizeKt6McJets") ) { fChain->SetBranchAddress("jetSizeKt6McJets", &jetSizeKt6McJets, &b_jetSizeKt6McJets); }
  if( fChain->GetBranchStatus("tau_NCand") ) { fChain->SetBranchAddress("tau_NCand", &tau_NCand, &b_tau_NCand); }
  if( fChain->GetBranchStatus("tau_ET") ) { fChain->SetBranchAddress("tau_ET", &tau_ET, &b_tau_ET); }
  if( fChain->GetBranchStatus("tau_eta") ) { fChain->SetBranchAddress("tau_eta", &tau_eta, &b_tau_eta); }
  if( fChain->GetBranchStatus("tau_phi") ) { fChain->SetBranchAddress("tau_phi", &tau_phi, &b_tau_phi); }
  if( fChain->GetBranchStatus("tau_nStrip") ) { fChain->SetBranchAddress("tau_nStrip", &tau_nStrip, &b_tau_nStrip); }
  if( fChain->GetBranchStatus("tau_stripWidth2") ) { fChain->SetBranchAddress("tau_stripWidth2", &tau_stripWidth2, &b_tau_stripWidth2); }
  if( fChain->GetBranchStatus("tau_EMRadius") ) { fChain->SetBranchAddress("tau_EMRadius", &tau_EMRadius, &b_tau_EMRadius); }
  if( fChain->GetBranchStatus("tau_IsoFrac") ) { fChain->SetBranchAddress("tau_IsoFrac", &tau_IsoFrac, &b_tau_IsoFrac); }
  if( fChain->GetBranchStatus("tau_ntrack") ) { fChain->SetBranchAddress("tau_ntrack", &tau_ntrack, &b_tau_ntrack); }
  if( fChain->GetBranchStatus("tau_charge") ) { fChain->SetBranchAddress("tau_charge", &tau_charge, &b_tau_charge); }
  if( fChain->GetBranchStatus("tau_secVtxX") ) { fChain->SetBranchAddress("tau_secVtxX", &tau_secVtxX, &b_tau_secVtxX); }
  if( fChain->GetBranchStatus("tau_secVtxY") ) { fChain->SetBranchAddress("tau_secVtxY", &tau_secVtxY, &b_tau_secVtxY); }
  if( fChain->GetBranchStatus("tau_secVtxZ") ) { fChain->SetBranchAddress("tau_secVtxZ", &tau_secVtxZ, &b_tau_secVtxZ); }
  if( fChain->GetBranchStatus("tau_secVtxXError") ) { fChain->SetBranchAddress("tau_secVtxXError", &tau_secVtxXError, &b_tau_secVtxXError); }
  if( fChain->GetBranchStatus("tau_secVtxYError") ) { fChain->SetBranchAddress("tau_secVtxYError", &tau_secVtxYError, &b_tau_secVtxYError); }
  if( fChain->GetBranchStatus("tau_secVtxZError") ) { fChain->SetBranchAddress("tau_secVtxZError", &tau_secVtxZError, &b_tau_secVtxZError); }
  if( fChain->GetBranchStatus("tau_sumEMe") ) { fChain->SetBranchAddress("tau_sumEMe", &tau_sumEMe, &b_tau_sumEMe); }
  if( fChain->GetBranchStatus("tau_sumEMpx") ) { fChain->SetBranchAddress("tau_sumEMpx", &tau_sumEMpx, &b_tau_sumEMpx); }
  if( fChain->GetBranchStatus("tau_sumEMpy") ) { fChain->SetBranchAddress("tau_sumEMpy", &tau_sumEMpy, &b_tau_sumEMpy); }
  if( fChain->GetBranchStatus("tau_sumEMpz") ) { fChain->SetBranchAddress("tau_sumEMpz", &tau_sumEMpz, &b_tau_sumEMpz); }
  if( fChain->GetBranchStatus("tau_energy") ) { fChain->SetBranchAddress("tau_energy", &tau_energy, &b_tau_energy); }
  if( fChain->GetBranchStatus("tau_EMenergy") ) { fChain->SetBranchAddress("tau_EMenergy", &tau_EMenergy, &b_tau_EMenergy); }
  if( fChain->GetBranchStatus("tau_nEMCell") ) { fChain->SetBranchAddress("tau_nEMCell", &tau_nEMCell, &b_tau_nEMCell); }
  if( fChain->GetBranchStatus("tau_stripET") ) { fChain->SetBranchAddress("tau_stripET", &tau_stripET, &b_tau_stripET); }
  if( fChain->GetBranchStatus("tau_ptTrack1") ) { fChain->SetBranchAddress("tau_ptTrack1", &tau_ptTrack1, &b_tau_ptTrack1); }
  if( fChain->GetBranchStatus("tau_etaTrack1") ) { fChain->SetBranchAddress("tau_etaTrack1", &tau_etaTrack1, &b_tau_etaTrack1); }
  if( fChain->GetBranchStatus("tau_phiTrack1") ) { fChain->SetBranchAddress("tau_phiTrack1", &tau_phiTrack1, &b_tau_phiTrack1); }
  if( fChain->GetBranchStatus("tau_distTrack1") ) { fChain->SetBranchAddress("tau_distTrack1", &tau_distTrack1, &b_tau_distTrack1); }
  if( fChain->GetBranchStatus("tau_ptTrack2") ) { fChain->SetBranchAddress("tau_ptTrack2", &tau_ptTrack2, &b_tau_ptTrack2); }
  if( fChain->GetBranchStatus("tau_etaTrack2") ) { fChain->SetBranchAddress("tau_etaTrack2", &tau_etaTrack2, &b_tau_etaTrack2); }
  if( fChain->GetBranchStatus("tau_phiTrack2") ) { fChain->SetBranchAddress("tau_phiTrack2", &tau_phiTrack2, &b_tau_phiTrack2); }
  if( fChain->GetBranchStatus("tau_distTrack2") ) { fChain->SetBranchAddress("tau_distTrack2", &tau_distTrack2, &b_tau_distTrack2); }
  if( fChain->GetBranchStatus("tau_ptTrack3") ) { fChain->SetBranchAddress("tau_ptTrack3", &tau_ptTrack3, &b_tau_ptTrack3); }
  if( fChain->GetBranchStatus("tau_etaTrack3") ) { fChain->SetBranchAddress("tau_etaTrack3", &tau_etaTrack3, &b_tau_etaTrack3); }
  if( fChain->GetBranchStatus("tau_phiTrack3") ) { fChain->SetBranchAddress("tau_phiTrack3", &tau_phiTrack3, &b_tau_phiTrack3); }
  if( fChain->GetBranchStatus("tau_distTrack3") ) { fChain->SetBranchAddress("tau_distTrack3", &tau_distTrack3, &b_tau_distTrack3); }
  if( fChain->GetBranchStatus("tau_etEMCalib") ) { fChain->SetBranchAddress("tau_etEMCalib", &tau_etEMCalib, &b_tau_etEMCalib); }
  if( fChain->GetBranchStatus("tau_etHadCalib") ) { fChain->SetBranchAddress("tau_etHadCalib", &tau_etHadCalib, &b_tau_etHadCalib); }
  if( fChain->GetBranchStatus("tau_Likelihood") ) { fChain->SetBranchAddress("tau_Likelihood", &tau_Likelihood, &b_tau_Likelihood); }
  if( fChain->GetBranchStatus("tau_tauJetLL") ) { fChain->SetBranchAddress("tau_tauJetLL", &tau_tauJetLL, &b_tau_tauJetLL); }
  if( fChain->GetBranchStatus("tau_tauELL") ) { fChain->SetBranchAddress("tau_tauELL", &tau_tauELL, &b_tau_tauELL); }
  if( fChain->GetBranchStatus("tau_tauJetNN") ) { fChain->SetBranchAddress("tau_tauJetNN", &tau_tauJetNN, &b_tau_tauJetNN); }
  if( fChain->GetBranchStatus("tau_tauENN") ) { fChain->SetBranchAddress("tau_tauENN", &tau_tauENN, &b_tau_tauENN); }
  if( fChain->GetBranchStatus("tau_nTracksdrdR") ) { fChain->SetBranchAddress("tau_nTracksdrdR", &tau_nTracksdrdR, &b_tau_nTracksdrdR); }
  if( fChain->GetBranchStatus("tau_accept") ) { fChain->SetBranchAddress("tau_accept", &tau_accept, &b_tau_accept); }
  if( fChain->GetBranchStatus("tau1p3p_NCand") ) { fChain->SetBranchAddress("tau1p3p_NCand", &tau1p3p_NCand, &b_tau1p3p_NCand); }
  if( fChain->GetBranchStatus("tau1p3p_ET") ) { fChain->SetBranchAddress("tau1p3p_ET", &tau1p3p_ET, &b_tau1p3p_ET); }
  if( fChain->GetBranchStatus("tau1p3p_eta") ) { fChain->SetBranchAddress("tau1p3p_eta", &tau1p3p_eta, &b_tau1p3p_eta); }
  if( fChain->GetBranchStatus("tau1p3p_phi") ) { fChain->SetBranchAddress("tau1p3p_phi", &tau1p3p_phi, &b_tau1p3p_phi); }
  if( fChain->GetBranchStatus("tau1p3p_nStrip") ) { fChain->SetBranchAddress("tau1p3p_nStrip", &tau1p3p_nStrip, &b_tau1p3p_nStrip); }
  if( fChain->GetBranchStatus("tau1p3p_stripWidth2") ) { fChain->SetBranchAddress("tau1p3p_stripWidth2", &tau1p3p_stripWidth2, &b_tau1p3p_stripWidth2); }
  if( fChain->GetBranchStatus("tau1p3p_EMRadius") ) { fChain->SetBranchAddress("tau1p3p_EMRadius", &tau1p3p_EMRadius, &b_tau1p3p_EMRadius); }
  if( fChain->GetBranchStatus("tau1p3p_IsoFrac") ) { fChain->SetBranchAddress("tau1p3p_IsoFrac", &tau1p3p_IsoFrac, &b_tau1p3p_IsoFrac); }
  if( fChain->GetBranchStatus("tau1p3p_ntrack") ) { fChain->SetBranchAddress("tau1p3p_ntrack", &tau1p3p_ntrack, &b_tau1p3p_ntrack); }
  if( fChain->GetBranchStatus("tau1p3p_charge") ) { fChain->SetBranchAddress("tau1p3p_charge", &tau1p3p_charge, &b_tau1p3p_charge); }
  if( fChain->GetBranchStatus("tau1p3p_secVtxX") ) { fChain->SetBranchAddress("tau1p3p_secVtxX", &tau1p3p_secVtxX, &b_tau1p3p_secVtxX); }
  if( fChain->GetBranchStatus("tau1p3p_secVtxY") ) { fChain->SetBranchAddress("tau1p3p_secVtxY", &tau1p3p_secVtxY, &b_tau1p3p_secVtxY); }
  if( fChain->GetBranchStatus("tau1p3p_secVtxZ") ) { fChain->SetBranchAddress("tau1p3p_secVtxZ", &tau1p3p_secVtxZ, &b_tau1p3p_secVtxZ); }
  if( fChain->GetBranchStatus("tau1p3p_secVtxXError") ) { fChain->SetBranchAddress("tau1p3p_secVtxXError", &tau1p3p_secVtxXError, &b_tau1p3p_secVtxXError); }
  if( fChain->GetBranchStatus("tau1p3p_secVtxYError") ) { fChain->SetBranchAddress("tau1p3p_secVtxYError", &tau1p3p_secVtxYError, &b_tau1p3p_secVtxYError); }
  if( fChain->GetBranchStatus("tau1p3p_secVtxZError") ) { fChain->SetBranchAddress("tau1p3p_secVtxZError", &tau1p3p_secVtxZError, &b_tau1p3p_secVtxZError); }
  if( fChain->GetBranchStatus("tau1p3p_sumEMe") ) { fChain->SetBranchAddress("tau1p3p_sumEMe", &tau1p3p_sumEMe, &b_tau1p3p_sumEMe); }
  if( fChain->GetBranchStatus("tau1p3p_sumEMpx") ) { fChain->SetBranchAddress("tau1p3p_sumEMpx", &tau1p3p_sumEMpx, &b_tau1p3p_sumEMpx); }
  if( fChain->GetBranchStatus("tau1p3p_sumEMpy") ) { fChain->SetBranchAddress("tau1p3p_sumEMpy", &tau1p3p_sumEMpy, &b_tau1p3p_sumEMpy); }
  if( fChain->GetBranchStatus("tau1p3p_sumEMpz") ) { fChain->SetBranchAddress("tau1p3p_sumEMpz", &tau1p3p_sumEMpz, &b_tau1p3p_sumEMpz); }
  if( fChain->GetBranchStatus("tau1p3p_track1Nt") ) { fChain->SetBranchAddress("tau1p3p_track1Nt", &tau1p3p_track1Nt, &b_tau1p3p_track1Nt); }
  if( fChain->GetBranchStatus("tau1p3p_track2Nt") ) { fChain->SetBranchAddress("tau1p3p_track2Nt", &tau1p3p_track2Nt, &b_tau1p3p_track2Nt); }
  if( fChain->GetBranchStatus("tau1p3p_track3Nt") ) { fChain->SetBranchAddress("tau1p3p_track3Nt", &tau1p3p_track3Nt, &b_tau1p3p_track3Nt); }
  if( fChain->GetBranchStatus("tau1p3p_ptTrack1") ) { fChain->SetBranchAddress("tau1p3p_ptTrack1", &tau1p3p_ptTrack1, &b_tau1p3p_ptTrack1); }
  if( fChain->GetBranchStatus("tau1p3p_ptTrack2") ) { fChain->SetBranchAddress("tau1p3p_ptTrack2", &tau1p3p_ptTrack2, &b_tau1p3p_ptTrack2); }
  if( fChain->GetBranchStatus("tau1p3p_ptTrack3") ) { fChain->SetBranchAddress("tau1p3p_ptTrack3", &tau1p3p_ptTrack3, &b_tau1p3p_ptTrack3); }
  if( fChain->GetBranchStatus("tau1p3p_track1ETChrgEM01") ) { fChain->SetBranchAddress("tau1p3p_track1ETChrgEM01", &tau1p3p_track1ETChrgEM01, &b_tau1p3p_track1ETChrgEM01); }
  if( fChain->GetBranchStatus("tau1p3p_track2ETChrgEM01") ) { fChain->SetBranchAddress("tau1p3p_track2ETChrgEM01", &tau1p3p_track2ETChrgEM01, &b_tau1p3p_track2ETChrgEM01); }
  if( fChain->GetBranchStatus("tau1p3p_track3ETChrgEM01") ) { fChain->SetBranchAddress("tau1p3p_track3ETChrgEM01", &tau1p3p_track3ETChrgEM01, &b_tau1p3p_track3ETChrgEM01); }
  if( fChain->GetBranchStatus("tau1p3p_track1ETResChrgEMTrk") ) { fChain->SetBranchAddress("tau1p3p_track1ETResChrgEMTrk", &tau1p3p_track1ETResChrgEMTrk, &b_tau1p3p_track1ETResChrgEMTrk); }
  if( fChain->GetBranchStatus("tau1p3p_track2ETResChrgEMTrk") ) { fChain->SetBranchAddress("tau1p3p_track2ETResChrgEMTrk", &tau1p3p_track2ETResChrgEMTrk, &b_tau1p3p_track2ETResChrgEMTrk); }
  if( fChain->GetBranchStatus("tau1p3p_track3ETResChrgEMTrk") ) { fChain->SetBranchAddress("tau1p3p_track3ETResChrgEMTrk", &tau1p3p_track3ETResChrgEMTrk, &b_tau1p3p_track3ETResChrgEMTrk); }
  if( fChain->GetBranchStatus("tau1p3p_etEMCL") ) { fChain->SetBranchAddress("tau1p3p_etEMCL", &tau1p3p_etEMCL, &b_tau1p3p_etEMCL); }
  if( fChain->GetBranchStatus("tau1p3p_etNeuEM") ) { fChain->SetBranchAddress("tau1p3p_etNeuEM", &tau1p3p_etNeuEM, &b_tau1p3p_etNeuEM); }
  if( fChain->GetBranchStatus("tau1p3p_etChrgEM") ) { fChain->SetBranchAddress("tau1p3p_etChrgEM", &tau1p3p_etChrgEM, &b_tau1p3p_etChrgEM); }
  if( fChain->GetBranchStatus("tau1p3p_etResNeuEM") ) { fChain->SetBranchAddress("tau1p3p_etResNeuEM", &tau1p3p_etResNeuEM, &b_tau1p3p_etResNeuEM); }
  if( fChain->GetBranchStatus("tau1p3p_cellsChrgHAD") ) { fChain->SetBranchAddress("tau1p3p_cellsChrgHAD", &tau1p3p_cellsChrgHAD, &b_tau1p3p_cellsChrgHAD); }
  if( fChain->GetBranchStatus("tau1p3p_cellsOtherEM") ) { fChain->SetBranchAddress("tau1p3p_cellsOtherEM", &tau1p3p_cellsOtherEM, &b_tau1p3p_cellsOtherEM); }
  if( fChain->GetBranchStatus("tau1p3p_cellsOtherHAD") ) { fChain->SetBranchAddress("tau1p3p_cellsOtherHAD", &tau1p3p_cellsOtherHAD, &b_tau1p3p_cellsOtherHAD); }
  if( fChain->GetBranchStatus("tau1p3p_ETeflow") ) { fChain->SetBranchAddress("tau1p3p_ETeflow", &tau1p3p_ETeflow, &b_tau1p3p_ETeflow); }
  if( fChain->GetBranchStatus("tau1p3p_ETcalo") ) { fChain->SetBranchAddress("tau1p3p_ETcalo", &tau1p3p_ETcalo, &b_tau1p3p_ETcalo); }
  if( fChain->GetBranchStatus("tau1p3p_MVisEflow") ) { fChain->SetBranchAddress("tau1p3p_MVisEflow", &tau1p3p_MVisEflow, &b_tau1p3p_MVisEflow); }
  if( fChain->GetBranchStatus("tau1p3p_discriCut") ) { fChain->SetBranchAddress("tau1p3p_discriCut", &tau1p3p_discriCut, &b_tau1p3p_discriCut); }
  if( fChain->GetBranchStatus("tau1p3p_discriNN") ) { fChain->SetBranchAddress("tau1p3p_discriNN", &tau1p3p_discriNN, &b_tau1p3p_discriNN); }
  if( fChain->GetBranchStatus("tau1p3p_discriPDRS") ) { fChain->SetBranchAddress("tau1p3p_discriPDRS", &tau1p3p_discriPDRS, &b_tau1p3p_discriPDRS); }
  if( fChain->GetBranchStatus("tau1p3p_MTrk3P") ) { fChain->SetBranchAddress("tau1p3p_MTrk3P", &tau1p3p_MTrk3P, &b_tau1p3p_MTrk3P); }
  if( fChain->GetBranchStatus("tau1p3p_NAssocTracksCore") ) { fChain->SetBranchAddress("tau1p3p_NAssocTracksCore", &tau1p3p_NAssocTracksCore, &b_tau1p3p_NAssocTracksCore); }
  if( fChain->GetBranchStatus("tau1p3p_NAssocTracksIsol") ) { fChain->SetBranchAddress("tau1p3p_NAssocTracksIsol", &tau1p3p_NAssocTracksIsol, &b_tau1p3p_NAssocTracksIsol); }
  if( fChain->GetBranchStatus("tau1p3p_RWidth2Trk3P") ) { fChain->SetBranchAddress("tau1p3p_RWidth2Trk3P", &tau1p3p_RWidth2Trk3P, &b_tau1p3p_RWidth2Trk3P); }
  if( fChain->GetBranchStatus("tau1p3p_SignD0Trk3P") ) { fChain->SetBranchAddress("tau1p3p_SignD0Trk3P", &tau1p3p_SignD0Trk3P, &b_tau1p3p_SignD0Trk3P); }
  if( fChain->GetBranchStatus("eg_nc") ) { fChain->SetBranchAddress("eg_nc", &eg_nc, &b_eg_nc); }
  if( fChain->GetBranchStatus("eg_e") ) { fChain->SetBranchAddress("eg_e", &eg_e, &b_eg_e); }
  if( fChain->GetBranchStatus("eg_eta") ) { fChain->SetBranchAddress("eg_eta", &eg_eta, &b_eg_eta); }
  if( fChain->GetBranchStatus("eg_phi") ) { fChain->SetBranchAddress("eg_phi", &eg_phi, &b_eg_phi); }
  if( fChain->GetBranchStatus("eg_cl_et") ) { fChain->SetBranchAddress("eg_cl_et", &eg_cl_et, &b_eg_cl_et); }
  if( fChain->GetBranchStatus("eg_cl_eta") ) { fChain->SetBranchAddress("eg_cl_eta", &eg_cl_eta, &b_eg_cl_eta); }
  if( fChain->GetBranchStatus("eg_cl_phi") ) { fChain->SetBranchAddress("eg_cl_phi", &eg_cl_phi, &b_eg_cl_phi); }
  if( fChain->GetBranchStatus("eg_etap") ) { fChain->SetBranchAddress("eg_etap", &eg_etap, &b_eg_etap); }
  if( fChain->GetBranchStatus("eg_zvertex") ) { fChain->SetBranchAddress("eg_zvertex", &eg_zvertex, &b_eg_zvertex); }
  if( fChain->GetBranchStatus("eg_errz") ) { fChain->SetBranchAddress("eg_errz", &eg_errz, &b_eg_errz); }
  if( fChain->GetBranchStatus("eg_depth") ) { fChain->SetBranchAddress("eg_depth", &eg_depth, &b_eg_depth); }
  if( fChain->GetBranchStatus("eg_e0") ) { fChain->SetBranchAddress("eg_e0", &eg_e0, &b_eg_e0); }
  if( fChain->GetBranchStatus("eg_e1") ) { fChain->SetBranchAddress("eg_e1", &eg_e1, &b_eg_e1); }
  if( fChain->GetBranchStatus("eg_e2") ) { fChain->SetBranchAddress("eg_e2", &eg_e2, &b_eg_e2); }
  if( fChain->GetBranchStatus("eg_e3") ) { fChain->SetBranchAddress("eg_e3", &eg_e3, &b_eg_e3); }
  if( fChain->GetBranchStatus("eg_eta0") ) { fChain->SetBranchAddress("eg_eta0", &eg_eta0, &b_eg_eta0); }
  if( fChain->GetBranchStatus("eg_eta1") ) { fChain->SetBranchAddress("eg_eta1", &eg_eta1, &b_eg_eta1); }
  if( fChain->GetBranchStatus("eg_eta2") ) { fChain->SetBranchAddress("eg_eta2", &eg_eta2, &b_eg_eta2); }
  if( fChain->GetBranchStatus("eg_eta3") ) { fChain->SetBranchAddress("eg_eta3", &eg_eta3, &b_eg_eta3); }
  if( fChain->GetBranchStatus("eg_phi0") ) { fChain->SetBranchAddress("eg_phi0", &eg_phi0, &b_eg_phi0); }
  if( fChain->GetBranchStatus("eg_phi1") ) { fChain->SetBranchAddress("eg_phi1", &eg_phi1, &b_eg_phi1); }
  if( fChain->GetBranchStatus("eg_phi2") ) { fChain->SetBranchAddress("eg_phi2", &eg_phi2, &b_eg_phi2); }
  if( fChain->GetBranchStatus("eg_phi3") ) { fChain->SetBranchAddress("eg_phi3", &eg_phi3, &b_eg_phi3); }
  if( fChain->GetBranchStatus("eg_Etha1") ) { fChain->SetBranchAddress("eg_Etha1", &eg_Etha1, &b_eg_Etha1); }
  if( fChain->GetBranchStatus("eg_Etha") ) { fChain->SetBranchAddress("eg_Etha", &eg_Etha, &b_eg_Etha); }
  if( fChain->GetBranchStatus("eg_Eha1") ) { fChain->SetBranchAddress("eg_Eha1", &eg_Eha1, &b_eg_Eha1); }
  if( fChain->GetBranchStatus("eg_F1") ) { fChain->SetBranchAddress("eg_F1", &eg_F1, &b_eg_F1); }
  if( fChain->GetBranchStatus("eg_F3") ) { fChain->SetBranchAddress("eg_F3", &eg_F3, &b_eg_F3); }
  if( fChain->GetBranchStatus("eg_E233") ) { fChain->SetBranchAddress("eg_E233", &eg_E233, &b_eg_E233); }
  if( fChain->GetBranchStatus("eg_E237") ) { fChain->SetBranchAddress("eg_E237", &eg_E237, &b_eg_E237); }
  if( fChain->GetBranchStatus("eg_E277") ) { fChain->SetBranchAddress("eg_E277", &eg_E277, &b_eg_E277); }
  if( fChain->GetBranchStatus("eg_Weta1") ) { fChain->SetBranchAddress("eg_Weta1", &eg_Weta1, &b_eg_Weta1); }
  if( fChain->GetBranchStatus("eg_Weta2") ) { fChain->SetBranchAddress("eg_Weta2", &eg_Weta2, &b_eg_Weta2); }
  if( fChain->GetBranchStatus("eg_E2ts1") ) { fChain->SetBranchAddress("eg_E2ts1", &eg_E2ts1, &b_eg_E2ts1); }
  if( fChain->GetBranchStatus("eg_E2tsts1") ) { fChain->SetBranchAddress("eg_E2tsts1", &eg_E2tsts1, &b_eg_E2tsts1); }
  if( fChain->GetBranchStatus("eg_Widths1") ) { fChain->SetBranchAddress("eg_Widths1", &eg_Widths1, &b_eg_Widths1); }
  if( fChain->GetBranchStatus("eg_Widths2") ) { fChain->SetBranchAddress("eg_Widths2", &eg_Widths2, &b_eg_Widths2); }
  if( fChain->GetBranchStatus("eg_poscs1") ) { fChain->SetBranchAddress("eg_poscs1", &eg_poscs1, &b_eg_poscs1); }
  if( fChain->GetBranchStatus("eg_poscs2") ) { fChain->SetBranchAddress("eg_poscs2", &eg_poscs2, &b_eg_poscs2); }
  if( fChain->GetBranchStatus("eg_Barys1") ) { fChain->SetBranchAddress("eg_Barys1", &eg_Barys1, &b_eg_Barys1); }
  if( fChain->GetBranchStatus("eg_Wtots1") ) { fChain->SetBranchAddress("eg_Wtots1", &eg_Wtots1, &b_eg_Wtots1); }
  if( fChain->GetBranchStatus("eg_Emins1") ) { fChain->SetBranchAddress("eg_Emins1", &eg_Emins1, &b_eg_Emins1); }
  if( fChain->GetBranchStatus("eg_Emaxs1") ) { fChain->SetBranchAddress("eg_Emaxs1", &eg_Emaxs1, &b_eg_Emaxs1); }
  if( fChain->GetBranchStatus("eg_Fracs1") ) { fChain->SetBranchAddress("eg_Fracs1", &eg_Fracs1, &b_eg_Fracs1); }
  if( fChain->GetBranchStatus("eg_EtCone45") ) { fChain->SetBranchAddress("eg_EtCone45", &eg_EtCone45, &b_eg_EtCone45); }
  if( fChain->GetBranchStatus("eg_EtCone20") ) { fChain->SetBranchAddress("eg_EtCone20", &eg_EtCone20, &b_eg_EtCone20); }
  if( fChain->GetBranchStatus("eg_EtCone30") ) { fChain->SetBranchAddress("eg_EtCone30", &eg_EtCone30, &b_eg_EtCone30); }
  if( fChain->GetBranchStatus("eg_EtCone40") ) { fChain->SetBranchAddress("eg_EtCone40", &eg_EtCone40, &b_eg_EtCone40); }
  if( fChain->GetBranchStatus("eg_IsEM") ) { fChain->SetBranchAddress("eg_IsEM", &eg_IsEM, &b_eg_IsEM); }
  if( fChain->GetBranchStatus("eg_epiNN") ) { fChain->SetBranchAddress("eg_epiNN", &eg_epiNN, &b_eg_epiNN); }
  if( fChain->GetBranchStatus("eg_EMWeight") ) { fChain->SetBranchAddress("eg_EMWeight", &eg_EMWeight, &b_eg_EMWeight); }
  if( fChain->GetBranchStatus("eg_PionWeight") ) { fChain->SetBranchAddress("eg_PionWeight", &eg_PionWeight, &b_eg_PionWeight); }
  if( fChain->GetBranchStatus("eg_Hmatrix") ) { fChain->SetBranchAddress("eg_Hmatrix", &eg_Hmatrix, &b_eg_Hmatrix); }
  if( fChain->GetBranchStatus("eg_IsEMse") ) { fChain->SetBranchAddress("eg_IsEMse", &eg_IsEMse, &b_eg_IsEMse); }
  if( fChain->GetBranchStatus("eg_epiNNse") ) { fChain->SetBranchAddress("eg_epiNNse", &eg_epiNNse, &b_eg_epiNNse); }
  if( fChain->GetBranchStatus("eg_EMWeightse") ) { fChain->SetBranchAddress("eg_EMWeightse", &eg_EMWeightse, &b_eg_EMWeightse); }
  if( fChain->GetBranchStatus("eg_PionWeightse") ) { fChain->SetBranchAddress("eg_PionWeightse", &eg_PionWeightse, &b_eg_PionWeightse); }
  if( fChain->GetBranchStatus("eg_F1core") ) { fChain->SetBranchAddress("eg_F1core", &eg_F1core, &b_eg_F1core); }
  if( fChain->GetBranchStatus("eg_F3core") ) { fChain->SetBranchAddress("eg_F3core", &eg_F3core, &b_eg_F3core); }
  if( fChain->GetBranchStatus("eg_Asy1") ) { fChain->SetBranchAddress("eg_Asy1", &eg_Asy1, &b_eg_Asy1); }
  if( fChain->GetBranchStatus("eg_Pos7") ) { fChain->SetBranchAddress("eg_Pos7", &eg_Pos7, &b_eg_Pos7); }
  if( fChain->GetBranchStatus("eg_Isol") ) { fChain->SetBranchAddress("eg_Isol", &eg_Isol, &b_eg_Isol); }
  if( fChain->GetBranchStatus("eg_Ecore") ) { fChain->SetBranchAddress("eg_Ecore", &eg_Ecore, &b_eg_Ecore); }
  if( fChain->GetBranchStatus("eg_E011") ) { fChain->SetBranchAddress("eg_E011", &eg_E011, &b_eg_E011); }
  if( fChain->GetBranchStatus("eg_E033") ) { fChain->SetBranchAddress("eg_E033", &eg_E033, &b_eg_E033); }
  if( fChain->GetBranchStatus("eg_E131") ) { fChain->SetBranchAddress("eg_E131", &eg_E131, &b_eg_E131); }
  if( fChain->GetBranchStatus("eg_E1153") ) { fChain->SetBranchAddress("eg_E1153", &eg_E1153, &b_eg_E1153); }
  if( fChain->GetBranchStatus("eg_E235") ) { fChain->SetBranchAddress("eg_E235", &eg_E235, &b_eg_E235); }
  if( fChain->GetBranchStatus("eg_E255") ) { fChain->SetBranchAddress("eg_E255", &eg_E255, &b_eg_E255); }
  if( fChain->GetBranchStatus("eg_E333") ) { fChain->SetBranchAddress("eg_E333", &eg_E333, &b_eg_E333); }
  if( fChain->GetBranchStatus("eg_E335") ) { fChain->SetBranchAddress("eg_E335", &eg_E335, &b_eg_E335); }
  if( fChain->GetBranchStatus("eg_E337") ) { fChain->SetBranchAddress("eg_E337", &eg_E337, &b_eg_E337); }
  if( fChain->GetBranchStatus("eg_E377") ) { fChain->SetBranchAddress("eg_E377", &eg_E377, &b_eg_E377); }
  if( fChain->GetBranchStatus("eg_trkmatchnt") ) { fChain->SetBranchAddress("eg_trkmatchnt", &eg_trkmatchnt, &b_eg_trkmatchnt); }
  if( fChain->GetBranchStatus("eg_primary") ) { fChain->SetBranchAddress("eg_primary", &eg_primary, &b_eg_primary); }
  if( fChain->GetBranchStatus("eg_numtracks") ) { fChain->SetBranchAddress("eg_numtracks", &eg_numtracks, &b_eg_numtracks); }
  if( fChain->GetBranchStatus("eg_eoverp") ) { fChain->SetBranchAddress("eg_eoverp", &eg_eoverp, &b_eg_eoverp); }
  if( fChain->GetBranchStatus("eg_etacorr") ) { fChain->SetBranchAddress("eg_etacorr", &eg_etacorr, &b_eg_etacorr); }
  if( fChain->GetBranchStatus("eg_deta0") ) { fChain->SetBranchAddress("eg_deta0", &eg_deta0, &b_eg_deta0); }
  if( fChain->GetBranchStatus("eg_dphi0") ) { fChain->SetBranchAddress("eg_dphi0", &eg_dphi0, &b_eg_dphi0); }
  if( fChain->GetBranchStatus("eg_deta1") ) { fChain->SetBranchAddress("eg_deta1", &eg_deta1, &b_eg_deta1); }
  if( fChain->GetBranchStatus("eg_dphi1") ) { fChain->SetBranchAddress("eg_dphi1", &eg_dphi1, &b_eg_dphi1); }
  if( fChain->GetBranchStatus("eg_deta2") ) { fChain->SetBranchAddress("eg_deta2", &eg_deta2, &b_eg_deta2); }
  if( fChain->GetBranchStatus("eg_dphi2") ) { fChain->SetBranchAddress("eg_dphi2", &eg_dphi2, &b_eg_dphi2); }
  if( fChain->GetBranchStatus("eg_deta3") ) { fChain->SetBranchAddress("eg_deta3", &eg_deta3, &b_eg_deta3); }
  if( fChain->GetBranchStatus("eg_dphi3") ) { fChain->SetBranchAddress("eg_dphi3", &eg_dphi3, &b_eg_dphi3); }
  if( fChain->GetBranchStatus("eg_trkopt") ) { fChain->SetBranchAddress("eg_trkopt", &eg_trkopt, &b_eg_trkopt); }
  if( fChain->GetBranchStatus("eg_trkinvpTV") ) { fChain->SetBranchAddress("eg_trkinvpTV", &eg_trkinvpTV, &b_eg_trkinvpTV); }
  if( fChain->GetBranchStatus("eg_trkcotThV") ) { fChain->SetBranchAddress("eg_trkcotThV", &eg_trkcotThV, &b_eg_trkcotThV); }
  if( fChain->GetBranchStatus("eg_trkphiV") ) { fChain->SetBranchAddress("eg_trkphiV", &eg_trkphiV, &b_eg_trkphiV); }
  if( fChain->GetBranchStatus("eg_convTrkMatch") ) { fChain->SetBranchAddress("eg_convTrkMatch", &eg_convTrkMatch, &b_eg_convTrkMatch); }
  if( fChain->GetBranchStatus("eg_convAngleMatch") ) { fChain->SetBranchAddress("eg_convAngleMatch", &eg_convAngleMatch, &b_eg_convAngleMatch); }
  if( fChain->GetBranchStatus("eg_nc_SE") ) { fChain->SetBranchAddress("eg_nc_SE", &eg_nc_SE, &b_eg_nc_SE); }
  if( fChain->GetBranchStatus("eg_e_SE") ) { fChain->SetBranchAddress("eg_e_SE", &eg_e_SE, &b_eg_e_SE); }
  if( fChain->GetBranchStatus("eg_eta_SE") ) { fChain->SetBranchAddress("eg_eta_SE", &eg_eta_SE, &b_eg_eta_SE); }
  if( fChain->GetBranchStatus("eg_phi_SE") ) { fChain->SetBranchAddress("eg_phi_SE", &eg_phi_SE, &b_eg_phi_SE); }
  if( fChain->GetBranchStatus("eg_cl_et_SE") ) { fChain->SetBranchAddress("eg_cl_et_SE", &eg_cl_et_SE, &b_eg_cl_et_SE); }
  if( fChain->GetBranchStatus("eg_cl_eta_SE") ) { fChain->SetBranchAddress("eg_cl_eta_SE", &eg_cl_eta_SE, &b_eg_cl_eta_SE); }
  if( fChain->GetBranchStatus("eg_cl_phi_SE") ) { fChain->SetBranchAddress("eg_cl_phi_SE", &eg_cl_phi_SE, &b_eg_cl_phi_SE); }
  if( fChain->GetBranchStatus("eg_etap_SE") ) { fChain->SetBranchAddress("eg_etap_SE", &eg_etap_SE, &b_eg_etap_SE); }
  if( fChain->GetBranchStatus("eg_zvertex_SE") ) { fChain->SetBranchAddress("eg_zvertex_SE", &eg_zvertex_SE, &b_eg_zvertex_SE); }
  if( fChain->GetBranchStatus("eg_errz_SE") ) { fChain->SetBranchAddress("eg_errz_SE", &eg_errz_SE, &b_eg_errz_SE); }
  if( fChain->GetBranchStatus("eg_depth_SE") ) { fChain->SetBranchAddress("eg_depth_SE", &eg_depth_SE, &b_eg_depth_SE); }
  if( fChain->GetBranchStatus("eg_e0_SE") ) { fChain->SetBranchAddress("eg_e0_SE", &eg_e0_SE, &b_eg_e0_SE); }
  if( fChain->GetBranchStatus("eg_e1_SE") ) { fChain->SetBranchAddress("eg_e1_SE", &eg_e1_SE, &b_eg_e1_SE); }
  if( fChain->GetBranchStatus("eg_e2_SE") ) { fChain->SetBranchAddress("eg_e2_SE", &eg_e2_SE, &b_eg_e2_SE); }
  if( fChain->GetBranchStatus("eg_e3_SE") ) { fChain->SetBranchAddress("eg_e3_SE", &eg_e3_SE, &b_eg_e3_SE); }
  if( fChain->GetBranchStatus("eg_eta0_SE") ) { fChain->SetBranchAddress("eg_eta0_SE", &eg_eta0_SE, &b_eg_eta0_SE); }
  if( fChain->GetBranchStatus("eg_eta1_SE") ) { fChain->SetBranchAddress("eg_eta1_SE", &eg_eta1_SE, &b_eg_eta1_SE); }
  if( fChain->GetBranchStatus("eg_eta2_SE") ) { fChain->SetBranchAddress("eg_eta2_SE", &eg_eta2_SE, &b_eg_eta2_SE); }
  if( fChain->GetBranchStatus("eg_eta3_SE") ) { fChain->SetBranchAddress("eg_eta3_SE", &eg_eta3_SE, &b_eg_eta3_SE); }
  if( fChain->GetBranchStatus("eg_phi0_SE") ) { fChain->SetBranchAddress("eg_phi0_SE", &eg_phi0_SE, &b_eg_phi0_SE); }
  if( fChain->GetBranchStatus("eg_phi1_SE") ) { fChain->SetBranchAddress("eg_phi1_SE", &eg_phi1_SE, &b_eg_phi1_SE); }
  if( fChain->GetBranchStatus("eg_phi2_SE") ) { fChain->SetBranchAddress("eg_phi2_SE", &eg_phi2_SE, &b_eg_phi2_SE); }
  if( fChain->GetBranchStatus("eg_phi3_SE") ) { fChain->SetBranchAddress("eg_phi3_SE", &eg_phi3_SE, &b_eg_phi3_SE); }
  if( fChain->GetBranchStatus("eg_Etha1_SE") ) { fChain->SetBranchAddress("eg_Etha1_SE", &eg_Etha1_SE, &b_eg_Etha1_SE); }
  if( fChain->GetBranchStatus("eg_Etha_SE") ) { fChain->SetBranchAddress("eg_Etha_SE", &eg_Etha_SE, &b_eg_Etha_SE); }
  if( fChain->GetBranchStatus("eg_Eha1_SE") ) { fChain->SetBranchAddress("eg_Eha1_SE", &eg_Eha1_SE, &b_eg_Eha1_SE); }
  if( fChain->GetBranchStatus("eg_F1_SE") ) { fChain->SetBranchAddress("eg_F1_SE", &eg_F1_SE, &b_eg_F1_SE); }
  if( fChain->GetBranchStatus("eg_F3_SE") ) { fChain->SetBranchAddress("eg_F3_SE", &eg_F3_SE, &b_eg_F3_SE); }
  if( fChain->GetBranchStatus("eg_E233_SE") ) { fChain->SetBranchAddress("eg_E233_SE", &eg_E233_SE, &b_eg_E233_SE); }
  if( fChain->GetBranchStatus("eg_E237_SE") ) { fChain->SetBranchAddress("eg_E237_SE", &eg_E237_SE, &b_eg_E237_SE); }
  if( fChain->GetBranchStatus("eg_E277_SE") ) { fChain->SetBranchAddress("eg_E277_SE", &eg_E277_SE, &b_eg_E277_SE); }
  if( fChain->GetBranchStatus("eg_Weta1_SE") ) { fChain->SetBranchAddress("eg_Weta1_SE", &eg_Weta1_SE, &b_eg_Weta1_SE); }
  if( fChain->GetBranchStatus("eg_Weta2_SE") ) { fChain->SetBranchAddress("eg_Weta2_SE", &eg_Weta2_SE, &b_eg_Weta2_SE); }
  if( fChain->GetBranchStatus("eg_E2ts1_SE") ) { fChain->SetBranchAddress("eg_E2ts1_SE", &eg_E2ts1_SE, &b_eg_E2ts1_SE); }
  if( fChain->GetBranchStatus("eg_E2tsts1_SE") ) { fChain->SetBranchAddress("eg_E2tsts1_SE", &eg_E2tsts1_SE, &b_eg_E2tsts1_SE); }
  if( fChain->GetBranchStatus("eg_Widths1_SE") ) { fChain->SetBranchAddress("eg_Widths1_SE", &eg_Widths1_SE, &b_eg_Widths1_SE); }
  if( fChain->GetBranchStatus("eg_Widths2_SE") ) { fChain->SetBranchAddress("eg_Widths2_SE", &eg_Widths2_SE, &b_eg_Widths2_SE); }
  if( fChain->GetBranchStatus("eg_poscs1_SE") ) { fChain->SetBranchAddress("eg_poscs1_SE", &eg_poscs1_SE, &b_eg_poscs1_SE); }
  if( fChain->GetBranchStatus("eg_poscs2_SE") ) { fChain->SetBranchAddress("eg_poscs2_SE", &eg_poscs2_SE, &b_eg_poscs2_SE); }
  if( fChain->GetBranchStatus("eg_Barys1_SE") ) { fChain->SetBranchAddress("eg_Barys1_SE", &eg_Barys1_SE, &b_eg_Barys1_SE); }
  if( fChain->GetBranchStatus("eg_Wtots1_SE") ) { fChain->SetBranchAddress("eg_Wtots1_SE", &eg_Wtots1_SE, &b_eg_Wtots1_SE); }
  if( fChain->GetBranchStatus("eg_Emins1_SE") ) { fChain->SetBranchAddress("eg_Emins1_SE", &eg_Emins1_SE, &b_eg_Emins1_SE); }
  if( fChain->GetBranchStatus("eg_Emaxs1_SE") ) { fChain->SetBranchAddress("eg_Emaxs1_SE", &eg_Emaxs1_SE, &b_eg_Emaxs1_SE); }
  if( fChain->GetBranchStatus("eg_Fracs1_SE") ) { fChain->SetBranchAddress("eg_Fracs1_SE", &eg_Fracs1_SE, &b_eg_Fracs1_SE); }
  if( fChain->GetBranchStatus("eg_EtCone45_SE") ) { fChain->SetBranchAddress("eg_EtCone45_SE", &eg_EtCone45_SE, &b_eg_EtCone45_SE); }
  if( fChain->GetBranchStatus("eg_EtCone20_SE") ) { fChain->SetBranchAddress("eg_EtCone20_SE", &eg_EtCone20_SE, &b_eg_EtCone20_SE); }
  if( fChain->GetBranchStatus("eg_EtCone30_SE") ) { fChain->SetBranchAddress("eg_EtCone30_SE", &eg_EtCone30_SE, &b_eg_EtCone30_SE); }
  if( fChain->GetBranchStatus("eg_EtCone40_SE") ) { fChain->SetBranchAddress("eg_EtCone40_SE", &eg_EtCone40_SE, &b_eg_EtCone40_SE); }
  if( fChain->GetBranchStatus("eg_IsEM_SE") ) { fChain->SetBranchAddress("eg_IsEM_SE", &eg_IsEM_SE, &b_eg_IsEM_SE); }
  if( fChain->GetBranchStatus("eg_epiNN_SE") ) { fChain->SetBranchAddress("eg_epiNN_SE", &eg_epiNN_SE, &b_eg_epiNN_SE); }
  if( fChain->GetBranchStatus("eg_EMWeight_SE") ) { fChain->SetBranchAddress("eg_EMWeight_SE", &eg_EMWeight_SE, &b_eg_EMWeight_SE); }
  if( fChain->GetBranchStatus("eg_PionWeight_SE") ) { fChain->SetBranchAddress("eg_PionWeight_SE", &eg_PionWeight_SE, &b_eg_PionWeight_SE); }
  if( fChain->GetBranchStatus("eg_Hmatrix_SE") ) { fChain->SetBranchAddress("eg_Hmatrix_SE", &eg_Hmatrix_SE, &b_eg_Hmatrix_SE); }
  if( fChain->GetBranchStatus("eg_IsEMse_SE") ) { fChain->SetBranchAddress("eg_IsEMse_SE", &eg_IsEMse_SE, &b_eg_IsEMse_SE); }
  if( fChain->GetBranchStatus("eg_epiNNse_SE") ) { fChain->SetBranchAddress("eg_epiNNse_SE", &eg_epiNNse_SE, &b_eg_epiNNse_SE); }
  if( fChain->GetBranchStatus("eg_EMWeightse_SE") ) { fChain->SetBranchAddress("eg_EMWeightse_SE", &eg_EMWeightse_SE, &b_eg_EMWeightse_SE); }
  if( fChain->GetBranchStatus("eg_PionWeightse_SE") ) { fChain->SetBranchAddress("eg_PionWeightse_SE", &eg_PionWeightse_SE, &b_eg_PionWeightse_SE); }
  if( fChain->GetBranchStatus("eg_F1core_SE") ) { fChain->SetBranchAddress("eg_F1core_SE", &eg_F1core_SE, &b_eg_F1core_SE); }
  if( fChain->GetBranchStatus("eg_F3core_SE") ) { fChain->SetBranchAddress("eg_F3core_SE", &eg_F3core_SE, &b_eg_F3core_SE); }
  if( fChain->GetBranchStatus("eg_Asy1_SE") ) { fChain->SetBranchAddress("eg_Asy1_SE", &eg_Asy1_SE, &b_eg_Asy1_SE); }
  if( fChain->GetBranchStatus("eg_Pos7_SE") ) { fChain->SetBranchAddress("eg_Pos7_SE", &eg_Pos7_SE, &b_eg_Pos7_SE); }
  if( fChain->GetBranchStatus("eg_Isol_SE") ) { fChain->SetBranchAddress("eg_Isol_SE", &eg_Isol_SE, &b_eg_Isol_SE); }
  if( fChain->GetBranchStatus("eg_Ecore_SE") ) { fChain->SetBranchAddress("eg_Ecore_SE", &eg_Ecore_SE, &b_eg_Ecore_SE); }
  if( fChain->GetBranchStatus("eg_E011_SE") ) { fChain->SetBranchAddress("eg_E011_SE", &eg_E011_SE, &b_eg_E011_SE); }
  if( fChain->GetBranchStatus("eg_E033_SE") ) { fChain->SetBranchAddress("eg_E033_SE", &eg_E033_SE, &b_eg_E033_SE); }
  if( fChain->GetBranchStatus("eg_E131_SE") ) { fChain->SetBranchAddress("eg_E131_SE", &eg_E131_SE, &b_eg_E131_SE); }
  if( fChain->GetBranchStatus("eg_E1153_SE") ) { fChain->SetBranchAddress("eg_E1153_SE", &eg_E1153_SE, &b_eg_E1153_SE); }
  if( fChain->GetBranchStatus("eg_E235_SE") ) { fChain->SetBranchAddress("eg_E235_SE", &eg_E235_SE, &b_eg_E235_SE); }
  if( fChain->GetBranchStatus("eg_E255_SE") ) { fChain->SetBranchAddress("eg_E255_SE", &eg_E255_SE, &b_eg_E255_SE); }
  if( fChain->GetBranchStatus("eg_E333_SE") ) { fChain->SetBranchAddress("eg_E333_SE", &eg_E333_SE, &b_eg_E333_SE); }
  if( fChain->GetBranchStatus("eg_E335_SE") ) { fChain->SetBranchAddress("eg_E335_SE", &eg_E335_SE, &b_eg_E335_SE); }
  if( fChain->GetBranchStatus("eg_E337_SE") ) { fChain->SetBranchAddress("eg_E337_SE", &eg_E337_SE, &b_eg_E337_SE); }
  if( fChain->GetBranchStatus("eg_E377_SE") ) { fChain->SetBranchAddress("eg_E377_SE", &eg_E377_SE, &b_eg_E377_SE); }
  if( fChain->GetBranchStatus("eg_trkmatchnt_SE") ) { fChain->SetBranchAddress("eg_trkmatchnt_SE", &eg_trkmatchnt_SE, &b_eg_trkmatchnt_SE); }
  if( fChain->GetBranchStatus("eg_primary_SE") ) { fChain->SetBranchAddress("eg_primary_SE", &eg_primary_SE, &b_eg_primary_SE); }
  if( fChain->GetBranchStatus("eg_numtracks_SE") ) { fChain->SetBranchAddress("eg_numtracks_SE", &eg_numtracks_SE, &b_eg_numtracks_SE); }
  if( fChain->GetBranchStatus("eg_eoverp_SE") ) { fChain->SetBranchAddress("eg_eoverp_SE", &eg_eoverp_SE, &b_eg_eoverp_SE); }
  if( fChain->GetBranchStatus("eg_etacorr_SE") ) { fChain->SetBranchAddress("eg_etacorr_SE", &eg_etacorr_SE, &b_eg_etacorr_SE); }
  if( fChain->GetBranchStatus("eg_deta0_SE") ) { fChain->SetBranchAddress("eg_deta0_SE", &eg_deta0_SE, &b_eg_deta0_SE); }
  if( fChain->GetBranchStatus("eg_dphi0_SE") ) { fChain->SetBranchAddress("eg_dphi0_SE", &eg_dphi0_SE, &b_eg_dphi0_SE); }
  if( fChain->GetBranchStatus("eg_deta1_SE") ) { fChain->SetBranchAddress("eg_deta1_SE", &eg_deta1_SE, &b_eg_deta1_SE); }
  if( fChain->GetBranchStatus("eg_dphi1_SE") ) { fChain->SetBranchAddress("eg_dphi1_SE", &eg_dphi1_SE, &b_eg_dphi1_SE); }
  if( fChain->GetBranchStatus("eg_deta2_SE") ) { fChain->SetBranchAddress("eg_deta2_SE", &eg_deta2_SE, &b_eg_deta2_SE); }
  if( fChain->GetBranchStatus("eg_dphi2_SE") ) { fChain->SetBranchAddress("eg_dphi2_SE", &eg_dphi2_SE, &b_eg_dphi2_SE); }
  if( fChain->GetBranchStatus("eg_deta3_SE") ) { fChain->SetBranchAddress("eg_deta3_SE", &eg_deta3_SE, &b_eg_deta3_SE); }
  if( fChain->GetBranchStatus("eg_dphi3_SE") ) { fChain->SetBranchAddress("eg_dphi3_SE", &eg_dphi3_SE, &b_eg_dphi3_SE); }
  if( fChain->GetBranchStatus("eg_trkopt_SE") ) { fChain->SetBranchAddress("eg_trkopt_SE", &eg_trkopt_SE, &b_eg_trkopt_SE); }
  if( fChain->GetBranchStatus("eg_trkinvpTV_SE") ) { fChain->SetBranchAddress("eg_trkinvpTV_SE", &eg_trkinvpTV_SE, &b_eg_trkinvpTV_SE); }
  if( fChain->GetBranchStatus("eg_trkcotThV_SE") ) { fChain->SetBranchAddress("eg_trkcotThV_SE", &eg_trkcotThV_SE, &b_eg_trkcotThV_SE); }
  if( fChain->GetBranchStatus("eg_trkphiV_SE") ) { fChain->SetBranchAddress("eg_trkphiV_SE", &eg_trkphiV_SE, &b_eg_trkphiV_SE); }
  if( fChain->GetBranchStatus("eg_convTrkMatch_SE") ) { fChain->SetBranchAddress("eg_convTrkMatch_SE", &eg_convTrkMatch_SE, &b_eg_convTrkMatch_SE); }
  if( fChain->GetBranchStatus("eg_convAngleMatch_SE") ) { fChain->SetBranchAddress("eg_convAngleMatch_SE", &eg_convAngleMatch_SE, &b_eg_convAngleMatch_SE); }
  if( fChain->GetBranchStatus("egtruth_nc") ) { fChain->SetBranchAddress("egtruth_nc", &egtruth_nc, &b_egtruth_nc); }
  if( fChain->GetBranchStatus("egtruth_et") ) { fChain->SetBranchAddress("egtruth_et", &egtruth_et, &b_egtruth_et); }
  if( fChain->GetBranchStatus("egtruth_eta") ) { fChain->SetBranchAddress("egtruth_eta", &egtruth_eta, &b_egtruth_eta); }
  if( fChain->GetBranchStatus("egtruth_phi") ) { fChain->SetBranchAddress("egtruth_phi", &egtruth_phi, &b_egtruth_phi); }
  if( fChain->GetBranchStatus("egtruth_id") ) { fChain->SetBranchAddress("egtruth_id", &egtruth_id, &b_egtruth_id); }
  if( fChain->GetBranchStatus("egtruth_barcode") ) { fChain->SetBranchAddress("egtruth_barcode", &egtruth_barcode, &b_egtruth_barcode); }
  if( fChain->GetBranchStatus("egtruth_truthnt") ) { fChain->SetBranchAddress("egtruth_truthnt", &egtruth_truthnt, &b_egtruth_truthnt); }
  if( fChain->GetBranchStatus("egtruth_etIsol") ) { fChain->SetBranchAddress("egtruth_etIsol", &egtruth_etIsol, &b_egtruth_etIsol); }
  if( fChain->GetBranchStatus("egtruth_etaCalo") ) { fChain->SetBranchAddress("egtruth_etaCalo", &egtruth_etaCalo, &b_egtruth_etaCalo); }
  if( fChain->GetBranchStatus("egtruth_phiCalo") ) { fChain->SetBranchAddress("egtruth_phiCalo", &egtruth_phiCalo, &b_egtruth_phiCalo); }
  if( fChain->GetBranchStatus("NTileMuonTag") ) { fChain->SetBranchAddress("NTileMuonTag", &NTileMuonTag, &b_NTileMuonTag); }
  if( fChain->GetBranchStatus("EtaTileMuonTag") ) { fChain->SetBranchAddress("EtaTileMuonTag", &EtaTileMuonTag, &b_EtaTileMuonTag); }
  if( fChain->GetBranchStatus("PhiTileMuonTag") ) { fChain->SetBranchAddress("PhiTileMuonTag", &PhiTileMuonTag, &b_PhiTileMuonTag); }
  if( fChain->GetBranchStatus("EnergyVecTileMuonTag") ) { fChain->SetBranchAddress("EnergyVecTileMuonTag", &EnergyVecTileMuonTag, &b_EnergyVecTileMuonTag); }
  if( fChain->GetBranchStatus("LabelTileMuonTag") ) { fChain->SetBranchAddress("LabelTileMuonTag", &LabelTileMuonTag, &b_LabelTileMuonTag); }
  if( fChain->GetBranchStatus("Muid_nmuonms") ) { fChain->SetBranchAddress("Muid_nmuonms", &Muid_nmuonms, &b_Muid_nmuonms); }
  if( fChain->GetBranchStatus("Muid_a0rms") ) { fChain->SetBranchAddress("Muid_a0rms", &Muid_a0rms, &b_Muid_a0rms); }
  if( fChain->GetBranchStatus("Muid_z0rms") ) { fChain->SetBranchAddress("Muid_z0rms", &Muid_z0rms, &b_Muid_z0rms); }
  if( fChain->GetBranchStatus("Muid_phirms") ) { fChain->SetBranchAddress("Muid_phirms", &Muid_phirms, &b_Muid_phirms); }
  if( fChain->GetBranchStatus("Muid_thetarms") ) { fChain->SetBranchAddress("Muid_thetarms", &Muid_thetarms, &b_Muid_thetarms); }
  if( fChain->GetBranchStatus("Muid_etarms") ) { fChain->SetBranchAddress("Muid_etarms", &Muid_etarms, &b_Muid_etarms); }
  if( fChain->GetBranchStatus("Muid_pirms") ) { fChain->SetBranchAddress("Muid_pirms", &Muid_pirms, &b_Muid_pirms); }
  if( fChain->GetBranchStatus("Muid_chi2ms") ) { fChain->SetBranchAddress("Muid_chi2ms", &Muid_chi2ms, &b_Muid_chi2ms); }
  if( fChain->GetBranchStatus("Muid_mdtms") ) { fChain->SetBranchAddress("Muid_mdtms", &Muid_mdtms, &b_Muid_mdtms); }
  if( fChain->GetBranchStatus("Muid_cscetams") ) { fChain->SetBranchAddress("Muid_cscetams", &Muid_cscetams, &b_Muid_cscetams); }
  if( fChain->GetBranchStatus("Muid_cscphims") ) { fChain->SetBranchAddress("Muid_cscphims", &Muid_cscphims, &b_Muid_cscphims); }
  if( fChain->GetBranchStatus("Muid_rpcetams") ) { fChain->SetBranchAddress("Muid_rpcetams", &Muid_rpcetams, &b_Muid_rpcetams); }
  if( fChain->GetBranchStatus("Muid_rpcphims") ) { fChain->SetBranchAddress("Muid_rpcphims", &Muid_rpcphims, &b_Muid_rpcphims); }
  if( fChain->GetBranchStatus("Muid_tgcetams") ) { fChain->SetBranchAddress("Muid_tgcetams", &Muid_tgcetams, &b_Muid_tgcetams); }
  if( fChain->GetBranchStatus("Muid_tgcphims") ) { fChain->SetBranchAddress("Muid_tgcphims", &Muid_tgcphims, &b_Muid_tgcphims); }
  if( fChain->GetBranchStatus("Muid_covr11ms") ) { fChain->SetBranchAddress("Muid_covr11ms", &Muid_covr11ms, &b_Muid_covr11ms); }
  if( fChain->GetBranchStatus("Muid_covr12ms") ) { fChain->SetBranchAddress("Muid_covr12ms", &Muid_covr12ms, &b_Muid_covr12ms); }
  if( fChain->GetBranchStatus("Muid_covr22ms") ) { fChain->SetBranchAddress("Muid_covr22ms", &Muid_covr22ms, &b_Muid_covr22ms); }
  if( fChain->GetBranchStatus("Muid_covr13ms") ) { fChain->SetBranchAddress("Muid_covr13ms", &Muid_covr13ms, &b_Muid_covr13ms); }
  if( fChain->GetBranchStatus("Muid_covr23ms") ) { fChain->SetBranchAddress("Muid_covr23ms", &Muid_covr23ms, &b_Muid_covr23ms); }
  if( fChain->GetBranchStatus("Muid_covr33ms") ) { fChain->SetBranchAddress("Muid_covr33ms", &Muid_covr33ms, &b_Muid_covr33ms); }
  if( fChain->GetBranchStatus("Muid_covr14ms") ) { fChain->SetBranchAddress("Muid_covr14ms", &Muid_covr14ms, &b_Muid_covr14ms); }
  if( fChain->GetBranchStatus("Muid_covr24ms") ) { fChain->SetBranchAddress("Muid_covr24ms", &Muid_covr24ms, &b_Muid_covr24ms); }
  if( fChain->GetBranchStatus("Muid_covr34ms") ) { fChain->SetBranchAddress("Muid_covr34ms", &Muid_covr34ms, &b_Muid_covr34ms); }
  if( fChain->GetBranchStatus("Muid_covr44ms") ) { fChain->SetBranchAddress("Muid_covr44ms", &Muid_covr44ms, &b_Muid_covr44ms); }
  if( fChain->GetBranchStatus("Muid_covr15ms") ) { fChain->SetBranchAddress("Muid_covr15ms", &Muid_covr15ms, &b_Muid_covr15ms); }
  if( fChain->GetBranchStatus("Muid_covr25ms") ) { fChain->SetBranchAddress("Muid_covr25ms", &Muid_covr25ms, &b_Muid_covr25ms); }
  if( fChain->GetBranchStatus("Muid_covr35ms") ) { fChain->SetBranchAddress("Muid_covr35ms", &Muid_covr35ms, &b_Muid_covr35ms); }
  if( fChain->GetBranchStatus("Muid_covr45ms") ) { fChain->SetBranchAddress("Muid_covr45ms", &Muid_covr45ms, &b_Muid_covr45ms); }
  if( fChain->GetBranchStatus("Muid_covr55ms") ) { fChain->SetBranchAddress("Muid_covr55ms", &Muid_covr55ms, &b_Muid_covr55ms); }
  if( fChain->GetBranchStatus("Muid_nmuonmu") ) { fChain->SetBranchAddress("Muid_nmuonmu", &Muid_nmuonmu, &b_Muid_nmuonmu); }
  if( fChain->GetBranchStatus("Muid_prelossmu") ) { fChain->SetBranchAddress("Muid_prelossmu", &Muid_prelossmu, &b_Muid_prelossmu); }
  if( fChain->GetBranchStatus("Muid_erelossmu") ) { fChain->SetBranchAddress("Muid_erelossmu", &Muid_erelossmu, &b_Muid_erelossmu); }
  if( fChain->GetBranchStatus("Muid_isparelossmu") ) { fChain->SetBranchAddress("Muid_isparelossmu", &Muid_isparelossmu, &b_Muid_isparelossmu); }
  if( fChain->GetBranchStatus("Muid_a0rmu") ) { fChain->SetBranchAddress("Muid_a0rmu", &Muid_a0rmu, &b_Muid_a0rmu); }
  if( fChain->GetBranchStatus("Muid_z0rmu") ) { fChain->SetBranchAddress("Muid_z0rmu", &Muid_z0rmu, &b_Muid_z0rmu); }
  if( fChain->GetBranchStatus("Muid_phirmu") ) { fChain->SetBranchAddress("Muid_phirmu", &Muid_phirmu, &b_Muid_phirmu); }
  if( fChain->GetBranchStatus("Muid_thetarmu") ) { fChain->SetBranchAddress("Muid_thetarmu", &Muid_thetarmu, &b_Muid_thetarmu); }
  if( fChain->GetBranchStatus("Muid_etarmu") ) { fChain->SetBranchAddress("Muid_etarmu", &Muid_etarmu, &b_Muid_etarmu); }
  if( fChain->GetBranchStatus("Muid_pirmu") ) { fChain->SetBranchAddress("Muid_pirmu", &Muid_pirmu, &b_Muid_pirmu); }
  if( fChain->GetBranchStatus("Muid_chi2mu") ) { fChain->SetBranchAddress("Muid_chi2mu", &Muid_chi2mu, &b_Muid_chi2mu); }
  if( fChain->GetBranchStatus("Muid_covr11mu") ) { fChain->SetBranchAddress("Muid_covr11mu", &Muid_covr11mu, &b_Muid_covr11mu); }
  if( fChain->GetBranchStatus("Muid_covr12mu") ) { fChain->SetBranchAddress("Muid_covr12mu", &Muid_covr12mu, &b_Muid_covr12mu); }
  if( fChain->GetBranchStatus("Muid_covr22mu") ) { fChain->SetBranchAddress("Muid_covr22mu", &Muid_covr22mu, &b_Muid_covr22mu); }
  if( fChain->GetBranchStatus("Muid_covr13mu") ) { fChain->SetBranchAddress("Muid_covr13mu", &Muid_covr13mu, &b_Muid_covr13mu); }
  if( fChain->GetBranchStatus("Muid_covr23mu") ) { fChain->SetBranchAddress("Muid_covr23mu", &Muid_covr23mu, &b_Muid_covr23mu); }
  if( fChain->GetBranchStatus("Muid_covr33mu") ) { fChain->SetBranchAddress("Muid_covr33mu", &Muid_covr33mu, &b_Muid_covr33mu); }
  if( fChain->GetBranchStatus("Muid_covr14mu") ) { fChain->SetBranchAddress("Muid_covr14mu", &Muid_covr14mu, &b_Muid_covr14mu); }
  if( fChain->GetBranchStatus("Muid_covr24mu") ) { fChain->SetBranchAddress("Muid_covr24mu", &Muid_covr24mu, &b_Muid_covr24mu); }
  if( fChain->GetBranchStatus("Muid_covr34mu") ) { fChain->SetBranchAddress("Muid_covr34mu", &Muid_covr34mu, &b_Muid_covr34mu); }
  if( fChain->GetBranchStatus("Muid_covr44mu") ) { fChain->SetBranchAddress("Muid_covr44mu", &Muid_covr44mu, &b_Muid_covr44mu); }
  if( fChain->GetBranchStatus("Muid_covr15mu") ) { fChain->SetBranchAddress("Muid_covr15mu", &Muid_covr15mu, &b_Muid_covr15mu); }
  if( fChain->GetBranchStatus("Muid_covr25mu") ) { fChain->SetBranchAddress("Muid_covr25mu", &Muid_covr25mu, &b_Muid_covr25mu); }
  if( fChain->GetBranchStatus("Muid_covr35mu") ) { fChain->SetBranchAddress("Muid_covr35mu", &Muid_covr35mu, &b_Muid_covr35mu); }
  if( fChain->GetBranchStatus("Muid_covr45mu") ) { fChain->SetBranchAddress("Muid_covr45mu", &Muid_covr45mu, &b_Muid_covr45mu); }
  if( fChain->GetBranchStatus("Muid_covr55mu") ) { fChain->SetBranchAddress("Muid_covr55mu", &Muid_covr55mu, &b_Muid_covr55mu); }
  if( fChain->GetBranchStatus("Muid_mslinkmu") ) { fChain->SetBranchAddress("Muid_mslinkmu", &Muid_mslinkmu, &b_Muid_mslinkmu); }
  if( fChain->GetBranchStatus("Muid_nmuoncb") ) { fChain->SetBranchAddress("Muid_nmuoncb", &Muid_nmuoncb, &b_Muid_nmuoncb); }
  if( fChain->GetBranchStatus("Muid_chi2mcb") ) { fChain->SetBranchAddress("Muid_chi2mcb", &Muid_chi2mcb, &b_Muid_chi2mcb); }
  if( fChain->GetBranchStatus("Muid_loosecb") ) { fChain->SetBranchAddress("Muid_loosecb", &Muid_loosecb, &b_Muid_loosecb); }
  if( fChain->GetBranchStatus("Muid_a0rcb") ) { fChain->SetBranchAddress("Muid_a0rcb", &Muid_a0rcb, &b_Muid_a0rcb); }
  if( fChain->GetBranchStatus("Muid_z0rcb") ) { fChain->SetBranchAddress("Muid_z0rcb", &Muid_z0rcb, &b_Muid_z0rcb); }
  if( fChain->GetBranchStatus("Muid_phircb") ) { fChain->SetBranchAddress("Muid_phircb", &Muid_phircb, &b_Muid_phircb); }
  if( fChain->GetBranchStatus("Muid_thetarcb") ) { fChain->SetBranchAddress("Muid_thetarcb", &Muid_thetarcb, &b_Muid_thetarcb); }
  if( fChain->GetBranchStatus("Muid_etarcb") ) { fChain->SetBranchAddress("Muid_etarcb", &Muid_etarcb, &b_Muid_etarcb); }
  if( fChain->GetBranchStatus("Muid_pircb") ) { fChain->SetBranchAddress("Muid_pircb", &Muid_pircb, &b_Muid_pircb); }
  if( fChain->GetBranchStatus("Muid_chi2cb") ) { fChain->SetBranchAddress("Muid_chi2cb", &Muid_chi2cb, &b_Muid_chi2cb); }
  if( fChain->GetBranchStatus("Muid_chi2prcb") ) { fChain->SetBranchAddress("Muid_chi2prcb", &Muid_chi2prcb, &b_Muid_chi2prcb); }
  if( fChain->GetBranchStatus("Muid_covr11cb") ) { fChain->SetBranchAddress("Muid_covr11cb", &Muid_covr11cb, &b_Muid_covr11cb); }
  if( fChain->GetBranchStatus("Muid_covr12cb") ) { fChain->SetBranchAddress("Muid_covr12cb", &Muid_covr12cb, &b_Muid_covr12cb); }
  if( fChain->GetBranchStatus("Muid_covr22cb") ) { fChain->SetBranchAddress("Muid_covr22cb", &Muid_covr22cb, &b_Muid_covr22cb); }
  if( fChain->GetBranchStatus("Muid_covr13cb") ) { fChain->SetBranchAddress("Muid_covr13cb", &Muid_covr13cb, &b_Muid_covr13cb); }
  if( fChain->GetBranchStatus("Muid_covr23cb") ) { fChain->SetBranchAddress("Muid_covr23cb", &Muid_covr23cb, &b_Muid_covr23cb); }
  if( fChain->GetBranchStatus("Muid_covr33cb") ) { fChain->SetBranchAddress("Muid_covr33cb", &Muid_covr33cb, &b_Muid_covr33cb); }
  if( fChain->GetBranchStatus("Muid_covr14cb") ) { fChain->SetBranchAddress("Muid_covr14cb", &Muid_covr14cb, &b_Muid_covr14cb); }
  if( fChain->GetBranchStatus("Muid_covr24cb") ) { fChain->SetBranchAddress("Muid_covr24cb", &Muid_covr24cb, &b_Muid_covr24cb); }
  if( fChain->GetBranchStatus("Muid_covr34cb") ) { fChain->SetBranchAddress("Muid_covr34cb", &Muid_covr34cb, &b_Muid_covr34cb); }
  if( fChain->GetBranchStatus("Muid_covr44cb") ) { fChain->SetBranchAddress("Muid_covr44cb", &Muid_covr44cb, &b_Muid_covr44cb); }
  if( fChain->GetBranchStatus("Muid_covr15cb") ) { fChain->SetBranchAddress("Muid_covr15cb", &Muid_covr15cb, &b_Muid_covr15cb); }
  if( fChain->GetBranchStatus("Muid_covr25cb") ) { fChain->SetBranchAddress("Muid_covr25cb", &Muid_covr25cb, &b_Muid_covr25cb); }
  if( fChain->GetBranchStatus("Muid_covr35cb") ) { fChain->SetBranchAddress("Muid_covr35cb", &Muid_covr35cb, &b_Muid_covr35cb); }
  if( fChain->GetBranchStatus("Muid_covr45cb") ) { fChain->SetBranchAddress("Muid_covr45cb", &Muid_covr45cb, &b_Muid_covr45cb); }
  if( fChain->GetBranchStatus("Muid_covr55cb") ) { fChain->SetBranchAddress("Muid_covr55cb", &Muid_covr55cb, &b_Muid_covr55cb); }
  if( fChain->GetBranchStatus("Muid_mslinkcb") ) { fChain->SetBranchAddress("Muid_mslinkcb", &Muid_mslinkcb, &b_Muid_mslinkcb); }
  if( fChain->GetBranchStatus("Muid_exlinkcb") ) { fChain->SetBranchAddress("Muid_exlinkcb", &Muid_exlinkcb, &b_Muid_exlinkcb); }
  if( fChain->GetBranchStatus("Muid_blayercb") ) { fChain->SetBranchAddress("Muid_blayercb", &Muid_blayercb, &b_Muid_blayercb); }
  if( fChain->GetBranchStatus("Muid_pixelcb") ) { fChain->SetBranchAddress("Muid_pixelcb", &Muid_pixelcb, &b_Muid_pixelcb); }
  if( fChain->GetBranchStatus("Muid_sctcb") ) { fChain->SetBranchAddress("Muid_sctcb", &Muid_sctcb, &b_Muid_sctcb); }
  if( fChain->GetBranchStatus("Muid_trtcb") ) { fChain->SetBranchAddress("Muid_trtcb", &Muid_trtcb, &b_Muid_trtcb); }
  if( fChain->GetBranchStatus("Muid_trthighcb") ) { fChain->SetBranchAddress("Muid_trthighcb", &Muid_trthighcb, &b_Muid_trthighcb); }
  if( fChain->GetBranchStatus("Muid_a0rid") ) { fChain->SetBranchAddress("Muid_a0rid", &Muid_a0rid, &b_Muid_a0rid); }
  if( fChain->GetBranchStatus("Muid_z0rid") ) { fChain->SetBranchAddress("Muid_z0rid", &Muid_z0rid, &b_Muid_z0rid); }
  if( fChain->GetBranchStatus("Muid_phirid") ) { fChain->SetBranchAddress("Muid_phirid", &Muid_phirid, &b_Muid_phirid); }
  if( fChain->GetBranchStatus("Muid_thetarid") ) { fChain->SetBranchAddress("Muid_thetarid", &Muid_thetarid, &b_Muid_thetarid); }
  if( fChain->GetBranchStatus("Muid_etarid") ) { fChain->SetBranchAddress("Muid_etarid", &Muid_etarid, &b_Muid_etarid); }
  if( fChain->GetBranchStatus("Muid_pirid") ) { fChain->SetBranchAddress("Muid_pirid", &Muid_pirid, &b_Muid_pirid); }
  if( fChain->GetBranchStatus("Muid_chi2id") ) { fChain->SetBranchAddress("Muid_chi2id", &Muid_chi2id, &b_Muid_chi2id); }
  if( fChain->GetBranchStatus("Muid_pulla0cb") ) { fChain->SetBranchAddress("Muid_pulla0cb", &Muid_pulla0cb, &b_Muid_pulla0cb); }
  if( fChain->GetBranchStatus("Muid_pullz0cb") ) { fChain->SetBranchAddress("Muid_pullz0cb", &Muid_pullz0cb, &b_Muid_pullz0cb); }
  if( fChain->GetBranchStatus("Muid_pullphicb") ) { fChain->SetBranchAddress("Muid_pullphicb", &Muid_pullphicb, &b_Muid_pullphicb); }
  if( fChain->GetBranchStatus("Muid_pullthecb") ) { fChain->SetBranchAddress("Muid_pullthecb", &Muid_pullthecb, &b_Muid_pullthecb); }
  if( fChain->GetBranchStatus("Muid_pullmomcb") ) { fChain->SetBranchAddress("Muid_pullmomcb", &Muid_pullmomcb, &b_Muid_pullmomcb); }
  if( fChain->GetBranchStatus("staco_nmuon") ) { fChain->SetBranchAddress("staco_nmuon", &staco_nmuon, &b_staco_nmuon); }
  if( fChain->GetBranchStatus("staco_A0") ) { fChain->SetBranchAddress("staco_A0", &staco_A0, &b_staco_A0); }
  if( fChain->GetBranchStatus("staco_Z") ) { fChain->SetBranchAddress("staco_Z", &staco_Z, &b_staco_Z); }
  if( fChain->GetBranchStatus("staco_Phi") ) { fChain->SetBranchAddress("staco_Phi", &staco_Phi, &b_staco_Phi); }
  if( fChain->GetBranchStatus("staco_Theta") ) { fChain->SetBranchAddress("staco_Theta", &staco_Theta, &b_staco_Theta); }
  if( fChain->GetBranchStatus("staco_qOverP") ) { fChain->SetBranchAddress("staco_qOverP", &staco_qOverP, &b_staco_qOverP); }
  if( fChain->GetBranchStatus("staco_chi2") ) { fChain->SetBranchAddress("staco_chi2", &staco_chi2, &b_staco_chi2); }
  if( fChain->GetBranchStatus("staco_covr11") ) { fChain->SetBranchAddress("staco_covr11", &staco_covr11, &b_staco_covr11); }
  if( fChain->GetBranchStatus("staco_covr21") ) { fChain->SetBranchAddress("staco_covr21", &staco_covr21, &b_staco_covr21); }
  if( fChain->GetBranchStatus("staco_covr22") ) { fChain->SetBranchAddress("staco_covr22", &staco_covr22, &b_staco_covr22); }
  if( fChain->GetBranchStatus("staco_covr31") ) { fChain->SetBranchAddress("staco_covr31", &staco_covr31, &b_staco_covr31); }
  if( fChain->GetBranchStatus("staco_covr32") ) { fChain->SetBranchAddress("staco_covr32", &staco_covr32, &b_staco_covr32); }
  if( fChain->GetBranchStatus("staco_covr33") ) { fChain->SetBranchAddress("staco_covr33", &staco_covr33, &b_staco_covr33); }
  if( fChain->GetBranchStatus("staco_covr41") ) { fChain->SetBranchAddress("staco_covr41", &staco_covr41, &b_staco_covr41); }
  if( fChain->GetBranchStatus("staco_covr42") ) { fChain->SetBranchAddress("staco_covr42", &staco_covr42, &b_staco_covr42); }
  if( fChain->GetBranchStatus("staco_covr43") ) { fChain->SetBranchAddress("staco_covr43", &staco_covr43, &b_staco_covr43); }
  if( fChain->GetBranchStatus("staco_covr44") ) { fChain->SetBranchAddress("staco_covr44", &staco_covr44, &b_staco_covr44); }
  if( fChain->GetBranchStatus("staco_covr51") ) { fChain->SetBranchAddress("staco_covr51", &staco_covr51, &b_staco_covr51); }
  if( fChain->GetBranchStatus("staco_covr52") ) { fChain->SetBranchAddress("staco_covr52", &staco_covr52, &b_staco_covr52); }
  if( fChain->GetBranchStatus("staco_covr53") ) { fChain->SetBranchAddress("staco_covr53", &staco_covr53, &b_staco_covr53); }
  if( fChain->GetBranchStatus("staco_covr54") ) { fChain->SetBranchAddress("staco_covr54", &staco_covr54, &b_staco_covr54); }
  if( fChain->GetBranchStatus("staco_covr55") ) { fChain->SetBranchAddress("staco_covr55", &staco_covr55, &b_staco_covr55); }
  if( fChain->GetBranchStatus("staco_A0MS") ) { fChain->SetBranchAddress("staco_A0MS", &staco_A0MS, &b_staco_A0MS); }
  if( fChain->GetBranchStatus("staco_ZMS") ) { fChain->SetBranchAddress("staco_ZMS", &staco_ZMS, &b_staco_ZMS); }
  if( fChain->GetBranchStatus("staco_PhiMS") ) { fChain->SetBranchAddress("staco_PhiMS", &staco_PhiMS, &b_staco_PhiMS); }
  if( fChain->GetBranchStatus("staco_ThetaMS") ) { fChain->SetBranchAddress("staco_ThetaMS", &staco_ThetaMS, &b_staco_ThetaMS); }
  if( fChain->GetBranchStatus("staco_qOverPMS") ) { fChain->SetBranchAddress("staco_qOverPMS", &staco_qOverPMS, &b_staco_qOverPMS); }
  if( fChain->GetBranchStatus("staco_A0ID") ) { fChain->SetBranchAddress("staco_A0ID", &staco_A0ID, &b_staco_A0ID); }
  if( fChain->GetBranchStatus("staco_ZID") ) { fChain->SetBranchAddress("staco_ZID", &staco_ZID, &b_staco_ZID); }
  if( fChain->GetBranchStatus("staco_PhiID") ) { fChain->SetBranchAddress("staco_PhiID", &staco_PhiID, &b_staco_PhiID); }
  if( fChain->GetBranchStatus("staco_ThetaID") ) { fChain->SetBranchAddress("staco_ThetaID", &staco_ThetaID, &b_staco_ThetaID); }
  if( fChain->GetBranchStatus("staco_qOverPID") ) { fChain->SetBranchAddress("staco_qOverPID", &staco_qOverPID, &b_staco_qOverPID); }
  if( fChain->GetBranchStatus("MuTag_nmuon") ) { fChain->SetBranchAddress("MuTag_nmuon", &MuTag_nmuon, &b_MuTag_nmuon); }
  if( fChain->GetBranchStatus("MuTag_A0") ) { fChain->SetBranchAddress("MuTag_A0", &MuTag_A0, &b_MuTag_A0); }
  if( fChain->GetBranchStatus("MuTag_Z") ) { fChain->SetBranchAddress("MuTag_Z", &MuTag_Z, &b_MuTag_Z); }
  if( fChain->GetBranchStatus("MuTag_Phi") ) { fChain->SetBranchAddress("MuTag_Phi", &MuTag_Phi, &b_MuTag_Phi); }
  if( fChain->GetBranchStatus("MuTag_Theta") ) { fChain->SetBranchAddress("MuTag_Theta", &MuTag_Theta, &b_MuTag_Theta); }
  if( fChain->GetBranchStatus("MuTag_qOverP") ) { fChain->SetBranchAddress("MuTag_qOverP", &MuTag_qOverP, &b_MuTag_qOverP); }
  if( fChain->GetBranchStatus("MuTag_covr11") ) { fChain->SetBranchAddress("MuTag_covr11", &MuTag_covr11, &b_MuTag_covr11); }
  if( fChain->GetBranchStatus("MuTag_covr21") ) { fChain->SetBranchAddress("MuTag_covr21", &MuTag_covr21, &b_MuTag_covr21); }
  if( fChain->GetBranchStatus("MuTag_covr22") ) { fChain->SetBranchAddress("MuTag_covr22", &MuTag_covr22, &b_MuTag_covr22); }
  if( fChain->GetBranchStatus("MuTag_covr31") ) { fChain->SetBranchAddress("MuTag_covr31", &MuTag_covr31, &b_MuTag_covr31); }
  if( fChain->GetBranchStatus("MuTag_covr32") ) { fChain->SetBranchAddress("MuTag_covr32", &MuTag_covr32, &b_MuTag_covr32); }
  if( fChain->GetBranchStatus("MuTag_covr33") ) { fChain->SetBranchAddress("MuTag_covr33", &MuTag_covr33, &b_MuTag_covr33); }
  if( fChain->GetBranchStatus("MuTag_covr41") ) { fChain->SetBranchAddress("MuTag_covr41", &MuTag_covr41, &b_MuTag_covr41); }
  if( fChain->GetBranchStatus("MuTag_covr42") ) { fChain->SetBranchAddress("MuTag_covr42", &MuTag_covr42, &b_MuTag_covr42); }
  if( fChain->GetBranchStatus("MuTag_covr43") ) { fChain->SetBranchAddress("MuTag_covr43", &MuTag_covr43, &b_MuTag_covr43); }
  if( fChain->GetBranchStatus("MuTag_covr44") ) { fChain->SetBranchAddress("MuTag_covr44", &MuTag_covr44, &b_MuTag_covr44); }
  if( fChain->GetBranchStatus("MuTag_covr51") ) { fChain->SetBranchAddress("MuTag_covr51", &MuTag_covr51, &b_MuTag_covr51); }
  if( fChain->GetBranchStatus("MuTag_covr52") ) { fChain->SetBranchAddress("MuTag_covr52", &MuTag_covr52, &b_MuTag_covr52); }
  if( fChain->GetBranchStatus("MuTag_covr53") ) { fChain->SetBranchAddress("MuTag_covr53", &MuTag_covr53, &b_MuTag_covr53); }
  if( fChain->GetBranchStatus("MuTag_covr54") ) { fChain->SetBranchAddress("MuTag_covr54", &MuTag_covr54, &b_MuTag_covr54); }
  if( fChain->GetBranchStatus("MuTag_covr55") ) { fChain->SetBranchAddress("MuTag_covr55", &MuTag_covr55, &b_MuTag_covr55); }
  if( fChain->GetBranchStatus("MET_ExMiss") ) { fChain->SetBranchAddress("MET_ExMiss", &MET_ExMiss, &b_MET_ExMiss); }
  if( fChain->GetBranchStatus("MET_EyMiss") ) { fChain->SetBranchAddress("MET_EyMiss", &MET_EyMiss, &b_MET_EyMiss); }
  if( fChain->GetBranchStatus("MET_EtSum") ) { fChain->SetBranchAddress("MET_EtSum", &MET_EtSum, &b_MET_EtSum); }
  if( fChain->GetBranchStatus("MET_ExMissTopo") ) { fChain->SetBranchAddress("MET_ExMissTopo", &MET_ExMissTopo, &b_MET_ExMissTopo); }
  if( fChain->GetBranchStatus("MET_EyMissTopo") ) { fChain->SetBranchAddress("MET_EyMissTopo", &MET_EyMissTopo, &b_MET_EyMissTopo); }
  if( fChain->GetBranchStatus("MET_EtSumTopo") ) { fChain->SetBranchAddress("MET_EtSumTopo", &MET_EtSumTopo, &b_MET_EtSumTopo); }
  if( fChain->GetBranchStatus("MET_ExMissCorr") ) { fChain->SetBranchAddress("MET_ExMissCorr", &MET_ExMissCorr, &b_MET_ExMissCorr); }
  if( fChain->GetBranchStatus("MET_EyMissCorr") ) { fChain->SetBranchAddress("MET_EyMissCorr", &MET_EyMissCorr, &b_MET_EyMissCorr); }
  if( fChain->GetBranchStatus("MET_EtSumCorr") ) { fChain->SetBranchAddress("MET_EtSumCorr", &MET_EtSumCorr, &b_MET_EtSumCorr); }
  if( fChain->GetBranchStatus("MET_ExMissLocHadTopo") ) { fChain->SetBranchAddress("MET_ExMissLocHadTopo", &MET_ExMissLocHadTopo, &b_MET_ExMissLocHadTopo); }
  if( fChain->GetBranchStatus("MET_EyMissLocHadTopo") ) { fChain->SetBranchAddress("MET_EyMissLocHadTopo", &MET_EyMissLocHadTopo, &b_MET_EyMissLocHadTopo); }
  if( fChain->GetBranchStatus("MET_EtSumLocHadTopo") ) { fChain->SetBranchAddress("MET_EtSumLocHadTopo", &MET_EtSumLocHadTopo, &b_MET_EtSumLocHadTopo); }
  if( fChain->GetBranchStatus("MET_ExMissCorrTopo") ) { fChain->SetBranchAddress("MET_ExMissCorrTopo", &MET_ExMissCorrTopo, &b_MET_ExMissCorrTopo); }
  if( fChain->GetBranchStatus("MET_EyMissCorrTopo") ) { fChain->SetBranchAddress("MET_EyMissCorrTopo", &MET_EyMissCorrTopo, &b_MET_EyMissCorrTopo); }
  if( fChain->GetBranchStatus("MET_EtSumCorrTopo") ) { fChain->SetBranchAddress("MET_EtSumCorrTopo", &MET_EtSumCorrTopo, &b_MET_EtSumCorrTopo); }
  if( fChain->GetBranchStatus("MET_ExMissMu") ) { fChain->SetBranchAddress("MET_ExMissMu", &MET_ExMissMu, &b_MET_ExMissMu); }
  if( fChain->GetBranchStatus("MET_EyMissMu") ) { fChain->SetBranchAddress("MET_EyMissMu", &MET_EyMissMu, &b_MET_EyMissMu); }
  if( fChain->GetBranchStatus("MET_EtSumMu") ) { fChain->SetBranchAddress("MET_EtSumMu", &MET_EtSumMu, &b_MET_EtSumMu); }
  if( fChain->GetBranchStatus("MET_ExMissMuBoy") ) { fChain->SetBranchAddress("MET_ExMissMuBoy", &MET_ExMissMuBoy, &b_MET_ExMissMuBoy); }
  if( fChain->GetBranchStatus("MET_EyMissMuBoy") ) { fChain->SetBranchAddress("MET_EyMissMuBoy", &MET_EyMissMuBoy, &b_MET_EyMissMuBoy); }
  if( fChain->GetBranchStatus("MET_EtSumMuBoy") ) { fChain->SetBranchAddress("MET_EtSumMuBoy", &MET_EtSumMuBoy, &b_MET_EtSumMuBoy); }
  if( fChain->GetBranchStatus("MET_ExMissFinal") ) { fChain->SetBranchAddress("MET_ExMissFinal", &MET_ExMissFinal, &b_MET_ExMissFinal); }
  if( fChain->GetBranchStatus("MET_EyMissFinal") ) { fChain->SetBranchAddress("MET_EyMissFinal", &MET_EyMissFinal, &b_MET_EyMissFinal); }
  if( fChain->GetBranchStatus("MET_EtSumFinal") ) { fChain->SetBranchAddress("MET_EtSumFinal", &MET_EtSumFinal, &b_MET_EtSumFinal); }
  if( fChain->GetBranchStatus("MET_ExMissRefFinal") ) { fChain->SetBranchAddress("MET_ExMissRefFinal", &MET_ExMissRefFinal, &b_MET_ExMissRefFinal); }
  if( fChain->GetBranchStatus("MET_EyMissRefFinal") ) { fChain->SetBranchAddress("MET_EyMissRefFinal", &MET_EyMissRefFinal, &b_MET_EyMissRefFinal); }
  if( fChain->GetBranchStatus("MET_EtSumRefFinal") ) { fChain->SetBranchAddress("MET_EtSumRefFinal", &MET_EtSumRefFinal, &b_MET_EtSumRefFinal); }
  if( fChain->GetBranchStatus("MET_ExMissCryo") ) { fChain->SetBranchAddress("MET_ExMissCryo", &MET_ExMissCryo, &b_MET_ExMissCryo); }
  if( fChain->GetBranchStatus("MET_EyMissCryo") ) { fChain->SetBranchAddress("MET_EyMissCryo", &MET_EyMissCryo, &b_MET_EyMissCryo); }
  if( fChain->GetBranchStatus("MET_EtSumCryo") ) { fChain->SetBranchAddress("MET_EtSumCryo", &MET_EtSumCryo, &b_MET_EtSumCryo); }
  if( fChain->GetBranchStatus("MET_ExMissCryoCone") ) { fChain->SetBranchAddress("MET_ExMissCryoCone", &MET_ExMissCryoCone, &b_MET_ExMissCryoCone); }
  if( fChain->GetBranchStatus("MET_EyMissCryoCone") ) { fChain->SetBranchAddress("MET_EyMissCryoCone", &MET_EyMissCryoCone, &b_MET_EyMissCryoCone); }
  if( fChain->GetBranchStatus("MET_EtSumCryoCone") ) { fChain->SetBranchAddress("MET_EtSumCryoCone", &MET_EtSumCryoCone, &b_MET_EtSumCryoCone); }
  if( fChain->GetBranchStatus("MET_ExMissLocHadTopoObj") ) { fChain->SetBranchAddress("MET_ExMissLocHadTopoObj", &MET_ExMissLocHadTopoObj, &b_MET_ExMissLocHadTopoObj); }
  if( fChain->GetBranchStatus("MET_EyMissLocHadTopoObj") ) { fChain->SetBranchAddress("MET_EyMissLocHadTopoObj", &MET_EyMissLocHadTopoObj, &b_MET_EyMissLocHadTopoObj); }
  if( fChain->GetBranchStatus("MET_EtSumLocHadTopoObj") ) { fChain->SetBranchAddress("MET_EtSumLocHadTopoObj", &MET_EtSumLocHadTopoObj, &b_MET_EtSumLocHadTopoObj); }
  if( fChain->GetBranchStatus("MET_ExMissTopoObj") ) { fChain->SetBranchAddress("MET_ExMissTopoObj", &MET_ExMissTopoObj, &b_MET_ExMissTopoObj); }
  if( fChain->GetBranchStatus("MET_EyMissTopoObj") ) { fChain->SetBranchAddress("MET_EyMissTopoObj", &MET_EyMissTopoObj, &b_MET_EyMissTopoObj); }
  if( fChain->GetBranchStatus("MET_EtSumTopoObj") ) { fChain->SetBranchAddress("MET_EtSumTopoObj", &MET_EtSumTopoObj, &b_MET_EtSumTopoObj); }
  if( fChain->GetBranchStatus("MET_ExMissRefEle") ) { fChain->SetBranchAddress("MET_ExMissRefEle", &MET_ExMissRefEle, &b_MET_ExMissRefEle); }
  if( fChain->GetBranchStatus("MET_EyMissRefEle") ) { fChain->SetBranchAddress("MET_EyMissRefEle", &MET_EyMissRefEle, &b_MET_EyMissRefEle); }
  if( fChain->GetBranchStatus("MET_EtSumRefEle") ) { fChain->SetBranchAddress("MET_EtSumRefEle", &MET_EtSumRefEle, &b_MET_EtSumRefEle); }
  if( fChain->GetBranchStatus("MET_ExMissRefMuo") ) { fChain->SetBranchAddress("MET_ExMissRefMuo", &MET_ExMissRefMuo, &b_MET_ExMissRefMuo); }
  if( fChain->GetBranchStatus("MET_EyMissRefMuo") ) { fChain->SetBranchAddress("MET_EyMissRefMuo", &MET_EyMissRefMuo, &b_MET_EyMissRefMuo); }
  if( fChain->GetBranchStatus("MET_EtSumRefMuo") ) { fChain->SetBranchAddress("MET_EtSumRefMuo", &MET_EtSumRefMuo, &b_MET_EtSumRefMuo); }
  if( fChain->GetBranchStatus("MET_ExMissRefJet") ) { fChain->SetBranchAddress("MET_ExMissRefJet", &MET_ExMissRefJet, &b_MET_ExMissRefJet); }
  if( fChain->GetBranchStatus("MET_EyMissRefJet") ) { fChain->SetBranchAddress("MET_EyMissRefJet", &MET_EyMissRefJet, &b_MET_EyMissRefJet); }
  if( fChain->GetBranchStatus("MET_EtSumRefJet") ) { fChain->SetBranchAddress("MET_EtSumRefJet", &MET_EtSumRefJet, &b_MET_EtSumRefJet); }
  if( fChain->GetBranchStatus("MET_ExMissCellOut") ) { fChain->SetBranchAddress("MET_ExMissCellOut", &MET_ExMissCellOut, &b_MET_ExMissCellOut); }
  if( fChain->GetBranchStatus("MET_EyMissCellOut") ) { fChain->SetBranchAddress("MET_EyMissCellOut", &MET_EyMissCellOut, &b_MET_EyMissCellOut); }
  if( fChain->GetBranchStatus("MET_EtSumCellOut") ) { fChain->SetBranchAddress("MET_EtSumCellOut", &MET_EtSumCellOut, &b_MET_EtSumCellOut); }
  if( fChain->GetBranchStatus("MET_ExFCALCorrTopo") ) { fChain->SetBranchAddress("MET_ExFCALCorrTopo", &MET_ExFCALCorrTopo, &b_MET_ExFCALCorrTopo); }
  if( fChain->GetBranchStatus("MET_EyFCALCorrTopo") ) { fChain->SetBranchAddress("MET_EyFCALCorrTopo", &MET_EyFCALCorrTopo, &b_MET_EyFCALCorrTopo); }
  if( fChain->GetBranchStatus("MET_EtSumFCALCorrTopo") ) { fChain->SetBranchAddress("MET_EtSumFCALCorrTopo", &MET_EtSumFCALCorrTopo, &b_MET_EtSumFCALCorrTopo); }
  if( fChain->GetBranchStatus("MET_ExFCALlocHadTopo") ) { fChain->SetBranchAddress("MET_ExFCALlocHadTopo", &MET_ExFCALlocHadTopo, &b_MET_ExFCALlocHadTopo); }
  if( fChain->GetBranchStatus("MET_EyFCALlocHadTopo") ) { fChain->SetBranchAddress("MET_EyFCALlocHadTopo", &MET_EyFCALlocHadTopo, &b_MET_EyFCALlocHadTopo); }
  if( fChain->GetBranchStatus("MET_EtSumFCALlocHadTopo") ) { fChain->SetBranchAddress("MET_EtSumFCALlocHadTopo", &MET_EtSumFCALlocHadTopo, &b_MET_EtSumFCALlocHadTopo); }
  if( fChain->GetBranchStatus("MET_ExPEMB") ) { fChain->SetBranchAddress("MET_ExPEMB", &MET_ExPEMB, &b_MET_ExPEMB); }
  if( fChain->GetBranchStatus("MET_EyPEMB") ) { fChain->SetBranchAddress("MET_EyPEMB", &MET_EyPEMB, &b_MET_EyPEMB); }
  if( fChain->GetBranchStatus("MET_EtSumPEMB") ) { fChain->SetBranchAddress("MET_EtSumPEMB", &MET_EtSumPEMB, &b_MET_EtSumPEMB); }
  if( fChain->GetBranchStatus("MET_nCellPEMB") ) { fChain->SetBranchAddress("MET_nCellPEMB", &MET_nCellPEMB, &b_MET_nCellPEMB); }
  if( fChain->GetBranchStatus("MET_ExEMB") ) { fChain->SetBranchAddress("MET_ExEMB", &MET_ExEMB, &b_MET_ExEMB); }
  if( fChain->GetBranchStatus("MET_EyEMB") ) { fChain->SetBranchAddress("MET_EyEMB", &MET_EyEMB, &b_MET_EyEMB); }
  if( fChain->GetBranchStatus("MET_EtSumEMB") ) { fChain->SetBranchAddress("MET_EtSumEMB", &MET_EtSumEMB, &b_MET_EtSumEMB); }
  if( fChain->GetBranchStatus("MET_nCellEMB") ) { fChain->SetBranchAddress("MET_nCellEMB", &MET_nCellEMB, &b_MET_nCellEMB); }
  if( fChain->GetBranchStatus("MET_ExPEMEC") ) { fChain->SetBranchAddress("MET_ExPEMEC", &MET_ExPEMEC, &b_MET_ExPEMEC); }
  if( fChain->GetBranchStatus("MET_EyPEMEC") ) { fChain->SetBranchAddress("MET_EyPEMEC", &MET_EyPEMEC, &b_MET_EyPEMEC); }
  if( fChain->GetBranchStatus("MET_EtSumPEMEC") ) { fChain->SetBranchAddress("MET_EtSumPEMEC", &MET_EtSumPEMEC, &b_MET_EtSumPEMEC); }
  if( fChain->GetBranchStatus("MET_nCellPEMEC") ) { fChain->SetBranchAddress("MET_nCellPEMEC", &MET_nCellPEMEC, &b_MET_nCellPEMEC); }
  if( fChain->GetBranchStatus("MET_ExEMEC") ) { fChain->SetBranchAddress("MET_ExEMEC", &MET_ExEMEC, &b_MET_ExEMEC); }
  if( fChain->GetBranchStatus("MET_EyEMEC") ) { fChain->SetBranchAddress("MET_EyEMEC", &MET_EyEMEC, &b_MET_EyEMEC); }
  if( fChain->GetBranchStatus("MET_EtSumEMEC") ) { fChain->SetBranchAddress("MET_EtSumEMEC", &MET_EtSumEMEC, &b_MET_EtSumEMEC); }
  if( fChain->GetBranchStatus("MET_nCellEMEC") ) { fChain->SetBranchAddress("MET_nCellEMEC", &MET_nCellEMEC, &b_MET_nCellEMEC); }
  if( fChain->GetBranchStatus("MET_ExTILE") ) { fChain->SetBranchAddress("MET_ExTILE", &MET_ExTILE, &b_MET_ExTILE); }
  if( fChain->GetBranchStatus("MET_EyTILE") ) { fChain->SetBranchAddress("MET_EyTILE", &MET_EyTILE, &b_MET_EyTILE); }
  if( fChain->GetBranchStatus("MET_EtSumTILE") ) { fChain->SetBranchAddress("MET_EtSumTILE", &MET_EtSumTILE, &b_MET_EtSumTILE); }
  if( fChain->GetBranchStatus("MET_nCellTILE") ) { fChain->SetBranchAddress("MET_nCellTILE", &MET_nCellTILE, &b_MET_nCellTILE); }
  if( fChain->GetBranchStatus("MET_ExHEC") ) { fChain->SetBranchAddress("MET_ExHEC", &MET_ExHEC, &b_MET_ExHEC); }
  if( fChain->GetBranchStatus("MET_EyHEC") ) { fChain->SetBranchAddress("MET_EyHEC", &MET_EyHEC, &b_MET_EyHEC); }
  if( fChain->GetBranchStatus("MET_EtSumHEC") ) { fChain->SetBranchAddress("MET_EtSumHEC", &MET_EtSumHEC, &b_MET_EtSumHEC); }
  if( fChain->GetBranchStatus("MET_nCellHEC") ) { fChain->SetBranchAddress("MET_nCellHEC", &MET_nCellHEC, &b_MET_nCellHEC); }
  if( fChain->GetBranchStatus("MET_ExFCAL") ) { fChain->SetBranchAddress("MET_ExFCAL", &MET_ExFCAL, &b_MET_ExFCAL); }
  if( fChain->GetBranchStatus("MET_EyFCAL") ) { fChain->SetBranchAddress("MET_EyFCAL", &MET_EyFCAL, &b_MET_EyFCAL); }
  if( fChain->GetBranchStatus("MET_EtSumFCAL") ) { fChain->SetBranchAddress("MET_EtSumFCAL", &MET_EtSumFCAL, &b_MET_EtSumFCAL); }
  if( fChain->GetBranchStatus("MET_nCellFCAL") ) { fChain->SetBranchAddress("MET_nCellFCAL", &MET_nCellFCAL, &b_MET_nCellFCAL); }
  if( fChain->GetBranchStatus("MET_ExBARReg") ) { fChain->SetBranchAddress("MET_ExBARReg", &MET_ExBARReg, &b_MET_ExBARReg); }
  if( fChain->GetBranchStatus("MET_EyBARReg") ) { fChain->SetBranchAddress("MET_EyBARReg", &MET_EyBARReg, &b_MET_EyBARReg); }
  if( fChain->GetBranchStatus("MET_EtSumBARReg") ) { fChain->SetBranchAddress("MET_EtSumBARReg", &MET_EtSumBARReg, &b_MET_EtSumBARReg); }
  if( fChain->GetBranchStatus("MET_ExECReg") ) { fChain->SetBranchAddress("MET_ExECReg", &MET_ExECReg, &b_MET_ExECReg); }
  if( fChain->GetBranchStatus("MET_EyECReg") ) { fChain->SetBranchAddress("MET_EyECReg", &MET_EyECReg, &b_MET_EyECReg); }
  if( fChain->GetBranchStatus("MET_EtSumECReg") ) { fChain->SetBranchAddress("MET_EtSumECReg", &MET_EtSumECReg, &b_MET_EtSumECReg); }
  if( fChain->GetBranchStatus("MET_ExFCALReg") ) { fChain->SetBranchAddress("MET_ExFCALReg", &MET_ExFCALReg, &b_MET_ExFCALReg); }
  if( fChain->GetBranchStatus("MET_EyFCALReg") ) { fChain->SetBranchAddress("MET_EyFCALReg", &MET_EyFCALReg, &b_MET_EyFCALReg); }
  if( fChain->GetBranchStatus("MET_EtSumFCALReg") ) { fChain->SetBranchAddress("MET_EtSumFCALReg", &MET_EtSumFCALReg, &b_MET_EtSumFCALReg); }
  if( fChain->GetBranchStatus("MET_ExTruthInt") ) { fChain->SetBranchAddress("MET_ExTruthInt", &MET_ExTruthInt, &b_MET_ExTruthInt); }
  if( fChain->GetBranchStatus("MET_EyTruthInt") ) { fChain->SetBranchAddress("MET_EyTruthInt", &MET_EyTruthInt, &b_MET_EyTruthInt); }
  if( fChain->GetBranchStatus("MET_EtSumTruthInt") ) { fChain->SetBranchAddress("MET_EtSumTruthInt", &MET_EtSumTruthInt, &b_MET_EtSumTruthInt); }
  if( fChain->GetBranchStatus("MET_ExTruthNonInt") ) { fChain->SetBranchAddress("MET_ExTruthNonInt", &MET_ExTruthNonInt, &b_MET_ExTruthNonInt); }
  if( fChain->GetBranchStatus("MET_EyTruthNonInt") ) { fChain->SetBranchAddress("MET_EyTruthNonInt", &MET_EyTruthNonInt, &b_MET_EyTruthNonInt); }
  if( fChain->GetBranchStatus("MET_EtSumTruthNonInt") ) { fChain->SetBranchAddress("MET_EtSumTruthNonInt", &MET_EtSumTruthNonInt, &b_MET_EtSumTruthNonInt); }
  if( fChain->GetBranchStatus("MET_ExTruthIntCentral") ) { fChain->SetBranchAddress("MET_ExTruthIntCentral", &MET_ExTruthIntCentral, &b_MET_ExTruthIntCentral); }
  if( fChain->GetBranchStatus("MET_EyTruthIntCentral") ) { fChain->SetBranchAddress("MET_EyTruthIntCentral", &MET_EyTruthIntCentral, &b_MET_EyTruthIntCentral); }
  if( fChain->GetBranchStatus("MET_EtSumTruthIntCentral") ) { fChain->SetBranchAddress("MET_EtSumTruthIntCentral", &MET_EtSumTruthIntCentral, &b_MET_EtSumTruthIntCentral); }
  if( fChain->GetBranchStatus("MET_ExTruthIntFwd") ) { fChain->SetBranchAddress("MET_ExTruthIntFwd", &MET_ExTruthIntFwd, &b_MET_ExTruthIntFwd); }
  if( fChain->GetBranchStatus("MET_EyTruthIntFwd") ) { fChain->SetBranchAddress("MET_EyTruthIntFwd", &MET_EyTruthIntFwd, &b_MET_EyTruthIntFwd); }
  if( fChain->GetBranchStatus("MET_EtSumTruthIntFwd") ) { fChain->SetBranchAddress("MET_EtSumTruthIntFwd", &MET_EtSumTruthIntFwd, &b_MET_EtSumTruthIntFwd); }
  if( fChain->GetBranchStatus("MET_ExTruthMuons") ) { fChain->SetBranchAddress("MET_ExTruthMuons", &MET_ExTruthMuons, &b_MET_ExTruthMuons); }
  if( fChain->GetBranchStatus("MET_EyTruthMuons") ) { fChain->SetBranchAddress("MET_EyTruthMuons", &MET_EyTruthMuons, &b_MET_EyTruthMuons); }
  if( fChain->GetBranchStatus("MET_EtSumTruthMuons") ) { fChain->SetBranchAddress("MET_EtSumTruthMuons", &MET_EtSumTruthMuons, &b_MET_EtSumTruthMuons); }
  if( fChain->GetBranchStatus("Obj_ExMissEle") ) { fChain->SetBranchAddress("Obj_ExMissEle", &Obj_ExMissEle, &b_Obj_ExMissEle); }
  if( fChain->GetBranchStatus("Obj_EyMissEle") ) { fChain->SetBranchAddress("Obj_EyMissEle", &Obj_EyMissEle, &b_Obj_EyMissEle); }
  if( fChain->GetBranchStatus("Obj_EtSumEle") ) { fChain->SetBranchAddress("Obj_EtSumEle", &Obj_EtSumEle, &b_Obj_EtSumEle); }
  if( fChain->GetBranchStatus("Obj_ExMissMu") ) { fChain->SetBranchAddress("Obj_ExMissMu", &Obj_ExMissMu, &b_Obj_ExMissMu); }
  if( fChain->GetBranchStatus("Obj_EyMissMu") ) { fChain->SetBranchAddress("Obj_EyMissMu", &Obj_EyMissMu, &b_Obj_EyMissMu); }
  if( fChain->GetBranchStatus("Obj_EtSumMu") ) { fChain->SetBranchAddress("Obj_EtSumMu", &Obj_EtSumMu, &b_Obj_EtSumMu); }
  if( fChain->GetBranchStatus("Obj_ExMissJet") ) { fChain->SetBranchAddress("Obj_ExMissJet", &Obj_ExMissJet, &b_Obj_ExMissJet); }
  if( fChain->GetBranchStatus("Obj_EyMissJet") ) { fChain->SetBranchAddress("Obj_EyMissJet", &Obj_EyMissJet, &b_Obj_EyMissJet); }
  if( fChain->GetBranchStatus("Obj_EtSumJet") ) { fChain->SetBranchAddress("Obj_EtSumJet", &Obj_EtSumJet, &b_Obj_EtSumJet); }
  if( fChain->GetBranchStatus("Obj_ExMissIdTrk") ) { fChain->SetBranchAddress("Obj_ExMissIdTrk", &Obj_ExMissIdTrk, &b_Obj_ExMissIdTrk); }
  if( fChain->GetBranchStatus("Obj_EyMissIdTrk") ) { fChain->SetBranchAddress("Obj_EyMissIdTrk", &Obj_EyMissIdTrk, &b_Obj_EyMissIdTrk); }
  if( fChain->GetBranchStatus("Obj_EtSumIdTrk") ) { fChain->SetBranchAddress("Obj_EtSumIdTrk", &Obj_EtSumIdTrk, &b_Obj_EtSumIdTrk); }
  if( fChain->GetBranchStatus("Obj_ExMissMJet") ) { fChain->SetBranchAddress("Obj_ExMissMJet", &Obj_ExMissMJet, &b_Obj_ExMissMJet); }
  if( fChain->GetBranchStatus("Obj_EyMissMJet") ) { fChain->SetBranchAddress("Obj_EyMissMJet", &Obj_EyMissMJet, &b_Obj_EyMissMJet); }
  if( fChain->GetBranchStatus("Obj_EtSumMJet") ) { fChain->SetBranchAddress("Obj_EtSumMJet", &Obj_EtSumMJet, &b_Obj_EtSumMJet); }
  if( fChain->GetBranchStatus("Obj_ExMissRest") ) { fChain->SetBranchAddress("Obj_ExMissRest", &Obj_ExMissRest, &b_Obj_ExMissRest); }
  if( fChain->GetBranchStatus("Obj_EyMissRest") ) { fChain->SetBranchAddress("Obj_EyMissRest", &Obj_EyMissRest, &b_Obj_EyMissRest); }
  if( fChain->GetBranchStatus("Obj_EtSumRest") ) { fChain->SetBranchAddress("Obj_EtSumRest", &Obj_EtSumRest, &b_Obj_EtSumRest); }
  if( fChain->GetBranchStatus("Obj_ExMissFinal") ) { fChain->SetBranchAddress("Obj_ExMissFinal", &Obj_ExMissFinal, &b_Obj_ExMissFinal); }
  if( fChain->GetBranchStatus("Obj_EyMissFinal") ) { fChain->SetBranchAddress("Obj_EyMissFinal", &Obj_EyMissFinal, &b_Obj_EyMissFinal); }
  if( fChain->GetBranchStatus("Obj_EtSumFinal") ) { fChain->SetBranchAddress("Obj_EtSumFinal", &Obj_EtSumFinal, &b_Obj_EtSumFinal); }
  if( fChain->GetBranchStatus("L1EM_Scale") ) { fChain->SetBranchAddress("L1EM_Scale", &L1EM_Scale, &b_L1EM_Scale); }
  if( fChain->GetBranchStatus("L1Em_nRoI") ) { fChain->SetBranchAddress("L1Em_nRoI", &L1Em_nRoI, &b_L1Em_nRoI); }
  if( fChain->GetBranchStatus("L1Em_RoIWord") ) { fChain->SetBranchAddress("L1Em_RoIWord", &L1Em_RoIWord, &b_L1Em_RoIWord); }
  if( fChain->GetBranchStatus("L1Em_Core") ) { fChain->SetBranchAddress("L1Em_Core", &L1Em_Core, &b_L1Em_Core); }
  if( fChain->GetBranchStatus("L1Em_EmClus") ) { fChain->SetBranchAddress("L1Em_EmClus", &L1Em_EmClus, &b_L1Em_EmClus); }
  if( fChain->GetBranchStatus("L1Em_TauClus") ) { fChain->SetBranchAddress("L1Em_TauClus", &L1Em_TauClus, &b_L1Em_TauClus); }
  if( fChain->GetBranchStatus("L1Em_EmIsol") ) { fChain->SetBranchAddress("L1Em_EmIsol", &L1Em_EmIsol, &b_L1Em_EmIsol); }
  if( fChain->GetBranchStatus("L1Em_HdIsol") ) { fChain->SetBranchAddress("L1Em_HdIsol", &L1Em_HdIsol, &b_L1Em_HdIsol); }
  if( fChain->GetBranchStatus("L1Em_HdCore") ) { fChain->SetBranchAddress("L1Em_HdCore", &L1Em_HdCore, &b_L1Em_HdCore); }
  if( fChain->GetBranchStatus("L1Em_EmTauThresh") ) { fChain->SetBranchAddress("L1Em_EmTauThresh", &L1Em_EmTauThresh, &b_L1Em_EmTauThresh); }
  if( fChain->GetBranchStatus("L1Em_EmTauThresh") ) { fChain->SetBranchAddress("L1Em_EmTauThresh", &L1Em_EmTauThresh, &b_L1Em_EmTauThresh); }
  if( fChain->GetBranchStatus("L1Em_eta") ) { fChain->SetBranchAddress("L1Em_eta", &L1Em_eta, &b_L1Em_eta); }
  if( fChain->GetBranchStatus("L1Em_phi") ) { fChain->SetBranchAddress("L1Em_phi", &L1Em_phi, &b_L1Em_phi); }
  if( fChain->GetBranchStatus("L1Jet_nRoI") ) { fChain->SetBranchAddress("L1Jet_nRoI", &L1Jet_nRoI, &b_L1Jet_nRoI); }
  if( fChain->GetBranchStatus("L1Jet_JetRoIWord") ) { fChain->SetBranchAddress("L1Jet_JetRoIWord", &L1Jet_JetRoIWord, &b_L1Jet_JetRoIWord); }
  if( fChain->GetBranchStatus("L1Jet_ET4x4") ) { fChain->SetBranchAddress("L1Jet_ET4x4", &L1Jet_ET4x4, &b_L1Jet_ET4x4); }
  if( fChain->GetBranchStatus("L1Jet_ET6x6") ) { fChain->SetBranchAddress("L1Jet_ET6x6", &L1Jet_ET6x6, &b_L1Jet_ET6x6); }
  if( fChain->GetBranchStatus("L1Jet_ET8x8") ) { fChain->SetBranchAddress("L1Jet_ET8x8", &L1Jet_ET8x8, &b_L1Jet_ET8x8); }
  if( fChain->GetBranchStatus("L1Jet_Thresh") ) { fChain->SetBranchAddress("L1Jet_Thresh", &L1Jet_Thresh, &b_L1Jet_Thresh); }
  if( fChain->GetBranchStatus("L1Jet_eta") ) { fChain->SetBranchAddress("L1Jet_eta", &L1Jet_eta, &b_L1Jet_eta); }
  if( fChain->GetBranchStatus("L1Jet_phi") ) { fChain->SetBranchAddress("L1Jet_phi", &L1Jet_phi, &b_L1Jet_phi); }
  if( fChain->GetBranchStatus("L1ET_EtMissHits") ) { fChain->SetBranchAddress("L1ET_EtMissHits", &L1ET_EtMissHits, &b_L1ET_EtMissHits); }
  if( fChain->GetBranchStatus("L1ET_EtSumHits") ) { fChain->SetBranchAddress("L1ET_EtSumHits", &L1ET_EtSumHits, &b_L1ET_EtSumHits); }
  if( fChain->GetBranchStatus("L1ET_Ex") ) { fChain->SetBranchAddress("L1ET_Ex", &L1ET_Ex, &b_L1ET_Ex); }
  if( fChain->GetBranchStatus("L1ET_Ey") ) { fChain->SetBranchAddress("L1ET_Ey", &L1ET_Ey, &b_L1ET_Ey); }
  if( fChain->GetBranchStatus("L1ET_EtMiss") ) { fChain->SetBranchAddress("L1ET_EtMiss", &L1ET_EtMiss, &b_L1ET_EtMiss); }
  if( fChain->GetBranchStatus("L1ET_EtSum") ) { fChain->SetBranchAddress("L1ET_EtSum", &L1ET_EtSum, &b_L1ET_EtSum); }
  if( fChain->GetBranchStatus("L1ET_JetEtHits") ) { fChain->SetBranchAddress("L1ET_JetEtHits", &L1ET_JetEtHits, &b_L1ET_JetEtHits); }
  if( fChain->GetBranchStatus("L1ET_JetEtSum") ) { fChain->SetBranchAddress("L1ET_JetEtSum", &L1ET_JetEtSum, &b_L1ET_JetEtSum); }
  if( fChain->GetBranchStatus("canMulti") ) { fChain->SetBranchAddress("canMulti", &canMulti, &b_canMulti); }
  if( fChain->GetBranchStatus("nDataWord") ) { fChain->SetBranchAddress("nDataWord", &nDataWord, &b_nDataWord); }
  if( fChain->GetBranchStatus("dataWord") ) { fChain->SetBranchAddress("dataWord", &dataWord, &b_dataWord); }
  if( fChain->GetBranchStatus("nCTP_ROI") ) { fChain->SetBranchAddress("nCTP_ROI", &nCTP_ROI, &b_nCTP_ROI); }
  if( fChain->GetBranchStatus("CTP_ROI") ) { fChain->SetBranchAddress("CTP_ROI", &CTP_ROI, &b_CTP_ROI); }
  if( fChain->GetBranchStatus("nMuCTPI_ROI") ) { fChain->SetBranchAddress("nMuCTPI_ROI", &nMuCTPI_ROI, &b_nMuCTPI_ROI); }
  if( fChain->GetBranchStatus("muCTPI_ROI") ) { fChain->SetBranchAddress("muCTPI_ROI", &muCTPI_ROI, &b_muCTPI_ROI); }
  if( fChain->GetBranchStatus("nEMTau_ROI") ) { fChain->SetBranchAddress("nEMTau_ROI", &nEMTau_ROI, &b_nEMTau_ROI); }
  if( fChain->GetBranchStatus("EMTau_ROI") ) { fChain->SetBranchAddress("EMTau_ROI", &EMTau_ROI, &b_EMTau_ROI); }
  if( fChain->GetBranchStatus("nJetEnergy_ROI") ) { fChain->SetBranchAddress("nJetEnergy_ROI", &nJetEnergy_ROI, &b_nJetEnergy_ROI); }
  if( fChain->GetBranchStatus("JetEnergy_ROI") ) { fChain->SetBranchAddress("JetEnergy_ROI", &JetEnergy_ROI, &b_JetEnergy_ROI); }
  if( fChain->GetBranchStatus("T2CaNclus") ) { fChain->SetBranchAddress("T2CaNclus", &T2CaNclus, &b_T2CaNclus); }
  if( fChain->GetBranchStatus("T2CaTauNclus") ) { fChain->SetBranchAddress("T2CaTauNclus", &T2CaTauNclus, &b_T2CaTauNclus); }
  if( fChain->GetBranchStatus("T2CaEmE") ) { fChain->SetBranchAddress("T2CaEmE", &T2CaEmE, &b_T2CaEmE); }
  if( fChain->GetBranchStatus("T2CaRawEmE") ) { fChain->SetBranchAddress("T2CaRawEmE", &T2CaRawEmE, &b_T2CaRawEmE); }
  if( fChain->GetBranchStatus("T2CaEmES0") ) { fChain->SetBranchAddress("T2CaEmES0", &T2CaEmES0, &b_T2CaEmES0); }
  if( fChain->GetBranchStatus("T2CaEmES1") ) { fChain->SetBranchAddress("T2CaEmES1", &T2CaEmES1, &b_T2CaEmES1); }
  if( fChain->GetBranchStatus("T2CaEmES2") ) { fChain->SetBranchAddress("T2CaEmES2", &T2CaEmES2, &b_T2CaEmES2); }
  if( fChain->GetBranchStatus("T2CaEmES3") ) { fChain->SetBranchAddress("T2CaEmES3", &T2CaEmES3, &b_T2CaEmES3); }
  if( fChain->GetBranchStatus("T2CaHadE") ) { fChain->SetBranchAddress("T2CaHadE", &T2CaHadE, &b_T2CaHadE); }
  if( fChain->GetBranchStatus("T2CaRawHadE") ) { fChain->SetBranchAddress("T2CaRawHadE", &T2CaRawHadE, &b_T2CaRawHadE); }
  if( fChain->GetBranchStatus("T2CaHadES0") ) { fChain->SetBranchAddress("T2CaHadES0", &T2CaHadES0, &b_T2CaHadES0); }
  if( fChain->GetBranchStatus("T2CaHadES1") ) { fChain->SetBranchAddress("T2CaHadES1", &T2CaHadES1, &b_T2CaHadES1); }
  if( fChain->GetBranchStatus("T2CaHadES2") ) { fChain->SetBranchAddress("T2CaHadES2", &T2CaHadES2, &b_T2CaHadES2); }
  if( fChain->GetBranchStatus("T2CaHadES3") ) { fChain->SetBranchAddress("T2CaHadES3", &T2CaHadES3, &b_T2CaHadES3); }
  if( fChain->GetBranchStatus("T2CaRcore") ) { fChain->SetBranchAddress("T2CaRcore", &T2CaRcore, &b_T2CaRcore); }
  if( fChain->GetBranchStatus("T2CaEratio") ) { fChain->SetBranchAddress("T2CaEratio", &T2CaEratio, &b_T2CaEratio); }
  if( fChain->GetBranchStatus("T2CaWidth") ) { fChain->SetBranchAddress("T2CaWidth", &T2CaWidth, &b_T2CaWidth); }
  if( fChain->GetBranchStatus("T2CaF73") ) { fChain->SetBranchAddress("T2CaF73", &T2CaF73, &b_T2CaF73); }
  if( fChain->GetBranchStatus("T2CaEta") ) { fChain->SetBranchAddress("T2CaEta", &T2CaEta, &b_T2CaEta); }
  if( fChain->GetBranchStatus("T2CaPhi") ) { fChain->SetBranchAddress("T2CaPhi", &T2CaPhi, &b_T2CaPhi); }
  if( fChain->GetBranchStatus("T2CaRawEta") ) { fChain->SetBranchAddress("T2CaRawEta", &T2CaRawEta, &b_T2CaRawEta); }
  if( fChain->GetBranchStatus("T2CaRawPhi") ) { fChain->SetBranchAddress("T2CaRawPhi", &T2CaRawPhi, &b_T2CaRawPhi); }
  if( fChain->GetBranchStatus("T2CaL1Sim_Eta") ) { fChain->SetBranchAddress("T2CaL1Sim_Eta", &T2CaL1Sim_Eta, &b_T2CaL1Sim_Eta); }
  if( fChain->GetBranchStatus("T2CaL1Sim_Phi") ) { fChain->SetBranchAddress("T2CaL1Sim_Phi", &T2CaL1Sim_Phi, &b_T2CaL1Sim_Phi); }
  if( fChain->GetBranchStatus("T2CaL1Sim_EmClus") ) { fChain->SetBranchAddress("T2CaL1Sim_EmClus", &T2CaL1Sim_EmClus, &b_T2CaL1Sim_EmClus); }
  if( fChain->GetBranchStatus("T2CaL1Sim_EmIsol") ) { fChain->SetBranchAddress("T2CaL1Sim_EmIsol", &T2CaL1Sim_EmIsol, &b_T2CaL1Sim_EmIsol); }
  if( fChain->GetBranchStatus("T2CaL1Sim_HdCore") ) { fChain->SetBranchAddress("T2CaL1Sim_HdCore", &T2CaL1Sim_HdCore, &b_T2CaL1Sim_HdCore); }
  if( fChain->GetBranchStatus("T2CaL1Sim_HdIsol") ) { fChain->SetBranchAddress("T2CaL1Sim_HdIsol", &T2CaL1Sim_HdIsol, &b_T2CaL1Sim_HdIsol); }
  if( fChain->GetBranchStatus("T2CaL1Sim_nRoIperRegion") ) { fChain->SetBranchAddress("T2CaL1Sim_nRoIperRegion", &T2CaL1Sim_nRoIperRegion, &b_T2CaL1Sim_nRoIperRegion); }
  if( fChain->GetBranchStatus("T2CaRoIword") ) { fChain->SetBranchAddress("T2CaRoIword", &T2CaRoIword, &b_T2CaRoIword); }
  if( fChain->GetBranchStatus("T2CaTauEta") ) { fChain->SetBranchAddress("T2CaTauEta", &T2CaTauEta, &b_T2CaTauEta); }
  if( fChain->GetBranchStatus("T2CaTauPhi") ) { fChain->SetBranchAddress("T2CaTauPhi", &T2CaTauPhi, &b_T2CaTauPhi); }
  if( fChain->GetBranchStatus("T2CaEMES0_nar") ) { fChain->SetBranchAddress("T2CaEMES0_nar", &T2CaEMES0_nar, &b_T2CaEMES0_nar); }
  if( fChain->GetBranchStatus("T2CaEMES0_wid") ) { fChain->SetBranchAddress("T2CaEMES0_wid", &T2CaEMES0_wid, &b_T2CaEMES0_wid); }
  if( fChain->GetBranchStatus("T2CaEMES1_nar") ) { fChain->SetBranchAddress("T2CaEMES1_nar", &T2CaEMES1_nar, &b_T2CaEMES1_nar); }
  if( fChain->GetBranchStatus("T2CaEMES1_wid") ) { fChain->SetBranchAddress("T2CaEMES1_wid", &T2CaEMES1_wid, &b_T2CaEMES1_wid); }
  if( fChain->GetBranchStatus("T2CaEMES2_nar") ) { fChain->SetBranchAddress("T2CaEMES2_nar", &T2CaEMES2_nar, &b_T2CaEMES2_nar); }
  if( fChain->GetBranchStatus("T2CaEMES2_wid") ) { fChain->SetBranchAddress("T2CaEMES2_wid", &T2CaEMES2_wid, &b_T2CaEMES2_wid); }
  if( fChain->GetBranchStatus("T2CaEMES3_nar") ) { fChain->SetBranchAddress("T2CaEMES3_nar", &T2CaEMES3_nar, &b_T2CaEMES3_nar); }
  if( fChain->GetBranchStatus("T2CaEMES3_wid") ) { fChain->SetBranchAddress("T2CaEMES3_wid", &T2CaEMES3_wid, &b_T2CaEMES3_wid); }
  if( fChain->GetBranchStatus("T2CaHADES1_nar") ) { fChain->SetBranchAddress("T2CaHADES1_nar", &T2CaHADES1_nar, &b_T2CaHADES1_nar); }
  if( fChain->GetBranchStatus("T2CaEHADS1_wid") ) { fChain->SetBranchAddress("T2CaEHADS1_wid", &T2CaEHADS1_wid, &b_T2CaEHADS1_wid); }
  if( fChain->GetBranchStatus("T2CaHADES2_nar") ) { fChain->SetBranchAddress("T2CaHADES2_nar", &T2CaHADES2_nar, &b_T2CaHADES2_nar); }
  if( fChain->GetBranchStatus("T2CaEHADS2_wid") ) { fChain->SetBranchAddress("T2CaEHADS2_wid", &T2CaEHADS2_wid, &b_T2CaEHADS2_wid); }
  if( fChain->GetBranchStatus("T2CaHADES3_nar") ) { fChain->SetBranchAddress("T2CaHADES3_nar", &T2CaHADES3_nar, &b_T2CaHADES3_nar); }
  if( fChain->GetBranchStatus("T2CaEHADS3_wid") ) { fChain->SetBranchAddress("T2CaEHADS3_wid", &T2CaEHADS3_wid, &b_T2CaEHADS3_wid); }
  if( fChain->GetBranchStatus("T2CaEMrad0") ) { fChain->SetBranchAddress("T2CaEMrad0", &T2CaEMrad0, &b_T2CaEMrad0); }
  if( fChain->GetBranchStatus("T2CaEMrad1") ) { fChain->SetBranchAddress("T2CaEMrad1", &T2CaEMrad1, &b_T2CaEMrad1); }
  if( fChain->GetBranchStatus("T2CaEMrad2") ) { fChain->SetBranchAddress("T2CaEMrad2", &T2CaEMrad2, &b_T2CaEMrad2); }
  if( fChain->GetBranchStatus("T2CaEMrad3") ) { fChain->SetBranchAddress("T2CaEMrad3", &T2CaEMrad3, &b_T2CaEMrad3); }
  if( fChain->GetBranchStatus("T2CaEMwid0") ) { fChain->SetBranchAddress("T2CaEMwid0", &T2CaEMwid0, &b_T2CaEMwid0); }
  if( fChain->GetBranchStatus("T2CaEMwid1") ) { fChain->SetBranchAddress("T2CaEMwid1", &T2CaEMwid1, &b_T2CaEMwid1); }
  if( fChain->GetBranchStatus("T2CaEMwid2") ) { fChain->SetBranchAddress("T2CaEMwid2", &T2CaEMwid2, &b_T2CaEMwid2); }
  if( fChain->GetBranchStatus("T2CaEMwid3") ) { fChain->SetBranchAddress("T2CaEMwid3", &T2CaEMwid3, &b_T2CaEMwid3); }
  if( fChain->GetBranchStatus("T2CaHADwid1") ) { fChain->SetBranchAddress("T2CaHADwid1", &T2CaHADwid1, &b_T2CaHADwid1); }
  if( fChain->GetBranchStatus("T2CaHADwid2") ) { fChain->SetBranchAddress("T2CaHADwid2", &T2CaHADwid2, &b_T2CaHADwid2); }
  if( fChain->GetBranchStatus("T2CaHADwid3") ) { fChain->SetBranchAddress("T2CaHADwid3", &T2CaHADwid3, &b_T2CaHADwid3); }
  if( fChain->GetBranchStatus("T2CaEMenorm0") ) { fChain->SetBranchAddress("T2CaEMenorm0", &T2CaEMenorm0, &b_T2CaEMenorm0); }
  if( fChain->GetBranchStatus("T2CaEMenorm1") ) { fChain->SetBranchAddress("T2CaEMenorm1", &T2CaEMenorm1, &b_T2CaEMenorm1); }
  if( fChain->GetBranchStatus("T2CaEMenorm2") ) { fChain->SetBranchAddress("T2CaEMenorm2", &T2CaEMenorm2, &b_T2CaEMenorm2); }
  if( fChain->GetBranchStatus("T2CaEMenorm3") ) { fChain->SetBranchAddress("T2CaEMenorm3", &T2CaEMenorm3, &b_T2CaEMenorm3); }
  if( fChain->GetBranchStatus("T2CaHADenorm1") ) { fChain->SetBranchAddress("T2CaHADenorm1", &T2CaHADenorm1, &b_T2CaHADenorm1); }
  if( fChain->GetBranchStatus("T2CaHADenorm2") ) { fChain->SetBranchAddress("T2CaHADenorm2", &T2CaHADenorm2, &b_T2CaHADenorm2); }
  if( fChain->GetBranchStatus("T2CaHADenorm3") ) { fChain->SetBranchAddress("T2CaHADenorm3", &T2CaHADenorm3, &b_T2CaHADenorm3); }
  if( fChain->GetBranchStatus("T2CaNumStripCells") ) { fChain->SetBranchAddress("T2CaNumStripCells", &T2CaNumStripCells, &b_T2CaNumStripCells); }
  if( fChain->GetBranchStatus("T2CaEnergyCalib") ) { fChain->SetBranchAddress("T2CaEnergyCalib", &T2CaEnergyCalib, &b_T2CaEnergyCalib); }
  if( fChain->GetBranchStatus("T2CaEMEnergyCalib") ) { fChain->SetBranchAddress("T2CaEMEnergyCalib", &T2CaEMEnergyCalib, &b_T2CaEMEnergyCalib); }
  if( fChain->GetBranchStatus("T2CaIsoFrac") ) { fChain->SetBranchAddress("T2CaIsoFrac", &T2CaIsoFrac, &b_T2CaIsoFrac); }
  if( fChain->GetBranchStatus("TrigNClus") ) { fChain->SetBranchAddress("TrigNClus", &TrigNClus, &b_TrigNClus); }
  if( fChain->GetBranchStatus("TrigClusEta") ) { fChain->SetBranchAddress("TrigClusEta", &TrigClusEta, &b_TrigClusEta); }
  if( fChain->GetBranchStatus("TrigClusPhi") ) { fChain->SetBranchAddress("TrigClusPhi", &TrigClusPhi, &b_TrigClusPhi); }
  if( fChain->GetBranchStatus("TrigClusEtaRef") ) { fChain->SetBranchAddress("TrigClusEtaRef", &TrigClusEtaRef, &b_TrigClusEtaRef); }
  if( fChain->GetBranchStatus("TrigClusPhiRef") ) { fChain->SetBranchAddress("TrigClusPhiRef", &TrigClusPhiRef, &b_TrigClusPhiRef); }
  if( fChain->GetBranchStatus("TrigClusEt") ) { fChain->SetBranchAddress("TrigClusEt", &TrigClusEt, &b_TrigClusEt); }
  if( fChain->GetBranchStatus("TrigClusE") ) { fChain->SetBranchAddress("TrigClusE", &TrigClusE, &b_TrigClusE); }
  if( fChain->GetBranchStatus("TrigClusEtaPresh") ) { fChain->SetBranchAddress("TrigClusEtaPresh", &TrigClusEtaPresh, &b_TrigClusEtaPresh); }
  if( fChain->GetBranchStatus("TrigClusPhiPresh") ) { fChain->SetBranchAddress("TrigClusPhiPresh", &TrigClusPhiPresh, &b_TrigClusPhiPresh); }
  if( fChain->GetBranchStatus("TrigClusEPresh") ) { fChain->SetBranchAddress("TrigClusEPresh", &TrigClusEPresh, &b_TrigClusEPresh); }
  if( fChain->GetBranchStatus("TrigClusEta1stS") ) { fChain->SetBranchAddress("TrigClusEta1stS", &TrigClusEta1stS, &b_TrigClusEta1stS); }
  if( fChain->GetBranchStatus("TrigClusPhi1stS") ) { fChain->SetBranchAddress("TrigClusPhi1stS", &TrigClusPhi1stS, &b_TrigClusPhi1stS); }
  if( fChain->GetBranchStatus("TrigClusE1stS") ) { fChain->SetBranchAddress("TrigClusE1stS", &TrigClusE1stS, &b_TrigClusE1stS); }
  if( fChain->GetBranchStatus("TrigClusEta2ndS") ) { fChain->SetBranchAddress("TrigClusEta2ndS", &TrigClusEta2ndS, &b_TrigClusEta2ndS); }
  if( fChain->GetBranchStatus("TrigClusPhi2ndS") ) { fChain->SetBranchAddress("TrigClusPhi2ndS", &TrigClusPhi2ndS, &b_TrigClusPhi2ndS); }
  if( fChain->GetBranchStatus("TrigClusE2ndS") ) { fChain->SetBranchAddress("TrigClusE2ndS", &TrigClusE2ndS, &b_TrigClusE2ndS); }
  if( fChain->GetBranchStatus("TrigClusEta3rdS") ) { fChain->SetBranchAddress("TrigClusEta3rdS", &TrigClusEta3rdS, &b_TrigClusEta3rdS); }
  if( fChain->GetBranchStatus("TrigClusPhi3rdS") ) { fChain->SetBranchAddress("TrigClusPhi3rdS", &TrigClusPhi3rdS, &b_TrigClusPhi3rdS); }
  if( fChain->GetBranchStatus("TrigClusE3rdS") ) { fChain->SetBranchAddress("TrigClusE3rdS", &TrigClusE3rdS, &b_TrigClusE3rdS); }
  if( fChain->GetBranchStatus("TrigClusMaker") ) { fChain->SetBranchAddress("TrigClusMaker", &TrigClusMaker, &b_TrigClusMaker); }
  if( fChain->GetBranchStatus("EFTauNCand") ) { fChain->SetBranchAddress("EFTauNCand", &EFTauNCand, &b_EFTauNCand); }
  if( fChain->GetBranchStatus("EFTauEnergy") ) { fChain->SetBranchAddress("EFTauEnergy", &EFTauEnergy, &b_EFTauEnergy); }
  if( fChain->GetBranchStatus("EFTauNRoI") ) { fChain->SetBranchAddress("EFTauNRoI", &EFTauNRoI, &b_EFTauNRoI); }
  if( fChain->GetBranchStatus("EFTauEMenergy") ) { fChain->SetBranchAddress("EFTauEMenergy", &EFTauEMenergy, &b_EFTauEMenergy); }
  if( fChain->GetBranchStatus("EFTauET") ) { fChain->SetBranchAddress("EFTauET", &EFTauET, &b_EFTauET); }
  if( fChain->GetBranchStatus("EFTauEta") ) { fChain->SetBranchAddress("EFTauEta", &EFTauEta, &b_EFTauEta); }
  if( fChain->GetBranchStatus("EFTauPhi") ) { fChain->SetBranchAddress("EFTauPhi", &EFTauPhi, &b_EFTauPhi); }
  if( fChain->GetBranchStatus("EFTauCaloEta") ) { fChain->SetBranchAddress("EFTauCaloEta", &EFTauCaloEta, &b_EFTauCaloEta); }
  if( fChain->GetBranchStatus("EFTauCaloPhi") ) { fChain->SetBranchAddress("EFTauCaloPhi", &EFTauCaloPhi, &b_EFTauCaloPhi); }
  if( fChain->GetBranchStatus("EFTauNStrip") ) { fChain->SetBranchAddress("EFTauNStrip", &EFTauNStrip, &b_EFTauNStrip); }
  if( fChain->GetBranchStatus("EFTauNEMCell") ) { fChain->SetBranchAddress("EFTauNEMCell", &EFTauNEMCell, &b_EFTauNEMCell); }
  if( fChain->GetBranchStatus("EFTauStripET") ) { fChain->SetBranchAddress("EFTauStripET", &EFTauStripET, &b_EFTauStripET); }
  if( fChain->GetBranchStatus("EFTauStripWidth2") ) { fChain->SetBranchAddress("EFTauStripWidth2", &EFTauStripWidth2, &b_EFTauStripWidth2); }
  if( fChain->GetBranchStatus("EFTauEMRadius") ) { fChain->SetBranchAddress("EFTauEMRadius", &EFTauEMRadius, &b_EFTauEMRadius); }
  if( fChain->GetBranchStatus("EFTauIsoFrac") ) { fChain->SetBranchAddress("EFTauIsoFrac", &EFTauIsoFrac, &b_EFTauIsoFrac); }
  if( fChain->GetBranchStatus("EFTauNtrack") ) { fChain->SetBranchAddress("EFTauNtrack", &EFTauNtrack, &b_EFTauNtrack); }
  if( fChain->GetBranchStatus("EFTauCharge") ) { fChain->SetBranchAddress("EFTauCharge", &EFTauCharge, &b_EFTauCharge); }
  if( fChain->GetBranchStatus("EFTauPtTrack1") ) { fChain->SetBranchAddress("EFTauPtTrack1", &EFTauPtTrack1, &b_EFTauPtTrack1); }
  if( fChain->GetBranchStatus("EFTauEtaTrack1") ) { fChain->SetBranchAddress("EFTauEtaTrack1", &EFTauEtaTrack1, &b_EFTauEtaTrack1); }
  if( fChain->GetBranchStatus("EFTauPhiTrack1") ) { fChain->SetBranchAddress("EFTauPhiTrack1", &EFTauPhiTrack1, &b_EFTauPhiTrack1); }
  if( fChain->GetBranchStatus("EFTauDistTrack1") ) { fChain->SetBranchAddress("EFTauDistTrack1", &EFTauDistTrack1, &b_EFTauDistTrack1); }
  if( fChain->GetBranchStatus("EFTauPtTrack2") ) { fChain->SetBranchAddress("EFTauPtTrack2", &EFTauPtTrack2, &b_EFTauPtTrack2); }
  if( fChain->GetBranchStatus("EFTauEtaTrack2") ) { fChain->SetBranchAddress("EFTauEtaTrack2", &EFTauEtaTrack2, &b_EFTauEtaTrack2); }
  if( fChain->GetBranchStatus("EFTauPhiTrack2") ) { fChain->SetBranchAddress("EFTauPhiTrack2", &EFTauPhiTrack2, &b_EFTauPhiTrack2); }
  if( fChain->GetBranchStatus("EFTauDistTrack2") ) { fChain->SetBranchAddress("EFTauDistTrack2", &EFTauDistTrack2, &b_EFTauDistTrack2); }
  if( fChain->GetBranchStatus("EFTauPtTrack3") ) { fChain->SetBranchAddress("EFTauPtTrack3", &EFTauPtTrack3, &b_EFTauPtTrack3); }
  if( fChain->GetBranchStatus("EFTauEtaTrack3") ) { fChain->SetBranchAddress("EFTauEtaTrack3", &EFTauEtaTrack3, &b_EFTauEtaTrack3); }
  if( fChain->GetBranchStatus("EFTauPhiTrack3") ) { fChain->SetBranchAddress("EFTauPhiTrack3", &EFTauPhiTrack3, &b_EFTauPhiTrack3); }
  if( fChain->GetBranchStatus("EFTauDistTrack3") ) { fChain->SetBranchAddress("EFTauDistTrack3", &EFTauDistTrack3, &b_EFTauDistTrack3); }
  if( fChain->GetBranchStatus("EFTauSumEMe") ) { fChain->SetBranchAddress("EFTauSumEMe", &EFTauSumEMe, &b_EFTauSumEMe); }
  if( fChain->GetBranchStatus("EFTauSumEMpx") ) { fChain->SetBranchAddress("EFTauSumEMpx", &EFTauSumEMpx, &b_EFTauSumEMpx); }
  if( fChain->GetBranchStatus("EFTauSumEMpy") ) { fChain->SetBranchAddress("EFTauSumEMpy", &EFTauSumEMpy, &b_EFTauSumEMpy); }
  if( fChain->GetBranchStatus("EFTauSumEMpz") ) { fChain->SetBranchAddress("EFTauSumEMpz", &EFTauSumEMpz, &b_EFTauSumEMpz); }
  if( fChain->GetBranchStatus("EFTauEtEMCalib") ) { fChain->SetBranchAddress("EFTauEtEMCalib", &EFTauEtEMCalib, &b_EFTauEtEMCalib); }
  if( fChain->GetBranchStatus("EFTauEtHadCalib") ) { fChain->SetBranchAddress("EFTauEtHadCalib", &EFTauEtHadCalib, &b_EFTauEtHadCalib); }
  if( fChain->GetBranchStatus("T2CaNjets") ) { fChain->SetBranchAddress("T2CaNjets", &T2CaNjets, &b_T2CaNjets); }
  if( fChain->GetBranchStatus("T2CaJetE") ) { fChain->SetBranchAddress("T2CaJetE", &T2CaJetE, &b_T2CaJetE); }
  if( fChain->GetBranchStatus("T2CaJeteta") ) { fChain->SetBranchAddress("T2CaJeteta", &T2CaJeteta, &b_T2CaJeteta); }
  if( fChain->GetBranchStatus("T2CaJetphi") ) { fChain->SetBranchAddress("T2CaJetphi", &T2CaJetphi, &b_T2CaJetphi); }
  if( fChain->GetBranchStatus("T2CaJetConeR") ) { fChain->SetBranchAddress("T2CaJetConeR", &T2CaJetConeR, &b_T2CaJetConeR); }
  if( fChain->GetBranchStatus("T2CaJetroi") ) { fChain->SetBranchAddress("T2CaJetroi", &T2CaJetroi, &b_T2CaJetroi); }
  if( fChain->GetBranchStatus("TrigNJet") ) { fChain->SetBranchAddress("TrigNJet", &TrigNJet, &b_TrigNJet); }
  if( fChain->GetBranchStatus("TrigJetEt") ) { fChain->SetBranchAddress("TrigJetEt", &TrigJetEt, &b_TrigJetEt); }
  if( fChain->GetBranchStatus("TrigJetEta") ) { fChain->SetBranchAddress("TrigJetEta", &TrigJetEta, &b_TrigJetEta); }
  if( fChain->GetBranchStatus("TrigJetPhi") ) { fChain->SetBranchAddress("TrigJetPhi", &TrigJetPhi, &b_TrigJetPhi); }
  if( fChain->GetBranchStatus("TrigJetE") ) { fChain->SetBranchAddress("TrigJetE", &TrigJetE, &b_TrigJetE); }
  if( fChain->GetBranchStatus("TrigJetM") ) { fChain->SetBranchAddress("TrigJetM", &TrigJetM, &b_TrigJetM); }
  if( fChain->GetBranchStatus("TrigJetMaker") ) { fChain->SetBranchAddress("TrigJetMaker", &TrigJetMaker, &b_TrigJetMaker); }
  if( fChain->GetBranchStatus("T2IdNtracks") ) { fChain->SetBranchAddress("T2IdNtracks", &T2IdNtracks, &b_T2IdNtracks); }
  if( fChain->GetBranchStatus("T2IdRoiID") ) { fChain->SetBranchAddress("T2IdRoiID", &T2IdRoiID, &b_T2IdRoiID); }
  if( fChain->GetBranchStatus("T2IdAlgo") ) { fChain->SetBranchAddress("T2IdAlgo", &T2IdAlgo, &b_T2IdAlgo); }
  if( fChain->GetBranchStatus("T2IdPt") ) { fChain->SetBranchAddress("T2IdPt", &T2IdPt, &b_T2IdPt); }
  if( fChain->GetBranchStatus("T2IdPhi0") ) { fChain->SetBranchAddress("T2IdPhi0", &T2IdPhi0, &b_T2IdPhi0); }
  if( fChain->GetBranchStatus("T2IdZ0") ) { fChain->SetBranchAddress("T2IdZ0", &T2IdZ0, &b_T2IdZ0); }
  if( fChain->GetBranchStatus("T2IdD0") ) { fChain->SetBranchAddress("T2IdD0", &T2IdD0, &b_T2IdD0); }
  if( fChain->GetBranchStatus("T2IdPhic") ) { fChain->SetBranchAddress("T2IdPhic", &T2IdPhic, &b_T2IdPhic); }
  if( fChain->GetBranchStatus("T2IdEta") ) { fChain->SetBranchAddress("T2IdEta", &T2IdEta, &b_T2IdEta); }
  if( fChain->GetBranchStatus("T2IdErrPt") ) { fChain->SetBranchAddress("T2IdErrPt", &T2IdErrPt, &b_T2IdErrPt); }
  if( fChain->GetBranchStatus("T2IdErrPhi0") ) { fChain->SetBranchAddress("T2IdErrPhi0", &T2IdErrPhi0, &b_T2IdErrPhi0); }
  if( fChain->GetBranchStatus("T2IdErrEta") ) { fChain->SetBranchAddress("T2IdErrEta", &T2IdErrEta, &b_T2IdErrEta); }
  if( fChain->GetBranchStatus("T2IdErrD0") ) { fChain->SetBranchAddress("T2IdErrD0", &T2IdErrD0, &b_T2IdErrD0); }
  if( fChain->GetBranchStatus("T2IdErrZ0") ) { fChain->SetBranchAddress("T2IdErrZ0", &T2IdErrZ0, &b_T2IdErrZ0); }
  if( fChain->GetBranchStatus("T2IdChi2") ) { fChain->SetBranchAddress("T2IdChi2", &T2IdChi2, &b_T2IdChi2); }
  if( fChain->GetBranchStatus("T2IdNDoF") ) { fChain->SetBranchAddress("T2IdNDoF", &T2IdNDoF, &b_T2IdNDoF); }
  if( fChain->GetBranchStatus("T2IdNSihits") ) { fChain->SetBranchAddress("T2IdNSihits", &T2IdNSihits, &b_T2IdNSihits); }
  if( fChain->GetBranchStatus("T2IdNTrthits") ) { fChain->SetBranchAddress("T2IdNTrthits", &T2IdNTrthits, &b_T2IdNTrthits); }
  if( fChain->GetBranchStatus("T2IdHPatt") ) { fChain->SetBranchAddress("T2IdHPatt", &T2IdHPatt, &b_T2IdHPatt); }
  if( fChain->GetBranchStatus("T2IdNstraw") ) { fChain->SetBranchAddress("T2IdNstraw", &T2IdNstraw, &b_T2IdNstraw); }
  if( fChain->GetBranchStatus("T2IdNtime") ) { fChain->SetBranchAddress("T2IdNtime", &T2IdNtime, &b_T2IdNtime); }
  if( fChain->GetBranchStatus("T2IdNtr") ) { fChain->SetBranchAddress("T2IdNtr", &T2IdNtr, &b_T2IdNtr); }
  if( fChain->GetBranchStatus("T2IdLastPlane") ) { fChain->SetBranchAddress("T2IdLastPlane", &T2IdLastPlane, &b_T2IdLastPlane); }
  if( fChain->GetBranchStatus("T2IdFirstPlane") ) { fChain->SetBranchAddress("T2IdFirstPlane", &T2IdFirstPlane, &b_T2IdFirstPlane); }
  if( fChain->GetBranchStatus("T2IdNkineHit") ) { fChain->SetBranchAddress("T2IdNkineHit", &T2IdNkineHit, &b_T2IdNkineHit); }
  if( fChain->GetBranchStatus("T2IdKineRef") ) { fChain->SetBranchAddress("T2IdKineRef", &T2IdKineRef, &b_T2IdKineRef); }
  if( fChain->GetBranchStatus("T2IdKineEnt") ) { fChain->SetBranchAddress("T2IdKineEnt", &T2IdKineEnt, &b_T2IdKineEnt); }
  if( fChain->GetBranchStatus("T2IdNkineHitTRT") ) { fChain->SetBranchAddress("T2IdNkineHitTRT", &T2IdNkineHitTRT, &b_T2IdNkineHitTRT); }
  if( fChain->GetBranchStatus("T2IdKineRefTRT") ) { fChain->SetBranchAddress("T2IdKineRefTRT", &T2IdKineRefTRT, &b_T2IdKineRefTRT); }
  if( fChain->GetBranchStatus("T2IdKineEntTRT") ) { fChain->SetBranchAddress("T2IdKineEntTRT", &T2IdKineEntTRT, &b_T2IdKineEntTRT); }
  if( fChain->GetBranchStatus("T2NVtx") ) { fChain->SetBranchAddress("T2NVtx", &T2NVtx, &b_T2NVtx); }
  if( fChain->GetBranchStatus("T2zVertex") ) { fChain->SetBranchAddress("T2zVertex", &T2zVertex, &b_T2zVertex); }
  if( fChain->GetBranchStatus("TrkEF_totalNumTracks") ) { fChain->SetBranchAddress("TrkEF_totalNumTracks", &TrkEF_totalNumTracks, &b_TrkEF_totalNumTracks); }
  if( fChain->GetBranchStatus("TrkEF_RoIId") ) { fChain->SetBranchAddress("TrkEF_RoIId", &TrkEF_RoIId, &b_TrkEF_RoIId); }
  if( fChain->GetBranchStatus("TrkEF_d0") ) { fChain->SetBranchAddress("TrkEF_d0", &TrkEF_d0, &b_TrkEF_d0); }
  if( fChain->GetBranchStatus("TrkEF_z0") ) { fChain->SetBranchAddress("TrkEF_z0", &TrkEF_z0, &b_TrkEF_z0); }
  if( fChain->GetBranchStatus("TrkEF_phi") ) { fChain->SetBranchAddress("TrkEF_phi", &TrkEF_phi, &b_TrkEF_phi); }
  if( fChain->GetBranchStatus("TrkEF_eta") ) { fChain->SetBranchAddress("TrkEF_eta", &TrkEF_eta, &b_TrkEF_eta); }
  if( fChain->GetBranchStatus("TrkEF_qOverP") ) { fChain->SetBranchAddress("TrkEF_qOverP", &TrkEF_qOverP, &b_TrkEF_qOverP); }
  if( fChain->GetBranchStatus("TrkEF_Chi2") ) { fChain->SetBranchAddress("TrkEF_Chi2", &TrkEF_Chi2, &b_TrkEF_Chi2); }
  if( fChain->GetBranchStatus("TrkEF_Ndf") ) { fChain->SetBranchAddress("TrkEF_Ndf", &TrkEF_Ndf, &b_TrkEF_Ndf); }
  if( fChain->GetBranchStatus("TrkEF_sigd0") ) { fChain->SetBranchAddress("TrkEF_sigd0", &TrkEF_sigd0, &b_TrkEF_sigd0); }
  if( fChain->GetBranchStatus("TrkEF_sigz0") ) { fChain->SetBranchAddress("TrkEF_sigz0", &TrkEF_sigz0, &b_TrkEF_sigz0); }
  if( fChain->GetBranchStatus("TrkEF_sigpt") ) { fChain->SetBranchAddress("TrkEF_sigpt", &TrkEF_sigpt, &b_TrkEF_sigpt); }
  if( fChain->GetBranchStatus("TrkEF_sigphi") ) { fChain->SetBranchAddress("TrkEF_sigphi", &TrkEF_sigphi, &b_TrkEF_sigphi); }
  if( fChain->GetBranchStatus("TrkEF_sigeta") ) { fChain->SetBranchAddress("TrkEF_sigeta", &TrkEF_sigeta, &b_TrkEF_sigeta); }
  if( fChain->GetBranchStatus("TrkEF_sigqOverP") ) { fChain->SetBranchAddress("TrkEF_sigqOverP", &TrkEF_sigqOverP, &b_TrkEF_sigqOverP); }
  if( fChain->GetBranchStatus("TrkEF_covVert21") ) { fChain->SetBranchAddress("TrkEF_covVert21", &TrkEF_covVert21, &b_TrkEF_covVert21); }
  if( fChain->GetBranchStatus("TrkEF_covVert31") ) { fChain->SetBranchAddress("TrkEF_covVert31", &TrkEF_covVert31, &b_TrkEF_covVert31); }
  if( fChain->GetBranchStatus("TrkEF_covVert32") ) { fChain->SetBranchAddress("TrkEF_covVert32", &TrkEF_covVert32, &b_TrkEF_covVert32); }
  if( fChain->GetBranchStatus("TrkEF_covVert41") ) { fChain->SetBranchAddress("TrkEF_covVert41", &TrkEF_covVert41, &b_TrkEF_covVert41); }
  if( fChain->GetBranchStatus("TrkEF_covVert42") ) { fChain->SetBranchAddress("TrkEF_covVert42", &TrkEF_covVert42, &b_TrkEF_covVert42); }
  if( fChain->GetBranchStatus("TrkEF_covVert43") ) { fChain->SetBranchAddress("TrkEF_covVert43", &TrkEF_covVert43, &b_TrkEF_covVert43); }
  if( fChain->GetBranchStatus("TrkEF_covVert51") ) { fChain->SetBranchAddress("TrkEF_covVert51", &TrkEF_covVert51, &b_TrkEF_covVert51); }
  if( fChain->GetBranchStatus("TrkEF_covVert52") ) { fChain->SetBranchAddress("TrkEF_covVert52", &TrkEF_covVert52, &b_TrkEF_covVert52); }
  if( fChain->GetBranchStatus("TrkEF_covVert53") ) { fChain->SetBranchAddress("TrkEF_covVert53", &TrkEF_covVert53, &b_TrkEF_covVert53); }
  if( fChain->GetBranchStatus("TrkEF_covVert54") ) { fChain->SetBranchAddress("TrkEF_covVert54", &TrkEF_covVert54, &b_TrkEF_covVert54); }
  if( fChain->GetBranchStatus("TrkEF_px") ) { fChain->SetBranchAddress("TrkEF_px", &TrkEF_px, &b_TrkEF_px); }
  if( fChain->GetBranchStatus("TrkEF_py") ) { fChain->SetBranchAddress("TrkEF_py", &TrkEF_py, &b_TrkEF_py); }
  if( fChain->GetBranchStatus("TrkEF_pz") ) { fChain->SetBranchAddress("TrkEF_pz", &TrkEF_pz, &b_TrkEF_pz); }
  if( fChain->GetBranchStatus("TrkEF_pt") ) { fChain->SetBranchAddress("TrkEF_pt", &TrkEF_pt, &b_TrkEF_pt); }
  if( fChain->GetBranchStatus("TrkEF_numberOfBLayerHits") ) { fChain->SetBranchAddress("TrkEF_numberOfBLayerHits", &TrkEF_numberOfBLayerHits, &b_TrkEF_numberOfBLayerHits); }
  if( fChain->GetBranchStatus("TrkEF_numberOfPixelHits") ) { fChain->SetBranchAddress("TrkEF_numberOfPixelHits", &TrkEF_numberOfPixelHits, &b_TrkEF_numberOfPixelHits); }
  if( fChain->GetBranchStatus("TrkEF_numberOfSCTHits") ) { fChain->SetBranchAddress("TrkEF_numberOfSCTHits", &TrkEF_numberOfSCTHits, &b_TrkEF_numberOfSCTHits); }
  if( fChain->GetBranchStatus("TrkEF_numberOfTRTHits") ) { fChain->SetBranchAddress("TrkEF_numberOfTRTHits", &TrkEF_numberOfTRTHits, &b_TrkEF_numberOfTRTHits); }
  if( fChain->GetBranchStatus("TrkEF_numberOfTRTHighThresholdHits") ) { fChain->SetBranchAddress("TrkEF_numberOfTRTHighThresholdHits", &TrkEF_numberOfTRTHighThresholdHits, &b_TrkEF_numberOfTRTHighThresholdHits); }
  if( fChain->GetBranchStatus("TrkEF_numberOfBLayerSharedHits") ) { fChain->SetBranchAddress("TrkEF_numberOfBLayerSharedHits", &TrkEF_numberOfBLayerSharedHits, &b_TrkEF_numberOfBLayerSharedHits); }
  if( fChain->GetBranchStatus("TrkEF_numberOfPixelSharedHits") ) { fChain->SetBranchAddress("TrkEF_numberOfPixelSharedHits", &TrkEF_numberOfPixelSharedHits, &b_TrkEF_numberOfPixelSharedHits); }
  if( fChain->GetBranchStatus("TrkEF_numberOfPixelHoles") ) { fChain->SetBranchAddress("TrkEF_numberOfPixelHoles", &TrkEF_numberOfPixelHoles, &b_TrkEF_numberOfPixelHoles); }
  if( fChain->GetBranchStatus("TrkEF_numberOfSCTSharedHits") ) { fChain->SetBranchAddress("TrkEF_numberOfSCTSharedHits", &TrkEF_numberOfSCTSharedHits, &b_TrkEF_numberOfSCTSharedHits); }
  if( fChain->GetBranchStatus("TrkEF_numberOfSCTHoles") ) { fChain->SetBranchAddress("TrkEF_numberOfSCTHoles", &TrkEF_numberOfSCTHoles, &b_TrkEF_numberOfSCTHoles); }
  if( fChain->GetBranchStatus("TrkEF_numberOfTRTOutliers") ) { fChain->SetBranchAddress("TrkEF_numberOfTRTOutliers", &TrkEF_numberOfTRTOutliers, &b_TrkEF_numberOfTRTOutliers); }
  if( fChain->GetBranchStatus("TrkEF_numberOfTRTHighThresholdOutliers") ) { fChain->SetBranchAddress("TrkEF_numberOfTRTHighThresholdOutliers", &TrkEF_numberOfTRTHighThresholdOutliers, &b_TrkEF_numberOfTRTHighThresholdOutliers); }
  if( fChain->GetBranchStatus("TrkEF_numberOfMdtHits") ) { fChain->SetBranchAddress("TrkEF_numberOfMdtHits", &TrkEF_numberOfMdtHits, &b_TrkEF_numberOfMdtHits); }
  if( fChain->GetBranchStatus("TrkEF_numberOfTgcPhiHits") ) { fChain->SetBranchAddress("TrkEF_numberOfTgcPhiHits", &TrkEF_numberOfTgcPhiHits, &b_TrkEF_numberOfTgcPhiHits); }
  if( fChain->GetBranchStatus("TrkEF_numberOfRpcPhiHits") ) { fChain->SetBranchAddress("TrkEF_numberOfRpcPhiHits", &TrkEF_numberOfRpcPhiHits, &b_TrkEF_numberOfRpcPhiHits); }
  if( fChain->GetBranchStatus("TrkEF_numberOfCscPhiHits") ) { fChain->SetBranchAddress("TrkEF_numberOfCscPhiHits", &TrkEF_numberOfCscPhiHits, &b_TrkEF_numberOfCscPhiHits); }
  if( fChain->GetBranchStatus("TrkEF_numberOfTgcEtaHits") ) { fChain->SetBranchAddress("TrkEF_numberOfTgcEtaHits", &TrkEF_numberOfTgcEtaHits, &b_TrkEF_numberOfTgcEtaHits); }
  if( fChain->GetBranchStatus("TrkEF_numberOfRpcEtaHits") ) { fChain->SetBranchAddress("TrkEF_numberOfRpcEtaHits", &TrkEF_numberOfRpcEtaHits, &b_TrkEF_numberOfRpcEtaHits); }
  if( fChain->GetBranchStatus("TrkEF_numberOfCscEtaHits") ) { fChain->SetBranchAddress("TrkEF_numberOfCscEtaHits", &TrkEF_numberOfCscEtaHits, &b_TrkEF_numberOfCscEtaHits); }
  if( fChain->GetBranchStatus("TrkEF_numberOfGangedPixels") ) { fChain->SetBranchAddress("TrkEF_numberOfGangedPixels", &TrkEF_numberOfGangedPixels, &b_TrkEF_numberOfGangedPixels); }
  if( fChain->GetBranchStatus("TrkEF_numberOfOutliersOnTrack") ) { fChain->SetBranchAddress("TrkEF_numberOfOutliersOnTrack", &TrkEF_numberOfOutliersOnTrack, &b_TrkEF_numberOfOutliersOnTrack); }
  if( fChain->GetBranchStatus("TrkEF_numberOfTrackSummaryTypes") ) { fChain->SetBranchAddress("TrkEF_numberOfTrackSummaryTypes", &TrkEF_numberOfTrackSummaryTypes, &b_TrkEF_numberOfTrackSummaryTypes); }
  if( fChain->GetBranchStatus("TrkEF_truthBarcode") ) { fChain->SetBranchAddress("TrkEF_truthBarcode", &TrkEF_truthBarcode, &b_TrkEF_truthBarcode); }
  if( fChain->GetBranchStatus("TrkEF_truthNt") ) { fChain->SetBranchAddress("TrkEF_truthNt", &TrkEF_truthNt, &b_TrkEF_truthNt); }
  if( fChain->GetBranchStatus("TrkEF_ParticlePdg") ) { fChain->SetBranchAddress("TrkEF_ParticlePdg", &TrkEF_ParticlePdg, &b_TrkEF_ParticlePdg); }
  if( fChain->GetBranchStatus("TrkEF_ParentPdg") ) { fChain->SetBranchAddress("TrkEF_ParentPdg", &TrkEF_ParentPdg, &b_TrkEF_ParentPdg); }
  if( fChain->GetBranchStatus("TrkEF_NumKineHits") ) { fChain->SetBranchAddress("TrkEF_NumKineHits", &TrkEF_NumKineHits, &b_TrkEF_NumKineHits); }
  if( fChain->GetBranchStatus("VxEF_vxp_numVertices") ) { fChain->SetBranchAddress("VxEF_vxp_numVertices", &VxEF_vxp_numVertices, &b_VxEF_vxp_numVertices); }
  if( fChain->GetBranchStatus("VxEF_vxp_vtx_x") ) { fChain->SetBranchAddress("VxEF_vxp_vtx_x", &VxEF_vxp_vtx_x, &b_VxEF_vxp_vtx_x); }
  if( fChain->GetBranchStatus("VxEF_vxp_vtx_y") ) { fChain->SetBranchAddress("VxEF_vxp_vtx_y", &VxEF_vxp_vtx_y, &b_VxEF_vxp_vtx_y); }
  if( fChain->GetBranchStatus("VxEF_vxp_vtx_z") ) { fChain->SetBranchAddress("VxEF_vxp_vtx_z", &VxEF_vxp_vtx_z, &b_VxEF_vxp_vtx_z); }
  if( fChain->GetBranchStatus("VxEF_vxp_numOfTruthVertices") ) { fChain->SetBranchAddress("VxEF_vxp_numOfTruthVertices", &VxEF_vxp_numOfTruthVertices, &b_VxEF_vxp_numOfTruthVertices); }
  if( fChain->GetBranchStatus("VxEF_vxp_vtx_x_truth") ) { fChain->SetBranchAddress("VxEF_vxp_vtx_x_truth", &VxEF_vxp_vtx_x_truth, &b_VxEF_vxp_vtx_x_truth); }
  if( fChain->GetBranchStatus("VxEF_vxp_vtx_y_truth") ) { fChain->SetBranchAddress("VxEF_vxp_vtx_y_truth", &VxEF_vxp_vtx_y_truth, &b_VxEF_vxp_vtx_y_truth); }
  if( fChain->GetBranchStatus("VxEF_vxp_vtx_z_truth") ) { fChain->SetBranchAddress("VxEF_vxp_vtx_z_truth", &VxEF_vxp_vtx_z_truth, &b_VxEF_vxp_vtx_z_truth); }
  if( fChain->GetBranchStatus("VxEF_vxp_vtx_x_res") ) { fChain->SetBranchAddress("VxEF_vxp_vtx_x_res", &VxEF_vxp_vtx_x_res, &b_VxEF_vxp_vtx_x_res); }
  if( fChain->GetBranchStatus("VxEF_vxp_vtx_y_res") ) { fChain->SetBranchAddress("VxEF_vxp_vtx_y_res", &VxEF_vxp_vtx_y_res, &b_VxEF_vxp_vtx_y_res); }
  if( fChain->GetBranchStatus("VxEF_vxp_vtx_z_res") ) { fChain->SetBranchAddress("VxEF_vxp_vtx_z_res", &VxEF_vxp_vtx_z_res, &b_VxEF_vxp_vtx_z_res); }
  if( fChain->GetBranchStatus("VxEF_vxp_sigvtx_x") ) { fChain->SetBranchAddress("VxEF_vxp_sigvtx_x", &VxEF_vxp_sigvtx_x, &b_VxEF_vxp_sigvtx_x); }
  if( fChain->GetBranchStatus("VxEF_vxp_sigvtx_y") ) { fChain->SetBranchAddress("VxEF_vxp_sigvtx_y", &VxEF_vxp_sigvtx_y, &b_VxEF_vxp_sigvtx_y); }
  if( fChain->GetBranchStatus("VxEF_vxp_sigvtx_z") ) { fChain->SetBranchAddress("VxEF_vxp_sigvtx_z", &VxEF_vxp_sigvtx_z, &b_VxEF_vxp_sigvtx_z); }
  if( fChain->GetBranchStatus("VxEF_vxp_vtxchi2") ) { fChain->SetBranchAddress("VxEF_vxp_vtxchi2", &VxEF_vxp_vtxchi2, &b_VxEF_vxp_vtxchi2); }
  if( fChain->GetBranchStatus("VxEF_vxp_vtxndf") ) { fChain->SetBranchAddress("VxEF_vxp_vtxndf", &VxEF_vxp_vtxndf, &b_VxEF_vxp_vtxndf); }
  if( fChain->GetBranchStatus("VxEF_vxp_pt") ) { fChain->SetBranchAddress("VxEF_vxp_pt", &VxEF_vxp_pt, &b_VxEF_vxp_pt); }
  if( fChain->GetBranchStatus("VxEF_vxp_vtxchi2prob") ) { fChain->SetBranchAddress("VxEF_vxp_vtxchi2prob", &VxEF_vxp_vtxchi2prob, &b_VxEF_vxp_vtxchi2prob); }
  if( fChain->GetBranchStatus("VxEF_vxp_numTracksPerVertex") ) { fChain->SetBranchAddress("VxEF_vxp_numTracksPerVertex", &VxEF_vxp_numTracksPerVertex, &b_VxEF_vxp_numTracksPerVertex); }
  if( fChain->GetBranchStatus("VxEF_vxp_totalNumTracks") ) { fChain->SetBranchAddress("VxEF_vxp_totalNumTracks", &VxEF_vxp_totalNumTracks, &b_VxEF_vxp_totalNumTracks); }
  if( fChain->GetBranchStatus("VxEF_vxp_chi2") ) { fChain->SetBranchAddress("VxEF_vxp_chi2", &VxEF_vxp_chi2, &b_VxEF_vxp_chi2); }
  if( fChain->GetBranchStatus("VxEF_vxp_d0") ) { fChain->SetBranchAddress("VxEF_vxp_d0", &VxEF_vxp_d0, &b_VxEF_vxp_d0); }
  if( fChain->GetBranchStatus("VxEF_vxp_sigd0") ) { fChain->SetBranchAddress("VxEF_vxp_sigd0", &VxEF_vxp_sigd0, &b_VxEF_vxp_sigd0); }
  if( fChain->GetBranchStatus("VxEF_vxp_z0") ) { fChain->SetBranchAddress("VxEF_vxp_z0", &VxEF_vxp_z0, &b_VxEF_vxp_z0); }
  if( fChain->GetBranchStatus("VxEF_vxp_sigz0") ) { fChain->SetBranchAddress("VxEF_vxp_sigz0", &VxEF_vxp_sigz0, &b_VxEF_vxp_sigz0); }
  if( fChain->GetBranchStatus("VxEF_vxp_phi") ) { fChain->SetBranchAddress("VxEF_vxp_phi", &VxEF_vxp_phi, &b_VxEF_vxp_phi); }
  if( fChain->GetBranchStatus("VxEF_vxp_sigphi") ) { fChain->SetBranchAddress("VxEF_vxp_sigphi", &VxEF_vxp_sigphi, &b_VxEF_vxp_sigphi); }
  if( fChain->GetBranchStatus("VxEF_vxp_theta") ) { fChain->SetBranchAddress("VxEF_vxp_theta", &VxEF_vxp_theta, &b_VxEF_vxp_theta); }
  if( fChain->GetBranchStatus("VxEF_vxp_sigtheta") ) { fChain->SetBranchAddress("VxEF_vxp_sigtheta", &VxEF_vxp_sigtheta, &b_VxEF_vxp_sigtheta); }
  if( fChain->GetBranchStatus("VxEF_vxp_qOverP") ) { fChain->SetBranchAddress("VxEF_vxp_qOverP", &VxEF_vxp_qOverP, &b_VxEF_vxp_qOverP); }
  if( fChain->GetBranchStatus("VxEF_vxp_sigqOverP") ) { fChain->SetBranchAddress("VxEF_vxp_sigqOverP", &VxEF_vxp_sigqOverP, &b_VxEF_vxp_sigqOverP); }
  if( fChain->GetBranchStatus("VxEF_vxp_d0g") ) { fChain->SetBranchAddress("VxEF_vxp_d0g", &VxEF_vxp_d0g, &b_VxEF_vxp_d0g); }
  if( fChain->GetBranchStatus("VxEF_vxp_sigd0g") ) { fChain->SetBranchAddress("VxEF_vxp_sigd0g", &VxEF_vxp_sigd0g, &b_VxEF_vxp_sigd0g); }
  if( fChain->GetBranchStatus("VxEF_vxp_z0g") ) { fChain->SetBranchAddress("VxEF_vxp_z0g", &VxEF_vxp_z0g, &b_VxEF_vxp_z0g); }
  if( fChain->GetBranchStatus("VxEF_vxp_sigz0g") ) { fChain->SetBranchAddress("VxEF_vxp_sigz0g", &VxEF_vxp_sigz0g, &b_VxEF_vxp_sigz0g); }
  if( fChain->GetBranchStatus("eg_nc_EF") ) { fChain->SetBranchAddress("eg_nc_EF", &eg_nc_EF, &b_eg_nc_EF); }
  if( fChain->GetBranchStatus("eg_e_EF") ) { fChain->SetBranchAddress("eg_e_EF", &eg_e_EF, &b_eg_e_EF); }
  if( fChain->GetBranchStatus("eg_eta_EF") ) { fChain->SetBranchAddress("eg_eta_EF", &eg_eta_EF, &b_eg_eta_EF); }
  if( fChain->GetBranchStatus("eg_phi_EF") ) { fChain->SetBranchAddress("eg_phi_EF", &eg_phi_EF, &b_eg_phi_EF); }
  if( fChain->GetBranchStatus("eg_cl_et_EF") ) { fChain->SetBranchAddress("eg_cl_et_EF", &eg_cl_et_EF, &b_eg_cl_et_EF); }
  if( fChain->GetBranchStatus("eg_cl_eta_EF") ) { fChain->SetBranchAddress("eg_cl_eta_EF", &eg_cl_eta_EF, &b_eg_cl_eta_EF); }
  if( fChain->GetBranchStatus("eg_cl_phi_EF") ) { fChain->SetBranchAddress("eg_cl_phi_EF", &eg_cl_phi_EF, &b_eg_cl_phi_EF); }
  if( fChain->GetBranchStatus("eg_etap_EF") ) { fChain->SetBranchAddress("eg_etap_EF", &eg_etap_EF, &b_eg_etap_EF); }
  if( fChain->GetBranchStatus("eg_zvertex_EF") ) { fChain->SetBranchAddress("eg_zvertex_EF", &eg_zvertex_EF, &b_eg_zvertex_EF); }
  if( fChain->GetBranchStatus("eg_errz_EF") ) { fChain->SetBranchAddress("eg_errz_EF", &eg_errz_EF, &b_eg_errz_EF); }
  if( fChain->GetBranchStatus("eg_depth_EF") ) { fChain->SetBranchAddress("eg_depth_EF", &eg_depth_EF, &b_eg_depth_EF); }
  if( fChain->GetBranchStatus("eg_e0_EF") ) { fChain->SetBranchAddress("eg_e0_EF", &eg_e0_EF, &b_eg_e0_EF); }
  if( fChain->GetBranchStatus("eg_e1_EF") ) { fChain->SetBranchAddress("eg_e1_EF", &eg_e1_EF, &b_eg_e1_EF); }
  if( fChain->GetBranchStatus("eg_e2_EF") ) { fChain->SetBranchAddress("eg_e2_EF", &eg_e2_EF, &b_eg_e2_EF); }
  if( fChain->GetBranchStatus("eg_e3_EF") ) { fChain->SetBranchAddress("eg_e3_EF", &eg_e3_EF, &b_eg_e3_EF); }
  if( fChain->GetBranchStatus("eg_eta0_EF") ) { fChain->SetBranchAddress("eg_eta0_EF", &eg_eta0_EF, &b_eg_eta0_EF); }
  if( fChain->GetBranchStatus("eg_eta1_EF") ) { fChain->SetBranchAddress("eg_eta1_EF", &eg_eta1_EF, &b_eg_eta1_EF); }
  if( fChain->GetBranchStatus("eg_eta2_EF") ) { fChain->SetBranchAddress("eg_eta2_EF", &eg_eta2_EF, &b_eg_eta2_EF); }
  if( fChain->GetBranchStatus("eg_eta3_EF") ) { fChain->SetBranchAddress("eg_eta3_EF", &eg_eta3_EF, &b_eg_eta3_EF); }
  if( fChain->GetBranchStatus("eg_phi0_EF") ) { fChain->SetBranchAddress("eg_phi0_EF", &eg_phi0_EF, &b_eg_phi0_EF); }
  if( fChain->GetBranchStatus("eg_phi1_EF") ) { fChain->SetBranchAddress("eg_phi1_EF", &eg_phi1_EF, &b_eg_phi1_EF); }
  if( fChain->GetBranchStatus("eg_phi2_EF") ) { fChain->SetBranchAddress("eg_phi2_EF", &eg_phi2_EF, &b_eg_phi2_EF); }
  if( fChain->GetBranchStatus("eg_phi3_EF") ) { fChain->SetBranchAddress("eg_phi3_EF", &eg_phi3_EF, &b_eg_phi3_EF); }
  if( fChain->GetBranchStatus("eg_Etha1_EF") ) { fChain->SetBranchAddress("eg_Etha1_EF", &eg_Etha1_EF, &b_eg_Etha1_EF); }
  if( fChain->GetBranchStatus("eg_Etha_EF") ) { fChain->SetBranchAddress("eg_Etha_EF", &eg_Etha_EF, &b_eg_Etha_EF); }
  if( fChain->GetBranchStatus("eg_Eha1_EF") ) { fChain->SetBranchAddress("eg_Eha1_EF", &eg_Eha1_EF, &b_eg_Eha1_EF); }
  if( fChain->GetBranchStatus("eg_F1_EF") ) { fChain->SetBranchAddress("eg_F1_EF", &eg_F1_EF, &b_eg_F1_EF); }
  if( fChain->GetBranchStatus("eg_F3_EF") ) { fChain->SetBranchAddress("eg_F3_EF", &eg_F3_EF, &b_eg_F3_EF); }
  if( fChain->GetBranchStatus("eg_E233_EF") ) { fChain->SetBranchAddress("eg_E233_EF", &eg_E233_EF, &b_eg_E233_EF); }
  if( fChain->GetBranchStatus("eg_E237_EF") ) { fChain->SetBranchAddress("eg_E237_EF", &eg_E237_EF, &b_eg_E237_EF); }
  if( fChain->GetBranchStatus("eg_E277_EF") ) { fChain->SetBranchAddress("eg_E277_EF", &eg_E277_EF, &b_eg_E277_EF); }
  if( fChain->GetBranchStatus("eg_Weta1_EF") ) { fChain->SetBranchAddress("eg_Weta1_EF", &eg_Weta1_EF, &b_eg_Weta1_EF); }
  if( fChain->GetBranchStatus("eg_Weta2_EF") ) { fChain->SetBranchAddress("eg_Weta2_EF", &eg_Weta2_EF, &b_eg_Weta2_EF); }
  if( fChain->GetBranchStatus("eg_E2ts1_EF") ) { fChain->SetBranchAddress("eg_E2ts1_EF", &eg_E2ts1_EF, &b_eg_E2ts1_EF); }
  if( fChain->GetBranchStatus("eg_E2tsts1_EF") ) { fChain->SetBranchAddress("eg_E2tsts1_EF", &eg_E2tsts1_EF, &b_eg_E2tsts1_EF); }
  if( fChain->GetBranchStatus("eg_Widths1_EF") ) { fChain->SetBranchAddress("eg_Widths1_EF", &eg_Widths1_EF, &b_eg_Widths1_EF); }
  if( fChain->GetBranchStatus("eg_Widths2_EF") ) { fChain->SetBranchAddress("eg_Widths2_EF", &eg_Widths2_EF, &b_eg_Widths2_EF); }
  if( fChain->GetBranchStatus("eg_poscs1_EF") ) { fChain->SetBranchAddress("eg_poscs1_EF", &eg_poscs1_EF, &b_eg_poscs1_EF); }
  if( fChain->GetBranchStatus("eg_poscs2_EF") ) { fChain->SetBranchAddress("eg_poscs2_EF", &eg_poscs2_EF, &b_eg_poscs2_EF); }
  if( fChain->GetBranchStatus("eg_Barys1_EF") ) { fChain->SetBranchAddress("eg_Barys1_EF", &eg_Barys1_EF, &b_eg_Barys1_EF); }
  if( fChain->GetBranchStatus("eg_Wtots1_EF") ) { fChain->SetBranchAddress("eg_Wtots1_EF", &eg_Wtots1_EF, &b_eg_Wtots1_EF); }
  if( fChain->GetBranchStatus("eg_Emins1_EF") ) { fChain->SetBranchAddress("eg_Emins1_EF", &eg_Emins1_EF, &b_eg_Emins1_EF); }
  if( fChain->GetBranchStatus("eg_Emaxs1_EF") ) { fChain->SetBranchAddress("eg_Emaxs1_EF", &eg_Emaxs1_EF, &b_eg_Emaxs1_EF); }
  if( fChain->GetBranchStatus("eg_Fracs1_EF") ) { fChain->SetBranchAddress("eg_Fracs1_EF", &eg_Fracs1_EF, &b_eg_Fracs1_EF); }
  if( fChain->GetBranchStatus("eg_EtCone45_EF") ) { fChain->SetBranchAddress("eg_EtCone45_EF", &eg_EtCone45_EF, &b_eg_EtCone45_EF); }
  if( fChain->GetBranchStatus("eg_EtCone20_EF") ) { fChain->SetBranchAddress("eg_EtCone20_EF", &eg_EtCone20_EF, &b_eg_EtCone20_EF); }
  if( fChain->GetBranchStatus("eg_EtCone30_EF") ) { fChain->SetBranchAddress("eg_EtCone30_EF", &eg_EtCone30_EF, &b_eg_EtCone30_EF); }
  if( fChain->GetBranchStatus("eg_EtCone40_EF") ) { fChain->SetBranchAddress("eg_EtCone40_EF", &eg_EtCone40_EF, &b_eg_EtCone40_EF); }
  if( fChain->GetBranchStatus("eg_IsEM_EF") ) { fChain->SetBranchAddress("eg_IsEM_EF", &eg_IsEM_EF, &b_eg_IsEM_EF); }
  if( fChain->GetBranchStatus("eg_epiNN_EF") ) { fChain->SetBranchAddress("eg_epiNN_EF", &eg_epiNN_EF, &b_eg_epiNN_EF); }
  if( fChain->GetBranchStatus("eg_EMWeight_EF") ) { fChain->SetBranchAddress("eg_EMWeight_EF", &eg_EMWeight_EF, &b_eg_EMWeight_EF); }
  if( fChain->GetBranchStatus("eg_PionWeight_EF") ) { fChain->SetBranchAddress("eg_PionWeight_EF", &eg_PionWeight_EF, &b_eg_PionWeight_EF); }
  if( fChain->GetBranchStatus("eg_Hmatrix_EF") ) { fChain->SetBranchAddress("eg_Hmatrix_EF", &eg_Hmatrix_EF, &b_eg_Hmatrix_EF); }
  if( fChain->GetBranchStatus("eg_IsEMse_EF") ) { fChain->SetBranchAddress("eg_IsEMse_EF", &eg_IsEMse_EF, &b_eg_IsEMse_EF); }
  if( fChain->GetBranchStatus("eg_epiNNse_EF") ) { fChain->SetBranchAddress("eg_epiNNse_EF", &eg_epiNNse_EF, &b_eg_epiNNse_EF); }
  if( fChain->GetBranchStatus("eg_EMWeightse_EF") ) { fChain->SetBranchAddress("eg_EMWeightse_EF", &eg_EMWeightse_EF, &b_eg_EMWeightse_EF); }
  if( fChain->GetBranchStatus("eg_PionWeightse_EF") ) { fChain->SetBranchAddress("eg_PionWeightse_EF", &eg_PionWeightse_EF, &b_eg_PionWeightse_EF); }
  if( fChain->GetBranchStatus("eg_E011_EF") ) { fChain->SetBranchAddress("eg_E011_EF", &eg_E011_EF, &b_eg_E011_EF); }
  if( fChain->GetBranchStatus("eg_E033_EF") ) { fChain->SetBranchAddress("eg_E033_EF", &eg_E033_EF, &b_eg_E033_EF); }
  if( fChain->GetBranchStatus("eg_E131_EF") ) { fChain->SetBranchAddress("eg_E131_EF", &eg_E131_EF, &b_eg_E131_EF); }
  if( fChain->GetBranchStatus("eg_E1153_EF") ) { fChain->SetBranchAddress("eg_E1153_EF", &eg_E1153_EF, &b_eg_E1153_EF); }
  if( fChain->GetBranchStatus("eg_E235_EF") ) { fChain->SetBranchAddress("eg_E235_EF", &eg_E235_EF, &b_eg_E235_EF); }
  if( fChain->GetBranchStatus("eg_E255_EF") ) { fChain->SetBranchAddress("eg_E255_EF", &eg_E255_EF, &b_eg_E255_EF); }
  if( fChain->GetBranchStatus("eg_E333_EF") ) { fChain->SetBranchAddress("eg_E333_EF", &eg_E333_EF, &b_eg_E333_EF); }
  if( fChain->GetBranchStatus("eg_E335_EF") ) { fChain->SetBranchAddress("eg_E335_EF", &eg_E335_EF, &b_eg_E335_EF); }
  if( fChain->GetBranchStatus("eg_E337_EF") ) { fChain->SetBranchAddress("eg_E337_EF", &eg_E337_EF, &b_eg_E337_EF); }
  if( fChain->GetBranchStatus("eg_E377_EF") ) { fChain->SetBranchAddress("eg_E377_EF", &eg_E377_EF, &b_eg_E377_EF); }
  if( fChain->GetBranchStatus("eg_trkmatchnt_EF") ) { fChain->SetBranchAddress("eg_trkmatchnt_EF", &eg_trkmatchnt_EF, &b_eg_trkmatchnt_EF); }
  if( fChain->GetBranchStatus("eg_primary_EF") ) { fChain->SetBranchAddress("eg_primary_EF", &eg_primary_EF, &b_eg_primary_EF); }
  if( fChain->GetBranchStatus("eg_numtracks_EF") ) { fChain->SetBranchAddress("eg_numtracks_EF", &eg_numtracks_EF, &b_eg_numtracks_EF); }
  if( fChain->GetBranchStatus("eg_eoverp_EF") ) { fChain->SetBranchAddress("eg_eoverp_EF", &eg_eoverp_EF, &b_eg_eoverp_EF); }
  if( fChain->GetBranchStatus("eg_etacorr_EF") ) { fChain->SetBranchAddress("eg_etacorr_EF", &eg_etacorr_EF, &b_eg_etacorr_EF); }
  if( fChain->GetBranchStatus("eg_deta0_EF") ) { fChain->SetBranchAddress("eg_deta0_EF", &eg_deta0_EF, &b_eg_deta0_EF); }
  if( fChain->GetBranchStatus("eg_dphi0_EF") ) { fChain->SetBranchAddress("eg_dphi0_EF", &eg_dphi0_EF, &b_eg_dphi0_EF); }
  if( fChain->GetBranchStatus("eg_deta1_EF") ) { fChain->SetBranchAddress("eg_deta1_EF", &eg_deta1_EF, &b_eg_deta1_EF); }
  if( fChain->GetBranchStatus("eg_dphi1_EF") ) { fChain->SetBranchAddress("eg_dphi1_EF", &eg_dphi1_EF, &b_eg_dphi1_EF); }
  if( fChain->GetBranchStatus("eg_deta2_EF") ) { fChain->SetBranchAddress("eg_deta2_EF", &eg_deta2_EF, &b_eg_deta2_EF); }
  if( fChain->GetBranchStatus("eg_dphi2_EF") ) { fChain->SetBranchAddress("eg_dphi2_EF", &eg_dphi2_EF, &b_eg_dphi2_EF); }
  if( fChain->GetBranchStatus("eg_deta3_EF") ) { fChain->SetBranchAddress("eg_deta3_EF", &eg_deta3_EF, &b_eg_deta3_EF); }
  if( fChain->GetBranchStatus("eg_dphi3_EF") ) { fChain->SetBranchAddress("eg_dphi3_EF", &eg_dphi3_EF, &b_eg_dphi3_EF); }
  if( fChain->GetBranchStatus("eg_trkopt_EF") ) { fChain->SetBranchAddress("eg_trkopt_EF", &eg_trkopt_EF, &b_eg_trkopt_EF); }
  if( fChain->GetBranchStatus("eg_trkinvpTV_EF") ) { fChain->SetBranchAddress("eg_trkinvpTV_EF", &eg_trkinvpTV_EF, &b_eg_trkinvpTV_EF); }
  if( fChain->GetBranchStatus("eg_trkcotThV_EF") ) { fChain->SetBranchAddress("eg_trkcotThV_EF", &eg_trkcotThV_EF, &b_eg_trkcotThV_EF); }
  if( fChain->GetBranchStatus("eg_trkphiV_EF") ) { fChain->SetBranchAddress("eg_trkphiV_EF", &eg_trkphiV_EF, &b_eg_trkphiV_EF); }
  if( fChain->GetBranchStatus("egRoiId_EF") ) { fChain->SetBranchAddress("egRoiId_EF", &egRoiId_EF, &b_egRoiId_EF); }
  if( fChain->GetBranchStatus("egKey_EF") ) { fChain->SetBranchAddress("egKey_EF", &egKey_EF, &b_egKey_EF); }
  if( fChain->GetBranchStatus("T2Bphys_NPar") ) { fChain->SetBranchAddress("T2Bphys_NPar", &T2Bphys_NPar, &b_T2Bphys_NPar); }
  if( fChain->GetBranchStatus("T2Bphys_roiId") ) { fChain->SetBranchAddress("T2Bphys_roiId", &T2Bphys_roiId, &b_T2Bphys_roiId); }
  if( fChain->GetBranchStatus("T2Bphys_eta") ) { fChain->SetBranchAddress("T2Bphys_eta", &T2Bphys_eta, &b_T2Bphys_eta); }
  if( fChain->GetBranchStatus("T2Bphys_phi") ) { fChain->SetBranchAddress("T2Bphys_phi", &T2Bphys_phi, &b_T2Bphys_phi); }
  if( fChain->GetBranchStatus("T2Bphys_pType") ) { fChain->SetBranchAddress("T2Bphys_pType", &T2Bphys_pType, &b_T2Bphys_pType); }
  if( fChain->GetBranchStatus("T2Bphys_mass") ) { fChain->SetBranchAddress("T2Bphys_mass", &T2Bphys_mass, &b_T2Bphys_mass); }
  if( fChain->GetBranchStatus("T2Bphys_fitmass") ) { fChain->SetBranchAddress("T2Bphys_fitmass", &T2Bphys_fitmass, &b_T2Bphys_fitmass); }
  if( fChain->GetBranchStatus("T2Bphys_fitchis") ) { fChain->SetBranchAddress("T2Bphys_fitchis", &T2Bphys_fitchis, &b_T2Bphys_fitchis); }
  if( fChain->GetBranchStatus("T2Bphys_fitprob") ) { fChain->SetBranchAddress("T2Bphys_fitprob", &T2Bphys_fitprob, &b_T2Bphys_fitprob); }
  if( fChain->GetBranchStatus("T2Bphys_secmass") ) { fChain->SetBranchAddress("T2Bphys_secmass", &T2Bphys_secmass, &b_T2Bphys_secmass); }
  if( fChain->GetBranchStatus("T2Bphys_secfitmass") ) { fChain->SetBranchAddress("T2Bphys_secfitmass", &T2Bphys_secfitmass, &b_T2Bphys_secfitmass); }
  if( fChain->GetBranchStatus("T2Bphys_secfitchis") ) { fChain->SetBranchAddress("T2Bphys_secfitchis", &T2Bphys_secfitchis, &b_T2Bphys_secfitchis); }
  if( fChain->GetBranchStatus("T2Bphys_secfitprob") ) { fChain->SetBranchAddress("T2Bphys_secfitprob", &T2Bphys_secfitprob, &b_T2Bphys_secfitprob); }
  if( fChain->GetBranchStatus("T2Bphys_pdgid_tr1") ) { fChain->SetBranchAddress("T2Bphys_pdgid_tr1", &T2Bphys_pdgid_tr1, &b_T2Bphys_pdgid_tr1); }
  if( fChain->GetBranchStatus("T2Bphys_pdgid_tr2") ) { fChain->SetBranchAddress("T2Bphys_pdgid_tr2", &T2Bphys_pdgid_tr2, &b_T2Bphys_pdgid_tr2); }
  if( fChain->GetBranchStatus("T2Bphys_pdgid_tr3") ) { fChain->SetBranchAddress("T2Bphys_pdgid_tr3", &T2Bphys_pdgid_tr3, &b_T2Bphys_pdgid_tr3); }
  if( fChain->GetBranchStatus("T2Bphys_NtEnt1") ) { fChain->SetBranchAddress("T2Bphys_NtEnt1", &T2Bphys_NtEnt1, &b_T2Bphys_NtEnt1); }
  if( fChain->GetBranchStatus("T2Bphys_NtEnt2") ) { fChain->SetBranchAddress("T2Bphys_NtEnt2", &T2Bphys_NtEnt2, &b_T2Bphys_NtEnt2); }
  if( fChain->GetBranchStatus("T2Bphys_NtEnt3") ) { fChain->SetBranchAddress("T2Bphys_NtEnt3", &T2Bphys_NtEnt3, &b_T2Bphys_NtEnt3); }
  if( fChain->GetBranchStatus("T2BjN") ) { fChain->SetBranchAddress("T2BjN", &T2BjN, &b_T2BjN); }
  if( fChain->GetBranchStatus("T2BjEta") ) { fChain->SetBranchAddress("T2BjEta", &T2BjEta, &b_T2BjEta); }
  if( fChain->GetBranchStatus("T2BjPhi0") ) { fChain->SetBranchAddress("T2BjPhi0", &T2BjPhi0, &b_T2BjPhi0); }
  if( fChain->GetBranchStatus("T2BjX2d") ) { fChain->SetBranchAddress("T2BjX2d", &T2BjX2d, &b_T2BjX2d); }
  if( fChain->GetBranchStatus("T2BjXd0") ) { fChain->SetBranchAddress("T2BjXd0", &T2BjXd0, &b_T2BjXd0); }
  if( fChain->GetBranchStatus("T2BjXz0") ) { fChain->SetBranchAddress("T2BjXz0", &T2BjXz0, &b_T2BjXz0); }
  if( fChain->GetBranchStatus("T2BjX3d") ) { fChain->SetBranchAddress("T2BjX3d", &T2BjX3d, &b_T2BjX3d); }
  if( fChain->GetBranchStatus("T2BjXmvtx") ) { fChain->SetBranchAddress("T2BjXmvtx", &T2BjXmvtx, &b_T2BjXmvtx); }
  if( fChain->GetBranchStatus("T2BjXevtx") ) { fChain->SetBranchAddress("T2BjXevtx", &T2BjXevtx, &b_T2BjXevtx); }
  if( fChain->GetBranchStatus("T2BjXnvtx") ) { fChain->SetBranchAddress("T2BjXnvtx", &T2BjXnvtx, &b_T2BjXnvtx); }
  if( fChain->GetBranchStatus("T2BjPvtx") ) { fChain->SetBranchAddress("T2BjPvtx", &T2BjPvtx, &b_T2BjPvtx); }
  if( fChain->GetBranchStatus("EFBjN") ) { fChain->SetBranchAddress("EFBjN", &EFBjN, &b_EFBjN); }
  if( fChain->GetBranchStatus("EFBjEta") ) { fChain->SetBranchAddress("EFBjEta", &EFBjEta, &b_EFBjEta); }
  if( fChain->GetBranchStatus("EFBjPhi0") ) { fChain->SetBranchAddress("EFBjPhi0", &EFBjPhi0, &b_EFBjPhi0); }
  if( fChain->GetBranchStatus("EFBjX2d") ) { fChain->SetBranchAddress("EFBjX2d", &EFBjX2d, &b_EFBjX2d); }
  if( fChain->GetBranchStatus("EFBjXd0") ) { fChain->SetBranchAddress("EFBjXd0", &EFBjXd0, &b_EFBjXd0); }
  if( fChain->GetBranchStatus("EFBjXz0") ) { fChain->SetBranchAddress("EFBjXz0", &EFBjXz0, &b_EFBjXz0); }
  if( fChain->GetBranchStatus("EFBjX3d") ) { fChain->SetBranchAddress("EFBjX3d", &EFBjX3d, &b_EFBjX3d); }
  if( fChain->GetBranchStatus("EFBjXmvtx") ) { fChain->SetBranchAddress("EFBjXmvtx", &EFBjXmvtx, &b_EFBjXmvtx); }
  if( fChain->GetBranchStatus("EFBjXevtx") ) { fChain->SetBranchAddress("EFBjXevtx", &EFBjXevtx, &b_EFBjXevtx); }
  if( fChain->GetBranchStatus("EFBjXnvtx") ) { fChain->SetBranchAddress("EFBjXnvtx", &EFBjXnvtx, &b_EFBjXnvtx); }
  if( fChain->GetBranchStatus("EFBjPvtx") ) { fChain->SetBranchAddress("EFBjPvtx", &EFBjPvtx, &b_EFBjPvtx); }
  if( fChain->GetBranchStatus("tau_L2ntaus") ) { fChain->SetBranchAddress("tau_L2ntaus", &tau_L2ntaus, &b_tau_ntaus); }
  if( fChain->GetBranchStatus("tau_L2eta") ) { fChain->SetBranchAddress("tau_L2eta", &tau_L2eta, &b_tau_L2eta); }
  if( fChain->GetBranchStatus("tau_L2phi") ) { fChain->SetBranchAddress("tau_L2phi", &tau_L2phi, &b_tau_L2phi); }
  if( fChain->GetBranchStatus("tau_L2pt") ) { fChain->SetBranchAddress("tau_L2pt", &tau_L2pt, &b_tau_L2pt); }
  if( fChain->GetBranchStatus("tau_L2Zvtx") ) { fChain->SetBranchAddress("tau_L2Zvtx", &tau_L2Zvtx, &b_tau_L2Zvtx); }
  if( fChain->GetBranchStatus("tau_L2nMatchTracks") ) { fChain->SetBranchAddress("tau_L2nMatchTracks", &tau_L2nMatchTracks, &b_tau_L2nMatchTracks); }
  if( fChain->GetBranchStatus("tau_L2roiId") ) { fChain->SetBranchAddress("tau_L2roiId", &tau_L2roiId, &b_tau_L2roiId); }
  if( fChain->GetBranchStatus("Trig_DecisionL1") ) { fChain->SetBranchAddress("Trig_DecisionL1", &Trig_DecisionL1, &b_Trig_DecisionL1); }
  if( fChain->GetBranchStatus("Trig_DecisionL2") ) { fChain->SetBranchAddress("Trig_DecisionL2", &Trig_DecisionL2, &b_Trig_DecisionL2); }
  if( fChain->GetBranchStatus("Trig_DecisionEF") ) { fChain->SetBranchAddress("Trig_DecisionEF", &Trig_DecisionEF, &b_Trig_DecisionEF); }
  if( fChain->GetBranchStatus("Trig_EM01") ) { fChain->SetBranchAddress("Trig_EM01", &Trig_EM01, &b_Trig_EM01); }
  if( fChain->GetBranchStatus("Trig_L1_2EM15") ) { fChain->SetBranchAddress("Trig_L1_2EM15", &Trig_L1_2EM15, &b_Trig_L1_2EM15); }
  if( fChain->GetBranchStatus("Trig_L1_2EM15I") ) { fChain->SetBranchAddress("Trig_L1_2EM15I", &Trig_L1_2EM15I, &b_Trig_L1_2EM15I); }
  if( fChain->GetBranchStatus("Trig_L1_EM25") ) { fChain->SetBranchAddress("Trig_L1_EM25", &Trig_L1_EM25, &b_Trig_L1_EM25); }
  if( fChain->GetBranchStatus("Trig_L1_EM25I") ) { fChain->SetBranchAddress("Trig_L1_EM25I", &Trig_L1_EM25I, &b_Trig_L1_EM25I); }
  if( fChain->GetBranchStatus("Trig_L1_EM60") ) { fChain->SetBranchAddress("Trig_L1_EM60", &Trig_L1_EM60, &b_Trig_L1_EM60); }
  if( fChain->GetBranchStatus("Trig_L1_XE20") ) { fChain->SetBranchAddress("Trig_L1_XE20", &Trig_L1_XE20, &b_Trig_L1_XE20); }
  if( fChain->GetBranchStatus("Trig_L1_XE30") ) { fChain->SetBranchAddress("Trig_L1_XE30", &Trig_L1_XE30, &b_Trig_L1_XE30); }
  if( fChain->GetBranchStatus("Trig_L1_XE40") ) { fChain->SetBranchAddress("Trig_L1_XE40", &Trig_L1_XE40, &b_Trig_L1_XE40); }
  if( fChain->GetBranchStatus("Trig_L1_XE50") ) { fChain->SetBranchAddress("Trig_L1_XE50", &Trig_L1_XE50, &b_Trig_L1_XE50); }
  if( fChain->GetBranchStatus("Trig_L1_XE100") ) { fChain->SetBranchAddress("Trig_L1_XE100", &Trig_L1_XE100, &b_Trig_L1_XE100); }
  if( fChain->GetBranchStatus("Trig_L1_XE200") ) { fChain->SetBranchAddress("Trig_L1_XE200", &Trig_L1_XE200, &b_Trig_L1_XE200); }
  if( fChain->GetBranchStatus("Trig_L1_TAU05") ) { fChain->SetBranchAddress("Trig_L1_TAU05", &Trig_L1_TAU05, &b_Trig_L1_TAU05); }
  if( fChain->GetBranchStatus("Trig_L1_TAU10") ) { fChain->SetBranchAddress("Trig_L1_TAU10", &Trig_L1_TAU10, &b_Trig_L1_TAU10); }
  if( fChain->GetBranchStatus("Trig_L1_TAU10I") ) { fChain->SetBranchAddress("Trig_L1_TAU10I", &Trig_L1_TAU10I, &b_Trig_L1_TAU10I); }
  if( fChain->GetBranchStatus("Trig_L1_TAU15") ) { fChain->SetBranchAddress("Trig_L1_TAU15", &Trig_L1_TAU15, &b_Trig_L1_TAU15); }
  if( fChain->GetBranchStatus("Trig_L1_TAU15I") ) { fChain->SetBranchAddress("Trig_L1_TAU15I", &Trig_L1_TAU15I, &b_Trig_L1_TAU15I); }
  if( fChain->GetBranchStatus("Trig_L1_TAU20I") ) { fChain->SetBranchAddress("Trig_L1_TAU20I", &Trig_L1_TAU20I, &b_Trig_L1_TAU20I); }
  if( fChain->GetBranchStatus("Trig_L1_TAU25I") ) { fChain->SetBranchAddress("Trig_L1_TAU25I", &Trig_L1_TAU25I, &b_Trig_L1_TAU25I); }
  if( fChain->GetBranchStatus("Trig_L1_TAU35I") ) { fChain->SetBranchAddress("Trig_L1_TAU35I", &Trig_L1_TAU35I, &b_Trig_L1_TAU35I); }
  if( fChain->GetBranchStatus("Trig_L1_MU06") ) { fChain->SetBranchAddress("Trig_L1_MU06", &Trig_L1_MU06, &b_Trig_L1_MU06); }
  if( fChain->GetBranchStatus("Trig_L1_2MU06") ) { fChain->SetBranchAddress("Trig_L1_2MU06", &Trig_L1_2MU06, &b_Trig_L1_2MU06); }
  if( fChain->GetBranchStatus("Trig_L1_MU08") ) { fChain->SetBranchAddress("Trig_L1_MU08", &Trig_L1_MU08, &b_Trig_L1_MU08); }
  if( fChain->GetBranchStatus("Trig_L1_MU10") ) { fChain->SetBranchAddress("Trig_L1_MU10", &Trig_L1_MU10, &b_Trig_L1_MU10); }
  if( fChain->GetBranchStatus("Trig_L1_MU11") ) { fChain->SetBranchAddress("Trig_L1_MU11", &Trig_L1_MU11, &b_Trig_L1_MU11); }
  if( fChain->GetBranchStatus("Trig_L1_MU20") ) { fChain->SetBranchAddress("Trig_L1_MU20", &Trig_L1_MU20, &b_Trig_L1_MU20); }
  if( fChain->GetBranchStatus("Trig_L1_MU40") ) { fChain->SetBranchAddress("Trig_L1_MU40", &Trig_L1_MU40, &b_Trig_L1_MU40); }
  if( fChain->GetBranchStatus("Trig_L1_J35") ) { fChain->SetBranchAddress("Trig_L1_J35", &Trig_L1_J35, &b_Trig_L1_J35); }
  if( fChain->GetBranchStatus("Trig_L1_J45") ) { fChain->SetBranchAddress("Trig_L1_J45", &Trig_L1_J45, &b_Trig_L1_J45); }
  if( fChain->GetBranchStatus("Trig_L1_2J45") ) { fChain->SetBranchAddress("Trig_L1_2J45", &Trig_L1_2J45, &b_Trig_L1_2J45); }
  if( fChain->GetBranchStatus("Trig_L1_3J45") ) { fChain->SetBranchAddress("Trig_L1_3J45", &Trig_L1_3J45, &b_Trig_L1_3J45); }
  if( fChain->GetBranchStatus("Trig_L1_4J45") ) { fChain->SetBranchAddress("Trig_L1_4J45", &Trig_L1_4J45, &b_Trig_L1_4J45); }
  if( fChain->GetBranchStatus("Trig_L1_FJ30") ) { fChain->SetBranchAddress("Trig_L1_FJ30", &Trig_L1_FJ30, &b_Trig_L1_FJ30); }
  if( fChain->GetBranchStatus("Trig_L1_J60") ) { fChain->SetBranchAddress("Trig_L1_J60", &Trig_L1_J60, &b_Trig_L1_J60); }
  if( fChain->GetBranchStatus("Trig_L1_J80") ) { fChain->SetBranchAddress("Trig_L1_J80", &Trig_L1_J80, &b_Trig_L1_J80); }
  if( fChain->GetBranchStatus("Trig_L1_J170") ) { fChain->SetBranchAddress("Trig_L1_J170", &Trig_L1_J170, &b_Trig_L1_J170); }
  if( fChain->GetBranchStatus("Trig_L1_J300") ) { fChain->SetBranchAddress("Trig_L1_J300", &Trig_L1_J300, &b_Trig_L1_J300); }
  if( fChain->GetBranchStatus("Trig_L1_BJT15") ) { fChain->SetBranchAddress("Trig_L1_BJT15", &Trig_L1_BJT15, &b_Trig_L1_BJT15); }
  if( fChain->GetBranchStatus("Trig_L1_EM5") ) { fChain->SetBranchAddress("Trig_L1_EM5", &Trig_L1_EM5, &b_Trig_L1_EM5); }
  if( fChain->GetBranchStatus("Trig_L2_met10f") ) { fChain->SetBranchAddress("Trig_L2_met10f", &Trig_L2_met10f, &b_Trig_L2_met10f); }
  if( fChain->GetBranchStatus("Trig_L2_g10") ) { fChain->SetBranchAddress("Trig_L2_g10", &Trig_L2_g10, &b_Trig_L2_g10); }
  if( fChain->GetBranchStatus("Trig_L2_g20iL2_g20i") ) { fChain->SetBranchAddress("Trig_L2_g20iL2_g20i", &Trig_L2_g20iL2_g20i, &b_Trig_L2_g20iL2_g20i); }
  if( fChain->GetBranchStatus("Trig_L2_g60") ) { fChain->SetBranchAddress("Trig_L2_g60", &Trig_L2_g60, &b_Trig_L2_g60); }
  if( fChain->GetBranchStatus("Trig_L2_jet20a") ) { fChain->SetBranchAddress("Trig_L2_jet20a", &Trig_L2_jet20a, &b_Trig_L2_jet20a); }
  if( fChain->GetBranchStatus("Trig_L2_jet20bL2_jet20b") ) { fChain->SetBranchAddress("Trig_L2_jet20bL2_jet20b", &Trig_L2_jet20bL2_jet20b, &b_Trig_L2_jet20bL2_jet20b); }
  if( fChain->GetBranchStatus("Trig_L2_jet20cL2_jet20cL2_jet20c") ) { fChain->SetBranchAddress("Trig_L2_jet20cL2_jet20cL2_jet20c", &Trig_L2_jet20cL2_jet20cL2_jet20c, &b_Trig_L2_jet20cL2_jet20cL2_jet20c); }
  if( fChain->GetBranchStatus("Trig_L2_jet20dL2_jet20dL2_jet20dL2_jet20d") ) { fChain->SetBranchAddress("Trig_L2_jet20dL2_jet20dL2_jet20dL2_jet20d", &Trig_L2_jet20dL2_jet20dL2_jet20dL2_jet20d, &b_Trig_L2_jet20dL2_jet20dL2_jet20dL2_jet20d); }
  if( fChain->GetBranchStatus("Trig_L2_jet20kt") ) { fChain->SetBranchAddress("Trig_L2_jet20kt", &Trig_L2_jet20kt, &b_Trig_L2_jet20kt); }
  if( fChain->GetBranchStatus("Trig_L2_jet160") ) { fChain->SetBranchAddress("Trig_L2_jet160", &Trig_L2_jet160, &b_Trig_L2_jet160); }
  if( fChain->GetBranchStatus("Trig_L2_jet120L2_jet120") ) { fChain->SetBranchAddress("Trig_L2_jet120L2_jet120", &Trig_L2_jet120L2_jet120, &b_Trig_L2_jet120L2_jet120); }
  if( fChain->GetBranchStatus("Trig_L2_jet65L2_jet65L2_jet65") ) { fChain->SetBranchAddress("Trig_L2_jet65L2_jet65L2_jet65", &Trig_L2_jet65L2_jet65L2_jet65, &b_Trig_L2_jet65L2_jet65L2_jet65); }
  if( fChain->GetBranchStatus("Trig_L2_jet50L2_jet50L2_jet50L2_jet50") ) { fChain->SetBranchAddress("Trig_L2_jet50L2_jet50L2_jet50L2_jet50", &Trig_L2_jet50L2_jet50L2_jet50L2_jet50, &b_Trig_L2_jet50L2_jet50L2_jet50L2_jet50); }
  if( fChain->GetBranchStatus("Trig_L2_frjet10") ) { fChain->SetBranchAddress("Trig_L2_frjet10", &Trig_L2_frjet10, &b_Trig_L2_frjet10); }
  if( fChain->GetBranchStatus("Trig_L2_fljet10") ) { fChain->SetBranchAddress("Trig_L2_fljet10", &Trig_L2_fljet10, &b_Trig_L2_fljet10); }
  if( fChain->GetBranchStatus("Trig_L2_BsDsPhiPi") ) { fChain->SetBranchAddress("Trig_L2_BsDsPhiPi", &Trig_L2_BsDsPhiPi, &b_Trig_L2_BsDsPhiPi); }
  if( fChain->GetBranchStatus("Trig_L2_BsDsPhiPiFullSCan") ) { fChain->SetBranchAddress("Trig_L2_BsDsPhiPiFullSCan", &Trig_L2_BsDsPhiPiFullSCan, &b_Trig_L2_BsDsPhiPiFullSCan); }
  if( fChain->GetBranchStatus("Trig_L2_BJpsiMU6mu") ) { fChain->SetBranchAddress("Trig_L2_BJpsiMU6mu", &Trig_L2_BJpsiMU6mu, &b_Trig_L2_BJpsiMU6mu); }
  if( fChain->GetBranchStatus("Trig_L2_b35") ) { fChain->SetBranchAddress("Trig_L2_b35", &Trig_L2_b35, &b_Trig_L2_b35); }
  if( fChain->GetBranchStatus("Trig_L2_e10TRTxK") ) { fChain->SetBranchAddress("Trig_L2_e10TRTxK", &Trig_L2_e10TRTxK, &b_Trig_L2_e10TRTxK); }
  if( fChain->GetBranchStatus("Trig_L2_e15iL2_e15i") ) { fChain->SetBranchAddress("Trig_L2_e15iL2_e15i", &Trig_L2_e15iL2_e15i, &b_Trig_L2_e15iL2_e15i); }
  if( fChain->GetBranchStatus("Trig_L2_e25i") ) { fChain->SetBranchAddress("Trig_L2_e25i", &Trig_L2_e25i, &b_Trig_L2_e25i); }
  if( fChain->GetBranchStatus("Trig_L2_e60") ) { fChain->SetBranchAddress("Trig_L2_e60", &Trig_L2_e60, &b_Trig_L2_e60); }
  if( fChain->GetBranchStatus("Trig_L2_e10") ) { fChain->SetBranchAddress("Trig_L2_e10", &Trig_L2_e10, &b_Trig_L2_e10); }
  if( fChain->GetBranchStatus("Trig_L2_e10L2_e10") ) { fChain->SetBranchAddress("Trig_L2_e10L2_e10", &Trig_L2_e10L2_e10, &b_Trig_L2_e10L2_e10); }
  if( fChain->GetBranchStatus("Trig_L2_mu6l") ) { fChain->SetBranchAddress("Trig_L2_mu6l", &Trig_L2_mu6l, &b_Trig_L2_mu6l); }
  if( fChain->GetBranchStatus("Trig_L2_mu20i") ) { fChain->SetBranchAddress("Trig_L2_mu20i", &Trig_L2_mu20i, &b_Trig_L2_mu20i); }
  if( fChain->GetBranchStatus("Trig_L2_mu6lL2_mu6l") ) { fChain->SetBranchAddress("Trig_L2_mu6lL2_mu6l", &Trig_L2_mu6lL2_mu6l, &b_Trig_L2_mu6lL2_mu6l); }
  if( fChain->GetBranchStatus("Trig_L2_mu6L2_mu6") ) { fChain->SetBranchAddress("Trig_L2_mu6L2_mu6", &Trig_L2_mu6L2_mu6, &b_Trig_L2_mu6L2_mu6); }
  if( fChain->GetBranchStatus("Trig_L2_mu6") ) { fChain->SetBranchAddress("Trig_L2_mu6", &Trig_L2_mu6, &b_Trig_L2_mu6); }
  if( fChain->GetBranchStatus("Trig_L2_Ze10e10") ) { fChain->SetBranchAddress("Trig_L2_Ze10e10", &Trig_L2_Ze10e10, &b_Trig_L2_Ze10e10); }
  if( fChain->GetBranchStatus("Trig_L2_tau10") ) { fChain->SetBranchAddress("Trig_L2_tau10", &Trig_L2_tau10, &b_Trig_L2_tau10); }
  if( fChain->GetBranchStatus("Trig_L2_tau10i") ) { fChain->SetBranchAddress("Trig_L2_tau10i", &Trig_L2_tau10i, &b_Trig_L2_tau10i); }
  if( fChain->GetBranchStatus("Trig_L2_tau15") ) { fChain->SetBranchAddress("Trig_L2_tau15", &Trig_L2_tau15, &b_Trig_L2_tau15); }
  if( fChain->GetBranchStatus("Trig_L2_tau15i") ) { fChain->SetBranchAddress("Trig_L2_tau15i", &Trig_L2_tau15i, &b_Trig_L2_tau15i); }
  if( fChain->GetBranchStatus("Trig_L2_tau20i") ) { fChain->SetBranchAddress("Trig_L2_tau20i", &Trig_L2_tau20i, &b_Trig_L2_tau20i); }
  if( fChain->GetBranchStatus("Trig_L2_tau25i") ) { fChain->SetBranchAddress("Trig_L2_tau25i", &Trig_L2_tau25i, &b_Trig_L2_tau25i); }
  if( fChain->GetBranchStatus("Trig_L2_tau35i") ) { fChain->SetBranchAddress("Trig_L2_tau35i", &Trig_L2_tau35i, &b_Trig_L2_tau35i); }
  if( fChain->GetBranchStatus("Trig_L2_tauNoCut") ) { fChain->SetBranchAddress("Trig_L2_tauNoCut", &Trig_L2_tauNoCut, &b_Trig_L2_tauNoCut); }
  if( fChain->GetBranchStatus("Trig_L2_BJpsimu6lmu6l") ) { fChain->SetBranchAddress("Trig_L2_BJpsimu6lmu6l", &Trig_L2_BJpsimu6lmu6l, &b_Trig_L2_BJpsimu6lmu6l); }
  if( fChain->GetBranchStatus("Trig_L2_BJpsimu6mu6") ) { fChain->SetBranchAddress("Trig_L2_BJpsimu6mu6", &Trig_L2_BJpsimu6mu6, &b_Trig_L2_BJpsimu6mu6); }
  if( fChain->GetBranchStatus("Trig_L2_BJpsimu6mu") ) { fChain->SetBranchAddress("Trig_L2_BJpsimu6mu", &Trig_L2_BJpsimu6mu, &b_Trig_L2_BJpsimu6mu); }
  if( fChain->GetBranchStatus("Trig_EF_met10") ) { fChain->SetBranchAddress("Trig_EF_met10", &Trig_EF_met10, &b_Trig_EF_met10); }
  if( fChain->GetBranchStatus("Trig_EF_jet20aEt") ) { fChain->SetBranchAddress("Trig_EF_jet20aEt", &Trig_EF_jet20aEt, &b_Trig_EF_jet20aEt); }
  if( fChain->GetBranchStatus("Trig_EF_jet20bEtEF_jet20bEt") ) { fChain->SetBranchAddress("Trig_EF_jet20bEtEF_jet20bEt", &Trig_EF_jet20bEtEF_jet20bEt, &b_Trig_EF_jet20bEtEF_jet20bEt); }
  if( fChain->GetBranchStatus("Trig_EF_jet20cEtEF_jet20cEtEF_jet20cEt") ) { fChain->SetBranchAddress("Trig_EF_jet20cEtEF_jet20cEtEF_jet20cEt", &Trig_EF_jet20cEtEF_jet20cEtEF_jet20cEt, &b_Trig_EF_jet20cEtEF_jet20cEtEF_jet20cEt); }
  if( fChain->GetBranchStatus("Trig_EF_jet20dEtEF_jet20dEtEF_jet20dEtEF_jet20dEt") ) { fChain->SetBranchAddress("Trig_EF_jet20dEtEF_jet20dEtEF_jet20dEtEF_jet20dEt", &Trig_EF_jet20dEtEF_jet20dEtEF_jet20dEtEF_jet20dEt, &b_Trig_EF_jet20dEtEF_jet20dEtEF_jet20dEtEF_jet20dEt); }
  if( fChain->GetBranchStatus("Trig_EF_jet20kt") ) { fChain->SetBranchAddress("Trig_EF_jet20kt", &Trig_EF_jet20kt, &b_Trig_EF_jet20kt); }
  if( fChain->GetBranchStatus("Trig_EF_jet160") ) { fChain->SetBranchAddress("Trig_EF_jet160", &Trig_EF_jet160, &b_Trig_EF_jet160); }
  if( fChain->GetBranchStatus("Trig_EF_jet120EF_jet120") ) { fChain->SetBranchAddress("Trig_EF_jet120EF_jet120", &Trig_EF_jet120EF_jet120, &b_Trig_EF_jet120EF_jet120); }
  if( fChain->GetBranchStatus("Trig_EF_jet65EF_jet65EF_jet65") ) { fChain->SetBranchAddress("Trig_EF_jet65EF_jet65EF_jet65", &Trig_EF_jet65EF_jet65EF_jet65, &b_Trig_EF_jet65EF_jet65EF_jet65); }
  if( fChain->GetBranchStatus("Trig_EF_jet50EF_jet50EF_jet50EF_jet50") ) { fChain->SetBranchAddress("Trig_EF_jet50EF_jet50EF_jet50EF_jet50", &Trig_EF_jet50EF_jet50EF_jet50EF_jet50, &b_Trig_EF_jet50EF_jet50EF_jet50EF_jet50); }
  if( fChain->GetBranchStatus("Trig_EF_frjet10") ) { fChain->SetBranchAddress("Trig_EF_frjet10", &Trig_EF_frjet10, &b_Trig_EF_frjet10); }
  if( fChain->GetBranchStatus("Trig_EF_fljet10") ) { fChain->SetBranchAddress("Trig_EF_fljet10", &Trig_EF_fljet10, &b_Trig_EF_fljet10); }
  if( fChain->GetBranchStatus("Trig_EF_MuonTRTExt_mu6l") ) { fChain->SetBranchAddress("Trig_EF_MuonTRTExt_mu6l", &Trig_EF_MuonTRTExt_mu6l, &b_Trig_EF_MuonTRTExt_mu6l); }
  if( fChain->GetBranchStatus("Trig_EF_BsDsPhiPi") ) { fChain->SetBranchAddress("Trig_EF_BsDsPhiPi", &Trig_EF_BsDsPhiPi, &b_Trig_EF_BsDsPhiPi); }
  if( fChain->GetBranchStatus("Trig_EF_b35") ) { fChain->SetBranchAddress("Trig_EF_b35", &Trig_EF_b35, &b_Trig_EF_b35); }
  if( fChain->GetBranchStatus("Trig_EF_tau10") ) { fChain->SetBranchAddress("Trig_EF_tau10", &Trig_EF_tau10, &b_Trig_EF_tau10); }
  if( fChain->GetBranchStatus("Trig_EF_tau10i") ) { fChain->SetBranchAddress("Trig_EF_tau10i", &Trig_EF_tau10i, &b_Trig_EF_tau10i); }
  if( fChain->GetBranchStatus("Trig_EF_tau15") ) { fChain->SetBranchAddress("Trig_EF_tau15", &Trig_EF_tau15, &b_Trig_EF_tau15); }
  if( fChain->GetBranchStatus("Trig_EF_tau15i") ) { fChain->SetBranchAddress("Trig_EF_tau15i", &Trig_EF_tau15i, &b_Trig_EF_tau15i); }
  if( fChain->GetBranchStatus("Trig_EF_tau20i") ) { fChain->SetBranchAddress("Trig_EF_tau20i", &Trig_EF_tau20i, &b_Trig_EF_tau20i); }
  if( fChain->GetBranchStatus("Trig_EF_tau25i") ) { fChain->SetBranchAddress("Trig_EF_tau25i", &Trig_EF_tau25i, &b_Trig_EF_tau25i); }
  if( fChain->GetBranchStatus("Trig_EF_tau35i") ) { fChain->SetBranchAddress("Trig_EF_tau35i", &Trig_EF_tau35i, &b_Trig_EF_tau35i); }
  if( fChain->GetBranchStatus("Trig_EF_tauNoCut") ) { fChain->SetBranchAddress("Trig_EF_tauNoCut", &Trig_EF_tauNoCut, &b_Trig_EF_tauNoCut); }
  if( fChain->GetBranchStatus("Trig_EF_mu6l") ) { fChain->SetBranchAddress("Trig_EF_mu6l", &Trig_EF_mu6l, &b_Trig_EF_mu6l); }
  if( fChain->GetBranchStatus("Trig_EF_mu6") ) { fChain->SetBranchAddress("Trig_EF_mu6", &Trig_EF_mu6, &b_Trig_EF_mu6); }
  if( fChain->GetBranchStatus("Trig_EF_mu20i") ) { fChain->SetBranchAddress("Trig_EF_mu20i", &Trig_EF_mu20i, &b_Trig_EF_mu20i); }
  if( fChain->GetBranchStatus("Trig_EF_g10") ) { fChain->SetBranchAddress("Trig_EF_g10", &Trig_EF_g10, &b_Trig_EF_g10); }
  if( fChain->GetBranchStatus("Trig_EF_g20iEF_g20i") ) { fChain->SetBranchAddress("Trig_EF_g20iEF_g20i", &Trig_EF_g20iEF_g20i, &b_Trig_EF_g20iEF_g20i); }
  if( fChain->GetBranchStatus("Trig_EF_g60") ) { fChain->SetBranchAddress("Trig_EF_g60", &Trig_EF_g60, &b_Trig_EF_g60); }
  if( fChain->GetBranchStatus("Trig_EF_e10") ) { fChain->SetBranchAddress("Trig_EF_e10", &Trig_EF_e10, &b_Trig_EF_e10); }
  if( fChain->GetBranchStatus("Trig_EF_e10TRTxK") ) { fChain->SetBranchAddress("Trig_EF_e10TRTxK", &Trig_EF_e10TRTxK, &b_Trig_EF_e10TRTxK); }
  if( fChain->GetBranchStatus("Trig_EF_e15iEF_e15i") ) { fChain->SetBranchAddress("Trig_EF_e15iEF_e15i", &Trig_EF_e15iEF_e15i, &b_Trig_EF_e15iEF_e15i); }
  if( fChain->GetBranchStatus("Trig_EF_e25i") ) { fChain->SetBranchAddress("Trig_EF_e25i", &Trig_EF_e25i, &b_Trig_EF_e25i); }
  if( fChain->GetBranchStatus("Trig_EF_e60") ) { fChain->SetBranchAddress("Trig_EF_e60", &Trig_EF_e60, &b_Trig_EF_e60); }
  if( fChain->GetBranchStatus("nMC") ) { fChain->SetBranchAddress("nMC", &nMC, &b_nMC); }
  if( fChain->GetBranchStatus("mcPx") ) { fChain->SetBranchAddress("mcPx", &mcPx, &b_mcPx); }
  if( fChain->GetBranchStatus("mcPy") ) { fChain->SetBranchAddress("mcPy", &mcPy, &b_mcPy); }
  if( fChain->GetBranchStatus("mcPz") ) { fChain->SetBranchAddress("mcPz", &mcPz, &b_mcPz); }
  if( fChain->GetBranchStatus("mcEne") ) { fChain->SetBranchAddress("mcEne", &mcEne, &b_mcEne); }
  if( fChain->GetBranchStatus("mcEtIsol") ) { fChain->SetBranchAddress("mcEtIsol", &mcEtIsol, &b_mcEtIsol); }
  if( fChain->GetBranchStatus("mcID") ) { fChain->SetBranchAddress("mcID", &mcID, &b_mcID); }
  if( fChain->GetBranchStatus("mcStatus") ) { fChain->SetBranchAddress("mcStatus", &mcStatus, &b_mcStatus); }
  if( fChain->GetBranchStatus("mcBarcode") ) { fChain->SetBranchAddress("mcBarcode", &mcBarcode, &b_mcBarcode); }
  if( fChain->GetBranchStatus("mcCharge") ) { fChain->SetBranchAddress("mcCharge", &mcCharge, &b_mcCharge); }
  if( fChain->GetBranchStatus("mcNMother") ) { fChain->SetBranchAddress("mcNMother", &mcNMother, &b_mcNMother); }
  if( fChain->GetBranchStatus("mcMotherNt") ) { fChain->SetBranchAddress("mcMotherNt", &mcMotherNt, &b_mcMotherNt); }
  if( fChain->GetBranchStatus("mcNChild") ) { fChain->SetBranchAddress("mcNChild", &mcNChild, &b_mcNChild); }
  if( fChain->GetBranchStatus("mcChildNt") ) { fChain->SetBranchAddress("mcChildNt", &mcChildNt, &b_mcChildNt); }
  if( fChain->GetBranchStatus("EleN") ) { fChain->SetBranchAddress("EleN", &EleN, &b_EleN); }
  if( fChain->GetBranchStatus("EleAuthor") ) { fChain->SetBranchAddress("EleAuthor", &EleAuthor, &b_EleAuthor); }
  if( fChain->GetBranchStatus("EleTrkNt") ) { fChain->SetBranchAddress("EleTrkNt", &EleTrkNt, &b_EleTrkNt); }
  if( fChain->GetBranchStatus("EleEta") ) { fChain->SetBranchAddress("EleEta", &EleEta, &b_EleEta); }
  if( fChain->GetBranchStatus("ElePhi") ) { fChain->SetBranchAddress("ElePhi", &ElePhi, &b_ElePhi); }
  if( fChain->GetBranchStatus("ElePt") ) { fChain->SetBranchAddress("ElePt", &ElePt, &b_ElePt); }
  if( fChain->GetBranchStatus("EleIsEM") ) { fChain->SetBranchAddress("EleIsEM", &EleIsEM, &b_EleIsEM); }
  if( fChain->GetBranchStatus("EleZ0") ) { fChain->SetBranchAddress("EleZ0", &EleZ0, &b_EleZ0); }
  if( fChain->GetBranchStatus("EleD0") ) { fChain->SetBranchAddress("EleD0", &EleD0, &b_EleD0); }
  if( fChain->GetBranchStatus("EleNBHits") ) { fChain->SetBranchAddress("EleNBHits", &EleNBHits, &b_EleNBHits); }
  if( fChain->GetBranchStatus("EleNPixelHits") ) { fChain->SetBranchAddress("EleNPixelHits", &EleNPixelHits, &b_EleNPixelHits); }
  if( fChain->GetBranchStatus("EleNSCTHits") ) { fChain->SetBranchAddress("EleNSCTHits", &EleNSCTHits, &b_EleNSCTHits); }
  if( fChain->GetBranchStatus("EleNTRTHits") ) { fChain->SetBranchAddress("EleNTRTHits", &EleNTRTHits, &b_EleNTRTHits); }
  if( fChain->GetBranchStatus("EleNTRHits") ) { fChain->SetBranchAddress("EleNTRHits", &EleNTRHits, &b_EleNTRHits); }
  if( fChain->GetBranchStatus("EleEOverP") ) { fChain->SetBranchAddress("EleEOverP", &EleEOverP, &b_EleEOverP); }
  if( fChain->GetBranchStatus("EleEtaBE2") ) { fChain->SetBranchAddress("EleEtaBE2", &EleEtaBE2, &b_EleEtaBE2); }
  if( fChain->GetBranchStatus("EleEt37") ) { fChain->SetBranchAddress("EleEt37", &EleEt37, &b_EleEt37); }
  if( fChain->GetBranchStatus("EleE237") ) { fChain->SetBranchAddress("EleE237", &EleE237, &b_EleE237); }
  if( fChain->GetBranchStatus("EleE277") ) { fChain->SetBranchAddress("EleE277", &EleE277, &b_EleE277); }
  if( fChain->GetBranchStatus("EleEthad1") ) { fChain->SetBranchAddress("EleEthad1", &EleEthad1, &b_EleEthad1); }
  if( fChain->GetBranchStatus("EleWeta1") ) { fChain->SetBranchAddress("EleWeta1", &EleWeta1, &b_EleWeta1); }
  if( fChain->GetBranchStatus("EleWeta2") ) { fChain->SetBranchAddress("EleWeta2", &EleWeta2, &b_EleWeta2); }
  if( fChain->GetBranchStatus("EleF1") ) { fChain->SetBranchAddress("EleF1", &EleF1, &b_EleF1); }
  if( fChain->GetBranchStatus("EleE2tsts1") ) { fChain->SetBranchAddress("EleE2tsts1", &EleE2tsts1, &b_EleE2tsts1); }
  if( fChain->GetBranchStatus("EleEmins1") ) { fChain->SetBranchAddress("EleEmins1", &EleEmins1, &b_EleEmins1); }
  if( fChain->GetBranchStatus("EleWtots1") ) { fChain->SetBranchAddress("EleWtots1", &EleWtots1, &b_EleWtots1); }
  if( fChain->GetBranchStatus("EleFracs1") ) { fChain->SetBranchAddress("EleFracs1", &EleFracs1, &b_EleFracs1); }
  if( fChain->GetBranchStatus("EleSoftEtaCorrMag") ) { fChain->SetBranchAddress("EleSoftEtaCorrMag", &EleSoftEtaCorrMag, &b_EleSoftEtaCorrMag); }
  if( fChain->GetBranchStatus("EleSoftF1Core") ) { fChain->SetBranchAddress("EleSoftF1Core", &EleSoftF1Core, &b_EleSoftF1Core); }
  if( fChain->GetBranchStatus("EleSoftF3Core") ) { fChain->SetBranchAddress("EleSoftF3Core", &EleSoftF3Core, &b_EleSoftF3Core); }
  if( fChain->GetBranchStatus("EleSoftPos7") ) { fChain->SetBranchAddress("EleSoftPos7", &EleSoftPos7, &b_EleSoftPos7); }
  if( fChain->GetBranchStatus("EleSoftIso") ) { fChain->SetBranchAddress("EleSoftIso", &EleSoftIso, &b_EleSoftIso); }
  if( fChain->GetBranchStatus("EleSoftWidths2") ) { fChain->SetBranchAddress("EleSoftWidths2", &EleSoftWidths2, &b_EleSoftWidths2); }
  if( fChain->GetBranchStatus("PhoN") ) { fChain->SetBranchAddress("PhoN", &PhoN, &b_PhoN); }
  if( fChain->GetBranchStatus("PhoAuthor") ) { fChain->SetBranchAddress("PhoAuthor", &PhoAuthor, &b_PhoAuthor); }
  if( fChain->GetBranchStatus("PhoEta") ) { fChain->SetBranchAddress("PhoEta", &PhoEta, &b_PhoEta); }
  if( fChain->GetBranchStatus("PhoPhi") ) { fChain->SetBranchAddress("PhoPhi", &PhoPhi, &b_PhoPhi); }
  if( fChain->GetBranchStatus("PhoPt") ) { fChain->SetBranchAddress("PhoPt", &PhoPt, &b_PhoPt); }
  if( fChain->GetBranchStatus("PhoIsEM") ) { fChain->SetBranchAddress("PhoIsEM", &PhoIsEM, &b_PhoIsEM); }
  if( fChain->GetBranchStatus("PhoEtaBE2") ) { fChain->SetBranchAddress("PhoEtaBE2", &PhoEtaBE2, &b_PhoEtaBE2); }
  if( fChain->GetBranchStatus("PhoEt37") ) { fChain->SetBranchAddress("PhoEt37", &PhoEt37, &b_PhoEt37); }
  if( fChain->GetBranchStatus("PhoE237") ) { fChain->SetBranchAddress("PhoE237", &PhoE237, &b_PhoE237); }
  if( fChain->GetBranchStatus("PhoE277") ) { fChain->SetBranchAddress("PhoE277", &PhoE277, &b_PhoE277); }
  if( fChain->GetBranchStatus("PhoEthad1") ) { fChain->SetBranchAddress("PhoEthad1", &PhoEthad1, &b_PhoEthad1); }
  if( fChain->GetBranchStatus("PhoWeta1") ) { fChain->SetBranchAddress("PhoWeta1", &PhoWeta1, &b_PhoWeta1); }
  if( fChain->GetBranchStatus("PhoWeta2") ) { fChain->SetBranchAddress("PhoWeta2", &PhoWeta2, &b_PhoWeta2); }
  if( fChain->GetBranchStatus("PhoF1") ) { fChain->SetBranchAddress("PhoF1", &PhoF1, &b_PhoF1); }
  if( fChain->GetBranchStatus("PhoE2tsts1") ) { fChain->SetBranchAddress("PhoE2tsts1", &PhoE2tsts1, &b_PhoE2tsts1); }
  if( fChain->GetBranchStatus("PhoEmins1") ) { fChain->SetBranchAddress("PhoEmins1", &PhoEmins1, &b_PhoEmins1); }
  if( fChain->GetBranchStatus("PhoWtots1") ) { fChain->SetBranchAddress("PhoWtots1", &PhoWtots1, &b_PhoWtots1); }
  if( fChain->GetBranchStatus("PhoFracs1") ) { fChain->SetBranchAddress("PhoFracs1", &PhoFracs1, &b_PhoFracs1); }
  if( fChain->GetBranchStatus("MuonN") ) { fChain->SetBranchAddress("MuonN", &MuonN, &b_MuonN); }
  if( fChain->GetBranchStatus("MuonAuthor") ) { fChain->SetBranchAddress("MuonAuthor", &MuonAuthor, &b_MuonAuthor); }
  if( fChain->GetBranchStatus("MuonCombTrkNt") ) { fChain->SetBranchAddress("MuonCombTrkNt", &MuonCombTrkNt, &b_MuonCombTrkNt); }
  if( fChain->GetBranchStatus("MuonInDetTrkNt") ) { fChain->SetBranchAddress("MuonInDetTrkNt", &MuonInDetTrkNt, &b_MuonInDetTrkNt); }
  if( fChain->GetBranchStatus("MuonExtrNt") ) { fChain->SetBranchAddress("MuonExtrNt", &MuonExtrNt, &b_MuonExtrNt); }
  if( fChain->GetBranchStatus("MuonEta") ) { fChain->SetBranchAddress("MuonEta", &MuonEta, &b_MuonEta); }
  if( fChain->GetBranchStatus("MuonPhi") ) { fChain->SetBranchAddress("MuonPhi", &MuonPhi, &b_MuonPhi); }
  if( fChain->GetBranchStatus("MuonPt") ) { fChain->SetBranchAddress("MuonPt", &MuonPt, &b_MuonPt); }
  if( fChain->GetBranchStatus("MuonZ0") ) { fChain->SetBranchAddress("MuonZ0", &MuonZ0, &b_MuonZ0); }
  if( fChain->GetBranchStatus("MuonD0") ) { fChain->SetBranchAddress("MuonD0", &MuonD0, &b_MuonD0); }
  if( fChain->GetBranchStatus("MuonCharge") ) { fChain->SetBranchAddress("MuonCharge", &MuonCharge, &b_MuonCharge); }
  if( fChain->GetBranchStatus("MuonNMDTHits") ) { fChain->SetBranchAddress("MuonNMDTHits", &MuonNMDTHits, &b_MuonNMDTHits); }
  if( fChain->GetBranchStatus("MuonNRPCEtaHits") ) { fChain->SetBranchAddress("MuonNRPCEtaHits", &MuonNRPCEtaHits, &b_MuonNRPCEtaHits); }
  if( fChain->GetBranchStatus("MuonNRPCPhiHits") ) { fChain->SetBranchAddress("MuonNRPCPhiHits", &MuonNRPCPhiHits, &b_MuonNRPCPhiHits); }
  if( fChain->GetBranchStatus("MuonNCSCEtaHits") ) { fChain->SetBranchAddress("MuonNCSCEtaHits", &MuonNCSCEtaHits, &b_MuonNCSCEtaHits); }
  if( fChain->GetBranchStatus("MuonNCSCPhiHits") ) { fChain->SetBranchAddress("MuonNCSCPhiHits", &MuonNCSCPhiHits, &b_MuonNCSCPhiHits); }
  if( fChain->GetBranchStatus("MuonNTGCEtaHits") ) { fChain->SetBranchAddress("MuonNTGCEtaHits", &MuonNTGCEtaHits, &b_MuonNTGCEtaHits); }
  if( fChain->GetBranchStatus("MuonNTGCPhiHits") ) { fChain->SetBranchAddress("MuonNTGCPhiHits", &MuonNTGCPhiHits, &b_MuonNTGCPhiHits); }
  if( fChain->GetBranchStatus("MuonLowPtNMDTHits") ) { fChain->SetBranchAddress("MuonLowPtNMDTHits", &MuonLowPtNMDTHits, &b_MuonLowPtNMDTHits); }
  if( fChain->GetBranchStatus("MuonLowPtNCSCEtaHits") ) { fChain->SetBranchAddress("MuonLowPtNCSCEtaHits", &MuonLowPtNCSCEtaHits, &b_MuonLowPtNCSCEtaHits); }
  if( fChain->GetBranchStatus("MuonLowPtNCSCPhiHits") ) { fChain->SetBranchAddress("MuonLowPtNCSCPhiHits", &MuonLowPtNCSCPhiHits, &b_MuonLowPtNCSCPhiHits); }
  if( fChain->GetBranchStatus("MuonLowPtNRPCEtaHits") ) { fChain->SetBranchAddress("MuonLowPtNRPCEtaHits", &MuonLowPtNRPCEtaHits, &b_MuonLowPtNRPCEtaHits); }
  if( fChain->GetBranchStatus("MuonLowPtNRPCPhiHits") ) { fChain->SetBranchAddress("MuonLowPtNRPCPhiHits", &MuonLowPtNRPCPhiHits, &b_MuonLowPtNRPCPhiHits); }
  if( fChain->GetBranchStatus("MuonLowPtNTGCEtaHits") ) { fChain->SetBranchAddress("MuonLowPtNTGCEtaHits", &MuonLowPtNTGCEtaHits, &b_MuonLowPtNTGCEtaHits); }
  if( fChain->GetBranchStatus("MuonLowPtNTGCPhiHits") ) { fChain->SetBranchAddress("MuonLowPtNTGCPhiHits", &MuonLowPtNTGCPhiHits, &b_MuonLowPtNTGCPhiHits); }
  if( fChain->GetBranchStatus("TauJet_N") ) { fChain->SetBranchAddress("TauJet_N", &TauJet_N, &b_TauJet_N); }
  if( fChain->GetBranchStatus("TauJet_Author") ) { fChain->SetBranchAddress("TauJet_Author", &TauJet_Author, &b_TauJet_Author); }
  if( fChain->GetBranchStatus("TauJet_numTrack") ) { fChain->SetBranchAddress("TauJet_numTrack", &TauJet_numTrack, &b_TauJet_numTrack); }
  if( fChain->GetBranchStatus("TauJet_decayVxNt") ) { fChain->SetBranchAddress("TauJet_decayVxNt", &TauJet_decayVxNt, &b_TauJet_decayVxNt); }
  if( fChain->GetBranchStatus("TauJet_clusterNt") ) { fChain->SetBranchAddress("TauJet_clusterNt", &TauJet_clusterNt, &b_TauJet_clusterNt); }
  if( fChain->GetBranchStatus("TauJet_eta") ) { fChain->SetBranchAddress("TauJet_eta", &TauJet_eta, &b_TauJet_eta); }
  if( fChain->GetBranchStatus("TauJet_phi") ) { fChain->SetBranchAddress("TauJet_phi", &TauJet_phi, &b_TauJet_phi); }
  if( fChain->GetBranchStatus("TauJet_pt") ) { fChain->SetBranchAddress("TauJet_pt", &TauJet_pt, &b_TauJet_pt); }
  if( fChain->GetBranchStatus("TauJet_m") ) { fChain->SetBranchAddress("TauJet_m", &TauJet_m, &b_TauJet_m); }
  if( fChain->GetBranchStatus("TauJet_et") ) { fChain->SetBranchAddress("TauJet_et", &TauJet_et, &b_TauJet_et); }
  if( fChain->GetBranchStatus("TauJet_charge") ) { fChain->SetBranchAddress("TauJet_charge", &TauJet_charge, &b_TauJet_charge); }
  if( fChain->GetBranchStatus("TauJet_emRadius") ) { fChain->SetBranchAddress("TauJet_emRadius", &TauJet_emRadius, &b_TauJet_emRadius); }
  if( fChain->GetBranchStatus("TauJet_stripWidth2") ) { fChain->SetBranchAddress("TauJet_stripWidth2", &TauJet_stripWidth2, &b_TauJet_stripWidth2); }
  if( fChain->GetBranchStatus("TauJet_nStripCells") ) { fChain->SetBranchAddress("TauJet_nStripCells", &TauJet_nStripCells, &b_TauJet_nStripCells); }
  if( fChain->GetBranchStatus("TauJet_NumTauRecParams") ) { fChain->SetBranchAddress("TauJet_NumTauRecParams", &TauJet_NumTauRecParams, &b_TauJet_NumTauRecParams); }
  if( fChain->GetBranchStatus("TauJet_pEM_px") ) { fChain->SetBranchAddress("TauJet_pEM_px", &TauJet_pEM_px, &b_TauJet_pEM_px); }
  if( fChain->GetBranchStatus("TauJet_pEM_py") ) { fChain->SetBranchAddress("TauJet_pEM_py", &TauJet_pEM_py, &b_TauJet_pEM_py); }
  if( fChain->GetBranchStatus("TauJet_pEM_pz") ) { fChain->SetBranchAddress("TauJet_pEM_pz", &TauJet_pEM_pz, &b_TauJet_pEM_pz); }
  if( fChain->GetBranchStatus("TauJet_pEM_e") ) { fChain->SetBranchAddress("TauJet_pEM_e", &TauJet_pEM_e, &b_TauJet_pEM_e); }
  if( fChain->GetBranchStatus("TauJet_etHadCalib") ) { fChain->SetBranchAddress("TauJet_etHadCalib", &TauJet_etHadCalib, &b_TauJet_etHadCalib); }
  if( fChain->GetBranchStatus("TauJet_etEMCalib") ) { fChain->SetBranchAddress("TauJet_etEMCalib", &TauJet_etEMCalib, &b_TauJet_etEMCalib); }
  if( fChain->GetBranchStatus("TauJet_isolationFraction") ) { fChain->SetBranchAddress("TauJet_isolationFraction", &TauJet_isolationFraction, &b_TauJet_isolationFraction); }
  if( fChain->GetBranchStatus("TauJet_centralityFraction") ) { fChain->SetBranchAddress("TauJet_centralityFraction", &TauJet_centralityFraction, &b_TauJet_centralityFraction); }
  if( fChain->GetBranchStatus("TauJet_logLikelihoodRatio") ) { fChain->SetBranchAddress("TauJet_logLikelihoodRatio", &TauJet_logLikelihoodRatio, &b_TauJet_logLikelihoodRatio); }
  if( fChain->GetBranchStatus("TauJet_lowPtTauJetDiscriminant") ) { fChain->SetBranchAddress("TauJet_lowPtTauJetDiscriminant", &TauJet_lowPtTauJetDiscriminant, &b_TauJet_lowPtTauJetDiscriminant); }
  if( fChain->GetBranchStatus("TauJet_lowPtTauEleDiscriminant") ) { fChain->SetBranchAddress("TauJet_lowPtTauEleDiscriminant", &TauJet_lowPtTauEleDiscriminant, &b_TauJet_lowPtTauEleDiscriminant); }
  if( fChain->GetBranchStatus("TauJet_tauJetNeuralnetwork") ) { fChain->SetBranchAddress("TauJet_tauJetNeuralnetwork", &TauJet_tauJetNeuralnetwork, &b_TauJet_tauJetNeuralnetwork); }
  if( fChain->GetBranchStatus("TauJet_tauENeuralNetwork") ) { fChain->SetBranchAddress("TauJet_tauENeuralNetwork", &TauJet_tauENeuralNetwork, &b_TauJet_tauENeuralNetwork); }
  if( fChain->GetBranchStatus("TauJet_TrkNts") ) { fChain->SetBranchAddress("TauJet_TrkNts", &TauJet_TrkNts, &b_TauJet_TrkNts); }
  if( fChain->GetBranchStatus("TauJet1p3p_N") ) { fChain->SetBranchAddress("TauJet1p3p_N", &TauJet1p3p_N, &b_TauJet1p3p_N); }
  if( fChain->GetBranchStatus("TauJet1p3p_Author") ) { fChain->SetBranchAddress("TauJet1p3p_Author", &TauJet1p3p_Author, &b_TauJet1p3p_Author); }
  if( fChain->GetBranchStatus("TauJet1p3p_numTrack") ) { fChain->SetBranchAddress("TauJet1p3p_numTrack", &TauJet1p3p_numTrack, &b_TauJet1p3p_numTrack); }
  if( fChain->GetBranchStatus("TauJet1p3p_decayVxNt") ) { fChain->SetBranchAddress("TauJet1p3p_decayVxNt", &TauJet1p3p_decayVxNt, &b_TauJet1p3p_decayVxNt); }
  if( fChain->GetBranchStatus("TauJet1p3p_clusterNt") ) { fChain->SetBranchAddress("TauJet1p3p_clusterNt", &TauJet1p3p_clusterNt, &b_TauJet1p3p_clusterNt); }
  if( fChain->GetBranchStatus("TauJet1p3p_eta") ) { fChain->SetBranchAddress("TauJet1p3p_eta", &TauJet1p3p_eta, &b_TauJet1p3p_eta); }
  if( fChain->GetBranchStatus("TauJet1p3p_phi") ) { fChain->SetBranchAddress("TauJet1p3p_phi", &TauJet1p3p_phi, &b_TauJet1p3p_phi); }
  if( fChain->GetBranchStatus("TauJet1p3p_pt") ) { fChain->SetBranchAddress("TauJet1p3p_pt", &TauJet1p3p_pt, &b_TauJet1p3p_pt); }
  if( fChain->GetBranchStatus("TauJet1p3p_m") ) { fChain->SetBranchAddress("TauJet1p3p_m", &TauJet1p3p_m, &b_TauJet1p3p_m); }
  if( fChain->GetBranchStatus("TauJet1p3p_et") ) { fChain->SetBranchAddress("TauJet1p3p_et", &TauJet1p3p_et, &b_TauJet1p3p_et); }
  if( fChain->GetBranchStatus("TauJet1p3p_charge") ) { fChain->SetBranchAddress("TauJet1p3p_charge", &TauJet1p3p_charge, &b_TauJet1p3p_charge); }
  if( fChain->GetBranchStatus("TauJet1p3p_emRadius") ) { fChain->SetBranchAddress("TauJet1p3p_emRadius", &TauJet1p3p_emRadius, &b_TauJet1p3p_emRadius); }
  if( fChain->GetBranchStatus("TauJet1p3p_stripWidth2") ) { fChain->SetBranchAddress("TauJet1p3p_stripWidth2", &TauJet1p3p_stripWidth2, &b_TauJet1p3p_stripWidth2); }
  if( fChain->GetBranchStatus("TauJet1p3p_nStripCells") ) { fChain->SetBranchAddress("TauJet1p3p_nStripCells", &TauJet1p3p_nStripCells, &b_TauJet1p3p_nStripCells); }
  if( fChain->GetBranchStatus("TauJet1p3p_NumTau1p3pParams") ) { fChain->SetBranchAddress("TauJet1p3p_NumTau1p3pParams", &TauJet1p3p_NumTau1p3pParams, &b_TauJet1p3p_NumTau1p3pParams); }
  if( fChain->GetBranchStatus("TauJet1p3p_annularIsolationFraction") ) { fChain->SetBranchAddress("TauJet1p3p_annularIsolationFraction", &TauJet1p3p_annularIsolationFraction, &b_TauJet1p3p_annularIsolationFraction); }
  if( fChain->GetBranchStatus("TauJet1p3p_etCaloAtEMScale") ) { fChain->SetBranchAddress("TauJet1p3p_etCaloAtEMScale", &TauJet1p3p_etCaloAtEMScale, &b_TauJet1p3p_etCaloAtEMScale); }
  if( fChain->GetBranchStatus("TauJet1p3p_etChargedHadCells") ) { fChain->SetBranchAddress("TauJet1p3p_etChargedHadCells", &TauJet1p3p_etChargedHadCells, &b_TauJet1p3p_etChargedHadCells); }
  if( fChain->GetBranchStatus("TauJet1p3p_etOtherEMCells") ) { fChain->SetBranchAddress("TauJet1p3p_etOtherEMCells", &TauJet1p3p_etOtherEMCells, &b_TauJet1p3p_etOtherEMCells); }
  if( fChain->GetBranchStatus("TauJet1p3p_etOtherHadCells") ) { fChain->SetBranchAddress("TauJet1p3p_etOtherHadCells", &TauJet1p3p_etOtherHadCells, &b_TauJet1p3p_etOtherHadCells); }
  if( fChain->GetBranchStatus("TauJet1p3p_discriminant") ) { fChain->SetBranchAddress("TauJet1p3p_discriminant", &TauJet1p3p_discriminant, &b_TauJet1p3p_discriminant); }
  if( fChain->GetBranchStatus("TauJet1p3p_discPDERS") ) { fChain->SetBranchAddress("TauJet1p3p_discPDERS", &TauJet1p3p_discPDERS, &b_TauJet1p3p_discPDERS); }
  if( fChain->GetBranchStatus("TauJet1p3p_discNN") ) { fChain->SetBranchAddress("TauJet1p3p_discNN", &TauJet1p3p_discNN, &b_TauJet1p3p_discNN); }
  if( fChain->GetBranchStatus("TauJet1p3p_nAssocTracksCore") ) { fChain->SetBranchAddress("TauJet1p3p_nAssocTracksCore", &TauJet1p3p_nAssocTracksCore, &b_TauJet1p3p_nAssocTracksCore); }
  if( fChain->GetBranchStatus("TauJet1p3p_nAssocTracksIsol") ) { fChain->SetBranchAddress("TauJet1p3p_nAssocTracksIsol", &TauJet1p3p_nAssocTracksIsol, &b_TauJet1p3p_nAssocTracksIsol); }
  if( fChain->GetBranchStatus("TauJet1p3p_sumPTtrack") ) { fChain->SetBranchAddress("TauJet1p3p_sumPTtrack", &TauJet1p3p_sumPTtrack, &b_TauJet1p3p_sumPTtrack); }
  if( fChain->GetBranchStatus("TauJet1p3p_rWidth2Trk3P") ) { fChain->SetBranchAddress("TauJet1p3p_rWidth2Trk3P", &TauJet1p3p_rWidth2Trk3P, &b_TauJet1p3p_rWidth2Trk3P); }
  if( fChain->GetBranchStatus("TauJet1p3p_massTrk3P") ) { fChain->SetBranchAddress("TauJet1p3p_massTrk3P", &TauJet1p3p_massTrk3P, &b_TauJet1p3p_massTrk3P); }
  if( fChain->GetBranchStatus("TauJet1p3p_TrkNts") ) { fChain->SetBranchAddress("TauJet1p3p_TrkNts", &TauJet1p3p_TrkNts, &b_TauJet1p3p_TrkNts); }
  if( fChain->GetBranchStatus("CTPWord0") ) { fChain->SetBranchAddress("CTPWord0", &CTPWord0, &b_CTPWord0); }
  if( fChain->GetBranchStatus("CTPWord1") ) { fChain->SetBranchAddress("CTPWord1", &CTPWord1, &b_CTPWord1); }
  if( fChain->GetBranchStatus("CTPWord2") ) { fChain->SetBranchAddress("CTPWord2", &CTPWord2, &b_CTPWord2); }
  if( fChain->GetBranchStatus("MuROI_N") ) { fChain->SetBranchAddress("MuROI_N", &MuROI_N, &b_MuROI_N); }
  if( fChain->GetBranchStatus("MuROI_Eta") ) { fChain->SetBranchAddress("MuROI_Eta", &MuROI_Eta, &b_MuROI_Eta); }
  if( fChain->GetBranchStatus("MuROI_Phi") ) { fChain->SetBranchAddress("MuROI_Phi", &MuROI_Phi, &b_MuROI_Phi); }
  if( fChain->GetBranchStatus("MuROI_Thr") ) { fChain->SetBranchAddress("MuROI_Thr", &MuROI_Thr, &b_MuROI_Thr); }
  if( fChain->GetBranchStatus("EmROI_N") ) { fChain->SetBranchAddress("EmROI_N", &EmROI_N, &b_EmROI_N); }
  if( fChain->GetBranchStatus("EmROI_Eta") ) { fChain->SetBranchAddress("EmROI_Eta", &EmROI_Eta, &b_EmROI_Eta); }
  if( fChain->GetBranchStatus("EmROI_Phi") ) { fChain->SetBranchAddress("EmROI_Phi", &EmROI_Phi, &b_EmROI_Phi); }
  if( fChain->GetBranchStatus("EmROI_Thr") ) { fChain->SetBranchAddress("EmROI_Thr", &EmROI_Thr, &b_EmROI_Thr); }
  if( fChain->GetBranchStatus("JetROI_N") ) { fChain->SetBranchAddress("JetROI_N", &JetROI_N, &b_JetROI_N); }
  if( fChain->GetBranchStatus("JetROI_Eta") ) { fChain->SetBranchAddress("JetROI_Eta", &JetROI_Eta, &b_JetROI_Eta); }
  if( fChain->GetBranchStatus("JetROI_Phi") ) { fChain->SetBranchAddress("JetROI_Phi", &JetROI_Phi, &b_JetROI_Phi); }
  if( fChain->GetBranchStatus("JetROI_Thr") ) { fChain->SetBranchAddress("JetROI_Thr", &JetROI_Thr, &b_JetROI_Thr); }
  if( fChain->GetBranchStatus("JetEtROI_N") ) { fChain->SetBranchAddress("JetEtROI_N", &JetEtROI_N, &b_JetEtROI_N); }
  if( fChain->GetBranchStatus("JetEtROI_Word") ) { fChain->SetBranchAddress("JetEtROI_Word", &JetEtROI_Word, &b_JetEtROI_Word); }
  if( fChain->GetBranchStatus("JetEtROI_Thr") ) { fChain->SetBranchAddress("JetEtROI_Thr", &JetEtROI_Thr, &b_JetEtROI_Thr); }
  if( fChain->GetBranchStatus("ESumROI_N") ) { fChain->SetBranchAddress("ESumROI_N", &ESumROI_N, &b_ESumROI_N); }
  if( fChain->GetBranchStatus("ESumROI_Ex") ) { fChain->SetBranchAddress("ESumROI_Ex", &ESumROI_Ex, &b_ESumROI_Ex); }
  if( fChain->GetBranchStatus("ESumROI_Ey") ) { fChain->SetBranchAddress("ESumROI_Ey", &ESumROI_Ey, &b_ESumROI_Ey); }
  if( fChain->GetBranchStatus("ESumROI_Et") ) { fChain->SetBranchAddress("ESumROI_Et", &ESumROI_Et, &b_ESumROI_Et); }
  if( fChain->GetBranchStatus("ESumROI_ThrS") ) { fChain->SetBranchAddress("ESumROI_ThrS", &ESumROI_ThrS, &b_ESumROI_ThrS); }
  if( fChain->GetBranchStatus("ESumROI_ThrM") ) { fChain->SetBranchAddress("ESumROI_ThrM", &ESumROI_ThrM, &b_ESumROI_ThrM); }
  if( fChain->GetBranchStatus("cl_ecluster_topo") ) { fChain->SetBranchAddress("cl_ecluster_topo", &cl_ecluster_topo, &b_cl_ecluster_topo); }
  if( fChain->GetBranchStatus("cl_nctotal_topo") ) { fChain->SetBranchAddress("cl_nctotal_topo", &cl_nctotal_topo, &b_cl_nctotal_topo); }
  if( fChain->GetBranchStatus("cl_nc_topo") ) { fChain->SetBranchAddress("cl_nc_topo", &cl_nc_topo, &b_cl_nc_topo); }
  if( fChain->GetBranchStatus("cl_et_topo") ) { fChain->SetBranchAddress("cl_et_topo", &cl_et_topo, &b_cl_et_topo); }
  if( fChain->GetBranchStatus("cl_e_topo") ) { fChain->SetBranchAddress("cl_e_topo", &cl_e_topo, &b_cl_e_topo); }
  if( fChain->GetBranchStatus("cl_eemb0_topo") ) { fChain->SetBranchAddress("cl_eemb0_topo", &cl_eemb0_topo, &b_cl_eemb0_topo); }
  if( fChain->GetBranchStatus("cl_eemb1_topo") ) { fChain->SetBranchAddress("cl_eemb1_topo", &cl_eemb1_topo, &b_cl_eemb1_topo); }
  if( fChain->GetBranchStatus("cl_eemb2_topo") ) { fChain->SetBranchAddress("cl_eemb2_topo", &cl_eemb2_topo, &b_cl_eemb2_topo); }
  if( fChain->GetBranchStatus("cl_eemb3_topo") ) { fChain->SetBranchAddress("cl_eemb3_topo", &cl_eemb3_topo, &b_cl_eemb3_topo); }
  if( fChain->GetBranchStatus("cl_nemb0_topo") ) { fChain->SetBranchAddress("cl_nemb0_topo", &cl_nemb0_topo, &b_cl_nemb0_topo); }
  if( fChain->GetBranchStatus("cl_nemb1_topo") ) { fChain->SetBranchAddress("cl_nemb1_topo", &cl_nemb1_topo, &b_cl_nemb1_topo); }
  if( fChain->GetBranchStatus("cl_nemb2_topo") ) { fChain->SetBranchAddress("cl_nemb2_topo", &cl_nemb2_topo, &b_cl_nemb2_topo); }
  if( fChain->GetBranchStatus("cl_nemb3_topo") ) { fChain->SetBranchAddress("cl_nemb3_topo", &cl_nemb3_topo, &b_cl_nemb3_topo); }
  if( fChain->GetBranchStatus("cl_phi2_topo") ) { fChain->SetBranchAddress("cl_phi2_topo", &cl_phi2_topo, &b_cl_phi2_topo); }
  if( fChain->GetBranchStatus("cl_eta0_topo") ) { fChain->SetBranchAddress("cl_eta0_topo", &cl_eta0_topo, &b_cl_eta0_topo); }
  if( fChain->GetBranchStatus("cl_eta1_topo") ) { fChain->SetBranchAddress("cl_eta1_topo", &cl_eta1_topo, &b_cl_eta1_topo); }
  if( fChain->GetBranchStatus("cl_eta2_topo") ) { fChain->SetBranchAddress("cl_eta2_topo", &cl_eta2_topo, &b_cl_eta2_topo); }
  if( fChain->GetBranchStatus("cl_eta3_topo") ) { fChain->SetBranchAddress("cl_eta3_topo", &cl_eta3_topo, &b_cl_eta3_topo); }
  if( fChain->GetBranchStatus("cl_eeme0_topo") ) { fChain->SetBranchAddress("cl_eeme0_topo", &cl_eeme0_topo, &b_cl_eeme0_topo); }
  if( fChain->GetBranchStatus("cl_eeme1_topo") ) { fChain->SetBranchAddress("cl_eeme1_topo", &cl_eeme1_topo, &b_cl_eeme1_topo); }
  if( fChain->GetBranchStatus("cl_eeme2_topo") ) { fChain->SetBranchAddress("cl_eeme2_topo", &cl_eeme2_topo, &b_cl_eeme2_topo); }
  if( fChain->GetBranchStatus("cl_eeme3_topo") ) { fChain->SetBranchAddress("cl_eeme3_topo", &cl_eeme3_topo, &b_cl_eeme3_topo); }
  if( fChain->GetBranchStatus("cl_neme0_topo") ) { fChain->SetBranchAddress("cl_neme0_topo", &cl_neme0_topo, &b_cl_neme0_topo); }
  if( fChain->GetBranchStatus("cl_neme1_topo") ) { fChain->SetBranchAddress("cl_neme1_topo", &cl_neme1_topo, &b_cl_neme1_topo); }
  if( fChain->GetBranchStatus("cl_neme2_topo") ) { fChain->SetBranchAddress("cl_neme2_topo", &cl_neme2_topo, &b_cl_neme2_topo); }
  if( fChain->GetBranchStatus("cl_neme3_topo") ) { fChain->SetBranchAddress("cl_neme3_topo", &cl_neme3_topo, &b_cl_neme3_topo); }
  if( fChain->GetBranchStatus("cl_etileg1_topo") ) { fChain->SetBranchAddress("cl_etileg1_topo", &cl_etileg1_topo, &b_cl_etileg1_topo); }
  if( fChain->GetBranchStatus("cl_etileg2_topo") ) { fChain->SetBranchAddress("cl_etileg2_topo", &cl_etileg2_topo, &b_cl_etileg2_topo); }
  if( fChain->GetBranchStatus("cl_etileg3_topo") ) { fChain->SetBranchAddress("cl_etileg3_topo", &cl_etileg3_topo, &b_cl_etileg3_topo); }
  if( fChain->GetBranchStatus("cl_ntileg1_topo") ) { fChain->SetBranchAddress("cl_ntileg1_topo", &cl_ntileg1_topo, &b_cl_ntileg1_topo); }
  if( fChain->GetBranchStatus("cl_ntileg2_topo") ) { fChain->SetBranchAddress("cl_ntileg2_topo", &cl_ntileg2_topo, &b_cl_ntileg2_topo); }
  if( fChain->GetBranchStatus("cl_ntileg3_topo") ) { fChain->SetBranchAddress("cl_ntileg3_topo", &cl_ntileg3_topo, &b_cl_ntileg3_topo); }
  if( fChain->GetBranchStatus("cl_eta_topo") ) { fChain->SetBranchAddress("cl_eta_topo", &cl_eta_topo, &b_cl_eta_topo); }
  if( fChain->GetBranchStatus("cl_phi_topo") ) { fChain->SetBranchAddress("cl_phi_topo", &cl_phi_topo, &b_cl_phi_topo); }
  if( fChain->GetBranchStatus("cl_time_topo") ) { fChain->SetBranchAddress("cl_time_topo", &cl_time_topo, &b_cl_time_topo); }
  if( fChain->GetBranchStatus("cl_reco_stat_topo") ) { fChain->SetBranchAddress("cl_reco_stat_topo", &cl_reco_stat_topo, &b_cl_reco_stat_topo); }
  if( fChain->GetBranchStatus("cl_m1_eta_topo") ) { fChain->SetBranchAddress("cl_m1_eta_topo", &cl_m1_eta_topo, &b_cl_m1_eta_topo); }
  if( fChain->GetBranchStatus("cl_m1_phi_topo") ) { fChain->SetBranchAddress("cl_m1_phi_topo", &cl_m1_phi_topo, &b_cl_m1_phi_topo); }
  if( fChain->GetBranchStatus("cl_m2_r_topo") ) { fChain->SetBranchAddress("cl_m2_r_topo", &cl_m2_r_topo, &b_cl_m2_r_topo); }
  if( fChain->GetBranchStatus("cl_m2_lambda_topo") ) { fChain->SetBranchAddress("cl_m2_lambda_topo", &cl_m2_lambda_topo, &b_cl_m2_lambda_topo); }
  if( fChain->GetBranchStatus("cl_delta_phi_topo") ) { fChain->SetBranchAddress("cl_delta_phi_topo", &cl_delta_phi_topo, &b_cl_delta_phi_topo); }
  if( fChain->GetBranchStatus("cl_delta_theta_topo") ) { fChain->SetBranchAddress("cl_delta_theta_topo", &cl_delta_theta_topo, &b_cl_delta_theta_topo); }
  if( fChain->GetBranchStatus("cl_delta_alpha_topo") ) { fChain->SetBranchAddress("cl_delta_alpha_topo", &cl_delta_alpha_topo, &b_cl_delta_alpha_topo); }
  if( fChain->GetBranchStatus("cl_center_x_topo") ) { fChain->SetBranchAddress("cl_center_x_topo", &cl_center_x_topo, &b_cl_center_x_topo); }
  if( fChain->GetBranchStatus("cl_center_y_topo") ) { fChain->SetBranchAddress("cl_center_y_topo", &cl_center_y_topo, &b_cl_center_y_topo); }
  if( fChain->GetBranchStatus("cl_center_z_topo") ) { fChain->SetBranchAddress("cl_center_z_topo", &cl_center_z_topo, &b_cl_center_z_topo); }
  if( fChain->GetBranchStatus("cl_center_lambda_topo") ) { fChain->SetBranchAddress("cl_center_lambda_topo", &cl_center_lambda_topo, &b_cl_center_lambda_topo); }
  if( fChain->GetBranchStatus("cl_lateral_topo") ) { fChain->SetBranchAddress("cl_lateral_topo", &cl_lateral_topo, &b_cl_lateral_topo); }
  if( fChain->GetBranchStatus("cl_longitudinal_topo") ) { fChain->SetBranchAddress("cl_longitudinal_topo", &cl_longitudinal_topo, &b_cl_longitudinal_topo); }
  if( fChain->GetBranchStatus("cl_eng_frac_em_topo") ) { fChain->SetBranchAddress("cl_eng_frac_em_topo", &cl_eng_frac_em_topo, &b_cl_eng_frac_em_topo); }
  if( fChain->GetBranchStatus("cl_eng_frac_max_topo") ) { fChain->SetBranchAddress("cl_eng_frac_max_topo", &cl_eng_frac_max_topo, &b_cl_eng_frac_max_topo); }
  if( fChain->GetBranchStatus("cl_eng_frac_core_topo") ) { fChain->SetBranchAddress("cl_eng_frac_core_topo", &cl_eng_frac_core_topo, &b_cl_eng_frac_core_topo); }
  if( fChain->GetBranchStatus("cl_m1_dens_topo") ) { fChain->SetBranchAddress("cl_m1_dens_topo", &cl_m1_dens_topo, &b_cl_m1_dens_topo); }
  if( fChain->GetBranchStatus("cl_m2_dens_topo") ) { fChain->SetBranchAddress("cl_m2_dens_topo", &cl_m2_dens_topo, &b_cl_m2_dens_topo); }
  if( fChain->GetBranchStatus("cl_ehec0_topo") ) { fChain->SetBranchAddress("cl_ehec0_topo", &cl_ehec0_topo, &b_cl_ehec0_topo); }
  if( fChain->GetBranchStatus("cl_ehec1_topo") ) { fChain->SetBranchAddress("cl_ehec1_topo", &cl_ehec1_topo, &b_cl_ehec1_topo); }
  if( fChain->GetBranchStatus("cl_ehec2_topo") ) { fChain->SetBranchAddress("cl_ehec2_topo", &cl_ehec2_topo, &b_cl_ehec2_topo); }
  if( fChain->GetBranchStatus("cl_ehec3_topo") ) { fChain->SetBranchAddress("cl_ehec3_topo", &cl_ehec3_topo, &b_cl_ehec3_topo); }
  if( fChain->GetBranchStatus("cl_nhec0_topo") ) { fChain->SetBranchAddress("cl_nhec0_topo", &cl_nhec0_topo, &b_cl_nhec0_topo); }
  if( fChain->GetBranchStatus("cl_nhec1_topo") ) { fChain->SetBranchAddress("cl_nhec1_topo", &cl_nhec1_topo, &b_cl_nhec1_topo); }
  if( fChain->GetBranchStatus("cl_nhec2_topo") ) { fChain->SetBranchAddress("cl_nhec2_topo", &cl_nhec2_topo, &b_cl_nhec2_topo); }
  if( fChain->GetBranchStatus("cl_nhec3_topo") ) { fChain->SetBranchAddress("cl_nhec3_topo", &cl_nhec3_topo, &b_cl_nhec3_topo); }
  if( fChain->GetBranchStatus("cl_etileb0_topo") ) { fChain->SetBranchAddress("cl_etileb0_topo", &cl_etileb0_topo, &b_cl_etileb0_topo); }
  if( fChain->GetBranchStatus("cl_etileb1_topo") ) { fChain->SetBranchAddress("cl_etileb1_topo", &cl_etileb1_topo, &b_cl_etileb1_topo); }
  if( fChain->GetBranchStatus("cl_etileb2_topo") ) { fChain->SetBranchAddress("cl_etileb2_topo", &cl_etileb2_topo, &b_cl_etileb2_topo); }
  if( fChain->GetBranchStatus("cl_ntileb0_topo") ) { fChain->SetBranchAddress("cl_ntileb0_topo", &cl_ntileb0_topo, &b_cl_ntileb0_topo); }
  if( fChain->GetBranchStatus("cl_ntileb1_topo") ) { fChain->SetBranchAddress("cl_ntileb1_topo", &cl_ntileb1_topo, &b_cl_ntileb1_topo); }
  if( fChain->GetBranchStatus("cl_ntileb2_topo") ) { fChain->SetBranchAddress("cl_ntileb2_topo", &cl_ntileb2_topo, &b_cl_ntileb2_topo); }
  if( fChain->GetBranchStatus("cl_etilee0_topo") ) { fChain->SetBranchAddress("cl_etilee0_topo", &cl_etilee0_topo, &b_cl_etilee0_topo); }
  if( fChain->GetBranchStatus("cl_etilee1_topo") ) { fChain->SetBranchAddress("cl_etilee1_topo", &cl_etilee1_topo, &b_cl_etilee1_topo); }
  if( fChain->GetBranchStatus("cl_etilee2_topo") ) { fChain->SetBranchAddress("cl_etilee2_topo", &cl_etilee2_topo, &b_cl_etilee2_topo); }
  if( fChain->GetBranchStatus("cl_efcal0_topo") ) { fChain->SetBranchAddress("cl_efcal0_topo", &cl_efcal0_topo, &b_cl_efcal0_topo); }
  if( fChain->GetBranchStatus("cl_efcal1_topo") ) { fChain->SetBranchAddress("cl_efcal1_topo", &cl_efcal1_topo, &b_cl_efcal1_topo); }
  if( fChain->GetBranchStatus("cl_efcal2_topo") ) { fChain->SetBranchAddress("cl_efcal2_topo", &cl_efcal2_topo, &b_cl_efcal2_topo); }
  if( fChain->GetBranchStatus("cl_ntilee0_topo") ) { fChain->SetBranchAddress("cl_ntilee0_topo", &cl_ntilee0_topo, &b_cl_ntilee0_topo); }
  if( fChain->GetBranchStatus("cl_ntilee1_topo") ) { fChain->SetBranchAddress("cl_ntilee1_topo", &cl_ntilee1_topo, &b_cl_ntilee1_topo); }
  if( fChain->GetBranchStatus("cl_ntilee2_topo") ) { fChain->SetBranchAddress("cl_ntilee2_topo", &cl_ntilee2_topo, &b_cl_ntilee2_topo); }
  if( fChain->GetBranchStatus("cl_nfcal0_topo") ) { fChain->SetBranchAddress("cl_nfcal0_topo", &cl_nfcal0_topo, &b_cl_nfcal0_topo); }
  if( fChain->GetBranchStatus("cl_nfcal1_topo") ) { fChain->SetBranchAddress("cl_nfcal1_topo", &cl_nfcal1_topo, &b_cl_nfcal1_topo); }
  if( fChain->GetBranchStatus("cl_nfcal2_topo") ) { fChain->SetBranchAddress("cl_nfcal2_topo", &cl_nfcal2_topo, &b_cl_nfcal2_topo); }
  if( fChain->GetBranchStatus("cl_ntotcells_topo") ) { fChain->SetBranchAddress("cl_ntotcells_topo", &cl_ntotcells_topo, &b_cl_ntotcells_topo); }
  if( fChain->GetBranchStatus("nPJ") ) { fChain->SetBranchAddress("nPJ", &nPJ, &b_nPJ); }
  if( fChain->GetBranchStatus("pjAuthor") ) { fChain->SetBranchAddress("pjAuthor", &pjAuthor, &b_pjAuthor); }
  if( fChain->GetBranchStatus("pjPx") ) { fChain->SetBranchAddress("pjPx", &pjPx, &b_pjPx); }
  if( fChain->GetBranchStatus("pjPy") ) { fChain->SetBranchAddress("pjPy", &pjPy, &b_pjPy); }
  if( fChain->GetBranchStatus("pjPz") ) { fChain->SetBranchAddress("pjPz", &pjPz, &b_pjPz); }
  if( fChain->GetBranchStatus("pjEne") ) { fChain->SetBranchAddress("pjEne", &pjEne, &b_pjEne); }
  if( fChain->GetBranchStatus("pjEta") ) { fChain->SetBranchAddress("pjEta", &pjEta, &b_pjEta); }
  if( fChain->GetBranchStatus("pjPhi") ) { fChain->SetBranchAddress("pjPhi", &pjPhi, &b_pjPhi); }
  if( fChain->GetBranchStatus("pjPt") ) { fChain->SetBranchAddress("pjPt", &pjPt, &b_pjPt); }
  if( fChain->GetBranchStatus("pjNtrk") ) { fChain->SetBranchAddress("pjNtrk", &pjNtrk, &b_pjNtrk); }
  if( fChain->GetBranchStatus("pjNmuo") ) { fChain->SetBranchAddress("pjNmuo", &pjNmuo, &b_pjNmuo); }
  if( fChain->GetBranchStatus("pjNele") ) { fChain->SetBranchAddress("pjNele", &pjNele, &b_pjNele); }
  if( fChain->GetBranchStatus("pjWeight") ) { fChain->SetBranchAddress("pjWeight", &pjWeight, &b_pjWeight); }
  if( fChain->GetBranchStatus("pjW2D") ) { fChain->SetBranchAddress("pjW2D", &pjW2D, &b_pjW2D); }
  if( fChain->GetBranchStatus("pjW3D") ) { fChain->SetBranchAddress("pjW3D", &pjW3D, &b_pjW3D); }
  if( fChain->GetBranchStatus("pjWSV2") ) { fChain->SetBranchAddress("pjWSV2", &pjWSV2, &b_pjWSV2); }
  if( fChain->GetBranchStatus("pjWlhSig") ) { fChain->SetBranchAddress("pjWlhSig", &pjWlhSig, &b_pjWlhSig); }
  if( fChain->GetBranchStatus("pjLabel") ) { fChain->SetBranchAddress("pjLabel", &pjLabel, &b_pjLabel); }
  if( fChain->GetBranchStatus("pjdRb") ) { fChain->SetBranchAddress("pjdRb", &pjdRb, &b_pjdRb); }
  if( fChain->GetBranchStatus("pjdRc") ) { fChain->SetBranchAddress("pjdRc", &pjdRc, &b_pjdRc); }
  if( fChain->GetBranchStatus("pjdRt") ) { fChain->SetBranchAddress("pjdRt", &pjdRt, &b_pjdRt); }
  if( fChain->GetBranchStatus("MemRSS") ) { fChain->SetBranchAddress("MemRSS", &MemRSS, &b_MemRSS); }
  if( fChain->GetBranchStatus("MemVSize") ) { fChain->SetBranchAddress("MemVSize", &MemVSize, &b_MemVSize); }
  if( fChain->GetBranchStatus("TimeTotal") ) { fChain->SetBranchAddress("TimeTotal", &TimeTotal, &b_TimeTotal); }
  if( fChain->GetBranchStatus("TimeSum") ) { fChain->SetBranchAddress("TimeSum", &TimeSum, &b_TimeSum); }
  if( fChain->GetBranchStatus("TimeEventCounter") ) { fChain->SetBranchAddress("TimeEventCounter", &TimeEventCounter, &b_TimeEventCounter); }
  if( fChain->GetBranchStatus("TimePixelRegionSelectorTable") ) { fChain->SetBranchAddress("TimePixelRegionSelectorTable", &TimePixelRegionSelectorTable, &b_TimePixelRegionSelectorTable); }
  if( fChain->GetBranchStatus("TimeSCT_RegionSelectorTable") ) { fChain->SetBranchAddress("TimeSCT_RegionSelectorTable", &TimeSCT_RegionSelectorTable, &b_TimeSCT_RegionSelectorTable); }
  if( fChain->GetBranchStatus("TimeTRT_RegionSelectorTable") ) { fChain->SetBranchAddress("TimeTRT_RegionSelectorTable", &TimeTRT_RegionSelectorTable, &b_TimeTRT_RegionSelectorTable); }
  if( fChain->GetBranchStatus("TimePixelClusterization") ) { fChain->SetBranchAddress("TimePixelClusterization", &TimePixelClusterization, &b_TimePixelClusterization); }
  if( fChain->GetBranchStatus("TimeSCT_Clusterization") ) { fChain->SetBranchAddress("TimeSCT_Clusterization", &TimeSCT_Clusterization, &b_TimeSCT_Clusterization); }
  if( fChain->GetBranchStatus("TimeTRT_RIO_Maker") ) { fChain->SetBranchAddress("TimeTRT_RIO_Maker", &TimeTRT_RIO_Maker, &b_TimeTRT_RIO_Maker); }
  if( fChain->GetBranchStatus("TimePRD_MultiTruthMaker") ) { fChain->SetBranchAddress("TimePRD_MultiTruthMaker", &TimePRD_MultiTruthMaker, &b_TimePRD_MultiTruthMaker); }
  if( fChain->GetBranchStatus("TimeSiTrackerSpacePointFinder") ) { fChain->SetBranchAddress("TimeSiTrackerSpacePointFinder", &TimeSiTrackerSpacePointFinder, &b_TimeSiTrackerSpacePointFinder); }
  if( fChain->GetBranchStatus("TimeiPatRec") ) { fChain->SetBranchAddress("TimeiPatRec", &TimeiPatRec, &b_TimeiPatRec); }
  if( fChain->GetBranchStatus("TimeiPatTrackTruthAssociator") ) { fChain->SetBranchAddress("TimeiPatTrackTruthAssociator", &TimeiPatTrackTruthAssociator, &b_TimeiPatTrackTruthAssociator); }
  if( fChain->GetBranchStatus("TimeiPatStatistics") ) { fChain->SetBranchAddress("TimeiPatStatistics", &TimeiPatStatistics, &b_TimeiPatStatistics); }
  if( fChain->GetBranchStatus("TimeXKaSeedsManager") ) { fChain->SetBranchAddress("TimeXKaSeedsManager", &TimeXKaSeedsManager, &b_TimeXKaSeedsManager); }
  if( fChain->GetBranchStatus("TimeXKaMField") ) { fChain->SetBranchAddress("TimeXKaMField", &TimeXKaMField, &b_TimeXKaMField); }
  if( fChain->GetBranchStatus("TimeXKaDetectorBuilder") ) { fChain->SetBranchAddress("TimeXKaDetectorBuilder", &TimeXKaDetectorBuilder, &b_TimeXKaDetectorBuilder); }
  if( fChain->GetBranchStatus("TimeXKalMan") ) { fChain->SetBranchAddress("TimeXKalMan", &TimeXKalMan, &b_TimeXKalMan); }
  if( fChain->GetBranchStatus("TimeInDetLegacyCnvAlg") ) { fChain->SetBranchAddress("TimeInDetLegacyCnvAlg", &TimeInDetLegacyCnvAlg, &b_TimeInDetLegacyCnvAlg); }
  if( fChain->GetBranchStatus("TimeConvertedXKalmanTracksDetailedTruthMaker") ) { fChain->SetBranchAddress("TimeConvertedXKalmanTracksDetailedTruthMaker", &TimeConvertedXKalmanTracksDetailedTruthMaker, &b_TimeConvertedXKalmanTracksDetailedTruthMaker); }
  if( fChain->GetBranchStatus("TimeConvertedXKalmanTracksTruthSelector") ) { fChain->SetBranchAddress("TimeConvertedXKalmanTracksTruthSelector", &TimeConvertedXKalmanTracksTruthSelector, &b_TimeConvertedXKalmanTracksTruthSelector); }
  if( fChain->GetBranchStatus("TimeConvertedIPatTracksDetailedTruthMaker") ) { fChain->SetBranchAddress("TimeConvertedIPatTracksDetailedTruthMaker", &TimeConvertedIPatTracksDetailedTruthMaker, &b_TimeConvertedIPatTracksDetailedTruthMaker); }
  if( fChain->GetBranchStatus("TimeConvertedIPatTracksTruthSelector") ) { fChain->SetBranchAddress("TimeConvertedIPatTracksTruthSelector", &TimeConvertedIPatTracksTruthSelector, &b_TimeConvertedIPatTracksTruthSelector); }
  if( fChain->GetBranchStatus("TimeInDetSiSPTrackFinder") ) { fChain->SetBranchAddress("TimeInDetSiSPTrackFinder", &TimeInDetSiSPTrackFinder, &b_TimeInDetSiSPTrackFinder); }
  if( fChain->GetBranchStatus("TimeSiSPSeededTracksDetailedTruthMaker") ) { fChain->SetBranchAddress("TimeSiSPSeededTracksDetailedTruthMaker", &TimeSiSPSeededTracksDetailedTruthMaker, &b_TimeSiSPSeededTracksDetailedTruthMaker); }
  if( fChain->GetBranchStatus("TimeSiSPSeededTracksTruthCollectionSelector") ) { fChain->SetBranchAddress("TimeSiSPSeededTracksTruthCollectionSelector", &TimeSiSPSeededTracksTruthCollectionSelector, &b_TimeSiSPSeededTracksTruthCollectionSelector); }
  if( fChain->GetBranchStatus("TimeInDetAmbiguitySolver") ) { fChain->SetBranchAddress("TimeInDetAmbiguitySolver", &TimeInDetAmbiguitySolver, &b_TimeInDetAmbiguitySolver); }
  if( fChain->GetBranchStatus("TimeResolvedTracksDetailedTruthMaker") ) { fChain->SetBranchAddress("TimeResolvedTracksDetailedTruthMaker", &TimeResolvedTracksDetailedTruthMaker, &b_TimeResolvedTracksDetailedTruthMaker); }
  if( fChain->GetBranchStatus("TimeResolvedTracksTruthCollectionSelector") ) { fChain->SetBranchAddress("TimeResolvedTracksTruthCollectionSelector", &TimeResolvedTracksTruthCollectionSelector, &b_TimeResolvedTracksTruthCollectionSelector); }
  if( fChain->GetBranchStatus("TimeInDetTRTExtension") ) { fChain->SetBranchAddress("TimeInDetTRTExtension", &TimeInDetTRTExtension, &b_TimeInDetTRTExtension); }
  if( fChain->GetBranchStatus("TimeInDetExtensionProcessor") ) { fChain->SetBranchAddress("TimeInDetExtensionProcessor", &TimeInDetExtensionProcessor, &b_TimeInDetExtensionProcessor); }
  if( fChain->GetBranchStatus("TimeExtendedTracksDetailedTruthMaker") ) { fChain->SetBranchAddress("TimeExtendedTracksDetailedTruthMaker", &TimeExtendedTracksDetailedTruthMaker, &b_TimeExtendedTracksDetailedTruthMaker); }
  if( fChain->GetBranchStatus("TimeExtendedTracksTruthCollectionSelector") ) { fChain->SetBranchAddress("TimeExtendedTracksTruthCollectionSelector", &TimeExtendedTracksTruthCollectionSelector, &b_TimeExtendedTracksTruthCollectionSelector); }
  if( fChain->GetBranchStatus("TimeInDetTRTTrackSegmentsFinder") ) { fChain->SetBranchAddress("TimeInDetTRTTrackSegmentsFinder", &TimeInDetTRTTrackSegmentsFinder, &b_TimeInDetTRTTrackSegmentsFinder); }
  if( fChain->GetBranchStatus("TimeInDetOutputCopyAlg") ) { fChain->SetBranchAddress("TimeInDetOutputCopyAlg", &TimeInDetOutputCopyAlg, &b_TimeInDetOutputCopyAlg); }
  if( fChain->GetBranchStatus("TimeDetailedTracksTruthMaker") ) { fChain->SetBranchAddress("TimeDetailedTracksTruthMaker", &TimeDetailedTracksTruthMaker, &b_TimeDetailedTracksTruthMaker); }
  if( fChain->GetBranchStatus("TimeTrackTruthCollectionSelector") ) { fChain->SetBranchAddress("TimeTrackTruthCollectionSelector", &TimeTrackTruthCollectionSelector, &b_TimeTrackTruthCollectionSelector); }
  if( fChain->GetBranchStatus("TimeInDetPriVxFinder") ) { fChain->SetBranchAddress("TimeInDetPriVxFinder", &TimeInDetPriVxFinder, &b_TimeInDetPriVxFinder); }
  if( fChain->GetBranchStatus("TimeInDetIPatPriVxFinder") ) { fChain->SetBranchAddress("TimeInDetIPatPriVxFinder", &TimeInDetIPatPriVxFinder, &b_TimeInDetIPatPriVxFinder); }
  if( fChain->GetBranchStatus("TimeInDetXKalPriVxFinder") ) { fChain->SetBranchAddress("TimeInDetXKalPriVxFinder", &TimeInDetXKalPriVxFinder, &b_TimeInDetXKalPriVxFinder); }
  if( fChain->GetBranchStatus("TimeInDetParticleCreation") ) { fChain->SetBranchAddress("TimeInDetParticleCreation", &TimeInDetParticleCreation, &b_TimeInDetParticleCreation); }
  if( fChain->GetBranchStatus("TimeInDetIPatParticleCreation") ) { fChain->SetBranchAddress("TimeInDetIPatParticleCreation", &TimeInDetIPatParticleCreation, &b_TimeInDetIPatParticleCreation); }
  if( fChain->GetBranchStatus("TimeInDetXKalParticleCreation") ) { fChain->SetBranchAddress("TimeInDetXKalParticleCreation", &TimeInDetXKalParticleCreation, &b_TimeInDetXKalParticleCreation); }
  if( fChain->GetBranchStatus("TimeInDetTrackParticleTruthMaker") ) { fChain->SetBranchAddress("TimeInDetTrackParticleTruthMaker", &TimeInDetTrackParticleTruthMaker, &b_TimeInDetTrackParticleTruthMaker); }
  if( fChain->GetBranchStatus("TimeInDetIPatTrackParticleTruthMaker") ) { fChain->SetBranchAddress("TimeInDetIPatTrackParticleTruthMaker", &TimeInDetIPatTrackParticleTruthMaker, &b_TimeInDetIPatTrackParticleTruthMaker); }
  if( fChain->GetBranchStatus("TimeInDetXKalTrackParticleTruthMaker") ) { fChain->SetBranchAddress("TimeInDetXKalTrackParticleTruthMaker", &TimeInDetXKalTrackParticleTruthMaker, &b_TimeInDetXKalTrackParticleTruthMaker); }
  if( fChain->GetBranchStatus("TimeInDetRecStatistics") ) { fChain->SetBranchAddress("TimeInDetRecStatistics", &TimeInDetRecStatistics, &b_TimeInDetRecStatistics); }
  if( fChain->GetBranchStatus("TimeCaloCellMaker") ) { fChain->SetBranchAddress("TimeCaloCellMaker", &TimeCaloCellMaker, &b_TimeCaloCellMaker); }
  if( fChain->GetBranchStatus("TimeCmbTowerBldr") ) { fChain->SetBranchAddress("TimeCmbTowerBldr", &TimeCmbTowerBldr, &b_TimeCmbTowerBldr); }
  if( fChain->GetBranchStatus("TimeCaloSWClusterMaker") ) { fChain->SetBranchAddress("TimeCaloSWClusterMaker", &TimeCaloSWClusterMaker, &b_TimeCaloSWClusterMaker); }
  if( fChain->GetBranchStatus("TimeTowerMaker") ) { fChain->SetBranchAddress("TimeTowerMaker", &TimeTowerMaker, &b_TimeTowerMaker); }
  if( fChain->GetBranchStatus("TimeLArSWClusterMaker") ) { fChain->SetBranchAddress("TimeLArSWClusterMaker", &TimeLArSWClusterMaker, &b_TimeLArSWClusterMaker); }
  if( fChain->GetBranchStatus("TimeLArgamClusterMaker") ) { fChain->SetBranchAddress("TimeLArgamClusterMaker", &TimeLArgamClusterMaker, &b_TimeLArgamClusterMaker); }
  if( fChain->GetBranchStatus("TimeLAr35ClusterMaker") ) { fChain->SetBranchAddress("TimeLAr35ClusterMaker", &TimeLAr35ClusterMaker, &b_TimeLAr35ClusterMaker); }
  if( fChain->GetBranchStatus("TimeLAr37ClusterMaker") ) { fChain->SetBranchAddress("TimeLAr37ClusterMaker", &TimeLAr37ClusterMaker, &b_TimeLAr37ClusterMaker); }
  if( fChain->GetBranchStatus("TimeLArgam35ClusterMaker") ) { fChain->SetBranchAddress("TimeLArgam35ClusterMaker", &TimeLArgam35ClusterMaker, &b_TimeLArgam35ClusterMaker); }
  if( fChain->GetBranchStatus("TimeLArgam37ClusterMaker") ) { fChain->SetBranchAddress("TimeLArgam37ClusterMaker", &TimeLArgam37ClusterMaker, &b_TimeLArgam37ClusterMaker); }
  if( fChain->GetBranchStatus("TimeCaloTopoClusterMaker") ) { fChain->SetBranchAddress("TimeCaloTopoClusterMaker", &TimeCaloTopoClusterMaker, &b_TimeCaloTopoClusterMaker); }
  if( fChain->GetBranchStatus("TimeCaloCell2TopoClusterMapper") ) { fChain->SetBranchAddress("TimeCaloCell2TopoClusterMapper", &TimeCaloCell2TopoClusterMapper, &b_TimeCaloCell2TopoClusterMapper); }
  if( fChain->GetBranchStatus("TimeEMTopoClusterMaker") ) { fChain->SetBranchAddress("TimeEMTopoClusterMaker", &TimeEMTopoClusterMaker, &b_TimeEMTopoClusterMaker); }
  if( fChain->GetBranchStatus("TimeEMCell2TopoClusterMapper") ) { fChain->SetBranchAddress("TimeEMCell2TopoClusterMapper", &TimeEMCell2TopoClusterMapper, &b_TimeEMCell2TopoClusterMapper); }
  if( fChain->GetBranchStatus("TimeMuonRdoToMuonDigit") ) { fChain->SetBranchAddress("TimeMuonRdoToMuonDigit", &TimeMuonRdoToMuonDigit, &b_TimeMuonRdoToMuonDigit); }
  if( fChain->GetBranchStatus("TimeCscRdoToCscPrepData") ) { fChain->SetBranchAddress("TimeCscRdoToCscPrepData", &TimeCscRdoToCscPrepData, &b_TimeCscRdoToCscPrepData); }
  if( fChain->GetBranchStatus("TimeMdtRdoToMdtPrepData") ) { fChain->SetBranchAddress("TimeMdtRdoToMdtPrepData", &TimeMdtRdoToMdtPrepData, &b_TimeMdtRdoToMdtPrepData); }
  if( fChain->GetBranchStatus("TimeRpcRdoToRpcPrepData") ) { fChain->SetBranchAddress("TimeRpcRdoToRpcPrepData", &TimeRpcRdoToRpcPrepData, &b_TimeRpcRdoToRpcPrepData); }
  if( fChain->GetBranchStatus("TimeTgcRdoToTgcPrepData") ) { fChain->SetBranchAddress("TimeTgcRdoToTgcPrepData", &TimeTgcRdoToTgcPrepData, &b_TimeTgcRdoToTgcPrepData); }
  if( fChain->GetBranchStatus("TimeCscThresholdClusterBuilder") ) { fChain->SetBranchAddress("TimeCscThresholdClusterBuilder", &TimeCscThresholdClusterBuilder, &b_TimeCscThresholdClusterBuilder); }
  if( fChain->GetBranchStatus("TimeTrackRecordFilter") ) { fChain->SetBranchAddress("TimeTrackRecordFilter", &TimeTrackRecordFilter, &b_TimeTrackRecordFilter); }
  if( fChain->GetBranchStatus("TimeMboyDigiEmptyLoop") ) { fChain->SetBranchAddress("TimeMboyDigiEmptyLoop", &TimeMboyDigiEmptyLoop, &b_TimeMboyDigiEmptyLoop); }
  if( fChain->GetBranchStatus("TimeMboyDigiEmptyLoop2") ) { fChain->SetBranchAddress("TimeMboyDigiEmptyLoop2", &TimeMboyDigiEmptyLoop2, &b_TimeMboyDigiEmptyLoop2); }
  if( fChain->GetBranchStatus("TimeMboyRec") ) { fChain->SetBranchAddress("TimeMboyRec", &TimeMboyRec, &b_TimeMboyRec); }
  if( fChain->GetBranchStatus("TimeMooMakePhiPatterns") ) { fChain->SetBranchAddress("TimeMooMakePhiPatterns", &TimeMooMakePhiPatterns, &b_TimeMooMakePhiPatterns); }
  if( fChain->GetBranchStatus("TimeMooMakeRzPatterns") ) { fChain->SetBranchAddress("TimeMooMakeRzPatterns", &TimeMooMakeRzPatterns, &b_TimeMooMakeRzPatterns); }
  if( fChain->GetBranchStatus("TimeMooCombinePatterns") ) { fChain->SetBranchAddress("TimeMooCombinePatterns", &TimeMooCombinePatterns, &b_TimeMooCombinePatterns); }
  if( fChain->GetBranchStatus("TimeMooCalibratedSegmentMaker") ) { fChain->SetBranchAddress("TimeMooCalibratedSegmentMaker", &TimeMooCalibratedSegmentMaker, &b_TimeMooCalibratedSegmentMaker); }
  if( fChain->GetBranchStatus("TimePhiPatternToPhiSeg") ) { fChain->SetBranchAddress("TimePhiPatternToPhiSeg", &TimePhiPatternToPhiSeg, &b_TimePhiPatternToPhiSeg); }
  if( fChain->GetBranchStatus("TimeCscPatternToCrudeSeg") ) { fChain->SetBranchAddress("TimeCscPatternToCrudeSeg", &TimeCscPatternToCrudeSeg, &b_TimeCscPatternToCrudeSeg); }
  if( fChain->GetBranchStatus("TimeMdtPatternToCrudeSeg") ) { fChain->SetBranchAddress("TimeMdtPatternToCrudeSeg", &TimeMdtPatternToCrudeSeg, &b_TimeMdtPatternToCrudeSeg); }
  if( fChain->GetBranchStatus("TimeMuonSegmentComboToMooRzSegmentCombo") ) { fChain->SetBranchAddress("TimeMuonSegmentComboToMooRzSegmentCombo", &TimeMuonSegmentComboToMooRzSegmentCombo, &b_TimeMuonSegmentComboToMooRzSegmentCombo); }
  if( fChain->GetBranchStatus("TimeMuonSegmentToMooRzSegment") ) { fChain->SetBranchAddress("TimeMuonSegmentToMooRzSegment", &TimeMuonSegmentToMooRzSegment, &b_TimeMuonSegmentToMooRzSegment); }
  if( fChain->GetBranchStatus("TimeMooRoadMaker") ) { fChain->SetBranchAddress("TimeMooRoadMaker", &TimeMooRoadMaker, &b_TimeMooRoadMaker); }
  if( fChain->GetBranchStatus("TimeMooMakeTracks") ) { fChain->SetBranchAddress("TimeMooMakeTracks", &TimeMooMakeTracks, &b_TimeMooMakeTracks); }
  if( fChain->GetBranchStatus("TimeMooCnvAlg") ) { fChain->SetBranchAddress("TimeMooCnvAlg", &TimeMooCnvAlg, &b_TimeMooCnvAlg); }
  if( fChain->GetBranchStatus("TimeMuidMooreTPCreator") ) { fChain->SetBranchAddress("TimeMuidMooreTPCreator", &TimeMuidMooreTPCreator, &b_TimeMuidMooreTPCreator); }
  if( fChain->GetBranchStatus("TimeConversionFinder") ) { fChain->SetBranchAddress("TimeConversionFinder", &TimeConversionFinder, &b_TimeConversionFinder); }
  if( fChain->GetBranchStatus("Timeegamma") ) { fChain->SetBranchAddress("Timeegamma", &Timeegamma, &b_Timeegamma); }
  if( fChain->GetBranchStatus("Timesofte") ) { fChain->SetBranchAddress("Timesofte", &Timesofte, &b_Timesofte); }
  if( fChain->GetBranchStatus("TimeConeTowerJets") ) { fChain->SetBranchAddress("TimeConeTowerJets", &TimeConeTowerJets, &b_TimeConeTowerJets); }
  if( fChain->GetBranchStatus("TimeCone4TowerJets") ) { fChain->SetBranchAddress("TimeCone4TowerJets", &TimeCone4TowerJets, &b_TimeCone4TowerJets); }
  if( fChain->GetBranchStatus("TimeKtTowerJets") ) { fChain->SetBranchAddress("TimeKtTowerJets", &TimeKtTowerJets, &b_TimeKtTowerJets); }
  if( fChain->GetBranchStatus("TimeKt4TowerJets") ) { fChain->SetBranchAddress("TimeKt4TowerJets", &TimeKt4TowerJets, &b_TimeKt4TowerJets); }
  if( fChain->GetBranchStatus("TimeConeTopoJetAlg") ) { fChain->SetBranchAddress("TimeConeTopoJetAlg", &TimeConeTopoJetAlg, &b_TimeConeTopoJetAlg); }
  if( fChain->GetBranchStatus("TimeCone4TopoJetAlg") ) { fChain->SetBranchAddress("TimeCone4TopoJetAlg", &TimeCone4TopoJetAlg, &b_TimeCone4TopoJetAlg); }
  if( fChain->GetBranchStatus("TimeKt6TopoJets") ) { fChain->SetBranchAddress("TimeKt6TopoJets", &TimeKt6TopoJets, &b_TimeKt6TopoJets); }
  if( fChain->GetBranchStatus("TimeKt4TopoJets") ) { fChain->SetBranchAddress("TimeKt4TopoJets", &TimeKt4TopoJets, &b_TimeKt4TopoJets); }
  if( fChain->GetBranchStatus("TimeConeTruthJets") ) { fChain->SetBranchAddress("TimeConeTruthJets", &TimeConeTruthJets, &b_TimeConeTruthJets); }
  if( fChain->GetBranchStatus("TimeCone4TruthJets") ) { fChain->SetBranchAddress("TimeCone4TruthJets", &TimeCone4TruthJets, &b_TimeCone4TruthJets); }
  if( fChain->GetBranchStatus("TimeKt6TruthJets") ) { fChain->SetBranchAddress("TimeKt6TruthJets", &TimeKt6TruthJets, &b_TimeKt6TruthJets); }
  if( fChain->GetBranchStatus("TimeKt4TruthJets") ) { fChain->SetBranchAddress("TimeKt4TruthJets", &TimeKt4TruthJets, &b_TimeKt4TruthJets); }
  if( fChain->GetBranchStatus("TimetauSequence") ) { fChain->SetBranchAddress("TimetauSequence", &TimetauSequence, &b_TimetauSequence); }
  if( fChain->GetBranchStatus("Timetau1p3pSequence") ) { fChain->SetBranchAddress("Timetau1p3pSequence", &Timetau1p3pSequence, &b_Timetau1p3pSequence); }
  if( fChain->GetBranchStatus("TimeTileLookForMuAlg") ) { fChain->SetBranchAddress("TimeTileLookForMuAlg", &TimeTileLookForMuAlg, &b_TimeTileLookForMuAlg); }
  if( fChain->GetBranchStatus("TimeMuidStandAlone") ) { fChain->SetBranchAddress("TimeMuidStandAlone", &TimeMuidStandAlone, &b_TimeMuidStandAlone); }
  if( fChain->GetBranchStatus("TimeMuidExtrCnvAlg") ) { fChain->SetBranchAddress("TimeMuidExtrCnvAlg", &TimeMuidExtrCnvAlg, &b_TimeMuidExtrCnvAlg); }
  if( fChain->GetBranchStatus("TimeMuidCombined") ) { fChain->SetBranchAddress("TimeMuidCombined", &TimeMuidCombined, &b_TimeMuidCombined); }
  if( fChain->GetBranchStatus("TimeMuidCnvAlg") ) { fChain->SetBranchAddress("TimeMuidCnvAlg", &TimeMuidCnvAlg, &b_TimeMuidCnvAlg); }
  if( fChain->GetBranchStatus("TimeMuGirl") ) { fChain->SetBranchAddress("TimeMuGirl", &TimeMuGirl, &b_TimeMuGirl); }
  if( fChain->GetBranchStatus("TimeMuGirlToCombinedMuon") ) { fChain->SetBranchAddress("TimeMuGirlToCombinedMuon", &TimeMuGirlToCombinedMuon, &b_TimeMuGirlToCombinedMuon); }
  if( fChain->GetBranchStatus("TimeStacoMaster") ) { fChain->SetBranchAddress("TimeStacoMaster", &TimeStacoMaster, &b_TimeStacoMaster); }
  if( fChain->GetBranchStatus("TimeMuTagMaster") ) { fChain->SetBranchAddress("TimeMuTagMaster", &TimeMuTagMaster, &b_TimeMuTagMaster); }
  if( fChain->GetBranchStatus("TimeMuonPrepRawDataTruthMaker") ) { fChain->SetBranchAddress("TimeMuonPrepRawDataTruthMaker", &TimeMuonPrepRawDataTruthMaker, &b_TimeMuonPrepRawDataTruthMaker); }
  if( fChain->GetBranchStatus("TimeMuonTrackTruthMaker") ) { fChain->SetBranchAddress("TimeMuonTrackTruthMaker", &TimeMuonTrackTruthMaker, &b_TimeMuonTrackTruthMaker); }
  if( fChain->GetBranchStatus("TimeMETRefAlg") ) { fChain->SetBranchAddress("TimeMETRefAlg", &TimeMETRefAlg, &b_TimeMETRefAlg); }
  if( fChain->GetBranchStatus("TimeMETAlg") ) { fChain->SetBranchAddress("TimeMETAlg", &TimeMETAlg, &b_TimeMETAlg); }
  if( fChain->GetBranchStatus("TimeLeptonAlg") ) { fChain->SetBranchAddress("TimeLeptonAlg", &TimeLeptonAlg, &b_TimeLeptonAlg); }
  if( fChain->GetBranchStatus("TimeHadJets") ) { fChain->SetBranchAddress("TimeHadJets", &TimeHadJets, &b_TimeHadJets); }
  if( fChain->GetBranchStatus("TimeHadronAlg") ) { fChain->SetBranchAddress("TimeHadronAlg", &TimeHadronAlg, &b_TimeHadronAlg); }
  if( fChain->GetBranchStatus("TimeTriggerTowerMaker") ) { fChain->SetBranchAddress("TimeTriggerTowerMaker", &TimeTriggerTowerMaker, &b_TimeTriggerTowerMaker); }
  if( fChain->GetBranchStatus("TimeJetElementMaker") ) { fChain->SetBranchAddress("TimeJetElementMaker", &TimeJetElementMaker, &b_TimeJetElementMaker); }
  if( fChain->GetBranchStatus("TimeEmTauTrigger") ) { fChain->SetBranchAddress("TimeEmTauTrigger", &TimeEmTauTrigger, &b_TimeEmTauTrigger); }
  if( fChain->GetBranchStatus("TimeJetTrigger") ) { fChain->SetBranchAddress("TimeJetTrigger", &TimeJetTrigger, &b_TimeJetTrigger); }
  if( fChain->GetBranchStatus("TimeEnergyTrigger") ) { fChain->SetBranchAddress("TimeEnergyTrigger", &TimeEnergyTrigger, &b_TimeEnergyTrigger); }
  if( fChain->GetBranchStatus("TimeROD") ) { fChain->SetBranchAddress("TimeROD", &TimeROD, &b_TimeROD); }
  if( fChain->GetBranchStatus("TimeTrigT1RPC") ) { fChain->SetBranchAddress("TimeTrigT1RPC", &TimeTrigT1RPC, &b_TimeTrigT1RPC); }
  if( fChain->GetBranchStatus("TimeLVL1TGCTrigger") ) { fChain->SetBranchAddress("TimeLVL1TGCTrigger", &TimeLVL1TGCTrigger, &b_TimeLVL1TGCTrigger); }
  if( fChain->GetBranchStatus("TimeL1Muctpi") ) { fChain->SetBranchAddress("TimeL1Muctpi", &TimeL1Muctpi, &b_TimeL1Muctpi); }
  if( fChain->GetBranchStatus("TimeCTPSim") ) { fChain->SetBranchAddress("TimeCTPSim", &TimeCTPSim, &b_TimeCTPSim); }
  if( fChain->GetBranchStatus("TimeRoIBuilder") ) { fChain->SetBranchAddress("TimeRoIBuilder", &TimeRoIBuilder, &b_TimeRoIBuilder); }
  if( fChain->GetBranchStatus("TimeZVertexFromKine") ) { fChain->SetBranchAddress("TimeZVertexFromKine", &TimeZVertexFromKine, &b_TimeZVertexFromKine); }
  if( fChain->GetBranchStatus("TimeStreamBS") ) { fChain->SetBranchAddress("TimeStreamBS", &TimeStreamBS, &b_TimeStreamBS); }
  if( fChain->GetBranchStatus("TimeTriggerConfig") ) { fChain->SetBranchAddress("TimeTriggerConfig", &TimeTriggerConfig, &b_TimeTriggerConfig); }
  if( fChain->GetBranchStatus("TimeStepController_L2") ) { fChain->SetBranchAddress("TimeStepController_L2", &TimeStepController_L2, &b_TimeStepController_L2); }
  if( fChain->GetBranchStatus("TimeStepController_EF") ) { fChain->SetBranchAddress("TimeStepController_EF", &TimeStepController_EF, &b_TimeStepController_EF); }
  if( fChain->GetBranchStatus("TimeTrigMooCnvAlg") ) { fChain->SetBranchAddress("TimeTrigMooCnvAlg", &TimeTrigMooCnvAlg, &b_TimeTrigMooCnvAlg); }
  if( fChain->GetBranchStatus("TimeTrigMuidMooreTPCreator") ) { fChain->SetBranchAddress("TimeTrigMuidMooreTPCreator", &TimeTrigMuidMooreTPCreator, &b_TimeTrigMuidMooreTPCreator); }
  if( fChain->GetBranchStatus("TimeTrigMuidExtrCnvAlg") ) { fChain->SetBranchAddress("TimeTrigMuidExtrCnvAlg", &TimeTrigMuidExtrCnvAlg, &b_TimeTrigMuidExtrCnvAlg); }
  if( fChain->GetBranchStatus("TimeTrigMuidCnvAlg") ) { fChain->SetBranchAddress("TimeTrigMuidCnvAlg", &TimeTrigMuidCnvAlg, &b_TimeTrigMuidCnvAlg); }
  if( fChain->GetBranchStatus("TimeTrigMuonBuilder") ) { fChain->SetBranchAddress("TimeTrigMuonBuilder", &TimeTrigMuonBuilder, &b_TimeTrigMuonBuilder); }
  if( fChain->GetBranchStatus("TimeTrigInDetTrackTruthMaker") ) { fChain->SetBranchAddress("TimeTrigInDetTrackTruthMaker", &TimeTrigInDetTrackTruthMaker, &b_TimeTrigInDetTrackTruthMaker); }
  if( fChain->GetBranchStatus("TimeTriggerDecisionMaker") ) { fChain->SetBranchAddress("TimeTriggerDecisionMaker", &TimeTriggerDecisionMaker, &b_TimeTriggerDecisionMaker); }
  if( fChain->GetBranchStatus("TimeL1CaloObjectsToESD") ) { fChain->SetBranchAddress("TimeL1CaloObjectsToESD", &TimeL1CaloObjectsToESD, &b_TimeL1CaloObjectsToESD); }
  if( fChain->GetBranchStatus("TimeRoIBResultToAOD") ) { fChain->SetBranchAddress("TimeRoIBResultToAOD", &TimeRoIBResultToAOD, &b_TimeRoIBResultToAOD); }
  if( fChain->GetBranchStatus("TimephotonBuilder") ) { fChain->SetBranchAddress("TimephotonBuilder", &TimephotonBuilder, &b_TimephotonBuilder); }
  if( fChain->GetBranchStatus("TimeelectronBuilder") ) { fChain->SetBranchAddress("TimeelectronBuilder", &TimeelectronBuilder, &b_TimeelectronBuilder); }
  if( fChain->GetBranchStatus("TimeMuonBuilder") ) { fChain->SetBranchAddress("TimeMuonBuilder", &TimeMuonBuilder, &b_TimeMuonBuilder); }
  if( fChain->GetBranchStatus("TimetauJetBuilder") ) { fChain->SetBranchAddress("TimetauJetBuilder", &TimetauJetBuilder, &b_TimetauJetBuilder); }
  if( fChain->GetBranchStatus("TimetauJetBuilder1p3p") ) { fChain->SetBranchAddress("TimetauJetBuilder1p3p", &TimetauJetBuilder1p3p, &b_TimetauJetBuilder1p3p); }
  if( fChain->GetBranchStatus("TimeKt4TowerParticleJetBuilder") ) { fChain->SetBranchAddress("TimeKt4TowerParticleJetBuilder", &TimeKt4TowerParticleJetBuilder, &b_TimeKt4TowerParticleJetBuilder); }
  if( fChain->GetBranchStatus("TimeKt6TowerParticleJetBuilder") ) { fChain->SetBranchAddress("TimeKt6TowerParticleJetBuilder", &TimeKt6TowerParticleJetBuilder, &b_TimeKt6TowerParticleJetBuilder); }
  if( fChain->GetBranchStatus("TimeCone4TowerParticleJetBuilder") ) { fChain->SetBranchAddress("TimeCone4TowerParticleJetBuilder", &TimeCone4TowerParticleJetBuilder, &b_TimeCone4TowerParticleJetBuilder); }
  if( fChain->GetBranchStatus("TimeConeTowerParticleJetBuilder") ) { fChain->SetBranchAddress("TimeConeTowerParticleJetBuilder", &TimeConeTowerParticleJetBuilder, &b_TimeConeTowerParticleJetBuilder); }
  if( fChain->GetBranchStatus("TimeKt4TopoParticleJetBuilder") ) { fChain->SetBranchAddress("TimeKt4TopoParticleJetBuilder", &TimeKt4TopoParticleJetBuilder, &b_TimeKt4TopoParticleJetBuilder); }
  if( fChain->GetBranchStatus("TimeKt6TopoParticleJetBuilder") ) { fChain->SetBranchAddress("TimeKt6TopoParticleJetBuilder", &TimeKt6TopoParticleJetBuilder, &b_TimeKt6TopoParticleJetBuilder); }
  if( fChain->GetBranchStatus("TimeCone4TopoParticleJetBuilder") ) { fChain->SetBranchAddress("TimeCone4TopoParticleJetBuilder", &TimeCone4TopoParticleJetBuilder, &b_TimeCone4TopoParticleJetBuilder); }
  if( fChain->GetBranchStatus("TimeConeTopoParticleJetBuilder") ) { fChain->SetBranchAddress("TimeConeTopoParticleJetBuilder", &TimeConeTopoParticleJetBuilder, &b_TimeConeTopoParticleJetBuilder); }
  if( fChain->GetBranchStatus("TimeSharedHitMapper") ) { fChain->SetBranchAddress("TimeSharedHitMapper", &TimeSharedHitMapper, &b_TimeSharedHitMapper); }
  if( fChain->GetBranchStatus("TimeBJetBuilderKt4Tower") ) { fChain->SetBranchAddress("TimeBJetBuilderKt4Tower", &TimeBJetBuilderKt4Tower, &b_TimeBJetBuilderKt4Tower); }
  if( fChain->GetBranchStatus("TimeBJetBuilderKt6Tower") ) { fChain->SetBranchAddress("TimeBJetBuilderKt6Tower", &TimeBJetBuilderKt6Tower, &b_TimeBJetBuilderKt6Tower); }
  if( fChain->GetBranchStatus("TimeBJetBuilderCone4Tower") ) { fChain->SetBranchAddress("TimeBJetBuilderCone4Tower", &TimeBJetBuilderCone4Tower, &b_TimeBJetBuilderCone4Tower); }
  if( fChain->GetBranchStatus("TimeBJetBuilderConeTower") ) { fChain->SetBranchAddress("TimeBJetBuilderConeTower", &TimeBJetBuilderConeTower, &b_TimeBJetBuilderConeTower); }
  if( fChain->GetBranchStatus("TimeBJetBuilderKt4Topo") ) { fChain->SetBranchAddress("TimeBJetBuilderKt4Topo", &TimeBJetBuilderKt4Topo, &b_TimeBJetBuilderKt4Topo); }
  if( fChain->GetBranchStatus("TimeBJetBuilderKt6Topo") ) { fChain->SetBranchAddress("TimeBJetBuilderKt6Topo", &TimeBJetBuilderKt6Topo, &b_TimeBJetBuilderKt6Topo); }
  if( fChain->GetBranchStatus("TimeBJetBuilderCone4Topo") ) { fChain->SetBranchAddress("TimeBJetBuilderCone4Topo", &TimeBJetBuilderCone4Topo, &b_TimeBJetBuilderCone4Topo); }
  if( fChain->GetBranchStatus("TimeBJetBuilderConeTopo") ) { fChain->SetBranchAddress("TimeBJetBuilderConeTopo", &TimeBJetBuilderConeTopo, &b_TimeBJetBuilderConeTopo); }
  if( fChain->GetBranchStatus("TimeKt4TruthParticleJetBuilder") ) { fChain->SetBranchAddress("TimeKt4TruthParticleJetBuilder", &TimeKt4TruthParticleJetBuilder, &b_TimeKt4TruthParticleJetBuilder); }
  if( fChain->GetBranchStatus("TimeKt6TruthParticleJetBuilder") ) { fChain->SetBranchAddress("TimeKt6TruthParticleJetBuilder", &TimeKt6TruthParticleJetBuilder, &b_TimeKt6TruthParticleJetBuilder); }
  if( fChain->GetBranchStatus("TimeCone4TruthParticleJetBuilder") ) { fChain->SetBranchAddress("TimeCone4TruthParticleJetBuilder", &TimeCone4TruthParticleJetBuilder, &b_TimeCone4TruthParticleJetBuilder); }
  if( fChain->GetBranchStatus("TimeConeTruthParticleJetBuilder") ) { fChain->SetBranchAddress("TimeConeTruthParticleJetBuilder", &TimeConeTruthParticleJetBuilder, &b_TimeConeTruthParticleJetBuilder); }
  if( fChain->GetBranchStatus("TimeMcAodBuilder") ) { fChain->SetBranchAddress("TimeMcAodBuilder", &TimeMcAodBuilder, &b_TimeMcAodBuilder); }
  if( fChain->GetBranchStatus("TimeTopSequence") ) { fChain->SetBranchAddress("TimeTopSequence", &TimeTopSequence, &b_TimeTopSequence); }
  if( fChain->GetBranchStatus("TimeAtlfastAodBuilder") ) { fChain->SetBranchAddress("TimeAtlfastAodBuilder", &TimeAtlfastAodBuilder, &b_TimeAtlfastAodBuilder); }
  if( fChain->GetBranchStatus("TimeFastAANT") ) { fChain->SetBranchAddress("TimeFastAANT", &TimeFastAANT, &b_TimeFastAANT); }
  if( fChain->GetBranchStatus("TimeFastAANTupleStream") ) { fChain->SetBranchAddress("TimeFastAANTupleStream", &TimeFastAANTupleStream, &b_TimeFastAANTupleStream); }
  if( fChain->GetBranchStatus("TimeStreamESD") ) { fChain->SetBranchAddress("TimeStreamESD", &TimeStreamESD, &b_TimeStreamESD); }
  if( fChain->GetBranchStatus("TimeMakeInputDataHeaderESD") ) { fChain->SetBranchAddress("TimeMakeInputDataHeaderESD", &TimeMakeInputDataHeaderESD, &b_TimeMakeInputDataHeaderESD); }
  if( fChain->GetBranchStatus("TimeStreamAOD") ) { fChain->SetBranchAddress("TimeStreamAOD", &TimeStreamAOD, &b_TimeStreamAOD); }
  if( fChain->GetBranchStatus("TimeCBNT_AthenaAware") ) { fChain->SetBranchAddress("TimeCBNT_AthenaAware", &TimeCBNT_AthenaAware, &b_TimeCBNT_AthenaAware); }
  if( fChain->GetBranchStatus("TimeAANTupleStream") ) { fChain->SetBranchAddress("TimeAANTupleStream", &TimeAANTupleStream, &b_TimeAANTupleStream); }
  if( fChain->GetBranchStatus("TimeDumpSp") ) { fChain->SetBranchAddress("TimeDumpSp", &TimeDumpSp, &b_TimeDumpSp); }

  Notify();
}

Bool_t WHTreeV15::Notify()
{
  // The Notify() function is called when a new file is opened. This
  // can be either for a new TTree in a TChain or when when a new TTree
  // is started when using PROOF. It is normally not necessary to make changes
  // to the generated code, but the routine can be extended by the
  // user if needed. The return value is currently not used.

  return kTRUE;
}

void WHTreeV15::Show(Long64_t entry)
{
  // Print contents of entry.
  // If entry is not specified, print current entry
  if (!fChain) return;
  fChain->Show(entry);
}
Int_t WHTreeV15::Cut(Long64_t entry)
{
  // This function may be called from Loop.
  // returns  1 if entry is accepted.
  // returns -1 otherwise.
  return 1;
}
#endif // #ifdef WHTreeV15_cxx
