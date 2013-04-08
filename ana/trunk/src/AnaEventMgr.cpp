#include "TrigFTKAna_config.h"

#include "TrigFTKAna/AnaCommon.hpp"
#include <vector>
#include <list>
#include <set>
#include <numeric>
#include <algorithm>
#include <functional>
#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <ext/functional>
#include <boost/version.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/scope_exit.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/function.hpp>
#include <boost/format.hpp>
#include <boost/unordered_map.hpp>
#include <boost/filesystem.hpp> 
#include "TrigFTKAna/ana_streams.hpp"
#include "TrigFTKAna/AnaEventMgr.hpp"
#include "TrigFTKAna/AnaSuperstrip.hpp"
#include "TrigFTKAna/Trees/NtWrapper.hpp"
#include "TrigFTKAna/Trees/NtTrigFTKLib.hpp"
#include "TrigFTKAna/Trees/NtRoad.hpp"
#include "TrigFTKAna/Trees/NtFitter.hpp"
#include "TrigFTKAna/Trees/NtCBNT.hpp"
#include "TrigFTKAna/Trees/NtD3PDphys.hpp"
#include "TrigFTKAna/Trees/NtD3PDphysPeter.hpp"
#include "TrigFTKAna/Trees/NtD3PDphysJetMET.hpp"
#include "TrigFTKAna/Trees/NtPGS.hpp"
#include "TrigFTKAna/Trees/NtJetTag.hpp"
#include "TrigFTKAna/AnaConfiguration.hpp"
#include "TrigFTKAna/EventCountCache.hpp"
#include "TrigFTKAna/IgnoreFileCache.hpp"
#include "TrigFTKAna/AnaVertex.hpp"
#include "TrigFTKAna/AnaEventNtuple.hpp"
#include "DataGraphics/DataGraphics.hh"
#include "DataGraphics/DgXMLSampleDescription.hh"

using namespace std;
typedef pair< string , vector<string> > FilenamePair;

const bool
AnaEventMgr::retrieve_filenames()
{
  // figure out filenames for various ntuples. _config_directory should contain a text
  // file with the relevant filenames in the following format:
  // V format version number (single integer; must be the first line)
  // # comment
  // F "flag_name" "bool_value" (optional; specify one or more flag states; used to pass information such as "is this mc" to the analysis)
  // S "sample_type_name" (optional; specify a sample description string e.g. "ttbar", "w+jets", etc.)
  // N number of events described by this set of ntuples (optional; providing this makes analysis slightly faster the first time)
  // W "wrapper output filename" (optional, up to one allowed)
  // L "TrigFTKLib output filename" (optional, arbitrary number allowed)
  // R "merged road finder output filename" for split 0 (optional, arbitrary number of splits allowed)
  // T "track fitter output filename" for split 0
  // R "merged road finder output filename" for split 1 (arbitrary number of splits allowed)
  // ...
  // R "merged road finder output filename" for split AnaEventMgr::_ftksim_output_splits-1
  // T "track fitter output filename" for split AnaEventMgr::_ftksim_output_splits-1
  // ...
  // C "v13 CBNT ntuple filename" (optional)
  // C14 "v14 CBNT ntuple filename" (optional)
  // B ATLAS JetTagNtuple flat ntuple for b tagging information (optional)
  // DWZ ATLAS SM WZ D3PD flat ntuple for SM W & Z analyses (optional)
  // DEW ATLAS SM EW D3PD flat ntuple for SM WW analysis (and Higgs?) (optional)
  // DTOP ATLAS SM TOP D3PD flat ntuple for top analyses (optional)
  // DSGTOP ATLAS SM SGTOP D3PD flat ntuple for single top analyses (optional)
  // PGS Pretty Good Simulation output file (optional)
  // DPETER ATLAS SM TOP D3PD flat ntuple for W asymmetry analyses in Peter Onyisi's format (optional)
  // DJETMET ATLAS SM TOP D3PD flat ntuple for Jet Et Miss analysis (optional)
  // A TrigFTKAna AnaEventNtuple (AnaEventMgr merged event data or skim produced by previous run of this program).
  //   path to metadata.xml for ntuple.
  // E end of section
  // W "next wrapper output filename" (optional)
  // ...
  const string definition_filename = AnaConfiguration::full_sample_definition_path();
  const unsigned int definition_version = 2u; // this code knows how to parse files from this version or earlier.
  
  boost::shared_ptr<ana_streams::istream> inf( ana_streams::open_for_read( definition_filename ) );

  if( !inf || !(*inf) || !inf->good() || inf->eof() ) {
    cout << "AnaEventMgr::open_sample: could not find definition file " << definition_filename << endl;
    return false;
  }

  unsigned long file_version = 0;
  {
    // check file version
    string line;
    getline( *inf , line );
    const char code( line[0] );
    file_version = std::strtoul( line.c_str() , NULL , 0 );
    if( code != 'V' || file_version > definition_version ) {
      cout << "AnaEventMgr::open_sample: file " << definition_filename << " version is " << file_version
           << " is not understood; use version " << definition_version << " or earlier, or update the code"
           << endl;
      cout << "line: " << line << endl;
      return false;
    }
  }

  DatasetDefinition::SampleFlags sample_flags; // global, cumulative. each ntuple group's flags are set to whatever the current state happens to be.
  DatasetDefinition::SampleNumericFlags sample_numeric_flags; // global, cumulative. each ntuple group's flags are set to whatever the current state happens to be.
  DatasetDefinition::SampleStrings sample_strings; // global, cumulative. each ntuple group's flags are set to whatever the current state happens to be.
  DatasetDefinition::SampleTags sample_tags; // global, cumulative. each ntuple group's tags are set to whatever the current state happens to be.
  _ntuple_filenames_pending.clear();
  _ntuple_filenames_done.clear();
  _ntuple_filenames_failed.clear();
  _ntuple_filenames_skipped.clear();
  unsigned long nevents = 0ul;
  string sample_type;
  map< string , vector<string> > wrapper_filenames;
  map< string , vector<string> > trigftklib_filenames;
  map< string , vector<string> > road_filenames;
  map< string , vector<unsigned int> > road_banks;
  map< string , vector<string> > fitter_filenames;
  map< string , vector<int> > fitter_banks;
  map< string , vector<string> > cbntv13_filenames;
  map< string , vector<string> > cbntv14_filenames;
  map< string , vector<string> > jettagnt_filenames;
  map< string , vector<string> > d3pdnt_filenames;
  map< string , vector<string> > pgs_filenames;
  string aen_filename = "";
  set<string> source_names;
  unsigned long line_number = 1ul; // already read version line
  while( !inf->eof() ) {
    string line;
    size_t ipos,ipos2;
    list<string> tokens;
    getline( *inf , line );
    ++line_number;
    boost::trim( line );
    boost::split( tokens , line, boost::is_any_of("\t \n") );
    if( tokens.empty() ) { continue; }
    const char code( tokens.front()[0] );
    string code_complete( tokens.front() );
    tokens.pop_front();
    std::string source_name = AnaEventMgr::DEFAULT_SOURCE;
    switch( code ) {
    case '#': continue; break;
    case 'F': 
      if( tokens.size() >=1 ) {
        std::string flag_name = tokens.front(); tokens.pop_front();
        const std::string flag_state = tokens.empty() ? "" : tokens.front(); if( !tokens.empty() ) { tokens.pop_front(); }
        boost::algorithm::to_lower(flag_name);
        // is this a numeric flag, or a bool? this can be specified with a code on the flag word, or inferred from the flag data.
        typedef enum { UNKNOWN , BOOL_FLAG , NUMERIC_FLAG , TAG_FLAG , STRING_FLAG } FlagType;
        FlagType flagtype = UNKNOWN;
        if( boost::algorithm::istarts_with( code_complete , "Fb" ) )  { flagtype = BOOL_FLAG; }
        else if( boost::algorithm::istarts_with( code_complete , "Fn" ) )  { flagtype = NUMERIC_FLAG; }
        else if( boost::algorithm::istarts_with( code_complete , "Ft" ) )  { flagtype = TAG_FLAG; }
        else if( boost::algorithm::istarts_with( code_complete , "Fs" ) )  { flagtype = STRING_FLAG; }
        else {
          // default
          flagtype = BOOL_FLAG;
        }
        switch( flagtype ) {
        case BOOL_FLAG:
          {
            const bool says_true = boost::algorithm::icontains( flag_state , "true" ) || boost::algorithm::icontains( flag_state , "1" );
            const bool says_false = boost::algorithm::icontains( flag_state , "false" ) || boost::algorithm::icontains( flag_state , "0" );
            if( (says_true ^ says_false) && flag_name!="" ) { 
              sample_flags[ flag_name ] = says_true;
            } else {
              cout << " could not understand flag setting: " << flag_name << " = " << flag_state << ". ignoring." << endl;
            }
          }
          break;
        case NUMERIC_FLAG:
          {
            double value = std::atof( flag_state.c_str() );
            if( flag_name!="" ) {
              sample_numeric_flags[ flag_name ] = value;
            } else {
              cout << " could not understand flag setting: " << flag_name << " = " << flag_state << ". ignoring." << endl;
            }
          }
          break;
        case STRING_FLAG:
          {
            sample_strings[ flag_name ] = string(boost::algorithm::to_lower_copy( flag_state ));
          }
          break;
        case TAG_FLAG:
          {
            const bool says_true = flag_state=="" || boost::algorithm::icontains( flag_state , "true" ) || boost::algorithm::icontains( flag_state , "1" );
            const bool says_false = flag_state!="" && (boost::algorithm::icontains( flag_state , "false" ) || boost::algorithm::icontains( flag_state , "0" ));
            if( (says_true ^ says_false) && flag_name!="" ) { 
              if( says_true ) { 
                sample_tags.insert( flag_name );
              } else if( says_false ) {
                if( sample_tags.find(flag_name) != sample_tags.end() ) {
                  sample_tags.erase( flag_name );
                }
              }
            } else {
              cout << " could not understand flag setting: " << flag_name << " = " << flag_state << ". ignoring." << endl;
            }
          }
          break;
        default: assert( !"unhandled case" );
        }
      }
      break;
    case 'S':
      if( tokens.size() >=1 ) {
        sample_type = tokens.front();
      }
      break;
    case 'W':
      if( tokens.size()==2 ) { source_name = tokens.back(); }
      wrapper_filenames[source_name].push_back( tokens.front() );
      source_names.insert( source_name );
      break;
    case 'L':
      if( tokens.size()==2 ) { source_name = tokens.back(); }
      trigftklib_filenames[source_name].push_back( tokens.front() );
      source_names.insert( source_name );
      break;
    case 'N':
      nevents = std::strtoul( tokens.front().c_str() , NULL , 0 );
      if( nevents == 0ul || nevents>1000000ul ) {
        cout << "AnaEventMgr::open_sample: warning: line " << line << " specifies nevents == " << nevents << endl;
      }
      break;
    case 'R':
      if( tokens.size()>=2 ) {
        if( tokens.size()>=3 ) { source_name = tokens.back(); }
        road_filenames[source_name].push_back( tokens.front() );
        tokens.pop_front();
        road_banks[source_name].push_back( atoi(tokens.front().c_str()) );
        source_names.insert( source_name );
      } else {
        cout << "AnaEventMgr::open_sample: no filename and bank number on line " << line << " in file " << definition_filename << ". skipping." << endl;
      }      
      break;
    case 'T':
      if( tokens.size()>=2 ) {
        if( tokens.size()>=3 ) { source_name = tokens.back(); }
        fitter_filenames[source_name].push_back( tokens.front() );
        tokens.pop_front();
        if( tokens.front() == "M" ) { // this is a merged bank
          fitter_banks[source_name].push_back( NtFitter::MERGED_BANK );
        } else {
          fitter_banks[source_name].push_back( atoi(tokens.front().c_str()) );
        }
        source_names.insert( source_name );
      } else {
        cout << "AnaEventMgr::open_sample: no filename on line " << line << " in file " << definition_filename << ". skipping." << endl;
      }
      break;
    case 'C':
      if( tokens.size()>=1 ) {
        if( tokens.size()>=2 ) { source_name = tokens.back(); }
        if( boost::starts_with( code_complete , "C14" ) ) {
          cbntv14_filenames[source_name].push_back( ana_streams::root_dcap_filename(tokens.front()) );
        } else {
          cbntv13_filenames[source_name].push_back( ana_streams::root_dcap_filename(tokens.front()) );
        }
        source_names.insert( source_name );
      } else {
        cout << "AnaEventMgr::open_sample: no filename on line " << line << " in file " << definition_filename << ". skipping." << endl;
      }
      break;
    case 'B':
      if( tokens.size()>=1 ) {
        if( tokens.size()>=2 ) { source_name = tokens.back(); }
        jettagnt_filenames[source_name].push_back( ana_streams::root_dcap_filename(tokens.front()) );
        source_names.insert( source_name );
      } else {
        cout << "AnaEventMgr::open_sample: no filename on line " << line << " in file " << definition_filename << ". skipping." << endl;
      }
      break;
    case 'D':
      if( tokens.size()>=1 ) {
        if( tokens.size()>=2 ) { source_name = tokens.back(); }
        std::string fname( ana_streams::root_dcap_filename(tokens.front()) );
        // check for null filename
        if( string(boost::algorithm::trim_copy(fname))!="" ) {
	  if(false) {
	    ;
#ifndef DISABLE_ANCIENT
	  } else if( boost::algorithm::istarts_with(code_complete,"DWZ") ) { 
	    assert(0 && "Disabled for now");
#endif
          } else if( boost::algorithm::istarts_with(code_complete,"DPETER") ) { 
            d3pdnt_filenames[source_name].push_back( NtD3PDphys::filename_to_metaname(fname,NtD3PDphys::PETER) );
            source_names.insert( source_name );
          } else if( boost::algorithm::istarts_with(code_complete,"DJETMET") ) { 
            d3pdnt_filenames[source_name].push_back( NtD3PDphys::filename_to_metaname(fname,NtD3PDphys::JETMET) );
            source_names.insert( source_name );
          } else {
            cout << "AnaEventMgr::open_sample: unrecognized D3PD type on line " << line << " in file " << definition_filename << ". skipping." << endl;
          }
        } else {
          cout << "AnaEventMgr::open_sample: unknown ntuple filename on line " << line << " in file " << definition_filename << ". skipping." << endl;
        }
      } else {
        cout << "AnaEventMgr::open_sample: no filename on line " << line << " in file " << definition_filename << ". skipping." << endl;
      }
      break;
    case 'P':
      if( code_complete == "PGS" ) {
        if( tokens.size()>=1 ) {
          pgs_filenames[source_name].push_back( tokens.front() );
          source_names.insert( source_name );
        } else { 
          cout << "AnaEventMgr::open_sample: no filename on line " << line << " in file " << definition_filename << ". skipping." << endl;
        }
      }
      break;
    case 'A':
      if( tokens.size()>=1 ) {
        // path to metadata filename
        std::string fname( ana_streams::root_dcap_filename(tokens.front()) );
        if( aen_filename != "" ) { 
          cout << " One AnaEventNtuple " << aen_filename << " already specified for this ntuple grouping. " << endl;
          cout << " IGNORED: multiple AnaEventNtuple " << fname << " on line " << line_number << endl;
        } else if( fname == "" ) {
          cout << " IGNORED: empty AnaEventNtuple specification on line " << line << " on line " << line_number << endl;
        } else {
          aen_filename = fname;
        }
      } else {
        cout << "AnaEventMgr::open_sample: no filename on line " << line << " in file " << definition_filename << ". skipping." << endl;
      }
      break;
    case 'E':
      // end section
      // compute number of events if it has not been assigned.
      if( nevents == 0ul && AnaConfiguration::precompute_number_of_events() ) {
        if( aen_filename != "" ) {
          boost::shared_ptr<AnaEventNtuple> tmp( new AnaEventNtuple );
          if( tmp->open_sample( aen_filename , AnaEventNtuple::READ ) ) {
            nevents = tmp->nevents_accepted();
            tmp->close_sample();
          }
        } else if( !cbntv14_filenames.empty() ) {
          BOOST_FOREACH( const FilenamePair& filename_pair , cbntv14_filenames ) {
            BOOST_FOREACH( const string& filename , filename_pair.second ) {
              nevents += _event_count_cache( filename , boost::bind( &NtCBNT::count_events , _1 , NtCBNT::MODE_V14 ) );
            }
          }
        } else if( !cbntv13_filenames.empty() ) {
          BOOST_FOREACH( const FilenamePair& filename_pair , cbntv13_filenames ) {
            BOOST_FOREACH( const string& filename , filename_pair.second ) {
              nevents += _event_count_cache( filename , boost::bind( &NtCBNT::count_events , _1 , NtCBNT::MODE_V13 ) );
            }
          }
        } else if( !wrapper_filenames.empty() ) {
          BOOST_FOREACH( const FilenamePair& filename_pair , wrapper_filenames ) {
            BOOST_FOREACH( const string& filename , filename_pair.second ) {
              nevents += _event_count_cache( filename , boost::bind( &NtWrapper::count_events , _1 ) );
            }
          }
        } else if( !road_filenames.empty() ) {
          BOOST_FOREACH( const FilenamePair& filename_pair , road_filenames ) {
            BOOST_FOREACH( const string& filename , filename_pair.second ) {
              nevents += _event_count_cache( filename , boost::bind( &NtRoad::count_events , _1 ) );
            }
          }
        } else if( !fitter_filenames.empty() ) {
          BOOST_FOREACH( const FilenamePair& filename_pair , fitter_filenames ) {
            BOOST_FOREACH( const string& filename , filename_pair.second ) {
              nevents += _event_count_cache( filename , boost::bind( &NtFitter::count_events , _1 ) );
            }
          }
        } else if( !trigftklib_filenames.empty() ) {
          BOOST_FOREACH( const FilenamePair& filename_pair , trigftklib_filenames ) {
            BOOST_FOREACH( const string& filename , filename_pair.second ) {
              nevents += _event_count_cache( filename , boost::bind( &NtTrigFTKLib::count_events , _1 ) );
            }
          }
        } else if( !jettagnt_filenames.empty() ) {
          BOOST_FOREACH( const FilenamePair& filename_pair , jettagnt_filenames ) {
            BOOST_FOREACH( const string& filename , filename_pair.second ) {
              nevents += _event_count_cache( filename , boost::bind( &NtJetTag::count_events , _1 ) );
            }
          }
        } else if( !d3pdnt_filenames.empty() ) {
          BOOST_FOREACH( const FilenamePair& filename_pair , d3pdnt_filenames ) {
            BOOST_FOREACH( const string& metaname , filename_pair.second ) {
              NtD3PDphys::Type type;
              string filename;
              if( NtD3PDphys::type_and_filename_from_metaname( metaname , type , filename ) ) {
                switch( type ) { 
                case NtD3PDphys::PETER: nevents += _event_count_cache( filename , boost::bind( &NtD3PDphysPeter::count_events , _1 ) ); break;
                case NtD3PDphys::JETMET: nevents += _event_count_cache( filename , boost::bind( &NtD3PDphysJetMET::count_events , _1 ) ); break;
                case NtD3PDphys::NONE: break;
                default:
                  assert( !"unhandled case" );
                }
              }
            }
          }
        } else if( !pgs_filenames.empty() ) {
          BOOST_FOREACH( const FilenamePair& filename_pair , pgs_filenames ) {
            BOOST_FOREACH( const string& filename , filename_pair.second ) {
              nevents += _event_count_cache( filename , boost::bind( &NtPGS::count_events , _1 ) );
            }
          }
        }
      }
      // validate the ntuple description for this section.
      {
        bool section_is_valid = true;
        if( nevents == 0ul && AnaConfiguration::precompute_number_of_events() ) {
          cout << "AnaEventMgr::open_sample: unable to compute number of events for section. please specify or process with instruction to ignore (see --help)" << endl;
        }
        if( aen_filename != "" ) {
          // no other ntuple specification is permitted in this case.
          if( !wrapper_filenames.empty() ||
              !trigftklib_filenames.empty() || 
              !road_filenames.empty() || 
              !road_banks.empty() || 
              !fitter_filenames.empty() || 
              !fitter_banks.empty() || 
              !cbntv13_filenames.empty() || 
              !cbntv14_filenames.empty() || 
              !jettagnt_filenames.empty() ||  
              !d3pdnt_filenames.empty() ||
              !pgs_filenames.empty() ) {
            cout << " WARNING: AnaEventNtuple specified. All other ntuples will be ignored. " << endl;
            wrapper_filenames.clear();
            trigftklib_filenames.clear();
            road_filenames.clear();
            road_banks.clear();
            fitter_filenames.clear();
            fitter_banks.clear();
            cbntv13_filenames.clear();
            cbntv14_filenames.clear();
            jettagnt_filenames.clear();
            d3pdnt_filenames.clear();
            pgs_filenames.clear();
          }
          source_names.clear();
        }
        for( set<string>::const_iterator i=source_names.begin(), f=source_names.end(); i!=f; ++i ) { 
          if( road_filenames.find(*i) != road_filenames.end() ) {
            if( road_banks[*i].size()!=road_filenames[*i].size() ) { 
              cout << "AnaEventMgr::open_sample: mismatch between road filename list and road bank list (ignoring this section for this job)" << endl;
              section_is_valid = false;
            }
          }
          if( fitter_filenames.find(*i) != fitter_filenames.end() ) {
            if( fitter_banks[*i].size()!=fitter_filenames[*i].size() ) { 
              cout << "AnaEventMgr::open_sample: mismatch between fitter filename list and fitter bank list (ignoring this section for this job)" << endl;
              section_is_valid = false;
            }
          }
          // multiple entries will lead to undefined behavior. warn the user. not needed unless the list of filenames goes back to a multimap.
          // note that the following commented code does not compile.
          // if( std::count_if(d3pdnt_filenames.begin(),d3pdnt_filenames.end(), boost::bind( std::equal_to<std::string>() , 
          //                                                                                 boost::bind( __gnu_cxx::select1st<FilenamePair>(_1) ) ,  *i ) ) > 1 ) {
          //   cout << " warning: multiple D3PDs for " << *i << " specified. only one will be retained for analysis." << endl;
          // }
          // if( ( std::count_if(cbntv13_filenames.begin(),cbntv13_filenames.end(), boost::bind( std::equal_to<std::string>() , boost::bind( __gnu_cxx::select1st<FilenamePair>(_1) ) ,  *i ) ) + 
          //       std::count_if(cbntv14_filenames.begin(),cbntv14_filenames.end(), boost::bind( std::equal_to<std::string>() , boost::bind( __gnu_cxx::select1st<FilenamePair>(_1) ) ,  *i ) ) ) > 1 ) {
          //   cout << " warning: multiple CBNTs for " << *i << " specified. only one will be retained for analysis." << endl;
          // }
          // if( std::count_if(wrapper_filenames.begin(),wrapper_filenames.end(), boost::bind( std::equal_to<std::string>() , boost::bind( __gnu_cxx::select1st<FilenamePair>(_1) ) ,  *i ) ) > 1 ) {
          //   cout << " warning: multiple wrapper files for " << *i << " specified. only one will be retained for analysis." << endl;
          // }
          // if( std::count_if(jettagnt_filenames.begin(),jettagnt_filenames.end(), boost::bind( std::equal_to<std::string>() , boost::bind( __gnu_cxx::select1st<FilenamePair>(_1) ) ,  *i ) ) > 1 ) {
          //   cout << " warning: multiple JetTagNtuple files for " << *i << " specified. only one will be retained for analysis." << endl;
          // }
        }
        if( section_is_valid ) {
          _ntuple_filenames_pending.push_back( DatasetDefinition() );
          _ntuple_filenames_pending.back().sample_type = sample_type;
          _ntuple_filenames_pending.back().nevents = nevents;
          _ntuple_filenames_pending.back().sample_flags = sample_flags;
          _ntuple_filenames_pending.back().sample_numeric_flags = sample_numeric_flags;
          _ntuple_filenames_pending.back().sample_strings = sample_strings;
          _ntuple_filenames_pending.back().sample_tags = sample_tags;
          _ntuple_filenames_pending.back().wrapper_filenames = wrapper_filenames;
          _ntuple_filenames_pending.back().trigftklib_filenames = trigftklib_filenames;
          _ntuple_filenames_pending.back().road_filenames = road_filenames;
          _ntuple_filenames_pending.back().road_banks = road_banks;
          _ntuple_filenames_pending.back().fitter_filenames = fitter_filenames;
          _ntuple_filenames_pending.back().fitter_banks = fitter_banks;
          _ntuple_filenames_pending.back().cbntv13_filenames = cbntv13_filenames;
          _ntuple_filenames_pending.back().cbntv14_filenames = cbntv14_filenames;
          _ntuple_filenames_pending.back().jettagnt_filenames = jettagnt_filenames;
          _ntuple_filenames_pending.back().d3pdnt_filenames = d3pdnt_filenames;
          _ntuple_filenames_pending.back().pgs_filenames = pgs_filenames;
          _ntuple_filenames_pending.back().aen_filename = aen_filename;
        } else {
          cout << "ignoring section. " << endl;
        }
      }
      sample_type = "";
      nevents = 0ul;
      wrapper_filenames.clear();
      trigftklib_filenames.clear();
      road_filenames.clear();
      road_banks.clear();
      fitter_filenames.clear();
      fitter_banks.clear();
      cbntv13_filenames.clear();
      cbntv14_filenames.clear();
      jettagnt_filenames.clear();
      d3pdnt_filenames.clear();
      pgs_filenames.clear();
      aen_filename = "";
      source_names.clear();
      break;
    case 0:
    case '\n': 
      // do nothing for empty lines
      break;
    default:
      cout << "AnaEventMgr::open_sample: could not understand line " << line << " in file " << definition_filename << ". skipping." << endl;
    }
  }

  return true;
}

