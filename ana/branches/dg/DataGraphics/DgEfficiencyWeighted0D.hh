#ifndef WRAP_EFFICIENCYWEIGHTED0D_HH
#define WRAP_EFFICIENCYWEIGHTED0D_HH

/* 
   Note: this class does not correctly compute the efficiency in weighted events,
   (for that, efficiency should be calculated in bins of weight separately)
   But it's a good approximation, and can also be used to keep track of event counts.
*/

#include <DataGraphics/DgTypes.hh>
#include <DataGraphics/DgEfficiency0D.hh>
#include <DataGraphics/DgAbsGraphic.hh>

#include <vector>
#include <numeric>
#include <algorithm>
#include <TNamed.h>
#ifndef __CINT__
#include <boost/serialization/base_object.hpp>
#endif

class TDirectory;

namespace
DataGraphics
{
  // 0D efficiency plot
  class
  DgEfficiencyWeighted0D : public DgEfficiency0D
  {
  protected:
    WeightedCount  _weighted_pass;
    WeightedCount  _weighted_total;
  public:
    DgEfficiencyWeighted0D();
    virtual ~DgEfficiencyWeighted0D();
    virtual void fill( const Value& x ) {}
    virtual void fillw( const Value& x , const WeightedCount& w ) {}
    virtual void fill( const Value& x , const Value& y ) {}
    virtual void fillw( const Value& x , const Value& y , const WeightedCount& w ) {}
    virtual void fille( const bool& yes ) {}
    virtual void fille( const Value& x , const bool& yes ) {}
    virtual void fillew( const bool& yes, const WeightedCount& w );
    virtual void fillew( const Value& x , const bool& yes, const WeightedCount& w ) {}
    virtual void fillew( const Value& x , const Value& y, const bool& yes, const WeightedCount& w ) {}
    virtual double denominator() const;
    virtual double numerator() const;
    virtual void efficiency( double& central , double& err_low , double& err_high ) const;
    virtual TNamed* new_ROOT( const char* name ) const;
    virtual void save_as_ROOT( const char* name , TDirectory* directory ) const;
    virtual void save( const char* name , TDirectory* directory ) const;
    virtual void load( const char* name , TDirectory* directory );
    virtual void merge( const DgAbsGraphic* other );
  public:
#ifndef __CINT__
    template<class archiveT>
    void serialize( archiveT & ar , const unsigned int version ) {
      ar & boost::serialization::base_object<DgEfficiency0D>(*this);
      ar & _weighted_pass;
      ar & _weighted_total;
    }
#endif
  };

  class TObjectDgEfficiencyWeighted0D : public DgEfficiencyWeighted0D , public TNamed
  {
  public:
    TObjectDgEfficiencyWeighted0D()
      : TNamed() , DgEfficiencyWeighted0D() {}
    TObjectDgEfficiencyWeighted0D( const char* name , const char* description , 
				   const DgEfficiencyWeighted0D& e1d )
      : TNamed(name,description)
      , DgEfficiencyWeighted0D(e1d) {}
    
    ClassDef(TObjectDgEfficiencyWeighted0D,1);
  };

};

#endif // WRAP_EFFICIENCYWEIGHTED0D_HH
