
#include <string>
#include <algorithm>
#include <iterator>
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
#include "TrigFTKAna/AnaRoad.hpp"
#include "TrigFTKAna/AnaSiCluster.hpp"
#include "TrigFTKAna/AnaTruthParticle.hpp"
#include "TrigFTKAna/SiModuleList.hpp"
#include "TrigFTKAna/AnaEventNtuple.hpp"
#include "TrigFTKAna/ana_streams_helper.hpp"
#include "TrigFTKAna/ana_streams.hpp"

using namespace std;
using namespace boost;
using namespace boost::posix_time;
using namespace boost::filesystem;
using namespace boost::algorithm;

const bool
AnaEventNtuple::read_event_block( const FileIndex& index )
{
  // load a new event block
  _event_block.clear();
  _current_event_block_index = std::numeric_limits<FileIndex>::max();
  _current_event_offset = 0ul;
  // look up filename from index
  FileMap::const_iterator ifile = _file_map.find( index );
  if( ifile==_file_map.end() ) { return false; }
  const FileMetadata& meta( ifile->second );
  const std::string filename = meta.filename();
  boost::shared_ptr<ana_streams::istream> inf( ana_streams::open_for_read(filename) );
  if( !inf || !(*inf) ) { return false; }
  // print stream contents
  // while( !inf->eof() ) {
  //   string contents;
  //   getline( *inf , contents );
  //   cout << contents << endl;
  // }
  // return false;
  boost::shared_ptr<archive::xml_iarchive> ari( new archive::xml_iarchive(*inf) );
  try {
    // expicitly force registration of some types. not all types must
    // appear here, but the types and the order in which they appear
    // must be the save as in the save function.
    ari->register_type(static_cast<AnaEvent*>(0));
    ari->register_type(static_cast<AnaRoad*>(0));
    ari->register_type(static_cast<AnaSuperstrip*>(0));
    ari->register_type(static_cast<AnaTrack*>(0));
    ari->register_type(static_cast<AnaElectron*>(0));
    ari->register_type(static_cast<AnaPhoton*>(0));
    ari->register_type(static_cast<AnaMuon*>(0));
    ari->register_type(static_cast<AnaJet*>(0));
    ari->register_type(static_cast<AnaMET*>(0));
    ari->register_type(static_cast<AnaTrigger*>(0));
    ari->register_type(static_cast<AnaSiChannel*>(0));
    ari->register_type(static_cast<AnaSiCluster*>(0));
    ari->register_type(static_cast<AnaRoad*>(0));
    ari->register_type(static_cast<AnaBeamline*>(0));
    ari->register_type(static_cast<AnaVertex*>(0));
    ari->register_type(static_cast<AnaTruthTable*>(0));
    ari->register_type(static_cast<AnaTruthParticle*>(0));
    // read
    (*ari) >> BOOST_SERIALIZATION_NVP(_event_block);
  } catch( archive::archive_exception& e ) {
    std::cout << e.code << std::endl;
    std::cout << e.what() << std::endl;
  }
  ari.reset(); // destroy the iarchive before the stream closes.
  inf.reset(); // inf passed by reference to text_iarchive; it must be destroyed after archive.
  // validate event counts against metadata
  if( _event_block.size() != meta.nevents_accepted() ) {
    cout << "warning: number of events in " << filename << " does not match metadata." << endl;
    cout << "         found " << _event_block.size() << ", expected " << meta.nevents_accepted() << endl;
  }
  _current_event_block_index = index;
  return true;
}
