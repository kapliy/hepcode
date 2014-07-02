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
#include "TrigFTKAna/Trees/NtD3PDphys.hpp"
#include "TrigFTKAna/Trees/NtD3PDphysPeter.hpp"
#include "TrigFTKAna/AnaConfiguration.hpp"
#include "TrigFTKAna/EventCountCache.hpp"
#include "TrigFTKAna/IgnoreFileCache.hpp"
#include "TrigFTKAna/AnaVertex.hpp"
#include "TrigFTKAna/AnaEventNtuple.hpp"
#include "DataGraphics/DataGraphics.hh"

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
  map< string , vector<string> > d3pdnt_filenames;
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
    case 'N':
      nevents = std::strtoul( tokens.front().c_str() , NULL , 0 );
      if( nevents == 0ul || nevents>1000000ul ) {
        cout << "AnaEventMgr::open_sample: warning: line " << line << " specifies nevents == " << nevents << endl;
      }
      break;
    case 'D':
      if( tokens.size()>=1 ) {
        if( tokens.size()>=2 ) { source_name = tokens.back(); }
        std::string fname( ana_streams::root_dcap_filename(tokens.front()) );
        fname = std::string( tokens.front() ); // Nov 2013: delay filename mangling
        // check for null filename
        if( string(boost::algorithm::trim_copy(fname))!="" ) {
	  if(false) {
	    ;
          } else if( boost::algorithm::istarts_with(code_complete,"DPETER") ) { 
            d3pdnt_filenames[source_name].push_back( NtD3PDphys::filename_to_metaname(fname,NtD3PDphys::PETER) );
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
    case 'E':
      // end section
      // compute number of events if it has not been assigned.
      if( nevents == 0ul && AnaConfiguration::precompute_number_of_events() ) {
	if( !d3pdnt_filenames.empty() ) {
          BOOST_FOREACH( const FilenamePair& filename_pair , d3pdnt_filenames ) {
            BOOST_FOREACH( const string& metaname , filename_pair.second ) {
              NtD3PDphys::Type type;
              string filename;
              if( NtD3PDphys::type_and_filename_from_metaname( metaname , type , filename ) ) {
                switch( type ) { 
                case NtD3PDphys::PETER: nevents += _event_count_cache( filename , boost::bind( &NtD3PDphysPeter::count_events , _1 ) ); break;
                case NtD3PDphys::NONE: break;
                default:
                  assert( !"unhandled case" );
                }
              }
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
        if( section_is_valid ) {
          _ntuple_filenames_pending.push_back( DatasetDefinition() );
          _ntuple_filenames_pending.back().sample_type = sample_type;
          _ntuple_filenames_pending.back().nevents = nevents;
          _ntuple_filenames_pending.back().sample_flags = sample_flags;
          _ntuple_filenames_pending.back().sample_numeric_flags = sample_numeric_flags;
          _ntuple_filenames_pending.back().sample_strings = sample_strings;
          _ntuple_filenames_pending.back().sample_tags = sample_tags;
          _ntuple_filenames_pending.back().d3pdnt_filenames = d3pdnt_filenames;
        } else {
          cout << "ignoring section. " << endl;
        }
      }
      sample_type = "";
      nevents = 0ul;
      d3pdnt_filenames.clear();
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
    for( map< string , vector< boost::shared_ptr<NtD3PDphys> > >::iterator i=_nt_d3pd.begin() ,f=_nt_d3pd.end(); i!=f; ++i ) {
      for( vector< boost::shared_ptr<NtD3PDphys> >::iterator j=i->second.begin(), jf=i->second.end(); j!=jf; ++j ) {
        if( *j ) { 
          _event_count_cache( (*j)->open_filename() , EventCountCache::SupplyComputedEventCount( _file_read_events ) );
          (*j)->end_file(); 
        }
      }
    }
    // update flag
    _file_open = false;
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
    map<string,vector<bool> > ok_d3pd;
    set<string> source_names;
    typedef pair< string , vector<string> > FilenamePair;
    BOOST_FOREACH( const FilenamePair& filename_pair , files.d3pdnt_filenames ) {
      unsigned int n=0u;
      for( vector<string>::const_iterator i=filename_pair.second.begin(), f=filename_pair.second.end(); i!=f; ++i,++n ) {
        while( _nt_d3pd[filename_pair.first].size()<=n ) { 
          _nt_d3pd[filename_pair.first].push_back( boost::shared_ptr<NtD3PDphys>(new NtD3PDphys) ); 
        }
        NtD3PDphys::Type type;
        string filename;
        if( NtD3PDphys::type_and_filename_from_metaname( *i , type , filename ) ) {
	  std::cerr << "INFO: opening | " << filename << " |" << std::endl;
	  if( _ignore_filenames_cache.check(filename) ) { 
	    ok_d3pd[filename_pair.first].push_back( false );
          } else if( type==NtD3PDphys::PETER ) { 
            _nt_d3pd[filename_pair.first][n].reset( new NtD3PDphysPeter );
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

    unsigned int n_failures = 0u;
    BOOST_FOREACH( const string& source_name , source_names ) {
      n_failures += std::count_if( ok_d3pd[source_name].begin() , ok_d3pd[source_name].end() , boost::bind( std::equal_to<bool>() , _1 , false ) );
    }

    if( true ) {
      string label("");
      if( label == "" ) { 
        BOOST_FOREACH( const string& source_name , source_names ) {
          map<string,vector<string> >::const_iterator i;
          if( label=="" && (i=files.d3pdnt_filenames.find(source_name))!=files.d3pdnt_filenames.end() ) {
            NtD3PDphys::Type type;
            string filename;
            if( NtD3PDphys::type_and_filename_from_metaname( i->second.front() , type , filename ) ) {
              if( !(i->second.empty()) ) { label = filename; }
            }
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

  // ntuples have been successfully read. update AnaEvent.
  // ----------------------------------------------------------------

  if(true) {
    // construct a new AnaEvent from the constituent ntuples.

    AnaConfiguration::OfflineTrackReadMode offline_mode = AnaConfiguration::OFFLINE_FROM_D3PD;

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
    _event->inconsistentCBNT = false;
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

    // DONE with event sync. now funnel the data into the AnaEvent objects
    // ================================================================

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
      } // end for each source
    } // end for each d3pd for this event

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

  if( false ) {
    // histogram performance.
    DataGraphics::dg::down( "performance" , "analysis performance statistics" ); BOOST_SCOPE_EXIT(void) { DataGraphics::dg::up(); } BOOST_SCOPE_EXIT_END;
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
    BOOST_FOREACH( const FilenamePair& fpair , ntf.d3pdnt_filenames ) {
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


void
AnaEventMgr::save_sample_description() const
{
  return;
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

