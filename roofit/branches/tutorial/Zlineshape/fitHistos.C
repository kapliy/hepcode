#include "TLatex.h"

using namespace RooFit ;

void fitHistos(TH1D* histOne, TH1D* histTwo){
  Bool_t showFit = true;

  gROOT->ProcessLine(".L RooEggePdf.cxx+");
  
  gROOT->SetStyle("Plain");
  //  gStyle->SetOptStat(0);

  masses = new TCanvas("masses","mass overview",0,0,1200,500);
  masses->Divide(2,1);
  masses->cd(1);
  histOne->Draw();
  // Start fitting stuff
  TH1D *hMassOne = histOne->Clone("hMassOne");
  double meanMass = -999.;   double meanErrMass = -999.;	  
  double sigmaMass = -999.;   double sigmaErrMass = -999.;	  
  double chi2one = -999.;

  performEggeFit(hMassOne, meanMass, meanErrMass, sigmaMass, sigmaErrMass, showFit, chi2one);
  // end fitting stuff

//   leg = new TLegend(0.15,0.73,0.35,0.88);
//   leg->SetFillColor(0);
//   leg->AddEntry(massCBm,"no cut","l");
//   leg->AddEntry(masscCBm,"dp cut","l");
//   //leg->AddEntry(masscCBm,"MATF cuts","l");
//   leg->Draw();

  masses->cd(2);
  histTwo->Draw();
  // Start fitting stuff
  TH1D *hMassTwo = (TH1D*)histTwo->Clone("hMassTwo");
  double meanMassTwo = -999.;   double meanErrMassTwo = -999.;	  
  double sigmaMassTwo = -999.;   double sigmaErrMassTwo = -999.;	  
  double chi2two = -999.;

  performEggeFit(hMassTwo, meanMassTwo, meanErrMassTwo, sigmaMassTwo, sigmaErrMassTwo, showFit, chi2two);
  // end fitting stuff



  std::cout << " " << std::endl;
  std::cout << " quality of RooEggePdf fits " << std::endl;
  std::cout << " histOne " << chi2one << std::endl;
  std::cout << " histTwo " << chi2two << std::endl;


  return;
}


void performEggeFit(TH1D* hist, double& meanMassCBm, double& meanErrMassCBm, double& sigmaMassCBm, double& sigmaErrMassCBm, Bool_t showFit, double& chi2){
    RooRealVar M("M","M", -120., 120.);
    RooDataHist data("data","hist", M, hist);
    RooPlot* mframe = M.frame(60,120);
    data.plotOn(mframe);
    //  mframe->Draw();

    RooRealVar A("A","A", 6.6058);//, 0., 50.);//3.35048e-07,0,10.) ;
    RooRealVar B("B","B", -3.0481);//-0.144,-10.,0.);//-5.52495e-01);//,-1.0,0);
    RooRealVar C("C","C", 1.);//3.25031e-01);//,0,5);

    RooRealVar mean("mean", "Mean of RooEggePdf" , 91.1876);//, 70.,100.) ;
    RooRealVar width("width","width", 2.4952);//, 0.5 ,10.);

    RooEggePdf f1("f1","f1",M,A,B,C,mean,width) ;
  
    // resolution function
    RooRealVar mu("mu", "Mu of Gaussian" , 0., -5., 5.) ;
    RooRealVar sigma("sigma","Sigma of Gaussian", 5., 1., 10.) ;
    RooGaussian gauss("gauss", "gauss(M,mean,sigma)", M, mu, sigma) ;

    // Convolute the signal with the Gaussian
    RooFFTConvPdf model("model","truth (X) gaussian", M, f1, gauss);

    std::cout << " PRINT FIT RESULT " << std::endl;

    RooFitResult* r = model.fitTo(data,Save(true),Hesse(true),Minos(false), Range(80,105)) ;
    r->Print("v");  
    model.plotOn(mframe) ;
    model.plotOn(mframe,Components(gauss),LineStyle(kDashed)) ;
    model.paramOn(mframe, Format("NEA", AutoPrecision(1)), Layout(0.6,0.97,0.95) ) ;

    if(showFit)  mframe->Draw();

    meanMassCBm    = mu.getVal();
    meanErrMassCBm = mu.getError();
    sigmaMassCBm    = sigma.getVal();
    sigmaErrMassCBm = sigma.getError();

    chi2 = mframe->chiSquare(4); // input number of fitted parameters
  }

