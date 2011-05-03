#include "TLatex.h"

using namespace RooFit ;

void exampleCode(){
  gROOT->ProcessLine(".L RooEggePdf.cxx+");
  gROOT->SetStyle("Plain");
  //  gStyle->SetOptStat(0);

  // Start fitting stuff
  TH1D *hnew = (TH1D*)hold->Clone("hnew");
  double meanMass = -999.;   double meanErrMass = -999.;	  
  double sigmaMass = -999.;   double sigmaErrMass = -999.;	  
  double chi2CBm = -999.;

  performEggeFit(hnew, meanMass, meanErrMass, sigmaMass, sigmaErrMass, showFit, chi2CBm);
 

  std::cout << " quality of RooEggePdf fits " << std::endl;
  std::cout << " CB " << chi2CBm << std::endl;

  return;
}


void performEggeFit(TH1D* hist, double& meanMassCBm, double& meanErrMassCBm, double& sigmaMassCBm, double& sigmaErrMassCBm, Bool_t showFit, double& chi2){
    RooRealVar M("M","M", -120., 120.);
    RooDataHist data("data","hist", M, hist);
    RooPlot* mframe = M.frame(60,120);
    data.plotOn(mframe);
    //  mframe->Draw();

    RooRealVar A("A","A", 3.35048e-07,0,10.) ;
    RooRealVar B("B","B", -5.52495e-01);//,-1.0,0);
    RooRealVar C("C","C", 3.25031e-01);//,0,5);

    RooRealVar mean("mean", "Mean of RooEggePdf" , 91.1876);//, 70.,100.) ;
    RooRealVar width("width","width", 2.4952);//, 0.5 ,10.);

    RooEggePdf f1("f1","f1",M,A,B,C,mean,width) ;
  
    // resolution function
    RooRealVar mu("mu", "Mu of Gaussian" , 0., -5., 5.) ;
    RooRealVar sigma("sigma","Sigma of Gaussian", 5., 1., 10.) ;
    RooGaussian gauss("gauss", "gauss(M,mean,sigma)", M, mu, sigma) ;

    // Convolute the signal with the Gaussian
    RooFFTConvPdf model("model","truth (X) gaussian", M, f1, gauss);

    RooFitResult* r = model.fitTo(data,Save(true),Hesse(true),Minos(false)); //, /*Range(80,100),*/) ;
    r->Print("v");  
    model.plotOn(mframe) ;
    model.plotOn(mframe,Components(f1),LineStyle(kDashed)) ;
    model.paramOn(mframe, Format("NEA", AutoPrecision(1)), Layout(0.7,1.0,0.95) ) ;

    if(showFit)  mframe->Draw();

    meanMassCBm    = mu.getVal();
    meanErrMassCBm = mu.getError();
    sigmaMassCBm    = sigma.getVal();
    sigmaErrMassCBm = sigma.getError();

    chi2 = mframe->chiSquare(3); // input number of fitted parameters
}
