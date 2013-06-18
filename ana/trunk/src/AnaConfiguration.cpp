
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
#include "DataGraphics/DgCollection.hh"
#include "DataGraphics/DgTools.hh"
#include "TrigFTKAna/Conference.hpp"
#include "TrigFTKAna/AnaConfiguration.hpp"
#include "TrigFTKAna/AnaEventMgr.hpp"
#include "TrigFTKAna/ana_streams.hpp"


using namespace std;
using namespace boost;
using namespace DataGraphics;
namespace fs = boost::filesystem;
namespace po = boost::program_options;


bool AnaConfiguration::_configured = false;
string AnaConfiguration::_sample_name = "";
unsigned long AnaConfiguration::_user_max_nevents = numeric_limits<unsigned long>::max();
unsigned int AnaConfiguration::_split_num_files = 0;
unsigned int AnaConfiguration::_split_offset_file = 0;
std::string AnaConfiguration::_output_dir = "./results";
std::string AnaConfiguration::_output_filename = "%DATE";
std::string AnaConfiguration::_full_output_path = "";
bool AnaConfiguration::_force_full_output_path = false;
std::string AnaConfiguration::_full_sample_definition_path = "";
bool AnaConfiguration::_verbose = false;
bool AnaConfiguration::_skip_systematics = false;
bool AnaConfiguration::_notrigger = false;
bool AnaConfiguration::_noexit = false;
bool AnaConfiguration::_apply_pileup_weights = false;
float AnaConfiguration::_pileup_data_scale = 1.0;
std::string AnaConfiguration::_pileup_weights_map = "";
std::string AnaConfiguration::_pileup_config_map = "";
std::string AnaConfiguration::_grl_xml_files = "";
float AnaConfiguration::_pt_cut = 20;
bool AnaConfiguration::_precompute_number_of_events = false;
int AnaConfiguration::_release = 17;
int AnaConfiguration::_replicas = 100;
int AnaConfiguration::_muon_scale = 3;
detector::MCP_TYPE AnaConfiguration::_muon_type = detector::MCP_STACO_COMBINED;
detector::EGAMMA_TYPE AnaConfiguration::_electron_type = detector::EGAMMA_TIGHTPP;
CONF::ConfType AnaConfiguration::_conf = CONF::LATEST;
DATARANGE::DataRange AnaConfiguration::_data_range = DATARANGE::DtoM;
int AnaConfiguration::_save_ntuples = 0;

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
      ( "sample-def-dir" , po::value<string>() , "read sample definitions from directory arg" )
      ( "sample,s" , po::value<string>() , "sample to process" )
      ( "maxevent,n" , po::value<unsigned long>() , "maximum number of events to process" )
      ( "nsplits" , po::value<unsigned int>() , "split input sample into n subjobs\n(each subjob must specify --split)" )
      ( "split" , po::value<unsigned int>() , "subjob number (0 to (nsplits-1))" )
      ( "rootdir" , po::value<string>() , "base directory for root output\n(full path will be arg/sample/filename)" )
      ( "root,r" , po::value<string>() , "output root filename\n(full path will be basedir/sample/arg)" )
      ( "force-sampledef-filename,i" , po::value<string>() , "force the sample definition filename to be exactly arg (overrides sample and sample-def-dir options)" )
      ( "force-output-filename" , po::value<string>() , "force the output filename to be exactly arg (overrides rootdir and root options)" )
      ( "pt" , po::value<float>() , (format("muon pT cut in GeV (default is %|10f| GeV)") % _pt_cut).str().c_str() )
      ( "apply-pileup" , "apply pileup weights from --pileup-map xml file" )
      ( "pileup-scale" , po::value<float>() , "A scale factor for TPileupReweighting::SetDataScaleFactor()" )
      ( "pileup-map" , po::value<string>() , "a ROOT file containing average_mu for data - use iLumiCalc --plots to generate one" )
      ( "pileup-config" , po::value<string>() , "a ROOT file to configure TPileupReweighting" )
      ( "verbose" , "provide verbose printouts for debugging")
      ( "noSys" , "only run the nominal analysis, skipping all systematic variations")
      ( "noTrig" , "skip the trigger cut (e.g., for cross-channel debugging)")
      ( "noExit" , "allow all destructors to run, instead of immediate exit" )
      ( "precompute-event-totals" , "determine the number of events per file before processing" )
      ( "out-ntuple" , po::value<string>() , "write AnaEventNtuple to this directory (default no output)" )
      ( "grl" , po::value<string>() , "set a default GRL file (in the future, this will support globbing)" )
      ( "muon-scale" , po::value<int>() , "Apply muon-scale correction to muon pT: 0=none, 201000=coarse 2010, 201011=fine 2010, 201100=coarse 2011, 201111=fine 2011 (default no scale)" )
      ( "muon-type" , po::value<int>() , "Select muon family: 0=STACO combined, 1=STACO loose, 2=STACO tight" )
      ( "conf", po::value<string>(), "Choose settings based on a conference targered by the analysis")
      ( "data-range", po::value<string>(), "Choose a data range that will drive the choice of scale/efficiency correction classes")
      ( "release" , po::value<int>() , "Set the release (default=16)" )
      ( "replicas" , po::value<int>() , "Set the number of ntoy replicas" )
      ( "save-ntuples" , po::value<int>(), "Enables saving of DgNtuple objects: 1 = save at reco level, 2 = save at reco and MC level" )
      ;
    po::positional_options_description cmdline_positional_options;
    cmdline_positional_options.add( "sample" , -1 ); // an unnamed argument anywhere on the command line will be treated as a sample name
    po::variables_map cmdline;
    po::parsed_options tmp( po::command_line_parser(argc,argv).options(cmdline_options).allow_unregistered().positional(cmdline_positional_options).run() ); 
    po::store( tmp , cmdline ); 
    po::notify( cmdline ); 
    if( cmdline.count( "help" ) ) { cout << cmdline_options << endl; exit(-1); }
    {
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
      if( cmdline.count( "rootdir" ) ) { _output_dir = cmdline["rootdir"].as<string>(); }
      if( cmdline.count( "root" ) ) { _output_filename = cmdline["root"].as<string>(); }
      if( cmdline.count( "pt" ) ) { _pt_cut = cmdline["pt"].as<float>(); }
      if( cmdline.count( "verbose" ) ) { _verbose = true; }
      if( cmdline.count( "noSys" ) ) { _skip_systematics = true; }
      if( cmdline.count( "noTrig" ) ) { _notrigger = true; }
      if( cmdline.count( "noExit" ) ) { _noexit = true; }
      if( cmdline.count( "apply-pileup" ) ) { _apply_pileup_weights = true; }
      if( cmdline.count( "pileup-scale" ) ) { _pileup_data_scale = cmdline["pileup-scale"].as<float>(); }
      if( cmdline.count( "pileup-map" ) ) { _pileup_weights_map = cmdline["pileup-map"].as<string>(); }
      if( cmdline.count( "pileup-config" ) ) { _pileup_config_map = cmdline["pileup-config"].as<string>(); }
      if( cmdline.count( "grl" ) ) { _grl_xml_files = cmdline["grl"].as<string>(); }
      if( cmdline.count( "precompute-event-totals" ) ) { _precompute_number_of_events = true; }
      if( cmdline.count( "force-output-filename" ) ) {
        _force_full_output_path = true; 
        _full_output_path = cmdline["force-output-filename"].as<string>();
      }
      if( cmdline.count( "force-sampledef-filename" ) ) {
        _full_sample_definition_path = cmdline["force-sampledef-filename"].as<string>();
        _sample_name = fs::basename(_full_sample_definition_path);
      }
      if( cmdline.count( "release" ) ) { _release = cmdline["release"].as<int>(); }
      if( cmdline.count( "replicas" ) ) { _replicas = cmdline["replicas"].as<int>(); }
      if( cmdline.count( "muon-scale" ) ) { _muon_scale = cmdline["muon-scale"].as<int>(); }
      if( cmdline.count( "muon-type" ) ) { _muon_type = detector::select_mcp_type(cmdline["muon-type"].as<int>()); }
      if( cmdline.count( "conf" ) ) { _conf = CONF::select_conf(cmdline["conf"].as<string>()); }
      if (cmdline.count( "data-range" ) ) { _data_range = DATARANGE::select_range(cmdline["data-range"].as<string>()); }
      if (cmdline.count( "save-ntuples" ) ) { _save_ntuples = cmdline["save-ntuples"].as<int>(); }
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

  // initialize plotting tool. this sets up a filename to which the
  // plots will eventually be saved.
  dg::root()->set_save_filename( _full_output_path );

  // The AnaEventMgr synchronizes all of the underlying
  // ntuples (wrapper text files, road finder/merger output, track
  // fitter output, athena ntuples) and provides a simple interface
  // (AnaEvent) to their data.
  AnaEventMgr* evtmgr = AnaEventMgr::instance(); assert(evtmgr);

  // load a sample description and prepare to loop over events
  const bool ok = evtmgr->open_sample();
  if( !ok ) { cout << "open sample " << _sample_name << " failed" << endl; exit(-1) ; }

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
    cout << "    output_dir " << _output_dir << endl;
    cout << "    output_filename " << _output_filename << endl;
    cout << "    full_output_path " << _full_output_path << endl;
    cout << "    force_full_output_path " << _force_full_output_path << endl;
    cout << "    full_sample_definition_path " << _full_sample_definition_path << endl;
    cout << "    skip_systematics " << _skip_systematics << endl;
    if(_apply_pileup_weights) {
      cout << "    pileup_data_scale " << _pileup_data_scale << endl;
      cout << "    pileup_weights_map " << _pileup_weights_map << endl;
      cout << "    pileup_config_map " << _pileup_config_map << endl;
    }
    cout << "    pt_cut " << _pt_cut << endl;
    cout << "    precompute_number_of_events " << _precompute_number_of_events << endl;
    cout << "    muon_scale " << _muon_scale << endl;
    cout << "    muon_type " << _muon_type << endl;
    cout << "    save_ntuples " << _save_ntuples << endl;
    cout << "    CONF " << _conf << endl;
    cout << "    DATARANGE " << _data_range << endl;
    cout << "    release " << _release << endl;
    cout << "    replicas " << _replicas << endl;
    cout << " ================================================================ " << endl;
    cout << endl;
  }

}

void
AnaConfiguration::save( const std::string& filename )
{
  return;
}

void
AnaConfiguration::load( const std::string& filename )
{
  return;
}
