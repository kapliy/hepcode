
#include <string>
#include <limits>
#include <cmath>
#include <boost/format.hpp>
#include <iostream>
#include <fstream>
#include <boost/shared_ptr.hpp>
#include <boost/program_options.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/base_object.hpp>
#include "DataGraphics/DgCollection.hh"
#include "DataGraphics/DgTools.hh"
#include "TrigFTKAna/Conference.hpp"
#include "TrigFTKAna/AnaConfiguration.hpp"
#include "TrigFTKAna/AnaEventMgr.hpp"
#include "TrigFTKAna/ana_streams.hpp"


using namespace std;
using namespace boost;
using namespace boost::serialization;
using namespace DataGraphics;
namespace fs = boost::filesystem;
namespace po = boost::program_options;


bool AnaConfiguration::_configured = false;
string AnaConfiguration::_sample_name = "";
unsigned long AnaConfiguration::_user_max_nevents = numeric_limits<unsigned long>::max();
unsigned int AnaConfiguration::_split_num_files = 0;
unsigned int AnaConfiguration::_split_offset_file = 0;
string AnaConfiguration::_source_to_treat_as_reference = "";
std::string AnaConfiguration::_output_dir = "./results";
std::string AnaConfiguration::_output_filename = "%DATE";
std::string AnaConfiguration::_full_output_path = "";
bool AnaConfiguration::_force_full_output_path = false;
std::string AnaConfiguration::_full_sample_definition_path = "";
bool AnaConfiguration::_save_as_ROOT = true;
bool AnaConfiguration::_save_as_dg = true;
bool AnaConfiguration::_skip_systematics = false;
bool AnaConfiguration::_apply_pileup_weights = false;
float AnaConfiguration::_pileup_data_scale = 1.0;
std::string AnaConfiguration::_pileup_weights_map = "";
std::string AnaConfiguration::_pileup_config_map = "";
std::string AnaConfiguration::_grl_xml_files = "";
bool AnaConfiguration::_do_hfor = false;
float AnaConfiguration::_barcode_matching_minimum_cut = 0.7;
float AnaConfiguration::_maximum_track_abseta_cut = 2.5;
float AnaConfiguration::_minimum_track_pt_cut = 1.;
float AnaConfiguration::_maximum_track_absd0_cut = 2.;
float AnaConfiguration::_central_value_track_phi_cut = 0.;
float AnaConfiguration::_half_width_track_phi_cut = M_PI+0.1;
bool AnaConfiguration::_si_channel_studies = false;
bool AnaConfiguration::_si_cluster_studies = false;
float AnaConfiguration::_jet_label_dr_cut = 0.1;
bool AnaConfiguration::_use_strict_majority = false;
float AnaConfiguration::_apply_jet_dz0_prune = 99999.;
bool AnaConfiguration::_train_tagger = false;
std::string AnaConfiguration::_trained_tagger_file = "./training.dat";
AnaConfiguration::OfflineTrackReadMode AnaConfiguration::_offline_track_read_mode = AnaConfiguration::OFFLINE_MERGE_WRAPPER_CBNT;
bool AnaConfiguration::_precompute_number_of_events = false;
std::string AnaConfiguration::_ana_event_ntuple_directory = "";
bool AnaConfiguration::_combine_roads = false;
int AnaConfiguration::_release = 17;
int AnaConfiguration::_JER_offset = 0;
int AnaConfiguration::_JES_offset = 0;
int AnaConfiguration::_elec_offset = 0;
int AnaConfiguration::_muon_offset = 0;
int AnaConfiguration::_muon_scale = 3;
detector::MCP_TYPE AnaConfiguration::_muon_type = detector::MCP_STACO_COMBINED;
detector::EGAMMA_TYPE AnaConfiguration::_electron_type = detector::EGAMMA_TIGHTPP;
bool AnaConfiguration::_read_offline_si_channels = true;
bool AnaConfiguration::_read_offline_si_clusters = true;
std::string AnaConfiguration::_ftk_pmap = "";
CONF::ConfType AnaConfiguration::_conf = CONF::LATEST;
DATARANGE::DataRange AnaConfiguration::_data_range = DATARANGE::DtoM;
int AnaConfiguration::_save_ntuples = 0;
int AnaConfiguration::_pdf_reweight = -1;

