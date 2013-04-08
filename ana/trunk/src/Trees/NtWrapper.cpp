
#include <string>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <list>
#include <map>
#include <set>
#include <bitset>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/interprocess/sync/file_lock.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/filesystem.hpp>
#include "TrigFTKAna/ana_streams.hpp"
#include "TrigFTKAna/Trees/NtWrapper.hpp"
#include "TrigFTKAna/UniqueBarcode.hpp"
#include "TrigFTKAna/Detector.hpp"
#include "TrigFTKAna/AnaSiCluster.hpp"
#include "TrigFTKAna/AnaConfiguration.hpp"

using namespace std;
using namespace boost;

const unsigned long
NtWrapper::count_events( const std::string& filename )
{
  boost::shared_ptr<ana_streams::istream> inf( ana_streams::open_for_read( filename ) );
  string line;
  unsigned long n = 0ul;
  while( !(inf->eof()) ) {
    getline( *inf , line );
    if( line[0] == 'L' ) { ++n; }
  }
  return n;
}


const bool
NtWrapper::begin_file( const std::string& filename )
{
  // lock the file TrigFTKAna/.file_open_lock. this is a signal to all
  // other parallel jobs that we are about to attempt a file open, and
  // they should chill. the motivation for this (and the subsequent
  // random delay) is to reduce the peak load on dcache servers that
  // might otherwise get hit with many file open requests
  // simultaneously.
  boost::shared_ptr<ana_streams::istream> tmp;
  try { 
    boost::interprocess::file_lock lock( "./.file_open_lock" );
    boost::interprocess::scoped_lock<boost::interprocess::file_lock> sc_lock( lock );
    tmp = ana_streams::open_for_read( ana_streams::root_dcap_filename(filename) );
  } catch( ... ) {
    // do nothing and hope for the best.
    tmp = ana_streams::open_for_read( ana_streams::root_dcap_filename(filename) );
  }
  if( !tmp || !(tmp->good()) ) { return false; }
  _inf.swap( tmp );
  _last_event = optional<unsigned int>();
  _open_filename = filename;
  return true;
}

const bool
NtWrapper::end_file()
{
  _inf.reset();
  _open_filename = string();
  _last_event = optional<unsigned int>();
}

