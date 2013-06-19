//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue Feb 21 09:18:12 2012 by ROOT version 5.28/00e
// from TTree tree/Muon tree
// found on file: user.kapliy.008576.flatntuple._00068.root
//////////////////////////////////////////////////////////

#ifndef D3PDPeterTreeV29l_h
#define D3PDPeterTreeV29l_h

#include "TrigFTKAna_config.h"
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <vector>
#include <string>
#include "TrigFTKAna/Trees/D3PDPeterTreeBase.h"

class
D3PDPeterTreeV29l : public D3PDPeterTreeBase
{
 public:
  enum {
    nele_max = 10000,
    njet_max = 10000,
    nmc_max = 100000,
    nmu_max = 10000,
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
  Float_t mu_pt[nmu_max]; //[nmu]
  Float_t mu_eta[nmu_max]; //[nmu]
  Float_t mu_phi[nmu_max]; //[nmu]
  Int_t mu_q[nmu_max]; //[nmu]
  Float_t mu_ptid[nmu_max]; //[nmu]
  Float_t mu_etaid[nmu_max]; //[nmu]
  Float_t mu_phiid[nmu_max]; //[nmu]
  Int_t mu_qid[nmu_max]; //[nmu]
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
  Int_t mu_bestmatch[nmu_max]; //[nmu]
  Float_t mu_etcone10[nmu_max]; //[nmu]
  Float_t mu_etcone20[nmu_max]; //[nmu]
  Float_t mu_etcone30[nmu_max]; //[nmu]
  Float_t mu_etcone40[nmu_max]; //[nmu]
  Float_t mu_ptcone10[nmu_max]; //[nmu]
  Float_t mu_ptcone20[nmu_max]; //[nmu]
  Float_t mu_ptcone30[nmu_max]; //[nmu]
  Float_t mu_ptcone40[nmu_max]; //[nmu]
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
  UShort_t mu_quality[nmu_max]; //[nmu]
#ifndef DISABLE_TRIGGER_MATCHING
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
  Int_t nele;
  Float_t ele_eta[nele_max]; //[nele]
  Float_t ele_phi[nele_max]; //[nele]
  Float_t ele_et[nele_max]; //[nele]
  Float_t ele_MET_wet[nele_max]; //[nele]
  Float_t ele_MET_wpx[nele_max]; //[nele]
  Float_t ele_MET_wpy[nele_max]; //[nele]
  Float_t ele_MET_status[nele_max]; //[nele]
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
  Int_t jet_vtx[njet_max]; //[njet]
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
  Float_t mc_e[nmc_max]; //[nmc]
  Float_t mc_pt[nmc_max]; //[nmc]
  Float_t mc_phi[nmc_max]; //[nmc]
  Float_t mc_eta[nmc_max]; //[nmc]
  Int_t mc_parent[nmc_max]; //[nmc]
  Int_t mc_status[nmc_max]; //[nmc]
  Float_t mc_vx_z[nmc_max]; //[nmc]
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
  TBranch *b_mu_pt; //!
  TBranch *b_mu_eta; //!
  TBranch *b_mu_phi; //!
  TBranch *b_mu_q; //!
  TBranch *b_mu_ptid; //!
  TBranch *b_mu_etaid; //!
  TBranch *b_mu_phiid; //!
  TBranch *b_mu_qid; //!
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
  TBranch *b_mu_bestmatch; //!
  TBranch *b_mu_etcone10; //!
  TBranch *b_mu_etcone20; //!
  TBranch *b_mu_etcone30; //!
  TBranch *b_mu_etcone40; //!
  TBranch *b_mu_ptcone10; //!
  TBranch *b_mu_ptcone20; //!
  TBranch *b_mu_ptcone30; //!
  TBranch *b_mu_ptcone40; //!
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
  TBranch *b_mu_quality; //!
#ifndef DISABLE_TRIGGER_MATCHING
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
  TBranch *b_nele; //!
  TBranch *b_ele_eta; //!
  TBranch *b_ele_phi; //!
  TBranch *b_ele_et; //!
  TBranch *b_ele_MET_wet; //!
  TBranch *b_ele_MET_wpx; //!
  TBranch *b_ele_MET_wpy; //!
  TBranch *b_ele_MET_status; //!
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
  TBranch *b_jet_vtx; //!
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
  TBranch *b_mc_e; //!
  TBranch *b_mc_pt; //!
  TBranch *b_mc_phi; //!
  TBranch *b_mc_eta; //!
  TBranch *b_mc_parent; //!
  TBranch *b_mc_status; //!
  TBranch *b_mc_vx_z; //!
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

  D3PDPeterTreeV29l(TTree *tree=0);
  virtual ~D3PDPeterTreeV29l();
  virtual Int_t GetEntry(Long64_t entry);
  virtual Long64_t LoadTree(Long64_t entry);
  virtual void Init(TTree *tree);
};

#endif
