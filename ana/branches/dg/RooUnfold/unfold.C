#include <iostream>
#include <string>
#include <cassert>
#include <vector>
#include "src/RooUnfoldResponse.h"
#include "src/RooUnfoldBayes.h"
#include "src/RooUnfoldBinByBin.h"
#include "src/RooUnfoldSvd.h"
#include <TH1F.h>
#include <TString.h>
#include <TFile.h>
#include <TSystem.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TMath.h>

const char *input = "/share/t3data3/antonk/ana/ana_v29D_04112012_DtoM_64bit_unfold1d_etaslices_numet_stacoCB_all/mc_pythia_wmunu/root_mc_pythia_wmunu.root";
TFile *finput = new TFile(input,"READ");
/* global flag controlling what's returned:
   0=asym, 1=POS(tru/unf), 2=NEG(tru/unf)
          10=POS(det)    ,20=NEG(det)
*/
int what = 0;


// returns (N+ - N-)/(N+ + N-)
double asym_value(const double& Np,const double& Nm) {
  return Np+Nm>0 ? (Np-Nm)/(Np+Nm) : 0;
}
// Error propagation formula for (N+ - N-)/(N+ + N-)
double asym_error(const double& Np, const double& sNp, const double& Nm, const double& sNm) {
  const double t = pow(Np+Nm,2);
  if (t==0) return 0;
  const double res2 = pow(2*Nm/t,2) * pow(sNp,2) + pow(2*Np/t,2) * pow(sNm,2);
  return res2>0 ? sqrt(res2) : 0;
}
// (W+ - W-)/(W+ + W-) workhorse function with correct errors
TH1* WAsymmetry(const TH1* hplus,const TH1* hminus, std::string suffix="_asymmetry",bool zero_errors=false) {
  if(what!=0) assert(false);
  std::string newname = std::string(hplus->GetName())+suffix;
  TH1* hsum = (TH1*) (hplus->Clone(newname.c_str()));
  hsum->Reset();
  // bin0 = underflow; bin (nbins+1) = overflow
  for(int i=0; i<=hsum->GetNbinsX()+1; i++) {
    const double Np  = hplus->GetBinContent(i);
    const double sNp = hplus->GetBinError(i);
    const double Nm  = hminus->GetBinContent(i);
    const double sNm = hminus->GetBinError(i);
    if(false) {
      std::cout.unsetf(std::ios::floatfield);
      std::cout.precision(5);
      std::cout << "AKK: bin " << i << " : " << int(Np) << " " << int(sNp) << " " << int(Nm) << " " << int(sNm)
		<< " " << asym_value(Np,Nm) << " , " << asym_error(Np,sNp,Nm,sNm) << std::endl;
    }
    hsum->SetBinContent( i , asym_value(Np,Nm) );
    hsum->SetBinError( i , zero_errors ? 0 : asym_error(Np,sNp,Nm,sNm) );
  }
  return hsum;
}
/* takes an array of histograms in eta bins, projects in second variable (e.g., pt) back on the eta axis, 
   and makes final POS and NEG eta histos. Finally, returns an asymmetry */
TH1* WAsymmetry_inbins(TH1** hPOS, TH1** hNEG , int iptmin=1 , int iptmax=-1 ) {
  if(iptmax<0) iptmax = hPOS[0]->GetNbinsX();
  // bootstrap final abseta histogram
  TH1 *htmp = (TH1*)finput->Get(TString::Format("dg/%s/st_w_final/baseline/POS/lepton_absetav","nominal").Data());
  TH1 *rPOS = (TH1*) htmp->Clone("POS");
  TH1 *rNEG = (TH1*) htmp->Clone("NEG");
  rPOS->Reset();
  rNEG->Reset();
  // make abseta POS histogram
  for(int ieta=1;ieta<rPOS->GetNbinsX()+1; ieta++) { // loop over TH1 eta bins that we're going to set. no overflow/underflow!
    const int i = ieta-1; // array index for hPOS
    assert(hPOS[i]);
    float val = 0;
    float err2 = 0;
    for(int ipt=iptmin; ipt<=iptmax; ipt++) {
      assert(ipt <= hPOS[i]->GetNbinsX()+1);
      val += hPOS[i]->GetBinContent(ipt);
      err2 += pow(hPOS[i]->GetBinError(ipt),2);
    }
    rPOS->SetBinContent(ieta,val);
    rPOS->SetBinError(ieta,sqrt(err2));
  }
  // make abseta NEG histogram
  for(int ieta=1;ieta<rNEG->GetNbinsX()+1; ieta++) { // loop over TH1 eta bins that we're going to set
    const int i = ieta-1; // array index for hNEG
    assert(hNEG[i]);
    float val = 0;
    float err2 = 0;
    for(int ipt=iptmin; ipt<=iptmax; ipt++) {
      val += hNEG[i]->GetBinContent(ipt);
      err2 += pow(hNEG[i]->GetBinError(ipt),2);
    }
    rNEG->SetBinContent(ieta,val);
    rNEG->SetBinError(ieta,sqrt(err2));
  }
  if(what==1 || what==10) return rPOS;
  if(what==2 || what==20) return rNEG;
  return WAsymmetry(rPOS,rNEG);
}


