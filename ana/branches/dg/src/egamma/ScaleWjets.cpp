/*
 * author: Anna Henrichs (anna.henrichs@cern.ch)
 * author: Adam Roe (adam.roe@cern.ch)

 **** Notes *** 
 date: 06 May 2011 author: Adam Roe
 larger statistics truth samples used
 - iqopt3 and ptjmin20 are updated 
 - iqopt2 (flat scale) and ptjmin20 are thought to be poorly 
    motivated physically and should not be used. 
 - ktfac and qfac are found to affect normalization and not kinematics
     and are thus not updated here (would be flat) 
 - more statistics for the tunes are not yet available. 

 ** suggested use ** 

//once per variation
string variation = "ptjmin10"
ScaleWjets* sc = new ScaleWjets(variation);

//in event loop 
TLorentzVector *leadingjet = new TLorentzVector();

//use kinematics of leading selected jet 
leadingjet->SetPxPyPzE(finaljets[0].px, finaljets[0].py,finaljets[0].pz,finaljets[0].e);

//first argument is 4-vector of leading jet
//second argument should be "Jet" 
//third is the number of selected jets

Event_Weight *= sc->getScale(leadingjet, "Jet", finaljets.size() );

 */

#include <iostream>
#include <cmath>
#include <vector>

#include "TrigFTKAna/egamma/ScaleWjets.hpp"

#include "TLorentzVector.h"
#include "TF1.h"

using namespace std;


// code lifted from here: https://twiki.cern.ch/twiki/bin/view/AtlasProtected/WjetsReweighting

/*
 * default constructor
 */
ScaleWjets::ScaleWjets(){
  _var = "none";
}
ScaleWjets::ScaleWjets(std::string variation){
  _var = variation;
  initializeScalingFunctions();
}
/*
 * destructor
 */
ScaleWjets::~ScaleWjets(){

  
  std::map<std::string,TF1*>::iterator iter;
  for(iter = _functions.begin(); iter != _functions.end(); iter++){
    delete iter->second;
  }
  
  _functions.clear();

}

/*
 * function to scale the object (based on the leading jet for now)
 */
