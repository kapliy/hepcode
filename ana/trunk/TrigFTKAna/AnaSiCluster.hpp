#ifndef WRAP_ANASICLUSTER_HPP
#define WRAP_ANASICLUSTER_HPP

// ANASICLUSTER
// ================================================================
// Data for a silicon cluster
// ================================================================
// 2010-02-17 Antonio Boveia (boveia@hep.uchicago.edu)

#include <numeric>
#include <cmath>
#include <boost/optional.hpp>
#include <boost/format.hpp>
#include <iostream>
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/version.hpp>
#include <TLorentzVector.h>
#include "TrigFTKAna/Detector.hpp"
#include "TrigFTKAna/UniqueBarcode.hpp"
#include "TrigFTKAna/AnaSiChannel.hpp"

class
AnaSiCluster
{

public:

  typedef AnaSiChannel::ftype ftype;
  typedef AnaSiChannel::ChannelType ClusterType;
  typedef AnaSiChannel::SideType SideType;
  typedef enum { UNKNOWN , OFFLINE , FTK } ClusterAlgorithm;

protected:

  ClusterType _cluster_type;
  ClusterAlgorithm _algorithm;

  ftype _x;
  ftype _y;
  ftype _z;
  
  int _barrel_ec;
  int _layer_or_disk;
  int _phi_module;
  int _eta_module;
  int _ftk_logical_layer;
  int _phi_index; 
  int _eta_index; // side for SCT
  ftype _local_x;
  ftype _local_z;
  int _width_x;
  int _width_z;
  int _n_channels;

  UniqueBarcode _barcode;
  ftype _truth_pt; // max pt of contributing truth tracks
  detector::TruthParentBitmask _truth_parent_bitmask;

  friend class NtWrapper;
  friend class AnaEventMgr;

private:

  friend class boost::serialization::access;
  template<class archiveT>
  void serialize( archiveT& ar, const unsigned int version )
  {
    ar & boost::serialization::make_nvp("cluster_type",_cluster_type);
    ar & boost::serialization::make_nvp("algorithm",_algorithm);
    ar & boost::serialization::make_nvp("x",_x);
    ar & boost::serialization::make_nvp("y",_y);
    ar & boost::serialization::make_nvp("z",_z);
    ar & boost::serialization::make_nvp("barrel_ec",_barrel_ec);
    ar & boost::serialization::make_nvp("layer_or_disk",_layer_or_disk);
    ar & boost::serialization::make_nvp("phi_module",_phi_module);
    ar & boost::serialization::make_nvp("eta_module",_eta_module);
    ar & boost::serialization::make_nvp("ftk_logical_layer",_ftk_logical_layer);
    ar & boost::serialization::make_nvp("phi_index",_phi_index); 
    ar & boost::serialization::make_nvp("eta_index",_eta_index);
    ar & boost::serialization::make_nvp("local_x",_local_x);
    ar & boost::serialization::make_nvp("local_z",_local_z);
    ar & boost::serialization::make_nvp("width_x",_width_x);
    ar & boost::serialization::make_nvp("width_z",_width_z);
    ar & boost::serialization::make_nvp("n_channels",_n_channels);
    ar & boost::serialization::make_nvp("barcode",_barcode);
    ar & boost::serialization::make_nvp("truth_pt",_truth_pt);
    ar & boost::serialization::make_nvp("truth_parent_bitmask",_truth_parent_bitmask);
  }

public:

  AnaSiCluster()
    : _cluster_type( AnaSiChannel::UNKNOWN_CHANNEL )
    , _algorithm( UNKNOWN )
    , _x( std::numeric_limits<ftype>::max() )
    , _y( std::numeric_limits<ftype>::max() )
    , _z( std::numeric_limits<ftype>::max() )
    , _barrel_ec( std::numeric_limits<int>::max() )
    , _layer_or_disk( std::numeric_limits<int>::max() )
    , _phi_module( std::numeric_limits<int>::max() )
    , _eta_module( std::numeric_limits<int>::max() )
    , _ftk_logical_layer( -1 )
    , _phi_index( std::numeric_limits<int>::max() )
    , _eta_index( std::numeric_limits<int>::max() )
    , _local_x( std::numeric_limits<ftype>::max() )
    , _local_z( std::numeric_limits<ftype>::max() )
    , _width_x( 0 )
    , _width_z( 0 )
    , _n_channels( 0 )
    , _barcode()
    , _truth_pt( 0. )
    , _truth_parent_bitmask()
  {}
  virtual ~AnaSiCluster() {}

  const ClusterAlgorithm& algorithm() const { return _algorithm; }
  const ClusterType& cluster_type() const { return _cluster_type; }
  const bool is_pixel() const { return _cluster_type==AnaSiChannel::PIXEL; }
  const bool is_sct() const { return _cluster_type==AnaSiChannel::STRIP; }
  const bool is_sct_phi() const { return ( _cluster_type==AnaSiChannel::STRIP && _eta_index==AnaSiChannel::RPHI ); }
  const bool is_sct_sas() const { return ( _cluster_type==AnaSiChannel::STRIP && _eta_index==AnaSiChannel::STEREO ); }

  const ftype& global_x() const { return _x; }
  const ftype& global_y() const { return _y; }
  const ftype& global_z() const { return _z; }
  
  const int& barrel_ec() const { return _barrel_ec; }
  const bool is_barrel() const { return _barrel_ec==0; }
  const bool is_endcap() const { return !is_barrel(); }
  const int& layer_or_disk() const { return _layer_or_disk; }
  const int& phi_module() const { return _phi_module; }
  const int& eta_module() const { return _eta_module; }
  const int& phi_index() const { return _phi_index; }
  const int& eta_index() const { assert(_cluster_type==AnaSiChannel::PIXEL ); return _eta_index; }
  const int& sct_side() const { assert(_cluster_type==AnaSiChannel::STRIP ); return _eta_index; }

  const int& ftk_logical_layer() const { return _ftk_logical_layer; }
                                                                                        

  const ftype& local_x() const { return _local_x; }
  const ftype& local_z() const { return _local_z; }
  const int& width_x() const { return _width_x; }
  const int& width_z() const { return _width_z; }
  const int& n_channels() const { return _n_channels; }

  const UniqueBarcode& barcode() const { return _barcode; }
  const ftype& max_truth_pt() const { return _truth_pt; }
  const bool from_truth_tau_decay() const { return detector::has_tau_parent(_truth_parent_bitmask); }
  const bool from_truth_b_decay() const { return detector::has_b_parent(_truth_parent_bitmask); }
  const bool from_truth_charged_pion_or_secondary() const { return detector::has_pion_parent(_truth_parent_bitmask); }
  const bool from_truth_charged_pion_or_immediate_secondary() const { return detector::has_pion_immediate_parent(_truth_parent_bitmask); }

  template<typename iterT> 
  boost::shared_ptr<const AnaSiCluster> find_matching_cluster( iterT begin , iterT end , ftype& diff ) const {
    boost::shared_ptr<const AnaSiCluster> match;
    ftype best_distance = 99999;
    for( iterT i=begin; i!=end; ++i ) { 
      const boost::shared_ptr<const AnaSiCluster>& cluster( *i );
      if( cluster->algorithm() != this->algorithm() ) { continue; }
      if( cluster->cluster_type() != this->cluster_type() ) { continue; }
      if( this->cluster_type() == AnaSiChannel::PIXEL ) { 
        if( cluster->ftk_logical_layer() != this->ftk_logical_layer() ) { continue; }
      } else if( this->cluster_type() == AnaSiChannel::STRIP ) {
        // if( cluster->ftk_logical_layer() != this->ftk_logical_layer() ) { continue; }
        // allow for logical layer to swap sides because of mismatch with ATLAS convention.
        // equivalent groupings: (3,4) (5,6) (7,8) (9,10)
        const int group_idA = (static_cast<int>((this->ftk_logical_layer()-1)/2))*2 + 1;
        const int group_idB = group_idA + 1;
        if( cluster->ftk_logical_layer() != group_idA && cluster->ftk_logical_layer() != group_idB ) { continue; }
      } else { 
        assert( !"unahndled silicon cluster type" );
      }
      const double diff_x = std::abs( cluster->local_x() - this->local_x() );
      const double diff_z = this->cluster_type()==AnaSiChannel::PIXEL ? std::abs( cluster->local_z() - this->local_z() ) : 0.;
      const double diff_r = detector::quadrature( diff_x, diff_z );
      if( best_distance <= diff_r ) { continue; }
      match = cluster;
      best_distance = diff_r;
    }
    if( match ) { diff = best_distance; }
    return match;
  }

  std::ostream& print( std::ostream& os = std::cout ) const {
    os << ( boost::format("d:%|1d| ll:%|2d| ly:%|2d| c:%|3d|,%|3d| lx:%|8.2g|,%|8.2g| t:%|3d|,%|6d|") 
            % (int)cluster_type() % ftk_logical_layer() % layer_or_disk()
            % phi_index() % (is_pixel() ? eta_index() : sct_side())
            % local_x() % (is_pixel() ? local_z() : sct_side())
            % barcode().event_index() % barcode().barcode()
            )
       << std::endl;
    return os;
  }

};

BOOST_CLASS_VERSION( AnaSiCluster , 1 );

#endif // WRAP_ANASICLUSTER_HPP
