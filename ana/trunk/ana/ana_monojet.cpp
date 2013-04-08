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
#include "TrigFTKAna/AnaMonojetCandidate.hpp"
#include "TrigFTKAna/StudyMonojet.hpp"
#include "TrigFTKAna/EventObjectColl.hpp"
#include "TrigFTKAna/Conference.hpp"
#include "TrigFTKAna/jetetmiss/LArHole_v2_00.hpp"
#include "DataGraphics/DataGraphics.hh"
#include "DataGraphics/DgCutSet.hh"
#include "DataGraphics/DgSTL.hh"
#include "TLorentzVector.h"
#include <stdio.h>

// specialized classes
#include "PileupReweighting/TPileupReweighting.h"
#include "GoodRunsLists/TGoodRunsListReader.h"
#include "GoodRunsLists/TGoodRunsList.h"
#include "MissingETUtility/METUtility.h"

using namespace std;
using namespace DataGraphics;
using namespace boost;


// shortcuts for cuts and collections
typedef EventObjectColl< boost::shared_ptr<const AnaTruthParticle> > TPCOL;
typedef DgCutSet< boost::shared_ptr<const AnaTruthParticle> > TPCUT;
typedef EventObjectColl< boost::shared_ptr<const AnaTrack> > TKCOL;
typedef DgCutSet< boost::shared_ptr<const AnaTrack> > TKCUT;
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
typedef EventObjectColl< boost::shared_ptr<const AnaMonojetCandidate> > MJCOL;
typedef DgCutSet< boost::shared_ptr<const AnaMonojetCandidate> > MJCUT;

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

void plot_trigger_efficiencies( const std::string& name , const boost::shared_ptr<const AnaMET>& reffinal , const boost::shared_ptr<const AnaMET>& lochadtopo , const boost::shared_ptr<const AnaTrigger>& trigger , const double& leading_jet_pt , const unsigned long& run_number , const double& ev_weight );

void mu_eff_scale( MUCOL& mucoll , const unsigned long& run_number , std::vector<double>& int_lumi , std::vector<std::string>& run_periods ,
		   double& eff_weight , double& eff_stat_error , double& eff_sys_error );

const bool jet_larhole_veto( std::vector< boost::shared_ptr<const AnaJet> >& jetcoll , const bool& is_mc , const unsigned long& run_number );

const double jet_drmin( boost::shared_ptr<const AnaJet>& jet , JETCOL& jetcoll );

// random constants
const detector::MCP_TYPE mu_type = detector::MCP_STACO_COMBINED;
const detector::EGAMMA_TYPE el_type = detector::EGAMMA_MEDIUMPP;