const bool
AnaEventMgr::open_sample()
{
  assert( !_file_open );

  // get sample definition
  const bool ok_definition = retrieve_filenames();
  if( !ok_definition ) { return false; }

  // only use events counts if all ntuples have nonzero values. otherwise ignore them for this job (we will update the event count cache after processing each file).
  const bool use_nevents = ( AnaConfiguration::precompute_number_of_events() && 
                             std::count_if( _ntuple_filenames_pending.begin() , _ntuple_filenames_pending.end() , DatasetDefinition::zeroEventsV() ) == 0 );
  const unsigned long nevents = use_nevents ? std::accumulate( _ntuple_filenames_pending.begin() , _ntuple_filenames_pending.end() , 0ul , DatasetDefinition::accNumEventsV() ) : 0;

  if( true ) {
    if( use_nevents ) { 
      cout << "sample " << AnaConfiguration::full_sample_definition_path() << " open with " << nevents << " events." << endl;
    } else {
      cout << "sample " << AnaConfiguration::full_sample_definition_path() << " open with an undetermined number of events." << endl;
    }
    if( false ) {
      cout << "filenames are:" << endl;
      BOOST_FOREACH( const DatasetDefinition& ntuple_desc , _ntuple_filenames_pending ) {
        if( ntuple_desc.aen_filename != "" ) {
          cout << "    AnaEventNtuple: " << ntuple_desc.aen_filename << endl;
        }
        typedef pair< string , vector<string> > FilenamePair;
        BOOST_FOREACH( const FilenamePair& filename_pair , ntuple_desc.wrapper_filenames ) {
          BOOST_FOREACH( const string& filename , filename_pair.second ) {
            cout << "    wrapper (" << filename_pair.first << "): " << filename << endl;
          }
        }
        BOOST_FOREACH( const FilenamePair& filename_pair , ntuple_desc.trigftklib_filenames ) {
          BOOST_FOREACH( const string& filename , filename_pair.second ) {
            cout << "    TrigFTKLib (" << filename_pair.first << "): " << filename << endl;
          }
        }
        BOOST_FOREACH( const FilenamePair& filename_pair , ntuple_desc.road_filenames ) {
          //           map< string , vector<unsigned int> >::const_iterator ibank=ntuple_desc.road_banks.find(filename_pair.first);
          //           assert( ibank!=ntuple_desc.road_banks.end() );
          //           fixme: print bank id with filename
          BOOST_FOREACH( const string& filename , filename_pair.second ) {
            cout << "    roads (" << filename_pair.first << "): " << filename << endl;
          }
        }
        BOOST_FOREACH( const FilenamePair& filename_pair , ntuple_desc.fitter_filenames ) {
          //           map< string , vector<unsigned int> >::const_iterator ibank=ntuple_desc.fitter_banks.find(filename_pair.first);
          //           assert( ibank!=ntuple_desc.fitter_banks.end() );
          //           fixme: print bank id with filename
          BOOST_FOREACH( const string& filename , filename_pair.second ) {
            cout << "    fitters (" << filename_pair.first << "): " << filename << endl;
          }
        }
        BOOST_FOREACH( const FilenamePair& filename_pair , ntuple_desc.cbntv14_filenames ) {
          BOOST_FOREACH( const string& filename , filename_pair.second ) {
            cout << "    CBNT (v14) (" << filename_pair.first << "): " << filename << endl;
          }
        }
        BOOST_FOREACH( const FilenamePair& filename_pair , ntuple_desc.cbntv13_filenames ) {
          BOOST_FOREACH( const string& filename , filename_pair.second ) {
            cout << "    CBNT (v13) (" << filename_pair.first << "): " << filename << endl;
          }
        }
        BOOST_FOREACH( const FilenamePair& filename_pair , ntuple_desc.jettagnt_filenames ) {
          BOOST_FOREACH( const string& filename , filename_pair.second ) {
            cout << "    JETTAGNTUPLE (" << filename_pair.first << "): " << filename << endl;
          }
        }
        BOOST_FOREACH( const FilenamePair& filename_pair , ntuple_desc.d3pdnt_filenames ) {
          BOOST_FOREACH( const string& filename , filename_pair.second ) {
            cout << "    D3PD: (" << filename_pair.first << "): " << filename << endl;
          }
        }
        BOOST_FOREACH( const FilenamePair& filename_pair , ntuple_desc.pgs_filenames ) {
          BOOST_FOREACH( const string& filename , filename_pair.second ) {
            cout << "    PGS (" << filename_pair.first << "): " << filename << endl;
          }
        }
      }
    }
  }

  // set iterators
  _begin = iterator( 0ul );
  if( use_nevents ) { 
    _end = iterator( nevents );
  } else {
    _end = iterator( 0 );
    _end.set_flag( AnaEventMgr::iterator::ALL_EVENTS ); 
  }

  if( use_nevents ) {
    _progress.set_total_events( nevents ); 
  }
  _progress.set_minimum_silent_time( 30. ); // display progress every x seconds
  _progress.reset();

  return true;
}

void
AnaEventMgr::close_sample()
{
  _begin = _end = iterator( 0ul );
  _progress.display();

  if( _ana_event_ntuple_write.mode() == AnaEventNtuple::WRITE ) { 
    _ana_event_ntuple_write.close_sample();
  }

  // save the sample description xml.
  cout << " [ AnaEventMgr processed " << _n_events_processed << " events." << " ]" << endl;
  save_sample_description();

  // report memory leaks
#ifndef DGMEMORY_OFF
  if( true ) {
    cout << " [ sample close == memory leak check ] " << endl;
    DgMemoryManager::leak_check( cout , DgHeap::SUMMARY );
  }
#endif
}

