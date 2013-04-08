 
#ifndef SERIALIZE_MULTITRUTH_HPP
#define SERIALIZE_MULTITRUTH_HPP

#ifdef HAVE_LIBFTKSIM

#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>
#include "TrigFTKSim/MultiTruth.h"

// TrigFTKSim MultiTruth data is private. I've fixed this
// with a recent commit (now it is protected). Meanwhile,
// get at that data this way.

class SerializeMultiTruth : public MultiTruth
{
public:
  typedef MultiTruth::TruthMap TruthMap;
private:
  // friend class boost::serialization::access;
  // template<class archiveT> friend void boost::serialization::serialize( archiveT& ar , MultiTruth& mt , const unsigned int version );
  // template<class archiveT>
  // void serialize_helper( archiveT& ar, const unsigned int version )
  // {
  //   ar & boost::serialization::make_nvp("truth_map",_truth);
  // }
public:
  SerializeMultiTruth( const MultiTruth& mt ) : MultiTruth(mt) {}
  TruthMap& truth_map() { return _truth; }
};

namespace boost {
  namespace serialization {
    template<class archiveT>
    void serialize( archiveT& ar , MultiTruth& mt , const unsigned int version ) {
      SerializeMultiTruth smt( mt );
      ar & boost::serialization::make_nvp("truth_map",smt.truth_map());
    }
  } // namespace serialization
} // namespace boost

#endif // HAVE_LIBFTKSIM

#endif // SERIALIZE_MULTITRUTH_HPP