const bool
NtWrapper::get_event( const unsigned long& ievent )
{
  // check that ievent is the next event to be processed.  there is
  // (at present) no mechanism for random, non-sequential access.
  if( !_last_event && ievent!=0ul ) {
    cout << " warning: NtWrapper::get_event is intended for sequential access only; cannot get event " << ievent << endl;
    return false;
  }
  if( _last_event && ievent!=((*_last_event)+1) ) {
    cout << " warning: NtWrapper::get_event is intended for sequential access only; cannot get event " << ievent << " after event " << (*_last_event) << endl;
    return false;
  }
  // parse the file until the end-of-event character is reached or the file ends.
  // line type characters:
  const char run_token = 'R';
  const char event_token = 'F';
  const char end_of_event_token = 'L';
  const char sichannel_token = 'S';
  const char reconstructed_track_token = 'E';
  const char pixel_cluster_token = 'P';
  const char sct_cluster_token = 'C';
  const char truth_track_token = 'T';
  const char beamline_token = 'V';
  const char extra_token = 'X';
  const char bad_module_token = 'B';
  // variables to read per event
  clear();
  EventClusterMap event_clu_map;
  //
  static bool reset_bad_module_list = true;
  //
  bool is_valid = false;
  TokenColl tokens;
  //tokens.reserve( 12 );
  do { // event read loop
    // retrieve the next line
    string line;
    if( !_inf || !_inf->good() || !getline(*_inf,line) ) {
      // may or may not be premature end of usable file. let the event manager figure it out.
      // cout << " warning: NtWrapper::get_event out of file at event " << ievent << " after event " << (*_last_event) << endl;
      return false;
    }
    //boost::trim( line );
    const char code( line.empty() ? 0 : line[0] );
    // optimization for high luminosity: early veto of 'P' and 'C' lines when reading offline silicon cluster data is disabled.
    // the tokenizing operation is unnecessary and, cumulatively, very expensive in that case.
    if( code==sichannel_token && !AnaConfiguration::read_offline_si_channels() ) { continue; }
    if( (code==pixel_cluster_token || code==sct_cluster_token) && !AnaConfiguration::read_offline_si_clusters() ) { continue; }
    //
    // boost::split( tokens , line, boost::is_any_of("\t \n") , boost::token_compress_on );
    // tokenize. this is an expensive operation.
    tokenize( line , tokens );
    if( tokens.empty() ) { continue; }
    std::reverse( tokens.begin() , tokens.end() );
    // assert( tokens.front()[0] == code );
    assert( tokens.back()[0] == code );
    // tokens.pop_front();
    tokens.pop_back();
    istringstream sline;
    switch( code ) {
    case run_token:
      //      convert( tokens.front() , _run_number );
      convert( tokens.back() , _run_number );
      break;
    case event_token:
      //convert( tokens.front() , _event_number );
      convert( tokens.back() , _event_number );
      break;
    case end_of_event_token:
      {
        unsigned long iev;
        // convert( tokens.front() , iev );
        convert( tokens.back() , iev );
        if( iev != _event_number ) {
          cout << " warning: NtWrapper::get_event event mismatch at " << ievent << " != " << iev << " after event " << ( _last_event ? (*_last_event) : -1) << endl;
          return false;
        } else {
          is_valid = true;
        }
      }
      break;
    case sichannel_token:
      // read silicon channel
      read_silicon_channel( tokens );
      break;
    case pixel_cluster_token:
      read_pixel_cluster( tokens );
      break;
    case sct_cluster_token:
      read_sct_cluster( tokens );
      break;
    case reconstructed_track_token:
      // read reconstructed track
      read_offline_track( tokens , event_clu_map );
      break;
    case truth_track_token:
      // read truth track
      read_truth_track( tokens );
      break;
    case beamline_token:
      read_beamline( tokens );
      break;
    case extra_token:
      if( atoi( tokens.back().c_str() ) == 1 ) { 
        tokens.pop_back();
        read_extra_trigger( tokens );
      } else {
        tokens.pop_back();
        read_extra_eventinfo( tokens );
      }
      static unsigned long last_lumi_block = std::numeric_limits<unsigned long>::max();
      if( _lumi_block_number!=last_lumi_block ) {
        last_lumi_block = _lumi_block_number;
        reset_bad_module_list = true;
      }
      break;
    case bad_module_token:
      read_bad_modules( tokens );
      break;
    default:
      cout << " warning: NtWrapper::get_event can't recognize line type " << code <<  " at event " << ievent << " after event " << ( _last_event ? (*_last_event) : -1 )<< endl;
    };
  } while( !is_valid );

  // if the was a new lumi block, switch to the new list of bad modules
  if( reset_bad_module_list ) { 
    _lumi_block_bad_modules.swap( _event_bad_modules );
    _event_bad_modules.reset();
    reset_bad_module_list = false;
  }

  // resolve si cluster mapping for reconstructed tracks
  //        event_clu_map.insert( pair<AnaTrack*, list<int> >(newtrk,iclu) );
  if( false && !_silicon_clusters.empty() ) { // FIXME - temp. disabled
    const int nrecsi = _silicon_clusters.size();
    map<int,int> hit_usage;
    for( map< boost::shared_ptr<AnaTrack> , list<int> >::iterator i=event_clu_map.begin(), f=event_clu_map.end(); i!=f; ++i ) {
      boost::shared_ptr<AnaTrack> track( i->first );
      if( !(track->_si_clusters.empty()) ) { continue; }
      assert( track->_si_clusters.empty() );
      track->_nhits = 0;
      track->_nmisses = 0;
      track->_n_b_layer_hits = 0;
      track->_n_pixel_hits = 0;
      track->_n_sct_hits = 0;
      track->_n_pixel_holes = 0;
      track->_n_sct_holes = 0;
      bitset<3> hit_layers_pixel;
      bitset<8> hit_layers_sct;
      const list<int>& indices( i->second );
      for( list<int>::const_iterator iclu=indices.begin(), fclu=indices.end(); iclu!=fclu; ++iclu ) {
        assert( *iclu < nrecsi );
        boost::shared_ptr<const AnaSiCluster> cluster = _silicon_clusters[*iclu];
        track->_si_clusters.push_back( cluster );
        ++hit_usage[*iclu];
        ++track->_nhits;
        if( cluster->is_pixel() ) { 
          hit_layers_pixel.set( cluster->layer_or_disk() , 1 );
          if( cluster->layer_or_disk()==0 ) { ++track->_n_b_layer_hits; }
          ++track->_n_pixel_hits;
        } else {
          hit_layers_sct.set( std::min(cluster->layer_or_disk(),7) , 1 );//count ec layer 8 as layer 7
          ++track->_n_sct_hits;
        }
      }
      track->_n_pixel_holes = hit_layers_pixel.size()-hit_layers_pixel.count();
      track->_n_sct_holes = hit_layers_sct.size()-hit_layers_sct.count();
      track->_nmisses = track->_n_pixel_holes + track->_n_sct_holes;
    } // end fill hit counts for offline tracks (except numbers of shared hits)
    // second pass to fill shared hit counts
    for( map< boost::shared_ptr<AnaTrack> , list<int> >::iterator i=event_clu_map.begin(), f=event_clu_map.end(); i!=f; ++i ) {
      boost::shared_ptr<AnaTrack> track( i->first );
      if( track->_si_clusters.empty() ) { continue; }
      track->_n_pixel_shared = 0;
      track->_n_sct_shared = 0;
      const list<int>& indices( i->second );
      for( list<int>::const_iterator iclu=indices.begin(), fclu=indices.end(); iclu!=fclu; ++iclu ) {
	assert( *iclu < nrecsi );
	const boost::shared_ptr<const AnaSiCluster>& cluster = _silicon_clusters[*iclu];
	if( hit_usage[*iclu]<=1 ) { continue; }
	if( cluster->is_pixel() ) { 
	  ++track->_n_pixel_shared;
	} else {
	  track->_n_sct_shared;
	}
      }
      if( false ) { 
        cout << "loaded offline track " << endl;
        track->print();
      }
    } // end fill shared hit counts for each track
  } // end resolve cluster mapping for reconstructed tracks

  if( true && !_silicon_clusters.empty() ) {
    // 20101010 - fix barcode matching fraction calculation for
    // offline tracks. recompute barcode from offline clusters.
    for( map< boost::shared_ptr<AnaTrack> , list<int> >::iterator i=event_clu_map.begin(), f=event_clu_map.end(); i!=f; ++i ) {
      boost::shared_ptr<AnaTrack> track( i->first );
      // loop over hits on this track and compute barcode matching fraction
      std::multiset<UniqueBarcode> track_hit_truth;
      BOOST_FOREACH( const boost::shared_ptr<const AnaSiCluster>& clu , track->_si_clusters ) {
        track_hit_truth.insert( clu->barcode() );
      } // end for each hit on track
      // find the most frequent barcode
      unsigned int best_barcode_frequency = 0u;
      UniqueBarcode best_barcode( -1 , -1 );
      unsigned int denominator = 0u;
      set<UniqueBarcode> processed_barcodes;
      for( std::multiset<UniqueBarcode>::const_iterator ibarcode=track_hit_truth.begin(), fbarcode=track_hit_truth.end(); ibarcode!=fbarcode; ++ibarcode ) {
        if( processed_barcodes.find(*ibarcode)!=processed_barcodes.end() ) { continue; }
        processed_barcodes.insert( *ibarcode );
        const unsigned int this_frequency = track_hit_truth.count( *ibarcode );
        denominator += this_frequency;
        if( (*ibarcode).barcode() < 0 ) { continue; }
        if( this_frequency > best_barcode_frequency ) {
          best_barcode = *ibarcode;
          best_barcode_frequency = this_frequency;
        }
      }
      if( false ) {
        cout << " recomputing truth information for track: "; track->print();
        cout << " number of hits on track : " << track->_si_clusters.size() << endl;
        cout << " number of hit links for track : " << i->second.size() << endl;
        cout << " total barcode fraction denominator : " << denominator << endl;
        cout << " total barcode fraction numerator : " << best_barcode_frequency << endl;
        cout << " best barcode: " << best_barcode.event_index() << " " << best_barcode.barcode() << endl;
      }
      track->_barcode = best_barcode;
      track->_barcode_matching_fraction = denominator > 0u ? static_cast<AnaTrack::ftype>(best_barcode_frequency)/denominator : 0.;
    }
  }


  if( false ) {
    cout << " NtWrapper: processed event " << _run_number << " " << _event_number << " with " 
         << _silicon_channels.size() << " silicon channels "
         << _offline_tracks.size() << " reconstructed offline tracks "
         << _truth_tracks.size() << " truth tracks "
         << endl;
  }

  // update last event accounting
  _last_event = ievent;
  return true;
}