// open the next set of files for reading
const bool
AnaEventMgr::next_file()
{
  const bool first_file = !_file_open; // flag whether this is first file or not in order to correctly apply subjob split feature

  // close the previous section of files if they are open
  if( _file_open ) {
    // update filename task list
    _ntuple_filenames_done.push_front( _ntuple_filenames_pending.front() );
    _ntuple_filenames_pending.pop_front();
    // close ntuples. update event count cache.
    if( _ana_event_ntuple_read.mode() != AnaEventNtuple::CLOSED ) {
      _ana_event_ntuple_read.close_sample();
    }
    for( map< string , vector< boost::shared_ptr<NtWrapper> > >::iterator i=_nt_wrappers.begin() ,f=_nt_wrappers.end(); i!=f; ++i ) {
      for( vector< boost::shared_ptr<NtWrapper> >::iterator j=i->second.begin(), jf=i->second.end(); j!=jf; ++j ) {
        if( *j ) { 
          _event_count_cache( (*j)->open_filename() , EventCountCache::SupplyComputedEventCount( _file_read_events ) );
          (*j)->end_file(); 
        }
      }
    }
    for( map< string , vector< boost::shared_ptr<NtTrigFTKLib> > >::iterator i=_nt_trigftklibs.begin() ,f=_nt_trigftklibs.end(); i!=f; ++i ) {
      for( vector< boost::shared_ptr<NtTrigFTKLib> >::iterator j=i->second.begin(), jf=i->second.end(); j!=jf; ++j ) {
        if( *j ) { 
          _event_count_cache( (*j)->open_filename() , EventCountCache::SupplyComputedEventCount( _file_read_events ) );
          (*j)->end_file(); 
        }
      }
    }
    for( map< string , vector< boost::shared_ptr<NtRoad> > >::iterator i=_nt_roads.begin() ,f=_nt_roads.end(); i!=f; ++i ) {
      for( vector< boost::shared_ptr<NtRoad> >::iterator j=i->second.begin(), jf=i->second.end(); j!=jf; ++j ) {
        if( *j ) {
          _event_count_cache( (*j)->open_filename() , EventCountCache::SupplyComputedEventCount( _file_read_events ) );
          (*j)->end_file(); 
        }
      }
    }
    for( map< string , vector< boost::shared_ptr<NtFitter> > >::iterator i=_nt_fitters.begin() ,f=_nt_fitters.end(); i!=f; ++i ) {
      for( vector< boost::shared_ptr<NtFitter> >::iterator j=i->second.begin(), jf=i->second.end(); j!=jf; ++j ) {
        if( *j ) { 
          (*j)->end_file(); 
          _event_count_cache( (*j)->open_filename() , EventCountCache::SupplyComputedEventCount( _file_read_events ) );
        }
      }
    }
    for( map< string , vector< boost::shared_ptr<NtCBNT> > >::iterator i=_nt_cbnt.begin() ,f=_nt_cbnt.end(); i!=f; ++i ) {
      for( vector< boost::shared_ptr<NtCBNT> >::iterator j=i->second.begin(), jf=i->second.end(); j!=jf; ++j ) {
        if( *j ) { 
          (*j)->end_file(); 
          _event_count_cache( (*j)->open_filename() , EventCountCache::SupplyComputedEventCount( _file_read_events ) );
        }
      }
    }
    for( map< string , vector< boost::shared_ptr<NtJetTag> > >::iterator i=_nt_jettag.begin() ,f=_nt_jettag.end(); i!=f; ++i ) {
      for( vector< boost::shared_ptr<NtJetTag> >::iterator j=i->second.begin(), jf=i->second.end(); j!=jf; ++j ) {
        if( *j ) { 
          _event_count_cache( (*j)->open_filename() , EventCountCache::SupplyComputedEventCount( _file_read_events ) );
          (*j)->end_file(); 
        }
      }
    }
    for( map< string , vector< boost::shared_ptr<NtD3PDphys> > >::iterator i=_nt_d3pd.begin() ,f=_nt_d3pd.end(); i!=f; ++i ) {
      for( vector< boost::shared_ptr<NtD3PDphys> >::iterator j=i->second.begin(), jf=i->second.end(); j!=jf; ++j ) {
        if( *j ) { 
          _event_count_cache( (*j)->open_filename() , EventCountCache::SupplyComputedEventCount( _file_read_events ) );
          (*j)->end_file(); 
        }
      }
    }
    for( map< string , vector< boost::shared_ptr<NtPGS> > >::iterator i=_nt_pgs.begin() ,f=_nt_pgs.end(); i!=f; ++i ) {
      for( vector< boost::shared_ptr<NtPGS> >::iterator j=i->second.begin(), jf=i->second.end(); j!=jf; ++j ) {
        if( *j ) { 
          _event_count_cache( (*j)->open_filename() , EventCountCache::SupplyComputedEventCount( _file_read_events ) );
          (*j)->end_file(); 
        }
      }
    }
    // update flag
    _file_open = false;
    if( true && !(_ntuple_filenames_done.front().wrapper_filenames.empty()) ) {
      string truncname = _ntuple_filenames_done.front().wrapper_filenames.begin()->second.front();
      if( truncname.size() > 80 ) { truncname.erase( 0 , truncname.size()-80 ); truncname.insert(0,3,'.'); }
      cout << "[ AnaEventMgr: close section: " << truncname << " ]" << endl;
    }
    if( true ) { 
      cout << "[ AnaEventMgr: finish section ### " << _ntuple_filenames_done.size() << " ntuple sections done, " 
           << _ntuple_filenames_failed.size() << " failed, "
           << _ntuple_filenames_pending.size() << " pending. ]" << endl;
    }
  }

  assert( !_file_open );

  while( !_file_open ) { 

    // open the next section of files. for normal running, just take the set of ntuple filenames at the front of the list.
    // but if splitting has been specified (n=AnaConfiguration::split_num_files()>0), skip by n. in order for each subjob
    // to process a different set of files, start the first file after skipping AnaConfiguration::split_offset_file().
    if( AnaConfiguration::split_num_files()>0 ) {
      if( _ntuple_filenames_pending.empty() ) { return false; }
      unsigned int nskip = first_file ? AnaConfiguration::split_offset_file() : (AnaConfiguration::split_num_files()-1);
      for( unsigned int i=0; i!=nskip; ++i ) { 
        _ntuple_filenames_skipped.push_front( _ntuple_filenames_pending.front() );
        _ntuple_filenames_pending.pop_front(); 
        if( _ntuple_filenames_pending.empty() ) { return false; }
      }
    }
  
    if( _ntuple_filenames_pending.empty() ) { return false; }

    const DatasetDefinition& files( _ntuple_filenames_pending.front() );
    set<string> bad_filenames;

    // open the new ntuples. initialize ntuple new interpreters, if necessary
    bool ok_aen = true;
    map<string,vector<bool> > ok_wrappers;
    map<string,vector<bool> > ok_trigftklibs;
    map<string,vector<bool> > ok_roads;
    map<string,vector<bool> > ok_fitters;
    map<string,vector<bool> > ok_cbnt;
    map<string,vector<bool> > ok_jettag;
    map<string,vector<bool> > ok_d3pd;
    map<string,vector<bool> > ok_pgs;
    set<string> source_names;
    if( files.aen_filename != "" ) {
      ok_aen = _ana_event_ntuple_read.open_sample( files.aen_filename , AnaEventNtuple::READ );
    } 
    typedef pair< string , vector<string> > FilenamePair;
    BOOST_FOREACH( const FilenamePair& filename_pair , files.wrapper_filenames ) {
      unsigned int n=0u;
      for( vector<string>::const_iterator i=filename_pair.second.begin(), f=filename_pair.second.end(); i!=f; ++i,++n ) {
        while( _nt_wrappers[filename_pair.first].size()<=n ) { _nt_wrappers[filename_pair.first].push_back( boost::shared_ptr<NtWrapper>(new NtWrapper) ); }
        if( _ignore_filenames_cache.check(*i) ) { ok_wrappers[filename_pair.first].push_back( false ); continue; }
        ok_wrappers[filename_pair.first].push_back( _nt_wrappers[filename_pair.first][n]->begin_file( *i ) );
        if( !ok_wrappers[filename_pair.first].back() ) { bad_filenames.insert( *i ); }
      }
      source_names.insert( filename_pair.first );
    }
    BOOST_FOREACH( const FilenamePair& filename_pair , files.trigftklib_filenames ) {
      unsigned int n=0u;
      for( vector<string>::const_iterator i=filename_pair.second.begin(), f=filename_pair.second.end(); i!=f; ++i,++n ) {
        while( _nt_trigftklibs[filename_pair.first].size()<=n ) { _nt_trigftklibs[filename_pair.first].push_back( boost::shared_ptr<NtTrigFTKLib>(new NtTrigFTKLib) ); }
        if( _ignore_filenames_cache.check(*i) ) { ok_trigftklibs[filename_pair.first].push_back( false ); continue; }
        ok_trigftklibs[filename_pair.first].push_back( _nt_trigftklibs[filename_pair.first][n]->begin_file( *i ) );
        if( !ok_trigftklibs[filename_pair.first].back() ) { bad_filenames.insert( *i ); }
      }
      source_names.insert( filename_pair.first );
    }
    BOOST_FOREACH( const FilenamePair& filename_pair , files.road_filenames ) {
      _nt_roads[filename_pair.first].clear();
      for( unsigned int n=0, fn=filename_pair.second.size(); n!=fn; ++n ) { 
        _nt_roads[filename_pair.first].push_back( boost::shared_ptr<NtRoad>(new NtRoad) ); 
      }
      unsigned int n=0u;
      for( vector<string>::const_iterator i=filename_pair.second.begin(), f=filename_pair.second.end(); i!=f; ++i,++n ) {
        const int ibank = files.road_banks.find(filename_pair.first)->second[n];
        if( _ignore_filenames_cache.check(*i) ) { ok_roads[filename_pair.first].push_back( false ); continue; }
        ok_roads[filename_pair.first].push_back( _nt_roads[filename_pair.first][n]->begin_file( ibank , *i ) );
        if( !ok_roads[filename_pair.first].back() ) { bad_filenames.insert( *i ); }
      }
      source_names.insert( filename_pair.first );
    }
    BOOST_FOREACH( const FilenamePair& filename_pair , files.fitter_filenames ) {
      unsigned int n=0u;
      for( vector<string>::const_iterator i=filename_pair.second.begin(), f=filename_pair.second.end(); i!=f; ++i,++n ) {
        while( _nt_fitters[filename_pair.first].size()<=n ) { _nt_fitters[filename_pair.first].push_back( boost::shared_ptr<NtFitter>(new NtFitter) ); }
        const int ibank = files.fitter_banks.find(filename_pair.first)->second[n];
        if( _ignore_filenames_cache.check(*i) ) { ok_fitters[filename_pair.first].push_back( false ); continue; }
        ok_fitters[filename_pair.first].push_back( _nt_fitters[filename_pair.first][n]->begin_file( ibank , *i ) );
        if( !ok_fitters[filename_pair.first].back() ) { bad_filenames.insert( *i ); }
      }
      source_names.insert( filename_pair.first );
    }
    BOOST_FOREACH( const FilenamePair& filename_pair , files.cbntv14_filenames ) {
      unsigned int n=0u;
      for( vector<string>::const_iterator i=filename_pair.second.begin(), f=filename_pair.second.end(); i!=f; ++i,++n ) {
        while( _nt_cbnt[filename_pair.first].size()<=n ) { _nt_cbnt[filename_pair.first].push_back( boost::shared_ptr<NtCBNT>(new NtCBNT) ); }
        if( _ignore_filenames_cache.check(*i) ) { ok_cbnt[filename_pair.first].push_back( false ); continue; }
        ok_cbnt[filename_pair.first].push_back( _nt_cbnt[filename_pair.first][n]->begin_file( *i , NtCBNT::MODE_V14 ) );
        if( !ok_cbnt[filename_pair.first].back() ) { bad_filenames.insert( *i ); }
      }
      source_names.insert( filename_pair.first );
    }
    BOOST_FOREACH( const FilenamePair& filename_pair , files.cbntv13_filenames ) {
      unsigned int n=0u;
      for( vector<string>::const_iterator i=filename_pair.second.begin(), f=filename_pair.second.end(); i!=f; ++i,++n ) {
        while( _nt_cbnt[filename_pair.first].size()<=n ) { _nt_cbnt[filename_pair.first].push_back( boost::shared_ptr<NtCBNT>(new NtCBNT) ); }
        if( _ignore_filenames_cache.check(*i) ) { ok_cbnt[filename_pair.first].push_back( false ); continue; }
        ok_cbnt[filename_pair.first].push_back( _nt_cbnt[filename_pair.first][n]->begin_file( *i , NtCBNT::MODE_V13 ) );
        if( !ok_cbnt[filename_pair.first].back() ) { bad_filenames.insert( *i ); }
      }
      source_names.insert( filename_pair.first );
    }
    BOOST_FOREACH( const FilenamePair& filename_pair , files.jettagnt_filenames ) {
      unsigned int n=0u;
      for( vector<string>::const_iterator i=filename_pair.second.begin(), f=filename_pair.second.end(); i!=f; ++i,++n ) {
        while( _nt_jettag[filename_pair.first].size()<=n ) { _nt_jettag[filename_pair.first].push_back( boost::shared_ptr<NtJetTag>(new NtJetTag) ); }
        if( _ignore_filenames_cache.check(*i) ) { ok_jettag[filename_pair.first].push_back( false ); continue; }
        ok_jettag[filename_pair.first].push_back( _nt_jettag[filename_pair.first][n]->begin_file( *i ) );
        if( !ok_jettag[filename_pair.first].back() ) { bad_filenames.insert( *i ); }
      }
      source_names.insert( filename_pair.first );
    }
    BOOST_FOREACH( const FilenamePair& filename_pair , files.d3pdnt_filenames ) {
      unsigned int n=0u;
      for( vector<string>::const_iterator i=filename_pair.second.begin(), f=filename_pair.second.end(); i!=f; ++i,++n ) {
        while( _nt_d3pd[filename_pair.first].size()<=n ) { 
          _nt_d3pd[filename_pair.first].push_back( boost::shared_ptr<NtD3PDphys>(new NtD3PDphys) ); 
        }
        NtD3PDphys::Type type;
        string filename;
        if( NtD3PDphys::type_and_filename_from_metaname( *i , type , filename ) ) {
	  std::cerr << "INFO: opening " << filename << std::endl;
	  if( _ignore_filenames_cache.check(filename) ) { 
	    ok_d3pd[filename_pair.first].push_back( false );
          } else if( type==NtD3PDphys::PETER ) { 
            _nt_d3pd[filename_pair.first][n].reset( new NtD3PDphysPeter );
            ok_d3pd[filename_pair.first].push_back( _nt_d3pd[filename_pair.first][n]->begin_file( filename ) );
            if( !ok_d3pd[filename_pair.first].back() ) { bad_filenames.insert( filename ); }
          } else if( type==NtD3PDphys::JETMET ) { 
            _nt_d3pd[filename_pair.first][n].reset( new NtD3PDphysJetMET );
            ok_d3pd[filename_pair.first].push_back( _nt_d3pd[filename_pair.first][n]->begin_file( filename ) );
            if( !ok_d3pd[filename_pair.first].back() ) { bad_filenames.insert( filename ); }
          } else { 
            ok_d3pd[filename_pair.first].push_back( false );
          }
        } else { 
          ok_d3pd[filename_pair.first].push_back( false );
        }
      }
      source_names.insert( filename_pair.first );
    }
    BOOST_FOREACH( const FilenamePair& filename_pair , files.pgs_filenames ) {
      unsigned int n=0u;
      for( vector<string>::const_iterator i=filename_pair.second.begin(), f=filename_pair.second.end(); i!=f; ++i,++n ) {
        while( _nt_pgs[filename_pair.first].size()<=n ) { _nt_pgs[filename_pair.first].push_back( boost::shared_ptr<NtPGS>(new NtPGS) ); }
        if( _ignore_filenames_cache.check(*i) ) { ok_pgs[filename_pair.first].push_back( false ); continue; }
        ok_pgs[filename_pair.first].push_back( _nt_pgs[filename_pair.first][n]->begin_file( *i ) );
        if( !ok_pgs[filename_pair.first].back() ) { bad_filenames.insert( *i ); }
      }
      source_names.insert( filename_pair.first );
    }


    unsigned int n_failures = 0u;
    n_failures += ok_aen ? 0 : 1;
    BOOST_FOREACH( const string& source_name , source_names ) {
      n_failures += std::count_if( ok_wrappers[source_name].begin() , ok_wrappers[source_name].end() , boost::bind( std::equal_to<bool>() , _1 , false ) );
      n_failures += std::count_if( ok_trigftklibs[source_name].begin() , ok_trigftklibs[source_name].end() , boost::bind( std::equal_to<bool>() , _1 , false ) );
      n_failures += std::count_if( ok_roads[source_name].begin() , ok_roads[source_name].end() , boost::bind( std::equal_to<bool>() , _1 , false ) );
      n_failures += std::count_if( ok_fitters[source_name].begin() , ok_fitters[source_name].end() , boost::bind( std::equal_to<bool>() , _1 , false ) );
      n_failures += std::count_if( ok_cbnt[source_name].begin() , ok_cbnt[source_name].end() , boost::bind( std::equal_to<bool>() , _1 , false ) );
      n_failures += std::count_if( ok_jettag[source_name].begin() , ok_jettag[source_name].end() , boost::bind( std::equal_to<bool>() , _1 , false ) );
      n_failures += std::count_if( ok_d3pd[source_name].begin() , ok_d3pd[source_name].end() , boost::bind( std::equal_to<bool>() , _1 , false ) );
      n_failures += std::count_if( ok_pgs[source_name].begin() , ok_pgs[source_name].end() , boost::bind( std::equal_to<bool>() , _1 , false ) );
    }


    if( true ) {
      string label("");
      if( files.aen_filename!="" ) { label = files.aen_filename; }
      if( label == "" ) { 
        BOOST_FOREACH( const string& source_name , source_names ) {
          map<string,vector<string> >::const_iterator i;
          if( label=="" && (i=files.wrapper_filenames.find(source_name))!=files.wrapper_filenames.end() ) {
            if( !(i->second.empty()) ) { label = i->second.front(); }
          }
          if( label=="" && (i=files.trigftklib_filenames.find(source_name))!=files.trigftklib_filenames.end() ) {
            if( !(i->second.empty()) ) { label = i->second.front(); }
          }
          if( label=="" && (i=files.road_filenames.find(source_name))!=files.road_filenames.end() ) {
            if( !(i->second.empty()) ) { label = i->second.front(); }
          }
          if( label=="" && (i=files.fitter_filenames.find(source_name))!=files.fitter_filenames.end() ) {
            if( !(i->second.empty()) ) { label = i->second.front(); }
          }
          if( label=="" && (i=files.cbntv14_filenames.find(source_name))!=files.cbntv14_filenames.end() ) {
            if( !(i->second.empty()) ) { label = i->second.front(); }
          }
          if( label=="" && (i=files.cbntv13_filenames.find(source_name))!=files.cbntv13_filenames.end() ) {
            if( !(i->second.empty()) ) { label = i->second.front(); }
          }
          if( label=="" && (i=files.jettagnt_filenames.find(source_name))!=files.jettagnt_filenames.end() ) {
            if( !(i->second.empty()) ) { label = i->second.front(); }
          }
          if( label=="" && (i=files.d3pdnt_filenames.find(source_name))!=files.d3pdnt_filenames.end() ) {
            NtD3PDphys::Type type;
            string filename;
            if( NtD3PDphys::type_and_filename_from_metaname( i->second.front() , type , filename ) ) {
              if( !(i->second.empty()) ) { label = filename; }
            }
          }
          if( label=="" && (i=files.pgs_filenames.find(source_name))!=files.pgs_filenames.end() ) {
            if( !(i->second.empty()) ) { label = i->second.front(); }
          }
        }
      }
      string truncname = label;
      if( truncname.size() > 80 ) { truncname.erase( 0 , truncname.size()-80 ); truncname.insert(0,3,'.'); }
      cout << "[ AnaEventMgr: new section: " << truncname;
      if( n_failures > 0u ) {
        cout << " (" << n_failures << " failures)";
      } 
      cout << "]" << endl;
    } // end report file open and number of failures

    // if number of failures > 0, one of the files was not valid, so skip the set
    if( n_failures>0u ) {
      cout << " [ AnaEventMgr: warning: skipping section because of " << n_failures << " failures ] " << endl;
      _ntuple_filenames_failed.push_front( _ntuple_filenames_pending.front() );
      _ntuple_filenames_pending.pop_front();
      _ignore_filenames_cache.add( bad_filenames.begin() , bad_filenames.end() );
    } else { 
      _file_open = true;
    }
  } // while a file has not been successfully opened

  _file_read_events = 0ul;
  _sample_type = _ntuple_filenames_pending.front().sample_type;

  return true;
}

