
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

void
AnaEventNtuple::write_event_block( const FileIndex& index )
{
  // save the event block and update metadata.
  const std::string filename = filename_for_index( index );
  boost::shared_ptr<ana_streams::ostream> outf( ana_streams::open_for_write(filename) );
  if( !outf || !(*outf) ) { 
    cout << " WARNING: could not write event block for file " << filename << endl;
    cout << "          dropping " << _event_block.size() << " events." << endl;
    return; 
  }
  boost::shared_ptr<archive::xml_oarchive> aro( new archive::xml_oarchive(*outf) );
  try {
    // expicitly force registration of some types. not all types must
    // appear here, but the types and the order in which they appear
    // must be the save as in the save function.
    aro->register_type(static_cast<AnaEvent*>(0));
    aro->register_type(static_cast<AnaRoad*>(0));
    aro->register_type(static_cast<AnaSuperstrip*>(0));
    aro->register_type(static_cast<AnaTrack*>(0));
    aro->register_type(static_cast<AnaElectron*>(0));
    aro->register_type(static_cast<AnaPhoton*>(0));
    aro->register_type(static_cast<AnaMuon*>(0));
    aro->register_type(static_cast<AnaJet*>(0));
    aro->register_type(static_cast<AnaMET*>(0));
    aro->register_type(static_cast<AnaTrigger*>(0));
    aro->register_type(static_cast<AnaSiChannel*>(0));
    aro->register_type(static_cast<AnaSiCluster*>(0));
    aro->register_type(static_cast<AnaRoad*>(0));
    aro->register_type(static_cast<AnaBeamline*>(0));
    aro->register_type(static_cast<AnaVertex*>(0));
    aro->register_type(static_cast<AnaTruthTable*>(0));
    aro->register_type(static_cast<AnaTruthParticle*>(0));
    // write the event block
    (*aro) << BOOST_SERIALIZATION_NVP(_event_block);
  } catch( archive::archive_exception& e ) {
    std::cout << e.code << std::endl;
    std::cout << e.what() << std::endl;
  }
  outf->flush();
  aro.reset(); // destroy the iarchive before the stream closes.
  outf.reset(); // inf passed by reference to text_oarchive; destroy after archive.
  // update file metadata
  assert( _file_map.find(index) == _file_map.end() );
  assert( _event_block.size() == _block_accepted );
  _file_map.insert( FileMap::value_type( index , FileMetadata(filename,_block_accepted,_block_total) ) );
  _block_accepted = 0;
  _block_total = 0;
  // update per event metadata
  if( _record_per_event_metadata ) {
    for( EventBlock::const_iterator i=_event_block.begin(), f=_event_block.end(); i!=f; ++i ) {
      const shared_ptr<AnaEvent>& p( *i );
      LumiBlockMap& bmap( _meta_map[ p->run_number() ] );
      EventMap& cmap( bmap[p->lumi_block()] );
      // ensure there are no duplicates
      //  EventMap::iterator ic = cmap.find( p->event_number() );
      //  assert( ic == cmap.end() );
      // allow duplicates
      cmap.insert( EventMap::value_type( p->event_number() ,
                                         EventMetadata( p->run_number() , p->lumi_block() , 
                                                        p->event_number() , index ) ) );
    }
  }
}
