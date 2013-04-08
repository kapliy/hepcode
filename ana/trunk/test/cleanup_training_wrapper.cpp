#include <string>
#include <vector>
#include <map>
#include <iterator>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cmath>
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/constants.hpp>
#include <boost/program_options.hpp>
#include <boost/foreach.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/progress.hpp>
#include <boost/optional.hpp>
#include <TLorentzVector.h>
#include <TRandom3.h>
#include "TrigFTKAna/Detector.hpp"
#include "TrigFTKAna/ana_streams.hpp"

using namespace std;
using namespace boost;
using namespace boost::program_options;

// take events in the list of wrapper input files and do one or more cleanup operations:
//  1) remove all silicon channels which do not correspond to a particular particle (ID or barcode)
//  2) remove all silicon channels corresponding to disk sensors
//  3) remove all events for which the truth track with 'barcode' has |eta|>maxeta
//  4) remove all truth and reco tracks without 'barcode'
//  5) emulate realistic (aka not perfect) barrel silicon detector with one or more of the following
//     compromised characteristics:
//       randomly remove a fraction of channels in a layer (same channels for all events)
//       randomly disable n modules in a layer (same modules for all events)
//       
// then write the output to a new file.

class 
RandomChannelDeterminer
{
protected:
  const double _random_fraction;
public:
  RandomChannelDeterminer( const double& random_frac ) : _random_fraction(random_frac) {}

  const bool operator()( const int& detector , const int& barrel_ec , const int& layer_disk ,
                         const int& phi_module , const int& eta_module ,
                         const int& phi_index , const int& eta_or_side_index );
};

class 
RandomModuleDeterminer
{
protected:
  const double _random_fraction;
public:
  RandomModuleDeterminer( const double& random_frac ) : _random_fraction(random_frac) {}

  const bool operator()( const int& detector , const int& barrel_ec , const int& layer_disk ,
                         const int& phi_module , const int& eta_module ,
                         const int& side );
};