// the following approach segfaults... not clear it would be faster anyway.
// void
// NtWrapper::tokenize( const std::string& line , TokenColl& tokens )
// {
//   // this is on a critical path and needs to be very fast.
//   tokens.clear();
//   tokens.reserve( 20 );
//   const char* delimiter = " \t\n";
//   boost::scoped_ptr<char> tmp( new char[line.size()] );
//   char* s1 = tmp.get();
//   strcpy( s1 , line.c_str() );
//   char* s3 = 0;
//   char* token = strtok_r( s1 , delimiter , &s3 );
//   while( token ) {
//     tokens.push_back( token );
//     token = strtok_r( 0 , delimiter , &s3 );
//   }
// }

void
NtWrapper::tokenize( const std::string& line , TokenColl& tokens )
{
  // this is on a critical path and needs to be very fast.
  tokens.clear();
  tokens.reserve( 20 );
  bool start_new_token = true;
  string current_token = "";
  current_token.reserve( 10 );
  for( string::const_iterator ic=line.begin(), fc=line.end(); ic!=fc; ++ic ) {
    const char c( *ic );
    const bool is_whitespace = (c==' ' || c=='\t' || c=='\n');
    if( start_new_token ) {
      if( is_whitespace ) { continue; }
      start_new_token = false;
      current_token.push_back( c );
      continue;
    } else {
      if( !is_whitespace ) { 
        current_token.push_back( c );
        continue;
      }
      // end token
      tokens.push_back( current_token );
      current_token.clear();
      start_new_token = true;
    }
  }
  if( !start_new_token ) {
    tokens.push_back( current_token );
  }
}

