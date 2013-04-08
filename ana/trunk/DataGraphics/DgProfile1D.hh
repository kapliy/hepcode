#ifndef WRAP_DGPROFILE1D_HH
#define WRAP_DGPROFILE1D_HH

#include <DataGraphics/DgTypes.hh>
#include <DataGraphics/DgAbsGraphic.hh>

#include <vector>
#include <numeric>
#include <limits>
#include <algorithm>
#include <string>
#include <TNamed.h>
#include <cmath>
#ifndef __CINT__
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>
#endif

class TDirectory;
class TNamed;

namespace
DataGraphics
{

  // compact 1D profile histogram class with fixed-width binning
  class
  DgProfile1D : public DgAbsGraphic
  {
  protected:
    BinSize             _nbins;
    Value               _min;
    Value               _max;
    Value               _binwidth;
    std::vector<WeightedCount> _weights;
    std::vector<WeightedCount> _sum_y;
    std::vector<WeightedCount> _sum_yy;
    WeightedCount       _underflow;
    WeightedCount       _overflow;
    std::string _x_axis_label;
    std::string _y_axis_label;
  public:
    DgProfile1D() : _nbins(0) , _min(0) , _max(0) , _binwidth(0) , _underflow(0) , _overflow(0) {}
    DgProfile1D( const BinSize& nbins , const Value& min , const Value& max ); 
    DgProfile1D( const DgProfile1D& rhs );
    virtual ~DgProfile1D();
    void set_x_axis_label( const std::string& xtitle ) { _x_axis_label = xtitle; }
    void set_y_axis_label( const std::string& ytitle ) { _y_axis_label = ytitle; }
    const std::string get_x_axis_label() const { return _x_axis_label; }
    const std::string get_y_axis_label() const { return _y_axis_label; }
    const std::string get_axis_label() const { return _x_axis_label; }
    // fill the histogram
    virtual void fill( const Value& x ) {}
    virtual void fillw( const Value& x , const WeightedCount& w ) {}
    virtual void fill( const Value& x , const Value& y );
    virtual void fillw( const Value& x , const Value& y , const WeightedCount& w );
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
    virtual Value bin_center( const BinSize& ibin ) const { return( _min + ((ibin+0.5)*_binwidth)); }
    virtual Value bin_min( const BinSize& ibin ) const { return( _min + ((ibin)*_binwidth)); }
    virtual Value bin_max( const BinSize& ibin ) const { return( _min + ((ibin+1-std::numeric_limits<double>::min())*_binwidth)); }
    virtual BinSize bin_index( const Value& value ) const { 
      assert( value >= _min );
      assert( value < _max );
      return( std::min( static_cast<BinSize>(std::floor( (value-_min)/_binwidth )) , static_cast<BinSize>(_nbins-1) ) );
    }
    virtual BinSize nbins() const { return _nbins; }
    // get the data for a given bin
    virtual WeightedCount bin_weight( const BinSize& ibin ) const;
    virtual WeightedCount bin_sumy( const BinSize& ibin ) const;
    virtual WeightedCount bin_sumysq( const BinSize& ibin ) const;
    virtual WeightedCount bin_mean( const BinSize& ibin ) const;
    virtual WeightedCount bin_rms( const BinSize& ibin ) const;
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
  public:
#ifndef __CINT__
    template<class archiveT>
    void serialize( archiveT & ar , const unsigned int version ) {
      ar & _nbins;
      ar & _min;
      ar & _max;
      ar & _binwidth;
      ar & _weights;
      ar & _sum_y;
      ar & _sum_yy;
      ar & _underflow;
      ar & _overflow;
      ar & _x_axis_label;
      ar & _y_axis_label;
    }
#endif
  }; // end DgProfile1D

  // TNamed-derived streamer classes
  class TObjectDgProfile1D : public DgProfile1D , public TNamed
  {
  public:
    TObjectDgProfile1D()
      : TNamed()
      , DgProfile1D() {}
    TObjectDgProfile1D( const char* name , const char* description , 
                                const DgProfile1D& h1d )
      : TNamed(name,description)
      , DgProfile1D(h1d) {}
    
    ClassDef(TObjectDgProfile1D,2);
  };

};

#endif // WRAP_DGPROFILE1D_HH


