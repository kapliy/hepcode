#ifndef WRAP_DGNTUPLE_HH
#define WRAP_DGNTUPLE_HH

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
  // Multi-dimensional ntuple of floats
  class
  DgNtuple : public DgAbsGraphic
  {
  public:
    class Tuple {
    public:
      // For now, everything is contained in a single array of floats.
      // If event weight is given, it is the first value in the array.
      // In the future, event weight could be separated out into a variable.
      std::vector<float> _vals;
      Tuple() {}
      Tuple(const std::vector<float>& vals):_vals(vals) {}
    public:
#ifndef __CINT__
    template<class archiveT>
    void serialize( archiveT & ar , const unsigned int version ) {
      ar & _vals;
    }
#endif
    };
    typedef std::vector< DataGraphics::DgNtuple::Tuple > Coll;
    typedef Coll::const_iterator const_iterator;
  protected:
    Coll  _data;
    std::vector<std::string> _names;
  public:
    DgNtuple() {}
    virtual ~DgNtuple() {}
    void set_names (const std::vector<std::string>& names) { _names = names; }
    void add_names (const std::string& name) { _names.push_back(name); }
    void add_names (const std::vector<std::string>& names) { _names.insert(_names.end(),names.begin(),names.end()); }
    const int get_rowsize() const { return _names.size(); }
    const std::vector<std::string> get_names() const { return _names; }
    const std::string get_name(int i) const { return _names.at(i); }
    const std::string get_varlist() const { 
      if (_names.empty()) return "";
      std::string v = _names.at(0);
      for(unsigned int i=1; i<_names.size(); i++) {
	v += ":" + _names.at(i);
      }
      return v;
    };
    virtual void fill( const Value& x ) {}
    virtual void fillw( const Value& x , const WeightedCount& w ) {}
    virtual void fill( const Value& x , const Value& y ) {}
    virtual void fillw( const Value& x , const Value& y , const WeightedCount& w ) {}
    virtual void fille( const bool& yes ) {}
    virtual void fille( const Value& x , const bool& yes ) {}
    virtual void fillew( const bool& yes, const WeightedCount& w ) {}
    virtual void fillew( const Value& x , const bool& yes, const WeightedCount& w ) {}
    virtual void fillew( const Value& x , const Value& y, const bool& yes, const WeightedCount& w ) {}
    virtual void fillnt( const std::vector<float>& x );
    virtual void fillntw( const std::vector<float>& x, const std::vector<float>& w );
    TNamed* new_ROOT( const char* name ) const;
    virtual void save_as_ROOT( const char* name , TDirectory* directory ) const;
    virtual void save( const char* name , TDirectory* directory ) const;
    virtual void load( const char* name , TDirectory* directory );
    virtual void merge( const DgAbsGraphic* other );
    DgNtuple::const_iterator begin() const { return _data.begin(); }
    DgNtuple::const_iterator end() const { return _data.end(); }
    const unsigned int number() const { return _data.size(); }
  public:
#ifndef __CINT__
    template<class archiveT>
    void serialize( archiveT & ar , const unsigned int version ) {
      ar & _data;
      ar & _names;
    }
#endif
  };

  class TObjectDgNtuple : public DgNtuple , public TNamed
  {
  public:
    TObjectDgNtuple() 
      : TNamed() , DgNtuple() {}
    TObjectDgNtuple( const char* name , const char* description , 
		     const DgNtuple& gnt ) 
      : TNamed(name,description) , DgNtuple(gnt) {}
    ClassDef(TObjectDgNtuple,2);
  };
}
#endif // WRAP_DGNTUPLE_HH
