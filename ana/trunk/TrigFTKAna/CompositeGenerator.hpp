 
#ifndef COMPOSITEGENERATOR_HPP
#define COMPOSITEGENERATOR_HPP

#include <cassert>
#include <algorithm>
#include <iterator>
#include <boost/shared_ptr.hpp>
#include <boost/any.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/math/special_functions/fpclassify.hpp>
#include "TrigFTKAna/AnaComposite.hpp"

template<typename particleT,typename compositeT>
class
CompositeGenerator
{
public:

  typedef boost::function< bool ( const typename boost::shared_ptr<particleT const>& , 
                                  const typename boost::shared_ptr< particleT const>& ) > ConditionalPairFunctor;
  typedef boost::function< bool ( const typename boost::shared_ptr<particleT const>& , 
                                  const typename boost::shared_ptr< particleT const>& ) > OrthogonalityPairFunctor;
  typedef boost::function< bool ( const typename boost::shared_ptr<compositeT const>& ) > ConditionalCompositeFunctor;

private:
  
  ConditionalPairFunctor _predicate_pair;
  OrthogonalityPairFunctor _orthogonal_pair;
  ConditionalCompositeFunctor _predicate_composite;
  boost::any _predicate_pair_copy;
  boost::any _orthogonal_pair_copy;
  boost::any _predicate_composite_copy;

public:
  CompositeGenerator()
    : _predicate_pair()
    , _orthogonal_pair()
    , _predicate_composite()
  {}
  virtual ~CompositeGenerator() {}

  template<typename bindT>
  void set_pair_predicate( bindT const& predicate ) {
    _predicate_pair_copy = predicate;
    _predicate_pair = ConditionalPairFunctor(boost::any_cast<bindT>(_predicate_pair_copy));
  }
  template<typename bindT>
  void set_pair_orthogonality( bindT const& ortho ) {
    _orthogonal_pair_copy = ortho;
    _orthogonal_pair = OrthogonalityPairFunctor(boost::any_cast<bindT>(_orthogonal_pair_copy));
  }
  template<typename bindT>
  void set_composite_predicate( bindT const& predicate ) {
    _predicate_composite_copy = predicate;
    _predicate_composite = ConditionalCompositeFunctor(boost::any_cast<bindT>(_predicate_composite_copy));
  }
  
  // two body composition
  template< typename inputAIterT , typename inputBIterT , typename compositionT , typename outputCollT >
  void generate( inputAIterT beginA , inputAIterT endA , 
                 inputBIterT beginB , inputBIterT endB , 
                 compositionT compositor ,
                 outputCollT& output_coll ) {
    assert( output_coll.empty() ); // not necessary, but pay attention to this for now.
    for( inputAIterT iA=beginA; iA!=endA; ++iA ) {
      const boost::shared_ptr<particleT const>& pA( boost::dynamic_pointer_cast<particleT const>(*iA) ); assert( pA );
      // skip particle with non-sensical four vectors
      if( boost::math::isnan( pA->pt() ) || boost::math::isnan( pA->eta() ) || std::abs( pA->eta() ) > 1E3 ) { continue; }
      for( inputBIterT iB=beginB; iB!=endB; ++iB ) {
        const boost::shared_ptr<particleT const>& pB( boost::dynamic_pointer_cast<particleT const>(*iB) ); assert( pB );
        // skip particle with non-sensical four vectors
        if( boost::math::isnan( pB->pt() ) || boost::math::isnan( pB->eta() ) || std::abs( pB->eta() ) > 1E3 ) { continue; }
        // check that this isn't the same particle.  redundant with
        // potential orthogonal state checks, but a quick way to rule
        // out the obvious.
        if( pA == pB ) { continue; }
        // check that particles are orthogonal, if requested.
        // e.g. do composite particles share the same final state?
        if( !_orthogonal_pair.empty() && !(_orthogonal_pair(pA,pB)) ) { continue; }
        // check that this pairing is not already in the list of candidate particles
        typename outputCollT::iterator io = std::find_if( output_coll.begin() , output_coll.end() ,
                                                          boost::bind( &AnaComposite::consists_of , _1 , pA , pB ) );
        // don't duplicate existing entries
        if( io != output_coll.end() ) { continue; }
        // reject pairs for which the condition functor returns false.
        if( !_predicate_pair_copy.empty() && !(_predicate_pair(pA,pB)) ) { continue; }
        // if the compositor returns a null pointer, the composite is not valid.
        typename outputCollT::value_type cop( compositor(pA,pB) );
        if( !cop ) { continue; }
        assert( std::distance( cop->begin_constituent() , cop->end_constituent() ) == 2 );
        // drop composites with nonsensical four vectors
        if( boost::math::isnan( cop->pt() ) || boost::math::isnan( cop->mass() ) ) { continue; }
        // apply predicate if it exists
        if( !_predicate_composite_copy.empty() && !(_predicate_composite(cop)) ) { continue; }
        // add the composite to the output
        output_coll.push_back( cop );
      } // end for B
    } // end for A
  }
  
  // three body composition
  template< typename inputAIterT , typename inputBIterT , typename inputCIterT ,
            typename compositionT , typename outputCollT >
  void generate( inputAIterT beginA , inputAIterT endA , 
                 inputBIterT beginB , inputBIterT endB , 
                 inputCIterT beginC , inputCIterT endC , 
                 compositionT compositor ,
                 outputCollT& output_coll ) {
    assert( output_coll.empty() ); // not necessary, but pay attention to this for now.
    for( inputAIterT iA=beginA; iA!=endA; ++iA ) {
      const boost::shared_ptr<particleT const>& pA( boost::dynamic_pointer_cast<particleT const>(*iA) ); assert( pA );
      // skip particle with non-sensical four vectors
      if( boost::math::isnan( pA->pt() ) || boost::math::isnan( pA->eta() ) || std::abs( pA->eta() ) > 1E3 ) { continue; }
      for( inputBIterT iB=beginB; iB!=endB; ++iB ) {
        const boost::shared_ptr<particleT const>& pB( boost::dynamic_pointer_cast<particleT const>(*iB) ); assert( pB );
        // skip particle with non-sensical four vectors
        if( boost::math::isnan( pB->pt() ) || boost::math::isnan( pB->eta() ) || std::abs( pB->eta() ) > 1E3 ) { continue; }
        // check that this isn't the same particle.  redundant with
        // potential orthogonal state checks, but a quick way to rule
        // out the obvious.
        if( pA == pB ) { continue; }
        for( inputCIterT iC=beginC; iC!=endC; ++iC ) {
          const boost::shared_ptr<particleT const>& pC( boost::dynamic_pointer_cast<particleT const>(*iC) ); assert( pC );
          // skip particle with non-sensical four vectors
          if( boost::math::isnan( pC->pt() ) || boost::math::isnan( pC->eta() ) || std::abs( pC->eta() ) > 1E3 ) { continue; }
          // check that A,B,C are three different particles.  redundant with
          // potential orthogonal state checks, but a quick way to rule
          // out the obvious.
          if( pA == pC || pB==pC ) { continue; }
          // check that particles are not orthogonal, if requested.
          // e.g. do composite particles share the same final state?
          if( !_orthogonal_pair_copy.empty() && 
              !(_orthogonal_pair(pA,pB)) &&
              !(_orthogonal_pair(pA,pC)) &&
              !(_orthogonal_pair(pB,pC)) ) { continue; }
          // check that this pairing is not already in the list of candidate particles
          typename outputCollT::iterator io = std::find_if( output_coll.begin() , output_coll.end() ,
                                                            boost::bind( &AnaComposite::consists_of , _1 , pA , pB , pC ) );
          // don't duplicate existing entries
          if( io != output_coll.end() ) { continue; }
          // reject pairs for which the condition functor returns false.
          if( !_predicate_pair_copy.empty() ) {
            if( !(_predicate_pair(pA,pB)) ) { continue; }
            if( !(_predicate_pair(pA,pC)) ) { continue; }
            if( !(_predicate_pair(pB,pC)) ) { continue; }
          }
          // create the composite. if the compositor returns a null
          // pointer, the composite is not valid.
          typename outputCollT::value_type cop( compositor(pA,pB,pC) );
          if( !cop ) { continue; }
          assert( std::distance( cop->begin_constituent() , cop->end_constituent() ) == 3 );
          // drop composites with nonsensical four vectors
          if( boost::math::isnan( cop->pt() ) || boost::math::isnan( cop->mass() ) ) { continue; }
          // apply predicate if it exists
          if( !_predicate_composite_copy.empty() && !(_predicate_composite(cop)) ) { continue; }
          // add the composite to the output.
          output_coll.push_back( cop );
        } // end for each C
      } // end for each B
    } // end for each A
  }

};

