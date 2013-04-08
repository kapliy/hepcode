
#include <iostream>
#include <iomanip>
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/format.hpp>
#include <RooRealVar.h>
#include <RooGaussian.h>
#include <RooCBShape.h>
#include <RooAddPdf.h>
#include <RooAbsPdf.h>
#include <RooArgList.h>
#include <RooArgSet.h>
#include <RooGlobalFunc.h>
#include <RooFitResult.h>
#include <RooPlot.h>
#include <RooDataHist.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TH1.h>
#include <TGraph.h>
#include "TrigFTKAna/LikelihoodParameterization1D.hpp"
#include "TrigFTKAna/Likelihood1D.hpp"
#include "TrigFTKAna/Detector.hpp"


using namespace std;

void
LikelihoodParameterization1D::resetRoos( const TH1* h )
{
  if( _type == LONG_LIVED ) {
    _x.reset( new RooRealVar("trackx","likelihoodinput",-40,40,"") );
    _mean.reset( new RooRealVar("bMean","b input PDF mean",0.,-1.,1.) );
    _mean3.reset();
    _cb_sigma.reset( new RooRealVar( "bSigma" , "b input PDF width" , 1. , 0.1 , 2. ) );
    _cb_alpha.reset( new RooRealVar( "bAlpha" , "b input PDF crystal ball tail parameter" , -0.5 , -1. , 0.000001 ) );
    _cb_order.reset( new RooRealVar( "bOrder" , "b input PDF crystal ball order parameter" , 1.2 , 1. , 40. ) );
    _cb.reset( new RooCBShape( "bModelA" , "b template PDF" , *_x , *_mean , *_cb_sigma , *_cb_alpha , *_cb_order ) );
    _gaus1_sigma.reset();
    _gaus2_sigma.reset( new RooRealVar( "bWidthB" , "b template PDF width B" , 2.5 * (h ? h->GetRMS() : 1) , 0.4 , 40. ) );
    _gaus3_sigma.reset( new RooRealVar( "bWidthC" , "b template PDF width C" , 2.5 * (h ? h->GetRMS() : 1) , 0.4 , 40. ) );
    _gaus1.reset();
    _gaus2.reset( new RooGaussian( "bGaussB" , "b template PDF gaussian B" , *_x , *_mean , *_gaus2_sigma ) );
    _gaus3.reset( new RooGaussian( "bGaussC" , "b template PDF gaussian C" , *_x , *_mean , *_gaus3_sigma ) );
    _frac2.reset( new RooRealVar( "bFA" , "fraction of core gaussian" , 0.75 , 0 , 1. ) );
    _frac3.reset( new RooRealVar( "bFC" , "fraction of core gaussian" , 0.25 , 0 , 1. ) );
    _pdf.reset( new RooAddPdf( "bModel" , "b template" , RooArgList(*_cb,*_gaus2,*_gaus3) , RooArgList(*_frac2,*_frac3) ) );
  } else if( _type == PROMPT ) {
    _x.reset( new RooRealVar( "trackx" , "likelihood input" , -40 , 40 , "" ) );
    _mean.reset( new RooRealVar( "uMean" , "u template PDF mean" , 0. ) );
    _mean3.reset( new RooRealVar( "uMean3" , "u template PDF mean" , 0. , -5 , 5 ) );
    _cb_sigma.reset();
    _cb_alpha.reset();
    _cb_order.reset();
    _cb.reset();
    _gaus1_sigma.reset( new RooRealVar( "uWidthA" , "u template PDF width A" , 0.5*(h ? h->GetRMS() : 1), 0 , 40. ) );
    _gaus2_sigma.reset( new RooRealVar( "uWidthB" , "u template PDF width B" , 1.5*(h ? h->GetRMS() : 1), 0 , 40. ) );
    _gaus3_sigma.reset( new RooRealVar( "uWidthC" , "u template PDF width C" , 2.5*(h ? h->GetRMS() : 1), 0 , 40. ) );
    _gaus1.reset( new RooGaussian( "uGaussA" , "u template PDF gaussian A" , *_x , *_mean , *_gaus1_sigma ) );
    _gaus2.reset( new RooGaussian( "uGaussB" , "u template PDF gaussian B" , *_x , *_mean , *_gaus2_sigma ) );
    _gaus3.reset( new RooGaussian( "uGaussC" , "u template PDF gaussian C" , *_x , *_mean3 , *_gaus3_sigma ) );
    _frac2.reset( new RooRealVar( "uFA" , "fraction of core gaussian" , 0.75 , 0 , 1. ) );
    _frac3.reset( new RooRealVar( "uFC" , "fraction of core gaussian" , 0.25 , 0 , 1. ) );
    _pdf.reset( new RooAddPdf( "uModel" , "u template double gaussian" , RooArgList(*_gaus1,*_gaus2,*_gaus3) , RooArgList(*_frac2,*_frac3) ) );
  } else {
    _x.reset();
    _mean.reset();
    _mean3.reset();
    _cb_sigma.reset();
    _cb_alpha.reset();
    _cb_order.reset();
    _gaus1_sigma.reset();
    _gaus2_sigma.reset();
    _gaus3_sigma.reset();
    _gaus1.reset();
    _gaus2.reset();
    _gaus3.reset();
    _cb.reset();
    _frac2.reset();
    _frac3.reset();
    _pdf.reset();
  }
  _integral = 0;
  claimRooOwnership();
}

