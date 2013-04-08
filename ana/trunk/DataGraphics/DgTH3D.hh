#ifndef WRAP_DGTH3D_HH
#define WRAP_DGTH3D_HH

#include <DataGraphics/DgTypes.hh>
#include <DataGraphics/DgAbsGraphic.hh>

#include <vector>
#include <numeric>
#include <limits>
#include <algorithm>
#include <string>
#include <cassert>
#include <TH3D.h>

class TDirectory;

namespace
DataGraphics 
{

  // a general-purpose wrapper around TH3D
  class
  DgTH3D : public DgAbsGraphic
  {
  public:
    static int nTH3D;
  protected:
    TH3D _h;
    std::string _axis_label_x;
    std::string _axis_label_y;
    std::string _axis_label_z;
  public:
    DgTH3D() { nTH3D++; }
    DgTH3D( const int& nbinsx , const Value& minx , const Value& maxx,
	    const int& nbinsy , const Value& miny , const Value& maxy,
	    const int& nbinsz , const Value& minz , const Value& maxz) : _h(TString::Format("TH3D_%d",nTH3D).Data(),TString::Format("TH3D_%d",nTH3D).Data(),nbinsx,minx,maxx,nbinsy,miny,maxy,nbinsz,minz,maxz) { 
      _h.SetDirectory(0);
      Sumw2();
      nTH3D++;
    }
    DgTH3D( const int& nbinsx , const std::vector<Value>& binsx,
	    const int& nbinsy , const std::vector<Value>& binsy,
	    const int& nbinsz , const std::vector<Value>& binsz) : _h(TString::Format("TH3D_%d",nTH3D).Data(),TString::Format("TH3D_%d",nTH3D).Data(),nbinsx,&binsx[0],nbinsy,&binsy[0],nbinsz,&binsz[0]) {
      _h.SetDirectory(0);
      Sumw2();
      nTH3D++; 
    }
    virtual ~DgTH3D() {}
    const TH3D* ptrTH3D() const { return &_h; }
    void Sumw2() { if(_h.GetSumw2N()==0) _h.Sumw2(); }
    void set_x_axis_label( const std::string& xtitle ) { _axis_label_x = xtitle; }
    void set_y_axis_label( const std::string& ytitle ) { _axis_label_y = ytitle; }
    void set_z_axis_label( const std::string& ztitle ) { _axis_label_z = ztitle; }
    const std::string get_x_axis_label() const { return _axis_label_x; }
    const std::string get_y_axis_label() const { return _axis_label_y; }
    const std::string get_z_axis_label() const { return _axis_label_z; }
    // fill the histogram
    virtual void fill( const Value& x , const Value& y , const Value& z) { _h.Fill(x,y,z); }
    virtual void fillw( const Value& x , const Value& y , const Value& z , const WeightedCount& w) { _h.Fill(x,y,z,static_cast<Value>(w)); }
    // save and load
    virtual TNamed* new_ROOT( const char* name ) const;
    virtual void save_as_ROOT( const char* name , TDirectory* directory ) const;
    virtual void save( const char* name , TDirectory* directory ) const;
    virtual void load( const char* name , TDirectory* directory );
    virtual void merge( const DgAbsGraphic* other );
    // get bin boundaries
    virtual BinSize nbins() const { return _h.GetNbinsX()*_h.GetNbinsY()*_h.GetNbinsZ(); }
  public:
    template<class archiveT>
    void serialize( archiveT & ar , const unsigned int version ) {
      assert(0&&"boost serialization not implemented for DgTH3D");
    }
  }; // end of DgTH3D

  // TNamed-derived streamer classes
  class TObjectDgTH3D : public DgTH3D , public TNamed
  {
  public:
    TObjectDgTH3D()
      : TNamed()
      , DgTH3D() {}
    TObjectDgTH3D( const char* name , const char* description , 
		   const DgTH3D& h3d )
      : TNamed(name,description)
      , DgTH3D(h3d) {}
    
    ClassDef(TObjectDgTH3D,1);
  };
  
 
};

#endif // WRAP_DGTH3D_HH
