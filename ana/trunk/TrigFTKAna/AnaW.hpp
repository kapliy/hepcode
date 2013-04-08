#ifndef ANAW_HPP
#define ANAW_HPP

#include <cmath>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/version.hpp>
#include "TrigFTKAna/AnaComposite.hpp"

class AnaMET;
class AnaParticle;
class AnaJet;

class
AnaW : public AnaComposite
{
public:
  typedef enum { SEMILEPTONIC , HADRONIC } DecayType;
private:
  DecayType _decay_type;
  // precomputed shortcuts to decay products
  mutable boost::shared_ptr<const AnaParticle> _lepton;
  mutable boost::shared_ptr<const AnaMET> _met;
  mutable std::vector< boost::shared_ptr<const AnaJet> > _jets; // pt-ordered jets
  mutable bool _cached_decays;
private:
  friend class boost::serialization::access;
  template<class archiveT>
  void serialize( archiveT& ar, const unsigned int version ) {
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP( AnaComposite );
    ar & boost::serialization::make_nvp("decay_type",_decay_type);
    if( !_cached_decays ) { _classify(); }
  }
private:
  void _classify();
public:
  // general case
  AnaW() : _cached_decays(false), AnaComposite() {}
  AnaW( iterator begin , iterator end );
  AnaW( const_iterator begin , const_iterator end );
  AnaW( const boost::shared_ptr<const AnaParticle>& p1 , const boost::shared_ptr<const AnaParticle>& p2 );
  AnaW( const boost::shared_ptr<const AnaParticle>& p1 , const boost::shared_ptr<const AnaParticle>& p2 , 
        const boost::shared_ptr<const AnaParticle>& p3 );
  AnaW( const AnaW& rhs );

  virtual ~AnaW() {}

  const DecayType decay_type() const { return _decay_type; }

  const boost::shared_ptr<const AnaParticle>& lepton() const { assert( _cached_decays ); assert( _decay_type == SEMILEPTONIC ); assert( _lepton ); return _lepton; }
  const boost::shared_ptr<const AnaMET>& met() const { assert( _cached_decays ); assert( _decay_type == SEMILEPTONIC ); assert( _met ); return _met; }

  const boost::shared_ptr<const AnaJet>& ith_jet( const unsigned int& ijet ) const { // pt-ordered jets
    assert( _cached_decays ); assert( _decay_type == HADRONIC ); assert( ijet < _jets.size() );
    return _jets[ijet];
  }
  const boost::shared_ptr<const AnaJet>& leading_jet() const {  // highest pt jet
    assert( _cached_decays ); assert( _decay_type == HADRONIC ); assert( !_jets.empty() );
    return _jets.front();
  }
  const boost::shared_ptr<const AnaJet>& trailing_jet() const { // lowest pt jet
    assert( _cached_decays ); assert( _decay_type == HADRONIC ); assert( !_jets.empty() ); 
    return _jets.back();
  }
  const boost::shared_ptr<const AnaJet>& subleading_jet() const { // 2nd-highest pt jet
    assert( _cached_decays ); assert( _decay_type == HADRONIC ); assert( _jets.size()>1 );
    return( *(++_jets.begin()) );
  }

public:

  // commonly useful function objects
  // ================================================================

  // W composition
  class Compose {
  public:
    template<typename particleAT,typename particleBT>
    const boost::shared_ptr<const AnaW>
    operator()( const typename boost::shared_ptr<const particleAT>& particleA ,
                const typename boost::shared_ptr<const particleBT>& particleB ) const {
      if( !particleA || !particleB ) { return boost::shared_ptr<const AnaW>(); }
      return boost::make_shared<const AnaW>( particleA , particleB );
    }
  };

};

BOOST_CLASS_VERSION( AnaW , 1 );

#endif // ANAW_HPP

