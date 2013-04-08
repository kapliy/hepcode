
#include <string>
#include <algorithm>
#include <iterator>
#include <boost/version.hpp>
#include <boost/unordered_map.hpp>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/format.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/filesystem.hpp>
#include <boost/date_time.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/set.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/list.hpp>
#include "TrigFTKAna/serialize_bitset.hpp" // backport from boost 1.43
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/scoped_ptr.hpp>
#include <boost/serialization/optional.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/bimap.hpp>
#include <vector>
#include "TrigFTKAna/AnaEvent.hpp"
#include "TrigFTKAna/AnaRoad.hpp"
#include "TrigFTKAna/AnaSuperstrip.hpp"
#include "TrigFTKAna/AnaTrack.hpp"
#include "TrigFTKAna/AnaElectron.hpp"
#include "TrigFTKAna/AnaPhoton.hpp"
#include "TrigFTKAna/AnaMuon.hpp"
#include "TrigFTKAna/AnaJet.hpp"
#include "TrigFTKAna/AnaMET.hpp"
#include "TrigFTKAna/AnaTrigger.hpp"
#include "TrigFTKAna/AnaSiChannel.hpp"
#include "TrigFTKAna/AnaBeamline.hpp"
#include "TrigFTKAna/AnaVertex.hpp"
#include "TrigFTKAna/AnaTruthTable.hpp"
#include "TrigFTKAna/AnaEventNtuple.hpp"
#include "TrigFTKAna/ana_streams_helper.hpp"
#include "TrigFTKAna/ana_streams.hpp"

using namespace std;
using namespace boost;
using namespace boost::posix_time;
using namespace boost::filesystem;
using namespace boost::algorithm;

const bool
AnaEventNtuple::open_sample( const std::string& name , const Mode& mode )
{
  assert( _directory=="" );
  assert( _mode == CLOSED );
  assert( name!="" );
  _mode = mode;
  switch( _mode ) {
  case WRITE:
    {
      _directory = path_to_new_sample_directory( name );
      _file_map.clear();
      _event_map.clear();
      _meta_map.clear();
      _current_event_block_index = std::numeric_limits<FileIndex>::max();
      _current_event_offset = 0ul;
      _event_block.clear();
      _next_file_index = 0;
      _block_accepted = 0;
      _block_total = 0;
      _nevents_accepted = 0;
      _nevents_total = 0;
    }
    break;
  case READ: 
    {
      // name may be a directory or it may be the metadata xml.
      filesystem::path spec = name;
      // directory or metadata file must exist.
      if( !filesystem::exists( spec ) ) { 
        cout << "AnaEventNtuple " << name << " not found." << endl;
        return false; 
      }
      spec.remove_filename();
      _directory = spec.string();
      const bool ok = read_metadata();
      if( !ok ) { return false; }
    }
    break;
  default: assert( 0 );
  }
  return true;
}

void
AnaEventNtuple::add_event( boost::shared_ptr<const AnaEvent> event , const bool accept_event ) 
{
  assert( _mode==WRITE );
  assert( _directory!="" );
  // ensure that an event is not written twice (subsequent add_event
  // calls for the same event). complicated by accepted/total logic.
  // be careful to update these counters correctly. result should be
  // logical OR: accept_event_1 || accept_event_2 || ...
  if( _last_add_attempt && 
      _last_add_attempt->event_manager_index() == event->event_manager_index() ) {
    if( !accept_event ) { return; }
    // this is the same event and it should be accepted. was it accepted last time?
    if( _last_add_success && _last_add_success->event_manager_index() == event->event_manager_index() ) {
      // event was already accepted last time. 
      return;
    }
    ++_block_accepted;
    ++_nevents_accepted;
    _last_add_success = boost::const_pointer_cast<AnaEvent>(event);
  } else { 
    ++_block_total;
    ++_nevents_total;
    _last_add_attempt = boost::const_pointer_cast<AnaEvent>(event);
    if( !accept_event ) { return; }
    ++_block_accepted;
    ++_nevents_accepted;
    _last_add_success = boost::const_pointer_cast<AnaEvent>(event);
  }
  // add the event to the event block
  // check that this event is not already present in the event block.
  if( std::find_if( _event_block.begin() , _event_block.end() , 
                    boost::bind(&AnaEvent::event_manager_index,_1) == event->event_manager_index() ) != _event_block.end() ) { 
    return; 
  }
  // event is not already in the event block. add the event. need to
  // convert it to a non-const AnaEvent. we promise not to change it
  // in WRITE mode---we just want to avoid separate read and write
  // event blocks.
  _event_block.push_back( boost::const_pointer_cast<AnaEvent>(event) );
  // updating metadata is taken care of when writing the event block.
  // if the event block is "full", write it to disk and start a new
  // one. event count is a goal because someday this may become
  // adaptive (e.g. track total memory usage with DgMemory and switch
  // to a new block when we hit n MBs of RAM).
  if( _event_block.size() >= _nevents_per_block_goal ) {
    write_event_block( _next_file_index++ );
    _event_block.clear();
  }
}

