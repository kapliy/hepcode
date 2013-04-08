#ifndef WRAP_EFFICIENCY0D_HH
#define WRAP_EFFICIENCY0D_HH

#include <DataGraphics/DgTypes.hh>
#include <DataGraphics/DgAbsGraphic.hh>

#include <vector>
#include <numeric>
#include <algorithm>
#include <TNamed.h>

class TDirectory;

namespace
DataGraphics
{
  // 0D efficiency plot
  class
  DgEfficiency0D : public DgAbsGraphic
  {
  protected:
    Count  _pass;
    Count  _total;
  public:
    DgEfficiency0D();
    virtual ~DgEfficiency0D();
    virtual void fill( const Value& x ) {}
    virtual void fillw( const Value& x , const WeightedCount& w ) {}
    virtual void fill( const Value& x , const Value& y ) {}
    virtual void fillw( const Value& x , const Value& y , const WeightedCount& w ) {}
    virtual void fille( const bool& yes );
    virtual void fille( const Value& x , const bool& yes ) {}
    virtual void fillew( const bool& yes, const WeightedCount& w ) {}
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
      ar & _pass;
      ar & _total;
    }
#endif
  };

  class TObjectDgEfficiency0D : public DgEfficiency0D , public TNamed
  {
  public:
    TObjectDgEfficiency0D()
      : TNamed() , DgEfficiency0D() {}
    TObjectDgEfficiency0D( const char* name , const char* description , 
                                 const DgEfficiency0D& e1d )
      : TNamed(name,description)
      , DgEfficiency0D(e1d) {}
    
    ClassDef(TObjectDgEfficiency0D,1);
  };  

}

#endif // WRAP_EFFICIENCY0D_HH
