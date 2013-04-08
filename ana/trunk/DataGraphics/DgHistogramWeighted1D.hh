#ifndef WRAP_DGHISTOGRAMWEIGHTED1D_HH
#define WRAP_DGHISTOGRAMWEIGHTED1D_HH

#include <DataGraphics/DgTypes.hh>
#include <DataGraphics/DgAbsGraphic.hh>
#include <DataGraphics/DgHistogram1D.hh>

#include <vector>
#include <numeric>
#include <limits>
#include <algorithm>
#include <TNamed.h>
#ifndef __CINT__
#include <boost/serialization/vector.hpp>
#include <boost/serialization/base_object.hpp>
#endif

class TDirectory;
class TNamed;

namespace
DataGraphics
{

  // compact 1D histogram class with fixed-width or variable-width binning and weighted bin contents
  class
  DgHistogramWeighted1D : public DgHistogram1D
  {
  protected:
    // note that DgHistogram1D::_count is used to store squares of the weights
    std::vector<WeightedCount>  _weighted_count;
    WeightedCount               _weighted_underflow;
    WeightedCount               _weighted_overflow;
    WeightedCount               _nentries;
  public:
    DgHistogramWeighted1D() : DgHistogram1D() , _weighted_underflow(0) , _weighted_overflow(0), _nentries(0) {}
    DgHistogramWeighted1D( const BinSize& nbins , const Value& min , const Value& max );
    DgHistogramWeighted1D( const BinSize& nbins , const std::vector<Value>& bins ); 
    virtual ~DgHistogramWeighted1D();
    // fill the histogram
    virtual void fill( const Value& x ) {}
    virtual void fillw( const Value& x , const WeightedCount& w );
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
    // integrate data over a range
    virtual WeightedCount integrate_weight( const double xmin = -std::numeric_limits<double>::max() , 
                                            const double xmax = std::numeric_limits<double>::max() ) const;
    // calculate mean of a range
    virtual Value mean_weight( const double xmin = -std::numeric_limits<double>::max() , 
                               const double xmax = std::numeric_limits<double>::max() ) const;
    // return underflow and overflow
    virtual WeightedCount underflow_weight() const { return _weighted_underflow; }
    virtual WeightedCount overflow_weight() const { return _weighted_overflow; }
    virtual WeightedCount nentries() const { return _nentries; }
  public:
#ifndef __CINT__
    template<class archiveT>
    void serialize( archiveT & ar , const unsigned int version ) {
      ar & boost::serialization::base_object<DgHistogram1D>(*this);
      ar & _weighted_count;
      ar & _weighted_underflow;  
      ar & _weighted_overflow;
      ar & _nentries;
    }
#endif
  }; // end DgHistogramWeighted1D

  // TNamed-derived streamer classes
  class TObjectDgHistogramWeighted1D : public DgHistogramWeighted1D , public TNamed
  {
  public:
    TObjectDgHistogramWeighted1D()
      : TNamed()
      , DgHistogramWeighted1D() {}
    TObjectDgHistogramWeighted1D( const char* name , const char* description , 
				  const DgHistogramWeighted1D& h1d )
      : TNamed(name,description)
      , DgHistogramWeighted1D(h1d) {}
    ClassDef(TObjectDgHistogramWeighted1D,1);
  };

};

#endif // WRAP_DGHISTOGRAMWEIGHTED1D_HH