TH1* truth_abseta() {
  TH1F *hPOS = (TH1F*)finput->Get("dg/truth/st_w_final/baseline/POS/lepton_absetav");
  assert(hPOS);
  if(what==1 || what==10) return hPOS;
  TH1F *hNEG = (TH1F*)finput->Get("dg/truth/st_w_final/baseline/NEG/lepton_absetav");
  assert(hNEG);
  if(what==2 || what==20) return hNEG;
  TH1 *res = WAsymmetry(hPOS,hNEG);
  return res;
}

// This version of truth-level plots is re-built from bin_%d
// The point is really to validate the bin_%d division here!
TH1* truth_inbins(const std::string& var="lpt", int iptmin=1, int iptmax=-1) {
  TH1 *hPOS[11];
  TH1 *hNEG[11];
  for(int i=0;i<11;i++) {
    hPOS[i] = (TH1*)finput->Get(TString::Format("dg/truth/st_w_final/baseline/POS/bin_%d/%s",i,var.c_str()).Data());
    assert(hPOS[i]);
    hNEG[i] = (TH1*)finput->Get(TString::Format("dg/truth/st_w_final/baseline/NEG/bin_%d/%s",i,var.c_str()).Data());
    assert(hNEG[i]);
  }
  return WAsymmetry_inbins(hPOS,hNEG,iptmin,iptmax);
}

/* General function that makes either a reco-level or unfolded asymmetry histogram */
TH1* unfold_abseta(bool do_unfold, int iresponse, const std::string& category , int unf_type=0 , int extra=4) {
  // This approximates "background-subtracted data", and is fixed to be "nominal".
  // The different variations are handled by running with different response matrices.
  TH1F *hPOS = (TH1F*)finput->Get(TString::Format("dg/%s/st_w_final/baseline/POS/lepton_absetav","nominal").Data());
  assert(hPOS);
  if(what==10) return hPOS;
  TH1F *hNEG = (TH1F*)finput->Get(TString::Format("dg/%s/st_w_final/baseline/NEG/lepton_absetav","nominal").Data());
  assert(hNEG);
  if(what==20) return hNEG;

  if(!do_unfold) return WAsymmetry(hPOS,hNEG);
  
  const char *pythia_unfold = "/share/t3data3/antonk/ana/ana_v29D_04112012_DtoM_64bit_unfold1d_etaslices_numet_stacoCB_all/unfold_pythia.root";
  const char *alpgen_unfold = "/share/t3data3/antonk/ana/ana_v29D_04112012_DtoM_64bit_unfold1d_etaslices_numet_stacoCB_all/unfold_alpgen.root";
  const char *mcnlo_unfold = "/share/t3data3/antonk/ana/ana_v29D_04112012_DtoM_64bit_unfold1d_etaslices_numet_stacoCB_all/unfold_mcnlo.root";
  const char *powheg_unfold = "/share/t3data3/antonk/ana/ana_v29D_04112012_DtoM_64bit_unfold1d_etaslices_numet_stacoCB_all/unfold_powheg.root";
  TFile *funfold;
  if(iresponse==0) funfold = new TFile(pythia_unfold,"READ");
  else if(iresponse==1) funfold = new TFile(alpgen_unfold,"READ");
  else if(iresponse==2) funfold = new TFile(mcnlo_unfold,"READ");
  else if(iresponse==3) funfold = new TFile(powheg_unfold,"READ");
  else assert(false);
  
  RooUnfoldResponse *r_POS = (RooUnfoldResponse*) funfold->Get(TString::Format("%s/POS/abseta",category.c_str()).Data());
  assert(r_POS);
  RooUnfoldResponse *r_NEG = (RooUnfoldResponse*) funfold->Get(TString::Format("%s/NEG/abseta",category.c_str()).Data());
  assert(r_NEG);

  RooUnfold *ru_POS;
  RooUnfold *ru_NEG;
  if(unf_type==0) {
    ru_POS = new RooUnfoldBayes(r_POS, hPOS, extra);
    ru_NEG = new RooUnfoldBayes(r_NEG, hNEG, extra);
  }
  else if(unf_type==1) {
    ru_POS = new RooUnfoldBinByBin(r_POS, hPOS);
    ru_NEG = new RooUnfoldBinByBin(r_NEG, hNEG);
  }
  else if(unf_type==2) {
    ru_POS = new RooUnfoldSvd(r_POS, hPOS, extra);
    ru_NEG = new RooUnfoldSvd(r_NEG, hNEG, extra);
  }
  else assert(0 && "Unsupported unfolding type");
  if(what==1) return ru_POS->Hreco();
  if(what==2) return ru_NEG->Hreco();
  TH1 *res = WAsymmetry(ru_POS->Hreco(),ru_NEG->Hreco());
  //unfold->PrintTable(std::cout, hTrue);

  return res;
}

