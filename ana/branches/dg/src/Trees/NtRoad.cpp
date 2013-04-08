#include "TrigFTKAna/Trees/NtRoad.hpp"
#ifdef HAVE_LIBFTKSIM

#include <string>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <set>
#include <cmath>
#include <boost/shared_ptr.hpp>
#include <boost/foreach.hpp>
#include <TChain.h>
#include <TFile.h>
#include "TrigFTKSim/FTKRoadStream.h"
#include "TrigFTKSim/MultiTruth.h"
#include "TrigFTKSim/FTKRoad.h"
#include "TrigFTKSim/FTKSS.h"
#include "TrigFTKAna/AnaConfiguration.hpp"
#include "TrigFTKAna/AnaRoad.hpp"
#include "TrigFTKAna/AnaTrack.hpp"
#include "TrigFTKAna/AnaSuperstrip.hpp"
#include "TrigFTKAna/ana_streams_helper.hpp"

using namespace std;

const bool
NtRoad::begin_file( const int& bank , const std::string& filename )
{
  if( !_chain ) { 
    _chain.reset( new TChain("ftkdata") ); 
    _chain->SetDirectory( 0 );
  }
  if( !_road_stream ) { 
    // abuse shared_ptr so that ROOT SetBranchAddress behaves correctly.
    _road_stream_ptr = new FTKRoadStream;
    _road_stream.reset( _road_stream_ptr ); 
  }

  _bank = bank;
  _chain->Reset();
  _chain->AddFile( ana_streams::root_dcap_filename(filename).c_str() );
  _n_events = _chain->GetEntries();

  stringstream bankstream;
  bankstream << "FTKBank" << _bank << ".";
  stringstream bankbranch;
  bankbranch << bankstream.str() << "*";

  _chain->SetBranchStatus( "*" , 0 );
  _chain->SetBranchStatus( bankbranch.str().c_str() , 1 );
  _chain->SetBranchAddress( bankstream.str().c_str() , &_road_stream_ptr );

  _n_roads = 0ul;
  _n_superstrips = 0ul;

  _open_filename = ana_streams::root_dcap_filename(filename);
  return true;
}

const bool
NtRoad::end_file()
{
  _open_filename = string();
  return true;
}

