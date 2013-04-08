#ifndef WRAP_ANACONFIGURATION_HH
#define WRAP_ANACONFIGURATION_HH

// class to centrally manage job configuration for all ana/*
// executables.

#include <string>
#include <boost/scope_exit.hpp>
#include <boost/foreach.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/version.hpp>
#include <boost/serialization/string.hpp>
#include "DataGraphics/DataGraphics.hh"
#include "TrigFTKAna/Detector.hpp"
#include "TrigFTKAna/Conference.hpp"

class
AnaConfiguration
{

public:

  typedef enum { OFFLINE_FROM_WRAPPER , OFFLINE_FROM_CBNT , OFFLINE_MERGE_WRAPPER_CBNT , OFFLINE_FROM_D3PD } OfflineTrackReadMode;

private:

  static bool _configured;
  static std::string _sample_name;
  static unsigned long _user_max_nevents;
  static unsigned int _split_num_files;
  static unsigned int _split_offset_file;
  static std::string _source_to_treat_as_reference;
  static std::string _output_dir;
  static std::string _output_filename;
  static std::string _full_output_path;
  static bool _force_full_output_path;
  static std::string _full_sample_definition_path;
  static bool _save_as_ROOT;
  static bool _save_as_dg;
  static bool _skip_systematics;
  static bool _apply_pileup_weights;
  static float _pileup_data_scale;
  static std::string _pileup_weights_map;
  static std::string _pileup_config_map;
  static std::string _grl_xml_files;
  static bool _do_hfor;
  static float _barcode_matching_minimum_cut;
  static float _maximum_track_abseta_cut;
  static float _minimum_track_pt_cut;
  static float _maximum_track_absd0_cut;
  static float _central_value_track_phi_cut;
  static float _half_width_track_phi_cut;
  static bool _si_channel_studies;
  static bool _si_cluster_studies;
  static float _jet_label_dr_cut;
  static bool _use_strict_majority;
  static float _apply_jet_dz0_prune;
  static bool _train_tagger;
  static std::string _trained_tagger_file;
  static OfflineTrackReadMode _offline_track_read_mode;
  static bool _precompute_number_of_events;
  static std::string _ana_event_ntuple_directory;
  static bool _combine_roads;
  static int _JES_offset;
  static int _JER_offset;
  static int _elec_offset;
  static int _muon_offset;
  static int _muon_scale;
  static detector::MCP_TYPE _muon_type;
  static detector::EGAMMA_TYPE _electron_type;
  static int _release;
  static bool _read_offline_si_channels;
  static bool _read_offline_si_clusters;
  static std::string _ftk_pmap;
  static CONF::ConfType _conf;
  static DATARANGE::DataRange _data_range;
  static int _save_ntuples;
  static int _pdf_reweight;

private:
  // disk i/o
  friend class boost::serialization::access;
  template<typename archiveT>
  void serialize( archiveT& ar , const unsigned int version ) {
    ar & _configured;
    ar & _sample_name;
    ar & _user_max_nevents;
    ar & _source_to_treat_as_reference;
    ar & _output_dir;
    ar & _output_filename;
    ar & _full_output_path;
    ar & _force_full_output_path;
    ar & _full_sample_definition_path;
    ar & _save_as_ROOT;
    ar & _save_as_dg;
    ar & _skip_systematics;
    ar & _apply_pileup_weights;
    ar & _pileup_data_scale;
    ar & _pileup_weights_map;
    ar & _pileup_config_map;
    ar & _grl_xml_files;
    ar & _do_hfor;
    ar & _barcode_matching_minimum_cut;
    ar & _maximum_track_abseta_cut;
    ar & _train_tagger;
    ar & _trained_tagger_file;
    ar & _offline_track_read_mode;
    ar & _split_num_files;
    ar & _split_offset_file;
    ar & _minimum_track_pt_cut;
    ar & _jet_label_dr_cut;
    ar & _use_strict_majority;
    ar & _apply_jet_dz0_prune;
    ar & _central_value_track_phi_cut;
    ar & _half_width_track_phi_cut;
    ar & _si_channel_studies;
    ar & _si_cluster_studies;
    ar & _precompute_number_of_events;
    ar & _maximum_track_absd0_cut;
    ar & _ana_event_ntuple_directory;
    ar & _combine_roads;
    ar & _JES_offset;
    ar & _JER_offset;
    ar & _elec_offset;
    ar & _muon_offset;
    ar & _muon_scale;
    ar & _release;
    ar & _read_offline_si_channels;
    ar & _read_offline_si_clusters;
    ar & _ftk_pmap;
    ar & _conf;
    ar & _data_range;
    ar & _save_ntuples;
    ar & _pdf_reweight;
    ar & _muon_type;
  }

public:

  AnaConfiguration() {}
  virtual ~AnaConfiguration() {}

  static void configure( const std::string& executable_name , int argc , char* argv[] );
  
  static const bool configured() { return _configured; }

  static const std::string sample_name() { return _sample_name; }
  static const unsigned long user_max_nevents() { return _user_max_nevents; }
  static const std::string ftk_pmap() { return _ftk_pmap; }
  static const CONF::ConfType conf() { return _conf; }
  static const DATARANGE::DataRange data_range() { return _data_range; }
  static const unsigned int split_num_files() { return _split_num_files; }
  static const unsigned int split_offset_file() { return _split_offset_file; }
  static const bool has_reference_source() { return _source_to_treat_as_reference!=""; }
  static const std::string source_to_treat_as_reference() { return _source_to_treat_as_reference; }
  static const std::string output_dir() { return _output_dir; }
  static const std::string output_filename() { return _output_filename; }
  static const std::string full_output_path() { return _full_output_path; }
  static const bool force_full_output_path() { return _force_full_output_path; }
  static const std::string full_sample_definition_path() { return _full_sample_definition_path; }
  static const bool save_as_ROOT() { return _save_as_ROOT; }
  static const bool save_as_dg() { return _save_as_dg; }
  static const bool skip_systematics() { return _skip_systematics; }
  static const bool apply_pileup_weights() { return _apply_pileup_weights; }
  static const float pileup_data_scale() { return _pileup_data_scale; }
  static const std::string pileup_weights_map() { return _pileup_weights_map; }
  static const std::string pileup_config_map() { return _pileup_config_map; }
  static const std::string grl_xml_files() { return _grl_xml_files; }
  static const bool do_hfor() { return _do_hfor; }
  static const float barcode_matching_minimum_cut() { return _barcode_matching_minimum_cut; }
  static const float maximum_track_abseta_cut() { return _maximum_track_abseta_cut; }
  static const float minimum_track_pt_cut() { return _minimum_track_pt_cut; }
  static const float maximum_track_absd0_cut() { return _maximum_track_absd0_cut; }
  static const float central_value_track_phi_cut() { return _central_value_track_phi_cut; }
  static const float half_width_track_phi_cut() { return _half_width_track_phi_cut; }
  static const bool phi_passes_cut( const float& phi ) { return std::abs(detector::delta_phi(phi,_central_value_track_phi_cut))<=_half_width_track_phi_cut; }
  static const bool phi_fails_cut( const float& phi ) { return !phi_passes_cut(phi); }
  static const bool read_offline_si_channels() { return _read_offline_si_channels; }
  static const bool read_offline_si_clusters() { return _read_offline_si_clusters; }
  static const bool do_si_channel_studies() { return _si_channel_studies; }
  static const bool do_si_cluster_studies() { return _si_cluster_studies; }
  static const float jet_label_dr_cut() { return _jet_label_dr_cut; }
  static const bool use_strict_majority() { return _use_strict_majority; }
  static const float jet_dz0_prune() { return _apply_jet_dz0_prune; }
  static const bool train_tagger() { return  _train_tagger; }
  static const std::string trained_tagger_file() { return  _trained_tagger_file; }
  static const AnaConfiguration::OfflineTrackReadMode offline_track_read_mode() { return _offline_track_read_mode; }
  static bool precompute_number_of_events() { return _precompute_number_of_events; }
  static const std::string ana_event_ntuple_directory() { return _ana_event_ntuple_directory; }
  static const bool combine_roads() { return  _combine_roads; }
  static const int JES_offset() { return _JES_offset; }
  static const int JER_offset() { return _JER_offset; }
  static const int elec_offset() { return _elec_offset; }
  static const int muon_offset() { return _muon_offset; }
  static const std::string muon_offsetS() {
    switch( _muon_offset ) {
    case 1:
      return "NOMINAL";
      break;
    case 2:
      return "MSLOW";
      break;
    case 3:
      return "MSUP";
      break;
    case 4:
      return "IDLOW";
      break;
    case 5:
      return "IDUP";
      break;
    case 6:
      return "SCALEKUP";
      break;
    case 7:
      return "SCALEKDOWN";
      break;
    case 8:
      return "SCALECUP";
      break;
    case 9:
      return "SCALECDOWN";
      break;
    }
    return "NOMINAL";
  }
  static const int muon_scale() { return _muon_scale; }
  static const detector::MCP_TYPE muon_type() { return _muon_type; }
  static const detector::EGAMMA_TYPE electron_type() { return _electron_type; }
  static const int release() { return _release; }
  static const int save_ntuples() { return _save_ntuples; }
  static const int pdf_reweight() { return _pdf_reweight; }

  static void sample_name( const std::string& val ) { _sample_name = val; }
  static void user_max_nevents( const unsigned long& val ) { _user_max_nevents = val; }
  static void split_num_files( const unsigned int& val ) { _split_num_files = val; }
  static void split_offset_file( const unsigned int& val ) { _split_offset_file = val; }
  static void source_to_treat_as_reference( const std::string& val ) { _source_to_treat_as_reference = val; }
  static void output_dir( const std::string& val ) { _output_dir = val; }
  static void output_filename( const std::string& val ) { _output_filename = val; }
  static void full_output_path( const std::string& val ) { _full_output_path = val; }
  static void force_full_output_path( const bool& val ) { _force_full_output_path = val; }
  static void full_sample_definition_path( const std::string& val ) { _full_sample_definition_path = val; }
  static void save_as_ROOT( const bool& val ) { _save_as_ROOT = val; }
  static void save_as_dg( const bool& val ) { _save_as_dg = val; }
  static void skip_systematics( const bool& val ) { _skip_systematics = val; }
  static void apply_pileup_weights( const bool& val ) { _apply_pileup_weights = val; }
  static void pileup_data_scale( const float& val ) { _pileup_data_scale = val; }
  static void pileup_weights_map( const std::string& val ) { _pileup_weights_map = val; }
  static void pileup_config_map( const std::string& val ) { _pileup_config_map = val; }
  static void grl_xml_files( const std::string& val ) { _grl_xml_files = val; }
  static void do_hfor( const bool& val ) { _do_hfor = val; }
  static void barcode_matching_minimum_cut( const float& val ) { _barcode_matching_minimum_cut = val; }
  static void maximum_track_abseta_cut( const float& val ) { _maximum_track_abseta_cut = val; }
  static void minimum_track_pt_cut( const float& val ) { _minimum_track_pt_cut = val; }
  static void maximum_track_absd0_cut( const float& val ) { _maximum_track_absd0_cut = val; }
  static void train_tagger( const bool& val ) { _train_tagger = val; }
  static void trained_tagger_file( const std::string& val ) { _trained_tagger_file = val; }
  static void offline_track_read_mode( const AnaConfiguration::OfflineTrackReadMode& mode ) { _offline_track_read_mode = mode; }
  static void precompute_number_of_events( const bool& yes ) { _precompute_number_of_events = yes; }
  static void ana_event_ntuple_directory( const std::string& dir ) { _ana_event_ntuple_directory = dir; }
  static void combine_roads( const bool& val ) { _combine_roads = val; }
  static void JES_offset( const int& val ) { _JES_offset = val; }
  static void JER_offset( const int& val ) { _JER_offset = val; }
  static void elec_offset( const int& val ) { _elec_offset = val; }
  static void muon_offset( const int& val ) { _muon_offset = val; }
  static void muon_scale( const int& val ) { _muon_scale = val; }
  static void muon_type( const detector::MCP_TYPE& val ) { _muon_type = val; }
  static void electron_type( const detector::EGAMMA_TYPE& val ) { _electron_type = val; }
  static void release( const int& val ) { _release = val; }
  static void ftk_pmap( const std::string &val ) { _ftk_pmap = val; }
  static void conf( const CONF::ConfType &val ) { _conf = val; }
  static void data_range( const DATARANGE::DataRange &val ) { _data_range = val; }
  static void save_ntuples( const int &val ) { _save_ntuples = val; }
  static void pdf_reweight( const int &val ) { _pdf_reweight = val; }

  // disk I/O
  void save( const std::string& filename );
  void load( const std::string& filename );

};

