#ifndef WRAP_DGAXIS_HPP
#define WRAP_DGAXIS_HPP

#include <string>
#include <limits>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iterator>
#include <boost/optional.hpp>
#include "DataGraphics/DgTypes.hh"
#include "DataGraphics/DgCollection.hh"

class
DgAxis
{
public:
  typedef enum { BINNED , UNBINNED } Type;
private:
  std::string _name; // unique name
  std::string _label;
protected:
  DataGraphics::Value _range_min;
  DataGraphics::Value _range_max;
  Type _type;
protected:
  DgAxis( const std::string& name , const DataGraphics::Value& min , const DataGraphics::Value& max , const std::string& label )
    : _name( name )
    , _label( label )
    , _type( UNBINNED )
    , _range_min( min )
    , _range_max( max )
  {}
public:
  virtual ~DgAxis() {}
  static const DgAxis* ax( const std::string& name , const DataGraphics::Value& min , const DataGraphics::Value& max , const std::string& label ) {
    const DgAxis* x = DataGraphics::DgCollection::current()->axis(name);
    if( x ) { return x; }
    DgAxis* y = new DgAxis(name,min,max,label);
    DataGraphics::DgCollection::current()->add_axis( y );
    return y;
  }
  const std::string& name() const { return _name; }
  const std::string& label() const { return _label; }
  const Type& type() const { return _type; }
  //
  const DataGraphics::Value& range_min() const { return _range_min; }
  const DataGraphics::Value& range_max() const { return _range_max; }
  const DataGraphics::Value& min_in_scope() const { return _range_min; }
  const DataGraphics::Value max_in_scope() const { return _range_max-std::numeric_limits<DataGraphics::Value>::min(); }
  //
  const bool in_range( const DataGraphics::Value& x ) const { return( x>=_range_min && x <_range_max ); }
  const bool underflow( const DataGraphics::Value& x ) const { return( x<_range_min ); }
  const bool overflow( const DataGraphics::Value& x ) const { return( x>=_range_max ); }
};

class
DgBinAxis : public DgAxis
{
public:
  typedef enum { FIXED_WIDTH , SPECIFIED } BinType;
private:
  DataGraphics::BinSize _nbins;
  BinType _bin_type;
  boost::optional<DataGraphics::Value> _fixed_bin_width;
  std::vector<DataGraphics::Value> _bins;
protected:
  DgBinAxis( const std::string& name , const DataGraphics::BinSize& nbins , const DataGraphics::Value& min , const DataGraphics::Value& max , const std::string& label )
    : _nbins( nbins )
    , _bin_type( FIXED_WIDTH )
    , _fixed_bin_width( (max-min)/static_cast<DataGraphics::Value>(nbins) )
    , _bins()
    , DgAxis( name , min , max , label )
  { 
    _type = DgAxis::BINNED;
  }
  // n+1 numbers: minimum_edge, bin_boundary_1, bin_boundary_2, ... maxmimum_edge
  DgBinAxis( const std::string& name , const std::vector<DataGraphics::Value>& bin_edges , const std::string& label )
    : _nbins( bin_edges.size()-1 )
    , _bin_type( SPECIFIED )
    , _fixed_bin_width()
    , _bins( bin_edges )
    , DgAxis( name , bin_edges.front() , bin_edges.back() , label )
  { 
    _type = DgAxis::BINNED;
  }
public:
  virtual ~DgBinAxis() {}
  static const DgBinAxis* ax( const std::string& name , const DataGraphics::BinSize& nbins , const DataGraphics::Value& min , const DataGraphics::Value& max , const std::string& label ) {
    const DgAxis* x = DataGraphics::DgCollection::current()->axis(name);
    if( x ) {
      assert( x->type() == DgAxis::BINNED );
      return dynamic_cast<const DgBinAxis*>(x);
    }
    DgBinAxis* y = new DgBinAxis(name,nbins,min,max,label);
    DataGraphics::DgCollection::current()->add_axis( y );
    return y;
  }
  static const DgBinAxis* ax( const std::string& name , const std::vector<DataGraphics::Value>& bin_edges , const std::string& label ) {
    const DgAxis* x = DataGraphics::DgCollection::current()->axis(name);
    if( x ) {
      assert( x->type() == DgAxis::BINNED );
      return dynamic_cast<const DgBinAxis*>(x);
    }
    DgBinAxis* y = new DgBinAxis(name,bin_edges,label);
    DataGraphics::DgCollection::current()->add_axis( y );
    return y;
  }
  //
  const DataGraphics::BinSize& nbins() const { return _nbins; }
  const DataGraphics::Value bin_center( const DataGraphics::BinSize& ibin ) const { 
    if( _bins.empty() ) { 
      assert( _fixed_bin_width );
      return( _range_min + ((ibin+0.5)*(*_fixed_bin_width))); 
    }
    return( _bins[ibin] + 0.5*(_bins[ibin+1]-_bins[ibin]));
  }
  const DataGraphics::Value bin_min( const DataGraphics::BinSize& ibin ) const {
    if( _bins.empty() ) { 
      assert( _fixed_bin_width );
      return( _range_min + ((ibin)*(*_fixed_bin_width))); 
    }
    return _bins[ibin];
  }
  const DataGraphics::Value bin_max( const DataGraphics::BinSize& ibin ) const { 
    if( _bins.empty() ) { 
      assert( _fixed_bin_width );
      return( _range_min + ((ibin+1-std::numeric_limits<double>::min())*(*_fixed_bin_width))); 
    }
    return _bins[ibin+1];
  }
  const  DataGraphics::BinSize bin_index( const DataGraphics::Value& value ) const {
    if( _bins.empty() ) {
#ifndef DATA_GRAPHICS_DEBUG
      // floor may return _nbins if value is within floating-point
      // precision of _max. the caller should check for
      // over/underflow before attempting to retrieve the bin index
      // (the following assertions enforce this). therefore clip the return value of floor to _nbins-1.
      assert( value >= _min );
      assert( value < _max );
      return( std::min( static_cast<DataGraphics::BinSize>(std::floor( (value-_range_min)/(*_fixed_bin_width) )) , static_cast<DataGraphics::BinSize>(_nbins-1) ) );
#else
      assert( _fixed_bin_width );
      const DataGraphics::BinSize ibin( std::min( static_cast<DataGraphics::BinSize>(std::floor( (value-_range_min)/(*_fixed_bin_width) )) , static_cast<DataGraphics::BinSize>(_nbins-1) ) );
      assert( ibin>=0 );
      assert( ibin<_count.size() );
      return ibin;
#endif
    }
    if( underflow(value) ) { return 0; }
    if( overflow(value) ) { return _bins.size()-1; }
    std::vector<DataGraphics::Value>::const_iterator lb = std::lower_bound(_bins.begin(),_bins.end(),value+1e-9); // [first,last)
    const DataGraphics::BinSize ibin = static_cast<DataGraphics::BinSize>( std::distance(_bins.begin(),lb) ) - 1;
#ifdef DATA_GRAPHICS_DEBUG
    assert( ibin>=0 );
    assert( ibin<_bins.size()-1 );
#endif
    return ibin;
  }

};

#endif // WRAP_DGAXIS_HPP
