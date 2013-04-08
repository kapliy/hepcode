//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Oct 15 15:41:10 2009 by ROOT version 5.22/00a
// from TTree CollectionTree/CollectionTree
// found on file: dcache:///pnfs/uct3/users/boveia/v14/WH120bb1E34.20091012/user09.JosephTuggle.WH120bb1E34.set01.20091005.EXT2._00001.output.aant.root
//////////////////////////////////////////////////////////

#ifndef WHTreeV14_h
#define WHTreeV14_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <vector>
#include <string>

class WHTreeV14 {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Int_t           RunNumber;
   Int_t           EventNumber;
   Char_t          StreamESD_ref;
   Char_t          Token[153];
   Int_t           Run;
   Int_t           Event;
   Int_t           Time;
   Int_t           LumiBlock;
   Int_t           BCID;
   Int_t           LVL1ID;
   Double_t        Weight;
   Int_t           IEvent;
   Int_t           StatusElement;
   Int_t           LVL1TriggerType;
   std::vector<long>    *LVL1TriggerInfo;
   std::vector<long>    *LVL2TriggerInfo;
   std::vector<long>    *EventFilterInfo;
   std::vector<std::string>  *StreamTagName;
   std::vector<std::string>  *StreamTagType;
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
   Float_t         cl_ecluster_topoEM430;
   Int_t           cl_nctotal_topoEM430;
   Int_t           cl_nc_topoEM430;
   std::vector<float>   *cl_et_topoEM430;
   std::vector<float>   *cl_e_topoEM430;
   std::vector<float>   *cl_eemb0_topoEM430;
   std::vector<float>   *cl_eemb1_topoEM430;
   std::vector<float>   *cl_eemb2_topoEM430;
   std::vector<float>   *cl_eemb3_topoEM430;
   std::vector<long>    *cl_nemb0_topoEM430;
   std::vector<long>    *cl_nemb1_topoEM430;
   std::vector<long>    *cl_nemb2_topoEM430;
   std::vector<long>    *cl_nemb3_topoEM430;
   std::vector<float>   *cl_phi2_topoEM430;
   std::vector<float>   *cl_eta0_topoEM430;
   std::vector<float>   *cl_eta1_topoEM430;
   std::vector<float>   *cl_eta2_topoEM430;
   std::vector<float>   *cl_eta3_topoEM430;
   std::vector<float>   *cl_eeme0_topoEM430;
   std::vector<float>   *cl_eeme1_topoEM430;
   std::vector<float>   *cl_eeme2_topoEM430;
   std::vector<float>   *cl_eeme3_topoEM430;
   std::vector<long>    *cl_neme0_topoEM430;
   std::vector<long>    *cl_neme1_topoEM430;
   std::vector<long>    *cl_neme2_topoEM430;
   std::vector<long>    *cl_neme3_topoEM430;
   std::vector<float>   *cl_etileg1_topoEM430;
   std::vector<float>   *cl_etileg2_topoEM430;
   std::vector<float>   *cl_etileg3_topoEM430;
   std::vector<long>    *cl_ntileg1_topoEM430;
   std::vector<long>    *cl_ntileg2_topoEM430;
   std::vector<long>    *cl_ntileg3_topoEM430;
   std::vector<float>   *cl_eta_topoEM430;
   std::vector<float>   *cl_phi_topoEM430;
   std::vector<long>    *cl_reco_stat_topoEM430;
   std::vector<float>   *cl_m1_eta_topoEM430;
   std::vector<float>   *cl_m1_phi_topoEM430;
   std::vector<float>   *cl_m2_r_topoEM430;
   std::vector<float>   *cl_m2_lambda_topoEM430;
   std::vector<float>   *cl_delta_phi_topoEM430;
   std::vector<float>   *cl_delta_theta_topoEM430;
   std::vector<float>   *cl_delta_alpha_topoEM430;
   std::vector<float>   *cl_center_x_topoEM430;
   std::vector<float>   *cl_center_y_topoEM430;
   std::vector<float>   *cl_center_z_topoEM430;
   std::vector<float>   *cl_center_lambda_topoEM430;
   std::vector<float>   *cl_lateral_topoEM430;
   std::vector<float>   *cl_longitudinal_topoEM430;
   std::vector<float>   *cl_eng_frac_em_topoEM430;
   std::vector<float>   *cl_eng_frac_max_topoEM430;
   std::vector<float>   *cl_eng_frac_core_topoEM430;
   std::vector<float>   *cl_m1_dens_topoEM430;
   std::vector<float>   *cl_m2_dens_topoEM430;
   std::vector<float>   *cl_isol_topoEM430;
   std::vector<long>    *cl_ntotcells_topoEM430;
   UInt_t          jetNumCone7H1TowerJets;
   std::vector<double>  *jetEtaCone7H1TowerJets;
   std::vector<double>  *jetPhiCone7H1TowerJets;
   std::vector<double>  *jetECone7H1TowerJets;
   std::vector<double>  *jetEtCone7H1TowerJets;
   std::vector<double>  *jetMCone7H1TowerJets;
   std::vector<double>  *jetPxCone7H1TowerJets;
   std::vector<double>  *jetPyCone7H1TowerJets;
   std::vector<double>  *jetPzCone7H1TowerJets;
   std::vector<long>    *jetSizeCone7H1TowerJets;
   std::vector<double>  *jetEmfCone7H1TowerJets;
   UInt_t          jetNumCone4H1TopoJets;
   std::vector<double>  *jetEtaCone4H1TopoJets;
   std::vector<double>  *jetPhiCone4H1TopoJets;
   std::vector<double>  *jetECone4H1TopoJets;
   std::vector<double>  *jetEtCone4H1TopoJets;
   std::vector<double>  *jetMCone4H1TopoJets;
   std::vector<double>  *jetPxCone4H1TopoJets;
   std::vector<double>  *jetPyCone4H1TopoJets;
   std::vector<double>  *jetPzCone4H1TopoJets;
   std::vector<long>    *jetSizeCone4H1TopoJets;
   std::vector<double>  *jetEmfCone4H1TopoJets;
   UInt_t          jetNumCone4H1TowerJets;
   std::vector<double>  *jetEtaCone4H1TowerJets;
   std::vector<double>  *jetPhiCone4H1TowerJets;
   std::vector<double>  *jetECone4H1TowerJets;
   std::vector<double>  *jetEtCone4H1TowerJets;
   std::vector<double>  *jetMCone4H1TowerJets;
   std::vector<double>  *jetPxCone4H1TowerJets;
   std::vector<double>  *jetPyCone4H1TowerJets;
   std::vector<double>  *jetPzCone4H1TowerJets;
   std::vector<long>    *jetSizeCone4H1TowerJets;
   std::vector<double>  *jetEmfCone4H1TowerJets;
   UInt_t          jetNumCone4TruthJets;
   std::vector<double>  *jetEtaCone4TruthJets;
   std::vector<double>  *jetPhiCone4TruthJets;
   std::vector<double>  *jetECone4TruthJets;
   std::vector<double>  *jetEtCone4TruthJets;
   std::vector<double>  *jetMCone4TruthJets;
   std::vector<double>  *jetPxCone4TruthJets;
   std::vector<double>  *jetPyCone4TruthJets;
   std::vector<double>  *jetPzCone4TruthJets;
   std::vector<long>    *jetSizeCone4TruthJets;
   std::vector<double>  *jetEmfCone4TruthJets;
   UInt_t          jetNumCone4TruthPileupJets;
   std::vector<double>  *jetEtaCone4TruthPileupJets;
   std::vector<double>  *jetPhiCone4TruthPileupJets;
   std::vector<double>  *jetECone4TruthPileupJets;
   std::vector<double>  *jetEtCone4TruthPileupJets;
   std::vector<double>  *jetMCone4TruthPileupJets;
   std::vector<double>  *jetPxCone4TruthPileupJets;
   std::vector<double>  *jetPyCone4TruthPileupJets;
   std::vector<double>  *jetPzCone4TruthPileupJets;
   std::vector<long>    *jetSizeCone4TruthPileupJets;
   std::vector<double>  *jetEmfCone4TruthPileupJets;
   Int_t           NTileMuonTag;
   std::vector<float>   *EtaTileMuonTag;
   std::vector<float>   *PhiTileMuonTag;
   std::vector<std::vector<float> > *EnergyVecTileMuonTag;
   std::vector<float>   *LabelTileMuonTag;
   UInt_t          CaloMu_ntrack;
   std::vector<int>     *CaloMu_trkPdgid;
   std::vector<int>     *CaloMu_trkMother;
   std::vector<double>  *CaloMu_trkEta;
   std::vector<double>  *CaloMu_trkPhi;
   std::vector<double>  *CaloMu_trkPt;
   std::vector<double>  *CaloMu_trkD0;
   std::vector<double>  *CaloMu_trkIP;
   std::vector<int>     *CaloMu_trkBLayerHits;
   std::vector<int>     *CaloMu_trkPixelHits;
   std::vector<int>     *CaloMu_trkSCTHits;
   std::vector<int>     *CaloMu_trkTRTHits;
   UInt_t          CaloMu_nmuon;
   std::vector<int>     *CaloMu_pdgid;
   std::vector<int>     *CaloMu_mother;
   std::vector<int>     *CaloMu_tag;
   std::vector<double>  *CaloMu_lhr;
   std::vector<std::vector<double> > *CaloMu_etCone;
   std::vector<std::vector<double> > *CaloMu_ptCone;
   std::vector<double>  *CaloMu_eta;
   std::vector<double>  *CaloMu_phi;
   std::vector<double>  *CaloMu_pt;
   std::vector<double>  *CaloMu_qoverp;
   std::vector<double>  *CaloMu_IP;
   std::vector<double>  *CaloMu_d0;
   std::vector<int>     *CaloMu_bLayerHits;
   std::vector<int>     *CaloMu_pixelHits;
   std::vector<int>     *CaloMu_SCTHits;
   std::vector<int>     *CaloMu_TRTHits;
   std::vector<std::vector<int> > *CaloMu_sample;
   std::vector<std::vector<double> > *CaloMu_energy;
   std::vector<std::vector<double> > *CaloMu_energyLoss;
   UInt_t          MuTagIMO_nmuon;
   UInt_t          MuTagIMO_nTagSegs;
   std::vector<int>     *MuTagIMO_trk_isTagged;
   std::vector<int>     *MuTagIMO_trk_matchesBarCode;
   std::vector<int>     *MuTagIMO_trk_matchesPdg;
   std::vector<int>     *MuTagIMO_trk_nrPixelHits;
   std::vector<int>     *MuTagIMO_trk_nrSctHits;
   std::vector<float>   *MuTagIMO_trk_A0;
   std::vector<float>   *MuTagIMO_trk_Z0;
   std::vector<float>   *MuTagIMO_trk_phi;
   std::vector<float>   *MuTagIMO_trk_theta;
   std::vector<float>   *MuTagIMO_trk_qOverP;
   std::vector<float>   *MuTagIMO_trk_chi2;
   std::vector<float>   *MuTagIMO_trk_ndof;
   std::vector<float>   *MuTagIMO_trk_Author;
   std::vector<float>   *MuTagIMO_trk_cov11;
   std::vector<float>   *MuTagIMO_trk_cov12;
   std::vector<float>   *MuTagIMO_trk_cov13;
   std::vector<float>   *MuTagIMO_trk_cov14;
   std::vector<float>   *MuTagIMO_trk_cov15;
   std::vector<float>   *MuTagIMO_trk_cov22;
   std::vector<float>   *MuTagIMO_trk_cov23;
   std::vector<float>   *MuTagIMO_trk_cov24;
   std::vector<float>   *MuTagIMO_trk_cov25;
   std::vector<float>   *MuTagIMO_trk_cov33;
   std::vector<float>   *MuTagIMO_trk_cov34;
   std::vector<float>   *MuTagIMO_trk_cov35;
   std::vector<float>   *MuTagIMO_trk_cov44;
   std::vector<float>   *MuTagIMO_trk_cov45;
   std::vector<float>   *MuTagIMO_trk_cov55;
   std::vector<int>     *MuTagIMO_exTrk_trackIndex;
   std::vector<float>   *MuTagIMO_exTrk_A0;
   std::vector<float>   *MuTagIMO_exTrk_Z0;
   std::vector<float>   *MuTagIMO_exTrk_phi;
   std::vector<float>   *MuTagIMO_exTrk_theta;
   std::vector<float>   *MuTagIMO_exTrk_qOverP;
   std::vector<float>   *MuTagIMO_exTrk_cov11;
   std::vector<float>   *MuTagIMO_exTrk_cov12;
   std::vector<float>   *MuTagIMO_exTrk_cov13;
   std::vector<float>   *MuTagIMO_exTrk_cov14;
   std::vector<float>   *MuTagIMO_exTrk_cov15;
   std::vector<float>   *MuTagIMO_exTrk_cov22;
   std::vector<float>   *MuTagIMO_exTrk_cov23;
   std::vector<float>   *MuTagIMO_exTrk_cov24;
   std::vector<float>   *MuTagIMO_exTrk_cov25;
   std::vector<float>   *MuTagIMO_exTrk_cov33;
   std::vector<float>   *MuTagIMO_exTrk_cov34;
   std::vector<float>   *MuTagIMO_exTrk_cov35;
   std::vector<float>   *MuTagIMO_exTrk_cov44;
   std::vector<float>   *MuTagIMO_exTrk_cov45;
   std::vector<float>   *MuTagIMO_exTrk_cov55;
   std::vector<float>   *MuTagIMO_exTrk_Eloss;
   std::vector<float>   *MuTagIMO_exTrk_gPosX;
   std::vector<float>   *MuTagIMO_exTrk_gPosY;
   std::vector<float>   *MuTagIMO_exTrk_gPosZ;
   std::vector<float>   *MuTagIMO_exTrk_gDirX;
   std::vector<float>   *MuTagIMO_exTrk_gDirY;
   std::vector<float>   *MuTagIMO_exTrk_gDirZ;
   std::vector<float>   *MuTagIMO_exTrk_locX;
   std::vector<float>   *MuTagIMO_exTrk_locY;
   std::vector<float>   *MuTagIMO_exTrk_angleXZ;
   std::vector<float>   *MuTagIMO_exTrk_angleYZ;
   std::vector<float>   *MuTagIMO_exTrk_locX_error;
   std::vector<float>   *MuTagIMO_exTrk_locY_error;
   std::vector<float>   *MuTagIMO_exTrk_angleXZ_error;
   std::vector<float>   *MuTagIMO_exTrk_angleYZ_error;
   std::vector<int>     *MuTagIMO_seg_isTagSeg;
   std::vector<int>     *MuTagIMO_seg_matchesBarCode;
   std::vector<int>     *MuTagIMO_seg_matchesPdg;
   std::vector<int>     *MuTagIMO_seg_exTrkIndex;
   std::vector<int>     *MuTagIMO_seg_trackIndex;
   std::vector<int>     *MuTagIMO_seg_stationLevel;
   std::vector<int>     *MuTagIMO_seg_isEndCap;
   std::vector<int>     *MuTagIMO_seg_isLarge;
   std::vector<int>     *MuTagIMO_seg_nrHoles;
   std::vector<int>     *MuTagIMO_seg_nrMLs;
   std::vector<int>     *MuTagIMO_seg_nrMdtHits;
   std::vector<int>     *MuTagIMO_seg_nrRpcHits;
   std::vector<int>     *MuTagIMO_seg_nrTgcHits;
   std::vector<int>     *MuTagIMO_seg_nrCscHits;
   std::vector<float>   *MuTagIMO_seg_phi;
   std::vector<float>   *MuTagIMO_seg_theta;
   std::vector<float>   *MuTagIMO_seg_chi2;
   std::vector<float>   *MuTagIMO_seg_cov11;
   std::vector<float>   *MuTagIMO_seg_cov12;
   std::vector<float>   *MuTagIMO_seg_cov13;
   std::vector<float>   *MuTagIMO_seg_cov14;
   std::vector<float>   *MuTagIMO_seg_cov15;
   std::vector<float>   *MuTagIMO_seg_cov22;
   std::vector<float>   *MuTagIMO_seg_cov23;
   std::vector<float>   *MuTagIMO_seg_cov24;
   std::vector<float>   *MuTagIMO_seg_cov25;
   std::vector<float>   *MuTagIMO_seg_cov33;
   std::vector<float>   *MuTagIMO_seg_cov34;
   std::vector<float>   *MuTagIMO_seg_cov35;
   std::vector<float>   *MuTagIMO_seg_cov44;
   std::vector<float>   *MuTagIMO_seg_cov45;
   std::vector<float>   *MuTagIMO_seg_cov55;
   std::vector<float>   *MuTagIMO_seg_gPosX;
   std::vector<float>   *MuTagIMO_seg_gPosY;
   std::vector<float>   *MuTagIMO_seg_gPosZ;
   std::vector<float>   *MuTagIMO_seg_gDirX;
   std::vector<float>   *MuTagIMO_seg_gDirY;
   std::vector<float>   *MuTagIMO_seg_gDirZ;
   std::vector<float>   *MuTagIMO_seg_locX;
   std::vector<float>   *MuTagIMO_seg_locY;
   std::vector<float>   *MuTagIMO_seg_angleXZ;
   std::vector<float>   *MuTagIMO_seg_angleYZ;
   std::vector<float>   *MuTagIMO_seg_locX_error;
   std::vector<float>   *MuTagIMO_seg_locY_error;
   std::vector<float>   *MuTagIMO_seg_angleXZ_error;
   std::vector<float>   *MuTagIMO_seg_angleYZ_error;
   std::vector<int>     *MuTagIMO_match_trackIndex;
   std::vector<float>   *MuTagIMO_match_locX;
   std::vector<float>   *MuTagIMO_match_locY;
   std::vector<float>   *MuTagIMO_match_angleXZ;
   std::vector<float>   *MuTagIMO_match_angleYZ;
   std::vector<int>     *MuTagIMO_truthID_barCode;
   std::vector<int>     *MuTagIMO_truthID_pdg;
   std::vector<float>   *MuTagIMO_truthID_posX;
   std::vector<float>   *MuTagIMO_truthID_posY;
   std::vector<float>   *MuTagIMO_truthID_posZ;
   std::vector<float>   *MuTagIMO_truthID_momX;
   std::vector<float>   *MuTagIMO_truthID_momY;
   std::vector<float>   *MuTagIMO_truthID_momZ;
   std::vector<float>   *MuTagIMO_truthID_energy;
   std::vector<int>     *MuTagIMO_truthMS_barCode;
   std::vector<int>     *MuTagIMO_truthMS_pdg;
   std::vector<float>   *MuTagIMO_truthMS_posX;
   std::vector<float>   *MuTagIMO_truthMS_posY;
   std::vector<float>   *MuTagIMO_truthMS_posZ;
   std::vector<float>   *MuTagIMO_truthMS_momX;
   std::vector<float>   *MuTagIMO_truthMS_momY;
   std::vector<float>   *MuTagIMO_truthMS_momZ;
   std::vector<float>   *MuTagIMO_truthMS_energy;
   std::vector<int>     *MuTagIMO_trHit_id;
   std::vector<int>     *MuTagIMO_trHit_barCode;
   std::vector<float>   *MuTagIMO_trHit_r;
   std::vector<float>   *MuTagIMO_trHit_alongTube;
   Float_t         L1EM_Scale;
   Int_t           L1Em_nRoI;
   std::vector<long>    *L1Em_RoIWord;
   std::vector<float>   *L1Em_Core;
   std::vector<float>   *L1Em_EmClus;
   std::vector<float>   *L1Em_TauClus;
   std::vector<float>   *L1Em_EmIsol;
   std::vector<float>   *L1Em_HdIsol;
   std::vector<float>   *L1Em_HdCore;
   std::vector<long>    *L1Em_EmThresh;
   std::vector<long>    *L1Em_TauThresh;
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
   UInt_t          nCTP_ROI;
   std::vector<unsigned long> *CTP_ROI;
   UInt_t          nMuCTPI_ROI;
   std::vector<unsigned long> *muCTPI_ROI;
   UInt_t          nEMTau_ROI;
   std::vector<unsigned long> *EMTau_ROI;
   UInt_t          nJetEnergy_ROI;
   std::vector<unsigned long> *JetEnergy_ROI;
   Int_t           L1CaloPPM_ntt;
   std::vector<double>  *L1CaloPPM_eta;
   std::vector<double>  *L1CaloPPM_phi;
   std::vector<unsigned int> *L1CaloPPM_emCrate;
   std::vector<unsigned int> *L1CaloPPM_emModule;
   std::vector<unsigned int> *L1CaloPPM_emSubmodule;
   std::vector<unsigned int> *L1CaloPPM_emChannel;
   std::vector<unsigned int> *L1CaloPPM_hadCrate;
   std::vector<unsigned int> *L1CaloPPM_hadModule;
   std::vector<unsigned int> *L1CaloPPM_hadSubmodule;
   std::vector<unsigned int> *L1CaloPPM_hadChannel;
   std::vector<unsigned int> *L1CaloPPM_emChannelId;
   std::vector<unsigned int> *L1CaloPPM_hadChannelId;
   std::vector<double>  *L1CaloPPM_emTTCellsEnergy;
   std::vector<double>  *L1CaloPPM_hadTTCellsEnergy;
   std::vector<std::vector<double> > *L1CaloPPM_emCaloWaves;
   std::vector<std::vector<double> > *L1CaloPPM_hadCaloWaves;
   std::vector<int>     *L1CaloPPM_emPeak;
   std::vector<int>     *L1CaloPPM_emADCPeak;
   std::vector<int>     *L1CaloPPM_hadPeak;
   std::vector<int>     *L1CaloPPM_hadADCPeak;
   std::vector<std::vector<int> > *L1CaloPPM_emLUT;
   std::vector<std::vector<int> > *L1CaloPPM_hadLUT;
   std::vector<std::vector<int> > *L1CaloPPM_emADC;
   std::vector<std::vector<int> > *L1CaloPPM_hadADC;
   std::vector<int>     *L1CaloPPM_emEnergy;
   std::vector<int>     *L1CaloPPM_hadEnergy;
   std::vector<std::vector<int> > *L1CaloPPM_emBCIDvec;
   std::vector<std::vector<int> > *L1CaloPPM_emBCIDext;
   std::vector<std::vector<int> > *L1CaloPPM_hadBCIDvec;
   std::vector<std::vector<int> > *L1CaloPPM_hadBCIDext;
   std::vector<int>     *L1CaloPPM_emError;
   std::vector<int>     *L1CaloPPM_hadError;
   std::vector<int>     *L1CaloPPM_emBCID;
   std::vector<int>     *L1CaloPPM_hadBCID;
   std::vector<int>     *L1CaloPPM_emIsSaturated;
   std::vector<int>     *L1CaloPPM_hadIsSaturated;
   std::vector<int>     *L1CaloPPM_emIsFilled;
   std::vector<int>     *L1CaloPPM_hadIsFilled;
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
   std::vector<float>   *T2CaEtas1;
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
   UInt_t          T2CaNjets;
   std::vector<float>   *T2CaJetE;
   std::vector<float>   *T2CaJetEhad0;
   std::vector<float>   *T2CaJetEem0;
   std::vector<float>   *T2CaJeteta;
   std::vector<float>   *T2CaJetphi;
   std::vector<unsigned int> *T2CaJetroi;
   UInt_t          TrigT2MissingEt;
   std::vector<float>   *TrigT2MissingEtEt;
   std::vector<float>   *TrigT2MissingEtEx;
   std::vector<float>   *TrigT2MissingEtEy;
   std::vector<float>   *TrigT2MissingEtsumEt;
   std::vector<float>   *TrigT2MissingEtRoIword;
   UInt_t          TrigEFMissingEt;
   std::vector<float>   *TrigEFMissingEtEt;
   std::vector<float>   *TrigEFMissingEtEx;
   std::vector<float>   *TrigEFMissingEtEy;
   std::vector<float>   *TrigEFMissingEtsumEt;
   std::vector<float>   *TrigEFMissingEtRoIword;
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
   std::vector<float>   *T2IdEtac;
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
   std::vector<long>    *TrkEF_truthBarcode;
   std::vector<long>    *TrkEF_truthNt;
   std::vector<long>    *TrkEF_ParticlePdg;
   std::vector<long>    *TrkEF_ParentPdg;
   std::vector<long>    *TrkEF_NumSiKineHits;
   std::vector<long>    *TrkEF_NumTRTKineHits;
   Int_t           TrkEF_sctSP;
   Int_t           TrkEF_pixSP;
   Int_t           VxEF_numVertices;
   std::vector<float>   *VxEF_vtx_x;
   std::vector<float>   *VxEF_vtx_y;
   std::vector<float>   *VxEF_vtx_z;
   Int_t           VxEF_numOfTruthVertices;
   std::vector<float>   *VxEF_vtx_x_truth;
   std::vector<float>   *VxEF_vtx_y_truth;
   std::vector<float>   *VxEF_vtx_z_truth;
   std::vector<float>   *VxEF_vtx_x_res;
   std::vector<float>   *VxEF_vtx_y_res;
   std::vector<float>   *VxEF_vtx_z_res;
   std::vector<float>   *VxEF_sigvtx_x;
   std::vector<float>   *VxEF_sigvtx_y;
   std::vector<float>   *VxEF_sigvtx_z;
   std::vector<float>   *VxEF_vtxchi2;
   std::vector<long>    *VxEF_vtxndf;
   std::vector<float>   *VxEF_pt;
   std::vector<float>   *VxEF_vtxchi2prob;
   std::vector<long>    *VxEF_numTracksPerVertex;
   std::vector<long>    *VxEF_vtxType;
   Int_t           VxEF_totalNumTracks;
   std::vector<float>   *VxEF_chi2;
   std::vector<float>   *VxEF_d0;
   std::vector<float>   *VxEF_sigd0;
   std::vector<float>   *VxEF_z0;
   std::vector<float>   *VxEF_sigz0;
   std::vector<float>   *VxEF_phi;
   std::vector<float>   *VxEF_sigphi;
   std::vector<float>   *VxEF_theta;
   std::vector<float>   *VxEF_sigtheta;
   std::vector<float>   *VxEF_qOverP;
   std::vector<float>   *VxEF_sigqOverP;
   std::vector<float>   *VxEF_d0g;
   std::vector<float>   *VxEF_sigd0g;
   std::vector<float>   *VxEF_z0g;
   std::vector<float>   *VxEF_sigz0g;
   std::vector<int>     *eg_RoiId_EF;
   std::vector<int>     *eg_TrigKey_EF;
   Int_t           eg_nc_EF;
   std::vector<float>   *eg_e_EF;
   std::vector<float>   *eg_eta_EF;
   std::vector<float>   *eg_phi_EF;
   std::vector<float>   *eg_author_EF;
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
   std::vector<double>  *eg_Hmatrix5_EF;
   std::vector<double>  *eg_Hmatrix10_EF;
   std::vector<double>  *eg_IsolationLikelihood_EF;
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
   std::vector<float>   *eg_bremInvpT_EF;
   std::vector<float>   *eg_bremRadius_EF;
   std::vector<float>   *eg_bremX_EF;
   std::vector<float>   *eg_bremClusterRadius_EF;
   std::vector<float>   *eg_bremInvpTerr_EF;
   UInt_t          EFBphys_NPar;
   std::vector<int>     *EFBphys_roiId;
   std::vector<float>   *EFBphys_eta;
   std::vector<float>   *EFBphys_phi;
   std::vector<int>     *EFBphys_pType;
   std::vector<float>   *EFBphys_mass;
   std::vector<float>   *EFBphys_fitmass;
   std::vector<float>   *EFBphys_fitchis;
   std::vector<float>   *EFBphys_fitprob;
   std::vector<float>   *EFBphys_secmass;
   std::vector<float>   *EFBphys_secfitmass;
   std::vector<float>   *EFBphys_secfitchis;
   std::vector<float>   *EFBphys_secfitprob;
   UInt_t          T2BjN;
   std::vector<float>   *T2BjEta;
   std::vector<float>   *T2BjPhi0;
   std::vector<float>   *T2BjPvtx;
   std::vector<float>   *T2BjPtJet;
   std::vector<float>   *T2BjXIP1D;
   std::vector<float>   *T2BjXIP2D;
   std::vector<float>   *T2BjXIP3D;
   std::vector<float>   *T2BjXChi2;
   std::vector<float>   *T2BjXSV;
   std::vector<float>   *T2BjXmvtx;
   std::vector<float>   *T2BjXevtx;
   std::vector<float>   *T2BjXnvtx;
   UInt_t          EFBjN;
   std::vector<float>   *EFBjEta;
   std::vector<float>   *EFBjPhi0;
   std::vector<float>   *EFBjPvtx;
   std::vector<float>   *EFBjPtJet;
   std::vector<float>   *EFBjXIP1D;
   std::vector<float>   *EFBjXIP2D;
   std::vector<float>   *EFBjXIP3D;
   std::vector<float>   *EFBjXChi2;
   std::vector<float>   *EFBjXSV;
   std::vector<float>   *EFBjXmvtx;
   std::vector<float>   *EFBjXevtx;
   std::vector<float>   *EFBjXnvtx;
   Int_t           tau_L2ntaus;
   std::vector<float>   *tau_L2eta;
   std::vector<float>   *tau_L2phi;
   std::vector<float>   *tau_L2pt;
   std::vector<float>   *tau_L2Zvtx;
   std::vector<float>   *tau_L2nMatchTracks;
   std::vector<float>   *tau_L2roiId;
   Float_t         MemRSS;
   Float_t         MemVSize;
   Float_t         TimeTotal;
   Float_t         TimeSum;
   Float_t         TimeEventCounter;
   Float_t         TimeInDetPixelClusterization;
   Float_t         TimeInDetSCT_Clusterization;
   Float_t         TimeInDetSiTrackerSpacePointFinder;
   Float_t         TimeInDetTRT_RIO_Maker;
   Float_t         TimeInDetPRD_MultiTruthMaker;
   Float_t         TimeInDetSiSpTrackFinder;
   Float_t         TimeSiSPSeededTracksDetailedTruthMaker;
   Float_t         TimeSiSPSeededTracksTruthCollectionSelector;
   Float_t         TimeInDetAmbiguitySolver;
   Float_t         TimeResolvedTracksDetailedTruthMaker;
   Float_t         TimeResolvedTracksTruthCollectionSelector;
   Float_t         TimeInDetTRT_Extension;
   Float_t         TimeInDetExtensionProcessor;
   Float_t         TimeExtendedTracksDetailedTruthMaker;
   Float_t         TimeExtendedTracksTruthCollectionSelector;
   Float_t         TimeInDetSegmentPRD_Association;
   Float_t         TimeInDetTRT_TrackSegmentsFinder;
   Float_t         TimeInDetTRT_SeededTrackFinder;
   Float_t         TimeTRTSeededTracksDetailedTruthMaker;
   Float_t         TimeTRTSeededTracksTruthCollectionSelector;
   Float_t         TimeInDetTRT_SeededAmbiguitySolver;
   Float_t         TimeResolvedTRTSeededTracksDetailedTruthMaker;
   Float_t         TimeResolvedTRTSeededTracksTruthCollectionSelector;
   Float_t         TimeInDetTRTonly_PRD_Association;
   Float_t         TimeInDetTRT_StandaloneTrackFinder;
   Float_t         TimeTRTStandaloneTracksDetailedTruthMaker;
   Float_t         TimeTRTStandaloneTracksTruthCollectionSelector;
   Float_t         TimeInDetTrackCollectionMerger;
   Float_t         TimeCombinedInDetTracksDetailedTruthMaker;
   Float_t         TimeCombinedInDetTracksTruthCollectionSelector;
   Float_t         TimeInDetTrackSlimmer;
   Float_t         TimeDetailedTrackTruthMaker;
   Float_t         TimeTrackTruthCollectionSelector;
   Float_t         TimeInDetPriVxFinder;
   Float_t         TimeInDetParticleCreation;
   Float_t         TimeInDetTrackParticleTruthMaker;
   Float_t         TimeInDetSecVtxFinder;
   Float_t         TimeInDetConversionFinder;
   Float_t         TimeInDetRecStatistics;
   Float_t         TimeInDetSegmentDriftCircleAssValidation;
   Float_t         TimeCaloCellMaker;
   Float_t         TimeCaloClusterTopoEM430Getter;
   Float_t         TimeEMCell2TopoCluster430;
   Float_t         TimeCscRdoToCscPrepData;
   Float_t         TimeMdtRdoToMdtPrepData;
   Float_t         TimeRpcRdoToRpcPrepData;
   Float_t         TimeTgcRdoToTgcPrepData;
   Float_t         TimeCscThresholdClusterBuilder;
   Float_t         TimeTrackRecordFilter;
   Float_t         TimeTileLookForMuAlg;
   Float_t         TimeInitializeMuonClusters;
   Float_t         TimeCaloTrkMuIdAlg;
   Float_t         TimeMuGirl;
   Float_t         TimeMuidStatistics;
   Float_t         TimeMergeMuonCollections;
   Float_t         TimeMakeAODMuons;
   Float_t         TimeFinalizeMuonClusters;
   Float_t         TimeBTagSharedHitMapper;
   Float_t         TimeCmbTowerBldr;
   Float_t         TimeTowerJetInput;
   Float_t         TimeCone7H1TowerJets;
   Float_t         TimeCone4H1TowerJets;
   Float_t         TimeCone4H1TopoJets;
   Float_t         TimeEsdTruthParticlesBuilder;
   Float_t         TimeCone4TruthJets;
   Float_t         TimeTrigConfDataIOVChanger;
   Float_t         TimeTriggerTowerMaker;
   Float_t         TimeCPMTowerMaker;
   Float_t         TimeJetElementMaker;
   Float_t         TimeEmTauTrigger;
   Float_t         TimeJetTrigger;
   Float_t         TimeEnergyTrigger;
   Float_t         TimeROD;
   Float_t         TimeCPCMMMaker;
   Float_t         TimeJEPCMMMaker;
   Float_t         TimeLVL1TrigT1MBTS;
   Float_t         TimeMuonRdoToMuonDigit;
   Float_t         TimeTrigT1RPC;
   Float_t         TimeLVL1TGCTriggerLVL1TGCTrigger;
   Float_t         TimeL1Muctpi;
   Float_t         TimeLVL1TrigT1BCM;
   Float_t         TimeLVL1TrigT1Lucid;
   Float_t         TimeCTPSimulation;
   Float_t         TimeRoIBuilder;
   Float_t         TimeRoIBResultToAOD;
   Float_t         TimeStreamBS;
   Float_t         TimeTrigSteer_L2;
   Float_t         TimeTrigSteer_EF;
   Float_t         TimeTrigDecMaker;
   Float_t         TimeTrigInDetTrackTruthMaker;
   Float_t         TimeCBNT_AthenaAware;
   Float_t         TimeCreateLumiBlockCollectionFromFile;
   Float_t         TimeStreamESD;
   Float_t         TimeStreamESD_FH;
   Float_t         TimeAANTupleStream;
   Float_t         TimeDumpSp;

