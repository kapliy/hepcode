
#include <cassert>
#include <cmath>
#include <iostream>
#include <typeinfo>
#include <algorithm>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <TLorentzVector.h>
#include "TrigFTKAna/AnaTrack.hpp"
#include "TrigFTKAna/AnaMatching.hpp"

using namespace std;
using namespace boost;

// template< typename objRefInputIteratorT , typename objCompInputIteratorT , typename objOutputIteratorT >
// void AnaMatchingAlgorithm::match( objRefInputIteratorT begin_reference , objRefInputIteratorT end_reference ,
//                                        objCompInputIteratorT begin_comparison , objCompInputIteratorT end_comparison ,
//                                        objOutputIteratorT output_iterator )
// {
//   cout << " unimplemented matching for reference "
//        << typeid(objRefInputIteratorT::value_type).name()
//        << " and comparison "
//        << typeid(objRefInputIteratorT::value_type).name()
//        << " for output iterator "
//        << typeid(objOutputIteratorT).name()
//        << endl;
//   assert( "!unimplemented" );
// }


// Matching using truth GEANT info.
// ================================================================

// AnaTrack -> AnaTrack matching
template< typename objRefInputIteratorT , typename objCompInputIteratorT , typename objOutputIteratorT >
void AnaGeantMatchingAlgorithm::match( objRefInputIteratorT i , objRefInputIteratorT end_reference ,
                                       objCompInputIteratorT begin_comparison , objCompInputIteratorT end_comparison ,
                                       objOutputIteratorT output_iterator )
{
  // make all matches which share the same GEANT parent.
  vector< boost::shared_ptr<const AnaTrack> > matches;
  for( ; i != end_reference; ++i ) {
    const boost::shared_ptr<const AnaTrack>& reference( *i );
    matches.clear();
    // copy only comparison tracks whose AnaTrack::unique_barcode==reference->unique_barcode().
    std::remove_copy_if( begin_comparison , end_comparison , back_inserter(matches) , 
                         bind(&AnaTrack::barcode,_1) != reference->barcode() );
    // add match to the output.
    *output_iterator++ = AnaMatch< boost::shared_ptr<const AnaTrack> , 
                                   boost::shared_ptr<const AnaTrack> >( reference , matches );
  }
}

// Matching using Guido's track parameter matching criteria
// ================================================================

// AnaTrack -> AnaTrack matching
template< typename objRefInputIteratorT , typename objCompInputIteratorT , typename objOutputIteratorT >
void AnaGuidoMatchingAlgorithm::match( objRefInputIteratorT i_reference , objRefInputIteratorT end_reference ,
                                       objCompInputIteratorT begin_comparison , objCompInputIteratorT end_comparison ,
                                       objOutputIteratorT output_iterator )
{
  // make all matches which share the same GEANT parent.
  vector< boost::shared_ptr<const AnaTrack> > matches;
  for( ; i_reference != end_reference; ++i_reference ) {
    const boost::shared_ptr<const AnaTrack>& reference( *i_reference );
    const AnaTrack::ftype ref_phi0 = reference->phi0();
    const AnaTrack::ftype ref_z0 = reference->z0();
    const AnaTrack::ftype ref_ct = reference->ct();
    const AnaTrack::ftype ref_curvature = reference->curvature();
    matches.clear();
    for( objCompInputIteratorT i=begin_comparison; i!=end_comparison; ++i ) {
      const boost::shared_ptr<const AnaTrack>& comparison( *i );
      // make Guido's cuts
      if( std::abs(comparison->phi0()-ref_phi0) >= 6e-3 ) { continue; }
      if( std::abs(comparison->ct()-ref_ct) >= 8e-3 ) { continue; }
      if( std::abs(comparison->curvature()-ref_curvature)/2.0 >= 2e-5 ) { continue; }
      if( std::abs(comparison->z0()-ref_z0) >= 0.8 ) { continue; }
      matches.push_back( comparison );
    }
    *output_iterator++ = AnaMatch< boost::shared_ptr<const AnaTrack> , 
                                   boost::shared_ptr<const AnaTrack> >( reference , matches );
  }
}