void
LikelihoodParameterization1D::claimRooOwnership()
{
  if( _x ) { _x->ResetBit( kMustCleanup ); _x->ResetBit( kCanDelete ); } 
  if( _mean ) { _mean->ResetBit( kMustCleanup ); _mean->ResetBit( kCanDelete ); } 
  if( _mean3 ) { _mean3->ResetBit( kMustCleanup ); _mean3->ResetBit( kCanDelete ); } 
  if( _cb_sigma ) { _cb_sigma->ResetBit( kMustCleanup ); _cb_sigma->ResetBit( kCanDelete ); } 
  if( _cb_alpha ) { _cb_alpha->ResetBit( kMustCleanup ); _cb_alpha->ResetBit( kCanDelete ); } 
  if( _cb_order ) { _cb_order->ResetBit( kMustCleanup ); _cb_order->ResetBit( kCanDelete ); } 
  if( _gaus1_sigma ) { _gaus1_sigma->ResetBit( kMustCleanup ); _gaus1_sigma->ResetBit( kCanDelete ); } 
  if( _gaus2_sigma ) { _gaus2_sigma->ResetBit( kMustCleanup ); _gaus2_sigma->ResetBit( kCanDelete ); } 
  if( _gaus3_sigma ) { _gaus3_sigma->ResetBit( kMustCleanup ); _gaus3_sigma->ResetBit( kCanDelete ); } 
  if( _gaus1 ) { _gaus1->ResetBit( kMustCleanup ); _gaus1->ResetBit( kCanDelete ); } 
  if( _gaus2 ) { _gaus2->ResetBit( kMustCleanup ); _gaus2->ResetBit( kCanDelete ); } 
  if( _gaus3 ) { _gaus3->ResetBit( kMustCleanup ); _gaus3->ResetBit( kCanDelete ); } 
  if( _cb ) { _cb->ResetBit( kMustCleanup ); _cb->ResetBit( kCanDelete ); } 
  if( _frac2 ) { _frac2->ResetBit( kMustCleanup ); _frac2->ResetBit( kCanDelete ); } 
  if( _frac3 ) { _frac3->ResetBit( kMustCleanup ); _frac3->ResetBit( kCanDelete ); } 
  if( _pdf ) { _pdf->ResetBit( kMustCleanup ); _pdf->ResetBit( kCanDelete ); } 
}