const std::string
AnaEventNtuple::filename_for_index( const FileIndex& index )
{
  std::string result = _directory + "/";
  result += (format("%|010d|") % index).str();
  result += ".xml.bz2";
  //result += ".xml";
  return result;
}

void
AnaEventNtuple::write_metadata()
{
  const std::string filename = _directory + "/" + "metadata.xml.bz2";
  boost::shared_ptr<ana_streams::ostream> outf( ana_streams::open_for_write(filename) );
  if( !outf || !(*outf) ) { 
    cout << "warning: could not write metadata for " << _directory << "; ntuple is now corrupt." << endl;
    return;
  }
  boost::shared_ptr<archive::xml_oarchive> aro( new archive::xml_oarchive(*outf) );
  try { 
    (*aro) << BOOST_SERIALIZATION_NVP(_directory);
    (*aro) << BOOST_SERIALIZATION_NVP(_file_map);
    (*aro) << BOOST_SERIALIZATION_NVP(_meta_map);
    (*aro) << BOOST_SERIALIZATION_NVP(_next_file_index);
  } catch( archive::archive_exception& e ) {
    std::cout << e.code << std::endl;
    std::cout << e.what() << std::endl;
  }
  outf->flush();
  aro.reset(); // destroy the iarchive before the stream closes.
  outf.reset(); // inf passed by reference to text_oarchive; destroy after archive.
}

const bool
AnaEventNtuple::read_metadata()
{
  filesystem::path filename = filesystem::path(_directory) / "metadata.xml.bz2";
  boost::shared_ptr<boost::iostreams::filtering_istream> inf( ana_streams::open_for_read(filename.string()) );
  // if( inf ) { inf->reset(); }
  // inf.reset(); 
  if( !inf ) {
    cout << " AnaEventNtuple: could not read metadata " << filename.string() << endl;
    return false;
  }
  if( !(*inf) || !(inf->good()) ) {
    cout << " AnaEventNtuple: file read error: metadata " << filename.string() << " not open." << endl;
    return false; 
  }
  boost::shared_ptr<archive::xml_iarchive> ari( new archive::xml_iarchive(*inf) );
  try {
    (*ari) >> BOOST_SERIALIZATION_NVP(_directory);
    (*ari) >> BOOST_SERIALIZATION_NVP(_file_map);
    (*ari) >> BOOST_SERIALIZATION_NVP(_meta_map);
    (*ari) >> BOOST_SERIALIZATION_NVP(_next_file_index);
  } catch( archive::archive_exception& e ) {
    std::cout << e.code << std::endl;
    std::cout << e.what() << std::endl;
  }  
  ari.reset(); // destroy the iarchive before the stream closes.
  inf.reset(); // inf passed by reference to text_iarchive; it must be destroyed after archive.
  // rebuild event map
  _nevents_accepted = 0ul;
  _nevents_total = 0ul;
  for( FileMap::const_iterator i=_file_map.begin(), f=_file_map.end(); i!=f; ++i ) {
    const FileIndex& index( i->first );
    const FileMetadata& meta( i->second );
    _event_map[ _nevents_accepted ] = index;
    _nevents_accepted += meta.nevents_accepted();
    _nevents_total += meta.nevents_total();
  }
  cout << " AnaEventNtuple: loaded metadata " << filename.string() << " with " << _nevents_accepted << " events " << endl;
  return true;
}

