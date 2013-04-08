#ifndef WRAP_ANASUPERSTRIP_HPP
#define WRAP_ANASUPERSTRIP_HPP

// ANASUPERSTRIP
// ================================================================
// Data for a single superstrip
// ================================================================
// 2009-05-21 Antonio Boveia (boveia@hep.uchicago.edu)

// Now that the TSP/DC options are implemented, the structure of
// AnaSuperstrip mimics that of FTKSS. In particular, if there are
// substrips, then only the bottom-most leaves have real information.

#include "TrigFTKAna_config.h"
#include <numeric>
#include <vector>
#include <cmath>
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/version.hpp>
#ifdef HAVE_LIBFTKSIM
#include "TrigFTKSim/MultiTruth.h"
#endif
#include "TrigFTKAna/Detector.hpp"
#include "TrigFTKAna/serialize_MultiTruth.hpp"

class
AnaSuperstrip
{
public:
  typedef float ftype;
  typedef long SuperstripID;
  typedef std::vector<ftype> Hit;
  typedef std::vector<Hit> HitVector;
  typedef std::vector<AnaSuperstrip> SubSSVector;
#ifdef HAVE_LIBFTKSIM
  typedef std::vector<MultiTruth> TruthVector;
#else
  typedef std::vector<int> TruthVector;
#endif
protected:
  unsigned int _layer;
  SuperstripID _superstrip_id;
  // hit position vector for each hit
  HitVector _coords;
  // hit truth for each hit
  TruthVector _truth;
  // Substrips
  SubSSVector _subss;
  friend class NtRoad;
private:
  friend class boost::serialization::access;
  template<class archiveT>
  void serialize( archiveT& ar, const unsigned int version )
  {
    ar & boost::serialization::make_nvp("layer",_layer);
    ar & boost::serialization::make_nvp("superstrip_id",_superstrip_id);
    ar & boost::serialization::make_nvp("coords",_coords);
    ar & boost::serialization::make_nvp("truth",_truth);
    ar & boost::serialization::make_nvp("subss",_subss);
  }
public:
  const unsigned int layer() const { return _layer; }
  const SuperstripID id() const { return _superstrip_id; }
  const unsigned long nhits() const { return _coords.size(); }
  const unsigned long nhits(const unsigned int& iss) const { return (iss<_subss.size()) ? _subss[iss].nhits() : nhits() ; }
  const HitVector& hits() const { return _coords; }
  const HitVector& hits(const unsigned int& iss) const { return (iss<_subss.size()) ? _subss[iss].hits() : hits() ; }
  const HitVector::const_iterator begin_hits() const { return _coords.begin(); }
  const HitVector::const_iterator begin_hits(const unsigned int& iss) const { return (iss<_subss.size()) ? _subss[iss].begin_hits() : begin_hits() ; }
  const HitVector::const_iterator end_hits() const { return _coords.end(); }  
  const HitVector::const_iterator end_hits(const unsigned int& iss) const { return (iss<_subss.size()) ? _subss[iss].end_hits() : end_hits() ; }
  const TruthVector& truth() const { return _truth; }
  const TruthVector& truth(const unsigned int& iss) const { return (iss<_subss.size()) ? _subss[iss].truth() : truth() ; }
  const TruthVector::const_iterator begin_truth() const { return _truth.begin(); }
  const TruthVector::const_iterator begin_truth(const unsigned int& iss) const { return (iss<_subss.size()) ? _subss[iss].begin_truth() : begin_truth(); }
  const TruthVector::const_iterator end_truth() const { return _truth.end(); }  
  const TruthVector::const_iterator end_truth(const unsigned int& iss) const { return (iss<_subss.size()) ? _subss[iss].end_truth() : end_truth(); }
  const unsigned int getNSubStrips() const { return _subss.size(); }
  const AnaSuperstrip& getSubStrip(unsigned int& iss) const { 
    if( iss >= _subss.size() ) {
      assert( false && "Tried to get non-existent sub-superstrip" );
    }
    return _subss[iss];
  }
  
};

BOOST_CLASS_VERSION( AnaSuperstrip , 2 );

#endif // WRAP_ANASUPERSTRIP_HPP