// fit the likelihood
void
LikelihoodParameterization1D::fit( const Type liktype , const TH1* h , const std::string fitname )
{
  _type = liktype;
  _integral = 0.;
  resetRoos( h );
  boost::shared_ptr<RooDataHist> liktemplate( new RooDataHist( "template_jets" , "jet likelihood input template" , *_x , h ) );
  if( liktemplate ) { liktemplate->ResetBit( kMustCleanup ); liktemplate->ResetBit( kCanDelete ); }

  RooFitResult* result = _pdf->fitTo( *liktemplate , RooFit::Save(true) , RooFit::InitialHesse(kTRUE) , RooFit::Hesse(kTRUE) , 
                                      RooFit::Minos(kTRUE) , RooFit::Verbose(kFALSE) );
  result->Print("v");
  if( true ) {
    TCanvas* can = new TCanvas("can","");
    if( !fitname.empty() ) { _x->SetTitle( fitname.c_str() ); }
    RooPlot* frame1 = _x->frame();
    liktemplate->plotOn( frame1 , RooFit::MarkerColor(kRed) , RooFit::LineColor(kRed)  );
    _pdf->plotOn( frame1 , RooFit::LineColor(13) );
    TVirtualPad::Pad()->SetLogy(1);
    frame1->Draw();
    can->Update();
    can->Modified();
    can->Draw();
    static unsigned int icanvas=0;
    ++icanvas;
    string filename = (boost::format("temp_%|s|_lik_%|d|.png") % fitname % icanvas).str();
    can->Print( filename.c_str() );
    // char buf[512];
    // cin >> buf;
  }
  _x->setRange( h->GetXaxis()->GetXmin() , h->GetXaxis()->GetXmax() );
  boost::scoped_ptr<RooAbsReal> intvar( _pdf->createIntegral( *_x ) );
  _integral = intvar->getVal();
  _xmin = h->GetXaxis()->GetXmin();
  _xmax = h->GetXaxis()->GetXmax();
  // cout << " RooFit integral: " << _integral << endl;
  // if( _type == LONG_LIVED ) {
  //   const double integral_gauss = std::sqrt( M_PI / 2. ) * _cb_sigma->getVal();
  //   const double integral_cb_fac1 = std::exp( - _cb_alpha->getVal()*_cb_alpha->getVal() / 2. );
  //   const double integral_cb_fac2 = std::pow( _cb_order->getVal() , _cb_order->getVal() );
  //   const double integral_cb_fac3 = std::pow( _cb_order->getVal() - (_cb_alpha->getVal()*_cb_alpha->getVal()) , 1 - _cb_order->getVal() ) * _cb_sigma->getVal();
  //   const double integral_cb_den = (1. - _cb_order->getVal())*_cb_alpha->getVal();
  //   _integral = detector::safe_divide( 1. , (integral_cb_fac1*integral_cb_fac2*integral_cb_fac3/integral_cb_den) - integral_gauss );
  // } else {
  //   const double integral_gauss = 2.* std::sqrt( M_PI / 2. ) * _gaus1_sigma->getVal();
  //   _integral = integral_gauss;
  // }
  // cout << " primary integral: " << _integral << endl;
  // _integral += (_frac2->getVal()*2.* std::sqrt( M_PI / 2. ) * _gaus2_sigma->getVal());
  // _integral += (_frac3->getVal()*2.* std::sqrt( M_PI / 2. ) * _gaus3_sigma->getVal());
  // cout << " total integral: " << _integral << endl;
  if( true ) {
    TCanvas* can = new TCanvas("can","");
    const double xmin = h->GetXaxis()->GetXmin();
    const double xmax = h->GetXaxis()->GetXmax();
    boost::scoped_ptr<TGraph> gr( new TGraph );
    for( unsigned int i=0; i!=50; ++i ) {
      double x = xmin + i*(xmax-xmin)/50.;
      gr->SetPoint(i,x,lik(x));
    }
    gr->SetMarkerStyle(8);
    gr->SetMarkerColor(kRed);
    gr->Draw("ap");
    can->Update();
    can->Modified();
    can->Draw();
    static unsigned int icanvas=0;
    ++icanvas;
    string filename = (boost::format("temp_%|s|_likgr_%|d|.png") % fitname % icanvas).str();
    can->Print( filename.c_str() );
    // char buf[512];
    // cin >> buf;
  }
}


