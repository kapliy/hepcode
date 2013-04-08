
#ifndef ANAWCROSSSECTIONCANDIDATE_HPP
#define ANAWCROSSSECTIONCANDIDATE_HPP

// A wrapper class around AnaW that contains additional event-wide information
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
#include <boost/math/special_functions/fpclassify.hpp>
#include <TLorentzVector.h>
#include "TrigFTKAna/AnaW.hpp"
#include "TrigFTKAna/AnaElectron.hpp"
#include "TrigFTKAna/AnaPhoton.hpp"
#include "TrigFTKAna/AnaMuon.hpp"
#include "TrigFTKAna/AnaJet.hpp"
#include "TrigFTKAna/AnaVertex.hpp"
#include "TrigFTKAna/AnaMET.hpp"

// BDT stuff
#include <TMVA/Factory.h>
#include <TMVA/Tools.h>
#include <TMVA/Reader.h>
#include <TMVA/MethodCuts.h>

class
AnaWCrossSectionCandidate : public AnaW
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
  std::vector< boost::shared_ptr<const AnaTruthParticle> > _event_w_truth; // unsorted
  std::vector< boost::shared_ptr<const AnaTruthParticle> > _event_nu_truth; // unsorted
  std::vector< boost::shared_ptr<const AnaTruthParticle> > _event_fj_truth; // unsorted
  unsigned long _event_number;
  bool _is_mc;
  mutable bool _cached;
private:
  friend class boost::serialization::access;
  template<class archiveT>
  void serialize( archiveT& ar, const unsigned int version ) {
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP( AnaW );
    ar & boost::serialization::make_nvp("event_leptons",_event_leptons);
    ar & boost::serialization::make_nvp("event_jets",_event_jets);
    ar & boost::serialization::make_nvp("event_met",_event_met);
    ar & boost::serialization::make_nvp("event_vertices",_event_vertices);
    ar & boost::serialization::make_nvp("event_all_vertices",_event_all_vertices);
    ar & boost::serialization::make_nvp("event_vlq_truth",_event_vlq_truth);
    ar & boost::serialization::make_nvp("event_w_truth",_event_w_truth);
    ar & boost::serialization::make_nvp("event_nu_truth",_event_nu_truth);
    ar & boost::serialization::make_nvp("event_fj_truth",_event_fj_truth);
  }
  void _refresh_cache() const {
    if( _cached ) { return; }
    // nothing done here yet
    _cached = true;
  }
