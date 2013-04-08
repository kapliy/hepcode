#ifndef WRAP_DGEFFICIENCY1D_HH
#define WRAP_DGEFFICIENCY1D_HH

#include <DataGraphics/DgTypes.hh>
#include <DataGraphics/DgHistogram1D.hh>

#include <vector>
#include <numeric>
#include <algorithm>
#include <TNamed.h>
#ifndef __CINT__
#include <boost/serialization/vector.hpp>
#include <boost/serialization/base_object.hpp>
#endif

class TDirectory;

namespace
DataGraphics 
{
  // 1D efficiency plot
  class
  DgEfficiency1D : public DgHistogram1D 
  {
  protected:
    std::vector<Count>  _pass;
  public:
    DgEfficiency1D( const BinSize& nbins , const Value& min , const Value& max );
    virtual ~DgEfficiency1D();
    // fill the efficiency histogram
    virtual void fill( const Value& x ) {}
    virtual void fillw( const Value& x , const WeightedCount& w ) {}
    virtual void fill( const Value& x , const Value& y ) {}
    virtual void fillw( const Value& x , const Value& y , const WeightedCount& w ) {}
    virtual void fille( const bool& yes ) {}
    virtual void fille( const Value& x , const bool& yes );
    virtual void fillew( const bool& yes, const WeightedCount& w ) {}
    virtual void fillew( const Value& x , const bool& yes, const WeightedCount& w ) {}
    virtual void fillew( const Value& x , const Value& y, const bool& yes, const WeightedCount& w ) {}
    // numerator and denominator for a single bin
    virtual double numerator( const unsigned int& ibin ) const;
    virtual double denominator( const unsigned int& ibin ) const;
    // efficiency for a single bin
    virtual void efficiency( const unsigned int& ibin , double& xmin , double& xmax , double& central , double& err_low , double& err_high ) const;
    // efficiency averaged across multiple bins. fbin is analogous to STL iterator end (the final bin+1)
    virtual void efficiency( const unsigned int& ibin , const unsigned int& fbin , double& xmin , double& xmax , double& central , double& err_low , double& err_high ) const;
    // save, load, etc.
    virtual TNamed* new_ROOT( const char* name ) const;
    virtual TNamed* new_ROOT( const char* name , const unsigned int& rebin ) const;
    virtual void save_as_ROOT( const char* name , TDirectory* directory ) const;
    virtual void save( const char* name , TDirectory* directory ) const;
    virtual void load( const char* name , TDirectory* directory );
    virtual void merge( const DgAbsGraphic* other );
  public:
#ifndef __CINT__
    template<class archiveT>
    void serialize( archiveT & ar , const unsigned int version ) {
      ar & boost::serialization::base_object<DgHistogram1D>(*this);
      ar & _pass;
    }
#endif
  };

  class TObjectDgEfficiency1D : public DgEfficiency1D , public TNamed
  {
  public:
    TObjectDgEfficiency1D()
      : TNamed() , DgEfficiency1D( 0 , 0 , 0 ) {}
    TObjectDgEfficiency1D( const char* name , const char* description , 
			   const DgEfficiency1D& e1d )
      : TNamed(name,description)
      , DgEfficiency1D(e1d) {}
    
    ClassDef(TObjectDgEfficiency1D,2);
  };

}

#endif // WRAP_DGEFFICIENCY1D_HH
