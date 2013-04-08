
#include <DataGraphics/DgHistogramWeighted1D.hh>
#include <DataGraphics/DgHistogram1D.hh>
#include <DataGraphics/DgTypes.hh>
#include <boost/math/special_functions/fpclassify.hpp>

#include <cassert>
#include <cmath>
#include <vector>
#include <algorithm>
#include <iterator>
#include <string.h>
#include <TH1D.h>
#include <TMath.h>
#include <TDirectory.h>

using namespace std;
using namespace DataGraphics;

DgHistogramWeighted1D::DgHistogramWeighted1D( const BinSize& nbins , const Value& min , const Value& max )
  : DgHistogram1D(nbins,min,max) , _weighted_count(nbins) ,
    _weighted_underflow(0) , _weighted_overflow(0) , _nentries(0)
{
  std::fill( _weighted_count.begin() , _weighted_count.end() , 0. );
}

DgHistogramWeighted1D::DgHistogramWeighted1D( const BinSize& nbins , const std::vector<Value>& bins)
  : DgHistogram1D(nbins,bins) , _weighted_count(nbins) ,
    _weighted_underflow(0) , _weighted_overflow(0) , _nentries(0)
{
  std::fill( _weighted_count.begin() , _weighted_count.end() , 0. );
}

DgHistogramWeighted1D::~DgHistogramWeighted1D()
{
}

void
DgHistogramWeighted1D::fillw( const DataGraphics::Value& x , const DataGraphics::WeightedCount& weight )
{
  bool lox = x<_min;
  bool hix = x>=_max || (_max-std::numeric_limits<Value>::min()<=x) || boost::math::isnan(x);
  _nentries += 1;
  if( !(lox || hix) ) {
    _count[bin_index(x)] += weight*weight;
    _weighted_count[bin_index(x)] += weight;
    return;
  } else if( lox ) {
    _underflow += weight*weight;
    _weighted_underflow += weight;
    return;
  } else {
    _overflow += weight*weight;
    _weighted_overflow += weight;
    return;
  }
}

DataGraphics::WeightedCount
DgHistogramWeighted1D::integrate_weight( const double xmin , const double xmax ) const
{
  BinSize ibin = 0;
  BinSize fbin = _nbins;
  if( xmin>=_min && xmin<_max ) { ibin = bin_index(xmin); }
  if( xmax<_max && xmax>=_min ) { fbin = bin_index(xmax); }
  vector<DataGraphics::WeightedCount>::const_iterator ibegin=_weighted_count.begin();
  vector<DataGraphics::WeightedCount>::const_iterator iend=_weighted_count.begin();
  advance(ibegin,ibin);
  advance(iend,fbin);
  DataGraphics::WeightedCount total( accumulate(ibegin,iend,DataGraphics::WeightedCount(0.)) );
  return total;
}

DataGraphics::Value
DgHistogramWeighted1D::mean_weight( const double xmin , const double xmax ) const
{
  DataGraphics::WeightedCount total( 0. );
  DataGraphics::Value sumx( 0. );
  BinSize ibin = 0;
  BinSize fbin = _nbins;
  if( xmin>=_min && xmin<_max ) { ibin = bin_index(xmin); }
  if( xmax<_max && xmax>=_min ) { ibin = bin_index(xmax); }
  for( ; ibin!=_nbins; ++ibin ) {
    total += _weighted_count[ibin];
    sumx += _weighted_count[ibin]*bin_center(ibin);
  }
  if( abs(total-0.)<1E-10 ) { return std::numeric_limits<double>::max(); }
  return( sumx/total );
}

TNamed*
DgHistogramWeighted1D::new_ROOT( const char* name ) const
{
  TH1D* h;
  if(_bins.empty()) {
    h = new TH1D( name , "" , _nbins , _min , _max );
  }
  else {
    Count *cv = new Count[_nbins+1];
    for(unsigned int i=0;i<_nbins+1;i++) cv[i] = _bins[i];
    h = new TH1D( name , "" , _nbins , cv );
    delete[] cv;
  }
  h->Sumw2();
  for( int ibin = 0; ibin<_nbins; ++ibin ) {
    h->SetBinContent( ibin+1 , _weighted_count[ibin] );
    h->SetBinError( ibin+1 , TMath::Sqrt(_count[ibin]) );
  }
  h->SetBinContent( 0 , _weighted_underflow );
  h->SetBinError( 0 , TMath::Sqrt(_underflow) );
  h->SetBinContent( _nbins+1 , _weighted_overflow );
  h->SetBinError( _nbins+1 , TMath::Sqrt(_overflow) );
  h->SetEntries( _nentries );
  h->SetDirectory(0);
  return h;
}

void
DgHistogramWeighted1D::save_as_ROOT( const char* name , TDirectory* dir ) const
{
  dir->cd();
  TNamed* h = new_ROOT(name);
  h->Write( name );
  delete h;
}

void
DgHistogramWeighted1D::save( const char* name , TDirectory* dir ) const
{
  dir->cd();
  TObjectDgHistogramWeighted1D* h = new TObjectDgHistogramWeighted1D( name , "" , *this );
  h->Write( name );
  delete h;
}

void
DgHistogramWeighted1D::load( const char* name , TDirectory* dir )
{
  dir->cd();
  const TObjectDgHistogramWeighted1D* h = dynamic_cast<const TObjectDgHistogramWeighted1D*>(dir->Get(name));
  assert( h );
  *this = *h;
  delete h;
}

void
DgHistogramWeighted1D::merge( const DgAbsGraphic* other )
{
  const DgHistogramWeighted1D* hother = dynamic_cast<const DgHistogramWeighted1D*>(other);  assert(hother);
  DgHistogram1D::merge( other );
  assert( _weighted_count.size() == hother->_weighted_count.size() );
  vector<WeightedCount>::iterator j = _weighted_count.begin();
  for( vector<WeightedCount>::const_iterator i=hother->_weighted_count.begin(), f=hother->_weighted_count.end(); i!=f; ++i,++j ) {
    (*j) += (*i);
  }
  _weighted_underflow += hother->_weighted_underflow;
  _weighted_overflow += hother->_weighted_overflow;
  _nentries += hother->_nentries;
}


#include <TObject.h>
ClassImp(TObjectDgHistogramWeighted1D)
