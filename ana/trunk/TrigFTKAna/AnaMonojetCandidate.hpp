#ifndef ANAMONOJETCANDIDATE_HPP
#define ANAMONOJETCANDIDATE_HPP

// A wrapper calss for AnaCandidateEvent containing event wide info designed specifically for Monojet events

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
#include "TrigFTKAna/AnaTrigger.hpp"


class
AnaMonojetCandidate
{
public:
  typedef AnaParticle::ftype ftype;
  typedef EventObjectColl< boost::shared_ptr<const AnaParticle> >::const_iterator const_particle_iterator;
  typedef EventObjectColl< boost::shared_ptr<const AnaElectron> >::const_iterator const_electron_iterator;
  typedef EventObjectColl< boost::shared_ptr<const AnaMuon> >::const_iterator const_muon_iterator;
  typedef EventObjectColl< boost::shared_ptr<const AnaJet> >::const_iterator const_jet_iterator;
  typedef EventObjectColl< boost::shared_ptr<const AnaVertex> >::const_iterator const_vertex_iterator;
  typedef EventObjectColl< boost::shared_ptr<const AnaTruthParticle> >::const_iterator const_truth_iterator;
  typedef EventObjectColl< boost::shared_ptr<const AnaTruthParticle> >::iterator truth_iterator;
  typedef EventObjectColl< boost::shared_ptr<const AnaTrack> >::const_iterator const_track_iterator;
  typedef EventObjectColl< boost::shared_ptr<const AnaTrack> >::iterator track_iterator;
private:
  std::vector< boost::shared_ptr<const AnaParticle> > _ordered_loose_leptons; // pt-ordered
  std::vector< boost::shared_ptr<const AnaParticle> > _ordered_leptons; // pt-ordered
  std::vector< boost::shared_ptr<const AnaElectron> > _ordered_electrons; // pt-ordered
  std::vector< boost::shared_ptr<const AnaMuon> > _ordered_loose_muons; // pt-ordered
  std::vector< boost::shared_ptr<const AnaMuon> > _ordered_muons; // pt-ordered
  std::vector< boost::shared_ptr<const AnaVertex> > _ordered_vtxs; // sumpt-ordered
  std::vector< boost::shared_ptr<const AnaJet> > _ordered_lepjets; // pt-ordered
  std::vector< boost::shared_ptr<const AnaJet> > _ordered_jets; // pt-ordered
  std::vector< int > _ordered_jet_pdgids;
  boost::shared_ptr<const AnaMET> _met;
  boost::shared_ptr<const AnaMET> _met_reffinal;
  boost::shared_ptr<const AnaMET> _met_reffinalcorr;
  boost::shared_ptr<const AnaTrigger> _trigger;
  TLorentzVector _ll_vec;
  TLorentzVector _lv_vec;
  TLorentzVector _total_vec;
  TLorentzVector _jets_vec;
  TLorentzVector _lepmet_vec;
  double _average_mu;
  int _bdff;
  bool _pgs;
  unsigned long _run_number;
  double _run_lumi;
  unsigned int _njet_overlaps;
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
  AnaMonojetCandidate( const_electron_iterator begin_electrons , const_electron_iterator end_electrons ,
		       const_muon_iterator begin_loose_muons , const_muon_iterator end_loose_muons ,
		       const_muon_iterator begin_tight_muons , const_muon_iterator end_tight_muons ,
		       const_jet_iterator begin_jets , const_jet_iterator end_jets ,
		       const_vertex_iterator begin_vtxs , const_vertex_iterator end_vtxs ,
		       const_track_iterator begin_tracks , const_track_iterator end_tracks ,
		       const_truth_iterator begin_truth , const_truth_iterator end_truth ,
		       const boost::shared_ptr<const AnaMET>& met_lochadtopo ,
		       const boost::shared_ptr<const AnaMET>& met_reffinal ,
		       const boost::shared_ptr<const AnaMET>& met_reffinalcorr ,
		       const boost::shared_ptr<const AnaTrigger>& trigger ,
		       const double& average_mu ,
		       const int& bdff ,
		       const bool _pgs ,
		       const unsigned long& run_number ,
		       const double& run_lumi )
    : _ll_vec( 0. , 0. , 0. , 0. )
    , _lv_vec( 0. , 0. , 0. , 0. )
    , _total_vec( 0. , 0. , 0. , 0. )
    , _jets_vec( 0. , 0. , 0. , 0. )
    , _lepmet_vec( 0. , 0. , 0. , 0. )
  {
    // fill ordered vectors
    std::copy( begin_electrons , end_electrons , back_inserter(_ordered_loose_leptons) );
    std::copy( begin_loose_muons , end_loose_muons , back_inserter(_ordered_loose_leptons) );
    std::sort( _ordered_loose_leptons.begin() , _ordered_loose_leptons.end() , bind(&AnaParticle::pt,_1) > bind(&AnaParticle::pt,_2) );
    std::copy( begin_electrons , end_electrons , back_inserter(_ordered_leptons) );
    std::copy( begin_tight_muons , end_tight_muons , back_inserter(_ordered_leptons) );
    std::sort( _ordered_leptons.begin() , _ordered_leptons.end() , bind(&AnaParticle::pt,_1) > bind(&AnaParticle::pt,_2) );    
    std::copy( begin_electrons , end_electrons , back_inserter(_ordered_electrons) );
    std::sort( _ordered_electrons.begin() , _ordered_electrons.end() , bind(&AnaElectron::pt,_1) > bind(&AnaElectron::pt,_2) );
    std::copy( begin_loose_muons , end_loose_muons , back_inserter(_ordered_loose_muons) );
    std::sort( _ordered_loose_muons.begin() , _ordered_loose_muons.end() , bind(&AnaMuon::pt,_1) > bind(&AnaMuon::pt,_2) );
    std::copy( begin_tight_muons , end_tight_muons , back_inserter(_ordered_muons) );
    std::sort( _ordered_muons.begin() , _ordered_muons.end() , bind(&AnaMuon::pt,_1) > bind(&AnaMuon::pt,_2) );
    std::copy( begin_jets , end_jets , back_inserter(_ordered_lepjets) );
    std::sort( _ordered_lepjets.begin() , _ordered_lepjets.end() , bind(&AnaJet::pt,_1) > bind(&AnaJet::pt,_2) );
    std::copy( begin_vtxs , end_vtxs , back_inserter(_ordered_vtxs) );
    std::sort( _ordered_vtxs.begin() , _ordered_vtxs.end() , bind(&AnaVertex::sum_pt,_1) > bind(&AnaVertex::sum_pt,_2) );
    _met = met_lochadtopo;
    _met_reffinal = met_reffinal;
    _met_reffinalcorr = met_reffinalcorr;
    _trigger = trigger;
    // fill a second jet vector and perform overlap removal
    std::copy( begin_jets , end_jets , back_inserter(_ordered_jets) );
    _njet_overlaps = 0;
    BOOST_FOREACH( boost::shared_ptr<const AnaElectron> el , _ordered_electrons ) {
      float drmin = std::numeric_limits<float>::max();
      std::vector< boost::shared_ptr<const AnaJet> >::iterator imin = _ordered_jets.begin();
      for( std::vector< boost::shared_ptr<const AnaJet> >::iterator i = _ordered_jets.begin() , f = _ordered_jets.end(); i!=f;  ++i ) {
	float thisdr = float(detector::delta_eta_phi( el->cluster_eta() , el->cluster_phi() , (*i)->em_scale_eta() , (*i)->em_scale_phi() ));
	if( thisdr < drmin ) {
	  drmin = thisdr;
	  imin = i;
	}
      }
      if( drmin < 0.3 ) {
	_ordered_jets.erase(imin);
	_njet_overlaps++;
      }
    }
    BOOST_FOREACH( boost::shared_ptr<const AnaMuon> mu , _ordered_muons ) {
      float drmin = std::numeric_limits<float>::max();
      std::vector< boost::shared_ptr<const AnaJet> >::iterator imin = _ordered_jets.begin();
      for( std::vector< boost::shared_ptr<const AnaJet> >::iterator i = _ordered_jets.begin() , f = _ordered_jets.end(); i!=f;  ++i ) {
	float thisdr = float(detector::delta_eta_phi( mu->id_eta() , mu->id_phi() , (*i)->em_scale_eta() , (*i)->em_scale_phi() ));
	if( thisdr < drmin ) {
	  drmin = thisdr;
	  imin = i;
	}
      }
      if( drmin < 0.3 ) {
	_ordered_jets.erase(imin);
	_njet_overlaps++;
      }
    }
    std::sort( _ordered_jets.begin() , _ordered_jets.end() , bind(&AnaJet::pt,_1) > bind(&AnaJet::pt,_2) );
    // fill composite objects
    if( _ordered_leptons.size()==1 ) { _lv_vec += _ordered_leptons[0]->transverse_four_vector(); _lv_vec += met_lochadtopo->transverse_four_vector(); }
    if( _ordered_leptons.size()==2 ) { _ll_vec += _ordered_leptons[0]->four_vector(); _ll_vec += _ordered_leptons[1]->four_vector(); }
    _lepmet_vec += met_lochadtopo->four_vector();
    BOOST_FOREACH( const boost::shared_ptr<const AnaParticle>& p , _ordered_leptons ) { 
      _total_vec += p->four_vector(); 
      _lepmet_vec += p->transverse_four_vector();
    }
    BOOST_FOREACH( const boost::shared_ptr<const AnaJet>& p , _ordered_jets ) { 
      _total_vec += p->four_vector(); 
      _jets_vec += p->four_vector();
    }
    // pileup var
    _average_mu = average_mu;
    _bdff = bdff;
    _run_number = run_number;
    _run_lumi = run_lumi;
    // do some additional jet studies
    // do truth q/g tagging
    BOOST_FOREACH( boost::shared_ptr<const AnaJet> j , _ordered_jets ) {
      float jet_eta = j->eta(); // maybe use EM scale?
      float jet_phi = j->phi();
      double max_truth_pt = 1.; // 1 instead of 0 to avoid the stupid root warning when four_vector pT = 0
      int truth_pdg = 0;
      double track_width_num = 0.;
      double track_width_den = 0.;
      // loop over truth particles to find the pdgid of the leading parton in the jet
      for( const_truth_iterator it = begin_truth ; it != end_truth ; ++it ) {
	if( (*it)->pt() <= max_truth_pt ) continue;
	if( detector::delta_eta_phi( jet_eta , jet_phi , (*it)->eta() , (*it)->phi() ) > 0.4 ) continue;
	max_truth_pt = (*it)->pt();
	truth_pdg = (*it)->pdg_id();
      }
      _ordered_jet_pdgids.push_back( truth_pdg );
      // loop over tracks to find track width
      /*
      for( const_track_iterator it = begin_tracks ; it != end_tracks ; ++it ) {
	float tmp_dr = detector::delta_eta_phi( jet_eta , jet_phi , (*it)->eta() , (*it)->phi() );
	if( tmp_dr > 0.4 ) continue;
	track_width_num += double(tmp_dr) * double((*it)->pt());
	track_width_den += double((*it)->pt());
      }
      _ordered_jet_trackwidths.push_back( track_width_num > 0. ? detector::safe_divide(track_width_num,track_width_den) : 0. );
      */
    }
    assert( _ordered_jets.size() == _ordered_jet_pdgids.size() );
  }

  virtual ~AnaMonojetCandidate() {}

  // object counts
  const unsigned int nlooseleptons() const { return _ordered_loose_leptons.size(); }
  const unsigned int nleptons() const { return _ordered_leptons.size(); }
  const unsigned int nelectrons() const { return _ordered_electrons.size(); }
  const unsigned int nloosemuons() const { return _ordered_loose_muons.size(); }
  const unsigned int nmuons() const { return _ordered_muons.size(); }
  const unsigned int njets() const { return _ordered_jets.size(); }
  const unsigned int nlepjets() const { return _ordered_lepjets.size(); }
  const unsigned int njetoverlaps() const { return _njet_overlaps; }
  const unsigned int nvtxs() const { return _ordered_vtxs.size(); }

  // event info vars
  const double average_mu() const { return _average_mu; }
  const int bdff() const { return _bdff; }
  const unsigned long run_number() const { return _run_number; }
  const double run_lumi() const { return _run_lumi; }

  // leading jet handles
  const AnaParticle::ftype leading_jet_emf() const { return _ordered_jets.size() ? _ordered_jets[0]->emf() : 0.; }
  const AnaParticle::ftype leading_jet_chf() const { return _ordered_jets.size() ? _ordered_jets[0]->chf() : 0.; }
  
  const int jet_partonid( const short& i ) const {
    if( i >= _ordered_jet_pdgids.size() ) return 0;
    return _ordered_jet_pdgids[i];
  }
  const int leading_jet_partonid() const { return jet_partonid(0); }
  const bool leading_jet_is_quark() const { return abs(leading_jet_partonid()) < 9 && abs(leading_jet_partonid()) > 0; }
  const bool leading_jet_is_gluon() const { return leading_jet_partonid() == 21; }
  const bool leading_jet_is_untagged() const { return leading_jet_partonid() == 0; }



  // return various particles, if they don't exist then return a NULL pointer
  const boost::shared_ptr<const AnaJet> jet( const short& i ) const { 
    if( i >= _ordered_jets.size() ) return boost::shared_ptr<const AnaJet>();
    return _ordered_jets[i];
  }
  const boost::shared_ptr<const AnaParticle> loose_lepton( const short& i ) const {
    if( i >= _ordered_loose_leptons.size() ) return boost::shared_ptr<const AnaParticle>();
    return _ordered_loose_leptons[i];
  }
  const boost::shared_ptr<const AnaParticle> lepton( const short& i ) const {
    if( i >= _ordered_leptons.size() ) return boost::shared_ptr<const AnaParticle>();
    return _ordered_leptons[i];
  }
  const boost::shared_ptr<const AnaElectron> electron( const short& i ) const {
    if( i >= _ordered_electrons.size() ) return boost::shared_ptr<const AnaElectron>();
    return _ordered_electrons[i];
  }
  const boost::shared_ptr<const AnaMuon> loose_muon( const short& i ) const {
    if( i >= _ordered_loose_muons.size() ) return boost::shared_ptr<const AnaMuon>();
    return _ordered_loose_muons[i];
  }
  const boost::shared_ptr<const AnaMuon> muon( const short& i ) const {
    if( i >= _ordered_muons.size() ) return boost::shared_ptr<const AnaMuon>();
    return _ordered_muons[i];
  }
  const boost::shared_ptr<const AnaMET> met() const { return _met; }
  const boost::shared_ptr<const AnaMET> met_lochadtopo() const { return _met; }
  const boost::shared_ptr<const AnaMET> met_reffinal() const { return _met_reffinal; }
  const boost::shared_ptr<const AnaMET> met_reffinalcorr() const { return _met_reffinalcorr; }

  // MET variables
  const AnaParticle::ftype met_pt() const { return _met ? _met->pt() : 0.; }
  const AnaParticle::ftype met_phi() const { return _met ? _met->phi() : 0.; }
  const AnaParticle::ftype met_reffinal_pt() const { return _met_reffinal ? _met_reffinal->pt() : 0.; }
  const AnaParticle::ftype met_reffinal_phi() const { return _met_reffinal ? _met_reffinal->phi() : 0.; }
  const AnaParticle::ftype met_reffinalcorr_pt() const { return _met_reffinalcorr ? _met_reffinalcorr->pt() : 0.; }
  const AnaParticle::ftype met_reffinalcorr_phi() const { return _met_reffinalcorr ? _met_reffinalcorr->phi() : 0.; }  
  // AnaParticle::ftype doesn't seem to work correctly with TLorentzVectors!!
  const double lepmet_pt() const { return _lepmet_vec.Pt(); }
  const double lepmet_phi() const { return _lepmet_vec.Phi(); }

  // lepton variables
  const AnaParticle::ftype leading_electron_pt() const { return _ordered_electrons.size() ? _ordered_electrons[0]->pt() : 0.; }
  const AnaParticle::ftype leading_electron_phi() const { return _ordered_electrons.size() ? _ordered_electrons[0]->phi() : 0.; }
  const AnaParticle::ftype leading_electron_eta() const { return _ordered_electrons.size() ? _ordered_electrons[0]->eta() : 0.; }
  const AnaParticle::ftype leading_muon_pt() const { return _ordered_muons.size() ? _ordered_muons[0]->pt() : 0.; }
  const AnaParticle::ftype leading_muon_phi() const { return _ordered_muons.size() ? _ordered_muons[0]->phi() : 0.; }
  const AnaParticle::ftype leading_muon_eta() const { return _ordered_muons.size() ? _ordered_muons[0]->eta() : 0.; }
  const bool electrons_ossf() const { 
    if( _ordered_electrons.size()!=2 ) return false;
    if( _ordered_electrons[0]->charge() == _ordered_electrons[1]->charge() ) return false;
    return true; 
  }
  const bool muons_ossf() const {
    if( _ordered_muons.size()!=2 ) return false;
    if( _ordered_muons[0]->scharge() == _ordered_muons[1]->scharge() ) return false;
    return true;
  }
  const bool leptons_ossf() const { nleptons()==2 ? electrons_ossf() || muons_ossf() : false; }
  const AnaParticle::ftype min_electron_pt() const { return _ordered_electrons.size() ? _ordered_electrons.back()->pt() : 0.; }
  const AnaParticle::ftype min_muon_pt() const { return _ordered_muons.size() ? _ordered_muons.back()->pt() : 0.; }

  // return some jet vars
  const AnaParticle::ftype first_jet_pt() const { return jet(0) ? jet(0)->pt() : 0.; }
  const AnaParticle::ftype second_jet_pt() const { return jet(1) ? jet(1)->pt() : 0.; }
  const AnaParticle::ftype third_jet_pt() const { return jet(2) ? jet(2)->pt() : 0.; }
  // AnaParticle::ftype doesn't seem to work correctly with TLorentzVectors!!
  const double vecsum_jet_mass() const { return _jets_vec.M(); }
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
  const double scalarsum_lepton_pt() const {
    double result = 0.;
    BOOST_FOREACH( boost::shared_ptr<const AnaParticle> lep , _ordered_leptons ) { result += lep->pt(); }
    return result;
  }
  const double scalarsum_lepton_m() const {
    double result = 0.;
    BOOST_FOREACH( boost::shared_ptr<const AnaParticle> lep , _ordered_leptons ) { result += lep->m(); }
    return result;
  }
  const double scalarsum_visible_pt() const { return scalarsum_jet_pt() + scalarsum_lepton_pt(); }
  const double scalarsum_visible_m() const { return scalarsum_jet_m() + scalarsum_lepton_m(); }
  const double scalarsum_pt() const { return scalarsum_jet_pt() + scalarsum_lepton_pt() + double(met_pt()); }
    

  // some composite variables
  const AnaParticle::ftype leading_lepton_pt() const { return _ordered_leptons.size() ? _ordered_leptons[0]->pt() : 0.; }
  const AnaParticle::ftype leading_lepton_phi() const { return _ordered_leptons.size() ? _ordered_leptons[0]->phi() : 0.; }
  const AnaParticle::ftype leading_lepton_eta() const { return _ordered_leptons.size() ? _ordered_leptons[0]->eta() : 0.; }
  // AnaParticle::ftype doesn't seem to work correctly with TLorentzVectors!!
  const double w_pt() const { nleptons()==1 ? _lv_vec.Pt() : 0.; }
  const double w_phi() const { nleptons()==1 ? _lv_vec.Phi() : 0.; }
  const double w_mt() const { nleptons()==1 ? _lv_vec.M() : 0.; }
  const double z_pt() const { nleptons()==2 ? _ll_vec.Pt() : 0.; }
  const double z_phi() const { nleptons()==2 ? _ll_vec.Phi() : 0.; }
  const double z_eta() const { nleptons()==2 ? _ll_vec.Eta() : 0.; }
  const double z_m() const { nleptons()==2 ? _ll_vec.M() : 0.; }

  // some random variables for cutting
  const AnaParticle::ftype dphi_met_second_jet() const {
    if( !jet(1) ) return std::numeric_limits<AnaParticle::ftype>::max();
    if( !_met ) return std::numeric_limits<AnaParticle::ftype>::max();
    return detector::delta_phi( _met->phi() , jet(1)->phi() );
  }

  // define some control regions
  const bool mu_trigger() const { _trigger ? (_trigger->ef_mu24i_tight() || _trigger->ef_mu36_tight()) : false; }
  const bool el_trigger() const { return true; } // FIXME
  const bool met_trigger() const { return _trigger ? _trigger->ef_xe80_tclcw() : false; }
  const bool cr_common() const {
    if( !_met ) return false;
    //if( _met->pt() < 25. ) return false;
    if( _ordered_jets.size()==0 ) return false;
    if( _ordered_jets.size() > 1 && fabs(dphi_met_second_jet()) <= 0.3 ) return false;
    if( _ordered_jets.size() > 2 && third_jet_pt() > 30. ) return false;
    //bp next to temp removed
    //if( leading_jet_emf() < 0.1 ) return false;
    //if( leading_jet_chf() < 0.1 ) return false;
    if( _ordered_jets[0]->pt() < 60. ) return false;
    return true;
  }
  const bool z_mass_cut() const { return z_m() > 66. && z_m() < 116.; }
  const bool w_mass_cut() const { return w_mt() > 40.; }
  const bool cr_wenu() const { 
    return cr_common() && _met->pt() > 25. && _ordered_muons.size()==0 && _ordered_electrons.size()==1 && leading_electron_pt() > 25. && w_mass_cut() && el_trigger(); 
  }
  const bool cr_wmunu() const { 
    return cr_common() && _met->pt() > 25. && _ordered_muons.size()==1 && _ordered_electrons.size()==0 && leading_muon_pt() > 25. && w_mass_cut() && mu_trigger(); 
  }
  const bool cr_w() const { return cr_wenu() || cr_wmunu(); }
  const bool cr_zee() const { 
    return cr_common() && _ordered_muons.size()==0 && _ordered_electrons.size()==2 && electrons_ossf() && min_electron_pt() > 25. && z_mass_cut() && el_trigger(); 
  }
  const bool cr_zmumu() const { 
    return cr_common() && _ordered_muons.size()==2 && _ordered_electrons.size()==0 && muons_ossf() && min_muon_pt() > 25. && z_mass_cut() && mu_trigger(); 
  }
  const bool cr_z() const { return cr_zee() || cr_zmumu(); }

  // some high met control regions
  const bool cr_common_highmet() const { return cr_common() && met_trigger() && _met->pt() > 120. && first_jet_pt() > 120.; }
  const bool cr_wmunu_highmet() const { return cr_wmunu() && met_trigger() && _met->pt() > 120. && first_jet_pt() > 120.; }
  const bool cr_zmumu_highmet() const { return cr_zmumu() && met_trigger() && _met->pt() > 120. && first_jet_pt() > 120.; }


  // define some signal regions
  const bool sr_presel0() const { 
    //BP for PGS
    if( !_pgs && !met_trigger() ) return false;
    if( !_met ) return false;
    if( _met->pt() < 25. ) return false;
    if( _ordered_electrons.size() ) return false;
    if( _ordered_muons.size() ) return false;
    if( _ordered_jets.size() == 0 ) return false;
    return true;
  }
  const bool sr_presel1() const {
    if( !sr_presel0() ) return false;
    if( _ordered_jets.size() > 1 && fabs(dphi_met_second_jet()) <= 0.3 ) return false;
    if( _ordered_jets.size() > 2 && third_jet_pt() > 30. ) return false;
    return true;
  }
  const bool sr_presel2() const {
    if( !sr_presel1() ) return false;
    if( leading_jet_emf() < 0.1 ) return false;
    if( !_pgs && leading_jet_chf() < 0.1 ) return false;
    return true;
  }
  const bool sr_common() const { return sr_presel2(); }
  const bool sr_120() const { return sr_common() && met_pt() > 120. && first_jet_pt() > 120.; }
  const bool sr_220() const { return sr_common() && met_pt() > 220. && first_jet_pt() > 220.; }
  const bool sr_350() const { return sr_common() && met_pt() > 350. && first_jet_pt() > 350.; }
  const bool sr_500() const { return sr_common() && met_pt() > 500. && first_jet_pt() > 500.; }
  const bool sr_800() const { return sr_common() && met_pt() > 800. && first_jet_pt() > 800.; }

  // regions for trigger efficiency plots
  const bool tr_mu0() const {
    if( !_met ) return false;
    if( _ordered_electrons.size() ) return false;
    if( _ordered_loose_muons.size() ) return false;
    if( _ordered_jets.size() == 0 ) return false;
    //if( _ordered_jets.size() > 1 && fabs(dphi_met_second_jet()) <= 0.3 ) return false;
    //if( _ordered_jets.size() > 2 && third_jet_pt() > 30. ) return false;
    return true;
  }
  const bool tr_mu1() const {
    if( !_met ) return false;
    if( !mu_trigger() ) return false;
    if( _ordered_jets.size()==0 ) return false;
    if( _ordered_electrons.size()!=0 ) return false;
    if( _ordered_muons.size()!=1 ) return false;
    return true;
  }
  const bool tr_mu2() const {
    return false;
  }

};

BOOST_CLASS_VERSION( AnaMonojetCandidate , 0 );

#endif // ANAMONOJETCANDIDATE_HPP
