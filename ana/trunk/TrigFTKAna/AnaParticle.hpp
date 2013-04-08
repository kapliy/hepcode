#ifndef WRAP_ANAPARTICLE_HPP
#define WRAP_ANAPARTICLE_HPP

#include <stdexcept>
#include <cmath>
#include <TLorentzVector.h>
#include <iostream>
#include <limits>
#include <boost/shared_ptr.hpp>
#include <boost/format.hpp>
#include <boost/function.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/version.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits.hpp>
#include "TrigFTKAna/Detector.hpp"
#include "TrigFTKAna/serialize_TLorentzVector.hpp"

class AnaTrack;
class AnaTruthParticle;

class
AnaParticle
{
public:
  typedef enum { GENERIC=0, ELECTRON , MUON , TAU , JET , MET , PHOTON , WBOSON , ZBOSON , HIGGS , TRUTH , RESERVED } Type;
  typedef float ftype;
private:
  Type _type;
protected:
  TLorentzVector _momentum;
  friend class NtD3PDphysWZ;
  friend class NtD3PDphysEW;
  friend class NtD3PDphysTop;
  friend class NtD3PDphysPeter;
  friend class NtD3PDphysJetMET;
  friend class AnaTruthTable;
  friend class NtCBNT;
  friend class NtJetTag;
  friend class NtPGS;
private:
  friend class boost::serialization::access;
  template<class archiveT>
  void serialize( archiveT& ar, const unsigned int version )
  {
    ar & boost::serialization::make_nvp("type",_type);
    ar & boost::serialization::make_nvp("momentum",_momentum);
  }
public:
  AnaParticle( const Type type = GENERIC ) 
    : _type( type )
    , _momentum( 0 , 0 , 0 , 0 )
  {}
  AnaParticle( const TLorentzVector& momentum , const Type type = GENERIC ) 
    : _type( type )
    , _momentum( momentum)
  {}
  virtual ~AnaParticle() {}

  // energy and mass
  const TLorentzVector& four_vector() const { return _momentum; }
  const TLorentzVector four_vector_MeV() const {
    return( TLorentzVector( _momentum.Px()*1000. , _momentum.Py()*1000. , _momentum.Pz()*1000. , _momentum.E()*1000. ) );
  }
  const TLorentzVector& momentum() const { return _momentum; }
  virtual const TLorentzVector& four_vector_for_composites() const { return _momentum; }
  const TLorentzVector transverse_four_vector() const { return( TLorentzVector( _momentum.Px() , _momentum.Py() , 0 , detector::quadrature(_momentum.Px(),_momentum.Py()) ) ); }
  const ftype energy() const { return _momentum.E(); }
  const ftype e() const { return _momentum.E(); }
  const ftype mass() const { return _momentum.M(); }
  const ftype m() const { return mass(); }
  virtual const ftype transverse_mass() const { 
    throw std::out_of_range("transverse mass not defined for AnaParticle");
    return 0; 
  }
  const ftype mt() const { return transverse_mass(); }
  const ftype mt_self() const { return _momentum.Mt(); }
  const ftype rapidity() const { return _momentum.Rapidity(); }
  const ftype pseudorapidity() const { return _momentum.PseudoRapidity(); }
  const ftype eta() const { return detector::etaFromTLorentzVector<ftype>(_momentum); }
  const ftype abseta() const { return std::abs(detector::etaFromTLorentzVector<ftype>(_momentum)); }
  const ftype theta() const { return _momentum.Theta(); }
  const ftype phi() const { return detector::canonical_phi(_momentum.Phi()); }
  const ftype p() const { return _momentum.P(); }
  const ftype pt() const { return _momentum.Pt(); }
  const ftype px() const { return _momentum.Px(); }
  const ftype py() const { return _momentum.Py(); }
  const ftype pz() const { return _momentum.Pz(); }
  const ftype et() const { return detector::ET<ftype>(_momentum); }
  const ftype lambda() const { return std::cos(_momentum.Theta()); }

  // other particle properties
  virtual const ftype charge() const { 
    throw std::out_of_range("charge not defined for AnaParticle");
    return 0.;
  }
  virtual const short scharge() const { 
    throw std::out_of_range("scharge not defined for AnaParticle");
    return 0;
  }

  // particle track. null if not meaningful (e.g. z boson or jet.)
  virtual const bool has_track() const { return false; }
  virtual boost::shared_ptr<const AnaTrack> track() const {
    throw std::out_of_range("track not defined by AnaParticle"); 
    return boost::shared_ptr<const AnaTrack>(); 
  }

  virtual const bool has_truth() const { return false; }
  const bool is_truth() const { return _type==TRUTH; }
  virtual const boost::shared_ptr<const AnaTruthParticle> truth() const { 
    throw std::out_of_range("truth not defined by AnaParticle"); 
    return boost::shared_ptr<const AnaTruthParticle>(); 
  }

 // "distance" between four vectors of this and other particle in eta-phi plane
  const ftype dist_eta( boost::shared_ptr<const AnaParticle> pother ) const { return detector::delta_eta( _momentum.Eta() , pother->_momentum.Eta() ); }
  const ftype dist_phi( boost::shared_ptr<const AnaParticle> pother ) const { return detector::delta_phi( _momentum.Phi() , pother->_momentum.Phi() ); }
  const ftype dist_eta_phi( boost::shared_ptr<const AnaParticle> pother ) const {   return detector::delta_eta_phi( _momentum.Eta() , _momentum.Phi() , 
                                                                                                   pother->_momentum.Eta() , pother->_momentum.Phi() ); }
  const ftype dist_eta( const ftype& eta ) const { return detector::delta_eta( static_cast<ftype>(_momentum.Eta()) , eta ); }
  const ftype dist_phi( const ftype& phi ) const { return detector::delta_phi( static_cast<ftype>(_momentum.Phi()) , phi ); }
  const ftype dist_eta_phi( const ftype& eta , const ftype& phi ) const { 
    return detector::delta_eta_phi( static_cast<ftype>(_momentum.Eta()) , static_cast<ftype>(_momentum.Phi()) , eta , phi ); 
  }
  // // is this particle within delta r < max_dist of the particles in
  // // the range begin to end?
  // template< typename iteratorT >
  // const bool within_dist_eta_phi( iteratorT begin , iteratorT end , const ftype max_dist ) const {
  //   if( begin==end ) { return false; }
  //   for( ; begin!=end; ++begin ) {
  //     if( dist_eta_phi( *begin ) < max_dist ) { return true; }
  //   }
  //   return false;
  // }
  // // does this particle have the smallest delta r of the particles in
  // // the range begin to end? if delta r > max_dist, return false
  // template< typename iteratorT >
  // const bool closest_within_dist_eta_phi( iteratorT begin , iteratorT end , const ftype max_dist = std::numeric_limits<ftype>::max() ) const {
  //   if( begin==end ) { return false; }
  //   ftype least_dr = std::numeric_limits<ftype>::max();
  //   for( ; begin!=end; ++begin ) {
  //     const ftype dr = dist_eta_phi( *begin );
  //     if( dr < least_dr ) { 
  //   }
  //   return false;
  // }

  virtual std::ostream& print( std::ostream& os = std::cout ) const {
    os << boost::format("AnaParticle: pt=%|+10|(GeV) eta=%|10| phi=%|10|") % et() % eta() % phi() << std::endl;
    return os;
  }

  const Type type() const { return _type; }
  virtual const bool is_charged_lepton() const {
    return( _type==ELECTRON || _type==MUON || _type==TAU );
  }

  void set_type( Type v ) { _type = v; }

public:

  // commonly useful function objects
  // ================================================================
  
  // select highest pt particle
  template<typename particleT>
  class HighestPt
  {
  public:
    typedef boost::shared_ptr<particleT> const& result_type;
    result_type operator()( result_type a , result_type b ) const {
      return( a->pt()>b->pt() ? a : b );
    }
  };

  // select lowest pt particle
  template<typename particleT>
  class LowestPt
  {
  public:
    typedef boost::shared_ptr<particleT> const& result_type;
    result_type operator()( result_type a , result_type b ) const {
      return( a->pt()<b->pt() ? a : b );
    }
  };

  // select lowest |eta| particle
  template<typename particleT>
  class LowestAbsEta
  {
  public:
    typedef boost::shared_ptr<particleT> const& result_type;
    result_type operator()( result_type a , result_type b ) const {
      return( std::abs(a->eta())<std::abs(b->eta()) ? a : b );
    }
  };

  // select highest |eta| particle
  template<typename particleT>
  class HighestAbsEta
  {
  public:
    typedef boost::shared_ptr<particleT> const& result_type;
    result_type operator()( result_type a , result_type b ) const {
      return( std::abs(a->eta())>std::abs(b->eta()) ? a : b );
    }
  };

  // smallest delta |phi| particle
    template<typename particleT>
  class SmallestDeltaPhi
  {
  private:
    AnaParticle::ftype _ref_phi;
  public:
    SmallestDeltaPhi( const AnaParticle::ftype& ref_phi ) : _ref_phi(ref_phi) {}
    typedef boost::shared_ptr<particleT> const& result_type;
    result_type operator()( result_type a , result_type b ) const {
      return( std::abs( detector::delta_phi(a->phi(),_ref_phi) ) < std::abs( detector::delta_phi(b->phi(),_ref_phi) ) ? a : b );
    }
  };

  // largest delta |phi| particle
  template<typename particleT>
  class LargestDeltaPhi
  {
  private:
    AnaParticle::ftype _ref_phi;
  public:
    LargestDeltaPhi( const AnaParticle::ftype& ref_phi ) : _ref_phi(ref_phi) {}
    typedef boost::shared_ptr<particleT> const& result_type;
    result_type operator()( result_type a , result_type b ) const {
      return( std::abs( detector::delta_phi(a->phi(),_ref_phi) ) > std::abs( detector::delta_phi(b->phi(),_ref_phi) ) ? a : b );
    }
  };

  // smallest delta r particle
  template<typename particleT>
  class SmallestDeltaEtaPhi
  {
  private:
    AnaParticle::ftype _ref_eta;
    AnaParticle::ftype _ref_phi;
  public:
    SmallestDeltaEtaPhi( const AnaParticle::ftype& ref_eta , const AnaParticle::ftype& ref_phi ) 
      : _ref_eta(ref_eta)
      , _ref_phi(ref_phi) 
    {}
    typedef boost::shared_ptr<particleT> const& result_type;
    result_type operator()( result_type a , result_type b ) const {
      return( detector::delta_eta_phi(a->eta(),a->phi(),_ref_eta,_ref_phi) < 
              detector::delta_eta_phi(b->eta(),b->phi(),_ref_eta,_ref_phi) ? a : b );
    }
  };

  // less than operator for delta-eta-phi comparisons between particles
  class DeltaEtaPhiLessThan
  {
  private:
    AnaParticle::ftype _ref_eta;
    AnaParticle::ftype _ref_phi;
  public:
    DeltaEtaPhiLessThan( const AnaParticle::ftype& ref_eta , const AnaParticle::ftype& ref_phi ) 
      : _ref_eta(ref_eta)
      , _ref_phi(ref_phi) 
    {}
    typedef const bool result_type;
    template<typename particlePtr>
    result_type operator()( particlePtr a , particlePtr b ) const {
      return( detector::delta_eta_phi(a->eta(),a->phi(),_ref_eta,_ref_phi) < 
              detector::delta_eta_phi(b->eta(),b->phi(),_ref_eta,_ref_phi) );
    }
  };

  // largest delta r particle
  template<typename particleT>
  class LargestDeltaEtaPhi
  {
  private:
    AnaParticle::ftype _ref_eta;
    AnaParticle::ftype _ref_phi;
  public:
    LargestDeltaEtaPhi( const AnaParticle::ftype& ref_eta , const AnaParticle::ftype& ref_phi ) 
      : _ref_eta(ref_eta)
      , _ref_phi(ref_phi) 
    {}
    typedef boost::shared_ptr<particleT> const& result_type;
    result_type operator()( result_type a , result_type b ) const {
      return( detector::delta_eta_phi(a->eta(),a->phi(),_ref_eta,_ref_phi) > 
              detector::delta_eta_phi(b->eta(),b->phi(),_ref_eta,_ref_phi) ? a : b );
    }
  };

  // select object closer to W transverse mass
  template<typename particleT>
  class ClosestTransverseMassToW
  {
  public:
    typedef boost::shared_ptr<particleT> const& result_type;
    result_type operator()( result_type a , result_type b ) const {
      return( ( std::abs(a->transverse_mass()-detector::W_mass(detector::GeV)) < std::abs(b->transverse_mass()-detector::W_mass(detector::GeV)) ) ? a : b );
    }
  };

  template<typename particleT,typename floatT>
  class ClosestMassToValue
  {
  protected:
    floatT _value;
  public:
    ClosestMassToValue( const floatT& value ) : _value(value) {}
    virtual ~ClosestMassToValue() {}
    typedef boost::shared_ptr<particleT> const& result_type;
    result_type operator()( boost::shared_ptr<particleT> const& a , boost::shared_ptr<particleT> const& b ) const {
      BOOST_STATIC_ASSERT((boost::is_convertible<typename particleT::ftype,const floatT>::value));
      return( ( std::abs(a->mass()-_value) < std::abs(b->mass()-_value) ) ? a : b );
    }
  };

  template<typename particleT,typename floatT>
  class ClosestMtSelfToValue
  {
  protected:
    floatT _value;
  public:
    ClosestMtSelfToValue( const floatT& value ) : _value(value) {}
    virtual ~ClosestMtSelfToValue() {}
    typedef boost::shared_ptr<particleT> const& result_type;
    result_type operator()( boost::shared_ptr<particleT> const& a , boost::shared_ptr<particleT> const& b ) const {
      BOOST_STATIC_ASSERT((boost::is_convertible<typename particleT::ftype,const floatT>::value));
      return( ( std::abs(a->mt_self()-_value) < std::abs(b->mt_self()-_value) ) ? a : b );
    }
  };
  
  // FIXME: update ClosestMassTo* functions below with call to ClosestMassToValue
  
  // select object closer to W mass
  template<typename particleT>
  class ClosestMassToW
  {
  public:
    typedef boost::shared_ptr<particleT> const& result_type;
    result_type operator()( boost::shared_ptr<particleT> const& a , boost::shared_ptr<particleT> const& b ) const {
      return( ( std::abs(a->mass()-detector::W_mass(detector::GeV)) < std::abs(b->mass()-detector::W_mass(detector::GeV)) ) ? a : b );
    }
  };
  template<typename particleT,typename argumentT>
  class ClosestMassToWof
  {
  public:
    typedef typename boost::shared_ptr<particleT> const& result_type;
    typedef boost::function< boost::shared_ptr<argumentT> ( result_type ) > operator_type;
  private:
    operator_type _op;
  public:
    ClosestMassToWof( operator_type op ) : _op(op) {}
    result_type operator()( result_type a , result_type b ) const {
      return( ( std::abs(_op(a)->mass()-detector::W_mass(detector::GeV)) < std::abs(_op(b)->mass()-detector::W_mass(detector::GeV)) ) ? a : b );
    }
  };

  // select object closer to Z mass
  template<typename particleT>
  class ClosestMassToZ
  {
  public:
    typedef boost::shared_ptr<particleT> const& result_type;
    result_type operator()( boost::shared_ptr<particleT> const& a , boost::shared_ptr<particleT> const& b ) const {
      return( ( std::abs(a->mass()-detector::Z_mass(detector::GeV)) < std::abs(b->mass()-detector::Z_mass(detector::GeV)) ) ? a : b );
    }
  };

  // select object closer to upsilon mass
  template<typename particleT>
  class ClosestMassToUpsilon1S
  {
  public:
    typedef boost::shared_ptr<particleT> const& result_type;
    result_type operator()( boost::shared_ptr<particleT> const& a , boost::shared_ptr<particleT> const& b ) const {
      return( ( std::abs(a->mass()-detector::upsilon_1s_mass(detector::GeV)) < std::abs(b->mass()-detector::upsilon_1s_mass(detector::GeV)) ) ? a : b );
    }
  };

  // select object closer to upsilon mass
  template<typename particleT>
  class ClosestMassToJPsi
  {
  public:
    typedef boost::shared_ptr<particleT> const& result_type;
    result_type operator()( boost::shared_ptr<particleT> const& a , boost::shared_ptr<particleT> const& b ) const {
      return( ( std::abs(a->mass()-detector::jpsi_mass(detector::GeV)) < std::abs(b->mass()-detector::jpsi_mass(detector::GeV)) ) ? a : b );
    }
  };
  
  // is object inside the given mass window?
  template<typename particleT>
  class MassInWindow
  {
  private:
    const AnaParticle::ftype _min_mass;
    const AnaParticle::ftype _max_mass;
  public:
    MassInWindow( const AnaParticle::ftype& min_mass , const AnaParticle::ftype& max_mass ) 
      : _min_mass(min_mass)
      , _max_mass(max_mass)
    {}
    typedef bool result_type;
    const result_type operator()( boost::shared_ptr<particleT> const& p ) const {
      return( (p->mass() >= _min_mass) && p->mass() <= _max_mass );
    }
  };


};

BOOST_CLASS_VERSION( AnaParticle , 1 );

#endif // WRAP_ANAPARTICLE_HPP
