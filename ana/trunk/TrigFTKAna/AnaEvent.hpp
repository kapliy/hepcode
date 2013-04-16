#ifndef WRAP_ANAEVENT_HPP
#define WRAP_ANAEVENT_HPP

#include "TrigFTKAna/AnaCommon.hpp"
#include <vector>
#include <map>
#include <set>
#include <numeric>
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/logic/tribool.hpp>
#include <boost/optional.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <boost/bind.hpp>
#include "TrigFTKAna/AnaTruthParticle.hpp"

class AnaRoad;
class AnaSuperstrip;
class AnaTrack;
class AnaElectron;
class AnaPhoton;
class AnaMuon;
class AnaJet;
class AnaMET;
class AnaTrigger;
class AnaSiChannel;
class AnaSiCluster;
class AnaBeamline;
class AnaVertex;

// interface to event data.

class
AnaEvent
{

public:
  
  typedef float ftype;

private:

  unsigned long _evt_mgr_index;
  std::string _sample_type;
  unsigned long _nbytes;
  unsigned long _run_number;
  unsigned long _event_number;
  unsigned long _lumi_block;
  double _average_mu;
  double _actual_mu;
  unsigned long _bunch_number;
  int _bdff;
  unsigned long _mc_channel;
  int _mc_hfor;
  ftype _mc_weight;
  std::vector<ftype> _pdf_weights;
  bool _lar_error;
  unsigned int _ftk_road_nplanes;

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

  std::set<std::string> _provenance; // e.g. ntuple filenames 

  typedef std::map< std::string , bool > SampleFlags;
  typedef std::map< std::string , double > SampleNumericFlags;
  typedef std::map< std::string , std::string > SampleStrings;
  typedef std::set< std::string > SampleTags;
  SampleFlags _sample_flags; // set by dataset definition.
  SampleNumericFlags _sample_numeric_flags; // set by dataset definition.
  SampleStrings _sample_strings; // set by dataset definition.
  SampleTags _sample_tags; // set by dataset definition.

  std::map< std::string , std::vector< boost::shared_ptr<const AnaTrack> > > _ftk_tracks;
  std::map< std::string , std::vector< boost::shared_ptr<const AnaTrack> > > _trigftklib_tracks;
  std::map< std::string , std::vector< boost::shared_ptr<const AnaTrack> > > _offline_tracks;
  std::map< std::string , std::vector< boost::shared_ptr<const AnaTrack> > > _truth_tracks;
  std::map< std::string , std::vector< boost::shared_ptr<const AnaTrack> > > _l2_idscan_tracks;
  std::map< std::string , std::vector< boost::shared_ptr<const AnaTrack> > > _l2_sitrack_tracks;
  std::map< std::string , std::vector< boost::shared_ptr<const AnaTruthParticle> > > _truth_particles;
  std::map< std::string , std::vector< boost::shared_ptr<const AnaElectron> > > _electrons;
  std::map< std::string , std::vector< boost::shared_ptr<const AnaPhoton> > > _photons;
  std::map< std::string , std::vector< boost::shared_ptr<const AnaMuon> > > _muons;
  std::map< std::string , std::vector< boost::shared_ptr<const AnaVertex> > > _vertices;
  std::map< std::string , boost::shared_ptr<const AnaVertex> > _highest_sum_pt_vertex; // const
  std::map< std::string , std::vector< boost::shared_ptr<const AnaMET> > > _met;
  std::map< std::string , boost::shared_ptr<const AnaTrigger> > _trigger; // const 
  std::map< std::string , boost::shared_ptr<const AnaBeamline> > _beamline; // const
  std::map< std::string , std::vector< boost::shared_ptr<const AnaJet> > > _atlas_anti_kt_4_h1_topo_jets;
  std::map< std::string , std::vector< boost::shared_ptr<const AnaJet> > > _atlas_anti_kt_6_h1_topo_jets;
  std::map< std::string , std::vector< boost::shared_ptr<const AnaJet> > > _atlas_anti_kt_4_h1_lc_topo_jets;
  std::map< std::string , std::vector< boost::shared_ptr<const AnaJet> > > _atlas_anti_kt_6_h1_lc_topo_jets;
  std::map< std::string , std::vector< boost::shared_ptr<const AnaJet> > > _atlas_met_goodness_jets;
  std::map< std::string , std::vector< boost::shared_ptr<const AnaJet> > > _l1_roi_jets;
  std::map< std::string , std::vector< boost::shared_ptr<const AnaJet> > > _atlas_cone_4_tower_jets;
  std::map< std::string , std::vector< boost::shared_ptr<const AnaJet> > > _atlas_cone_7_tower_jets;
  std::map< std::string , std::vector< boost::shared_ptr<const AnaJet> > > _atlas_cone_4_truth_jets;
  std::map< std::string , std::vector< boost::shared_ptr<const AnaJet> > > _atlas_cone_4_truth_pileup_jets;
  std::map< std::string , std::vector< boost::shared_ptr<const AnaJet> > > _atlas_cone_6_truth_jets;
  std::map< std::string , std::vector< boost::shared_ptr<const AnaJet> > > _atlas_cone_6_truth_pileup_jets;
  std::map< std::string , std::vector< boost::shared_ptr<const AnaJet> > > _pgs_jets;
  std::map< std::string , std::vector< boost::shared_ptr<const AnaRoad> > > _ftk_roads;
  std::map< std::string , std::vector< boost::shared_ptr<const AnaSuperstrip> > > _ftk_superstrips;
  std::map< std::string , std::vector< boost::shared_ptr<const AnaSiChannel> > > _si_channels;
  std::map< std::string , std::vector< boost::shared_ptr<const AnaSiCluster> > > _si_clusters;

  std::map< std::string , std::vector<unsigned long> > _ftk_n_tracks;
  std::map< std::string , std::vector<unsigned long> > _ftk_n_combinations;
  std::map< std::string , std::vector<unsigned long> > _ftk_n_fits;
  std::map< std::string , std::vector<unsigned long> > _ftk_n_fits_majority;
  std::map< std::string , std::vector<unsigned long> > _ftk_n_fits_bad;
  std::map< std::string , std::vector<unsigned long> > _ftk_n_fits_majority_bad;
  std::map< std::string , std::vector<unsigned long> > _ftk_n_fits_hitwarrior_rejected;

  std::map< std::string , std::vector<unsigned long> > _ftk_n_clusters;

private:
  
  // save memory to specialized DgMemory heap 
  DECLARE_DGHEAP;

protected:

  template<typename objectT>
  std::vector<objectT>& get_from_source( const std::string& source , std::map< std::string , std::vector<objectT> >& object_map ) {
    return object_map[source]; // create if it doesn't exist
  }
  template<typename objectT>
  objectT& get_from_source( const std::string& source , std::map< std::string , objectT >& object_map ) {
    return object_map[source]; // create if it doesn't exist
  }
  template<typename objectT>
  const std::vector<objectT>& get_from_source( const std::string& source , const std::map< std::string , std::vector<objectT> >& object_map ) const {
    static std::vector<objectT> null_vector;
    typename std::map< std::string , std::vector<objectT> >::const_iterator i=object_map.find(source);
    if( i==object_map.end() ) { return null_vector; }
    return i->second;
  }
  template<typename objectT>
  const objectT& get_from_source( const std::string& source , const std::map< std::string , objectT >& object_map ) const {
    static objectT null_object;
    typename std::map< std::string , objectT >::const_iterator i=object_map.find(source);
    if( i==object_map.end() ) { return null_object; }
    return i->second;
  }
  
protected:

  std::vector< boost::shared_ptr<const AnaTrack> >& ftk_tracks( const std::string& source ) { return get_from_source(source,_ftk_tracks); } // ftk tracks from nominal simulation
  std::vector< boost::shared_ptr<const AnaTrack> >& trigftklib_tracks( const std::string& source ) { return get_from_source(source,_trigftklib_tracks); } // ftk tracks from old TrigFTKLib simulation
  std::vector< boost::shared_ptr<const AnaTrack> >& offline_tracks( const std::string& source ) { return get_from_source(source,_offline_tracks); }
  std::vector< boost::shared_ptr<const AnaTrack> >& truth_tracks( const std::string& source ) { return get_from_source(source,_truth_tracks); }
  std::vector< boost::shared_ptr<const AnaTrack> >& l2_idscan_tracks( const std::string& source ) { return get_from_source(source,_l2_idscan_tracks); }
  std::vector< boost::shared_ptr<const AnaTrack> >& l2_sitrack_tracks( const std::string& source ) { return get_from_source(source,_l2_sitrack_tracks); }
  std::vector< boost::shared_ptr<const AnaVertex> >& vertices( const std::string& source ) { return get_from_source(source,_vertices); }
  std::vector< boost::shared_ptr<const AnaJet> >& atlas_anti_kt_4_h1_topo_jets( const std::string& source ) { return get_from_source(source,_atlas_anti_kt_4_h1_topo_jets); }
  std::vector< boost::shared_ptr<const AnaJet> >& atlas_anti_kt_6_h1_topo_jets( const std::string& source ) { return get_from_source(source,_atlas_anti_kt_6_h1_topo_jets); }
  std::vector< boost::shared_ptr<const AnaJet> >& atlas_anti_kt_4_h1_lc_topo_jets( const std::string& source ) { return get_from_source(source,_atlas_anti_kt_4_h1_lc_topo_jets); }
  std::vector< boost::shared_ptr<const AnaJet> >& atlas_anti_kt_6_h1_lc_topo_jets( const std::string& source ) { return get_from_source(source,_atlas_anti_kt_6_h1_lc_topo_jets); }
  std::vector< boost::shared_ptr<const AnaJet> >& atlas_met_goodness_jets( const std::string& source ) { return get_from_source(source,_atlas_met_goodness_jets); }
  std::vector< boost::shared_ptr<const AnaJet> >& l1_roi_jets( const std::string& source ) { return get_from_source(source,_l1_roi_jets); }
  std::vector< boost::shared_ptr<const AnaJet> >& atlas_cone_4_tower_jets( const std::string& source ) { return get_from_source(source,_atlas_cone_4_tower_jets); }
  std::vector< boost::shared_ptr<const AnaJet> >& atlas_cone_7_tower_jets( const std::string& source ) { return get_from_source(source,_atlas_cone_7_tower_jets); }
  std::vector< boost::shared_ptr<const AnaJet> >& atlas_cone_4_truth_jets( const std::string& source ) { return get_from_source(source,_atlas_cone_4_truth_jets); }
  std::vector< boost::shared_ptr<const AnaJet> >& atlas_cone_4_truth_pileup_jets( const std::string& source ) { return get_from_source(source,_atlas_cone_4_truth_pileup_jets); }
  std::vector< boost::shared_ptr<const AnaJet> >& atlas_cone_6_truth_jets( const std::string& source ) { return get_from_source(source,_atlas_cone_6_truth_jets); }
  std::vector< boost::shared_ptr<const AnaJet> >& atlas_cone_6_truth_pileup_jets( const std::string& source ) { return get_from_source(source,_atlas_cone_6_truth_pileup_jets); }
  std::vector< boost::shared_ptr<const AnaJet> >& pgs_jets( const std::string& source ) { return get_from_source(source,_pgs_jets); }
  std::vector< boost::shared_ptr<const AnaTruthParticle> >& truth_particles( const std::string& source ) { return get_from_source(source,_truth_particles); };
  std::vector< boost::shared_ptr<const AnaElectron> >& electrons( const std::string& source ) { return get_from_source(source,_electrons); };
  std::vector< boost::shared_ptr<const AnaPhoton> >& photons( const std::string& source ) { return get_from_source(source,_photons); };
  std::vector< boost::shared_ptr<const AnaMuon> >& muons( const std::string& source ) { return get_from_source(source,_muons); };
  boost::shared_ptr<const AnaVertex>& highest_sum_pt_vertex( const std::string& source ) { return get_from_source(source,_highest_sum_pt_vertex); };
  std::vector< boost::shared_ptr<const AnaMET> >& missing_et( const std::string& source ) { return get_from_source(source,_met); };
  boost::shared_ptr<const AnaTrigger>& trigger( const std::string& source ) { return get_from_source(source,_trigger); };
  boost::shared_ptr<const AnaBeamline>& beamline( const std::string& source ) { return get_from_source(source,_beamline); };
  std::vector< boost::shared_ptr<const AnaRoad> >& ftk_roads( const std::string& source ) { return get_from_source(source,_ftk_roads); } // ftk roads from nominal simulation
  std::vector< boost::shared_ptr<const AnaSuperstrip> >& ftk_superstrips( const std::string& source ) { return get_from_source(source,_ftk_superstrips); } // ftk roads from nominal simulation
  std::vector< boost::shared_ptr<const AnaSiChannel> >& si_channels( const std::string& source ) { return get_from_source(source,_si_channels); } // silicon readout objects from athena
  std::vector< boost::shared_ptr<const AnaSiCluster> >& si_clusters( const std::string& source ) { return get_from_source(source,_si_clusters); } // silicon readout objects from athena

  std::vector<unsigned long>& ftk_n_tracks_each( const std::string& source ) { return get_from_source(source,_ftk_n_tracks); }
  std::vector<unsigned long>& ftk_n_combinations_each( const std::string& source ) { return get_from_source(source,_ftk_n_combinations); }
  std::vector<unsigned long>& ftk_n_fits_each( const std::string& source ) { return get_from_source(source,_ftk_n_fits); }
  std::vector<unsigned long>& ftk_n_fits_majority_each( const std::string& source ) { return get_from_source(source,_ftk_n_fits_majority); }
  std::vector<unsigned long>& ftk_n_fits_bad_each( const std::string& source ) { return get_from_source(source,_ftk_n_fits_bad); }
  std::vector<unsigned long>& ftk_n_fits_majority_bad_each( const std::string& source ) { return get_from_source(source,_ftk_n_fits_majority_bad); }
  std::vector<unsigned long>& ftk_n_fits_hitwarrior_rejected_each( const std::string& source ) { return get_from_source(source,_ftk_n_fits_hitwarrior_rejected); }
  std::vector<unsigned long>& ftk_n_clusters( const std::string& source ) { return get_from_source(source,_ftk_n_clusters); }
  
  friend class AnaEventMgr;

public:

  AnaEvent()
    : _sample_type("")
    , _evt_mgr_index( 0ul )
    , _nbytes( 0ul )
    , _run_number( 0ul )
    , _event_number( 0ul )
    , _lumi_block( 0ul )
    , _average_mu( 0.0 )
    , _actual_mu( 0.0 )
    , _bunch_number( 0ul )
    , _bdff( 0 )
    , _mc_weight( 1. )
    , _mc_channel( 0 )
    , _mc_hfor( 0 )
    , _lar_error( false )
    , _ftk_road_nplanes( 0 )
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
    , _provenance()
    , _sample_flags()
    , _sample_numeric_flags()
    , _sample_strings()
    , _sample_tags()
    , inconsistentCBNT(false)
  {}
  virtual ~AnaEvent() {}

  static const std::string DEFAULT_SOURCE;
  bool inconsistentCBNT; // Used to work around athena/CBNT problem

  const std::string sample_type() const { return _sample_type; }

  const unsigned long run_number() const { return _run_number; }
  const unsigned long event_number() const { return _event_number; }
  const unsigned long lumi_block() const { return _lumi_block; }
  const double average_mu() const { return _average_mu; }
  const double actual_mu() const { return _actual_mu; }
  const unsigned long bunch_number() const { return _bunch_number; }
  const int bdff() const { return _bdff; }
  const int bunch_distance_from_front() const { return _bdff; }
  const ftype mc_weight() const { return _mc_weight; }
  const std::vector<ftype>& pdf_weights() const { return _pdf_weights; }
  const ftype pdf_weight(int idx) const { return idx<_pdf_weights.size() ? _pdf_weights[idx] : 1.0; }
  const unsigned long mc_channel() const { return _mc_channel; }
  const int mc_hfor() const { return _mc_hfor; }
  const ftype lar_error() const { return _lar_error; }
  const unsigned int ftk_road_nplanes() const { return _ftk_road_nplanes; }
  const unsigned long nbytes() const { return _nbytes; }

  const float mcevt_event_scale() const { return _mcevt_event_scale; }
  const float mcevt_alphaQCD() const { return _mcevt_alphaQCD; }
  const float mcevt_alphaQED() const { return _mcevt_alphaQED; }
  const float mcevt_pdf_scale() const { return _mcevt_pdf_scale; }
  const int mcevt_id1() const { return _mcevt_id1; }
  const int mcevt_id2() const { return _mcevt_id2; }
  const float mcevt_pdf_x1() const { return _mcevt_pdf_x1; }
  const float mcevt_pdf_x2() const { return _mcevt_pdf_x2; }
  const float mcevt_pdf1() const { return _mcevt_pdf1; }
  const float mcevt_pdf2() const { return _mcevt_pdf2; }
  
  const std::set<std::string> provenance() const { return _provenance; }
  const std::string provenance_first_name() const { return _provenance.empty() ? "" : *_provenance.begin(); }
  const unsigned long event_manager_index() const { return _evt_mgr_index; }

  typedef boost::logic::tribool tribool;
  const tribool check_flag( std::string flag_name ) const {
    boost::algorithm::to_lower( flag_name );
    SampleFlags::const_iterator i = _sample_flags.find( flag_name );
    if( i!=_sample_flags.end() ) { return tribool(i->second); }
    return tribool(boost::logic::indeterminate); 
  }
  const boost::optional<double> check_numeric_flag( std::string flag_name ) const {
    boost::algorithm::to_lower( flag_name );
    SampleNumericFlags::const_iterator i = _sample_numeric_flags.find( flag_name );
    if( i!=_sample_numeric_flags.end() ) { 
      return boost::optional<double>( i->second );
    }
    static boost::optional<double> null;
    return null;
  }
  const boost::optional<std::string> check_strings( std::string flag_name ) const {
    boost::algorithm::to_lower( flag_name );
    SampleStrings::const_iterator i = _sample_strings.find( flag_name );
    if( i!=_sample_strings.end() ) { 
      return boost::optional<std::string>( i->second );
    }
    static boost::optional<std::string> null;
    return null;
  }

  const bool check_tag( std::string tag_name ) const {
    boost::algorithm::to_lower( tag_name );
    return( _sample_tags.find( tag_name )!=_sample_tags.end() );
  }
  
  const std::vector< boost::shared_ptr<const AnaTrack> >& ftk_tracks( const std::string source = AnaEvent::DEFAULT_SOURCE ) const { 
    return get_from_source(source,_ftk_tracks); 
  }
  const std::vector< boost::shared_ptr<const AnaTrack> >& trigftklib_tracks( const std::string source = AnaEvent::DEFAULT_SOURCE ) const { 
    return get_from_source(source,_trigftklib_tracks); 
  }
  const std::vector< boost::shared_ptr<const AnaTrack> >& offline_tracks( const std::string source = AnaEvent::DEFAULT_SOURCE ) const { 
    return get_from_source(source,_offline_tracks); 
  }
  const std::vector< boost::shared_ptr<const AnaTrack> >& truth_tracks( const std::string source = AnaEvent::DEFAULT_SOURCE ) const { 
    return get_from_source(source,_truth_tracks); 
  }
  const std::vector< boost::shared_ptr<const AnaTrack> >& l2_idscan_tracks( const std::string source = AnaEvent::DEFAULT_SOURCE ) const { 
    return get_from_source(source,_l2_idscan_tracks); 
  }
  const std::vector< boost::shared_ptr<const AnaTrack> >& l2_sitrack_tracks( const std::string source = AnaEvent::DEFAULT_SOURCE ) const { 
    return get_from_source(source,_l2_sitrack_tracks); 
  }
  const std::vector< boost::shared_ptr<const AnaVertex> >& vertices( const std::string& source) const {
    return get_from_source(source,_vertices); 
  }
  const std::vector< boost::shared_ptr<const AnaVertex> >& vertices() const {return get_from_source(AnaEvent::DEFAULT_SOURCE,_vertices); }
  const std::vector< boost::shared_ptr<const AnaJet> >& atlas_anti_kt_4_h1_topo_jets() const {
    return get_from_source(AnaEvent::DEFAULT_SOURCE,_atlas_anti_kt_4_h1_topo_jets); 
  }
  const std::vector< boost::shared_ptr<const AnaJet> >& atlas_anti_kt_4_h1_topo_jets( const std::string& source) const {
    return get_from_source(source,_atlas_anti_kt_4_h1_topo_jets); 
  }
  const std::vector< boost::shared_ptr<const AnaJet> >& atlas_anti_kt_6_h1_topo_jets( const std::string& source = AnaEvent::DEFAULT_SOURCE ) const {
    return get_from_source(source,_atlas_anti_kt_6_h1_topo_jets); 
  }
  const std::vector< boost::shared_ptr<const AnaJet> >& atlas_anti_kt_4_h1_lc_topo_jets( const std::string& source = AnaEvent::DEFAULT_SOURCE ) const {
    return get_from_source(source,_atlas_anti_kt_4_h1_lc_topo_jets); 
  }
  const std::vector< boost::shared_ptr<const AnaJet> >& atlas_anti_kt_6_h1_lc_topo_jets( const std::string& source = AnaEvent::DEFAULT_SOURCE ) const {
    return get_from_source(source,_atlas_anti_kt_6_h1_lc_topo_jets); 
  }
  const std::vector< boost::shared_ptr<const AnaJet> >& atlas_met_goodness_jets( const std::string& source = AnaEvent::DEFAULT_SOURCE ) const {
    return get_from_source(source,_atlas_met_goodness_jets); 
  }
  const std::vector< boost::shared_ptr<const AnaJet> >& l1_roi_jets( const std::string& source = AnaEvent::DEFAULT_SOURCE ) const { 
    return get_from_source(source,_l1_roi_jets); 
  }
  const std::vector< boost::shared_ptr<const AnaJet> >& atlas_cone_4_tower_jets( const std::string& source = AnaEvent::DEFAULT_SOURCE ) const { 
    return get_from_source(source,_atlas_cone_4_tower_jets); 
  }
  const std::vector< boost::shared_ptr<const AnaJet> >& atlas_cone_7_tower_jets( const std::string& source = AnaEvent::DEFAULT_SOURCE ) const { 
    return get_from_source(source,_atlas_cone_7_tower_jets); 
  }
  const std::vector< boost::shared_ptr<const AnaJet> >& atlas_cone_4_truth_jets( const std::string& source = AnaEvent::DEFAULT_SOURCE ) const { 
    return get_from_source(source,_atlas_cone_4_truth_jets); 
  }
  const std::vector< boost::shared_ptr<const AnaJet> >& atlas_cone_4_truth_pileup_jets( const std::string& source = AnaEvent::DEFAULT_SOURCE ) const { 
    return get_from_source(source,_atlas_cone_4_truth_pileup_jets); 
  }
  const std::vector< boost::shared_ptr<const AnaJet> >& atlas_cone_6_truth_jets( const std::string& source = AnaEvent::DEFAULT_SOURCE ) const { 
    return get_from_source(source,_atlas_cone_6_truth_jets); 
  }
  const std::vector< boost::shared_ptr<const AnaJet> >& atlas_cone_6_truth_pileup_jets( const std::string& source = AnaEvent::DEFAULT_SOURCE ) const { 
    return get_from_source(source,_atlas_cone_6_truth_pileup_jets); 
  }
  const std::vector< boost::shared_ptr<const AnaJet> >& pgs_jets( const std::string& source = AnaEvent::DEFAULT_SOURCE ) const { 
    return get_from_source(source,_pgs_jets); 
  }
  const std::vector< boost::shared_ptr<const AnaElectron> >& electrons( const std::string& source = AnaEvent::DEFAULT_SOURCE ) const { 
    return get_from_source(source,_electrons);
  }
  const std::vector< boost::shared_ptr<const AnaPhoton> >& photons( const std::string& source = AnaEvent::DEFAULT_SOURCE ) const { 
    return get_from_source(source,_photons);
  }
  const std::vector< boost::shared_ptr<const AnaMuon> >& muons( const std::string& source = AnaEvent::DEFAULT_SOURCE ) const { 
    return get_from_source(source,_muons);
  }
  boost::shared_ptr<const AnaVertex> const& highest_sum_pt_vertex( const std::string& source ) const { 
    return get_from_source(source,_highest_sum_pt_vertex);
  }
  boost::shared_ptr<const AnaVertex> const& highest_sum_pt_vertex( ) const { 
    return get_from_source(AnaEvent::DEFAULT_SOURCE,_highest_sum_pt_vertex);
  }
  const std::vector< boost::shared_ptr<const AnaMET> >& missing_et( const std::string& source = AnaEvent::DEFAULT_SOURCE ) const { 
    return get_from_source(source,_met);
  }
  boost::shared_ptr<const AnaTrigger> const& trigger( const std::string& source ) const { 
    return get_from_source(source,_trigger);
  }
  boost::shared_ptr<const AnaTrigger> const& trigger() const { return get_from_source(AnaEvent::DEFAULT_SOURCE,_trigger); }
  boost::shared_ptr<const AnaBeamline> const& beamline( const std::string& source ) const { 
    return get_from_source(source,_beamline);
  }
  boost::shared_ptr<const AnaBeamline> const& beamline() const { return get_from_source(AnaEvent::DEFAULT_SOURCE,_beamline); }
  const std::vector< boost::shared_ptr<const AnaTruthParticle> >& truth_particles( const std::string& source = AnaEvent::DEFAULT_SOURCE ) const { 
    return get_from_source(source,_truth_particles);
  }
  const std::vector< boost::shared_ptr<const AnaRoad> >& ftk_roads( const std::string source = AnaEvent::DEFAULT_SOURCE ) const { 
    return get_from_source(source,_ftk_roads); 
  }
  const std::vector< boost::shared_ptr<const AnaSuperstrip> >& ftk_superstrips( const std::string source = AnaEvent::DEFAULT_SOURCE ) const { 
    return get_from_source(source,_ftk_superstrips); 
  }
  const std::vector< boost::shared_ptr<const AnaSiChannel> >& si_channels( const std::string source = AnaEvent::DEFAULT_SOURCE ) const { 
    return get_from_source(source,_si_channels); 
  }
  const std::vector< boost::shared_ptr<const AnaSiCluster> >& si_clusters( const std::string source = AnaEvent::DEFAULT_SOURCE ) const { 
    return get_from_source(source,_si_clusters); 
  }
  const std::vector<unsigned long>& ftk_n_tracks_each( const std::string source = AnaEvent::DEFAULT_SOURCE ) const { 
    return get_from_source(source,_ftk_n_tracks); 
  }
  const std::vector<unsigned long>& ftk_n_combinations_each( const std::string source = AnaEvent::DEFAULT_SOURCE ) const { 
    return get_from_source(source,_ftk_n_combinations); 
  }
  const std::vector<unsigned long>& ftk_n_fits_each( const std::string source = AnaEvent::DEFAULT_SOURCE ) const { 
    return get_from_source(source,_ftk_n_fits); 
  }
  const std::vector<unsigned long>& ftk_n_fits_majority_each( const std::string source = AnaEvent::DEFAULT_SOURCE ) const { 
    return get_from_source(source,_ftk_n_fits_majority); 
  }
  const std::vector<unsigned long>& ftk_n_fits_bad_each( const std::string source = AnaEvent::DEFAULT_SOURCE ) const { 
    return get_from_source(source,_ftk_n_fits_bad); 
  }
  const std::vector<unsigned long>& ftk_n_fits_majority_bad_each( const std::string source = AnaEvent::DEFAULT_SOURCE ) const { 
    return get_from_source(source,_ftk_n_fits_majority_bad); 
  }
  const std::vector<unsigned long>& ftk_n_fits_hitwarrior_rejected_each( const std::string source = AnaEvent::DEFAULT_SOURCE ) const { 
    return get_from_source(source,_ftk_n_fits_hitwarrior_rejected); 
  }
  const std::vector<unsigned long>& ftk_n_clusters( const std::string source = AnaEvent::DEFAULT_SOURCE ) const { 
    return get_from_source(source,_ftk_n_clusters); 
  }

  const unsigned long ftk_n_tracks( const std::string source = AnaEvent::DEFAULT_SOURCE ) const {
    const std::vector<unsigned long> vec = ( get_from_source(source,_ftk_n_tracks) );
    return std::accumulate( vec.begin() , vec.end() , 0ul ); 
  }
  const unsigned long ftk_n_combinations( const std::string source = AnaEvent::DEFAULT_SOURCE ) const {
    const std::vector<unsigned long>& vec( get_from_source(source,_ftk_n_combinations) );
    return std::accumulate( vec.begin() , vec.end() , 0ul ); 
  }
  const unsigned long ftk_n_fits( const std::string source = AnaEvent::DEFAULT_SOURCE ) const {
    const std::vector<unsigned long>& vec( get_from_source(source,_ftk_n_fits) );
    return std::accumulate( vec.begin() , vec.end() , 0ul ); 
  }
  const unsigned long ftk_n_fits_majority( const std::string source = AnaEvent::DEFAULT_SOURCE ) const {
    const std::vector<unsigned long>& vec( get_from_source(source,_ftk_n_fits_majority) );
    return std::accumulate( vec.begin() , vec.end() , 0ul ); 
  }
  const unsigned long ftk_n_fits_bad( const std::string source = AnaEvent::DEFAULT_SOURCE ) const {
    const std::vector<unsigned long>& vec( get_from_source(source,_ftk_n_fits_bad) );
    return std::accumulate( vec.begin() , vec.end() , 0ul ); 
  }
  const unsigned long ftk_n_fits_majority_bad( const std::string source = AnaEvent::DEFAULT_SOURCE ) const {
    const std::vector<unsigned long>& vec( get_from_source(source,_ftk_n_fits_majority_bad) );
    return std::accumulate( vec.begin() , vec.end() , 0ul ); 
  }
  const unsigned long ftk_n_fits_hitwarrior_rejected( const std::string source = AnaEvent::DEFAULT_SOURCE ) const {
    const std::vector<unsigned long>& vec( get_from_source(source,_ftk_n_fits_hitwarrior_rejected) );
    return std::accumulate( vec.begin() , vec.end() , 0ul ); 
  }

  unsigned int nprimary_vertices() const;
  
};

#endif // WRAP_ANAEVENT_HPP
