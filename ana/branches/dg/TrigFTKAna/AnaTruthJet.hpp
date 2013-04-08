#ifndef ANATRUTHJET_HPP
#define ANATRUTHJET_HPP

#include <limits>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/version.hpp>
#include "TrigFTKAna/AnaTruthParticle.hpp"

class
AnaTruthJet : public AnaTruthParticle
{
private:
  typedef AnaParticle::ftype ftype;
  int _label;
  ftype _min_delta_r_b;
  ftype _min_delta_r_c;
  ftype _min_delta_r_t;
  friend class AnaTruthTable;
  friend class NtD3PDphysTop;
private:
  friend class boost::serialization::access;
  template<class archiveT>
  void serialize( archiveT& ar, const unsigned int version ) {
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP( AnaTruthParticle );
    ar & boost::serialization::make_nvp("label",_label);
    ar & boost::serialization::make_nvp("min_delta_r_b",_min_delta_r_b);
    ar & boost::serialization::make_nvp("min_delta_r_c",_min_delta_r_c);
    ar & boost::serialization::make_nvp("min_delta_r_t",_min_delta_r_t);
  }
public:
  AnaTruthJet() 
    : _label( 0 )
    , _min_delta_r_b( std::numeric_limits<ftype>::max() )
    , _min_delta_r_c( std::numeric_limits<ftype>::max() )
    , _min_delta_r_t( std::numeric_limits<ftype>::max() )
    , AnaTruthParticle()
  {}
  virtual ~AnaTruthJet() {}

  const int label() const { return _label; }
  const ftype min_delta_r_b() const { return _min_delta_r_b; }
  const ftype min_delta_r_c() const { return _min_delta_r_c; }
  const ftype min_delta_r_t() const { return _min_delta_r_t; }

  virtual std::ostream& print( std::ostream& os ) const {
    os << "AnaTruthJet: [barcode " << index()
       << "] [pdg " << pdg_name()
       << "] [pt " << pt()
       << "] [eta " << eta() 
       << "] [phi " << phi() 
       << "] [mass " << mass() 
       << "] [label " << label()
       << "] [drb " << min_delta_r_b()
       << "] [drc " << min_delta_r_c()
       << "] [drt " << min_delta_r_t()
       << "]" << std::endl;
    return os;
  }

};

BOOST_CLASS_VERSION( AnaTruthJet , 1 );

#endif // ANATRUTHJET_HPP

