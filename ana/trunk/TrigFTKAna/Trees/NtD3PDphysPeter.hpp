#ifndef WRAP_NTD3PDPHYSPETER_HPP
#define WRAP_NTD3PDPHYSPETER_HPP

// NTD3PDPHYSPETER
// ================================================================
// w->munu asymmetry analysis over Peter Onyisi's ntuple
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
#include <TFile.h>
#include "TrigFTKAna/Trees/NtD3PDphys.hpp"
#include "TrigFTKAna/AnaJet.hpp"
#include "TrigFTKAna/AnaTruthParticle.hpp"
#include "TrigFTKAna/AnaElectron.hpp"
#include "TrigFTKAna/AnaMuon.hpp"
#include "TrigFTKAna/AnaPhoton.hpp"
#include "TrigFTKAna/AnaMET.hpp"
#include "TrigFTKAna/AnaTrigger.hpp"
#include "TrigFTKAna/AnaVertex.hpp"
#include "TrigFTKAna/Trees/D3PDPeterTreeBase.h"

class TBranch;

class
NtD3PDphysPeter : public NtD3PDphys
{

public:
  // quickly loop through the file and count the number of events
  static const unsigned long count_events( const std::string& filename );

  typedef float ftype;

private:

  std::string _open_filename;

  unsigned long _n_events;
  unsigned long _run_number;
  unsigned long _event_number;
  unsigned long _lumi_block;
  double _average_mu;
  double _actual_mu;
  ftype _mc_weight;
  std::vector<ftype> _pdf_weights;
  unsigned long _mc_channel;
  int _mc_hfor;
  bool  _lar_error;
  int _bdff;

  // McEvtCollection variables (for PDF reweighting and other fancy stuff)
  float _mcevt_event_scale;
  float _mcevt_alphaQCD;
  float _mcevt_alphaQED;
  float _mcevt_pdf_scale;  // pdf energy scale (Q)
  int _mcevt_id1;  // pdg id of first incoming parton
  int _mcevt_id2;  // pdg id of second incoming parton
  float _mcevt_pdf_x1; // bjorken x of first incoming parton
  float _mcevt_pdf_x2; // bjorken x of second incoming parton
  float _mcevt_pdf1;   // pdf probability for first incoming parton
  float _mcevt_pdf2;   // pdf probability for second incoming parton

  boost::scoped_ptr<TFile> _file;
  typedef enum { V29, V29g, V29i, RESERVED=100 } Version;
  Version _madeclass_version;
  boost::scoped_ptr<D3PDPeterTreeBase> _madeclass;

  std::vector< boost::shared_ptr<const AnaJet> > _jets;
  std::vector< boost::shared_ptr<const AnaTruthParticle> > _truth_particles;
  std::vector< boost::shared_ptr<const AnaElectron> > _electrons;
  std::vector< boost::shared_ptr<const AnaPhoton> > _photons;
  std::vector< boost::shared_ptr<const AnaMuon> > _muons;
  std::vector< boost::shared_ptr<const AnaTrack> > _offline_tracks;
  std::vector< boost::shared_ptr<const AnaMET> > _met;
  boost::shared_ptr<AnaTrigger> _trigger;
  std::vector< boost::shared_ptr<const AnaVertex> > _vertices;

private:

  const bool _get_event_V29();
  const bool _get_event_V29g();
  const bool _get_event_V29i();

  template<typename ntT> void _make_associated_track_for_electron( boost::shared_ptr<AnaElectron>& mu , ntT nt , const unsigned int& i );
  template<typename ntT> void _make_associated_track_for_muon( boost::shared_ptr<AnaMuon>& mu , ntT nt , const unsigned int& i );
  
  template<typename iteratorT>
  const bool _check_trigger( iteratorT begin , iteratorT end , const char* trig_name ) const;

public:

  NtD3PDphysPeter()
    : _open_filename()
    , _n_events(0ul)
    , _run_number(0ul)
    , _event_number(0ul)
    , _lumi_block(0ul)
    , _average_mu(0.0)
    , _actual_mu(0.0)
    , _mc_weight(1.)
    , _mc_channel(0)
    , _mc_hfor(0)
    , _lar_error(false)
    , _bdff(0)
    , _mcevt_event_scale( 0 )
    , _mcevt_alphaQCD( 0 )
    , _mcevt_alphaQED( 0 )
    , _mcevt_pdf_scale( 0 )
    , _mcevt_id1( 0 )
    , _mcevt_id2( 0 )
    , _mcevt_pdf_x1( 0 )
    , _mcevt_pdf_x2( 0 )
    , _mcevt_pdf1( 0 )
    , _mcevt_pdf2( 0 )
    , _file()
    , _madeclass_version( RESERVED )
    , _madeclass()
    , _jets()
    , _truth_particles()
    , _electrons()
    , _photons()
    , _muons()
    , _offline_tracks()
    , _met()
    , _trigger()
    , _vertices()
    , NtD3PDphys(NtD3PDphys::PETER)
  {}
  virtual ~NtD3PDphysPeter() {
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
  virtual const double average_mu() const { return _average_mu; }
  virtual const double actual_mu() const { return _actual_mu; }
  virtual const ftype mc_weight() const { return _mc_weight; }
  virtual const std::vector<ftype>& pdf_weights() const { return _pdf_weights; }
  virtual const unsigned long mc_channel() const { return _mc_channel; }
  virtual const int mc_hfor() const { return _mc_hfor; }
  virtual const bool lar_error() const { return _lar_error; }
  virtual const int bdff() const { return _bdff; }

  virtual const float mcevt_event_scale() const { return _mcevt_event_scale; }
  virtual const float mcevt_alphaQCD() const { return _mcevt_alphaQCD; }
  virtual const float mcevt_alphaQED() const { return _mcevt_alphaQED; }
  virtual const float mcevt_pdf_scale() const { return _mcevt_pdf_scale; }
  virtual const int mcevt_id1() const { return _mcevt_id1; }
  virtual const int mcevt_id2() const { return _mcevt_id2; }
  virtual const float mcevt_pdf_x1() const { return _mcevt_pdf_x1; }
  virtual const float mcevt_pdf_x2() const { return _mcevt_pdf_x2; }
  virtual const float mcevt_pdf1() const { return _mcevt_pdf1; }
  virtual const float mcevt_pdf2() const { return _mcevt_pdf2; }

  virtual const unsigned long nbytes() const { // size of current event 
    return( 0 );
  }

  virtual const std::vector< boost::shared_ptr<const AnaTruthParticle> >& truth_particles() const { return _truth_particles; }
  virtual const std::vector< boost::shared_ptr<const AnaElectron> >& electrons() const { return _electrons; }
  virtual const std::vector< boost::shared_ptr<const AnaPhoton> >& photons() const { return _photons; }
  virtual const std::vector< boost::shared_ptr<const AnaMuon> >& muons() const { return _muons; }
  virtual const std::vector< boost::shared_ptr<const AnaTrack> >& offline_tracks() const { return _offline_tracks; }
  virtual const std::vector< boost::shared_ptr<const AnaJet> >& jets() const { return _jets; }
  virtual const std::vector< boost::shared_ptr<const AnaVertex> >& vertices() const { return _vertices; }
  virtual const std::vector< boost::shared_ptr<const AnaMET> >& missing_et() const { return _met; }
  virtual const boost::shared_ptr<const AnaTrigger> trigger() const { return boost::const_pointer_cast<const AnaTrigger>(_trigger); }

protected:

  // clear the current event
  void clear() { 
    _run_number = 0ul;
    _event_number = 0ul;
    _lumi_block = 0ul;
    _average_mu = 0.0;
    _actual_mu = 0.0;
    _mc_weight = 1.;
    _mc_channel = 0;
    _mc_hfor = 0;
    _lar_error = false;
    _bdff = 0;
    _mcevt_event_scale = 0;
    _mcevt_alphaQCD = 0;
    _mcevt_alphaQED = 0;
    _mcevt_pdf_scale = 0;
    _mcevt_id1 = 0;
    _mcevt_id2 = 0;
    _mcevt_pdf_x1 = 0;
    _mcevt_pdf_x2 = 0;
    _mcevt_pdf1 = 0;
    _mcevt_pdf2 = 0;
    _truth_particles.clear();
    _electrons.clear();
    _photons.clear();
    _muons.clear();
    _offline_tracks.clear();
    _jets.clear();
    _vertices.clear();
    _met.clear();
    _trigger.reset();
    assert( _truth_particles.empty() );
    assert( _electrons.empty() );
    assert( _photons.empty() );
    assert( _muons.empty() );
    assert( _offline_tracks.empty() );
    assert( _jets.empty() );
    assert( _vertices.empty() );
    assert( _met.empty() );
  }

  void set_branch_addresses();
  void set_enabled_branches();

};

#endif // WRAP_NTD3PDPHYSPETER_HPP
