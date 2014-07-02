 
#ifndef STUDYMONOJET_HPP
#define STUDYMONOJET_HPP

#include <cmath>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <numeric>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/format.hpp>
#include <boost/scope_exit.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/assign/std/vector.hpp>
#include "DataGraphics/DgStudy.hh"
#include "DataGraphics/DgCollection.hh"
#include "TrigFTKAna/AnaMonojetCandidate.hpp"
#include "TrigFTKAna/AnaParticle.hpp"
#include "TrigFTKAna/AnaMET.hpp"
#include "TrigFTKAna/AnaMuon.hpp"
#include "TrigFTKAna/AnaElectron.hpp"

#include "TrigFTKAna/Detector.hpp"

class
StudyMonojet : public DataGraphics::DgStudy< boost::shared_ptr<const AnaMonojetCandidate> >
{
public:
  typedef boost::shared_ptr<const AnaMonojetCandidate> type; // must be same type as the template argument for DgStudy.
protected:
  // a wrapper implementing (optional) charge separation
  virtual void _study( type const& mjevent ) {
    using namespace DataGraphics;
    study_do(mjevent);

    if( mjevent->leading_jet_is_quark() ) {
      dg::down( "QUARKJET" , "" ); BOOST_SCOPE_EXIT(void) { dg::up(); } BOOST_SCOPE_EXIT_END;
      study_do(mjevent);
    }

    if( mjevent->leading_jet_is_gluon() ) {
      dg::down( "GLUONJET" , "" ); BOOST_SCOPE_EXIT(void) { dg::up(); } BOOST_SCOPE_EXIT_END;
      study_do(mjevent);
    }

  }
  // actual study
  virtual void study_do( type const& mjevent ) {
    using namespace std;
    using namespace boost;
    using namespace DataGraphics;
    const boost::shared_ptr<const AnaMonojetCandidate>& mj = mjevent;
    dg::fillh( "total" , 1 , 1 , 2 , 1 );
    if( mj->leading_jet_is_quark() ) dg::fillh( "total_quark_leadingjet" , 1 , 1 , 2 , 1 );
    if( mj->leading_jet_is_gluon() ) dg::fillh( "total_gluon_leadingjet" , 1 , 1 , 2 , 1 );
    if( mj->leading_jet_is_untagged() ) dg::fillh( "total_untagged_leadingjet" , 1 , 1 , 2 , 1 );
    // plot object counts
    dg::fillh( "njets" , 20 , 0 , 20 , mj->njets() , "N_{jets}" );
    dg::fillh( "nlepjets" , 20 , 0 , 20 , mj->nlepjets() , "N_{lepjets}" );
    dg::fillh( "njetoverlaps", 10 , 0 , 10 , mj->njetoverlaps() , "N_{jet overlaps}" );
    dg::fillh( "nelectrons" , 20 , 0 , 20 , mj->nelectrons() , "N_{electrons}" );
    dg::fillh( "nleptons" , 20 , 0 , 20 , mj->nleptons() , "N_{leptons}" );
    dg::fillh( "nlooseleptons" , 20 , 0 , 20 , mj->nlooseleptons() , "N_{leptons}" );
    dg::fillh( "nmuons" , 20 , 0 , 20 , mj->nmuons() , "N_{muons}" );
    dg::fillh( "nloosemuons" , 20 , 0 , 20 , mj->nloosemuons() , "N_{muons}" );
    dg::fillh( "nvtxs" , 50 , 0 , 50 , mj->nvtxs() , "N_{vtxs}" );
    dg::fillh( "average_mu" , 50 , 0 , 50 , mj->average_mu() , "<#mu>" );
    dg::fillh( "bdff" , 100 , 0 , 100 , mj->bdff() , "bunch distance from front of train" );
    // scalar sums
    /*
    dg::fillh( "scalarsum_jet_pt" , 2000 , 0 , 2000 , mj->scalarsum_jet_pt() );
    dg::fillh( "scalarsum_lepton_pt" , 2000 , 0 , 2000 , mj->scalarsum_lepton_pt() );
    dg::fillh( "scalarsum_visible_pt" , 2000 , 0 , 2000 , mj->scalarsum_visible_pt() );
    dg::fillh( "scalarsum_pt" , 2000 , 0 , 2000 , mj->scalarsum_pt() );
    dg::fillh( "scalarsum_jet_m" , 2000 , 0 , 2000 , mj->scalarsum_jet_m() );
    dg::fillh( "scalarsum_lepton_m" , 2000 , 0 , 2000 , mj->scalarsum_lepton_m() );
    dg::fillh( "scalarsum_visible_m" , 2000 , 0 , 2000 , mj->scalarsum_visible_m() );
    */
    if( mj->met() ) {
      dg::fillh( "met_pt" , 2000 , 0 , 2000 , mj->met_pt() , "E^{miss}_{T} [GeV]" );
      dg::fillh( "met_phi" , 200 , -M_PI , M_PI , mj->met_phi() , "#phi (E^{miss}_{T}) [rad]" );
      dg::fillh( "met_xs" , 500 , 0 , 100 , mj->met()->xs() );
      dg::fillh( "lepmet_pt" , 2000 , 0 , 2000 , mj->lepmet_pt() , "p_{T} (E^{miss}_{T},leptons) [GeV]" );
      dg::fillh( "lepmet_phi" , 200 , -M_PI , M_PI , mj->lepmet_phi() , "#phi (E^{miss}_{T},leptons) [GeV]" );
    }
    if( mj->met_reffinal() ) {
      dg::fillh( "met_reffinal_pt" , 2000 , 0 , 2000 , mj->met_reffinal_pt() , "E^{miss}_{T} [GeV]" );
      dg::fillh( "met_reffinal_phi" , 200 , -M_PI , M_PI , mj->met_reffinal_phi() , "#phi (E^{miss}_{T}) [rad]" );
    }
    if( mj->met_reffinalcorr() ) {
      dg::fillh( "met_reffinalcorr_pt" , 2000 , 0 , 2000 , mj->met_reffinalcorr_pt() );
      dg::fillh( "met_reffinalcorr_phi" , 200 , -M_PI , M_PI , mj->met_reffinalcorr_phi() );
    }
    if( mj->nelectrons() ) {
      dg::fillh( "leading_electron_pt" , 500 , 0 , 500 , mj->leading_electron_pt() , "p_{T} (leading e) [GeV]" );
      dg::fillh( "leading_electron_phi" , 200 , -M_PI , M_PI , mj->leading_electron_phi() , "#phi (leading e) [GeV]" );
      dg::fillh( "leading_electron_eta" , 200 , -3 , 3 , mj->leading_electron_eta() , "#eta (leading e)" );
    }
    if( mj->nmuons() ) {
      dg::fillh( "leading_muon_pt" , 500 , 0 , 500 , mj->leading_muon_pt() , "p_{T} (leading #mu) [GeV]" );
      dg::fillh( "leading_muon_phi" , 200 , -M_PI , M_PI , mj->leading_muon_phi() , "#phi (leading #mu) [GeV]" );
      dg::fillh( "leading_muon_eta" , 200 , -3 , 3 , mj->leading_muon_eta() , "#eta (leading #mu)" );
    }
    if( mj->nleptons()==1 ) {
      dg::fillh( "w_pt" , 500 , 0 , 500 , mj->w_pt() , "p_{T} (l,E^{miss}_{T}) [GeV]" );
      dg::fillh( "w_phi" , 200 , -M_PI , M_PI , mj->w_phi() , "#phi (l,E^{miss}_{T}) [rad]" );
      dg::fillh( "w_mt" , 500 , 0 , 500 , mj->w_mt() , "M_{T} (l,E^{miss}_{T}) [GeV]" );
    }
    if( mj->nleptons()==2 && mj->leptons_ossf() ) {
      dg::fillh( "z_pt" , 500 , 0 , 500 , mj->z_pt() , "p_{T} (l,l) [GeV]" );
      dg::fillh( "z_phi" , 200 , -M_PI , M_PI , mj->z_phi() , "#phi (l,l) [rad]" );
      dg::fillh( "z_eta" , 200 , -6 , 6 , mj->z_eta() , "#eta (l,l)" );
      dg::fillh( "z_m" , 500 , 0 , 500 , mj->z_m() , "M (l,l) [GeV]" );
    }
    if( mj->njets() ) {
      dg::fillh( "vecsum_jet_mass" , 2000 , 0 , 2000 , mj->vecsum_jet_mass() , "M (jets) [GeV]" );
      dg::fillh( "vecsum_jet_pt" , 2000 , 0 , 2000 , mj->vecsum_jet_pt() , "p_{T} (jets) [GeV]" );
      dg::fillh( "vecsum_jet_phi" , 200 , -M_PI , M_PI , mj->vecsum_jet_phi() , "#phi (jets) [rad]" );
      dg::fillh( "vecsum_jet_eta" , 200 , -5 , 5 , mj->vecsum_jet_eta() , "#eta (jets)" );
      if( mj->njets() > 1 && mj->met() ) {
	dg::fillh( "dphi_met_jet1" , 200 , -M_PI , M_PI , mj->dphi_met_second_jet() , "#Delta#phi (MET, second jet) [rad]" );
      }
    }
    // plot distributions for each lepton
    for( short i = 0 ; i < mj->nleptons() ; ++i ) {
      if( i > 5 ) break;
      std::string istr = boost::lexical_cast<std::string>(i);
      dg::fillh( "lepton"+istr+"_pt" , 2000 , 0 , 2000 , mj->lepton(i)->pt() , "p_{T} (lepton "+istr+") [GeV]" );
      dg::fillh( "lepton"+istr+"_phi" , 200 , -M_PI , M_PI , mj->lepton(i)->phi() , "#phi (lepton "+istr+") [rad]" );
      dg::fillh( "lepton"+istr+"_eta" , 200 , -5 , 5 , mj->lepton(i)->eta() , "#eta (lepton "+istr+")" );
    } 
    // plot distributions for each jet
    for( short i = 0 ; i < mj->njets() ; ++i ) {
      if( i > 5 ) break;
      std::string istr = boost::lexical_cast<std::string>(i);
      dg::fillh( "jet"+istr+"_pt" , 2000 , 0 , 2000 , mj->jet(i)->pt() , "p_{T} (jet "+istr+") [GeV]" );
      dg::fillh( "jet"+istr+"_phi" , 200 , -M_PI , M_PI , mj->jet(i)->phi() , "#phi (jet "+istr+") [GeV]" );
      dg::fillh( "jet"+istr+"_eta" , 200 , -5 , 5 , mj->jet(i)->eta() , "#eta (jet "+istr+")" );
      dg::fillh( "jet"+istr+"_chf" , 100 , 0 , 1 , mj->jet(i)->chf() );
      dg::fillh( "jet"+istr+"_emf" , 100 , 0 , 1 , mj->jet(i)->emf() );
      dg::fillh( "jet"+istr+"_ntracks" , 50 , 0 , 50 , mj->jet(i)->ntrkjetalg() );
      dg::fillh( "jet"+istr+"_ntracks_allpv" , 50 , 0 , 50 , mj->jet(i)->ntrkjetalg_allpv() );
      dg::fillh( "jet"+istr+"_sumpttrk" , 500 , 0 , 500 , mj->jet(i)->sumpttrk() );
      dg::fillh( "jet"+istr+"_sumpttrk_allpv" , 500 , 0 , 500 , mj->jet(i)->sumpttrk_allpv() );
      dg::fillh( "jet"+istr+"_colowidth" , 500 , 0 , 1 , mj->jet(i)->calowidth() );
      dg::fillh( "jet"+istr+"_trackwidth" , 500 , 0 , 1 , mj->jet(i)->trackwidth() );
      dg::fillh( "jet"+istr+"_trackwidth_allpv" , 500 , 0 , 1 , mj->jet(i)->trackwidth_allpv() );
      dg::fillh( "jet"+istr+"_partonid" , 30 , -8 , 22 , mj->jet_partonid(i) );
    }
    // plot the yield if flag is set to true
    dg::fillhw( "yield" , _run_number_max - _run_number_min , _run_number_min , _run_number_max , mj->run_number() , dg::global_weight()/mj->run_lumi() );
  }
protected:
  boost::shared_ptr<const AnaEvent> _event;
  unsigned long _run_number_min;
  unsigned long _run_number_max;
public:
  StudyMonojet( const std::string& name , const std::string& description , const unsigned long& run_number_min = 0 , const unsigned long& run_number_max = 1 ) 
    : _event()
    , _run_number_min( run_number_min )
    , _run_number_max( run_number_max )
    , DataGraphics::DgStudy<type>(name,description)
  {
  }
  virtual ~StudyMonojet() {}
  void for_event( const boost::shared_ptr<const AnaEvent>& event ) { _event = event; }
  void for_run( unsigned long rnum ) { return; }
};

#endif // STUDYMONOJET_HPP
