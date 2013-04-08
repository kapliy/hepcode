
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
#include "TrigFTKAna/AnaWCrossSectionCandidate.hpp"
#include "TrigFTKAna/StudyWCrossSection.hpp"
#include "TrigFTKAna/EventObjectColl.hpp"
#include "TrigFTKAna/GoodRunList.hpp"
#include "DataGraphics/DataGraphics.hh"
#include "DataGraphics/DgCutSet.hh"
#include "DataGraphics/DgSTL.hh"
#include "TLorentzVector.h"

using namespace std;
using namespace DataGraphics;
using namespace boost;

#define boost_foreach BOOST_FOREACH

template< typename lepIterT , typename metIterT , typename jetIterT , typename vtxIterT , typename outputIterT >
void build_w_candidates( lepIterT lep_begin , lepIterT lep_end ,
                         metIterT met_begin , metIterT met_end ,
                         jetIterT jet_begin , jetIterT jet_end ,
                         vtxIterT vtx_begin , vtxIterT vtx_end ,
                         outputIterT output_iterator );

template< typename overlapIterT , typename refIterT , typename outputIterT >
void overlap_removal( overlapIterT overlap_begin , overlapIterT overlap_end ,
                      refIterT ref_begin , refIterT ref_end ,
                      const AnaParticle::ftype& max_overlap_dr ,
                      outputIterT output_iterator );

template< typename anaEventPtrT >
const boost::shared_ptr<const AnaVertex>& 
get_primary_vertex( anaEventPtrT evt )
{
  const boost::shared_ptr<const AnaVertex>* choice = 0;
  std::vector< boost::shared_ptr<const AnaVertex> >::const_iterator i = std::max_element( evt->vertices().begin() , evt->vertices().end() , 
                                                                                          boost::bind( &AnaVertex::sum_pt , _1 ) < boost::bind( &AnaVertex::sum_pt , _2 ) );
  if( i == evt->vertices().end() ) {
    static boost::shared_ptr<const AnaVertex> null;
    return null;
  }
  return *i;
}

