#ifndef WRAP_STUDYTRACKMATCHES_HH
#define WRAP_STUDYTRACKMATCHES_HH


// usage: e.g. to match FTK tracks to truth and study the match properties,
//
// StudyTrackMatches st( "ftk_wrt_all_truth" , "FTK reconstruction studies, given a truth track" );
// st.use_matching_strategy( TrackMatchingStrategy() );
// st.look_for_tracks_in( ftk_tracks.begin() , ftk_tracks.end() );
// st.for_each( truth_tracks.begin() , truth_tracks.end() ); // for each reference track

#include <vector>
#include <string>
#include <algorithm>
#include <utility>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <boost/foreach.hpp>
#include "DataGraphics/DgStudy.hh"
#include "DataGraphics/DgCollection.hh"
#include "TrigFTKAna/AnaTrack.hpp"

class TrackMatchingStrategy;

template<typename comparisonIteratorT>
class
StudyTrackMatches : public DataGraphics::DgStudy< boost::shared_ptr<const AnaTrack> >
{
public:
  typedef boost::shared_ptr<const AnaTrack> type; // must be same type as the template argument for DgStudy.
  typedef std::pair< type , std::vector<type> > Matches;
  //  typedef boost::function<bool (objectT const&) > TruthCriteria;
private:
  boost::shared_ptr<const TrackMatchingStrategy>  _strategy;
  comparisonIteratorT _begin_tracks;
  comparisonIteratorT _end_tracks;
protected:
  // given the reference track in the argument, 
  virtual void _study( type const& reference_track ) { // the second const is important---matches the base class.
    using namespace DataGraphics;
    // look for tracks matching this reference
    std::vector<type> matches;
    
  }
public:
  StudyTrackMatches( const std::string& name , const std::string& description , 
                     comparisonIteratorT begin_tracks , comparisonIteratorT end_tracks ) 
    : DataGraphics::DgStudy<type>(name,description)
    , _strategy( 0 )
    , _begin_tracks( begin_tracks )
    , _end_tracks( end_tracks )
  {}
  virtual ~StudyTrackMatches() {}
  void use_matching_strategy( const boost::shared_ptr<const TrackMatchingStrategy>& strategy ) { _strategy = strategy; }
  void look_for_tracks_in( comparisonIteratorT begin_tracks , comparisonIteratorT end_tracks ) {
    _begin_tracks = begin_tracks;
    _end_tracks = end_tracks;
  }
  
  
};
#endif // WRAP_STUDYTRACKMATCHES_HH
