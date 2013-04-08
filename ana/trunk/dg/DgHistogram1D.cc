
#include <DataGraphics/DgHistogram1D.hh>
#include <DataGraphics/DgTypes.hh>

#include <cassert>
#include <cmath>
#include <vector>
#include <limits>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <string>
#include <boost/math/special_functions/fpclassify.hpp>
#include <TH1D.h>
#include <TDirectory.h>
#include <TAxis.h>

using namespace std;
using namespace DataGraphics;

DgHistogram1D::DgHistogram1D( const BinSize& nbins , const Value& min , const Value& max )
  : _nbins( nbins )
  , _min( min )
  , _max( max )
  , _binwidth( (max-min)/nbins )
  , _underflow( 0 )
  , _overflow( 0 )
  , _count( nbins )
{
  std::fill( _count.begin() , _count.end() , 0 );
}

DgHistogram1D::DgHistogram1D( const BinSize& nbins , const std::vector<Value>& bins )
  : _nbins( nbins )
  , _bins( bins )
  , _min( bins[0] )
  , _max( bins[nbins] )
  , _binwidth( (_max-_min)/_nbins ) // only average
  , _underflow( 0 )
  , _overflow( 0 )
  , _count( nbins )
{
  std::fill( _count.begin() , _count.end() , 0 );
}

DgHistogram1D::~DgHistogram1D()
{
}


void
DgHistogram1D::fill( const DataGraphics::Value& x )
{
  bool lox = x<_min;
  bool hix = x>=_max || (_max-std::numeric_limits<Value>::min()<=x) || boost::math::isnan(x);
  if( !(lox || hix) ) {
    ++_count[bin_index(x)];
    return;
  } else if( lox ) {
    ++_underflow;
    return;
  } else {
    ++_overflow;
    return;
  }
}

DataGraphics::Count
DgHistogram1D::integrate( const double xmin , const double xmax ) const
{
  BinSize ibin = 0;
  BinSize fbin = _nbins;
  if( xmin>=_min && xmin<_max ) { ibin = bin_index(xmin); }
  if( xmax<_max && xmax>=_min ) { fbin = bin_index(xmax); }
  vector<DataGraphics::Count>::const_iterator ibegin=_count.begin();
  vector<DataGraphics::Count>::const_iterator iend=_count.begin();
  advance(ibegin,ibin);
  advance(iend,fbin);
  DataGraphics::Count total( accumulate(ibegin,iend,DataGraphics::Count(0)) );
  return total;
}

DataGraphics::Value
DgHistogram1D::mean( const double xmin , const double xmax ) const
{
  DataGraphics::Count total( 0 );
  DataGraphics::Value sumx( 0. );
  BinSize ibin = 0;
  BinSize fbin = _nbins;
  if( xmin>=_min && xmin<_max ) { ibin = bin_index(xmin); }
  if( xmax<_max && xmax>=_min ) { ibin = bin_index(xmax); }
  for( ; ibin!=_nbins; ++ibin ) {
    total += _count[ibin];
    sumx += _count[ibin]*bin_center(ibin);
  }
  if( abs(total-0.)<1E-10 ) { return std::numeric_limits<double>::max(); }
  return( sumx/total );
}

DataGraphics::Value
DgHistogram1D::rms( const double xmin , const double xmax ) const
{
  DataGraphics::Count total( 0 );
  DataGraphics::Value sumx( 0. );
  DataGraphics::Value sumxx( 0. );
  BinSize ibin = 0;
  BinSize fbin = _nbins;
  if( xmin>=_min && xmin<_max ) { ibin = bin_index(xmin); }
  if( xmax<_max && xmax>=_min ) { ibin = bin_index(xmax); }
  for( ; ibin!=_nbins; ++ibin ) {
    total += _count[ibin];
    double binx = bin_center(ibin);
    sumx += _count[ibin]*binx;
    sumxx += _count[ibin]*binx*binx;
  }
  if( abs(total-0.)<1E-10 ) { return std::numeric_limits<double>::max(); }
  sumx /= total;
  sumxx /= total;
  return( sqrt(sumxx - (sumx*sumx)) );
}

TNamed*
DgHistogram1D::new_ROOT( const char* name ) const
{
  TH1D* h;
  if(_bins.empty()) {
    h = new TH1D( name , "" , _nbins , _min , _max );
  }
  else {
    Double_t *cv = new Double_t[_nbins+1];
    for(unsigned int i=0;i<_nbins+1;i++) cv[i] = _bins[i];
    h = new TH1D( name , "" , _nbins , cv );
    delete [] cv;
  }
  Count total = 0;
  for( int ibin = 0; ibin<_nbins; ++ibin ) {
    h->SetBinContent( ibin+1 , _count[ibin] );
    total += _count[ibin];
  }
  if( _axis_label != "" ) {
    h->GetXaxis()->SetTitle( _axis_label.c_str() );
    h->GetYaxis()->SetTitle( "ENTRIES / BIN" );
  }
  h->SetBinContent( 0 , _underflow );
  h->SetBinContent( _nbins+1 , _overflow );
  total += _underflow + _overflow;
  h->SetEntries( total );
  h->SetDirectory(0);
  return h;
}

void
DgHistogram1D::save_as_ROOT( const char* name , TDirectory* dir ) const
{
  dir->cd();
  TNamed* h = new_ROOT(name);
  h->Write( name );
  delete h;
}

void
DgHistogram1D::save( const char* name , TDirectory* dir ) const
{
  dir->cd();
  TObjectDgHistogram1D* h = new TObjectDgHistogram1D( name , "" , *this );
  h->Write( name );
  delete h;
}

void
DgHistogram1D::load( const char* name , TDirectory* dir )
{
  dir->cd();
  const TObjectDgHistogram1D* h = dynamic_cast<const TObjectDgHistogram1D*>( dir->Get(name) );
  assert( h );
  *this = *h;
  delete h;
}

void
DgHistogram1D::merge( const DgAbsGraphic* other )
{
  const DgHistogram1D* hother = dynamic_cast<const DgHistogram1D*>(other);  assert(hother);
  assert( _nbins == hother->_nbins );
  assert( abs(_min-hother->_min)<abs(std::numeric_limits<Value>::min()) );
  assert( abs(_max-hother->_max)<abs(std::numeric_limits<Value>::min()) );
  assert( _count.size() == hother->_count.size() );
  vector<Count>::iterator j = _count.begin();
  for( vector<Count>::const_iterator i=hother->_count.begin(), f=hother->_count.end(); i!=f; ++i,++j ) {
    (*j) += (*i);
  }
  _underflow += hother->_underflow;
  _overflow += hother->_overflow;
}


#include <TObject.h>
ClassImp(TObjectDgHistogram1D)
