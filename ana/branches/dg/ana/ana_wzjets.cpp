#include "TrigFTKAna/AnaCommon.hpp"
#include <set>
#include <string>
#include <vector>
#include <list>
#include <bitset>
#include <algorithm>
#include <functional>
#include <iterator>
#include <cassert>
#include <cmath>
#include <ctime>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/scope_exit.hpp>
#include <boost/foreach.hpp>
#include <boost/format.hpp>
#include <boost/any.hpp>
#include <boost/ref.hpp>
#include <iostream>
#include <iomanip>
#include "TrigFTKAna/AnaConfiguration.hpp"
#include "TrigFTKAna/AnaEventMgr.hpp"
#include "TrigFTKAna/AnaTrack.hpp"
#include "TrigFTKAna/AnaElectron.hpp"
#include "TrigFTKAna/AnaPhoton.hpp"
#include "TrigFTKAna/AnaMuon.hpp"
#include "TrigFTKAna/AnaJet.hpp"
#include "TrigFTKAna/AnaMET.hpp"
#include "TrigFTKAna/AnaTrigger.hpp"
#include "TrigFTKAna/AnaTruthParticle.hpp"
#include "TrigFTKAna/AnaVLQCandidate.hpp"
#include "TrigFTKAna/StudyVLQ.hpp"
#include "TrigFTKAna/EventObjectColl.hpp"
#include "TrigFTKAna/GoodRunList.hpp"
#include "TrigFTKAna/Conference.hpp"
#include "TrigFTKAna/jetetmiss/LArHole_v2_00.hpp"
#include "DataGraphics/DataGraphics.hh"
#include "DataGraphics/DgCutSet.hh"
#include "DataGraphics/DgSTL.hh"
#include "TLorentzVector.h"
#include <stdio.h>

// specialized classes
#include "PileupReweighting/TPileupReweighting.h"
#include "MissingETUtility/METUtility.h"

using namespace std;
using namespace DataGraphics;
using namespace boost;

#define boost_foreach BOOST_FOREACH

// shortcuts for cuts and collections
typedef EventObjectColl< boost::shared_ptr<const AnaTruthParticle> > TPCOL;
typedef DgCutSet< boost::shared_ptr<const AnaTruthParticle> > TPCUT;
typedef EventObjectColl< boost::shared_ptr<const AnaMuon> > MUCOL;
typedef DgCutSet< boost::shared_ptr<const AnaMuon> > MUCUT;
typedef EventObjectColl< boost::shared_ptr<const AnaElectron> > ELCOL;
typedef DgCutSet< boost::shared_ptr<const AnaElectron> > ELCUT;
typedef EventObjectColl< boost::shared_ptr<const AnaJet> > JETCOL;
typedef DgCutSet< boost::shared_ptr<const AnaJet> > JETCUT;
typedef EventObjectColl< boost::shared_ptr<const AnaMET> > METCOL;
typedef DgCutSet< boost::shared_ptr<const AnaMET> > METCUT;
typedef EventObjectColl< boost::shared_ptr<const AnaVertex> > VXCOL;
typedef DgCutSet< boost::shared_ptr<const AnaVertex> > VXCUT;
typedef EventObjectColl< boost::shared_ptr<const AnaVLQCandidate> > VLQCOL;
typedef DgCutSet< boost::shared_ptr<const AnaVLQCandidate> > VLQCUT;

struct RunConfig {
  std::string name;
  std::string electron_calib;
  std::string muon_calib;
  std::string jet_calib;
  std::string met_calib;
  std::string muon_effscale;
  std::string electron_effscale;
  std::string muon_trigscale;
  std::string electron_trigscale;
  RunConfig( std::string n , std::string mucal , std::string elcal , std::string jetcal , std::string metcal , 
	     std::string mueff , std::string mutrig , std::string eleff , std::string eltrig )
    : name( n )
    , muon_calib( mucal )
    , electron_calib( elcal )
    , jet_calib( jetcal )
    , met_calib( metcal )
    , muon_effscale( mueff )
    , muon_trigscale( mutrig )
    , electron_effscale( eleff )
    , electron_trigscale( eltrig )
  {
    std::cout << "Adding RunConfig :: "
              << name << " "
              << muon_calib << " "
              << electron_calib << " "
              << jet_calib << " "
              << met_calib << " "
              << muon_effscale << " "
              << muon_trigscale << " "
              << electron_effscale << " "
              << electron_trigscale << std::endl;
  }
};

const double jet_drmin( boost::shared_ptr<const AnaJet>& jet , JETCOL& jetcoll )
{
  if( jetcoll.size()<2 ) return 0.;
  std::sort( jetcoll.begin() , jetcoll.end() , bind(&AnaParticle::dist_eta_phi,_1,jet) < bind(&AnaParticle::dist_eta_phi,_2,jet) );
  return double( jet->dist_eta_phi( jetcoll[1] ) );
}

// MC11a/MC11b/MC11c via https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/WZElectroweakCommonTopics2011
const bool jet_larhole_veto( JETCOL& jetcoll , const bool& is_mc , const unsigned long& run_number )
{
  if( is_mc && (run_number != 183003) ) { return false; }
  if( !is_mc && ( (run_number < 180614) || (run_number >= 185352) ) ) { return false; }
  BOOST_FOREACH( boost::shared_ptr<const AnaJet> jet , jetcoll ) {
    if( LArHole::simpleVeto( jet->pt() , jet->eta() , jet->phi() , jet->f_BCH_CORR_JET() , jet->f_BCH_CORR_CELL() , 25. ) ) { return true; }
  }
  return false;
}

void el_mu_overlap_removal( ELCOL& elcol , MUCOL& mucol , const float& dr ) {
  BOOST_FOREACH( boost::shared_ptr<const AnaMuon> mu , mucol ) {
    float drmin = std::numeric_limits<float>::max();
    ELCOL::iterator imin = elcol.begin();
    for( ELCOL::iterator i = elcol.begin() , f = elcol.end() ; i!=f ; ++i ) {
      float thisdr = float( detector::delta_eta_phi( mu->id_eta() , mu->id_phi() , (*i)->cluster_eta() , (*i)->cluster_phi() ) );
      if( thisdr < drmin ) {
	drmin = thisdr;
	imin = i;
      }
    }
    if( drmin < dr ) { elcol.erase(imin); }
  }
}

void jet_el_overlap_removal( JETCOL& jetcol , ELCOL& elcol , const float& dr ) {
  BOOST_FOREACH( boost::shared_ptr<const AnaElectron> el , elcol ) {
    float drmin = std::numeric_limits<float>::max();
    JETCOL::iterator imin = jetcol.begin();
    for( JETCOL::iterator i = jetcol.begin() , f = jetcol.end() ; i!=f ; ++i ) {
      float thisdr = float( detector::delta_eta_phi( el->cluster_eta() , el->cluster_phi() , (*i)->em_scale_eta() , (*i)->em_scale_phi() ) );
      if( thisdr < dr ) { 
	drmin = thisdr;
	imin = i;
      }
    }
    if( drmin < dr ) { jetcol.erase(imin); }
  }
}

void mu_jet_overlap_removal( MUCOL& mucol , JETCOL& jetcol , const float& dr ) {
  BOOST_FOREACH( boost::shared_ptr<const AnaJet> jet , jetcol ) {
    float drmin = std::numeric_limits<float>::max();
    MUCOL::iterator imin = mucol.begin();
    for( MUCOL::iterator i = mucol.begin() , f = mucol.end() ; i!=f ; ++i ) {
      float thisdr = float( detector::delta_eta_phi( jet->em_scale_eta() , jet->em_scale_phi() , (*i)->id_eta() , (*i)->id_phi() ) );
      if( thisdr < dr ) { 
	drmin = thisdr;
	imin = i;
      }
    }
    if( drmin < dr ) { mucol.erase(imin); }
  }
}

// random constants
const unsigned long low_run_number = 177531;
const unsigned long high_run_number = 191933;


