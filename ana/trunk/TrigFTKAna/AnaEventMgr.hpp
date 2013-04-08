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

class NtWrapper;
class NtTrigFTKLib;
class NtRoad;
class NtFitter;
class NtCBNT;
class NtJetTag;
class NtPGS;
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
    std::map< std::string , std::vector<std::string> > wrapper_filenames;
    std::map< std::string , std::vector<std::string> > trigftklib_filenames;
    std::map< std::string , std::vector<std::string> > road_filenames;
    std::map< std::string , std::vector<unsigned int> > road_banks;
    std::map< std::string , std::vector<std::string> > fitter_filenames;
    std::map< std::string , std::vector<int> > fitter_banks;
    std::map< std::string , std::vector<std::string> > cbntv13_filenames;
    std::map< std::string , std::vector<std::string> > cbntv14_filenames;
    std::map< std::string , std::vector<std::string> > jettagnt_filenames;
    std::map< std::string , std::vector<std::string> > d3pdnt_filenames;
    std::map< std::string , std::vector<std::string> > pgs_filenames;
    std::string aen_filename;
    DatasetDefinition() 
      : sample_type("")
      , nevents(0)
      , sample_flags()
      , sample_numeric_flags()
      , sample_strings()
      , sample_tags()
      , wrapper_filenames()
      , trigftklib_filenames()
      , road_filenames()
      , road_banks()
      , fitter_filenames()
      , fitter_banks()
      , cbntv13_filenames()
      , cbntv14_filenames()
      , jettagnt_filenames()
      , d3pdnt_filenames()
      , pgs_filenames()
      , aen_filename()
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

  std::map< std::string , std::vector< boost::shared_ptr<NtWrapper> > > _nt_wrappers;
  std::map< std::string , std::vector< boost::shared_ptr<NtTrigFTKLib> > > _nt_trigftklibs;
  std::map< std::string , std::vector< boost::shared_ptr<NtRoad> > > _nt_roads;
  std::map< std::string , std::vector< boost::shared_ptr<NtFitter> > > _nt_fitters;
  std::map< std::string , std::vector< boost::shared_ptr<NtCBNT> > > _nt_cbnt;
  std::map< std::string , std::vector< boost::shared_ptr<NtJetTag> > > _nt_jettag;
  std::map< std::string , std::vector< boost::shared_ptr<NtPGS> > > _nt_pgs;
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

  // merge CBNT offline track info with wrapper offline track info.
  template< typename insertIterT >
  void merge_wrapper_cbnt( std::vector< boost::shared_ptr<const AnaTrack> >::const_iterator beginWrapper , 
                           std::vector< boost::shared_ptr<const AnaTrack> >::const_iterator endWrapper ,
                           std::vector< boost::shared_ptr<const AnaTrack> >::const_iterator beginCBNT , 
                           std::vector< boost::shared_ptr<const AnaTrack> >::const_iterator endCBNT ,
                           insertIterT iOutput );

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
  // retrieve source ntuples (currently only for merging in test/merge_cbnt_ftk.cpp)
  const boost::shared_ptr<NtCBNT>& source_cbnt( const std::string& source = DEFAULT_SOURCE ) const { 
    std::map< std::string , std::vector< boost::shared_ptr<NtCBNT> > >::const_iterator i = _nt_cbnt.find(source);
    if( i==_nt_cbnt.end() ) { 
      static boost::shared_ptr<NtCBNT> empty;
      return empty;
    }
    return i->second.front();
  }
  const boost::shared_ptr<NtFitter>& source_ftk_fitter( const std::string& source = DEFAULT_SOURCE ) const { 
    std::map< std::string , std::vector< boost::shared_ptr<NtFitter> > >::const_iterator i = _nt_fitters.find(source);
    if( i==_nt_fitters.end() ) { 
      static boost::shared_ptr<NtFitter> empty;
      return empty;
    }
    return i->second.front();
  }
  void save_sample_description() const;
  

  // skimming/trimming/conversion to AnaEventNtuple
  const bool create_ana_event_ntuple( const std::string& directory );
  // call this for every event with the decision on whether or not to keep an event.
  // this allows the code to track the fraction of rejected events.
  const bool add_to_ana_event_ntuple( const bool& add_true_not_false );
  const AnaEventNtuple::Mode ana_event_ntuple_mode() const { return _ana_event_ntuple_write.mode(); }
  
};

#endif // WRAP_ANAEVENTMGR_HPP
