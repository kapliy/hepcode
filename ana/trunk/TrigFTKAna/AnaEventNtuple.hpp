 
#ifndef ANAEVENTNTUPLE_HPP
#define ANAEVENTNTUPLE_HPP

#include "TrigFTKAna/AnaCommon.hpp"
#include <string>
#include <algorithm>
#include <map>
#include <vector>
#include <limits>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/version.hpp>
#include "TrigFTKAna/AnaEvent.hpp"


class
AnaEventNtuple
{
public:
  typedef enum { CLOSED , READ , WRITE } Mode;
public:
  typedef unsigned long FileIndex;
  class EventMetadata
  {
  private:
    unsigned long _run_number;
    unsigned long _lumi_block;
    unsigned long _event_number;
    FileIndex _ntuple_file_number;
  private:
    friend class boost::serialization::access;
    template<class archiveT>
    void serialize( archiveT& ar, const unsigned int version )
    {
      ar & BOOST_SERIALIZATION_NVP(_run_number);
      ar & BOOST_SERIALIZATION_NVP(_lumi_block);
      ar & BOOST_SERIALIZATION_NVP(_event_number);
      ar & BOOST_SERIALIZATION_NVP(_ntuple_file_number);
    }
  public:
    EventMetadata()
      : _run_number(std::numeric_limits<unsigned long>::max())
      , _lumi_block(std::numeric_limits<unsigned long>::max())
      , _event_number(std::numeric_limits<unsigned long>::max())
      , _ntuple_file_number(std::numeric_limits<FileIndex>::max())
    {}
    EventMetadata( const unsigned long& run_number , const unsigned long& lumi_block , 
                   const unsigned long& event_number , const FileIndex& ntuple_file_number )
      : _run_number(run_number)
      , _lumi_block(lumi_block)
      , _event_number(event_number)
      , _ntuple_file_number(ntuple_file_number)
    {}
    const unsigned long& run_number() const { return _run_number; }
    const unsigned long& lumi_block() const { return _lumi_block; }
    const unsigned long& event_number() const { return _event_number; }
    const FileIndex& ntuple_file_number() const { return _ntuple_file_number; }
  };
  class FileMetadata
  {
  private:
    std::string _filename;
    unsigned long _nevents_accepted;
    unsigned long _nevents_total;
  private:
    friend class boost::serialization::access;
    template<class archiveT>
    void serialize( archiveT& ar, const unsigned int version )
    {
      ar & BOOST_SERIALIZATION_NVP(_filename);
      ar & BOOST_SERIALIZATION_NVP(_nevents_accepted);
      ar & BOOST_SERIALIZATION_NVP(_nevents_total);
    }
  public:
    FileMetadata() 
      : _filename("")
      , _nevents_accepted(0ul)
      , _nevents_total(0ul)
    {}
    FileMetadata( const std::string& filename , const unsigned long& nevents_accepted , const unsigned long& nevents_total )
      : _filename(filename)
      , _nevents_accepted(nevents_accepted)
      , _nevents_total(nevents_total)
    {}
    const std::string& filename() const { return _filename; }
    const unsigned long& nevents_accepted() const { return _nevents_accepted; }
    const unsigned long& nevents_total() const { return _nevents_total; }
    const float fraction_accepted() const { return( _nevents_total>0 ? _nevents_accepted / static_cast<float>(_nevents_total) : 0. ); }
  };
  // in the following, all but EventFileMap can be an unordred_map,
  // but at present there is no serialization class for it.
  typedef std::map<FileIndex,FileMetadata> FileMap;
  typedef std::map<unsigned long,FileIndex> EventFileMap; // must be sorted
  typedef std::multimap<unsigned long,EventMetadata> EventMap;
  typedef std::map<unsigned long,EventMap> LumiBlockMap;
  typedef std::map<unsigned long,LumiBlockMap> EventMetadataMap;
  typedef std::vector< boost::shared_ptr<AnaEvent> > EventBlock;
private:
  std::string _directory;
  Mode _mode;
  FileMap _file_map;
  EventFileMap _event_map; // built on metadata load only
  EventMetadataMap _meta_map; // optional per-event metadata
  FileIndex _current_event_block_index;
  unsigned long _current_event_offset;
  EventBlock _event_block;
  unsigned long _block_accepted; // number of events accepted, reconstructed from metadata
  unsigned long _block_total; // total number of events, reconstructed from metadata
  boost::shared_ptr<AnaEvent> _last_add_attempt; // track last event passed to add_event (needed for bookkeeping accepted/total)
  boost::shared_ptr<AnaEvent> _last_add_success; // track last event passing add_event (needed for bookkeeping accepted/total)
  FileIndex _next_file_index;
  unsigned long _nevents_accepted; // number of events accepted, reconstructed from metadata
  unsigned long _nevents_total; // total number of events, reconstructed from metadata
  bool _record_per_event_metadata;
  unsigned int _nevents_per_block_goal; // try to write this number of events per block
  // save memory to specialized DgMemory heap 
  DECLARE_DGHEAP;
private:
  const std::string filename_for_index( const FileIndex& index );
  const FileIndex index_for_sequential_event( const unsigned long& ievent );
  const std::string path_to_new_sample_directory( const std::string& directory );
  void write_event_block( const FileIndex& index );
  void write_metadata();
  const bool read_metadata();
  const bool read_event_block( const FileIndex& index );
public:
  AnaEventNtuple() 
    : _directory("")
    , _mode( CLOSED )
    , _file_map()
    , _event_map()
    , _meta_map()
    , _current_event_block_index(std::numeric_limits<FileIndex>::max())
    , _current_event_offset(0)
    , _event_block()
    , _last_add_attempt()
    , _last_add_success()
    , _next_file_index(0)
    , _block_accepted(0ul)
    , _block_total(0ul)
    , _nevents_accepted(0ul)
    , _nevents_total(0ul)
    , _record_per_event_metadata(false)
    , _nevents_per_block_goal( 50u )
  {}
  virtual ~AnaEventNtuple() {
    if( _mode==WRITE ) { close_sample(); }
  }

  const bool open_sample( const std::string& name , const Mode& mode );
  void close_sample();

  const std::string& directory() const { return _directory; }
  const Mode& mode() const { return _mode; }

  // option to record per event information in metadata.xml. not
  // recommended when producing ntuples for large numbers of events,
  // because the metadata store can get very large. this slows writing
  // and reading down (since the entire metadata table is kept in
  // memory and must be serialized).
  void record_per_event_metadata( const bool& yes ) { _record_per_event_metadata = yes; }
  void nevents_per_block_goal( const unsigned int& nevents ) { _nevents_per_block_goal = nevents; }

  // write mode
  void add_event( boost::shared_ptr<const AnaEvent> event , const bool save_this_event = true );
  
  // read mode
  boost::shared_ptr<const AnaEvent> get_event( unsigned long& ievent );
  const unsigned long nevents_accepted() const { return _nevents_accepted; }
  const unsigned long nevents_total() const { return _nevents_total; }
  const float fraction_accepted() const { return( _nevents_total>0 ? _nevents_accepted / static_cast<float>(_nevents_total) : 0. ); }

};

BOOST_CLASS_VERSION( AnaEventNtuple::EventMetadata , 1 );
BOOST_CLASS_VERSION( AnaEventNtuple::FileMetadata , 1 );

#endif // ANAEVENTNTUPLE_HPP
