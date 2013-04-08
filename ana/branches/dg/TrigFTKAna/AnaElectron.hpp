#ifndef WRAP_ANAELECTRON_HPP
#define WRAP_ANAELECTRON_HPP

// ANAELECTRON
// ================================================================
// Data for a single electron.
// ================================================================
// 2009-07-15 Antonio Boveia (boveia@hep.uchicago.edu)

#include <numeric>
#include <cmath>
#include <map>
#include <string>
#include <iostream>
#include <boost/format.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/version.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include "TrigFTKAna/AnaParticle.hpp"
#include "TrigFTKAna/AnaTrack.hpp"
#include "TrigFTKAna/Detector.hpp"
#include "TrigFTKAna/egamma/egammaPIDdefsV17.h"
#include "TrigFTKAna/Conference.hpp"

class
AnaElectron : public AnaParticle
{
public:
  typedef AnaParticle::ftype ftype;
  typedef enum { DEFAULT=0 , TRACK=1 } ElectronType;
  typedef enum { CONE10=0 , CONE20=1, CONE30=2, CONE40=3 , NSIZES=4 } ConeSize;
  typedef enum { PIXEL=0 , SCT=1 , BL=2 , TRT=3 , NTYPES=4 } HitType;
protected:

  ElectronType _ele_type;
  int _author;
  ftype _charge;
  bool _tight;
  ftype _trackd0pv; // d0 from primary vertex
  ftype _trackz0pv; // z0 from primary vertex
  ftype _trackerrd0pvunbiased; // err d0 from primary vertex
  ftype _trackerrd0pv; //  errd0 from primary vertex
  ftype _trackerrd0pv_ratio; //  d0/errd0 from primary vertex
  unsigned int _is_em; // is em bitmask
  unsigned int _oq; // ]e/gamma quality
  unsigned int _aod_quality; // object quality flag from Peter's ntuple (3 bits, highest to lowest order tight/medium/loose)
  ftype _rhad1; // had in layer 1 / em
  ftype _rhad; // had/em
  ftype _reta; // ratio in eta of cell energies in 3x7 versus 7x7 cells
  ftype _weta2; // lateral shower width
  ftype _wstot; // total shower width
  ftype _deltaeta; // delta eta between cluster and track
  ftype _deltaphi; // delta phi between cluster and track
  ftype _eoverp; // ratio of cluster energy to track momentum
  ftype _demaxs1; // max (E1-E2)/sum
  ftype _eta2; // eta from second sampling measurement
  ftype _etap; // pointing eta reconstructed from the cluseter (1st and 2nd sampling)
  ftype _et2; // et from second sampling eta measurement
  ftype _met_wet;
  ftype _met_wpx;
  ftype _met_wpy;
  ftype _met_status;
  boost::optional<ftype> _z0_exPV;
  boost::optional<ftype> _d0_exPV;
  boost::optional<ftype> _d0_exPV_err;
  boost::optional<ftype> _d0_vx;
  boost::optional<ftype> _d0_vx_err;
  boost::optional<ftype> _z0_vx;
  int _trigmatch; // old trigger matching
  int _ntuple_index;
  std::vector<ftype> _pt_cone; // inner detector pt in a given cone
  std::vector<ftype> _et_cone; // calo et in a given cone
  std::vector<ftype> _uncorrected_et_cone;
  std::vector<int> _nhits; // numbers of hits
  std::vector<int> _noutliers; // numbers of outliers
  short _expect_b_layer;
  TLorentzVector _cluster_momentum;
  TLorentzVector _track_momentum;
  bool _use_cluster_energy_particle_angle;
  boost::shared_ptr<const AnaTrack> _id_track;
  mutable bool _passes_otx_check;
  mutable bool _passes_lar_check;
  mutable TLorentzVector _cluster_energy_particle_angle;
  mutable boost::shared_ptr<const AnaElectron> _uncorrected_parent;
  mutable std::map< std::string , ftype > _trigger_match_dr; // peter's trigger matching
  friend class NtD3PDphysWZ;
  friend class NtD3PDphysEW;
  friend class NtD3PDphysTop;
  friend class NtD3PDphysSingleTop;
  friend class NtD3PDphysPeter;
  friend class NtD3PDphysJetMET;
  friend class NtPGS;

private:
  friend class boost::serialization::access;
  template<class archiveT>
  void serialize( archiveT& ar, const unsigned int version ) {
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(AnaParticle);
    if( version>=4 ) { ar & boost::serialization::make_nvp("ele_type",_ele_type); }
    ar & boost::serialization::make_nvp("author",_author); 
    ar & boost::serialization::make_nvp("charge",_charge);
    ar & boost::serialization::make_nvp("tight",_tight);
    ar & boost::serialization::make_nvp("trackd0pv",_trackd0pv);
    ar & boost::serialization::make_nvp("trackz0pv",_trackz0pv);
    ar & boost::serialization::make_nvp("trackerrd0pvunbiased",_trackerrd0pvunbiased);
    ar & boost::serialization::make_nvp("trackerrd0pv",_trackerrd0pv);
    ar & boost::serialization::make_nvp("is_em",_is_em);
    if( version<5 ) { 
      bool tmp;
      ar & boost::serialization::make_nvp("is_em_robustMedium",tmp);
      ar & boost::serialization::make_nvp("is_em_robusterTight",tmp);
      ar & boost::serialization::make_nvp("is_em_robustLoose_v16",tmp);
      ar & boost::serialization::make_nvp("is_em_robustMedium_v16",tmp);
      ar & boost::serialization::make_nvp("is_em_tight_v16",tmp);
      ar & boost::serialization::make_nvp("is_em_QCD_v16",tmp);
    }
    if( version>=5 ) {
      ar & boost::serialization::make_nvp("rhad1",_rhad1);
      ar & boost::serialization::make_nvp("rhad",_rhad);
      ar & boost::serialization::make_nvp("reta",_reta);
      ar & boost::serialization::make_nvp("weta2",_weta2);
      ar & boost::serialization::make_nvp("wstot",_wstot);
      ar & boost::serialization::make_nvp("deltaeta",_deltaeta);
      ar & boost::serialization::make_nvp("deltaphi",_deltaphi);
      ar & boost::serialization::make_nvp("eoverp",_eoverp);
      ar & boost::serialization::make_nvp("demaxs1",_demaxs1);
      ar & boost::serialization::make_nvp("eta2",_eta2);
      ar & boost::serialization::make_nvp("etap",_etap);
      ar & boost::serialization::make_nvp("et2",_et2);
      ar & boost::serialization::make_nvp("met_wet",_met_wet);
      ar & boost::serialization::make_nvp("met_wpx",_met_wpx);
      ar & boost::serialization::make_nvp("met_wpy",_met_wpy);
      ar & boost::serialization::make_nvp("met_status",_met_status);
      ar & boost::serialization::make_nvp("oq",_oq);
      ar & boost::serialization::make_nvp("aod_quality",_aod_quality);
      ar & boost::serialization::make_nvp("z0_exPV",_z0_exPV);
      ar & boost::serialization::make_nvp("d0_exPV",_d0_exPV);
      ar & boost::serialization::make_nvp("d0_exPV_err",_d0_exPV_err);
      ar & boost::serialization::make_nvp("d0_vx",_d0_vx);
      ar & boost::serialization::make_nvp("d0_vx_err",_d0_vx_err);
      ar & boost::serialization::make_nvp("z0_vx",_z0_vx);
      ar & boost::serialization::make_nvp("trigmatch",_trigmatch);
      ar & boost::serialization::make_nvp("ntuple_index",_ntuple_index);
      ar & boost::serialization::make_nvp("trigger_match_dr",_trigger_match_dr);
    }
    ar & boost::serialization::make_nvp("pt_cone",_pt_cone);
    ar & boost::serialization::make_nvp("et_cone",_et_cone);
    ar & boost::serialization::make_nvp("uncorrected_et_cone",_uncorrected_et_cone);
    ar & boost::serialization::make_nvp("cluster_momentum",_cluster_momentum);
    ar & boost::serialization::make_nvp("track_momentum",_track_momentum);
    ar & boost::serialization::make_nvp("passes_otx_check",_passes_otx_check);
    ar & boost::serialization::make_nvp("passes_lar_check",_passes_lar_check);
    ar & boost::serialization::make_nvp("use_cluster_energy_particle_angle",_use_cluster_energy_particle_angle);
    ar & boost::serialization::make_nvp("nhits",_nhits);
    ar & boost::serialization::make_nvp("noutliers",_noutliers);
    ar & boost::serialization::make_nvp("expect_b_layer",_expect_b_layer);
    if( version>=2 ) {
      boost::shared_ptr<AnaTrack> cid_track( boost::const_pointer_cast<AnaTrack>( _id_track ) );
      ar & boost::serialization::make_nvp("id_track",cid_track);
      _id_track = cid_track;
    }
    _cache_uuid = 0l;
  }
private:
  // cached computations
  void _update_cache() const;
  mutable long _cache_uuid;
public:

  AnaElectron()
    : _ele_type( AnaElectron::DEFAULT )
    , _author(0)
    , _charge(0.)
    , _tight(0.)
    , _trackd0pv(0.)
    , _trackz0pv(0.)
    , _trackerrd0pvunbiased(0.)
    , _trackerrd0pv(0.)
    , _is_em( 0u )
    , _oq( 0u )
    , _aod_quality( 0u )
    , _rhad1( std::numeric_limits<AnaElectron::ftype>::max() )
    , _rhad( std::numeric_limits<AnaElectron::ftype>::max() )
    , _reta( std::numeric_limits<AnaElectron::ftype>::max() )
    , _weta2( std::numeric_limits<AnaElectron::ftype>::max() )
    , _wstot( std::numeric_limits<AnaElectron::ftype>::max() )
    , _deltaeta( std::numeric_limits<AnaElectron::ftype>::max() )
    , _deltaphi( std::numeric_limits<AnaElectron::ftype>::max() )
    , _eoverp( std::numeric_limits<AnaElectron::ftype>::max() )
    , _demaxs1( std::numeric_limits<AnaElectron::ftype>::max() )
    , _eta2( std::numeric_limits<AnaElectron::ftype>::max() )
    , _etap( std::numeric_limits<AnaElectron::ftype>::max() )
    , _et2( std::numeric_limits<AnaElectron::ftype>::max() )
    , _met_wet( 0. )
    , _met_wpx( 0. )
    , _met_wpy( 0. )
    , _met_status( 0. )
    , _z0_exPV()
    , _d0_exPV()
    , _d0_exPV_err()
    , _d0_vx()
    , _d0_vx_err()
    , _z0_vx()
    , _ntuple_index(-1)
    , _pt_cone()
    , _et_cone()
    , _uncorrected_et_cone()
    , _nhits()
    , _noutliers()
    , _expect_b_layer( 0 )
    , _cluster_momentum()
    , _track_momentum()
    , _use_cluster_energy_particle_angle( true )
    , _id_track()
    , _passes_otx_check( false ) // default: check hasn't been performed, so false
    , _passes_lar_check( false ) // default: check hasn't been performed, so false
    , _cluster_energy_particle_angle()
    , _cache_uuid( 0l )
    , _uncorrected_parent()
    , _trigger_match_dr()
    , AnaParticle( AnaParticle::ELECTRON )
  {}
  virtual ~AnaElectron() {}
  static const boost::shared_ptr<const AnaElectron> make_track_electron( const boost::shared_ptr<const AnaTrack>& track ) {
    boost::shared_ptr<AnaElectron> ele( new AnaElectron );
    ele->_ele_type = AnaElectron::TRACK;
    ele->_author = 0;
    ele->_charge = track->charge();
    ele->_tight = 0;
    ele->_trackd0pv = track->exPV_d0();
    ele->_trackz0pv = track->exPV_z0();
    ele->_trackerrd0pv = track->exPV_d0_signif();
    ele->_momentum = track->momentum();
    ele->_is_em = 0;
    ele->_oq = 0;
    ele->_aod_quality = 0;
    ele->_rhad1 = std::numeric_limits<AnaElectron::ftype>::max() ;
    ele->_rhad = std::numeric_limits<AnaElectron::ftype>::max() ;
    ele->_reta = std::numeric_limits<AnaElectron::ftype>::max() ;
    ele->_weta2 = std::numeric_limits<AnaElectron::ftype>::max() ;
    ele->_wstot = std::numeric_limits<AnaElectron::ftype>::max() ;
    ele->_deltaeta = std::numeric_limits<AnaElectron::ftype>::max() ;
    ele->_deltaphi = std::numeric_limits<AnaElectron::ftype>::max() ;
    ele->_eoverp = std::numeric_limits<AnaElectron::ftype>::max() ;
    ele->_demaxs1 = std::numeric_limits<AnaElectron::ftype>::max() ;
    ele->_eta2 = std::numeric_limits<AnaElectron::ftype>::max() ;
    ele->_etap = std::numeric_limits<AnaElectron::ftype>::max() ;
    ele->_et2 = 0;
    ele->_met_wet = 0;
    ele->_met_wpx = 0;
    ele->_met_wpy = 0;
    ele->_met_status = 0;
    ele->_z0_exPV.reset();
    ele->_d0_exPV.reset();
    ele->_d0_exPV_err.reset();
    ele->_pt_cone = boost::assign::list_of( 999999. )( 999999. )( 999999. )( 999999. ); // not computed.
    ele->_et_cone = boost::assign::list_of( 999999. )( 999999. )( 999999. )( 999999. ); // not computed.
    ele->_uncorrected_et_cone = boost::assign::list_of( 999999. )( 999999. )( 999999. )( 999999. ); // not computed.
    ele->_cluster_momentum = ele->_momentum;
    ele->_nhits = ( boost::assign::list_of( track->pixel_nhits() )
                    (track->sct_nhits())(track->b_layer_nhits())
                    (track->trt_nhits()) );
    ele->_noutliers = ( boost::assign::list_of( track->n_pixel_outliers() )( 0 )( track->n_b_layer_outliers() )( 0 ) );
    ele->_expect_b_layer = track->expect_b_layer_hits();
    ele->_id_track = track;
    return boost::const_pointer_cast<const AnaElectron>(ele);
  }
  static const boost::shared_ptr<const AnaElectron> make_particle_electron( const boost::shared_ptr<const AnaParticle>& p ) {
    boost::shared_ptr<AnaElectron> ele( new AnaElectron );
    ele->_momentum = p->momentum();
    ele->_cluster_momentum = ele->_momentum;
    return boost::const_pointer_cast<const AnaElectron>(ele);
  }
  
  const bool use_cluster_energy_and_particle_angle() const { return _use_cluster_energy_particle_angle; }
  void use_cluster_energy_and_particle_angle( const bool& yes ) { _use_cluster_energy_particle_angle = yes; }

  virtual const ftype charge() const { return _charge; }
  const short scharge() const __attribute__((deprecated)) { return static_cast<short>(_charge); }

  virtual const TLorentzVector& four_vector_for_composites() const { 
    if( _use_cluster_energy_particle_angle ) {
      _update_cache();
      return _cluster_energy_particle_angle;
    } else {
      return four_vector(); 
    }
  }
  const TLorentzVector& cluster_momentum() const { return _cluster_momentum; }
  const ftype cluster_eta() const { return _cluster_momentum.Eta(); }
  const ftype absclustereta() const { return std::abs(_cluster_momentum.Eta());   }
  const ftype cluster_phi() const { return _cluster_momentum.Phi(); }
  const ftype cluster_pt() const { return _cluster_momentum.Pt(); }

  const TLorentzVector& track_momentum() const { return _track_momentum; }
  const ftype track_eta() const { return _track_momentum.Eta(); }
  const ftype track_phi() const { return _track_momentum.Phi(); }
  const ftype track_pt() const { return _track_momentum.Pt(); }
  const ftype trackd0pv() const { return _trackd0pv; }
  const ftype trackz0pv() const { return _trackz0pv; }
  const ftype trackerrd0pvunbiased() const { return _trackerrd0pvunbiased; }
  const ftype trackerrd0pv() const { return _trackerrd0pv; }
  const ftype trackerrd0pv_abs_signif() const { return std::abs(detector::safe_divide( trackd0pv(), trackerrd0pv() )); }

  const ftype met_wet() const { return _met_wet; }
  const ftype met_wpx() const { return _met_wpx; }
  const ftype met_wpy() const { return _met_wpy; }
  const ftype met_status() const { return _met_status; }

  const ftype et_from_track_angle() const {
    //pt = el_nSiHits>=4 ? el_cl_E/cosh(el_tracketa) : el_cl_pt ; following egamma prescription for electron pt, eta, phi
    //return detector::safe_divide( static_cast<ftype>(_cluster_momentum.E()) , std::cosh(eta()));
    // return( cluster_momentum().E() * std::sin( detector::thetaFromEta(eta()) ) );
    /*    if( has_track()) 
        cout << "track / cluster" << _cluster_momentum.Pt() << " " << _id_track->Pt() << endl;
    */
    if( !(has_track()) ) { return _cluster_momentum.Pt(); }
    const unsigned int& npix = _id_track->pixel_nhits();
    const unsigned int& nsi = npix + _id_track->sct_nhits();
    if ( nsi < 4 )
      return _cluster_momentum.Pt();
    else
      return detector::safe_divide( static_cast<ftype>(_cluster_momentum.E()) , std::cosh(track_eta()));    
  }

  const ElectronType& electron_type() const { return _ele_type; }
  const int author() const { return _author; }
  const unsigned int is_em() const { return _is_em; }
  const bool is_good_oq() const { return !( _oq & 1446 ); }
  const unsigned short int aod_quality() const { return _aod_quality; }
  const bool is_tight() const { return( _tight==1 ); }
  const bool is_aod_tight() const { return( _aod_quality & (1<<2) ); }
  const bool is_aod_tight_plusplus() const { return( _aod_quality & (1<<5) ); } // IS THIS RIGHT?
  const bool is_aod_medium() const { return( _aod_quality & (1<<1) ); }
  const bool is_aod_medium_plusplus() const { return( _aod_quality & (1<<4) ); }
  const bool is_aod_loose() const { return( _aod_quality & (1<<0) ); }

  // like egammaEvent/src/egamma.cxx : "return (m_egPID->isEM(mask, pid)==0);" where mask = egammaPID::ElectronLoose, etc.
  // https://twiki.cern.ch/twiki/bin/view/AtlasProtected/IsEMIdentification#2_1_2011_data_analyses_with_rele
  // release 17
  const bool is_ElectronLoose_v17() const { return( (egammaPIDv17::ElectronIDLoose & _is_em )==0 ); }
  const bool is_ElectronMedium_v17() const { return( (egammaPIDv17::ElectronIDMedium & _is_em )==0 ); }
  const bool is_ElectronTight_v17() const { return( (egammaPIDv17::ElectronIDTight & _is_em )==0 ); }
  const bool is_ElectronLoosePP_v17() const { return( (egammaPIDv17::ElectronIDLoosePP & _is_em )==0 ); }
  const bool is_ElectronMediumPP_v17() const { return( (egammaPIDv17::ElectronIDMediumPP & _is_em )==0 ); }
  const bool is_ElectronTightPP_v17() const { return( (egammaPIDv17::ElectronIDTightPP & _is_em )==0 ); }

  const bool isBadElectron_FEBhole() const {
    float eta = cluster_eta();
    float phi = cluster_phi();
    //    cout << "eta, phi: " << eta << " " << phi << endl;
    //    cout << "passed: " << pass_lar() << endl;
    if ( pass_lar() ) return false;
    return true;
  }

  const bool isBadElectron_FEB() const {
    //bool passEls(double eta, double phi) { return !((eta < 1.55 && eta > -0.1) && (phi < -0.492 && phi > -0.888)); }
    float dr = 0.1;

    float eta = cluster_eta();
    float phi = cluster_phi();
    
    if ((eta < (1.45+dr) && eta > (0-dr)) && (phi < (-0.592+dr) && phi > (-0.788-dr)))
      return true;

    return false;
  }

  const bool is_PhotonLoose() const { return( (_is_em & egammaPIDv17::PhotonLoose)==0 ); }

  const ftype rhad1() const { return _rhad1; }
  const ftype rhad() const { return _rhad; }
  const ftype reta() const { return _reta; }
  const ftype weta2() const { return _weta2; }
  const ftype wstot() const { return _wstot; }
  const ftype deltaeta() const { return _deltaeta; }
  const ftype deltaphi() const { return _deltaphi; }
  const ftype eoverp() const { return _eoverp; }
  const ftype demaxs1() const { return _demaxs1; }
  const ftype eta2() const { return _eta2; }
  const ftype etap() const { return _etap; }
  const ftype et2() const { return _et2; }
  
  // extrapolated to beam axis
  const ftype z0_exPV() const { 
    //    std::cout << "z0_expv: " << *_z0_exPV << std::endl;
    return _z0_exPV ? *_z0_exPV: 0; }
  const ftype d0_exPV() const { return _d0_exPV ? *_d0_exPV: 0; }
  const ftype d0_exPV_err() const { return _d0_exPV_err ? *_d0_exPV_err : std::numeric_limits<ftype>::max(); }
  const ftype d0_exPV_signif() const { return (_d0_exPV && _d0_exPV_err) ? detector::safe_divide( *_d0_exPV , *_d0_exPV_err ) : std::numeric_limits<ftype>::max(); }
  const ftype abs_d0_exPV_signif() const { return (_d0_exPV && _d0_exPV_err) ? std::abs( detector::safe_divide( *_d0_exPV , *_d0_exPV_err ) ) : std::numeric_limits<ftype>::max(); }

  const ftype d0_vx() const { return _d0_vx ? *_d0_vx : std::numeric_limits<ftype>::max(); }
  const ftype d0_vx_err() const { return _d0_vx_err ? *_d0_vx_err : std::numeric_limits<ftype>::max(); }
  const ftype d0_vx_signif() const { return (_d0_vx && _d0_vx_err) ? detector::safe_divide( *_d0_vx , *_d0_vx_err ) : std::numeric_limits<ftype>::max(); }
  const ftype z0_vx() const { return _z0_vx ? *_z0_vx : std::numeric_limits<ftype>::max(); }

  const ftype pt_cone( const ConeSize& cone ) const { return _pt_cone[cone]; }
  const ftype pt_cone_ratio( const ConeSize& cone ) const { return detector::safe_divide( pt_cone(cone) , pt() ); }
  const ftype pt_cone_ratio_from_track_angle( const ConeSize& cone ) const { 
    if (pt_cone(cone)==-0.999) 
      return 1;
    else
      return std::abs(detector::safe_divide( pt_cone(cone) , et_from_track_angle() )); 
  }
  const ftype et_cone( const ConeSize& cone ) const { return _et_cone[cone]; }
  const ftype et_cone_ratio( const ConeSize& cone ) const { return detector::safe_divide( et_cone(cone) , pt() ); }
  const ftype cluster_et_cone_ratio( const ConeSize& cone ) const { return detector::safe_divide( et_cone(cone) , cluster_pt() ); }
  const ftype et_cone_ratio_from_track_angle( const ConeSize& cone ) const {     
    if (et_cone(cone)==-0.999) 
      return 1;
    else
      return std::abs(detector::safe_divide( et_cone(cone) , et_from_track_angle() )); 
  }
  const ftype uncorrected_et_cone( const ConeSize& cone ) const { return _uncorrected_et_cone[cone]; }
  const ftype uncorrected_et_cone_ratio( const ConeSize& cone ) const { return detector::safe_divide( uncorrected_et_cone(cone) , pt() ); }
  const ftype uncorrected_cluster_et_cone_ratio( const ConeSize& cone ) const { return detector::safe_divide( uncorrected_et_cone(cone) , cluster_pt() ); }
  const ftype uncorrected_et_cone_ratio_from_track_angle( const ConeSize& cone ) const {     
    if (uncorrected_et_cone(cone)==-0.999) 
      return 1;
    else
      return std::abs(detector::safe_divide( uncorrected_et_cone(cone) , et_from_track_angle() )); 
  }



  const int nhits( const HitType& hit ) const { assert(_nhits.size()>hit); return _nhits[hit]; }
  const int noutliers( const HitType& hit ) const { assert(_noutliers.size()>hit); return _noutliers[hit]; }

  const int ntuple_index() const { return _ntuple_index; }

  virtual const bool has_track() const { return _id_track; }
  virtual boost::shared_ptr<const AnaTrack> track() const { return _id_track; } // may return null!
  virtual const ftype track_exPV_d0() const { return( _id_track ? _id_track->exPV_d0() : std::numeric_limits<ftype>::max() ); }
  virtual const ftype track_exPV_d0_err() const { return( _id_track ? _id_track->exPV_d0_err() : std::numeric_limits<ftype>::max() ); }
  virtual const ftype track_exPV_d0_signif() const { return( _id_track ? _id_track->exPV_d0_signif() : std::numeric_limits<ftype>::max() ); }
  virtual const ftype track_exPV_abs_d0_signif() const { return( _id_track ? std::abs( _id_track->exPV_d0_signif() ) : std::numeric_limits<ftype>::max() ); }
  virtual const ftype track_exPV_z0() const { return( _id_track ? _id_track->exPV_z0() : std::numeric_limits<ftype>::max() ); }

  const bool expect_b_layer() const { return( _id_track ? _id_track->expect_b_layer_hits() : false ); }

  const short expect_b_layer_vlq() const { return _expect_b_layer; }
  const int expect_b_layer_vlq_flag() const {
    int nhitcandidates = _nhits[BL] + _noutliers[BL];
    if( nhitcandidates >= 1 ) return nhitcandidates;
    if( _expect_b_layer != 1 ) return -1;
    return 0;
  }

  const bool pass_otx() const { _update_cache(); return _passes_otx_check; }
  const bool pass_lar() const { _update_cache(); return _passes_lar_check; }

  const bool is_corrected() const { return _uncorrected_parent; }
  const boost::shared_ptr<const AnaElectron> uncorrected_parent() const { return _uncorrected_parent; }

  // generic smear/correct/scale functions
  static boost::shared_ptr<const AnaElectron> egamma_smeared( const CONF::ConfType& conf , 
							      const boost::shared_ptr<const AnaElectron>& electron , 
							      const unsigned long& event_number , const size_t& electron_number , 
							      const std::string& fun );
  static boost::shared_ptr<const AnaElectron> egamma_corrected( const CONF::ConfType& conf , 
								const boost::shared_ptr<const AnaElectron>& electron , 
								const unsigned long& event_number , const size_t& electron_number , 
								const std::string& fun );
  static void egamma_effscale( const CONF::ConfType& conf ,
			       const boost::shared_ptr<const AnaElectron>& electron ,
			       float& eff , float& error );

  // templated workhorse function to apply rel. 17 smearing to electrons
  template <typename SMEARCLASS>
  static boost::shared_ptr<const AnaElectron>
  egamma_smeared_apply_v17( const boost::shared_ptr<const AnaElectron>& electron ,
			    const unsigned long& event_number , const size_t& electron_number ,
			    const bool& with_CT , const std::string& fun = "NOMINAL" ) {
    static SMEARCLASS rescaler;
    static bool rescaler_init = false;
    if( !rescaler_init ) {
      rescaler.useDefaultCalibConstants( "2011" );
      rescaler_init = true;
    }
    if( true ) {
      // some useful debugging output
      if( electron->et() == 0. || 
	  electron->cluster_pt() == 0. ||
	  fabs(electron->eta()) > 99999. ||
	  fabs(electron->cluster_eta()) > 99999. ) {
	std::cout << "WARNING :: found bad electron in event " << event_number << " ::" 
		  << " et = " << electron->et() << " , cluster_et = " << electron->cluster_pt() 
		  << " , eta = " << electron->eta() << " , cluster_eta = " << electron->cluster_eta() << std::endl;
      }
    }
    // first do transition region calibration
    double et_after_crack = electron->et() * rescaler.applyMCCalibrationGeV( electron->cluster_eta() , electron->et() , "ELECTRON" );
    double e_after_crack = detector::safe_divide( et_after_crack , double(sin(electron->theta())) );
    // next do smearing
    // NOTE: if you are using Atlfast MC, you also need to do an Atlfast->Geant4 correction that is not done here!
    //       https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/EnergyScaleResolutionRecommendations#2012_Moriond_Recommendations_201
    int smear_direction;
    int scale_direction = 0;
    if( fun == "NOMINAL" ) { smear_direction = 0; }
    else if( fun == "SMEARDOWN" ) { smear_direction = 1; }
    else if( fun == "SMEARUP" ) { smear_direction = 2; }
    // the scale_directions are reversed here since we are applying a data correction to MC
    else if( fun == "SCALEDOWN" ) { smear_direction = 0; scale_direction = 2; }
    else if( fun == "SCALEUP" ) { smear_direction = 0; scale_direction = 1; }
    else assert(0&&"fun not understood by egamma_smeared_apply_v17");
    double e_final = e_after_crack * rescaler.getSmearingCorrectionGeV( electron->cluster_eta() , e_after_crack , smear_direction , with_CT );

    // also apply scale if desired for evaluating EES uncertainty
    if( scale_direction != 0 ) {
      double et_final = e_final * sin(electron->theta());
      double e_nom_scale = rescaler.applyEnergyCorrectionGeV( electron->cluster_eta() , electron->cluster_phi() , e_final , et_final , 0 , "ELECTRON" );
      double e_dir_scale = rescaler.applyEnergyCorrectionGeV( electron->cluster_eta() , electron->cluster_phi() , e_final , et_final , scale_direction , "ELECTRON" );
      // the fraction here might appear to be upside-down, but it is done this way on purpose since
      // we are applying a data scale factor to MC
      e_final = e_final * detector::safe_divide(e_nom_scale,e_dir_scale);
    }
    
    boost::shared_ptr<AnaElectron> new_electron( boost::make_shared<AnaElectron>(*electron) );
    new_electron->_momentum.SetPtEtaPhiM( e_final/cosh( electron->eta() ) , 
					  electron->eta() , 
					  electron->phi() , 
					  detector::electron_mass(detector::GeV) );
    new_electron->_uncorrected_parent = electron;
    return boost::const_pointer_cast<const AnaElectron>( new_electron );
  }

  // templated workhorse function to apply rel. 17 corrections to electrons
  template <typename SMEARCLASS>
  static boost::shared_ptr<const AnaElectron>
  egamma_corrected_apply_v17( const boost::shared_ptr<const AnaElectron>& electron ,
			      const unsigned long& event_number , const size_t& electron_number ,
			      const std::string& fun = "NOMINAL" ) {
    static SMEARCLASS rescaler;
    static bool rescaler_init = false;
    if( !rescaler_init ) {
      rescaler.useDefaultCalibConstants( "2011" );
      rescaler_init = true;
    }
    // first do scale correction
    int scale_direction;
    if( fun == "NOMINAL" ) scale_direction = 0;
    else if( fun == "DOWN" ) scale_direction = 1;
    else if( fun == "UP" ) scale_direction = 2;
    else assert(0&&"fun not understood by egamma_corrected_apply_v17");
    double e_after_scale = rescaler.applyEnergyCorrectionGeV( electron->cluster_eta() , electron->cluster_phi() , electron->energy() , electron->et() , scale_direction , "ELECTRON" );
    double et_after_scale = e_after_scale * sin(electron->theta());
    // next do transition region calibration
    double et_after_crack = et_after_scale * rescaler.applyMCCalibrationGeV( electron->cluster_eta() , et_after_scale , "ELECTRON" );
    boost::shared_ptr<AnaElectron> new_electron( boost::make_shared<AnaElectron>(*electron) );
    new_electron->_momentum.SetPtEtaPhiM( et_after_crack ,
					  electron->eta() ,
					  electron->phi() ,
					  detector::electron_mass(detector::GeV) );
    new_electron->_uncorrected_parent = electron;
    return boost::const_pointer_cast<const AnaElectron>( new_electron );
  }

  // templated workhorse function to set reconstruction efficiency for rel. 17 electrons
  template <typename EFFCLASS>
  static void
  egamma_effscale_apply_v17( const boost::shared_ptr<const AnaElectron>& electron ,
			     float& eff , float& error ) {
    static EFFCLASS egammasf;
    // The 4 tells egammasf to use recon efficiencies and not trigger efficiencies
    // The 0 is the range, it has no effect in v17
    // The 6 is for G4 MC (use 7 for AtlFast MC)
    // The true tells the class to use Et
    std::pair< float , float > sf;
    sf = egammasf.scaleFactor( fabs(electron->cluster_eta()) < 2.47 ? electron->cluster_eta() : 2.46 * detector::sign_of(electron->cluster_eta()) , // function errors if |eta|>2.47
			       electron->pt()*1000. , // MeV
			       4 , 0 , 6 , true );
    eff = sf.first;
    error = sf.second;
  }

  static boost::shared_ptr<const AnaElectron> corrected_electron_2011( const boost::shared_ptr<const AnaElectron>& ele , 
                                                                       const unsigned long& event_number , 
                                                                       const size_t& elec_number ,
                                                                       const int& corrtype = 0 );
  static boost::shared_ptr<const AnaElectron> smeared_electron_2011( const boost::shared_ptr<const AnaElectron>& ele ,
                                                                     const unsigned long& event_number , 
                                                                     const size_t& elec_number ,
                                                                     const int& corrtype = 0 ,
								     const bool& les_uncert = false );


  const bool vlq_trigger_matched( const unsigned long& run_number ) const {
    if( run_number < 186873 ) {
      return trigger_match_dr("EF_e20_medium") < 0.2;
    } else if( run_number < 188902 ) {
      return trigger_match_dr("EF_e22_medium") < 0.2;
    } else {
      return trigger_match_dr("EF_e22vh_medium1") < 0.2;
    }
  }

  const std::map< std::string , ftype > trigger_match_dr() const { return _trigger_match_dr; } // peter's trigger matching
  const ftype trigger_match_dr( const std::string& trigname ) const { 
    std::map<std::string,ftype>::const_iterator i=_trigger_match_dr.find(trigname);
    if( i==_trigger_match_dr.end() ) { return std::numeric_limits<ftype>::max(); }
    return i->second;
  }


  virtual std::ostream& print( std::ostream& os = std::cout ) const {
    os << boost::format("AnaElectron: et=%|+10|(GeV) eta=%|10| phi=%|10|") % et() % eta() % phi() << std::endl;
    return os;
  }

};

BOOST_CLASS_VERSION( AnaElectron , 5 );

#endif // WRAP_ANAELECTRON_HPP
