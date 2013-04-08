#ifndef WRAP_ANAJET_HPP
#define WRAP_ANAJET_HPP

// ANAJET
// ================================================================
// Data for a single jet.
// ================================================================
// 2009-07-15 Antonio Boveia (boveia@hep.uchicago.edu)

#include <numeric>
#include <cmath>
#include <string>
#include <iostream>
#include <boost/format.hpp>
#include <boost/optional.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/version.hpp>
#include <TLorentzVector.h>
#include "TrigFTKAna/AnaParticle.hpp"
#include "TrigFTKAna/Detector.hpp"
#include "TrigFTKAna/Conference.hpp"

class
AnaJet : public AnaParticle
{

public:

  typedef AnaParticle::ftype ftype;

  typedef enum { JESVAR_JES , JESVAR_NPV , JESVAR_MU , JESVAR_NPVMU } JES_VARIATION;

  class Algorithm
  {
  public:
    typedef enum { UNKNOWN=0 , 
                   ATLAS_CONE_4_TOWER=1 , 
                   ATLAS_CONE_4_TRUTH=2, 
                   L1_ROI=3 , 
                   ATLAS_CONE_4_TRUTH_PILEUP=4 , 
                   ATLAS_CONE_7_TOWER=5 , 
                   ATLAS_ANTI_KT_CONE_4_H1_TOPO=6,
                   ATLAS_ANTI_KT_CONE_6_H1_TOPO=7,
		   ATLAS_ANTI_KT_CONE_10_H1_TOPO=8,
                   ATLAS_MET_GOODNESS=9,
		   ATLAS_ANTI_KT_CONE_4_H1_LC_TOPO=10,
		   ATLAS_ANTI_KT_CONE_6_H1_LC_TOPO=11,
		   ATLAS_ANTI_KT_CONE_10_H1_LC_TOPO=12,
		   ATLAS_CONE_6_TRUTH=13,
		   ATLAS_CONE_6_TRUTH_PILEUP=14
    } Type;
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
    Type type() const { return _type; }
    const bool operator==( const Algorithm& rhs ) const { return _type==rhs._type; }
    const bool operator==( const Algorithm::Type& rhs ) const { return _type==rhs; }
    const std::string& name() const { 
      switch( _type ) {
      case ATLAS_CONE_4_TOWER:
        { static std::string result("ATLAS_CONE_4_TOWER"); return result; }
        break;
      case ATLAS_CONE_7_TOWER:
        { static std::string result("ATLAS_CONE_7_TOWER"); return result; }
        break;
      case ATLAS_CONE_4_TRUTH:
        { static std::string result("ATLAS_CONE_4_TRUTH"); return result; }
        break;
      case ATLAS_CONE_4_TRUTH_PILEUP:
        { static std::string result("ATLAS_CONE_4_TRUTH_PILEUP"); return result; }
        break;
      case L1_ROI:
        { static std::string result("L1_ROI"); return result; }
        break;
      case ATLAS_ANTI_KT_CONE_4_H1_TOPO:
       { static std::string result("ATLAS_ANTI_KT_CONE_4_H1_TOPO"); return result; }
        break;
      case ATLAS_ANTI_KT_CONE_6_H1_TOPO:
        { static std::string result("ATLAS_ANTI_KT_CONE_6_H1_TOPO"); return result; }
        break;
      case ATLAS_MET_GOODNESS:
        { static std::string result("ATLAS_MET_GOODNESS"); return result; }
        break;
      case ATLAS_ANTI_KT_CONE_4_H1_LC_TOPO:
	{ static std::string result("ATLAS_ANTI_KT_CONE_4_H1_LC_TOPO"); return result; }
	break;
      case ATLAS_ANTI_KT_CONE_6_H1_LC_TOPO:
	{ static std::string result("ATLAS_ANTI_KT_CONE_6_H1_LC_TOPO"); return result; }
	break;
      case ATLAS_CONE_6_TRUTH:
	{ static std::string result("ATLAS_CONE_6_TRUTH"); return result; }
	break;
      case ATLAS_CONE_6_TRUTH_PILEUP:
	{ static std::string result("ATLAS_CONE_6_TRUTH_PILEUP"); return result; }
	break;
      case UNKNOWN:
      default:
        { static std::string unk("UNKNOWN"); return unk; }
        break;
      }
    }
    operator std::string() { return name(); }
  };

  class
  Hadron
  {
  public:
    typedef enum { UNKNOWN , LIGHT_QUARK , C_QUARK , B_QUARK } Type;
  protected:
    Type _type;
    int _pdg_code;
    ftype _x;
    ftype _y;
    ftype _z;
    ftype _eta;
    ftype _phi;
    unsigned int _num_daughters;
    friend class NtCBNT;
  private:
    friend class boost::serialization::access;
    template<class archiveT>
    void serialize( archiveT& ar, const unsigned int version ) {
      ar & boost::serialization::make_nvp("type",_type);
      ar & boost::serialization::make_nvp("pdg_code",_pdg_code);
      ar & boost::serialization::make_nvp("x",_x);
      ar & boost::serialization::make_nvp("y",_y);
      ar & boost::serialization::make_nvp("z",_z);
      ar & boost::serialization::make_nvp("eta",_eta);
      ar & boost::serialization::make_nvp("phi",_phi);
      ar & boost::serialization::make_nvp("num_daughters",_num_daughters);
    }
  public:
    Hadron()
      : _type( UNKNOWN )
      , _pdg_code( 0 )
      , _x( std::numeric_limits<ftype>::max() )
      , _y( std::numeric_limits<ftype>::max() )
      , _z( std::numeric_limits<ftype>::max() )
      , _eta( std::numeric_limits<ftype>::max() )
      , _phi( std::numeric_limits<ftype>::max() )
      , _num_daughters( 0 )
    {}
    Hadron( const Type& type , const int& pdg_code , 
            const ftype& x , const ftype& y , const ftype& z ,
            const ftype& eta, const ftype& phi , 
            const unsigned int& num_daughters )
      : _type(type) , _pdg_code(pdg_code)
      , _x(x) , _y(y) , _z(z)
      , _eta(eta) , _phi(phi)
      , _num_daughters(num_daughters)
    {}
    const Type& type() const { return _type; }
    const int& pdg_code() const { return _pdg_code; }
    const ftype& x() const { return _x; }
    const ftype& y() const { return _y; }
    const ftype& z() const { return _z; }
    const ftype& eta() const { return _eta; }
    const ftype& phi() const { return _phi; }
    const unsigned int& num_daughters() const { return _num_daughters; }
  };

protected:

  Algorithm _alg;

  int _jet_index;

  // track fit/parameters
  TLorentzVector _full_scale_momentum;
  TLorentzVector _em_scale_momentum;
  TLorentzVector _cs_scale_momentum;
  bool           _has_cs_scale;     // this variable is missing in ntuples before V29G
  ftype _full_scale_E; // need this variable for removing negative energy jets
                       // the TLorentzVector is not good enough
  ftype _em_scale_correction;
  ftype _emf;

  // Jet quality
  ftype _vtx_fraction;
  ftype _n90;
  boost::optional<ftype> _hecf;
  boost::optional<ftype> _quality;
  boost::optional<ftype> _LArQuality;
  boost::optional<ftype> _AverageLArQF;
  boost::optional<ftype> _HECQuality;
  boost::optional<ftype> _negativeE;
  boost::optional<ftype> _sumPtTrack;
  boost::optional<ftype> _sumPtTrack_allpv;
  boost::optional<ftype> _fmax;
  boost::optional<ftype> _timing;
  boost::optional<ftype> _tgap3f;
  ftype _EtaOriginEM;
  ftype _PhiOriginEM;
  ftype _MOriginEM;
  ftype _EtaOrigin;
  ftype _PhiOrigin;
  ftype _MOrigin;
  int _is_bad_loose;
  ftype _BCH_CORR_CELL;
  ftype _BCH_CORR_JET;
  int _ntrkjetalg;
  int _ntrkjetalg_allpv;

  ftype _met_wet;
  ftype _met_wpx;
  ftype _met_wpy;
  ftype _met_status;
  ftype _gcwjes;

  ftype _dr_b_parton; // higgs
  ftype _dr_c_parton;
  ftype _dr_light_parton;
  ftype _dr_b_parton_any; // higgs, other primary, or other pileup
  ftype _dr_c_parton_any;
  ftype _dr_light_parton_any;
  ftype _eta_highest_pt_parton;
  ftype _phi_highest_pt_parton;
  int _pdg_label; // pdg code label for truth jets, taken from ntupler

  ftype _calowidth;
  ftype _trackwidth;
  ftype _trackwidth_allpv;

  ftype _primary_vertex_x; // primary vertex position. no per-event uncertainties for now.
  ftype _primary_vertex_y;
  ftype _primary_vertex_z;

  ftype _tag_l2_xd0; // trigger tagger results
  ftype _tag_l2_xz0;
  ftype _tag_l2_x2d;
  ftype _tag_ef_xd0;
  ftype _tag_ef_xz0;
  ftype _tag_ef_x2d;
  mutable ftype _tag_ip2d_w; // offline tagging results
  mutable ftype _tag_ip3d_w; 
  mutable ftype _tag_sv0_w; // signed distance (in 3D) between PV and SV divided by its error
  mutable ftype _tag_sv1_w; 
  mutable ftype _tag_sv2_w; 
  mutable ftype _tag_cmb_w; // IP3D+SV1
  mutable ftype _tag_cmbnn_w; // JetFitter Comb NN
  mutable ftype _tag_jetp_w; //  b-jet probability from JetProb algorithm
  mutable ftype _tag_jetpneg_w; // result of JetProb in negative tagging mode
  mutable ftype _tag_trkc_w; // weight from TrackCounting (2D) algorithm
  mutable ftype _tag_pgs; // PGS b tagging

  // pointer back to uncorrected jet, if this jet has been
  // corrected. note that this is not serialized, so any corrections
  // should be made at the ana_* executable level and not at the
  // ntuple reading level.
  mutable boost::shared_ptr<const AnaJet> _uncorrected_parent;

  std::vector<Hadron> _associated_hadrons;

  friend class NtCBNT;
  friend class NtJetTag;
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
    ar & boost::serialization::make_nvp("algorithm",_alg);
    ar & boost::serialization::make_nvp("jet_index",_jet_index);
    ar & boost::serialization::make_nvp("full_scale_momentum",_full_scale_momentum);
    ar & boost::serialization::make_nvp("em_scale_momentum",_em_scale_momentum);
    ar & boost::serialization::make_nvp("cs_scale_momentum",_cs_scale_momentum);
    ar & boost::serialization::make_nvp("has_cs_scale",_has_cs_scale);
    ar & boost::serialization::make_nvp("full_scale_E",_full_scale_E);
    ar & boost::serialization::make_nvp("em_scale_correction",_em_scale_correction);
    ar & boost::serialization::make_nvp("emf",_emf);
    ar & boost::serialization::make_nvp("vtx_fraction",_vtx_fraction);
    ar & boost::serialization::make_nvp("n90",_n90);
    ar & boost::serialization::make_nvp("hecf",_hecf);
    ar & boost::serialization::make_nvp("quality",_quality);
    ar & boost::serialization::make_nvp("LArQuality",_LArQuality);
    ar & boost::serialization::make_nvp("AverageLArQF",_AverageLArQF);
    ar & boost::serialization::make_nvp("HECQuality",_HECQuality);
    ar & boost::serialization::make_nvp("negativeE",_negativeE);
    ar & boost::serialization::make_nvp("sumPtTrack",_sumPtTrack);
    ar & boost::serialization::make_nvp("sumPtTrack_allpv",_sumPtTrack_allpv);
    ar & boost::serialization::make_nvp("fmax",_fmax);
    ar & boost::serialization::make_nvp("timing",_timing);
    ar & boost::serialization::make_nvp("tgap3f",_tgap3f);
    ar & boost::serialization::make_nvp("EtaOriginEM",_EtaOriginEM);
    ar & boost::serialization::make_nvp("PhiOriginEM",_PhiOriginEM);
    ar & boost::serialization::make_nvp("MOriginEM",_MOriginEM);
    ar & boost::serialization::make_nvp("EtaOrigin",_EtaOrigin);
    ar & boost::serialization::make_nvp("PhiOrigin",_PhiOrigin);
    ar & boost::serialization::make_nvp("MOrigin",_MOrigin);
    ar & boost::serialization::make_nvp("is_bad_loose",_is_bad_loose);
    ar & boost::serialization::make_nvp("BCH_CORR_CELL",_BCH_CORR_CELL);
    ar & boost::serialization::make_nvp("BCH_CORR_JET",_BCH_CORR_JET);
    ar & boost::serialization::make_nvp("ntrkjetalg",_ntrkjetalg);
    ar & boost::serialization::make_nvp("ntrkjetalg_allpv",_ntrkjetalg_allpv);
    ar & boost::serialization::make_nvp("met_wet",_met_wet);
    ar & boost::serialization::make_nvp("met_wpx",_met_wpx);
    ar & boost::serialization::make_nvp("met_wpy",_met_wpy);
    ar & boost::serialization::make_nvp("met_status",_met_status);
    ar & boost::serialization::make_nvp("gcwjes",_gcwjes);
    ar & boost::serialization::make_nvp("dr_b_parton",_dr_b_parton);
    ar & boost::serialization::make_nvp("dr_c_parton",_dr_c_parton);
    ar & boost::serialization::make_nvp("dr_light_parton",_dr_light_parton);
    ar & boost::serialization::make_nvp("dr_b_parton_any",_dr_b_parton_any);
    ar & boost::serialization::make_nvp("dr_c_parton_any",_dr_c_parton_any);
    ar & boost::serialization::make_nvp("dr_light_parton_any",_dr_light_parton_any);
    ar & boost::serialization::make_nvp("pdg_label",_pdg_label);
    ar & boost::serialization::make_nvp("calowidth",_calowidth);
    ar & boost::serialization::make_nvp("trackwidth",_trackwidth);
    ar & boost::serialization::make_nvp("trackwidth_allpv",_trackwidth_allpv);
    ar & boost::serialization::make_nvp("eta_highest_pt_parton",_eta_highest_pt_parton);
    ar & boost::serialization::make_nvp("phi_highest_pt_parton",_phi_highest_pt_parton);
    ar & boost::serialization::make_nvp("primary_vertex_x",_primary_vertex_x);
    ar & boost::serialization::make_nvp("primary_vertex_y",_primary_vertex_y);
    ar & boost::serialization::make_nvp("primary_vertex_z",_primary_vertex_z);
    ar & boost::serialization::make_nvp("tag_l2_xd0",_tag_l2_xd0);
    ar & boost::serialization::make_nvp("tag_l2_xz0",_tag_l2_xz0);
    ar & boost::serialization::make_nvp("tag_l2_x2d",_tag_l2_x2d);
    ar & boost::serialization::make_nvp("tag_ef_xd0",_tag_ef_xd0);
    ar & boost::serialization::make_nvp("tag_ef_xz0",_tag_ef_xz0);
    ar & boost::serialization::make_nvp("tag_ef_x2d",_tag_ef_x2d);
    ar & boost::serialization::make_nvp("tag_ip2d_w",_tag_ip2d_w);
    ar & boost::serialization::make_nvp("tag_ip3d_w",_tag_ip3d_w); 
    ar & boost::serialization::make_nvp("tag_sv0_w",_tag_sv0_w);
    ar & boost::serialization::make_nvp("tag_sv1_w",_tag_sv1_w); 
    ar & boost::serialization::make_nvp("tag_sv2_w",_tag_sv2_w); 
    ar & boost::serialization::make_nvp("tag_cmb_w",_tag_cmb_w);
    ar & boost::serialization::make_nvp("tag_cmbnn_w",_tag_cmbnn_w);
    ar & boost::serialization::make_nvp("tag_jetp_w",_tag_jetp_w);
    ar & boost::serialization::make_nvp("tag_jetpneg_w",_tag_jetpneg_w);
    ar & boost::serialization::make_nvp("tag_trkc_w",_tag_trkc_w);
    ar & boost::serialization::make_nvp("tag_pgs",_tag_pgs);
    ar & boost::serialization::make_nvp("associated_hadrons",_associated_hadrons);
  }

