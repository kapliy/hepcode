#ifndef ANAVLQCANDIDATE_HPP
#define ANAVLQCANDIDATE_HPP

// A wrapper calss for AnaCandidateEvent containing event wide info designed specifically for VLQ events

#include <cassert>
#include <vector>
#include <iterator>
#include <numeric>
#include <boost/shared_ptr.hpp>
#include <boost/foreach.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/version.hpp>
#include <boost/math/special_functions/fpclassify.hpp>
#include <TLorentzVector.h>
#include "TrigFTKAna/EventObjectColl.hpp"
#include "TrigFTKAna/AnaElectron.hpp"
#include "TrigFTKAna/AnaPhoton.hpp"
#include "TrigFTKAna/AnaMuon.hpp"
#include "TrigFTKAna/AnaJet.hpp"
#include "TrigFTKAna/AnaVertex.hpp"
#include "TrigFTKAna/AnaMET.hpp"
#include "TrigFTKAna/AnaTruthParticle.hpp"
#include "TrigFTKAna/AnaTrigger.hpp"


class
AnaVLQCandidate
{
public:
  typedef AnaParticle::ftype ftype;
  typedef EventObjectColl< boost::shared_ptr<const AnaParticle> >::const_iterator const_particle_iterator;
  typedef EventObjectColl< boost::shared_ptr<const AnaElectron> >::const_iterator const_electron_iterator;
  typedef EventObjectColl< boost::shared_ptr<const AnaMuon> >::const_iterator const_muon_iterator;
  typedef EventObjectColl< boost::shared_ptr<const AnaJet> >::const_iterator const_jet_iterator;
  typedef EventObjectColl< boost::shared_ptr<const AnaVertex> >::const_iterator const_vertex_iterator;
  typedef EventObjectColl< boost::shared_ptr<const AnaTruthParticle> >::const_iterator const_truth_iterator;
private:
  std::vector< boost::shared_ptr<const AnaParticle> > _ordered_leptons; // pt-ordered
  std::vector< boost::shared_ptr<const AnaElectron> > _ordered_electrons; // pt-ordered
  std::vector< boost::shared_ptr<const AnaMuon> > _ordered_muons; // pt-ordered
  std::vector< boost::shared_ptr<const AnaJet> > _ordered_jets; // pt-ordered
  std::vector< boost::shared_ptr<const AnaVertex> > _ordered_vtxs; // sumpt-ordered
  boost::shared_ptr<const AnaMET> _met;
  boost::shared_ptr<const AnaTrigger> _trigger;
  boost::shared_ptr<const AnaTruthParticle> _truth_vlq;
  boost::shared_ptr<const AnaTruthParticle> _truth_v;
  TLorentzVector _lv_vec_detawj;
  TLorentzVector _lv_vec_matched;
  TLorentzVector _lv_vec_small;
  TLorentzVector _lv_vec_large;
  TLorentzVector _lv_vec_detalv;
  TLorentzVector _v_vec_detawj;
  TLorentzVector _lvj_vec_detawj;
  TLorentzVector _lvj_vec_matched;
  TLorentzVector _lvj_vec_small;
  TLorentzVector _lvj_vec_large;
  TLorentzVector _lvj_vec_detalv;
  TLorentzVector _total_vec;
  TLorentzVector _jets_vec;
  TLorentzVector _lepmet_vec;
  TLorentzVector _farjet_vec;
  double _average_mu;
  unsigned int _njet_overlaps;
  unsigned long _event_number;
  unsigned long _run_number;
  bool _is_data;
  mutable bool _cached;
private:
  friend class boost::serialization::access;
  template<class archiveT>
  void serialize( archiveT& ar, const unsigned int version ) { return; }
  void _refresh_cache() const {
    if( _cached ) { return; }
    // nothing done here yet
    _cached = true;
  }
public:
  AnaVLQCandidate( const_electron_iterator begin_electrons , const_electron_iterator end_electrons ,
		   const_muon_iterator begin_muons , const_muon_iterator end_muons ,
		   const_jet_iterator begin_jets , const_jet_iterator end_jets ,
		   const_vertex_iterator begin_vtxs , const_vertex_iterator end_vtxs ,
		   const boost::shared_ptr<const AnaMET>& met ,
		   const boost::shared_ptr<const AnaTrigger>& trigger ,
		   const double& average_mu ,
		   const unsigned long& event_number ,
		   const unsigned long& run_number ,
		   const bool& is_data ,
		   const_truth_iterator begin_vlq_truth , const_truth_iterator end_vlq_truth ,
		   const_truth_iterator begin_v_truth , const_truth_iterator end_v_truth )
    : _lv_vec_detawj( 0. , 0. , 0. , 0. )
    , _lv_vec_matched( 0. , 0. , 0. , 0. )
    , _lv_vec_small( 0. , 0. , 0. , 0. )
    , _lv_vec_large( 0. , 0. , 0. , 0. )
    , _lv_vec_detalv( 0. , 0. , 0. , 0. )
    , _v_vec_detawj( 0. , 0. , 0. , 0. )
    , _lvj_vec_detawj( 0. , 0. , 0. , 0. )
    , _lvj_vec_matched( 0. , 0. , 0. , 0. )
    , _lvj_vec_small( 0. , 0. , 0. , 0. )
    , _lvj_vec_large( 0. , 0. , 0. , 0. )
    , _lvj_vec_detalv( 0. , 0. , 0. , 0. )
    , _total_vec( 0. , 0. , 0. , 0. )
    , _jets_vec( 0. , 0. , 0. , 0. )
    , _lepmet_vec( 0. , 0. , 0. , 0. )
    , _farjet_vec( 0. , 0. , 0. , 0. )
    , _event_number( event_number ) 
    , _run_number( run_number )
    , _is_data( is_data )
  {
    // fill ordered vectors
    std::copy( begin_electrons , end_electrons , back_inserter(_ordered_leptons) );
    std::copy( begin_muons , end_muons , back_inserter(_ordered_leptons) );
    std::sort( _ordered_leptons.begin() , _ordered_leptons.end() , bind(&AnaParticle::pt,_1) > bind(&AnaParticle::pt,_2) );
    std::copy( begin_electrons , end_electrons , back_inserter(_ordered_electrons) );
    std::sort( _ordered_electrons.begin() , _ordered_electrons.end() , bind(&AnaElectron::pt,_1) > bind(&AnaElectron::pt,_2) );
    std::copy( begin_muons , end_muons , back_inserter(_ordered_muons) );
    std::sort( _ordered_muons.begin() , _ordered_muons.end() , bind(&AnaMuon::pt,_1) > bind(&AnaMuon::pt,_2) );
    std::copy( begin_jets , end_jets , back_inserter(_ordered_jets) );
    std::sort( _ordered_jets.begin() , _ordered_jets.end() , bind(&AnaJet::pt,_1) > bind(&AnaJet::pt,_2) );
    std::copy( begin_vtxs , end_vtxs , back_inserter(_ordered_vtxs) );
    std::sort( _ordered_vtxs.begin() , _ordered_vtxs.end() , bind(&AnaVertex::sum_pt,_1) > bind(&AnaVertex::sum_pt,_2) );
    _met = met;
    _trigger = trigger;
    // truth particles
    if( begin_vlq_truth != end_vlq_truth ) _truth_vlq = *begin_vlq_truth;
    else _truth_vlq = boost::shared_ptr<const AnaTruthParticle>();
    if( begin_v_truth != end_v_truth ) _truth_v = *begin_v_truth;
    else _truth_v = boost::shared_ptr<const AnaTruthParticle>();
    // fill the TLorentzVectors
    assert( _ordered_leptons.size()==1 );
    _lepmet_vec += met->four_vector();
    _lepmet_vec += _ordered_leptons[0]->transverse_four_vector();
    _total_vec += met->four_vector();
    _total_vec += _ordered_leptons[0]->four_vector();
    BOOST_FOREACH( const boost::shared_ptr<const AnaJet>& p , _ordered_jets ) { 
      _total_vec += p->four_vector(); 
      _jets_vec += p->four_vector();
    }
    AnaParticle::ftype small_pz = detector::reconstructedMETpz( _ordered_leptons[0]->px() , _ordered_leptons[0]->py() , _ordered_leptons[0]->pz() , met->px() , met->py() , 0 );
    AnaParticle::ftype large_pz = detector::reconstructedMETpz( _ordered_leptons[0]->px() , _ordered_leptons[0]->py() , _ordered_leptons[0]->pz() , met->px() , met->py() , 1 );
    AnaParticle::ftype small_eta = detector::etaFromPxPyPz(met->px(),met->py(),small_pz);
    AnaParticle::ftype large_eta = detector::etaFromPxPyPz(met->px(),met->py(),large_pz);
    // here we use some tricky logic to fill the various W TLorentzVectors using different pz solutions
    // keep in mind that the order here is important!
    _lv_vec_small.SetPtEtaPhiM( met->pt() , small_eta , met->phi() , 0. );
    _lv_vec_large.SetPtEtaPhiM( met->pt() , large_eta , met->eta() , 0. );
    if( _truth_v ) {
      // do truth matching based on the dR between the truth and reco neutrinos
      if( detector::delta_eta_phi(_lv_vec_small.Eta(),_lv_vec_small.Phi(),double(_truth_v->eta()),double(_truth_v->phi())) < 
	  detector::delta_eta_phi(_lv_vec_large.Eta(),_lv_vec_large.Phi(),double(_truth_v->eta()),double(_truth_v->phi())) ) { _lv_vec_matched = _lv_vec_small; }
      else { _lv_vec_matched = _lv_vec_large; }
    } else {
      // if there is no truth neutrino provided, just call the smaller pz solution the "matched" solution
      _lv_vec_matched = _lv_vec_small;
    }
    _lv_vec_detalv.SetPtEtaPhiM( met->pt() , ( fabs(small_eta-_ordered_leptons[0]->eta()) < fabs(large_eta-_ordered_leptons[0]->eta()) ? small_eta : large_eta ) , met->phi() , 0. );
    _lv_vec_small += _ordered_leptons[0]->four_vector();
    _lv_vec_large += _ordered_leptons[0]->four_vector();
    _lv_vec_detalv += _ordered_leptons[0]->four_vector();
    _lv_vec_matched += _ordered_leptons[0]->four_vector();
    if( _ordered_jets.size()==0 ) {
      _lv_vec_detawj = _lv_vec_small;
      _v_vec_detawj.SetPtEtaPhiM( met->pt() , small_eta , met->phi() , 0. );
    } else {
      _lv_vec_detawj = fabs( _lv_vec_small.Eta() - double(_ordered_jets[0]->eta()) ) < fabs( _lv_vec_large.Eta() - double(_ordered_jets[0]->eta()) ) ? _lv_vec_small : _lv_vec_large;
      _v_vec_detawj.SetPtEtaPhiM( met->pt() , large_eta , met->phi() , 0. );
    }
    // vlq vectors
    _lvj_vec_small = _lv_vec_small;
    _lvj_vec_large = _lv_vec_large;
    _lvj_vec_detawj = _lv_vec_detawj;
    _lvj_vec_detalv = _lv_vec_detalv;
    _lvj_vec_matched = _lv_vec_matched;
    if( _ordered_jets.size() ) {
      _lvj_vec_small += _ordered_jets[0]->four_vector();
      _lvj_vec_large += _ordered_jets[0]->four_vector();
      _lvj_vec_detawj += _ordered_jets[0]->four_vector();
      _lvj_vec_detalv += _ordered_jets[0]->four_vector();
      _lvj_vec_matched += _ordered_jets[0]->four_vector();
    }
    // far jet vector
    if( _ordered_jets.size()==1 ) { _farjet_vec += _ordered_jets[0]->four_vector(); }
    else if( _ordered_jets.size() > 1 ) {
      AnaParticle::ftype max_deta = 0.;
      std::vector< boost::shared_ptr<const AnaJet> >::iterator ifar = _ordered_jets.begin()+1;
      for( std::vector< boost::shared_ptr<const AnaJet> >::iterator i = _ordered_jets.begin()+1 , f = _ordered_jets.end() ; i!=f ; ++i ) {
	if( fabs( (*i)->eta() - _ordered_jets[0]->eta() ) > max_deta ) {
	  max_deta = fabs( (*i)->eta() - _ordered_jets[0]->eta() );
	  ifar = i;
	}
      }
      _farjet_vec += (*ifar)->four_vector();
    }
    // pileup var
    _average_mu = average_mu;
  }

  virtual ~AnaVLQCandidate() {}

  // charge of lepton
  const short scharge() const { return lepton() ? lepton()->scharge() : 0; }

  // object counts
  const unsigned int nleptons() const { return _ordered_leptons.size(); }
  const unsigned int nelectrons() const { return _ordered_electrons.size(); }
  const unsigned int nmuons() const { return _ordered_muons.size(); }
  const unsigned int njets() const { return _ordered_jets.size(); }
  const unsigned int nvtxs() const { return _ordered_vtxs.size(); }
  const bool has_vlq_truth() const { return _truth_vlq ? true : false; }
  const bool has_v_truth() const { return _truth_v ? true : false; }

  // extra pileup vars, misc
  const double average_mu() const { return _average_mu; }

  // event info
  const unsigned long event_number() const { return _event_number; }
  const unsigned long run_number() const { return _run_number; }
  const bool is_data() const { return _is_data; }

  // return various particles, if they don't exist then return a NULL pointer
  const boost::shared_ptr<const AnaJet> jet( const short& i = 0 ) const { 
    if( i >= _ordered_jets.size() ) return boost::shared_ptr<const AnaJet>();
    return _ordered_jets[i];
  }
  const boost::shared_ptr<const AnaParticle> lepton( const short& i = 0 ) const {
    if( i >= _ordered_leptons.size() ) return boost::shared_ptr<const AnaParticle>();
    return _ordered_leptons[i];
  }
  const boost::shared_ptr<const AnaElectron> electron( const short& i = 0 ) const {
    if( i >= _ordered_electrons.size() ) return boost::shared_ptr<const AnaElectron>();
    return _ordered_electrons[i];
  }
  const boost::shared_ptr<const AnaMuon> muon( const short& i ) const {
    if( i >= _ordered_muons.size() ) return boost::shared_ptr<const AnaMuon>();
    return _ordered_muons[i];
  }
  const boost::shared_ptr<const AnaMET> met() const { return _met; }

  // MET variables
  const double met_pt() const { return _met ? double(_met->pt()) : 0.; }
  const double met_phi() const { return _met ? double(_met->phi()) : 0.; }
  // AnaParticle::ftype doesn't seem to work correctly with TLorentzVectors!!
  const double met_eta() const { return _v_vec_detawj.Eta(); }

  // lepton variables
  const double lepton_pt() const { return lepton() ? double(lepton()->pt()) : 0.; }
  const double lepton_phi() const { return lepton() ? double(lepton()->phi()) : 0.; }
  const double lepton_eta() const { return lepton() ? double(lepton()->eta()) : 0.; }
  const double lepton_m() const { return lepton() ? double(lepton()->m()) : 0.; }

  // return some jet vars
  const double firstjet_pt() const { return jet(0) ? double(jet(0)->pt()) : 0.; }
  const double firstjet_phi() const { return jet(0) ? double(jet(0)->phi()) : 0.; }
  const double firstjet_eta() const { return jet(0) ? double(jet(0)->eta()) : 0.; }
  // AnaParticle::ftype doesn't seem to work correctly with TLorentzVectors!!
  const double farjet_pt() const { return _farjet_vec.Pt(); }
  const double farjet_phi() const { return _farjet_vec.Phi(); }
  const double farjet_eta() const { return _farjet_vec.Eta(); }
  const double vecsum_jet_m() const { return _jets_vec.M(); }
  const double vecsum_jet_pt() const { return _jets_vec.Pt(); }
  const double vecsum_jet_phi() const { return _jets_vec.Phi(); }
  const double vecsum_jet_eta() const { return _jets_vec.Eta(); }

  // scalar sum variables
  const double scalarsum_jet_pt() const {
    double result = 0.;
    BOOST_FOREACH( boost::shared_ptr<const AnaJet> jet , _ordered_jets ) { result += jet->pt(); }
    return result;
  }
  const double scalarsum_jet_m() const {
    double result = 0.;
    BOOST_FOREACH( boost::shared_ptr<const AnaJet> jet , _ordered_jets ) { result += jet->m(); }
    return result;
  }
  const double scalarsum_visible_pt() const { return scalarsum_jet_pt() + lepton_pt(); }
  const double scalarsum_visible_m() const { return scalarsum_jet_m() + lepton_m(); }
  const double scalarsum_pt() const { return scalarsum_jet_pt() + lepton_pt() + double(met_pt()); }
    

  // some composite variables
  // AnaParticle::ftype doesn't seem to work correctly with TLorentzVectors!!
  const double w_pt() const { _lv_vec_detawj.Pt(); }
  const double w_phi() const { _lv_vec_detawj.Phi(); }
  const double w_m() const { _lv_vec_detawj.M(); }
  const double w_eta() const { _lv_vec_detawj.Eta(); }
  const double w_eta_matched() const { _lv_vec_matched.Eta(); }
  const double w_mt() const { _lepmet_vec.M(); }
  // VLQ masses
  const double vlq_m_detawj() const { return _lvj_vec_detawj.M(); }
  const double vlq_m_detalv() const { return _lvj_vec_detalv.M(); }
  const double vlq_m_matched() const { return _lvj_vec_matched.M(); }
  const double vlq_m_small() const { return _lvj_vec_small.M(); }
  const double vlq_m_large() const { return _lvj_vec_large.M(); }

  // truth info
  const double vlq_m_truth() const { return _truth_vlq ? _truth_vlq->m() : 0.; }

  // phi separation variables
  const double dphi_lepton_met() const { return detector::delta_phi( lepton_phi() , met_phi() ); }
  const double dphi_lepton_leadingjet() const { return detector::delta_phi( lepton_phi() , firstjet_phi() ); }
  const double dphi_met_leadingjet() const { return detector::delta_phi( met_phi() , firstjet_phi() ); }
  const double dphi_w_leadingjet() const { return detector::delta_phi( w_phi() , firstjet_phi() ); }
  const double dphi_leadingjet_farjet() const { return detector::delta_phi( firstjet_phi() , farjet_phi() ); }

  // eta separation variables
  const double deta_leadingjet_farjet() const { return firstjet_eta() - farjet_eta(); }
  const double deta_lepton_leadingjet() const { return lepton_eta() - firstjet_eta(); }
  const double deta_w_leadingjet() const { return w_eta() - firstjet_eta(); }
  const double deta_w_farjet() const { return w_eta() - farjet_eta(); }
  // for W with truth matched neutrino (for debugging)
  const double deta_w_matched_leadingjet() const { return w_eta_matched() - firstjet_eta(); }
  const double deta_w_matched_farjet() const { return w_eta_matched() - farjet_eta(); }

  // define some signal/control regions
  const unsigned int jetbin() const { return njets() < 3 ? njets() : 3; }
  const bool wmt() const {
    if( !_met ) return false;
    if( _ordered_jets.size() < 2 ) return false;
    if( w_mt() < 40. ) return false;
    return true;
  }
  const bool cr_base() const {
    // here are some common cuts
    // many are already included in the event loop
    if( !_met ) return false;
    if( _met->pt() < 25. ) return false;
    if( _ordered_jets.size() < 2 ) return false;
    if( w_mt() < 40. ) return false;
    return true;
  }
  const bool sr_base() const {
    if( !cr_base() ) return false;
    if( firstjet_pt() < 60 ) return false;
    if( met_pt() < 50 ) return false;
    if( fabs(w_eta()) > 2.5 ) return false;
    return true;
  }
  const bool qcd_cr() const { return met_pt() > 30 && met_pt() < 50 && cr_base(); }
  const bool wjets_cr() const { return met_pt() > 50 && dphi_lepton_met() > 2. && cr_base(); }
  const bool opt_400_sr() const {
    if( !sr_base() ) return false;
    if( fabs(deta_w_leadingjet()) > 2.3 ) return false;
    if( fabs(dphi_w_leadingjet()) < 2.1 ) return false;
    if( fabs(deta_w_farjet()) < 1.5 ) return false;
    if( fabs(deta_leadingjet_farjet()) < 1.6 ) return false;
    if( fabs(dphi_lepton_met()) > 1.3 ) return false;
    return true;
  }
  // negative W signal region
  const bool opt_400_sr_negative() const {
    if( !opt_400_sr() ) return false;
    if( !( scharge() < 0 ) ) return false;
    return true;
  }

  // test region for looking at vlq mass resolution in signal
  const bool opt_400_sr_matched() const {
    if( !cr_base() ) return false;
    if( firstjet_pt() < 60 ) return false;
    if( met_pt() < 50 ) return false;
    if( fabs(w_eta_matched()) > 2.5 ) return false;
    if( fabs(deta_w_matched_leadingjet()) > 2.3 ) return false;
    if( fabs(dphi_w_leadingjet()) < 2.1 ) return false;
    if( fabs(deta_w_matched_farjet()) < 1.5 ) return false;
    if( fabs(deta_leadingjet_farjet()) < 1.6 ) return false;
    if( fabs(dphi_lepton_met()) > 1.3 ) return false;
    return true;
  }

  // test region for looking at vlq mass resolution in signal -- putting no cuts on w eta
  const bool opt_400_sr_base() const {
    if( !cr_base() ) return false;
    if( firstjet_pt() < 60 ) return false;
    if( met_pt() < 50 ) return false;
    //if( fabs(w_eta_matched()) > 2.5 ) return false;
    //if( fabs(deta_w_matched_leadingjet()) > 2.3 ) return false;
    if( fabs(dphi_w_leadingjet()) < 2.1 ) return false;
    //if( fabs(deta_w_matched_farjet()) < 1.5 ) return false;
    if( fabs(deta_leadingjet_farjet()) < 1.6 ) return false;
    if( fabs(dphi_lepton_met()) > 1.3 ) return false;
    return true;
  }  

  const bool opt_400_cr() const {
    if( !sr_base() ) return false;
    if( fabs(deta_w_leadingjet()) > 2.3 ) return false;
    if( fabs(dphi_w_leadingjet()) < 2.1 ) return false;
    if( fabs(deta_w_farjet()) < 1.5 ) return false;
    if( fabs(deta_leadingjet_farjet()) < 1.6 ) return false;
    if( fabs(dphi_lepton_met()) < 1.3 ) return false;
    return true;
  }
  const bool opt_400_cr2() const {
    if( !sr_base() ) return false;
    if( fabs(deta_w_leadingjet()) < 2.3 ) return false;
    if( fabs(dphi_w_leadingjet()) < 2.1 ) return false;
    if( fabs(deta_w_farjet()) < 1.5 ) return false;
    if( fabs(deta_leadingjet_farjet()) < 1.6 ) return false;
    if( fabs(dphi_lepton_met()) > 1.3 ) return false;
    return true;
  }
  const bool opt_400_cr3() const {
    if( !sr_base() ) return false;
    if( fabs(deta_w_leadingjet()) > 2.3 ) return false;
    if( fabs(dphi_w_leadingjet()) < 2.1 ) return false;
    if( fabs(deta_w_farjet()) > 1.5 ) return false;
    if( fabs(deta_leadingjet_farjet()) < 1.6 ) return false;
    if( fabs(dphi_lepton_met()) > 1.3 ) return false;
    return true;
  }
  const bool opt_900_sr() const {
    if( !sr_base() ) return false;
    if( fabs(deta_w_leadingjet()) > 2.0 ) return false;
    if( fabs(dphi_w_leadingjet()) < 2.8 ) return false;
    if( fabs(deta_w_farjet()) < 1.8 ) return false;
    if( fabs(deta_leadingjet_farjet()) < 2.2 ) return false;
    if( fabs(dphi_lepton_met()) > 0.7 ) return false;
    return true;
  }
  const bool opt_900_cr() const {
    if( !sr_base() ) return false;
    if( fabs(deta_w_leadingjet()) > 2.0 ) return false;
    if( fabs(dphi_w_leadingjet()) < 2.8 ) return false;
    if( fabs(deta_w_farjet()) < 1.8 ) return false;
    if( fabs(deta_leadingjet_farjet()) < 2.2 ) return false;
    if( fabs(dphi_lepton_met()) < 0.7 ) return false;
    return true;
  }
  const bool opt_1200_sr() const {
    if( !sr_base() ) return false;
    if( fabs(deta_w_leadingjet()) > 2.5 ) return false;
    if( fabs(dphi_w_leadingjet()) < 2.4 ) return false;
    if( fabs(deta_w_farjet()) < 0.8 ) return false;
    if( fabs(deta_leadingjet_farjet()) < 1.4 ) return false;
    if( fabs(dphi_lepton_met()) > 0.7 ) return false;
    return true;
  }
  const bool opt_1200_cr() const {
    if( !sr_base() ) return false;
    if( fabs(deta_w_leadingjet()) > 2.5 ) return false;
    if( fabs(dphi_w_leadingjet()) < 2.4 ) return false;
    if( fabs(deta_w_farjet()) < 0.8 ) return false;
    if( fabs(deta_leadingjet_farjet()) < 1.4 ) return false;
    if( fabs(dphi_lepton_met()) < 0.7 ) return false;
    return true;
  }

};

BOOST_CLASS_VERSION( AnaVLQCandidate , 0 );

#endif // ANAMONOJETCANDIDATE_HPP
