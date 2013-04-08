#ifndef WRAP_NTROAD_HPP
#define WRAP_NTROAD_HPP

// NTROAD
// ================================================================
// TrigFTKSim ROOT road ntuple interpreter class.
// ================================================================
// 2009-05-20 Antonio Boveia (boveia@hep.uchicago.edu)

#include "TrigFTKAna_config.h"
#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include "TChain.h"
#include "TrigFTKAna/AnaRoad.hpp"
#include "TrigFTKAna/AnaSuperstrip.hpp"
#include "TrigFTKAna/Trees/NtRoadBankLookup.hpp"

#ifdef HAVE_LIBFTKSIM
class FTKRoadStream;
#endif

class AnaTrack;

class
NtRoad
{

public:
  // quickly loop through the file and count the number of events
  static const unsigned long count_events( const std::string& filename );

private:

  std::string _open_filename;

  AnaRoad::BankID _bank;
  unsigned long _run_number;
  unsigned long _event_number;
  unsigned long _n_events;
  unsigned int _n_planes;
  unsigned long _n_roads;
  unsigned long _n_superstrips;
  std::vector< boost::shared_ptr<const AnaSuperstrip> > _superstrips;
  std::vector< boost::shared_ptr<const AnaRoad> > _roads;
  std::vector< boost::shared_ptr<const AnaRoad> > _am_roads;
  std::vector<unsigned int> _n_clusters; // by plane
  
  boost::shared_ptr<TChain> _chain;
#ifdef HAVE_LIBFTKSIM
  boost::shared_ptr<FTKRoadStream> _road_stream;
  FTKRoadStream* _road_stream_ptr; // work around idiotic ROOT pass by pointer to pointer.
#endif

  NtRoadBankLookup _road_lookup;
  NtRoadBankLookup _am_road_lookup;
  NtRoadLayerLookup _ss_lookup;

protected:

  void clear() { 
    _open_filename = std::string();
    _run_number = 0ul;
    _event_number = 0ul;
    _n_planes = 0u;
    _n_roads = 0u;
    _n_superstrips = 0u;
    _superstrips.clear();
    _roads.clear();
    _am_roads.clear();
    _n_clusters.clear();
    _road_lookup.clear();
    _am_road_lookup.clear();
    _ss_lookup.clear();
  }
  
public:

  NtRoad()
    : _open_filename()
    , _bank(-1)
    , _run_number(0ul)
    , _event_number(0ul)
    , _n_events(0ul)
    , _n_planes(0u)
    , _n_roads(0ul)
    , _n_superstrips(0ul)
    , _chain()
#ifdef HAVE_LIBFTKSIM
    , _road_stream()
    , _road_stream_ptr(0)
#endif
    , _road_lookup()
    , _am_road_lookup()
    , _ss_lookup()
  {}
  virtual ~NtRoad() {
    clear();
  }

  const bool begin_file( const int& bank , const std::string& filename );
  const bool end_file();
  const std::string& open_filename() const { return _open_filename; }

  const bool get_event( const unsigned long& ievent );

  const unsigned long run_number() const { return _run_number; }
  const unsigned long event_number() const { return _event_number; }
  const unsigned long n_roads() const { return _n_roads; }
  const unsigned long n_superstrips() const { return _n_superstrips; }
  const unsigned int n_planes() const { return _n_planes; }
  const std::vector< boost::shared_ptr<const AnaSuperstrip> >& superstrips() const { return _superstrips; }
  const std::vector< boost::shared_ptr<const AnaRoad> >& roads() const { return _roads; }
  const std::vector< boost::shared_ptr<const AnaRoad> >& am_roads() const { return _am_roads; }
  const unsigned int n_clusters() const { return std::accumulate(_n_clusters.begin(),_n_clusters.end(),0); }
  const unsigned int n_clusters(unsigned int ipl) const { return ipl<_n_clusters.size() ? _n_clusters[ipl] : 0; }
  const NtRoadBankLookup& bank_road_lookup() const { return _road_lookup; }
  const NtRoadBankLookup& bank_am_road_lookup() const { return _am_road_lookup; }
  const NtRoadLayerLookup& bank_superstrip_lookup() const { return _ss_lookup; }
  // FlagJT: nbytes probably isn't correct any more (28Feb2011)
  const unsigned long nbytes() const { // size of current event 
    return( n_superstrips()*sizeof(AnaSuperstrip) + n_roads()*sizeof(AnaRoad) );
  }

  static const boost::shared_ptr<const AnaRoad>& road_lookup( const NtRoadBankLookup& roadlookup , const int& bank_id , const unsigned int& road_id ) {
    static boost::shared_ptr<const AnaRoad> null;
    NtRoadBankLookup::const_iterator i = roadlookup.find( bank_id );
    if( i==roadlookup.end() ) { return null; }
    NtRoadLookup::const_iterator j = i->second.find( road_id );
    if( j==i->second.end() ) { return null; }
    return j->second;
  }
  static const boost::shared_ptr<const AnaSuperstrip>& ss_lookup( const NtRoadLayerLookup& sslookup , const unsigned int& layer , const unsigned long& ss_id ) {
    static boost::shared_ptr<const AnaSuperstrip> null;
    NtRoadLayerLookup::const_iterator i = sslookup.find( layer );
    if( i==sslookup.end() ) { return null; }
    NtRoadSuperstripLookup::const_iterator j = i->second.find( ss_id );
    if( j==i->second.end() ) { return null; }
    return j->second;
  }

  static void fill_ftk_track_nshared( std::vector< boost::shared_ptr<const AnaTrack> >::const_iterator begin , 
                                      std::vector< boost::shared_ptr<const AnaTrack> >::const_iterator end , 
                                      const NtRoadBankLookup& roadlookup , const NtRoadLayerLookup& sslookup );
  
};

#endif // WRAP_NTROAD_HPP
