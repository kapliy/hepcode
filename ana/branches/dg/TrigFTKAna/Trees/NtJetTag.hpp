#ifndef WRAP_NTJETTAG_HPP
#define WRAP_NTJETTAG_HPP

// NTJETTAG
// ================================================================
// 13.0.40 or 14.2.25.10 JETTAG interpreter class.
// ================================================================
// 2009-07-14 Antonio Boveia (boveia@hep.uchicago.edu)

#include <string>
#include <vector>
#include <map>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <TChain.h>
#include "TrigFTKAna/Trees/TagTree.h"

class TBranch;
class AnaTrack;
class AnaJet;


class
NtJetTag
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
  boost::scoped_ptr<TagTree> _madeclass;


public:

  NtJetTag()
    : _open_filename()
    , _n_events(0ul)
    , _run_number(0ul)
    , _event_number(0ul)
    , _chain()
    , _madeclass()
  {}
  virtual ~NtJetTag() {
    clear();
  }

  const bool begin_file( const std::string& filename );
  const unsigned long num_events() const { return _n_events; }
  const std::string open_filename() const { return _open_filename; }
  const bool end_file();

  const bool get_event( const unsigned long& ievent );

  const unsigned long run_number() const { return _run_number; }
  const unsigned long event_number() const { return _event_number; }

  const unsigned long nbytes() const { // size of current event 
    return( 0 );
  }

  void fill_jet_tag_info( std::vector< boost::shared_ptr<const AnaJet> >::iterator begin , std::vector< boost::shared_ptr<const AnaJet> >::iterator end ) const;

protected:

  // clear the current event
  void clear() { 
    _open_filename = std::string();
    _run_number = 0ul;
    _event_number = 0ul;
  }

  void set_branch_addresses();
  void set_enabled_branches();

  // construct from the ntuple version
  template<typename vntupleT>
  const bool _get_event( const vntupleT* nt );
  
};

#endif // WRAP_NTJETTAG_HPP
