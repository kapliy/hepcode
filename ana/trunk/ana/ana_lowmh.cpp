
// low mass H->WW analysis code.

#include <algorithm>
#include <ext/algorithm>
#include <functional>
#include <cassert>
#include <cmath>
#include <string>
#include <iostream>
#include <iomanip>
#include <list>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/pointer_cast.hpp>
#include <boost/bind.hpp>
#include <boost/scope_exit.hpp>
#include <boost/foreach.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/assign/std/vector.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/dynamic_bitset.hpp>
#include "TrigFTKAna/serialize_bitset.hpp"
#include "TrigFTKAna/AnaConfiguration.hpp"
#include "TrigFTKAna/AnaEventMgr.hpp"
#include "TrigFTKAna/AnaElectron.hpp"
#include "TrigFTKAna/AnaPhoton.hpp"
#include "TrigFTKAna/AnaMuon.hpp"
#include "TrigFTKAna/AnaJet.hpp"
#include "TrigFTKAna/AnaMET.hpp"
#include "TrigFTKAna/AnaVertex.hpp"
#include "TrigFTKAna/AnaTrigger.hpp"
#include "TrigFTKAna/AnaTrack.hpp"
#include "TrigFTKAna/AnaZ.hpp"
#include "TrigFTKAna/AnaW.hpp"
//#include "TrigFTKAna/AnaWCrossSectionCandidate.hpp"
#include "TrigFTKAna/AnaTruthParticle.hpp"
#include "TrigFTKAna/AnaHWWCandidate.hpp"
#include "TrigFTKAna/GoodRunList.hpp"
#include "TrigFTKAna/EventObjectColl.hpp"
#include "DataGraphics/DataGraphics.hh"
#include "DataGraphics/DgCutSet.hh"
#include "DataGraphics/DgSTL.hh"
#include <DataGraphics/DgXMLMap.hh>
#include "TrigFTKAna/StudyComposite.hpp"
#include "TrigFTKAna/StudyJet.hpp"
#include "TrigFTKAna/StudyMET.hpp"
#include "TrigFTKAna/StudyHiggs.hpp"
#include "TrigFTKAna/StudyW.hpp"
#include "TrigFTKAna/StudyTagProbeZ.hpp"
#include "TrigFTKAna/StudyHwwTruthProducts.hpp"
#include "TrigFTKAna/StudyLeptonReconstruction.hpp"
#include "TrigFTKAna/StudyFakeLeptonsInZEvents.hpp"
#include "TrigFTKAna/StudyMuonFake.hpp"
#include "TrigFTKAna/StudyFakeMatrixMethod.hpp"
#include "TrigFTKAna/CompositeGenerator.hpp"
#include "TrigFTKAna/Conference.hpp"
#include "TrigFTKAna/egamma/EnergyRescaler.h"
#include "TrigFTKAna/misc/TPileupReweighting.h"
#include "CommonHiggs/egammaAnalysisUtils/egammaSFclass.C"
#include "CommonHiggs/RandomDataPeriod.h"
#include "CommonHiggs/muon_Staco_SF_EPS.h"

using namespace std;
using namespace DataGraphics;
using namespace boost;

template< typename overlapIterT , typename refIterT , typename outputIterT >
void overlap_removal_smallest_dr( overlapIterT overlap_begin , overlapIterT overlap_end ,
                                  refIterT ref_begin , refIterT ref_end ,
                                  const AnaParticle::ftype& max_overlap_dr ,
                                  outputIterT output_iterator );
template< typename overlapIterT , typename refIterT , typename outputIterT >
void overlap_removal_all_in_cone( overlapIterT overlap_begin , overlapIterT overlap_end ,
                                  refIterT ref_begin , refIterT ref_end ,
                                  const AnaParticle::ftype& max_overlap_dr ,
                                  outputIterT output_iterator );
template< typename overlapIterT , typename outputIterT >
void internal_overlap_removal_by_pt( overlapIterT overlap_begin , overlapIterT overlap_end ,
                                     const AnaParticle::ftype& max_overlap_dr ,
                                     outputIterT output_iterator ,
                                     const bool minDrOnly = true );

void hsg3_cutflow( const bool is_good_run , float& evtweight , 
                   const bool ok_clean_met , const bool ok_clean_met_lowpt , const bool bad_FEB , 
                   const EventObjectColl< shared_ptr<const AnaElectron> >& ele_hww ,
                   const EventObjectColl< shared_ptr<const AnaMuon> >& mu_hww ,
                   const EventObjectColl< shared_ptr<const AnaMuon> >& mu_hww_lowpt ,
                   const EventObjectColl< shared_ptr<const AnaMET> >& met_hww ,
                   const EventObjectColl< shared_ptr<const AnaJet> >& jet_hww ,
                   float& outweight , float febWeight , const float pileupWeight , const float mcweight );

