

// H->WW analysis code.

#include <algorithm>
#include <ext/algorithm>
#include <functional>
#include <cassert>
#include <cmath>
#include <string>
#include <iostream>
#include <iomanip>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/scope_exit.hpp>
#include <boost/foreach.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/serialization/nvp.hpp>
#include "TrigFTKAna/AnaConfiguration.hpp"
#include "TrigFTKAna/AnaEventMgr.hpp"
#include "TrigFTKAna/AnaElectron.hpp"
#include "TrigFTKAna/AnaMuon.hpp"
#include "TrigFTKAna/AnaPhoton.hpp"
#include "TrigFTKAna/AnaJet.hpp"
#include "TrigFTKAna/AnaMET.hpp"
#include "TrigFTKAna/AnaVertex.hpp"
#include "TrigFTKAna/AnaTrigger.hpp"
#include "TrigFTKAna/AnaTrack.hpp"
#include "TrigFTKAna/AnaZ.hpp"
#include "TrigFTKAna/AnaW.hpp"
#include "TrigFTKAna/AnaZCrossSectionCandidate.hpp"
#include "TrigFTKAna/AnaWCrossSectionCandidate.hpp"
#include "TrigFTKAna/AnaHWWCandidate.hpp"
#include "TrigFTKAna/GoodRunList.hpp"
#include "TrigFTKAna/EventObjectColl.hpp"
#include "DataGraphics/DataGraphics.hh"
#include "DataGraphics/DgCutSet.hh"
#include "DataGraphics/DgSTL.hh"
#include <DataGraphics/DgXMLMap.hh>
#include "TrigFTKAna/StudyComposite.hpp"
#include "TrigFTKAna/StudyMET.hpp"
#include "TrigFTKAna/StudyGGHiggs.hpp"
#include "TrigFTKAna/StudyWCrossSection.hpp"
#include "TrigFTKAna/StudyZCrossSection.hpp"
#include "TrigFTKAna/CompositeGenerator.hpp"
#include "TrigFTKAna/Conference.hpp"

using namespace std;
using namespace DataGraphics;
using namespace boost;

const std::string& which_trigger_mu(int r, bool mc);
const std::string& which_trigger_el(int r, bool mc);
const std::string& which_trigger_phot(int r, bool mc);
const std::string& which_period(int r);
const double pi = 3.1416; 

template< typename lepIterT , typename metIterT , typename jetIterT , typename vtxIterT , typename outputIterT >
void build_w_candidates( lepIterT lep_begin , lepIterT lep_end ,
                         metIterT met_begin , metIterT met_end ,
                         jetIterT jet_begin , jetIterT jet_end ,
                         vtxIterT vtx_begin , vtxIterT vtx_end ,
                         outputIterT output_iterator );
template< typename lepIterT , typename metIterT , typename jetIterT , typename vtxIterT , typename outputIterT >
void build_z_candidates( lepIterT lep_begin , lepIterT lep_end ,
                         metIterT met_begin , metIterT met_end ,
                         jetIterT jet_begin , jetIterT jet_end ,
                         vtxIterT vtx_begin , vtxIterT vtx_end ,
                         outputIterT output_iterator );
template< typename overlapIterT , typename refIterT , typename outputIterT >
void overlap_removal( overlapIterT overlap_begin , overlapIterT overlap_end ,
                      refIterT ref_begin , refIterT ref_end ,
                      const AnaParticle::ftype& max_overlap_dr ,
                      outputIterT output_iterator );
