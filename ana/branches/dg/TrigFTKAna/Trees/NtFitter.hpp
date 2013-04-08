#ifndef WRAP_NTFITTER_HPP
#define WRAP_NTFITTER_HPP

// NTFITTER
// ================================================================
// TrigFTKSim ROOT fitter ntuple interpreter class.
// ================================================================
// 2009-05-20 Antonio Boveia (boveia@hep.uchicago.edu)

#include "TrigFTKAna_config.h"
#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include "TChain.h"
#include "TrigFTKAna/AnaTrack.hpp"

#ifdef HAVE_LIBFTKSIM
class FTKTrackStream;
#endif

class
NtFitter
{

public:
  // quickly loop through the file and count the number of events
  static const unsigned long count_events( const std::string& filename );

  static const int MERGED_BANK;

private:

  std::string _open_filename;

  int _bank;
  unsigned long _run_number;
  unsigned long _event_number;
  unsigned long _n_events;
  unsigned long _n_ftk_tracks;
  unsigned long _n_ftk_combinations;
  unsigned long _n_ftk_fits;
  unsigned long _n_ftk_fits_majority;
  unsigned long _n_ftk_fits_rec;
  unsigned long _n_ftk_fits_addrec;
  unsigned long _n_ftk_fits_bad;
  unsigned long _n_ftk_fits_hitwarrior_rejected;
  unsigned long _n_ftk_fits_bad_majority;
  unsigned long _n_ftk_fits_hitwarrior_rejected_majority;
  std::vector< boost::shared_ptr<const AnaTrack> > _ftk_tracks;
  // Simple way: duplicate everything for 7L incomplete tracks
  unsigned long _n_ftk_tracksI;
  unsigned long _n_ftk_combinationsI;
  unsigned long _n_ftk_fitsI;
  unsigned long _n_ftk_fits_majorityI;
  unsigned long _n_ftk_fits_recI;
  unsigned long _n_ftk_fits_addrecI;
  unsigned long _n_ftk_fits_badI;
  unsigned long _n_ftk_fits_hitwarrior_rejectedI;
  unsigned long _n_ftk_fits_bad_majorityI;
  unsigned long _n_ftk_fits_hitwarrior_rejected_majorityI;
  std::vector< boost::shared_ptr<const AnaTrack> > _ftk_tracks_incomplete;
  
  boost::shared_ptr<TChain> _chain;
#ifdef HAVE_LIBFTKSIM
  boost::shared_ptr<FTKTrackStream> _track_stream;
  FTKTrackStream* _track_stream_ptr; // work around idiotic ROOT pass by pointer to pointer.
#endif

protected:

  void clear() {
    _open_filename = std::string();
    _run_number = 0ul;
    _event_number = 0ul;
    _n_ftk_tracks = 0l;
    _n_ftk_combinations = 0;
    _n_ftk_fits = 0;
    _n_ftk_fits_majority = 0;
    _n_ftk_fits_rec = 0;
    _n_ftk_fits_addrec = 0;
    _n_ftk_fits_bad = 0;
    _n_ftk_fits_hitwarrior_rejected = 0;
    _n_ftk_fits_bad_majority = 0;
    _n_ftk_fits_hitwarrior_rejected_majority = 0;
    _ftk_tracks.clear();
    _n_ftk_tracksI = 0l;
    _n_ftk_combinationsI = 0;
    _n_ftk_fitsI = 0;
    _n_ftk_fits_majorityI = 0;
    _n_ftk_fits_recI = 0;
    _n_ftk_fits_addrecI = 0;
    _n_ftk_fits_badI = 0;
    _n_ftk_fits_hitwarrior_rejectedI = 0;
    _n_ftk_fits_bad_majorityI = 0;
    _n_ftk_fits_hitwarrior_rejected_majorityI = 0;
    _ftk_tracks_incomplete.clear();
  }

public:

  NtFitter()
    : _open_filename()
    , _bank(-1)
    , _run_number(0ul)
    , _event_number(0ul)
    , _n_events(0ul)
    , _n_ftk_tracks(0ul)
    , _n_ftk_combinations(0ul)
    , _n_ftk_fits(0ul)
    , _n_ftk_fits_majority(0ul)
    , _n_ftk_fits_rec(0ul)
    , _n_ftk_fits_addrec(0ul)
    , _n_ftk_fits_bad(0ul)
    , _n_ftk_fits_hitwarrior_rejected(0ul)
    , _n_ftk_fits_bad_majority(0ul)
    , _n_ftk_fits_hitwarrior_rejected_majority(0ul)
    , _n_ftk_tracksI(0ul)
    , _n_ftk_combinationsI(0ul)
    , _n_ftk_fitsI(0ul)
    , _n_ftk_fits_majorityI(0ul)
    , _n_ftk_fits_recI(0ul)
    , _n_ftk_fits_addrecI(0ul)
    , _n_ftk_fits_badI(0ul)
    , _n_ftk_fits_hitwarrior_rejectedI(0ul)
    , _n_ftk_fits_bad_majorityI(0ul)
    , _n_ftk_fits_hitwarrior_rejected_majorityI(0ul)
    , _chain()
#ifdef HAVE_LIBFTKSIM
    , _track_stream()
    , _track_stream_ptr(0)
#endif
  {}
  virtual ~NtFitter() {
    clear();
  }

  const bool begin_file( const int& bank , const std::string& filename );
  const bool end_file();
  const std::string& open_filename() const { return _open_filename; }

  const bool get_event( const unsigned long& ievent );

  const unsigned long run_number() const { return _run_number; }
  const unsigned long event_number() const { return _event_number; }
  const unsigned long ftk_n_tracks(bool incomplete = false) const { return incomplete ? _n_ftk_tracksI : _n_ftk_tracks; }
  const unsigned long ftk_n_combinations(bool incomplete = false) const { return incomplete ? _n_ftk_combinationsI : _n_ftk_combinations; }
  const unsigned long ftk_n_fits(bool incomplete = false) const { return incomplete ? _n_ftk_fitsI : _n_ftk_fits; }
  const unsigned long ftk_n_fits_majority(bool incomplete = false) const { return incomplete ? _n_ftk_fits_majorityI : _n_ftk_fits_majority; }
  const unsigned long ftk_n_fits_rec(bool incomplete = false) const { return incomplete ? _n_ftk_fits_recI : _n_ftk_fits_rec; }
  const unsigned long ftk_n_fits_addrec(bool incomplete = false) const { return incomplete ? _n_ftk_fits_addrecI : _n_ftk_fits_addrec; }
  const unsigned long ftk_n_fits_bad(bool incomplete = false) const { return incomplete ? _n_ftk_fits_badI : _n_ftk_fits_bad; }
  const unsigned long ftk_n_fits_hitwarrior_rejected(bool incomplete = false) const { return incomplete ? _n_ftk_fits_hitwarrior_rejectedI : _n_ftk_fits_hitwarrior_rejected; }
  const unsigned long ftk_n_fits_majority_bad(bool incomplete = false) const { return incomplete ? _n_ftk_fits_bad_majorityI : _n_ftk_fits_bad_majority; }
  const unsigned long ftk_n_fits_hitwarrior_rejected_majority(bool incomplete = false) const { return incomplete ? _n_ftk_fits_hitwarrior_rejected_majorityI : _n_ftk_fits_hitwarrior_rejected_majority; }
  const std::vector< boost::shared_ptr<const AnaTrack> >& ftk_tracks(bool incomplete = false) const { return incomplete ? _ftk_tracks_incomplete : _ftk_tracks; }
  const unsigned long nbytes() const { // size of current event 
    return( ftk_n_tracks()*sizeof(AnaTrack) );
  }

};

#endif // WRAP_NTFITTER_HPP
