
#include "TrigFTKAna/AnaCommon.hpp"
#include <set>
#include <string>
#include <list>
#include <bitset>
#include <algorithm>
#include <functional>
#include <iterator>
#include <cassert>
#include <cmath>
#include <cassert>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/scope_exit.hpp>
#include <boost/foreach.hpp>
#include <boost/format.hpp>
#include <boost/any.hpp>
#include <boost/ref.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/assign/std/set.hpp>
#include <boost/assign/std/vector.hpp>
#include <boost/assign/list_of.hpp>
#include <ctime>
#include <iostream>
#include <iomanip>
#include "DataGraphics/DataGraphics.hh"
#include "DataGraphics/DgCutSet.hh"
#include "DataGraphics/DgSTL.hh"
#include "TrigFTKAna/AnaConfiguration.hpp"
#include "TrigFTKAna/AnaEventMgr.hpp"
#include "TrigFTKAna/AnaTruthTable.hpp"
#include "TrigFTKAna/AnaTrigger.hpp"
#include "TrigFTKAna/AnaTrack.hpp"
#include "TrigFTKAna/AnaElectron.hpp"
#include "TrigFTKAna/AnaMuon.hpp"
#include "TrigFTKAna/AnaJet.hpp"
#include "TrigFTKAna/AnaMET.hpp"
#include "TrigFTKAna/AnaW.hpp"
#include "TrigFTKAna/EventObjectColl.hpp"
#include "TrigFTKAna/GoodRunList.hpp"
#include "TrigFTKAna/CompositeGenerator.hpp"
#include "TrigFTKAna/StudySemileptonicW.hpp"
#include "TrigFTKAna/StudyJet.hpp"

using namespace std;
using namespace DataGraphics;
using namespace boost;
using namespace boost::assign;
using boost::shared_ptr;
using dgSTL::copy_if;

