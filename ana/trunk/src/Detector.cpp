
#include <cassert>
#include <cmath>
#include <vector>
#include <bitset>
#include <algorithm>
#include <boost/foreach.hpp>
#include <boost/tuple/tuple.hpp>
#include "TrigFTKAna/Detector.hpp"
#include "TrigFTKAna/AnaTrack.hpp"

using namespace std;

// return track_position.phi() at track_position.mag() == r.
template<typename floatT, typename trackPointerT>
const bool
detector::phi_position_at_r( trackPointerT const track , const floatT& r_in_mm , floatT& phi )
{
  assert( track );
  assert( r_in_mm>=0. );
  if( std::abs(r_in_mm)<1E-6 ) { return false; } // phi is undefined at the coordinate origin.
  if( r_in_mm < std::abs(track->d0()) ) { return false; } // never reaches this radius
  // helix radius in solenoidal field
  const floatT rho_in_mm = std::abs( track->pt() / (curvature_constant_in_gev_mm_tesla<floatT>()*solenoidal_field_in_tesla<floatT>()) );
  //
  if( rho_in_mm > 1E8 ) { 
    // helix ~= line on the scale of the ATLAS detector.
    phi = track->phi0();
    return true;
  }
  // curvature is in 1./MeV. convert to 1/mm.
  const floatT curv_in_mm = std::abs( detector::safe_divide(1.,2.*rho_in_mm) ) * detector::sign_of(track->curvature());
  if( r_in_mm > std::abs(track->d0() + detector::safe_divide(static_cast<floatT>(1),curv_in_mm)) ) { return false; } // track never reaches this radius
  const floatT den = 1.+(2.*curv_in_mm*track->d0());
  if( std::abs(den)<1E-6 ) { 
    // q rho == -d0. track circles around the coordinate origin with radius d0.
    // in this case, phi is not uniquely determined by the r_in_mm.
    return false;
  }
  floatT sindphi( ( (curv_in_mm*r_in_mm) + (1.+(curv_in_mm*track->d0()))*track->d0()/r_in_mm )/den );
  if( std::abs(sindphi) > 1. ) {
    // sin(dphi) = +/-1 corresponds to r=d0 or r=d0+2rho, the minimum
    // or maximum distance between the track and the coordinate
    // origin.  larger absolute values shouldn't happen except by numerical
    // mistake.
    assert( std::abs(sindphi) < 2. );
    // correct numerical error.
    if( sindphi < -1. ) { sindphi = -1.; }
    if( sindphi > 1. ) { sindphi = 1.; }
  }
  phi = std::asin(sindphi) + track->phi0();
  return true;
}

// return track_position.z() at track_position.mag() == r.
// note that z should be multi-valued at the minimum (|r|=|d0|) and maximum radii 
// from the z axis. in this case, the single z returned is the one closest
// to track->z0().
template<typename floatT,typename trackPointerT>
const bool
detector::z_at_r( trackPointerT const track , const floatT& r_in_mm , floatT& z_in_mm )
{
  assert( track );
  // helix radius in solenoidal field
  const floatT rho_in_mm = std::abs( track->pt() / (curvature_constant_in_gev_mm_tesla<floatT>()*solenoidal_field_in_tesla<floatT>()) );
  if( rho_in_mm > 1E8 ) { 
    // helix ~= line on the scale of the ATLAS detector.
    const floatT dr = std::sqrt( pow<2>(r_in_mm) - pow<2>(track->d0()) );
    z_in_mm = ( track->z0() + track->ct()*dr );
    return true;
  }
  // convert curvature from 1/MeV to 1/mm
  const floatT curv_in_mm = std::abs( detector::safe_divide(1.,2.*rho_in_mm) ) * detector::sign_of(track->curvature());
  if( std::abs(2.*curv_in_mm*track->d0()+1) < std::numeric_limits<floatT>::min() ) {
    // q rho == -d0. track circles around the coordinate origin with radius d0.
    // in this case, z is not uniquely determined by the r_in_mm.
    return false;
  }
  floatT tmp = curv_in_mm * std::sqrt( (pow<2>(r_in_mm)-pow<2>(track->d0())) / (1+2.*curv_in_mm*track->d0()) );
  // sin(dphi) = +/-1 corresponds to r=d0 or r=d0+2rho, the minimum
  // or maximum distance between the track and the coordinate
  // origin.  larger absolute values shouldn't happen except by numerical
  // mistake. as noted above, z is multi-valued at these points, so we
  // return the one closest to track->z0().
  tmp = std::max(static_cast<floatT>(-1),tmp);
  tmp = std::min(static_cast<floatT>(1),tmp);
  tmp = std::asin(tmp);
  z_in_mm = track->z0()+track->ct()*tmp/curv_in_mm;
  return true;
}

