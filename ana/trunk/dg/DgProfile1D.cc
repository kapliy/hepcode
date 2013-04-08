
#include <DataGraphics/DgProfile1D.hh>
#include <DataGraphics/DgTypes.hh>
#include <boost/math/special_functions/fpclassify.hpp>

#include <cassert>
#include <cmath>
#include <vector>
#include <limits>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <string.h>
#include <TProfile.h>
#include <TDirectory.h>

using namespace std;
using namespace DataGraphics;

DgProfile1D::DgProfile1D( const BinSize& nbins , const Value& min , const Value& max )
  : _nbins( nbins )
  , _min( min )
  , _max( max )
  , _binwidth( (max-min)/nbins )
  , _underflow( 0 )
  , _overflow( 0 )
  , _weights( nbins )
  , _sum_y( nbins )
  , _sum_yy( nbins )
{
  std::fill( _weights.begin() , _weights.end() , 0 );
  std::fill( _sum_y.begin() , _sum_y.end() , 0 );
  std::fill( _sum_yy.begin() , _sum_yy.end() , 0 );
}

DgProfile1D::DgProfile1D( const DgProfile1D& rhs )
{
  _nbins = rhs._nbins;
  _min = rhs._min;
  _max = rhs._max;
  _binwidth = rhs._binwidth;
  _weights = rhs._weights;
  _sum_y = rhs._sum_y;
  _sum_yy = rhs._sum_yy;
  _underflow = rhs._underflow;
  _overflow = rhs._overflow;  
}

DgProfile1D::~DgProfile1D()
{
}


void
DgProfile1D::fill( const DataGraphics::Value& x , const DataGraphics::Value& y )
{
  bool lox = x<_min;
  bool hix = x>=_max || boost::math::isnan(x);
  if( !(lox || hix) ) {
    BinSize ibin = bin_index(x);
    ++(_weights[ibin]);
    _sum_y[ibin] += y;
    _sum_yy[ibin] += (y*y);
    return;
  } else if( lox ) {
    ++_underflow;
    return;
  } else {
    ++_overflow;
    return;
  }
}

