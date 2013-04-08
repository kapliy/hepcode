#ifndef __LARHOLE__
#define __LARHOLE__

/**
   
Purpose: LAr hole treatment

- Version 2.0 (2 December 2011):  
- Version 1.0 (23 June 2011):  based on SUSYTools-00-00-29 implementation

Usage: 
double threshold=40000;   //(40GeV threshold by default)
bool flag = LArHole::simpleVeto(...);
bool flag = LArHole::smartVeto(...);
//Remove event is flag = true 

*/


namespace LArHole
{
  
  
  bool IsLArHole(float eta, float phi,
		 float etamin = -0.1, float etamax = 1.5,
		 float phimin = -0.9, float phimax = -0.5)
  {
    if (eta < etamin || eta > etamax) return false;
    if (phi < phimin || phi > phimax) return false;
    return true;     
  }
  
  bool simpleVeto(float jet_pt, 
		     float jet_eta,                
		     float jet_phi,                
		     float jet_BCH_CORR_JET, 
		     float jet_BCH_CORR_CELL,
		     float threshold=40000.)
  {
    if (!IsLArHole(jet_eta,jet_phi)) return false;
    float pt = threshold;
    // Correct threshold cut by missing energy in LAr hole in data.
    // BCH_CORR_CELL is necessary to take into account dead tile module near LAr hole.
    pt = pt*(1. - jet_BCH_CORR_JET)/(1. - jet_BCH_CORR_CELL);
    if (jet_pt <=pt) return false;
    return true;
  }
  
  bool smartVeto(float jet_pt, 
		 float jet_eta,                
		 float jet_phi,                
		 float jet_BCH_CORR_JET, 
		 float jet_BCH_CORR_CELL,		
		 float MET, 
		 float MET_phi,
		 float threshold=10000.,
		 float frac=0.1)
  {
    if (!IsLArHole(jet_eta,jet_phi)) return false;
    double estimator=jet_pt*(1-jet_BCH_CORR_CELL)*(1/(1-jet_BCH_CORR_JET)-1);
    double estimator_parralel=estimator*cos(jet_phi-MET_phi);

    if(threshold>=0&& estimator_parralel>threshold ) return true;
    if(frac>=0 && estimator_parralel>MET*frac) return true;

    return false;
  }

}



#endif
