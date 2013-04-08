
#include <DataGraphics/DgEfficiencyWeighted0D.hh>
#include <DataGraphics/DgEfficiency0D.hh>
#include <DataGraphics/DgTypes.hh>
#include <DataGraphics/DgBinomialErrors.hh>

#include <cassert>
#include <cmath>
#include <vector>
#include <algorithm>
#include <iterator>
#include <string.h>
#include <TGraphAsymmErrors.h>
#include <TDirectory.h>

using namespace std;
using namespace DataGraphics;

DgEfficiencyWeighted0D::DgEfficiencyWeighted0D()
  : DgEfficiency0D()
  , _weighted_pass(0) 
  , _weighted_total(0)
{
}

DgEfficiencyWeighted0D::~DgEfficiencyWeighted0D()
{
}

void
DgEfficiencyWeighted0D::fillew( const bool& pass, const WeightedCount& w )
{
  ++_total;
  _weighted_total += w;
  if( pass ) {
    ++_pass;
    _weighted_pass += w;
  }
}

double
DgEfficiencyWeighted0D::denominator() const
{
  return _weighted_total;
}

double
DgEfficiencyWeighted0D::numerator() const
{
  return _weighted_pass;
}

void
DgEfficiencyWeighted0D::efficiency( double& central , double& err_low , double& err_high ) const
{
  // for MC@NLO events, negative weights could result in _weighted_pass > _weighted_total
  // In such cases, just report 0%
  if(_weighted_pass <= _weighted_total && _weighted_pass>=0) {
    FindEfficiencyWithAsymmetricError( _weighted_pass , _weighted_total , central , err_low , err_high );
  }
  else {
    central = 0.;
    err_low = 0.;
    err_high = 0.;
  }
}


TNamed*
DgEfficiencyWeighted0D::new_ROOT( const char* name ) const
{
  Double_t* x  = new Double_t[1];
  Double_t* y  = new Double_t[1];
  Double_t* exl = new Double_t[1];
  Double_t* exh = new Double_t[1];
  Double_t* eyl = new Double_t[1];
  Double_t* eyh = new Double_t[1];
  Int_t j=0;
  x[0] = 0.;
  y[0] = 0.;
  exl[0] = 0.;
  exh[0] = 0.;
  eyl[0] = 0.;
  eyh[0] = 0.;
  if( _weighted_total>0 ) {
    Double_t eff , eff_min_err , eff_max_err;
    efficiency( eff , eff_min_err , eff_max_err );
    x[0] = 0.5;
    y[0] = eff;
    if(false) {
      // drawn x error should be indistinguishable from zero
      exl[0] = (-0.0001)*0.5;
      exh[0] = ( 0.0001)*0.5;
    } else {
      // save numerator/denominator in x error
      exl[0] = _weighted_pass;
      exh[0] = _weighted_total;
    }
    // drawn y error is asymmetric result
    eyl[0] = eff_min_err;
    eyh[0] = eff_max_err;
    ++j;
  }
  TGraphAsymmErrors* gr = new TGraphAsymmErrors(j,x,y,exl,exh,eyl,eyh);
  delete[] eyh;
  delete[] eyl;
  delete[] exh;
  delete[] exl;
  delete[] y;
  delete[] x;
  return gr;
}

void
DgEfficiencyWeighted0D::save_as_ROOT( const char* name , TDirectory* dir ) const
{
  dir->cd();
  TNamed* gr = new_ROOT( name );
  gr->Write( name );
  delete gr;
}

void
DgEfficiencyWeighted0D::save( const char* name , TDirectory* dir ) const
{
  dir->cd();
  TObjectDgEfficiencyWeighted0D* h = new TObjectDgEfficiencyWeighted0D( name , "" , *this );
  h->Write( name );
  delete h;
}

void
DgEfficiencyWeighted0D::load( const char* name , TDirectory* dir )
{
  dir->cd();
  const TObjectDgEfficiencyWeighted0D* h = dynamic_cast<const TObjectDgEfficiencyWeighted0D*>(dir->Get(name));
  assert( h );
  *this = *h;
  delete h;
}

void
DgEfficiencyWeighted0D::merge( const DgAbsGraphic* other )
{
  const DgEfficiencyWeighted0D* hother = dynamic_cast<const DgEfficiencyWeighted0D*>(other);  assert(hother);
  DgEfficiency0D::merge( other );
  _weighted_pass += hother->_weighted_pass;
  _weighted_total += hother->_weighted_total;
}

#include <TObject.h>
ClassImp(TObjectDgEfficiencyWeighted0D)
