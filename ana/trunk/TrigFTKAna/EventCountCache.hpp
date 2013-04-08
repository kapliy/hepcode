#ifndef EVENTCOUNTCACHE_HPP
#define EVENTCOUNTCACHE_HPP

#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <stdexcept>
#include <algorithm>
#include <utility>
#include <boost/version.hpp>
#include <boost/function.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/map.hpp>
#include <boost/interprocess/sync/file_lock.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/filesystem.hpp>

class
EventCountCache
{
private:
  boost::filesystem::path _db_filename;
  typedef std::map< std::string , unsigned long > map_type;
  map_type _counts;
private:
  void _save_cache() {
#if BOOST_VERSION>=104800
    std::ofstream outf( _db_filename.string().c_str() );
#else
    std::ofstream outf( _db_filename.file_string().c_str() );
#endif
    if( !outf || !outf.good() ) { throw; }
    boost::archive::xml_oarchive aro( outf );
    aro << BOOST_SERIALIZATION_NVP(_counts);
    outf.flush();
    outf.close();
  }
  void _load_cache() {
#if BOOST_VERSION>=104800
    std::ifstream inf( _db_filename.string().c_str() );
#else
    std::ifstream inf( _db_filename.file_string().c_str() );
#endif
    if( !inf || !inf.good() ) { throw; }
    boost::archive::xml_iarchive ar( inf );
    ar >> BOOST_SERIALIZATION_NVP(_counts);
    inf.close();
  }
public:
  EventCountCache( const std::string& filename ) {
    _db_filename = boost::filesystem::path( filename );
    if( boost::filesystem::extension(_db_filename) != "xml" ) { 
#if BOOST_VERSION>=104800
      _db_filename.replace_extension( ".xml" );
#else
      _db_filename.replace_extension( "xml" );
#endif
    }
    if( boost::filesystem::exists( _db_filename ) ) { 
      try { 
        // lock the input file, so that it does not change during the read.
        boost::interprocess::file_lock lock( _db_filename.string().c_str() );
        boost::interprocess::scoped_lock<boost::interprocess::file_lock> sc_lock( lock );
        _load_cache(); 
      } catch( ... ) {
        std::cout << "warning: could not obtain lock on event count cache: " << _db_filename << std::endl;
        std::cout << "         ignoring cache data" << std::endl;
      }
    }
  }
  virtual ~EventCountCache() {
    // lock will throw this exception if the file doesn't exist. create a null one.
    // is there a better way to do this?
    if( !boost::filesystem::exists( _db_filename ) ) {
      // "touch" the file
#if BOOST_VERSION>=104800
      std::ofstream tmp_outf( _db_filename.string().c_str() );
#else
      std::ofstream tmp_outf( _db_filename.file_string().c_str() );
#endif
      if( !tmp_outf ) { throw; }
      tmp_outf.flush();
      tmp_outf.close();
      // lock the file
      boost::interprocess::file_lock lock( _db_filename.string().c_str() );
      boost::interprocess::scoped_lock<boost::interprocess::file_lock> sc_lock( lock );
      // write a null cache
      map_type null_cache;
      null_cache.swap( _counts );
      _save_cache();
      null_cache.swap( _counts );
    }

    // lock the output file so that no other instance can change
    // it. since the file may have changed since the initial read,
    // re-read it and update the internal map. then archive the
    // updated map.
    boost::interprocess::file_lock lock( _db_filename.string().c_str() );
    try {
      // lock the file
      boost::interprocess::scoped_lock<boost::interprocess::file_lock> sc_lock( lock );
      // retain current cache state
      map_type tmp_map( _counts );
      // reload current cache from file
      _load_cache();
      // merge caches
      map_type tmp_merge;
      std::merge( tmp_map.begin() , tmp_map.end() , _counts.begin() , _counts.end() , std::inserter(tmp_merge,tmp_merge.begin()) );
      _counts.swap( tmp_merge );
      // write merged cache
      _save_cache();
    } catch ( boost::interprocess::interprocess_exception ) {
      std::cout << "warning: could not obtain lock on event count cache: " << _db_filename << std::endl;
      std::cout << "         cache was not saved" << std::endl;
    } catch (...) {
      std::cout << "warning: could not store event count cache: " << _db_filename << std::endl;
      std::cout << "         cache was not saved" << std::endl;
    }
  }
  
  template< typename bindT > 
  const unsigned long operator()( const std::string& filename , bindT counter ) {
    map_type::const_iterator i = _counts.find(filename);
    if( i!=_counts.end() ) { return i->second; }
    unsigned long n( counter(filename) );
    _counts[filename] = n;
    return n;
  }

  class SupplyComputedEventCount {
  protected:
    unsigned long _n;
  public:
    SupplyComputedEventCount( const unsigned long& n ) : _n(n) {}
    const unsigned long operator()( const std::string& filename ) const { return _n; }
  };

};

#endif // EVENTCOUNTCACHE_HPP