////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C"
#endif
int
F77_MAIN( int argc , char* argv[] )
{

  AnaConfiguration::configure("ana_monojet",argc,argv);
  if( !AnaConfiguration::configured() ) { exit(0); }
  dg::root()->no_save_at_exit();
  BOOST_SCOPE_EXIT() { dg::root()->do_save_at_exit(); } BOOST_SCOPE_EXIT_END;

  const int REL = AnaConfiguration::release();
  AnaConfiguration::conf( CONF::SUSI );
  const CONF::ConfType conf = AnaConfiguration::conf();
  std::cout << "Athena release = " << REL << std::endl;
  std::cout << "Conference code = " << conf << std::endl;

  // define lists of analysis objects for each event. insert each
  // object collection into this list. each EventObjectColl will
  // automatically get cleared at the beginning of each event.

  // event preselection
  // ================================================================
  Root::TGoodRunsListReader grlR;
  Root::TGoodRunsList grl;
  // updated 2012-06-01 from here: http://atlasdqm.web.cern.ch/atlasdqm/grlgen/SymLinks/
  // I know it says "JETMET" here, but this is actually the good run list we're using in all streams  
  std::string grlname = "./good_run_lists/data12_8TeV_pro14_latest_All_Good.xml";
  grlR.SetXMLFile( grlname.c_str() );
  if( !grlR.Interpret() ) assert(0&&"Failed to add a GRL file");
  grl = grlR.GetMergedGoodRunsList();


  // define primary vertex selections
  // ================================================================
  VXCOL vtx_coll , vtx_multijes_coll , vtx_jetcalib_coll;

  VXCUT vtx_sel( "vtx_sel" );
  vtx_sel.add_uu( "ntracks" , bind(&AnaVertex::ntracks,_1)>=2 , bind( &dg::fillh , _1 , 15 , 0 , 15 , bind(&AnaVertex::ntracks,_2)) );
  //vtx_sel.add_uu( "z_vtx" , bind(std::ptr_fun<AnaVertex::ftype,AnaVertex::ftype>(std::abs),bind(&AnaVertex::z,_1)) < 200., bind(&dg::fillh,_1,90,-300,300,bind(&AnaVertex::z,_2)) );

  VXCUT vtx_multijes_sel( "vtx_multijes_sel" );
  vtx_multijes_sel.add_uu( "ntracks" , bind(&AnaVertex::ntracks,_1)>=5 , bind( &dg::fillh , _1 , 15 , 0 , 15 , bind(&AnaVertex::ntracks,_2)) );

  VXCUT vtx_jetcalib_sel( "vtx_jetcalib_sel" );
  vtx_jetcalib_sel.based_on( vtx_sel );

  // define truth selections
  // ================================================================

  // define electron selections
  // ================================================================
  ELCOL el_calib_coll , el_coll;

  ELCUT el_sel( "el_common_sel" );
  el_sel.add_uu( "author" , bind(&AnaElectron::author,_1)==1 || bind(&AnaElectron::author,_1)==3 , bind(&dg::fillh,_1,10,0,10,bind(&AnaElectron::author,_2)) );
  el_sel.add_uu( "is_good_oq" , bind(&AnaElectron::is_good_oq,_1)==1 , bind(&dg::filleff,_1,bind(&AnaElectron::is_good_oq,_2)==1) );
  el_sel.add_uu( "mediumpp" , bind(&AnaElectron::is_aod_medium_plusplus,_1) , bind(&dg::filleff,_1,bind(&AnaElectron::is_aod_medium_plusplus,_2)) );
  //el_sel.add_uu( "cluster_eta" , bind( dgSTL::absF , bind(&AnaElectron::cluster_eta,_1) ) < 2.47 &&
  //( bind( dgSTL::absF , bind(&AnaElectron::cluster_eta,_1) ) < 1.37  || bind( dgSTL::absF , bind(&AnaElectron::cluster_eta,_1) ) > 1.52 ) ,
  //bind(&dg::fillh,_1,60,-3,3,bind(&AnaElectron::eta,_2)) );
  el_sel.add_uu( "cluster_eta" , bind(dgSTL::absF,bind(&AnaElectron::cluster_eta,_1)) < 2.47 , bind(&dg::fillh,_1,60,-3,3,bind(&AnaElectron::eta,_2)) );
  el_sel.add_uu( "cluster_et" , bind(&AnaElectron::cluster_pt,_1) > 20. , bind(&dg::fillh,_1,100,0,100,bind(&AnaElectron::cluster_pt,_2)) );


  // define muon selections
  // ================================================================
  MUCOL mu_calib_coll , mu_loose_coll , mu_medium_coll , mu_tight_coll;

  MUCUT mu_loose_sel( "mu_loose_sel" );
  //mu_loose_sel.add_uu( "staco" , bind(&AnaMuon::is_aod_staco,_1)==true , bind( &dg::filleff , _1 , _2 ) );
  mu_loose_sel.add_uu( "author" , bind(&AnaMuon::primary_author,_1)==1 || bind(&AnaMuon::primary_author,_1)==6 , bind(&dg::fillh,_1,10,0,10,bind(&AnaMuon::primary_author,_2)) );
  mu_loose_sel.add_uu( "mcp_idhits" , bind(&AnaMuon::passes_hits_mcp_2012,_1)==true, bind(&dg::fillh,_1,50,0,50,bind(&AnaMuon::nhits_silicon,_2)) );
  mu_loose_sel.add_uu( "eta" , bind(dgSTL::absF,bind(&AnaMuon::eta,_1)) < 2.5 , bind(&dg::fillh,_1,100,-5,5,bind(&AnaMuon::eta,_2)) );
  mu_loose_sel.add_uu( "pt_7gev" , bind(&AnaMuon::pt,_1) > 7. , bind(&dg::fillh,_1,100,0,100,bind(&AnaMuon::pt,_2)) );
  mu_loose_sel.add_uu( "pt_cone_20" , bind(&AnaMuon::pt_cone,_1,AnaMuon::CONE20) < 1.8 , bind(&dg::fillh,_1,100,0,30,bind(&AnaMuon::pt_cone,_2,AnaMuon::CONE20)) );
  //mu_loose_sel.add_uu( "z0" , bind(dgSTL::absF,bind(&AnaMuon::z0_vx,_1)) < 10. , bind(&dg::fillh,_1,100,-700,700,bind(&AnaMuon::z0_exPV,_2)) );
  //mu_loose_sel.add_uu( "d0" , bind(dgSTL::absF,bind(&AnaMuon::d0_vx_signif,_1)) < 10. , bind(&dg::fillh,_1,100,-50,50,bind(&AnaMuon::d0_exPV_signif,_2)) );

  MUCUT mu_medium_sel( "mu_medium_sel" );
  mu_medium_sel.based_on( mu_loose_sel );
  mu_medium_sel.add_uu( "pt_cone_20_ratio" , bind(&AnaMuon::pt_cone_ratio,_1,AnaMuon::CONE20) < 0.1 , bind(&dg::fillh,_1,100,0,1,bind(&AnaMuon::pt_cone_ratio,_2,AnaMuon::CONE20)) );
  mu_medium_sel.add_uu( "pt_20gev"  , bind(&AnaMuon::pt,_1) > 20. , bind(&dg::fillh,_1,100,0,100,bind(&AnaMuon::pt,_2)) );  

  MUCUT mu_tight_sel( "mu_tight_sel" );
  mu_tight_sel.based_on( mu_medium_sel );
  mu_tight_sel.add_uu( "pt_25gev"  , bind(&AnaMuon::pt,_1) > 25. , bind(&dg::fillh,_1,100,0,100,bind(&AnaMuon::pt,_2)) );


  // define jet selections
  // ================================================================
  JETCOL lepjet_calib_coll , lepjet_coll , badlepjet_coll;

  // common selection
  JETCUT jet_common_sel( "jet_common_sel" );
//   jet_common_sel.add_uu( "algorithm_antikt4" , bind(&AnaJet::is_atlas_anti_kt_cone_4_h1_topo,_1)==true , bind(&dg::filleff,_1,bind(&AnaJet::is_atlas_anti_kt_cone_4_h1_topo,_2)==true) ); // sanity cut
//   jet_common_sel.add_uu( "rapidity", bind(dgSTL::absF,bind(&AnaJet::rapidity,_1)) < 4.5 , bind(&dg::fillh,_1,100,-4.5,4.5,bind(&AnaJet::rapidity,_2)) );

  // standard good jet selection
  JETCUT jet_sel( "jet_sel" );
  jet_sel.based_on( jet_common_sel );
//   jet_sel.add_uu( "pt" , bind(&AnaJet::pt,_1) > 30. , bind(&dg::fillh,_1,100,0,100,bind(&AnaJet::pt,_2)) );
//   jet_sel.add_uu( "medium_cleaning" , bind(&AnaJet::isBadJet_medium_v17,_1)==false , bind(&dg::fillh,_1,200,0,200,bind(&AnaJet::pt,_2)) );

  // bad jet selection to use for MET cleaning
  JETCUT jet_bad_sel( "jet_bad_sel" );
  jet_bad_sel.based_on( jet_common_sel );
  jet_bad_sel.add_uu( "pt" , bind(&AnaJet::pt,_1) > 20. , bind(&dg::fillh,_1,100,0,100,bind(&AnaJet::pt,_2)) );
  jet_bad_sel.add_uu( "bad_looser" , bind(&AnaJet::isBadJet_looser_v17,_1)==true , bind(&dg::fillh,_1,200,0,200,bind(&AnaJet::pt,_2)) );
  // no ugly requirement for now

  // MultijetJES selection (need to feed the number of passing jets into the MultjetJES calculator)
  JETCUT jet_multijes_sel( "jet_multijes_sel" );
  jet_multijes_sel.add_uu( "pt" , bind(&AnaJet::pt,_1) > 7. , bind(&dg::fillh,_1,100,0,100,bind(&AnaJet::pt,_2)) );


  // MET selection
  // ================================================================
  METCOL met_calib_coll , met_reffinal_coll , met_lochadtopo_coll , met_reffinalcorr_coll;

  METCUT met_reffinal_sel( "met_reffinal_sel" );
  met_reffinal_sel.add_u( "met_type" , bind(&AnaMET::met_type,_1) == AnaMET::REF_FINAL );

  METCUT met_reffinalcorr_sel( "met_reffinalcorr_sel" );
  met_reffinalcorr_sel.add_u( "met_type" , bind(&AnaMET::met_type,_1) == AnaMET::FINAL );  

  METCUT met_lochadtopo_sel( "met_lochadtopo_sel" );
  met_lochadtopo_sel.add_u( "met_type" , bind(&AnaMET::met_type,_1) == AnaMET::LOCHADTOPO );


  // TRUTH selection
  // ================================================================
  TPCOL truth_coll;

  TPCUT truth_sel( "truth_sel" );
  //truth_sel.add_u( "final_state" , bind(&AnaTruthParticle::is_final_state,_1) );
  truth_sel.add_u( "is_parton" , bind(&AnaTruthParticle::is_quark_or_gluon,_1) );

  // Track selection
  // ================================================================
  TKCOL track_coll;

  TKCUT track_sel( "track_sel" );
  track_sel.add_uu( "pixhits" , bind(&AnaTrack::pixel_nhits,_1) > 0 , bind(&dg::fillh,_1,10,0,10,bind(&AnaTrack::pixel_nhits,_2)) );
  track_sel.add_uu( "scthits" , bind(&AnaTrack::sct_nhits,_1) > 5 , bind(&dg::fillh,_1,10,0,10,bind(&AnaTrack::sct_nhits,_2)) );
  track_sel.add_uu( "d0" , bind(dgSTL::absF,bind(&AnaTrack::exPV_d0,_1)) < 0.2 , bind(&dg::fillh,_1,50,-1,1,bind(&AnaTrack::exPV_d0,_2)) );
  track_sel.add_uu( "z0" , bind(dgSTL::absF,bind(&AnaTrack::exPV_z0,_1)) < 0.45 , bind(&dg::fillh,_1,50,-1,1,bind(&AnaTrack::exPV_z0,_2)) );
  //track_sel.add_uu( "chi2ndof" , bind(&AnaTrack::chi2ndof,_1) < 3 , bind(&dg::fillh,_1,50,0,10,bind(&AnaTrack::chi2ndof,_2)) );


  // define event-wide selections
  // ================================================================
  MJCOL mj_calib_coll , mj_coll;

  MJCUT mj_sel( "mj_sel" );
  mj_sel.add_uu( "njets_ge_one" , bind(&AnaMonojetCandidate::njets,_1) > 0 , bind(&dg::fillh,_1,10,0,10,bind(&AnaMonojetCandidate::njets,_2)) );
  //mj_sel.add_uu( "leading_jet_emf" , bind(&AnaMonojetCandidate::leading_jet_emf,_1) > 0.1 ,
  //bind(&dg::fillh,_1,50,0,1,bind(&AnaMonojetCandidate::leading_jet_emf,_2)) );
  //mj_sel.add_uu( "leading_jet_chf" , bind(&AnaMonojetCandidate::leading_jet_chf,_1) > 0.1 ,
  //bind(&dg::fillh,_1,50,0,1,bind(&AnaMonojetCandidate::leading_jet_chf,_2)) );
  mj_sel.add_uu( "lepmet_pt" , bind(&AnaMonojetCandidate::lepmet_pt,_1) > 25. , bind(&dg::fillh,_1,100,0,200,bind(&AnaMonojetCandidate::lepmet_pt,_2)) );


  MJCUT mj_sel_pgs( "mj_sel" );
  mj_sel_pgs.add_uu( "njets_ge_one" , bind(&AnaMonojetCandidate::njets,_1) > 0 , bind(&dg::fillh,_1,10,0,10,bind(&AnaMonojetCandidate::njets,_2)) );
  mj_sel_pgs.add_uu( "lepmet_pt" , bind(&AnaMonojetCandidate::lepmet_pt,_1) > 25. , bind(&dg::fillh,_1,100,0,200,bind(&AnaMonojetCandidate::lepmet_pt,_2)) );

  // The following cut sets are used only for producing cutflows. The cuts are never actually applied.
  MJCUT mj_sr_full_sel( "mj_sr_full_sel" );
  mj_sr_full_sel.based_on( mj_sel );
  mj_sr_full_sel.add_uu( "trigger" , bind(&AnaMonojetCandidate::met_trigger,_1) , bind(&dg::fillh,_1,1,1,2,1) );
  mj_sr_full_sel.add_u( "electron_veto" , bind(&AnaMonojetCandidate::nelectrons,_1)==0 );
  mj_sr_full_sel.add_u( "muon_veto" , bind(&AnaMonojetCandidate::nmuons,_1)==0 );
  mj_sr_full_sel.add_u( "third_jet_pt" , bind(&AnaMonojetCandidate::third_jet_pt,_1)<30. );
  mj_sr_full_sel.add_u( "second_jet_dphi" , bind(&AnaMonojetCandidate::njets,_1)<2 || bind(dgSTL::absF,bind(&AnaMonojetCandidate::dphi_met_second_jet,_1))>0.3 );
  mj_sr_full_sel.add_u( "jetmet_120" , bind(&AnaMonojetCandidate::first_jet_pt,_1)>120. && bind(&AnaMonojetCandidate::met_pt,_1)>120. );
  mj_sr_full_sel.add_u( "jetmet_220" , bind(&AnaMonojetCandidate::first_jet_pt,_1)>220. && bind(&AnaMonojetCandidate::met_pt,_1)>220. );
  mj_sr_full_sel.add_u( "jetmet_350" , bind(&AnaMonojetCandidate::first_jet_pt,_1)>350. && bind(&AnaMonojetCandidate::met_pt,_1)>350. );
  mj_sr_full_sel.add_u( "jetmet_500" , bind(&AnaMonojetCandidate::first_jet_pt,_1)>500. && bind(&AnaMonojetCandidate::met_pt,_1)>500. );
  mj_sr_full_sel.add_u( "jetmet_800" , bind(&AnaMonojetCandidate::first_jet_pt,_1)>800. && bind(&AnaMonojetCandidate::met_pt,_1)>800. );

  MJCUT mj_cr_common_sel( "mj_cr_sel" );
  mj_cr_common_sel.add_uu( "met_pt" , bind(&AnaMonojetCandidate::met_pt,_1) > 25. , bind(&dg::fillh,_1,100,0,500,bind(&AnaMonojetCandidate::met_pt,_2)) );
  mj_cr_common_sel.add_uu( "dphi_met_jet1" , bind(dgSTL::absF,bind(&AnaMonojetCandidate::dphi_met_second_jet,_1)) > 0.3 ,
                           bind(&dg::fillh,_1,100,-2,2,bind(&AnaMonojetCandidate::dphi_met_second_jet,_2)) );
  mj_cr_common_sel.add_uu( "leading_jet" , bind(&AnaMonojetCandidate::first_jet_pt,_1) > 60. ,
                           bind(&dg::fillh,_1,100,0,500,bind(&AnaMonojetCandidate::first_jet_pt,_2)) );

  MJCUT mj_zmumu_sel( "mj_zmumu_sel" );
  mj_zmumu_sel.based_on( mj_cr_common_sel );
  mj_zmumu_sel.add_uu( "trigger" , bind(&AnaMonojetCandidate::mu_trigger,_1)==true , bind(&dg::filleff,_1,bind(&AnaMonojetCandidate::mu_trigger,_2)) );
  mj_zmumu_sel.add_uu( "nmuons" , bind(&AnaMonojetCandidate::nmuons,_1)==2 , bind(&dg::fillh,_1,10,0,10,bind(&AnaMonojetCandidate::nmuons,_2)) );
  mj_zmumu_sel.add_uu( "nelectrons" , bind(&AnaMonojetCandidate::nelectrons,_1)==0 , bind(&dg::fillh,_1,10,0,10,bind(&AnaMonojetCandidate::nelectrons,_2)) );
  mj_zmumu_sel.add_uu( "ossf" , bind(&AnaMonojetCandidate::muons_ossf,_1)==true , bind(&dg::filleff,_1,bind(&AnaMonojetCandidate::muons_ossf,_2)) );
  mj_zmumu_sel.add_uu( "min_muon_pt" , bind(&AnaMonojetCandidate::min_muon_pt,_1) > 25 , bind(&dg::fillh,_1,100,0,250,bind(&AnaMonojetCandidate::min_muon_pt,_2)) );
  mj_zmumu_sel.add_uu( "z_m" , bind(&AnaMonojetCandidate::z_mass_cut,_1)==true , bind(&dg::fillh,_1,100,0,250,bind(&AnaMonojetCandidate::z_m,_2)) );

  MJCUT mj_zee_sel( "mj_zee_sel" );
  mj_zee_sel.based_on( mj_cr_common_sel );
  mj_zee_sel.add_uu( "trigger" , bind(&AnaMonojetCandidate::el_trigger,_1)==true , bind(&dg::filleff,_1,bind(&AnaMonojetCandidate::el_trigger,_2)) );
  mj_zee_sel.add_uu( "nmuons" , bind(&AnaMonojetCandidate::nmuons,_1)==0 , bind(&dg::fillh,_1,10,0,10,bind(&AnaMonojetCandidate::nmuons,_2)) );
  mj_zee_sel.add_uu( "nelectrons" , bind(&AnaMonojetCandidate::nelectrons,_1)==2 , bind(&dg::fillh,_1,10,0,10,bind(&AnaMonojetCandidate::nelectrons,_2)) );
  mj_zee_sel.add_uu( "ossf" , bind(&AnaMonojetCandidate::electrons_ossf,_1)==true , bind(&dg::filleff,_1,bind(&AnaMonojetCandidate::electrons_ossf,_2)) );
  mj_zee_sel.add_uu( "min_electron_pt" , bind(&AnaMonojetCandidate::min_electron_pt,_1)>25 , bind(&dg::fillh,_1,100,0,250,bind(&AnaMonojetCandidate::min_electron_pt,_2)) );
  mj_zee_sel.add_uu( "z_m" , bind(&AnaMonojetCandidate::z_mass_cut,_1)==true , bind(&dg::fillh,_1,100,0,250,bind(&AnaMonojetCandidate::z_m,_2)) );

  MJCUT mj_wmunu_sel( "mj_wmunu_sel" );
  mj_wmunu_sel.based_on( mj_cr_common_sel );
  mj_wmunu_sel.add_uu( "trigger" , bind(&AnaMonojetCandidate::mu_trigger,_1)==true , bind(&dg::filleff,_1,bind(&AnaMonojetCandidate::mu_trigger,_2)) );
  mj_wmunu_sel.add_uu( "nmuons" , bind(&AnaMonojetCandidate::nmuons,_1)==1 , bind(&dg::fillh,_1,10,0,10,bind(&AnaMonojetCandidate::nmuons,_2)) );
  mj_wmunu_sel.add_uu( "nelectrons" , bind(&AnaMonojetCandidate::nelectrons,_1)==0 , bind(&dg::fillh,_1,10,0,10,bind(&AnaMonojetCandidate::nelectrons,_2)) );
  mj_wmunu_sel.add_uu( "muon_pt" , bind(&AnaMonojetCandidate::leading_muon_pt,_1) > 25 , bind(&dg::fillh,_1,100,0,250,bind(&AnaMonojetCandidate::leading_muon_pt,_2)) );
  mj_wmunu_sel.add_uu( "w_mt" , bind(&AnaMonojetCandidate::w_mt,_1) > 40 , bind(&dg::fillh,_1,100,0,250,bind(&AnaMonojetCandidate::w_mt,_2)) );

  MJCUT mj_wenu_sel( "mj_wenu_sel" );
  mj_wenu_sel.based_on( mj_cr_common_sel );
  mj_wenu_sel.add_uu( "trigger" , bind(&AnaMonojetCandidate::el_trigger,_1)==true , bind(&dg::filleff,_1,bind(&AnaMonojetCandidate::el_trigger,_2)) );
  mj_wenu_sel.add_uu( "nmuons" , bind(&AnaMonojetCandidate::nmuons,_1)==0 , bind(&dg::fillh,_1,10,0,10,bind(&AnaMonojetCandidate::nmuons,_2)) );
  mj_wenu_sel.add_uu( "nelectrons" , bind(&AnaMonojetCandidate::nelectrons,_1)==1 , bind(&dg::fillh,_1,10,0,10,bind(&AnaMonojetCandidate::nelectrons,_2)) );
  mj_wenu_sel.add_uu( "electron_pt" , bind(&AnaMonojetCandidate::leading_electron_pt,_1) > 25 , bind(&dg::fillh,_1,100,0,250,bind(&AnaMonojetCandidate::leading_electron_pt,_2)) );
  mj_wenu_sel.add_uu( "w_mt" , bind(&AnaMonojetCandidate::w_mt,_1) > 40 , bind(&dg::fillh,_1,100,0,250,bind(&AnaMonojetCandidate::w_mt,_2)) );

  // Initialize Study class
  // ================================================================
  StudyMonojet stmj( "" , "" , 200842 , 215643 );
  // control regions
  // W control regions
  /*
  stmj.add_category( "control_wmunu_j60" , 
		     bind(&AnaMonojetCandidate::cr_wmunu,_1) && 
		     bind(&AnaMonojetCandidate::first_jet_pt,_1) > 60. && bind(&AnaMonojetCandidate::first_jet_pt,_1) < 120. );
  stmj.add_category( "control_wmunu_j120" , 
		     bind(&AnaMonojetCandidate::cr_wmunu,_1) &&
		     bind(&AnaMonojetCandidate::first_jet_pt,_1) > 120. && bind(&AnaMonojetCandidate::first_jet_pt,_1) < 220. );
  */
  stmj.add_category( "control_wmunu_j120inc" , 
		     bind(&AnaMonojetCandidate::cr_wmunu,_1) &&
		     bind(&AnaMonojetCandidate::first_jet_pt,_1) > 120. );
  stmj.add_category( "control_wmunu_j220inc" , 
		     bind(&AnaMonojetCandidate::cr_wmunu,_1) &&
		     bind(&AnaMonojetCandidate::first_jet_pt,_1) > 220. );
  /*
  stmj.add_category( "control_wmunu_j60_lm60" , 
		     bind(&AnaMonojetCandidate::cr_wmunu,_1) && 
		     bind(&AnaMonojetCandidate::first_jet_pt,_1) > 60. && bind(&AnaMonojetCandidate::first_jet_pt,_1) < 120. &&
		     bind(&AnaMonojetCandidate::lepmet_pt,_1) > 60. && bind(&AnaMonojetCandidate::lepmet_pt,_1) < 120. );
  stmj.add_category( "control_wmunu_j120_lm120" , 
		     bind(&AnaMonojetCandidate::cr_wmunu,_1) &&
		     bind(&AnaMonojetCandidate::first_jet_pt,_1) > 120. && bind(&AnaMonojetCandidate::first_jet_pt,_1) < 220. &&
		     bind(&AnaMonojetCandidate::lepmet_pt,_1) > 120. && bind(&AnaMonojetCandidate::lepmet_pt,_1) < 220. );
  */
  stmj.add_category( "control_wmunu_j120inc_lm120inc" , 
		     bind(&AnaMonojetCandidate::cr_wmunu,_1) &&
		     bind(&AnaMonojetCandidate::first_jet_pt,_1) > 120. &&
		     bind(&AnaMonojetCandidate::lepmet_pt,_1) > 120. );
  stmj.add_category( "control_wmunu_j220inc_lm220inc" , 
		     bind(&AnaMonojetCandidate::cr_wmunu,_1) &&
		     bind(&AnaMonojetCandidate::first_jet_pt,_1) > 220. &&
		     bind(&AnaMonojetCandidate::lepmet_pt,_1) > 220. );
  /*
  stmj.add_category( "control_wmunu_j60_m120inc" , 
		     bind(&AnaMonojetCandidate::cr_wmunu,_1) && 
		     bind(&AnaMonojetCandidate::first_jet_pt,_1) > 60. && bind(&AnaMonojetCandidate::first_jet_pt,_1) < 120. &&
		     bind(&AnaMonojetCandidate::met_pt,_1) > 120. );
  stmj.add_category( "control_wmunu_j120_m120inc" , 
		     bind(&AnaMonojetCandidate::cr_wmunu,_1) &&
		     bind(&AnaMonojetCandidate::first_jet_pt,_1) > 120. && bind(&AnaMonojetCandidate::first_jet_pt,_1) < 220. &&
		     bind(&AnaMonojetCandidate::met_pt,_1) > 120. );
  */
  stmj.add_category( "control_wmunu_j120inc_m120inc" , 
		     bind(&AnaMonojetCandidate::cr_wmunu,_1) &&
		     bind(&AnaMonojetCandidate::first_jet_pt,_1) > 120. &&
		     bind(&AnaMonojetCandidate::met_pt,_1) > 120. );
  stmj.add_category( "control_wmunu_j220inc_m220inc" , 
		     bind(&AnaMonojetCandidate::cr_wmunu,_1) &&
		     bind(&AnaMonojetCandidate::first_jet_pt,_1) > 220. &&
		     bind(&AnaMonojetCandidate::met_pt,_1) > 120. );
  // Z control regions
  /*
  stmj.add_category( "control_zmumu_j60" , 
		     bind(&AnaMonojetCandidate::cr_zmumu,_1) && 
		     bind(&AnaMonojetCandidate::first_jet_pt,_1) > 60. && bind(&AnaMonojetCandidate::first_jet_pt,_1) < 120. );
  stmj.add_category( "control_zmumu_j120" , 
		     bind(&AnaMonojetCandidate::cr_zmumu,_1) &&
		     bind(&AnaMonojetCandidate::first_jet_pt,_1) > 120. && bind(&AnaMonojetCandidate::first_jet_pt,_1) < 220. );
  */
  stmj.add_category( "control_zmumu_j120inc" , 
		     bind(&AnaMonojetCandidate::cr_zmumu,_1) &&
		     bind(&AnaMonojetCandidate::first_jet_pt,_1) > 120. );
  stmj.add_category( "control_zmumu_j220inc" , 
		     bind(&AnaMonojetCandidate::cr_zmumu,_1) &&
		     bind(&AnaMonojetCandidate::first_jet_pt,_1) > 220. );
  /*
  stmj.add_category( "control_zmumu_j60_lm60" , 
		     bind(&AnaMonojetCandidate::cr_zmumu,_1) && 
		     bind(&AnaMonojetCandidate::first_jet_pt,_1) > 60. && bind(&AnaMonojetCandidate::first_jet_pt,_1) < 120. &&
		     bind(&AnaMonojetCandidate::lepmet_pt,_1) > 60. && bind(&AnaMonojetCandidate::lepmet_pt,_1) < 120. );
  stmj.add_category( "control_zmumu_j120_lm120" , 
		     bind(&AnaMonojetCandidate::cr_zmumu,_1) &&
		     bind(&AnaMonojetCandidate::first_jet_pt,_1) > 120. && bind(&AnaMonojetCandidate::first_jet_pt,_1) < 220. &&
		     bind(&AnaMonojetCandidate::lepmet_pt,_1) > 120. && bind(&AnaMonojetCandidate::lepmet_pt,_1) < 220. );
  */
  stmj.add_category( "control_zmumu_j120inc_lm120inc" , 
		     bind(&AnaMonojetCandidate::cr_zmumu,_1) &&
		     bind(&AnaMonojetCandidate::first_jet_pt,_1) > 120. &&
		     bind(&AnaMonojetCandidate::lepmet_pt,_1) > 120. );
  stmj.add_category( "control_zmumu_j220inc_lm220inc" , 
		     bind(&AnaMonojetCandidate::cr_zmumu,_1) &&
		     bind(&AnaMonojetCandidate::first_jet_pt,_1) > 220. &&
		     bind(&AnaMonojetCandidate::lepmet_pt,_1) > 220. );
  /*
  stmj.add_category( "control_zmumu_j60_m120inc" , 
		     bind(&AnaMonojetCandidate::cr_zmumu,_1) && 
		     bind(&AnaMonojetCandidate::first_jet_pt,_1) > 60. && bind(&AnaMonojetCandidate::first_jet_pt,_1) < 120. &&
		     bind(&AnaMonojetCandidate::met_pt,_1) > 120. );
  stmj.add_category( "control_zmumu_j120_m120inc" , 
		     bind(&AnaMonojetCandidate::cr_zmumu,_1) &&
		     bind(&AnaMonojetCandidate::first_jet_pt,_1) > 120. && bind(&AnaMonojetCandidate::first_jet_pt,_1) < 220. &&
		     bind(&AnaMonojetCandidate::met_pt,_1) > 120. );
  */
  stmj.add_category( "control_zmumu_j120inc_m120inc" , 
		     bind(&AnaMonojetCandidate::cr_zmumu,_1) &&
		     bind(&AnaMonojetCandidate::first_jet_pt,_1) > 120. &&
		     bind(&AnaMonojetCandidate::met_pt,_1) > 120. );
  stmj.add_category( "control_zmumu_j220inc_m220inc" , 
		     bind(&AnaMonojetCandidate::cr_zmumu,_1) &&
		     bind(&AnaMonojetCandidate::first_jet_pt,_1) > 220. &&
		     bind(&AnaMonojetCandidate::met_pt,_1) > 120. );
  // signal regions
  //stmj.add_category( "signal_common" , bind(&AnaMonojetCandidate::sr_common,_1) );
  stmj.add_category( "signal_presel0" , bind(&AnaMonojetCandidate::sr_presel0,_1) );
  stmj.add_category( "signal_presel1" , bind(&AnaMonojetCandidate::sr_presel1,_1) );
  stmj.add_category( "signal_presel2" , bind(&AnaMonojetCandidate::sr_presel2,_1) );
  stmj.add_category( "signal_120" , bind(&AnaMonojetCandidate::sr_120,_1) );
  stmj.add_category( "signal_220" , bind(&AnaMonojetCandidate::sr_220,_1) );
  stmj.add_category( "signal_350" , bind(&AnaMonojetCandidate::sr_350,_1) );
  stmj.add_category( "signal_500" , bind(&AnaMonojetCandidate::sr_500,_1) );


  // Create all the different RunConfigurations for each different systematic shift, etc
  // ================================================================
  std::vector<RunConfig> mc_run_configs;
  std::cout << "Loading MC run configurations..." << std::endl;
  mc_run_configs.push_back( RunConfig( "raw" , "RAW" , "RAW" , "RAW" , "RAW" , "RAW" , "RAW" , "RAW" , "RAW" ) );
  mc_run_configs.push_back( RunConfig( "nominal" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" ) );
  // configurations for muon smear systematics
  mc_run_configs.push_back( RunConfig( "idup" , "IDUP" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" ) );
  mc_run_configs.push_back( RunConfig( "idlow" , "IDLOW" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" ) );
  mc_run_configs.push_back( RunConfig( "msup" , "MSUP" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" ) );
  mc_run_configs.push_back( RunConfig( "mslow" , "MSLOW" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" ) );
  // configurations for the muon efficiency scale systematic
  // buggy at the moment
  //mc_run_configs.push_back( RunConfig( "mueffup" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "UP" , "NOMINAL" , "NOMINAL" , "NOMINAL" ) );
  //mc_run_configs.push_back( RunConfig( "mueffdown" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "DOWN" , "NOMINAL" , "NOMINAL" , "NOMINAL" ) );
  // configurations for JES systematics
  // the JES is still buggy
  //mc_run_configs.push_back( RunConfig( "jesup" , "NOMINAL" , "NOMINAL" , "JESUP" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" ) );
  //mc_run_configs.push_back( RunConfig( "jesdown" , "NOMINAL" , "NOMINAL" , "JESDOWN" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" ) );
  std::vector<RunConfig> data_run_configs;
  std::cout << "Loading DATA run configurations..." << std::endl;
  data_run_configs.push_back( RunConfig( "raw" , "RAW" , "RAW" , "RAW" , "RAW" , "RAW" , "RAW" , "RAW" , "RAW" ) );
  data_run_configs.push_back( RunConfig( "nominal" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" , "NOMINAL" ) );

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

  // this gets set to PU_MC12A in the event loop when the pw tool is initialized for the first time
  std::string pileup_data_file = "./pileup_histos/ilumicalc_histograms_EF_xe80_tclcw_200842-215643.root";
  std::string pileup_config_file = "./pileup_histos/mc12a_v1_50.prw.root";
  double lumi_tot;
  std::vector<double> int_lumi;

  // this is meant to list all the run period letters that we're using, but we shouldn't need this
  // for 2012 muon eff scale calculations... only 2011
  std::vector<std::string> run_periods;  

  bool report_good_file = true;

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
    const bool is_mc = evt->check_flag("mc")==true;
    const bool is_pgs = evt->check_flag("pgs")==true;
    const bool is_data = !is_mc;

    // get run info
    const unsigned long raw_run_number = evt->run_number();
    const unsigned long mc_channel = evt->mc_channel();
    const unsigned long event_number = evt->event_number();
    const double average_mu = evt->average_mu();
    const unsigned long lumi_block = evt->lumi_block();
    unsigned long run_number = raw_run_number;
    const int bdff = evt->bdff();

    // SET PILEUP WEIGHT
    // =================
    // Currently there is no weight or veto for data

    double pu_weight = 1.;
    double mc_weight = 1.;
    static boost::shared_ptr<Root::TPileupReweighting> pw;
    // we want to initialize the pw object event for data, so we can use it to calculate run_lumi
    if( !pw ) {
      pw.reset( new Root::TPileupReweighting( "pw" ) );
      pw->SetDefaultChannel( 0 );
      pw->UsePeriodConfig( "MC12a" );
      pw->SetUnrepresentedDataAction( 2 ); // just keep the data
      pw->DisableWarnings( true );
      pw->SetDataScaleFactors( AnaConfiguration::pileup_data_scale() );
      pw->AddConfigFile( pileup_config_file );
      pw->AddLumiCalcFile( pileup_data_file );
      pw->Initialize();
      int_lumi = pw->getIntegratedLumiVector();
      lumi_tot = pw->GetIntegratedLumi();
      std::cout << "Initialized TPileupReweighting for MC12A" << std::endl;
      std::cout << "Total lumi = " << lumi_tot << std::endl;
    }

    // for data, we have an option of dropping "unrepresented" data
    if( is_data ) const bool vetoEvent = false;
    // for MC, calculate pileup weight
    else {
      if( is_pgs ) {
	// do nothing
      } else {
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
    }
    const double run_lumi = pw->GetIntegratedLumi( run_number , run_number );
    const double ev_weight = pu_weight * mc_weight;

    if( false ) {
      std::cout << "DEBUG :: pileup weighting :: pu_weight = " << pu_weight << " , mc_weight = " << mc_weight << " , average_mu = " << average_mu << std::endl;
    }

    // EVENT PRESELECTION
    // ==================

    // check that a trigger is present
    const boost::shared_ptr<const AnaTrigger>& trigger = evt->trigger();
    if( true ) {
      dg::down( "presel" , "" );
      dg::filleff( "0_trigger_present" , trigger );
      dg::up();
    }
    if( !trigger && !is_pgs) {
      std::cerr << "WARNING: trigger information not present for run " << raw_run_number << " evt " << event_number << ". skipping..." << std::endl;
      continue;
    }

    // count total events
    dg::fillhw( "total_raw" , 1 , 1 , 2 , 1 , 1. );
    dg::fillhw( "total" , 1 , 1 , 2 , 1 , ev_weight );

    // GRL selection
    const bool is_good_run = is_mc ? true : grl.HasRunLumiBlock( run_number , lumi_block );
    if( true ) {
      dg::down( "presel" , "" );
      dg::filleff( "1_grl_passed" , is_good_run );
      dg::up();
    }
    if( !is_good_run ) { continue; }

    // larError check for data only
    const bool lar_error = is_data && evt->trigger()->error_lar();
    if( true ) {
      dg::down( "presel" , "" );
      dg::filleff( "2_no_larerror" , !lar_error );
      dg::up();
    }
    if( lar_error && !is_pgs) { continue; }

    // vertex selection
    dgSTL::copy_if( evt->vertices().begin() , evt->vertices().end() , back_inserter(vtx_coll) , CUT_ALL(AnaVertex,vtx_sel) );
    if( true ) {
      dg::down( "presel" , "" );
      dg::filleff( "3_vtx_passed" , vtx_coll.size() );
      dg::fillh( "nvtxs" , 20 , 0 , 20 , vtx_coll.size() );
      dg::up();
    }
    if( vtx_coll.size() == 0 && !is_pgs ) { continue; }

    // store some other vertex collections for jet calibrations/systematics
    dgSTL::copy_if( evt->vertices().begin() , evt->vertices().end() , back_inserter(vtx_multijes_coll) , CUT_ALL(AnaVertex,vtx_multijes_sel) );
    dgSTL::copy_if( evt->vertices().begin() , evt->vertices().end() , back_inserter(vtx_jetcalib_coll) , CUT_ALL(AnaVertex,vtx_jetcalib_sel) );

    // trigger check
    //if( !trigger->ef_xe70() && !trigger->ef_mu18_medium() && !trigger->ef_e15_medium() ) { continue; }

    // get truth and track collections for jet studies
    dgSTL::copy_if( evt->truth_particles().begin() , evt->truth_particles().end() , back_inserter(truth_coll) , CUT_ALL(AnaTruthParticle,truth_sel) );
    //dgSTL::copy_if( evt->offline_tracks().begin() , evt->offline_tracks().end() , back_inserter(track_coll) , CUT_ALL(AnaTrack,track_sel) );


    // LOOP OVER ALL RUN CONFIGURATIONS
    // ===============================
    // This is just an "easy" way to process all the systematics at the same time. Basically,
    // we rerun the event for each different systematic shift that we care about
    std::vector<RunConfig> run_configs = is_mc ? mc_run_configs : data_run_configs;
    BOOST_FOREACH( RunConfig config , run_configs ) {
      // first we want to make sure all our object vectors are empty
      el_calib_coll.clear();
      el_coll.clear();
      mu_calib_coll.clear();
      mu_loose_coll.clear();
      mu_medium_coll.clear();
      mu_tight_coll.clear();
      lepjet_calib_coll.clear();
      lepjet_coll.clear();
      badlepjet_coll.clear();
      met_calib_coll.clear();
      met_reffinal_coll.clear();
      met_reffinalcorr_coll.clear();
      met_lochadtopo_coll.clear();
      mj_calib_coll.clear();
      mj_coll.clear();

      // OBJECT CORRECTIONS/CALIBRATIONS (specific to this run config)
      // ===============================

      // preselection smearing and scaling for jets
      // in data we just need to calibrate the jets (using EMJES calibration)


//       size_t ijet = 0;
//       std::vector< boost::shared_ptr<const AnaJet> > jets_partially_calibrated;
//       if(!is_pgs){
//         BOOST_FOREACH( const boost::shared_ptr<const AnaJet>& jet , evt->atlas_anti_kt_4_h1_topo_jets() ) {
//           if( config.jet_calib == "RAW" ) {
//             lepjet_calib_coll.push_back( jet );
//           } else if( is_data ) {
//             lepjet_calib_coll.push_back( jet->jetetmiss_calibrate(conf,jet,event_number,ijet,average_mu,vtx_jetcalib_coll.size(),is_mc,true,false) );
//           } else {
//             jets_partially_calibrated.push_back( jet->jetetmiss_calibrate(conf,jet,event_number,ijet,average_mu,vtx_jetcalib_coll.size(),is_mc,true,true) );
//           }
//           ijet++;
//         }
//       }//!is_pgs
//       else{
//         BOOST_FOREACH( const boost::shared_ptr<const AnaJet>& jet , evt->atlas_anti_kt_4_h1_topo_jets() ) {
//           if( config.jet_calib == "RAW" && !is_pgs) {
//             lepjet_calib_coll.push_back( jet );
//           } else if( is_data ) {
//             lepjet_calib_coll.push_back( jet->jetetmiss_calibrate(conf,jet,event_number,ijet,average_mu,vtx_jetcalib_coll.size(),is_mc,true,false) );
//           } else {
//             jets_partially_calibrated.push_back( jet->jetetmiss_calibrate(conf,jet,event_number,ijet,average_mu,vtx_jetcalib_coll.size(),is_mc,true,true) );
//           }
//           ijet++;
//         }
//       }//!is_pgs
//       if( is_mc && !is_pgs) {
//         // do systematic shifts, started with the collection of partially calibrated jets
//         ijet = 0;
//         BOOST_FOREACH( const boost::shared_ptr<const AnaJet>& jet , jets_partially_calibrated ) {
//           if( config.jet_calib == "NOMINAL" ) {
//             lepjet_calib_coll.push_back( jet );
//           } else {
//             lepjet_calib_coll.push_back( jet->jetetmiss_scale(conf,jet,event_number,1,0.,ijet,average_mu,vtx_coll.size(),config.jet_calib) );
//           }
//           ijet++;
//         }
//       }


      if(1){
        size_t ijet = 0;
        std::vector< boost::shared_ptr<const AnaJet> > jets_partially_calibrated;
        BOOST_FOREACH( const boost::shared_ptr<const AnaJet>& jet , evt->atlas_anti_kt_4_h1_topo_jets() ) {
          if( config.jet_calib == "RAW" ) {
            lepjet_calib_coll.push_back( jet );
          } else if( is_data ) {
            lepjet_calib_coll.push_back( jet->jetetmiss_calibrate(conf,jet,event_number,ijet,average_mu,vtx_jetcalib_coll.size(),is_mc,true,false) );
          } else {
            jets_partially_calibrated.push_back( jet->jetetmiss_calibrate(conf,jet,event_number,ijet,average_mu,vtx_jetcalib_coll.size(),is_mc,true,true) );
          }
          ijet++;
        }
        if( is_mc && config.jet_calib != "RAW" ) {
          // do systematic shifts, started with the collection of partially calibrated jets
          ijet = 0;
          BOOST_FOREACH( const boost::shared_ptr<const AnaJet>& jet , jets_partially_calibrated ) {
            if( config.jet_calib == "NOMINAL" ) {
	      lepjet_calib_coll.push_back( jet );
            } else {
              lepjet_calib_coll.push_back( jet->jetetmiss_scale(conf,jet,event_number,1,0.,ijet,average_mu,vtx_coll.size(),config.jet_calib) );
            }
            ijet++;
          }
        }
      }
      
     
      if(is_pgs){
        size_t ijet = 0;
        std::vector< boost::shared_ptr<const AnaJet> > jets_partially_calibrated;
        BOOST_FOREACH( const boost::shared_ptr<const AnaJet>& jet , evt->pgs_jets()) { 
            lepjet_calib_coll.push_back( jet );
          if( config.jet_calib == "RAW" && !is_pgs ) {
            lepjet_calib_coll.push_back( jet );
          } else if( is_data ) {
            lepjet_calib_coll.push_back( jet->jetetmiss_calibrate(conf,jet,event_number,ijet,average_mu,vtx_jetcalib_coll.size(),is_mc,true,false) );
          } else {
            jets_partially_calibrated.push_back( jet->jetetmiss_calibrate(conf,jet,event_number,ijet,average_mu,vtx_jetcalib_coll.size(),is_mc,true,true) );
          }
          ijet++;
        }
        if( is_mc && !is_pgs) {
          // do systematic shifts, started with the collection of partially calibrated jets
          ijet = 0;
          BOOST_FOREACH( const boost::shared_ptr<const AnaJet>& jet , jets_partially_calibrated ) {
            if( config.jet_calib == "NOMINAL" ) {
              lepjet_calib_coll.push_back( jet );
            } else {
              lepjet_calib_coll.push_back( jet->jetetmiss_scale(conf,jet,event_number,1,0.,ijet,average_mu,vtx_coll.size(),config.jet_calib) );
            }
            ijet++;
          }
        }
      }
      

      // preselection energy corrections for electrons in data
      // I think these are done based on 7 TeV Z mass peak studies, so they are most likely out-of-date
      size_t iel = 0;
      BOOST_FOREACH( const boost::shared_ptr<const AnaElectron>& el , evt->electrons() ) {
	if( config.electron_calib == "RAW" ) {
	  el_calib_coll.push_back( el );
	} else if( is_data ) {
          el_calib_coll.push_back( el->egamma_corrected( conf , el , event_number , iel , "NOMINAL" ) );
        } else {
          if( fabs(el->cluster_eta()) > 999999. ) continue; // this is here to prevent an annoying run time crash when electron eT ~= 0
          el_calib_coll.push_back( el->egamma_smeared( conf , el , event_number , iel , config.electron_calib ) );
	}
        iel++;
      }

      // preselection smearing for muons, including systematic shifts...
      // not doing this yet
      size_t imu = 0;
      const int muscale = 3;
      BOOST_FOREACH( const boost::shared_ptr<const AnaMuon>& mu , evt->muons() ) {
	if( config.muon_calib == "RAW" ) {
	  mu_calib_coll.push_back( mu );
	} else if( is_data ) {
          mu_calib_coll.push_back( mu );
        } else {
          detector::MCP_TYPE mu_type = detector::MCP_STACO_COMBINED; // for author==6
	  // NOTE: primary_author==7 corresponds to "Segment-Tagged", but I think this is still the best option for mu_type
          // NOTE: data_range is not actually used here
          mu_calib_coll.push_back( mu->mcp_smeared( conf , AnaConfiguration::data_range() , mu , mu_type , event_number , imu , muscale , config.muon_calib ) );
        }
        imu++;
      }

      // preselection MET reconstruction using METUtility...
      // for now lets not bother with this... it's pretty complicated
      std::copy( evt->missing_et().begin() , evt->missing_et().end() , back_inserter(met_calib_coll) );


      // OBJECT SELECTIONS
      // ===============================

      // study selections
      if( config.name == "nominal" || config.name == "raw" ) {
        // some histograms are made here, but none of the cuts are actually applied yet
        dg::down( "object_sels" , "" );
	dg::down( config.name , "" );
        el_sel.for_each( el_calib_coll.begin() , el_calib_coll.end() );
        mu_loose_sel.for_each( mu_calib_coll.begin() , mu_calib_coll.end() );
	mu_medium_sel.for_each( mu_calib_coll.begin() , mu_calib_coll.end() );
        mu_tight_sel.for_each( mu_calib_coll.begin() , mu_calib_coll.end() );
        jet_sel.for_each( lepjet_calib_coll.begin() , lepjet_calib_coll.end() );
        met_lochadtopo_sel.for_each( met_calib_coll.begin() , met_calib_coll.end() );
        if( config.name == "raw" ) track_sel.for_each( evt->offline_tracks().begin() , evt->offline_tracks().end() );
        dg::up(); // config name
        dg::up(); // object_sels
      }

      if(is_pgs){
        std::copy( evt->missing_et().begin() , evt->missing_et().end() , back_inserter(met_lochadtopo_coll) );
        std::copy( evt->missing_et().begin() , evt->missing_et().end() , back_inserter(met_reffinal_coll) );
	std::copy( evt->missing_et().begin() , evt->missing_et().end() , back_inserter(met_reffinalcorr_coll) );
      }
      

      // apply all the individual object selections defined in the cut sets at the top of this file
      dgSTL::copy_if( el_calib_coll.begin() , el_calib_coll.end() , back_inserter(el_coll) , CUT_ALL(AnaElectron,el_sel) );
      dgSTL::copy_if( mu_calib_coll.begin() , mu_calib_coll.end() , back_inserter(mu_loose_coll) , CUT_ALL(AnaMuon,mu_loose_sel) );
      dgSTL::copy_if( mu_loose_coll.begin() , mu_loose_coll.end() , back_inserter(mu_medium_coll) , CUT_ALL(AnaMuon,mu_medium_sel) );
      dgSTL::copy_if( mu_medium_coll.begin() , mu_medium_coll.end() , back_inserter(mu_tight_coll) , CUT_ALL(AnaMuon,mu_tight_sel) );
      dgSTL::copy_if( lepjet_calib_coll.begin() , lepjet_calib_coll.end() , back_inserter(lepjet_coll) , CUT_ALL(AnaJet,jet_sel) );
      //      dgSTL::copy_if( lepjet_calib_coll.begin() , lepjet_calib_coll.end() , back_inserter(badlepjet_coll) , CUT_ALL(AnaJet,jet_bad_sel) );
      dgSTL::copy_if( met_calib_coll.begin() , met_calib_coll.end() , back_inserter(met_reffinal_coll) , CUT_ALL(AnaMET,met_reffinal_sel) );
      dgSTL::copy_if( met_calib_coll.begin() , met_calib_coll.end() , back_inserter(met_reffinalcorr_coll) , CUT_ALL(AnaMET,met_reffinalcorr_sel) );
      dgSTL::copy_if( met_calib_coll.begin() , met_calib_coll.end() , back_inserter(met_lochadtopo_coll) , CUT_ALL(AnaMET,met_lochadtopo_sel) );

      
      
      // FINAL EVENT SELECTIONS
      // ======================

      // remove events if not exactly one MET
      if( config.name == "nominal" ) {
	dg::down( "postsel" , "" );
	dg::down( "object_sels" , "" );
	dg::fillh( "0_nmet" , 10 , 0 , 10 , met_lochadtopo_coll.size() );
	dg::fillh( "1_nbadjets" , 10 , 0 , 10 , badlepjet_coll.size() );
	dg::up();
	dg::up();
      }
      if( met_lochadtopo_coll.size()!=1 && !is_pgs) { continue; }

      // we don't bother with any jet electron overlap-removal yet

      // are we supposed to apply to LAr hole veto?... was the LAr hole fixed before 8 TeV runs?
      // not doing this yet


      // build collections with all objects
      boost::shared_ptr<const AnaMonojetCandidate> mj_calib( new AnaMonojetCandidate( el_coll.begin() , el_coll.end() ,
                                                                                      mu_loose_coll.begin() , mu_loose_coll.end() ,
                                                                                      mu_tight_coll.begin() , mu_tight_coll.end() ,
                                                                                      lepjet_coll.begin() , lepjet_coll.end() ,
                                                                                      vtx_coll.begin() , vtx_coll.end() ,
                                                                                      track_coll.begin() , track_coll.end() ,
                                                                                      truth_coll.begin() , truth_coll.end() ,
                                                                                      met_lochadtopo_coll.front() ,
                                                                                      met_reffinal_coll.front() ,
										      met_reffinalcorr_coll.front() ,
                                                                                      trigger , average_mu , bdff, is_pgs , 
										      run_number , run_lumi ) );


      mj_calib_coll.push_back( mj_calib );
      if( config.name == "nominal" ) {
        dg::down( "object_sels" , "" );
        dg::down( config.name , "" );
	mj_sel.for_each( mj_calib_coll.begin() , mj_calib_coll.end() );
        dg::up();
        dg::up();
      }
      // apply base selection
      dgSTL::copy_if( mj_calib_coll.begin() , mj_calib_coll.end() , back_inserter(mj_coll) , CUT_ALL(AnaMonojetCandidate,mj_sel) );
      if( badlepjet_coll.size() ) mj_coll.clear();

      if( config.name == "nominal" ) {
	// all we're doing here is making some cutflows specific for a few sidebands
        dg::down( "object_sels" , "" );
	dg::down( config.name , "" );
	mj_sr_full_sel.for_each( mj_coll.begin() , mj_coll.end() );
        mj_cr_common_sel.for_each( mj_coll.begin() , mj_coll.end() );
        mj_zmumu_sel.for_each( mj_coll.begin() , mj_coll.end() );
        mj_zee_sel.for_each( mj_coll.begin() , mj_coll.end() );
        mj_wmunu_sel.for_each( mj_coll.begin() , mj_coll.end() );
        mj_wenu_sel.for_each( mj_coll.begin() , mj_coll.end() );
	dg::up();
        dg::up();
      }

      // MAKE PLOTS
      // ==========
      // first we need to find the reconstruction/trigger scales
      double mueff_scale = 1. , mueff_stat = 0. , mueff_sys = 0.;
      // MU EFF SCALES ARE BROKEN AT THE MOMENT
      /*
      if( !is_data && config.name != "RAW" ) {
	mu_eff_scale( mu_tight_coll , run_number , int_lumi , run_periods , mueff_scale , mueff_stat , mueff_sys );
	double mueff_err = sqrt( mueff_stat*mueff_stat + mueff_sys*mueff_sys );
	if( config.muon_effscale == "UP" ) mueff_scale += mueff_err;
	else if( config.muon_effscale == "DOWN" ) mueff_scale -= mueff_err;
	// missing other scales
      }
      */
      dg::set_global_weight( ev_weight * mueff_scale ); 

      stmj.repurpose( config.name , "" );
      stmj.for_each( mj_coll.begin() , mj_coll.end() );



      // TRIGGER EFFICIENCY PLOTS
      // ========================
      if( config.name == "nominal" && mj_calib_coll.size()==1 ) {
	// Trigger Selection & Trigger Eff Studies
	// fill some histograms for drawing trigger efficiencies
	std::vector< double > jetptbins;
	jetptbins.push_back( 0. );
	jetptbins.push_back( 30. );
	jetptbins.push_back( 60. );
	jetptbins.push_back( 90. );
	jetptbins.push_back( 120. );
	jetptbins.push_back( 130. );
	jetptbins.push_back( 140. );
	jetptbins.push_back( 150. );
	jetptbins.push_back( 160. );
	if( mj_calib_coll.front()->nelectrons()==0 ) {
	  dg::down( "trigger" , "" );
	  double leading_jet_pt = mj_calib_coll.front()->first_jet_pt();
	  dg::fillhw( "leading_jet_pt" , 1000 , 0 , 1000 , leading_jet_pt , ev_weight );
	  dg::fillhw( "average_mu" , 30 , 0 , 30 , average_mu , ev_weight );
	  dg::fillhw( "bdff" , 100 , 0 , 100 , bdff , ev_weight );
	  std::string jetbinstr = boost::lexical_cast<std::string>(mj_calib_coll.front()->njets());
	  BOOST_FOREACH( double pt_thresh , jetptbins ) {
	    if( leading_jet_pt < pt_thresh ) continue;
	    std::string ptstr = boost::lexical_cast<std::string>(int(pt_thresh));
	    // find average mu bin
	    std::string mustr = boost::lexical_cast<std::string>(average_mu);
	    std::string mubinstr = "undefined";
	    if( average_mu <= 10 ) mubinstr = "low";
	    else if( average_mu <= 20 ) mubinstr = "mid";
	    else mubinstr = "high";
	    // find bunch position (within train) bin
	    std::string bdffstr = boost::lexical_cast<std::string>(bdff);
	    std::string bdffbinstr = "undefined";
	    if( bdff <= 10 ) bdffbinstr = "front";
	    else if( bdff <= 20 ) bdffbinstr = "middle";
	    else bdffbinstr = "back";
	    // find vertex bin
	    std::string vtxstr = boost::lexical_cast<std::string>(vtx_coll.size());
	    std::string vtxbinstr = "undefined";
	    if( vtx_coll.size() <= 5 ) vtxbinstr = "low";
	    else if( vtx_coll.size() <= 10 ) vtxbinstr = "mid";
	    else if( vtx_coll.size() <= 15 ) vtxbinstr = "midhigh";
	    else vtxbinstr = "high";
	    if( mj_calib_coll.front()->tr_mu0() ) {
	      plot_trigger_efficiencies("mu0_ljpt"+ptstr,met_reffinalcorr_coll.front(),met_lochadtopo_coll.front(),trigger,leading_jet_pt,run_number,ev_weight);
	      plot_trigger_efficiencies("mu0_ljpt"+ptstr+"_avgmu"+mubinstr,met_reffinalcorr_coll.front(),met_lochadtopo_coll.front(),trigger,leading_jet_pt,run_number,ev_weight);
	      plot_trigger_efficiencies("mu0_ljpt"+ptstr+"_bd"+bdffbinstr,met_reffinalcorr_coll.front(),met_lochadtopo_coll.front(),trigger,leading_jet_pt,run_number,ev_weight);
	      plot_trigger_efficiencies("mu0_ljpt"+ptstr+"_nvtx"+vtxbinstr,met_reffinalcorr_coll.front(),met_lochadtopo_coll.front(),trigger,leading_jet_pt,run_number,ev_weight);
	      plot_trigger_efficiencies("mu0_ljpt"+ptstr+"_njet"+jetbinstr,met_reffinalcorr_coll.front(),met_lochadtopo_coll.front(),trigger,leading_jet_pt,run_number,ev_weight);
	      if( mj_calib_coll.front()->leading_jet_is_gluon() ) {
		plot_trigger_efficiencies("mu0_ljpt"+ptstr+"_gluonjet",met_reffinalcorr_coll.front(),met_lochadtopo_coll.front(),trigger,leading_jet_pt,run_number,ev_weight);
	      } else if( mj_calib_coll.front()->leading_jet_is_quark() ) {
		plot_trigger_efficiencies("mu0_ljpt"+ptstr+"_quarkjet",met_reffinalcorr_coll.front(),met_lochadtopo_coll.front(),trigger,leading_jet_pt,run_number,ev_weight);
	      }
	    }
	    if( mj_calib_coll.front()->tr_mu1() ) {
	      plot_trigger_efficiencies("mu1_ljpt"+ptstr,met_reffinalcorr_coll.front(),met_lochadtopo_coll.front(),trigger,leading_jet_pt,run_number,ev_weight);
	      plot_trigger_efficiencies("mu1_ljpt"+ptstr+"_avgmu"+mubinstr,met_reffinalcorr_coll.front(),met_lochadtopo_coll.front(),trigger,leading_jet_pt,run_number,ev_weight);
	      plot_trigger_efficiencies("mu1_ljpt"+ptstr+"_bd"+bdffbinstr,met_reffinalcorr_coll.front(),met_lochadtopo_coll.front(),trigger,leading_jet_pt,run_number,ev_weight);
	      plot_trigger_efficiencies("mu1_ljpt"+ptstr+"_nvtx"+vtxbinstr,met_reffinalcorr_coll.front(),met_lochadtopo_coll.front(),trigger,leading_jet_pt,run_number,ev_weight);
	      plot_trigger_efficiencies("mu1_ljpt"+ptstr+"_njet"+jetbinstr,met_reffinalcorr_coll.front(),met_lochadtopo_coll.front(),trigger,leading_jet_pt,run_number,ev_weight);
	    }
	    if( mj_calib_coll.front()->tr_mu2() ) {
	      plot_trigger_efficiencies("mu2_ljpt"+ptstr,met_reffinalcorr_coll.front(),met_lochadtopo_coll.front(),trigger,leading_jet_pt,run_number,ev_weight);
	      plot_trigger_efficiencies("mu2_ljpt"+ptstr+"_avgmu"+mubinstr,met_reffinalcorr_coll.front(),met_lochadtopo_coll.front(),trigger,leading_jet_pt,run_number,ev_weight);
	      plot_trigger_efficiencies("mu2_ljpt"+ptstr+"_bd"+bdffbinstr,met_reffinalcorr_coll.front(),met_lochadtopo_coll.front(),trigger,leading_jet_pt,run_number,ev_weight);
	      plot_trigger_efficiencies("mu2_ljpt"+ptstr+"_nvtx"+vtxbinstr,met_reffinalcorr_coll.front(),met_lochadtopo_coll.front(),trigger,leading_jet_pt,run_number,ev_weight);
	      plot_trigger_efficiencies("mu2_ljpt"+ptstr+"_njet"+jetbinstr,met_reffinalcorr_coll.front(),met_lochadtopo_coll.front(),trigger,leading_jet_pt,run_number,ev_weight);
	    }
	  }
	  dg::up();
	}
      }

      dg::set_global_weight( 1. );

    } // end loop over the different run configurations
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

const double jet_drmin( boost::shared_ptr<const AnaJet>& jet , JETCOL& jetcoll )
{
  if( jetcoll.size()<2 ) return 0.;
  std::sort( jetcoll.begin() , jetcoll.end() , bind(&AnaParticle::dist_eta_phi,_1,jet) < bind(&AnaParticle::dist_eta_phi,_2,jet) );
  return double( jet->dist_eta_phi( jetcoll[1] ) );
}

// MC11a/MC11b/MC11c via https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/WZElectroweakCommonTopics2011
const bool jet_larhole_veto( std::vector< boost::shared_ptr<const AnaJet> >& jetcoll , const bool& is_mc , const unsigned long& run_number )
{
  if( is_mc && (run_number != 183003) ) { return false; }
  if( !is_mc && ( (run_number < 180614) || (run_number >= 185352) ) ) { return false; }
  BOOST_FOREACH( boost::shared_ptr<const AnaJet>& jet , jetcoll ) {
    if( LArHole::simpleVeto( jet->pt() , jet->eta() , jet->phi() , jet->f_BCH_CORR_JET() , jet->f_BCH_CORR_CELL() , 25. ) ) { return true; }
  }
  return false;
}

void mu_eff_scale( MUCOL& mucoll , const unsigned long& run_number , std::vector<double>& int_lumi , std::vector<std::string>& run_periods ,
                   double& eff_weight , double& eff_stat_error , double& eff_sys_error )
{
  eff_weight = 1.;
  eff_stat_error = 0.;
  eff_sys_error = 0.;
  for( std::vector< shared_ptr<const AnaMuon> >::const_iterator imu = mucoll.begin() ; imu != mucoll.end() ; ++imu ) {
    const shared_ptr<const AnaMuon> mu(*imu);
    double wt , stat_err , sys_err;
    AnaMuon::mcp_effscale( AnaConfiguration::conf() , AnaConfiguration::data_range() , mu , mu_type , run_number , int_lumi , run_periods , wt , stat_err , sys_err );
    eff_weight *= wt;
    eff_stat_error += pow(stat_err/wt,2);
    eff_sys_error += pow(sys_err/wt,2);
  }
  eff_stat_error = eff_weight*sqrt(eff_stat_error);
  eff_sys_error = eff_weight*sqrt(eff_sys_error);
}

void el_eff_scale( std::vector< boost::shared_ptr<const AnaElectron> >& elcoll , float &eff , float& error )
{
  eff = 1.;
  error = 0.;
  for( std::vector< shared_ptr<const AnaElectron> >::const_iterator iel = elcoll.begin() ; iel != elcoll.end() ; ++iel ) {
    const shared_ptr<const AnaElectron> el(*iel);
    float wt , err;
    AnaElectron::egamma_effscale( AnaConfiguration::conf() , el , wt , err );
    eff *= wt;
    error += pow(err/wt,2);
  }
  error = sqrt(error);
}

void mu_trig_scale( MUCOL& mucoll , const detector::EGAMMA_TYPE& el_type , const unsigned long& run_number ,
                    double& trig_weight , double& trig_stat_error )
{
  std::vector<TLorentzVector> velectrons; // should be empty
  std::vector<TLorentzVector> vmuons;
  for( std::vector< shared_ptr<const AnaMuon> >::const_iterator imu = mucoll.begin() ; imu != mucoll.end() ; ++imu ) {
    vmuons.push_back( (*imu)->four_vector() );
  }
  AnaMuon::GetTriggerSF_v17( AnaConfiguration::conf() , mu_type , el_type , vmuons , velectrons , run_number , 0 , trig_weight , trig_stat_error );
}

void el_trig_scale( std::vector< boost::shared_ptr<const AnaElectron> >& elcoll , const detector::EGAMMA_TYPE& el_type , const unsigned long& run_number ,
                    double& trig_weight , double& trig_stat_error )
{
  std::vector<TLorentzVector> velectrons;
  std::vector<TLorentzVector> vmuons; // should be empty
  for( std::vector< shared_ptr<const AnaElectron> >::const_iterator iel = elcoll.begin() ; iel != elcoll.end() ; ++iel ) {
    TLorentzVector tmp_vec;
    tmp_vec.SetPtEtaPhiE( (*iel)->pt() , (*iel)->cluster_eta() , (*iel)->cluster_phi() , (*iel)->energy() ); // use some cluster vars
    velectrons.push_back( tmp_vec );
  }
  AnaMuon::GetTriggerSF_v17( AnaConfiguration::conf() , mu_type , el_type , vmuons , velectrons , run_number , 0 , trig_weight , trig_stat_error );
}



void plot_trigger_efficiencies( const std::string& name , const boost::shared_ptr<const AnaMET>& reffinal , const boost::shared_ptr<const AnaMET>& lochadtopo , const boost::shared_ptr<const AnaTrigger>& trigger , const double& leading_jet_pt , const unsigned long& run_number , const double& ev_weight )
{
  if( !reffinal ) return;
  if( !lochadtopo ) return;
  if( !trigger ) return;
  if( name.size()==0 ) return;

  // put together list of all met objects
  typedef std::pair< boost::shared_ptr<const AnaMET> , std::string > METPair;
  std::vector< METPair > metlist;
  //metlist.push_back( std::make_pair( reffinal , "rf" ) );
  metlist.push_back( std::make_pair( lochadtopo , "lht" ) );

  // put together list of all triggers that we're interested in
  typedef std::pair< bool , std::string > TriggerPair;
  std::vector< TriggerPair > triggerlist;
  triggerlist.push_back( std::make_pair( trigger->monojet_met(run_number) , "composite" ) );
  triggerlist.push_back( std::make_pair( trigger->ef_xe80() , "xe80" ) );
  triggerlist.push_back( std::make_pair( trigger->ef_xe70() , "xe70" ) );
  triggerlist.push_back( std::make_pair( trigger->ef_xe60() , "xe60" ) );
  triggerlist.push_back( std::make_pair( trigger->ef_xe60t() , "xe60t" ) );
  triggerlist.push_back( std::make_pair( trigger->ef_xe70_tclcw() , "xe70_tclcw" ) );
  triggerlist.push_back( std::make_pair( trigger->ef_xe80_tclcw() , "xe80_tclcw" ) );
  //triggerlist.push_back( std::make_pair( trigger->ef_j110_a4tchad_xe75_tclcw() , "j110_xe75" ) );
  //triggerlist.push_back( std::make_pair( trigger->ef_j80_a4tchad_xe100_tclcw_veryloose() , "j80_xe100" ) );
  triggerlist.push_back( std::make_pair( trigger->ef_xe60_tclcw_loose_delayed() , "xe60_tclcw_loose_delayed" ) );
  triggerlist.push_back( std::make_pair( trigger->ef_xe60t_tclcw_loose_delayed() , "xe60t_tclcw_loose_delayed" ) );

  // make all the standard plots for reffinal and lochadtopo
  BOOST_FOREACH( METPair p , metlist ) {
    dg::down( p.second + "_" + name , "" );
    boost::shared_ptr<const AnaMET> met( p.first );
    assert( met );
    dg::fillh( "met_raw" , 500 , 0 , 500 , met->pt() );
    dg::fillhw( "met" , 500 , 0 , 500 , met->pt() , ev_weight );
    //dg::fillh( "jetpt_raw" , 500 , 0 , 500 , leading_jet_pt );
    //dg::fillhw( "jetpt" , 500 , 0 , 500 , leading_jet_pt , ev_weight );
    dg::fillh( "metxs_raw" , 500 , 0 , 100 , met->xs() );
    dg::fillhw( "metxs" , 500 , 0 , 100 , met->xs() , ev_weight );
    BOOST_FOREACH( TriggerPair t , triggerlist ) {
      if( t.first ) {
        dg::fillh( "met_"+t.second+"_raw" , 500 , 0 , 500 , met->pt() );
        dg::fillhw( "met_"+t.second , 500 , 0 , 500 , met->pt() , ev_weight );
        //dg::fillh( "jetpt_"+t.second+"_raw" , 500 , 0 , 500 , leading_jet_pt );
        //dg::fillhw( "jetpt_"+t.second , 500 , 0 , 500 , leading_jet_pt , ev_weight );
	dg::fillh( "metxs_"+t.second+"_raw" , 500 , 0 , 100 , met->xs() );
	dg::fillhw( "metxs_"+t.second , 500 , 0 , 100 , met->xs() , ev_weight );
      }
    }
    dg::up();
  }

  // make (lochadtopo - reffinal) plots
  /*
  dg::down( "diff_" + name , "" );
  dg::fillh( "met_raw" , 500 , -250 , 250 , lochadtopo->pt() - reffinal->pt() );
  dg::fillhw( "met" , 500 , -250 , 250 , lochadtopo->pt() - reffinal->pt() , ev_weight );
  BOOST_FOREACH( TriggerPair t , triggerlist ) {
    if( t.first ) {
      dg::fillh( "met_"+t.second+"_raw" , 500 , -250 , 250 , lochadtopo->pt() - reffinal->pt() );
      dg::fillhw( "met_"+t.second , 500 , -250 , 250 , lochadtopo->pt() - reffinal->pt() , ev_weight );
    }
  }
  dg::up();
  */
}

#ifdef F77_DUMMY_MAIN
#ifdef __cplusplus
extern "C"
#endif
int F77_DUMMY_MAIN() { abort(); return 1; }
#endif