int
main( int argc , char* argv[] )
{
  // parse filter options
  vector<string> pair_filenames;
  vector<string> input_filenames;
  string output_filename;
  bool nodisks = false;
  optional<unsigned int> keep_abspdg;
  optional<long> keep_barcode;
  optional<long> eta_barcode;
  optional<float> eta_maxeta;
  optional<long> track_barcode;
  optional<int> realistic_layer;
  optional<double> realistic_dead_channel_frac;
  // optional<double> realistic_hot_channel_frac;
  optional<double> realistic_dead_module_frac;
  try {
    options_description cmdline_options( "cleanup_training_wrapper options" );
    cmdline_options.add_options()
      ( "help" , "display help message" )
      ( "input,i" , value< vector<string> >() , "input wrapper path/to/filename(s) to be filtered" )
      ( "output,o" , value<string>() , "filtered wrapper output path/to/filename" )
      ( "nodisks,d" , "remove silicon channels on disk modules (default is to keep all)" )
      ( "pdg,p" , value<unsigned int>() , "keep silicon channels associated to particles with |pdgcode| = arg (default is to keep all)" )
      ( "bar,b" , value<long>() , "keep silicon channels associated to particles with barcode = arg (default is to keep all)" )
      ( "maxeta" , value<float>() , "reject all events with no truth track satisfying barcode='etabar' (see below) and |eta|>maxeta)" )
      ( "etabar" , value<long>() , "particle barcode for eta filter" )
      ( "trackbar" , value<long>() , "track barcode" )
      ( "realistic-layer" , value<int>() , "barrel layer for 'realistic' studies; -1 means all layers" )
      ( "realistic-dead-channel-frac" , value<double>() , "fraction of dead channels on a 'realistic' layer" )
      // ( "realistic-hot-channel-frac" , value<double>() , "fraction of hot channels on a 'realistic' layer" )
      ( "realistic-dead-module-frac" , value<double>() , "fraction of dead modules on a 'realistic' layer" )
      ;
    variables_map cmdline;
    parsed_options tmp( command_line_parser(argc,argv).options(cmdline_options).run() ); 
    store( tmp , cmdline ); 
    notify( cmdline ); 
    if( cmdline.count( "help" ) || cmdline.count("input")==0 || cmdline.count("output")==0 ) { cout << cmdline_options << endl; return -1; }
    input_filenames = cmdline["input"].as< vector<string> >();
    output_filename = cmdline["output"].as<string>();
    nodisks = cmdline.count("nodisks");
    if( cmdline.count("pdg") ) { keep_abspdg.reset( cmdline["pdg"].as<unsigned int>() ); }
    if( cmdline.count("bar") ) { keep_barcode.reset( cmdline["bar"].as<long>() ); }
    if( cmdline.count("maxeta")>0 ^ cmdline.count("etabar")>0 ) { cout << cmdline_options << endl; return -1; }
    if( cmdline.count("maxeta") ) { eta_maxeta.reset( cmdline["maxeta"].as<float>() ); }
    if( cmdline.count("etabar") ) { eta_barcode.reset( cmdline["etabar"].as<long>() ); }
    if( cmdline.count("trackbar") ) { track_barcode.reset( cmdline["trackbar"].as<long>() ); }
    if( cmdline.count("realistic-layer") ) { realistic_layer.reset( cmdline["realistic-layer"].as<int>() ); }
    if( cmdline.count("realistic-dead-channel-frac") ) { realistic_dead_channel_frac.reset( cmdline["realistic-dead-channel-frac"].as<double>() ); }
    // if( cmdline.count("realistic-hot-channel-frac") ) { realistic_hot_channel_frac.reset( cmdline["realistic-hot-channel-frac"].as<double>() ); }
    if( cmdline.count("realistic-dead-module-frac") ) { realistic_dead_module_frac.reset( cmdline["realistic-dead-module-frac"].as<double>() ); }
  } catch(...) { cerr << "unhandled exception while parsing command line."; return -1; }
  assert( !((bool)eta_maxeta ^ (bool)eta_barcode) ); // both must be set or neither must be set.

  // open output file
  shared_ptr<ana_streams::ostream> outf( ana_streams::open_for_write(output_filename) );
  if( !outf || !outf->good() ) { cerr << "couldn't open " << output_filename << " for output." << endl; return -1; }

  // loop over input files and extract the events
  cout << " processing " << input_filenames.size() << " files with the following settings: " << endl;
  cout << format( "%|10t| exclude disk channels: %|25t| %s" ) % (nodisks ? "True" : "False") << endl;
  cout << format( "%|10t| keep |pdg code|: %|25t| %s %d" ) % (keep_abspdg ? "True" : "False") % (keep_abspdg ? *keep_abspdg : 0) << endl;
  cout << format( "%|10t| keep barcode: %|25t| %s %d" ) % (keep_barcode ? "True" : "False") % (keep_barcode ? *keep_barcode : 0) << endl;
  cout << format( "%|10t| keep track barcode: %|25t| %s %d" ) % (track_barcode ? "True" : "False") % (track_barcode ? *track_barcode : -1) << endl;
  cout << format( "%|10t| reject barcode: %|25t| %s barcode %d eta>%|5.2g|" ) % (eta_barcode ? "True" : "False") 
    % (eta_barcode ? *eta_barcode : 0) % (eta_maxeta ? *eta_maxeta : 999)
       << endl;
  cout << ( format( "%|10t| realistic barrel detector: layers: %|10d| fraction of dead channels: %|10f| dead modules: %|10f|")
            % (realistic_layer ? (*realistic_layer) : 999)
            % (realistic_dead_channel_frac ? (*realistic_dead_channel_frac) : 0.)
            % (realistic_dead_module_frac ? (*realistic_dead_module_frac) : 0.)
            ) 
       << endl;
  // cout << ( format( "%|10t| realistic barrel detector: layers: %|10d| fraction of hot channels: %|10f| dead channels: %|10f| dead modules: %|10f|")
  //           % (realistic_layer ? (*realistic_layer) : 999)
  //           % (realistic_hot_channel_frac ? (*realistic_hot_channel_frac) : 0.)
  //           % (realistic_dead_channel_frac ? (*realistic_dead_channel_frac) : 0.)
  //           % (realistic_dead_module_frac ? (*realistic_dead_module_frac) : 0.)
  //           ) 
  //      << endl;
  cout << endl;
  // if realistic detector layers desired, precompute channels and modules to drop.
  // hot/dead channels are assigned in a determinstic but randomly distributed way that is not correlated across modules.
  // RandomChannelDeterminer op_hot_channels( realistic_hot_channel_frac ? (*realistic_hot_channel_frac) : 0 );
  RandomChannelDeterminer op_dead_channels( realistic_dead_channel_frac ? (*realistic_dead_channel_frac) : 0 );
  RandomModuleDeterminer op_dead_modules( realistic_dead_module_frac ? (*realistic_dead_module_frac) : 0 );
  //
  const unsigned long nfilenames = input_filenames.size();
  progress_display progress( nfilenames );
  vector<string> prehits;
  vector<string> posthits;
  vector<string> hits;
  map<long,unsigned int> barcode_to_pdgcode;
  map<long,float> barcode_eta;
  BOOST_FOREACH( const string& filename , input_filenames ) {
    if( nfilenames!=1 ) { ++progress; }
    shared_ptr<ana_streams::istream> inf( ana_streams::open_for_read(filename) );
    if( !inf ) { cerr << "couldn't open " << filename << " for input. ignoring." << endl; continue; }
    string line;
    while( *inf ) {
      getline( *inf , line );
      trim( line );
      if( line.empty() ) { continue; }
      char code( line.c_str()[0] );
      // keep processing an event until it ends
      if( code=='R' || code=='F' ) { 
        prehits.push_back( line );
        continue; 
      } else if( code=='E' ) { 
        // extract barcode
        long evtindex, barcode;
        list<string> tokens;
        split( tokens , line , is_any_of(" \t") , token_compress_on );
        tokens.pop_front();
        assert( tokens.size() >= 6 );
        {
          list<string>::iterator i=tokens.begin();
          advance(i,5);
          stringstream ulstr;
          ulstr << (*i);
          ulstr >> evtindex;
          advance(i,1);
          stringstream ulstr2;
          ulstr2 << (*i);
          ulstr2 >> barcode;
        }
        if( !track_barcode || barcode==(*track_barcode) ) {
          posthits.push_back( line );
        }
        continue;
      } else if( code=='T' ) {
        // extract pdg and barcode
        long evtindex, barcode;
        list<string> tokens;
        split( tokens , line , is_any_of(" \t") , token_compress_on );
        tokens.pop_front();
        // 0- 7, 10
        assert( tokens.size() >= 12 );
        {
          list<string>::iterator i=tokens.begin();
          advance(i,7);
          unsigned int pdgcode = atoi( (*i).c_str() );
          advance(i,3);
          stringstream ulstr; ulstr << (*i);
          ulstr >> evtindex;
          advance( i , 1 );
          stringstream ulstr2;
          ulstr2 << (*i);
          ulstr2 >> barcode;
          assert( evtindex == 0 );
          barcode_to_pdgcode[ barcode ] = pdgcode;
        }
        {
          // compute barcode eta
          list<string>::iterator i=tokens.begin(), f=tokens.begin();
          vector<double> pi;
          advance(i,4);
          advance(f,7);
          for( ; i!=f; ++i ) {
            double pxi;
            stringstream dbstr;
            dbstr << *i;
            dbstr >> pxi;
            pi.push_back( pxi ); 
          }
          assert( pi.size()==3 );
          TLorentzVector p;
          p.SetPxPyPzE( pi[0] , pi[1] , pi[2] , detector::quadrature( pi[0] , pi[1] , pi[2] ) );
          i = tokens.begin();
          advance( i , 10 );
          long evtindex, barcode;
          stringstream ulstr;
          ulstr << (*i);
          ulstr >> evtindex;
          stringstream ulstr2;
          advance( i , 1 );
          ulstr2 << (*i);
          ulstr2 >> barcode;
          assert( evtindex == 0 );
          barcode_eta[ barcode ] = p.Eta();
        }
        if( !track_barcode || barcode==(*track_barcode) ) {
          posthits.push_back( line );
        }
        continue;
      } else if( code=='S' ) {
        // check whether or not to apply "realistic detector" processing
        if( realistic_layer ) { 
          vector<string> tokens;
          split( tokens , line , is_any_of(" \t") , token_compress_on );
          // pixel or strip?
          assert( tokens.size()>11 );
          const bool is_pixel_not_sct = atoi((tokens[4]).c_str())==1;
          bool dead_channel = false;
          bool dead_module = false;
          if( is_pixel_not_sct ) {
            stringstream sbe,sld,spm,sem,spi,sei;
            int be,ld,pm,em,pi,ei;
            sbe << tokens[5]; sbe >> be;
            sld << tokens[6]; sld >> ld;
            spm << tokens[7]; spm >> pm;
            sem << tokens[8]; sem >> em;
            spi << tokens[9]; spi >> pi;
            sei << tokens[10]; sei >> ei;
            // pixel is dead?
            const bool is_realistic_layer = be==0 && ( realistic_layer ? 
                                                       (*realistic_layer)==ld :
                                                       (*realistic_layer)==-1 );
            dead_channel = is_realistic_layer && op_dead_channels((int)is_pixel_not_sct,be,ld,pm,em,pi,ei);
            dead_module = is_realistic_layer && op_dead_modules((int)is_pixel_not_sct,be,ld,pm,em,0);
            // const bool hot_pixel = is_realistic_layer && op_hot_channels(be,ld,pm,em,pi,ei);
          } else {
              stringstream sbe,sld,spm,sem,spi,sei;
              int be,ld,pm,em,pi,ei;
              sbe << tokens[5]; sbe >> be;
              sld << tokens[6]; sld >> ld;
              spm << tokens[7]; spm >> pm;
              sem << tokens[8]; sem >> em;
              sei << tokens[9]; sei >> ei;
              spi << tokens[10]; spi >> pi;
              // pixel is dead?
              const bool is_realistic_layer = be==0 && ( realistic_layer ? 
                                                         (*realistic_layer)==ld :
                                                         (*realistic_layer)==-1 );
              dead_channel = is_realistic_layer && op_dead_channels((int)is_pixel_not_sct,be,ld,pm,em,pi,ei);
              dead_module = is_realistic_layer && op_dead_modules((int)is_pixel_not_sct,be,ld,pm,em,ei);
          }
          if( !dead_channel && !dead_module ) {
            hits.push_back( line );
          }
        } else {
          // perfect detector / no processing
          hits.push_back( line );
        }
      } else if( code=='L' ) {
        // process the event.
        bool reject_entire_event = false;
        if( eta_maxeta && eta_barcode ) {
          map<long,float>::const_iterator ib = barcode_eta.find( *eta_barcode );
          reject_entire_event = true;
          if( ib!=barcode_eta.end() ) {
            reject_entire_event = ( std::abs(static_cast<float>(ib->second))>(*eta_maxeta) );
          }
        }
        if( !reject_entire_event ) {
          copy( prehits.begin() , prehits.end() , ostream_iterator<string>( *outf , "\n" ) );
          BOOST_FOREACH( const string hitline , hits ) {
            list<string> tokens;
            split( tokens , hitline , is_any_of(" \t") , token_compress_on );
            tokens.pop_front();
            // reject disk hits?
            if( nodisks ) {
              assert( tokens.size() >= 5 );
              list<string>::iterator i=tokens.begin();
              advance(i,4);
              unsigned int bec = atoi( (*i).c_str() );
              if( bec!=0 ) { continue; } // skip disk hits
            }
            // reject hits from incorrect particles?
            if( keep_abspdg || keep_barcode ) {
              assert( tokens.size() >= 13 );
              list<string>::iterator i=tokens.begin();
              advance( i , 11 );
              long evtindex, barcode;
              stringstream ulstr;
              ulstr << (*i);
              ulstr >> evtindex;
              advance( i , 1 );
              stringstream ulstr2;
              ulstr2 << (*i);
              ulstr2 >> barcode;
              if( barcode<0 ) { continue; } // skip particles with no barcode--these won't match anyway.
              assert( evtindex == 0 );
              const bool ok_barcode = keep_barcode ? (*keep_barcode)==barcode : true;
              map<long,unsigned int>::const_iterator ib = barcode_to_pdgcode.find( barcode );
              const bool known_pdgcode = ib!=barcode_to_pdgcode.end();
              const int this_pdgcode = known_pdgcode ? ib->second : 0;
              const bool ok_pdg = keep_abspdg ? (known_pdgcode ? std::abs(this_pdgcode)==(*keep_abspdg) : false) : true;
              if( !ok_barcode || !ok_pdg ) { continue; }
            }
            // copy this hit
            *outf << hitline << endl;
          } // end for each hit
          copy( posthits.begin() , posthits.end() , ostream_iterator<string>( *outf , "\n" ) );
          // don't forget to copy the end of record code.
          *outf << line << endl;
        }
        // clear
        prehits.clear();
        posthits.clear();
        hits.clear();
        barcode_to_pdgcode.clear();
      } else {
        cerr << " unhandled wrapper field code: " << line << endl;
        assert( false );
      }
    } // end for each line in the input file
  } // end for each input file

  // done. 
  cout << endl << endl << "done." << endl;

  return 0;
}


