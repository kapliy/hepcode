//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Mar  7 23:02:49 2012 by ROOT version 5.28/00e
// from TTree tree/Muon tree
// found on file: /atlas/uct3/data/users/antonk/NTUPLE/v1_29d/user.kapliy.UCNTUP.mc11_7TeV.106047.PythiaZmumu_no_filter.merge.AOD.e815_s1272_s1274_r3043_r2993.j.mc11c.v1_29d.120218115907/user.kapliy.008483.flatntuple._00001.root
//////////////////////////////////////////////////////////

#ifndef Deleteme_h
#define Deleteme_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
   const Int_t kMaxmu_trig_l1matchvector = 316;
   const Int_t kMaxmu_trig_l2matchvector = 738;
   const Int_t kMaxmu_trig_l2samatchvector = 795;
   const Int_t kMaxmu_trig_efmatchvector = 684;
   const Int_t kMaxele_trig_l1matchvector = 359;
   const Int_t kMaxele_trig_l2matchvector = 237;
   const Int_t kMaxele_trig_efmatchvector = 170;

class Deleteme {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   UInt_t          run;
   UInt_t          event;
   UInt_t          lb;
   UInt_t          bcid;
   Float_t         lumi_mu_actual;
   Float_t         lumi_mu_average;
   Int_t           bdff;
   Int_t           bfif600;
   Float_t         bint;
   UInt_t          error_lar;
   UInt_t          error_background;
   vector<string>  *trig_ef;
   vector<string>  *trig_l2;
   vector<string>  *trig_l1;
   Float_t         bs_x;
   Float_t         bs_y;
   Float_t         bs_z;
   Float_t         bs_status;
   Int_t           nvx;
   Int_t           vx_type[21];   //[nvx]
   Int_t           vx_ntracks[21];   //[nvx]
   Float_t         vx_sumpt[21];   //[nvx]
   Float_t         vx_x[21];   //[nvx]
   Float_t         vx_y[21];   //[nvx]
   Float_t         vx_z[21];   //[nvx]
   Int_t           nmu;
   Short_t         mu_type[14];   //[nmu]
   Float_t         mu_pt[14];   //[nmu]
   Float_t         mu_eta[14];   //[nmu]
   Float_t         mu_phi[14];   //[nmu]
   Int_t           mu_q[14];   //[nmu]
   Float_t         mu_ptcid[14];   //[nmu]
   Float_t         mu_etacid[14];   //[nmu]
   Float_t         mu_phicid[14];   //[nmu]
   Int_t           mu_qcid[14];   //[nmu]
   Float_t         mu_ptid[14];   //[nmu]
   Float_t         mu_etaid[14];   //[nmu]
   Float_t         mu_phiid[14];   //[nmu]
   Int_t           mu_qid[14];   //[nmu]
   Float_t         mu_ptexid[14];   //[nmu]
   Float_t         mu_etaexid[14];   //[nmu]
   Float_t         mu_phiexid[14];   //[nmu]
   Int_t           mu_qexid[14];   //[nmu]
   Float_t         mu_ptms[14];   //[nmu]
   Float_t         mu_etams[14];   //[nmu]
   Float_t         mu_phims[14];   //[nmu]
   Int_t           mu_qms[14];   //[nmu]
   Float_t         mu_ptexms[14];   //[nmu]
   Float_t         mu_etaexms[14];   //[nmu]
   Float_t         mu_phiexms[14];   //[nmu]
   Int_t           mu_qexms[14];   //[nmu]
   UShort_t        mu_author[14];   //[nmu]
   UShort_t        mu_primauthor[14];   //[nmu]
   Short_t         mu_class[14];   //[nmu]
   Short_t         mu_iscombined[14];   //[nmu]
   Short_t         mu_issegtagged[14];   //[nmu]
   Float_t         mu_fitchi2prob[14];   //[nmu]
   Float_t         mu_matchchi2prob[14];   //[nmu]
   Float_t         mu_fitchi2[14];   //[nmu]
   Float_t         mu_matchchi2[14];   //[nmu]
   Int_t           mu_bestmatch[14];   //[nmu]
   Int_t           mu_nucone10[14];   //[nmu]
   Int_t           mu_nucone20[14];   //[nmu]
   Int_t           mu_nucone30[14];   //[nmu]
   Int_t           mu_nucone40[14];   //[nmu]
   Float_t         mu_etcone10[14];   //[nmu]
   Float_t         mu_etcone20[14];   //[nmu]
   Float_t         mu_etcone30[14];   //[nmu]
   Float_t         mu_etcone40[14];   //[nmu]
   Float_t         mu_ptcone10[14];   //[nmu]
   Float_t         mu_ptcone20[14];   //[nmu]
   Float_t         mu_ptcone30[14];   //[nmu]
   Float_t         mu_ptcone40[14];   //[nmu]
   Float_t         mu_ptconemod10[14];   //[nmu]
   Float_t         mu_ptconemod20[14];   //[nmu]
   Float_t         mu_ptconemod30[14];   //[nmu]
   Float_t         mu_ptconemod40[14];   //[nmu]
   Float_t         mu_eloss[14];   //[nmu]
   Float_t         mu_d0[14];   //[nmu]
   Float_t         mu_z0[14];   //[nmu]
   Float_t         mu_db[14];   //[nmu]
   Float_t         mu_zb[14];   //[nmu]
   Float_t         mu_dvx[14];   //[nmu]
   Float_t         mu_zvx[14];   //[nmu]
   Float_t         mu_dxpv[14];   //[nmu]
   Float_t         mu_zxpv[14];   //[nmu]
   Float_t         mu_covdb[14];   //[nmu]
   Float_t         mu_covdvx[14];   //[nmu]
   Float_t         mu_covdxpv[14];   //[nmu]
   Int_t           mu_vtxtype[14];   //[nmu]
   Int_t           mu_vtx[14];   //[nmu]
   Int_t           mu_nhitbl[14];   //[nmu]
   Short_t         mu_expectbl[14];   //[nmu]
   Int_t           mu_nhitpix[14];   //[nmu]
   Int_t           mu_nholespix[14];   //[nmu]
   Int_t           mu_ndeadsensorspix[14];   //[nmu]
   Int_t           mu_nhitsct[14];   //[nmu]
   Int_t           mu_nholessct[14];   //[nmu]
   Int_t           mu_ndeadsensorssct[14];   //[nmu]
   Int_t           mu_nhittrt[14];   //[nmu]
   Int_t           mu_nholestrt[14];   //[nmu]
   Int_t           mu_nhitmdt[14];   //[nmu]
   Int_t           mu_nhitcsceta[14];   //[nmu]
   Int_t           mu_nhitcscphi[14];   //[nmu]
   Int_t           mu_nhitrpceta[14];   //[nmu]
   Int_t           mu_nhitrpcphi[14];   //[nmu]
   Int_t           mu_nhittgceta[14];   //[nmu]
   Int_t           mu_nhittgcphi[14];   //[nmu]
   Int_t           mu_noutlierstrt[14];   //[nmu]
   Int_t           mu_nhitcscunspoilt[14];   //[nmu]
   Int_t           mu_nMDTBIHits[14];   //[nmu]
   Int_t           mu_nMDTBMHits[14];   //[nmu]
   Int_t           mu_nMDTBOHits[14];   //[nmu]
   Int_t           mu_nMDTBEEHits[14];   //[nmu]
   Int_t           mu_nMDTBIS78Hits[14];   //[nmu]
   Int_t           mu_nMDTEIHits[14];   //[nmu]
   Int_t           mu_nMDTEMHits[14];   //[nmu]
   Int_t           mu_nMDTEOHits[14];   //[nmu]
   Int_t           mu_nMDTEEHits[14];   //[nmu]
   UShort_t        mu_quality[14];   //[nmu]
   Float_t         mu_scatt_curv_sig[14];   //[nmu]
   Float_t         mu_scatt_neighb_sig[14];   //[nmu]
   Float_t         mu_field_integral[14];   //[nmu]
   Int_t           mu_trig_l1matchvector_;
   Int_t           mu_trig_l1matchvector_first[kMaxmu_trig_l1matchvector];   //[mu_trig_l1matchvector_]
   Int_t           mu_trig_l1matchvector_second[kMaxmu_trig_l1matchvector];   //[mu_trig_l1matchvector_]
   vector<float>   *mu_trig_l1matchdelr;
   Int_t           mu_trig_l2matchvector_;
   Int_t           mu_trig_l2matchvector_first[kMaxmu_trig_l2matchvector];   //[mu_trig_l2matchvector_]
   Int_t           mu_trig_l2matchvector_second[kMaxmu_trig_l2matchvector];   //[mu_trig_l2matchvector_]
   vector<float>   *mu_trig_l2matchdelr;
   Int_t           mu_trig_l2samatchvector_;
   Int_t           mu_trig_l2samatchvector_first[kMaxmu_trig_l2samatchvector];   //[mu_trig_l2samatchvector_]
   Int_t           mu_trig_l2samatchvector_second[kMaxmu_trig_l2samatchvector];   //[mu_trig_l2samatchvector_]
   vector<float>   *mu_trig_l2samatchdelr;
   Int_t           mu_trig_efmatchvector_;
   Int_t           mu_trig_efmatchvector_first[kMaxmu_trig_efmatchvector];   //[mu_trig_efmatchvector_]
   Int_t           mu_trig_efmatchvector_second[kMaxmu_trig_efmatchvector];   //[mu_trig_efmatchvector_]
   vector<float>   *mu_trig_efmatchdelr;
   Int_t           mu_passes_mcp[14];   //[nmu]
   Float_t         mu_MET_wet[14];   //[nmu]
   Float_t         mu_MET_wpx[14];   //[nmu]
   Float_t         mu_MET_wpy[14];   //[nmu]
   Float_t         mu_MET_status[14];   //[nmu]
   Float_t         mu_MET_wet2[14];   //[nmu]
   Float_t         mu_MET_wpx2[14];   //[nmu]
   Float_t         mu_MET_wpy2[14];   //[nmu]
   Float_t         mu_MET_status2[14];   //[nmu]
   Int_t           mu_ptype[14];   //[nmu]
   Int_t           mu_porigin[14];   //[nmu]
   Int_t           mu_poutcome[14];   //[nmu]
   Int_t           nele;
   UShort_t        ele_author[25];   //[nele]
   Float_t         ele_eta[25];   //[nele]
   Float_t         ele_phi[25];   //[nele]
   Float_t         ele_et[25];   //[nele]
   Int_t           ele_q[25];   //[nele]
   Float_t         ele_etaclus[25];   //[nele]
   Float_t         ele_phiclus[25];   //[nele]
   Float_t         ele_etclus[25];   //[nele]
   Float_t         ele_etatrk[25];   //[nele]
   Float_t         ele_phitrk[25];   //[nele]
   Float_t         ele_pttrk[25];   //[nele]
   Float_t         ele_etabest[25];   //[nele]
   Float_t         ele_phibest[25];   //[nele]
   Float_t         ele_etbest[25];   //[nele]
   Float_t         ele_etap[25];   //[nele]
   Float_t         ele_f1[25];   //[nele]
   UInt_t          ele_isEM[25];   //[nele]
   Int_t           ele_nucone20[25];   //[nele]
   Int_t           ele_nucone30[25];   //[nele]
   Int_t           ele_nucone40[25];   //[nele]
   Float_t         ele_etcone20[25];   //[nele]
   Float_t         ele_etcone30[25];   //[nele]
   Float_t         ele_etcone40[25];   //[nele]
   Float_t         ele_etcone20_cor[25];   //[nele]
   Float_t         ele_etcone30_cor[25];   //[nele]
   Float_t         ele_etcone40_cor[25];   //[nele]
   Float_t         ele_ptcone20[25];   //[nele]
   Float_t         ele_ptcone30[25];   //[nele]
   Float_t         ele_ptcone40[25];   //[nele]
   Float_t         ele_d0[25];   //[nele]
   Float_t         ele_z0[25];   //[nele]
   Float_t         ele_db[25];   //[nele]
   Float_t         ele_zb[25];   //[nele]
   Float_t         ele_dvx[25];   //[nele]
   Float_t         ele_zvx[25];   //[nele]
   Float_t         ele_dxpv[25];   //[nele]
   Float_t         ele_zxpv[25];   //[nele]
   Float_t         ele_covdb[25];   //[nele]
   Float_t         ele_covdvx[25];   //[nele]
   Float_t         ele_covdxpv[25];   //[nele]
   Int_t           ele_vtx[25];   //[nele]
   Int_t           ele_nhitpix[25];   //[nele]
   Int_t           ele_nhitsct[25];   //[nele]
   Int_t           ele_nhittrt[25];   //[nele]
   UShort_t        ele_quality[25];   //[nele]
   UInt_t          ele_isgoodoq[25];   //[nele]
   UInt_t          ele_oq[25];   //[nele]
   Float_t         ele_rhad[25];   //[nele]
   Float_t         ele_rhad1[25];   //[nele]
   Float_t         ele_reta[25];   //[nele]
   Float_t         ele_rphi[25];   //[nele]
   Float_t         ele_w2[25];   //[nele]
   Float_t         ele_wstot[25];   //[nele]
   Float_t         ele_demaxs1[25];   //[nele]
   Float_t         ele_deltaeta[25];   //[nele]
   Float_t         ele_deltaphi[25];   //[nele]
   Float_t         ele_trratio[25];   //[nele]
   Short_t         ele_nhitbl[25];   //[nele]
   Short_t         ele_noutliersbl[25];   //[nele]
   Short_t         ele_expectbl[25];   //[nele]
   Short_t         ele_noutlierspix[25];   //[nele]
   Short_t         ele_noutlierssct[25];   //[nele]
   Float_t         ele_eta2[25];   //[nele]
   Float_t         ele_et2[25];   //[nele]
   Int_t           ele_trig_l1matchvector_;
   Int_t           ele_trig_l1matchvector_first[kMaxele_trig_l1matchvector];   //[ele_trig_l1matchvector_]
   Int_t           ele_trig_l1matchvector_second[kMaxele_trig_l1matchvector];   //[ele_trig_l1matchvector_]
   vector<float>   *ele_trig_l1matchdelr;
   Int_t           ele_trig_l2matchvector_;
   Int_t           ele_trig_l2matchvector_first[kMaxele_trig_l2matchvector];   //[ele_trig_l2matchvector_]
   Int_t           ele_trig_l2matchvector_second[kMaxele_trig_l2matchvector];   //[ele_trig_l2matchvector_]
   vector<float>   *ele_trig_l2matchdelr;
   Int_t           ele_trig_efmatchvector_;
   Int_t           ele_trig_efmatchvector_first[kMaxele_trig_efmatchvector];   //[ele_trig_efmatchvector_]
   Int_t           ele_trig_efmatchvector_second[kMaxele_trig_efmatchvector];   //[ele_trig_efmatchvector_]
   vector<float>   *ele_trig_efmatchdelr;
   Float_t         ele_MET_wet[25];   //[nele]
   Float_t         ele_MET_wpx[25];   //[nele]
   Float_t         ele_MET_wpy[25];   //[nele]
   Float_t         ele_MET_status[25];   //[nele]
   Int_t           ele_ptype[25];   //[nele]
   Int_t           ele_porigin[25];   //[nele]
   Int_t           ele_poutcome[25];   //[nele]
   Int_t           nph;
   UShort_t        ph_author[94];   //[nph]
   Float_t         ph_eta[94];   //[nph]
   Float_t         ph_phi[94];   //[nph]
   Float_t         ph_etaclus[94];   //[nph]
   Float_t         ph_phiclus[94];   //[nph]
   Float_t         ph_et[94];   //[nph]
   Float_t         ph_eclus[94];   //[nph]
   UInt_t          ph_isEM[94];   //[nph]
   UInt_t          ph_isgoodoq[94];   //[nph]
   UInt_t          ph_oq[94];   //[nph]
   UShort_t        ph_quality[94];   //[nph]
   Float_t         ph_etcone20[94];   //[nph]
   Float_t         ph_etcone30[94];   //[nph]
   Float_t         ph_etcone40[94];   //[nph]
   Float_t         ph_etcone20_cor[94];   //[nph]
   Float_t         ph_etcone30_cor[94];   //[nph]
   Float_t         ph_etcone40_cor[94];   //[nph]
   Float_t         ph_ptcone20[94];   //[nph]
   Float_t         ph_ptcone30[94];   //[nph]
   Float_t         ph_ptcone40[94];   //[nph]
   Int_t           ph_isRecovered[94];   //[nph]
   Int_t           ph_convFlag[94];   //[nph]
   Float_t         ph_rconv[94];   //[nph]
   Float_t         ph_zconv[94];   //[nph]
   Float_t         ph_etas2[94];   //[nph]
   Float_t         ph_ethad1[94];   //[nph]
   Float_t         ph_ethad[94];   //[nph]
   Float_t         ph_e277[94];   //[nph]
   Float_t         ph_e237[94];   //[nph]
   Float_t         ph_e233[94];   //[nph]
   Float_t         ph_weta2[94];   //[nph]
   Float_t         ph_f1[94];   //[nph]
   Float_t         ph_emax[94];   //[nph]
   Float_t         ph_emaxs2[94];   //[nph]
   Float_t         ph_demaxs1[94];   //[nph]
   Float_t         ph_fside[94];   //[nph]
   Float_t         ph_wstot[94];   //[nph]
   Float_t         ph_ws3[94];   //[nph]
   Float_t         ph_emins1[94];   //[nph]
   Float_t         ph_etap[94];   //[nph]
   Float_t         ph_deltaeta[94];   //[nph]
   Float_t         ph_MET_wet[94];   //[nph]
   Float_t         ph_MET_wpx[94];   //[nph]
   Float_t         ph_MET_wpy[94];   //[nph]
   Float_t         ph_MET_status[94];   //[nph]
   Int_t           ph_ptype[94];   //[nph]
   Int_t           ph_porigin[94];   //[nph]
   Int_t           ph_poutcome[94];   //[nph]
   Int_t           ntrk;
   Float_t         trk_pt[15];   //[ntrk]
   Float_t         trk_eta[15];   //[ntrk]
   Float_t         trk_phi[15];   //[ntrk]
   Float_t         trk_m[15];   //[ntrk]
   Int_t           trk_q[15];   //[ntrk]
   Int_t           trk_mu[15];   //[ntrk]
   Int_t           trk_nucone10[15];   //[ntrk]
   Int_t           trk_nucone20[15];   //[ntrk]
   Int_t           trk_nucone30[15];   //[ntrk]
   Int_t           trk_nucone40[15];   //[ntrk]
   Float_t         trk_ptcone10[15];   //[ntrk]
   Float_t         trk_ptcone20[15];   //[ntrk]
   Float_t         trk_ptcone30[15];   //[ntrk]
   Float_t         trk_ptcone40[15];   //[ntrk]
   Int_t           trk_vtx[15];   //[ntrk]
   Float_t         trk_dxpv[15];   //[ntrk]
   Float_t         trk_zxpv[15];   //[ntrk]
   Float_t         trk_covdxpv[15];   //[ntrk]
   Int_t           trk_nhitbl[15];   //[ntrk]
   Short_t         trk_expectbl[15];   //[ntrk]
   Int_t           trk_nhitpix[15];   //[ntrk]
   Int_t           trk_nholespix[15];   //[ntrk]
   Int_t           trk_ndeadsensorspix[15];   //[ntrk]
   Int_t           trk_nhitsct[15];   //[ntrk]
   Int_t           trk_nholessct[15];   //[ntrk]
   Int_t           trk_ndeadsensorssct[15];   //[ntrk]
   Int_t           trk_nhittrt[15];   //[ntrk]
   Int_t           trk_nholestrt[15];   //[ntrk]
   Int_t           trk_noutlierstrt[15];   //[ntrk]
   Int_t           trk_ptype[15];   //[ntrk]
   Int_t           trk_porigin[15];   //[ntrk]
   Int_t           trk_poutcome[15];   //[ntrk]
   Float_t         trk_pmatchprob[15];   //[ntrk]
   Int_t           njet;
   Int_t           jet_type[172];   //[njet]
   Float_t         jet_et[172];   //[njet]
   Float_t         jet_pt[172];   //[njet]
   Float_t         jet_eta[172];   //[njet]
   Float_t         jet_phi[172];   //[njet]
   Float_t         jet_m[172];   //[njet]
   Float_t         jet_pt_em[172];   //[njet]
   Float_t         jet_eta_em[172];   //[njet]
   Float_t         jet_phi_em[172];   //[njet]
   Float_t         jet_m_em[172];   //[njet]
   Float_t         jet_eta_origin[172];   //[njet]
   Float_t         jet_phi_origin[172];   //[njet]
   Float_t         jet_m_origin[172];   //[njet]
   Float_t         jet_eta_origin_em[172];   //[njet]
   Float_t         jet_phi_origin_em[172];   //[njet]
   Float_t         jet_m_origin_em[172];   //[njet]
   Float_t         jet_gcwjes[172];   //[njet]
   Float_t         jet_emjes[172];   //[njet]
   Int_t           jet_isgood[172];   //[njet]
   Int_t           jet_isbad[172];   //[njet]
   Int_t           jet_isugly[172];   //[njet]
   Int_t           jet_n90[172];   //[njet]
   Float_t         jet_hecquality[172];   //[njet]
   Float_t         jet_larquality[172];   //[njet]
   Float_t         jet_averagelarqf[172];   //[njet]
   Float_t         jet_negativee[172];   //[njet]
   Float_t         jet_sumpttrk[172];   //[njet]
   Int_t           jet_ntrk[172];   //[njet]
   Float_t         jet_jvtxf[172];   //[njet]
   Float_t         jet_jvf[172];   //[njet]
   Float_t         jet_bchcorr[172];   //[njet]
   Float_t         jet_bchcorrjet[172];   //[njet]
   Float_t         jet_tgap3f[172];   //[njet]
   Float_t         jet_time[172];   //[njet]
   Float_t         jet_emf[172];   //[njet]
   Float_t         jet_hecf[172];   //[njet]
   Float_t         jet_fmax[172];   //[njet]
   Float_t         jet_tagSV0[172];   //[njet]
   Float_t         jet_tagSV1[172];   //[njet]
   Float_t         jet_tagSV2[172];   //[njet]
   Float_t         jet_tagJetProb[172];   //[njet]
   Float_t         jet_tagIP1D[172];   //[njet]
   Float_t         jet_tagIP2D[172];   //[njet]
   Float_t         jet_tagIP3D[172];   //[njet]
   Float_t         jet_tagJetFitterTag[172];   //[njet]
   Float_t         jet_tagJetFitterCOMB[172];   //[njet]
   Float_t         jet_tagJetFitterCOMBNN[172];   //[njet]
   Float_t         jet_tagIP3DSV1[172];   //[njet]
   Float_t         jet_tagSoftElectronTag[172];   //[njet]
   Float_t         jet_tagSoftMuonTag[172];   //[njet]
   Int_t           jet_vtx[172];   //[njet]
   Float_t         jet_js_tau1[172];   //[njet]
   Float_t         jet_js_tau2[172];   //[njet]
   Float_t         jet_js_tau3[172];   //[njet]
   Float_t         jet_js_width[172];   //[njet]
   Float_t         jet_js_qw[172];   //[njet]
   Float_t         jet_js_split12[172];   //[njet]
   Float_t         jet_js_split23[172];   //[njet]
   Float_t         jet_js_split34[172];   //[njet]
   Float_t         jet_js_zcut12[172];   //[njet]
   Float_t         jet_js_zcut23[172];   //[njet]
   Float_t         jet_js_zcut34[172];   //[njet]
   Float_t         jet_js_statshape0[172];   //[njet]
   Float_t         jet_js_statshape1[172];   //[njet]
   Float_t         jet_js_statshape2[172];   //[njet]
   Float_t         jet_js_statshape5[172];   //[njet]
   Float_t         jet_js_statshape8[172];   //[njet]
   Float_t         jet_js_pullmag[172];   //[njet]
   Float_t         jet_js_pullphi[172];   //[njet]
   Float_t         jet_js_pullc00[172];   //[njet]
   Float_t         jet_js_pullc01[172];   //[njet]
   Float_t         jet_js_pullc10[172];   //[njet]
   Float_t         jet_js_pullc11[172];   //[njet]
   Float_t         jet_MET_wet[172];   //[njet]
   Float_t         jet_MET_wpx[172];   //[njet]
   Float_t         jet_MET_wpy[172];   //[njet]
   Float_t         jet_MET_status[172];   //[njet]
   Float_t         met_reffinal;
   Float_t         met_reffinal_phi;
   Float_t         met_reffinal_sumet;
   Float_t         met_reffinal_em;
   Float_t         met_reffinal_em_phi;
   Float_t         met_reffinal_em_sumet;
   Float_t         met_refjet;
   Float_t         met_refjet_phi;
   Float_t         met_refjet_sumet;
   Float_t         met_cellout;
   Float_t         met_cellout_phi;
   Float_t         met_cellout_sumet;
   Float_t         met_softjets;
   Float_t         met_softjets_phi;
   Float_t         met_softjets_sumet;
   Float_t         met_cellout_eflow;
   Float_t         met_cellout_eflow_phi;
   Float_t         met_cellout_eflow_sumet;
   Float_t         met_cellout_eflow_muid;
   Float_t         met_cellout_eflow_muid_phi;
   Float_t         met_cellout_eflow_muid_sumet;
   Float_t         met_cryo;
   Float_t         met_cryo_phi;
   Float_t         met_cryo_sumet;
   Float_t         met_refmuon;
   Float_t         met_refmuon_phi;
   Float_t         met_refmuon_sumet;
   Float_t         met_refmuon_track;
   Float_t         met_refmuon_track_phi;
   Float_t         met_refmuon_track_sumet;
   Float_t         met_muonboy;
   Float_t         met_muonboy_phi;
   Float_t         met_muonboy_sumet;
   Float_t         met_muid;
   Float_t         met_muid_phi;
   Float_t         met_muid_sumet;
   Float_t         met_topo;
   Float_t         met_topo_phi;
   Float_t         met_topo_sumet;
   Float_t         met_topo_eta45;
   Float_t         met_topo_eta45_phi;
   Float_t         met_topo_eta45_sumet;
   Float_t         met_lochadtopo;
   Float_t         met_lochadtopo_phi;
   Float_t         met_lochadtopo_sumet;
   Float_t         met_lochadtopo_eta45;
   Float_t         met_lochadtopo_eta45_phi;
   Float_t         met_lochadtopo_eta45_sumet;
   Float_t         met_reffinal_eta45;
   Float_t         met_reffinal_eta45_phi;
   Float_t         met_reffinal_eta45_sumet;
   Float_t         met_muon;
   Float_t         met_muon_phi;
   Float_t         met_muon_sumet;
   Float_t         met_muon_muid;
   Float_t         met_muon_muid_phi;
   Float_t         met_muon_muid_sumet;
   Float_t         met_refele;
   Float_t         met_refele_phi;
   Float_t         met_refele_sumet;
   Float_t         met_reftau;
   Float_t         met_reftau_phi;
   Float_t         met_reftau_sumet;
   Float_t         met_refgamma;
   Float_t         met_refgamma_phi;
   Float_t         met_refgamma_sumet;
   Float_t         met_plhc;
   Float_t         met_plhc_phi;
   Float_t         met_plhc_sumet;
   Float_t         met_ichep;
   Float_t         met_ichep_phi;
   Float_t         met_ichep_sumet;
   Float_t         met_truth;
   Float_t         met_truth_phi;
   Float_t         met_truth_sumet;
   Float_t         met_track;
   Float_t         met_track_phi;
   Float_t         met_track_sumet;
   Int_t           nmc;
   Float_t         mc_weight;
   Int_t           mc_hfor;
   Int_t           mc_channel;
   Int_t           mc_pdgid[928];   //[nmc]
   Float_t         mc_m[928];   //[nmc]
   Float_t         mc_e[928];   //[nmc]
   Float_t         mc_pt[928];   //[nmc]
   Float_t         mc_phi[928];   //[nmc]
   Float_t         mc_eta[928];   //[nmc]
   Int_t           mc_parent[928];   //[nmc]
   Int_t           mc_status[928];   //[nmc]
   Int_t           mc_ptype[928];   //[nmc]
   Int_t           mc_porigin[928];   //[nmc]
   Int_t           mc_poutcome[928];   //[nmc]
   Int_t           mc_pmotherpdg[928];   //[nmc]
   Int_t           mcevt_hardindex;
   Int_t           mcevt_pdfset;
   Float_t         mcevt_weight;
   Int_t           mcevt_sigid;
   Float_t         mcevt_event_scale;
   Float_t         mcevt_pdf_scale;
   Float_t         mcevt_alphaQCD;
   Float_t         mcevt_alphaQED;
   Int_t           mcevt_id1;
   Int_t           mcevt_id2;
   Float_t         mcevt_pdf_x1;
   Float_t         mcevt_pdf_x2;
   Float_t         mcevt_pdf1;
   Float_t         mcevt_pdf2;
   Int_t           njpsi;
   Float_t         jpsi_mass[1];   //[njpsi]
   Float_t         jpsi_masserr[1];   //[njpsi]
   Float_t         jpsi_phi[1];   //[njpsi]
   Float_t         jpsi_pt[1];   //[njpsi]
   Float_t         jpsi_y[1];   //[njpsi]
   Float_t         jpsi_chisq[1];   //[njpsi]
   Float_t         jpsi_tau[1];   //[njpsi]
   Float_t         jpsi_tauerr[1];   //[njpsi]
   Float_t         jpsi_costhetastar[1];   //[njpsi]
   Int_t           jpsi_is_os[1];   //[njpsi]
   Int_t           jpsi_mup[1];   //[njpsi]
   Int_t           jpsi_mum[1];   //[njpsi]
   Float_t         lha_cteq6ll;
   Float_t         lha_MRSTMCal;
   Float_t         lha_CT10;
   Float_t         lha_MSTW2008lo68cl;
   Float_t         lha_MSTW2008nlo68cl;
   Float_t         lha_NNPDF21_100;
   Float_t         lha_HERAPDF10_EIG;
   Float_t         lha_cteq66;