   // List of branches
   TBranch        *b_RunNumber;   //!
   TBranch        *b_EventNumber;   //!
   TBranch        *b_StreamESD_ref;   //!
   TBranch        *b_Token;   //!
   TBranch        *b_Run;   //!
   TBranch        *b_Event;   //!
   TBranch        *b_Time;   //!
   TBranch        *b_LumiBlock;   //!
   TBranch        *b_BCID;   //!
   TBranch        *b_LVL1ID;   //!
   TBranch        *b_Weight;   //!
   TBranch        *b_IEvent;   //!
   TBranch        *b_StatusElement;   //!
   TBranch        *b_LVL1TriggerType;   //!
   TBranch        *b_LVL1TriggerInfo;   //!
   TBranch        *b_LVL2TriggerInfo;   //!
   TBranch        *b_EventFilterInfo;   //!
   TBranch        *b_StreamTagName;   //!
   TBranch        *b_StreamTagType;   //!
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
   TBranch        *b_cl_ecluster_topoEM430;   //!
   TBranch        *b_cl_nctotal_topoEM430;   //!
   TBranch        *b_cl_nc_topoEM430;   //!
   TBranch        *b_cl_et_topoEM430;   //!
   TBranch        *b_cl_e_topoEM430;   //!
   TBranch        *b_cl_eemb0_topoEM430;   //!
   TBranch        *b_cl_eemb1_topoEM430;   //!
   TBranch        *b_cl_eemb2_topoEM430;   //!
   TBranch        *b_cl_eemb3_topoEM430;   //!
   TBranch        *b_cl_nemb0_topoEM430;   //!
   TBranch        *b_cl_nemb1_topoEM430;   //!
   TBranch        *b_cl_nemb2_topoEM430;   //!
   TBranch        *b_cl_nemb3_topoEM430;   //!
   TBranch        *b_cl_phi2_topoEM430;   //!
   TBranch        *b_cl_eta0_topoEM430;   //!
   TBranch        *b_cl_eta1_topoEM430;   //!
   TBranch        *b_cl_eta2_topoEM430;   //!
   TBranch        *b_cl_eta3_topoEM430;   //!
   TBranch        *b_cl_eeme0_topoEM430;   //!
   TBranch        *b_cl_eeme1_topoEM430;   //!
   TBranch        *b_cl_eeme2_topoEM430;   //!
   TBranch        *b_cl_eeme3_topoEM430;   //!
   TBranch        *b_cl_neme0_topoEM430;   //!
   TBranch        *b_cl_neme1_topoEM430;   //!
   TBranch        *b_cl_neme2_topoEM430;   //!
   TBranch        *b_cl_neme3_topoEM430;   //!
   TBranch        *b_cl_etileg1_topoEM430;   //!
   TBranch        *b_cl_etileg2_topoEM430;   //!
   TBranch        *b_cl_etileg3_topoEM430;   //!
   TBranch        *b_cl_ntileg1_topoEM430;   //!
   TBranch        *b_cl_ntileg2_topoEM430;   //!
   TBranch        *b_cl_ntileg3_topoEM430;   //!
   TBranch        *b_cl_eta_topoEM430;   //!
   TBranch        *b_cl_phi_topoEM430;   //!
   TBranch        *b_cl_reco_stat_topoEM430;   //!
   TBranch        *b_cl_m1_eta_topoEM430;   //!
   TBranch        *b_cl_m1_phi_topoEM430;   //!
   TBranch        *b_cl_m2_r_topoEM430;   //!
   TBranch        *b_cl_m2_lambda_topoEM430;   //!
   TBranch        *b_cl_delta_phi_topoEM430;   //!
   TBranch        *b_cl_delta_theta_topoEM430;   //!
   TBranch        *b_cl_delta_alpha_topoEM430;   //!
   TBranch        *b_cl_center_x_topoEM430;   //!
   TBranch        *b_cl_center_y_topoEM430;   //!
   TBranch        *b_cl_center_z_topoEM430;   //!
   TBranch        *b_cl_center_lambda_topoEM430;   //!
   TBranch        *b_cl_lateral_topoEM430;   //!
   TBranch        *b_cl_longitudinal_topoEM430;   //!
   TBranch        *b_cl_eng_frac_em_topoEM430;   //!
   TBranch        *b_cl_eng_frac_max_topoEM430;   //!
   TBranch        *b_cl_eng_frac_core_topoEM430;   //!
   TBranch        *b_cl_m1_dens_topoEM430;   //!
   TBranch        *b_cl_m2_dens_topoEM430;   //!
   TBranch        *b_cl_isol_topoEM430;   //!
   TBranch        *b_cl_ntotcells_topoEM430;   //!
   TBranch        *b_Cone7H1TowerJetsjetNum;   //!
   TBranch        *b_jetEtaCone7H1TowerJets;   //!
   TBranch        *b_jetPhiCone7H1TowerJets;   //!
   TBranch        *b_jetECone7H1TowerJets;   //!
   TBranch        *b_jetEtCone7H1TowerJets;   //!
   TBranch        *b_jetMCone7H1TowerJets;   //!
   TBranch        *b_jetPxCone7H1TowerJets;   //!
   TBranch        *b_jetPyCone7H1TowerJets;   //!
   TBranch        *b_jetPzCone7H1TowerJets;   //!
   TBranch        *b_jetSizeCone7H1TowerJets;   //!
   TBranch        *b_jetEmfCone7H1TowerJets;   //!
   TBranch        *b_Cone4H1TopoJetsjetNum;   //!
   TBranch        *b_jetEtaCone4H1TopoJets;   //!
   TBranch        *b_jetPhiCone4H1TopoJets;   //!
   TBranch        *b_jetECone4H1TopoJets;   //!
   TBranch        *b_jetEtCone4H1TopoJets;   //!
   TBranch        *b_jetMCone4H1TopoJets;   //!
   TBranch        *b_jetPxCone4H1TopoJets;   //!
   TBranch        *b_jetPyCone4H1TopoJets;   //!
   TBranch        *b_jetPzCone4H1TopoJets;   //!
   TBranch        *b_jetSizeCone4H1TopoJets;   //!
   TBranch        *b_jetEmfCone4H1TopoJets;   //!
   TBranch        *b_Cone4H1TowerJetsjetNum;   //!
   TBranch        *b_jetEtaCone4H1TowerJets;   //!
   TBranch        *b_jetPhiCone4H1TowerJets;   //!
   TBranch        *b_jetECone4H1TowerJets;   //!
   TBranch        *b_jetEtCone4H1TowerJets;   //!
   TBranch        *b_jetMCone4H1TowerJets;   //!
   TBranch        *b_jetPxCone4H1TowerJets;   //!
   TBranch        *b_jetPyCone4H1TowerJets;   //!
   TBranch        *b_jetPzCone4H1TowerJets;   //!
   TBranch        *b_jetSizeCone4H1TowerJets;   //!
   TBranch        *b_jetEmfCone4H1TowerJets;   //!
   TBranch        *b_Cone4TruthJetsjetNum;   //!
   TBranch        *b_jetEtaCone4TruthJets;   //!
   TBranch        *b_jetPhiCone4TruthJets;   //!
   TBranch        *b_jetECone4TruthJets;   //!
   TBranch        *b_jetEtCone4TruthJets;   //!
   TBranch        *b_jetMCone4TruthJets;   //!
   TBranch        *b_jetPxCone4TruthJets;   //!
   TBranch        *b_jetPyCone4TruthJets;   //!
   TBranch        *b_jetPzCone4TruthJets;   //!
   TBranch        *b_jetSizeCone4TruthJets;   //!
   TBranch        *b_jetEmfCone4TruthJets;   //!
   TBranch        *b_Cone4TruthPileupJetsjetNum;   //!
   TBranch        *b_jetEtaCone4TruthPileupJets;   //!
   TBranch        *b_jetPhiCone4TruthPileupJets;   //!
   TBranch        *b_jetECone4TruthPileupJets;   //!
   TBranch        *b_jetEtCone4TruthPileupJets;   //!
   TBranch        *b_jetMCone4TruthPileupJets;   //!
   TBranch        *b_jetPxCone4TruthPileupJets;   //!
   TBranch        *b_jetPyCone4TruthPileupJets;   //!
   TBranch        *b_jetPzCone4TruthPileupJets;   //!
   TBranch        *b_jetSizeCone4TruthPileupJets;   //!
   TBranch        *b_jetEmfCone4TruthPileupJets;   //!
   TBranch        *b_NTileMuonTag;   //!
   TBranch        *b_EtaTileMuonTag;   //!
   TBranch        *b_PhiTileMuonTag;   //!
   TBranch        *b_EnergyVecTileMuonTag;   //!
   TBranch        *b_LabelTileMuonTag;   //!
   TBranch        *b__ntrack;   //!
   TBranch        *b_CaloMu_trkPdgid;   //!
   TBranch        *b_CaloMu_trkMother;   //!
   TBranch        *b_CaloMu_trkEta;   //!
   TBranch        *b_CaloMu_trkPhi;   //!
   TBranch        *b_CaloMu_trkPt;   //!
   TBranch        *b_CaloMu_trkD0;   //!
   TBranch        *b_CaloMu_trkIP;   //!
   TBranch        *b_CaloMu_trkBLayerHits;   //!
   TBranch        *b_CaloMu_trkPixelHits;   //!
   TBranch        *b_CaloMu_trkSCTHits;   //!
   TBranch        *b_CaloMu_trkTRTHits;   //!
   TBranch        *b__nmuon;   //!
   TBranch        *b_CaloMu_pdgid;   //!
   TBranch        *b_CaloMu_mother;   //!
   TBranch        *b_CaloMu_tag;   //!
   TBranch        *b_CaloMu_lhr;   //!
   TBranch        *b_CaloMu_etCone;   //!
   TBranch        *b_CaloMu_ptCone;   //!
   TBranch        *b_CaloMu_eta;   //!
   TBranch        *b_CaloMu_phi;   //!
   TBranch        *b_CaloMu_pt;   //!
   TBranch        *b_CaloMu_qoverp;   //!
   TBranch        *b_CaloMu_IP;   //!
   TBranch        *b_CaloMu_d0;   //!
   TBranch        *b_CaloMu_bLayerHits;   //!
   TBranch        *b_CaloMu_pixelHits;   //!
   TBranch        *b_CaloMu_SCTHits;   //!
   TBranch        *b_CaloMu_TRTHits;   //!
   TBranch        *b_CaloMu_sample;   //!
   TBranch        *b_CaloMu_energy;   //!
   TBranch        *b_CaloMu_energyLoss;   //!
   TBranch        *b_MuTagIMO__nmuon;   //!
   TBranch        *b_MuTagIMO__nTagSegs;   //!
   TBranch        *b_MuTagIMO_trk_isTagged;   //!
   TBranch        *b_MuTagIMO_trk_matchesBarCode;   //!
   TBranch        *b_MuTagIMO_trk_matchesPdg;   //!
   TBranch        *b_MuTagIMO_trk_nrPixelHits;   //!
   TBranch        *b_MuTagIMO_trk_nrSctHits;   //!
   TBranch        *b_MuTagIMO_trk_A0;   //!
   TBranch        *b_MuTagIMO_trk_Z0;   //!
   TBranch        *b_MuTagIMO_trk_phi;   //!
   TBranch        *b_MuTagIMO_trk_theta;   //!
   TBranch        *b_MuTagIMO_trk_qOverP;   //!
   TBranch        *b_MuTagIMO_trk_chi2;   //!
   TBranch        *b_MuTagIMO_trk_ndof;   //!
   TBranch        *b_MuTagIMO_trk_Author;   //!
   TBranch        *b_MuTagIMO_trk_cov11;   //!
   TBranch        *b_MuTagIMO_trk_cov12;   //!
   TBranch        *b_MuTagIMO_trk_cov13;   //!
   TBranch        *b_MuTagIMO_trk_cov14;   //!
   TBranch        *b_MuTagIMO_trk_cov15;   //!
   TBranch        *b_MuTagIMO_trk_cov22;   //!
   TBranch        *b_MuTagIMO_trk_cov23;   //!
   TBranch        *b_MuTagIMO_trk_cov24;   //!
   TBranch        *b_MuTagIMO_trk_cov25;   //!
   TBranch        *b_MuTagIMO_trk_cov33;   //!
   TBranch        *b_MuTagIMO_trk_cov34;   //!
   TBranch        *b_MuTagIMO_trk_cov35;   //!
   TBranch        *b_MuTagIMO_trk_cov44;   //!
   TBranch        *b_MuTagIMO_trk_cov45;   //!
   TBranch        *b_MuTagIMO_trk_cov55;   //!
   TBranch        *b_MuTagIMO_exTrk_trackIndex;   //!
   TBranch        *b_MuTagIMO_exTrk_A0;   //!
   TBranch        *b_MuTagIMO_exTrk_Z0;   //!
   TBranch        *b_MuTagIMO_exTrk_phi;   //!
   TBranch        *b_MuTagIMO_exTrk_theta;   //!
   TBranch        *b_MuTagIMO_exTrk_qOverP;   //!
   TBranch        *b_MuTagIMO_exTrk_cov11;   //!
   TBranch        *b_MuTagIMO_exTrk_cov12;   //!
   TBranch        *b_MuTagIMO_exTrk_cov13;   //!
   TBranch        *b_MuTagIMO_exTrk_cov14;   //!
   TBranch        *b_MuTagIMO_exTrk_cov15;   //!
   TBranch        *b_MuTagIMO_exTrk_cov22;   //!
   TBranch        *b_MuTagIMO_exTrk_cov23;   //!
   TBranch        *b_MuTagIMO_exTrk_cov24;   //!
   TBranch        *b_MuTagIMO_exTrk_cov25;   //!
   TBranch        *b_MuTagIMO_exTrk_cov33;   //!
   TBranch        *b_MuTagIMO_exTrk_cov34;   //!
   TBranch        *b_MuTagIMO_exTrk_cov35;   //!
   TBranch        *b_MuTagIMO_exTrk_cov44;   //!
   TBranch        *b_MuTagIMO_exTrk_cov45;   //!
   TBranch        *b_MuTagIMO_exTrk_cov55;   //!
   TBranch        *b_MuTagIMO_exTrk_Eloss;   //!
   TBranch        *b_MuTagIMO_exTrk_gPosX;   //!
   TBranch        *b_MuTagIMO_exTrk_gPosY;   //!
   TBranch        *b_MuTagIMO_exTrk_gPosZ;   //!
   TBranch        *b_MuTagIMO_exTrk_gDirX;   //!
   TBranch        *b_MuTagIMO_exTrk_gDirY;   //!
   TBranch        *b_MuTagIMO_exTrk_gDirZ;   //!
   TBranch        *b_MuTagIMO_exTrk_locX;   //!
   TBranch        *b_MuTagIMO_exTrk_locY;   //!
   TBranch        *b_MuTagIMO_exTrk_angleXZ;   //!
   TBranch        *b_MuTagIMO_exTrk_angleYZ;   //!
   TBranch        *b_MuTagIMO_exTrk_locX_error;   //!
   TBranch        *b_MuTagIMO_exTrk_locY_error;   //!
   TBranch        *b_MuTagIMO_exTrk_angleXZ_error;   //!
   TBranch        *b_MuTagIMO_exTrk_angleYZ_error;   //!
   TBranch        *b_MuTagIMO_seg_isTagSeg;   //!
   TBranch        *b_MuTagIMO_seg_matchesBarCode;   //!
   TBranch        *b_MuTagIMO_seg_matchesPdg;   //!
   TBranch        *b_MuTagIMO_seg_exTrkIndex;   //!
   TBranch        *b_MuTagIMO_seg_trackIndex;   //!
   TBranch        *b_MuTagIMO_seg_stationLevel;   //!
   TBranch        *b_MuTagIMO_seg_isEndCap;   //!
   TBranch        *b_MuTagIMO_seg_isLarge;   //!
   TBranch        *b_MuTagIMO_seg_nrHoles;   //!
   TBranch        *b_MuTagIMO_seg_nrMLs;   //!
   TBranch        *b_MuTagIMO_seg_nrMdtHits;   //!
   TBranch        *b_MuTagIMO_seg_nrRpcHits;   //!
   TBranch        *b_MuTagIMO_seg_nrTgcHits;   //!
   TBranch        *b_MuTagIMO_seg_nrCscHits;   //!
   TBranch        *b_MuTagIMO_seg_phi;   //!
   TBranch        *b_MuTagIMO_seg_theta;   //!
   TBranch        *b_MuTagIMO_seg_chi2;   //!
   TBranch        *b_MuTagIMO_seg_cov11;   //!
   TBranch        *b_MuTagIMO_seg_cov12;   //!
   TBranch        *b_MuTagIMO_seg_cov13;   //!
   TBranch        *b_MuTagIMO_seg_cov14;   //!
   TBranch        *b_MuTagIMO_seg_cov15;   //!
   TBranch        *b_MuTagIMO_seg_cov22;   //!
   TBranch        *b_MuTagIMO_seg_cov23;   //!
   TBranch        *b_MuTagIMO_seg_cov24;   //!
   TBranch        *b_MuTagIMO_seg_cov25;   //!
   TBranch        *b_MuTagIMO_seg_cov33;   //!
   TBranch        *b_MuTagIMO_seg_cov34;   //!
   TBranch        *b_MuTagIMO_seg_cov35;   //!
   TBranch        *b_MuTagIMO_seg_cov44;   //!
   TBranch        *b_MuTagIMO_seg_cov45;   //!
   TBranch        *b_MuTagIMO_seg_cov55;   //!
   TBranch        *b_MuTagIMO_seg_gPosX;   //!
   TBranch        *b_MuTagIMO_seg_gPosY;   //!
   TBranch        *b_MuTagIMO_seg_gPosZ;   //!
   TBranch        *b_MuTagIMO_seg_gDirX;   //!
   TBranch        *b_MuTagIMO_seg_gDirY;   //!
   TBranch        *b_MuTagIMO_seg_gDirZ;   //!
   TBranch        *b_MuTagIMO_seg_locX;   //!
   TBranch        *b_MuTagIMO_seg_locY;   //!
   TBranch        *b_MuTagIMO_seg_angleXZ;   //!
   TBranch        *b_MuTagIMO_seg_angleYZ;   //!
   TBranch        *b_MuTagIMO_seg_locX_error;   //!
   TBranch        *b_MuTagIMO_seg_locY_error;   //!
   TBranch        *b_MuTagIMO_seg_angleXZ_error;   //!
   TBranch        *b_MuTagIMO_seg_angleYZ_error;   //!
   TBranch        *b_MuTagIMO_match_trackIndex;   //!
   TBranch        *b_MuTagIMO_match_locX;   //!
   TBranch        *b_MuTagIMO_match_locY;   //!
   TBranch        *b_MuTagIMO_match_angleXZ;   //!
   TBranch        *b_MuTagIMO_match_angleYZ;   //!
   TBranch        *b_MuTagIMO_truthID_barCode;   //!
   TBranch        *b_MuTagIMO_truthID_pdg;   //!
   TBranch        *b_MuTagIMO_truthID_posX;   //!
   TBranch        *b_MuTagIMO_truthID_posY;   //!
   TBranch        *b_MuTagIMO_truthID_posZ;   //!
   TBranch        *b_MuTagIMO_truthID_momX;   //!
   TBranch        *b_MuTagIMO_truthID_momY;   //!
   TBranch        *b_MuTagIMO_truthID_momZ;   //!
   TBranch        *b_MuTagIMO_truthID_energy;   //!
   TBranch        *b_MuTagIMO_truthMS_barCode;   //!
   TBranch        *b_MuTagIMO_truthMS_pdg;   //!
   TBranch        *b_MuTagIMO_truthMS_posX;   //!
   TBranch        *b_MuTagIMO_truthMS_posY;   //!
   TBranch        *b_MuTagIMO_truthMS_posZ;   //!
   TBranch        *b_MuTagIMO_truthMS_momX;   //!
   TBranch        *b_MuTagIMO_truthMS_momY;   //!
   TBranch        *b_MuTagIMO_truthMS_momZ;   //!
   TBranch        *b_MuTagIMO_truthMS_energy;   //!
   TBranch        *b_MuTagIMO_trHit_id;   //!
   TBranch        *b_MuTagIMO_trHit_barCode;   //!
   TBranch        *b_MuTagIMO_trHit_r;   //!
   TBranch        *b_MuTagIMO_trHit_alongTube;   //!
   TBranch        *b_L1EM_Scale;   //!
   TBranch        *b_L1Em_nRoI;   //!
   TBranch        *b_L1Em_RoIWord;   //!
   TBranch        *b_L1Em_Core;   //!
   TBranch        *b_L1Em_EmClus;   //!
   TBranch        *b_L1Em_TauClus;   //!
   TBranch        *b_L1Em_EmIsol;   //!
   TBranch        *b_L1Em_HdIsol;   //!
   TBranch        *b_L1Em_HdCore;   //!
   TBranch        *b_L1Em_EmThresh;   //!
   TBranch        *b_L1Em_TauThresh;   //!
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
   TBranch        *b_nCTP_ROI;   //!
   TBranch        *b_CTP_ROI;   //!
   TBranch        *b_nMuCTPI_ROI;   //!
   TBranch        *b_muCTPI_ROI;   //!
   TBranch        *b_nEMTau_ROI;   //!
   TBranch        *b_EMTau_ROI;   //!
   TBranch        *b_nJetEnergy_ROI;   //!
   TBranch        *b_JetEnergy_ROI;   //!
   TBranch        *b_L1CaloPPM_ntt;   //!
   TBranch        *b_L1CaloPPM_eta;   //!
   TBranch        *b_L1CaloPPM_phi;   //!
   TBranch        *b_L1CaloPPM_emCrate;   //!
   TBranch        *b_L1CaloPPM_emModule;   //!
   TBranch        *b_L1CaloPPM_emSubmodule;   //!
   TBranch        *b_L1CaloPPM_emChannel;   //!
   TBranch        *b_L1CaloPPM_hadCrate;   //!
   TBranch        *b_L1CaloPPM_hadModule;   //!
   TBranch        *b_L1CaloPPM_hadSubmodule;   //!
   TBranch        *b_L1CaloPPM_hadChannel;   //!
   TBranch        *b_L1CaloPPM_emChannelId;   //!
   TBranch        *b_L1CaloPPM_hadChannelId;   //!
   TBranch        *b_L1CaloPPM_emTTCellsEnergy;   //!
   TBranch        *b_L1CaloPPM_hadTTCellsEnergy;   //!
   TBranch        *b_L1CaloPPM_emCaloWaves;   //!
   TBranch        *b_L1CaloPPM_hadCaloWaves;   //!
   TBranch        *b_L1CaloPPM_emPeak;   //!
   TBranch        *b_L1CaloPPM_emADCPeak;   //!
   TBranch        *b_L1CaloPPM_hadPeak;   //!
   TBranch        *b_L1CaloPPM_hadADCPeak;   //!
   TBranch        *b_L1CaloPPM_emLUT;   //!
   TBranch        *b_L1CaloPPM_hadLUT;   //!
   TBranch        *b_L1CaloPPM_emADC;   //!
   TBranch        *b_L1CaloPPM_hadADC;   //!
   TBranch        *b_L1CaloPPM_emEnergy;   //!
   TBranch        *b_L1CaloPPM_hadEnergy;   //!
   TBranch        *b_L1CaloPPM_emBCIDvec;   //!
   TBranch        *b_L1CaloPPM_emBCIDext;   //!
   TBranch        *b_L1CaloPPM_hadBCIDvec;   //!
   TBranch        *b_L1CaloPPM_hadBCIDext;   //!
   TBranch        *b_L1CaloPPM_emError;   //!
   TBranch        *b_L1CaloPPM_hadError;   //!
   TBranch        *b_L1CaloPPM_emBCID;   //!
   TBranch        *b_L1CaloPPM_hadBCID;   //!
   TBranch        *b_L1CaloPPM_emIsSaturated;   //!
   TBranch        *b_L1CaloPPM_hadIsSaturated;   //!
   TBranch        *b_L1CaloPPM_emIsFilled;   //!
   TBranch        *b_L1CaloPPM_hadIsFilled;   //!
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
   TBranch        *b_T2CaEtas1;   //!
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
   TBranch        *b_T2CaNjets;   //!
   TBranch        *b_T2CaJetE;   //!
   TBranch        *b_T2CaJetEhad0;   //!
   TBranch        *b_T2CaJetEem0;   //!
   TBranch        *b_T2CaJeteta;   //!
   TBranch        *b_T2CaJetphi;   //!
   TBranch        *b_T2CaJetroi;   //!
   TBranch        *b_TrigT2MissingEt;   //!
   TBranch        *b_TrigT2MissingEtEt;   //!
   TBranch        *b_TrigT2MissingEtEx;   //!
   TBranch        *b_TrigT2MissingEtEy;   //!
   TBranch        *b_TrigT2MissingEtsumEt;   //!
   TBranch        *b_TrigT2MissingEtRoIword;   //!
   TBranch        *b_TrigEFMissingEt;   //!
   TBranch        *b_TrigEFMissingEtEt;   //!
   TBranch        *b_TrigEFMissingEtEx;   //!
   TBranch        *b_TrigEFMissingEtEy;   //!
   TBranch        *b_TrigEFMissingEtsumEt;   //!
   TBranch        *b_TrigEFMissingEtRoIword;   //!
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
   TBranch        *b_T2IdEtac;   //!
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
   TBranch        *b_TrkEF_truthBarcode;   //!
   TBranch        *b_TrkEF_truthNt;   //!
   TBranch        *b_TrkEF_ParticlePdg;   //!
   TBranch        *b_TrkEF_ParentPdg;   //!
   TBranch        *b_TrkEF_NumSiKineHits;   //!
   TBranch        *b_TrkEF_NumTRTKineHits;   //!
   TBranch        *b_TrkEF_sctSP;   //!
   TBranch        *b_TrkEF_pixSP;   //!
   TBranch        *b_VxEF_numVertices;   //!
   TBranch        *b_VxEF_vtx_x;   //!
   TBranch        *b_VxEF_vtx_y;   //!
   TBranch        *b_VxEF_vtx_z;   //!
   TBranch        *b_VxEF_numOfTruthVertices;   //!
   TBranch        *b_VxEF_vtx_x_truth;   //!
   TBranch        *b_VxEF_vtx_y_truth;   //!
   TBranch        *b_VxEF_vtx_z_truth;   //!
   TBranch        *b_VxEF_vtx_x_res;   //!
   TBranch        *b_VxEF_vtx_y_res;   //!
   TBranch        *b_VxEF_vtx_z_res;   //!
   TBranch        *b_VxEF_sigvtx_x;   //!
   TBranch        *b_VxEF_sigvtx_y;   //!
   TBranch        *b_VxEF_sigvtx_z;   //!
   TBranch        *b_VxEF_vtxchi2;   //!
   TBranch        *b_VxEF_vtxndf;   //!
   TBranch        *b_VxEF_pt;   //!
   TBranch        *b_VxEF_vtxchi2prob;   //!
   TBranch        *b_VxEF_numTracksPerVertex;   //!
   TBranch        *b_VxEF_vtxType;   //!
   TBranch        *b_VxEF_totalNumTracks;   //!
   TBranch        *b_VxEF_chi2;   //!
   TBranch        *b_VxEF_d0;   //!
   TBranch        *b_VxEF_sigd0;   //!
   TBranch        *b_VxEF_z0;   //!
   TBranch        *b_VxEF_sigz0;   //!
   TBranch        *b_VxEF_phi;   //!
   TBranch        *b_VxEF_sigphi;   //!
   TBranch        *b_VxEF_theta;   //!
   TBranch        *b_VxEF_sigtheta;   //!
   TBranch        *b_VxEF_qOverP;   //!
   TBranch        *b_VxEF_sigqOverP;   //!
   TBranch        *b_VxEF_d0g;   //!
   TBranch        *b_VxEF_sigd0g;   //!
   TBranch        *b_VxEF_z0g;   //!
   TBranch        *b_VxEF_sigz0g;   //!
   TBranch        *b_eg_RoiId_EF;   //!
   TBranch        *b_eg_TrigKey_EF;   //!
   TBranch        *b_eg_nc_EF;   //!
   TBranch        *b_eg_e_EF;   //!
   TBranch        *b_eg_eta_EF;   //!
   TBranch        *b_eg_phi_EF;   //!
   TBranch        *b_eg_author_EF;   //!
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
   TBranch        *b_eg_Hmatrix5_EF;   //!
   TBranch        *b_eg_Hmatrix10_EF;   //!
   TBranch        *b_eg_IsolationLikelihood_EF;   //!
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
   TBranch        *b_eg_bremInvpT_EF;   //!
   TBranch        *b_eg_bremRadius_EF;   //!
   TBranch        *b_eg_bremX_EF;   //!
   TBranch        *b_eg_bremClusterRadius_EF;   //!
   TBranch        *b_eg_bremInvpTerr_EF;   //!
   TBranch        *b_EFBphys_NPar;   //!
   TBranch        *b_EFBphys_roiId;   //!
   TBranch        *b_EFBphys_eta;   //!
   TBranch        *b_EFBphys_phi;   //!
   TBranch        *b_EFBphys_pType;   //!
   TBranch        *b_EFBphys_mass;   //!
   TBranch        *b_EFBphys_fitmass;   //!
   TBranch        *b_EFBphys_fitchis;   //!
   TBranch        *b_EFBphys_fitprob;   //!
   TBranch        *b_EFBphys_secmass;   //!
   TBranch        *b_EFBphys_secfitmass;   //!
   TBranch        *b_EFBphys_secfitchis;   //!
   TBranch        *b_EFBphys_secfitprob;   //!
   TBranch        *b_T2BjN;   //!
   TBranch        *b_T2BjEta;   //!
   TBranch        *b_T2BjPhi0;   //!
   TBranch        *b_T2BjPvtx;   //!
   TBranch        *b_T2BjPtJet;   //!
   TBranch        *b_T2BjXIP1D;   //!
   TBranch        *b_T2BjXIP2D;   //!
   TBranch        *b_T2BjXIP3D;   //!
   TBranch        *b_T2BjXChi2;   //!
   TBranch        *b_T2BjXSV;   //!
   TBranch        *b_T2BjXmvtx;   //!
   TBranch        *b_T2BjXevtx;   //!
   TBranch        *b_T2BjXnvtx;   //!
   TBranch        *b_EFBjN;   //!
   TBranch        *b_EFBjEta;   //!
   TBranch        *b_EFBjPhi0;   //!
   TBranch        *b_EFBjPvtx;   //!
   TBranch        *b_EFBjPtJet;   //!
   TBranch        *b_EFBjXIP1D;   //!
   TBranch        *b_EFBjXIP2D;   //!
   TBranch        *b_EFBjXIP3D;   //!
   TBranch        *b_EFBjXChi2;   //!
   TBranch        *b_EFBjXSV;   //!
   TBranch        *b_EFBjXmvtx;   //!
   TBranch        *b_EFBjXevtx;   //!
   TBranch        *b_EFBjXnvtx;   //!
   TBranch        *b_tau_ntaus;   //!
   TBranch        *b_tau_L2eta;   //!
   TBranch        *b_tau_L2phi;   //!
   TBranch        *b_tau_L2pt;   //!
   TBranch        *b_tau_L2Zvtx;   //!
   TBranch        *b_tau_L2nMatchTracks;   //!
   TBranch        *b_tau_L2roiId;   //!
   TBranch        *b_MemRSS;   //!
   TBranch        *b_MemVSize;   //!
   TBranch        *b_TimeTotal;   //!
   TBranch        *b_TimeSum;   //!
   TBranch        *b_TimeEventCounter;   //!
   TBranch        *b_TimeInDetPixelClusterization;   //!
   TBranch        *b_TimeInDetSCT_Clusterization;   //!
   TBranch        *b_TimeInDetSiTrackerSpacePointFinder;   //!
   TBranch        *b_TimeInDetTRT_RIO_Maker;   //!
   TBranch        *b_TimeInDetPRD_MultiTruthMaker;   //!
   TBranch        *b_TimeInDetSiSpTrackFinder;   //!
   TBranch        *b_TimeSiSPSeededTracksDetailedTruthMaker;   //!
   TBranch        *b_TimeSiSPSeededTracksTruthCollectionSelector;   //!
   TBranch        *b_TimeInDetAmbiguitySolver;   //!
   TBranch        *b_TimeResolvedTracksDetailedTruthMaker;   //!
   TBranch        *b_TimeResolvedTracksTruthCollectionSelector;   //!
   TBranch        *b_TimeInDetTRT_Extension;   //!
   TBranch        *b_TimeInDetExtensionProcessor;   //!
   TBranch        *b_TimeExtendedTracksDetailedTruthMaker;   //!
   TBranch        *b_TimeExtendedTracksTruthCollectionSelector;   //!
   TBranch        *b_TimeInDetSegmentPRD_Association;   //!
   TBranch        *b_TimeInDetTRT_TrackSegmentsFinder;   //!
   TBranch        *b_TimeInDetTRT_SeededTrackFinder;   //!
   TBranch        *b_TimeTRTSeededTracksDetailedTruthMaker;   //!
   TBranch        *b_TimeTRTSeededTracksTruthCollectionSelector;   //!
   TBranch        *b_TimeInDetTRT_SeededAmbiguitySolver;   //!
   TBranch        *b_TimeResolvedTRTSeededTracksDetailedTruthMaker;   //!
   TBranch        *b_TimeResolvedTRTSeededTracksTruthCollectionSelector;   //!
   TBranch        *b_TimeInDetTRTonly_PRD_Association;   //!
   TBranch        *b_TimeInDetTRT_StandaloneTrackFinder;   //!
   TBranch        *b_TimeTRTStandaloneTracksDetailedTruthMaker;   //!
   TBranch        *b_TimeTRTStandaloneTracksTruthCollectionSelector;   //!
   TBranch        *b_TimeInDetTrackCollectionMerger;   //!
   TBranch        *b_TimeCombinedInDetTracksDetailedTruthMaker;   //!
   TBranch        *b_TimeCombinedInDetTracksTruthCollectionSelector;   //!
   TBranch        *b_TimeInDetTrackSlimmer;   //!
   TBranch        *b_TimeDetailedTrackTruthMaker;   //!
   TBranch        *b_TimeTrackTruthCollectionSelector;   //!
   TBranch        *b_TimeInDetPriVxFinder;   //!
   TBranch        *b_TimeInDetParticleCreation;   //!
   TBranch        *b_TimeInDetTrackParticleTruthMaker;   //!
   TBranch        *b_TimeInDetSecVtxFinder;   //!
   TBranch        *b_TimeInDetConversionFinder;   //!
   TBranch        *b_TimeInDetRecStatistics;   //!
   TBranch        *b_TimeInDetSegmentDriftCircleAssValidation;   //!
   TBranch        *b_TimeCaloCellMaker;   //!
   TBranch        *b_TimeCaloClusterTopoEM430Getter;   //!
   TBranch        *b_TimeEMCell2TopoCluster430;   //!
   TBranch        *b_TimeCscRdoToCscPrepData;   //!
   TBranch        *b_TimeMdtRdoToMdtPrepData;   //!
   TBranch        *b_TimeRpcRdoToRpcPrepData;   //!
   TBranch        *b_TimeTgcRdoToTgcPrepData;   //!
   TBranch        *b_TimeCscThresholdClusterBuilder;   //!
   TBranch        *b_TimeTrackRecordFilter;   //!
   TBranch        *b_TimeTileLookForMuAlg;   //!
   TBranch        *b_TimeInitializeMuonClusters;   //!
   TBranch        *b_TimeCaloTrkMuIdAlg;   //!
   TBranch        *b_TimeMuGirl;   //!
   TBranch        *b_TimeMuidStatistics;   //!
   TBranch        *b_TimeMergeMuonCollections;   //!
   TBranch        *b_TimeMakeAODMuons;   //!
   TBranch        *b_TimeFinalizeMuonClusters;   //!
   TBranch        *b_TimeBTagSharedHitMapper;   //!
   TBranch        *b_TimeCmbTowerBldr;   //!
   TBranch        *b_TimeTowerJetInput;   //!
   TBranch        *b_TimeCone7H1TowerJets;   //!
   TBranch        *b_TimeCone4H1TowerJets;   //!
   TBranch        *b_TimeCone4H1TopoJets;   //!
   TBranch        *b_TimeEsdTruthParticlesBuilder;   //!
   TBranch        *b_TimeCone4TruthJets;   //!
   TBranch        *b_TimeTrigConfDataIOVChanger;   //!
   TBranch        *b_TimeTriggerTowerMaker;   //!
   TBranch        *b_TimeCPMTowerMaker;   //!
   TBranch        *b_TimeJetElementMaker;   //!
   TBranch        *b_TimeEmTauTrigger;   //!
   TBranch        *b_TimeJetTrigger;   //!
   TBranch        *b_TimeEnergyTrigger;   //!
   TBranch        *b_TimeROD;   //!
   TBranch        *b_TimeCPCMMMaker;   //!
   TBranch        *b_TimeJEPCMMMaker;   //!
   TBranch        *b_TimeLVL1TrigT1MBTS;   //!
   TBranch        *b_TimeMuonRdoToMuonDigit;   //!
   TBranch        *b_TimeTrigT1RPC;   //!
   TBranch        *b_TimeLVL1TGCTriggerLVL1TGCTrigger;   //!
   TBranch        *b_TimeL1Muctpi;   //!
   TBranch        *b_TimeLVL1TrigT1BCM;   //!
   TBranch        *b_TimeLVL1TrigT1Lucid;   //!
   TBranch        *b_TimeCTPSimulation;   //!
   TBranch        *b_TimeRoIBuilder;   //!
   TBranch        *b_TimeRoIBResultToAOD;   //!
   TBranch        *b_TimeStreamBS;   //!
   TBranch        *b_TimeTrigSteer_L2;   //!
   TBranch        *b_TimeTrigSteer_EF;   //!
   TBranch        *b_TimeTrigDecMaker;   //!
   TBranch        *b_TimeTrigInDetTrackTruthMaker;   //!
   TBranch        *b_TimeCBNT_AthenaAware;   //!
   TBranch        *b_TimeCreateLumiBlockCollectionFromFile;   //!
   TBranch        *b_TimeStreamESD;   //!
   TBranch        *b_TimeStreamESD_FH;   //!
   TBranch        *b_TimeAANTupleStream;   //!
   TBranch        *b_TimeDumpSp;   //!

