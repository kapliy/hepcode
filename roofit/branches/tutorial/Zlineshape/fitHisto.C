#include "TLatex.h"
#include "TCanvas.h"

using namespace RooFit;

void fitHisto(TH1D* histOne){
  fitHisto(histOne, 2, 0., 0.);
  return;
}

void fitHisto(TH1D* histOne, int howMuchGauss, double& dmm, double& errorDmm){
  double Zmass = 91.1876;

  //  histOne->Rebin(2);

  // Start fitting stuff
  TH1D *hMassOne = histOne->Clone("hMassOne");
  double meanMass = -999.;   double meanErrMass = -999.;	  
  double sigmaMass = -999.;   double sigmaErrMass = -999.;	  
  double chi2one = -999.;

  new TCanvas;
  performEggeFit(hMassOne, howMuchGauss, meanMass, meanErrMass, sigmaMass, sigmaErrMass, chi2one);
  // end fitting stuff

  std::cout << " " << std::endl;
  std::cout << " quality of RooEggePdf fit " << std::endl;
  std::cout << " histOne " << chi2one << std::endl;

  std::cout << " " << std::endl;
  std::cout << " fit values" << std::endl;
  std::cout << " meanOne " << meanMass-Zmass <<  " +/- " << meanErrMass << "     sigmaOne " << sigmaMass << " +/- " << sigmaErrMass << std::endl;

  dmm = sigmaMass/Zmass;
  errorDmm = sigmaErrMass/Zmass;

  std::cout << " " << std::endl;
  std::cout << "                        dM/M " << std::endl;
  std::cout << histOne->GetTitle() << "  " << dmm << " +/- " << errorDmm << "       "  << chi2one << "    " << histOne->GetEntries() << std::endl;

  return;
}


void performEggeFit(TH1D* hist, int howMuchGauss, double& meanMassCBm, double& meanErrMassCBm, double& sigmaMassCBm, double& sigmaErrMassCBm, double& chi2){
  //  gROOT->ProcessLine(".L RooEggePdf.cxx+");
  gROOT->SetStyle("Plain");

  RooRealVar M("M","M", 60., 120.);
  RooDataHist data("data","hist", M, hist);
  RooPlot* mframe = M.frame(70,110);
  data.plotOn(mframe);
  //  mframe->Draw();

  RooRealVar A("A","A", 6.6058);//, 0., 50.);//3.35048e-07,0,10.) ;
  RooRealVar B("B","B", -3.0481);//,-10.,0.);//-5.52495e-01);//,-1.0,0);
  RooRealVar C("C","C", 1.);//3.25031e-01);//,0,5);

  RooRealVar mean("mean", "Mean of RooEggePdf" , 91.1876, 70.,100.) ;
  RooRealVar width("width","width", 2.4952);//, 0.5 ,10.);

  RooEggePdf f1("f1","f1",M,A,B,C,mean,width) ;
  
  // resolution function
  RooRealVar mu("mu", "Mu of Gaussian" , 0.);//, -5., 5.) ;
  RooRealVar sigma("sigma","Sigma of Gaussian", 5., 1., 10.) ;
  RooGaussian gauss("gauss", "gauss(M,mean,sigma)", M, mu, sigma) ;
  if(howMuchGauss==1){
    // Convolute the signal with the resolution
    RooFFTConvPdf model("model","truth (X) gaussian", M, f1, gauss);
  }else if(howMuchGauss==2){
    RooRealVar sigma2oversigma1("sigma2oversigma1","",2.);//
    RooFormulaVar sigma2("sigma2","Sigma of Gaussian", "@0*@1",RooArgList(sigma2oversigma1,sigma)); 
    RooGaussian gauss2("gauss2", "gauss(M,mean,sigma)", M, mu, sigma2) ;

    RooRealVar gaussFrac("gaussFrac","Gauss Fraction", 0.85);//, 0., 1.);
    RooAddPdf dGauss("dGauss","double Gauss", gauss, gauss2, gaussFrac);
    
    // Convolute the signal with the resolution
    RooFFTConvPdf model("model","truth (X) gaussian", M, f1, dGauss);
  }else if(howMuchGauss==3){
    RooRealVar sigma2oversigma1("sigma2oversigma1","",2.);//
    RooFormulaVar sigma2("sigma2","Sigma of Gaussian", "@0*@1",RooArgList(sigma2oversigma1,sigma)); 
    RooGaussian gauss2("gauss2", "gauss(M,mean,sigma)", M, mu, sigma2) ;

    RooRealVar gaussFrac("gaussFrac","Gauss Fraction", 0.6);//, 0., 1.);
    RooAddPdf dGauss("dGauss","double Gauss", gauss, gauss2, gaussFrac);    

    RooRealVar sigma3oversigma1("sigma3oversigma1","",4.);//
    RooFormulaVar sigma3("sigma3","Sigma of Gaussian", "@0*@1",RooArgList(sigma3oversigma1,sigma)); 
    RooGaussian gauss3("gauss3", "gauss(M,mean,sigma)", M, mu, sigma3) ;

    RooRealVar gaussFrac2("gaussFrac2","Gauss Fraction", 0.85);//, 0., 1.);
    RooAddPdf tGauss("tGauss","triple Gauss", dGauss, gauss3, gaussFrac2);
    
    // Convolute the signal with the resolution
    RooFFTConvPdf model("model","truth (X) gaussian", M, f1, tGauss);
  }

  std::cout << " PRINT FIT RESULT " << std::endl;

//   RooFitResult* r = model.fitTo(data,Save(true),Hesse(true),Minos(false), Range(76,101)) ;
//   r->Print("v");  
  model.fitTo(data,Save(true),Hesse(true),Minos(false)) ;
  model.plotOn(mframe) ;
  model.paramOn(mframe, Format("NEA", AutoPrecision(1)), Layout(0.62,0.92,0.93) ) ;

/*
  mframe->SetTitle("MS");
  mframe->SetXTitle("M_{#mu#mu} [GeV]");
  mframe->Draw();
  //  myMarkerText(0.25,0.62,1,20,"Data",1.3);
  //  myText(      0.2,0.86,1,"Simulation");
  myText(      0.2,0.86,1,"Data 2010, #sqrt{s}= 7 TeV");
  myText(      0.25,0.76,1,"#int L #approx 41 nb^{-1}, ");
  myText(      0.25,0.66,1,"|#eta|<1.05");
*/
  mframe->Draw();
  meanMassCBm    = mean.getVal();
  meanErrMassCBm = mean.getError();
  sigmaMassCBm    = sigma.getVal();
  sigmaErrMassCBm = sigma.getError();

  chi2 = mframe->chiSquare(2); // input number of fitted parameters
}

