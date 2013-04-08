#ifndef WRAP_DGTH2D_HH
#define WRAP_DGTH2D_HH

#include <DataGraphics/DgTypes.hh>
#include <DataGraphics/DgAbsGraphic.hh>

#include <vector>
#include <numeric>
#include <limits>
#include <algorithm>
#include <string>
#include <cassert>
#include <TH2D.h>

class TDirectory;

namespace
DataGraphics 
{

  // a general-purpose wrapper around TH2D
  class
  DgTH2D : public DgAbsGraphic
  {
  public:
    static int nTH2D;
  protected:
    TH2D _h;
    std::string _axis_label_x;
    std::string _axis_label_y;
  public:
    DgTH2D() { nTH2D++; }
    DgTH2D( const int& nbinsx , const Value& minx , const Value& maxx, const int& nbinsy , const Value& miny , const Value& maxy ) : _h(TString::Format("TH2D_%d",nTH2D).Data(),TString::Format("TH2D_%d",nTH2D).Data(),nbinsx,minx,maxx,nbinsy,miny,maxy) { 
      _h.SetDirectory(0);
      Sumw2();
      nTH2D++;
    }
    DgTH2D( const int& nbinsx , const std::vector<Value>& binsx , const int& nbinsy , const std::vector<Value>& binsy ) : _h(TString::Format("TH2D_%d",nTH2D).Data(),TString::Format("TH2D_%d",nTH2D).Data(),nbinsx,&binsx[0],nbinsy,&binsy[0]) {
      _h.SetDirectory(0);
      Sumw2();
      nTH2D++; 
    }
    virtual ~DgTH2D() {}
    const TH2D* ptrTH2D() const { return &_h; }
    void Sumw2() { if(_h.GetSumw2N()==0) _h.Sumw2(); }
    void set_x_axis_label( const std::string& xtitle ) { _axis_label_x = xtitle; }
    void set_y_axis_label( const std::string& ytitle ) { _axis_label_y = ytitle; }
    const std::string get_x_axis_label() const { return _axis_label_x; }
    const std::string get_y_axis_label() const { return _axis_label_y; }
    // fill the histogram
    virtual void fill( const Value& x , const Value& y ) { _h.Fill(x,y); }
    virtual void fillw( const Value& x , const Value& y , const WeightedCount& w) { _h.Fill(x,y,static_cast<Value>(w)); }
    // save and load
    virtual TNamed* new_ROOT( const char* name ) const;
    virtual void save_as_ROOT( const char* name , TDirectory* directory ) const;
    virtual void save( const char* name , TDirectory* directory ) const;
    virtual void load( const char* name , TDirectory* directory );
    virtual void merge( const DgAbsGraphic* other );
    // get bin boundaries
    virtual BinSize nbins() const { return _h.GetNbinsX()*_h.GetNbinsY(); }
  public:
    template<class archiveT>
    void serialize( archiveT & ar , const unsigned int version ) {
      assert(0&&"boost serialization not implemented for DgTH2D");
    }
  }; // end of DgTH2D

  // TNamed-derived streamer classes
  class TObjectDgTH2D : public DgTH2D , public TNamed
  {
  public:
    TObjectDgTH2D()
      : TNamed()
      , DgTH2D() {}
    TObjectDgTH2D( const char* name , const char* description , 
		   const DgTH2D& h2d )
      : TNamed(name,description)
      , DgTH2D(h2d) {}
    
    ClassDef(TObjectDgTH2D,1);
  };
  
 
};

#endif // WRAP_DGTH2D_HH