   // List of branches
   TBranch        *b_run;   //!
   TBranch        *b_event;   //!
   TBranch        *b_lb;   //!
   TBranch        *b_bcid;   //!
   TBranch        *b_lumi_mu_actual;   //!
   TBranch        *b_lumi_mu_average;   //!
   TBranch        *b_bdff;   //!
   TBranch        *b_bfif600;   //!
   TBranch        *b_bint;   //!
   TBranch        *b_error_lar;   //!
   TBranch        *b_error_background;   //!
   TBranch        *b_trig_ef;   //!
   TBranch        *b_trig_l2;   //!
   TBranch        *b_trig_l1;   //!
   TBranch        *b_bs_x;   //!
   TBranch        *b_bs_y;   //!
   TBranch        *b_bs_z;   //!
   TBranch        *b_bs_status;   //!
   TBranch        *b_nvx;   //!
   TBranch        *b_vx_type;   //!
   TBranch        *b_vx_ntracks;   //!
   TBranch        *b_vx_sumpt;   //!
   TBranch        *b_vx_x;   //!
   TBranch        *b_vx_y;   //!
   TBranch        *b_vx_z;   //!
   TBranch        *b_nmu;   //!
   TBranch        *b_mu_type;   //!
   TBranch        *b_mu_pt;   //!
   TBranch        *b_mu_eta;   //!
   TBranch        *b_mu_phi;   //!
   TBranch        *b_mu_q;   //!
   TBranch        *b_mu_ptcid;   //!
   TBranch        *b_mu_etacid;   //!
   TBranch        *b_mu_phicid;   //!
   TBranch        *b_mu_qcid;   //!
   TBranch        *b_mu_ptid;   //!
   TBranch        *b_mu_etaid;   //!
   TBranch        *b_mu_phiid;   //!
   TBranch        *b_mu_qid;   //!
   TBranch        *b_mu_ptexid;   //!
   TBranch        *b_mu_etaexid;   //!
   TBranch        *b_mu_phiexid;   //!
   TBranch        *b_mu_qexid;   //!
   TBranch        *b_mu_ptms;   //!
   TBranch        *b_mu_etams;   //!
   TBranch        *b_mu_phims;   //!
   TBranch        *b_mu_qms;   //!
   TBranch        *b_mu_ptexms;   //!
   TBranch        *b_mu_etaexms;   //!
   TBranch        *b_mu_phiexms;   //!
   TBranch        *b_mu_qexms;   //!
   TBranch        *b_mu_author;   //!
   TBranch        *b_mu_primauthor;   //!
   TBranch        *b_mu_class;   //!
   TBranch        *b_mu_iscombined;   //!
   TBranch        *b_mu_issegtagged;   //!
   TBranch        *b_mu_fitchi2prob;   //!
   TBranch        *b_mu_matchchi2prob;   //!
   TBranch        *b_mu_fitchi2;   //!
   TBranch        *b_mu_matchchi2;   //!
   TBranch        *b_mu_bestmatch;   //!
   TBranch        *b_mu_nucone10;   //!
   TBranch        *b_mu_nucone20;   //!
   TBranch        *b_mu_nucone30;   //!
   TBranch        *b_mu_nucone40;   //!
   TBranch        *b_mu_etcone10;   //!
   TBranch        *b_mu_etcone20;   //!
   TBranch        *b_mu_etcone30;   //!
   TBranch        *b_mu_etcone40;   //!
   TBranch        *b_mu_ptcone10;   //!
   TBranch        *b_mu_ptcone20;   //!
   TBranch        *b_mu_ptcone30;   //!
   TBranch        *b_mu_ptcone40;   //!
   TBranch        *b_mu_ptconemod10;   //!
   TBranch        *b_mu_ptconemod20;   //!
   TBranch        *b_mu_ptconemod30;   //!
   TBranch        *b_mu_ptconemod40;   //!
   TBranch        *b_mu_eloss;   //!
   TBranch        *b_mu_d0;   //!
   TBranch        *b_mu_z0;   //!
   TBranch        *b_mu_db;   //!
   TBranch        *b_mu_zb;   //!
   TBranch        *b_mu_dvx;   //!
   TBranch        *b_mu_zvx;   //!
   TBranch        *b_mu_dxpv;   //!
   TBranch        *b_mu_zxpv;   //!
   TBranch        *b_mu_covdb;   //!
   TBranch        *b_mu_covdvx;   //!
   TBranch        *b_mu_covdxpv;   //!
   TBranch        *b_mu_vtxtype;   //!
   TBranch        *b_mu_vtx;   //!
   TBranch        *b_mu_nhitbl;   //!
   TBranch        *b_mu_expectbl;   //!
   TBranch        *b_mu_nhitpix;   //!
   TBranch        *b_mu_nholespix;   //!
   TBranch        *b_mu_ndeadsensorspix;   //!
   TBranch        *b_mu_nhitsct;   //!
   TBranch        *b_mu_nholessct;   //!
   TBranch        *b_mu_ndeadsensorssct;   //!
   TBranch        *b_mu_nhittrt;   //!
   TBranch        *b_mu_nholestrt;   //!
   TBranch        *b_mu_nhitmdt;   //!
   TBranch        *b_mu_nhitcsceta;   //!
   TBranch        *b_mu_nhitcscphi;   //!
   TBranch        *b_mu_nhitrpceta;   //!
   TBranch        *b_mu_nhitrpcphi;   //!
   TBranch        *b_mu_nhittgceta;   //!
   TBranch        *b_mu_nhittgcphi;   //!
   TBranch        *b_mu_noutlierstrt;   //!
   TBranch        *b_mu_nhitcscunspoilt;   //!
   TBranch        *b_mu_nMDTBIHits;   //!
   TBranch        *b_mu_nMDTBMHits;   //!
   TBranch        *b_mu_nMDTBOHits;   //!
   TBranch        *b_mu_nMDTBEEHits;   //!
   TBranch        *b_mu_nMDTBIS78Hits;   //!
   TBranch        *b_mu_nMDTEIHits;   //!
   TBranch        *b_mu_nMDTEMHits;   //!
   TBranch        *b_mu_nMDTEOHits;   //!
   TBranch        *b_mu_nMDTEEHits;   //!
   TBranch        *b_mu_quality;   //!
   TBranch        *b_mu_scatt_curv_sig;   //!
   TBranch        *b_mu_scatt_neighb_sig;   //!
   TBranch        *b_mu_field_integral;   //!
   TBranch        *b_mu_trig_l1matchvector_;   //!
   TBranch        *b_mu_trig_l1matchvector_first;   //!
   TBranch        *b_mu_trig_l1matchvector_second;   //!
   TBranch        *b_mu_trig_l1matchdelr;   //!
   TBranch        *b_mu_trig_l2matchvector_;   //!
   TBranch        *b_mu_trig_l2matchvector_first;   //!
   TBranch        *b_mu_trig_l2matchvector_second;   //!
   TBranch        *b_mu_trig_l2matchdelr;   //!
   TBranch        *b_mu_trig_l2samatchvector_;   //!
   TBranch        *b_mu_trig_l2samatchvector_first;   //!
   TBranch        *b_mu_trig_l2samatchvector_second;   //!
   TBranch        *b_mu_trig_l2samatchdelr;   //!
   TBranch        *b_mu_trig_efmatchvector_;   //!
   TBranch        *b_mu_trig_efmatchvector_first;   //!
   TBranch        *b_mu_trig_efmatchvector_second;   //!
   TBranch        *b_mu_trig_efmatchdelr;   //!
   TBranch        *b_mu_passes_mcp;   //!
   TBranch        *b_mu_MET_wet;   //!
   TBranch        *b_mu_MET_wpx;   //!
   TBranch        *b_mu_MET_wpy;   //!
   TBranch        *b_mu_MET_status;   //!
   TBranch        *b_mu_MET_wet2;   //!
   TBranch        *b_mu_MET_wpx2;   //!
   TBranch        *b_mu_MET_wpy2;   //!
   TBranch        *b_mu_MET_status2;   //!
   TBranch        *b_mu_ptype;   //!
   TBranch        *b_mu_porigin;   //!
   TBranch        *b_mu_poutcome;   //!
   TBranch        *b_nele;   //!
   TBranch        *b_ele_author;   //!
   TBranch        *b_ele_eta;   //!
   TBranch        *b_ele_phi;   //!
   TBranch        *b_ele_et;   //!
   TBranch        *b_ele_q;   //!
   TBranch        *b_ele_etaclus;   //!
   TBranch        *b_ele_phiclus;   //!
   TBranch        *b_ele_etclus;   //!
   TBranch        *b_ele_etatrk;   //!
   TBranch        *b_ele_phitrk;   //!
   TBranch        *b_ele_pttrk;   //!
   TBranch        *b_ele_etabest;   //!
   TBranch        *b_ele_phibest;   //!
   TBranch        *b_ele_etbest;   //!
   TBranch        *b_ele_etap;   //!
   TBranch        *b_ele_f1;   //!
   TBranch        *b_ele_isEM;   //!
   TBranch        *b_ele_nucone20;   //!
   TBranch        *b_ele_nucone30;   //!
   TBranch        *b_ele_nucone40;   //!
   TBranch        *b_ele_etcone20;   //!
   TBranch        *b_ele_etcone30;   //!
   TBranch        *b_ele_etcone40;   //!
   TBranch        *b_ele_etcone20_cor;   //!
   TBranch        *b_ele_etcone30_cor;   //!
   TBranch        *b_ele_etcone40_cor;   //!
   TBranch        *b_ele_ptcone20;   //!
   TBranch        *b_ele_ptcone30;   //!
   TBranch        *b_ele_ptcone40;   //!
   TBranch        *b_ele_d0;   //!
   TBranch        *b_ele_z0;   //!
   TBranch        *b_ele_db;   //!
   TBranch        *b_ele_zb;   //!
   TBranch        *b_ele_dvx;   //!
   TBranch        *b_ele_zvx;   //!
   TBranch        *b_ele_dxpv;   //!
   TBranch        *b_ele_zxpv;   //!
   TBranch        *b_ele_covdb;   //!
   TBranch        *b_ele_covdvx;   //!
   TBranch        *b_ele_covdxpv;   //!
   TBranch        *b_ele_vtx;   //!
   TBranch        *b_ele_nhitpix;   //!
   TBranch        *b_ele_nhitsct;   //!
   TBranch        *b_ele_nhittrt;   //!
   TBranch        *b_ele_quality;   //!
   TBranch        *b_ele_isgoodoq;   //!
   TBranch        *b_ele_oq;   //!
   TBranch        *b_ele_rhad;   //!
   TBranch        *b_ele_rhad1;   //!
   TBranch        *b_ele_reta;   //!
   TBranch        *b_ele_rphi;   //!
   TBranch        *b_ele_w2;   //!
   TBranch        *b_ele_wstot;   //!
   TBranch        *b_ele_demaxs1;   //!
   TBranch        *b_ele_deltaeta;   //!
   TBranch        *b_ele_deltaphi;   //!
   TBranch        *b_ele_trratio;   //!
   TBranch        *b_ele_nhitbl;   //!
   TBranch        *b_ele_noutliersbl;   //!
   TBranch        *b_ele_expectbl;   //!
   TBranch        *b_ele_noutlierspix;   //!
   TBranch        *b_ele_noutlierssct;   //!
   TBranch        *b_ele_eta2;   //!
   TBranch        *b_ele_et2;   //!
   TBranch        *b_ele_trig_l1matchvector_;   //!
   TBranch        *b_ele_trig_l1matchvector_first;   //!
   TBranch        *b_ele_trig_l1matchvector_second;   //!
   TBranch        *b_ele_trig_l1matchdelr;   //!
   TBranch        *b_ele_trig_l2matchvector_;   //!
   TBranch        *b_ele_trig_l2matchvector_first;   //!
   TBranch        *b_ele_trig_l2matchvector_second;   //!
   TBranch        *b_ele_trig_l2matchdelr;   //!
   TBranch        *b_ele_trig_efmatchvector_;   //!
   TBranch        *b_ele_trig_efmatchvector_first;   //!
   TBranch        *b_ele_trig_efmatchvector_second;   //!
   TBranch        *b_ele_trig_efmatchdelr;   //!
   TBranch        *b_ele_MET_wet;   //!
   TBranch        *b_ele_MET_wpx;   //!
   TBranch        *b_ele_MET_wpy;   //!
   TBranch        *b_ele_MET_status;   //!
   TBranch        *b_ele_ptype;   //!
   TBranch        *b_ele_porigin;   //!
   TBranch        *b_ele_poutcome;   //!
   TBranch        *b_nph;   //!
   TBranch        *b_ph_author;   //!
   TBranch        *b_ph_eta;   //!
   TBranch        *b_ph_phi;   //!
   TBranch        *b_ph_etaclus;   //!
   TBranch        *b_ph_phiclus;   //!
   TBranch        *b_ph_et;   //!
   TBranch        *b_ph_eclus;   //!
   TBranch        *b_ph_isEM;   //!
   TBranch        *b_ph_isgoodoq;   //!
   TBranch        *b_ph_oq;   //!
   TBranch        *b_ph_quality;   //!
   TBranch        *b_ph_etcone20;   //!
   TBranch        *b_ph_etcone30;   //!
   TBranch        *b_ph_etcone40;   //!
   TBranch        *b_ph_etcone20_cor;   //!
   TBranch        *b_ph_etcone30_cor;   //!
   TBranch        *b_ph_etcone40_cor;   //!
   TBranch        *b_ph_ptcone20;   //!
   TBranch        *b_ph_ptcone30;   //!
   TBranch        *b_ph_ptcone40;   //!
   TBranch        *b_ph_isRecovered;   //!
   TBranch        *b_ph_convFlag;   //!
   TBranch        *b_ph_rconv;   //!
   TBranch        *b_ph_zconv;   //!
   TBranch        *b_ph_etas2;   //!
   TBranch        *b_ph_ethad1;   //!
   TBranch        *b_ph_ethad;   //!
   TBranch        *b_ph_e277;   //!
   TBranch        *b_ph_e237;   //!
   TBranch        *b_ph_e233;   //!
   TBranch        *b_ph_weta2;   //!
   TBranch        *b_ph_f1;   //!
   TBranch        *b_ph_emax;   //!
   TBranch        *b_ph_emaxs2;   //!
   TBranch        *b_ph_demaxs1;   //!
   TBranch        *b_ph_fside;   //!
   TBranch        *b_ph_wstot;   //!
   TBranch        *b_ph_ws3;   //!
   TBranch        *b_ph_emins1;   //!
   TBranch        *b_ph_etap;   //!
   TBranch        *b_ph_deltaeta;   //!
   TBranch        *b_ph_MET_wet;   //!
   TBranch        *b_ph_MET_wpx;   //!
   TBranch        *b_ph_MET_wpy;   //!
   TBranch        *b_ph_MET_status;   //!
   TBranch        *b_ph_ptype;   //!
   TBranch        *b_ph_porigin;   //!
   TBranch        *b_ph_poutcome;   //!
   TBranch        *b_ntrk;   //!
   TBranch        *b_trk_pt;   //!
   TBranch        *b_trk_eta;   //!
   TBranch        *b_trk_phi;   //!
   TBranch        *b_trk_m;   //!
   TBranch        *b_trk_q;   //!
   TBranch        *b_trk_mu;   //!
   TBranch        *b_trk_nucone10;   //!
   TBranch        *b_trk_nucone20;   //!
   TBranch        *b_trk_nucone30;   //!
   TBranch        *b_trk_nucone40;   //!
   TBranch        *b_trk_ptcone10;   //!
   TBranch        *b_trk_ptcone20;   //!
   TBranch        *b_trk_ptcone30;   //!
   TBranch        *b_trk_ptcone40;   //!
   TBranch        *b_trk_vtx;   //!
   TBranch        *b_trk_dxpv;   //!
   TBranch        *b_trk_zxpv;   //!
   TBranch        *b_trk_covdxpv;   //!
   TBranch        *b_trk_nhitbl;   //!
   TBranch        *b_trk_expectbl;   //!
   TBranch        *b_trk_nhitpix;   //!
   TBranch        *b_trk_nholespix;   //!
   TBranch        *b_trk_ndeadsensorspix;   //!
   TBranch        *b_trk_nhitsct;   //!
   TBranch        *b_trk_nholessct;   //!
   TBranch        *b_trk_ndeadsensorssct;   //!
   TBranch        *b_trk_nhittrt;   //!
   TBranch        *b_trk_nholestrt;   //!
   TBranch        *b_trk_noutlierstrt;   //!
   TBranch        *b_trk_ptype;   //!
   TBranch        *b_trk_porigin;   //!
   TBranch        *b_trk_poutcome;   //!
   TBranch        *b_trk_pmatchprob;   //!
   TBranch        *b_njet;   //!
   TBranch        *b_jet_type;   //!
   TBranch        *b_jet_et;   //!
   TBranch        *b_jet_pt;   //!
   TBranch        *b_jet_eta;   //!
   TBranch        *b_jet_phi;   //!
   TBranch        *b_jet_m;   //!
   TBranch        *b_jet_pt_em;   //!
   TBranch        *b_jet_eta_em;   //!
   TBranch        *b_jet_phi_em;   //!
   TBranch        *b_jet_m_em;   //!
   TBranch        *b_jet_eta_origin;   //!
   TBranch        *b_jet_phi_origin;   //!
   TBranch        *b_jet_m_origin;   //!
   TBranch        *b_jet_eta_origin_em;   //!
   TBranch        *b_jet_phi_origin_em;   //!
   TBranch        *b_jet_m_origin_em;   //!
   TBranch        *b_jet_gcwjes;   //!
   TBranch        *b_jet_emjes;   //!
   TBranch        *b_jet_isgood;   //!
   TBranch        *b_jet_isbad;   //!
   TBranch        *b_jet_isugly;   //!
   TBranch        *b_jet_n90;   //!
   TBranch        *b_jet_hecquality;   //!
   TBranch        *b_jet_larquality;   //!
   TBranch        *b_jet_averagelarqf;   //!
   TBranch        *b_jet_negativee;   //!
   TBranch        *b_jet_sumpttrk;   //!
   TBranch        *b_jet_ntrk;   //!
   TBranch        *b_jet_jvtxf;   //!
   TBranch        *b_jet_jvf;   //!
   TBranch        *b_jet_bchcorr;   //!
   TBranch        *b_jet_bchcorrjet;   //!
   TBranch        *b_jet_tgap3f;   //!
   TBranch        *b_jet_time;   //!
   TBranch        *b_jet_emf;   //!
   TBranch        *b_jet_hecf;   //!
   TBranch        *b_jet_fmax;   //!
   TBranch        *b_jet_tagSV0;   //!
   TBranch        *b_jet_tagSV1;   //!
   TBranch        *b_jet_tagSV2;   //!
   TBranch        *b_jet_tagJetProb;   //!
   TBranch        *b_jet_tagIP1D;   //!
   TBranch        *b_jet_tagIP2D;   //!
   TBranch        *b_jet_tagIP3D;   //!
   TBranch        *b_jet_tagJetFitterTag;   //!
   TBranch        *b_jet_tagJetFitterCOMB;   //!
   TBranch        *b_jet_tagJetFitterCOMBNN;   //!
   TBranch        *b_jet_tagIP3DSV1;   //!
   TBranch        *b_jet_tagSoftElectronTag;   //!
   TBranch        *b_jet_tagSoftMuonTag;   //!
   TBranch        *b_jet_vtx;   //!
   TBranch        *b_jet_js_tau1;   //!
   TBranch        *b_jet_js_tau2;   //!
   TBranch        *b_jet_js_tau3;   //!
   TBranch        *b_jet_js_width;   //!
   TBranch        *b_jet_js_qw;   //!
   TBranch        *b_jet_js_split12;   //!
   TBranch        *b_jet_js_split23;   //!
   TBranch        *b_jet_js_split34;   //!
   TBranch        *b_jet_js_zcut12;   //!
   TBranch        *b_jet_js_zcut23;   //!
   TBranch        *b_jet_js_zcut34;   //!
   TBranch        *b_jet_js_statshape0;   //!
   TBranch        *b_jet_js_statshape1;   //!
   TBranch        *b_jet_js_statshape2;   //!
   TBranch        *b_jet_js_statshape5;   //!
   TBranch        *b_jet_js_statshape8;   //!
   TBranch        *b_jet_js_pullmag;   //!
   TBranch        *b_jet_js_pullphi;   //!
   TBranch        *b_jet_js_pullc00;   //!
   TBranch        *b_jet_js_pullc01;   //!
   TBranch        *b_jet_js_pullc10;   //!
   TBranch        *b_jet_js_pullc11;   //!
   TBranch        *b_jet_MET_wet;   //!
   TBranch        *b_jet_MET_wpx;   //!
   TBranch        *b_jet_MET_wpy;   //!
   TBranch        *b_jet_MET_status;   //!
   TBranch        *b_met_reffinal;   //!
   TBranch        *b_met_reffinal_phi;   //!
   TBranch        *b_met_reffinal_sumet;   //!
   TBranch        *b_met_reffinal_em;   //!
   TBranch        *b_met_reffinal_em_phi;   //!
   TBranch        *b_met_reffinal_em_sumet;   //!
   TBranch        *b_met_refjet;   //!
   TBranch        *b_met_refjet_phi;   //!
   TBranch        *b_met_refjet_sumet;   //!
   TBranch        *b_met_cellout;   //!
   TBranch        *b_met_cellout_phi;   //!
   TBranch        *b_met_cellout_sumet;   //!
   TBranch        *b_met_softjets;   //!
   TBranch        *b_met_softjets_phi;   //!
   TBranch        *b_met_softjets_sumet;   //!
   TBranch        *b_met_cellout_eflow;   //!
   TBranch        *b_met_cellout_eflow_phi;   //!
   TBranch        *b_met_cellout_eflow_sumet;   //!
   TBranch        *b_met_cellout_eflow_muid;   //!
   TBranch        *b_met_cellout_eflow_muid_phi;   //!
   TBranch        *b_met_cellout_eflow_muid_sumet;   //!
   TBranch        *b_met_cryo;   //!
   TBranch        *b_met_cryo_phi;   //!
   TBranch        *b_met_cryo_sumet;   //!
   TBranch        *b_met_refmuon;   //!
   TBranch        *b_met_refmuon_phi;   //!
   TBranch        *b_met_refmuon_sumet;   //!
   TBranch        *b_met_refmuon_track;   //!
   TBranch        *b_met_refmuon_track_phi;   //!
   TBranch        *b_met_refmuon_track_sumet;   //!
   TBranch        *b_met_muonboy;   //!
   TBranch        *b_met_muonboy_phi;   //!
   TBranch        *b_met_muonboy_sumet;   //!
   TBranch        *b_met_muid;   //!
   TBranch        *b_met_muid_phi;   //!
   TBranch        *b_met_muid_sumet;   //!
   TBranch        *b_met_topo;   //!
   TBranch        *b_met_topo_phi;   //!
   TBranch        *b_met_topo_sumet;   //!
   TBranch        *b_met_topo_eta45;   //!
   TBranch        *b_met_topo_eta45_phi;   //!
   TBranch        *b_met_topo_eta45_sumet;   //!
   TBranch        *b_met_lochadtopo;   //!
   TBranch        *b_met_lochadtopo_phi;   //!
   TBranch        *b_met_lochadtopo_sumet;   //!
   TBranch        *b_met_lochadtopo_eta45;   //!
   TBranch        *b_met_lochadtopo_eta45_phi;   //!
   TBranch        *b_met_lochadtopo_eta45_sumet;   //!
   TBranch        *b_met_reffinal_eta45;   //!
   TBranch        *b_met_reffinal_eta45_phi;   //!
   TBranch        *b_met_reffinal_eta45_sumet;   //!
   TBranch        *b_met_muon;   //!
   TBranch        *b_met_muon_phi;   //!
   TBranch        *b_met_muon_sumet;   //!
   TBranch        *b_met_muon_muid;   //!
   TBranch        *b_met_muon_muid_phi;   //!
   TBranch        *b_met_muon_muid_sumet;   //!
   TBranch        *b_met_refele;   //!
   TBranch        *b_met_refele_phi;   //!
   TBranch        *b_met_refele_sumet;   //!
   TBranch        *b_met_reftau;   //!
   TBranch        *b_met_reftau_phi;   //!
   TBranch        *b_met_reftau_sumet;   //!
   TBranch        *b_met_refgamma;   //!
   TBranch        *b_met_refgamma_phi;   //!
   TBranch        *b_met_refgamma_sumet;   //!
   TBranch        *b_met_plhc;   //!
   TBranch        *b_met_plhc_phi;   //!
   TBranch        *b_met_plhc_sumet;   //!
   TBranch        *b_met_ichep;   //!
   TBranch        *b_met_ichep_phi;   //!
   TBranch        *b_met_ichep_sumet;   //!
   TBranch        *b_met_truth;   //!
   TBranch        *b_met_truth_phi;   //!
   TBranch        *b_met_truth_sumet;   //!
   TBranch        *b_met_track;   //!
   TBranch        *b_met_track_phi;   //!
   TBranch        *b_met_track_sumet;   //!
   TBranch        *b_nmc;   //!
   TBranch        *b_mc_weight;   //!
   TBranch        *b_mc_hfor;   //!
   TBranch        *b_mc_channel;   //!
   TBranch        *b_mc_pdgid;   //!
   TBranch        *b_mc_m;   //!
   TBranch        *b_mc_e;   //!
   TBranch        *b_mc_pt;   //!
   TBranch        *b_mc_phi;   //!
   TBranch        *b_mc_eta;   //!
   TBranch        *b_mc_parent;   //!
   TBranch        *b_mc_status;   //!
   TBranch        *b_mc_ptype;   //!
   TBranch        *b_mc_porigin;   //!
   TBranch        *b_mc_poutcome;   //!
   TBranch        *b_mc_pmotherpdg;   //!
   TBranch        *b_mcevt_hardindex;   //!
   TBranch        *b_mcevt_pdfset;   //!
   TBranch        *b_mcevt_weight;   //!
   TBranch        *b_mcevt_sigid;   //!
   TBranch        *b_mcevt_event_scale;   //!
   TBranch        *b_mcevt_pdf_scale;   //!
   TBranch        *b_mcevt_alphaQCD;   //!
   TBranch        *b_mcevt_alphaQED;   //!
   TBranch        *b_mcevt_id1;   //!
   TBranch        *b_mcevt_id2;   //!
   TBranch        *b_mcevt_pdf_x1;   //!
   TBranch        *b_mcevt_pdf_x2;   //!
   TBranch        *b_mcevt_pdf1;   //!
   TBranch        *b_mcevt_pdf2;   //!
   TBranch        *b_njpsi;   //!
   TBranch        *b_jpsi_mass;   //!
   TBranch        *b_jpsi_masserr;   //!
   TBranch        *b_jpsi_phi;   //!
   TBranch        *b_jpsi_pt;   //!
   TBranch        *b_jpsi_y;   //!
   TBranch        *b_jpsi_chisq;   //!
   TBranch        *b_jpsi_tau;   //!
   TBranch        *b_jpsi_tauerr;   //!
   TBranch        *b_jpsi_costhetastar;   //!
   TBranch        *b_jpsi_is_os;   //!
   TBranch        *b_jpsi_mup;   //!
   TBranch        *b_jpsi_mum;   //!
   TBranch        *b_lha_cteq6ll;   //!
   TBranch        *b_lha_MRSTMCal;   //!
   TBranch        *b_lha_CT10;   //!
   TBranch        *b_lha_MSTW2008lo68cl;   //!
   TBranch        *b_lha_MSTW2008nlo68cl;   //!
   TBranch        *b_lha_NNPDF21_100;   //!
   TBranch        *b_lha_HERAPDF10_EIG;   //!
   TBranch        *b_lha_cteq66;   //!

