#include <TROOT.h>
#include <TChain.h>
#include <TBranch.h>
#include <TFile.h>
#include <vector>
#include "TrigFTKAna/Trees/D3PDPeterTreeV30.h"

D3PDPeterTreeV30::D3PDPeterTreeV30(TTree *tree)
{
  Init(tree);
}

D3PDPeterTreeV30::~D3PDPeterTreeV30()
{
  if (!fChain) return;
  // delete fChain->GetCurrentFile();
}

Int_t D3PDPeterTreeV30::GetEntry(Long64_t entry)
{
  // Read contents of entry.
  if (!fChain) return 0;
  fChain->LoadTree(entry); // EXPERIMENTAL: needed to enable TTree cache
  return fChain->GetEntry(entry);
}
Long64_t D3PDPeterTreeV30::LoadTree(Long64_t entry)
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

void D3PDPeterTreeV30::Init(TTree *tree)
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
  trig_ef = 0;
  trig_l2 = 0;
  trig_l1 = 0;
  mu_trig_l1matchdelr = 0;
  mu_trig_l2matchdelr = 0;
  mu_trig_l2samatchdelr = 0;
  mu_trig_efmatchdelr = 0;
  ele_trig_l1matchdelr = 0;
  ele_trig_l2matchdelr = 0;
  ele_trig_efmatchdelr = 0;
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
  fChain->SetBranchStatus("trig_l2*",1);
  fChain->SetBranchStatus("trig_l1*",1);
  // vertex information
  fChain->SetBranchStatus("nvx*",1);
  fChain->SetBranchStatus("vx_*",1);
  // muons
  fChain->SetBranchStatus("nmu*",1);
  fChain->SetBranchStatus("mu_*",1);
  // electrons
  fChain->SetBranchStatus("nele*",1);
  fChain->SetBranchStatus("ele*",1);
  /*
  // tracks
  fChain->SetBranchStatus("ntrk*",1);
  fChain->SetBranchStatus("trk*",1);
  // jpsi
  fChain->SetBranchStatus("njpsi*",1);
  fChain->SetBranchStatus("jpsi*",1);
  // photons
  fChain->SetBranchStatus("nph*",1);
  fChain->SetBranchStatus("ph*",1);
  */
  //jets
  fChain->SetBranchStatus("njet*",1);
  fChain->SetBranchStatus("jet_*",1);
  // MET
  fChain->SetBranchStatus("met_*",1);
  // MC truth
  fChain->SetBranchStatus("nmc*",1);
  fChain->SetBranchStatus("mc_*",1);
  // MC event info
  //fChain->SetBranchStatus("mcevt_*",1);

  // Anton March 2012: disable some additional variables (that we never use)
  // to conserve some precious memory that ROOT will try to steal via TBufferFile
  if(true) {
    // truth classifier variables (they are unfilled in V29 ntuple anyway)
    fChain->SetBranchStatus("mu_ptype",0);
    fChain->SetBranchStatus("mu_porigin",0);
    fChain->SetBranchStatus("mu_poutcome",0);
    fChain->SetBranchStatus("ele_ptype",0);
    fChain->SetBranchStatus("ele_porigin",0);
    fChain->SetBranchStatus("ele_poutcome",0);
    fChain->SetBranchStatus("mc_ptype",0);
    fChain->SetBranchStatus("mc_porigin",0);
    fChain->SetBranchStatus("mc_poutcome",0);
    fChain->SetBranchStatus("mc_pmotherpdg",0);
    // muons
    fChain->SetBranchStatus("mu*_nucone*",0);
    fChain->SetBranchStatus("mu_type",0);
    fChain->SetBranchStatus("mu_*cid",0);
    fChain->SetBranchStatus("mu_*exid",0);
    fChain->SetBranchStatus("mu_iscombined",0);
    fChain->SetBranchStatus("mu_issegtagged",0);
    fChain->SetBranchStatus("mu_ptconemod*",0);
    fChain->SetBranchStatus("mu_nMDT*",0);
    fChain->SetBranchStatus("mu_scatt*",0);
    // electrons
    fChain->SetBranchStatus("ele*_nucone*",0);
    fChain->SetBranchStatus("ele_*best",0);
    // jet substructure
    fChain->SetBranchStatus("jet_is*",0);
    fChain->SetBranchStatus("jet_js_*",0);
    // MC truth
    fChain->SetBranchStatus("mc_m",0);

  }

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
  if( fChain->GetBranchStatus("trig_l2") ) { fChain->SetBranchAddress("trig_l2", &trig_l2, &b_trig_l2); }
  if( fChain->GetBranchStatus("trig_l1") ) { fChain->SetBranchAddress("trig_l1", &trig_l1, &b_trig_l1); }
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
  if( fChain->GetBranchStatus("mu_type") ) { fChain->SetBranchAddress("mu_type", mu_type, &b_mu_type); }
  if( fChain->GetBranchStatus("mu_pt") ) { fChain->SetBranchAddress("mu_pt", mu_pt, &b_mu_pt); }
  if( fChain->GetBranchStatus("mu_eta") ) { fChain->SetBranchAddress("mu_eta", mu_eta, &b_mu_eta); }
  if( fChain->GetBranchStatus("mu_phi") ) { fChain->SetBranchAddress("mu_phi", mu_phi, &b_mu_phi); }
  if( fChain->GetBranchStatus("mu_q") ) { fChain->SetBranchAddress("mu_q", mu_q, &b_mu_q); }
  if( fChain->GetBranchStatus("mu_ptcid") ) { fChain->SetBranchAddress("mu_ptcid", mu_ptcid, &b_mu_ptcid); }
  if( fChain->GetBranchStatus("mu_etacid") ) { fChain->SetBranchAddress("mu_etacid", mu_etacid, &b_mu_etacid); }
  if( fChain->GetBranchStatus("mu_phicid") ) { fChain->SetBranchAddress("mu_phicid", mu_phicid, &b_mu_phicid); }
  if( fChain->GetBranchStatus("mu_qcid") ) { fChain->SetBranchAddress("mu_qcid", mu_qcid, &b_mu_qcid); }
  if( fChain->GetBranchStatus("mu_ptid") ) { fChain->SetBranchAddress("mu_ptid", mu_ptid, &b_mu_ptid); }
  if( fChain->GetBranchStatus("mu_etaid") ) { fChain->SetBranchAddress("mu_etaid", mu_etaid, &b_mu_etaid); }
  if( fChain->GetBranchStatus("mu_phiid") ) { fChain->SetBranchAddress("mu_phiid", mu_phiid, &b_mu_phiid); }
  if( fChain->GetBranchStatus("mu_qid") ) { fChain->SetBranchAddress("mu_qid", mu_qid, &b_mu_qid); }
  if( fChain->GetBranchStatus("mu_ptexid") ) { fChain->SetBranchAddress("mu_ptexid", mu_ptexid, &b_mu_ptexid); }
  if( fChain->GetBranchStatus("mu_etaexid") ) { fChain->SetBranchAddress("mu_etaexid", mu_etaexid, &b_mu_etaexid); }
  if( fChain->GetBranchStatus("mu_phiexid") ) { fChain->SetBranchAddress("mu_phiexid", mu_phiexid, &b_mu_phiexid); }
  if( fChain->GetBranchStatus("mu_qexid") ) { fChain->SetBranchAddress("mu_qexid", mu_qexid, &b_mu_qexid); }
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
  if( fChain->GetBranchStatus("mu_iscombined") ) { fChain->SetBranchAddress("mu_iscombined", mu_iscombined, &b_mu_iscombined); }
  if( fChain->GetBranchStatus("mu_issegtagged") ) { fChain->SetBranchAddress("mu_issegtagged", mu_issegtagged, &b_mu_issegtagged); }
  if( fChain->GetBranchStatus("mu_fitchi2prob") ) { fChain->SetBranchAddress("mu_fitchi2prob", mu_fitchi2prob, &b_mu_fitchi2prob); }
  if( fChain->GetBranchStatus("mu_matchchi2prob") ) { fChain->SetBranchAddress("mu_matchchi2prob", mu_matchchi2prob, &b_mu_matchchi2prob); }
  if( fChain->GetBranchStatus("mu_fitchi2") ) { fChain->SetBranchAddress("mu_fitchi2", mu_fitchi2, &b_mu_fitchi2); }
  if( fChain->GetBranchStatus("mu_matchchi2") ) { fChain->SetBranchAddress("mu_matchchi2", mu_matchchi2, &b_mu_matchchi2); }
  if( fChain->GetBranchStatus("mu_bestmatch") ) { fChain->SetBranchAddress("mu_bestmatch", mu_bestmatch, &b_mu_bestmatch); }
  if( fChain->GetBranchStatus("mu_nucone10") ) { fChain->SetBranchAddress("mu_nucone10", mu_nucone10, &b_mu_nucone10); }
  if( fChain->GetBranchStatus("mu_nucone20") ) { fChain->SetBranchAddress("mu_nucone20", mu_nucone20, &b_mu_nucone20); }
  if( fChain->GetBranchStatus("mu_nucone30") ) { fChain->SetBranchAddress("mu_nucone30", mu_nucone30, &b_mu_nucone30); }
  if( fChain->GetBranchStatus("mu_nucone40") ) { fChain->SetBranchAddress("mu_nucone40", mu_nucone40, &b_mu_nucone40); }
  if( fChain->GetBranchStatus("mu_etcone10") ) { fChain->SetBranchAddress("mu_etcone10", mu_etcone10, &b_mu_etcone10); }
  if( fChain->GetBranchStatus("mu_etcone20") ) { fChain->SetBranchAddress("mu_etcone20", mu_etcone20, &b_mu_etcone20); }
  if( fChain->GetBranchStatus("mu_etcone30") ) { fChain->SetBranchAddress("mu_etcone30", mu_etcone30, &b_mu_etcone30); }
  if( fChain->GetBranchStatus("mu_etcone40") ) { fChain->SetBranchAddress("mu_etcone40", mu_etcone40, &b_mu_etcone40); }
  if( fChain->GetBranchStatus("mu_ptcone10") ) { fChain->SetBranchAddress("mu_ptcone10", mu_ptcone10, &b_mu_ptcone10); }
  if( fChain->GetBranchStatus("mu_ptcone20") ) { fChain->SetBranchAddress("mu_ptcone20", mu_ptcone20, &b_mu_ptcone20); }
  if( fChain->GetBranchStatus("mu_ptcone30") ) { fChain->SetBranchAddress("mu_ptcone30", mu_ptcone30, &b_mu_ptcone30); }
  if( fChain->GetBranchStatus("mu_ptcone40") ) { fChain->SetBranchAddress("mu_ptcone40", mu_ptcone40, &b_mu_ptcone40); }
  if( fChain->GetBranchStatus("mu_ptconemod10") ) { fChain->SetBranchAddress("mu_ptconemod10", mu_ptconemod10, &b_mu_ptconemod10); }
  if( fChain->GetBranchStatus("mu_ptconemod20") ) { fChain->SetBranchAddress("mu_ptconemod20", mu_ptconemod20, &b_mu_ptconemod20); }
  if( fChain->GetBranchStatus("mu_ptconemod30") ) { fChain->SetBranchAddress("mu_ptconemod30", mu_ptconemod30, &b_mu_ptconemod30); }
  if( fChain->GetBranchStatus("mu_ptconemod40") ) { fChain->SetBranchAddress("mu_ptconemod40", mu_ptconemod40, &b_mu_ptconemod40); }
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
  if( fChain->GetBranchStatus("mu_nMDTBIHits") ) { fChain->SetBranchAddress("mu_nMDTBIHits", mu_nMDTBIHits, &b_mu_nMDTBIHits); }
  if( fChain->GetBranchStatus("mu_nMDTBMHits") ) { fChain->SetBranchAddress("mu_nMDTBMHits", mu_nMDTBMHits, &b_mu_nMDTBMHits); }
  if( fChain->GetBranchStatus("mu_nMDTBOHits") ) { fChain->SetBranchAddress("mu_nMDTBOHits", mu_nMDTBOHits, &b_mu_nMDTBOHits); }
  if( fChain->GetBranchStatus("mu_nMDTBEEHits") ) { fChain->SetBranchAddress("mu_nMDTBEEHits", mu_nMDTBEEHits, &b_mu_nMDTBEEHits); }
  if( fChain->GetBranchStatus("mu_nMDTBIS78Hits") ) { fChain->SetBranchAddress("mu_nMDTBIS78Hits", mu_nMDTBIS78Hits, &b_mu_nMDTBIS78Hits); }
  if( fChain->GetBranchStatus("mu_nMDTEIHits") ) { fChain->SetBranchAddress("mu_nMDTEIHits", mu_nMDTEIHits, &b_mu_nMDTEIHits); }
  if( fChain->GetBranchStatus("mu_nMDTEMHits") ) { fChain->SetBranchAddress("mu_nMDTEMHits", mu_nMDTEMHits, &b_mu_nMDTEMHits); }
  if( fChain->GetBranchStatus("mu_nMDTEOHits") ) { fChain->SetBranchAddress("mu_nMDTEOHits", mu_nMDTEOHits, &b_mu_nMDTEOHits); }
  if( fChain->GetBranchStatus("mu_nMDTEEHits") ) { fChain->SetBranchAddress("mu_nMDTEEHits", mu_nMDTEEHits, &b_mu_nMDTEEHits); }
  if( fChain->GetBranchStatus("mu_quality") ) { fChain->SetBranchAddress("mu_quality", mu_quality, &b_mu_quality); }
  if( fChain->GetBranchStatus("mu_scatt_curv_sig") ) { fChain->SetBranchAddress("mu_scatt_curv_sig", mu_scatt_curv_sig, &b_mu_scatt_curv_sig); }
  if( fChain->GetBranchStatus("mu_scatt_neighb_sig") ) { fChain->SetBranchAddress("mu_scatt_neighb_sig", mu_scatt_neighb_sig, &b_mu_scatt_neighb_sig); }
  if( fChain->GetBranchStatus("mu_field_integral") ) { fChain->SetBranchAddress("mu_field_integral", mu_field_integral, &b_mu_field_integral); }
  if( fChain->GetBranchStatus("mu_trig_l1matchvector") ) { fChain->SetBranchAddress("mu_trig_l1matchvector", &mu_trig_l1matchvector_, &b_mu_trig_l1matchvector_); }
  if( fChain->GetBranchStatus("mu_trig_l1matchvector.first") ) { fChain->SetBranchAddress("mu_trig_l1matchvector.first", mu_trig_l1matchvector_first, &b_mu_trig_l1matchvector_first); }
  if( fChain->GetBranchStatus("mu_trig_l1matchvector.second") ) { fChain->SetBranchAddress("mu_trig_l1matchvector.second", mu_trig_l1matchvector_second, &b_mu_trig_l1matchvector_second); }
  if( fChain->GetBranchStatus("mu_trig_l1matchdelr") ) { fChain->SetBranchAddress("mu_trig_l1matchdelr", &mu_trig_l1matchdelr, &b_mu_trig_l1matchdelr); }
  if( fChain->GetBranchStatus("mu_trig_l2matchvector") ) { fChain->SetBranchAddress("mu_trig_l2matchvector", &mu_trig_l2matchvector_, &b_mu_trig_l2matchvector_); }
  if( fChain->GetBranchStatus("mu_trig_l2matchvector.first") ) { fChain->SetBranchAddress("mu_trig_l2matchvector.first", mu_trig_l2matchvector_first, &b_mu_trig_l2matchvector_first); }
  if( fChain->GetBranchStatus("mu_trig_l2matchvector.second") ) { fChain->SetBranchAddress("mu_trig_l2matchvector.second", mu_trig_l2matchvector_second, &b_mu_trig_l2matchvector_second); }
  if( fChain->GetBranchStatus("mu_trig_l2matchdelr") ) { fChain->SetBranchAddress("mu_trig_l2matchdelr", &mu_trig_l2matchdelr, &b_mu_trig_l2matchdelr); }
  if( fChain->GetBranchStatus("mu_trig_l2samatchvector") ) { fChain->SetBranchAddress("mu_trig_l2samatchvector", &mu_trig_l2samatchvector_, &b_mu_trig_l2samatchvector_); }
  if( fChain->GetBranchStatus("mu_trig_l2samatchvector.first") ) { fChain->SetBranchAddress("mu_trig_l2samatchvector.first", mu_trig_l2samatchvector_first, &b_mu_trig_l2samatchvector_first); }
  if( fChain->GetBranchStatus("mu_trig_l2samatchvector.second") ) { fChain->SetBranchAddress("mu_trig_l2samatchvector.second", mu_trig_l2samatchvector_second, &b_mu_trig_l2samatchvector_second); }
  if( fChain->GetBranchStatus("mu_trig_l2samatchdelr") ) { fChain->SetBranchAddress("mu_trig_l2samatchdelr", &mu_trig_l2samatchdelr, &b_mu_trig_l2samatchdelr); }
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
  if( fChain->GetBranchStatus("mu_ptype") ) { fChain->SetBranchAddress("mu_ptype", mu_ptype, &b_mu_ptype); }
  if( fChain->GetBranchStatus("mu_porigin") ) { fChain->SetBranchAddress("mu_porigin", mu_porigin, &b_mu_porigin); }
  if( fChain->GetBranchStatus("mu_poutcome") ) { fChain->SetBranchAddress("mu_poutcome", mu_poutcome, &b_mu_poutcome); }
  if( fChain->GetBranchStatus("nele") ) { fChain->SetBranchAddress("nele", &nele, &b_nele); }
  if( fChain->GetBranchStatus("ele_author") ) { fChain->SetBranchAddress("ele_author", ele_author, &b_ele_author); }
  if( fChain->GetBranchStatus("ele_eta") ) { fChain->SetBranchAddress("ele_eta", ele_eta, &b_ele_eta); }
  if( fChain->GetBranchStatus("ele_phi") ) { fChain->SetBranchAddress("ele_phi", ele_phi, &b_ele_phi); }
  if( fChain->GetBranchStatus("ele_et") ) { fChain->SetBranchAddress("ele_et", ele_et, &b_ele_et); }
  if( fChain->GetBranchStatus("ele_q") ) { fChain->SetBranchAddress("ele_q", ele_q, &b_ele_q); }
  if( fChain->GetBranchStatus("ele_etaclus") ) { fChain->SetBranchAddress("ele_etaclus", ele_etaclus, &b_ele_etaclus); }
  if( fChain->GetBranchStatus("ele_phiclus") ) { fChain->SetBranchAddress("ele_phiclus", ele_phiclus, &b_ele_phiclus); }
  if( fChain->GetBranchStatus("ele_etclus") ) { fChain->SetBranchAddress("ele_etclus", ele_etclus, &b_ele_etclus); }
  if( fChain->GetBranchStatus("ele_etatrk") ) { fChain->SetBranchAddress("ele_etatrk", ele_etatrk, &b_ele_etatrk); }
  if( fChain->GetBranchStatus("ele_phitrk") ) { fChain->SetBranchAddress("ele_phitrk", ele_phitrk, &b_ele_phitrk); }
  if( fChain->GetBranchStatus("ele_pttrk") ) { fChain->SetBranchAddress("ele_pttrk", ele_pttrk, &b_ele_pttrk); }
  if( fChain->GetBranchStatus("ele_etabest") ) { fChain->SetBranchAddress("ele_etabest", ele_etabest, &b_ele_etabest); }
  if( fChain->GetBranchStatus("ele_phibest") ) { fChain->SetBranchAddress("ele_phibest", ele_phibest, &b_ele_phibest); }
  if( fChain->GetBranchStatus("ele_etbest") ) { fChain->SetBranchAddress("ele_etbest", ele_etbest, &b_ele_etbest); }
  if( fChain->GetBranchStatus("ele_etap") ) { fChain->SetBranchAddress("ele_etap", ele_etap, &b_ele_etap); }
  if( fChain->GetBranchStatus("ele_f1") ) { fChain->SetBranchAddress("ele_f1", ele_f1, &b_ele_f1); }
  if( fChain->GetBranchStatus("ele_isEM") ) { fChain->SetBranchAddress("ele_isEM", ele_isEM, &b_ele_isEM); }
  if( fChain->GetBranchStatus("ele_nucone20") ) { fChain->SetBranchAddress("ele_nucone20", ele_nucone20, &b_ele_nucone20); }
  if( fChain->GetBranchStatus("ele_nucone30") ) { fChain->SetBranchAddress("ele_nucone30", ele_nucone30, &b_ele_nucone30); }
  if( fChain->GetBranchStatus("ele_nucone40") ) { fChain->SetBranchAddress("ele_nucone40", ele_nucone40, &b_ele_nucone40); }
  if( fChain->GetBranchStatus("ele_etcone20") ) { fChain->SetBranchAddress("ele_etcone20", ele_etcone20, &b_ele_etcone20); }
  if( fChain->GetBranchStatus("ele_etcone30") ) { fChain->SetBranchAddress("ele_etcone30", ele_etcone30, &b_ele_etcone30); }
  if( fChain->GetBranchStatus("ele_etcone40") ) { fChain->SetBranchAddress("ele_etcone40", ele_etcone40, &b_ele_etcone40); }
  if( fChain->GetBranchStatus("ele_etcone20_cor") ) { fChain->SetBranchAddress("ele_etcone20_cor", ele_etcone20_cor, &b_ele_etcone20_cor); }
  if( fChain->GetBranchStatus("ele_etcone30_cor") ) { fChain->SetBranchAddress("ele_etcone30_cor", ele_etcone30_cor, &b_ele_etcone30_cor); }
  if( fChain->GetBranchStatus("ele_etcone40_cor") ) { fChain->SetBranchAddress("ele_etcone40_cor", ele_etcone40_cor, &b_ele_etcone40_cor); }
  if( fChain->GetBranchStatus("ele_ptcone20") ) { fChain->SetBranchAddress("ele_ptcone20", ele_ptcone20, &b_ele_ptcone20); }
  if( fChain->GetBranchStatus("ele_ptcone30") ) { fChain->SetBranchAddress("ele_ptcone30", ele_ptcone30, &b_ele_ptcone30); }
  if( fChain->GetBranchStatus("ele_ptcone40") ) { fChain->SetBranchAddress("ele_ptcone40", ele_ptcone40, &b_ele_ptcone40); }
  if( fChain->GetBranchStatus("ele_d0") ) { fChain->SetBranchAddress("ele_d0", ele_d0, &b_ele_d0); }
  if( fChain->GetBranchStatus("ele_z0") ) { fChain->SetBranchAddress("ele_z0", ele_z0, &b_ele_z0); }
  if( fChain->GetBranchStatus("ele_db") ) { fChain->SetBranchAddress("ele_db", ele_db, &b_ele_db); }
  if( fChain->GetBranchStatus("ele_zb") ) { fChain->SetBranchAddress("ele_zb", ele_zb, &b_ele_zb); }
  if( fChain->GetBranchStatus("ele_dvx") ) { fChain->SetBranchAddress("ele_dvx", ele_dvx, &b_ele_dvx); }
  if( fChain->GetBranchStatus("ele_zvx") ) { fChain->SetBranchAddress("ele_zvx", ele_zvx, &b_ele_zvx); }
  if( fChain->GetBranchStatus("ele_dxpv") ) { fChain->SetBranchAddress("ele_dxpv", ele_dxpv, &b_ele_dxpv); }
  if( fChain->GetBranchStatus("ele_zxpv") ) { fChain->SetBranchAddress("ele_zxpv", ele_zxpv, &b_ele_zxpv); }
  if( fChain->GetBranchStatus("ele_covdb") ) { fChain->SetBranchAddress("ele_covdb", ele_covdb, &b_ele_covdb); }
  if( fChain->GetBranchStatus("ele_covdvx") ) { fChain->SetBranchAddress("ele_covdvx", ele_covdvx, &b_ele_covdvx); }
  if( fChain->GetBranchStatus("ele_covdxpv") ) { fChain->SetBranchAddress("ele_covdxpv", ele_covdxpv, &b_ele_covdxpv); }
  if( fChain->GetBranchStatus("ele_vtx") ) { fChain->SetBranchAddress("ele_vtx", ele_vtx, &b_ele_vtx); }
  if( fChain->GetBranchStatus("ele_nhitpix") ) { fChain->SetBranchAddress("ele_nhitpix", ele_nhitpix, &b_ele_nhitpix); }
  if( fChain->GetBranchStatus("ele_nhitsct") ) { fChain->SetBranchAddress("ele_nhitsct", ele_nhitsct, &b_ele_nhitsct); }
  if( fChain->GetBranchStatus("ele_nhittrt") ) { fChain->SetBranchAddress("ele_nhittrt", ele_nhittrt, &b_ele_nhittrt); }
  if( fChain->GetBranchStatus("ele_quality") ) { fChain->SetBranchAddress("ele_quality", ele_quality, &b_ele_quality); }
  if( fChain->GetBranchStatus("ele_isgoodoq") ) { fChain->SetBranchAddress("ele_isgoodoq", ele_isgoodoq, &b_ele_isgoodoq); }
  if( fChain->GetBranchStatus("ele_oq") ) { fChain->SetBranchAddress("ele_oq", ele_oq, &b_ele_oq); }
  if( fChain->GetBranchStatus("ele_rhad") ) { fChain->SetBranchAddress("ele_rhad", ele_rhad, &b_ele_rhad); }
  if( fChain->GetBranchStatus("ele_rhad1") ) { fChain->SetBranchAddress("ele_rhad1", ele_rhad1, &b_ele_rhad1); }
  if( fChain->GetBranchStatus("ele_reta") ) { fChain->SetBranchAddress("ele_reta", ele_reta, &b_ele_reta); }
  if( fChain->GetBranchStatus("ele_rphi") ) { fChain->SetBranchAddress("ele_rphi", ele_rphi, &b_ele_rphi); }
  if( fChain->GetBranchStatus("ele_w2") ) { fChain->SetBranchAddress("ele_w2", ele_w2, &b_ele_w2); }
  if( fChain->GetBranchStatus("ele_wstot") ) { fChain->SetBranchAddress("ele_wstot", ele_wstot, &b_ele_wstot); }
  if( fChain->GetBranchStatus("ele_demaxs1") ) { fChain->SetBranchAddress("ele_demaxs1", ele_demaxs1, &b_ele_demaxs1); }
  if( fChain->GetBranchStatus("ele_deltaeta") ) { fChain->SetBranchAddress("ele_deltaeta", ele_deltaeta, &b_ele_deltaeta); }
  if( fChain->GetBranchStatus("ele_deltaphi") ) { fChain->SetBranchAddress("ele_deltaphi", ele_deltaphi, &b_ele_deltaphi); }
  if( fChain->GetBranchStatus("ele_trratio") ) { fChain->SetBranchAddress("ele_trratio", ele_trratio, &b_ele_trratio); }
  if( fChain->GetBranchStatus("ele_nhitbl") ) { fChain->SetBranchAddress("ele_nhitbl", ele_nhitbl, &b_ele_nhitbl); }
  if( fChain->GetBranchStatus("ele_noutliersbl") ) { fChain->SetBranchAddress("ele_noutliersbl", ele_noutliersbl, &b_ele_noutliersbl); }
  if( fChain->GetBranchStatus("ele_expectbl") ) { fChain->SetBranchAddress("ele_expectbl", ele_expectbl, &b_ele_expectbl); }
  if( fChain->GetBranchStatus("ele_noutlierspix") ) { fChain->SetBranchAddress("ele_noutlierspix", ele_noutlierspix, &b_ele_noutlierspix); }
  if( fChain->GetBranchStatus("ele_noutlierssct") ) { fChain->SetBranchAddress("ele_noutlierssct", ele_noutlierssct, &b_ele_noutlierssct); }
  if( fChain->GetBranchStatus("ele_eta2") ) { fChain->SetBranchAddress("ele_eta2", ele_eta2, &b_ele_eta2); }
  if( fChain->GetBranchStatus("ele_et2") ) { fChain->SetBranchAddress("ele_et2", ele_et2, &b_ele_et2); }
  if( fChain->GetBranchStatus("ele_trig_l1matchvector") ) { fChain->SetBranchAddress("ele_trig_l1matchvector", &ele_trig_l1matchvector_, &b_ele_trig_l1matchvector_); }
  if( fChain->GetBranchStatus("ele_trig_l1matchvector.first") ) { fChain->SetBranchAddress("ele_trig_l1matchvector.first", ele_trig_l1matchvector_first, &b_ele_trig_l1matchvector_first); }
  if( fChain->GetBranchStatus("ele_trig_l1matchvector.second") ) { fChain->SetBranchAddress("ele_trig_l1matchvector.second", ele_trig_l1matchvector_second, &b_ele_trig_l1matchvector_second); }
  if( fChain->GetBranchStatus("ele_trig_l1matchdelr") ) { fChain->SetBranchAddress("ele_trig_l1matchdelr", &ele_trig_l1matchdelr, &b_ele_trig_l1matchdelr); }
  if( fChain->GetBranchStatus("ele_trig_l2matchvector") ) { fChain->SetBranchAddress("ele_trig_l2matchvector", &ele_trig_l2matchvector_, &b_ele_trig_l2matchvector_); }
  if( fChain->GetBranchStatus("ele_trig_l2matchvector.first") ) { fChain->SetBranchAddress("ele_trig_l2matchvector.first", ele_trig_l2matchvector_first, &b_ele_trig_l2matchvector_first); }
  if( fChain->GetBranchStatus("ele_trig_l2matchvector.second") ) { fChain->SetBranchAddress("ele_trig_l2matchvector.second", ele_trig_l2matchvector_second, &b_ele_trig_l2matchvector_second); }
  if( fChain->GetBranchStatus("ele_trig_l2matchdelr") ) { fChain->SetBranchAddress("ele_trig_l2matchdelr", &ele_trig_l2matchdelr, &b_ele_trig_l2matchdelr); }
  if( fChain->GetBranchStatus("ele_trig_efmatchvector") ) { fChain->SetBranchAddress("ele_trig_efmatchvector", &ele_trig_efmatchvector_, &b_ele_trig_efmatchvector_); }
  if( fChain->GetBranchStatus("ele_trig_efmatchvector.first") ) { fChain->SetBranchAddress("ele_trig_efmatchvector.first", ele_trig_efmatchvector_first, &b_ele_trig_efmatchvector_first); }
  if( fChain->GetBranchStatus("ele_trig_efmatchvector.second") ) { fChain->SetBranchAddress("ele_trig_efmatchvector.second", ele_trig_efmatchvector_second, &b_ele_trig_efmatchvector_second); }
  if( fChain->GetBranchStatus("ele_trig_efmatchdelr") ) { fChain->SetBranchAddress("ele_trig_efmatchdelr", &ele_trig_efmatchdelr, &b_ele_trig_efmatchdelr); }
  if( fChain->GetBranchStatus("ele_MET_wet") ) { fChain->SetBranchAddress("ele_MET_wet", ele_MET_wet, &b_ele_MET_wet); }
  if( fChain->GetBranchStatus("ele_MET_wpx") ) { fChain->SetBranchAddress("ele_MET_wpx", ele_MET_wpx, &b_ele_MET_wpx); }
  if( fChain->GetBranchStatus("ele_MET_wpy") ) { fChain->SetBranchAddress("ele_MET_wpy", ele_MET_wpy, &b_ele_MET_wpy); }
  if( fChain->GetBranchStatus("ele_MET_status") ) { fChain->SetBranchAddress("ele_MET_status", ele_MET_status, &b_ele_MET_status); }
  if( fChain->GetBranchStatus("ele_ptype") ) { fChain->SetBranchAddress("ele_ptype", ele_ptype, &b_ele_ptype); }
  if( fChain->GetBranchStatus("ele_porigin") ) { fChain->SetBranchAddress("ele_porigin", ele_porigin, &b_ele_porigin); }
  if( fChain->GetBranchStatus("ele_poutcome") ) { fChain->SetBranchAddress("ele_poutcome", ele_poutcome, &b_ele_poutcome); }
  if( fChain->GetBranchStatus("nph") ) { fChain->SetBranchAddress("nph", &nph, &b_nph); }
  if( fChain->GetBranchStatus("ph_author") ) { fChain->SetBranchAddress("ph_author", ph_author, &b_ph_author); }
  if( fChain->GetBranchStatus("ph_eta") ) { fChain->SetBranchAddress("ph_eta", ph_eta, &b_ph_eta); }
  if( fChain->GetBranchStatus("ph_phi") ) { fChain->SetBranchAddress("ph_phi", ph_phi, &b_ph_phi); }
  if( fChain->GetBranchStatus("ph_etaclus") ) { fChain->SetBranchAddress("ph_etaclus", ph_etaclus, &b_ph_etaclus); }
  if( fChain->GetBranchStatus("ph_phiclus") ) { fChain->SetBranchAddress("ph_phiclus", ph_phiclus, &b_ph_phiclus); }
  if( fChain->GetBranchStatus("ph_et") ) { fChain->SetBranchAddress("ph_et", ph_et, &b_ph_et); }
  if( fChain->GetBranchStatus("ph_eclus") ) { fChain->SetBranchAddress("ph_eclus", ph_eclus, &b_ph_eclus); }
  if( fChain->GetBranchStatus("ph_isEM") ) { fChain->SetBranchAddress("ph_isEM", ph_isEM, &b_ph_isEM); }
  if( fChain->GetBranchStatus("ph_isgoodoq") ) { fChain->SetBranchAddress("ph_isgoodoq", ph_isgoodoq, &b_ph_isgoodoq); }
  if( fChain->GetBranchStatus("ph_oq") ) { fChain->SetBranchAddress("ph_oq", ph_oq, &b_ph_oq); }
  if( fChain->GetBranchStatus("ph_quality") ) { fChain->SetBranchAddress("ph_quality", ph_quality, &b_ph_quality); }
  if( fChain->GetBranchStatus("ph_etcone20") ) { fChain->SetBranchAddress("ph_etcone20", ph_etcone20, &b_ph_etcone20); }
  if( fChain->GetBranchStatus("ph_etcone30") ) { fChain->SetBranchAddress("ph_etcone30", ph_etcone30, &b_ph_etcone30); }
  if( fChain->GetBranchStatus("ph_etcone40") ) { fChain->SetBranchAddress("ph_etcone40", ph_etcone40, &b_ph_etcone40); }
  if( fChain->GetBranchStatus("ph_etcone20_cor") ) { fChain->SetBranchAddress("ph_etcone20_cor", ph_etcone20_cor, &b_ph_etcone20_cor); }
  if( fChain->GetBranchStatus("ph_etcone30_cor") ) { fChain->SetBranchAddress("ph_etcone30_cor", ph_etcone30_cor, &b_ph_etcone30_cor); }
  if( fChain->GetBranchStatus("ph_etcone40_cor") ) { fChain->SetBranchAddress("ph_etcone40_cor", ph_etcone40_cor, &b_ph_etcone40_cor); }
  if( fChain->GetBranchStatus("ph_ptcone20") ) { fChain->SetBranchAddress("ph_ptcone20", ph_ptcone20, &b_ph_ptcone20); }
  if( fChain->GetBranchStatus("ph_ptcone30") ) { fChain->SetBranchAddress("ph_ptcone30", ph_ptcone30, &b_ph_ptcone30); }
  if( fChain->GetBranchStatus("ph_ptcone40") ) { fChain->SetBranchAddress("ph_ptcone40", ph_ptcone40, &b_ph_ptcone40); }
  if( fChain->GetBranchStatus("ph_isRecovered") ) { fChain->SetBranchAddress("ph_isRecovered", ph_isRecovered, &b_ph_isRecovered); }
  if( fChain->GetBranchStatus("ph_convFlag") ) { fChain->SetBranchAddress("ph_convFlag", ph_convFlag, &b_ph_convFlag); }
  if( fChain->GetBranchStatus("ph_rconv") ) { fChain->SetBranchAddress("ph_rconv", ph_rconv, &b_ph_rconv); }
  if( fChain->GetBranchStatus("ph_zconv") ) { fChain->SetBranchAddress("ph_zconv", ph_zconv, &b_ph_zconv); }
  if( fChain->GetBranchStatus("ph_etas2") ) { fChain->SetBranchAddress("ph_etas2", ph_etas2, &b_ph_etas2); }
  if( fChain->GetBranchStatus("ph_ethad1") ) { fChain->SetBranchAddress("ph_ethad1", ph_ethad1, &b_ph_ethad1); }
  if( fChain->GetBranchStatus("ph_ethad") ) { fChain->SetBranchAddress("ph_ethad", ph_ethad, &b_ph_ethad); }
  if( fChain->GetBranchStatus("ph_e277") ) { fChain->SetBranchAddress("ph_e277", ph_e277, &b_ph_e277); }
  if( fChain->GetBranchStatus("ph_e237") ) { fChain->SetBranchAddress("ph_e237", ph_e237, &b_ph_e237); }
  if( fChain->GetBranchStatus("ph_e233") ) { fChain->SetBranchAddress("ph_e233", ph_e233, &b_ph_e233); }
  if( fChain->GetBranchStatus("ph_weta2") ) { fChain->SetBranchAddress("ph_weta2", ph_weta2, &b_ph_weta2); }
  if( fChain->GetBranchStatus("ph_f1") ) { fChain->SetBranchAddress("ph_f1", ph_f1, &b_ph_f1); }
  if( fChain->GetBranchStatus("ph_emax") ) { fChain->SetBranchAddress("ph_emax", ph_emax, &b_ph_emax); }
  if( fChain->GetBranchStatus("ph_emaxs2") ) { fChain->SetBranchAddress("ph_emaxs2", ph_emaxs2, &b_ph_emaxs2); }
  if( fChain->GetBranchStatus("ph_demaxs1") ) { fChain->SetBranchAddress("ph_demaxs1", ph_demaxs1, &b_ph_demaxs1); }
  if( fChain->GetBranchStatus("ph_fside") ) { fChain->SetBranchAddress("ph_fside", ph_fside, &b_ph_fside); }
  if( fChain->GetBranchStatus("ph_wstot") ) { fChain->SetBranchAddress("ph_wstot", ph_wstot, &b_ph_wstot); }
  if( fChain->GetBranchStatus("ph_ws3") ) { fChain->SetBranchAddress("ph_ws3", ph_ws3, &b_ph_ws3); }
  if( fChain->GetBranchStatus("ph_emins1") ) { fChain->SetBranchAddress("ph_emins1", ph_emins1, &b_ph_emins1); }
  if( fChain->GetBranchStatus("ph_etap") ) { fChain->SetBranchAddress("ph_etap", ph_etap, &b_ph_etap); }
  if( fChain->GetBranchStatus("ph_deltaeta") ) { fChain->SetBranchAddress("ph_deltaeta", ph_deltaeta, &b_ph_deltaeta); }
  if( fChain->GetBranchStatus("ph_MET_wet") ) { fChain->SetBranchAddress("ph_MET_wet", ph_MET_wet, &b_ph_MET_wet); }
  if( fChain->GetBranchStatus("ph_MET_wpx") ) { fChain->SetBranchAddress("ph_MET_wpx", ph_MET_wpx, &b_ph_MET_wpx); }
  if( fChain->GetBranchStatus("ph_MET_wpy") ) { fChain->SetBranchAddress("ph_MET_wpy", ph_MET_wpy, &b_ph_MET_wpy); }
  if( fChain->GetBranchStatus("ph_MET_status") ) { fChain->SetBranchAddress("ph_MET_status", ph_MET_status, &b_ph_MET_status); }
  if( fChain->GetBranchStatus("ph_ptype") ) { fChain->SetBranchAddress("ph_ptype", ph_ptype, &b_ph_ptype); }
  if( fChain->GetBranchStatus("ph_porigin") ) { fChain->SetBranchAddress("ph_porigin", ph_porigin, &b_ph_porigin); }
  if( fChain->GetBranchStatus("ph_poutcome") ) { fChain->SetBranchAddress("ph_poutcome", ph_poutcome, &b_ph_poutcome); }
  if( fChain->GetBranchStatus("ntrk") ) { fChain->SetBranchAddress("ntrk", &ntrk, &b_ntrk); }
  if( fChain->GetBranchStatus("trk_pt") ) { fChain->SetBranchAddress("trk_pt", trk_pt, &b_trk_pt); }
  if( fChain->GetBranchStatus("trk_eta") ) { fChain->SetBranchAddress("trk_eta", trk_eta, &b_trk_eta); }
  if( fChain->GetBranchStatus("trk_phi") ) { fChain->SetBranchAddress("trk_phi", trk_phi, &b_trk_phi); }
  if( fChain->GetBranchStatus("trk_m") ) { fChain->SetBranchAddress("trk_m", trk_m, &b_trk_m); }
  if( fChain->GetBranchStatus("trk_q") ) { fChain->SetBranchAddress("trk_q", trk_q, &b_trk_q); }
  if( fChain->GetBranchStatus("trk_mu") ) { fChain->SetBranchAddress("trk_mu", trk_mu, &b_trk_mu); }
  if( fChain->GetBranchStatus("trk_nucone10") ) { fChain->SetBranchAddress("trk_nucone10", trk_nucone10, &b_trk_nucone10); }
  if( fChain->GetBranchStatus("trk_nucone20") ) { fChain->SetBranchAddress("trk_nucone20", trk_nucone20, &b_trk_nucone20); }
  if( fChain->GetBranchStatus("trk_nucone30") ) { fChain->SetBranchAddress("trk_nucone30", trk_nucone30, &b_trk_nucone30); }
  if( fChain->GetBranchStatus("trk_nucone40") ) { fChain->SetBranchAddress("trk_nucone40", trk_nucone40, &b_trk_nucone40); }
  if( fChain->GetBranchStatus("trk_ptcone10") ) { fChain->SetBranchAddress("trk_ptcone10", trk_ptcone10, &b_trk_ptcone10); }
  if( fChain->GetBranchStatus("trk_ptcone20") ) { fChain->SetBranchAddress("trk_ptcone20", trk_ptcone20, &b_trk_ptcone20); }
  if( fChain->GetBranchStatus("trk_ptcone30") ) { fChain->SetBranchAddress("trk_ptcone30", trk_ptcone30, &b_trk_ptcone30); }
  if( fChain->GetBranchStatus("trk_ptcone40") ) { fChain->SetBranchAddress("trk_ptcone40", trk_ptcone40, &b_trk_ptcone40); }
  if( fChain->GetBranchStatus("trk_vtx") ) { fChain->SetBranchAddress("trk_vtx", trk_vtx, &b_trk_vtx); }
  if( fChain->GetBranchStatus("trk_dxpv") ) { fChain->SetBranchAddress("trk_dxpv", trk_dxpv, &b_trk_dxpv); }
  if( fChain->GetBranchStatus("trk_zxpv") ) { fChain->SetBranchAddress("trk_zxpv", trk_zxpv, &b_trk_zxpv); }
  if( fChain->GetBranchStatus("trk_covdxpv") ) { fChain->SetBranchAddress("trk_covdxpv", trk_covdxpv, &b_trk_covdxpv); }
  if( fChain->GetBranchStatus("trk_nhitbl") ) { fChain->SetBranchAddress("trk_nhitbl", trk_nhitbl, &b_trk_nhitbl); }
  if( fChain->GetBranchStatus("trk_expectbl") ) { fChain->SetBranchAddress("trk_expectbl", trk_expectbl, &b_trk_expectbl); }
  if( fChain->GetBranchStatus("trk_nhitpix") ) { fChain->SetBranchAddress("trk_nhitpix", trk_nhitpix, &b_trk_nhitpix); }
  if( fChain->GetBranchStatus("trk_nholespix") ) { fChain->SetBranchAddress("trk_nholespix", trk_nholespix, &b_trk_nholespix); }
  if( fChain->GetBranchStatus("trk_ndeadsensorspix") ) { fChain->SetBranchAddress("trk_ndeadsensorspix", trk_ndeadsensorspix, &b_trk_ndeadsensorspix); }
  if( fChain->GetBranchStatus("trk_nhitsct") ) { fChain->SetBranchAddress("trk_nhitsct", trk_nhitsct, &b_trk_nhitsct); }
  if( fChain->GetBranchStatus("trk_nholessct") ) { fChain->SetBranchAddress("trk_nholessct", trk_nholessct, &b_trk_nholessct); }
  if( fChain->GetBranchStatus("trk_ndeadsensorssct") ) { fChain->SetBranchAddress("trk_ndeadsensorssct", trk_ndeadsensorssct, &b_trk_ndeadsensorssct); }
  if( fChain->GetBranchStatus("trk_nhittrt") ) { fChain->SetBranchAddress("trk_nhittrt", trk_nhittrt, &b_trk_nhittrt); }
  if( fChain->GetBranchStatus("trk_nholestrt") ) { fChain->SetBranchAddress("trk_nholestrt", trk_nholestrt, &b_trk_nholestrt); }
  if( fChain->GetBranchStatus("trk_noutlierstrt") ) { fChain->SetBranchAddress("trk_noutlierstrt", trk_noutlierstrt, &b_trk_noutlierstrt); }
  if( fChain->GetBranchStatus("trk_ptype") ) { fChain->SetBranchAddress("trk_ptype", trk_ptype, &b_trk_ptype); }
  if( fChain->GetBranchStatus("trk_porigin") ) { fChain->SetBranchAddress("trk_porigin", trk_porigin, &b_trk_porigin); }
  if( fChain->GetBranchStatus("trk_poutcome") ) { fChain->SetBranchAddress("trk_poutcome", trk_poutcome, &b_trk_poutcome); }
  if( fChain->GetBranchStatus("trk_pmatchprob") ) { fChain->SetBranchAddress("trk_pmatchprob", trk_pmatchprob, &b_trk_pmatchprob); }
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
  if( fChain->GetBranchStatus("jet_tagSV0") ) { fChain->SetBranchAddress("jet_tagSV0", jet_tagSV0, &b_jet_tagSV0); }
  if( fChain->GetBranchStatus("jet_tagSV1") ) { fChain->SetBranchAddress("jet_tagSV1", jet_tagSV1, &b_jet_tagSV1); }
  if( fChain->GetBranchStatus("jet_tagSV2") ) { fChain->SetBranchAddress("jet_tagSV2", jet_tagSV2, &b_jet_tagSV2); }
  if( fChain->GetBranchStatus("jet_tagJetProb") ) { fChain->SetBranchAddress("jet_tagJetProb", jet_tagJetProb, &b_jet_tagJetProb); }
  if( fChain->GetBranchStatus("jet_tagIP1D") ) { fChain->SetBranchAddress("jet_tagIP1D", jet_tagIP1D, &b_jet_tagIP1D); }
  if( fChain->GetBranchStatus("jet_tagIP2D") ) { fChain->SetBranchAddress("jet_tagIP2D", jet_tagIP2D, &b_jet_tagIP2D); }
  if( fChain->GetBranchStatus("jet_tagIP3D") ) { fChain->SetBranchAddress("jet_tagIP3D", jet_tagIP3D, &b_jet_tagIP3D); }
  if( fChain->GetBranchStatus("jet_tagJetFitterTag") ) { fChain->SetBranchAddress("jet_tagJetFitterTag", jet_tagJetFitterTag, &b_jet_tagJetFitterTag); }
  if( fChain->GetBranchStatus("jet_tagJetFitterCOMB") ) { fChain->SetBranchAddress("jet_tagJetFitterCOMB", jet_tagJetFitterCOMB, &b_jet_tagJetFitterCOMB); }
  if( fChain->GetBranchStatus("jet_tagJetFitterCOMBNN") ) { fChain->SetBranchAddress("jet_tagJetFitterCOMBNN", jet_tagJetFitterCOMBNN, &b_jet_tagJetFitterCOMBNN); }
  if( fChain->GetBranchStatus("jet_tagIP3DSV1") ) { fChain->SetBranchAddress("jet_tagIP3DSV1", jet_tagIP3DSV1, &b_jet_tagIP3DSV1); }
  if( fChain->GetBranchStatus("jet_tagSoftElectronTag") ) { fChain->SetBranchAddress("jet_tagSoftElectronTag", jet_tagSoftElectronTag, &b_jet_tagSoftElectronTag); }
  if( fChain->GetBranchStatus("jet_tagSoftMuonTag") ) { fChain->SetBranchAddress("jet_tagSoftMuonTag", jet_tagSoftMuonTag, &b_jet_tagSoftMuonTag); }
  if( fChain->GetBranchStatus("jet_vtx") ) { fChain->SetBranchAddress("jet_vtx", jet_vtx, &b_jet_vtx); }
  if( fChain->GetBranchStatus("jet_js_tau1") ) { fChain->SetBranchAddress("jet_js_tau1", jet_js_tau1, &b_jet_js_tau1); }
  if( fChain->GetBranchStatus("jet_js_tau2") ) { fChain->SetBranchAddress("jet_js_tau2", jet_js_tau2, &b_jet_js_tau2); }
  if( fChain->GetBranchStatus("jet_js_tau3") ) { fChain->SetBranchAddress("jet_js_tau3", jet_js_tau3, &b_jet_js_tau3); }
  if( fChain->GetBranchStatus("jet_js_width") ) { fChain->SetBranchAddress("jet_js_width", jet_js_width, &b_jet_js_width); }
  if( fChain->GetBranchStatus("jet_js_qw") ) { fChain->SetBranchAddress("jet_js_qw", jet_js_qw, &b_jet_js_qw); }
  if( fChain->GetBranchStatus("jet_js_split12") ) { fChain->SetBranchAddress("jet_js_split12", jet_js_split12, &b_jet_js_split12); }
  if( fChain->GetBranchStatus("jet_js_split23") ) { fChain->SetBranchAddress("jet_js_split23", jet_js_split23, &b_jet_js_split23); }
  if( fChain->GetBranchStatus("jet_js_split34") ) { fChain->SetBranchAddress("jet_js_split34", jet_js_split34, &b_jet_js_split34); }
  if( fChain->GetBranchStatus("jet_js_zcut12") ) { fChain->SetBranchAddress("jet_js_zcut12", jet_js_zcut12, &b_jet_js_zcut12); }
  if( fChain->GetBranchStatus("jet_js_zcut23") ) { fChain->SetBranchAddress("jet_js_zcut23", jet_js_zcut23, &b_jet_js_zcut23); }
  if( fChain->GetBranchStatus("jet_js_zcut34") ) { fChain->SetBranchAddress("jet_js_zcut34", jet_js_zcut34, &b_jet_js_zcut34); }
  if( fChain->GetBranchStatus("jet_js_statshape0") ) { fChain->SetBranchAddress("jet_js_statshape0", jet_js_statshape0, &b_jet_js_statshape0); }
  if( fChain->GetBranchStatus("jet_js_statshape1") ) { fChain->SetBranchAddress("jet_js_statshape1", jet_js_statshape1, &b_jet_js_statshape1); }
  if( fChain->GetBranchStatus("jet_js_statshape2") ) { fChain->SetBranchAddress("jet_js_statshape2", jet_js_statshape2, &b_jet_js_statshape2); }
  if( fChain->GetBranchStatus("jet_js_statshape5") ) { fChain->SetBranchAddress("jet_js_statshape5", jet_js_statshape5, &b_jet_js_statshape5); }
  if( fChain->GetBranchStatus("jet_js_statshape8") ) { fChain->SetBranchAddress("jet_js_statshape8", jet_js_statshape8, &b_jet_js_statshape8); }
  if( fChain->GetBranchStatus("jet_js_pullmag") ) { fChain->SetBranchAddress("jet_js_pullmag", jet_js_pullmag, &b_jet_js_pullmag); }
  if( fChain->GetBranchStatus("jet_js_pullphi") ) { fChain->SetBranchAddress("jet_js_pullphi", jet_js_pullphi, &b_jet_js_pullphi); }
  if( fChain->GetBranchStatus("jet_js_pullc00") ) { fChain->SetBranchAddress("jet_js_pullc00", jet_js_pullc00, &b_jet_js_pullc00); }
  if( fChain->GetBranchStatus("jet_js_pullc01") ) { fChain->SetBranchAddress("jet_js_pullc01", jet_js_pullc01, &b_jet_js_pullc01); }
  if( fChain->GetBranchStatus("jet_js_pullc10") ) { fChain->SetBranchAddress("jet_js_pullc10", jet_js_pullc10, &b_jet_js_pullc10); }
  if( fChain->GetBranchStatus("jet_js_pullc11") ) { fChain->SetBranchAddress("jet_js_pullc11", jet_js_pullc11, &b_jet_js_pullc11); }
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
  if( fChain->GetBranchStatus("met_plhc") ) { fChain->SetBranchAddress("met_plhc", &met_plhc, &b_met_plhc); }
  if( fChain->GetBranchStatus("met_plhc_phi") ) { fChain->SetBranchAddress("met_plhc_phi", &met_plhc_phi, &b_met_plhc_phi); }
  if( fChain->GetBranchStatus("met_plhc_sumet") ) { fChain->SetBranchAddress("met_plhc_sumet", &met_plhc_sumet, &b_met_plhc_sumet); }
  if( fChain->GetBranchStatus("met_ichep") ) { fChain->SetBranchAddress("met_ichep", &met_ichep, &b_met_ichep); }
  if( fChain->GetBranchStatus("met_ichep_phi") ) { fChain->SetBranchAddress("met_ichep_phi", &met_ichep_phi, &b_met_ichep_phi); }
  if( fChain->GetBranchStatus("met_ichep_sumet") ) { fChain->SetBranchAddress("met_ichep_sumet", &met_ichep_sumet, &b_met_ichep_sumet); }
  if( fChain->GetBranchStatus("met_truth") ) { fChain->SetBranchAddress("met_truth", &met_truth, &b_met_truth); }
  if( fChain->GetBranchStatus("met_truth_phi") ) { fChain->SetBranchAddress("met_truth_phi", &met_truth_phi, &b_met_truth_phi); }
  if( fChain->GetBranchStatus("met_truth_sumet") ) { fChain->SetBranchAddress("met_truth_sumet", &met_truth_sumet, &b_met_truth_sumet); }
  if( fChain->GetBranchStatus("met_track") ) { fChain->SetBranchAddress("met_track", &met_track, &b_met_track); }
  if( fChain->GetBranchStatus("met_track_phi") ) { fChain->SetBranchAddress("met_track_phi", &met_track_phi, &b_met_track_phi); }
  if( fChain->GetBranchStatus("met_track_sumet") ) { fChain->SetBranchAddress("met_track_sumet", &met_track_sumet, &b_met_track_sumet); }
  if( fChain->GetBranchStatus("nmc") ) { fChain->SetBranchAddress("nmc", &nmc, &b_nmc); }
  if( fChain->GetBranchStatus("mc_weight") ) { fChain->SetBranchAddress("mc_weight", &mc_weight, &b_mc_weight); }
  if( fChain->GetBranchStatus("mc_hfor") ) { fChain->SetBranchAddress("mc_hfor", &mc_hfor, &b_mc_hfor); }
  if( fChain->GetBranchStatus("mc_channel") ) { fChain->SetBranchAddress("mc_channel", &mc_channel, &b_mc_channel); }
  if( fChain->GetBranchStatus("mc_pdgid") ) { fChain->SetBranchAddress("mc_pdgid", mc_pdgid, &b_mc_pdgid); }
  if( fChain->GetBranchStatus("mc_m") ) { fChain->SetBranchAddress("mc_m", mc_m, &b_mc_m); }
  if( fChain->GetBranchStatus("mc_e") ) { fChain->SetBranchAddress("mc_e", mc_e, &b_mc_e); }
  if( fChain->GetBranchStatus("mc_pt") ) { fChain->SetBranchAddress("mc_pt", mc_pt, &b_mc_pt); }
  if( fChain->GetBranchStatus("mc_phi") ) { fChain->SetBranchAddress("mc_phi", mc_phi, &b_mc_phi); }
  if( fChain->GetBranchStatus("mc_eta") ) { fChain->SetBranchAddress("mc_eta", mc_eta, &b_mc_eta); }
  if( fChain->GetBranchStatus("mc_parent") ) { fChain->SetBranchAddress("mc_parent", mc_parent, &b_mc_parent); }
  if( fChain->GetBranchStatus("mc_status") ) { fChain->SetBranchAddress("mc_status", mc_status, &b_mc_status); }
  if( fChain->GetBranchStatus("mc_ptype") ) { fChain->SetBranchAddress("mc_ptype", mc_ptype, &b_mc_ptype); }
  if( fChain->GetBranchStatus("mc_porigin") ) { fChain->SetBranchAddress("mc_porigin", mc_porigin, &b_mc_porigin); }
  if( fChain->GetBranchStatus("mc_poutcome") ) { fChain->SetBranchAddress("mc_poutcome", mc_poutcome, &b_mc_poutcome); }
  if( fChain->GetBranchStatus("mc_pmotherpdg") ) { fChain->SetBranchAddress("mc_pmotherpdg", mc_pmotherpdg, &b_mc_pmotherpdg); }
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
  if( fChain->GetBranchStatus("njpsi") ) { fChain->SetBranchAddress("njpsi", &njpsi, &b_njpsi); }
  if( fChain->GetBranchStatus("jpsi_mass") ) { fChain->SetBranchAddress("jpsi_mass", jpsi_mass, &b_jpsi_mass); }
  if( fChain->GetBranchStatus("jpsi_masserr") ) { fChain->SetBranchAddress("jpsi_masserr", jpsi_masserr, &b_jpsi_masserr); }
  if( fChain->GetBranchStatus("jpsi_phi") ) { fChain->SetBranchAddress("jpsi_phi", jpsi_phi, &b_jpsi_phi); }
  if( fChain->GetBranchStatus("jpsi_pt") ) { fChain->SetBranchAddress("jpsi_pt", jpsi_pt, &b_jpsi_pt); }
  if( fChain->GetBranchStatus("jpsi_y") ) { fChain->SetBranchAddress("jpsi_y", jpsi_y, &b_jpsi_y); }
  if( fChain->GetBranchStatus("jpsi_chisq") ) { fChain->SetBranchAddress("jpsi_chisq", jpsi_chisq, &b_jpsi_chisq); }
  if( fChain->GetBranchStatus("jpsi_tau") ) { fChain->SetBranchAddress("jpsi_tau", jpsi_tau, &b_jpsi_tau); }
  if( fChain->GetBranchStatus("jpsi_tauerr") ) { fChain->SetBranchAddress("jpsi_tauerr", jpsi_tauerr, &b_jpsi_tauerr); }
  if( fChain->GetBranchStatus("jpsi_costhetastar") ) { fChain->SetBranchAddress("jpsi_costhetastar", jpsi_costhetastar, &b_jpsi_costhetastar); }
  if( fChain->GetBranchStatus("jpsi_is_os") ) { fChain->SetBranchAddress("jpsi_is_os", jpsi_is_os, &b_jpsi_is_os); }
  if( fChain->GetBranchStatus("jpsi_mup") ) { fChain->SetBranchAddress("jpsi_mup", jpsi_mup, &b_jpsi_mup); }
  if( fChain->GetBranchStatus("jpsi_mum") ) { fChain->SetBranchAddress("jpsi_mum", jpsi_mum, &b_jpsi_mum); }

  // EXPERIMENTAL: enable TTree cache:
  Int_t cachesize = 100000000;   //100 MBytes
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