double ScaleWjets::getScale(TLorentzVector* vec, std::string obj, int jb){

  if(_var == "none") return 1;
  
  if( jb > 6){
    if( false ) std::cout<<"only have reweighting functions up to the 6th jetbin - which is inclusive - will use that one"<<std::endl;
    jb = 6;
  }

  char buffer[2];
  sprintf(buffer,"%d", jb);
  obj = obj+"_"+buffer;
  
  if(_functions.find(obj) == _functions.end()) return 1;
  double mod = _functions[obj]->Eval(vec->Pt());
  return mod;
}
void ScaleWjets::initializeScalingFunctions(){

  // here set the variations for the different variations
  if(_var == "iqopt2"){
    TF1* f1 = new TF1("Jet_1", "pol4");
    f1->SetParameters(1.04417,-0.00271923,3.89526e-05,-1.27925e-07,1.29249e-10); 
    _functions["Jet_1"] = f1; 
    
    TF1* f2 = new TF1("Jet_2", "pol3");
    f2->SetParameters(0.959637,-4.75419e-05,8.7002e-06,-1.40251e-08);
    _functions["Jet_2"] = f2; 
     
    TF1* f3 = new TF1("Jet_3", "pol3");
    f3->SetParameters(0.901935,0.000552405,4.92526e-06,-6.43912e-09); 
    _functions["Jet_3"] = f3; 
     
    TF1* f4 = new TF1("Jet_4", "pol4");
    f4->SetParameters(0.72606,0.00359715,-1.61699e-05,5.28696e-08,-6.05961e-11);
    _functions["Jet_4"] = f4;

    TF1* f5 = new TF1("Jet_5", "pol1");
    f5->SetParameters( 0.804679,0.00130292); 
    _functions["Jet_5"] = f5;

    TF1* f6 = new TF1("Jet_6", "pol3");
    f6->SetParameters(0.889621,-0.00105478,1.1196e-05,-1.51176e-08);
    _functions["Jet_6"] = f6; 
  }

  else if(_var == "iqopt3"){
    TF1* f1 = new TF1("Jet_1", "pol1");
    f1->SetParameters(1.01538,-0.000448452);
    _functions["Jet_1"] = f1;
    
    TF1* f2 = new TF1("Jet_2", "pol2");
    f2->SetParameters(0.992624,5.42143e-05,7.37113e-07);
    _functions["Jet_2"] = f2;
    
    TF1* f3 = new TF1("Jet_3", "pol1");
    f3->SetParameters(0.952514,0.000564514);
    _functions["Jet_3"] = f3;
    
    TF1* f4 = new TF1("Jet_4", "pol1");
    f4->SetParameters(0.918226,0.000671739);
    _functions["Jet_4"] = f4;

    TF1* f5 = new TF1("Jet_5", "pol1");
    f5->SetParameters(0.799767,0.00221036);
    _functions["Jet_5"] = f5;

    TF1* f6 = new TF1("Jet_6", "pol1");
    f6->SetParameters(1.0,0);
    _functions["Jet_6"] = f6;
  }

  else if(_var == "ktfac05"){

    TF1* f0 = new TF1("Jet_0", "pol1");
    f0->SetParameters(1.1, 0);
    _functions["Jet_0"] = f0;

    TF1* f1 = new TF1("Jet_1", "pol2");
    f1->SetParameters(1.02353,-0.0006833,1.27524e-06);
    _functions["Jet_1"] = f1;
    
    TF1* f2 = new TF1("Jet_2", "pol1");
    f2->SetParameters(1.0, 0);
    _functions["Jet_2"] = f2;
    
    TF1* f3 = new TF1("Jet_3", "pol1");
    f3->SetParameters(1.0, 0);
    _functions["Jet_3"] = f3;
    
    TF1* f4 = new TF1("Jet_4", "pol1");
    f4->SetParameters(1.0, 0);
    _functions["Jet_4"] = f4;
    
    TF1* f5 = new TF1("Jet_5", "pol1");
    f5->SetParameters(1.0, 0);
    _functions["Jet_5"] = f5;
    
    TF1* f6 = new TF1("Jet_6", "pol1");
    f6->SetParameters(1.0, 0);
    _functions["Jet_6"] = f6;
  }
  else if(_var == "ktfac20"){
    TF1* f0 = new TF1("Jet_0", "pol1");
    f0->SetParameters(1.1,0);
    _functions["Jet_0"] = f0;
    
    TF1* f1 = new TF1("Jet_1", "pol1");
    f1->SetParameters(1.041,-0.000437);
    _functions["Jet_1"] = f1;
    
    TF1* f2 = new TF1("Jet_2", "pol1");
    f2->SetParameters(1.0,0);
    _functions["Jet_2"] = f2;
    
    TF1* f3 = new TF1("Jet_3", "pol1");
    f3->SetParameters(1.0,0);
    _functions["Jet_3"] = f3;
    
    TF1* f4 = new TF1("Jet_4", "pol1");
    f4->SetParameters(1.0,0);
    _functions["Jet_4"] = f4;

    TF1* f5 = new TF1("Jet_5", "pol1");
    f5->SetParameters(1.0,0.0);
    _functions["Jet_5"] = f5;

    TF1* f6 = new TF1("Jet_6", "pol1");
    f6->SetParameters(1.0,0);
    _functions["Jet_6"] = f6;
  }

  else if(_var == "perugia_hard"){

    TF1* f0 = new TF1("Jet_0", "pol1");
    f0->SetParameters(0.9, 0);
    _functions["Jet_0"] = f0;

    TF1* f1 = new TF1("Jet_1", "pol2");
    f1->SetParameters(1.09995,-0.00239568,1.43849e-05,-2.43483e-08);
    _functions["Jet_1"] = f1;
    
    TF1* f2 = new TF1("Jet_2", "pol3");
    f2->SetParameters(0.862623,0.00375945,-3.07728e-05,6.03586e-08);
    _functions["Jet_2"] = f2;
    
    TF1* f3 = new TF1("Jet_3", "pol3");
    f3->SetParameters(0.988608,0.000989681,-1.23312e-05,2.78109e-08);
    _functions["Jet_3"] = f3;
    
    TF1* f4 = new TF1("Jet_4", "pol2");
    f4->SetParameters(1.12839,-0.00207875,5.21628e-06);
    _functions["Jet_4"] = f4;

    TF1* f5 = new TF1("Jet_5", "pol1");
    f5->SetParameters(1.02964,-0.000278417);
    _functions["Jet_5"] = f5;

    TF1* f6 = new TF1("Jet_6", "pol1");
    f6->SetParameters(1.03039,-0.000288288);
    _functions["Jet_6"] = f6;
  }
  else if(_var == "perugia"){
    
    TF1* f0 = new TF1("Jet_0", "pol1");
    f0->SetParameters(1.13653, 0);
    _functions["Jet_0"] = f0;

    TF1* f1 = new TF1("Jet_1", "pol1");
    f1->SetParameters(0.99,0.00010612);
    _functions["Jet_1"] = f1;
    
    TF1* f2 = new TF1("Jet_2", "pol2");
    f2->SetParameters(0.954153,0.000994919,-2.40087e-06);
    _functions["Jet_2"] = f2;
    
    TF1* f3 = new TF1("Jet_3", "pol2");
    f3->SetParameters(0.91526,-0.000779009,1.94091e-06);
    _functions["Jet_3"] = f3;
    
    TF1* f4 = new TF1("Jet_4", "pol2");
    f4->SetParameters(1.1403,-0.00215634,5.01697e-06);
    _functions["Jet_4"] = f4;

    TF1* f5 = new TF1("Jet_5", "pol1");
    f5->SetParameters(1.02,-0.000166);
    _functions["Jet_5"] = f5;

    TF1* f6 = new TF1("Jet_6", "pol1");
    f6->SetParameters(1.03864,-0.000347294);
    _functions["Jet_6"] = f6;
  }

  else if(_var == "perugia_soft"){
    
    TF1* f0 = new TF1("Jet_0", "pol1");
    f0->SetParameters(1.225, 0);
    _functions["Jet_0"] = f0;

    TF1* f1 = new TF1("Jet_1", "pol2");
    f1->SetParameters(0.93306,0.00118502,-1.88068e-06);
    _functions["Jet_1"] = f1;
    
    TF1* f2 = new TF1("Jet_2", "pol2");
    f2->SetParameters(0.901641,0.00163148,-2.97767e-06);
    _functions["Jet_2"] = f2;
    
    TF1* f3 = new TF1("Jet_3", "pol1");
    f3->SetParameters(0.960882,0.000472045);
    _functions["Jet_3"] = f3;
    
    TF1* f4 = new TF1("Jet_4", "pol2");
    f4->SetParameters(1.03584,-0.000846036,2.89668e-06);
    _functions["Jet_4"] = f4;

    TF1* f5 = new TF1("Jet_5", "pol1");
    f5->SetParameters(1.00678,-8.76705e-05);
    _functions["Jet_5"] = f5;

    TF1* f6 = new TF1("Jet_6", "pol1");
    f6->SetParameters(0.985048,-2.16763e-05);
    _functions["Jet_6"] = f6;
  }

  else if(_var == "ptjmin10"){
    
    TF1* f0 = new TF1("Jet_0", "pol1");
    f0->SetParameters(1.145, 0);
    _functions["Jet_0"] = f0;

    TF1* f1 = new TF1("Jet_1", "pol1");
    f1->SetParameters(1.0,0);
    _functions["Jet_1"] = f1;
    
    TF1* f2 = new TF1("Jet_2", "pol2");
    f2->SetParameters(0.946973, 0.000949434, -1.8592e-06);
    _functions["Jet_2"] = f2;
    
    TF1* f3 = new TF1("Jet_3", "pol2");
    f3->SetParameters(0.959433,0.000527614,-8.91845e-07);
    _functions["Jet_3"] = f3;
    
    TF1* f4 = new TF1("Jet_4", "pol3");
    f4->SetParameters(0.864826,0.00213587,-8.60115e-06,1.02618e-08);
    _functions["Jet_4"] = f4;

    TF1* f5 = new TF1("Jet_5", "pol3");
    f5->SetParameters(0.867957,0.00263563,-1.57348e-05,2.49292e-08);
    _functions["Jet_5"] = f5;

    TF1* f6 = new TF1("Jet_6", "pol1");
    f6->SetParameters(1.02228,-0.000332224);
    _functions["Jet_6"] = f6;
  }

  
  else if(_var == "ptjmin20"){
    
    TF1* f0 = new TF1("Jet_0", "pol1");
    f0->SetParameters(1.275, 0);
    _functions["Jet_0"] = f0;

    TF1* f1 = new TF1("Jet_1", "pol1");
    f1->SetParameters(1.0,0.0);
    _functions["Jet_1"] = f1;
    
    TF1* f2 = new TF1("Jet_2", "pol3");
    f2->SetParameters(0.819168,0.00592192,-4.27351e-05,7.8851e-08);
    _functions["Jet_2"] = f2;
    
    TF1* f3 = new TF1("Jet_3", "pol3");
    f3->SetParameters(0.737845,0.00703666,-4.64791e-05,8.30497e-08);
    _functions["Jet_3"] = f3;
    
    TF1* f4 = new TF1("Jet_4", "pol1");
    f4->SetParameters(0.979056,0.000183471);
    _functions["Jet_4"] = f4;
    
    TF1* f5 = new TF1("Jet_5", "pol1");
    f5->SetParameters(0.971243,0.000158724);
    _functions["Jet_5"] = f5;

    TF1* f6 = new TF1("Jet_6", "pol1");
    f6->SetParameters(0.97881,-3.65867e-05);
    _functions["Jet_6"] = f6;
  }

  
  else if(_var == "qfac05"){
    
    TF1* f0 = new TF1("Jet_0", "pol1");
    f0->SetParameters(1.125, 0);
    _functions["Jet_0"] = f0;

    TF1* f1 = new TF1("Jet_1", "pol2");
    f1->SetParameters(0.96884,0.000918626,-1.74032e-06);
    _functions["Jet_1"] = f1;
    
    TF1* f2 = new TF1("Jet_2", "pol3");
    f2->SetParameters(0.950907,0.00144935,-9.21053e-06,1.65875e-08);
    _functions["Jet_2"] = f2;
    
    TF1* f3 = new TF1("Jet_3", "pol1");
    f3->SetParameters(0.96829,0.000366);
    _functions["Jet_3"] = f3;
    
    TF1* f4 = new TF1("Jet_4", "pol1");
    f4->SetParameters(1.0,0.0);
    _functions["Jet_4"] = f4;

    TF1* f5 = new TF1("Jet_5", "pol1");
    f5->SetParameters(1.0,0.0);
    _functions["Jet_5"] = f5;

    TF1* f6 = new TF1("Jet_6", "pol1");
    f6->SetParameters(1.0,0.0);
    _functions["Jet_6"] = f6;
  }

  
  else if(_var == "qfac20"){
    
    TF1* f0 = new TF1("Jet_0", "pol1");
    f0->SetParameters(1.35, 0);
    _functions["Jet_0"] = f0; 

    TF1* f1 = new TF1("Jet_1", "pol3");
    f1->SetParameters(1.10661,-0.0039263,2.57041e-05,-4.74584e-08);
    _functions["Jet_1"] = f1;
    
    TF1* f2 = new TF1("Jet_2", "pol3");
    f2->SetParameters(1.06187,-0.0017312,9.92048e-06,-1.64962e-08);
    _functions["Jet_2"] = f2;
    
    TF1* f3 = new TF1("Jet_3", "pol1");
    f3->SetParameters(1.0,0);
    _functions["Jet_3"] = f3;
    
    TF1* f4 = new TF1("Jet_4", "pol1");
    f4->SetParameters(1.0,0);
    _functions["Jet_4"] = f4;

    TF1* f5 = new TF1("Jet_5", "pol1");
    f5->SetParameters(1.0,0);
    _functions["Jet_5"] = f5;

    TF1* f6 = new TF1("Jet_6", "pol1");
    f6->SetParameters(1.0,0);
    _functions["Jet_6"] = f6;
  }

  return;

} 








