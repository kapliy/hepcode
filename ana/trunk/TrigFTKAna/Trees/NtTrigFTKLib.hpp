#ifndef WRAP_NTTRIGFTKLIB_HPP
#define WRAP_NTTRIGFTKLIB_HPP

// NTTRIGFTKLIB
// ================================================================
// FastTrackSimWrap text ntuple interpreter class.
// ================================================================
// 2009-05-20 Antonio Boveia (boveia@hep.uchicago.edu)

#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/optional.hpp>
#include "TrigFTKAna/ana_streams.hpp"
#include "TrigFTKAna/AnaSiChannel.hpp"
#include "TrigFTKAna/AnaTrack.hpp"

class
NtTrigFTKLib
{

public:
  // quickly loop through the file and count the number of events
  static const unsigned long count_events( const std::string& filename );

private:

  std::string _open_filename;

  unsigned long _run_number;
  unsigned long _event_number;
  std::vector< boost::shared_ptr<const AnaTrack> > _ftk_tracks;

  boost::shared_ptr<ana_streams::istream> _inf;
  boost::optional<unsigned long> _last_event;

protected:

  void clear() {
    _open_filename.clear();
    _run_number = 0ul;
    _event_number = 0ul;
    _ftk_tracks.clear();
  }

public:

  NtTrigFTKLib()
    : _open_filename()
    , _run_number(0ul)
    , _event_number(0ul)
    , _inf()
    , _last_event()
  {}
  virtual ~NtTrigFTKLib() {
    clear();
  }

  const bool begin_file( const std::string& filename );
  const bool end_file();
  const std::string& open_filename() const { return _open_filename; }

  const bool get_event( const unsigned long& ievent );

  const unsigned long run_number() const { return _run_number; }
  const unsigned long event_number() const { return _event_number; }
  const std::vector< boost::shared_ptr<const AnaTrack> >& ftk_tracks() const { return _ftk_tracks; }
  const unsigned long nbytes() const { // size of current event 
    return( _ftk_tracks.size()*sizeof(AnaTrack) );
  }

};

#endif // WRAP_NTTRIGFTKLIB_HPP
