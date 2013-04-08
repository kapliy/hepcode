#ifndef WRAP_NTCBNT_HPP
#define WRAP_NTCBNT_HPP

// NTCBNT
// ================================================================
// 13.0.40 or 14.2.25.10 CBNT interpreter class.
// ================================================================
// 2009-07-14 Antonio Boveia (boveia@hep.uchicago.edu)

#include <string>
#include <vector>
#include <map>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include "TTree.h"
#include "TFile.h"
#include "TrigFTKAna/Trees/WHTreeV13.h"
#include "TrigFTKAna/Trees/WHTreeV14.h"

class TBranch;
class AnaTrack;
class AnaJet;

#include "TrigFTKAna/AnaJet.hpp"

class
NtCBNT
{

public:

  // v13 or v14 ntuple?
  typedef enum { MODE_V13=1, MODE_V14=0 } Mode;

  // quickly loop through the file and count the number of events
  static const unsigned long count_events( const std::string& filename , const Mode& mode );

private:

  std::string _open_filename;

  unsigned long _n_events;
  unsigned long _run_number;
  unsigned long _event_number;

  boost::scoped_ptr<TFile> _file;
  Mode _mode;
  boost::scoped_ptr<WHTreeV13> _madeclass_v13;
  boost::scoped_ptr<WHTreeV14> _madeclass_v14;

  std::vector< boost::shared_ptr<const AnaTrack> > _l2_idscan_tracks;
  std::vector< boost::shared_ptr<const AnaTrack> > _l2_sitrack_tracks;
  std::vector< boost::shared_ptr<const AnaTrack> > _offline_tracks;

  std::vector< boost::shared_ptr<const AnaJet> > _l1_roi_jets;
  std::vector< boost::shared_ptr<const AnaJet> > _atlas_cone_4_tower_jets;
  std::vector< boost::shared_ptr<const AnaJet> > _atlas_cone_7_tower_jets;
  std::vector< boost::shared_ptr<const AnaJet> > _atlas_cone_4_truth_jets;
  std::vector< boost::shared_ptr<const AnaJet> > _atlas_cone_4_truth_pileup_jets;

  typedef std::map<unsigned int,AnaJet::Hadron> HadronMap;
  HadronMap _hadrons_by_index;

  bool _inconsistentCBNT; // work around athena CBNT fill problem

public:

  NtCBNT()
    : _open_filename()
    , _n_events(0ul)
    , _run_number(0ul)
    , _event_number(0ul)
    , _file()
    , _mode( MODE_V14 )
    , _madeclass_v13()
    , _madeclass_v14()
    , _l2_idscan_tracks()
    , _l2_sitrack_tracks()
    , _l1_roi_jets()
    , _atlas_cone_4_tower_jets()
    , _atlas_cone_7_tower_jets()
    , _atlas_cone_4_truth_jets()
    , _atlas_cone_4_truth_pileup_jets()
    , _hadrons_by_index()
    , _inconsistentCBNT(false)
  {}
  virtual ~NtCBNT() {
    clear();
  }

  const bool begin_file( const std::string& filename , const Mode& mode );
  const unsigned long num_events() const { return _n_events; }
  const std::string open_filename() const { return _open_filename; }
  const bool end_file();

  const bool get_event( const unsigned long& ievent );

  const unsigned long run_number() const { return _run_number; }
  const unsigned long event_number() const { return _event_number; }

  const std::vector< boost::shared_ptr<const AnaTrack> >& l2_idscan_tracks() const { return _l2_idscan_tracks; }
  const std::vector< boost::shared_ptr<const AnaTrack> >& l2_sitrack_tracks() const { return _l2_sitrack_tracks; }
  const std::vector< boost::shared_ptr<const AnaTrack> >& offline_tracks() const { return _offline_tracks; }

  const std::vector< boost::shared_ptr<const AnaJet> >& l1_roi_jets() const { return _l1_roi_jets; }
  const std::vector< boost::shared_ptr<const AnaJet> >& atlas_cone_4_tower_jets() const { return _atlas_cone_4_tower_jets; }
  const std::vector< boost::shared_ptr<const AnaJet> >& atlas_cone_7_tower_jets() const { return _atlas_cone_7_tower_jets; }
  const std::vector< boost::shared_ptr<const AnaJet> >& atlas_cone_4_truth_jets() const { return _atlas_cone_4_truth_jets; }
  const std::vector< boost::shared_ptr<const AnaJet> >& atlas_cone_4_truth_pileup_jets() const { return _atlas_cone_4_truth_pileup_jets; }

  const unsigned long nbytes() const { // size of current event 
    return( 0 );
  }

  const bool inconsistentCBNT() const { return _inconsistentCBNT; }

  void fill_truth_track_endvertex( std::vector< boost::shared_ptr<const AnaTrack> >& tracks ) const;

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
    _atlas_cone_4_tower_jets.clear();
    _atlas_cone_7_tower_jets.clear();
    _atlas_cone_4_truth_jets.clear();
    _atlas_cone_4_truth_pileup_jets.clear();
    _inconsistentCBNT = false;
  }

  void set_branch_addresses();
  void set_enabled_branches();

  // construct from the ntuple version
  template<typename vntupleT>
  const bool _get_event( const vntupleT* nt );
  
};

#endif // WRAP_NTCBNT_HPP