const boost::shared_ptr<const AnaEvent>
AnaEventMgr::get_event( const unsigned long& ievent , const bool tried_next_file_in )
{
  ++_n_events_processed;

  if( false ) { 
    cout << " [ memory checkpoint ] " << endl;
    DgMemoryManager::print( cout );
    cout << endl;
  }

  if( _file_open ) { 
    do_end_event_callbacks();
  }

  bool tried_next_file( tried_next_file_in );
  if( !_file_open ) {
    // open the next file
    const bool ok = next_file();
    if( !ok ) { return boost::shared_ptr<const AnaEvent>(); }
    _file_ievent_offset = ievent;
  } // end open the next file

  // get next event. 
  unsigned long file_local_ievent( ievent - _file_ievent_offset );
  // running tally of event memory footprint
  unsigned long nbytes( 0ul );

  set<string> provenance;

  // get wrappers
  unsigned int nfailed_wrappers = 0;
  typedef pair< const string , vector< boost::shared_ptr<NtWrapper> > > WrapperPair;
  BOOST_FOREACH( WrapperPair& fpair , _nt_wrappers ) {
    for( vector< boost::shared_ptr<NtWrapper> >::iterator i=fpair.second.begin(), f=fpair.second.end(); i!=f; ++i ) {
      const bool ok = (*i)->get_event( file_local_ievent );
      provenance.insert( (*i)->open_filename() );
      nbytes += (*i)->nbytes();
      if( !ok ) { ++nfailed_wrappers; }
    }
  }
  if( nfailed_wrappers>0 && _ntuple_filenames_pending.empty() ) {
    cout << " AnaEventMgr::get_event warning: event " << ievent << " (local " << file_local_ievent << "): could not load wrapper for event" << endl;
    return boost::shared_ptr<const AnaEvent>();
  } else if( nfailed_wrappers>0 ) {
    // try to load next file;
    const bool ok = tried_next_file ? false : next_file();
    if( !ok ) { return boost::shared_ptr<const AnaEvent>(); }
    tried_next_file = true;
    _file_ievent_offset = ievent;
    return( get_event(ievent,tried_next_file) );
  }
  
  // get trigftklib output
  unsigned int nfailed_trigftklibs = 0;
  typedef pair< const string , vector< boost::shared_ptr<NtTrigFTKLib> > > TrigFTKLibPair;
  BOOST_FOREACH( TrigFTKLibPair& fpair , _nt_trigftklibs ) {
    for( vector< boost::shared_ptr<NtTrigFTKLib> >::iterator i=fpair.second.begin(), f=fpair.second.end(); i!=f; ++i ) {
      const bool ok = (*i)->get_event( file_local_ievent );
      provenance.insert( (*i)->open_filename() );
      nbytes += (*i)->nbytes();
      if( !ok ) { ++nfailed_trigftklibs; }
    }
  }
  if( nfailed_trigftklibs>0 && _ntuple_filenames_pending.empty() ) {
    cout << " AnaEventMgr::get_event warning: event " << ievent << " (local " << file_local_ievent << "): could not load trigftklib for event" << endl;
    return boost::shared_ptr<const AnaEvent>();
  } else if( nfailed_trigftklibs>0 ) {
    // try to load next file;
    const bool ok = tried_next_file ? false : next_file();
    if( !ok ) { return boost::shared_ptr<const AnaEvent>(); }
    tried_next_file = true;
    _file_ievent_offset = ievent;
    return( get_event(ievent,tried_next_file) );
  }

  // try to load road ntuples
  typedef pair< const string , vector< boost::shared_ptr<NtRoad> > > RoadPair;
  unsigned int nfailed_roads = 0;
  BOOST_FOREACH( RoadPair& fpair , _nt_roads ) {
    for( vector< boost::shared_ptr<NtRoad> >::iterator i=fpair.second.begin(), f=fpair.second.end(); i!=f; ++i ) {
      const bool ok = (*i)->get_event( file_local_ievent );
      provenance.insert( (*i)->open_filename() );
      nbytes += (*i)->nbytes();
      if( !ok ) { ++nfailed_roads; }
    }
  }
  if( nfailed_roads>0 && _ntuple_filenames_pending.empty() ) {
    cout << " AnaEventMgr::get_event warning: event " << ievent << " (local " << file_local_ievent << "): could not load roads for event" << endl;
    cout << "                                 failed " << nfailed_roads << " files" << endl;
    return boost::shared_ptr<const AnaEvent>();
  } else if( nfailed_roads>0 ) {
    // try to load next file;
    const bool ok = tried_next_file ? false : next_file();
    if( !ok ) { return boost::shared_ptr<const AnaEvent>(); }
    tried_next_file = true;
    _file_ievent_offset = ievent;
    return( get_event(ievent,tried_next_file) );
  }

  // try to load fitter ntuples
  unsigned int nfailed_fitters = 0;
  typedef pair< const string , vector< boost::shared_ptr<NtFitter> > > FitterPair;
  BOOST_FOREACH( FitterPair& fpair , _nt_fitters ) {
    for( vector< boost::shared_ptr<NtFitter> >::iterator i=fpair.second.begin(), f=fpair.second.end(); i!=f; ++i ) {
      const bool ok = (*i)->get_event( file_local_ievent );
      provenance.insert( (*i)->open_filename() );
      nbytes += (*i)->nbytes();
      if( !ok ) { ++nfailed_fitters; }
    }
  }
  if( nfailed_fitters>0 && _ntuple_filenames_pending.empty() ) {
    cout << " AnaEventMgr::get_event warning: event " << ievent << " (local " << file_local_ievent << "): could not load fitters for event" << endl;
    cout << "                                 failed " << nfailed_fitters << " files" << endl;
    return boost::shared_ptr<const AnaEvent>();
  } else if( nfailed_fitters>0 ) {
    // try to load next file;
    const bool ok = tried_next_file ? false : next_file();
    if( !ok ) { return boost::shared_ptr<const AnaEvent>(); }
    tried_next_file = true;
    _file_ievent_offset = ievent;
    return( get_event(ievent,tried_next_file) );
  }

  // try to load CBNT ntuples
  unsigned int nfailed_cbnt = 0;
  unsigned int n_inconsistent_cbnt = 0;
  typedef pair< const string , vector< boost::shared_ptr<NtCBNT> > > CBNTPair;
  BOOST_FOREACH( CBNTPair& fpair , _nt_cbnt ) {
    for( vector< boost::shared_ptr<NtCBNT> >::iterator i=fpair.second.begin(), f=fpair.second.end(); i!=f; ++i ) {
      const bool ok = (*i)->get_event( file_local_ievent );
      provenance.insert( (*i)->open_filename() );
      nbytes += (*i)->nbytes();
      if( !ok ) { ++nfailed_cbnt; }
      if( (*i)->inconsistentCBNT() ) { ++n_inconsistent_cbnt; }
    }
  }
  // If we get an inconsistent CBNT then just try the next event
  if( n_inconsistent_cbnt > 0 ) {
    cout << " AnaEventMgr::get_event warning: event " << ievent << " (local " << file_local_ievent << "): inconsistent CBNT. Not filled." << endl;
  }
  if( nfailed_cbnt>0 && _ntuple_filenames_pending.empty() ) {
    cout << " AnaEventMgr::get_event warning: event " << ievent << " (local " << file_local_ievent << "): could not load CBNT for event" << endl;
    cout << "                                 failed " << nfailed_cbnt << " files" << endl;
    return boost::shared_ptr<const AnaEvent>();
  } else if( nfailed_cbnt>0 ) {
    // try to load next file;
    const bool ok = tried_next_file ? false : next_file();
    if( !ok ) { return boost::shared_ptr<const AnaEvent>(); }
    tried_next_file = true;
    _file_ievent_offset = ievent;
    return( get_event(ievent,tried_next_file) );
  }

  // try to load JetTagNtuples
  unsigned int nfailed_jettag = 0;
  typedef pair< const string , vector< boost::shared_ptr<NtJetTag> > > JetTagPair;
  BOOST_FOREACH( JetTagPair& fpair , _nt_jettag ) {
    for( vector< boost::shared_ptr<NtJetTag> >::iterator i=fpair.second.begin(), f=fpair.second.end(); i!=f; ++i ) {
      const bool ok = (*i)->get_event( file_local_ievent );
      provenance.insert( (*i)->open_filename() );
      nbytes += (*i)->nbytes();
      if( !ok ) { ++nfailed_jettag; }
    }
  }
  if( nfailed_jettag>0 && _ntuple_filenames_pending.empty() ) {
    cout << " AnaEventMgr::get_event warning: event " << ievent << " (local " << file_local_ievent << "): could not load JetTag for event" << endl;
    cout << "                                 failed " << nfailed_jettag << " files" << endl;
    return boost::shared_ptr<const AnaEvent>();
  } else if( nfailed_jettag>0 ) {
    // try to load next file;
    const bool ok = tried_next_file ? false : next_file();
    if( !ok ) { return boost::shared_ptr<const AnaEvent>(); }
    tried_next_file = true;
    _file_ievent_offset = ievent;
    return( get_event(ievent,tried_next_file) );
  }

  // try to load D3PDs
  unsigned int nfailed_d3pd = 0;
  typedef pair< const string , vector< boost::shared_ptr<NtD3PDphys> > > D3PDPair;
  BOOST_FOREACH( D3PDPair& fpair , _nt_d3pd ) {
    for( vector< boost::shared_ptr<NtD3PDphys> >::iterator i=fpair.second.begin(), f=fpair.second.end(); i!=f; ++i ) {
      const bool ok = (*i)->get_event( file_local_ievent );
      provenance.insert( (*i)->open_filename() );
      nbytes += (*i)->nbytes();
      if( !ok ) { ++nfailed_d3pd; }
    }
  }
  if( nfailed_d3pd>0 && _ntuple_filenames_pending.empty() ) {
    cout << " AnaEventMgr::get_event warning: event " << ievent << " (local " << file_local_ievent << "): could not load D3PD for event" << endl;
    cout << "                                 failed " << nfailed_d3pd << " files" << endl;
    return boost::shared_ptr<const AnaEvent>();
  } else if( nfailed_d3pd>0 ) {
    // try to load next file;
    const bool ok = tried_next_file ? false : next_file();
    if( !ok ) { return boost::shared_ptr<const AnaEvent>(); }
    tried_next_file = true;
    _file_ievent_offset = ievent;
    return( get_event(ievent,tried_next_file) );
  }

  // get ana event ntuple
  boost::shared_ptr<const AnaEvent> aen_event;
  if( _ana_event_ntuple_read.mode() == AnaEventNtuple::READ ) {
    aen_event = _ana_event_ntuple_read.get_event( file_local_ievent );
    if( !aen_event ) { 
      // try to load next file;
      const bool ok = tried_next_file ? false : next_file();
      if( !ok ) { return boost::shared_ptr<const AnaEvent>(); }
      tried_next_file = true;
      _file_ievent_offset = ievent;
      return( get_event(ievent,tried_next_file) );
    }
  }

  // try to load PGS input
  unsigned int nfailed_pgs = 0;
  typedef pair< const string , vector< boost::shared_ptr<NtPGS> > > PGSPair;
  BOOST_FOREACH( PGSPair& fpair , _nt_pgs ) {
    for( vector< boost::shared_ptr<NtPGS> >::iterator i=fpair.second.begin(), f=fpair.second.end(); i!=f; ++i ) {
      const bool ok = (*i)->get_event( file_local_ievent );
      provenance.insert( (*i)->open_filename() );
      nbytes += (*i)->nbytes();
      if( !ok ) { ++nfailed_pgs; }
    }
  }
  if( nfailed_pgs>0 && _ntuple_filenames_pending.empty() ) {
    cout << " AnaEventMgr::get_event warning: event " << ievent << " (local " << file_local_ievent << "): could not load pgs for event" << endl;
    return boost::shared_ptr<const AnaEvent>();
  } else if( nfailed_pgs>0 ) {
    // try to load next file;
    const bool ok = tried_next_file ? false : next_file();
    if( !ok ) { return boost::shared_ptr<const AnaEvent>(); }
    tried_next_file = true;
    _file_ievent_offset = ievent;
    return( get_event(ievent,tried_next_file) );
  }


  // ntuples have been successfully read. update AnaEvent.
  // ----------------------------------------------------------------

  if( aen_event ) {

    _event_const = aen_event;
    _event.reset();

  } else { 
    // construct a new AnaEvent from the constituent ntuples.

    AnaConfiguration::OfflineTrackReadMode offline_mode = AnaConfiguration::offline_track_read_mode();
    // if offline_mode won't work (e.g. merge or CBNT when the CBNTs are
    // not available), fall back to something that will. warn the user
    // because this introduces file-specific analysis settings that
    // could lead to inconsistent results if some events have CBNTs and
    // others do not.
    if( _nt_cbnt.empty() && offline_mode!=AnaConfiguration::OFFLINE_FROM_WRAPPER && !_nt_wrappers.empty() ) {
      static unsigned int warning_count = 0u;
      if( warning_count++ < 10 ) { 
        cout << "WARNING: offline tracks from CBNTs requested, but no CBNTs are available. falling back to wrapper files." << endl;
        offline_mode=AnaConfiguration::OFFLINE_FROM_WRAPPER;
        if( warning_count == 10 ) { cout << "         additional warnings suppressed." << endl; }
      }
    }

    typedef enum { VERTICES_FROM_D3PD = 0 , VERTEX_FROM_WRAPPER_BEAMLINE = 1 , VERTICES_FROM_D3PD_OR_ELSE_WRAPPER = 2 } VertexReadMode;
    VertexReadMode vertex_read_mode = VERTICES_FROM_D3PD_OR_ELSE_WRAPPER;

    // get run and event number. try all D3PDs, all wrapper ntuples,
    // all FTK ntuples, all CBNTs, all PGS inputs. check that each ntuple
    // is synchronized. if an ntuple does not provide run/event
    // info, warn the user. if the ntuple provides a nonzero
    // run/event number that does not agree with the others,
    // return a null event.
    _event.reset( new AnaEvent );
    _event_const = boost::const_pointer_cast<const AnaEvent>(_event);
    _event->_sample_type = _sample_type;
    _event->_run_number = 0ul;
    _event->_event_number = 0ul;
    _event->_lumi_block = 0ul;
    _event->_average_mu = 0ul;
    _event->_actual_mu = 0ul;
    _event->_bdff = 0;
    _event->_mc_weight = 1.;
    _event->_pdf_weights.clear();
    _event->_mc_channel = 0;
    _event->_mc_hfor = 0;
    _event->_lar_error = false;
    _event->_mcevt_event_scale = 0;
    _event->_mcevt_alphaQCD = 0;
    _event->_mcevt_alphaQED = 0;
    _event->_mcevt_pdf_scale = 0;
    _event->_mcevt_id1 = 0;
    _event->_mcevt_id2 = 0;
    _event->_mcevt_pdf_x1 = 0;
    _event->_mcevt_pdf_x2 = 0;
    _event->_mcevt_pdf1 = 0;
    _event->_mcevt_pdf2 = 0;
    _event->_provenance = provenance;
    _event->_nbytes = nbytes;
    _event->inconsistentCBNT = (bool)n_inconsistent_cbnt;
    BOOST_FOREACH( const D3PDPair& fpair , _nt_d3pd ) {
      for( vector< boost::shared_ptr<NtD3PDphys> >::const_iterator i=fpair.second.begin(), f=fpair.second.end(); i!=f; ++i ) {
        const boost::shared_ptr<NtD3PDphys>& ntuple( *i );
        if( ntuple->run_number()!=0 && ntuple->event_number()!=0 ) {
          if( _event->_run_number==0 && _event->_event_number==0 ) {
            _event->_run_number = ntuple->run_number();
            _event->_event_number = ntuple->event_number();
            _event->_lumi_block = ntuple->lumi_block();
	    _event->_average_mu = ntuple->average_mu();
	    _event->_actual_mu = ntuple->actual_mu();
	    _event->_bdff = ntuple->bdff();
            _event->_mc_weight = ntuple->mc_weight();
	    _event->_pdf_weights = ntuple->pdf_weights();
	    _event->_mc_channel = ntuple->mc_channel();
	    _event->_mc_hfor = ntuple->mc_hfor();
            _event->_lar_error = ntuple->lar_error();
	    _event->_mcevt_event_scale = ntuple->mcevt_event_scale();
	    _event->_mcevt_alphaQCD = ntuple->mcevt_alphaQCD();
	    _event->_mcevt_alphaQED = ntuple->mcevt_alphaQED();
	    _event->_mcevt_pdf_scale = ntuple->mcevt_pdf_scale();
	    _event->_mcevt_id1 = ntuple->mcevt_id1();
	    _event->_mcevt_id2 = ntuple->mcevt_id2();
	    _event->_mcevt_pdf_x1 = ntuple->mcevt_pdf_x1();
	    _event->_mcevt_pdf_x2 = ntuple->mcevt_pdf_x2();
	    _event->_mcevt_pdf1 = ntuple->mcevt_pdf1();
	    _event->_mcevt_pdf2 = ntuple->mcevt_pdf2();
          } else if( _event->_run_number!=ntuple->run_number() || _event->_event_number!=ntuple->event_number() || 
                     _event->_lumi_block!=ntuple->lumi_block() ) {
            cout << " ERROR: " << ntuple->open_filename() << " out of sync with"
                 << " run " << ntuple->run_number()
                 << " event " << ntuple->event_number()
                 << " lumi block " << ntuple->lumi_block()
                 << " mc weight " << ntuple->mc_weight()
		 << " mc channel " << ntuple->mc_channel()
		 << " mc HFOR " << ntuple->mc_hfor()
                 << " ; expected"
                 << " run " << _event->_run_number
                 << " event " << _event->_event_number
                 << " lumi block " << _event->_lumi_block
                 << endl;
            return boost::shared_ptr<const AnaEvent>();
          }
        } else {
          static set<NtD3PDphys*> already_warned;
          if( already_warned.find(ntuple.get())==already_warned.end() ) {
            cout << " warning: empty run, event info for " << ntuple->open_filename() << endl;
            cout << " suppressing further warnings." << endl;
            already_warned.insert(ntuple.get());
          }
        }
      }
    }
    BOOST_FOREACH( const WrapperPair& fpair , _nt_wrappers ) {
      for( vector< boost::shared_ptr<NtWrapper> >::const_iterator i=fpair.second.begin(), f=fpair.second.end(); i!=f; ++i ) {
        const boost::shared_ptr<NtWrapper>& wrap( *i );
        if( wrap->run_number()!=0 && wrap->event_number()!=0 ) {
          if( _event->_run_number==0 && _event->_event_number==0 ) {
            _event->_run_number = wrap->run_number();
            _event->_event_number = wrap->event_number();
          } else if( _event->_run_number!=wrap->run_number() || _event->_event_number!=wrap->event_number() ) {
            cout << " ERROR: " << wrap->open_filename() << " out of sync with"
                 << " run " << wrap->run_number()
                 << " event " << wrap->event_number()
                 << " ; expected"
                 << " run " << _event->_run_number
                 << " event " << _event->_event_number
                 << endl;
            return boost::shared_ptr<const AnaEvent>();
          }
        } else {
          static set<NtWrapper*> already_warned;
          if( already_warned.find(wrap.get())==already_warned.end() ) {
            cout << " warning: empty run, event info for " << wrap->open_filename() << endl;
            cout << " suppressing further warnings." << endl;
            already_warned.insert(wrap.get());
          }
        }
      }
    }
    BOOST_FOREACH( const RoadPair& fpair , _nt_roads ) {
      for( vector< boost::shared_ptr<NtRoad> >::const_iterator i=fpair.second.begin(), f=fpair.second.end(); i!=f; ++i ) {
        const boost::shared_ptr<NtRoad>& ntuple( *i );
        if( ntuple->run_number()!=0 && ntuple->event_number()!=0 ) {
          if( _event->_run_number==0 && _event->_event_number==0 ) {
            _event->_run_number = ntuple->run_number();
            _event->_event_number = ntuple->event_number();
          } else if( _event->_run_number!=ntuple->run_number() || _event->_event_number!=ntuple->event_number() ) {
            cout << " ERROR: " << ntuple->open_filename() << " out of sync with"
                 << " run " << ntuple->run_number()
                 << " event " << ntuple->event_number()
                 << " ; expected"
                 << " run " << _event->_run_number
                 << " event " << _event->_event_number
                 << endl;
            return boost::shared_ptr<const AnaEvent>();
          }
        } else {
          static set<NtRoad*> already_warned;
          if( already_warned.find(ntuple.get())==already_warned.end() ) {
            cout << " warning: empty run, event info for " << ntuple->open_filename() << endl;
            cout << " suppressing further warnings." << endl;
            already_warned.insert(ntuple.get());
          }
        }
      }
    }
    BOOST_FOREACH( const FitterPair& fpair , _nt_fitters ) {
      for( vector< boost::shared_ptr<NtFitter> >::const_iterator i=fpair.second.begin(), f=fpair.second.end(); i!=f; ++i ) {
        const boost::shared_ptr<NtFitter>& ntuple( *i );
        if( ntuple->run_number()!=0 && ntuple->event_number()!=0 ) {
          if( _event->_run_number==0 && _event->_event_number==0 ) {
            _event->_run_number = ntuple->run_number();
            _event->_event_number = ntuple->event_number();
          } else if( _event->_run_number!=ntuple->run_number() || _event->_event_number!=ntuple->event_number() ) {
            cout << " ERROR: " << ntuple->open_filename() << " out of sync with"
                 << " run " << ntuple->run_number()
                 << " event " << ntuple->event_number()
                 << " ; expected"
                 << " run " << _event->_run_number
                 << " event " << _event->_event_number
                 << endl;
            return boost::shared_ptr<const AnaEvent>();
          }
        } else {
          static set<NtFitter*> already_warned;
          if( already_warned.find(ntuple.get())==already_warned.end() ) {
            cout << " warning: empty run, event info for " << ntuple->open_filename() << endl;
            cout << " suppressing further warnings." << endl;
            already_warned.insert(ntuple.get());
          }
        }
      }
    }
    BOOST_FOREACH( const CBNTPair& fpair , _nt_cbnt ) {
      for( vector< boost::shared_ptr<NtCBNT> >::const_iterator i=fpair.second.begin(), f=fpair.second.end(); i!=f; ++i ) {
        const boost::shared_ptr<NtCBNT>& ntuple( *i );
        if( ntuple->run_number()!=0 && ntuple->event_number()!=0 ) {
          if( _event->_run_number==0 && _event->_event_number==0 ) {
            _event->_run_number = ntuple->run_number();
            _event->_event_number = ntuple->event_number();
          } else if( _event->_run_number!=ntuple->run_number() || _event->_event_number!=ntuple->event_number() ) {
            cout << " ERROR: " << ntuple->open_filename() << " out of sync with"
                 << " run " << ntuple->run_number()
                 << " event " << ntuple->event_number()
                 << " ; expected"
                 << " run " << _event->_run_number
                 << " event " << _event->_event_number
                 << endl;
            return boost::shared_ptr<const AnaEvent>();
          }
        } else {
          static set<NtCBNT*> already_warned;
          if( already_warned.find(ntuple.get())==already_warned.end() ) {
            cout << " warning: empty run, event info for " << ntuple->open_filename() << endl;
            cout << " suppressing further warnings." << endl;
            already_warned.insert(ntuple.get());
          }
        }
      }
    }
    BOOST_FOREACH( const JetTagPair& fpair , _nt_jettag ) {
      for( vector< boost::shared_ptr<NtJetTag> >::const_iterator i=fpair.second.begin(), f=fpair.second.end(); i!=f; ++i ) {
        const boost::shared_ptr<NtJetTag>& ntuple( *i );
        if( ntuple->run_number()!=0 && ntuple->event_number()!=0 ) {
          if( _event->_run_number==0 && _event->_event_number==0 ) {
            _event->_run_number = ntuple->run_number();
            _event->_event_number = ntuple->event_number();
          } else if( _event->_run_number!=ntuple->run_number() || _event->_event_number!=ntuple->event_number() ) {
            cout << " ERROR: " << ntuple->open_filename() << " out of sync with"
                 << " run " << ntuple->run_number()
                 << " event " << ntuple->event_number()
                 << " ; expected"
                 << " run " << _event->_run_number
                 << " event " << _event->_event_number
                 << endl;
            return boost::shared_ptr<const AnaEvent>();
          }
        } else {
          static set<NtJetTag*> already_warned;
          if( already_warned.find(ntuple.get())==already_warned.end() ) {
            cout << " warning: empty run, event info for " << ntuple->open_filename() << endl;
            cout << " suppressing further warnings." << endl;
            already_warned.insert(ntuple.get());
          }
        }
      }
    }
    BOOST_FOREACH( const PGSPair& fpair , _nt_pgs ) {
      for( vector< boost::shared_ptr<NtPGS> >::const_iterator i=fpair.second.begin(), f=fpair.second.end(); i!=f; ++i ) {
        const boost::shared_ptr<NtPGS>& nt( *i );
        if( nt->run_number()!=0 && nt->event_number()!=0 ) {
          if( _event->_run_number==0 && _event->_event_number==0 ) {
            _event->_run_number = nt->run_number();
            _event->_event_number = nt->event_number();
          } else if( _event->_run_number!=nt->run_number() || _event->_event_number!=nt->event_number() ) {
            cout << " ERROR: " << nt->open_filename() << " out of sync with"
                 << " run " << nt->run_number()
                 << " event " << nt->event_number()
                 << " ; expected"
                 << " run " << _event->_run_number
                 << " event " << _event->_event_number
                 << endl;
            return boost::shared_ptr<const AnaEvent>();
          }
        } else {
          static set<NtPGS*> already_warned;
          if( already_warned.find(nt.get())==already_warned.end() ) {
            cout << " warning: empty run, event info for " << nt->open_filename() << endl;
            cout << " suppressing further warnings." << endl;
            already_warned.insert(nt.get());
          }
        }
      }
    }

    // check for wrapper and CBNT files. if neither are present,
    // switch to reading offline tracks from D3PDs.
    if( (offline_mode==AnaConfiguration::OFFLINE_FROM_WRAPPER || 
         offline_mode==AnaConfiguration::OFFLINE_FROM_CBNT || 
         offline_mode==AnaConfiguration::OFFLINE_MERGE_WRAPPER_CBNT) && _nt_wrappers.empty() && _nt_cbnt.empty() && !_nt_d3pd.empty() ) {
      offline_mode = AnaConfiguration::OFFLINE_FROM_D3PD;
      static unsigned int nwarnings = 0u;
      const unsigned int max_nwarnings = 3;
      if( nwarnings < max_nwarnings ) { 
        cout << " warning: no wrapper/CBNT present. ignoring instructions to draw offline tracks from them." << endl
             << "          configure to use --offline-read d3pd on your next run" << endl;
        if( nwarnings==max_nwarnings-1 ) { cout << " further warnings suppressed" << endl; }
      }
      ++nwarnings;
    }

    // DONE with event sync. now funnel the data into the AnaEvent objects
    // ================================================================

    // ftk tracks and fitter data
    BOOST_FOREACH( const FitterPair& fpair , _nt_fitters ) {
      const string& source( fpair.first );
      _event->ftk_tracks(source).clear();
      _event->ftk_tracks(source+"_incomplete").clear();
      for( vector< boost::shared_ptr<NtFitter> >::const_iterator i=fpair.second.begin(), f=fpair.second.end(); i!=f; ++i ) {
        for( vector< boost::shared_ptr<const AnaTrack> >::const_iterator j=(*i)->ftk_tracks().begin(), jf=(*i)->ftk_tracks().end(); j!=jf; ++j ) {
          _event->ftk_tracks(source).push_back( *j );
        }
        for( vector< boost::shared_ptr<const AnaTrack> >::const_iterator j=(*i)->ftk_tracks(true).begin(), jf=(*i)->ftk_tracks(true).end(); j!=jf; ++j ) {
          _event->ftk_tracks(source+"_incomplete").push_back( *j );
        }
      }
      // ftk statistics
      _event->ftk_n_tracks_each(source).clear();
      _event->ftk_n_tracks_each(source+"_incomplete").clear();
      for( vector< boost::shared_ptr<NtFitter> >::const_iterator i=fpair.second.begin(), f=fpair.second.end(); i!=f; ++i ) {
        _event->ftk_n_tracks_each(source).push_back( (*i)->ftk_n_tracks() );
        _event->ftk_n_tracks_each(source+"_incomplete").push_back( (*i)->ftk_n_tracks(true) );
      }
      _event->ftk_n_combinations_each(source).clear();
      _event->ftk_n_combinations_each(source+"_incomplete").clear();
      for( vector< boost::shared_ptr<NtFitter> >::const_iterator i=fpair.second.begin(), f=fpair.second.end(); i!=f; ++i ) {
        _event->ftk_n_combinations_each(source).push_back( (*i)->ftk_n_combinations() );
        _event->ftk_n_combinations_each(source+"_incomplete").push_back( (*i)->ftk_n_combinations(true) );
      }
      _event->ftk_n_fits_each(source).clear();
      _event->ftk_n_fits_each(source+"_incomplete").clear();
      for( vector< boost::shared_ptr<NtFitter> >::const_iterator i=fpair.second.begin(), f=fpair.second.end(); i!=f; ++i ) {
        _event->ftk_n_fits_each(source).push_back( (*i)->ftk_n_fits() );
        _event->ftk_n_fits_each(source+"_incomplete").push_back( (*i)->ftk_n_fits(true) );
      }
      _event->ftk_n_fits_majority_each(source).clear();
      _event->ftk_n_fits_majority_each(source+"_incomplete").clear();
      for( vector< boost::shared_ptr<NtFitter> >::const_iterator i=fpair.second.begin(), f=fpair.second.end(); i!=f; ++i ) {
        _event->ftk_n_fits_majority_each(source).push_back( (*i)->ftk_n_fits_majority() );
        _event->ftk_n_fits_majority_each(source+"_incomplete").push_back( (*i)->ftk_n_fits_majority(true) );
      }
      _event->ftk_n_fits_bad_each(source).clear();
      _event->ftk_n_fits_bad_each(source+"_incomplete").clear();
      for( vector< boost::shared_ptr<NtFitter> >::const_iterator i=fpair.second.begin(), f=fpair.second.end(); i!=f; ++i ) {
        _event->ftk_n_fits_bad_each(source).push_back( (*i)->ftk_n_fits_bad() );
        _event->ftk_n_fits_bad_each(source+"_incomplete").push_back( (*i)->ftk_n_fits_bad(true) );
      }
      _event->ftk_n_fits_majority_bad_each(source).clear();
      _event->ftk_n_fits_majority_bad_each(source+"_incomplete").clear();
      for( vector< boost::shared_ptr<NtFitter> >::const_iterator i=fpair.second.begin(), f=fpair.second.end(); i!=f; ++i ) {
        _event->ftk_n_fits_majority_bad_each(source).push_back( (*i)->ftk_n_fits_majority_bad() );
        _event->ftk_n_fits_majority_bad_each(source+"_incomplete").push_back( (*i)->ftk_n_fits_majority_bad(true) );
      }
      _event->ftk_n_fits_hitwarrior_rejected_each(source).clear();
      _event->ftk_n_fits_hitwarrior_rejected_each(source+"_incomplete").clear();
      for( vector< boost::shared_ptr<NtFitter> >::const_iterator i=fpair.second.begin(), f=fpair.second.end(); i!=f; ++i ) {
        _event->ftk_n_fits_hitwarrior_rejected_each(source).push_back( (*i)->ftk_n_fits_hitwarrior_rejected() );
        _event->ftk_n_fits_hitwarrior_rejected_each(source+"_incomplete").push_back( (*i)->ftk_n_fits_hitwarrior_rejected(true) );
      }
    }
    BOOST_FOREACH( const TrigFTKLibPair& fpair , _nt_trigftklibs ) {
      const string& source( fpair.first );
      _event->trigftklib_tracks(source).clear();
      for( vector< boost::shared_ptr<NtTrigFTKLib> >::const_iterator i=fpair.second.begin(), f=fpair.second.end(); i!=f; ++i ) {
        for( vector< boost::shared_ptr<const AnaTrack> >::const_iterator j=(*i)->ftk_tracks().begin(), jf=(*i)->ftk_tracks().end(); j!=jf; ++j ) {
          _event->trigftklib_tracks(source).push_back( *j );
        }
      }
    }
    // offline and truth tracks, silicon channel data. beamline.
    BOOST_FOREACH( const WrapperPair& fpair , _nt_wrappers ) {
      const string& source( fpair.first );
      _event->truth_tracks(source).clear();
      _event->truth_tracks(source).reserve( _event->ftk_tracks(source).size() ); // estimate
      _event->si_channels(source).clear();
      _event->si_clusters(source).clear();
      _event->beamline(source).reset();
      _event->trigger(source).reset(); // minbias trigger info: FIXME play nice with D3PD trigger info
      _event->bad_modules(source).reset();
      if( vertex_read_mode == VERTEX_FROM_WRAPPER_BEAMLINE || vertex_read_mode == VERTICES_FROM_D3PD_OR_ELSE_WRAPPER ) {
        _event->vertices(source).clear();
      }
      if( offline_mode == AnaConfiguration::OFFLINE_FROM_WRAPPER || offline_mode == AnaConfiguration::OFFLINE_MERGE_WRAPPER_CBNT ) { 
        _event->offline_tracks(source).clear();
        _event->offline_tracks(source).reserve( _event->ftk_tracks(source).size() ); // estimate
      }      
      for( vector< boost::shared_ptr<NtWrapper> >::const_iterator i=fpair.second.begin(), f=fpair.second.end(); i!=f; ++i ) {
        const boost::shared_ptr<NtWrapper>& wrapper( *i );
        // get lumi block and bunch number
        _event->_lumi_block = wrapper->lumi_block_number();
        _event->_bunch_number = wrapper->bunch_number();
        // truth tracks.  should use vector<>::assign, but alas.
        BOOST_FOREACH( const boost::shared_ptr<const AnaTrack>& track , wrapper->truth_tracks() ) {
          _event->truth_tracks(source).push_back( track );
        }
        if( offline_mode == AnaConfiguration::OFFLINE_FROM_WRAPPER || offline_mode == AnaConfiguration::OFFLINE_MERGE_WRAPPER_CBNT ) { 
          // offline tracks.  should use vector<>::assign, but alas.
          BOOST_FOREACH( const boost::shared_ptr<const AnaTrack>& track , wrapper->offline_tracks() ) {
            _event->offline_tracks(source).push_back( track );
          }
        }
        if( vertex_read_mode == VERTEX_FROM_WRAPPER_BEAMLINE || vertex_read_mode == VERTICES_FROM_D3PD_OR_ELSE_WRAPPER ) {
          // offline tracks.  should use vector<>::assign, but alas.
          BOOST_FOREACH( const boost::shared_ptr<const AnaVertex>& vertex , wrapper->vertices() ) {
            _event->vertices(source).push_back( vertex );
          }
        }
        // silicon channels
        _event->si_channels(source).assign( wrapper->silicon_channels().begin() , wrapper->silicon_channels().end() );
        // silicon clusters
        _event->si_clusters(source).assign( wrapper->silicon_clusters().begin() , wrapper->silicon_clusters().end() );
        // list of bad modules
        _event->bad_modules(source) = wrapper->bad_modules();
        // beamline
        _event->beamline(source) = wrapper->beamline();
        // trigger
        _event->trigger(source) = wrapper->trigger();
      } // end for each wrapper ntuple
    }
    // idscan and sitrack tracks. l1 roi and atlas cone jets. offline tracks
    // from cbnts.
    BOOST_FOREACH( const CBNTPair& fpair , _nt_cbnt ) {
      const string& source( fpair.first );
      _event->l2_idscan_tracks(source).clear();
      _event->l2_sitrack_tracks(source).clear();
      if( offline_mode == AnaConfiguration::OFFLINE_FROM_CBNT ) { 
        _event->offline_tracks(source).clear();
        _event->offline_tracks(source).reserve( _event->ftk_tracks(source).size() ); // estimate
      }
      for( vector< boost::shared_ptr<NtCBNT> >::const_iterator i=fpair.second.begin(), f=fpair.second.end(); i!=f; ++i ) {
        const boost::shared_ptr<NtCBNT>& cbnt( *i );
        _event->l2_idscan_tracks(source).assign( cbnt->l2_idscan_tracks().begin() , cbnt->l2_idscan_tracks().end() );
        _event->l2_sitrack_tracks(source).assign( cbnt->l2_sitrack_tracks().begin() , cbnt->l2_sitrack_tracks().end() );
        if( offline_mode == AnaConfiguration::OFFLINE_FROM_CBNT ) { 
          _event->offline_tracks(source).assign( cbnt->offline_tracks().begin() , cbnt->offline_tracks().end() );
        } else if( offline_mode == AnaConfiguration::OFFLINE_MERGE_WRAPPER_CBNT ) {
          vector< boost::shared_ptr<const AnaTrack> > tmp;
          merge_wrapper_cbnt( _event->offline_tracks(source).begin() , _event->offline_tracks(source).end() ,
                              cbnt->offline_tracks().begin() , cbnt->offline_tracks().end() ,
                              back_inserter(tmp) );
          _event->offline_tracks(source).swap( tmp );
        }
        _event->l1_roi_jets(source).assign( cbnt->l1_roi_jets().begin() , cbnt->l1_roi_jets().end() );
        _event->atlas_cone_4_tower_jets(source).assign( cbnt->atlas_cone_4_tower_jets().begin() , cbnt->atlas_cone_4_tower_jets().end() );
        _event->atlas_cone_7_tower_jets(source).assign( cbnt->atlas_cone_7_tower_jets().begin() , cbnt->atlas_cone_7_tower_jets().end() );
        _event->atlas_cone_4_truth_jets(source).assign( cbnt->atlas_cone_4_truth_jets().begin() , cbnt->atlas_cone_4_truth_jets().end() );
        _event->atlas_cone_4_truth_pileup_jets(source).assign( cbnt->atlas_cone_4_truth_pileup_jets().begin() , cbnt->atlas_cone_4_truth_pileup_jets().end() );
        // FIXME HACK: fill the truth track endvertex
        cbnt->fill_truth_track_endvertex( _event->truth_tracks(source) );
        cbnt->fill_truth_track_endvertex( _event->offline_tracks(source) );
      } // end for each cbnt ntuple
    }

    // ftk roads and superstrips
    BOOST_FOREACH( const RoadPair& fpair , _nt_roads ) {
      const string& source( fpair.first );
      // ftk roads and lookup table
      _event->ftk_roads(source).clear();
      _event->ftk_roads(source+"_amroads").clear();
      NtRoadBankLookup& road_lookup = _event->_ftk_road_lookup[source];
      NtRoadBankLookup& am_road_lookup = _event->_ftk_road_lookup[source+"_amroads"];
      road_lookup.clear();
      am_road_lookup.clear();
      for( vector< boost::shared_ptr<NtRoad> >::const_iterator i=fpair.second.begin(), f=fpair.second.end(); i!=f; ++i ) {
	if( _event->_ftk_road_nplanes == 0 ) {
	  _event->_ftk_road_nplanes = (*i)->n_planes();
	}
	else if( _event->_ftk_road_nplanes != (*i)->n_planes() ) {
	  cout << " ERROR: " 
	       << " run " << _event->_run_number
	       << " event " << _event->_event_number
	       << " ftk_road_nplanes already set to " << _event->_ftk_road_nplanes 
	       << ", file attempts to reset to " << (*i)->n_planes()
	       << endl;
	}
        for( vector< boost::shared_ptr<const AnaRoad> >::const_iterator j=(*i)->roads().begin(), jf=(*i)->roads().end(); j!=jf; ++j ) {
          _event->ftk_roads(source).push_back( *j );
        }
        for( vector< boost::shared_ptr<const AnaRoad> >::const_iterator j=(*i)->am_roads().begin(), jf=(*i)->am_roads().end(); j!=jf; ++j ) {
          _event->ftk_roads(source+"_amroads").push_back( *j );
        }
	// Clusters
	for( unsigned int ipl = 0; ipl < _event->_ftk_road_nplanes; ipl++ ) {
	  _event->ftk_n_clusters(source).push_back( (*i)->n_clusters(ipl) );
	}
        // merge bank lookup
        const NtRoadBankLookup& bank_lookup( (*i)->bank_road_lookup() );
        for( NtRoadBankLookup::const_iterator j=bank_lookup.begin(), jf=bank_lookup.end(); j!=jf; ++j ) {
          const AnaRoad::BankID& bid( j->first );
          for( NtRoadLookup::const_iterator k=j->second.begin(), kf=j->second.end(); k!=kf; ++k ) {
            road_lookup[ bid ][ k->first ] = k->second; // merge, override
          }
        }
        const NtRoadBankLookup& am_bank_lookup( (*i)->bank_am_road_lookup() );
        for( NtRoadBankLookup::const_iterator j=am_bank_lookup.begin(), jf=am_bank_lookup.end(); j!=jf; ++j ) {
          const AnaRoad::BankID& bid( j->first );
          for( NtRoadLookup::const_iterator k=j->second.begin(), kf=j->second.end(); k!=kf; ++k ) {
            am_road_lookup[ bid ][ k->first ] = k->second; // merge, override
          }
        }
      }
      // cout << " FTK road lookup for source " << source << " loaded with " << road_lookup.size() << " bank ids" << endl;
      // BOOST_FOREACH( const NtRoadBankLookup::value_type& bpair , road_lookup ) { 
      //   cout << " bank " << bpair.first << " with " << bpair.second.size() << " entries " << endl;
      // }
      // ftk superstrips
      _event->ftk_superstrips(source).clear();
      NtRoadLayerLookup& ss_lookup = _event->_ftk_ss_lookup[source];
      ss_lookup.clear();
      for( vector< boost::shared_ptr<NtRoad> >::const_iterator i=fpair.second.begin(), f=fpair.second.end(); i!=f; ++i ) {
        for( vector< boost::shared_ptr<const AnaSuperstrip> >::const_iterator j=(*i)->superstrips().begin(), jf=(*i)->superstrips().end(); j!=jf; ++j ) {
          _event->ftk_superstrips(source).push_back( *j );
          ss_lookup[ (*j)->layer() ][ (*j)->id() ] = *j;
        }
      }
    }

    // fill shared hit info for tracks passing tagging requirements
    BOOST_FOREACH( const FitterPair& fpair , _nt_fitters ) {
      const string& source( fpair.first );
      if( _nt_roads.find( source ) != _nt_roads.end() ) {
        vector< boost::shared_ptr<const AnaTrack> > tmpcoll;
        for( vector< boost::shared_ptr<const AnaTrack> >::const_iterator it=_event->ftk_tracks(source).begin(), ft=_event->ftk_tracks(source).end(); it!=ft; ++it ) {
          const boost::shared_ptr<const AnaTrack>& track( *it );
          if( track->pt() < 1. ) { continue; }
          if( track->b_layer_nhits() < 1 ) { continue; }
          if( std::abs(track->d0()) > 1. ) { continue; }
          tmpcoll.push_back( track );
        }
        NtRoad::fill_ftk_track_nshared( tmpcoll.begin() , tmpcoll.end() , _event->_ftk_road_lookup[source] , _event->_ftk_ss_lookup[source] );
      }
    }

    BOOST_FOREACH( const JetTagPair& fpair , _nt_jettag ) {
      const string& source( fpair.first );
      for( vector< boost::shared_ptr<NtJetTag> >::const_iterator i=fpair.second.begin(), f=fpair.second.end(); i!=f; ++i ) {
        const boost::shared_ptr<NtJetTag>& jettag( *i );
        // find cbnt for this source
        jettag->fill_jet_tag_info( _event->atlas_cone_4_tower_jets(source).begin() , _event->atlas_cone_4_tower_jets(source).end() );
        jettag->fill_jet_tag_info( _event->atlas_cone_4_truth_jets(source).begin() , _event->atlas_cone_4_truth_jets(source).end() );
        jettag->fill_jet_tag_info( _event->atlas_cone_4_truth_pileup_jets(source).begin() , _event->atlas_cone_4_truth_pileup_jets(source).end() );
      } // end for each jettag ntuple
    }

    // make si clusters for FTK tracks (if requested)
    if( _copy_ftk_si_clusters ) {
      // there are no ftk clusters if TrigFTKSim library support is disabled.
#ifdef HAVE_LIBFTKSIM
      BOOST_FOREACH( const FitterPair& fpair , _nt_fitters ) {
        const string& source( fpair.first );
        const bool have_road_data = _nt_roads.find( source ) != _nt_roads.end(); // does not indicate that any roads were actually loaded, only that it was tried.
        vector< boost::shared_ptr<const AnaTrack> > tmpcoll;
        if( false ) { // limit to a subset of tracks to save time
          for( vector< boost::shared_ptr<const AnaTrack> >::const_iterator it=_event->ftk_tracks(source).begin(), ft=_event->ftk_tracks(source).end(); it!=ft; ++it ) {
            const boost::shared_ptr<const AnaTrack>& track( *it );
            // some set of cuts here
            if( track->pt() < 1. ) { continue; }
            if( track->b_layer_nhits() < 1 ) { continue; }
            if( std::abs(track->d0()) > 1. ) { continue; }
            // end cuts
            tmpcoll.push_back( track );
          }
        } else {
          tmpcoll.assign( _event->ftk_tracks(source).begin() , _event->ftk_tracks(source).end() );
        }
        for( vector< boost::shared_ptr<const AnaTrack> >::iterator i=tmpcoll.begin(), f=tmpcoll.end(); i!=f; ++i ) { 
          const boost::shared_ptr<const AnaTrack>& track( *i );
          const unsigned int bank_id = track->ftk_bank_id();
          const unsigned long road_id = track->ftk_road_id();
          const boost::shared_ptr<const AnaRoad> road( have_road_data ? _event->ftk_road_lookup( source , bank_id , road_id ) : boost::shared_ptr<const AnaRoad>() );
          for( int i=0, f=detector::ftk_nlayers(), ilayer=0; i!=f; ++i ) {
            if( !detector::ftkHasPlane(track->ftk_bitmask(),i) ) { continue; }
            // get ss for this layer
            //cout << " i: " << i << " ssid: " << road->superstrip_ids()[i] << endl;
            const boost::shared_ptr<const AnaSuperstrip> ss = have_road_data ? _event->ftk_ss_lookup( source , i , road->superstrip_ids()[i] ) : boost::shared_ptr<const AnaSuperstrip>();
            ++ilayer;
            const bool is_pixel_layer = i<detector::ftk_nlayers_pixels();
            boost::shared_ptr<AnaSiCluster> cluster( boost::make_shared<AnaSiCluster>() );
            cluster->_cluster_type = is_pixel_layer ? AnaSiChannel::PIXEL : AnaSiChannel::STRIP;
            // cluster->_ftk_logical_layer = ilayer-1; // has already been incremented for the next iteration
            cluster->_ftk_logical_layer = i; // has already been incremented for the next iteration
            //cluster->_barrel_ec = ;
            //cluster->_layer_or_disk = i - (is_pixel_layer ? 0 : 3); // FIXME: use endcap mapping from Joe to correctly handle barrel
            if( !is_pixel_layer ) { 
              cluster->_eta_index = ((i%2)==(((i-3)/2)%2)) + 1; // +1 to match offline (1,2) numbering
              // cluster->_eta_index = (ilayer%2)==(((ilayer-3)/2)%2) ? AnaSiChannel::RPHI : AnaSiChannel::STEREO;
              // cluster->_layer_or_disk = (cluster->_layer_or_disk/2); // only floor part!
            }
            //cluster->_phi_module;
            //cluster->_eta_module;
            cluster->_local_x = track->ftk_coords()[detector::ftk_1st_coordinate_for_layer(i)];
            if( is_pixel_layer ) { cluster->_local_z = track->ftk_coords()[detector::ftk_2nd_coordinate_for_layer(i)]; }
            // are these the appropriate translations?
            cluster->_phi_index = static_cast<int>( std::floor(cluster->_local_x) ); 
            if( is_pixel_layer ) { cluster->_eta_index = static_cast<int>( std::floor(cluster->_local_z) ); }
            // cluster->_cluster_width_x = ;
            // cluster->_cluster_width_y = ;
            // cluster->_n_channels = ;
            if( have_road_data && ss ) {
              assert( std::distance( ss->begin_hits() , ss->end_hits() ) == ss->truth().size() );
              int cc = 0;
              BOOST_FOREACH( const MultiTruth& mt , ss->truth() ) {
                // is this the hit used on the track?
                AnaSuperstrip::HitVector::const_iterator iss = ss->begin_hits();
                std::advance( iss , cc++ );
                AnaSuperstrip::ftype ihitfx = (*iss).front();
                AnaSuperstrip::ftype ihitfy = is_pixel_layer ? (*iss).back() : 99999.;
                if( !(std::abs( ihitfx - cluster->_local_x ) < 0.5 && (!is_pixel_layer || (std::abs( ihitfy - cluster->_local_z ) < 0.5) )) ) { continue; }
                MultiTruth::Barcode code;
                MultiTruth::Weight weight;
                const bool ok = mt.best(code,weight);
                if( ok ) { 
                  cluster->_barcode = UniqueBarcode( code.first , code.second );
                }
                //cluster->_truth_pt = ;
                //cluster->_truth_parent_bitmask =;
              }
            } // end if have road data, load truth for this cluster
            track->_si_clusters.push_back( cluster );
          } // end for each layer on the track
        } // end for each track
      } // end for each FTK source name
#endif // HAVE_LIBFTKSIM
    } // end if copy FTK clusters to AnaTracks

    // electrons, muons, MET, jets, trigger, etc. from D3PDs
    BOOST_FOREACH( const D3PDPair& fpair , _nt_d3pd ) {
      const string& source( fpair.first );
      _event->truth_particles(source).clear();
      _event->electrons(source).clear();
      _event->photons(source).clear();
      _event->muons(source).clear();
      if( vertex_read_mode == VERTICES_FROM_D3PD ) { _event->vertices(source).clear(); }
      _event->atlas_anti_kt_4_h1_topo_jets(source).clear();
      _event->atlas_anti_kt_6_h1_topo_jets(source).clear();
      _event->atlas_anti_kt_4_h1_lc_topo_jets(source).clear();
      _event->atlas_anti_kt_6_h1_lc_topo_jets(source).clear();
      _event->atlas_cone_4_truth_jets(source).clear();
      _event->atlas_cone_4_truth_pileup_jets(source).clear();
      _event->atlas_cone_6_truth_jets(source).clear();
      _event->atlas_cone_6_truth_pileup_jets(source).clear();      
      if( offline_mode == AnaConfiguration::OFFLINE_FROM_D3PD ) { _event->offline_tracks(source).clear(); }
      _event->atlas_met_goodness_jets(source).clear();
      _event->missing_et(source).clear();
      _event->trigger(source).reset();
      _event->truth_table(source).reset();
      for( vector< boost::shared_ptr<NtD3PDphys> >::const_iterator i=fpair.second.begin(), f=fpair.second.end(); i!=f; ++i ) {
        const boost::shared_ptr<NtD3PDphys>& d3pd( *i );
        assert( _event->truth_particles(source).empty() || _event->truth_particles(source).begin()!=_event->truth_particles(source).end() );
        assert( _event->electrons(source).empty() || _event->electrons(source).begin()!=_event->electrons(source).end() );
        assert( _event->photons(source).empty() || _event->photons(source).begin()!=_event->photons(source).end() );
        assert( _event->muons(source).empty() || _event->muons(source).begin()!=_event->muons(source).end() );
        assert( _event->atlas_anti_kt_4_h1_topo_jets(source).empty() || _event->atlas_anti_kt_4_h1_topo_jets(source).begin()!=_event->atlas_anti_kt_4_h1_topo_jets(source).end() );
        _event->truth_particles(source).assign( d3pd->truth_particles().begin() , d3pd->truth_particles().end() );
        _event->electrons(source).assign( d3pd->electrons().begin() , d3pd->electrons().end() );
        _event->photons(source).assign( d3pd->photons().begin() , d3pd->photons().end() );
        _event->muons(source).assign( d3pd->muons().begin() , d3pd->muons().end() );
        std::remove_copy_if( d3pd->jets().begin() , d3pd->jets().end() , back_inserter( _event->atlas_anti_kt_4_h1_topo_jets(source) ) ,
                             boost::bind( &AnaJet::Algorithm::type , boost::bind( &AnaJet::algorithm , _1 ) ) != AnaJet::Algorithm::ATLAS_ANTI_KT_CONE_4_H1_TOPO );
        std::remove_copy_if( d3pd->jets().begin() , d3pd->jets().end() , back_inserter( _event->atlas_anti_kt_6_h1_topo_jets(source) ) ,
                             boost::bind( &AnaJet::Algorithm::type , boost::bind( &AnaJet::algorithm , _1 ) ) != AnaJet::Algorithm::ATLAS_ANTI_KT_CONE_6_H1_TOPO );
        std::remove_copy_if( d3pd->jets().begin() , d3pd->jets().end() , back_inserter( _event->atlas_anti_kt_4_h1_lc_topo_jets(source) ) ,
                             boost::bind( &AnaJet::Algorithm::type , boost::bind( &AnaJet::algorithm , _1 ) ) != AnaJet::Algorithm::ATLAS_ANTI_KT_CONE_4_H1_LC_TOPO );
        std::remove_copy_if( d3pd->jets().begin() , d3pd->jets().end() , back_inserter( _event->atlas_anti_kt_6_h1_lc_topo_jets(source) ) ,
                             boost::bind( &AnaJet::Algorithm::type , boost::bind( &AnaJet::algorithm , _1 ) ) != AnaJet::Algorithm::ATLAS_ANTI_KT_CONE_6_H1_LC_TOPO );
        std::remove_copy_if( d3pd->jets().begin() , d3pd->jets().end() , back_inserter( _event->atlas_cone_4_truth_jets(source) ) ,
                             boost::bind( &AnaJet::Algorithm::type , boost::bind( &AnaJet::algorithm , _1 ) ) != AnaJet::Algorithm::ATLAS_CONE_4_TRUTH );
        std::remove_copy_if( d3pd->jets().begin() , d3pd->jets().end() , back_inserter( _event->atlas_cone_4_truth_pileup_jets(source) ) ,
                             boost::bind( &AnaJet::Algorithm::type , boost::bind( &AnaJet::algorithm , _1 ) ) != AnaJet::Algorithm::ATLAS_CONE_4_TRUTH_PILEUP );
        std::remove_copy_if( d3pd->jets().begin() , d3pd->jets().end() , back_inserter( _event->atlas_cone_6_truth_jets(source) ) ,
                             boost::bind( &AnaJet::Algorithm::type , boost::bind( &AnaJet::algorithm , _1 ) ) != AnaJet::Algorithm::ATLAS_CONE_6_TRUTH );
        std::remove_copy_if( d3pd->jets().begin() , d3pd->jets().end() , back_inserter( _event->atlas_cone_6_truth_pileup_jets(source) ) ,
                             boost::bind( &AnaJet::Algorithm::type , boost::bind( &AnaJet::algorithm , _1 ) ) != AnaJet::Algorithm::ATLAS_CONE_6_TRUTH_PILEUP );
        std::remove_copy_if( d3pd->jets().begin() , d3pd->jets().end() , back_inserter( _event->atlas_met_goodness_jets(source) ) ,
                             boost::bind( &AnaJet::Algorithm::type , boost::bind( &AnaJet::algorithm , _1 ) ) != AnaJet::Algorithm::ATLAS_MET_GOODNESS );
        if( vertex_read_mode == VERTICES_FROM_D3PD || (vertex_read_mode == VERTICES_FROM_D3PD_OR_ELSE_WRAPPER && !(d3pd->vertices().empty()) ) ) {
          _event->vertices(source).assign( d3pd->vertices().begin() , d3pd->vertices().end() );
        }
        if( offline_mode == AnaConfiguration::OFFLINE_FROM_D3PD ) { _event->offline_tracks(source).assign( d3pd->offline_tracks().begin() , 
                                                                                                           d3pd->offline_tracks().end() ); }
        _event->missing_et(source).assign( d3pd->missing_et().begin() , d3pd->missing_et().end() );
        _event->trigger(source) = d3pd->trigger();
        _event->truth_table(source) = d3pd->truth_table();
      } // end for each source
    } // end for each d3pd for this event

    // offline and truth tracks, silicon channel data. beamline.
    BOOST_FOREACH( const PGSPair& fpair , _nt_pgs ) {
      const string& source( fpair.first );
      _event->electrons(source).clear();
      _event->muons(source).clear();
      _event->pgs_jets(source).clear();
      _event->missing_et(source).clear();
      for( vector< boost::shared_ptr<NtPGS> >::const_iterator i=fpair.second.begin(), f=fpair.second.end(); i!=f; ++i ) {
        const boost::shared_ptr<NtPGS>& nt( *i );
        BOOST_FOREACH( const boost::shared_ptr<const AnaElectron>& electron , nt->electrons() ) {
          _event->electrons(source).push_back( electron );
        }
        BOOST_FOREACH( const boost::shared_ptr<const AnaMuon>& muon , nt->muons() ) {
          _event->muons(source).push_back( muon );
        }
        BOOST_FOREACH( const boost::shared_ptr<const AnaJet>& jet , nt->jets() ) {
          _event->pgs_jets(source).push_back( jet );
        }
        BOOST_FOREACH( const boost::shared_ptr<const AnaMET>& met , nt->mets() ) {
          _event->missing_et(source).push_back( met );
        }
      } // end for each nt ntuple
    }

    if( true ) { // compute highest sum pt vertex for each source
      typedef std::pair< std::string , std::vector< boost::shared_ptr<const AnaVertex> > > pairtype;
      BOOST_FOREACH( const pairtype& p , _event->_vertices ) {
        const std::string& source = p.first;
        const std::vector< boost::shared_ptr<const AnaVertex> >& vertices = p.second;
        _event->highest_sum_pt_vertex(source).reset();
	// highest sum pt
        std::vector< boost::shared_ptr<const AnaVertex> >::const_iterator ihspt = std::max_element( vertices.begin() , vertices.end() , 
                                                                                                    boost::bind(&AnaVertex::sum_pt,_1) < boost::bind(&AnaVertex::sum_pt,_2) );
        if( ihspt != vertices.end() ) { 
          _event->highest_sum_pt_vertex(source) = *ihspt; 
        }
      }
    } // end find highest sum pt vertex for each source

    // update sample flags
    _event->_sample_flags = _ntuple_filenames_pending.front().sample_flags;
    _event->_sample_numeric_flags = _ntuple_filenames_pending.front().sample_numeric_flags;
    _event->_sample_strings = _ntuple_filenames_pending.front().sample_strings;
    _event->_sample_tags = _ntuple_filenames_pending.front().sample_tags;
    _event->_evt_mgr_index = ievent;

  } // end if use anaevent or else merge ntuples into an AnaEvent

  _progress.update( 1 , _event_const->nbytes() ); // sometimes writes status report to cout

  do_begin_event_callbacks();

  ++_file_read_events;

  if( true ) {
    // histogram performance.
    DataGraphics::dg::down( "performance" , "analysis performance statistics" ); BOOST_SCOPE_EXIT() { DataGraphics::dg::up(); } BOOST_SCOPE_EXIT_END;
#ifndef DGMEMORY_OFF
    DataGraphics::dg::fillh( "total_mem" , 1000 , 0 , 100 , DgMemoryManager::num_bytes_allocated()/(1024.*1024) , "RAM (MB)" );
    DataGraphics::dg::fillh( "total_alloc" , 1000 , 0 , 100 , DgMemoryManager::num_allocations()/1E6 , "MILLION ALLOCATIONS" );
#endif
    DataGraphics::dg::fillh( "event_rate" , 250 , 0 , 500 , _progress.mean_lap_rate() , "EVENT RATE (Hz)" );
    DataGraphics::dg::fillh( "data_rate" , 100 , 0 , 1000 , _progress.mean_byte_rate()/1024. , "DATA RATE (kB/sec)" );
  }

  return( _event_const );
}