public:

  AnaJet()
    : AnaParticle( AnaParticle::JET )
    , _alg( Algorithm::UNKNOWN )
    , _jet_index()
    , _full_scale_momentum()
    , _em_scale_momentum()
    , _cs_scale_momentum()
    , _has_cs_scale(false)
    , _full_scale_E(0.)
    , _em_scale_correction(1.)
    , _emf( std::numeric_limits<ftype>::max() ) // if unfilled, default fail cuts
    , _vtx_fraction(1) // if unfilled, default fails passes
    , _n90( -std::numeric_limits<ftype>::max() ) // if unfilled, default fails cuts
    , _hecf( std::numeric_limits<ftype>::max() ) 
    , _quality()
    , _LArQuality()
    , _AverageLArQF()
    , _HECQuality()
    , _negativeE()
    , _sumPtTrack()
    , _sumPtTrack_allpv()
    , _fmax()
    , _timing()
    , _tgap3f()
    , _EtaOriginEM( 0. )
    , _PhiOriginEM( 0. )
    , _MOriginEM( 0. )
    , _EtaOrigin( 0. )
    , _PhiOrigin( 0. )
    , _MOrigin( 0. )
    , _is_bad_loose( 1 )
    , _BCH_CORR_CELL( std::numeric_limits<ftype>::max() )
    , _BCH_CORR_JET( std::numeric_limits<ftype>::max() )
    , _ntrkjetalg( -std::numeric_limits<int>::max() )
    , _ntrkjetalg_allpv( -std::numeric_limits<int>::max() )
    , _met_wet( 0. )
    , _met_wpx( 0. )
    , _met_wpy( 0. )
    , _met_status( 0. )
    , _gcwjes( 0. )
    , _dr_b_parton( std::numeric_limits<ftype>::max() )
    , _dr_c_parton( std::numeric_limits<ftype>::max() )
    , _dr_light_parton( std::numeric_limits<ftype>::max() )
    , _dr_b_parton_any( std::numeric_limits<ftype>::max() )
    , _dr_c_parton_any( std::numeric_limits<ftype>::max() )
    , _dr_light_parton_any( std::numeric_limits<ftype>::max() )
    , _eta_highest_pt_parton( std::numeric_limits<ftype>::max() )
    , _phi_highest_pt_parton( std::numeric_limits<ftype>::max() )
    , _pdg_label( -std::numeric_limits<int>::max() )
    , _calowidth( -1. )
    , _trackwidth( -1. )
    , _trackwidth_allpv( -1 )
    , _primary_vertex_x( 0. )
    , _primary_vertex_y( 0. )
    , _primary_vertex_z( 0. )
    , _tag_l2_xd0(-999)
    , _tag_l2_xz0(-999)
    , _tag_l2_x2d(-999)
    , _tag_ef_xd0(-999)
    , _tag_ef_xz0(-999)
    , _tag_ef_x2d(-999)
    , _tag_ip2d_w(-999)
    , _tag_ip3d_w(-999)
    , _tag_sv0_w(-999)
    , _tag_sv1_w(-999)
    , _tag_sv2_w(-999)
    , _tag_cmb_w(-999)
    , _tag_cmbnn_w(-999)
    , _tag_jetp_w(-999)
    , _tag_jetpneg_w(-999)
    , _tag_trkc_w(-999)
    , _tag_pgs(0.)
    , _uncorrected_parent()
  {}
  ~AnaJet() {}
  
  const Algorithm& algorithm() const { return _alg; }
  const int jet_index() const { return _jet_index; }
  const Algorithm& author() const { return algorithm(); }
  const bool is_atlas_cone_4_tower() const { return _alg==Algorithm::ATLAS_CONE_4_TOWER; }
  const bool is_atlas_cone_7_tower() const { return _alg==Algorithm::ATLAS_CONE_7_TOWER; }
  const bool is_atlas_cone_4_truth() const { return _alg==Algorithm::ATLAS_CONE_4_TRUTH; }
  const bool is_atlas_cone_4_truth_pileup() const { return _alg==Algorithm::ATLAS_CONE_4_TRUTH_PILEUP; }
  const bool is_atlas_cone_6_truth() const { return _alg==Algorithm::ATLAS_CONE_6_TRUTH; }
  const bool is_atlas_cone_6_truth_pileup() const { return _alg==Algorithm::ATLAS_CONE_6_TRUTH_PILEUP; }
  const bool is_atlas_anti_kt_cone_4_h1_topo() const { return _alg==Algorithm::ATLAS_ANTI_KT_CONE_4_H1_TOPO; }
  const bool is_atlas_anti_kt_cone_6_h1_topo() const { return _alg==Algorithm::ATLAS_ANTI_KT_CONE_6_H1_TOPO; }
  const bool is_atlas_anti_kt_cone_4_h1_lc_topo() const { return _alg==Algorithm::ATLAS_ANTI_KT_CONE_4_H1_LC_TOPO; }
  const bool is_atlas_anti_kt_cone_6_h1_lc_topo() const { return _alg==Algorithm::ATLAS_ANTI_KT_CONE_6_H1_LC_TOPO; }
  const bool is_atlas_met_goodness() const { return _alg==Algorithm::ATLAS_MET_GOODNESS; }
  const bool is_l1_roi() const { return _alg==Algorithm::L1_ROI; }

  // track parameters, uncertainties, and significances (pulls)
  const ftype centroid_eta() const { return _momentum.Eta(); }
  const ftype centroid_phi() const { return _momentum.Phi(); }

  const TLorentzVector& full_scale_momentum() const { return _full_scale_momentum; }
  const ftype full_scale_E() const { return _full_scale_E; }


  const ftype full_scale_pt() const { return _full_scale_momentum.Pt(); }
  const ftype em_scale_pt() const { return _em_scale_momentum.Pt(); }
  const ftype cs_scale_pt() const { return _has_cs_scale ? _cs_scale_momentum.Pt() : std::numeric_limits<ftype>::max(); }
  const ftype hem_scale_pt() const { return _momentum.Pt(); }

  const ftype em_scale_eta() const { return _em_scale_momentum.Eta(); }
  const ftype em_scale_phi() const { return _em_scale_momentum.Phi(); }

  const ftype cs_scale_eta() const { return _has_cs_scale ? _cs_scale_momentum.Eta() : std::numeric_limits<ftype>::max(); }
  const ftype cs_scale_phi() const { return _has_cs_scale ? _cs_scale_momentum.Phi() : std::numeric_limits<ftype>::max(); }

  // em scale variables, primarily for corrections to four-vector
  const TLorentzVector& em_scale_momentum() const { return _em_scale_momentum; }
  const ftype em_scale_correction() const { return _em_scale_correction; }

  const TLorentzVector& cs_scale_momentum() const { return _cs_scale_momentum; }
  const bool& has_cs_scale() const { return _has_cs_scale; }
  const ftype EtaOriginEM() const { return _EtaOriginEM; }
  const ftype PhiOriginEM() const { return _PhiOriginEM; }
  const ftype MOriginEM() const { return _MOriginEM; }
  const ftype EtaOrigin() const { return _EtaOrigin; }
  const ftype PhiOrigin() const { return _PhiOrigin; }
  const ftype MOrigin() const { return _MOrigin; }
  
  const ftype sumpttrk() const { return *_sumPtTrack; }
  const ftype sumpttrk_allpv() const { return *_sumPtTrack_allpv; }

  const ftype emf() const { return _emf; }
  const ftype emfrac() const { return _emf; }
  const ftype chf() const { return detector::safe_divide( *_sumPtTrack , static_cast<ftype>(_full_scale_momentum.Pt()) ); }

  // for jets that have been corrected using functions in AnaJet.cpp
  const bool is_corrected() const { return _uncorrected_parent; }
  const boost::shared_ptr<const AnaJet> uncorrected_parent() const { return _uncorrected_parent; }
  const ftype highest_parent_pt() const {
    if( !is_corrected() ) return pt();
    boost::shared_ptr<const AnaJet> tmpjet = uncorrected_parent();
    while( tmpjet->is_corrected() ) tmpjet = tmpjet->uncorrected_parent();
    return tmpjet->pt();
  }
  const ftype highest_parent_eta() const {
    if( !is_corrected() ) return eta();
    boost::shared_ptr<const AnaJet> tmpjet = uncorrected_parent();
    while( tmpjet->is_corrected() ) tmpjet = tmpjet->uncorrected_parent();
    return tmpjet->eta();
  }
  
  const bool is_bad() const {
    return (  (!_hecf || (_n90 <= 5 && *_hecf > 0.8)) ||
              (!_quality || !(_emf < std::numeric_limits<ftype>::max()) ||
               (std::abs(*_quality) > 0.8 && _emf > 0.95) ) ||
              (!(_timing < std::numeric_limits<ftype>::max()) ||
               std::abs(*_timing) >= 50. )  );
  }
  // Medium -- https://twiki.cern.ch/twiki/bin/view/AtlasProtected/HowToCleanJets#Bad_jets_rel16_data
  // 20110215
  const bool is_ugly() const {
    if( !_tgap3f ) { return true; }
    if( !(_BCH_CORR_CELL < std::numeric_limits<ftype>::max()) ) { return true; }
    const bool dotx = (*_tgap3f) > 0.5  ||  _BCH_CORR_CELL > 0.5; // fixme: should is be an AND or an OR?
    return dotx;
  }
  const bool is_good() const { return !(is_bad() || is_ugly()); }

  /* MET cleaning criteria from summer 2010 conferences:
     "Reject the event if at least one AntiKt4TopoJet with pT(EMScale)>10 GeV has a badJet in the event"
     "badJet = (|JetQuality|>0.8 && emf>0.95) || (n90<=5 && fHEC>0.8) || |jetTime|>50 ns"
  */
  const bool is_good_for_met() const {
    return ( _em_scale_momentum.Pt() <= 10. || !is_bad() );
  }
  const bool is_bad_for_met() const { return !is_good_for_met(); }
  const int is_bad_loose() const { return _is_bad_loose; }

  ////////////////////////////////////////////////////////
  // Release-17 (MC11A/MC11B) bad/ugly jets
  ////////////////////////////////////////////////////////

  // Looser -- https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/HowToCleanJets2011
  const bool isBadJet_looser_v17() const {
    const float emf = _emf;
    const float hecf = *_hecf;
    const float quality = *_LArQuality;
    const float AvgLArQ = *_AverageLArQF;
    const float HecQ = *_HECQuality;
    const float sumpttrk = *_sumPtTrack;
    const float eta = _em_scale_momentum.Eta();
    const float pt = _full_scale_momentum.Pt();
    const float fmax = *_fmax;
    const float NegE = (*_negativeE)*1000.;
    const float chf = pt != 0 ? sumpttrk/pt : 0;
    const float LArQmean = AvgLArQ/65335. ; 


    if( !_hecf || !_LArQuality || !_AverageLArQF || !_HECQuality || !_negativeE || !_fmax || !_sumPtTrack) { 
      printf("WARNING     missing variable for isBadJet_looser_v17\n");
      return true;
    }

    // HEC spike
    if( hecf>0.5 && fabs(HecQ)>0.5 && LArQmean>0.8)         return true;
    if( fabs(NegE)>60000./*MeV*/)                          return true;
    // EM coherent noise
    if( emf>0.95 && fabs(quality)>0.8 && LArQmean>0.8 && fabs(eta)<2.8 )   return true;
    // Cosmics and Beam background
    if( emf<0.05 && chf<0.05 && fabs(eta)<2. )             return true;
    if( emf<0.05 && fabs(eta)>2. )                         return true;
    if( fmax>0.99&&fabs(eta)<2)                            return true;

    return false;
  }
  
  // Loose -- https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/HowToCleanJets2011
  const bool isBadJet_loose_v17() const {
    const float emf = _emf;
    const float hecf = *_hecf;
    const float quality = *_LArQuality;
    const float HecQ = *_HECQuality;
    const float time = *_timing;
    const float eta = _em_scale_momentum.Eta();
    const float fmax = *_fmax;

    if( !_hecf || !_LArQuality || !_HECQuality || !_timing || !_fmax ) {
      printf("WARNING     missing variable for isBadJet_loose_v17\n");
      return true;
    }

    if(isBadJet_looser_v17()) return true;
    
    // HEC spike
    if( hecf>0.5 && fabs(HecQ)>0.5 )                       return true;
    // EM coherent noise
    if( emf>0.95 && fabs(quality)>0.8 && fabs(eta)<2.8 )   return true;
    // Cosmics and Beam background
    if( fabs(time)>25. )                                   return true;

    return false;
  }

  // Medium -- https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/HowToCleanJets2011
  const bool isBadJet_medium_v17() const {
    const float emf = _emf;
    const float hecf = *_hecf;
    const float quality = *_LArQuality;
    const float HecQ = *_HECQuality;
    const float time = *_timing;
    const float sumpttrk = *_sumPtTrack;
    const float eta = _em_scale_momentum.Eta();
    const float pt = _full_scale_momentum.Pt();
    const float chf = pt != 0 ? sumpttrk/pt : 0;

    if( !_hecf || !_LArQuality || !_HECQuality || !_timing ) { 
      printf("WARNING     missing variable for isBadJet_medium_v17\n");
      return true;
    }

    if(isBadJet_loose_v17()) return true;
    
    // HEC spike
    if( hecf> 1-fabs(HecQ))                               return true; 
    // EM coherent noise
    if( emf>0.9 && fabs(quality)>0.8 && fabs(eta)<2.8 )   return true;
    // Cosmics and Beam background
    if( fabs(time)>10. )                                  return true;
    if( emf < 0.05 && chf < 0.1  && fabs(eta)<2. )        return true;
    if( emf > 0.95 && chf < 0.05 && fabs(eta)<2. )        return true;

    return false;
  }

  // Tight -- https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/HowToCleanJets2011
  const bool isBadJet_tight_v17() const {
    const float emf = _emf;
    const float hecf = *_hecf;
    const float quality = *_LArQuality;
    const float HecQ = *_HECQuality;
    const float sumpttrk = *_sumPtTrack;
    const float eta = _em_scale_momentum.Eta();
    const float pt = _full_scale_momentum.Pt();
    const float chf = pt != 0 ? sumpttrk/pt : 0;

    if( !_hecf || !_LArQuality || !_HECQuality || !_sumPtTrack) {
      printf("WARNING     missing variable for isBadJet_tight_v17\n");
      return true;
    }

    if(isBadJet_medium_v17()) return true;

    // EM coherent noise
    if ( fabs(quality)>0.95 )                   return true ;
    if (emf>0.98 && fabs(quality)>0.05)         return true ;
    // Cosmics and Beam background
    if (chf < 0.2 && fabs(eta)<2.5 )           return true;
    if (emf<0.1 && fabs(eta)>2. )               return true;
    if (emf>0.9 && chf < 0.1 && fabs(eta)<2.5 ) return true;

    return false;
  }

  // Ugly - https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/HowToCleanJets
  const bool isUglyJet_v17() const {
    if( !_tgap3f || !(_BCH_CORR_CELL < std::numeric_limits<ftype>::max()) ) {
      printf("WARNING     missing variable for isUglyJet_v17\n");
      return true; 
    }

    const float fcor = _BCH_CORR_CELL;
    const float tileGap3f = *_tgap3f;

    if (fcor>0.5) return true;
    if (tileGap3f >0.5) return true;


    return false;
  }

  const bool is_good_looser_for_met_jetetmiss_v17() const {
    if( _momentum.Pt() > 20. && (isBadJet_looser_v17()) ) return false;
    // for now, JetEtMiss recommends NOT to filter out ugly jets (01/22/2012)
    //if( _momentum.Pt() > 20. && (isBadJet_looser_v17() || isUglyJet_v17()) ) return false;
    return true;
  }
  const bool is_good_loose_for_met_jetetmiss_v17() const {
    if( _momentum.Pt() > 20. && (isBadJet_loose_v17() ) ) return false;
    // for now, JetEtMiss recommends NOT to filter out ugly jets (01/22/2012)
    //if( _momentum.Pt() > 20. && (isBadJet_loose_v17() || isUglyJet_v17()) ) return false;
    return true;
  }


  ////////////////////////////////////////////////////////
  // Release-16 LAr hole cleaning
  ////////////////////////////////////////////////////////
  
  // https://twiki.cern.ch/twiki/pub/AtlasProtected/HowToCleanJets/LArHole.hpp
  const bool IsLArHole(float eta, float phi,
                       float etamin = -0.1, float etamax = 1.5,
                       float phimin = -0.9, float phimax = -0.5)  const {
    if (eta < etamin || eta > etamax) return false;
    if (phi < phimin || phi > phimax) return false;
    return true;
  }
  
  const bool isBadJet_FEBhole() const {
    const float eta = _momentum.Eta();
    const float phi = _momentum.Phi();
    const float pt = _momentum.Pt();
    //    std::cout << "pt, eta, phi " << pt << " " << eta << " " << phi << std::endl;
    if (pt>25. && IsLArHole(eta, phi)) {
      // std::cout << "IN HOLE" << std::endl;
      return true;
    }
    else {
      // std::cout << "NOT IN HOLE" << std::endl;
      return false;
    }
  }  

  /*
  const bool isBadJet_FEB() const {
   //bool passJet(double eta, double phi) { return !((eta < 1.85 && eta > -0.4) && (phi < -0.192 && phi > -1.188)); }
    float dr = 0.2;

    float eta = _em_scale_momentum.Eta();
    float phi = _em_scale_momentum.Phi();

    if ((eta < (1.45+dr) && eta > (0-dr)) && (phi < (-0.592+dr) && phi > (-0.788-dr)))
      return true;
    
    return false;
  }
  */

  const ftype vtx_fraction() const { return _vtx_fraction; }
  const ftype n90() const { return _n90; }
  const boost::optional<ftype> hecf() const { return _hecf; }
  const boost::optional<ftype> quality() const { return _quality; }
  const boost::optional<ftype> fmax() const { return _fmax; }
  const boost::optional<ftype> timing() const { return _timing; }
  const boost::optional<ftype> tgap3f() const { return _tgap3f; }
  const ftype BCH_CORR_CELL() const { return _BCH_CORR_CELL; }
  const ftype BCH_CORR_JET() const { return _BCH_CORR_JET; }
  const ftype f_BCH_CORR_JET() const { return _BCH_CORR_JET; }
  const ftype f_BCH_CORR_CELL() const { return _BCH_CORR_CELL; }
  const int ntrkjetalg() const { return _ntrkjetalg; }
  const int ntrkjetalg_allpv() const { return _ntrkjetalg; }
  
  const ftype met_wet() const { return _met_wet; }
  const ftype met_wpx() const { return _met_wpx; }
  const ftype met_wpy() const { return _met_wpy; }
  const ftype met_status() const { return _met_status; }
  const ftype gcwjes() const { return _gcwjes; }

  const ftype dr_b_parton() const { return _dr_b_parton; }
  const ftype dr_c_parton() const { return _dr_c_parton; }
  const ftype dr_light_parton() const { return _dr_light_parton; }
  const ftype dr_b_parton_any() const { return _dr_b_parton_any; }
  const ftype dr_c_parton_any() const { return _dr_c_parton_any; }
  const ftype dr_light_parton_any() const { return _dr_light_parton_any; }
  const ftype eta_highest_pt_parton() const { return _eta_highest_pt_parton; }
  const ftype phi_highest_pt_parton() const { return _phi_highest_pt_parton; }
  const int pdg_label() const { return _pdg_label; }

  const ftype calowidth() const { return _calowidth; }
  const ftype trackwidth() const { return _trackwidth; }
  const ftype trackwidth_allpv() const { return _trackwidth_allpv; }

  const ftype primary_vertex_x() const { return _primary_vertex_x; }
  const ftype primary_vertex_y() const { return _primary_vertex_y; }
  const ftype primary_vertex_z() const { return _primary_vertex_z; }

  const ftype tag_l2_xd0() const { return _tag_l2_xd0; }
  const ftype tag_l2_xz0() const { return _tag_l2_xz0; }
  const ftype tag_l2_x2d() const { return _tag_l2_x2d; }
  const ftype tag_ef_xd0() const { return _tag_ef_xd0; }
  const ftype tag_ef_xz0() const { return _tag_ef_xz0; }
  const ftype tag_ef_x2d() const { return _tag_ef_x2d; }
  const ftype tag_ip2d_w() const { return _tag_ip2d_w; }
  const ftype tag_ip3d_w() const { return _tag_ip3d_w; }
  const ftype tag_sv0_w() const { return _tag_sv0_w; }
  const ftype tag_sv1_w() const { return _tag_sv1_w; }
  const ftype tag_sv2_w() const { return _tag_sv2_w; }
  const ftype tag_cmb_w() const { return _tag_cmb_w; }
  const ftype tag_cmbnn_w() const { return _tag_cmbnn_w; }
  const ftype tag_jetp_w() const { return _tag_jetp_w; }
  const ftype tag_jetpneg_w() const { return _tag_jetpneg_w; }
  const ftype tag_trkc_w() const { return _tag_trkc_w; }
  const ftype tag_pgs() const { return _tag_pgs; }

  const std::vector<AnaJet::Hadron>& associated_hadrons() const { return _associated_hadrons; }
  
  const ftype min_dr_hadron( const AnaJet::Hadron::Type& type , AnaJet::Hadron& mini ) const {
    ftype mindr = std::numeric_limits<ftype>::max();
    for( std::vector<Hadron>::const_iterator i=_associated_hadrons.begin(), f=_associated_hadrons.end(); i!=f; ++i ) {
      if( type!=Hadron::UNKNOWN && i->type() != type ) { continue; }
      const ftype dr = std::max( std::abs(static_cast<ftype>(i->eta())-eta()) , 
                                 std::abs( detector::delta_phi(static_cast<ftype>(i->phi()),phi()) ) );
      if( dr < mindr ) {
        mindr = dr;
        mini = *i;
      }
    }
    return mindr;
  }
  const bool contains_b_hadron_within( const ftype& dr ) const {
    static Hadron had;
    return( min_dr_hadron(Hadron::B_QUARK,had)<=dr );
  }
  const bool contains_c_hadron_within( const ftype& dr ) const { 
    static Hadron had;
    return( min_dr_hadron(Hadron::C_QUARK,had)<=dr );
  }
  const bool contains_light_hadron_within( const ftype& dr ) const { 
    static Hadron had;
    return( min_dr_hadron(Hadron::LIGHT_QUARK,had)<=dr ); 
  }

  // v17 functions
  static const boost::shared_ptr<const AnaJet> jet_JESpileupUncertainty_v17( const boost::shared_ptr<const AnaJet>& jet , const double& mu,const int& nGoodPV , const double& direction , const CONF::ConfType& conf , const JES_VARIATION& jvar );
  static const boost::shared_ptr<const AnaJet> jet_multijetJESpileupUncertainty_v17( const boost::shared_ptr<const AnaJet>& jet , const double& mu ,
										     const int& nGoodPV , const double& dRmin , 
										     const double& direction ,const CONF::ConfType& conf,
										     bool isbjet=false);
  static const boost::shared_ptr<const AnaJet> jet_CalibrationTools_v17( const boost::shared_ptr<const AnaJet>& jet , 
									 const unsigned long& evtnum , const int& jetnum , 
									 const double& mu, const int& nGoodPV , const bool& is_mc , 
									 const bool& calibrate, const bool& smear,const CONF::ConfType& conf);
  static const boost::shared_ptr<const AnaJet> jet_CalibrationTools_v17_v2( const boost::shared_ptr<const AnaJet>& jet , 
									    const unsigned long& evtnum , const int& jetnum , 
									    const double& mu, const int& nGoodPV , const bool& is_mc , 
									    const bool& calibrate, const int& smear,const CONF::ConfType& conf);
  static boost::shared_ptr<const AnaJet> jetetmiss_calibrate( const CONF::ConfType& conf ,
							      const boost::shared_ptr<const AnaJet>& jet ,
							      const unsigned long& event_number , const int& jet_number ,
							      const double& mu , const int& npv , const bool& is_mc ,
							      const bool& calibrate, const bool& smear );
  static boost::shared_ptr<const AnaJet> jetetmiss_calibrate2( const CONF::ConfType& conf ,
							       const boost::shared_ptr<const AnaJet>& jet ,
							       const unsigned long& event_number , const int& jet_number ,
							       const double& mu , const int& npv , const bool& is_mc ,
							       const bool& calibrate, const int& smear );

  static boost::shared_ptr<const AnaJet> jetetmiss_scale( const CONF::ConfType& conf , const boost::shared_ptr<const AnaJet>& jet ,
							  const unsigned long& event_number , const int& njets , const double& drmin , const int& jet_number ,
							  const double& mu , const int& npv , const std::string& fun, bool isbjet=false);
  
  std::ostream& print( std::ostream& os = std::cout ) const {
    os << boost::format("AnaJet: %|10| et=%|+10|(GeV) eta=%|10| phi=%|10|") % algorithm().name() % et() % eta() % phi() << std::endl;
    return os;
  }
  
};

BOOST_CLASS_VERSION( AnaJet::Algorithm , 2 );
BOOST_CLASS_VERSION( AnaJet::Hadron , 1 );
BOOST_CLASS_VERSION( AnaJet , 3 );

#endif // WRAP_ANAJET_HPP