////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C"
#endif
int
F77_MAIN( int argc , char* argv[] )
{

  AnaConfiguration::configure("ana_wzjets",argc,argv);
  if( !AnaConfiguration::configured() ) { exit(0); }
  dg::root()->no_save_at_exit();
  BOOST_SCOPE_EXIT() { dg::root()->do_save_at_exit(); } BOOST_SCOPE_EXIT_END;

  const int REL = AnaConfiguration::release();
  const CONF::ConfType conf = AnaConfiguration::conf();
  std::cout << "Athena release = " << REL << std::endl;
  std::cout << "Conference code = " << conf << std::endl;

  // define lists of analysis objects for each event. insert each
  // object collection into this list. each EventObjectColl will
  // automatically get cleared at the beginning of each event.

  // Using selections described by the DibosonResonance group
  // Set up 5/25/2011 following the guidelines here:
  // https://twiki.cern.ch/twiki/bin/view/AtlasProtected/DibosonResonances

  // event preselection
  // ================================================================
  GoodRunList grl;
  // Taken from here (2012-02-10):
  // http://atlasdqm.web.cern.ch/atlasdqm/grlgen/StandardModel/WZjets_allchannels_v4/
  grl.add( GoodRunList::ELECTRON , "./good_run_lists/data11_7TeV.periodAllYear_DetStatus-v36-pro10_CoolRunQuery-00-04-08_WZjets_allchannels.xml" );
  grl.add( GoodRunList::MUON ,     "./good_run_lists/data11_7TeV.periodAllYear_DetStatus-v36-pro10_CoolRunQuery-00-04-08_WZjets_allchannels.xml" );

  // define primary vertex selections
  // ================================================================
  VXCOL vtx_coll , vtx_multijes_coll , vtx_jetcalib_coll , first_vtx_coll;

  // COMMON VERTEX CUTS
  VXCUT vtx_sel( "vtx_sel" );
  vtx_sel.add_uu( "ntracks" , bind(&AnaVertex::ntracks,_1)>=3 , bind( &dg::fillh , _1 , 15 , 0 , 15 , bind(&AnaVertex::ntracks,_2)) );
  vtx_sel.add_uu( "z_vtx" , bind(std::ptr_fun<AnaVertex::ftype,AnaVertex::ftype>(std::abs),bind(&AnaVertex::z,_1)) < 200., //mm
		  bind( &dg::fillh , _1 , 90 , -300 , 300 , bind(&AnaVertex::z,_2) ) );
  
  VXCUT vtx_multijes_sel( "vtx_multijes_sel" );
  vtx_multijes_sel.add_uu( "ntracks" , bind(&AnaVertex::ntracks,_1)>=5 , bind( &dg::fillh , _1 , 15 , 0 , 15 , bind(&AnaVertex::ntracks,_2)) );

  VXCUT vtx_jetcalib_sel( "vtx_jetcalib_sel" );
  vtx_jetcalib_sel.based_on( vtx_sel ); // FIXME
  //vtx_jetcalib_sel.add_uu( "ntracks" , bind(&AnaVertex::ntracks,_1)>=2 , bind( &dg::fillh , _1 , 15 , 0 , 15 , bind(&AnaVertex::ntracks,_2)) );
  
  // define truth selections
  // ================================================================
  
  // define electron selections
  // ================================================================
  ELCOL el_calib_coll , el_tight_coll , el_loose_coll , el_qcd_coll;

  // common cuts
  ELCUT el_common_sel( "el_common_sel" );
  el_common_sel.add_uu( "author" , bind(&AnaElectron::author,_1)==1 || bind(&AnaElectron::author,_1)==3 , bind( &dg::fillh , _1 , 10 , 0 , 10 , bind(&AnaElectron::author,_2) ) );
  el_common_sel.add_uu( "is_good_oq" , bind(&AnaElectron::is_good_oq,_1)==1 , bind(&dg::filleff,_1,bind(&AnaElectron::is_good_oq,_2)==1) );
  el_common_sel.add_uu( "mediumpp" , bind(&AnaElectron::is_aod_medium_plusplus,_1) , bind(&dg::filleff,_1,bind(&AnaElectron::is_aod_medium_plusplus,_2)) );
  el_common_sel.add_uu( "cluster_eta" ,
			bind( dgSTL::absF , bind(&AnaElectron::cluster_eta,_1) ) < 2.47 &&
			( bind( dgSTL::absF , bind(&AnaElectron::cluster_eta,_1) ) < 1.37  ||
			  bind( dgSTL::absF , bind(&AnaElectron::cluster_eta,_1) ) > 1.52 ) ,
			bind(&dg::fillh,_1,60,-3,3,bind(&AnaElectron::eta,_2)) );
  el_common_sel.add_uu( "cluster_et_20" , bind(&AnaElectron::cluster_pt,_1) > 20. , bind(&dg::fillh,_1,100,0,100,bind(&AnaElectron::cluster_pt,_2)) );
  el_common_sel.add_uu( "expect_b_layer" , bind(&AnaElectron::expect_b_layer_vlq_flag,_1) != 0 , bind(&dg::fillh,_1,11,-1,10,bind(&AnaElectron::expect_b_layer_vlq_flag,_2)) );
  el_common_sel.add_uu( "z0" , bind(dgSTL::absF,bind(&AnaElectron::z0_vx,_1)) < 1. , bind(&dg::fillh,_1,100,-50,50,bind(&AnaElectron::z0_vx,_2)) );
  el_common_sel.add_uu( "d0" , bind(dgSTL::absF,bind(&AnaElectron::d0_vx_signif,_1)) < 10. , bind(&dg::fillh,_1,100,-50,50,bind(&AnaElectron::d0_vx_signif,_2)) );

  
  
  // CC channel
  ELCUT el_tight_sel( "el_tight_sel" );
  el_tight_sel.based_on( el_common_sel );
  el_tight_sel.add_uu( "etcone_20_ratio" , bind(&AnaElectron::uncorrected_cluster_et_cone_ratio,_1,AnaElectron::CONE20) < 0.1 ,
		   bind(&dg::fillh,_1,100,0,1,bind(&AnaElectron::uncorrected_cluster_et_cone_ratio,_2,AnaElectron::CONE20)) );
  el_tight_sel.add_uu( "cluster_et_25" , bind(&AnaElectron::cluster_pt,_1) > 25. , bind(&dg::fillh,_1,100,0,100,bind(&AnaElectron::cluster_pt,_2)) );
  el_tight_sel.add_uu( "tightpp" , bind(&AnaElectron::is_aod_tight_plusplus,_1) , bind(&dg::filleff,_1,bind(&AnaElectron::is_aod_tight_plusplus,_2)) );

  ELCUT el_loose_sel( "el_loose_sel" );
  el_loose_sel.based_on( el_common_sel );
  el_loose_sel.add_uu( "etcone_20_ratio" , bind(&AnaElectron::uncorrected_cluster_et_cone_ratio,_1,AnaElectron::CONE20) < 0.1 ,
			bind(&dg::fillh,_1,100,0,1,bind(&AnaElectron::uncorrected_cluster_et_cone_ratio,_2,AnaElectron::CONE20)) );

  ELCUT el_qcd_sel( "el_qcd_sel" );
  el_qcd_sel.based_on( el_common_sel );
  el_qcd_sel.add_uu( "etcone_20_ratio" , 
		       bind(&AnaElectron::uncorrected_cluster_et_cone_ratio,_1,AnaElectron::CONE20) < 0.1 ,
		       bind(&dg::fillh,_1,100,0,1,bind(&AnaElectron::uncorrected_cluster_et_cone_ratio,_2,AnaElectron::CONE20)) );
  el_qcd_sel.add_uu( "cluster_et_25" , bind(&AnaElectron::cluster_pt,_1) > 25. , bind(&dg::fillh,_1,100,0,100,bind(&AnaElectron::cluster_pt,_2)) );
  el_qcd_sel.add_uu( "mediumpp_not_tightpp" , bind(&AnaElectron::is_aod_medium_plusplus,_1)==true && bind(&AnaElectron::is_aod_tight_plusplus,_1)==false , 
		       bind(&dg::filleff,_1,bind(&AnaElectron::is_aod_medium_plusplus,_2)==true && bind(&AnaElectron::is_aod_tight_plusplus,_2)==false) );


  // define muon selections
  // ================================================================
  MUCOL mu_calib_coll , mu_loose_coll , mu_tight_coll , mu_qcd_coll;

  // common cuts
  MUCUT mu_common_sel( "mu_common_sel" );
  mu_common_sel.add_uu( "staco" , bind(&AnaMuon::is_aod_staco,_1)==true , bind( &dg::filleff , _1 , _2 ) );
  mu_common_sel.add_uu( "author" , bind(&AnaMuon::primary_author,_1)==1 || bind(&AnaMuon::primary_author,_1)==6 , bind( &dg::fillh , _1 , 10 , 0 , 10 , bind(&AnaMuon::primary_author,_2) ) );
  mu_common_sel.add_uu( "mcp_idhits" , bind(&AnaMuon::passes_hits_mcp,_1,conf)==true, bind(&dg::fillh,_1,50,0,50,bind(&AnaMuon::nhits_silicon,_2)) );
  mu_common_sel.add_uu( "z0" , bind(dgSTL::absF,bind(&AnaMuon::z0_vx,_1)) < 1. , bind(&dg::fillh,_1,100,-700,700,bind(&AnaMuon::z0_exPV,_2)) );
  mu_common_sel.add_uu( "d0" , bind(dgSTL::absF,bind(&AnaMuon::d0_vx_signif,_1)) < 3. , bind(&dg::fillh,_1,100,-50,50,bind(&AnaMuon::d0_exPV_signif,_2)) );
  mu_common_sel.add_uu( "eta" , bind( std::ptr_fun<AnaMuon::ftype,AnaMuon::ftype>(std::abs) , bind(&AnaMuon::eta,_1)) < 2.4 , bind(&dg::fillh,_1,100,-5,5,bind(&AnaMuon::eta,_2)) );
  mu_common_sel.add_uu( "pt" , bind(&AnaMuon::pt,_1) > 25. , bind(&dg::fillh,_1,100,0,100,bind(&AnaMuon::pt,_2)) );

  // CC channel
  MUCUT mu_tight_sel( "mu_tight_sel" );
  mu_tight_sel.based_on( mu_common_sel );
  mu_tight_sel.add_uu( "ptcone_20_ratio" , bind(&AnaMuon::pt_cone_ratio,_1,AnaMuon::CONE20) < 0.1 , bind(&dg::fillh,_1,100,0,1,bind(&AnaMuon::pt_cone_ratio,_2,AnaMuon::CONE20)) );
  mu_tight_sel.add_u( "etcone_30_corr" , bind(&AnaMuon::et_cone_corr_ratio,_1,AnaMuon::CONE30,bind(&AnaEvent::nprimary_vertices,boost::ref(AnaEventMgr::current_event()))) < 0.15 );


  // CC channel qcd selection
  MUCUT mu_qcd_sel( "mu_qcd_sel" );
  mu_qcd_sel.based_on( mu_common_sel );
  mu_qcd_sel.add_uu( "ptcone_20_ratio" , bind(&AnaMuon::pt_cone_ratio,_1,AnaMuon::CONE20) >= 0.1 && bind(&AnaMuon::pt_cone_ratio,_1,AnaMuon::CONE20) < 0.2 , bind(&dg::fillh,_1,100,0,1,bind(&AnaMuon::pt_cone_ratio,_2,AnaMuon::CONE20)) );

  // NC channel
  MUCUT mu_loose_sel( "mu_loose_sel" );
  mu_loose_sel.based_on( mu_common_sel );
  mu_loose_sel.add_uu( "ptcone_20_ratio" , bind(&AnaMuon::pt_cone_ratio,_1,AnaMuon::CONE20) < 0.1 , bind(&dg::fillh,_1,100,0,1,bind(&AnaMuon::pt_cone_ratio,_2,AnaMuon::CONE20)) );
  
  // define jet selections
  // ================================================================
  JETCOL lepjet_calib_coll , lepjet_coll , badlepjet_coll;
  JETCOL lepjet_lc_calib_coll;
  JETCOL jetempty;

  // standard selection
  JETCUT jet_sel( "jet_sel" );
  //jet_sel.based_on( jet_common_sel );
  jet_sel.add_uu( "pt" , bind(&AnaJet::pt,_1) > 25. , bind(&dg::fillh,_1,100,0,100,bind(&AnaJet::pt,_2)) );
  jet_sel.add_uu( "rapidity", bind(dgSTL::absF,bind(&AnaJet::rapidity,_1)) < 4.4 , bind(&dg::fillh,_1,100,-4.5,4.5,bind(&AnaJet::rapidity,_2)) );
  jet_sel.add_uu( "JVF" , bind(dgSTL::absF,bind(&AnaJet::vtx_fraction,_1)) >= 0.75 || bind(&AnaJet::ntrkjetalg,_1) == 0 , bind(&dg::fillh,_1,100,-1,1, bind(&AnaJet::vtx_fraction,_2)) );
  jet_sel.add_uu( "jet_cleaning" , bind(&AnaJet::isBadJet_looser_v17,_1)==false , bind(&dg::fillh,_1,200,0,200,bind(&AnaJet::pt,_2)) );

  // badjet selection for MET cleaning
  JETCUT jet_bad_sel( "jet_bad_sel" );
  //jet_bad_sel.based_on( jet_common_sel );
  jet_bad_sel.add_uu( "pt" , bind(&AnaJet::pt,_1) > 25. , bind(&dg::fillh,_1,100,0,100,bind(&AnaJet::pt,_2)) );
  jet_bad_sel.add_uu( "bad_looser" , bind(&AnaJet::isBadJet_looser_v17,_1)==true , bind(&dg::fillh,_1,200,0,200,bind(&AnaJet::pt,_2)) );
  // no ugly requirement for now

  // MultijetJES jet selection
  JETCUT jet_multijes_sel( "jet_multijes_sel" );
  //jet_multijes_sel.based_on( jet_common_sel );
  jet_multijes_sel.add_uu( "pt" , bind(&AnaJet::pt,_1) > 7. , bind(&dg::fillh,_1,100,0,100,bind(&AnaJet::pt,_2)) );

  // MET selection
  // ================================================================
  METCOL met_reffinal_coll , met_calib_coll;

  // VLQ selection
  // ================================================================
  VLQCOL vlq_calib_coll , vlq_coll;

  VLQCUT vlq_sel( "vlq_sel" );
  vlq_sel.add_uu( "njets_ge_2" , bind(&AnaVLQCandidate::njets,_1) > 1 , bind(&dg::fillh,_1,10,0,10,bind(&AnaVLQCandidate::njets,_2)) );


  // define TRUTH selection
  // ================================================================
  TPCOL truth_vlq_coll;
  TPCOL truth_w_coll;
  TPCOL truth_v_coll;
  TPCOL truth_forwardjet_coll;

  TPCUT truth_vlq_sel( "truth_vlq_sel" );
  truth_vlq_sel.add_u( "pdg_id" , bind(&AnaTruthParticle::pdg_id,_1) > 4000000 || bind(&AnaTruthParticle::pdg_id,_1) < -4000000 );
  truth_vlq_sel.add_u( "status" , bind(&AnaTruthParticle::status,_1)==3 );

  TPCUT truth_wz_signal_sel( "truth_wz_signal_sel" );
  truth_wz_signal_sel.add_u( "pdg_id" , bind(dgSTL::absF,bind(&AnaTruthParticle::pdg_id,_1))==24 || bind(&AnaTruthParticle::pdg_id,_1)==23 );
  truth_wz_signal_sel.add_u( "status" , bind(&AnaTruthParticle::status,_1)==3 );
  truth_wz_signal_sel.add_u( "parent_id" , bind(&AnaTruthParticle::parent_pdg_id,_1) > 4000000 || bind(&AnaTruthParticle::parent_pdg_id,_1) < -4000000 );

  // select W's from W+jets MC
  TPCUT truth_w_sel( "truth_w_sel" );
  // this selects 2 particles -- the leptons and neutrino
  truth_w_sel.add_u( "pdg_id" , bind(dgSTL::absF,bind(&AnaTruthParticle::pdg_id,_1))>=11 && bind(dgSTL::absF,bind(&AnaTruthParticle::pdg_id,_1))<=18 );
  truth_w_sel.add_u( "status" , bind(&AnaTruthParticle::status,_1)==1 ); // final state
  truth_w_sel.add_u( "parent" , bind(&AnaTruthParticle::parent,_1)>0 ); // from hard scatter

  TPCUT truth_v_sel( "truth_v_sel" );
  truth_v_sel.add_u( "pdg_id" , bind(dgSTL::absF,bind(&AnaTruthParticle::pdg_id,_1))==12 || bind(dgSTL::absF,bind(&AnaTruthParticle::pdg_id,_1))==14 );
  truth_v_sel.add_u( "status" , bind(&AnaTruthParticle::status,_1)==1 ); // final state
  truth_v_sel.add_u( "parent" , bind(&AnaTruthParticle::parent,_1)>0 ); // from hard scatter

  TPCUT truth_forwardjet_sel( "truth_forwardjet_sel" );
  truth_forwardjet_sel.add_uu( "parent" , bind(&AnaTruthParticle::parent,_1)==0 , bind(&dg::fillh,_1,10,0,10,bind(&AnaTruthParticle::parent,_2)) );
  truth_forwardjet_sel.add_uu( "status" , bind(&AnaTruthParticle::status,_1)==3 , bind(&dg::fillh,_1,10,0,10,bind(&AnaTruthParticle::status,_2)) );
  truth_forwardjet_sel.add_uu( "pdg_id" , bind(dgSTL::absF,bind(&AnaTruthParticle::pdg_id,_1)) < 6 , bind(&dg::fillh,_1,100,0,100,bind(&AnaTruthParticle::pdg_id,_2)) );
  // two quarks will pass this selection in VLQ signal, the lower pT one should be dropped

  // Study Objects
  // ================================================================
  StudyVLQ stvlq( "" , "" );
  stvlq.do_charge_separation( false );
  stvlq.do_eta_separation( true );
  stvlq.add_category( "wmt" , bind(&AnaVLQCandidate::wmt,_1) );
  stvlq.add_category( "baseline" , bind(&AnaVLQCandidate::cr_base,_1) );
  stvlq.add_category( "qcd_region" , bind(&AnaVLQCandidate::qcd_cr,_1) );
  stvlq.add_category( "wjets_region" , bind(&AnaVLQCandidate::wjets_cr,_1) );
  stvlq.add_category( "signal_base" , bind(&AnaVLQCandidate::sr_base,_1) );
  stvlq.add_category( "signal_400" , bind(&AnaVLQCandidate::opt_400_sr,_1) );
  stvlq.add_category( "signal_400_negative" , bind(&AnaVLQCandidate::opt_400_sr_negative,_1) );
  //stvlq.add_category( "signal_400_base" , bind(&AnaVLQCandidate::opt_400_sr_base,_1) );
  //stvlq.add_category( "signal_400_matched" , bind(&AnaVLQCandidate::opt_400_sr_matched,_1) );
  stvlq.add_category( "control_400" , bind(&AnaVLQCandidate::opt_400_cr,_1) );
  stvlq.add_category( "control2_400" , bind(&AnaVLQCandidate::opt_400_cr2,_1) );
  stvlq.add_category( "control3_400" , bind(&AnaVLQCandidate::opt_400_cr3,_1) );
  for( unsigned int ijet=2; ijet<=3; ijet++ ) {
    std::string jetstr = boost::lexical_cast<std::string>(ijet);
    stvlq.add_category( jetstr+"jets_wmt" , bind(&AnaVLQCandidate::wmt,_1) && bind(&AnaVLQCandidate::jetbin,_1)==ijet );
    stvlq.add_category( jetstr+"jets_baseline" , bind(&AnaVLQCandidate::cr_base,_1) && bind(&AnaVLQCandidate::jetbin,_1)==ijet );
    stvlq.add_category( jetstr+"jets_qcd_region" , bind(&AnaVLQCandidate::qcd_cr,_1) && bind(&AnaVLQCandidate::jetbin,_1)==ijet );
    stvlq.add_category( jetstr+"jets_wjets_region" , bind(&AnaVLQCandidate::wjets_cr,_1) && bind(&AnaVLQCandidate::jetbin,_1)==ijet );
    stvlq.add_category( jetstr+"jets_signal_base" , bind(&AnaVLQCandidate::sr_base,_1) && bind(&AnaVLQCandidate::jetbin,_1)==ijet );
    stvlq.add_category( jetstr+"jets_signal_400" , bind(&AnaVLQCandidate::opt_400_sr,_1) && bind(&AnaVLQCandidate::jetbin,_1)==ijet );
    stvlq.add_category( jetstr+"jets_signal_400_negative" , bind(&AnaVLQCandidate::opt_400_sr_negative,_1) && bind(&AnaVLQCandidate::jetbin,_1)==ijet );    
    //stvlq.add_category( jetstr+"jets_signal_400_base" , bind(&AnaVLQCandidate::opt_400_sr_base,_1) && bind(&AnaVLQCandidate::jetbin,_1)==ijet );
    //stvlq.add_category( jetstr+"jets_signal_400_matched" , bind(&AnaVLQCandidate::opt_400_sr_matched,_1) && bind(&AnaVLQCandidate::jetbin,_1)==ijet );
    stvlq.add_category( jetstr+"jets_control_400" , bind(&AnaVLQCandidate::opt_400_cr,_1) && bind(&AnaVLQCandidate::jetbin,_1)==ijet );
    stvlq.add_category( jetstr+"jets_control2_400" , bind(&AnaVLQCandidate::opt_400_cr2,_1) && bind(&AnaVLQCandidate::jetbin,_1)==ijet );
    stvlq.add_category( jetstr+"jets_control3_400" , bind(&AnaVLQCandidate::opt_400_cr3,_1) && bind(&AnaVLQCandidate::jetbin,_1)==ijet );
  }

  // Create all the different RunConfigurations for each different systematic shift, etc
  // ================================================================
  std::vector<RunConfig> mc_run_configs;
  std::cout << "Loading MC run configurations..." << std::endl;
  //mc_run_configs.push_back( RunConfig( "raw" , "RAW" , "RAW" , "RAW" , "RAW" , "RAW" , "RAW" , "RAW" , "RAW" ) );
  mc_run_configs.push_back( RunConfig( "nominal" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" ) );
  mc_run_configs.push_back( RunConfig( "qcd" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" ) );
  // configurations for JES systematics
  mc_run_configs.push_back( RunConfig( "jesup" , "NOMINAL" , "NOMINAL" , "JESUP" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" ) );
  mc_run_configs.push_back( RunConfig( "jesdown" , "NOMINAL" , "NOMINAL" , "JESDOWN" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" ) );
  // configurations for JER
  mc_run_configs.push_back( RunConfig( "jer" , "NOMINAL" , "NOMINAL" , "JER" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" ) );
  // configurations for electron smear systematics
  mc_run_configs.push_back( RunConfig( "elsmrup" , "NOMINAL" , "SMEARUP" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" ) );
  mc_run_configs.push_back( RunConfig( "elsmrdown" , "NOMINAL" , "SMEARDOWN" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" ) );
  mc_run_configs.push_back( RunConfig( "elsclup" , "NOMINAL" , "SCALEUP" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" ) );
  mc_run_configs.push_back( RunConfig( "elscldown" , "NOMINAL" , "SCALEDOWN" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" ) );
  // configurations for muon smear systematics
  mc_run_configs.push_back( RunConfig( "muidup" , "IDUP" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" ) );
  mc_run_configs.push_back( RunConfig( "muidlow" , "IDLOW" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" ) );
  mc_run_configs.push_back( RunConfig( "mumsup" , "MSUP" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" ) );
  mc_run_configs.push_back( RunConfig( "mumslow" , "MSLOW" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" ) );
  if( true ) {
    // configurations for the muon efficiency scale systematic
    mc_run_configs.push_back( RunConfig( "mueffup" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "STATUP" , "NOMINAL" , "NOMINAL" , "NOMINAL" ) );
    mc_run_configs.push_back( RunConfig( "mueffdown" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "STATDOWN" , "NOMINAL" , "NOMINAL" , "NOMINAL" ) );
    mc_run_configs.push_back( RunConfig( "mutrigup" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "STATUP" , "NOMINAL" , "NOMINAL" ) );
    mc_run_configs.push_back( RunConfig( "mutrigdown" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "STATDOWN" , "NOMINAL" , "NOMINAL" ) );
    // configurations for the electron efficiency scale systematic
    mc_run_configs.push_back( RunConfig( "eleffup" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "STATUP" , "NOMINAL" ) );
    mc_run_configs.push_back( RunConfig( "eleffdown" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "STATDOWN" , "NOMINAL" ) );
    //mc_run_configs.push_back( RunConfig( "eltrigup" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "STATUP" ) );
    //mc_run_configs.push_back( RunConfig( "eltrigdown" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "STATDOWN" ) );
  }
  // configurations for the MET systematics
  // FIXME
  std::vector<RunConfig> data_run_configs;
  std::cout << "Loading DATA run configurations..." << std::endl;
  //data_run_configs.push_back( RunConfig( "raw" , "RAW" , "RAW" , "RAW" , "RAW" , "RAW" , "RAW" , "RAW" , "RAW" ) );
  data_run_configs.push_back( RunConfig( "nominal" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" ) );
  data_run_configs.push_back( RunConfig( "qcd" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" ) );


  // Other Useful Defs
  // ================================================================  
  METUtility *metutil = new METUtility();
  // Turn on (off) the relevant MET terms
  // Standard MET_RefFinal has:
  // RefEle, RefGamma, RefTau, RefJet, SoftJets, RefMuon, MuonTotal, (CellOut), CellOut_Eflow
  metutil->defineMissingET(true, true, true, true, true, true, true, false, true);
  // The threshold below which jets enter the SoftJets term (JES is not applied)
  metutil->setSoftJetCut(20e3);
  // Whether to use MUID muons (otherwise STACO).
  metutil->setIsMuid(false);
  // Whether METUtility should scream at you over every little thing
  metutil->setVerbosity(false);

  static enum { PU_NONE , PU_MC11C } pileup_reweighting_mode = PU_NONE;
  std::string pileup_data_file = "./pileup_histos/ilumicalc_histograms_None_178044-191933.root"; // FIXME
  std::string pileup_config_file = "./pileup_histos/mc11c_custom.root";
  double lumi_tot;
  std::vector<double> int_lumi;
  // this lists all the run periods included in your pileup reweighting histogram... need it for muon efficiency calculation
  std::vector<std::string> run_periods;
  run_periods.push_back( "B" );
  run_periods.push_back( "D" );
  run_periods.push_back( "E" );
  run_periods.push_back( "F" );
  run_periods.push_back( "G" );
  run_periods.push_back( "H" );
  run_periods.push_back( "I" );
  run_periods.push_back( "J" );
  run_periods.push_back( "K" );
  run_periods.push_back( "L" );
  run_periods.push_back( "M" );

  // ================================================================  
  // ----- EVENT LOOP -----
  // ================================================================  
  unsigned long n_events = 0ul;
  AnaEventMgr* evtmgr = AnaEventMgr::instance(); assert(evtmgr);
  for( AnaEventMgr::iterator i=evtmgr->begin(), f=evtmgr->end(AnaConfiguration::user_max_nevents()); i!=f; ++i, ++n_events ) {

    const boost::shared_ptr<const AnaEvent>& evt( *i );
    if( !evt ) break; 
    dg::set_global_weight( 1. );
    
    // check sample type and decide whether or not we need to make
    // data-only or mc-only preselection cuts. then make them.
    // ================================================================
    AnaEvent::tribool is_mc = evt->check_flag( "mc" );
    bool is_data = !is_mc;

    // get run info
    const unsigned long raw_run_number = evt->run_number();
    const unsigned long mc_channel = evt->mc_channel();
    const unsigned long event_number = evt->event_number();
    const double average_mu = evt->average_mu();
    const unsigned long lumi_block = evt->lumi_block();
    unsigned long run_number = raw_run_number;

    bool is_wjets_mc = mc_channel >= 107680 && mc_channel <= 107705;
    bool is_signal = ( mc_channel >= 145082 && mc_channel <= 145091 ) || ( mc_channel >= 115511 && mc_channel <= 115519 );

    /* SET PILEUP WEIGHT */
    double pu_weight = 1.;
    double mc_weight = 1.;
    static boost::shared_ptr<Root::TPileupReweighting> pw( new Root::TPileupReweighting( "pw" ) );
    // we want to initialize the pw object event for data, so we can use it to calculate run_lumi
    if( pileup_reweighting_mode != PU_MC11C ) { //evt->check_tag("mc11c") && pileup_reweighting_mode != PU_MC11C ) {
      pileup_reweighting_mode = PU_MC11C;
      pw.reset( new Root::TPileupReweighting( "pw" ) );

      /*
      //pw->UsePeriodConfig("MC11c");
      pw->SetDefaultChannel( 0 );
      pw->SetUnrepresentedDataAction( 2 ); // just keep the data
      pw->DisableWarnings( true );
      pw->SetDataScaleFactors( AnaConfiguration::pileup_data_scale() );
      pw->AddConfigFile( pileup_config_file );
      pw->AddLumiCalcFile( pileup_data_file );
      pw->Initialize();
      */

      // Sam's version
      std::string PileupHistMC = "./pileup_histos/sam/mc11b_defaults.prw.root";
      std::string PileupHistData = "./pileup_histos/sam/ilumicalc_histograms_None_179710-191933.root";
      pw->SetUnrepresentedDataAction(2);
      pw->UsePeriodConfig("MC11b");
      pw->AddConfigFile(PileupHistMC);
      pw->AddLumiCalcFile(PileupHistData);
      pw->Initialize();
      // end Sam's version

      int_lumi = pw->getIntegratedLumiVector();
      lumi_tot = pw->GetIntegratedLumi();
      std::cout << "Initialized TPileupReweighting for MC11C" << std::endl;
      std::cout << "Total lumi = " << lumi_tot << std::endl;
    }
   
    // for data, we have an option of dropping "unrepresented" data
    if( is_data ) const bool vetoEvent = false;
    // for MC, calculate pileup weight
    else if( is_mc ) {
      if( true ) {
	dg::down( "pileup" , "pileup reweighting details" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
	dg::filleff( "pu_mc11c" , pileup_reweighting_mode==PU_MC11C );
      }
      pw->SetRandomSeed( 314159 + mc_channel*2718 + event_number );
      run_number = pw->GetRandomRunNumber( raw_run_number );
      pu_weight = pw->GetCombinedWeight( raw_run_number , mc_channel , average_mu );
      // skip event; negative pileup weight means we don't know the weight (rare event)
      if( pu_weight<0 ) {
	pu_weight = 0.;
	std::cerr << "WARNING: found event with negative pileup weight: " << pu_weight << std::endl;
      }
      mc_weight = evt->mc_weight();
      //dg::set_global_weight( pu_weight * mc_weight );
      dg::event_info().set_mc_weight( mc_weight );
      dg::event_info().set_pu_weight( pu_weight );
      if( true ) {
	dg::down( "pileup" , "" );
	dg::fillh( "pu_weight" , 100 , 0 , 10 , pu_weight );
	dg::fillh( "pu_weight_log10" , 100 , -10 , 10 , detector::safe_log10(pu_weight) );
	dg::fillh( "mc_weight" , 10 , -5 , 5 , mc_weight );
	dg::fillh( "event_weight" , 1000 , -20 , 20 , pu_weight * mc_weight );
	dg::up();
      }
      if( false ) {
	std::cout << "Weights: " << pu_weight << "\t" << mc_weight << std::endl;
      }
    }
    const double run_lumi = pw->GetIntegratedLumi( run_number , run_number );
    const double ev_weight = pu_weight * mc_weight;

    // check that a trigger is present
    const boost::shared_ptr<const AnaTrigger>& trigger = evt->trigger();
    if( true ) {
      dg::down( "triggers" , "" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      dg::filleff( "trigger_present" , trigger );
    }
    if( !trigger ) { std::cerr << "WARNING: trigger information not present for run " << raw_run_number << " evt " << event_number << ". skipping..." << std::endl; continue; }

    // count total events
    dg::fillhw( "total_raw" , 1 , 1 , 2 , 1 , 1. );
    dg::fillhw( "total" , 1 , 1 , 2 , 1 , ev_weight );

    // GRL selection
    // NOTE: the electron and muon GRLs are identical
    const bool is_good_run = is_mc ? true : grl.is_in( GoodRunList::ELECTRON , run_number , lumi_block );
    // keep track of how many events pass GRL
    if( true ) {
      dg::down( "presel" , "" );
      dg::filleff( "c1_grl" , is_good_run );
      dg::up();
    }
    if( !is_good_run ) continue;

    // larError check for data only
    const bool lar_error = ( is_data && evt->trigger()->error_lar() );
    if( true ) {
      dg::down( "presel" , "" );
      dg::filleff( "c2_larerror" , !lar_error );
      dg::up();
    }
    if( lar_error ) { continue; }

    // vertex selection
    dgSTL::copy_if( evt->vertices().begin() , evt->vertices().end() , back_inserter(vtx_coll) , CUT_ALL(AnaVertex,vtx_sel) );
    if( evt->vertices().size() ) {
      dgSTL::copy_if( evt->vertices().begin() , evt->vertices().begin()+1 , back_inserter(first_vtx_coll) , CUT_ALL(AnaVertex,vtx_sel) );
    }
    if( true ) {
      dg::down( "presel" , "" );
      dg::fillh( "nvtxs" , 20 , 0 , 20 , vtx_coll.size()<20 ? vtx_coll.size() : 19 );
      dg::filleff( "c3_nvtxs" , first_vtx_coll.size() > 0 );
      dg::up();
    }
    if( first_vtx_coll.size() == 0 ) { continue; }
    dgSTL::copy_if( evt->vertices().begin() , evt->vertices().end() , back_inserter(vtx_multijes_coll) , CUT_ALL(AnaVertex,vtx_multijes_sel) );
    dgSTL::copy_if( evt->vertices().begin() , evt->vertices().end() , back_inserter(vtx_jetcalib_coll) , CUT_ALL(AnaVertex,vtx_jetcalib_sel) );

    bool at_least_one_trigger = ( evt->trigger()->wz_single_electron_trigger_2012(run_number) ||
				  //evt->trigger()->wz_dielectron_trigger_2012(run_number) ||
				  evt->trigger()->wz_single_muon_trigger_2012(run_number) );//||
				  //evt->trigger()->wz_dimuon_trigger_2012(run_number) );

    if( true ) {
      dg::down( "presel" , "" );
      dg::filleff( "c4_trigger" , at_least_one_trigger );
      dg::up();
    }
    if( !at_least_one_trigger ) continue;


    // find some TRUTH particles
    double truth_w_pt = 0.;
    if( is_mc ) {
      if( is_signal ) {
	// signal -- find VLQ and neutrino
	dgSTL::copy_if( evt->truth_particles().begin() , evt->truth_particles().end() , back_inserter(truth_vlq_coll) , CUT_ALL(AnaTruthParticle,truth_vlq_sel) );
	dgSTL::copy_if( evt->truth_particles().begin() , evt->truth_particles().end() , back_inserter(truth_v_coll) , CUT_ALL(AnaTruthParticle,truth_v_sel) );
	if( truth_vlq_coll.size() == 0 ) { std::cout << "WARNING truth_vlq_coll.size() = " << truth_vlq_coll.size() << " for event " << event_number << " , clearing" << std::endl; }
	else if( truth_vlq_coll.size() > 1 ) {
	  std::cout << "WARNING truth_vlq_coll.size() = " << truth_vlq_coll.size() << " for event " << event_number << " , popping" << std::endl;
	  std::sort( truth_vlq_coll.begin() , truth_vlq_coll.end() , bind(&AnaTruthParticle::parent,_1) < bind(&AnaTruthParticle::parent,_2) );
	  while( truth_vlq_coll.size() > 1 ) truth_vlq_coll.pop_back();
	}
	if( truth_v_coll.size() == 0 ) { std::cout << "WARNING truth_v_coll.size() = " << truth_v_coll.size() << " for event " << event_number << " , clearing" << std::endl; }
	else if( truth_v_coll.size() > 1 ) {
	  std::cout << "WARNING truth_v_coll.size() = " << truth_v_coll.size() << " for event " << event_number << " , popping" << std::endl;
	  std::sort( truth_v_coll.begin() , truth_v_coll.end() , bind(&AnaTruthParticle::parent,_1) < bind(&AnaTruthParticle::parent,_2) );
	  while( truth_v_coll.size() > 1 ) truth_v_coll.pop_back();
	}
	if( false ) {
	  dgSTL::copy_if( evt->truth_particles().begin() , evt->truth_particles().end() , back_inserter(truth_forwardjet_coll) , CUT_ALL(AnaTruthParticle,truth_forwardjet_sel) );
	  std::sort( truth_forwardjet_coll.begin() , truth_forwardjet_coll.end() , bind(&AnaTruthParticle::pt,_1) > bind(&AnaTruthParticle::pt,_2) );
	  if( truth_forwardjet_coll.size() ) {
	    boost::shared_ptr<const AnaTruthParticle> tmp = truth_forwardjet_coll[0];
	    truth_forwardjet_coll.clear();
	    truth_forwardjet_coll.push_back( tmp );
	    tmp.reset();
	  }
	}
      }
      if( is_wjets_mc ) {
	// Wjets MC -- find W pT for reweighting
	dgSTL::copy_if( evt->truth_particles().begin() , evt->truth_particles().end() , back_inserter(truth_w_coll) , CUT_ALL(AnaTruthParticle,truth_w_sel) );
	if( truth_w_coll.size() < 2 ) {
	  std::cout << "WARNING truth_w_coll.size() = " << truth_w_coll.size() << " for event " << event_number << " , clearing" << std::endl;
	  truth_w_coll.clear();
	} else if( truth_w_coll.size() > 2 ) {
	  std::cout << "WARNING truth_w_coll.size() = " << truth_w_coll.size() << " for event " << event_number << " , popping" << std::endl;
	  std::sort( truth_w_coll.begin() , truth_w_coll.end() , bind(&AnaTruthParticle::parent,_1) < bind(&AnaTruthParticle::parent,_2) );
	  while( truth_w_coll.size() > 2 ) truth_w_coll.pop_back();
	}
	if( truth_w_coll.size() == 2 ) {
	  TLorentzVector tmp_w( 0. , 0. , 0. , 0. );
	  tmp_w += truth_w_coll.front()->four_vector();
	  tmp_w += truth_w_coll.back()->four_vector();
	  truth_w_pt = tmp_w.Pt();
	}
      }
    }
    
    // some preliminary metutil calibration setup that we only want to do once... not for every run configuration
    dgSTL::copy_if( evt->missing_et().begin() , evt->missing_et().end() , back_inserter(met_reffinal_coll) , bind(&AnaMET::met_type,_1) == AnaMET::REF_FINAL );
    assert( met_reffinal_coll.size()==1 );
    boost::shared_ptr<const AnaMET> met( *met_reffinal_coll.begin() );
    const float orig_met_pt = met->pt();
    metutil->reset();
    const float MeV = 1000.;
    metutil->setMETTerm( METUtil::RefGamma , met->refgamma_etx()*MeV , met->refgamma_ety()*MeV , met->refgamma_sumet()*MeV );
    metutil->setMETTerm( METUtil::RefTau , met->reftau_etx()*MeV , met->reftau_ety()*MeV , met->reftau_sumet()*MeV );
    metutil->setMETTerm( METUtil::CellOutEflow , met->cellout_eflow_etx()*MeV , met->cellout_eflow_ety()*MeV , met->cellout_eflow_sumet()*MeV );
    metutil->setMETTerm( METUtil::MuonTotal , met->muonboy_etx()*MeV , met->muonboy_ety()*MeV , met->muonboy_sumet()*MeV );
    //metutil->setMETTerm( METUtil::CellOut , met->cellout_etx()*MeV , met->cellout_ety()*MeV , met->cellout_sumet()*MeV );
    metutil->setMETTerm( METUtil::RefMuon , met->refmuon_etx()*MeV , met->refmuon_ety()*MeV , met->refmuon_sumet()*MeV );
    metutil->setMETTerm( METUtil::SoftJets , met->softjets_etx()*MeV , met->softjets_ety()*MeV , met->softjets_sumet()*MeV );    
    //metutil->setMETTerm( METUtil::RefJet , met->refjet_etx()*MeV , met->refjet_ety()*MeV , met->refjet_sumet()*MeV );


    // LOOP OVER ALL RUN CONFIGURATIONS
    // ===============================
    // This is just an "easy" way to process all the systematics at the same time. Basically,
    // we rerun the event for each different systematic shift that we care about
    std::vector<RunConfig> run_configs = is_mc ? mc_run_configs : data_run_configs;
    BOOST_FOREACH( RunConfig config , run_configs ) {
      // first we want to make sure all our object vectors are empty
      el_calib_coll.clear();
      el_loose_coll.clear();
      el_tight_coll.clear();
      el_qcd_coll.clear();
      mu_calib_coll.clear();
      mu_loose_coll.clear();
      mu_tight_coll.clear();
      mu_qcd_coll.clear();
      lepjet_calib_coll.clear();
      lepjet_coll.clear();
      badlepjet_coll.clear();
      lepjet_lc_calib_coll.clear();
      met_calib_coll.clear();
      vlq_calib_coll.clear();
      vlq_coll.clear();

      // preselection energy corrections for electrons in data
      size_t iel = 0;
      BOOST_FOREACH( const boost::shared_ptr<const AnaElectron>& el , evt->electrons() ) {
	if( config.electron_calib == "RAW" ) {
	  el_calib_coll.push_back( el );
	} else if( is_data ) {
	  el_calib_coll.push_back( el->egamma_corrected( conf , el , event_number , iel , "NOMINAL" ) );
	} else {
	  if( fabs(el->cluster_eta()) > 999999. ) continue; // this prevents an annoying run-time crash
	  el_calib_coll.push_back( el->egamma_smeared( conf , el , event_number , iel , config.electron_calib ) );
	}
	iel++;
      }

      // preselection smearing for muons
      size_t imu = 0;
      const int muscale = 3;
      BOOST_FOREACH( const boost::shared_ptr<const AnaMuon>& mu , evt->muons() ) {
	if( config.muon_calib == "RAW" ) {
	  mu_calib_coll.push_back( mu );
	} else if( is_data ) {
	  mu_calib_coll.push_back( mu );
	} else {
	  mu_calib_coll.push_back( mu->mcp_smeared( conf , AnaConfiguration::data_range() , mu , detector::MCP_STACO_COMBINED , event_number , imu , muscale , config.muon_calib ) );
	}
	imu++;
      }

      // preselection smearing and scaling for jets
      size_t ijet = 0;
      BOOST_FOREACH( const boost::shared_ptr<const AnaJet>& jet , evt->atlas_anti_kt_4_h1_topo_jets() ) {
	if( config.jet_calib == "RAW" ) {
	  lepjet_calib_coll.push_back( jet );
	} else if( is_data ) {
	  lepjet_calib_coll.push_back( jet->jetetmiss_calibrate(conf,jet,event_number,ijet,average_mu,vtx_jetcalib_coll.size(),is_mc,true,false) );
	} else if( config.jet_calib == "NOMINAL" ) {
	  lepjet_calib_coll.push_back( jet->jetetmiss_calibrate(conf,jet,event_number,ijet,average_mu,vtx_jetcalib_coll.size(),is_mc,true,false) );
	} else if( config.jet_calib == "JER" ) {
	  lepjet_calib_coll.push_back( jet->jetetmiss_calibrate(conf,jet,event_number,ijet,average_mu,vtx_jetcalib_coll.size(),is_mc,true,true) );
	} else {
	  boost::shared_ptr<const AnaJet> tmp_jet = jet->jetetmiss_calibrate(conf,jet,event_number,ijet,average_mu,vtx_jetcalib_coll.size(),is_mc,true,false);
	  lepjet_calib_coll.push_back( tmp_jet->jetetmiss_scale(conf,jet,event_number,1,0.,ijet,average_mu,vtx_multijes_coll.size(),config.jet_calib) );
	  tmp_jet.reset();
	}
	ijet++;
      }

      // preselection smearing and scaling for LC jets (for MET calculation)
      ijet = 0;
      BOOST_FOREACH( const boost::shared_ptr<const AnaJet>& jet , evt->atlas_anti_kt_4_h1_lc_topo_jets() ) {
	// no EtaOffset calibration done here becaus we need the constscale jet vars to do this correctly
	if( config.jet_calib == "RAW" ) {
	  lepjet_lc_calib_coll.push_back( jet );
	} else if( is_data ) {
	  lepjet_lc_calib_coll.push_back( jet );
	} else if( config.jet_calib == "NOMINAL" ) {
	  lepjet_lc_calib_coll.push_back( jet );
	  //lepjet_lc_calib_coll.push_back( jet->jetetmiss_calibrate(conf,jet,event_number,ijet,average_mu,vtx_jetcalib_coll.size(),is_mc,false,true) );
	} else if( config.jet_calib == "JER" ) {
	  lepjet_lc_calib_coll.push_back( jet->jetetmiss_calibrate(conf,jet,event_number,ijet,average_mu,vtx_jetcalib_coll.size(),is_mc,false,true) );
	} else {
	  lepjet_lc_calib_coll.push_back( jet->jetetmiss_scale(conf,jet,event_number,1,0.,ijet,average_mu,vtx_multijes_coll.size(),config.jet_calib) );
	}
	ijet++;
      }

      // preselection MET reconstruction using METUtility
      // FIXME add met systematics
      met_calib_coll.push_back( met->jetetmiss_met( met , metutil ,
						    el_calib_coll.begin() , el_calib_coll.end() , true ,
						    mu_calib_coll.begin() , mu_calib_coll.end() , true , 
						    lepjet_lc_calib_coll.begin() , lepjet_lc_calib_coll.end() , true ,
						    METUtil::RefFinal , METUtil::None ) );
      if( true ) {
	dg::down( "metcalib" , "" );
	dg::fillh( config.name + "_reffinal_res" , 200 , -20 , 20 , met->pt() - (metutil->getMissingET(METUtil::RefFinal).et()/MeV) );
	if( config.name == "nominal" ) {
	  dg::fillh( config.name + "_refgamma_res" , 200 , -20 , 20 , met->refgamma() - (metutil->getMissingET( METUtil::RefGamma ).et()/MeV) );
	  dg::fillh( config.name + "_reftau_res" , 200 , -20 , 20 , met->reftau() - (metutil->getMissingET( METUtil::RefTau ).et()/MeV) );
	  dg::fillh( config.name + "_cellouteflow_res" , 200 , -20 , 20 , met->cellout_eflow() - (metutil->getMissingET( METUtil::CellOutEflow ).et()/MeV) );
	  dg::fillh( config.name + "_muontotal_res" , 200 , -20 , 20 , met->muonboy() - (metutil->getMissingET( METUtil::MuonTotal ).et()/MeV) );
	  dg::fillh( config.name + "_refmuon_res" , 200 , -20 , 20 , met->refmuon() - (metutil->getMissingET( METUtil::RefMuon ).et()/MeV) );
	  dg::fillh( config.name + "_softjets_res" , 200 , -20 , 20 , met->softjets() - (metutil->getMissingET( METUtil::SoftJets ).et()/MeV) );
	  dg::fillh( config.name + "_refele_res" , 200 , -20 , 20 , met->refele() - (metutil->getMissingET( METUtil::RefEle ).et()/MeV) );
	  dg::fillh( config.name + "_refjet_res" , 200 , -20 , 20 , met->refjet() - (metutil->getMissingET( METUtil::RefJet ).et()/MeV) );
	  dg::fillh( config.name + "_cellout_res" , 200 , -20 , 20 , met->cellout() - (metutil->getMissingET( METUtil::CellOut ).et()/MeV) );
	}
	dg::up();
      }


      // study selections
      if( config.name == "nominal" ) {
        // some histograms are made here, but none of the cuts are actually applied yet
        dg::down( "object_sels" , "" );
	dg::down( config.name , "" );
	el_tight_sel.for_each( el_calib_coll.begin() , el_calib_coll.end() );
	el_loose_sel.for_each( el_calib_coll.begin() , el_calib_coll.end() );
	el_qcd_sel.for_each( el_calib_coll.begin() , el_calib_coll.end() );
	mu_tight_sel.for_each( mu_calib_coll.begin() , mu_calib_coll.end() );
	mu_loose_sel.for_each( mu_calib_coll.begin() , mu_calib_coll.end() );
	mu_qcd_sel.for_each( mu_calib_coll.begin() , mu_calib_coll.end() );
	jet_sel.for_each( lepjet_calib_coll.begin() , lepjet_calib_coll.end() );
	jet_bad_sel.for_each( lepjet_calib_coll.begin() , lepjet_calib_coll.end() );
	dg::up();
	dg::up(); // object_sels
      }

      // apply selections
      dgSTL::copy_if( el_calib_coll.begin() , el_calib_coll.end() , back_inserter(el_tight_coll) , CUT_ALL(AnaElectron,el_tight_sel) );
      dgSTL::copy_if( el_calib_coll.begin() , el_calib_coll.end() , back_inserter(el_loose_coll) , CUT_ALL(AnaElectron,el_loose_sel) );
      if( config.name == "qcd" ) dgSTL::copy_if( el_calib_coll.begin() , el_calib_coll.end() , back_inserter(el_qcd_coll) , CUT_ALL(AnaElectron,el_qcd_sel) );
      dgSTL::copy_if( mu_calib_coll.begin() , mu_calib_coll.end() , back_inserter(mu_tight_coll) , CUT_ALL(AnaMuon,mu_tight_sel) );
      dgSTL::copy_if( mu_calib_coll.begin() , mu_calib_coll.end() , back_inserter(mu_loose_coll) , CUT_ALL(AnaMuon,mu_loose_sel) );
      if( config.name == "qcd" ) dgSTL::copy_if( mu_calib_coll.begin() , mu_calib_coll.end() , back_inserter(mu_qcd_coll) , CUT_ALL(AnaMuon,mu_qcd_sel) );
      dgSTL::copy_if( lepjet_calib_coll.begin() , lepjet_calib_coll.end() , back_inserter(lepjet_coll) , CUT_ALL(AnaJet,jet_sel) );
      dgSTL::copy_if( lepjet_calib_coll.begin() , lepjet_calib_coll.end() , back_inserter(badlepjet_coll) , CUT_ALL(AnaJet,jet_bad_sel) );
      
      // also apply trigger matching
      std::remove_if( el_tight_coll.begin() , el_tight_coll.end() , bind(&AnaElectron::vlq_trigger_matched,_1,run_number)==false );
      std::remove_if( el_loose_coll.begin() , el_loose_coll.end() , bind(&AnaElectron::vlq_trigger_matched,_1,run_number)==false );
      std::remove_if( el_qcd_coll.begin() , el_qcd_coll.end() , bind(&AnaElectron::vlq_trigger_matched,_1,run_number)==false );      
      std::remove_if( mu_tight_coll.begin() , mu_tight_coll.end() , bind(&AnaMuon::vlq_trigger_matched,_1,run_number)==false );
      std::remove_if( mu_loose_coll.begin() , mu_loose_coll.end() , bind(&AnaMuon::vlq_trigger_matched,_1,run_number)==false );
      std::remove_if( mu_qcd_coll.begin() , mu_qcd_coll.end() , bind(&AnaMuon::vlq_trigger_matched,_1,run_number)==false );

      // overlap removal
      el_mu_overlap_removal( el_tight_coll , mu_loose_coll , 0.1 );
      el_mu_overlap_removal( el_loose_coll , mu_loose_coll , 0.1 );
      el_mu_overlap_removal( el_qcd_coll , mu_loose_coll , 0.1 );
      if( el_tight_coll.size() ) el_qcd_coll.clear();
      if( config.name == "qcd" ) {
	jet_el_overlap_removal( lepjet_coll , el_qcd_coll , 0.3 );
	jet_el_overlap_removal( badlepjet_coll , el_qcd_coll , 0.3 );
      } else {
	jet_el_overlap_removal( lepjet_coll , el_tight_coll , 0.3 );
	jet_el_overlap_removal( badlepjet_coll , el_tight_coll , 0.3 );
      }
      mu_jet_overlap_removal( mu_tight_coll , lepjet_coll , 0.4 );
      mu_jet_overlap_removal( mu_loose_coll , lepjet_coll , 0.4 );
      mu_jet_overlap_removal( mu_qcd_coll , lepjet_coll , 0.4 );

      // post-selection
      bool jet_larhole = jet_larhole_veto( lepjet_coll , is_mc , raw_run_number );
      assert( met_calib_coll.size()==1 );
      if( config.name == "nominal" ) {
	dg::down( "postsel" , "" );
	dg::down( config.name , "" );
	dg::fillh( "met_pt" , 500 , 0 , 500 , met_calib_coll.front()->pt() );
	dg::fillh( "njets" , 10 , 0 , 10 , lepjet_coll.size() );
	dg::fillh( "nbadjets" , 10 , 0 , 10 , badlepjet_coll.size() );
	dg::fillh( "ntightelectrons" , 10 , 0 , 10 , el_tight_coll.size() );
	dg::fillh( "nlooseelectrons" , 10 , 0 , 10 , el_loose_coll.size() );
	dg::fillh( "ntightmuons" , 10 , 0 , 10 , mu_tight_coll.size() );
	dg::fillh( "nloosemuons" , 10 , 0 , 10 , mu_loose_coll.size() );
	dg::fillh( "nqcdelectrons" , 10 , 0 , 10 , el_qcd_coll.size() );
	dg::fillh( "nqcdmuons" , 10 , 0 , 10 , mu_qcd_coll.size() );
	dg::filleff( "jet_larhole_veto" , jet_larhole );
	dg::up();
	dg::up();
      }
      if( jet_larhole ) { continue; }
      if( badlepjet_coll.size() ) { continue; }
      if( lepjet_coll.size() < 2 ) { continue; }

      // the logic here gets a bit tricky... I'll try to explain as I go
      // if there's a good tight electron or muon, then don't pay attention to the qcd vectors
      if( el_tight_coll.size() ) { el_qcd_coll.clear(); mu_qcd_coll.clear(); }
      if( mu_tight_coll.size() ) { el_qcd_coll.clear(); mu_qcd_coll.clear(); }
      // now that this logic is in place, we should be able to move the the "qcd" collections over to the "tight" collections
      // for the "qcd" run config, so both will be treated the same inside AnaVLQCandidate
      if( config.name == "qcd" ) {
	el_tight_coll.clear();
	mu_tight_coll.clear();
	std::copy( el_qcd_coll.begin() , el_qcd_coll.end() , back_inserter(el_tight_coll) );
	std::copy( mu_qcd_coll.begin() , mu_qcd_coll.end() , back_inserter(mu_tight_coll) );
      }
      // if there are good electrons and muons, skip the event
      if( el_tight_coll.size() && mu_tight_coll.size() ) { continue; }
      // if there are too many good electrons or muons, skip the event
      if( el_tight_coll.size() > 1 ) { continue; }
      if( el_loose_coll.size() > 1 ) { continue; }
      if( mu_tight_coll.size() > 1 ) { continue; }
      // if there are no good electrons && muons, skip the event
      if( el_tight_coll.size()==0 && mu_tight_coll.size()==0 ) { continue; }
      // if we've made it this far, there should only be one lepton remaining, so lets assert that this is the case.
      assert( el_tight_coll.size() + mu_tight_coll.size() == 1 );

      // now make sure we don't bother making plots for systematics that will have no effect
      if( el_tight_coll.size() ) {
	if( config.name=="muidup" || config.name=="muidlow" || config.name=="mumsup" || config.name=="mumslow" || 
	    config.name=="mueffup" || config.name=="mueffdown" || config.name=="mutrigup" || config.name=="mutrigdown" ) 
	  { continue; }
      }
      if( mu_tight_coll.size() ) {
	if( config.name=="elsmrup" || config.name=="elsmrdown" || config.name=="elsclup" || config.name=="elscldown" || 
	    config.name=="eleffup" || config.name=="eleffdown" || config.name=="eltrigup" || config.name=="eltrigdown" )
	  { continue; }
      }

      boost::shared_ptr<const AnaVLQCandidate> vlq_calib( new AnaVLQCandidate( el_tight_coll.begin() , el_tight_coll.end() ,
									       mu_tight_coll.begin() , mu_tight_coll.end() ,
									       lepjet_coll.begin() , lepjet_coll.end() ,
									       vtx_coll.begin() , vtx_coll.end() ,
									       met_calib_coll.front() ,
									       trigger ,
									       average_mu ,
									       event_number ,
									       raw_run_number ,
									       is_data ,
									       truth_vlq_coll.begin() , truth_vlq_coll.end() ,
									       truth_v_coll.begin() , truth_v_coll.end() ) );
      vlq_calib_coll.push_back( vlq_calib );


      if( config.name == "nominal" ) {
	dg::down( "object_sels" , "" );
	dg::down( config.name , "" );
	vlq_sel.for_each( vlq_calib_coll.begin() , vlq_calib_coll.end() );
	dg::up();
	dg::up();
      }
      // apply selection
      dgSTL::copy_if( vlq_calib_coll.begin() , vlq_calib_coll.end() , back_inserter(vlq_coll) , CUT_ALL(AnaVLQCandidate,vlq_sel) );
      
      // MAKE PLOTS
      // ==========
      // first we need to find the reconstruction/trigger scales
      double mueff_scale = 1. , mueff_stat = 1. , mueff_sys = 1.;
      double mutrig_scale = 1. , mutrig_stat = 1. , mutrig_sys = 1.;
      float eleff_scale = 1. , eleff_stat = 1. , eleff_sys = 1.;
      double eltrig_scale = 1. , eltrig_stat = 1. , eltrig_sys = 1.;
      float wjets_scale = 1.;
      if( !is_data && config.name != "raw" ) {
	std::vector<TLorentzVector> el_fourvec;
	std::vector<TLorentzVector> mu_fourvec;
	if( mu_tight_coll.size() ) mu_fourvec.push_back( mu_tight_coll.front()->four_vector() );
	if( el_tight_coll.size() ) {
	  TLorentzVector cluster_el;
	  cluster_el.SetPtEtaPhiE( el_tight_coll.front()->pt() , el_tight_coll.front()->cluster_eta() , el_tight_coll.front()->cluster_phi() , el_tight_coll.front()->energy() );
	  el_fourvec.push_back( cluster_el );
	}
	detector::EGAMMA_TYPE eltype = detector::EGAMMA_TIGHTPP;
	if( config.name == "qcd" ) eltype = detector::EGAMMA_MEDIUMPP;
	// muon scales
	if( mu_tight_coll.size() ) {
	    AnaMuon::mcp_effscale( AnaConfiguration::conf() , AnaConfiguration::data_range() , mu_tight_coll.front() , detector::MCP_STACO_COMBINED , 
				   run_number , int_lumi , run_periods , 
				   mueff_scale , mueff_stat , mueff_sys );
	    AnaMuon::GetTriggerSF_v17( AnaConfiguration::conf() , detector::MCP_STACO_COMBINED , eltype , mu_fourvec , el_fourvec , run_number , 
				       0 , mutrig_scale , mutrig_stat );
	}
	if( config.muon_effscale == "STATUP" ) mueff_scale += mueff_stat;
	else if( config.muon_effscale == "STATDOWN" ) mueff_scale -= mueff_stat;
	else if( config.muon_effscale == "SYSUP" ) mueff_scale += mueff_sys;
	else if( config.muon_effscale == "SYSDOWN" ) mueff_scale -= mueff_sys;
	else if( config.muon_trigscale == "STATUP" ) mutrig_scale += mutrig_stat;
	else if( config.muon_trigscale == "STATDOWN" ) mutrig_scale -= mutrig_stat;
	// electron scales
	if( el_tight_coll.size() ) {
	  AnaElectron::egamma_effscale( AnaConfiguration::conf() , el_tight_coll.front() , eleff_scale , eleff_stat );
	  AnaMuon::GetTriggerSF_v17( AnaConfiguration::conf() , detector::MCP_STACO_COMBINED , eltype , mu_fourvec , el_fourvec , run_number , 0 , mutrig_scale , mutrig_stat );
	}
	if( config.electron_effscale == "STATUP" ) eleff_scale += eleff_stat;
	else if( config.electron_effscale == "STATDOWN" ) eleff_scale -= eleff_stat;
	else if( config.electron_trigscale == "STATUP" ) eltrig_scale += eltrig_stat;
	else if( config.electron_trigscale == "STATDOWN" ) eltrig_scale -= eltrig_stat;
	// wjets scales
	if( false && is_wjets_mc ) {
	  if( truth_w_pt < 10. ) wjets_scale = 0.9;
	  else if( truth_w_pt < 20. ) wjets_scale = 1.05;
	  else if( truth_w_pt < 30. ) wjets_scale = 1.13;
	  else if( truth_w_pt < 40. ) wjets_scale = 1.15;
	  else if( truth_w_pt < 250. ) wjets_scale = exp( 0.2165 - (0.0022075*truth_w_pt) );
	  // otherwise let it stay set to 1.
	}
      }
      if( true && config.name == "nominal" ) { // FIXME skipping this for now
	dg::down( "particle_reweighting" , "" );
	dg::down( config.name , "" );
	dg::fillh( "mueff_scale" , 100 , 0 , 5 , mueff_scale );
	dg::fillh( "mutrig_scale" , 100 , 0 , 5 , mutrig_scale );
	dg::fillh( "eleff_scale" , 100 , 0 , 5 , eleff_scale );
	dg::fillh( "eltrig_scale" , 100 , 0 , 5 , eltrig_scale );
	dg::fillh( "wjets_scale" , 100 , 0 , 5 , wjets_scale );
	dg::up();
	dg::up();
      }
      dg::set_global_weight( ev_weight * mueff_scale * mutrig_scale * eleff_scale * eltrig_scale * wjets_scale ); 
      
      // now make the plots
      if( el_tight_coll.size() ) dg::down( "elec" , "" );
      else dg::down( "muon" , "" );
      stvlq.repurpose( config.name , "" );
      stvlq.for_each( vlq_coll.begin() , vlq_coll.end() );
      dg::up();

      // dump some event info if necessary
      if( is_data && config.name == "nominal" && vlq_coll.size() ) {
	if( vlq_coll[0]->opt_400_sr() ) {
	  if( vlq_coll[0]->vlq_m_detawj() > 300. ) {
	    std::string flavor = el_tight_coll.size() ? "elec" : "muon";
	    std::cout << "HIGHMASSVLQEVENT " << flavor << " " << lumi_block << " " << raw_run_number << " " << event_number << " " << vlq_coll[0]->vlq_m_detawj() << std::endl;
	  }
	}
      }

      dg::set_global_weight( 1. );

    } // end loop over run configurations
  } // end for each event
  
  AnaEventMgr::instance()->close_sample();

} // end analysis



#ifdef F77_DUMMY_MAIN
#ifdef __cplusplus
extern "C"
#endif
int F77_DUMMY_MAIN() { abort(); return 1; }
#endif

