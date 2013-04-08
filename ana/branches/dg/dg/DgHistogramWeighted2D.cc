
#include <DataGraphics/DgHistogramWeighted2D.hh>
#include <DataGraphics/DgHistogram2D.hh>
#include <DataGraphics/DgTypes.hh>
#include <boost/math/special_functions/fpclassify.hpp>

#include <cassert>
#include <cmath>
#include <vector>
#include <algorithm>
#include <iterator>
#include <string.h>
#include <TH2D.h>
#include <TDirectory.h>

using namespace std;
using namespace DataGraphics;

DgHistogramWeighted2D::DgHistogramWeighted2D( const BinSize& nbinsx , const Value& minx , const Value& maxx ,
					      const BinSize& nbinsy , const Value& miny , const Value& maxy )
  : DgHistogram2D(nbinsx,minx,maxx,nbinsy,miny,maxy)
  , _weighted_count( nbinsx*nbinsy )
  , _weighted_outofbounds_xminymin( 0 )
  , _weighted_outofbounds_xmin( 0 )
  , _weighted_outofbounds_xminymax( 0 )
  , _weighted_outofbounds_ymin( 0 )
  , _weighted_outofbounds_ymax( 0 )
  , _weighted_outofbounds_xmaxymin( 0 )
  , _weighted_outofbounds_xmax( 0 )
  , _weighted_outofbounds_xmaxymax( 0 )
  , _nentries( 0 )
{
  std::fill( _weighted_count.begin() , _weighted_count.end() , 0 );
}

DgHistogramWeighted2D::~DgHistogramWeighted2D()
{
}

void
DgHistogramWeighted2D::fillw( const Value& x , const Value& y, const DataGraphics::WeightedCount& weight )
{
  bool lox = x<_minx;
  bool loy = y<_miny;
  bool hix = x>=_maxx || boost::math::isnan(x);
  bool hiy = y>=_maxy || boost::math::isnan(y);
  _nentries += 1;
  if( !(lox||loy||hix||hiy) ) {
    ++_count[bin_index(x,y)];
    _weighted_count[bin_index(x,y)]+=weight;
    return;
  } else if( !(loy||hiy) ) {
    if( lox ) {
      ++_outofbounds_xmin;
      _weighted_outofbounds_xmin+=weight;
      return;
    } else {
      ++_outofbounds_xmax;
      _weighted_outofbounds_xmax+=weight;
      return;
    }
  } else if( !(lox||hix) ) {
    if( lox ) {
      ++_outofbounds_ymin;
      _weighted_outofbounds_ymin+=weight;
      return;
    } else {
      ++_outofbounds_ymax;
      _weighted_outofbounds_ymax+=weight;
      return;
    }
  } else if( lox && loy ) {
    ++_outofbounds_xminymin;
    _weighted_outofbounds_xminymin+=weight;
    return;
  } else if( lox && hiy ) {
    ++_outofbounds_xminymax;
    _weighted_outofbounds_xminymax+=weight;
    return;
  } else if( hix && loy ) {
    ++_outofbounds_xmaxymin;
    _weighted_outofbounds_xmaxymin+=weight;
    return;
  } else {
    ++_outofbounds_xmaxymax;
    _weighted_outofbounds_xmaxymax+=weight;
    return;
  }
}

TNamed*
DgHistogramWeighted2D::new_ROOT( const char* name ) const
{
  TH2D* h = new TH2D( name , "" , _nbinsx , _minx , _maxx , _nbinsy , _miny , _maxy );
  for( int ibiny = 0; ibiny<_nbinsy; ++ibiny ) {
    for( int ibinx = 0; ibinx<_nbinsx; ++ibinx ) {
      int ibin = (ibiny*_nbinsx)+ibinx;
      h->SetBinContent( ibinx+1 , ibiny+1 , _weighted_count[ibin] );
    }
  }
  // fill under/overflows.  note that ROOT tracks these in a grid, i.e.
  // there are 2*_nbinsx + 2*_nbinsy + 4 under/overflow bins.
  // since this class maintains 8 under/overflow bins, assign these
  // values to a subset of the ROOT overflow bins.  ROOT will
  // display the correct counts when viewing an entire histogram,
  // but it will display the wrong under/overflow counts when
  // zooming on either axis.
  h->SetBinContent( 0 , _weighted_outofbounds_xminymin );
  h->SetBinContent( 1 , _weighted_outofbounds_ymin );
  h->SetBinContent( _nbinsx+1 , _weighted_outofbounds_xmaxymin );
  h->SetBinContent( (1*(_nbinsx+2))+0 , _weighted_outofbounds_xmin );
  h->SetBinContent( (1*(_nbinsx+2))+_nbinsx+1 , _weighted_outofbounds_xmaxymin );
  h->SetBinContent( ((_nbinsy+1)*(_nbinsx+2))+0 , _weighted_outofbounds_xminymax );
  h->SetBinContent( ((_nbinsy+1)*(_nbinsx+2))+1 , _weighted_outofbounds_ymax );
  h->SetBinContent( ((_nbinsy+1)*(_nbinsx+2))+_nbinsx+1 , _weighted_outofbounds_xmaxymax );
  h->SetEntries( _nentries );
  h->SetDirectory(0);
  return h;
}