const bool
RandomChannelDeterminer::operator()( const int& detector , const int& barrel_ec , const int& layer_disk ,
                                     const int& phi_module , const int& eta_module ,
                                     const int& phi_index , const int& eta_or_side_index )
{
  static TRandom3 rgen;
  unsigned int seed;
  // want a different random subset of (phi_index, eta_or_side_index)
  // combinations for each combination of (barrel_ec, layer_disk,
  // phi_module, eta_module), but the subset must be the same on each
  // subsequent call. the following constitutes criminal abuse of a
  // random number generator. all we're really interested in is
  // suppressing correlations in selected strip/pixel position across
  // modules, in getting the correct fraction of selected channels, and
  // in some approximation of a uniform distribution.
  seed = static_cast<unsigned int>( 1234*detector + 
                                    100000u*barrel_ec +
                                    1000u*layer_disk + 
                                    phi_module + 
                                    5432u*eta_module +
                                    1000u*phi_index + 
                                    10000u*eta_or_side_index );
  rgen.SetSeed(seed);
  const bool is_dead = rgen.Rndm() < _random_fraction;
  if( true && is_dead ) { 
    cout << ( boost::format("DEAD CHANNEL: d%|1d| b%|2d| l%|1d| pm%|3d| em%|3d| pi%|3d| ei%|3d|")
              % detector % barrel_ec % layer_disk % phi_module % eta_module % phi_index % eta_or_side_index )
         << endl;
  }
  return( is_dead );
}

