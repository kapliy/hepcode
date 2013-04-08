/* 
 * Name: UnfoldingHistogramTool
 * Author: A. Lewis (adrian.lewis@cern.ch)
 * 
 * Very simple class to create and fill the set of MC input histograms to the EWUnfold package
 * These histograms can be used as an alternative to the n-tuples, which is the default input format.
 * The histograms that are filled corresponed to:
 *   - reco distribution
 *   - truth distribution
 *   - efficiency correction factors
 *   - fiducial correction factors
 *   - purity numbers
 *   - bin to bin migration matrix
 */

#ifndef _UNFOLDING_HISTOGRAM_TOOL_H_
#define _UNFOLDING_HISTOGRAM_TOOL_H_

#include <string>

#include <TFile.h>
#include <TDirectory.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TH3D.h>

class UnfoldingHistogramTool
{
public:
  // Constructors for unfolding histograms for 1-D and 2-D binning
  // fout must be an existing TFile that is open for writing. The user is responsible for writing and closing and  TFile after filling the histograms
  // dirName is the name of the directory that the unfolding histograms will be written to
  UnfoldingHistogramTool(TFile *fout, std::string dirName, unsigned int NbinsX, double bin_edgesX[], unsigned int Ntoys = 1);

  // If unfoldingInY is true, the response matrix will be filled for the unfolding of the Y variable, in slices of X
  UnfoldingHistogramTool(TFile *fout, std::string dirName, unsigned int NbinsX, double bin_edgesX[], unsigned int NbinsY, double bin_edgesY[], bool unfoldingInY = true, unsigned int Ntoys = 1);

  UnfoldingHistogramTool();
  ~UnfoldingHistogramTool();

  // Call this method once per event
  // if isReco or is TruthFid are false, the corresponding values and weights won't get filled so just pass a dummy argument e.g. truthX = 999.0
  void fill(bool isReco, double recoX, double recoWeight, bool isTruthFid, double truthX, double truthWeight, unsigned int toyIndex = 0);
  
  // The corresponding method for 2D histograms
  // Here the unfolding can be performed in X in slices of Y
  void fill(bool isReco, double recoX, double recoY, double recoWeight, bool isTruthFid, double truthX, double truthY, double truthWeight, unsigned int toyIndex = 0);

  // Dump histograms to file
  void write();

private:
  
  TFile * m_outfile;
  TDirectory * m_outdir;
  
  bool m_is2D;
  bool m_unfoldingInY;

  unsigned int m_Ntoys;

  // 1D unfolding

  std::vector<TH1D*> m_fidCorrNum1D;
  std::vector<TH1D*> m_fidCorrDenom1D;
  std::vector<TH1D*> m_effCorrNum1D;
  std::vector<TH1D*> m_effCorrDenom1D;
  std::vector<TH1D*> m_purityNum1D;
  std::vector<TH1D*> m_purityDenom1D;
  std::vector<TH1D*> m_stabilityDenom1D; // numerator is same as for purity
  std::vector<TH2D*> m_Response1D;

  // 2D unfolding

  std::vector<TH2D*> m_fidCorrNum2D;
  std::vector<TH2D*> m_fidCorrDenom2D;
  std::vector<TH2D*> m_effCorrNum2D;
  std::vector<TH2D*> m_effCorrDenom2D;
  std::vector<TH2D*> m_purityNum2D;
  std::vector<TH2D*> m_purityDenom2D;
  std::vector<TH2D*> m_stabilityDenom2D;

  // Response matrix of X in slices of Y
  std::vector<TH3D*> m_Response2D;
};

#endif /* _UNFOLDING_HISTOGRAM_TOOL_H_ */