// the following lines are a funny fortran+gcc way to say "this is the
// main function".
#ifdef __cplusplus
extern "C"
#endif
int
F77_MAIN( int argc , char* argv[] )
{
  // initialize configuration and event data manager. 
  //
  // run this program with "--help" to see a list of command line
  // options set by the following lines.
  AnaConfiguration::configure("ana_lowmh",argc,argv);
  if( !AnaConfiguration::configured() ) { exit(0); }
  dg::root()->no_save_at_exit();
  BOOST_SCOPE_EXIT() { dg::root()->do_save_at_exit(); } BOOST_SCOPE_EXIT_END;
  const CONF::ConfType conf = AnaConfiguration::conf();
  //
  AnaEventMgr* evtmgr = AnaEventMgr::instance(); assert(evtmgr);
  //
  // define event quality criteria
  // ================================================================
  GoodRunList grl;

  // grl.add( GoodRunList::ELECTRON , "good_run_lists/Wel_GRLs/WjetsEl_MayReprAB_FirstPassCDEFGHI_GRL.xml");
  // grl.add( GoodRunList::MUON     , "good_run_lists/Wmu_GRLs/WjetsMu_MayReprAB_FirstPassCDEFGHI_GRL.xml");

  // grl.add( GoodRunList::ELECTRON , "good_run_lists/Wmu_GRLs/WjetsMu_MayReprAB_FirstPassCDEFGHI_GRL.xml");
  // grl.add( GoodRunList::MUON     , "good_run_lists/Wmu_GRLs/WjetsMu_MayReprAB_FirstPassCDEFGHI_GRL.xml");

  grl.add( GoodRunList::ELECTRON , "./CommonHiggs/grl/data11_7TeV.periodAllYear_DetStatus-v28-pro08-07_CoolRunQuery-00-04-00_WZjets_allchannels.xml" );
  grl.add( GoodRunList::MUON , "./CommonHiggs/grl/data11_7TeV.periodAllYear_DetStatus-v28-pro08-07_CoolRunQuery-00-04-00_WZjets_allchannels.xml" );
   // grl.add( GoodRunList::ELECTRON , "./CommonHiggs/grl/data11_7TeV.periodAllYear_DetStatus-v18-pro08-04_WZjets_allchannels.xml");
  //data11_7TeV.periodAllYear_DetStatus-v18-pro08-04_CoolRunQuery-00-03-98_WZjets_allchannels.xml");
  // grl.add( GoodRunList::MUON     , "./CommonHiggs/grl/data11_7TeV.periodAllYear_DetStatus-v18-pro08-04_WZjets_allchannels.xml");
  //data11_7TeV.periodAllYear_DetStatus-v18-pro08-04_CoolRunQuery-00-03-98_WZjets_allchannels.xml");
  grl.add( GoodRunList::GAMMAJET , "./CommonHiggs/grl/data11_7TeV.periodAllYear_DetStatus-v28-pro08-07_CoolRunQuery-00-04-00_GammaJet.xml" );

  // define RandomDataPeriod for FEB treatment
  // https://twiki.cern.ch/twiki/bin/view/AtlasProtected/BaslinecutsforWWatPLHC#Technical_Details
  RandomDataPeriod mc_dataperiod;
  mc_dataperiod.Load("./good_run_lists/asym_runlumi_muons_L7.dat"); // update if using a different period!

  // define primary vertex selection
  // ================================================================
  EventObjectColl< shared_ptr<const AnaVertex> > primary_vertices;
  EventObjectColl< shared_ptr<const AnaVertex> > jetcalib_primary_vertices;
  DgCutSet< shared_ptr<const AnaVertex> > primary_vertex_selector( "primary_vertex_selector" , primary_vertices.attach() );
  primary_vertex_selector.add_uu( "n_tracks", bind(&AnaVertex::ntracks,_1)>=3, bind( &dg::fillh , _1 , 50 , 0 , 50 , bind(&AnaVertex::ntracks,_2) ) );
  // primary_vertex_selector.add_uu( "pv_z", bind( std::ptr_fun<AnaVertex::ftype,AnaVertex::ftype>(std::abs) , bind(&AnaVertex::z,_1) ) < 150./*mm*/, 
  //                               bind( &dg::fillh , _1 , 90 , -300 , 300 , bind(&AnaVertex::z,_2) ) );
  DgCutSet< shared_ptr<const AnaVertex> > jetcalib_primary_vertex_selector( "jetcalib_primary_vertex_selector" , jetcalib_primary_vertices.attach() );
  jetcalib_primary_vertex_selector.add_uu( "n_tracks", bind(&AnaVertex::ntracks,_1)>=5, bind( &dg::fillh , _1 , 50 , 0 , 50 , bind(&AnaVertex::ntracks,_2) ) );
  //
  // set up particle selectors
  // ================================================================
  //
  // electron selection
  // ----------------------------------------------------------------
  //
  EventObjectColl< shared_ptr<const AnaElectron> > ele_event;
  EventObjectColl< shared_ptr<const AnaElectron> > ele_FEB;
  DgCutSet< shared_ptr<const AnaElectron> > ele_FEB_selector( "ele_FEB_selector" , ele_FEB.attach() );
  ele_FEB_selector.add_uu( "pt" , bind(&AnaElectron::pt,_1)>15. , 
                           bind(&dg::fillh, _1 , 100 , 0 , 200 , bind(&AnaElectron::pt,_2) ) );
  // probe electrons; basic acceptance cuts but no id. used as denominator for id optimization.
  EventObjectColl< shared_ptr<const AnaElectron> > ele_probe;
  EventObjectColl< shared_ptr<const AnaElectron> > ele_expanded_probe;
  DgCutSet< shared_ptr<const AnaElectron> > ele_probe_selector( "ele_probe_selector" , ele_probe.attach() );  
  ele_probe_selector.add_uu( "pt" , bind(&AnaElectron::pt,_1)>=5. , 
                             bind(&dg::fillh, _1 , 100 , 0 , 200 , bind(&AnaElectron::pt,_2) ) );
  DgCutSet< shared_ptr<const AnaElectron> > ele_expanded_probe_selector( "ele_expanded_probe_selector" , ele_expanded_probe.attach() );  
  ele_expanded_probe_selector.add_uu( "pt" , bind(&AnaParticle::pt,_1)>=5. , 
                                      bind(&dg::fillh, _1 , 100 , 0 , 200 , bind(&AnaParticle::pt,_2) ) );
  EventObjectColl< shared_ptr<const AnaElectron> > ele_hww;
  EventObjectColl< shared_ptr<const AnaElectron> > ele_hww_withoverlap;
  DgCutSet< shared_ptr<const AnaElectron> > ele_hww_selector( "ele_hww_selector" , ele_hww_withoverlap.attach() );
  ele_hww_selector.add_uu( "author" , 
                           bind(&AnaElectron::author,_1)==1 || bind(&AnaElectron::author,_1)==3 ,
                           bind( &dg::fillh , _1 , 10 , 0 , 10 , bind(&AnaElectron::author,_2) ) );  
  ele_hww_selector.add_uu( "pt" , bind(&AnaElectron::pt,_1) > 15. , 
                           bind(&dg::fillh, _1 , 100. , 0. , 200. , bind(&AnaElectron::pt,_2) ) );
  ele_hww_selector.add_uu( "is_fiducial_247" , bind(&AnaElectron::absclustereta,_1)<2.47 , bind( &dg::fillh , _1 , 40 , -4 , 4 , bind(&AnaElectron::cluster_eta,_2) ) );
  ele_hww_selector.add_uu( "is_fiducial_forward_crack" , bind(&AnaElectron::absclustereta,_1)<=1.37 || bind(&AnaElectron::absclustereta,_1)>=1.52 ,
                           bind( &dg::fillh , _1 , 40 , -4 , 4 , bind(&AnaElectron::cluster_eta,_2) ) );
  ele_hww_selector.add_uu( "good_oq" , bind( &AnaElectron::is_good_oq , _1 ) == true ,
                           bind(&dg::fillh,_1,100,-4,4,bind(&AnaElectron::eta,_2)) );
  ele_hww_selector.add_uu( "is_tight" , bind(&AnaElectron::is_ElectronTight_v16,_1)==true , 
                           //  ele_hww_selector.add_uu( "is_tight" , bind(&AnaElectron::is_tight,_1)==true , 
                           bind( &dg::fillh , _1 , 40 , -4 , 4 , bind(&AnaElectron::eta,_2) , string("#eta") ) );
  ele_hww_selector.add_uu( "from_beamline_z0" , bind( std::ptr_fun<AnaParticle::ftype,AnaParticle::ftype>(std::abs) , bind( &AnaElectron::z0_exPV , _1 ) ) < 10. , 
                           bind( &dg::fillh , _1 , 160 , -40 , 40 , bind( &AnaElectron::z0_exPV , _2 ) ) );
  ele_hww_selector.add_uu( "from_beamline_d0_signif" , bind( &AnaElectron::abs_d0_exPV_signif , _1 ) < 10. , 
                           bind( &dg::fillh , _1 , 100 , -20 , 20 , bind( &AnaElectron::d0_exPV_signif , _2 ) ) );
  ele_hww_selector.add_uu( "et_cone_20_ratio" , bind(&AnaElectron::uncorrected_et_cone_ratio,_1,AnaElectron::CONE20)<0.15 , 
                           bind(&dg::fillh, _1 , 100 , 0 , 2 , bind(&AnaElectron::uncorrected_et_cone_ratio,_2,AnaElectron::CONE20) ) );
  ele_hww_selector.add_uu( "pt_cone_20_ratio" , bind(&AnaElectron::pt_cone_ratio,_1,AnaElectron::CONE20)<0.1 , 
                           bind(&dg::fillh, _1 , 100 , 0 , 2 , bind(&AnaElectron::pt_cone_ratio,_2,AnaElectron::CONE20) ) ); 
  // low pt electrons as defined similar to the SM WW observation
  EventObjectColl< shared_ptr<const AnaElectron> > ele_hww_lowpt;
  EventObjectColl< shared_ptr<const AnaElectron> > ele_hww_lowpt_withoverlap;
  DgCutSet< shared_ptr<const AnaElectron> > ele_hww_lowpt_selector( "ele_hww_lowpt_selector" , ele_hww_lowpt_withoverlap.attach() );
  ele_hww_lowpt_selector.add_uu( "author" , 
                           bind(&AnaElectron::author,_1)==1 || bind(&AnaElectron::author,_1)==3 ,
                           bind( &dg::fillh , _1 , 10 , 0 , 10 , bind(&AnaElectron::author,_2) ) );  
  ele_hww_lowpt_selector.add_uu( "pt" , bind(&AnaElectron::pt,_1) > 15. , 
                           bind(&dg::fillh, _1 , 100. , 0. , 200. , bind(&AnaElectron::pt,_2) ) );
  ele_hww_lowpt_selector.add_uu( "is_fiducial_247" , bind(&AnaElectron::absclustereta,_1)<2.47 , bind( &dg::fillh , _1 , 40 , -4 , 4 , bind(&AnaElectron::cluster_eta,_2) ) );
  ele_hww_lowpt_selector.add_uu( "is_fiducial_forward_crack" , bind(&AnaElectron::absclustereta,_1)<=1.37 || bind(&AnaElectron::absclustereta,_1)>=1.52 ,
                           bind( &dg::fillh , _1 , 40 , -4 , 4 , bind(&AnaElectron::cluster_eta,_2) ) );
  ele_hww_lowpt_selector.add_uu( "good_oq" , bind( &AnaElectron::is_good_oq , _1 ) == true ,
                           bind(&dg::fillh,_1,100,-4,4,bind(&AnaElectron::eta,_2)) );
  ele_hww_lowpt_selector.add_uu( "is_tight" , bind(&AnaElectron::is_ElectronTight_v16,_1)==true , 
                           //  ele_hww_lowpt_selector.add_uu( "is_tight" , bind(&AnaElectron::is_tight,_1)==true , 
                           bind( &dg::fillh , _1 , 40 , -4 , 4 , bind(&AnaElectron::eta,_2) , string("#eta") ) );
  ele_hww_lowpt_selector.add_uu( "from_beamline_z0" , bind( std::ptr_fun<AnaParticle::ftype,AnaParticle::ftype>(std::abs) , bind( &AnaElectron::z0_exPV , _1 ) ) < 10. , 
                           bind( &dg::fillh , _1 , 160 , -40 , 40 , bind( &AnaElectron::z0_exPV , _2 ) ) );
  ele_hww_lowpt_selector.add_uu( "from_beamline_d0_signif" , bind( &AnaElectron::abs_d0_exPV_signif , _1 ) < 10. , 
                           bind( &dg::fillh , _1 , 100 , -20 , 20 , bind( &AnaElectron::d0_exPV_signif , _2 ) ) );
  ele_hww_lowpt_selector.add_uu( "et_cone_20_ratio" , bind(&AnaElectron::et_cone_ratio,_1,AnaElectron::CONE20)<0.15 , 
                           bind(&dg::fillh, _1 , 100 , 0 , 2 , bind(&AnaElectron::et_cone_ratio,_2,AnaElectron::CONE20) ) );
  ele_hww_lowpt_selector.add_uu( "pt_cone_20_ratio" , bind(&AnaElectron::pt_cone_ratio,_1,AnaElectron::CONE20)<0.1 , 
                           bind(&dg::fillh, _1 , 100 , 0 , 2 , bind(&AnaElectron::pt_cone_ratio,_2,AnaElectron::CONE20) ) ); 
  EventObjectColl< shared_ptr<const AnaElectron> > ele_nontrigger_loose;
  DgCutSet< shared_ptr<const AnaElectron> > ele_nontrigger_loose_selector( "ele_nontrigger_loose_selector" , ele_nontrigger_loose.attach() );
  ele_nontrigger_loose_selector.add_uu( "author" , 
                                        bind(&AnaElectron::author,_1)==1 || bind(&AnaElectron::author,_1)==3 ,
                                        bind( &dg::fillh , _1 , 10 , 0 , 10 , bind(&AnaElectron::author,_2) ) );
  ele_nontrigger_loose_selector.add_uu( "pt" , bind(&AnaElectron::pt,_1)>=10 , 
                                        bind(&dg::fillh, _1 , 100 , 0 , 200 , bind(&AnaElectron::pt,_2) ) );
  ele_nontrigger_loose_selector.add_uu( "is_fiducial_247" , bind(&AnaElectron::abseta,_1)<2.47 , bind( &dg::fillh , _1 , 40 , -4 , 4 , bind(&AnaElectron::eta,_2) ) );
  ele_nontrigger_loose_selector.add_uu( "is_fiducial_forward_crack" , bind(&AnaElectron::abseta,_1)<=1.37 || bind(&AnaElectron::abseta,_1)>=1.52 ,
                                        bind( &dg::fillh , _1 , 40 , -4 , 4 , bind(&AnaElectron::eta,_2) ) );
  ele_nontrigger_loose_selector.add_uu( "good_oq" , bind( &AnaElectron::is_good_oq , _1 ) == true ,
                                        bind(&dg::fillh,_1,100,-4,4,bind(&AnaElectron::eta,_2)) );
  ele_nontrigger_loose_selector.add_uu( "is_loosepp" , bind(&AnaElectron::is_ElectronLoosePlusPlus_v16,_1)==true , 
                                        bind( &dg::fillh , _1 , 40 , -4 , 4 , bind(&AnaElectron::eta,_2) , string("#eta") ) );
  ele_nontrigger_loose_selector.add_uu( "from_beamline_z0" , bind( &AnaElectron::track_exPV_z0 , _1 ) < 10. ,
                                        bind( &dg::fillh , _1 , 160 , -40 , 40 , bind( &AnaElectron::track_exPV_z0 , _2 ) ) );
  ele_nontrigger_loose_selector.add_uu( "from_beamline_d0_signif" , bind( &AnaElectron::track_exPV_abs_d0_signif , _1 ) < 10 ,
                                        bind( &dg::fillh , _1 , 100 , -20 , 20 , bind( &AnaElectron::track_exPV_d0_signif , _2 ) ) );
  ele_nontrigger_loose_selector.add_uu( "et_cone_20_ratio" , bind(&AnaElectron::et_cone_ratio,_1,AnaElectron::CONE20)<0.15 , 
                                        bind(&dg::fillh, _1 , 100 , 0 , 2 , bind(&AnaElectron::et_cone_ratio,_2,AnaElectron::CONE20) ) );
  ele_nontrigger_loose_selector.add_uu( "pt_cone_20_ratio" , bind(&AnaElectron::pt_cone_ratio,_1,AnaElectron::CONE20)<0.1 , 
                                        bind(&dg::fillh, _1 , 100 , 0 , 2 , bind(&AnaElectron::pt_cone_ratio,_2,AnaElectron::CONE20) ) );
  // anti-selected electrons
  EventObjectColl< shared_ptr<const AnaElectron> > ele_anti;
  DgCutSet< shared_ptr<const AnaElectron> > ele_anti_selector( "ele_anti_selector" , ele_anti.attach() );
  ele_anti_selector.add_uu( "author" , 
                            bind(&AnaElectron::author,_1)==1 || bind(&AnaElectron::author,_1)==3 ,
                            bind( &dg::fillh , _1 , 10 , 0 , 10 , bind(&AnaElectron::author,_2) ) );
  ele_anti_selector.add_uu( "pt" , bind(&AnaElectron::pt,_1)>=15 , 
                            bind(&dg::fillh, _1 , 100 , 0 , 200 , bind(&AnaElectron::pt,_2) ) );
  ele_anti_selector.add_uu( "is_fiducial_247" , bind(&AnaElectron::abseta,_1)<2.47 , bind( &dg::fillh , _1 , 40 , -4 , 4 , bind(&AnaElectron::eta,_2) ) );
  ele_anti_selector.add_uu( "is_fiducial_forward_crack" , bind(&AnaElectron::abseta,_1)<=1.37 || bind(&AnaElectron::abseta,_1)>=1.52 ,
                            bind( &dg::fillh , _1 , 40 , -4 , 4 , bind(&AnaElectron::eta,_2) ) );
  ele_anti_selector.add_uu( "good_oq" , bind( &AnaElectron::is_good_oq , _1 ) == true ,
                            bind(&dg::fillh,_1,100,-4,4,bind(&AnaElectron::eta,_2)) );
  ele_anti_selector.add_uu( "is_qcd" , bind(&AnaElectron::is_QCD_v16,_1)==true , 
                            bind( &dg::fillh , _1 , 40 , -4 , 4 , bind(&AnaElectron::eta,_2) , string("#eta") ) );
  ele_anti_selector.add_uu( "from_beamline_z0" , bind( &AnaElectron::track_exPV_z0 , _1 ) < 10. ,
                            bind( &dg::fillh , _1 , 160 , -40 , 40 , bind( &AnaElectron::track_exPV_z0 , _2 ) ) );
  ele_anti_selector.add_uu( "from_beamline_d0_signif" , bind( &AnaElectron::track_exPV_abs_d0_signif , _1 ) < 10 ,
                            bind( &dg::fillh , _1 , 100 , -20 , 20 , bind( &AnaElectron::track_exPV_d0_signif , _2 ) ) );
  ele_anti_selector.add_uu( "et_cone_20_ratio" , bind(&AnaElectron::et_cone_ratio,_1,AnaElectron::CONE20)<0.15 , 
                            bind(&dg::fillh, _1 , 100 , 0 , 2 , bind(&AnaElectron::et_cone_ratio,_2,AnaElectron::CONE20) ) );
  ele_anti_selector.add_uu( "pt_cone_20_ratio" , bind(&AnaElectron::pt_cone_ratio,_1,AnaElectron::CONE20)<0.1 , 
                            bind(&dg::fillh, _1 , 100 , 0 , 2 , bind(&AnaElectron::pt_cone_ratio,_2,AnaElectron::CONE20) ) );

  //
  // muon selection
  // ----------------------------------------------------------------
  // source muon collection. if this is mc, the muons here are smeared 
  // according to the Muon Combined Performance prescription. if this is data,
  // they are taken directly from the input ntuple.
  EventObjectColl< shared_ptr<const AnaMuon> > mu_event;
  //
  // probe muons; basic acceptance cuts but no id. used as denominator for id optimization.
  EventObjectColl< shared_ptr<const AnaMuon> > mu_probe_staco;
  EventObjectColl< shared_ptr<const AnaMuon> > mu_probe_muid;
  EventObjectColl< shared_ptr<const AnaMuon> > mu_expanded_probe;
  DgCutSet< shared_ptr<const AnaMuon> > mu_probe_staco_selector( "mu_probe_staco_selector" , mu_probe_staco.attach() );
  mu_probe_staco_selector.add_uu( "staco" , bind(&AnaMuon::is_staco_any,_1)==true , bind( &dg::filleff ,  _1 , _2 ) );
  mu_probe_staco_selector.add_uu( "pt" , bind(&AnaMuon::pt,_1)>=5. , 
                                  bind(&dg::fillh, _1 , 100 , 0 , 200 , bind(&AnaMuon::pt,_2) ) );
  DgCutSet< shared_ptr<const AnaMuon> > mu_probe_muid_selector( "mu_probe_muid_selector" , mu_probe_muid.attach() );
  mu_probe_muid_selector.add_uu( "muid" , bind(&AnaMuon::is_muid,_1)==true , bind( &dg::filleff ,  _1 , _2 ) );
  mu_probe_muid_selector.add_uu( "pt" , bind(&AnaMuon::pt,_1)>=5. , 
                                 bind(&dg::fillh, _1 , 100 , 0 , 200 , bind(&AnaMuon::pt,_2) ) );
  DgCutSet< shared_ptr<const AnaMuon> > mu_expanded_probe_selector( "mu_expanded_probe_selector" , mu_expanded_probe.attach() );
  mu_expanded_probe_selector.add_uu( "pt" , bind(&AnaParticle::pt,_1)>=5. , 
                                     bind(&dg::fillh, _1 , 100 , 0 , 200 , bind(&AnaParticle::pt,_2) ) );
    
  // H WW muons
  EventObjectColl< shared_ptr<const AnaMuon> > mu_hww_preiso;
  EventObjectColl< shared_ptr<const AnaMuon> > mu_hww;
  EventObjectColl< shared_ptr<const AnaMuon> > mu_hww_lowpt_preiso;
  EventObjectColl< shared_ptr<const AnaMuon> > mu_hww_lowpt;
  DgCutSet< shared_ptr<const AnaMuon> > mu_hww_common_selector( "mu_hww_common_selector" );
  mu_hww_common_selector.add_uu( "is_staco_combined" , bind(&AnaMuon::is_staco_combined,_1)==true , 
                                 bind( &dg::fillh , _1 , 48 , -M_PI , M_PI , bind(&AnaMuon::phi,_2) , string("#phi") ) );
  mu_hww_common_selector.add_uu( "is_fiducial_24" , bind(&AnaMuon::abseta,_1)<2.4 , bind( &dg::fillh , _1 , 40 , -4 , 4 , bind(&AnaMuon::abseta,_2) ) );
  mu_hww_common_selector.add_uu( "b_layer_if_expected", 
                                 bind(&AnaMuon::passes_hits_mcp_blayer_v16,_1)==true ,
                                 bind(&dg::fillh,_1,5,0,5,bind(&AnaMuon::nhits,_2,AnaMuon::BL)) );
  mu_hww_common_selector.add_uu( "pix_hits", 
                                 bind(&AnaMuon::passes_hits_mcp_pix_v16,_1)==true ,
                                 bind(&dg::fillh,_1,5,0,5,bind(&AnaMuon::nhits,_2,AnaMuon::PIXEL)) );
  mu_hww_common_selector.add_uu( "sct_hits", 
                                 bind(&AnaMuon::passes_hits_mcp_sct_v16,_1)==true ,
                                 bind(&dg::fillh,_1,5,0,5,bind(&AnaMuon::nhits,_2,AnaMuon::SCT)) );
  mu_hww_common_selector.add_uu( "si_holes", 
                                 bind(&AnaMuon::passes_hits_mcp_holes_v16,_1)==true ,
                                 bind(&dg::fillh,_1,20,0,20,bind(&AnaMuon::nholes_silicon,_2)) );
  mu_hww_common_selector.add_uu( "sitrt_hits", 
                                 bind(&AnaMuon::passes_hits_mcp_sitrt_v16,_1)==true ,
                                 bind(&dg::fillh,_1,60,0,60,bind(&AnaMuon::nhits,_2)) );
  mu_hww_common_selector.add_uu( "from_beamline_z0" , bind( std::ptr_fun<AnaMuon::ftype,AnaMuon::ftype>(std::abs), bind(&AnaMuon::z0_exPV , _1 )) < 10. ,
                                 bind( &dg::fillh , _1 , 160 , -40 , 40 , bind( &AnaMuon::z0_exPV , _2 ) ) );
  mu_hww_common_selector.add_uu( "from_beamline_d0_signif" , bind( &AnaMuon::abs_d0_exPV_signif , _1 ) < 10. ,
                                 bind( &dg::fillh , _1 , 100 , -20 , 20 , bind( &AnaMuon::d0_exPV_signif , _2 ) ) );
  DgCutSet< shared_ptr<const AnaMuon> > mu_hww_preiso_selector( "mu_hww_preiso_selector" , mu_hww_preiso.attach() );
  mu_hww_preiso_selector.based_on( mu_hww_common_selector );
  mu_hww_preiso_selector.add_uu( "pt" , bind(&AnaMuon::pt,_1)>=15. , 
                                 bind(&dg::fillh, _1 , 100 , 0 , 200 , bind(&AnaMuon::pt,_2) ) );
  DgCutSet< shared_ptr<const AnaMuon> > mu_hww_selector( "mu_hww_selector" , mu_hww.attach() );
  mu_hww_selector.based_on( mu_hww_common_selector );
  mu_hww_selector.add_uu( "pt" , bind(&AnaMuon::pt,_1)>=15. , 
                                 bind(&dg::fillh, _1 , 100 , 0 , 200 , bind(&AnaMuon::pt,_2) ) );
  mu_hww_selector.add_uu( "et_cone_20_ratio" , bind(&AnaMuon::et_cone_ratio,_1,AnaMuon::CONE20) < 0.15 , 
                                 bind(&dg::fillh, _1 , 100 , 0. , 1. , bind(&AnaMuon::et_cone_ratio,_2,AnaMuon::CONE20) ) );
  mu_hww_selector.add_uu( "pt_cone_20_ratio" , bind(&AnaMuon::pt_cone_ratio,_1,AnaMuon::CONE20) < 0.1 , 
                                 bind(&dg::fillh, _1 , 100 , 0. , 1. , bind(&AnaMuon::pt_cone_ratio,_2,AnaMuon::CONE20) ) );
  // hww muons with lower pt threshold.
  DgCutSet< shared_ptr<const AnaMuon> > mu_hww_lowpt_preiso_selector( "mu_hww_lowpt_preiso_selector" , mu_hww_lowpt_preiso.attach() );
  mu_hww_lowpt_preiso_selector.based_on( mu_hww_common_selector );
  mu_hww_lowpt_preiso_selector.add_uu( "pt" , bind(&AnaMuon::pt,_1)>=10. , 
                                       bind(&dg::fillh, _1 , 100 , 0 , 200 , bind(&AnaMuon::pt,_2) ) );
  DgCutSet< shared_ptr<const AnaMuon> > mu_hww_lowpt_selector( "mu_hww_lowpt_selector" , mu_hww_lowpt.attach() );
  mu_hww_lowpt_selector.based_on( mu_hww_common_selector );
  mu_hww_lowpt_selector.add_uu( "pt" , bind(&AnaMuon::pt,_1)>=10. , 
                                bind(&dg::fillh, _1 , 100 , 0 , 200 , bind(&AnaMuon::pt,_2) ) );
  mu_hww_lowpt_selector.add_uu( "et_cone_20_ratio" , bind(&AnaMuon::et_cone_ratio,_1,AnaMuon::CONE20) < 0.15 , 
                                bind(&dg::fillh, _1 , 100 , 0. , 1. , bind(&AnaMuon::et_cone_ratio,_2,AnaMuon::CONE20) ) );
  mu_hww_lowpt_selector.add_uu( "pt_cone_20_ratio" , bind(&AnaMuon::pt_cone_ratio,_1,AnaMuon::CONE20) < 0.1 , 
                                bind(&dg::fillh, _1 , 100 , 0. , 1. , bind(&AnaMuon::pt_cone_ratio,_2,AnaMuon::CONE20) ) );
  // hww muons with lower pt threshold
  // EventObjectColl< shared_ptr<const AnaMuon> > mu_hww_lowpt;
  // DgCutSet< shared_ptr<const AnaMuon> > mu_hww_lowpt_selector( "mu_hww_lowpt_selector" , mu_hww_lowpt.attach() );
  // mu_hww_lowpt_selector.add_uu( "is_staco_combined" , bind(&AnaMuon::is_staco_combined,_1)==true , 
  //                               bind( &dg::fillh , _1 , 48 , -M_PI , M_PI , bind(&AnaMuon::phi,_2) , string("#phi") ) );
  // mu_hww_lowpt_selector.add_uu( "is_fiducial_24" , bind(&AnaMuon::abseta,_1)<2.4 , bind( &dg::fillh , _1 , 40 , -4 , 4 , bind(&AnaMuon::abseta,_2) ) );
  // mu_hww_lowpt_selector.add_uu( "pt" , bind(&AnaMuon::pt,_1)>=7. , 
  //                               bind(&dg::fillh, _1 , 100 , 0 , 200 , bind(&AnaMuon::pt,_2) ) );
  // mu_hww_lowpt_selector.add_uu( "b_layer_if_expected", 
  //                               bind(&AnaMuon::passes_hits_mcp_blayer_v16,_1)==true ,
  //                               bind(&dg::fillh,_1,5,0,5,bind(&AnaMuon::nhits,_2,AnaMuon::BL)) );
  // mu_hww_lowpt_selector.add_uu( "pix_hits", 
  //                               bind(&AnaMuon::passes_hits_mcp_pix_v16,_1)==true ,
  //                               bind(&dg::fillh,_1,5,0,5,bind(&AnaMuon::nhits,_2,AnaMuon::PIXEL)) );
  // mu_hww_lowpt_selector.add_uu( "sct_hits", 
  //                               bind(&AnaMuon::passes_hits_mcp_sct_v16,_1)==true ,
  //                               bind(&dg::fillh,_1,5,0,5,bind(&AnaMuon::nhits,_2,AnaMuon::SCT)) );
  // mu_hww_lowpt_selector.add_uu( "si_holes", 
  //                               bind(&AnaMuon::passes_hits_mcp_holes_v16,_1)==true ,
  //                               bind(&dg::fillh,_1,20,0,20,bind(&AnaMuon::nholes_silicon,_2)) );
  // mu_hww_lowpt_selector.add_uu( "sitrt_hits", 
  //                               bind(&AnaMuon::passes_hits_mcp_sitrt_v16,_1)==true ,
  //                               bind(&dg::fillh,_1,60,0,60,bind(&AnaMuon::nhits,_2)) );
  // mu_hww_lowpt_selector.add_uu( "from_beamline_z0" , bind( std::ptr_fun<AnaMuon::ftype,AnaMuon::ftype>(std::abs), bind(&AnaMuon::z0_exPV , _1 )) < 10. ,
  //                               bind( &dg::fillh , _1 , 160 , -40 , 40 , bind( &AnaMuon::z0_exPV , _2 ) ) );
  // mu_hww_lowpt_selector.add_uu( "from_beamline_d0_signif" , bind( &AnaMuon::abs_d0_exPV_signif , _1 ) < 10. ,
  //                               bind( &dg::fillh , _1 , 100 , -20 , 20 , bind( &AnaMuon::d0_exPV_signif , _2 ) ) );
  // mu_hww_lowpt_selector.add_uu( "et_cone_20_ratio" , bind(&AnaMuon::et_cone_ratio,_1,AnaMuon::CONE20) < 0.15 , 
  //                               bind(&dg::fillh, _1 , 100 , 0. , 1. , bind(&AnaMuon::et_cone_ratio,_2,AnaMuon::CONE20) ) );
  // mu_hww_lowpt_selector.add_uu( "pt_cone_20_ratio" , bind(&AnaMuon::pt_cone_ratio,_1,AnaMuon::CONE20) < 0.1 , 
  //                               bind(&dg::fillh, _1 , 100 , 0. , 1. , bind(&AnaMuon::pt_cone_ratio,_2,AnaMuon::CONE20) ) );
  // muons like LBL SS muon pair analysis
  EventObjectColl< shared_ptr<const AnaMuon> > mu_lbl_tight;
  EventObjectColl< shared_ptr<const AnaMuon> > mu_lbl_loose;
  EventObjectColl< shared_ptr<const AnaMuon> > mu_lbl_preiso;
  EventObjectColl< shared_ptr<const AnaMuon> > mu_lbl_preiso_pred0sig;
  EventObjectColl< shared_ptr<const AnaMuon> > mu_lbl_nonprompt_preiso;
  DgCutSet< shared_ptr<const AnaMuon> > mu_lbl_common_selector( "mu_lbl_common_selector" );
  DgCutSet< shared_ptr<const AnaMuon> > mu_lbl_tight_selector( "mu_lbl_tight_selector" , mu_lbl_tight.attach() );
  DgCutSet< shared_ptr<const AnaMuon> > mu_lbl_loose_selector( "mu_lbl_loose_selector" , mu_lbl_loose.attach() );
  DgCutSet< shared_ptr<const AnaMuon> > mu_lbl_nonprompt_preiso_selector( "mu_lbl_nonprompt_preiso_selector" , mu_lbl_nonprompt_preiso.attach() );
  DgCutSet< shared_ptr<const AnaMuon> > mu_lbl_preiso_selector( "mu_lbl_preiso_selector" , mu_lbl_preiso.attach() );
  DgCutSet< shared_ptr<const AnaMuon> > mu_lbl_preiso_pred0sig_selector( "mu_lbl_preiso_pred0sig_selector" , mu_lbl_preiso_pred0sig.attach() );
  mu_lbl_common_selector.add_uu( "is_staco_combined" , bind(&AnaMuon::is_staco_combined,_1)==true , 
                                 bind( &dg::fillh , _1 , 48 , -M_PI , M_PI , bind(&AnaMuon::phi,_2) , string("#phi") ) );
  mu_lbl_common_selector.add_uu( "is_fiducial_24" , bind(&AnaMuon::abseta,_1)<2.4 , bind( &dg::fillh , _1 , 40 , -4 , 4 , bind(&AnaMuon::abseta,_2) ) );
  mu_lbl_common_selector.add_uu( "pt" , bind(&AnaMuon::pt,_1)>=10. , 
                                 bind(&dg::fillh, _1 , 100 , 0 , 200 , bind(&AnaMuon::pt,_2) ) );
  mu_lbl_common_selector.add_uu( "b_layer_if_expected", 
                                 bind(&AnaMuon::passes_hits_mcp_blayer_v16,_1)==true ,
                                 bind(&dg::fillh,_1,5,0,5,bind(&AnaMuon::nhits,_2,AnaMuon::BL)) );
  mu_lbl_common_selector.add_uu( "pix_hits", 
                                 bind(&AnaMuon::passes_hits_mcp_pix_v16,_1)==true ,
                                 bind(&dg::fillh,_1,5,0,5,bind(&AnaMuon::nhits,_2,AnaMuon::PIXEL)) );
  mu_lbl_common_selector.add_uu( "sct_hits", 
                                 bind(&AnaMuon::passes_hits_mcp_sct_v16,_1)==true ,
                                 bind(&dg::fillh,_1,5,0,5,bind(&AnaMuon::nhits,_2,AnaMuon::SCT)) );
  mu_lbl_common_selector.add_uu( "si_holes", 
                                 bind(&AnaMuon::passes_hits_mcp_holes_v16,_1)==true ,
                                 bind(&dg::fillh,_1,20,0,20,bind(&AnaMuon::nholes_silicon,_2)) );
  mu_lbl_common_selector.add_uu( "sitrt_hits", 
                                 bind(&AnaMuon::passes_hits_mcp_sitrt_v16,_1)==true ,
                                 bind(&dg::fillh,_1,60,0,60,bind(&AnaMuon::nhits,_2)) );
  mu_lbl_common_selector.add_uu( "qid_qmx", 
                                 bind(&AnaMuon::exms_charge,_1)==bind(&AnaMuon::id_charge,_1) ,
                                 bind(&dg::fillh,_1,2,0,2,bind(&AnaMuon::exms_charge,_2)==bind(&AnaMuon::id_charge,_2)) );
  mu_lbl_common_selector.add_uu( "from_beamline_z0sin" , bind( std::ptr_fun<AnaMuon::ftype,AnaMuon::ftype>(std::abs), bind(&AnaMuon::z0_exPV_sin , _1 )) < 5. ,
                                 bind( &dg::fillh , _1 , 160 , -40 , 40 , bind( &AnaMuon::z0_exPV_sin , _2 ) ) );
  // track isolation only for tight muons. loose muons are tight with
  // isolation criteria reversed.
  mu_lbl_tight_selector.based_on( mu_lbl_common_selector );
  mu_lbl_tight_selector.add_uu( "from_beamline_d0" , bind( &AnaMuon::abs_d0_exPV , _1 ) < 0.2 ,
                                bind( &dg::fillh , _1 , 100 , -2.5 , 2.5 , bind( &AnaMuon::d0_exPV , _2 ) ) );
  mu_lbl_tight_selector.add_uu( "from_beamline_d0_signif" , bind( &AnaMuon::abs_d0_exPV_signif , _1 ) < 3. ,
                                bind( &dg::fillh , _1 , 100 , -20 , 20 , bind( &AnaMuon::d0_exPV_signif , _2 ) ) );
  mu_lbl_tight_selector.add_uu( "pt_cone_40" , bind(&AnaMuon::pt_cone,_1,AnaMuon::CONE40) < 5. , 
                                bind(&dg::fillh, _1 , 100 , 0. , 50. , bind(&AnaMuon::pt_cone,_2,AnaMuon::CONE40) ) );
  mu_lbl_tight_selector.add_uu( "pt_cone_40_ratio" , bind(&AnaMuon::pt_cone_ratio,_1,AnaMuon::CONE40) < 0.1 , 
                                bind(&dg::fillh, _1 , 100 , 0. , 1. , bind(&AnaMuon::pt_cone_ratio,_2,AnaMuon::CONE40) ) );
  mu_lbl_loose_selector.based_on( mu_lbl_common_selector );
  mu_lbl_loose_selector.add_uu( "from_beamline_d0" , bind( &AnaMuon::abs_d0_exPV , _1 ) < 0.2 ,
                                bind( &dg::fillh , _1 , 100 , -2.5 , 2.5 , bind( &AnaMuon::d0_exPV , _2 ) ) );
  mu_lbl_loose_selector.add_uu( "from_beamline_d0_signif" , bind( &AnaMuon::abs_d0_exPV_signif , _1 ) < 3. ,
                                bind( &dg::fillh , _1 , 100 , -20 , 20 , bind( &AnaMuon::d0_exPV_signif , _2 ) ) );
  mu_lbl_loose_selector.add_uu( "pt_cone_40_or_ratio" , // loose means fail either ptcone or ptcone ratio pieces of tight selection, not that it fails both.
                                bind( std::logical_or<bool>() ,
                                      bind(&AnaMuon::pt_cone,_1,AnaMuon::CONE40) > 5. ,
                                      bind(&AnaMuon::pt_cone_ratio,_1,AnaMuon::CONE40) > 0.1) ,
                                //bind(&dg::fillh, _1 , 100 , 0. , 1. , bind(&AnaMuon::pt_cone_ratio,_2,AnaMuon::CONE40) ) );
                                bind(&dg::fillh, _1 , 100 , 0. , 50. , bind(&AnaMuon::pt_cone,_2,AnaMuon::CONE40) ) );
  mu_lbl_nonprompt_preiso_selector.based_on( mu_lbl_common_selector );
  mu_lbl_nonprompt_preiso_selector.add_uu( "from_beamline_d0_signif" , bind( &AnaMuon::abs_d0_exPV_signif , _1 ) > 5. ,
                                           bind( &dg::fillh , _1 , 100 , -20 , 20 , bind( &AnaMuon::d0_exPV_signif , _2 ) ) );
  mu_lbl_preiso_pred0sig_selector.based_on( mu_lbl_common_selector );
  mu_lbl_preiso_selector.based_on( mu_lbl_common_selector );
  mu_lbl_preiso_selector.add_uu( "from_beamline_d0" , bind( &AnaMuon::abs_d0_exPV , _1 ) < 0.2 ,
                                 bind( &dg::fillh , _1 , 100 , -2.5 , 2.5 , bind( &AnaMuon::d0_exPV , _2 ) ) );
  mu_lbl_preiso_selector.add_uu( "from_beamline_d0_signif" , bind( &AnaMuon::abs_d0_exPV_signif , _1 ) < 3. ,
                                 bind( &dg::fillh , _1 , 100 , -20 , 20 , bind( &AnaMuon::d0_exPV_signif , _2 ) ) );
  // low pt muons as defined similar to the SM WW observation
  EventObjectColl< shared_ptr<const AnaMuon> > mu_nontrigger_loose;
  DgCutSet< shared_ptr<const AnaMuon> > mu_nontrigger_loose_selector( "mu_nontrigger_loose_selector" , mu_nontrigger_loose.attach() );
  mu_nontrigger_loose_selector.add_uu( "is_staco_loose" , bind(&AnaMuon::is_staco_any,_1)==true && bind(&AnaMuon::quality_loose,_1)==true , 
                                       bind( &dg::fillh , _1 , 48 , -M_PI , M_PI , bind(&AnaMuon::phi,_2) , string("#phi") ) );
  mu_nontrigger_loose_selector.add_uu( "pt" , bind(&AnaMuon::pt,_1)>=10. , 
                                       bind(&dg::fillh, _1 , 100 , 0 , 200 , bind(&AnaMuon::pt,_2) ) );
  mu_nontrigger_loose_selector.add_uu( "is_fiducial_24" , bind(&AnaMuon::abseta,_1)<2.4 , bind( &dg::fillh , _1 , 40 , -4 , 4 , bind(&AnaMuon::abseta,_2) ) );
  mu_nontrigger_loose_selector.add_uu( "b_layer_if_expected", 
                                       bind(&AnaMuon::passes_hits_mcp_blayer_v16,_1)==true ,
                                       bind(&dg::fillh,_1,5,0,5,bind(&AnaMuon::nhits,_2,AnaMuon::BL)) );
  mu_nontrigger_loose_selector.add_uu( "si_holes", 
                                       bind(&AnaMuon::passes_hits_mcp_holes_v16,_1)==true ,
                                       bind(&dg::fillh,_1,20,0,20,bind(&AnaMuon::nholes_silicon,_2)) );
  mu_nontrigger_loose_selector.add_uu( "sitrt_hits", 
                                       bind(&AnaMuon::passes_hits_mcp_sitrt_v16,_1)==true ,
                                       bind(&dg::fillh,_1,60,0,60,bind(&AnaMuon::nhits,_2)) );
  mu_nontrigger_loose_selector.add_uu( "from_beamline_z0" , bind( &AnaMuon::exPV_z0 , _1 ) < 10. ,
                                       bind( &dg::fillh , _1 , 160 , -40 , 40 , bind( &AnaMuon::exPV_z0 , _2 ) ) );
  mu_nontrigger_loose_selector.add_uu( "from_beamline_d0_signif" , bind( &AnaMuon::exPV_abs_d0_signif , _1 ) < 10 ,
                                       bind( &dg::fillh , _1 , 100 , -20 , 20 , bind( &AnaMuon::exPV_d0_signif , _2 ) ) );
  mu_nontrigger_loose_selector.add_uu( "pt_cone_20_ratio" , bind(&AnaMuon::pt_cone_ratio,_1,AnaMuon::CONE20) < 0.1 , 
                                       bind(&dg::fillh, _1 , 100 , 0. , 1. , bind(&AnaMuon::pt_cone_ratio,_2,AnaMuon::CONE20) ) );
  mu_nontrigger_loose_selector.add_uu( "et_cone_20_ratio" , bind(&AnaMuon::et_cone_ratio,_1,AnaMuon::CONE20) < 0.15 , 
                                       bind(&dg::fillh, _1 , 100 , 0. , 1. , bind(&AnaMuon::et_cone_ratio,_2,AnaMuon::CONE20) ) );
  // low pt muons as defined similar to the SM WW observation
  EventObjectColl< shared_ptr<const AnaMuon> > mu_tag_lowmh;
  DgCutSet< shared_ptr<const AnaMuon> > mu_tag_lowmh_selector( "mu_tag_lowmh_selector" , mu_tag_lowmh.attach() );
  mu_tag_lowmh_selector.add_u( "is_combined_or_tag" , bind(&AnaMuon::is_staco_combined,_1)==true || bind(&AnaMuon::is_mutag,_1)==true );
  mu_tag_lowmh_selector.add_uu( "pt" , bind(&AnaMuon::pt,_1)>=10 , 
                                bind(&dg::fillh, _1 , 100 , 0 , 200 , bind(&AnaMuon::pt,_2) ) );
  mu_tag_lowmh_selector.add_uu( "ptms" , bind(&AnaMuon::exms_pt,_1) > 10. || bind(&AnaMuon::is_mutag,_1)==true, 
                                bind(&dg::fillh, _1 , 100 , 0 , 100 , bind(&AnaMuon::exms_pt,_2) ) );
  mu_tag_lowmh_selector.add_u( "ptDiffRatio", 
                               bind( std::ptr_fun<AnaMuon::ftype,AnaMuon::ftype>(std::abs) , bind(&AnaMuon::pt_diff_ratio,_1)) < 0.5 || bind(&AnaMuon::is_mutag,_1)==true );
  mu_tag_lowmh_selector.add_u( "trackerHits", 
                               ( bind( std::ptr_fun<AnaMuon::ftype,AnaMuon::ftype>(std::abs) , 
                                       bind(&AnaMuon::eta,_1)) > 2.0 || 
                                 ( bind(&AnaMuon::nhits,_1,AnaMuon::PIXEL) > 1 &&
                                   bind(&AnaMuon::nhits,_1,AnaMuon::SCT) > 5 &&
                                   bind(&AnaMuon::nhits,_1,AnaMuon::TRT) > 0 ) ) );
  mu_tag_lowmh_selector.add_uu( "is_fiducial_24" , bind(&AnaMuon::abseta,_1)<2.4 , bind( &dg::fillh , _1 , 40 , -4 , 4 , bind(&AnaMuon::abseta,_2) ) );
  mu_tag_lowmh_selector.add_uu( "from_beamline_z0" , bind( &AnaMuon::exPV_z0 , _1 ) < 10. ,
                                bind( &dg::fillh , _1 , 160 , -40 , 40 , bind( &AnaMuon::exPV_z0 , _2 ) ) );
  mu_tag_lowmh_selector.add_uu( "from_beamline_d0_signif" , bind( &AnaMuon::exPV_abs_d0_signif , _1 ) < 10 ,
                                bind( &dg::fillh , _1 , 100 , -20 , 20 , bind( &AnaMuon::exPV_d0_signif , _2 ) ) );
  mu_tag_lowmh_selector.add_uu( "pt_cone_20_ratio" , bind(&AnaMuon::pt_cone_ratio,_1,AnaMuon::CONE20) < 0.1 , 
                                bind(&dg::fillh, _1 , 100 , 0. , 1. , bind(&AnaMuon::pt_cone_ratio,_2,AnaMuon::CONE20) ) );
  // anti-selected muons (fakes)
  EventObjectColl< shared_ptr<const AnaMuon> > mu_anti;
  DgCutSet< shared_ptr<const AnaMuon> > mu_anti_selector( "mu_anti_selector" , mu_anti.attach() );
  mu_anti_selector.add_uu( "pt" , bind(&AnaMuon::pt,_1)>=10 , 
                           bind(&dg::fillh, _1 , 100 , 0 , 200 , bind(&AnaMuon::pt,_2) ) );
  mu_anti_selector.add_uu( "ptms" , bind(&AnaMuon::exms_pt,_1) > 10. || bind(&AnaMuon::is_mutag,_1)==true, 
                           bind(&dg::fillh, _1 , 100 , 0 , 100 , bind(&AnaMuon::exms_pt,_2) ) );
  // mu_anti_selector.add_u( "ptDiffRatio", 
  //                         bind( std::ptr_fun<AnaMuon::ftype,AnaMuon::ftype>(std::abs) , bind(&AnaMuon::pt_diff_ratio,_1)) < 0.5 );
  mu_anti_selector.add_u( "trackerHits", 
                          ( bind( std::ptr_fun<AnaMuon::ftype,AnaMuon::ftype>(std::abs) , 
                                  bind(&AnaMuon::eta,_1)) > 2.0 || 
                            ( bind(&AnaMuon::nhits,_1,AnaMuon::PIXEL) > 1 &&
                              bind(&AnaMuon::nhits,_1,AnaMuon::SCT) > 5 &&
                              bind(&AnaMuon::nhits,_1,AnaMuon::TRT) > 0 ) ) );
  mu_anti_selector.add_uu( "is_fiducial_24" , bind(&AnaMuon::abseta,_1)<2.4 , bind( &dg::fillh , _1 , 40 , -4 , 4 , bind(&AnaMuon::abseta,_2) ) );
  mu_anti_selector.add_uu( "from_beamline_z0" , bind( &AnaMuon::exPV_z0 , _1 ) < 10. ,
                           bind( &dg::fillh , _1 , 160 , -40 , 40 , bind( &AnaMuon::exPV_z0 , _2 ) ) );
  mu_anti_selector.add_uu( "from_beamline_d0_signif" , bind( &AnaMuon::exPV_abs_d0_signif , _1 ) < 50 , // relax cut to allow more decay-in-flight
                           bind( &dg::fillh , _1 , 100 , -100 , 100 , bind( &AnaMuon::exPV_d0_signif , _2 ) ) );
  mu_anti_selector.add_uu( "fails_pt_cone_20_ratio" , bind(&AnaMuon::pt_cone_ratio,_1,AnaMuon::CONE20) > 0.1 , 
                           bind(&dg::fillh, _1 , 100 , 0. , 1. , bind(&AnaMuon::pt_cone_ratio,_2,AnaMuon::CONE20) ) );
  
  // combined lepton collections
  // ----------------------------------------------------------------
  EventObjectColl< shared_ptr<const AnaParticle> > lepton_probe;
  EventObjectColl< shared_ptr<const AnaParticle> > lepton_expanded_probe;
  // EventObjectColl< shared_ptr<const AnaParticle> > lepton_quayleww;
  EventObjectColl< shared_ptr<const AnaParticle> > lepton_hww;
  EventObjectColl< shared_ptr<const AnaParticle> > lepton_hww_lowpt;
  EventObjectColl< shared_ptr<const AnaParticle> > lepton_lowmh;
  EventObjectColl< shared_ptr<const AnaParticle> > lepton_anti;

  
  // photon selection
  // ================================================================
  EventObjectColl<  boost::shared_ptr<const AnaPhoton>  > loose_photon;
  DgCutSet< boost::shared_ptr<const AnaPhoton> > loose_photon_selector( "loose_photon_selector" , loose_photon.attach() );
  loose_photon_selector.add_u( "loose" , bind( &AnaPhoton::aod_loose , _1 ) );
  loose_photon_selector.add_uu( "cluster_ET" , bind(&AnaPhoton::cluster_pt,_1) > 10. , bind(&dg::fillh,_1,100,0,100,bind(&AnaPhoton::cluster_pt,_2)) );

  EventObjectColl<  boost::shared_ptr<const AnaPhoton>  > tight_photon;
  DgCutSet< boost::shared_ptr<const AnaPhoton> > tight_photon_selector( "tight_photon_selector" , tight_photon.attach() );
  tight_photon_selector.add_u( "tight" , bind( &AnaPhoton::aod_tight , _1 ) );
  tight_photon_selector.add_uu( "cluster_ET" , bind(&AnaPhoton::cluster_pt,_1) > 10. , bind(&dg::fillh,_1,100,0,100,bind(&AnaPhoton::cluster_pt,_2)) );

  // jet selection
  // ================================================================
  EventObjectColl< shared_ptr<const AnaJet> > jet_FEB_withoverlap;
  EventObjectColl< shared_ptr<const AnaJet> > jet_FEB;
  EventObjectColl< shared_ptr<const AnaJet> > jet_hww_cleanup_withoverlap;
  EventObjectColl< shared_ptr<const AnaJet> > jet_hww_cleanup;
  EventObjectColl< shared_ptr<const AnaJet> > jet_hww_withoverlap;
  EventObjectColl< shared_ptr<const AnaJet> > jet_hww;
  EventObjectColl< shared_ptr<const AnaJet> > jet_hww_lowpt_withoverlap;
  EventObjectColl< shared_ptr<const AnaJet> > jet_hww_lowpt;
  DgCutSet< shared_ptr<const AnaJet> > jet_FEB_withoverlap_selector( "jet_FEB_withoverlap_selector" , jet_FEB_withoverlap.attach() );
  jet_FEB_withoverlap_selector.add_u( "algorithm_antikt4" , bind(&AnaJet::is_atlas_anti_kt_cone_4_h1_topo,_1)==true );
  DgCutSet< shared_ptr<const AnaJet> > jet_hww_withoverlap_selector( "jet_hww_withoverlap_selector" , jet_hww_withoverlap.attach() );
  jet_hww_withoverlap_selector.add_u( "algorithm_antikt4" , bind(&AnaJet::is_atlas_anti_kt_cone_4_h1_topo,_1)==true );
  // jet_hww_cleanup_withoverlap_selector.add_u( "is_good" , bind(&AnaJet::is_good,_1)==true );
  //  jet_hww_withoverlap_selector.add_uu( "pt_25" ,  bind(&AnaJet::pt,_1) > 25. , bind(&dg::fillh,_1,100,0,100,bind(&AnaJet::pt,_2)) );
  jet_hww_withoverlap_selector.add_uu( "pt_25" , 
                                       bind(&TLorentzVector::Pt, bind(&AnaJet::full_scale_momentum,_1)) > 25. ,
                                       bind(&dg::fillh,_1,100,0,100,bind(&AnaJet::pt,_2)));
  jet_hww_withoverlap_selector.add_uu( "fiducial_45" , 
                                       bind(std::ptr_fun<AnaJet::ftype,AnaJet::ftype>(std::abs),
                                            bind(&AnaJet::eta, _1)) < 4.5 ,
                                       bind(&dg::fillh,_1,100,-3,3, 
                                            bind(&AnaJet::eta, _2)) );
  DgCutSet< shared_ptr<const AnaJet> > jet_hww_lowpt_withoverlap_selector( "jet_hww_lowpt_withoverlap_selector" , jet_hww_lowpt_withoverlap.attach() );
  jet_hww_lowpt_withoverlap_selector.based_on( jet_hww_withoverlap_selector );
  // met cleanup jet definition
  DgCutSet< shared_ptr<const AnaJet> > jet_hww_cleanup_withoverlap_selector( "jet_hww_cleanup_withoverlap_selector" , jet_hww_cleanup_withoverlap.attach() );
  jet_hww_cleanup_withoverlap_selector.add_u( "algorithm_antikt4" , bind(&AnaJet::is_atlas_anti_kt_cone_4_h1_topo,_1)==true );
  jet_hww_cleanup_withoverlap_selector.add_uu( "fiducial_45" , 
                                               bind(std::ptr_fun<AnaJet::ftype,AnaJet::ftype>(std::abs),
                                                    bind(&TLorentzVector::Eta, bind(&AnaJet::em_scale_momentum,_1))) < 4.5 ,
                                               bind(&dg::fillh,_1,100,-3,3, 
                                                    bind(&TLorentzVector::Eta, bind(&AnaJet::em_scale_momentum,_2))) );
  jet_hww_cleanup_withoverlap_selector.add_uu( "pt_25" , bind(&AnaJet::pt,_1) > 25. , bind(&dg::fillh,_1,100,0,100,bind(&AnaJet::pt,_2)) );
  jet_hww_cleanup_withoverlap_selector.add_uu( "is_not_medium_bad" ,
                                               bind(&AnaJet::isBadJet_medium_v16,_1)==false || bind(&TLorentzVector::Pt,bind(&AnaJet::em_scale_momentum,_1))<20. ,
                                               bind(&dg::fillh,_1,100,-4,4,bind(&TLorentzVector::Eta, bind(&AnaJet::em_scale_momentum,_2))) );
  jet_hww_cleanup_withoverlap_selector.add_uu( "is_not_ugly" ,
                                               bind(&AnaJet::is_ugly,_1)==false || bind(&TLorentzVector::Pt,bind(&AnaJet::em_scale_momentum,_1))<20. ,
                                               bind(&dg::fillh,_1,100,-4,4,bind(&TLorentzVector::Eta, bind(&AnaJet::em_scale_momentum,_2))) );
  // vertex fraction>=0.7 for jets with eta<2.5, elif jets eta>2.1 accept jet
  jet_hww_cleanup_withoverlap_selector.add_uu("vertex_fraction_for_jets_eta_2",
                                              ( bind(std::ptr_fun<AnaJet::ftype,AnaJet::ftype>(std::abs), bind(&AnaJet::vtx_fraction,_1)) >= 0.75 && 
                                                bind(std::ptr_fun<AnaJet::ftype,AnaJet::ftype>(std::abs),bind(&TLorentzVector::Eta, bind(&AnaJet::em_scale_momentum,_1))) < 2.1 )  ||
                                              bind(std::ptr_fun<AnaJet::ftype,AnaJet::ftype>(std::abs),bind(&TLorentzVector::Eta, bind(&AnaJet::em_scale_momentum,_1))) >= 2.1,
                                              bind(&dg::fillh,_1,100,-1,1, bind(&AnaJet::vtx_fraction,_2)));
  // MET selection
  // ================================================================
  // source MET collection. MET is recalculated for |eta|<4.5 towers
  // and the muon corrections applied to staco muons.
  EventObjectColl< shared_ptr<const AnaMET> > met_hww_event;
  EventObjectColl< shared_ptr<const AnaMET> > met_hww;   // selected METs
  EventObjectColl< shared_ptr<const AnaMET> > met_hww_lowpt_event;
  EventObjectColl< shared_ptr<const AnaMET> > met_hww_lowpt;   // selected METs
  DgCutSet< shared_ptr<const AnaMET> > met_hww_selector( "met_hww_selector" , met_hww.attach() );
  StudyMET study_met_wrt_jet_hww_withoverlap( "wjets_met_wrt_jet_hww_withoverlap" , "study wjets MET wrt HWW jets" );
  met_hww_selector.do_extra_study( study_met_wrt_jet_hww_withoverlap );
  // met_hww_selector.add_u( "met_type" , bind(&AnaMET::met_type,_1) == AnaMET::REF_FINAL_EM ); // v16 2011 MET requirement
  met_hww_selector.add_u( "met_type" , bind(&AnaMET::met_type,_1) == AnaMET::LOCHADTOPOCEF ); // hww PLHC 2011
  DgCutSet< shared_ptr<const AnaMET> > met_hww_lowpt_selector( "met_hww_lowpt_selector" , met_hww_lowpt.attach() );
  // met_hww_selector.add_u( "met_type" , bind(&AnaMET::met_type,_1) == AnaMET::REF_FINAL_EM ); // v16 2011 MET requirement
  met_hww_lowpt_selector.add_u( "met_type" , bind(&AnaMET::met_type,_1) == AnaMET::LOCHADTOPOCEF ); // hww PLHC 2011

  // W boson selection
  EventObjectColl< shared_ptr<const AnaW> > w_enu_hww;
  EventObjectColl< shared_ptr<const AnaW> > w_munu_hww;
  EventObjectColl< shared_ptr<const AnaW> > w_lnu_hww;
  EventObjectColl< shared_ptr<const AnaW> > w_munu_hww_preiso;
  EventObjectColl< shared_ptr<const AnaW> > w_munu_hww_lowpt_preiso;
  EventObjectColl< shared_ptr<const AnaW> > w_jj_closestmw_hww;
  //
  // DgCutSet< shared_ptr<const AnaW> > w_enu_quayleww_selector( "w_enu_quayleww" , w_enu_quayleww.attach() );
  // DgCutSet< shared_ptr<const AnaW> > w_munu_quayleww_selector( "w_munu_quayleww" , w_munu_quayleww.attach() );
  // w_enu_quayleww_selector.add_uu( "met" ,
  //                                 bind(&AnaParticle::pt,bind(&AnaW::met,_1)) > 25. ,
  //                                 bind(dg::fillh,_1,150,0,150,bind(&AnaParticle::pt,bind(&AnaW::met,_2))) );
  // w_enu_quayleww_selector.add_uu( "transverse_mass" , bind( &AnaW::transverse_mass , _1 ) > 40.,
  //                                 bind(&dg::fillh,_1,100,0,100, bind(&AnaW::transverse_mass , _2)) );
  // w_munu_quayleww_selector.based_on( w_enu_quayleww_selector ); // identical selection, different input and output lists
  //
  DgCutSet< shared_ptr<const AnaW> > w_enu_hww_selector( "w_enu_hww" , w_enu_hww.attach() );
  DgCutSet< shared_ptr<const AnaW> > w_munu_hww_selector( "w_munu_hww" , w_munu_hww.attach() );
  w_enu_hww_selector.add_uu( "met" ,
                              bind(&AnaParticle::pt,bind(&AnaW::met,_1)) > 25. ,
                              bind(dg::fillh,_1,150,0,150,bind(&AnaParticle::pt,bind(&AnaW::met,_2))) );
  w_enu_hww_selector.add_uu( "transverse_mass" , bind( &AnaW::transverse_mass , _1 ) > 40.,
                              bind(&dg::fillh,_1,100,0,100, bind(&AnaW::transverse_mass , _2)) );
  w_munu_hww_selector.based_on( w_enu_hww_selector ); // identical selection, different input and output lists

  // ----------------------------------------------------------------
  // low mass higgs selection
  // ----------------------------------------------------------------
  EventObjectColl< shared_ptr<const AnaHiggs> > hww_expanded_probe_input;
  EventObjectColl< shared_ptr<const AnaHiggs> > hww_tight_probe_input;
  EventObjectColl< shared_ptr<const AnaHiggs> > hww_tight_expanded_probe_input;
  EventObjectColl< shared_ptr<const AnaHiggs> > hww_input;
  EventObjectColl< shared_ptr<const AnaHiggs> > hww_lowmh_input;
  EventObjectColl< shared_ptr<const AnaHiggs> > hww_probe_peter;
  EventObjectColl< shared_ptr<const AnaHiggs> > hww_expanded_probe_peter;
  EventObjectColl< shared_ptr<const AnaHiggs> > hww_peter;
  EventObjectColl< shared_ptr<const AnaHiggs> > hww_lowmh_peter;
  EventObjectColl< shared_ptr<const AnaHiggs> > hww_tight_probe_peter;
  EventObjectColl< shared_ptr<const AnaHiggs> > hww_tight_expanded_probe_peter;
  EventObjectColl< shared_ptr<const AnaHiggs> > hww_expanded_probe_hsg3;
  EventObjectColl< shared_ptr<const AnaHiggs> > hww;
  EventObjectColl< shared_ptr<const AnaHiggs> > hww_lowmh;
  DgCutSet< shared_ptr<const AnaHiggs> > hww_selector( "hww_selector" , hww.attach() );
  hww_selector.add_uu( "leading_eta" , 
                                 bind( &AnaParticle::abseta , bind( &AnaHiggs::dilepton_leading_lepton,_1) ) < 2.5 , 
                                 bind( &dg::fillh , _1 , 80 , -4 , 4 , bind( &AnaParticle::eta , bind(&AnaHiggs::dilepton_leading_lepton,_2) ) , "leading lepton #eta" ) );
  hww_selector.add_uu( "subleading_eta" , 
                                 bind( &AnaParticle::abseta , bind( &AnaHiggs::dilepton_subleading_lepton,_1) ) < 2.5 , 
                                 bind( &dg::fillh , _1 , 80 , -4 , 4 , bind( &AnaParticle::eta , bind(&AnaHiggs::dilepton_subleading_lepton,_2) ) , "subleading lepton #eta" ) );
  hww_selector.add_uu( "mll" , 
                                 bind( &AnaParticle::mass , bind(&AnaHiggs::dilepton_pair,_1) ) > 15. && bind( &AnaParticle::mass , bind(&AnaHiggs::dilepton_pair,_1) ) < 50. ,
                                 bind( &dg::fillh , _1 , 100 , 0 , 100 , bind( &AnaParticle::mass , bind(&AnaHiggs::dilepton_pair,_2) ) , "dilepton mass (GeV)" ) );
  hww_selector.add_uu( "met" , 
                                 bind( &AnaParticle::pt , bind(&AnaHiggs::dilepton_met,_1) ) > 30. ,
                                 bind( &dg::fillh , _1 , 100 , 0 , 100 , bind( &AnaParticle::pt , bind(&AnaHiggs::dilepton_met,_2) ) , "MET (GeV)" ) );
  hww_selector.add_uu( "ptll" , 
                                 bind( &AnaParticle::pt , bind(&AnaHiggs::dilepton_pair,_1) ) > 30. ,
                                 bind( &dg::fillh , _1 , 100 , 0 , 100 , bind( &AnaParticle::pt , bind(&AnaHiggs::dilepton_pair,_2) ) , "dilepton system p_{T} (GeV)" ) );
  hww_selector.add_uu( "dphill" , 
                                 bind( std::ptr_fun<AnaParticle::ftype,AnaParticle::ftype>(std::abs) , bind( &AnaHiggs::dilepton_dphi,_1) ) < 1.3 ,
                                 bind( &dg::fillh , _1 , 96 , -M_PI , M_PI , bind( &AnaHiggs::dilepton_dphi,_2) , "#Delta #phi between leptons (rad)" ) );
  hww_selector.add_uu( "leading_pt" , 
                                 bind( &AnaParticle::pt , bind(&AnaHiggs::dilepton_leading_lepton,_1) ) > 20. ,
                                 bind( &dg::fillh , _1 , 100 , 0 , 100 , bind( &AnaParticle::pt , bind(&AnaHiggs::dilepton_leading_lepton,_2) ) , "p_{T} (GeV)" ) );
  hww_selector.add_uu( "subleading_pt" , 
                                 bind( &AnaParticle::pt , bind(&AnaHiggs::dilepton_subleading_lepton,_1) ) > 15. ,
                                 bind( &dg::fillh , _1 , 100 , 0 , 100 , bind( &AnaParticle::pt , bind(&AnaHiggs::dilepton_subleading_lepton,_2) ) , "p_{T} (GeV)" ) );

  DgCutSet< shared_ptr<const AnaHiggs> > hww_peter_selector( "hww_peter_selector" , hww_peter.attach() );
  hww_peter_selector.add_uu( "leading_eta" , 
                                  bind( &AnaParticle::abseta , bind( &AnaHiggs::dilepton_leading_lepton,_1) ) < 2.5 , 
                                  bind( &dg::fillh , _1 , 80 , -4 , 4 , bind( &AnaParticle::eta , bind(&AnaHiggs::dilepton_leading_lepton,_2) ) , "leading lepton #eta" ) );
  hww_peter_selector.add_uu( "subleading_eta" , 
                                  bind( &AnaParticle::abseta , bind( &AnaHiggs::dilepton_subleading_lepton,_1) ) < 2.5 , 
                                  bind( &dg::fillh , _1 , 80 , -4 , 4 , bind( &AnaParticle::eta , bind(&AnaHiggs::dilepton_subleading_lepton,_2) ) , "subleading lepton #eta" ) );
  hww_peter_selector.add_uu( "mll" , 
                                  bind( &AnaParticle::mass , bind(&AnaHiggs::dilepton_pair,_1) ) > 11. && bind( &AnaParticle::mass , bind(&AnaHiggs::dilepton_pair,_1) ) < 50. ,
                                  bind( &dg::fillh , _1 , 100 , 0 , 100 , bind( &AnaParticle::mass , bind(&AnaHiggs::dilepton_pair,_2) ) , "dilepton mass (GeV)" ) );
  hww_peter_selector.add_uu( "met" , 
                                  bind( &AnaParticle::pt , bind(&AnaHiggs::dilepton_met,_1) ) > 30. ,
                                  bind( &dg::fillh , _1 , 100 , 0 , 100 , bind( &AnaParticle::pt , bind(&AnaHiggs::dilepton_met,_2) ) , "MET (GeV)" ) );
  hww_peter_selector.add_uu( "ptll" , 
                                  bind( &AnaParticle::pt , bind(&AnaHiggs::dilepton_pair,_1) ) > 20. ,
                                  bind( &dg::fillh , _1 , 100 , 0 , 100 , bind( &AnaParticle::pt , bind(&AnaHiggs::dilepton_pair,_2) ) , "dilepton system p_{T} (GeV)" ) );
  hww_peter_selector.add_uu( "dphill" , 
                                  bind( std::ptr_fun<AnaParticle::ftype,AnaParticle::ftype>(std::abs) , bind( &AnaHiggs::dilepton_dphi,_1) ) < 1.3 ,
                                  bind( &dg::fillh , _1 , 96 , -M_PI , M_PI , bind( &AnaHiggs::dilepton_dphi,_2) , "#Delta #phi between leptons (rad)" ) );
  hww_peter_selector.add_uu( "leading_pt" , 
                                  bind( &AnaParticle::pt , bind(&AnaHiggs::dilepton_leading_lepton,_1) ) > 20. ,
                                  bind( &dg::fillh , _1 , 100 , 0 , 100 , bind( &AnaParticle::pt , bind(&AnaHiggs::dilepton_leading_lepton,_2) ) , "p_{T} (GeV)" ) );
  hww_peter_selector.add_uu( "subleading_pt" , 
                                  bind( &AnaParticle::pt , bind(&AnaHiggs::dilepton_subleading_lepton,_1) ) > 10. ,
                                  bind( &dg::fillh , _1 , 100 , 0 , 100 , bind( &AnaParticle::pt , bind(&AnaHiggs::dilepton_subleading_lepton,_2) ) , "p_{T} (GeV)" ) );

  // hww_peter_selector.add_uu( "hmt" , 
  //                  bind( &AnaParticle::transverse_mass , _1 ) > 0.75*mhiggs && bind( &AnaParticle::transverse_mass , _1 ) <= 1.*mhiggs ,
  //                  bind( &dg::fillh , _1 , 100 , 0 , 100 , bind( &AnaParticle::transverse_mass,_2) , "higgs transverse mass (GeV)" ) );

  DgCutSet< shared_ptr<const AnaHiggs> > hww_lowmh_selector( "hww_lowmh_selector" , hww_lowmh.attach() );
  hww_lowmh_selector.based_on( hww_selector );
  DgCutSet< shared_ptr<const AnaHiggs> > hww_lowmh_peter_selector( "hww_lowmh_peter_selector" , hww_lowmh_peter.attach() );
  hww_lowmh_peter_selector.based_on( hww_peter_selector );
  DgCutSet< shared_ptr<const AnaHiggs> > hww_probe_peter_selector( "hww_probe_peter_selector" , hww_probe_peter.attach() );
  hww_probe_peter_selector.based_on( hww_peter_selector );
  DgCutSet< shared_ptr<const AnaHiggs> > hww_expanded_probe_peter_selector( "hww_expanded_probe_peter_selector" , hww_expanded_probe_peter.attach() );
  hww_expanded_probe_peter_selector.based_on( hww_peter_selector );

  DgCutSet< shared_ptr<const AnaHiggs> > hww_tight_probe_peter_selector( "hww_tight_probe_peter_selector" , hww_tight_probe_peter.attach() );
  hww_tight_probe_peter_selector.based_on( hww_peter_selector );
  DgCutSet< shared_ptr<const AnaHiggs> > hww_tight_expanded_probe_peter_selector( "hww_tight_expanded_probe_peter_selector" , hww_tight_expanded_probe_peter.attach() );
  hww_tight_expanded_probe_peter_selector.based_on( hww_peter_selector );
  

  //
  // Z boson selection
  // ----------------------------------------------------------------
  //   input lists
  EventObjectColl< shared_ptr<const AnaZ> > ee_hww;
  EventObjectColl< shared_ptr<const AnaZ> > mumu_hww;
  EventObjectColl< shared_ptr<const AnaZ> > mumu_hww_preiso;
  EventObjectColl< shared_ptr<const AnaZ> > mumu_hww_lowpt;
  EventObjectColl< shared_ptr<const AnaZ> > mumu_hww_lowpt_preiso;
  EventObjectColl< shared_ptr<const AnaZ> > ee_lowmh;
  EventObjectColl< shared_ptr<const AnaZ> > mumu_nontrigger_loose;
  EventObjectColl< shared_ptr<const AnaZ> > ee_probe;
  EventObjectColl< shared_ptr<const AnaZ> > mumu_probe_staco;
  EventObjectColl< shared_ptr<const AnaZ> > mumu_probe_muid;
  EventObjectColl< shared_ptr<const AnaZ> > ee_tagprobe;
  EventObjectColl< shared_ptr<const AnaZ> > mumu_tagprobe;
  EventObjectColl< shared_ptr<const AnaZ> > ee_tagprobe_expanded;
  EventObjectColl< shared_ptr<const AnaZ> > mumu_tagprobe_expanded;
  // anti-selected lepton pairs
  EventObjectColl< shared_ptr<const AnaZ> > ee_photonprobe;
  EventObjectColl< shared_ptr<const AnaZ> > ee_taganti;
  EventObjectColl< shared_ptr<const AnaZ> > mumu_taganti;
  // LBL fake rate dilepton pairs
  EventObjectColl< shared_ptr<const AnaComposite> > mumu_lbl_preiso_ss;
  EventObjectColl< shared_ptr<const AnaComposite> > mumu_lbl_preiso_2mu10_ss;
  EventObjectColl< shared_ptr<const AnaComposite> > mumu_lbl_preiso_mu6_ss;
  EventObjectColl< shared_ptr<const AnaComposite> > mumu_lbl_preiso_mu18or40_ss;
  EventObjectColl< shared_ptr<const AnaComposite> > mumu_lbl_preiso_os;
  EventObjectColl< shared_ptr<const AnaComposite> > mumu_lbl_preiso_2mu10_os;
  EventObjectColl< shared_ptr<const AnaComposite> > mumu_lbl_preiso_mu6_os;
  EventObjectColl< shared_ptr<const AnaComposite> > mumu_lbl_preiso_mu18or40_os;
  EventObjectColl< shared_ptr<const AnaComposite> > mumu_lbl_preiso_as;
  EventObjectColl< shared_ptr<const AnaComposite> > mumu_lbl_preiso_2mu10_as;
  EventObjectColl< shared_ptr<const AnaComposite> > mumu_lbl_preiso_mu6_as;
  EventObjectColl< shared_ptr<const AnaComposite> > mumu_lbl_preiso_mu18or40_as;
  // 
  // W boson selection
  // ----------------------------------------------------------------
  // input lists
  EventObjectColl< shared_ptr<const AnaW> > enu_hww;
  EventObjectColl< shared_ptr<const AnaW> > munu_hww;
  EventObjectColl< shared_ptr<const AnaW> > munu_hww_preiso;
  EventObjectColl< shared_ptr<const AnaW> > munu_hww_lowpt_preiso;
  EventObjectColl< shared_ptr<const AnaW> > enu_probe;
  EventObjectColl< shared_ptr<const AnaW> > munu_probe;


  //
  // event loop
  // ================================================================
  for( AnaEventMgr::iterator i=evtmgr->begin(), f=evtmgr->end(AnaConfiguration::user_max_nevents()); i!=f; ++i ) {

    // load the event data from disk into an "AnaEvent" object.
    const shared_ptr<const AnaEvent>& evt( *i );
    if( !evt ) { break; } // no AnaEvent means something went wrong, e.g. premature end of file

    // make trigger and good run requirements
    // ----------------------------------------------------------------
    AnaEvent::tribool is_mc = evt->check_flag( "mc" );
    const bool do_not_require_good_run = is_mc;
    // check good run list
    const bool is_good_run_ele = do_not_require_good_run ? true : grl.is_in( GoodRunList::ELECTRON , evt->run_number() , evt->lumi_block() );
    const bool is_good_run_mu = do_not_require_good_run ? true : grl.is_in( GoodRunList::MUON , evt->run_number() , evt->lumi_block() );
    // cout << " run " << evt->run_number() << " lumi " << evt->lumi_block() << " is in GRL? " << grl.is_in( GoodRunList::ELECTRON , evt->run_number() , evt->lumi_block() ) << endl;

    float evtweight = 1.;
    float outweight = 1.;
    float mcWeight = 1.;
    float pileupWeight = 1.;
    if( true ) { 
      dg::set_global_weight( 1. ); // reset weight to "no weight"
      {
        dg::down( "plhc_hsg3_cutflow" , "selector details" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
        dg::filleff( "c0_all" , 1 );
        dg::set_global_weight( evt->mc_weight() );
        if( is_mc ) {
          dg::filleff( "c05_mc", 1 );
        }
      }
    }

    // set event pileup weight, if mc
    if( true ) { 
      dg::set_global_weight( 1. ); // reset weight to "no weight"
      static boost::shared_ptr<Root::TPileupReweighting> pw( new Root::TPileupReweighting( "pw" ) );
      static enum { PU_NONE , PU_MC10A , PU_MC10B } pileup_reweighting_mode;
      {
        // figure out correct pileup reweighting mode for this event.
        if( evt->check_tag( "mc10a" ) && pileup_reweighting_mode!=PU_MC10A ) {
          pw.reset( new Root::TPileupReweighting( "pw" ) );
          pw->initialize( "./CommonHiggs/pileup/ilumicalc_histograms_EF_mu18_MG_178044-184072.root" , "avgintperbx" ,
                          "./CommonHiggs/pileup/mu_mc10a.root" , "mu_mc10a" );
          // pw->initialize( "./CommonHiggs/pileup/ilumicalc_histograms_EF_mu20_MG_178044-182486.root" , "avgintperbx" ,
          //                 "./CommonHiggs/pileup/mu_mc10a.root" , "mu_mc10a" );
          pileup_reweighting_mode=PU_MC10A;
        } else if( evt->check_tag( "mc10b" ) && pileup_reweighting_mode!=PU_MC10B ) {
          pw.reset( new Root::TPileupReweighting( "pw" ) );
          pw->initialize( "./CommonHiggs/pileup/ilumicalc_histograms_EF_mu18_MG_178044-184072.root" , "avgintperbx" ,
                          "./CommonHiggs/pileup/mu_mc10b.root" , "mu_mc10b" );
          // pw->initialize( "./CommonHiggs/pileup/ilumicalc_histograms_EF_mu20_MG_178044-182486.root" , "avgintperbx" ,
          //                 "./CommonHiggs/pileup/mu_mc10b.root" , "mu_mc10b" );
          pileup_reweighting_mode=PU_MC10B;
        } else if( !evt->check_tag( "mc10a" ) && !evt->check_tag( "mc10b" ) && pileup_reweighting_mode!=PU_NONE ) {
          pw.reset();
          pileup_reweighting_mode=PU_NONE;
        }
        // histogram current pileup mode
        dg::down( "pileup_reweighting" , "pileup reweighting details" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
        dg::filleff( "pu_mc10a"  , pileup_reweighting_mode==PU_MC10A );
        dg::filleff( "pu_mc10b"  , pileup_reweighting_mode==PU_MC10B );
        dg::filleff( "pu_none"  , pileup_reweighting_mode==PU_NONE );
        if( is_mc ) { 
          const double pu_weight = pw ? pw->getPileupWeight( evt->average_mu() ) : 1. ;
          const double mc_weight = evt->mc_weight() > 0. ? 1. : -1.;
          dg::fillh( "pu_weight" , 100 , -10 , 10 , pu_weight );
          dg::fillh( "pu_weight_log10" , 100 , -10 , 10 , detector::safe_log10(pu_weight) );
          dg::fillh( "mc_weight" , 10 , -5 , 5 , mc_weight );
          // cout << "pu: " << pu_weight << " ,mc: " << mc_weight << ", evt:" << evtweight << endl;          
          float tmpweight = evtweight;
          evtweight = tmpweight * pu_weight * mc_weight;
          mcWeight=mc_weight;
          pileupWeight = pu_weight;
          dg::set_global_weight( evtweight );
          //dg::set_global_weight( 1. ); // cm do not reweigh to count objects
        }
      }
    } // end do mc event reweighting


      
    {
      dg::down( "plhc_hsg3_cutflow" , "selector details" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      dg::fillh( "c1_weight" , 100 , -10 , 10 , dg::global_weight() );
      dg::fillh( "c1_weight_log10" , 100 , -10 , 10 , detector::safe_log10(dg::global_weight()) );
      dg::filleff( "c1_reweighted" , 1 );
    }

    // make good run requirement
    // ================================================================
    {
      dg::down( "plhc_hsg3_cutflow" , "selector details" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      dg::filleff( "c2_is_good_run", is_good_run_ele || is_good_run_mu );
      dg::filleff( "c2_is_good_run_ele" , is_good_run_ele );
      dg::filleff( "c2_is_good_run_mu" , is_good_run_mu );
    }
    if( !(is_good_run_ele || is_good_run_mu) ) { continue; }

    // select electrons and anti-electrons
    // ================================================================
    if( is_mc ) { dg::set_global_weight( 1. ); } // reverted to 1 to count objects
    // select primary vertices, and fill selector histograms
    // ================================================================
    {
      dg::down( "selectors" , "selector details" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      primary_vertex_selector.for_only( evt->vertices().front() ); // HWW uses only the 'primary'/first/highest pT vertex
      jetcalib_primary_vertex_selector.for_each( evt->vertices().begin() , evt->vertices().end() );
    }
    const int nprivtx = primary_vertices.size();
    const int jetcalib_nprivtx = jetcalib_primary_vertices.size();
    {
      dg::fillh( "nprivtx" , 30 , 0 , 30 , nprivtx , "NUMBER OF PRIMARY VERTICES WITH >= 3 TRACKS" );
      dg::fillh( "jetcalib_nprivtx" , 30 , 0 , 30 , jetcalib_nprivtx , "NUMBER OF PRIMARY VERTICES WITH >= 5 TRACKS" );
      dg::fillh( "jetcalib_vs_hww_nprivtx" , 30 , 0 , 30 , 30 , 0 , 30 , 
                 nprivtx , jetcalib_nprivtx , 
                 "NUMBER OF PRIMARY VERTICES WITH >= 3 TRACKS" , "NUMBER OF PRIMARY VERTICES WITH >= 5 TRACKS" );
    }
    
    if( is_good_run_ele ) { 
      dg::down( "selectors" , "selector details" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      ele_probe_selector.for_each( evt->electrons().begin() , evt->electrons().end() );
      if( is_mc ) { // do smearing
        // FIXME
        size_t ielec = 0;
        for( std::vector< shared_ptr<const AnaElectron> >::const_iterator i=evt->electrons().begin() , f=evt->electrons().end(); i!=f; ++i,++ielec ) {
          const shared_ptr<const AnaElectron> ele(*i);
          //cout << "ele pt before: " << ele->pt() << " " << ele->cluster_pt() << endl;
          ele_event.push_back( ele->smeared_electron_2011( ele , evt->event_number() , ielec  , 0 ) ); // 0=NOMINAL, 1=ERR_DOWN, 2=ERR_UP
          //cout << "ele pt after: " << ele_event.back()->pt() << " " << ele->cluster_pt() << endl;
        }
      } else { // rescale for data
        size_t ielec = 0;
        for( std::vector< shared_ptr<const AnaElectron> >::const_iterator i=evt->electrons().begin() , f=evt->electrons().end(); i!=f; ++i,++ielec ) {
          const shared_ptr<const AnaElectron> ele(*i);
          ele_event.push_back( ele->corrected_electron_2011( ele , evt->event_number() , ielec  , 0 ) ); // 0=NOMINAL, 1=ERR_DOWN, 2=ERR_UP
        }
      }
      ele_hww_selector.for_each( ele_event.begin() , ele_event.end() );
      ele_hww_lowpt_selector.for_each( ele_event.begin() , ele_event.end() );
      ele_FEB_selector.for_each( ele_event.begin() , ele_event.end() );
      // ele_hww_withoverlap now filled. remove overlaps later.
      ele_nontrigger_loose_selector.for_each( evt->electrons().begin() , evt->electrons().end() );
      ele_anti_selector.for_each( evt->electrons().begin() , evt->electrons().end() );
      dg::fillh( "nele_input" , 100 , 0 , 100 , evt->electrons().size() );
      dg::fillh( "nele_probe" , 100 , 0 , 100 , ele_probe.size() );
      dg::fillh( "nele_expanded_probe" , 100 , 0 , 100 , ele_expanded_probe.size() );
      dg::fillh( "nele_hww_withoverlap" , 100 , 0 , 100 , ele_hww_withoverlap.size() );
      dg::fillh( "nele_hww_lowpt_withoverlap" , 100 , 0 , 100 , ele_hww_lowpt_withoverlap.size() );
      dg::fillh( "nele_nontrigger_loose" , 100 , 0 , 100 , ele_nontrigger_loose.size() );
      dg::fillh( "nele_anti" , 100 , 0 , 100 , ele_anti.size() );
    }

    if( is_good_run_ele ) { 
      // photons in 16.6 and later have corrections already applied. see
      // https://twiki.cern.ch/twiki/bin/view/AtlasProtected/EnergyScaleResolutionRecommendations#Recommendations_for_EPS_2011_dat
      loose_photon_selector.for_each( evt->photons().begin() , evt->photons().end() );
      tight_photon_selector.for_each( evt->photons().begin() , evt->photons().end() );
    }

    // select muons
    // ================================================================
    if( is_good_run_mu ) { 
      dg::down( "selectors" , "selector details" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      // smear any MC muons according to the muon combined performance prescription.
      if( is_mc ) {
        size_t imuon = 0;
        for( std::vector< shared_ptr<const AnaMuon> >::const_iterator i=evt->muons().begin() , f=evt->muons().end(); i!=f; ++i,++imuon ) {
          const shared_ptr<const AnaMuon> mu(*i);
          mu_event.push_back( mu->mcp_smeared(conf,AnaConfiguration::data_period(),mu,evt->event_number(),imuon,true) );
        }
      } else {
        mu_event.push_back_all( evt->muons().begin() , evt->muons().end() );
      }
      // apply selectors
      mu_probe_staco_selector.for_each( mu_event.begin() , mu_event.end() );
      // mu_probe_muid_selector.for_each( mu_event.begin() , mu_event.end() );
      //
      // mu_expanded_probe_selector.for_each( mu_event.begin() , mu_event.end() ); // includes calo muons
      // vector< shared_ptr<const AnaMuon> > track_muons;
      // std::transform( evt->offline_tracks().begin() , evt->offline_tracks().end() , back_inserter( track_muons ) ,
      //                 bind( &AnaMuon::make_track_muon , _1 ) );
      // mu_expanded_probe_selector.for_each( track_muons.begin() , track_muons.end() ); // includes ID tracks
      //
      mu_hww_selector.for_each( mu_event.begin() , mu_event.end() );
      mu_hww_preiso_selector.for_each( mu_event.begin() , mu_event.end() );
      mu_hww_lowpt_preiso_selector.for_each( mu_event.begin() , mu_event.end() );
      mu_hww_lowpt_selector.for_each( mu_event.begin() , mu_event.end() );
      //
      mu_lbl_tight_selector.for_each( mu_event.begin() , mu_event.end() );
      mu_lbl_loose_selector.for_each( mu_event.begin() , mu_event.end() );
      mu_lbl_nonprompt_preiso_selector.for_each( mu_event.begin() , mu_event.end() );
      mu_lbl_preiso_pred0sig_selector.for_each( mu_event.begin() , mu_event.end() );
      mu_lbl_preiso_selector.for_each( mu_event.begin() , mu_event.end() );
      //
      mu_nontrigger_loose_selector.for_each( mu_event.begin() , mu_event.end() );
      mu_tag_lowmh_selector.for_each( mu_event.begin() , mu_event.end() );
      mu_anti_selector.for_each( mu_event.begin() , mu_event.end() );
    }

    // remove electron-muon and electron-electron overlap.
    // ================================================================
    if( true ) { 
      dg::down( "selectors" , "selector details" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      /* is it the overlaps? */
      vector< shared_ptr<const AnaElectron> > tmp;
      vector< shared_ptr<const AnaElectron> > tmp_low;
      // remove electron-muon overlap according to PLHC HSG3 prescription.
      overlap_removal_all_in_cone( ele_hww_withoverlap.begin() , ele_hww_withoverlap.end() , mu_hww.begin() , mu_hww.end() , 0.1 , back_inserter(tmp) ); 
      overlap_removal_all_in_cone( ele_hww_lowpt_withoverlap.begin() , ele_hww_lowpt_withoverlap.end() , mu_hww_lowpt.begin() , mu_hww_lowpt.end() , 0.1 , back_inserter(tmp_low) ); 
      dg::fillh( "nele_hww_nomuoverlap" , 100 , 0 , 100 , tmp.size() );
      dg::fillh( "nele_hww_low_nomuoverlap" , 100 , 0 , 100 , tmp_low.size() );
      // remove internal electron-electron overlap according to PLHC HSG3 prescription.
      internal_overlap_removal_by_pt( tmp.begin() , tmp.end() , 0.1 , back_inserter(ele_hww) , false ); 
      internal_overlap_removal_by_pt( tmp_low.begin() , tmp_low.end() , 0.1 , back_inserter(ele_hww_lowpt) , false ); 
      dg::fillh( "nele_hww" , 100 , 0 , 100 , ele_hww.size() );
      dg::fillh( "nele_hww_lowpt" , 100 , 0 , 100 , ele_hww_lowpt.size() );
    }


    // remove double-counting overlap between expanded probe leptons and probe leptons
    // if( !ele_expanded_probe.empty() ) {
    //   dg::fillh( "ele_expanded_probe_preoverlap" , 10 , 0 , 10 , ele_expanded_probe.size() );
    //   dg::fillh( "ele_probe_preoverlap" , 10 , 0 , 10 , ele_probe.size() );
    //   vector< shared_ptr<const AnaElectron> > tmp( ele_expanded_probe.begin() , ele_expanded_probe.end() );
    //   ele_expanded_probe.clear();
    //   overlap_removal( tmp.begin() , tmp.end() ,
    //                    ele_probe.begin() , ele_probe.end() ,
    //                    0.05 , ele_expanded_probe.back_inserter() );
    //   dg::fillh( "ele_expanded_probe_postoverlap" , 10 , 0 , 10 , ele_expanded_probe.size() );
    //   std::copy( ele_probe.begin() , ele_probe.end() , ele_expanded_probe.back_inserter() ); // add one copy of probe ele back in.
    //   dg::fillh( "ele_expanded_probe_postmerge" , 10 , 0 , 10 , ele_expanded_probe.size() );
    // }
    // if( !mu_expanded_probe.empty() ) {
    //   dg::fillh( "mu_expanded_probe_preoverlap" , 10 , 0 , 10 , mu_expanded_probe.size() );
    //   dg::fillh( "mu_probe_staco_preoverlap" , 10 , 0 , 10 , mu_probe_staco.size() );
    //   dg::fillh( "mu_probe_muid_preoverlap" , 10 , 0 , 10 , mu_probe_muid.size() );
    //   vector< shared_ptr<const AnaMuon> > tmp( mu_expanded_probe.begin() , mu_expanded_probe.end() );
    //   vector< shared_ptr<const AnaMuon> > tmpB;
    //   std::copy( mu_probe_staco.begin() , mu_probe_staco.end() , back_inserter(tmpB) );
    //   // std::copy( mu_probe_muid.begin() , mu_probe_muid.end() , back_inserter(tmpB) );
    //   mu_expanded_probe.clear();
    //   overlap_removal( tmp.begin() , tmp.end() ,
    //                    tmpB.begin() , tmpB.end() ,
    //                    0.05 , mu_expanded_probe.back_inserter() );
    //   dg::fillh( "mu_expanded_probe_postoverlap" , 10 , 0 , 10 , mu_expanded_probe.size() );
    //   std::copy( tmpB.begin() , tmpB.end() , mu_expanded_probe.back_inserter() ); // add one copy of probe muons back in.
    //   dg::fillh( "mu_expanded_probe_postmerge" , 10 , 0 , 10 , mu_expanded_probe.size() );
    // }

    // merge lepton collections
    // ================================================================
    lepton_probe.push_back_all( ele_probe.begin() , ele_probe.end() );
    lepton_probe.push_back_all( mu_probe_staco.begin() , mu_probe_staco.end() );
    // lepton_expanded_probe.push_back_all( ele_expanded_probe.begin() , ele_expanded_probe.end() );
    // lepton_expanded_probe.push_back_all( mu_expanded_probe.begin() , mu_expanded_probe.end() );
    lepton_hww.push_back_all( ele_hww.begin() , ele_hww.end() );
    lepton_hww.push_back_all( mu_hww.begin() , mu_hww.end() );
    lepton_hww_lowpt.push_back_all( ele_hww_lowpt.begin() , ele_hww_lowpt.end() );
    lepton_hww_lowpt.push_back_all( mu_hww_lowpt.begin() , mu_hww_lowpt.end() );
    lepton_lowmh.push_back_all( ele_nontrigger_loose.begin() , ele_nontrigger_loose.end() );
    lepton_lowmh.push_back_all( mu_nontrigger_loose.begin() , mu_nontrigger_loose.end() );
    lepton_anti.push_back_all( ele_anti.begin() , ele_anti.end() );
    lepton_anti.push_back_all( mu_anti.begin() , mu_anti.end() );

    // select jets and missing ET
    // ================================================================
    // recompute missing ET for H->WW->dil
    if( true ) {
      BOOST_FOREACH( const shared_ptr<const AnaMET>& met , evt->missing_et() ) {
        met_hww_event.push_back( AnaMET::recompute_for_hww( met , mu_hww.begin() , mu_hww.end() ) );
        met_hww_lowpt_event.push_back( AnaMET::recompute_for_hww( met , mu_hww_lowpt.begin() , mu_hww_lowpt.end() ) );
      }
    }
    // apply selectors
    if( true ) {
      dg::down( "selectors" , "selector details" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      jet_FEB_withoverlap_selector.for_each( evt->atlas_anti_kt_4_h1_topo_jets().begin() , 
                                             evt->atlas_anti_kt_4_h1_topo_jets().end() );
      jet_hww_cleanup_withoverlap_selector.for_each( evt->atlas_anti_kt_4_h1_topo_jets().begin() , 
                                                     evt->atlas_anti_kt_4_h1_topo_jets().end() );
      jet_hww_withoverlap_selector.for_each( evt->atlas_anti_kt_4_h1_topo_jets().begin() , 
                                             evt->atlas_anti_kt_4_h1_topo_jets().end() );
      jet_hww_lowpt_withoverlap_selector.for_each( evt->atlas_anti_kt_4_h1_topo_jets().begin() , 
                                                   evt->atlas_anti_kt_4_h1_topo_jets().end() );
      // jet-lepton removal
      overlap_removal_all_in_cone( jet_hww_cleanup_withoverlap.begin() , jet_hww_cleanup_withoverlap.end() , ele_hww.begin() , ele_hww.end() , 0.3 , back_inserter(jet_hww_cleanup) );
      overlap_removal_all_in_cone( jet_hww_withoverlap.begin() , jet_hww_withoverlap.end() , ele_hww.begin() , ele_hww.end() , 0.3 , back_inserter(jet_hww) );
      overlap_removal_all_in_cone( jet_hww_lowpt_withoverlap.begin() , jet_hww_lowpt_withoverlap.end() , ele_hww_lowpt.begin() , ele_hww_lowpt.end() , 0.3 , back_inserter(jet_hww_lowpt) );
      overlap_removal_all_in_cone( jet_FEB_withoverlap.begin() , jet_FEB_withoverlap.end() , ele_FEB.begin() , ele_FEB.end() , 0.3 , back_inserter(jet_FEB) );
      
      // must fill MET after the jets
      met_hww_selector.for_each( met_hww_event.begin() , met_hww_event.end() );
      met_hww_lowpt_selector.for_each( met_hww_lowpt_event.begin() , met_hww_lowpt_event.end() );
    }

    if( is_mc ) { dg::set_global_weight( evtweight ); } // back to evtWeight (reverted earlier to 1 to count objects) 

    if( true ) {
      dg::down( "plhc_hsg3_cutflow" , "selector details" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      dg::filleff( "c3_privtx" , nprivtx>=1 );
    }
    if( !nprivtx ) { continue; }


    const bool jets_in_bad_FEB = std::count_if( jet_FEB.begin() , jet_FEB.end() , 
                                                bind(&AnaJet::isBadJet_FEBhole,_1)==true) > 0;
    const bool eles_in_bad_FEB = std::count_if( ele_FEB.begin() , ele_FEB.end() , 
                                                bind(&AnaElectron::isBadElectron_FEBhole, _1 ) == true) > 0;

    mc_dataperiod.SetSeed(evt->event_number());
    int dataperiodformc = mc_dataperiod.GetDataPeriod();
    const bool bad_FEB = ( jets_in_bad_FEB || eles_in_bad_FEB );
    
    // check that all jets in the jet collection not matched to other objects are good for met in v16.
    const bool any_bad_jets_loose = std::count_if( jet_hww.begin() , jet_hww.end() , 
                                                   bind(&AnaJet::isBadJet_hww_v16,_1)==true && bind(&AnaJet::pt,_1)>20.) > 0;
    dg::fillh( "njets_withoverlap" , 50 , 0 , 50 , jet_hww_withoverlap.size() );
    dg::fillh( "njets" , 50 , 0 , 50 , jet_hww.size() );

    bool badFEBdata = false;
    float febWeight =  (163.5 + 2052 - 1035)/2052;
    if( is_mc && bad_FEB ) {
      float tmpweight = evtweight;
      evtweight = tmpweight * febWeight;
    } else if( !is_mc && evt->run_number() >= 180614 && bad_FEB ) {
      badFEBdata = true;   
    }

    if( is_mc ) { dg::set_global_weight( evtweight ); } // back to evtWeight (reverted earlier to 1 to count objects) 

    const bool ok_clean_met = ( (is_mc || !any_bad_jets_loose) && !met_hww.empty() && !badFEBdata );
    const bool ok_clean_met_lowpt = ( (is_mc || !any_bad_jets_loose) && !met_hww_lowpt.empty() && !badFEBdata );
    {
      dg::down( "plhc_hsg3_cutflow" , "selector details" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      dg::filleff("c4_event_clean", ok_clean_met );
      dg::filleff("c4_event_clean_lowpt", ok_clean_met_lowpt );
    }
    if( !ok_clean_met && !ok_clean_met_lowpt ) { continue; }

    // retrieve trigger
    const boost::shared_ptr<const AnaTrigger>& trigger = evt->trigger();
    assert( trigger );
    trigger->strict_checking( false ); // FIXME: turn on aggressive checking!
    const bool is_triggered_ele = trigger->passes("EF_e20_medium");
    const bool is_triggered_mu = trigger->passes("EF_mu18_mg") || trigger->passes("EF_mu40_MSOnly_barrel");
    const bool is_triggered = is_triggered_ele || is_triggered_mu;
    const bool is_good_triggered_ele = is_triggered_ele && is_good_run_ele;
    const bool is_good_triggered_mu = is_triggered_mu && is_good_run_mu;
    const bool is_good_triggered = is_good_triggered_ele || is_good_triggered_mu;
    // alternate triggers
    const bool is_good_triggered_e15_medium = is_good_run_ele && trigger->passes("EF_e15_medium");
    const bool is_good_triggered_e20_medium = is_good_run_ele && trigger->passes("EF_e20_medium");
    const bool is_good_triggered_e25_medium = is_good_run_ele && trigger->passes("EF_e25_medium");
    const bool is_good_triggered_e30_loose = is_good_run_ele && trigger->passes("EF_e30_loose");
    const bool is_good_triggered_mu15_mg = is_good_run_ele && trigger->passes("EF_mu15_mg");
    const bool is_good_triggered_mu20 = is_good_run_ele && trigger->passes("EF_mu20");
    const bool is_good_triggered_2mu10_loose = is_good_run_ele && trigger->passes("EF_2mu10_loose");
    const bool is_good_triggered_2e10_medium = is_good_run_ele && trigger->passes("EF_2e10_medium");
    const bool is_good_triggered_e10_loose_mu6 = is_good_run_ele && trigger->passes("EF_e10_loose_mu6");

    dg::fillh( "n_electrons_probe" , 100 , 0 , 100 , ele_probe.size() , "Number of probe electrons" );
    dg::fillh( "n_muons_probe_staco" , 100 , 0 , 100 , mu_probe_staco.size() , "Number of STACO probe muons" );
    dg::fillh( "n_muons_probe_muid" , 100 , 0 , 100 , mu_probe_muid.size() , "Number of MUID probe muons" );
    dg::fillh( "n_electrons_expanded_probe" , 100 , 0 , 100 , ele_expanded_probe.size() , "Number of expanded_probe electrons" );
    dg::fillh( "n_muons_expanded_probe" , 100 , 0 , 100 , mu_expanded_probe.size() , "Number of expanded_probe muons" );

    dg::fillh( "n_jets_hww" , 100 , 0 , 100 , jet_hww.size() , "Number of selected hsg3 hww jets" );
    dg::fillh( "n_electrons_hww" , 100 , 0 , 100 , ele_hww.size() , "Number of selected SM WW electrons" );
    dg::fillh( "n_muons_hww" , 100 , 0 , 100 , mu_hww.size() , "Number of selected SM WW muons" );
    dg::fillh( "n_muons_lowpt_hww" , 100 , 0 , 100 , mu_hww_lowpt.size() , "Number of selected SM WW muons" );
    dg::fillh( "n_met_hww" , 10 , 0 , 10, met_hww.size() , "Number of selected H->WW met (should be exactly one)" );

    dg::filleff( "trigger_fired_ele", is_triggered_ele );
    dg::filleff( "trigger_fired_mu", is_triggered_mu );
    dg::filleff( "good_triggered_ele", is_good_triggered_ele );
    dg::filleff( "good_triggered_mu", is_good_triggered_mu );
    dg::filleff( "trigger_fired", is_triggered );
    dg::filleff( "good_triggered", is_good_triggered );
    if( true && (is_good_run_ele||is_good_run_mu) ) {
      dg::down( "trigger" , "trigger bits" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      dg::filleff( "EF_2e10_medium" , trigger->passes("EF_2e10_medium") );
      dg::filleff( "EF_2mu10" , trigger->passes("EF_2mu10") );
      dg::filleff( "EF_e10_loose_mu6" , trigger->passes("EF_e10_loose_mu6") );
      dg::filleff( "EF_e15_medium" , trigger->passes("EF_e15_medium") );
      dg::filleff( "EF_e20_medium" , trigger->passes("EF_e20_medium") );
      dg::filleff( "EF_e25_medium" , trigger->passes("EF_e25_medium") );
      dg::filleff( "EF_e30_loose" , trigger->passes("EF_e30_loose") );
      dg::filleff( "EF_mu10" , trigger->passes("EF_mu10") );
      dg::filleff( "EF_mu10_mg" , trigger->passes("EF_mu10_mg") );
      dg::filleff( "EF_mu13_mg" , trigger->passes("EF_mu13_mg") );
      dg::filleff( "EF_mu13_mg_tight" , trigger->passes("EF_mu13_mg_tight") );
      dg::filleff( "EF_mu15_mg" , trigger->passes("EF_mu15_mg") );
      dg::filleff( "EF_mu18" , trigger->passes("EF_mu18") );
      dg::filleff( "EF_mu18_mg" , trigger->passes("EF_mu18_mg") );
      dg::filleff( "EF_mu40_MSOnly_barrel" , trigger->passes("EF_mu40_MSOnly_barrel") );
      dg::filleff( "EF_2mu10_loose" , trigger->passes("EF_2mu10_loose") );
      dg::filleff( "EF_mu6" , trigger->passes("EF_mu6") );
      dg::filleff( "EF_mu20" , trigger->passes("EF_mu20") );
      dg::filleff( "L1_em14" , trigger->passes("L1_em14") );
      dg::filleff( "L1_mu0" , trigger->passes("L1_mu0") );
      dg::filleff( "L1_mu10" , trigger->passes("L1_mu10") );
      dg::filleff( "L1_mu6" , trigger->passes("L1_mu6") );
    }

    // HSG3 cutflow
    // ================================================================
    hsg3_cutflow( is_good_run_ele || is_good_run_mu , evtweight , ok_clean_met , ok_clean_met_lowpt , bad_FEB , 
                  ele_hww , mu_hww , mu_hww_lowpt , met_hww , jet_hww , 
                  outweight , febWeight , pileupWeight , mcWeight );
    
    /* // check cutmask returned from the above, and do fake rate studies at various stages of the cutflow.
        // do Z+n tracks fake study before cut on number of leptons...
        if( lepton_hww.size()>=2 ) { 
          vector< shared_ptr<const AnaParticle> > leading_leptons(2);
          std::partial_sort_copy( lepton_hww.begin() , lepton_hww.end() , leading_leptons.begin() , leading_leptons.end() ,
                                  bind(&AnaParticle::pt,_1) > bind(&AnaParticle::pt,_2) );
          assert( leading_leptons.size()==2 );
          const shared_ptr<const AnaParticle>& leading_lepton( leading_leptons[0] );
          const shared_ptr<const AnaParticle>& subleading_lepton( leading_leptons[1] );
          if( leading_lepton->type()==subleading_lepton->type() ) {
            shared_ptr<const AnaZ> zboson( make_shared<const AnaZ>(leading_lepton,subleading_lepton) );
            StudyFakeLeptonsInZEvents stfake_leptons( "zfakes_wrt_none" , "fake leptons in z events" );
            stfake_leptons.add_category( "zwin" , bind( AnaParticle::MassInWindow<const AnaParticle>(76.,106.) , _1 ) );
            stfake_leptons.set_leptons( lepton_hww.begin() , lepton_hww.end() );
            stfake_leptons.repurpose( "zfakes_wrt_jets" , "fake leptons in z events" );
            stfake_leptons.set_denominator( jet_hww.begin() , jet_hww.end() );
            stfake_leptons.for_only( zboson );
            stfake_leptons.repurpose( "zfakes_wrt_recmuons" , "fake leptons in z events" );
            stfake_leptons.set_denominator( mu_probe_staco.begin() , mu_probe_staco.end() );
            stfake_leptons.for_only( zboson );
            stfake_leptons.repurpose( "zfakes_wrt_recelectrons" , "fake leptons in z events" );
            stfake_leptons.set_denominator( ele_probe.begin() , ele_probe.end() );
            stfake_leptons.for_only( zboson );
          }
        } // end Z+n fakes study
        // low pt muons Z+n fakes study
        if( mu_hww_lowpt.size()>=2 ) { 
          vector< shared_ptr<const AnaParticle> > leading_leptons(2);
          std::partial_sort_copy( mu_hww_lowpt.begin() , mu_hww_lowpt.end() , leading_leptons.begin() , leading_leptons.end() ,
                                  bind(&AnaParticle::pt,_1) > bind(&AnaParticle::pt,_2) );
          assert( leading_leptons.size()==2 );
          const shared_ptr<const AnaParticle>& leading_lepton( leading_leptons[0] );
          const shared_ptr<const AnaParticle>& subleading_lepton( leading_leptons[1] );
          if( leading_lepton->type()==subleading_lepton->type() ) {
            shared_ptr<const AnaZ> zboson( make_shared<const AnaZ>(leading_lepton,subleading_lepton) );
            StudyFakeLeptonsInZEvents stfake_leptons( "zfakes_lowpt_wrt_none" , "fake leptons in z events" );
            stfake_leptons.add_category( "zwin" , bind( AnaParticle::MassInWindow<const AnaParticle>(76.,106.) , _1 ) );
            stfake_leptons.set_leptons( mu_hww_lowpt.begin() , mu_hww_lowpt.end() );
            stfake_leptons.repurpose( "zfakes_lowpt_wrt_jets" , "fake leptons in z events" );
            stfake_leptons.set_denominator( jet_hww.begin() , jet_hww.end() );
            stfake_leptons.for_only( zboson );
            stfake_leptons.repurpose( "zfakes_lowpt_wrt_recmuons" , "fake leptons in z events" );
            stfake_leptons.set_denominator( mu_probe_staco.begin() , mu_probe_staco.end() );
            stfake_leptons.for_only( zboson );
          }
        } // end Z+n fakes study

        // do Z+n tracks fake study with low pt leptons,  before cut on number of leptons...
        if( lepton_hww_lowpt.size()>=2 ) { 
          vector< shared_ptr<const AnaParticle> > leading_leptons(2);
          std::partial_sort_copy( lepton_hww_lowpt.begin() , lepton_hww_lowpt.end() , leading_leptons.begin() , leading_leptons.end() ,
                                  bind(&AnaParticle::pt,_1) > bind(&AnaParticle::pt,_2) );
          assert( leading_leptons.size()==2 );
          const shared_ptr<const AnaParticle>& leading_lepton( leading_leptons[0] );
          const shared_ptr<const AnaParticle>& subleading_lepton( leading_leptons[1] );
          if( leading_lepton->type()==subleading_lepton->type() ) {
            shared_ptr<const AnaZ> zboson( make_shared<const AnaZ>(leading_lepton,subleading_lepton) );
            StudyFakeLeptonsInZEvents stfake_leptons( "zfakes_wrt_none_lowpt" , "fake leptons in z events" );
            stfake_leptons.add_category( "zwin" , bind( AnaParticle::MassInWindow<const AnaParticle>(76.,106.) , _1 ) );
            stfake_leptons.set_leptons( lepton_hww_lowpt.begin() , lepton_hww_lowpt.end() );
            stfake_leptons.repurpose( "zfakes_wrt_jets_lowpt" , "fake leptons in z events" );
            stfake_leptons.set_denominator( jet_hww_lowpt.begin() , jet_hww_lowpt.end() );
            stfake_leptons.for_only( zboson );
            stfake_leptons.repurpose( "zfakes_wrt_recmuons_lowpt" , "fake leptons in z events" );
            stfake_leptons.set_denominator( mu_probe_staco.begin() , mu_probe_staco.end() );
            stfake_leptons.for_only( zboson );
            stfake_leptons.repurpose( "zfakes_wrt_recelectrons_lowpt" , "fake leptons in z events" );
            stfake_leptons.set_denominator( ele_probe.begin() , ele_probe.end() );
            stfake_leptons.for_only( zboson );
          }
        } // end Z+n fakes study
    */

    // ================================================================
    // identify truth-level info for higgs signal mc
    // ================================================================
    if( false ) { 
      vector<AnaTruthTable::truth_ptr> accepted_truth_w_products;
      vector<AnaTruthTable::truth_ptr> accepted_truth_w_lepton_products;
      vector<AnaTruthTable::truth_ptr> accepted_truth_w_ele_products;
      vector<AnaTruthTable::truth_ptr> accepted_truth_w_mu_products;
      vector<AnaTruthTable::truth_ptr> truth_w_products;
      unsigned int ngen_w_ele = 0u;
      unsigned int ngen_w_mu = 0u;
      if( evt->truth_table() ) {
        truth_w_products = evt->truth_table()->all_from_decays_of_type< vector<AnaTruthTable::truth_ptr> >(24u,
                                                                                                           bind(&AnaTruthParticle::is_final_state,_1)==true ||
                                                                                                           bind(&AnaTruthParticle::is_quark,_1)==true) ;
        ngen_w_ele = std::count_if( truth_w_products.begin() , truth_w_products.end() , bind(&AnaTruthParticle::is_electron,_1)==true );
        ngen_w_mu = std::count_if( truth_w_products.begin() , truth_w_products.end() , bind(&AnaTruthParticle::is_muon,_1)==true );
        // collect list of w products within acceptance
        BOOST_FOREACH( shared_ptr<AnaTruthParticle>& p , truth_w_products ) { 
          if( p->pt() < 0.001 ) { continue; }
          if( p->is_electron() && std::abs(p->eta())<4 && std::abs(p->pt())>10. ) {
            accepted_truth_w_products.push_back( p );
            accepted_truth_w_lepton_products.push_back( p );
            accepted_truth_w_ele_products.push_back( p );
          } else if( p->is_muon() && std::abs(p->eta())<4 && std::abs(p->pt())>10. ) {
            accepted_truth_w_products.push_back( p );
            accepted_truth_w_lepton_products.push_back( p );
            accepted_truth_w_mu_products.push_back( p );
          } else if( p->is_neutrino() ) {
            accepted_truth_w_products.push_back( p );
          } else if( p->is_quark_or_gluon() && std::abs(p->eta())<2.0 && std::abs(p->pt())>10. ) {
            accepted_truth_w_products.push_back( p );
          }
        }
      }
      dg::fillh( "ngen_w_ele" , 10 , 0 , 10 , ngen_w_ele );
      dg::fillh( "ngen_w_mu" , 10 , 0 , 10 , ngen_w_mu );
      dg::fillh( "ngen_w_eleORmu" , 10 , 0 , 10 , ngen_w_ele+ngen_w_mu );
      
      // plot truth level spectra of w products and accepted w products.
      if( true ) { 
        dg::down( "generator_kinematics" , "generator-level kinematic selection" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
        StudyHwwTruthProducts< vector<AnaTruthTable::truth_ptr> > stht( "hww_truth_products" , "higgs->ww->xxxx truth products" );
        stht.add_constant_category( "dielectron" , ngen_w_ele>=2 && ngen_w_mu==0 );
        stht.add_constant_category( "dimuon" , ngen_w_mu>=2 && ngen_w_ele==0 );
        stht.add_constant_category( "electron_muon" , ngen_w_mu>=1 && ngen_w_ele>=1 );
        // stht.for_only( truth_w_products );
        stht.repurpose( "hww_truth_products_accepted" , "higgs->ww->xxxx truth products in acceptance" );
        stht.for_only( accepted_truth_w_products );
      }

      // if higgs leptons, plot efficiency vs position
      if( !accepted_truth_w_lepton_products.empty() ) {
        dg::down( "mc_lepton_efficiency" , "selector details" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
        if( true ) { 
          dg::down( "probe_leptons" , "probe lepton reconstruction" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
          StudyLeptonReconstruction<> stlep( "ele_rec" , "lepton reconstruction study" );
          stlep.reconstructed_electrons( ele_probe.begin() , ele_probe.end() );
          stlep.reconstructed_muons( mu_probe_staco.begin() , mu_probe_staco.end() );
          stlep.for_each( accepted_truth_w_ele_products.begin() , accepted_truth_w_ele_products.end() );
          stlep.repurpose( "mu_rec" , "lepton reconstruction study" );
          stlep.for_each( accepted_truth_w_mu_products.begin() , accepted_truth_w_mu_products.end() );
          // restrict to only staco combined + mutag + muonboy standalone
          // vector< shared_ptr<const AnaMuon> > staco_only_mu;
          // dgSTL::copy_if( mu_probe.begin() , mu_probe.end() , back_inserter(staco_only_mu) , 
          //                 bind(&AnaMuon::is_staco_any,_1)==true );
          vector< shared_ptr<const AnaMuon> > staco_cmm;
          dgSTL::copy_if( mu_probe_staco.begin() , mu_probe_staco.end() , back_inserter(staco_cmm) , 
                          bind(&AnaMuon::is_staco_combined,_1)==true ||
                          bind(&AnaMuon::is_mutag,_1)==true ||
                          bind(&AnaMuon::is_muonboy,_1)==true );
          // restrict to only staco combined + mutag
          vector< shared_ptr<const AnaMuon> > staco_mutag;
          dgSTL::copy_if( mu_probe_staco.begin() , mu_probe_staco.end() , back_inserter(staco_mutag) , 
                          bind(&AnaMuon::is_staco_combined,_1)==true ||
                          bind(&AnaMuon::is_mutag,_1)==true );
          // restrict to only staco combined
          vector< shared_ptr<const AnaMuon> > staco;
          dgSTL::copy_if( mu_probe_staco.begin() , mu_probe_staco.end() , back_inserter(staco) , 
                          bind(&AnaMuon::is_staco_combined,_1)==true );
          // restrict to only staco combined with ID cuts
          vector< shared_ptr<const AnaMuon> > staco_idcuts;
          dgSTL::copy_if( staco.begin() , staco.end() , back_inserter(staco_idcuts) , 
                          ( bind( std::ptr_fun<AnaMuon::ftype,AnaMuon::ftype>(std::abs) , 
                                  bind(&AnaMuon::eta,_1)) > 2.0 || 
                            ( bind(&AnaMuon::nhits,_1,AnaMuon::PIXEL) > 1 &&
                              bind(&AnaMuon::nhits,_1,AnaMuon::SCT) > 5 &&
                              bind(&AnaMuon::nhits,_1,AnaMuon::TRT) > 0 ) ) );
          // make lepton reconsturction plots for these cuts.
          // stlep.repurpose( "staco_mutag_or_muonboy" , "lepton reconstruction study" );
          // stlep.reconstructed_muons( staco_cmm.begin() , staco_cmm.end() );
          // stlep.for_each( accepted_truth_w_mu_products.begin() , accepted_truth_w_mu_products.end() );
          stlep.repurpose( "staco_or_mutag" , "lepton reconstruction study" );
          stlep.reconstructed_muons( staco_mutag.begin() , staco_mutag.end() );
          stlep.for_each( accepted_truth_w_mu_products.begin() , accepted_truth_w_mu_products.end() );
          stlep.repurpose( "staco_combined" , "lepton reconstruction study" );
          stlep.reconstructed_muons( staco.begin() , staco.end() );
          stlep.for_each( accepted_truth_w_mu_products.begin() , accepted_truth_w_mu_products.end() );
          // stlep.repurpose( "staco_idcuts" , "lepton reconstruction study" );
          // stlep.reconstructed_muons( staco_idcuts.begin() , staco_idcuts.end() );
          // stlep.for_each( accepted_truth_w_mu_products.begin() , accepted_truth_w_mu_products.end() );
        }
        if( false ) { 
          dg::down( "probe_muid_leptons" , "probe lepton reconstruction" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
          StudyLeptonReconstruction<> stlep( "mu_rec" , "lepton reconstruction study" );
          stlep.reconstructed_muons( mu_probe_muid.begin() , mu_probe_muid.end() );
          stlep.for_each( accepted_truth_w_mu_products.begin() , accepted_truth_w_mu_products.end() );
        }
        if( false ) { 
          dg::down( "expanded_probe_leptons" , "expanded_probe lepton reconstruction" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
          StudyLeptonReconstruction<> stlep( "ele_rec" , "lepton reconstruction study" );
          stlep.reconstructed_electrons( ele_expanded_probe.begin() , ele_expanded_probe.end() );
          stlep.reconstructed_muons( mu_expanded_probe.begin() , mu_expanded_probe.end() );
          stlep.for_each( accepted_truth_w_ele_products.begin() , accepted_truth_w_ele_products.end() );
          stlep.repurpose( "mu_rec" , "lepton reconstruction study" );
          stlep.for_each( accepted_truth_w_mu_products.begin() , accepted_truth_w_mu_products.end() );
        }
        if( true ) { 
          dg::down( "hww_leptons" , "hww lepton reconstruction" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
          StudyLeptonReconstruction<> stlep( "ele_rec" , "lepton reconstruction study" );
          stlep.reconstructed_electrons( ele_hww.begin() , ele_hww.end() );
          stlep.reconstructed_muons( mu_hww.begin() , mu_hww.end() );
          stlep.for_each( accepted_truth_w_ele_products.begin() , accepted_truth_w_ele_products.end() );
          stlep.repurpose( "mu_rec" , "lepton reconstruction study" );
          stlep.for_each( accepted_truth_w_mu_products.begin() , accepted_truth_w_mu_products.end() );      
        }
        if( false ) { 
          dg::down( "hww_leptons_wrt_probe" , "hww lepton reconstruction" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
          StudyLeptonReconstruction<> stlep( "ele_rec" , "lepton reconstruction study" );
          stlep.reconstructed_electrons( ele_hww.begin() , ele_hww.end() );
          stlep.reconstructed_muons( mu_hww.begin() , mu_hww.end() );
          stlep.for_each( ele_probe.begin() , ele_probe.end() );
          stlep.repurpose( "mu_rec" , "lepton reconstruction study" );
          stlep.for_each( mu_probe_staco.begin() , mu_probe_staco.end() );
        }
        if( true ) { 
          dg::down( "lowmh_leptons" , "lowmh lepton reconstruction" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
          StudyLeptonReconstruction<> stlep( "ele_rec" , "lepton reconstruction study" );
          stlep.reconstructed_electrons( ele_nontrigger_loose.begin() , ele_nontrigger_loose.end() );
          stlep.reconstructed_muons( mu_nontrigger_loose.begin() , mu_nontrigger_loose.end() );
          stlep.for_each( accepted_truth_w_ele_products.begin() , accepted_truth_w_ele_products.end() );
          stlep.repurpose( "mu_rec" , "lepton reconstruction study" );
          stlep.for_each( accepted_truth_w_mu_products.begin() , accepted_truth_w_mu_products.end() );      
        }
        if( false ) { 
          dg::down( "lowmh_leptons_wrt_probe" , "lowmh lepton reconstruction" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
          StudyLeptonReconstruction<> stlep( "ele_rec" , "lepton reconstruction study" );
          stlep.reconstructed_electrons( ele_nontrigger_loose.begin() , ele_nontrigger_loose.end() );
          stlep.reconstructed_muons( mu_nontrigger_loose.begin() , mu_nontrigger_loose.end() );
          stlep.for_each( ele_probe.begin() , ele_probe.end() );
          stlep.repurpose( "mu_rec" , "lepton reconstruction study" );
          stlep.for_each( mu_probe_staco.begin() , mu_probe_staco.end() );
        }
      }
    } // end if have higgs truth, do a bunch of stuff.

    // make higgs candidates out of the reconstructed leptons and MET.
    if( (is_good_run_ele || is_good_run_mu) &&
        (is_triggered_ele || is_triggered_mu) ) {
      dg::down( "higgs" , "higgs selection" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      // form all possible combinations from input leptons and MET.
      CompositeGenerator<AnaParticle,AnaHiggs> hgen;
      hgen.generate( lepton_hww.begin() , lepton_hww.end() ,
                     lepton_hww.begin() , lepton_hww.end() ,
                     met_hww.begin() , met_hww.end() ,
                     AnaHiggs::Compose() , hww_input );
      hgen.generate( lepton_lowmh.begin() , lepton_lowmh.end() ,
                     lepton_lowmh.begin() , lepton_lowmh.end() ,
                     met_hww.begin() , met_hww.end() ,
                     AnaHiggs::Compose() , hww_lowmh_input );
      hgen.generate( lepton_hww.begin() , lepton_hww.end() ,
                     lepton_probe.begin() , lepton_probe.end() ,
                     met_hww.begin() , met_hww.end() ,
                     AnaHiggs::Compose() , hww_tight_probe_input );
      // histogram all possible lepton combinations
      StudyHiggs sth( "probe_input_higgs" , "all input higgs combinations from probe lepton selection" );
      hww_peter_selector.for_each( hww_input.begin() , hww_input.end() );
      hww_lowmh_peter_selector.for_each( hww_lowmh_input.begin() , hww_lowmh_input.end() );
      hww_tight_probe_peter_selector.for_each( hww_tight_probe_input.begin() , hww_tight_probe_input.end() );
      // histogram number of surviving combinations
      dg::fillh( "n_input_expanded_probe" , 10 , 0 , 10 , hww_expanded_probe_input.size() );
      dg::fillh( "n_input_hww" , 10 , 0 , 10 , hww_input.size() );
      dg::fillh( "n_input_lowmh" , 10 , 0 , 10 , hww_lowmh_input.size() );
      dg::fillh( "n_hww" , 10 , 0 , 10 , hww.size() );
      dg::fillh( "n_lowmh" , 10 , 0 , 10 , hww_lowmh.size() );
      dg::fillh( "n_peter_probe" , 10 , 0 , 10 , hww_probe_peter.size() );
      dg::fillh( "n_peter_expanded_probe" , 10 , 0 , 10 , hww_expanded_probe_peter.size() );
      dg::fillh( "n_peter_tight_probe" , 10 , 0 , 10 , hww_tight_probe_peter.size() );
      dg::fillh( "n_peter_tight_expanded_probe" , 10 , 0 , 10 , hww_tight_expanded_probe_peter.size() );
      dg::fillh( "n_peter_hww" , 10 , 0 , 10 , hww_peter.size() );
      dg::fillh( "n_peter_lowmh" , 10 , 0 , 10 , hww_lowmh_peter.size() );
      // study surviving combinations
      StudyHiggs sthbest( "probe_peter_higgs" , "all peter higgs combinations from probe lepton selection" ); 
      sthbest.add_category( "all_electron" , 
                            bind( &AnaParticle::type , bind( &AnaHiggs::dilepton_leading_lepton , _1 ) ) == AnaParticle::ELECTRON &&
                            bind( &AnaParticle::type , bind( &AnaHiggs::dilepton_subleading_lepton , _1 ) ) == AnaParticle::ELECTRON );
      sthbest.add_category( "all_muon" , 
                            bind( &AnaParticle::type , bind( &AnaHiggs::dilepton_leading_lepton , _1 ) ) == AnaParticle::MUON &&
                            bind( &AnaParticle::type , bind( &AnaHiggs::dilepton_subleading_lepton , _1 ) ) == AnaParticle::MUON );
      sthbest.add_selector( "highest_pt_lep" , AnaComposite::HighestPtConstituents<const AnaHiggs>() );
      sthbest.for_each( hww_probe_peter.begin() , hww_probe_peter.end() );
      sthbest.repurpose( "tight_probe_peter_higgs" , "all peter higgs combinations from tight_probe lepton selection" );
      sthbest.for_each( hww_tight_probe_peter.begin() , hww_tight_probe_peter.end() );
      sthbest.repurpose( "hww_peter_higgs" , "all peter higgs combinations from hww lepton selection" );
      sthbest.for_each( hww_peter.begin() , hww_peter.end() );
      sthbest.repurpose( "lowmh_peter_higgs" , "all peter higgs combinations from lowmh lepton selection" );
      sthbest.for_each( hww_lowmh_peter.begin() , hww_lowmh_peter.end() );
    } // end study reconstructed higgs.
    
    // study data-style lepton reconstruction relative to probe
    if( (is_good_run_ele || is_good_run_mu) &&
        (is_triggered_ele || is_triggered_mu) ) {
      dg::down( "probe_lepton_efficiency" , "selector details" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      if( true ) { 
        dg::down( "lowmh_leptons_wrt_probe" , "lowmh lepton reconstruction" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
        StudyLeptonReconstruction<> stlep( "ele_rec" , "lepton reconstruction study" );
        stlep.reconstructed_electrons( ele_nontrigger_loose.begin() , ele_nontrigger_loose.end() );
        stlep.reconstructed_muons( mu_nontrigger_loose.begin() , mu_nontrigger_loose.end() );
        stlep.for_each( ele_probe.begin() , ele_probe.end() );
        stlep.repurpose( "mu_rec" , "lepton reconstruction study" );
        stlep.for_each( mu_probe_staco.begin() , mu_probe_staco.end() );
      }
      if( true ) { 
        dg::down( "hww_leptons_wrt_probe" , "hww lepton reconstruction" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
        StudyLeptonReconstruction<> stlep( "ele_rec" , "lepton reconstruction study" );
        stlep.reconstructed_electrons( ele_hww.begin() , ele_hww.end() );
        stlep.reconstructed_muons( mu_hww.begin() , mu_hww.end() );
        stlep.for_each( ele_probe.begin() , ele_probe.end() );
        stlep.repurpose( "mu_rec" , "lepton reconstruction study" );
        stlep.for_each( mu_probe_staco.begin() , mu_probe_staco.end() );
      }
    }


    if( (is_good_run_ele || is_good_run_mu) &&
        (is_triggered_ele || is_triggered_mu) ) {
      dg::down( "higgs" , "higgs selection" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      boost::shared_ptr<AnaCandidateEvent> candidate_event( new AnaCandidateEvent );
      candidate_event->electrons( ele_hww.begin() , ele_hww.end() );
      candidate_event->muons( mu_hww.begin() , mu_hww.end() );
      candidate_event->jets( jet_hww.begin() , jet_hww.end() );
      candidate_event->met( met_hww.back() ); // fixme
      candidate_event->trigger( evt->trigger() );
      // study surviving higgs combinations wrt the trigger
      StudyHiggs sthbest( "probe_peter_higgs" , "all peter higgs combinations from probe lepton selection" ); 
      sthbest.for_event( candidate_event );
      sthbest.add_constant_category( "is_trig_ele" , is_triggered_ele );
      sthbest.add_constant_category( "is_trig_mu" , is_triggered_mu );
      sthbest.add_constant_category( "is_trig_notelemu" , !is_triggered_ele && !is_triggered_mu );
      sthbest.for_each( hww_probe_peter.begin() , hww_probe_peter.end() );
      // sthbest.repurpose( "expanded_probe_peter_higgs" , "all peter higgs combinations from expanded_probe lepton selection" );
      // sthbest.for_each( hww_expanded_probe_peter.begin() , hww_expanded_probe_peter.end() );
      sthbest.repurpose( "hww_peter_higgs" , "all peter higgs combinations from hww lepton selection" );
      sthbest.for_each( hww_peter.begin() , hww_peter.end() );
      sthbest.repurpose( "lowmh_peter_higgs" , "all peter higgs combinations from lowmh lepton selection" );
      sthbest.for_each( hww_lowmh_peter.begin() , hww_lowmh_peter.end() );
      if( !hww_probe_peter.empty() ) {
        dg::filleff( "eff_probe_peter_trig_ele" , is_triggered_ele );
        dg::filleff( "eff_probe_peter_trig_mu" , is_triggered_mu );
        dg::filleff( "eff_probe_peter_trig_elemu" , is_triggered_ele && is_triggered_mu );
        dg::filleff( "eff_probe_peter_trig_none" , !is_triggered_ele && !is_triggered_mu );
        dg::filleff( "eff_probe_peter_trig_e15_medium" , is_good_triggered_e15_medium );
        dg::filleff( "eff_probe_peter_trig_e20_medium" , is_good_triggered_e20_medium );
        dg::filleff( "eff_probe_peter_trig_e25_medium" , is_good_triggered_e25_medium );
        dg::filleff( "eff_probe_peter_trig_e30_loose" , is_good_triggered_e30_loose );
        dg::filleff( "eff_probe_peter_trig_mu15_mg" , is_good_triggered_mu15_mg );
        dg::filleff( "eff_probe_peter_trig_mu20" , is_good_triggered_mu20 );
        dg::filleff( "eff_probe_peter_trig_2mu10_loose" , is_good_triggered_2mu10_loose );
        dg::filleff( "eff_probe_peter_trig_2e10_medium" , is_good_triggered_2e10_medium );
        dg::filleff( "eff_probe_peter_trig_e10_loose_mu6" , is_good_triggered_e10_loose_mu6 );
        if( !is_good_triggered_e20_medium && !is_good_triggered_mu20 ) {
          dg::filleff( "eff_probe_peter_trig_combined_nots20" , is_good_triggered_2mu10_loose || is_good_triggered_2e10_medium ||
                       is_good_triggered_e10_loose_mu6 );
        }
        // if( ngen_w_ele>=2 && ngen_w_mu==0 ) { dg::filleff( "eff_probe_peter_trig_ele_genee" , is_triggered_ele ); }
        // if( ngen_w_mu>=2 && ngen_w_ele==0 ) { dg::filleff( "eff_probe_peter_trig_mu_genmm" , is_triggered_mu ); }
        dg::filleff( "eff_probe_peter_clean_met" , ok_clean_met );
        dg::filleff( "eff_probe_peter_privertex" , !primary_vertices.empty() );
      }
      if( !hww_peter.empty() ) {
        dg::filleff( "eff_hww_peter_trig_ele" , is_triggered_ele );
        dg::filleff( "eff_hww_peter_trig_mu" , is_triggered_mu );
        dg::filleff( "eff_hww_peter_trig_elemu" , is_triggered_ele && is_triggered_mu );
        dg::filleff( "eff_hww_peter_trig_none" , !is_triggered_ele && !is_triggered_mu );
        dg::filleff( "eff_hww_peter_trig_e15_medium" , is_good_triggered_e15_medium );
        dg::filleff( "eff_hww_peter_trig_e20_medium" , is_good_triggered_e20_medium );
        dg::filleff( "eff_hww_peter_trig_e25_medium" , is_good_triggered_e25_medium );
        dg::filleff( "eff_hww_peter_trig_e30_loose" , is_good_triggered_e30_loose );
        dg::filleff( "eff_hww_peter_trig_mu15_mg" , is_good_triggered_mu15_mg );
        dg::filleff( "eff_hww_peter_trig_mu20" , is_good_triggered_mu20 );
        dg::filleff( "eff_hww_peter_trig_2mu10_loose" , is_good_triggered_2mu10_loose );
        dg::filleff( "eff_hww_peter_trig_2e10_medium" , is_good_triggered_2e10_medium );
        dg::filleff( "eff_hww_peter_trig_e10_loose_mu6" , is_good_triggered_e10_loose_mu6 );
        if( !is_good_triggered_e20_medium && !is_good_triggered_mu20 ) {
          dg::filleff( "eff_hww_peter_trig_combined_nots20" , is_good_triggered_2mu10_loose || is_good_triggered_2e10_medium ||
                       is_good_triggered_e10_loose_mu6 );
        }
        // if( ngen_w_ele>=2 && ngen_w_mu==0 ) { dg::filleff( "eff_hww_peter_trig_ele_genee" , is_triggered_ele ); }
        // if( ngen_w_mu>=2 && ngen_w_ele==0 ) { dg::filleff( "eff_hww_peter_trig_mu_genmm" , is_triggered_mu ); }
        dg::filleff( "eff_hww_peter_clean_met" , ok_clean_met );
        dg::filleff( "eff_hww_peter_privertex" , !primary_vertices.empty() );
      }
      if( !hww_lowmh_peter.empty() ) {
        dg::filleff( "eff_lowmh_peter_trig_ele" , is_triggered_ele );
        dg::filleff( "eff_lowmh_peter_trig_mu" , is_triggered_mu );
        dg::filleff( "eff_lowmh_peter_trig_elemu" , is_triggered_ele && is_triggered_mu );
        dg::filleff( "eff_lowmh_peter_trig_none" , !is_triggered_ele && !is_triggered_mu );
        dg::filleff( "eff_lowmh_peter_trig_e15_medium" , is_good_triggered_e15_medium );
        dg::filleff( "eff_lowmh_peter_trig_e20_medium" , is_good_triggered_e20_medium );
        dg::filleff( "eff_lowmh_peter_trig_e25_medium" , is_good_triggered_e25_medium );
        dg::filleff( "eff_lowmh_peter_trig_e30_loose" , is_good_triggered_e30_loose );
        dg::filleff( "eff_lowmh_peter_trig_mu15_mg" , is_good_triggered_mu15_mg );
        dg::filleff( "eff_lowmh_peter_trig_mu20" , is_good_triggered_mu20 );
        dg::filleff( "eff_lowmh_peter_trig_2mu10_loose" , is_good_triggered_2mu10_loose );
        dg::filleff( "eff_lowmh_peter_trig_2e10_medium" , is_good_triggered_2e10_medium );
        dg::filleff( "eff_lowmh_peter_trig_e10_loose_mu6" , is_good_triggered_e10_loose_mu6 );
        if( !is_good_triggered_e20_medium && !is_good_triggered_mu20 ) {
          dg::filleff( "eff_lowmh_peter_trig_combined_nots20" , is_good_triggered_2mu10_loose || is_good_triggered_2e10_medium ||
                       is_good_triggered_e10_loose_mu6 );
        }
        // if( ngen_w_ele>=2 && ngen_w_mu==0 ) { dg::filleff( "eff_lowmh_peter_trig_ele_genee" , is_triggered_ele ); }
        // if( ngen_w_mu>=2 && ngen_w_ele==0 ) { dg::filleff( "eff_lowmh_peter_trig_mu_genmm" , is_triggered_mu ); }
        dg::filleff( "eff_lowmh_peter_clean_met" , ok_clean_met );
        dg::filleff( "eff_lowmh_peter_privertex" , !primary_vertices.empty() );
      }
    } // study higgs in GRL+triggered events


    // test XML wrapper output
    // save event if it has a W candidate
    if( AnaEventMgr::instance()->ana_event_ntuple_mode() == AnaEventNtuple::WRITE ) {
      AnaEventMgr::instance()->add_to_ana_event_ntuple( true );
    }
    
    // // build composites: W's, Z's, H's
    // // ================================================================

    // build lists of W's
    CompositeGenerator<AnaParticle,AnaW> wgen;
    wgen.generate( ele_hww.begin() , ele_hww.end() , met_hww.begin() , met_hww.end() , AnaW::Compose() , enu_hww );
    wgen.generate( mu_hww.begin() , mu_hww.end() , met_hww.begin() , met_hww.end() , AnaW::Compose() , munu_hww );
    wgen.generate( mu_hww_preiso.begin() , mu_hww_preiso.end() , met_hww.begin() , met_hww.end() , AnaW::Compose() , munu_hww_preiso );
    wgen.generate( mu_hww_lowpt_preiso.begin() , mu_hww_lowpt_preiso.end() , met_hww.begin() , met_hww.end() , AnaW::Compose() , munu_hww_lowpt_preiso );
    wgen.generate( ele_probe.begin() , ele_probe.end() , met_hww.begin() , met_hww.end() , AnaW::Compose() , enu_probe );
    wgen.generate( mu_probe_staco.begin() , mu_probe_staco.end() , met_hww.begin() , met_hww.end() , AnaW::Compose() , munu_probe );

    // build lists of Z's
    CompositeGenerator<AnaParticle,AnaZ> zgen;
    zgen.generate( ele_hww.begin() , ele_hww.end() , ele_hww.begin() , ele_hww.end() , AnaZ::Compose() , ee_hww );
    zgen.generate( mu_hww.begin() , mu_hww.end() , mu_hww.begin() , mu_hww.end() , AnaZ::Compose() , mumu_hww );
    zgen.generate( mu_hww_lowpt.begin() , mu_hww_lowpt.end() , mu_hww_lowpt.begin() , mu_hww_lowpt.end() , AnaZ::Compose() , mumu_hww_lowpt );
    zgen.generate( mu_hww_preiso.begin() , mu_hww_preiso.end() , mu_hww_preiso.begin() , mu_hww_preiso.end() , AnaZ::Compose() , mumu_hww_preiso );
    zgen.generate( mu_hww_lowpt_preiso.begin() , mu_hww_lowpt_preiso.end() , mu_hww_lowpt_preiso.begin() , mu_hww_lowpt_preiso.end() , AnaZ::Compose() , mumu_hww_lowpt_preiso );
    zgen.generate( ele_probe.begin() , ele_probe.end() , ele_probe.begin() , ele_probe.end() , AnaZ::Compose() , ee_probe );
    zgen.generate( mu_probe_staco.begin() , mu_probe_staco.end() , mu_probe_staco.begin() , mu_probe_staco.end() , AnaZ::Compose() , mumu_probe_staco );

    zgen.generate( ele_nontrigger_loose.begin() , ele_nontrigger_loose.end() , ele_nontrigger_loose.begin() , ele_nontrigger_loose.end() , AnaZ::Compose() , ee_lowmh );
    zgen.generate( mu_nontrigger_loose.begin() , mu_nontrigger_loose.end() , mu_nontrigger_loose.begin() , mu_nontrigger_loose.end() , AnaZ::Compose() , mumu_nontrigger_loose );

    zgen.generate( ele_hww.begin() , ele_hww.end() , ele_probe.begin() , ele_probe.end() , AnaZ::Compose() , ee_tagprobe );
    zgen.generate( mu_hww.begin() , mu_hww.end() , mu_probe_staco.begin() , mu_probe_staco.end() , AnaZ::Compose() , mumu_tagprobe );
    // zgen.generate( ele_hww.begin() , ele_hww.end() , ele_expanded_probe.begin() , ele_expanded_probe.end() , AnaZ::Compose() , ee_tagprobe_expanded );
    // zgen.generate( mu_hww.begin() , mu_hww.end() , mu_expanded_probe.begin() , mu_expanded_probe.end() , AnaZ::Compose() , mumu_tagprobe_expanded );
    
    // anti-selected leptons
    zgen.generate( ele_hww.begin() , ele_hww.end() , ele_anti.begin() , ele_anti.end() , AnaZ::Compose() , ee_taganti );
    zgen.generate( mu_hww.begin() , mu_hww.end() , mu_anti.begin() , mu_anti.end() , AnaZ::Compose() , mumu_taganti );

    // ================================================================
    // HSG3 W+jets fakes studies
    // ================================================================
    // require gamma+jet GRL (good for egamma+jet+triggers) as well as
    // analysis GRL (since we're measuring analysis fake rates, we
    // don't want bad-run junk)
    if( grl.is_in( GoodRunList::GAMMAJET , evt->run_number() , evt->lumi_block() ) &&
        grl.is_in( GoodRunList::MUON , evt->run_number() , evt->lumi_block() ) ) { 
      // check for W or Z candidates
      std::for_each( munu_hww_lowpt_preiso.begin() , munu_hww_lowpt_preiso.end() ,
                     bind( &dg::fillh , "munu_hww_lowpt_preiso_mt" , 120 , 0 , 120 , 
                           bind( &AnaW::transverse_mass , _1 ) ) );
      std::for_each( munu_hww_lowpt_preiso.begin() , munu_hww_lowpt_preiso.end() ,
                     bind( &dg::fillh , "mumu_hww_lowpt_preiso_mass" , 120 , 0 , 120 , 
                           bind( &AnaZ::mass , _1 ) ) );
      const bool has_w_cand = ( find_if(munu_hww_lowpt_preiso.begin(),munu_hww_lowpt_preiso.end(),
                                        bind(&AnaMET::pt,bind(&AnaW::met,_1)) > 30. ) != munu_hww_lowpt_preiso.end() );
      const bool has_z_cand = ( find_if(mumu_hww_lowpt_preiso.begin(),mumu_hww_lowpt_preiso.end(),
                                        bind( std::logical_and<bool>() ,
                                              bind( std::logical_and<bool>() ,
                                                    bind(&AnaZ::mass,_1) > 76 ,
                                                    bind(&AnaZ::mass,_1) < 106. ) ,
                                              bind( AnaComposite::IsOppositeSignPair() , _1 ) ) 
                                        ) != mumu_hww_lowpt_preiso.end() );
      // check for photons
      StudyParticle stp( "tight_photon" , "tight photon" );
      stp.add_selector( "leading_pt" , AnaParticle::HighestPt<const AnaParticle>() );
      stp.for_each( tight_photon.begin() , tight_photon.end() );
      // check triggers
      const bool fired_j10 = trigger->passes("EF_j10_a4tc_EFFS"); // prescale of 2.5M
      const bool fired_g20 = trigger->passes("EF_g20_loose"); // prescale of 30
      dg::filleff( "fired_j10" , fired_j10 );
      dg::filleff( "fired_g20" , fired_g20 );
      // if W/Z veto satisfied and 10gev jet trigger (prescaled by 2.5M), this is the dijet sample
      if( !has_w_cand && !has_z_cand && fired_j10 ) {
        dg::down( "dijet_fake_region" , "selector details" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
        std::for_each( munu_hww_lowpt_preiso.begin() , munu_hww_lowpt_preiso.end() ,
                       bind( &dg::fillh , "munu_hww_lowpt_preiso_mt" , 120 , 0 , 120 , 
                             bind( &AnaW::transverse_mass , _1 ) ) );
        std::for_each( munu_hww_lowpt_preiso.begin() , munu_hww_lowpt_preiso.end() ,
                       bind( &dg::fillh , "mumu_hww_lowpt_preiso_mass" , 120 , 0 , 120 , 
                             bind( &AnaZ::mass , _1 ) ) );
        StudyMuonFake stf( "dijet_fake_hww" , "fake rate from W+jets dijet sample" );
        stf.set_cut( mu_hww_selector );
        stf.for_each( mu_hww_preiso.begin() , mu_hww_preiso.end() );
        stf.repurpose( "dijet_fake_hww_lowpt" , "fake rate from W+jets dijet sample" );
        stf.set_cut( mu_hww_lowpt_selector );
        stf.for_each( mu_hww_lowpt_preiso.begin() , mu_hww_lowpt_preiso.end() );
      }

      if( fired_g20 && !tight_photon.empty() ) { // prescale of 30
        shared_ptr<const AnaPhoton> leading_ph( *std::max_element(tight_photon.begin(),tight_photon.end(),bind(&AnaPhoton::pt,_1)<bind(&AnaPhoton::pt,_2)) );
        // consider denominator back to back with leading photon
        StudyMuonFake stf( "gammajet_fake_hww" , "fake rate from W+jets gamma jet sample" );
        stf.study_fakeable( true );
        stf.set_fakeable_cut( bind<bool>( detector::abs_delta_phi<AnaPhoton::ftype> , bind(&AnaMuon::phi,_1) , leading_ph->phi() ) > (M_PI-0.3) );
        stf.set_cut( mu_hww_selector );
        stf.for_each( mu_hww_preiso.begin() , mu_hww_preiso.end() );
        stf.repurpose( "gammajet_fake_hww_lowpt" , "fake rate from W+jets gamma jet sample" );
        stf.set_cut( mu_hww_lowpt_selector );
        stf.for_each( mu_hww_lowpt_preiso.begin() , mu_hww_lowpt_preiso.end() );        
      }
    }


    // LBL fake rate pairs (as in x-devonthink-item://E7953DC5-8E86-4079-B260-DA4A6139CC71)
    // construct predicate to satisfy remaining LBL requirements of pairs:
    //   - one muon with pT > 20 GeV that is matched to EF trigger object with DR<0.15
    if( is_good_run_ele || is_good_run_mu ) {
      AnaMuon::IsTriggerMatch match_2mu10("EF_2mu10_loose",0.15);
      AnaMuon::IsTriggerMatch match_mu6("EF_mu6",0.15);
      AnaMuon::IsTriggerMatch match_mu18("EF_mu18_MG",0.15);
      AnaMuon::IsTriggerMatch match_mu40("EF_mu40_MSonly_barrel",0.15);
      
      AnaComposite::AtLeastOnePasses<const AnaMuon> oneEFmatch_2mu10;
      oneEFmatch_2mu10.add_cut( bind( &AnaTrigger::ef_2mu10_loose , bind(&AnaEvent::trigger,boost::ref(AnaEventMgr::current_event())) ) == true );
      oneEFmatch_2mu10.add_cut( bind(match_2mu10,_1) );
      oneEFmatch_2mu10.add_cut( bind(&AnaMuon::pt,_1) > 20. );
      //
      AnaComposite::AtLeastOnePasses<const AnaMuon> oneEFmatch_mu6;
      oneEFmatch_mu6.add_cut( bind( &AnaTrigger::ef_mu6 , bind(&AnaEvent::trigger,boost::ref(AnaEventMgr::current_event())) ) == true );
      oneEFmatch_mu6.add_cut( bind(match_mu6,_1) );
      oneEFmatch_mu6.add_cut( bind(&AnaMuon::pt,_1) > 20. );
      //
      AnaComposite::AtLeastOnePasses<const AnaMuon> oneEFmatch_mu18or40;
      oneEFmatch_mu18or40.add_cut( bind<bool>( std::logical_or<bool>() ,
                                               bind( &AnaTrigger::ef_mu18_mg , bind(&AnaEvent::trigger,boost::ref(AnaEventMgr::current_event())) ) == true ,
                                               bind( &AnaTrigger::ef_mu40_msonly_barrel , bind(&AnaEvent::trigger,boost::ref(AnaEventMgr::current_event())) ) == true ) );
      oneEFmatch_mu18or40.add_cut( bind<bool>( std::logical_or<bool>() , 
                                               bind(match_mu18,_1) ,
                                               bind(match_mu40,_1) ) );
      oneEFmatch_mu18or40.add_cut( bind(&AnaMuon::pt,_1) > 20. );
      //
      CompositeGenerator<AnaParticle,AnaComposite> cgen;
      {
        cgen.generate( mu_lbl_preiso.begin() , mu_lbl_preiso.end() , mu_lbl_preiso.begin() , mu_lbl_preiso.end() ,
                       AnaComposite::Compose() , mumu_lbl_preiso_as );
        //
        cgen.set_composite_predicate( bind(oneEFmatch_2mu10,_1) );
        cgen.generate( mu_lbl_preiso.begin() , mu_lbl_preiso.end() , mu_lbl_preiso.begin() , mu_lbl_preiso.end() ,
                       AnaComposite::Compose() , mumu_lbl_preiso_2mu10_as );
        //
        cgen.set_composite_predicate( bind(oneEFmatch_mu6,_1) );
        cgen.generate( mu_lbl_preiso.begin() , mu_lbl_preiso.end() , mu_lbl_preiso.begin() , mu_lbl_preiso.end() ,
                       AnaComposite::Compose() , mumu_lbl_preiso_mu6_as );
        //
        cgen.set_composite_predicate( bind(oneEFmatch_mu18or40,_1) );
        cgen.generate( mu_lbl_preiso.begin() , mu_lbl_preiso.end() , mu_lbl_preiso.begin() , mu_lbl_preiso.end() ,
                       AnaComposite::Compose() , mumu_lbl_preiso_mu18or40_as );     
      }
      {
        cgen.set_pair_predicate( AnaComposite::IsOppositeSignPair() );
        //
        cgen.generate( mu_lbl_preiso.begin() , mu_lbl_preiso.end() , mu_lbl_preiso.begin() , mu_lbl_preiso.end() ,
                       AnaComposite::Compose() , mumu_lbl_preiso_os );
        //
        cgen.set_composite_predicate( bind(oneEFmatch_2mu10,_1) );
        cgen.generate( mu_lbl_preiso.begin() , mu_lbl_preiso.end() , mu_lbl_preiso.begin() , mu_lbl_preiso.end() ,
                       AnaComposite::Compose() , mumu_lbl_preiso_2mu10_os );
        //
        cgen.set_composite_predicate( bind(oneEFmatch_mu6,_1) );
        cgen.generate( mu_lbl_preiso.begin() , mu_lbl_preiso.end() , mu_lbl_preiso.begin() , mu_lbl_preiso.end() ,
                       AnaComposite::Compose() , mumu_lbl_preiso_mu6_os );
        //
        cgen.set_composite_predicate( bind(oneEFmatch_mu18or40,_1) );
        cgen.generate( mu_lbl_preiso.begin() , mu_lbl_preiso.end() , mu_lbl_preiso.begin() , mu_lbl_preiso.end() ,
                       AnaComposite::Compose() , mumu_lbl_preiso_mu18or40_os );     
      }
      {
        cgen.set_pair_predicate( AnaComposite::IsSameSignPair() );
        //
        cgen.generate( mu_lbl_preiso.begin() , mu_lbl_preiso.end() , mu_lbl_preiso.begin() , mu_lbl_preiso.end() ,
                       AnaComposite::Compose() , mumu_lbl_preiso_ss );
        //
        cgen.set_composite_predicate( bind(oneEFmatch_2mu10,_1) );
        cgen.generate( mu_lbl_preiso.begin() , mu_lbl_preiso.end() , mu_lbl_preiso.begin() , mu_lbl_preiso.end() ,
                       AnaComposite::Compose() , mumu_lbl_preiso_2mu10_ss );
        //
        cgen.set_composite_predicate( bind(oneEFmatch_mu6,_1) );
        cgen.generate( mu_lbl_preiso.begin() , mu_lbl_preiso.end() , mu_lbl_preiso.begin() , mu_lbl_preiso.end() ,
                       AnaComposite::Compose() , mumu_lbl_preiso_mu6_ss );
        //
        cgen.set_composite_predicate( bind(oneEFmatch_mu18or40,_1) );
        cgen.generate( mu_lbl_preiso.begin() , mu_lbl_preiso.end() , mu_lbl_preiso.begin() , mu_lbl_preiso.end() ,
                       AnaComposite::Compose() , mumu_lbl_preiso_mu18or40_ss );     
      }
      // ----------------------------------------------------------------
      // the isolation cut:
      // ----------------------------------------------------------------
      DgCutSet< shared_ptr<const AnaMuon> > tight_mu_cut( "tight_mu_cut" );
      tight_mu_cut.add_uu( "pt_cone_40" , bind(&AnaMuon::pt_cone,_1,AnaMuon::CONE40) < 5. , 
                           bind(&dg::fillh, _1 , 100 , 0. , 50. , bind(&AnaMuon::pt_cone,_2,AnaMuon::CONE40) ) );
      tight_mu_cut.add_uu( "pt_cone_40_ratio" , bind(&AnaMuon::pt_cone_ratio,_1,AnaMuon::CONE40) < 0.1 , 
                           bind(&dg::fillh, _1 , 100 , 0. , 1. , bind(&AnaMuon::pt_cone_ratio,_2,AnaMuon::CONE40) ) );
      // ----------------------------------------------------------------
      // validation of all this junk
      // ----------------------------------------------------------------
      {
        cout << " ---- event ---------------- " << evt->run_number() << " " << evt->event_number() << endl;
        cout << "  muons " << endl;
        BOOST_FOREACH( shared_ptr<const AnaMuon>& mu , mu_event ) {
          if( ! mu->is_staco_combined() ) { continue; }
          if( ! mu->abseta()>2.4 ) { continue; }
          cout << ( format( " mu pt %|5.2f| d0 %|5.2f| d0sf %|5.2f| z0s %|5.2f| iso %|5.2f| riso %|5.2f| %|65t| sc %|1d| spd %|1d| sp%|1d| sn %|1d| sl %|1d| st %|1d| m2684 %|1d|%|1d|%|1d|%|1d|" )
                    % mu->pt()
                    % mu->d0_exPV()
                    % mu->d0_exPV_signif()
                    % std::abs(mu->z0_exPV_sin())
                    % mu->pt_cone(AnaMuon::CONE40)
                    % mu->pt_cone_ratio(AnaMuon::CONE40)
                    % mu_lbl_common_selector.passes_all( mu )
                    % mu_lbl_preiso_pred0sig_selector.passes_all( mu )
                    % mu_lbl_preiso_selector.passes_all( mu )
                    % mu_lbl_nonprompt_preiso_selector.passes_all( mu )
                    % mu_lbl_loose_selector.passes_all( mu )
                    % mu_lbl_tight_selector.passes_all( mu )
                    % match_2mu10(mu)
                    % match_mu6(mu)
                    % match_mu18(mu)
                    % match_mu40(mu)
                    ) << endl;
        }
      }
      // ================================================================
      //  matrix method fake studies
      // ================================================================
      {
        dg::down( "matrix_method" , "metrix method results" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
        // check inputs
        StudyComposite stp( "mumu_all_os" , "dimuon kinematics" );
        stp.add_category( "match_2mu10" , oneEFmatch_2mu10 );
        stp.add_category( "match_mu6" , oneEFmatch_mu6 );
        stp.add_category( "match_mu18or40" , oneEFmatch_mu18or40 );
        stp.for_each( mumu_lbl_preiso_os.begin() , mumu_lbl_preiso_os.end() );
        stp.repurpose( "mumu_2mu10_os" , "dimuon kinematics" );
        stp.for_each( mumu_lbl_preiso_2mu10_os.begin() , mumu_lbl_preiso_2mu10_os.end() );
        stp.repurpose( "mumu_mu6_os" , "dimuon kinematics" );
        stp.for_each( mumu_lbl_preiso_mu6_os.begin() , mumu_lbl_preiso_mu6_os.end() );
        stp.repurpose( "mumu_mu18or40_os" , "dimuon kinematics" );
        stp.for_each( mumu_lbl_preiso_mu18or40_os.begin() , mumu_lbl_preiso_mu18or40_os.end() );
        //
        stp.repurpose( "mumu_all_ss" , "dimuon kinematics" );
        stp.for_each( mumu_lbl_preiso_ss.begin() , mumu_lbl_preiso_ss.end() );
        stp.repurpose( "mumu_2mu10_ss" , "dimuon kinematics" );
        stp.for_each( mumu_lbl_preiso_2mu10_ss.begin() , mumu_lbl_preiso_2mu10_ss.end() );
        stp.repurpose( "mumu_mu6_ss" , "dimuon kinematics" );
        stp.for_each( mumu_lbl_preiso_mu6_ss.begin() , mumu_lbl_preiso_mu6_ss.end() );
        stp.repurpose( "mumu_mu18or40_ss" , "dimuon kinematics" );
        stp.for_each( mumu_lbl_preiso_mu18or40_ss.begin() , mumu_lbl_preiso_mu18or40_ss.end() );
        //
        stp.repurpose( "mumu_all_as" , "dimuon kinematics" );
        stp.for_each( mumu_lbl_preiso_as.begin() , mumu_lbl_preiso_as.end() );
        stp.repurpose( "mumu_2mu10_as" , "dimuon kinematics" );
        stp.for_each( mumu_lbl_preiso_2mu10_as.begin() , mumu_lbl_preiso_2mu10_as.end() );
        stp.repurpose( "mumu_mu6_as" , "dimuon kinematics" );
        stp.for_each( mumu_lbl_preiso_mu6_as.begin() , mumu_lbl_preiso_mu6_as.end() );
        stp.repurpose( "mumu_mu18or40_as" , "dimuon kinematics" );
        stp.for_each( mumu_lbl_preiso_mu18or40_as.begin() , mumu_lbl_preiso_mu18or40_as.end() );
        // compute fake rates ( tight/(tight+!tight) ) from pre-isolated
        // muon samples derived from the four LBL SS trigger choices. 
        //   1) any pre-isolated muon pair with 15--55 GeV dimuon mass?
        //   2) construct denominator
        //   3) do fake rate study
        // ----------------------------------------------------------------
        // EF 2mu10 loose OS
        {
          const bool has_dimuon_pair = std::find_if( mumu_lbl_preiso_2mu10_os.begin() , mumu_lbl_preiso_2mu10_os.end() ,
                                                     bind( AnaParticle::MassInWindow<const AnaParticle>(15.,55.) , _1 ) ) != mumu_lbl_preiso_2mu10_os.end();
          dg::filleff( "has_fakeA_2mu10_os" , has_dimuon_pair );
          if( has_dimuon_pair ) { 
            StudyMuonFake stf( "fakeA_2mu10_os" , "fake rate from LBL EF_2mu10 OS sample" );
            stf.set_cut( tight_mu_cut );
            stf.study_n_objects( true );
            stf.for_each( mu_lbl_nonprompt_preiso.begin() , mu_lbl_nonprompt_preiso.end() );
          }
        }
        // EF mu6 OS
        {
          const bool has_dimuon_pair = std::find_if( mumu_lbl_preiso_mu6_os.begin() , mumu_lbl_preiso_mu6_os.end() ,
                                                     bind( AnaParticle::MassInWindow<const AnaParticle>(15.,55.) , _1 ) ) != mumu_lbl_preiso_mu6_os.end();
          const bool single_trigger_lepton = ( !has_dimuon_pair && mu_lbl_preiso.size()==1 && match_mu6(mu_lbl_preiso.back()) );
          //
          dg::filleff( "has_fakeB_mu6_os_pair" , has_dimuon_pair );
          dg::filleff( "has_fakeB_mu6_os_trig" , single_trigger_lepton );
          dg::filleff( "has_fakeB_mu6_os" , has_dimuon_pair || single_trigger_lepton );
          if( has_dimuon_pair || single_trigger_lepton ) { 
            StudyMuonFake stf( "fakeB_mu6_os" , "fake rate from LBL EF_mu6 OS sample" );
            stf.set_cut( tight_mu_cut );
            stf.study_n_objects( true );
            stf.for_each( mu_lbl_nonprompt_preiso.begin() , mu_lbl_nonprompt_preiso.end() );
          }
        }
        // mu18 || mu40 OS
        {
          const bool has_dimuon_pair = std::find_if( mumu_lbl_preiso_mu18or40_os.begin() , mumu_lbl_preiso_mu18or40_os.end() ,
                                                     bind( AnaParticle::MassInWindow<const AnaParticle>(15.,55.) , _1 ) ) != mumu_lbl_preiso_mu18or40_os.end();
          const bool single_trigger_lepton = ( !has_dimuon_pair && mu_lbl_preiso.size()==1 && 
                                               (match_mu18(mu_lbl_preiso.back()) || match_mu40(mu_lbl_preiso.back())) );
          dg::filleff( "has_fakeC_mu18or40_os_pair" , has_dimuon_pair );
          dg::filleff( "has_fakeC_mu18or40_os_trig" , single_trigger_lepton );
          dg::filleff( "has_fakeC_mu18or40_os" , has_dimuon_pair || single_trigger_lepton );
          if( has_dimuon_pair || single_trigger_lepton ) { 
            StudyMuonFake stf( "fakeC_mu18or40_os" , "fake rate from LBL EF_mu18or40 OS sample" );
            stf.set_cut( tight_mu_cut );
            stf.study_n_objects( true );
            stf.for_each( mu_lbl_nonprompt_preiso.begin() , mu_lbl_nonprompt_preiso.end() );
          }
        }
        // EF 2mu10 loose SS
        {
          const bool has_dimuon_pair = std::find_if( mumu_lbl_preiso_2mu10_ss.begin() , mumu_lbl_preiso_2mu10_ss.end() ,
                                                     bind( AnaParticle::MassInWindow<const AnaParticle>(15.,55.) , _1 ) ) != mumu_lbl_preiso_2mu10_ss.end();
          dg::filleff( "has_fakeA_2mu10_ss" , has_dimuon_pair );
          if( has_dimuon_pair ) { 
            StudyMuonFake stf( "fakeA_2mu10_ss" , "fake rate from LBL EF_2mu10 SS sample" );
            stf.set_cut( tight_mu_cut );
            stf.study_n_objects( true );
            stf.for_each( mu_lbl_nonprompt_preiso.begin() , mu_lbl_nonprompt_preiso.end() );
          }
        }
        // EF mu6 SS
        {
          const bool has_dimuon_pair = std::find_if( mumu_lbl_preiso_mu6_ss.begin() , mumu_lbl_preiso_mu6_ss.end() ,
                                                     bind( AnaParticle::MassInWindow<const AnaParticle>(15.,55.) , _1 ) ) != mumu_lbl_preiso_mu6_ss.end();
          const bool single_trigger_lepton = ( !has_dimuon_pair && mu_lbl_preiso.size()==1 && match_mu6(mu_lbl_preiso.back()) );
          //
          dg::filleff( "has_fakeB_mu6_ss_pair" , has_dimuon_pair );
          dg::filleff( "has_fakeB_mu6_ss_trig" , single_trigger_lepton );
          dg::filleff( "has_fakeB_mu6_ss" , has_dimuon_pair || single_trigger_lepton );
          if( has_dimuon_pair || single_trigger_lepton ) { 
            StudyMuonFake stf( "fakeB_mu6_ss" , "fake rate from LBL EF_mu6 SS sample" );
            stf.set_cut( tight_mu_cut );
            stf.study_n_objects( true );
            stf.for_each( mu_lbl_nonprompt_preiso.begin() , mu_lbl_nonprompt_preiso.end() );
          }
        }
        // mu18 || mu40 SS
        {
          const bool has_dimuon_pair = std::find_if( mumu_lbl_preiso_mu18or40_ss.begin() , mumu_lbl_preiso_mu18or40_ss.end() ,
                                                     bind( AnaParticle::MassInWindow<const AnaParticle>(15.,55.) , _1 ) ) != mumu_lbl_preiso_mu18or40_ss.end();
          const bool single_trigger_lepton = ( !has_dimuon_pair && mu_lbl_preiso.size()==1 && 
                                               (match_mu18(mu_lbl_preiso.back()) || match_mu40(mu_lbl_preiso.back())) );
          dg::filleff( "has_fakeC_mu18or40_ss_pair" , has_dimuon_pair );
          dg::filleff( "has_fakeC_mu18or40_ss_trig" , single_trigger_lepton );
          dg::filleff( "has_fakeC_mu18or40_ss" , has_dimuon_pair || single_trigger_lepton );
          if( has_dimuon_pair || single_trigger_lepton ) { 
            StudyMuonFake stf( "fakeC_mu18or40_ss" , "fake rate from LBL EF_mu18or40 SS sample" );
            stf.set_cut( tight_mu_cut );
            stf.study_n_objects( true );
            stf.for_each( mu_lbl_nonprompt_preiso.begin() , mu_lbl_nonprompt_preiso.end() );
          }
        }
        // EF 2mu10 loose AS (any-sign)
        {
          const bool has_dimuon_pair = std::find_if( mumu_lbl_preiso_2mu10_as.begin() , mumu_lbl_preiso_2mu10_as.end() ,
                                                     bind( AnaParticle::MassInWindow<const AnaParticle>(15.,55.) , _1 ) ) != mumu_lbl_preiso_2mu10_as.end();
          dg::filleff( "has_fakeA_2mu10_as" , has_dimuon_pair );
          if( has_dimuon_pair ) { 
            StudyMuonFake stf( "fakeA_2mu10_as" , "fake rate from LBL EF_2mu10 AS sample" );
            stf.set_cut( tight_mu_cut );
            stf.study_n_objects( true );
            stf.for_each( mu_lbl_nonprompt_preiso.begin() , mu_lbl_nonprompt_preiso.end() );
          }
        }
        // EF mu6 AS
        {
          const bool has_dimuon_pair = std::find_if( mumu_lbl_preiso_mu6_as.begin() , mumu_lbl_preiso_mu6_as.end() ,
                                                     bind( AnaParticle::MassInWindow<const AnaParticle>(15.,55.) , _1 ) ) != mumu_lbl_preiso_mu6_as.end();
          const bool single_trigger_lepton = ( !has_dimuon_pair && mu_lbl_preiso.size()==1 && match_mu6(mu_lbl_preiso.back()) );
          //
          dg::filleff( "has_fakeB_mu6_as_pair" , has_dimuon_pair );
          dg::filleff( "has_fakeB_mu6_as_trig" , single_trigger_lepton );
          dg::filleff( "has_fakeB_mu6_as" , has_dimuon_pair || single_trigger_lepton );
          if( has_dimuon_pair || single_trigger_lepton ) { 
            StudyMuonFake stf( "fakeB_mu6_as" , "fake rate from LBL EF_mu6 AS sample" );
            stf.set_cut( tight_mu_cut );
            stf.study_n_objects( true );
            stf.for_each( mu_lbl_nonprompt_preiso.begin() , mu_lbl_nonprompt_preiso.end() );
          }
        }
        // mu18 || mu40 AS
        {
          const bool has_dimuon_pair = std::find_if( mumu_lbl_preiso_mu18or40_as.begin() , mumu_lbl_preiso_mu18or40_as.end() ,
                                                     bind( AnaParticle::MassInWindow<const AnaParticle>(15.,55.) , _1 ) ) != mumu_lbl_preiso_mu18or40_as.end();
          const bool single_trigger_lepton = ( !has_dimuon_pair && mu_lbl_preiso.size()==1 && 
                                               (match_mu18(mu_lbl_preiso.back()) || match_mu40(mu_lbl_preiso.back())) );
          dg::filleff( "has_fakeC_mu18or40_as_pair" , has_dimuon_pair );
          dg::filleff( "has_fakeC_mu18or40_as_trig" , single_trigger_lepton );
          dg::filleff( "has_fakeC_mu18or40_as" , has_dimuon_pair || single_trigger_lepton );
          if( has_dimuon_pair || single_trigger_lepton ) { 
            StudyMuonFake stf( "fakeC_mu18or40_as" , "fake rate from LBL EF_mu18or40 AS sample" );
            stf.set_cut( tight_mu_cut );
            stf.study_n_objects( true );
            stf.for_each( mu_lbl_nonprompt_preiso.begin() , mu_lbl_nonprompt_preiso.end() );
          }
        }
        // all (e.g. for jet et sample)
        {
          const bool has_dimuon_pair = std::find_if( mumu_lbl_preiso_os.begin() , mumu_lbl_preiso_os.end() ,
                                                     bind( AnaParticle::MassInWindow<const AnaParticle>(15.,55.) , _1 ) ) != mumu_lbl_preiso_os.end();
          dg::filleff( "has_fake_all_os_pair" , has_dimuon_pair );
          if( has_dimuon_pair ) { 
            StudyMuonFake stf( "fake_all_os" , "fake rate from dimuon OS sample" );
            stf.set_cut( tight_mu_cut );
            stf.study_n_objects( true );
            stf.for_each( mu_lbl_nonprompt_preiso.begin() , mu_lbl_nonprompt_preiso.end() );
          }
        }
        {
          const bool has_dimuon_pair = std::find_if( mumu_lbl_preiso_ss.begin() , mumu_lbl_preiso_ss.end() ,
                                                     bind( AnaParticle::MassInWindow<const AnaParticle>(15.,55.) , _1 ) ) != mumu_lbl_preiso_ss.end();
          if( has_dimuon_pair ) { 
            StudyMuonFake stf( "fake_all_ss" , "fake rate from dimuon SS sample" );
            stf.set_cut( tight_mu_cut );
            stf.study_n_objects( true );
            stf.for_each( mu_lbl_nonprompt_preiso.begin() , mu_lbl_nonprompt_preiso.end() );
          }
        }
        {
          const bool has_dimuon_pair = std::find_if( mumu_lbl_preiso_as.begin() , mumu_lbl_preiso_as.end() ,
                                                     bind( AnaParticle::MassInWindow<const AnaParticle>(15.,55.) , _1 ) ) != mumu_lbl_preiso_as.end();
          if( has_dimuon_pair ) { 
            StudyMuonFake stf( "fake_all_as" , "fake rate from dimuon AS sample" );
            stf.set_cut( tight_mu_cut );
            stf.study_n_objects( true );
            stf.for_each( mu_lbl_nonprompt_preiso.begin() , mu_lbl_nonprompt_preiso.end() );
          }
        }
        // all nonprompt muons
        {
          StudyMuonFake stf( "fake_all" , "fake rate from all nonprompt muon sample" );
          stf.set_cut( tight_mu_cut );
          stf.study_n_objects( true );
          stf.for_each( mu_lbl_nonprompt_preiso.begin() , mu_lbl_nonprompt_preiso.end() );
        }
        // do something else with the LBL muons here. call a study, and limit
        // the study to the 15--55 GeV mass window with a category like
        // this:
        //  study.add_category( "lowwin" , bind( AnaParticle::MassInWindow<const AnaParticle>(15.,55.) , _1 ) );
        // 
        // EF 2mu10 loose OS
      }
      // ================================================================
      // matrix method real lepton efficiencies
      // ================================================================
      //   measure isolation cut efficiency using Z's, Upsilons, J/Psi's
      //

      // ================================================================
      // matrix method sample composition counts: NTT, NTL, NLT, NLL
      // ================================================================
      {
        dg::down( "matrix_method" , "metrix method results" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
        // measure using all preisolated OS pairings
        StudyFakeMatrixMethod<const AnaMuon,const AnaMuon> stf( "sel_comp_preiso_os" , "numbers of events NTT, NTL, NLT, NLL" );
        stf.set_cut_legA( tight_mu_cut );
        stf.set_cut_legB( tight_mu_cut );
        stf.add_category( "lowwin" , bind( AnaParticle::MassInWindow<const AnaParticle>(15.,55.) , _1 ) );
        stf.add_category( "zwin" , bind( AnaParticle::MassInWindow<const AnaParticle>(76.,106.) , _1 ) );
        stf.for_each( mumu_lbl_preiso_os.begin() , mumu_lbl_preiso_os.end() );
        // measure using 2mu10 pairings OS
        stf.repurpose( "sel_comp_2mu10_os" , "numbers of events NTT, NTL, NLT, NLL" );
        stf.for_each( mumu_lbl_preiso_2mu10_os.begin() , mumu_lbl_preiso_2mu10_os.end() );
        // measure using mu6 pairings OS
        stf.repurpose( "sel_comp_mu6_os" , "numbers of events NTT, NTL, NLT, NLL" );
        stf.for_each( mumu_lbl_preiso_mu6_os.begin() , mumu_lbl_preiso_mu6_os.end() );
        // measure using mu18or40 pairings OS
        stf.repurpose( "sel_comp_mu18or40_os" , "numbers of events NTT, NTL, NLT, NLL" );
        stf.for_each( mumu_lbl_preiso_mu18or40_os.begin() , mumu_lbl_preiso_mu18or40_os.end() );
        //
        // measure using all preisolated pairings SS
        stf.repurpose( "sel_comp_preiso_ss" , "numbers of events NTT, NTL, NLT, NLL" );
        stf.for_each( mumu_lbl_preiso_ss.begin() , mumu_lbl_preiso_ss.end() );
        // measure using 2mu10 pairings SS
        stf.repurpose( "sel_comp_2mu10_ss" , "numbers of events NTT, NTL, NLT, NLL" );
        stf.for_each( mumu_lbl_preiso_2mu10_ss.begin() , mumu_lbl_preiso_2mu10_ss.end() );
        // measure using mu6 pairings SS
        stf.repurpose( "sel_comp_mu6_ss" , "numbers of events NTT, NTL, NLT, NLL" );
        stf.for_each( mumu_lbl_preiso_mu6_ss.begin() , mumu_lbl_preiso_mu6_ss.end() );
        // measure using mu18or40 pairings SS
        stf.repurpose( "sel_comp_mu18or40_ss" , "numbers of events NTT, NTL, NLT, NLL" );
        stf.for_each( mumu_lbl_preiso_mu18or40_ss.begin() , mumu_lbl_preiso_mu18or40_ss.end() );
        //
        // measure using all preisolated pairings AS (any-sign)
        stf.repurpose( "sel_comp_preiso_as" , "numbers of events NTT, NTL, NLT, NLL" );
        stf.for_each( mumu_lbl_preiso_as.begin() , mumu_lbl_preiso_as.end() );
        // measure using 2mu10 pairings AS
        stf.repurpose( "sel_comp_2mu10_as" , "numbers of events NTT, NTL, NLT, NLL" );
        stf.for_each( mumu_lbl_preiso_2mu10_as.begin() , mumu_lbl_preiso_2mu10_as.end() );
        // measure using mu6 pairings AS
        stf.repurpose( "sel_comp_mu6_as" , "numbers of events NTT, NTL, NLT, NLL" );
        stf.for_each( mumu_lbl_preiso_mu6_as.begin() , mumu_lbl_preiso_mu6_as.end() );
        // measure using mu18or40 pairings AS
        stf.repurpose( "sel_comp_mu18or40_as" , "numbers of events NTT, NTL, NLT, NLL" );
        stf.for_each( mumu_lbl_preiso_mu18or40_as.begin() , mumu_lbl_preiso_mu18or40_as.end() );
      } // end sample composition studies
    }
    
    // study W's
    if( true && (is_good_run_ele || is_good_run_mu) &&
        (is_triggered_ele || is_triggered_mu) ) {
      StudyComposite st( "munu_cand_hww" , "hww w->mu nu candidates" );
      st.add_selector( "nearest_mtw" , AnaParticle::ClosestTransverseMassToW<const AnaComposite>() );
      st.for_each( munu_hww.begin() , munu_hww.end() );
      st.repurpose( "enu_cand_hww" , "hww w->e nu candidates" );
      st.for_each( enu_hww.begin() , enu_hww.end() );

      // st.repurpose( "munu_cand_probe" , "probe w->mu nu candidates" );
      // st.for_each( munu_probe.begin() , munu_probe.end() );
      // st.repurpose( "enu_cand_probe" , "probe w->e nu candidates" );
      // st.for_each( enu_probe.begin() , enu_probe.end() );
      //
      dg::fillh( "n_enu_hww" , 10 , 0 , 10 , enu_hww.size() ); 
      dg::filleff( "eff_enu_hww" , !enu_hww.empty() ); 
      dg::fillh( "n_munu_hww" , 10 , 0 , 10 , munu_hww.size() ); 
      dg::filleff( "eff_munu_hww" , !munu_hww.empty() ); 
      // dg::fillh( "n_enu_probe" , 10 , 0 , 10 , enu_probe.size() ); 
      // dg::filleff( "eff_enu_probe" , !enu_probe.empty() ); 
      // dg::fillh( "n_munu_probe" , 10 , 0 , 10 , munu_probe.size() ); 
      // dg::filleff( "eff_munu_probe" , !munu_probe.empty() ); 
    }
    
    // study Z's
    if( true && (is_good_run_ele || is_good_run_mu) &&
        (is_triggered_ele || is_triggered_mu) ) {
      StudyComposite st( "mm_cand_hww" , "hww dimuon candidates" );
      st.add_selector( "nearest_zpole" , AnaParticle::ClosestMassToZ<const AnaComposite>() );
      st.study_categorized( false ); // only the one nearest the z pole please
        //
        // st.add_category( "zwindow" , bind( AnaParticle::MassInWindow<const AnaParticle>(81.,101.) , _1 ) );
        // st.add_category( "jpsiwindow" , bind( AnaParticle::MassInWindow<const AnaParticle>(2.8,3.3) , _1 ) );
        // st.add_category( "upsilonwindow" , bind( AnaParticle::MassInWindow<const AnaParticle>(5.,12.) , _1 ) );
        //
        // st.add_category( "zwindow_os" , bind( AnaParticle::MassInWindow<const AnaParticle>(81.,101.) , _1 ) && bind( AnaComposite::IsOppositeSignPair() , _1 ) );
        // st.add_category( "jpsiwindow_os" , bind( AnaParticle::MassInWindow<const AnaParticle>(2.8,3.3) , _1 ) && bind( AnaComposite::IsOppositeSignPair() , _1 ) );
        // st.add_category( "upsilonwindow_os" , bind( AnaParticle::MassInWindow<const AnaParticle>(5.,12.) , _1 ) && bind( AnaComposite::IsOppositeSignPair() , _1 ) );
        //
        // st.add_category( "zwindow_ss" , bind( AnaParticle::MassInWindow<const AnaParticle>(81.,101.) , _1 ) && bind( AnaComposite::IsSameSignPair() , _1 ) );
        // st.add_category( "jpsiwindow_ss" , bind( AnaParticle::MassInWindow<const AnaParticle>(2.8,3.3) , _1 ) && bind( AnaComposite::IsSameSignPair() , _1 ) );
        // st.add_category( "upsilonwindow_ss" , bind( AnaParticle::MassInWindow<const AnaParticle>(5.,12.) , _1 ) && bind( AnaComposite::IsSameSignPair() , _1 ) );
        //
      st.for_each( mumu_hww.begin() , mumu_hww.end() );
      st.repurpose( "ee_cand_hww" , "SM WW dielectron candidates" );
      st.for_each( ee_hww.begin() , ee_hww.end() );
      st.repurpose( "ee_cand_lowmh" , "lowmh dielectron candidates" );
      st.for_each( ee_lowmh.begin() , ee_lowmh.end() );
      // st.repurpose( "ee_cand_relaxed_lowmh" , "medium_lowmh dielectron candidates" );
      // st.for_each( ee_relaxed_lowmh.begin() , ee_relaxed_lowmh.end() );
      st.repurpose( "mm_cand_lowmh" , "lowmh dimuon candidates" );
      st.for_each( mumu_nontrigger_loose.begin() , mumu_nontrigger_loose.end() );
      // st.repurpose( "mm_cand_relaxed_lowmh" , "lowmh dimuon candidates" );
      // st.for_each( mumu_relaxed_lowmh.begin() , mumu_relaxed_lowmh.end() );
      //
      st.repurpose( "mm_cand_probe_staco" , "probe_staco dimuon candidates" );
      st.for_each( mumu_probe_staco.begin() , mumu_probe_staco.end() );
      st.repurpose( "mm_cand_probe_staco" , "probe_staco dimuon candidates" );
      st.for_each( mumu_probe_staco.begin() , mumu_probe_staco.end() );
      // st.repurpose( "mm_cand_probe_muid" , "probe_muid dimuon candidates" );
      // st.for_each( mumu_probe_muid.begin() , mumu_probe_muid.end() );
      st.repurpose( "ee_cand_probe" , "probe dielectron candidates" );
      st.for_each( ee_probe.begin() , ee_probe.end() );
      //
      {
        StudyLeptonReconstruction<const AnaElectron> stlep( "tagprobe_ele_rec" , "lepton reconstruction study" );
        stlep.reconstructed_electrons( ele_hww.begin() , ele_hww.end() );
        StudyTagProbeZ<const AnaElectron,const AnaElectron> stz( "ee_cand_tagprobe" , "tag + probe dielectron candidates" );
        stz.add_selector( "nearest_zpole" , AnaParticle::ClosestMassToZ<const AnaZ>() );
        stz.study_categorized( false ); // only the one nearest the z pole please
        AnaComposite::AllConstituentsPass<const AnaElectron> cutEL; cutEL.add_cut( bind( &AnaElectron::is_ElectronLoose_v16,_1) );
        AnaComposite::AllConstituentsPass<const AnaElectron> cutELpp; cutELpp.add_cut( bind( &AnaElectron::is_ElectronLoosePlusPlus_v16,_1) );
        AnaComposite::AllConstituentsPass<const AnaElectron> cutEM; cutEM.add_cut( bind( &AnaElectron::is_ElectronMedium_v16,_1) );
        AnaComposite::AllConstituentsPass<const AnaElectron> cutEMpp; cutEMpp.add_cut( bind( &AnaElectron::is_ElectronMediumPlusPlus_v16,_1) );
        AnaComposite::AllConstituentsPass<const AnaElectron> cutET; cutET.add_cut( bind( &AnaElectron::is_ElectronTight_v16,_1) );
        // AnaComposite::AllConstituentsPass<const AnaElectron> cutPEL; cutPEL.add_cut( bind( &AnaElectron::is_PhotonLoose,_1) );
        stz.add_category( "ele_loose" , cutEL );
        stz.add_category( "ele_loose_plusplus" , cutELpp );
        stz.add_category( "ele_medium" , cutEM );
        stz.add_category( "ele_medium_plusplus" , cutEMpp );
        stz.add_category( "ele_tight" , cutET );
        // stz.add_category( "ele_pho_loose" , cutPEL );
        stz.add_category( "opposite_sign" , AnaComposite::IsOppositeSignPair() );
        stz.add_category( "same_sign" , AnaComposite::IsSameSignPair() );
        stz.tag_identifier( &ele_hww_selector );
        stz.probe_study( &stlep );
        stz.probe_cut_to_study( "hww" , &ele_hww_selector );
        stz.probe_cut_to_study( "lowmh" , &ele_nontrigger_loose_selector );
        stz.for_each( ee_tagprobe.begin() , ee_tagprobe.end() );
      }
      {
        StudyLeptonReconstruction<const AnaElectron> stlep( "tagprobe_expanded_ele_rec" , "lepton reconstruction study" );
        stlep.reconstructed_electrons( ele_hww.begin() , ele_hww.end() );
        StudyTagProbeZ<const AnaElectron,const AnaElectron> stz( "ee_cand_tagprobe_expanded" , "tag + expanded probe dielectron candidates" );
        stz.add_selector( "nearest_zpole" , AnaParticle::ClosestMassToZ<const AnaZ>() );
        stz.study_categorized( false ); // only the one nearest the z pole please
        AnaComposite::AllConstituentsPass<const AnaElectron> cutEL; cutEL.add_cut( bind( &AnaElectron::is_ElectronLoose_v16,_1) );
        AnaComposite::AllConstituentsPass<const AnaElectron> cutELpp; cutELpp.add_cut( bind( &AnaElectron::is_ElectronLoosePlusPlus_v16,_1) );
        AnaComposite::AllConstituentsPass<const AnaElectron> cutEM; cutEM.add_cut( bind( &AnaElectron::is_ElectronMedium_v16,_1) );
        AnaComposite::AllConstituentsPass<const AnaElectron> cutEMpp; cutEMpp.add_cut( bind( &AnaElectron::is_ElectronMediumPlusPlus_v16,_1) );
        AnaComposite::AllConstituentsPass<const AnaElectron> cutET; cutET.add_cut( bind( &AnaElectron::is_ElectronTight_v16,_1) );
        // AnaComposite::AllConstituentsPass<const AnaElectron> cutPEL; cutPEL.add_cut( bind( &AnaElectron::is_PhotonLoose,_1) );
        stz.add_category( "ele_loose" , cutEL );
        stz.add_category( "ele_loose_plusplus" , cutELpp );
        stz.add_category( "ele_medium" , cutEM );
        stz.add_category( "ele_medium_plusplus" , cutEMpp );
        stz.add_category( "ele_tight" , cutET );
        // stz.add_category( "ele_pho_loose" , cutPEL );
        stz.add_category( "opposite_sign" , AnaComposite::IsOppositeSignPair() );
        stz.add_category( "same_sign" , AnaComposite::IsSameSignPair() );
        stz.tag_identifier( &ele_hww_selector );
        stz.probe_study( &stlep );
        stz.probe_cut_to_study( "probe" , &ele_probe_selector );
        stz.probe_cut_to_study( "hww" , &ele_hww_selector );
        stz.probe_cut_to_study( "lowmh" , &ele_nontrigger_loose_selector );
        stz.for_each( ee_tagprobe_expanded.begin() , ee_tagprobe_expanded.end() );
      }
      {
        StudyLeptonReconstruction<const AnaElectron> stlep( "taganti_ele_rec" , "lepton reconstruction study" );
        stlep.reconstructed_electrons( ele_hww.begin() , ele_hww.end() );
        StudyTagProbeZ<const AnaElectron,const AnaElectron> stz( "ee_cand_taganti" , "anti-electron + probe dielectron candidates" );
        stz.add_selector( "nearest_zpole" , AnaParticle::ClosestMassToZ<const AnaZ>() );
        stz.study_categorized( false ); // only the one nearest the z pole please
        AnaComposite::AllConstituentsPass<const AnaElectron> cutEL; cutEL.add_cut( bind( &AnaElectron::is_ElectronLoose_v16,_1) );
        AnaComposite::AllConstituentsPass<const AnaElectron> cutELpp; cutELpp.add_cut( bind( &AnaElectron::is_ElectronLoosePlusPlus_v16,_1) );
        AnaComposite::AllConstituentsPass<const AnaElectron> cutEM; cutEM.add_cut( bind( &AnaElectron::is_ElectronMedium_v16,_1) );
        AnaComposite::AllConstituentsPass<const AnaElectron> cutEMpp; cutEMpp.add_cut( bind( &AnaElectron::is_ElectronMediumPlusPlus_v16,_1) );
        AnaComposite::AllConstituentsPass<const AnaElectron> cutET; cutET.add_cut( bind( &AnaElectron::is_ElectronTight_v16,_1) );
        // AnaComposite::AllConstituentsPass<const AnaElectron> cutPEL; cutPEL.add_cut( bind( &AnaElectron::is_PhotonLoose,_1) );
        stz.add_category( "ele_loose" , cutEL );
        stz.add_category( "ele_loose_plusplus" , cutELpp );
        stz.add_category( "ele_medium" , cutEM );
        stz.add_category( "ele_medium_plusplus" , cutEMpp );
        stz.add_category( "ele_tight" , cutET );
        // stz.add_category( "ele_pho_loose" , cutPEL );
        stz.add_category( "opposite_sign" , AnaComposite::IsOppositeSignPair() );
        stz.add_category( "same_sign" , AnaComposite::IsSameSignPair() );
        stz.tag_identifier( &ele_hww_selector );
        stz.probe_study( &stlep );
        stz.probe_cut_to_study( "hww" , &ele_hww_selector );
        stz.probe_cut_to_study( "lowmh" , &ele_nontrigger_loose_selector );
        stz.for_each( ee_taganti.begin() , ee_taganti.end() );
      }
      {
        StudyLeptonReconstruction<const AnaElectron> stlep( "photonprobe_ele_rec" , "lepton reconstruction study" );
        stlep.reconstructed_electrons( ele_hww.begin() , ele_hww.end() );
        StudyTagProbeZ<const AnaPhoton,const AnaElectron> stz( "ee_cand_photonprobe" , "tag + probe dielectron candidates" );
        stz.add_selector( "nearest_zpole" , AnaParticle::ClosestMassToZ<const AnaZ>() );
        stz.study_categorized( false ); // only the one nearest the z pole please
        // stz.tag_identifier( &photon_wjets_selector );
        stz.probe_study( &stlep );
        stz.probe_cut_to_study( "hww" , &ele_hww_selector );
        stz.probe_cut_to_study( "lowmh" , &ele_nontrigger_loose_selector );
        stz.for_each( ee_photonprobe.begin() , ee_photonprobe.end() );
      }
      {
        StudyLeptonReconstruction<const AnaMuon> stlep( "tagprobe_mu_rec" , "lepton reconstruction study" );
        stlep.reconstructed_muons( mu_hww.begin() , mu_hww.end() );
        StudyTagProbeZ<const AnaMuon,const AnaMuon> stz( "mumu_cand_tagprobe" , "tag + probe dimuon candidates" );
        stz.add_selector( "nearest_zpole" , AnaParticle::ClosestMassToZ<const AnaZ>() );
        stz.study_categorized( false ); // only the one nearest the z pole please
        AnaComposite::AllConstituentsPass<const AnaMuon> cutMA; cutMA.add_cut( bind( &AnaMuon::is_staco_any,_1) );
        AnaComposite::AllConstituentsPass<const AnaMuon> cutMT; cutMT.add_cut( bind( &AnaMuon::is_mutag,_1) );
        AnaComposite::AllConstituentsPass<const AnaMuon> cutMB; cutMB.add_cut( bind( &AnaMuon::is_muonboy,_1) );
        AnaComposite::AllConstituentsPass<const AnaMuon> cutMC; cutMC.add_cut( bind( &AnaMuon::is_staco_combined,_1) );
        stz.add_category( "mu_any" , cutMA );
        stz.add_category( "mu_mutag" , cutMT );
        stz.add_category( "mu_muonboy" , cutMB );
        stz.add_category( "mu_staco_combined" , cutMC );
        stz.add_category( "opposite_sign" , AnaComposite::IsOppositeSignPair() );
        stz.add_category( "same_sign" , AnaComposite::IsSameSignPair() );
        stz.tag_identifier( &mu_hww_selector );
        stz.probe_study( &stlep );
        stz.probe_cut_to_study( "hww" , &mu_hww_selector );
        stz.probe_cut_to_study( "lowmh" , &mu_nontrigger_loose_selector );
        stz.probe_cut_to_study( "tag_lowmh" , &mu_tag_lowmh_selector );
        stz.for_each( mumu_tagprobe.begin() , mumu_tagprobe.end() );
      }
      // {
      //   StudyLeptonReconstruction<const AnaMuon> stlep( "tagprobe_expanded_mu_rec" , "lepton reconstruction study" );
      //   stlep.reconstructed_muons( mu_hww.begin() , mu_hww.end() );
      //   StudyTagProbeZ<const AnaMuon,const AnaMuon> stz( "mumu_cand_tagprobe_expanded" , "tag + expanded probe dimuon candidates" );
      //   stz.add_selector( "nearest_zpole" , AnaParticle::ClosestMassToZ<const AnaZ>() );
      //   stz.study_categorized( false ); // only the one nearest the z pole please
      //   AnaComposite::AllConstituentsPass<const AnaMuon> cutMA; cutMA.add_cut( bind( &AnaMuon::is_staco_any,_1) );
      //   AnaComposite::AllConstituentsPass<const AnaMuon> cutMT; cutMT.add_cut( bind( &AnaMuon::is_mutag,_1) );
      //   AnaComposite::AllConstituentsPass<const AnaMuon> cutMB; cutMB.add_cut( bind( &AnaMuon::is_muonboy,_1) );
      //   AnaComposite::AllConstituentsPass<const AnaMuon> cutMC; cutMC.add_cut( bind( &AnaMuon::is_staco_combined,_1) );
      //   // stz.add_category( "mu_any" , cutMA );
      //   // stz.add_category( "mu_mutag" , cutMT );
      //   // stz.add_category( "mu_muonboy" , cutMB );
      //   // stz.add_category( "mu_staco_combined" , cutMC );
      //   stz.add_category( "opposite_sign" , AnaComposite::IsOppositeSignPair() );
      //   stz.add_category( "same_sign" , AnaComposite::IsSameSignPair() );
      //   stz.tag_identifier( &mu_hww_selector );
      //   stz.probe_study( &stlep );
      //   stz.probe_cut_to_study( "probe_staco" , &mu_probe_staco_selector );
      //   // stz.probe_cut_to_study( "probe_muid" , &mu_probe_muid_selector );
      //   stz.probe_cut_to_study( "hww" , &mu_hww_selector );
      //   stz.probe_cut_to_study( "lowmh" , &mu_nontrigger_loose_selector );
      //   stz.probe_cut_to_study( "tag_lowmh" , &mu_tag_lowmh_selector );
      //   stz.for_each( mumu_tagprobe_expanded.begin() , mumu_tagprobe_expanded.end() );
      // }
      {
        StudyLeptonReconstruction<const AnaMuon> stlep( "taganti_mu_rec" , "lepton reconstruction study" );
        stlep.reconstructed_muons( mu_hww.begin() , mu_hww.end() );
        StudyTagProbeZ<const AnaMuon,const AnaMuon> stz( "mumu_cand_taganti" , "tag + probe dimuon candidates" );
        stz.add_selector( "nearest_zpole" , AnaParticle::ClosestMassToZ<const AnaZ>() );
        stz.study_categorized( false ); // only the one nearest the z pole please
        // AnaComposite::AllConstituentsPass<const AnaMuon> cutMA; cutMA.add_cut( bind( &AnaMuon::is_staco_any,_1) );
        // AnaComposite::AllConstituentsPass<const AnaMuon> cutMT; cutMT.add_cut( bind( &AnaMuon::is_mutag,_1) );
        // AnaComposite::AllConstituentsPass<const AnaMuon> cutMB; cutMB.add_cut( bind( &AnaMuon::is_muonboy,_1) );
        // AnaComposite::AllConstituentsPass<const AnaMuon> cutMC; cutMC.add_cut( bind( &AnaMuon::is_staco_combined,_1) );
        // stz.add_category( "mu_any" , cutMA );
        // stz.add_category( "mu_mutag" , cutMT );
        // stz.add_category( "mu_muonboy" , cutMB );
        // stz.add_category( "mu_staco_combined" , cutMC );
        stz.add_category( "opposite_sign" , AnaComposite::IsOppositeSignPair() );
        stz.add_category( "same_sign" , AnaComposite::IsSameSignPair() );
        stz.tag_identifier( &mu_hww_selector );
        stz.probe_study( &stlep );
        stz.probe_cut_to_study( "hww" , &mu_hww_selector );
        stz.probe_cut_to_study( "lowmh" , &mu_nontrigger_loose_selector );
        stz.probe_cut_to_study( "tag_lowmh" , &mu_tag_lowmh_selector );
        stz.for_each( mumu_taganti.begin() , mumu_taganti.end() );
      }

      dg::fillh( "n_ee_hww" , 10 , 0 , 10 , ee_hww.size() ); 
      dg::filleff( "eff_ee_hww" , !ee_hww.empty() ); 
      dg::fillh( "n_mmm_hww" , 10 , 0 , 10 , mumu_hww.size() ); 
      dg::filleff( "eff_mm_hww" , !mumu_hww.empty() ); 
      dg::fillh( "n_ee_probe" , 10 , 0 , 10 , ee_probe.size() ); 
      dg::filleff( "eff_ee_probe" , !ee_probe.empty() ); 
      dg::fillh( "n_mm_probe" , 10 , 0 , 10 , mumu_probe_staco.size() ); 
      dg::filleff( "eff_mm_probe" , !mumu_probe_staco.empty() ); 
      dg::fillh( "n_mm_probe" , 10 , 0 , 10 , mumu_probe_muid.size() ); 
      dg::filleff( "eff_mm_probe" , !mumu_probe_muid.empty() ); 
    }

    // if( !is_good_triggered ) { continue; }

    // // select primary vertices
    // // selector has been filled
    // dg::filleff("c2_has_good_primary_vertex", !primary_vertices.empty() );
    // if( primary_vertices.empty() ) { continue; }

  } // end for each event
  
  // end the analysis. cleanup, histogram writing, etc. is automatic.
  cout << " analysis is complete." << endl;

  AnaEventMgr::instance()->close_sample();

} // end analysis


// compare two collections of particles. for each reference particle,
// look for the particle in the overlap collection with the smallest
// delta r. if that delta r is less than or equal to max_overlap_dr ,
// remove it. do not remove more than one overlap_coll particle for
// each reference coll particle.

template< typename overlapIterT , typename refIterT , typename outputIterT >
void overlap_removal_smallest_dr( overlapIterT overlap_begin , overlapIterT overlap_end ,
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

// compare two collections of particles. for each reference particle,
// removal particles in the overlap collection which have delta r less
// than or equal to max_overlap_dr

template< typename overlapIterT , typename refIterT , typename outputIterT >
void overlap_removal_all_in_cone( overlapIterT overlap_begin , overlapIterT overlap_end ,
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

  for( overlapIterT iover=overlap_begin; iover!=overlap_end; ++iover ) {
    bool overlap = false;
    for( refIterT iref=ref_begin; iref!=ref_end; ++iref ) {
      if( (*iref)->dist_eta_phi( (*iover) ) <= max_overlap_dr ) { overlap = true; break; }
    }
    if( overlap ) { continue; }
    *(output_iterator++) = *iover;
  }
}


// internal overlap removal by pt. if two particles in the overlap
// collection have delta r less than or equal to max_overlap_dr ,
// remove the lowest pT particle. if minDrOnly is true, only remove
// the smallest delta r overlap. if minDrOnly is false, remove all
// lower-pt overlapping particles.

template< typename overlapIterT , typename outputIterT >
void internal_overlap_removal_by_pt( overlapIterT overlap_begin , overlapIterT overlap_end ,
                                     const AnaParticle::ftype& max_overlap_dr ,
                                     outputIterT output_iterator ,
                                     const bool minDrOnly = true )
{
  if( overlap_begin == overlap_end ) { return; }
  assert( overlap_begin != overlap_end );
  // if the size of the collection is one, there is no overlap. return the single entry.
  if( std::distance( overlap_begin , overlap_end ) == 1 ) {
    *(output_iterator++) = *overlap_begin;
    return;
  }
  //
  // construct list of input particles ordered in decreasing pT.
  typedef std::list< typename iterator_traits<overlapIterT>::value_type > overlap_collection;
  // if element_type doesn't exist, this is not a collection of
  // shared_ptr. if you need to use this with generic pointers, handle
  // that instance.
  typedef typename iterator_traits<overlapIterT>::value_type::element_type overlap_particle_type;
  overlap_collection overlap( overlap_begin , overlap_end );
  overlap.sort( boost::bind(&AnaParticle::pt,_1) > boost::bind(&AnaParticle::pt,_2) );

  for( typename overlap_collection::iterator i=overlap.begin(), f=overlap.end(); i!=f; ++i ) {
    // compute lowest delta R of the lower pT particles with the ith particle.
    typename overlap_collection::iterator j=i; 
    ++j; // j may be equal to f after this, but it will not run beyond its bounds as long as i!=f
    // removal loop; exit only when either the smallest dr overlap is
    // larger than max_overlap_dr, or if minDrOnly is true and the
    // smallest dr particle has already been removed.
    while( true ) {
      typename overlap_collection::iterator kmin = std::min_element( j , f , AnaParticle::SmallestDeltaEtaPhi<overlap_particle_type>( (*i)->eta() , (*i)->phi() ) );
      bool is_overlap = false;
      if( kmin != f ) {
        const AnaParticle::ftype dr = (*i)->dist_eta_phi( *kmin );
        is_overlap = (dr <= max_overlap_dr);
      }
      if( !is_overlap ) { break; }
      // remove the overlap. does not invalid any iterator other than kmin and anything equal to kmin.
      assert( j!=f ); // must be true or there would be no overlap
      assert( kmin!=f );
      if( kmin==j ) {
        // erasure will invalidate j. update it.
        j=i;
        overlap.erase( kmin );
        ++j;
      } else {
        overlap.erase( kmin );
      }
      // smallest dr particle has now been removed. exit the loop if minDrOnly.
      if( minDrOnly ) { break; }
    }
  }

  // overlaps have been removed from the "overlap" collection. now
  // return the output.
  std::copy( overlap.begin() , overlap.end() , output_iterator );

}

void
hsg3_cutflow( const bool is_good_run , float& evtweight , const bool ok_clean_met , const bool ok_clean_met_lowpt , const bool bad_FEB , 
              const EventObjectColl< shared_ptr<const AnaElectron> >& ele_hww ,
              const EventObjectColl< shared_ptr<const AnaMuon> >& mu_hww ,
              const EventObjectColl< shared_ptr<const AnaMuon> >& mu_hww_lowpt ,
              const EventObjectColl< shared_ptr<const AnaMET> >& met_hww ,
              const EventObjectColl< shared_ptr<const AnaJet> >& jet_hww ,
              float& outweight , float febWeight , const float pileupWeight , const float mcweight )
{
  // HSG3 cutflow
  // ================================================================
  //
  // declarations for minintuple
  float evtLeptonSF=1.;
  float evtMuonSF=1.;
  float evtElectronSF=1.;
  float evtTriggerSF = 1.;
  int categoryTrigMatched = 0.;
  //
  vector< shared_ptr<const AnaParticle> > lepton_hww;
  copy( ele_hww.cbegin() , ele_hww.cend() , back_inserter(lepton_hww) );
  copy( mu_hww.cbegin() , mu_hww.cend() , back_inserter(lepton_hww) );
    vector< shared_ptr<const AnaParticle> > lepton_hww_lowpt;
  //  copy( ele_hww.cbegin() , ele_hww_lowpt.cend() , back_inserter(lepton_hww_lowpt) );
    copy( mu_hww_lowpt.cbegin() , mu_hww_lowpt.cend() , back_inserter(lepton_hww_lowpt) );
  //
  const CONF::ConfType conf = AnaConfiguration::conf();
  //
  const string HSG3_ZEROJET( "zerojet" ); // define string constants so compiler checks for typos/misnaming                                            
  const string HSG3_ONEJET( "onejet" );
  const string HSG3_TWOJET( "twojet" );
  const string HSG3_TWOELE( "twoele" ); // define string constants so compiler checks for typos/misnaming
  const string HSG3_TWOMU( "twomu" );
  const string HSG3_TWOMU_LOWPT( "twomu_lowpt" );
  const string HSG3_EMU( "emu" );
  std::map< string , map< string ,dynamic_bitset<> > > hsg3_cutflow;
  // enum { HSG3_TRIGGER , 

  // hsg3_cutflow[ HSG3_TWOELE ][ HSG3_ZEROJET ].reset();
  // hsg3_cutflow[ HSG3_TWOMU  ][ HSG3_ZEROJET ].reset();
  // hsg3_cutflow[ HSG3_TWOMU_LOWPT ][ HSG3_ZEROJET ].reset();
  // hsg3_cutflow[ HSG3_EMU    ][ HSG3_ZEROJET ].reset();
  // hsg3_cutflow[ HSG3_TWOELE ][ HSG3_ONEJET ].reset();
  // hsg3_cutflow[ HSG3_TWOMU  ][ HSG3_ONEJET ].reset();
  // hsg3_cutflow[ HSG3_TWOMU_LOWPT ][ HSG3_ONEJET ].reset();
  // hsg3_cutflow[ HSG3_EMU    ][ HSG3_ONEJET ].reset();
  // hsg3_cutflow[ HSG3_TWOELE ][ HSG3_TWOJET ].reset();
  // hsg3_cutflow[ HSG3_TWOMU  ][ HSG3_TWOJET ].reset();
  // hsg3_cutflow[ HSG3_TWOMU_LOWPT ][ HSG3_TWOJET ].reset();
  // hsg3_cutflow[ HSG3_EMU    ][ HSG3_TWOJET ].reset();
  //
  const shared_ptr<const AnaEvent>& evt = AnaEventMgr::current_event();
  const shared_ptr<const AnaTrigger>& trigger = evt->trigger();
  AnaEvent::tribool is_mc = evt->check_flag( "mc" );
  const bool is_triggered_ele = trigger->passes("EF_e20_medium");
  const bool is_triggered_mu = trigger->passes("EF_mu18_mg") || trigger->passes("EF_mu40_MSOnly_barrel");
  const bool is_triggered = is_triggered_ele || is_triggered_mu;

  //
  if( is_good_run ) {
    dg::down( "plhc_hsg3_cutflow" , "selector details" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
    // split trigger cutflow by lepton channel.
    dg::filleff( "c5_trigger_any_ele" , is_triggered_ele );
    dg::filleff( "c5_trigger_any_mu" , is_triggered_mu );
    dg::filleff( "c5_trigger_any" , is_triggered_ele || is_triggered_mu );
    //
    vector<string> categories;
    categories.push_back(HSG3_TWOELE);
    categories.push_back(HSG3_TWOMU);
    categories.push_back(HSG3_TWOMU_LOWPT);
    categories.push_back(HSG3_EMU);
    categories.push_back("none");
    outweight = evtweight;
    BOOST_FOREACH( const string& category , categories ) {
      evtweight = outweight;
      if( is_mc ) dg::set_global_weight( evtweight );
      // if( ele_hww.size()>=2 ) { category = "twoele"; }
      // else if( mu_hww.size()>=2 ) { category = "twomu"; }
      // else if( lepton_hww.size()>=2 ) { category = "emu"; }
      // else { category= "no_leptons"; }
      assert( category != "" );
      dg::down( category , "dilepton category" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      //
      // dg::filleff( "c5_trigger_ele" , is_triggered_ele );
      // dg::filleff( "c5_trigger_mu" , is_triggered_mu );
      // dg::filleff( "c5_trigger_any" , is_triggered_ele || is_triggered_mu );
      //
      bool fail_trigger = false;
      if( category==HSG3_TWOELE && !is_triggered_ele ) { fail_trigger=true; }
      if( category==HSG3_TWOMU && !is_triggered_mu ) { fail_trigger=true; }
      if( category==HSG3_TWOMU_LOWPT && !is_triggered_mu ) { fail_trigger=true; }
      if( category==HSG3_EMU && !is_triggered_ele && !is_triggered_mu  ) { fail_trigger=true; }
      if( category==HSG3_TWOMU_LOWPT ) {
        dg::filleff( "c5_trigger" , !fail_trigger && ok_clean_met_lowpt );
      }
      else if( category==HSG3_TWOELE || category==HSG3_TWOMU || category==HSG3_EMU ) {
        dg::filleff( "c5_trigger" , !fail_trigger && ok_clean_met );
      }
      if( category==HSG3_TWOMU_LOWPT ) {
        if( fail_trigger || !ok_clean_met_lowpt ) { continue; }
      }
      else if(category==HSG3_TWOELE || category==HSG3_TWOMU || category==HSG3_EMU ) {
        if( fail_trigger || !ok_clean_met ) { continue; }
      }
      //
      static egammaSFclass fEGammaSF;
      bool exactly_two_leptons = category==HSG3_TWOMU_LOWPT ? mu_hww_lowpt.size()==2 : lepton_hww.size()==2;
      //
      if( exactly_two_leptons ) { 
        if( category==HSG3_TWOELE && ele_hww.size()!=2 ) { exactly_two_leptons=false; }
        if( category==HSG3_TWOMU && mu_hww.size()!=2 ) { exactly_two_leptons=false; }
        if( category==HSG3_TWOMU_LOWPT && mu_hww_lowpt.size()!=2 ) { exactly_two_leptons=false; }
        if( category==HSG3_EMU && (ele_hww.size()!=1 || mu_hww.size()!=1) ) { exactly_two_leptons=false; }
        if( category==HSG3_TWOELE && ele_hww.size()==2 ) { 
          double eleRecoSF0 = fEGammaSF.scaleFactorRecoTrkQual( ele_hww.front()->eta()).first;
          double eleIdSF0 = fEGammaSF.scaleFactorTightETcorrected( ele_hww.front()->eta(), ele_hww.front()->pt()*1000).first;
          double eleRecoSF1 = fEGammaSF.scaleFactorRecoTrkQual( ele_hww.back()->eta()).first;
          double eleIdSF1 = fEGammaSF.scaleFactorTightETcorrected( ele_hww.back()->eta(), ele_hww.back()->pt()*1000).first;
          evtLeptonSF = eleRecoSF0 * eleRecoSF1 * eleIdSF0 * eleIdSF1; 
          evtElectronSF = eleRecoSF0 * eleRecoSF1 * eleIdSF0 * eleIdSF1; 
          //cout << setprecision (5) << "reco0/id0/reco1/id1:" << eleRecoSF0 << " " << eleIdSF0 << " " << eleRecoSF1 << " " << eleIdSF1 << " " << evtLeptonSF << endl;
        }
        else if( category==HSG3_TWOMU && mu_hww.size()==2 ) {
	  double muSF0,muSF1;
	  float e2,e3;
	  AnaMuon::mcp_effscale( conf, AnaConfiguration::data_period(), mu_hww.front(), 0, evt->run_number(), muSF0,e2,e3);
	  AnaMuon::mcp_effscale( conf, AnaConfiguration::data_period(), mu_hww.back(), 0, evt->run_number(), muSF0,e2,e3);
          evtLeptonSF=muSF0*muSF1; 
          evtMuonSF = muSF0*muSF1; 
          //cout << setprecision (5) << "reco0/reco1:" << muSF0 << " " << muSF1 << endl;
        }
        else if( category==HSG3_TWOMU_LOWPT && mu_hww_lowpt.size()==2 ) {  
	  double muSF0,muSF1;
	  float e2,e3;
	  AnaMuon::mcp_effscale( conf, AnaConfiguration::data_period(), mu_hww_lowpt.front(), 0, evt->run_number(), muSF0,e2,e3);
	  AnaMuon::mcp_effscale( conf, AnaConfiguration::data_period(), mu_hww_lowpt.back(), 0, evt->run_number(), muSF0,e2,e3);
          evtLeptonSF=muSF0*muSF1; 
          evtMuonSF = muSF0*muSF1; 
          //cout << setprecision (5) << "reco0/reco1:" << muSF0 << " " << muSF1 << endl;
        }
        else if( category==HSG3_EMU && (ele_hww.size()==1 && mu_hww.size()==1) ) { 
          double eleRecoSF0 = fEGammaSF.scaleFactorRecoTrkQual( ele_hww.front()->eta()).first;
          double eleIdSF0 = fEGammaSF.scaleFactorTightETcorrected( ele_hww.front()->eta(), ele_hww.front()->pt()*1000).first;
	  double muSF0;
	  float e2,e3;
	  AnaMuon::mcp_effscale( conf, AnaConfiguration::data_period(), mu_hww.front(), 0, evt->run_number(), muSF0,e2,e3);
          evtLeptonSF=muSF0*eleRecoSF0*eleIdSF0; 
          evtMuonSF = muSF0; 
          evtElectronSF = eleRecoSF0*eleIdSF0; 
        }
      }
      int elMatched = 0;
      bool elMatched0 = false;
      bool elMatched1 = false;
      for(int i=0; i!=ele_hww.size(); i++){
        double el_pt = (i==0) ? ele_hww.front()->pt() : ele_hww.back()->pt();
        double el_dr = (i==0) ? ele_hww.front()->trigger_match_dr("EF_e20_medium") : ele_hww.back()->trigger_match_dr("EF_e20_medium");
        if( el_pt>25 && el_dr<0.15 ) {
          if( i==0 ) { elMatched0=true; }
          else if( i==1 ) { elMatched1=true; }
        }
      }
      if( elMatched0 ) { ++elMatched; }
      if( elMatched1 ) { ++elMatched; }

      int muMatched = 0;
      bool muMatched0 = false;
      bool muMatched1 = false;
      for( int i=0; i!=mu_hww.size(); i++ ) {
        double mu_pt = (i==0) ? mu_hww.front()->pt() : mu_hww.back()->pt();
        double mu_dr_18 = (i==0) ? mu_hww.front()->trigger_match_dr("EF_mu18_MG") : mu_hww.back()->trigger_match_dr("EF_mu18_MG");
        double mu_l2_dr_18 = (i==0) ? mu_hww.front()->trigger_match_dr("L2_mu18_MG") : mu_hww.back()->trigger_match_dr("L2_mu18_MG");
        //cout << "l2dr/efdr/mupt mu18: " << mu_dr_18 << " " << mu_l2_dr_18 << " " << mu_pt << endl;
        if ( mu_pt>20 && mu_dr_18<0.10 && mu_l2_dr_18<0.1) {
          if (i==0) muMatched0=true;
          else if (i==1) muMatched1=true;
        }
      }
      for(int i=0; i!=mu_hww.size(); i++){
        double mu_pt = (i==0) ? mu_hww.front()->pt() : mu_hww.back()->pt();
        double mu_dr_40 = (i==0) ? mu_hww.front()->trigger_match_dr("EF_mu40_MSonly_barrel") : mu_hww.back()->trigger_match_dr("EF_mu40_MSonly_barrel");
        double mu_l2_dr_40 = (i==0) ? mu_hww.front()->trigger_match_dr("L2_mu40_MSonly_barrel") : mu_hww.back()->trigger_match_dr("L2_mu40_MSonly_barrel"); // sa
        //cout << "l2dr/efdr/mupt mu40: " << mu_dr_40 << " " << mu_l2_dr_40 << " " << mu_pt << endl;
        if ( mu_pt>42 && mu_dr_40<0.10 && mu_l2_dr_40<0.1){
          if (i==0) muMatched0=true;
          else if (i==1) muMatched1=true;
        }
      }
      if (muMatched0) {
        muMatched++;
      }
      if (muMatched1) {
        muMatched++;
      }

      // low pt muons trigger matching
      int muMatched_lowpt = 0;
      bool muMatched_lowpt0 = false;
      bool muMatched_lowpt1 = false;
      for(int i=0; i!=mu_hww_lowpt.size(); i++){
        double mu_pt = (i==0) ? mu_hww_lowpt.front()->pt() : mu_hww_lowpt.back()->pt();
        double mu_dr_18 = (i==0) ? mu_hww_lowpt.front()->trigger_match_dr("EF_mu18_MG") : mu_hww_lowpt.back()->trigger_match_dr("EF_mu18_MG");
        double mu_l2_dr_18 = (i==0) ? mu_hww_lowpt.front()->trigger_match_dr("L2_mu18_MG") : mu_hww_lowpt.back()->trigger_match_dr("L2_mu18_MG");
        //cout << "l2dr/efdr/mupt mu18: " << mu_dr_18 << " " << mu_l2_dr_18 << " " << mu_pt << endl;
        if ( mu_pt>20 && mu_dr_18<0.10 && mu_l2_dr_18<0.1) {
          if (i==0) muMatched_lowpt0=true;
          else if (i==1) muMatched_lowpt1=true;
        }
      }
      for(int i=0; i!=mu_hww_lowpt.size(); i++){
        double mu_pt = (i==0) ? mu_hww_lowpt.front()->pt() : mu_hww_lowpt.back()->pt();
        double mu_dr_40 = (i==0) ? mu_hww_lowpt.front()->trigger_match_dr("EF_mu40_MSonly_barrel") : mu_hww_lowpt.back()->trigger_match_dr("EF_mu40_MSonly_barrel");
        double mu_l2_dr_40 = (i==0) ? mu_hww_lowpt.front()->trigger_match_dr("L2_mu40_MSonly_barrel") : mu_hww_lowpt.back()->trigger_match_dr("L2_mu40_MSonly_barrel"); // sa
        //cout << "l2dr/efdr/mupt mu40: " << mu_dr_40 << " " << mu_l2_dr_40 << " " << mu_pt << endl;
        if ( mu_pt>42 && mu_dr_40<0.10 && mu_l2_dr_40<0.1){
          if (i==0) muMatched_lowpt0=true;
          else if (i==1) muMatched_lowpt1=true;
        }
      }
      if (muMatched_lowpt0) {
        muMatched_lowpt++;
      }
      if (muMatched_lowpt1) {
        muMatched_lowpt++;
      }
      //cout << "mu: " << evt->event_number() << muMatched0 << muMatched1 << muMatched << endl;

      bool trigMatched=true;
      if( category==HSG3_TWOELE && ele_hww.size()==2){
        if (elMatched==0) trigMatched=false;
        else {
          categoryTrigMatched=1;
        }
      }
      else if( category==HSG3_TWOMU && mu_hww.size()==2){
        if (muMatched==0 ) trigMatched=false;
        else {
          categoryTrigMatched=2;
        }
      }
      else if( category==HSG3_TWOMU_LOWPT && mu_hww_lowpt.size()==2){
        if (muMatched_lowpt==0 ) trigMatched=false;
        else {
          if (categoryTrigMatched==2)
            categoryTrigMatched=2;
          else
            categoryTrigMatched=4;                        
        }
      }
      else if( category==HSG3_EMU && mu_hww.size()==1 && ele_hww.size()==1){
        if (elMatched==0 && muMatched==0 ) trigMatched=false;
        else {
          categoryTrigMatched=3;
        }
      }
          
      if( category==HSG3_TWOELE && ele_hww.size()==2){
        if( elMatched==2 ) {
          double sf1 = fEGammaSF.scaleFactorTrigger(ele_hww.front()->cluster_eta()).first; 
          double sf2 = fEGammaSF.scaleFactorTrigger(ele_hww.back()->cluster_eta()).first; 
          double eff1 = 0.9894;
          double eff2 = 0.9894;
          evtTriggerSF = (eff1*sf1 + eff2*sf2 - (eff1*sf1 * eff2*sf2)) / (eff1 + eff2 - eff1 * eff2);
        }
        else if( elMatched==1){
          double sf1 = elMatched0 ? fEGammaSF.scaleFactorTrigger(ele_hww.front()->cluster_eta()).first : fEGammaSF.scaleFactorTrigger(ele_hww.back()->cluster_eta()).first; 
          evtTriggerSF = sf1;
        }
      }         
      else if( category==HSG3_TWOMU && mu_hww.size()==2 ) {
        if( muMatched==2 ){
          double effdata1 = mu_CB_staco_trigger_Eff_data(mu_hww.front()->eta(),mu_hww.front()->phi(),mu_hww.front()->pt());
          double effdata2 = mu_CB_staco_trigger_Eff_data(mu_hww.back()->eta(),mu_hww.back()->phi(),mu_hww.back()->pt());
          double effmc1 = mu_CB_staco_trigger_Eff_MC(mu_hww.front()->eta(),mu_hww.front()->phi(),mu_hww.front()->pt());
          double effmc2 = mu_CB_staco_trigger_Eff_MC(mu_hww.back()->eta(),mu_hww.back()->phi(),mu_hww.back()->pt());
          double rate_not_fired_data = (1-effdata1)*(1-effdata2);
          double rate_not_fired_mc = (1-effmc1)*(1-effmc2);
          evtTriggerSF = (1-rate_not_fired_data)/(1-rate_not_fired_mc);
          /*
            double sf1 = mu_CB_staco_trigger_SF(mu_hww.front()->eta(),mu_hww.front()->phi(),mu_hww.front()->pt());
            double sf2 = mu_CB_staco_trigger_SF(mu_hww.back()->eta(),mu_hww.back()->phi(),mu_hww.back()->pt());
            double eff1 = (is_mc) ? mu_CB_staco_trigger_Eff_MC(mu_hww.front()->eta(),mu_hww.front()->phi(),mu_hww.front()->pt()) : mu_CB_staco_trigger_Eff_data(mu_hww.front()->eta(),mu_hww.front()->phi(),mu_hww.front()->pt());
            double eff2 = (is_mc) ? mu_CB_staco_trigger_Eff_MC(mu_hww.back()->eta(),mu_hww.back()->phi(),mu_hww.back()->pt()) : mu_CB_staco_trigger_Eff_data(mu_hww.back()->eta(),mu_hww.back()->phi(),mu_hww.back()->pt());
            evtTriggerSF = (eff1*sf1 + eff2*sf2 - (eff1*sf1 * eff2*sf2)) / (eff1 + eff2 - eff1 * eff2);
          */
        }
        else if( muMatched==1 ){
          double effdata1 = muMatched0 ? mu_CB_staco_trigger_Eff_data(mu_hww.front()->eta(),mu_hww.front()->phi(),mu_hww.front()->pt()) : mu_CB_staco_trigger_Eff_data(mu_hww.back()->eta(),mu_hww.back()->phi(),mu_hww.back()->pt());
          double effmc1 = muMatched0 ? mu_CB_staco_trigger_Eff_MC(mu_hww.front()->eta(),mu_hww.front()->phi(),mu_hww.front()->pt()) : mu_CB_staco_trigger_Eff_MC(mu_hww.back()->eta(),mu_hww.back()->phi(),mu_hww.back()->pt());
          double rate_not_fired_data = (1-effdata1);
          double rate_not_fired_mc = (1-effmc1);
          evtTriggerSF = (1-rate_not_fired_data)/(1-rate_not_fired_mc);
          /*            double sf1 = muMatched0 ? mu_CB_staco_trigger_SF(mu_hww.front()->eta(),mu_hww.front()->phi(),mu_hww.front()->pt()) : mu_CB_staco_trigger_SF(mu_hww.back()->eta(),mu_hww.back()->phi(),mu_hww.back()->pt());
                        evtTriggerSF = sf1;*/
        }
      }
      else if( category==HSG3_TWOMU_LOWPT && mu_hww_lowpt.size()==2 ) {
        if( muMatched_lowpt==2 ){
          double effdata1 = mu_CB_staco_trigger_Eff_data(mu_hww_lowpt.front()->eta(),mu_hww_lowpt.front()->phi(),mu_hww_lowpt.front()->pt());
          double effdata2 = mu_CB_staco_trigger_Eff_data(mu_hww_lowpt.back()->eta(),mu_hww_lowpt.back()->phi(),mu_hww_lowpt.back()->pt());
          double effmc1 = mu_CB_staco_trigger_Eff_MC(mu_hww_lowpt.front()->eta(),mu_hww_lowpt.front()->phi(),mu_hww_lowpt.front()->pt());
          double effmc2 = mu_CB_staco_trigger_Eff_MC(mu_hww_lowpt.back()->eta(),mu_hww_lowpt.back()->phi(),mu_hww_lowpt.back()->pt());
          double rate_not_fired_data = (1-effdata1)*(1-effdata2);
          double rate_not_fired_mc = (1-effmc1)*(1-effmc2);
          evtTriggerSF = (1-rate_not_fired_data)/(1-rate_not_fired_mc);
                  
          /*            double sf1 = mu_CB_staco_trigger_SF(mu_hww_lowpt.front()->eta(),mu_hww_lowpt.front()->phi(),mu_hww_lowpt.front()->pt());
                        double sf2 = mu_CB_staco_trigger_SF(mu_hww_lowpt.back()->eta(),mu_hww_lowpt.back()->phi(),mu_hww_lowpt.back()->pt());
                        double eff1 = (is_mc) ? mu_CB_staco_trigger_Eff_MC(mu_hww_lowpt.front()->eta(),mu_hww_lowpt.front()->phi(),mu_hww_lowpt.front()->pt()) : mu_CB_staco_trigger_Eff_data(mu_hww_lowpt.front()->eta(),mu_hww_lowpt.front()->phi(),mu_hww_lowpt.front()->pt());
                        double eff2 = (is_mc) ? mu_CB_staco_trigger_Eff_MC(mu_hww_lowpt.back()->eta(),mu_hww_lowpt.back()->phi(),mu_hww_lowpt.back()->pt()) : mu_CB_staco_trigger_Eff_data(mu_hww_lowpt.back()->eta(),mu_hww_lowpt.back()->phi(),mu_hww_lowpt.back()->pt());
                        evtTriggerSF = (eff1*sf1 + eff2*sf2 - (eff1*sf1 * eff2*sf2)) / (eff1 + eff2 - eff1 * eff2);*/
        }
        else if( muMatched_lowpt==1 ){
          /*            double sf1 = muMatched_lowpt0 ? mu_CB_staco_trigger_SF(mu_hww_lowpt.front()->eta(),mu_hww_lowpt.front()->phi(),mu_hww_lowpt.front()->pt()) : mu_CB_staco_trigger_SF(mu_hww_lowpt.back()->eta(),mu_hww_lowpt.back()->phi(),mu_hww_lowpt.back()->pt());
                        evtTriggerSF = sf1;*/
          double effdata1 = muMatched_lowpt0 ? mu_CB_staco_trigger_Eff_data(mu_hww_lowpt.front()->eta(),mu_hww_lowpt.front()->phi(),mu_hww_lowpt.front()->pt()) : mu_CB_staco_trigger_Eff_data(mu_hww_lowpt.back()->eta(),mu_hww_lowpt.back()->phi(),mu_hww_lowpt.back()->pt());
          double effmc1 = muMatched_lowpt0 ? mu_CB_staco_trigger_Eff_MC(mu_hww_lowpt.front()->eta(),mu_hww_lowpt.front()->phi(),mu_hww_lowpt.front()->pt()) : mu_CB_staco_trigger_Eff_MC(mu_hww_lowpt.back()->eta(),mu_hww_lowpt.back()->phi(),mu_hww_lowpt.back()->pt());
          double rate_not_fired_data = (1-effdata1);
          double rate_not_fired_mc = (1-effmc1);
          evtTriggerSF = (1-rate_not_fired_data)/(1-rate_not_fired_mc);
        }
      }
      else if( category==HSG3_EMU && ele_hww.size()==1 && mu_hww.size()==1 ) {
        if( elMatched==1 && muMatched==1 ){
          double sf1 = elMatched0  ? fEGammaSF.scaleFactorTrigger(ele_hww.front()->cluster_eta()).first : fEGammaSF.scaleFactorTrigger(ele_hww.back()->cluster_eta()).first; 
          double effdata1 = muMatched0 ? mu_CB_staco_trigger_Eff_data(mu_hww.front()->eta(),mu_hww.front()->phi(),mu_hww.front()->pt()) : mu_CB_staco_trigger_Eff_data(mu_hww.back()->eta(),mu_hww.back()->phi(),mu_hww.back()->pt());
          double effmc1 = muMatched0 ? mu_CB_staco_trigger_Eff_MC(mu_hww.front()->eta(),mu_hww.front()->phi(),mu_hww.front()->pt()) : mu_CB_staco_trigger_Eff_MC(mu_hww.back()->eta(),mu_hww.back()->phi(),mu_hww.back()->pt());
          double rate_not_fired_data = (1-effdata1);
          double rate_not_fired_mc = (1-effmc1);
          double sf2 = (1-rate_not_fired_data)/(1-rate_not_fired_mc);
          /*            double sf2 = muMatched0  ? mu_CB_staco_trigger_SF(mu_hww.front()->eta(),mu_hww.front()->phi(),mu_hww.front()->pt()) : mu_CB_staco_trigger_SF(mu_hww.back()->eta(),mu_hww.back()->phi(),mu_hww.back()->pt());*/
          double eff1 = 0.9894;
          double eff2 = effmc1;
          evtTriggerSF = (eff1*sf1 + eff2*sf2 - (eff1*sf1 * eff2*sf2)) / (eff1 + eff2 - eff1 * eff2);
          //cout << setprecision (5) << "triggersf/sf1/sf2/eff1/eff2: " << evtTriggerSF << " " << sf1 << " " << sf2 << " " << eff1 << " " << eff2 << endl;
        }
        else if( elMatched==1 ){ 
          double sf1 = elMatched0 ? fEGammaSF.scaleFactorTrigger(ele_hww.front()->cluster_eta()).first : fEGammaSF.scaleFactorTrigger(ele_hww.back()->cluster_eta()).first; 
          evtTriggerSF = sf1;
        }
        else if( muMatched==1 ){ 
          /* double sf1 = muMatched0 ? mu_CB_staco_trigger_SF(mu_hww.front()->eta(),mu_hww.front()->phi(),mu_hww.front()->pt()) : mu_CB_staco_trigger_SF(mu_hww.back()->eta(),mu_hww.back()->phi(),mu_hww.back()->pt());*/
          double effdata1 = muMatched0 ? mu_CB_staco_trigger_Eff_data(mu_hww.front()->eta(),mu_hww.front()->phi(),mu_hww.front()->pt()) : mu_CB_staco_trigger_Eff_data(mu_hww.back()->eta(),mu_hww.back()->phi(),mu_hww.back()->pt());
          double effmc1 = muMatched0 ? mu_CB_staco_trigger_Eff_MC(mu_hww.front()->eta(),mu_hww.front()->phi(),mu_hww.front()->pt()) : mu_CB_staco_trigger_Eff_MC(mu_hww.back()->eta(),mu_hww.back()->phi(),mu_hww.back()->pt());
          double rate_not_fired_data = (1-effdata1);
          double rate_not_fired_mc = (1-effmc1);
          double sf1 = (1-rate_not_fired_data)/(1-rate_not_fired_mc);
          evtTriggerSF = sf1;
        }
      }
      //        cout << setprecision (5) << "triggersf: " << evtTriggerSF << endl;
      if( is_mc ) { 
        dg::fillh( "evtTriggerSF" , 100 , 0 , 10 , evtTriggerSF );
        dg::fillh( "evtLeptonSF" , 100 , 0 , 10 , evtLeptonSF );
        dg::fillh( "evtWeightBeforeLepton" , 100 , -5 , 5 , evtweight );
        dg::fillh( "evtWeightBeforeTrigger" , 100 , -5 , 5 , evtweight*evtLeptonSF );
        dg::fillh( "evtWeightAfterTrigger" , 100 , -5 , 5 , evtweight*evtLeptonSF*evtTriggerSF );
      }
      //evtTriggerSF=1.;
      {
        float tmpweight = outweight;
        evtweight = tmpweight * evtLeptonSF;
      }
      if( is_mc ) dg::set_global_weight( evtweight );

      dg::filleff( "c6_two_leptons" , exactly_two_leptons );

      dg::fillh( "nele_at_cut" , 10 , 0 , 10 , ele_hww.size() );
      dg::fillh( "nmu_at_cut" , 10 , 0 , 10 , mu_hww.size() );
      dg::fillh( "nmu_lowpt_at_cut" , 10 , 0 , 10 , mu_hww_lowpt.size() );
      dg::fillh( "nlepton_at_cut" , 10 , 0 , 10 , lepton_hww.size() );

      // set dilepton scale factor for subsequent studies (e.g. fake tracks study)
      {
        float tmpweight = outweight;
        evtweight = tmpweight * evtLeptonSF * evtTriggerSF;
      }
      if( is_mc ) dg::set_global_weight( evtweight );

      if( lepton_hww.size()!=2 ) { continue; }
      assert( lepton_hww.size()==2 );

      EventObjectColl< shared_ptr<const AnaParticle> >::const_iterator ileadinglep 
        = std::max_element( lepton_hww.begin() , lepton_hww.end() , bind(&AnaParticle::pt,_1)<bind(&AnaParticle::pt,_2) );
      assert( ileadinglep != lepton_hww.end() ); // already cut on exactly two leptons
      const shared_ptr<const AnaParticle>& leading_lepton( *ileadinglep );
      const shared_ptr<const AnaParticle>& subleading_lepton( ( lepton_hww.back() == leading_lepton ) ? lepton_hww.front() : lepton_hww.back() );
      //
      StudyParticle stp( "leading_lepton" , "leading lepton" );
      stp.for_only( leading_lepton );
      const bool leading_lepton_pt_cut = leading_lepton->pt() > 25.;
      dg::filleff( "c7_leading_pt" , leading_lepton_pt_cut && trigMatched);
      if( !leading_lepton_pt_cut || !trigMatched ) { continue; }

      //
      stp.repurpose( "subleading_lepton" , "subleading lepton" );
      stp.for_only( subleading_lepton );
      //
      shared_ptr<const AnaComposite> dilepton( make_shared<const AnaComposite>(leading_lepton , subleading_lepton , AnaParticle::GENERIC) );
      //
      const bool lepton_os_cut = (leading_lepton->charge()*subleading_lepton->charge()) < 0.;
      dg::filleff( "c8_os_leptons" , lepton_os_cut );
      if( !lepton_os_cut ) { continue; }
      // 
      StudyComposite stc( "dilepton" , "dilepton" );
      stc.for_only( dilepton );
      //
      const bool mll_min_cut = dilepton->mass() > (category!=HSG3_EMU ? 15. : 10.);
      dg::filleff( "c9_mll_min" , mll_min_cut );
      if( !mll_min_cut ) { continue; }
      //
      const bool mll_z_cut = (category==HSG3_EMU) || std::abs(dilepton->mass()-91.1876) > 15.;
      dg::filleff( "c10_mll_zveto" , mll_z_cut );
      if( !mll_z_cut ) { continue; }
      //
      if( met_hww.size()!=1 ) { cout << " warning: met_hww size == " << met_hww.size() << endl; }
      const shared_ptr<const AnaMET>& met( met_hww.front() );
      vector< shared_ptr<const AnaParticle> > met_rel_objects( dilepton->begin_constituent() , dilepton->end_constituent() );
      std::copy( jet_hww.cbegin() , jet_hww.cend() , back_inserter(met_rel_objects) );
      const AnaMET::ftype met_rel = met->met_rel( met_rel_objects.begin() , met_rel_objects.end() , M_PI/2. );
      const bool metrel_cut = met_rel > (category!=HSG3_EMU ? 40. : 25.);
      dg::fillh( "metrel_at_cut" , 120 , 0 , 120 , met_rel , "MET REL (GeV)" );
      dg::filleff( "c11_metrel" , metrel_cut );
      if( !metrel_cut ) { continue; }
      //
      // n jet bins
      // 
      const unsigned int njets = jet_hww.size();
      vector<string> jetcategories;
      jetcategories.push_back(HSG3_ZEROJET);
      jetcategories.push_back(HSG3_ONEJET);
      jetcategories.push_back(HSG3_TWOJET);
      jetcategories.push_back("none");
      BOOST_FOREACH( const string& jetcategory , jetcategories ) {
        assert( jetcategory != "" );
        dg::down( jetcategory , "jet bin category" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
        const bool njets_zero = njets==0;
        const bool njets_one = njets==1;
        const bool njets_two = njets==2;
        dg::fillh( "njets_at_cut" , 100 , 0 , 100 , njets , "NUMBER OF ANTI-KT4 H1 TOPO JETS" );
        StudyJet stj( "jets_at_cut" , "jets" );
        stj.add_selector( "highest_pt_jet" , AnaParticle::HighestPt<const AnaJet>() );
        stj.add_selector( "lowest_pt_jet" , AnaParticle::LowestPt<const AnaJet>() );
        stj.add_selector( "highest_eta_jet" , AnaParticle::HighestAbsEta<const AnaJet>() );
        stj.for_each( jet_hww.cbegin() , jet_hww.cend() );
        if( jetcategory==HSG3_ZEROJET ) {
          dg::filleff( "j0_0jet" , njets_zero );
        } else if( jetcategory==HSG3_ONEJET ) {
          dg::filleff( "j0_1jet" , njets_one );
        } else if( jetcategory==HSG3_TWOJET ) {
          dg::filleff( "j0_2jet" , njets_two );
        }
        if( jetcategory==HSG3_ZEROJET && !njets_zero) { continue; }
        if( jetcategory==HSG3_ONEJET && !njets_one) { continue; }
        if( jetcategory==HSG3_TWOJET && !njets_two) { continue; }
        //
        if( jetcategory==HSG3_ZEROJET ) {
          const bool ptll_cut = dilepton->pt() > 30.;
          dg::fillh( "ptll_at_cut" , 120 , 0 , 120 , dilepton->pt() , "DILEPTON p_{T} (GeV)" );
          dg::filleff( "j1_ptll" , ptll_cut );
          if( !ptll_cut ) { continue; }
        }
        if( jetcategory==HSG3_ONEJET ) {
          const bool jet_pt_gt25 = jet_hww.front()->pt()>25.;
          const bool jet_sv0_gt585 = jet_hww.front()->tag_sv0_w()>5.85;
          dg::fillh( "jet_pt_at_cut" , 120 , 0 , 120 , jet_hww.front()->pt() , "JET p_{T} (GeV)" );
          dg::fillh( "jet_sv0_at_cut" , 20 , -10 , 10 , jet_hww.front()->tag_sv0_w() , "JET TAG SV0 WEIGHT" );
          dg::filleff( "j1_pt_sv0_veto" , !jet_pt_gt25 || !jet_sv0_gt585 );
          if( jet_pt_gt25 && jet_sv0_gt585 ) { continue; }

          float lowerPx = dilepton->px()+met->px();
          float lowerPy = dilepton->py()+met->py();
          float m_nt_PtTotalX=lowerPx+jet_hww.front()->px();
          float m_nt_PtTotalY=lowerPy+jet_hww.front()->py();
          float m_nt_PtTotal = sqrt(m_nt_PtTotalX*m_nt_PtTotalX+m_nt_PtTotalY*m_nt_PtTotalY);
          dg::fillh( "total_pt_at_cut" , 120 , 0 , 120 , m_nt_PtTotal , "Total p_{T} (GeV)" );
          dg::filleff( "j2_pt_total" , m_nt_PtTotal<30. );
          if( m_nt_PtTotal>30. ) { continue; }

          //collinear approximation
          float px1 = leading_lepton->px();
          float py1 = leading_lepton->py();
          float px2 = subleading_lepton->px();
          float py2 = subleading_lepton->py();
          float MetX= met->px();
          float MetY= met->py();
          float m_nt_chi1 = py2*(px1+MetX)-px2*(py1+MetY);
          float m_nt_chi2 = px1*(py2+MetY)-py1*(px2+MetX);
          if(m_nt_chi1!=0) m_nt_chi1 = (px1*py2-px2*py1)/m_nt_chi1;
          if(m_nt_chi2!=0) m_nt_chi2 = (px1*py2-px2*py1)/m_nt_chi2;
          float m_nt_Mtautau = dilepton->mass()/sqrt(fabs(m_nt_chi1*m_nt_chi2));
          dg::fillh( "m_tautau" , 120 , 0 , 120 , m_nt_Mtautau , "Mass tau tau (GeV)" );
          bool tautau_veto= (m_nt_chi1>0 && m_nt_chi2>0 && fabs(m_nt_Mtautau-91.1876)<25.);
          dg::filleff( "j3_tautau_veto" , !tautau_veto);
          if( tautau_veto ) { continue; }
        }
        // 
        // topopogical selection (valid only for lower mass : < 170 GeV only)
        //
        const bool mll_max_cut = dilepton->mass() < 50.;
        dg::fillh( "mll_at_max_cut" , 120 , 0 , 120 , dilepton->mass() , "DILEPTON MASS (GeV)" );
        dg::filleff( "t1_mll_max" , mll_max_cut );
        if( !mll_max_cut ) { continue; }
        // 
        const AnaParticle::ftype delta_phi_ll = dilepton->max_delta_phi_opening();
        const bool dphi_cut = std::abs(delta_phi_ll) < 1.3;
        dg::fillh( "dphill_at_cut" , 100 , -M_PI , M_PI , delta_phi_ll , "#delta #phi_{ll} (rad)" );
        dg::filleff( "t2_dphi" , dphi_cut );
        if( !dphi_cut ) { continue; }
          
        float lowerEt = sqrt(dilepton->pt()*dilepton->pt()+dilepton->mass()*dilepton->mass()) + met->pt() ;
        float lowerPx = dilepton->px()+met->px();
        float lowerPy = dilepton->py()+met->py();
        AnaParticle::ftype mthiggs = sqrt(lowerEt*lowerEt-lowerPx*lowerPx-lowerPy*lowerPy);
        const bool mthiggs_cut = mthiggs > (0.75*160.) && mthiggs < 160.;
        dg::fillh( "mthiggs_at_cut" , 200 , 0 , 200 , mthiggs , "M_T Higgs (GeV)" );
        dg::filleff( "t3_mthiggs" , mthiggs_cut );
      } // for each jet bin

    } // end for each lepton channel
  } // end if do HSG3 cutflow

  { // parenthesis for mini ntuple
//      if( lepton_hww.size()!=2 || categoryTrigMatched<=0 || categoryTrigMatched>=4 ) { return;}
//      if( mu_hww_lowpt.size()!=2 || categoryTrigMatched!=4) { return; }
      if( !(lepton_hww.size()==2 && categoryTrigMatched>0 && categoryTrigMatched<4) && !(mu_hww_lowpt.size()==2 && categoryTrigMatched==4)) { return; }
    assert( (lepton_hww.size()==2 && categoryTrigMatched>0 && categoryTrigMatched<4) || (mu_hww_lowpt.size()==2 && categoryTrigMatched==4) );
    //
    // code to fill mini-ntuple
    bool do_ntuple = false;
    if( do_ntuple ) { 
      using namespace boost::assign; // bring 'operator+=()' into scope                                                                                                 
      std::vector<std::string> names = list_of("EventNumber")("RunNumber")("LBN");
      std::vector<float> values = list_of(evt->event_number())(evt->run_number())(evt->lumi_block());
      double higgs_pt = -1;
      double higgs_m = -1;
      if (evt->run_number()>=116600 && evt->run_number()<=116916){
        BOOST_FOREACH( const shared_ptr<const AnaTruthParticle>& p , evt->truth_particles() ) { 
          if(abs(p->pdg_id()==25)){
            higgs_pt = p->pt();
            higgs_m = p->m();
          }
        }
      }
      names += "higgs_pt","higgs_m";
      values += higgs_pt,higgs_m;
      //"hptWeight"
      //"triggerDecisionWeight"
      febWeight = bad_FEB ? febWeight : 1;
      names += "TrigSF","febWeight","pileupWeight","NElec","NMuon","NMuonLowPt","MVAWeight";
      if(categoryTrigMatched!=4)
          values += evtTriggerSF,febWeight,pileupWeight,ele_hww.size(),mu_hww.size(),mu_hww_lowpt.size(),evtTriggerSF*evtLeptonSF*mcweight;
      else
          values += evtTriggerSF,febWeight,pileupWeight,ele_hww.size(),mu_hww_lowpt.size(),mu_hww_lowpt.size(),evtTriggerSF*evtLeptonSF*mcweight;
      names += "EventWeight","xsec","MVAEVTWeight";
      if((*(evt->check_numeric_flag( "nev" )))){
        values += mcweight,(*(evt->check_numeric_flag( "xsec" ))),(*(evt->check_numeric_flag( "xsec" )))*evtTriggerSF*evtLeptonSF*mcweight/(*(evt->check_numeric_flag( "nev" )));
      }
      else if((*(evt->check_numeric_flag( "xsec" )))){
        values += mcweight,(*(evt->check_numeric_flag( "xsec" ))),-1;
      }
      else{
        values += mcweight,-1,-1;                
      }
//      const shared_ptr<const AnaParticle> leading_lepton;
//      const shared_ptr<const AnaParticle> subleading_lepton;
      shared_ptr<const AnaComposite> dilepton;
      if(categoryTrigMatched!=4){
          EventObjectColl< shared_ptr<const AnaParticle> >::const_iterator ileadinglep 
          = std::max_element( lepton_hww.begin() , lepton_hww.end() , bind(&AnaParticle::pt,_1)<bind(&AnaParticle::pt,_2) );
          assert( ileadinglep != lepton_hww.end() ); // already cut on exactly two leptons                                                    
          const shared_ptr<const AnaParticle> leading_lepton( *ileadinglep );
          const shared_ptr<const AnaParticle> subleading_lepton( ( lepton_hww.back() == leading_lepton ) ? lepton_hww.front() : lepton_hww.back() );
          dilepton = make_shared<const AnaComposite>(leading_lepton , subleading_lepton , AnaParticle::GENERIC);
      } else {
          EventObjectColl< shared_ptr<const AnaParticle> >::const_iterator ileadinglep 
          = std::max_element( lepton_hww_lowpt.begin() , lepton_hww_lowpt.end() , bind(&AnaParticle::pt,_1)<bind(&AnaParticle::pt,_2) );
          assert( ileadinglep != lepton_hww_lowpt.end() ); // already cut on exactly two leptons                                                    
          const shared_ptr<const AnaParticle> leading_lepton( *ileadinglep );
          const shared_ptr<const AnaParticle> subleading_lepton( ( lepton_hww_lowpt.back() == leading_lepton ) ? lepton_hww_lowpt.front() : lepton_hww_lowpt.back() );
          dilepton = make_shared<const AnaComposite>(leading_lepton , subleading_lepton , AnaParticle::GENERIC);
      }
      const shared_ptr<const AnaMET>& met( met_hww.front() );
      vector< shared_ptr<const AnaParticle> > met_rel_objects( dilepton->begin_constituent() , dilepton->end_constituent() );
      std::copy( jet_hww.cbegin() , jet_hww.cend() , back_inserter(met_rel_objects) );
      const AnaMET::ftype met_rel = met->met_rel( met_rel_objects.begin() , met_rel_objects.end() , M_PI/2. );
      names += "metrel","met","metphi";
      values += met_rel,met->pt(),met->phi();
      // pttot
      float lowerPx = dilepton->px()+met->px();
      float lowerPy = dilepton->py()+met->py();
      float m_nt_PtTotalX=lowerPx;
      float m_nt_PtTotalY=lowerPy;
      if(jet_hww.size()>0){
        m_nt_PtTotalX=lowerPx+jet_hww.front()->px();
        m_nt_PtTotalY=lowerPy+jet_hww.front()->py();
        if(jet_hww.size()==2){
          m_nt_PtTotalX=m_nt_PtTotalX+jet_hww.back()->px();
          m_nt_PtTotalY=m_nt_PtTotalY+jet_hww.back()->py();
        }
      }
      float m_nt_PtTotal = sqrt(m_nt_PtTotalX*m_nt_PtTotalX+m_nt_PtTotalY*m_nt_PtTotalY);
      // mt
      float lowerEt = sqrt(dilepton->pt()*dilepton->pt()+dilepton->mass()*dilepton->mass()) + met->pt() ;
      AnaParticle::ftype mthiggs = sqrt(lowerEt*lowerEt-lowerPx*lowerPx-lowerPy*lowerPy);
      // mtautau
      float px1 = dilepton->ith_constituent(0)->px();
      float py1 = dilepton->ith_constituent(0)->py();
      float px2 = dilepton->ith_constituent(1)->px();
      float py2 = dilepton->ith_constituent(1)->py();
      float MetX= met->px();
      float MetY= met->py();
      float m_nt_chi1 = py2*(px1+MetX)-px2*(py1+MetY);
      float m_nt_chi2 = px1*(py2+MetY)-py1*(px2+MetX);
      if(m_nt_chi1!=0) m_nt_chi1 = (px1*py2-px2*py1)/m_nt_chi1;
      if(m_nt_chi2!=0) m_nt_chi2 = (px1*py2-px2*py1)/m_nt_chi2;
      float m_nt_Mtautau = dilepton->mass()/sqrt(fabs(m_nt_chi1*m_nt_chi2)); 
      bool tautau_veto= (m_nt_chi1>0 && m_nt_chi2>0 && fabs(m_nt_Mtautau-91.1876)<25.);
      names += "PtTot","Mt","Mll","Ptll","DPhill","DRll","ztautau";
      values += m_nt_PtTotal, mthiggs, dilepton->mass(), dilepton->pt(), dilepton->max_delta_phi_opening(), dilepton->max_delta_r_opening(),tautau_veto;
      int PDGID1=-1;
      int PDGID2=-1;
      float d0sig1=-1;
      float d0sig2=-1;
      //          cout << "lepton type:" << leading_lepton->type() << endl;
      if(dilepton->ith_constituent(0)->type()==1){
        PDGID1=11;
        d0sig1=ele_hww.front()->d0_exPV_signif();
        if(dilepton->ith_constituent(1)->type()==1){
          PDGID2=11;
          d0sig2=ele_hww.back()->d0_exPV_signif();
        }
        else if(dilepton->ith_constituent(1)->type()==2){
          PDGID2=13;
          d0sig2=mu_hww.front()->d0_exPV_signif();
        }
      }
      else if(dilepton->ith_constituent(0)->type()==2){
        PDGID1=13; 
        if(categoryTrigMatched!=4)
            d0sig1=mu_hww.front()->d0_exPV_signif();
        else
            d0sig1=mu_hww_lowpt.front()->d0_exPV_signif();
        if(dilepton->ith_constituent(1)->type()==2){
          PDGID2=13;
          if(categoryTrigMatched!=4)
            d0sig2=mu_hww.back()->d0_exPV_signif();
          else
            d0sig2=mu_hww_lowpt.back()->d0_exPV_signif();
        }
        else if(dilepton->ith_constituent(1)->type()==1){
          PDGID2=11;
          d0sig2=ele_hww.front()->d0_exPV_signif();
        }
      }
      //          cout << "d0sig1 d0sig2:" << d0sig1 << " " << d0sig2 << endl;
      names += "DPhiMETLep2","PtLep1","EtaLep1","PhiLep1","PDGID1","D0sigLep1","PtLep2","EtaLep2","PhiLep2","PDGID2","D0sigLep2";
      values += met->dist_phi(dilepton->ith_constituent(1)),dilepton->ith_constituent(0)->pt(),dilepton->ith_constituent(0)->eta(),dilepton->ith_constituent(0)->phi(),PDGID1*dilepton->ith_constituent(0)->charge(),d0sig1;
      values += dilepton->ith_constituent(1)->pt(),dilepton->ith_constituent(1)->eta(),dilepton->ith_constituent(1)->phi(),PDGID2*dilepton->ith_constituent(1)->charge(),d0sig2;
      names += "NJet";
      values += jet_hww.size();
      names += "PtJet1","EtaJet1","JetProbJet1","sv0Jet1","ip3dsv1Jet1","jetcombnnJet1";
      names += "PtJet2","EtaJet2","JetProbJet2","sv0Jet2","ip3dsv1Jet2","jetcombnnJet2";
      names += "Mjj","DEtajj","NextraJet";
      if (jet_hww.size()>0){
        EventObjectColl< shared_ptr<const AnaJet> >::const_iterator ileadingjet
          = std::max_element( jet_hww.cbegin() , jet_hww.cend() , bind(&AnaJet::pt,_1)<bind(&AnaJet::pt,_2) );
        const shared_ptr<const AnaJet>& leading_jet( *ileadingjet );
        values += leading_jet->pt(), leading_jet->eta(), leading_jet->tag_jetp_w(),leading_jet->tag_sv0_w(),leading_jet->tag_cmb_w(),leading_jet->tag_cmbnn_w();
        //            cout << "jetw: " << leading_jet->tag_jetp_w() << " " << leading_jet->tag_sv0_w() << " " << leading_jet->tag_cmb_w() << " " << leading_jet->tag_cmbnn_w() << endl;
        if (jet_hww.size()>1){
          const shared_ptr<const AnaJet>& subleading_jet( ( jet_hww.back() == leading_jet ) ? jet_hww.front() : jet_hww.back() );
          shared_ptr<const AnaComposite> dijet( make_shared<const AnaComposite>(leading_jet , subleading_jet , AnaParticle::GENERIC) );
          values += subleading_jet->pt(), subleading_jet->eta(), subleading_jet->tag_jetp_w(),subleading_jet->tag_sv0_w(),subleading_jet->tag_cmb_w(),subleading_jet->tag_cmbnn_w();
          values += dijet->mass(), leading_jet->dist_eta(subleading_jet), jet_hww.size();
        }
        else{
          values += -1,-1,-1,-1,-1,-1;
          values += -1,-1,-1;
        }
      }
      else{
        values += -1,-1,-1,-1,-1,-1;
        values += -1,-1,-1,-1,-1,-1;
        values += -1,-1,-1;
      }
      names+="ElecEffScale";                                                                                                                                                           
      if(abs(PDGID1)==11 || abs(PDGID2)==11){
          values+=evtElectronSF;
      }
      else{
        values+=0;
      }
      names+="MuonEffScale";
      if(abs(PDGID1)==13 || abs(PDGID2)==13){
          values+=evtMuonSF;
      }
      else{
        values+=0;
      }
      names+="febPos";
      values+=bad_FEB;
      dg::fillnt("ntuple",values,names);
    } // end if fill mini ntuple
  } // end parenthesis for mini ntuple

}
