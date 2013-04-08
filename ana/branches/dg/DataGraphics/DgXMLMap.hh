#ifndef DG_XML_MAP_H
#define DG_XML_MAP_H

// Generic serializable mapping from a string to a value. 
// author: Anton Kapliy
// nit-picking: Antonio Boveia

#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <cstdlib>
#include <numeric>

#include <boost/algorithm/string.hpp>
#include <boost/unordered_map.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/filesystem.hpp>
#include "DataGraphics/serialize_unordered.hpp"

template<class valueT>
class DgXMLMap
{

protected:

  typedef boost::unordered_map< std::string , valueT > map_type;
  typedef std::pair< std::string , valueT > pair_type;
  map_type _data;
  valueT _default;

public:

  DgXMLMap() {}
  DgXMLMap(const valueT& def) : _default(def) {}
  virtual ~DgXMLMap() {}

  // return the first key string that appears in "pattern"
  const std::string key( const std::string& pattern ) const {
    static const std::string empty = std::string();
    for( typename map_type::const_iterator i=_data.begin(), f=_data.end(); i!=f; ++i ) { 
      if( boost::icontains( pattern , i->first ) ) {
        return i->first;
      }
    }
    return empty;
  }
  // return the value of the first key string that appears in "pattern"
  const valueT value( const std::string& pattern ) const {
    const std::string i( key(pattern) );
    return i.empty() ? _default : _data.find(i)->second;
  }
  // return the key-value pair of the first key string that appears in "pattern"
  const std::pair<std::string,valueT> key_value( const std::string& pattern ) const {
    const std::string i = key(pattern);
    return i.empty() ? pair_type(i,_default) : pair_type(i,_data.find(i)->second);
  }
  //
  const bool load( const std::string& filename ) {
    boost::shared_ptr<std::istream> inf( boost::make_shared<std::ifstream>( filename.c_str() ) );
    if( !inf || !(*inf) ) { 
      std::cout << " WARNING: could not load xml data from filename " << filename << std::endl;
      return false; 
    }
    try {
      boost::shared_ptr<boost::archive::xml_iarchive> ari( new boost::archive::xml_iarchive(*inf) );
      (*ari) >> BOOST_SERIALIZATION_NVP(_data);
      ari.reset(); // destroy the iarchive before the stream closes.
      inf.reset(); // inf passed by reference to xml_iarchive; it must be destroyed after archive.
    } catch( ... ) {
      std::cout << " WARNING: exception; could not load xml data from filename " << filename << std::endl;
      return false;
    }
    return true;
  }
  void save( const std::string& filename ) const {
    boost::shared_ptr<std::ostream> outf( boost::make_shared<std::ofstream>( filename.c_str() ) );
    if( !outf || !(*outf) ) { 
      std::cout << " WARNING: could not save xml data to filename " << filename << std::endl;
      return; 
    }
    try { 
      boost::shared_ptr<boost::archive::xml_oarchive> aro( new boost::archive::xml_oarchive(*outf) );
      (*aro) << BOOST_SERIALIZATION_NVP(_data);
      outf->flush();
      aro.reset(); // destroy the oarchive before the stream closes.
      outf.reset(); // outf passed by reference to xml_oarchive; it must be destroyed after archive
    } catch( ... ) {
      std::cout << " WARNING: exception; could not save xml data to filename " << filename << std::endl;
      return;
    }
  }
};

// a version of the map indexed by an integer (much simplified)
template <class valueT>
class DgXMLIntMap {

protected:

  typedef std::map< int , valueT > map_type;
  typedef std::pair< int , valueT > pair_type;
  typedef std::pair< const int , valueT > cpair_type;
  map_type _data;
  valueT _default;

public:

  DgXMLIntMap() {}
  DgXMLIntMap( const valueT& def ) : _default(def) {}
  virtual ~DgXMLIntMap() {}

  // find the value indexed by integer key 'pattern'
  const valueT value( const int& pattern ) const {
    typename map_type::const_iterator res = _data.find(pattern);
    return res==_data.end() ? _default : res->second;
  }

  const bool load( const std::string& filename ) {
    boost::shared_ptr<std::istream> inf( boost::make_shared<std::ifstream>( filename.c_str() ) );
    if( !inf || !(*inf) ) { 
      std::cout << " WARNING: could not load xml data from filename " << filename << std::endl;
      return false; 
    }
    try {
      boost::shared_ptr<boost::archive::xml_iarchive> ari( new boost::archive::xml_iarchive(*inf) );
      (*ari) >> BOOST_SERIALIZATION_NVP(_data);
      ari.reset(); // destroy the iarchive before the stream closes.
      inf.reset(); // inf passed by reference to xml_iarchive; it must be destroyed after archive.
    } catch( ... ) {
      std::cout << " WARNING: exception; could not load xml data from filename " << filename << std::endl;
      return false;
    }
  }

  void save( const std::string& filename ) {
    boost::shared_ptr<std::ostream> outf( boost::make_shared<std::ofstream>( filename.c_str() ) );
    if( !outf || !(*outf) ) { 
      std::cout << " WARNING: could not save xml data to filename " << filename << std::endl;
      return; 
    }
    try {
      boost::shared_ptr<boost::archive::xml_oarchive> aro( new boost::archive::xml_oarchive(*outf) );
      (*aro) << BOOST_SERIALIZATION_NVP(_data);
      outf->flush();
      aro.reset(); // destroy the oarchive before the stream closes.
      outf.reset(); // outf passed by reference to xml_oarchive; it must be destroyed after archive
    } catch( ... ) {
      std::cout << " WARNING: exception; could not save xml data to filename " << filename << std::endl;
      return false;
    }
  }

  const std::pair<int,int> min_max_key() const {
    int _min_key = std::numeric_limits<int>::max();
    int _max_key = std::numeric_limits<int>::min();
    for( typename map_type::const_iterator i=_data.begin(), f=_data.end(); i!=f; ++i ) {
      const int& key( i->first );
      if( key < _min_key ) { key = _min_key; }
      if( key > _max_key ) { key = _max_key; }
    }
    return std::pair<int,int>( _min_key , _max_key );
  }
};

#endif