// compute track position.mag() at position.z().
template<typename floatT,typename trackPointerT>
const bool
detector::r_at_z( trackPointerT const track , const floatT& z_in_mm , floatT& r_in_mm )
{
  assert( track );
  if( std::abs(track->ct()) < std::numeric_limits<floatT>::min() ) { return track->z0(); }
  const floatT rho_in_mm = std::abs( track->pt() / (curvature_constant_in_gev_mm_tesla<floatT>()*solenoidal_field_in_tesla<floatT>()) );
  const floatT curv_in_mm = std::abs( detector::safe_divide(1.,2.*rho_in_mm) ) * detector::sign_of(track->curvature());
  if( rho_in_mm > 1E8 ) { 
    // helix ~= line on the scale of the ATLAS detector.
    r_in_mm = std::sqrt( pow<2>(track->d0()) + safe_divide(pow<2>(z_in_mm-track->z0()),pow<2>(track->ct())) );
    return true;
  }
  floatT tmp = (z_in_mm-track->z0())*curv_in_mm/track->ct();
  tmp = pow<2>( std::sin(tmp) ) * (1.+2.*curv_in_mm*track->d0());
  tmp /= pow<2>(curv_in_mm);
  r_in_mm = std::sqrt( pow<2>(track->d0()) + tmp );
  return true;
}


// return track_position.phi() at track_position.z().
template<typename floatT,typename trackPointerT>
const bool
detector::phi_position_at_z( trackPointerT const track , const floatT& z_in_mm , floatT& phi )
{
  assert( track );
  if( std::abs(track->ct()) < std::numeric_limits<floatT>::min() ) { 
    // track travels in +/- z direction. phi cannot be uniquely
    // determined from z.
    return false;
  }
  // helix radius in solenoidal field
  const floatT rho_in_mm = std::abs( track->pt() / (curvature_constant_in_gev_mm_tesla<floatT>()*solenoidal_field_in_tesla<floatT>()) );
  //
  if( rho_in_mm > 1E8 ) {
    // track is a straight line on the scale of the ATLAS detector.
    phi = track->phi0();
    return true;
  }
  // curvature is in 1./MeV. convert to 1/mm.
  const floatT curv_in_mm = std::abs( detector::safe_divide(1.,2.*rho_in_mm) ) * detector::sign_of(track->curvature());
  // compute track radius at z_in_mm.
  const floatT tmpA = pow<2>( std::sin( (z_in_mm-track->z0())*curv_in_mm/track->ct() ) );
  const floatT tmpB = safe_divide( (1+2*curv_in_mm*track->d0())*tmpA , pow<2>(curv_in_mm) );
  const floatT r_in_mm = std::sqrt( pow<2>(track->d0()) + tmpB );
  // retrieve phi position at r_in_mm
  phi = phi_position_at_r(track,r_in_mm,phi);
  return true;
}

