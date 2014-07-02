 
#ifndef STUDYVLQ_HPP
#define STUDYVLQ_HPP

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
#include "TrigFTKAna/AnaVLQCandidate.hpp"
#include "TrigFTKAna/AnaParticle.hpp"
#include "TrigFTKAna/AnaMET.hpp"
#include "TrigFTKAna/AnaW.hpp"
#include "TrigFTKAna/AnaMuon.hpp"
#include "TrigFTKAna/AnaElectron.hpp"
#include "TrigFTKAna/Detector.hpp"

class
StudyVLQ : public DataGraphics::DgStudy< boost::shared_ptr<const AnaVLQCandidate> >
{
public:
  typedef boost::shared_ptr<const AnaVLQCandidate> type; // must be same type as the template argument for DgStudy.
protected:
  // a wrapper implementing (optional) charge separation
  virtual void _study( type const& vlqevent ) {
    using namespace DataGraphics;

    study_do(vlqevent);

    if(_do_charge_separation) {
      if(vlqevent->scharge()>0) {
	dg::down( "POS" , "W+ candidates" ); BOOST_SCOPE_EXIT(void) { dg::up(); } BOOST_SCOPE_EXIT_END;
	study_do(vlqevent);
      }
      if(vlqevent->scharge()<0) {
	dg::down( "NEG" , "W- candidates" ); BOOST_SCOPE_EXIT(void) { dg::up(); } BOOST_SCOPE_EXIT_END;
	study_do(vlqevent);
      }
    }

    if( _do_eta_separation && vlqevent->nelectrons()>0 ) {
      if( fabs(vlqevent->lepton_eta()) >= 1.9 ) {
	dg::down( "ENDCAP" , "leptons with |eta| >= 1.9" ); BOOST_SCOPE_EXIT(void) { dg::up(); } BOOST_SCOPE_EXIT_END;
	study_do(vlqevent);
      } else {
	dg::down( "BARREL" , "leptons with |eta| < 1.9" ); BOOST_SCOPE_EXIT(void) { dg::up(); } BOOST_SCOPE_EXIT_END;
	study_do(vlqevent);
      }
    }

  }
  // actual study
  virtual void study_do( type const& vlqevent ) {
    using namespace std;
    using namespace boost;
    using namespace DataGraphics;
    // particle counts
    dg::fillh( "total" , 1 , 1 , 2 , 1 );
    dg::fillhw( "total_raw" , 1 , 1 , 2 , 1 , 1. );
    dg::fillhw( "weight" , 100 , 0 , 5 , dg::global_weight() , 1. );
    dg::fillh( "njets" , 20 , 0 , 20 , vlqevent->njets() );
    dg::fillh( "nvtxs" , 40 , 0 , 40 , vlqevent->nvtxs() );
    dg::fillh( "average_mu" , 40 , 0 , 40 , vlqevent->average_mu() );
    if( vlqevent->average_mu() <= 6 ) {
      dg::fillh( "njets_lowmu" , 20 , 0 , 20 , vlqevent->njets() );
    } else if( vlqevent->average_mu() <= 12 ) {
      dg::fillh( "njets_midmu" , 20 , 0 , 20 , vlqevent->njets() );
    } else {
      dg::fillh( "njets_highmu" , 20 , 0 , 20 , vlqevent->njets() );
    }
    dg::fillh( "nelectrons" , 10 , 0 , 10 , vlqevent->nelectrons() );
    dg::fillh( "nmuons" , 10 , 0 , 10 , vlqevent->nmuons() );
    dg::fillh( "nleptons" , 10 , 0 , 10 , vlqevent->nleptons() );
    // lepton
    dg::fillh( "lepton_pt" , 500 , 0 , 500 , vlqevent->lepton_pt() );
    dg::fillh( "lepton_phi" , 100 , -M_PI , M_PI , vlqevent->lepton_phi() );
    dg::fillh( "lepton_eta" , 100 , -4 , 4 , vlqevent->lepton_eta() );
    // met
    dg::fillh( "met_pt" , 500 , 0 , 500 , vlqevent->met_pt() );
    dg::fillh( "met_phi" , 100 , -M_PI , M_PI , vlqevent->met_phi() );
    dg::fillh( "met_eta" , 100 , -5 , 5 , vlqevent->met_eta() );
    // W
    dg::fillh( "w_pt" , 500 , 0 , 500 , vlqevent->w_pt() );
    dg::fillh( "w_phi" , 100 , -M_PI , M_PI , vlqevent->w_phi() );
    dg::fillh( "w_eta" , 100 , -5 , 5 , vlqevent->w_eta() );
    dg::fillh( "w_mt" , 500 , 0 , 500 , vlqevent->w_mt() );
    // VLQ
    dg::fillh( "vlq_m" , 3000 , 0 , 3000 , vlqevent->vlq_m_detawj() );
    if( vlqevent->has_vlq_truth() && vlqevent->has_v_truth() ) {
      dg::fillh( "vlq_m_truth" , 3000 , 0 , 3000 , vlqevent->vlq_m_truth() );
      dg::fillh( "vlq_m_matched" , 3000 , 0 , 3000 , vlqevent->vlq_m_matched() );
      dg::fillh( "vlq_m_small" , 3000 , 0 , 3000 , vlqevent->vlq_m_small() );
      dg::fillh( "vlq_m_detalv" , 3000 , 0 , 3000 , vlqevent->vlq_m_detalv() );
      dg::fillh( "vlq_detawj_res" , 4000 , -4000 , 4000 , vlqevent->vlq_m_detawj() - vlqevent->vlq_m_truth() );
      dg::fillh( "vlq_detalv_res" , 4000 , -4000 , 4000 , vlqevent->vlq_m_detalv() - vlqevent->vlq_m_truth() );
      dg::fillh( "vlq_small_res" , 4000 , -4000 , 4000 , vlqevent->vlq_m_small() - vlqevent->vlq_m_truth() );
      dg::fillh( "vlq_matched_res" , 4000 , -4000 , 4000 , vlqevent->vlq_m_matched() - vlqevent->vlq_m_truth() );
    }
    // jets
    dg::fillh( "farjet_pt" , 500 , 0 , 500 , vlqevent->farjet_pt() );
    dg::fillh( "farjet_phi" , 100 , -M_PI , M_PI , vlqevent->farjet_phi() );
    dg::fillh( "farjet_eta" , 100 , 5 , 5 , vlqevent->farjet_eta() );
    for( short i = 0 ; i < vlqevent->njets() ; ++i ) {
      std::string istr = boost::lexical_cast<std::string>(i);
      dg::fillh( "jet"+istr+"_pt" , 500 , 0 , 500 , vlqevent->jet(i)->pt() );
      dg::fillh( "jet"+istr+"_phi" , 100 , -M_PI , M_PI , vlqevent->jet(i)->phi() );
      dg::fillh( "jet"+istr+"_eta" , 100 , -5 , 5 , vlqevent->jet(i)->eta() );
    }
    // phi separations
    dg::fillh( "dphi_lepton_met" , 100 , -M_PI , M_PI , vlqevent->dphi_lepton_met() );
    dg::fillh( "dphi_lepton_leadingjet" , 100 , -M_PI , M_PI, vlqevent->dphi_lepton_leadingjet() );
    dg::fillh( "dphi_met_leadingjet" , 100 , -M_PI , M_PI , vlqevent->dphi_met_leadingjet() );
    dg::fillh( "dphi_w_leadingjet" , 100 , -M_PI , M_PI , vlqevent->dphi_w_leadingjet() );
    dg::fillh( "dphi_leadingjet_farjet" , 100 , -M_PI , M_PI , vlqevent->dphi_leadingjet_farjet() );
    // eta separations
    dg::fillh( "deta_leadingjet_farjet" , 100 , -8 , 8 , vlqevent->deta_leadingjet_farjet() );
    dg::fillh( "deta_lepton_leadginjet" , 100 , -8 , 8 , vlqevent->deta_lepton_leadingjet() );
    dg::fillh( "deta_w_leadingjet" , 100 , -8 , 8 , vlqevent->deta_w_leadingjet() );
    dg::fillh( "deta_w_farjet" , 100 , -8 , 8 , vlqevent->deta_w_farjet() );
    // vector sums
    dg::fillh( "vectorsum_jet_m" , 500 , 0 , 500 , vlqevent->vecsum_jet_m() );
    dg::fillh( "vectorsum_jet_pt" , 500 , 0 , 500 , vlqevent->vecsum_jet_pt() );
    // scalar sums
    dg::fillh( "scalarsum_jet_m" , 500 , 0 , 500 , vlqevent->scalarsum_jet_m() );
    dg::fillh( "scalarsum_jet_pt" , 500 , 0 , 500 , vlqevent->scalarsum_jet_pt() );
    dg::fillh( "scalarsum_visible_m" , 500 , 0 , 500 , vlqevent->scalarsum_visible_m() );
    dg::fillh( "scalarsum_visible_pt" , 500 , 0 , 500 , vlqevent->scalarsum_visible_pt() );
    dg::fillh( "scalarsum_pt" , 500 , 0 , 500 , vlqevent->scalarsum_pt() );
    // plot the yield if flag is set to true
    if( _do_yield ) {
      dg::fillhw( "yield" , 
		  int( _yield_max - _yield_min ) , _yield_min , _yield_max ,
		  _yield_runnumber , dg::global_weight() / _yield_lumi , "run number" );
    }
  } // end study VLQ event candidate
protected:
  boost::shared_ptr<const AnaEvent> _event;
  bool _do_xcheck_studies;
  bool _do_2d_plots;
  bool _do_charge_separation;
  bool _do_eta_separation;
  bool _do_yield;
  unsigned long _yield_min;
  unsigned long _yield_max;
  unsigned long _yield_runnumber;
  float _yield_lumi;
  unsigned long _run_number;
public:
  void do_xcheck_studies( bool v = true ) { _do_xcheck_studies = v; }
  void do_2d_plots( bool v = true ) { _do_2d_plots = v; }
  void do_charge_separation( bool v = true ) { _do_charge_separation = v; }
  void do_eta_separation( bool v = true ) { _do_eta_separation = v; }
  void do_yield(bool v = true) { _do_yield = v; }
  void do_yield( const unsigned long& min , const unsigned long& max , const unsigned long& runnumber , const float& lumi ) { 
    _do_yield = true; _yield_min = min ; _yield_max = max ; _yield_runnumber = runnumber; _yield_lumi = lumi; 
  }
  StudyVLQ( const std::string& name , const std::string& description ) 
    : _event()
    , _do_xcheck_studies(false)
    , _do_2d_plots(true)
    , _do_charge_separation(false)
    , _do_eta_separation(false)
    , _do_yield(false)
    , _yield_min( 0. )
    , _yield_max( 1. )
    , _yield_runnumber( 0. )
    , _yield_lumi( 1. )
    , _run_number(0)
    , DataGraphics::DgStudy<type>(name,description)
  {}
  virtual ~StudyVLQ() {}
  void for_event( const boost::shared_ptr<const AnaEvent>& event ) { _event = event; }
  void for_run( unsigned long rnum ) { _run_number = rnum; }
};

#endif // STUDYWCROSSECTION_HPP
