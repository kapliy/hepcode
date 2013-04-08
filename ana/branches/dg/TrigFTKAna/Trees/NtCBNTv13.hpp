#ifndef WRAP_NTCBNTV13_HPP
#define WRAP_NTCBNTV13_HPP

// NTCBNTV13
// ================================================================
// 13.0.40 CBNT interpreter class.
// ================================================================
// 2009-07-14 Antonio Boveia (boveia@hep.uchicago.edu)

#include <string>
#include <vector>
#include <boost/scoped_ptr.hpp>
#include "TChain.h"
#include "TrigFTKAna/WHTreeV15.h"

class TBranch;
class AnaTrack;
class AnaJet;

class
NtCBNTv13
{

public:

  // quickly loop through the file and count the number of events
  static const unsigned long count_events( const std::string& filename );

private:

  std::string _open_filename;

  unsigned long _n_events;
  unsigned long _run_number;
  unsigned long _event_number;

  boost::scoped_ptr<TChain> _chain;
  boost::scoped_ptr<WHTreeV15> _madeclass;

  std::vector<const AnaTrack*> _l2_idscan_tracks;
  std::vector<const AnaTrack*> _l2_sitrack_tracks;
  std::vector<const AnaTrack*> _offline_tracks;

  std::vector<const AnaJet*> _l1_roi_jets;
  std::vector<const AnaJet*> _atlas_cone_4_jets;

public:

  NtCBNTv13()
    : _open_filename()
    , _n_events(0ul)
    , _run_number(0ul)
    , _event_number(0ul)
    , _chain()
    , _madeclass()
    , _l2_idscan_tracks()
    , _l2_sitrack_tracks()
    , _l1_roi_jets()
    , _atlas_cone_4_jets()
  {}
  virtual ~NtCBNTv13() {
    clear();
  }

  const bool begin_file( const std::string& filename );
  const unsigned long num_events() const { return _n_events; }
  const std::string open_filename() const { return _open_filename; }
  const bool end_file();

  const bool get_event( const unsigned long& ievent );

  const unsigned long run_number() const { return _run_number; }
  const unsigned long event_number() const { return _event_number; }

  const std::vector<const AnaTrack*>& l2_idscan_tracks() const { return _l2_idscan_tracks; }
  const std::vector<const AnaTrack*>& l2_sitrack_tracks() const { return _l2_sitrack_tracks; }
  const std::vector<const AnaTrack*>& offline_tracks() const { return _offline_tracks; }

  const std::vector<const AnaJet*>& l1_roi_jets() const { return _l1_roi_jets; }
  const std::vector<const AnaJet*>& atlas_cone_4_jets() const { return _atlas_cone_4_jets; }

  const unsigned long nbytes() const { // size of current event 
    return( 0 );
  }

protected:

  // clear the current event
  void clear() { 
    _open_filename = std::string();
    _run_number = 0ul;
    _event_number = 0ul;
    _l2_idscan_tracks.clear();
    _l2_sitrack_tracks.clear();
    _offline_tracks.clear();
    _l1_roi_jets.clear();
    _atlas_cone_4_jets.clear();
  }

  void set_branch_addresses();
  void set_enabled_branches();
  
};

#endif // WRAP_NTCBNTV13_HPP
