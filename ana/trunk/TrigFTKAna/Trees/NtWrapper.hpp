#ifndef WRAP_NTWRAPPER_HPP
#define WRAP_NTWRAPPER_HPP

// NTWRAPPER
// ================================================================
// FastTrackSimWrap text ntuple interpreter class.
// ================================================================
// 2009-05-20 Antonio Boveia (boveia@hep.uchicago.edu)

#include <string>
#include <vector>
#include <list>
#include <map>
#include <sstream>
#include <boost/shared_ptr.hpp>
#include <boost/optional.hpp>
#include "TrigFTKAna/ana_streams.hpp"
#include "TrigFTKAna/AnaSiChannel.hpp"
#include "TrigFTKAna/AnaSiCluster.hpp"
#include "TrigFTKAna/AnaTrack.hpp"
#include "TrigFTKAna/AnaBeamline.hpp"
#include "TrigFTKAna/AnaVertex.hpp"
#include "TrigFTKAna/AnaTrigger.hpp"
#include "TrigFTKAna/SiModuleList.hpp"

class
NtWrapper
{

public:
  // quickly loop through the file and count the number of events
  static const unsigned long count_events( const std::string& filename );

private:

  std::string _open_filename;

  unsigned long _run_number;
  unsigned long _event_number;
  unsigned long _lumi_block_number;
  unsigned long _bunch_number;
  std::vector< boost::shared_ptr<const AnaSiChannel> > _silicon_channels;
  std::vector< boost::shared_ptr<const AnaSiCluster> > _silicon_clusters;
  std::vector< boost::shared_ptr<const AnaTrack> > _offline_tracks;
  std::vector< boost::shared_ptr<const AnaTrack> > _truth_tracks;
  std::vector< boost::shared_ptr<const AnaVertex> > _vertices;
  boost::shared_ptr<AnaBeamline> _beamline;
  boost::shared_ptr<AnaTrigger> _trigger;
  boost::shared_ptr<SiModuleList> _lumi_block_bad_modules;
  boost::shared_ptr<SiModuleList> _event_bad_modules;

  boost::shared_ptr<ana_streams::istream> _inf;
  boost::optional<unsigned long> _last_event;

protected:

  void clear() {
    _open_filename.clear();
    _run_number = 0ul;
    _event_number = 0ul;
    _silicon_channels.clear();
    _silicon_clusters.clear();
    _offline_tracks.clear();
    _truth_tracks.clear();
    _vertices.clear();
    _beamline.reset();
    _trigger.reset();
    _event_bad_modules.reset();
  }

  template<typename T>
  void convert( const std::string& str , T& result ) const {
    std::stringstream s(str);
    s >> result;
  }

  // parsing subroutines
  // typedef std::list<std::string> TokenColl;
  typedef std::vector<std::string> TokenColl;
  typedef std::map< boost::shared_ptr<AnaTrack> , std::list<int> > EventClusterMap;
  void tokenize( const std::string& line , TokenColl& tokens );
  void read_silicon_channel( TokenColl& tokens );
  void read_pixel_cluster( TokenColl& tokens );
  void read_sct_cluster( TokenColl& tokens );
  void read_offline_track( TokenColl& tokens , EventClusterMap& event_clu_map );
  void read_truth_track( TokenColl& tokens );
  void read_beamline( TokenColl& tokens );
  void read_extra_trigger( TokenColl& tokens );
  void read_extra_eventinfo( TokenColl& tokens );
  void read_bad_modules( TokenColl& tokens );

public:

  NtWrapper()
    : _open_filename()
    , _run_number(0ul)
    , _event_number(0ul)
    , _inf()
    , _last_event()
  {}
  virtual ~NtWrapper() {
    clear();
  }

  const bool begin_file( const std::string& filename );
  const bool end_file();
  const std::string& open_filename() const { return _open_filename; }

  const bool get_event( const unsigned long& ievent );

  const unsigned long run_number() const { return _run_number; }
  const unsigned long event_number() const { return _event_number; }
  const unsigned long lumi_block_number() const { return _lumi_block_number; }
  const unsigned long bunch_number() const { return _bunch_number; }
  const std::vector< boost::shared_ptr<const AnaSiChannel> >& silicon_channels() const { return _silicon_channels; }
  const std::vector< boost::shared_ptr<const AnaSiCluster> >& silicon_clusters() const { return _silicon_clusters; }
  const std::vector< boost::shared_ptr<const AnaTrack> >& offline_tracks() const { return _offline_tracks; }
  const std::vector< boost::shared_ptr<const AnaTrack> >& truth_tracks() const { return _truth_tracks; }
  const std::vector< boost::shared_ptr<const AnaVertex> >& vertices() const { return _vertices; }
  const boost::shared_ptr<const AnaBeamline> beamline() const { return boost::const_pointer_cast<const AnaBeamline>(_beamline); }
  const boost::shared_ptr<const AnaTrigger> trigger() const { return boost::const_pointer_cast<const AnaTrigger>(_trigger); }
  const boost::shared_ptr<const SiModuleList> bad_modules() const { return boost::const_pointer_cast<const SiModuleList>(_lumi_block_bad_modules); }
  const unsigned long nbytes() const { // size of current event 
    return( (_offline_tracks.size()+_truth_tracks.size())*sizeof(AnaTrack) );
  }

};

#endif // WRAP_NTWRAPPER_HPP