// compute phi,z values of any intersections silicon barrels
template<typename floatT,typename trackPointerT>
const std::vector< boost::tuples::tuple<floatT,floatT> >
detector::intersections_with_silicon_barrels( trackPointerT const track )
{
  std::vector< boost::tuples::tuple<floatT,floatT> > result;
  vector<floatT> phi_at_intersecting_layer;
  for( unsigned int i=0u, f=detector::silicon_barrel_radii<floatT>().size(); i!=f; ++i ) {
    const floatT r_in_mm( silicon_barrel_radii<floatT>()[i] );
    const floatT min_z_in_mm( silicon_barrel_z_range<floatT>()[i].first );
    const floatT max_z_in_mm( silicon_barrel_z_range<floatT>()[i].second );
    floatT z_at_r_in_mm;
    const bool ok_z = z_at_r(track,r_in_mm,z_at_r_in_mm);
    if( !ok_z ) { continue; }
    if( z_at_r_in_mm<min_z_in_mm || z_at_r_in_mm>max_z_in_mm ) { continue; }
    floatT phi_at_r; 
    const bool ok_phi = phi_position_at_r(track,r_in_mm,phi_at_r);
    if( !ok_phi ) { continue; }
    result.push_back( boost::tuples::make_tuple(phi_at_r,z_at_r_in_mm) );
  }
  return result;
}

// compute phi,z values of any intersections silicon disks
template<typename floatT,typename trackPointerT>
const std::vector< boost::tuples::tuple<floatT,floatT> > 
detector::intersections_with_silicon_disks( trackPointerT const track )
{
  assert( track );
  std::vector< boost::tuples::tuple<floatT,floatT> > result;
  // determine phi position at each silicon barrel intersection
  vector<floatT> phi_at_intersecting_layer;
  for( unsigned int i=0u, f=detector::silicon_disk_z_positions<floatT>().size(); i!=f; ++i ) {
    const floatT z_in_mm( silicon_disk_z_positions<floatT>()[i] );
    const floatT min_r_in_mm( silicon_disk_radii_range<floatT>()[i].first );
    const floatT max_r_in_mm( silicon_disk_radii_range<floatT>()[i].second );
    floatT r_at_z_in_mm;
    const bool ok_r = r_at_z(track,z_in_mm,r_at_z_in_mm);
    if( !ok_r ) { continue; }
    if( r_at_z_in_mm<min_r_in_mm || r_at_z_in_mm>max_r_in_mm ) { continue; }
    floatT phi_at_z; 
    const bool ok_phi = phi_position_at_z(track,z_in_mm,phi_at_z);
    if( !ok_phi ) { continue; }
    result.push_back( boost::tuples::make_tuple(phi_at_z,z_in_mm) );
  }
  return result;
}

template<typename trackPointerT>
const std::bitset<8>
detector::ftkTestIntersectionCountInRegions( trackPointerT const track , const unsigned int& nlayers )
{
  assert( track );
  // determine phi position at each silicon barrel intersection
  typedef boost::tuples::tuple<AnaTrack::ftype,AnaTrack::ftype> IntersectionType;
  std::vector<IntersectionType> intersections_barrel( intersections_with_silicon_barrels<AnaTrack::ftype>(track) );
  std::vector<IntersectionType> intersections_disks( intersections_with_silicon_disks<AnaTrack::ftype>(track) );
  std::vector<IntersectionType> intersections( intersections_barrel );
  intersections.insert( intersections.end() , intersections_disks.begin() , intersections_disks.end() );
  return ftkTestIntersectionsCountInRegions( intersections , nlayers );
}

template<typename floatT>
const std::bitset<8>
detector::ftkTestIntersectionsCountInRegions( const std::vector< boost::tuples::tuple<floatT,floatT> >& intersections , 
                                              const unsigned int& nlayers )
{
  typedef boost::tuples::tuple<floatT,floatT> IntersectionType;
  // compare phi at each intersecting layer with the definition of each region.
  vector<unsigned int> hits_per_region;
  std::fill_n( back_inserter(hits_per_region) , 8 , 0u );
  for( typename vector<IntersectionType>::const_iterator i=intersections.begin(), f=intersections.end(); i!=f; ++i ) {
    const IntersectionType& intersection( *i );
    const floatT phi_at_layer = boost::tuples::get<0>(intersection);
    //const floatT phi_at_layer = i->get<0>();
    bitset<8> hit_regions( ftkPhiInRegion(phi_at_layer) );
    if( !hit_regions.any() ) { continue; }
    for( unsigned int i=0; i!=8; ++i ) { if(hit_regions.test(i)) ++hits_per_region[i]; }
  }
  bitset<8> result;
  for( unsigned int i=0; i!=8; ++i ) { if( hits_per_region[i]>=nlayers ) result.set(i); }
  return result;
}

