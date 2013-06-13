#ifndef WRAP_ANAMUON_HPP
#define WRAP_ANAMUON_HPP

// ANAMUON
// ================================================================
// Data for a single muon.
// ================================================================
// 2009-07-15 Antonio Boveia (boveia@hep.uchicago.edu)

#include <numeric>
#include <vector>
#include <string>
#include <map>
#include <cmath>
#include <iostream>
#include <utility>
#include <bitset>
#include <boost/shared_ptr.hpp>
#include <boost/optional.hpp>
#include <boost/format.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/version.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/optional.hpp>
#include "TrigFTKAna/mcp/MuonAllAuthor.hpp"
#include "TrigFTKAna/AnaParticle.hpp"
#include "TrigFTKAna/AnaTrack.hpp"
#include "TrigFTKAna/Detector.hpp"
#include "TrigFTKAna/Conference.hpp"

class
AnaMuon : public AnaParticle
{
  
public:

  typedef AnaParticle::ftype ftype;
  typedef std::bitset<16> MuonTypeBitset; // as of 20110310, the Athena muon bitmask uses 13 bits, so one word should be enough for now.
  // do NOT add new types in the middle of enum; append only near the end (just before NTOTALs)
  typedef enum { CONE10=0 , CONE20=1, CONE30=2, CONE40=3 , NSIZES=4 } ConeSize;
  typedef enum { PIXEL=0 , SCT=1, TRT=2, TRTOUTLIER=3, BL=4, NTYPES=5 } HitType;
  typedef enum { PIXHOLE=0 , SCTHOLE=1, TRTHOLE=2, NHOLETYPES=3 } HoleType;
  typedef enum { PIXDEAD=0 , SCTDEAD=1, TRTDEAD=2, NDEADTYPES=3 } DeadType;
  // Peter's muon 'class'
  typedef enum { AOD_UNKNOWN=0 , AOD_STACO=1, AOD_MUID=2, AOD_CALO=3, AOD_TRACK=4 } AodClass;
  // Number of replicas to study efficiency scale factor stat. errors
  static int NREPLICASF;

protected:

  int _primary_author;
  // bitmask with interpretation is described at http://alxr.usatlas.bnl.gov/lxr-stb5/source/atlas/Reconstruction/MuonIdentification/muonEvent/muonEvent/Muon.h#124
  MuonTypeBitset _all_author_bitmask; 
  unsigned int _aod_quality;
  AodClass _aod_class;
  ftype _charge;
  std::vector<ftype> _pt_cone; // inner detector pt in a given cone
  std::vector<ftype> _et_cone; // calo et in a given cone
  std::vector<int> _nhits; // Number of hits in each tracker
  std::vector<int> _nholes;
  std::vector<int> _ndead;
  bool _expect_b_layer;
  int _trigmatch;
  int _ntuple_index;
  int _topinputs;
  int _topinputs_in_trigger;
  mutable std::map< std::string , ftype > _trigger_match_dr; // peter's trigger matching

  // The following variables are defined for combined muons and point to 
  // Muon Spectrometer (ms) and Inner Detector (id) standalone AnaMuons
  TLorentzVector _ms_muon;
  TLorentzVector _exms_muon;
  TLorentzVector _id_muon;
  ftype _ms_charge;
  ftype _exms_charge;
  ftype _id_charge;

  ftype _met_wet , _met_wet2;
  ftype _met_wpx , _met_wpx2;
  ftype _met_wpy , _met_wpy2;
  ftype _met_status , _met_status2;

  // Optional variables valid only for combined muons:
  boost::optional<ftype> _fitchi2;
  boost::optional<ftype> _fitchi2prob;
  boost::optional<ftype> _matchchi2;
  boost::optional<ftype> _matchchi2prob;
  boost::shared_ptr<const AnaTrack> _id_track;
  // z0 at perigee (useful to require muons from the primary vertex)
  boost::optional<ftype> _z0;
  boost::optional<ftype> _z0_exPV;
  boost::optional<ftype> _d0_exPV;
  boost::optional<ftype> _d0_exPV_err;
  boost::optional<ftype> _z0_vx;
  boost::optional<ftype> _d0_vx;
  boost::optional<ftype> _d0_vx_err;
  boost::optional<ftype> _d0;
  boost::optional<ftype> _mu_phi;
  // extrapolated pt (usually MS pt extrapolated to ID)
  boost::optional<ftype> _expt;

  // pointer back to uncorrected muon, if this muon has been
  // corrected. note that this is not serialized, so any corrections
  // should be made at the ana_* executable level and not at the
  // ntuple reading level.
  mutable boost::shared_ptr<const AnaMuon> _uncorrected_parent;

  friend class NtD3PDphysWZ;
  friend class NtD3PDphysEW;
  friend class NtD3PDphysTop;
  friend class NtD3PDphysSingleTop;
  friend class NtPGS;
  friend class NtD3PDphysPeter;
  friend class NtD3PDphysJetMET;

private:

  friend class boost::serialization::access;
  template<class archiveT>
  void serialize( archiveT& ar, const unsigned int version ) {
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP( AnaParticle );
    ar & boost::serialization::make_nvp("primary_author",_primary_author);
    { 
      // bitset serialization class not yet available in boost 1.39
      unsigned long ultmp( _all_author_bitmask.to_ulong() );
      ar & boost::serialization::make_nvp("all_author_bitmask",ultmp);
      _all_author_bitmask = MuonTypeBitset( ultmp );
    }
    if( version >=6 ) { 
      ar & boost::serialization::make_nvp("quality",_aod_quality);
      ar & boost::serialization::make_nvp("class",_aod_class);
      ar & boost::serialization::make_nvp("trigmatch",_trigmatch);
      ar & boost::serialization::make_nvp("ntuple_index",_ntuple_index);
      ar & boost::serialization::make_nvp("trigger_match_dr",_trigger_match_dr);
      ar & boost::serialization::make_nvp("met_wet",_met_wet);
      ar & boost::serialization::make_nvp("met_wpx",_met_wpx);
      ar & boost::serialization::make_nvp("met_wpy",_met_wpy);
      ar & boost::serialization::make_nvp("met_status",_met_status);
      ar & boost::serialization::make_nvp("met_wet2",_met_wet2);
      ar & boost::serialization::make_nvp("met_wpx2",_met_wpx2);
      ar & boost::serialization::make_nvp("met_wpy2",_met_wpy2);
      ar & boost::serialization::make_nvp("met_status2",_met_status2);
    }
    ar & boost::serialization::make_nvp("charge",_charge);
    ar & boost::serialization::make_nvp("nhits",_nhits);
    ar & boost::serialization::make_nvp("nholes",_nholes);
    ar & boost::serialization::make_nvp("ndead",_ndead);
    ar & boost::serialization::make_nvp("expect_b_layer",_expect_b_layer);
    ar & boost::serialization::make_nvp("pt_cone",_pt_cone);
    ar & boost::serialization::make_nvp("et_cone",_et_cone);
    ar & boost::serialization::make_nvp("topinputs",_topinputs);
    ar & boost::serialization::make_nvp("topinputs_in_trigger",_topinputs_in_trigger);
    ar & boost::serialization::make_nvp("ms_muon",_ms_muon);
    ar & boost::serialization::make_nvp("exms_muon",_exms_muon);
    ar & boost::serialization::make_nvp("id_muon",_id_muon);
    ar & boost::serialization::make_nvp("ms_charge",_ms_charge);
    ar & boost::serialization::make_nvp("exms_charge",_exms_charge);
    ar & boost::serialization::make_nvp("id_charge",_id_charge);
    ar & boost::serialization::make_nvp("fitchi2",_fitchi2);
    ar & boost::serialization::make_nvp("matchchi2",_matchchi2);
    ar & boost::serialization::make_nvp("fitchi2prob",_fitchi2prob);
    ar & boost::serialization::make_nvp("matchchi2prob",_matchchi2prob);
    ar & boost::serialization::make_nvp("z0",_z0);
    ar & boost::serialization::make_nvp("z0_exPV",_z0_exPV);
    ar & boost::serialization::make_nvp("d0_exPV",_d0_exPV);
    ar & boost::serialization::make_nvp("d0_exPV_err",_d0_exPV_err);
    ar & boost::serialization::make_nvp("z0_vx",_z0_vx);
    ar & boost::serialization::make_nvp("d0_vx",_d0_vx);
    ar & boost::serialization::make_nvp("d0_vx_err",_d0_vx_err);
    ar & boost::serialization::make_nvp("d0",_d0);
    ar & boost::serialization::make_nvp("mu_phi",_mu_phi);
    ar & boost::serialization::make_nvp("expt",_expt);
    if( version>=2 ) {
      boost::shared_ptr<AnaTrack> cid_track( boost::const_pointer_cast<AnaTrack>( _id_track ) );
      ar & boost::serialization::make_nvp("id_track",cid_track);
      _id_track = cid_track;
    }
  }

public:

  AnaMuon()
    : _primary_author( 0 )
    , _all_author_bitmask( 0ul )
    , _aod_quality(0)
    , _aod_class(AOD_UNKNOWN)
    , _charge(0.)
    , _pt_cone()
    , _et_cone()
    , _nhits()
    , _nholes()
    , _ndead()
    , _expect_b_layer( false )
    , _trigmatch(  )
    , _ntuple_index( -1 )
    , _topinputs( -999 )
    , _topinputs_in_trigger( -999 )
    , _ms_muon()
    , _exms_muon()
    , _id_muon()
    , _ms_charge(0.)
    , _exms_charge(0.)
    , _id_charge(0.)
    , _id_track()
    , _fitchi2()
    , _fitchi2prob()
    , _matchchi2( 999999. )
    , _matchchi2prob( 0. )
    , _z0()
    , _z0_exPV()
    , _d0_exPV()
    , _d0_exPV_err()
    , _z0_vx()
    , _d0_vx()
    , _d0_vx_err()
    , _d0()
    , _mu_phi()
    , _expt()
    , _uncorrected_parent()
    , _trigger_match_dr()
    , _met_wet( 0. )
    , _met_wpx( 0. )
    , _met_wpy( 0. )
    , _met_status( 0. )
    , _met_wet2( 0. )
    , _met_wpx2( 0. )
    , _met_wpy2( 0. )
    , _met_status2( 0. )
    , AnaParticle( AnaParticle::MUON )
  {}
  virtual ~AnaMuon() {}
  static const boost::shared_ptr<const AnaMuon> make_track_muon( const boost::shared_ptr<const AnaTrack>& track ) {
    boost::shared_ptr<AnaMuon> mu( new AnaMuon );
    mu->_charge = track->charge();
    mu->_momentum = track->momentum();
    mu->_pt_cone = boost::assign::list_of( 999999. )( 999999. )( 999999. )( 999999. ); // not computed.
    mu->_et_cone = boost::assign::list_of( 999999. )( 999999. )( 999999. )( 999999. ); // not computed.
    mu->_nhits = ( boost::assign::list_of( track->pixel_nhits() )
                   (track->sct_nhits())(track->trt_nhits())(99999)(track->b_layer_nhits()) );
    mu->_nholes = boost::assign::list_of( track->pixel_nholes() )( track->sct_nholes() )(0); // trt unavailable
    mu->_ndead = boost::assign::list_of( 0 )( 0 )( 0 ); // unavailable.
    mu->_expect_b_layer = false;
    mu->_fitchi2 = track->chi2ndof(); // x
    mu->_fitchi2prob = track->chi2prob(); // x
    mu->_z0 = track->z0();
    mu->_d0 = track->d0();
    mu->_z0_exPV = 0;
    mu->_d0_exPV = 0;
    mu->_d0_exPV_err = 0;
    mu->_z0_vx = 0;
    mu->_d0_vx = 0;
    mu->_d0_vx_err = 0;
    mu->_mu_phi = track->phi();
    mu->_expt = track->pt();
    mu->_id_track = track;
    return boost::const_pointer_cast<const AnaMuon>(mu);
  }
  
  const int primary_author() const { return _primary_author; }
  const unsigned long all_author_bitmask() const { return _all_author_bitmask.to_ulong(); }
  const unsigned int aod_quality() const { return _aod_quality; }
  const bool quality_tight() const { return( _aod_quality & (1<<2) ); }
  const bool quality_medium() const { return( _aod_quality & (1<<1) ); }
  const bool quality_loose() const { return( _aod_quality & (1<<0) ); }
  const AodClass aod_class() const { return _aod_class; }
  const bool is_aod_staco() const { return _aod_class == AnaMuon::AOD_STACO; }
  const bool is_muonboy() const { return _all_author_bitmask.test( MuonAllAuthor::Muonboy ); }
  const bool is_staco_combined() const {
    return ( _all_author_bitmask.test( MuonAllAuthor::STACO ) || _all_author_bitmask.test( MuonAllAuthor::MuonCombinedRefit ) ) ;
  }
  const bool is_staco_any() const { return( is_staco_combined() || is_mutag() || is_muonboy() ); }
  const bool is_mutag() const { return _all_author_bitmask.test( MuonAllAuthor::MuTag ); }
  const bool is_muid_standalone() const { return _all_author_bitmask.test( MuonAllAuthor::MuidSA ); }
  const bool is_muid_combined() const { return _all_author_bitmask.test( MuonAllAuthor::MuidCo ); }
  const bool is_mugirl() const { return _all_author_bitmask.test( MuonAllAuthor::MuGirl ); }
  const bool is_calo_muon_id() const { return _all_author_bitmask.test( MuonAllAuthor::CaloID ); }
  const bool is_calo_muon_tag() const { return _all_author_bitmask.test( MuonAllAuthor::CaloTag ); }
  const bool is_calo_muon_likelihood() const { return _all_author_bitmask.test( MuonAllAuthor::CaloLikelihood ); }
  const bool is_mutag_imo() const { return _all_author_bitmask.test( MuonAllAuthor::MuTagIMO ); }
  const bool is_combined_refit() const { return _all_author_bitmask.test( MuonAllAuthor::MuonCombinedRefit ); }
  const bool is_ip_extrapolated() const { return _all_author_bitmask.test( MuonAllAuthor::ExtrapolatedMuonToIP ); }
  const bool is_muid() const { return( is_muid_standalone() || is_muid_combined() ); }
  const bool is_combined() const { return( is_staco_combined() || is_muid_combined() ); }
  const bool is_calo() const { return( is_calo_muon_id() || is_calo_muon_tag() || is_calo_muon_likelihood() ); }

  const bool cutflow_type() const {
    if(primary_author()==MuonPrimAuthor::STACO && is_staco_combined()) return 0;
    if(primary_author()==MuonPrimAuthor::MuTag && quality_tight()) return 2;
    if(primary_author()==MuonPrimAuthor::MuTag && quality_loose()) return 1;
    if(primary_author()==MuonPrimAuthor::IDtrack) return 3;
    return -1;
  }

  virtual const short scharge() const { return static_cast<short>(_charge); }
  virtual const ftype charge() const { return _charge; }

  const int nhits( const HitType& hit ) const { if( _nhits.empty() ) { return 0; }; assert(_nhits.size()>hit); return _nhits[hit]; }
  const int nhits_silicon() const { if( _nhits.empty() ) { return 0; }; return( _nhits[PIXEL] + _nhits[SCT] ); }
  const int nhits() const { if( _nhits.empty() ) { return 0; }; return( _nhits[PIXEL] + _nhits[SCT] + _nhits[TRT] ); }
  const int nholes( const HoleType& hole ) const { if( _nholes.empty() ) { return 999; }; assert(_nholes.size()>hole); return _nholes[hole]; }
  const int nholes_silicon() const { if( _nholes.empty() ) { return 0; }; return( _nholes[PIXHOLE] + _nholes[SCTHOLE] ); }
  const int ndead( const DeadType& dead ) const { if( _ndead.empty() ) { return 999; }; assert(_ndead.size()>dead); return _ndead[dead]; }
  // wrapper function that automatically selects the correct version of MCP smearing
  const bool passes_hits_mcp(const CONF::ConfType& conf) const {
    switch(conf) {
    case CONF::SUSI:
      return passes_hits_mcp_2012();
      break;
    case CONF::LATEST:
      return passes_hits_mcp_v17(3);
      break;
    }
    assert(0&&"Unsupported --conf in MCP ID track hit requirements");
    return false;
  }

  // MCP hit cuts
  // https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/MCPAnalysisGuidelinesRel17MC11a
  const bool passes_hits_mcp_blayer_v17() const { return( nhits(BL) || !_expect_b_layer ); }
  const bool passes_hits_mcp_holes_v17(const int maxholes) const { return( nholes(PIXHOLE) + nholes(SCTHOLE) < maxholes ); }
  const bool passes_hits_mcp_holes_v17() const { return passes_hits_mcp_holes_v17(3); }
  const bool passes_hits_mcp_pix_v17( const int nhitdead = 1 ) const { return ( nhits(PIXEL) + ndead(PIXDEAD) > nhitdead ); }
  const bool passes_hits_mcp_sct_v17( const int nhitdead = 5 ) const { return ( nhits(SCT) + ndead(SCTDEAD) > nhitdead );} 
  const bool passes_hits_mcp_sitrt_v17() const {
    const int ntrt = nhits(TRT)+nhits(TRTOUTLIER);
    const bool passes_hits = ( nhits(PIXEL) + ndead(PIXDEAD) > 1 ) && ( nhits(SCT) + ndead(SCTDEAD) >=6 ) && 
      (std::fabs(eta())<1.9 ? (ntrt>=6 && nhits(TRTOUTLIER)<0.9*ntrt) : (ntrt<6 || nhits(TRTOUTLIER)<0.9*ntrt));
    return passes_hits;
  }
  const bool passes_hits_mcp_sitrt_2012() const {
    const int ntrt = nhits(TRT)+nhits(TRTOUTLIER);
    const bool passes_hits = ( nhits(PIXEL) + ndead(PIXDEAD) > 0 ) && ( nhits(SCT) + ndead(SCTDEAD) > 4 ) && 
      ((std::fabs(eta())<1.9 && std::fabs(eta())>0.1) ? (ntrt>5 && nhits(TRTOUTLIER)<0.9*ntrt) : (ntrt<6 || nhits(TRTOUTLIER)<0.9*ntrt));
    return passes_hits;
  }
  // maxholes = 2 for fall 2010 and PLHC2011; maxholes = 3 for EPS2011 and later
  const bool passes_hits_mcp_v17(const int maxholes) const {
    const bool passes_b_layer = passes_hits_mcp_blayer_v17();
    const bool passes_holes = passes_hits_mcp_holes_v17(maxholes);
    const bool passes_hits = passes_hits_mcp_sitrt_v17();
    return( passes_b_layer && passes_holes && passes_hits );
  }
  const bool passes_hits_mcp_2012() const {
    const bool passes_b_layer = passes_hits_mcp_blayer_v17();
    const bool passes_holes = passes_hits_mcp_holes_v17(3);
    const bool passes_hits = passes_hits_mcp_sitrt_2012();
    return( passes_b_layer && passes_holes && passes_hits );
  }
  const bool passes_hits_mcp_v17_moriond() const { return passes_hits_mcp_v17(3); }

  void print_TRT_vars(const int evtnum, const int muonnum) const {
    printf("  event #     = %i\n",evtnum);
    printf("  muon #      = %i\n",muonnum);
    printf("  mu_eta      = %15.5f\n",eta() );
    printf("  TRTHits     = %i\n",nhits(TRT) );
    printf("  TRTOutliers = %i\n\n",nhits(TRTOUTLIER) );
  }

  const ftype pt_cone( const ConeSize& cone ) const { return _pt_cone[cone]; }
  const ftype pt_cone_ratio( const ConeSize& cone ) const { return detector::safe_divide( pt_cone(cone) , pt() ); }
  const ftype id_pt_cone_ratio( const ConeSize& cone ) const { return detector::safe_divide( pt_cone(cone) , id_pt() ); }
  const ftype ms_pt_cone_ratio( const ConeSize& cone ) const { return detector::safe_divide( pt_cone(cone) , ms_pt() ); }
  const ftype exms_pt_cone_ratio( const ConeSize& cone ) const { return detector::safe_divide( pt_cone(cone) , exms_pt() ); }
  const ftype et_cone( const ConeSize& cone ) const { return _et_cone[cone]; }
  const ftype et_cone_ratio( const ConeSize& cone ) const { return detector::safe_divide( et_cone(cone) , pt() ); }
  const ftype id_et_cone_ratio( const ConeSize& cone ) const { return detector::safe_divide( et_cone(cone) , id_pt() ); }
  const ftype ms_et_cone_ratio( const ConeSize& cone ) const { return detector::safe_divide( et_cone(cone) , ms_pt() ); }
  const ftype exms_et_cone_ratio( const ConeSize& cone ) const { return detector::safe_divide( et_cone(cone) , exms_pt() ); }
  const int trigmatch_word() const { return _trigmatch; }
  const int trigmatch() const { return _trigmatch; }
  const bool trigmatch_all() const { return _trigmatch==7; }
  const bool trigmatch_L1() const { return _trigmatch&1; }
  const bool trigmatch_L2() const { return _trigmatch&2; }
  const bool trigmatch_EF() const { return _trigmatch&4; }
  const int topinputs() const { return _topinputs; }
  const int topinputs_in_trigger() const { return _topinputs_in_trigger; }
  const int ntuple_index() const { return _ntuple_index; }

  const ftype met_wet() const { return _met_wet; }
  const ftype met_wpx() const { return _met_wpx; }
  const ftype met_wpy() const { return _met_wpy; }
  const ftype met_status() const { return _met_status; }
  const ftype met_wet2() const { return _met_wet2; }
  const ftype met_wpx2() const { return _met_wpx2; }
  const ftype met_wpy2() const { return _met_wpy2; }
  const ftype met_status2() const { return _met_status2; }
  const std::vector<ftype> met_wet_vec() const { const ftype wet[2] = { _met_wet , _met_wet2 }; const std::vector<ftype> wetvec( wet , wet+2 ); return wetvec; }
  const std::vector<ftype> met_wpx_vec() const { const ftype wpx[2] = { _met_wpx , _met_wpx2 }; const std::vector<ftype> wpxvec( wpx , wpx+2 ); return wpxvec; }
  const std::vector<ftype> met_wpy_vec() const { const ftype wpy[2] = { _met_wpy , _met_wpy2 }; const std::vector<ftype> wpyvec( wpy , wpy+2 ); return wpyvec; }
  const std::vector<unsigned int> met_status_vec() const { 
    const unsigned int status[2] = { int(_met_status) , int(_met_status2) }; 
    const std::vector<unsigned int> statusvec( status , status+2 ); 
    return statusvec; 
  }

  const TLorentzVector& ms_muon() const { return _ms_muon; }
  const TLorentzVector& exms_muon() const { return _exms_muon; }
  const TLorentzVector& id_muon() const { return _id_muon; }
  const ftype& ms_charge() const { return _ms_charge; }
  const ftype& exms_charge() const { return _exms_charge; }
  const ftype& id_charge() const { return _id_charge; }

  virtual const bool has_track() const { return _id_track; }
  virtual boost::shared_ptr<const AnaTrack> track() const { return _id_track; }
  const ftype exPV_d0() const { return( _id_track ? _id_track->exPV_d0() : std::numeric_limits<ftype>::max() ); }
  const ftype exPV_d0_err() const { return( _id_track ? _id_track->exPV_d0_err() : std::numeric_limits<ftype>::max() ); }
  const ftype exPV_d0_signif() const { return detector::safe_divide( exPV_d0() , exPV_d0_err() ); }
  const ftype exPV_abs_d0_signif() const { return std::abs(detector::safe_divide( exPV_d0() , exPV_d0_err()) ); }
  const ftype exPV_z0() const { return( _id_track ? _id_track->exPV_z0() : std::numeric_limits<ftype>::max() ); }
  const ftype exPV_z0sin() const { return( _id_track ? _id_track->exPV_z0_sin() : std::numeric_limits<ftype>::max() ); }

  const ftype matchchi2() const { return _matchchi2 ? *_matchchi2 : std::numeric_limits<ftype>::max(); }

  // z0 at perigee (useful to require muons from the primary vertex)
  const ftype z0() const { return _z0 ? *_z0 : std::numeric_limits<ftype>::max(); }
  const ftype d0() const { return _d0 ? *_d0 : std::numeric_limits<ftype>::max(); }
  // extrapolated to beam axis
  const ftype z0_exPV() const { return _z0_exPV ? *_z0_exPV : std::numeric_limits<ftype>::max(); }
  const ftype z0_exPV_sin() const { return _z0_exPV ? ((*_z0_exPV)*std::sin(theta())) : std::numeric_limits<ftype>::max(); }
  const ftype d0_exPV() const { return _d0_exPV ? *_d0_exPV : std::numeric_limits<ftype>::max(); }
  const ftype abs_d0_exPV() const { return _d0_exPV ? std::abs( *_d0_exPV ) : std::numeric_limits<ftype>::max(); }
  const ftype d0_exPV_err() const { return _d0_exPV_err ? *_d0_exPV_err : std::numeric_limits<ftype>::max(); }
  const ftype d0_exPV_signif() const { return (_d0_exPV && _d0_exPV_err) ? detector::safe_divide( *_d0_exPV , *_d0_exPV_err ) : std::numeric_limits<ftype>::max(); }
  const ftype abs_d0_exPV_signif() const { return (_d0_exPV && _d0_exPV_err) ? std::abs( detector::safe_divide( *_d0_exPV , *_d0_exPV_err ) ) : std::numeric_limits<ftype>::max(); }
  // extrapolated to nearest vertex
  const ftype z0_vx() const { return _z0_vx ? *_z0_vx : std::numeric_limits<ftype>::max(); }
  const ftype d0_vx() const { return _d0_vx ? *_d0_vx : std::numeric_limits<ftype>::max(); }
  const ftype d0_vx_err() const { return _d0_vx_err ? *_d0_vx_err : std::numeric_limits<ftype>::max(); }
  const ftype d0_vx_signif() const { return (_d0_vx && _d0_vx_err) ? detector::safe_divide( *_d0_vx , *_d0_vx_err ) : std::numeric_limits<ftype>::max(); }

  const ftype mu_phi() const { return _mu_phi ? *_mu_phi: 0; }
  const ftype mu_x0() const { return _d0 && _mu_phi ? (*_d0)*std::cos(*_mu_phi) : std::numeric_limits<ftype>::max(); }
  const ftype mu_y0() const { return _d0 && _mu_phi ? (*_d0)*std::sin(*_mu_phi) : std::numeric_limits<ftype>::max(); }
  
  // parameters of constituent MS and ID muons (only valid for combined muons)
  const ftype id_eta() const { return id_muon().Eta(); }
  const ftype id_phi() const { return id_muon().Phi(); }
  const ftype id_pt() const { return id_muon().Pt(); }
  const ftype ms_eta() const { return ms_muon().Eta(); }
  const ftype ms_phi() const { return ms_muon().Phi(); }
  const ftype ms_pt() const { return ms_muon().Pt(); }
  const ftype exms_eta() const { return exms_muon().Eta(); }
  const ftype exms_phi() const { return exms_muon().Phi(); }
  const ftype exms_pt() const { return exms_muon().Pt(); }
  // pt difference between ID muon and extrapolated MS muon
  const ftype pt_difference_id_ms() const { return id_pt()<std::numeric_limits<ftype>::max() && exms_pt()<std::numeric_limits<ftype>::max() ? id_pt() - exms_pt() : 0.0; }
  const ftype pt_diff_ratio() const { 
    return id_pt()<std::numeric_limits<ftype>::max() && 
      exms_pt()<std::numeric_limits<ftype>::max() ?
      std::abs(pt_difference_id_ms())/id_pt() : std::numeric_limits<ftype>::max(); 
  };

  const bool is_corrected() const { return _uncorrected_parent; }
  const boost::shared_ptr<const AnaMuon> uncorrected_parent() const { return _uncorrected_parent; }

  // change in muon pt as a result of MCP correction
  const ftype corrected_change_in_px() const {
    if(!is_corrected()) return 0;
    return px() - uncorrected_parent()->px();
  }
  const ftype corrected_change_in_py() const {
    if(!is_corrected()) return 0;
    return py() - uncorrected_parent()->py();
  }
  
  virtual std::ostream& print( std::ostream& os = std::cout ) const {
    os << boost::format("AnaMuon: pt=%|+10|(GeV) eta=%|10| phi=%|10|") % pt() % eta() % phi() << std::endl;
    return os;
  }

  // Corrected etcone isolation that uses nvtx information to properly account for the energy in a cone
  // (from Peter and Doug: https://svnweb.cern.ch/trac/penn/browser/schae/MuonIsolationCorrection/trunk/)
  const ftype et_cone_corr( const ConeSize& cone , unsigned int nvtxs ) const;
  const ftype et_cone_corr_ratio( const ConeSize& cone , unsigned int nvtxs ) const {
    return detector::safe_divide( et_cone_corr(cone, nvtxs) , pt() );
  }

  // EFFICIENCY SCALE FACTORS
  // returns the efficiency scale factor for this muon per MCP recommendations
  // the MCP efficiency scale class version is chosen automatically based on --conf
  static void
  mcp_effscale( const CONF::ConfType& conf, const DATARANGE::DataRange& data_range, 
		const boost::shared_ptr<const AnaMuon>& muon , const detector::MCP_TYPE& mu_type,
		const unsigned long& run_number, const int runRange,
		const std::vector<double>& int_lumi, const std::vector<std::string>& run_periods,
		double *effs,
		double& eff, double& errstat, double& errsys );
  
  // templated workhorse function that computes the efficiency scale factor (rel. 17)
  template <typename EFFCLASS>
  static void
  mcp_effscale_apply_v17( const boost::shared_ptr<const AnaMuon>& muon , const detector::MCP_TYPE& mu_type,
			  const unsigned long& run_number , const int runRange,
			  const std::vector<double>& int_lumi, const std::vector<std::string>& run_periods,
			  const std::string& flag ,
			  double *effs,
			  double& eff, double& errstat, double& errsys )
  {
    static std::string datadir("CommonAnalysis/RootCore/data/MuonEfficiencyCorrections/");
    const int seed = 5000; //1721
    static EFFCLASS *mcp_staco_cb = 0;
    static EFFCLASS *mcp_staco_cb_DtoK = 0;
    static EFFCLASS *mcp_staco_cb_LtoM = 0;
    static EFFCLASS *mcp_staco_loose = 0;
    static EFFCLASS *mcp_muid_cb = 0;
    static EFFCLASS *mcp_muid_loose = 0;
    if( !mcp_staco_cb && runRange==0) { //DtoM
      if(false) { // EtaPhi // FIXME
	mcp_staco_cb = new EFFCLASS( datadir , "STACO_CB_2011_SF_fine.txt" , "GeV" , EFFCLASS::AverageOverPeriods );
      } else {   // EtaPt
	mcp_staco_cb = new EFFCLASS( datadir , "STACO_CB_2011_SF_fineEtaBinning.txt" , "GeV" , EFFCLASS::AverageOverPeriods );
      }
      mcp_staco_loose = new EFFCLASS( datadir , "STACO_CB_plus_ST_2011_SF.txt" , "GeV" , EFFCLASS::AverageOverPeriods );
      mcp_muid_cb = new EFFCLASS( datadir , "Muid_CB_2011_SF.txt" , "GeV" , EFFCLASS::AverageOverPeriods );
      mcp_muid_loose = new EFFCLASS( datadir , "Muid_CB_plus_ST_2011_SF.txt" , "GeV" , EFFCLASS::AverageOverPeriods );
      assert( int_lumi.size() == run_periods.size() && "probably need to edit run_periods vector in your analysis file" );
      std::cout << "Inserting run periods into muon efficiency tool: " << runRange << std::endl;
      for( int i = 0 ; i < run_periods.size() ; ++i ) {
	std::cout << "EFFSCALE WEIGHTS: " << run_periods[i] << " " << int_lumi[i] << std::endl;
	mcp_staco_cb->addPeriod( run_periods[i] , int_lumi[i] );
	mcp_staco_loose->addPeriod( run_periods[i] , int_lumi[i] );
	mcp_muid_cb->addPeriod( run_periods[i] , int_lumi[i] );
	mcp_muid_loose->addPeriod( run_periods[i] , int_lumi[i] );
      }
      mcp_staco_cb->Initialise();
      if(NREPLICASF>0) mcp_staco_cb->generateReplicas(NREPLICASF , seed);
      mcp_staco_loose->Initialise();
      mcp_muid_cb->Initialise();
      mcp_muid_loose->Initialise();
    }
    if( !mcp_staco_cb_DtoK && runRange==1 ) { //DtoK
      mcp_staco_cb_DtoK = new EFFCLASS( datadir , "STACO_CB_2011_SF_fineEtaBinning.txt" , "GeV" , EFFCLASS::AverageOverPeriods );
      assert( int_lumi.size() == run_periods.size() && "probably need to edit run_periods vector in your analysis file" );
      assert( int_lumi[9]==0 && int_lumi[10]==0 );
      std::cout << "Inserting run periods into muon efficiency tool: " << runRange << std::endl;
      for( int i = 0 ; i < run_periods.size() ; ++i ) {
	std::cout << "EFFSCALE WEIGHTS: " << run_periods[i] << " " << int_lumi[i] << std::endl;
	mcp_staco_cb_DtoK->addPeriod( run_periods[i] , int_lumi[i] );
      }
      mcp_staco_cb_DtoK->Initialise();
    }
    if( !mcp_staco_cb_LtoM && runRange==2 ) { //LtoM
      mcp_staco_cb_LtoM = new EFFCLASS( datadir , "STACO_CB_2011_SF_fineEtaBinning.txt" , "GeV" , EFFCLASS::AverageOverPeriods );
      assert( int_lumi.size() == run_periods.size() && "probably need to edit run_periods vector in your analysis file" );
      assert( int_lumi[1]==0 && int_lumi[8]==0 );
      std::cout << "Inserting run periods into muon efficiency tool: " << runRange << std::endl;
      for( int i = 0 ; i < run_periods.size() ; ++i ) {
	std::cout << "EFFSCALE WEIGHTS: " << run_periods[i] << " " << int_lumi[i] << std::endl;
	mcp_staco_cb_LtoM->addPeriod( run_periods[i] , int_lumi[i] );
      }
      mcp_staco_cb_LtoM->Initialise();
    }
    EFFCLASS *mcp = 0;
    if(mu_type==detector::MCP_STACO_COMBINED) {
      // mcp = runRange==2 ? mcp_staco_cb_LtoM : ( runRange==1 ? mcp_staco_cb_DtoK : mcp_staco_cb);
      mcp = mcp_staco_cb;
    }
    //    if(mu_type==detector::MCP_STACO_LOOSE || mu_type==detector::MCP_STACO_TIGHT) mcp = mcp_staco_loose;
    //    if(mu_type==detector::MCP_MUID_COMBINED) mcp = mcp_muid_cb;
    //    if(mu_type==detector::MCP_MUID_LOOSE || mu_type==detector::MCP_MUID_TIGHT) mcp = mcp_muid_loose;
    //    if(replica>=0) assert(runRange==0); // replicas are only used for runRange=0 (DtoM)
    const std::pair<int,int> bin = mcp->get_pt_eta_phi_bin_index(muon->scharge(),muon->four_vector());
    for(int replica=0;replica<NREPLICASF;replica++) {
      effs[replica] *= mcp->scaleFactorReplica(muon->scharge(),muon->four_vector(),replica,bin);
    }
    eff = mcp->scaleFactor(muon->scharge(),muon->four_vector(),bin); 
    errstat = mcp->scaleFactorUncertainty(muon->scharge(),muon->four_vector(),bin);
    errsys = mcp->scaleFactorSystematicUncertainty(muon->scharge(),muon->four_vector(),bin);
    return;
  }

  // TRIGGER SCALE FACTORS
  
  // returns total trigger scale factor for all leptons in the event
  // This function is provided by TrigMuonEfficiency package
  static void
  GetTriggerSF_v17( const CONF::ConfType& conf, const detector::MCP_TYPE& mu_type , const detector::EGAMMA_TYPE& el_type , 
		    std::vector<TLorentzVector>& muons,
		    std::vector<TLorentzVector>& electrons,
		    const unsigned long& run_number , const int& mu_trig ,
		    double& eff, double& err ,
		    int replica=-1 );

  // a custom version (from Max) of trigger scale factors that can be used with the ScaleFactorProvider package
  static void
  GetTriggerSF_v17_custom( const CONF::ConfType& conf, const detector::MCP_TYPE& mu_type , const detector::EGAMMA_TYPE& el_type , 
			   std::vector<TLorentzVector>& muons,
			   std::vector<TLorentzVector>& electrons,
			   std::vector<ftype>& muon_charges,
			   const unsigned long& run_number , const int& mu_trig ,
			   double& eff, double& err , int choice ,
			   int replica=-1 );

  // Eta x Pt: a custom version (from Carl) of trigger scale factors that can be used with the ScaleFactorProvider package
  // these scale factors are provided by MCP trigger group *unofficially* for the 2011 measurement.
  static Double_t GetSFError( Double_t a, Double_t b, Double_t c, Double_t d);
  static void
  GetTriggerSF_v17_custom2( const CONF::ConfType& conf, const detector::MCP_TYPE& mu_type , const detector::EGAMMA_TYPE& el_type , 
			    std::vector<TLorentzVector>& muons,
			    std::vector<TLorentzVector>& electrons,
			    std::vector<ftype>& muon_charges,
			    const unsigned long& run_number , const int& mu_trig ,
			    double& eff, double& err,
			    int replica=-1 );

  // Eta x Phi: a custom version (from Carl) of trigger scale factors that can be used with the ScaleFactorProvider package
  // these scale factors are provided by MCP trigger group *unofficially* for the 2011 measurement.
  // THIS version adds eta-phi corrections on top of eta-pt in GetTriggerSF_v17_custom2 - works for many muons
  static void
  GetTriggerSF_v17_custom4( const CONF::ConfType& conf, const detector::MCP_TYPE& mu_type , const detector::EGAMMA_TYPE& el_type , 
			    std::vector<TLorentzVector>& muons,
			    std::vector<TLorentzVector>& electrons,
			    std::vector<ftype>& muon_charges,
			    const unsigned long& run_number , const int& mu_trig ,
			    double& eff, double& err,
			    int replica=-1);


  // trying out temporary variations to reduce statistical error
  static void
  GetTriggerSF_v17_custom2_ptindep( const CONF::ConfType& conf, const detector::MCP_TYPE& mu_type , const detector::EGAMMA_TYPE& el_type , 
				    std::vector<TLorentzVector>& muons,
				    std::vector<TLorentzVector>& electrons,
				    std::vector<ftype>& muon_charges,
				    const unsigned long& run_number , const int& mu_trig ,
				    double& eff, double& err,
				    int replica=-1 );
  static void
  GetTriggerSF_v17_custom2_qindep( const CONF::ConfType& conf, const detector::MCP_TYPE& mu_type , const detector::EGAMMA_TYPE& el_type , 
				   std::vector<TLorentzVector>& muons,
				   std::vector<TLorentzVector>& electrons,
				   std::vector<ftype>& muon_charges,
				   const unsigned long& run_number , const int& mu_trig ,
				   double& eff, double& err,
				   int replica=-1 );


  // ISOLATION SCALE FACTORS
  static void
  GetIsolationSF_v17( const CONF::ConfType& conf, const detector::MCP_TYPE& mu_type , 
		      std::vector<TLorentzVector>& muons,
		      const unsigned long& run_number ,
		      double& eff, double& err ,
		      int choice, int replica=-1 );
  static void
  GetIsolationSF_v17_old( const CONF::ConfType& conf, const detector::MCP_TYPE& mu_type , 
			  std::vector<TLorentzVector>& muons,
			  const unsigned long& run_number ,
			  double& eff, double& err ,
			  int replica=-1 );
  
  // SMEARING AND MOMENTUM SCALING (MCP)
  // smear this muon.
  // the smearing class version is chosen automatically based on --conf
  static boost::shared_ptr<const AnaMuon>
  mcp_smeared( const CONF::ConfType& conf, const DATARANGE::DataRange& data_range,
	       const boost::shared_ptr<const AnaMuon>& muon , const detector::MCP_TYPE& mu_type ,
	       const unsigned long& event_number , const size_t& muon_number ,
	       const int& scale, const std::string& fun = "NOMINAL" );
  
  // templated workhorse function that applies the smearing for rel. 17
  // As of rel. 17, we can also apply momentum scaling
  template <typename SMEARCLASS>
  static boost::shared_ptr<const AnaMuon>
  mcp_smeared_apply_v17( const boost::shared_ptr<const AnaMuon>& muon , const detector::MCP_TYPE& mu_type ,
			 const unsigned long& event_number , const size_t& muon_number ,
			 const int& scale, const std::string& datatype , const std::string& release ,
			 const std::string& fun = "NOMINAL" )
  {
    // various momentum scaling options ("scale" parameter)
    static std::string datadir("CommonAnalysis/RootCore/data/MuonMomentumCorrections/");
    static const std::string smtype("pT"); // alternative: "q_pT"
    static SMEARCLASS staco_smear_default(datatype,"staco",smtype,release,datadir);
    static SMEARCLASS muid_smear_default(datatype,"muid",smtype,release,datadir);
    static SMEARCLASS staco_smear_kpkm(datatype,"staco",smtype,release,datadir);
    static SMEARCLASS muid_smear_kpkm(datatype,"muid",smtype,release,datadir);
    static SMEARCLASS staco_smear_KC(datatype,"staco",smtype,release,datadir);
    static SMEARCLASS muid_smear_KC(datatype,"muid",smtype,release,datadir);
    static bool mcp_initialize_staco = true;
    static bool mcp_initialize_muid = true;
    if( mcp_initialize_staco ) {
      // NOTE: some of these options are not availible yet in 2012 samples, hence the "if" statements
      if( datatype != "Data12" ) staco_smear_KC.RestrictCurvatureCorrections();
      staco_smear_default.UseImprovedCombine();
      staco_smear_kpkm.UseImprovedCombine();
      staco_smear_KC.UseImprovedCombine();
      staco_smear_default.UseGeV();
      staco_smear_kpkm.UseGeV();
      staco_smear_KC.UseGeV();
      staco_smear_default.FillScales("");
      if( datatype != "Data12" ) staco_smear_kpkm.FillScales("kpkm");
      if( datatype != "Data12" ) staco_smear_KC.FillScales("KC");
      mcp_initialize_staco = false;
    }
    if( mcp_initialize_muid ) {
      if( datatype != "Data12" ) muid_smear_KC.RestrictCurvatureCorrections();
      muid_smear_default.UseImprovedCombine();
      muid_smear_kpkm.UseImprovedCombine();
      muid_smear_KC.UseImprovedCombine();
      muid_smear_default.UseGeV();
      muid_smear_kpkm.UseGeV();
      muid_smear_KC.UseGeV();
      muid_smear_default.FillScales("");
      if( datatype != "Data12" ) muid_smear_kpkm.FillScales("kpkm");
      if( datatype != "Data12" ) muid_smear_KC.FillScales("KC");
      mcp_initialize_muid = false;
    }
    // special case: do not apply any smearing/scaling
    if(fun=="NONE") return muon;
    // select the right version of smearing/scaling class
    SMEARCLASS *mcp_smear = 0;
    switch(std::abs(scale)) {
    case 0:
    case 1:
      mcp_smear = (mu_type==detector::MCP_STACO_COMBINED || mu_type==detector::MCP_STACO_LOOSE || mu_type==detector::MCP_STACO_TIGHT) ? &staco_smear_default : &muid_smear_default;
      break;
    case 2:
      mcp_smear = (mu_type==detector::MCP_STACO_COMBINED || mu_type==detector::MCP_STACO_LOOSE || mu_type==detector::MCP_STACO_TIGHT) ? &staco_smear_kpkm : &muid_smear_kpkm;
      break;
    case 3:
      mcp_smear = (mu_type==detector::MCP_STACO_COMBINED || mu_type==detector::MCP_STACO_LOOSE || mu_type==detector::MCP_STACO_TIGHT) ? &staco_smear_KC : &muid_smear_KC;
      break;
    default:
      assert(0&&"Unsupported muon scale parameter");
    }
    assert(mcp_smear);
    mcp_smear->UseScale( scale!=0 );
    mcp_smear->ApplyToData( scale<0 );
    mcp_smear->SetSeed( event_number , muon_number ); 
    mcp_smear->Event( muon->exms_muon().Pt() ,
		      muon->id_muon().Pt() ,
		      muon->pt() ,
		      muon->eta() ,
		      muon->scharge()
		      );
    double pt_combined_smeared = mcp_smear->pTCB();
    double pt_ms_smeared = mcp_smear->pTMS();
    double pt_id_smeared = mcp_smear->pTID();
    double debug_pt_cb_orig = pt_combined_smeared;
    if( !(fun=="NOMINAL" || fun=="") ) { // careful: it is expected that the user won't call PTVar with fun=NOMINAL
      mcp_smear->PTVar( pt_ms_smeared , pt_id_smeared , pt_combined_smeared , fun );
    }
    if(false) std::cout << fun << " " << muon->pt() << " " << debug_pt_cb_orig << " " << pt_combined_smeared << std::endl;
    // make smeared muon.
    boost::shared_ptr<AnaMuon> new_muon( boost::make_shared<AnaMuon>(*muon) );
    //std::cout << "pt: " << muon->pt() << " " << pt_combined_smeared << std::endl; 
    new_muon->_momentum.SetPtEtaPhiM( pt_combined_smeared ,
				      pt_combined_smeared > 0. ? muon->_momentum.Eta() : 999999. ,
				      muon->_momentum.Phi() ,
				      muon->_momentum.M() );
    new_muon->_exms_muon.SetPtEtaPhiM( pt_ms_smeared ,
				       pt_ms_smeared > 0. ? muon->_exms_muon.Eta() : 999999. ,
				       muon->_exms_muon.Phi() ,
				       muon->_exms_muon.M() );
    new_muon->_id_muon.SetPtEtaPhiM( pt_id_smeared ,
				     pt_id_smeared > 0. ? muon->_id_muon.Eta() : 999999. ,
				     muon->_id_muon.Phi() ,
				     muon->_id_muon.M() );
    // smear ID track. do not bother with propagating this into
    // the covariance matrix; just smear the track pt.
    if( muon->_id_track ) {
      boost::shared_ptr<AnaTrack> new_track( boost::make_shared<AnaTrack>(*(muon->_id_track)) );
      // curv = A/pt => curv'/curv = (A/pt')/(A/pt) = pt/pt'
      new_track->_curv *= detector::safe_divide( muon->_id_muon.Pt() , pt_id_smeared );
      new_muon->_id_track = boost::const_pointer_cast<const AnaTrack>(new_track);
    }
    // true: smear MS muon like extrapolated-MS muon.
    // false: set to zero to keep user from using it.
    if( true ) {
      mcp_smear->SetSeed( event_number , muon_number ); // same random seed as exms case!
      mcp_smear->Event( muon->ms_muon().Pt() ,
			muon->id_muon().Pt() ,
			muon->pt() ,
			muon->eta() ,
			muon->scharge()
			);
      // propagate systematic variation. Treat MS and EXMS as fully correlated!
      double pt_combined_smeared = mcp_smear->pTCB();
      double pt_ms_smeared = mcp_smear->pTMS();
      double pt_id_smeared = mcp_smear->pTID();
      if( !(fun=="NOMINAL" || fun=="") ) { // careful: it is expected that the user won't call PTVar with fun=NOMINAL
	mcp_smear->PTVar( pt_ms_smeared , pt_id_smeared , pt_combined_smeared , fun );
      }
      new_muon->_ms_muon.SetPtEtaPhiM( pt_ms_smeared ,
				       pt_ms_smeared > 0. ? muon->_ms_muon.Eta() : 999999. ,
				       muon->_ms_muon.Phi() ,
				       muon->_ms_muon.M() );
    } else {
      new_muon->_ms_muon.SetPtEtaPhiM( 0 , 0 , 0 , 0 );
    }
    new_muon->_uncorrected_parent = muon;
    // done.
    return boost::const_pointer_cast<const AnaMuon>( new_muon );
  }

  // add back FSR photon candidates around a cone
  // todo

  // wrapper function that checks if EF object is within cone 0.2 of this muon
  const bool passes_trigger_matching_v17() const;
  const bool vlq_trigger_matched( const unsigned long& run_number ) const {
    if( run_number < 186516 ) {
      return trigger_match_dr("EF_mu18_MG") < 0.2 || trigger_match_dr("EF_mu18") < 0.2;
    } else {
      return trigger_match_dr("EF_mu18_MG_medium") < 0.2 || trigger_match_dr("EF_mu18_medium") < 0.2;
    }
  }
  
  const std::map< std::string , ftype > trigger_match_dr() const { return _trigger_match_dr; } // peter's trigger matching
  const ftype trigger_match_dr( const std::string& trigname ) const { 
    std::map<std::string,ftype>::const_iterator i=_trigger_match_dr.find(trigname);
    if( i==_trigger_match_dr.end() ) { return std::numeric_limits<ftype>::max(); }
    return i->second;
  }

  class IsTriggerMatch {
  private:
    std::string _trigger_name;
    AnaMuon::ftype _max_dr;
  public:
    typedef const bool result_type;
    IsTriggerMatch( const std::string name , const AnaMuon::ftype& max_dr )
      : _trigger_name(name) , _max_dr(max_dr) {}
    template<class muonPtr>
    result_type operator()( muonPtr const& p ) const {
      const AnaMuon::ftype dr = p->trigger_match_dr( _trigger_name );
      return( dr < _max_dr );
    }
  };

};

  BOOST_CLASS_VERSION( AnaMuon , 8 );

#endif // WRAP_ANAMUON_HPP
