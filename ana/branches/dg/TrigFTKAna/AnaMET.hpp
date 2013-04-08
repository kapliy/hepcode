#ifndef WRAP_ANAMET_HPP
#define WRAP_ANAMET_HPP

// ANAMET
// ================================================================
// Data for MET.
// ================================================================
// 2009-07-15 Antonio Boveia (boveia@hep.uchicago.edu)

#include <numeric>
#include <cmath>
#include <string>
#include <iostream>
#include <vector>
#include <utility>
#include <boost/shared_ptr.hpp>
#include <boost/format.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/version.hpp>
#include <TLorentzVector.h>
#include "TrigFTKAna/AnaParticle.hpp"
#include "TrigFTKAna/AnaMuon.hpp"
#include "TrigFTKAna/Detector.hpp"
//#include "MissingETUtility/METUtility.h"


class AnaMuon;

class
AnaMET : public AnaParticle
{

public:

  typedef AnaParticle::ftype ftype;
  typedef enum { UNKNOWN
                 , TRUTH
		 , TRUTH_INT
		 , TRUTH_NONINT
		 , TRUTH_MUONS
                 , FINAL
                 , REF_FINAL
		 , REF_FINAL_45
                 , TOPO
                 , LOCHADTOPO
                 , LOCHADTOPOMU
                 , LOCHADTOPOMUHIGHETA
                 , LOCHADTOPOELHIGHETA
                 , LOCHADTOPOCEF
                 , ICHEP
                 , REF_FINAL_ANTIKT_EMJES
                 , RAW
                 , REF_FINAL_EM
  } MetType;

protected:

  MetType _met_type;
  ftype _sumet;

  // component calculation
  ftype _MET_RefMuonTrack_etx;
  ftype _MET_RefMuonTrack_ety;
  ftype _MET_cal_etx;
  ftype _MET_cal_ety;
  ftype _MET_MuonBoy_etx;
  ftype _MET_MuonBoy_ety;
  ftype _MET_cal_etx_CentralReg;
  ftype _MET_cal_ety_CentralReg;
  ftype _MET_cal_etx_EndcapRegion;
  ftype _MET_cal_ety_EndcapRegion;
  ftype _MET_cal_etx_ForwardReg;
  ftype _MET_cal_ety_ForwardReg;

  ftype _refgamma;
  ftype _refgamma_phi;
  ftype _refgamma_sumet;
  ftype _refmuon;
  ftype _refmuon_phi;
  ftype _refmuon_sumet;
  ftype _refmuon_track;
  ftype _refmuon_track_phi;
  ftype _refmuon_track_sumet;
  ftype _refele;
  ftype _refele_phi;
  ftype _refele_sumet;
  ftype _refjet;
  ftype _refjet_phi;
  ftype _refjet_sumet;
  ftype _reftau;
  ftype _reftau_phi;
  ftype _reftau_sumet;
  ftype _cellout_eflow;
  ftype _cellout_eflow_phi;
  ftype _cellout_eflow_sumet;
  ftype _cellout;
  ftype _cellout_phi;
  ftype _cellout_sumet;
  ftype _softjets;
  ftype _softjets_phi;
  ftype _softjets_sumet;
  ftype _cryo;
  ftype _cryo_phi;
  ftype _cryo_sumet;
  ftype _muonboy;
  ftype _muonboy_phi;
  ftype _muonboy_sumet;
  ftype _lochad_topo;
  ftype _lochad_topo_phi;
  ftype _lochad_topo_sumet;
  ftype _lochad_topo_ichep;
  ftype _lochad_topo_ichep_phi;
  ftype _lochad_topo_ichep_sumet;
  ftype _truth_int;
  ftype _truth_int_phi;
  ftype _truth_int_sumet;
  ftype _truth_nonint;
  ftype _truth_nonint_phi;
  ftype _truth_nonint_sumet;

  bool _is_good;

  // pointer back to uncorrected met, if this met has been
  // corrected. note that this is not serialized, so any corrections
  // should be made at the ana_* executable level and not at the
  // ntuple reading level.
  mutable boost::shared_ptr<const AnaMET> _uncorrected_parent;

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
  void serialize( archiveT& ar , const unsigned int version ) {
    assert( version==3 && "AnaMET is not backward compatible with earlier versions. Remake your AnaEventNtuples." );
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(AnaParticle);
    ar & boost::serialization::make_nvp("met_type",_met_type);
    ar & boost::serialization::make_nvp("sumet",_sumet);
    ar & boost::serialization::make_nvp("RefMuonTrack_etx",_MET_RefMuonTrack_etx);
    ar & boost::serialization::make_nvp("RefMuonTrack_ety",_MET_RefMuonTrack_ety);
    ar & boost::serialization::make_nvp("cal_etx",_MET_cal_etx);
    ar & boost::serialization::make_nvp("cal_ety",_MET_cal_ety);
    ar & boost::serialization::make_nvp("MuonBoy_etx",_MET_MuonBoy_etx);
    ar & boost::serialization::make_nvp("MuonBoy_ety",_MET_MuonBoy_ety);
    ar & boost::serialization::make_nvp("cal_etx_CentralReg",_MET_cal_etx_CentralReg);
    ar & boost::serialization::make_nvp("cal_ety_CentralReg",_MET_cal_ety_CentralReg);
    ar & boost::serialization::make_nvp("cal_etx_EndcapRegion",_MET_cal_etx_EndcapRegion);
    ar & boost::serialization::make_nvp("cal_ety_EndcapRegion",_MET_cal_ety_EndcapRegion);
    ar & boost::serialization::make_nvp("cal_etx_ForwardReg",_MET_cal_etx_ForwardReg);
    ar & boost::serialization::make_nvp("refgamma",_refgamma);
    ar & boost::serialization::make_nvp("refgamma_phi",_refgamma_phi);
    ar & boost::serialization::make_nvp("refgamma_sumet",_refgamma_sumet);
    ar & boost::serialization::make_nvp("refmuon",_refmuon);
    ar & boost::serialization::make_nvp("refmuon_phi",_refmuon_phi);
    ar & boost::serialization::make_nvp("refmuon_sumet",_refmuon_sumet);
    ar & boost::serialization::make_nvp("refmuon_track",_refmuon_track);
    ar & boost::serialization::make_nvp("refmuon_track_phi",_refmuon_track_phi);
    ar & boost::serialization::make_nvp("refmuon_track_sumet",_refmuon_track_sumet);
    ar & boost::serialization::make_nvp("refele",_refele);
    ar & boost::serialization::make_nvp("refele_phi",_refele_phi);
    ar & boost::serialization::make_nvp("refele_sumet",_refele_sumet);
    ar & boost::serialization::make_nvp("refjet",_refjet);
    ar & boost::serialization::make_nvp("refjet_phi",_refjet_phi);
    ar & boost::serialization::make_nvp("refjet_sumet",_refjet_sumet);
    ar & boost::serialization::make_nvp("reftau",_reftau);
    ar & boost::serialization::make_nvp("reftau_phi",_reftau_phi);
    ar & boost::serialization::make_nvp("reftau_sumet",_reftau_sumet);
    ar & boost::serialization::make_nvp("cellout_eflow",_cellout_eflow);
    ar & boost::serialization::make_nvp("cellout_eflow_phi",_cellout_eflow_phi);
    ar & boost::serialization::make_nvp("cellout_eflow_sumet",_cellout_eflow_sumet);
    ar & boost::serialization::make_nvp("cellout",_cellout);
    ar & boost::serialization::make_nvp("cellout_phi",_cellout_phi);
    ar & boost::serialization::make_nvp("cellout_sumet",_cellout_sumet);
    ar & boost::serialization::make_nvp("softjets",_softjets);
    ar & boost::serialization::make_nvp("softjets_phi",_softjets_phi);
    ar & boost::serialization::make_nvp("softjets_sumet",_softjets_sumet);
    ar & boost::serialization::make_nvp("cryo",_cryo);
    ar & boost::serialization::make_nvp("cryo_phi",_cryo_phi);
    ar & boost::serialization::make_nvp("cryo_sumet",_cryo_sumet);
    ar & boost::serialization::make_nvp("muonboy",_muonboy);
    ar & boost::serialization::make_nvp("muonboy_phi",_muonboy_phi);
    ar & boost::serialization::make_nvp("muonboy_sumet",_muonboy_sumet);
    ar & boost::serialization::make_nvp("lochad_topo",_lochad_topo);
    ar & boost::serialization::make_nvp("lochad_topo_phi",_lochad_topo_phi);
    ar & boost::serialization::make_nvp("lochad_topo_sumet",_lochad_topo_sumet);
    ar & boost::serialization::make_nvp("lochad_topo_ichep",_lochad_topo_ichep);
    ar & boost::serialization::make_nvp("lochad_topo_ichep_phi",_lochad_topo_ichep_phi);
    ar & boost::serialization::make_nvp("lochad_topo_ichep_sumet",_lochad_topo_ichep_sumet);
    ar & boost::serialization::make_nvp("truth_int",_truth_int);
    ar & boost::serialization::make_nvp("truth_int_phi",_truth_int_phi);
    ar & boost::serialization::make_nvp("truth_int_sumet",_truth_int_sumet);
    ar & boost::serialization::make_nvp("truth_nonint",_truth_nonint);
    ar & boost::serialization::make_nvp("truth_nonint_phi",_truth_nonint_phi);
    ar & boost::serialization::make_nvp("truth_nonint_sumet",_truth_nonint_sumet);
    ar & boost::serialization::make_nvp("cal_ety_ForwardReg",_MET_cal_ety_ForwardReg);
    ar & boost::serialization::make_nvp("is_good",_is_good);
  }

public:

  AnaMET()
    : _met_type( UNKNOWN )
    , _sumet( 0. )
    , _MET_RefMuonTrack_etx( 0. )
    , _MET_RefMuonTrack_ety( 0. )
    , _MET_cal_etx( 0. )
    , _MET_cal_ety( 0. )
    , _MET_MuonBoy_etx( 0. )
    , _MET_MuonBoy_ety( 0. )
    , _MET_cal_etx_CentralReg( 0. )
    , _MET_cal_ety_CentralReg( 0. )
    , _MET_cal_etx_EndcapRegion( 0. )
    , _MET_cal_ety_EndcapRegion( 0. )
    , _MET_cal_etx_ForwardReg( 0. )
    , _MET_cal_ety_ForwardReg( 0. )
    , _refgamma( 0. )
    , _refgamma_phi( 0. )
    , _refgamma_sumet( 0. )
    , _refmuon( 0. )
    , _refmuon_phi( 0. )
    , _refmuon_sumet( 0. )
    , _refmuon_track( 0. )
    , _refmuon_track_phi( 0. )
    , _refmuon_track_sumet( 0. )
    , _refele( 0. )
    , _refele_phi( 0. )
    , _refele_sumet( 0. )
    , _refjet( 0. )
    , _refjet_phi( 0. )
    , _refjet_sumet( 0. )
    , _reftau( 0. )
    , _reftau_phi( 0. )
    , _reftau_sumet( 0. )
    , _cellout_eflow( 0. )
    , _cellout_eflow_phi( 0. )
    , _cellout_eflow_sumet( 0. )
    , _cellout( 0. )
    , _cellout_phi( 0. )
    , _cellout_sumet( 0. )
    , _softjets( 0. )
    , _softjets_phi( 0. )
    , _softjets_sumet( 0. )
    , _cryo( 0. )
    , _cryo_phi( 0. )
    , _cryo_sumet( 0. )
    , _muonboy( 0. )
    , _muonboy_phi( 0. )
    , _muonboy_sumet( 0. )
    , _lochad_topo( 0. )
    , _lochad_topo_phi( 0. )
    , _lochad_topo_sumet( 0. )
    , _lochad_topo_ichep( 0. )
    , _lochad_topo_ichep_phi( 0. )
    , _lochad_topo_ichep_sumet( 0. )
    , _truth_int( 0. )
    , _truth_int_phi( 0. )
    , _truth_int_sumet( 0. )
    , _truth_nonint( 0. )
    , _truth_nonint_phi( 0. )
    , _truth_nonint_sumet( 0. )
    , _is_good( false )
    , AnaParticle( AnaParticle::MET )
  {}
  ~AnaMET() {}

  const MetType& met_type() const { return _met_type; }
  const ftype sumet() const { return _sumet; }
  const ftype xs() const { return detector::safe_divide( double(pt()) , double(0.5 * sqrt(_sumet)) ); }
  const ftype sum_et() const { return pt(); }
  const ftype sum_et_x() const { return -px(); }
  const ftype sum_et_y() const { return -py(); }
  const ftype cal_sum_et() const { return detector::quadrature(_MET_cal_etx,_MET_cal_ety); }
  const ftype muon_sum_et() const { return detector::quadrature(_MET_MuonBoy_etx,_MET_MuonBoy_ety); }
  const ftype met() const { return pt(); }
  const ftype metphi() const { return phi(); }
  const bool is_good() const { return _is_good; }

  const ftype refgamma() const { return _refgamma; }
  const ftype refgamma_phi() const { return _refgamma_phi; }
  const ftype refgamma_sumet() const { return _refgamma_sumet; }
  const ftype refgamma_etx() const { return _refgamma * cos(_refgamma_phi); }
  const ftype refgamma_ety() const { return _refgamma * sin(_refgamma_phi); }

  const ftype refmuon() const { return _refmuon; }
  const ftype refmuon_phi() const { return _refmuon_phi; }
  const ftype refmuon_sumet() const { return _refmuon_sumet; }
  const ftype refmuon_etx() const { return _refmuon * cos(_refmuon_phi); }
  const ftype refmuon_ety() const { return _refmuon * sin(_refmuon_phi); }

  const ftype refmuon_track() const { return _refmuon_track; }
  const ftype refmuon_track_phi() const { return _refmuon_track_phi; }
  const ftype refmuon_track_sumet() const { return _refmuon_track_sumet; }
  const ftype refmuon_track_etx() const { return _refmuon_track * cos(_refmuon_track_phi); }
  const ftype refmuon_track_ety() const { return _refmuon_track * sin(_refmuon_track_phi); }

  const ftype refele() const { return _refele; }
  const ftype refele_phi() const { return _refele_phi; }
  const ftype refele_sumet() const { return _refele_sumet; }
  const ftype refele_etx() const { return _refele * cos(_refele_phi); }
  const ftype refele_ety() const { return _refele * sin(_refele_phi); }

  const ftype refjet() const { return _refjet; }
  const ftype refjet_phi() const { return _refjet_phi; }
  const ftype refjet_sumet() const { return _refjet_sumet; }
  const ftype refjet_etx() const { return _refjet * cos(_refjet_phi); }
  const ftype refjet_ety() const { return _refjet * sin(_refjet_phi); }

  const ftype reftau() const { return _reftau; }
  const ftype reftau_phi() const { return _reftau_phi; }
  const ftype reftau_sumet() const { return _reftau_sumet; }
  const ftype reftau_etx() const { return _reftau * cos(_reftau_phi); }
  const ftype reftau_ety() const { return _reftau * sin(_reftau_phi); }

  const ftype cellout_eflow() const { return _cellout_eflow; }
  const ftype cellout_eflow_phi() const { return _cellout_eflow_phi; }
  const ftype cellout_eflow_sumet() const { return _cellout_eflow_sumet; }
  const ftype cellout_eflow_etx() const { return _cellout_eflow * cos(_cellout_eflow_phi); }
  const ftype cellout_eflow_ety() const { return _cellout_eflow * sin(_cellout_eflow_phi); }

  const ftype cellout() const { return _cellout; }
  const ftype cellout_phi() const { return _cellout_phi; }
  const ftype cellout_sumet() const { return _cellout_sumet; }
  const ftype cellout_etx() const { return _cellout * cos(_cellout_phi); }
  const ftype cellout_ety() const { return _cellout * sin(_cellout_phi); }

  const ftype softjets() const { return _softjets; }
  const ftype softjets_phi() const { return _softjets_phi; }
  const ftype softjets_sumet() const { return _softjets_sumet; }
  const ftype softjets_etx() const { return _softjets * cos(_softjets_phi); }
  const ftype softjets_ety() const { return _softjets * sin(_softjets_phi); }

  const ftype cryo() const { return _cryo; }
  const ftype cryo_phi() const { return _cryo_phi; }
  const ftype cryo_sumet() const { return _cryo_sumet; }
  const ftype cryo_etx() const { return _cryo * cos(_cryo_phi); }
  const ftype cryo_ety() const { return _cryo * sin(_cryo_phi); }

  const ftype muonboy() const { return _muonboy; }
  const ftype muonboy_phi() const { return _muonboy_phi; }
  const ftype muonboy_sumet() const { return _muonboy_sumet; }
  const ftype muonboy_etx() const { return _muonboy * cos(_muonboy_phi); }
  const ftype muonboy_ety() const { return _muonboy * sin(_muonboy_phi); }

  const ftype lochad_topo() const { return _lochad_topo; }
  const ftype lochad_topo_phi() const { return _lochad_topo_phi; }
  const ftype lochad_topo_sumet() const { return _lochad_topo_sumet; }
  const ftype lochad_topo_etx() const { return _lochad_topo * cos(_lochad_topo_phi); }
  const ftype lochad_topo_ety() const { return _lochad_topo * sin(_lochad_topo_phi); }

  const ftype lochad_topo_ichep() const { return _lochad_topo_ichep; }
  const ftype lochad_topo_ichep_phi() const { return _lochad_topo_ichep_phi; }
  const ftype lochad_topo_ichep_sumet() const { return _lochad_topo_ichep_sumet; }
  const ftype lochad_topo_ichep_etx() const { return _lochad_topo_ichep * cos(_lochad_topo_ichep_phi); }
  const ftype lochad_topo_ichep_ety() const { return _lochad_topo_ichep * sin(_lochad_topo_ichep_phi); }

  const ftype truth_int() const { return _truth_int; }
  const ftype truth_int_phi() const { return _truth_int_phi; }
  const ftype truth_int_sumet() const { return _truth_int_sumet; }
  const ftype truth_int_etx() const { return _truth_int * cos(_truth_int_phi); }
  const ftype truth_int_ety() const { return _truth_int * sin(_truth_int_phi); }

  const ftype truth_nonint() const { return _truth_nonint; }
  const ftype truth_nonint_phi() const { return _truth_nonint_phi; }
  const ftype truth_nonint_sumet() const { return _truth_nonint_sumet; }
  const ftype truth_nonint_etx() const { return _truth_nonint * cos(_truth_nonint_phi); }
  const ftype truth_nonint_ety() const { return _truth_nonint * sin(_truth_nonint_phi); }

  template<class particleIterT>
  const ftype met_rel( particleIterT begin , particleIterT end , const ftype& crit_dphi ) const {
    // compute relative MET, defined as
    //        MET, the smallest dphi with any particle from the range (begin,end) is larger than crit_dphi
    //        MET*sin(dphi), if the smallest dphi is less than or eq to crit_dphi
    assert( begin!=end );
    AnaParticle::ftype min_dphi = std::numeric_limits<AnaParticle::ftype>::max();
    for( particleIterT i=begin; i!=end; ++i ) {
      const AnaParticle::ftype this_min_dphi = this->dist_phi( *i );
      if( std::abs(this_min_dphi) < std::abs(min_dphi) ) {
        min_dphi = this_min_dphi;
      }
    }
    if( std::abs(min_dphi) > crit_dphi ) { return met(); }
    return std::abs( met() * std::sin( min_dphi ) );
  }



  std::ostream& print( std::ostream& os = std::cout ) const {
    os << boost::format("AnaMET: pt=%|+10|(GeV) phi=%|10|(rad) is_good=%|10|") % met() % metphi() % is_good() << std::endl;
    return os;
  }

  template< typename iteratorT >
  static const AnaMET* met_of_type( const MetType& type , iteratorT begin , iteratorT end ) {
    for( iteratorT i=begin; i!=end; ++i ) {
      const AnaMET* met( *i );
      if( met->met_type()==type ) { return met; }
    }
    return 0;
  }

  const bool is_corrected() const { return _uncorrected_parent; }
  const boost::shared_ptr<const AnaMET> uncorrected_parent() const { return _uncorrected_parent; }

  // MET setup, useful for representation of truth level MET
  // eta+m should only be set for AnaTruthParticle objects
  void setup_met_for_truth( const ftype& met , const ftype& phi , ftype eta=0 , ftype m=0 ) {
    _met_type = TRUTH;
    _is_good = true;
    _momentum.SetPtEtaPhiM( met , eta , phi , m );
    _MET_cal_etx = _momentum.Px();
    _MET_cal_ety = _momentum.Py();
    _MET_RefMuonTrack_etx = 0.;
    _MET_RefMuonTrack_ety = 0.;
    _MET_MuonBoy_etx = 0.;
    _MET_MuonBoy_ety = 0.;
    _MET_cal_etx_CentralReg = 0.;
    _MET_cal_ety_CentralReg = 0.;
    _MET_cal_etx_EndcapRegion = 0.;
    _MET_cal_ety_EndcapRegion = 0.;
    _MET_cal_etx_ForwardReg = 0.;
    _MET_cal_ety_ForwardReg = 0.;
  }

  // MET recalculation 
  // ================================================================

  // we want to 1) do this with a templated iterator over the muon
  // collection and 2) avoid including the muon header in this
  // header. therefore, the muon code must go into a separate function
  // in AnaMET.cpp, and we define some helper functions to factor most
  // of the code into that source file.

private:

  // helper functions
  static std::pair<ftype,ftype> _base_met_xy_cef( const boost::shared_ptr<const AnaMET>& met );
  static void _accumulate_muon_delta_met_xy( const boost::shared_ptr<const AnaMuon>& mu , std::pair<ftype,ftype>& delta_met_xy );
  static boost::shared_ptr<const AnaMET> _make_recomputed_met_hww( const boost::shared_ptr<const AnaMET>& met , 
                                                                   const std::pair<ftype,ftype>& met_xy ,
                                                                   const std::pair<ftype,ftype>& muon_delta_met_xy );
  
public:

  // public interface
  template< typename muonIterT >
  static
  const boost::shared_ptr<const AnaMET> 
  recompute_for_hww( const boost::shared_ptr<const AnaMET>& met , muonIterT begin_muons , muonIterT end_muons )
  {
    // correct met for muons (and in the future other objects) according to the
    // H->WW instructions at https://twiki.cern.ch/twiki/bin/view/AtlasProtected/BaslinecutsforWWCONF.
    std::pair<ftype,ftype> met_xy = _base_met_xy_cef( met );
    std::pair<ftype,ftype> muon_delta_met_xy = std::pair<ftype,ftype>( 0. , 0. );
    for( muonIterT iMu=begin_muons , fMu=end_muons; iMu!=fMu; ++iMu ) { 
      _accumulate_muon_delta_met_xy( *iMu , muon_delta_met_xy );
    }
    return _make_recomputed_met_hww( met , met_xy , muon_delta_met_xy);
  }

  static boost::shared_ptr<const AnaMET> remove_four_vector( const boost::shared_ptr<const AnaMET>& met , const TLorentzVector& vec );
  static boost::shared_ptr<const AnaMET> add_four_vector( const boost::shared_ptr<const AnaMET>& met , const TLorentzVector& vec );
  static boost::shared_ptr<const AnaMET> met_corrected( const boost::shared_ptr<const AnaMET>& met , const float& et , const float& phi );


  template< typename METUTILSYS , typename METUTILTERM , typename METUTILCLASS , typename elIterT , typename muIterT , typename jetIterT >
  static
  const boost::shared_ptr<const AnaMET>
  jetetmiss_met( const boost::shared_ptr<const AnaMET>& met , METUTILCLASS *metutil ,
		 const elIterT& el_begin , const elIterT& el_end , const bool& reset_el ,
		 const muIterT& mu_begin , const muIterT& mu_end , const bool& reset_mu ,
		 const jetIterT& jet_begin , const jetIterT& jet_end , const bool& reset_jet ,
		 const METUTILTERM& term , const METUTILSYS& sys ) 
  {
    static float MeV = 1000.;

    std::vector< float > vel_pt , vel_eta , vel_phi , vel_e , vel_wet , vel_wpx , vel_wpy;
    std::vector< float > vmu_pt , vmu_eta , vmu_phi , vmu_mspt , vmu_mstheta , vmu_msphi;
    std::vector< std::vector<float> > vmu_wet , vmu_wpx , vmu_wpy;
    std::vector< float > vjet_pt , vjet_eta , vjet_phi , vjet_e , vjet_wet , vjet_wpx , vjet_wpy , vjet_gcwjes , vjet_m;
    std::vector< float > vorijet_pt;
    std::vector< unsigned short > vel_status , vjet_status;
    std::vector< std::vector<unsigned int> > vmu_status;
    
    if( reset_el ) {
      for( elIterT i = el_begin , f = el_end ; i != f ; ++i ) {
	vel_pt.push_back( (*i)->pt()*MeV );
	vel_eta.push_back( (*i)->eta() );
	vel_phi.push_back( (*i)->phi() );
	vel_e.push_back( (*i)->energy()*MeV );
	vel_wet.push_back( (*i)->met_wet() );
	vel_wpx.push_back( (*i)->met_wpx() );
	vel_wpy.push_back( (*i)->met_wpy() );
	vel_status.push_back( short((*i)->met_status()) );    
      }
    }
    if( reset_mu ) {
      for( muIterT i = mu_begin , f = mu_end ; i != f ; ++i ) {
	vmu_pt.push_back( (*i)->pt()*MeV );
	vmu_eta.push_back( (*i)->eta() );
	vmu_phi.push_back( (*i)->phi() );
	vmu_wet.push_back( (*i)->met_wet_vec() );
	vmu_wpx.push_back( (*i)->met_wpx_vec() );
	vmu_wpy.push_back( (*i)->met_wpy_vec() );
	vmu_mspt.push_back( (*i)->ms_muon().Pt()*MeV );
	vmu_mstheta.push_back( (*i)->ms_muon().Theta() );
	vmu_msphi.push_back( (*i)->ms_muon().Phi() );
	vmu_status.push_back( (*i)->met_status_vec() );
      }
    }
    if( reset_jet ) {
      for( jetIterT i = jet_begin , f = jet_end ; i != f ; ++i ) {
	vjet_pt.push_back( (*i)->pt()*MeV );
	vjet_eta.push_back( (*i)->eta() );
	vjet_phi.push_back( (*i)->phi() );
	vjet_e.push_back( (*i)->energy()*MeV );
	vjet_wet.push_back( (*i)->met_wet() );
	vjet_wpx.push_back( (*i)->met_wpx() );
	vjet_wpy.push_back( (*i)->met_wpy() );
	vjet_gcwjes.push_back( (*i)->gcwjes() );
	vjet_m.push_back( (*i)->m()*MeV );
	vjet_status.push_back( short((*i)->met_status()) );
	vorijet_pt.push_back( (*i)->highest_parent_pt()*MeV );
      }
    }
    
    // Only set the passed in variables unless newcall is set to TRUE.
    // This could save time when recalculating MET for different systematics.
    if( reset_el ) 
      metutil->setElectronParameters( &vel_pt , &vel_eta , &vel_phi , &vel_wet , &vel_wpx , &vel_wpy , &vel_status );
    if( reset_mu ) {
      metutil->setMuonParameters( &vmu_pt , &vmu_eta , &vmu_phi , &vmu_wet , &vmu_wpx , &vmu_wpy , &vmu_status );
      metutil->setExtraMuonParameters( &vmu_mspt , &vmu_mstheta , &vmu_msphi );
    }
    if( reset_jet ) {
      metutil->setJetParameters( &vjet_pt , &vjet_eta , &vjet_phi , &vjet_e , &vjet_wet , &vjet_wpx , &vjet_wpy , &vjet_status );
      metutil->setOriJetParameters( &vorijet_pt );
    }
    
    boost::shared_ptr<AnaMET> newMET(boost::make_shared<AnaMET>(*met) );
    newMET->_momentum.SetPtEtaPhiM( metutil->getMissingET( term , sys ).et()/MeV , 0. , 
				    metutil->getMissingET( term , sys ).phi() , 0. );
    newMET->_uncorrected_parent = met;

    //    std::cout << "DEBUG :: newMET = " << newMET->pt() << " = " << metutil->getMissingET("RefFinal").et()/MeV << std::endl
    //	      << "         oldMET = " << met->pt() << std::endl;

    return boost::const_pointer_cast<const AnaMET>( newMET );
  }

};

BOOST_CLASS_VERSION( AnaMET , 4 );

#endif // WRAP_ANAMET_HPP
