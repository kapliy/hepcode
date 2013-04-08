#ifndef WRAP_DGHISTOGRAM1D_HH
#define WRAP_DGHISTOGRAM1D_HH

#include <DataGraphics/DgTypes.hh>
#include <DataGraphics/DgAbsGraphic.hh>

#include <vector>
#include <numeric>
#include <limits>
#include <algorithm>
#include <TNamed.h>
#include <TMath.h>
#include <cmath>

#ifndef __CINT__
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/version.hpp>
#include <boost/serialization/base_object.hpp>
#endif
//#include <boost/flyweight.hpp>

class TDirectory;
class TNamed;

namespace
DataGraphics
{

  // compact 1D histogram class with fixed-width or variable-width binning
  class
  DgHistogram1D : public DgAbsGraphic
  {
  protected:
    BinSize             _nbins;
    std::vector<Value>  _bins;   // array of low edges of size (_nbins+1)
    Value               _min;
    Value               _max;
    Value               _binwidth;
    std::vector<Count>  _count;
    Count               _underflow;
    Count               _overflow;
    //     boost::flyweight<std::string> _axis_label;
    std::string _axis_label;
  public:
    DgHistogram1D() : _nbins(0) , _min(0) , _max(0) , _binwidth(0) , _underflow(0) , _overflow(0) {} // required by TObject
    DgHistogram1D( const BinSize& nbins , const Value& min , const Value& max );
    DgHistogram1D( const BinSize& nbins , const std::vector<Value>& bins); 
    virtual ~DgHistogram1D();
    void set_axis_label( const std::string& axis_label ) { _axis_label = axis_label; }
    const std::string get_axis_label() const { return _axis_label; }
    // fill the histogram
    virtual void fill( const Value& x );
    virtual void fillw( const Value& x , const WeightedCount& w ) {}
    virtual void fill( const Value& x , const Value& y ) {}
    virtual void fillw( const Value& x , const Value& y , const WeightedCount& w ) {}
    virtual void fille( const bool& yes ) {}
    virtual void fille( const Value& x , const bool& yes ) {}
    virtual void fillew( const bool& yes, const WeightedCount& w ) {}
    virtual void fillew( const Value& x , const bool& yes, const WeightedCount& w ) {}
    virtual void fillew( const Value& x , const Value& y, const bool& yes, const WeightedCount& w ) {}
    // save and load
    virtual TNamed* new_ROOT( const char* name ) const;
    virtual void save_as_ROOT( const char* name , TDirectory* directory ) const;
    virtual void save( const char* name , TDirectory* directory ) const;
    virtual void load( const char* name , TDirectory* directory );
    virtual void merge( const DgAbsGraphic* other );
    // get bin boundaries
    virtual BinSize nbins() const { return _nbins; }
    virtual Value bin_center( const BinSize& ibin ) const { 
      if(_bins.empty()) return( _min + ((ibin+0.5)*_binwidth));
      return( _bins[ibin] + 0.5*(_bins[ibin+1]-_bins[ibin]));
    }
    virtual Value bin_min( const BinSize& ibin ) const {
      if(_bins.empty()) return( _min + ((ibin)*_binwidth));
      return _bins[ibin];
    }
    virtual Value bin_max( const BinSize& ibin ) const { 
      if(_bins.empty()) return( _min + ((ibin+1-std::numeric_limits<double>::min())*_binwidth)); 
      return _bins[ibin+1];
    }
    virtual BinSize bin_index( const Value& value ) const {
      if( _bins.empty() ) {
        // floor may return _nbins if value is within floating-point
        // precision of _max. the caller should check for
        // over/underflow before attempting to retrieve the bin index
        // (the following assertions enforce this). therefore clip the return value of floor to _nbins-1.
        assert( value >= _min );
        assert( value < _max );
        return( std::min( static_cast<BinSize>(std::floor( (value-_min)/_binwidth )) , static_cast<BinSize>(_nbins-1) ) );
      }
      if( value<_min ) { return 0; }
      if( value>=_max ) { return _count.size() - 1; }
      std::vector<Value>::const_iterator lb = std::lower_bound(_bins.begin(),_bins.end(),value+1e-9); // [first,last)
      return( std::min( static_cast<BinSize>( std::distance(_bins.begin(),lb) - 1 ) , static_cast<BinSize>(_nbins-1) ) );
    }
    // integrate data over a range
    virtual Count integrate( const double xmin = -std::numeric_limits<double>::max() , 
                             const double xmax = std::numeric_limits<double>::max() ) const;
    // calculate mean of a range
    virtual Value mean( const double xmin = -std::numeric_limits<double>::max() , 
                        const double xmax = std::numeric_limits<double>::max() ) const;
    // calculate rms of a range
    virtual Value rms( const double xmin = -std::numeric_limits<double>::max() , 
                       const double xmax = std::numeric_limits<double>::max() ) const;
    // return underflow and overflow
    virtual Count underflow() const { return _underflow; }
    virtual Count overflow() const { return _overflow; }
    // minimum and maximum count
    Count bin_count( const BinSize& ibin ) const {
      assert( ibin<_nbins );
      return _count[ibin];
    }
    Count maximum_count() const { 
      std::vector<Count>::const_iterator i = max_element( _count.begin() , _count.end() );
      assert( i != _count.end() );
      return (*i);
    }
    Count minimum_count() const { 
      std::vector<Count>::const_iterator i = min_element( _count.begin() , _count.end() );
      assert( i != _count.end() );
      return (*i);
    }
  public:
#ifndef __CINT__
    friend class boost::serialization::access;
    template<class archiveT>
    void serialize( archiveT & ar , const unsigned int version ) {
      ar & boost::serialization::base_object<DgAbsGraphic>(*this);
      ar & _nbins;
      ar & _bins;
      ar & _min;
      ar & _max;
      ar & _binwidth;
      ar & _count;
      ar & _underflow;  
      ar & _overflow;
      ar & _axis_label;
    }
#endif
  }; // end DgHistogram1D

  // TNamed-derived streamer classes
  class TObjectDgHistogram1D : public DgHistogram1D , public TNamed
  {
  public:
    TObjectDgHistogram1D()
      : TNamed()
      , DgHistogram1D() {}
    TObjectDgHistogram1D( const char* name , const char* description , 
			  const DgHistogram1D& h1d )
      : TNamed(name,description)
      , DgHistogram1D(h1d) {}
    
    ClassDef(TObjectDgHistogram1D,3);
  };

};

#ifndef __CINT__
BOOST_CLASS_VERSION( DataGraphics::DgHistogram1D , 1 );
#endif

#endif // WRAP_DGHISTOGRAM1D_HH
