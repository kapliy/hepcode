#ifndef ANACOMPOSITE_HPP
#define ANACOMPOSITE_HPP

#include <cassert>
#include <list>
#include <algorithm>
#include <iterator>
#include <limits>
#include <cmath>
#include <stdexcept>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/optional.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/foreach.hpp>
#include <boost/any.hpp>
#include <boost/function.hpp>
#include <boost/function_types/function_arity.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/version.hpp>
#include "DataGraphics/DgCutSet.hh"
#include "TrigFTKAna/AnaParticle.hpp"

class
AnaComposite : public AnaParticle
{
public:
  typedef boost::shared_ptr<const AnaParticle> ptr_type;
  typedef std::list< ptr_type > Coll;
  typedef Coll::iterator iterator;
  typedef Coll::const_iterator const_iterator;
  typedef AnaParticle::ftype ftype;
private:
  Coll _constituents;
  boost::optional<ftype> _charge;
protected:
  boost::optional<AnaParticle::ftype> _transverse_mass;
private:
  friend class boost::serialization::access;
  template<class archiveT>
  void serialize( archiveT& ar, const unsigned int version ) {
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(AnaParticle);
    ar & boost::serialization::make_nvp("constituents",_constituents);
    ar & boost::serialization::make_nvp("charge",_charge);
    ar & boost::serialization::make_nvp("transverse_mass",_transverse_mass);
  }
private:
  void _precompute();
protected:
  void _compute_transverse_mass();
public:
  // general case
  AnaComposite() : AnaParticle() {}
  AnaComposite( iterator begin , iterator end , const Type type );
  AnaComposite( const_iterator begin , const_iterator end , const Type type );
  // special case: two particles
  AnaComposite( const ptr_type& p1 , const ptr_type& p2 , const Type type );
  // special case: three particles
  AnaComposite( const boost::shared_ptr<const AnaParticle>& p1 , const boost::shared_ptr<const AnaParticle>& p2 , const boost::shared_ptr<const AnaParticle>& p3 , const Type type );
  virtual ~AnaComposite() {}

  virtual const ftype charge() const { 
    if( _charge ) { // may not be defined if not defined for some constituents
      return *_charge;
    } else {
      throw std::out_of_range("charge not defined");
    }
    return 0; 
  }  
  virtual const ftype transverse_mass() const { 
    if( _transverse_mass ) { 
      return *_transverse_mass; 
    } else {
      throw std::out_of_range("transverse mass not defined");
    }
    return 0; 
  }

  const_iterator begin_constituent() const { return _constituents.begin(); }
  const_iterator end_constituent() const { return _constituents.end(); }
  const unsigned int num_constituents() const { return _constituents.size(); }
  const boost::shared_ptr<const AnaParticle> ith_constituent( const unsigned int& i ) const {
    assert( i < _constituents.size() );
    const_iterator ic = _constituents.begin();
    std::advance( ic , i );  assert( ic!=_constituents.end() );
    return( *ic );
  }

  template<typename T>
  const unsigned int num_constituents_type() const { 
    struct dynamic_cast_visitor { T operator()( T& t ) { return dynamic_cast<T>(t); }; };
    return( std::count_if( _constituents.begin() , _constituents.end() ,
                           dynamic_cast_visitor() ) );
  }

  const bool consists_of( const ptr_type& pA , const ptr_type& pB ) const {
    if( num_constituents() != 2 ) { return false; }
    assert( pA!=pB );
    return( (std::find( _constituents.begin() , _constituents.end() , pA ) != _constituents.end()) &&
            (std::find( _constituents.begin() , _constituents.end() , pB ) != _constituents.end()) );
  }

  const bool consists_of( const ptr_type& pA , const ptr_type& pB , const ptr_type& pC ) const {
    if( num_constituents() != 3 ) { return false; }
    assert( pA!=pB );
    assert( pA!=pC );
    assert( pB!=pC );
    return( (std::find( _constituents.begin() , _constituents.end() , pA ) != _constituents.end()) &&
            (std::find( _constituents.begin() , _constituents.end() , pB ) != _constituents.end()) &&
            (std::find( _constituents.begin() , _constituents.end() , pC ) != _constituents.end()) );
  }

  template< typename outputIteratorT >
  outputIteratorT final_states( outputIteratorT output_iterator ) const {
    // copy all final state particles to the output iterator. if
    // particle is composite, recursively call this routine to
    // identify the final state constituents.
    for( const_iterator i=_constituents.begin(), f=_constituents.end(); i!=f; ++i ) {
      const ptr_type p( *i );
      const boost::shared_ptr<const AnaComposite> cop( boost::dynamic_pointer_cast<const AnaComposite>(p) );
      if( cop ) {
        output_iterator = cop->final_states( output_iterator ); 
      } else { 
        (*output_iterator++) = p;
      }
    }
    return output_iterator;
  }

  // general n-particle case
  virtual const AnaParticle::ftype max_delta_phi_opening() const;
  virtual const AnaParticle::ftype min_delta_phi_opening() const;
  virtual const AnaParticle::ftype max_delta_eta_opening() const;
  virtual const AnaParticle::ftype min_delta_eta_opening() const;
  virtual const AnaParticle::ftype max_delta_r_opening() const;
  virtual const AnaParticle::ftype min_delta_r_opening() const;

  template< typename particlePtrT >
  const AnaParticle::ftype signed_min_amongst_delta_phi_wrt( particlePtrT& ref ) const {
    assert( !_constituents.empty() );
    AnaParticle::ftype result = std::numeric_limits<AnaParticle::ftype>::max();
    BOOST_FOREACH( const boost::shared_ptr<const AnaParticle>& p , _constituents ) {
      const AnaParticle::ftype this_result = p->dist_phi( ref );
      if( std::abs(this_result) < std::abs(result) ) {
        result = this_result;
      }
    }
    return result;
  }

  template< typename particlePtrT >
  const AnaParticle::ftype signed_min_amongst_delta_eta_wrt( particlePtrT& ref ) const {
    assert( !_constituents.empty() );
    const AnaParticle::ftype result = std::numeric_limits<AnaParticle::ftype>::max();
    BOOST_FOREACH( const boost::shared_ptr<const AnaParticle>& p , _constituents ) {
      const AnaParticle::ftype this_result = p->dist_eta( ref );
      if( std::abs(this_result) < std::abs(result) ) {
        result = this_result;
      }
    }
    return result;
  }

  template< typename particlePtrT >
  const AnaParticle::ftype signed_min_amongst_delta_eta_phi_wrt( particlePtrT& ref ) const {
    assert( !_constituents.empty() );
    const AnaParticle::ftype result = std::numeric_limits<AnaParticle::ftype>::max();
    BOOST_FOREACH( const boost::shared_ptr<const AnaParticle>& p , _constituents ) {
      const AnaParticle::ftype this_result = p->dist_eta_phi( ref );
      if( std::abs(this_result) < std::abs(result) ) {
        result = this_result;
      }
    }
    return result;
  }

  // print to cout

  virtual std::ostream& print( std::ostream& os = std::cout ) const {
    os << boost::format("AnaComposite: pt=%|+10|(GeV) eta=%|10| phi=%|10|") % et() % eta() % phi() << std::endl;
    BOOST_FOREACH( const ptr_type& v , _constituents ) {
      v->print( os );
    }
    return os;
  }


  // commonly useful function objects
  // ================================================================

  // composition from two particles
  class Compose {
  public:
    template<typename particleT>
    const boost::shared_ptr<const AnaComposite>
    operator()( const typename boost::shared_ptr<const particleT>& particle1 ,
                const typename boost::shared_ptr<const particleT>& particle2 ) const {
      if( !particle1 || !particle2 ) { return boost::shared_ptr<const AnaComposite>(); }
      return boost::make_shared<const AnaComposite>( boost::dynamic_pointer_cast<const AnaParticle>(particle1) , 
                                                     boost::dynamic_pointer_cast<const AnaParticle>(particle2) ,
                                                     AnaParticle::GENERIC );
    }
  };

  // all particles pass selector
  template<typename particleT>
  class AllConstituentsPass
  {
  public:
    typedef bool result_type;
    typedef DataGraphics::DgCutSet< boost::shared_ptr<particleT> > Selector;
    typedef boost::function<bool (typename boost::shared_ptr<particleT> const&) > Cut;
  protected:
    Selector* _selector;
    std::vector<boost::any> _copy_coll;
    std::vector< Cut > _cut_coll;
  public:
    AllConstituentsPass() : _selector(0) {}
    AllConstituentsPass( const Selector* sel ) : _selector(sel) {}
    template<typename bindT>
    void add_cut( bindT const& cut ) {
      _copy_coll.push_back( cut );
      _cut_coll.push_back( Cut(boost::any_cast<bindT>( _copy_coll.back() )) );
      assert( _copy_coll.size() == _cut_coll.size() );
    }
    template<class iteratorT>
    result_type operator()( iteratorT begin , iteratorT end ) const {
      assert( _selector || !_copy_coll.empty() );
      for( AnaComposite::const_iterator i=begin, f=end; i!=f; ++i ) {
        const boost::shared_ptr<particleT>& q( boost::dynamic_pointer_cast<particleT>(*i) );
        if( !q ) { // particle isn't even the desired type; it fails
          return false;
        }
        assert( q );
        if( _selector ) {
          if( !_selector->passes_all( q ) ) { return false; }
        }
        if( !_copy_coll.empty() ) {
          BOOST_FOREACH( const Cut& cut , _cut_coll ) {
            if( !cut( q ) ) { return false; }
          }
        }
      }
      return true;
    }
    result_type operator()( boost::shared_ptr<const AnaComposite> const& cop ) const {
      AnaComposite::const_iterator i=cop->begin_constituent();
      AnaComposite::const_iterator f=cop->end_constituent();
      return operator()( i , f );
    }
  };

  // at least one particle passes selector
  template<typename particleT>
  class AtLeastOnePasses
  {
  public:
    typedef bool result_type;
    typedef DataGraphics::DgCutSet< boost::shared_ptr<particleT> > Selector;
    typedef boost::function<bool (typename boost::shared_ptr<particleT> const&) > Cut;
  protected:
    Selector* _selector;
    std::vector<boost::any> _copy_coll;
    std::vector< Cut > _cut_coll;
  public:
    AtLeastOnePasses() : _selector(0) {}
    AtLeastOnePasses( const Selector* sel ) : _selector(sel) {}
    template<typename bindT>
    void add_cut( bindT const& cut ) {
      _copy_coll.push_back( cut );
      _cut_coll.push_back( Cut(boost::any_cast<bindT>( _copy_coll.back() )) );
      assert( _copy_coll.size() == _cut_coll.size() );
    }
    template<class iteratorT>
    result_type operator()( iteratorT begin , iteratorT end ) const {
      assert( _selector || !_copy_coll.empty() );
      for( iteratorT i=begin, f=end; i!=f; ++i ) {
        const boost::shared_ptr<particleT>& q( boost::dynamic_pointer_cast<particleT>(*i) );
        if( !q ) { // particle isn't even the desired type; it fails
          continue;
        }
        assert( q );
        if( _selector ) {
          if( _selector->passes_all( q ) ) { return true; }
        }
        if( !_copy_coll.empty() ) {
          bool passes = true;
          BOOST_FOREACH( const Cut& cut , _cut_coll ) {
            if( !cut( q ) ) { passes = false; break; }
          }
          if( passes ) { return true; }
        }
      }
      return false;
    }
    result_type operator()( boost::shared_ptr<const AnaComposite> const& cop ) const {
      AnaComposite::const_iterator i=cop->begin_constituent();
      AnaComposite::const_iterator f=cop->end_constituent();
      return operator()( i , f );
    }
  };
  
  // constituents are oppositely-charged
  class IsOppositeSignPair
  {
  public:
    typedef bool result_type;
  public:
    template<typename particlePtrT>
    const bool operator()( particlePtrT& p ) const {
      const boost::shared_ptr<const AnaComposite>& cop( boost::dynamic_pointer_cast<const AnaComposite>(p) );
      if( !cop ) { return false; }
      if( cop->num_constituents() !=2 ) { return false; }
      AnaParticle::ftype net_charge = 1.;
      for( AnaComposite::const_iterator i=cop->begin_constituent(), f=cop->end_constituent(); i!=f; ++i ) {
        try {
          net_charge *= (*i)->charge();
        } catch( std::out_of_range& e ) {
          // at least one particle is not charged.
          return false;
        }
      }
      return( net_charge < 0. );
    }
    template<typename daughterPtrT>
    const bool operator()( daughterPtrT const& p1 , daughterPtrT const& p2 ) const {
      try {
        return( p1->charge()*p2->charge() < 0 );
      } catch( std::out_of_range& e ) {
      }
      return false;
    }
  };

  // constituents are same-charge
  class IsSameSignPair
  {
  public:
    typedef bool result_type;
  public:
    template<typename particlePtrT>
    const bool operator()( particlePtrT& p ) const {
      const boost::shared_ptr<const AnaComposite>& cop( boost::dynamic_pointer_cast<const AnaComposite>(p) );
      if( !cop ) { return false; }
      if( cop->num_constituents() !=2 ) { return false; }
      AnaParticle::ftype net_charge = 1.;
      for( AnaComposite::const_iterator i=cop->begin_constituent(), f=cop->end_constituent(); i!=f; ++i ) {
        try {
          net_charge *= (*i)->charge();
        } catch( std::out_of_range& e ) {
          // at least one particle is not charged.
          return false;
        }
      }
      return( net_charge > 0. );
    }
    template<typename daughterPtrT>
    const bool operator()( daughterPtrT const& p1 , daughterPtrT const& p2 ) const {
      try {
        return( p1->charge()*p2->charge() > 0 );
      } catch( std::out_of_range& e ) {
      }
      return false;
    }
  };

  template<typename particleT>
  class HighestPtConstituents
  {
  public:
    HighestPtConstituents() {}
    virtual ~HighestPtConstituents() {}
    typedef boost::shared_ptr<particleT> const& result_type;
    result_type operator()( boost::shared_ptr<particleT> const& ap , boost::shared_ptr<particleT> const& bp ) const {
      boost::shared_ptr<const AnaComposite> const a( boost::dynamic_pointer_cast<const AnaComposite>(ap) );
      boost::shared_ptr<const AnaComposite> const b( boost::dynamic_pointer_cast<const AnaComposite>(bp) );
      if( !a && b ) { return bp; }
      if( !b && a ) { return ap; }
      // FIXME only works for higgs->ll+MET at present
      const unsigned int na = a->num_constituents();
      const unsigned int nb = b->num_constituents();
      if( nb!=3 ) { return ap; }
      if( na!=3 && nb==3 ) { return bp; }
      boost::shared_ptr<const AnaParticle> a1;
      boost::shared_ptr<const AnaParticle> a2;
      boost::shared_ptr<const AnaParticle> b1;
      boost::shared_ptr<const AnaParticle> b2;
      for( AnaComposite::const_iterator i=a->begin_constituent(), f=a->end_constituent(); i!=f; ++i ) {
        const boost::shared_ptr<const AnaParticle>& p(*i);
        if( p->type()!=AnaParticle::ELECTRON && p->type()!=AnaParticle::MUON ) { continue; }
        if( !a1 ) { a1 = p; } else if( !a2 ) { a2=p; }
      }
      for( AnaComposite::const_iterator i=b->begin_constituent(), f=b->end_constituent(); i!=f; ++i ) {
        const boost::shared_ptr<const AnaParticle>& p(*i);
        if( p->type()!=AnaParticle::ELECTRON && p->type()!=AnaParticle::MUON ) { continue; }
        if( !b1 ) { b1 = p; } else if( !b2 ) { b2=p; }
      }
      if( !b1 || !b2 ) { return ap; }
      if( b1 && b2 && (!a1 || !a2) ) { return bp; }
      if( a1->pt() < a2->pt() ) { a1.swap( a2 ); }
      if( b1->pt() < b2->pt() ) { b1.swap( b2 ); }
      if( a1->pt() < b1->pt() ) { return ap; }
      if( a1->pt() > b1->pt() ) { return bp; }
      // a1 pt == b1 pt
      return( a2->pt() < b2->pt() ? ap : bp );
      // for( AnaComposite::const_iterator ia=a->begin_constituents(), fa=a->end_constituents(); ia!=fa; ++ia ) {
      //   for( AnaComposite::const_iterator ib=b->begin_constituents(), fb=b->end_constituents(); ib!=fb; ++ib ) {
      //     if( a->pt() > 
      //   }
      // }
    }
  };

};

BOOST_CLASS_VERSION( AnaComposite , 1 );

#endif // ANACOMPOSITE_HPP