// retrieve the list of filenames relevant for the given event
const std::vector<std::string>
AnaEventMgr::source_filenames( const AnaEventMgr::iterator& i )
{
  std::vector<std::string> result;
  result.reserve( 20 );
  // if the iterator points to the currently loaded event, this is easy.
  if( i._ievent == _event_const->_evt_mgr_index && !_ntuple_filenames_pending.empty() ) {
    const DatasetDefinition& ntf( _ntuple_filenames_pending.front() );
    typedef pair< string , vector<string> > FilenamePair;
    BOOST_FOREACH( const FilenamePair& fpair , ntf.wrapper_filenames ) {
      copy( fpair.second.begin() , fpair.second.end() , back_inserter(result) );
    }
    BOOST_FOREACH( const FilenamePair& fpair , ntf.trigftklib_filenames ) {
      copy( fpair.second.begin() , fpair.second.end() , back_inserter(result) );
    }
    BOOST_FOREACH( const FilenamePair& fpair , ntf.road_filenames ) {
      copy( fpair.second.begin() , fpair.second.end() , back_inserter(result) );
    }
    BOOST_FOREACH( const FilenamePair& fpair , ntf.fitter_filenames ) {
      copy( fpair.second.begin() , fpair.second.end() , back_inserter(result) );
    }
    BOOST_FOREACH( const FilenamePair& fpair , ntf.cbntv14_filenames ) {
      copy( fpair.second.begin() , fpair.second.end() , back_inserter(result) );
    }
    BOOST_FOREACH( const FilenamePair& fpair , ntf.cbntv13_filenames ) {
      copy( fpair.second.begin() , fpair.second.end() , back_inserter(result) );
    }
    BOOST_FOREACH( const FilenamePair& fpair , ntf.jettagnt_filenames ) {
      copy( fpair.second.begin() , fpair.second.end() , back_inserter(result) );
    }
    return result;
  }
  // cases where the iterator is not the current event are unimplemented.
  assert( !"unimplemented" );
  return result;
}

