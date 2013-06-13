#include "TrigFTKAna/misc/UnfoldingHistogramTool.hpp"

#include <TFile.h>
#include <TString.h>
#include <TObjString.h>
#include <TObjArray.h>
#include <iostream>
#include <string>
#include <sstream>
#include <cassert>

/* recursive mkdir for TFiles */
void mkdir_r(TDirectory *d , const TString& path , bool verbose=false) {
  TObjArray* tokens = path.Tokenize("/");
  int nt = tokens->GetEntries();
  if(verbose) {
    std::cout << "mkdir: " << d->GetPath() << " " << path << " " << nt << std::endl;
  }
  TString dcur_name = ((TObjString*)tokens->At(0))->GetString();
  TDirectory *dcur = d->Get(dcur_name) ? (TDirectory*)d->Get(dcur_name) : d->mkdir(dcur_name);
  if(nt==1) {
    return;
  }
  else {
    TString path2 = ((TObjString*)tokens->At(1))->GetString();
    for(int i=2; i<nt; i++) path2 += TString("/")+((TObjString*)tokens->At(i))->GetString();
    mkdir_r(dcur,path2,verbose);
  }
}

UnfoldingHistogramTool::UnfoldingHistogramTool()
{
  m_is2D = false;
  m_unfoldingInY = false;
  m_Ntoys = 1;
  m_fidCorrNum1D.clear();
  m_fidCorrDenom1D.clear();
  m_effCorrNum1D.clear();
  m_effCorrDenom1D.clear();
  m_purityNum1D.clear();
  m_purityDenom1D.clear();
  m_stabilityDenom1D.clear();
  m_Response1D.clear();
  m_outfile = 0;
  m_outdir = 0;
  m_fidCorrNum2D.clear();
  m_fidCorrDenom2D.clear();
  m_effCorrNum2D.clear();
  m_effCorrDenom2D.clear();
  m_purityNum2D.clear();
  m_purityDenom2D.clear();
  m_stabilityDenom2D.clear();
  m_Response2D.clear();
}

void UnfoldingHistogramTool::write() {
  if(m_outdir) m_outdir->cd();
  for(unsigned int i=0; i<m_fidCorrNum1D.size(); i++) m_fidCorrNum1D[i]->Write();
  for(unsigned int i=0; i<m_fidCorrDenom1D.size(); i++) m_fidCorrDenom1D[i]->Write();
  for(unsigned int i=0; i<m_effCorrNum1D.size(); i++) m_effCorrNum1D[i]->Write();
  for(unsigned int i=0; i<m_effCorrDenom1D.size(); i++) m_effCorrDenom1D[i]->Write();
  for(unsigned int i=0; i<m_purityNum1D.size(); i++) m_purityNum1D[i]->Write();
  for(unsigned int i=0; i<m_purityDenom1D.size(); i++) m_purityDenom1D[i]->Write();
  for(unsigned int i=0; i<m_stabilityDenom1D.size(); i++) m_stabilityDenom1D[i]->Write();
  for(unsigned int i=0; i<m_Response1D.size(); i++) m_Response1D[i]->Write();
  for(unsigned int i=0; i<m_fidCorrNum2D.size(); i++) m_fidCorrNum2D[i]->Write();
  for(unsigned int i=0; i<m_fidCorrDenom2D.size(); i++) m_fidCorrDenom2D[i]->Write();
  for(unsigned int i=0; i<m_effCorrNum2D.size(); i++) m_effCorrNum2D[i]->Write();
  for(unsigned int i=0; i<m_effCorrDenom2D.size(); i++) m_effCorrDenom2D[i]->Write();
  for(unsigned int i=0; i<m_purityNum2D.size(); i++) m_purityNum2D[i]->Write();
  for(unsigned int i=0; i<m_purityDenom2D.size(); i++) m_purityDenom2D[i]->Write();
  for(unsigned int i=0; i<m_stabilityDenom2D.size(); i++) m_stabilityDenom2D[i]->Write();
  for(unsigned int i=0; i<m_Response2D.size(); i++) m_Response2D[i]->Write();
  return;
}