const bool
RandomModuleDeterminer::operator()( const int& detector , const int& barrel_ec , const int& layer_disk ,
                                    const int& phi_module , const int& eta_module ,
                                    const int& side )

{
  static TRandom3 rgen;
  unsigned int seed;
  // want a different random subset of (phi_module, eta_module)
  // combinations for each combination of (barrel_ec, layer_disk,
  // side), but the subset must be the same on each
  // subsequent call. the following constitutes criminal abuse of a
  // random number generator. all we're really interested in is
  // suppressing correlations in selected module position across
  // layers, in getting the correct fraction of selected modules, and in
  // some approximation of a uniform distribution.
  seed = static_cast<unsigned int>( 10000*detector + 
                                    12345u*barrel_ec +
                                    9876u*layer_disk + 
                                    123*phi_module + 
                                    5432u*eta_module +
                                    5555u*side );
  rgen.SetSeed(seed);
  const bool is_dead = rgen.Rndm() < _random_fraction;
  if( true && is_dead ) { 
    cout << ( boost::format("DEAD MODULE : d%|1d| b%|2d| l%|1d| p%|3d| e%|3d| s%|3d|")
              % detector % barrel_ec % layer_disk % phi_module % eta_module % side )
         << endl;
  }
  return( is_dead );
}
