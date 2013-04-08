#include <string>
#include <vector>
#include <map>
#include <iterator>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/constants.hpp>
#include <boost/program_options.hpp>
#include <boost/foreach.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/progress.hpp>
#include <boost/optional.hpp>
#include "TrigFTKAna/ana_streams.hpp"
#include "TrigFTKAna/AnaTaggerMgr.hpp"
#include "TrigFTKAna/SignedD0JetTagger.hpp"
#include "TrigFTKAna/SignedD0ParameterizedJetTagger.hpp"
#include "TrigFTKAna/Detector.hpp"
#include <TH1F.h>
#include <TFile.h>

using namespace std;
using namespace boost;
using namespace boost::program_options;
using namespace DataGraphics;

// take a list of tagger training files. treating each as the training
// for disjoint subsets of events, combine all of the files into a
// higher-statistics training file.

void do_hack_cbnt();
void evaluate_taggers( const std::string filename , const vector<double>& xes );

#ifdef __cplusplus
extern "C"
#endif
int
F77_MAIN( int argc , char* argv[] )
{
  argc = std::distance( argv , std::find( argv , argv+256 , (char*)0 ) ); // HACK: fortran is passing a nonsense argc.
  // parse filter options
  vector<string> input_filenames;
  string output_filename = "merged.tagger";
  string root_filename = "tagger_%DATE";
  bool create_parameterized_taggers = true;
  try {
    options_description cmdline_options( "merge_trained_tagger options" );
    cmdline_options.add_options()
      ( "help" , "display help message" )
      ( "input,i" , value< vector<string> >() , "input tagger training path/to/filename(s) to be combined" )
      ( "list,l" , value< vector<string> >() , "files containing lists of input tagger training path/to/filename(s) to be combined" )
      ( "output,o" , value<string>() , "combined tagger training path/to/filename" )
      ( "root,r" , value<string>() , "optional root display of merged tagger output, path/to/filename" )
      ( "noparam" , "disable parameterized taggers" )
      ( "cbnt" , "hack: match CBNT ntuples to wrapper ntuples" )
      ( "x" , value< vector<double> >() , "hack: evaluate all tagger track likelihoods at this value(s)" )
      ;
    positional_options_description cmdline_positional_options;
    cmdline_positional_options.add( "input" , -1 ); // an unnamed argument anywhere on the command line will be treated as a input filename
    variables_map cmdline;
    parsed_options tmp( command_line_parser(argc,argv).options(cmdline_options).allow_unregistered().positional(cmdline_positional_options).run() ); 
    store( tmp , cmdline ); 
    notify( cmdline ); 
    if( cmdline.count("cbnt") ) {
      do_hack_cbnt();
      return 0;
    }
    if( cmdline.count("x") ) {
      evaluate_taggers( cmdline["input"].as< vector<string> >().front() , cmdline["x"].as< vector<double> >() );
      return 0;
    }
    if( cmdline.count( "help" ) || (cmdline.count("input")==0 && cmdline.count("list")==0) || cmdline.count("output")==0 ) { cout << cmdline_options << endl; return -1; }
    if( cmdline.count("input") ) {
      input_filenames = cmdline["input"].as< vector<string> >();
    }
    output_filename = cmdline["output"].as<string>();
    if( cmdline.count("root") ) { 
      root_filename = cmdline["root"].as<string>();
    }
    if( cmdline.count("noparam") ) { create_parameterized_taggers = false; }
    if( cmdline.count("list") ) {
      vector<string> inlists = cmdline["list"].as< vector<string> >();
      for( vector<string>::const_iterator i=inlists.begin(), f=inlists.end(); i!=f; ++i ) {
        shared_ptr<ana_streams::istream> istr = ana_streams::open_for_read( *i );
        if( !istr ) { continue; }
        while( !(istr->eof()) ) { 
          string line;
          getline( *istr , line );
          if( line != "" ) { input_filenames.push_back( line ); }
        }
        istr->reset();
      }
    }
  } catch(...) { cerr << "unhandled exception while parsing command line."; return -1; }

  // loop over input files and extract the events
  cout << " processing " << input_filenames.size() << " files. " << endl;
  cout << endl;
  const unsigned long nfilenames = input_filenames.size();
  progress_display progress( nfilenames );

  AnaTaggerMgr mgr_a, mgr_b;
  
  BOOST_FOREACH( const std::string& in_filename , input_filenames ) {
    mgr_b.load( in_filename );
    mgr_a += mgr_b;
    ++progress;
  }

  // create parameterized taggers
    // configure parameterized likelihood taggers
  if( create_parameterized_taggers ) { 
    vector<string> binned_tagger_names;
    for( AnaTaggerMgr::const_iterator i=mgr_a.begin(), f=mgr_a.end(); i!=f; ++i ) {
      binned_tagger_names.push_back( i->first );
    }
    //transform( tagger_mgr.begin() , tagger_mgr.end() , back_inserter(binned_tagger_names) , boost::bind( &AnaTaggerMgr::get_name , tagger_mgr , _1 ) );
    BOOST_FOREACH( const string& binned_name , binned_tagger_names ) {
      if( boost::algorithm::icontains(binned_name,"IDSCAN") || boost::algorithm::icontains(binned_name,"TRUTH") ) { continue; }
      cout << " PARAMETERIZING TAGGER " << binned_name << endl;
      const bool ok = mgr_a.add_parameterization<SignedD0JetTagger,SignedD0ParameterizedJetTagger>( binned_name );
    }
  }

  cout << " saving merged tagger to " << output_filename << endl;
  mgr_a.save( output_filename );

  if( root_filename != "" ) {
    TFile* f = new TFile( root_filename.c_str() , "RECREATE" );
    TH1F* hb = mgr_a.likelihood( "FTK_SIGNEDD0_TAGGER" , true );
    hb->SetName("hb");
    TH1F* hnotb = mgr_a.likelihood( "FTK_SIGNEDD0_TAGGER" , false );
    hnotb->SetName("hnotb");
    hb->SetDirectory( f );
    hnotb->SetDirectory( f );
    f->Write();
    f->Close();
  }

  // done. 
  cout << endl << endl << "done." << endl;

  return 0;
}