void
DgHistogramWeighted2D::save_as_ROOT( const char* name , TDirectory* dir ) const
{
  dir->cd();
  TNamed* h = new_ROOT( name );
  h->Write( name );
  delete h;
}

void
DgHistogramWeighted2D::save( const char* name , TDirectory* dir ) const
{
  dir->cd();
  TObjectDgHistogramWeighted2D* h = new TObjectDgHistogramWeighted2D( name , "" , *this );
  h->Write( name );
  delete h;
}

void
DgHistogramWeighted2D::load( const char* name , TDirectory* dir )
{
  dir->cd();
  const TObjectDgHistogramWeighted2D* h = dynamic_cast<const TObjectDgHistogramWeighted2D*>(dir->Get(name));
  assert( h );
  *this = *h;
  delete h;
}

void
DgHistogramWeighted2D::merge( const DgAbsGraphic* other )
{
  const DgHistogramWeighted2D* hother = dynamic_cast<const DgHistogramWeighted2D*>(other);  assert(hother);
  DgHistogram2D::merge( other );
  assert( _nbinsx == hother->_nbinsx );
  assert( _nbinsy == hother->_nbinsy );
  assert( abs(_minx-hother->_minx)<abs(std::numeric_limits<Value>::min()) );
  assert( abs(_maxx-hother->_maxx)<abs(std::numeric_limits<Value>::min()) );
  assert( abs(_miny-hother->_miny)<abs(std::numeric_limits<Value>::min()) );
  assert( abs(_maxy-hother->_maxy)<abs(std::numeric_limits<Value>::min()) );
  assert( _weighted_count.size() == hother->_weighted_count.size() );
  vector<WeightedCount>::iterator j = _weighted_count.begin();
  for( vector<WeightedCount>::const_iterator i=hother->_weighted_count.begin(), f=hother->_weighted_count.end(); i!=f; ++i,++j ) {
    (*j) += (*i);
  }
  _weighted_outofbounds_xminymin += hother->_weighted_outofbounds_xminymin;  
  _weighted_outofbounds_xmin += hother->_weighted_outofbounds_xmin;  
  _weighted_outofbounds_xminymax += hother->_weighted_outofbounds_xminymax;  
  _weighted_outofbounds_ymin += hother->_weighted_outofbounds_ymin;
  _weighted_outofbounds_ymax += hother->_weighted_outofbounds_ymax;
  _weighted_outofbounds_xmaxymin += hother->_weighted_outofbounds_xmaxymin;  
  _weighted_outofbounds_xmax += hother->_weighted_outofbounds_xmax;  
  _weighted_outofbounds_xmaxymax += hother->_weighted_outofbounds_xmaxymax;  
  _nentries += hother->_nentries;
}

// calculate total integral over a range
DataGraphics::WeightedCount
DgHistogramWeighted2D::integrate_weight( const double xmin , const double xmax , const double ymin , const double ymax ) const
{
  // figure out bin limits
  const BinSize ibinxmin = (xmin>=_minx && xmin<=_maxx) ? (BinSize)floor( (xmin-_minx)/_binwidthx ) : 0;
  const BinSize ibinxmax = (xmax>=_minx && xmax<=_maxx) ? (BinSize)floor( (xmax-_minx)/_binwidthx ) : (_nbinsx-1);
  const BinSize ibinymin = (ymin>=_miny && ymin<=_maxy) ? (BinSize)floor( (ymin-_miny)/_binwidthy ) : 0;
  const BinSize ibinymax = (ymax>=_miny && ymax<=_maxy) ? (BinSize)floor( (ymax-_miny)/_binwidthy ) : (_nbinsy-1);
  WeightedCount result = 0;
  for( BinSize iy=ibinymin; iy!=(ibinymax+1); ++iy ) {
    for( BinSize ix=ibinxmin; ix!=(ibinxmax+1); ++ix ) {
      const BinSize ibin = (iy*_nbinsx)+ix;
      assert( ibin<_weighted_count.size() );
      result += _weighted_count[ibin];
    }
  }
  return result;
}

// calculate mean of a range
Value 
DgHistogramWeighted2D::mean_x_weight( const double xmin , const double xmax , const double ymin , const double ymax ) const
{
  return 0.;
}

Value 
DgHistogramWeighted2D::mean_y_weight( const double xmin , const double xmax , const double ymin , const double ymax ) const
{
  return 0.;
}

#include <TObject.h>
ClassImp(TObjectDgHistogramWeighted2D)