void
NtWrapper::read_silicon_channel( TokenColl& tokens )
{
  if( !AnaConfiguration::read_offline_si_channels() ) { return; }
  float x,y,z;
  int dettype;
  int barrel_ec;
  int layer_or_disk;
  unsigned int phi_module;
  int eta_module;
  unsigned int phi_index , eta_index;
  unsigned int tot;
  int side;
  unsigned int strip , size;
  // convert( tokens.front() , x ); tokens.pop_front();
  convert( tokens.back() , x ); tokens.pop_back();
  // convert( tokens.front() , y ); tokens.pop_front();
  convert( tokens.back() , y ); tokens.pop_back();
  // convert( tokens.front() , z ); tokens.pop_front();
  convert( tokens.back() , z ); tokens.pop_back();
  // convert( tokens.front() , dettype ); tokens.pop_front();
  convert( tokens.back() , dettype ); tokens.pop_back();
  // convert( tokens.front() , barrel_ec ); tokens.pop_front();
  convert( tokens.back() , barrel_ec ); tokens.pop_back();
  // convert( tokens.front() , layer_or_disk ); tokens.pop_front();
  convert( tokens.back() , layer_or_disk ); tokens.pop_back();
  // convert( tokens.front() , phi_module ); tokens.pop_front();
  convert( tokens.back() , phi_module ); tokens.pop_back();
  // convert( tokens.front() , eta_module ); tokens.pop_front();
  convert( tokens.back() , eta_module ); tokens.pop_back();
  if( dettype==0 ) {
    // convert( tokens.front() , side ); tokens.pop_front();
    convert( tokens.back() , side ); tokens.pop_back();
    // convert( tokens.front() , strip ); tokens.pop_front();
    convert( tokens.back() , strip ); tokens.pop_back();
    // convert( tokens.front() , size ); tokens.pop_front();
    convert( tokens.back() , size ); tokens.pop_back();
  } else if( dettype==1 ) {
    // convert( tokens.front() , phi_index ); tokens.pop_front();
    convert( tokens.back() , phi_index ); tokens.pop_back();
    // convert( tokens.front() , eta_index ); tokens.pop_front();
    convert( tokens.back() , eta_index ); tokens.pop_back();
    // convert( tokens.front() , tot ); tokens.pop_front();
    convert( tokens.back() , tot ); tokens.pop_back();
  } else {
    cout << " warning: NtWrapper::get_event garbled si channel ignored." << endl;
    return;
  }
  // determine format.
  // June 2009 format: remaining fields are the UniqueBarcode and the highest pt contributing to the channel, in that order.
  // August 2009 format: remaing fields are event_index, barcode, highest pt, parentage mask.
  UniqueBarcode code;
  detector::TruthParentBitmask parentage_mask;
  int highest_pt = 0;
  if( tokens.size()==4 ) { // August 2009 format
    long event_index, barcode;
    //float highest_pt;
    // convert( tokens.front() , event_index ); tokens.pop_front();
    convert( tokens.back() , event_index ); tokens.pop_back();
    // convert( tokens.front() , barcode ); tokens.pop_front();
    convert( tokens.back() , barcode ); tokens.pop_back();
    code = UniqueBarcode( event_index , barcode );
    // convert( tokens.front() , highest_pt ); tokens.pop_front();
    convert( tokens.back() , highest_pt ); tokens.pop_back();
    unsigned long mask = 0ul;
    // convert( tokens.front() , mask ); tokens.pop_front();
    convert( tokens.back() , mask ); tokens.pop_back();
    parentage_mask = mask;
  } else { // assume June 2009 format
    long barcode;
    // convert( tokens.front() , barcode ); tokens.pop_front();
    convert( tokens.back() , barcode ); tokens.pop_back();
    code = UniqueBarcode(barcode);
  }
  boost::shared_ptr<AnaSiChannel> result;
  if( dettype==0 ) { // SCT
    result.reset( new AnaSiChannel(AnaSiChannel::STRIP , x , y , z , barrel_ec ,
                                   layer_or_disk , phi_module , eta_module ,
                                   (side==1? AnaSiChannel::STEREO : AnaSiChannel::RPHI) , strip , size , code , parentage_mask)
                  );
  } else if( dettype==1 ) { // pixel
    result.reset( new AnaSiChannel(AnaSiChannel::PIXEL , x , y , z , barrel_ec ,
                                   layer_or_disk , phi_module , eta_module ,
                                   phi_index , eta_index , tot , code , parentage_mask) 
                  );
  }
  result->highest_pt( highest_pt );
  _silicon_channels.push_back( result );
}


