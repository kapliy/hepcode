
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

DgEfficiency0D::DgEfficiency0D()
  : _pass(0) 
  , _total(0)
{
}

DgEfficiency0D::~DgEfficiency0D()
{
}

void
DgEfficiency0D::fille( const bool& yes )
{
  ++_total;
  if( yes ) { ++_pass; }
}

double
DgEfficiency0D::denominator() const
{
  return _total;
}

double
DgEfficiency0D::numerator() const
{
  return _pass;
}

void
DgEfficiency0D::efficiency( double& central , double& err_low , double& err_high ) const
{
  assert(_pass<=_total);
  FindEfficiencyWithAsymmetricError( _pass , _total , central , err_low , err_high );
}


TNamed*
DgEfficiency0D::new_ROOT( const char* name ) const
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
  if( _total>0 ) {
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
      exl[0] = _pass;
      exh[0] = _total;
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
DgEfficiency0D::save_as_ROOT( const char* name , TDirectory* dir ) const
{
  dir->cd();
  TNamed* gr = new_ROOT( name );
  gr->Write( name );
  delete gr;
}

void
DgEfficiency0D::save( const char* name , TDirectory* dir ) const
{
  dir->cd();
  TObjectDgEfficiency0D* h = new TObjectDgEfficiency0D( name , "" , *this );
  h->Write( name );
  delete h;
}

void
DgEfficiency0D::load( const char* name , TDirectory* dir )
{
  dir->cd();
  const TObjectDgEfficiency0D* h = dynamic_cast<const TObjectDgEfficiency0D*>(dir->Get(name));
  assert( h );
  *this = *h;
  delete h;
}

void
DgEfficiency0D::merge( const DgAbsGraphic* other )
{
  const DgEfficiency0D* hother = dynamic_cast<const DgEfficiency0D*>(other);  assert(hother);
  _pass += hother->_pass;
  _total += hother->_total;
}

#include <TObject.h>
ClassImp(TObjectDgEfficiency0D)
