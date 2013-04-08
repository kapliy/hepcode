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
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/serialization/version.hpp>
#include "TrigFTKAna/Trees/NtRoadBankLookup.hpp"

#include "TrigFTKAna/AnaTruthTable.hpp"
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
class AnaTruthTable;
class AnaSiChannel;
class AnaSiCluster;
class AnaBeamline;
class AnaVertex;
class SiModuleList;

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
  std::map< std::string , boost::shared_ptr<const AnaTruthTable> > _truth_table; 
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
  std::map< std::string , NtRoadBankLookup > _ftk_road_lookup;
  std::map< std::string , std::vector< boost::shared_ptr<const AnaSuperstrip> > > _ftk_superstrips;
  std::map< std::string , NtRoadLayerLookup > _ftk_ss_lookup;
  std::map< std::string , std::vector< boost::shared_ptr<const AnaSiChannel> > > _si_channels;
  std::map< std::string , std::vector< boost::shared_ptr<const AnaSiCluster> > > _si_clusters;
  std::map< std::string , boost::shared_ptr<const SiModuleList> > _bad_modules;

  std::map< std::string , std::vector<unsigned long> > _ftk_n_tracks;
  std::map< std::string , std::vector<unsigned long> > _ftk_n_combinations;
  std::map< std::string , std::vector<unsigned long> > _ftk_n_fits;
  std::map< std::string , std::vector<unsigned long> > _ftk_n_fits_majority;
  std::map< std::string , std::vector<unsigned long> > _ftk_n_fits_bad;
  std::map< std::string , std::vector<unsigned long> > _ftk_n_fits_majority_bad;
  std::map< std::string , std::vector<unsigned long> > _ftk_n_fits_hitwarrior_rejected;

  std::map< std::string , std::vector<unsigned long> > _ftk_n_clusters;

private:
  
  // note: as of this writing, the most recent version of BOOST
  // available in the ATLAS software (via the LCG Applications Area)
  // is 1.39. that version is 1) incompatible with DgMemory and 2)
  // with the serialization (load) of STL collections of const
  // pointers. thus, the following workaround for 2): split the
  // serialize function into load and save functions and load the
  // collections of const pointers into a temporary, then convert them
  // to const and put them in the collection. when a newer version of
  // BOOST is available on grid nodes or on your local machine, define
  // HAVE_BOOST_SERIALIZATION_1_40_OR_LATER for better performance.

#ifdef HAVE_BOOST_SERIALIZATION_1_40_OR_LATER
  friend class boost::serialization::access;
  // template<class T> T& override_const( const T& t ) const { const_cast<T&>(t); }