const bool
NtRoad::get_event( const unsigned long& ievent )
{
  if( !_chain ) { return false; }
  if( _n_events == 0ul ) { return false; }
  if( ievent>=_n_events ) { return false; }

  _chain->GetEntry( ievent );
  clear();

  _run_number = _road_stream->runNumber();
  _event_number = _road_stream->eventNumber();

  // convert road stream to internal representations
  _n_planes = _road_stream->getNPlanes();
  // convert roads
  _n_roads = _road_stream->prepareFitList();

  // Get cluster information
  for(unsigned int ipl = 0; ipl < _n_planes; ipl++) {
    _n_clusters.push_back(_road_stream->naoGetNclus(ipl));
  }

  for( int am_roads = 0; am_roads <= 1; am_roads++ ) {
    if( am_roads && _n_roads && 
	_n_roads == _road_stream->getNRoads() ) {
      // No need to loop over AM roads if we've already done them
      _road_stream->rewindFitList();
      FTKRoad* testroad = _road_stream->fetchRoad();
      assert(testroad);
      if( testroad == testroad->getAMRoad() ) continue;
    }
    map< vector<int> , boost::shared_ptr<AnaRoad> > uniqueRoads; // Only used if AnaConfiguration::combine_roads() == true
    for( unsigned long iroad=0; iroad!=(am_roads?_road_stream->getNRoads():_n_roads); ++iroad ) {
      const FTKRoad* road( (am_roads?_road_stream->getRoad(iroad):_road_stream->fetchRoad()) ); assert( road );
      if( am_roads ) _road_stream->attachHits(iroad);
      boost::shared_ptr<AnaRoad> tmp;
      
      vector<int> ssids; // Only used if AnaConfiguration::combine_roads() == true
      if( AnaConfiguration::combine_roads() ) {
	for( int i = 0; i < _n_planes; i++ ) {
	  ssids.push_back(road->getSSID(i));
	}
      }
      
      if( !(AnaConfiguration::combine_roads()) || uniqueRoads.count(ssids) == 0 ) {
	// Completely new road:
	tmp.reset(new AnaRoad);
	tmp->_road_id = road->getRoadID();//iroad; // == m_roadID?
	tmp->_bank_id = road->getBankID();
	tmp->_pattern_id = road->getPatternID();
	tmp->_sector_id = road->getSectorID();
	tmp->_n_planes = road->getNPlanes();
	tmp->_rw_rejected = road->getRWRejected();
	tmp->_rw_bank_id = road->getRWBank();
	tmp->_rw_road_id = road->getRWRoad();
	tmp->_n_hits = road->getNHits();
	tmp->_bitmask = road->getBitMask();
	// Turn to 11L bitmask if not option A
	if( road->getNPlanes() == 8 ) { // SCT-only
	  tmp->_bitmask = ((tmp->_bitmask)<<3);
	}
	else if( road->getNPlanes() == 4 ) { // 3 pix + 1 SCTtrk
	  tmp->_bitmask = ((tmp->_bitmask)&0x7);
	}
	tmp->_superstrip_id.reserve( tmp->_n_planes );
	tmp->_coords.reserve( tmp->_n_planes );
	tmp->_m_hits_mask.resize( tmp->_n_planes );
	for( unsigned int iplane=0; iplane!=tmp->_n_planes; ++iplane ) {
	  tmp->_superstrip_id.push_back( road->getAMRoad()->getSSID(iplane) ); // Save the SSIDs at AM level
	  vector< vector<AnaRoad::ftype> > coords_for_plane;
	  AnaRoad::TruthVector truth_for_plane;
	  if( road->getNHits(iplane)!=0 ) { 
	    if( !road->getHitsMask().empty() ) {
	      int nHitBits(road->getHitsMaskByPlane(iplane).GetNbits() );
	      tmp->_m_hits_mask[iplane].resize( nHitBits );
	      for( int ibit = 0; ibit < nHitBits; ibit++ ) {
		tmp->_m_hits_mask[iplane][ibit] = road->hit_failed_HF(iplane,ibit);
	      }
	    }
	    else {
	      tmp->_m_hits_mask[iplane].resize( road->getNHits(iplane), 0 );
	    }
	    for( vector<FTKHit>::const_iterator ihit=road->getHits(static_cast<int>(iplane)).begin(), fhit=road->getHits(static_cast<int>(iplane)).end(); ihit!=fhit; ++ihit ) {
	      const FTKCoord& coord( (*ihit).getCoord() );
	      vector<AnaRoad::ftype> these_coords;
	      for( unsigned int idim=0, fdim=coord.getDim(); idim!=fdim; ++idim ) {
		these_coords.push_back( coord[idim] );
	      }
	      coords_for_plane.push_back( these_coords );
	      // Get truth info for the hit
	      truth_for_plane.push_back( (*ihit).getTruth() );
	    }
	  }
	  tmp->_coords.push_back( coords_for_plane );
	  tmp->_truth.push_back( truth_for_plane );
	}
	if( am_roads ) { 
	  _am_roads.push_back( tmp );
	  _am_road_lookup[ tmp->bank_id() ][ tmp->road_id() ] = tmp;
	}
	else {
	  _roads.push_back( tmp );
	  _road_lookup[ tmp->bank_id() ][ tmp->road_id() ] = tmp;
	}
	if( AnaConfiguration::combine_roads() ) {
	  uniqueRoads[ssids] = tmp;
	}
      }
      else {
	assert( AnaConfiguration::combine_roads() && !am_roads );
	// Duplicate road, probably with a different hit mask
	// This breaks the rw_* variables, and changes n_hits, bitmask,
	// coords, hitsmask, truth
	tmp = uniqueRoads[ssids];
	// OR the bitmasks together
	tmp->_bitmask |= road->getBitMask();
	// Get the number of hits now based on the bitmask
	tmp->_n_hits = 0;
	for( int ibit = 0; ibit < tmp->_n_planes; ibit++ )
	  if( ((tmp->_bitmask>>ibit) % 2) ) tmp->_n_hits++;
	
	// Assign coordinates and set the hit mask
	for( unsigned int iplane=0; iplane!=tmp->_n_planes; ++iplane ) {
	  assert( tmp->superstrip_id(iplane) == road->getSSID(iplane) );
	  int nHitBits(road->getHitsMaskByPlane(iplane).GetNbits() );
	  // If _m_hits_mask[iplane].size() is zero, then initialize it from road
	  if( tmp->_m_hits_mask[iplane].size() == 0 ) {
	    tmp->_m_hits_mask[iplane].resize( nHitBits );
	    for( int ibit = 0; ibit < nHitBits; ibit++ )
	      tmp->_m_hits_mask[iplane][ibit] = road->hit_failed_HF(iplane,ibit);
	  }
	  else {
	    // Else if _m_hits_mask[iplane].size() is smaller than road, expand it and combine
	    if( tmp->_m_hits_mask[iplane].size() < nHitBits )
	      tmp->_m_hits_mask[iplane].resize(nHitBits);
	    // Else just merge
	    for( int ibit = 0; ibit < nHitBits; ibit++ )
	      tmp->_m_hits_mask[iplane][ibit] &= road->hit_failed_HF(iplane,ibit);
	  }
	  
	  // Check to see if the coordinates are filled already
	  // Can be unfilled if previous roads didn't have hits in
	  // this plane or vice versa
	  if( tmp->_coords[iplane].size() != road->getNHits(iplane) ) {
	    assert( tmp->_coords[iplane].size() == 0 || 
		    road->getNHits(iplane) == 0 );
	    vector< vector<AnaRoad::ftype> > coords_for_plane;
	    AnaRoad::TruthVector truth_for_plane;
	    if( road->getNHits(iplane)!=0 ) { 
	      for( vector<FTKHit>::const_iterator ihit=road->getHits(static_cast<int>(iplane)).begin(), 
		     fhit=road->getHits(static_cast<int>(iplane)).end(); ihit!=fhit; ++ihit ) {
		const FTKCoord& coord( (*ihit).getCoord() );
		vector<AnaRoad::ftype> these_coords;
		for( unsigned int idim=0, fdim=coord.getDim(); idim!=fdim; ++idim ) {
		  these_coords.push_back( coord[idim] );
		}
		coords_for_plane.push_back( these_coords );
		truth_for_plane.push_back( (*ihit).getTruth() );
	      }
	    }
	    tmp->_coords[iplane] = coords_for_plane;
	    tmp->_truth[iplane] = truth_for_plane;
	  }
	}
      }
      if( am_roads ) _road_stream->detachHits(iroad);
    }  // end convert each FTKRoad to an AnaRoad
  }
  // convert superstrips
  _n_superstrips = 0ul;
  const FTKSS_container_t& ss_container = _road_stream->getSSContainer();
  for( unsigned long iplane=0; iplane!=_n_planes; ++iplane ) {
    unsigned long n_ss_plane = _road_stream->getNSS(static_cast<int>(iplane));
    _n_superstrips += n_ss_plane;
  }
  for( FTKSS_container_t::const_iterator iplane=ss_container.begin(), fplane=ss_container.end(); iplane!=fplane; ++iplane ) {
    const unsigned int nplane = std::distance(ss_container.begin(),iplane);
    for( FTKSS_map_t::const_iterator iss=(*iplane).begin(), fss=(*iplane).end(); iss!=fss; ++iss ) {
      const unsigned long nss = iss->first;
      const FTKSS& ss( iss->second );
      boost::shared_ptr<AnaSuperstrip> tmp( new AnaSuperstrip );
      tmp->_layer = nplane;
      tmp->_superstrip_id = nss;
      tmp->_coords.clear();
      for( int iss = -1; iss < (int)ss.getNSubStrips(); iss++ ) {
	// If iss is -1 then we're just doing the top-level SS
	AnaSuperstrip tmpss;
	if( iss >= 0 ) {
	  tmpss._layer = nplane;
	  tmpss._superstrip_id = nss; // Same ssid as parent
	  tmpss._coords.clear();
	}
	const FTKSS* ssptr( iss<0 ? &ss : &(ss.getSubSS(iss)) ); // const cast to work around non-const getCoords
	const vector<FTKHit>& hits( const_cast<FTKSS*>(ssptr)->getHits() );
	for( vector<FTKHit>::const_iterator ihit=hits.begin(), fhit=hits.end(); ihit!=fhit; ++ihit ) {
	  const FTKCoord& coord( (*ihit).getCoord() );
	  vector<AnaSuperstrip::ftype> these_coords;
	  for( unsigned int idim=0, fdim=coord.getDim(); idim!=fdim; ++idim ) {
	    these_coords.push_back( coord[idim] );
	  }
	  iss<0 ? tmp->_coords.push_back( these_coords ) : tmpss._coords.push_back( these_coords );
	}
	for( int itr=0; itr!=(ssptr)->getNHits(); ++itr ) {
	  iss<0 ? tmp->_truth.push_back(const_cast<FTKSS*>(ssptr)->getTruth(itr)) : tmpss._truth.push_back(const_cast<FTKSS*>(ssptr)->getTruth(itr));
	}
	if( iss >= 0 ) { tmp->_subss.push_back(tmpss); } 
      }
      _superstrips.push_back( tmp );    
      _ss_lookup[ tmp->layer() ][ tmp->id() ] = tmp;
    } // end for each ss in plane
  } // end for each plane convert each FTKSS to an AnaSuperstrip

  // cout << " FTK road lookup with " << _road_lookup.size() << " bank ids" << endl;
  // BOOST_FOREACH( const NtRoadBankLookup::value_type& bpair , _road_lookup ) { 
  //   cout << " bank " << bpair.first << " with " << bpair.second.size() << " entries " << endl;
  // }
  
  return true;
}