void
NtWrapper::read_pixel_cluster( TokenColl& tokens ) 
{
  if( !AnaConfiguration::read_offline_si_clusters() ) { return; }
  if( tokens.size() < 18 ) { 
    static unsigned int warncount = 0;
    if( warncount < 10 ) {
      cout << "skipping pixel cluster entry." << endl;
    } else if( warncount == 10 ) { 
      cout << "further skipping pixel cluster entry warning will be surpressed." << endl;
    }
    return;
  }
  assert( tokens.size() >= 18 );
  boost::shared_ptr<AnaSiCluster> cluster( boost::make_shared<AnaSiCluster>() );
  cluster->_cluster_type = AnaSiChannel::PIXEL;
  cluster->_algorithm = AnaSiCluster::OFFLINE;
  // convert( tokens.front() , cluster->_x ); tokens.pop_front();
  convert( tokens.back() , cluster->_x ); tokens.pop_back();
  // convert( tokens.front() , cluster->_y ); tokens.pop_front();
  convert( tokens.back() , cluster->_y ); tokens.pop_back();
  // convert( tokens.front() , cluster->_z ); tokens.pop_front();
  convert( tokens.back() , cluster->_z ); tokens.pop_back();
  // convert( tokens.front() , cluster->_barrel_ec ); tokens.pop_front();
  convert( tokens.back() , cluster->_barrel_ec ); tokens.pop_back();
  // convert( tokens.front() , cluster->_layer_or_disk ); tokens.pop_front();
  convert( tokens.back() , cluster->_layer_or_disk ); tokens.pop_back();
  // convert( tokens.front() , cluster->_phi_module ); tokens.pop_front();
  convert( tokens.back() , cluster->_phi_module ); tokens.pop_back();
  // convert( tokens.front() , cluster->_eta_module ); tokens.pop_front();
  convert( tokens.back() , cluster->_eta_module ); tokens.pop_back();
  cluster->_ftk_logical_layer = detector::ftk_physical_to_logical_layer( detector::PIXEL_DETECTOR , cluster->_barrel_ec , cluster->_layer_or_disk , cluster->_eta_module , 0 );
  assert( cluster->_ftk_logical_layer >=0 && cluster->_ftk_logical_layer<=2 );
  // convert( tokens.front() , cluster->_phi_index ); tokens.pop_front();
  convert( tokens.back() , cluster->_phi_index ); tokens.pop_back();
  // convert( tokens.front() , cluster->_eta_index ); tokens.pop_front();
  convert( tokens.back() , cluster->_eta_index ); tokens.pop_back();
  // convert( tokens.front() , cluster->_local_x ); tokens.pop_front();
  convert( tokens.back() , cluster->_local_x ); tokens.pop_back();
  // convert( tokens.front() , cluster->_local_z ); tokens.pop_front();
  convert( tokens.back() , cluster->_local_z ); tokens.pop_back();
  // convert( tokens.front() , cluster->_width_x ); tokens.pop_front();
  convert( tokens.back() , cluster->_width_x ); tokens.pop_back();
  // convert( tokens.front() , cluster->_width_z ); tokens.pop_front();
  convert( tokens.back() , cluster->_width_z ); tokens.pop_back();
  // convert( tokens.front() , cluster->_n_channels ); tokens.pop_front();
  convert( tokens.back() , cluster->_n_channels ); tokens.pop_back();
  long event_index, barcode;
  // convert( tokens.front() , event_index ); tokens.pop_front();
  convert( tokens.back() , event_index ); tokens.pop_back();
  // convert( tokens.front() , barcode ); tokens.pop_front();
  convert( tokens.back() , barcode ); tokens.pop_back();
  cluster->_barcode = UniqueBarcode( event_index , barcode );
  // convert( tokens.front() , cluster->_truth_pt ); tokens.pop_front();
  convert( tokens.back() , cluster->_truth_pt ); tokens.pop_back();
  unsigned long parent_mask;
  // convert( tokens.front() , parent_mask ); tokens.pop_front();
  convert( tokens.back() , parent_mask ); tokens.pop_back();
  cluster->_truth_parent_bitmask = detector::TruthParentBitmask( parent_mask );
  _silicon_clusters.push_back( boost::const_pointer_cast<const AnaSiCluster>(cluster) );
}


void
NtWrapper::read_sct_cluster( TokenColl& tokens )
{
  if( !AnaConfiguration::read_offline_si_clusters() ) { return; }
  assert( tokens.size() >= 16 );
  boost::shared_ptr<AnaSiCluster> cluster( boost::make_shared<AnaSiCluster>() );
  cluster->_cluster_type = AnaSiChannel::STRIP;
  cluster->_algorithm = AnaSiCluster::OFFLINE;
  // convert( tokens.front() , cluster->_x ); tokens.pop_front();
  convert( tokens.back() , cluster->_x ); tokens.pop_back();
  // convert( tokens.front() , cluster->_y ); tokens.pop_front();
  convert( tokens.back() , cluster->_y ); tokens.pop_back();
  // convert( tokens.front() , cluster->_z ); tokens.pop_front();
  convert( tokens.back() , cluster->_z ); tokens.pop_back();
  // tokens.pop_front(); // sct: pixel==0
  tokens.pop_back(); // sct: pixel==0
  // convert( tokens.front() , cluster->_barrel_ec ); tokens.pop_front();
  convert( tokens.back() , cluster->_barrel_ec ); tokens.pop_back();
  // convert( tokens.front() , cluster->_layer_or_disk ); tokens.pop_front();
  convert( tokens.back() , cluster->_layer_or_disk ); tokens.pop_back();
  // convert( tokens.front() , cluster->_phi_module ); tokens.pop_front();
  convert( tokens.back() , cluster->_phi_module ); tokens.pop_back();
  // convert( tokens.front() , cluster->_eta_module ); tokens.pop_front();
  convert( tokens.back() , cluster->_eta_module ); tokens.pop_back();
  // convert( tokens.front() , cluster->_eta_index ); tokens.pop_front(); // side
  convert( tokens.back() , cluster->_eta_index ); tokens.pop_back(); // side
  cluster->_ftk_logical_layer = detector::ftk_physical_to_logical_layer( detector::SCT_DETECTOR ,
                                                                         cluster->_barrel_ec ,
                                                                         cluster->_layer_or_disk ,
                                                                         cluster->_eta_module , 
                                                                         cluster->_eta_index ); //==AnaSiChannel::RPHI ? 0 : 1 );
  assert( cluster->_ftk_logical_layer >=3 && cluster->_ftk_logical_layer<=10 );
  cluster->_eta_index = ( detector::ftk_physical_layer_is_stereo_sct( cluster->_barrel_ec , 
                                                                      cluster->_layer_or_disk ) ? 
                          AnaSiChannel::STEREO : AnaSiChannel::RPHI );
  // convert( tokens.front() , cluster->_phi_index ); tokens.pop_front();
  convert( tokens.back() , cluster->_phi_index ); tokens.pop_back();
  // convert( tokens.front() , cluster->_local_x ); tokens.pop_front();
  convert( tokens.back() , cluster->_local_x ); tokens.pop_back();
  // convert( tokens.front() , cluster->_width_x ); tokens.pop_front();
  convert( tokens.back() , cluster->_width_x ); tokens.pop_back();
  cluster->_n_channels = cluster->_width_x;
  long event_index, barcode;
  // convert( tokens.front() , event_index ); tokens.pop_front();
  convert( tokens.back() , event_index ); tokens.pop_back();
  // convert( tokens.front() , barcode ); tokens.pop_front();
  convert( tokens.back() , barcode ); tokens.pop_back();
  cluster->_barcode = UniqueBarcode( event_index , barcode );
  // convert( tokens.front() , cluster->_truth_pt ); tokens.pop_front();
  convert( tokens.back() , cluster->_truth_pt ); tokens.pop_back();
  unsigned long parent_mask;
  // convert( tokens.front() , parent_mask ); tokens.pop_front();
  convert( tokens.back() , parent_mask ); tokens.pop_back();
  cluster->_truth_parent_bitmask = detector::TruthParentBitmask( parent_mask );
  _silicon_clusters.push_back( boost::const_pointer_cast<const AnaSiCluster>(cluster) );
}

