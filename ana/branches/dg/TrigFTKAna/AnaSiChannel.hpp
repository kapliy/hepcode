#ifndef WRAP_ANASICHANNEL_HPP
#define WRAP_ANASICHANNEL_HPP

// ANASICHANNEL
// ================================================================
// Data for a single occupied silicon channel (strip or pixel).
// ================================================================
// 2009-05-21 Antonio Boveia (boveia@hep.uchicago.edu)

#include <numeric>
#include <cmath>
#include <iostream>
#include <boost/optional.hpp>
#include <boost/format.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/version.hpp>
#include <TLorentzVector.h>
#include "TrigFTKAna/Detector.hpp"
#include "TrigFTKAna/UniqueBarcode.hpp"

class
AnaSiChannel
{

public:

  typedef float ftype;
  typedef enum { UNKNOWN_CHANNEL=0 , PIXEL=1 , STRIP=2 } ChannelType;
  typedef enum { UNKNOWN_SIDE=0 , RPHI=1 , STEREO=2 } SideType;

protected:

  ChannelType _channel_type;

  ftype _x;
  ftype _y;
  ftype _z;
  
  int _barrel_ec;
  int _layer_or_disk;
  unsigned int _phi_module;
  int _eta_module;

  boost::optional<unsigned int> _pixel_phi_index; // valid only for pixels
  boost::optional<unsigned int> _pixel_eta_index; // valid only for pixels
  boost::optional<unsigned int> _pixel_tot; // valid only for pixels
  
  boost::optional<SideType> _strip_side; // valid only for strips
  boost::optional<unsigned int> _strip_index; // valid only for strips
  boost::optional<unsigned int> _strip_group_size; // valid only for strips

  // best (currently highest pt) truth barcode for this channel
  UniqueBarcode _barcode; 
  int _highest_pt;
  // bitmask describing various contributions to this channel
  detector::TruthParentBitmask _truth_parent_bitmask;
  
  friend class NtWrapper;

  friend class boost::serialization::access;
  template<class archiveT>
  void serialize( archiveT& ar, const unsigned int version ) {
    ar & boost::serialization::make_nvp("channel_type",_channel_type);
    ar & boost::serialization::make_nvp("x",_x);
    ar & boost::serialization::make_nvp("y",_y);
    ar & boost::serialization::make_nvp("z",_z);
    ar & boost::serialization::make_nvp("barrel_ec",_barrel_ec);
    ar & boost::serialization::make_nvp("layer_or_disk",_layer_or_disk);
    ar & boost::serialization::make_nvp("phi_module",_phi_module);
    ar & boost::serialization::make_nvp("eta_module",_eta_module);
    ar & boost::serialization::make_nvp("pixel_phi_index",_pixel_phi_index);
    ar & boost::serialization::make_nvp("pixel_eta_index",_pixel_eta_index);
    ar & boost::serialization::make_nvp("pixel_tot",_pixel_tot);
    ar & boost::serialization::make_nvp("strip_side",_strip_side); 
    ar & boost::serialization::make_nvp("strip_index",_strip_index);
    ar & boost::serialization::make_nvp("strip_group_size",_strip_group_size);
    ar & boost::serialization::make_nvp("barcode",_barcode); 
    ar & boost::serialization::make_nvp("highest_pt",_highest_pt);
    ar & boost::serialization::make_nvp("truth_parent_bitmask",_truth_parent_bitmask);
  }
public:

  AnaSiChannel()
    : _channel_type( UNKNOWN_CHANNEL )
    , _x( std::numeric_limits<ftype>::max() )
    , _y( std::numeric_limits<ftype>::max() )
    , _z( std::numeric_limits<ftype>::max() )
    , _barrel_ec( std::numeric_limits<int>::max() )
    , _layer_or_disk( std::numeric_limits<int>::max() )
    , _phi_module( std::numeric_limits<unsigned int>::max() )
    , _eta_module( std::numeric_limits<int>::max() )
    , _pixel_phi_index()
    , _pixel_eta_index()
    , _pixel_tot()
    , _strip_side()
    , _strip_index()
    , _strip_group_size()
    , _barcode()
    , _highest_pt(0)
    , _truth_parent_bitmask()
  {}
  AnaSiChannel( const ChannelType& channel , const ftype& x , const ftype& y , const ftype& z , const int& barrel_ec ,
                const int& layer_or_disk , const unsigned int& phi_module , const int& eta_module , 
                const unsigned int& phi_index , const unsigned int& eta_index , const unsigned int& tot , 
                const UniqueBarcode& barcode , const detector::TruthParentBitmask& mask )
    : _channel_type( channel )
    , _x( x )
    , _y( y )
    , _z( z )
    , _barrel_ec( barrel_ec )
    , _layer_or_disk( layer_or_disk )
    , _phi_module( phi_module )
    , _eta_module( eta_module )
    , _pixel_phi_index( phi_index )
    , _pixel_eta_index( eta_index )
    , _pixel_tot( tot )
    , _strip_side()
    , _strip_index()
    , _strip_group_size()
    , _barcode( barcode )
    , _highest_pt(0)
    , _truth_parent_bitmask( mask )
  { assert( channel == PIXEL ); }
  AnaSiChannel( const ChannelType& channel , const ftype& x , const ftype& y , const ftype& z , const int& barrel_ec ,
                const int& layer_or_disk , const unsigned int& phi_module , const int& eta_module , 
                const SideType& side , const unsigned int& strip_index , const unsigned int& group_size , 
                const UniqueBarcode& barcode , const detector::TruthParentBitmask& mask )
    : _channel_type( channel )
    , _x( x )
    , _y( y )
    , _z( z )
    , _barrel_ec( barrel_ec )
    , _layer_or_disk( layer_or_disk )
    , _phi_module( phi_module )
    , _eta_module( eta_module )
    , _pixel_phi_index()
    , _pixel_eta_index()
    , _pixel_tot()
    , _strip_side( side )
    , _strip_index( strip_index )
    , _strip_group_size( group_size )
    , _barcode( barcode )
    , _highest_pt(0)
    , _truth_parent_bitmask( mask )
  { assert( channel == STRIP ); }
  virtual ~AnaSiChannel() {}

  const ChannelType& channel_type() const { return _channel_type; }

  const ftype& global_x() const { return _x; }
  const ftype& global_y() const { return _y; }
  const ftype& global_z() const { return _z; }
  
  const int& barrel_ec() const { return _barrel_ec; }
  const bool is_barrel() const { return _barrel_ec==0; }
  const bool is_endcap() const { return !is_barrel(); }
  const int& layer_or_disk() const { return _layer_or_disk; }
  const unsigned int& phi_module() const { return _phi_module; }
  const int& eta_module() const { return _eta_module; }

  const unsigned int& phi_index() const { assert(_channel_type==PIXEL && _pixel_phi_index); return *_pixel_phi_index; }
  const unsigned int& eta_index() const { assert(_channel_type==PIXEL && _pixel_eta_index); return *_pixel_eta_index; }
  const unsigned int& tot() const { assert(_channel_type==PIXEL && _pixel_tot); return *_pixel_tot; }
  const ftype tot_corr() const { 
    if( _channel_type!=PIXEL || !_pixel_tot ) { return 0.; }
    if( _barrel_ec == 0 ) {
      // barrel; corrected is tot*sin(theta)
      return( (*_pixel_tot) * std::abs( std::sin( atan2( detector::quadrature(global_x(),global_y()) , global_z() ) ) ) );
    } else {
      // endcap: corrected is tot*cos(theta)
      return( (*_pixel_tot) * std::abs( std::cos( atan2( detector::quadrature(global_x(),global_y()) , global_z() ) ) ) );
    }
  }

  const unsigned int side() const { assert(_channel_type==STRIP && _strip_side); return *_strip_side; }
  const unsigned int& strip_index() const { assert(_channel_type==STRIP && _strip_index); return *_strip_index; }
  const unsigned int& group_size() const { assert(_channel_type==STRIP && _strip_group_size); return *_strip_group_size; }

  const int ftk_logical_layer() const { return detector::ftk_physical_to_logical_layer( _channel_type==PIXEL ? detector::PIXEL_DETECTOR : detector::SCT_DETECTOR ,
                                                                                        _barrel_ec , _layer_or_disk , _eta_module , 0 ); }

  const int ftk_layerid() const { return detector::ftk_get_layer_or_disk( _channel_type==PIXEL ? detector::PIXEL_DETECTOR : detector::SCT_DETECTOR ,
									  _barrel_ec , _layer_or_disk , _eta_module , _channel_type==STRIP ? side()-1 : 0  ); }

  const UniqueBarcode& barcode() const { return _barcode; }
  const ftype highest_pt() const { return static_cast<ftype>(_highest_pt)/1000; }
  void highest_pt( const int& pt ) { _highest_pt = pt; }
  const bool from_truth_tau_decay() const { return detector::has_tau_parent(_truth_parent_bitmask); }
  const bool from_truth_b_decay() const { return detector::has_b_parent(_truth_parent_bitmask); }
  const bool from_truth_charged_pion_or_secondary() const { return detector::has_pion_parent(_truth_parent_bitmask); }
  const bool from_truth_charged_pion_or_immediate_secondary() const { return detector::has_pion_immediate_parent(_truth_parent_bitmask); }

  std::ostream& print( std::ostream& os ) const {
    if( _channel_type == PIXEL ) {
      os << boost::format("S %|15| %|15| %|15| %|10| %|10| %|10| %|10| %|10| %|10| %|10| %|10| %|10| %|10| %|10|")
	% _x % _y % _z % 1 % _barrel_ec % _layer_or_disk % _phi_module % _eta_module
	% *_pixel_phi_index % *_pixel_eta_index % *_pixel_tot % _barcode.event_index()
	% _barcode.barcode() % _truth_parent_bitmask.to_ulong()
	 << std::endl;
    }
    else {
      os << boost::format("S %|15| %|15| %|15| %|10| %|10| %|10| %|10| %|10| %|10| %|10| %|10| %|10| %|10| %|10|")
	% _x % _y % _z % 0 % _barrel_ec % _layer_or_disk % _phi_module % _eta_module
	% (*_strip_side == AnaSiChannel::STEREO ? 1 : 0) % *_strip_index % *_strip_group_size % _barcode.event_index()
	% _barcode.barcode() % _truth_parent_bitmask.to_ulong()
	 << std::endl;
    }
    return os;
  }
  std::ostream& print() const {
    return print( std::cout );
  }

};

BOOST_CLASS_VERSION( AnaSiChannel , 1 );

#endif // WRAP_ANASICHANNEL_HPP