const unsigned long
NtRoad::count_events( const std::string& filename )
{
  TChain chain( "ftkdata" );
  chain.SetDirectory( 0 );
  chain.Reset();
  chain.AddFile( filename.c_str() );
  return( chain.GetEntries() );
}

class HitID {
public:
  bool pixel;
  int layer;
  unsigned int ssid;
  AnaSuperstrip::ftype coord_x;
  AnaSuperstrip::ftype coord_y;
  HitID() : pixel(false) , layer(-1) , ssid(0) , coord_x(9999.) , coord_y(9999.) {}
  HitID(const int& lyr,const unsigned int& ss,const AnaSuperstrip::ftype& x) : pixel(false) , layer(lyr) , ssid(ss) , coord_x(x) {}
  HitID(const int& lyr,const unsigned int& ss,const AnaSuperstrip::ftype& x,const AnaSuperstrip::ftype& y) : pixel(true) , layer(lyr) , ssid(ss) , coord_x(x) , coord_y(y) {}
  const bool operator=( const HitID& rhs ) const {
    if( layer!=rhs.layer ) { return false; }
    if( ssid!=rhs.ssid ) { return false; }
    if( pixel ) {
      return( coord_x==rhs.coord_x && coord_y==rhs.coord_y );
    } else {
      return( coord_x==rhs.coord_x );
    }
  }
};
struct LessThanHitID {
  const bool operator()( const HitID& a , const HitID& b ) const {
    if( a.layer!=b.layer ) { return( a.layer < b.layer ); }
    if( a.ssid!=b.ssid ) { return( a.ssid < b.ssid ); }
    if( a.pixel ) {
      if( a.coord_x != b.coord_x ) { return( a.coord_x < b.coord_x ); }
      if( a.coord_y != b.coord_y ) { return( a.coord_x < b.coord_y ); }
      return false;
    } else {
      if( a.coord_x != b.coord_x ) { return( a.coord_x < b.coord_x ); }
      return false;
    }
  }
};