// useful and common conditional and orthogonality classes

// determine if pA and pA share any final states.  if pA and pB are not composite, returns false
// if pA==pB and true otherwise.  if pA and/or pB are composite, returns false if, for any
// final states q1 in pA and q2 in pB, q1==q2.

template<typename compositeParticleT>
class 
OrthogonalFinalStatesCheck
{
public:
  template<typename particleT>
  const bool operator()( const boost::shared_ptr<particleT>& pA , const boost::shared_ptr<particleT>& pB ) const {
    assert( pA );
    assert( pB );
    const boost::shared_ptr<const compositeParticleT> copA( boost::dynamic_pointer_cast< const compositeParticleT >( pA ) );
    const boost::shared_ptr<const compositeParticleT> copB( boost::dynamic_pointer_cast< const compositeParticleT >( pB ) );
    if( copA && copB ) {
      for( typename compositeParticleT::const_iterator iA=copA->begin_constituent(), fA=copA->end_constituent(); iA!=fA; ++iA ) {
        for( typename compositeParticleT::const_iterator iB=copB->begin_constituent(), fB=copB->end_constituent(); iB!=fB; ++iB ) {
          if( ! operator()(*iA,*iB) ) { return false; }
        }
      }
      return true;
    } else if( copA ) {
      for( typename compositeParticleT::const_iterator iA=copA->begin_constituent(), fA=copA->end_constituent(); iA!=fA; ++iA ) {
        if( ! operator()(*iA,pB) ) { return false; }
      }
      return true;
    } else if( copB ) {
      for( typename compositeParticleT::const_iterator iB=copB->begin_constituent(), fB=copB->end_constituent(); iB!=fB; ++iB ) {
        if( ! operator()(pA,*iB) ) { return false; }
      }
      return true;
    } else {
      return( pA!=pB );
    }
  }
};


#endif // COMPOSITEGENERATOR_HPP
