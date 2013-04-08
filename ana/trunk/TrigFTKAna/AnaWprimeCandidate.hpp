
#ifndef ANAWPRIMECANDIDATE_HPP
#define ANAWPRIMECANDIDATE_HPP

// A wrapper class around AnaW that contains additional event-wide information
// relevant for x-section and ratio analyses (e.g., jets)

#include <cassert>
#include <vector>
#include <iterator>
#include <numeric>
#include <algorithm>
#include <boost/shared_ptr.hpp>
#include <boost/foreach.hpp>
#include <boost/bind.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/version.hpp>
#include <boost/math/special_functions/fpclassify.hpp>
#include <TLorentzVector.h>
#include "TrigFTKAna/AnaHiggs.hpp"
#include "TrigFTKAna/AnaW.hpp"
#include "TrigFTKAna/AnaElectron.hpp"
#include "TrigFTKAna/AnaPhoton.hpp"
#include "TrigFTKAna/AnaMuon.hpp"
#include "TrigFTKAna/AnaJet.hpp"
#include "TrigFTKAna/AnaVertex.hpp"
#include "TrigFTKAna/AnaMET.hpp"
#include "TrigFTKAna/AnaParticle.hpp"
#include "TrigFTKAna/AnaComposite.hpp"
#include "TrigFTKAna/CompositeGenerator.hpp"

