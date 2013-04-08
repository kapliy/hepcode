
#include <cassert>
#include <cmath>
#include <list>
#include <limits>
#include <boost/shared_ptr.hpp>
#include <boost/assign/std/list.hpp>
#include <boost/foreach.hpp>
#include <TLorentzVector.h>
#include "TrigFTKAna/AnaComposite.hpp"
#include "TrigFTKAna/Detector.hpp"

using namespace boost::assign;

AnaComposite::AnaComposite( AnaComposite::iterator begin , AnaComposite::iterator end , const Type type = AnaParticle::GENERIC )
  : _constituents( begin , end ) 
  , _charge()
  , _transverse_mass()
  , AnaParticle( type ) 
{
  assert( begin!=end );
  _precompute();
}

AnaComposite::AnaComposite( AnaComposite::const_iterator begin , AnaComposite::const_iterator end , const Type type = AnaParticle::GENERIC  )
  : _constituents( begin , end ) 
  , _charge()
  , _transverse_mass()
  , AnaParticle( type ) 
{
  assert( begin!=end );
  _precompute();
}

AnaComposite::AnaComposite( const boost::shared_ptr<const AnaParticle>& p1 , const boost::shared_ptr<const AnaParticle>& p2 , const Type type = AnaParticle::GENERIC  )
  : _constituents()
  , _charge()
  , _transverse_mass()
  , AnaParticle( type ) 
{
  assert( p1 );
  assert( p2 );
  assert( p1!=p2 );
  push_back( _constituents ) = p1 , p2;
  _precompute();
}

AnaComposite::AnaComposite( const boost::shared_ptr<const AnaParticle>& p1 , const boost::shared_ptr<const AnaParticle>& p2 , const boost::shared_ptr<const AnaParticle>& p3 , const Type type = AnaParticle::GENERIC  )
  : _constituents()
  , _charge()
  , _transverse_mass()
  , AnaParticle( type )
{
  assert( p1 );
  assert( p2 );
  assert( p3 );
  assert( p1!=p2 );
  assert( p1!=p3 );
  assert( p2!=p3 );
  push_back( _constituents ) = p1 , p2 , p3;
  _precompute();
}

void
AnaComposite::_precompute()
{
  _momentum.SetPxPyPzE( 0 , 0 , 0 , 0 );
  _charge.reset( 0 );
  BOOST_FOREACH( boost::shared_ptr<const AnaParticle> p , _constituents ) {
    _momentum += p->four_vector();
    if( _charge ) { 
      try {
        (*_charge) += p->charge();
      } catch( std::out_of_range& ) {
        _charge.reset(); // charge is undefined
      }
    }
  } // sum over each particle
}

void
AnaComposite::_compute_transverse_mass()
{
  // n-particle generalization of MT = [ 2 pT MET (1-cos(dphi)) ]^(1/2)
  TLorentzVector q( 0 , 0 , 0 , 0 );
  BOOST_FOREACH( boost::shared_ptr<const AnaParticle> p , _constituents ) {
    assert( p );
    const TLorentzVector& pinc( p->four_vector_for_composites() );
    // create a massless transverse four vector
    static TLorentzVector tmp;
    tmp.SetPxPyPzE( pinc.Px() , pinc.Py() , 0. , detector::quadrature(pinc.Px(),pinc.Py()) );
    q += tmp;
  }
  _transverse_mass.reset( q.M() );
}

const AnaParticle::ftype
AnaComposite::max_delta_phi_opening() const 
{
  AnaParticle::ftype max_abs_delta_phi( std::numeric_limits<AnaParticle::ftype>::min() );
  for( const_iterator i=_constituents.begin(), f=_constituents.end(); i!=f; ++i ) {
    for( const_iterator j=i; j!=f; ++j ) {
      if( i==j ) { continue; }
      const AnaParticle::ftype this_abs_dphi( std::abs( detector::delta_phi( (*i)->phi() , (*j)->phi() ) ) );
      max_abs_delta_phi = std::max( max_abs_delta_phi , this_abs_dphi );
    }
  }
  return max_abs_delta_phi;
}

const AnaParticle::ftype
AnaComposite::min_delta_phi_opening() const 
{
  AnaParticle::ftype min_abs_delta_phi( std::numeric_limits<AnaParticle::ftype>::max() );
  for( const_iterator i=_constituents.begin(), f=_constituents.end(); i!=f; ++i ) {
    for( const_iterator j=i; j!=f; ++j ) {
      if( i==j ) { continue; }
      const AnaParticle::ftype this_abs_dphi( std::abs( detector::delta_phi( (*i)->phi() , (*j)->phi() ) ) );
      min_abs_delta_phi = std::min( min_abs_delta_phi , this_abs_dphi );
    }
  }
  return min_abs_delta_phi;
}

const AnaParticle::ftype
AnaComposite::max_delta_eta_opening() const 
{
  AnaParticle::ftype max_abs_delta_eta( std::numeric_limits<AnaParticle::ftype>::min() );
  for( const_iterator i=_constituents.begin(), f=_constituents.end(); i!=f; ++i ) {
    for( const_iterator j=i; j!=f; ++j ) {
      if( i==j ) { continue; }
      const AnaParticle::ftype this_abs_deta( std::abs( detector::delta_eta( (*i)->eta() , (*j)->eta() ) ) );
      max_abs_delta_eta = std::max( max_abs_delta_eta , this_abs_deta );
    }
  }
  return max_abs_delta_eta;
}

const AnaParticle::ftype
AnaComposite::min_delta_eta_opening() const 
{
  AnaParticle::ftype min_abs_delta_eta( std::numeric_limits<AnaParticle::ftype>::max() );
  for( const_iterator i=_constituents.begin(), f=_constituents.end(); i!=f; ++i ) {
    for( const_iterator j=i; j!=f; ++j ) {
      if( i==j ) { continue; }
      const AnaParticle::ftype this_abs_deta( std::abs( detector::delta_eta( (*i)->eta() , (*j)->eta() ) ) );
      min_abs_delta_eta = std::min( min_abs_delta_eta , this_abs_deta );
    }
  }
  return min_abs_delta_eta;
}

const AnaParticle::ftype
AnaComposite::max_delta_r_opening() const 
{
  AnaParticle::ftype max_delta_r( std::numeric_limits<AnaParticle::ftype>::min() );
  for( const_iterator i=_constituents.begin(), f=_constituents.end(); i!=f; ++i ) {
    for( const_iterator j=i; j!=f; ++j ) {
      if( i==j ) { continue; }
      const AnaParticle::ftype this_dr( detector::delta_eta_phi( (*i)->eta() , (*i)->phi() ,
                                                                 (*j)->eta() , (*j)->phi() ) );
      max_delta_r = std::max( max_delta_r , this_dr );
    }
  }
  return max_delta_r;
}

const AnaParticle::ftype
AnaComposite::min_delta_r_opening() const 
{
  AnaParticle::ftype min_delta_r( std::numeric_limits<AnaParticle::ftype>::max() );
  for( const_iterator i=_constituents.begin(), f=_constituents.end(); i!=f; ++i ) {
    for( const_iterator j=i; j!=f; ++j ) {
      if( i==j ) { continue; }
      const AnaParticle::ftype this_dr( detector::delta_eta_phi( (*i)->eta() , (*i)->phi() ,
                                                                 (*j)->eta() , (*j)->phi() ) );
      min_delta_r = std::min( min_delta_r , this_dr );
    }
  }
  return min_delta_r;
}
