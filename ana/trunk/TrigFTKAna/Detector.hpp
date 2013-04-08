#ifndef WRAP_DETECTOR_HPP
#define WRAP_DETECTOR_HPP

#include <cmath>
#include <bitset>
#include <vector>
#include <iterator>
#include <numeric>
#include <algorithm>
#include <utility>
#include <bitset>
#include <sstream>
#include <cassert>
#include <boost/tuple/tuple.hpp>
#include <boost/bind.hpp>
#include <boost/math/special_functions/fpclassify.hpp>

#include "TMath.h"
#include "TLorentzVector.h"
#include "TDatabasePDG.h"

#include "TrigFTKAna/tracking/VertexType.hpp"

#include "TrigFTKAna/Detector_power.hpp"

class AnaTrack;

namespace
detector
{

  // some constants
  // ================================================================
  typedef enum { PIXEL_DETECTOR=1 , SCT_DETECTOR=2 } DetectorType;
  typedef enum { MeV=1 , GeV=1000 , TeV=1000000 } EnergyUnits;
  static const float pion_mass( const EnergyUnits& units ) { return 139.57018/float(units); }
  static const float proton_mass( const EnergyUnits& units ) { return 938.272013/float(units); }
  static const float electron_mass( const EnergyUnits& units ) { return 0.510998910/float(units); }
  static const float muon_mass( const EnergyUnits& units ) { return 105.658367/float(units); }
  static const float K0_mass( const EnergyUnits& units ) { return 497.614/float(units); }
  static const float lambda_mass( const EnergyUnits& units ) { return 1115.683/float(units); }
  static const float jpsi_mass( const EnergyUnits& units ) { return 3096.916/float(units); }
  static const float upsilon_1s_mass( const EnergyUnits& units ) { return 9460.30/float(units); } // pdg 2010
  static const float upsilon_2s_mass( const EnergyUnits& units ) { return 10023.26/float(units); } // pdg 2010
  static const float upsilon_3s_mass( const EnergyUnits& units ) { return 10355.2/float(units); } // pdg 2010
  static const float upsilon_4s_mass( const EnergyUnits& units ) { return 10579.4/float(units); } // pdg 2010
  static const float W_mass( const EnergyUnits& units ) { return 80398./float(units); }
  static const float W_width( const EnergyUnits& units ) { return 2141./float(units); }
  static const float Z_mass( const EnergyUnits& units ) { return 91187.6/float(units); }
  static const float Z_width( const EnergyUnits& units ) { return 2495.2/float(units); }

  // simple math
  // ================================================================
  // sign of input
  template<typename floatT>
  static const floatT sign_of( const floatT& x ) { return( x>0. ? 1 : -1 ); }
  // ET from four vector
  template<typename floatT>
  static const floatT ET( const TLorentzVector& v ) { return (v.E()*v.Pt()/v.P()); }
  // likelihood x calculation
  template<typename floatT>
  static const floatT likx( const floatT& lik ) { return (lik/(static_cast<floatT>(1)+lik)); }
  // DBZ-safe division
  template<typename floatT>
  static const floatT safe_divide( const floatT& num , const floatT& den ) {
    if( boost::math::isnan(den) ) { return( boost::math::isnan(num) ? -std::numeric_limits<floatT>::max() : 0. ); }
    if( den!=0. ) { 
      if( boost::math::isnan(num) ) { return std::numeric_limits<floatT>::max(); }
      return( num/den );
    } else { 
      return( std::numeric_limits<floatT>::max()*sign_of(num) ); 
    }
  }
  // which of the two numbers has the smallest absolute value?
  template<typename floatT>
  static const floatT min_abs( const floatT& a , const floatT& b ) {
    if( std::abs(b) < std::abs(a) ) { return b; }
    return a;
  }
  // which of the two numbers has the largest absolute value?
  template<typename floatT>
  static const floatT max_abs( const floatT& a , const floatT& b ) {
    if( std::abs(b) > std::abs(a) ) { return b; }
    return a;
  }
  // chi2/ndof
  template<typename floatT>
  static const floatT chi2ndof( const floatT& chi2 , const unsigned int& ndof ) { return detector::safe_divide(chi2,static_cast<floatT>(ndof)); }
  // chi2 probability
  template<typename floatT>
  static const floatT chi2prob( const floatT& chi2 , const unsigned int& ndof ) { return TMath::Prob(chi2,ndof); }
  // cotangent
  template<typename floatT>
  static const floatT cot( const floatT& angle ) { return safe_divide(static_cast<floatT>(1),std::tan(angle)); }
  // secant
  template<typename floatT>
  static const floatT sec( const floatT& angle ) { return safe_divide(static_cast<floatT>(1),std::cos(angle)); }
  // cosecant
  template<typename floatT>
  static const floatT csc( const floatT& angle ) { return safe_divide(static_cast<floatT>(1),std::sin(angle)); }
  // safe log10
  template<typename floatT>
  static const floatT safe_log10( const floatT& val ) { if(val>0.) { return std::log10(val); } else { return -std::numeric_limits<floatT>::max(); } }
  // safe ln
  template<typename floatT>
  static const floatT safe_ln( const floatT& val ) { if(val>0.) { return std::log(val); } else { return -std::numeric_limits<floatT>::max(); } }
  //   // non-integer power, multiplied by std::pow
  //   template<typename N,class Y> 
  //   static const Y pow( const Y& x ) { return std::pow(x,static_cast<Y>(N)); }
  // integer power, multiplied by the compiler
  template<long N,class Y> 
  static const Y pow( const Y& x ) { return _pow_impl::pow_impl<N,Y>::call(x); }
  // safe sqrt
  template<typename floatT>
  static const floatT safe_sqrt( const floatT& val ) { if(val>0.) { return std::sqrt(val); } else { return static_cast<floatT>(0); } }
  // sum in quadrature
  template<typename floatT>
  static const floatT quadrature( const floatT& termA , const floatT& termB ) { return std::sqrt( pow<2>(termA) + pow<2>(termB) ); }
  template<typename floatT>
  static const floatT quadrature( const floatT& termA , const floatT& termB , const floatT& termC ) { return std::sqrt( pow<2>(termA) + pow<2>(termB) + pow<2>(termC)); }
  template<typename floatT>
  static const floatT quadrature( const floatT& termA , const floatT& termB , 
                                         const floatT& termC , const floatT& termD ) { 
    return std::sqrt( pow<2>(termA) + pow<2>(termB) + pow<2>(termC)); 
  }

  // coordinates
  // ================================================================
  // convert theta to eta
  template<typename floatT>
  static const floatT etaFromTheta( const floatT& theta ) { return -std::log(std::tan(theta/2.)); }
  // convert lambda to eta
  template<typename floatT>
  static const floatT etaFromCotTheta( const floatT& lambda ) { return etaFromTheta(std::atan2(1,lambda)); }
  // convert eta to theta
  template<typename floatT>
  static const floatT thetaFromEta( const floatT& eta ) { return( 2.*std::atan2(exp(-eta),1) ); }
  // convert lambda to theta
  template<typename floatT>
  static const floatT thetaFromCotTheta( const floatT& lambda ) { return( std::atan2(1,lambda) ); }
  // convert eta to lambda
  template<typename floatT>
  static const floatT cotThetaFromEta( const floatT& eta ) { return( 1./std::tan(thetaFromEta(eta)) ); } // not divide-by-zero safe
  // convert theta uncertainty to eta uncertainty
  template<typename floatT>
  static const floatT etaFromThetaErr( const floatT& theta , const floatT& theta_err ) { return std::abs( -csc(theta) * theta_err ); }
  // convert lambda uncertainty to eta uncertainty
  template<typename floatT>
  static const floatT etaFromCotThetaErr( const floatT& lambda , const floatT& lambda_err ) { 
    return etaFromThetaErr( lambda , std::abs(static_cast<floatT>(1) / (1+pow<2>(lambda))) ); 
  }
  // return TLorentzVector::Eta after suppressing error messages for pT=0 vectors.
  template<typename floatT>
  static const floatT etaFromTLorentzVector( const TLorentzVector& p ) { 
    if( p.Pt() > 0. ) { return p.Eta(); }
    if( p.Pz() > 0. ) { return std::numeric_limits<floatT>::max(); }
    if( p.Pz() < 0. ) { return -std::numeric_limits<floatT>::max(); }
    return std::numeric_limits<floatT>::max();
  }

  template<typename floatT>
  static const floatT mtFromTLorentzVector( const TLorentzVector& mu, const TLorentzVector& nu )  {
    // mT=sqrt( 2*mu.Pt()*_Tmet.Pt()*(1-cos(_Tmet.Phi()-mu.Phi())) )
    return std::sqrt( 2*mu.Pt()*nu.Pt()*(1-cos(mu.Phi()-nu.Phi())) );
  }

  // fmodd: floating point mod with offset
  template<typename floatT>
  static const floatT fmodd( const floatT& num , const floatT& den , const floatT& off ) {
    floatT tmp = num;
    // what happens if tmp is huge compared to den? boom
    if( safe_divide(tmp,den) > 1E6 ) { printf( "WARNING: num = %f , den = %f\n" , num , den ); return 0; assert(false); } //FIXME AK
    while( tmp>=(den+off) ) { tmp -= den; }
    while( tmp<off ) { tmp += den; }
    return tmp;
  }
  // convert phi to "canonical" branch cut (-Pi..Pi)
  template<typename floatT>
  static const floatT canonical_phi( const floatT& phi ) { return fmodd(phi,2*static_cast<floatT>(M_PI),-static_cast<floatT>(M_PI)); }
  // delta phi
  template<typename floatT>
  static const floatT delta_phi( const floatT& p1 , const floatT& p2 ) { return canonical_phi(p1-p2); }
  // abs delta phi
  template<typename floatT>
  static const floatT abs_delta_phi( const floatT& p1 , const floatT& p2 ) { return std::abs(canonical_phi(p1-p2)); }
  // delta eta
  template<typename floatT>
  static const floatT delta_eta( const floatT& e1 , const floatT& e2 ) { return e1-e2; }
  // delta r in eta-phi plane
  template<typename floatT>
  const floatT delta_eta_phi( const floatT& e1 , const floatT& p1 , 
                              const floatT& e2 , const floatT& p2 )
  {
    const floatT deta = delta_eta(e1,e2);
    const floatT dphi = delta_phi(p1,p2);
    return quadrature( deta , dphi );
  }

  // ATLAS detector geometry
  // ================================================================
  template<typename floatT> static const std::vector<floatT>& silicon_barrel_radii();
  template<typename floatT> static const std::vector<floatT>& silicon_disk_z_positions();
  template<typename floatT> static const std::vector< std::pair<floatT,floatT> >& silicon_barrel_z_range();
  template<typename floatT> static const std::vector< std::pair<floatT,floatT> >& silicon_disk_radii_range();
  template<typename floatT> 
  static typename std::vector<floatT>::const_iterator pixel_barrel_radii_begin() { return silicon_barrel_radii<floatT>().begin(); }
  template<typename floatT> 
  static typename std::vector<floatT>::const_iterator pixel_barrel_radii_end() { return std::advance(silicon_barrel_radii<floatT>().begin(),3); }
  template<typename floatT> 
  static typename std::vector<floatT>::const_iterator sct_barrel_radii_begin() { return std::advance(silicon_barrel_radii<floatT>().begin(),3); }
  template<typename floatT> 
  static typename std::vector<floatT>::const_iterator sct_barrel_radii_end() { return silicon_barrel_radii<floatT>().end(); }
  template<typename floatT> 
  static typename std::vector<floatT>::const_iterator pixel_barrel_z_range_begin() { return silicon_barrel_z_range<floatT>().begin(); }
  template<typename floatT> 
  static typename std::vector<floatT>::const_iterator pixel_barrel_z_range_end() { return std::advance(silicon_barrel_z_range<floatT>().begin(),3); }
  template<typename floatT> 
  static typename std::vector<floatT>::const_iterator sct_barrel_z_range_begin() { return std::advance(silicon_barrel_z_range<floatT>().begin(),3); }
  template<typename floatT> 
  static typename std::vector<floatT>::const_iterator sct_barrel_z_range_end() { return silicon_barrel_z_range<floatT>().end(); }
  template<typename floatT> 
  static typename std::vector<floatT>::const_iterator pixel_disk_z_positions_begin() { return silicon_disk_z_positions<floatT>().begin(); }
  template<typename floatT> 
  static typename std::vector<floatT>::const_iterator pixel_disk_z_positions_end() { return std::advance(silicon_disk_z_positions<floatT>().begin(),3); }
  template<typename floatT> 
  static typename std::vector<floatT>::const_iterator sct_disk_z_positions_begin() { return std::advance(silicon_disk_z_positions<floatT>().begin(),3); }
  template<typename floatT> 
  static typename std::vector<floatT>::const_iterator sct_disk_z_positions_end() { return silicon_disk_z_positions<floatT>().end(); }
  template<typename floatT> 
  static typename std::vector<floatT>::const_iterator pixel_disk_radii_range_begin() { return silicon_disk_radii_range<floatT>().begin(); }
  template<typename floatT> 
  static typename std::vector<floatT>::const_iterator pixel_disk_radii_range_end() { return std::advance(silicon_disk_radii_range<floatT>().begin(),3); }
  template<typename floatT> 
  static typename std::vector<floatT>::const_iterator sct_disk_radii_range_begin() { return std::advance(silicon_disk_radii_range<floatT>().begin(),3); }
  template<typename floatT> 
  static typename std::vector<floatT>::const_iterator sct_disk_radii_range_end() { return silicon_disk_radii_range<floatT>().end(); }

  // particle "type" from pdg code
  // ================================================================
  typedef enum { OTHER=0 , PROTON=1 , KAON=2 , PION=3 , MUON=4 , ELECTRON=5 , TAU=6 , NPARTICLES=7 } ParticleType;
  static const ParticleType particle_type( const int& pdg_code ) {
    switch( std::abs(pdg_code) ) {
    case 2212: return PROTON;
    case 321: return KAON;
    case 211: return PION;
    case 15: return TAU;
    case 13: return MUON;
    case 11: return ELECTRON;
    default: return OTHER;
    }
    return OTHER;
  }
  // particle "charge" from pdg code
  static const double charge_from_pdg( const int& pdg_code ) {
    TParticlePDG* p = TDatabasePDG::Instance()->GetParticle(pdg_code);
    if( !p ) {
      switch( pdg_code ) { 
      case 2212: p = TDatabasePDG::Instance()->GetParticle("proton"); break;
      case -2212: p = TDatabasePDG::Instance()->GetParticle("antiproton"); break;
      default: break;
      }
    }
    if( !p ) { return 0.; }
    return p->Charge()/3.0;
  }
  
  // identify b, charm, and light hadrons from pdg code
  static const bool is_b_hadron( const int& pdg_code ) {
    const unsigned int ac( std::abs(pdg_code) % 10000 );
    return( (ac>=500 && ac<=599) ||
            (ac>=5000 && ac<=5999) );
  }
  static const bool is_c_hadron( const int& pdg_code ) {
    const unsigned int ac( std::abs(pdg_code) % 10000 );
    return( (ac>=400 && ac<=499) ||
            (ac>=4000 && ac<=4999) );
  }
  static const bool is_light_hadron( const int& pdg_code ) {
    const unsigned int ac( std::abs(pdg_code) % 10000 );
    return( (ac>=100 && ac<=199) ||
            (ac>=1000 && ac<=1999) ||
            (ac>=200 && ac<=299) ||
            (ac>=2000 && ac<=2999) ||
            (ac>=300 && ac<=399) ||
            (ac>=3000 && ac<=3999) );
  }
  // truth track bitmask from FastTrackSimWrap
  typedef enum { TAU_PARENT_BIT , B_PARENT_BIT , PION_PARENT_BIT , PION_IMMEDIATE_PARENT_BIT , NBITS_PARENT_BIT } TruthParentBits;
  typedef std::bitset<NBITS_PARENT_BIT> TruthParentBitmask;
  static const bool has_tau_parent( const TruthParentBitmask& mask ) { return mask.test(TAU_PARENT_BIT); }
  static const bool has_b_parent( const TruthParentBitmask& mask ) { return mask.test(B_PARENT_BIT); }
  static const bool has_pion_parent( const TruthParentBitmask& mask ) { return mask.test(PION_PARENT_BIT); }
  static const bool has_pion_immediate_parent( const TruthParentBitmask& mask ) { return mask.test(PION_IMMEDIATE_PARENT_BIT); }

  // tracks
  // ================================================================
  // curvature to pt
  template<typename floatT>
  static const floatT ptFromCurvature( const floatT& curvature , const EnergyUnits& units ) { 
    return std::abs(safe_divide(static_cast<floatT>(1),curvature)/floatT(units)); 
  }
  // q over p & theta & charge to pt
  template<typename floatT>
  static const floatT ptFromChargeThetaQoverP( const floatT& charge , const floatT& theta , const floatT& qoverp ) {
    const floatT p = safe_divide(charge,qoverp);
    return p*std::sin(theta);
  }
  // phi direction of vector given x and y components
  template<typename floatT>
  static const floatT phiFromXY( const floatT& x , const floatT& y ) {
    return x != 0 ? std::atan2( y , x ) : std::numeric_limits<floatT>::max()*sign_of(y) ;
  }
  // atlas impact parameter sign convention
  template<typename floatT>
  static const floatT d0_sign( const floatT& x0 , const floatT& y0 , const floatT& px , const floatT& py ) {
    return( sign_of( y0*px - x0*py ) );
  }
  // signed impact parameters for tagging
  template<typename floatT>
  static const floatT directionalD0( const floatT& track_d0 , const floatT& track_phi0 , const floatT& direction_phi ) {
    const floatT dphi = delta_phi( direction_phi , track_phi0 );
    return( std::abs(track_d0) * sign_of(track_d0*sin(dphi)) );
  }
  template<typename floatT>
  static const floatT directionalZ0( const floatT& track_z0 , const floatT& track_eta , const floatT& direction_eta ) {
    const floatT deta = delta_eta(direction_eta,track_eta);
    return( std::abs(track_z0) * sign_of(track_z0*deta) );
  }
  // solenoidal field amplitude in tesla
  template<typename floatT> static const floatT solenoidal_field_in_tesla() { return 2.0; }
  // curvature constant = pT_in_GeV / (rho_in_mm * B_in_tesla)
  template<typename floatT> static const floatT curvature_constant_in_gev_mm_tesla() { return 0.00029979; }
  // compute impact parameter and error on with respect to a given beamline instead of the coordinate origin.
  // valid to zeroth order in the track curvature (series expansion in powers of track curvature).
  template<typename floatT>
  const floatT corrected_d0( const floatT& d0 , const floatT& z0 , const floatT& phi0 , 
                             const floatT& charge , const floatT& ref_x0 , const floatT& ref_y0 , const floatT& ref_z0 ) {
    floatT result = d0;
    result -= (ref_y0*std::cos(phi0)) - (ref_x0*std::sin(phi0)); // zero-th order in curvature.
    // first few terms for higher order corrections. curvature expressed in 1/mm.
    // it is probably not a good idea to include these. the zero-th order term (above) 
    // approximates the result to less than one micron for pT>1 GeV tracks. since this is less than the
    // typical resolution on ref_x0 and ref_y0, that calculation is more than sufficient. also, the
    // curvature-dependent terms below require translation of curvature 1/MeV to 1/mm, which introduces
    // a dependence on the magnetic field. if we obtain an incorrect estimate of the field strength we
    // would introduce errors in the higher order calculation which would limit its usefulness. better
    // to leave these out entirely.
    // result += curvature*detector::pow<2>( (ref_x0*std::cos(phi0)) + (yv*std::sin(phi0)) ); 1st order in curvature
    // result -= detector::pow<2>(curvature)*( ( d0 - (ref_y0*std::cos(phi0)) + (ref_x0*std::sin(phi0)) ) * 
    //                                         detector::pow<2>( (ref_x0*std::cos(phi0)) + (yv*std::sin(phi0)) ) ); 2nd order in curvature
    return result;
  }
  template<typename floatT>
  // corrected d0 error. approximate; not the full calculation with the track and vertex covariance matricies, but close enough.
  const floatT corrected_d0_err( const floatT& d0_err , const floatT& ref_x0_err , const floatT& ref_y0_err , const floatT& phi0 ) {
    return detector::quadrature( d0_err , std::cos(phi0) * ref_y0_err , std::sin(phi0) * ref_x0_err );
  }
  // compute impact parameter with respect to a given primary vertex position instead of the coordinate origin.
  // for now, this is simple, but the interface is there in case the calculation needs to change.
  template<typename floatT>
  const floatT corrected_z0( const floatT& d0 , const floatT& z0 , const floatT& phi0 , const floatT& lambda ,
                             const floatT& charge , const floatT& ref_x0 , const floatT& ref_y0 , const floatT& ref_z0 ) {
    return( z0 - ref_z0 );
  }
  // corrected z0 error. approximate; not the full calculation with the track and vertex covariance matricies, but close enough.
  template<typename floatT>
  const floatT corrected_z0_err( const floatT& z0_err , const floatT& ref_z0_err ) {
    return detector::quadrature( z0_err , ref_z0_err );
  }
  // return position.phi() at a radius of r_in_mm. false means phi is
  // not uniquely determined by the given radius.
  template<typename floatT,typename trackPointerT> static const bool phi_position_at_r( trackPointerT const track , const floatT& r_in_mm , floatT& phi );
  // return position.phi() at z_in_mm. false means phi is not uniquely
  // determined by the given z.
  template<typename floatT,typename trackPointerT> const bool phi_position_at_z( trackPointerT const track , const floatT& z_in_mm , floatT& phi );
  // return track_position.z() at track_position.mag() == r. note that
  // z should be multi-valued at the minimum (|r|=|d0|) and maximum
  // radii from the z axis. in this case, the single z returned is the
  // one closest to track->z0().
  template<typename floatT,typename trackPointerT> const bool z_at_r( trackPointerT const track , const floatT& r_in_mm , floatT& z_in_mm );
  // compute track position.mag() at position.z().
  template<typename floatT,typename trackPointerT> const bool r_at_z( trackPointerT const track , const floatT& z_in_mm , floatT& r_in_mm );
  // compute phi,z values of any intersections silicon barrels
  template<typename floatT,typename trackPointerT> const std::vector< boost::tuples::tuple<floatT,floatT> > intersections_with_silicon_barrels( trackPointerT const track );
  // compute phi,z values of any intersections silicon disks
  template<typename floatT,typename trackPointerT> const std::vector< boost::tuples::tuple<floatT,floatT> > intersections_with_silicon_disks( trackPointerT const track );
  // two track mass with id assumptions
  template<typename floatT> const floatT two_track_mass( const TLorentzVector& trkA , const TLorentzVector& trkB , const floatT& massA , const floatT& massB ) {
    const floatT energy = ( std::sqrt( static_cast<floatT>(pow<2>(trkA.P())) + pow<2>(massA) ) +
                            std::sqrt( static_cast<floatT>(pow<2>(trkB.P())) + pow<2>(massB) ) );
    return safe_sqrt( (energy-static_cast<floatT>(trkA.Pz()+trkB.Pz()))*(energy+static_cast<floatT>(trkA.Pz()+trkB.Pz())) 
                      - pow<2>(static_cast<floatT>(trkA.Px()+trkB.Px())) - pow<2>(static_cast<floatT>(trkA.Py()+trkB.Py())) );
  }

  // reverses a d0 correction that might be applied in TrigFTKLib, assuming the ATLAS d0 sign convention
  // for more info see here: ...
  template<typename floatT>
  static const floatT uncorrected_d0_ATLAS( const floatT d0 , const floatT phi , const floatT sgn_q , const floatT vx , const floatT vy ) {
    floatT x0 = d0 * sin(phi);
    floatT y0 = -d0 * cos(phi);
    return ( (x0+vx) * sin(phi) ) - ( (y0+vy) * cos(phi) );
  }

  // reverses a d0 correction that might be applied in TrigFTKLib, assuming the CDF d0 sign convention
  // for more info see here: ...
  template<typename floatT>
  static const floatT uncorrected_d0_CDF( const floatT d0 , const floatT phi , const floatT sgn_q , const floatT vx , const floatT vy ) {
    floatT x0 = -sgn_q * d0 * sin(phi);
    floatT y0 = sgn_q * d0 * cos(phi);
    return -sgn_q * ( (x0+vx) * sin(phi) ) - ( (y0+vy) * cos(phi) );
  }
  
  // reconstructs MET pz for semileptonic W decay
  // takes lpx, lpy, lpz, metx, mety
  template<typename floatT>
  static const floatT reconstructedMETpz( const floatT& lpx , const floatT& lpy , const floatT& lpz ,
                                          const floatT& metx , const floatT& mety , const short& sol=0 ) {
    floatT pznu;
    floatT soln1=0,soln2=0,det=0;
    floatT a=0, b=0, c=0;
    const floatT wm = W_mass(GeV);//80.448;//+-0.037
    a = lpx*lpx + lpy*lpy;
    b = -( wm*wm*lpz + 2*lpx*lpz*metx + 2*lpy*lpz*mety );
    c = mety*mety*( lpx*lpx+lpz*lpz ) + metx*metx*( lpy*lpy+lpz*lpz )
             - 0.25*wm*wm*wm*wm - wm*wm*lpx*metx - wm*wm*lpy*mety
             - 2*lpx*lpy*metx*mety;
    det = b*b - 4*a*c;
    //cout <<"a, b, c "<<a<<" "<<b<<" "<<c<<endl;
    if (det>0){
      soln1 = (-b + sqrt( b*b - 4*a*c ))/(2*a);
      soln2 = (-b - sqrt( b*b - 4*a*c ))/(2*a);
    }
    else{
      soln1 = -b / (2*a);
      soln2 = -b / (2*a);
    }

    //assign sensible solutions
    if (soln1==soln2) pznu = soln1;
    else {
      if( sol==0 ) pznu = std::fabs(soln1) < std::fabs(soln2) ? soln1 : soln2;
      else pznu = std::fabs(soln1) > std::fabs(soln2) ? soln1 : soln2;
    }

    return pznu;
  }

  // returns eta from px, py, pz
  template<typename floatT>
  static const floatT etaFromPxPyPz( const floatT& px , const floatT& py , const floatT& pz ) {
    return 0.5 * safe_ln( safe_divide( quadrature(px,py,pz) + pz , quadrature(px,py,pz) - pz) );
  }


  // ftk details
  // ================================================================
  // constants below set for current 11 layer (14 coordinate) configuration.
  static const unsigned int ftk_nplanes() { return 14; }
  static const unsigned int ftk_nplanes_pixels() { return 2; }
  static const unsigned int ftk_nplanes_sct() { return 1; }
  static const unsigned int ftk_nlayers() { return 11; }
  static const unsigned int ftk_nlayers_pixels() { return 3; }
  static const unsigned int ftk_nlayers_sct() { return 8; }
  // convert from coordinate to layer and back
  static const unsigned int ftk_1st_coordinate_for_layer( const unsigned int& layer ) {
    if( layer<ftk_nlayers_pixels() ) { return ftk_nplanes_pixels()*layer; }
    else { return ftk_nplanes_pixels()*ftk_nlayers_pixels() + (layer-ftk_nlayers_pixels())*ftk_nplanes_sct(); }
  }
  static const unsigned int ftk_2nd_coordinate_for_layer( const unsigned int& layer ) {
    assert( layer<ftk_nlayers_pixels() );
    return (ftk_nplanes_pixels()*layer) + 1;
  }
  // ftk bitmask interpretation. 
  static const bool ftkHasPlane( const unsigned int& ftk_bitmask , const unsigned int& plane ) { return( ftk_bitmask & (1<<plane) ); }
  static const bool ftkMissedPlane( const unsigned int& ftk_bitmask , const unsigned int& plane ) { return !ftkHasPlane(ftk_bitmask,plane); }
  static const bool ftkHasLayer( const unsigned int& ftk_bitmask , const unsigned int& layer ) {
    if( layer<ftk_nlayers_pixels() ) {
      const unsigned int ibit = ftk_nplanes_pixels()*layer;
      return( ftk_bitmask & (1<<ibit) && (ftk_bitmask & (1<<(ibit+1))) );;
    } // else SCT layer
    static const unsigned int first_sct_bit = ftk_nlayers_pixels()*ftk_nplanes_pixels();
    const unsigned int ibit = ((layer-ftk_nlayers_pixels())*ftk_nplanes_sct()) + first_sct_bit;
    return( ftk_bitmask & (1<<ibit) );
  }
  static const bool ftkMissedLayerSandwich( const unsigned int& ftk_bitmask , const unsigned int& layer ) {
    for( unsigned int il=0, fl=ftk_nlayers_pixels()+ftk_nlayers_sct(); il!=fl; ++il ) {
      if( il>0 && il<(fl-1) ) { // sandwich
        return( !(ftkHasLayer(ftk_bitmask,il-1) && ftkHasLayer(ftk_bitmask,il+1) && !ftkHasLayer(ftk_bitmask,il)) );
      } else if( il==0 ) {
        return( !(ftkHasLayer(ftk_bitmask,il+1) && !ftkHasLayer(ftk_bitmask,il)) );
      } else if( il==(fl-1) ) {
        return( !(ftkHasLayer(ftk_bitmask,il-1) && !ftkHasLayer(ftk_bitmask,il)) );
      }
    }
  }
  static const unsigned int ftkNumPlanesHit( const unsigned int& ftk_bitmask ) {
    unsigned int ihits=0;
    for( unsigned int i=0, f=ftk_nplanes(); i!=f; ++i ) {
      if( ftkHasPlane(ftk_bitmask,i) ) { ++ihits; }
    }
    return ihits;
  }
  static const unsigned int ftkNumPlanesMissed( const unsigned int& ftk_bitmask ) {
    unsigned int ihits=0;
    for( unsigned int i=0, f=ftk_nplanes(); i!=f; ++i ) {
      if( !ftkHasPlane(ftk_bitmask,i) ) { ++ihits; }
    }
    return ihits;
  }
  static const unsigned int ftkNumLayersHit( const unsigned int& ftk_bitmask ) {
    unsigned int ihits=0;
    for( unsigned int i=0, f=(ftk_nlayers_pixels()+ftk_nlayers_sct()); i!=f; ++i ) {
      if( ftkHasLayer(ftk_bitmask,i) ) { ++ihits; }
    }
    return ihits;
  }
  static const unsigned int ftkNumLayersMissed( const unsigned int& ftk_bitmask ) {
    unsigned int imisses=0;
    for( unsigned int i=0, f=(ftk_nlayers_pixels()+ftk_nlayers_sct()); i!=f; ++i ) {
      if( !ftkHasLayer(ftk_bitmask,i) ) { ++imisses; }
    }
    return imisses;
  }
  static const bool ftkBLayerHit( const unsigned int& ftk_bitmask ) {
    return( ftkHasLayer(ftk_bitmask,0) );
  }
  static const unsigned int ftkNumLayersHitPixels( const unsigned int& ftk_bitmask ) {
    unsigned int ihits=0;
    for( unsigned int i=0, f=ftk_nlayers_pixels(); i!=f; ++i ) {
      if( ftkHasLayer(ftk_bitmask,i) ) { ++ihits; }
    }
    return ihits;
  }
  static const unsigned int ftkNumLayersHitSCT( const unsigned int& ftk_bitmask ) {
    unsigned int ihits=0;
    for( unsigned int i=ftk_nlayers_pixels(), f=(ftk_nlayers_pixels()+ftk_nlayers_sct()); i!=f; ++i ) {
      if( ftkHasLayer(ftk_bitmask,i) ) { ++ihits; }
    }
    return ihits;
  }
  static const unsigned int ftkNumLayersMissedPixels( const unsigned int& ftk_bitmask ) {
    unsigned int imisses=0;
    for( unsigned int i=0, f=ftk_nlayers_pixels(); i!=f; ++i ) {
      if( !ftkHasLayer(ftk_bitmask,i) ) { ++imisses; }
    }
    return imisses;
  }
  static const unsigned int ftkNumLayersMissedSCT( const unsigned int& ftk_bitmask ) {
    unsigned int imisses=0;
    for( unsigned int i=ftk_nlayers_pixels(), f=(ftk_nlayers_pixels()+ftk_nlayers_sct()); i!=f; ++i ) {
      if( !ftkHasLayer(ftk_bitmask,i) ) { ++imisses; }
    }
    return imisses;
  }
  static const std::string ftk_bitmask_string( const unsigned long ftk_bitmask ) {
    std::bitset<14> mask( ftk_bitmask );
    std::ostringstream res;
    res << mask;
    return res.str();
  }

  // detector to FTK logical layer using current disk assignment (April 2010, Joe Tuggle)
  //  get the FTK internal variable layer_or_disk as a function of the wrapper output variables.
  const int ftk_get_layer_or_disk( const DetectorType& dettype, const int& barrel_ec, const int& physical_layer, const int& eta_module, const int& side ); // last two parameters ignored for pixels
  //  get the logical layer (11L config) as a function of the layer_or_disk
  const int ftk_physical_to_logical_layer( const DetectorType& dettype, const int& barrel_ec, const int& layer_or_disk );
  //  get the logical layer (11L config) as a function of the wrapper output variables.
  static const int ftk_physical_to_logical_layer( const DetectorType& dettype, const int& barrel_ec, const int& physical_layer, const int& eta_module, const int& side ) {
    const int layer_or_disk = ftk_get_layer_or_disk(dettype, barrel_ec, physical_layer, eta_module, side);
    return ftk_physical_to_logical_layer(dettype, barrel_ec, layer_or_disk);
  }
  //  determine SCT stereo or axial layer given the layer_or_disk
  const bool ftk_physical_layer_is_stereo_sct( const int& barrel_ec, const int& layer_or_disk );
  //  determine SCT stereo or axial layer given the wrapper output variables.
  static const bool ftk_physical_layer_is_stereo_sct( const int& barrel_ec, const int& physical_layer, const int& eta_module, const int& side) {
    const int layer_or_disk = ftk_get_layer_or_disk(SCT_DETECTOR, barrel_ec, physical_layer, eta_module, side);
    return ftk_physical_layer_is_stereo_sct(barrel_ec, layer_or_disk);
  }
  
  // determine region(s) for a given track based on crude detector
  // geometry only.
  template<typename floatT> static const std::bitset<8> ftkPhiInRegion( const floatT& phi );
  template<typename trackPointerT>
  const std::bitset<8> ftkTestIntersectionCountInRegions( trackPointerT const track , const unsigned int& nlayers );
  template<typename floatT>
  const std::bitset<8> ftkTestIntersectionsCountInRegions( const std::vector< boost::tuple<floatT,floatT> >& intersections , 
                                                           const unsigned int& nlayers );

  template<typename floatT> static const floatT hm_range_cov_cc() { return pow<2,floatT>(3E-5); }
  template<typename floatT> static const floatT hm_range_cov_dd() { return pow<2,floatT>(0.1); }
  template<typename floatT> static const floatT hm_range_cov_pp() { return pow<2,floatT>(1E-2); }
  template<typename floatT> static const floatT hm_range_cov_zz() { return pow<2,floatT>(0.2); }
  template<typename floatT> static const floatT hm_range_cov_tt() { return pow<2,floatT>(0.01); }

  template<typename floatT> static const floatT hm_range_cov_cd() { return 5E-7; }
  template<typename floatT> static const floatT hm_range_cov_cp() { return 1E-8; }
  template<typename floatT> static const floatT hm_range_cov_cz() { return 1E-7; }
  template<typename floatT> static const floatT hm_range_cov_ct() { return 5E-7; }
  template<typename floatT> static const floatT hm_range_cov_dp() { return 4E-4; }
  template<typename floatT> static const floatT hm_range_cov_dz() { return 5E-4; }
  template<typename floatT> static const floatT hm_range_cov_dt() { return 1E-5; }
  template<typename floatT> static const floatT hm_range_cov_pz() { return 1E-5; }
  template<typename floatT> static const floatT hm_range_cov_pt() { return 5E-7; }
  template<typename floatT> static const floatT hm_range_cov_zt() { return 1E-3; }


  // experimental function to determine whether or not a pixel channel
  // is ganged. derived from a quick look at Alberto's code but not
  // tested.
  template<typename localT>
  static const bool is_ganged_pixel_channel( const localT& lx ) {
    return( (lx>=153. && lx<154 ) ||
            (lx>=155. && lx<156 ) ||
            (lx>=157. && lx<158 ) ||
            (lx>=159. && lx<160 ) ||
            (lx>=168. && lx<169 ) ||
            (lx>=170. && lx<171 ) ||
            (lx>=172. && lx<173 ) ||
            (lx>=174. && lx<175 ) ||
            (lx>=160. && lx<=167.) );
  }
  template<typename localT>
  static const bool range_includes_ganged_pixel_channel( const localT& lx1 , const localT& lx2 ) {
    // FIXME: does this function really handle all possible cases? debug
    assert( lx2>=lx1 );
    if( lx2<153. ) { return false; }
    if( lx1>=175.) { return false; }
    if( lx1<153. && lx2>=175. ) { return true; }
    if( (lx2-lx1)>=1. ) { return true; }
    return( is_ganged_pixel_channel(lx1) || is_ganged_pixel_channel(lx2) );
  }

  template<typename intT>
  static const ATLAS::VertexType remap_vertex_type_enum( const intT& i ) {
    switch( i ) {
    case 0: return ATLAS::NoVtx; break;
    case 1: return ATLAS::PriVtx; break;
    case 2: return ATLAS::SecVtx; break;
    case 3: return ATLAS::PileUp; break;
    case 4: return ATLAS::ConvVtx; break;
    case 5: return ATLAS::V0Vtx; break;
    case 6: return ATLAS::KinkVtx; break;
    case 7: return ATLAS::V0Lambda; break;
    case 8: return ATLAS::V0LambdaBar; break;
    case 9: return ATLAS::V0KShort; break;
    case -99: return ATLAS::NotSpecified; break;
    default: assert( !"unhandled vertex type; update from ATLAS VertexType.h enum definition" ); break;
    };
  }

  static const std::string& vertex_type_to_string( const ATLAS::VertexType& i ) {
    static std::string sNoVtx("NoVtx");
    static std::string sPriVtx("PriVtx");
    static std::string sSecVtx("SecVtx");
    static std::string sPileUp("PileUp");
    static std::string sConVtx("ConVtx");
    static std::string sV0Vtx("V0Vtx");
    static std::string sKinkVtx("KinkVtx");
    static std::string sV0Lambda("V0Lambda");
    static std::string sV0LambdaBar("V0LambdaBar");
    static std::string sV0KShort("V0KShort");
    switch( i ) {
    case ATLAS::NoVtx: return sNoVtx; break;
    case ATLAS::PriVtx: return sPriVtx; break;
    case ATLAS::SecVtx: return sSecVtx; break;
    case ATLAS::PileUp: return sPileUp; break;
    case ATLAS::ConvVtx: return sConVtx; break;
    case ATLAS::V0Vtx: return sV0Vtx; break;
    case ATLAS::KinkVtx: return sKinkVtx; break;
    case ATLAS::V0Lambda: return sV0Lambda; break;
    case ATLAS::V0LambdaBar: return sV0LambdaBar; break;
    case ATLAS::V0KShort: return sV0KShort; break;
    default: assert( !"unhandled vertex type; update from ATLAS VertexType.h enum definition" ); break;
    };
  }

  // Overall muon type for the purpose of MCP efficiency/trigger correction constants
  // https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/MCPAnalysisGuidelinesEPS2011
  // This is not a property of the muon (since a STACO_COMBINED muon is by definition also STACO_LOOSE)
  // Rather, it must be given as an input from the user when invoking efficiency correction classes
  enum MCP_TYPE {
    MCP_STACO_COMBINED=0,
    MCP_STACO_LOOSE=1,
    MCP_STACO_TIGHT=2,
    MCP_MUID_COMBINED=100,
    MCP_MUID_LOOSE=101,
    MCP_MUID_TIGHT=102
  };
  static const MCP_TYPE select_mcp_type(const int& v) {
    switch(v) {
    case 0: return MCP_STACO_COMBINED; break;
    case 1: return MCP_STACO_LOOSE; break;
    case 2: return MCP_STACO_TIGHT; break;
    case 100: return MCP_MUID_COMBINED; break;
    case 101: return MCP_MUID_LOOSE; break;
    case 102: return MCP_MUID_TIGHT; break;
    default: return MCP_STACO_COMBINED;
    }
    return MCP_STACO_COMBINED;
  }

  // Overall electron type for the purpose of egamma efficiency/trigger correction constants
  // This is not a property of the electron (since a loose electron also satisfied tight criteria)
  // Rather, it must be given as an input from the user when invoking efficiency correction classes
  enum EGAMMA_TYPE {
    EGAMMA_LOOSEPP=0,
    EGAMMA_MEDIUMPP=1,
    EGAMMA_TIGHTPP=2
  };
  static const EGAMMA_TYPE select_egamma_type(const int& v) {
    switch(v) {
    case 0: return EGAMMA_LOOSEPP; break;
    case 1: return EGAMMA_MEDIUMPP; break;
    case 2: return EGAMMA_TIGHTPP; break;
    default: return EGAMMA_TIGHTPP;
    }
    return EGAMMA_TIGHTPP;
  }
  
  // strings
  template<typename T>
  inline const std::string to_string( T const& value ) {
    std::ostringstream streamOut;
    streamOut << value;
    return streamOut.str();
  }

};

#endif // WRAP_DETECTOR_HPP
