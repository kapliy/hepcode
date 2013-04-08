
#include <DataGraphics/DgEfficiency2D.hh>
#include <DataGraphics/DgHistogram2D.hh>
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
#include <TH2.h>
#include <TDirectory.h>
#include <TAxis.h>

using namespace std;
using namespace DataGraphics;

DgEfficiency2D::DgEfficiency2D( const BinSize& nbinsx , const Value& minx , const Value& maxx ,
                                const BinSize& nbinsy , const Value& miny , const Value& maxy ) 
  : DgHistogram2D(nbinsx,minx,maxx,nbinsy,miny,maxy) ,  _pass( nbinsx*nbinsy )
{
  std::fill( _pass.begin() , _pass.end() , 0 );
}

DgEfficiency2D::~DgEfficiency2D()
{
}

void
DgEfficiency2D::fille( const Value& x , const Value& y , const bool& yes )
{
  DgHistogram2D::fill( x , y );
  if( !yes ) { return; }
  const bool lox = x<_minx;
  const bool loy = y<_miny;
  const bool hix = x>=_maxx || boost::math::isnan(x);
  const bool hiy = y>=_maxy || boost::math::isnan(y);
  //  _r.update( x , y );
  if( !(lox||loy||hix||hiy) ) {
    ++_pass[bin_index(x,y)];
  }
}

double
DgEfficiency2D::numerator( const unsigned int& ibinx , const unsigned int& ibiny ) const 
{
  assert( ibinx<_nbinsx );
  assert( ibiny<_nbinsy );
  const unsigned int ibin = (ibiny*_nbinsx)+ibinx;
  assert( ibin < _pass.size() );
  return _pass[ibin];
}

double
DgEfficiency2D::denominator( const unsigned int& ibinx , const unsigned int& ibiny ) const 
{
  assert( ibinx<_nbinsx );
  assert( ibiny<_nbinsy );
  const unsigned int ibin = (ibiny*_nbinsx)+ibinx;
  assert( ibin < _pass.size() );
  return _count[ibin];
}

void
DgEfficiency2D::efficiency( const unsigned int& ibinx , const unsigned int& ibiny ,
                            double& xmin , double& xmax , 
                            double& ymin , double& ymax , 
                            double& central , double& err_low , double& err_high ) const
{
  assert( ibinx<_nbinsx );
  assert( ibiny<_nbinsy );
  const unsigned int ibin = (ibiny*_nbinsx)+ibinx;
  assert( ibin < _pass.size() );
  const Count& den = _count[ibin];
  const Count& num = _pass[ibin];
  FindEfficiencyWithAsymmetricError( num , den , central , err_low , err_high );
  xmin = bin_min_x(ibinx);
  xmax = bin_max_x(ibinx);
  ymin = bin_min_y(ibiny);
  ymax = bin_max_y(ibiny);
}

void
DgEfficiency2D::efficiency( const unsigned int& ibinx , const unsigned int& fbinx ,
                            const unsigned int& ibiny , const unsigned int& fbiny ,
                            double& xmin , double& xmax , 
                            double& ymin , double& ymax , 
                            double& central , double& err_low , double& err_high ) const
{
  assert( ibinx<_nbinsx );
  assert( fbinx<=_nbinsx );
  assert( ibinx<=fbinx );
  assert( ibiny<_nbinsy );
  assert( fbiny<=_nbinsy );
  assert( ibiny<=fbiny );
  // std::vector<Count>::const_iterator id=_count.begin();
  // std::vector<Count>::const_iterator fd=_count.begin();
  // std::vector<Count>::const_iterator in=_pass.begin();
  // std::vector<Count>::const_iterator fn=_pass.begin();
  // advance(id,ibin);
  // advance(fd,fbin);
  // advance(in,ibin);
  // advance(fn,fbin);
  // const Count den = std::accumulate( id , fd , static_cast<Count>(0) );
  // const Count num = std::accumulate( in , fn , static_cast<Count>(0) );
  // FindEfficiencyWithAsymmetricError( num , den , central , err_low , err_high );
  // xmin = bin_min(ibin);
  // xmax = bin_max(fbin-1);
  assert( !"unimplemented. please implement me." );
}

TNamed*
DgEfficiency2D::new_ROOT( const char* name ) const
{
  return new_ROOT(name,1,1);
}

TNamed*
DgEfficiency2D::new_ROOT( const char* name , 
                          const unsigned int& rebinx , const unsigned int& rebiny ) const
{
  assert( rebinx==1 || !"rebin not yet implemented." );
  assert( rebiny==1 || !"rebin not yet implemented." );
  // build 2D efficiency plot. if show_error = false, just make a 2D
  // colz histogram with the values without uncertainties. 
  const bool show_error = false;
  TH2D* heff = new TH2D( "tmphist" , "" , 
                         _nbinsx , _minx , _maxx ,
                         _nbinsy , _miny , _maxy );
  for( unsigned int ibiny=0; ibiny!=_nbinsy; ++ibiny ) {
    for( unsigned int ibinx=0; ibinx!=_nbinsx; ++ibinx ) {
      const unsigned int ibin = (ibiny*_nbinsx)+ibinx;
      const Count& den = _count[ibin];
      if( den <= 0 ) { continue; }
      Double_t eff , eff_min_err , eff_max_err , xmin , xmax , ymin , ymax;
      efficiency( ibinx , ibiny , xmin , xmax , ymin , ymax , eff , eff_min_err , eff_max_err );
      const double xmean = 0.5*(xmax+xmin);
      const double ymean = 0.5*(ymax+ymin);
      heff->SetBinContent( heff->GetXaxis()->FindBin(xmean) , heff->GetYaxis()->FindBin(ymean) , eff );
    }
  }
  return heff;
}

void
DgEfficiency2D::save_as_ROOT( const char* name , TDirectory* dir ) const
{
  dir->cd();
  TNamed* gr = new_ROOT( name );
  gr->Write( name );
  delete gr;
}

void
DgEfficiency2D::save( const char* name , TDirectory* dir ) const
{
  dir->cd();
  TObjectDgEfficiency2D* h = new TObjectDgEfficiency2D( name , "" , *this );
  h->Write( name );
  delete h;
}

void
DgEfficiency2D::load( const char* name , TDirectory* dir )
{
  dir->cd();
  const TObjectDgEfficiency2D* h = dynamic_cast<const TObjectDgEfficiency2D*>(dir->Get(name));
  assert( h );
  *this = *h;
  delete h;
}

void
DgEfficiency2D::merge( const DgAbsGraphic* other )
{
  const DgEfficiency2D* hother = dynamic_cast<const DgEfficiency2D*>(other);  assert(hother);
  DgHistogram2D::merge( other );
  assert( _pass.size() == hother->_pass.size() );
  vector<Count>::iterator j = _pass.begin();
  for( vector<Count>::const_iterator i=hother->_pass.begin(), f=hother->_pass.end(); i!=f; ++i,++j ) {
    (*j) += (*i);
  }
}

#include <TObject.h>
ClassImp(TObjectDgEfficiency2D)