void
NtRoad::fill_ftk_track_nshared( std::vector< boost::shared_ptr<const AnaTrack> >::const_iterator begin , 
                                std::vector< boost::shared_ptr<const AnaTrack> >::const_iterator end , 
                                const NtRoadBankLookup& roadlookup , const NtRoadLayerLookup& sslookup )
{
  typedef std::vector< boost::shared_ptr<const AnaTrack> >::const_iterator iterT;

  // cout << " FTK fill track nshared with road lookup size " << roadlookup.size() 
  //      << " and ss lookup size " << sslookup.size()
  //      << endl;

  // loop over all tracks and count how many times a given hit is
  // shared with other tracks. since each track retains the
  // coordinates of all hits, check these first. this is not
  // completely unambiguous, since hits on two different modules might
  // coincidentally have the same local coordinates. to eliminate that
  // possibilty, if coordinates match, undertake the slow process of
  // retrieving the layer and ssids for each hit, and see if these
  // match.

  for( iterT i=begin, f=end; i!=f; ++i ) {
    const boost::shared_ptr<const AnaTrack>& track( *i );
    track->_n_pixel_shared = 0;
    track->_n_sct_shared = 0;
  }

  for( iterT i=begin, f=end; i!=f; ++i ) {
    const boost::shared_ptr<const AnaTrack>& track( *i );
    for( iterT j=i, jf=end; j!=jf; ++j ) {
      if( j==i ) { continue; }
      const boost::shared_ptr<const AnaTrack>& other_track( *j );
      // loop over layers and test.
      for( int il=0, ilf=detector::ftk_nlayers(), ilayer=0; il!=ilf; ++il ) {
        if( !detector::ftkHasPlane(track->ftk_bitmask(),il) ) { continue; }
        if( !detector::ftkHasPlane(other_track->ftk_bitmask(),il) ) { continue; }
        const bool is_pixel_layer = il<detector::ftk_nlayers_pixels();
        const int ixlayer = static_cast<int>(detector::ftk_1st_coordinate_for_layer(il));
        const int iylayer = is_pixel_layer ? static_cast<int>(detector::ftk_2nd_coordinate_for_layer(il)) : -1;
        AnaTrack::ftype hitfx = track->ftk_coords()[ixlayer];
        AnaTrack::ftype other_hitfx = other_track->ftk_coords()[ixlayer];
        if( std::abs(hitfx-other_hitfx)>0.0001 ) { continue; }
        if( is_pixel_layer ) { 
          AnaTrack::ftype hitfy = ( is_pixel_layer ? track->ftk_coords()[iylayer] : -9999. );
          AnaTrack::ftype other_hitfy = ( is_pixel_layer ? other_track->ftk_coords()[iylayer] : -9999. );
          if( std::abs(hitfy-other_hitfy)>0.0001 ) { continue; }
          //if( hitfy!=other_hitfy ) { continue; }
        }
        // clusters have same coordinate. now we have to go to the superstrip id. this is slow,
        // but by now we should have killed most of the combinatorics.
        const unsigned int bank_id = track->ftk_bank_id();
        const unsigned long road_id = track->ftk_road_id();
        const boost::shared_ptr<const AnaRoad>& road( road_lookup( roadlookup , bank_id , road_id ) );
        //assert( road );
        if( !road ) { continue; }
        //assert( road );
        const unsigned int other_bank_id = other_track->ftk_bank_id();
        const unsigned long other_road_id = other_track->ftk_road_id();
        const boost::shared_ptr<const AnaRoad>& other_road( road_lookup( roadlookup , other_bank_id , other_road_id ) );
        //assert( other_road );
        if( !other_road ) continue;
        int ssid = road->superstrip_ids()[il];
        int other_ssid = other_road->superstrip_ids()[il];
        // const boost::shared_ptr<const AnaSuperstrip>& ss = ss_lookup( sslookup , i , ssid );
        // if( !ss ) { continue; }
        // const boost::shared_ptr<const AnaSuperstrip>& other_ss = ss_lookup( sslookup , i , other_ssid );
        if( ssid!=other_ssid ) { continue; }
        // hits match.
        if( is_pixel_layer ) { 
          ++track->_n_pixel_shared;
          ++other_track->_n_pixel_shared;
        } else { 
          ++track->_n_sct_shared;
          ++other_track->_n_sct_shared;
        }
      }
    }
  }
}