BOOST_CLASS_VERSION( AnaConfiguration , 12 );

// utility macros for analysis programs

// standard definition of the program main function.
//
#ifdef __cplusplus 
#define TRIGFTKANA_MAIN( argcparm , argvparm ) \
  extern "C"                                   \
  int                                          \
  F77_MAIN( argcparm , argvparm )              
#else
#define TRIGFTKANA_MAIN( argcparm , argvparm ) \
  int                                          \
  F77_MAIN( argcparm , argvparm )              
#endif

// standard "attach configuration and histograms" boilerplate
#define ANA_CONFIGURE( program_name ) \
  AnaConfiguration::configure(program_name,argc,argv); \
  if( !AnaConfiguration::configured() ) { exit(0); } \
  DataGraphics::dg::root()->no_save_at_exit();                          \
  BOOST_SCOPE_EXIT() { DataGraphics::dg::root()->do_save_at_exit(); } BOOST_SCOPE_EXIT_END;

// standard event loop
#define TRIGFTKANA_FOREACHEVENT( eventdecl , neventdecl ) \
  unsigned long neventdecl = 0ul;                                 \
  AnaEventMgr* __evtmgr = AnaEventMgr::instance(); assert(__evtmgr); \
  AnaEventMgr::iterator __ievtmgr=__evtmgr->begin() , __fevtmgr=__evtmgr->end(AnaConfiguration::user_max_nevents()); \
  for( shared_ptr<const AnaEvent> eventdecl( *__ievtmgr ); __ievtmgr!=__fevtmgr; ++__ievtmgr, ++neventdecl, eventdecl=(*__ievtmgr) )


