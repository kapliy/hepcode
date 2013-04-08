
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

DgHistogram2D::DgHistogram2D( const BinSize& nbinsx , const Value& minx , const Value& maxx ,
                              const BinSize& nbinsy , const Value& miny , const Value& maxy )
  : _nbinsx( nbinsx )
  , _nbinsy( nbinsy )
  , _minx( minx )
  , _maxx( maxx )
  , _miny( miny )
  , _maxy( maxy )
  , _binwidthx( (maxx-minx)/nbinsx )
  , _binwidthy( (maxy-miny)/nbinsy )
  , _count( static_cast<BinIndex>(nbinsx)*nbinsy )
  , _outofbounds_xminymin( 0 )
  , _outofbounds_xmin( 0 )
  , _outofbounds_xminymax( 0 )
  , _outofbounds_ymin( 0 )
  , _outofbounds_ymax( 0 )
  , _outofbounds_xmaxymin( 0 )
  , _outofbounds_xmax( 0 )
  , _outofbounds_xmaxymax( 0 )
    //  , _r()
{
  std::fill( _count.begin() , _count.end() , 0 );
}

DgHistogram2D::~DgHistogram2D()
{
}

void
DgHistogram2D::fill( const Value& x , const Value& y )
{
  bool lox = x<_minx;
  bool loy = y<_miny;
  bool hix = x>=_maxx || boost::math::isnan(x);
  bool hiy = y>=_maxy || boost::math::isnan(y);
  //  _r.update( x , y );
  if( !(lox||loy||hix||hiy) ) {
    ++_count[bin_index(x,y)];
    return;
  } else if( !(loy||hiy) ) {
    if( lox ) {
      ++_outofbounds_xmin;
      return;
    } else {
      ++_outofbounds_xmax;
      return;
    }
  } else if( !(lox||hix) ) {
    if( lox ) {
      ++_outofbounds_ymin;
      return;
    } else {
      ++_outofbounds_ymax;
      return;
    }
  } else if( lox && loy ) {
    ++_outofbounds_xminymin;
    return;
  } else if( lox && hiy ) {
    ++_outofbounds_xminymax;
    return;
  } else if( hix && loy ) {
    ++_outofbounds_xmaxymin;
    return;
  } else {
    ++_outofbounds_xmaxymax;
    return;
  }
}

TNamed*
DgHistogram2D::new_ROOT( const char* name ) const
{
  TH2D* h = new TH2D( name , "" , _nbinsx , _minx , _maxx , _nbinsy , _miny , _maxy );
  Count total = 0;
  for( int ibiny = 0; ibiny<_nbinsy; ++ibiny ) {
    for( int ibinx = 0; ibinx<_nbinsx; ++ibinx ) {
      int ibin = (ibiny*_nbinsx)+ibinx;
      h->SetBinContent( ibinx+1 , ibiny+1 , _count[ibin] );
      total += _count[ibin];
    }
  }
  if( _axis_label_x != "" || _axis_label_y != "" ) {
    h->GetXaxis()->SetTitle( _axis_label_x.c_str() );
    h->GetYaxis()->SetTitle( _axis_label_y.c_str() );
  }
  // fill under/overflows.  note that ROOT tracks these in a grid, i.e.
  // there are 2*_nbinsx + 2*_nbinsy + 4 under/overflow bins.
  // since this class maintains 8 under/overflow bins, assign these
  // values to a subset of the ROOT overflow bins.  ROOT will
  // display the correct counts when viewing an entire histogram,
  // but it will display the wrong under/overflow counts when
  // zooming on either axis.
  h->SetBinContent( 0 , _outofbounds_xminymin );
  h->SetBinContent( 1 , _outofbounds_ymin );
  h->SetBinContent( _nbinsx+1 , _outofbounds_xmaxymin );
  h->SetBinContent( (1*(_nbinsx+2))+0 , _outofbounds_xmin );
  h->SetBinContent( (1*(_nbinsx+2))+_nbinsx+1 , _outofbounds_xmaxymin );
  h->SetBinContent( ((_nbinsy+1)*(_nbinsx+2))+0 , _outofbounds_xminymax );
  h->SetBinContent( ((_nbinsy+1)*(_nbinsx+2))+1 , _outofbounds_ymax );
  h->SetBinContent( ((_nbinsy+1)*(_nbinsx+2))+_nbinsx+1 , _outofbounds_xmaxymax );
  total += ( _outofbounds_xminymin + _outofbounds_ymin +
             _outofbounds_xmaxymin + _outofbounds_xmin +
             _outofbounds_xmaxymin + _outofbounds_xminymax +
             _outofbounds_ymax + _outofbounds_xmaxymax );
  h->SetEntries( total );
  h->SetDirectory(0);
  return h;
}

