
#ifndef ANAZCROSSSECTIONCANDIDATE_HPP
#define ANAZCROSSSECTIONCANDIDATE_HPP


// A wrapper class around AnaZ that contains additional event-wide information
// relevant for x-section and ratio analyses (e.g., jets)

#include <cassert>
#include <vector>
#include <iterator>
#include <numeric>
#include <boost/shared_ptr.hpp>
#include <boost/foreach.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/version.hpp>
#include "TrigFTKAna/AnaZ.hpp"
#include "TrigFTKAna/AnaElectron.hpp"
#include "TrigFTKAna/AnaMuon.hpp"
#include "TrigFTKAna/AnaJet.hpp"
#include "TrigFTKAna/AnaMET.hpp"
#include "TrigFTKAna/AnaVertex.hpp"


class
AnaZCrossSectionCandidate : public AnaZ
{
public:
  typedef AnaParticle::ftype ftype;
  typedef std::vector< boost::shared_ptr<const AnaJet> > JetColl;
  typedef JetColl::const_iterator jet_const_iterator;
private:
  std::vector< boost::shared_ptr<const AnaParticle> > _event_leptons; // pt-ordered
  std::vector< boost::shared_ptr<const AnaJet> > _event_jets; // pt-ordered
  std::vector< boost::shared_ptr<const AnaMET> > _event_met;
  std::vector< boost::shared_ptr<const AnaVertex> > _event_vertices; // after ntrack/pt cuts
  std::vector< boost::shared_ptr<const AnaVertex> > _event_all_vertices; // all primary vertices
  std::vector< boost::shared_ptr<const AnaTruthParticle> > _event_vlq_truth; // unsorted
  std::vector< boost::shared_ptr<const AnaTruthParticle> > _event_fj_truth; // unsorted
  mutable bool _cached;
private:
  friend class boost::serialization::access;
  template<class archiveT>
  void serialize( archiveT& ar, const unsigned int version ) {
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP( AnaZ );
    ar & boost::serialization::make_nvp("event_leptons",_event_leptons);
    ar & boost::serialization::make_nvp("event_jets",_event_jets);
    ar & boost::serialization::make_nvp("event_met",_event_met);
    ar & boost::serialization::make_nvp("event_vertices",_event_vertices);
    ar & boost::serialization::make_nvp("event_all_vertices",_event_all_vertices);
    ar & boost::serialization::make_nvp("event_vlq_truth",_event_vlq_truth);
    ar & boost::serialization::make_nvp("event_fj_truth",_event_fj_truth);
  }
  void _refresh_cache() const {
    if( _cached ) { return; }
    // nothing done here yet
    _cached = true;
  }
public:

  AnaZCrossSectionCandidate()
    : _event_leptons()
    , _event_jets()
    , _event_met()
    , _event_vertices()
    , _event_all_vertices()
    , _event_vlq_truth()
    , _event_fj_truth()
    , AnaZ()
  {
  }
  AnaZCrossSectionCandidate(const boost::shared_ptr<const AnaZ>& z)
    : _event_leptons()
    , _event_jets()
    , _event_met()
    , _event_vertices()
    , _event_all_vertices()
    , _event_vlq_truth()
    , _event_fj_truth()
    , _cached(false)
    , AnaZ(*z)
  {
    _refresh_cache();
  }

  virtual ~AnaZCrossSectionCandidate() {}

  // event-wide quantities
  const unsigned int nleptons() const { return _event_leptons.size(); }
  const unsigned int nleptons25() const{
    return std::count_if( _event_leptons.begin(),_event_leptons.end(),bind(&AnaParticle::pt,_1)>25.0);
  }
  const unsigned int ntruth() const { return _event_vlq_truth.size() + _event_fj_truth.size(); }
  const unsigned int nvlqtruth() const { return _event_vlq_truth.size(); }
  const unsigned int nfjtruth() const { return _event_fj_truth.size(); }
  const bool exactly_one_lepton() const { return _event_leptons.size()==1; }
  const bool exactly_two_leptons() const { return _event_leptons.size()==2; }
  const bool atleast_two_leptons() const { return _event_leptons.size()>=2; }
  const bool atmost_two_leptons() const { return _event_leptons.size()<=2; }
  const bool exactly_two_opposite_leptons() const { 
    return _event_leptons.size()==2 && _event_leptons[0]->scharge() != _event_leptons[1]->scharge(); 
  }
  const bool exactly_two_samesign_leptons() const { 
    return _event_leptons.size()==2 && _event_leptons[0]->scharge() == _event_leptons[1]->scharge();
  }
  const unsigned int nmets() const { return _event_met.size(); }
  const bool exactly_one_met() const { return _event_met.size()==1; }
  const boost::shared_ptr<const AnaMET> met() const { return( !_event_met.empty() ? _event_met.front() : boost::shared_ptr<const AnaMET>() ); }
  const unsigned int nvtxs() const { return _event_vertices.size(); }
  const unsigned int nvtxs_all() const { return _event_all_vertices.size(); }
  const unsigned int njets() const { return _event_jets.size(); }
  const unsigned int jetbin() const { return njets()<4 ? njets() : 4; }
  const unsigned int vtxbin() const { return nvtxs()<10 ? nvtxs() : 10; }
  jet_const_iterator begin_jets() const { return _event_jets.begin(); }
  jet_const_iterator end_jets() const { return _event_jets.end(); }
  const bool no_btag_event_jet() const {
    return( !(std::find_if( _event_jets.begin() , _event_jets.end() , boost::bind( &AnaJet::tag_sv0_w , _1 ) >= 5.72 ) != _event_jets.end() ) );
  }
  const boost::shared_ptr<const AnaJet> leading_jet() const {
    static boost::shared_ptr<const AnaJet> null;
    std::vector< boost::shared_ptr<const AnaJet> > jets_by_pt = _event_jets;
    std::sort( jets_by_pt.begin() , jets_by_pt.end() , bind(&AnaJet::pt,_1) > bind(&AnaJet::pt,_2) );
    return( !jets_by_pt.empty() ? (*jets_by_pt.begin()) : null );
  }
  const boost::shared_ptr<const AnaJet> subleading_jet() const {
    static boost::shared_ptr<const AnaJet> null;
    std::vector< boost::shared_ptr<const AnaJet> > jets_by_pt = _event_jets;
    std::sort( jets_by_pt.begin() , jets_by_pt.end() , bind(&AnaJet::pt,_1) > bind(&AnaJet::pt,_2) );
    const boost::shared_ptr<const AnaJet>& secondjet( *(++(jets_by_pt.begin())) );
    return( !jets_by_pt.empty() ? secondjet : null );
  }
  const boost::shared_ptr<const AnaJet> subsubleading_jet() const {
    static boost::shared_ptr<const AnaJet> null;
    if( njets()<=2 ) return null;
    std::vector< boost::shared_ptr<const AnaJet> > jets_by_pt = _event_jets;
    std::sort( jets_by_pt.begin() , jets_by_pt.end() , bind(&AnaJet::pt,_1) > bind(&AnaJet::pt,_2) );
    return *( jets_by_pt.begin() + 2 );
  }
  const boost::shared_ptr<const AnaJet> associated_jet() const {
    if( _event_jets.size() < 2 ) return boost::shared_ptr<const AnaJet>();
    if( _event_jets.size() < 3 ) return subleading_jet();
    AnaParticle::ftype leading_eta = leading_jet()->eta();
    AnaParticle::ftype subleading_eta = subleading_jet()->eta();
    AnaParticle::ftype subsubleading_eta = subsubleading_jet()->eta();
    if( fabs(leading_eta - subleading_eta) > fabs(leading_eta-subsubleading_eta) ) return subleading_jet();
    return subsubleading_jet();
  }

  // electron and muon objects
  const boost::shared_ptr<const AnaElectron>& electron1() const {
    const boost::shared_ptr<const AnaElectron> tmp;
    const boost::shared_ptr<const AnaElectron> electron = boost::dynamic_pointer_cast<const AnaElectron,const AnaParticle>( lepton1() );
    return ( electron ? electron : tmp );
  }
  const boost::shared_ptr<const AnaElectron>& electron2() const {
    const boost::shared_ptr<const AnaElectron> tmp;
    const boost::shared_ptr<const AnaElectron> electron = boost::dynamic_pointer_cast<const AnaElectron,const AnaParticle>( lepton2() );
    return ( electron ? electron : tmp );
  }
  const boost::shared_ptr<const AnaMuon>& muon1() const {
    static boost::shared_ptr<const AnaMuon> tmp;
    const boost::shared_ptr<const AnaMuon> muon = boost::dynamic_pointer_cast<const AnaMuon,const AnaParticle>( lepton1() );
    return ( muon ? muon : tmp );
  }
  const boost::shared_ptr<const AnaMuon>& muon2() const {
    static boost::shared_ptr<const AnaMuon> tmp;
    const boost::shared_ptr<const AnaMuon> muon = boost::dynamic_pointer_cast<const AnaMuon,const AnaParticle>( lepton2() );
    return ( muon ? muon : tmp );
  }

  // max deta between leading jet and the next two highest pT jets
  const AnaParticle::ftype max_deta_jets() const {
    if( njets()<=1 ) return 0.;
    if( njets()<=2 ) return detector::delta_eta( leading_jet()->eta() , subleading_jet()->eta() );
    if( detector::delta_eta( leading_jet()->eta() , subleading_jet()->eta() ) < detector::delta_eta( leading_jet()->eta() , subsubleading_jet()->eta() ) ) {
      return  detector::delta_eta( leading_jet()->eta() , subsubleading_jet()->eta() );
    } 
    return detector::delta_eta( leading_jet()->eta() , subleading_jet()->eta() );
  }

  // Some signal/control regions for the heavy quark search
  const AnaParticle::ftype truth_m() const {
    if( _event_vlq_truth.size() != 1 ) return 0.;
    return _event_vlq_truth[0]->m();
  }
  const bool lowpt_CR() const { return njets()>=2 && pt()<=50.; }
  const bool highpt_lowmet_SR() const { return met() ? met()->pt()<50. && pt()>50. && njets()>=2 : false; }
  const bool highpt_SR() const { return njets()>=2 && pt()>50; }
  const bool highptdeta_SR() const { return njets()>=2 && pt()>50 && std::abs( max_deta_jets() )>1.; }
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

  const bool asym_iso_default() const { // 2012 analysis
    const boost::shared_ptr<const AnaParticle>& l1 = lepton1();
    const boost::shared_ptr<const AnaParticle>& l2 = lepton2();
    const boost::shared_ptr<const AnaMuon> m1 = boost::dynamic_pointer_cast<const AnaMuon,const AnaParticle>(l1);
    const boost::shared_ptr<const AnaMuon> m2 = boost::dynamic_pointer_cast<const AnaMuon,const AnaParticle>(l2);
    const bool m1_passes = m1 ? m1->pt_cone_ratio(AnaMuon::CONE40) < 0.1 : true;
    const bool m2_passes = m2 ? m2->pt_cone_ratio(AnaMuon::CONE40) < 0.1 : true;
    return m1_passes && m2_passes;
  }
  const bool asym_iso_ww() const {
    const boost::shared_ptr<const AnaParticle>& l1 = lepton1();
    const boost::shared_ptr<const AnaParticle>& l2 = lepton2();
    const boost::shared_ptr<const AnaMuon> m1 = boost::dynamic_pointer_cast<const AnaMuon,const AnaParticle>(l1);
    const boost::shared_ptr<const AnaMuon> m2 = boost::dynamic_pointer_cast<const AnaMuon,const AnaParticle>(l2);
    const bool m1_passes = m1 ? m1->pt_cone_ratio(AnaMuon::CONE40) < 0.1 && m1->et_cone_corr_ratio(AnaMuon::CONE40,nvtxs_all()) < 0.1 : true;
    const bool m2_passes = m2 ? m2->pt_cone_ratio(AnaMuon::CONE40) < 0.1 && m2->et_cone_corr_ratio(AnaMuon::CONE40,nvtxs_all()) < 0.1 : true;
    return m1_passes && m2_passes;
  }
  const bool asym_isofail_default() const { // anti-isolation: both leptons fail
    const boost::shared_ptr<const AnaParticle>& l1 = lepton1();
    const boost::shared_ptr<const AnaParticle>& l2 = lepton2();
    const boost::shared_ptr<const AnaMuon> m1 = boost::dynamic_pointer_cast<const AnaMuon,const AnaParticle>(l1);
    const boost::shared_ptr<const AnaMuon> m2 = boost::dynamic_pointer_cast<const AnaMuon,const AnaParticle>(l2);
    const bool m1_passes = m1 ? m1->pt_cone_ratio(AnaMuon::CONE40) >= 0.1 : true;
    const bool m2_passes = m2 ? m2->pt_cone_ratio(AnaMuon::CONE40) >= 0.1 : true;
    return m1_passes && m2_passes;
  }
  const bool asym_isofail_loose() const { // anti-isolation: both leptons fail
    const boost::shared_ptr<const AnaParticle>& l1 = lepton1();
    const boost::shared_ptr<const AnaParticle>& l2 = lepton2();
    const boost::shared_ptr<const AnaMuon> m1 = boost::dynamic_pointer_cast<const AnaMuon,const AnaParticle>(l1);
    const boost::shared_ptr<const AnaMuon> m2 = boost::dynamic_pointer_cast<const AnaMuon,const AnaParticle>(l2);
    const bool m1_passes = m1 ? m1->pt_cone_ratio(AnaMuon::CONE20) >= 0.1 : true;
    const bool m2_passes = m2 ? m2->pt_cone_ratio(AnaMuon::CONE20) >= 0.1 : true;
    return m1_passes && m2_passes;
  }
  const bool asym_quality() const {
    const boost::shared_ptr<const AnaParticle>& l1 = lepton1();
    const boost::shared_ptr<const AnaParticle>& l2 = lepton2();
    const boost::shared_ptr<const AnaMuon> m1 = boost::dynamic_pointer_cast<const AnaMuon,const AnaParticle>(l1);
    const boost::shared_ptr<const AnaMuon> m2 = boost::dynamic_pointer_cast<const AnaMuon,const AnaParticle>(l2);
    const bool m1_passes = m1 ? m1->passes_hits_mcp_v17_moriond() && std::abs(m1->exPV_z0())<10. : true;
    const bool m2_passes = m2 ? m2->passes_hits_mcp_v17_moriond() && std::abs(m2->exPV_z0())<10. : true;
    return m1_passes && m2_passes;
  }
  const bool asym_kinematics() const {
    const boost::shared_ptr<const AnaParticle>& l1 = lepton1();
    const boost::shared_ptr<const AnaParticle>& l2 = lepton2();
    const bool m1_passes = l1 ? l1->pt()>20. && std::abs(l1->eta())<2.4 : false;
    const bool m2_passes = l2 ? l2->pt()>20. && std::abs(l2->eta())<2.4 : false;
    return m1_passes && m2_passes;
  }
  const bool asym_zwindow() const {
    const ftype mm = m();
    return mm>66. && mm<116.;
  }
  const bool asym_opsign() const {
    return opposite_leptons(); 
  }
  const bool asym_samesign() const { 
    return samesign_leptons(); 
  }
  const bool asym_trigger() const { 
    const boost::shared_ptr<const AnaParticle>& l1 = lepton1();
    const boost::shared_ptr<const AnaParticle>& l2 = lepton2();
    const boost::shared_ptr<const AnaMuon> m1 = boost::dynamic_pointer_cast<const AnaMuon,const AnaParticle>(l1);
    const boost::shared_ptr<const AnaMuon> m2 = boost::dynamic_pointer_cast<const AnaMuon,const AnaParticle>(l2);
    const bool m1_passes = m1 ? m1->passes_trigger_matching_v17() : true;
    const bool m2_passes = m2 ? m2->passes_trigger_matching_v17() : true;
    // for now, require at least one muon to match trigger
    return m1_passes || m2_passes;
  }
  const bool asym_nmuons() const {
    return atmost_two_leptons(); 
  }
  // baseline for the analysis [W/Z default isolation]
  const bool asym_baseline() const {
    return asym_nmuons() && asym_quality() && asym_kinematics() && asym_iso_default() && asym_zwindow() && asym_opsign();
  }
  const bool asym_ss() const {
    return asym_nmuons() && asym_quality() && asym_kinematics() && asym_iso_default() && asym_zwindow() && asym_samesign();
  }
  // baseline for the analysis [WW righter isolation]
  const bool asym_baseline_ww() const {
    return asym_nmuons() && asym_quality() && asym_kinematics() && asym_iso_ww() && asym_zwindow() && asym_opsign();
  }
  const bool asym_ss_ww() const {
    return asym_nmuons() && asym_quality() && asym_kinematics() && asym_iso_ww() && asym_zwindow() && asym_samesign();
  }
  // version of baseline and metfit assuming the isolation and kinematic cuts have already been applied
  const bool asym_baseline_truth() const {
    return asym_kinematics() && asym_zwindow() && asym_opsign();
  }
  const bool asym_baseline_quick() const {
    return asym_nmuons() && asym_zwindow() && asym_opsign();
  }
  const bool asym_ss_quick() const {
    return asym_nmuons() && asym_zwindow() && asym_samesign();
  }
  // extras
  const bool asym_isofail() const {
    return asym_nmuons() && asym_quality() && asym_kinematics() && asym_isofail_default() && asym_zwindow() && asym_opsign();
  }

  // 0's cuts for finding Z->mumu tag-and-probe events
  const bool leptons_moving_at_opposite_phi_directions() const {
    return std::abs( detector::delta_phi(_event_leptons[0]->phi(),_event_leptons[1]->phi()) ) > 2.356; 
  }
  const bool opposite_signs_opposite_directions() const { return exactly_two_opposite_leptons() && leptons_moving_at_opposite_phi_directions(); }
  const bool opposite_signs_0jets() const { return exactly_two_opposite_leptons() && njets()==0; }
  const bool opposite_signs_1jets() const { return exactly_two_opposite_leptons() && njets()==1; }
  const bool opposite_signs_2jets() const { return exactly_two_opposite_leptons() && njets()==2; }
  const bool opposite_signs_3jets() const { return exactly_two_opposite_leptons() && njets()==3; }
  const bool opposite_signs_4jets() const { return exactly_two_opposite_leptons() && njets()==4; }
  const bool opposite_signs_5jets() const { return exactly_two_opposite_leptons() && njets()==5; }
  const bool opposite_signs_6upjets() const { return exactly_two_opposite_leptons() && njets()>=6; }
  const bool opposite_signs_jets() const { return exactly_two_opposite_leptons() && njets()>1; }
  const bool opposite_signs_2to4_jets() const { return exactly_two_opposite_leptons() && njets()>1 && njets()<5; }
  const bool opposite_signs_2to3_jets() const { return exactly_two_opposite_leptons() && njets()>1 && njets()<4; }
  const bool atleast_one_muon_is_tight() const {
    const boost::shared_ptr<const AnaParticle>& l1 = lepton1();
    const boost::shared_ptr<const AnaParticle>& l2 = lepton2();
    const boost::shared_ptr<const AnaMuon> m1 = boost::dynamic_pointer_cast<const AnaMuon,const AnaParticle>(l1);
    const boost::shared_ptr<const AnaMuon> m2 = boost::dynamic_pointer_cast<const AnaMuon,const AnaParticle>(l2);
    return ( m1->pt_cone_ratio(AnaMuon::CONE20) < 0.1) || ( m2->pt_cone_ratio(AnaMuon::CONE20) < 0.1);
  }

  // a few assignment operators for event-wide objects
  void add_event_lepton(const boost::shared_ptr<const AnaParticle>& v) { _event_leptons.push_back(v); }
  void add_event_jet(const boost::shared_ptr<const AnaJet>& v) { _event_jets.push_back(v); }
  template<typename iteratorJetT>
  void add_event_jets( iteratorJetT begin_jets , iteratorJetT end_jets ) {
    _event_jets.assign( begin_jets , end_jets );
  }
  void add_event_met(const boost::shared_ptr<const AnaMET>& v) { _event_met.push_back(v); }
  void add_event_vertex(const boost::shared_ptr<const AnaVertex>& v) { _event_vertices.push_back(v); }
  
  // old interface - used to build z candidates directly from components
  // it constructs a Z from the first two leptons in the lepton collection
  template<typename iteratorLepT,typename iteratorMetT,typename iteratorJetT,typename iteratorVtxT,typename iteratorTruthT>
  static const boost::shared_ptr<AnaZCrossSectionCandidate> build( iteratorLepT begin_leptons , iteratorLepT end_leptons ,
								   iteratorMetT begin_mets , iteratorMetT end_mets ,
								   iteratorJetT begin_jets , iteratorJetT end_jets ,
								   iteratorVtxT begin_vtxs , iteratorVtxT end_vtxs ) {
    if( std::distance(begin_leptons,end_leptons )!=2 ) { 
      //std::cout << "WARNING trying to build Z candidate with nleptons = " << std::distance(begin_leptons,end_leptons) << " , failed" << std::endl;
      return boost::shared_ptr<AnaZCrossSectionCandidate>(); 
    }
    if( begin_mets==end_mets ) { 
      //std::cout << "WARNING trying to build Z candidate with no MET , failed" << std::endl;
      return boost::shared_ptr<AnaZCrossSectionCandidate>(); 
    }
    // sort leptons by pt
    std::vector< boost::shared_ptr<const AnaParticle> > leptons_by_pt;
    for( iteratorLepT i=begin_leptons; i!=end_leptons; ++i ) { leptons_by_pt.push_back( *i ); }
    std::sort( leptons_by_pt.begin() , leptons_by_pt.end() , bind(&AnaParticle::pt,_1) > bind(&AnaParticle::pt,_2) );
    // form the Z candidate from the two highest-pt leptons
    boost::shared_ptr<AnaZCrossSectionCandidate> result(new AnaZCrossSectionCandidate(boost::shared_ptr<AnaZ>(new AnaZ(  *(leptons_by_pt.begin()) , *(++leptons_by_pt.begin())  ))));
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
    result->_event_vertices.assign( begin_vtxs , end_vtxs );
    iteratorLepT lep2=begin_leptons; lep2++;
    assert( !(result->_event_leptons.empty()) );
    assert( !(result->_event_met.empty()) );
    return result;
  }


  template<typename iteratorLepT,typename iteratorMetT,typename iteratorJetT,typename iteratorVtxT,typename iteratorTruthT>
  static const boost::shared_ptr<AnaZCrossSectionCandidate> build( iteratorLepT begin_leptons , iteratorLepT end_leptons ,
								   iteratorMetT begin_mets , iteratorMetT end_mets ,
								   iteratorJetT begin_jets , iteratorJetT end_jets ,
								   iteratorVtxT begin_vtxs , iteratorVtxT end_vtxs ,
								   iteratorTruthT begin_vlq_truth , iteratorTruthT end_vlq_truth ,
								   iteratorTruthT begin_fj_truth , iteratorTruthT end_fj_truth ) {
    if( std::distance(begin_leptons,end_leptons )!=2 ) { 
      //std::cout << "WARNING trying to build Z candidate with nleptons = " << std::distance(begin_leptons,end_leptons) << " , failed" << std::endl;
      return boost::shared_ptr<AnaZCrossSectionCandidate>(); 
    }
    if( begin_mets==end_mets ) { 
      //std::cout << "WARNING trying to build Z candidate with no MET , failed" << std::endl;
      return boost::shared_ptr<AnaZCrossSectionCandidate>(); 
    }
    // sort leptons by pt
    std::vector< boost::shared_ptr<const AnaParticle> > leptons_by_pt;
    for( iteratorLepT i=begin_leptons; i!=end_leptons; ++i ) { leptons_by_pt.push_back( *i ); }
    std::sort( leptons_by_pt.begin() , leptons_by_pt.end() , bind(&AnaParticle::pt,_1) > bind(&AnaParticle::pt,_2) );
    // form the Z candidate from the two highest-pt leptons
    boost::shared_ptr<AnaZCrossSectionCandidate> result(new AnaZCrossSectionCandidate(boost::shared_ptr<AnaZ>(new AnaZ(  *(leptons_by_pt.begin()) , *(++leptons_by_pt.begin())  ))));
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
    result->_event_vertices.assign( begin_vtxs , end_vtxs );
    result->_event_vlq_truth.assign( begin_vlq_truth , end_vlq_truth );
    result->_event_fj_truth.assign( begin_fj_truth , end_fj_truth );
    iteratorLepT lep2=begin_leptons; lep2++;
    assert( !(result->_event_leptons.empty()) );
    assert( !(result->_event_met.empty()) );
    return result;
  }

  // new interface - uses pre-built z's
  template<typename iteratorLepT,typename iteratorMetT,typename iteratorJetT,typename iteratorVertexT>
  class Compose {
  private:
    iteratorLepT _begin_leptons, _end_leptons;
    iteratorMetT _begin_mets, _end_mets;
    iteratorJetT _begin_jets, _end_jets;
    iteratorVertexT _begin_vertices, _end_vertices;
  public:
    typedef boost::shared_ptr<const AnaZCrossSectionCandidate> result_type;
    Compose( iteratorLepT begin_leptons , iteratorLepT end_leptons ,
             iteratorMetT begin_mets , iteratorMetT end_mets ,
             iteratorJetT begin_jets , iteratorJetT end_jets ,
             iteratorVertexT begin_vertices , iteratorVertexT end_vertices )
      : _begin_leptons(begin_leptons), _end_leptons(end_leptons)
      , _begin_mets(begin_mets), _end_mets(end_mets)
      , _begin_jets(begin_jets), _end_jets(end_jets)
      , _begin_vertices(begin_vertices), _end_vertices(end_vertices)
    {
      assert( std::distance( begin_mets , end_mets ) == 1 );
    }
    result_type operator()( const boost::shared_ptr<const AnaZ>& z ) const {
      boost::shared_ptr<AnaZCrossSectionCandidate> result( new AnaZCrossSectionCandidate(z) ); assert( result );
      result->_event_leptons.assign( _begin_leptons , _end_leptons );
      result->_event_met.assign( _begin_mets , _end_mets );
      result->_event_jets.assign( _begin_jets , _end_jets );
      result->_event_vertices.assign( _begin_vertices , _end_vertices );
      std::sort( result->_event_leptons.begin() , result->_event_leptons.end() , boost::bind( &AnaParticle::pt , _1 ) > boost::bind( &AnaParticle::pt , _2 ) );
      std::sort( result->_event_jets.begin() , result->_event_jets.end() , boost::bind( &AnaParticle::pt , _1 ) > boost::bind( &AnaParticle::pt , _2 ) );
      return result;
    }
  };

  
  // new interface - uses pre-built z's; allows to specify an additional vertex collection
  template<typename iteratorLepT,typename iteratorMetT,typename iteratorJetT,typename iteratorVertexT>
  class Compose2 {
  private:
    iteratorLepT _begin_leptons, _end_leptons;
    iteratorMetT _begin_mets, _end_mets;
    iteratorJetT _begin_jets, _end_jets;
    iteratorVertexT _begin_vertices, _end_vertices;
    iteratorVertexT _begin_all_vertices, _end_all_vertices;
  public:
    typedef boost::shared_ptr<const AnaZCrossSectionCandidate> result_type;
    Compose2( iteratorLepT begin_leptons , iteratorLepT end_leptons ,
	      iteratorMetT begin_mets , iteratorMetT end_mets ,
	      iteratorJetT begin_jets , iteratorJetT end_jets ,
	      iteratorVertexT begin_vertices , iteratorVertexT end_vertices ,
	      iteratorVertexT begin_all_vertices , iteratorVertexT end_all_vertices )
      : _begin_leptons(begin_leptons), _end_leptons(end_leptons)
      , _begin_mets(begin_mets), _end_mets(end_mets)
      , _begin_jets(begin_jets), _end_jets(end_jets)
      , _begin_vertices(begin_vertices), _end_vertices(end_vertices)
      , _begin_all_vertices(begin_all_vertices), _end_all_vertices(end_all_vertices)
    {
      assert( std::distance( begin_mets , end_mets ) == 1 );
    }
    result_type operator()( const boost::shared_ptr<const AnaZ>& z ) const {
      boost::shared_ptr<AnaZCrossSectionCandidate> result( new AnaZCrossSectionCandidate(z) ); assert( result );
      result->_event_leptons.assign( _begin_leptons , _end_leptons );
      result->_event_met.assign( _begin_mets , _end_mets );
      result->_event_jets.assign( _begin_jets , _end_jets );
      result->_event_vertices.assign( _begin_vertices , _end_vertices );
      result->_event_all_vertices.assign( _begin_all_vertices , _end_all_vertices );
      std::sort( result->_event_leptons.begin() , result->_event_leptons.end() , boost::bind( &AnaParticle::pt , _1 ) > boost::bind( &AnaParticle::pt , _2 ) );
      std::sort( result->_event_jets.begin() , result->_event_jets.end() , boost::bind( &AnaParticle::pt , _1 ) > boost::bind( &AnaParticle::pt , _2 ) );
      return result;
    }
  };

};

BOOST_CLASS_VERSION( AnaZCrossSectionCandidate , 3 );

#endif // ANAZCROSSSECTIONCANDIDATE_HPP
