#ifndef WRAP_SiModuleList_HPP
#define WRAP_SiModuleList_HPP

// SiModuleList
// ================================================================
// List of silicon modules (e.g. bad modules) 
// ================================================================
// 2011-04-11 Antonio Boveia (boveia@hep.uchicago.edu)

#include <numeric>
#include <cmath>
#include <iostream>
#include <set>
#include <boost/optional.hpp>
#include <boost/format.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/version.hpp>
#include <boost/serialization/set.hpp>
#include "TrigFTKAna/Detector.hpp"

class
SiModuleList
{

public:

  typedef float ftype;
  typedef enum { UNKNOWN_TYPE=0 , PIXEL=1 , SCT=2 } DetectorType;
  typedef enum { UNKNOWN_SIDE=0 , RPHI=1 , STEREO=2 } SideType;

public:

  class SiModule {
  private:
    DetectorType _detector_type;
    int _barrel_ec;
    int _layer_or_disk;
    unsigned int _phi_module;
    int _eta_module;
    boost::optional<SideType> _sct_side; // valid only for SCT
    friend class NtWrapper;
  private:
    friend class boost::serialization::access;
    template<class archiveT>
    void serialize( archiveT& ar, const unsigned int version ) {
      ar & boost::serialization::make_nvp("detector_type",_detector_type);
      ar & boost::serialization::make_nvp("barrel_ec",_barrel_ec);
      ar & boost::serialization::make_nvp("layer_or_disk",_layer_or_disk);
      ar & boost::serialization::make_nvp("phi_module",_phi_module);
      ar & boost::serialization::make_nvp("eta_module",_eta_module);
      ar & boost::serialization::make_nvp("sct_side",_sct_side); 
    }
  public:
    SiModule()
      : _detector_type( UNKNOWN_TYPE )
      , _barrel_ec( std::numeric_limits<int>::max() )
      , _layer_or_disk( std::numeric_limits<int>::max() )
      , _phi_module( std::numeric_limits<unsigned int>::max() )
      , _eta_module( std::numeric_limits<int>::max() )
      , _sct_side()
    {}
    SiModule( const int& barrel_ec , const int& layer_or_disk ,
              const unsigned int& phi_module , const int& eta_module )
      : _detector_type( PIXEL )
      , _barrel_ec( barrel_ec )
      , _layer_or_disk( layer_or_disk )
      , _phi_module( phi_module )
      , _eta_module( eta_module )
      , _sct_side()
    {}
    SiModule( const int& barrel_ec , const int& layer_or_disk ,
              const unsigned int& phi_module , const int& eta_module ,
              const SideType& side )
      : _detector_type( SCT )
      , _barrel_ec( barrel_ec )
      , _layer_or_disk( layer_or_disk )
      , _phi_module( phi_module )
      , _eta_module( eta_module )
      , _sct_side( side )
    {}

    const DetectorType detector_type() const { return _detector_type; }
    const int barrel_ec() const { return _barrel_ec; }
    const int layer_or_disk() const { return _layer_or_disk; }
    const unsigned int phi_module() const { return _phi_module; }
    const int eta_module() const { return _eta_module; }
    const boost::optional<SideType> sct_side() const { assert( _detector_type == SCT ); return _sct_side; } // valid only for SCT
    std::ostream& print( std::ostream& os ) const {
      switch( _detector_type ) {
      case PIXEL:
        os << "pix " << boost::format("be %|3d| ld %|3d| em %|3d| pm %|3d|") % _barrel_ec % _layer_or_disk % _eta_module % _phi_module << std::endl;
        break;
      case SCT:
        os << "sct " << boost::format("be %|3d| ld %|3d| em %|3d| pm %|3d| si %|2d| ") % _barrel_ec % _layer_or_disk % _eta_module % _phi_module % (_sct_side==RPHI ? "rp" : "st") << std::endl;
        break;
      default: assert( !"unhandled" );
      }
    }
    const bool operator==( const SiModule& rhs ) const {
      if( _detector_type != rhs._detector_type ) { return false; }
      if( _barrel_ec != rhs._barrel_ec ) { return false; }
      if( _layer_or_disk != rhs._layer_or_disk ) { return false; }
      if( _phi_module != rhs._phi_module ) { return false; }
      if( _eta_module != rhs._eta_module ) { return false; }
      if( _detector_type == SCT ) { if(_sct_side!=rhs._sct_side) { return false; } }
      return true;
    }
    const bool operator<( const SiModule& rhs ) const {
      if( _detector_type != rhs._detector_type ) { return( _detector_type==PIXEL ); }
      if( _barrel_ec != rhs._barrel_ec ) { return _barrel_ec < rhs._barrel_ec; }
      if( _layer_or_disk != rhs._layer_or_disk ) { return _layer_or_disk < rhs._layer_or_disk; }
      if( _phi_module != rhs._phi_module ) { return _phi_module < rhs._phi_module; }
      if( _eta_module != rhs._eta_module ) { return _eta_module < rhs._eta_module; }
      if( _detector_type == SCT ) { 
        if(_sct_side!=rhs._sct_side) { return _sct_side==RPHI; } 
      }
      return false;
    }

  }; // end SiModule class

public:

  typedef std::set< SiModule > ModuleList;
  typedef ModuleList::const_iterator const_iterator;
  typedef ModuleList::const_reference const_reference;

protected:

  ModuleList _modules;

private:

  friend class boost::serialization::access;
  template<class archiveT>
  void serialize( archiveT& ar, const unsigned int version ) {
    ar & boost::serialization::make_nvp("modules",_modules);
  }

public:

  SiModuleList() {}
  virtual ~SiModuleList() {}

  void clear() { _modules.clear(); }
  const unsigned int size() const { return _modules.size(); }

  void add( const int& barrel_ec , const int& layer_or_disk ,
            const unsigned int& phi_module , const int& eta_module ) {
    _modules.insert( SiModule(barrel_ec,layer_or_disk,phi_module,eta_module) );
  }
  void add( const int& barrel_ec , const int& layer_or_disk ,
            const unsigned int& phi_module , const int& eta_module , const SideType& side ) {
    _modules.insert( SiModule(barrel_ec,layer_or_disk,phi_module,eta_module,side) );
  }
  void push_back( const SiModule& module ) { _modules.insert(module); }

  const bool has_module( const int& barrel_ec , const int& layer_or_disk ,
                   const unsigned int& phi_module , const int& eta_module ) {
    return( _modules.find( SiModule(barrel_ec,layer_or_disk,phi_module,eta_module) ) != _modules.end() );
  }
  const bool has_module( const int& barrel_ec , const int& layer_or_disk ,
                   const unsigned int& phi_module , const int& eta_module ,
                   const SideType& side ) {
    return( _modules.find( SiModule(barrel_ec,layer_or_disk,phi_module,eta_module,side) ) != _modules.end() );
  }
  
  const_iterator begin() const { return _modules.begin(); }
  const_iterator end() const { return _modules.end(); }

  std::ostream& print( std::ostream& os ) const {
    BOOST_FOREACH( const SiModule& module , _modules ) {
      module.print( os );
    }
  }
  std::ostream& print() const {
    return print( std::cout );
  }

};

BOOST_CLASS_VERSION( SiModuleList::SiModule , 1 );
BOOST_CLASS_VERSION( SiModuleList , 1 );


#endif // WRAP_SiModuleList_HPP
