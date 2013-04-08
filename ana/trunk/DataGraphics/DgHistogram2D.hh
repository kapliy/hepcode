#ifndef WRAP_DGHISTOGRAM2D_HH
#define WRAP_DGHISTOGRAM2D_HH

#include <DataGraphics/DgTypes.hh>
#include <DataGraphics/DgAbsGraphic.hh>
//#include <DataGraphics/DgStatistics.hh>

#include <vector>
#include <numeric>
#include <limits>
#include <algorithm>
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
  // compact 2D histogram class with fixed-width binning
  class
  DgHistogram2D : public DgAbsGraphic
  {
  protected:
    BinSize _nbinsx;
    BinSize _nbinsy;
    Value   _minx;
    Value   _maxx;
    Value   _miny;
    Value   _maxy;
    Value   _binwidthx;
    Value   _binwidthy;
    std::vector<Count>  _count;
    Count   _outofbounds_xminymin;  
    Count   _outofbounds_xmin;  
    Count   _outofbounds_xminymax;  
    Count   _outofbounds_ymin;
    Count   _outofbounds_ymax;
    Count   _outofbounds_xmaxymin;  
    Count   _outofbounds_xmax;  
    Count   _outofbounds_xmaxymax;  
    std::string _axis_label_x;
    std::string _axis_label_y;
    //mutable DgStatPearsonCoeff _r;
    //Value _r_value;
  public:
    DgHistogram2D() 
      : _nbinsx(0) , _nbinsy(0) , _minx(0) , _maxx(0) , _miny(0) , _maxy(0) , _binwidthx(0) , _binwidthy(0)
      , _count() , _outofbounds_xminymin(0) , _outofbounds_xmin(0) , _outofbounds_xminymax(0) , _outofbounds_ymin(0)
      , _outofbounds_ymax(0) , _outofbounds_xmaxymin(0) , _outofbounds_xmax(0) , _outofbounds_xmaxymax(0)
      , _axis_label_x("") , _axis_label_y("") {}
    
    DgHistogram2D( const BinSize& nbinsx , const Value& minx , const Value& maxx ,
                   const BinSize& nbinsy , const Value& miny , const Value& maxy );
    virtual ~DgHistogram2D();
    void set_x_axis_label( const std::string& xtitle ) { _axis_label_x = xtitle; }
    void set_y_axis_label( const std::string& ytitle ) { _axis_label_y = ytitle; }
    const std::string get_x_axis_label() const { return _axis_label_x; }
    const std::string get_y_axis_label() const { return _axis_label_y; }
    virtual void fill( const Value& x ) {}
    virtual void fillw( const Value& x , const WeightedCount& w ) {}
    virtual void fill( const Value& x , const Value& y );
    virtual void fillw( const Value& x , const Value& y , const WeightedCount& w ) {}
    virtual void fille( const bool& yes ) {}
    virtual void fille( const Value& x , const bool& yes ) {}
    virtual void fillew( const bool& yes, const WeightedCount& w ) {}
    virtual void fillew( const Value& x , const bool& yes, const WeightedCount& w ) {}
    virtual void fillew( const Value& x , const Value& y, const bool& yes, const WeightedCount& w ) {}
    virtual TNamed* new_ROOT( const char* name ) const;
    virtual void save_as_ROOT( const char* name , TDirectory* directory ) const;
    virtual void save( const char* name , TDirectory* directory ) const;
    virtual void load( const char* name , TDirectory* directory );
    virtual void merge( const DgAbsGraphic* other );
    // get bin boundaries
    virtual const Value bin_center_x( const BinIndex& ibin ) const { return( _minx + (((ibin%_nbinsx)+0.5)*_binwidthx)); }
    virtual const Value bin_min_x( const BinSize& ibinx ) const { return( _minx + ((ibinx%_nbinsx)*_binwidthx)); }
    virtual const Value bin_max_x( const BinSize& ibinx ) const { return( _minx + (((ibinx%_nbinsx)+1-std::numeric_limits<double>::min())*_binwidthx)); }

    virtual const Value bin_min_y( const BinSize& ibiny ) const { return( _miny + ((ibiny%_nbinsy)*_binwidthy)); }
    virtual const Value bin_max_y( const BinSize& ibiny ) const { return( _miny + (((ibiny%_nbinsy)+1-std::numeric_limits<double>::min())*_binwidthy)); }
    virtual const BinIndex bin_index( const Value& x , const Value& y ) const { 
      // floor may return _nbinsx if x is within floating-point
      // precision of _maxx. the caller should check for
      // over/underflow before attempting to retrieve the bin index
      // (the following assertions enforce this). therefore clip the return value of floor to _nbinsx-1.
      // similar for y.
      assert( x >= _minx );
      assert( x < _maxx );
      assert( y >= _miny );
      assert( y < _maxy );
      const BinSize ibinx( std::min( static_cast<BinSize>(std::floor( (x-_minx)/_binwidthx )) , static_cast<BinSize>(_nbinsx-1) ) );
      const BinSize ibiny( std::min( static_cast<BinSize>(std::floor( (y-_miny)/_binwidthy )) , static_cast<BinSize>(_nbinsy-1) ) );
      return( std::min( (static_cast<BinIndex>(ibiny)*_nbinsx)+ibinx , static_cast<BinIndex>(_count.size()-1) ) );
    }
    // integrate data over a range
    virtual const Count integrate( const double xmin = -std::numeric_limits<double>::max() , const double xmax = std::numeric_limits<double>::max() ,
                                   const double ymin = -std::numeric_limits<double>::max() , const double ymax = std::numeric_limits<double>::max() ) const;
    // calculate mean of a range
    virtual const Value mean_x( const double xmin = -std::numeric_limits<double>::max() , const double xmax = std::numeric_limits<double>::max() ,
                                const double ymin = -std::numeric_limits<double>::max() , const double ymax = std::numeric_limits<double>::max() ) const;
    virtual const Value mean_y( const double xmin = -std::numeric_limits<double>::max() , const double xmax = std::numeric_limits<double>::max() ,
                                const double ymin = -std::numeric_limits<double>::max() , const double ymax = std::numeric_limits<double>::max() ) const;
    // calculate rms of a range
    virtual const Value rms_x( const double xmin = -std::numeric_limits<double>::max() , const double xmax = std::numeric_limits<double>::max() ,
                               const double ymin = -std::numeric_limits<double>::max() , const double ymax = std::numeric_limits<double>::max() ) const;
    virtual const Value rms_y( const double xmin = -std::numeric_limits<double>::max() , const double xmax = std::numeric_limits<double>::max() ,
                               const double ymin = -std::numeric_limits<double>::max() , const double ymax = std::numeric_limits<double>::max() ) const;
    // return underflow and overflow
    virtual Count underflow() const { return _outofbounds_xminymin+_outofbounds_xmin+_outofbounds_ymin; }
    virtual Count overflow() const { return _outofbounds_xmaxymax+_outofbounds_xmax+_outofbounds_ymax; }
    virtual Count underflow_x() const { return _outofbounds_xminymin+_outofbounds_xmin+_outofbounds_xminymax; }
    virtual Count underflow_y() const { return _outofbounds_xminymin+_outofbounds_ymin+_outofbounds_xmaxymin; }
    virtual Count overflow_x() const { return _outofbounds_xmaxymax+_outofbounds_xmax+_outofbounds_xmaxymin; }
    virtual Count overflow_y() const { return _outofbounds_xmaxymax+_outofbounds_ymax+_outofbounds_xminymax; }
  public:
#ifndef __CINT__
    template<class archiveT>
    void serialize( archiveT & ar , const unsigned int version ) {
      ar & _nbinsx;
      ar & _nbinsy;
      ar & _minx;
      ar & _maxx;
      ar & _miny;
      ar & _maxy;
      ar & _binwidthx;
      ar & _binwidthy;
      ar & _count;
      ar & _outofbounds_xminymin;  
      ar & _outofbounds_xmin;  
      ar & _outofbounds_xminymax;  
      ar & _outofbounds_ymin;
      ar & _outofbounds_ymax;
      ar & _outofbounds_xmaxymin;  
      ar & _outofbounds_xmax;  
      ar & _outofbounds_xmaxymax;  
      ar & _axis_label_x;
      ar & _axis_label_y;
      //ar & _r;
    }
#endif
  }; // end DgHistogram2D

  class TObjectDgHistogram2D : public DgHistogram2D , public TNamed
  {
  public:
    TObjectDgHistogram2D()
      : TNamed()
      , DgHistogram2D(0,0,0,0,0,0) {}
    TObjectDgHistogram2D( const char* name , const char* description , 
                                const DgHistogram2D& h2d )
      : TNamed(name,description)
      , DgHistogram2D(h2d) {}
    ClassDef(TObjectDgHistogram2D,2);
  };
};

#endif // WRAP_DGHISTOGRAM2D_HH