public:

  AnaWCrossSectionCandidate()
    : _event_leptons()
    , _event_jets()
    , _event_met()
    , _event_vertices()
    , _event_all_vertices()
    , _event_vlq_truth()
    , _event_w_truth()
    , _event_nu_truth()
    , _event_fj_truth()
    , AnaW()
  {
  }
  AnaWCrossSectionCandidate(const boost::shared_ptr<const AnaW>& w)
    : _event_leptons()
    , _event_jets()
    , _event_met()
    , _event_vertices()
    , _event_all_vertices()
    , _event_vlq_truth()
    , _event_w_truth()
    , _event_nu_truth()
    , _event_fj_truth()
    , _cached(false)
    , AnaW(*w)
  {
    _refresh_cache();
  }

  virtual ~AnaWCrossSectionCandidate() {}

  // event info
  const unsigned long event_number() const { return _event_number; }
  const bool mc() const { return _is_mc; }


  // particle counts
  const unsigned int nleptons() const { return _event_leptons.size(); }
  const unsigned int nleptons25() const{
    return std::count_if( _event_leptons.begin(),_event_leptons.end(),bind(&AnaParticle::pt,_1)>25.0);
  }
  const unsigned int nmets() const { return _event_met.size(); }
  const unsigned int njets() const { return _event_jets.size(); }
  const unsigned int ntruth() const { return _event_vlq_truth.size() + _event_w_truth.size() + _event_nu_truth.size() + _event_fj_truth.size(); }
  const unsigned int nvlqtruth() const { return _event_vlq_truth.size(); }
  const unsigned int nwtruth() const { return _event_w_truth.size(); }
  const unsigned int nnutruth() const { return _event_nu_truth.size(); }
  const unsigned int nfjtruth() const { return _event_fj_truth.size(); }
  const bool exactly_one_lepton() const { return _event_leptons.size()==1; }
  const bool atleast_one_lepton() const { return _event_leptons.size()>=1; }
  const bool atmost_one_lepton() const { return _event_leptons.size()<=1; }


  // return leptons
  const boost::shared_ptr<const AnaMuon>& muon() const {
    static boost::shared_ptr<const AnaMuon> tmp;
    const boost::shared_ptr<const AnaMuon> muon = boost::dynamic_pointer_cast<const AnaMuon,const AnaParticle>( lepton() );
    return ( muon ? muon : tmp);
  }
  const boost::shared_ptr<const AnaElectron>& electron() const {
    static boost::shared_ptr<const AnaElectron> tmp;
    const boost::shared_ptr<const AnaElectron> electron = boost::dynamic_pointer_cast<const AnaElectron,const AnaParticle>( lepton() );
    return ( electron ? electron : tmp);
  }

  // lepton variables
  const short scharge() const { lepton() ? lepton()->scharge() : 0.; }
  const AnaParticle::ftype lepton_pt() const { return lepton() ? lepton()->pt() : std::numeric_limits<AnaParticle::ftype>::min(); }
  const AnaParticle::ftype lepton_eta() const { return lepton() ? lepton()->eta() : std::numeric_limits<AnaParticle::ftype>::max(); }
  const AnaParticle::ftype lepton_phi() const { return lepton() ? lepton()->phi() : std::numeric_limits<AnaParticle::ftype>::max(); }
  const bool lepton_is_muon() const { return muon() ? true : false; }
  const bool lepton_is_electron() const { return electron() ? true : false; }

  const AnaParticle::ftype muon_pt() const { return lepton_pt(); }
  const AnaParticle::ftype muon_id_pt() const { return muon()->id_pt(); }
  const AnaParticle::ftype muon_exms_pt() const { return muon()->exms_pt(); }
  const AnaParticle::ftype muon_pt_uncorrected() const { return muon()->is_corrected() ? muon()->uncorrected_parent()->pt() : muon_pt(); }
  const AnaParticle::ftype muon_id_pt_uncorrected() const { return muon()->is_corrected() ? muon()->uncorrected_parent()->id_pt() : muon_id_pt(); }
  const AnaParticle::ftype muon_exms_pt_uncorrected() const { return muon()->is_corrected() ? muon()->uncorrected_parent()->exms_pt() : muon_exms_pt(); }

  // return various jets
  jet_const_iterator begin_jets() const { return _event_jets.begin(); }
  jet_const_iterator end_jets() const { return _event_jets.end(); }
  const boost::shared_ptr<const AnaJet> jet( const short& i ) const {
    if( i >= _event_jets.size() ) return boost::shared_ptr<const AnaJet>();
    return _event_jets[i];
  }
  const boost::shared_ptr<const AnaJet> leading_jet() const { return jet(0); }
  const boost::shared_ptr<const AnaJet> subleading_jet() const { return jet(1); }
  const boost::shared_ptr<const AnaJet> subsubleading_jet() const { return jet(2); }
  const boost::shared_ptr<const AnaJet> far_jet() const {
    if( njets() < 2 ) return boost::shared_ptr<const AnaJet>();
    return _event_jets[far_jet_id()-1];
  }
  const std::size_t  far_jet_id() const {
    if( njets() < 2 ) 0;
    AnaParticle::ftype max_deta = 0;
    std::size_t ifar = 1;
    for( std::size_t i = 1 ; i < _event_jets.size() ; ++i ) {
      if( fabs(_event_jets[0]->eta() - _event_jets[i]->eta() ) > max_deta ) {
	max_deta = fabs(_event_jets[0]->eta() - _event_jets[i]->eta());
	ifar = i;
      }
    }
    return ifar+1;
  }
  const boost::shared_ptr<const AnaJet> associated_jet() const {
    if( _event_jets.size() < 2 ) return boost::shared_ptr<const AnaJet>();
    return _event_jets[associated_jet_id()-1];
  }
  const std::size_t associated_jet_id() const {
    if( _event_jets.size() < 2 ) return 0;
    if( _event_jets.size() < 3 ) return 2;
    if( fabs(_event_jets[0]->eta() - _event_jets[1]->eta()) > fabs(_event_jets[0]->eta() - _event_jets[2]->eta()) ) return 2;
    return 3;
  }


  // jet truth matching
  const std::size_t truth_matched_jet_id() const {
    if( njets()<2 ) return std::numeric_limits<std::size_t>::min();
    if( _event_fj_truth.size()!=1 ) return std::numeric_limits<std::size_t>::min();
    AnaParticle::ftype mindr = std::numeric_limits<AnaParticle::ftype>::max();
    std::size_t imatched = 0;
    for( std::size_t i = 1 ; i < _event_jets.size() ; ++i ) {
      AnaParticle::ftype this_dr = detector::delta_eta_phi( _event_jets[i]->eta() , _event_jets[i]->phi() , _event_fj_truth[0]->eta() , _event_fj_truth[0]->phi() );
      if( this_dr < mindr ) {
	mindr = this_dr;
	imatched = i;
      }
    }
    return imatched+1;
  }
  const bool associatedjet_is_correct() const {
    if( njets()<2 ) return false;
    if( _event_fj_truth.size()!=1 ) return false;
    if( truth_matched_jet_id() != associated_jet_id() ) return false;
    return true;
  }
  const bool farjet_is_correct() const {
    if( njets()<2 ) return false;
    if( _event_fj_truth.size()!=1 ) return false;
    if( truth_matched_jet_id() != far_jet_id() ) return false;
    return true;
  }



  // MET variables
  const bool exactly_one_met() const { return _event_met.size()==1; }
  const AnaParticle::ftype met_pt() const { return met() ? met()->pt() : std::numeric_limits<AnaParticle::ftype>::min(); }
  const AnaParticle::ftype met_phi() const { return met() ? met()->phi() : std::numeric_limits<AnaParticle::ftype>::max(); }

  // reconstructed Pz MET variables
  const int npzsolutions() const {
    if( met_pz(0) == met_pz(1) ) return 1;
    else return 2;
  }
  const AnaParticle::ftype met_eta( const AnaParticle::ftype& pz ) const { return detector::etaFromPxPyPz( met()->px() , met()->py() , pz ); }
  TLorentzVector met_four_vector( const AnaParticle::ftype& pz ) const {
    TLorentzVector result( 0. , 0. , 0. , 0. );
    result.SetPtEtaPhiM( met()->pt() , met_eta(pz) , met()->phi() , 0. );
    return result;
  }
  const AnaParticle::ftype met_pz( const short& sol ) const { 
    if( !met() ) return 0.;
    if( !lepton() ) return 0.;
    return detector::reconstructedMETpz( lepton()->px() , lepton()->py() , lepton()->pz() , met()->px() , met()->py() , sol );
  }
  const AnaParticle::ftype met_pz_low() const { return met_pz(0); }
  const AnaParticle::ftype met_pz_high() const { return met_pz(1); }
  const AnaParticle::ftype met_pz_vlq() const {
    if( !lepton() ) return 0.;
    if( !met() ) return 0.;
    AnaParticle::ftype pz_low = met_pz_low();
    AnaParticle::ftype pz_high = met_pz_high();
    if( pz_low==pz_high ) return pz_low;
    if( njets()==0 ) return pz_high;
    TLorentzVector w_low( 0. , 0. , 0. , 0. );
    TLorentzVector w_high( 0. , 0. , 0. , 0. );
    w_low += lepton()->four_vector();
    w_low += met_four_vector(pz_low);
    w_high += lepton()->four_vector();
    w_high += met_four_vector(pz_high);
    if( std::fabs(w_low.Eta()-leading_jet()->eta()) <= std::fabs(w_high.Eta()-leading_jet()->eta()) ) return pz_low;
    return pz_high;
  }
  const AnaParticle::ftype met_pz_nu_matched() const {
    if( !met() ) return 0.;
    if( _event_nu_truth.size()!=1 ) return 0.;
    TLorentzVector nu_low = met_four_vector( met_pz_low() );
    TLorentzVector nu_high = met_four_vector( met_pz_high() );
    if( detector::delta_eta_phi( float(nu_low.Eta()) , float(nu_low.Phi()) , _event_nu_truth[0]->eta() , _event_nu_truth[0]->phi() ) <=
	detector::delta_eta_phi( float(nu_high.Eta()) , float(nu_high.Phi()) , _event_nu_truth[0]->eta() , _event_nu_truth[0]->phi() ) )
      return met_pz_low();
    return met_pz_high();
    
  }
  const AnaParticle::ftype met_pz_w_matched() const {
    if( !lepton() ) return 0.;
    if( !met() ) return 0.;
    if( _event_w_truth.size()!=1 ) return 0.;
    AnaParticle::ftype pz_low = met_pz_low();
    AnaParticle::ftype pz_high = met_pz_high();
    if( pz_low==pz_high ) return pz_low;
    TLorentzVector w_low( 0. , 0. , 0. , 0. );
    TLorentzVector w_high( 0. , 0. , 0. , 0. );
    w_low += lepton()->four_vector();
    w_low += met_four_vector(pz_low);
    w_high += lepton()->four_vector();
    w_high += met_four_vector(pz_high);
    if( detector::delta_eta_phi( float(w_low.Eta()) , float(w_low.Phi()) , _event_w_truth[0]->eta() , _event_w_truth[0]->phi() ) <=
	detector::delta_eta_phi( float(w_high.Eta()) , float(w_high.Phi()) , _event_w_truth[0]->eta() , _event_w_truth[0]->phi() ) )
      return pz_low;
    return pz_high;
  }
  const AnaParticle::ftype met_pz_vlq_matched() const {
    if( !lepton() ) return 0.;
    if( !met() ) return 0.;
    if( _event_w_truth.size()!=1 ) return 0.;
    AnaParticle::ftype pz_low = met_pz_low();
    AnaParticle::ftype pz_high = met_pz_high();
    if( pz_low==pz_high ) return pz_low;
    if( njets()==0 ) return 0.;
    TLorentzVector vlq_low( 0. , 0. , 0. , 0. );
    TLorentzVector vlq_high( 0. , 0. , 0. , 0. );
    vlq_low += lepton()->four_vector();
    vlq_low += met_four_vector(pz_low);
    vlq_low += leading_jet()->four_vector();
    vlq_high += lepton()->four_vector();
    vlq_high += met_four_vector(pz_high);
    vlq_high += leading_jet()->four_vector();
    if( detector::delta_eta_phi( float(vlq_low.Eta()) , float(vlq_low.Phi()) , _event_w_truth[0]->eta() , _event_w_truth[0]->phi() ) <=
	detector::delta_eta_phi( float(vlq_high.Eta()) , float(vlq_high.Phi()) , _event_w_truth[0]->eta() , _event_w_truth[0]->phi() ) )
      return pz_low;
    return pz_high;
  }
  TLorentzVector w_vlq() const {
    TLorentzVector result( 0. , 0. , 0. , 0. );
    if( !lepton() ) return result;
    if( !met() ) return result;
    result += lepton()->four_vector();
    result += met_four_vector( met_pz_vlq() );
    return result;
  }
  const bool low_pz_solution_is_correct() const {
    if( npzsolutions()==1 ) return true;
    if( _event_nu_truth.size()!=1 ) return false;
    //if( !lepton() ) return false;
    if( !met() ) return false;
    //if( njets()==0 ) return false;
    return met_pz_low() == met_pz_nu_matched();
  }
  const bool high_pz_solution_is_correct() const  {
    if( npzsolutions()==1 ) return true;
    if( _event_nu_truth.size()!=1 ) return false;
    //if( !lepton() ) return false;
    if( !met() ) return false;
    //if( njets()==0 ) return false;
    return met_pz_high() == met_pz_nu_matched();
  }
  const bool vlq_pz_solution_is_correct() const {
    if( npzsolutions()==1 ) return true;
    if( _event_nu_truth.size()!=1 ) return false;
    //if( !lepton() ) return false;
    if( !met() ) return false;
    //if( njets()==0 ) return false;
    return met_pz_vlq() == met_pz_nu_matched();
  }


  // vertex variables
  const unsigned int nvtxs() const { return _event_vertices.size(); }
  const unsigned int nvtxs_all() const { return _event_all_vertices.size(); }


  // TRUTH particles
  const boost::shared_ptr<const AnaTruthParticle> truth_vlq() const {
    if( _event_vlq_truth.size()!=1 ) return boost::shared_ptr<const AnaTruthParticle>();
    return _event_vlq_truth[0];
  }
  const boost::shared_ptr<const AnaTruthParticle> truth_forwardjet() const {
    if( _event_fj_truth.size()!=1 ) return boost::shared_ptr<const AnaTruthParticle>();
    return _event_fj_truth[0];
  }


  // phi separations
  const AnaParticle::ftype dphi_lepton_met() const { 
    if( !met() ) return std::numeric_limits<AnaParticle::ftype>::max();
    if( !lepton() ) return std::numeric_limits<AnaParticle::ftype>::max();
    return detector::delta_phi( lepton()->phi() , met()->phi() );
  }
  const AnaParticle::ftype dphi_lepton_leadingjet() const {
    if( !lepton() ) return std::numeric_limits<AnaParticle::ftype>::max();
    if( !jet(0) ) return std::numeric_limits<AnaParticle::ftype>::max();
    return detector::delta_phi( lepton()->phi() , jet(0)->phi() );
  }
  const AnaParticle::ftype dphi_met_leadingjet() const {
    if( !met() ) return std::numeric_limits<AnaParticle::ftype>::max();
    if( !jet(0) ) return std::numeric_limits<AnaParticle::ftype>::max();
    return detector::delta_phi( met()->phi() , jet(0)->phi() );
  }
  const AnaParticle::ftype dphi_w_leadingjet() const {
    if( !jet(0) ) return std::numeric_limits<AnaParticle::ftype>::max();
    return detector::delta_phi( phi() , jet(0)->phi() );
  }
  const AnaParticle::ftype dphi_leadingjet_associatedjet() const {
    if( njets() < 2 ) return std::numeric_limits<AnaParticle::ftype>::max();
    return detector::delta_phi( leading_jet()->phi() , associated_jet()->phi() );
  }
  const AnaParticle::ftype dphi_leadingjet_farjet() const {
    if( njets() < 2 ) return std::numeric_limits<AnaParticle::ftype>::max();
    return detector::delta_phi( leading_jet()->phi() , far_jet()->phi() );
  }



  // eta separations
  const AnaParticle::ftype deta_leadingjet_subleadingjet() const {
    if( njets() < 2 ) return std::numeric_limits<AnaParticle::ftype>::max();
    return leading_jet()->eta() - subleading_jet()->eta();
  }
  const AnaParticle::ftype deta_lepton_leadingjet() const {
    if( !lepton() ) return std::numeric_limits<AnaParticle::ftype>::max();
    if( njets()==0 ) return std::numeric_limits<AnaParticle::ftype>::max();
    return lepton()->eta() - leading_jet()->eta();
  }
  const AnaParticle::ftype deta_leadingjet_associatedjet() const {
    if( njets() < 2 ) return std::numeric_limits<AnaParticle::ftype>::max();
    return leading_jet()->eta() - associated_jet()->eta();
  }
  const AnaParticle::ftype deta_leadingjet_farjet() const {
    if( njets() < 2 ) return std::numeric_limits<AnaParticle::ftype>::max();
    return leading_jet()->eta() - far_jet()->eta();
  }
  const AnaParticle::ftype deta_w_leadingjet() const {
    if( njets()==0 ) return std::numeric_limits<AnaParticle::ftype>::max();
    return w_eta() - leading_jet()->eta();
  }
  const AnaParticle::ftype deta_w_associatedjet() const {
    if( njets()<2 ) return std::numeric_limits<AnaParticle::ftype>::max();
    return w_eta() - associated_jet()->eta();
  }
  const AnaParticle::ftype deta_w_farjet() const {
    if( njets()<2 ) return std::numeric_limits<AnaParticle::ftype>::max();
    return w_eta() - far_jet()->eta();
  }


  // additional functions for random cuts
  const bool muon_eta_cut() const {
    if( !lepton_is_muon() ) return true;
    const boost::shared_ptr<const AnaMuon> muon = boost::dynamic_pointer_cast<const AnaMuon,const AnaParticle>( lepton() );
    return std::abs(muon->eta())<2.4;
  }



  // functions for comparing to Constantinos' cutflow
  const bool no_btag_event_jet() const {
    return( !(std::find_if( _event_jets.begin() , _event_jets.end() , boost::bind( &AnaJet::tag_sv0_w , _1 ) >= 5.72 ) != _event_jets.end() ) );
  }
  const bool low_dijet_eta() const {
    return( njets()>1 ? leading_jet()->eta() < 1.8 && subleading_jet()->eta() < 1.8 : false );
  }
  const bool mid_dijet_eta() const {
    return( njets()>1 ? leading_jet()->eta() < 2.5 && subleading_jet()->eta() < 2.5 : false );
  }


  // mass plots for heavy quark studies
  const AnaParticle::ftype truth_vlq_m() const {
    if( _event_vlq_truth.size()!=1 ) return 0.;
    return _event_vlq_truth[0]->m();
  }
  const double jets_m() const {
    TLorentzVector q( 0 , 0 , 0 , 0 );
    BOOST_FOREACH( boost::shared_ptr<const AnaJet> j , _event_jets ) { q += j->four_vector(); }
    return q.M();
  }
  const double dijet_m() const {
    if( njets()<2 ) return 0.;
    TLorentzVector q( 0 , 0 , 0 , 0 );
    q += leading_jet()->four_vector();
    q += subleading_jet()->four_vector();
    return boost::math::isnan( q.M() ) ? -1. : q.M();
  }
  const double event_m() const {
    TLorentzVector q( 0 , 0 , 0 , 0 );
    q += lepton()->four_vector();
    q += met_four_vector( met_pz_vlq() );
    BOOST_FOREACH( boost::shared_ptr<const AnaJet> j , _event_jets ) { q += j->four_vector(); }
    return boost::math::isnan( q.M() ) ? -1. : q.M();
  }
  const double event_mt() const {
    TLorentzVector q( 0 , 0 , 0 , 0 );
    q += lepton()->transverse_four_vector();
    q += met()->transverse_four_vector();
    BOOST_FOREACH( boost::shared_ptr<const AnaJet> j , _event_jets ) { q+= j->transverse_four_vector(); }
    return boost::math::isnan( q.M() ) ? -1. : q.M();
  }
  const double w_m() const { 
    double result = w_vlq().M();
    return boost::math::isnan(result) ? -999. : result;
  }
  const double w_eta() const {
    double result = w_vlq().Eta();
    return boost::math::isnan(result) ? -999. : result;
  }
  const double w_low_eta() const {
    TLorentzVector q( 0 , 0 , 0 , 0 );
    q += lepton()->four_vector();
    q += met_four_vector( met_pz_low() );
    return boost::math::isnan(q.Eta()) ? -999. : q.Eta();
  }
  const double w_high_eta() const {
    TLorentzVector q( 0 , 0 , 0 , 0 );
    q += lepton()->four_vector();
    q += met_four_vector( met_pz_high() );
    return boost::math::isnan(q.Eta()) ? -999. : q.Eta();
  }
  const double w_bdt_eta() const {
    TLorentzVector q( 0 , 0 , 0 , 0 );
    q += lepton()->four_vector();
    q += met_four_vector( met_pz_low() );
    return boost::math::isnan(q.Eta()) ? -999. : q.Eta();
  }
  const double wjj_mt() const {
    if( njets() < 2 ) return 0.;
    TLorentzVector q( 0 , 0 , 0 , 0 );
    q += transverse_four_vector();
    q += leading_jet()->transverse_four_vector();
    q += subleading_jet()->transverse_four_vector();
    return boost::math::isnan(q.M()) ? -1. : q.M();
  }
  const double wjj_m() const {
    if( njets() < 2 ) return 0;
    TLorentzVector q( 0 , 0 , 0 , 0 );
    q += w_vlq();
    q += leading_jet()->four_vector();
    q += subleading_jet()->four_vector();
    return boost::math::isnan(q.M()) ? -1. : q.M();
  }
  const ftype heavyQ_mt() const {
    if( njets()==0 ) return 0.;
    TLorentzVector q( 0 , 0 , 0 , 0 );
    q += transverse_four_vector();
    q += leading_jet()->transverse_four_vector();
    return boost::math::isnan(q.M()) ? -1. : q.M();
  }
  const ftype heavyQ_m() const {
    if( njets()==0 ) return 0.;
    if( !lepton() ) return 0.;
    if( !met() ) return 0.;
    TLorentzVector q( 0. , 0. , 0. , 0. );
    q += w_vlq();
    q += leading_jet()->four_vector();
    return boost::math::isnan(q.M()) ? -1 : q.M();
  }
  const ftype heavyQ_m_matched() const {
    if( njets()==0 ) return 0.;
    if( !lepton() ) return 0.;
    if( !met() ) return 0.;
    TLorentzVector q( 0. , 0. , 0. , 0. );
    q += leading_jet()->four_vector();
    q += lepton()->four_vector();
    q += met_four_vector( met_pz_nu_matched() );
    return boost::math::isnan(q.M()) ? -1 : q.M();
  }
  const ftype heavyQ_m_low() const {
    if( njets()==0 ) return 0.;
    if( !lepton() ) return 0.;
    if( !met() ) return 0.;
    TLorentzVector q( 0. , 0. , 0. , 0. );
    q += leading_jet()->four_vector();
    q += lepton()->four_vector();
    q += met_four_vector( met_pz_low() );
    return boost::math::isnan(q.M()) ? -1 : q.M();
  }
  const ftype heavyQ_m_high() const {
    if( njets()==0 ) return 0;
    if( !lepton() ) return 0.;
    if( !met() ) return 0.;
    TLorentzVector q( 0. , 0. , 0. , 0. );
    q += leading_jet()->four_vector();
    q += lepton()->four_vector();
    q += met_four_vector( met_pz_high() );
    return boost::math::isnan(q.M()) ? -1 : q.M();    
  }
    
    

  // JORDAN (1/22/2010) some useful definitions for control/signal regions
  const bool wmt() const { return mt()>40.; }
  const bool baseline() const { return met() ? ( mt()>40. && met()->pt()>25. ) : false; }
  const bool metfit_CR() const { return met() ? met()->pt()>10. : false; }
  const bool lowmet_CR() const { return met() ? met()->pt()<30. : false; }
  const bool lowmet_wmt_CR() const { return lowmet_CR() && mt()>40.; }
  const bool midmet_CR() const { return metfit_CR() && lowmet_CR(); }
  const bool midmet_wmt_CR() const { return midmet_CR() && mt()>40.; }
  const bool midmet_20_wmt_CR() const { return midmet_wmt_CR() && met()->pt()>20.; }
  const bool midmet_25_wmt_CR() const { return baseline() && lowmet_CR(); }
  const bool qcd_CR() const { return baseline() && met()->pt()>30. && met()->pt()<50.; }
  const bool qcd_2_CR() const { return qcd_CR() && std::fabs(dphi_lepton_met())>2.4; }
  const bool highmet_highdphi_CR() const { return ( met() && lepton() ) ? ( met()->pt()>30. && std::fabs(dphi_lepton_met())>2. ) : false; }
  const bool highermet_highdphi_CR() const { return ( met() && lepton() ) ? ( met()->pt()>50. && std::fabs(dphi_lepton_met())>2. ) : false; }
  const bool highmet_highdphi_wmt_CR() const { return highmet_highdphi_CR() && mt()>40.; }
  const bool highermet_highdphi_wmt_CR() const { return highermet_highdphi_CR() && mt()>40.; }
  const bool bdt_common_base() const {
    //if( _is_mc && _event_number %2 == 0 ) return false; // FIXMEEEE!!!!!!!!!
    if( !met() ) return false;
    if( !lepton() ) return false;
    if( mt() <= 40. ) return false;
    if( njets() < 2 ) return false;
    if( leading_jet()->pt() <= 60. ) return false;
    if( met()->pt() <= 50. ) return false; 
    return true;
  }
  const bool bdt_common() const {
    if( !bdt_common_base() ) return false;
    if( std::fabs(w_eta()) >= 2.5 ) return false;
    //if( std::fabs(w_low_eta()) >= 2.5 && std::fabs(w_high_eta()) >= 2.5 ) return false;
    //if( std::fabs(w_bdt_eta()) >= 2.5 ) return false;
    return true;
  }
  const double bdt_response( const std::string& bdtmethod = "BDT method W 900" ) const {
    if( !bdt_common() ) return -666.;
    static TMVA::Reader *cc_bdt_reader;
    static float cut_delta_eta_w_j1_n , cut_delta_phi_w_j1 , cut_delta_eta_w_j2_n , cut_delta_phi_lep_nu , cut_delta_eta_j1_j2;
    if( !cc_bdt_reader ) {
      cc_bdt_reader = new TMVA::Reader();
      cc_bdt_reader->AddVariable("cut_delta_eta_w_j1_n[0]", &cut_delta_eta_w_j1_n);
      cc_bdt_reader->AddVariable("cut_delta_phi_w_j1[0]", &cut_delta_phi_w_j1);
      cc_bdt_reader->AddVariable("cut_delta_eta_w_j2_n[0]", &cut_delta_eta_w_j2_n);
      cc_bdt_reader->AddVariable("cut_delta_eta_j1_j2[0]", &cut_delta_eta_j1_j2);
      cc_bdt_reader->AddVariable("cut_delta_phi_lep_nu[0]", &cut_delta_phi_lep_nu);
      cc_bdt_reader->BookMVA( "BDT method W 400" , "bdt_weights/VLQ_CC/TMVAClassificationHQ_115513.Pythia_MadGraph_vlq_qd_cc_400.merge.NTUP_SMWZ.e887_s1372_s1370_r3043_r2993_p833_BDT.weights.xml" );
      cc_bdt_reader->BookMVA( "BDT method W 900" , "bdt_weights/VLQ_CC/TMVAClassificationHQ_115518.Pythia_MadGraph_vlq_qd_cc_900.merge.NTUP_SMWZ.e887_s1372_s1370_r3043_r2993_p833_BDT.weights.xml" );
      cc_bdt_reader->BookMVA( "BDT method W 1200" , "bdt_weights/VLQ_CC/TMVAClassificationHQ_145083.Pythia_MadGraph_vlq_qd_cc_1200.merge.NTUP_SMWZ.e997_s1372_s1370_r3043_r2993_p833_BDT.weights.xml" );
    }
    // set the cut variables
    //float this_w_bdt_eta = w_bdt_eta();
    float this_w_bdt_eta = float(w_eta());
    float j2_eta = far_jet()->eta();
    cut_delta_eta_w_j1_n = std::fabs(this_w_bdt_eta - leading_jet()->eta());
    cut_delta_phi_w_j1 = std::fabs( detector::delta_phi( AnaW::phi() , leading_jet()->phi() ) );
    cut_delta_eta_w_j2_n = std::fabs(this_w_bdt_eta - j2_eta);
    cut_delta_eta_j1_j2 = std::fabs(leading_jet()->eta() - j2_eta);
    cut_delta_phi_lep_nu = std::fabs( detector::delta_phi( lepton()->phi() , met()->phi() ) );
    return cc_bdt_reader->EvaluateMVA( bdtmethod.c_str() );
  }
  void dump_bdt_vars( const unsigned long ev_number = 0 ) const {
    if( ev_number==0 || ev_number==_event_number ) {
      std::cout << "BDT DEBUGGING OUTPUT:" << std::endl
		<< " event_number = " << _event_number << std::endl
		<< " w_mt = " << mt() << std::endl
		<< " njets = " << njets() << std::endl
		<< " met = " << met()->pt() << " ( " << met()->uncorrected_parent()->pt() << " )" << std::endl
		<< " leading_jet_pt = " << leading_jet()->pt() << std::endl
		<< "   bdt_common_base = " << bdt_common_base() << std::endl
		<< " w_eta = " << w_eta() << " ( " << w_low_eta() << " , " << w_high_eta() << " )" << std::endl
		<< " leading_jet_eta = " << leading_jet()->eta() << std::endl
		<< "   bdt_common = " << bdt_common() << std::endl;
      if( bdt_common() ) {
	std::cout << " cut_delta_eta_w_j1_n = " << w_eta() - leading_jet()->eta() << std::endl
		  << " cut_delta_phi_w_j1 = " << detector::delta_phi( AnaW::phi() , leading_jet()->phi() ) << std::endl
		  << " cut_delta_eta_w_j2_n = " << w_eta() - far_jet()->eta() << std::endl
		  << " cut_delta_eta_j1_j1 = " << leading_jet()->eta() - far_jet()->eta() << std::endl
		  << " cut_delta_phi_lep_nu = " << detector::delta_phi( lepton()->phi() , met()->phi() ) << std::endl
		  << "   bdt_response_400 = " << bdt_response( "BDT method W 400" ) << std::endl
		  << "   bdt_response_900 = " << bdt_response( "BDT method W 900" ) << std::endl
		  << "   bdt_response_1200 = " << bdt_response( "BDT method W 1200" ) << std::endl;
      }
    }
  }

  const bool bdt_CR( const std::string& bdtmethod = "BDT method W 900" , const double& cut = -0.4 ) const { return bdt_common() && bdt_response(bdtmethod) < cut; }
  const bool bdt_SR( const std::string& bdtmethod = "BDT method W 900" , const double& cut = -0.4 ) const { return bdt_common() && bdt_response(bdtmethod) > cut; }
  const bool opt_400() const { return false; }
  const bool opt_900() const {
    float this_w_eta = float(w_eta());
    float j2_eta = far_jet()->eta();
    if( std::fabs( this_w_eta - leading_jet()->eta() ) >= 2.0 ) return false;
    if( std::fabs( detector::delta_phi(AnaW::phi(),leading_jet()->phi()) ) <= 2.8 ) return false;
    if( std::fabs( this_w_eta - j2_eta ) <= 1.8 ) return false;
    if( std::fabs( leading_jet()->eta() - j2_eta ) <= 2.2 ) return false;
    if( std::fabs( detector::delta_phi( lepton()->phi() , met()->phi() ) ) >= 0.7 ) return false;
    return true;
  }
  const bool opt_1200() const {
    float this_w_eta = float(w_eta());
    float j2_eta = far_jet()->eta();
    if( std::fabs( this_w_eta - leading_jet()->eta() ) >= 2.5 ) return false;
    if( std::fabs( detector::delta_phi(AnaW::phi(),leading_jet()->phi()) ) <= 2.4 ) return false;
    if( std::fabs( this_w_eta - j2_eta ) <= 0.8 ) return false;
    if( std::fabs( leading_jet()->eta() - j2_eta ) <= 1.4 ) return false;
    if( std::fabs( detector::delta_phi( lepton()->phi() , met()->phi() ) ) >= 0.7 ) return false;
    return true;
  }
  const bool opt_400_SR() const {
    if( !bdt_common() ) return false;
    if( !opt_400() ) return false;
    return true;
  }
  const bool opt_400_CR() const {
    if( !bdt_common() ) return false;
    if( opt_400() ) return false;
    return true;
  }
  const bool opt_900_SR() const {
    if( !bdt_common() ) return false;
    if( !opt_900() ) return false;
    return true;
  }
  const bool opt_900_CR() const {
    if( !bdt_common() ) return false;
    if( opt_900() ) return false;
    return true;
  }
  const bool opt_1200_SR() const {
    if( !bdt_common() ) return false;
    if( !opt_1200() ) return false;
    return true;
  }
  const bool opt_1200_CR() const {
    if( !bdt_common() ) return false;
    if( opt_1200() ) return false;
    return true;
  }

    
  const bool highdetamet_SR() const { 
    return ( met() && lepton() && njets()>1 ) ?
      ( met()->pt()>50. && leading_jet()->pt()>50. && std::fabs(dphi_lepton_met())<2.4 && std::fabs(deta_leadingjet_associatedjet())>1. && mt()>40. ) : false;
  }
  const bool base_SR() const {
    return bdt_common();
    if( !met() ) return false;
    if( !lepton() ) return false;
    if( njets() < 2 ) return false;
    if( mt() <= 40. ) return false;
    if( met()->pt() <= 50. ) return false;
    if( leading_jet()->pt() <= 60. ) return false;
  }
  const bool rectangle3_base_SR() const {
    if( !bdt_common_base() ) return false;
    if( fabs(dphi_lepton_met()) >= 1.5 ) return false;
    if( fabs(dphi_w_leadingjet()) <= 2.5 ) return false;
    //if( fabs(deta_w_leadingjet()) >= 2 ) return false;
    return true;
  }
  const bool rectangle3_SR() const {
    if( !bdt_common() ) return false;
    if( fabs(dphi_lepton_met()) >= 1.5 ) return false;
    if( fabs(dphi_w_leadingjet()) <= 2.5 ) return false;
    if( fabs(deta_w_leadingjet()) >= 2 ) return false;
    return true;
  }
  const bool rectangle6_SR() const {
    if( !bdt_common() ) return false;
    if( !rectangle3_SR() ) return false;
    if( fabs(deta_w_farjet()) >= 4. ) return false;
    if( fabs(deta_leadingjet_farjet()) <=2. ) return false;
    return true;
  }
  const bool rectangle6_SR_aj() const {
    if( !bdt_common() ) return false;
    if( !rectangle3_SR() ) return false;
    if( fabs(deta_w_associatedjet()) <= 4. ) return false;
    if( fabs(deta_leadingjet_associatedjet()) <= 2. ) return false;
    return true;
  }
  const unsigned int jetbin() const { return njets()<3 ? njets() : 3; }
  const unsigned int vtxbin() const { return nvtxs()<10 ? nvtxs() : 10; }

  
  // list all cuts
  const bool asym_iso_default() const {
    return muon() ? muon()->pt_cone_ratio(AnaMuon::CONE40) < 0.1 : true ;
  }
  const bool asym_iso_ww() const {
    return  muon() ? muon()->pt_cone_ratio(AnaMuon::CONE40) < 0.1 && muon()->et_cone_corr_ratio(AnaMuon::CONE40,nvtxs_all()) < 0.1 : true ;
  }
  const bool asym_quality() const {
    return muon() ? muon()->passes_hits_mcp_v17_moriond() && std::abs(muon()->exPV_z0())<10. : true ;
  }
  const bool asym_kinematics() const {
    return lepton() ? lepton()->pt()>20. && std::abs(lepton()->eta())<2.4 : false ;
  }
  const bool asym_kinematics25() const {
    return lepton() ? lepton()->pt()>25. && std::abs(lepton()->eta())<2.4 : false ;
  }
  const bool asym_eta() const {
    return lepton() ? std::abs(lepton()->eta())<2.4 : false ;
  }
  const bool asym_lowpt() const {
    return lepton() ? lepton()->pt()>15. && lepton()->pt()<20. : false ;
  }
  const bool asym_met() const {
    return met() ? met()->pt()>25. : false ;
  }
  const bool asym_wmt() const {
    return met() ? transverse_mass()>40. : false ;
  }
  const bool asym_d0sig() const {
    return muon() ? std::abs(muon()->exPV_d0_signif())<5.0 : true ;
  }
  const bool asym_trigger() const {
    return muon() ? muon()->passes_trigger_matching_v17() : true ;
  }
  const bool asym_nmuons() const {
    return atmost_one_lepton();
  }
  // excluding bad eta regions that have trigger problems
  const bool asym_eta_trig() const {
    if( !lepton() ) return false;
    AnaParticle::ftype e = lepton()->eta();
    const short q = lepton()->scharge();
    if( q>0 && e>-1.62 && e<-1.56 ) return false; // mu+ bin 8
    if( q>0 && e>-2.15 && e<-2.09 ) return false; // mu+ bin 10
    if( q<0 && e>-2.13 && e<-2.06 ) return false; // mu- bin 10
    return true;
  }
  // baseline for the analysis [W/Z isolation]
  const bool asym_baseline() const {
    return asym_nmuons() && asym_quality() && asym_kinematics() && asym_iso_default() && asym_met() && asym_wmt();
  }
  const bool asym_baseline25() const {
    return asym_nmuons() && asym_quality() && asym_kinematics25() && asym_iso_default() && asym_met() && asym_wmt();
  }
  // version of baseline and metfit assuming the isolation and kinematic cuts have already been applied
  // asym_eta_trig() is not applied here - this way, it will be reverted via unfolding.
  // ATTN: if Jan wants to do extrapolation over these cut-out regions, asym_eta_trig must be added here.
  const bool asym_baseline_truth() const {
    return asym_kinematics() && asym_met() && asym_wmt();
  }
  const bool asym_baseline_truth25() const {
    return asym_kinematics25() && asym_met() && asym_wmt();
  }
  // same as asym_baseline but avoiding un-necessary work that has already been performed elsewhere
  const bool asym_baseline_quick() const {
    return asym_nmuons() && asym_met() && asym_wmt();
  }
  const bool asym_metfit_quick() const {
    return asym_nmuons() && asym_wmt();
  }
  const bool asym_wmtfit_quick() const {
    return asym_nmuons() && asym_met();
  }
  const bool asym_anyfit_quick() const {
    const bool passwmt =  met() ? transverse_mass()>0. : false ;
    return passwmt && asym_nmuons();  // no met OR wmt cut applied
  }
  const bool asym_anyfit20_quick() const {
    const bool passwmt =  met() ? transverse_mass()>20. : false ;
    return passwmt && asym_nmuons();
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
  
  // old interface - used to build w candidates directly from components
  // it constructs a W from the first lepton in the lepton collection
  template<typename iteratorLepT,typename iteratorMetT,typename iteratorJetT,typename iteratorVtxT>
  static const boost::shared_ptr<AnaWCrossSectionCandidate> build( iteratorLepT begin_leptons , iteratorLepT end_leptons ,
                                                                   iteratorMetT begin_mets , iteratorMetT end_mets ,
                                                                   iteratorJetT begin_jets , iteratorJetT end_jets ,
                                                                   iteratorVtxT begin_vtxs , iteratorVtxT end_vtxs , 
								   const unsigned long& event_number = 0 , const bool& is_mc = true ) {
    if( begin_leptons==end_leptons ) { return boost::shared_ptr<AnaWCrossSectionCandidate>(); }
    if( begin_mets==end_mets ) { return boost::shared_ptr<AnaWCrossSectionCandidate>(); }
    assert( std::distance( begin_mets , end_mets ) >= 1 );
    assert( std::distance( begin_leptons , end_leptons ) >=1 );
    boost::shared_ptr<AnaWCrossSectionCandidate> result(new AnaWCrossSectionCandidate(boost::shared_ptr<AnaW>(new AnaW( *begin_leptons , *begin_mets ))));
    assert( result );
    result->_event_number = event_number;
    result->_is_mc = is_mc;
    // leptons
    result->_event_leptons.assign( begin_leptons , end_leptons );
    std::sort( result->_event_leptons.begin() , result->_event_leptons.end() , boost::bind(&AnaParticle::pt,_1) > boost::bind(&AnaParticle::pt,_2) );
    // met
    result->_event_met.assign( begin_mets , end_mets );
    // jets
    result->_event_jets.assign( begin_jets , end_jets );
    std::sort( result->_event_jets.begin() , result->_event_jets.end() , boost::bind(&AnaParticle::pt,_1) > boost::bind(&AnaParticle::pt,_2) );
    // vertices
    result->_event_vertices.assign( begin_vtxs , end_vtxs );
    assert( !(result->_event_leptons.empty()) );
    assert( !(result->_event_met.empty()) );
    return result;
  }

  template<typename iteratorLepT,typename iteratorMetT,typename iteratorJetT,typename iteratorVtxT,typename iteratorTruthT>
  static const boost::shared_ptr<AnaWCrossSectionCandidate> build( iteratorLepT begin_leptons , iteratorLepT end_leptons ,
                                                                   iteratorMetT begin_mets , iteratorMetT end_mets ,
                                                                   iteratorJetT begin_jets , iteratorJetT end_jets ,
                                                                   iteratorVtxT begin_vtxs , iteratorVtxT end_vtxs , 
								   iteratorTruthT begin_vlq_truth , iteratorTruthT end_vlq_truth ,
								   iteratorTruthT begin_w_truth , iteratorTruthT end_w_truth ,
								   iteratorTruthT begin_nu_truth , iteratorTruthT end_nu_truth ,
								   iteratorTruthT begin_fj_truth , iteratorTruthT end_fj_truth ,
								   const unsigned long& event_number = 0 , const bool& is_mc = true ) {
    if( begin_leptons==end_leptons ) { return boost::shared_ptr<AnaWCrossSectionCandidate>(); }
    if( begin_mets==end_mets ) { return boost::shared_ptr<AnaWCrossSectionCandidate>(); }
    assert( std::distance( begin_mets , end_mets ) >= 1 );
    assert( std::distance( begin_leptons , end_leptons ) >=1 );
    boost::shared_ptr<AnaWCrossSectionCandidate> result(new AnaWCrossSectionCandidate(boost::shared_ptr<AnaW>(new AnaW( *begin_leptons , *begin_mets ))));
    assert( result );
    result->_event_number = event_number;
    result->_is_mc = is_mc;
    // leptons
    result->_event_leptons.assign( begin_leptons , end_leptons );
    std::sort( result->_event_leptons.begin() , result->_event_leptons.end() , boost::bind(&AnaParticle::pt,_1) > boost::bind(&AnaParticle::pt,_2) );
    // met
    result->_event_met.assign( begin_mets , end_mets );
    // jets
    result->_event_jets.assign( begin_jets , end_jets );
    std::sort( result->_event_jets.begin() , result->_event_jets.end() , boost::bind(&AnaParticle::pt,_1) > boost::bind(&AnaParticle::pt,_2) );
    // vertices
    result->_event_vertices.assign( begin_vtxs , end_vtxs );
    // VLQ truth particle
    result->_event_vlq_truth.assign( begin_vlq_truth , end_vlq_truth );
    // W truth particle
    result->_event_w_truth.assign( begin_w_truth , end_w_truth );
    // neutrino truth particle
    result->_event_nu_truth.assign( begin_nu_truth , end_nu_truth );
    // forward jet truth particle
    result->_event_fj_truth.assign( begin_fj_truth , end_fj_truth );
    assert( !(result->_event_leptons.empty()) );
    assert( !(result->_event_met.empty()) );
    return result;
  }

  // new interface - uses pre-built w's
  template<typename iteratorLepT,typename iteratorMetT,typename iteratorJetT,typename iteratorVertexT>
  class Compose {
  private:
    iteratorLepT _begin_leptons, _end_leptons;
    iteratorMetT _begin_mets, _end_mets;
    iteratorJetT _begin_jets, _end_jets;
    iteratorVertexT _begin_vertices, _end_vertices;
  public:
    typedef boost::shared_ptr<const AnaWCrossSectionCandidate> result_type;
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
    result_type operator()( const boost::shared_ptr<const AnaW>& w ) const {
      boost::shared_ptr<AnaWCrossSectionCandidate> result( new AnaWCrossSectionCandidate(w) ); assert( result );
      result->_event_leptons.assign( _begin_leptons , _end_leptons );
      result->_event_met.assign( _begin_mets , _end_mets );
      result->_event_jets.assign( _begin_jets , _end_jets );
      result->_event_vertices.assign( _begin_vertices , _end_vertices );
      std::sort( result->_event_leptons.begin() , result->_event_leptons.end() , boost::bind( &AnaParticle::pt , _1 ) > boost::bind( &AnaParticle::pt , _2 ) );
      std::sort( result->_event_jets.begin() , result->_event_jets.end() , boost::bind( &AnaParticle::pt , _1 ) > boost::bind( &AnaParticle::pt , _2 ) );
      return result;
    }
  };


  // new interface - uses pre-built w's; allows to specify an additional vertex collection
  template<typename iteratorLepT,typename iteratorMetT,typename iteratorJetT,typename iteratorVertexT>
  class Compose2 {
  private:
    iteratorLepT _begin_leptons, _end_leptons;
    iteratorMetT _begin_mets, _end_mets;
    iteratorJetT _begin_jets, _end_jets;
    iteratorVertexT _begin_vertices, _end_vertices;
    iteratorVertexT _begin_all_vertices, _end_all_vertices;
  public:
    typedef boost::shared_ptr<const AnaWCrossSectionCandidate> result_type;
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
    result_type operator()( const boost::shared_ptr<const AnaW>& w ) const {
      boost::shared_ptr<AnaWCrossSectionCandidate> result( new AnaWCrossSectionCandidate(w) ); assert( result );
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

BOOST_CLASS_VERSION( AnaWCrossSectionCandidate , 4 );

#endif // ANAWCROSSSECTIONCANDIDATE_HPP