UnfoldingHistogramTool::UnfoldingHistogramTool(TFile *fout, std::string dirName, unsigned int NbinsX, double bin_edgesX[], unsigned int Ntoys)
{
  if (!fout->IsOpen() || !fout->IsWritable()) {
    std::cerr << "[UnfoldingHistogramTool] ERROR: Output TFile is not open for writing." << std::endl;
    throw;
  }

  m_is2D = false;
  m_unfoldingInY = false;
  m_Ntoys = Ntoys;

  m_outfile = fout;
  m_outfile->cd();
  mkdir_r(m_outfile,TString(dirName));
  assert(m_outfile->Get(dirName.c_str()));
  m_outdir = (TDirectory*)(m_outfile->Get(dirName.c_str()));
  m_outfile->cd(dirName.c_str());

  m_fidCorrNum1D.reserve(Ntoys);
  m_fidCorrDenom1D.reserve(Ntoys);
  m_effCorrNum1D.reserve(Ntoys);
  m_effCorrDenom1D.reserve(Ntoys);
  m_purityNum1D.reserve(Ntoys);
  m_purityDenom1D.reserve(Ntoys);
  m_stabilityDenom1D.reserve(Ntoys);
  m_Response1D.reserve(Ntoys);

  for (unsigned int i = 0; i < Ntoys; i++) {

    std::stringstream index;
    if (Ntoys != 1) {
      index << i;
    }

    m_fidCorrNum1D.push_back(new TH1D(("fidCorrNum" + index.str()).c_str(), "Fiducial Correction Numerator 1D", NbinsX, bin_edgesX));
    m_fidCorrDenom1D.push_back(new TH1D(("fidCorrDenom" + index.str()).c_str(), "Fiducial Correction Denominator 1D", NbinsX, bin_edgesX));
    m_effCorrNum1D.push_back(new TH1D(("effCorrNum" + index.str()).c_str(), "Efficiency Correction Numerator 1D", NbinsX, bin_edgesX));
    m_effCorrDenom1D.push_back(new TH1D(("effCorrDenom" + index.str()).c_str(), "Efficiency Correction Denominator 1D", NbinsX, bin_edgesX));
    m_purityNum1D.push_back(new TH1D(("purityNum" + index.str()).c_str(), "Purity Numerator 1D", NbinsX, bin_edgesX));
    m_purityDenom1D.push_back(new TH1D(("purityDenom" + index.str()).c_str(), "Purity Denominator 1D", NbinsX, bin_edgesX));
    m_stabilityDenom1D.push_back(new TH1D(("stabilityDenom" + index.str()).c_str(), "Stability Denominator 1D", NbinsX, bin_edgesX));
    m_Response1D.push_back(new TH2D(("ResponseMatrix" + index.str()).c_str(), "Response Matrix 1D", NbinsX, bin_edgesX, NbinsX, bin_edgesX));
  

    m_fidCorrNum1D.back()->Sumw2();
    m_fidCorrDenom1D.back()->Sumw2();
    m_effCorrNum1D.back()->Sumw2();
    m_effCorrDenom1D.back()->Sumw2();
    m_purityNum1D.back()->Sumw2();
    m_purityDenom1D.back()->Sumw2();
    m_stabilityDenom1D.back()->Sumw2();
    m_Response1D.back()->Sumw2();
  }

  m_outfile->cd();

}