boost::shared_ptr<const AnaEvent>
AnaEventNtuple::get_event( unsigned long& ievent )
{
  if( ievent >= _nevents_accepted ) { return boost::shared_ptr<const AnaEvent>(); }
  // look up correct event block from metadata.
  const FileIndex correct_index = index_for_sequential_event( ievent );
  if( correct_index == std::numeric_limits<FileIndex>::max() ) {
    cout << " could not find sequential event " << ievent << " from metadata." << endl;
    return boost::shared_ptr<const AnaEvent>();
  }
  if( _current_event_block_index!=correct_index ) {
    // we need to locate and load the correct event block
    const bool ok = read_event_block( correct_index );
    if( !ok ) { return boost::shared_ptr<const AnaEvent>(); }
    _current_event_block_index = correct_index;
    _current_event_offset = 0ul;
    for( FileMap::const_iterator i=_file_map.begin(), f=_file_map.end(); i!=f; ++i ) {
      const FileIndex& index( i->first );
      if( index >= correct_index ) { break; }
      const FileMetadata& meta( i->second );
      _current_event_offset += meta.nevents_accepted();
    }
  } // if read different event block
  // correct event block is loaded. this is now a quick operation.
  // no read necessary. compute correct local event number and return.
  assert( ievent < _nevents_accepted );
  const unsigned long local_event = ievent - _current_event_offset;
  assert( local_event < _event_block.size() );
  return( boost::const_pointer_cast<const AnaEvent>( _event_block[local_event] ) );
}

const AnaEventNtuple::FileIndex
AnaEventNtuple::index_for_sequential_event( const unsigned long& ievent )
{
  EventFileMap::const_iterator result = _event_map.end();
  for( EventFileMap::const_iterator it=_event_map.begin(), ft=_event_map.end(); it!=ft; ++it ) {
    const unsigned long& block_start_event( it->first );
    if( ievent < block_start_event ) { break; }
    result = it;
  }
  if( result != _event_map.end() ) {
    return result->second;
  }
  assert( !"logic error or corrupt metadata" );
  return std::numeric_limits<FileIndex>::max();
}


void
AnaEventNtuple::close_sample()
{
  assert( _directory!="" );
  assert( _mode != CLOSED );
  switch( _mode ) {
  case WRITE:
    {
      const std::string nt_out_dir = directory();
      write_event_block( _next_file_index++ );
      write_metadata();
      cout << " AnaEventNtuple saved to directory: " << nt_out_dir << endl;
    }
    break;
  case READ: break;
  default: assert( 0 );
  }  
  _file_map.clear();
  _event_map.clear();
  _meta_map.clear();
  _event_block.clear();
  _current_event_block_index = std::numeric_limits<FileIndex>::max();
  _current_event_offset = 0ul;
  _next_file_index = 0;
  _directory = "";
  _mode = CLOSED;
}

