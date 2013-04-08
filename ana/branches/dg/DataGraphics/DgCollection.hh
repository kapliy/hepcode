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
#include <DataGraphics/DgAbsGraphic.hh>
#include <DataGraphics/DgResourceMgr.hh>
#include <DataGraphics/DgEventInfo.hh>
#include <DataGraphics/DgBin.hh>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/optional.hpp>
#include <boost/scope_exit.hpp>
#include <boost/config.hpp>
#ifndef __CINT__
#include <boost/serialization/map.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/split_member.hpp>
#endif

class TDirectory;

#ifdef __GNUG__
#define inline  __attribute__((always_inline))
#endif

namespace
DataGraphics
{
  // group of graphics objects
  class
  DgCollection
  {
  public:
    class ViewValue {
    protected:
      const DgCollection* _collection;
      std::string _name;
      DgResourceMgr::ID _id;
      //boost::shared_ptr<const DgAbsGraphic> _graphic;
    public:
      ViewValue(const DgCollection* a,const std::string& b,const DgResourceMgr::ID c) 
        : _collection(a) , _name(b) , _id(c) {}
      const std::string& name() const { return _name; }
      const DgCollection* collection() const { return _collection; }
      const boost::shared_ptr<const DgAbsGraphic> graphic() const { assert(_id!=0); return DgResourceMgr::get_graphic( _id ); }
      const bool valid_graphic() const { return( _id!=0 ); }
    };
    typedef std::vector<ViewValue> View;
  protected:
    typedef std::map< std::string , DgResourceMgr::ID > ResourceIDMap;
  public:
    // replace these with unordered_map hashes
    typedef std::map<std::string,DgAbsGraphic*> Coll;
    typedef std::map<std::string,DgCollection*> SubColl;
    // typedef std::map<std::string,DgAxis*> AxisColl;
    static const std::string& graphic_name( const Coll::const_iterator& i ) { return i->first; }
    static const std::string& graphic_name( const ResourceIDMap::const_iterator& i ) { return i->first; }
    static const DgAbsGraphic* graphic_ptr( const Coll::const_iterator& i ) { return i->second; }
    static boost::shared_ptr<const DgAbsGraphic> graphic_resource( const Coll::const_iterator& i ) { 
      if( current()->_read_on_demand ) {
        ResourceIDMap::const_iterator k = current()->_resource_id_map.find( i->first );
        assert( k != current()->_resource_id_map.end() );
        return( DgResourceMgr::get_graphic( k->second ) );
      }
      return boost::shared_ptr<const DgAbsGraphic>();
    }
    static boost::shared_ptr<const DgAbsGraphic> graphic_resource( const ResourceIDMap::const_iterator& k ) { 
      if( k!=current()->_resource_id_map.end() ) {
        return( DgResourceMgr::get_graphic( k->second ) );
      }
      return boost::shared_ptr<const DgAbsGraphic>();
    }
    static const std::string& subcollection_name( const SubColl::const_iterator& i ) { return i->first; }
    static const DgCollection* subcollection_ptr( const SubColl::const_iterator& i ) { return i->second; }
    // static const std::string& axis_name( const AxisColl::const_iterator& i ) { return i->first; }
    // static const DgAxis* axis_ptr( const AxisColl::const_iterator& i ) { return i->second; }
  protected:
    std::string                      _name;        // collection name
    std::string                      _description; // collection description
    Coll                             _graphics;    // mapping of name -> graphic object pointer
    DgCollection*                    _parent;      // this collection's parent collection
    SubColl                          _children;    // subgroups
    // AxisColl                         _axes;        // shared plot axes
    static DgCollection              _base; // root() returns a pointer to this
    // tree navigation
    static DgCollection* const         _root;
    static DgCollection*               _current;
    // global event weight
    static boost::optional<WeightedCount> _global_weight;
    // additional event-wide info, including other weights
    static DgEventInfo _event_info;
    // event-wide binning templates
    static DgBin _binning;
    // automatic end-of-job save. this stuff must be local to ensure
    // it hasn't been destroyed before the destructor gets called. it
    // also allows the user to set up saves of subsets to separate
    // files. since _root is static, turning autosave on for it will
    // save partial job output even after some types of crashes.
    boost::shared_ptr<std::string> _save_filename;
    bool                        _save_at_exit;
    bool                        _save_as_ROOT;
    bool                        _save_as_dg;
    // on-demand loading
    mutable bool                _locked;
    mutable bool                _read_on_demand;
    mutable ResourceIDMap       _resource_id_map;
    // browser-style navigation
    static std::stack<DgCollection*>   _history;
  public:
    static bool save_unique;
  protected:
    friend class boost::serialization::access;
    DgCollection() {}; // for boost::serialization load
    DgCollection( DgCollection* parent , const std::string& sname , const std::string& description );
    template<class tobjT,class graphT> void load_obj( const tobjT* tobj , const bool merge );
  protected:
    // load on demand manipulations
    void lock( const bool yes = true ) { _locked = yes; }
    const bool is_locked() const { return _locked; }
  public:
    void read_on_demand( const bool yes = true ) { 
      assert( !_read_on_demand || yes ); // once read_on_demand is set, you should not unset it!
      _read_on_demand = yes; 
    }
    const bool is_read_on_demand() const { return _read_on_demand; }
  public:
    virtual ~DgCollection();
  public:
    // new save interface
    void do_save_at_exit();
    void set_save_filename( const std::string& filename ) { 
      _save_filename.reset(new std::string(filename)); 
      _save_at_exit=true;
      _save_as_ROOT=true;
      _save_as_dg=true;
    }
    void save_at_exit() { _save_at_exit = true; }
    void no_save_at_exit() { _save_at_exit = false; }
    void root_save() { _save_as_ROOT = true; }
    void no_root_save() { _save_as_ROOT = false; }
    void dg_save() { _save_as_dg = true; }
    void no_dg_save() { _save_as_dg = false; }
    const bool stat_root_save() const { return _save_as_ROOT; }
    const bool stat_dg_save() const { return _save_as_dg; }
    // old save interface---manual save
    void save_as_ROOT( TDirectory* directory ) const;
    void save( TDirectory* directory ) const;
    // load interface
    void load( TDirectory* directory , const bool merge = false , const std::string parent_path = "" , const bool skip_path_substitutions = false );
    // construction / destruction
    static DgCollection* root() { return _root; }
    void delete_me() { if(this!=root()) { delete this; } }
    // navigation
    // ================================================================
    static DgCollection* current() { if( !_current ) { _current = root(); }; return _current; }
    // this collection name and description
    const std::string& name() const { return _name; }
    const std::string& description() const { return _description; }
    // path string to current collection
    static const std::string path_string() {
      std::string full_path = "";
      const DgCollection* coll = current();
      do {
        full_path = coll->name() + "/" + full_path;
      } while( coll!=root() && (coll=coll->parent()) );
      return full_path;
    }
    // change current group to this group
    void cd() { _current = this; }
    // navigate forward and backward, browser style
    void forward() {
      if( _current ) { _history.push( _current ); }
      _current = this;
    }
    static void back() {
      if( _history.empty() ) { _current = root(); return; }
      _current = _history.top();
      _history.pop();
    }
    // change current collection to specified "path" (absolute or relative)
    static DgCollection* cd(const std::string& coll); 
    // move to this subgroup (creating it if necessary)
    inline static void down( const std::string& sname , const std::string& description ) {
      assert( sname != "" );
      assert( current() );
#ifdef DATA_GRAPHICS_DEBUG
      current()->validate();
#endif
      SubColl::iterator i = current()->_children.find(sname);
      if( i != current()->_children.end() ) {
        _current = i->second;
        return;
      }
      DgCollection* g = new DgCollection( current() , sname , description );
      assert( g );
      _current = g;
    }
    // move up to the parent group
    inline static void up() { _current = current()->_parent; }
    // retrieve the parent group
    inline const DgCollection* parent() const { return _parent; }
    // return full "path" for a graphics object in the specified
    // collection (or in the current collection, if no collection is
    // specified.)
    static std::string path_to( const std::string& sname , const DgCollection* coll = 0 );
    // get list of available data objects
    typedef ResourceIDMap ResourceColl;
    inline const ResourceColl& graphics() const { return _resource_id_map; }
    // get list of the available subcollections
    inline const SubColl& subcollections() const { return _children; }
    // inline const AxisColl& axes() const { return _axes; }
    // does subcollection exist?
    inline static const bool is_subcollection( const std::string& sname ) { 
      assert( current() );
      return current()->_children.find(sname)!=current()->_children.end();
    }
  //   // does axis exist?
  //   static inline const DgAxis* axis( const std::string& ax_name ) { 
  //     assert( current() );
  //     AxisColl::const_iterator i=_current->_axes.find(ax_name);
  //     return( i!=_current->_axes.end() ? i->second : 0 );
  //   }
  // public:
  //   // add an axis
  //   static void add_axis( DgAxis* x );
  //   // friend class DgAxis;
  public:
    // look up all graphics object names matching pattern.  "search_anywhere" looks for objects
    // in all collections.  "search_down" looks for objects in the specified collection and its child collections.
    // "search_in" looks for objects in the specified collection only.  search looks in the this collection. 
    // if the specified collection pointer is null or unspecified, the current collection is used. 
    // "pattern" supports limited regular expressions via TRegexp.
    static unsigned int search_anywhere( View& view , const std::string& pattern ) { return search_down(view,pattern,root()); }
    static unsigned int search_down( View& view , const std::string& pattern , const DgCollection* coll = 0 );
    static unsigned int search_in( View& view ,  const std::string& pattern , const DgCollection* coll = 0 ) {
      if( coll ) { return coll->search(view,pattern); } else { assert(current()); return current()->search(view,pattern); }
    }
    unsigned int search( View& view , const std::string& pattern ) const;
    // retrieve graphics object or collection with absolute or relative path.
    static const DgCollection::View get( const std::string& path );
    static const DgCollection* get_coll( const std::string& path );
    // ================================================================
    // if global_weight is set, all "fill" calls are redirected to corresponding "fillw"
    static const bool has_global_weight() { return( _global_weight ); }
    static const WeightedCount global_weight() { return( _global_weight ? *_global_weight : 1. ); }
    static const WeightedCount get_global_weight() { return global_weight(); }
    static DgEventInfo& event_info() { return _event_info; }
    static DgBin& bin() { return _binning; }
    static void set_global_weight( const WeightedCount& v ) { _global_weight.reset( v ); }
    static void mult_global_weight( const WeightedCount& v ) { _global_weight.reset( global_weight()*v ); }
    static const WeightedCount setret_global_weight( const WeightedCount& v ) { _global_weight.reset( v ); return 1.0; }
    //
    // fill 1d histogram with fixed binning. Functions ending with "2" are for the alternative histo implementation
    static void fillh( const std::string& sname , const BinSize& nbins , const Value& min , const Value& max ,
                       const Value& x );
    static void fillh2( const std::string& sname , const BinSize& nbins , const Value& min , const Value& max ,
			const Value& x );
    static void fillh( const std::string& sname , const BinSize& nbins , const Value& min , const Value& max ,
                       const Value& x , const std::string& axis_label );
    static void fillh2( const std::string& sname , const BinSize& nbins , const Value& min , const Value& max ,
			const Value& x , const std::string& axis_label );
    // fill 1d histogram with variable binning
    static void fillvh( const std::string& sname , const BinSize& nbins , const std::vector<Value>& bins ,
                        const Value& x );
    static void fillvh2( const std::string& sname , const BinSize& nbins , const std::vector<Value>& bins ,
			 const Value& x );
    static void fillvh( const std::string& sname , const BinSize& nbins , const std::vector<Value>& bins ,
                        const Value& x , const std::string& axis_label );
    static void fillvh2( const std::string& sname , const BinSize& nbins , const std::vector<Value>& bins ,
			 const Value& x , const std::string& axis_label );
    // fill weighted 1d histogram with fixed binning
    static void fillhw( const std::string& sname , const BinSize& nbins , const Value& min , const Value& max ,
                        const Value& x , const WeightedCount& weight );
    static void fillhw2( const std::string& sname , const BinSize& nbins , const Value& min , const Value& max ,
			 const Value& x , const WeightedCount& weight );
    static void fillhw( const std::string& sname , const BinSize& nbins , const Value& min , const Value& max ,
                        const Value& x , const WeightedCount& weight, const std::string& axis_label );
    static void fillhw2( const std::string& sname , const BinSize& nbins , const Value& min , const Value& max ,
			 const Value& x , const WeightedCount& weight, const std::string& axis_label );
    // fill weighted 1d histogram with variable binning
    static void fillvhw( const std::string& sname , const BinSize& nbins , const std::vector<Value>& bins ,
                         const Value& x , const WeightedCount& weight );
    static void fillvhw2( const std::string& sname , const BinSize& nbins , const std::vector<Value>& bins ,
			  const Value& x , const WeightedCount& weight );
    static void fillvhw( const std::string& sname , const BinSize& nbins , const std::vector<Value>& bins ,
                         const Value& x , const WeightedCount& weight, const std::string& axis_label );
    static void fillvhw2( const std::string& sname , const BinSize& nbins , const std::vector<Value>& bins ,
			  const Value& x , const WeightedCount& weight, const std::string& axis_label );
    // fill 2d histogram
    static void fillh( const std::string& sname , 
                       const BinSize& nbinsx , const Value& minx , const Value& maxx , 
                       const BinSize& nbinsy , const Value& miny , const Value& maxy , 
                       const Value& x , const Value& y , const std::string& axis_label_x , const std::string& axis_label_y );
    static void fillh2( const std::string& sname , 
			const BinSize& nbinsx , const Value& minx , const Value& maxx , 
			const BinSize& nbinsy , const Value& miny , const Value& maxy , 
			const Value& x , const Value& y , const std::string& axis_label_x , const std::string& axis_label_y );
    static void fillvh2( const std::string& sname , 
			 const BinSize& nbinsx , const std::vector<Value>& binsx ,
			 const BinSize& nbinsy , const std::vector<Value>& binsy ,
			 const Value& x , const Value& y , const std::string& axis_label_x , const std::string& axis_label_y );
    // fill weighted 2d histogram
    static void fillhw( const std::string& sname , 
                        const BinSize& nbinsx , const Value& minx , const Value& maxx , 
                        const BinSize& nbinsy , const Value& miny , const Value& maxy , 
                        const Value& x , const Value& y , const WeightedCount& weight ,
                        const std::string& axis_label_x , const std::string& axis_label_y );
    static void fillhw2( const std::string& sname , 
			 const BinSize& nbinsx , const Value& minx , const Value& maxx , 
			 const BinSize& nbinsy , const Value& miny , const Value& maxy , 
			 const Value& x , const Value& y , const WeightedCount& weight ,
			 const std::string& axis_label_x , const std::string& axis_label_y );
    static void fillvhw2( const std::string& sname , 
			  const BinSize& nbinsx , const std::vector<Value>& binsx ,
			  const BinSize& nbinsy , const std::vector<Value>& binsy ,
			  const Value& x , const Value& y , const WeightedCount& weight ,
			  const std::string& axis_label_x , const std::string& axis_label_y );
    // 3d histograms (only implemented as DgTH*D classes)
    static void fillh2( const std::string& sname , 
			const BinSize& nbinsx , const Value& minx , const Value& maxx , 
			const BinSize& nbinsy , const Value& miny , const Value& maxy ,
			const BinSize& nbinsz , const Value& minz , const Value& maxz , 
			const Value& x , const Value& y , const Value& z ,
			const std::string& axis_label_x , const std::string& axis_label_y , const std::string& axis_label_z );
    static void fillvh2( const std::string& sname , 
			 const BinSize& nbinsx , const std::vector<Value>& binsx ,
			 const BinSize& nbinsy , const std::vector<Value>& binsy ,
			 const BinSize& nbinsz , const std::vector<Value>& binsz ,
			 const Value& x , const Value& y , const Value& z ,
			 const std::string& axis_label_x , const std::string& axis_label_y , const std::string& axis_label_z );
    static void fillhw2( const std::string& sname , 
			 const BinSize& nbinsx , const Value& minx , const Value& maxx , 
			 const BinSize& nbinsy , const Value& miny , const Value& maxy ,
			 const BinSize& nbinsz , const Value& minz , const Value& maxz , 
			 const Value& x , const Value& y , const Value& z , const WeightedCount& weight ,
			 const std::string& axis_label_x , const std::string& axis_label_y , const std::string& axis_label_z );
    static void fillvhw2( const std::string& sname , 
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
                        const Value& x , const Value& y , const WeightedCount& weight );
    // fill graph
    static void fillgraph( const std::string& sname , const Value& x, const Value& y , const std::string& axis_label_x , const std::string& axis_label_y );
    // fill ntuple of floats
    static void fillnt( const std::string& sname , const std::vector<float>& x, const std::vector<std::string>& names );
    static void fillntw( const std::string& sname , const std::vector<float>& x, const std::vector<std::string>& names, const DgEventInfo& w );
    // fill 0d efficiency plot
    static void filleff( const std::string& sname , const bool& pass );
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
    // fill weighted 0d efficiency plot (use for event counts only)
    static void filleffw( const std::string& sname , const bool& pass , const WeightedCount& weight);
    // remove a graphics object
    static void remove_graphic( const std::string& sname );
    // access graphics object directly
    inline static DgCollection::View lookup_graphic( const std::string& sname ) {
      assert( current() );
      assert( current()->_read_on_demand ); // not implemented for use otherwise
      DgCollection::View result;
      ResourceIDMap::iterator i = current()->_resource_id_map.find( sname );
      if( i!=current()->_resource_id_map.end() ) {
        result.push_back( View::value_type(current(),i->first,i->second) );
      }
      return result;
    }
    const unsigned long count_data_objects() const {
      unsigned long ndata = _graphics.size();
      for( SubColl::const_iterator i=_children.begin(), f=_children.end(); i!=f; ++i ) {
        ndata += (*i).second->count_data_objects();
      }
      return ndata;
    }
    const unsigned long count_collections() const {
      if( _children.empty() ) { 
        return 1;
      } else { 
        unsigned long nsub = 0ul;
        for( SubColl::const_iterator i=_children.begin(), f=_children.end(); i!=f; ++i ) {
          nsub += (*i).second->count_collections();
        }
        return nsub;
      }
    }
    const unsigned long max_depth() const {
      if( _children.empty() ) { return 0; }
      unsigned long depth = 0ul;
      for( SubColl::const_iterator i=_children.begin(), f=_children.end(); i!=f; ++i ) {
        depth = std::max( depth , (*i).second->max_depth() );
      }
      return depth;
    }

    // check validity of all graphics pointers and subcollection pointers.
    void validate() const {
      for( Coll::const_iterator i=_graphics.begin(), f=_graphics.end(); i!=f; ++i ) {
        assert( !(graphic_name(i).empty()) );
        assert( graphic_ptr(i) );
      }
      for( SubColl::const_iterator i=_children.begin(), f=_children.end(); i!=f; ++i ) {
        assert( !(subcollection_name(i).empty()) );
        assert( subcollection_ptr(i) );
      }
      for( SubColl::const_iterator i=_children.begin(), f=_children.end(); i!=f; ++i ) {
        subcollection_ptr(i)->validate();
      }
    }

  public:
#ifndef __CINT__
    template<class archiveT> void save( archiveT & ar , const unsigned int version ) const;
    template<class archiveT> void load( archiveT & ar , unsigned int version );
    BOOST_SERIALIZATION_SPLIT_MEMBER()
#endif

    public:
    
  }; // end DgCollection

  typedef DgCollection dg;

};

#undef inline

/*
  A helper macro that temporarily (within scope) reverts dg global weight to 1.0
  DGUNWEIGHTED {
     dg::fillh("unweighted_histo",<...>)
  }
 */
#define DGUNWEIGHTED for(WeightedCount lpval__tmp=dg::global_weight(),lpexit__tmp=dg::setret_global_weight(1.0); lpexit__tmp!=0; dg::set_global_weight(lpval__tmp),lpexit__tmp=0)

// helper macro for "down" and then returning "up" when exiting scope
#define DG_DOWN_FOR_SCOPE(name,desc) DgCollection::down(name,desc); BOOST_SCOPE_EXIT() { DgCollection::up(); } BOOST_SCOPE_EXIT_END
      

#endif // WRAP_DGCOLLECTION_HH