#define override_const(a) a

  template<class archiveT>
  void serialize( archiveT& ar, const unsigned int version )
  {
    // header
    ar & boost::serialization::make_nvp("sample_type",override_const(_sample_type));
    ar & boost::serialization::make_nvp("evt_mgr_index",override_const(_evt_mgr_index));
    ar & boost::serialization::make_nvp("nbytes",override_const(_nbytes));
    ar & boost::serialization::make_nvp("run_number",override_const(_run_number));
    ar & boost::serialization::make_nvp("event_number",override_const(_event_number));
    ar & boost::serialization::make_nvp("lumi_block",override_const(_lumi_block));
    if( version>=7) {
      ar & boost::serialization::make_nvp("average_mu",override_const(_average_mu));
      ar & boost::serialization::make_nvp("actual_mu",override_const(_actual_mu));
    }
    ar & boost::serialization::make_nvp("ftk_road_nplanes",override_const(_ftk_road_nplanes));
    if ( version>=8) {
      ar & boost::serialization::make_nvp("mcevt_event_scale",override_const(_mcevt_event_scale));
      ar & boost::serialization::make_nvp("mcevt_alphaQCD",override_const(_mcevt_alphaQCD));
      ar & boost::serialization::make_nvp("mcevt_alphaQED",override_const(_mcevt_alphaQED));
      ar & boost::serialization::make_nvp("mcevt_pdf_scale",override_const(_mcevt_pdf_scale));
      ar & boost::serialization::make_nvp("mcevt_id1",override_const(_mcevt_id1));
      ar & boost::serialization::make_nvp("mcevt_id2",override_const(_mcevt_id2));
      ar & boost::serialization::make_nvp("mcevt_pdf_x1",override_const(_mcevt_pdf_x1));
      ar & boost::serialization::make_nvp("mcevt_pdf_x2",override_const(_mcevt_pdf_x2));
      ar & boost::serialization::make_nvp("mcevt_pdf1",override_const(_mcevt_pdf1));
      ar & boost::serialization::make_nvp("mcevt_pdf2",override_const(_mcevt_pdf2));
    }
    ar & boost::serialization::make_nvp("provenance",override_const(_provenance));
    ar & boost::serialization::make_nvp("sample_flags",override_const(_sample_flags));
    ar & boost::serialization::make_nvp("sample_numeric_flags",override_const(_sample_numeric_flags));
    if( version>=4 ) {
      ar & boost::serialization::make_nvp("sample_strings",override_const(_sample_strings));
      ar & boost::serialization::make_nvp("sample_tags",override_const(_sample_tags));
    }
    // data
    ar & boost::serialization::make_nvp("ftk_tracks",override_const(_ftk_tracks));
    ar & boost::serialization::make_nvp("trigftklib_tracks",override_const(_trigftklib_tracks));
    ar & boost::serialization::make_nvp("offline_tracks",override_const(_offline_tracks));
    ar & boost::serialization::make_nvp("truth_tracks",override_const(_truth_tracks));
    ar & boost::serialization::make_nvp("l2_idscan_tracks",override_const(_l2_idscan_tracks));
    ar & boost::serialization::make_nvp("l2_sitrack_tracks",override_const(_l2_sitrack_tracks));
    ar & boost::serialization::make_nvp("truth_particles",override_const(_truth_particles));
    ar & boost::serialization::make_nvp("electrons",override_const(_electrons));
    ar & boost::serialization::make_nvp("photons",override_const(_photons));
    ar & boost::serialization::make_nvp("muons",override_const(_muons));
    ar & boost::serialization::make_nvp("vertices",override_const(_vertices));
    ar & boost::serialization::make_nvp("highest_sum_pt_vertex",override_const(_highest_sum_pt_vertex));
    ar & boost::serialization::make_nvp("met",override_const(_met));
    ar & boost::serialization::make_nvp("trigger",override_const(_trigger));
    ar & boost::serialization::make_nvp("beamline",override_const(_beamline));
    ar & boost::serialization::make_nvp("truth_table",override_const(_truth_table));
    ar & boost::serialization::make_nvp("atlas_anti_kt_4_h1_topo_jets",override_const(_atlas_anti_kt_4_h1_topo_jets));
    ar & boost::serialization::make_nvp("atlas_anti_kt_6_h1_topo_jets",override_const(_atlas_anti_kt_6_h1_topo_jets));
    ar & boost::serialization::make_nvp("atlas_anti_kt_4_h1_lc_topo_jets",override_const(_atlas_anti_kt_4_h1_lc_topo_jets));
    ar & boost::serialization::make_nvp("atlas_anti_kt_6_h1_lc_topo_jets",override_const(_atlas_anti_kt_6_h1_lc_topo_jets));
    ar & boost::serialization::make_nvp("atlas_met_goodness_jets",override_const(_atlas_met_goodness_jets));
    ar & boost::serialization::make_nvp("l1_roi_jets",override_const(_l1_roi_jets));
    ar & boost::serialization::make_nvp("atlas_cone_4_tower_jets",override_const(_atlas_cone_4_tower_jets));
    ar & boost::serialization::make_nvp("atlas_cone_7_tower_jets",override_const(_atlas_cone_7_tower_jets));
    ar & boost::serialization::make_nvp("atlas_cone_4_truth_jets",override_const(_atlas_cone_4_truth_jets));
    ar & boost::serialization::make_nvp("atlas_cone_4_truth_pileup_jets",override_const(_atlas_cone_4_truth_pileup_jets));
    ar & boost::serialization::make_nvp("atlas_cone_6_truth_jets",override_const(_atlas_cone_6_truth_jets));
    ar & boost::serialization::make_nvp("atlas_cone_6_truth_pileup_jets",override_const(_atlas_cone_6_truth_pileup_jets));
    ar & boost::serialization::make_nvp("pgs_jets",override_const(_pgs_jets));
    //
    ar & boost::serialization::make_nvp("ftk_roads",override_const(_ftk_roads));
    ar & boost::serialization::make_nvp("ftk_superstrips",override_const(_ftk_superstrips));
    ar & boost::serialization::make_nvp("si_channels",override_const(_si_channels));
    ar & boost::serialization::make_nvp("si_clusters",override_const(_si_clusters));
    ar & boost::serialization::make_nvp("bad_modules",override_const(bad_modules));
    //
    ar & boost::serialization::make_nvp("ftk_n_tracks",override_const(_ftk_n_tracks));
    ar & boost::serialization::make_nvp("ftk_n_combinations",override_const(_ftk_n_combinations));
    ar & boost::serialization::make_nvp("ftk_n_fits",override_const(_ftk_n_fits));
    ar & boost::serialization::make_nvp("ftk_n_fits_majority",override_const(_ftk_n_fits_majority));
    ar & boost::serialization::make_nvp("ftk_n_fits_bad",override_const(_ftk_n_fits_bad));
    ar & boost::serialization::make_nvp("ftk_n_fits_majority_bad",override_const(_ftk_n_fits_majority_bad));
    ar & boost::serialization::make_nvp("ftk_n_fits_hitwarrior_rejected",override_const(_ftk_n_fits_hitwarrior_rejected));
    ar & boost::serialization::make_nvp("ftk_n_clusters",override_const(_ftk_n_clusters));
    ar & boost::serialization::make_nvp("bunch_number",override_const(_bunch_number));
    ar & boost::serialization::make_nvp("bdff",override_const(_bdff));
    ar & boost::serialization::make_nvp("mc_weight",override_const(_mc_weight));
    ar & boost::serialization::make_nvp("pdf_weights",override_const(_pdf_weights));
    ar & boost::serialization::make_nvp("lar_error",override_const(_lar_error));
    ar & boost::serialization::make_nvp("mc_channel",override_const(_mc_channel));
    ar & boost::serialization::make_nvp("mc_hfor",override_const(_mc_hfor));
  }