/* Unfolding in 1d kinematic bins (lpt,wpt etc) */
TH1* unfold_inbins(bool do_unfold, int iresponse, const std::string& category , const std::string& var="lpt", int iptmin=1, int iptmax=-1, int unf_type=0 , int extra=4) {
  
  // This approximates "background-subtracted data", and is fixed to be "nominal".
  // The different variations are handled by running with different response matrices.
  TH1 *hPOS[11];
  TH1 *hNEG[11];
  for(int i=0;i<11;i++) {
    hPOS[i] = (TH1*)finput->Get(TString::Format("dg/%s/st_w_final/baseline/POS/bin_%d/%s","nominal",i,var.c_str()).Data());
    assert(hPOS[i]);
    hNEG[i] = (TH1*)finput->Get(TString::Format("dg/%s/st_w_final/baseline/NEG/bin_%d/%s","nominal",i,var.c_str()).Data());
    assert(hNEG[i]);
  }
  if((!do_unfold) || what==10 || what==20) return WAsymmetry_inbins(hPOS,hNEG,iptmin,iptmax);
  
  const char *pythia_unfold = "/share/t3data3/antonk/ana/ana_v29D_04112012_DtoM_64bit_unfold1d_etaslices_numet_stacoCB_all/unfold_pythia.root";
  const char *alpgen_unfold = "/share/t3data3/antonk/ana/ana_v29D_04112012_DtoM_64bit_unfold1d_etaslices_numet_stacoCB_all/unfold_alpgen.root";
  const char *mcnlo_unfold = "/share/t3data3/antonk/ana/ana_v29D_04112012_DtoM_64bit_unfold1d_etaslices_numet_stacoCB_all/unfold_mcnlo.root";
  const char *powheg_unfold = "/share/t3data3/antonk/ana/ana_v29D_04112012_DtoM_64bit_unfold1d_etaslices_numet_stacoCB_all/unfold_powheg.root";
  TFile *funfold;
  if(iresponse==0) funfold = new TFile(pythia_unfold,"READ");
  else if(iresponse==1) funfold = new TFile(alpgen_unfold,"READ");
  else if(iresponse==2) funfold = new TFile(mcnlo_unfold,"READ");
  else if(iresponse==3) funfold = new TFile(powheg_unfold,"READ");
  else assert(false);
    
  RooUnfoldResponse *r_POS[20];
  RooUnfoldResponse *r_NEG[20];
  for(int i=0; i<11; i++) {
    r_POS[i] = (RooUnfoldResponse*) funfold->Get(TString::Format("%s/POS/bin_%d/%s",category.c_str(),i,var.c_str()).Data());
    assert(r_POS[i]);
    r_NEG[i] = (RooUnfoldResponse*) funfold->Get(TString::Format("%s/NEG/bin_%d/%s",category.c_str(),i,var.c_str()).Data());
    assert(r_NEG[i]);
  }

  RooUnfold *ru_POS[20];
  RooUnfold *ru_NEG[20];
  TH1* hreco_POS[20];
  TH1* hreco_NEG[20];
  for(int i=0;i<11;i++) {
    if(unf_type==0) {
      ru_POS[i] = new RooUnfoldBayes(r_POS[i], hPOS[i], extra);
      ru_NEG[i] = new RooUnfoldBayes(r_NEG[i], hNEG[i], extra);
    }
    else if(unf_type==1) {
      ru_POS[i] = new RooUnfoldBinByBin(r_POS[i], hPOS[i]);
      ru_NEG[i] = new RooUnfoldBinByBin(r_NEG[i], hNEG[i]);
    }
    else if(unf_type==2) {
      ru_POS[i] = new RooUnfoldSvd(r_POS[i], hPOS[i], extra);
      ru_NEG[i] = new RooUnfoldSvd(r_NEG[i], hNEG[i], extra);
    }
    else assert(0 && "Unsupported unfolding type");
    hreco_POS[i] = ru_POS[i]->Hreco();
    hreco_NEG[i] = ru_NEG[i]->Hreco();
  }

  TH1 *res = WAsymmetry_inbins(hreco_POS,hreco_NEG,iptmin,iptmax);
  //unfold->PrintTable(std::cout, hTrue);

  return res;
}

