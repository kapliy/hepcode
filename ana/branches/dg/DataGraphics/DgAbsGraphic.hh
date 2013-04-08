#ifndef WRAP_ABSGRAPHIC_HH
#define WRAP_ABSGRAPHIC_HH

#include <vector>
#include "DataGraphics/DgTypes.hh"

class TNamed;
class TDirectory;

namespace
DataGraphics
{

  // abstract base class for graphics objects
  class
  DgAbsGraphic
  {
  public:
    DgAbsGraphic() { }
    virtual ~DgAbsGraphic() { }
    // create a ROOT plot class (or a canvas, if necessary) to represent this graphic
    virtual TNamed* new_ROOT( const char* name ) const = 0;
    // convert the plot to a ROOT plot class and save to the specified directory.
    virtual void save_as_ROOT( const char* name , TDirectory* directory ) const = 0;
    // save the plot as a ROOT TObject.
    virtual void save( const char* name , TDirectory* directory ) const = 0;
    // load the plot from a ROOT TObject.
    virtual void load( const char* name , TDirectory* directory ) = 0;
    // merge the plot contents with another instance
    virtual void merge( const DgAbsGraphic* other ) = 0;
    // fill abstractions
    virtual void fill( const Value& x ) {}
    virtual void fillw( const Value& x , const WeightedCount& w ) {}
    virtual void fill( const Value& x , const Value& y ) {}
    virtual void fillw( const Value& x , const Value& y , const WeightedCount& w ) {}
    virtual void fill( const Value& x , const Value& y , const Value& z) {}
    virtual void fillw( const Value& x , const Value& y , const Value& z , const WeightedCount& w ) {}
    virtual void fille( const bool& yes ) {}
    virtual void fille( const Value& x , const bool& yes ) {}
    virtual void fille( const Value& x , const Value& y , const bool& yes ) {}
    virtual void fillew( const bool& yes, const WeightedCount& w ) {}
    virtual void fillew( const Value& x , const bool& yes, const WeightedCount& w ) {}
    virtual void fillew( const Value& x , const Value& y, const bool& yes, const WeightedCount& w ) {}
    virtual void fillnt( const std::vector<float>& x ) {}
    virtual void fillntw( const std::vector<float>& x, const std::vector<float>& w ) {}
    
  public:
    template<typename archiveT> void serialize( archiveT& ar , const unsigned int version ) {}
  };
  
};

#endif // WRAP_ABSGRAPHIC_HH