#else
  friend class boost::serialization::access;
  template<class archiveT>
  void save( archiveT & ar, const unsigned int version) const {
    // header
    ar & boost::serialization::make_nvp("sample_type",_sample_type);
    ar & boost::serialization::make_nvp("evt_mgr_index",_evt_mgr_index);
    ar & boost::serialization::make_nvp("nbytes",_nbytes);
    ar & boost::serialization::make_nvp("run_number",_run_number);
    ar & boost::serialization::make_nvp("event_number",_event_number);
    ar & boost::serialization::make_nvp("lumi_block",_lumi_block);
    if (version>=7) {
      ar & boost::serialization::make_nvp("average_mu",_average_mu);
      ar & boost::serialization::make_nvp("actual_mu",_actual_mu);
    }
    ar & boost::serialization::make_nvp("ftk_road_nplanes",_ftk_road_nplanes);
    if ( version>=8) {
      ar & boost::serialization::make_nvp("mcevt_event_scale",_mcevt_event_scale);
      ar & boost::serialization::make_nvp("mcevt_alphaQCD",_mcevt_alphaQCD);
      ar & boost::serialization::make_nvp("mcevt_alphaQED",_mcevt_alphaQED);
      ar & boost::serialization::make_nvp("mcevt_pdf_scale",_mcevt_pdf_scale);
      ar & boost::serialization::make_nvp("mcevt_id1",_mcevt_id1);
      ar & boost::serialization::make_nvp("mcevt_id2",_mcevt_id2);
      ar & boost::serialization::make_nvp("mcevt_pdf_x1",_mcevt_pdf_x1);
      ar & boost::serialization::make_nvp("mcevt_pdf_x2",_mcevt_pdf_x2);
      ar & boost::serialization::make_nvp("mcevt_pdf1",_mcevt_pdf1);
      ar & boost::serialization::make_nvp("mcevt_pdf2",_mcevt_pdf2);
    }
    ar & boost::serialization::make_nvp("provenance",_provenance);
    ar & boost::serialization::make_nvp("sample_flags",_sample_flags);
    ar & boost::serialization::make_nvp("sample_numeric_flags",_sample_numeric_flags);
    if( version>=4 ) {
      ar & boost::serialization::make_nvp("sample_strings",_sample_strings);
      ar & boost::serialization::make_nvp("sample_tags",_sample_tags);
    }
    // data
    ar & boost::serialization::make_nvp("ftk_tracks",_ftk_tracks);
    ar & boost::serialization::make_nvp("trigftklib_tracks",_trigftklib_tracks);
    ar & boost::serialization::make_nvp("offline_tracks",_offline_tracks);
    ar & boost::serialization::make_nvp("truth_tracks",_truth_tracks);
    ar & boost::serialization::make_nvp("l2_idscan_tracks",_l2_idscan_tracks);
    ar & boost::serialization::make_nvp("l2_sitrack_tracks",_l2_sitrack_tracks);
    ar & boost::serialization::make_nvp("truth_particles",_truth_particles);
    ar & boost::serialization::make_nvp("electrons",_electrons);
    ar & boost::serialization::make_nvp("photons",_photons);
    ar & boost::serialization::make_nvp("muons",_muons);
    ar & boost::serialization::make_nvp("vertices",_vertices);
    ar & boost::serialization::make_nvp("highest_sum_pt_vertex",_highest_sum_pt_vertex);
    ar & boost::serialization::make_nvp("met",_met);
    ar & boost::serialization::make_nvp("trigger",_trigger);
    ar & boost::serialization::make_nvp("beamline",_beamline);
    ar & boost::serialization::make_nvp("truth_table",_truth_table);
    ar & boost::serialization::make_nvp("atlas_anti_kt_4_h1_topo_jets",_atlas_anti_kt_4_h1_topo_jets);
    ar & boost::serialization::make_nvp("atlas_anti_kt_6_h1_topo_jets",_atlas_anti_kt_6_h1_topo_jets);
    ar & boost::serialization::make_nvp("atlas_anti_kt_4_h1_lc_topo_jets",_atlas_anti_kt_4_h1_lc_topo_jets);
    ar & boost::serialization::make_nvp("atlas_anti_kt_6_h1_lc_topo_jets",_atlas_anti_kt_6_h1_lc_topo_jets);
    ar & boost::serialization::make_nvp("atlas_met_goodness_jets",_atlas_met_goodness_jets);
    ar & boost::serialization::make_nvp("l1_roi_jets",_l1_roi_jets);
    ar & boost::serialization::make_nvp("atlas_cone_4_tower_jets",_atlas_cone_4_tower_jets);
    ar & boost::serialization::make_nvp("atlas_cone_7_tower_jets",_atlas_cone_7_tower_jets);
    ar & boost::serialization::make_nvp("atlas_cone_4_truth_jets",_atlas_cone_4_truth_jets);
    ar & boost::serialization::make_nvp("atlas_cone_4_truth_pileup_jets",_atlas_cone_4_truth_pileup_jets);
    ar & boost::serialization::make_nvp("atlas_cone_6_truth_jets",_atlas_cone_6_truth_jets);
    ar & boost::serialization::make_nvp("atlas_cone_6_truth_pileup_jets",_atlas_cone_6_truth_pileup_jets);
    ar & boost::serialization::make_nvp("pgs_jets",_pgs_jets);
    ar & boost::serialization::make_nvp("ftk_roads",_ftk_roads);
    ar & boost::serialization::make_nvp("ftk_superstrips",_ftk_superstrips);
    ar & boost::serialization::make_nvp("si_channels",_si_channels);
    ar & boost::serialization::make_nvp("si_clusters",_si_clusters);
    ar & boost::serialization::make_nvp("bad_modules",_bad_modules);
    ar & boost::serialization::make_nvp("ftk_n_tracks",_ftk_n_tracks);
    ar & boost::serialization::make_nvp("ftk_n_combinations",_ftk_n_combinations);
    ar & boost::serialization::make_nvp("ftk_n_fits",_ftk_n_fits);
    ar & boost::serialization::make_nvp("ftk_n_fits_majority",_ftk_n_fits_majority);
    ar & boost::serialization::make_nvp("ftk_n_fits_bad",_ftk_n_fits_bad);
    ar & boost::serialization::make_nvp("ftk_n_fits_majority_bad",_ftk_n_fits_majority_bad);
    ar & boost::serialization::make_nvp("ftk_n_fits_hitwarrior_rejected",_ftk_n_fits_hitwarrior_rejected);
    ar & boost::serialization::make_nvp("ftk_n_clusters",_ftk_n_clusters);
    ar & boost::serialization::make_nvp("bunch_number",_bunch_number);
    ar & boost::serialization::make_nvp("bdff",_bdff);
    ar & boost::serialization::make_nvp("mc_weight",_mc_weight);
    ar & boost::serialization::make_nvp("pdf_weight",_pdf_weights);
    ar & boost::serialization::make_nvp("mc_channel",_mc_channel);
    ar & boost::serialization::make_nvp("mc_hfor",_mc_hfor);
  }
  
  // helper functions for loading const classes
  template<typename objectT,typename collT,typename archiveT>
  inline void archive_load_const_pointer_vector( collT& coll , archiveT& ar , const char* name ) {
    typedef std::map< std::string , std::vector< boost::shared_ptr<objectT> > > nonconstCollT;
    nonconstCollT tmp;
    ar & boost::serialization::make_nvp(name,tmp);
    coll.clear();
    BOOST_FOREACH( typename nonconstCollT::value_type& v , tmp ) {
      std::vector< boost::shared_ptr<const objectT> > tmpb;
      std::copy( v.second.begin() , v.second.end() , std::back_inserter(tmpb) );
      coll.insert( typename collT::value_type( v.first , tmpb ) );
    }
  }
  template<typename objectT,typename collT,typename archiveT>
  inline void archive_load_const_pointer( collT& coll , archiveT& ar , const char* name ) {
    typedef std::map< std::string , boost::shared_ptr<objectT> > nonconstCollT;
    nonconstCollT tmp;
    ar & boost::serialization::make_nvp(name,tmp);
    coll.clear();
    BOOST_FOREACH( typename nonconstCollT::value_type& v , tmp ) {
      coll.insert( typename collT::value_type( v.first , v.second ) );
    }
  }
  
  template<class archiveT>
  void load( archiveT& ar, const unsigned int version) {
    // header
    ar & boost::serialization::make_nvp("sample_type",_sample_type);
    ar & boost::serialization::make_nvp("evt_mgr_index",_evt_mgr_index);
    ar & boost::serialization::make_nvp("nbytes",_nbytes);
    ar & boost::serialization::make_nvp("run_number",_run_number);
    ar & boost::serialization::make_nvp("event_number",_event_number);
    ar & boost::serialization::make_nvp("lumi_block",_lumi_block);
    if (version>=7 ) {
      ar & boost::serialization::make_nvp("average_mu",_average_mu);
      ar & boost::serialization::make_nvp("actual_mu",_actual_mu);
    }
    ar & boost::serialization::make_nvp("ftk_road_nplanes",_ftk_road_nplanes);
    if ( version>=8) {
      ar & boost::serialization::make_nvp("mcevt_event_scale",_mcevt_event_scale);
      ar & boost::serialization::make_nvp("mcevt_alphaQCD",_mcevt_alphaQCD);
      ar & boost::serialization::make_nvp("mcevt_alphaQED",_mcevt_alphaQED);
      ar & boost::serialization::make_nvp("mcevt_pdf_scale",_mcevt_pdf_scale);
      ar & boost::serialization::make_nvp("mcevt_id1",_mcevt_id1);
      ar & boost::serialization::make_nvp("mcevt_id2",_mcevt_id2);
      ar & boost::serialization::make_nvp("mcevt_pdf_x1",_mcevt_pdf_x1);
      ar & boost::serialization::make_nvp("mcevt_pdf_x2",_mcevt_pdf_x2);
      ar & boost::serialization::make_nvp("mcevt_pdf1",_mcevt_pdf1);
      ar & boost::serialization::make_nvp("mcevt_pdf2",_mcevt_pdf2);
    }
    ar & boost::serialization::make_nvp("provenance",_provenance);
    ar & boost::serialization::make_nvp("sample_flags",_sample_flags);
    ar & boost::serialization::make_nvp("sample_numeric_flags",_sample_numeric_flags);
    if( version>=4 ) {
      ar & boost::serialization::make_nvp("sample_strings",_sample_strings);
      ar & boost::serialization::make_nvp("sample_tags",_sample_tags);
    }
    // data
    archive_load_const_pointer_vector<AnaTrack>( _ftk_tracks , ar , "ftk_tracks" );
    archive_load_const_pointer_vector<AnaTrack>( _trigftklib_tracks , ar , "trigftklib_tracks" );
    archive_load_const_pointer_vector<AnaTrack>( _offline_tracks , ar , "offline_tracks" );
    archive_load_const_pointer_vector<AnaTrack>( _truth_tracks , ar , "truth_tracks" );
    archive_load_const_pointer_vector<AnaTrack>( _l2_idscan_tracks , ar , "l2_idscan_tracks" );
    archive_load_const_pointer_vector<AnaTrack>( _l2_sitrack_tracks , ar , "l2_sitrack_tracks" );
    archive_load_const_pointer_vector<AnaTruthParticle>( _truth_particles , ar , "truth_particles" );
    archive_load_const_pointer_vector<AnaElectron>( _electrons , ar , "electrons" );
    archive_load_const_pointer_vector<AnaPhoton>( _photons , ar , "photons" );
    archive_load_const_pointer_vector<AnaMuon>( _muons , ar , "muons" );
    archive_load_const_pointer_vector<AnaVertex>( _vertices , ar , "vertices" );
    archive_load_const_pointer<AnaVertex>( _highest_sum_pt_vertex , ar , "highest_sum_pt_vertex" );
    archive_load_const_pointer_vector<AnaMET>( _met , ar , "met" );
    archive_load_const_pointer<AnaTrigger>( _trigger , ar , "trigger" );
    archive_load_const_pointer<AnaBeamline>( _beamline , ar , "beamline" );
    archive_load_const_pointer<AnaTruthTable>( _truth_table , ar , "truth_table" );
    archive_load_const_pointer_vector<AnaJet>( _atlas_anti_kt_4_h1_topo_jets , ar , "atlas_anti_kt_4_h1_topo_jets" );
    archive_load_const_pointer_vector<AnaJet>( _atlas_anti_kt_6_h1_topo_jets , ar , "atlas_anti_kt_6_h1_topo_jets" );
    archive_load_const_pointer_vector<AnaJet>( _atlas_anti_kt_4_h1_lc_topo_jets , ar , "atlas_anti_kt_4_h1_lc_topo_jets" );
    archive_load_const_pointer_vector<AnaJet>( _atlas_anti_kt_6_h1_lc_topo_jets , ar , "atlas_anti_kt_6_h1_lc_topo_jets" );
    archive_load_const_pointer_vector<AnaJet>( _atlas_met_goodness_jets , ar , "atlas_met_goodness_jets" );
    archive_load_const_pointer_vector<AnaJet>( _l1_roi_jets , ar , "l1_roi_jets" );
    archive_load_const_pointer_vector<AnaJet>( _atlas_cone_4_tower_jets , ar , "atlas_cone_4_tower_jets" );
    archive_load_const_pointer_vector<AnaJet>( _atlas_cone_7_tower_jets , ar , "atlas_cone_7_tower_jets" );
    archive_load_const_pointer_vector<AnaJet>( _atlas_cone_4_truth_jets , ar , "atlas_cone_4_truth_jets" );
    archive_load_const_pointer_vector<AnaJet>( _atlas_cone_4_truth_pileup_jets , ar , "atlas_cone_4_truth_pileup_jets" );
    archive_load_const_pointer_vector<AnaJet>( _atlas_cone_6_truth_jets , ar , "atlas_cone_6_truth_jets" );
    archive_load_const_pointer_vector<AnaJet>( _atlas_cone_6_truth_pileup_jets , ar , "atlas_cone_6_truth_pileup_jets" );
    archive_load_const_pointer_vector<AnaJet>( _pgs_jets , ar , "pgs_jets" );
    archive_load_const_pointer_vector<AnaRoad>( _ftk_roads , ar , "ftk_roads" );
    archive_load_const_pointer_vector<AnaSuperstrip>( _ftk_superstrips , ar , "ftk_superstrips" );
    archive_load_const_pointer_vector<AnaSiChannel>( _si_channels , ar , "si_channels" );
    archive_load_const_pointer_vector<AnaSiCluster>( _si_clusters , ar , "si_clusters" );
    archive_load_const_pointer<SiModuleList>( _bad_modules , ar , "bad_modules" );
    ar & boost::serialization::make_nvp( "ftk_n_tracks" , _ftk_n_tracks );
    ar & boost::serialization::make_nvp( "ftk_n_combinations" , _ftk_n_combinations );
    ar & boost::serialization::make_nvp( "ftk_n_fits" , _ftk_n_fits );
    ar & boost::serialization::make_nvp( "ftk_n_fits_majority" , _ftk_n_fits_majority );
    ar & boost::serialization::make_nvp( "ftk_n_fits_bad" , _ftk_n_fits_bad );
    ar & boost::serialization::make_nvp( "ftk_n_fits_majority_bad" , _ftk_n_fits_majority_bad );
    ar & boost::serialization::make_nvp( "ftk_n_fits_hitwarrior_rejected" , _ftk_n_fits_hitwarrior_rejected );
    ar & boost::serialization::make_nvp( "ftk_n_clusters" , _ftk_n_clusters );
    ar & boost::serialization::make_nvp("bunch_number",_bunch_number);
    ar & boost::serialization::make_nvp("bdff",_bdff);
    ar & boost::serialization::make_nvp("mc_weight",_mc_weight);
    ar & boost::serialization::make_nvp("pdf_weight",_pdf_weights);
    ar & boost::serialization::make_nvp("mc_channel",_mc_channel);
    ar & boost::serialization::make_nvp("mc_hfor",_mc_hfor);
  }
  BOOST_SERIALIZATION_SPLIT_MEMBER();