// get the likelihood
const float
LikelihoodParameterization1D::lik( const float& x ) const
{
  // integrate up to max
  _x->setRange( x , _xmax );
  boost::scoped_ptr<RooAbsReal> intvar( _pdf->createIntegral( *_x ) );
  return( intvar->getVal() / _integral );
  //_x->setVal( x );
  //return( _pdf->getVal() / _integral );
  // average over a small window
  // _x->setRange( x - 0.01 , x+0.01 );
  // boost::scoped_ptr<RooAbsReal> intvar( _pdf->createIntegral( *_x ) );  
  // return( (intvar->getVal()/0.02) / _integral );
}

void
LikelihoodParameterization1D::print( std::ostream& os ) const
{
  if( _type == LONG_LIVED ) {
    cout << "long lived likelihood: " << endl
         << boost::format(" %|10t| mean: %|12g|") % _mean->getVal() << endl
         << boost::format(" %|10t| cb sigma: %|12g|") % _cb_sigma->getVal() << endl
         << boost::format(" %|10t| cb alpha: %|12g|") % _cb_alpha->getVal() << endl
         << boost::format(" %|10t| cb order: %|12g|") % _cb_order->getVal() << endl
         << boost::format(" %|10t| gaus #2 sigma: %|12g|") % _gaus2_sigma->getVal() << endl
         << boost::format(" %|10t| gaus #3 sigma: %|12g|") % _gaus3_sigma->getVal() << endl
         << boost::format(" %|10t| frac #2: %|12g|") % _frac2->getVal() << endl
         << boost::format(" %|10t| frac #3: %|12g|") % _frac3->getVal() << endl
         << boost::format(" %|10t| xrange: %|12g| %|12g|") % _xmin % _xmax << endl
         << boost::format(" %|10t| integral: %|12g|") % _integral << endl
         << boost::format(" %|10t| lik(0): %|12g|") % lik(0) << endl
         << boost::format(" %|10t| lik(-10): %|12g|") % lik(-10) << endl
         << boost::format(" %|10t| lik(10): %|12g|") % lik(10) << endl
      ;
  } else if( _type == PROMPT ) {
    cout << "prompt likelihood: " << endl
         << boost::format(" %|10t| mean: %|12g|") % _mean->getVal() << endl
         << boost::format(" %|10t| mean #3: %|12g|") % _mean3->getVal() << endl
         << boost::format(" %|10t| gaus #1 sigma: %|12g|") % _gaus1_sigma->getVal() << endl
         << boost::format(" %|10t| gaus #2 sigma: %|12g|") % _gaus2_sigma->getVal() << endl
         << boost::format(" %|10t| gaus #3 sigma: %|12g|") % _gaus3_sigma->getVal() << endl
         << boost::format(" %|10t| frac #2: %|12g|") % _frac2->getVal() << endl
         << boost::format(" %|10t| frac #3: %|12g|") % _frac3->getVal() << endl
         << boost::format(" %|10t| xrange: %|12g| %|12g|") % _xmin % _xmax << endl
         << boost::format(" %|10t| integral: %|12g|") % _integral << endl
         << boost::format(" %|10t| lik(0): %|12g|") % lik(0) << endl
         << boost::format(" %|10t| lik(-10): %|12g|") % lik(-10) << endl
         << boost::format(" %|10t| lik(10): %|12g|") % lik(10) << endl
      ;
  }
}