// PILE UP WEIGHTS
class PileupWeights : public DgXMLIntMap<double>
{
public:
  const double weight( const int& nvtx ) const {
    if(nvtx>=7) return value(7);
    return value(nvtx);
  }
  PileupWeights() : DgXMLIntMap<double>(1.0) {}
  virtual ~PileupWeights() {}
  void apply_example() {
    std::cout << "Loading pileup weights from December 2010, preselection level" << std::endl;
    // melachrinos calculation
    /*
      _data[0] = 1.;
      _data[1] = 1.58366;
      _data[2] = 1.10086;
      _data[3] = 0.87356;
      _data[4] = 0.73579;
      _data[5] = 0.66055;
      _data[6] = 0.70938;
      _data[7] = 0.74940;
    */
    /* SM WW float vtx_cf[7]={1.6169, 1.1216, 0.8561, 0.7070, 0.6859, 0.6218, 0.6168}; */
    _data[0] = 1.;
    _data[1] = 1.6169;
    _data[2] = 1.1216;
    _data[3] = 0.8561;
    _data[4] = 0.7070;
    _data[5] = 0.6859;
    _data[6] = 0.6218;
    _data[7] = 0.6168;
  }
  void save_example( const std::string& filename ) {
    apply_example();
    // save( filename );
  }
};



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
  AnaConfiguration::configure("ana_hww",argc,argv);
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
  // grl.add( GoodRunList::MUON , "good_run_lists/Wmu_GRLs/WjetsMu_MayReprAB_FirstPassCDEFGHI_GRL.xml");
  // Bill Quayle uses muon list for both electrons and muons.
  // grl.add( GoodRunList::ELECTRON , "good_run_lists/Wmu_GRLs/WjetsMu_MayReprAB_FirstPassCDEFGHI_GRL.xml");
  // grl.add( GoodRunList::MUON , "good_run_lists/Wmu_GRLs/WjetsMu_MayReprAB_FirstPassCDEFGHI_GRL.xml");
  // This is Bill's GRL:
  grl.add( GoodRunList::ELECTRON , "good_run_lists/WW_GRL_periods_A-I.xml");
  grl.add( GoodRunList::MUON , "good_run_lists/WW_GRL_periods_A-I.xml");
  //
  // pileup vertexing
  boost::shared_ptr<PileupWeights> pw(new PileupWeights());
  const bool do_pileup = AnaConfiguration::apply_pileup_weights();
  if(do_pileup) {
    pw->apply_example();
    dg::set_global_weight(1.0);
  }

  // define primary vertex selection
  // ================================================================
  EventObjectColl< shared_ptr<const AnaVertex> > primary_vertices;
  DgCutSet< shared_ptr<const AnaVertex> > primary_vertex_selector( "primary_vertex_selector" , primary_vertices.attach() );
  primary_vertex_selector.add_uu( "n_tracks", bind(&AnaVertex::ntracks,_1)>=3, bind( &dg::fillh , _1 , 50 , 0 , 50 , bind(&AnaVertex::ntracks,_2) ) );
  // primary_vertex_selector.add_uu( "pv_z", bind( std::ptr_fun<AnaVertex::ftype,AnaVertex::ftype>(std::abs) , bind(&AnaVertex::z,_1) ) < 150./*mm*/, 
  //                               bind( &dg::fillh , _1 , 90 , -300 , 300 , bind(&AnaVertex::z,_2) ) );
  //
  // set up particle selectors
  // ================================================================
  //
  // muon selection
  // ----------------------------------------------------------------
  // source muon collection. if this is mc, the muons here are smeared 
  // according to the Muon Combined Performance prescription. if this is data,
  // they are taken directly from the input ntuple.
  EventObjectColl< shared_ptr<const AnaMuon> > mu_event;
  // electrons SM WW observation
  EventObjectColl< shared_ptr<const AnaElectron> > ele_smww_all;
  EventObjectColl< shared_ptr<const AnaElectron> > ele_smww;
  DgCutSet< shared_ptr<const AnaElectron> > ele_smww_base( "ele_smww_base" );
  ele_smww_base.add_uu( "author" , 
                        bind(&AnaElectron::author,_1)==1 || bind(&AnaElectron::author,_1)==3 ,
                        bind( &dg::fillh , _1 , 10 , 0 , 10 , bind(&AnaElectron::author,_2) ) );
  ele_smww_base.add_uu( "lar_otx" , bind(&AnaElectron::pass_otx,_1)==true ,
                        bind( &dg::fillh , _1 , 40 , -4 , 4 , bind(&AnaElectron::cluster_eta,_2) ) );
  ele_smww_base.add_uu( "pt" , bind(&AnaElectron::cluster_pt,_1)>20 , 
                        bind(&dg::fillh, _1 , 100 , 0 , 200 , bind(&AnaElectron::cluster_pt,_2) ) );
  ele_smww_base.add_uu( "is_fiducial_247" , 
                        bind(&AnaElectron::cluster_eta,_1) < 2.47  && bind(&AnaElectron::cluster_eta,_1) > -2.47 ,
                        bind( &dg::fillh , _1 , 40 , -4 , 4 , bind(&AnaElectron::cluster_eta,_2) ) );
  ele_smww_base.add_uu( "is_fiducial_forward_crack" , 
                        ( bind(&AnaElectron::cluster_eta,_1) <= 1.37 && bind(&AnaElectron::cluster_eta,_1) >= -1.37 ) 
                        || bind(&AnaElectron::cluster_eta,_1) >= 1.52 || bind(&AnaElectron::cluster_eta,_1) <= -1.52,
                        bind( &dg::fillh , _1 , 40 , -4 , 4 , bind(&AnaElectron::cluster_eta,_2) ) );
  DgCutSet< shared_ptr<const AnaElectron> > ele_smww_selector( "ele_smww_selector" , ele_smww_all.attach() );
  ele_smww_selector.based_on(ele_smww_base);
  ele_smww_selector.add_uu( "is_robusterTight" , bind(&AnaElectron::is_ElectronRobusterTight_v15,_1)==true , 
                            bind( &dg::fillh , _1 , 40 , -4 , 4 , bind(&AnaElectron::phi,_2) , string("#phi") ) );
  ele_smww_selector.add_uu( "from_beamline_z0" , 
                            bind(&AnaElectron::trackz0pv,_1) < 10.  && bind(&AnaElectron::trackz0pv,_1) > -10. ,
                            bind( &dg::fillh , _1 , 100 , -10 , 10 , bind(&AnaElectron::trackz0pv,_2) ) );
  ele_smww_selector.add_uu( "from_beamline_d0_sig" , 
                            bind(&AnaElectron::tracksigd0pv_ratio,_1) < 10.  && bind(&AnaElectron::tracksigd0pv_ratio,_1) > -10. ,
                            bind( &dg::fillh , _1 , 100 , -10 , 10 , bind(&AnaElectron::tracksigd0pv_ratio,_2) ) );
  ele_smww_selector.add_uu( "et_cone_30_6" , bind(&AnaElectron::et_cone,_1,AnaElectron::CONE30)<6. , 
                            bind(&dg::fillh, _1 , 100 , 0 , 100 , bind(&AnaElectron::et_cone,_2,AnaElectron::CONE30) ) );
  // electrons SM WW observation not robuster tight, but medium (for qcd studies)
  EventObjectColl< shared_ptr<const AnaElectron> > ele_smww_medium;
  DgCutSet< shared_ptr<const AnaElectron> > ele_smww_medium_selector( "ele_smww_medium_selector" , ele_smww_medium.attach() );
  
  ele_smww_medium_selector.based_on(ele_smww_base);  
  ele_smww_medium_selector.add_uu( "is_not_robusterTight" , bind(&AnaElectron::is_ElectronRobusterTight_v15,_1)==false , 
                                   bind( &dg::fillh , _1 , 40 , -4 , 4 , bind(&AnaElectron::phi,_2) , string("#phi") ) );
  ele_smww_medium_selector.add_uu( "is_RobustMedium" , 
                                   bind(&AnaElectron::is_ElectronRobustMedium_v15,_1) == true ,
                                   bind( &dg::filleff , _1 , bind(&AnaElectron::is_ElectronRobustMedium_v15,_2) == true ) );
  ele_smww_medium_selector.add_uu( "from_beamline_z0" , 
                                   bind(&AnaElectron::trackz0pv,_1) < 10.  && bind(&AnaElectron::trackz0pv,_1) > -10. ,
                                   bind( &dg::fillh , _1 , 100 , -10 , 10 , bind(&AnaElectron::trackz0pv,_2) ) );
  ele_smww_medium_selector.add_uu( "from_beamline_d0_sig" , 
                                   bind(&AnaElectron::tracksigd0pv_ratio,_1) < 10.  && bind(&AnaElectron::tracksigd0pv_ratio,_1) > -10. ,
                                   bind( &dg::fillh , _1 , 100 , -10 , 10 , bind(&AnaElectron::tracksigd0pv_ratio,_2) ) );
  ele_smww_medium_selector.add_uu( "et_cone_30_6" , bind(&AnaElectron::et_cone,_1,AnaElectron::CONE30)<6. , 
                                   bind(&dg::fillh, _1 , 100 , 0 , 100 , bind(&AnaElectron::et_cone,_2,AnaElectron::CONE30) ) );
  // electrons SM WW observation not isolated (for qcd studies)
  EventObjectColl< shared_ptr<const AnaElectron> > ele_smww_revIso;
  DgCutSet< shared_ptr<const AnaElectron> > ele_smww_revIso_selector( "ele_smww_revIso_selector" , ele_smww_revIso.attach() );
  ele_smww_revIso_selector.based_on(ele_smww_base);  
  ele_smww_revIso_selector.add_uu( "is_not_robusterTight" , bind(&AnaElectron::is_ElectronRobusterTight_v15,_1)==false , 
                                   bind( &dg::fillh , _1 , 40 , -4 , 4 , bind(&AnaElectron::phi,_2) , string("#phi") ) );
  ele_smww_revIso_selector.add_uu( "from_beamline_z0" , 
                                   bind(&AnaElectron::trackz0pv,_1) < 10.  && bind(&AnaElectron::trackz0pv,_1) > -10. ,
                                   bind( &dg::fillh , _1 , 100 , -10 , 10 , bind(&AnaElectron::trackz0pv,_2) ) );
  ele_smww_revIso_selector.add_uu( "from_beamline_d0_sig" , 
                                   bind(&AnaElectron::tracksigd0pv_ratio,_1) < 10.  && bind(&AnaElectron::tracksigd0pv_ratio,_1) > -10. ,
                                   bind( &dg::fillh , _1 , 100 , -10 , 10 , bind(&AnaElectron::tracksigd0pv_ratio,_2) ) );
  ele_smww_revIso_selector.add_uu( "et_cone_30_gt6" , bind(&AnaElectron::et_cone,_1,AnaElectron::CONE30)>6. , 
                                   bind(&dg::fillh, _1 , 100 , 0 , 100 , bind(&AnaElectron::et_cone,_2,AnaElectron::CONE30) ) );
  EventObjectColl< shared_ptr<const AnaElectron> > ele_smww_medium_msn;
  DgCutSet< shared_ptr<const AnaElectron> > ele_smww_medium_msn_selector( "ele_smww_medium_msn_selector" , ele_smww_medium_msn.attach() );
  ele_smww_medium_msn_selector.add_uu( "author" , 
                                       bind(&AnaElectron::author,_1)==1 || bind(&AnaElectron::author,_1)==3 ,
                                       bind( &dg::fillh , _1 , 10 , 0 , 10 , bind(&AnaElectron::author,_2) ) );
  ele_smww_medium_msn_selector.add_uu( "pt" , bind(&AnaElectron::pt,_1)>20 , 
                                       bind(&dg::fillh, _1 , 100 , 0 , 200 , bind(&AnaElectron::pt,_2) ) );
  ele_smww_medium_msn_selector.add_uu( "is_fiducial_247" , bind(&AnaElectron::abseta,_1)<2.47 , bind( &dg::fillh , _1 , 40 , -4 , 4 , bind(&AnaElectron::eta,_2) ) );
  ele_smww_medium_msn_selector.add_uu( "lar_otx" , bind(&AnaElectron::pass_otx,_1)==true ,
                                       bind( &dg::fillh , _1 , 40 , -4 , 4 , bind(&AnaElectron::eta,_2) ) );
  ele_smww_medium_msn_selector.add_uu( "is_RobustMedium" , 
                                       bind(&AnaElectron::is_ElectronRobustMedium_v15,_1) == true ,
                                       bind( &dg::filleff , _1 , bind(&AnaElectron::is_ElectronRobustMedium_v15,_2) == true ) );
    
  // photons SM WW observation
  EventObjectColl<  boost::shared_ptr<const AnaPhoton>  > photon_smww;
  // W/Z Baseline 2010 selection
  DgCutSet<  boost::shared_ptr<const AnaPhoton>  > photon_smww_selector( "photon_smww_selector" , photon_smww.attach() );
  photon_smww_selector.add_uu( "author" , 
                               bind(&AnaPhoton::author,_1)==4,
                               bind( &dg::fillh , _1 , 10 , 0 , 10 , bind(&AnaPhoton::author,_2) ) );
  photon_smww_selector.add_u( "passOTX" , bind(&AnaPhoton::pass_otx,_1)==true );
  photon_smww_selector.add_uu( "cluster_eta" , 
                               ( bind( std::ptr_fun<AnaPhoton::ftype,AnaPhoton::ftype>(std::abs) , 
                                       bind(&AnaPhoton::cluster_eta,_1)) < 2.47  &&
                                 ( bind(std::ptr_fun<AnaPhoton::ftype,AnaPhoton::ftype>(std::abs) , 
                                        bind(&AnaPhoton::cluster_eta,_1)) < 1.37 ||
                                   bind(std::ptr_fun<AnaPhoton::ftype,AnaPhoton::ftype>(std::abs) , 
                                        bind(&AnaPhoton::cluster_eta,_1)) > 1.52 ) ) ,
                               bind( &dg::fillh , _1 , 60 , -3 , 3 , bind(&AnaPhoton::cluster_eta,_2) ) );
  photon_smww_selector.add_uu( "cluster_ET" ,
                               bind(&AnaPhoton::cluster_pt,_1) > 20. ,
                               bind(&dg::fillh,_1,100,0,100,bind(&AnaPhoton::cluster_pt,_2)) );
  photon_smww_selector.add_uu( "isEM_PhotonLoose" ,
                               bind(&AnaPhoton::is_PhotonLoose,_1) == true ,
                               bind( &dg::filleff , _1 , bind(&AnaPhoton::is_PhotonLoose,_2) == true ) );  
  photon_smww_selector.add_uu( "etCone30ratio" , 
                               bind(&AnaPhoton::et_cone_ratio,_1,AnaPhoton::CONE30) > 0.15,
                               bind(&dg::fillh,_1,100,0,1,bind(&AnaPhoton::et_cone_ratio,_2,AnaPhoton::CONE30)) );
  // SM WW muons
  EventObjectColl< shared_ptr<const AnaMuon> > mu_smww;
  DgCutSet< shared_ptr<const AnaMuon> > mu_smww_base( "mu_smww_base" );
  DgCutSet< shared_ptr<const AnaMuon> > mu_smww_selector( "mu_smww_selector" , mu_smww.attach() );
  mu_smww_base.add_uu( "is_combined" , bind(&AnaMuon::is_combined,_1)==true , 
                       bind( &dg::fillh , _1 , 48 , -M_PI , M_PI , bind(&AnaMuon::phi,_2) , "#phi" ) );
  mu_smww_base.add_uu( "author" , 
                       bind(&AnaMuon::primary_author,_1)==6 || bind(&AnaMuon::primary_author,_1)==7,
                       bind( &dg::fillh , _1 , 10 , 0 , 10 , bind(&AnaMuon::primary_author,_2) ) );
  mu_smww_base.add_uu( "pt" , bind(&AnaMuon::pt,_1)>20 , 
                       bind(&dg::fillh, _1 , 100 , 0 , 200 , bind(&AnaMuon::pt,_2) ) );
  mu_smww_base.add_uu( "ptDiffRatio", 
                       bind( std::ptr_fun<AnaMuon::ftype,AnaMuon::ftype>(std::abs) , bind(&AnaMuon::pt_diff_ratio,_1)) < 0.4 || 
                       bind(&AnaMuon::exms_pt,_1) > 50.,
                       bind(&dg::fillh, _1 , 100 , 0 , 100 , bind(&AnaMuon::exms_pt,_2) ) );
  mu_smww_base.add_u( "trackerHits", bind(&AnaMuon::passes_hits_mcp_v15,_1)==true );
  mu_smww_base.add_uu( "matchchi2" , bind(&AnaMuon::matchchi2,_1)<150 , 
                       bind(&dg::fillh, _1 , 400 , 0 , 400 , bind(&AnaMuon::matchchi2,_2) ) );
  mu_smww_base.add_uu( "is_fiducial_24" , 
                       bind(&AnaMuon::abseta,_1)<2.4 , 
                       bind( &dg::fillh , _1 , 40 , -4 , 4 , bind(&AnaMuon::abseta,_2) ) );
  mu_smww_base.add_uu( "from_beamline_z0" , bind( &AnaTrack::exPV_z0 , bind( &AnaMuon::track , _1 ) ) < 10. ,
                       bind( &dg::fillh , _1 , 100 , -100 , 100 , bind( &AnaTrack::exPV_z0 , bind( &AnaMuon::track , _2 ) ) ) );  
  mu_smww_base.add_uu( "from_beamline_d0" , bind( &AnaTrack::exPV_d0 , bind( &AnaMuon::track , _1 ) ) < 1 ,
                       bind( &dg::fillh , _1 , 100 , -100 , 100 , bind( &AnaTrack::exPV_d0 , bind( &AnaMuon::track , _2 ) ) ) );
  mu_smww_selector.based_on(mu_smww_base);
  mu_smww_selector.add_uu( "pt_cone_20_st1_0" , bind(&AnaMuon::pt_cone,_1,AnaMuon::CONE20) < 1.0 , 
                           bind(&dg::fillh, _1 , 100 , 0. , 10 , bind(&AnaMuon::pt_cone,_2,AnaMuon::CONE20) ) );
  /*  mu_smww_selector.add_uu( "pt_cone_20_ratio_st1" , bind(&AnaMuon::pt_cone_ratio,_1,AnaMuon::CONE20) < 0.1 , 
      bind(&dg::fillh, _1 , 100 , 0. , 10 , bind(&AnaMuon::pt_cone_ratio,_2,AnaMuon::CONE20) ) );*/
  /*
    mu_smww_selector.add_uu( "pt_cone_30_st1_8" , bind(&AnaMuon::pt_cone,_1,AnaMuon::CONE30) < 1.8 , 
    bind(&dg::fillh, _1 , 100 , 0. , 100 , bind(&AnaMuon::pt_cone,_2,AnaMuon::CONE30) ) );
    mu_smww_selector.add_uu( "pt_cone_40_st1_8" , bind(&AnaMuon::pt_cone,_1,AnaMuon::CONE40) < 1.8 , 
    bind(&dg::fillh, _1 , 100 , 0. , 100 , bind(&AnaMuon::pt_cone,_2,AnaMuon::CONE40) ) );
  */
  // SM WW muons reversed isolation for QCD
  EventObjectColl< shared_ptr<const AnaMuon> > mu_smww_revIso;
  DgCutSet< shared_ptr<const AnaMuon> > mu_smww_revIso_selector( "mu_smww_revIso_selector" , mu_smww_revIso.attach() );
  mu_smww_revIso_selector.based_on(mu_smww_base);
  mu_smww_revIso_selector.add_uu( "pt_cone_20_gt1_0" , bind(&AnaMuon::pt_cone,_1,AnaMuon::CONE20) > 1.0 , 
                                  bind(&dg::fillh, _1 , 100 , 0. , 100 , bind(&AnaMuon::pt_cone,_2,AnaMuon::CONE20) ) );

  EventObjectColl< shared_ptr<const AnaParticle> > lepton_smww;
  // SM WW jets
  EventObjectColl< shared_ptr<const AnaJet> > jet_smww_all;
  EventObjectColl< shared_ptr<const AnaJet> > jet_smww;
  DgCutSet< shared_ptr<const AnaJet> > jet_smww_selector( "jet_smww_selector" , jet_smww_all.attach() );
  jet_smww_selector.add_u( "algorithm_antikt4" , bind(&AnaJet::is_atlas_anti_kt_cone_4_h1_topo,_1)==true );
  jet_smww_selector.add_u( "is_good" , bind(&AnaJet::is_good_jetetmiss_v15,_1)==true );
  jet_smww_selector.add_uu( "pt_20" , 
                            bind<AnaJet::ftype>(std::multiplies<AnaJet::ftype>(),
                                                bind(&TLorentzVector::Pt,bind(&AnaJet::em_scale_momentum,_1)),
                                                bind(&AnaJet::em_scale_correction,_1)) >= 20.,
                            bind(&dg::fillh,_1,100,0,100,
                                 bind<AnaJet::ftype>(std::multiplies<AnaJet::ftype>(),
                                                     bind(&TLorentzVector::Pt,bind(&AnaJet::em_scale_momentum,_2)),
                                                     bind(&AnaJet::em_scale_correction,_2))));
  jet_smww_selector.add_uu( "fiducial_45" , 
                            bind(std::ptr_fun<AnaJet::ftype,AnaJet::ftype>(std::abs),
                                 bind(&TLorentzVector::Eta,bind(&AnaJet::em_scale_momentum,_1))) < 4.5 ,
                            bind(&dg::fillh,_1,100,-5,5, 
                                 bind(&TLorentzVector::Eta,bind(&AnaJet::em_scale_momentum,_2))) );
  jet_smww_selector.add_uu( "pt_30" , 
                            bind<AnaJet::ftype>(std::multiplies<AnaJet::ftype>(),
                                                bind(&TLorentzVector::Pt,bind(&AnaJet::em_scale_momentum,_1)),
                                                bind(&AnaJet::em_scale_correction,_1)) >= 30.,
                            bind(&dg::fillh,_1,100,0,100,
                                 bind<AnaJet::ftype>(std::multiplies<AnaJet::ftype>(),
                                                     bind(&TLorentzVector::Pt,bind(&AnaJet::em_scale_momentum,_2)),
                                                     bind(&AnaJet::em_scale_correction,_2))));
  // MET selection
  // ================================================================
  // source MET collection. MET is recalculated for |eta|<4.5 towers
  // and the muon corrections applied to staco muons.
  EventObjectColl< shared_ptr<const AnaMET> > met_hww_event;
  // selected METs
  EventObjectColl< shared_ptr<const AnaMET> > met_hww;
  DgCutSet< shared_ptr<const AnaMET> > met_hww_selector( "met_hww_selector" , met_hww.attach() );
  //
  met_hww_selector.add_u( "met_type" , bind(&AnaMET::met_type,_1) == AnaMET::LOCHADTOPOCEF );
  met_hww_selector.add_u( "missing_et_good" , bind( &AnaMET::is_good, _1 ) == true );
  //  met_wjets_selector.add_uu( "missing_et" , bind( &AnaMET::met , _1 ) > 0. ,  // don't cut, just plot
  //			     bind(&dg::fillh,_1,100,0,100, bind(&AnaMET::met,_2)) );

  // W boson selection
  //  input lists SM WW
  EventObjectColl< shared_ptr<const AnaW> > enu_smww;
  EventObjectColl< shared_ptr<const AnaW> > munu_smww;
  EventObjectColl< shared_ptr<const AnaW> > photnu_smww;
  EventObjectColl< shared_ptr<const AnaW> > munu_smww_revIso;
  EventObjectColl< shared_ptr<const AnaW> > enu_smww_revIso;
  EventObjectColl< shared_ptr<const AnaW> > enu_smww_medium;
  EventObjectColl< shared_ptr<const AnaW> > lnu_smww;
  EventObjectColl< shared_ptr<const AnaW> > wjets_smww;
  EventObjectColl< shared_ptr<const AnaW> > w_jj_closestmw_smww;
  
  EventObjectColl< shared_ptr<const AnaWCrossSectionCandidate> > w_munu_jan20;
  EventObjectColl< shared_ptr<const AnaWCrossSectionCandidate> > w_enu_jan20;
  DgCutSet< shared_ptr<const AnaWCrossSectionCandidate> > w_jan20_selector( "w_jan20_selector" );
  DgCutSet< shared_ptr<const AnaWCrossSectionCandidate> > w_munu_jan20_selector( "w_munu_jan20_selector" , w_munu_jan20.attach() );
  DgCutSet< shared_ptr<const AnaWCrossSectionCandidate> > w_enu_jan20_selector( "w_enu_jan20_selector" , w_enu_jan20.attach() );
  w_jan20_selector.add_uu( "exactly_one_lepton" ,
                           bind(&AnaWCrossSectionCandidate::exactly_one_lepton,_1) == true ,
                           bind(&dg::fillh,_1,10,0,10,bind(&AnaWCrossSectionCandidate::nleptons,_2)) );
  w_jan20_selector.add_uu( "lepton_pt_gt30" ,
                           bind(&AnaParticle::pt,bind(&AnaWCrossSectionCandidate::lepton,_1)) > 30. ,
                           bind(dg::fillh,_1,150,0,150,bind(&AnaParticle::pt,bind(&AnaWCrossSectionCandidate::lepton,_2))) );
  w_jan20_selector.add_uu( "exactly_one_met" , 
                           bind(&AnaWCrossSectionCandidate::exactly_one_met,_1) == true ,
                           bind(&dg::filleff,_1,bind(&AnaWCrossSectionCandidate::exactly_one_met,_2) == true) );
  w_jan20_selector.add_uu( "met_gt_30" ,
                           bind(&AnaParticle::pt,bind(&AnaWCrossSectionCandidate::met,_1)) > 30. ,
                           bind(dg::fillh,_1,150,0,150,bind(&AnaParticle::pt,bind(&AnaWCrossSectionCandidate::met,_2))) );
  w_munu_jan20_selector.based_on( w_jan20_selector ); // identical selection, different input and output lists
  w_enu_jan20_selector.based_on( w_jan20_selector ); // identical selection, different input and output lists
  // Higgs selection SM WW
  EventObjectColl< shared_ptr<const AnaHiggs> > h_smww;
  EventObjectColl< shared_ptr<const AnaHWWCandidate> > hww_smww;
  EventObjectColl< shared_ptr<const AnaHiggs> > he_smww;
  EventObjectColl< shared_ptr<const AnaHWWCandidate> > heww_smww;
  EventObjectColl< shared_ptr<const AnaHiggs> > hmu_smww;
  EventObjectColl< shared_ptr<const AnaHWWCandidate> > hmuww_smww;
  EventObjectColl< shared_ptr<const AnaHiggs> > hphot_smww;
  EventObjectColl< shared_ptr<const AnaHWWCandidate> > hphotww_smww;
  EventObjectColl< shared_ptr<const AnaHiggs> > he_smww_medium;
  EventObjectColl< shared_ptr<const AnaHWWCandidate> > heww_smww_medium;
  EventObjectColl< shared_ptr<const AnaHiggs> > he_smww_revIso;
  EventObjectColl< shared_ptr<const AnaHWWCandidate> > heww_smww_revIso;
  EventObjectColl< shared_ptr<const AnaHiggs> > hmu_smww_revIso;
  EventObjectColl< shared_ptr<const AnaHWWCandidate> > hmuww_smww_revIso;
  // create output collections change names
  EventObjectColl< shared_ptr<const AnaHWWCandidate> > ggh_e2j_smww;
  EventObjectColl< shared_ptr<const AnaHWWCandidate> > ggh_e3j_smww;
  EventObjectColl< shared_ptr<const AnaHWWCandidate> > vbfh_ege4j_smww;
  EventObjectColl< shared_ptr<const AnaHWWCandidate> > ggh_mu2j_smww;
  EventObjectColl< shared_ptr<const AnaHWWCandidate> > ggh_mu3j_smww;
  EventObjectColl< shared_ptr<const AnaHWWCandidate> > vbfh_muge4j_smww;
  EventObjectColl< shared_ptr<const AnaHWWCandidate> > ggh_phot2j_smww;
  EventObjectColl< shared_ptr<const AnaHWWCandidate> > ggh_phot3j_smww;
  EventObjectColl< shared_ptr<const AnaHWWCandidate> > vbfh_photge4j_smww;
  EventObjectColl< shared_ptr<const AnaHWWCandidate> > ggh_e2j_smww_medium;
  EventObjectColl< shared_ptr<const AnaHWWCandidate> > ggh_e3j_smww_medium;
  EventObjectColl< shared_ptr<const AnaHWWCandidate> > vbfh_ege4j_smww_medium;
  EventObjectColl< shared_ptr<const AnaHWWCandidate> > ggh_e2j_smww_revIso;
  EventObjectColl< shared_ptr<const AnaHWWCandidate> > ggh_e3j_smww_revIso;
  EventObjectColl< shared_ptr<const AnaHWWCandidate> > vbfh_ege4j_smww_revIso;
  EventObjectColl< shared_ptr<const AnaHWWCandidate> > ggh_mu2j_smww_revIso;
  EventObjectColl< shared_ptr<const AnaHWWCandidate> > ggh_mu3j_smww_revIso;
  EventObjectColl< shared_ptr<const AnaHWWCandidate> > vbfh_muge4j_smww_revIso;

  DgCutSet< shared_ptr<const AnaHWWCandidate> > h2j_sel( "h2j" );
  DgCutSet< shared_ptr<const AnaHWWCandidate> > h3j_sel( "h3j" );
  DgCutSet< shared_ptr<const AnaHWWCandidate> > hge4j_sel( "hge4j" );
  h2j_sel.add_u( "exactly_two_jets" ,
                 bind(&AnaHWWCandidate::event_njets,_1) == 2 );
  h3j_sel.add_u( "exactly_three_jets" ,
                 bind(&AnaHWWCandidate::event_njets,_1) == 3 );
  hge4j_sel.add_u( "ge_four_jets" ,
                   bind(&AnaHWWCandidate::event_njets,_1) >= 4 );

  // ggH selection
  // ----------------------------------------------------------------
  EventObjectColl< shared_ptr<const AnaHWWCandidate> > gghww_sm_jan20;
  EventObjectColl< shared_ptr<const AnaHWWCandidate> > gghww_sm_jan20_phot;
  EventObjectColl< shared_ptr<const AnaHWWCandidate> > gghww_sm_jan20_emedium;
  EventObjectColl< shared_ptr<const AnaHWWCandidate> > gghww_sm_jan20_erevIso;
  EventObjectColl< shared_ptr<const AnaHWWCandidate> > gghww_sm_jan20_murevIso;
  DgCutSet< shared_ptr<const AnaHWWCandidate> > gghww_sm_jan20_base( "gghww_sm" );
  DgCutSet< shared_ptr<const AnaHWWCandidate> > gghww_sm_jan20_selector( "gghww_sm_jan20" , gghww_sm_jan20.attach() );
  DgCutSet< shared_ptr<const AnaHWWCandidate> > gghww_sm_jan20_phot_selector( "gghww_sm_jan20_phot" , gghww_sm_jan20_phot.attach() );
  DgCutSet< shared_ptr<const AnaHWWCandidate> > gghww_sm_jan20_emedium_selector( "gghww_sm_jan20_emedium" , gghww_sm_jan20_emedium.attach() );
  DgCutSet< shared_ptr<const AnaHWWCandidate> > gghww_sm_jan20_erevIso_selector( "gghww_sm_jan20_erevIso" , gghww_sm_jan20_erevIso.attach() );
  DgCutSet< shared_ptr<const AnaHWWCandidate> > gghww_sm_jan20_murevIso_selector( "gghww_sm_jan20_murevIso" , gghww_sm_jan20_murevIso.attach() );
  gghww_sm_jan20_base.add_uu( "exactly_one_lepton" ,
                              bind(&AnaHWWCandidate::exactly_one_event_lepton,_1) == true ,
                              bind(&dg::fillh,_1,10,0,10,bind(&AnaHWWCandidate::event_nleptons,_2)) );
  gghww_sm_jan20_base.add_uu( "lepton_pt_gt30" ,
                              bind(&AnaHWWCandidate::lepton_pt,_1) > 30. ,
                              bind(&dg::fillh,_1,100,0,100,bind(&AnaHWWCandidate::lepton_pt,_2)) );
  gghww_sm_jan20_base.add_uu( "exactly_one_met" , 
                              bind(&AnaHWWCandidate::exactly_one_met,_1) == true ,
                              bind(&dg::filleff,_1,bind(&AnaHWWCandidate::exactly_one_met,_2) == true) );
  gghww_sm_jan20_base.add_uu( "met_gt_30" ,
                              bind(&AnaParticle::pt,bind(&AnaHWWCandidate::met,_1)) > 30. ,
                              bind(dg::fillh,_1,150,0,150,bind(&AnaParticle::pt,bind(&AnaHWWCandidate::met,_2))) );
  gghww_sm_jan20_base.add_uu( "m_wjj_in_71_91_GeV", 
                              bind(&AnaHWWCandidate::wjj_m,_1) > 71. && bind(&AnaHWWCandidate::wjj_m,_1) < 91.,
                              bind(&dg::fillh , _1 , 200 , 0 , 200. , bind(&AnaHWWCandidate::wjj_m,_2)));
  gghww_sm_jan20_base.add_u( "wjj_j0_abseta_lt2.8", bind(&AnaHWWCandidate::wjj_j0central,_1) == true );
  gghww_sm_jan20_base.add_u( "wjj_j1_abseta_lt2.8", bind(&AnaHWWCandidate::wjj_j1central,_1) == true );
  gghww_sm_jan20_base.add_u( "no_btag_jet", bind(&AnaHWWCandidate::no_btag_event_jet,_1) == true );
  gghww_sm_jan20_selector.based_on(gghww_sm_jan20_base);
  gghww_sm_jan20_phot_selector.based_on(gghww_sm_jan20_base);
  gghww_sm_jan20_emedium_selector.based_on(gghww_sm_jan20_base);
  gghww_sm_jan20_erevIso_selector.based_on(gghww_sm_jan20_base);
  gghww_sm_jan20_murevIso_selector.based_on(gghww_sm_jan20_base);
  // SM WW selectors
  DgCutSet< shared_ptr<const AnaHWWCandidate> > ggh_sm_e2j_jan20_sel( "ggh_sm_e2j_jan20", ggh_e2j_smww.attach() );
  DgCutSet< shared_ptr<const AnaHWWCandidate> > ggh_sm_mu2j_jan20_sel( "ggh_sm_mu2j_jan20", ggh_mu2j_smww.attach() );
  DgCutSet< shared_ptr<const AnaHWWCandidate> > ggh_sm_medium_e2j_jan20_sel( "ggh_sm_medium_e2j_jan20", ggh_e2j_smww_medium.attach() );
  DgCutSet< shared_ptr<const AnaHWWCandidate> > ggh_sm_revIso_e2j_jan20_sel( "ggh_sm_revIso_e2j_jan20", ggh_e2j_smww_revIso.attach() );
  DgCutSet< shared_ptr<const AnaHWWCandidate> > ggh_sm_revIso_mu2j_jan20_sel( "ggh_sm_revIso_mu2j_jan20", ggh_mu2j_smww_revIso.attach() );
  DgCutSet< shared_ptr<const AnaHWWCandidate> > ggh_sm_phot2j_jan20_sel( "ggh_sm_phot2j_jan20", ggh_phot2j_smww.attach() );
  DgCutSet< shared_ptr<const AnaHWWCandidate> > ggh_sm_e3j_jan20_sel( "ggh_sm_e3j_jan20", ggh_e3j_smww.attach() );
  DgCutSet< shared_ptr<const AnaHWWCandidate> > ggh_sm_mu3j_jan20_sel( "ggh_sm_mu3j_jan20", ggh_mu3j_smww.attach() );
  DgCutSet< shared_ptr<const AnaHWWCandidate> > ggh_sm_medium_e3j_jan20_sel( "ggh_sm_medium_e3j_jan20", ggh_e3j_smww_medium.attach() );
  DgCutSet< shared_ptr<const AnaHWWCandidate> > ggh_sm_revIso_e3j_jan20_sel( "ggh_sm_revIso_e3j_jan20", ggh_e3j_smww_revIso.attach() );
  DgCutSet< shared_ptr<const AnaHWWCandidate> > ggh_sm_revIso_mu3j_jan20_sel( "ggh_sm_revIso_mu3j_jan20", ggh_mu3j_smww_revIso.attach() );
  DgCutSet< shared_ptr<const AnaHWWCandidate> > ggh_sm_phot3j_jan20_sel( "ggh_sm_phot3j_jan20", ggh_phot3j_smww.attach() );
  ggh_sm_e2j_jan20_sel.based_on( gghww_sm_jan20_base );
  ggh_sm_mu2j_jan20_sel.based_on( gghww_sm_jan20_base );
  ggh_sm_medium_e2j_jan20_sel.based_on( gghww_sm_jan20_base );
  ggh_sm_revIso_e2j_jan20_sel.based_on( gghww_sm_jan20_base );
  ggh_sm_revIso_mu2j_jan20_sel.based_on( gghww_sm_jan20_base );
  ggh_sm_phot2j_jan20_sel.based_on( gghww_sm_jan20_base );
  ggh_sm_e3j_jan20_sel.based_on( gghww_sm_jan20_base );
  ggh_sm_mu3j_jan20_sel.based_on( gghww_sm_jan20_base );
  ggh_sm_medium_e3j_jan20_sel.based_on( gghww_sm_jan20_base );
  ggh_sm_revIso_e3j_jan20_sel.based_on( gghww_sm_jan20_base );
  ggh_sm_revIso_mu3j_jan20_sel.based_on( gghww_sm_jan20_base );
  ggh_sm_phot3j_jan20_sel.based_on( gghww_sm_jan20_base );

  // VBF H selection
  // ----------------------------------------------------------------
  EventObjectColl< shared_ptr<const AnaHWWCandidate> > vbfhww_sm_jan20;
  EventObjectColl< shared_ptr<const AnaHWWCandidate> > vbfhww_sm_jan20_phot;
  EventObjectColl< shared_ptr<const AnaHWWCandidate> > vbfhww_sm_jan20_emedium;
  EventObjectColl< shared_ptr<const AnaHWWCandidate> > vbfhww_sm_jan20_erevIso;
  EventObjectColl< shared_ptr<const AnaHWWCandidate> > vbfhww_sm_jan20_murevIso;
  DgCutSet< shared_ptr<const AnaHWWCandidate> > vbfhww_sm_jan20_base( "vbfhww_sm" );
  DgCutSet< shared_ptr<const AnaHWWCandidate> > vbfhww_sm_jan20_selector( "vbfhww_sm_jan20" , vbfhww_sm_jan20.attach() );
  DgCutSet< shared_ptr<const AnaHWWCandidate> > vbfhww_sm_jan20_phot_selector( "vbfhww_sm_jan20_phot" , vbfhww_sm_jan20_phot.attach() );
  DgCutSet< shared_ptr<const AnaHWWCandidate> > vbfhww_sm_jan20_emedium_selector( "vbfhww_sm_jan20_emedium" , vbfhww_sm_jan20_emedium.attach() );
  DgCutSet< shared_ptr<const AnaHWWCandidate> > vbfhww_sm_jan20_erevIso_selector( "vbfhww_sm_jan20_erevIso" , vbfhww_sm_jan20_erevIso.attach() );
  DgCutSet< shared_ptr<const AnaHWWCandidate> > vbfhww_sm_jan20_murevIso_selector( "vbfhww_sm_jan20_murevIso" , vbfhww_sm_jan20_murevIso.attach() );
  vbfhww_sm_jan20_base.add_uu( "njets_ge4" ,
                               bind(&AnaHWWCandidate::event_njets,_1) >= 4 ,
                               bind(&dg::fillh,_1,10,0,10,bind(&AnaHWWCandidate::event_njets,_2)) );
  vbfhww_sm_jan20_base.add_uu( "exactly_one_lepton" ,
                               bind(&AnaHWWCandidate::exactly_one_event_lepton,_1) == true ,
                               bind(&dg::fillh,_1,10,0,10,bind(&AnaHWWCandidate::event_nleptons,_2)) );
  vbfhww_sm_jan20_base.add_uu( "lepton_pt_gt25" ,
                               bind(&AnaHWWCandidate::lepton_pt,_1) > 25. ,
                               bind(&dg::fillh,_1,100,0,100,bind(&AnaHWWCandidate::lepton_pt,_2)) );
  vbfhww_sm_jan20_base.add_uu( "exactly_one_met" , 
                               bind(&AnaHWWCandidate::exactly_one_met,_1) == true ,
                               bind(&dg::filleff,_1,bind(&AnaHWWCandidate::exactly_one_met,_2) == true) );
  vbfhww_sm_jan20_base.add_uu( "met_gt_30" ,
                               bind(&AnaParticle::pt,bind(&AnaHWWCandidate::met,_1)) > 30. ,
                               bind(dg::fillh,_1,150,0,150,bind(&AnaParticle::pt,bind(&AnaHWWCandidate::met,_2))) );
  vbfhww_sm_jan20_base.add_uu( "m_wjj_in_71_91_GeV", 
                               bind( AnaW::MassInWindow<const AnaW>(71.,91.) , bind(&AnaHWWCandidate::hadronic_w,_1) ) == true ,
                               bind( &dg::fillh , _1 , 400 , 0 , 400. , bind(&AnaHWWCandidate::mass,bind(&AnaHWWCandidate::hadronic_w,_2)) ) );
  vbfhww_sm_jan20_base.add_u( "tagjj_opposite_hemis", bind(&AnaHWWCandidate::tagjj_opposite_hemis,_1) == true ); 
  vbfhww_sm_jan20_base.add_uu( "tagjj_deta_gt3", 
                               bind( &AnaHWWCandidate::tagjj_deta,_1) > 3. ,
                               bind( &dg::fillh , _1 , 40 , 0 , 4 , bind(&AnaHWWCandidate::tagjj_deta,_2)) );
  vbfhww_sm_jan20_base.add_u( "lepton_eta_between_tagjj", bind(&AnaHWWCandidate::lepton_eta_between_tagjj,_1) == true ); 
  vbfhww_sm_jan20_base.add_uu( "tagjj_mass_gt600", 
                               bind(&AnaHWWCandidate::tagjj_mass,_1) > 600. ,
                               bind( &dg::fillh , _1 , 100 , 0 , 1000. , bind(&AnaHWWCandidate::tagjj_mass,_2)) );
  vbfhww_sm_jan20_base.add_u( "no_add_jets_eta_lt32", bind(&AnaHWWCandidate::no_add_jets_eta_lt32,_1) == true ); 
  vbfhww_sm_jan20_base.add_u( "no_btag_jet", bind(&AnaHWWCandidate::no_btag_event_jet,_1) == true );
  vbfhww_sm_jan20_selector.based_on( vbfhww_sm_jan20_base );
  vbfhww_sm_jan20_phot_selector.based_on( vbfhww_sm_jan20_base );
  vbfhww_sm_jan20_emedium_selector.based_on( vbfhww_sm_jan20_base );
  vbfhww_sm_jan20_erevIso_selector.based_on( vbfhww_sm_jan20_base );
  vbfhww_sm_jan20_murevIso_selector.based_on( vbfhww_sm_jan20_base );
  DgCutSet< shared_ptr<const AnaHWWCandidate> > vbfh_sm_muge4j_jan20_sel( "vbfh_sm_muge4j_jan20", vbfh_muge4j_smww.attach() );
  DgCutSet< shared_ptr<const AnaHWWCandidate> > vbfh_sm_ege4j_jan20_sel( "vbfh_sm_ege4j_jan20", vbfh_ege4j_smww.attach() );
  DgCutSet< shared_ptr<const AnaHWWCandidate> > vbfh_sm_medium_ege4j_jan20_sel( "vbfh_sm_medium_ege4j_jan20", vbfh_ege4j_smww_medium.attach() );
  DgCutSet< shared_ptr<const AnaHWWCandidate> > vbfh_sm_revIso_ege4j_jan20_sel( "vbfh_sm_revIso_ege4j_jan20", vbfh_ege4j_smww_revIso.attach() );
  DgCutSet< shared_ptr<const AnaHWWCandidate> > vbfh_sm_revIso_muge4j_jan20_sel( "vbfh_sm_revIso_muge4j_jan20", vbfh_muge4j_smww_revIso.attach() );
  DgCutSet< shared_ptr<const AnaHWWCandidate> > vbfh_sm_photge4j_jan20_sel( "vbfh_sm_photge4j_jan20", vbfh_photge4j_smww.attach() );
  vbfh_sm_muge4j_jan20_sel.based_on( vbfhww_sm_jan20_base );
  vbfh_sm_ege4j_jan20_sel.based_on( vbfhww_sm_jan20_base );
  vbfh_sm_medium_ege4j_jan20_sel.based_on( vbfhww_sm_jan20_base );
  vbfh_sm_revIso_ege4j_jan20_sel.based_on( vbfhww_sm_jan20_base );
  vbfh_sm_revIso_muge4j_jan20_sel.based_on( vbfhww_sm_jan20_base );
  vbfh_sm_photge4j_jan20_sel.based_on( vbfhww_sm_jan20_base );

  //
  // Z boson selection
  // ----------------------------------------------------------------
  //   input lists
  EventObjectColl< shared_ptr<const AnaZ> > ee_smww;
  EventObjectColl< shared_ptr<const AnaZ> > mumu_smww;

  // Initialize study classes
  // study W cands
  StudyWCrossSection st_w( "w_cands" , "W event candidates after exactly 1 lepton selection" );
  st_w.add_category( "w_met30" , bind(&AnaMET::pt,bind(&AnaWCrossSectionCandidate::met,_1)) > 30. );
  st_w.add_category( "w_0j" , bind( &AnaWCrossSectionCandidate::njets , _1 ) == 0 );
  st_w.add_category( "w_1j" , bind( &AnaWCrossSectionCandidate::njets , _1 ) == 1 );
  st_w.add_category( "w_2j" , bind( &AnaWCrossSectionCandidate::njets , _1 ) == 2 );
  st_w.add_category( "w_3j" , bind( &AnaWCrossSectionCandidate::njets , _1 ) == 3 );
  st_w.add_category( "w_ge4j" , bind( &AnaWCrossSectionCandidate::njets , _1 ) >= 4 );
  st_w.add_category( "w_met30_0j" , 
                     bind( &AnaMET::pt,bind(&AnaWCrossSectionCandidate::met,_1)) > 30. &&
                     bind( &AnaWCrossSectionCandidate::njets , _1 ) == 0 );
  st_w.add_category( "w_met30_1j" , 
                     bind( &AnaMET::pt,bind(&AnaWCrossSectionCandidate::met,_1)) > 30. &&
                     bind( &AnaWCrossSectionCandidate::njets , _1 ) == 1 );
  st_w.add_category( "w_met30_2j" , 
                     bind( &AnaMET::pt,bind(&AnaWCrossSectionCandidate::met,_1)) > 30. &&
                     bind( &AnaWCrossSectionCandidate::njets , _1 ) == 2 );
  st_w.add_category( "w_met30_3j" , 
                     bind( &AnaMET::pt,bind(&AnaWCrossSectionCandidate::met,_1)) > 30. &&
                     bind( &AnaWCrossSectionCandidate::njets , _1 ) == 3 );
  st_w.add_category( "w_met30_ge4j" , 
                     bind( &AnaMET::pt,bind(&AnaWCrossSectionCandidate::met,_1)) > 30. &&
                     bind( &AnaWCrossSectionCandidate::njets , _1 ) >= 4 );
  StudyZCrossSection st_z( "z_cands" , "Z event candidates after exactly 1 lepton selection" );
  st_z.add_category( "z_met30" , bind(&AnaMET::pt,bind(&AnaZCrossSectionCandidate::met,_1)) > 30. );
  st_z.add_category( "z_0j" , bind( &AnaZCrossSectionCandidate::njets , _1 ) == 0 );
  st_z.add_category( "z_1j" , bind( &AnaZCrossSectionCandidate::njets , _1 ) == 1 );
  st_z.add_category( "z_2j" , bind( &AnaZCrossSectionCandidate::njets , _1 ) == 2 );
  st_z.add_category( "z_3j" , bind( &AnaZCrossSectionCandidate::njets , _1 ) == 3 );
  st_z.add_category( "z_ge4j" , bind( &AnaZCrossSectionCandidate::njets , _1 ) >= 4 );
  st_z.add_category( "z_met30_0j" , 
                     bind( &AnaMET::pt,bind(&AnaZCrossSectionCandidate::met,_1)) > 30. &&
                     bind( &AnaZCrossSectionCandidate::njets , _1 ) == 0 );
  st_z.add_category( "z_met30_1j" , 
                     bind( &AnaMET::pt,bind(&AnaZCrossSectionCandidate::met,_1)) > 30. &&
                     bind( &AnaZCrossSectionCandidate::njets , _1 ) == 1 );
  st_z.add_category( "z_met30_2j" , 
                     bind( &AnaMET::pt,bind(&AnaZCrossSectionCandidate::met,_1)) > 30. &&
                     bind( &AnaZCrossSectionCandidate::njets , _1 ) == 2 );
  st_z.add_category( "z_met30_3j" , 
                     bind( &AnaMET::pt,bind(&AnaZCrossSectionCandidate::met,_1)) > 30. &&
                     bind( &AnaZCrossSectionCandidate::njets , _1 ) == 3 );
  st_z.add_category( "z_met30_ge4j" , 
                     bind( &AnaMET::pt,bind(&AnaZCrossSectionCandidate::met,_1)) > 30. &&
                     bind( &AnaZCrossSectionCandidate::njets , _1 ) >= 4 );

  // code to retrieve the hadronic W from each AnaHWWCandidate
  boost::function< shared_ptr<const AnaW> ( shared_ptr<const AnaHWWCandidate> const& ) > get_wjj( bind(&AnaHWWCandidate::hadronic_w,_1) );
  // code to choose the hadronic W candidate closest to the W mass
  AnaParticle::ClosestMassToWof<const AnaHWWCandidate,const AnaW> masswof( get_wjj );
  // study higgs candidates before selections (only njets>=2)
  StudyGGHiggs st_hall( "hall_cands" , "H event candidates before full selection" );
  st_hall.add_selector( "mjj_closest_mw" , bind( masswof , _1 , _2 ) );
  st_hall.add_category( "h_mu", 
                        bind( &AnaHWWCandidate::lepton_is_muon , _1 ) == true);
  st_hall.add_category( "h_e", 
                        bind( &AnaHWWCandidate::lepton_is_electron , _1 ) == true);
  st_hall.add_category( "h_2j" , bind( &AnaHWWCandidate::event_njets , _1 ) == 2 );
  st_hall.add_category( "h_3j" , bind( &AnaHWWCandidate::event_njets , _1 ) == 3 );
  st_hall.add_category( "h_ge4j" , bind( &AnaHWWCandidate::event_njets , _1 ) >= 4 );
  st_hall.add_category( "h_met30_2j" , 
                        bind( &AnaHWWCandidate::event_njets , _1 ) == 2 &&
                        bind( &AnaParticle::pt,bind(&AnaHWWCandidate::met,_1) ) > 30.);
  st_hall.add_category( "h_met30_3j" , 
                        bind( &AnaHWWCandidate::event_njets , _1 ) == 3 &&
                        bind( &AnaParticle::pt,bind(&AnaHWWCandidate::met,_1) ) > 30.);
  st_hall.add_category( "h_met30_ge4j" , 
                        bind( &AnaHWWCandidate::event_njets , _1 ) >= 4 &&
                        bind( &AnaParticle::pt,bind(&AnaHWWCandidate::met,_1) ) > 30.);
  st_hall.add_category( "h_mu2j", 
                        bind( &AnaHWWCandidate::event_njets , _1 ) == 2. &&
                        bind( &AnaHWWCandidate::lepton_is_muon , _1 ) == true);
  st_hall.add_category( "h_mu3j", 
                        bind( &AnaHWWCandidate::event_njets , _1 ) == 3. &&
                        bind( &AnaHWWCandidate::lepton_is_muon , _1 ) == true);
  st_hall.add_category( "h_e2j", 
                        bind( &AnaHWWCandidate::event_njets , _1 ) == 2. &&
                        bind( &AnaHWWCandidate::lepton_is_electron , _1 ) == true);
  st_hall.add_category( "h_e3j", 
                        bind( &AnaHWWCandidate::event_njets , _1 ) == 3. &&
                        bind( &AnaHWWCandidate::lepton_is_electron , _1 ) == true);
  st_hall.add_category( "h_muge4j", 
                        bind( &AnaHWWCandidate::event_njets , _1 ) >= 4. &&
                        bind( &AnaHWWCandidate::lepton_is_muon , _1 ) == true);
  st_hall.add_category( "h_ege4j", 
                        bind( &AnaHWWCandidate::event_njets , _1 ) >= 4. &&
                        bind( &AnaHWWCandidate::lepton_is_electron , _1 ) == true);
  st_hall.add_category( "h_met30_mu2j", 
                        bind( &AnaHWWCandidate::event_njets , _1 ) == 2. &&
                        bind( &AnaHWWCandidate::lepton_is_muon , _1 ) == true &&
                        bind( &AnaParticle::pt,bind(&AnaHWWCandidate::met,_1) ) > 30.);
  st_hall.add_category( "h_met30_mu3j", 
                        bind( &AnaHWWCandidate::event_njets , _1 ) == 3. &&
                        bind( &AnaHWWCandidate::lepton_is_muon , _1 ) == true &&
                        bind( &AnaParticle::pt,bind(&AnaHWWCandidate::met,_1) ) > 30.);
  st_hall.add_category( "h_met30_e2j", 
                        bind( &AnaHWWCandidate::event_njets , _1 ) == 2. &&
                        bind( &AnaHWWCandidate::lepton_is_electron , _1 ) == true &&
                        bind( &AnaParticle::pt,bind(&AnaHWWCandidate::met,_1) ) > 30.);
  st_hall.add_category( "h_met30_e3j", 
                        bind( &AnaHWWCandidate::event_njets , _1 ) == 3. &&
                        bind( &AnaHWWCandidate::lepton_is_electron , _1 ) == true &&
                        bind( &AnaParticle::pt,bind(&AnaHWWCandidate::met,_1) ) > 30.);
  st_hall.add_category( "h_met30_muge4j", 
                        bind( &AnaHWWCandidate::event_njets , _1 ) >= 4. &&
                        bind( &AnaHWWCandidate::lepton_is_muon , _1 ) == true &&
                        bind( &AnaParticle::pt,bind(&AnaHWWCandidate::met,_1) ) > 30.);
  st_hall.add_category( "h_met30_ege4j", 
                        bind( &AnaHWWCandidate::event_njets , _1 ) >= 4. &&
                        bind( &AnaHWWCandidate::lepton_is_electron , _1 ) == true &&
                        bind( &AnaParticle::pt,bind(&AnaHWWCandidate::met,_1) ) > 30.);
  // study event before even njets, but after requiring one lepton (WCrossSection study)
  EventObjectColl< shared_ptr<const AnaWCrossSectionCandidate>  > w_enu_baseline_loose;
  EventObjectColl< shared_ptr<const AnaWCrossSectionCandidate>  > w_munu_baseline_loose;
  EventObjectColl< shared_ptr<const AnaWCrossSectionCandidate>  > w_all_baseline_loose;
  // study SM WW W cands
  EventObjectColl< shared_ptr<const AnaWCrossSectionCandidate>  > w_enu_sm_baseline_loose;
  EventObjectColl< shared_ptr<const AnaWCrossSectionCandidate>  > w_enu_sm_medium_baseline_loose;
  EventObjectColl< shared_ptr<const AnaWCrossSectionCandidate>  > w_enu_sm_revIso_baseline_loose;
  EventObjectColl< shared_ptr<const AnaWCrossSectionCandidate>  > w_munu_sm_baseline_loose;
  EventObjectColl< shared_ptr<const AnaWCrossSectionCandidate>  > w_munu_sm_revIso_baseline_loose;
  EventObjectColl< shared_ptr<const AnaWCrossSectionCandidate>  > w_photnu_sm_baseline_loose;
  EventObjectColl< shared_ptr<const AnaWCrossSectionCandidate>  > w_all_sm_baseline_loose;
  // Z events
  EventObjectColl<  boost::shared_ptr<const AnaZCrossSectionCandidate>  > zall_muon;
  EventObjectColl<  boost::shared_ptr<const AnaZCrossSectionCandidate>  > zall_electron;

  //
  // event loop
  // ================================================================
  //
  //
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

    // MC pileup reweighting
    {
      dg::down( "selectors" , "selector details" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      primary_vertex_selector.for_each( evt->vertices().begin() , evt->vertices().end() );
    }
    int nvtx_sec = primary_vertices.size();
    if(is_mc && do_pileup) {
      const double w_pu = pw->weight(nvtx_sec);
      dg::set_global_weight(w_pu);
    }

    // select electrons
    // ================================================================
    //    if( is_good_run_ele ) { 
    if( true ) { 
      dg::down( "selectors" , "selector details" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      ele_smww_selector.for_each( evt->electrons().begin() , evt->electrons().end() );
      ele_smww_medium_msn_selector.for_each( evt->electrons().begin() , evt->electrons().end() );
      // For the QCD background, if none of the electrons passed, then go to the photon selection:
      if( ele_smww.empty() && !is_mc ) {
        photon_smww_selector.for_each( evt->photons().begin() , evt->photons().end() );
        ele_smww_medium_selector.for_each( evt->electrons().begin() , evt->electrons().end() );
        ele_smww_revIso_selector.for_each( evt->electrons().begin() , evt->electrons().end() );
      }
    }

    // select muons
    // ================================================================
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
    //    if( is_good_run_mu ) { 
    if( true ) { 
      dg::down( "selectors" , "selector details" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      mu_smww_selector.for_each( mu_event.begin() , mu_event.end() );
      if ( mu_smww.empty() && !is_mc ) {
        mu_smww_revIso_selector.for_each( mu_event.begin() , mu_event.end() );
      }
    }
    /*
      BOOST_FOREACH( const shared_ptr<const AnaMuon>& mu , evt->muons() ) {
      std::cout << "Muon ptcone20:" << mu->pt_cone(AnaMuon::CONE20) << std::endl;
      if( mu->pt_cone(AnaMuon::CONE20) < 1.8 )
      std::cout << "Smaller than 1.8!" << std::endl;
      else
      std::cout << "Greater than 1.8!" << std::endl;
      }
    */
    // select jets and missing ET
    // ================================================================
    // recompute missing ET for H->WWdil
    if( mu_smww.size()>0 ) {
      BOOST_FOREACH( const shared_ptr<const AnaMET>& met , evt->missing_et() ) {
        met_hww_event.push_back( AnaMET::recompute_for_hww( met , mu_smww.begin() , mu_smww.end() ) );
      }
      /*
        if( false && "debug output for missing ET correction" ) {
        boost::shared_ptr<archive::xml_oarchive> aro( new archive::xml_oarchive(cout) );
        // print all uncorrected met
        cout << "Uncorrected METs for this event: " << endl;
        BOOST_FOREACH( const shared_ptr<const AnaMET>& met , evt->missing_et() ) {
        if( met->met_type() != AnaMET::LOCHADTOPOCEF ) { continue; }
        (*aro) << BOOST_SERIALIZATION_NVP(met);
        } 
        cout << "Corrected METs for this event: " << endl;
        BOOST_FOREACH( const shared_ptr<const AnaMET>& met , met_hww_event ) {
        if( met->met_type() != AnaMET::LOCHADTOPOCEF ) { continue; }
        (*aro) << BOOST_SERIALIZATION_NVP(met);
        }
        cout.flush();
        aro.reset();
        }
      */
    } else{
      met_hww_event.push_back_all(evt->missing_et().begin(),evt->missing_et().end());
    }
    // apply selectors
    bool nobadjets=true;
    if( true ) { 
      dg::down( "selectors" , "selector details" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      jet_smww_selector.for_each( evt->atlas_anti_kt_4_h1_topo_jets().begin() , 
                                  evt->atlas_anti_kt_4_h1_topo_jets().end() );
      
      BOOST_FOREACH( const shared_ptr<const AnaJet>& jet , jet_smww ) {
        if( jet->is_bad_tight_jetetmiss_v15() )
          nobadjets=false;
      }
      // must fill MET after the jets
      //met_hww_selector.for_each( evt->missing_et().begin(), evt->missing_et().end() );
      // changed MET here, because of error with:
      // Warning in <TVector3::PseudoRapidity>: transvers momentum = 0! return +/- 10e10
      met_hww_selector.for_each( met_hww_event.begin() , met_hww_event.end() );
    }
    // make good run requirement
    dg::filleff( "c0_is_good_run", is_good_run_ele || is_good_run_mu );
    dg::filleff( "c0_is_good_run_ele" , is_good_run_ele );
    dg::filleff( "c0_is_good_run_mu" , is_good_run_mu );
    if( !(is_good_run_ele || is_good_run_mu) ) { continue; }

    dg::filleff("c1_event_clean", ( !met_hww.empty() && nobadjets ));
    if( met_hww.empty() || !nobadjets ) { continue; }

    // select primary vertices
    // selector has been filled
    dg::filleff("c2_has_good_primary_vertex", !primary_vertices.empty() );
    if( primary_vertices.empty() ) { continue; }

    // retrieve trigger
    const boost::shared_ptr<const AnaTrigger>& trigger = evt->trigger();
    assert(trigger);
    const bool is_triggered_ele = trigger->passes(which_trigger_el(evt->run_number(),is_mc));
    const bool is_triggered_phot = trigger->passes(which_trigger_phot(evt->run_number(),is_mc));
    const bool is_triggered_mu = trigger->passes(which_trigger_mu(evt->run_number(),is_mc));
    const bool is_triggered = is_triggered_ele || is_triggered_mu || is_triggered_phot;
    const bool is_good_triggered_ele = is_triggered_ele && is_good_run_ele;
    const bool is_good_triggered_mu = is_triggered_mu && is_good_run_mu;
    const bool is_good_triggered_phot = is_triggered_phot && is_good_run_ele;
    const bool is_good_triggered = is_good_triggered_ele || is_good_triggered_mu || is_good_triggered_phot;
    dg::filleff( "c3_trigger_fired_ele", is_triggered_ele );
    dg::filleff( "c3_trigger_fired_mu", is_triggered_mu );
    dg::filleff( "c3_trigger_fired_phot", is_triggered_phot );
    dg::filleff( "c3_good_triggered_ele", is_good_triggered_ele );
    dg::filleff( "c3_good_triggered_mu", is_good_triggered_mu );
    dg::filleff( "c3_good_triggered_phot", is_good_triggered_phot );
    dg::filleff( "c3_trigger_fired", is_triggered );
    dg::filleff( "c3_good_triggered", is_good_triggered );
    if( !is_good_triggered ) { continue; }

    // remove electrons within DR<0.1 cone of a muon
    /*
      if( !ele_smww_all.empty() and !mu_smww.empty() ) {
      overlap_removal( ele_smww_all.begin() , ele_smww_all.end() ,
      mu_smww.begin() , mu_smww.end() ,
      0.1 , back_inserter(ele_smww) );
      } else { 
      ele_smww.push_back_all(ele_smww_all.begin() , ele_smww_all.end() );
      }
    */
    if(!ele_smww_all.empty())
      ele_smww.push_back_all(ele_smww_all.begin() , ele_smww_all.end() );
    // remove jets inside a DR<0.3 cone of an electron. remove a
    // maximum of one jet per electron; if more than one electron with
    // DR<0.3, remove the one with the smallest DR.
    if (is_mc) {
      if( !ele_smww.empty() ) { 
        overlap_removal( jet_smww_all.begin() , jet_smww_all.end() ,
                         ele_smww.begin() , ele_smww.end() ,
                         0.3 , back_inserter(jet_smww) );
      }  else {
        jet_smww.push_back_all(jet_smww_all.begin() , jet_smww_all.end() );
      }
    }
    else {
      if( !ele_smww.empty() ) { 
        overlap_removal( jet_smww_all.begin() , jet_smww_all.end() ,
                         ele_smww.begin() , ele_smww.end() ,
                         0.3 , back_inserter(jet_smww) );
      } else if ( !photon_smww.empty() ) {
        // Photon list only has entries if electron list does not:
        overlap_removal( jet_smww_all.begin() , jet_smww_all.end() ,
                         photon_smww.begin() , photon_smww.end() ,
                         0.3 , back_inserter(jet_smww) );
      } else {
        jet_smww.push_back_all(jet_smww_all.begin() , jet_smww_all.end() );
      }
    }

    // merge lepton collections
    // ================================================================
    lepton_smww.push_back_all( ele_smww.begin() , ele_smww.end() );
    lepton_smww.push_back_all( mu_smww.begin() , mu_smww.end() );
    
    dg::fillh( "n_electrons_smww" , 10 , 0 , 10 , ele_smww.size() , "Number of selected SM WW electrons" );
    dg::fillh( "n_photons_smww" , 10 , 0 , 10 , photon_smww.size() , "Number of selected SM WW photons" );
    dg::fillh( "n_muons_smww" , 10 , 0 , 10 , mu_smww.size() , "Number of selected SM WW muons" );
    dg::fillh( "n_jets_smww_all" , 20 , 0 , 20 , jet_smww_all.size() , "Number of selected SM WW all jets" );
    dg::fillh( "n_jets_smww" , 20 , 0 , 20 , jet_smww.size() , "Number of selected SM WW jets" );
    dg::fillh( "n_met_hww" , 10 , 0 , 10, met_hww.size() , "Number of selected H->WW met (should be exactly one)" );

    // build composites: W's, Z's, H's
    // ================================================================

    // build lists of Z's
    if ( ele_smww.size() == 2 || mu_smww.size() == 2 ) {
      CompositeGenerator<AnaParticle,AnaZ> zgen;
      zgen.generate( ele_smww.begin() , ele_smww.end() , ele_smww.begin() , ele_smww.end() , AnaZ::Compose() , ee_smww );
      zgen.generate( mu_smww.begin() , mu_smww.end() , mu_smww.begin() , mu_smww.end() , AnaZ::Compose() , mumu_smww );
      build_z_candidates( ele_smww.begin() , ele_smww.end() ,
                          met_hww.begin() , met_hww.end() ,
                          jet_smww.begin() , jet_smww.end() ,
                          primary_vertices.begin() , primary_vertices.end() ,
                          back_inserter(zall_electron) );
      build_z_candidates( mu_smww.begin()  , mu_smww.end()  ,
                          met_hww.begin() , met_hww.end() ,
                          jet_smww.begin() , jet_smww.end() ,
                          primary_vertices.begin() , primary_vertices.end() ,
                          back_inserter(zall_muon) );
      // study selected Z events
      {
        dg::down( "constantinos_zs" , "Constantinos's Z Candidates" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
        st_z.repurpose( "z_muon_candidates" , "Zmumu event candidates Baseline" );
        st_z.for_each( zall_muon.begin() , zall_muon.end() );
        st_z.repurpose( "z_electron_candidates" , "Zee event candidates Baseline" );
        st_z.for_each( zall_electron.begin() , zall_electron.end() );
      }
      //
      // study Z's
      {
        // study Z cands
        StudyComposite st_zc( "mumu_cand_top_tight" , "top tight+tight dimuon candidates" );
        st_zc.add_selector( "nearest_zpole" , AnaParticle::ClosestMassToZ<const AnaParticle>() );
        typedef boost::function< bool ( shared_ptr<const AnaZ> const & ) > AnaZBind;
        AnaZBind bNotLow( bind(&AnaZ::mass,_1) > 76. );
        AnaZBind bNotHigh( bind(&AnaZ::mass,_1) < 106. );
        st_zc.add_category( "zwindow" , boost::bind( std::logical_and<AnaZBind>() , bNotLow , bNotHigh ) );
        st_zc.repurpose( "ee_cand_smww" , "SM WW dielectron candidates" );
        st_zc.for_each( ee_smww.begin() , ee_smww.end() );
        st_zc.repurpose( "mumu_cand_smww" , "SM WW dimuon candidates" );
        st_zc.for_each( mumu_smww.begin() , mumu_smww.end() );
      }
      dg::fillh( "n_ee_smww" , 10 , 0 , 10 , ee_smww.size() ); 
      dg::filleff( "eff_ee_smww" , !ee_smww.empty() ); 
      dg::fillh( "n_mumu_smww" , 10 , 0 , 10 , mumu_smww.size() ); 
      dg::filleff( "eff_mumu_smww" , !mumu_smww.empty() ); 
    }

    const bool exactly_one_mu = is_good_triggered_mu && (mu_smww.size()==1);
    const bool exactly_one_ele = is_good_triggered_ele && (ele_smww.size()==1);
    const bool exactly_two_lep = is_good_triggered && exactly_one_mu && exactly_one_ele;
    const bool exactly_one_lep = (exactly_one_mu || exactly_one_ele) && !exactly_two_lep;
    dg::filleff( "c4_exactly_one_mu", exactly_one_mu);
    dg::filleff( "c4_exactly_one_ele", exactly_one_ele);
    dg::filleff( "c4_exactly_two_lep", exactly_two_lep);
    dg::filleff( "c4_exactly_one_lep", exactly_one_lep);
    //if ( !exactly_one_lep ) continue;

    const bool more_medium_ele_one_mu = ele_smww_medium_msn.size() > 0 ;
    const bool more_medium_ele_one_ele = ele_smww_medium_msn.size() > 1 ;
    dg::filleff( "c5_no_more_medium_ele_one_mu", !more_medium_ele_one_mu && exactly_one_mu );
    dg::filleff( "c5_no_more_medium_ele_one_ele", !more_medium_ele_one_ele && exactly_one_ele );
    dg::filleff( "c5_no_more_medium_ele_one_lep", ( !more_medium_ele_one_mu && exactly_one_mu ) || ( !more_medium_ele_one_ele && exactly_one_ele ) );
    //if ( !( ( !more_medium_ele_one_mu && exactly_one_mu ) || ( !more_medium_ele_one_ele && exactly_one_ele ) ) ) continue;

    /* WRITE NTUPLE (if specified by flag) */
    // save event if it has a W candidate
    if( AnaEventMgr::instance()->ana_event_ntuple_mode() == AnaEventNtuple::WRITE ) {
      AnaEventMgr::instance()->add_to_ana_event_ntuple( jet_smww.size() >= 2 );
      //continue;
    }

    // build lists of W's
    CompositeGenerator<AnaParticle,AnaW> wgen;
    // SM WW pairings
    wgen.generate( ele_smww.begin() , ele_smww.end() , met_hww.begin() , met_hww.end() , AnaW::Compose() , enu_smww );
    wgen.generate( mu_smww.begin() , mu_smww.end() , met_hww.begin() , met_hww.end() , AnaW::Compose() , munu_smww );
    if (!is_mc) {
      wgen.generate( ele_smww_medium.begin() , ele_smww_medium.end() , met_hww.begin() , met_hww.end() , AnaW::Compose() , enu_smww_medium );
      wgen.generate( ele_smww_revIso.begin() , ele_smww_revIso.end() , met_hww.begin() , met_hww.end() , AnaW::Compose() , enu_smww_revIso );
      wgen.generate( mu_smww_revIso.begin() , mu_smww_revIso.end() , met_hww.begin() , met_hww.end() , AnaW::Compose() , munu_smww_revIso );
      wgen.generate( photon_smww.begin() , photon_smww.end() , met_hww.begin() , met_hww.end() , AnaW::Compose() , photnu_smww );
    }

    wgen.generate( jet_smww.begin() , jet_smww.end() , jet_smww.begin() , jet_smww.end() , AnaW::Compose() , wjets_smww );
    // generate w cands without cuts
    lnu_smww.push_back_all( enu_smww.begin() , enu_smww.end() );
    lnu_smww.push_back_all( munu_smww.begin() , munu_smww.end() );
    if( !wjets_smww.empty() ) { 
      std::cout << "evt:" << evt->event_number() << std::endl; // CM
      std::cout << "njets:" << jet_smww.size() << std::endl; // CM
      std::cout << "wjets:" << wjets_smww.size() << std::endl; // CM
      //      w_jj_closestmw_smww.push_back( *std::min_element( wjets_smww.begin() , wjets_smww.end() , 
      //						AnaParticle::ClosestMassToW<const AnaW>() ) );
      boost::shared_ptr<const AnaW> closeW;
      float wmass=80398./1000.;
      float closest_wMass_diff(std::numeric_limits<float>::max());
      BOOST_FOREACH( boost::shared_ptr<const AnaW> theW, wjets_smww ) {
        std::cout << "Wmass: " << theW->m() << std::endl; // CM
        float wMass_diff = std::abs(theW->m() - wmass); // CM
        if( wMass_diff < closest_wMass_diff ) {
          std::cout << "diff: " << wMass_diff << std::endl; // CM
          closest_wMass_diff = wMass_diff;
          closeW = theW;
        }
      }
      std::cout << "closest: " << closeW->m() << std::endl; // CM
      //std::cout << "chosen Wmass: " << closeW->m() << std::endl; // CM
      w_jj_closestmw_smww.push_back(closeW);
    }

    // study W's
    if( true ) {
      dg::fillh( "n_enu_smww" , 10 , 0 , 10 , enu_smww.size() );
      dg::fillh( "n_munu_smww" , 10 , 0 , 10 , munu_smww.size() );
      dg::fillh( "n_photnu_smww" , 10 , 0 , 10 , photnu_smww.size() );
      dg::fillh( "n_lnu_smww" , 10 , 0 , 10 , lnu_smww.size() );
      dg::fillh( "n_w_jj_smww" , 10 , 0 , 10 , wjets_smww.size() );
      dg::fillh( "n_w_jj_closestmw_smww" , 3 , 0 , 3 , w_jj_closestmw_smww.size() );
      // study W candidates
      // ----------------------------------------------------------------
      
      // ----------------------------------------------------------------
      // study Constantinos's W candidates
      // ----------------------------------------------------------------
      {
        dg::down( "constantinos_ws" , "Constantinos's W Candidates" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
        // study W event (one lepton, no met cut)
        if ( lepton_smww.size() == 1 ) {
          build_w_candidates( ele_smww.begin() , ele_smww.end() ,
                              met_hww.begin() , met_hww.end() ,
                              jet_smww.begin() , jet_smww.end() ,
                              primary_vertices.begin() , primary_vertices.end() ,
                              back_inserter(w_enu_sm_baseline_loose) );
          build_w_candidates( mu_smww.begin() , mu_smww.end() ,
                              met_hww.begin() , met_hww.end() ,
                              jet_smww.begin() , jet_smww.end() ,
                              primary_vertices.begin() , primary_vertices.end() ,
                              back_inserter(w_munu_sm_baseline_loose) );
          w_all_sm_baseline_loose.push_back_all(w_enu_sm_baseline_loose.begin(),w_enu_sm_baseline_loose.end());
          w_all_sm_baseline_loose.push_back_all(w_munu_sm_baseline_loose.begin(),w_munu_sm_baseline_loose.end());
          w_enu_jan20_selector.for_each( w_enu_sm_baseline_loose.begin(),w_enu_sm_baseline_loose.end() );
          w_munu_jan20_selector.for_each( w_munu_sm_baseline_loose.begin(),w_munu_sm_baseline_loose.end() );
          dg::fillh( "n_enu_smww_loose" , 10 , 0 , 10 , w_enu_sm_baseline_loose.size() );
          dg::fillh( "n_munu_smww_loose" , 10 , 0 , 10 , w_munu_sm_baseline_loose.size() );
          dg::fillh( "n_lnu_smww_loose" , 10 , 0 , 10 , w_all_sm_baseline_loose.size() );
          st_w.repurpose( "wall_sm_cands" , "W event candidates after exactly 1 SMWW lepton selection" );
          st_w.for_each( w_all_sm_baseline_loose.begin(), w_all_sm_baseline_loose.end() );
          st_w.repurpose( "wmunu_sm_cands" , "W event candidates after exactly 1 SMWW muon selection" );
          st_w.for_each( w_munu_sm_baseline_loose.begin(), w_munu_sm_baseline_loose.end() );
          st_w.repurpose( "wenu_sm_cands" , "W event candidates after exactly 1 SMWW electron selection" );
          st_w.for_each( w_enu_sm_baseline_loose.begin(), w_enu_sm_baseline_loose.end() );
        }
        if ( lepton_smww.size() == 0 && !is_mc ) {
          if (photon_smww.size() == 1 ) {
            build_w_candidates( photon_smww.begin() , photon_smww.end() ,
                                met_hww.begin() , met_hww.end() ,
                                jet_smww.begin() , jet_smww.end() ,
                                primary_vertices.begin() , primary_vertices.end() ,
                                back_inserter(w_photnu_sm_baseline_loose) );
            st_w.repurpose( "wphotnu_sm_cands" , "W event candidates after exactly 1 SMWW photon selection" );
            st_w.for_each( w_photnu_sm_baseline_loose.begin(), w_photnu_sm_baseline_loose.end() );
          }
          if ( ele_smww_medium.size() == 1 ) {
            build_w_candidates( ele_smww_medium.begin() , ele_smww_medium.end() ,
                                met_hww.begin() , met_hww.end() ,
                                jet_smww.begin() , jet_smww.end() ,
                                primary_vertices.begin() , primary_vertices.end() ,
                                back_inserter(w_enu_sm_medium_baseline_loose) );
            st_w.repurpose( "wenu_sm_medium_cands" , "W event candidates after exactly 1 SMWW medium electron selection" );
            st_w.for_each( w_enu_sm_medium_baseline_loose.begin(), w_enu_sm_medium_baseline_loose.end() );
          }
          if ( ele_smww_revIso.size() == 1 ) {
            build_w_candidates( ele_smww_revIso.begin() , ele_smww_revIso.end() ,
                                met_hww.begin() , met_hww.end() ,
                                jet_smww.begin() , jet_smww.end() ,
                                primary_vertices.begin() , primary_vertices.end() ,
                                back_inserter(w_enu_sm_revIso_baseline_loose) );
            st_w.repurpose( "wenu_sm_revIso_cands" , "W event candidates after exactly 1 SMWW revIso electron selection" );
            st_w.for_each( w_enu_sm_revIso_baseline_loose.begin(), w_enu_sm_revIso_baseline_loose.end() );
          }
          if ( mu_smww_revIso.size() == 1 ) {
            build_w_candidates( mu_smww_revIso.begin() , mu_smww_revIso.end() ,
                                met_hww.begin() , met_hww.end() ,
                                jet_smww.begin() , jet_smww.end() ,
                                primary_vertices.begin() , primary_vertices.end() ,
                                back_inserter(w_munu_sm_revIso_baseline_loose) );
            st_w.repurpose( "wmunu_sm_revIso_cands" , "W event candidates after exactly 1 SMWW revIso muon selection" );
            st_w.for_each( w_munu_sm_revIso_baseline_loose.begin(), w_munu_sm_revIso_baseline_loose.end() );
          }
        }
      }
    }
    if( lnu_smww.size()==1 && !wjets_smww.empty()) {
      // if ((!enu_smww.empty() || !munu_smww.empty()) && !wjets_smww.empty() ) { 
      // SM WW candidates
      CompositeGenerator<const AnaW,const AnaHiggs> hgen;
      hgen.generate( munu_smww.begin() , munu_smww.end() , 
                     w_jj_closestmw_smww.begin(),w_jj_closestmw_smww.end() ,
                     //wjets_smww.begin() , wjets_smww.end() , 
                     AnaHiggs::Compose() , hmu_smww );
      hgen.generate( enu_smww.begin() , enu_smww.end() , 
                     w_jj_closestmw_smww.begin(),w_jj_closestmw_smww.end() ,
                     //wjets_smww.begin() , wjets_smww.end() , 
                     AnaHiggs::Compose() , he_smww );
      // make H->WW->lnuqq analysis candidates
      assert(lepton_smww.begin()!=lepton_smww.end());
      AnaHWWCandidate::Compose< EventObjectColl< shared_ptr<const AnaParticle> >::const_iterator ,
        EventObjectColl< shared_ptr<const AnaMET> >::const_iterator ,
        EventObjectColl< shared_ptr<const AnaJet> >::const_iterator ,
        EventObjectColl< shared_ptr<const AnaVertex> >::const_iterator
        > hww_compositor(lepton_smww.begin() , lepton_smww.end() ,
						 met_hww.begin() , met_hww.end() ,
                         jet_smww.begin() , jet_smww.end(),
                         primary_vertices.begin() , primary_vertices.end() );      
      std::transform( hmu_smww.begin() , hmu_smww.end() , 
                      hmuww_smww.back_inserter() ,
                      bind( hww_compositor, _1 ) );
      hww_smww.push_back_all(hmuww_smww.begin(), hmuww_smww.end());
      std::transform( he_smww.begin() , he_smww.end() , 
                      heww_smww.back_inserter() ,
                      bind( hww_compositor, _1 ) );
      hww_smww.push_back_all(heww_smww.begin(), heww_smww.end());
      // make H->WW analysis candidates
      dg::fillh( "n_h_sm_cands_before" , 4 , 0 , 4 , hww_smww.size() );
      // mark's plots for higgs candidates
      // study here higgs before cuts
      st_hall.repurpose("h_sm_cands_before" , "H event candidates before full selection (SMWW lepton)" );
      st_hall.for_each( hww_smww.begin(), hww_smww.end() );
      // select H->WW analysis candidates
      {
        dg::down( "selectors" , "selector details" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;	
        gghww_sm_jan20_selector.for_each_if( hww_smww.begin() , hww_smww.end(),
                                             bind( &AnaHWWCandidate::event_njets , _1 ) == 2 ||
                                             bind( &AnaHWWCandidate::event_njets , _1 ) == 3 );
        vbfhww_sm_jan20_selector.for_each_if( hww_smww.begin() , hww_smww.end(),
                                              bind( &AnaHWWCandidate::event_njets , _1 ) >= 4 );
      }
      {
        // fill histograms for the above
        st_hall.repurpose("ggh_sm_cands_after" , "ggH event candidates after full selection (SMWW lepton)" );
        st_hall.for_each( gghww_sm_jan20.begin() , gghww_sm_jan20.end() );
        st_hall.repurpose("vbfh_sm_cands_after" , "vbfH event candidates after full selection (SMWW lepton)" );
        st_hall.for_each( vbfhww_sm_jan20.begin() , vbfhww_sm_jan20.end() );
      }
      {
        dg::down( "selectors" , "selector details" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
        ggh_sm_mu2j_jan20_sel.for_each_if( hmuww_smww.begin() , hmuww_smww.end(),
                                           bind( &AnaHWWCandidate::event_njets , _1 ) == 2 );
        ggh_sm_mu3j_jan20_sel.for_each_if( hmuww_smww.begin() , hmuww_smww.end(),
                                           bind( &AnaHWWCandidate::event_njets , _1 ) == 3 );
        ggh_sm_e2j_jan20_sel.for_each_if( heww_smww.begin() , heww_smww.end(),
                                          bind( &AnaHWWCandidate::event_njets , _1 ) == 2 );
        ggh_sm_e3j_jan20_sel.for_each_if( heww_smww.begin() , heww_smww.end(),
                                          bind( &AnaHWWCandidate::event_njets , _1 ) == 3 );
        vbfh_sm_muge4j_jan20_sel.for_each_if( hmuww_smww.begin() , hmuww_smww.end(),
                                              bind( &AnaHWWCandidate::event_njets , _1 ) >= 4 );
        vbfh_sm_ege4j_jan20_sel.for_each_if( heww_smww.begin() , heww_smww.end(),
                                             bind( &AnaHWWCandidate::event_njets , _1 ) >= 4 );
      }
    }  
    {
      if ( !is_mc ) {
        if( !photnu_smww.empty() && !wjets_smww.empty() && enu_smww.empty()  && munu_smww.empty() ) { 
          // SM WW candidates
          CompositeGenerator<const AnaW,const AnaHiggs> hgen;
          hgen.generate( photnu_smww.begin() , photnu_smww.end() , 
                         w_jj_closestmw_smww.begin(),w_jj_closestmw_smww.end() ,
                         //wjets_smww.begin() , wjets_smww.end() , 
                         AnaHiggs::Compose() , hphot_smww );
          // make H->WW->lnuqq analysis candidates
          assert(photon_smww.begin()!=photon_smww.end());
          AnaHWWCandidate::Compose< EventObjectColl< shared_ptr<const AnaPhoton> >::const_iterator ,
            EventObjectColl< shared_ptr<const AnaMET> >::const_iterator ,
            EventObjectColl< shared_ptr<const AnaJet> >::const_iterator ,
            EventObjectColl< shared_ptr<const AnaVertex> >::const_iterator
            > hww_compositor(photon_smww.begin() , photon_smww.end() ,
                             met_hww.begin() , met_hww.end() ,
                             jet_smww.begin() , jet_smww.end(),
                             primary_vertices.begin() , primary_vertices.end() );
          std::transform( hphot_smww.begin() , hphot_smww.end() , 
                          hphotww_smww.back_inserter() ,
                          bind( hww_compositor, _1 ) );

          // mark's plots for higgs candidates
          // study here higgs before cuts
          st_hall.repurpose("hphot_sm_cands_before" , "H event candidates before full selection (SMWW phot)" );
          st_hall.for_each( hphotww_smww.begin(), hphotww_smww.end() );
          // select H->WW analysis candidates
          {
            dg::down( "selectors" , "selector details" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
            gghww_sm_jan20_phot_selector.for_each_if( hphotww_smww.begin() , hphotww_smww.end(),
                                                      bind( &AnaHWWCandidate::event_njets , _1 ) == 2 ||
                                                      bind( &AnaHWWCandidate::event_njets , _1 ) == 3	);
            vbfhww_sm_jan20_phot_selector.for_each_if( hphotww_smww.begin() , hphotww_smww.end(),
                                                       bind( &AnaHWWCandidate::event_njets , _1 ) >= 4 );
          }
          {
            // fill histograms for the above
            st_hall.repurpose("gghphot_sm_cands_after" , "ggH event candidates after full selection (SMWW phot)" );
            st_hall.for_each( gghww_sm_jan20_phot.begin() , gghww_sm_jan20_phot.end() );
            st_hall.repurpose("vbfhphot_sm_cands_after" , "vbfH event candidates after full selection (SMWW phot)" );
            st_hall.for_each( vbfhww_sm_jan20_phot.begin() , vbfhww_sm_jan20_phot.end() );
          }
          {
            dg::down( "selectors" , "selector details" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
            ggh_sm_phot2j_jan20_sel.for_each_if( hphotww_smww.begin() , hphotww_smww.end(),
                                                 bind( &AnaHWWCandidate::event_njets , _1 ) == 2 );
            ggh_sm_phot3j_jan20_sel.for_each_if( hphotww_smww.begin() , hphotww_smww.end(),
                                                 bind( &AnaHWWCandidate::event_njets , _1 ) == 3 );
            vbfh_sm_photge4j_jan20_sel.for_each_if( hphotww_smww.begin() , hphotww_smww.end(),
                                                    bind( &AnaHWWCandidate::event_njets , _1 ) >= 4 );
          }
        }
        if( !enu_smww_medium.empty() && !wjets_smww.empty() && enu_smww.empty()  && munu_smww.empty() ) { 
          // SM WW candidates
          CompositeGenerator<const AnaW,const AnaHiggs> hgen;
          hgen.generate( enu_smww_medium.begin() , enu_smww_medium.end() , 
                         w_jj_closestmw_smww.begin(),w_jj_closestmw_smww.end() ,
                         //wjets_smww.begin() , wjets_smww.end() , 
                         AnaHiggs::Compose() , he_smww_medium );
          // make H->WW->lnuqq analysis candidates
          assert(ele_smww_medium.begin()!=ele_smww_medium.end());
          AnaHWWCandidate::Compose< EventObjectColl< shared_ptr<const AnaElectron> >::const_iterator ,
            EventObjectColl< shared_ptr<const AnaMET> >::const_iterator ,
            EventObjectColl< shared_ptr<const AnaJet> >::const_iterator ,
            EventObjectColl< shared_ptr<const AnaVertex> >::const_iterator
            > hww_compositor(ele_smww_medium.begin() , ele_smww_medium.end() ,
                             met_hww.begin() , met_hww.end() ,
                             jet_smww.begin() , jet_smww.end(),
                             primary_vertices.begin() , primary_vertices.end() );

          std::transform( he_smww_medium.begin() , he_smww_medium.end() , 
                          heww_smww_medium.back_inserter() ,
                          bind( hww_compositor, _1 ) );

          // mark's plots for higgs candidates
          // study here higgs before cuts
          st_hall.repurpose("he_sm_medium_cands_before" , "H event candidates before full selection (SMWW ele medium)" );
          st_hall.for_each( heww_smww_medium.begin(), heww_smww_medium.end() );
          // select H->WW analysis candidates
          {
            dg::down( "selectors" , "selector details" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
            gghww_sm_jan20_emedium_selector.for_each_if( heww_smww_medium.begin() , heww_smww_medium.end(),
                                                         bind( &AnaHWWCandidate::event_njets , _1 ) == 2 ||
                                                         bind( &AnaHWWCandidate::event_njets , _1 ) == 3	);
            vbfhww_sm_jan20_emedium_selector.for_each_if( heww_smww_medium.begin() , heww_smww_medium.end(),
                                                          bind( &AnaHWWCandidate::event_njets , _1 ) >= 4 );
          }
          {
            // fill histograms for the above
            st_hall.repurpose("gghe_sm_medium_cands_after" , "ggH event candidates after full selection (SMWW ele medium)" );
            st_hall.for_each( gghww_sm_jan20_emedium.begin() , gghww_sm_jan20_emedium.end() );
            st_hall.repurpose("vbfhe_sm_medium_cands_after" , "vbfH event candidates after full selection (SMWW ele medium)" );
            st_hall.for_each( vbfhww_sm_jan20_emedium.begin() , vbfhww_sm_jan20_emedium.end() );
          }
          {
            dg::down( "selectors" , "selector details" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
            ggh_sm_medium_e2j_jan20_sel.for_each_if( heww_smww_medium.begin() , heww_smww_medium.end(),
                                                     bind( &AnaHWWCandidate::event_njets , _1 ) == 2 );
            ggh_sm_medium_e3j_jan20_sel.for_each_if( heww_smww_medium.begin() , heww_smww_medium.end(),
                                                     bind( &AnaHWWCandidate::event_njets , _1 ) == 3 );
            vbfh_sm_medium_ege4j_jan20_sel.for_each_if( heww_smww_medium.begin() , heww_smww_medium.end(),
                                                        bind( &AnaHWWCandidate::event_njets , _1 ) >= 4 );
          }
        }
        if( !enu_smww_revIso.empty() && !wjets_smww.empty() && enu_smww.empty()  && munu_smww.empty() ) { 
          // SM WW candidates
          CompositeGenerator<const AnaW,const AnaHiggs> hgen;
          hgen.generate( enu_smww_revIso.begin() , enu_smww_revIso.end() , 
                         w_jj_closestmw_smww.begin(),w_jj_closestmw_smww.end() ,
                         //wjets_smww.begin() , wjets_smww.end() , 
                         AnaHiggs::Compose() , he_smww_revIso );
          // make H->WW->lnuqq analysis candidates
          assert(ele_smww_revIso.begin()!=ele_smww_revIso.end());
          AnaHWWCandidate::Compose< EventObjectColl< shared_ptr<const AnaElectron> >::const_iterator ,
            EventObjectColl< shared_ptr<const AnaMET> >::const_iterator ,
            EventObjectColl< shared_ptr<const AnaJet> >::const_iterator ,
            EventObjectColl< shared_ptr<const AnaVertex> >::const_iterator
            > hww_compositor(ele_smww_revIso.begin() , ele_smww_revIso.end() ,
                             met_hww.begin() , met_hww.end() ,
                             jet_smww.begin() , jet_smww.end(),
                             primary_vertices.begin() , primary_vertices.end() );

          std::transform( he_smww_revIso.begin() , he_smww_revIso.end() , 
                          heww_smww_revIso.back_inserter() ,
                          bind( hww_compositor, _1 ) );

          // mark's plots for higgs candidates
          // study here higgs before cuts
          st_hall.repurpose("he_sm_revIso_cands_before" , "H event candidates before full selection (SMWW ele revIso)" );
          st_hall.for_each( heww_smww_revIso.begin(), heww_smww_revIso.end() );
          // select H->WW analysis candidates
          {
            dg::down( "selectors" , "selector details" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
            gghww_sm_jan20_erevIso_selector.for_each_if( heww_smww_revIso.begin() , heww_smww_revIso.end(),
                                                         bind( &AnaHWWCandidate::event_njets , _1 ) == 2 ||
                                                         bind( &AnaHWWCandidate::event_njets , _1 ) == 3	);
            vbfhww_sm_jan20_erevIso_selector.for_each_if( heww_smww_revIso.begin() , heww_smww_revIso.end(),
                                                          bind( &AnaHWWCandidate::event_njets , _1 ) >= 4 );
          }
          {
            // fill histograms for the above
            st_hall.repurpose("gghe_sm_revIso_cands_after" , "ggH event candidates after full selection (SMWW ele revIso)" );
            st_hall.for_each( gghww_sm_jan20_erevIso.begin() , gghww_sm_jan20_erevIso.end() );
            st_hall.repurpose("vbfhe_sm_revIso_cands_after" , "vbfH event candidates after full selection (SMWW ele revIso)" );
            st_hall.for_each( vbfhww_sm_jan20_erevIso.begin() , vbfhww_sm_jan20_erevIso.end() );
          }
          {
            dg::down( "selectors" , "selector details" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
            ggh_sm_revIso_e2j_jan20_sel.for_each_if( heww_smww_revIso.begin() , heww_smww_revIso.end(),
                                                     bind( &AnaHWWCandidate::event_njets , _1 ) == 2 );
            ggh_sm_revIso_e3j_jan20_sel.for_each_if( heww_smww_revIso.begin() , heww_smww_revIso.end(),
                                                     bind( &AnaHWWCandidate::event_njets , _1 ) == 3 );
            vbfh_sm_revIso_ege4j_jan20_sel.for_each_if( heww_smww_revIso.begin() , heww_smww_revIso.end(),
                                                        bind( &AnaHWWCandidate::event_njets , _1 ) >= 4 );
          }
        }
        if( !munu_smww_revIso.empty() && !wjets_smww.empty() && enu_smww.empty()  && munu_smww.empty() ) { 
          // SM WW candidates
          CompositeGenerator<const AnaW,const AnaHiggs> hgen;
          hgen.generate( munu_smww_revIso.begin() , munu_smww_revIso.end() , 
                         w_jj_closestmw_smww.begin(),w_jj_closestmw_smww.end() ,
                         //wjets_smww.begin() , wjets_smww.end() , 
                         AnaHiggs::Compose() , hmu_smww_revIso );
          // make H->WW->lnuqq analysis candidates
          assert(mu_smww_revIso.begin()!=mu_smww_revIso.end());
          AnaHWWCandidate::Compose< EventObjectColl< shared_ptr<const AnaMuon> >::const_iterator ,
            EventObjectColl< shared_ptr<const AnaMET> >::const_iterator ,
            EventObjectColl< shared_ptr<const AnaJet> >::const_iterator ,
            EventObjectColl< shared_ptr<const AnaVertex> >::const_iterator
            > hww_compositor(mu_smww_revIso.begin() , mu_smww_revIso.end() ,
                             met_hww.begin() , met_hww.end() ,
                             jet_smww.begin() , jet_smww.end(),
                             primary_vertices.begin() , primary_vertices.end() );

          std::transform( hmu_smww_revIso.begin() , hmu_smww_revIso.end() , 
                          hmuww_smww_revIso.back_inserter() ,
                          bind( hww_compositor, _1 ) );

          // mark's plots for higgs candidates
          // study here higgs before cuts
          st_hall.repurpose("hmu_sm_revIso_cands_before" , "H event candidates before full selection (SMWW mu revIso)" );
          st_hall.for_each( hmuww_smww_revIso.begin(), hmuww_smww_revIso.end() );
          // select H->WW analysis candidates
          {
            dg::down( "selectors" , "selector details" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
            gghww_sm_jan20_murevIso_selector.for_each_if( hmuww_smww_revIso.begin() , hmuww_smww_revIso.end(),
                                                          bind( &AnaHWWCandidate::event_njets , _1 ) == 2 ||
                                                          bind( &AnaHWWCandidate::event_njets , _1 ) == 3	);
            vbfhww_sm_jan20_murevIso_selector.for_each_if( hmuww_smww_revIso.begin() , hmuww_smww_revIso.end(),
                                                           bind( &AnaHWWCandidate::event_njets , _1 ) >= 4 );
          }
          {
            // fill histograms for the above
            st_hall.repurpose("gghmu_sm_revIso_cands_after" , "ggH event candidates after full selection (SMWW mu revIso)" );
            st_hall.for_each( gghww_sm_jan20_murevIso.begin() , gghww_sm_jan20_murevIso.end() );
            st_hall.repurpose("vbfhmu_sm_revIso_cands_after" , "vbfH event candidates after full selection (SMWW mu revIso)" );
            st_hall.for_each( vbfhww_sm_jan20_murevIso.begin() , vbfhww_sm_jan20_murevIso.end() );
          }
          {
            dg::down( "selectors" , "selector details" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
            ggh_sm_revIso_mu2j_jan20_sel.for_each_if( hmuww_smww_revIso.begin() , hmuww_smww_revIso.end(),
                                                      bind( &AnaHWWCandidate::event_njets , _1 ) == 2 );
            ggh_sm_revIso_mu3j_jan20_sel.for_each_if( hmuww_smww_revIso.begin() , hmuww_smww_revIso.end(),
                                                      bind( &AnaHWWCandidate::event_njets , _1 ) == 3 );
            vbfh_sm_revIso_muge4j_jan20_sel.for_each_if( hmuww_smww_revIso.begin() , hmuww_smww_revIso.end(),
                                                         bind( &AnaHWWCandidate::event_njets , _1 ) >= 4 );
          }
        }
      }
    }
  } // end for each event

    // end the analysis. cleanup, histogram writing, etc. is automatic.
  cout << " analysis is complete." << endl;

  AnaEventMgr::instance()->close_sample();

} // end analysis


/* A map of trigger choices depending on period */
const std::string& which_trigger_mu(int r, bool mc) {
  static std::string ANY("ANY");
  static std::string NONE("NONE");
  static std::string MISSING_TRIGGER("MISSING_TRIGGER");
  static std::string L1_MU0("L1_MU0");
  static std::string L1_MU4("L1_MU4");
  static std::string L1_MU6("L1_MU6");
  static std::string L1_MU10("L1_MU10");
  static std::string EF_MU4("EF_MU4");
  static std::string EF_MU6("EF_MU6");
  static std::string EF_MU10("EF_MU10");
  static std::string EF_MU10_MG("EF_MU10_MG");
  static std::string EF_MU13_MG("EF_MU13_MG");
  static std::string EF_MU13_MG_TIGHT("EF_MU13_MG_TIGHT");
  if(mc) return EF_MU10_MG;
  if(r>=152166 && r<=153200) return L1_MU10; //A
  if(r>=153565 && r<=155160) return L1_MU10; //B
  if(r>=155228 && r<=156692) return L1_MU10; //C
  if(r>=158045 && r<=159224) return L1_MU10; //D
  if(r>=160387 && r<=160879) return L1_MU10; //E1-E3
  if(r>=160899 && r<=161948) return EF_MU10_MG; //E4-E7
  if(r>=162347 && r<=162882) return EF_MU10_MG; //F
  if(r>=165591 && r<=165632) return EF_MU10_MG; //G1
  if(r>=165703 && r<=166383) return EF_MU13_MG; //G2-G6
  if(r>=166466 && r<=166964) return EF_MU13_MG; //H1-H2
  if(r>=167575 && r<=167576) return EF_MU13_MG; //I1(up to 167576)
  if(r>=167607 && r<=167844) return EF_MU13_MG_TIGHT; //I1(from 167607)-I2
  //  if(r>=167575 && r<=167680) return EF_MU13_MG; //I1
  //  if(r>=167776 && r<=167844) return EF_MU13_MG_TIGHT; //I2
  return NONE;
}

/* A map of trigger choices depending on period */
const std::string& which_trigger_el(int r, bool mc) {
  static std::string ANY("ANY");
  static std::string NONE("NONE");
  static std::string MISSING_TRIGGER("MISSING_TRIGGER");
  static std::string L1_EM14("L1_EM14"); // A-E3, MC
  static std::string EF_E15_MEDIUM("EF_E15_MEDIUM"); // E4-on
  //if(mc) return L1_EM14; // W+jets MC trigger
  if(mc) return EF_E15_MEDIUM; // H->WW MC trigger
  if(r>=152166 && r<=153200) return L1_EM14; //A
  if(r>=153565 && r<=155160) return L1_EM14; //B
  if(r>=155228 && r<=156692) return L1_EM14; //C
  if(r>=158045 && r<=159224) return L1_EM14; //D
  if(r>=160387 && r<=160879) return L1_EM14; //E1-E3
  if(r>=160899 && r<=161948) return EF_E15_MEDIUM; //E4-E7
  if(r>=162347 && r<=162882) return EF_E15_MEDIUM; //F
  if(r>=165591 && r<=165632) return EF_E15_MEDIUM; //G1
  if(r>=165703 && r<=166383) return EF_E15_MEDIUM; //G2-G6
  if(r>=166466 && r<=166964) return EF_E15_MEDIUM; //H1-H2
  if(r>=167575 && r<=167576) return EF_E15_MEDIUM; //I1(up to 167576)
  if(r>=167607 && r<=167844) return EF_E15_MEDIUM; //I1(from 167607)-I2
  return NONE;
}

const std::string& which_trigger_phot(int r, bool mc) {
  static std::string ANY("ANY");
  static std::string NONE("NONE");
  static std::string MISSING_TRIGGER("MISSING_TRIGGER");
  static std::string EF_G20_LOOSE("EF_G20_LOOSE"); // E4-on, data
  if(mc) return NONE;
  if(r>=152166 && r<=153200) return NONE; //A
  if(r>=153565 && r<=155160) return NONE; //B
  if(r>=155228 && r<=156692) return NONE; //C
  if(r>=158045 && r<=159224) return NONE; //D
  if(r>=160387 && r<=160879) return NONE; //E1-E3
  if(r>=160899 && r<=161948) return EF_G20_LOOSE; //E4-E7
  if(r>=162347 && r<=162882) return EF_G20_LOOSE; //F
  if(r>=165591 && r<=165632) return EF_G20_LOOSE; //G1
  if(r>=165703 && r<=166383) return EF_G20_LOOSE; //G2-G6
  if(r>=166466 && r<=166964) return EF_G20_LOOSE; //H1-H2
  if(r>=167575 && r<=167576) return EF_G20_LOOSE; //I1(up to 167576)
  if(r>=167607 && r<=167844) return EF_G20_LOOSE; //I1(from 167607)-I2
  return NONE;
}

/* A map of period names depending on run */
const std::string& which_period(int r) {
  static std::string OTHER("OTHER");
  static std::string A("A");
  static std::string B("B");
  static std::string C("C");
  static std::string D("D");
  static std::string E("E");
  static std::string F("F");
  static std::string G1("G1");
  static std::string G2("G2");
  static std::string G3("G3");
  static std::string G4("G4");
  static std::string G5("G5");
  static std::string G6("G6");
  static std::string H1("H1");
  static std::string H2("H2");
  static std::string I1("I1");
  static std::string I2("I2");
  
  if(r>=152166 && r<=153200) return A; //A
  if(r>=153565 && r<=155160) return B; //B
  if(r>=155228 && r<=156692) return C; //C
  if(r>=158045 && r<=159224) return D; //D
  if(r>=160387 && r<=160879) return E; //E1-E3
  if(r>=160899 && r<=161948) return E; //E4-E7
  if(r>=162347 && r<=162882) return F; //F
  if(r>=165591 && r<=165632) return G1; //G1
  if(r>=165703 && r<=165732) return G2;
  if(r>=165767 && r<=165815) return G3;
  if(r>=165817 && r<=165818) return G4;
  if(r>=165821 && r<=166143) return G5;
  if(r>=166198 && r<=166383) return G6;
  if(r>=166466 && r<=166850) return H1; //H1
  if(r>=166856 && r<=166964) return H2; //H1
  if(r>=167575 && r<=167680) return I1; //I1
  if(r>=167776 && r<=167844) return I2; //I2
  return OTHER;
}

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
template< typename lepIterT , typename metIterT , typename jetIterT , typename vtxIterT , typename outputIterT >
void build_z_candidates( lepIterT lep_begin , lepIterT lep_end ,
                         metIterT met_begin , metIterT met_end ,
                         jetIterT jet_begin , jetIterT jet_end ,
                         vtxIterT vtx_begin , vtxIterT vtx_end ,
                         outputIterT output_iterator )
{
  boost::shared_ptr<AnaZCrossSectionCandidate> cand( AnaZCrossSectionCandidate::build( lep_begin , lep_end ,
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