// retrieve the run number and event numbers for the given event
const unsigned long
AnaEventMgr::source_run_number( const AnaEventMgr::iterator& i )
{
  // if the iterator points to the currently loaded event, this is easy.
  if( i._ievent == _event_const->_evt_mgr_index && !_ntuple_filenames_pending.empty() ) {
    return( _event_const->run_number() );
  }
  // cases where the iterator is not the current event are unimplemented.
  assert( !"unimplemented" );
}

const unsigned long
AnaEventMgr::source_event_number( const AnaEventMgr::iterator& i )
{
  // if the iterator points to the currently loaded event, this is easy.
  if( i._ievent == _event_const->_evt_mgr_index && !_ntuple_filenames_pending.empty() ) {
    return( _event_const->event_number() );
  }
  // cases where the iterator is not the current event are unimplemented.
  assert( !"unimplemented" );
}


template< typename insertIterT >
void
AnaEventMgr::merge_wrapper_cbnt( std::vector< boost::shared_ptr<const AnaTrack> >::const_iterator beginWrapper , 
                                 std::vector< boost::shared_ptr<const AnaTrack> >::const_iterator endWrapper ,
                                 std::vector< boost::shared_ptr<const AnaTrack> >::const_iterator beginCBNT , 
                                 std::vector< boost::shared_ptr<const AnaTrack> >::const_iterator endCBNT ,
                                 insertIterT iOutput )
{
  typedef boost::unordered_multimap< AnaTrack::ftype , boost::shared_ptr<const AnaTrack> > SubsetMap;
  // verbose debugging for development
  const bool debug = false;
  // match wrapper info to each CBNT track and merge into output collection.
  unsigned long ncbnt_tracks = 0ul;
  unsigned long nmatched_tracks = 0ul;
  if( false ) { // do old algorithm
    if( debug ) {
      cout << " wrapper tracks: " << endl;
      for( std::vector< boost::shared_ptr<const AnaTrack> >::const_iterator iWrapper=beginWrapper; iWrapper!=endWrapper; ++iWrapper ) {
        cout << "wrap ";
        (*iWrapper)->print();
      }
      cout << " CBNT tracks: " << endl;
      for( std::vector< boost::shared_ptr<const AnaTrack> >::const_iterator iCBNT=beginCBNT; iCBNT!=endCBNT; ++iCBNT ) {
        cout << "cbnt ";
        (*iCBNT)->print();
      }
    } // end debug conditional
    for( std::vector< boost::shared_ptr<const AnaTrack> >::const_iterator iCBNT=beginCBNT; iCBNT!=endCBNT; ++iCBNT ) {
      const boost::shared_ptr<const AnaTrack>& cbnt_track( *iCBNT );
      ++ncbnt_tracks;
      // find matching wrapper track.
      std::vector< boost::shared_ptr<const AnaTrack> >::const_iterator iWrapper = std::find_if( beginWrapper , endWrapper , boost::bind(AnaTrack::parametersEqualV(cbnt_track),_1) );
      // if( debug ) {
      //   cout << " match: " << endl;
      //   cout << "cbnt ";
      //   cbnt_track->print();
      //   cout << "wrap ";
      //   if( iWrapper!=endWrapper ) { 
      //     (*iWrapper)->print();
      //   } else {
      //     cout << " NONE!" << endl;
      //   }
      // } // end debug conditional
      // if no matching wrapper track, something is wrong.
      if( iWrapper == endWrapper ) { 
        cerr << " Warning: CBNT offline track " << endl << "\t";
        cbnt_track->print( cerr );
        cerr << " has no matching wrapper track." << endl;
        continue;
      }
      ++nmatched_tracks;
      // save CBNT track, but add correct truth info from wrapper first.
      const boost::shared_ptr<const AnaTrack>& wrapper_track( *iWrapper );
      boost::shared_ptr<AnaTrack> tmp( new AnaTrack(*cbnt_track) );
      tmp->_barcode = wrapper_track->_barcode;
      tmp->_barcode_matching_fraction = wrapper_track->_barcode_matching_fraction;
      tmp->_truth_parent_bitmask = wrapper_track->_truth_parent_bitmask;
      tmp->_si_clusters = wrapper_track->_si_clusters;
      *(iOutput++) = boost::const_pointer_cast<const AnaTrack>(tmp);
    } // end merge each CBNT track
  } else { // do new faster algorithm
    // process subsets of the input tracks, keyed by track pt. 
    SubsetMap cbnt_subset;
    SubsetMap wrapper_subset;
    for( std::vector< boost::shared_ptr<const AnaTrack> >::const_iterator iCBNT=beginCBNT; iCBNT!=endCBNT; ++iCBNT ) {
      const boost::shared_ptr<const AnaTrack>& track( *iCBNT );
      if( track->pt() < AnaConfiguration::minimum_track_pt_cut() ) { continue; }
      if( std::abs(track->d0()) > AnaConfiguration::maximum_track_absd0_cut() ) { continue; }
      if( std::abs(track->eta()) > AnaConfiguration::maximum_track_abseta_cut() ) { continue; }
      cbnt_subset.insert( SubsetMap::value_type(std::floor(track->eta()*1000),track) );
    }
    for( std::vector< boost::shared_ptr<const AnaTrack> >::const_iterator iWrapper=beginWrapper; iWrapper!=endWrapper; ++iWrapper ) {
      const boost::shared_ptr<const AnaTrack>& track( *iWrapper );
      if( track->pt() < AnaConfiguration::minimum_track_pt_cut() ) { continue; }
      if( std::abs(track->d0()) > AnaConfiguration::maximum_track_absd0_cut() ) { continue; }
      if( std::abs(track->eta()) > AnaConfiguration::maximum_track_abseta_cut() ) { continue; }
      wrapper_subset.insert( SubsetMap::value_type(std::floor(track->eta()*1000),track) );
    }
    if( debug ) {
      cout << " wrapper tracks: " << wrapper_subset.size() << " / " << std::distance(beginWrapper,endWrapper) << endl;
      BOOST_FOREACH( const SubsetMap::value_type& wrapper_pair , wrapper_subset ) {
        const boost::shared_ptr<const AnaTrack>& wrapper_track( wrapper_pair.second );
        cout << "wrap ";
        wrapper_track->print();
      }
      cout << " CBNT tracks: " << cbnt_subset.size() << " / " << std::distance(beginCBNT,endCBNT) << endl;
      BOOST_FOREACH( const SubsetMap::value_type& cbnt_pair , cbnt_subset ) {
        const boost::shared_ptr<const AnaTrack>& cbnt_track( cbnt_pair.second );
        cbnt_track->print();
      }
    } // end debug conditional
    BOOST_FOREACH( const SubsetMap::value_type& cbnt_pair , cbnt_subset ) {
      const boost::shared_ptr<const AnaTrack>& cbnt_track( cbnt_pair.second );
      ++ncbnt_tracks;
      std::pair< SubsetMap::const_iterator , SubsetMap::const_iterator > rmatches = wrapper_subset.equal_range( cbnt_pair.first );
      if( debug ) { 
        cout << " trying to match CBNT track: ";
        cbnt_track->print( cout );
        cout << " possible matches (same eta): " << std::distance( rmatches.first , rmatches.second ) << endl;
      }
      if( rmatches.first == wrapper_subset.end() ) {
        cerr << " Warning: CBNT offline track " << endl << "\t";
        cbnt_track->print( cerr );
        cerr << " has no matching wrapper track." << endl;
        continue;
      }
      // look in the range with precisely matching eta.
      bool found_exact_match = false;
      AnaTrack::parametersEqualV matcher( cbnt_track );
      for( SubsetMap::const_iterator iw=rmatches.first, fw=rmatches.second; iw!=fw; ++iw ) {
        const boost::shared_ptr<const AnaTrack>& wrapper_track( (*iw).second );
        bool is_exact_match = matcher(wrapper_track);
        if( !is_exact_match && std::distance(rmatches.first,rmatches.second)==1 ) {
          // only one track with approximately the same eta. check parameters agree to within negligible precision.
          is_exact_match = ( std::abs(detector::delta_phi(wrapper_track->phi0(),cbnt_track->phi0()))<0.000001 &&
                             std::abs( detector::safe_divide( wrapper_track->curvature()-cbnt_track->curvature() , cbnt_track->curvature() ) ) < 0.0001 &&
                             std::abs( detector::safe_divide( wrapper_track->d0()-cbnt_track->d0() , cbnt_track->d0() ) ) < 0.0001 &&
                             std::abs( detector::safe_divide( wrapper_track->z0()-cbnt_track->z0() , cbnt_track->z0() ) ) < 0.0001 );
        }
        if( is_exact_match ) {
          ++nmatched_tracks;
          // save CBNT track, but add correct truth info from wrapper first.
          boost::shared_ptr<AnaTrack> tmp( new AnaTrack(*cbnt_track) );
          tmp->_barcode = wrapper_track->_barcode;
          tmp->_barcode_matching_fraction = wrapper_track->_barcode_matching_fraction;
          tmp->_truth_parent_bitmask = wrapper_track->_truth_parent_bitmask;
          tmp->_si_clusters = wrapper_track->_si_clusters;
          *(iOutput++) = boost::const_pointer_cast<const AnaTrack>(tmp);
          found_exact_match = true;
          break;
        }
      } // end check each match
      if( !found_exact_match) {
        cerr << " Warning: CBNT offline track " << endl << "\t";
        cbnt_track->print( cerr );
        cerr << " has no good wrapper track match." << endl;
        continue;
      }
    } // end merge each CBNT track
  } // end do one of the available matching algorithms
  // verbose debugging for development
  if( debug && ncbnt_tracks > 0 ) {
    cout << "matched " << nmatched_tracks << " / " << ncbnt_tracks << " tracks (" << (100.*static_cast<float>(nmatched_tracks)/ncbnt_tracks) << " %)" << endl;
  }
}

