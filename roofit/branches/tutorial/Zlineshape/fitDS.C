#include "TLatex.h"
#include "RooDataSet.h"

using namespace RooFit ;

void fitDS(Bool_t doSave=false, Bool_t showFit=true){
  gROOT->ProcessLine(".L RooEggePdf.cxx+");
  
  gROOT->SetStyle("Plain");
  //  gStyle->SetOptStat(0);

  TFile* file = TFile::Open("testNtuple.root");
  TTree* tree = (TTree *)file->Get("egge");
//   tree->SetDirectory(0);
//   file->Close();

  RooRealVar trueMass("trueMass","trueMass", 40., 140.);
  RooDataSet data("data", "data", tree, trueMass);
  RooDataSet* reduced = (RooDataSet*)data.reduce("trueMass>0");
  RooPlot* mframe = trueMass.frame(40,140);
  reduced->plotOn(mframe);
  mframe->Draw();
  
  RooRealVar A("A","A", 6.0967,0.,100.) ;//3.7651
  RooRealVar B("B","B", -3.1479,-100.,0.);//-5.52495e-01);//,-1.0,0);//-2.8825
  RooRealVar C("C","C", 1.);//1.,0., 50.);//3.25031e-01);//,0,5);//1
  
  RooRealVar mean("mean", "Mean of RooEggePdf" , 91.1876);//0, 70.,100.) ;
  RooRealVar width("width","width",  2.4952);//5., 0.5 ,10.); //2.4952
  
  RooEggePdf f1("f1","f1",trueMass,A,B,C,mean,width) ;
  
  // resolution function
  RooRealVar mu("mu", "Mu of Gaussian" , 0., -5., 5.) ;
  RooRealVar sigma("sigma","Sigma of Gaussian", 5., 1., 10.) ;
  RooGaussian gauss("gauss", "gauss(trueMass,mean,sigma)", trueMass, mu, sigma) ;
  
  // Convolute the signal with the Gaussian
  RooFFTConvPdf model("model","truth (X) gaussian", trueMass, f1, gauss);
  
  std::cout << " PRINT FIT RESULT " << std::endl;
  
  RooFitResult* r = f1.fitTo(*reduced,Save(true),Hesse(true),Minos(false)); //, /*Range(80,100),*/) ;
  r->Print("v");  
  f1.plotOn(mframe) ;
  f1.plotOn(mframe,Components(gauss),LineStyle(kDashed)) ;
  f1.paramOn(mframe, Format("NEA", AutoPrecision(1)), Layout(0.6,0.88,0.95) ) ;
  
  if(showFit)  mframe->Draw();
  
  std::cout << " quality of fit " << mframe->chiSquare(2) << std::endl; // input number of fitted parameters

  return;
}
