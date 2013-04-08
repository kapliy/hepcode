#include <vector>
#include <string>
#include <map>
#include <list>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/program_options.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/foreach.hpp>
#include <boost/progress.hpp>
#include "TrigFTKAna/ana_streams.hpp"

using namespace std;
using namespace boost;
using namespace boost::program_options;

// filter wrapper input files for specific events using a list of run,event pairs.

int
main( int argc , char* argv[] )
{
  // parse filter options
  vector<string> pair_filenames;
  vector<string> input_filenames;
  string output_filename;
  try {
    options_description cmdline_options( "cull_wrapper_events options" );
    cmdline_options.add_options()
      ( "help" , "display help message" )
      ( "pairs,f" , value< vector<string> >() , "read list(s) of desired \"run event\" pairs (one pair per line, separated by a space) from this file" )
      ( "input,i" , value< vector<string> >() , "input wrapper path/to/filename(s) to be filtered" )
      ( "output,o" , value<string>() , "filtered wrapper output path/to/filename" )
      ;
    variables_map cmdline;
    parsed_options tmp( command_line_parser(argc,argv).options(cmdline_options).run() ); 
    store( tmp , cmdline ); 
    notify( cmdline ); 
    if( cmdline.count( "help" ) || cmdline.count("pairs")==0 || cmdline.count("input")==0 || cmdline.count("output")==0 ) { cout << cmdline_options << endl; return -1; }
    pair_filenames = cmdline["pairs"].as< vector<string> >();
    input_filenames = cmdline["input"].as< vector<string> >();
    output_filename = cmdline["output"].as<string>();
  } catch(...) { cerr << "unhandled exception while parsing command line."; return -1; }

  // open output file
  shared_ptr<ana_streams::ostream> outf( ana_streams::open_for_write(output_filename) );
  if( !outf || !outf->good() ) { cerr << "couldn't open " << output_filename << " for output." << endl; return -1; }

  // get list of run and event pairs
  typedef map<unsigned long,bool> EventChecklist; // bool=true if event was extracted.
  typedef map<unsigned long,EventChecklist> RunEventChecklist;
  RunEventChecklist run_event_checklist; 
  unsigned long n_to_do = 0ul; 
  cout << "reading list(s) of desired run,event pairs..." << endl;
  BOOST_FOREACH( const string& filename , pair_filenames ) {
    shared_ptr<ana_streams::istream> inf( ana_streams::open_for_read(filename) );
    if( !inf ) { cerr << "couldn't open " << filename << " for input. ignoring." << endl; continue; }
    string line;
    vector<string> tokens; 
    tokens.reserve( 2 );
    while( *inf ) {
      getline( *inf , line );
      trim( line );
      tokens.clear();
      split( tokens , line , is_any_of(" ") );
      if( tokens.empty() ) { continue; }
      if( tokens.size()!=2 ) { cerr << "invalid format. ignoring line: \"" << line << "\" in file " << filename << endl; continue; }
      (run_event_checklist[ strtoul(tokens.front().c_str(),0,0) ])[strtoul(tokens.back().c_str(),0,0)] = false;
      ++n_to_do; // upper bound on the number of pairs to process (multiply counts duplicates)
    } // end for each line in the file
  } // end for each run,event pair specification file

  // loop over input files and extract the events
  cout << "skimming for requested run,event pairs..." << endl;
  progress_display skim_progress( n_to_do );
  BOOST_FOREACH( const string& filename , input_filenames ) {
    shared_ptr<ana_streams::istream> inf( ana_streams::open_for_read(filename) );
    if( !inf ) { cerr << "couldn't open " << filename << " for input. ignoring." << endl; continue; }
    bool in_good_event = false;
    unsigned long run = 0;
    unsigned long event = 0;
    string line;
    while( *inf ) {
      getline( *inf , line );
      trim( line );
      if( line.empty() ) { continue; }
      char code( line.c_str()[0] );
      // keep processing a good event until it ends
      if( in_good_event && code!='L' ) {
        (*outf) << line << endl;
        continue;
      }
      if( in_good_event && code=='L' ) { // end the event
        (*outf) << line << endl;
        // check off the event
        (run_event_checklist[run])[event] = true;
        in_good_event = false;
        run = 0;
        event = 0;
        ++skim_progress;
        continue;
      }
      list<string> tokens;
      split( tokens , line , is_any_of(" \t") );
      tokens.pop_front();
      if( code=='R' ) {
        run = strtoul( tokens.front().c_str() , 0 , 0 );
        continue;
      }
      if( code=='F' ) {
        event = strtoul( tokens.front().c_str() , 0 , 0 );
        // is this a good event? if so, start it.
        RunEventChecklist::iterator i=run_event_checklist.find( run );
        if( i==run_event_checklist.end() ) { run=0; continue; } // run is not in the list
        EventChecklist::iterator j=i->second.find( event );
        if( j==i->second.end() ) { run=0; event=0; continue; } // event is not in the list
        // run and event are in the list. 
        in_good_event = true;
        (*outf) << "R\t" << run << endl;
        (*outf) << "F\t" << event << endl;
      }
    }
  } // end for each input file

  // done. 
  cout << endl << endl << "done." << endl;

  // warn if any requested events were not found and filtered.
  bool any_unfilitered = false;
  for( RunEventChecklist::const_iterator i=run_event_checklist.begin(), f=run_event_checklist.end(); i!=f; ++i ) {
    for( EventChecklist::const_iterator j=i->second.begin(), jf=i->second.end(); j!=jf; ++j ) {
      if( !(j->second) ) {
        any_unfilitered = true;
        cout << format("warning: did not find run %1%%|25t| event %2%%|40t|") % i->first % j->first << endl;
      }
    }
  }

  return 0;
}
