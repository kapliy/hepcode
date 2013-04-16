#ifndef WRAP_ANACONFIGURATION_HH
#define WRAP_ANACONFIGURATION_HH

// class to centrally manage job configuration for all ana/*
// executables.

#include <string>
#include <boost/scope_exit.hpp>
#include <boost/foreach.hpp>
#include "DataGraphics/DataGraphics.hh"
#include "TrigFTKAna/Detector.hpp"
#include "TrigFTKAna/Conference.hpp"

class
AnaConfiguration
{

public:

  typedef enum { OFFLINE_FROM_D3PD } OfflineTrackReadMode;

private:

  static bool _configured;
  static std::string _sample_name;
  static unsigned long _user_max_nevents;
  static unsigned int _split_num_files;
  static unsigned int _split_offset_file;
  static std::string _output_dir;
  static std::string _output_filename;
  static std::string _full_output_path;
  static bool _force_full_output_path;
  static std::string _full_sample_definition_path;
  static bool _skip_systematics;
  static bool _apply_pileup_weights;
  static float _pileup_data_scale;
  static std::string _pileup_weights_map;
  static std::string _pileup_config_map;
  static std::string _grl_xml_files;
  static float _pt_cut;
  static bool _precompute_number_of_events;
  static int _muon_scale;
  static detector::MCP_TYPE _muon_type;
  static detector::EGAMMA_TYPE _electron_type;
  static int _release;
  static int _replicas;
  static CONF::ConfType _conf;
  static DATARANGE::DataRange _data_range;
  static int _save_ntuples;

private:
public:

  AnaConfiguration() {}
  virtual ~AnaConfiguration() {}

  static void configure( const std::string& executable_name , int argc , char* argv[] );
  
  static const bool configured() { return _configured; }

  static const std::string sample_name() { return _sample_name; }
  static const unsigned long user_max_nevents() { return _user_max_nevents; }
  static const CONF::ConfType conf() { return _conf; }
  static const DATARANGE::DataRange data_range() { return _data_range; }
  static const unsigned int split_num_files() { return _split_num_files; }
  static const unsigned int split_offset_file() { return _split_offset_file; }
  static const std::string output_dir() { return _output_dir; }
  static const std::string output_filename() { return _output_filename; }
  static const std::string full_output_path() { return _full_output_path; }
  static const bool force_full_output_path() { return _force_full_output_path; }
  static const std::string full_sample_definition_path() { return _full_sample_definition_path; }
  static const bool skip_systematics() { return _skip_systematics; }
  static const bool apply_pileup_weights() { return _apply_pileup_weights; }
  static const float pileup_data_scale() { return _pileup_data_scale; }
  static const std::string pileup_weights_map() { return _pileup_weights_map; }
  static const std::string pileup_config_map() { return _pileup_config_map; }
  static const std::string grl_xml_files() { return _grl_xml_files; }
  static const float pt_cut() { return _pt_cut; }
  static bool precompute_number_of_events() { return _precompute_number_of_events; }
  static const int muon_scale() { return _muon_scale; }
  static const detector::MCP_TYPE muon_type() { return _muon_type; }
  static const detector::EGAMMA_TYPE electron_type() { return _electron_type; }
  static const int release() { return _release; }
  static const int replicas() { return _replicas; }
  static const int save_ntuples() { return _save_ntuples; }

  static void sample_name( const std::string& val ) { _sample_name = val; }
  static void user_max_nevents( const unsigned long& val ) { _user_max_nevents = val; }
  static void split_num_files( const unsigned int& val ) { _split_num_files = val; }
  static void split_offset_file( const unsigned int& val ) { _split_offset_file = val; }
  static void output_dir( const std::string& val ) { _output_dir = val; }
  static void output_filename( const std::string& val ) { _output_filename = val; }
  static void full_output_path( const std::string& val ) { _full_output_path = val; }
  static void force_full_output_path( const bool& val ) { _force_full_output_path = val; }
  static void full_sample_definition_path( const std::string& val ) { _full_sample_definition_path = val; }
  static void skip_systematics( const bool& val ) { _skip_systematics = val; }
  static void apply_pileup_weights( const bool& val ) { _apply_pileup_weights = val; }
  static void pileup_data_scale( const float& val ) { _pileup_data_scale = val; }
  static void pileup_weights_map( const std::string& val ) { _pileup_weights_map = val; }
  static void pileup_config_map( const std::string& val ) { _pileup_config_map = val; }
  static void grl_xml_files( const std::string& val ) { _grl_xml_files = val; }
  static void pt_cut( const float& val ) { _pt_cut = val; }
  static void precompute_number_of_events( const bool& yes ) { _precompute_number_of_events = yes; }
  static void muon_scale( const int& val ) { _muon_scale = val; }
  static void muon_type( const detector::MCP_TYPE& val ) { _muon_type = val; }
  static void electron_type( const detector::EGAMMA_TYPE& val ) { _electron_type = val; }
  static void release( const int& val ) { _release = val; }
  static void replicas( const int& val ) { _replicas = val; }
  static void conf( const CONF::ConfType &val ) { _conf = val; }
  static void data_range( const DATARANGE::DataRange &val ) { _data_range = val; }
  static void save_ntuples( const int &val ) { _save_ntuples = val; }

  // disk I/O
  void save( const std::string& filename );
  void load( const std::string& filename );

};

#endif // WRAP_ANACONFIGURATION_HH