UnfoldingHistogramTool::UnfoldingHistogramTool(TFile *fout, std::string dirName, unsigned int NbinsX, double bin_edgesX[], unsigned int NbinsY, double bin_edgesY[], bool unfoldingInY, unsigned int Ntoys)
{
  if (!fout->IsOpen() || !fout->IsWritable()) {
    std::cerr << "[UnfoldingHistogramTool] ERROR: Output TFile is not open for writing." << std::endl;
    throw;
  }

  m_is2D = true;
  m_unfoldingInY = unfoldingInY;
  m_Ntoys = Ntoys;

  m_outfile = fout;
  m_outfile->cd();
  mkdir_r(m_outfile,TString(dirName));
  assert(m_outfile->Get(dirName.c_str()));
  m_outdir = (TDirectory*)(m_outfile->Get(dirName.c_str()));
  m_outfile->cd(dirName.c_str());

  m_fidCorrNum2D.reserve(Ntoys);
  m_fidCorrDenom2D.reserve(Ntoys);
  m_effCorrNum2D.reserve(Ntoys);
  m_effCorrDenom2D.reserve(Ntoys);
  m_purityNum2D.reserve(Ntoys);
  m_purityDenom2D.reserve(Ntoys);
  m_stabilityDenom2D.reserve(Ntoys);
  m_Response2D.reserve(Ntoys);

  for (unsigned int i = 0; i < Ntoys; i++) {

    std::stringstream index;
    if (Ntoys != 1) {
      index << i;
    }

    m_fidCorrNum2D.push_back(new TH2D(("fidCorrNum" + index.str()).c_str(), "Fiducial Correction Numerator 2D", NbinsX, bin_edgesX, NbinsY, bin_edgesY));
    m_fidCorrDenom2D.push_back(new TH2D(("fidCorrDenom" + index.str()).c_str(), "Fiducial Correction Denominator 2D", NbinsX, bin_edgesX, NbinsY, bin_edgesY));
    m_effCorrNum2D.push_back(new TH2D(("effCorrNum" + index.str()).c_str(), "Efficiency Correction Numerator 2D", NbinsX, bin_edgesX, NbinsY, bin_edgesY));
    m_effCorrDenom2D.push_back(new TH2D(("effCorrDenom" + index.str()).c_str(), "Efficiency Correction Denominator 2D", NbinsX, bin_edgesX, NbinsY, bin_edgesY));
    m_purityNum2D.push_back(new TH2D(("purityNum" + index.str()).c_str(), "Purity Numerator 2D", NbinsX, bin_edgesX, NbinsY, bin_edgesY));
    m_purityDenom2D.push_back(new TH2D(("purityDenom" + index.str()).c_str(), "Purity Denominator 2D", NbinsX, bin_edgesX, NbinsY, bin_edgesY));
    m_stabilityDenom2D.push_back(new TH2D(("stabilityDenom" + index.str()).c_str(), "Stability Denominator 2D", NbinsX, bin_edgesX, NbinsY, bin_edgesY));

    if (unfoldingInY) {
      m_Response2D.push_back(new TH3D(("ResponseMatrix" + index.str()).c_str(), "Response Matrix in Slices of X", NbinsY, bin_edgesY, NbinsY, bin_edgesY, NbinsX, bin_edgesX));
    } else {
      m_Response2D.push_back(new TH3D(("ResponseMatrix" + index.str()).c_str(), "Response Matrix in Slices of Y", NbinsX, bin_edgesX, NbinsX, bin_edgesX, NbinsY, bin_edgesY));
    }

    m_fidCorrNum2D.back()->Sumw2();
    m_fidCorrDenom2D.back()->Sumw2();
    m_effCorrNum2D.back()->Sumw2();
    m_effCorrDenom2D.back()->Sumw2();
    m_purityNum2D.back()->Sumw2();
    m_purityDenom2D.back()->Sumw2();
    m_stabilityDenom2D.back()->Sumw2();
    m_Response2D.back()->Sumw2();
  }

  m_outfile->cd();

}

UnfoldingHistogramTool::~UnfoldingHistogramTool()
{
}

#define T for(t=0; t<m_Ntoys; t++)

