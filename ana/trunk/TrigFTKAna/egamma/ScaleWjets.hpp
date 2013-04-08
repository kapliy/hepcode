#ifndef SCALEWJETS_H_
#define SCALEWJETS_H_

#include <vector>
#include <string>
#include <map>
#include "TLorentzVector.h"
#include "TF1.h"


// code lifted from here: https://twiki.cern.ch/twiki/bin/view/AtlasProtected/WjetsReweighting

using namespace std;

class ScaleWjets
{
 public:
  ScaleWjets();
  ScaleWjets(std::string variation);
  ~ScaleWjets();
  
  double getScale(TLorentzVector* vec, std::string obj, int jb);
  
  
 private:
  void initializeScalingFunctions();

  std::string _var;
  std::map<std::string, TF1*> _functions;
};




#endif