void
AnaEventMgr::save_sample_description() const
{
  // write sample description once per job
  static const bool write_desc = true;
  if( write_desc ) {
    string desc_filename = AnaConfiguration::full_output_path();
    boost::filesystem::path ipath( DataGraphics::Tools::expand_filename_to_valid_unique(desc_filename+".xml") );
    boost::filesystem::path idefpath( DataGraphics::Tools::expand_filename_to_valid_unique(AnaConfiguration::full_sample_definition_path()+".xml") );
    //boost::filesystem::path descpath = ipath.root_path() / ipath.stem();
    // boost::filesystem::path descpath = ipath;
    // ipath.change_extension( "xml" );
    // boost::filesystem::change_extension( ipath , "xml" );
    //
    assert( ! _ntuple_filenames_pending.empty() || ! _ntuple_filenames_done.empty() || ! _ntuple_filenames_skipped.empty() );
    const DatasetDefinition& definition( ! _ntuple_filenames_pending.empty() ? 
                                         _ntuple_filenames_pending.front() : 
                                         (! _ntuple_filenames_done.empty() ? _ntuple_filenames_done.front() : _ntuple_filenames_skipped.front() ) );
    const DatasetDefinition::SampleStrings sample_strings( definition.sample_strings );
    DatasetDefinition::SampleStrings::const_iterator ileg = sample_strings.find( "legend" );
    DatasetDefinition::SampleStrings::const_iterator idesc = sample_strings.find( "description" );
    DatasetDefinition::SampleStrings::const_iterator iname = sample_strings.find( "name" );
#if BOOST_VERSION>=104800
    string name_text = iname!=sample_strings.end() ? iname->second : idefpath.stem().string();
#else
    string name_text = iname!=sample_strings.end() ? iname->second : idefpath.stem();
#endif
    string legend_text = ileg!=sample_strings.end() ? ileg->second : string("");
    string desc_text = idesc!=sample_strings.end() ? idesc->second : string("");
    DatasetDefinition::SampleFlags::const_iterator imc = definition.sample_flags.find( "mc" );
    const bool is_mc = (imc!=definition.sample_flags.end()) && imc->second;
    // construct description: labels, tags, properties of sample
    DgXMLSampleDescription d( name_text , legend_text , desc_text );
    BOOST_FOREACH( const string& tag , definition.sample_tags ) { d.set_tag( tag ); }
    if( is_mc ) {
      {
        DatasetDefinition::SampleFlags::const_iterator inm = definition.sample_flags.find( "absolute_normalization" );
        const bool is_absolute_norm = (inm!=definition.sample_flags.end()) && inm->second;
        if( is_absolute_norm ) { d.set_normalization( DgXMLSampleDescription::ABSOLUTE_NORMALIZATION ); }
        else { d.set_normalization( DgXMLSampleDescription::NO_NORMALIZATION ); }
      }
      DatasetDefinition::SampleNumericFlags::const_iterator il=definition.sample_numeric_flags.find("xsec");
      if( il!=definition.sample_numeric_flags.end() ) {
        d.set_cross_section( il->second ); // pb
      }
    } else {
      d.set_tag( "data" );
      d.set_type( DgXMLSampleDescription::DATA );
      if( boost::algorithm::icontains(ipath.string(),"egamma") || definition.sample_tags.find("egamma")!=definition.sample_tags.end() ) {
        d.set_tag( "egamma" );
        d.set_stream( DgXMLSampleDescription::EGAMMA_STREAM );
      }
      if( boost::algorithm::icontains(ipath.string(),"muons") || definition.sample_tags.find("muons")!=definition.sample_tags.end() ) {
        d.set_tag( "muons" );
        d.set_stream( DgXMLSampleDescription::MUONS_STREAM );
      }
      DatasetDefinition::SampleNumericFlags::const_iterator il=definition.sample_numeric_flags.find("lumi");
      if( il!=definition.sample_numeric_flags.end() ) {
        d.set_luminosity( il->second ); // pb^-1
      }
    }
    DatasetDefinition::SampleNumericFlags::const_iterator iw=definition.sample_numeric_flags.find("weight");
    DatasetDefinition::SampleNumericFlags::const_iterator ikf=definition.sample_numeric_flags.find("kfactor");
    float weight = 1.;
    bool use_weight = false;
    if( iw!=definition.sample_numeric_flags.end() ) { 
      weight *= iw->second; 
      use_weight = true; 
      cout << " reweight: " << iw->second << endl;
    }
    if( ikf!=definition.sample_numeric_flags.end() ) { 
      weight *= ikf->second; 
      use_weight = true; 
      cout << " kfactor: " << ikf->second << endl;
    }
    if( use_weight ) { 
      d.set_weight( weight );
    } // sample re-weight
    d.set_num_events_total( _n_events_processed ); // total number of events processed in this job.
    for( std::map<std::string,double>::const_iterator i=_reweighted_event_counts.begin() , f=_reweighted_event_counts.end(); i!=f; ++i ) {
      d.set_weighted_nevents( i->first , i->second );
    }
    if( AnaConfiguration::split_num_files()> 0 ) {
      // compute number of splits
      d.set_partial_sample( AnaConfiguration::split_num_files() , AnaConfiguration::split_offset_file() );
    }
    cout << "saving dataset description xml to: " << ipath.string() << endl;
    d.save_xml( ipath.string() );
  }
}

