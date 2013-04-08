
#include <cassert>
#include <algorithm>
#include <utility>
#include <vector>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/foreach.hpp>
// #include <boost/multi_index_container.hpp>
// #include <boost/multi_index/ordered_index.hpp>
// #include <boost/multi_index/identity.hpp>
// #include <boost/multi_index/member.hpp>
#include "TrigFTKAna/AnaJet.hpp"
#include "TrigFTKAna/AnaTrack.hpp"
#include "TrigFTKAna/Detector.hpp"
#include "TrigFTKAna/AnaJetTrackAssociation.hpp"

using namespace std;
using namespace boost;
// using namespace boost::multi_index;

void
AnaJetTrackAssociation::_recompute_delta_r()
{
  _tracks_dr.clear();
  if( !_jet || _tracks.empty() ) { return; }
  // compute dr for each track. store the dr for each track in a
  // parallel vector, then sort both the _tracks and _tracks_dr vector
  // keyed by the track dr.
  typedef vector< pair<size_t,ftype> > Index;
  Index indices;
  indices.reserve( _tracks.size() );
  size_t n=0;
  for( TrackColl::const_iterator i=_tracks.begin(), f=_tracks.end(); i!=f; ++i,++n ) {
    const shared_ptr<const AnaTrack>& track( *i ); assert( track );
    const double dr = detector::delta_eta_phi( _jet->eta() , _jet->phi() , 
                                               track->eta() , track->phi0() );
    indices.push_back( Index::value_type(dr,n) );
  }
  std::sort( indices.begin() , indices.end() , bind( &Index::value_type::second,_1) < bind( &Index::value_type::second,_2) );
  TrackColl tmp; 
  tmp.reserve( _tracks.size() );
  _tracks_dr.reserve( _tracks.size() );
  for( Index::iterator i=indices.begin(), f=indices.end(); i!=f; ++i ) {
    tmp.push_back( _tracks[i->first] );
    _tracks_dr.push_back( i->second );
  }
  assert( _tracks_dr.size() == _tracks.size() );
  assert( tmp.size() == _tracks.size() );
  tmp.swap( _tracks );
}


template< typename jetIterT , typename trackIterT , typename jetOutputIterT > 
void
AnaJetTrackAssociation::build_associations( jetIterT begin_jets , jetIterT end_jets ,
                                            trackIterT begin_tracks , trackIterT end_tracks ,
                                            const typename AnaJetTrackAssociation::ftype& cone , 
                                            jetOutputIterT output_association )
{
  vector< shared_ptr< const AnaTrack > > tracks;
  for( jetIterT ijet = begin_jets; ijet!=end_jets; ++ijet ) {
    const shared_ptr< const AnaJet >& jet( *ijet );
    // associate tracks.
    tracks.clear();
    for( trackIterT itrack = begin_tracks; itrack != end_tracks; ++itrack ) {
      const shared_ptr< const AnaTrack >& track( *itrack );
      const double dr = detector::delta_eta_phi( jet->eta() , jet->phi() , track->eta() , track->phi0() );
      if( dr >= cone ) { continue; }
      tracks.push_back( track );
    }
    AnaJetTrackAssociation assoc( jet , tracks.begin() , tracks.end() , cone );
    (*output_association++) = assoc;
  }
}

// template< typename vector< shared_ptr<const AnaJet> >::iterator , typename vector< shared_ptr<const AnaTrack> >::iterator , back_insert_iterator< vector< shared_ptr<const AnaJetTrackAssociation> > > >
// void
// AnaJetTrackAssociation::build_associations( vector< shared_ptr<const AnaJet> >::iterator begin_jets , vector< shared_ptr<const AnaJet> >::iterator end_jets ,
//                                             vector< shared_ptr<const AnaTrack> >::iterator begin_tracks , vector< shared_ptr<const AnaTrack> >::iterator end_tracks ,
//                                             const AnaJetTrackAssociation::ftype& cone , 
//                                             back_insert_iterator< vector< shared_ptr<const AnaJetTrackAssociation> > > outIter );
