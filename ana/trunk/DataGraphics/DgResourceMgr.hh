#ifndef WRAP_DGRESOURCEMGR_HPP
#define WRAP_DGRESOURCEMGR_HPP

#include <string>
#include <vector>
#include <map>
#include <list>
#include <utility>
#include <iterator>
#include <cmath>
#include <cassert>
#include <iostream>
#include <algorithm>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/unordered_map.hpp>
#include <algorithm>
#include <numeric>

namespace DataGraphics { class DgAbsGraphic; }


// interface to a "resource", i.e. a C++ class instance (in this case
// a DgAbsGraphic) that may reside in memory or may be sitting in a
// file on disk.  the resource itself is created by the DgResourceMgr
// singleton.

class TFile;

class
DgResourceMgr
{
public:
  typedef unsigned long ID;
protected:
  struct
  DgResource
  {
    typedef enum { GRAPHIC , FILE } ResourceType;
    ResourceType type;
    ID id;
    ID file_id;
    std::string filename; // disk file
    std::string path; // path within disk file to graphic
    boost::shared_ptr<DataGraphics::DgAbsGraphic> graphic;
    boost::shared_ptr<TFile> file;
    const unsigned long load(); // returns graphic size in bytes
    void unload();
    const bool valid() const { !id; }
    const bool unique() const {
      if( type==GRAPHIC ) { return graphic && graphic.unique(); }
      else if( type==FILE ) { return file && file.unique(); }
      else { assert(!"unhandled"); }
    }
    const bool loaded() const {
      if( type==GRAPHIC ) { return graphic; }
      else if( type==FILE ) { return file; }
      else { assert(!"unhandled"); }
    }
    DgResource() {}
    DgResource( const ID& v_id ,
                const std::string& v_filename ,
                const std::string& v_path ,
                const ID& v_file_id )
      : type(DgResource::GRAPHIC) , id(v_id) , filename(v_filename) , path(v_path) , file_id(v_file_id) {}
    DgResource( const ID& v_id ,
                const std::string& v_filename )
      : type(DgResource::FILE) , id(v_id) , filename(v_filename) {}
  };
private:
  // typedef std::map< ID , DgResource > ResourceColl;
  // typedef std::map< ID , unsigned long > SizeColl;
  // typedef std::map< ID , unsigned long > FreqColl;
  // typedef std::map< ID , unsigned long > HistoryColl;
  typedef boost::unordered_map< ID , DgResource > ResourceColl;
  typedef boost::unordered_map< ID , unsigned long > SizeColl;
  typedef boost::unordered_map< ID , unsigned long > FreqColl;
  typedef boost::unordered_map< ID , unsigned long > HistoryColl;
  typedef std::map< std::string , ID > FileLookup;
  ID _next_id;
  ResourceColl _resources;
  SizeColl _sizes;
  FreqColl _freqs;
  HistoryColl _history;
  FileLookup _files;
  unsigned long _total_mem;
  unsigned long _max_mem;
  unsigned long _max_freq;
  unsigned long _misses;
  unsigned long _hits;
  unsigned long _requests;
protected:
  DgResourceMgr()
    : _next_id(1ul)  // 0=invalid resource
    , _resources()
    , _sizes()
    , _freqs()
    , _history()
    , _files()
    , _total_mem(0)
    , _max_mem(629145600) // 600 MB
    , _max_freq(0)
    , _misses(0ul)
    , _hits(0ul)
    , _requests(0ul)
  {}
  virtual ~DgResourceMgr();
  static DgResourceMgr* _instance;
public:
  static inline DgResourceMgr* instance() { 
    if( !_instance ) {
      _instance = new DgResourceMgr;
    }
    return _instance;
  }

  static const ID add_graphic_resource( const std::string& filename , const std::string& path , const DgResourceMgr::ID& file_id ) {
    DgResourceMgr* mgr = instance();
    mgr->_resources[ mgr->_next_id ] = DgResource( mgr->_next_id , filename , path , file_id );
    mgr->_sizes[ mgr->_next_id ] = 0;
    // don't reset frequency if it already exists; mgr->_freqs[ mgr->_next_id ] = 0;
    ID this_id = mgr->_next_id;
    ++(mgr->_next_id);
    return this_id;
  }

  static const ID add_file_resource( const std::string& filename ) {
    DgResourceMgr* mgr = instance();
    FileLookup::iterator ifile=mgr->_files.find( filename );
    if( ifile != mgr->_files.end() ) { return ifile->second; }
    mgr->_resources[ mgr->_next_id ] = DgResource( mgr->_next_id , filename );
    mgr->_sizes[ mgr->_next_id ] = 0;
    // don't reset frequency if it already exists; mgr->_freqs[ mgr->_next_id ] = 0;
    ID this_id = mgr->_next_id;
    mgr->_files[ filename ] = this_id;
    ++(mgr->_next_id);
    return this_id;
  }

  static const boost::shared_ptr<TFile> get_file( const ID& id );

  static const boost::shared_ptr<DataGraphics::DgAbsGraphic> get_graphic( const ID& id ) {
    DgResourceMgr* mgr = instance();
    ++(mgr->_requests);
    ResourceColl::iterator i = mgr->_resources.find( id );
    if( i==mgr->_resources.end() ) {       
      return boost::shared_ptr<DataGraphics::DgAbsGraphic>(); 
    }
    unsigned long& freq = mgr->_freqs[ id ];
    ++freq;
    mgr->_max_freq = std::max( freq , mgr->_max_freq );
    DgResource& res( i->second );
    if( res.type != DgResource::GRAPHIC ) { 
      return boost::shared_ptr<DataGraphics::DgAbsGraphic>(); 
    }
    if( res.graphic ) { 
      ++mgr->_hits;
      mgr->_history[ id ] = (mgr->_requests)-1;
      return res.graphic; 
    }
    // not loaded yet. load
    ++mgr->_misses;
    const unsigned long nbytes = res.load();
    mgr->_sizes[ id ] = nbytes;
    mgr->_history[ id ] = (mgr->_requests)-1;
    mgr->_total_mem += nbytes;
    assert( res.graphic );
    return res.graphic;
  }
  
  // not necessary to call release, by it is a courtesy. if you
  // still have the shared_ptr, reset it first, or this will do nothing.
  static void release( const ID& id ) {
    DgResourceMgr* mgr = instance();
    ResourceColl::iterator i = mgr->_resources.find( id );
    assert( i != mgr->_resources.end() );
    DgResource& res( i->second );
    if( res.type == DgResource::FILE ) {
      if( ! res.file ) { return; } // not acquired
    } else { 
      assert( res.type == DgResource::GRAPHIC );
      if( ! res.graphic ) { return; } // not acquired
    }
    if( ! res.unique() ) { return; } // someone else still using this 
    res.unload();
    mgr->_resources.erase( i );
    SizeColl::iterator j = mgr->_sizes.find( id );
    assert( mgr->_total_mem >= j->second );
    mgr->_total_mem -= j->second;
    mgr->_sizes.erase( j );
    // keep frequency
    return;
  }

  // periodic cleanup
  static void maintenance() {
    // check memory used. if more than max allowed, look for stuff to drop from the cache.
    DgResourceMgr* mgr = instance();
    if( mgr->_resources.empty() ) { return; }
    //
    if( mgr->_total_mem < ( (mgr->_max_mem) >> 1) ) {
      // still less than half the limit. plenty o' memory. do nothing.
      return;
    }
    // look for stuff to decache. prefer big stuff that is infrequently accessed and hasn't been accessed in awhile.
    // first try the lower-ID stuff, since it has a higher probability of irrelevance.
    ID max_id = 0;
    size_t nresources = mgr->_resources.size();
    {
      ResourceColl::const_iterator ir=mgr->_resources.begin();
      std::advance( ir , std::min(static_cast<size_t>(50),nresources-1) );
      max_id = ir->first;
    }
    remove_unused_resources( max_id );
    // if total memory used is greater than the max allowed, something will have to be de-allocated.
    float delete_goal = mgr->_total_mem - mgr->_max_mem;
    float weight_cut = 0.5;
    std::vector<ID> todelete;
    unsigned long del_size = 0ul;
    while( mgr->_total_mem > mgr->_max_mem && weight_cut>0.1 ) {
      // consider each resource, starting with the low IDs. 
      for( ResourceColl::iterator i=mgr->_resources.begin() , f=mgr->_resources.end(); i!=f; ++i ) {
        const ID& res_id( i->first );
        DgResource& res( i->second );
        // veto resources still used
        if( ! res.unique() ) { continue; }
        // time weight -- oldest request has weight = 1, newest has weight 0.
        const float time_weight = ( mgr->_requests - (mgr->_history[res_id]) ) / static_cast<float>(mgr->_requests);
        // size weight -- 0.1 for each kb.
        const unsigned long res_size = mgr->_sizes[res_id];
        const float size_weight = ( res_size ) / 1024.;
        // frequency weight -- most frequent has weight = 0.001, least frequent has weight 1.
        const float freq_weight = std::max( ( mgr->_max_freq - (mgr->_freqs[res_id]) ) / static_cast<float>(mgr->_max_freq) , 0.001f );
        // total weight
        const float total_weight = std::abs( time_weight * size_weight * freq_weight );
        if( total_weight >= weight_cut ) {
          del_size += res_size;
          todelete.push_back( res_id );
        }
        if( del_size > delete_goal ) { break; }
      }
      // decrease weight cut.
      weight_cut = std::max( 0. , weight_cut - 0.25 ); // weight cannot be smaller than zero.
    }
    // meet the goal yet? if not, just start deleting everything that isn't actively used.
    if( del_size > delete_goal ) {
      for( std::vector<ID>::iterator i=todelete.begin(), f=todelete.end(); i!=f; ++i ) {
        release( *i );
      }
    } else {
      // delete stuff identified
      for( std::vector<ID>::iterator i=todelete.begin(), f=todelete.end(); i!=f; ++i ) {
        release( *i );
      }
      // start deleting everything else
      while( del_size <= delete_goal && ( ! mgr->_resources.empty() ) ) {
        ResourceColl::iterator i = mgr->_resources.begin();
        DgResource& res( i->second );
        if( ! res.unique() ) { continue; }
        del_size += mgr->_sizes[ i->first ];
        release( i->first );
      }
      // maybe complain to the user here.
    }
  }
  
  static void remove_unused_resources( const ID& max_id ) {
    DgResourceMgr* mgr = instance();
    std::vector<ID> delete_these;
    for( ResourceColl::iterator i=mgr->_resources.begin() , f=mgr->_resources.end(); i!=f; ++i ) {
      if( i->first > max_id ) { break; }
      if( i->second.unique() ) { 
        delete_these.push_back( i->first );
      }
    }
    for( std::vector<ID>::iterator i=delete_these.begin(), f=delete_these.end(); i!=f; ++i ) {
      mgr->release( *i );
    }
  }
    
  static const unsigned long total_size() { return instance()->_total_mem; }
  static const unsigned long total_resources() { return instance()->_resources.size(); }
  static const unsigned long total_resources_loaded() { std::count_if( instance()->_resources.begin() , instance()->_resources.end() , 
                                                                       boost::bind( &DgResource::loaded , boost::bind(&ResourceColl::value_type::second,_1) ) == true ); }
  static const unsigned long total_file_resources_loaded() { std::count_if( instance()->_resources.begin() , instance()->_resources.end() , 
                                                                            boost::bind( &DgResource::loaded , boost::bind(&ResourceColl::value_type::second,_1) ) == true &&
                                                                            boost::bind( &DgResource::type , boost::bind(&ResourceColl::value_type::second,_1) ) == DgResource::FILE ); }
  static const unsigned long total_requests() { return instance()->_requests; }
  static const unsigned long total_hits() { return instance()->_hits; }
  static const unsigned long total_misses() { return instance()->_misses; }

  std::ostream& print( std::ostream& os ) {
    using std::endl; 
    os << "ResourceMgr report: " << endl;
    os << "  total number of resources       : " << total_resources() << endl;
    os << "  total number of resources loaded: " << total_resources_loaded() << endl;
    os << "  total number of resource files loaded: " << total_file_resources_loaded() << endl;
    os << "  total RAM used (kb): " << (total_size() >> 10) << endl;
    os << "  max RAM threshold (kb): " << (instance()->_max_mem >> 10) << endl;
    os << "  total cache requests: " << total_requests() << endl;
    if( total_requests()>0 ) {
      os << "  total cache hits    : " << total_hits() << " " << static_cast<float>(total_hits())/total_requests() << "%" << endl;
      os << "  total cache misses    : " << total_misses() << " " << static_cast<float>(total_misses())/total_requests() << "%" << endl;
    }
    return os;
  }


};


#endif // WRAP_DGRESOURCEMGR_HPP
