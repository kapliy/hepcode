#ifndef WRAP_DGGRAPH2D_HH
#define WRAP_DGGRAPH2D_HH

#include <DataGraphics/DgTypes.hh>
#include <DataGraphics/DgAbsGraphic.hh>

#include <vector>
#include <numeric>
#include <limits>
#include <algorithm>
#include <string>
#include <TNamed.h>
#ifndef __CINT__
#include <boost/serialization/vector.hpp>
#endif


class TDirectory;

namespace
DataGraphics 
{
  // 2D graph class
  class
  DgGraph2D : public DgAbsGraphic
  {
  public:
    class Tuple {
    public:
      Value _x;
      Value _y;
      WeightedCount _w;
      Tuple() : _x(-std::numeric_limits<Value>::max()),_y(-std::numeric_limits<Value>::max()),_w(1.) {}
      Tuple(const Value& x,const Value& y):_x(x),_y(y),_w(1){}
      Tuple(const Value& x,const Value& y,const WeightedCount& w):_x(x),_y(y),_w(w){}
    public:
#ifndef __CINT__
    template<class archiveT>
    void serialize( archiveT & ar , const unsigned int version ) {
      ar & _x;
      ar & _y;
      ar & _w;
    }
#endif
    };
    typedef std::vector< DataGraphics::DgGraph2D::Tuple > Coll;
    typedef Coll::const_iterator const_iterator;
  protected:
    Coll  _data;
    std::string _x_axis_label;
    std::string _y_axis_label;
  public:
    DgGraph2D() {}
    virtual ~DgGraph2D() {}
    void set_x_axis_label( const std::string& xtitle ) { _x_axis_label = xtitle; }
    void set_y_axis_label( const std::string& ytitle ) { _y_axis_label = ytitle; }
    const std::string get_x_axis_label() const { return _x_axis_label; }
    const std::string get_y_axis_label() const { return _y_axis_label; }
    const std::string get_axis_label() const { return _x_axis_label; }
    virtual void fill( const Value& x ) {}
    virtual void fillw( const Value& x , const WeightedCount& w ) {}
    virtual void fill( const Value& x , const Value& y );
    virtual void fillw( const Value& x , const Value& y , const WeightedCount& w );
    virtual void fille( const bool& yes ) {}
    virtual void fille( const Value& x , const bool& yes ) {}
    virtual void fillew( const bool& yes, const WeightedCount& w ) {}
    virtual void fillew( const Value& x , const bool& yes, const WeightedCount& w ) {}
    virtual void fillew( const Value& x , const Value& y, const bool& yes, const WeightedCount& w ) {}
    TNamed* new_ROOT( const char* name ) const;
    virtual void save_as_ROOT( const char* name , TDirectory* directory ) const;
    virtual void save( const char* name , TDirectory* directory ) const;
    virtual void load( const char* name , TDirectory* directory );
    virtual void merge( const DgAbsGraphic* other );
    DgGraph2D::const_iterator begin() const { return _data.begin(); }
    DgGraph2D::const_iterator end() const { return _data.end(); }
    const unsigned int number() const { return _data.size(); }
  public:
#ifndef __CINT__
    template<class archiveT>
    void serialize( archiveT & ar , const unsigned int version ) {
      ar & _x_axis_label;
      ar & _y_axis_label;
      ar & _data;
    }
#endif
  };

  class TObjectDgGraph2D : public DgGraph2D , public TNamed
  {
  public:
    TObjectDgGraph2D() 
      : TNamed() , DgGraph2D() {}
    TObjectDgGraph2D( const char* name , const char* description , 
                            const DgGraph2D& g2d ) 
      : TNamed(name,description) , DgGraph2D(g2d) {}
    ClassDef(TObjectDgGraph2D,3);
  };
}
#endif // WRAP_DGGRAPH2D_HH