////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C"
#endif
int
F77_MAIN( int argc , char* argv[] )
{

  AnaConfiguration::configure("ana_top",argc,argv);
  if( !AnaConfiguration::configured() ) { exit(0); }
  dg::root()->no_save_at_exit();
  BOOST_SCOPE_EXIT() { dg::root()->do_save_at_exit(); } BOOST_SCOPE_EXIT_END;

  // description of Constantinos's requirements
  // ----------------------------------------------------------------
  // preselection: good run list && trig.Passed_EF_mu10
  // muons: Muid pt eta phi e charge
  //             author etcone ptcone topinputs
  //        |eta|<2.5
  //        pt>20 GeV
  //        ptcone<4 GeV
  //        etcone/mu_pt < 0.1
  //        author==12
  //        topinputs==1
  // met: RefFinal antikt_emjes
  //        MET>20 GeV
  // jets: AntiKt4TopoEMJESJets pt,eta,phi,e,et
  //       pT>20 GeV (good jets) 40 GeV (tight jets)
  //       |eta|<2.5 
  //
  // event selection: 
  //    at least one reconstructed muon: RMU_pt,_phi,_etcone,_ptcone,_author
  //    MET>20 GeV
  //    mW>=30 GeV
  // 
  // plots:
  // - RMETall and RMET_phi for all events passing preselection
  // - studies of n pileup around line 253 of looperData.py for preselected events
  //       NTRKS{PRVX,PILEUPVX,PRVXout,PILEUPVXout,etc.}
  // - RMU{0,1,...}_pt,_eta for first, second, good muon in event
  //   RGOODMU_pt,_eta for all good muons in event
  //   RBADMU_pt,_eta for muons failing cuts
  //   Zmass,_highRMET for good dimuon closest to MZ with at least two good muons in event,
  //                   and subset with MET>20 GeV
  //   RMU0pt_highRMET for first good muon in events with MET>20 GeV, _lowRMET for <20GeV
  // - METmjbef for MET with at least one good jet
  //      0jbef,bef for MET with no good jets
  // - HT = jetETs + muon ET
  // - jet pt, eta for nth pt jets
  // - RM0_phi0,badjets0j,ht0j,muon_pt0j,wtm0j,rmet0j for events with zero jets
  // - MW, MW in events with two good muons
  // 
  // 
  // todo: add event weights to all histograms


  // event preselection
  // ================================================================
  GoodRunList grl;
  grl.add( GoodRunList::ELECTRON , "./good_run_lists/top_allchannels_7TeV_periodA.xml" );
  grl.add( GoodRunList::ELECTRON , "./good_run_lists/top_allchannels_7TeV_periodB.xml" );
  grl.add( GoodRunList::ELECTRON , "./good_run_lists/top_allchannels_7TeV_periodC.xml" );
  grl.add( GoodRunList::ELECTRON , "./good_run_lists/top_allchannels_7TeV_periodD1.xml" );
  grl.add( GoodRunList::MUON , "./good_run_lists/top_allchannels_7TeV_periodA.xml" );
  grl.add( GoodRunList::MUON , "./good_run_lists/top_allchannels_7TeV_periodB.xml" );
  grl.add( GoodRunList::MUON , "./good_run_lists/top_allchannels_7TeV_periodC.xml" );
  grl.add( GoodRunList::MUON , "./good_run_lists/top_allchannels_7TeV_periodD1.xml" );

  // define good muon selection
#define abs_value(objectT) std::ptr_fun<objectT::ftype,objectT::ftype>(std::abs)
  EventObjectColl< shared_ptr<const AnaMuon> > mu_top;
  EventObjectColl< shared_ptr<const AnaMuon> > mu_trigger_top;
  DgCutSet< shared_ptr<const AnaMuon> > mu_top_sel( "mu_top_selection" );
  mu_top_sel.add_uu( "primary_author" , 
                     bind(&AnaMuon::primary_author,_1) == 12 , 
                     bind(&dg::fillh,_1,64,0,64,bind(&AnaMuon::primary_author,_2)) );
  mu_top_sel.add_uu( "pt" , 
                     bind(&AnaMuon::pt,_1) > 20. , 
                     bind(&dg::fillh,_1,100,0,100,bind(&AnaMuon::pt,_2)) );
  mu_top_sel.add_uu( "eta" , 
                     bind( abs_value(AnaMuon) , bind(&AnaMuon::eta,_1)) < 2.5 ,
                     bind( &dg::fillh , _1 , 60 , -3 , 3 , bind(&AnaMuon::eta,_2) ) );
  mu_top_sel.add_uu( "ptcone30" , 
                     bind(&AnaMuon::pt_cone,_1,AnaMuon::CONE30) < 4. , 
                     bind(&dg::fillh,_1,100,0,20,bind(&AnaMuon::pt_cone,_2,AnaMuon::CONE30)) );
  mu_top_sel.add_uu( "etcone20ratio" , 
                     bind(&AnaMuon::et_cone_ratio,_1,AnaMuon::CONE20) < 0.1 , 
                     bind(&dg::fillh,_1,100,0,3,bind(&AnaMuon::et_cone_ratio,_2,AnaMuon::CONE20)) );
  mu_top_sel.add_uu( "topinputs" , 
                     bind(&AnaMuon::topinputs,_1) == 1 , 
                     bind(&dg::fillh,_1,10,0,10,bind(&AnaMuon::topinputs,_2)) );
  DgCutSet< shared_ptr<const AnaMuon> > mu_trigger_top_sel( "mu_trigger_top_selection" );
  mu_trigger_top_sel.based_on( mu_top_sel );
  mu_trigger_top_sel.add_uu( "trigger_mu10" ,
                             bind( &AnaTrigger::ef_mu10 , 
                                   bind(&AnaEvent::trigger,boost::ref(AnaEventMgr::current_event())) ) == true ,
                             bind( &dg::filleff , _1 , 40 , 0 , 40 , bind(&AnaMuon::pt,_2) ,
                                   bind( &AnaTrigger::ef_mu10 , bind(&AnaEvent::trigger,boost::ref(AnaEventMgr::current_event())) ) == true ) );
  mu_trigger_top_sel.set_cutflow_order( list_of("trigger_mu10")("primary_author")("pt")("eta")("ptcone30")("etcone20ratio")("topinputs") );
  
  // missing ET selection
  EventObjectColl< shared_ptr<const AnaMET> > met_top;
  DgCutSet< shared_ptr<const AnaMET> > met_top_sel( "met_top_selection" );
  met_top_sel.add_u( "met_type" , 
                     bind(&AnaMET::met_type,_1) == AnaMET::REF_FINAL_ANTIKT_EMJES );
  met_top_sel.add_uu( "met" , 
                      bind(&AnaMET::met,_1) > 20. , 
                      bind(&dg::fillh,_1,100,0,100,bind(&AnaMET::met,_2)) );
  EventObjectColl< shared_ptr<const AnaMET> > met_ref;
  DgCutSet< shared_ptr<const AnaMET> > met_ref_sel( "met_ref_selection" );
  met_ref_sel.add_u( "met_type" , 
                     bind(&AnaMET::met_type,_1) == AnaMET::REF_FINAL );
  met_ref_sel.add_uu( "met" , 
                      bind(&AnaMET::met,_1) > 20. , 
                      bind(&dg::fillh,_1,100,0,100,bind(&AnaMET::met,_2)) );

  // jet selection
  DgCutSet< shared_ptr<const AnaJet> > jet_top_common_sel( "jet_top_common_selection" );
  jet_top_common_sel.add_u( "jet_type" , 
                            bind(&AnaJet::algorithm,_1) == AnaJet::Algorithm::ATLAS_ANTI_KT_CONE_4_H1_TOPO );
  jet_top_common_sel.add_uu( "eta" , 
                             bind( abs_value(AnaJet) , bind(&AnaJet::eta,_1)) < 2.5 ,
                             bind( &dg::fillh , _1 , 60 , -3 , 3 , bind(&AnaJet::eta,_2) ) );

  EventObjectColl< shared_ptr<const AnaJet> > jet_loose_top;
  DgCutSet< shared_ptr<const AnaJet> > jet_loose_top_sel( "jet_loose_top_selection" );
  jet_loose_top_sel.based_on( jet_top_common_sel );
  jet_loose_top_sel.add_uu( "pt" , 
                            bind( &TLorentzVector::Pt , bind(&AnaJet::em_scale_momentum,_1) ) > 20 ,
                            bind(&dg::fillh,_1,100,0,100,bind(&TLorentzVector::Pt,bind(&AnaJet::em_scale_momentum,_2))) );

  EventObjectColl< shared_ptr<const AnaJet> > jet_tight_top;
  DgCutSet< shared_ptr<const AnaJet> > jet_tight_top_sel( "jet_tight_top_selection" );
  jet_tight_top_sel.based_on( jet_top_common_sel );
  jet_tight_top_sel.add_uu( "pt" , 
                            bind( &TLorentzVector::Pt , bind(&AnaJet::em_scale_momentum,_1) ) > 40 ,
                            bind(&dg::fillh,_1,100,0,100,bind(&TLorentzVector::Pt,bind(&AnaJet::em_scale_momentum,_2))) );

  // w selection
  EventObjectColl< shared_ptr<const AnaW> > w_all_candidates;
  EventObjectColl< shared_ptr<const AnaW> > w_all_ref_final_candidates;
  EventObjectColl< shared_ptr<const AnaW> > w_top;
  EventObjectColl< shared_ptr<const AnaW> > w_top_ref_final;
  DgCutSet< shared_ptr<const AnaW> > w_top_sel( "w_top_selection" );
  w_top_sel.add_uu( "mT" , 
                    bind( &AnaW::transverse_mass , _1 ) >= 30. ,
                    bind( &dg::fillh , _1 , 100 , 0 , 100 , bind(&AnaW::transverse_mass, _2) , "Transverse Mass (GeV)") );
  

  // event loop
  // ================================================================
  unsigned long n_events = 0ul;

  AnaEventMgr* evtmgr = AnaEventMgr::instance(); assert(evtmgr);
  for( AnaEventMgr::iterator i=evtmgr->begin(), f=evtmgr->end(AnaConfiguration::user_max_nevents()); i!=f; ++i, ++n_events ) {

    const shared_ptr<const AnaEvent>& evt( *i );
    if( !evt ) { break; } // no AnaEvent means either we're out of events or something went wrong, e.g. premature end of file.
    
    // break down sample into "data" and MC truth categories
    std::string event_type = "data";
    if( evt->truth_table() && !(evt->truth_table()->empty()) ) {
      switch( evt->truth_table()->event_type() ) {
      case AnaTruthTable::TTBAR: event_type = "ttbar"; break;
      case AnaTruthTable::SMWW: event_type = "smww"; break;
      case AnaTruthTable::SMWZ: event_type = "smwz"; break;
      case AnaTruthTable::SMZZ: event_type = "smzz"; break;
      case AnaTruthTable::SINGLETOP: event_type = "singletop"; break;
      case AnaTruthTable::WBOSON: event_type = "wboson"; break;
      case AnaTruthTable::ZBOSON: event_type = "zboson"; break;
      case AnaTruthTable::OTHER: event_type = "other"; break;
      default: event_type = "other"; break;
      }
    }
    dg::down( event_type , "sample categorization" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;

    // check sample type and decide whether or not we need to make
    // data-only or mc-only preselection cuts. then make them.
    // ================================================================
    AnaEvent::tribool is_mc = evt->check_flag( "mc" );
    const bool do_not_require_good_run = is_mc;
    // check good run list
    const bool is_good_run_ele = do_not_require_good_run ? true : grl.is_in( GoodRunList::ELECTRON , evt->run_number() , evt->lumi_block() );
    const bool is_good_run_mu = do_not_require_good_run ? true : grl.is_in( GoodRunList::MUON , evt->run_number() , evt->lumi_block() );;
    dg::filleff( "is_good_run_ele" , is_good_run_ele );
    dg::filleff( "is_good_run_mu" , is_good_run_mu );

    if( !is_good_run_mu ) { continue; }

    // make muon selection plots
    mu_top_sel.for_each( evt->muons().begin() , evt->muons().end() );
    mu_trigger_top_sel.for_each( evt->muons().begin() , evt->muons().end() );
    // select muons
    mu_top.fill_for_each_if( evt->muons().begin() , evt->muons().end() , mu_top_sel );
    mu_trigger_top.fill_for_each_if( evt->muons().begin() , evt->muons().end() , mu_trigger_top_sel );

    // make met selection plots
    met_top_sel.for_each( evt->missing_et().begin() , evt->missing_et().end() );
    met_ref_sel.for_each( evt->missing_et().begin() , evt->missing_et().end() );
    // select met
    met_top.fill_for_each_if( evt->missing_et().begin() , evt->missing_et().end() , met_top_sel );
    met_ref.fill_for_each_if( evt->missing_et().begin() , evt->missing_et().end() , met_ref_sel );

    // make jet selection plots
    jet_loose_top_sel.for_each( evt->atlas_anti_kt_4_h1_topo_jets().begin() , evt->atlas_anti_kt_4_h1_topo_jets().end() );
    jet_tight_top_sel.for_each( evt->atlas_anti_kt_4_h1_topo_jets().begin() , evt->atlas_anti_kt_4_h1_topo_jets().end() );
    // select jets
    jet_loose_top.fill_for_each_if( evt->atlas_anti_kt_4_h1_topo_jets().begin() , evt->atlas_anti_kt_4_h1_topo_jets().end() , jet_loose_top_sel );
    jet_tight_top.fill_for_each_if( evt->atlas_anti_kt_4_h1_topo_jets().begin() , evt->atlas_anti_kt_4_h1_topo_jets().end() , jet_tight_top_sel );

    // BOOST_FOREACH( const shared_ptr<const AnaMuon>& mu , evt->muons() ) {
    //   const DataGraphics::DgCutResult result( mu_trigger_top_sel.cut(mu) );
    //   if( result.passes( "trigger_mu10") && result.passes("primary_author") && result.passes("pt") && result.passes("eta") ) {
    //     cout << " mu: " << mu->pt_cone( AnaMuon::CONE30 ) << " " << mu->et_cone( AnaMuon::CONE20 ) << " " << mu->topinputs() << endl;
    //   }
    // }

    // properties of events with a good muon
    if( !mu_top.empty() ) {
      dg::down( "events_with_good_muon" , "events with a good muon candidate" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      met_top.fill_for_each_if( evt->missing_et().begin() , evt->missing_et().end() , met_top_sel );
      met_ref.fill_for_each_if( evt->missing_et().begin() , evt->missing_et().end() , met_ref_sel );
      jet_loose_top_sel.for_each( evt->atlas_anti_kt_4_h1_topo_jets().begin() , evt->atlas_anti_kt_4_h1_topo_jets().end() );
      jet_tight_top_sel.for_each( evt->atlas_anti_kt_4_h1_topo_jets().begin() , evt->atlas_anti_kt_4_h1_topo_jets().end() );
      dg::fillh( "n_loose_jets" , 10 , 0 , 10 , jet_loose_top.size() );
      dg::fillh( "n_tight_jets" , 10 , 0 , 10 , jet_tight_top.size() );
    }

    // build W candidates.
    CompositeGenerator< const AnaParticle , const AnaW > w_generator;
    w_generator.generate( mu_trigger_top.begin() , mu_trigger_top.end() , 
                          met_top.begin() , met_top.end() ,
                          AnaW::Compose() , w_all_candidates );
    w_generator.generate( mu_trigger_top.begin() , mu_trigger_top.end() , 
                          met_ref.begin() , met_ref.end() ,
                          AnaW::Compose() , w_all_ref_final_candidates );
    dg::fillh( "n_w_all_candidates" , 10 , 0 , 10 , w_all_candidates.size() );
    dg::fillh( "n_w_all_ref_final_candidates" , 10 , 0 , 10 , w_all_ref_final_candidates.size() );
    dg::filleff( "has_w_all_candidate" , !(w_all_candidates.empty()) );
    dg::filleff( "has_w_all_ref_final_candidate" , !(w_all_ref_final_candidates.empty()) );
    // make W selection plots
    w_top_sel.for_each( w_all_candidates.begin() , w_all_candidates.end() );
    // select W candidates for this event
    w_top.fill_for_each_if( w_all_candidates.begin() , w_all_candidates.end() , w_top_sel );
    w_top_ref_final.fill_for_each_if( w_all_ref_final_candidates.begin() , w_all_ref_final_candidates.end() , w_top_sel );
    dg::fillh( "n_w_top_candidates" , 10 , 0 , 10 , w_top.size() );
    dg::filleff( "has_w_top_candidate" , !(w_top.empty()) );
    dg::fillh( "n_w_top_ref_final_candidates" , 10 , 0 , 10 , w_top_ref_final.size() );
    dg::filleff( "has_w_top_ref_final_candidate" , !(w_top_ref_final.empty()) );
    
    // study W properties
    StudySemileptonicW stw( "top_selection_w_candidates" , "all W candidates passing top selection" );
    stw.study_n_objects( true );
    stw.add_selector( "nearest_to_MW" , AnaParticle::ClosestTransverseMassToW<const AnaW>() );
    stw.add_constant_category( "njets_gte_4" , jet_loose_top.size() >= 4 );
    stw.for_each( w_top.begin() , w_top.end() );
    stw.repurpose( "top_ref_final_selection_w_candidates" , "all W candidates passing top selection with REF_FINAL MET calculation" );
    stw.for_each( w_top_ref_final.begin() , w_top_ref_final.end() );

    // study jet properties in events with at least one good
    // semileptonic W.
    if( !w_top.empty() ) {
      StudyJet stj( "top_selection_jet_candidates" , "'loose' jets passing top selection in events with a good W candidate" );
      stj.study_n_objects( true );
      stj.add_selector( "highest_pt_jet" , AnaParticle::HighestPt<const AnaJet>() );
      stj.add_selector( "lowest_eta" , AnaParticle::LowestAbsEta<const AnaJet>() );
      stj.add_selector( "highest_eta" , AnaParticle::HighestAbsEta<const AnaJet>() );
      stj.add_category( "sv0_w_gt_6" , bind( &AnaJet::tag_sv0_w, _1 ) > 6 );
      // may 50% eff recommendation from https://twiki.cern.ch/twiki/bin/view/AtlasProtected/TopCommonObjects#B_tagging
      stj.add_category( "sv0_w_gt_5.72" , bind( &AnaJet::tag_sv0_w, _1 ) > 5.72 ); 
      stj.add_category( "sv0_w_gt_4" , bind( &AnaJet::tag_sv0_w, _1 ) > 4 ); 
      stj.add_category( "sv0_w_gt_2" , bind( &AnaJet::tag_sv0_w, _1 ) > 2 ); 
      stj.add_category( "sv0_w_lt_1" , bind( &AnaJet::tag_sv0_w, _1 ) < 1 );
      stj.for_each( jet_loose_top.begin() , jet_loose_top.end() );
      dg::down( "events_with_wcand" , "events with a w candidate" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      jet_loose_top_sel.for_each( evt->atlas_anti_kt_4_h1_topo_jets().begin() , evt->atlas_anti_kt_4_h1_topo_jets().end() );
      jet_tight_top_sel.for_each( evt->atlas_anti_kt_4_h1_topo_jets().begin() , evt->atlas_anti_kt_4_h1_topo_jets().end() );
      dg::fillh( "n_loose_jets" , 10 , 0 , 10 , jet_loose_top.size() );
      dg::fillh( "n_tight_jets" , 10 , 0 , 10 , jet_tight_top.size() );
    }

    // study event properties in events with at least one b tagged jet.
    // may 50% eff recommendation from https://twiki.cern.ch/twiki/bin/view/AtlasProtected/TopCommonObjects#B_tagging
    const unsigned int n_top_sv0_jets = std::count_if( jet_loose_top.begin() , jet_loose_top.end() , bind(&AnaJet::tag_sv0_w,_1) > 5.72 );
    if( !w_top.empty() && n_top_sv0_jets > 0 ) {
      dg::down( "events_with_sv0_b_jets" , "events with b jets" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      StudyJet stj( "top_selection_jet_candidates" , "'loose' jets passing top selection in events with a good W candidate" );
      stj.study_n_objects( true );
      stj.add_category( "bjets_sv0" , bind( &AnaJet::tag_sv0_w,_1) > 5.72 );
      stj.add_category( "non_bjets_sv0" , bind( &AnaJet::tag_sv0_w,_1) < 5.72 );
      stj.add_category( "not_bjets_sv0" , bind( &AnaJet::tag_sv0_w,_1) < 1 );
      stj.for_each( jet_loose_top.begin() , jet_loose_top.end() );
      StudySemileptonicW stw( "top_selection_w_candidates" , "all W candidates passing top selection" );
      stw.study_n_objects( true );
      stw.add_selector( "nearest_to_MW" , AnaParticle::ClosestTransverseMassToW<const AnaW>() );
      stw.add_constant_category( "nbjets_1_njets_gte_1" , jet_loose_top.size() >= 1 && n_top_sv0_jets >= 1 );
      stw.add_constant_category( "nbjets_1_njets_gte_2" , jet_loose_top.size() >= 2 && n_top_sv0_jets >= 1 );
      stw.add_constant_category( "nbjets_1_njets_gte_3" , jet_loose_top.size() >= 3 && n_top_sv0_jets >= 1 );
      stw.add_constant_category( "nbjets_1_njets_gte_4" , jet_loose_top.size() >= 4 && n_top_sv0_jets >= 1 );
      stw.add_constant_category( "nbjets_1_njets_gte_5" , jet_loose_top.size() >= 5 && n_top_sv0_jets >= 1 );
      stw.add_constant_category( "nbjets_1_njets_gte_6" , jet_loose_top.size() >= 6 && n_top_sv0_jets >= 1 );
      stw.add_constant_category( "nbjets_1_njets_gte_7" , jet_loose_top.size() >= 7 && n_top_sv0_jets >= 1 );
      stw.add_constant_category( "nbjets_1_njets_gte_8" , jet_loose_top.size() >= 8 && n_top_sv0_jets >= 1 );
      stw.add_constant_category( "nbjets_2_njets_gte_1" , jet_loose_top.size() >= 1 && n_top_sv0_jets >= 2 );
      stw.add_constant_category( "nbjets_2_njets_gte_2" , jet_loose_top.size() >= 2 && n_top_sv0_jets >= 2 );
      stw.add_constant_category( "nbjets_2_njets_gte_3" , jet_loose_top.size() >= 3 && n_top_sv0_jets >= 2 );
      stw.add_constant_category( "nbjets_2_njets_gte_4" , jet_loose_top.size() >= 4 && n_top_sv0_jets >= 2 );
      stw.add_constant_category( "nbjets_2_njets_gte_5" , jet_loose_top.size() >= 5 && n_top_sv0_jets >= 2 );
      stw.add_constant_category( "nbjets_2_njets_gte_6" , jet_loose_top.size() >= 6 && n_top_sv0_jets >= 2 );
      stw.add_constant_category( "nbjets_2_njets_gte_7" , jet_loose_top.size() >= 7 && n_top_sv0_jets >= 2 );
      stw.add_constant_category( "nbjets_2_njets_gte_8" , jet_loose_top.size() >= 8 && n_top_sv0_jets >= 2 );
      stw.for_each( w_top.begin() , w_top.end() );
      stw.repurpose( "top_ref_final_selection_w_candidates" , "all W candidates passing top selection with REF_FINAL MET calculation" );
      stw.for_each( w_top_ref_final.begin() , w_top_ref_final.end() );
    }

    // skim events with good W to output ntuple.
    if( AnaEventMgr::instance()->ana_event_ntuple_mode() == AnaEventNtuple::WRITE ) {
      AnaEventMgr::instance()->add_to_ana_event_ntuple( !w_top.empty() );
    }

  } // end for each event

  AnaEventMgr::instance()->close_sample();
  
}

#ifdef F77_DUMMY_MAIN
#ifdef __cplusplus
extern "C"
#endif
int F77_DUMMY_MAIN() { abort(); return 1; }
#endif