int run01_truth_reco() {
  TH1* htruth = truth_abseta();
  htruth->SetTitle("Asymmetry at particle and detector level");
  htruth->SetLineColor(kBlack);
  TH1* hreco = unfold_abseta(false,0,"nominal",0);
  hreco->SetLineColor(kBlack+1);
  
  TCanvas *c = new TCanvas("c01","c01",800,600);
  c->cd();
  htruth->Draw();
  hreco->Draw("A SAME");

  TLegend *l = new TLegend(0.40,0.70,0.7,0.80,"Asymmetries","brNDC");
  l->AddEntry(htruth,"Particle level (Pythia)","L");
  l->AddEntry(hreco,"Detector level (Pythia)","L");

  if(what==0) htruth->GetYaxis()->SetRangeUser(0.0,0.5);
  htruth->GetYaxis()->SetTitle("Asymmetry");
  l->Draw("SAME");

  c->SaveAs("run01_truth_reco.png");
  return 0;
}

int run02_generators() {
  TH1* htruth = truth_abseta();
  htruth->SetTitle("Unfolded asymmetry from different response matrices");
  htruth->SetLineColor(kBlack);
  TH1* hpythia = unfold_abseta(true,0,"nominal");
  hpythia->SetLineColor(kBlack+1);
  TH1* halpgen = unfold_abseta(true,1,"nominal");
  halpgen->SetLineColor(kBlack+2);
  TH1* hmcnlo = unfold_abseta(true,2,"nominal");
  hmcnlo->SetLineColor(kBlack+3);
  TH1* hpowheg = unfold_abseta(true,3,"nominal");
  hpowheg->SetLineColor(kBlack+4);

  TCanvas *c = new TCanvas("c02","c02",800,600);
  c->cd();

  TLegend *l = new TLegend(0.20,0.70,0.7,0.90,"Asymmetries","brNDC");
  l->AddEntry(htruth,"Particle level (Pythia)","L");
  l->AddEntry(hpythia,"Unfolded from detector level (Pythia response)","L");
  l->AddEntry(halpgen,"Unfolded from detector level (Alpgen response)","L");
  l->AddEntry(hmcnlo,"Unfolded from detector level (MC@NLO response)","L");
  l->AddEntry(hpowheg,"Unfolded from detector level (Powheg response)","L");

  htruth->Draw();
  hpythia->Draw("A SAME");
  halpgen->Draw("A SAME");
  hmcnlo->Draw("A SAME");
  hpowheg->Draw("A SAME");
  if(what==0) {
    htruth->GetYaxis()->SetRangeUser(0.0,0.5);
    htruth->GetYaxis()->SetTitle("Asymmetry");
  }

  l->Draw("SAME");

  c->SaveAs("run02_generators.png");
  return 0;
}

