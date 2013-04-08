#ifndef WRAP_ANATRACK_HPP
#define WRAP_ANATRACK_HPP

// ANATRACK
// ================================================================
// Data for a single track
// ================================================================
// 2009-05-21 Antonio Boveia (boveia@hep.uchicago.edu)

#include <vector>
#include <numeric>
#include <cmath>
#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <bitset>
#include <boost/shared_ptr.hpp>
#include <boost/format.hpp>
#include <boost/optional.hpp>
#include <boost/foreach.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/version.hpp>
#include <TLorentzVector.h>
#include "TrigFTKAna/Detector.hpp"
#include "TrigFTKAna/UniqueBarcode.hpp"
#include "TrigFTKAna/AnaSiCluster.hpp"
#include "TrigFTKAna/AnaVertex.hpp"
#include "TrigFTKAna/AnaParticle.hpp"

class
AnaTrack : public AnaParticle
{

public:

  typedef float ftype;

  class Algorithm
  {
  public:
    typedef enum { UNKNOWN=0 , SITRACK=1 , IDSCAN=2 , FTK=5 , EF=6 , OFFL=7 , TRUTH=99 } Type;
  private:
    Type _type;
private:
  friend class boost::serialization::access;
  template<class archiveT>
  void serialize( archiveT& ar, const unsigned int version ) {
    ar & boost::serialization::make_nvp("type",_type);
  }
  public:
    Algorithm() : _type(UNKNOWN) {}
    Algorithm( const Type& rtype ) : _type(rtype) {}
    operator Type() const { return _type; }
    const std::string& name() const { 
      switch( _type ) {
      case TRUTH:
        { static std::string result("TRUTH"); return result; }
        break;
      case OFFL:
        { static std::string result("OFFLINE"); return result; }
        break;
      case EF:
        { static std::string result("EF"); return result; }
        break;
      case FTK:
        { static std::string result("FTK"); return result; }
        break;
      case IDSCAN:
        { static std::string result("IDSCAN"); return result; }
        break;
      case SITRACK:
        { static std::string result("SITRACK"); return result; }
        break;
      case UNKNOWN:
      default:
        { static std::string unk("UNKNOWN"); return unk; }
        break;
      }
    }
    operator std::string() { return name(); }
  };

protected:

  Algorithm _alg;

  // track fit/parameters
  ftype _curv; // signed by charge
  ftype _d0;
  ftype _d0_raw;
  ftype _phi0;
  ftype _phi0_raw;
  ftype _z0;
  ftype _z0_raw;
  ftype _lambda; // cot(theta)
  boost::optional<ftype> _exPV_d0;
  boost::optional<ftype> _exPV_z0;
  boost::optional<ftype> _exPV_phi0;
  boost::optional<ftype> _exPV_d0_err;
  boost::optional<ftype> _exPV_z0_err;
  boost::optional<ftype> _exPV_phi0_err;
  // covariance matrix
  ftype _cov_cc; 
  ftype _cov_cd;
  ftype _cov_cp;
  ftype _cov_cz;
  ftype _cov_ct;
  ftype _cov_dd;
  ftype _cov_dp;
  ftype _cov_dz;
  ftype _cov_dt;
  ftype _cov_pp;
  ftype _cov_pz;
  ftype _cov_pt;
  ftype _cov_zz;
  ftype _cov_zt;
  ftype _cov_tt;
  // fit properties
  ftype _chi2;
  ftype _origchi2;
  unsigned int _ndof;
  unsigned int _nhits;
  unsigned int _nmisses;
  // hit counts
  unsigned int _n_b_layer_hits;
  unsigned int _n_pixel_hits;
  unsigned int _n_sct_hits;
  unsigned int _n_trt_hits;
  unsigned int _expect_b_layer_hits;
  ftype _trratio; // (nTRThigh+nTRThighOutliers)/(nTRT+nTRTOutliers)
  unsigned int _n_b_layer_outliers;
  unsigned int _n_pixel_outliers;
  unsigned int _n_pixel_holes;
  unsigned int _n_sct_holes;
  mutable int _n_pixel_shared;
  mutable int _n_sct_shared;

  // information valid only for ftk tracks
  unsigned long _ftk_bitmask;
  std::vector<AnaTrack::ftype> _ftk_coords;
  unsigned int _ftk_bank;
  unsigned int _ftk_bank_id;
  unsigned long _ftk_road_id;
  unsigned long _ftk_bank_track_id;
  int _ftk_hf_rejected;
  int _ftk_hw_rejected;
  int _ftk_hw_track_id;
  int _ftk_HW_rejected;
  mutable int _ftk_hole_number;

  // truth info. for mc tracks, this is the identity of the particle.
  // for reconstructed tracks, this is identity of the matched geant particle,
  // and _barcode_matching_fraction is the matching figure of merit.
  UniqueBarcode _barcode; 
  int _pdg_code;
  detector::TruthParentBitmask _truth_parent_bitmask;
  bool _detpaper;
  mutable ftype _endvert_radius;
  mutable ftype _beginvert_z;
  mutable ftype _beginvert_radius;
  ftype _barcode_matching_fraction;

  mutable std::vector< boost::shared_ptr<const AnaSiCluster> > _si_clusters;
  
  friend class NtWrapper;
  friend class NtRoad;
  friend class NtFitter;
  friend class NtTrigFTKLib;
  friend class NtCBNT;
  friend class AnaEventMgr;
  friend class NtD3PDphysWZ;
  friend class NtD3PDphysEW;
  friend class NtD3PDphysTop;
  friend class NtD3PDphysPeter;
  friend class NtD3PDphysJetMET;
  friend class AnaMuon;

private:

  friend class boost::serialization::access;
  template<class archiveT>
  void serialize( archiveT& ar, const unsigned int version ) {
    if( version>2 ) { ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(AnaParticle); }
    ar & boost::serialization::make_nvp("alg",_alg);
    ar & boost::serialization::make_nvp("curv",_curv);
    ar & boost::serialization::make_nvp("d0",_d0);
    ar & boost::serialization::make_nvp("d0_raw",_d0_raw);
    ar & boost::serialization::make_nvp("phi0",_phi0);
    ar & boost::serialization::make_nvp("phi0_raw",_phi0_raw);
    ar & boost::serialization::make_nvp("z0",_z0);
    ar & boost::serialization::make_nvp("z0_raw",_z0_raw);
    ar & boost::serialization::make_nvp("lambda",_lambda);
    ar & boost::serialization::make_nvp("cov_cc",_cov_cc); 
    ar & boost::serialization::make_nvp("cov_cd",_cov_cd);
    ar & boost::serialization::make_nvp("cov_cp",_cov_cp);
    ar & boost::serialization::make_nvp("cov_cz",_cov_cz);
    ar & boost::serialization::make_nvp("cov_ct",_cov_ct);
    ar & boost::serialization::make_nvp("cov_dd",_cov_dd);
    ar & boost::serialization::make_nvp("cov_dp",_cov_dp);
    ar & boost::serialization::make_nvp("cov_dz",_cov_dz);
    ar & boost::serialization::make_nvp("cov_dt",_cov_dt);
    ar & boost::serialization::make_nvp("cov_pp",_cov_pp);
    ar & boost::serialization::make_nvp("cov_pz",_cov_pz);
    ar & boost::serialization::make_nvp("cov_pt",_cov_pt);
    ar & boost::serialization::make_nvp("cov_zz",_cov_zz);
    ar & boost::serialization::make_nvp("cov_zt",_cov_zt);
    ar & boost::serialization::make_nvp("cov_tt",_cov_tt);
    ar & boost::serialization::make_nvp("chi2",_chi2);
    ar & boost::serialization::make_nvp("origchi2",_origchi2);
    ar & boost::serialization::make_nvp("ndof",_ndof);
    ar & boost::serialization::make_nvp("nhits",_nhits);
    ar & boost::serialization::make_nvp("nmisses",_nmisses);
    ar & boost::serialization::make_nvp("n_b_layer_hits",_n_b_layer_hits);
    ar & boost::serialization::make_nvp("n_pixel_hits",_n_pixel_hits);
    ar & boost::serialization::make_nvp("n_sct_hits",_n_sct_hits);
    if( version>=3 ) { ar & boost::serialization::make_nvp("n_trt_hits",_n_trt_hits); }
    if( version>=4 ) {
      ar & boost::serialization::make_nvp("expect_b_layer_hits",_expect_b_layer_hits);
      ar & boost::serialization::make_nvp("trratio",_trratio);
      ar & boost::serialization::make_nvp("n_b_layer_outliers",_n_b_layer_outliers);
      ar & boost::serialization::make_nvp("n_pixel_outliers",_n_pixel_outliers);
    }
    ar & boost::serialization::make_nvp("n_pixel_holes",_n_pixel_holes);
    ar & boost::serialization::make_nvp("n_sct_holes",_n_sct_holes);
    ar & boost::serialization::make_nvp("n_pixel_shared",_n_pixel_shared);
    ar & boost::serialization::make_nvp("n_sct_shared",_n_sct_shared);
    ar & boost::serialization::make_nvp("ftk_bitmask",_ftk_bitmask);
    ar & boost::serialization::make_nvp("ftk_coords",_ftk_coords);
    ar & boost::serialization::make_nvp("ftk_bank",_ftk_bank);
    ar & boost::serialization::make_nvp("ftk_bank_id",_ftk_bank_id);
    ar & boost::serialization::make_nvp("ftk_road_id",_ftk_road_id);
    ar & boost::serialization::make_nvp("ftk_bank_track_id",_ftk_bank_track_id);
    ar & boost::serialization::make_nvp("ftk_hf_rejected",_ftk_hf_rejected);
    ar & boost::serialization::make_nvp("ftk_hw_rejected",_ftk_hw_rejected);
    ar & boost::serialization::make_nvp("ftk_hw_track_id",_ftk_hw_track_id);
    ar & boost::serialization::make_nvp("ftk_HW_rejected",_ftk_HW_rejected);
    ar & boost::serialization::make_nvp("ftk_hole_number",_ftk_hole_number);
    ar & boost::serialization::make_nvp("barcode",_barcode); 
    ar & boost::serialization::make_nvp("pdg_code",_pdg_code);
    ar & boost::serialization::make_nvp("truth_parent_bitmask",_truth_parent_bitmask);
    ar & boost::serialization::make_nvp("detpaper",_detpaper);
    ar & boost::serialization::make_nvp("endvert_radius",_endvert_radius);
    ar & boost::serialization::make_nvp("beginvert_z",_beginvert_z);
    ar & boost::serialization::make_nvp("beginvert_radius",_beginvert_radius);
    ar & boost::serialization::make_nvp("barcode_matching_fraction",_barcode_matching_fraction);
    if( version<2 ) { return; }
    ar & boost::serialization::make_nvp("corr_d0",_exPV_d0);
    ar & boost::serialization::make_nvp("corr_phi0",_exPV_phi0);
    ar & boost::serialization::make_nvp("corr_z0",_exPV_z0);
    ar & boost::serialization::make_nvp("corr_d0_err",_exPV_d0_err);
    ar & boost::serialization::make_nvp("corr_phi0_err",_exPV_phi0_err);
    ar & boost::serialization::make_nvp("corr_z0_err",_exPV_z0_err);
    //ar & boost::serialization::make_nvp("si_clusters",_si_clusters); // not supported yet (wait for boost 1.40)
  }

public:

  AnaTrack()
    : _alg( Algorithm::UNKNOWN )
    , _curv( std::numeric_limits<ftype>::min() )
    , _d0( 0. )
    , _d0_raw( 0. )
    , _phi0( 0. )
    , _phi0_raw( 0. )
    , _z0( 0. )
    , _z0_raw( 0. )
    , _exPV_d0()
    , _exPV_phi0()
    , _exPV_z0()
    , _exPV_d0_err()
    , _exPV_phi0_err()
    , _exPV_z0_err()
    , _lambda( std::numeric_limits<ftype>::max() )
    , _cov_cc( std::numeric_limits<ftype>::max() )
    , _cov_cd( std::numeric_limits<ftype>::max() )
    , _cov_cp( std::numeric_limits<ftype>::max() )
    , _cov_cz( std::numeric_limits<ftype>::max() )
    , _cov_ct( std::numeric_limits<ftype>::max() )
    , _cov_dd( std::numeric_limits<ftype>::max() )
    , _cov_dp( std::numeric_limits<ftype>::max() )
    , _cov_dz( std::numeric_limits<ftype>::max() )
    , _cov_dt( std::numeric_limits<ftype>::max() )
    , _cov_pp( std::numeric_limits<ftype>::max() )
    , _cov_pz( std::numeric_limits<ftype>::max() )
    , _cov_pt( std::numeric_limits<ftype>::max() )
    , _cov_zz( std::numeric_limits<ftype>::max() )
    , _cov_zt( std::numeric_limits<ftype>::max() )
    , _cov_tt( std::numeric_limits<ftype>::max() )
    , _chi2( std::numeric_limits<ftype>::max() )
    , _origchi2( std::numeric_limits<ftype>::max() )
    , _ndof( 0u )
    , _nhits( 0u )
    , _nmisses( std::numeric_limits<unsigned int>::max() )
    , _n_b_layer_hits( 0 )
    , _n_pixel_hits( 0 )
    , _n_sct_hits( 0 )
    , _n_trt_hits( 0 )
    , _expect_b_layer_hits( 0 )
    , _trratio( std::numeric_limits<ftype>::max() )
    , _n_b_layer_outliers( std::numeric_limits<unsigned int>::max() )
    , _n_pixel_outliers( std::numeric_limits<unsigned int>::max() )
    , _n_pixel_holes( std::numeric_limits<unsigned int>::max() )
    , _n_sct_holes( std::numeric_limits<unsigned int>::max()  )
    , _n_pixel_shared( -1 )
    , _n_sct_shared( -1 )
    , _ftk_bitmask( 0ul )
    , _ftk_coords()
    , _ftk_bank( std::numeric_limits<unsigned int>::max() )
    , _ftk_bank_id( std::numeric_limits<unsigned int>::max() )
    , _ftk_road_id( std::numeric_limits<unsigned long>::max() ) 
    , _ftk_bank_track_id( std::numeric_limits<unsigned long>::max() ) 
    , _ftk_hf_rejected( -1 )
    , _ftk_hw_rejected( -1 )
    , _ftk_hw_track_id( -1 )
    , _ftk_HW_rejected( 0 )
    , _ftk_hole_number( -1 )
    , _barcode(-1,-1)
    , _pdg_code(0)
    , _truth_parent_bitmask()
    , _detpaper(false)
    , _endvert_radius( std::numeric_limits<ftype>::max() )
    , _beginvert_radius( 0. )
    , _beginvert_z( 0. )
    , _barcode_matching_fraction( 0. )
    , _si_clusters()
  {}
  AnaTrack(UniqueBarcode& code)
    : _alg( Algorithm::UNKNOWN )
    , _curv( std::numeric_limits<ftype>::min() )
    , _d0( 0. )
    , _d0_raw( 0. )
    , _phi0( 0. )
    , _phi0_raw( 0. )
    , _z0( 0. )
    , _z0_raw( 0. )
    , _lambda( std::numeric_limits<ftype>::max() )
    , _cov_cc( std::numeric_limits<ftype>::max() )
    , _cov_cd( std::numeric_limits<ftype>::max() )
    , _cov_cp( std::numeric_limits<ftype>::max() )
    , _cov_cz( std::numeric_limits<ftype>::max() )
    , _cov_ct( std::numeric_limits<ftype>::max() )
    , _cov_dd( std::numeric_limits<ftype>::max() )
    , _cov_dp( std::numeric_limits<ftype>::max() )
    , _cov_dz( std::numeric_limits<ftype>::max() )
    , _cov_dt( std::numeric_limits<ftype>::max() )
    , _cov_pp( std::numeric_limits<ftype>::max() )
    , _cov_pz( std::numeric_limits<ftype>::max() )
    , _cov_pt( std::numeric_limits<ftype>::max() )
    , _cov_zz( std::numeric_limits<ftype>::max() )
    , _cov_zt( std::numeric_limits<ftype>::max() )
    , _cov_tt( std::numeric_limits<ftype>::max() )
    , _chi2( std::numeric_limits<ftype>::max() )
    , _origchi2( std::numeric_limits<ftype>::max() )
    , _ndof( 0u )
    , _nhits( 0u )
    , _nmisses( std::numeric_limits<unsigned int>::max() )
    , _n_b_layer_hits( 0 )
    , _n_pixel_hits( 0 )
    , _n_sct_hits( 0 )
    , _n_trt_hits( 0 )
    , _expect_b_layer_hits( 0 )
    , _trratio( std::numeric_limits<ftype>::max() )
    , _n_b_layer_outliers( std::numeric_limits<unsigned int>::max() )
    , _n_pixel_outliers( std::numeric_limits<unsigned int>::max() )
    , _n_pixel_holes( std::numeric_limits<unsigned int>::max() )
    , _n_sct_holes( std::numeric_limits<unsigned int>::max()  )
    , _n_pixel_shared( -1 )
    , _n_sct_shared( -1 )
    , _ftk_bitmask( 0ul )
    , _ftk_coords()
    , _ftk_bank( std::numeric_limits<unsigned int>::max() )
    , _ftk_bank_id( std::numeric_limits<unsigned int>::max() )
    , _ftk_road_id( std::numeric_limits<unsigned long>::max() ) 
    , _ftk_bank_track_id( std::numeric_limits<unsigned long>::max() ) 
    , _ftk_hf_rejected( -1 )
    , _ftk_hw_rejected( -1 )
    , _ftk_hw_track_id( -1 )
    , _ftk_HW_rejected( 0 )
    , _ftk_hole_number( -1 )
    , _barcode(code)
    , _pdg_code(0)
    , _truth_parent_bitmask()
    , _detpaper(false)
    , _endvert_radius( std::numeric_limits<ftype>::max() )
    , _beginvert_radius( 0. )
    , _beginvert_z( 0. )
    , _barcode_matching_fraction( 0. )
    , _si_clusters()
  {}
  virtual ~AnaTrack() {}

  const Algorithm& algorithm() const { return _alg; }
  void setAlgorithm(const Algorithm& newalg) { _alg = newalg; }
  const Algorithm& author() const { return algorithm(); }
  const bool is_sitrack() const { return _alg==Algorithm::SITRACK; }
  const bool is_idscan() const { return _alg==Algorithm::IDSCAN; }
  const bool is_ftk() const { return _alg==Algorithm::FTK; }
  const bool is_ef() const { return _alg==Algorithm::EF; }
  const bool is_offline() const { return _alg==Algorithm::OFFL; }
  const bool is_truth() const { return _alg==Algorithm::TRUTH; }
  const bool is_truth_tau_decay() const { return detector::has_tau_parent(_truth_parent_bitmask); }
  const bool is_truth_b_decay() const { return detector::has_b_parent(_truth_parent_bitmask); }
  const bool is_truth_charged_pion_or_secondary() const { return detector::has_pion_parent(_truth_parent_bitmask); }
  const bool is_truth_charged_pion_or_immediate_secondary() const { return detector::has_pion_immediate_parent(_truth_parent_bitmask); }

  // track parameters, uncertainties, and significances (pulls)
  const ftype charge() const { return( detector::sign_of(_curv) ); }
  const ftype curvature() const { return _curv; } // 1/MeV
  const ftype pt() const { return detector::ptFromCurvature(_curv,detector::GeV); } // GeV
  const ftype phi0() const { return _phi0; } // rad
  const ftype phi0_raw() const { return _phi0; } // rad
  const ftype d0() const { return _d0; } 
  const ftype d0_raw() const { return _d0_raw; } // use if the correction is done in TrigFTKSim
  const ftype d0_raw( const ftype vx , const ftype vy ) const { return detector::uncorrected_d0_CDF(_d0,_phi0,detector::sign_of(_curv),vx,vy); }
  const ftype exPV_d0() const { if( _exPV_d0 ) { return( *_exPV_d0 ); } else { return std::numeric_limits<ftype>::max(); } }
  const ftype exPV_d0_err() const { if( _exPV_d0_err ) { return( *_exPV_d0_err ); } else { return std::numeric_limits<ftype>::max(); } }
  const ftype exPV_d0_signif() const { if( _exPV_d0 && _exPV_d0_err ) { return( detector::safe_divide( *_exPV_d0 , *_exPV_d0_err ) ); } else { return std::numeric_limits<ftype>::max(); } }
  const ftype exPV_abs_d0_signif() const { if( _exPV_d0 && _exPV_d0_err ) { return( std::abs( detector::safe_divide( *_exPV_d0 , *_exPV_d0_err ) ) ); } else { return std::numeric_limits<ftype>::max(); } }
  const ftype exPV_d0( const ftype vx , const ftype vy , const ftype vz ) const { return detector::corrected_d0(_d0,_z0,_phi0,detector::sign_of(_curv),vx,vy,vz); }
  const ftype d0_corr( const ftype vx , const ftype vy , const ftype vz ) const { return exPV_d0(vx,vy,vz); }
  const ftype exPV_z0() const { if( _exPV_z0 ) { return( *_exPV_z0 ); } else { return std::numeric_limits<ftype>::max(); } }
  const ftype exPV_z0_sin() const { if( _exPV_z0 ) { return( (*_exPV_z0)*std::sin(theta()) ); } else { return std::numeric_limits<ftype>::max(); } }
  const ftype exPV_z0_err() const { if( _exPV_z0_err ) { return( *_exPV_z0_err ); } else { return std::numeric_limits<ftype>::max(); } }
  const ftype exPV_z0_signif() const { if( _exPV_z0 && _exPV_z0_err ) { return( detector::safe_divide( *_exPV_z0 , *_exPV_z0_err ) ); } else { return std::numeric_limits<ftype>::max(); } }
  const ftype exPV_abs_z0_signif() const { if( _exPV_z0 && _exPV_z0_err ) { return( std::abs( detector::safe_divide( *_exPV_z0 , *_exPV_z0_err ) ) ); } else { return std::numeric_limits<ftype>::max(); } }
  const ftype exPV_phi0() const { if( _exPV_phi0 ) { return( *_exPV_phi0 ); } else { return std::numeric_limits<ftype>::max(); } }
  const ftype exPV_phi0_err() const { if( _exPV_phi0_err ) { return( *_exPV_phi0_err ); } else { return std::numeric_limits<ftype>::max(); } }
  const ftype exPV_phi0_signif() const { if( _exPV_phi0 && _exPV_phi0_err ) { return( detector::safe_divide( *_exPV_phi0 , *_exPV_phi0_err ) ); } else { return std::numeric_limits<ftype>::max(); } }
  const ftype lambda() const { return _lambda; }
  const ftype ct() const { return _lambda; }
  const ftype theta() const { return detector::thetaFromCotTheta(_lambda); }
  const ftype eta() const { return detector::etaFromCotTheta(_lambda); }
  const ftype z0() const { return _z0; } 
  const ftype z0_raw() const { return _z0_raw; } // use if correction is done in TrigFTKSim
  const ftype z0_raw( const float zvert ) const { return _z0 + zvert; }
  const ftype z0c(const ftype& zvert) const { return( _z0 - zvert ); } // z0 corrected for vertex
  const ftype z0cL(const ftype& zvert) const { return( z0c(zvert) * std::sin( theta() ) ); } // z0 corrected for vertex and angle

  const ftype pt_err() const { return( detector::ptFromCurvature(_curv,detector::GeV) * detector::safe_divide(curv_err(),_curv) ); }
  const ftype curv_err() const { return std::sqrt( _cov_cc ); }
  const ftype curvature_err() const { return curv_err(); }
  const ftype d0_err() const { return std::sqrt( _cov_dd ); }
  const ftype phi0_err() const { return std::sqrt( _cov_pp ); }
  const ftype z0_err() const { if( !is_ftk() ) { return std::sqrt( _cov_zz ); } else { return 0.0188733 /*crude; rms of residual. FIXME*/; } }
  const ftype lambda_err() const { return std::sqrt( _cov_tt ); }
  const ftype eta_err() const { return detector::etaFromCotThetaErr(_lambda,lambda_err()); }
  const ftype covariance_cc() const { return _cov_cc; } // 1/GeV^2
  const ftype covariance_cd() const { return _cov_cd; } // 1/GeV cm
  const ftype covariance_cp() const { return _cov_cp; } // 1/GeV rad
  const ftype covariance_cz() const { return _cov_cz; } // 1/GeV cm
  const ftype covariance_ct() const { return _cov_ct; } // 1/GeV
  const ftype covariance_dd() const { return _cov_dd; } // cm^2
  const ftype covariance_dp() const { return _cov_dp; } // etc...
  const ftype covariance_dz() const { return _cov_dz; }
  const ftype covariance_dt() const { return _cov_dt; }
  const ftype covariance_pp() const { return _cov_pp; }
  const ftype covariance_pz() const { return _cov_pz; }
  const ftype covariance_pt() const { return _cov_pt; }
  const ftype covariance_zz() const { return _cov_zz; }
  const ftype covariance_zt() const { return _cov_zt; }
  const ftype covariance_tt() const { return _cov_tt; }

  const TLorentzVector four_momentum() const { 
    return TLorentzVector( pt()*std::cos(phi0()),pt()*std::sin(phi0()),pt()*ct(),
                           detector::quadrature( std::abs(pt()*(1.+detector::pow<2>(ct()))) , static_cast<double>(detector::pion_mass(detector::GeV)) ) );
  }
  
  const ftype curvature_signif() const { return detector::safe_divide(_curv,curv_err()); }
  const ftype d0_signif() const { return detector::safe_divide(_d0,d0_err()); }
  const ftype abs_d0_signif() const { return std::abs( detector::safe_divide(_d0,d0_err()) ); }
  const ftype phi0_signif() const { return detector::safe_divide(_phi0,phi0_err()); }
  const ftype z0_signif() const { return detector::safe_divide(_z0,z0_err()); }
  const ftype ct_signif() const { return detector::safe_divide(_lambda,lambda_err()); }
  const ftype eta_signif() const { return detector::safe_divide(eta(),eta_err()); }
   // corrected z0 significance:  z0cL / (z0CL*fractional_uncertainty) = 1 / fractional_uncertainty
  //    fractional uncertainty is si
  const ftype z0cL_signif(const ftype& zvert,const ftype& zvert_err) const { 
    const ftype this_dz0c( z0c(zvert) );
    if( this_dz0c == static_cast<ftype>(0) ) { return std::numeric_limits<ftype>::max(); }
    const ftype dz0_part = ( detector::pow<2>( z0_err() ) + detector::pow<2>( zvert_err ) ) / detector::pow<2>(this_dz0c);
    const ftype lambda_part = detector::pow<2>( (lambda_err() * lambda()) / (static_cast<ftype>(1) + detector::pow<2>(lambda()) ) );
    const ftype frac_uncertainty = std::sqrt( dz0_part + lambda_part );
    return detector::safe_divide( static_cast<ftype>(1) , frac_uncertainty );
  }

  
  const ftype signed_d0( const ftype& phi_dir ) const { return detector::directionalD0(_d0,_phi0,phi_dir); }
  const ftype signed_z0cL( const ftype& eta_dir , const ftype& zvert ) const { return detector::directionalZ0(z0cL(zvert),eta(),eta_dir); }
  const ftype signed_d0_signif( const ftype& phi_dir ) const { return detector::safe_divide(detector::directionalD0(_d0,_phi0,phi_dir),d0_err()); }
  const ftype signed_z0cL_signif( const ftype& eta_dir , const ftype& zvert , const ftype& zvert_err ) const { 
    const ftype sz0cl = signed_z0cL( eta_dir , zvert );
    const ftype sign_z0cL = detector::sign_of( sz0cl );
    return( std::abs(z0cL_signif(zvert,zvert_err)) * sign_z0cL );
  }

  const ftype monica_d0_err() const {
    // monica's v12/13 resolution parameterization
    if( is_idscan() || is_ef() ) { return d0_err(); }
    if( !is_ftk() && !is_offline() ) { return std::numeric_limits<double>::max(); }
    // handle FTK and OFFLINE here
    ftype par[4] = {5.17623e-02, 2.34935e-01, -8.70057e-01, -6.64523e-04};
    if( is_offline() ) {
      // if ipat use different parameters
      par[0] = 3.93576e-02;
      par[1] = 1.87202e-01;
      par[2] = -8.32746e-01;
      par[3] = -7.09288e-04;
    }
    ftype pt_tmp = std::min( pt() , static_cast<float>(15) ); // clip at 15 GeV. express in gev.
    ftype sigma = par[0]+par[1]*exp(par[2]*pt_tmp)+par[3]*pt_tmp;
    // need to multiply by 0.9, handle extremes better.
    return sigma;
  }
  const ftype d0_signif_monica() const { return detector::safe_divide(_d0,monica_d0_err()); }
  const ftype signed_d0_signif_monica( const ftype& phi_dir ) const { return detector::safe_divide(detector::directionalD0(d0(),phi0(),phi_dir),monica_d0_err()); }
  
  const ftype chi2() const { return _chi2; }
  const ftype origchi2() const { return _origchi2; }
  const unsigned int nhits() const { return _nhits; }
  const unsigned int ndof() const { return _ndof; }
  const unsigned int nmisses() const { return _nmisses; }
  const ftype chi2ndof() const { return detector::chi2ndof(_chi2,_ndof); }
  const ftype chi2prob() const { return detector::chi2prob(_chi2,_ndof); }
  const ftype logchi2prob() const { const ftype prob=detector::chi2prob(_chi2,_ndof); return(prob>0. ? log10(prob) : -std::numeric_limits<ftype>::max() ); }

  const unsigned int b_layer_nhits() const { return _n_b_layer_hits; }
  const unsigned int pixel_nhits() const { return _n_pixel_hits; }
  const unsigned int sct_nhits() const { return _n_sct_hits; }
  const unsigned int trt_nhits() const { return _n_trt_hits; }

  const unsigned int expect_b_layer_hits() const { return _expect_b_layer_hits; }
  const ftype trratio() const { return _trratio; }
  const unsigned int n_b_layer_outliers() const { return _n_b_layer_outliers; }
  const unsigned int n_pixel_outliers() const { return _n_pixel_outliers; }

  const unsigned int pixel_nholes() const { return _n_pixel_holes; }
  const unsigned int sct_nholes() const { return _n_sct_holes; }
  const unsigned int nholes() const { return _n_pixel_holes+_n_sct_holes; }
  const unsigned int nshared() const { return _n_pixel_shared+_n_sct_shared; }
  const int pixel_nshared() const { return _n_pixel_shared; }
  const int sct_nshared() const { return _n_sct_shared; }

  const unsigned long ftk_bitmask() const { assert(is_ftk()&&"ftk_bitmask is valid only for FTK tracks!"); return _ftk_bitmask; }
  // for majority road tracks, Guido states that the final chi^2 ==
  // the original chi^2. it is only for majority recovery tracks that
  // there is a difference in the two chi^2 values. the best test that
  // a track is either a majority road or a recovery is to check
  // nmisses.
  const bool ftk_is_majority_recovery() const { return( is_ftk() && std::abs(_chi2-_origchi2)>1E-4 ); }
  const bool ftk_is_majority_road() const { return( is_ftk() && _nmisses>0 && !ftk_is_majority_recovery() ); }
  const bool ftk_is_majority() const { return( is_ftk() && _nmisses>0 ); }
  const bool ftk_is_guido_category_3() const { 
    assert(is_ftk()&&"valid only for FTK tracks!");
    return( pixel_nhits()==2 && (detector::ftkMissedPlane(ftk_bitmask(),7)||
                                 detector::ftkMissedPlane(ftk_bitmask(),9)||
                                 detector::ftkMissedPlane(ftk_bitmask(),11)||
                                 detector::ftkMissedPlane(ftk_bitmask(),13)) );
  }
  const std::string ftk_bitmask_string() const { assert(is_ftk()&&"ftk_bitmask_string is valid only for FTK tracks!"); detector::ftk_bitmask_string(_ftk_bitmask); }
  const ftype delta_maj_chi2() const { return _origchi2-_chi2; }
  const std::vector<AnaTrack::ftype>& ftk_coords() const { assert(is_ftk()&&"ftk_coords is valid only for FTK tracks!"); return _ftk_coords; }
  const unsigned int ftk_bank() const { assert(is_ftk()&&"ftk_bank is valid only for FTK tracks!"); return _ftk_bank; }
  const unsigned int ftk_bank_id() const { assert(is_ftk()&&"ftk_bank_id is valid only for FTK tracks!"); return _ftk_bank_id; }
  const unsigned long ftk_road_id() const { assert(is_ftk()&&"ftk_road_id is valid only for FTK tracks!"); return _ftk_road_id; }
  const unsigned long ftk_bank_track_id() const { assert(is_ftk()&&"ftk_bank_track_id is valid only for FTK tracks!"); return _ftk_bank_track_id; }
  const int ftk_hf_rejected() const { assert(is_ftk()&&"ftk_hf_rejected is valid only for FTK tracks!"); return _ftk_hf_rejected; }
  const int ftk_hw_rejected() const { assert(is_ftk()&&"ftk_hw_rejected is valid only for FTK tracks!"); return _ftk_hw_rejected; }
  const int ftk_hw_track_id() const { assert(is_ftk()&&"ftk_hw_track_id is valid only for FTK tracks!"); return _ftk_hw_track_id; }
  const int ftk_getHWRejected() const { assert(is_ftk()&&"getHWRejected is valid only for FTK tracks!"); return _ftk_HW_rejected; }
  const int ftk_hole_number() const { 
    assert(is_ftk()&&"ftk_hole_number is valid only for FTK tracks!"); 
    if( nmisses()==0 ) { return -1; }
    if( _ftk_hole_number !=-1 ) { return _ftk_hole_number; }
    for( unsigned int iplane=0, fplane=detector::ftk_nplanes(); iplane!=fplane; ++iplane ) { 
      if( !detector::ftkHasPlane(ftk_bitmask(),iplane) ) {
        _ftk_hole_number = iplane;
        return _ftk_hole_number;
      }
    }
    std::cout << " warning: could not find ftk hole number!" << std::endl;
    _ftk_hole_number = 999;
    return _ftk_hole_number;
  }

  const std::vector< boost::shared_ptr<const AnaSiCluster> >& si_clusters() const { return _si_clusters; }
  const unsigned int si_clusters_pixel_logical_layers_present() const {
    std::bitset<3> has_layer(0);
    BOOST_FOREACH( boost::shared_ptr<const AnaSiCluster> cluster , _si_clusters ) { 
      if( !(cluster->is_pixel()) ) { continue; }
      assert( cluster->is_pixel() );
      has_layer.set( cluster->ftk_logical_layer() );
    }
    return has_layer.count();
  }
  const unsigned int si_clusters_sct_logical_layers_present() const {
    std::bitset<8> has_layer(0);
    BOOST_FOREACH( boost::shared_ptr<const AnaSiCluster> cluster , _si_clusters ) { 
      if( !(cluster->is_sct()) ) { continue; }
      assert( cluster->is_sct() );
      has_layer.set( cluster->ftk_logical_layer()-3 );
    }
    return has_layer.count();
  }
  const unsigned int si_clusters_logical_layers_present() const {
    std::bitset<11> has_layer(0);
    BOOST_FOREACH( boost::shared_ptr<const AnaSiCluster> cluster , _si_clusters ) { 
      has_layer.set( cluster->ftk_logical_layer() );
    }
    return has_layer.count();
  }

  const UniqueBarcode& barcode() const { return _barcode; }
  const bool is_hard_scatter_truth() const { 
    return (_barcode.event_index()==0 && _barcode.barcode() != 0 && _barcode.barcode() < 200000 ); }
  const bool is_pileup_truth() const { return _barcode.event_index()!=0; }
  const int pdg_code() const { return _pdg_code; }
  const detector::ParticleType unsigned_truth_particle_type() const { return detector::particle_type( pdg_code() ); }
  const int signed_truth_particle_type() const { return unsigned_truth_particle_type() * detector::sign_of( pdg_code() ); }
  const bool& detpaper() const { return _detpaper; }
  const ftype& begin_vertex_radius() const { return _beginvert_radius; }
  const ftype& begin_vertex_z() const { return _beginvert_z; }
  const ftype& end_vertex_radius() const { return _endvert_radius; }
  const ftype& barcode_matching_fraction() const { return _barcode_matching_fraction; }
  const ftype& barcode_fraction() const { return barcode_matching_fraction(); }
  const bool satisfies_old_parameter_matching_with( const boost::shared_ptr<const AnaTrack>& reference_track ) const {
    return( std::abs(phi0()-reference_track->phi0()) < 6e-3 &&
            std::abs(z0()-reference_track->z0()) < 0.8 &&
            std::abs(ct()-reference_track->ct()) < 8e-3 &&
            std::abs(curvature()-reference_track->curvature())/2.0 < 2e-5 );
  }
  const bool fails_old_parameter_matching_with( const boost::shared_ptr<const AnaTrack>& reference_track ) const { return !satisfies_old_parameter_matching_with(reference_track); }

  // return position.phi() at a radius of r_in_mm. false means phi is not uniquely determined by the given radius.
  const bool phi_position_at_r( const ftype& r_in_mm , ftype& phi ) const { return detector::phi_position_at_r(this,r_in_mm,phi); }
  // return position.phi() at z_in_mm. false means phi is not uniquely determined by the given z.
  const bool phi_position_at_z( const ftype& z_in_mm , ftype& phi ) const { return detector::phi_position_at_z(this,z_in_mm,phi); }

  const ftype delta_eta_phi( const AnaTrack& other ) const {
    return detector::delta_eta_phi( eta(), _phi0, other.eta(), other.phi0() );
  }

  struct ptLessThanV {
    bool operator()( const boost::shared_ptr<const AnaTrack>& a , const boost::shared_ptr<const AnaTrack>& b ) const { return a->pt()<b->pt(); }
  };
  struct matchingFracLessThanV {
    bool operator()( const boost::shared_ptr<const AnaTrack>& a , const boost::shared_ptr<const AnaTrack>& b ) const { return a->barcode_matching_fraction()<b->barcode_matching_fraction(); }
  };
  struct matchingFracGreaterThanV {
    bool operator()( const boost::shared_ptr<const AnaTrack>& a , const boost::shared_ptr<const AnaTrack>& b ) const { return a->barcode_matching_fraction()>b->barcode_matching_fraction(); }
  };
  struct matchingFracNHitsLessThanV {
    bool operator()( const boost::shared_ptr<const AnaTrack>& a , const boost::shared_ptr<const AnaTrack>& b ) const { 
      const ftype fraca = a->barcode_matching_fraction();
      const ftype fracb = b->barcode_matching_fraction();
      const bool lessfrac = fraca<fracb;
      if( !lessfrac && fraca==fracb ) { return a->nhits()<b->nhits(); }
      return lessfrac; 
    }
  };
  class parametersEqualV {
  protected:
    boost::shared_ptr<const AnaTrack> _comp;
  public:
    typedef bool result_type;
    parametersEqualV() : _comp() {}
    parametersEqualV(const boost::shared_ptr<const AnaTrack>& comp) : _comp(comp) {}
    bool operator()( const boost::shared_ptr<const AnaTrack>& a , const boost::shared_ptr<const AnaTrack>& b ) const { 
      return( (std::cos(a->phi0()-b->phi0())>0.9999) && 
              std::abs(a->d0()-b->d0())<0.001 &&
              std::abs(a->z0()-b->z0())<0.001 &&
              std::abs(a->pt()-b->pt())<0.01 &&
              std::abs(a->eta()-b->eta())<0.01 );
    }
    bool operator()( const boost::shared_ptr<const AnaTrack>& a ) const { 
      if( !_comp ) { return false; }
      return operator()( a , _comp );
    }
  };
  class trackIsEqualV {
  protected:
    boost::shared_ptr<const AnaTrack> _comp;
  public:
    typedef bool result_type;
    trackIsEqualV() : _comp() {}
    trackIsEqualV( const boost::shared_ptr<const AnaTrack>& comp ) : _comp(comp) {}
    bool operator()( const boost::shared_ptr<const AnaTrack>& a , const boost::shared_ptr<const AnaTrack>& b ) const { 
      static parametersEqualV parV;
      return( parV(a,b) &&
              ( (std::abs(a->barcode().event_index()-b->barcode().event_index())<1) /*||
                (a->barcode().event_index()==0 && b->barcode().event_index()==-1) ||
                (a->barcode().event_index()==-1 && b->barcode().event_index()==0) */) && 
              std::abs(a->barcode().barcode()-b->barcode().barcode())<1 );
    }
    bool operator()( const boost::shared_ptr<const AnaTrack>& a ) const { 
      if( !_comp ) { return false; }
      return operator()( a , _comp );
    }
  };
  

  std::ostream& print( std::ostream& os ) const {
    if( is_truth() ) { 
      os << ( boost::format("AnaTrack: %|10| pt=%|+10|(GeV) phi0=%|10| eta=%|14| d0=%|10| z0=%|10| ei=%|10| bc=%|10| bf=%|10| pm=%|10|") % algorithm().name() 
              % pt() % phi0() % eta() % d0() % z0() % _barcode.event_index() % _barcode.barcode() % barcode_matching_fraction() % _truth_parent_bitmask.to_ulong()
              )
         << std::endl;
    } else if( is_ftk() ) {
      std::bitset<14> mask( _ftk_bitmask );
      os << ( boost::format("AnaTrack: %|10| pt=%|+10|(GeV) phi0=%|10| eta=%|14| d0=%|10| z0=%|10| " ) % algorithm().name() 
              % pt() % phi0() % eta() % d0() % z0() )
         << ( boost::format("ei=%|10| bc=%|10| bf=%|10| nh=%|10| ") 
              % _barcode.event_index() % _barcode.barcode() % barcode_matching_fraction() % nhits() ) 
         << mask
         << ( boost::format( "bid=%|10| rid=%|10| x2=%|10| df=%|10|" ) % ftk_bank_id() % ftk_road_id() % chi2() % ndof() )
         << std::endl;
    } else {
      os << ( boost::format("AnaTrack: %|10| pt=%|+10|(GeV) phi0=%|10| eta=%|14| d0=%|10| z0=%|10| ei=%|10| bc=%|10| bf=%|10| nh=%|10| x2r=%|10|") % algorithm().name() 
              % pt() % phi0() % eta() % d0() % z0() % _barcode.event_index() % _barcode.barcode() % barcode_matching_fraction() % nhits() % chi2ndof()
              )
         << std::endl;
      if( !_si_clusters.empty() ) {
        BOOST_FOREACH( boost::shared_ptr<const AnaSiCluster> cluster , _si_clusters ) {
          cluster->print( std::cout );
        }
      }
    }
    return os;
  }
  std::ostream& print() const {
    return print( std::cout );
  }

};

BOOST_CLASS_VERSION( AnaTrack , 4 );

#endif // WRAP_ANATRACK_HPP