void
NtWrapper::read_offline_track( TokenColl& tokens , EventClusterMap& event_clu_map )
{
  AnaTrack::ftype qpt;
  AnaTrack tmp;
  // convert( tokens.front() , tmp._d0 ); tokens.pop_front();
  convert( tokens.back() , tmp._d0 ); tokens.pop_back();
  // convert( tokens.front() , tmp._z0 ); tokens.pop_front();
  convert( tokens.back() , tmp._z0 ); tokens.pop_back();
  // convert( tokens.front() , tmp._phi0 ); tokens.pop_front();
  convert( tokens.back() , tmp._phi0 ); tokens.pop_back();
  // convert( tokens.front() , tmp._lambda ); tokens.pop_front();
  convert( tokens.back() , tmp._lambda ); tokens.pop_back();
  // convert( tokens.front() , qpt ); tokens.pop_front();
  convert( tokens.back() , qpt ); tokens.pop_back();
  // determine format.
  // June 2009 format: remaining fields are the UniqueBarcode and the truth hit matching fraction, in that order.
  // August 2009 format: remaining fields are event_index, barcode, matching fraction.
  // January 2010 format: remaining fields are event_index, barcode, matching fraction, cluster(s)
  UniqueBarcode code;
  AnaTrack::ftype matching_fraction;
  list<int> iclu;
  if( tokens.size()>= 3 ) { // August 2009 format
    long event_index, barcode;
    // convert( tokens.front() , event_index ); tokens.pop_front();
    convert( tokens.back() , event_index ); tokens.pop_back();
    // convert( tokens.front() , barcode ); tokens.pop_front();
    convert( tokens.back() , barcode ); tokens.pop_back();
    code = UniqueBarcode( event_index , barcode );
    // convert( tokens.front() , matching_fraction ); tokens.pop_front();
    convert( tokens.back() , matching_fraction ); tokens.pop_back();
    while( !tokens.empty() ) {
      int ipix=-1;
      // convert( tokens.front() , ipix );
      convert( tokens.back() , ipix );
      iclu.push_back( ipix );
      // tokens.pop_front();
      tokens.pop_back();
    }
  } else { // assume June 2009 format
    long barcode;
    // convert( tokens.front() , barcode ); tokens.pop_front();
    convert( tokens.back() , barcode ); tokens.pop_back();
    code = UniqueBarcode( barcode );
    // convert( tokens.front() , matching_fraction ); tokens.pop_front();
    convert( tokens.back() , matching_fraction ); tokens.pop_back();
  }
  tmp._alg = AnaTrack::Algorithm::OFFL;
  tmp._curv = detector::safe_divide(static_cast<AnaTrack::ftype>(1),qpt)/1000.;
  tmp._chi2 = std::numeric_limits<AnaTrack::ftype>::max();
  tmp._ndof = 0;
  tmp._nhits = 0;
  tmp._nmisses = std::numeric_limits<unsigned int>::max();
  tmp._barcode = code;
  tmp._barcode_matching_fraction = matching_fraction;
  // record offline silicon cluster indices
  boost::shared_ptr<AnaTrack> newtrk( boost::make_shared<AnaTrack>(tmp) );
  event_clu_map.insert( pair< boost::shared_ptr<AnaTrack> , list<int> > (newtrk,iclu) );
  _offline_tracks.push_back( newtrk );
}