// void
// NtRoad::fill_ftk_track_nshared( std::vector< boost::shared_ptr<const AnaTrack> >::const_iterator begin , std::vector< boost::shared_ptr<const AnaTrack> >::const_iterator end , 
//                                 const NtRoadBankLookup& roadlookup , const NtRoadLayerLookup& sslookup )
// {
//   typedef std::vector< boost::shared_ptr<const AnaTrack> >::const_iterator iterT;
//   // build list hits on each track. hits are essentially a unique
//   // combination of layer, superstrip id, and coordinate.

//   typedef vector<HitID> HitsOnTrack;
//   typedef map< boost::shared_ptr<const AnaTrack> ,HitsOnTrack> HitMap;
//   typedef multiset<HitID,LessThanHitID> HitSet;
//   HitMap hitmap;
//   HitSet hitset;

//   for( iterT i=begin, f=end; i!=f; ++i ) {
//     const boost::shared_ptr<const AnaTrack>& track( *i );
//     const unsigned int bank_id = track->ftk_bank_id();
//     const unsigned long road_id = track->ftk_road_id();
//     const boost::shared_ptr<const AnaRoad>& road( road_lookup( roadlookup , bank_id , road_id ) );
//     if( !road ) { continue; }
//     for( int i=0, f=detector::ftk_nlayers(), ilayer=0; i!=f; ++i ) {
//       if( !detector::ftkHasPlane(track->ftk_bitmask(),i) ) { continue; }
//       unsigned int ssid = road->superstrip_ids()[i];
//       const boost::shared_ptr<const AnaSuperstrip>& ss = ss_lookup( sslookup , i , ssid );
//       if( !ss ) { continue; }
//       const bool is_pixel_layer = i<detector::ftk_nlayers_pixels();
//       AnaTrack::ftype hitfx = track->ftk_coords()[detector::ftk_1st_coordinate_for_layer(i)];
//       AnaTrack::ftype hitfy = ( is_pixel_layer ? 
//                                 track->ftk_coords()[detector::ftk_2nd_coordinate_for_layer(i)] :
//                                 -9999. );
      