   WHTreeV14(TTree *tree=0);
   virtual ~WHTreeV14();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef WHTreeV14_cxx
WHTreeV14::WHTreeV14(TTree *tree)
{
  Init(tree);
}

WHTreeV14::~WHTreeV14()
{
  if( !fChain ) { return; }
  //if( fChain->GetCurrentFile() ) { delete fChain->GetCurrentFile(); }
}

Int_t WHTreeV14::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t WHTreeV14::LoadTree(Long64_t entry)
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

void WHTreeV14::Init(TTree *tree)
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
  LVL1TriggerInfo = 0;
  LVL2TriggerInfo = 0;
  EventFilterInfo = 0;
  StreamTagName = 0;
  StreamTagType = 0;
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
  cl_et_topoEM430 = 0;
  cl_e_topoEM430 = 0;
  cl_eemb0_topoEM430 = 0;
  cl_eemb1_topoEM430 = 0;
  cl_eemb2_topoEM430 = 0;
  cl_eemb3_topoEM430 = 0;
  cl_nemb0_topoEM430 = 0;
  cl_nemb1_topoEM430 = 0;
  cl_nemb2_topoEM430 = 0;
  cl_nemb3_topoEM430 = 0;
  cl_phi2_topoEM430 = 0;
  cl_eta0_topoEM430 = 0;
  cl_eta1_topoEM430 = 0;
  cl_eta2_topoEM430 = 0;
  cl_eta3_topoEM430 = 0;
  cl_eeme0_topoEM430 = 0;
  cl_eeme1_topoEM430 = 0;
  cl_eeme2_topoEM430 = 0;
  cl_eeme3_topoEM430 = 0;
  cl_neme0_topoEM430 = 0;
  cl_neme1_topoEM430 = 0;
  cl_neme2_topoEM430 = 0;
  cl_neme3_topoEM430 = 0;
  cl_etileg1_topoEM430 = 0;
  cl_etileg2_topoEM430 = 0;
  cl_etileg3_topoEM430 = 0;
  cl_ntileg1_topoEM430 = 0;
  cl_ntileg2_topoEM430 = 0;
  cl_ntileg3_topoEM430 = 0;
  cl_eta_topoEM430 = 0;
  cl_phi_topoEM430 = 0;
  cl_reco_stat_topoEM430 = 0;
  cl_m1_eta_topoEM430 = 0;
  cl_m1_phi_topoEM430 = 0;
  cl_m2_r_topoEM430 = 0;
  cl_m2_lambda_topoEM430 = 0;
  cl_delta_phi_topoEM430 = 0;
  cl_delta_theta_topoEM430 = 0;
  cl_delta_alpha_topoEM430 = 0;
  cl_center_x_topoEM430 = 0;
  cl_center_y_topoEM430 = 0;
  cl_center_z_topoEM430 = 0;
  cl_center_lambda_topoEM430 = 0;
  cl_lateral_topoEM430 = 0;
  cl_longitudinal_topoEM430 = 0;
  cl_eng_frac_em_topoEM430 = 0;
  cl_eng_frac_max_topoEM430 = 0;
  cl_eng_frac_core_topoEM430 = 0;
  cl_m1_dens_topoEM430 = 0;
  cl_m2_dens_topoEM430 = 0;
  cl_isol_topoEM430 = 0;
  cl_ntotcells_topoEM430 = 0;
  jetEtaCone7H1TowerJets = 0;
  jetPhiCone7H1TowerJets = 0;
  jetECone7H1TowerJets = 0;
  jetEtCone7H1TowerJets = 0;
  jetMCone7H1TowerJets = 0;
  jetPxCone7H1TowerJets = 0;
  jetPyCone7H1TowerJets = 0;
  jetPzCone7H1TowerJets = 0;
  jetSizeCone7H1TowerJets = 0;
  jetEmfCone7H1TowerJets = 0;
  jetEtaCone4H1TopoJets = 0;
  jetPhiCone4H1TopoJets = 0;
  jetECone4H1TopoJets = 0;
  jetEtCone4H1TopoJets = 0;
  jetMCone4H1TopoJets = 0;
  jetPxCone4H1TopoJets = 0;
  jetPyCone4H1TopoJets = 0;
  jetPzCone4H1TopoJets = 0;
  jetSizeCone4H1TopoJets = 0;
  jetEmfCone4H1TopoJets = 0;
  jetEtaCone4H1TowerJets = 0;
  jetPhiCone4H1TowerJets = 0;
  jetECone4H1TowerJets = 0;
  jetEtCone4H1TowerJets = 0;
  jetMCone4H1TowerJets = 0;
  jetPxCone4H1TowerJets = 0;
  jetPyCone4H1TowerJets = 0;
  jetPzCone4H1TowerJets = 0;
  jetSizeCone4H1TowerJets = 0;
  jetEmfCone4H1TowerJets = 0;
  jetEtaCone4TruthJets = 0;
  jetPhiCone4TruthJets = 0;
  jetECone4TruthJets = 0;
  jetEtCone4TruthJets = 0;
  jetMCone4TruthJets = 0;
  jetPxCone4TruthJets = 0;
  jetPyCone4TruthJets = 0;
  jetPzCone4TruthJets = 0;
  jetSizeCone4TruthJets = 0;
  jetEmfCone4TruthJets = 0;
  jetEtaCone4TruthPileupJets = 0;
  jetPhiCone4TruthPileupJets = 0;
  jetECone4TruthPileupJets = 0;
  jetEtCone4TruthPileupJets = 0;
  jetMCone4TruthPileupJets = 0;
  jetPxCone4TruthPileupJets = 0;
  jetPyCone4TruthPileupJets = 0;
  jetPzCone4TruthPileupJets = 0;
  jetSizeCone4TruthPileupJets = 0;
  jetEmfCone4TruthPileupJets = 0;
  EtaTileMuonTag = 0;
  PhiTileMuonTag = 0;
  EnergyVecTileMuonTag = 0;
  LabelTileMuonTag = 0;
  CaloMu_trkPdgid = 0;
  CaloMu_trkMother = 0;
  CaloMu_trkEta = 0;
  CaloMu_trkPhi = 0;
  CaloMu_trkPt = 0;
  CaloMu_trkD0 = 0;
  CaloMu_trkIP = 0;
  CaloMu_trkBLayerHits = 0;
  CaloMu_trkPixelHits = 0;
  CaloMu_trkSCTHits = 0;
  CaloMu_trkTRTHits = 0;
  CaloMu_pdgid = 0;
  CaloMu_mother = 0;
  CaloMu_tag = 0;
  CaloMu_lhr = 0;
  CaloMu_etCone = 0;
  CaloMu_ptCone = 0;
  CaloMu_eta = 0;
  CaloMu_phi = 0;
  CaloMu_pt = 0;
  CaloMu_qoverp = 0;
  CaloMu_IP = 0;
  CaloMu_d0 = 0;
  CaloMu_bLayerHits = 0;
  CaloMu_pixelHits = 0;
  CaloMu_SCTHits = 0;
  CaloMu_TRTHits = 0;
  CaloMu_sample = 0;
  CaloMu_energy = 0;
  CaloMu_energyLoss = 0;
  MuTagIMO_trk_isTagged = 0;
  MuTagIMO_trk_matchesBarCode = 0;
  MuTagIMO_trk_matchesPdg = 0;
  MuTagIMO_trk_nrPixelHits = 0;
  MuTagIMO_trk_nrSctHits = 0;
  MuTagIMO_trk_A0 = 0;
  MuTagIMO_trk_Z0 = 0;
  MuTagIMO_trk_phi = 0;
  MuTagIMO_trk_theta = 0;
  MuTagIMO_trk_qOverP = 0;
  MuTagIMO_trk_chi2 = 0;
  MuTagIMO_trk_ndof = 0;
  MuTagIMO_trk_Author = 0;
  MuTagIMO_trk_cov11 = 0;
  MuTagIMO_trk_cov12 = 0;
  MuTagIMO_trk_cov13 = 0;
  MuTagIMO_trk_cov14 = 0;
  MuTagIMO_trk_cov15 = 0;
  MuTagIMO_trk_cov22 = 0;
  MuTagIMO_trk_cov23 = 0;
  MuTagIMO_trk_cov24 = 0;
  MuTagIMO_trk_cov25 = 0;
  MuTagIMO_trk_cov33 = 0;
  MuTagIMO_trk_cov34 = 0;
  MuTagIMO_trk_cov35 = 0;
  MuTagIMO_trk_cov44 = 0;
  MuTagIMO_trk_cov45 = 0;
  MuTagIMO_trk_cov55 = 0;
  MuTagIMO_exTrk_trackIndex = 0;
  MuTagIMO_exTrk_A0 = 0;
  MuTagIMO_exTrk_Z0 = 0;
  MuTagIMO_exTrk_phi = 0;
  MuTagIMO_exTrk_theta = 0;
  MuTagIMO_exTrk_qOverP = 0;
  MuTagIMO_exTrk_cov11 = 0;
  MuTagIMO_exTrk_cov12 = 0;
  MuTagIMO_exTrk_cov13 = 0;
  MuTagIMO_exTrk_cov14 = 0;
  MuTagIMO_exTrk_cov15 = 0;
  MuTagIMO_exTrk_cov22 = 0;
  MuTagIMO_exTrk_cov23 = 0;
  MuTagIMO_exTrk_cov24 = 0;
  MuTagIMO_exTrk_cov25 = 0;
  MuTagIMO_exTrk_cov33 = 0;
  MuTagIMO_exTrk_cov34 = 0;
  MuTagIMO_exTrk_cov35 = 0;
  MuTagIMO_exTrk_cov44 = 0;
  MuTagIMO_exTrk_cov45 = 0;
  MuTagIMO_exTrk_cov55 = 0;
  MuTagIMO_exTrk_Eloss = 0;
  MuTagIMO_exTrk_gPosX = 0;
  MuTagIMO_exTrk_gPosY = 0;
  MuTagIMO_exTrk_gPosZ = 0;
  MuTagIMO_exTrk_gDirX = 0;
  MuTagIMO_exTrk_gDirY = 0;
  MuTagIMO_exTrk_gDirZ = 0;
  MuTagIMO_exTrk_locX = 0;
  MuTagIMO_exTrk_locY = 0;
  MuTagIMO_exTrk_angleXZ = 0;
  MuTagIMO_exTrk_angleYZ = 0;
  MuTagIMO_exTrk_locX_error = 0;
  MuTagIMO_exTrk_locY_error = 0;
  MuTagIMO_exTrk_angleXZ_error = 0;
  MuTagIMO_exTrk_angleYZ_error = 0;
  MuTagIMO_seg_isTagSeg = 0;
  MuTagIMO_seg_matchesBarCode = 0;
  MuTagIMO_seg_matchesPdg = 0;
  MuTagIMO_seg_exTrkIndex = 0;
  MuTagIMO_seg_trackIndex = 0;
  MuTagIMO_seg_stationLevel = 0;
  MuTagIMO_seg_isEndCap = 0;
  MuTagIMO_seg_isLarge = 0;
  MuTagIMO_seg_nrHoles = 0;
  MuTagIMO_seg_nrMLs = 0;
  MuTagIMO_seg_nrMdtHits = 0;
  MuTagIMO_seg_nrRpcHits = 0;
  MuTagIMO_seg_nrTgcHits = 0;
  MuTagIMO_seg_nrCscHits = 0;
  MuTagIMO_seg_phi = 0;
  MuTagIMO_seg_theta = 0;
  MuTagIMO_seg_chi2 = 0;
  MuTagIMO_seg_cov11 = 0;
  MuTagIMO_seg_cov12 = 0;
  MuTagIMO_seg_cov13 = 0;
  MuTagIMO_seg_cov14 = 0;
  MuTagIMO_seg_cov15 = 0;
  MuTagIMO_seg_cov22 = 0;
  MuTagIMO_seg_cov23 = 0;
  MuTagIMO_seg_cov24 = 0;
  MuTagIMO_seg_cov25 = 0;
  MuTagIMO_seg_cov33 = 0;
  MuTagIMO_seg_cov34 = 0;
  MuTagIMO_seg_cov35 = 0;
  MuTagIMO_seg_cov44 = 0;
  MuTagIMO_seg_cov45 = 0;
  MuTagIMO_seg_cov55 = 0;
  MuTagIMO_seg_gPosX = 0;
  MuTagIMO_seg_gPosY = 0;
  MuTagIMO_seg_gPosZ = 0;
  MuTagIMO_seg_gDirX = 0;
  MuTagIMO_seg_gDirY = 0;
  MuTagIMO_seg_gDirZ = 0;
  MuTagIMO_seg_locX = 0;
  MuTagIMO_seg_locY = 0;
  MuTagIMO_seg_angleXZ = 0;
  MuTagIMO_seg_angleYZ = 0;
  MuTagIMO_seg_locX_error = 0;
  MuTagIMO_seg_locY_error = 0;
  MuTagIMO_seg_angleXZ_error = 0;
  MuTagIMO_seg_angleYZ_error = 0;
  MuTagIMO_match_trackIndex = 0;
  MuTagIMO_match_locX = 0;
  MuTagIMO_match_locY = 0;
  MuTagIMO_match_angleXZ = 0;
  MuTagIMO_match_angleYZ = 0;
  MuTagIMO_truthID_barCode = 0;
  MuTagIMO_truthID_pdg = 0;
  MuTagIMO_truthID_posX = 0;
  MuTagIMO_truthID_posY = 0;
  MuTagIMO_truthID_posZ = 0;
  MuTagIMO_truthID_momX = 0;
  MuTagIMO_truthID_momY = 0;
  MuTagIMO_truthID_momZ = 0;
  MuTagIMO_truthID_energy = 0;
  MuTagIMO_truthMS_barCode = 0;
  MuTagIMO_truthMS_pdg = 0;
  MuTagIMO_truthMS_posX = 0;
  MuTagIMO_truthMS_posY = 0;
  MuTagIMO_truthMS_posZ = 0;
  MuTagIMO_truthMS_momX = 0;
  MuTagIMO_truthMS_momY = 0;
  MuTagIMO_truthMS_momZ = 0;
  MuTagIMO_truthMS_energy = 0;
  MuTagIMO_trHit_id = 0;
  MuTagIMO_trHit_barCode = 0;
  MuTagIMO_trHit_r = 0;
  MuTagIMO_trHit_alongTube = 0;
  L1Em_RoIWord = 0;
  L1Em_Core = 0;
  L1Em_EmClus = 0;
  L1Em_TauClus = 0;
  L1Em_EmIsol = 0;
  L1Em_HdIsol = 0;
  L1Em_HdCore = 0;
  L1Em_EmThresh = 0;
  L1Em_TauThresh = 0;
  L1Em_eta = 0;
  L1Em_phi = 0;
  L1Jet_JetRoIWord = 0;
  L1Jet_ET4x4 = 0;
  L1Jet_ET6x6 = 0;
  L1Jet_ET8x8 = 0;
  L1Jet_Thresh = 0;
  L1Jet_eta = 0;
  L1Jet_phi = 0;
  CTP_ROI = 0;
  muCTPI_ROI = 0;
  EMTau_ROI = 0;
  JetEnergy_ROI = 0;
  L1CaloPPM_eta = 0;
  L1CaloPPM_phi = 0;
  L1CaloPPM_emCrate = 0;
  L1CaloPPM_emModule = 0;
  L1CaloPPM_emSubmodule = 0;
  L1CaloPPM_emChannel = 0;
  L1CaloPPM_hadCrate = 0;
  L1CaloPPM_hadModule = 0;
  L1CaloPPM_hadSubmodule = 0;
  L1CaloPPM_hadChannel = 0;
  L1CaloPPM_emChannelId = 0;
  L1CaloPPM_hadChannelId = 0;
  L1CaloPPM_emTTCellsEnergy = 0;
  L1CaloPPM_hadTTCellsEnergy = 0;
  L1CaloPPM_emCaloWaves = 0;
  L1CaloPPM_hadCaloWaves = 0;
  L1CaloPPM_emPeak = 0;
  L1CaloPPM_emADCPeak = 0;
  L1CaloPPM_hadPeak = 0;
  L1CaloPPM_hadADCPeak = 0;
  L1CaloPPM_emLUT = 0;
  L1CaloPPM_hadLUT = 0;
  L1CaloPPM_emADC = 0;
  L1CaloPPM_hadADC = 0;
  L1CaloPPM_emEnergy = 0;
  L1CaloPPM_hadEnergy = 0;
  L1CaloPPM_emBCIDvec = 0;
  L1CaloPPM_emBCIDext = 0;
  L1CaloPPM_hadBCIDvec = 0;
  L1CaloPPM_hadBCIDext = 0;
  L1CaloPPM_emError = 0;
  L1CaloPPM_hadError = 0;
  L1CaloPPM_emBCID = 0;
  L1CaloPPM_hadBCID = 0;
  L1CaloPPM_emIsSaturated = 0;
  L1CaloPPM_hadIsSaturated = 0;
  L1CaloPPM_emIsFilled = 0;
  L1CaloPPM_hadIsFilled = 0;
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
  T2CaEtas1 = 0;
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
  T2CaJetE = 0;
  T2CaJetEhad0 = 0;
  T2CaJetEem0 = 0;
  T2CaJeteta = 0;
  T2CaJetphi = 0;
  T2CaJetroi = 0;
  TrigT2MissingEtEt = 0;
  TrigT2MissingEtEx = 0;
  TrigT2MissingEtEy = 0;
  TrigT2MissingEtsumEt = 0;
  TrigT2MissingEtRoIword = 0;
  TrigEFMissingEtEt = 0;
  TrigEFMissingEtEx = 0;
  TrigEFMissingEtEy = 0;
  TrigEFMissingEtsumEt = 0;
  TrigEFMissingEtRoIword = 0;
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
  T2IdEtac = 0;
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
  TrkEF_truthBarcode = 0;
  TrkEF_truthNt = 0;
  TrkEF_ParticlePdg = 0;
  TrkEF_ParentPdg = 0;
  TrkEF_NumSiKineHits = 0;
  TrkEF_NumTRTKineHits = 0;
  VxEF_vtx_x = 0;
  VxEF_vtx_y = 0;
  VxEF_vtx_z = 0;
  VxEF_vtx_x_truth = 0;
  VxEF_vtx_y_truth = 0;
  VxEF_vtx_z_truth = 0;
  VxEF_vtx_x_res = 0;
  VxEF_vtx_y_res = 0;
  VxEF_vtx_z_res = 0;
  VxEF_sigvtx_x = 0;
  VxEF_sigvtx_y = 0;
  VxEF_sigvtx_z = 0;
  VxEF_vtxchi2 = 0;
  VxEF_vtxndf = 0;
  VxEF_pt = 0;
  VxEF_vtxchi2prob = 0;
  VxEF_numTracksPerVertex = 0;
  VxEF_vtxType = 0;
  VxEF_chi2 = 0;
  VxEF_d0 = 0;
  VxEF_sigd0 = 0;
  VxEF_z0 = 0;
  VxEF_sigz0 = 0;
  VxEF_phi = 0;
  VxEF_sigphi = 0;
  VxEF_theta = 0;
  VxEF_sigtheta = 0;
  VxEF_qOverP = 0;
  VxEF_sigqOverP = 0;
  VxEF_d0g = 0;
  VxEF_sigd0g = 0;
  VxEF_z0g = 0;
  VxEF_sigz0g = 0;
  eg_RoiId_EF = 0;
  eg_TrigKey_EF = 0;
  eg_e_EF = 0;
  eg_eta_EF = 0;
  eg_phi_EF = 0;
  eg_author_EF = 0;
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
  eg_Hmatrix5_EF = 0;
  eg_Hmatrix10_EF = 0;
  eg_IsolationLikelihood_EF = 0;
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
  eg_bremInvpT_EF = 0;
  eg_bremRadius_EF = 0;
  eg_bremX_EF = 0;
  eg_bremClusterRadius_EF = 0;
  eg_bremInvpTerr_EF = 0;
  EFBphys_roiId = 0;
  EFBphys_eta = 0;
  EFBphys_phi = 0;
  EFBphys_pType = 0;
  EFBphys_mass = 0;
  EFBphys_fitmass = 0;
  EFBphys_fitchis = 0;
  EFBphys_fitprob = 0;
  EFBphys_secmass = 0;
  EFBphys_secfitmass = 0;
  EFBphys_secfitchis = 0;
  EFBphys_secfitprob = 0;
  T2BjEta = 0;
  T2BjPhi0 = 0;
  T2BjPvtx = 0;
  T2BjPtJet = 0;
  T2BjXIP1D = 0;
  T2BjXIP2D = 0;
  T2BjXIP3D = 0;
  T2BjXChi2 = 0;
  T2BjXSV = 0;
  T2BjXmvtx = 0;
  T2BjXevtx = 0;
  T2BjXnvtx = 0;
  EFBjEta = 0;
  EFBjPhi0 = 0;
  EFBjPvtx = 0;
  EFBjPtJet = 0;
  EFBjXIP1D = 0;
  EFBjXIP2D = 0;
  EFBjXIP3D = 0;
  EFBjXChi2 = 0;
  EFBjXSV = 0;
  EFBjXmvtx = 0;
  EFBjXevtx = 0;
  EFBjXnvtx = 0;
  tau_L2eta = 0;
  tau_L2phi = 0;
  tau_L2pt = 0;
  tau_L2Zvtx = 0;
  tau_L2nMatchTracks = 0;
  tau_L2roiId = 0;
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
  fChain->SetBranchStatus("jetNumCone4H1TowerJets*",1);
  fChain->SetBranchStatus("jetEtaCone4H1TowerJets*",1);
  fChain->SetBranchStatus("jetPhiCone4H1TowerJets*",1);
  fChain->SetBranchStatus("jetPxCone4H1TowerJets*",1);
  fChain->SetBranchStatus("jetPyCone4H1TowerJets*",1);
  fChain->SetBranchStatus("jetEtCone4H1TowerJets*",1);
  fChain->SetBranchStatus("jetEmfCone4H1TowerJets*",1);
  fChain->SetBranchStatus("jetNumCone7H1TowerJets*",1);
  fChain->SetBranchStatus("jetEtaCone7H1TowerJets*",1);
  fChain->SetBranchStatus("jetPhiCone7H1TowerJets*",1);
  fChain->SetBranchStatus("jetPxCone7H1TowerJets*",1);
  fChain->SetBranchStatus("jetPyCone7H1TowerJets*",1);
  fChain->SetBranchStatus("jetEtCone7H1TowerJets*",1);
  fChain->SetBranchStatus("jetEmfCone7H1TowerJets*",1);
  fChain->SetBranchStatus("jetNumCone4TruthJets*",1);
  fChain->SetBranchStatus("jetEtaCone4TruthJets*",1);
  fChain->SetBranchStatus("jetPhiCone4TruthJets*",1);
  fChain->SetBranchStatus("jetPxCone4TruthJets*",1);
  fChain->SetBranchStatus("jetPyCone4TruthJets*",1);
  fChain->SetBranchStatus("jetEtCone4TruthJets*",1);
  fChain->SetBranchStatus("jetEmfCone4TruthJets*",1);
  fChain->SetBranchStatus("jetNumCone4TruthPileupJets*",1);
  fChain->SetBranchStatus("jetEtaCone4TruthPileupJets*",1);
  fChain->SetBranchStatus("jetPhiCone4TruthPileupJets*",1);
  fChain->SetBranchStatus("jetPxCone4TruthPileupJets*",1);
  fChain->SetBranchStatus("jetPyCone4TruthPileupJets*",1);
  fChain->SetBranchStatus("jetEtCone4TruthPileupJets*",1);
  fChain->SetBranchStatus("jetEmfCone4TruthPileupJets*",1);
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
  fChain->SetBranchStatus("GenStat*",1);
  fChain->SetBranchStatus("GenRef*",1);
  fChain->SetBranchStatus("KMothNt*",1);
  fChain->SetBranchStatus("KEndVNt*",1);
  fChain->SetBranchStatus("KOriVNt*",1);
  fChain->SetBranchStatus("Type*",1);
  fChain->SetBranchStatus("PtGen*",1);
  fChain->SetBranchStatus("EtaGen*",1);
  fChain->SetBranchStatus("PhiGen*",1);
  fChain->SetBranchStatus("MGen*",1);
  fChain->SetBranchStatus("RVGen*",1);
  fChain->SetBranchStatus("PhiVGen*",1);
  fChain->SetBranchStatus("ZVGen*",1);
  fChain->SetBranchStatus("RV*",1);
  fChain->SetBranchStatus("ZV*",1);
  fChain->SetBranchStatus("vxp_vtx_x_truth*",1);
  fChain->SetBranchStatus("vxp_vtx_y_truth*",1);
  fChain->SetBranchStatus("vxp_vtx_z_truth*",1);
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
  fChain->SetBranchStatus("Trk_sigphi",1);
  fChain->SetBranchStatus("Trk_sigtheta",1);
  fChain->SetBranchStatus("Trk_sigqOverP",1);
  fChain->SetBranchStatus("Trk_covVert21",1);
  fChain->SetBranchStatus("Trk_covVert31",1);
  fChain->SetBranchStatus("Trk_covVert41",1);
  fChain->SetBranchStatus("Trk_covVert51",1);
  fChain->SetBranchStatus("Trk_covVert32",1);
  fChain->SetBranchStatus("Trk_covVert42",1);
  fChain->SetBranchStatus("Trk_covVert52",1);
  fChain->SetBranchStatus("Trk_covVert43",1);
  fChain->SetBranchStatus("Trk_covVert53",1);
  fChain->SetBranchStatus("Trk_covVert54",1);
  fChain->SetBranchStatus("Trk_Chi2",1);
  fChain->SetBranchStatus("Trk_Ndf",1);
  fChain->SetBranchStatus("Trk_numberOfPixelHits",1);
  fChain->SetBranchStatus("Trk_numberOfSCTHits",1);
  fChain->SetBranchStatus("Trk_numberOfTRTHits",1);
  fChain->SetBranchStatus("Trk_numberOfPixelHoles",1);
  fChain->SetBranchStatus("Trk_numberOfSCTHoles",1);
  fChain->SetBranchStatus("Trk_numberOfPixelSharedHits",1);
  fChain->SetBranchStatus("Trk_numberOfSCTSharedHits",1);
  fChain->SetBranchStatus("Trk_numberOfBLayerHits",1);
  fChain->SetBranchStatus("Trk_truthBarcode",1);
  fChain->SetBranchStatus("Trk_truthNt",1);
  fChain->SetBranchStatus("Trk_ParticlePdg",1);
  fChain->SetBranchStatus("Trk_ParentPdg",1);