#include "TrigFTKAna/WHTreeV13.h" // sufficient for v14 also: only run and event number used
#include "TChain.h"

void
do_hack_cbnt()
{
  // open each CBNT ntuple and record run and event number.
  typedef map<unsigned int,string> Map;
  {
    Map b_map;
    ifstream infb( "cbnt_ntuples_b.list" );
    string line;
    progress_display progress( 1000 );
    while( infb ) {
      getline( infb , line );
      if( line.find(".root")==string::npos ) { continue; }
      boost::shared_ptr<TChain> chain( new TChain("CollectionTree") );
      chain->AddFile( line.c_str() );
      boost::shared_ptr<WHTreeV13> tree( new WHTreeV13(chain.get()) );
      tree->Init( chain.get() );
      if( chain->GetEntries()<1 ) { continue; }
      Long64_t ientry = tree->LoadTree( 0 );
      if( ientry < 0 ) { continue; }
      tree->GetEntry( 0 ); // returns nbytes
      b_map.insert( Map::value_type(tree->EventNumber , line) );
      ++progress;
    }
    ofstream outb( "cbnt_event_pairs_b.txt" );
    BOOST_FOREACH( Map::value_type& pr , b_map ) {
      outb << pr.first << " " << pr.second << endl;
    }
  }

  {
    Map u_map;
    ifstream infu( "cbnt_ntuples_u.list" );
    string line;
    progress_display progress( 1000 );
    while( infu ) {
      getline( infu , line );
      if( line.find(".root")==string::npos ) { continue; }
      boost::shared_ptr<TChain> chain( new TChain("CollectionTree") );
      chain->AddFile( line.c_str() );
      boost::shared_ptr<WHTreeV13> tree( new WHTreeV13(chain.get()) );
      tree->Init( chain.get() );
      if( chain->GetEntries()<1 ) { continue; }
      Long64_t ientry = tree->LoadTree( 0 );
      if( ientry < 0 ) { continue; }
      tree->GetEntry( 0 ); // returns nbytes
      u_map.insert( Map::value_type(tree->EventNumber , line) );
      ++progress;
    }
    ofstream outu( "cbnt_event_pairs_u.txt" );
    BOOST_FOREACH( Map::value_type& pr , u_map ) {
      outu << pr.first << " " << pr.second << endl;
    }
  }

}

void
evaluate_taggers( const std::string filename , const vector<double>& xes )
{
  using boost::format;
  AnaTaggerMgr mgr;
  mgr.load( filename );
  const unsigned int n = std::distance( mgr.begin() , mgr.end() );
  vector<double> products;
  fill_n( back_inserter(products) , n , 1. );
  for( vector<double>::const_iterator ix=xes.begin(), fx=xes.end(); ix!=fx; ++ix ) {
    const double& x(*ix);
    cout << format("%|10t| x: %|12g|") % x << endl;
    for( AnaTaggerMgr::const_iterator i=mgr.begin(), f=mgr.end(); i!=f; ++i ) {
      const JetTagger* jt( mgr.get_tagger(i).get() );
      const SignedD0JetTagger* sdjt = dynamic_cast<const SignedD0JetTagger*>(jt);
      const SignedD0ParameterizedJetTagger* sdpjt = dynamic_cast<const SignedD0ParameterizedJetTagger*>(jt);
      if( sdjt ) { 
        const double blik = sdjt->b_d0_likelihood().lik( x );
        const double notblik = sdjt->notb_d0_likelihood().lik( x );
        const double y = detector::safe_divide(blik,notblik);
        cout << format("%|10t| x: %|12g| %|12g| %|12g| %|50s|") % blik % notblik % y % mgr.get_name(i) << endl;
        products[ std::distance(mgr.begin(),i) ] *= y;
      }
      if( sdpjt ) { 
        const double blik = sdpjt->b_d0_likelihood().lik( x );
        const double notblik = sdpjt->notb_d0_likelihood().lik( x );
        const double y = detector::safe_divide(blik,notblik);
        cout << format("%|10t| x: %|12g| %|12g| %|12g| %|50s|") % blik % notblik % y % mgr.get_name(i) << endl;
        products[ std::distance(mgr.begin(),i) ] *= y;
      }
    }
  }
  cout << " total result: " << endl;
  for( AnaTaggerMgr::const_iterator i=mgr.begin(), f=mgr.end(); i!=f; ++i ) {
    const JetTagger* jt( mgr.get_tagger(i).get() );
    cout << format("%|15t| %|50s| %|12g|") %  mgr.get_name(i) % products[ std::distance(mgr.begin(),i) ] << endl;
  }

}

#ifdef F77_DUMMY_MAIN
#ifdef __cplusplus
extern "C"
#endif
int F77_DUMMY_MAIN() { abort(); return 1; }
#endif
