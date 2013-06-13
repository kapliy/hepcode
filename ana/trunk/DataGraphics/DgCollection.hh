#ifndef WRAP_DGCOLLECTION_HH
#define WRAP_DGCOLLECTION_HH

#include "TrigFTKAna_config.h"
#include <map>
#include <vector>
#include <stack>
#include <algorithm>
#include <limits>
#include <string>
#include <cstring>
#include <DataGraphics/DgEventInfo.hh>
#include <DataGraphics/DgBin.hh>
#include <DataGraphics/DgTools.hh>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/scope_exit.hpp>
#include <boost/config.hpp>

class TDirectory;
class TFile;
class BootstrapGenerator;

//#ifdef __GNUG__
//#define inline  __attribute__((always_inline))
//#endif

namespace
DataGraphics
{
  // group of graphics objects. Everything is static
  class
  DgCollection
  {
  protected:
    // root() returns a pointer to this
    static DgCollection              _base;
    // file
    static TFile* _file;
    static TDirectory* _current;
    // global event weight
    static WeightedCount _global_weight;
    // additional event-wide info, including other weights
    static DgEventInfo _event_info;
    // event-wide binning templates
    static DgBin _binning;
    // bootstrap generator
    static int _replicas;
    static BootstrapGenerator *_boot;
    // in-memory metadata
    static std::map< std::string , std::vector<std::string> > _mapH;   // histograms
    static std::map< std::string , std::map<std::string,TObject*> > _mapB;   // bootstrap histograms, memory-resident
    static std::map< std::string , std::vector<std::string> > _mapT;  // trees
  public:
    DgCollection() { return;  }
    ~DgCollection() { return; }
  public:
    static void set_save_filename( const std::string& _filename , bool force_unique = false);
    static void initialize_bootstrap(const std::string& bname, const int nreplicas);
    static void generate_bootstrap(unsigned int RunNumber, unsigned int EventNumber);
    static void dump_names();
    static void finalize();
    static void save(bool verbose=false);
    // construction / destruction
    static DgCollection* root() { return &_base; }
    // move to this subgroup (creating it if necessary)
    /* inline */ static void down( const std::string& sname , const std::string& description );
    // move up to the parent group
    /* inline */ static void up();
  public:
    // if global_weight is set, all "fill" calls are redirected to corresponding "fillw"
    static const WeightedCount global_weight() { return _global_weight; }
    static const WeightedCount get_global_weight() { return global_weight(); }
    static DgEventInfo& event_info() { return _event_info; }
    static DgBin& bin() { return _binning; }
    static void set_global_weight( const WeightedCount& v ) { _global_weight =  v ; }
    // fill 1d histogram with fixed binning
    static void fillh( const std::string& sname , const BinSize& nbins , const Value& min , const Value& max ,
                       const Value& x ) {
      static std::string dummy = "";
      fillh(sname,nbins,min,max,x,dummy);
    }
    static void fillh( const std::string& sname , const BinSize& nbins , const Value& min , const Value& max ,
                       const Value& x , const std::string& axis_label);
    // fill 1d histogram with variable binning
    static void fillvh( const std::string& sname , const BinSize& nbins , const std::vector<Value>& bins ,
                        const Value& x ,  const std::string& axis_label);
    // fill weighted 1d histogram with fixed binning
    static void fillhw( const std::string& sname , const BinSize& nbins , const Value& min , const Value& max ,
			const Value& x , const WeightedCount& weight ) {
      static std::string dummy = "";
      fillhw(sname,nbins,min,max,x,weight,dummy);
    }
    static void fillhw( const std::string& sname , const BinSize& nbins , const Value& min , const Value& max ,
			const Value& x , const WeightedCount& weight , const std::string& axis_label);
    // fill weighted 1d histogram with variable binning
    static void fillvhw( const std::string& sname , const BinSize& nbins , const std::vector<Value>& bins ,
                         const Value& x , const WeightedCount& weight , const std::string& axis_label);
    // fill 2d histogram
    static void fillh( const std::string& sname , 
                       const BinSize& nbinsx , const Value& minx , const Value& maxx , 
                       const BinSize& nbinsy , const Value& miny , const Value& maxy , 
                       const Value& x , const Value& y , 
		       const std::string& axis_label_x , const std::string& axis_label_y );
    static void fillvh( const std::string& sname , 
			const BinSize& nbinsx , const std::vector<Value>& binsx ,
			const BinSize& nbinsy , const std::vector<Value>& binsy ,
			const Value& x , const Value& y ,
			const std::string& axis_label_x , const std::string& axis_label_y );
    // fill weighted 2d histogram
    static void fillhw( const std::string& sname , 
                        const BinSize& nbinsx , const Value& minx , const Value& maxx , 
                        const BinSize& nbinsy , const Value& miny , const Value& maxy , 
                        const Value& x , const Value& y , const WeightedCount& weight ,
                        const std::string& axis_label_x , const std::string& axis_label_y );
    static void fillvhw( const std::string& sname , 
			 const BinSize& nbinsx , const std::vector<Value>& binsx ,
			 const BinSize& nbinsy , const std::vector<Value>& binsy ,
			 const Value& x , const Value& y , const WeightedCount& weight ,
			 const std::string& axis_label_x , const std::string& axis_label_y );
    // 3d histograms (only implemented as DgTH*D classes)
    static void fillh( const std::string& sname , 
			const BinSize& nbinsx , const Value& minx , const Value& maxx , 
			const BinSize& nbinsy , const Value& miny , const Value& maxy ,
			const BinSize& nbinsz , const Value& minz , const Value& maxz , 
			const Value& x , const Value& y , const Value& z ,
		       const std::string& axis_label_x , const std::string& axis_label_y , const std::string& axis_label_z );
    static void fillvh( const std::string& sname , 
			const BinSize& nbinsx , const std::vector<Value>& binsx ,
			const BinSize& nbinsy , const std::vector<Value>& binsy ,
			const BinSize& nbinsz , const std::vector<Value>& binsz ,
			const Value& x , const Value& y , const Value& z ,
			const std::string& axis_label_x , const std::string& axis_label_y , const std::string& axis_label_z );
    static void fillhw( const std::string& sname , 
			const BinSize& nbinsx , const Value& minx , const Value& maxx , 
			const BinSize& nbinsy , const Value& miny , const Value& maxy ,
			const BinSize& nbinsz , const Value& minz , const Value& maxz , 
			const Value& x , const Value& y , const Value& z , const WeightedCount& weight ,
			const std::string& axis_label_x , const std::string& axis_label_y , const std::string& axis_label_z );
    static void fillvhw( const std::string& sname , 
			 const BinSize& nbinsx , const std::vector<Value>& binsx ,
			 const BinSize& nbinsy , const std::vector<Value>& binsy ,
			 const BinSize& nbinsz , const std::vector<Value>& binsz ,
			 const Value& x , const Value& y , const Value& z , const WeightedCount& weight ,
			 const std::string& axis_label_x , const std::string& axis_label_y , const std::string& axis_label_z );
    // fill 1d profile histogram
    static void fillf( const std::string& sname , const BinSize& nbins , const Value& min , const Value& max , 
                       const Value& x , const Value& y ,
                       const std::string& axis_label_x , const std::string& axis_label_y );
    static void fillfw( const std::string& sname , const BinSize& nbins , const Value& min , const Value& max ,
                        const Value& x , const Value& y , const WeightedCount& weight ,
			const std::string& axis_label_x , const std::string& axis_label_y );
    // fill graph
    static void fillgraph( const std::string& sname , const Value& x, const Value& y , const std::string& axis_label_x , const std::string& axis_label_y );
    // fill ntuple of floats
    static void fillnt( const std::string& sname , const std::vector<float>& x, const std::vector<std::string>& names );
    static void fillntw( const std::string& sname , const std::vector<float>& x, const std::vector<std::string>& names, 
			 const DgEventInfo& w );
    // fill 0d efficiency plot
    static void filleff( const std::string& sname , const bool& pass );
    // fill weighted 0d efficiency plot (use for event counts only)
    static void filleffw( const std::string& sname , const bool& pass , const WeightedCount& weight);
    // fill 1d efficiency plot
    static void filleff( const std::string& sname , const BinSize& nbins , const Value& min , const Value& max , 
                         const Value& x , const bool& pass );
    static void filleff( const std::string& sname , const BinSize& nbins , const Value& min , const Value& max ,
                         const Value& x , const bool& pass , 
                         const std::string& axis_label );
    // fill 2d efficiency plot
    static void filleff( const std::string& sname , 
                         const BinSize& nbinsx , const Value& minx , const Value& maxx , 
                         const BinSize& nbinsy , const Value& miny , const Value& maxy , 
                         const Value& x , const Value& y , const bool& pass , 
                         const std::string& axis_label_x , const std::string& axis_label_y );
    
    /*
      -----------------------------------------
      BOOTSTRAP HISTOGRAMS
      -----------------------------------------
    */
    // fill 1d histogram with fixed binning
    static void bfillh( const std::string& sname , const BinSize& nbins , const Value& min , const Value& max ,
                       const Value& x ) {
      static std::string dummy = "";
      bfillh(sname,nbins,min,max,x,dummy);
    }
    static void bfillh( const std::string& sname , const BinSize& nbins , const Value& min , const Value& max ,
                       const Value& x , const std::string& axis_label);
    // fill 1d histogram with variable binning
    static void bfillvh( const std::string& sname , const BinSize& nbins , const std::vector<Value>& bins ,
                        const Value& x ,  const std::string& axis_label);
    // fill weighted 1d histogram with fixed binning
    static void bfillhw( const std::string& sname , const BinSize& nbins , const Value& min , const Value& max ,
			const Value& x , const WeightedCount& weight ) {
      static std::string dummy = "";
      bfillhw(sname,nbins,min,max,x,weight,dummy);
    }
    static void bfillhw( const std::string& sname , const BinSize& nbins , const Value& min , const Value& max ,
			const Value& x , const WeightedCount& weight , const std::string& axis_label);
    // fill weighted 1d histogram with variable binning
    static void bfillvhw( const std::string& sname , const BinSize& nbins , const std::vector<Value>& bins ,
                         const Value& x , const WeightedCount& weight , const std::string& axis_label);
    // fill 2d histogram
    static void bfillh( const std::string& sname , 
                       const BinSize& nbinsx , const Value& minx , const Value& maxx , 
                       const BinSize& nbinsy , const Value& miny , const Value& maxy , 
                       const Value& x , const Value& y , 
		       const std::string& axis_label_x , const std::string& axis_label_y );
    static void bfillvh( const std::string& sname , 
			const BinSize& nbinsx , const std::vector<Value>& binsx ,
			const BinSize& nbinsy , const std::vector<Value>& binsy ,
			const Value& x , const Value& y ,
			const std::string& axis_label_x , const std::string& axis_label_y );
    // fill weighted 2d histogram
    static void bfillhw( const std::string& sname , 
                        const BinSize& nbinsx , const Value& minx , const Value& maxx , 
                        const BinSize& nbinsy , const Value& miny , const Value& maxy , 
                        const Value& x , const Value& y , const WeightedCount& weight ,
                        const std::string& axis_label_x , const std::string& axis_label_y );
    static void bfillvhw( const std::string& sname , 
			 const BinSize& nbinsx , const std::vector<Value>& binsx ,
			 const BinSize& nbinsy , const std::vector<Value>& binsy ,
			 const Value& x , const Value& y , const WeightedCount& weight ,
			 const std::string& axis_label_x , const std::string& axis_label_y );
    

  }; // end DgCollection

  typedef DgCollection dg;

};

//#undef inline

#endif // WRAP_DGCOLLECTION_HH