void
NtWrapper::read_truth_track( TokenColl& tokens )
{
  AnaTrack::ftype x0, y0, z0, q, px, py, pz;
  int irecmatch;
  AnaTrack::ftype precmatch;
  // convert( tokens.front() , x0 ); tokens.pop_front();
  convert( tokens.back() , x0 ); tokens.pop_back();
  // convert( tokens.front() , y0 ); tokens.pop_front();
  convert( tokens.back() , y0 ); tokens.pop_back();
  // convert( tokens.front() , z0 ); tokens.pop_front();
  convert( tokens.back() , z0 ); tokens.pop_back();
  // convert( tokens.front() , q ); tokens.pop_front();
  convert( tokens.back() , q ); tokens.pop_back();
  // convert( tokens.front() , px ); tokens.pop_front();
  convert( tokens.back() , px ); tokens.pop_back();
  // convert( tokens.front() , py ); tokens.pop_front();
  convert( tokens.back() , py ); tokens.pop_back();
  // if pT < 1/2 offline pt cut (e.g. 500 MeV), drop the truth track info to save time in high lumi pileup events.
  if( detector::quadrature( px , py ) < 0.5 * AnaConfiguration::minimum_track_pt_cut()*1000. ) { return; }
  _truth_tracks.push_back( boost::make_shared<AnaTrack>() );
  AnaTrack& tmp( *(boost::const_pointer_cast<AnaTrack>(_truth_tracks.back())) );
  tmp._z0 = z0;
  // convert( tokens.front() , pz ); tokens.pop_front();
  convert( tokens.back() , pz ); tokens.pop_back();
  // convert( tokens.front() , tmp._pdg_code ); tokens.pop_front();
  convert( tokens.back() , tmp._pdg_code ); tokens.pop_back();
  // convert( tokens.front() , irecmatch ); tokens.pop_front();
  convert( tokens.back() , irecmatch ); tokens.pop_back();
  // convert( tokens.front() , precmatch ); tokens.pop_front();
  convert( tokens.back() , precmatch ); tokens.pop_back();
  // determine format.
  // June 2009 format: remaining fields are the UniqueBarcode and the detpaper flag, in that order.
  // August 2009 format: remaing fields are event_index, barcode, parentage mask, matching fraction.
  UniqueBarcode code;
  detector::TruthParentBitmask parentage_mask;
  if( tokens.size()==4 ) { // August 2009 format
    long event_index, barcode;
    // convert( tokens.front() , event_index ); tokens.pop_front();
    convert( tokens.back() , event_index ); tokens.pop_back();
    // convert( tokens.front() , barcode ); tokens.pop_front();
    convert( tokens.back() , barcode ); tokens.pop_back();
    code = UniqueBarcode( event_index , barcode );
    unsigned long mask;
    // convert( tokens.front() , mask ); tokens.pop_front();
    convert( tokens.back() , mask ); tokens.pop_back();
    parentage_mask = mask;
  } else { // assume June 2009 format
    long barcode;
    // convert( tokens.front() , barcode ); tokens.pop_front();
    convert( tokens.back() , barcode ); tokens.pop_back();
    code = UniqueBarcode( barcode );
  }
  // convert( tokens.front() , tmp._detpaper ); tokens.pop_front();
  convert( tokens.back() , tmp._detpaper ); tokens.pop_back();
  tmp._alg = AnaTrack::Algorithm::TRUTH;
  tmp._curv = detector::safe_divide(q,detector::quadrature(px,py));
  tmp._d0 = detector::quadrature(x0,y0)*detector::d0_sign(x0,y0,px,py);
  tmp._phi0 = std::atan2(py,px);
  tmp._lambda = detector::safe_divide(pz,detector::quadrature(px,py));
  tmp._cov_cc = std::numeric_limits<AnaTrack::ftype>::min();
  tmp._cov_dd = std::numeric_limits<AnaTrack::ftype>::min();
  tmp._cov_pp = std::numeric_limits<AnaTrack::ftype>::min();
  tmp._cov_zz = std::numeric_limits<AnaTrack::ftype>::min();
  tmp._cov_tt = std::numeric_limits<AnaTrack::ftype>::min();
  tmp._cov_cd = tmp._cov_cp = tmp._cov_cz = tmp._cov_ct = 0.;
  tmp._cov_dp = tmp._cov_dz = tmp._cov_dt = 0.;
  tmp._cov_pz = tmp._cov_pt = 0.;
  tmp._cov_zt = 0.;
  tmp._chi2 = std::numeric_limits<AnaTrack::ftype>::min();
  tmp._ndof = 0;
  tmp._nhits = 0;
  tmp._nmisses = 0;
  tmp._barcode = code;
  tmp._barcode_matching_fraction = 1.;
  tmp._truth_parent_bitmask = parentage_mask;
}