// eventually base this on BOOST_FOREACH:
// #define BOOST_FOREACH(VAR, COL)                                                                                   \
//     BOOST_FOREACH_PREAMBLE()                                                                                      \
//     if (boost::foreach_detail_::auto_any_t BOOST_FOREACH_ID(_foreach_col) = BOOST_FOREACH_CONTAIN(COL)) {} else   \
//     if (boost::foreach_detail_::auto_any_t BOOST_FOREACH_ID(_foreach_cur) = BOOST_FOREACH_BEGIN(COL)) {} else     \
//     if (boost::foreach_detail_::auto_any_t BOOST_FOREACH_ID(_foreach_end) = BOOST_FOREACH_END(COL)) {} else       \
//     for (bool BOOST_FOREACH_ID(_foreach_continue) = true;                                                         \
//               BOOST_FOREACH_ID(_foreach_continue) && !BOOST_FOREACH_DONE(COL);                                    \
//               BOOST_FOREACH_ID(_foreach_continue) ? BOOST_FOREACH_NEXT(COL) : (void)0)                            \
//         if  (boost::foreach_detail_::set_false(BOOST_FOREACH_ID(_foreach_continue))) {} else                      \
//         for (VAR = BOOST_FOREACH_DEREF(COL); !BOOST_FOREACH_ID(_foreach_continue); BOOST_FOREACH_ID(_foreach_continue) = true)


#endif // WRAP_ANACONFIGURATION_HH