template<typename floatT> 
const std::bitset<8>
detector::ftkPhiInRegion( const floatT& phi )
{
  bitset<8> result;
  for( unsigned int ibank=0; ibank!=8; ++ibank ) {
    //const floatT center_phi_region = (ibank*M_PI/2) + (ibank<4 ? (M_PI/4) : 0);
    const floatT center_phi_region = ibank*M_PI/static_cast<floatT>(4);
    const floatT absdphi = std::abs(delta_phi(phi,center_phi_region));
    if( absdphi < (static_cast<floatT>(M_PI)/static_cast<floatT>(4)) ) { result.set(ibank); }
  }
  return result;
}


template<typename floatT> 
const std::vector<floatT>&
detector::silicon_barrel_radii()
{
  // geometry taken from ATLAS CSC book, p17, Figure 2.
  static vector<floatT> result_in_mm;
  if( result_in_mm.empty() ) {
    result_in_mm.reserve( 7 );
    result_in_mm.push_back( 50.5 );
    result_in_mm.push_back( 88.5 );
    result_in_mm.push_back( 122.5 );
    result_in_mm.push_back( 299. );
    result_in_mm.push_back( 371. );
    result_in_mm.push_back( 443. );
    result_in_mm.push_back( 514. );
  }
  return result_in_mm;
}

template<typename floatT>
const std::vector< std::pair<floatT,floatT> >&
detector::silicon_barrel_z_range()
{
  // geometry taken from ATLAS CSC book, p17, Figure 2.
  static vector< pair<floatT,floatT> > result_in_mm;
  if( result_in_mm.empty() ) {
    std::fill_n( back_inserter(result_in_mm) , 3 , pair<floatT,floatT>(-400.5,400.5) );
    std::fill_n( back_inserter(result_in_mm) , 4 , pair<floatT,floatT>(-749,749) );
  }
  return result_in_mm;
}

template<typename floatT>
const std::vector<floatT>&
detector::silicon_disk_z_positions()
{
  // geometry taken from ATLAS CSC book, p17, Figure 2.
  static vector<floatT> result_in_mm;
  if( result_in_mm.empty() ) {
    result_in_mm.reserve( 12 );
    result_in_mm.push_back( 495. );
    result_in_mm.push_back( 580. );
    result_in_mm.push_back( 650. );
    result_in_mm.push_back( 853.8 );
    result_in_mm.push_back( 934. );
    result_in_mm.push_back( 1091.5 );
    result_in_mm.push_back( 1299.9 );
    result_in_mm.push_back( 1399.7 );
    result_in_mm.push_back( 1771.4 );
    result_in_mm.push_back( 2115.2 );
    result_in_mm.push_back( 2505 );
    result_in_mm.push_back( 2720.2 );
  }
  return result_in_mm;
}

template<typename floatT>
const std::vector< std::pair<floatT,floatT> >&
detector::silicon_disk_radii_range()
{
  // geometry taken from ATLAS CSC book, p17, Figure 2.
  static vector< pair<floatT,floatT> > result_in_mm;
  if( result_in_mm.empty() ) {
    std::fill_n( back_inserter(result_in_mm) , 1 , pair<floatT,floatT>(-334,334) ); // eyeballed estimate; not specified in Figure 2 or the accompanying table.
    std::fill_n( back_inserter(result_in_mm) , 5 , pair<floatT,floatT>(-275,275) );
    std::fill_n( back_inserter(result_in_mm) , 1 , pair<floatT,floatT>(-337.6,337.6) );
    std::fill_n( back_inserter(result_in_mm) , 1 , pair<floatT,floatT>(-408,408) );
    std::fill_n( back_inserter(result_in_mm) , 1 , pair<floatT,floatT>(-438.8,438.8) );
  }
  return result_in_mm;
}
 


// detector to FTK logical layer functions from Joe Tuggle using current disk
// assignment (April 2010). 

