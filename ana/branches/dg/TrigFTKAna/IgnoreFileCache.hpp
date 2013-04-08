#ifndef IGNOREFILECACHE_HPP
#define IGNOREFILECACHE_HPP

#include <string>
#include <iostream>
#include <fstream>
#include <set>
#include <stdexcept>
#include <algorithm>
#include <utility>
#include <boost/version.hpp>
#include <boost/function.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/set.hpp>
#include <boost/interprocess/sync/file_lock.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/filesystem.hpp>

class
IgnoreFileCache
{
private:
  boost::filesystem::path _db_filename;
  typedef std::set< std::string > set_type;
  set_type _filenames;
private:
  void _save_cache() {
#if BOOST_VERSION>=104800
    std::ofstream outf( _db_filename.string().c_str() );
#else
    std::ofstream outf( _db_filename.file_string().c_str() );
#endif
    if( !outf ) { throw; }
    boost::archive::xml_oarchive aro( outf );
    aro << BOOST_SERIALIZATION_NVP(_filenames);
    outf.flush();
    outf.close();
  }
  void _load_cache() {
#if BOOST_VERSION>=104800
    std::ifstream inf( _db_filename.string().c_str() );
#else
    std::ifstream inf( _db_filename.file_string().c_str() );
#endif
    if( !inf ) { throw; }
    boost::archive::xml_iarchive ar( inf );
    ar >> BOOST_SERIALIZATION_NVP(_filenames);
    inf.close();
  }
public:
  IgnoreFileCache( const std::string& db_filename ) {
    _db_filename = boost::filesystem::path( db_filename );
    if( boost::filesystem::extension(_db_filename) != "xml" ) { 
#if BOOST_VERSION>=104800
      _db_filename.replace_extension( ".xml" );
#else
      _db_filename.replace_extension( "xml" );
#endif
    }
    try {
      if( boost::filesystem::exists( _db_filename ) ) { 
        // lock the input file, so that it does not change during the read.
        boost::interprocess::file_lock lock( _db_filename.string().c_str() );
        boost::interprocess::scoped_lock<boost::interprocess::file_lock> sc_lock( lock );
        _load_cache(); 
      }
    } catch( ... ) {
      std::cout << "warning: could not obtain lock on ignore file cache: " << _db_filename << std::endl;
      std::cout << "         ignoring cache data" << std::endl;
    }
  }
  virtual ~IgnoreFileCache() {
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
      set_type null_cache;
      null_cache.swap( _filenames );
      _save_cache();
      null_cache.swap( _filenames );
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
      set_type tmp_set( _filenames );
      // reload current cache from file
      _load_cache();
      // merge caches
      set_type tmp_merge;
      std::merge( tmp_set.begin() , tmp_set.end() , _filenames.begin() , _filenames.end() , std::inserter(tmp_merge,tmp_merge.begin()) );
      _filenames.swap( tmp_merge );
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
  
  const bool check( const std::string& filename ) const {
    return false; // FIXME: temp. hack to force processing of all files; should add this as a possible setting.
    set_type::const_iterator i = _filenames.find(filename);
    const bool found = (i!=_filenames.end());
    if( found ) { 
      std::cout << " IgnoreFileCache: file " << filename << " flagged as ignored." << std::endl;
    }
    return( found );
  }
  void add( const std::string& filename ) {
    _filenames.insert( filename );
    std::cout << " IgnoreFileCache: added " << filename << " to the list of files to be ignored." << std::endl;
  }
  template<typename iterT>
  void add( iterT beg , iterT end ) {
    for( iterT i=beg; i!=end; ++i ) {
      add( *i );
    }
  }

  
};

#endif // IGNOREFILECACHE_HPP

