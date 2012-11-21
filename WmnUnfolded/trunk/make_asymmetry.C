// RUN IT LIKE THIS:
// root -b -q  make_asymmetry.C

#include <iostream>
#include <TFile.h>
#include <TH1.h>
#include <TString.h>
#include <TObjString.h>
#include <TObjArray.h>

// full path to the folder where WMinus and WPlus root files are located
TString files_dir("/afs/cern.ch/user/m/mbellomo/public/WmnUnfolded/");
TString fname_neg(files_dir+"EWUnfolding.WMinusMuNu_LepEta_PowhegPythia_data_bkgunc.root");
TString fname_pos(files_dir+"EWUnfolding.WPlusMuNu_LepEta_PowhegPythia_data_bkgunc.root");
// output file name (by default, saved in $PWD)
TString fname_out("EWUnfolding.WAsymMuNu_LepEta_PowhegPythia_data_bkgunc.root");

// returns asymmetry (N+ - N-)/(N+ + N-)
Double_t asym_value(const Double_t& Np,const Double_t& Nm) {
  return Np+Nm>0 ? (Np-Nm)/(Np+Nm) : 0;
}
// Error propagation formula for asymmetry (N+ - N-)/(N+ + N-)
Double_t asym_error(const Double_t& Np, const Double_t& sNp, const Double_t& Nm, const Double_t& sNm) {
  const Double_t t = pow(Np+Nm,2);
  if (t==0) return 0;
  const Double_t res2 = pow(2*Nm/t,2) * pow(sNp,2) + pow(2*Np/t,2) * pow(sNm,2);
  return res2>0 ? sqrt(res2) : 0;
}
// Converts hplus and hminus histograms into an asymmetry with correct errors
TH1* WAsymmetry(TH1* hplus, TH1* hminus, std::string suffix="") {
  assert(hplus);
  assert(hminus);
  std::string newname = std::string(hplus->GetName())+suffix;
  TH1* hsum = (TH1*) hplus->Clone(newname.c_str());
  hsum->Reset();
  // bin0 = underflow; bin (nbins+1) = overflow
  for(int i=0; i<=hsum->GetNbinsX()+1; i++) {
    const Double_t Np  = hplus->GetBinContent(i);
    const Double_t sNp = hplus->GetBinError(i);
    const Double_t Nm  = hminus->GetBinContent(i);
    const Double_t sNm = hminus->GetBinError(i);
    hsum->SetBinContent( i , asym_value(Np,Nm) );
    hsum->SetBinError( i , asym_error(Np,sNp,Nm,sNm) );
  }
  return hsum;
}
// returns base directory. E.g., Result/Nominal/Nominal_Unfolded  -> Result/Nominal
TString basedir(const TString& path) {
  TString res="";
  TObjArray *T = path.Tokenize("/");
  for(int i=0;i<T->GetLast(); i++) {
    res += ((TObjString*)T->At(i))->String();
  }
  delete T;
  return res;
}

int make_asymmetry() {
  // input
  TFile *fpos = TFile::Open(fname_pos,"READ"); assert(fpos->IsOpen());
  TFile *fneg = TFile::Open(fname_neg,"READ"); assert(fneg->IsOpen());
  // output
  TFile *fout = TFile::Open(fname_out,"RECREATE"); assert(fout->IsOpen());
  fout->cd();
  int ih=0;
  TH1* H[1000];
  // The following macro creates and saves asymmetry curves for each systematic
#define ASYM(path) \
  H[ih] = WAsymmetry((TH1*)fpos->Get(path),(TH1*)fneg->Get(path));	\
  {									\
    TString base = basedir(path);					\
    if( base!="" && !fout->Get(base) ) fout->mkdir(base);		\
    fout->cd(base);							\
  }									\
  H[ih]->Write();							\
  ih++
  ASYM("Nominal/Nominal_Unfolded");
  ASYM("Nominal/Nominal_MCTruth");
  ASYM("Nominal/Nominal_MCReco");
  ASYM("Systematic/Systematic_JetResolDown_Unfolded");
  ASYM("Systematic/Systematic_JetResolUp_Unfolded");
  ASYM("Systematic/Systematic_JetScaleDown_Unfolded");
  ASYM("Systematic/Systematic_JetScaleUp_Unfolded");
  ASYM("Systematic/Systematic_MuonCScaleDown_Unfolded");
  ASYM("Systematic/Systematic_MuonCScaleUp_Unfolded");
  ASYM("Systematic/Systematic_MuonIsoSFDown_Unfolded");
  ASYM("Systematic/Systematic_MuonIsoSFUp_Unfolded");
  ASYM("Systematic/Systematic_MuonKScaleDown_Unfolded");
  ASYM("Systematic/Systematic_MuonKScaleUp_Unfolded");
  ASYM("Systematic/Systematic_MuonRecoSFDown_Unfolded");
  ASYM("Systematic/Systematic_MuonRecoSFUp_Unfolded");
  ASYM("Systematic/Systematic_MuonResIDDown_Unfolded");
  ASYM("Systematic/Systematic_MuonResIDUp_Unfolded");
  ASYM("Systematic/Systematic_MuonResMSDown_Unfolded");
  ASYM("Systematic/Systematic_MuonResMSUp_Unfolded");
  ASYM("Systematic/Systematic_MuonTriggerSFDown_Unfolded");
  ASYM("Systematic/Systematic_MuonTriggerSFUp_Unfolded");
  ASYM("Systematic/Systematic_ResoSoftTerms_ptHardDown_Unfolded");
  ASYM("Systematic/Systematic_ResoSoftTerms_ptHardUp_Unfolded");
  ASYM("Systematic/Systematic_ScaleSoftTerms_ptHardDown_Unfolded");
  ASYM("Systematic/Systematic_ScaleSoftTerms_ptHardUp_Unfolded");
#undef ASYM
  return 0;
}
