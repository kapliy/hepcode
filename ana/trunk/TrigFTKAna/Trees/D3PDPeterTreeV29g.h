//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue Feb 21 09:18:12 2012 by ROOT version 5.28/00e
// from TTree tree/Muon tree
// found on file: user.kapliy.008576.flatntuple._00068.root
//////////////////////////////////////////////////////////

#ifndef D3PDPeterTreeV29g_h
#define D3PDPeterTreeV29g_h

#include "TrigFTKAna_config.h"
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <vector>
#include <string>
#include "TrigFTKAna/Trees/D3PDPeterTreeBase.h"

class
D3PDPeterTreeV29g : public D3PDPeterTreeBase
{
 public:
  enum {
    nele_max = 10000,
    njet_max = 10000,
    njpsi_max = 10000,
    nmc_max = 100000,
    nmu_max = 10000,
    nph_max = 10000,
    ntrk_max = 10000,
    nvx_max = 10000,
    matchvector_max = 100000
  };
 public:
  TTree *fChain; //!pointer to the analyzed TTree or TChain
  Int_t fCurrent; //!current Tree number in a TChain

  // Declaration of leaf types
  UInt_t run;
  UInt_t event;
  UInt_t lb;
  UInt_t bcid;
  Float_t lumi_mu_actual;
  Float_t lumi_mu_average;
  Int_t bdff;
  Int_t bfif600;
  Float_t bint;
  UInt_t error_lar;
  UInt_t error_background;
  std::vector<std::string> *trig_ef;
  std::vector<std::string> *trig_l2;
  std::vector<std::string> *trig_l1;
  Float_t bs_x;
  Float_t bs_y;
  Float_t bs_z;
  Float_t bs_status;
  Int_t nvx;
  Int_t vx_type[nvx_max]; //[nvx]
  Int_t vx_ntracks[nvx_max]; //[nvx]
  Float_t vx_sumpt[nvx_max]; //[nvx]
  Float_t vx_x[nvx_max]; //[nvx]
  Float_t vx_y[nvx_max]; //[nvx]
  Float_t vx_z[nvx_max]; //[nvx]
  Int_t nmu;
  Short_t mu_type[nmu_max]; //[nmu]
  Float_t mu_pt[nmu_max]; //[nmu]
  Float_t mu_eta[nmu_max]; //[nmu]
  Float_t mu_phi[nmu_max]; //[nmu]
  Int_t mu_q[nmu_max]; //[nmu]
  Float_t mu_ptcid[nmu_max]; //[nmu]
  Float_t mu_etacid[nmu_max]; //[nmu]
  Float_t mu_phicid[nmu_max]; //[nmu]
  Int_t mu_qcid[nmu_max]; //[nmu]
  Float_t mu_ptid[nmu_max]; //[nmu]
  Float_t mu_etaid[nmu_max]; //[nmu]
  Float_t mu_phiid[nmu_max]; //[nmu]
  Int_t mu_qid[nmu_max]; //[nmu]
  Float_t mu_ptexid[nmu_max]; //[nmu]
  Float_t mu_etaexid[nmu_max]; //[nmu]
  Float_t mu_phiexid[nmu_max]; //[nmu]
  Int_t mu_qexid[nmu_max]; //[nmu]
  Float_t mu_ptms[nmu_max]; //[nmu]
  Float_t mu_etams[nmu_max]; //[nmu]
  Float_t mu_phims[nmu_max]; //[nmu]
  Int_t mu_qms[nmu_max]; //[nmu]
  Float_t mu_ptexms[nmu_max]; //[nmu]
  Float_t mu_etaexms[nmu_max]; //[nmu]
  Float_t mu_phiexms[nmu_max]; //[nmu]
  Int_t mu_qexms[nmu_max]; //[nmu]
  UShort_t mu_author[nmu_max]; //[nmu]
  UShort_t mu_primauthor[nmu_max]; //[nmu]
  Short_t mu_class[nmu_max]; //[nmu]
  Short_t mu_iscombined[nmu_max]; //[nmu]
  Short_t mu_issegtagged[nmu_max]; //[nmu]
  Float_t mu_fitchi2prob[nmu_max]; //[nmu]
  Float_t mu_matchchi2prob[nmu_max]; //[nmu]
  Float_t mu_fitchi2[nmu_max]; //[nmu]
  Float_t mu_matchchi2[nmu_max]; //[nmu]
  Int_t mu_bestmatch[nmu_max]; //[nmu]
  Int_t mu_nucone10[nmu_max]; //[nmu]
  Int_t mu_nucone20[nmu_max]; //[nmu]
  Int_t mu_nucone30[nmu_max]; //[nmu]
  Int_t mu_nucone40[nmu_max]; //[nmu]
  Float_t mu_etcone10[nmu_max]; //[nmu]
  Float_t mu_etcone20[nmu_max]; //[nmu]
  Float_t mu_etcone30[nmu_max]; //[nmu]
  Float_t mu_etcone40[nmu_max]; //[nmu]
  Float_t mu_ptcone10[nmu_max]; //[nmu]
  Float_t mu_ptcone20[nmu_max]; //[nmu]
  Float_t mu_ptcone30[nmu_max]; //[nmu]
  Float_t mu_ptcone40[nmu_max]; //[nmu]
  Float_t mu_ptconemod10[nmu_max]; //[nmu]
  Float_t mu_ptconemod20[nmu_max]; //[nmu]
  Float_t mu_ptconemod30[nmu_max]; //[nmu]
  Float_t mu_ptconemod40[nmu_max]; //[nmu]
  Float_t mu_eloss[nmu_max]; //[nmu]
  Float_t mu_d0[nmu_max]; //[nmu]
  Float_t mu_z0[nmu_max]; //[nmu]
  Float_t mu_db[nmu_max]; //[nmu]
  Float_t mu_zb[nmu_max]; //[nmu]
  Float_t mu_dvx[nmu_max]; //[nmu]
  Float_t mu_zvx[nmu_max]; //[nmu]
  Float_t mu_dxpv[nmu_max]; //[nmu]
  Float_t mu_zxpv[nmu_max]; //[nmu]
  Float_t mu_covdb[nmu_max]; //[nmu]
  Float_t mu_covdvx[nmu_max]; //[nmu]
  Float_t mu_covdxpv[nmu_max]; //[nmu]
  Float_t mu_phiid_expv[nmu_max];   //[nmu]
  Float_t mu_phiexms_expv[nmu_max];   //[nmu]
  Float_t mu_thetaid_expv[nmu_max];   //[nmu]
  Float_t mu_thetaexms_expv[nmu_max];   //[nmu]
  Float_t mu_qoverpid_expv[nmu_max];   //[nmu]
  Float_t mu_qoverpexms_expv[nmu_max];   //[nmu]
  Float_t mu_covqoverp_expv[nmu_max];   //[nmu]
  Float_t mu_covqoverpid_expv[nmu_max];   //[nmu]
  Float_t mu_covqoverpexms_expv[nmu_max];   //[nmu]
  Int_t mu_vtxtype[nmu_max]; //[nmu]
  Int_t mu_vtx[nmu_max]; //[nmu]
  Int_t mu_nhitbl[nmu_max]; //[nmu]
  Short_t mu_expectbl[nmu_max]; //[nmu]
  Int_t mu_nhitpix[nmu_max]; //[nmu]
  Int_t mu_nholespix[nmu_max]; //[nmu]
  Int_t mu_ndeadsensorspix[nmu_max]; //[nmu]
  Int_t mu_nhitsct[nmu_max]; //[nmu]
  Int_t mu_nholessct[nmu_max]; //[nmu]
  Int_t mu_ndeadsensorssct[nmu_max]; //[nmu]
  Int_t mu_nhittrt[nmu_max]; //[nmu]
  Int_t mu_nholestrt[nmu_max]; //[nmu]
  Int_t mu_nhitmdt[nmu_max]; //[nmu]
  Int_t mu_nhitcsceta[nmu_max]; //[nmu]
  Int_t mu_nhitcscphi[nmu_max]; //[nmu]
  Int_t mu_nhitrpceta[nmu_max]; //[nmu]
  Int_t mu_nhitrpcphi[nmu_max]; //[nmu]
  Int_t mu_nhittgceta[nmu_max]; //[nmu]
  Int_t mu_nhittgcphi[nmu_max]; //[nmu]
  Int_t mu_noutlierstrt[nmu_max]; //[nmu]
  Int_t mu_nhitcscunspoilt[nmu_max]; //[nmu]
  Int_t mu_nMDTBIHits[nmu_max]; //[nmu]
  Int_t mu_nMDTBMHits[nmu_max]; //[nmu]
  Int_t mu_nMDTBOHits[nmu_max]; //[nmu]
  Int_t mu_nMDTBEEHits[nmu_max]; //[nmu]
  Int_t mu_nMDTBIS78Hits[nmu_max]; //[nmu]
  Int_t mu_nMDTEIHits[nmu_max]; //[nmu]
  Int_t mu_nMDTEMHits[nmu_max]; //[nmu]
  Int_t mu_nMDTEOHits[nmu_max]; //[nmu]
  Int_t mu_nMDTEEHits[nmu_max]; //[nmu]
  UShort_t mu_quality[nmu_max]; //[nmu]
  Float_t mu_scatt_curv_sig[nmu_max]; //[nmu]
  Float_t mu_scatt_neighb_sig[nmu_max]; //[nmu]
  Float_t mu_field_integral[nmu_max]; //[nmu]
#ifndef DISABLE_TRIGGER_MATCHING
  Int_t mu_trig_l1matchvector_;
  Int_t mu_trig_l1matchvector_first[matchvector_max]; //[mu_trig_l1matchvector_]
  Int_t mu_trig_l1matchvector_second[matchvector_max]; //[mu_trig_l1matchvector_]
  std::vector<float> *mu_trig_l1matchdelr;
  Int_t mu_trig_l2matchvector_;
  Int_t mu_trig_l2matchvector_first[matchvector_max]; //[mu_trig_l2matchvector_]
  Int_t mu_trig_l2matchvector_second[matchvector_max]; //[mu_trig_l2matchvector_]
  std::vector<float> *mu_trig_l2matchdelr;
  Int_t mu_trig_l2samatchvector_;
  Int_t mu_trig_l2samatchvector_first[matchvector_max]; //[mu_trig_l2samatchvector_]
  Int_t mu_trig_l2samatchvector_second[matchvector_max]; //[mu_trig_l2samatchvector_]
  std::vector<float> *mu_trig_l2samatchdelr;
  Int_t mu_trig_efmatchvector_;
  Int_t mu_trig_efmatchvector_first[matchvector_max]; //[mu_trig_efmatchvector_]
  Int_t mu_trig_efmatchvector_second[matchvector_max]; //[mu_trig_efmatchvector_]
  std::vector<float> *mu_trig_efmatchdelr;
#endif
  Int_t mu_passes_mcp[nmu_max]; //[nmu]
  Float_t mu_MET_wet[nmu_max]; //[nmu]
  Float_t mu_MET_wpx[nmu_max]; //[nmu]
  Float_t mu_MET_wpy[nmu_max]; //[nmu]
  Float_t mu_MET_status[nmu_max]; //[nmu]
  Float_t mu_MET_wet2[nmu_max]; //[nmu]
  Float_t mu_MET_wpx2[nmu_max]; //[nmu]
  Float_t mu_MET_wpy2[nmu_max]; //[nmu]
  Float_t mu_MET_status2[nmu_max]; //[nmu]
  Int_t mu_ptype[nmu_max]; //[nmu]
  Int_t mu_porigin[nmu_max]; //[nmu]
  Int_t mu_poutcome[nmu_max]; //[nmu]
  Int_t nele;
  UShort_t ele_author[nele_max]; //[nele]
  Float_t ele_eta[nele_max]; //[nele]
  Float_t ele_phi[nele_max]; //[nele]
  Float_t ele_et[nele_max]; //[nele]
  Int_t ele_q[nele_max]; //[nele]
  Float_t ele_etaclus[nele_max]; //[nele]
  Float_t ele_phiclus[nele_max]; //[nele]
  Float_t ele_etclus[nele_max]; //[nele]
  Float_t ele_etatrk[nele_max]; //[nele]
  Float_t ele_phitrk[nele_max]; //[nele]
  Float_t ele_pttrk[nele_max]; //[nele]
  Float_t ele_etabest[nele_max]; //[nele]
  Float_t ele_phibest[nele_max]; //[nele]
  Float_t ele_etbest[nele_max]; //[nele]
  Float_t ele_etap[nele_max]; //[nele]
  Float_t ele_f1[nele_max]; //[nele]
  UInt_t ele_isEM[nele_max]; //[nele]
  Int_t ele_nucone20[nele_max]; //[nele]
  Int_t ele_nucone30[nele_max]; //[nele]
  Int_t ele_nucone40[nele_max]; //[nele]
  Float_t ele_etcone20[nele_max]; //[nele]
  Float_t ele_etcone30[nele_max]; //[nele]
  Float_t ele_etcone40[nele_max]; //[nele]
  Float_t ele_etcone20_cor[nele_max]; //[nele]
  Float_t ele_etcone30_cor[nele_max]; //[nele]
  Float_t ele_etcone40_cor[nele_max]; //[nele]
  Float_t ele_ptcone20[nele_max]; //[nele]
  Float_t ele_ptcone30[nele_max]; //[nele]
  Float_t ele_ptcone40[nele_max]; //[nele]
  Float_t ele_d0[nele_max]; //[nele]
  Float_t ele_z0[nele_max]; //[nele]
  Float_t ele_db[nele_max]; //[nele]
  Float_t ele_zb[nele_max]; //[nele]
  Float_t ele_dvx[nele_max]; //[nele]
  Float_t ele_zvx[nele_max]; //[nele]
  Float_t ele_dxpv[nele_max]; //[nele]
  Float_t ele_zxpv[nele_max]; //[nele]
  Float_t ele_covdb[nele_max]; //[nele]
  Float_t ele_covdvx[nele_max]; //[nele]
  Float_t ele_covdxpv[nele_max]; //[nele]
  Int_t ele_vtx[nele_max]; //[nele]
  Int_t ele_nhitpix[nele_max]; //[nele]
  Int_t ele_nhitsct[nele_max]; //[nele]
  Int_t ele_nhittrt[nele_max]; //[nele]
  UShort_t ele_quality[nele_max]; //[nele]
  UInt_t ele_isgoodoq[nele_max]; //[nele]
  UInt_t ele_oq[nele_max]; //[nele]
  Float_t ele_rhad[nele_max]; //[nele]
  Float_t ele_rhad1[nele_max]; //[nele]
  Float_t ele_reta[nele_max]; //[nele]
  Float_t ele_rphi[nele_max]; //[nele]
  Float_t ele_w2[nele_max]; //[nele]
  Float_t ele_wstot[nele_max]; //[nele]
  Float_t ele_demaxs1[nele_max]; //[nele]
  Float_t ele_deltaeta[nele_max]; //[nele]
  Float_t ele_deltaphi[nele_max]; //[nele]
  Float_t ele_trratio[nele_max]; //[nele]
  Short_t ele_nhitbl[nele_max]; //[nele]
  Short_t ele_noutliersbl[nele_max]; //[nele]
  Short_t ele_expectbl[nele_max]; //[nele]
  Short_t ele_noutlierspix[nele_max]; //[nele]
  Short_t ele_noutlierssct[nele_max]; //[nele]
  Float_t ele_eta2[nele_max]; //[nele]
  Float_t ele_et2[nele_max]; //[nele]
#ifndef DISABLE_TRIGGER_MATCHING
  Int_t ele_trig_l1matchvector_;
  Int_t ele_trig_l1matchvector_first[matchvector_max]; //[ele_trig_l1matchvector_]
  Int_t ele_trig_l1matchvector_second[matchvector_max]; //[ele_trig_l1matchvector_]
  std::vector<float> *ele_trig_l1matchdelr;
  Int_t ele_trig_l2matchvector_;
  Int_t ele_trig_l2matchvector_first[matchvector_max]; //[ele_trig_l2matchvector_]
  Int_t ele_trig_l2matchvector_second[matchvector_max]; //[ele_trig_l2matchvector_]
  std::vector<float> *ele_trig_l2matchdelr;
  Int_t ele_trig_efmatchvector_;
  Int_t ele_trig_efmatchvector_first[matchvector_max]; //[ele_trig_efmatchvector_]
  Int_t ele_trig_efmatchvector_second[matchvector_max]; //[ele_trig_efmatchvector_]
  std::vector<float> *ele_trig_efmatchdelr;
#endif
  Float_t ele_MET_wet[nele_max]; //[nele]
  Float_t ele_MET_wpx[nele_max]; //[nele]
  Float_t ele_MET_wpy[nele_max]; //[nele]
  Float_t ele_MET_status[nele_max]; //[nele]
  Int_t ele_ptype[nele_max]; //[nele]
  Int_t ele_porigin[nele_max]; //[nele]
  Int_t ele_poutcome[nele_max]; //[nele]
  Int_t nph;
  UShort_t ph_author[nph_max]; //[nph]
  Float_t ph_eta[nph_max]; //[nph]
  Float_t ph_phi[nph_max]; //[nph]
  Float_t ph_etaclus[nph_max]; //[nph]
  Float_t ph_phiclus[nph_max]; //[nph]
  Float_t ph_et[nph_max]; //[nph]
  Float_t ph_eclus[nph_max]; //[nph]
  UInt_t ph_isEM[nph_max]; //[nph]
  UInt_t ph_isgoodoq[nph_max]; //[nph]
  UInt_t ph_oq[nph_max]; //[nph]
  UShort_t ph_quality[nph_max]; //[nph]
  Float_t ph_etcone20[nph_max]; //[nph]
  Float_t ph_etcone30[nph_max]; //[nph]
  Float_t ph_etcone40[nph_max]; //[nph]
  Float_t ph_etcone20_cor[nph_max]; //[nph]
  Float_t ph_etcone30_cor[nph_max]; //[nph]
  Float_t ph_etcone40_cor[nph_max]; //[nph]
  Float_t ph_ptcone20[nph_max]; //[nph]
  Float_t ph_ptcone30[nph_max]; //[nph]
  Float_t ph_ptcone40[nph_max]; //[nph]
  Int_t ph_isRecovered[nph_max]; //[nph]
  Int_t ph_convFlag[nph_max]; //[nph]
  Float_t ph_rconv[nph_max]; //[nph]
  Float_t ph_zconv[nph_max]; //[nph]
  Float_t ph_etas2[nph_max]; //[nph]
  Float_t ph_ethad1[nph_max]; //[nph]
  Float_t ph_ethad[nph_max]; //[nph]
  Float_t ph_e277[nph_max]; //[nph]
  Float_t ph_e237[nph_max]; //[nph]
  Float_t ph_e233[nph_max]; //[nph]
  Float_t ph_weta2[nph_max]; //[nph]
  Float_t ph_f1[nph_max]; //[nph]
  Float_t ph_emax[nph_max]; //[nph]
  Float_t ph_emaxs2[nph_max]; //[nph]
  Float_t ph_demaxs1[nph_max]; //[nph]
  Float_t ph_fside[nph_max]; //[nph]
  Float_t ph_wstot[nph_max]; //[nph]
  Float_t ph_ws3[nph_max]; //[nph]
  Float_t ph_emins1[nph_max]; //[nph]
  Float_t ph_etap[nph_max]; //[nph]
  Float_t ph_deltaeta[nph_max]; //[nph]
  Float_t ph_MET_wet[nph_max]; //[nph]
  Float_t ph_MET_wpx[nph_max]; //[nph]
  Float_t ph_MET_wpy[nph_max]; //[nph]
  Float_t ph_MET_status[nph_max]; //[nph]
  Int_t ph_ptype[nph_max]; //[nph]
  Int_t ph_porigin[nph_max]; //[nph]
  Int_t ph_poutcome[nph_max]; //[nph]
  Int_t ntrk;
  Float_t trk_pt[ntrk_max]; //[ntrk]
  Float_t trk_eta[ntrk_max]; //[ntrk]
  Float_t trk_phi[ntrk_max]; //[ntrk]
  Float_t trk_m[ntrk_max]; //[ntrk]
  Int_t trk_q[ntrk_max]; //[ntrk]
  Int_t trk_mu[ntrk_max]; //[ntrk]
  Int_t trk_nucone10[ntrk_max]; //[ntrk]
  Int_t trk_nucone20[ntrk_max]; //[ntrk]
  Int_t trk_nucone30[ntrk_max]; //[ntrk]
  Int_t trk_nucone40[ntrk_max]; //[ntrk]
  Float_t trk_ptcone10[ntrk_max]; //[ntrk]
  Float_t trk_ptcone20[ntrk_max]; //[ntrk]
  Float_t trk_ptcone30[ntrk_max]; //[ntrk]
  Float_t trk_ptcone40[ntrk_max]; //[ntrk]
  Int_t trk_vtx[ntrk_max]; //[ntrk]
  Float_t trk_dxpv[ntrk_max]; //[ntrk]
  Float_t trk_zxpv[ntrk_max]; //[ntrk]
  Float_t trk_covdxpv[ntrk_max]; //[ntrk]
  Int_t trk_nhitbl[ntrk_max]; //[ntrk]
  Short_t trk_expectbl[ntrk_max]; //[ntrk]
  Int_t trk_nhitpix[ntrk_max]; //[ntrk]
  Int_t trk_nholespix[ntrk_max]; //[ntrk]
  Int_t trk_ndeadsensorspix[ntrk_max]; //[ntrk]
  Int_t trk_nhitsct[ntrk_max]; //[ntrk]
  Int_t trk_nholessct[ntrk_max]; //[ntrk]
  Int_t trk_ndeadsensorssct[ntrk_max]; //[ntrk]
  Int_t trk_nhittrt[ntrk_max]; //[ntrk]
  Int_t trk_nholestrt[ntrk_max]; //[ntrk]
  Int_t trk_noutlierstrt[ntrk_max]; //[ntrk]
  Int_t trk_ptype[ntrk_max]; //[ntrk]
  Int_t trk_porigin[ntrk_max]; //[ntrk]
  Int_t trk_poutcome[ntrk_max]; //[ntrk]
  Float_t trk_pmatchprob[ntrk_max]; //[ntrk]
  Int_t njet;
  Int_t jet_type[njet_max]; //[njet]
  Float_t jet_et[njet_max]; //[njet]
  Float_t jet_pt[njet_max]; //[njet]
  Float_t jet_eta[njet_max]; //[njet]
  Float_t jet_phi[njet_max]; //[njet]
  Float_t jet_m[njet_max]; //[njet]
  Float_t jet_pt_em[njet_max]; //[njet]
  Float_t jet_eta_em[njet_max]; //[njet]
  Float_t jet_phi_em[njet_max]; //[njet]
  Float_t jet_m_em[njet_max]; //[njet]
  Float_t jet_pt_cs[njet_max];   //[njet]
  Float_t jet_eta_cs[njet_max];   //[njet]
  Float_t jet_phi_cs[njet_max];   //[njet]
  Float_t jet_m_cs[njet_max];   //[njet]
  Float_t jet_eta_origin[njet_max]; //[njet]
  Float_t jet_phi_origin[njet_max]; //[njet]
  Float_t jet_m_origin[njet_max]; //[njet]
  Float_t jet_eta_origin_em[njet_max]; //[njet]
  Float_t jet_phi_origin_em[njet_max]; //[njet]
  Float_t jet_m_origin_em[njet_max]; //[njet]
  Float_t jet_gcwjes[njet_max]; //[njet]
  Float_t jet_emjes[njet_max]; //[njet]
  Int_t jet_isgood[njet_max]; //[njet]
  Int_t jet_isbad[njet_max]; //[njet]
  Int_t jet_isugly[njet_max]; //[njet]
  Int_t jet_n90[njet_max]; //[njet]
  Float_t jet_hecquality[njet_max]; //[njet]
  Float_t jet_larquality[njet_max]; //[njet]
  Float_t jet_averagelarqf[njet_max]; //[njet]
  Float_t jet_negativee[njet_max]; //[njet]
  Float_t jet_sumpttrk[njet_max]; //[njet]
  Int_t jet_ntrk[njet_max]; //[njet]
  Float_t jet_jvtxf[njet_max]; //[njet]
  Float_t jet_jvf[njet_max]; //[njet]
  Float_t jet_bchcorr[njet_max]; //[njet]
  Float_t jet_bchcorrjet[njet_max]; //[njet]
  Float_t jet_tgap3f[njet_max]; //[njet]
  Float_t jet_time[njet_max]; //[njet]
  Float_t jet_emf[njet_max]; //[njet]
  Float_t jet_hecf[njet_max]; //[njet]
  Float_t jet_fmax[njet_max]; //[njet]
  Float_t jet_tagSV0[njet_max]; //[njet]
  Float_t jet_tagSV1[njet_max]; //[njet]
  Float_t jet_tagSV2[njet_max]; //[njet]
  Float_t jet_tagJetProb[njet_max]; //[njet]
  Float_t jet_tagIP1D[njet_max]; //[njet]
  Float_t jet_tagIP2D[njet_max]; //[njet]
  Float_t jet_tagIP3D[njet_max]; //[njet]
  Float_t jet_tagJetFitterTag[njet_max]; //[njet]
  Float_t jet_tagJetFitterCOMB[njet_max]; //[njet]
  Float_t jet_tagJetFitterCOMBNN[njet_max]; //[njet]
  Float_t jet_tagIP3DSV1[njet_max]; //[njet]
  Float_t jet_tagSoftElectronTag[njet_max]; //[njet]
  Float_t jet_tagSoftMuonTag[njet_max]; //[njet]
  Int_t jet_vtx[njet_max]; //[njet]
  Float_t jet_js_tau1[njet_max]; //[njet]
  Float_t jet_js_tau2[njet_max]; //[njet]
  Float_t jet_js_tau3[njet_max]; //[njet]
  Float_t jet_js_width[njet_max]; //[njet]
  Float_t jet_js_qw[njet_max]; //[njet]
  Float_t jet_js_split12[njet_max]; //[njet]
  Float_t jet_js_split23[njet_max]; //[njet]
  Float_t jet_js_split34[njet_max]; //[njet]
  Float_t jet_js_zcut12[njet_max]; //[njet]
  Float_t jet_js_zcut23[njet_max]; //[njet]
  Float_t jet_js_zcut34[njet_max]; //[njet]
  Float_t jet_js_statshape0[njet_max]; //[njet]
  Float_t jet_js_statshape1[njet_max]; //[njet]
  Float_t jet_js_statshape2[njet_max]; //[njet]
  Float_t jet_js_statshape5[njet_max]; //[njet]
  Float_t jet_js_statshape8[njet_max]; //[njet]
  Float_t jet_js_pullmag[njet_max]; //[njet]
  Float_t jet_js_pullphi[njet_max]; //[njet]
  Float_t jet_js_pullc00[njet_max]; //[njet]
  Float_t jet_js_pullc01[njet_max]; //[njet]
  Float_t jet_js_pullc10[njet_max]; //[njet]
  Float_t jet_js_pullc11[njet_max]; //[njet]
  Float_t jet_MET_wet[njet_max]; //[njet]
  Float_t jet_MET_wpx[njet_max]; //[njet]
  Float_t jet_MET_wpy[njet_max]; //[njet]
  Float_t jet_MET_status[njet_max]; //[njet]
  Float_t met_reffinal;
  Float_t met_reffinal_phi;
  Float_t met_reffinal_sumet;
  Float_t met_reffinal_em;
  Float_t met_reffinal_em_phi;
  Float_t met_reffinal_em_sumet;
  Float_t met_refjet;
  Float_t met_refjet_phi;
  Float_t met_refjet_sumet;
  Float_t met_cellout;
  Float_t met_cellout_phi;
  Float_t met_cellout_sumet;
  Float_t met_softjets;
  Float_t met_softjets_phi;
  Float_t met_softjets_sumet;
  Float_t met_cellout_eflow;
  Float_t met_cellout_eflow_phi;
  Float_t met_cellout_eflow_sumet;
  Float_t met_cellout_eflow_muid;
  Float_t met_cellout_eflow_muid_phi;
  Float_t met_cellout_eflow_muid_sumet;
  Float_t met_cryo;
  Float_t met_cryo_phi;
  Float_t met_cryo_sumet;
  Float_t met_refmuon;
  Float_t met_refmuon_phi;
  Float_t met_refmuon_sumet;
  Float_t met_refmuon_track;
  Float_t met_refmuon_track_phi;
  Float_t met_refmuon_track_sumet;
  Float_t met_muonboy;
  Float_t met_muonboy_phi;
  Float_t met_muonboy_sumet;
  Float_t met_muid;
  Float_t met_muid_phi;
  Float_t met_muid_sumet;
  Float_t met_topo;
  Float_t met_topo_phi;
  Float_t met_topo_sumet;
  Float_t met_topo_eta45;
  Float_t met_topo_eta45_phi;
  Float_t met_topo_eta45_sumet;
  Float_t met_lochadtopo;
  Float_t met_lochadtopo_phi;
  Float_t met_lochadtopo_sumet;
  Float_t met_lochadtopo_eta45;
  Float_t met_lochadtopo_eta45_phi;
  Float_t met_lochadtopo_eta45_sumet;
  Float_t met_reffinal_eta45;
  Float_t met_reffinal_eta45_phi;
  Float_t met_reffinal_eta45_sumet;
  Float_t met_muon;
  Float_t met_muon_phi;
  Float_t met_muon_sumet;
  Float_t met_muon_muid;
  Float_t met_muon_muid_phi;
  Float_t met_muon_muid_sumet;
  Float_t met_refele;
  Float_t met_refele_phi;
  Float_t met_refele_sumet;
  Float_t met_reftau;
  Float_t met_reftau_phi;
  Float_t met_reftau_sumet;
  Float_t met_refgamma;
  Float_t met_refgamma_phi;
  Float_t met_refgamma_sumet;
  Float_t met_plhc;
  Float_t met_plhc_phi;
  Float_t met_plhc_sumet;
  Float_t met_ichep;
  Float_t met_ichep_phi;
  Float_t met_ichep_sumet;
  Float_t met_truth;
  Float_t met_truth_phi;
  Float_t met_truth_sumet;
  Float_t met_truth_nonint;
  Float_t met_truth_nonint_phi;
  Float_t met_truth_nonint_sumet;
  Float_t met_truth_muons;
  Float_t met_truth_muons_phi;
  Float_t met_truth_muons_sumet;
  Float_t met_track;
  Float_t met_track_phi;
  Float_t met_track_sumet;
  Int_t nmc;
  Float_t mc_weight;
  Int_t mc_hfor;
  Int_t mc_channel;
  Int_t mc_pdgid[nmc_max]; //[nmc]
  Float_t mc_m[nmc_max]; //[nmc]
  Float_t mc_e[nmc_max]; //[nmc]
  Float_t mc_pt[nmc_max]; //[nmc]
  Float_t mc_phi[nmc_max]; //[nmc]
  Float_t mc_eta[nmc_max]; //[nmc]
  Int_t mc_parent[nmc_max]; //[nmc]
  Int_t mc_status[nmc_max]; //[nmc]
  Int_t mc_ptype[nmc_max]; //[nmc]
  Int_t mc_porigin[nmc_max]; //[nmc]
  Int_t mc_poutcome[nmc_max]; //[nmc]
  Int_t mc_pmotherpdg[nmc_max]; //[nmc]
  Int_t mcevt_hardindex;
  Int_t mcevt_pdfset;
  Float_t mcevt_weight;
  Int_t mcevt_sigid;
  Float_t mcevt_event_scale;
  Float_t mcevt_pdf_scale;
  Float_t mcevt_alphaQCD;
  Float_t mcevt_alphaQED;
  Int_t mcevt_id1;
  Int_t mcevt_id2;
  Float_t mcevt_pdf_x1;
  Float_t mcevt_pdf_x2;
  Float_t mcevt_pdf1;
  Float_t mcevt_pdf2;
  Int_t njpsi;
  Float_t jpsi_mass[njpsi_max]; //[njpsi]
  Float_t jpsi_masserr[njpsi_max]; //[njpsi]
  Float_t jpsi_phi[njpsi_max]; //[njpsi]
  Float_t jpsi_pt[njpsi_max]; //[njpsi]
  Float_t jpsi_y[njpsi_max]; //[njpsi]
  Float_t jpsi_chisq[njpsi_max]; //[njpsi]
  Float_t jpsi_tau[njpsi_max]; //[njpsi]
  Float_t jpsi_tauerr[njpsi_max]; //[njpsi]
  Float_t jpsi_costhetastar[njpsi_max]; //[njpsi]
  Int_t jpsi_is_os[njpsi_max]; //[njpsi]
  Int_t jpsi_mup[njpsi_max]; //[njpsi]
  Int_t jpsi_mum[njpsi_max]; //[njpsi]
  // attention: these do NOT exist in V29 ntuples!
  Float_t lha_cteq6ll;
  Float_t lha_MRSTMCal;
  Float_t lha_CT10;
  Float_t lha_MSTW2008lo68cl;
  Float_t lha_MSTW2008nlo68cl;
  Float_t lha_NNPDF21_100;
  Float_t lha_HERAPDF10_EIG;
  Float_t lha_cteq66;

  // List of branches
  TBranch *b_run; //!
  TBranch *b_event; //!
  TBranch *b_lb; //!
  TBranch *b_bcid; //!
  TBranch *b_lumi_mu_actual; //!
  TBranch *b_lumi_mu_average; //!
  TBranch *b_bdff; //!
  TBranch *b_bfif600; //!
  TBranch *b_bint; //!
  TBranch *b_error_lar; //!
  TBranch *b_error_background; //!
  TBranch *b_trig_ef; //!
  TBranch *b_trig_l2; //!
  TBranch *b_trig_l1; //!
  TBranch *b_bs_x; //!
  TBranch *b_bs_y; //!
  TBranch *b_bs_z; //!
  TBranch *b_bs_status; //!
  TBranch *b_nvx; //!
  TBranch *b_vx_type; //!
  TBranch *b_vx_ntracks; //!
  TBranch *b_vx_sumpt; //!
  TBranch *b_vx_x; //!
  TBranch *b_vx_y; //!
  TBranch *b_vx_z; //!
  TBranch *b_nmu; //!
  TBranch *b_mu_type; //!
  TBranch *b_mu_pt; //!
  TBranch *b_mu_eta; //!
  TBranch *b_mu_phi; //!
  TBranch *b_mu_q; //!
  TBranch *b_mu_ptcid; //!
  TBranch *b_mu_etacid; //!
  TBranch *b_mu_phicid; //!
  TBranch *b_mu_qcid; //!
  TBranch *b_mu_ptid; //!
  TBranch *b_mu_etaid; //!
  TBranch *b_mu_phiid; //!
  TBranch *b_mu_qid; //!
  TBranch *b_mu_ptexid; //!
  TBranch *b_mu_etaexid; //!
  TBranch *b_mu_phiexid; //!
  TBranch *b_mu_qexid; //!
  TBranch *b_mu_ptms; //!
  TBranch *b_mu_etams; //!
  TBranch *b_mu_phims; //!
  TBranch *b_mu_qms; //!
  TBranch *b_mu_ptexms; //!
  TBranch *b_mu_etaexms; //!
  TBranch *b_mu_phiexms; //!
  TBranch *b_mu_qexms; //!
  TBranch *b_mu_author; //!
  TBranch *b_mu_primauthor; //!
  TBranch *b_mu_class; //!
  TBranch *b_mu_iscombined; //!
  TBranch *b_mu_issegtagged; //!
  TBranch *b_mu_fitchi2prob; //!
  TBranch *b_mu_matchchi2prob; //!
  TBranch *b_mu_fitchi2; //!
  TBranch *b_mu_matchchi2; //!
  TBranch *b_mu_bestmatch; //!
  TBranch *b_mu_nucone10; //!
  TBranch *b_mu_nucone20; //!
  TBranch *b_mu_nucone30; //!
  TBranch *b_mu_nucone40; //!
  TBranch *b_mu_etcone10; //!
  TBranch *b_mu_etcone20; //!
  TBranch *b_mu_etcone30; //!
  TBranch *b_mu_etcone40; //!
  TBranch *b_mu_ptcone10; //!
  TBranch *b_mu_ptcone20; //!
  TBranch *b_mu_ptcone30; //!
  TBranch *b_mu_ptcone40; //!
  TBranch *b_mu_ptconemod10; //!
  TBranch *b_mu_ptconemod20; //!
  TBranch *b_mu_ptconemod30; //!
  TBranch *b_mu_ptconemod40; //!
  TBranch *b_mu_eloss; //!
  TBranch *b_mu_d0; //!
  TBranch *b_mu_z0; //!
  TBranch *b_mu_db; //!
  TBranch *b_mu_zb; //!
  TBranch *b_mu_dvx; //!
  TBranch *b_mu_zvx; //!
  TBranch *b_mu_dxpv; //!
  TBranch *b_mu_zxpv; //!
  TBranch *b_mu_covdb; //!
  TBranch *b_mu_covdvx; //!
  TBranch *b_mu_covdxpv; //!
  TBranch *b_mu_phiid_expv;   //!
  TBranch *b_mu_phiexms_expv;   //!
  TBranch *b_mu_thetaid_expv;   //!
  TBranch *b_mu_thetaexms_expv;   //!
  TBranch *b_mu_qoverpid_expv;   //!
  TBranch *b_mu_qoverpexms_expv;   //!
  TBranch *b_mu_covqoverp_expv;   //!
  TBranch *b_mu_covqoverpid_expv;   //!
  TBranch *b_mu_covqoverpexms_expv;   //!
  TBranch *b_mu_vtxtype; //!
  TBranch *b_mu_vtx; //!
  TBranch *b_mu_nhitbl; //!
  TBranch *b_mu_expectbl; //!
  TBranch *b_mu_nhitpix; //!
  TBranch *b_mu_nholespix; //!
  TBranch *b_mu_ndeadsensorspix; //!
  TBranch *b_mu_nhitsct; //!
  TBranch *b_mu_nholessct; //!
  TBranch *b_mu_ndeadsensorssct; //!
  TBranch *b_mu_nhittrt; //!
  TBranch *b_mu_nholestrt; //!
  TBranch *b_mu_nhitmdt; //!
  TBranch *b_mu_nhitcsceta; //!
  TBranch *b_mu_nhitcscphi; //!
  TBranch *b_mu_nhitrpceta; //!
  TBranch *b_mu_nhitrpcphi; //!
  TBranch *b_mu_nhittgceta; //!
  TBranch *b_mu_nhittgcphi; //!
  TBranch *b_mu_noutlierstrt; //!
  TBranch *b_mu_nhitcscunspoilt; //!
  TBranch *b_mu_nMDTBIHits; //!
  TBranch *b_mu_nMDTBMHits; //!
  TBranch *b_mu_nMDTBOHits; //!
  TBranch *b_mu_nMDTBEEHits; //!
  TBranch *b_mu_nMDTBIS78Hits; //!
  TBranch *b_mu_nMDTEIHits; //!
  TBranch *b_mu_nMDTEMHits; //!
  TBranch *b_mu_nMDTEOHits; //!
  TBranch *b_mu_nMDTEEHits; //!
  TBranch *b_mu_quality; //!
  TBranch *b_mu_scatt_curv_sig; //!
  TBranch *b_mu_scatt_neighb_sig; //!
  TBranch *b_mu_field_integral; //!
#ifndef DISABLE_TRIGGER_MATCHING
  TBranch *b_mu_trig_l1matchvector_; //!
  TBranch *b_mu_trig_l1matchvector_first; //!
  TBranch *b_mu_trig_l1matchvector_second; //!
  TBranch *b_mu_trig_l1matchdelr; //!
  TBranch *b_mu_trig_l2matchvector_; //!
  TBranch *b_mu_trig_l2matchvector_first; //!
  TBranch *b_mu_trig_l2matchvector_second; //!
  TBranch *b_mu_trig_l2matchdelr; //!
  TBranch *b_mu_trig_l2samatchvector_; //!
  TBranch *b_mu_trig_l2samatchvector_first; //!
  TBranch *b_mu_trig_l2samatchvector_second; //!
  TBranch *b_mu_trig_l2samatchdelr; //!
  TBranch *b_mu_trig_efmatchvector_; //!
  TBranch *b_mu_trig_efmatchvector_first; //!
  TBranch *b_mu_trig_efmatchvector_second; //!
  TBranch *b_mu_trig_efmatchdelr; //!
#endif
  TBranch *b_mu_passes_mcp; //!
  TBranch *b_mu_MET_wet; //!
  TBranch *b_mu_MET_wpx; //!
  TBranch *b_mu_MET_wpy; //!
  TBranch *b_mu_MET_status; //!
  TBranch *b_mu_MET_wet2; //!
  TBranch *b_mu_MET_wpx2; //!
  TBranch *b_mu_MET_wpy2; //!
  TBranch *b_mu_MET_status2; //!
  TBranch *b_mu_ptype; //!
  TBranch *b_mu_porigin; //!
  TBranch *b_mu_poutcome; //!
  TBranch *b_nele; //!
  TBranch *b_ele_author; //!
  TBranch *b_ele_eta; //!
  TBranch *b_ele_phi; //!
  TBranch *b_ele_et; //!
  TBranch *b_ele_q; //!
  TBranch *b_ele_etaclus; //!
  TBranch *b_ele_phiclus; //!
  TBranch *b_ele_etclus; //!
  TBranch *b_ele_etatrk; //!
  TBranch *b_ele_phitrk; //!
  TBranch *b_ele_pttrk; //!
  TBranch *b_ele_etabest; //!
  TBranch *b_ele_phibest; //!
  TBranch *b_ele_etbest; //!
  TBranch *b_ele_etap; //!
  TBranch *b_ele_f1; //!
  TBranch *b_ele_isEM; //!
  TBranch *b_ele_nucone20; //!
  TBranch *b_ele_nucone30; //!
  TBranch *b_ele_nucone40; //!
  TBranch *b_ele_etcone20; //!
  TBranch *b_ele_etcone30; //!
  TBranch *b_ele_etcone40; //!
  TBranch *b_ele_etcone20_cor; //!
  TBranch *b_ele_etcone30_cor; //!
  TBranch *b_ele_etcone40_cor; //!
  TBranch *b_ele_ptcone20; //!
  TBranch *b_ele_ptcone30; //!
  TBranch *b_ele_ptcone40; //!
  TBranch *b_ele_d0; //!
  TBranch *b_ele_z0; //!
  TBranch *b_ele_db; //!
  TBranch *b_ele_zb; //!
  TBranch *b_ele_dvx; //!
  TBranch *b_ele_zvx; //!
  TBranch *b_ele_dxpv; //!
  TBranch *b_ele_zxpv; //!
  TBranch *b_ele_covdb; //!
  TBranch *b_ele_covdvx; //!
  TBranch *b_ele_covdxpv; //!
  TBranch *b_ele_vtx; //!
  TBranch *b_ele_nhitpix; //!
  TBranch *b_ele_nhitsct; //!
  TBranch *b_ele_nhittrt; //!
  TBranch *b_ele_quality; //!
  TBranch *b_ele_isgoodoq; //!
  TBranch *b_ele_oq; //!
  TBranch *b_ele_rhad; //!
  TBranch *b_ele_rhad1; //!
  TBranch *b_ele_reta; //!
  TBranch *b_ele_rphi; //!
  TBranch *b_ele_w2; //!
  TBranch *b_ele_wstot; //!
  TBranch *b_ele_demaxs1; //!
  TBranch *b_ele_deltaeta; //!
  TBranch *b_ele_deltaphi; //!
  TBranch *b_ele_trratio; //!
  TBranch *b_ele_nhitbl; //!
  TBranch *b_ele_noutliersbl; //!
  TBranch *b_ele_expectbl; //!
  TBranch *b_ele_noutlierspix; //!
  TBranch *b_ele_noutlierssct; //!
  TBranch *b_ele_eta2; //!
  TBranch *b_ele_et2; //!
#ifndef DISABLE_TRIGGER_MATCHING
  TBranch *b_ele_trig_l1matchvector_; //!
  TBranch *b_ele_trig_l1matchvector_first; //!
  TBranch *b_ele_trig_l1matchvector_second; //!
  TBranch *b_ele_trig_l1matchdelr; //!
  TBranch *b_ele_trig_l2matchvector_; //!
  TBranch *b_ele_trig_l2matchvector_first; //!
  TBranch *b_ele_trig_l2matchvector_second; //!
  TBranch *b_ele_trig_l2matchdelr; //!
  TBranch *b_ele_trig_efmatchvector_; //!
  TBranch *b_ele_trig_efmatchvector_first; //!
  TBranch *b_ele_trig_efmatchvector_second; //!
  TBranch *b_ele_trig_efmatchdelr; //!
#endif
  TBranch *b_ele_MET_wet; //!
  TBranch *b_ele_MET_wpx; //!
  TBranch *b_ele_MET_wpy; //!
  TBranch *b_ele_MET_status; //!
  TBranch *b_ele_ptype; //!
  TBranch *b_ele_porigin; //!
  TBranch *b_ele_poutcome; //!
  TBranch *b_nph; //!
  TBranch *b_ph_author; //!
  TBranch *b_ph_eta; //!
  TBranch *b_ph_phi; //!
  TBranch *b_ph_etaclus; //!
  TBranch *b_ph_phiclus; //!
  TBranch *b_ph_et; //!
  TBranch *b_ph_eclus; //!
  TBranch *b_ph_isEM; //!
  TBranch *b_ph_isgoodoq; //!
  TBranch *b_ph_oq; //!
  TBranch *b_ph_quality; //!
  TBranch *b_ph_etcone20; //!
  TBranch *b_ph_etcone30; //!
  TBranch *b_ph_etcone40; //!
  TBranch *b_ph_etcone20_cor; //!
  TBranch *b_ph_etcone30_cor; //!
  TBranch *b_ph_etcone40_cor; //!
  TBranch *b_ph_ptcone20; //!
  TBranch *b_ph_ptcone30; //!
  TBranch *b_ph_ptcone40; //!
  TBranch *b_ph_isRecovered; //!
  TBranch *b_ph_convFlag; //!
  TBranch *b_ph_rconv; //!
  TBranch *b_ph_zconv; //!
  TBranch *b_ph_etas2; //!
  TBranch *b_ph_ethad1; //!
  TBranch *b_ph_ethad; //!
  TBranch *b_ph_e277; //!
  TBranch *b_ph_e237; //!
  TBranch *b_ph_e233; //!
  TBranch *b_ph_weta2; //!
  TBranch *b_ph_f1; //!
  TBranch *b_ph_emax; //!
  TBranch *b_ph_emaxs2; //!
  TBranch *b_ph_demaxs1; //!
  TBranch *b_ph_fside; //!
  TBranch *b_ph_wstot; //!
  TBranch *b_ph_ws3; //!
  TBranch *b_ph_emins1; //!
  TBranch *b_ph_etap; //!
  TBranch *b_ph_deltaeta; //!
  TBranch *b_ph_MET_wet; //!
  TBranch *b_ph_MET_wpx; //!
  TBranch *b_ph_MET_wpy; //!
  TBranch *b_ph_MET_status; //!
  TBranch *b_ph_ptype; //!
  TBranch *b_ph_porigin; //!
  TBranch *b_ph_poutcome; //!
  TBranch *b_ntrk; //!
  TBranch *b_trk_pt; //!
  TBranch *b_trk_eta; //!
  TBranch *b_trk_phi; //!
  TBranch *b_trk_m; //!
  TBranch *b_trk_q; //!
  TBranch *b_trk_mu; //!
  TBranch *b_trk_nucone10; //!
  TBranch *b_trk_nucone20; //!
  TBranch *b_trk_nucone30; //!
  TBranch *b_trk_nucone40; //!
  TBranch *b_trk_ptcone10; //!
  TBranch *b_trk_ptcone20; //!
  TBranch *b_trk_ptcone30; //!
  TBranch *b_trk_ptcone40; //!
  TBranch *b_trk_vtx; //!
  TBranch *b_trk_dxpv; //!
  TBranch *b_trk_zxpv; //!
  TBranch *b_trk_covdxpv; //!
  TBranch *b_trk_nhitbl; //!
  TBranch *b_trk_expectbl; //!
  TBranch *b_trk_nhitpix; //!
  TBranch *b_trk_nholespix; //!
  TBranch *b_trk_ndeadsensorspix; //!
  TBranch *b_trk_nhitsct; //!
  TBranch *b_trk_nholessct; //!
  TBranch *b_trk_ndeadsensorssct; //!
  TBranch *b_trk_nhittrt; //!
  TBranch *b_trk_nholestrt; //!
  TBranch *b_trk_noutlierstrt; //!
  TBranch *b_trk_ptype; //!
  TBranch *b_trk_porigin; //!
  TBranch *b_trk_poutcome; //!
  TBranch *b_trk_pmatchprob; //!
  TBranch *b_njet; //!
  TBranch *b_jet_type; //!
  TBranch *b_jet_et; //!
  TBranch *b_jet_pt; //!
  TBranch *b_jet_eta; //!
  TBranch *b_jet_phi; //!
  TBranch *b_jet_m; //!
  TBranch *b_jet_pt_em; //!
  TBranch *b_jet_eta_em; //!
  TBranch *b_jet_phi_em; //!
  TBranch *b_jet_m_em; //!
  TBranch *b_jet_pt_cs;   //!
  TBranch *b_jet_eta_cs;   //!
  TBranch *b_jet_phi_cs;   //!
  TBranch *b_jet_m_cs;   //!
  TBranch *b_jet_eta_origin; //!
  TBranch *b_jet_phi_origin; //!
  TBranch *b_jet_m_origin; //!
  TBranch *b_jet_eta_origin_em; //!
  TBranch *b_jet_phi_origin_em; //!
  TBranch *b_jet_m_origin_em; //!
  TBranch *b_jet_gcwjes; //!
  TBranch *b_jet_emjes; //!
  TBranch *b_jet_isgood; //!
  TBranch *b_jet_isbad; //!
  TBranch *b_jet_isugly; //!
  TBranch *b_jet_n90; //!
  TBranch *b_jet_hecquality; //!
  TBranch *b_jet_larquality; //!
  TBranch *b_jet_averagelarqf; //!
  TBranch *b_jet_negativee; //!
  TBranch *b_jet_sumpttrk; //!
  TBranch *b_jet_ntrk; //!
  TBranch *b_jet_jvtxf; //!
  TBranch *b_jet_jvf; //!
  TBranch *b_jet_bchcorr; //!
  TBranch *b_jet_bchcorrjet; //!
  TBranch *b_jet_tgap3f; //!
  TBranch *b_jet_time; //!
  TBranch *b_jet_emf; //!
  TBranch *b_jet_hecf; //!
  TBranch *b_jet_fmax; //!
  TBranch *b_jet_tagSV0; //!
  TBranch *b_jet_tagSV1; //!
  TBranch *b_jet_tagSV2; //!
  TBranch *b_jet_tagJetProb; //!
  TBranch *b_jet_tagIP1D; //!
  TBranch *b_jet_tagIP2D; //!
  TBranch *b_jet_tagIP3D; //!
  TBranch *b_jet_tagJetFitterTag; //!
  TBranch *b_jet_tagJetFitterCOMB; //!
  TBranch *b_jet_tagJetFitterCOMBNN; //!
  TBranch *b_jet_tagIP3DSV1; //!
  TBranch *b_jet_tagSoftElectronTag; //!
  TBranch *b_jet_tagSoftMuonTag; //!
  TBranch *b_jet_vtx; //!
  TBranch *b_jet_js_tau1; //!
  TBranch *b_jet_js_tau2; //!
  TBranch *b_jet_js_tau3; //!
  TBranch *b_jet_js_width; //!
  TBranch *b_jet_js_qw; //!
  TBranch *b_jet_js_split12; //!
  TBranch *b_jet_js_split23; //!
  TBranch *b_jet_js_split34; //!
  TBranch *b_jet_js_zcut12; //!
  TBranch *b_jet_js_zcut23; //!
  TBranch *b_jet_js_zcut34; //!
  TBranch *b_jet_js_statshape0; //!
  TBranch *b_jet_js_statshape1; //!
  TBranch *b_jet_js_statshape2; //!
  TBranch *b_jet_js_statshape5; //!
  TBranch *b_jet_js_statshape8; //!
  TBranch *b_jet_js_pullmag; //!
  TBranch *b_jet_js_pullphi; //!
  TBranch *b_jet_js_pullc00; //!
  TBranch *b_jet_js_pullc01; //!
  TBranch *b_jet_js_pullc10; //!
  TBranch *b_jet_js_pullc11; //!
  TBranch *b_jet_MET_wet; //!
  TBranch *b_jet_MET_wpx; //!
  TBranch *b_jet_MET_wpy; //!
  TBranch *b_jet_MET_status; //!
  TBranch *b_met_reffinal; //!
  TBranch *b_met_reffinal_phi; //!
  TBranch *b_met_reffinal_sumet; //!
  TBranch *b_met_reffinal_em; //!
  TBranch *b_met_reffinal_em_phi; //!
  TBranch *b_met_reffinal_em_sumet; //!
  TBranch *b_met_refjet; //!
  TBranch *b_met_refjet_phi; //!
  TBranch *b_met_refjet_sumet; //!
  TBranch *b_met_cellout; //!
  TBranch *b_met_cellout_phi; //!
  TBranch *b_met_cellout_sumet; //!
  TBranch *b_met_softjets; //!
  TBranch *b_met_softjets_phi; //!
  TBranch *b_met_softjets_sumet; //!
  TBranch *b_met_cellout_eflow; //!
  TBranch *b_met_cellout_eflow_phi; //!
  TBranch *b_met_cellout_eflow_sumet; //!
  TBranch *b_met_cellout_eflow_muid; //!
  TBranch *b_met_cellout_eflow_muid_phi; //!
  TBranch *b_met_cellout_eflow_muid_sumet; //!
  TBranch *b_met_cryo; //!
  TBranch *b_met_cryo_phi; //!
  TBranch *b_met_cryo_sumet; //!
  TBranch *b_met_refmuon; //!
  TBranch *b_met_refmuon_phi; //!
  TBranch *b_met_refmuon_sumet; //!
  TBranch *b_met_refmuon_track; //!
  TBranch *b_met_refmuon_track_phi; //!
  TBranch *b_met_refmuon_track_sumet; //!
  TBranch *b_met_muonboy; //!
  TBranch *b_met_muonboy_phi; //!
  TBranch *b_met_muonboy_sumet; //!
  TBranch *b_met_muid; //!
  TBranch *b_met_muid_phi; //!
  TBranch *b_met_muid_sumet; //!
  TBranch *b_met_topo; //!
  TBranch *b_met_topo_phi; //!
  TBranch *b_met_topo_sumet; //!
  TBranch *b_met_topo_eta45; //!
  TBranch *b_met_topo_eta45_phi; //!
  TBranch *b_met_topo_eta45_sumet; //!
  TBranch *b_met_lochadtopo; //!
  TBranch *b_met_lochadtopo_phi; //!
  TBranch *b_met_lochadtopo_sumet; //!
  TBranch *b_met_lochadtopo_eta45; //!
  TBranch *b_met_lochadtopo_eta45_phi; //!
  TBranch *b_met_lochadtopo_eta45_sumet; //!
  TBranch *b_met_reffinal_eta45; //!
  TBranch *b_met_reffinal_eta45_phi; //!
  TBranch *b_met_reffinal_eta45_sumet; //!
  TBranch *b_met_muon; //!
  TBranch *b_met_muon_phi; //!
  TBranch *b_met_muon_sumet; //!
  TBranch *b_met_muon_muid; //!
  TBranch *b_met_muon_muid_phi; //!
  TBranch *b_met_muon_muid_sumet; //!
  TBranch *b_met_refele; //!
  TBranch *b_met_refele_phi; //!
  TBranch *b_met_refele_sumet; //!
  TBranch *b_met_reftau; //!
  TBranch *b_met_reftau_phi; //!
  TBranch *b_met_reftau_sumet; //!
  TBranch *b_met_refgamma; //!
  TBranch *b_met_refgamma_phi; //!
  TBranch *b_met_refgamma_sumet; //!
  TBranch *b_met_plhc; //!
  TBranch *b_met_plhc_phi; //!
  TBranch *b_met_plhc_sumet; //!
  TBranch *b_met_ichep; //!
  TBranch *b_met_ichep_phi; //!
  TBranch *b_met_ichep_sumet; //!
  TBranch *b_met_truth; //!
  TBranch *b_met_truth_phi; //!
  TBranch *b_met_truth_sumet; //!
  TBranch *b_met_truth_nonint;   //!
  TBranch *b_met_truth_nonint_phi;   //!
  TBranch *b_met_truth_nonint_sumet;   //!
  TBranch *b_met_truth_muons;   //!
  TBranch *b_met_truth_muons_phi;   //!
  TBranch *b_met_truth_muons_sumet;   //!
  TBranch *b_met_track; //!
  TBranch *b_met_track_phi; //!
  TBranch *b_met_track_sumet; //!
  TBranch *b_nmc; //!
  TBranch *b_mc_weight; //!
  TBranch *b_mc_hfor; //!
  TBranch *b_mc_channel; //!
  TBranch *b_mc_pdgid; //!
  TBranch *b_mc_m; //!
  TBranch *b_mc_e; //!
  TBranch *b_mc_pt; //!
  TBranch *b_mc_phi; //!
  TBranch *b_mc_eta; //!
  TBranch *b_mc_parent; //!
  TBranch *b_mc_status; //!
  TBranch *b_mc_ptype; //!
  TBranch *b_mc_porigin; //!
  TBranch *b_mc_poutcome; //!
  TBranch *b_mc_pmotherpdg; //!
  TBranch *b_mcevt_hardindex; //!
  TBranch *b_mcevt_pdfset; //!
  TBranch *b_mcevt_weight; //!
  TBranch *b_mcevt_sigid; //!
  TBranch *b_mcevt_event_scale; //!
  TBranch *b_mcevt_pdf_scale; //!
  TBranch *b_mcevt_alphaQCD; //!
  TBranch *b_mcevt_alphaQED; //!
  TBranch *b_mcevt_id1; //!
  TBranch *b_mcevt_id2; //!
  TBranch *b_mcevt_pdf_x1; //!
  TBranch *b_mcevt_pdf_x2; //!
  TBranch *b_mcevt_pdf1; //!
  TBranch *b_mcevt_pdf2; //!
  TBranch *b_njpsi; //!
  TBranch *b_jpsi_mass; //!
  TBranch *b_jpsi_masserr; //!
  TBranch *b_jpsi_phi; //!
  TBranch *b_jpsi_pt; //!
  TBranch *b_jpsi_y; //!
  TBranch *b_jpsi_chisq; //!
  TBranch *b_jpsi_tau; //!
  TBranch *b_jpsi_tauerr; //!
  TBranch *b_jpsi_costhetastar; //!
  TBranch *b_jpsi_is_os; //!
  TBranch *b_jpsi_mup; //!
  TBranch *b_jpsi_mum; //!
  TBranch *b_lha_cteq6ll; //!
  TBranch *b_lha_MRSTMCal; //!
  TBranch *b_lha_CT10; //!
  TBranch *b_lha_MSTW2008lo68cl; //!
  TBranch *b_lha_MSTW2008nlo68cl; //!
  TBranch *b_lha_NNPDF21_100; //!
  TBranch *b_lha_HERAPDF10_EIG; //!
  TBranch *b_lha_cteq66; //!

  D3PDPeterTreeV29g(TTree *tree=0);
  virtual ~D3PDPeterTreeV29g();
  virtual Int_t GetEntry(Long64_t entry);
  virtual Long64_t LoadTree(Long64_t entry);
  virtual void Init(TTree *tree);
};

#endif