#endif
  
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
  boost::shared_ptr<const AnaTruthTable>& truth_table( const std::string& source ) { return get_from_source(source,_truth_table); };
  std::vector< boost::shared_ptr<const AnaRoad> >& ftk_roads( const std::string& source ) { return get_from_source(source,_ftk_roads); } // ftk roads from nominal simulation
  const NtRoadBankLookup& ftk_road_lookup( const std::string& source ) { return get_from_source(source,_ftk_road_lookup); } // ftk roads from nominal simulation
  const NtRoadLayerLookup& ftk_superstrip_lookup( const std::string& source ) { return get_from_source(source,_ftk_ss_lookup); } // ftk roads from nominal simulation
  std::vector< boost::shared_ptr<const AnaSuperstrip> >& ftk_superstrips( const std::string& source ) { return get_from_source(source,_ftk_superstrips); } // ftk roads from nominal simulation
  std::vector< boost::shared_ptr<const AnaSiChannel> >& si_channels( const std::string& source ) { return get_from_source(source,_si_channels); } // silicon readout objects from athena
  std::vector< boost::shared_ptr<const AnaSiCluster> >& si_clusters( const std::string& source ) { return get_from_source(source,_si_clusters); } // silicon readout objects from athena
  boost::shared_ptr<const SiModuleList>& bad_modules( const std::string& source ) { return get_from_source(source,_bad_modules); } // list of bad modules from athena

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
  boost::shared_ptr<const AnaTruthTable> const& truth_table() const { return get_from_source(AnaEvent::DEFAULT_SOURCE,_truth_table); }
  boost::shared_ptr<const AnaTruthTable> const& truth_table( const std::string& source ) const { 
    return get_from_source(source,_truth_table);
  }
  const NtRoadBankLookup& ftk_road_lookup( const std::string source = AnaEvent::DEFAULT_SOURCE ) const { 
    return get_from_source(source,_ftk_road_lookup); 
  }
  const std::vector< boost::shared_ptr<const AnaTruthParticle> >& truth_particles( const std::string& source = AnaEvent::DEFAULT_SOURCE ) const { 
    return get_from_source(source,_truth_particles);
  }
  const std::vector< boost::shared_ptr<const AnaRoad> >& ftk_roads( const std::string source = AnaEvent::DEFAULT_SOURCE ) const { 
    return get_from_source(source,_ftk_roads); 
  }
  boost::shared_ptr<const AnaRoad> ftk_road_lookup( const std::string source = AnaEvent::DEFAULT_SOURCE , const int& bank_id = -1 , const unsigned int& road_id = 0 ) const {
    assert( bank_id != -1 && "pass bank id and road id for valid lookup" );
    static boost::shared_ptr<const AnaRoad> null;
    const NtRoadBankLookup& lookup = get_from_source(source,_ftk_road_lookup);
    NtRoadBankLookup::const_iterator i = lookup.find( bank_id );
    if( i==lookup.end() ) { return null; }
    NtRoadLookup::const_iterator j = i->second.find( road_id );
    if( j==i->second.end() ) { return null; }
    return j->second;
  }
  const std::vector< boost::shared_ptr<const AnaSuperstrip> >& ftk_superstrips( const std::string source = AnaEvent::DEFAULT_SOURCE ) const { 
    return get_from_source(source,_ftk_superstrips); 
  }
  const NtRoadLayerLookup& ftk_superstrip_lookup( const std::string source = AnaEvent::DEFAULT_SOURCE ) const { 
    return get_from_source(source,_ftk_ss_lookup); 
  }
  boost::shared_ptr<const AnaSuperstrip> ftk_ss_lookup( const std::string source = AnaEvent::DEFAULT_SOURCE , const unsigned int& layer = 999 , const unsigned long& ss_id = 0 ) const {
    assert( layer != 999 && "pass layer and superstrip id for valid lookup" );
    static boost::shared_ptr<const AnaSuperstrip> null;
    const NtRoadLayerLookup& lookup = get_from_source(source,_ftk_ss_lookup);
    NtRoadLayerLookup::const_iterator i = lookup.find( layer );
    if( i==lookup.end() ) { return null; }
    NtRoadSuperstripLookup::const_iterator j = i->second.find( ss_id );
    if( j==i->second.end() ) { return null; }
    return j->second;
  }
  const std::vector< boost::shared_ptr<const AnaSiChannel> >& si_channels( const std::string source = AnaEvent::DEFAULT_SOURCE ) const { 
    return get_from_source(source,_si_channels); 
  }
  const std::vector< boost::shared_ptr<const AnaSiCluster> >& si_clusters( const std::string source = AnaEvent::DEFAULT_SOURCE ) const { 
    return get_from_source(source,_si_clusters); 
  }
  const boost::shared_ptr<const SiModuleList>& bad_modules( const std::string source = AnaEvent::DEFAULT_SOURCE ) const { 
    return get_from_source(source,_bad_modules); 
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

BOOST_CLASS_VERSION( AnaEvent , 9 );

#endif // WRAP_ANAEVENT_HPP