class
AnaWprimeCandidate : public AnaW
{
public:
  typedef AnaParticle::ftype ftype;
  typedef std::vector< boost::shared_ptr<const AnaJet> > JetColl;
  typedef JetColl::const_iterator jet_const_iterator;
private:
  std::vector< boost::shared_ptr<const AnaParticle> > _event_leptons; // pt-ordered
  std::vector< boost::shared_ptr<const AnaJet> > _event_jets; // pt-ordered
  std::vector< boost::shared_ptr<const AnaJet> > _event_bjets;
  std::vector< boost::shared_ptr<const AnaMET> > _event_met;
  std::vector< boost::shared_ptr<const AnaVertex> > _event_vertices;
  mutable bool _cached;
private:
  friend class boost::serialization::access;
  template<class archiveT>
  void serialize( archiveT& ar, const unsigned int version ) {
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP( AnaW );
    ar & boost::serialization::make_nvp("event_leptons",_event_leptons);
    ar & boost::serialization::make_nvp("event_jets",_event_jets);
    ar & boost::serialization::make_nvp("event_bjets",_event_bjets);
    ar & boost::serialization::make_nvp("event_met",_event_met);
    ar & boost::serialization::make_nvp("event_vertices",_event_vertices);
  }
  void _refresh_cache() const {
    if( _cached ) { return; }
    // nothing done here yet
    _cached = true;
  }
public:

  AnaWprimeCandidate()
    : _event_leptons()
    , _event_jets()
    , _event_bjets()
    , _event_met()
    , _event_vertices()
    , AnaW()
  {}
  AnaWprimeCandidate(const boost::shared_ptr<const AnaW>& w)
    : _event_leptons()
    , _event_jets()
    , _event_bjets()
    , _event_met()
    , _event_vertices()
    , _cached(false)
    , AnaW(*w)
  {
    _refresh_cache();
  }

  virtual ~AnaWprimeCandidate() {}

  // leptons
  const unsigned int nleptons() const { return _event_leptons.size(); }
  const bool exactly_one_lepton() const { return _event_leptons.size()==1; }
  const bool atleast_one_lepton() const { return _event_leptons.size()>=1; }
  const short scharge() const {
    const boost::shared_ptr<const AnaParticle> lp = lepton();
    return lp ? lp->scharge() : 0;
  }
  const AnaParticle::ftype lepton_eta() const {
    const boost::shared_ptr<const AnaParticle> lp = lepton();
    return lp ? lp->eta() : std::numeric_limits<AnaParticle::ftype>::max();
  }
  const AnaParticle::ftype lepton_phi() const {
    const boost::shared_ptr<const AnaParticle> lp = lepton();
    return lp ? lp->phi() : std::numeric_limits<AnaParticle::ftype>::max();
  }
  const AnaParticle::ftype dphi_lepton_met() const {
    return detector::delta_phi( lepton_phi() , met_phi() );
  }
  const AnaParticle::ftype dphi_lepton_leading_jet() const {
    if( njets()<1 ) return std::numeric_limits<AnaParticle::ftype>::max();
    return lepton()->dist_phi( leading_jet() );
  }
  const AnaParticle::ftype dphi_met_leading_jet() const {
    if( njets()<1 ) return std::numeric_limits<AnaParticle::ftype>::max();
    return met()->dist_phi( leading_jet() );
  }
  const AnaParticle::ftype deta_leading_jets() const { return njets()>1 ? detector::delta_eta(leading_jet()->eta(),subleading_jet()->eta()) : 0.; }
  const AnaParticle::ftype max_deta_jets() const {
    if( njets()<=1 ) return 0.;
    if( njets()<=2 ) return detector::delta_eta( leading_jet()->eta() , subleading_jet()->eta() );
    if( detector::delta_eta( leading_jet()->eta() , subleading_jet()->eta() ) < detector::delta_eta( leading_jet()->eta() , subsubleading_jet()->eta() ) ) {
      return  detector::delta_eta( leading_jet()->eta() , subsubleading_jet()->eta() );
    } 
    return detector::delta_eta( leading_jet()->eta() , subleading_jet()->eta() );
  }
  const AnaParticle::ftype true_max_deta_jets() const {
    if( njets()<=1 ) return 0.;
    AnaParticle::ftype max_deta = 0.;
    BOOST_FOREACH( boost::shared_ptr<const AnaJet> j , _event_jets ) { 
      AnaParticle::ftype this_deta = detector::delta_eta( leading_jet()->eta() , j->eta() );
      if( this_deta > max_deta ) max_deta = this_deta;
    }
    return max_deta;
  }
  const AnaParticle::ftype deta_lepton_leading_jet() const {
    if( njets()<=1 || !lepton() ) return 0.;
    return lepton()->dist_eta( leading_jet() );
  }
  const bool lepton_is_muon() const {
    const boost::shared_ptr<const AnaMuon> muon = boost::dynamic_pointer_cast<const AnaMuon,const AnaParticle>( lepton() );
    return( muon ? true : false );
  }
  const boost::shared_ptr<const AnaMuon>& muon() const {
    static boost::shared_ptr<const AnaMuon> tmp;
    const boost::shared_ptr<const AnaMuon> muon = boost::dynamic_pointer_cast<const AnaMuon,const AnaParticle>( lepton() );
    return ( muon ? muon : tmp);
  }
  const bool lepton_is_electron() const {
    const boost::shared_ptr<const AnaElectron> electron = boost::dynamic_pointer_cast<const AnaElectron,const AnaParticle>( lepton() );
    return( electron ? true : false );
  }
  const boost::shared_ptr<const AnaElectron>& electron() const {
    static boost::shared_ptr<const AnaElectron> tmp;
    const boost::shared_ptr<const AnaElectron> electron = boost::dynamic_pointer_cast<const AnaElectron,const AnaParticle>( lepton() ); // FIXME this line leadso to SIGABT
    return ( electron ? electron : tmp);
  }
  const bool electron_is_aod_tight() const {
    if( !lepton_is_electron() ) return true;
    const boost::shared_ptr<const AnaElectron> electron = boost::dynamic_pointer_cast<const AnaElectron,const AnaParticle>( lepton() );
    return electron->is_aod_tight();
  }
  const bool muon_eta_cut() const {
    if( !lepton_is_muon() ) return true;
    const boost::shared_ptr<const AnaMuon> muon = boost::dynamic_pointer_cast<const AnaMuon,const AnaParticle>( lepton() );
    return std::abs(muon->eta())<2.4;
  }
  // MET
  const unsigned int nmets() const { return _event_met.size(); }
  const bool exactly_one_met() const { return _event_met.size()==1; }
  const AnaParticle::ftype met_phi() const {
    const boost::shared_ptr<const AnaParticle> mp = met();
    return mp ? mp->phi() : std::numeric_limits<AnaParticle::ftype>::max();
  }
  const AnaParticle::ftype met_reconstructed_pz_low() const {
    const boost::shared_ptr<const AnaParticle> mp = met();
    const boost::shared_ptr<const AnaParticle> lp = lepton();
    return detector::reconstructedMETpz( lp->px() , lp->py() , lp->pz() , mp->px() , mp->py() , 0 );
  }
  const AnaParticle::ftype met_reconstructed_pz_high() const {
    const boost::shared_ptr<const AnaParticle> mp = met();
    const boost::shared_ptr<const AnaParticle> lp = lepton();
    return detector::reconstructedMETpz( lp->px() , lp->py() , lp->pz() , mp->px() , mp->py() , 1 );
  }
  const AnaParticle::ftype met_pz_low_deta_W_leadingjet() const {
    AnaParticle::ftype pz1 = met_reconstructed_pz_low();
    AnaParticle::ftype pz2 = met_reconstructed_pz_high();
    if( pz1 == pz2 ) return pz1;
    // make W four-vectors
    TLorentzVector W1( 0 , 0 , 0 , 0 );
    W1 += lepton()->four_vector();
    W1 += met_reconstructed_four_vector( pz1 );
    TLorentzVector W2( 0 , 0 , 0 , 0 );
    W2 += lepton()->four_vector();
    W2 += met_reconstructed_four_vector( pz2 );
    // compare eta
    return std::abs( W1.Eta() - leading_jet()->eta() ) <= std::abs( W2.Eta() - leading_jet()->eta() ) ? pz1 : pz2;    
  }
  const AnaParticle::ftype met_preferred_pz() const { 
    if ( lepton() && leading_jet() ) return met_pz_low_deta_W_leadingjet();
    else return met_reconstructed_pz_high();
  }
  const AnaParticle::ftype met_eta( const AnaParticle::ftype pz ) const {
    const boost::shared_ptr<const AnaParticle> mp = met();
    return detector::etaFromPxPyPz( mp->px() , mp->py() , pz );
  }
  TLorentzVector met_reconstructed_four_vector( const AnaParticle::ftype pz ) const {
    TLorentzVector q(0,0,0,0);
    q.SetPtEtaPhiM( met()->pt() , met_eta(pz) , met_phi() , 0. );
    return q;
  }

  // jets and vertices
  const unsigned int nvtxs() const { return _event_vertices.size(); }
  const unsigned int njets() const { return _event_jets.size(); }
  const unsigned int nbjets() const { return _event_bjets.size(); }
  jet_const_iterator begin_jets() const { return _event_jets.begin(); }
  jet_const_iterator end_jets() const { return _event_jets.end(); }
  jet_const_iterator begin_bjets() const { return _event_bjets.begin(); }
  jet_const_iterator end_bjets() const { return _event_bjets.end(); }
  const boost::shared_ptr<const AnaJet> leading_jet() const {
    static boost::shared_ptr<const AnaJet> null;
    if( njets()<1 ) return null;
    std::vector< boost::shared_ptr<const AnaJet> > jets_by_pt = _event_jets;
    std::sort( jets_by_pt.begin() , jets_by_pt.end() , bind(&AnaJet::pt,_1) > bind(&AnaJet::pt,_2) );
    return *( jets_by_pt.begin() );
  }
  const boost::shared_ptr<const AnaJet> subleading_jet() const {
    static boost::shared_ptr<const AnaJet> null;
    if( njets()<=1 ) return null;
    std::vector< boost::shared_ptr<const AnaJet> > jets_by_pt = _event_jets;
    std::sort( jets_by_pt.begin() , jets_by_pt.end() , bind(&AnaJet::pt,_1) > bind(&AnaJet::pt,_2) );
    return *( jets_by_pt.begin() + 1 );
  }
  const boost::shared_ptr<const AnaJet> subsubleading_jet() const {
    static boost::shared_ptr<const AnaJet> null;
    if( njets()<=2 ) return null;
    std::vector< boost::shared_ptr<const AnaJet> > jets_by_pt = _event_jets;
    std::sort( jets_by_pt.begin() , jets_by_pt.end() , bind(&AnaJet::pt,_1) > bind(&AnaJet::pt,_2) );
    return *( jets_by_pt.begin() + 2 );
  }

  // functions for comparing to Constantinos' cutflow
  const bool no_btag_event_jet() const {
    return( !(std::find_if( _event_jets.begin() , _event_jets.end() , boost::bind( &AnaJet::tag_sv0_w , _1 ) >= 5.85 ) != _event_jets.end() ) );
  }
  const bool low_dijet_eta() const {
    return( njets()>1 ? leading_jet()->eta() < 1.8 && subleading_jet()->eta() < 1.8 : false );
  }
  const bool mid_dijet_eta() const {
    return( njets()>1 ? leading_jet()->eta() < 2.5 && subleading_jet()->eta() < 2.5 : false );
  }
  // Ht
  const ftype ht() const {
    float q = 0.0;
    q += met()->pt();
    q += lepton()->pt();
    q += leading_jet()->pt();
    q += subleading_jet()->pt();
    return q;
  }
  // St
  const ftype st() const {
    float q = 0.0;
    float D = 0.0;
    float px_met = (met()->pt())*cos(met()->phi());
    float px_lepton = (lepton()->pt())*cos(lepton()->phi());
    float px_jet1 = (leading_jet()->pt())*cos(leading_jet()->phi());
    float px_jet2 = (subleading_jet()->pt())*cos(subleading_jet()->phi());
    float py_met = (met()->pt())*sin(met()->phi());
    float py_lepton = (lepton()->pt())*sin(lepton()->phi());
    float py_jet1 = (leading_jet()->pt())*sin(leading_jet()->phi());
    float py_jet2 = (subleading_jet()->pt())*sin(subleading_jet()->phi());
    float qx = 0.0;
    float qy = 0.0;
    float qxy = 0.0;
    q += (met()->pt())*(met()->pt());
    q += (lepton()->pt())*(lepton()->pt());
    q += (leading_jet()->pt())*(leading_jet()->pt());
    q += (subleading_jet()->pt())*(subleading_jet()->pt());
    qx += px_met*px_met;
    qx += px_lepton*px_lepton;
    qx += px_jet1*px_jet1;
    qx += px_jet2*px_jet2;
    qy += py_met*py_met;
    qy += py_lepton*py_lepton;
    qy += py_jet1*py_jet1;
    qy += py_jet2*py_jet2;
    qxy += px_met*py_met;
    qxy += px_lepton*py_lepton;
    qxy += px_jet1*py_jet1;
    qxy += px_jet2*py_jet2;
    D = sqrt(q*q - 4*qx*qy + 4*qxy*qxy);
    float q1 = (1-(D/q));
    return q1;
  }

  // mass plots for W' studies
  const ftype jets_m() const {
    TLorentzVector q( 0 , 0 , 0 , 0 );
    BOOST_FOREACH( boost::shared_ptr<const AnaJet> j , _event_jets ) { q += j->four_vector(); }
    return q.M();
  }
  const ftype dijet_m() const {
    if( njets()>1 ) {
      TLorentzVector q( 0 , 0 , 0 , 0 );
      q += leading_jet()->four_vector();
      q += subleading_jet()->four_vector();
      return boost::math::isnan( q.M() ) ? -1. : q.M();
    } else return 0.;
  }
  const ftype event_m() const {
    TLorentzVector q( 0 , 0 , 0 , 0 );
    q += lepton()->four_vector();
    q += met_reconstructed_four_vector( met_preferred_pz() );
    BOOST_FOREACH( boost::shared_ptr<const AnaJet> j , _event_jets ) { q += j->four_vector(); }
    return boost::math::isnan( q.M() ) ? -1. : q.M();
  }
  const ftype event_mt() const {
    TLorentzVector q( 0 , 0 , 0 , 0 );
    q += lepton()->transverse_four_vector();
    q += met()->transverse_four_vector();
    BOOST_FOREACH( boost::shared_ptr<const AnaJet> j , _event_jets ) { q+= j->transverse_four_vector(); }
    return boost::math::isnan( q.M() ) ? -1. : q.M();
  }
  const ftype w_m() const {
    TLorentzVector q( 0 , 0 , 0 , 0 );
    q += lepton()->four_vector();
    q += met_reconstructed_four_vector( met_preferred_pz() );
    return boost::math::isnan( q.M() ) ? -1. : q.M();
  }
  const ftype w_eta() const {
    TLorentzVector q( 0 , 0 , 0 , 0 );
    q += lepton()->four_vector();
    q += met_reconstructed_four_vector( met_preferred_pz() );
    return boost::math::isnan( q.Eta() ) ? -999. : q.Eta();
  }
  const ftype w_phi() const {
    TLorentzVector q( 0 , 0 , 0 , 0 );
    q += lepton()->four_vector();
    q += met_reconstructed_four_vector( met_preferred_pz() );
    return boost::math::isnan( q.Phi() ) ? -999. : q.Phi();
  }
  const ftype w_mt() const {
    TLorentzVector q( 0 , 0 , 0 , 0 );
    q += lepton()->transverse_four_vector();
    q += met()->transverse_four_vector();
    return boost::math::isnan( q.M() ) ? -1. : q.M();
  }
  const ftype wjj_mt() const {
    if( njets()>1 ) {
      TLorentzVector q( 0 , 0 , 0 , 0 );
      q += met()->transverse_four_vector();
      q += lepton()->transverse_four_vector();
      q += leading_jet()->transverse_four_vector();
      q += subleading_jet()->transverse_four_vector();
      return boost::math::isnan( q.M() ) ? -1. : q.M();
    } else return 0.;
  }
  const ftype wjj_m() const {
    if( njets()>1 ) {
      TLorentzVector q( 0 , 0 , 0 , 0 );
      q += lepton()->four_vector();
      q += met_reconstructed_four_vector( met_preferred_pz() );
      q += leading_jet()->four_vector();
      q += subleading_jet()->four_vector();
      return boost::math::isnan( q.M() ) ? -1. : q.M();
    } return 0.;
  }
  const ftype top_mt() const {
    if( njets()>0 ) {
      TLorentzVector q( 0 , 0 , 0 , 0 );
      q += lepton()->transverse_four_vector();
      q += met()->transverse_four_vector();
      q += leading_jet()->transverse_four_vector();
      return boost::math::isnan( q.M() ) ? -1. : q.M();
    } else return 0.;
  }
  const ftype top_m() const {
    if( njets()>0 ) {
      TLorentzVector q( 0 , 0 , 0 , 0 );
      q += lepton()->four_vector();
      q += met_reconstructed_four_vector( met_preferred_pz() );
      q += leading_jet()->four_vector();
      return boost::math::isnan( q.M() ) ? -1 : q.M();
    } else return 0.;
  }
  const ftype wprime_mt() const {
    if( njets()>1 ) {
      TLorentzVector q( 0 , 0 , 0 , 0 );
      q += met()->transverse_four_vector();
      q += lepton()->transverse_four_vector();
      q += leading_jet()->transverse_four_vector();
      q += subleading_jet()->transverse_four_vector();
      return boost::math::isnan( q.M() ) ? -1. : q.M();
    } else return 0.;
  }
  const ftype wprime_m() const {
    if( njets()>1 ) {
      TLorentzVector q( 0 , 0 , 0 , 0 );
      q += lepton()->four_vector();
      q += met_reconstructed_four_vector( met_preferred_pz() );
      q += leading_jet()->four_vector();
      q += subleading_jet()->four_vector();
      return boost::math::isnan( q.M() ) ? -1. : q.M();
    } return 0.;
  }

  // JORDAN (1/22/2010) some useful definitions for control/signal regions
  const bool wmt() const { return mt()>30.; }
  const bool baseline() const { return met() ? mt()>30. && met()->pt()>30. : false; }
  const bool metfit_CR() const { return met() ? met()->pt()>10. : false; }
  const bool lowmet_CR() const { return met() ? met()->pt()<30. : false; }
  const bool lowmet_wmt_CR() const { return lowmet_CR() && mt()>30.; }
  const bool midmet_CR() const { return metfit_CR() && lowmet_CR(); }
  const bool midmet_wmt_CR() const { return midmet_CR() && mt()>30.; }
  const bool midmet_20_wmt_CR() const { return midmet_wmt_CR() && met()->pt()>20.; }
  const bool midmet_25_wmt_CR() const { return baseline() && lowmet_CR(); }
  const bool tag_and_probe_CR() const {
    if( njets()<=1 ) return false;
    BOOST_FOREACH( boost::shared_ptr<const AnaJet> test_jet , _event_jets ) {
      if( std::abs(test_jet->eta())>=2 )   continue;
      if( test_jet->isBadJet_tight_v17() ) continue;
      BOOST_FOREACH( boost::shared_ptr<const AnaJet> jt , _event_jets ) {
        // this loop includes test jet, but it doesn't matter since it will fail the dphi cut
        if( 2.*std::abs(jt->pt()-test_jet->pt())/(jt->pt()+test_jet->pt()) >= 0.4 ) continue;
        if( std::abs( detector::delta_phi(jt->phi(),test_jet->phi()) ) <= 2.6 )     continue;
        return true;
      }
    }
    return false;
  }
  
  // old interface - used to build w candidates directly from components
  // it constructs a W from the first lepton in the lepton collection
  template<typename iteratorLepT,typename iteratorMetT,typename iteratorJetT,typename iteratorVtxT>
  static const boost::shared_ptr<AnaWprimeCandidate> build( iteratorLepT begin_leptons , iteratorLepT end_leptons ,
							    iteratorMetT begin_mets , iteratorMetT end_mets ,
							    iteratorJetT begin_jets , iteratorJetT end_jets ,
							    iteratorJetT begin_bjets , iteratorJetT end_bjets ,
							    iteratorVtxT begin_vtxs , iteratorVtxT end_vtxs ) {
    if( begin_leptons==end_leptons ) { return boost::shared_ptr<AnaWprimeCandidate>(); }
    if( begin_mets==end_mets ) { return boost::shared_ptr<AnaWprimeCandidate>(); }
    assert( std::distance( begin_mets , end_mets ) >= 1 );
    assert( std::distance( begin_leptons , end_leptons ) >=1 );
    boost::shared_ptr<AnaWprimeCandidate> result(new AnaWprimeCandidate(boost::shared_ptr<AnaW>(new AnaW( *begin_leptons , *begin_mets ))));
    assert( result );
    result->_event_leptons.clear();
    for( iteratorLepT i=begin_leptons; i!=end_leptons; ++i ) {
      result->_event_leptons.push_back( *i );
    }
    assert( result->_event_leptons.size() == std::distance( begin_leptons , end_leptons ) );

    result->_event_met.clear();
    for( iteratorMetT i=begin_mets; i!=end_mets; ++i ) {
      result->_event_met.push_back( *i );
    }
    assert( result->_event_met.size() == std::distance( begin_mets , end_mets ) );
    result->_event_jets.assign( begin_jets , end_jets );
    result->_event_bjets.assign( begin_bjets , end_bjets );
    result->_event_vertices.assign( begin_vtxs , end_vtxs );
    assert( !(result->_event_leptons.empty()) );
    assert( !(result->_event_met.empty()) );
    return result;
  }

  // 0's cuts
  const bool baseline_jets() const { return baseline() && njets()>1; }
  const bool baseline_2jets() const { return baseline() && njets()==2; }
  const bool baseline_3jets() const { return baseline() && njets()==3;}
  const bool baseline_4jets() const { return baseline() && njets()==4;}
  const bool baseline_5jets() const { return baseline() && njets()==5;}
  const bool baseline_6upjets() const { return baseline() && njets()>=6;}
  const bool baseline_2to3jets() const { return baseline() && njets()>1 && njets()<4;}
  const bool baseline_2to4jets() const { return baseline() && njets()>1 && njets()<5;}
  const bool baseline_btag() const { return baseline() && nbjets()>0; }
  const bool baseline_btag_jets() const { return baseline() && njets()>1 && nbjets()>0; }
  const bool baseline_btag_2jets() const { return baseline() && njets()==2 && nbjets()>0; }
  const bool baseline_btag_3jets() const { return baseline() && njets()==3 && nbjets()>0; }
  const bool baseline_btag_4jets() const { return baseline() && njets()==4 && nbjets()>0; }
  const bool baseline_btag_5jets() const { return baseline() && njets()==5 && nbjets()>0; }
  const bool baseline_btag_6upjets() const { return baseline() && njets()>=6 && nbjets()>0; }
  const bool baseline_btag_2to3jets() const { return baseline() && njets()>1 && njets()<4 && nbjets()>0; }
  const bool baseline_btag_2to4jets() const { return baseline() && njets()>1 && njets()<5 && nbjets()>0; }

  const bool baseline_btag_jets_st1() const { return baseline() && njets()>1 && nbjets()>0 && st()<0.1; }
  const bool baseline_btag_jets_st2() const { return baseline() && njets()>1 && nbjets()>0 && st()<0.2; }
  const bool baseline_btag_jets_st3() const { return baseline() && njets()>1 && nbjets()>0 && st()<0.4; }
  const bool baseline_btag_2jets_st1() const { return baseline() && njets()==2 && nbjets()>0 && st()<0.1; }
  const bool baseline_btag_2jets_st2() const { return baseline() && njets()==2 && nbjets()>0 && st()<0.2; }
  const bool baseline_btag_2jets_st3() const { return baseline() && njets()==2 && nbjets()>0 && st()<0.4; }
  const bool baseline_btag_3jets_st1() const { return baseline() && njets()==3 && nbjets()>0 && st()<0.1; }
  const bool baseline_btag_3jets_st2() const { return baseline() && njets()==3 && nbjets()>0 && st()<0.2; }
  const bool baseline_btag_3jets_st3() const { return baseline() && njets()==3 && nbjets()>0 && st()<0.4; }
  const bool baseline_btag_2to3jets_st1() const { return baseline() && njets()>1 && njets()<4 && nbjets()>0 && st()<0.1; }
  const bool baseline_btag_2to3jets_st2() const { return baseline() && njets()>1 && njets()<4 && nbjets()>0 && st()<0.2; }
  const bool baseline_btag_2to3jets_st3() const { return baseline() && njets()>1 && njets()<4 && nbjets()>0 && st()<0.4; }
  const bool baseline_btag_2to4jets_st1() const { return baseline() && njets()>1 && njets()<5 && nbjets()>0 && st()<0.1; }
  const bool baseline_btag_2to4jets_st2() const { return baseline() && njets()>1 && njets()<5 && nbjets()>0 && st()<0.2; }
  const bool baseline_btag_2to4jets_st3() const { return baseline() && njets()>1 && njets()<5 && nbjets()>0 && st()<0.4; }
  const bool baseline_btag_jets_dphi1() const { return baseline() && njets()>1 && nbjets()>0 && fabs(dphi_lepton_met())<1; }
  const bool baseline_btag_jets_dphi2() const { return baseline() && njets()>1 && nbjets()>0 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_btag_jets_dphi3() const { return baseline() && njets()>1 && nbjets()>0 && fabs(dphi_lepton_met())<2; }
  const bool baseline_btag_2jets_dphi1() const { return baseline() && njets()==2 && nbjets()>0 && fabs(dphi_lepton_met())<1; }
  const bool baseline_btag_2jets_dphi2() const { return baseline() && njets()==2 && nbjets()>0 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_btag_2jets_dphi3() const { return baseline() && njets()==2 && nbjets()>0 && fabs(dphi_lepton_met())<2; }
  const bool baseline_btag_3jets_dphi1() const { return baseline() && njets()==3 && nbjets()>0 && fabs(dphi_lepton_met())<1; }
  const bool baseline_btag_3jets_dphi2() const { return baseline() && njets()==3 && nbjets()>0 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_btag_3jets_dphi3() const { return baseline() && njets()==3 && nbjets()>0 && fabs(dphi_lepton_met())<2; }
  const bool baseline_btag_2to3jets_dphi1() const { return baseline() && njets()>1 && njets()<4 && nbjets()>0 && fabs(dphi_lepton_met())<1; }
  const bool baseline_btag_2to3jets_dphi2() const { return baseline() && njets()>1 && njets()<4 && nbjets()>0 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_btag_2to3jets_dphi3() const { return baseline() && njets()>1 && njets()<4 && nbjets()>0 && fabs(dphi_lepton_met())<2; }
  const bool baseline_btag_2to4jets_dphi1() const { return baseline() && njets()>1 && njets()<5 && nbjets()>0 && fabs(dphi_lepton_met())<1; }
  const bool baseline_btag_2to4jets_dphi2() const { return baseline() && njets()>1 && njets()<5 && nbjets()>0 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_btag_2to4jets_dphi3() const { return baseline() && njets()>1 && njets()<5 && nbjets()>0 && fabs(dphi_lepton_met())<2; }
  const bool baseline_btag_jets_st1_dphi1() const { return baseline() && njets()>1 && nbjets()>0 && st()<0.1 && fabs(dphi_lepton_met())<1; }
  const bool baseline_btag_jets_st1_dphi2() const { return baseline() && njets()>1 && nbjets()>0 && st()<0.1 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_btag_jets_st1_dphi3() const { return baseline() && njets()>1 && nbjets()>0 && st()<0.1 && fabs(dphi_lepton_met())<2; }
  const bool baseline_btag_jets_st2_dphi1() const { return baseline() && njets()>1 && nbjets()>0 && st()<0.2 && fabs(dphi_lepton_met())<1; }
  const bool baseline_btag_jets_st2_dphi2() const { return baseline() && njets()>1 && nbjets()>0 && st()<0.2 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_btag_jets_st2_dphi3() const { return baseline() && njets()>1 && nbjets()>0 && st()<0.2 && fabs(dphi_lepton_met())<2; }
  const bool baseline_btag_jets_st3_dphi1() const { return baseline() && njets()>1 && nbjets()>0 && st()<0.4 && fabs(dphi_lepton_met())<1; }
  const bool baseline_btag_jets_st3_dphi2() const { return baseline() && njets()>1 && nbjets()>0 && st()<0.4 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_btag_jets_st3_dphi3() const { return baseline() && njets()>1 && nbjets()>0 && st()<0.4 && fabs(dphi_lepton_met())<2; }
  const bool baseline_btag_2jets_st1_dphi1() const { return baseline() && njets()==2 && nbjets()>0 && st()<0.1 && fabs(dphi_lepton_met())<1; }
  const bool baseline_btag_2jets_st1_dphi2() const { return baseline() && njets()==2 && nbjets()>0 && st()<0.1 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_btag_2jets_st1_dphi3() const { return baseline() && njets()==2 && nbjets()>0 && st()<0.1 && fabs(dphi_lepton_met())<2; }
  const bool baseline_btag_2jets_st2_dphi1() const { return baseline() && njets()==2 && nbjets()>0 && st()<0.2 && fabs(dphi_lepton_met())<1; }
  const bool baseline_btag_2jets_st2_dphi2() const { return baseline() && njets()==2 && nbjets()>0 && st()<0.2 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_btag_2jets_st2_dphi3() const { return baseline() && njets()==2 && nbjets()>0 && st()<0.2 && fabs(dphi_lepton_met())<2; }
  const bool baseline_btag_2jets_st3_dphi1() const { return baseline() && njets()==2 && nbjets()>0 && st()<0.4 && fabs(dphi_lepton_met())<1; }
  const bool baseline_btag_2jets_st3_dphi2() const { return baseline() && njets()==2 && nbjets()>0 && st()<0.4 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_btag_2jets_st3_dphi3() const { return baseline() && njets()==2 && nbjets()>0 && st()<0.4 && fabs(dphi_lepton_met())<2; }
  const bool baseline_btag_3jets_st1_dphi1() const { return baseline() && njets()==3 && nbjets()>0 && st()<0.1 && fabs(dphi_lepton_met())<1; }
  const bool baseline_btag_3jets_st1_dphi2() const { return baseline() && njets()==3 && nbjets()>0 && st()<0.1 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_btag_3jets_st1_dphi3() const { return baseline() && njets()==3 && nbjets()>0 && st()<0.1 && fabs(dphi_lepton_met())<2; }
  const bool baseline_btag_3jets_st2_dphi1() const { return baseline() && njets()==3 && nbjets()>0 && st()<0.2 && fabs(dphi_lepton_met())<1; }
  const bool baseline_btag_3jets_st2_dphi2() const { return baseline() && njets()==3 && nbjets()>0 && st()<0.2 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_btag_3jets_st2_dphi3() const { return baseline() && njets()==3 && nbjets()>0 && st()<0.2 && fabs(dphi_lepton_met())<2; }
  const bool baseline_btag_3jets_st3_dphi1() const { return baseline() && njets()==3 && nbjets()>0 && st()<0.4 && fabs(dphi_lepton_met())<1; }
  const bool baseline_btag_3jets_st3_dphi2() const { return baseline() && njets()==3 && nbjets()>0 && st()<0.4 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_btag_3jets_st3_dphi3() const { return baseline() && njets()==3 && nbjets()>0 && st()<0.4 && fabs(dphi_lepton_met())<2; }
  const bool baseline_btag_2to3jets_st1_dphi1() const { return baseline() && njets()>1 && njets()<4 && nbjets()>0 && st()<0.1 && fabs(dphi_lepton_met())<1; }
  const bool baseline_btag_2to3jets_st1_dphi2() const { return baseline() && njets()>1 && njets()<4 && nbjets()>0 && st()<0.1 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_btag_2to3jets_st1_dphi3() const { return baseline() && njets()>1 && njets()<4 && nbjets()>0 && st()<0.1 && fabs(dphi_lepton_met())<2; }
  const bool baseline_btag_2to3jets_st2_dphi1() const { return baseline() && njets()>1 && njets()<4 && nbjets()>0 && st()<0.2 && fabs(dphi_lepton_met())<1; }
  const bool baseline_btag_2to3jets_st2_dphi2() const { return baseline() && njets()>1 && njets()<4 && nbjets()>0 && st()<0.2 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_btag_2to3jets_st2_dphi3() const { return baseline() && njets()>1 && njets()<4 && nbjets()>0 && st()<0.2 && fabs(dphi_lepton_met())<2; }
  const bool baseline_btag_2to3jets_st3_dphi1() const { return baseline() && njets()>1 && njets()<4 && nbjets()>0 && st()<0.4 && fabs(dphi_lepton_met())<1; }
  const bool baseline_btag_2to3jets_st3_dphi2() const { return baseline() && njets()>1 && njets()<4 && nbjets()>0 && st()<0.4 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_btag_2to3jets_st3_dphi3() const { return baseline() && njets()>1 && njets()<4 && nbjets()>0 && st()<0.4 && fabs(dphi_lepton_met())<2; }
  const bool baseline_btag_2to4jets_st1_dphi1() const { return baseline() && njets()>1 && njets()<5 && nbjets()>0 && st()<0.1 && fabs(dphi_lepton_met())<1; }
  const bool baseline_btag_2to4jets_st1_dphi2() const { return baseline() && njets()>1 && njets()<5 && nbjets()>0 && st()<0.1 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_btag_2to4jets_st1_dphi3() const { return baseline() && njets()>1 && njets()<5 && nbjets()>0 && st()<0.1 && fabs(dphi_lepton_met())<2; }
  const bool baseline_btag_2to4jets_st2_dphi1() const { return baseline() && njets()>1 && njets()<5 && nbjets()>0 && st()<0.2 && fabs(dphi_lepton_met())<1; }
  const bool baseline_btag_2to4jets_st2_dphi2() const { return baseline() && njets()>1 && njets()<5 && nbjets()>0 && st()<0.2 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_btag_2to4jets_st2_dphi3() const { return baseline() && njets()>1 && njets()<5 && nbjets()>0 && st()<0.2 && fabs(dphi_lepton_met())<2; }
  const bool baseline_btag_2to4jets_st3_dphi1() const { return baseline() && njets()>1 && njets()<5 && nbjets()>0 && st()<0.4 && fabs(dphi_lepton_met())<1; }
  const bool baseline_btag_2to4jets_st3_dphi2() const { return baseline() && njets()>1 && njets()<5 && nbjets()>0 && st()<0.4 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_btag_2to4jets_st3_dphi3() const { return baseline() && njets()>1 && njets()<5 && nbjets()>0 && st()<0.4 && fabs(dphi_lepton_met())<2; }

  const bool baseline_1btag_jets() const { return baseline() && njets()>1 && nbjets()==1; }
  const bool baseline_1btag_2jets() const { return baseline() && njets()==2 && nbjets()==1; }
  const bool baseline_1btag_3jets() const { return baseline() && njets()==3 && nbjets()==1; }
  const bool baseline_1btag_4jets() const { return baseline() && njets()==4 && nbjets()==1; }
  const bool baseline_1btag_5jets() const { return baseline() && njets()==5 && nbjets()==1; }
  const bool baseline_1btag_6upjets() const { return baseline() && njets()>=6 && nbjets()==1; }
  const bool baseline_1btag_2to3jets() const { return baseline() && njets()>1 && njets()<4 && nbjets()==1; }
  const bool baseline_1btag_2to4jets() const { return baseline() && njets()>1 && njets()<5 && nbjets()==1; }
  const bool baseline_1btag_jets_st1() const { return baseline() && njets()>1 && nbjets()==1 && st()<0.1; }
  const bool baseline_1btag_jets_st2() const { return baseline() && njets()>1 && nbjets()==1 && st()<0.2; }
  const bool baseline_1btag_jets_st3() const { return baseline() && njets()>1 && nbjets()==1 && st()<0.4; }
  const bool baseline_1btag_2jets_st1() const { return baseline() && njets()==2 && nbjets()==1 && st()<0.1; }
  const bool baseline_1btag_2jets_st2() const { return baseline() && njets()==2 && nbjets()==1 && st()<0.2; }
  const bool baseline_1btag_2jets_st3() const { return baseline() && njets()==2 && nbjets()==1 && st()<0.4; }
  const bool baseline_1btag_3jets_st1() const { return baseline() && njets()==3 && nbjets()==1 && st()<0.1; }
  const bool baseline_1btag_3jets_st2() const { return baseline() && njets()==3 && nbjets()==1 && st()<0.2; }
  const bool baseline_1btag_3jets_st3() const { return baseline() && njets()==3 && nbjets()==1 && st()<0.4; }
  const bool baseline_1btag_4jets_st1() const { return baseline() && njets()==4 && nbjets()==1 && st()<0.1; }
  const bool baseline_1btag_4jets_st2() const { return baseline() && njets()==4 && nbjets()==1 && st()<0.2; }
  const bool baseline_1btag_4jets_st3() const { return baseline() && njets()==4 && nbjets()==1 && st()<0.4; }
  const bool baseline_1btag_5jets_st1() const { return baseline() && njets()==5 && nbjets()==1 && st()<0.1; }
  const bool baseline_1btag_5jets_st2() const { return baseline() && njets()==5 && nbjets()==1 && st()<0.2; }
  const bool baseline_1btag_5jets_st3() const { return baseline() && njets()==5 && nbjets()==1 && st()<0.4; }
  const bool baseline_1btag_6upjets_st1() const { return baseline() && njets()>=6 && nbjets()==1 && st()<0.1; }
  const bool baseline_1btag_6upjets_st2() const { return baseline() && njets()>=6 && nbjets()==1 && st()<0.2; }
  const bool baseline_1btag_6upjets_st3() const { return baseline() && njets()>=6 && nbjets()==1 && st()<0.4; }
  const bool baseline_1btag_2to3jets_st1() const { return baseline() && njets()>1 && njets()<4 && nbjets()==1 && st()<0.1; }
  const bool baseline_1btag_2to3jets_st2() const { return baseline() && njets()>1 && njets()<4 && nbjets()==1 && st()<0.2; }
  const bool baseline_1btag_2to3jets_st3() const { return baseline() && njets()>1 && njets()<4 && nbjets()==1 && st()<0.4; }
  const bool baseline_1btag_2to4jets_st1() const { return baseline() && njets()>1 && njets()<5 && nbjets()==1 && st()<0.1; }
  const bool baseline_1btag_2to4jets_st2() const { return baseline() && njets()>1 && njets()<5 && nbjets()==1 && st()<0.2; }
  const bool baseline_1btag_2to4jets_st3() const { return baseline() && njets()>1 && njets()<5 && nbjets()==1 && st()<0.4; }
  const bool baseline_1btag_jets_dphi1() const { return baseline() && njets()>1 && nbjets()==1 && fabs(dphi_lepton_met())<1; }
  const bool baseline_1btag_jets_dphi2() const { return baseline() && njets()>1 && nbjets()==1 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_1btag_jets_dphi3() const { return baseline() && njets()>1 && nbjets()==1 && fabs(dphi_lepton_met())<2; }
  const bool baseline_1btag_2jets_dphi1() const { return baseline() && njets()==2 && nbjets()==1 && fabs(dphi_lepton_met())<1; }
  const bool baseline_1btag_2jets_dphi2() const { return baseline() && njets()==2 && nbjets()==1 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_1btag_2jets_dphi3() const { return baseline() && njets()==2 && nbjets()==1 && fabs(dphi_lepton_met())<2; }
  const bool baseline_1btag_3jets_dphi1() const { return baseline() && njets()==3 && nbjets()==1 && fabs(dphi_lepton_met())<1; }
  const bool baseline_1btag_3jets_dphi2() const { return baseline() && njets()==3 && nbjets()==1 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_1btag_3jets_dphi3() const { return baseline() && njets()==3 && nbjets()==1 && fabs(dphi_lepton_met())<2; }
  const bool baseline_1btag_4jets_dphi1() const { return baseline() && njets()==4 && nbjets()==1 && fabs(dphi_lepton_met())<1; }
  const bool baseline_1btag_4jets_dphi2() const { return baseline() && njets()==4 && nbjets()==1 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_1btag_4jets_dphi3() const { return baseline() && njets()==4 && nbjets()==1 && fabs(dphi_lepton_met())<2; }
  const bool baseline_1btag_5jets_dphi1() const { return baseline() && njets()==5 && nbjets()==1 && fabs(dphi_lepton_met())<1; }
  const bool baseline_1btag_5jets_dphi2() const { return baseline() && njets()==5 && nbjets()==1 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_1btag_5jets_dphi3() const { return baseline() && njets()==5 && nbjets()==1 && fabs(dphi_lepton_met())<2; }
  const bool baseline_1btag_6upjets_dphi1() const { return baseline() && njets()>=6 && nbjets()==1 && fabs(dphi_lepton_met())<1; }
  const bool baseline_1btag_6upjets_dphi2() const { return baseline() && njets()>=6 && nbjets()==1 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_1btag_6upjets_dphi3() const { return baseline() && njets()>=6 && nbjets()==1 && fabs(dphi_lepton_met())<2; }
  const bool baseline_1btag_2to3jets_dphi1() const { return baseline() && njets()>1 && njets()<4 && nbjets()==1 && fabs(dphi_lepton_met())<1; }
  const bool baseline_1btag_2to3jets_dphi2() const { return baseline() && njets()>1 && njets()<4 && nbjets()==1 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_1btag_2to3jets_dphi3() const { return baseline() && njets()>1 && njets()<4 && nbjets()==1 && fabs(dphi_lepton_met())<2; }
  const bool baseline_1btag_2to4jets_dphi1() const { return baseline() && njets()>1 && njets()<5 && nbjets()==1 && fabs(dphi_lepton_met())<1; }
  const bool baseline_1btag_2to4jets_dphi2() const { return baseline() && njets()>1 && njets()<5 && nbjets()==1 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_1btag_2to4jets_dphi3() const { return baseline() && njets()>1 && njets()<5 && nbjets()==1 && fabs(dphi_lepton_met())<2; }
  const bool baseline_1btag_jets_st1_dphi1() const { return baseline() && njets()>1 && nbjets()==1 && st()<0.1 && fabs(dphi_lepton_met())<1; }
  const bool baseline_1btag_jets_st1_dphi2() const { return baseline() && njets()>1 && nbjets()==1 && st()<0.1 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_1btag_jets_st1_dphi3() const { return baseline() && njets()>1 && nbjets()==1 && st()<0.1 && fabs(dphi_lepton_met())<2; }
  const bool baseline_1btag_jets_st2_dphi1() const { return baseline() && njets()>1 && nbjets()==1 && st()<0.2 && fabs(dphi_lepton_met())<1; }
  const bool baseline_1btag_jets_st2_dphi2() const { return baseline() && njets()>1 && nbjets()==1 && st()<0.2 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_1btag_jets_st2_dphi3() const { return baseline() && njets()>1 && nbjets()==1 && st()<0.2 && fabs(dphi_lepton_met())<2; }
  const bool baseline_1btag_jets_st3_dphi1() const { return baseline() && njets()>1 && nbjets()==1 && st()<0.4 && fabs(dphi_lepton_met())<1; }
  const bool baseline_1btag_jets_st3_dphi2() const { return baseline() && njets()>1 && nbjets()==1 && st()<0.4 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_1btag_jets_st3_dphi3() const { return baseline() && njets()>1 && nbjets()==1 && st()<0.4 && fabs(dphi_lepton_met())<2; }
  const bool baseline_1btag_2jets_st1_dphi1() const { return baseline() && njets()==2 && nbjets()==1 && st()<0.1 && fabs(dphi_lepton_met())<1; }
  const bool baseline_1btag_2jets_st1_dphi2() const { return baseline() && njets()==2 && nbjets()==1 && st()<0.1 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_1btag_2jets_st1_dphi3() const { return baseline() && njets()==2 && nbjets()==1 && st()<0.1 && fabs(dphi_lepton_met())<2; }
  const bool baseline_1btag_2jets_st2_dphi1() const { return baseline() && njets()==2 && nbjets()==1 && st()<0.2 && fabs(dphi_lepton_met())<1; }
  const bool baseline_1btag_2jets_st2_dphi2() const { return baseline() && njets()==2 && nbjets()==1 && st()<0.2 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_1btag_2jets_st2_dphi3() const { return baseline() && njets()==2 && nbjets()==1 && st()<0.2 && fabs(dphi_lepton_met())<2; }
  const bool baseline_1btag_2jets_st3_dphi1() const { return baseline() && njets()==2 && nbjets()==1 && st()<0.4 && fabs(dphi_lepton_met())<1; }
  const bool baseline_1btag_2jets_st3_dphi2() const { return baseline() && njets()==2 && nbjets()==1 && st()<0.4 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_1btag_2jets_st3_dphi3() const { return baseline() && njets()==2 && nbjets()==1 && st()<0.4 && fabs(dphi_lepton_met())<2; }
  const bool baseline_1btag_3jets_st1_dphi1() const { return baseline() && njets()==3 && nbjets()==1 && st()<0.1 && fabs(dphi_lepton_met())<1; }
  const bool baseline_1btag_3jets_st1_dphi2() const { return baseline() && njets()==3 && nbjets()==1 && st()<0.1 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_1btag_3jets_st1_dphi3() const { return baseline() && njets()==3 && nbjets()==1 && st()<0.1 && fabs(dphi_lepton_met())<2; }
  const bool baseline_1btag_3jets_st2_dphi1() const { return baseline() && njets()==3 && nbjets()==1 && st()<0.2 && fabs(dphi_lepton_met())<1; }
  const bool baseline_1btag_3jets_st2_dphi2() const { return baseline() && njets()==3 && nbjets()==1 && st()<0.2 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_1btag_3jets_st2_dphi3() const { return baseline() && njets()==3 && nbjets()==1 && st()<0.2 && fabs(dphi_lepton_met())<2; }
  const bool baseline_1btag_3jets_st3_dphi1() const { return baseline() && njets()==3 && nbjets()==1 && st()<0.4 && fabs(dphi_lepton_met())<1; }
  const bool baseline_1btag_3jets_st3_dphi2() const { return baseline() && njets()==3 && nbjets()==1 && st()<0.4 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_1btag_3jets_st3_dphi3() const { return baseline() && njets()==3 && nbjets()==1 && st()<0.4 && fabs(dphi_lepton_met())<2; }
  const bool baseline_1btag_4jets_st1_dphi1() const { return baseline() && njets()==4 && nbjets()==1 && st()<0.1 && fabs(dphi_lepton_met())<1; }
  const bool baseline_1btag_4jets_st1_dphi2() const { return baseline() && njets()==4 && nbjets()==1 && st()<0.1 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_1btag_4jets_st1_dphi3() const { return baseline() && njets()==4 && nbjets()==1 && st()<0.1 && fabs(dphi_lepton_met())<2; }
  const bool baseline_1btag_4jets_st2_dphi1() const { return baseline() && njets()==4 && nbjets()==1 && st()<0.2 && fabs(dphi_lepton_met())<1; }
  const bool baseline_1btag_4jets_st2_dphi2() const { return baseline() && njets()==4 && nbjets()==1 && st()<0.2 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_1btag_4jets_st2_dphi3() const { return baseline() && njets()==4 && nbjets()==1 && st()<0.2 && fabs(dphi_lepton_met())<2; }
  const bool baseline_1btag_4jets_st3_dphi1() const { return baseline() && njets()==4 && nbjets()==1 && st()<0.4 && fabs(dphi_lepton_met())<1; }
  const bool baseline_1btag_4jets_st3_dphi2() const { return baseline() && njets()==4 && nbjets()==1 && st()<0.4 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_1btag_4jets_st3_dphi3() const { return baseline() && njets()==4 && nbjets()==1 && st()<0.4 && fabs(dphi_lepton_met())<2; }
  const bool baseline_1btag_5jets_st1_dphi1() const { return baseline() && njets()==5 && nbjets()==1 && st()<0.1 && fabs(dphi_lepton_met())<1; }
  const bool baseline_1btag_5jets_st1_dphi2() const { return baseline() && njets()==5 && nbjets()==1 && st()<0.1 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_1btag_5jets_st1_dphi3() const { return baseline() && njets()==5 && nbjets()==1 && st()<0.1 && fabs(dphi_lepton_met())<2; }
  const bool baseline_1btag_5jets_st2_dphi1() const { return baseline() && njets()==5 && nbjets()==1 && st()<0.2 && fabs(dphi_lepton_met())<1; }
  const bool baseline_1btag_5jets_st2_dphi2() const { return baseline() && njets()==5 && nbjets()==1 && st()<0.2 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_1btag_5jets_st2_dphi3() const { return baseline() && njets()==5 && nbjets()==1 && st()<0.2 && fabs(dphi_lepton_met())<2; }
  const bool baseline_1btag_5jets_st3_dphi1() const { return baseline() && njets()==5 && nbjets()==1 && st()<0.4 && fabs(dphi_lepton_met())<1; }
  const bool baseline_1btag_5jets_st3_dphi2() const { return baseline() && njets()==5 && nbjets()==1 && st()<0.4 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_1btag_5jets_st3_dphi3() const { return baseline() && njets()==5 && nbjets()==1 && st()<0.4 && fabs(dphi_lepton_met())<2; }
  const bool baseline_1btag_6upjets_st1_dphi1() const { return baseline() && njets()>=6 && nbjets()==1 && st()<0.1 && fabs(dphi_lepton_met())<1; }
  const bool baseline_1btag_6upjets_st1_dphi2() const { return baseline() && njets()>=6 && nbjets()==1 && st()<0.1 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_1btag_6upjets_st1_dphi3() const { return baseline() && njets()>=6 && nbjets()==1 && st()<0.1 && fabs(dphi_lepton_met())<2; }
  const bool baseline_1btag_6upjets_st2_dphi1() const { return baseline() && njets()>=6 && nbjets()==1 && st()<0.2 && fabs(dphi_lepton_met())<1; }
  const bool baseline_1btag_6upjets_st2_dphi2() const { return baseline() && njets()>=6 && nbjets()==1 && st()<0.2 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_1btag_6upjets_st2_dphi3() const { return baseline() && njets()>=6 && nbjets()==1 && st()<0.2 && fabs(dphi_lepton_met())<2; }
  const bool baseline_1btag_6upjets_st3_dphi1() const { return baseline() && njets()>=6 && nbjets()==1 && st()<0.4 && fabs(dphi_lepton_met())<1; }
  const bool baseline_1btag_6upjets_st3_dphi2() const { return baseline() && njets()>=6 && nbjets()==1 && st()<0.4 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_1btag_6upjets_st3_dphi3() const { return baseline() && njets()>=6 && nbjets()==1 && st()<0.4 && fabs(dphi_lepton_met())<2; }

  const bool baseline_1btag_2to3jets_st1_dphi1() const { return baseline() && njets()>1 && njets()<4 && nbjets()==1 && st()<0.1 && fabs(dphi_lepton_met())<1; }
  const bool baseline_1btag_2to3jets_st1_dphi2() const { return baseline() && njets()>1 && njets()<4 && nbjets()==1 && st()<0.1 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_1btag_2to3jets_st1_dphi3() const { return baseline() && njets()>1 && njets()<4 && nbjets()==1 && st()<0.1 && fabs(dphi_lepton_met())<2; }
  const bool baseline_1btag_2to3jets_st2_dphi1() const { return baseline() && njets()>1 && njets()<4 && nbjets()==1 && st()<0.2 && fabs(dphi_lepton_met())<1; }
  const bool baseline_1btag_2to3jets_st2_dphi2() const { return baseline() && njets()>1 && njets()<4 && nbjets()==1 && st()<0.2 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_1btag_2to3jets_st2_dphi3() const { return baseline() && njets()>1 && njets()<4 && nbjets()==1 && st()<0.2 && fabs(dphi_lepton_met())<2; }
  const bool baseline_1btag_2to3jets_st3_dphi1() const { return baseline() && njets()>1 && njets()<4 && nbjets()==1 && st()<0.4 && fabs(dphi_lepton_met())<1; }
  const bool baseline_1btag_2to3jets_st3_dphi2() const { return baseline() && njets()>1 && njets()<4 && nbjets()==1 && st()<0.4 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_1btag_2to3jets_st3_dphi3() const { return baseline() && njets()>1 && njets()<4 && nbjets()==1 && st()<0.4 && fabs(dphi_lepton_met())<2; }
  const bool baseline_1btag_2to4jets_st1_dphi1() const { return baseline() && njets()>1 && njets()<5 && nbjets()==1 && st()<0.1 && fabs(dphi_lepton_met())<1; }
  const bool baseline_1btag_2to4jets_st1_dphi2() const { return baseline() && njets()>1 && njets()<5 && nbjets()==1 && st()<0.1 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_1btag_2to4jets_st1_dphi3() const { return baseline() && njets()>1 && njets()<5 && nbjets()==1 && st()<0.1 && fabs(dphi_lepton_met())<2; }
  const bool baseline_1btag_2to4jets_st2_dphi1() const { return baseline() && njets()>1 && njets()<5 && nbjets()==1 && st()<0.2 && fabs(dphi_lepton_met())<1; }
  const bool baseline_1btag_2to4jets_st2_dphi2() const { return baseline() && njets()>1 && njets()<5 && nbjets()==1 && st()<0.2 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_1btag_2to4jets_st2_dphi3() const { return baseline() && njets()>1 && njets()<5 && nbjets()==1 && st()<0.2 && fabs(dphi_lepton_met())<2; }
  const bool baseline_1btag_2to4jets_st3_dphi1() const { return baseline() && njets()>1 && njets()<5 && nbjets()==1 && st()<0.4 && fabs(dphi_lepton_met())<1; }
  const bool baseline_1btag_2to4jets_st3_dphi2() const { return baseline() && njets()>1 && njets()<5 && nbjets()==1 && st()<0.4 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_1btag_2to4jets_st3_dphi3() const { return baseline() && njets()>1 && njets()<5 && nbjets()==1 && st()<0.4 && fabs(dphi_lepton_met())<2; }

  const bool baseline_2btag() const { return baseline() && nbjets()==2; }
  const bool baseline_2btag_jets() const { return baseline() && njets()>1 && nbjets()==2; }
  const bool baseline_2btag_2jets() const { return baseline() && njets()==2 && nbjets()==2; }
  const bool baseline_2btag_3jets() const { return baseline() && njets()==3 && nbjets()==2; }
  const bool baseline_2btag_4jets() const { return baseline() && njets()==4 && nbjets()==2; }
  const bool baseline_2btag_5jets() const { return baseline() && njets()==5 && nbjets()==2; }
  const bool baseline_2btag_6upjets() const { return baseline() && njets()>=6 && nbjets()==2; }
  const bool baseline_2btag_2to3jets() const { return baseline() && njets()>1 && njets()<4 && nbjets()==2; }
  const bool baseline_2btag_2to4jets() const { return baseline() && njets()>1 && njets()<5 && nbjets()==2; }
  const bool baseline_2btag_jets_st1() const { return baseline() && njets()>1 && nbjets()==2 && st()<0.1; }
  const bool baseline_2btag_jets_st2() const { return baseline() && njets()>1 && nbjets()==2 && st()<0.2; }
  const bool baseline_2btag_jets_st3() const { return baseline() && njets()>1 && nbjets()==2 && st()<0.4; }
  const bool baseline_2btag_2jets_st1() const { return baseline() && njets()==2 && nbjets()==2 && st()<0.1; }
  const bool baseline_2btag_2jets_st2() const { return baseline() && njets()==2 && nbjets()==2 && st()<0.2; }
  const bool baseline_2btag_2jets_st3() const { return baseline() && njets()==2 && nbjets()==2 && st()<0.4; }
  const bool baseline_2btag_3jets_st1() const { return baseline() && njets()==3 && nbjets()==2 && st()<0.1; }
  const bool baseline_2btag_3jets_st2() const { return baseline() && njets()==3 && nbjets()==2 && st()<0.2; }
  const bool baseline_2btag_3jets_st3() const { return baseline() && njets()==3 && nbjets()==2 && st()<0.4; }
  const bool baseline_2btag_4jets_st1() const { return baseline() && njets()==4 && nbjets()==2 && st()<0.1; }
  const bool baseline_2btag_4jets_st2() const { return baseline() && njets()==4 && nbjets()==2 && st()<0.2; }
  const bool baseline_2btag_4jets_st3() const { return baseline() && njets()==4 && nbjets()==2 && st()<0.4; }
  const bool baseline_2btag_5jets_st1() const { return baseline() && njets()==5 && nbjets()==2 && st()<0.1; }
  const bool baseline_2btag_5jets_st2() const { return baseline() && njets()==5 && nbjets()==2 && st()<0.2; }
  const bool baseline_2btag_5jets_st3() const { return baseline() && njets()==5 && nbjets()==2 && st()<0.4; }
  const bool baseline_2btag_6upjets_st1() const { return baseline() && njets()>=6 && nbjets()==2 && st()<0.1; }
  const bool baseline_2btag_6upjets_st2() const { return baseline() && njets()>=6 && nbjets()==2 && st()<0.2; }
  const bool baseline_2btag_6upjets_st3() const { return baseline() && njets()>=6 && nbjets()==2 && st()<0.4; }
  const bool baseline_2btag_2to3jets_st1() const { return baseline() && njets()>1 && njets()<4 && nbjets()==2 && st()<0.1; }
  const bool baseline_2btag_2to3jets_st2() const { return baseline() && njets()>1 && njets()<4 && nbjets()==2 && st()<0.2; }
  const bool baseline_2btag_2to3jets_st3() const { return baseline() && njets()>1 && njets()<4 && nbjets()==2 && st()<0.4; }
  const bool baseline_2btag_2to4jets_st1() const { return baseline() && njets()>1 && njets()<5 && nbjets()==2 && st()<0.1; }
  const bool baseline_2btag_2to4jets_st2() const { return baseline() && njets()>1 && njets()<5 && nbjets()==2 && st()<0.2; }
  const bool baseline_2btag_2to4jets_st3() const { return baseline() && njets()>1 && njets()<5 && nbjets()==2 && st()<0.4; }
  const bool baseline_2btag_jets_dphi1() const { return baseline() && njets()>1 && nbjets()==2 && fabs(dphi_lepton_met())<1; }
  const bool baseline_2btag_jets_dphi2() const { return baseline() && njets()>1 && nbjets()==2 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_2btag_jets_dphi3() const { return baseline() && njets()>1 && nbjets()==2 && fabs(dphi_lepton_met())<2; }
  const bool baseline_2btag_2jets_dphi1() const { return baseline() && njets()==2 && nbjets()==2 && fabs(dphi_lepton_met())<1; }
  const bool baseline_2btag_2jets_dphi2() const { return baseline() && njets()==2 && nbjets()==2 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_2btag_2jets_dphi3() const { return baseline() && njets()==2 && nbjets()==2 && fabs(dphi_lepton_met())<2; }
  const bool baseline_2btag_3jets_dphi1() const { return baseline() && njets()==3 && nbjets()==2 && fabs(dphi_lepton_met())<1; }
  const bool baseline_2btag_3jets_dphi2() const { return baseline() && njets()==3 && nbjets()==2 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_2btag_3jets_dphi3() const { return baseline() && njets()==3 && nbjets()==2 && fabs(dphi_lepton_met())<2; }
  const bool baseline_2btag_4jets_dphi1() const { return baseline() && njets()==4 && nbjets()==2 && fabs(dphi_lepton_met())<1; }
  const bool baseline_2btag_4jets_dphi2() const { return baseline() && njets()==4 && nbjets()==2 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_2btag_4jets_dphi3() const { return baseline() && njets()==4 && nbjets()==2 && fabs(dphi_lepton_met())<2; }
  const bool baseline_2btag_5jets_dphi1() const { return baseline() && njets()==5 && nbjets()==2 && fabs(dphi_lepton_met())<1; }
  const bool baseline_2btag_5jets_dphi2() const { return baseline() && njets()==5 && nbjets()==2 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_2btag_5jets_dphi3() const { return baseline() && njets()==5 && nbjets()==2 && fabs(dphi_lepton_met())<2; }
  const bool baseline_2btag_6upjets_dphi1() const { return baseline() && njets()>=6 && nbjets()==2 && fabs(dphi_lepton_met())<1; }
  const bool baseline_2btag_6upjets_dphi2() const { return baseline() && njets()>=6 && nbjets()==2 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_2btag_6upjets_dphi3() const { return baseline() && njets()>=6 && nbjets()==2 && fabs(dphi_lepton_met())<2; }
  const bool baseline_2btag_2to3jets_dphi1() const { return baseline() && njets()>1 && njets()<4 && nbjets()==2 && fabs(dphi_lepton_met())<1; }
  const bool baseline_2btag_2to3jets_dphi2() const { return baseline() && njets()>1 && njets()<4 && nbjets()==2 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_2btag_2to3jets_dphi3() const { return baseline() && njets()>1 && njets()<4 && nbjets()==2 && fabs(dphi_lepton_met())<2; }
  const bool baseline_2btag_2to4jets_dphi1() const { return baseline() && njets()>1 && njets()<5 && nbjets()==2 && fabs(dphi_lepton_met())<1; }
  const bool baseline_2btag_2to4jets_dphi2() const { return baseline() && njets()>1 && njets()<5 && nbjets()==2 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_2btag_2to4jets_dphi3() const { return baseline() && njets()>1 && njets()<5 && nbjets()==2 && fabs(dphi_lepton_met())<2; }
  const bool baseline_2btag_jets_st1_dphi1() const { return baseline() && njets()>1 && nbjets()==2 && st()<0.1 && fabs(dphi_lepton_met())<1; }
  const bool baseline_2btag_jets_st1_dphi2() const { return baseline() && njets()>1 && nbjets()==2 && st()<0.1 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_2btag_jets_st1_dphi3() const { return baseline() && njets()>1 && nbjets()==2 && st()<0.1 && fabs(dphi_lepton_met())<2; }
  const bool baseline_2btag_jets_st2_dphi1() const { return baseline() && njets()>1 && nbjets()==2 && st()<0.2 && fabs(dphi_lepton_met())<1; }
  const bool baseline_2btag_jets_st2_dphi2() const { return baseline() && njets()>1 && nbjets()==2 && st()<0.2 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_2btag_jets_st2_dphi3() const { return baseline() && njets()>1 && nbjets()==2 && st()<0.2 && fabs(dphi_lepton_met())<2; }
  const bool baseline_2btag_jets_st3_dphi1() const { return baseline() && njets()>1 && nbjets()==2 && st()<0.4 && fabs(dphi_lepton_met())<1; }
  const bool baseline_2btag_jets_st3_dphi2() const { return baseline() && njets()>1 && nbjets()==2 && st()<0.4 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_2btag_jets_st3_dphi3() const { return baseline() && njets()>1 && nbjets()==2 && st()<0.4 && fabs(dphi_lepton_met())<2; }
  const bool baseline_2btag_2jets_st1_dphi1() const { return baseline() && njets()==2 && nbjets()==2 && st()<0.1 && fabs(dphi_lepton_met())<1; }
  const bool baseline_2btag_2jets_st1_dphi2() const { return baseline() && njets()==2 && nbjets()==2 && st()<0.1 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_2btag_2jets_st1_dphi3() const { return baseline() && njets()==2 && nbjets()==2 && st()<0.1 && fabs(dphi_lepton_met())<2; }
  const bool baseline_2btag_2jets_st2_dphi1() const { return baseline() && njets()==2 && nbjets()==2 && st()<0.2 && fabs(dphi_lepton_met())<1; }
  const bool baseline_2btag_2jets_st2_dphi2() const { return baseline() && njets()==2 && nbjets()==2 && st()<0.2 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_2btag_2jets_st2_dphi3() const { return baseline() && njets()==2 && nbjets()==2 && st()<0.2 && fabs(dphi_lepton_met())<2; }
  const bool baseline_2btag_2jets_st3_dphi1() const { return baseline() && njets()==2 && nbjets()==2 && st()<0.4 && fabs(dphi_lepton_met())<1; }
  const bool baseline_2btag_2jets_st3_dphi2() const { return baseline() && njets()==2 && nbjets()==2 && st()<0.4 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_2btag_2jets_st3_dphi3() const { return baseline() && njets()==2 && nbjets()==2 && st()<0.4 && fabs(dphi_lepton_met())<2; }
  const bool baseline_2btag_3jets_st1_dphi1() const { return baseline() && njets()==3 && nbjets()==2 && st()<0.1 && fabs(dphi_lepton_met())<1; }
  const bool baseline_2btag_3jets_st1_dphi2() const { return baseline() && njets()==3 && nbjets()==2 && st()<0.1 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_2btag_3jets_st1_dphi3() const { return baseline() && njets()==3 && nbjets()==2 && st()<0.1 && fabs(dphi_lepton_met())<2; }
  const bool baseline_2btag_3jets_st2_dphi1() const { return baseline() && njets()==3 && nbjets()==2 && st()<0.2 && fabs(dphi_lepton_met())<1; }
  const bool baseline_2btag_3jets_st2_dphi2() const { return baseline() && njets()==3 && nbjets()==2 && st()<0.2 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_2btag_3jets_st2_dphi3() const { return baseline() && njets()==3 && nbjets()==2 && st()<0.2 && fabs(dphi_lepton_met())<2; }
  const bool baseline_2btag_3jets_st3_dphi1() const { return baseline() && njets()==3 && nbjets()==2 && st()<0.4 && fabs(dphi_lepton_met())<1; }
  const bool baseline_2btag_3jets_st3_dphi2() const { return baseline() && njets()==3 && nbjets()==2 && st()<0.4 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_2btag_3jets_st3_dphi3() const { return baseline() && njets()==3 && nbjets()==2 && st()<0.4 && fabs(dphi_lepton_met())<2; }
  const bool baseline_2btag_4jets_st1_dphi1() const { return baseline() && njets()==4 && nbjets()==2 && st()<0.1 && fabs(dphi_lepton_met())<1; }
  const bool baseline_2btag_4jets_st1_dphi2() const { return baseline() && njets()==4 && nbjets()==2 && st()<0.1 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_2btag_4jets_st1_dphi3() const { return baseline() && njets()==4 && nbjets()==2 && st()<0.1 && fabs(dphi_lepton_met())<2; }
  const bool baseline_2btag_4jets_st2_dphi1() const { return baseline() && njets()==4 && nbjets()==2 && st()<0.2 && fabs(dphi_lepton_met())<1; }
  const bool baseline_2btag_4jets_st2_dphi2() const { return baseline() && njets()==4 && nbjets()==2 && st()<0.2 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_2btag_4jets_st2_dphi3() const { return baseline() && njets()==4 && nbjets()==2 && st()<0.2 && fabs(dphi_lepton_met())<2; }
  const bool baseline_2btag_4jets_st3_dphi1() const { return baseline() && njets()==4 && nbjets()==2 && st()<0.4 && fabs(dphi_lepton_met())<1; }
  const bool baseline_2btag_4jets_st3_dphi2() const { return baseline() && njets()==4 && nbjets()==2 && st()<0.4 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_2btag_4jets_st3_dphi3() const { return baseline() && njets()==4 && nbjets()==2 && st()<0.4 && fabs(dphi_lepton_met())<2; }
  const bool baseline_2btag_5jets_st1_dphi1() const { return baseline() && njets()==5 && nbjets()==2 && st()<0.1 && fabs(dphi_lepton_met())<1; }
  const bool baseline_2btag_5jets_st1_dphi2() const { return baseline() && njets()==5 && nbjets()==2 && st()<0.1 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_2btag_5jets_st1_dphi3() const { return baseline() && njets()==5 && nbjets()==2 && st()<0.1 && fabs(dphi_lepton_met())<2; }
  const bool baseline_2btag_5jets_st2_dphi1() const { return baseline() && njets()==5 && nbjets()==2 && st()<0.2 && fabs(dphi_lepton_met())<1; }
  const bool baseline_2btag_5jets_st2_dphi2() const { return baseline() && njets()==5 && nbjets()==2 && st()<0.2 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_2btag_5jets_st2_dphi3() const { return baseline() && njets()==5 && nbjets()==2 && st()<0.2 && fabs(dphi_lepton_met())<2; }
  const bool baseline_2btag_5jets_st3_dphi1() const { return baseline() && njets()==5 && nbjets()==2 && st()<0.4 && fabs(dphi_lepton_met())<1; }
  const bool baseline_2btag_5jets_st3_dphi2() const { return baseline() && njets()==5 && nbjets()==2 && st()<0.4 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_2btag_5jets_st3_dphi3() const { return baseline() && njets()==5 && nbjets()==2 && st()<0.4 && fabs(dphi_lepton_met())<2; }
  const bool baseline_2btag_6upjets_st1_dphi1() const { return baseline() && njets()>=6 && nbjets()==2 && st()<0.1 && fabs(dphi_lepton_met())<1; }
  const bool baseline_2btag_6upjets_st1_dphi2() const { return baseline() && njets()>=6 && nbjets()==2 && st()<0.1 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_2btag_6upjets_st1_dphi3() const { return baseline() && njets()>=6 && nbjets()==2 && st()<0.1 && fabs(dphi_lepton_met())<2; }
  const bool baseline_2btag_6upjets_st2_dphi1() const { return baseline() && njets()>=6 && nbjets()==2 && st()<0.2 && fabs(dphi_lepton_met())<1; }
  const bool baseline_2btag_6upjets_st2_dphi2() const { return baseline() && njets()>=6 && nbjets()==2 && st()<0.2 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_2btag_6upjets_st2_dphi3() const { return baseline() && njets()>=6 && nbjets()==2 && st()<0.2 && fabs(dphi_lepton_met())<2; }
  const bool baseline_2btag_6upjets_st3_dphi1() const { return baseline() && njets()>=6 && nbjets()==2 && st()<0.4 && fabs(dphi_lepton_met())<1; }
  const bool baseline_2btag_6upjets_st3_dphi2() const { return baseline() && njets()>=6 && nbjets()==2 && st()<0.4 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_2btag_6upjets_st3_dphi3() const { return baseline() && njets()>=6 && nbjets()==2 && st()<0.4 && fabs(dphi_lepton_met())<2; }
  const bool baseline_2btag_2to3jets_st1_dphi1() const { return baseline() && njets()>1 && njets()<4 && nbjets()==2 && st()<0.1 && fabs(dphi_lepton_met())<1; }
  const bool baseline_2btag_2to3jets_st1_dphi2() const { return baseline() && njets()>1 && njets()<4 && nbjets()==2 && st()<0.1 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_2btag_2to3jets_st1_dphi3() const { return baseline() && njets()>1 && njets()<4 && nbjets()==2 && st()<0.1 && fabs(dphi_lepton_met())<2; }
  const bool baseline_2btag_2to3jets_st2_dphi1() const { return baseline() && njets()>1 && njets()<4 && nbjets()==2 && st()<0.2 && fabs(dphi_lepton_met())<1; }
  const bool baseline_2btag_2to3jets_st2_dphi2() const { return baseline() && njets()>1 && njets()<4 && nbjets()==2 && st()<0.2 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_2btag_2to3jets_st2_dphi3() const { return baseline() && njets()>1 && njets()<4 && nbjets()==2 && st()<0.2 && fabs(dphi_lepton_met())<2; }
  const bool baseline_2btag_2to3jets_st3_dphi1() const { return baseline() && njets()>1 && njets()<4 && nbjets()==2 && st()<0.4 && fabs(dphi_lepton_met())<1; }
  const bool baseline_2btag_2to3jets_st3_dphi2() const { return baseline() && njets()>1 && njets()<4 && nbjets()==2 && st()<0.4 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_2btag_2to3jets_st3_dphi3() const { return baseline() && njets()>1 && njets()<4 && nbjets()==2 && st()<0.4 && fabs(dphi_lepton_met())<2; }
  const bool baseline_2btag_2to4jets_st1_dphi1() const { return baseline() && njets()>1 && njets()<5 && nbjets()==2 && st()<0.1 && fabs(dphi_lepton_met())<1; }
  const bool baseline_2btag_2to4jets_st1_dphi2() const { return baseline() && njets()>1 && njets()<5 && nbjets()==2 && st()<0.1 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_2btag_2to4jets_st1_dphi3() const { return baseline() && njets()>1 && njets()<5 && nbjets()==2 && st()<0.1 && fabs(dphi_lepton_met())<2; }
  const bool baseline_2btag_2to4jets_st2_dphi1() const { return baseline() && njets()>1 && njets()<5 && nbjets()==2 && st()<0.2 && fabs(dphi_lepton_met())<1; }
  const bool baseline_2btag_2to4jets_st2_dphi2() const { return baseline() && njets()>1 && njets()<5 && nbjets()==2 && st()<0.2 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_2btag_2to4jets_st2_dphi3() const { return baseline() && njets()>1 && njets()<5 && nbjets()==2 && st()<0.2 && fabs(dphi_lepton_met())<2; }
  const bool baseline_2btag_2to4jets_st3_dphi1() const { return baseline() && njets()>1 && njets()<5 && nbjets()==2 && st()<0.4 && fabs(dphi_lepton_met())<1; }
  const bool baseline_2btag_2to4jets_st3_dphi2() const { return baseline() && njets()>1 && njets()<5 && nbjets()==2 && st()<0.4 && fabs(dphi_lepton_met())<(M_PI/2); }
  const bool baseline_2btag_2to4jets_st3_dphi3() const { return baseline() && njets()>1 && njets()<5 && nbjets()==2 && st()<0.4 && fabs(dphi_lepton_met())<2; }

  const bool metfit_CR_1btag() const { return metfit_CR() && njets()>1 && nbjets()==1; }
  const bool metfit_CR_2btag() const { return metfit_CR() && njets()>1 && nbjets()==2; }
  const bool wmt_met() const { return wmt() && metfit_CR() && njets()>1 ; }
  const bool wmt_met_1btag() const { return wmt_met() && nbjets()==1 && njets()>1 ; }
  const bool wmt_met_2btag() const { return wmt_met() && nbjets()==2 && njets()>1 ; }

  const bool baseline_nobtag() const { return baseline() && nbjets()==0; }
  const bool baseline_nobtag_jets() const { return baseline() && njets()>1 && nbjets()==0; }
  const bool baseline_nobtag_2jets() const { return baseline() && njets()==2 && nbjets()==0; }
  const bool baseline_nobtag_3jets() const { return baseline() && njets()==3 && nbjets()==0; }
  const bool baseline_nobtag_2to3jets() const { return baseline() && njets()>1 && njets()<4 && nbjets()==0; }
  const bool baseline_nobtag_2to4jets() const { return baseline() && njets()>1 && njets()<5 && nbjets()==0; }
  const bool qcd_fake() const { return met() ? mt()<20. && (mt()+met()->pt())<60. : false; }
  const bool qcd_fake_jets() const { return qcd_fake() && njets()>1; }
  const bool qcd_fake_0jet() const { return qcd_fake() && njets()==0; }
  const bool qcd_fake_1jet() const { return qcd_fake() && njets()==1; }
  const bool qcd_fake_2jets() const { return qcd_fake() && njets()==2; }
  const bool qcd_fake_3jets() const { return qcd_fake() && njets()==3; }
  const bool qcd_fake_4jets() const { return qcd_fake() && njets()==4; }
  const bool qcd_fake_5jets() const { return qcd_fake() && njets()==5; }
  const bool qcd_fake_6upjets() const { return qcd_fake() && njets()>=6; }
  const bool qcd_fake_2to3jets() const { return qcd_fake() && njets()>1 && njets()<4; }
  const bool qcd_fake_2to4jets() const { return qcd_fake() && njets()>1 && njets()<5; }
  const bool events_jets() const { return njets()>1; }
  const bool events_2jets() const { return njets()==2; }
  const bool events_3jets() const { return njets()==3; }
  const bool events_4jets() const { return njets()==4; }
  const bool events_5jets() const { return njets()==5; }
  const bool events_6upjets() const { return njets()>=6; }
  const bool events_2to3jets() const { return njets()>1 && njets()<4;}
  const bool events_2to4jets() const { return njets()>1 && njets()<5;}
  const bool events_btag() const { return nbjets()>0; }
  const bool events_btag_jets() const { return njets()>1 && nbjets()>0; }
  const bool events_btag_2jets() const { return njets()==2 && nbjets()>0; }
  const bool events_btag_3jets() const { return njets()==3 && nbjets()>0; }
  const bool events_btag_2to3jets() const { return njets()>1 && njets()<4 && nbjets()>0; }
  const bool events_btag_2to4jets() const { return njets()>1 && njets()<5 && nbjets()>0; }
  const bool events_2btag() const { return nbjets()>1; }
  const bool events_2btag_jets() const { return njets()>1 && nbjets()>1; }
  const bool events_2btag_2jets() const { return njets()==2 && nbjets()>1; }
  const bool events_2btag_3jets() const { return njets()==3 && nbjets()>1; }
  const bool events_2btag_2to3jets() const { return njets()>1 && njets()<4 && nbjets()>1; }
  const bool events_2btag_2to4jets() const { return njets()>1 && njets()<5 && nbjets()>1; }
  const bool events_nobtag() const { return nbjets()==0; }
  const bool events_nobtag_jets() const { return njets()>1 && nbjets()==0; }
  const bool events_nobtag_2jets() const { return njets()==2 && nbjets()==0; }
  const bool events_nobtag_3jets() const { return njets()==3 && nbjets()==0; }
  const bool events_nobtag_2to3jets() const { return njets()>1 && njets()<4 && nbjets()==0; }
  const bool events_nobtag_2to4jets() const { return njets()>1 && njets()<5 && nbjets()==0; }

  // new interface - uses pre-built w's
  template<typename iteratorLepT,typename iteratorMetT,typename iteratorJetT,typename iteratorVertexT>
  class Compose {
  private:
    iteratorLepT _begin_leptons, _end_leptons;
    iteratorMetT _begin_mets, _end_mets;
    iteratorJetT _begin_jets, _end_jets;
    iteratorJetT _begin_bjets, _end_bjets;
    iteratorVertexT _begin_vertices, _end_vertices;
  public:
    typedef boost::shared_ptr<const AnaWprimeCandidate> result_type;
    Compose( iteratorLepT begin_leptons , iteratorLepT end_leptons ,
             iteratorMetT begin_mets , iteratorMetT end_mets ,
             iteratorJetT begin_jets , iteratorJetT end_jets ,
	     iteratorJetT begin_bjets , iteratorJetT end_bjets ,
             iteratorVertexT begin_vertices , iteratorVertexT end_vertices )
      : _begin_leptons(begin_leptons), _end_leptons(end_leptons)
      , _begin_mets(begin_mets), _end_mets(end_mets)
      , _begin_jets(begin_jets), _end_jets(end_jets)
	, _begin_bjets(begin_bjets), _end_bjets(end_bjets)
      , _begin_vertices(begin_vertices), _end_vertices(end_vertices)
    {
      assert( std::distance( begin_mets , end_mets ) == 1 );
    }
    result_type operator()( const boost::shared_ptr<const AnaW>& w ) const {
      boost::shared_ptr<AnaWprimeCandidate> result( new AnaWprimeCandidate(w) ); assert( result );
      result->_event_leptons.assign( _begin_leptons , _end_leptons );
      result->_event_met.assign( _begin_mets , _end_mets );
      result->_event_jets.assign( _begin_jets , _end_jets );
      result->_event_bjets.assign( _begin_bjets , _end_bjets );
      result->_event_vertices.assign( _begin_vertices , _end_vertices );
      std::sort( result->_event_leptons.begin() , result->_event_leptons.end() , boost::bind( &AnaParticle::pt , _1 ) > boost::bind( &AnaParticle::pt , _2 ) );
      std::sort( result->_event_jets.begin() , result->_event_jets.end() , boost::bind( &AnaParticle::pt , _1 ) > boost::bind( &AnaParticle::pt , _2 ) );
      std::sort( result->_event_bjets.begin() , result->_event_bjets.end() , boost::bind( &AnaParticle::pt , _1 ) > boost::bind( &AnaParticle::pt , _2 ) );
      return result;
    }
  };


};

BOOST_CLASS_VERSION( AnaWprimeCandidate , 2 );

#endif // ANAWPRIMECANDIDATE_HPP