int run03_methods() {
  TH1* htruth = truth_abseta();
  htruth->SetTitle("Unfolded asymmetry using different unfolding methods");
  htruth->SetLineColor(kBlack);
  // use alpgen matrix for this study

  TH1* h[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  // bin by bin
  h[0] = unfold_abseta(true,1,"nominal",1,4);
  // iterative
  h[1] = unfold_abseta(true,1,"nominal",0,4);
  h[2] = unfold_abseta(true,1,"nominal",0,2);
  h[3] = unfold_abseta(true,1,"nominal",0,10);
  // svd
  h[4] = unfold_abseta(true,1,"nominal",2,2);
  h[5] = unfold_abseta(true,1,"nominal",2,4);
  int color = 1;
  for(int i=0;i<10;i++) {
    if(h[i]) h[i]->SetLineColor(color);
    color++;
    if(color==11) color++;
  }

  TCanvas *c = new TCanvas("c03","c03",800,600);
  c->cd();

  TLegend *l = new TLegend(0.20,0.70,0.7,0.90,"Asymmetries","brNDC");
  l->AddEntry(htruth,"Particle level (Pythia)","L");
  l->AddEntry(h[0],"Unfolded (Alpgen response), bin-by-bin","L");

  l->AddEntry(h[2],"Unfolded (Alpgen response), iterative N=2","L");
  l->AddEntry(h[1],"Unfolded (Alpgen response), iterative N=4","L");
  l->AddEntry(h[3],"Unfolded (Alpgen response), iterative N=10","L");
  l->AddEntry(h[4],"Unfolded (Alpgen response), SVD tau=2","L");
  l->AddEntry(h[5],"Unfolded (Alpgen response), SVD tau=4","L");

  htruth->Draw();
  for(int i=0;i<6;i++) h[i]->Draw("A SAME");
  if(what==0) {
    htruth->GetYaxis()->SetRangeUser(0.0,0.5);
    htruth->GetYaxis()->SetTitle("Asymmetry");
  }

  l->Draw("SAME");

  c->SaveAs("run03_methods.png");

  return 0;
}

int run04_systematics() {
  TH1* htruth = truth_abseta();
  htruth->SetTitle("Unfolded asymmetry from different response matrices");
  htruth->SetLineColor(kBlack);
  TH1* sys[50];

  std::vector<std::string> unf_keys;
  unf_keys.push_back("nominal");
  unf_keys.push_back("nominal_effstatup");
  unf_keys.push_back("nominal_effstatdown");
  unf_keys.push_back("nominal_effsysup");
  unf_keys.push_back("nominal_effsysdown");
  unf_keys.push_back("nominal_trigstatup");
  unf_keys.push_back("nominal_trigstatdown");
  unf_keys.push_back("met_allcluup");
  unf_keys.push_back("met_allcludown");
  unf_keys.push_back("met_softjetup");
  unf_keys.push_back("met_softjetdown");
  unf_keys.push_back("jet_jer");
  unf_keys.push_back("jet_jesup");
  unf_keys.push_back("jet_jesdown");
  unf_keys.push_back("mcp_msup");
  unf_keys.push_back("mcp_msdown");
  unf_keys.push_back("mcp_idup");
  unf_keys.push_back("mcp_iddown");
  unf_keys.push_back("mcp_kup");
  unf_keys.push_back("mcp_kdown");
  unf_keys.push_back("mcp_cup");
  unf_keys.push_back("mcp_cdown");
  unf_keys.push_back("mcp_unscaled");

  int color = kBlack;
  for(unsigned int i=0;i<unf_keys.size();i++) {
    sys[i] = unfold_abseta(true,1,unf_keys[i]); // use alpgen response matrices to better approximate bg-subtracted data
    sys[i]->SetLineColor(color);
    color++;
    if(color==11) color++;
  }
  
  TCanvas *c = new TCanvas("c04","c04",800,600);
  c->cd();

  if(false){
    TLegend *l = new TLegend(0.20,0.70,0.7,0.90,"Asymmetries","brNDC");
    //l->AddEntry(htruth,"Particle level (Pythia)","L");
    l->Draw("SAME");
  }

  sys[0]->SetTitle("Systematic variations in the response matrix");
  sys[0]->Draw();
  for(unsigned int i=1;i<unf_keys.size();i++) {
    sys[i]->Draw("A SAME");
  }
  if(what==0) {
    sys[0]->GetYaxis()->SetRangeUser(0.0,0.5);
    sys[1]->GetYaxis()->SetTitle("Asymmetry");
  }

  c->SaveAs("run04_systematics.png");
  return 0;
}

int run10_reco_sanity_withoverflow() {
  TCanvas *c = new TCanvas("c10","c10",800,600);
  c->cd();
  TH1 *h = unfold_abseta(false,1, "nominal", 1,4); //alpgen
  h->SetTitle("Sanity check: re-building reco-level asymmetry from 11 eta slices (|eta| x lepton_pT)");
  h->SetLineColor(kBlack);
  TH1 *h2 = unfold_inbins(false,1, "nominal", "lpt",1,5, 1,4);
  h2->SetLineColor(kRed);

  h->Draw();
  h2->Draw("A SAME");

  if(what==0) {
    h->GetYaxis()->SetRangeUser(0.0,0.5);
    h->GetYaxis()->SetTitle("Asymmetry");
  }

  TLegend *l = new TLegend(0.20,0.70,0.7,0.90,"Asymmetries","brNDC");
  l->AddEntry(h,"Reco-level asymmetry from 1D |eta| histograms","L");
  l->AddEntry(h2,"Same, but re-built from 11 eta slices. 20<lepton_pT ","L");
  l->Draw();

  c->SaveAs("run10_reco_sanity_withoverflow.png");
  return 0;
}

int run11_reco_sanity_nooverflow() {
  TCanvas *c = new TCanvas("c11","c11",800,600);
  c->cd();
  TH1 *h = unfold_abseta(false,1, "nominal", 1,4); //alpgen
  h->SetTitle("Sanity check: re-building reco-level asymmetry from 11 eta slices (|eta| x lepton_pT)");
  h->SetLineColor(kBlack);
  TH1 *h2 = unfold_inbins(false,1, "nominal", "lpt",1,-1, 1,4);
  h2->SetLineColor(kRed);

  h->Draw();
  h2->Draw("A SAME");

  if(what==0) {
    h->GetYaxis()->SetRangeUser(0.0,0.5);
    h->GetYaxis()->SetTitle("Asymmetry");
  }

  TLegend *l = new TLegend(0.20,0.70,0.7,0.90,"Asymmetries","brNDC");
  l->AddEntry(h,"Reco-level asymmetry from 1D |eta| histograms","L");
  l->AddEntry(h2,"Same, but re-built from 11 eta slices. 20<lepton_pT<200 ","L");
  l->Draw();

  c->SaveAs("run11_reco_sanity_nooverflow.png");
  return 0;
}

int run12_generators(int ibin) {
  TH1* hpythia = unfold_inbins(true,0,"nominal","lpt",ibin,ibin, 1,4);
  hpythia->SetLineColor(kBlack+1);
  TH1* halpgen = unfold_inbins(true,1,"nominal","lpt",ibin,ibin, 1,4);
  halpgen->SetLineColor(kBlack+2);
  TH1* hmcnlo = unfold_inbins(true,2,"nominal","lpt",ibin,ibin, 1,4);
  hmcnlo->SetLineColor(kBlack+3);
  TH1* hpowheg = unfold_inbins(true,3,"nominal","lpt",ibin,ibin, 1,4);
  hpowheg->SetLineColor(kBlack+4);

  if(ibin==1) hpythia->SetTitle("Lepton pT = 20..25 ");
  else if(ibin==2) hpythia->SetTitle("Lepton pT = 25..40 ");
  else if(ibin==3) hpythia->SetTitle("Lepton pT = 40..80 ");
  else if(ibin==4) hpythia->SetTitle("Lepton pT = 80..200 ");

  TCanvas *c = new TCanvas(TString::Format("c12_%d",ibin).Data(),"c02",800,600);
  c->cd();

  TLegend *l = new TLegend(0.20,0.70,0.7,0.90,"Asymmetries","brNDC");
  l->AddEntry(hpythia,"Unfolded from detector level (Pythia response)","L");
  l->AddEntry(halpgen,"Unfolded from detector level (Alpgen response)","L");
  l->AddEntry(hmcnlo,"Unfolded from detector level (MC@NLO response)","L");
  l->AddEntry(hpowheg,"Unfolded from detector level (Powheg response)","L");

  hpythia->Draw();
  halpgen->Draw("A SAME");
  hmcnlo->Draw("A SAME");
  hpowheg->Draw("A SAME");
  if(what==0) {
    hpythia->GetYaxis()->SetRangeUser(0.0,0.5);
    hpythia->GetYaxis()->SetTitle("Asymmetry");
  }

  l->Draw("SAME");

  c->SaveAs("run02_generators.png");
  return 0;
}

int run13_unf(const std::string& pvar) {
  TCanvas *c = new TCanvas(TString::Format("c13_%s",pvar).Data(),"c13",800,600);
  c->cd();
  TH1 *h = unfold_abseta(true,1, "nominal", 1,4);
  h->SetTitle(("Unfolded asymmetry from 1D |eta| plots, VS from 11 eta slices (|eta| x "+pvar+")").c_str());
  h->SetLineColor(kBlack);
  TH1 *h2 = unfold_inbins(true,1, "nominal", pvar,1,-1, 1,4);
  h2->SetLineColor(kRed);

  h->Draw();
  h2->Draw("A SAME");

  if(what==0) {
    h->GetYaxis()->SetRangeUser(0.0,0.5);
    h->GetYaxis()->SetTitle("Asymmetry");
  }

  TLegend *l = new TLegend(0.20,0.70,0.7,0.90,"Asymmetries","brNDC");
  l->AddEntry(h,"Unfolded asymmetry from 1D eta histograms & response matrices","L");
  l->AddEntry(h2,("Same, but re-built from 11 eta slices. Response matrix is a function of"+pvar).c_str(),"L");
  l->Draw();
  c->SaveAs(("run13_unfold_in_bins_"+pvar+".png").c_str());
  return 0;
}

int debug(const std::string& pvar) {
  TCanvas *c = new TCanvas(TString::Format("c13_%s",pvar).Data(),"c13",800,600);
  c->cd();
  TH1 *htr_abseta = truth_abseta();
  htr_abseta->SetLineColor(kBlack);
  TH1 *htr_inbins = truth_inbins(pvar,1,5); //match!
  //TH1 *htr_inbins = truth_inbins(pvar,1,-1);
  htr_inbins->SetLineColor(kGray);
  TH1 *h = unfold_abseta(true,1, "nominal", 1,4);
  h->SetTitle(("Unfolded asymmetry from 1D |eta| plots, VS from 11 eta slices (|eta| x "+pvar+")").c_str());
  h->SetLineColor(kBlue);
  TH1 *h2 = unfold_inbins(true,1, "nominal", pvar,1,-1, 1,4);
  h2->SetLineColor(kRed);
  
  htr_abseta->Draw();
  htr_inbins->Draw("SAME");
  
  h->Draw("A SAME");
  h2->Draw("A SAME");

  if(what==0) {
    h->GetYaxis()->SetRangeUser(0.0,0.5);
    h->GetYaxis()->SetTitle("Asymmetry");
  }

  TLegend *l = new TLegend(0.20,0.70,0.7,0.90,"Asymmetries","brNDC");
  l->AddEntry(htr_abseta,"Truth 1D histogram","L");
  l->AddEntry(htr_inbins,"Truth in eta slices","L");
  l->AddEntry(h,"Unfolded from 1D histogram","L");
  l->AddEntry(h2,("Unfolded inside eta slices vs "+pvar).c_str(),"L");
  l->Draw();
  c->SaveAs(("debug_bins_"+pvar+".png").c_str());
  return 0;
}

int unfold() {
  gSystem->Load("libRooUnfold.so");
  run02_generators();
  
  if(false) {
    run01_truth_reco();
    run02_generators();
    run03_methods();
    run04_systematics();
    run10_reco_sanity_withoverflow();
    run11_reco_sanity_nooverflow();
    run12_generators(1);
    run12_generators(2);
    run12_generators(3);
  }
  if(false) {
    run13_unf("lpt");
    run13_unf("wpt");
    run13_unf("wmt");
    run13_unf("met");
  }
  //debug("nj25");
  return 0;
}
