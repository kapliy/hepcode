#ifndef MET_Cleaning_Utils_V1_h
#define MET_Cleaning_Utils_V1_h

#include <vector>
#include "TLorentzVector.h"
#include "TrigFTKAna/jetetmiss/Top_MET-11-06-23.hpp"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// class MET_Cleaning_Utils
// Proves a standard interface to modify the missing ET in the TopInputs D3PD
// -enables modification fo the missing ET for default corrections to the selected objects and for studies of systematic uncertainties. 
// -implements the MET only uncertainties for the CellOut term and the SoftJet term.   
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class MET_Cleaning_Utils_v1 {

 public:
  MET_Cleaning_Utils_v1();
  virtual ~MET_Cleaning_Utils_v1();

  // Common funtions
  float DeltaPhi(float phi1, float phi2);
  float DeltaEta(float eta1, float eta2);
  float dr(float phi1, float eta1,float phi2, float eta2);

  //Cleaning Tools
  bool isJetSafe(std::vector<TLorentzVector> &jet, std::vector<float> *BCH_CORR_JET,
		 std::vector<float> *BCH_CORR_CELL, bool isdata, float shift);
  bool hasUnmatchTrackJet(std::vector<TLorentzVector> &jet,float JCone,std::vector<TLorentzVector> &trackjet,
			  float TJCone,float minpt);
  bool isJetAffected(std::vector<TLorentzVector> &jet, float iso, float JCone, float minpt );
  bool isJetAffected(TLorentzVector &jet, float iso, float JCone, float minpt );
  bool iswithinDeadFEB(float eta, float phi, float objCone, float objIso,bool dosquare=false);




  // Validation Tools
  float dphi_MET_MPT(float &met_phi,std::vector<float> *trk_pt,std::vector<float> *trk_eta,
		     std::vector<float> *trk_d0_wrtPV,std::vector<float> *trk_z0_wrtPV,
		     std::vector<float> *trk_theta_wrtPV,std::vector<float> *trk_phi_wrtPV,
		     std::vector<int> *trk_nPixHits,std::vector<int> *trk_nSCTHits);
  float MPT_Loose_phi(std::vector<float> *trk_pt,std::vector<float> *trk_eta,std::vector<float> *trk_d0_wrtPV,
		      std::vector<float> *trk_z0_wrtPV,std::vector<float> *trk_theta_wrtPV,
		      std::vector<float> *trk_phi_wrtPV,std::vector<int> *trk_nPixHits,
		      std::vector<int> *trk_nSCTHits );
  float dphi_Jet_MET(std::vector<TLorentzVector> &jet, float met_phi);
  float JetTrackJetRatio(std::vector<TLorentzVector> &jet,TLorentzVector &trackjet);
  
 private:
  std::vector<float> eta_min, eta_max, phi_min, phi_max;
  float GeV;
  float jet_THRESHOLD;
  float jet_THRESHOLD_1sigma;
  float jet_iso;
  Top_MET_v1 top_met;
};
#endif
