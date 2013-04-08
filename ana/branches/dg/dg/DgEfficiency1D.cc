
#include <DataGraphics/DgEfficiency1D.hh>
#include <DataGraphics/DgTypes.hh>
#include <DataGraphics/DgBinomialErrors.hh>
#include <boost/math/special_functions/fpclassify.hpp>

#include <cassert>
#include <cmath>
#include <vector>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <string.h>
#include <TGraphAsymmErrors.h>
#include <TDirectory.h>
#include <TAxis.h>

using namespace std;
using namespace DataGraphics;

DgEfficiency1D::DgEfficiency1D( const BinSize& nbins , const Value& min , const Value& max ) 
  : DgHistogram1D(nbins,min,max) ,  _pass( nbins )
{
  std::fill( _pass.begin() , _pass.end() , 0 );
}

DgEfficiency1D::~DgEfficiency1D()
{
}

void
DgEfficiency1D::fille( const Value& x , const bool& yes )
{
  DgHistogram1D::fill( x );
  if( !yes ) { return; }
  bool lox = x<_min;
  bool hix = x>=_max || boost::math::isnan(x);
  if( !(lox || hix) ) {
    ++_pass[bin_index(x)];
    return;
  }
}

double
DgEfficiency1D::numerator( const unsigned int& ibin ) const 
{
  assert( ibin<_nbins );
  return _pass[ibin];
}

double
DgEfficiency1D::denominator( const unsigned int& ibin ) const 
{
  assert( ibin<_nbins );
  return _count[ibin];
}

void
DgEfficiency1D::efficiency( const unsigned int& ibin , double& xmin , double& xmax , double& central , double& err_low , double& err_high ) const
{
  assert( ibin<_nbins );
  const Count& den = _count[ibin];
  const Count& num = _pass[ibin];
  FindEfficiencyWithAsymmetricError( num , den , central , err_low , err_high );
  xmin = bin_min(ibin);
  xmax = bin_max(ibin);
}

void
DgEfficiency1D::efficiency( const unsigned int& ibin , const unsigned int& fbin, double& xmin , double& xmax , double& central , double& err_low , double& err_high ) const
{
  assert( ibin<_nbins );
  assert( fbin<=_nbins );
  assert( ibin<=fbin );
  std::vector<Count>::const_iterator id=_count.begin();
  std::vector<Count>::const_iterator fd=_count.begin();
  std::vector<Count>::const_iterator in=_pass.begin();
  std::vector<Count>::const_iterator fn=_pass.begin();
  advance(id,ibin);
  advance(fd,fbin);
  advance(in,ibin);
  advance(fn,fbin);
  const Count den = std::accumulate( id , fd , static_cast<Count>(0) );
  const Count num = std::accumulate( in , fn , static_cast<Count>(0) );
  FindEfficiencyWithAsymmetricError( num , den , central , err_low , err_high );
  xmin = bin_min(ibin);
  xmax = bin_max(fbin-1);
}

TNamed*
DgEfficiency1D::new_ROOT( const char* name ) const
{
  return new_ROOT(name,1);
}

TNamed*
DgEfficiency1D::new_ROOT( const char* name , const unsigned int& rebin ) const
{
  Int_t n = _nbins;
  Double_t* x  = new Double_t[n];
  Double_t* y  = new Double_t[n];
  Double_t* exl = new Double_t[n];
  Double_t* exh = new Double_t[n];
  Double_t* eyl = new Double_t[n];
  Double_t* eyh = new Double_t[n];
  Int_t j=0;
  Int_t bin_step = std::max( rebin , 1u );
  for( Int_t i=0; i<n; i+=bin_step ) {
    const Count& den = _count[i];
    if( den <= 0 ) { continue; }
    Double_t eff , eff_min_err , eff_max_err , xmin , xmax;
    const unsigned int fbin = std::max(static_cast<unsigned int>(i+rebin),static_cast<unsigned int>(i+1));
    efficiency( i , fbin , xmin , xmax , eff , eff_min_err , eff_max_err );
    assert( j<n );
    x[j] = 0.;
    for( unsigned int ibin=i; ibin!=fbin; ++ibin ) {
      x[j] += bin_center(ibin);
    }
    const double dnbins = static_cast<double>(fbin-i);
    x[j] /= dnbins;
    y[j] = eff;
    // drawn x error should be indistinguishable from zero
    exl[j] = (-0.0001)*_binwidth;
    exh[j] = ( 0.0001)*_binwidth;
    // drawn y error is asymmetric result
    eyl[j] = eff_min_err;
    eyh[j] = eff_max_err;
    ++j;
  }
  TGraphAsymmErrors* gr = new TGraphAsymmErrors(j,x,y,exl,exh,eyl,eyh);
  if( _axis_label != "" ) {
    gr->SetTitle( _axis_label.c_str() );
    gr->GetXaxis()->SetTitle( _axis_label.c_str() );
    gr->GetYaxis()->SetTitle( "EFFICIENCY" );
  }
  delete[] eyh;
  delete[] eyl;
  delete[] exh;
  delete[] exl;
  delete[] y;
  delete[] x;
  return gr;
}

void
DgEfficiency1D::save_as_ROOT( const char* name , TDirectory* dir ) const
{
  dir->cd();
  TNamed* gr = new_ROOT( name );
  gr->Write( name );
  delete gr;
}

void
DgEfficiency1D::save( const char* name , TDirectory* dir ) const
{
  dir->cd();
  TObjectDgEfficiency1D* h = new TObjectDgEfficiency1D( name , "" , *this );
  h->Write( name );
  delete h;
}

void
DgEfficiency1D::load( const char* name , TDirectory* dir )
{
  dir->cd();
  const TObjectDgEfficiency1D* h = dynamic_cast<const TObjectDgEfficiency1D*>(dir->Get(name));
  assert( h );
  *this = *h;
  delete h;
}

void
DgEfficiency1D::merge( const DgAbsGraphic* other )
{
  const DgEfficiency1D* hother = dynamic_cast<const DgEfficiency1D*>(other);  assert(hother);
  DgHistogram1D::merge( other );
  assert( _pass.size() == hother->_pass.size() );
  vector<Count>::iterator j = _pass.begin();
  for( vector<Count>::const_iterator i=hother->_pass.begin(), f=hother->_pass.end(); i!=f; ++i,++j ) {
    (*j) += (*i);
  }
}

#include <TObject.h>
ClassImp(TObjectDgEfficiency1D)
