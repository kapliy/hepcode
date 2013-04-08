#ifndef WRAP_NTPGS_HPP
#define WRAP_NTPGS_HPP

// NTPGS
// ================================================================
// Pretty Good Simulation (PGS) output interpreter class.
// ================================================================
// 2010-10-20 Antonio Boveia (boveia@hep.uchicago.edu)

#include <string>
#include <vector>
#include <sstream>
#include <boost/shared_ptr.hpp>
#include <boost/optional.hpp>
#include "TrigFTKAna/ana_streams.hpp"
#include "TrigFTKAna/AnaJet.hpp"
#include "TrigFTKAna/AnaElectron.hpp"
#include "TrigFTKAna/AnaMuon.hpp"
#include "TrigFTKAna/AnaMET.hpp"

class
NtPGS
{

public:
  // quickly loop through the file and count the number of events
  static const unsigned long count_events( const std::string& filename );

private:

  std::string _open_filename;

  unsigned long _run_number;
  unsigned long _event_number;
  std::vector< boost::shared_ptr<const AnaElectron> > _electrons;
  std::vector< boost::shared_ptr<const AnaMuon> > _muons;
  std::vector< boost::shared_ptr<const AnaJet> > _jets;
  std::vector< boost::shared_ptr<const AnaMET> > _mets;

  boost::shared_ptr<ana_streams::istream> _inf;
  boost::optional<unsigned long> _last_event;
  boost::optional<std::string> _cached_line;

protected:

  void clear() {
    _open_filename.clear();
    _run_number = 0ul;
    _event_number = 0ul;
    _electrons.clear();
    _muons.clear();
    _jets.clear();
    _mets.clear();
  }

  template<typename T>
  void convert( const std::string& str , T& result ) const {
    std::stringstream s(str);
    s >> result;
  }

public:

  NtPGS()
      : _open_filename()
      , _run_number(0ul)
      , _event_number(0ul)
      , _inf()
      , _last_event()
  {}
  virtual ~NtPGS() {
    clear();
  }

  const bool begin_file( const std::string& filename );
  const bool end_file();
  const std::string& open_filename() const { return _open_filename; }

  const bool get_event( const unsigned long& ievent );

  const unsigned long run_number() const { return _run_number; }
  const unsigned long event_number() const { return _event_number; }
  const std::vector< boost::shared_ptr<const AnaElectron> >& electrons() const { return _electrons; }
  const std::vector< boost::shared_ptr<const AnaMuon> >& muons() const { return _muons; }
  const std::vector< boost::shared_ptr<const AnaJet> >& jets() const { return _jets; }
  const std::vector< boost::shared_ptr<const AnaMET> >& mets() const { return _mets; }
  const unsigned long nbytes() const { return 0; } // size of current event; not implemented

};

#endif // WRAP_NTPGS_HPP