//       for( AnaSuperstrip::HitVector::const_iterator ihit=ss->begin_hits(), fhit=ss->end_hits(); ihit!=fhit; ++ihit ) {
//         const AnaSuperstrip::Hit& hit(*ihit);
//         if( is_pixel_layer ) {
//           hitmap[ track ].push_back( HitID(i,ssid,hit.front(),hit.back()) );
//           hitset.insert( HitID(i,ssid,hit.front(),hit.back()) );
//         } else {
//           hitmap[ track ].push_back( HitID(false,i,ssid,hit.front()) );
//           hitset.insert( HitID(i,ssid,hit.front()) );
//         }
//       }
//     } // end for each layer
//   } // end for each track

//   // loop over each track and count number of shared hits.
//   for( iterT i=begin, f=end; i!=f; ++i ) {
//     const boost::shared_ptr<const AnaTrack>& track( *i );
//     unsigned int nshared_pixel = 0u;
//     unsigned int nshared_sct = 0u;
//     const unsigned int bank_id = track->ftk_bank_id();
//     const unsigned long road_id = track->ftk_road_id();
//     const boost::shared_ptr<const AnaRoad>& road( road_lookup( roadlookup , bank_id , road_id ) );
//     if( !road ) { continue; }
//     for( int i=0, f=detector::ftk_nlayers(), ilayer=0; i!=f; ++i ) {
//       if( !detector::ftkHasPlane(track->ftk_bitmask(),i) ) { continue; }
//       unsigned int ssid = road->superstrip_ids()[i];
//       const boost::shared_ptr<const AnaSuperstrip>& ss = ss_lookup( sslookup , i , ssid );
//       if( !ss ) { continue; }
//       const bool is_pixel_layer = i<detector::ftk_nlayers_pixels();
//       AnaTrack::ftype hitfx = track->ftk_coords()[detector::ftk_1st_coordinate_for_layer(i)];
//       AnaTrack::ftype hitfy = ( is_pixel_layer ? 
//                                 track->ftk_coords()[detector::ftk_2nd_coordinate_for_layer(i)] :
//                                 -9999. );
      
//       for( AnaSuperstrip::HitVector::const_iterator ihit=ss->begin_hits(), fhit=ss->end_hits(); ihit!=fhit; ++ihit ) {
//         const AnaSuperstrip::Hit& hit(*ihit);
//         if( is_pixel_layer ) {
//           HitID id(i,ssid,hit.front(),hit.back());
//           unsigned int c = hitset.count( id );
//           if( c>1 ) { ++nshared_pixel; }
//         } else {
//           HitID id(i,ssid,hit.front());
//           unsigned int c = hitset.count( id );
//           if( c>1 ) { ++nshared_sct; }
//         }
//       }
//     } // end for each layer
//     track->_n_pixel_shared = nshared_pixel;
//     track->_n_sct_shared = nshared_sct;
//   } // end for each track
  
// }

#else // else if not HAVE_LIBFTKSIM

#include <iostream>
#include "TrigFTKAna/Trees/NtRoad.hpp"

using namespace std;

const bool
NtRoad::begin_file( const int& bank , const std::string& filename )
{
  cout << " support for FTK roads ntuple not compiled." << endl;
  return false;
}

const bool
NtRoad::end_file()
{ 
  return false;
}

const bool
NtRoad::get_event( const unsigned long& ievent )
{
  return false;
}

const unsigned long
NtRoad::count_events( const std::string& filename )
{
  cout << " support for FTK roads ntuple not compiled." << endl;
  return 0;
}

void
NtRoad::fill_ftk_track_nshared( std::vector< boost::shared_ptr<const AnaTrack> >::const_iterator begin , 
                                std::vector< boost::shared_ptr<const AnaTrack> >::const_iterator end , 
                                const NtRoadBankLookup& roadlookup , const NtRoadLayerLookup& sslookup )
{
  return;
}

#endif // if else HAVE_LIBFTKSIM