   Deleteme(TTree *tree=0);
   virtual ~Deleteme();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef Deleteme_cxx
Deleteme::Deleteme(TTree *tree)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/atlas/uct3/data/users/antonk/NTUPLE/v1_29d/user.kapliy.UCNTUP.mc11_7TeV.106047.PythiaZmumu_no_filter.merge.AOD.e815_s1272_s1274_r3043_r2993.j.mc11c.v1_29d.120218115907/user.kapliy.008483.flatntuple._00001.root");
      if (!f) {
         f = new TFile("/atlas/uct3/data/users/antonk/NTUPLE/v1_29d/user.kapliy.UCNTUP.mc11_7TeV.106047.PythiaZmumu_no_filter.merge.AOD.e815_s1272_s1274_r3043_r2993.j.mc11c.v1_29d.120218115907/user.kapliy.008483.flatntuple._00001.root");
      }
      tree = (TTree*)gDirectory->Get("tree");

   }
   Init(tree);
}

Deleteme::~Deleteme()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t Deleteme::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t Deleteme::LoadTree(Long64_t entry)
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

void Deleteme::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

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

   fChain->SetBranchAddress("run", &run, &b_run);
   fChain->SetBranchAddress("event", &event, &b_event);
   fChain->SetBranchAddress("lb", &lb, &b_lb);
   fChain->SetBranchAddress("bcid", &bcid, &b_bcid);
   fChain->SetBranchAddress("lumi_mu_actual", &lumi_mu_actual, &b_lumi_mu_actual);
   fChain->SetBranchAddress("lumi_mu_average", &lumi_mu_average, &b_lumi_mu_average);
   fChain->SetBranchAddress("bdff", &bdff, &b_bdff);
   fChain->SetBranchAddress("bfif600", &bfif600, &b_bfif600);
   fChain->SetBranchAddress("bint", &bint, &b_bint);
   fChain->SetBranchAddress("error_lar", &error_lar, &b_error_lar);
   fChain->SetBranchAddress("error_background", &error_background, &b_error_background);
   fChain->SetBranchAddress("trig_ef", &trig_ef, &b_trig_ef);
   fChain->SetBranchAddress("trig_l2", &trig_l2, &b_trig_l2);
   fChain->SetBranchAddress("trig_l1", &trig_l1, &b_trig_l1);
   fChain->SetBranchAddress("bs_x", &bs_x, &b_bs_x);
   fChain->SetBranchAddress("bs_y", &bs_y, &b_bs_y);
   fChain->SetBranchAddress("bs_z", &bs_z, &b_bs_z);
   fChain->SetBranchAddress("bs_status", &bs_status, &b_bs_status);
   fChain->SetBranchAddress("nvx", &nvx, &b_nvx);
   fChain->SetBranchAddress("vx_type", vx_type, &b_vx_type);
   fChain->SetBranchAddress("vx_ntracks", vx_ntracks, &b_vx_ntracks);
   fChain->SetBranchAddress("vx_sumpt", vx_sumpt, &b_vx_sumpt);
   fChain->SetBranchAddress("vx_x", vx_x, &b_vx_x);
   fChain->SetBranchAddress("vx_y", vx_y, &b_vx_y);
   fChain->SetBranchAddress("vx_z", vx_z, &b_vx_z);
   fChain->SetBranchAddress("nmu", &nmu, &b_nmu);
   fChain->SetBranchAddress("mu_type", mu_type, &b_mu_type);
   fChain->SetBranchAddress("mu_pt", mu_pt, &b_mu_pt);
   fChain->SetBranchAddress("mu_eta", mu_eta, &b_mu_eta);
   fChain->SetBranchAddress("mu_phi", mu_phi, &b_mu_phi);
   fChain->SetBranchAddress("mu_q", mu_q, &b_mu_q);
   fChain->SetBranchAddress("mu_ptcid", mu_ptcid, &b_mu_ptcid);
   fChain->SetBranchAddress("mu_etacid", mu_etacid, &b_mu_etacid);
   fChain->SetBranchAddress("mu_phicid", mu_phicid, &b_mu_phicid);
   fChain->SetBranchAddress("mu_qcid", mu_qcid, &b_mu_qcid);
   fChain->SetBranchAddress("mu_ptid", mu_ptid, &b_mu_ptid);
   fChain->SetBranchAddress("mu_etaid", mu_etaid, &b_mu_etaid);
   fChain->SetBranchAddress("mu_phiid", mu_phiid, &b_mu_phiid);
   fChain->SetBranchAddress("mu_qid", mu_qid, &b_mu_qid);
   fChain->SetBranchAddress("mu_ptexid", mu_ptexid, &b_mu_ptexid);
   fChain->SetBranchAddress("mu_etaexid", mu_etaexid, &b_mu_etaexid);
   fChain->SetBranchAddress("mu_phiexid", mu_phiexid, &b_mu_phiexid);
   fChain->SetBranchAddress("mu_qexid", mu_qexid, &b_mu_qexid);
   fChain->SetBranchAddress("mu_ptms", mu_ptms, &b_mu_ptms);
   fChain->SetBranchAddress("mu_etams", mu_etams, &b_mu_etams);
   fChain->SetBranchAddress("mu_phims", mu_phims, &b_mu_phims);
   fChain->SetBranchAddress("mu_qms", mu_qms, &b_mu_qms);
   fChain->SetBranchAddress("mu_ptexms", mu_ptexms, &b_mu_ptexms);
   fChain->SetBranchAddress("mu_etaexms", mu_etaexms, &b_mu_etaexms);
   fChain->SetBranchAddress("mu_phiexms", mu_phiexms, &b_mu_phiexms);
   fChain->SetBranchAddress("mu_qexms", mu_qexms, &b_mu_qexms);
   fChain->SetBranchAddress("mu_author", mu_author, &b_mu_author);
   fChain->SetBranchAddress("mu_primauthor", mu_primauthor, &b_mu_primauthor);
   fChain->SetBranchAddress("mu_class", mu_class, &b_mu_class);
   fChain->SetBranchAddress("mu_iscombined", mu_iscombined, &b_mu_iscombined);
   fChain->SetBranchAddress("mu_issegtagged", mu_issegtagged, &b_mu_issegtagged);
   fChain->SetBranchAddress("mu_fitchi2prob", mu_fitchi2prob, &b_mu_fitchi2prob);
   fChain->SetBranchAddress("mu_matchchi2prob", mu_matchchi2prob, &b_mu_matchchi2prob);
   fChain->SetBranchAddress("mu_fitchi2", mu_fitchi2, &b_mu_fitchi2);
   fChain->SetBranchAddress("mu_matchchi2", mu_matchchi2, &b_mu_matchchi2);
   fChain->SetBranchAddress("mu_bestmatch", mu_bestmatch, &b_mu_bestmatch);
   fChain->SetBranchAddress("mu_nucone10", mu_nucone10, &b_mu_nucone10);
   fChain->SetBranchAddress("mu_nucone20", mu_nucone20, &b_mu_nucone20);
   fChain->SetBranchAddress("mu_nucone30", mu_nucone30, &b_mu_nucone30);
   fChain->SetBranchAddress("mu_nucone40", mu_nucone40, &b_mu_nucone40);
   fChain->SetBranchAddress("mu_etcone10", mu_etcone10, &b_mu_etcone10);
   fChain->SetBranchAddress("mu_etcone20", mu_etcone20, &b_mu_etcone20);
   fChain->SetBranchAddress("mu_etcone30", mu_etcone30, &b_mu_etcone30);
   fChain->SetBranchAddress("mu_etcone40", mu_etcone40, &b_mu_etcone40);
   fChain->SetBranchAddress("mu_ptcone10", mu_ptcone10, &b_mu_ptcone10);
   fChain->SetBranchAddress("mu_ptcone20", mu_ptcone20, &b_mu_ptcone20);
   fChain->SetBranchAddress("mu_ptcone30", mu_ptcone30, &b_mu_ptcone30);
   fChain->SetBranchAddress("mu_ptcone40", mu_ptcone40, &b_mu_ptcone40);
   fChain->SetBranchAddress("mu_ptconemod10", mu_ptconemod10, &b_mu_ptconemod10);
   fChain->SetBranchAddress("mu_ptconemod20", mu_ptconemod20, &b_mu_ptconemod20);
   fChain->SetBranchAddress("mu_ptconemod30", mu_ptconemod30, &b_mu_ptconemod30);
   fChain->SetBranchAddress("mu_ptconemod40", mu_ptconemod40, &b_mu_ptconemod40);
   fChain->SetBranchAddress("mu_eloss", mu_eloss, &b_mu_eloss);
   fChain->SetBranchAddress("mu_d0", mu_d0, &b_mu_d0);
   fChain->SetBranchAddress("mu_z0", mu_z0, &b_mu_z0);
   fChain->SetBranchAddress("mu_db", mu_db, &b_mu_db);
   fChain->SetBranchAddress("mu_zb", mu_zb, &b_mu_zb);
   fChain->SetBranchAddress("mu_dvx", mu_dvx, &b_mu_dvx);
   fChain->SetBranchAddress("mu_zvx", mu_zvx, &b_mu_zvx);
   fChain->SetBranchAddress("mu_dxpv", mu_dxpv, &b_mu_dxpv);
   fChain->SetBranchAddress("mu_zxpv", mu_zxpv, &b_mu_zxpv);
   fChain->SetBranchAddress("mu_covdb", mu_covdb, &b_mu_covdb);
   fChain->SetBranchAddress("mu_covdvx", mu_covdvx, &b_mu_covdvx);
   fChain->SetBranchAddress("mu_covdxpv", mu_covdxpv, &b_mu_covdxpv);
   fChain->SetBranchAddress("mu_vtxtype", mu_vtxtype, &b_mu_vtxtype);
   fChain->SetBranchAddress("mu_vtx", mu_vtx, &b_mu_vtx);
   fChain->SetBranchAddress("mu_nhitbl", mu_nhitbl, &b_mu_nhitbl);
   fChain->SetBranchAddress("mu_expectbl", mu_expectbl, &b_mu_expectbl);
   fChain->SetBranchAddress("mu_nhitpix", mu_nhitpix, &b_mu_nhitpix);
   fChain->SetBranchAddress("mu_nholespix", mu_nholespix, &b_mu_nholespix);
   fChain->SetBranchAddress("mu_ndeadsensorspix", mu_ndeadsensorspix, &b_mu_ndeadsensorspix);
   fChain->SetBranchAddress("mu_nhitsct", mu_nhitsct, &b_mu_nhitsct);
   fChain->SetBranchAddress("mu_nholessct", mu_nholessct, &b_mu_nholessct);
   fChain->SetBranchAddress("mu_ndeadsensorssct", mu_ndeadsensorssct, &b_mu_ndeadsensorssct);
   fChain->SetBranchAddress("mu_nhittrt", mu_nhittrt, &b_mu_nhittrt);
   fChain->SetBranchAddress("mu_nholestrt", mu_nholestrt, &b_mu_nholestrt);
   fChain->SetBranchAddress("mu_nhitmdt", mu_nhitmdt, &b_mu_nhitmdt);
   fChain->SetBranchAddress("mu_nhitcsceta", mu_nhitcsceta, &b_mu_nhitcsceta);
   fChain->SetBranchAddress("mu_nhitcscphi", mu_nhitcscphi, &b_mu_nhitcscphi);
   fChain->SetBranchAddress("mu_nhitrpceta", mu_nhitrpceta, &b_mu_nhitrpceta);
   fChain->SetBranchAddress("mu_nhitrpcphi", mu_nhitrpcphi, &b_mu_nhitrpcphi);
   fChain->SetBranchAddress("mu_nhittgceta", mu_nhittgceta, &b_mu_nhittgceta);
   fChain->SetBranchAddress("mu_nhittgcphi", mu_nhittgcphi, &b_mu_nhittgcphi);
   fChain->SetBranchAddress("mu_noutlierstrt", mu_noutlierstrt, &b_mu_noutlierstrt);
   fChain->SetBranchAddress("mu_nhitcscunspoilt", mu_nhitcscunspoilt, &b_mu_nhitcscunspoilt);
   fChain->SetBranchAddress("mu_nMDTBIHits", mu_nMDTBIHits, &b_mu_nMDTBIHits);
   fChain->SetBranchAddress("mu_nMDTBMHits", mu_nMDTBMHits, &b_mu_nMDTBMHits);
   fChain->SetBranchAddress("mu_nMDTBOHits", mu_nMDTBOHits, &b_mu_nMDTBOHits);
   fChain->SetBranchAddress("mu_nMDTBEEHits", mu_nMDTBEEHits, &b_mu_nMDTBEEHits);
   fChain->SetBranchAddress("mu_nMDTBIS78Hits", mu_nMDTBIS78Hits, &b_mu_nMDTBIS78Hits);
   fChain->SetBranchAddress("mu_nMDTEIHits", mu_nMDTEIHits, &b_mu_nMDTEIHits);
   fChain->SetBranchAddress("mu_nMDTEMHits", mu_nMDTEMHits, &b_mu_nMDTEMHits);
   fChain->SetBranchAddress("mu_nMDTEOHits", mu_nMDTEOHits, &b_mu_nMDTEOHits);
   fChain->SetBranchAddress("mu_nMDTEEHits", mu_nMDTEEHits, &b_mu_nMDTEEHits);
   fChain->SetBranchAddress("mu_quality", mu_quality, &b_mu_quality);
   fChain->SetBranchAddress("mu_scatt_curv_sig", mu_scatt_curv_sig, &b_mu_scatt_curv_sig);
   fChain->SetBranchAddress("mu_scatt_neighb_sig", mu_scatt_neighb_sig, &b_mu_scatt_neighb_sig);
   fChain->SetBranchAddress("mu_field_integral", mu_field_integral, &b_mu_field_integral);
   fChain->SetBranchAddress("mu_trig_l1matchvector", &mu_trig_l1matchvector_, &b_mu_trig_l1matchvector_);
   fChain->SetBranchAddress("mu_trig_l1matchvector.first", mu_trig_l1matchvector_first, &b_mu_trig_l1matchvector_first);
   fChain->SetBranchAddress("mu_trig_l1matchvector.second", mu_trig_l1matchvector_second, &b_mu_trig_l1matchvector_second);
   fChain->SetBranchAddress("mu_trig_l1matchdelr", &mu_trig_l1matchdelr, &b_mu_trig_l1matchdelr);
   fChain->SetBranchAddress("mu_trig_l2matchvector", &mu_trig_l2matchvector_, &b_mu_trig_l2matchvector_);
   fChain->SetBranchAddress("mu_trig_l2matchvector.first", mu_trig_l2matchvector_first, &b_mu_trig_l2matchvector_first);
   fChain->SetBranchAddress("mu_trig_l2matchvector.second", mu_trig_l2matchvector_second, &b_mu_trig_l2matchvector_second);
   fChain->SetBranchAddress("mu_trig_l2matchdelr", &mu_trig_l2matchdelr, &b_mu_trig_l2matchdelr);
   fChain->SetBranchAddress("mu_trig_l2samatchvector", &mu_trig_l2samatchvector_, &b_mu_trig_l2samatchvector_);
   fChain->SetBranchAddress("mu_trig_l2samatchvector.first", mu_trig_l2samatchvector_first, &b_mu_trig_l2samatchvector_first);
   fChain->SetBranchAddress("mu_trig_l2samatchvector.second", mu_trig_l2samatchvector_second, &b_mu_trig_l2samatchvector_second);
   fChain->SetBranchAddress("mu_trig_l2samatchdelr", &mu_trig_l2samatchdelr, &b_mu_trig_l2samatchdelr);
   fChain->SetBranchAddress("mu_trig_efmatchvector", &mu_trig_efmatchvector_, &b_mu_trig_efmatchvector_);
   fChain->SetBranchAddress("mu_trig_efmatchvector.first", mu_trig_efmatchvector_first, &b_mu_trig_efmatchvector_first);
   fChain->SetBranchAddress("mu_trig_efmatchvector.second", mu_trig_efmatchvector_second, &b_mu_trig_efmatchvector_second);
   fChain->SetBranchAddress("mu_trig_efmatchdelr", &mu_trig_efmatchdelr, &b_mu_trig_efmatchdelr);
   fChain->SetBranchAddress("mu_passes_mcp", mu_passes_mcp, &b_mu_passes_mcp);
   fChain->SetBranchAddress("mu_MET_wet", mu_MET_wet, &b_mu_MET_wet);
   fChain->SetBranchAddress("mu_MET_wpx", mu_MET_wpx, &b_mu_MET_wpx);
   fChain->SetBranchAddress("mu_MET_wpy", mu_MET_wpy, &b_mu_MET_wpy);
   fChain->SetBranchAddress("mu_MET_status", mu_MET_status, &b_mu_MET_status);
   fChain->SetBranchAddress("mu_MET_wet2", mu_MET_wet2, &b_mu_MET_wet2);
   fChain->SetBranchAddress("mu_MET_wpx2", mu_MET_wpx2, &b_mu_MET_wpx2);
   fChain->SetBranchAddress("mu_MET_wpy2", mu_MET_wpy2, &b_mu_MET_wpy2);
   fChain->SetBranchAddress("mu_MET_status2", mu_MET_status2, &b_mu_MET_status2);
   fChain->SetBranchAddress("mu_ptype", mu_ptype, &b_mu_ptype);
   fChain->SetBranchAddress("mu_porigin", mu_porigin, &b_mu_porigin);
   fChain->SetBranchAddress("mu_poutcome", mu_poutcome, &b_mu_poutcome);
   fChain->SetBranchAddress("nele", &nele, &b_nele);
   fChain->SetBranchAddress("ele_author", ele_author, &b_ele_author);
   fChain->SetBranchAddress("ele_eta", ele_eta, &b_ele_eta);
   fChain->SetBranchAddress("ele_phi", ele_phi, &b_ele_phi);
   fChain->SetBranchAddress("ele_et", ele_et, &b_ele_et);
   fChain->SetBranchAddress("ele_q", ele_q, &b_ele_q);
   fChain->SetBranchAddress("ele_etaclus", ele_etaclus, &b_ele_etaclus);
   fChain->SetBranchAddress("ele_phiclus", ele_phiclus, &b_ele_phiclus);
   fChain->SetBranchAddress("ele_etclus", ele_etclus, &b_ele_etclus);
   fChain->SetBranchAddress("ele_etatrk", ele_etatrk, &b_ele_etatrk);
   fChain->SetBranchAddress("ele_phitrk", ele_phitrk, &b_ele_phitrk);
   fChain->SetBranchAddress("ele_pttrk", ele_pttrk, &b_ele_pttrk);
   fChain->SetBranchAddress("ele_etabest", ele_etabest, &b_ele_etabest);
   fChain->SetBranchAddress("ele_phibest", ele_phibest, &b_ele_phibest);
   fChain->SetBranchAddress("ele_etbest", ele_etbest, &b_ele_etbest);
   fChain->SetBranchAddress("ele_etap", ele_etap, &b_ele_etap);
   fChain->SetBranchAddress("ele_f1", ele_f1, &b_ele_f1);
   fChain->SetBranchAddress("ele_isEM", ele_isEM, &b_ele_isEM);
   fChain->SetBranchAddress("ele_nucone20", ele_nucone20, &b_ele_nucone20);
   fChain->SetBranchAddress("ele_nucone30", ele_nucone30, &b_ele_nucone30);
   fChain->SetBranchAddress("ele_nucone40", ele_nucone40, &b_ele_nucone40);
   fChain->SetBranchAddress("ele_etcone20", ele_etcone20, &b_ele_etcone20);
   fChain->SetBranchAddress("ele_etcone30", ele_etcone30, &b_ele_etcone30);
   fChain->SetBranchAddress("ele_etcone40", ele_etcone40, &b_ele_etcone40);
   fChain->SetBranchAddress("ele_etcone20_cor", ele_etcone20_cor, &b_ele_etcone20_cor);
   fChain->SetBranchAddress("ele_etcone30_cor", ele_etcone30_cor, &b_ele_etcone30_cor);
   fChain->SetBranchAddress("ele_etcone40_cor", ele_etcone40_cor, &b_ele_etcone40_cor);
   fChain->SetBranchAddress("ele_ptcone20", ele_ptcone20, &b_ele_ptcone20);
   fChain->SetBranchAddress("ele_ptcone30", ele_ptcone30, &b_ele_ptcone30);
   fChain->SetBranchAddress("ele_ptcone40", ele_ptcone40, &b_ele_ptcone40);
   fChain->SetBranchAddress("ele_d0", ele_d0, &b_ele_d0);
   fChain->SetBranchAddress("ele_z0", ele_z0, &b_ele_z0);
   fChain->SetBranchAddress("ele_db", ele_db, &b_ele_db);
   fChain->SetBranchAddress("ele_zb", ele_zb, &b_ele_zb);
   fChain->SetBranchAddress("ele_dvx", ele_dvx, &b_ele_dvx);
   fChain->SetBranchAddress("ele_zvx", ele_zvx, &b_ele_zvx);
   fChain->SetBranchAddress("ele_dxpv", ele_dxpv, &b_ele_dxpv);
   fChain->SetBranchAddress("ele_zxpv", ele_zxpv, &b_ele_zxpv);
   fChain->SetBranchAddress("ele_covdb", ele_covdb, &b_ele_covdb);
   fChain->SetBranchAddress("ele_covdvx", ele_covdvx, &b_ele_covdvx);
   fChain->SetBranchAddress("ele_covdxpv", ele_covdxpv, &b_ele_covdxpv);
   fChain->SetBranchAddress("ele_vtx", ele_vtx, &b_ele_vtx);
   fChain->SetBranchAddress("ele_nhitpix", ele_nhitpix, &b_ele_nhitpix);
   fChain->SetBranchAddress("ele_nhitsct", ele_nhitsct, &b_ele_nhitsct);
   fChain->SetBranchAddress("ele_nhittrt", ele_nhittrt, &b_ele_nhittrt);
   fChain->SetBranchAddress("ele_quality", ele_quality, &b_ele_quality);
   fChain->SetBranchAddress("ele_isgoodoq", ele_isgoodoq, &b_ele_isgoodoq);
   fChain->SetBranchAddress("ele_oq", ele_oq, &b_ele_oq);
   fChain->SetBranchAddress("ele_rhad", ele_rhad, &b_ele_rhad);
   fChain->SetBranchAddress("ele_rhad1", ele_rhad1, &b_ele_rhad1);
   fChain->SetBranchAddress("ele_reta", ele_reta, &b_ele_reta);
   fChain->SetBranchAddress("ele_rphi", ele_rphi, &b_ele_rphi);
   fChain->SetBranchAddress("ele_w2", ele_w2, &b_ele_w2);
   fChain->SetBranchAddress("ele_wstot", ele_wstot, &b_ele_wstot);
   fChain->SetBranchAddress("ele_demaxs1", ele_demaxs1, &b_ele_demaxs1);
   fChain->SetBranchAddress("ele_deltaeta", ele_deltaeta, &b_ele_deltaeta);
   fChain->SetBranchAddress("ele_deltaphi", ele_deltaphi, &b_ele_deltaphi);
   fChain->SetBranchAddress("ele_trratio", ele_trratio, &b_ele_trratio);
   fChain->SetBranchAddress("ele_nhitbl", ele_nhitbl, &b_ele_nhitbl);
   fChain->SetBranchAddress("ele_noutliersbl", ele_noutliersbl, &b_ele_noutliersbl);
   fChain->SetBranchAddress("ele_expectbl", ele_expectbl, &b_ele_expectbl);
   fChain->SetBranchAddress("ele_noutlierspix", ele_noutlierspix, &b_ele_noutlierspix);
   fChain->SetBranchAddress("ele_noutlierssct", ele_noutlierssct, &b_ele_noutlierssct);
   fChain->SetBranchAddress("ele_eta2", ele_eta2, &b_ele_eta2);
   fChain->SetBranchAddress("ele_et2", ele_et2, &b_ele_et2);
   fChain->SetBranchAddress("ele_trig_l1matchvector", &ele_trig_l1matchvector_, &b_ele_trig_l1matchvector_);
   fChain->SetBranchAddress("ele_trig_l1matchvector.first", ele_trig_l1matchvector_first, &b_ele_trig_l1matchvector_first);
   fChain->SetBranchAddress("ele_trig_l1matchvector.second", ele_trig_l1matchvector_second, &b_ele_trig_l1matchvector_second);
   fChain->SetBranchAddress("ele_trig_l1matchdelr", &ele_trig_l1matchdelr, &b_ele_trig_l1matchdelr);
   fChain->SetBranchAddress("ele_trig_l2matchvector", &ele_trig_l2matchvector_, &b_ele_trig_l2matchvector_);
   fChain->SetBranchAddress("ele_trig_l2matchvector.first", ele_trig_l2matchvector_first, &b_ele_trig_l2matchvector_first);
   fChain->SetBranchAddress("ele_trig_l2matchvector.second", ele_trig_l2matchvector_second, &b_ele_trig_l2matchvector_second);
   fChain->SetBranchAddress("ele_trig_l2matchdelr", &ele_trig_l2matchdelr, &b_ele_trig_l2matchdelr);
   fChain->SetBranchAddress("ele_trig_efmatchvector", &ele_trig_efmatchvector_, &b_ele_trig_efmatchvector_);
   fChain->SetBranchAddress("ele_trig_efmatchvector.first", ele_trig_efmatchvector_first, &b_ele_trig_efmatchvector_first);
   fChain->SetBranchAddress("ele_trig_efmatchvector.second", ele_trig_efmatchvector_second, &b_ele_trig_efmatchvector_second);
   fChain->SetBranchAddress("ele_trig_efmatchdelr", &ele_trig_efmatchdelr, &b_ele_trig_efmatchdelr);
   fChain->SetBranchAddress("ele_MET_wet", ele_MET_wet, &b_ele_MET_wet);
   fChain->SetBranchAddress("ele_MET_wpx", ele_MET_wpx, &b_ele_MET_wpx);
   fChain->SetBranchAddress("ele_MET_wpy", ele_MET_wpy, &b_ele_MET_wpy);
   fChain->SetBranchAddress("ele_MET_status", ele_MET_status, &b_ele_MET_status);
   fChain->SetBranchAddress("ele_ptype", ele_ptype, &b_ele_ptype);
   fChain->SetBranchAddress("ele_porigin", ele_porigin, &b_ele_porigin);
   fChain->SetBranchAddress("ele_poutcome", ele_poutcome, &b_ele_poutcome);
   fChain->SetBranchAddress("nph", &nph, &b_nph);
   fChain->SetBranchAddress("ph_author", ph_author, &b_ph_author);
   fChain->SetBranchAddress("ph_eta", ph_eta, &b_ph_eta);
   fChain->SetBranchAddress("ph_phi", ph_phi, &b_ph_phi);
   fChain->SetBranchAddress("ph_etaclus", ph_etaclus, &b_ph_etaclus);
   fChain->SetBranchAddress("ph_phiclus", ph_phiclus, &b_ph_phiclus);
   fChain->SetBranchAddress("ph_et", ph_et, &b_ph_et);
   fChain->SetBranchAddress("ph_eclus", ph_eclus, &b_ph_eclus);
   fChain->SetBranchAddress("ph_isEM", ph_isEM, &b_ph_isEM);
   fChain->SetBranchAddress("ph_isgoodoq", ph_isgoodoq, &b_ph_isgoodoq);
   fChain->SetBranchAddress("ph_oq", ph_oq, &b_ph_oq);
   fChain->SetBranchAddress("ph_quality", ph_quality, &b_ph_quality);
   fChain->SetBranchAddress("ph_etcone20", ph_etcone20, &b_ph_etcone20);
   fChain->SetBranchAddress("ph_etcone30", ph_etcone30, &b_ph_etcone30);
   fChain->SetBranchAddress("ph_etcone40", ph_etcone40, &b_ph_etcone40);
   fChain->SetBranchAddress("ph_etcone20_cor", ph_etcone20_cor, &b_ph_etcone20_cor);
   fChain->SetBranchAddress("ph_etcone30_cor", ph_etcone30_cor, &b_ph_etcone30_cor);
   fChain->SetBranchAddress("ph_etcone40_cor", ph_etcone40_cor, &b_ph_etcone40_cor);
   fChain->SetBranchAddress("ph_ptcone20", ph_ptcone20, &b_ph_ptcone20);
   fChain->SetBranchAddress("ph_ptcone30", ph_ptcone30, &b_ph_ptcone30);
   fChain->SetBranchAddress("ph_ptcone40", ph_ptcone40, &b_ph_ptcone40);
   fChain->SetBranchAddress("ph_isRecovered", ph_isRecovered, &b_ph_isRecovered);
   fChain->SetBranchAddress("ph_convFlag", ph_convFlag, &b_ph_convFlag);
   fChain->SetBranchAddress("ph_rconv", ph_rconv, &b_ph_rconv);
   fChain->SetBranchAddress("ph_zconv", ph_zconv, &b_ph_zconv);
   fChain->SetBranchAddress("ph_etas2", ph_etas2, &b_ph_etas2);
   fChain->SetBranchAddress("ph_ethad1", ph_ethad1, &b_ph_ethad1);
   fChain->SetBranchAddress("ph_ethad", ph_ethad, &b_ph_ethad);
   fChain->SetBranchAddress("ph_e277", ph_e277, &b_ph_e277);
   fChain->SetBranchAddress("ph_e237", ph_e237, &b_ph_e237);
   fChain->SetBranchAddress("ph_e233", ph_e233, &b_ph_e233);
   fChain->SetBranchAddress("ph_weta2", ph_weta2, &b_ph_weta2);
   fChain->SetBranchAddress("ph_f1", ph_f1, &b_ph_f1);
   fChain->SetBranchAddress("ph_emax", ph_emax, &b_ph_emax);
   fChain->SetBranchAddress("ph_emaxs2", ph_emaxs2, &b_ph_emaxs2);
   fChain->SetBranchAddress("ph_demaxs1", ph_demaxs1, &b_ph_demaxs1);
   fChain->SetBranchAddress("ph_fside", ph_fside, &b_ph_fside);
   fChain->SetBranchAddress("ph_wstot", ph_wstot, &b_ph_wstot);
   fChain->SetBranchAddress("ph_ws3", ph_ws3, &b_ph_ws3);
   fChain->SetBranchAddress("ph_emins1", ph_emins1, &b_ph_emins1);
   fChain->SetBranchAddress("ph_etap", ph_etap, &b_ph_etap);
   fChain->SetBranchAddress("ph_deltaeta", ph_deltaeta, &b_ph_deltaeta);
   fChain->SetBranchAddress("ph_MET_wet", ph_MET_wet, &b_ph_MET_wet);
   fChain->SetBranchAddress("ph_MET_wpx", ph_MET_wpx, &b_ph_MET_wpx);
   fChain->SetBranchAddress("ph_MET_wpy", ph_MET_wpy, &b_ph_MET_wpy);
   fChain->SetBranchAddress("ph_MET_status", ph_MET_status, &b_ph_MET_status);
   fChain->SetBranchAddress("ph_ptype", ph_ptype, &b_ph_ptype);
   fChain->SetBranchAddress("ph_porigin", ph_porigin, &b_ph_porigin);
   fChain->SetBranchAddress("ph_poutcome", ph_poutcome, &b_ph_poutcome);
   fChain->SetBranchAddress("ntrk", &ntrk, &b_ntrk);
   fChain->SetBranchAddress("trk_pt", trk_pt, &b_trk_pt);
   fChain->SetBranchAddress("trk_eta", trk_eta, &b_trk_eta);
   fChain->SetBranchAddress("trk_phi", trk_phi, &b_trk_phi);
   fChain->SetBranchAddress("trk_m", trk_m, &b_trk_m);
   fChain->SetBranchAddress("trk_q", trk_q, &b_trk_q);
   fChain->SetBranchAddress("trk_mu", trk_mu, &b_trk_mu);
   fChain->SetBranchAddress("trk_nucone10", trk_nucone10, &b_trk_nucone10);
   fChain->SetBranchAddress("trk_nucone20", trk_nucone20, &b_trk_nucone20);
   fChain->SetBranchAddress("trk_nucone30", trk_nucone30, &b_trk_nucone30);
   fChain->SetBranchAddress("trk_nucone40", trk_nucone40, &b_trk_nucone40);
   fChain->SetBranchAddress("trk_ptcone10", trk_ptcone10, &b_trk_ptcone10);
   fChain->SetBranchAddress("trk_ptcone20", trk_ptcone20, &b_trk_ptcone20);
   fChain->SetBranchAddress("trk_ptcone30", trk_ptcone30, &b_trk_ptcone30);
   fChain->SetBranchAddress("trk_ptcone40", trk_ptcone40, &b_trk_ptcone40);
   fChain->SetBranchAddress("trk_vtx", trk_vtx, &b_trk_vtx);
   fChain->SetBranchAddress("trk_dxpv", trk_dxpv, &b_trk_dxpv);
   fChain->SetBranchAddress("trk_zxpv", trk_zxpv, &b_trk_zxpv);
   fChain->SetBranchAddress("trk_covdxpv", trk_covdxpv, &b_trk_covdxpv);
   fChain->SetBranchAddress("trk_nhitbl", trk_nhitbl, &b_trk_nhitbl);
   fChain->SetBranchAddress("trk_expectbl", trk_expectbl, &b_trk_expectbl);
   fChain->SetBranchAddress("trk_nhitpix", trk_nhitpix, &b_trk_nhitpix);
   fChain->SetBranchAddress("trk_nholespix", trk_nholespix, &b_trk_nholespix);
   fChain->SetBranchAddress("trk_ndeadsensorspix", trk_ndeadsensorspix, &b_trk_ndeadsensorspix);
   fChain->SetBranchAddress("trk_nhitsct", trk_nhitsct, &b_trk_nhitsct);
   fChain->SetBranchAddress("trk_nholessct", trk_nholessct, &b_trk_nholessct);
   fChain->SetBranchAddress("trk_ndeadsensorssct", trk_ndeadsensorssct, &b_trk_ndeadsensorssct);
   fChain->SetBranchAddress("trk_nhittrt", trk_nhittrt, &b_trk_nhittrt);
   fChain->SetBranchAddress("trk_nholestrt", trk_nholestrt, &b_trk_nholestrt);
   fChain->SetBranchAddress("trk_noutlierstrt", trk_noutlierstrt, &b_trk_noutlierstrt);
   fChain->SetBranchAddress("trk_ptype", trk_ptype, &b_trk_ptype);
   fChain->SetBranchAddress("trk_porigin", trk_porigin, &b_trk_porigin);
   fChain->SetBranchAddress("trk_poutcome", trk_poutcome, &b_trk_poutcome);
   fChain->SetBranchAddress("trk_pmatchprob", trk_pmatchprob, &b_trk_pmatchprob);
   fChain->SetBranchAddress("njet", &njet, &b_njet);
   fChain->SetBranchAddress("jet_type", jet_type, &b_jet_type);
   fChain->SetBranchAddress("jet_et", jet_et, &b_jet_et);
   fChain->SetBranchAddress("jet_pt", jet_pt, &b_jet_pt);
   fChain->SetBranchAddress("jet_eta", jet_eta, &b_jet_eta);
   fChain->SetBranchAddress("jet_phi", jet_phi, &b_jet_phi);
   fChain->SetBranchAddress("jet_m", jet_m, &b_jet_m);
   fChain->SetBranchAddress("jet_pt_em", jet_pt_em, &b_jet_pt_em);
   fChain->SetBranchAddress("jet_eta_em", jet_eta_em, &b_jet_eta_em);
   fChain->SetBranchAddress("jet_phi_em", jet_phi_em, &b_jet_phi_em);
   fChain->SetBranchAddress("jet_m_em", jet_m_em, &b_jet_m_em);
   fChain->SetBranchAddress("jet_eta_origin", jet_eta_origin, &b_jet_eta_origin);
   fChain->SetBranchAddress("jet_phi_origin", jet_phi_origin, &b_jet_phi_origin);
   fChain->SetBranchAddress("jet_m_origin", jet_m_origin, &b_jet_m_origin);
   fChain->SetBranchAddress("jet_eta_origin_em", jet_eta_origin_em, &b_jet_eta_origin_em);
   fChain->SetBranchAddress("jet_phi_origin_em", jet_phi_origin_em, &b_jet_phi_origin_em);
   fChain->SetBranchAddress("jet_m_origin_em", jet_m_origin_em, &b_jet_m_origin_em);
   fChain->SetBranchAddress("jet_gcwjes", jet_gcwjes, &b_jet_gcwjes);
   fChain->SetBranchAddress("jet_emjes", jet_emjes, &b_jet_emjes);
   fChain->SetBranchAddress("jet_isgood", jet_isgood, &b_jet_isgood);
   fChain->SetBranchAddress("jet_isbad", jet_isbad, &b_jet_isbad);
   fChain->SetBranchAddress("jet_isugly", jet_isugly, &b_jet_isugly);
   fChain->SetBranchAddress("jet_n90", jet_n90, &b_jet_n90);
   fChain->SetBranchAddress("jet_hecquality", jet_hecquality, &b_jet_hecquality);
   fChain->SetBranchAddress("jet_larquality", jet_larquality, &b_jet_larquality);
   fChain->SetBranchAddress("jet_averagelarqf", jet_averagelarqf, &b_jet_averagelarqf);
   fChain->SetBranchAddress("jet_negativee", jet_negativee, &b_jet_negativee);
   fChain->SetBranchAddress("jet_sumpttrk", jet_sumpttrk, &b_jet_sumpttrk);
   fChain->SetBranchAddress("jet_ntrk", jet_ntrk, &b_jet_ntrk);
   fChain->SetBranchAddress("jet_jvtxf", jet_jvtxf, &b_jet_jvtxf);
   fChain->SetBranchAddress("jet_jvf", jet_jvf, &b_jet_jvf);
   fChain->SetBranchAddress("jet_bchcorr", jet_bchcorr, &b_jet_bchcorr);
   fChain->SetBranchAddress("jet_bchcorrjet", jet_bchcorrjet, &b_jet_bchcorrjet);
   fChain->SetBranchAddress("jet_tgap3f", jet_tgap3f, &b_jet_tgap3f);
   fChain->SetBranchAddress("jet_time", jet_time, &b_jet_time);
   fChain->SetBranchAddress("jet_emf", jet_emf, &b_jet_emf);
   fChain->SetBranchAddress("jet_hecf", jet_hecf, &b_jet_hecf);
   fChain->SetBranchAddress("jet_fmax", jet_fmax, &b_jet_fmax);
   fChain->SetBranchAddress("jet_tagSV0", jet_tagSV0, &b_jet_tagSV0);
   fChain->SetBranchAddress("jet_tagSV1", jet_tagSV1, &b_jet_tagSV1);
   fChain->SetBranchAddress("jet_tagSV2", jet_tagSV2, &b_jet_tagSV2);
   fChain->SetBranchAddress("jet_tagJetProb", jet_tagJetProb, &b_jet_tagJetProb);
   fChain->SetBranchAddress("jet_tagIP1D", jet_tagIP1D, &b_jet_tagIP1D);
   fChain->SetBranchAddress("jet_tagIP2D", jet_tagIP2D, &b_jet_tagIP2D);
   fChain->SetBranchAddress("jet_tagIP3D", jet_tagIP3D, &b_jet_tagIP3D);
   fChain->SetBranchAddress("jet_tagJetFitterTag", jet_tagJetFitterTag, &b_jet_tagJetFitterTag);
   fChain->SetBranchAddress("jet_tagJetFitterCOMB", jet_tagJetFitterCOMB, &b_jet_tagJetFitterCOMB);
   fChain->SetBranchAddress("jet_tagJetFitterCOMBNN", jet_tagJetFitterCOMBNN, &b_jet_tagJetFitterCOMBNN);
   fChain->SetBranchAddress("jet_tagIP3DSV1", jet_tagIP3DSV1, &b_jet_tagIP3DSV1);
   fChain->SetBranchAddress("jet_tagSoftElectronTag", jet_tagSoftElectronTag, &b_jet_tagSoftElectronTag);
   fChain->SetBranchAddress("jet_tagSoftMuonTag", jet_tagSoftMuonTag, &b_jet_tagSoftMuonTag);
   fChain->SetBranchAddress("jet_vtx", jet_vtx, &b_jet_vtx);
   fChain->SetBranchAddress("jet_js_tau1", jet_js_tau1, &b_jet_js_tau1);
   fChain->SetBranchAddress("jet_js_tau2", jet_js_tau2, &b_jet_js_tau2);
   fChain->SetBranchAddress("jet_js_tau3", jet_js_tau3, &b_jet_js_tau3);
   fChain->SetBranchAddress("jet_js_width", jet_js_width, &b_jet_js_width);
   fChain->SetBranchAddress("jet_js_qw", jet_js_qw, &b_jet_js_qw);
   fChain->SetBranchAddress("jet_js_split12", jet_js_split12, &b_jet_js_split12);
   fChain->SetBranchAddress("jet_js_split23", jet_js_split23, &b_jet_js_split23);
   fChain->SetBranchAddress("jet_js_split34", jet_js_split34, &b_jet_js_split34);
   fChain->SetBranchAddress("jet_js_zcut12", jet_js_zcut12, &b_jet_js_zcut12);
   fChain->SetBranchAddress("jet_js_zcut23", jet_js_zcut23, &b_jet_js_zcut23);
   fChain->SetBranchAddress("jet_js_zcut34", jet_js_zcut34, &b_jet_js_zcut34);
   fChain->SetBranchAddress("jet_js_statshape0", jet_js_statshape0, &b_jet_js_statshape0);
   fChain->SetBranchAddress("jet_js_statshape1", jet_js_statshape1, &b_jet_js_statshape1);
   fChain->SetBranchAddress("jet_js_statshape2", jet_js_statshape2, &b_jet_js_statshape2);
   fChain->SetBranchAddress("jet_js_statshape5", jet_js_statshape5, &b_jet_js_statshape5);
   fChain->SetBranchAddress("jet_js_statshape8", jet_js_statshape8, &b_jet_js_statshape8);
   fChain->SetBranchAddress("jet_js_pullmag", jet_js_pullmag, &b_jet_js_pullmag);
   fChain->SetBranchAddress("jet_js_pullphi", jet_js_pullphi, &b_jet_js_pullphi);
   fChain->SetBranchAddress("jet_js_pullc00", jet_js_pullc00, &b_jet_js_pullc00);
   fChain->SetBranchAddress("jet_js_pullc01", jet_js_pullc01, &b_jet_js_pullc01);
   fChain->SetBranchAddress("jet_js_pullc10", jet_js_pullc10, &b_jet_js_pullc10);
   fChain->SetBranchAddress("jet_js_pullc11", jet_js_pullc11, &b_jet_js_pullc11);
   fChain->SetBranchAddress("jet_MET_wet", jet_MET_wet, &b_jet_MET_wet);
   fChain->SetBranchAddress("jet_MET_wpx", jet_MET_wpx, &b_jet_MET_wpx);
   fChain->SetBranchAddress("jet_MET_wpy", jet_MET_wpy, &b_jet_MET_wpy);
   fChain->SetBranchAddress("jet_MET_status", jet_MET_status, &b_jet_MET_status);
   fChain->SetBranchAddress("met_reffinal", &met_reffinal, &b_met_reffinal);
   fChain->SetBranchAddress("met_reffinal_phi", &met_reffinal_phi, &b_met_reffinal_phi);
   fChain->SetBranchAddress("met_reffinal_sumet", &met_reffinal_sumet, &b_met_reffinal_sumet);
   fChain->SetBranchAddress("met_reffinal_em", &met_reffinal_em, &b_met_reffinal_em);
   fChain->SetBranchAddress("met_reffinal_em_phi", &met_reffinal_em_phi, &b_met_reffinal_em_phi);
   fChain->SetBranchAddress("met_reffinal_em_sumet", &met_reffinal_em_sumet, &b_met_reffinal_em_sumet);
   fChain->SetBranchAddress("met_refjet", &met_refjet, &b_met_refjet);
   fChain->SetBranchAddress("met_refjet_phi", &met_refjet_phi, &b_met_refjet_phi);
   fChain->SetBranchAddress("met_refjet_sumet", &met_refjet_sumet, &b_met_refjet_sumet);
   fChain->SetBranchAddress("met_cellout", &met_cellout, &b_met_cellout);
   fChain->SetBranchAddress("met_cellout_phi", &met_cellout_phi, &b_met_cellout_phi);
   fChain->SetBranchAddress("met_cellout_sumet", &met_cellout_sumet, &b_met_cellout_sumet);
   fChain->SetBranchAddress("met_softjets", &met_softjets, &b_met_softjets);
   fChain->SetBranchAddress("met_softjets_phi", &met_softjets_phi, &b_met_softjets_phi);
   fChain->SetBranchAddress("met_softjets_sumet", &met_softjets_sumet, &b_met_softjets_sumet);
   fChain->SetBranchAddress("met_cellout_eflow", &met_cellout_eflow, &b_met_cellout_eflow);
   fChain->SetBranchAddress("met_cellout_eflow_phi", &met_cellout_eflow_phi, &b_met_cellout_eflow_phi);
   fChain->SetBranchAddress("met_cellout_eflow_sumet", &met_cellout_eflow_sumet, &b_met_cellout_eflow_sumet);
   fChain->SetBranchAddress("met_cellout_eflow_muid", &met_cellout_eflow_muid, &b_met_cellout_eflow_muid);
   fChain->SetBranchAddress("met_cellout_eflow_muid_phi", &met_cellout_eflow_muid_phi, &b_met_cellout_eflow_muid_phi);
   fChain->SetBranchAddress("met_cellout_eflow_muid_sumet", &met_cellout_eflow_muid_sumet, &b_met_cellout_eflow_muid_sumet);
   fChain->SetBranchAddress("met_cryo", &met_cryo, &b_met_cryo);
   fChain->SetBranchAddress("met_cryo_phi", &met_cryo_phi, &b_met_cryo_phi);
   fChain->SetBranchAddress("met_cryo_sumet", &met_cryo_sumet, &b_met_cryo_sumet);
   fChain->SetBranchAddress("met_refmuon", &met_refmuon, &b_met_refmuon);
   fChain->SetBranchAddress("met_refmuon_phi", &met_refmuon_phi, &b_met_refmuon_phi);
   fChain->SetBranchAddress("met_refmuon_sumet", &met_refmuon_sumet, &b_met_refmuon_sumet);
   fChain->SetBranchAddress("met_refmuon_track", &met_refmuon_track, &b_met_refmuon_track);
   fChain->SetBranchAddress("met_refmuon_track_phi", &met_refmuon_track_phi, &b_met_refmuon_track_phi);
   fChain->SetBranchAddress("met_refmuon_track_sumet", &met_refmuon_track_sumet, &b_met_refmuon_track_sumet);
   fChain->SetBranchAddress("met_muonboy", &met_muonboy, &b_met_muonboy);
   fChain->SetBranchAddress("met_muonboy_phi", &met_muonboy_phi, &b_met_muonboy_phi);
   fChain->SetBranchAddress("met_muonboy_sumet", &met_muonboy_sumet, &b_met_muonboy_sumet);
   fChain->SetBranchAddress("met_muid", &met_muid, &b_met_muid);
   fChain->SetBranchAddress("met_muid_phi", &met_muid_phi, &b_met_muid_phi);
   fChain->SetBranchAddress("met_muid_sumet", &met_muid_sumet, &b_met_muid_sumet);
   fChain->SetBranchAddress("met_topo", &met_topo, &b_met_topo);
   fChain->SetBranchAddress("met_topo_phi", &met_topo_phi, &b_met_topo_phi);
   fChain->SetBranchAddress("met_topo_sumet", &met_topo_sumet, &b_met_topo_sumet);
   fChain->SetBranchAddress("met_topo_eta45", &met_topo_eta45, &b_met_topo_eta45);
   fChain->SetBranchAddress("met_topo_eta45_phi", &met_topo_eta45_phi, &b_met_topo_eta45_phi);
   fChain->SetBranchAddress("met_topo_eta45_sumet", &met_topo_eta45_sumet, &b_met_topo_eta45_sumet);
   fChain->SetBranchAddress("met_lochadtopo", &met_lochadtopo, &b_met_lochadtopo);
   fChain->SetBranchAddress("met_lochadtopo_phi", &met_lochadtopo_phi, &b_met_lochadtopo_phi);
   fChain->SetBranchAddress("met_lochadtopo_sumet", &met_lochadtopo_sumet, &b_met_lochadtopo_sumet);
   fChain->SetBranchAddress("met_lochadtopo_eta45", &met_lochadtopo_eta45, &b_met_lochadtopo_eta45);
   fChain->SetBranchAddress("met_lochadtopo_eta45_phi", &met_lochadtopo_eta45_phi, &b_met_lochadtopo_eta45_phi);
   fChain->SetBranchAddress("met_lochadtopo_eta45_sumet", &met_lochadtopo_eta45_sumet, &b_met_lochadtopo_eta45_sumet);
   fChain->SetBranchAddress("met_reffinal_eta45", &met_reffinal_eta45, &b_met_reffinal_eta45);
   fChain->SetBranchAddress("met_reffinal_eta45_phi", &met_reffinal_eta45_phi, &b_met_reffinal_eta45_phi);
   fChain->SetBranchAddress("met_reffinal_eta45_sumet", &met_reffinal_eta45_sumet, &b_met_reffinal_eta45_sumet);
   fChain->SetBranchAddress("met_muon", &met_muon, &b_met_muon);
   fChain->SetBranchAddress("met_muon_phi", &met_muon_phi, &b_met_muon_phi);
   fChain->SetBranchAddress("met_muon_sumet", &met_muon_sumet, &b_met_muon_sumet);
   fChain->SetBranchAddress("met_muon_muid", &met_muon_muid, &b_met_muon_muid);
   fChain->SetBranchAddress("met_muon_muid_phi", &met_muon_muid_phi, &b_met_muon_muid_phi);
   fChain->SetBranchAddress("met_muon_muid_sumet", &met_muon_muid_sumet, &b_met_muon_muid_sumet);
   fChain->SetBranchAddress("met_refele", &met_refele, &b_met_refele);
   fChain->SetBranchAddress("met_refele_phi", &met_refele_phi, &b_met_refele_phi);
   fChain->SetBranchAddress("met_refele_sumet", &met_refele_sumet, &b_met_refele_sumet);
   fChain->SetBranchAddress("met_reftau", &met_reftau, &b_met_reftau);
   fChain->SetBranchAddress("met_reftau_phi", &met_reftau_phi, &b_met_reftau_phi);
   fChain->SetBranchAddress("met_reftau_sumet", &met_reftau_sumet, &b_met_reftau_sumet);
   fChain->SetBranchAddress("met_refgamma", &met_refgamma, &b_met_refgamma);
   fChain->SetBranchAddress("met_refgamma_phi", &met_refgamma_phi, &b_met_refgamma_phi);
   fChain->SetBranchAddress("met_refgamma_sumet", &met_refgamma_sumet, &b_met_refgamma_sumet);
   fChain->SetBranchAddress("met_plhc", &met_plhc, &b_met_plhc);
   fChain->SetBranchAddress("met_plhc_phi", &met_plhc_phi, &b_met_plhc_phi);
   fChain->SetBranchAddress("met_plhc_sumet", &met_plhc_sumet, &b_met_plhc_sumet);
   fChain->SetBranchAddress("met_ichep", &met_ichep, &b_met_ichep);
   fChain->SetBranchAddress("met_ichep_phi", &met_ichep_phi, &b_met_ichep_phi);
   fChain->SetBranchAddress("met_ichep_sumet", &met_ichep_sumet, &b_met_ichep_sumet);
   fChain->SetBranchAddress("met_truth", &met_truth, &b_met_truth);
   fChain->SetBranchAddress("met_truth_phi", &met_truth_phi, &b_met_truth_phi);
   fChain->SetBranchAddress("met_truth_sumet", &met_truth_sumet, &b_met_truth_sumet);
   fChain->SetBranchAddress("met_track", &met_track, &b_met_track);
   fChain->SetBranchAddress("met_track_phi", &met_track_phi, &b_met_track_phi);
   fChain->SetBranchAddress("met_track_sumet", &met_track_sumet, &b_met_track_sumet);
   fChain->SetBranchAddress("nmc", &nmc, &b_nmc);
   fChain->SetBranchAddress("mc_weight", &mc_weight, &b_mc_weight);
   fChain->SetBranchAddress("mc_hfor", &mc_hfor, &b_mc_hfor);
   fChain->SetBranchAddress("mc_channel", &mc_channel, &b_mc_channel);
   fChain->SetBranchAddress("mc_pdgid", mc_pdgid, &b_mc_pdgid);
   fChain->SetBranchAddress("mc_m", mc_m, &b_mc_m);
   fChain->SetBranchAddress("mc_e", mc_e, &b_mc_e);
   fChain->SetBranchAddress("mc_pt", mc_pt, &b_mc_pt);
   fChain->SetBranchAddress("mc_phi", mc_phi, &b_mc_phi);
   fChain->SetBranchAddress("mc_eta", mc_eta, &b_mc_eta);
   fChain->SetBranchAddress("mc_parent", mc_parent, &b_mc_parent);
   fChain->SetBranchAddress("mc_status", mc_status, &b_mc_status);
   fChain->SetBranchAddress("mc_ptype", mc_ptype, &b_mc_ptype);
   fChain->SetBranchAddress("mc_porigin", mc_porigin, &b_mc_porigin);
   fChain->SetBranchAddress("mc_poutcome", mc_poutcome, &b_mc_poutcome);
   fChain->SetBranchAddress("mc_pmotherpdg", mc_pmotherpdg, &b_mc_pmotherpdg);
   fChain->SetBranchAddress("mcevt_hardindex", &mcevt_hardindex, &b_mcevt_hardindex);
   fChain->SetBranchAddress("mcevt_pdfset", &mcevt_pdfset, &b_mcevt_pdfset);
   fChain->SetBranchAddress("mcevt_weight", &mcevt_weight, &b_mcevt_weight);
   fChain->SetBranchAddress("mcevt_sigid", &mcevt_sigid, &b_mcevt_sigid);
   fChain->SetBranchAddress("mcevt_event_scale", &mcevt_event_scale, &b_mcevt_event_scale);
   fChain->SetBranchAddress("mcevt_pdf_scale", &mcevt_pdf_scale, &b_mcevt_pdf_scale);
   fChain->SetBranchAddress("mcevt_alphaQCD", &mcevt_alphaQCD, &b_mcevt_alphaQCD);
   fChain->SetBranchAddress("mcevt_alphaQED", &mcevt_alphaQED, &b_mcevt_alphaQED);
   fChain->SetBranchAddress("mcevt_id1", &mcevt_id1, &b_mcevt_id1);
   fChain->SetBranchAddress("mcevt_id2", &mcevt_id2, &b_mcevt_id2);
   fChain->SetBranchAddress("mcevt_pdf_x1", &mcevt_pdf_x1, &b_mcevt_pdf_x1);
   fChain->SetBranchAddress("mcevt_pdf_x2", &mcevt_pdf_x2, &b_mcevt_pdf_x2);
   fChain->SetBranchAddress("mcevt_pdf1", &mcevt_pdf1, &b_mcevt_pdf1);
   fChain->SetBranchAddress("mcevt_pdf2", &mcevt_pdf2, &b_mcevt_pdf2);
   fChain->SetBranchAddress("njpsi", &njpsi, &b_njpsi);
   fChain->SetBranchAddress("jpsi_mass", jpsi_mass, &b_jpsi_mass);
   fChain->SetBranchAddress("jpsi_masserr", jpsi_masserr, &b_jpsi_masserr);
   fChain->SetBranchAddress("jpsi_phi", jpsi_phi, &b_jpsi_phi);
   fChain->SetBranchAddress("jpsi_pt", jpsi_pt, &b_jpsi_pt);
   fChain->SetBranchAddress("jpsi_y", jpsi_y, &b_jpsi_y);
   fChain->SetBranchAddress("jpsi_chisq", jpsi_chisq, &b_jpsi_chisq);
   fChain->SetBranchAddress("jpsi_tau", jpsi_tau, &b_jpsi_tau);
   fChain->SetBranchAddress("jpsi_tauerr", jpsi_tauerr, &b_jpsi_tauerr);
   fChain->SetBranchAddress("jpsi_costhetastar", jpsi_costhetastar, &b_jpsi_costhetastar);
   fChain->SetBranchAddress("jpsi_is_os", jpsi_is_os, &b_jpsi_is_os);
   fChain->SetBranchAddress("jpsi_mup", jpsi_mup, &b_jpsi_mup);
   fChain->SetBranchAddress("jpsi_mum", jpsi_mum, &b_jpsi_mum);
   fChain->SetBranchAddress("lha_cteq6ll", &lha_cteq6ll, &b_lha_cteq6ll);
   fChain->SetBranchAddress("lha_MRSTMCal", &lha_MRSTMCal, &b_lha_MRSTMCal);
   fChain->SetBranchAddress("lha_CT10", &lha_CT10, &b_lha_CT10);
   fChain->SetBranchAddress("lha_MSTW2008lo68cl", &lha_MSTW2008lo68cl, &b_lha_MSTW2008lo68cl);
   fChain->SetBranchAddress("lha_MSTW2008nlo68cl", &lha_MSTW2008nlo68cl, &b_lha_MSTW2008nlo68cl);
   fChain->SetBranchAddress("lha_NNPDF21_100", &lha_NNPDF21_100, &b_lha_NNPDF21_100);
   fChain->SetBranchAddress("lha_HERAPDF10_EIG", &lha_HERAPDF10_EIG, &b_lha_HERAPDF10_EIG);
   fChain->SetBranchAddress("lha_cteq66", &lha_cteq66, &b_lha_cteq66);
   Notify();
}

Bool_t Deleteme::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void Deleteme::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t Deleteme::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef Deleteme_cxx