const std::string
AnaEventNtuple::path_to_new_sample_directory( const std::string& directory )
{
  // do %DATE and %TIME substitution, if needed
  string pattern = directory;
  // retrieve date and time strings in YYYYMMDD and HHMMSS format
  ptime now( second_clock::local_time() );
  string temp( to_iso_string(now) );
  vector<string> isonow;
  split( isonow , temp , is_any_of("T") );
  assert( isonow.size()==2 );
  // replace all occurences of %DATE and %TIME with their values as of now.
  replace_all(pattern,"%DATE", isonow.front() );
  replace_all(pattern,"%TIME", isonow.back() );
  // remove trailing slashes and periods
  trim_right_if(pattern,is_any_of("/.")); 
  // check for (create) output directory.
  filesystem::path output_path( pattern );
  if( boost::filesystem::extension(output_path) != "aen" ) { 
#if BOOST_VERSION>=104800
    output_path.replace_extension( ".aen" );
#else
    output_path.replace_extension( "aen" );
#endif
  }
  // don't want to overwrite an extant
  // ntuple, so check whether or not pattern already exists. if it
  // does, append an integer that makes it unique.
  filesystem::path original_path = output_path;
  filesystem::path metadata_path = output_path / "metadata.xml.bz2";
  std::string orig_extension = extension(output_path);
  unsigned int version_number = 0;
  while( filesystem::exists( metadata_path ) ) {
    ++version_number;
    assert (false && "THE LINES BELOW NEED TO BE FIXED FOR BOOST 1.48 OR LATER BECAUSE replace_extension FUNCTION BEHAVES DIFFERENTLY");
#if BOOST_VERSION>=104800
    string new_extension = "." + (format("%|d|%|s|") % version_number % orig_extension).str();
#else
    string new_extension = (format("%|d|%|s|") % version_number % orig_extension).str();
#endif
    output_path = original_path;
    output_path.replace_extension( new_extension );
    metadata_path = output_path / "metadata.xml.bz2";
  }
  // ensure the output directory exists. create it if it does not.
  path unique_name( output_path );
  output_path.remove_filename();
  path verified_path = unique_name.root_directory();
  for( path::iterator i=unique_name.begin(), f=unique_name.end(); i!=f; ++i ) {
    if( (*i) == "." ) { continue; }
    if( (*i) == unique_name.root_directory() ) { continue; }
    // try to create the directory. if this fails, do not include this element in the path.
    // if the directory already exists, it will succeed.
    create_directory( verified_path / (*i) );
    if( exists( verified_path / (*i) ) ) {
      verified_path /= (*i);
    }
  }
  // ready to go
  return verified_path.string();
}

DEFINE_DGHEAP(AnaEventNtuple,"AnaEventNtuple");

// ================================================================================================================================================  

// // test program.
// //
// // compile like
// // g++ -g -I/usr/local/boost/include -L/usr/local/boost/lib -lboost_serialization -o AnaEventNtuple src/AnaEventNtuple.cpp dg/DgMemory.cpp

// DEFINE_DGHEAP(AnaEvent,"AnaEvent");

// AnaEventNtuple ntuple;
// const std::string directory_name = "./test_ntuple";

// AnaEventNtuple ntuple_read;

// #include <iostream>
// #include <boost/format.hpp>
// #include <iterator>
// #include <algorithm>

// void
// fill_ntuple()
// {
//   for( unsigned int i=0; i!=100; ++i ) {
//     AnaEvent* ev = new AnaEvent;
//     ev->_ets.clear();
//     for( unsigned int j=0; j!=i; ++j ) {
//       ev->_ets.push_back( j*((i%10)+1)*1. );
//     }
//     ev->_run_number = 1;
//     ev->_lumi_block = i % 10;
//     ev->_event_number = i;
//     ntuple.add_event( ev );
//     delete ev;
//   }
// }

// void
// read_ntuple()
// {
//   for( unsigned int ievent=0, fevent=ntuple_read.nevents(); ievent!=fevent; ++ievent ) {
//     const AnaEvent* ev = ntuple_read.get_event( ievent );
//     assert( ev );
//     cout << format( "ev %|5d| n %|5d| i %|5d| et %|5f| " ) 
//       % ievent % ev->_ets.size() % -1 % 0 << endl;
//     for( vector<double>::const_iterator i=ev->_ets.begin(), f=ev->_ets.end(); i!=f; ++i ) {
//       cout << format( "ev %|5d| n %|5d| i %|5d| et %|5f| " ) 
//         % ievent % ev->_ets.size() % std::distance(ev->_ets.begin(),i) % (*i) << endl;
//     }
//   }
// }

// int
// main( void )
// {

//   ntuple.open_sample( directory_name , AnaEventNtuple::WRITE );
//   fill_ntuple();
//   ntuple.close_sample();

//   DgMemoryManager::print( std::cout );

//   ntuple_read.open_sample( directory_name , AnaEventNtuple::READ );
//   read_ntuple();
//   ntuple_read.close_sample();

//   DgMemoryManager::print( std::cout );
//   DgMemoryManager::leak_check( std::cout );
  
// }