////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C"
#endif
int
F77_MAIN( int argc , char* argv[] )
{

  AnaConfiguration::configure("ana_wjets",argc,argv);
  if( !AnaConfiguration::configured() ) { exit(0); }
  dg::root()->no_save_at_exit();
  BOOST_SCOPE_EXIT() { dg::root()->do_save_at_exit(); } BOOST_SCOPE_EXIT_END;

  // define lists of analysis objects for each event. insert each
  // object collection into this list. each EventObjectColl will
  // automatically get cleared at the beginning of each event.

  // implement selections. includes W/Z group 2010 "Strawman"
  // selections described at:
  // (https://twiki.cern.ch/twiki/bin/view/AtlasProtected/StandardModelWZjetsSubgroup?sortcol=1;table=4;up=0#sorted_table;)

  // event preselection
  // ================================================================
  GoodRunList grl;
  grl.add( GoodRunList::ELECTRON , "good_run_lists/Wel_GRLs/WjetsEl_MayReprAB_FirstPassCDEFGHI_GRL.xml");
  grl.add( GoodRunList::MUON , "good_run_lists/Wmu_GRLs/WjetsMu_MayReprAB_FirstPassCDEFGHI_GRL.xml");
  // grl.print( GoodRunList::ELECTRON , std::cout );
  // grl.print( GoodRunList::MUON , std::cout );

  // define primary vertex selections
  // ================================================================
  EventObjectColl<  boost::shared_ptr<const AnaVertex>  > vtx_baseline;
  DgCutSet<  boost::shared_ptr<const AnaVertex>  > vtx_baseline_sel( "vtx_baseline" );
  // There's an AnaVertex::is_good_primary_candidate function that does this, but I'm getting them separately
  vtx_baseline_sel.add_uu( "nTracks", 
                           bind(&AnaVertex::ntracks,_1)>=3,
                           bind( &dg::fillh , _1 , 15 , 0 , 15 , bind(&AnaVertex::ntracks,_2)) );
  vtx_baseline_sel.add_uu( "zVtx", 
                           bind(std::ptr_fun<AnaVertex::ftype,AnaVertex::ftype>(std::abs),
                                bind(&AnaVertex::z,_1))<150., //mm
                           bind( &dg::fillh , _1 , 90 , -300 , 300 , bind(&AnaVertex::z,_2) ) );

  // define electron selections
  // ================================================================

  EventObjectColl<  boost::shared_ptr<const AnaElectron>  > ele_baseline;

  // W/Z Baseline 2010 selection
  DgCutSet<  boost::shared_ptr<const AnaElectron>  > ele_baseline_sel_core( "ele_baseline_sel_core" );
  ele_baseline_sel_core.add_uu( "author" , 
                                bind(&AnaElectron::author,_1)==1 || bind(&AnaElectron::author,_1)==3 ,
                                bind( &dg::fillh , _1 , 10 , 0 , 10 , bind(&AnaElectron::author,_2) ) );
  ele_baseline_sel_core.add_u( "passOTX" , bind(&AnaElectron::pass_otx,_1)==true );
  switch ( AnaConfiguration::elec_trig() ) {
  case 1 :  // Trigger for MC and periods A - E3
  case 0 :
    ele_baseline_sel_core.add_gu( "trigger_L1_EM14" ,
                                  bind( &AnaTrigger::l1_em14 , bind(&AnaEvent::trigger,boost::ref(AnaEventMgr::current_event())) ) == true ,
                                  bind( &dg::filleff , _1 , 60 , -3 , 3 , bind(&AnaElectron::cluster_eta,_2) ,
                                        bind(&AnaTrigger::l1_em14  , 
                                             bind(&AnaEvent::trigger,boost::ref(AnaEventMgr::current_event()))) == true ) );
    break;
  case 2 : // Alternate electron trigger to use in data periods E4 and on (as of 12/18/2010)
    ele_baseline_sel_core.add_gu( "trigger_EF_e15_medium" ,
                                  bind( &AnaTrigger::ef_e15_medium , bind(&AnaEvent::trigger,boost::ref(AnaEventMgr::current_event())) ) == true ,
                                  bind( &dg::filleff , _1 , 60 , -3 , 3 , bind(&AnaElectron::cluster_eta,_2) ,
                                        bind(&AnaTrigger::ef_e15_medium  ,
                                             bind(&AnaEvent::trigger,boost::ref(AnaEventMgr::current_event()))) == true ) );
    break;
  case 3 : // Alternate electron trigger to use in data periods E4-F for photon MET selection
    ele_baseline_sel_core.add_gu( "trigger_EF_g20_loose" ,
                                  bind( &AnaTrigger::ef_g20_loose , bind(&AnaEvent::trigger,boost::ref(AnaEventMgr::current_event())) ) == true ,
                                  bind( &dg::filleff , _1 , 60 , -3 , 3 , bind(&AnaElectron::cluster_eta,_2) ,
                                        bind(&AnaTrigger::ef_g20_loose  ,
                                             bind(&AnaEvent::trigger,boost::ref(AnaEventMgr::current_event()))) == true ) );
    break;
  default :
    cout << " invalid electron trigger " << AnaConfiguration::elec_trig() << endl;
    exit(-1);
  } // end trigger selection
  ele_baseline_sel_core.add_uu( "cluster_eta" , 
                                ( bind( std::ptr_fun<AnaElectron::ftype,AnaElectron::ftype>(std::abs) , 
                                        bind(&AnaElectron::cluster_eta,_1)) < 2.47  &&
                                  ( bind(std::ptr_fun<AnaElectron::ftype,AnaElectron::ftype>(std::abs) , 
                                         bind(&AnaElectron::cluster_eta,_1)) < 1.37 ||
                                    bind(std::ptr_fun<AnaElectron::ftype,AnaElectron::ftype>(std::abs) , 
                                         bind(&AnaElectron::cluster_eta,_1)) > 1.52 ) ) ,
                                bind( &dg::fillh , _1 , 60 , -3 , 3 , bind(&AnaElectron::cluster_eta,_2) ) );
  ele_baseline_sel_core.add_uu( "cluster_ET" ,
                                bind(&AnaElectron::cluster_pt,_1) > 20. ,
                                bind(&dg::fillh,_1,100,0,100,bind(&AnaElectron::cluster_pt,_2)) );
  
  DgCutSet<  boost::shared_ptr<const AnaElectron>  > ele_baseline_sel( "ele_baseline" );
  ele_baseline_sel.based_on( ele_baseline_sel_core );
  ele_baseline_sel.add_uu( "isEM_robusterTight" ,
                           bind(&AnaElectron::is_ElectronRobusterTight,_1) == true ,
                           bind( &dg::filleff , _1 , bind(&AnaElectron::is_ElectronRobusterTight,_2) == true ) );

  // This bit here is to veto Zee
  DgCutSet<  boost::shared_ptr<const AnaElectron>  > ele_baseline_sel_other( "ele_baseline_sel_other" );
  ele_baseline_sel_other.based_on( ele_baseline_sel_core );
  ele_baseline_sel_other.add_uu( "isEM_robustMedium" ,
                                 bind(&AnaElectron::is_ElectronRobustMedium,_1) == true ,
                                 bind( &dg::filleff , _1 , bind(&AnaElectron::is_ElectronRobustMedium,_2) == true ) );

  // define photon selections
  // This is used to select photons from QCD events in real
  // data. Based on http://cdsweb.cern.ch/record/1292774 version 7
  // lines 262-268
  // ================================================================
  EventObjectColl<  boost::shared_ptr<const AnaPhoton>  > photon_baseline;

  // W/Z Baseline 2010 selection
  DgCutSet<  boost::shared_ptr<const AnaPhoton>  > photon_baseline_sel( "photon_baseline" );
  photon_baseline_sel.add_uu( "author" , 
                              bind(&AnaPhoton::author,_1)==4,
                              bind( &dg::fillh , _1 , 10 , 0 , 10 , bind(&AnaPhoton::author,_2) ) );
  photon_baseline_sel.add_u( "passOTX" , bind(&AnaPhoton::pass_otx,_1)==true );
  switch ( AnaConfiguration::phot_trig() ) {
  case 1 :  // Trigger for MC and periods A - E3
  case 0 :
    photon_baseline_sel.add_gu( "trigger_L1_EM14" ,
                                bind( &AnaTrigger::l1_em14 , bind(&AnaEvent::trigger,boost::ref(AnaEventMgr::current_event())) ) == true ,
                                bind( &dg::filleff , _1 , 60 , -3 , 3 , bind(&AnaPhoton::cluster_eta,_2) ,
                                      bind(&AnaTrigger::l1_em14  ,
                                           bind(&AnaEvent::trigger,boost::ref(AnaEventMgr::current_event()))) == true ) );
    break;
  case 2 : // Alternate electron trigger to use in data periods E4 and on (as of 12/18/2010)
    photon_baseline_sel.add_gu( "trigger_EF_e15_medium" ,
                                bind( &AnaTrigger::ef_e15_medium , bind(&AnaEvent::trigger,boost::ref(AnaEventMgr::current_event())) ) == true ,
                                bind( &dg::filleff , _1 , 60 , -3 , 3 , bind(&AnaPhoton::cluster_eta,_2) ,
                                      bind(&AnaTrigger::ef_e15_medium  ,
                                           bind(&AnaEvent::trigger,boost::ref(AnaEventMgr::current_event()))) == true ) );
    break;
  case 3 : // Alternate electron trigger to use in data periods E4-F for photon MET selection
    photon_baseline_sel.add_gu( "trigger_EF_g20_loose" ,
                                bind( &AnaTrigger::ef_g20_loose , bind(&AnaEvent::trigger,boost::ref(AnaEventMgr::current_event())) ) == true ,
                                bind( &dg::filleff , _1 , 60 , -3 , 3 , bind(&AnaPhoton::cluster_eta,_2) ,
                                      bind(&AnaTrigger::ef_g20_loose  ,
                                           bind(&AnaEvent::trigger,boost::ref(AnaEventMgr::current_event()))) == true ) );
    break;
  default :
    cout << " invalid photon trigger " << AnaConfiguration::phot_trig() << endl;
    exit(-1);
  } // end trigger selection
  photon_baseline_sel.add_uu( "cluster_eta" , 
                              ( bind( std::ptr_fun<AnaPhoton::ftype,AnaPhoton::ftype>(std::abs) , 
                                      bind(&AnaPhoton::cluster_eta,_1)) < 2.47  &&
                                ( bind(std::ptr_fun<AnaPhoton::ftype,AnaPhoton::ftype>(std::abs) , 
                                       bind(&AnaPhoton::cluster_eta,_1)) < 1.37 ||
                                  bind(std::ptr_fun<AnaPhoton::ftype,AnaPhoton::ftype>(std::abs) , 
                                       bind(&AnaPhoton::cluster_eta,_1)) > 1.52 ) ) ,
                              bind( &dg::fillh , _1 , 60 , -3 , 3 , bind(&AnaPhoton::cluster_eta,_2) ) );
  photon_baseline_sel.add_uu( "cluster_ET" ,
                              bind(&AnaPhoton::cluster_pt,_1) > 20. ,
                              bind(&dg::fillh,_1,100,0,100,bind(&AnaPhoton::cluster_pt,_2)) );
  
  photon_baseline_sel.add_uu( "isEM_PhotonLoose" ,
                              bind(&AnaPhoton::is_PhotonLoose,_1) == true ,
                              bind( &dg::filleff , _1 , bind(&AnaPhoton::is_PhotonLoose,_2) == true ) );
  
  photon_baseline_sel.add_uu( "etCone30ratio" , 
                              bind(&AnaPhoton::et_cone_ratio,_1,AnaPhoton::CONE30) > 0.15,
                              bind(&dg::fillh,_1,100,0,1,bind(&AnaPhoton::et_cone_ratio,_2,AnaPhoton::CONE30)) );

  // define muon selections
  // ================================================================

  EventObjectColl<  boost::shared_ptr<const AnaMuon>  > mu_baseline;

  DgCutSet<  boost::shared_ptr<const AnaMuon>  > mu_baseline_sel( "mu_baseline" );
  mu_baseline_sel.add_uu( "staco" , bind(&AnaMuon::is_staco_combined,_1) ==true , bind( &dg::filleff , _1 , _2 ) );
  mu_baseline_sel.add_uu( "eta" , 
                          bind( std::ptr_fun<AnaMuon::ftype,AnaMuon::ftype>(std::abs) , 
                                bind(&AnaMuon::eta,_1)) < 2.4 ,
                          bind( &dg::fillh , _1 , 60 , -3 , 3 , bind(&AnaMuon::eta,_2) ) );
  switch ( AnaConfiguration::muon_trig() ) {
  case 1 :  // Trigger for MC and periods A - E3
  case 0 :
    mu_baseline_sel.add_gu( "trigger_L1_MU10" , 
                            bind( &AnaTrigger::l1_mu10 , bind(&AnaEvent::trigger,boost::ref(AnaEventMgr::current_event())) )==true,
                            bind( &dg::filleff , _1 , 60 , -3 , 3 , bind(&AnaMuon::eta,_2) ,
                                  bind(&AnaTrigger::l1_mu10 , 
                                       bind(&AnaEvent::trigger,boost::ref(AnaEventMgr::current_event())))==true ) );
    break;
  case 2 : // alternate trigger for periods E4 - G1
    mu_baseline_sel.add_gu( "trigger_EF_MU10_MG" ,
                            bind( &AnaTrigger::ef_mu10_mg , bind(&AnaEvent::trigger,boost::ref(AnaEventMgr::current_event())) )==true,
                            bind( &dg::filleff , _1 , 60 , -3 , 3 , bind(&AnaMuon::eta,_2) ,
                                  bind(&AnaTrigger::ef_mu10_mg ,
                                       bind(&AnaEvent::trigger,boost::ref(AnaEventMgr::current_event())))==true ) );
    break;
  case 3 : // alternate trigger for periods G2 to I1 (through 167576)
    mu_baseline_sel.add_gu( "trigger_EF_MU13_MG" ,
                            bind( &AnaTrigger::ef_mu13_mg , bind(&AnaEvent::trigger,boost::ref(AnaEventMgr::current_event())) )==true,
                            bind( &dg::filleff , _1 , 60 , -3 , 3 , bind(&AnaMuon::eta,_2) ,
                                  bind(&AnaTrigger::ef_mu13_mg ,
                                       bind(&AnaEvent::trigger,boost::ref(AnaEventMgr::current_event())))==true ) );
    break;
  case 4 : // alternate trigger for periods I1 (from 167607) to I2
    mu_baseline_sel.add_gu( "trigger_EF_MU13_MG_TIGHT" ,
                            bind( &AnaTrigger::ef_mu13_mg_tight , bind(&AnaEvent::trigger,boost::ref(AnaEventMgr::current_event())) )==true,
                            bind( &dg::filleff , _1 , 60 , -3 , 3 , bind(&AnaMuon::eta,_2) ,
                                  bind(&AnaTrigger::ef_mu13_mg_tight ,
                                       bind(&AnaEvent::trigger,boost::ref(AnaEventMgr::current_event())))==true ) );
    break;
  default :
    cout << " invalid muon trigger " << AnaConfiguration::muon_trig() << endl;
    exit(-1);
  } // end trigger selection
  mu_baseline_sel.add_uu( "pt" , bind(&AnaMuon::pt,_1) > 20. , bind(&dg::fillh,_1,100,0,100,bind(&AnaMuon::pt,_2)) );
  mu_baseline_sel.add_uu( "ptCone20" , 
                          bind(&AnaMuon::pt_cone,_1,AnaMuon::CONE20) < 1.8 /*GeV*/ , 
                          bind(&dg::fillh,_1,100,0,10,bind(&AnaMuon::pt_cone,_2,AnaMuon::CONE20)) );
  // Quality cuts:
  mu_baseline_sel.add_u( "trackerHits", 
                         ( bind( std::ptr_fun<AnaMuon::ftype,AnaMuon::ftype>(std::abs) , 
                                 bind(&AnaMuon::eta,_1)) > 2.0 || 
                           ( bind(&AnaMuon::nhits,_1,AnaMuon::PIXEL) > 1 &&
                             bind(&AnaMuon::nhits,_1,AnaMuon::SCT) > 5 &&
                             bind(&AnaMuon::nhits,_1,AnaMuon::TRT) > 0 ) ) );

  mu_baseline_sel.add_u( "ptDiffRatio", 
                         bind( std::ptr_fun<AnaMuon::ftype,AnaMuon::ftype>(std::abs) , bind(&AnaMuon::pt_diff_ratio,_1)) < 0.5 );
  mu_baseline_sel.add_uu( "ptms" , bind(&AnaMuon::exms_pt,_1) > 10. , bind(&dg::fillh,_1,100,0,100,bind(&AnaMuon::exms_pt,_2)) );

  // define jet selections
  // ================================================================

  EventObjectColl<  boost::shared_ptr<const AnaJet>  > elejet_baseline;
  EventObjectColl<  boost::shared_ptr<const AnaJet>  > jet_baseline_afterOv;   // after overlap removal
  EventObjectColl<  boost::shared_ptr<const AnaJet>  > jet_baseline;   // final selection
  EventObjectColl<  boost::shared_ptr<const AnaJet>  > jet_baseline_higheta;   // final selection

  DgCutSet<  boost::shared_ptr<const AnaJet>  > jet_baseline_common_sel( "jet_baseline_common_sel" );
  jet_baseline_common_sel.add_uu( "algorithm_antikt4" , bind(&AnaJet::is_atlas_anti_kt_cone_4_h1_topo,_1)==true ,
                                  bind(&dg::filleff,_1,bind(&AnaJet::is_atlas_anti_kt_cone_4_h1_topo,_2)==true) );
  jet_baseline_common_sel.add_uu( "isGood" , bind(&AnaJet::is_good_jetetmiss_v15,_1)==true ,
                                  bind(&dg::filleff,_1,bind(&AnaJet::is_good_jetetmiss_v15,_2)==true) );
  DgCutSet<  boost::shared_ptr<const AnaJet>  > jet_baseline_noeta_sel( "jet_baseline_noeta" );
  jet_baseline_noeta_sel.add_uu( "pt" , bind(&AnaJet::pt,_1) > 20. , bind(&dg::fillh,_1,100,0,100,bind(&AnaJet::pt,_2)) );
  jet_baseline_noeta_sel.add_uu("vertex_fraction",
                                bind(std::ptr_fun<AnaJet::ftype,AnaJet::ftype>(std::abs),
                                     bind(&AnaJet::vtx_fraction,_1)) >= 0.75,
                                bind(&dg::fillh,_1,100,-1,1, bind(&AnaJet::vtx_fraction,_2)));
  // baseline selection: only go out to eta = 2.8
  DgCutSet<  boost::shared_ptr<const AnaJet>  > jet_baseline_sel( "jet_baseline" );
  jet_baseline_sel.based_on( jet_baseline_noeta_sel );
  jet_baseline_sel.add_uu( "eta" ,
                           bind( std::ptr_fun<AnaJet::ftype,AnaJet::ftype>(std::abs) , bind(&AnaJet::eta,_1)) < 2.8 ,
                           bind(&dg::fillh,_1,100,-3,3,bind(&AnaJet::eta,_2)) );

  // go out to eta = 4.5 but ignore jets with negative energy
  DgCutSet<  boost::shared_ptr<const AnaJet>  > jet_baseline_higheta_sel( "jet_baseline_higheta" );
  jet_baseline_higheta_sel.based_on( jet_baseline_noeta_sel );
  jet_baseline_higheta_sel.add_uu( "eta" ,
                                   bind( std::ptr_fun<AnaJet::ftype,AnaJet::ftype>(std::abs) , bind(&AnaJet::eta,_1)) < 4.5 ,
                                   bind(&dg::fillh,_1,100,-3,3,bind(&AnaJet::eta,_2)) );

  // merged lepton collections
  // ================================================================
  EventObjectColl<  boost::shared_ptr<const AnaParticle>  > lepton_baseline;
  EventObjectColl<  boost::shared_ptr<const AnaParticle>  > photon_particle_baseline;

  // MET selection
  // ================================================================
  EventObjectColl<  boost::shared_ptr<const AnaMET>  > met_baseline;
  EventObjectColl<  boost::shared_ptr<const AnaMET>  > met_baseline_higheta;

  // W/Z Baseline electron MET selection
  DgCutSet<  boost::shared_ptr<const AnaMET>  > met_baseline_el_sel( "met_baseline_el" );
  met_baseline_el_sel.add_u( "met_type" , bind(&AnaMET::met_type,_1) == AnaMET::LOCHADTOPO );
  met_baseline_el_sel.add_u( "missing_et_good" , bind( &AnaMET::is_good, _1 ) == true );
  met_baseline_el_sel.add_uu( "missing_et" , bind( &AnaMET::met , _1 ) > 0. , 
                              bind(&dg::fillh,_1,100,0,100, bind(&AnaMET::met,_2)) );

  // W/Z Baseline electron MET selection with adjustment for higheta (FCAL)
  DgCutSet<  boost::shared_ptr<const AnaMET>  > met_baseline_elhigheta_sel( "met_baseline_elhigheta" );
  met_baseline_elhigheta_sel.add_u( "met_type" , bind(&AnaMET::met_type,_1) == AnaMET::LOCHADTOPOELHIGHETA );
  met_baseline_elhigheta_sel.add_u( "missing_et_good" , bind( &AnaMET::is_good, _1 ) == true );
  met_baseline_elhigheta_sel.add_uu( "missing_et" , bind( &AnaMET::met , _1 ) > 0. ,
                                     bind(&dg::fillh,_1,100,0,100, bind(&AnaMET::met,_2)) );

  // W/Z Baseline muon MET selection with corrections appropriate for eta<4.5 (also using for low eta)
  DgCutSet<  boost::shared_ptr<const AnaMET>  > met_baseline_muhigheta_sel( "met_baseline_muhigheta" );
  met_baseline_muhigheta_sel.add_u( "met_type" , bind(&AnaMET::met_type,_1) == AnaMET::LOCHADTOPOMUHIGHETA );
  met_baseline_muhigheta_sel.add_u( "missing_et_good" , bind( &AnaMET::is_good, _1 ) == true );
  met_baseline_muhigheta_sel.add_uu( "missing_et" , bind( &AnaMET::met , _1 ) > 0. ,
                                     bind(&dg::fillh,_1,100,0,100, bind(&AnaMET::met,_2)) );
  

  // define W selections
  // ================================================================

  // W events
  EventObjectColl<  boost::shared_ptr<const AnaWCrossSectionCandidate>  > wall;
  EventObjectColl<  boost::shared_ptr<const AnaWCrossSectionCandidate>  > wall_muon;
  EventObjectColl<  boost::shared_ptr<const AnaWCrossSectionCandidate>  > wall_electron;
  EventObjectColl<  boost::shared_ptr<const AnaWCrossSectionCandidate>  > wall_photon;
  EventObjectColl<  boost::shared_ptr<const AnaWCrossSectionCandidate>  > wall_higheta;
  EventObjectColl<  boost::shared_ptr<const AnaWCrossSectionCandidate>  > wall_muon_higheta;
  EventObjectColl<  boost::shared_ptr<const AnaWCrossSectionCandidate>  > wall_electron_higheta;
  EventObjectColl<  boost::shared_ptr<const AnaWCrossSectionCandidate>  > wall_photon_higheta;
  // W event selected candidates
  EventObjectColl<  boost::shared_ptr<const AnaWCrossSectionCandidate>  > w_loose;
  EventObjectColl<  boost::shared_ptr<const AnaWCrossSectionCandidate>  > w_muon_loose;
  EventObjectColl<  boost::shared_ptr<const AnaWCrossSectionCandidate>  > w_electron_loose;
  EventObjectColl<  boost::shared_ptr<const AnaWCrossSectionCandidate>  > w_photon_loose;
  EventObjectColl<  boost::shared_ptr<const AnaWCrossSectionCandidate>  > w_loose_higheta;
  EventObjectColl<  boost::shared_ptr<const AnaWCrossSectionCandidate>  > w_muon_loose_higheta;
  EventObjectColl<  boost::shared_ptr<const AnaWCrossSectionCandidate>  > w_electron_loose_higheta;
  EventObjectColl<  boost::shared_ptr<const AnaWCrossSectionCandidate>  > w_photon_loose_higheta;

  DgCutSet<  boost::shared_ptr<const AnaWCrossSectionCandidate>  > w_baseline_common_sel( "w_baseline_common" );
  w_baseline_common_sel.add_uu( "exactly_one_lepton" ,
                                bind(&AnaWCrossSectionCandidate::exactly_one_lepton,_1) == true ,
                                bind(&dg::fillh,_1,10,0,10,bind(&AnaWCrossSectionCandidate::nleptons,_2)) );
  w_baseline_common_sel.add_u( "exactly_one_met" , bind(&AnaWCrossSectionCandidate::exactly_one_met,_1) );
  //  w_baseline_common_sel.add_uu( "electron_id" , bind(&AnaWCrossSectionCandidate::offline_electron_id,_1) ,
  //                                bind( &dg::filleff , _1 , bind(&AnaWCrossSectionCandidate::offline_electron_id,_2)) );
  //  w_baseline_common_sel.add_uu( "muon_pt" , bind(&AnaWCrossSectionCandidate::pass_muon_pt,_1) ,
  //                                bind(&dg::filleff, _1 , bind(&AnaWCrossSectionCandidate::pass_muon_pt,_2)) );

  DgCutSet<  boost::shared_ptr<const AnaWCrossSectionCandidate>  > w_baseline_sel( "w_baseline" );
  w_baseline_sel.based_on( w_baseline_common_sel );
  w_baseline_sel.add_uu( "met" ,
                         bind(&AnaMET::pt,bind(&AnaWCrossSectionCandidate::met,_1)) > 25. ,
                         bind(dg::fillh,_1,100,0,100,bind(&AnaMET::pt,bind(&AnaWCrossSectionCandidate::met,_2))) );
  w_baseline_sel.add_uu( "transverse_mass" , bind( &AnaWCrossSectionCandidate::transverse_mass , _1 ) > 40.,
                         bind(&dg::fillh,_1,100,0,100, bind(&AnaWCrossSectionCandidate::transverse_mass , _2)) );

  DgCutSet<  boost::shared_ptr<const AnaWCrossSectionCandidate>  > w_constantinos_sel( "w_constantinos" );
  w_constantinos_sel.based_on( w_baseline_common_sel );
  w_constantinos_sel.add_u( "2jets" , bind(&AnaWCrossSectionCandidate::njets,_1) == 1 );
  w_constantinos_sel.add_u( "mid_dijet_eta" , bind(&AnaWCrossSectionCandidate::mid_dijet_eta,_1) );
  w_constantinos_sel.add_u( "lepton_pt" , bind(&AnaParticle::pt,bind(&AnaWCrossSectionCandidate::lepton,_1)) > 40. );
  w_constantinos_sel.add_u( "met" , bind(&AnaMET::pt,bind(&AnaWCrossSectionCandidate::met,_1)) > 30. );
  w_constantinos_sel.add_u( "low_dijet_eta" , bind(&AnaWCrossSectionCandidate::low_dijet_eta,_1) );
  w_constantinos_sel.add_u( "jj_mass_in_window" , bind(&AnaWCrossSectionCandidate::dijet_m,_1)>=71. &&
                            bind(&AnaWCrossSectionCandidate::dijet_m,_1)<=91. );
  w_constantinos_sel.add_u( "no_btag_jets" , bind(&AnaWCrossSectionCandidate::no_btag_event_jet,_1) );
  
  // ================================================================

  // event loop
  unsigned long n_events = 0ul;
  AnaEventMgr* evtmgr = AnaEventMgr::instance(); assert(evtmgr);
  for( AnaEventMgr::iterator i=evtmgr->begin(), f=evtmgr->end(AnaConfiguration::user_max_nevents()); i!=f; ++i, ++n_events ) {

    const boost::shared_ptr<const AnaEvent>& evt( *i );
    if( !evt ) { break; }

    // check trigger
    const boost::shared_ptr<const AnaTrigger>& trigger = evt->trigger();
    dg::filleff( "trigger_present" , trigger );
    if( !trigger ) { cout << "warning: trigger information not present for run " << evt->run_number() << " evt " << evt->event_number() << ". skipping..." << endl; continue; }

    if( false ) { cout << boost::format( "run %|d| event %|d| lumi block %|d|") % evt->run_number() % evt->event_number() % evt->lumi_block() << endl; }

    if( false ) {
      cout << " nele: " << evt->electrons().size() << endl;
      BOOST_FOREACH( const boost::shared_ptr<const AnaParticle>& p , evt->electrons() ) { p->print(cout); }
      cout << " nmu: " << evt->muons().size() << endl;
      BOOST_FOREACH( const boost::shared_ptr<const AnaParticle>& p , evt->muons() ) { p->print(cout); }
      cout << " njet: " << evt->atlas_anti_kt_4_h1_topo_jets().size() << endl;
      BOOST_FOREACH( const boost::shared_ptr<const AnaParticle>& p , evt->atlas_anti_kt_4_h1_topo_jets() ) { p->print(cout); }
      cout << " nmet: " << evt->missing_et().size() << endl;
      BOOST_FOREACH( const boost::shared_ptr<const AnaParticle>& p , evt->missing_et() ) { p->print(cout); }
    }

    // count total events
    dg::filleff( "00_nevts" , true );
    dg::fillh("h00_nevts",10,0,10,1);

    // check sample type and decide whether or not we need to make
    // data-only or mc-only preselection cuts. then make them.
    // ================================================================
    AnaEvent::tribool is_mc = evt->check_flag( "mc" );
    const bool do_not_require_good_run = is_mc;
    // check good run list
    const bool is_good_run_ele = do_not_require_good_run ? true : grl.is_in( GoodRunList::ELECTRON , evt->run_number() , evt->lumi_block() );
    const bool is_good_run_mu = do_not_require_good_run ? true : grl.is_in( GoodRunList::MUON , evt->run_number() , evt->lumi_block() );;
    // keep track of how many events pass GRL
    dg::filleff( "01_grl_el" , is_good_run_ele );
    if( is_good_run_ele ) dg::fillh("h01_grl_el",10,0,10,1);
    dg::filleff( "01_grl_mu" , is_good_run_mu );
    if( is_good_run_mu ) dg::fillh("h01_grl_mu",10,0,10,1);
    dg::filleff( "01_grl_lep" , is_good_run_mu || is_good_run_ele );
    if( is_good_run_mu || is_good_run_ele ) dg::fillh("h01_grl_lep",10,0,10,1);

    // Check the primary vertex
    dgSTL::copy_if( evt->vertices().begin() , evt->vertices().end() , back_inserter(vtx_baseline),
                    CUT_ALL(AnaVertex,vtx_baseline_sel) );
    dg::filleff("02_vtx", vtx_baseline.size()>0 && (is_good_run_ele || is_good_run_mu ) );
    if( vtx_baseline.size()>0 && (is_good_run_ele || is_good_run_mu ) ) dg::fillh("h02_vtx",10,0,10,1);
    

    // study selections.
    if( is_good_run_ele ) { 
      ele_baseline_sel.for_each( evt->electrons().begin() , evt->electrons().end() );
    }

    if( is_good_run_mu ) { 
      mu_baseline_sel.for_each( evt->muons().begin() , evt->muons().end() );
    }

    if( !is_good_run_ele && !is_good_run_mu ) { continue; }
    if( vtx_baseline.size()<=0 ) { continue; }

    met_baseline_el_sel.for_each( evt->missing_et().begin() , evt->missing_et().end() );
    met_baseline_elhigheta_sel.for_each( evt->missing_et().begin() , evt->missing_et().end() );
    met_baseline_muhigheta_sel.for_each( evt->missing_et().begin() , evt->missing_et().end() );

    jet_baseline_common_sel.for_each( evt->atlas_anti_kt_4_h1_topo_jets().begin() , 
                                      evt->atlas_anti_kt_4_h1_topo_jets().end() );

    // select final state candidates
    if( is_good_run_ele ) { 
      dgSTL::copy_if( evt->electrons().begin() , evt->electrons().end() , back_inserter(ele_baseline),
                      CUT_ALL(AnaElectron,ele_baseline_sel) );

      // For the QCD background, if none of the electrons passed, then go to the photon selection:
      if( ele_baseline.empty() ) {
        photon_baseline_sel.for_each( evt->photons().begin() , evt->photons().end() );
        dgSTL::copy_if( evt->photons().begin() , evt->photons().end() , back_inserter(photon_baseline),
                        CUT_ALL(AnaPhoton,photon_baseline_sel) );
      }
    }

    if( is_good_run_mu ) { 
      // First do the d0/z0 cuts. Procedure is to go through muons,
      // find the vertex closest to it in z0, then apply the cuts
      std::vector< boost::shared_ptr<const AnaMuon> > mu_good_d0z0;
      BOOST_FOREACH( boost::shared_ptr<const AnaMuon> theMu, evt->muons() ) {
        float closest_z0_diff(std::numeric_limits<float>::max());
        boost::shared_ptr<const AnaVertex> closeVtx;
        BOOST_FOREACH( boost::shared_ptr<const AnaVertex> theVtx, vtx_baseline ) {
          float z0_diff = std::abs(theVtx->z() - theMu->z0());
          if( z0_diff < closest_z0_diff ) {
            closest_z0_diff = z0_diff;
            closeVtx = theVtx;
          }
        }
        // Apply cuts:
        float d0_diff = detector::corrected_d0( theMu->d0() , theMu->z0() , theMu->phi() , 
                                                theMu->charge() , closeVtx->x() , closeVtx->y() , closeVtx->z() );
        if( closest_z0_diff < 10. && d0_diff < 0.1 ) { 
          mu_good_d0z0.push_back(theMu);
        }
      }
      // Now do the rest of the cuts:
      dgSTL::copy_if( mu_good_d0z0.begin() , mu_good_d0z0.end() , back_inserter(mu_baseline),
                      CUT_ALL(AnaMuon, mu_baseline_sel) );
    }

    // Use only good jets for electron-jet overlap
    dgSTL::copy_if( evt->atlas_anti_kt_4_h1_topo_jets().begin() , evt->atlas_anti_kt_4_h1_topo_jets().end() , 
                    back_inserter(elejet_baseline),CUT_ALL(AnaJet,jet_baseline_common_sel) ); 

    // select the appropriate MET to use based on whether event is electron or muon event
    if( mu_baseline.size() == 0 ) {
      dgSTL::copy_if( evt->missing_et().begin() , evt->missing_et().end() , 
                      back_inserter(met_baseline),CUT_ALL(AnaMET, met_baseline_el_sel) );
      dgSTL::copy_if( evt->missing_et().begin() , evt->missing_et().end() , 
                      back_inserter(met_baseline_higheta),CUT_ALL(AnaMET, met_baseline_elhigheta_sel) );
    } else {
      dgSTL::copy_if( evt->missing_et().begin() , evt->missing_et().end() , 
                      back_inserter(met_baseline),CUT_ALL(AnaMET, met_baseline_muhigheta_sel) );
      dgSTL::copy_if( evt->missing_et().begin() , evt->missing_et().end() ,
                      back_inserter(met_baseline_higheta),CUT_ALL(AnaMET, met_baseline_muhigheta_sel) );
    }

    // remove jet-electron overlap
    if( ele_baseline.size() ) {
      overlap_removal( elejet_baseline.begin() , elejet_baseline.end() ,
                       ele_baseline.begin() , ele_baseline.end() ,
                       0.5 , back_inserter(jet_baseline_afterOv) );
    } else {
      // Photon list only has entries if electron list does not:
      overlap_removal( elejet_baseline.begin() , elejet_baseline.end() ,
                       photon_baseline.begin() , photon_baseline.end() ,
                       0.5 , back_inserter(jet_baseline_afterOv) );
    }
    

    // Cut study
    jet_baseline_sel.for_each( jet_baseline_afterOv.begin() , jet_baseline_afterOv.end() );
    jet_baseline_higheta_sel.for_each( jet_baseline_afterOv.begin() , jet_baseline_afterOv.end() );

    // Final jet selection 
    dgSTL::copy_if( jet_baseline_afterOv.begin() , jet_baseline_afterOv.end() , back_inserter(jet_baseline),
                    CUT_ALL(AnaJet,jet_baseline_sel) );
    dgSTL::copy_if( jet_baseline_afterOv.begin() , jet_baseline_afterOv.end() , back_inserter(jet_baseline_higheta),
                    CUT_ALL(AnaJet,jet_baseline_higheta_sel) );

    // copy electrons and muons to "leptons"
    std::copy( ele_baseline.begin() , ele_baseline.end() , back_inserter( lepton_baseline ) );
    std::copy( mu_baseline.begin() , mu_baseline.end() , back_inserter( lepton_baseline ) );
    std::copy( photon_baseline.begin() , photon_baseline.end() , back_inserter( photon_particle_baseline ) );

    if( true ) {
      // number of final states selected.
      dg::fillh( "n_mu" , 10 , 0 , 10 , mu_baseline.size() );
      dg::fillh( "n_ele" , 10 , 0 , 10 , ele_baseline.size() );
      dg::fillh( "n_photon" , 10 , 0 , 10 , photon_baseline.size() );
      dg::fillh( "n_lepton" , 10 , 0 , 10 , lepton_baseline.size() );
      dg::fillh( "n_met" , 10 , 0 , 10 , met_baseline.size() );
      dg::fillh( "n_elejet" , 10 , 0 , 10 , elejet_baseline.size() );
      dg::fillh( "n_elejet_removed" , 10 , 0 , 10 , elejet_baseline.size() - jet_baseline.size() );
      dg::fillh( "n_jet" , 10 , 0 , 10 , jet_baseline.size() );
      dg::fillh( "n_jet_higheta" , 10 , 0 , 10 , jet_baseline_higheta.size() );
    }

    // build W event candidates
    build_w_candidates( lepton_baseline.begin() , lepton_baseline.end() ,
                        met_baseline.begin() , met_baseline.end() ,
                        jet_baseline.begin() , jet_baseline.end() ,
                        vtx_baseline.begin() , vtx_baseline.end() ,
                        back_inserter(wall) );

    build_w_candidates( ele_baseline.begin() , ele_baseline.end() ,
                        met_baseline.begin() , met_baseline.end() ,
                        jet_baseline.begin() , jet_baseline.end() ,
                        vtx_baseline.begin() , vtx_baseline.end() ,
                        back_inserter(wall_electron) );

    build_w_candidates( mu_baseline.begin()  , mu_baseline.end()  ,
                        met_baseline.begin() , met_baseline.end() ,
                        jet_baseline.begin() , jet_baseline.end() ,
                        vtx_baseline.begin() , vtx_baseline.end() ,
                        back_inserter(wall_muon) );

    build_w_candidates( photon_particle_baseline.begin() , photon_particle_baseline.end() ,
                        met_baseline.begin() , met_baseline.end() ,
                        jet_baseline.begin() , jet_baseline.end() ,
                        vtx_baseline.begin() , vtx_baseline.end() ,
                        back_inserter(wall_photon) );

    build_w_candidates( lepton_baseline.begin() , lepton_baseline.end() ,
                        met_baseline_higheta.begin() , met_baseline_higheta.end() ,
                        jet_baseline_higheta.begin() , jet_baseline_higheta.end() ,
                        vtx_baseline.begin() , vtx_baseline.end() ,
                        back_inserter(wall_higheta) );

    build_w_candidates( ele_baseline.begin() , ele_baseline.end() ,
                        met_baseline_higheta.begin() , met_baseline_higheta.end() ,
                        jet_baseline_higheta.begin() , jet_baseline_higheta.end() ,
                        vtx_baseline.begin() , vtx_baseline.end() ,
                        back_inserter(wall_electron_higheta) );

    build_w_candidates( mu_baseline.begin()  , mu_baseline.end()  ,
                        met_baseline_higheta.begin() , met_baseline_higheta.end() ,
                        jet_baseline_higheta.begin() , jet_baseline_higheta.end() ,
                        vtx_baseline.begin() , vtx_baseline.end() ,
                        back_inserter(wall_muon_higheta) );

    build_w_candidates( photon_particle_baseline.begin() , photon_particle_baseline.end() ,
                        met_baseline_higheta.begin() , met_baseline_higheta.end() ,
                        jet_baseline_higheta.begin() , jet_baseline_higheta.end() ,
                        vtx_baseline.begin() , vtx_baseline.end() ,
                        back_inserter(wall_photon_higheta) );

    // study W selection
    w_baseline_common_sel.for_each( wall.begin() , wall.end() );
    w_baseline_common_sel.for_each( wall_photon.begin() , wall_photon.end() );
    w_baseline_common_sel.for_each( wall_higheta.begin() , wall_higheta.end() );

    // create some vertex distributions for reweighting pileup
    if( AnaConfiguration::vtx_plots() ) {
      if( evt->trigger()->l1_em14() ) {
        dg::filleff( "passed_l1_em14" , true );
        dg::fillh("nvtxs_l1_em14",20,0,20,vtx_baseline.size() );
      }
      if( evt->trigger()->ef_e15_medium() ) {
        dg::filleff( "passed_e15_medium" , true );
        dg::fillh("nvtxs_e15_medium",20,0,20,vtx_baseline.size() );
      }
      if( evt->trigger()->ef_g20_loose() ) {
        dg::filleff( "passed_g20_loose" , true );
        dg::fillh("nvtxs_g20_loose",20,0,20,vtx_baseline.size() );
      }
      if( evt->trigger()->l1_mu10() ) {
        dg::filleff( "passed_l1_mu10" , true );
        dg::fillh("nvtxs_l1_mu10",20,0,20,vtx_baseline.size() );
      }
      if( evt->trigger()->ef_mu10_mg() ) {
        dg::filleff( "passed_mu10_mg" , true );
        dg::fillh("nvtxs_mu10_mg",20,0,20,vtx_baseline.size() );
      }
      if( evt->trigger()->ef_mu13_mg() ) {
        dg::filleff( "passed_mu13_mg" , true );
        dg::fillh("nvtxs_mu13_mg",20,0,20,vtx_baseline.size() );
      }
      if( evt->trigger()->ef_mu13_mg_tight() ) {
        dg::filleff( "passed_mu13_mg_tight" , true );
        dg::fillh("nvtxs_mu13_mg_tight",20,0,20,vtx_baseline.size() );
      }
      if( wall_electron.size()>0 ) dg::fillh("nvtxs_wall_electron",20,0,20,vtx_baseline.size() );
      if( wall_electron_higheta.size()>0 ) dg::fillh("nvtxs_wall_electron_higheta",20,0,20,vtx_baseline.size() );
      if( wall_muon.size()>0 ) dg::fillh("nvtxs_wall_muon",20,0,20,vtx_baseline.size() );
      if( wall_muon_higheta.size()>0 ) dg::fillh("nvtxs_wall_muon_higheta",20,0,20,vtx_baseline.size() );
      if( wall_photon.size()>0 ) dg::fillh("nvtxs_wall_photon",20,0,20,vtx_baseline.size() );
      if( wall_photon_higheta.size()>0 ) dg::fillh("nvtxs_wall_photon_higheta",20,0,20,vtx_baseline.size() );
    }

    // save event if it has a W candidate
    if( AnaEventMgr::instance()->ana_event_ntuple_mode() == AnaEventNtuple::WRITE ) {
      AnaEventMgr::instance()->add_to_ana_event_ntuple( wall_higheta.size()>0 || wall_photon_higheta.size()>0 );
      continue;
    }

    // apply event selection
    // loose w candidates
    dgSTL::copy_if( wall.begin() , wall.end() , back_inserter(w_loose) ,
                    CUT_ALL(AnaWCrossSectionCandidate, w_baseline_common_sel));
    dgSTL::copy_if( wall_muon.begin() , wall_muon.end() , back_inserter(w_muon_loose) ,
                    CUT_ALL(AnaWCrossSectionCandidate, w_baseline_common_sel) );
    dgSTL::copy_if( wall_electron.begin() , wall_electron.end() , back_inserter(w_electron_loose) ,
                    CUT_ALL(AnaWCrossSectionCandidate, w_baseline_common_sel) );
    dgSTL::copy_if( wall_photon.begin() , wall_photon.end() , back_inserter(w_photon_loose) ,
                    CUT_ALL(AnaWCrossSectionCandidate, w_baseline_common_sel));
    // loose W candidates with jet eta < 4.5
    dgSTL::copy_if( wall_higheta.begin() , wall_higheta.end() , back_inserter(w_loose_higheta) ,
                    CUT_ALL(AnaWCrossSectionCandidate, w_baseline_common_sel));
    dgSTL::copy_if( wall_muon_higheta.begin() , wall_muon_higheta.end() , back_inserter(w_muon_loose_higheta) ,
                    CUT_ALL(AnaWCrossSectionCandidate, w_baseline_common_sel) );
    dgSTL::copy_if( wall_electron_higheta.begin() , wall_electron_higheta.end() , 
                    back_inserter(w_electron_loose_higheta) , CUT_ALL(AnaWCrossSectionCandidate, w_baseline_common_sel) );
    dgSTL::copy_if( wall_photon_higheta.begin() , wall_photon_higheta.end() , back_inserter(w_photon_loose_higheta) ,
                    CUT_ALL(AnaWCrossSectionCandidate, w_baseline_common_sel));

    // Veto events in which there's a second medium electron (Zee veto)
    if( w_electron_loose.size()>0 || w_photon_loose.size()>0 ) {
      assert( w_electron_loose.size() == 1 || w_photon_loose.size() == 1 );
      set< boost::shared_ptr<const AnaElectron> > el_medium_container;
      dgSTL::copy_if( evt->electrons().begin() , evt->electrons().end() , 
                      inserter(el_medium_container,el_medium_container.begin()),
                      bind(&DgCutResult::all_pass,
                           bind(&DgCutSet< boost::shared_ptr<const AnaElectron> >::cut,
                                ele_baseline_sel_other,_1))==true );
      boost::shared_ptr<const AnaElectron> main_elec;
      if( w_electron_loose.size() ) {
        main_elec = boost::static_pointer_cast<const AnaElectron>((*w_electron_loose.begin())->lepton());
      }
      else {
        main_elec = boost::static_pointer_cast<const AnaElectron>((*w_photon_loose.begin())->lepton());
      }
      assert( main_elec );
      el_medium_container.erase(main_elec);
      if( w_electron_loose.size() ) {
        if( el_medium_container.size() ) {
          w_loose.clear();
          w_electron_loose.clear();
          w_loose_higheta.clear();
          w_electron_loose_higheta.clear();
        }
        dg::filleff("w_loose_Zee_veto_eff", w_loose.size()>0);
      }
      else if( w_photon_loose.size() ) {
        if( el_medium_container.size() ) {
          w_photon_loose.clear();
          w_photon_loose_higheta.clear();
        }
        dg::filleff("wphoton_loose_Zee_veto_eff", w_photon_loose.size()>0);
      }
    }

    // Veto events in which there's a second good muon (Zmumu veto)
    if( w_muon_loose.size()>0 ) {
      assert( w_muon_loose.size() == 1 );
      set< boost::shared_ptr<const AnaMuon> > mu_container;
      dgSTL::copy_if( evt->muons().begin() , evt->muons().end() ,
                      inserter(mu_container,mu_container.begin()),
                      bind(&DgCutResult::all_pass,
                           bind(&DgCutSet< boost::shared_ptr<const AnaMuon> >::cut,
                                mu_baseline_sel,_1))==true );
      boost::shared_ptr<const AnaMuon> main_muon( boost::static_pointer_cast<const AnaMuon>((*w_muon_loose.begin())->lepton()) );
      assert( main_muon );
      mu_container.erase(main_muon);
      if( mu_container.size() ) {
        w_loose.clear();
        w_muon_loose.clear();
        w_loose_higheta.clear();
        w_muon_loose_higheta.clear();
      }
      // Get the Zee veto efficiency relative to the W selection
      dg::filleff("w_loose_Zmumu_veto_eff", w_loose.size()>0);
    }


    if( true ) {
      // number of W events selected
      dg::fillh( "n_W_loose" , 10 , 0 , 10 , w_loose.size() );
      dg::fillh( "n_Wphoton_loose" , 10 , 0 , 10 , w_photon_loose.size() );
    }

    // study selected W events
    StudyWCrossSection stw( "w_loose_candidates" , "W event candidates Baseline 2010 before MET and mT" );
    stw.do_heavyQ_studies();
    //    stw.do_xcheck_studies();
    // define the signal and control regions to study
    // typedef boost::function<bool (shared_ptr<const AnaWCrossSectionCandidate> const& )> WxsecBind;
    stw.add_category( "baseline" , bind(&AnaWCrossSectionCandidate::baseline,_1) );
    stw.add_category( "my_SR" , bind(&AnaWCrossSectionCandidate::my_SR,_1) );
    stw.add_category( "martina_SR" , bind(&AnaWCrossSectionCandidate::martina_SR,_1) );
    stw.add_category( "metfit_CR" , bind(&AnaWCrossSectionCandidate::metfit_CR,_1) );
    stw.add_category( "lowmet_CR" , bind(&AnaWCrossSectionCandidate::lowmet_CR,_1) );
    stw.add_category( "lowmet_wmt_CR" , bind(&AnaWCrossSectionCandidate::lowmet_wmt_CR,_1) );
    stw.add_category( "midmet_CR" , bind(&AnaWCrossSectionCandidate::midmet_CR,_1) );
    stw.add_category( "midmet_wmt_CR" , bind(&AnaWCrossSectionCandidate::midmet_wmt_CR,_1) );
    stw.add_category( "highmet_highdphi_CR" , bind(&AnaWCrossSectionCandidate::highmet_highdphi_CR,_1) );
    stw.add_category( "highmet_highdphi_wmt_CR" , bind(&AnaWCrossSectionCandidate::highmet_highdphi_wmt_CR,_1) );
    for( unsigned int ithjet=0; ithjet<=3; ithjet++ ) {
      stw.add_category( (format("%|d|jets") % ithjet ).str() , bind(&AnaWCrossSectionCandidate::jetbin,_1)==ithjet );
      stw.add_category( (format("%|d|jets_baseline") % ithjet ).str() , bind(&AnaWCrossSectionCandidate::baseline,_1) &&
                        bind(&AnaWCrossSectionCandidate::jetbin,_1)==ithjet );
      stw.add_category( (format("%|d|jets_my_SR") % ithjet ).str() , bind(&AnaWCrossSectionCandidate::my_SR,_1) &&
                        bind(&AnaWCrossSectionCandidate::jetbin,_1)==ithjet );
      stw.add_category( (format("%|d|jets_martina_SR") % ithjet ).str() , bind(&AnaWCrossSectionCandidate::martina_SR,_1) &&
                        bind(&AnaWCrossSectionCandidate::jetbin,_1)==ithjet );
      stw.add_category( (format("%|d|jets_metfit_CR") % ithjet ).str() , bind(&AnaWCrossSectionCandidate::metfit_CR,_1) &&
                        bind(&AnaWCrossSectionCandidate::jetbin,_1)==ithjet );
      stw.add_category( (format("%|d|jets_lowmet_CR") % ithjet ).str() , bind(&AnaWCrossSectionCandidate::lowmet_CR,_1) &&
                        bind(&AnaWCrossSectionCandidate::jetbin,_1)==ithjet );
      stw.add_category( (format("%|d|jets_lowmet_wmt_CR") % ithjet ).str() , bind(&AnaWCrossSectionCandidate::lowmet_wmt_CR,_1) &&
                        bind(&AnaWCrossSectionCandidate::jetbin,_1)==ithjet );
      stw.add_category( (format("%|d|jets_midmet_CR") % ithjet ).str() , bind(&AnaWCrossSectionCandidate::midmet_CR,_1) &&
                        bind(&AnaWCrossSectionCandidate::jetbin,_1)==ithjet );
      stw.add_category( (format("%|d|jets_midmet_wmt_CR") % ithjet ).str() , bind(&AnaWCrossSectionCandidate::midmet_wmt_CR,_1) &&
                        bind(&AnaWCrossSectionCandidate::jetbin,_1)==ithjet );
      stw.add_category( (format("%|d|jets_highmet_highdphi_CR") % ithjet ).str() , bind(&AnaWCrossSectionCandidate::highmet_highdphi_CR,_1) &&
                        bind(&AnaWCrossSectionCandidate::jetbin,_1)==ithjet );
      stw.add_category( (format("%|d|jets_highmet_highdphi_wmt_CR") % ithjet ).str() , bind(&AnaWCrossSectionCandidate::highmet_highdphi_wmt_CR,_1) &&
                        bind(&AnaWCrossSectionCandidate::jetbin,_1)==ithjet );
    }
    if( AnaConfiguration::vtx_plots() ) {
      for( unsigned int ithvtx=0; ithvtx<=10; ithvtx++ ) {
        stw.add_category( (format("%|d|vtxs") % ithvtx ).str() , bind(&AnaWCrossSectionCandidate::vtxbin,_1) == ithvtx );
        stw.add_category( (format("%|d|vtxs_baseline") % ithvtx ).str() , bind(&AnaWCrossSectionCandidate::baseline,_1) &&
                          bind(&AnaWCrossSectionCandidate::vtxbin,_1)==ithvtx );
        stw.add_category( (format("%|d|vtxs_my_SR") % ithvtx ).str() , bind(&AnaWCrossSectionCandidate::my_SR,_1) &&
                          bind(&AnaWCrossSectionCandidate::vtxbin,_1)==ithvtx );
        stw.add_category( (format("%|d|vtxs_martina_SR") % ithvtx ).str() , bind(&AnaWCrossSectionCandidate::martina_SR,_1) &&
                          bind(&AnaWCrossSectionCandidate::vtxbin,_1)==ithvtx );
        stw.add_category( (format("%|d|vtxs_metfit_CR") % ithvtx ).str() , bind(&AnaWCrossSectionCandidate::metfit_CR,_1) &&
                          bind(&AnaWCrossSectionCandidate::vtxbin,_1)==ithvtx );
        stw.add_category( (format("%|d|vtxs_lowmet_CR") % ithvtx ).str() , bind(&AnaWCrossSectionCandidate::lowmet_CR,_1) &&
                          bind(&AnaWCrossSectionCandidate::vtxbin,_1)==ithvtx );
        stw.add_category( (format("%|d|vtxs_lowmet_wmt_CR") % ithvtx ).str() , bind(&AnaWCrossSectionCandidate::lowmet_wmt_CR,_1) &&
                          bind(&AnaWCrossSectionCandidate::vtxbin,_1)==ithvtx );
        stw.add_category( (format("%|d|vtxs_midmet_CR") % ithvtx ).str() , bind(&AnaWCrossSectionCandidate::midmet_CR,_1) &&
                          bind(&AnaWCrossSectionCandidate::vtxbin,_1)==ithvtx );
        stw.add_category( (format("%|d|vtxs_midmet_wmt_CR") % ithvtx ).str() , bind(&AnaWCrossSectionCandidate::midmet_wmt_CR,_1) &&
                          bind(&AnaWCrossSectionCandidate::vtxbin,_1)==ithvtx );
        stw.add_category( (format("%|d|vtxs_highmet_highdphi_CR") % ithvtx ).str() , bind(&AnaWCrossSectionCandidate::highmet_highdphi_CR,_1) &&
                          bind(&AnaWCrossSectionCandidate::vtxbin,_1)==ithvtx );
        stw.add_category( (format("%|d|vtxs_highmet_highdphi_wmt_CR") % ithvtx ).str() , bind(&AnaWCrossSectionCandidate::highmet_highdphi_wmt_CR,_1) &&
                          bind(&AnaWCrossSectionCandidate::vtxbin,_1)==ithvtx );
        for( unsigned int ithjet=0; ithjet<=3; ithjet++ ) {
          stw.add_category( (format("%|d|vtxs_%|d|jets") % ithvtx % ithjet ).str() , bind(&AnaWCrossSectionCandidate::vtxbin,_1) == ithvtx && 
                            bind(&AnaWCrossSectionCandidate::jetbin,_1)==ithjet );
          stw.add_category( (format("%|d|vtxs_%|d|jets_baseline") % ithvtx % ithjet ).str() , bind(&AnaWCrossSectionCandidate::baseline,_1) &&
                            bind(&AnaWCrossSectionCandidate::jetbin,_1)==ithjet &&  
                            bind(&AnaWCrossSectionCandidate::vtxbin,_1)==ithvtx );
          stw.add_category( (format("%|d|vtxs_%|d|jets_my_SR") % ithvtx % ithjet ).str() , bind(&AnaWCrossSectionCandidate::my_SR,_1) &&
                            bind(&AnaWCrossSectionCandidate::jetbin,_1)==ithjet &&
                            bind(&AnaWCrossSectionCandidate::vtxbin,_1)==ithvtx );
          stw.add_category( (format("%|d|vtxs_%|d|jets_martina_SR") % ithvtx % ithjet ).str() , bind(&AnaWCrossSectionCandidate::martina_SR,_1) &&
                            bind(&AnaWCrossSectionCandidate::jetbin,_1)==ithjet &&
                            bind(&AnaWCrossSectionCandidate::vtxbin,_1)==ithvtx );
          stw.add_category( (format("%|d|vtxs_%|d|jets_metfit_CR") % ithvtx % ithjet ).str() , bind(&AnaWCrossSectionCandidate::metfit_CR,_1) &&
                            bind(&AnaWCrossSectionCandidate::jetbin,_1)==ithjet &&
                            bind(&AnaWCrossSectionCandidate::vtxbin,_1)==ithvtx );
          stw.add_category( (format("%|d|vtxs_%|d|jets_lowmet_CR") % ithvtx % ithjet ).str() , bind(&AnaWCrossSectionCandidate::lowmet_CR,_1) &&
                            bind(&AnaWCrossSectionCandidate::jetbin,_1)==ithjet &&
                            bind(&AnaWCrossSectionCandidate::vtxbin,_1)==ithvtx );
          stw.add_category( (format("%|d|vtxs_%|d|jets_lowmet_wmt_CR") % ithvtx % ithjet ).str() , bind(&AnaWCrossSectionCandidate::lowmet_wmt_CR,_1) &&
                            bind(&AnaWCrossSectionCandidate::jetbin,_1)==ithjet &&
                            bind(&AnaWCrossSectionCandidate::vtxbin,_1)==ithvtx );
          stw.add_category( (format("%|d|vtxs_%|d|jets_midmet_CR") % ithvtx % ithjet ).str() , bind(&AnaWCrossSectionCandidate::midmet_CR,_1) &&
                            bind(&AnaWCrossSectionCandidate::jetbin,_1)==ithjet &&
                            bind(&AnaWCrossSectionCandidate::vtxbin,_1)==ithvtx );
          stw.add_category( (format("%|d|vtxs_%|d|jets_midmet_wmt_CR") % ithvtx % ithjet ).str() , bind(&AnaWCrossSectionCandidate::midmet_wmt_CR,_1) &&
                            bind(&AnaWCrossSectionCandidate::jetbin,_1)==ithjet &&
                            bind(&AnaWCrossSectionCandidate::vtxbin,_1)==ithvtx );
          stw.add_category( (format("%|d|vtxs_%|d|jets_highmet_highdphi_CR") % ithvtx % ithjet ).str() , 
                            bind(&AnaWCrossSectionCandidate::highmet_highdphi_CR,_1) &&
                            bind(&AnaWCrossSectionCandidate::jetbin,_1)==ithjet &&
                            bind(&AnaWCrossSectionCandidate::vtxbin,_1)==ithvtx );
          stw.add_category( (format("%|d|vtxs_%|d|jets_highmet_highdphi_wmt_CR") % ithvtx % ithjet ).str() , 
                            bind(&AnaWCrossSectionCandidate::highmet_highdphi_wmt_CR,_1) &&
                            bind(&AnaWCrossSectionCandidate::jetbin,_1)==ithjet &&
                            bind(&AnaWCrossSectionCandidate::vtxbin,_1)==ithvtx );
        } // end njets loop
      } // end vtx plots loop
    } // end vtx plots if statement
    //make plots for loose selections
    stw.for_each( w_loose.begin() , w_loose.end() );
    stw.repurpose( "w_loose_candidates_higheta" , "W event candidates Baseline 2010 before MET and mT" );
    stw.for_each( w_loose_higheta.begin() , w_loose_higheta.end() );
    if( AnaConfiguration::muon_trig()!=0 ) {
      stw.repurpose( "w_muon_loose_candidates" , "W event candidates in muon channel before MET and mT" );
      stw.for_each( w_muon_loose.begin() , w_muon_loose.end() );
      stw.repurpose( "w_muon_loose_candidates_higheta" , "W event candidates in muon channel before MET and mT" );
      stw.for_each( w_muon_loose_higheta.begin() , w_muon_loose_higheta.end() );
    }
    if( AnaConfiguration::elec_trig()!=0 ) {
      stw.repurpose( "w_electron_loose_candidates" , "W event candidates in elec channel before MET and mT" );
      stw.for_each( w_electron_loose.begin() , w_electron_loose.end() );
      stw.repurpose( "w_electron_loose_candidates_higheta" , "W event candidates in elec channel before MET and mT" );
      stw.for_each( w_electron_loose_higheta.begin() , w_electron_loose_higheta.end() );
    }
    if( AnaConfiguration::phot_trig()!=0 ) {
      stw.repurpose( "w_photon_loose_candidates" , "W event candidates built with photons before MET and mT" );
      stw.for_each( w_photon_loose.begin() , w_photon_loose.end() );
      stw.repurpose( "w_photon_loose_candidates_higheta" , "W event candidates built with photons before MET and mT" );
      stw.for_each( w_photon_loose_higheta.begin() , w_photon_loose_higheta.end() );
    }

    // W+jets electron CUT FLOW EXERCISE. Joe Tuggle's implementation.
    if( AnaConfiguration::elec_trig()!=0 ) {
      dg::down( "cutflow_el_baseline", "Electron cut flow for Baseline 2010" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      // electron mode:
      // Primary vertex is already checked above
      dg::filleff("00_vtx_grl",true);
      dg::fillh("h00_vtx_grl",10,0,10,1);
      // Electron trigger:
      vector<  boost::shared_ptr<const AnaElectron>  > ele_trig;
      switch ( AnaConfiguration::elec_trig() ) {
      case 1 :
      case 0 :
        dgSTL::copy_if( evt->electrons().begin() , evt->electrons().end() , back_inserter(ele_trig),
                        CUT_ONE(AnaElectron,ele_baseline_sel,"trigger_L1_EM14") );
        break;
      case 2 :
        dgSTL::copy_if( evt->electrons().begin() , evt->electrons().end() , back_inserter(ele_trig),
                        CUT_ONE(AnaElectron,ele_baseline_sel,"trigger_EF_e15_medium") ); // for alternate E4 and on
        break;
      case 3 :
        dgSTL::copy_if( evt->electrons().begin() , evt->electrons().end() , back_inserter(ele_trig),
                        CUT_ONE(AnaElectron,ele_baseline_sel,"trigger_EF_g20_loose") ); // for alternate E4-F photon MET selection
        break;
      default :
        cout << " invalid electron trigger " << AnaConfiguration::elec_trig() << endl;
        exit(-1);
      }
      dg::filleff("01_ele_trig", ele_trig.size()>0);
      if( ele_trig.size()>0 ) dg::fillh("h01_ele_trig",10,0,10,1);
      // Electron author
      vector<  boost::shared_ptr<const AnaElectron>  > ele_auth;
      dgSTL::copy_if( ele_trig.begin() , ele_trig.end() , back_inserter(ele_auth),
                      CUT_ONE(AnaElectron,ele_baseline_sel,"author") );
      dg::filleff("02_ele_auth", ele_auth.size()>0);
      if( ele_auth.size()>0 ) dg::fillh("h02_ele_auth",10,0,10,1);
      // Electron Eta
      vector<  boost::shared_ptr<const AnaElectron>  > ele_eta;
      dgSTL::copy_if( ele_auth.begin() , ele_auth.end() , back_inserter(ele_eta),
                      CUT_ONE(AnaElectron,ele_baseline_sel,"cluster_eta") );
      dg::filleff("03_ele_eta", ele_eta.size()>0);
      if( ele_eta.size()>0 ) dg::fillh("h03_ele_eta",10,0,10,1);
      // Electron pT      
      vector<  boost::shared_ptr<const AnaElectron>  > ele_pt;
      dgSTL::copy_if( ele_eta.begin() , ele_eta.end() , back_inserter(ele_pt),
                      CUT_ONE(AnaElectron,ele_baseline_sel,"cluster_ET") );
      dg::filleff("04_ele_pt", ele_pt.size()>0);
      if( ele_pt.size()>0 ) dg::fillh("h04_ele_pt",10,0,10,1);
      // MET Cleaning
      vector<  boost::shared_ptr<const AnaMET>  > met_good;
      dgSTL::copy_if( evt->missing_et().begin() , evt->missing_et().end() , back_inserter(met_good),
                      CUT_ALL(AnaMET,met_baseline_el_sel) );
      dg::filleff("05_met_good", met_good.size()==1 && ele_pt.size()>0);
      if( met_good.size()==1 && ele_pt.size()>0 ) dg::fillh("h05_met_good",10,0,10,1);
      // Electron OTX cleaning
      vector<  boost::shared_ptr<const AnaElectron>  > ele_otx;
      dgSTL::copy_if( ele_pt.begin() , ele_pt.end() , back_inserter(ele_otx),
                      CUT_ONE(AnaElectron,ele_baseline_sel,"passOTX") );
      dg::filleff("06_ele_otx", ele_otx.size()>0 && met_good.size()==1);
      if( ele_otx.size()>0 && met_good.size()==1 ) dg::fillh("h06_ele_otx",10,0,10,1);
      // Ensure exactly one medium electron, which is also a tight electron:
      vector<  boost::shared_ptr<const AnaElectron>  > ele_isem;
      dgSTL::copy_if( ele_otx.begin() , ele_otx.end() , back_inserter(ele_isem),
                      CUT_ONE(AnaElectron,ele_baseline_sel_other,"isEM_robustMedium") );
      vector<  boost::shared_ptr<const AnaElectron>  > ele_isem_tight;
      dgSTL::copy_if( ele_isem.begin() , ele_isem.end() , back_inserter(ele_isem_tight),
                      CUT_ONE(AnaElectron,ele_baseline_sel,"isEM_robusterTight") );
      dg::filleff("07_ele_one_tight", ele_isem_tight.size()==1 && met_good.size()==1);
      if( ele_isem_tight.size()==1 && met_good.size()==1 ) dg::fillh("h07_ele_one_tight",10,0,10,1);
      // And make sure the same is true for medium
      dg::filleff("08_ele_one_med", ele_isem.size()==1 && ele_isem_tight.size()==1 && met_good.size()==1);
      if( ele_isem.size()==1 && ele_isem_tight.size()==1 && met_good.size()==1 ) dg::fillh("h08_ele_one_med",10,0,10,1);
      // Number of electrons
      vector<  boost::shared_ptr<const AnaWCrossSectionCandidate>  > w_elec;
      dgSTL::copy_if( wall_electron.begin() , wall_electron.end() , back_inserter(w_elec),
                      CUT_ONE(AnaWCrossSectionCandidate,w_baseline_sel,"exactly_one_lepton") &&
                      CUT_ONE(AnaWCrossSectionCandidate,w_baseline_sel,"exactly_one_met") );
      assert( w_elec.size()<=1 );
      // MET Cut
      vector<  boost::shared_ptr<const AnaWCrossSectionCandidate>  > w_met;
      dgSTL::copy_if( w_elec.begin() , w_elec.end() , back_inserter(w_met),
                      CUT_ONE(AnaWCrossSectionCandidate,w_baseline_sel,"met") );
      dg::filleff("09_w_met", w_met.size()>0 );
      if( w_met.size()>0 ) dg::fillh("h09_w_met",10,0,10,1);
      // W Transverse Mass
      vector<  boost::shared_ptr<const AnaWCrossSectionCandidate>  > w_tmass;
      dgSTL::copy_if( w_met.begin() , w_met.end() , back_inserter(w_tmass),
                      CUT_ONE(AnaWCrossSectionCandidate,w_baseline_sel,"transverse_mass") );
      dg::filleff("10_w_transverse_mass", w_tmass.size()>0);
      if( w_tmass.size()>0 ) dg::fillh("h10_w_transverse_mass",10,0,10,1);
      // NJet distribution
      if( w_tmass.size() > 0 ) {
        int njets = (*w_tmass.begin())->njets();
        if( njets > 3 ) njets = 3;
        dg::fillh( "n_jets" , 4 , 0 , 4 , njets );
      }
    } // end W+jets electron cut flow exercise

    // W+jets photon CUT FLOW EXERCISE
    if( AnaConfiguration::phot_trig()!=0 ) {
      dg::down( "cutflow_ph_baseline", "Photon cut flow for Baseline 2010" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      // photon mode:
      // Primary vertex is already checked above
      dg::filleff("00_vtx_grl",true);
      dg::fillh("h00_vtx_grl",10,0,10,1);
      // Photon trigger:
      vector<  boost::shared_ptr<const AnaPhoton>  > ph_trig;
      switch ( AnaConfiguration::phot_trig() ) {
      case 1 :
      case 0 :
        dgSTL::copy_if( evt->photons().begin() , evt->photons().end() , back_inserter(ph_trig),
                        CUT_ONE(AnaPhoton,photon_baseline_sel,"trigger_L1_EM14") );
        break;
      case 2 :
        dgSTL::copy_if( evt->photons().begin() , evt->photons().end() , back_inserter(ph_trig),
                        CUT_ONE(AnaPhoton,photon_baseline_sel,"trigger_EF_e15_medium") ); // for alternate E4 and on
        break;
      case 3 :
        dgSTL::copy_if( evt->photons().begin() , evt->photons().end() , back_inserter(ph_trig),
                        CUT_ONE(AnaPhoton,photon_baseline_sel,"trigger_EF_g20_loose") ); // for alternate E4-F photon MET selection
        break;
      default :
        cout << " invalid photon trigger " << AnaConfiguration::phot_trig() << endl;
        exit(-1);
      }
      dg::filleff("01_ph_trig", ph_trig.size()>0);
      if( ph_trig.size()>0 ) dg::fillh("h01_ph_trig",10,0,10,1);
      // Photon author
      vector<  boost::shared_ptr<const AnaPhoton>  > ph_auth;
      dgSTL::copy_if( ph_trig.begin() , ph_trig.end() , back_inserter(ph_auth),
                      CUT_ONE(AnaPhoton,photon_baseline_sel,"author") );
      dg::filleff("02_ph_auth", ph_auth.size()>0);
      if( ph_auth.size()>0 ) dg::fillh("h02_ph_auth",10,0,10,1);
      // Photon Eta
      vector<  boost::shared_ptr<const AnaPhoton>  > ph_eta;
      dgSTL::copy_if( ph_auth.begin() , ph_auth.end() , back_inserter(ph_eta),
                      CUT_ONE(AnaPhoton,photon_baseline_sel,"cluster_eta") );
      dg::filleff("03_ph_eta", ph_eta.size()>0);
      if( ph_eta.size()>0 ) dg::fillh("h03_ph_eta",10,0,10,1);
      // Photon pT      
      vector<  boost::shared_ptr<const AnaPhoton>  > ph_pt;
      dgSTL::copy_if( ph_eta.begin() , ph_eta.end() , back_inserter(ph_pt),
                      CUT_ONE(AnaPhoton,photon_baseline_sel,"cluster_ET") );
      dg::filleff("04_ph_pt", ph_pt.size()>0);
      if( ph_pt.size()>0 ) dg::fillh("h04_ph_pt",10,0,10,1);
      // MET Cleaning
      vector<  boost::shared_ptr<const AnaMET>  > met_good;
      dgSTL::copy_if( evt->missing_et().begin() , evt->missing_et().end() , back_inserter(met_good),
                      CUT_ALL(AnaMET,met_baseline_el_sel) );
      dg::filleff("05_met_good", met_good.size()==1 && ph_pt.size()>0);
      if( met_good.size()==1 && ph_pt.size()>0 ) dg::fillh("h05_met_good",10,0,10,1);
      // Photon OTX cleaning
      vector<  boost::shared_ptr<const AnaPhoton>  > ph_otx;
      dgSTL::copy_if( ph_pt.begin() , ph_pt.end() , back_inserter(ph_otx),
                      CUT_ONE(AnaPhoton,photon_baseline_sel,"passOTX") );
      dg::filleff("06_ph_otx", ph_otx.size()>0 && met_good.size()==1);
      if( ph_otx.size()>0 && met_good.size()==1 ) dg::fillh("h06_ph_otx",10,0,10,1);
      // Ensure exactly one medium photon, which is also a tight photon:
      vector<  boost::shared_ptr<const AnaPhoton>  > ph_isem_loose;
      dgSTL::copy_if( ph_otx.begin() , ph_otx.end() , back_inserter(ph_isem_loose),
                      CUT_ONE(AnaPhoton,photon_baseline_sel,"isEM_PhotonLoose") );
      dg::filleff("07_ph_one_loose", ph_isem_loose.size() == 1 && met_good.size()==1);
      // And make sure there is not an extra medium
      vector<  boost::shared_ptr<const AnaWCrossSectionCandidate>  > w_isem;
      std::copy( wall_photon.begin() , wall_photon.end() , back_inserter( w_isem ) );
      if( w_isem.size()>0 ) {
        assert( w_isem.size() == 1 );
        set< boost::shared_ptr<const AnaElectron> > el_medium_container;
        dgSTL::copy_if( evt->electrons().begin() , evt->electrons().end() ,
                        inserter(el_medium_container,el_medium_container.begin()),
                        bind(&DgCutResult::all_pass,
                             bind(&DgCutSet< boost::shared_ptr<const AnaElectron> >::cut,
                                  ele_baseline_sel_other,_1))==true );
        boost::shared_ptr<const AnaElectron> main_elec;
        main_elec = boost::static_pointer_cast<const AnaElectron>((*w_isem.begin())->lepton());
        assert( main_elec );
        el_medium_container.erase(main_elec);
        if( el_medium_container.size() ) {
          w_isem.clear();
        }
      }
      dg::filleff("08_ph_no_med", w_isem.size()==1 && met_good.size()==1);
      if( w_isem.size()==1 && met_good.size()==1 ) dg::fillh("h08_ph_no_med",10,0,10,1);
      // Number of photons
      vector<  boost::shared_ptr<const AnaWCrossSectionCandidate>  > w_phot;
      dgSTL::copy_if( w_isem.begin() , w_isem.end() , back_inserter(w_phot),
                      CUT_ONE(AnaWCrossSectionCandidate,w_baseline_sel,"exactly_one_lepton") &&
                      CUT_ONE(AnaWCrossSectionCandidate,w_baseline_sel,"exactly_one_met") );
      assert( w_phot.size() <= 1 );
      // MET Cut
      vector<  boost::shared_ptr<const AnaWCrossSectionCandidate>  > w_met;
      dgSTL::copy_if( w_phot.begin() , w_phot.end() , back_inserter(w_met),
                      CUT_ONE(AnaWCrossSectionCandidate,w_baseline_sel,"met") );
      dg::filleff("09_w_met", w_met.size()>0 );
      if( w_met.size()>0 ) dg::fillh("h09_w_met",10,0,10,1);
      // W Transverse Mass
      vector<  boost::shared_ptr<const AnaWCrossSectionCandidate>  > w_tmass;
      dgSTL::copy_if( w_met.begin() , w_met.end() , back_inserter(w_tmass),
                      CUT_ONE(AnaWCrossSectionCandidate,w_baseline_sel,"transverse_mass") );
      dg::filleff("10_w_transverse_mass", w_tmass.size()>0);
      if( w_tmass.size()>0 ) dg::fillh("h10_w_transverse_mass",10,0,10,1);
      // NJet distribution
      if( w_tmass.size() > 0 ) {
        int njets = (*w_tmass.begin())->njets();
        if( njets > 3 ) njets = 3;
        dg::fillh( "n_jets" , 4 , 0 , 4 , njets );
      }
    } // end W+jets photon cut flow exercise

    if( AnaConfiguration::muon_trig()!=0 ) {
      dg::down( "cutflow_mu_baseline", "Muon cut flow for Baseline 2010" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      // muon mode:
      // Primary vertex is already checked above
      dg::filleff("00_vtx_grl",true);
      dg::fillh("h00_vtx_grl",10,0,10,1);
      // Muon trigger:
      vector<  boost::shared_ptr<const AnaMuon>  > mu_trig;
      switch ( AnaConfiguration::muon_trig() ) {
      case 1 :
      case 0 :
        dgSTL::copy_if( evt->muons().begin() , evt->muons().end() , back_inserter(mu_trig),
                        CUT_ONE(AnaMuon,mu_baseline_sel,"trigger_L1_MU10") ); // for A - E3
        break;
      case 2 :
        dgSTL::copy_if( evt->muons().begin() , evt->muons().end() , back_inserter(mu_trig),
                        CUT_ONE(AnaMuon,mu_baseline_sel,"trigger_EF_MU10_MG") ); // for E4 - G1
        break;
      case 3 :
        dgSTL::copy_if( evt->muons().begin() , evt->muons().end() , back_inserter(mu_trig),
                        CUT_ONE(AnaMuon,mu_baseline_sel,"trigger_EF_MU13_MG") ); // for G2 - I1 (167576)
        break;
      case 4 :
        dgSTL::copy_if( evt->muons().begin() , evt->muons().end() , back_inserter(mu_trig),
                        CUT_ONE(AnaMuon,mu_baseline_sel,"trigger_EF_MU13_MG_TIGHT") ); // for I1 (167607) - I2
        break;
      default :
        cout << " invalid muon trigger " << AnaConfiguration::muon_trig() << endl;
        exit(-1);
      }
      dg::filleff("01_mu_trig", mu_trig.size()>0);
      if( mu_trig.size()>0 ) dg::fillh("h01_mu_trig",10,0,10,1);
      // Muon isCombined:
      vector<  boost::shared_ptr<const AnaMuon>  > mu_isCombined;
      dgSTL::copy_if( mu_trig.begin() , mu_trig.end() , back_inserter(mu_isCombined),
                      CUT_ONE(AnaMuon,mu_baseline_sel,"staco") &&
                      CUT_ONE(AnaMuon,mu_baseline_sel,"is_combined") );
      dg::filleff("02_mu_isCombined", mu_isCombined.size()>0);
      if( mu_isCombined.size()>0 ) dg::fillh("h02_mu_isCombined",10,0,10,1);
      // Muon z0:
      vector<  boost::shared_ptr<const AnaMuon>  > mu_z0;
      BOOST_FOREACH( boost::shared_ptr<const AnaMuon> theMu, mu_isCombined ) { 
        float closest_z0_diff(std::numeric_limits<float>::max());
        boost::shared_ptr<const AnaVertex> closeVtx;
        BOOST_FOREACH( boost::shared_ptr<const AnaVertex> theVtx, vtx_baseline ) {
          float z0_diff = std::abs(theVtx->z() - theMu->z0());
          if( z0_diff < closest_z0_diff ) {
            closest_z0_diff = z0_diff;
            closeVtx = theVtx;
          }
        }
        if( closest_z0_diff < 10. ) {
          mu_z0.push_back(theMu);
        }
      }
      dg::filleff("03_mu_z0", mu_z0.size()>0 );
      if( mu_z0.size()>0 ) dg::fillh("h03_mu_z0",10,0,10,1);
      // Muon quality:
      vector<  boost::shared_ptr<const AnaMuon>  > mu_trackerHits;
      dgSTL::copy_if( mu_z0.begin() , mu_z0.end() , back_inserter(mu_trackerHits),
                      CUT_ONE(AnaMuon,mu_baseline_sel,"trackerHits") );
      dg::filleff("04_mu_trackerHits", mu_trackerHits.size()>0);
      if( mu_trackerHits.size()>0 ) dg::fillh("h04_mu_trackerHits",10,0,10,1);
      // Muon MS pt:
      vector<  boost::shared_ptr<const AnaMuon>  > mu_mspt;
      dgSTL::copy_if( mu_trackerHits.begin() , mu_trackerHits.end() , back_inserter(mu_mspt),
                      CUT_ONE(AnaMuon,mu_baseline_sel,"ptms") );
      dg::filleff("05_mu_mspt", mu_mspt.size()>0);
      if( mu_mspt.size()>0 ) dg::fillh("h05_mu_mspt",10,0,10,1);
      // Muon ID-MS pt:
      vector<  boost::shared_ptr<const AnaMuon>  > mu_idmspt;
      dgSTL::copy_if( mu_mspt.begin() , mu_mspt.end() , back_inserter(mu_idmspt),
                      CUT_ONE(AnaMuon,mu_baseline_sel,"ptDiffRatio") );
      dg::filleff("06_mu_idmspt", mu_idmspt.size()>0);
      if( mu_idmspt.size()>0 ) dg::fillh("h06_mu_idmspt",10,0,10,1);
      // Muon d0:
      vector<  boost::shared_ptr<const AnaMuon>  > mu_d0;
      BOOST_FOREACH( boost::shared_ptr<const AnaMuon> theMu, mu_idmspt ) {
        float closest_z0_diff(std::numeric_limits<float>::max());
        boost::shared_ptr<const AnaVertex> closeVtx;
        BOOST_FOREACH( boost::shared_ptr<const AnaVertex> theVtx, vtx_baseline ) {
          float z0_diff = std::abs(theVtx->z() - theMu->z0());
          if( z0_diff < closest_z0_diff ) {
            closest_z0_diff = z0_diff;
            closeVtx = theVtx;
          }
        }
        float d0_diff = detector::corrected_d0( theMu->d0() , theMu->z0() , theMu->phi() ,
                                                theMu->charge() , closeVtx->x() , closeVtx->y() , closeVtx->z() );
        if( d0_diff < 0.1 ) {
          mu_d0.push_back(theMu);
        }
      }
      dg::filleff("07_mu_d0", mu_d0.size()>0);
      if( mu_d0.size()>0 ) dg::fillh("h07_mu_d0",10,0,10,1);
      // Muon track iso:
      vector<  boost::shared_ptr<const AnaMuon>  > mu_iso;
      dgSTL::copy_if( mu_d0.begin() , mu_d0.end() , back_inserter(mu_iso),
                      CUT_ONE(AnaMuon,mu_baseline_sel,"ptCone20") );
      dg::filleff("08_mu_trackIso", mu_iso.size()>0);
      if( mu_iso.size()>0 ) dg::fillh("h08_mu_trackIso",10,0,10,1);
      // Muon exactly one:
      dg::filleff("09_mu_one",mu_iso.size()==1);
      if( mu_iso.size()==1 ) dg::fillh("h09_mu_one",10,0,10,1);
      // Muon pT:
      vector<  boost::shared_ptr<const AnaMuon>  > mu_pt;
      dgSTL::copy_if( mu_iso.begin() , mu_iso.end() , back_inserter(mu_pt),
                      CUT_ONE(AnaMuon,mu_baseline_sel,"pt") );
      dg::filleff("10_mu_pt", mu_pt.size()==1);
      if( mu_pt.size()==1 ) dg::fillh("h10_mu_pt",10,0,10,1);
      // Muon eta:
      vector<  boost::shared_ptr<const AnaMuon>  > mu_eta;
      dgSTL::copy_if( mu_pt.begin() , mu_pt.end() , back_inserter(mu_eta),
                      CUT_ONE(AnaMuon,mu_baseline_sel,"eta") );
      dg::filleff("11_mu_eta", mu_eta.size()==1);
      if( mu_eta.size()==1 ) dg::fillh("h11_mu_eta",10,0,10,1);
      // MET Cleaning
      vector<  boost::shared_ptr<const AnaMET>  > met_good;
      dgSTL::copy_if( evt->missing_et().begin() , evt->missing_et().end() , back_inserter(met_good),
                      CUT_ALL(AnaMET,met_baseline_el_sel) );
      dg::filleff("12_met_clean", met_good.size()>0 && mu_eta.size()==1);
      if( met_good.size()>0 && mu_eta.size()==1 ) dg::fillh("h12_met_clean",10,0,10,1);
      // W MET
      vector<  boost::shared_ptr<const AnaWCrossSectionCandidate>  > w_met;
      dgSTL::copy_if( wall_muon.begin() , wall_muon.end() , back_inserter(w_met),
                      CUT_ONE(AnaWCrossSectionCandidate,w_baseline_sel,"exactly_one_lepton") &&
                      CUT_ONE(AnaWCrossSectionCandidate,w_baseline_sel,"exactly_one_met") &&
                      CUT_ONE(AnaWCrossSectionCandidate,w_baseline_sel,"met") );
      dg::filleff("13_w_met", w_met.size()>0 );
      if( w_met.size()>1 ) dg::fillh( "h13_w_met",10,0,10,1 );
      // W Transverse Mass
      vector<  boost::shared_ptr<const AnaWCrossSectionCandidate>  > w_tmass;
      dgSTL::copy_if( w_met.begin() , w_met.end() , back_inserter(w_tmass),
                      CUT_ONE(AnaWCrossSectionCandidate,w_baseline_sel,"transverse_mass") );
      dg::filleff("14_w_transverse_mass", w_tmass.size()>0);
      if( w_tmass.size()>1 ) dg::fillh( "h14_w_transverse_mass",10,0,10,1 );
      // NJet distribution
      if( w_tmass.size() > 0 ) {
        int njets = (*w_tmass.begin())->njets();
        if( njets > 3 ) njets = 3;
        dg::fillh( "n_jets" , 4 , 0 , 4 , njets );
      }
    } // end W+jets muon cut flow exercise

    if( true ) {
      dg::down( "cutflow_mu_constantions", "Muon cut flow for Constantinos" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      // muon mode:
      // Primary vertex is already checked above
      dg::filleff("00_vtx_grl",true);
      dg::fillh("h00_vtx_grl",10,0,10,1);
      // Clean event
      dg::filleff("01_wall_mu", wall_muon.size()>0);
      if( wall_muon.size()>0 ) dg::fillh("h01_clean_evt",10,0,10,1);
      // 1 MET
      vector<  boost::shared_ptr<const AnaWCrossSectionCandidate>  > w_onemet;
      dgSTL::copy_if( wall_muon.begin() , wall_muon.end() , back_inserter( w_onemet ) ,
                      CUT_ONE(AnaWCrossSectionCandidate,w_constantinos_sel,"exactly_one_met" ) );
      dg::filleff( "02_w_onemet" , w_onemet.size()>0 );
      if( w_onemet.size()>0 ) dg::fillh("h02_w_onemet",10,0,10,1);
      // 2 jets
      vector<  boost::shared_ptr<const AnaWCrossSectionCandidate>  > w_twojets;
      dgSTL::copy_if( w_onemet.begin() , w_onemet.end() , back_inserter( w_twojets ) ,
                      CUT_ONE(AnaWCrossSectionCandidate,w_constantinos_sel,"2jets" ) );
      dg::filleff( "03_w_twojets" , w_twojets.size()>0 );
      if( w_twojets.size()>0 ) dg::fillh("h03_w_twojets",10,0,10,1);
      // make an eta cut on jets at 2.5 to match constantinos initial jet selection
      vector<  boost::shared_ptr<const AnaWCrossSectionCandidate>  > w_midjeteta;
      dgSTL::copy_if( w_twojets.begin() , w_twojets.end() , back_inserter( w_midjeteta ) ,
                      CUT_ONE(AnaWCrossSectionCandidate,w_constantinos_sel,"mid_dijet_eta" ) );
      dg::filleff( "04_w_midjetseta" , w_midjeteta.size()>0 );
      if( w_midjeteta.size()>0 ) dg::fillh("h04_w_midjetseta",10,0,10,1);
      // 1 lepton
      vector<  boost::shared_ptr<const AnaWCrossSectionCandidate>  > w_onelep;
      dgSTL::copy_if( w_midjeteta.begin() , w_midjeteta.end() , back_inserter( w_onelep ) ,
                      CUT_ONE(AnaWCrossSectionCandidate,w_constantinos_sel,"exactly_one_lepton" ) );
      dg::filleff( "05_w_onelep" , w_onelep.size()>0 );
      if( w_onelep.size()>0 ) dg::fillh("h05_w_onelep",10,0,10,1);
      // lepton pt > 40
      vector<  boost::shared_ptr<const AnaWCrossSectionCandidate>  > w_lepton_pt;
      dgSTL::copy_if( w_onelep.begin() , w_onelep.end() , back_inserter( w_lepton_pt ) ,
                      CUT_ONE(AnaWCrossSectionCandidate,w_constantinos_sel,"lepton_pt" ) );
      dg::filleff( "06_w_lepton_pt" , w_lepton_pt.size()>0 );
      if( w_lepton_pt.size()>0 ) dg::fillh("h06_w_lepton_pt",10,0,10,1);
      // met > 30
      vector<  boost::shared_ptr<const AnaWCrossSectionCandidate>  > w_met;
      dgSTL::copy_if( w_lepton_pt.begin() , w_lepton_pt.end() , back_inserter( w_met ) ,
                      CUT_ONE(AnaWCrossSectionCandidate,w_constantinos_sel,"met" ) );
      dg::filleff( "07_w_met" , w_met.size()>0 );
      if( w_met.size()>0 ) dg::fillh("h07_w_met",10,0,10,1);
      // mwjj in window 71-91
      vector<  boost::shared_ptr<const AnaWCrossSectionCandidate>  > w_jjmass;
      dgSTL::copy_if( w_met.begin() , w_met.end() , back_inserter( w_jjmass ) ,
                      CUT_ONE(AnaWCrossSectionCandidate,w_constantinos_sel,"jj_mass_in_window" ) );
      dg::filleff( "08_w_jjmass" , w_jjmass.size()>0 );
      if( w_jjmass.size()>0 ) dg::fillh("h08_w_jjmass",10,0,10,1);
      // both jets have eta<1.8
      vector<  boost::shared_ptr<const AnaWCrossSectionCandidate>  > w_jetseta;
      dgSTL::copy_if( w_jjmass.begin() , w_jjmass.end() , back_inserter( w_jetseta ) ,
                      CUT_ONE(AnaWCrossSectionCandidate,w_constantinos_sel,"low_dijet_eta" ) ); 
      dg::filleff( "09_w_jetseta" , w_jetseta.size()>0 );
      if( w_jetseta.size()>0 ) dg::fillh("h09_w_jetseta",10,0,10,1);
      // no b jets
      vector<  boost::shared_ptr<const AnaWCrossSectionCandidate>  > w_nobtag;
      dgSTL::copy_if( w_jetseta.begin() , w_jetseta.end() , back_inserter( w_nobtag ) ,
                      CUT_ONE(AnaWCrossSectionCandidate,w_constantinos_sel,"no_btag_jets" ) );
      dg::filleff( "10_w_nobtag" , w_nobtag.size()>0 );
      if( w_nobtag.size()>0 ) dg::fillh("h10_w_nobtag",10,0,10,1);
    } // end W+jets muon cut flow exercise


  } // end for each event

  AnaEventMgr::instance()->close_sample();

} // end analysis





class
drL
{
public:
  drL() {}
  template< typename typeA , typename typeB >
  const double operator()(  boost::shared_ptr<const typeA>  a ,  boost::shared_ptr<const typeB>  b ) const {
    return detector::quadrature( detector::delta_phi(a->phi(),b->phi()) , detector::delta_eta(a->eta(),b->eta()) );
  }
};

template<typename typeB>
class
drLessThanL
{
protected:
  boost::shared_ptr<const typeB>  _ref;
public:
  drLessThanL(  boost::shared_ptr<const typeB>  b ) : _ref(b) {}
  template< typename typeA >
  const bool operator()(  boost::shared_ptr<const typeA>  a1 ,  boost::shared_ptr<const typeA>  a2 ) const {
    static drL l;
    return( l(a1,_ref) < l(a2,_ref) );
  }
};

template< typename lepIterT , typename metIterT , typename jetIterT , typename vtxIterT , typename outputIterT >
void build_w_candidates( lepIterT lep_begin , lepIterT lep_end ,
                         metIterT met_begin , metIterT met_end ,
                         jetIterT jet_begin , jetIterT jet_end ,
                         vtxIterT vtx_begin , vtxIterT vtx_end ,
                         outputIterT output_iterator )
{
  boost::shared_ptr<AnaWCrossSectionCandidate> cand( AnaWCrossSectionCandidate::build( lep_begin , lep_end , 
                                                                                       met_begin , met_end , 
                                                                                       jet_begin , jet_end ,
                                                                                       vtx_begin , vtx_end ) );
  if( !cand ) { return; }
  (*output_iterator) = cand;
}


template< typename overlapIterT , typename refIterT , typename outputIterT >
void overlap_removal( overlapIterT overlap_begin , overlapIterT overlap_end ,
                      refIterT ref_begin , refIterT ref_end ,
                      const AnaParticle::ftype& max_overlap_dr ,
                      outputIterT output_iterator )
{
  if( overlap_begin == overlap_end ) { return; }
  if( ref_begin == ref_end ) {
    std::copy( overlap_begin , overlap_end , output_iterator );
    return;
  }
  assert( overlap_begin != overlap_end );
  assert( ref_begin != ref_end );
  
  std::vector< typename iterator_traits<overlapIterT>::value_type > overlap_coll( overlap_begin , overlap_end );
  unsigned int overlap_coll_size = overlap_coll.size();

  // for each reference particle, look for the particle in the overlap
  // collection with the smallest delta r. if that delta r is less
  // than or equal to max_overlap_dr , remove it.
  for( ; ref_begin!=ref_end && !overlap_coll.empty(); ++ref_begin ) {
    typename iterator_traits<refIterT>::value_type ref_particle( *ref_begin );
    if( overlap_coll_size > 1 ) {
      typename std::vector< typename iterator_traits<overlapIterT>::value_type >::reverse_iterator imid = overlap_coll.rbegin();
      ++imid;
      std::partial_sort( overlap_coll.rbegin() , imid , overlap_coll.rend() ,
                         bind( &AnaParticle::dist_eta_phi , _1 , ref_particle ) < bind( &AnaParticle::dist_eta_phi , _2 , ref_particle ) );
    }
    assert( !overlap_coll.empty() );
    if( overlap_coll.back()->dist_eta_phi( ref_particle ) <= max_overlap_dr ) {
      overlap_coll.pop_back();
      --overlap_coll_size;
    }
  }
  // the objects in overlap_coll have survived the overlap
  // removal. copy them to the output.
  std::copy( overlap_coll.begin() , overlap_coll.end() , output_iterator );
}

#ifdef F77_DUMMY_MAIN
#ifdef __cplusplus
extern "C"
#endif
int F77_DUMMY_MAIN() { abort(); return 1; }
#endif