// skimming/trimming/conversion to AnaEventNtuple
// ================================================================

const bool
AnaEventMgr::create_ana_event_ntuple( const std::string& directory )
{
  if( _ana_event_ntuple_write.mode() != AnaEventNtuple::CLOSED ) { 
    cout << " AnaEventNtuple already open." << endl;
    return false; 
  }
  _ana_event_ntuple_write.open_sample( directory , AnaEventNtuple::WRITE );
  cout << " opened AnaEventNtuple in directory " << _ana_event_ntuple_write.directory() << endl;
  return true;
}

const bool
AnaEventMgr::add_to_ana_event_ntuple( const bool& add_true_not_false )
{
  if( _ana_event_ntuple_write.mode() != AnaEventNtuple::WRITE ) { 
    cout << " AnaEventNtuple must be opened with AnaEventMgr::create_ana_event_ntuple( const std::string& directory ) first." << endl;
    return false;
  }  
  _ana_event_ntuple_write.add_event( _event , add_true_not_false );
  return true;
}

void
AnaEventMgr::do_begin_event_callbacks() const {
  BOOST_FOREACH( const Callback& cb , _begin_event_callbacks ) {
    cb();
  }
}

void
AnaEventMgr::do_end_event_callbacks() const {
  BOOST_FOREACH( Callback& cb , _end_event_callbacks ) {
    cb();
  }
}

AnaEventMgr* AnaEventMgr::_instance = 0;
boost::shared_ptr<AnaEvent> AnaEventMgr::_event( new AnaEvent );
boost::shared_ptr<const AnaEvent> AnaEventMgr::_event_const = boost::const_pointer_cast<const AnaEvent>(AnaEventMgr::_event);
AnaEventNtuple AnaEventMgr::_ana_event_ntuple_read;
AnaEventNtuple AnaEventMgr::_ana_event_ntuple_write;
AnaProgress<std::ostream> AnaEventMgr::_progress( "AnaEventMgr" , std::cout );
EventCountCache AnaEventMgr::_event_count_cache( "./cached_event_counts" );
IgnoreFileCache AnaEventMgr::_ignore_filenames_cache( "./cached_ignore_filenames" );
const std::string& AnaEventMgr::DEFAULT_SOURCE = AnaEvent::DEFAULT_SOURCE;
unsigned long AnaEventMgr::_n_events_processed = 0ul;
std::map<std::string,double> AnaEventMgr::_reweighted_event_counts;
std::vector< AnaEventMgr::Callback > AnaEventMgr::_begin_event_callbacks;
std::vector< AnaEventMgr::Callback > AnaEventMgr::_end_event_callbacks;

bool AnaEventMgr::_copy_ftk_si_clusters = false;