void
NtWrapper::read_beamline( TokenColl& tokens )
{
  _beamline.reset( new AnaBeamline );
  if( tokens.size() >= 9 ) { 
    // convert( tokens.front() , _beamline->_x0 ); tokens.pop_front();
    convert( tokens.back() , _beamline->_x0 ); tokens.pop_back();
    // convert( tokens.front() , _beamline->_y0 ); tokens.pop_front();
    convert( tokens.back() , _beamline->_y0 ); tokens.pop_back();
    // convert( tokens.front() , _beamline->_z0 ); tokens.pop_front();
    convert( tokens.back() , _beamline->_z0 ); tokens.pop_back();
    // convert( tokens.front() , _beamline->_sigma_x ); tokens.pop_front();
    convert( tokens.back() , _beamline->_sigma_x ); tokens.pop_back();
    // convert( tokens.front() , _beamline->_sigma_y ); tokens.pop_front();
    convert( tokens.back() , _beamline->_sigma_y ); tokens.pop_back();
    // convert( tokens.front() , _beamline->_sigma_z ); tokens.pop_front();
    convert( tokens.back() , _beamline->_sigma_z ); tokens.pop_back();
    // convert( tokens.front() , _beamline->_cov_xy ); tokens.pop_front();
    convert( tokens.back() , _beamline->_cov_xy ); tokens.pop_back();
    // convert( tokens.front() , _beamline->_tilt_xz ); tokens.pop_front();
    convert( tokens.back() , _beamline->_tilt_xz ); tokens.pop_back();
    // convert( tokens.front() , _beamline->_tilt_yz ); tokens.pop_front();
    convert( tokens.back() , _beamline->_tilt_yz ); tokens.pop_back();
  }
  // create dummy primary vertex with this information
  boost::shared_ptr<AnaVertex> v( boost::make_shared<AnaVertex>() );
  v->_x = _beamline->_x0;
  v->_y = _beamline->_y0;
  v->_z = _beamline->_z0;
  v->_type = ATLAS::NotSpecified;
  _vertices.push_back( v );
}

void
NtWrapper::read_extra_trigger( TokenColl& tokens )
{
  _trigger.reset( new AnaTrigger );
  if( tokens.size() >= 14 ) { 
    bool tmp;
    convert( tokens.back() , tmp ); tokens.pop_back(); _trigger->_trig_l1_mbts_1_1_col = tmp; tokens.pop_back(); // ignore pass-thru
    convert( tokens.back() , tmp ); tokens.pop_back(); _trigger->_trig_l1_mbts_1_1_empty = tmp; tokens.pop_back(); // ignore pass-thru
    convert( tokens.back() , tmp ); tokens.pop_back(); _trigger->_trig_l1_mbts_1_1_unparied = tmp; tokens.pop_back(); // ignore pass-thru
    convert( tokens.back() , tmp ); tokens.pop_back(); _trigger->_trig_l1_mbts_1_1_unparied1 = tmp; tokens.pop_back(); // ignore pass-thru
    convert( tokens.back() , tmp ); tokens.pop_back(); _trigger->_trig_l1_mbts_1_1_unparied2 = tmp; tokens.pop_back(); // ignore pass-thru
    convert( tokens.back() , tmp ); tokens.pop_back(); _trigger->_trig_l1_mbts_1_1 = tmp; tokens.pop_back(); // ignore pass-thru
    convert( tokens.back() , tmp ); tokens.pop_back(); _trigger->_trig_l1_mbts_1 = tmp; tokens.pop_back(); // ignore pass-thru
  }
}

void
NtWrapper::read_extra_eventinfo( TokenColl& tokens )
{
  if( tokens.size() >= 2 ) { 
    convert( tokens.back() , _lumi_block_number ); tokens.pop_back(); 
    convert( tokens.back() , _bunch_number ); tokens.pop_back(); 
  }
}

void
NtWrapper::read_bad_modules( TokenColl& tokens )
{
  // don't clear this on each event; wrapper only provides it once per
  // (at the start of) each lumi block.
  if( !_event_bad_modules ) { _event_bad_modules.reset( new SiModuleList ); }
  if( tokens.size() >= 5 ) { 
    int tmp;
    convert( tokens.back() , tmp ); tokens.pop_back();
    if( tmp==1 ) { // pix
      int be, ld, em;
      unsigned int pm;
      convert( tokens.back() , be ); tokens.pop_back(); 
      convert( tokens.back() , ld ); tokens.pop_back(); 
      convert( tokens.back() , pm ); tokens.pop_back(); 
      convert( tokens.back() , em ); tokens.pop_back(); 
      _event_bad_modules->add( be , ld , pm , em );
    } else { // sct
      int be, ld, em, side;
      unsigned int pm;
      convert( tokens.back() , be ); tokens.pop_back(); 
      convert( tokens.back() , ld ); tokens.pop_back(); 
      convert( tokens.back() , pm ); tokens.pop_back(); 
      convert( tokens.back() , em ); tokens.pop_back(); 
      if( tokens.empty() ) { return; } // bad entry
      convert( tokens.back() , side ); tokens.pop_back();
      _event_bad_modules->add( be , ld , pm , em , (side==1? SiModuleList::STEREO : SiModuleList::RPHI) );
    }
  }
}
