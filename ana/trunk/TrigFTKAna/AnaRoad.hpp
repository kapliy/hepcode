#ifndef WRAP_ANAROAD_HPP
#define WRAP_ANAROAD_HPP

// ANAROAD
// ================================================================
// Data for a single road
// ================================================================
// 2009-05-21 Antonio Boveia (boveia@hep.uchicago.edu)

#include "TrigFTKAna_config.h"
#include <numeric>
#include <cmath>
#include <vector>
#include <iostream>
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>
#ifdef HAVE_LIBFTKSIM
#include "TrigFTKSim/MultiTruth.h"
#endif
#include "TrigFTKAna/serialize_MultiTruth.hpp"
#include <boost/serialization/version.hpp>
#include <boost/dynamic_bitset.hpp>
#include "TrigFTKAna/Detector.hpp"
#include "TrigFTKAna/serialize_dynamic_bitset.hpp"

class
AnaRoad
{
public:
  typedef float ftype;
  typedef int BankID;
  typedef unsigned long RoadID;
#ifdef HAVE_LIBFTKSIM
  typedef std::vector<MultiTruth> TruthVector;
#else
  typedef std::vector<int> TruthVector;
#endif
protected:
  RoadID _road_id;
  BankID _bank_id;
  unsigned long _pattern_id;
  unsigned long _sector_id;
  unsigned int _n_planes;
  bool _rw_rejected;
  unsigned int _rw_bank_id;
  unsigned long _rw_road_id;
  unsigned int _n_hits;
  unsigned long _bitmask;
  // superstrip id for each plane
  std::vector<long> _superstrip_id;
  // vector of coordinates for each hit for each plane
  std::vector< std::vector< std::vector<ftype> > > _coords;
  std::vector<boost::dynamic_bitset<> > _m_hits_mask; // HF bitmask, divided by plane
  // hit truth for each hit
  std::vector< TruthVector > _truth; // One entry per plane per _coord
  // _coords[plane].size() should be the same as _truth[plane].size()
  friend class NtRoad;
private:
  friend class boost::serialization::access;
  template<class archiveT>
  void serialize( archiveT& ar, const unsigned int version )
  {
    ar & boost::serialization::make_nvp("road_id",_road_id);
    ar & boost::serialization::make_nvp("bank_id",_bank_id);
    ar & boost::serialization::make_nvp("pattern_id",_pattern_id);
    ar & boost::serialization::make_nvp("sector_id",_sector_id);
    ar & boost::serialization::make_nvp("n_planes",_n_planes);
    ar & boost::serialization::make_nvp("rw_rejected",_rw_rejected);
    ar & boost::serialization::make_nvp("rw_bank_id",_rw_bank_id);
    ar & boost::serialization::make_nvp("rw_road_id",_rw_road_id);
    ar & boost::serialization::make_nvp("n_hits",_n_hits);
    ar & boost::serialization::make_nvp("bitmask",_bitmask);
    ar & boost::serialization::make_nvp("superstrip_id",_superstrip_id);
    ar & boost::serialization::make_nvp("coords",_coords);
    ar & boost::serialization::make_nvp("hits_mask",_m_hits_mask);
    ar & boost::serialization::make_nvp("truth",_truth);
  }
public:
  AnaRoad() 
    : _road_id(std::numeric_limits<RoadID>::max())
    , _bank_id(std::numeric_limits<BankID>::max())
    , _pattern_id(std::numeric_limits<unsigned long>::max())
    , _sector_id(std::numeric_limits<unsigned long>::max())
    , _n_planes(0u)
    , _rw_rejected(false)
    , _rw_bank_id(std::numeric_limits<unsigned int>::max())
    , _rw_road_id(std::numeric_limits<unsigned long>::max())
    , _n_hits(0u)
    , _bitmask(0ul)
    , _superstrip_id()
    , _coords()
    , _m_hits_mask()
    , _truth()
  {}
  virtual ~AnaRoad() {}

  const RoadID road_id() const { return _road_id; }
  const BankID bank_id() const { return _bank_id; }
  const BankID bank() const { return _bank_id % 100; }
  const unsigned long pattern_id() const { return _pattern_id; }
  const unsigned long sector_id() const { return _sector_id; }
  const unsigned int n_planes() const { return _n_planes; }
  const bool rw_rejected() const { return _rw_rejected; }
  const unsigned int rw_bank_id() const { return _rw_bank_id; }
  const unsigned long rw_road_id() const { return _rw_road_id; }
  const unsigned int n_hits() const { return _n_hits; }
  const unsigned long bitmask() const { return _bitmask; }
  const unsigned long n_comb() const { 
    unsigned long combs(1);
    for(unsigned int i=0; i < _n_planes; i++) {
      combs *= n_hits(i);
    }
    return combs;
  }
  // superstrip id for each plane
  const std::vector<long>& superstrip_ids() const { return _superstrip_id; }
  const long superstrip_id(const unsigned int& plane) const { assert(plane<=_superstrip_id.size()); return _superstrip_id[plane]; }
  // vector of coordinates for each hit for each plane
  const std::vector< std::vector< std::vector<ftype> > >& hits() const { return _coords; }
  const std::vector< std::vector<ftype> >& hits(const unsigned int& plane) const { assert(plane<=_coords.size()); return _coords[plane]; }
  const unsigned long n_hits( const unsigned int& plane ) const { return hits(plane).size(); }
  const std::vector<ftype>& hit( const unsigned int& plane , const unsigned long& ihit ) const { return( hits(plane)[ihit] ); }
  const std::vector< TruthVector >& truth() const { return _truth; }
  const TruthVector& truth(const unsigned int& plane) const { assert(plane<=_truth.size()); return _truth[plane]; }

  bool hit_failed_HF(int pl,int idx) const {
    if(_m_hits_mask.empty()) return false; 
    return _m_hits_mask[pl].test(idx);
  };
  const std::vector<boost::dynamic_bitset<> >& getHitsMask() const {return _m_hits_mask;}
  const boost::dynamic_bitset<> & getHitsMaskByPlane(int pl) const {return _m_hits_mask[pl];}

  void print() const {
    std::cout << "AnaRoad " << std::hex << this << " road id " << std::dec 
	      << _road_id << " bank id " << _bank_id << " _pattern_id "
	      << _pattern_id << " sector id " << _sector_id << " _n_hits "
	      << _n_hits << " _bitmask " << std::hex << _bitmask << std::dec
	      << " Number of hits by layer:" << std::endl
	      << "\t";
    for(int i = 0; i < _n_planes; i++) {
      std::cout << " " << _coords[i].size();
    }
    std::cout << std::endl;
  }
};

BOOST_CLASS_VERSION( AnaRoad , 2 );

#endif // WRAP_ANAROAD_HPP