void UnfoldingHistogramTool::fill(bool isReco, const double& recoX, const std::vector<double>& recoWeight,
				  bool isTruthFid, const double& truthX, const std::vector<double>& truthWeight,
				  int nreplicas )
{
  unsigned int t;
  assert(nreplicas==m_Ntoys);
  const bool isSame = ( m_purityNum1D[0]->FindBin(recoX) == m_purityNum1D[0]->FindBin(truthX) );
  //  const Int_t bt = m_purityNum1D[0]->FindBin(truthX);
  //  const Int_t br = m_purityNum1D[0]->FindBin(recoX);
//   if (m_is2D) {
//     std::cerr << "[UnfoldingHistogramTool]: WARNING: Attempting to fill 1D histograms after calling 2D constructor." << std::endl;
//     return;
//   }
  
  T {
    
    if (isReco) {
      // Fiducial correction denominator
      m_fidCorrDenom1D[t]->Fill(recoX, recoWeight[t]);
      
      // Purity and stability calculation
      
      // purity denom is the same as fidCorrDenom, but include it for backwards compatibility
      m_purityDenom1D[t]->Fill(recoX, recoWeight[t]);
      
      // stability has the same numerator as the purity, but different denominator
      m_stabilityDenom1D[t]->Fill(truthX, recoWeight[t]);
      
      if (isSame) {
	m_purityNum1D[t]->Fill(recoX, recoWeight[t]);
      }
      
      if (isTruthFid) {
	// Efficiency and Fiducial correction numerators
	m_fidCorrNum1D[t]->Fill(recoX, recoWeight[t]);
	m_effCorrNum1D[t]->Fill(truthX, recoWeight[t]);
	
	m_Response1D[t]->Fill(recoX, truthX, recoWeight[t]);
      }
    }
    
    // Efficiency correction denominator
    if (isTruthFid) {
      m_effCorrDenom1D[t]->Fill(truthX, truthWeight[t]);
    }
  }
}

void UnfoldingHistogramTool::fill(bool isReco, const double& recoX, const double& recoY, const std::vector<double>& recoWeight,
				  bool isTruthFid, const double& truthX, const double& truthY, const std::vector<double>& truthWeight,
  				  int nreplicas )
{
  unsigned int t;
  assert(nreplicas==m_Ntoys);
//   if (!m_is2D) {
//     std::cerr << "[UnfoldingHistogramTool]: WARNING: Attempting to fill 2D histograms after calling 1D constructor." << std::endl;
//     return;
//   }

  // check whether both truth and reco are in the same fiducial slice (in the variable we are not unfolding in)
  bool isSameSlice = m_unfoldingInY ?
    (m_fidCorrNum2D[0]->GetXaxis()->FindBin(recoX) == m_fidCorrNum2D[0]->GetXaxis()->FindBin(truthX)) :
    (m_fidCorrNum2D[0]->GetYaxis()->FindBin(recoY) == m_fidCorrNum2D[0]->GetYaxis()->FindBin(truthY));

  bool isSameUnfoldingBin = m_unfoldingInY ?
    (m_fidCorrNum2D[0]->GetYaxis()->FindBin(recoY) == m_fidCorrNum2D[0]->GetYaxis()->FindBin(truthY)) : 
    (m_fidCorrNum2D[0]->GetXaxis()->FindBin(recoX) == m_fidCorrNum2D[0]->GetXaxis()->FindBin(truthX));

  T {
  if (isReco) {
    // Fiducial correction denominator
    m_fidCorrDenom2D[t]->Fill(recoX, recoY, recoWeight[t]);

    // Purity and stability calculation

    // purity denom is the same as fidCorrDenom, but include it for backwards compatibility    
    m_purityDenom2D[t]->Fill(recoX, recoY, recoWeight[t]);

    // stability has the same numerator as the purity, but different denominator
    m_stabilityDenom2D[t]->Fill(truthX, truthY, recoWeight[t]);

    if (isSameUnfoldingBin && isSameSlice) {
      m_purityNum2D[t]->Fill(recoX, recoY, recoWeight[t]);
    }
    
    // require both reco and truth to be in the same fiducial slice
    if (isTruthFid && isSameSlice) { 
      
      // Efficiency and Fiducial correction numerators
      m_fidCorrNum2D[t]->Fill(recoX, recoY, recoWeight[t]);
      m_effCorrNum2D[t]->Fill(truthX, truthY, recoWeight[t]);

      if (m_unfoldingInY) {
	m_Response2D[t]->Fill(recoY, truthY, truthX, recoWeight[t]);
      } else {
	m_Response2D[t]->Fill(recoX, truthX, truthY, recoWeight[t]);
      }
    }
  }

  // Efficiency correction denominator
  if (isTruthFid) {
    m_effCorrDenom2D[t]->Fill(truthX, truthY, truthWeight[t]);
  }
  }
}

#undef T
