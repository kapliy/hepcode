#ifndef WRAP_ANAJETTRACKASSOCIATION_HPP
#define WRAP_ANAJETTRACKASSOCIATION_HPP

#include <vector>
#include <algorithm>
#include <iterator>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>

class AnaTrack;
class AnaJet;

class
AnaJetTrackAssociation
{
public:
  typedef float ftype;
private:
  typedef std::vector< boost::shared_ptr<const AnaTrack> > TrackColl;
  typedef std::vector< ftype > TrackDeltaColl;
  boost::shared_ptr<const AnaJet> _jet;
  TrackColl _tracks;
  TrackDeltaColl _tracks_dr; // delta_r between track and jet for each track in _tracks
  ftype _max_cone_radius;
private:
  void _recompute_delta_r();
public:
  AnaJetTrackAssociation() 
    : _jet()
    , _tracks()
    , _tracks_dr()
    , _max_cone_radius(0.)
  {}
  AnaJetTrackAssociation( const boost::shared_ptr<const AnaJet>& jet )
    : _jet(jet)
    , _tracks()
    , _tracks_dr()
    , _max_cone_radius(0.)
  {}
  template< typename iteratorT >
  AnaJetTrackAssociation( const boost::shared_ptr<const AnaJet>& jet , 
                       iteratorT begin_tracks , iteratorT end_tracks ,
                       const ftype& cone_radius )
    : _jet(jet)
    , _tracks(begin_tracks,end_tracks)
    , _tracks_dr()
    , _max_cone_radius(cone_radius)
  {
    _recompute_delta_r();
  }
  virtual ~AnaJetTrackAssociation() {}

  void set_jet( const boost::shared_ptr< const AnaJet >& jet , const ftype& max_cone_radius ) {
    _jet = jet;
    _max_cone_radius = max_cone_radius;
    _tracks.clear();
    _tracks_dr.clear();
  }
  void add_track( const boost::shared_ptr< const AnaTrack >& track ) {
    _tracks.push_back( track );
    _recompute_delta_r();
  }

  const boost::shared_ptr<const AnaJet> jet() const { return _jet; }

  // tracks within maximum input radius
  const ftype max_radius() const { return _max_cone_radius; }
  const unsigned int n_tracks() const { return _tracks.size(); }
  const bool empty() const { return _tracks.empty(); }
  template< typename iteratorT > iteratorT begin() const { return _tracks.begin(); }
  template< typename iteratorT > iteratorT end() const { return _tracks.end(); }

  // tracks within radius dr
  const unsigned int n_tracks( const ftype& r) const { 
    return( std::count_if( _tracks_dr.begin() , _tracks_dr.end() , boost::bind( std::less<ftype>() , _1 , r ) ) );
  }
  const unsigned int empty( const ftype& r) const { 
      return( n_tracks(r)==0 );
  }
  template< typename iteratorT > iteratorT begin( const ftype& r ) const {
    if( _tracks.empty() ) { return end<iteratorT>(r); }
    if( _tracks_dr.front() >= r ) { return end<iteratorT>(r); }
    return _tracks.begin(); 
  }
  template< typename iteratorT > iteratorT end( const ftype& r ) const { 
    if( _tracks.empty() ) { return _tracks.end(); }
    if( _tracks_dr.front() >= r ) { return _tracks.end(); }
    TrackDeltaColl::const_iterator i = std::find_if( _tracks_dr.begin() , _tracks_dr.end() , boost::bind( std::greater_equal<ftype>() , _1 , r ) );
    if( i==_tracks_dr.end() ) { return _tracks.end(); }
    TrackDeltaColl::const_iterator j = _tracks.begin();
    std::advance( j , std::distance(_tracks_dr.begin(),i) );
    return j;
  }


  // helper function
  template< typename jetIterT , typename trackIterT , typename jetOutputIterT > 
  static void build_associations( jetIterT begin_jets , jetIterT end_jets ,
                                  trackIterT begin_tracks , trackIterT end_tracks ,
                                  const AnaJetTrackAssociation::ftype& max_cone_size ,
                                  jetOutputIterT output_association );
  
};


#endif // WRAP_ANAJETTRACKASSOCIATION_HPP
