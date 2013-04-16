#ifndef WRAP_ANAEVENTMGR_HPP
#define WRAP_ANAEVENTMGR_HPP

// ANAEVENTMGR
// ================================================================
// Event retrieval facade. Hides complexity of multiple input files
// from different ntuple formats/text files.
//
// Call open_sample/close_sample to open/close the sample. Call begin
// and end for iterator-style traversal of the list of events

// ================================================================
// 2009-03-20 Antonio Boveia (boveia@hep.uchicago.edu)

#include <string>
#include <vector>
#include <map>
#include <deque>
#include <iostream>
#include <bitset>
#include <limits>
#include <cassert>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <boost/ref.hpp>
#include "TrigFTKAna/AnaProgress.hpp"
#include "TrigFTKAna/AnaEvent.hpp"
#include "TrigFTKAna/AnaTrack.hpp"
#include "TrigFTKAna/AnaSiCluster.hpp"
#include "TrigFTKAna/EventCountCache.hpp"
#include "TrigFTKAna/IgnoreFileCache.hpp"
#include "TrigFTKAna/AnaEventNtuple.hpp"

class NtD3PDphys;

class
AnaEventMgr
{

public:

  class
  iterator
  {
  public:
    // updated reserved to be equal to the number of special flags (number of bits)
    typedef enum { ALL_EVENTS=0 , RESERVED=1 } SpecialFlags;
  private:
    unsigned long _ievent;
    std::bitset<RESERVED> _special_flags;
  protected:
    iterator() : _ievent(0ul) , _special_flags(0) {}
    iterator( const unsigned long ievent ) : _ievent(ievent) , _special_flags(0) {}
    friend class AnaEventMgr;
  public:
    boost::shared_ptr<const AnaEvent> operator*() const { return AnaEventMgr::instance()->get_event(_ievent); }
    const bool operator==(const iterator& rhs) const { 
      if( (rhs._ievent==0 || rhs._ievent==std::numeric_limits<unsigned long>::max()) && rhs._special_flags.test(ALL_EVENTS) ) { return false; }
      if( (_ievent==0 || _ievent==std::numeric_limits<unsigned long>::max()) && _special_flags.test(ALL_EVENTS) ) { return false; }
      return _ievent==rhs._ievent; 
    }
    const bool operator!=(const iterator& rhs) const { return !( *this == rhs ); }
    iterator& operator++() { ++_ievent; return *this; }
    void operator++(int) { ++(*this); }
    void set_flag( const SpecialFlags& flag , const bool value = true ) { _special_flags.set( flag ); }
  };
  friend class iterator;

  static const std::string& DEFAULT_SOURCE;

  typedef boost::function<void (void)> Callback;

private:

  struct DatasetDefinition
  {
    typedef std::map< std::string , bool > SampleFlags;
    typedef std::map< std::string , double > SampleNumericFlags;
    typedef std::map< std::string , std::string > SampleStrings;
    typedef std::set< std::string > SampleTags;
    std::string sample_type;
    unsigned long nevents;
    SampleFlags sample_flags;
    SampleNumericFlags sample_numeric_flags;
    SampleStrings sample_strings;
    SampleTags sample_tags;
    std::map< std::string , std::vector<std::string> > d3pdnt_filenames;
    DatasetDefinition() 
      : sample_type("")
      , nevents(0)
      , sample_flags()
      , sample_numeric_flags()
      , sample_strings()
      , sample_tags()
      , d3pdnt_filenames()
    {}
    struct accNumEventsV { const unsigned long operator()( const unsigned long& a , const DatasetDefinition& b ) { return( a + b.nevents ); } };
    struct zeroEventsV { const bool operator()( const DatasetDefinition& b ) { return( b.nevents==0 ); } };
  };

private:

  std::deque<DatasetDefinition> _ntuple_filenames_pending;
  std::deque<DatasetDefinition> _ntuple_filenames_done;
  std::deque<DatasetDefinition> _ntuple_filenames_failed;
  std::deque<DatasetDefinition> _ntuple_filenames_skipped;
  iterator _begin;
  iterator _end;

  bool _file_open;
  unsigned long _file_ievent_offset;
  unsigned long _file_read_events;
  std::string _sample_type;
  static unsigned long _n_events_processed;
  static std::map<std::string,double> _reweighted_event_counts;

  std::map< std::string , std::vector< boost::shared_ptr<NtD3PDphys> > > _nt_d3pd;

  static AnaEventMgr* _instance;
  static AnaProgress<std::ostream> _progress;
  static EventCountCache _event_count_cache;
  static IgnoreFileCache _ignore_filenames_cache;
  static boost::shared_ptr<AnaEvent> _event;
  static boost::shared_ptr<const AnaEvent> _event_const;
  static AnaEventNtuple _ana_event_ntuple_read;
  static AnaEventNtuple _ana_event_ntuple_write;
  static std::vector< Callback > _begin_event_callbacks;
  static std::vector< Callback > _end_event_callbacks;
  static bool _copy_ftk_si_clusters;

protected:

  AnaEventMgr() 
    : _file_open( false )
    , _file_ievent_offset( 0ul )
    , _file_read_events( 0ul )
    , _sample_type( "" )
  {}
  virtual ~AnaEventMgr() {}
  // retrieve filenames from sample definition
  const bool retrieve_filenames();
  // open the next file
  const bool next_file();
  // retrieve this event number 
  const boost::shared_ptr<const AnaEvent> get_event( const unsigned long& ievent , const bool tried_next_file = false );

public:

  // get pointer to the singleton
  static AnaEventMgr* instance() { 
    if( !_instance ) { _instance = new AnaEventMgr; }
    return _instance;
  }
  // retrieve whatever the current event is at this exact moment. you
  // may want to wrap this with boost::ref and evaluate it
  // later. eventually there should be a function to do this for you.
  static const boost::shared_ptr<const AnaEvent>& current_event() { assert(_event_const); return _event_const; }

  static void add_begin_event_callback( const Callback& cb ) { _begin_event_callbacks.push_back(cb); }
  static void add_end_event_callback( const Callback& cb ) { _end_event_callbacks.push_back(cb); }

  void copy_ftk_si_clusters( const bool& yes ) { _copy_ftk_si_clusters = yes; }

  // open/close sample
  const bool open_sample(); // opens sample specified by AnaConfigration
  void close_sample();

  void set_weighted_nevents( const std::string& tag , const double& value ) { _reweighted_event_counts[ tag ] = value; }

  // execute begin/end event callbacks
  void do_begin_event_callbacks() const;
  void do_end_event_callbacks() const;

  // iterator access and traversal of AnaEvents
  AnaEventMgr::iterator begin() const { return _begin; }
  AnaEventMgr::iterator end() const { return _end; }
  AnaEventMgr::iterator end( const unsigned long& nevents ) const { 
    iterator newend( _end );
    newend._ievent = newend._ievent>0 ? std::min( newend._ievent , nevents ) : nevents;
    // if( nevents != std::numeric_limits<unsigned long>::max() ) {
    //   newend._special_flags.set( iterator::ALL_EVENTS , 0 );
    // }
    return newend;
  }
  
  // dataset "provenance"
  // retrieve the list of filenames relevant for the given event
  const std::vector<std::string> source_filenames( const AnaEventMgr::iterator& i );
  // retrieve the run number and event numbers for the given event
  const unsigned long source_run_number( const AnaEventMgr::iterator& i );
  const unsigned long source_event_number( const AnaEventMgr::iterator& i );
  void save_sample_description() const;
  
};

#endif // WRAP_ANAEVENTMGR_HPP