const int
detector::ftk_get_layer_or_disk( const DetectorType& dettype, const int& barrel_ec, const int& physical_layer, const int& eta_module, const int& side )
{
  int layer_or_disk( physical_layer );
  // This bit copied from FTK_RawInput:
  if( dettype == SCT_DETECTOR ) {
    if (barrel_ec == 0){ // is barrel
      if     ( physical_layer==0 && side == 1 ) { layer_or_disk = 0; }
      else if( physical_layer==0 && side == 0 ) { layer_or_disk = 1; }
      else if( physical_layer==1 && side == 0 ) { layer_or_disk = 2; }
      else if( physical_layer==1 && side == 1 ) { layer_or_disk = 3; }
      else if( physical_layer==2 && side == 1 ) { layer_or_disk = 4; }
      else if( physical_layer==2 && side == 0 ) { layer_or_disk = 5; }
      else if( physical_layer==3 && side == 0 ) { layer_or_disk = 6; }
      else if( physical_layer==3 && side == 1 ) { layer_or_disk = 7; }
      else { assert( !"unhandled physical layer + side combination" ); }
    } else { // endcap
      int outside = 0;
      int inner_ring = 0;
      if( eta_module > 0 ) { inner_ring = 1; }
      if( (eta_module == 0 || eta_module == 2) && side == 1 ) { outside = 1; }
      if( eta_module == 1 && side == 0 ) { outside = 1; }
      // disk 8 flipped: special case
      if( physical_layer == 8 && side == 1 ) { outside = 0; }
      if( physical_layer == 8 && side == 0 ) { outside = 1; }
      // split the disks up by inner/outer, and by side
      layer_or_disk = 4*physical_layer + 2*inner_ring + outside;
    }
  } // end if SCT
  if( layer_or_disk > 33 || layer_or_disk < 0 ) { 
    assert( !"unknown logical layer assignment" );
    return -1;
  }
  return layer_or_disk;
}

const int
detector::ftk_physical_to_logical_layer( const DetectorType& dettype, const int& barrel_ec, const int& layer_or_disk )
{
  if( layer_or_disk > 33 || layer_or_disk < 0 ) { return -1; }
  // Lookup table based on raw_11L.pmap.  This is for the sct
  // disks. Needs to change if the pmap file changes.
  const int sct_disk[34] = { 9, 10, 9, 10, 7, 8, 7, 8, 
                             5, 6, 5, 6, 3, 4, 3, 4,
                             9, 10, 9, 10, 7, 8, 7, 8,
                             5, 6, 5, 6, 3, 4, 3, 4,
                             9, 10 };
  if( dettype == PIXEL_DETECTOR ) {
    if( barrel_ec == 0 ) { return layer_or_disk; }
    // This needs to change if pmap file changes:
    else if( layer_or_disk < 2 ) { return 2; }
    else { return 1; }
  } else { // SCT
    assert( dettype == SCT_DETECTOR );
    if( barrel_ec == 0 ) { return layer_or_disk+3; }
    else { return sct_disk[layer_or_disk]; }
  }
  assert( !"unhandled" );
  return -1;
}

const bool
detector::ftk_physical_layer_is_stereo_sct( const int& barrel_ec, 
                                            const int& layer_or_disk )
{
  // Lookup table based on raw_11L.pmap. I could do this with 'if'
  // statements, but this is clearer.
  const bool sct_disk_stereo[34] = { 1, 0, 1, 0,
                                     0, 1, 0, 1,
                                     1, 0, 1, 0,
                                     0, 1, 0, 1,
                                     1, 0, 1, 0,
                                     0, 1, 0, 1,
                                     1, 0, 1, 0,
                                     0, 1, 0, 1,
                                     1, 0 };
  if( layer_or_disk > 33 || layer_or_disk < 0 ) { 
    assert( !"unhandled" );
    return 0; 
  }
  if( barrel_ec == 0 ) { return (layer_or_disk % 2); } // In the barrel the odd layer_or_disks are stereo
  else { return sct_disk_stereo[layer_or_disk]; }
}