void
AnaConfiguration::configure( const std::string& executable_name , int argc , char* argv[] )
{
  // HACK: fortran is passing a nonsense argc. find the real termination (limited here to 256 tokens).
  const unsigned int num_max_tokens = 256;
  argc = std::distance( argv , std::find( argv , argv+num_max_tokens , (char*)0 ) ); 
  // define some job configuration. parse the command line (and optionally a configuration file).
  // if this looks like gobbledigook to you, skip down to "done reading the command line."
  _output_dir = "./results/" + executable_name;
  string sample_configuration_dir = "./samples";
  po::options_description cmdline_options( executable_name + " options" );
  try {
    cmdline_options.add_options()
      ( "help" , "display help message" )
      ( "suppl" , po::value<string>() , "read supplementary options from config file arg" )
      ( "config,c" , po::value<string>() , "read configuration entirely from file arg (ignores any other command line options)" )
      ( "sample-def-dir" , po::value<string>() , "read sample definitions from directory arg" )
      ( "sample,s" , po::value<string>() , "sample to process" )
      ( "maxevent,n" , po::value<unsigned long>() , "maximum number of events to process" )
      ( "nsplits" , po::value<unsigned int>() , "split input sample into n subjobs\n(each subjob must specify --split)" )
      ( "split" , po::value<unsigned int>() , "subjob number (0 to (nsplits-1))" )
      ( "ref" , po::value<string>() , "named 'source' to treat as reference" )
      ( "rootdir" , po::value<string>() , "base directory for root output\n(full path will be arg/sample/filename)" )
      ( "root,r" , po::value<string>() , "output root filename\n(full path will be basedir/sample/arg)" )
      ( "offline-read" , po::value<string>() , "offline track read mode ('def'=CBNT+truth from wrapper,'cbnt'=CBNT only,'wrap'=wrapper only,'d3pd'=d3pd")
      ( "force-sampledef-filename,i" , po::value<string>() , "force the sample definition filename to be exactly arg (overrides sample and sample-def-dir options)" )
      ( "force-output-filename" , po::value<string>() , "force the output filename to be exactly arg (overrides rootdir and root options)" )
      ( "mcut" , po::value<float>() , (format("barcode matching fraction minimum (default is %|10f|)") % _barcode_matching_minimum_cut).str().c_str() )
      ( "absetacut" , po::value<float>() , (format("maximum track |eta| (default is %|10f|)") % _maximum_track_abseta_cut).str().c_str() )
      ( "pt" , po::value<float>() , (format("minimum track pt to consider, in GeV (default is %|10f| GeV)") % _minimum_track_pt_cut).str().c_str() )
      ( "absd0" , po::value<float>() , (format("maximum track uncorrected d0 to consider, in mm (default is %|10f| mm)") % _maximum_track_absd0_cut).str().c_str() )
      ( "region" , po::value<int>() , "restrict analysis to this FTK region (default is all regions" )
      ( "apply-pileup" , "apply pileup weights from --pileup-map xml file" )
      ( "pileup-scale" , po::value<float>() , "A scale factor for TPileupReweighting::SetDataScaleFactor()" )
      ( "pileup-map" , po::value<string>() , "a ROOT file containing average_mu for data - use iLumiCalc --plots to generate one" )
      ( "pileup-config" , po::value<string>() , "a ROOT file to configure TPileupReweighting" )
      ( "read-offline-si-channels" , po::value<bool>() , "load offline silicon channel data\n(very slow at high luminosity)" )
      ( "read-offline-si-clusters" , po::value<bool>() , "load offline silicon cluster data\n(very slow at high luminosity)" )
      ( "study-si-channels" , po::value<bool>() , "do silicon channel studies" )
      ( "study-si-clusters" , po::value<bool>() , "do silicon clusters studies" )
      ( "jetdr" , po::value<float>() , (format("jet label dr cut (default is %|10f|)") % _jet_label_dr_cut).str().c_str() )
      ( "strictmajority" , "use strict majority (10 or 11 hits) criterion for FTK tracks")
      ( "jetdz0prune" , po::value<float>() , "|dz0| maximum cut (rel to highest pt track) in mm for jet association (default is a very large value, i.e. no cut)")
      ( "train-tagger" , "train b taggers (default is false)" )
      ( "trained-tagger-file" , po::value<string>() , (format("b tagger training file (default is %|s|)")%_trained_tagger_file).str().c_str() )
      ( "noROOT" , "do not write ROOT TObject output")
      ( "noDG" , "do not write DG output")
      ( "noSys" , "only run the nominal analysis, skipping all systematic variations")
      ( "precompute-event-totals" , "determine the number of events per file before processing" )
      ( "out-ntuple" , po::value<string>() , "write AnaEventNtuple to this directory (default no output)" )
      ( "grl" , po::value<string>() , "set a default GRL file (in the future, this will support globbing)" )
      ( "hfor" , "use hfor variable")
      ( "combine-roads" , "Combine roads that have all the same superstrips, but perhaps different hit masks and bit masks (like in Option B), default is false" )
      ( "noVTX" , "JORDAN Turn off the production of vertex plots for ana_wjets/ana_zjets" )
      ( "JES-offset" , po::value<int>() , "Apply JES offset to jet pT: 0=none, 1=down, 2=up (default no offset)" )
      ( "JER-offset" , po::value<int>() , "Apply JER offset to jet pT: 0=none, 1=offset (default no offset)" )
      ( "elec-offset" , po::value<int>() , "Apply correction to electron pT: 0=none, 1=nominal (smear MC), 2=down, 3=up (default no offset)" )
      ( "muon-offset" , po::value<int>() , "Apply correction to muon pT: 0=none, 1=nominal (smear MC), 2=MSdown, 3=MSup, 4=IDdown, 5=IDup (default no offset)" )
      ( "muon-scale" , po::value<int>() , "Apply muon-scale correction to muon pT: 0=none, 201000=coarse 2010, 201011=fine 2010, 201100=coarse 2011, 201111=fine 2011 (default no scale)" )
      ( "muon-type" , po::value<int>() , "Select muon family: 0=STACO combined, 1=STACO loose, 2=STACO tight" )
      ( "elec-trig" , po::value<int>() , "JORDAN Set electron trigger for ana_wjets/ana_zjets. 0=false, 1=L1_EM14 (def), 2=EF_e15_medium, 3=EF_g20_loose" )
      ( "phot-trig" , po::value<int>() , "JORDAN Set photon trigger for ana_wjets/ana_zjets. 0=false, 1=L1_EM14 (def), 2=EF_e15_medium, 3=EF_g20_loose" )
      ( "muon-trig" , po::value<int>() , "JORDAN Set muon trigger for ana_wjets/ana_zjets. 0=false, 1=L1_MU10 (def), 2=EF_mu10_MG, 3=EF_mu13_MG, 4=EF_mu13_MG_tight" )
      ( "ftk-pmap", po::value<string>(), "Set FTK PMAP file, for full efficiency study")
      ( "conf", po::value<string>(), "Choose settings based on a conference targered by the analysis")
      ( "data-range", po::value<string>(), "Choose a data range that will drive the choice of scale/efficiency correction classes")
      ( "release" , po::value<int>() , "Set the release (default=16)" )
      ( "save-ntuples" , po::value<int>(), "Enables saving of DgNtuple objects: 1 = save at reco level, 2 = save at reco and MC level" )
      ( "pdf-reweight" , po::value<int>(), "Enables PDF reweighting. The value is the index of LHA-PDF weight, as saved by FlatNtupler D3PDReader." )
      ;
    po::positional_options_description cmdline_positional_options;
    cmdline_positional_options.add( "sample" , -1 ); // an unnamed argument anywhere on the command line will be treated as a sample name
    po::variables_map cmdline;
    po::parsed_options tmp( po::command_line_parser(argc,argv).options(cmdline_options).allow_unregistered().positional(cmdline_positional_options).run() ); 
    po::store( tmp , cmdline ); 
    po::notify( cmdline ); 
    if( cmdline.count( "help" ) ) { cout << cmdline_options << endl; exit(-1); }
    if( cmdline.count( "suppl" ) ) { 
      const std::string config_filename( cmdline["suppl"].as<string>() );
      if( boost::filesystem::exists( config_filename ) ) { 
        cout << "reading options from config file " << config_filename << endl;
        if( boost::iends_with( config_filename , ".cfg" ) ) {
          AnaConfiguration tmp;
          tmp.load( config_filename );
        } else {
          boost::shared_ptr<ana_streams::istream> config_inf( ana_streams::open_for_read( config_filename ) );
          po::store( po::parse_config_file(*config_inf,cmdline_options) , cmdline );
          po::notify( cmdline );
        }
      } else {
        cout << "supplementary config file " << config_filename << " does not exist. skipping." << endl;
      }
    }
    if( cmdline.count( "config" ) ) {
      AnaConfiguration tmp;
      tmp.load( cmdline["config"].as<string>() );
    } else {
      if( cmdline.count( "sample-def-dir" ) ) { sample_configuration_dir = cmdline["sample-def-dir"].as<string>(); }
      if( cmdline.count( "sample" ) ) { _sample_name = cmdline["sample"].as<string>(); }
      if( cmdline.count( "maxevent" ) ) { 
        _user_max_nevents = cmdline["maxevent"].as<unsigned long>();
        cout << " restricting processed events to at most " << _user_max_nevents << endl;
      }
      if( cmdline.count( "nsplits" ) ) { 
        if( !cmdline.count( "split" ) ) { 
          cout << " specify --split i --nsplits n simultaneously " << endl;
          throw;
        }
        _split_num_files = cmdline["nsplits"].as<unsigned int>();
        _split_offset_file = cmdline["split"].as<unsigned int>();
        if( _split_offset_file >= _split_num_files ) {
          cout << " specify --split i --nsplits n where 0<=i<n " << endl;
          throw;
        }
      }
      if( cmdline.count( "ref" ) ) { _source_to_treat_as_reference = cmdline["ref"].as<string>(); }
      if( cmdline.count( "rootdir" ) ) { _output_dir = cmdline["rootdir"].as<string>(); }
      if( cmdline.count( "root" ) ) { _output_filename = cmdline["root"].as<string>(); }
      if( cmdline.count( "offline-read" ) ) { 
        string option = cmdline["offline-read"].as<string>();
        if( boost::ifind_first( option , "def" ) ) {
          _offline_track_read_mode = AnaConfiguration::OFFLINE_MERGE_WRAPPER_CBNT;
        } else if( boost::ifind_first( option , "cbnt" ) ) {
          _offline_track_read_mode = AnaConfiguration::OFFLINE_FROM_CBNT;
        } else if( boost::ifind_first( option , "wrap" ) ) {
          _offline_track_read_mode = AnaConfiguration::OFFLINE_FROM_WRAPPER;
        } else if( boost::ifind_first( option , "d3pd" ) ) {
          _offline_track_read_mode = AnaConfiguration::OFFLINE_FROM_D3PD;
        } else {
          throw;
        }
      }
      if( cmdline.count( "mcut" ) ) { _barcode_matching_minimum_cut = cmdline["mcut"].as<float>(); }
      if( cmdline.count( "absetacut" ) ) { _maximum_track_abseta_cut = cmdline["absetacut"].as<float>(); }
      if( cmdline.count( "pt" ) ) { _minimum_track_pt_cut = cmdline["pt"].as<float>(); }
      if( cmdline.count( "absd0" ) ) { _maximum_track_absd0_cut = cmdline["absd0"].as<float>(); }
      if( cmdline.count( "region" ) ) { 
        const int region = cmdline["region"].as<int>(); 
        if( region<0 ) { 
          _central_value_track_phi_cut = 0.;
          _half_width_track_phi_cut = M_PI+0.1;
        } else if( region==0 ) {
          _central_value_track_phi_cut = -0.4;
          _half_width_track_phi_cut = 0.2;
          cout << (format(" restricting phi range to %|10g| +/- %|10g|") % _central_value_track_phi_cut % _half_width_track_phi_cut) << endl;
        } else {
          cout << " region cut only implemented for region 0. define your region " << region << " at " << __FILE__ << " line " << __LINE__ << endl;
          throw;
        }
      }
      if( cmdline.count( "study-si-channels" ) ) { _si_channel_studies = cmdline["study-si-channels"].as<bool>(); }
      if( cmdline.count( "study-si-clusters" ) ) { _si_cluster_studies = cmdline["study-si-clusters"].as<bool>(); }
      if( cmdline.count( "read-offline-si-channels" ) ) { _read_offline_si_channels = cmdline["read-offline-si-channels"].as<bool>(); }
      if( cmdline.count( "read-offline-si-clusters" ) ) { _read_offline_si_clusters = cmdline["read-offline-si-clusters"].as<bool>(); }
      if( cmdline.count( "jetdr" ) ) { _jet_label_dr_cut = cmdline["jetdr"].as<float>(); }
      if( cmdline.count( "strictmajority" ) ) { _use_strict_majority = true; }
      if( cmdline.count( "jetdz0prune" ) ) { _apply_jet_dz0_prune = cmdline["jetdz0prune"].as<float>(); }
      if( cmdline.count( "noROOT" ) ) { _save_as_ROOT = false; }
      if( cmdline.count( "noDG" ) ) { _save_as_dg = false; }
      if( cmdline.count( "noSys" ) ) { _skip_systematics = true; }
      if( cmdline.count( "apply-pileup" ) ) { _apply_pileup_weights = true; }
      if( cmdline.count( "pileup-scale" ) ) { _pileup_data_scale = cmdline["pileup-scale"].as<float>(); }
      if( cmdline.count( "pileup-map" ) ) { _pileup_weights_map = cmdline["pileup-map"].as<string>(); }
      if( cmdline.count( "pileup-config" ) ) { _pileup_config_map = cmdline["pileup-config"].as<string>(); }
      if( cmdline.count( "grl" ) ) { _grl_xml_files = cmdline["grl"].as<string>(); }
      if( cmdline.count( "hfor" ) ) { _do_hfor = true;}
      if( cmdline.count( "precompute-event-totals" ) ) { _precompute_number_of_events = true; }
      if( cmdline.count( "force-output-filename" ) ) {
        _force_full_output_path = true; 
        _full_output_path = cmdline["force-output-filename"].as<string>();
      }
      if( cmdline.count( "force-sampledef-filename" ) ) {
        _full_sample_definition_path = cmdline["force-sampledef-filename"].as<string>();
        _sample_name = fs::basename(_full_sample_definition_path);
      }
      _train_tagger = cmdline.count("train-tagger");
      if( cmdline.count("trained-tagger-file") ) {
        _trained_tagger_file = cmdline["trained-tagger-file"].as<string>();
      }
      if( cmdline.count("out-ntuple") ) {
        _ana_event_ntuple_directory = cmdline["out-ntuple"].as<string>();
      }
      _combine_roads = cmdline.count("combine-roads");
      if( cmdline.count( "release" ) ) { _release = cmdline["release"].as<int>(); }
      if( cmdline.count( "JES-offset" ) ) { _JES_offset = cmdline["JES-offset"].as<int>(); }
      if( cmdline.count( "JER-offset" ) ) { _JER_offset = cmdline["JER-offset"].as<int>(); }
      if( cmdline.count( "elec-offset" ) ) { _elec_offset = cmdline["elec-offset"].as<int>(); }
      if( cmdline.count( "muon-offset" ) ) { _muon_offset = cmdline["muon-offset"].as<int>(); }
      if( cmdline.count( "muon-scale" ) ) { _muon_scale = cmdline["muon-scale"].as<int>(); }
      if( cmdline.count( "muon-type" ) ) { _muon_type = detector::select_mcp_type(cmdline["muon-type"].as<int>()); }
      if( cmdline.count( "electron-type" ) ) { _electron_type = detector::select_egamma_type(cmdline["electron-type"].as<int>()); }
      if( cmdline.count( "ftk-pmap" ) ) { _ftk_pmap = cmdline["ftk-pmap"].as<string>(); }
      if( cmdline.count( "conf" ) ) { _conf = CONF::select_conf(cmdline["conf"].as<string>()); }
      if (cmdline.count( "data-range" ) ) { _data_range = DATARANGE::select_range(cmdline["data-range"].as<string>()); }
      if (cmdline.count( "save-ntuples" ) ) { _save_ntuples = cmdline["save-ntuples"].as<int>(); }
      if (cmdline.count( "pdf-reweight" ) ) { _pdf_reweight = cmdline["pdf-reweight"].as<int>(); }
    }
  } catch(...) { 
    cout << " incorrect command line options. syntax is as follows." << endl;
    cout << cmdline_options << endl;
    //  cerr << "unhandled exception while parsing command line." << endl;
    exit(-1);
  }
  if( !_force_full_output_path ) { 
    _full_output_path = _output_dir + "/" + _sample_name + "/" + _output_filename;
  }
  if( _full_sample_definition_path=="" ) { 
    _full_sample_definition_path = sample_configuration_dir + "/" + _sample_name;
  }
  // done reading the command line.
  
  // validate some input
  if( !fs::exists(_full_sample_definition_path) ) {
    cout << " could not find " << _full_sample_definition_path << " sample definition." << endl;
    cout << cmdline_options << endl;
    exit(-1);
  }

  if( _si_channel_studies && !_read_offline_si_channels ) {
    _read_offline_si_channels = true;
    cout << " enabled reading of offline silicon channel data for silicon channel studies." << endl;
  }
  if( _si_cluster_studies && !_read_offline_si_clusters ) {
    _read_offline_si_clusters = true;
    cout << " enabled reading of offline silicon cluster data for silicon cluster studies." << endl;
  }

  // initialize plotting tool. this sets up a filename to which the
  // plots will eventually be saved. saving must be done via a call
  // to dg::root()->do_save_at_exit() when the job finishes.
  dg::root()->set_save_filename( _full_output_path );
  if( !_save_as_ROOT ) { dg::root()->no_root_save(); }
  if( !_save_as_dg ) { dg::root()->no_dg_save(); }

  // The AnaEventMgr synchronizes all of the underlying
  // ntuples (wrapper text files, road finder/merger output, track
  // fitter output, athena ntuples) and provides a simple interface
  // (AnaEvent) to their data.
  AnaEventMgr* evtmgr = AnaEventMgr::instance(); assert(evtmgr);

  // load a sample description and prepare to loop over events
  const bool ok = evtmgr->open_sample();
  if( !ok ) { cout << "open sample " << _sample_name << " failed" << endl; exit(-1) ; }

  if( _ana_event_ntuple_directory != "" ) {
    const bool ok_out = AnaEventMgr::instance()->create_ana_event_ntuple( _ana_event_ntuple_directory );
    if( !ok_out ) { cout << "warning: error opening AnaEventNtuple directory for output." << endl; }
  }

  _configured = true;

  AnaConfiguration cfg;
  cfg.save( DataGraphics::Tools::expand_filename_to_valid_unique(_full_output_path+".cfg") );

  if( true ) {
    cout << " ================================================================ " << endl;
    cout << "  configured as follows: " << endl;
    cout << "    sample: " << _sample_name << endl;
    cout << "    max events: " << _user_max_nevents << endl;
    cout << "    split_num_files " << _split_num_files << endl;
    cout << "    split_offset_file " << _split_offset_file << endl;
    cout << "    source_to_treat_as_reference " << _source_to_treat_as_reference << endl;
    cout << "    output_dir " << _output_dir << endl;
    cout << "    output_filename " << _output_filename << endl;
    cout << "    full_output_path " << _full_output_path << endl;
    cout << "    force_full_output_path " << _force_full_output_path << endl;
    cout << "    full_sample_definition_path " << _full_sample_definition_path << endl;
    cout << "    save_as_ROOT " << _save_as_ROOT << endl;
    cout << "    save_as_dg " << _save_as_dg << endl;
    cout << "    skip_systematics " << _skip_systematics << endl;
    if(_apply_pileup_weights) {
      cout << "    pileup_data_scale " << _pileup_data_scale << endl;
      cout << "    pileup_weights_map " << _pileup_weights_map << endl;
      cout << "    pileup_config_map " << _pileup_config_map << endl;
    }
    cout << "    barcode_matching_minimum_cut " << _barcode_matching_minimum_cut << endl;
    cout << "    maximum_track_abseta_cut " << _maximum_track_abseta_cut << endl;
    cout << "    minimum_track_pt_cut " << _minimum_track_pt_cut << endl;
    cout << "    maximum_track_absd0_cut " << _maximum_track_absd0_cut << endl;
    cout << "    read_offline_si_channels " << _read_offline_si_channels << endl;
    cout << "    read_offline_si_clusters " << _read_offline_si_clusters << endl;
    cout << "    do_si_channel_studies " << _si_channel_studies << endl;
    cout << "    do_si_cluster_studies " << _si_cluster_studies << endl;
    cout << "    jet_label_dr_cut " << _jet_label_dr_cut << endl;
    cout << "    use_strict_majority " << _use_strict_majority << endl;
    cout << "    apply_jet_dz0_prune " << _apply_jet_dz0_prune << endl;
    cout << "    train_tagger " << _train_tagger << endl;
    cout << "    trained_tagger_file " << _trained_tagger_file << endl;
    cout << "    offline_track_read_mode " << _offline_track_read_mode << endl;
    cout << "    precompute_number_of_events " << _precompute_number_of_events << endl;
    cout << "    combine_roads " << _combine_roads << endl;
    cout << "    JES_offset " << _JES_offset << endl;
    cout << "    JER_offset " << _JER_offset << endl;
    cout << "    elec_offset " << _elec_offset << endl;
    cout << "    muon_offset " << _muon_offset << endl;
    cout << "    muon_scale " << _muon_scale << endl;
    cout << "    muon_type " << _muon_type << endl;
    cout << "    electron_type " << _electron_type << endl;
    cout << "    save_ntuples " << _save_ntuples << endl;
    cout << "    pdf_reweight " << _pdf_reweight << endl;
    cout << "    CONF " << _conf << endl;
    cout << "    DATARANGE " << _data_range << endl;
    cout << "    release " << _release << endl;
    cout << "    AnaEventNtuple output directory: " << _ana_event_ntuple_directory << endl;
    cout << " ================================================================ " << endl;
    cout << endl;
  }

}

void
AnaConfiguration::save( const std::string& filename )
{
  ofstream outf( filename.c_str() );
  boost::archive::text_oarchive ar( outf );
  serialize( ar , 1 );
  //  boost::serialization::base_object<AnaConfiguration>(*this) >> ar;
  if( outf ) { cout << "AnaConfiguration saved to " << filename << endl; }
}

void
AnaConfiguration::load( const std::string& filename )
{
  ifstream inf( filename.c_str() );
  boost::archive::text_iarchive ar( inf );
  serialize( ar , 1 );
  //boost::serialization::base_object<AnaConfiguration>(*this) << ar;
  if( inf ) { cout << "AnaConfiguration read from " << filename << endl; }
}
