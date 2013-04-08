#ifndef WRAP_NTD3PDPHYSWZ_HPP
#define WRAP_NTD3PDPHYSWZ_HPP

// NTD3PDPHYSWZ
// ================================================================
// WZ + jets analysis physics D3PDs
// ================================================================
// 2010-06-23 Updated to May reprocessing D3PD format
// 2010-04-16 Antonio Boveia (boveia@hep.uchicago.edu)

#include <string>
#include <vector>
#include <map>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/foreach.hpp>
#include <algorithm>
#include "TFile.h"
#include "TTree.h"
#include "TrigFTKAna/Trees/NtD3PDphys.hpp"
#include "TrigFTKAna/AnaJet.hpp"
#include "TrigFTKAna/AnaElectron.hpp"
#include "TrigFTKAna/AnaPhoton.hpp"
#include "TrigFTKAna/AnaMuon.hpp"
#include "TrigFTKAna/AnaMET.hpp"
#include "TrigFTKAna/AnaTrack.hpp"
#include "TrigFTKAna/AnaTrigger.hpp"
#include "TrigFTKAna/AnaVertex.hpp"
#include "TrigFTKAna/AnaTruthParticle.hpp"
#include "TrigFTKAna/AnaTruthTable.hpp"
#include "TrigFTKAna/Trees/D3PDWZTreeBase.h"

class TBranch;

class
NtD3PDphysWZ : public NtD3PDphys
{

public:
  // quickly loop through the file and count the number of events
  static const unsigned long count_events( const std::string& filename );

private:

  std::string _open_filename;

  unsigned long _n_events;
  unsigned long _run_number;
  unsigned long _event_number;
  unsigned long _lumi_block;
  ftype _mc_weight;

  boost::scoped_ptr<TFile> _file;
  typedef enum { V1=0 , V2=1 , V3=2, V4=3, V5=4 , RESERVED=5 } Version;
  Version _madeclass_version;
  boost::scoped_ptr<D3PDWZTreeBase> _madeclass;

  std::vector< boost::shared_ptr<const AnaJet> > _jets;
  std::vector< boost::shared_ptr<const AnaElectron> > _electrons;
  std::vector< boost::shared_ptr<const AnaPhoton> > _photons;
  std::vector< boost::shared_ptr<const AnaMuon> > _muons;
  std::vector< boost::shared_ptr<const AnaMET> > _met;
  boost::shared_ptr<AnaTrigger> _trigger;
  std::vector< boost::shared_ptr<const AnaVertex> > _vertices;
  std::vector< boost::shared_ptr<const AnaTrack> > _tracks;
  std::vector< boost::shared_ptr<const AnaTruthParticle> > _truth_particles;
  boost::shared_ptr<AnaTruthTable> _truth_table;

private:

  const bool _get_event_V1();
  const bool _get_event_V2();
  const bool _get_event_V3();
  const bool _get_event_V4();
  const bool _get_event_V5();

  template<typename ntPtrT> void _make_associated_track_for_electron( boost::shared_ptr<AnaElectron>& ele , 
                                                                      ntPtrT nt , const unsigned int& i , const boost::shared_ptr<const AnaVertex>& vx );
  template<typename ntPtrT> void _make_associated_track_for_muid_muon( boost::shared_ptr<AnaMuon>& mu , 
                                                                       ntPtrT nt , const unsigned int& i , const boost::shared_ptr<const AnaVertex>& vx );
  template<typename ntPtrT> void _make_associated_track_for_staco_muon( boost::shared_ptr<AnaMuon>& mu , 
                                                                        ntPtrT nt , const unsigned int& i , const boost::shared_ptr<const AnaVertex>& vx );
  template<typename ntT> void  _make_associated_track_for_calo_muon( boost::shared_ptr<AnaMuon>& mu , 
                                                                     ntT nt , const unsigned int& i , const boost::shared_ptr<const AnaVertex>& vx );

  template<typename ntT> void _make_vertices( ntT nt );

  template<typename ntT> void _make_vertices_V4( ntT nt );

  template<typename ntT> void _make_vertices_V5( ntT nt );


public:

  NtD3PDphysWZ()
    : _open_filename()
    , _n_events(0ul)
    , _run_number(0ul)
    , _event_number(0ul)
    , _lumi_block(0ul)
    , _mc_weight(1.)
    , _madeclass_version( V1 )
    , _madeclass()
    , _jets()
    , _electrons()
    , _photons()
    , _muons()
    , _met()
    , _trigger()
    , _vertices()
    , _tracks()
    , _truth_particles()
    , _truth_table()
    , NtD3PDphys(NtD3PDphys::WZ)
  {}
  virtual ~NtD3PDphysWZ() {
    clear();
  }

  virtual const bool begin_file( const std::string& filename );
  virtual const unsigned long num_events() const { return _n_events; }
  virtual const std::string open_filename() const { return _open_filename; }
  virtual const bool end_file();

  virtual const bool get_event( const unsigned long& ievent );

  virtual const unsigned long run_number() const { return _run_number; }
  virtual const unsigned long event_number() const { return _event_number; }
  virtual const unsigned long lumi_block() const { return _lumi_block; }
  virtual const ftype mc_weight() const { return _mc_weight; }

  virtual const unsigned long nbytes() const { // size of current event 
    return( 0 );
  }

  virtual const std::vector< boost::shared_ptr<const AnaElectron> >& electrons() const { return _electrons; }
  virtual const std::vector< boost::shared_ptr<const AnaPhoton> >& photons() const { return _photons; }
  virtual const std::vector< boost::shared_ptr<const AnaMuon> >& muons() const { return _muons; }
  virtual const std::vector< boost::shared_ptr<const AnaJet> >& jets() const { return _jets; }
  virtual const std::vector< boost::shared_ptr<const AnaVertex> >& vertices() const { return _vertices; }
  virtual const std::vector< boost::shared_ptr<const AnaTrack> >& offline_tracks() const { return _tracks; }
  virtual const std::vector< boost::shared_ptr<const AnaMET> >& missing_et() const { return _met; }
  virtual const boost::shared_ptr<const AnaTrigger> trigger() const { return boost::const_pointer_cast<const AnaTrigger>(_trigger); }
  virtual const std::vector< boost::shared_ptr<const AnaTruthParticle> >& truth_particles() const { return _truth_particles; }
  virtual const boost::shared_ptr<const AnaTruthTable> truth_table() const { return boost::const_pointer_cast<const AnaTruthTable>(_truth_table); }

protected:

  // clear the current event
  void clear() { 
    _run_number = 0ul;
    _event_number = 0ul;
    _lumi_block = 0ul;
    _mc_weight = 1.;
    _electrons.clear();
    _photons.clear();
    _muons.clear();
    _jets.clear();
    _vertices.clear();
    _tracks.clear();
    _met.clear();
    _trigger.reset();
    _truth_particles.clear();
    _truth_table.reset();
    assert( _electrons.empty() );
    assert( _photons.empty() );
    assert( _muons.empty() );
    assert( _jets.empty() );
    assert( _vertices.empty() );
    assert( _tracks.empty() );
    assert( _met.empty() );
    assert( _truth_particles.empty() );
  }

  void set_branch_addresses();
  void set_enabled_branches();

};

#endif // WRAP_NTD3PDPHYSWZ_HPP
