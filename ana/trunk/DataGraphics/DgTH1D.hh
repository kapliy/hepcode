#ifndef WRAP_DGTH1D_HH
#define WRAP_DGTH1D_HH

#include <DataGraphics/DgTypes.hh>
#include <DataGraphics/DgAbsGraphic.hh>

#include <vector>
#include <numeric>
#include <limits>
#include <algorithm>
#include <string>
#include <cassert>
#include <TH1D.h>

class TDirectory;

namespace
DataGraphics 
{

  // a general-purpose wrapper around TH1D
  class
  DgTH1D : public DgAbsGraphic
  {
  public:
    static int nTH1D;
  protected:
    TH1D _h;
    std::string _axis_label;
  public:
    DgTH1D() { nTH1D++; }
    DgTH1D( const int& nbins , const Value& min , const Value& max ) : _h(TString::Format("TH1D_%d",nTH1D).Data(),TString::Format("TH1D_%d",nTH1D).Data(),nbins,min,max) {
      _h.SetDirectory(0);
      Sumw2();
      nTH1D++; 
    }
    DgTH1D( const int& nbins , const std::vector<Value>& bins ) : _h(TString::Format("TH1D_%d",nTH1D).Data(),TString::Format("TH1D_%d",nTH1D).Data(),nbins,&bins[0]) {
      _h.SetDirectory(0);
      Sumw2();
      nTH1D++; 
    }
    virtual ~DgTH1D() {}
    const TH1D* ptrTH1D() const { return &_h; }
    void Sumw2() { if(_h.GetSumw2N()==0) _h.Sumw2(); }
    void set_axis_label( const std::string& axis_label ) { _axis_label = axis_label; }
    const std::string get_axis_label() const { return _axis_label; }
    // fill the histogram
    virtual void fill( const Value& x ) { _h.Fill(x); }
    virtual void fillw( const Value& x , const WeightedCount& w ) { _h.Fill(x,static_cast<Value>(w)); }
    // save and load
    virtual TNamed* new_ROOT( const char* name ) const;
    virtual void save_as_ROOT( const char* name , TDirectory* directory ) const;
    virtual void save( const char* name , TDirectory* directory ) const;
    virtual void load( const char* name , TDirectory* directory );
    virtual void merge( const DgAbsGraphic* other );
    // get bin boundaries
    virtual BinSize nbins() const { return _h.GetNbinsX(); }
    virtual Value bin_center( const BinSize& ibin ) const { 
      return _h.GetBinCenter(ibin);
    }
    virtual Value bin_min( const BinSize& ibin ) const {
      return _h.GetBinLowEdge(ibin);
    }
    virtual Value bin_max( const BinSize& ibin ) const {
      return _h.GetBinLowEdge(ibin) + _h.GetBinWidth(ibin);
    }
    virtual BinSize bin_index( const Value& value ) const {
      return _h.FindFixBin(value);
    }
    // integrate data over a range
    virtual Count integrate( const double xmin = -std::numeric_limits<double>::max() , 
                             const double xmax = std::numeric_limits<double>::max() ) const {
      return _h.Integral(bin_index(xmin),bin_index(xmax));
    }
    // calculate mean of a range
    virtual Value mean( const double xmin = -std::numeric_limits<double>::max() , 
                        const double xmax = std::numeric_limits<double>::max() ) const {
      // range is ignored!
      return _h.GetMean();
    }
    // calculate rms of a range
    virtual Value rms( const double xmin = -std::numeric_limits<double>::max() , 
                       const double xmax = std::numeric_limits<double>::max() ) const {
      // range is ignored!
      return _h.GetRMS();
    }
    // return underflow and overflow
    virtual Count underflow() const { return _h.GetBinContent(0); }
    virtual Count overflow() const { return _h.GetBinContent( nbins() + 1 ); }
    // minimum and maximum count
    Count bin_count( const BinSize& ibin ) const {
      assert( ibin <= nbins() );
      return _h.GetBinContent(ibin);
    }
    Count maximum_count() const { 
      return _h.GetBinContent(_h.GetMaximumBin());
    }
    Count minimum_count() const { 
      return _h.GetBinContent(_h.GetMinimumBin());
    }
  public:
    template<class archiveT>
    void serialize( archiveT & ar , const unsigned int version ) {
      assert(0&&"boost serialization not implemented for DgTH1D");
    }
  }; // end of DgTH1D

  // TNamed-derived streamer classes
  class TObjectDgTH1D : public DgTH1D , public TNamed
  {
  public:
    TObjectDgTH1D()
      : TNamed()
      , DgTH1D() {}
    TObjectDgTH1D( const char* name , const char* description , 
		   const DgTH1D& h1f )
      : TNamed(name,description)
      , DgTH1D(h1f) {}
    
    ClassDef(TObjectDgTH1D,1);
  };
  
};

#endif // WRAP_DGTH1D_HH
