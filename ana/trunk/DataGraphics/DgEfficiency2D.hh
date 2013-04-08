#ifndef WRAP_DGEFFICIENCY2D_HH
#define WRAP_DGEFFICIENCY2D_HH

#include <DataGraphics/DgTypes.hh>
#include <DataGraphics/DgHistogram2D.hh>

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
  // 2D efficiency plot
  class
  DgEfficiency2D : public DgHistogram2D 
  {
  protected:
    std::vector<Count>  _pass;
  public:
    DgEfficiency2D( const BinSize& nbinsx , const Value& minx , const Value& maxx ,
                    const BinSize& nbinsy , const Value& miny , const Value& maxy );
    virtual ~DgEfficiency2D();
    // fill the efficiency histogram
    virtual void fill( const Value& x ) {}
    virtual void fillw( const Value& x , const WeightedCount& w ) {}
    virtual void fill( const Value& x , const Value& y ) {}
    virtual void fillw( const Value& x , const Value& y , const WeightedCount& w ) {}
    virtual void fille( const bool& yes ) {}
    virtual void fille( const Value& x , const bool& yes ) {}
    virtual void fille( const Value& x , const Value& y , const bool& yes );
    virtual void fillew( const bool& yes, const WeightedCount& w ) {}
    virtual void fillew( const Value& x , const bool& yes, const WeightedCount& w ) {}
    virtual void fillew( const Value& x , const Value& y, const bool& yes, const WeightedCount& w ) {}
    // numerator and denominator for a single bin
    virtual double numerator( const unsigned int& ibinx , const unsigned int& ibiny ) const;
    virtual double denominator( const unsigned int& ibinx , const unsigned int& ibiny ) const;
    // efficiency for a single bin
    virtual void efficiency( const unsigned int& ibinx , const unsigned int& ibiny ,
			     double& xmin , double& xmax , 
			     double& ymin , double& ymax , 
			     double& central , double& err_low , double& err_high ) const;
    // efficiency averaged across multiple bins. 
    // fbin is analogous to STL iterator end (the final bin+1)
    virtual void efficiency( const unsigned int& ibinx , const unsigned int& fbinx , 
			     const unsigned int& ibiny , const unsigned int& fbiny , 
			     double& xmin , double& xmax , 
			     double& ymin , double& ymax , 
			     double& central , double& err_low , double& err_high ) const;
    // save, load, etc.
    virtual TNamed* new_ROOT( const char* name ) const;
    virtual TNamed* new_ROOT( const char* name , 
                      const unsigned int& rebinx , const unsigned int& rebiny ) const;
    virtual void save_as_ROOT( const char* name , TDirectory* directory ) const;
    virtual void save( const char* name , TDirectory* directory ) const;
    virtual void load( const char* name , TDirectory* directory );
    virtual void merge( const DgAbsGraphic* other );
  public:
#ifndef __CINT__
    template<class archiveT>
    void serialize( archiveT & ar , const unsigned int version ) {
      ar & boost::serialization::base_object<DgHistogram2D>(*this);
      ar & _pass;
    }
#endif
  };

  class TObjectDgEfficiency2D : public DgEfficiency2D , public TNamed
  {
  public:
    TObjectDgEfficiency2D()
      : TNamed() , DgEfficiency2D( 0 , 0 , 0 , 0 , 0 , 0 ) {}
    TObjectDgEfficiency2D( const char* name , const char* description , 
                           const DgEfficiency2D& e2d )
      : TNamed(name,description)
      , DgEfficiency2D(e2d) {}

    ClassDef(TObjectDgEfficiency2D,1);
  };

}

#endif // WRAP_DGEFFICIENCY2D_HH
