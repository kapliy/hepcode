#include <TROOT.h>
#include <TChain.h>
#include <TBranch.h>
#include <TFile.h>
#include <vector>
#include "TrigFTKAna/Trees/D3PDPeterTreeV29l.h"


D3PDPeterTreeV29l::D3PDPeterTreeV29l(TTree *tree)
{
  Init(tree);
}

D3PDPeterTreeV29l::~D3PDPeterTreeV29l()
{
  if (!fChain) return;
  // delete fChain->GetCurrentFile();
}

Int_t D3PDPeterTreeV29l::GetEntry(Long64_t entry)
{
  // Read contents of entry.
  if (!fChain) return 0;
  fChain->LoadTree(entry); // EXPERIMENTAL: needed to enable TTree cache
  return fChain->GetEntry(entry);
}
Long64_t D3PDPeterTreeV29l::LoadTree(Long64_t entry)
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

void D3PDPeterTreeV29l::Init(TTree *tree)
{
  // The Init() function is called when the selector needs to initialize
  // a new tree or chain. Typically here the branch addresses and branch
  // pointers of the tree will be set.
  // It is normally not necessary to make changes to the generated
  // code, but the routine can be extended by the user if needed.
  // Init() will be called many times when running on PROOF
  // (once per file to be processed).

  // ATTENTION:
  // if level==1, tracks and mcevtinfo branches are disabled
  const int level = 1;

  fChain = 0;

  // Set object pointer
  trig_ef = 0;
  mu_trig_efmatchdelr = 0;
  // Set branch addresses and branch pointers
  if (!tree) return;
  fChain = tree;
  fCurrent = -1;
  fChain->SetMakeClass(1);

  fChain->SetBranchStatus("*",0);

  // event metadata
  fChain->SetBranchStatus("run*",1);
  fChain->SetBranchStatus("event*",1);
  fChain->SetBranchStatus("lb*",1);
  fChain->SetBranchStatus("bcid*",1);
  fChain->SetBranchStatus("error_*",1);
  // beam spot and luminosity
  fChain->SetBranchStatus("lumi_mu*", 1);
  fChain->SetBranchStatus("bdff*", 1);
  fChain->SetBranchStatus("bfif600*", 1);
  fChain->SetBranchStatus("bint*", 1);
  fChain->SetBranchStatus("bs_*",1);
  // trigger
  fChain->SetBranchStatus("trig_ef*",1);
  // vertex information
  fChain->SetBranchStatus("nvx*",1);
  fChain->SetBranchStatus("vx_*",1);
  // muons
  fChain->SetBranchStatus("nmu*",1);
  fChain->SetBranchStatus("mu_*",1);
  // special trickery for disabled trigger matching branches
  if(false) {
    fChain->SetBranchStatus("mu_trig_l1match*",0);
    fChain->SetBranchStatus("mu_trig_l2match*",0);
    fChain->SetBranchStatus("mu_trig_l2samatch*",0);
  }
  // electrons
  fChain->SetBranchStatus("nele",1);
  fChain->SetBranchStatus("ele_eta",1);
  fChain->SetBranchStatus("ele_phi",1);
  fChain->SetBranchStatus("ele_et",1);
  fChain->SetBranchStatus("ele_MET_wet",1);
  fChain->SetBranchStatus("ele_MET_wpx",1);
  fChain->SetBranchStatus("ele_MET_wpy",1);
  fChain->SetBranchStatus("ele_MET_status",1);
  //jets
  fChain->SetBranchStatus("njet*",1);
  fChain->SetBranchStatus("jet_*",1);
  // MET
  fChain->SetBranchStatus("met_*",1);
  // MC truth
  fChain->SetBranchStatus("nmc*",1);
  fChain->SetBranchStatus("mc_*",1);
  // MC event info
  fChain->SetBranchStatus("mcevt_*",1);

  if( fChain->GetBranchStatus("run") ) { fChain->SetBranchAddress("run", &run, &b_run); }
  if( fChain->GetBranchStatus("event") ) { fChain->SetBranchAddress("event", &event, &b_event); }
  if( fChain->GetBranchStatus("lb") ) { fChain->SetBranchAddress("lb", &lb, &b_lb); }
  if( fChain->GetBranchStatus("bcid") ) { fChain->SetBranchAddress("bcid", &bcid, &b_bcid); }
  if( fChain->GetBranchStatus("lumi_mu_actual") ) { fChain->SetBranchAddress("lumi_mu_actual", &lumi_mu_actual, &b_lumi_mu_actual); }
  if( fChain->GetBranchStatus("lumi_mu_average") ) { fChain->SetBranchAddress("lumi_mu_average", &lumi_mu_average, &b_lumi_mu_average); }
  if( fChain->GetBranchStatus("bdff") ) { fChain->SetBranchAddress("bdff", &bdff, &b_bdff); }
  if( fChain->GetBranchStatus("bfif600") ) { fChain->SetBranchAddress("bfif600", &bfif600, &b_bfif600); }
  if( fChain->GetBranchStatus("bint") ) { fChain->SetBranchAddress("bint", &bint, &b_bint); }
  if( fChain->GetBranchStatus("error_lar") ) { fChain->SetBranchAddress("error_lar", &error_lar, &b_error_lar); }
  if( fChain->GetBranchStatus("error_background") ) { fChain->SetBranchAddress("error_background", &error_background, &b_error_background); }
  if( fChain->GetBranchStatus("trig_ef") ) { fChain->SetBranchAddress("trig_ef", &trig_ef, &b_trig_ef); }
  if( fChain->GetBranchStatus("bs_x") ) { fChain->SetBranchAddress("bs_x", &bs_x, &b_bs_x); }
  if( fChain->GetBranchStatus("bs_y") ) { fChain->SetBranchAddress("bs_y", &bs_y, &b_bs_y); }
  if( fChain->GetBranchStatus("bs_z") ) { fChain->SetBranchAddress("bs_z", &bs_z, &b_bs_z); }
  if( fChain->GetBranchStatus("bs_status") ) { fChain->SetBranchAddress("bs_status", &bs_status, &b_bs_status); }
  if( fChain->GetBranchStatus("nvx") ) { fChain->SetBranchAddress("nvx", &nvx, &b_nvx); }
  if( fChain->GetBranchStatus("vx_type") ) { fChain->SetBranchAddress("vx_type", vx_type, &b_vx_type); }
  if( fChain->GetBranchStatus("vx_ntracks") ) { fChain->SetBranchAddress("vx_ntracks", vx_ntracks, &b_vx_ntracks); }
  if( fChain->GetBranchStatus("vx_sumpt") ) { fChain->SetBranchAddress("vx_sumpt", vx_sumpt, &b_vx_sumpt); }
  if( fChain->GetBranchStatus("vx_x") ) { fChain->SetBranchAddress("vx_x", vx_x, &b_vx_x); }
  if( fChain->GetBranchStatus("vx_y") ) { fChain->SetBranchAddress("vx_y", vx_y, &b_vx_y); }
  if( fChain->GetBranchStatus("vx_z") ) { fChain->SetBranchAddress("vx_z", vx_z, &b_vx_z); }
  if( fChain->GetBranchStatus("nmu") ) { fChain->SetBranchAddress("nmu", &nmu, &b_nmu); }
  if( fChain->GetBranchStatus("mu_pt") ) { fChain->SetBranchAddress("mu_pt", mu_pt, &b_mu_pt); }
  if( fChain->GetBranchStatus("mu_eta") ) { fChain->SetBranchAddress("mu_eta", mu_eta, &b_mu_eta); }
  if( fChain->GetBranchStatus("mu_phi") ) { fChain->SetBranchAddress("mu_phi", mu_phi, &b_mu_phi); }
  if( fChain->GetBranchStatus("mu_q") ) { fChain->SetBranchAddress("mu_q", mu_q, &b_mu_q); }
  if( fChain->GetBranchStatus("mu_ptid") ) { fChain->SetBranchAddress("mu_ptid", mu_ptid, &b_mu_ptid); }
  if( fChain->GetBranchStatus("mu_etaid") ) { fChain->SetBranchAddress("mu_etaid", mu_etaid, &b_mu_etaid); }
  if( fChain->GetBranchStatus("mu_phiid") ) { fChain->SetBranchAddress("mu_phiid", mu_phiid, &b_mu_phiid); }
  if( fChain->GetBranchStatus("mu_qid") ) { fChain->SetBranchAddress("mu_qid", mu_qid, &b_mu_qid); }
  if( fChain->GetBranchStatus("mu_ptms") ) { fChain->SetBranchAddress("mu_ptms", mu_ptms, &b_mu_ptms); }
  if( fChain->GetBranchStatus("mu_etams") ) { fChain->SetBranchAddress("mu_etams", mu_etams, &b_mu_etams); }
  if( fChain->GetBranchStatus("mu_phims") ) { fChain->SetBranchAddress("mu_phims", mu_phims, &b_mu_phims); }
  if( fChain->GetBranchStatus("mu_qms") ) { fChain->SetBranchAddress("mu_qms", mu_qms, &b_mu_qms); }
  if( fChain->GetBranchStatus("mu_ptexms") ) { fChain->SetBranchAddress("mu_ptexms", mu_ptexms, &b_mu_ptexms); }
  if( fChain->GetBranchStatus("mu_etaexms") ) { fChain->SetBranchAddress("mu_etaexms", mu_etaexms, &b_mu_etaexms); }
  if( fChain->GetBranchStatus("mu_phiexms") ) { fChain->SetBranchAddress("mu_phiexms", mu_phiexms, &b_mu_phiexms); }
  if( fChain->GetBranchStatus("mu_qexms") ) { fChain->SetBranchAddress("mu_qexms", mu_qexms, &b_mu_qexms); }
  if( fChain->GetBranchStatus("mu_author") ) { fChain->SetBranchAddress("mu_author", mu_author, &b_mu_author); }
  if( fChain->GetBranchStatus("mu_primauthor") ) { fChain->SetBranchAddress("mu_primauthor", mu_primauthor, &b_mu_primauthor); }
  if( fChain->GetBranchStatus("mu_class") ) { fChain->SetBranchAddress("mu_class", mu_class, &b_mu_class); }
  if( fChain->GetBranchStatus("mu_bestmatch") ) { fChain->SetBranchAddress("mu_bestmatch", mu_bestmatch, &b_mu_bestmatch); }
  if( fChain->GetBranchStatus("mu_etcone10") ) { fChain->SetBranchAddress("mu_etcone10", mu_etcone10, &b_mu_etcone10); }
  if( fChain->GetBranchStatus("mu_etcone20") ) { fChain->SetBranchAddress("mu_etcone20", mu_etcone20, &b_mu_etcone20); }
  if( fChain->GetBranchStatus("mu_etcone30") ) { fChain->SetBranchAddress("mu_etcone30", mu_etcone30, &b_mu_etcone30); }
  if( fChain->GetBranchStatus("mu_etcone40") ) { fChain->SetBranchAddress("mu_etcone40", mu_etcone40, &b_mu_etcone40); }
  if( fChain->GetBranchStatus("mu_ptcone10") ) { fChain->SetBranchAddress("mu_ptcone10", mu_ptcone10, &b_mu_ptcone10); }
  if( fChain->GetBranchStatus("mu_ptcone20") ) { fChain->SetBranchAddress("mu_ptcone20", mu_ptcone20, &b_mu_ptcone20); }
  if( fChain->GetBranchStatus("mu_ptcone30") ) { fChain->SetBranchAddress("mu_ptcone30", mu_ptcone30, &b_mu_ptcone30); }
  if( fChain->GetBranchStatus("mu_ptcone40") ) { fChain->SetBranchAddress("mu_ptcone40", mu_ptcone40, &b_mu_ptcone40); }
  if( fChain->GetBranchStatus("mu_eloss") ) { fChain->SetBranchAddress("mu_eloss", mu_eloss, &b_mu_eloss); }
  if( fChain->GetBranchStatus("mu_d0") ) { fChain->SetBranchAddress("mu_d0", mu_d0, &b_mu_d0); }
  if( fChain->GetBranchStatus("mu_z0") ) { fChain->SetBranchAddress("mu_z0", mu_z0, &b_mu_z0); }
  if( fChain->GetBranchStatus("mu_db") ) { fChain->SetBranchAddress("mu_db", mu_db, &b_mu_db); }
  if( fChain->GetBranchStatus("mu_zb") ) { fChain->SetBranchAddress("mu_zb", mu_zb, &b_mu_zb); }
  if( fChain->GetBranchStatus("mu_dvx") ) { fChain->SetBranchAddress("mu_dvx", mu_dvx, &b_mu_dvx); }
  if( fChain->GetBranchStatus("mu_zvx") ) { fChain->SetBranchAddress("mu_zvx", mu_zvx, &b_mu_zvx); }
  if( fChain->GetBranchStatus("mu_dxpv") ) { fChain->SetBranchAddress("mu_dxpv", mu_dxpv, &b_mu_dxpv); }
  if( fChain->GetBranchStatus("mu_zxpv") ) { fChain->SetBranchAddress("mu_zxpv", mu_zxpv, &b_mu_zxpv); }
  if( fChain->GetBranchStatus("mu_covdb") ) { fChain->SetBranchAddress("mu_covdb", mu_covdb, &b_mu_covdb); }
  if( fChain->GetBranchStatus("mu_covdvx") ) { fChain->SetBranchAddress("mu_covdvx", mu_covdvx, &b_mu_covdvx); }
  if( fChain->GetBranchStatus("mu_covdxpv") ) { fChain->SetBranchAddress("mu_covdxpv", mu_covdxpv, &b_mu_covdxpv); }
  if( fChain->GetBranchStatus("mu_phiid_expv") ) { fChain->SetBranchAddress("mu_phiid_expv", mu_phiid_expv, &b_mu_phiid_expv); }
  if( fChain->GetBranchStatus("mu_phiexms_expv") ) { fChain->SetBranchAddress("mu_phiexms_expv", mu_phiexms_expv, &b_mu_phiexms_expv); }
  if( fChain->GetBranchStatus("mu_thetaid_expv") ) { fChain->SetBranchAddress("mu_thetaid_expv", mu_thetaid_expv, &b_mu_thetaid_expv); }
  if( fChain->GetBranchStatus("mu_thetaexms_expv") ) { fChain->SetBranchAddress("mu_thetaexms_expv", mu_thetaexms_expv, &b_mu_thetaexms_expv); }
  if( fChain->GetBranchStatus("mu_qoverpid_expv") ) { fChain->SetBranchAddress("mu_qoverpid_expv", mu_qoverpid_expv, &b_mu_qoverpid_expv); }
  if( fChain->GetBranchStatus("mu_qoverpexms_expv") ) { fChain->SetBranchAddress("mu_qoverpexms_expv", mu_qoverpexms_expv, &b_mu_qoverpexms_expv); }
  if( fChain->GetBranchStatus("mu_covqoverp_expv") ) { fChain->SetBranchAddress("mu_covqoverp_expv", mu_covqoverp_expv, &b_mu_covqoverp_expv); }
  if( fChain->GetBranchStatus("mu_covqoverpid_expv") ) { fChain->SetBranchAddress("mu_covqoverpid_expv", mu_covqoverpid_expv, &b_mu_covqoverpid_expv); }
  if( fChain->GetBranchStatus("mu_covqoverpexms_expv") ) { fChain->SetBranchAddress("mu_covqoverpexms_expv", mu_covqoverpexms_expv, &b_mu_covqoverpexms_expv); }
  if( fChain->GetBranchStatus("mu_vtxtype") ) { fChain->SetBranchAddress("mu_vtxtype", mu_vtxtype, &b_mu_vtxtype); }
  if( fChain->GetBranchStatus("mu_vtx") ) { fChain->SetBranchAddress("mu_vtx", mu_vtx, &b_mu_vtx); }
  if( fChain->GetBranchStatus("mu_nhitbl") ) { fChain->SetBranchAddress("mu_nhitbl", mu_nhitbl, &b_mu_nhitbl); }
  if( fChain->GetBranchStatus("mu_expectbl") ) { fChain->SetBranchAddress("mu_expectbl", mu_expectbl, &b_mu_expectbl); }
  if( fChain->GetBranchStatus("mu_nhitpix") ) { fChain->SetBranchAddress("mu_nhitpix", mu_nhitpix, &b_mu_nhitpix); }
  if( fChain->GetBranchStatus("mu_nholespix") ) { fChain->SetBranchAddress("mu_nholespix", mu_nholespix, &b_mu_nholespix); }
  if( fChain->GetBranchStatus("mu_ndeadsensorspix") ) { fChain->SetBranchAddress("mu_ndeadsensorspix", mu_ndeadsensorspix, &b_mu_ndeadsensorspix); }
  if( fChain->GetBranchStatus("mu_nhitsct") ) { fChain->SetBranchAddress("mu_nhitsct", mu_nhitsct, &b_mu_nhitsct); }
  if( fChain->GetBranchStatus("mu_nholessct") ) { fChain->SetBranchAddress("mu_nholessct", mu_nholessct, &b_mu_nholessct); }
  if( fChain->GetBranchStatus("mu_ndeadsensorssct") ) { fChain->SetBranchAddress("mu_ndeadsensorssct", mu_ndeadsensorssct, &b_mu_ndeadsensorssct); }
  if( fChain->GetBranchStatus("mu_nhittrt") ) { fChain->SetBranchAddress("mu_nhittrt", mu_nhittrt, &b_mu_nhittrt); }
  if( fChain->GetBranchStatus("mu_nholestrt") ) { fChain->SetBranchAddress("mu_nholestrt", mu_nholestrt, &b_mu_nholestrt); }
  if( fChain->GetBranchStatus("mu_nhitmdt") ) { fChain->SetBranchAddress("mu_nhitmdt", mu_nhitmdt, &b_mu_nhitmdt); }
  if( fChain->GetBranchStatus("mu_nhitcsceta") ) { fChain->SetBranchAddress("mu_nhitcsceta", mu_nhitcsceta, &b_mu_nhitcsceta); }
  if( fChain->GetBranchStatus("mu_nhitcscphi") ) { fChain->SetBranchAddress("mu_nhitcscphi", mu_nhitcscphi, &b_mu_nhitcscphi); }
  if( fChain->GetBranchStatus("mu_nhitrpceta") ) { fChain->SetBranchAddress("mu_nhitrpceta", mu_nhitrpceta, &b_mu_nhitrpceta); }
  if( fChain->GetBranchStatus("mu_nhitrpcphi") ) { fChain->SetBranchAddress("mu_nhitrpcphi", mu_nhitrpcphi, &b_mu_nhitrpcphi); }
  if( fChain->GetBranchStatus("mu_nhittgceta") ) { fChain->SetBranchAddress("mu_nhittgceta", mu_nhittgceta, &b_mu_nhittgceta); }
  if( fChain->GetBranchStatus("mu_nhittgcphi") ) { fChain->SetBranchAddress("mu_nhittgcphi", mu_nhittgcphi, &b_mu_nhittgcphi); }
  if( fChain->GetBranchStatus("mu_noutlierstrt") ) { fChain->SetBranchAddress("mu_noutlierstrt", mu_noutlierstrt, &b_mu_noutlierstrt); }
  if( fChain->GetBranchStatus("mu_nhitcscunspoilt") ) { fChain->SetBranchAddress("mu_nhitcscunspoilt", mu_nhitcscunspoilt, &b_mu_nhitcscunspoilt); }
  if( fChain->GetBranchStatus("mu_quality") ) { fChain->SetBranchAddress("mu_quality", mu_quality, &b_mu_quality); }
  if( fChain->GetBranchStatus("mu_trig_efmatchvector") ) { fChain->SetBranchAddress("mu_trig_efmatchvector", &mu_trig_efmatchvector_, &b_mu_trig_efmatchvector_); }
  if( fChain->GetBranchStatus("mu_trig_efmatchvector.first") ) { fChain->SetBranchAddress("mu_trig_efmatchvector.first", mu_trig_efmatchvector_first, &b_mu_trig_efmatchvector_first); }
  if( fChain->GetBranchStatus("mu_trig_efmatchvector.second") ) { fChain->SetBranchAddress("mu_trig_efmatchvector.second", mu_trig_efmatchvector_second, &b_mu_trig_efmatchvector_second); }
  if( fChain->GetBranchStatus("mu_trig_efmatchdelr") ) { fChain->SetBranchAddress("mu_trig_efmatchdelr", &mu_trig_efmatchdelr, &b_mu_trig_efmatchdelr); }
  if( fChain->GetBranchStatus("mu_passes_mcp") ) { fChain->SetBranchAddress("mu_passes_mcp", mu_passes_mcp, &b_mu_passes_mcp); }
  if( fChain->GetBranchStatus("mu_MET_wet") ) { fChain->SetBranchAddress("mu_MET_wet", mu_MET_wet, &b_mu_MET_wet); }
  if( fChain->GetBranchStatus("mu_MET_wpx") ) { fChain->SetBranchAddress("mu_MET_wpx", mu_MET_wpx, &b_mu_MET_wpx); }
  if( fChain->GetBranchStatus("mu_MET_wpy") ) { fChain->SetBranchAddress("mu_MET_wpy", mu_MET_wpy, &b_mu_MET_wpy); }
  if( fChain->GetBranchStatus("mu_MET_status") ) { fChain->SetBranchAddress("mu_MET_status", mu_MET_status, &b_mu_MET_status); }
  if( fChain->GetBranchStatus("mu_MET_wet2") ) { fChain->SetBranchAddress("mu_MET_wet2", mu_MET_wet2, &b_mu_MET_wet2); }
  if( fChain->GetBranchStatus("mu_MET_wpx2") ) { fChain->SetBranchAddress("mu_MET_wpx2", mu_MET_wpx2, &b_mu_MET_wpx2); }
  if( fChain->GetBranchStatus("mu_MET_wpy2") ) { fChain->SetBranchAddress("mu_MET_wpy2", mu_MET_wpy2, &b_mu_MET_wpy2); }
  if( fChain->GetBranchStatus("mu_MET_status2") ) { fChain->SetBranchAddress("mu_MET_status2", mu_MET_status2, &b_mu_MET_status2); }
  if( fChain->GetBranchStatus("nele") ) { fChain->SetBranchAddress("nele", &nele, &b_nele); }
  if( fChain->GetBranchStatus("ele_eta") ) { fChain->SetBranchAddress("ele_eta", ele_eta, &b_ele_eta); }
  if( fChain->GetBranchStatus("ele_phi") ) { fChain->SetBranchAddress("ele_phi", ele_phi, &b_ele_phi); }
  if( fChain->GetBranchStatus("ele_et") ) { fChain->SetBranchAddress("ele_et", ele_et, &b_ele_et); }
  if( fChain->GetBranchStatus("ele_MET_wet") ) { fChain->SetBranchAddress("ele_MET_wet", ele_MET_wet, &b_ele_MET_wet); }
  if( fChain->GetBranchStatus("ele_MET_wpx") ) { fChain->SetBranchAddress("ele_MET_wpx", ele_MET_wpx, &b_ele_MET_wpx); }
  if( fChain->GetBranchStatus("ele_MET_wpy") ) { fChain->SetBranchAddress("ele_MET_wpy", ele_MET_wpy, &b_ele_MET_wpy); }
  if( fChain->GetBranchStatus("ele_MET_status") ) { fChain->SetBranchAddress("ele_MET_status", ele_MET_status, &b_ele_MET_status); }
  if( fChain->GetBranchStatus("njet") ) { fChain->SetBranchAddress("njet", &njet, &b_njet); }
  if( fChain->GetBranchStatus("jet_type") ) { fChain->SetBranchAddress("jet_type", jet_type, &b_jet_type); }
  if( fChain->GetBranchStatus("jet_et") ) { fChain->SetBranchAddress("jet_et", jet_et, &b_jet_et); }
  if( fChain->GetBranchStatus("jet_pt") ) { fChain->SetBranchAddress("jet_pt", jet_pt, &b_jet_pt); }
  if( fChain->GetBranchStatus("jet_eta") ) { fChain->SetBranchAddress("jet_eta", jet_eta, &b_jet_eta); }
  if( fChain->GetBranchStatus("jet_phi") ) { fChain->SetBranchAddress("jet_phi", jet_phi, &b_jet_phi); }
  if( fChain->GetBranchStatus("jet_m") ) { fChain->SetBranchAddress("jet_m", jet_m, &b_jet_m); }
  if( fChain->GetBranchStatus("jet_pt_em") ) { fChain->SetBranchAddress("jet_pt_em", jet_pt_em, &b_jet_pt_em); }
  if( fChain->GetBranchStatus("jet_eta_em") ) { fChain->SetBranchAddress("jet_eta_em", jet_eta_em, &b_jet_eta_em); }
  if( fChain->GetBranchStatus("jet_phi_em") ) { fChain->SetBranchAddress("jet_phi_em", jet_phi_em, &b_jet_phi_em); }
  if( fChain->GetBranchStatus("jet_m_em") ) { fChain->SetBranchAddress("jet_m_em", jet_m_em, &b_jet_m_em); }
  if( fChain->GetBranchStatus("jet_pt_cs") ) { fChain->SetBranchAddress("jet_pt_cs", jet_pt_cs, &b_jet_pt_cs); }
  if( fChain->GetBranchStatus("jet_eta_cs") ) { fChain->SetBranchAddress("jet_eta_cs", jet_eta_cs, &b_jet_eta_cs); }
  if( fChain->GetBranchStatus("jet_phi_cs") ) { fChain->SetBranchAddress("jet_phi_cs", jet_phi_cs, &b_jet_phi_cs); }
  if( fChain->GetBranchStatus("jet_m_cs") ) { fChain->SetBranchAddress("jet_m_cs", jet_m_cs, &b_jet_m_cs); }
  if( fChain->GetBranchStatus("jet_eta_origin") ) { fChain->SetBranchAddress("jet_eta_origin", jet_eta_origin, &b_jet_eta_origin); }
  if( fChain->GetBranchStatus("jet_phi_origin") ) { fChain->SetBranchAddress("jet_phi_origin", jet_phi_origin, &b_jet_phi_origin); }
  if( fChain->GetBranchStatus("jet_m_origin") ) { fChain->SetBranchAddress("jet_m_origin", jet_m_origin, &b_jet_m_origin); }
  if( fChain->GetBranchStatus("jet_eta_origin_em") ) { fChain->SetBranchAddress("jet_eta_origin_em", jet_eta_origin_em, &b_jet_eta_origin_em); }
  if( fChain->GetBranchStatus("jet_phi_origin_em") ) { fChain->SetBranchAddress("jet_phi_origin_em", jet_phi_origin_em, &b_jet_phi_origin_em); }
  if( fChain->GetBranchStatus("jet_m_origin_em") ) { fChain->SetBranchAddress("jet_m_origin_em", jet_m_origin_em, &b_jet_m_origin_em); }
  if( fChain->GetBranchStatus("jet_gcwjes") ) { fChain->SetBranchAddress("jet_gcwjes", jet_gcwjes, &b_jet_gcwjes); }
  if( fChain->GetBranchStatus("jet_emjes") ) { fChain->SetBranchAddress("jet_emjes", jet_emjes, &b_jet_emjes); }
  if( fChain->GetBranchStatus("jet_isgood") ) { fChain->SetBranchAddress("jet_isgood", jet_isgood, &b_jet_isgood); }
  if( fChain->GetBranchStatus("jet_isbad") ) { fChain->SetBranchAddress("jet_isbad", jet_isbad, &b_jet_isbad); }
  if( fChain->GetBranchStatus("jet_isugly") ) { fChain->SetBranchAddress("jet_isugly", jet_isugly, &b_jet_isugly); }
  if( fChain->GetBranchStatus("jet_n90") ) { fChain->SetBranchAddress("jet_n90", jet_n90, &b_jet_n90); }
  if( fChain->GetBranchStatus("jet_hecquality") ) { fChain->SetBranchAddress("jet_hecquality", jet_hecquality, &b_jet_hecquality); }
  if( fChain->GetBranchStatus("jet_larquality") ) { fChain->SetBranchAddress("jet_larquality", jet_larquality, &b_jet_larquality); }
  if( fChain->GetBranchStatus("jet_averagelarqf") ) { fChain->SetBranchAddress("jet_averagelarqf", jet_averagelarqf, &b_jet_averagelarqf); }
  if( fChain->GetBranchStatus("jet_negativee") ) { fChain->SetBranchAddress("jet_negativee", jet_negativee, &b_jet_negativee); }
  if( fChain->GetBranchStatus("jet_sumpttrk") ) { fChain->SetBranchAddress("jet_sumpttrk", jet_sumpttrk, &b_jet_sumpttrk); }
  if( fChain->GetBranchStatus("jet_ntrk") ) { fChain->SetBranchAddress("jet_ntrk", jet_ntrk, &b_jet_ntrk); }
  if( fChain->GetBranchStatus("jet_jvtxf") ) { fChain->SetBranchAddress("jet_jvtxf", jet_jvtxf, &b_jet_jvtxf); }
  if( fChain->GetBranchStatus("jet_jvf") ) { fChain->SetBranchAddress("jet_jvf", jet_jvf, &b_jet_jvf); }
  if( fChain->GetBranchStatus("jet_bchcorr") ) { fChain->SetBranchAddress("jet_bchcorr", jet_bchcorr, &b_jet_bchcorr); }
  if( fChain->GetBranchStatus("jet_bchcorrjet") ) { fChain->SetBranchAddress("jet_bchcorrjet", jet_bchcorrjet, &b_jet_bchcorrjet); }
  if( fChain->GetBranchStatus("jet_tgap3f") ) { fChain->SetBranchAddress("jet_tgap3f", jet_tgap3f, &b_jet_tgap3f); }
  if( fChain->GetBranchStatus("jet_time") ) { fChain->SetBranchAddress("jet_time", jet_time, &b_jet_time); }
  if( fChain->GetBranchStatus("jet_emf") ) { fChain->SetBranchAddress("jet_emf", jet_emf, &b_jet_emf); }
  if( fChain->GetBranchStatus("jet_hecf") ) { fChain->SetBranchAddress("jet_hecf", jet_hecf, &b_jet_hecf); }
  if( fChain->GetBranchStatus("jet_fmax") ) { fChain->SetBranchAddress("jet_fmax", jet_fmax, &b_jet_fmax); }
  if( fChain->GetBranchStatus("jet_vtx") ) { fChain->SetBranchAddress("jet_vtx", jet_vtx, &b_jet_vtx); }
  if( fChain->GetBranchStatus("jet_MET_wet") ) { fChain->SetBranchAddress("jet_MET_wet", jet_MET_wet, &b_jet_MET_wet); }
  if( fChain->GetBranchStatus("jet_MET_wpx") ) { fChain->SetBranchAddress("jet_MET_wpx", jet_MET_wpx, &b_jet_MET_wpx); }
  if( fChain->GetBranchStatus("jet_MET_wpy") ) { fChain->SetBranchAddress("jet_MET_wpy", jet_MET_wpy, &b_jet_MET_wpy); }
  if( fChain->GetBranchStatus("jet_MET_status") ) { fChain->SetBranchAddress("jet_MET_status", jet_MET_status, &b_jet_MET_status); }
  if( fChain->GetBranchStatus("met_reffinal") ) { fChain->SetBranchAddress("met_reffinal", &met_reffinal, &b_met_reffinal); }
  if( fChain->GetBranchStatus("met_reffinal_phi") ) { fChain->SetBranchAddress("met_reffinal_phi", &met_reffinal_phi, &b_met_reffinal_phi); }
  if( fChain->GetBranchStatus("met_reffinal_sumet") ) { fChain->SetBranchAddress("met_reffinal_sumet", &met_reffinal_sumet, &b_met_reffinal_sumet); }
  if( fChain->GetBranchStatus("met_reffinal_em") ) { fChain->SetBranchAddress("met_reffinal_em", &met_reffinal_em, &b_met_reffinal_em); }
  if( fChain->GetBranchStatus("met_reffinal_em_phi") ) { fChain->SetBranchAddress("met_reffinal_em_phi", &met_reffinal_em_phi, &b_met_reffinal_em_phi); }
  if( fChain->GetBranchStatus("met_reffinal_em_sumet") ) { fChain->SetBranchAddress("met_reffinal_em_sumet", &met_reffinal_em_sumet, &b_met_reffinal_em_sumet); }
  if( fChain->GetBranchStatus("met_refjet") ) { fChain->SetBranchAddress("met_refjet", &met_refjet, &b_met_refjet); }
  if( fChain->GetBranchStatus("met_refjet_phi") ) { fChain->SetBranchAddress("met_refjet_phi", &met_refjet_phi, &b_met_refjet_phi); }
  if( fChain->GetBranchStatus("met_refjet_sumet") ) { fChain->SetBranchAddress("met_refjet_sumet", &met_refjet_sumet, &b_met_refjet_sumet); }
  if( fChain->GetBranchStatus("met_cellout") ) { fChain->SetBranchAddress("met_cellout", &met_cellout, &b_met_cellout); }
  if( fChain->GetBranchStatus("met_cellout_phi") ) { fChain->SetBranchAddress("met_cellout_phi", &met_cellout_phi, &b_met_cellout_phi); }
  if( fChain->GetBranchStatus("met_cellout_sumet") ) { fChain->SetBranchAddress("met_cellout_sumet", &met_cellout_sumet, &b_met_cellout_sumet); }
  if( fChain->GetBranchStatus("met_softjets") ) { fChain->SetBranchAddress("met_softjets", &met_softjets, &b_met_softjets); }
  if( fChain->GetBranchStatus("met_softjets_phi") ) { fChain->SetBranchAddress("met_softjets_phi", &met_softjets_phi, &b_met_softjets_phi); }
  if( fChain->GetBranchStatus("met_softjets_sumet") ) { fChain->SetBranchAddress("met_softjets_sumet", &met_softjets_sumet, &b_met_softjets_sumet); }
  if( fChain->GetBranchStatus("met_cellout_eflow") ) { fChain->SetBranchAddress("met_cellout_eflow", &met_cellout_eflow, &b_met_cellout_eflow); }
  if( fChain->GetBranchStatus("met_cellout_eflow_phi") ) { fChain->SetBranchAddress("met_cellout_eflow_phi", &met_cellout_eflow_phi, &b_met_cellout_eflow_phi); }
  if( fChain->GetBranchStatus("met_cellout_eflow_sumet") ) { fChain->SetBranchAddress("met_cellout_eflow_sumet", &met_cellout_eflow_sumet, &b_met_cellout_eflow_sumet); }
  if( fChain->GetBranchStatus("met_cellout_eflow_muid") ) { fChain->SetBranchAddress("met_cellout_eflow_muid", &met_cellout_eflow_muid, &b_met_cellout_eflow_muid); }
  if( fChain->GetBranchStatus("met_cellout_eflow_muid_phi") ) { fChain->SetBranchAddress("met_cellout_eflow_muid_phi", &met_cellout_eflow_muid_phi, &b_met_cellout_eflow_muid_phi); }
  if( fChain->GetBranchStatus("met_cellout_eflow_muid_sumet") ) { fChain->SetBranchAddress("met_cellout_eflow_muid_sumet", &met_cellout_eflow_muid_sumet, &b_met_cellout_eflow_muid_sumet); }
  if( fChain->GetBranchStatus("met_cryo") ) { fChain->SetBranchAddress("met_cryo", &met_cryo, &b_met_cryo); }
  if( fChain->GetBranchStatus("met_cryo_phi") ) { fChain->SetBranchAddress("met_cryo_phi", &met_cryo_phi, &b_met_cryo_phi); }
  if( fChain->GetBranchStatus("met_cryo_sumet") ) { fChain->SetBranchAddress("met_cryo_sumet", &met_cryo_sumet, &b_met_cryo_sumet); }
  if( fChain->GetBranchStatus("met_refmuon") ) { fChain->SetBranchAddress("met_refmuon", &met_refmuon, &b_met_refmuon); }
  if( fChain->GetBranchStatus("met_refmuon_phi") ) { fChain->SetBranchAddress("met_refmuon_phi", &met_refmuon_phi, &b_met_refmuon_phi); }
  if( fChain->GetBranchStatus("met_refmuon_sumet") ) { fChain->SetBranchAddress("met_refmuon_sumet", &met_refmuon_sumet, &b_met_refmuon_sumet); }
  if( fChain->GetBranchStatus("met_refmuon_track") ) { fChain->SetBranchAddress("met_refmuon_track", &met_refmuon_track, &b_met_refmuon_track); }
  if( fChain->GetBranchStatus("met_refmuon_track_phi") ) { fChain->SetBranchAddress("met_refmuon_track_phi", &met_refmuon_track_phi, &b_met_refmuon_track_phi); }
  if( fChain->GetBranchStatus("met_refmuon_track_sumet") ) { fChain->SetBranchAddress("met_refmuon_track_sumet", &met_refmuon_track_sumet, &b_met_refmuon_track_sumet); }
  if( fChain->GetBranchStatus("met_muonboy") ) { fChain->SetBranchAddress("met_muonboy", &met_muonboy, &b_met_muonboy); }
  if( fChain->GetBranchStatus("met_muonboy_phi") ) { fChain->SetBranchAddress("met_muonboy_phi", &met_muonboy_phi, &b_met_muonboy_phi); }
  if( fChain->GetBranchStatus("met_muonboy_sumet") ) { fChain->SetBranchAddress("met_muonboy_sumet", &met_muonboy_sumet, &b_met_muonboy_sumet); }
  if( fChain->GetBranchStatus("met_muid") ) { fChain->SetBranchAddress("met_muid", &met_muid, &b_met_muid); }
  if( fChain->GetBranchStatus("met_muid_phi") ) { fChain->SetBranchAddress("met_muid_phi", &met_muid_phi, &b_met_muid_phi); }
  if( fChain->GetBranchStatus("met_muid_sumet") ) { fChain->SetBranchAddress("met_muid_sumet", &met_muid_sumet, &b_met_muid_sumet); }
  if( fChain->GetBranchStatus("met_topo") ) { fChain->SetBranchAddress("met_topo", &met_topo, &b_met_topo); }
  if( fChain->GetBranchStatus("met_topo_phi") ) { fChain->SetBranchAddress("met_topo_phi", &met_topo_phi, &b_met_topo_phi); }
  if( fChain->GetBranchStatus("met_topo_sumet") ) { fChain->SetBranchAddress("met_topo_sumet", &met_topo_sumet, &b_met_topo_sumet); }
  if( fChain->GetBranchStatus("met_topo_eta45") ) { fChain->SetBranchAddress("met_topo_eta45", &met_topo_eta45, &b_met_topo_eta45); }
  if( fChain->GetBranchStatus("met_topo_eta45_phi") ) { fChain->SetBranchAddress("met_topo_eta45_phi", &met_topo_eta45_phi, &b_met_topo_eta45_phi); }
  if( fChain->GetBranchStatus("met_topo_eta45_sumet") ) { fChain->SetBranchAddress("met_topo_eta45_sumet", &met_topo_eta45_sumet, &b_met_topo_eta45_sumet); }
  if( fChain->GetBranchStatus("met_lochadtopo") ) { fChain->SetBranchAddress("met_lochadtopo", &met_lochadtopo, &b_met_lochadtopo); }
  if( fChain->GetBranchStatus("met_lochadtopo_phi") ) { fChain->SetBranchAddress("met_lochadtopo_phi", &met_lochadtopo_phi, &b_met_lochadtopo_phi); }
  if( fChain->GetBranchStatus("met_lochadtopo_sumet") ) { fChain->SetBranchAddress("met_lochadtopo_sumet", &met_lochadtopo_sumet, &b_met_lochadtopo_sumet); }
  if( fChain->GetBranchStatus("met_lochadtopo_eta45") ) { fChain->SetBranchAddress("met_lochadtopo_eta45", &met_lochadtopo_eta45, &b_met_lochadtopo_eta45); }
  if( fChain->GetBranchStatus("met_lochadtopo_eta45_phi") ) { fChain->SetBranchAddress("met_lochadtopo_eta45_phi", &met_lochadtopo_eta45_phi, &b_met_lochadtopo_eta45_phi); }
  if( fChain->GetBranchStatus("met_lochadtopo_eta45_sumet") ) { fChain->SetBranchAddress("met_lochadtopo_eta45_sumet", &met_lochadtopo_eta45_sumet, &b_met_lochadtopo_eta45_sumet); }
  if( fChain->GetBranchStatus("met_reffinal_eta45") ) { fChain->SetBranchAddress("met_reffinal_eta45", &met_reffinal_eta45, &b_met_reffinal_eta45); }
  if( fChain->GetBranchStatus("met_reffinal_eta45_phi") ) { fChain->SetBranchAddress("met_reffinal_eta45_phi", &met_reffinal_eta45_phi, &b_met_reffinal_eta45_phi); }
  if( fChain->GetBranchStatus("met_reffinal_eta45_sumet") ) { fChain->SetBranchAddress("met_reffinal_eta45_sumet", &met_reffinal_eta45_sumet, &b_met_reffinal_eta45_sumet); }
  if( fChain->GetBranchStatus("met_muon") ) { fChain->SetBranchAddress("met_muon", &met_muon, &b_met_muon); }
  if( fChain->GetBranchStatus("met_muon_phi") ) { fChain->SetBranchAddress("met_muon_phi", &met_muon_phi, &b_met_muon_phi); }
  if( fChain->GetBranchStatus("met_muon_sumet") ) { fChain->SetBranchAddress("met_muon_sumet", &met_muon_sumet, &b_met_muon_sumet); }
  if( fChain->GetBranchStatus("met_muon_muid") ) { fChain->SetBranchAddress("met_muon_muid", &met_muon_muid, &b_met_muon_muid); }
  if( fChain->GetBranchStatus("met_muon_muid_phi") ) { fChain->SetBranchAddress("met_muon_muid_phi", &met_muon_muid_phi, &b_met_muon_muid_phi); }
  if( fChain->GetBranchStatus("met_muon_muid_sumet") ) { fChain->SetBranchAddress("met_muon_muid_sumet", &met_muon_muid_sumet, &b_met_muon_muid_sumet); }
  if( fChain->GetBranchStatus("met_refele") ) { fChain->SetBranchAddress("met_refele", &met_refele, &b_met_refele); }
  if( fChain->GetBranchStatus("met_refele_phi") ) { fChain->SetBranchAddress("met_refele_phi", &met_refele_phi, &b_met_refele_phi); }
  if( fChain->GetBranchStatus("met_refele_sumet") ) { fChain->SetBranchAddress("met_refele_sumet", &met_refele_sumet, &b_met_refele_sumet); }
  if( fChain->GetBranchStatus("met_reftau") ) { fChain->SetBranchAddress("met_reftau", &met_reftau, &b_met_reftau); }
  if( fChain->GetBranchStatus("met_reftau_phi") ) { fChain->SetBranchAddress("met_reftau_phi", &met_reftau_phi, &b_met_reftau_phi); }
  if( fChain->GetBranchStatus("met_reftau_sumet") ) { fChain->SetBranchAddress("met_reftau_sumet", &met_reftau_sumet, &b_met_reftau_sumet); }
  if( fChain->GetBranchStatus("met_refgamma") ) { fChain->SetBranchAddress("met_refgamma", &met_refgamma, &b_met_refgamma); }
  if( fChain->GetBranchStatus("met_refgamma_phi") ) { fChain->SetBranchAddress("met_refgamma_phi", &met_refgamma_phi, &b_met_refgamma_phi); }
  if( fChain->GetBranchStatus("met_refgamma_sumet") ) { fChain->SetBranchAddress("met_refgamma_sumet", &met_refgamma_sumet, &b_met_refgamma_sumet); }
  if( fChain->GetBranchStatus("met_truth") ) { fChain->SetBranchAddress("met_truth", &met_truth, &b_met_truth); }
  if( fChain->GetBranchStatus("met_truth_phi") ) { fChain->SetBranchAddress("met_truth_phi", &met_truth_phi, &b_met_truth_phi); }
  if( fChain->GetBranchStatus("met_truth_sumet") ) { fChain->SetBranchAddress("met_truth_sumet", &met_truth_sumet, &b_met_truth_sumet); }
  if( fChain->GetBranchStatus("met_truth_nonint") ) { fChain->SetBranchAddress("met_truth_nonint", &met_truth_nonint, &b_met_truth_nonint); }
  if( fChain->GetBranchStatus("met_truth_nonint_phi") ) { fChain->SetBranchAddress("met_truth_nonint_phi", &met_truth_nonint_phi, &b_met_truth_nonint_phi); }
  if( fChain->GetBranchStatus("met_truth_nonint_sumet") ) { fChain->SetBranchAddress("met_truth_nonint_sumet", &met_truth_nonint_sumet, &b_met_truth_nonint_sumet); }
  if( fChain->GetBranchStatus("met_truth_muons") ) { fChain->SetBranchAddress("met_truth_muons", &met_truth_muons, &b_met_truth_muons); }
  if( fChain->GetBranchStatus("met_truth_muons_phi") ) { fChain->SetBranchAddress("met_truth_muons_phi", &met_truth_muons_phi, &b_met_truth_muons_phi); }
  if( fChain->GetBranchStatus("met_truth_muons_sumet") ) { fChain->SetBranchAddress("met_truth_muons_sumet", &met_truth_muons_sumet, &b_met_truth_muons_sumet); }
  if( fChain->GetBranchStatus("met_track") ) { fChain->SetBranchAddress("met_track", &met_track, &b_met_track); }
  if( fChain->GetBranchStatus("met_track_phi") ) { fChain->SetBranchAddress("met_track_phi", &met_track_phi, &b_met_track_phi); }
  if( fChain->GetBranchStatus("met_track_sumet") ) { fChain->SetBranchAddress("met_track_sumet", &met_track_sumet, &b_met_track_sumet); }
  if( fChain->GetBranchStatus("nmc") ) { fChain->SetBranchAddress("nmc", &nmc, &b_nmc); }
  if( fChain->GetBranchStatus("mc_weight") ) { fChain->SetBranchAddress("mc_weight", &mc_weight, &b_mc_weight); }
  if( fChain->GetBranchStatus("mc_hfor") ) { fChain->SetBranchAddress("mc_hfor", &mc_hfor, &b_mc_hfor); }
  if( fChain->GetBranchStatus("mc_channel") ) { fChain->SetBranchAddress("mc_channel", &mc_channel, &b_mc_channel); }
  if( fChain->GetBranchStatus("mc_pdgid") ) { fChain->SetBranchAddress("mc_pdgid", mc_pdgid, &b_mc_pdgid); }
  if( fChain->GetBranchStatus("mc_e") ) { fChain->SetBranchAddress("mc_e", mc_e, &b_mc_e); }
  if( fChain->GetBranchStatus("mc_pt") ) { fChain->SetBranchAddress("mc_pt", mc_pt, &b_mc_pt); }
  if( fChain->GetBranchStatus("mc_phi") ) { fChain->SetBranchAddress("mc_phi", mc_phi, &b_mc_phi); }
  if( fChain->GetBranchStatus("mc_eta") ) { fChain->SetBranchAddress("mc_eta", mc_eta, &b_mc_eta); }
  if( fChain->GetBranchStatus("mc_parent") ) { fChain->SetBranchAddress("mc_parent", mc_parent, &b_mc_parent); }
  if( fChain->GetBranchStatus("mc_status") ) { fChain->SetBranchAddress("mc_status", mc_status, &b_mc_status); }
  if( fChain->GetBranchStatus("mc_vx_z") ) { fChain->SetBranchAddress("mc_vx_z", mc_vx_z, &b_mc_vx_z); }
  if( fChain->GetBranchStatus("mcevt_hardindex") ) { fChain->SetBranchAddress("mcevt_hardindex", &mcevt_hardindex, &b_mcevt_hardindex); }
  if( fChain->GetBranchStatus("mcevt_pdfset") ) { fChain->SetBranchAddress("mcevt_pdfset", &mcevt_pdfset, &b_mcevt_pdfset); }
  if( fChain->GetBranchStatus("mcevt_weight") ) { fChain->SetBranchAddress("mcevt_weight", &mcevt_weight, &b_mcevt_weight); }
  if( fChain->GetBranchStatus("mcevt_sigid") ) { fChain->SetBranchAddress("mcevt_sigid", &mcevt_sigid, &b_mcevt_sigid); }
  if( fChain->GetBranchStatus("mcevt_event_scale") ) { fChain->SetBranchAddress("mcevt_event_scale", &mcevt_event_scale, &b_mcevt_event_scale); }
  if( fChain->GetBranchStatus("mcevt_pdf_scale") ) { fChain->SetBranchAddress("mcevt_pdf_scale", &mcevt_pdf_scale, &b_mcevt_pdf_scale); }
  if( fChain->GetBranchStatus("mcevt_alphaQCD") ) { fChain->SetBranchAddress("mcevt_alphaQCD", &mcevt_alphaQCD, &b_mcevt_alphaQCD); }
  if( fChain->GetBranchStatus("mcevt_alphaQED") ) { fChain->SetBranchAddress("mcevt_alphaQED", &mcevt_alphaQED, &b_mcevt_alphaQED); }
  if( fChain->GetBranchStatus("mcevt_id1") ) { fChain->SetBranchAddress("mcevt_id1", &mcevt_id1, &b_mcevt_id1); }
  if( fChain->GetBranchStatus("mcevt_id2") ) { fChain->SetBranchAddress("mcevt_id2", &mcevt_id2, &b_mcevt_id2); }
  if( fChain->GetBranchStatus("mcevt_pdf_x1") ) { fChain->SetBranchAddress("mcevt_pdf_x1", &mcevt_pdf_x1, &b_mcevt_pdf_x1); }
  if( fChain->GetBranchStatus("mcevt_pdf_x2") ) { fChain->SetBranchAddress("mcevt_pdf_x2", &mcevt_pdf_x2, &b_mcevt_pdf_x2); }
  if( fChain->GetBranchStatus("mcevt_pdf1") ) { fChain->SetBranchAddress("mcevt_pdf1", &mcevt_pdf1, &b_mcevt_pdf1); }
  if( fChain->GetBranchStatus("mcevt_pdf2") ) { fChain->SetBranchAddress("mcevt_pdf2", &mcevt_pdf2, &b_mcevt_pdf2); }

  // EXPERIMENTAL: enable TTree cache:
  Int_t cachesize = 10000000;   //200 MBytes
  fChain->SetCacheSize(cachesize);
  if(false) { // old way: cache all branches
    fChain->AddBranchToCache("*",kTRUE);
  } else {    // new way: only cache enabled branches
    TIter iter(fChain->GetListOfBranches());
    while(TBranch *sub = (TBranch *)iter.Next()) {
      if( fChain->GetBranchStatus(sub->GetName()) ) {
        fChain->AddBranchToCache(sub->GetName(),kTRUE);
      } else {
        continue;
      }
    }
  }

  fChain->StopCacheLearningPhase();

}
