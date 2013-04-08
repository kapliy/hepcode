#ifndef WRAP_DGHISTOGRAMWEIGHTED2D_HH
#define WRAP_DGHISTOGRAMWEIGHTED2D_HH

#include <DataGraphics/DgTypes.hh>
#include <DataGraphics/DgAbsGraphic.hh>
#include <DataGraphics/DgHistogram2D.hh>

#include <vector>
#include <numeric>
#include <limits>
#include <algorithm>
#include <TNamed.h>
#include <cmath>
#ifndef __CINT__
#include <boost/serialization/vector.hpp>
#include <boost/serialization/base_object.hpp>
#endif

class TDirectory;
class TNamed;

namespace
DataGraphics 
{
  // compact weighted 2D histogram class with fixed-width binning
  class
  DgHistogramWeighted2D : public DgHistogram2D
  {
  protected:
    std::vector<WeightedCount>  _weighted_count;
    WeightedCount   _weighted_outofbounds_xminymin;
    WeightedCount   _weighted_outofbounds_xmin;
    WeightedCount   _weighted_outofbounds_xminymax;
    WeightedCount   _weighted_outofbounds_ymin;
    WeightedCount   _weighted_outofbounds_ymax;
    WeightedCount   _weighted_outofbounds_xmaxymin;
    WeightedCount   _weighted_outofbounds_xmax;
    WeightedCount   _weighted_outofbounds_xmaxymax;
    WeightedCount   _nentries;
  public:
    DgHistogramWeighted2D() : DgHistogram2D ()
			    , _weighted_count()
			    , _weighted_outofbounds_xminymin(0) , _weighted_outofbounds_xmin(0) 
			    , _weighted_outofbounds_xminymax(0) , _weighted_outofbounds_ymin(0)
			    , _weighted_outofbounds_ymax(0) , _weighted_outofbounds_xmaxymin(0)
			    , _weighted_outofbounds_xmax(0) , _weighted_outofbounds_xmaxymax(0)
			    ,  _nentries(0)
    {}
    DgHistogramWeighted2D( const BinSize& nbinsx , const Value& minx , const Value& maxx ,
			   const BinSize& nbinsy , const Value& miny , const Value& maxy );
    virtual ~DgHistogramWeighted2D();
    // fill the histogram
    virtual void fill( const Value& x ) {}
    virtual void fillw( const Value& x , const WeightedCount& w ) {}
    virtual void fill( const Value& x , const Value& y ) {}
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
    // integrate data over a range
    virtual WeightedCount integrate_weight( const double xmin = -std::numeric_limits<double>::max() , const double xmax = std::numeric_limits<double>::max() ,
					    const double ymin = -std::numeric_limits<double>::max() , const double ymax = std::numeric_limits<double>::max() ) const;
    // calculate mean of a range
    virtual Value mean_x_weight( const double xmin = -std::numeric_limits<double>::max() , const double xmax = std::numeric_limits<double>::max() ,
				 const double ymin = -std::numeric_limits<double>::max() , const double ymax = std::numeric_limits<double>::max() ) const;
    virtual Value mean_y_weight( const double xmin = -std::numeric_limits<double>::max() , const double xmax = std::numeric_limits<double>::max() ,
				 const double ymin = -std::numeric_limits<double>::max() , const double ymax = std::numeric_limits<double>::max() ) const;
    // return underflow and overflow
    virtual WeightedCount underflow_weight() const { return _weighted_outofbounds_xminymin+_weighted_outofbounds_xmin+_weighted_outofbounds_ymin; }
    virtual WeightedCount overflow_weight() const { return _weighted_outofbounds_xmaxymax+_weighted_outofbounds_xmax+_weighted_outofbounds_ymax; }
    virtual WeightedCount underflow_x_weight() const { return _weighted_outofbounds_xminymin+_weighted_outofbounds_xmin+_weighted_outofbounds_xminymax; }
    virtual WeightedCount underflow_y_weight() const { return _weighted_outofbounds_xminymin+_weighted_outofbounds_ymin+_weighted_outofbounds_xmaxymin; }
    virtual WeightedCount overflow_x_weight() const { return _weighted_outofbounds_xmaxymax+_weighted_outofbounds_xmax+_weighted_outofbounds_xmaxymin; }
    virtual WeightedCount overflow_y_weight() const { return _weighted_outofbounds_xmaxymax+_weighted_outofbounds_ymax+_weighted_outofbounds_xminymax; }
    virtual WeightedCount nentries() const { return _nentries; }
  public:
#ifndef __CINT__
    template<class archiveT>
    void serialize( archiveT & ar , const unsigned int version ) {
      ar & boost::serialization::base_object<DgHistogram2D>(*this);
      ar & _weighted_count;
      ar & _weighted_outofbounds_xminymin;  
      ar & _weighted_outofbounds_xmin;  
      ar & _weighted_outofbounds_xminymax;  
      ar & _weighted_outofbounds_ymin;
      ar & _weighted_outofbounds_ymax;
      ar & _weighted_outofbounds_xmaxymin;  
      ar & _weighted_outofbounds_xmax;  
      ar & _weighted_outofbounds_xmaxymax;
      ar & _nentries;  
    }
#endif
  }; // end DgHistogramWeighted2D

  // TNamed-derived streamer classes
  class TObjectDgHistogramWeighted2D : public DgHistogramWeighted2D , public TNamed
  {
  public:
    TObjectDgHistogramWeighted2D()
      : TNamed()
      , DgHistogramWeighted2D() {}
    TObjectDgHistogramWeighted2D( const char* name , const char* description , 
				  const DgHistogramWeighted2D& h2d )
      : TNamed(name,description)
      , DgHistogramWeighted2D(h2d) {}
    ClassDef(TObjectDgHistogramWeighted2D,2);
  };

};

#endif // WRAP_DGHISTOGRAMWEIGHTED2D_HH