void
DgHistogram2D::save_as_ROOT( const char* name , TDirectory* dir ) const
{
  dir->cd();
  TNamed* h = new_ROOT( name );
  h->Write( name );
  delete h;
}

void
DgHistogram2D::save( const char* name , TDirectory* dir ) const
{
  dir->cd();
  TObjectDgHistogram2D* h = new TObjectDgHistogram2D( name , "" , *this );
  h->Write( name );
  delete h;
}

void
DgHistogram2D::load( const char* name , TDirectory* dir )
{
  dir->cd();
  const TObjectDgHistogram2D* h = dynamic_cast<const TObjectDgHistogram2D*>(dir->Get(name));
  assert( h );
  *this = *h;
  delete h;
}

void
DgHistogram2D::merge( const DgAbsGraphic* other )
{
  const DgHistogram2D* hother = dynamic_cast<const DgHistogram2D*>(other);  assert(hother);
  assert( _nbinsx == hother->_nbinsx );
  assert( _nbinsy == hother->_nbinsy );
  assert( abs(_minx-hother->_minx)<abs(std::numeric_limits<Value>::min()) );
  assert( abs(_maxx-hother->_maxx)<abs(std::numeric_limits<Value>::min()) );
  assert( abs(_miny-hother->_miny)<abs(std::numeric_limits<Value>::min()) );
  assert( abs(_maxy-hother->_maxy)<abs(std::numeric_limits<Value>::min()) );
  assert( _count.size() == hother->_count.size() );
  vector<Count>::iterator j = _count.begin();
  for( vector<Count>::const_iterator i=hother->_count.begin(), f=hother->_count.end(); i!=f; ++i,++j ) {
    (*j) += (*i);
  }
  _outofbounds_xminymin += hother->_outofbounds_xminymin;  
  _outofbounds_xmin += hother->_outofbounds_xmin;  
  _outofbounds_xminymax += hother->_outofbounds_xminymax;  
  _outofbounds_ymin += hother->_outofbounds_ymin;
  _outofbounds_ymax += hother->_outofbounds_ymax;
  _outofbounds_xmaxymin += hother->_outofbounds_xmaxymin;  
  _outofbounds_xmax += hother->_outofbounds_xmax;  
  _outofbounds_xmaxymax += hother->_outofbounds_xmaxymax;  
}

// calculate total integral over a range
const Count
DgHistogram2D::integrate( const double xmin , const double xmax , const double ymin , const double ymax ) const
{
  // figure out bin limits
  const BinSize ibinxmin = (xmin>=_minx && xmin<=_maxx) ? (BinSize)floor( (xmin-_minx)/_binwidthx ) : 0;
  const BinSize ibinxmax = (xmax>=_minx && xmax<=_maxx) ? (BinSize)floor( (xmax-_minx)/_binwidthx ) : (_nbinsx-1);
  const BinSize ibinymin = (ymin>=_miny && ymin<=_maxy) ? (BinSize)floor( (ymin-_miny)/_binwidthy ) : 0;
  const BinSize ibinymax = (ymax>=_miny && ymax<=_maxy) ? (BinSize)floor( (ymax-_miny)/_binwidthy ) : (_nbinsy-1);
  Count result = 0;
  for( BinSize iy=ibinymin; iy!=(ibinymax+1); ++iy ) {
    for( BinSize ix=ibinxmin; ix!=(ibinxmax+1); ++ix ) {
      const BinIndex ibin = (static_cast<BinIndex>(iy)*_nbinsx)+ix;
      assert( ibin<_count.size() );
      result += _count[ibin];
    }
  }
  return result;
}

// calculate mean of a range
const Value
DgHistogram2D::mean_x( const double xmin , const double xmax , const double ymin , const double ymax ) const
{
  return 0.;
}

const Value
DgHistogram2D::mean_y( const double xmin , const double xmax , const double ymin , const double ymax ) const
{
  return 0.;
}

// calculate rms of a range
const Value
DgHistogram2D::rms_x( const double xmin , const double xmax , const double ymin , const double ymax ) const
{
  return 0.;
}

const Value
DgHistogram2D::rms_y( const double xmin , const double xmax , const double ymin , const double ymax ) const
{
  return 0.;
}



#include <TObject.h>
ClassImp(TObjectDgHistogram2D)