  fChain->SetBranchStatus("T2BjN",1);
  fChain->SetBranchStatus("T2BjEta",1);
  fChain->SetBranchStatus("T2BjPhi0",1);
  fChain->SetBranchStatus("T2BjXIP1D",1);
  fChain->SetBranchStatus("T2BjXIP2D",1);
  fChain->SetBranchStatus("T2BjXIP3D",1);
  fChain->SetBranchStatus("T2BjXChi2",1);
  fChain->SetBranchStatus("T2BjXSV",1);

  fChain->SetBranchStatus("EFBjN",1);
  fChain->SetBranchStatus("EFBjEta",1);
  fChain->SetBranchStatus("EFBjPhi0",1);
  fChain->SetBranchStatus("EFBjXIP1D",1);
  fChain->SetBranchStatus("EFBjXIP2D",1);
  fChain->SetBranchStatus("EFBjXIP3D",1);
  fChain->SetBranchStatus("EFBjXChi2",1);
  fChain->SetBranchStatus("EFBjXSV",1);


  if( fChain->GetBranchStatus("RunNumber") ) { fChain->SetBranchAddress("RunNumber", &RunNumber, &b_RunNumber); }
  if( fChain->GetBranchStatus("EventNumber") ) { fChain->SetBranchAddress("EventNumber", &EventNumber, &b_EventNumber); }
  if( fChain->GetBranchStatus("StreamESD_ref") ) { fChain->SetBranchAddress("StreamESD_ref", &StreamESD_ref, &b_StreamESD_ref); }
  if( fChain->GetBranchStatus("Token") ) { fChain->SetBranchAddress("Token", Token, &b_Token); }
  if( fChain->GetBranchStatus("Run") ) { fChain->SetBranchAddress("Run", &Run, &b_Run); }
  if( fChain->GetBranchStatus("Event") ) { fChain->SetBranchAddress("Event", &Event, &b_Event); }
  if( fChain->GetBranchStatus("Time") ) { fChain->SetBranchAddress("Time", &Time, &b_Time); }
  if( fChain->GetBranchStatus("LumiBlock") ) { fChain->SetBranchAddress("LumiBlock", &LumiBlock, &b_LumiBlock); }
  if( fChain->GetBranchStatus("BCID") ) { fChain->SetBranchAddress("BCID", &BCID, &b_BCID); }
  if( fChain->GetBranchStatus("LVL1ID") ) { fChain->SetBranchAddress("LVL1ID", &LVL1ID, &b_LVL1ID); }
  if( fChain->GetBranchStatus("Weight") ) { fChain->SetBranchAddress("Weight", &Weight, &b_Weight); }
  if( fChain->GetBranchStatus("IEvent") ) { fChain->SetBranchAddress("IEvent", &IEvent, &b_IEvent); }
  if( fChain->GetBranchStatus("StatusElement") ) { fChain->SetBranchAddress("StatusElement", &StatusElement, &b_StatusElement); }
  if( fChain->GetBranchStatus("LVL1TriggerType") ) { fChain->SetBranchAddress("LVL1TriggerType", &LVL1TriggerType, &b_LVL1TriggerType); }
  if( fChain->GetBranchStatus("LVL1TriggerInfo") ) { fChain->SetBranchAddress("LVL1TriggerInfo", &LVL1TriggerInfo, &b_LVL1TriggerInfo); }
  if( fChain->GetBranchStatus("LVL2TriggerInfo") ) { fChain->SetBranchAddress("LVL2TriggerInfo", &LVL2TriggerInfo, &b_LVL2TriggerInfo); }
  if( fChain->GetBranchStatus("EventFilterInfo") ) { fChain->SetBranchAddress("EventFilterInfo", &EventFilterInfo, &b_EventFilterInfo); }
  if( fChain->GetBranchStatus("StreamTagName") ) { fChain->SetBranchAddress("StreamTagName", &StreamTagName, &b_StreamTagName); }
  if( fChain->GetBranchStatus("StreamTagType") ) { fChain->SetBranchAddress("StreamTagType", &StreamTagType, &b_StreamTagType); }
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
  if( fChain->GetBranchStatus("cl_ecluster_topoEM430") ) { fChain->SetBranchAddress("cl_ecluster_topoEM430", &cl_ecluster_topoEM430, &b_cl_ecluster_topoEM430); }
  if( fChain->GetBranchStatus("cl_nctotal_topoEM430") ) { fChain->SetBranchAddress("cl_nctotal_topoEM430", &cl_nctotal_topoEM430, &b_cl_nctotal_topoEM430); }
  if( fChain->GetBranchStatus("cl_nc_topoEM430") ) { fChain->SetBranchAddress("cl_nc_topoEM430", &cl_nc_topoEM430, &b_cl_nc_topoEM430); }
  if( fChain->GetBranchStatus("cl_et_topoEM430") ) { fChain->SetBranchAddress("cl_et_topoEM430", &cl_et_topoEM430, &b_cl_et_topoEM430); }
  if( fChain->GetBranchStatus("cl_e_topoEM430") ) { fChain->SetBranchAddress("cl_e_topoEM430", &cl_e_topoEM430, &b_cl_e_topoEM430); }
  if( fChain->GetBranchStatus("cl_eemb0_topoEM430") ) { fChain->SetBranchAddress("cl_eemb0_topoEM430", &cl_eemb0_topoEM430, &b_cl_eemb0_topoEM430); }
  if( fChain->GetBranchStatus("cl_eemb1_topoEM430") ) { fChain->SetBranchAddress("cl_eemb1_topoEM430", &cl_eemb1_topoEM430, &b_cl_eemb1_topoEM430); }
  if( fChain->GetBranchStatus("cl_eemb2_topoEM430") ) { fChain->SetBranchAddress("cl_eemb2_topoEM430", &cl_eemb2_topoEM430, &b_cl_eemb2_topoEM430); }
  if( fChain->GetBranchStatus("cl_eemb3_topoEM430") ) { fChain->SetBranchAddress("cl_eemb3_topoEM430", &cl_eemb3_topoEM430, &b_cl_eemb3_topoEM430); }
  if( fChain->GetBranchStatus("cl_nemb0_topoEM430") ) { fChain->SetBranchAddress("cl_nemb0_topoEM430", &cl_nemb0_topoEM430, &b_cl_nemb0_topoEM430); }
  if( fChain->GetBranchStatus("cl_nemb1_topoEM430") ) { fChain->SetBranchAddress("cl_nemb1_topoEM430", &cl_nemb1_topoEM430, &b_cl_nemb1_topoEM430); }
  if( fChain->GetBranchStatus("cl_nemb2_topoEM430") ) { fChain->SetBranchAddress("cl_nemb2_topoEM430", &cl_nemb2_topoEM430, &b_cl_nemb2_topoEM430); }
  if( fChain->GetBranchStatus("cl_nemb3_topoEM430") ) { fChain->SetBranchAddress("cl_nemb3_topoEM430", &cl_nemb3_topoEM430, &b_cl_nemb3_topoEM430); }
  if( fChain->GetBranchStatus("cl_phi2_topoEM430") ) { fChain->SetBranchAddress("cl_phi2_topoEM430", &cl_phi2_topoEM430, &b_cl_phi2_topoEM430); }
  if( fChain->GetBranchStatus("cl_eta0_topoEM430") ) { fChain->SetBranchAddress("cl_eta0_topoEM430", &cl_eta0_topoEM430, &b_cl_eta0_topoEM430); }
  if( fChain->GetBranchStatus("cl_eta1_topoEM430") ) { fChain->SetBranchAddress("cl_eta1_topoEM430", &cl_eta1_topoEM430, &b_cl_eta1_topoEM430); }
  if( fChain->GetBranchStatus("cl_eta2_topoEM430") ) { fChain->SetBranchAddress("cl_eta2_topoEM430", &cl_eta2_topoEM430, &b_cl_eta2_topoEM430); }
  if( fChain->GetBranchStatus("cl_eta3_topoEM430") ) { fChain->SetBranchAddress("cl_eta3_topoEM430", &cl_eta3_topoEM430, &b_cl_eta3_topoEM430); }
  if( fChain->GetBranchStatus("cl_eeme0_topoEM430") ) { fChain->SetBranchAddress("cl_eeme0_topoEM430", &cl_eeme0_topoEM430, &b_cl_eeme0_topoEM430); }
  if( fChain->GetBranchStatus("cl_eeme1_topoEM430") ) { fChain->SetBranchAddress("cl_eeme1_topoEM430", &cl_eeme1_topoEM430, &b_cl_eeme1_topoEM430); }
  if( fChain->GetBranchStatus("cl_eeme2_topoEM430") ) { fChain->SetBranchAddress("cl_eeme2_topoEM430", &cl_eeme2_topoEM430, &b_cl_eeme2_topoEM430); }
  if( fChain->GetBranchStatus("cl_eeme3_topoEM430") ) { fChain->SetBranchAddress("cl_eeme3_topoEM430", &cl_eeme3_topoEM430, &b_cl_eeme3_topoEM430); }
  if( fChain->GetBranchStatus("cl_neme0_topoEM430") ) { fChain->SetBranchAddress("cl_neme0_topoEM430", &cl_neme0_topoEM430, &b_cl_neme0_topoEM430); }
  if( fChain->GetBranchStatus("cl_neme1_topoEM430") ) { fChain->SetBranchAddress("cl_neme1_topoEM430", &cl_neme1_topoEM430, &b_cl_neme1_topoEM430); }
  if( fChain->GetBranchStatus("cl_neme2_topoEM430") ) { fChain->SetBranchAddress("cl_neme2_topoEM430", &cl_neme2_topoEM430, &b_cl_neme2_topoEM430); }
  if( fChain->GetBranchStatus("cl_neme3_topoEM430") ) { fChain->SetBranchAddress("cl_neme3_topoEM430", &cl_neme3_topoEM430, &b_cl_neme3_topoEM430); }
  if( fChain->GetBranchStatus("cl_etileg1_topoEM430") ) { fChain->SetBranchAddress("cl_etileg1_topoEM430", &cl_etileg1_topoEM430, &b_cl_etileg1_topoEM430); }
  if( fChain->GetBranchStatus("cl_etileg2_topoEM430") ) { fChain->SetBranchAddress("cl_etileg2_topoEM430", &cl_etileg2_topoEM430, &b_cl_etileg2_topoEM430); }
  if( fChain->GetBranchStatus("cl_etileg3_topoEM430") ) { fChain->SetBranchAddress("cl_etileg3_topoEM430", &cl_etileg3_topoEM430, &b_cl_etileg3_topoEM430); }
  if( fChain->GetBranchStatus("cl_ntileg1_topoEM430") ) { fChain->SetBranchAddress("cl_ntileg1_topoEM430", &cl_ntileg1_topoEM430, &b_cl_ntileg1_topoEM430); }
  if( fChain->GetBranchStatus("cl_ntileg2_topoEM430") ) { fChain->SetBranchAddress("cl_ntileg2_topoEM430", &cl_ntileg2_topoEM430, &b_cl_ntileg2_topoEM430); }
  if( fChain->GetBranchStatus("cl_ntileg3_topoEM430") ) { fChain->SetBranchAddress("cl_ntileg3_topoEM430", &cl_ntileg3_topoEM430, &b_cl_ntileg3_topoEM430); }
  if( fChain->GetBranchStatus("cl_eta_topoEM430") ) { fChain->SetBranchAddress("cl_eta_topoEM430", &cl_eta_topoEM430, &b_cl_eta_topoEM430); }
  if( fChain->GetBranchStatus("cl_phi_topoEM430") ) { fChain->SetBranchAddress("cl_phi_topoEM430", &cl_phi_topoEM430, &b_cl_phi_topoEM430); }
  if( fChain->GetBranchStatus("cl_reco_stat_topoEM430") ) { fChain->SetBranchAddress("cl_reco_stat_topoEM430", &cl_reco_stat_topoEM430, &b_cl_reco_stat_topoEM430); }
  if( fChain->GetBranchStatus("cl_m1_eta_topoEM430") ) { fChain->SetBranchAddress("cl_m1_eta_topoEM430", &cl_m1_eta_topoEM430, &b_cl_m1_eta_topoEM430); }
  if( fChain->GetBranchStatus("cl_m1_phi_topoEM430") ) { fChain->SetBranchAddress("cl_m1_phi_topoEM430", &cl_m1_phi_topoEM430, &b_cl_m1_phi_topoEM430); }
  if( fChain->GetBranchStatus("cl_m2_r_topoEM430") ) { fChain->SetBranchAddress("cl_m2_r_topoEM430", &cl_m2_r_topoEM430, &b_cl_m2_r_topoEM430); }
  if( fChain->GetBranchStatus("cl_m2_lambda_topoEM430") ) { fChain->SetBranchAddress("cl_m2_lambda_topoEM430", &cl_m2_lambda_topoEM430, &b_cl_m2_lambda_topoEM430); }
  if( fChain->GetBranchStatus("cl_delta_phi_topoEM430") ) { fChain->SetBranchAddress("cl_delta_phi_topoEM430", &cl_delta_phi_topoEM430, &b_cl_delta_phi_topoEM430); }
  if( fChain->GetBranchStatus("cl_delta_theta_topoEM430") ) { fChain->SetBranchAddress("cl_delta_theta_topoEM430", &cl_delta_theta_topoEM430, &b_cl_delta_theta_topoEM430); }
  if( fChain->GetBranchStatus("cl_delta_alpha_topoEM430") ) { fChain->SetBranchAddress("cl_delta_alpha_topoEM430", &cl_delta_alpha_topoEM430, &b_cl_delta_alpha_topoEM430); }
  if( fChain->GetBranchStatus("cl_center_x_topoEM430") ) { fChain->SetBranchAddress("cl_center_x_topoEM430", &cl_center_x_topoEM430, &b_cl_center_x_topoEM430); }
  if( fChain->GetBranchStatus("cl_center_y_topoEM430") ) { fChain->SetBranchAddress("cl_center_y_topoEM430", &cl_center_y_topoEM430, &b_cl_center_y_topoEM430); }
  if( fChain->GetBranchStatus("cl_center_z_topoEM430") ) { fChain->SetBranchAddress("cl_center_z_topoEM430", &cl_center_z_topoEM430, &b_cl_center_z_topoEM430); }
  if( fChain->GetBranchStatus("cl_center_lambda_topoEM430") ) { fChain->SetBranchAddress("cl_center_lambda_topoEM430", &cl_center_lambda_topoEM430, &b_cl_center_lambda_topoEM430); }
  if( fChain->GetBranchStatus("cl_lateral_topoEM430") ) { fChain->SetBranchAddress("cl_lateral_topoEM430", &cl_lateral_topoEM430, &b_cl_lateral_topoEM430); }
  if( fChain->GetBranchStatus("cl_longitudinal_topoEM430") ) { fChain->SetBranchAddress("cl_longitudinal_topoEM430", &cl_longitudinal_topoEM430, &b_cl_longitudinal_topoEM430); }
  if( fChain->GetBranchStatus("cl_eng_frac_em_topoEM430") ) { fChain->SetBranchAddress("cl_eng_frac_em_topoEM430", &cl_eng_frac_em_topoEM430, &b_cl_eng_frac_em_topoEM430); }
  if( fChain->GetBranchStatus("cl_eng_frac_max_topoEM430") ) { fChain->SetBranchAddress("cl_eng_frac_max_topoEM430", &cl_eng_frac_max_topoEM430, &b_cl_eng_frac_max_topoEM430); }
  if( fChain->GetBranchStatus("cl_eng_frac_core_topoEM430") ) { fChain->SetBranchAddress("cl_eng_frac_core_topoEM430", &cl_eng_frac_core_topoEM430, &b_cl_eng_frac_core_topoEM430); }
  if( fChain->GetBranchStatus("cl_m1_dens_topoEM430") ) { fChain->SetBranchAddress("cl_m1_dens_topoEM430", &cl_m1_dens_topoEM430, &b_cl_m1_dens_topoEM430); }
  if( fChain->GetBranchStatus("cl_m2_dens_topoEM430") ) { fChain->SetBranchAddress("cl_m2_dens_topoEM430", &cl_m2_dens_topoEM430, &b_cl_m2_dens_topoEM430); }
  if( fChain->GetBranchStatus("cl_isol_topoEM430") ) { fChain->SetBranchAddress("cl_isol_topoEM430", &cl_isol_topoEM430, &b_cl_isol_topoEM430); }
  if( fChain->GetBranchStatus("cl_ntotcells_topoEM430") ) { fChain->SetBranchAddress("cl_ntotcells_topoEM430", &cl_ntotcells_topoEM430, &b_cl_ntotcells_topoEM430); }
  if( fChain->GetBranchStatus("jetNumCone7H1TowerJets") ) { fChain->SetBranchAddress("jetNumCone7H1TowerJets", &jetNumCone7H1TowerJets, &b_Cone7H1TowerJetsjetNum); }
  if( fChain->GetBranchStatus("jetEtaCone7H1TowerJets") ) { fChain->SetBranchAddress("jetEtaCone7H1TowerJets", &jetEtaCone7H1TowerJets, &b_jetEtaCone7H1TowerJets); }
  if( fChain->GetBranchStatus("jetPhiCone7H1TowerJets") ) { fChain->SetBranchAddress("jetPhiCone7H1TowerJets", &jetPhiCone7H1TowerJets, &b_jetPhiCone7H1TowerJets); }
  if( fChain->GetBranchStatus("jetECone7H1TowerJets") ) { fChain->SetBranchAddress("jetECone7H1TowerJets", &jetECone7H1TowerJets, &b_jetECone7H1TowerJets); }
  if( fChain->GetBranchStatus("jetEtCone7H1TowerJets") ) { fChain->SetBranchAddress("jetEtCone7H1TowerJets", &jetEtCone7H1TowerJets, &b_jetEtCone7H1TowerJets); }
  if( fChain->GetBranchStatus("jetMCone7H1TowerJets") ) { fChain->SetBranchAddress("jetMCone7H1TowerJets", &jetMCone7H1TowerJets, &b_jetMCone7H1TowerJets); }
  if( fChain->GetBranchStatus("jetPxCone7H1TowerJets") ) { fChain->SetBranchAddress("jetPxCone7H1TowerJets", &jetPxCone7H1TowerJets, &b_jetPxCone7H1TowerJets); }
  if( fChain->GetBranchStatus("jetPyCone7H1TowerJets") ) { fChain->SetBranchAddress("jetPyCone7H1TowerJets", &jetPyCone7H1TowerJets, &b_jetPyCone7H1TowerJets); }
  if( fChain->GetBranchStatus("jetPzCone7H1TowerJets") ) { fChain->SetBranchAddress("jetPzCone7H1TowerJets", &jetPzCone7H1TowerJets, &b_jetPzCone7H1TowerJets); }
  if( fChain->GetBranchStatus("jetSizeCone7H1TowerJets") ) { fChain->SetBranchAddress("jetSizeCone7H1TowerJets", &jetSizeCone7H1TowerJets, &b_jetSizeCone7H1TowerJets); }
  if( fChain->GetBranchStatus("jetEmfCone7H1TowerJets") ) { fChain->SetBranchAddress("jetEmfCone7H1TowerJets", &jetEmfCone7H1TowerJets, &b_jetEmfCone7H1TowerJets); }
  if( fChain->GetBranchStatus("jetNumCone4H1TopoJets") ) { fChain->SetBranchAddress("jetNumCone4H1TopoJets", &jetNumCone4H1TopoJets, &b_Cone4H1TopoJetsjetNum); }
  if( fChain->GetBranchStatus("jetEtaCone4H1TopoJets") ) { fChain->SetBranchAddress("jetEtaCone4H1TopoJets", &jetEtaCone4H1TopoJets, &b_jetEtaCone4H1TopoJets); }
  if( fChain->GetBranchStatus("jetPhiCone4H1TopoJets") ) { fChain->SetBranchAddress("jetPhiCone4H1TopoJets", &jetPhiCone4H1TopoJets, &b_jetPhiCone4H1TopoJets); }
  if( fChain->GetBranchStatus("jetECone4H1TopoJets") ) { fChain->SetBranchAddress("jetECone4H1TopoJets", &jetECone4H1TopoJets, &b_jetECone4H1TopoJets); }
  if( fChain->GetBranchStatus("jetEtCone4H1TopoJets") ) { fChain->SetBranchAddress("jetEtCone4H1TopoJets", &jetEtCone4H1TopoJets, &b_jetEtCone4H1TopoJets); }
  if( fChain->GetBranchStatus("jetMCone4H1TopoJets") ) { fChain->SetBranchAddress("jetMCone4H1TopoJets", &jetMCone4H1TopoJets, &b_jetMCone4H1TopoJets); }
  if( fChain->GetBranchStatus("jetPxCone4H1TopoJets") ) { fChain->SetBranchAddress("jetPxCone4H1TopoJets", &jetPxCone4H1TopoJets, &b_jetPxCone4H1TopoJets); }
  if( fChain->GetBranchStatus("jetPyCone4H1TopoJets") ) { fChain->SetBranchAddress("jetPyCone4H1TopoJets", &jetPyCone4H1TopoJets, &b_jetPyCone4H1TopoJets); }
  if( fChain->GetBranchStatus("jetPzCone4H1TopoJets") ) { fChain->SetBranchAddress("jetPzCone4H1TopoJets", &jetPzCone4H1TopoJets, &b_jetPzCone4H1TopoJets); }
  if( fChain->GetBranchStatus("jetSizeCone4H1TopoJets") ) { fChain->SetBranchAddress("jetSizeCone4H1TopoJets", &jetSizeCone4H1TopoJets, &b_jetSizeCone4H1TopoJets); }
  if( fChain->GetBranchStatus("jetEmfCone4H1TopoJets") ) { fChain->SetBranchAddress("jetEmfCone4H1TopoJets", &jetEmfCone4H1TopoJets, &b_jetEmfCone4H1TopoJets); }
  if( fChain->GetBranchStatus("jetNumCone4H1TowerJets") ) { fChain->SetBranchAddress("jetNumCone4H1TowerJets", &jetNumCone4H1TowerJets, &b_Cone4H1TowerJetsjetNum); }
  if( fChain->GetBranchStatus("jetEtaCone4H1TowerJets") ) { fChain->SetBranchAddress("jetEtaCone4H1TowerJets", &jetEtaCone4H1TowerJets, &b_jetEtaCone4H1TowerJets); }
  if( fChain->GetBranchStatus("jetPhiCone4H1TowerJets") ) { fChain->SetBranchAddress("jetPhiCone4H1TowerJets", &jetPhiCone4H1TowerJets, &b_jetPhiCone4H1TowerJets); }
  if( fChain->GetBranchStatus("jetECone4H1TowerJets") ) { fChain->SetBranchAddress("jetECone4H1TowerJets", &jetECone4H1TowerJets, &b_jetECone4H1TowerJets); }
  if( fChain->GetBranchStatus("jetEtCone4H1TowerJets") ) { fChain->SetBranchAddress("jetEtCone4H1TowerJets", &jetEtCone4H1TowerJets, &b_jetEtCone4H1TowerJets); }
  if( fChain->GetBranchStatus("jetMCone4H1TowerJets") ) { fChain->SetBranchAddress("jetMCone4H1TowerJets", &jetMCone4H1TowerJets, &b_jetMCone4H1TowerJets); }
  if( fChain->GetBranchStatus("jetPxCone4H1TowerJets") ) { fChain->SetBranchAddress("jetPxCone4H1TowerJets", &jetPxCone4H1TowerJets, &b_jetPxCone4H1TowerJets); }
  if( fChain->GetBranchStatus("jetPyCone4H1TowerJets") ) { fChain->SetBranchAddress("jetPyCone4H1TowerJets", &jetPyCone4H1TowerJets, &b_jetPyCone4H1TowerJets); }
  if( fChain->GetBranchStatus("jetPzCone4H1TowerJets") ) { fChain->SetBranchAddress("jetPzCone4H1TowerJets", &jetPzCone4H1TowerJets, &b_jetPzCone4H1TowerJets); }
  if( fChain->GetBranchStatus("jetSizeCone4H1TowerJets") ) { fChain->SetBranchAddress("jetSizeCone4H1TowerJets", &jetSizeCone4H1TowerJets, &b_jetSizeCone4H1TowerJets); }
  if( fChain->GetBranchStatus("jetEmfCone4H1TowerJets") ) { fChain->SetBranchAddress("jetEmfCone4H1TowerJets", &jetEmfCone4H1TowerJets, &b_jetEmfCone4H1TowerJets); }
  if( fChain->GetBranchStatus("jetNumCone4TruthJets") ) { fChain->SetBranchAddress("jetNumCone4TruthJets", &jetNumCone4TruthJets, &b_Cone4TruthJetsjetNum); }
  if( fChain->GetBranchStatus("jetEtaCone4TruthJets") ) { fChain->SetBranchAddress("jetEtaCone4TruthJets", &jetEtaCone4TruthJets, &b_jetEtaCone4TruthJets); }
  if( fChain->GetBranchStatus("jetPhiCone4TruthJets") ) { fChain->SetBranchAddress("jetPhiCone4TruthJets", &jetPhiCone4TruthJets, &b_jetPhiCone4TruthJets); }
  if( fChain->GetBranchStatus("jetECone4TruthJets") ) { fChain->SetBranchAddress("jetECone4TruthJets", &jetECone4TruthJets, &b_jetECone4TruthJets); }
  if( fChain->GetBranchStatus("jetEtCone4TruthJets") ) { fChain->SetBranchAddress("jetEtCone4TruthJets", &jetEtCone4TruthJets, &b_jetEtCone4TruthJets); }
  if( fChain->GetBranchStatus("jetMCone4TruthJets") ) { fChain->SetBranchAddress("jetMCone4TruthJets", &jetMCone4TruthJets, &b_jetMCone4TruthJets); }
  if( fChain->GetBranchStatus("jetPxCone4TruthJets") ) { fChain->SetBranchAddress("jetPxCone4TruthJets", &jetPxCone4TruthJets, &b_jetPxCone4TruthJets); }
  if( fChain->GetBranchStatus("jetPyCone4TruthJets") ) { fChain->SetBranchAddress("jetPyCone4TruthJets", &jetPyCone4TruthJets, &b_jetPyCone4TruthJets); }
  if( fChain->GetBranchStatus("jetPzCone4TruthJets") ) { fChain->SetBranchAddress("jetPzCone4TruthJets", &jetPzCone4TruthJets, &b_jetPzCone4TruthJets); }
  if( fChain->GetBranchStatus("jetSizeCone4TruthJets") ) { fChain->SetBranchAddress("jetSizeCone4TruthJets", &jetSizeCone4TruthJets, &b_jetSizeCone4TruthJets); }
  if( fChain->GetBranchStatus("jetEmfCone4TruthJets") ) { fChain->SetBranchAddress("jetEmfCone4TruthJets", &jetEmfCone4TruthJets, &b_jetEmfCone4TruthJets); }
  if( fChain->GetBranchStatus("jetNumCone4TruthPileupJets") ) { fChain->SetBranchAddress("jetNumCone4TruthPileupJets", &jetNumCone4TruthPileupJets, &b_Cone4TruthPileupJetsjetNum); }
  if( fChain->GetBranchStatus("jetEtaCone4TruthPileupJets") ) { fChain->SetBranchAddress("jetEtaCone4TruthPileupJets", &jetEtaCone4TruthPileupJets, &b_jetEtaCone4TruthPileupJets); }
  if( fChain->GetBranchStatus("jetPhiCone4TruthPileupJets") ) { fChain->SetBranchAddress("jetPhiCone4TruthPileupJets", &jetPhiCone4TruthPileupJets, &b_jetPhiCone4TruthPileupJets); }
  if( fChain->GetBranchStatus("jetECone4TruthPileupJets") ) { fChain->SetBranchAddress("jetECone4TruthPileupJets", &jetECone4TruthPileupJets, &b_jetECone4TruthPileupJets); }
  if( fChain->GetBranchStatus("jetEtCone4TruthPileupJets") ) { fChain->SetBranchAddress("jetEtCone4TruthPileupJets", &jetEtCone4TruthPileupJets, &b_jetEtCone4TruthPileupJets); }
  if( fChain->GetBranchStatus("jetMCone4TruthPileupJets") ) { fChain->SetBranchAddress("jetMCone4TruthPileupJets", &jetMCone4TruthPileupJets, &b_jetMCone4TruthPileupJets); }
  if( fChain->GetBranchStatus("jetPxCone4TruthPileupJets") ) { fChain->SetBranchAddress("jetPxCone4TruthPileupJets", &jetPxCone4TruthPileupJets, &b_jetPxCone4TruthPileupJets); }
  if( fChain->GetBranchStatus("jetPyCone4TruthPileupJets") ) { fChain->SetBranchAddress("jetPyCone4TruthPileupJets", &jetPyCone4TruthPileupJets, &b_jetPyCone4TruthPileupJets); }
  if( fChain->GetBranchStatus("jetPzCone4TruthPileupJets") ) { fChain->SetBranchAddress("jetPzCone4TruthPileupJets", &jetPzCone4TruthPileupJets, &b_jetPzCone4TruthPileupJets); }
  if( fChain->GetBranchStatus("jetSizeCone4TruthPileupJets") ) { fChain->SetBranchAddress("jetSizeCone4TruthPileupJets", &jetSizeCone4TruthPileupJets, &b_jetSizeCone4TruthPileupJets); }
  if( fChain->GetBranchStatus("jetEmfCone4TruthPileupJets") ) { fChain->SetBranchAddress("jetEmfCone4TruthPileupJets", &jetEmfCone4TruthPileupJets, &b_jetEmfCone4TruthPileupJets); }
  if( fChain->GetBranchStatus("NTileMuonTag") ) { fChain->SetBranchAddress("NTileMuonTag", &NTileMuonTag, &b_NTileMuonTag); }
  if( fChain->GetBranchStatus("EtaTileMuonTag") ) { fChain->SetBranchAddress("EtaTileMuonTag", &EtaTileMuonTag, &b_EtaTileMuonTag); }
  if( fChain->GetBranchStatus("PhiTileMuonTag") ) { fChain->SetBranchAddress("PhiTileMuonTag", &PhiTileMuonTag, &b_PhiTileMuonTag); }
  if( fChain->GetBranchStatus("EnergyVecTileMuonTag") ) { fChain->SetBranchAddress("EnergyVecTileMuonTag", &EnergyVecTileMuonTag, &b_EnergyVecTileMuonTag); }
  if( fChain->GetBranchStatus("LabelTileMuonTag") ) { fChain->SetBranchAddress("LabelTileMuonTag", &LabelTileMuonTag, &b_LabelTileMuonTag); }
  if( fChain->GetBranchStatus("CaloMu_ntrack") ) { fChain->SetBranchAddress("CaloMu_ntrack", &CaloMu_ntrack, &b__ntrack); }
  if( fChain->GetBranchStatus("CaloMu_trkPdgid") ) { fChain->SetBranchAddress("CaloMu_trkPdgid", &CaloMu_trkPdgid, &b_CaloMu_trkPdgid); }
  if( fChain->GetBranchStatus("CaloMu_trkMother") ) { fChain->SetBranchAddress("CaloMu_trkMother", &CaloMu_trkMother, &b_CaloMu_trkMother); }
  if( fChain->GetBranchStatus("CaloMu_trkEta") ) { fChain->SetBranchAddress("CaloMu_trkEta", &CaloMu_trkEta, &b_CaloMu_trkEta); }
  if( fChain->GetBranchStatus("CaloMu_trkPhi") ) { fChain->SetBranchAddress("CaloMu_trkPhi", &CaloMu_trkPhi, &b_CaloMu_trkPhi); }
  if( fChain->GetBranchStatus("CaloMu_trkPt") ) { fChain->SetBranchAddress("CaloMu_trkPt", &CaloMu_trkPt, &b_CaloMu_trkPt); }
  if( fChain->GetBranchStatus("CaloMu_trkD0") ) { fChain->SetBranchAddress("CaloMu_trkD0", &CaloMu_trkD0, &b_CaloMu_trkD0); }
  if( fChain->GetBranchStatus("CaloMu_trkIP") ) { fChain->SetBranchAddress("CaloMu_trkIP", &CaloMu_trkIP, &b_CaloMu_trkIP); }
  if( fChain->GetBranchStatus("CaloMu_trkBLayerHits") ) { fChain->SetBranchAddress("CaloMu_trkBLayerHits", &CaloMu_trkBLayerHits, &b_CaloMu_trkBLayerHits); }
  if( fChain->GetBranchStatus("CaloMu_trkPixelHits") ) { fChain->SetBranchAddress("CaloMu_trkPixelHits", &CaloMu_trkPixelHits, &b_CaloMu_trkPixelHits); }
  if( fChain->GetBranchStatus("CaloMu_trkSCTHits") ) { fChain->SetBranchAddress("CaloMu_trkSCTHits", &CaloMu_trkSCTHits, &b_CaloMu_trkSCTHits); }
  if( fChain->GetBranchStatus("CaloMu_trkTRTHits") ) { fChain->SetBranchAddress("CaloMu_trkTRTHits", &CaloMu_trkTRTHits, &b_CaloMu_trkTRTHits); }
  if( fChain->GetBranchStatus("CaloMu_nmuon") ) { fChain->SetBranchAddress("CaloMu_nmuon", &CaloMu_nmuon, &b__nmuon); }
  if( fChain->GetBranchStatus("CaloMu_pdgid") ) { fChain->SetBranchAddress("CaloMu_pdgid", &CaloMu_pdgid, &b_CaloMu_pdgid); }
  if( fChain->GetBranchStatus("CaloMu_mother") ) { fChain->SetBranchAddress("CaloMu_mother", &CaloMu_mother, &b_CaloMu_mother); }
  if( fChain->GetBranchStatus("CaloMu_tag") ) { fChain->SetBranchAddress("CaloMu_tag", &CaloMu_tag, &b_CaloMu_tag); }
  if( fChain->GetBranchStatus("CaloMu_lhr") ) { fChain->SetBranchAddress("CaloMu_lhr", &CaloMu_lhr, &b_CaloMu_lhr); }
  if( fChain->GetBranchStatus("CaloMu_etCone") ) { fChain->SetBranchAddress("CaloMu_etCone", &CaloMu_etCone, &b_CaloMu_etCone); }
  if( fChain->GetBranchStatus("CaloMu_ptCone") ) { fChain->SetBranchAddress("CaloMu_ptCone", &CaloMu_ptCone, &b_CaloMu_ptCone); }
  if( fChain->GetBranchStatus("CaloMu_eta") ) { fChain->SetBranchAddress("CaloMu_eta", &CaloMu_eta, &b_CaloMu_eta); }
  if( fChain->GetBranchStatus("CaloMu_phi") ) { fChain->SetBranchAddress("CaloMu_phi", &CaloMu_phi, &b_CaloMu_phi); }
  if( fChain->GetBranchStatus("CaloMu_pt") ) { fChain->SetBranchAddress("CaloMu_pt", &CaloMu_pt, &b_CaloMu_pt); }
  if( fChain->GetBranchStatus("CaloMu_qoverp") ) { fChain->SetBranchAddress("CaloMu_qoverp", &CaloMu_qoverp, &b_CaloMu_qoverp); }
  if( fChain->GetBranchStatus("CaloMu_IP") ) { fChain->SetBranchAddress("CaloMu_IP", &CaloMu_IP, &b_CaloMu_IP); }
  if( fChain->GetBranchStatus("CaloMu_d0") ) { fChain->SetBranchAddress("CaloMu_d0", &CaloMu_d0, &b_CaloMu_d0); }
  if( fChain->GetBranchStatus("CaloMu_bLayerHits") ) { fChain->SetBranchAddress("CaloMu_bLayerHits", &CaloMu_bLayerHits, &b_CaloMu_bLayerHits); }
  if( fChain->GetBranchStatus("CaloMu_pixelHits") ) { fChain->SetBranchAddress("CaloMu_pixelHits", &CaloMu_pixelHits, &b_CaloMu_pixelHits); }
  if( fChain->GetBranchStatus("CaloMu_SCTHits") ) { fChain->SetBranchAddress("CaloMu_SCTHits", &CaloMu_SCTHits, &b_CaloMu_SCTHits); }
  if( fChain->GetBranchStatus("CaloMu_TRTHits") ) { fChain->SetBranchAddress("CaloMu_TRTHits", &CaloMu_TRTHits, &b_CaloMu_TRTHits); }
  if( fChain->GetBranchStatus("CaloMu_sample") ) { fChain->SetBranchAddress("CaloMu_sample", &CaloMu_sample, &b_CaloMu_sample); }
  if( fChain->GetBranchStatus("CaloMu_energy") ) { fChain->SetBranchAddress("CaloMu_energy", &CaloMu_energy, &b_CaloMu_energy); }
  if( fChain->GetBranchStatus("CaloMu_energyLoss") ) { fChain->SetBranchAddress("CaloMu_energyLoss", &CaloMu_energyLoss, &b_CaloMu_energyLoss); }
  if( fChain->GetBranchStatus("MuTagIMO_nmuon") ) { fChain->SetBranchAddress("MuTagIMO_nmuon", &MuTagIMO_nmuon, &b_MuTagIMO__nmuon); }
  if( fChain->GetBranchStatus("MuTagIMO_nTagSegs") ) { fChain->SetBranchAddress("MuTagIMO_nTagSegs", &MuTagIMO_nTagSegs, &b_MuTagIMO__nTagSegs); }
  if( fChain->GetBranchStatus("MuTagIMO_trk_isTagged") ) { fChain->SetBranchAddress("MuTagIMO_trk_isTagged", &MuTagIMO_trk_isTagged, &b_MuTagIMO_trk_isTagged); }
  if( fChain->GetBranchStatus("MuTagIMO_trk_matchesBarCode") ) { fChain->SetBranchAddress("MuTagIMO_trk_matchesBarCode", &MuTagIMO_trk_matchesBarCode, &b_MuTagIMO_trk_matchesBarCode); }
  if( fChain->GetBranchStatus("MuTagIMO_trk_matchesPdg") ) { fChain->SetBranchAddress("MuTagIMO_trk_matchesPdg", &MuTagIMO_trk_matchesPdg, &b_MuTagIMO_trk_matchesPdg); }
  if( fChain->GetBranchStatus("MuTagIMO_trk_nrPixelHits") ) { fChain->SetBranchAddress("MuTagIMO_trk_nrPixelHits", &MuTagIMO_trk_nrPixelHits, &b_MuTagIMO_trk_nrPixelHits); }
  if( fChain->GetBranchStatus("MuTagIMO_trk_nrSctHits") ) { fChain->SetBranchAddress("MuTagIMO_trk_nrSctHits", &MuTagIMO_trk_nrSctHits, &b_MuTagIMO_trk_nrSctHits); }
  if( fChain->GetBranchStatus("MuTagIMO_trk_A0") ) { fChain->SetBranchAddress("MuTagIMO_trk_A0", &MuTagIMO_trk_A0, &b_MuTagIMO_trk_A0); }
  if( fChain->GetBranchStatus("MuTagIMO_trk_Z0") ) { fChain->SetBranchAddress("MuTagIMO_trk_Z0", &MuTagIMO_trk_Z0, &b_MuTagIMO_trk_Z0); }
  if( fChain->GetBranchStatus("MuTagIMO_trk_phi") ) { fChain->SetBranchAddress("MuTagIMO_trk_phi", &MuTagIMO_trk_phi, &b_MuTagIMO_trk_phi); }
  if( fChain->GetBranchStatus("MuTagIMO_trk_theta") ) { fChain->SetBranchAddress("MuTagIMO_trk_theta", &MuTagIMO_trk_theta, &b_MuTagIMO_trk_theta); }
  if( fChain->GetBranchStatus("MuTagIMO_trk_qOverP") ) { fChain->SetBranchAddress("MuTagIMO_trk_qOverP", &MuTagIMO_trk_qOverP, &b_MuTagIMO_trk_qOverP); }
  if( fChain->GetBranchStatus("MuTagIMO_trk_chi2") ) { fChain->SetBranchAddress("MuTagIMO_trk_chi2", &MuTagIMO_trk_chi2, &b_MuTagIMO_trk_chi2); }
  if( fChain->GetBranchStatus("MuTagIMO_trk_ndof") ) { fChain->SetBranchAddress("MuTagIMO_trk_ndof", &MuTagIMO_trk_ndof, &b_MuTagIMO_trk_ndof); }
  if( fChain->GetBranchStatus("MuTagIMO_trk_Author") ) { fChain->SetBranchAddress("MuTagIMO_trk_Author", &MuTagIMO_trk_Author, &b_MuTagIMO_trk_Author); }
  if( fChain->GetBranchStatus("MuTagIMO_trk_cov11") ) { fChain->SetBranchAddress("MuTagIMO_trk_cov11", &MuTagIMO_trk_cov11, &b_MuTagIMO_trk_cov11); }
  if( fChain->GetBranchStatus("MuTagIMO_trk_cov12") ) { fChain->SetBranchAddress("MuTagIMO_trk_cov12", &MuTagIMO_trk_cov12, &b_MuTagIMO_trk_cov12); }
  if( fChain->GetBranchStatus("MuTagIMO_trk_cov13") ) { fChain->SetBranchAddress("MuTagIMO_trk_cov13", &MuTagIMO_trk_cov13, &b_MuTagIMO_trk_cov13); }
  if( fChain->GetBranchStatus("MuTagIMO_trk_cov14") ) { fChain->SetBranchAddress("MuTagIMO_trk_cov14", &MuTagIMO_trk_cov14, &b_MuTagIMO_trk_cov14); }
  if( fChain->GetBranchStatus("MuTagIMO_trk_cov15") ) { fChain->SetBranchAddress("MuTagIMO_trk_cov15", &MuTagIMO_trk_cov15, &b_MuTagIMO_trk_cov15); }
  if( fChain->GetBranchStatus("MuTagIMO_trk_cov22") ) { fChain->SetBranchAddress("MuTagIMO_trk_cov22", &MuTagIMO_trk_cov22, &b_MuTagIMO_trk_cov22); }
  if( fChain->GetBranchStatus("MuTagIMO_trk_cov23") ) { fChain->SetBranchAddress("MuTagIMO_trk_cov23", &MuTagIMO_trk_cov23, &b_MuTagIMO_trk_cov23); }
  if( fChain->GetBranchStatus("MuTagIMO_trk_cov24") ) { fChain->SetBranchAddress("MuTagIMO_trk_cov24", &MuTagIMO_trk_cov24, &b_MuTagIMO_trk_cov24); }
  if( fChain->GetBranchStatus("MuTagIMO_trk_cov25") ) { fChain->SetBranchAddress("MuTagIMO_trk_cov25", &MuTagIMO_trk_cov25, &b_MuTagIMO_trk_cov25); }
  if( fChain->GetBranchStatus("MuTagIMO_trk_cov33") ) { fChain->SetBranchAddress("MuTagIMO_trk_cov33", &MuTagIMO_trk_cov33, &b_MuTagIMO_trk_cov33); }
  if( fChain->GetBranchStatus("MuTagIMO_trk_cov34") ) { fChain->SetBranchAddress("MuTagIMO_trk_cov34", &MuTagIMO_trk_cov34, &b_MuTagIMO_trk_cov34); }
  if( fChain->GetBranchStatus("MuTagIMO_trk_cov35") ) { fChain->SetBranchAddress("MuTagIMO_trk_cov35", &MuTagIMO_trk_cov35, &b_MuTagIMO_trk_cov35); }
  if( fChain->GetBranchStatus("MuTagIMO_trk_cov44") ) { fChain->SetBranchAddress("MuTagIMO_trk_cov44", &MuTagIMO_trk_cov44, &b_MuTagIMO_trk_cov44); }
  if( fChain->GetBranchStatus("MuTagIMO_trk_cov45") ) { fChain->SetBranchAddress("MuTagIMO_trk_cov45", &MuTagIMO_trk_cov45, &b_MuTagIMO_trk_cov45); }
  if( fChain->GetBranchStatus("MuTagIMO_trk_cov55") ) { fChain->SetBranchAddress("MuTagIMO_trk_cov55", &MuTagIMO_trk_cov55, &b_MuTagIMO_trk_cov55); }
  if( fChain->GetBranchStatus("MuTagIMO_exTrk_trackIndex") ) { fChain->SetBranchAddress("MuTagIMO_exTrk_trackIndex", &MuTagIMO_exTrk_trackIndex, &b_MuTagIMO_exTrk_trackIndex); }
  if( fChain->GetBranchStatus("MuTagIMO_exTrk_A0") ) { fChain->SetBranchAddress("MuTagIMO_exTrk_A0", &MuTagIMO_exTrk_A0, &b_MuTagIMO_exTrk_A0); }
  if( fChain->GetBranchStatus("MuTagIMO_exTrk_Z0") ) { fChain->SetBranchAddress("MuTagIMO_exTrk_Z0", &MuTagIMO_exTrk_Z0, &b_MuTagIMO_exTrk_Z0); }
  if( fChain->GetBranchStatus("MuTagIMO_exTrk_phi") ) { fChain->SetBranchAddress("MuTagIMO_exTrk_phi", &MuTagIMO_exTrk_phi, &b_MuTagIMO_exTrk_phi); }
  if( fChain->GetBranchStatus("MuTagIMO_exTrk_theta") ) { fChain->SetBranchAddress("MuTagIMO_exTrk_theta", &MuTagIMO_exTrk_theta, &b_MuTagIMO_exTrk_theta); }
  if( fChain->GetBranchStatus("MuTagIMO_exTrk_qOverP") ) { fChain->SetBranchAddress("MuTagIMO_exTrk_qOverP", &MuTagIMO_exTrk_qOverP, &b_MuTagIMO_exTrk_qOverP); }
  if( fChain->GetBranchStatus("MuTagIMO_exTrk_cov11") ) { fChain->SetBranchAddress("MuTagIMO_exTrk_cov11", &MuTagIMO_exTrk_cov11, &b_MuTagIMO_exTrk_cov11); }
  if( fChain->GetBranchStatus("MuTagIMO_exTrk_cov12") ) { fChain->SetBranchAddress("MuTagIMO_exTrk_cov12", &MuTagIMO_exTrk_cov12, &b_MuTagIMO_exTrk_cov12); }
  if( fChain->GetBranchStatus("MuTagIMO_exTrk_cov13") ) { fChain->SetBranchAddress("MuTagIMO_exTrk_cov13", &MuTagIMO_exTrk_cov13, &b_MuTagIMO_exTrk_cov13); }
  if( fChain->GetBranchStatus("MuTagIMO_exTrk_cov14") ) { fChain->SetBranchAddress("MuTagIMO_exTrk_cov14", &MuTagIMO_exTrk_cov14, &b_MuTagIMO_exTrk_cov14); }
  if( fChain->GetBranchStatus("MuTagIMO_exTrk_cov15") ) { fChain->SetBranchAddress("MuTagIMO_exTrk_cov15", &MuTagIMO_exTrk_cov15, &b_MuTagIMO_exTrk_cov15); }
  if( fChain->GetBranchStatus("MuTagIMO_exTrk_cov22") ) { fChain->SetBranchAddress("MuTagIMO_exTrk_cov22", &MuTagIMO_exTrk_cov22, &b_MuTagIMO_exTrk_cov22); }
  if( fChain->GetBranchStatus("MuTagIMO_exTrk_cov23") ) { fChain->SetBranchAddress("MuTagIMO_exTrk_cov23", &MuTagIMO_exTrk_cov23, &b_MuTagIMO_exTrk_cov23); }
  if( fChain->GetBranchStatus("MuTagIMO_exTrk_cov24") ) { fChain->SetBranchAddress("MuTagIMO_exTrk_cov24", &MuTagIMO_exTrk_cov24, &b_MuTagIMO_exTrk_cov24); }
  if( fChain->GetBranchStatus("MuTagIMO_exTrk_cov25") ) { fChain->SetBranchAddress("MuTagIMO_exTrk_cov25", &MuTagIMO_exTrk_cov25, &b_MuTagIMO_exTrk_cov25); }
  if( fChain->GetBranchStatus("MuTagIMO_exTrk_cov33") ) { fChain->SetBranchAddress("MuTagIMO_exTrk_cov33", &MuTagIMO_exTrk_cov33, &b_MuTagIMO_exTrk_cov33); }
  if( fChain->GetBranchStatus("MuTagIMO_exTrk_cov34") ) { fChain->SetBranchAddress("MuTagIMO_exTrk_cov34", &MuTagIMO_exTrk_cov34, &b_MuTagIMO_exTrk_cov34); }
  if( fChain->GetBranchStatus("MuTagIMO_exTrk_cov35") ) { fChain->SetBranchAddress("MuTagIMO_exTrk_cov35", &MuTagIMO_exTrk_cov35, &b_MuTagIMO_exTrk_cov35); }
  if( fChain->GetBranchStatus("MuTagIMO_exTrk_cov44") ) { fChain->SetBranchAddress("MuTagIMO_exTrk_cov44", &MuTagIMO_exTrk_cov44, &b_MuTagIMO_exTrk_cov44); }
  if( fChain->GetBranchStatus("MuTagIMO_exTrk_cov45") ) { fChain->SetBranchAddress("MuTagIMO_exTrk_cov45", &MuTagIMO_exTrk_cov45, &b_MuTagIMO_exTrk_cov45); }
  if( fChain->GetBranchStatus("MuTagIMO_exTrk_cov55") ) { fChain->SetBranchAddress("MuTagIMO_exTrk_cov55", &MuTagIMO_exTrk_cov55, &b_MuTagIMO_exTrk_cov55); }
  if( fChain->GetBranchStatus("MuTagIMO_exTrk_Eloss") ) { fChain->SetBranchAddress("MuTagIMO_exTrk_Eloss", &MuTagIMO_exTrk_Eloss, &b_MuTagIMO_exTrk_Eloss); }
  if( fChain->GetBranchStatus("MuTagIMO_exTrk_gPosX") ) { fChain->SetBranchAddress("MuTagIMO_exTrk_gPosX", &MuTagIMO_exTrk_gPosX, &b_MuTagIMO_exTrk_gPosX); }
  if( fChain->GetBranchStatus("MuTagIMO_exTrk_gPosY") ) { fChain->SetBranchAddress("MuTagIMO_exTrk_gPosY", &MuTagIMO_exTrk_gPosY, &b_MuTagIMO_exTrk_gPosY); }
  if( fChain->GetBranchStatus("MuTagIMO_exTrk_gPosZ") ) { fChain->SetBranchAddress("MuTagIMO_exTrk_gPosZ", &MuTagIMO_exTrk_gPosZ, &b_MuTagIMO_exTrk_gPosZ); }
  if( fChain->GetBranchStatus("MuTagIMO_exTrk_gDirX") ) { fChain->SetBranchAddress("MuTagIMO_exTrk_gDirX", &MuTagIMO_exTrk_gDirX, &b_MuTagIMO_exTrk_gDirX); }
  if( fChain->GetBranchStatus("MuTagIMO_exTrk_gDirY") ) { fChain->SetBranchAddress("MuTagIMO_exTrk_gDirY", &MuTagIMO_exTrk_gDirY, &b_MuTagIMO_exTrk_gDirY); }
  if( fChain->GetBranchStatus("MuTagIMO_exTrk_gDirZ") ) { fChain->SetBranchAddress("MuTagIMO_exTrk_gDirZ", &MuTagIMO_exTrk_gDirZ, &b_MuTagIMO_exTrk_gDirZ); }
  if( fChain->GetBranchStatus("MuTagIMO_exTrk_locX") ) { fChain->SetBranchAddress("MuTagIMO_exTrk_locX", &MuTagIMO_exTrk_locX, &b_MuTagIMO_exTrk_locX); }
  if( fChain->GetBranchStatus("MuTagIMO_exTrk_locY") ) { fChain->SetBranchAddress("MuTagIMO_exTrk_locY", &MuTagIMO_exTrk_locY, &b_MuTagIMO_exTrk_locY); }
  if( fChain->GetBranchStatus("MuTagIMO_exTrk_angleXZ") ) { fChain->SetBranchAddress("MuTagIMO_exTrk_angleXZ", &MuTagIMO_exTrk_angleXZ, &b_MuTagIMO_exTrk_angleXZ); }
  if( fChain->GetBranchStatus("MuTagIMO_exTrk_angleYZ") ) { fChain->SetBranchAddress("MuTagIMO_exTrk_angleYZ", &MuTagIMO_exTrk_angleYZ, &b_MuTagIMO_exTrk_angleYZ); }
  if( fChain->GetBranchStatus("MuTagIMO_exTrk_locX_error") ) { fChain->SetBranchAddress("MuTagIMO_exTrk_locX_error", &MuTagIMO_exTrk_locX_error, &b_MuTagIMO_exTrk_locX_error); }
  if( fChain->GetBranchStatus("MuTagIMO_exTrk_locY_error") ) { fChain->SetBranchAddress("MuTagIMO_exTrk_locY_error", &MuTagIMO_exTrk_locY_error, &b_MuTagIMO_exTrk_locY_error); }
  if( fChain->GetBranchStatus("MuTagIMO_exTrk_angleXZ_error") ) { fChain->SetBranchAddress("MuTagIMO_exTrk_angleXZ_error", &MuTagIMO_exTrk_angleXZ_error, &b_MuTagIMO_exTrk_angleXZ_error); }
  if( fChain->GetBranchStatus("MuTagIMO_exTrk_angleYZ_error") ) { fChain->SetBranchAddress("MuTagIMO_exTrk_angleYZ_error", &MuTagIMO_exTrk_angleYZ_error, &b_MuTagIMO_exTrk_angleYZ_error); }
  if( fChain->GetBranchStatus("MuTagIMO_seg_isTagSeg") ) { fChain->SetBranchAddress("MuTagIMO_seg_isTagSeg", &MuTagIMO_seg_isTagSeg, &b_MuTagIMO_seg_isTagSeg); }
  if( fChain->GetBranchStatus("MuTagIMO_seg_matchesBarCode") ) { fChain->SetBranchAddress("MuTagIMO_seg_matchesBarCode", &MuTagIMO_seg_matchesBarCode, &b_MuTagIMO_seg_matchesBarCode); }
  if( fChain->GetBranchStatus("MuTagIMO_seg_matchesPdg") ) { fChain->SetBranchAddress("MuTagIMO_seg_matchesPdg", &MuTagIMO_seg_matchesPdg, &b_MuTagIMO_seg_matchesPdg); }
  if( fChain->GetBranchStatus("MuTagIMO_seg_exTrkIndex") ) { fChain->SetBranchAddress("MuTagIMO_seg_exTrkIndex", &MuTagIMO_seg_exTrkIndex, &b_MuTagIMO_seg_exTrkIndex); }
  if( fChain->GetBranchStatus("MuTagIMO_seg_trackIndex") ) { fChain->SetBranchAddress("MuTagIMO_seg_trackIndex", &MuTagIMO_seg_trackIndex, &b_MuTagIMO_seg_trackIndex); }
  if( fChain->GetBranchStatus("MuTagIMO_seg_stationLevel") ) { fChain->SetBranchAddress("MuTagIMO_seg_stationLevel", &MuTagIMO_seg_stationLevel, &b_MuTagIMO_seg_stationLevel); }
  if( fChain->GetBranchStatus("MuTagIMO_seg_isEndCap") ) { fChain->SetBranchAddress("MuTagIMO_seg_isEndCap", &MuTagIMO_seg_isEndCap, &b_MuTagIMO_seg_isEndCap); }
  if( fChain->GetBranchStatus("MuTagIMO_seg_isLarge") ) { fChain->SetBranchAddress("MuTagIMO_seg_isLarge", &MuTagIMO_seg_isLarge, &b_MuTagIMO_seg_isLarge); }
  if( fChain->GetBranchStatus("MuTagIMO_seg_nrHoles") ) { fChain->SetBranchAddress("MuTagIMO_seg_nrHoles", &MuTagIMO_seg_nrHoles, &b_MuTagIMO_seg_nrHoles); }
  if( fChain->GetBranchStatus("MuTagIMO_seg_nrMLs") ) { fChain->SetBranchAddress("MuTagIMO_seg_nrMLs", &MuTagIMO_seg_nrMLs, &b_MuTagIMO_seg_nrMLs); }
  if( fChain->GetBranchStatus("MuTagIMO_seg_nrMdtHits") ) { fChain->SetBranchAddress("MuTagIMO_seg_nrMdtHits", &MuTagIMO_seg_nrMdtHits, &b_MuTagIMO_seg_nrMdtHits); }
  if( fChain->GetBranchStatus("MuTagIMO_seg_nrRpcHits") ) { fChain->SetBranchAddress("MuTagIMO_seg_nrRpcHits", &MuTagIMO_seg_nrRpcHits, &b_MuTagIMO_seg_nrRpcHits); }
  if( fChain->GetBranchStatus("MuTagIMO_seg_nrTgcHits") ) { fChain->SetBranchAddress("MuTagIMO_seg_nrTgcHits", &MuTagIMO_seg_nrTgcHits, &b_MuTagIMO_seg_nrTgcHits); }
  if( fChain->GetBranchStatus("MuTagIMO_seg_nrCscHits") ) { fChain->SetBranchAddress("MuTagIMO_seg_nrCscHits", &MuTagIMO_seg_nrCscHits, &b_MuTagIMO_seg_nrCscHits); }
  if( fChain->GetBranchStatus("MuTagIMO_seg_phi") ) { fChain->SetBranchAddress("MuTagIMO_seg_phi", &MuTagIMO_seg_phi, &b_MuTagIMO_seg_phi); }
  if( fChain->GetBranchStatus("MuTagIMO_seg_theta") ) { fChain->SetBranchAddress("MuTagIMO_seg_theta", &MuTagIMO_seg_theta, &b_MuTagIMO_seg_theta); }
  if( fChain->GetBranchStatus("MuTagIMO_seg_chi2") ) { fChain->SetBranchAddress("MuTagIMO_seg_chi2", &MuTagIMO_seg_chi2, &b_MuTagIMO_seg_chi2); }
  if( fChain->GetBranchStatus("MuTagIMO_seg_cov11") ) { fChain->SetBranchAddress("MuTagIMO_seg_cov11", &MuTagIMO_seg_cov11, &b_MuTagIMO_seg_cov11); }
  if( fChain->GetBranchStatus("MuTagIMO_seg_cov12") ) { fChain->SetBranchAddress("MuTagIMO_seg_cov12", &MuTagIMO_seg_cov12, &b_MuTagIMO_seg_cov12); }
  if( fChain->GetBranchStatus("MuTagIMO_seg_cov13") ) { fChain->SetBranchAddress("MuTagIMO_seg_cov13", &MuTagIMO_seg_cov13, &b_MuTagIMO_seg_cov13); }
  if( fChain->GetBranchStatus("MuTagIMO_seg_cov14") ) { fChain->SetBranchAddress("MuTagIMO_seg_cov14", &MuTagIMO_seg_cov14, &b_MuTagIMO_seg_cov14); }
  if( fChain->GetBranchStatus("MuTagIMO_seg_cov15") ) { fChain->SetBranchAddress("MuTagIMO_seg_cov15", &MuTagIMO_seg_cov15, &b_MuTagIMO_seg_cov15); }
  if( fChain->GetBranchStatus("MuTagIMO_seg_cov22") ) { fChain->SetBranchAddress("MuTagIMO_seg_cov22", &MuTagIMO_seg_cov22, &b_MuTagIMO_seg_cov22); }
  if( fChain->GetBranchStatus("MuTagIMO_seg_cov23") ) { fChain->SetBranchAddress("MuTagIMO_seg_cov23", &MuTagIMO_seg_cov23, &b_MuTagIMO_seg_cov23); }
  if( fChain->GetBranchStatus("MuTagIMO_seg_cov24") ) { fChain->SetBranchAddress("MuTagIMO_seg_cov24", &MuTagIMO_seg_cov24, &b_MuTagIMO_seg_cov24); }
  if( fChain->GetBranchStatus("MuTagIMO_seg_cov25") ) { fChain->SetBranchAddress("MuTagIMO_seg_cov25", &MuTagIMO_seg_cov25, &b_MuTagIMO_seg_cov25); }
  if( fChain->GetBranchStatus("MuTagIMO_seg_cov33") ) { fChain->SetBranchAddress("MuTagIMO_seg_cov33", &MuTagIMO_seg_cov33, &b_MuTagIMO_seg_cov33); }
  if( fChain->GetBranchStatus("MuTagIMO_seg_cov34") ) { fChain->SetBranchAddress("MuTagIMO_seg_cov34", &MuTagIMO_seg_cov34, &b_MuTagIMO_seg_cov34); }
  if( fChain->GetBranchStatus("MuTagIMO_seg_cov35") ) { fChain->SetBranchAddress("MuTagIMO_seg_cov35", &MuTagIMO_seg_cov35, &b_MuTagIMO_seg_cov35); }
  if( fChain->GetBranchStatus("MuTagIMO_seg_cov44") ) { fChain->SetBranchAddress("MuTagIMO_seg_cov44", &MuTagIMO_seg_cov44, &b_MuTagIMO_seg_cov44); }
  if( fChain->GetBranchStatus("MuTagIMO_seg_cov45") ) { fChain->SetBranchAddress("MuTagIMO_seg_cov45", &MuTagIMO_seg_cov45, &b_MuTagIMO_seg_cov45); }
  if( fChain->GetBranchStatus("MuTagIMO_seg_cov55") ) { fChain->SetBranchAddress("MuTagIMO_seg_cov55", &MuTagIMO_seg_cov55, &b_MuTagIMO_seg_cov55); }
  if( fChain->GetBranchStatus("MuTagIMO_seg_gPosX") ) { fChain->SetBranchAddress("MuTagIMO_seg_gPosX", &MuTagIMO_seg_gPosX, &b_MuTagIMO_seg_gPosX); }
  if( fChain->GetBranchStatus("MuTagIMO_seg_gPosY") ) { fChain->SetBranchAddress("MuTagIMO_seg_gPosY", &MuTagIMO_seg_gPosY, &b_MuTagIMO_seg_gPosY); }
  if( fChain->GetBranchStatus("MuTagIMO_seg_gPosZ") ) { fChain->SetBranchAddress("MuTagIMO_seg_gPosZ", &MuTagIMO_seg_gPosZ, &b_MuTagIMO_seg_gPosZ); }
  if( fChain->GetBranchStatus("MuTagIMO_seg_gDirX") ) { fChain->SetBranchAddress("MuTagIMO_seg_gDirX", &MuTagIMO_seg_gDirX, &b_MuTagIMO_seg_gDirX); }
  if( fChain->GetBranchStatus("MuTagIMO_seg_gDirY") ) { fChain->SetBranchAddress("MuTagIMO_seg_gDirY", &MuTagIMO_seg_gDirY, &b_MuTagIMO_seg_gDirY); }
  if( fChain->GetBranchStatus("MuTagIMO_seg_gDirZ") ) { fChain->SetBranchAddress("MuTagIMO_seg_gDirZ", &MuTagIMO_seg_gDirZ, &b_MuTagIMO_seg_gDirZ); }
  if( fChain->GetBranchStatus("MuTagIMO_seg_locX") ) { fChain->SetBranchAddress("MuTagIMO_seg_locX", &MuTagIMO_seg_locX, &b_MuTagIMO_seg_locX); }
  if( fChain->GetBranchStatus("MuTagIMO_seg_locY") ) { fChain->SetBranchAddress("MuTagIMO_seg_locY", &MuTagIMO_seg_locY, &b_MuTagIMO_seg_locY); }
  if( fChain->GetBranchStatus("MuTagIMO_seg_angleXZ") ) { fChain->SetBranchAddress("MuTagIMO_seg_angleXZ", &MuTagIMO_seg_angleXZ, &b_MuTagIMO_seg_angleXZ); }
  if( fChain->GetBranchStatus("MuTagIMO_seg_angleYZ") ) { fChain->SetBranchAddress("MuTagIMO_seg_angleYZ", &MuTagIMO_seg_angleYZ, &b_MuTagIMO_seg_angleYZ); }
  if( fChain->GetBranchStatus("MuTagIMO_seg_locX_error") ) { fChain->SetBranchAddress("MuTagIMO_seg_locX_error", &MuTagIMO_seg_locX_error, &b_MuTagIMO_seg_locX_error); }
  if( fChain->GetBranchStatus("MuTagIMO_seg_locY_error") ) { fChain->SetBranchAddress("MuTagIMO_seg_locY_error", &MuTagIMO_seg_locY_error, &b_MuTagIMO_seg_locY_error); }
  if( fChain->GetBranchStatus("MuTagIMO_seg_angleXZ_error") ) { fChain->SetBranchAddress("MuTagIMO_seg_angleXZ_error", &MuTagIMO_seg_angleXZ_error, &b_MuTagIMO_seg_angleXZ_error); }
  if( fChain->GetBranchStatus("MuTagIMO_seg_angleYZ_error") ) { fChain->SetBranchAddress("MuTagIMO_seg_angleYZ_error", &MuTagIMO_seg_angleYZ_error, &b_MuTagIMO_seg_angleYZ_error); }
  if( fChain->GetBranchStatus("MuTagIMO_match_trackIndex") ) { fChain->SetBranchAddress("MuTagIMO_match_trackIndex", &MuTagIMO_match_trackIndex, &b_MuTagIMO_match_trackIndex); }
  if( fChain->GetBranchStatus("MuTagIMO_match_locX") ) { fChain->SetBranchAddress("MuTagIMO_match_locX", &MuTagIMO_match_locX, &b_MuTagIMO_match_locX); }
  if( fChain->GetBranchStatus("MuTagIMO_match_locY") ) { fChain->SetBranchAddress("MuTagIMO_match_locY", &MuTagIMO_match_locY, &b_MuTagIMO_match_locY); }
  if( fChain->GetBranchStatus("MuTagIMO_match_angleXZ") ) { fChain->SetBranchAddress("MuTagIMO_match_angleXZ", &MuTagIMO_match_angleXZ, &b_MuTagIMO_match_angleXZ); }
  if( fChain->GetBranchStatus("MuTagIMO_match_angleYZ") ) { fChain->SetBranchAddress("MuTagIMO_match_angleYZ", &MuTagIMO_match_angleYZ, &b_MuTagIMO_match_angleYZ); }
  if( fChain->GetBranchStatus("MuTagIMO_truthID_barCode") ) { fChain->SetBranchAddress("MuTagIMO_truthID_barCode", &MuTagIMO_truthID_barCode, &b_MuTagIMO_truthID_barCode); }
  if( fChain->GetBranchStatus("MuTagIMO_truthID_pdg") ) { fChain->SetBranchAddress("MuTagIMO_truthID_pdg", &MuTagIMO_truthID_pdg, &b_MuTagIMO_truthID_pdg); }
  if( fChain->GetBranchStatus("MuTagIMO_truthID_posX") ) { fChain->SetBranchAddress("MuTagIMO_truthID_posX", &MuTagIMO_truthID_posX, &b_MuTagIMO_truthID_posX); }
  if( fChain->GetBranchStatus("MuTagIMO_truthID_posY") ) { fChain->SetBranchAddress("MuTagIMO_truthID_posY", &MuTagIMO_truthID_posY, &b_MuTagIMO_truthID_posY); }
  if( fChain->GetBranchStatus("MuTagIMO_truthID_posZ") ) { fChain->SetBranchAddress("MuTagIMO_truthID_posZ", &MuTagIMO_truthID_posZ, &b_MuTagIMO_truthID_posZ); }
  if( fChain->GetBranchStatus("MuTagIMO_truthID_momX") ) { fChain->SetBranchAddress("MuTagIMO_truthID_momX", &MuTagIMO_truthID_momX, &b_MuTagIMO_truthID_momX); }
  if( fChain->GetBranchStatus("MuTagIMO_truthID_momY") ) { fChain->SetBranchAddress("MuTagIMO_truthID_momY", &MuTagIMO_truthID_momY, &b_MuTagIMO_truthID_momY); }
  if( fChain->GetBranchStatus("MuTagIMO_truthID_momZ") ) { fChain->SetBranchAddress("MuTagIMO_truthID_momZ", &MuTagIMO_truthID_momZ, &b_MuTagIMO_truthID_momZ); }
  if( fChain->GetBranchStatus("MuTagIMO_truthID_energy") ) { fChain->SetBranchAddress("MuTagIMO_truthID_energy", &MuTagIMO_truthID_energy, &b_MuTagIMO_truthID_energy); }
  if( fChain->GetBranchStatus("MuTagIMO_truthMS_barCode") ) { fChain->SetBranchAddress("MuTagIMO_truthMS_barCode", &MuTagIMO_truthMS_barCode, &b_MuTagIMO_truthMS_barCode); }
  if( fChain->GetBranchStatus("MuTagIMO_truthMS_pdg") ) { fChain->SetBranchAddress("MuTagIMO_truthMS_pdg", &MuTagIMO_truthMS_pdg, &b_MuTagIMO_truthMS_pdg); }
  if( fChain->GetBranchStatus("MuTagIMO_truthMS_posX") ) { fChain->SetBranchAddress("MuTagIMO_truthMS_posX", &MuTagIMO_truthMS_posX, &b_MuTagIMO_truthMS_posX); }
  if( fChain->GetBranchStatus("MuTagIMO_truthMS_posY") ) { fChain->SetBranchAddress("MuTagIMO_truthMS_posY", &MuTagIMO_truthMS_posY, &b_MuTagIMO_truthMS_posY); }
  if( fChain->GetBranchStatus("MuTagIMO_truthMS_posZ") ) { fChain->SetBranchAddress("MuTagIMO_truthMS_posZ", &MuTagIMO_truthMS_posZ, &b_MuTagIMO_truthMS_posZ); }
  if( fChain->GetBranchStatus("MuTagIMO_truthMS_momX") ) { fChain->SetBranchAddress("MuTagIMO_truthMS_momX", &MuTagIMO_truthMS_momX, &b_MuTagIMO_truthMS_momX); }
  if( fChain->GetBranchStatus("MuTagIMO_truthMS_momY") ) { fChain->SetBranchAddress("MuTagIMO_truthMS_momY", &MuTagIMO_truthMS_momY, &b_MuTagIMO_truthMS_momY); }
  if( fChain->GetBranchStatus("MuTagIMO_truthMS_momZ") ) { fChain->SetBranchAddress("MuTagIMO_truthMS_momZ", &MuTagIMO_truthMS_momZ, &b_MuTagIMO_truthMS_momZ); }
  if( fChain->GetBranchStatus("MuTagIMO_truthMS_energy") ) { fChain->SetBranchAddress("MuTagIMO_truthMS_energy", &MuTagIMO_truthMS_energy, &b_MuTagIMO_truthMS_energy); }
  if( fChain->GetBranchStatus("MuTagIMO_trHit_id") ) { fChain->SetBranchAddress("MuTagIMO_trHit_id", &MuTagIMO_trHit_id, &b_MuTagIMO_trHit_id); }
  if( fChain->GetBranchStatus("MuTagIMO_trHit_barCode") ) { fChain->SetBranchAddress("MuTagIMO_trHit_barCode", &MuTagIMO_trHit_barCode, &b_MuTagIMO_trHit_barCode); }
  if( fChain->GetBranchStatus("MuTagIMO_trHit_r") ) { fChain->SetBranchAddress("MuTagIMO_trHit_r", &MuTagIMO_trHit_r, &b_MuTagIMO_trHit_r); }
  if( fChain->GetBranchStatus("MuTagIMO_trHit_alongTube") ) { fChain->SetBranchAddress("MuTagIMO_trHit_alongTube", &MuTagIMO_trHit_alongTube, &b_MuTagIMO_trHit_alongTube); }
  if( fChain->GetBranchStatus("L1EM_Scale") ) { fChain->SetBranchAddress("L1EM_Scale", &L1EM_Scale, &b_L1EM_Scale); }
  if( fChain->GetBranchStatus("L1Em_nRoI") ) { fChain->SetBranchAddress("L1Em_nRoI", &L1Em_nRoI, &b_L1Em_nRoI); }
  if( fChain->GetBranchStatus("L1Em_RoIWord") ) { fChain->SetBranchAddress("L1Em_RoIWord", &L1Em_RoIWord, &b_L1Em_RoIWord); }
  if( fChain->GetBranchStatus("L1Em_Core") ) { fChain->SetBranchAddress("L1Em_Core", &L1Em_Core, &b_L1Em_Core); }
  if( fChain->GetBranchStatus("L1Em_EmClus") ) { fChain->SetBranchAddress("L1Em_EmClus", &L1Em_EmClus, &b_L1Em_EmClus); }
  if( fChain->GetBranchStatus("L1Em_TauClus") ) { fChain->SetBranchAddress("L1Em_TauClus", &L1Em_TauClus, &b_L1Em_TauClus); }
  if( fChain->GetBranchStatus("L1Em_EmIsol") ) { fChain->SetBranchAddress("L1Em_EmIsol", &L1Em_EmIsol, &b_L1Em_EmIsol); }
  if( fChain->GetBranchStatus("L1Em_HdIsol") ) { fChain->SetBranchAddress("L1Em_HdIsol", &L1Em_HdIsol, &b_L1Em_HdIsol); }
  if( fChain->GetBranchStatus("L1Em_HdCore") ) { fChain->SetBranchAddress("L1Em_HdCore", &L1Em_HdCore, &b_L1Em_HdCore); }
  if( fChain->GetBranchStatus("L1Em_EmThresh") ) { fChain->SetBranchAddress("L1Em_EmThresh", &L1Em_EmThresh, &b_L1Em_EmThresh); }
  if( fChain->GetBranchStatus("L1Em_TauThresh") ) { fChain->SetBranchAddress("L1Em_TauThresh", &L1Em_TauThresh, &b_L1Em_TauThresh); }
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
  if( fChain->GetBranchStatus("nCTP_ROI") ) { fChain->SetBranchAddress("nCTP_ROI", &nCTP_ROI, &b_nCTP_ROI); }
  if( fChain->GetBranchStatus("CTP_ROI") ) { fChain->SetBranchAddress("CTP_ROI", &CTP_ROI, &b_CTP_ROI); }
  if( fChain->GetBranchStatus("nMuCTPI_ROI") ) { fChain->SetBranchAddress("nMuCTPI_ROI", &nMuCTPI_ROI, &b_nMuCTPI_ROI); }
  if( fChain->GetBranchStatus("muCTPI_ROI") ) { fChain->SetBranchAddress("muCTPI_ROI", &muCTPI_ROI, &b_muCTPI_ROI); }
  if( fChain->GetBranchStatus("nEMTau_ROI") ) { fChain->SetBranchAddress("nEMTau_ROI", &nEMTau_ROI, &b_nEMTau_ROI); }
  if( fChain->GetBranchStatus("EMTau_ROI") ) { fChain->SetBranchAddress("EMTau_ROI", &EMTau_ROI, &b_EMTau_ROI); }
  if( fChain->GetBranchStatus("nJetEnergy_ROI") ) { fChain->SetBranchAddress("nJetEnergy_ROI", &nJetEnergy_ROI, &b_nJetEnergy_ROI); }
  if( fChain->GetBranchStatus("JetEnergy_ROI") ) { fChain->SetBranchAddress("JetEnergy_ROI", &JetEnergy_ROI, &b_JetEnergy_ROI); }
  if( fChain->GetBranchStatus("L1CaloPPM_ntt") ) { fChain->SetBranchAddress("L1CaloPPM_ntt", &L1CaloPPM_ntt, &b_L1CaloPPM_ntt); }
  if( fChain->GetBranchStatus("L1CaloPPM_eta") ) { fChain->SetBranchAddress("L1CaloPPM_eta", &L1CaloPPM_eta, &b_L1CaloPPM_eta); }
  if( fChain->GetBranchStatus("L1CaloPPM_phi") ) { fChain->SetBranchAddress("L1CaloPPM_phi", &L1CaloPPM_phi, &b_L1CaloPPM_phi); }
  if( fChain->GetBranchStatus("L1CaloPPM_emCrate") ) { fChain->SetBranchAddress("L1CaloPPM_emCrate", &L1CaloPPM_emCrate, &b_L1CaloPPM_emCrate); }
  if( fChain->GetBranchStatus("L1CaloPPM_emModule") ) { fChain->SetBranchAddress("L1CaloPPM_emModule", &L1CaloPPM_emModule, &b_L1CaloPPM_emModule); }
  if( fChain->GetBranchStatus("L1CaloPPM_emSubmodule") ) { fChain->SetBranchAddress("L1CaloPPM_emSubmodule", &L1CaloPPM_emSubmodule, &b_L1CaloPPM_emSubmodule); }
  if( fChain->GetBranchStatus("L1CaloPPM_emChannel") ) { fChain->SetBranchAddress("L1CaloPPM_emChannel", &L1CaloPPM_emChannel, &b_L1CaloPPM_emChannel); }
  if( fChain->GetBranchStatus("L1CaloPPM_hadCrate") ) { fChain->SetBranchAddress("L1CaloPPM_hadCrate", &L1CaloPPM_hadCrate, &b_L1CaloPPM_hadCrate); }
  if( fChain->GetBranchStatus("L1CaloPPM_hadModule") ) { fChain->SetBranchAddress("L1CaloPPM_hadModule", &L1CaloPPM_hadModule, &b_L1CaloPPM_hadModule); }
  if( fChain->GetBranchStatus("L1CaloPPM_hadSubmodule") ) { fChain->SetBranchAddress("L1CaloPPM_hadSubmodule", &L1CaloPPM_hadSubmodule, &b_L1CaloPPM_hadSubmodule); }
  if( fChain->GetBranchStatus("L1CaloPPM_hadChannel") ) { fChain->SetBranchAddress("L1CaloPPM_hadChannel", &L1CaloPPM_hadChannel, &b_L1CaloPPM_hadChannel); }
  if( fChain->GetBranchStatus("L1CaloPPM_emChannelId") ) { fChain->SetBranchAddress("L1CaloPPM_emChannelId", &L1CaloPPM_emChannelId, &b_L1CaloPPM_emChannelId); }
  if( fChain->GetBranchStatus("L1CaloPPM_hadChannelId") ) { fChain->SetBranchAddress("L1CaloPPM_hadChannelId", &L1CaloPPM_hadChannelId, &b_L1CaloPPM_hadChannelId); }
  if( fChain->GetBranchStatus("L1CaloPPM_emTTCellsEnergy") ) { fChain->SetBranchAddress("L1CaloPPM_emTTCellsEnergy", &L1CaloPPM_emTTCellsEnergy, &b_L1CaloPPM_emTTCellsEnergy); }
  if( fChain->GetBranchStatus("L1CaloPPM_hadTTCellsEnergy") ) { fChain->SetBranchAddress("L1CaloPPM_hadTTCellsEnergy", &L1CaloPPM_hadTTCellsEnergy, &b_L1CaloPPM_hadTTCellsEnergy); }
  if( fChain->GetBranchStatus("L1CaloPPM_emCaloWaves") ) { fChain->SetBranchAddress("L1CaloPPM_emCaloWaves", &L1CaloPPM_emCaloWaves, &b_L1CaloPPM_emCaloWaves); }
  if( fChain->GetBranchStatus("L1CaloPPM_hadCaloWaves") ) { fChain->SetBranchAddress("L1CaloPPM_hadCaloWaves", &L1CaloPPM_hadCaloWaves, &b_L1CaloPPM_hadCaloWaves); }
  if( fChain->GetBranchStatus("L1CaloPPM_emPeak") ) { fChain->SetBranchAddress("L1CaloPPM_emPeak", &L1CaloPPM_emPeak, &b_L1CaloPPM_emPeak); }
  if( fChain->GetBranchStatus("L1CaloPPM_emADCPeak") ) { fChain->SetBranchAddress("L1CaloPPM_emADCPeak", &L1CaloPPM_emADCPeak, &b_L1CaloPPM_emADCPeak); }
  if( fChain->GetBranchStatus("L1CaloPPM_hadPeak") ) { fChain->SetBranchAddress("L1CaloPPM_hadPeak", &L1CaloPPM_hadPeak, &b_L1CaloPPM_hadPeak); }
  if( fChain->GetBranchStatus("L1CaloPPM_hadADCPeak") ) { fChain->SetBranchAddress("L1CaloPPM_hadADCPeak", &L1CaloPPM_hadADCPeak, &b_L1CaloPPM_hadADCPeak); }
  if( fChain->GetBranchStatus("L1CaloPPM_emLUT") ) { fChain->SetBranchAddress("L1CaloPPM_emLUT", &L1CaloPPM_emLUT, &b_L1CaloPPM_emLUT); }
  if( fChain->GetBranchStatus("L1CaloPPM_hadLUT") ) { fChain->SetBranchAddress("L1CaloPPM_hadLUT", &L1CaloPPM_hadLUT, &b_L1CaloPPM_hadLUT); }
  if( fChain->GetBranchStatus("L1CaloPPM_emADC") ) { fChain->SetBranchAddress("L1CaloPPM_emADC", &L1CaloPPM_emADC, &b_L1CaloPPM_emADC); }
  if( fChain->GetBranchStatus("L1CaloPPM_hadADC") ) { fChain->SetBranchAddress("L1CaloPPM_hadADC", &L1CaloPPM_hadADC, &b_L1CaloPPM_hadADC); }
  if( fChain->GetBranchStatus("L1CaloPPM_emEnergy") ) { fChain->SetBranchAddress("L1CaloPPM_emEnergy", &L1CaloPPM_emEnergy, &b_L1CaloPPM_emEnergy); }
  if( fChain->GetBranchStatus("L1CaloPPM_hadEnergy") ) { fChain->SetBranchAddress("L1CaloPPM_hadEnergy", &L1CaloPPM_hadEnergy, &b_L1CaloPPM_hadEnergy); }
  if( fChain->GetBranchStatus("L1CaloPPM_emBCIDvec") ) { fChain->SetBranchAddress("L1CaloPPM_emBCIDvec", &L1CaloPPM_emBCIDvec, &b_L1CaloPPM_emBCIDvec); }
  if( fChain->GetBranchStatus("L1CaloPPM_emBCIDext") ) { fChain->SetBranchAddress("L1CaloPPM_emBCIDext", &L1CaloPPM_emBCIDext, &b_L1CaloPPM_emBCIDext); }
  if( fChain->GetBranchStatus("L1CaloPPM_hadBCIDvec") ) { fChain->SetBranchAddress("L1CaloPPM_hadBCIDvec", &L1CaloPPM_hadBCIDvec, &b_L1CaloPPM_hadBCIDvec); }
  if( fChain->GetBranchStatus("L1CaloPPM_hadBCIDext") ) { fChain->SetBranchAddress("L1CaloPPM_hadBCIDext", &L1CaloPPM_hadBCIDext, &b_L1CaloPPM_hadBCIDext); }
  if( fChain->GetBranchStatus("L1CaloPPM_emError") ) { fChain->SetBranchAddress("L1CaloPPM_emError", &L1CaloPPM_emError, &b_L1CaloPPM_emError); }
  if( fChain->GetBranchStatus("L1CaloPPM_hadError") ) { fChain->SetBranchAddress("L1CaloPPM_hadError", &L1CaloPPM_hadError, &b_L1CaloPPM_hadError); }
  if( fChain->GetBranchStatus("L1CaloPPM_emBCID") ) { fChain->SetBranchAddress("L1CaloPPM_emBCID", &L1CaloPPM_emBCID, &b_L1CaloPPM_emBCID); }
  if( fChain->GetBranchStatus("L1CaloPPM_hadBCID") ) { fChain->SetBranchAddress("L1CaloPPM_hadBCID", &L1CaloPPM_hadBCID, &b_L1CaloPPM_hadBCID); }
  if( fChain->GetBranchStatus("L1CaloPPM_emIsSaturated") ) { fChain->SetBranchAddress("L1CaloPPM_emIsSaturated", &L1CaloPPM_emIsSaturated, &b_L1CaloPPM_emIsSaturated); }
  if( fChain->GetBranchStatus("L1CaloPPM_hadIsSaturated") ) { fChain->SetBranchAddress("L1CaloPPM_hadIsSaturated", &L1CaloPPM_hadIsSaturated, &b_L1CaloPPM_hadIsSaturated); }
  if( fChain->GetBranchStatus("L1CaloPPM_emIsFilled") ) { fChain->SetBranchAddress("L1CaloPPM_emIsFilled", &L1CaloPPM_emIsFilled, &b_L1CaloPPM_emIsFilled); }
  if( fChain->GetBranchStatus("L1CaloPPM_hadIsFilled") ) { fChain->SetBranchAddress("L1CaloPPM_hadIsFilled", &L1CaloPPM_hadIsFilled, &b_L1CaloPPM_hadIsFilled); }
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
  if( fChain->GetBranchStatus("T2CaEtas1") ) { fChain->SetBranchAddress("T2CaEtas1", &T2CaEtas1, &b_T2CaEtas1); }
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
  if( fChain->GetBranchStatus("T2CaNjets") ) { fChain->SetBranchAddress("T2CaNjets", &T2CaNjets, &b_T2CaNjets); }
  if( fChain->GetBranchStatus("T2CaJetE") ) { fChain->SetBranchAddress("T2CaJetE", &T2CaJetE, &b_T2CaJetE); }
  if( fChain->GetBranchStatus("T2CaJetEhad0") ) { fChain->SetBranchAddress("T2CaJetEhad0", &T2CaJetEhad0, &b_T2CaJetEhad0); }
  if( fChain->GetBranchStatus("T2CaJetEem0") ) { fChain->SetBranchAddress("T2CaJetEem0", &T2CaJetEem0, &b_T2CaJetEem0); }
  if( fChain->GetBranchStatus("T2CaJeteta") ) { fChain->SetBranchAddress("T2CaJeteta", &T2CaJeteta, &b_T2CaJeteta); }
  if( fChain->GetBranchStatus("T2CaJetphi") ) { fChain->SetBranchAddress("T2CaJetphi", &T2CaJetphi, &b_T2CaJetphi); }
  if( fChain->GetBranchStatus("T2CaJetroi") ) { fChain->SetBranchAddress("T2CaJetroi", &T2CaJetroi, &b_T2CaJetroi); }
  if( fChain->GetBranchStatus("TrigT2MissingEt") ) { fChain->SetBranchAddress("TrigT2MissingEt", &TrigT2MissingEt, &b_TrigT2MissingEt); }
  if( fChain->GetBranchStatus("TrigT2MissingEtEt") ) { fChain->SetBranchAddress("TrigT2MissingEtEt", &TrigT2MissingEtEt, &b_TrigT2MissingEtEt); }
  if( fChain->GetBranchStatus("TrigT2MissingEtEx") ) { fChain->SetBranchAddress("TrigT2MissingEtEx", &TrigT2MissingEtEx, &b_TrigT2MissingEtEx); }
  if( fChain->GetBranchStatus("TrigT2MissingEtEy") ) { fChain->SetBranchAddress("TrigT2MissingEtEy", &TrigT2MissingEtEy, &b_TrigT2MissingEtEy); }
  if( fChain->GetBranchStatus("TrigT2MissingEtsumEt") ) { fChain->SetBranchAddress("TrigT2MissingEtsumEt", &TrigT2MissingEtsumEt, &b_TrigT2MissingEtsumEt); }
  if( fChain->GetBranchStatus("TrigT2MissingEtRoIword") ) { fChain->SetBranchAddress("TrigT2MissingEtRoIword", &TrigT2MissingEtRoIword, &b_TrigT2MissingEtRoIword); }
  if( fChain->GetBranchStatus("TrigEFMissingEt") ) { fChain->SetBranchAddress("TrigEFMissingEt", &TrigEFMissingEt, &b_TrigEFMissingEt); }
  if( fChain->GetBranchStatus("TrigEFMissingEtEt") ) { fChain->SetBranchAddress("TrigEFMissingEtEt", &TrigEFMissingEtEt, &b_TrigEFMissingEtEt); }
  if( fChain->GetBranchStatus("TrigEFMissingEtEx") ) { fChain->SetBranchAddress("TrigEFMissingEtEx", &TrigEFMissingEtEx, &b_TrigEFMissingEtEx); }
  if( fChain->GetBranchStatus("TrigEFMissingEtEy") ) { fChain->SetBranchAddress("TrigEFMissingEtEy", &TrigEFMissingEtEy, &b_TrigEFMissingEtEy); }
  if( fChain->GetBranchStatus("TrigEFMissingEtsumEt") ) { fChain->SetBranchAddress("TrigEFMissingEtsumEt", &TrigEFMissingEtsumEt, &b_TrigEFMissingEtsumEt); }
  if( fChain->GetBranchStatus("TrigEFMissingEtRoIword") ) { fChain->SetBranchAddress("TrigEFMissingEtRoIword", &TrigEFMissingEtRoIword, &b_TrigEFMissingEtRoIword); }
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
  if( fChain->GetBranchStatus("T2IdEtac") ) { fChain->SetBranchAddress("T2IdEtac", &T2IdEtac, &b_T2IdEtac); }
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
  if( fChain->GetBranchStatus("TrkEF_truthBarcode") ) { fChain->SetBranchAddress("TrkEF_truthBarcode", &TrkEF_truthBarcode, &b_TrkEF_truthBarcode); }
  if( fChain->GetBranchStatus("TrkEF_truthNt") ) { fChain->SetBranchAddress("TrkEF_truthNt", &TrkEF_truthNt, &b_TrkEF_truthNt); }
  if( fChain->GetBranchStatus("TrkEF_ParticlePdg") ) { fChain->SetBranchAddress("TrkEF_ParticlePdg", &TrkEF_ParticlePdg, &b_TrkEF_ParticlePdg); }
  if( fChain->GetBranchStatus("TrkEF_ParentPdg") ) { fChain->SetBranchAddress("TrkEF_ParentPdg", &TrkEF_ParentPdg, &b_TrkEF_ParentPdg); }
  if( fChain->GetBranchStatus("TrkEF_NumSiKineHits") ) { fChain->SetBranchAddress("TrkEF_NumSiKineHits", &TrkEF_NumSiKineHits, &b_TrkEF_NumSiKineHits); }
  if( fChain->GetBranchStatus("TrkEF_NumTRTKineHits") ) { fChain->SetBranchAddress("TrkEF_NumTRTKineHits", &TrkEF_NumTRTKineHits, &b_TrkEF_NumTRTKineHits); }
  if( fChain->GetBranchStatus("TrkEF_sctSP") ) { fChain->SetBranchAddress("TrkEF_sctSP", &TrkEF_sctSP, &b_TrkEF_sctSP); }
  if( fChain->GetBranchStatus("TrkEF_pixSP") ) { fChain->SetBranchAddress("TrkEF_pixSP", &TrkEF_pixSP, &b_TrkEF_pixSP); }
  if( fChain->GetBranchStatus("VxEF_numVertices") ) { fChain->SetBranchAddress("VxEF_numVertices", &VxEF_numVertices, &b_VxEF_numVertices); }
  if( fChain->GetBranchStatus("VxEF_vtx_x") ) { fChain->SetBranchAddress("VxEF_vtx_x", &VxEF_vtx_x, &b_VxEF_vtx_x); }
  if( fChain->GetBranchStatus("VxEF_vtx_y") ) { fChain->SetBranchAddress("VxEF_vtx_y", &VxEF_vtx_y, &b_VxEF_vtx_y); }
  if( fChain->GetBranchStatus("VxEF_vtx_z") ) { fChain->SetBranchAddress("VxEF_vtx_z", &VxEF_vtx_z, &b_VxEF_vtx_z); }
  if( fChain->GetBranchStatus("VxEF_numOfTruthVertices") ) { fChain->SetBranchAddress("VxEF_numOfTruthVertices", &VxEF_numOfTruthVertices, &b_VxEF_numOfTruthVertices); }
  if( fChain->GetBranchStatus("VxEF_vtx_x_truth") ) { fChain->SetBranchAddress("VxEF_vtx_x_truth", &VxEF_vtx_x_truth, &b_VxEF_vtx_x_truth); }
  if( fChain->GetBranchStatus("VxEF_vtx_y_truth") ) { fChain->SetBranchAddress("VxEF_vtx_y_truth", &VxEF_vtx_y_truth, &b_VxEF_vtx_y_truth); }
  if( fChain->GetBranchStatus("VxEF_vtx_z_truth") ) { fChain->SetBranchAddress("VxEF_vtx_z_truth", &VxEF_vtx_z_truth, &b_VxEF_vtx_z_truth); }
  if( fChain->GetBranchStatus("VxEF_vtx_x_res") ) { fChain->SetBranchAddress("VxEF_vtx_x_res", &VxEF_vtx_x_res, &b_VxEF_vtx_x_res); }
  if( fChain->GetBranchStatus("VxEF_vtx_y_res") ) { fChain->SetBranchAddress("VxEF_vtx_y_res", &VxEF_vtx_y_res, &b_VxEF_vtx_y_res); }
  if( fChain->GetBranchStatus("VxEF_vtx_z_res") ) { fChain->SetBranchAddress("VxEF_vtx_z_res", &VxEF_vtx_z_res, &b_VxEF_vtx_z_res); }
  if( fChain->GetBranchStatus("VxEF_sigvtx_x") ) { fChain->SetBranchAddress("VxEF_sigvtx_x", &VxEF_sigvtx_x, &b_VxEF_sigvtx_x); }
  if( fChain->GetBranchStatus("VxEF_sigvtx_y") ) { fChain->SetBranchAddress("VxEF_sigvtx_y", &VxEF_sigvtx_y, &b_VxEF_sigvtx_y); }
  if( fChain->GetBranchStatus("VxEF_sigvtx_z") ) { fChain->SetBranchAddress("VxEF_sigvtx_z", &VxEF_sigvtx_z, &b_VxEF_sigvtx_z); }
  if( fChain->GetBranchStatus("VxEF_vtxchi2") ) { fChain->SetBranchAddress("VxEF_vtxchi2", &VxEF_vtxchi2, &b_VxEF_vtxchi2); }
  if( fChain->GetBranchStatus("VxEF_vtxndf") ) { fChain->SetBranchAddress("VxEF_vtxndf", &VxEF_vtxndf, &b_VxEF_vtxndf); }
  if( fChain->GetBranchStatus("VxEF_pt") ) { fChain->SetBranchAddress("VxEF_pt", &VxEF_pt, &b_VxEF_pt); }
  if( fChain->GetBranchStatus("VxEF_vtxchi2prob") ) { fChain->SetBranchAddress("VxEF_vtxchi2prob", &VxEF_vtxchi2prob, &b_VxEF_vtxchi2prob); }
  if( fChain->GetBranchStatus("VxEF_numTracksPerVertex") ) { fChain->SetBranchAddress("VxEF_numTracksPerVertex", &VxEF_numTracksPerVertex, &b_VxEF_numTracksPerVertex); }
  if( fChain->GetBranchStatus("VxEF_vtxType") ) { fChain->SetBranchAddress("VxEF_vtxType", &VxEF_vtxType, &b_VxEF_vtxType); }
  if( fChain->GetBranchStatus("VxEF_totalNumTracks") ) { fChain->SetBranchAddress("VxEF_totalNumTracks", &VxEF_totalNumTracks, &b_VxEF_totalNumTracks); }
  if( fChain->GetBranchStatus("VxEF_chi2") ) { fChain->SetBranchAddress("VxEF_chi2", &VxEF_chi2, &b_VxEF_chi2); }
  if( fChain->GetBranchStatus("VxEF_d0") ) { fChain->SetBranchAddress("VxEF_d0", &VxEF_d0, &b_VxEF_d0); }
  if( fChain->GetBranchStatus("VxEF_sigd0") ) { fChain->SetBranchAddress("VxEF_sigd0", &VxEF_sigd0, &b_VxEF_sigd0); }
  if( fChain->GetBranchStatus("VxEF_z0") ) { fChain->SetBranchAddress("VxEF_z0", &VxEF_z0, &b_VxEF_z0); }
  if( fChain->GetBranchStatus("VxEF_sigz0") ) { fChain->SetBranchAddress("VxEF_sigz0", &VxEF_sigz0, &b_VxEF_sigz0); }
  if( fChain->GetBranchStatus("VxEF_phi") ) { fChain->SetBranchAddress("VxEF_phi", &VxEF_phi, &b_VxEF_phi); }
  if( fChain->GetBranchStatus("VxEF_sigphi") ) { fChain->SetBranchAddress("VxEF_sigphi", &VxEF_sigphi, &b_VxEF_sigphi); }
  if( fChain->GetBranchStatus("VxEF_theta") ) { fChain->SetBranchAddress("VxEF_theta", &VxEF_theta, &b_VxEF_theta); }
  if( fChain->GetBranchStatus("VxEF_sigtheta") ) { fChain->SetBranchAddress("VxEF_sigtheta", &VxEF_sigtheta, &b_VxEF_sigtheta); }
  if( fChain->GetBranchStatus("VxEF_qOverP") ) { fChain->SetBranchAddress("VxEF_qOverP", &VxEF_qOverP, &b_VxEF_qOverP); }
  if( fChain->GetBranchStatus("VxEF_sigqOverP") ) { fChain->SetBranchAddress("VxEF_sigqOverP", &VxEF_sigqOverP, &b_VxEF_sigqOverP); }
  if( fChain->GetBranchStatus("VxEF_d0g") ) { fChain->SetBranchAddress("VxEF_d0g", &VxEF_d0g, &b_VxEF_d0g); }
  if( fChain->GetBranchStatus("VxEF_sigd0g") ) { fChain->SetBranchAddress("VxEF_sigd0g", &VxEF_sigd0g, &b_VxEF_sigd0g); }
  if( fChain->GetBranchStatus("VxEF_z0g") ) { fChain->SetBranchAddress("VxEF_z0g", &VxEF_z0g, &b_VxEF_z0g); }
  if( fChain->GetBranchStatus("VxEF_sigz0g") ) { fChain->SetBranchAddress("VxEF_sigz0g", &VxEF_sigz0g, &b_VxEF_sigz0g); }
  if( fChain->GetBranchStatus("eg_RoiId_EF") ) { fChain->SetBranchAddress("eg_RoiId_EF", &eg_RoiId_EF, &b_eg_RoiId_EF); }
  if( fChain->GetBranchStatus("eg_TrigKey_EF") ) { fChain->SetBranchAddress("eg_TrigKey_EF", &eg_TrigKey_EF, &b_eg_TrigKey_EF); }
  if( fChain->GetBranchStatus("eg_nc_EF") ) { fChain->SetBranchAddress("eg_nc_EF", &eg_nc_EF, &b_eg_nc_EF); }
  if( fChain->GetBranchStatus("eg_e_EF") ) { fChain->SetBranchAddress("eg_e_EF", &eg_e_EF, &b_eg_e_EF); }
  if( fChain->GetBranchStatus("eg_eta_EF") ) { fChain->SetBranchAddress("eg_eta_EF", &eg_eta_EF, &b_eg_eta_EF); }
  if( fChain->GetBranchStatus("eg_phi_EF") ) { fChain->SetBranchAddress("eg_phi_EF", &eg_phi_EF, &b_eg_phi_EF); }
  if( fChain->GetBranchStatus("eg_author_EF") ) { fChain->SetBranchAddress("eg_author_EF", &eg_author_EF, &b_eg_author_EF); }
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
  if( fChain->GetBranchStatus("eg_Hmatrix5_EF") ) { fChain->SetBranchAddress("eg_Hmatrix5_EF", &eg_Hmatrix5_EF, &b_eg_Hmatrix5_EF); }
  if( fChain->GetBranchStatus("eg_Hmatrix10_EF") ) { fChain->SetBranchAddress("eg_Hmatrix10_EF", &eg_Hmatrix10_EF, &b_eg_Hmatrix10_EF); }
  if( fChain->GetBranchStatus("eg_IsolationLikelihood_EF") ) { fChain->SetBranchAddress("eg_IsolationLikelihood_EF", &eg_IsolationLikelihood_EF, &b_eg_IsolationLikelihood_EF); }
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
  if( fChain->GetBranchStatus("eg_bremInvpT_EF") ) { fChain->SetBranchAddress("eg_bremInvpT_EF", &eg_bremInvpT_EF, &b_eg_bremInvpT_EF); }
  if( fChain->GetBranchStatus("eg_bremRadius_EF") ) { fChain->SetBranchAddress("eg_bremRadius_EF", &eg_bremRadius_EF, &b_eg_bremRadius_EF); }
  if( fChain->GetBranchStatus("eg_bremX_EF") ) { fChain->SetBranchAddress("eg_bremX_EF", &eg_bremX_EF, &b_eg_bremX_EF); }
  if( fChain->GetBranchStatus("eg_bremClusterRadius_EF") ) { fChain->SetBranchAddress("eg_bremClusterRadius_EF", &eg_bremClusterRadius_EF, &b_eg_bremClusterRadius_EF); }
  if( fChain->GetBranchStatus("eg_bremInvpTerr_EF") ) { fChain->SetBranchAddress("eg_bremInvpTerr_EF", &eg_bremInvpTerr_EF, &b_eg_bremInvpTerr_EF); }
  if( fChain->GetBranchStatus("EFBphys_NPar") ) { fChain->SetBranchAddress("EFBphys_NPar", &EFBphys_NPar, &b_EFBphys_NPar); }
  if( fChain->GetBranchStatus("EFBphys_roiId") ) { fChain->SetBranchAddress("EFBphys_roiId", &EFBphys_roiId, &b_EFBphys_roiId); }
  if( fChain->GetBranchStatus("EFBphys_eta") ) { fChain->SetBranchAddress("EFBphys_eta", &EFBphys_eta, &b_EFBphys_eta); }
  if( fChain->GetBranchStatus("EFBphys_phi") ) { fChain->SetBranchAddress("EFBphys_phi", &EFBphys_phi, &b_EFBphys_phi); }
  if( fChain->GetBranchStatus("EFBphys_pType") ) { fChain->SetBranchAddress("EFBphys_pType", &EFBphys_pType, &b_EFBphys_pType); }
  if( fChain->GetBranchStatus("EFBphys_mass") ) { fChain->SetBranchAddress("EFBphys_mass", &EFBphys_mass, &b_EFBphys_mass); }
  if( fChain->GetBranchStatus("EFBphys_fitmass") ) { fChain->SetBranchAddress("EFBphys_fitmass", &EFBphys_fitmass, &b_EFBphys_fitmass); }
  if( fChain->GetBranchStatus("EFBphys_fitchis") ) { fChain->SetBranchAddress("EFBphys_fitchis", &EFBphys_fitchis, &b_EFBphys_fitchis); }
  if( fChain->GetBranchStatus("EFBphys_fitprob") ) { fChain->SetBranchAddress("EFBphys_fitprob", &EFBphys_fitprob, &b_EFBphys_fitprob); }
  if( fChain->GetBranchStatus("EFBphys_secmass") ) { fChain->SetBranchAddress("EFBphys_secmass", &EFBphys_secmass, &b_EFBphys_secmass); }
  if( fChain->GetBranchStatus("EFBphys_secfitmass") ) { fChain->SetBranchAddress("EFBphys_secfitmass", &EFBphys_secfitmass, &b_EFBphys_secfitmass); }
  if( fChain->GetBranchStatus("EFBphys_secfitchis") ) { fChain->SetBranchAddress("EFBphys_secfitchis", &EFBphys_secfitchis, &b_EFBphys_secfitchis); }
  if( fChain->GetBranchStatus("EFBphys_secfitprob") ) { fChain->SetBranchAddress("EFBphys_secfitprob", &EFBphys_secfitprob, &b_EFBphys_secfitprob); }
  if( fChain->GetBranchStatus("T2BjN") ) { fChain->SetBranchAddress("T2BjN", &T2BjN, &b_T2BjN); }
  if( fChain->GetBranchStatus("T2BjEta") ) { fChain->SetBranchAddress("T2BjEta", &T2BjEta, &b_T2BjEta); }
  if( fChain->GetBranchStatus("T2BjPhi0") ) { fChain->SetBranchAddress("T2BjPhi0", &T2BjPhi0, &b_T2BjPhi0); }
  if( fChain->GetBranchStatus("T2BjPvtx") ) { fChain->SetBranchAddress("T2BjPvtx", &T2BjPvtx, &b_T2BjPvtx); }
  if( fChain->GetBranchStatus("T2BjPtJet") ) { fChain->SetBranchAddress("T2BjPtJet", &T2BjPtJet, &b_T2BjPtJet); }
  if( fChain->GetBranchStatus("T2BjXIP1D") ) { fChain->SetBranchAddress("T2BjXIP1D", &T2BjXIP1D, &b_T2BjXIP1D); }
  if( fChain->GetBranchStatus("T2BjXIP2D") ) { fChain->SetBranchAddress("T2BjXIP2D", &T2BjXIP2D, &b_T2BjXIP2D); }
  if( fChain->GetBranchStatus("T2BjXIP3D") ) { fChain->SetBranchAddress("T2BjXIP3D", &T2BjXIP3D, &b_T2BjXIP3D); }
  if( fChain->GetBranchStatus("T2BjXChi2") ) { fChain->SetBranchAddress("T2BjXChi2", &T2BjXChi2, &b_T2BjXChi2); }
  if( fChain->GetBranchStatus("T2BjXSV") ) { fChain->SetBranchAddress("T2BjXSV", &T2BjXSV, &b_T2BjXSV); }
  if( fChain->GetBranchStatus("T2BjXmvtx") ) { fChain->SetBranchAddress("T2BjXmvtx", &T2BjXmvtx, &b_T2BjXmvtx); }
  if( fChain->GetBranchStatus("T2BjXevtx") ) { fChain->SetBranchAddress("T2BjXevtx", &T2BjXevtx, &b_T2BjXevtx); }
  if( fChain->GetBranchStatus("T2BjXnvtx") ) { fChain->SetBranchAddress("T2BjXnvtx", &T2BjXnvtx, &b_T2BjXnvtx); }
  if( fChain->GetBranchStatus("EFBjN") ) { fChain->SetBranchAddress("EFBjN", &EFBjN, &b_EFBjN); }
  if( fChain->GetBranchStatus("EFBjEta") ) { fChain->SetBranchAddress("EFBjEta", &EFBjEta, &b_EFBjEta); }
  if( fChain->GetBranchStatus("EFBjPhi0") ) { fChain->SetBranchAddress("EFBjPhi0", &EFBjPhi0, &b_EFBjPhi0); }
  if( fChain->GetBranchStatus("EFBjPvtx") ) { fChain->SetBranchAddress("EFBjPvtx", &EFBjPvtx, &b_EFBjPvtx); }
  if( fChain->GetBranchStatus("EFBjPtJet") ) { fChain->SetBranchAddress("EFBjPtJet", &EFBjPtJet, &b_EFBjPtJet); }
  if( fChain->GetBranchStatus("EFBjXIP1D") ) { fChain->SetBranchAddress("EFBjXIP1D", &EFBjXIP1D, &b_EFBjXIP1D); }
  if( fChain->GetBranchStatus("EFBjXIP2D") ) { fChain->SetBranchAddress("EFBjXIP2D", &EFBjXIP2D, &b_EFBjXIP2D); }
  if( fChain->GetBranchStatus("EFBjXIP3D") ) { fChain->SetBranchAddress("EFBjXIP3D", &EFBjXIP3D, &b_EFBjXIP3D); }
  if( fChain->GetBranchStatus("EFBjXChi2") ) { fChain->SetBranchAddress("EFBjXChi2", &EFBjXChi2, &b_EFBjXChi2); }
  if( fChain->GetBranchStatus("EFBjXSV") ) { fChain->SetBranchAddress("EFBjXSV", &EFBjXSV, &b_EFBjXSV); }
  if( fChain->GetBranchStatus("EFBjXmvtx") ) { fChain->SetBranchAddress("EFBjXmvtx", &EFBjXmvtx, &b_EFBjXmvtx); }
  if( fChain->GetBranchStatus("EFBjXevtx") ) { fChain->SetBranchAddress("EFBjXevtx", &EFBjXevtx, &b_EFBjXevtx); }
  if( fChain->GetBranchStatus("EFBjXnvtx") ) { fChain->SetBranchAddress("EFBjXnvtx", &EFBjXnvtx, &b_EFBjXnvtx); }
  if( fChain->GetBranchStatus("tau_L2ntaus") ) { fChain->SetBranchAddress("tau_L2ntaus", &tau_L2ntaus, &b_tau_ntaus); }
  if( fChain->GetBranchStatus("tau_L2eta") ) { fChain->SetBranchAddress("tau_L2eta", &tau_L2eta, &b_tau_L2eta); }
  if( fChain->GetBranchStatus("tau_L2phi") ) { fChain->SetBranchAddress("tau_L2phi", &tau_L2phi, &b_tau_L2phi); }
  if( fChain->GetBranchStatus("tau_L2pt") ) { fChain->SetBranchAddress("tau_L2pt", &tau_L2pt, &b_tau_L2pt); }
  if( fChain->GetBranchStatus("tau_L2Zvtx") ) { fChain->SetBranchAddress("tau_L2Zvtx", &tau_L2Zvtx, &b_tau_L2Zvtx); }
  if( fChain->GetBranchStatus("tau_L2nMatchTracks") ) { fChain->SetBranchAddress("tau_L2nMatchTracks", &tau_L2nMatchTracks, &b_tau_L2nMatchTracks); }
  if( fChain->GetBranchStatus("tau_L2roiId") ) { fChain->SetBranchAddress("tau_L2roiId", &tau_L2roiId, &b_tau_L2roiId); }
  if( fChain->GetBranchStatus("MemRSS") ) { fChain->SetBranchAddress("MemRSS", &MemRSS, &b_MemRSS); }
  if( fChain->GetBranchStatus("MemVSize") ) { fChain->SetBranchAddress("MemVSize", &MemVSize, &b_MemVSize); }
  if( fChain->GetBranchStatus("TimeTotal") ) { fChain->SetBranchAddress("TimeTotal", &TimeTotal, &b_TimeTotal); }
  if( fChain->GetBranchStatus("TimeSum") ) { fChain->SetBranchAddress("TimeSum", &TimeSum, &b_TimeSum); }
  if( fChain->GetBranchStatus("TimeEventCounter") ) { fChain->SetBranchAddress("TimeEventCounter", &TimeEventCounter, &b_TimeEventCounter); }
  if( fChain->GetBranchStatus("TimeInDetPixelClusterization") ) { fChain->SetBranchAddress("TimeInDetPixelClusterization", &TimeInDetPixelClusterization, &b_TimeInDetPixelClusterization); }
  if( fChain->GetBranchStatus("TimeInDetSCT_Clusterization") ) { fChain->SetBranchAddress("TimeInDetSCT_Clusterization", &TimeInDetSCT_Clusterization, &b_TimeInDetSCT_Clusterization); }
  if( fChain->GetBranchStatus("TimeInDetSiTrackerSpacePointFinder") ) { fChain->SetBranchAddress("TimeInDetSiTrackerSpacePointFinder", &TimeInDetSiTrackerSpacePointFinder, &b_TimeInDetSiTrackerSpacePointFinder); }
  if( fChain->GetBranchStatus("TimeInDetTRT_RIO_Maker") ) { fChain->SetBranchAddress("TimeInDetTRT_RIO_Maker", &TimeInDetTRT_RIO_Maker, &b_TimeInDetTRT_RIO_Maker); }
  if( fChain->GetBranchStatus("TimeInDetPRD_MultiTruthMaker") ) { fChain->SetBranchAddress("TimeInDetPRD_MultiTruthMaker", &TimeInDetPRD_MultiTruthMaker, &b_TimeInDetPRD_MultiTruthMaker); }
  if( fChain->GetBranchStatus("TimeInDetSiSpTrackFinder") ) { fChain->SetBranchAddress("TimeInDetSiSpTrackFinder", &TimeInDetSiSpTrackFinder, &b_TimeInDetSiSpTrackFinder); }
  if( fChain->GetBranchStatus("TimeSiSPSeededTracksDetailedTruthMaker") ) { fChain->SetBranchAddress("TimeSiSPSeededTracksDetailedTruthMaker", &TimeSiSPSeededTracksDetailedTruthMaker, &b_TimeSiSPSeededTracksDetailedTruthMaker); }
  if( fChain->GetBranchStatus("TimeSiSPSeededTracksTruthCollectionSelector") ) { fChain->SetBranchAddress("TimeSiSPSeededTracksTruthCollectionSelector", &TimeSiSPSeededTracksTruthCollectionSelector, &b_TimeSiSPSeededTracksTruthCollectionSelector); }
  if( fChain->GetBranchStatus("TimeInDetAmbiguitySolver") ) { fChain->SetBranchAddress("TimeInDetAmbiguitySolver", &TimeInDetAmbiguitySolver, &b_TimeInDetAmbiguitySolver); }
  if( fChain->GetBranchStatus("TimeResolvedTracksDetailedTruthMaker") ) { fChain->SetBranchAddress("TimeResolvedTracksDetailedTruthMaker", &TimeResolvedTracksDetailedTruthMaker, &b_TimeResolvedTracksDetailedTruthMaker); }
  if( fChain->GetBranchStatus("TimeResolvedTracksTruthCollectionSelector") ) { fChain->SetBranchAddress("TimeResolvedTracksTruthCollectionSelector", &TimeResolvedTracksTruthCollectionSelector, &b_TimeResolvedTracksTruthCollectionSelector); }
  if( fChain->GetBranchStatus("TimeInDetTRT_Extension") ) { fChain->SetBranchAddress("TimeInDetTRT_Extension", &TimeInDetTRT_Extension, &b_TimeInDetTRT_Extension); }
  if( fChain->GetBranchStatus("TimeInDetExtensionProcessor") ) { fChain->SetBranchAddress("TimeInDetExtensionProcessor", &TimeInDetExtensionProcessor, &b_TimeInDetExtensionProcessor); }
  if( fChain->GetBranchStatus("TimeExtendedTracksDetailedTruthMaker") ) { fChain->SetBranchAddress("TimeExtendedTracksDetailedTruthMaker", &TimeExtendedTracksDetailedTruthMaker, &b_TimeExtendedTracksDetailedTruthMaker); }
  if( fChain->GetBranchStatus("TimeExtendedTracksTruthCollectionSelector") ) { fChain->SetBranchAddress("TimeExtendedTracksTruthCollectionSelector", &TimeExtendedTracksTruthCollectionSelector, &b_TimeExtendedTracksTruthCollectionSelector); }
  if( fChain->GetBranchStatus("TimeInDetSegmentPRD_Association") ) { fChain->SetBranchAddress("TimeInDetSegmentPRD_Association", &TimeInDetSegmentPRD_Association, &b_TimeInDetSegmentPRD_Association); }
  if( fChain->GetBranchStatus("TimeInDetTRT_TrackSegmentsFinder") ) { fChain->SetBranchAddress("TimeInDetTRT_TrackSegmentsFinder", &TimeInDetTRT_TrackSegmentsFinder, &b_TimeInDetTRT_TrackSegmentsFinder); }
  if( fChain->GetBranchStatus("TimeInDetTRT_SeededTrackFinder") ) { fChain->SetBranchAddress("TimeInDetTRT_SeededTrackFinder", &TimeInDetTRT_SeededTrackFinder, &b_TimeInDetTRT_SeededTrackFinder); }
  if( fChain->GetBranchStatus("TimeTRTSeededTracksDetailedTruthMaker") ) { fChain->SetBranchAddress("TimeTRTSeededTracksDetailedTruthMaker", &TimeTRTSeededTracksDetailedTruthMaker, &b_TimeTRTSeededTracksDetailedTruthMaker); }
  if( fChain->GetBranchStatus("TimeTRTSeededTracksTruthCollectionSelector") ) { fChain->SetBranchAddress("TimeTRTSeededTracksTruthCollectionSelector", &TimeTRTSeededTracksTruthCollectionSelector, &b_TimeTRTSeededTracksTruthCollectionSelector); }
  if( fChain->GetBranchStatus("TimeInDetTRT_SeededAmbiguitySolver") ) { fChain->SetBranchAddress("TimeInDetTRT_SeededAmbiguitySolver", &TimeInDetTRT_SeededAmbiguitySolver, &b_TimeInDetTRT_SeededAmbiguitySolver); }
  if( fChain->GetBranchStatus("TimeResolvedTRTSeededTracksDetailedTruthMaker") ) { fChain->SetBranchAddress("TimeResolvedTRTSeededTracksDetailedTruthMaker", &TimeResolvedTRTSeededTracksDetailedTruthMaker, &b_TimeResolvedTRTSeededTracksDetailedTruthMaker); }
  if( fChain->GetBranchStatus("TimeResolvedTRTSeededTracksTruthCollectionSelector") ) { fChain->SetBranchAddress("TimeResolvedTRTSeededTracksTruthCollectionSelector", &TimeResolvedTRTSeededTracksTruthCollectionSelector, &b_TimeResolvedTRTSeededTracksTruthCollectionSelector); }
  if( fChain->GetBranchStatus("TimeInDetTRTonly_PRD_Association") ) { fChain->SetBranchAddress("TimeInDetTRTonly_PRD_Association", &TimeInDetTRTonly_PRD_Association, &b_TimeInDetTRTonly_PRD_Association); }
  if( fChain->GetBranchStatus("TimeInDetTRT_StandaloneTrackFinder") ) { fChain->SetBranchAddress("TimeInDetTRT_StandaloneTrackFinder", &TimeInDetTRT_StandaloneTrackFinder, &b_TimeInDetTRT_StandaloneTrackFinder); }
  if( fChain->GetBranchStatus("TimeTRTStandaloneTracksDetailedTruthMaker") ) { fChain->SetBranchAddress("TimeTRTStandaloneTracksDetailedTruthMaker", &TimeTRTStandaloneTracksDetailedTruthMaker, &b_TimeTRTStandaloneTracksDetailedTruthMaker); }
  if( fChain->GetBranchStatus("TimeTRTStandaloneTracksTruthCollectionSelector") ) { fChain->SetBranchAddress("TimeTRTStandaloneTracksTruthCollectionSelector", &TimeTRTStandaloneTracksTruthCollectionSelector, &b_TimeTRTStandaloneTracksTruthCollectionSelector); }
  if( fChain->GetBranchStatus("TimeInDetTrackCollectionMerger") ) { fChain->SetBranchAddress("TimeInDetTrackCollectionMerger", &TimeInDetTrackCollectionMerger, &b_TimeInDetTrackCollectionMerger); }
  if( fChain->GetBranchStatus("TimeCombinedInDetTracksDetailedTruthMaker") ) { fChain->SetBranchAddress("TimeCombinedInDetTracksDetailedTruthMaker", &TimeCombinedInDetTracksDetailedTruthMaker, &b_TimeCombinedInDetTracksDetailedTruthMaker); }
  if( fChain->GetBranchStatus("TimeCombinedInDetTracksTruthCollectionSelector") ) { fChain->SetBranchAddress("TimeCombinedInDetTracksTruthCollectionSelector", &TimeCombinedInDetTracksTruthCollectionSelector, &b_TimeCombinedInDetTracksTruthCollectionSelector); }
  if( fChain->GetBranchStatus("TimeInDetTrackSlimmer") ) { fChain->SetBranchAddress("TimeInDetTrackSlimmer", &TimeInDetTrackSlimmer, &b_TimeInDetTrackSlimmer); }
  if( fChain->GetBranchStatus("TimeDetailedTrackTruthMaker") ) { fChain->SetBranchAddress("TimeDetailedTrackTruthMaker", &TimeDetailedTrackTruthMaker, &b_TimeDetailedTrackTruthMaker); }
  if( fChain->GetBranchStatus("TimeTrackTruthCollectionSelector") ) { fChain->SetBranchAddress("TimeTrackTruthCollectionSelector", &TimeTrackTruthCollectionSelector, &b_TimeTrackTruthCollectionSelector); }
  if( fChain->GetBranchStatus("TimeInDetPriVxFinder") ) { fChain->SetBranchAddress("TimeInDetPriVxFinder", &TimeInDetPriVxFinder, &b_TimeInDetPriVxFinder); }
  if( fChain->GetBranchStatus("TimeInDetParticleCreation") ) { fChain->SetBranchAddress("TimeInDetParticleCreation", &TimeInDetParticleCreation, &b_TimeInDetParticleCreation); }
  if( fChain->GetBranchStatus("TimeInDetTrackParticleTruthMaker") ) { fChain->SetBranchAddress("TimeInDetTrackParticleTruthMaker", &TimeInDetTrackParticleTruthMaker, &b_TimeInDetTrackParticleTruthMaker); }
  if( fChain->GetBranchStatus("TimeInDetSecVtxFinder") ) { fChain->SetBranchAddress("TimeInDetSecVtxFinder", &TimeInDetSecVtxFinder, &b_TimeInDetSecVtxFinder); }
  if( fChain->GetBranchStatus("TimeInDetConversionFinder") ) { fChain->SetBranchAddress("TimeInDetConversionFinder", &TimeInDetConversionFinder, &b_TimeInDetConversionFinder); }
  if( fChain->GetBranchStatus("TimeInDetRecStatistics") ) { fChain->SetBranchAddress("TimeInDetRecStatistics", &TimeInDetRecStatistics, &b_TimeInDetRecStatistics); }
  if( fChain->GetBranchStatus("TimeInDetSegmentDriftCircleAssValidation") ) { fChain->SetBranchAddress("TimeInDetSegmentDriftCircleAssValidation", &TimeInDetSegmentDriftCircleAssValidation, &b_TimeInDetSegmentDriftCircleAssValidation); }
  if( fChain->GetBranchStatus("TimeCaloCellMaker") ) { fChain->SetBranchAddress("TimeCaloCellMaker", &TimeCaloCellMaker, &b_TimeCaloCellMaker); }
  if( fChain->GetBranchStatus("TimeCaloClusterTopoEM430Getter") ) { fChain->SetBranchAddress("TimeCaloClusterTopoEM430Getter", &TimeCaloClusterTopoEM430Getter, &b_TimeCaloClusterTopoEM430Getter); }
  if( fChain->GetBranchStatus("TimeEMCell2TopoCluster430") ) { fChain->SetBranchAddress("TimeEMCell2TopoCluster430", &TimeEMCell2TopoCluster430, &b_TimeEMCell2TopoCluster430); }
  if( fChain->GetBranchStatus("TimeCscRdoToCscPrepData") ) { fChain->SetBranchAddress("TimeCscRdoToCscPrepData", &TimeCscRdoToCscPrepData, &b_TimeCscRdoToCscPrepData); }
  if( fChain->GetBranchStatus("TimeMdtRdoToMdtPrepData") ) { fChain->SetBranchAddress("TimeMdtRdoToMdtPrepData", &TimeMdtRdoToMdtPrepData, &b_TimeMdtRdoToMdtPrepData); }
  if( fChain->GetBranchStatus("TimeRpcRdoToRpcPrepData") ) { fChain->SetBranchAddress("TimeRpcRdoToRpcPrepData", &TimeRpcRdoToRpcPrepData, &b_TimeRpcRdoToRpcPrepData); }
  if( fChain->GetBranchStatus("TimeTgcRdoToTgcPrepData") ) { fChain->SetBranchAddress("TimeTgcRdoToTgcPrepData", &TimeTgcRdoToTgcPrepData, &b_TimeTgcRdoToTgcPrepData); }
  if( fChain->GetBranchStatus("TimeCscThresholdClusterBuilder") ) { fChain->SetBranchAddress("TimeCscThresholdClusterBuilder", &TimeCscThresholdClusterBuilder, &b_TimeCscThresholdClusterBuilder); }
  if( fChain->GetBranchStatus("TimeTrackRecordFilter") ) { fChain->SetBranchAddress("TimeTrackRecordFilter", &TimeTrackRecordFilter, &b_TimeTrackRecordFilter); }
  if( fChain->GetBranchStatus("TimeTileLookForMuAlg") ) { fChain->SetBranchAddress("TimeTileLookForMuAlg", &TimeTileLookForMuAlg, &b_TimeTileLookForMuAlg); }
  if( fChain->GetBranchStatus("TimeInitializeMuonClusters") ) { fChain->SetBranchAddress("TimeInitializeMuonClusters", &TimeInitializeMuonClusters, &b_TimeInitializeMuonClusters); }
  if( fChain->GetBranchStatus("TimeCaloTrkMuIdAlg") ) { fChain->SetBranchAddress("TimeCaloTrkMuIdAlg", &TimeCaloTrkMuIdAlg, &b_TimeCaloTrkMuIdAlg); }
  if( fChain->GetBranchStatus("TimeMuGirl") ) { fChain->SetBranchAddress("TimeMuGirl", &TimeMuGirl, &b_TimeMuGirl); }
  if( fChain->GetBranchStatus("TimeMuidStatistics") ) { fChain->SetBranchAddress("TimeMuidStatistics", &TimeMuidStatistics, &b_TimeMuidStatistics); }
  if( fChain->GetBranchStatus("TimeMergeMuonCollections") ) { fChain->SetBranchAddress("TimeMergeMuonCollections", &TimeMergeMuonCollections, &b_TimeMergeMuonCollections); }
  if( fChain->GetBranchStatus("TimeMakeAODMuons") ) { fChain->SetBranchAddress("TimeMakeAODMuons", &TimeMakeAODMuons, &b_TimeMakeAODMuons); }
  if( fChain->GetBranchStatus("TimeFinalizeMuonClusters") ) { fChain->SetBranchAddress("TimeFinalizeMuonClusters", &TimeFinalizeMuonClusters, &b_TimeFinalizeMuonClusters); }
  if( fChain->GetBranchStatus("TimeBTagSharedHitMapper") ) { fChain->SetBranchAddress("TimeBTagSharedHitMapper", &TimeBTagSharedHitMapper, &b_TimeBTagSharedHitMapper); }
  if( fChain->GetBranchStatus("TimeCmbTowerBldr") ) { fChain->SetBranchAddress("TimeCmbTowerBldr", &TimeCmbTowerBldr, &b_TimeCmbTowerBldr); }
  if( fChain->GetBranchStatus("TimeTowerJetInput") ) { fChain->SetBranchAddress("TimeTowerJetInput", &TimeTowerJetInput, &b_TimeTowerJetInput); }
  if( fChain->GetBranchStatus("TimeCone7H1TowerJets") ) { fChain->SetBranchAddress("TimeCone7H1TowerJets", &TimeCone7H1TowerJets, &b_TimeCone7H1TowerJets); }
  if( fChain->GetBranchStatus("TimeCone4H1TowerJets") ) { fChain->SetBranchAddress("TimeCone4H1TowerJets", &TimeCone4H1TowerJets, &b_TimeCone4H1TowerJets); }
  if( fChain->GetBranchStatus("TimeCone4H1TopoJets") ) { fChain->SetBranchAddress("TimeCone4H1TopoJets", &TimeCone4H1TopoJets, &b_TimeCone4H1TopoJets); }
  if( fChain->GetBranchStatus("TimeEsdTruthParticlesBuilder") ) { fChain->SetBranchAddress("TimeEsdTruthParticlesBuilder", &TimeEsdTruthParticlesBuilder, &b_TimeEsdTruthParticlesBuilder); }
  if( fChain->GetBranchStatus("TimeCone4TruthJets") ) { fChain->SetBranchAddress("TimeCone4TruthJets", &TimeCone4TruthJets, &b_TimeCone4TruthJets); }
  if( fChain->GetBranchStatus("TimeTrigConfDataIOVChanger") ) { fChain->SetBranchAddress("TimeTrigConfDataIOVChanger", &TimeTrigConfDataIOVChanger, &b_TimeTrigConfDataIOVChanger); }
  if( fChain->GetBranchStatus("TimeTriggerTowerMaker") ) { fChain->SetBranchAddress("TimeTriggerTowerMaker", &TimeTriggerTowerMaker, &b_TimeTriggerTowerMaker); }
  if( fChain->GetBranchStatus("TimeCPMTowerMaker") ) { fChain->SetBranchAddress("TimeCPMTowerMaker", &TimeCPMTowerMaker, &b_TimeCPMTowerMaker); }
  if( fChain->GetBranchStatus("TimeJetElementMaker") ) { fChain->SetBranchAddress("TimeJetElementMaker", &TimeJetElementMaker, &b_TimeJetElementMaker); }
  if( fChain->GetBranchStatus("TimeEmTauTrigger") ) { fChain->SetBranchAddress("TimeEmTauTrigger", &TimeEmTauTrigger, &b_TimeEmTauTrigger); }
  if( fChain->GetBranchStatus("TimeJetTrigger") ) { fChain->SetBranchAddress("TimeJetTrigger", &TimeJetTrigger, &b_TimeJetTrigger); }
  if( fChain->GetBranchStatus("TimeEnergyTrigger") ) { fChain->SetBranchAddress("TimeEnergyTrigger", &TimeEnergyTrigger, &b_TimeEnergyTrigger); }
  if( fChain->GetBranchStatus("TimeROD") ) { fChain->SetBranchAddress("TimeROD", &TimeROD, &b_TimeROD); }
  if( fChain->GetBranchStatus("TimeCPCMMMaker") ) { fChain->SetBranchAddress("TimeCPCMMMaker", &TimeCPCMMMaker, &b_TimeCPCMMMaker); }
  if( fChain->GetBranchStatus("TimeJEPCMMMaker") ) { fChain->SetBranchAddress("TimeJEPCMMMaker", &TimeJEPCMMMaker, &b_TimeJEPCMMMaker); }
  if( fChain->GetBranchStatus("TimeLVL1TrigT1MBTS") ) { fChain->SetBranchAddress("TimeLVL1TrigT1MBTS", &TimeLVL1TrigT1MBTS, &b_TimeLVL1TrigT1MBTS); }
  if( fChain->GetBranchStatus("TimeMuonRdoToMuonDigit") ) { fChain->SetBranchAddress("TimeMuonRdoToMuonDigit", &TimeMuonRdoToMuonDigit, &b_TimeMuonRdoToMuonDigit); }
  if( fChain->GetBranchStatus("TimeTrigT1RPC") ) { fChain->SetBranchAddress("TimeTrigT1RPC", &TimeTrigT1RPC, &b_TimeTrigT1RPC); }
  if( fChain->GetBranchStatus("TimeLVL1TGCTriggerLVL1TGCTrigger") ) { fChain->SetBranchAddress("TimeLVL1TGCTriggerLVL1TGCTrigger", &TimeLVL1TGCTriggerLVL1TGCTrigger, &b_TimeLVL1TGCTriggerLVL1TGCTrigger); }
  if( fChain->GetBranchStatus("TimeL1Muctpi") ) { fChain->SetBranchAddress("TimeL1Muctpi", &TimeL1Muctpi, &b_TimeL1Muctpi); }
  if( fChain->GetBranchStatus("TimeLVL1TrigT1BCM") ) { fChain->SetBranchAddress("TimeLVL1TrigT1BCM", &TimeLVL1TrigT1BCM, &b_TimeLVL1TrigT1BCM); }
  if( fChain->GetBranchStatus("TimeLVL1TrigT1Lucid") ) { fChain->SetBranchAddress("TimeLVL1TrigT1Lucid", &TimeLVL1TrigT1Lucid, &b_TimeLVL1TrigT1Lucid); }
  if( fChain->GetBranchStatus("TimeCTPSimulation") ) { fChain->SetBranchAddress("TimeCTPSimulation", &TimeCTPSimulation, &b_TimeCTPSimulation); }
  if( fChain->GetBranchStatus("TimeRoIBuilder") ) { fChain->SetBranchAddress("TimeRoIBuilder", &TimeRoIBuilder, &b_TimeRoIBuilder); }
  if( fChain->GetBranchStatus("TimeRoIBResultToAOD") ) { fChain->SetBranchAddress("TimeRoIBResultToAOD", &TimeRoIBResultToAOD, &b_TimeRoIBResultToAOD); }
  if( fChain->GetBranchStatus("TimeStreamBS") ) { fChain->SetBranchAddress("TimeStreamBS", &TimeStreamBS, &b_TimeStreamBS); }
  if( fChain->GetBranchStatus("TimeTrigSteer_L2") ) { fChain->SetBranchAddress("TimeTrigSteer_L2", &TimeTrigSteer_L2, &b_TimeTrigSteer_L2); }
  if( fChain->GetBranchStatus("TimeTrigSteer_EF") ) { fChain->SetBranchAddress("TimeTrigSteer_EF", &TimeTrigSteer_EF, &b_TimeTrigSteer_EF); }
  if( fChain->GetBranchStatus("TimeTrigDecMaker") ) { fChain->SetBranchAddress("TimeTrigDecMaker", &TimeTrigDecMaker, &b_TimeTrigDecMaker); }
  if( fChain->GetBranchStatus("TimeTrigInDetTrackTruthMaker") ) { fChain->SetBranchAddress("TimeTrigInDetTrackTruthMaker", &TimeTrigInDetTrackTruthMaker, &b_TimeTrigInDetTrackTruthMaker); }
  if( fChain->GetBranchStatus("TimeCBNT_AthenaAware") ) { fChain->SetBranchAddress("TimeCBNT_AthenaAware", &TimeCBNT_AthenaAware, &b_TimeCBNT_AthenaAware); }
  if( fChain->GetBranchStatus("TimeCreateLumiBlockCollectionFromFile") ) { fChain->SetBranchAddress("TimeCreateLumiBlockCollectionFromFile", &TimeCreateLumiBlockCollectionFromFile, &b_TimeCreateLumiBlockCollectionFromFile); }
  if( fChain->GetBranchStatus("TimeStreamESD") ) { fChain->SetBranchAddress("TimeStreamESD", &TimeStreamESD, &b_TimeStreamESD); }
  if( fChain->GetBranchStatus("TimeStreamESD_FH") ) { fChain->SetBranchAddress("TimeStreamESD_FH", &TimeStreamESD_FH, &b_TimeStreamESD_FH); }
  if( fChain->GetBranchStatus("TimeAANTupleStream") ) { fChain->SetBranchAddress("TimeAANTupleStream", &TimeAANTupleStream, &b_TimeAANTupleStream); }
  if( fChain->GetBranchStatus("TimeDumpSp") ) { fChain->SetBranchAddress("TimeDumpSp", &TimeDumpSp, &b_TimeDumpSp); }

  Notify();
}

Bool_t WHTreeV14::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void WHTreeV14::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t WHTreeV14::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef WHTreeV14_cxx