void
DgProfile1D::fillw( const DataGraphics::Value& x , const DataGraphics::Value& y , const DataGraphics::WeightedCount& weight )
{
  bool lox = x<_min;
  bool hix = x>=_max || boost::math::isnan(x);
  if( !(lox || hix) ) {
    BinSize ibin = bin_index(x);
    _weights[ibin] += weight;
    _sum_y[ibin] += y*weight;
    _sum_yy[ibin] += (y*y)*weight;
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
DgProfile1D::integrate( const double xmin , const double xmax ) const
{
//   BinSize ibin = 0;
//   BinSize fbin = _nbins;
//   if( xmin>=_min && xmin<_max ) { ibin = bin_index(xmin); }
//   if( xmax<_max && xmax>=_min ) { fbin = bin_index(xmax); }
//   vector<DataGraphics::Count>::const_iterator ibegin=_count.begin();
//   vector<DataGraphics::Count>::const_iterator iend=_count.begin();
//   advance(ibegin,ibin);
//   advance(iend,fbin);
//   DataGraphics::Count total( accumulate(ibegin,iend,DataGraphics::Count(0)) );
//   return total;
  return 0.;
}

DataGraphics::Value
DgProfile1D::mean( const double xmin , const double xmax ) const
{
//   DataGraphics::Count total( 0 );
//   DataGraphics::Value sumx( 0. );
//   BinSize ibin = 0;
//   BinSize fbin = _nbins;
//   if( xmin>=_min && xmin<_max ) { ibin = bin_index(xmin); }
//   if( xmax<_max && xmax>=_min ) { ibin = bin_index(xmax); }
//   for( ; ibin!=_nbins; ++ibin ) {
//     total += _count[ibin];
//     sumx += _count[ibin]*bin_center(ibin);
//   }
//   if( abs(total-0.)<1E-10 ) { return std::numeric_limits<double>::max(); }
//   return( sumx/total );
  return 0.;
}

DataGraphics::Value
DgProfile1D::rms( const double xmin , const double xmax ) const
{
//   DataGraphics::Count total( 0 );
//   DataGraphics::Value sumx( 0. );
//   DataGraphics::Value sumxx( 0. );
//   BinSize ibin = 0;
//   BinSize fbin = _nbins;
//   if( xmin>=_min && xmin<_max ) { ibin = bin_index(xmin); }
//   if( xmax<_max && xmax>=_min ) { ibin = bin_index(xmax); }
//   for( ; ibin!=_nbins; ++ibin ) {
//     total += _count[ibin];
//     double binx = bin_center(ibin);
//     sumx += _count[ibin]*binx;
//     sumxx += _count[ibin]*binx*binx;
//   }
//   if( abs(total-0.)<1E-10 ) { return std::numeric_limits<double>::max(); }
//   sumx /= total;
//   sumxx /= total;
//   return( sqrt(sumxx - (sumx*sumx)) );
  return 0.;
}

TNamed*
DgProfile1D::new_ROOT( const char* name ) const
{
  TProfile* p = new TProfile( name , "" , _nbins , _min , _max );
  assert( _sum_y.size() == _nbins );
  for( BinSize ibin = 0; ibin!=_nbins; ++ibin ) {
    // compute mean and uncertainty
    const WeightedCount weight = _weights[ibin];
    if( weight<=0. ) { continue; }
    const WeightedCount mean_y = _sum_y[ibin] / weight;
    const WeightedCount mean_yy = _sum_yy[ibin] / weight;
    const WeightedCount rms_y = std::sqrt( mean_yy - (mean_y*mean_y) );
    p->SetBinContent( ibin+1 , mean_y );
    p->SetBinError( ibin+1 , rms_y / sqrt( weight ) ); // uncertainty on a gaussian mean
  }
  if( _x_axis_label != "" || _y_axis_label != "" ) {
    p->GetXaxis()->SetTitle( _x_axis_label.c_str() );
    p->GetYaxis()->SetTitle( _y_axis_label.c_str() );
  }
  p->SetDirectory(0);
  return p;
}

void
DgProfile1D::save_as_ROOT( const char* name , TDirectory* dir ) const
{
  dir->cd();
  TNamed* h = new_ROOT(name);
  h->Write( name );
  delete h;
}

void
DgProfile1D::save( const char* name , TDirectory* dir ) const
{
  dir->cd();
  TObjectDgProfile1D* h = new TObjectDgProfile1D( name , "" , *this );
  h->Write( name );
  delete h;
}

void
DgProfile1D::load( const char* name , TDirectory* dir )
{
  dir->cd();
  const TObjectDgProfile1D* h = dynamic_cast<const TObjectDgProfile1D*>( dir->Get(name) );
  assert( h );
  *this = *h;
  delete h;
}

void
DgProfile1D::merge( const DgAbsGraphic* other )
{
  const DgProfile1D* hother = dynamic_cast<const DgProfile1D*>(other);  assert(hother);
  assert( _nbins == hother->_nbins );
  assert( abs(_min-hother->_min)<abs(std::numeric_limits<Value>::min()) );
  assert( abs(_max-hother->_max)<abs(std::numeric_limits<Value>::min()) );
  assert( _weights.size() == hother->_weights.size() );
  assert( _sum_y.size() == hother->_sum_y.size() );
  assert( _sum_yy.size() == hother->_sum_yy.size() );
  {
    vector<WeightedCount>::iterator j = _weights.begin();
    for( vector<WeightedCount>::const_iterator i=hother->_weights.begin(), f=hother->_weights.end(); i!=f; ++i,++j ) {
      (*j) += (*i);
    }
  }
  {
    vector<WeightedCount>::iterator j = _sum_y.begin();
    for( vector<WeightedCount>::const_iterator i=hother->_sum_y.begin(), f=hother->_sum_y.end(); i!=f; ++i,++j ) {
      (*j) += (*i);
    }
  }
  {
    vector<WeightedCount>::iterator j = _sum_yy.begin();
    for( vector<WeightedCount>::const_iterator i=hother->_sum_yy.begin(), f=hother->_sum_yy.end(); i!=f; ++i,++j ) {
      (*j) += (*i);
    }
  }
  _underflow += hother->_underflow;
  _overflow += hother->_overflow;
}

WeightedCount
DgProfile1D::bin_weight( const BinSize& ibin ) const
{
  assert( ibin<=_nbins );
  return( _weights[ibin] );
}

WeightedCount
DgProfile1D::bin_sumy( const BinSize& ibin ) const
{
  assert( ibin<=_nbins );
  return( _sum_y[ibin] );
}

WeightedCount
DgProfile1D::bin_sumysq( const BinSize& ibin ) const
{
  assert( ibin<=_nbins );
  return( _sum_yy[ibin] );
}

WeightedCount
DgProfile1D::bin_mean( const BinSize& ibin ) const
{
  assert( ibin<=_nbins );
  if( std::abs(_weights[ibin]) < std::numeric_limits<WeightedCount>::min() ) { return std::numeric_limits<WeightedCount>::max(); }
  return( _sum_y[ibin] / _weights[ibin] );
}

WeightedCount
DgProfile1D::bin_rms( const BinSize& ibin ) const
{
  assert( ibin<=_nbins );
  if( std::abs(_weights[ibin]) < std::numeric_limits<WeightedCount>::min() ) { return std::numeric_limits<WeightedCount>::max(); }
  const double meany = _sum_y[ibin] / _weights[ibin];
  const double meanyy = _sum_yy[ibin] / _weights[ibin];
  const double rmssq = meanyy - (meany*meany);
  if( std::abs(rmssq) < 1E-6 ) { return 0.; }
  if( rmssq < 0. ) { return std::numeric_limits<WeightedCount>::max(); }
  return( std::sqrt(rmssq) );
}


#include <TObject.h>
ClassImp(TObjectDgProfile1D)
