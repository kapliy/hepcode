
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
#include "TrigFTKAna/AnaParticle.hpp"
#include "TrigFTKAna/AnaTruthParticle.hpp"
#include "TrigFTKAna/AnaTruthTable.hpp"
#include "TrigFTKAna/AnaTrigger.hpp"
#include "TrigFTKAna/AnaWprimeCandidate.hpp"
#include "TrigFTKAna/StudyWprime.hpp"
#include "TrigFTKAna/AnaZCrossSectionCandidate.hpp"
#include "TrigFTKAna/StudyZCrossSection.hpp"
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
#include "egammaAnalysisUtils/EisoTool.h"
#include "JetTagAlgorithms/MV1.h"

using namespace std;
using namespace DataGraphics;
using namespace boost;

#define boost_foreach BOOST_FOREACH

template< typename lepIterT , typename metIterT , typename jetIterT , typename vtxIterT , typename outputIterT >
void build_w_candidates( lepIterT lep_begin , lepIterT lep_end ,
                         metIterT met_begin , metIterT met_end ,
                         jetIterT jet_begin , jetIterT jet_end ,
			 jetIterT bjet_begin , jetIterT bjet_end ,
			 vtxIterT vtx_begin , vtxIterT vtx_end ,
                         outputIterT output_iterator );

template< typename lepIterT , typename metIterT , typename jetIterT , typename vtxIterT , typename outputIterT >
void build_z_candidates( lepIterT lep_begin , lepIterT lep_end ,
                         metIterT met_begin , metIterT met_end ,
                         jetIterT jet_begin , jetIterT jet_end ,
                         vtxIterT vtx_begin , vtxIterT vtx_end ,
                         outputIterT output_iterator );

template< typename overlapIterT , typename refIterT , typename outputIterT >
void overlap_removal_loose( overlapIterT overlap_begin , overlapIterT overlap_end ,
                            refIterT ref_begin , refIterT ref_end ,
                            const AnaParticle::ftype& max_overlap_dr ,
                            outputIterT output_iterator );

template< typename overlapIterT , typename refIterT , typename outputIterT >
void overlap_removal_tight( overlapIterT overlap_begin , overlapIterT overlap_end ,
                            refIterT ref_begin , refIterT ref_end ,
                            const AnaParticle::ftype& max_overlap_dr ,
                            outputIterT output_iterator );

const bool jet_larhole_veto( std::vector< boost::shared_ptr<const AnaJet> >& jetcoll , const bool& is_mc , const unsigned long& run_number );

const double jet_drmin( boost::shared_ptr<const AnaJet>& jet , boost::shared_ptr<const AnaJet>& jetcoll );

void el_eff_scale( EventObjectColl< boost::shared_ptr<const AnaElectron> >& elcoll , float &eff , float& error );

void mu_eff_scale( EventObjectColl< boost::shared_ptr<const AnaMuon> >& mucoll , const unsigned long& run_number , std::vector<double>& int_lumi , std::vector<std::string>& run_periods , double& eff_weight , double& eff_stat_error , double& eff_sys_error );

void mu_trig_scale( EventObjectColl< boost::shared_ptr<const AnaMuon> >& mucoll , const detector::EGAMMA_TYPE& el_type , const unsigned long& run_number , double& trig_weight , double& trig_stat_error );

void el_trig_scale( EventObjectColl< boost::shared_ptr<const AnaElectron> >& elcoll , const detector::EGAMMA_TYPE& el_type , const unsigned long& run_number , double& trig_weight , double& trig_stat_error );

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

// random constants
const detector::MCP_TYPE mu_type = detector::MCP_MUID_COMBINED;
const detector::EGAMMA_TYPE el_type = detector::EGAMMA_TIGHTPP;

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
  grl.add( GoodRunList::ELECTRON , "good_run_lists/data11_7TeV.periodAllYear_DetStatus-v36-pro10_CoolRunQuery-00-04-08_Top_allchannels.xml" );
  grl.add( GoodRunList::MUON , "good_run_lists/data11_7TeV.periodAllYear_DetStatus-v36-pro10_CoolRunQuery-00-04-08_Top_allchannels.xml" );
  // grl.print( GoodRunList::ELECTRON , std::cout );
  // grl.print( GoodRunList::MUON , std::cout );

  // toggle for creating plots to study selection
  const bool studySelections = true;

  // define primary vertex selections
  // ================================================================
  EventObjectColl<  boost::shared_ptr<const AnaVertex>  > vtx_jetcorrection;
  EventObjectColl<  boost::shared_ptr<const AnaVertex>  > vtx_jetCal;
  EventObjectColl<  boost::shared_ptr<const AnaVertex>  > vtx_baseline;

  // VRTX CUT FOR JET CORRECTIONS
  DgCutSet<  boost::shared_ptr<const AnaVertex>  > vtx_jetcorrection_sel( "vtx_jetcorrection_sel" );
  vtx_jetcorrection_sel.add_uu( "nTracks", bind(&AnaVertex::ntracks,_1)>=5, 
                                bind( &dg::fillh , _1 , 15 , 0 , 15 , bind(&AnaVertex::ntracks,_2)) );

  // VRTX CUT FOR JET CALIBRATION TOOLS
  DgCutSet<  boost::shared_ptr<const AnaVertex>  > vtx_jetCal_sel( "vtx_jetCal_sel" );
  vtx_jetCal_sel.add_uu( "nTracks", bind(&AnaVertex::ntracks,_1)>=2,
			 bind( &dg::fillh , _1 , 15 , 0 , 15 , bind(&AnaVertex::ntracks,_2)) );

  // COMMON VERTEX CUTS
  DgCutSet<  boost::shared_ptr<const AnaVertex>  > vtx_sel( "vtx_sel" );
  vtx_sel.add_uu( "nTracks", bind(&AnaVertex::ntracks,_1)>4, 
		  bind( &dg::fillh , _1 , 15 , 0 , 15 , bind(&AnaVertex::ntracks,_2)) );
  vtx_sel.add_uu( "z_vtx", bind(std::ptr_fun<AnaVertex::ftype,AnaVertex::ftype>(std::abs), bind(&AnaVertex::z,_1))<200., //mm
		  bind( &dg::fillh , _1 , 90 , -300 , 300 , bind(&AnaVertex::z,_2) ) );


  // define electron selections
  // ================================================================

  EventObjectColl<  boost::shared_ptr<const AnaElectron>  > el_trigger;
  EventObjectColl<  boost::shared_ptr<const AnaElectron>  > el_corrected; // data
  EventObjectColl<  boost::shared_ptr<const AnaElectron>  > el_trigger_smeared;
  EventObjectColl<  boost::shared_ptr<const AnaElectron>  > el_trigger_smeareddown;
  EventObjectColl<  boost::shared_ptr<const AnaElectron>  > el_trigger_smearedup;
  EventObjectColl<  boost::shared_ptr<const AnaElectron>  > el_iso;
  EventObjectColl<  boost::shared_ptr<const AnaElectron>  > el_iso_smeared;
  EventObjectColl<  boost::shared_ptr<const AnaElectron>  > el_iso_smearedup;
  EventObjectColl<  boost::shared_ptr<const AnaElectron>  > el_iso_smeareddown;
  EventObjectColl<  boost::shared_ptr<const AnaElectron>  > el_baseline;
  EventObjectColl<  boost::shared_ptr<const AnaElectron>  > el_baseline_smeared;
  EventObjectColl<  boost::shared_ptr<const AnaElectron>  > el_baseline_smeareddown;
  EventObjectColl<  boost::shared_ptr<const AnaElectron>  > el_baseline_smearedup;
  EventObjectColl<  boost::shared_ptr<const AnaElectron>  > el_qcd;

  // COMMON ELECTRON CUTS
  DgCutSet<  boost::shared_ptr<const AnaElectron>  > el_common_sel( "el_common_sel" ); 
  el_common_sel.add_uu( "author" ,
                        bind(&AnaElectron::author,_1)==1 || bind(&AnaElectron::author,_1)==3 ,
                        bind( &dg::fillh , _1 , 10 , 0 , 10 , bind(&AnaElectron::author,_2) ) );
  el_common_sel.add_uu( "is_good_oq" , bind(&AnaElectron::is_good_oq,_1)==1 , bind(&dg::filleff,_1,bind(&AnaElectron::is_good_oq,_2)==1) );
  el_common_sel.add_uu( "cluster_ET" ,
                        bind(&AnaElectron::cluster_pt,_1) > 25. ,
                        bind(&dg::fillh,_1,100,0,100,bind(&AnaElectron::cluster_pt,_2)) );
  el_common_sel.add_uu( "eta" ,
                        bind( std::ptr_fun<AnaElectron::ftype,AnaElectron::ftype>(std::abs) , bind(&AnaElectron::eta,_1) ) < 2.47 &&
                        ( bind( std::ptr_fun<AnaElectron::ftype,AnaElectron::ftype>(std::abs) , bind(&AnaElectron::eta,_1) ) < 1.37  ||
                          bind( std::ptr_fun<AnaElectron::ftype,AnaElectron::ftype>(std::abs) , bind(&AnaElectron::eta,_1) ) > 1.52 ) ,
                        bind(&dg::fillh,_1,60,-3,3,bind(&AnaElectron::eta,_2)) );
  
  // RELEASE 17
  DgCutSet<  boost::shared_ptr<const AnaElectron>  > el_sel( "el_sel" );
  el_sel.based_on( el_common_sel );
  // el_sel.add_uu( "tight" , bind(&AnaElectron::is_aod_tight_plusplus,_1) , bind( &dg::filleff,_1,bind(&AnaElectron::is_aod_tight_plusplus,_2)) );
  el_sel.add_uu( "tight" , bind(&AnaElectron::is_ElectronTightPP_v17,_1) , bind( &dg::filleff,_1,bind(&AnaElectron::is_ElectronTightPP_v17,_2)) );

  // RELEASE 17 QCD
  DgCutSet<  boost::shared_ptr<const AnaElectron>  > el_qcd_sel( "el_qcd_sel" );
  el_qcd_sel.based_on( el_common_sel );
  // el_qcd_sel.add_uu( "isolation" , bind(&AnaElectron::et_cone,_1,AnaElectron::CONE20) > 4., bind(&dg::fillh,_1,100,0,50,bind(&AnaElectron::et_cone,_2,AnaElectron::CONE20)) );
  el_qcd_sel.add_uu( "medium_not_tight" ,
		 bind(&AnaElectron::is_aod_medium_plusplus,_1)==true && bind(&AnaElectron::is_aod_tight_plusplus,_1)==false ,
		 bind( &dg::filleff,_1,bind(&AnaElectron::is_aod_medium_plusplus,_2)==true && bind(&AnaElectron::is_aod_tight_plusplus,_2)==false) );


  // define muon selections
  // ================================================================

  EventObjectColl<  boost::shared_ptr<const AnaMuon>  > mu_trigger;
  EventObjectColl<  boost::shared_ptr<const AnaMuon>  > mu_smeared;
  EventObjectColl<  boost::shared_ptr<const AnaMuon>  > mu_msdown;
  EventObjectColl<  boost::shared_ptr<const AnaMuon>  > mu_msup;
  EventObjectColl<  boost::shared_ptr<const AnaMuon>  > mu_iddown;
  EventObjectColl<  boost::shared_ptr<const AnaMuon>  > mu_idup;
  // after overlap removal with jets
  EventObjectColl<  boost::shared_ptr<const AnaMuon>  > mu_trigger_or;
  EventObjectColl<  boost::shared_ptr<const AnaMuon>  > mu_smeared_or;
  EventObjectColl<  boost::shared_ptr<const AnaMuon>  > mu_msdown_or;
  EventObjectColl<  boost::shared_ptr<const AnaMuon>  > mu_msup_or;
  EventObjectColl<  boost::shared_ptr<const AnaMuon>  > mu_iddown_or;
  EventObjectColl<  boost::shared_ptr<const AnaMuon>  > mu_idup_or;
  EventObjectColl<  boost::shared_ptr<const AnaMuon>  > mu_baseline;
  EventObjectColl<  boost::shared_ptr<const AnaMuon>  > mu_baseline_smeared;
  EventObjectColl<  boost::shared_ptr<const AnaMuon>  > mu_baseline_msdown;
  EventObjectColl<  boost::shared_ptr<const AnaMuon>  > mu_baseline_msup;
  EventObjectColl<  boost::shared_ptr<const AnaMuon>  > mu_baseline_iddown;
  EventObjectColl<  boost::shared_ptr<const AnaMuon>  > mu_baseline_idup;

  EventObjectColl<  boost::shared_ptr<const AnaMuon>  > mu_loose; // for Zmumu tag-and-probe study
  EventObjectColl<  boost::shared_ptr<const AnaMuon>  > mu_qcd; // for QCD
  EventObjectColl<  boost::shared_ptr<const AnaMuon>  > mu_smeared_qcd; // for QCD


  DgCutSet<  boost::shared_ptr<const AnaMuon>  > mu_common_sel( "mu_common_sel" );
  mu_common_sel.add_uu( "muid" , bind(&AnaMuon::is_muid,_1) ==true , bind(&dg::filleff,_1,_2) );
  mu_common_sel.add_uu( "is_combined" , bind(&AnaMuon::is_combined,_1) == 1 ,
                        bind( &dg::fillh , _1 , 10 , 0 , 10 , bind(&AnaMuon::is_combined,_2) ) );
  mu_common_sel.add_uu( "author" , bind(&AnaMuon::primary_author,_1)==12 , bind( &dg::fillh , _1 , 10 , 0 , 10 , bind(&AnaMuon::primary_author,_2) ) );
  mu_common_sel.add_uu( "mcp_idhits" , bind(&AnaMuon::passes_hits_mcp,_1,conf)==true, bind(&dg::fillh,_1,50,0,50,bind(&AnaMuon::nhits_silicon,_2)) );
  mu_common_sel.add_uu( "z0" , bind(dgSTL::absF,bind(&AnaMuon::z0_vx,_1)) < 10. , bind(&dg::fillh,_1,100,-700,700,bind(&AnaMuon::z0_exPV,_2)) );
  mu_common_sel.add_uu( "d0" , bind(dgSTL::absF,bind(&AnaMuon::d0_vx_signif,_1)) < 10. , bind(&dg::fillh,_1,100,-50,50,bind(&AnaMuon::d0_exPV_signif,_2)) );
  mu_common_sel.add_uu( "pt" , bind(&AnaMuon::pt,_1) > 20. , bind(&dg::fillh,_1,100,0,100,bind(&AnaMuon::pt,_2)) );
  mu_common_sel.add_uu( "eta" , bind( std::ptr_fun<AnaMuon::ftype,AnaMuon::ftype>(std::abs) , bind(&AnaMuon::eta,_1)) < 2.5 ,
                        bind(&dg::fillh,_1,100,-5,5,bind(&AnaMuon::eta,_2)) );
  //mu_common_sel.add_u( "ptDiffRatio", bind( std::ptr_fun<AnaMuon::ftype,AnaMuon::ftype>(std::abs) , bind(&AnaMuon::pt_diff_ratio,_1)) < 0.5 );

  // muon events
  DgCutSet<  boost::shared_ptr<const AnaMuon>  > mu_sel( "mu_sel" );
  mu_sel.based_on( mu_common_sel );
  mu_sel.add_uu( "pt_cone_30" ,
		       bind(&AnaMuon::pt_cone,_1,AnaMuon::CONE30) < 2.5 ,
		       bind(&dg::fillh,_1,100,0,50,bind(&AnaMuon::pt_cone,_2,AnaMuon::CONE30)) );
  mu_sel.add_uu( "et_cone_20" ,
		       bind(&AnaMuon::et_cone,_1,AnaMuon::CONE20) < 4.0 ,
		       bind(&dg::fillh,_1,100,0,50,bind(&AnaMuon::et_cone,_2,AnaMuon::CONE20)) );

  // QCD
  DgCutSet<  boost::shared_ptr<const AnaMuon>  > mu_qcd_sel( "mu_qcd_sel" );
  mu_qcd_sel.based_on( mu_common_sel );
  mu_qcd_sel.add_uu( "qcd_cone" ,
			   ( bind(&AnaMuon::pt_cone,_1,AnaMuon::CONE30) >= 2.5 ) || ( bind(&AnaMuon::et_cone,_1,AnaMuon::CONE20) >= 4.0 ),
			   bind(&dg::fillh,_1,10,0,10,1) );


  // define jet selections
  // ================================================================

  EventObjectColl<  boost::shared_ptr<const AnaJet>  > lepjet_jetCal;
  EventObjectColl<  boost::shared_ptr<const AnaJet>  > lepjet_JER;
  EventObjectColl<  boost::shared_ptr<const AnaJet>  > lepjet_JESdown;
  EventObjectColl<  boost::shared_ptr<const AnaJet>  > lepjet_JESup;
  EventObjectColl<  boost::shared_ptr<const AnaJet>  > lepjet_baseline;
  EventObjectColl<  boost::shared_ptr<const AnaJet>  > badlepjet_baseline;
  EventObjectColl<  boost::shared_ptr<const AnaJet>  > lepjet_baseline_JER;
  EventObjectColl<  boost::shared_ptr<const AnaJet>  > lepjet_baseline_JESdown;
  EventObjectColl<  boost::shared_ptr<const AnaJet>  > lepjet_baseline_JESup;
  EventObjectColl<  boost::shared_ptr<const AnaJet>  > lepjet_LC;
  EventObjectColl<  boost::shared_ptr<const AnaJet>  > jet_baseline_el;
  EventObjectColl<  boost::shared_ptr<const AnaJet>  > jet_baseline_mu;
  EventObjectColl<  boost::shared_ptr<const AnaJet>  > jet_baseline_JER_el;
  EventObjectColl<  boost::shared_ptr<const AnaJet>  > jet_baseline_JER_mu;
  EventObjectColl<  boost::shared_ptr<const AnaJet>  > jet_baseline_JESup_el;
  EventObjectColl<  boost::shared_ptr<const AnaJet>  > jet_baseline_JESup_mu;
  EventObjectColl<  boost::shared_ptr<const AnaJet>  > jet_baseline_JESdown_el;
  EventObjectColl<  boost::shared_ptr<const AnaJet>  > jet_baseline_JESdown_mu;

  EventObjectColl<  boost::shared_ptr<const AnaJet>  > jet_baseline_mu_qcd;
  EventObjectColl<  boost::shared_ptr<const AnaJet>  > jet_baseline_mu_loose;

  // common jet cuts
  DgCutSet<  boost::shared_ptr<const AnaJet>  > jet_common_sel( "jet_common_sel" );
  jet_common_sel.add_uu( "algorithm_antikt4" , bind(&AnaJet::is_atlas_anti_kt_cone_4_h1_topo,_1)==true , bind(&dg::filleff,_1,bind(&AnaJet::is_atlas_anti_kt_cone_4_h1_topo,_2)==true) );

  // standard selections
  DgCutSet<  boost::shared_ptr<const AnaJet>  > jet_sel( "jet_sel" );
  jet_sel.add_uu( "pt" , bind(&AnaJet::pt,_1) > 25.0 , bind(&dg::fillh,_1,100,0,100,bind(&AnaJet::pt,_2)) );
  jet_sel.add_uu( "eta" , bind( std::ptr_fun<AnaJet::ftype,AnaJet::ftype>(std::abs) , bind(&AnaJet::eta,_1)) < 2.5 , bind(&dg::fillh,_1,100,-3,3,bind(&AnaJet::eta,_2)) );
  jet_sel.add_uu( "JVF", bind(std::ptr_fun<AnaJet::ftype,AnaJet::ftype>(std::abs), bind(&AnaJet::vtx_fraction,_1)) >= 0.75, bind(&dg::fillh,_1,100,-1,1, bind(&AnaJet::vtx_fraction,_2)));
  jet_sel.add_uu( "jet_cleaning" , bind(&AnaJet::isBadJet_looser_v17,_1)==false , bind(&dg::fillh,_1,200,0,200,bind(&AnaJet::pt,_2)) );

  // badjet selection for MET cleaning
  DgCutSet<  boost::shared_ptr<const AnaJet>  > jet_bad_sel( "jet_bad_sel" );
  jet_bad_sel.based_on( jet_common_sel );
  jet_bad_sel.add_uu( "pt" , bind(&AnaJet::pt,_1) > 20. , bind(&dg::fillh,_1,100,0,100,bind(&AnaJet::pt,_2)) );
  jet_bad_sel.add_uu( "bad_looser" , bind(&AnaJet::isBadJet_looser_v17,_1)==true , bind(&dg::fillh,_1,200,0,200,bind(&AnaJet::pt,_2)) );
  // no ugly requirement for now

  // MultijetJES jet selection
  DgCutSet<  boost::shared_ptr<const AnaJet>  >jet_multijes_sel( "jet_multijes_sel" );
  jet_multijes_sel.based_on( jet_common_sel );
  jet_multijes_sel.add_uu( "pt" , bind(&AnaJet::pt,_1) > 7. , bind(&dg::fillh,_1,100,0,100,bind(&AnaJet::pt,_2)) );


  // b-tag
  // ===============================================================
  EventObjectColl<  boost::shared_ptr<const AnaJet>  > jet_baseline_el_btag;   // b-tagged jets from jet_baseline_el collection
  EventObjectColl<  boost::shared_ptr<const AnaJet>  > jet_baseline_mu_btag;   // b-tagged jets from jet_baseline_mu collection

  EventObjectColl<  boost::shared_ptr<const AnaJet>  > jet_baseline_mu_qcd_btag;
  EventObjectColl<  boost::shared_ptr<const AnaJet>  > jet_baseline_mu_loose_btag;

  DgCutSet<  boost::shared_ptr<const AnaJet>  > jet_baseline_btag_sel("jet_baseline_btag");
  // jet_baseline_btag_sel.add_uu( "btag" , bind(&mv1Eval,_1, AnaJet::tag_ip3d_w, AnaJet::tag_sv1_w, AnaJet::tag_cmbnn_w, AnaJet::pt, AnaJet::eta ) > 0.601713 ,
  //				bind(&dg::fillh,_1,100,0,2,bind(&mv1Eval( &AnaJet::tag_ip3d_w , &AnaJet::tag_sv1_w , &AnaJet::tag_cmbnn_w , &AnaJet::pt , &AnaJet::eta )) ) );
  // jet_baseline_btag_sel.add_uu( "btag" , bind(&AnaJet::tag_sv0_w,_1) > 5.85, bind(&dg::fillh,_1,100,-100.0,100.0, bind(&AnaJet::tag_sv0_w,_2) ) );
  
  /*  DgCutSet<  boost::shared_ptr<const AnaJet>  > jet_baseline_bfromt_sel("jet_baseline_bfromt");  // find the b jet that has phi value closer to lepton's
      jet_baseline_bfromt_sel.add_uu( "b from t" ,
      detector::delta_eta_phi(bind(&AnaJet::eta,_1),bind(&AnaJet::phi,_1),bind(&AnaElectron::eta,_1),bind(&AnaElectron::phi,_1))) // phi is called phi? how to add electron data to it? how to ensure that the electron is the selected one? */

  // MET selection
  // ================================================================
  EventObjectColl<  boost::shared_ptr<const AnaMET>  > met_reffinal_uncorrected;
  EventObjectColl<  boost::shared_ptr<const AnaMET>  > met_reffinal;

  // REFFINAL
  DgCutSet<  boost::shared_ptr<const AnaMET>  > met_rf_sel( "met_rf_sel" );
  met_rf_sel.add_u( "met_type" , bind(&AnaMET::met_type,_1) == AnaMET::REF_FINAL );

  // define W selections
  // ================================================================

  // W events
  EventObjectColl<  boost::shared_ptr<const AnaWprimeCandidate>  > wall_muon;
  EventObjectColl<  boost::shared_ptr<const AnaWprimeCandidate>  > wall_muon_smeared;
  EventObjectColl<  boost::shared_ptr<const AnaWprimeCandidate>  > wall_muon_smeared_JER;
  EventObjectColl<  boost::shared_ptr<const AnaWprimeCandidate>  > wall_muon_smeared_JESup;
  EventObjectColl<  boost::shared_ptr<const AnaWprimeCandidate>  > wall_muon_smeared_JESdown;
  EventObjectColl<  boost::shared_ptr<const AnaWprimeCandidate>  > wall_muon_msdown;
  EventObjectColl<  boost::shared_ptr<const AnaWprimeCandidate>  > wall_muon_msup;
  EventObjectColl<  boost::shared_ptr<const AnaWprimeCandidate>  > wall_muon_iddown;
  EventObjectColl<  boost::shared_ptr<const AnaWprimeCandidate>  > wall_muon_idup;
  EventObjectColl<  boost::shared_ptr<const AnaWprimeCandidate>  > wall_electron;
  EventObjectColl<  boost::shared_ptr<const AnaWprimeCandidate>  > wall_electron_smeared;
  EventObjectColl<  boost::shared_ptr<const AnaWprimeCandidate>  > wall_electron_smearedup;
  EventObjectColl<  boost::shared_ptr<const AnaWprimeCandidate>  > wall_electron_smeareddown;
  EventObjectColl<  boost::shared_ptr<const AnaWprimeCandidate>  > wall_electron_smeared_JER;
  EventObjectColl<  boost::shared_ptr<const AnaWprimeCandidate>  > wall_electron_smeared_JESdown;
  EventObjectColl<  boost::shared_ptr<const AnaWprimeCandidate>  > wall_electron_smeared_JESup;
  EventObjectColl<  boost::shared_ptr<const AnaWprimeCandidate>  > wall_elqcd;
  EventObjectColl<  boost::shared_ptr<const AnaWprimeCandidate>  > wall_elqcd_smeareddown;
  EventObjectColl<  boost::shared_ptr<const AnaWprimeCandidate>  > wall_elqcd_smearedup;
  EventObjectColl<  boost::shared_ptr<const AnaWprimeCandidate>  > wall_elqcd_JER;
  EventObjectColl<  boost::shared_ptr<const AnaWprimeCandidate>  > wall_elqcd_JESup;
  EventObjectColl<  boost::shared_ptr<const AnaWprimeCandidate>  > wall_elqcd_JESdown;
  EventObjectColl<  boost::shared_ptr<const AnaWprimeCandidate>  > wall_muon_qcd;
  EventObjectColl<  boost::shared_ptr<const AnaWprimeCandidate>  > wall_muon_smeared_qcd;

  // W event selected candidates
  EventObjectColl<  boost::shared_ptr<const AnaWprimeCandidate>  > w_muon;
  EventObjectColl<  boost::shared_ptr<const AnaWprimeCandidate>  > w_muon_smeared;
  EventObjectColl<  boost::shared_ptr<const AnaWprimeCandidate>  > w_muon_smeared_JER;
  EventObjectColl<  boost::shared_ptr<const AnaWprimeCandidate>  > w_muon_smeared_JESup;
  EventObjectColl<  boost::shared_ptr<const AnaWprimeCandidate>  > w_muon_smeared_JESdown;
  EventObjectColl<  boost::shared_ptr<const AnaWprimeCandidate>  > w_muon_msdown;
  EventObjectColl<  boost::shared_ptr<const AnaWprimeCandidate>  > w_muon_msup;
  EventObjectColl<  boost::shared_ptr<const AnaWprimeCandidate>  > w_muon_iddown;
  EventObjectColl<  boost::shared_ptr<const AnaWprimeCandidate>  > w_muon_idup;
  EventObjectColl<  boost::shared_ptr<const AnaWprimeCandidate>  > w_electron;
  EventObjectColl<  boost::shared_ptr<const AnaWprimeCandidate>  > w_electron_smeared;
  EventObjectColl<  boost::shared_ptr<const AnaWprimeCandidate>  > w_electron_smearedup;
  EventObjectColl<  boost::shared_ptr<const AnaWprimeCandidate>  > w_electron_smeareddown;
  EventObjectColl<  boost::shared_ptr<const AnaWprimeCandidate>  > w_electron_smeared_JER;
  EventObjectColl<  boost::shared_ptr<const AnaWprimeCandidate>  > w_electron_smeared_JESdown;
  EventObjectColl<  boost::shared_ptr<const AnaWprimeCandidate>  > w_electron_smeared_JESup;
  EventObjectColl<  boost::shared_ptr<const AnaWprimeCandidate>  > w_elqcd;
  EventObjectColl<  boost::shared_ptr<const AnaWprimeCandidate>  > w_elqcd_smeareddown;
  EventObjectColl<  boost::shared_ptr<const AnaWprimeCandidate>  > w_elqcd_smearedup;
  EventObjectColl<  boost::shared_ptr<const AnaWprimeCandidate>  > w_elqcd_JER;
  EventObjectColl<  boost::shared_ptr<const AnaWprimeCandidate>  > w_elqcd_JESup;
  EventObjectColl<  boost::shared_ptr<const AnaWprimeCandidate>  > w_elqcd_JESdown;
  EventObjectColl<  boost::shared_ptr<const AnaWprimeCandidate>  > w_muon_qcd;
  EventObjectColl<  boost::shared_ptr<const AnaWprimeCandidate>  > w_muon_smeared_qcd;

  // COMMON W CUTS
  DgCutSet<  boost::shared_ptr<const AnaWprimeCandidate>  > w_common_sel( "w_common_sel" );
  /*w_common_sel.add_uu( "at_least_one_lepton" ,
		       bind(&AnaWprimeCandidate::nleptons,_1) > 0,
		       bind(&dg::fillh,_1,10,0,10,bind(&AnaWprimeCandidate::nleptons,_2)) );*/
  w_common_sel.add_uu( "exactly_one_lepton" ,
		       bind(&AnaWprimeCandidate::exactly_one_lepton,_1) == true ,
		       bind(&dg::fillh,_1,10,0,10,bind(&AnaWprimeCandidate::nleptons,_2)) );
  w_common_sel.add_u( "exactly_one_met" , bind(&AnaWprimeCandidate::exactly_one_met,_1) );

  // RELEASE 17
  DgCutSet<  boost::shared_ptr<const AnaWprimeCandidate>  > w_sel( "w_sel" );
  w_sel.based_on( w_common_sel );

  // ALTERNATE RELEASE 17 SELECTION FOR W+JETS QCD SELECTION METHOD I
  DgCutSet<  boost::shared_ptr<const AnaWprimeCandidate>  > w_elqcd_sel( "w_elqcd_sel" );
  w_elqcd_sel.based_on( w_common_sel );
  //w_elqcd_sel.add_uu( "tight_electron" , bind(&AnaWprimeCandidate::electron_is_aod_tight,_1) ,
  //                  bind( &dg::filleff , _1 , bind(&AnaWprimeCandidate::electron_is_aod_tight,_2)) );

  // W SELECTION FOR W+JETS SYSTEMATIC STUDY
  // http://cdsweb.cern.ch/record/1325347 P. 29
  // This is implemented by adding a category to the StudyWprime object


  // define Z selections (tag and probe)
  // ================================================================

  // Z events
  EventObjectColl<  boost::shared_ptr<const AnaZCrossSectionCandidate>  > zall_electron;
  EventObjectColl<  boost::shared_ptr<const AnaZCrossSectionCandidate>  > zall_muon;
  EventObjectColl<  boost::shared_ptr<const AnaZCrossSectionCandidate>  > zall_muon_loose;
  // Z event selected candidates
  EventObjectColl<  boost::shared_ptr<const AnaZCrossSectionCandidate>  > z_electron;
  EventObjectColl<  boost::shared_ptr<const AnaZCrossSectionCandidate>  > z_muon;
  EventObjectColl<  boost::shared_ptr<const AnaZCrossSectionCandidate>  > z_muon_loose;

  // COMMON Z CUTS
  DgCutSet<  boost::shared_ptr<const AnaZCrossSectionCandidate>  > z_common_sel( "z_common_sel" );
  /*z_common_sel.add_uu( "exactly_two_opposite_leptons" ,
                       bind(&AnaZCrossSectionCandidate::exactly_two_opposite_leptons,_1) == true ,
                       bind(&dg::fillh,_1,10,0,10,bind(&AnaZCrossSectionCandidate::nleptons,_2)) );*/
  /*  z_common_sel.add_uu( "leptons_moving_at_opposite_phi_directions",
                       bind(&AnaZCrossSectionCandidate::leptons_moving_at_opposite_phi_directions,_1) == true ,
                       bind(&dg::fillh,_1,10,0,10,bind(&AnaZCrossSectionCandidate::nleptons,_2)) );*/

  // RELEASE 16
  DgCutSet<  boost::shared_ptr<const AnaZCrossSectionCandidate>  > z_sel( "z_sel" );
  z_sel.based_on( z_common_sel );
  z_sel.add_uu( "inv_mass" ,
                bind(&AnaZCrossSectionCandidate::m,_1)<=100. && bind(&AnaZCrossSectionCandidate::m,_1)>=80. ,
                bind(&dg::fillh,_1,500,0,500, bind(&AnaZCrossSectionCandidate::m , _2)) );
  z_sel.add_uu( "atleast_one_muon_is_tight" ,
		bind(&AnaZCrossSectionCandidate::atleast_one_muon_is_tight,_1) == true ,
		bind(&dg::fillh,_1,500,0,500, bind(&AnaZCrossSectionCandidate::nleptons , _2)) );


  // ================================================================
  METUtility *metutil = new METUtility();
  // Old configuration
  //metutil->setCellFix( false );
  //metutil->doForwardEtaCut( false );
  //metutil->setPileUpUncertainty( 0.1 );

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

  bool isMC(true);
  EisoTool* m_eiso_mc = new EisoTool(isMC);
  EisoTool* m_eiso = new EisoTool();
  //usleep(5000000); //****

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

  // ---------------------- event loop -------------------------

  unsigned long n_events = 0ul;
  AnaEventMgr* evtmgr = AnaEventMgr::instance(); assert(evtmgr);
  for( AnaEventMgr::iterator i=evtmgr->begin(), f=evtmgr->end(AnaConfiguration::user_max_nevents()); i!=f; ++i, ++n_events ) {

    const boost::shared_ptr<const AnaEvent>& evt( *i );
    if( !evt ) { break; }
    dg::set_global_weight( 1. );

    // check sample type and decide whether or not we need to make
    // data-only or mc-only preselection cuts. then make them.
    // ================================================================
    AnaEvent::tribool is_mc = evt->check_flag( "mc" );
    const bool do_not_require_good_run = is_mc;

    // get run info
    const unsigned long raw_run_number = evt->run_number();
    const unsigned long mc_channel = evt->mc_channel();
    const unsigned long event_number = evt->event_number();
    const double average_mu = evt->average_mu();
    const unsigned long lumi_block = evt->lumi_block();
    unsigned long run_number = raw_run_number;

    //if( run_number!=183130 ) {continue;} // temporary
    //if( run_number!=183038 ) {continue;}
    dg::fillhw( "events_in_run" , 30000, 160000, 190000, run_number , 1. );

    /* SET PILEUP WEIGHT */
    double pu_weight = 1.;
    double mc_weight = 1.;
    static boost::shared_ptr<Root::TPileupReweighting> pw( new Root::TPileupReweighting( "pw" ) );
    if( evt->check_tag("mc11c") && pileup_reweighting_mode != PU_MC11C ) {
      pileup_reweighting_mode = PU_MC11C;
      pw.reset( new Root::TPileupReweighting( "pw" ) );
      pw->SetDefaultChannel( 0 );
      pw->SetUnrepresentedDataAction( 2 ); // just keep the data
      pw->DisableWarnings( true );
      pw->SetDataScaleFactors( AnaConfiguration::pileup_data_scale() );
      pw->AddConfigFile( pileup_config_file );
      pw->AddLumiCalcFile( pileup_data_file );
      pw->Initialize();
      int_lumi = pw->getIntegratedLumiVector();
      lumi_tot = pw->GetIntegratedLumi();
      std::cout << "Initialized TExtendedPileupReweighting for MC11C" << std::endl;
      std::cout << "Total lumi = " << lumi_tot << std::endl;
    }

    // for data, we have an option of dropping "unrepresented" data
    if( !is_mc ) const bool vetoEvent = false;
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
    const double event_weight = pu_weight * mc_weight;

    // check trigger
    const boost::shared_ptr<const AnaTrigger>& trigger = evt->trigger();
    if( true ) {
      dg::down( "triggers" , "" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      dg::filleff( "trigger_present" , trigger );
    }
    if( !trigger ) { std::cerr << "WARNING: trigger information not present for run " << raw_run_number << " evt " << event_number << ". skipping..." << std::endl; continue; }

    // Hfor (for W+jets)
    int hfor_type = evt->mc_hfor();
    if( is_mc && (AnaConfiguration::do_hfor()==true) ) {
      if( (hfor_type<0) || (hfor_type>3) ) { continue; }
    }

    // count total events
    dg::fillhw( "h_total_evts" , 1 , 1 , 2 , 1 , event_weight );

    /* GOOD RUNS LIST */
    const bool is_good_run_ele = is_mc ? true : grl.is_in( GoodRunList::ELECTRON , run_number , lumi_block );
    const bool is_good_run_mu = is_mc ? true : grl.is_in( GoodRunList::MUON , run_number , lumi_block );;
    if( true ) {
      dg::down( "presel" , "" );
      dg::filleff( "c1_grl_el" , is_good_run_ele );
      dg::filleff( "c1_grl_mu" , is_good_run_mu );
      dg::filleff( "c1_grl_lep" , is_good_run_mu || is_good_run_ele );
      dg::up();
    }
    if( !is_good_run_ele && !is_good_run_mu ) { continue; }
    dg::fillhw( "h_total_evts_grl" , 1 , 1 , 2 , 1 , event_weight );

    // larError check for data only
    const bool lar_error = ( (!is_mc) && evt->trigger()->error_lar() );
    if( true ) {
      dg::down( "presel" , "" );
      dg::filleff( "c2_larerror" , !lar_error );
      dg::up();
    }
    if( lar_error ) { continue; }


    /* VERTEX SELECTION */
    if( studySelections ) {
      vtx_sel.for_each( evt->vertices().begin() , evt->vertices().end() );
      // electron trigger
      if( true ) {
	dg::down( "sel_after_electron_trigger" , "" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
	if( is_mc) {
	  if( evt->trigger()->top_electron_trigger_2012_mc11bc( run_number ) ) vtx_sel.for_each( evt->vertices().begin() , evt->vertices().end() );
	}
	else {
	  if( evt->trigger()->top_electron_trigger_2012_data( run_number ) ) vtx_sel.for_each( evt->vertices().begin() , evt->vertices().end() );
	}
	// muon trigger
	if( true ) {
	  dg::down( "sel_after_muon_trigger" , "" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
	  if( evt->trigger()->top_muon_trigger_2012( run_number ) ) vtx_sel.for_each( evt->vertices().begin() , evt->vertices().end() );
	}
      }
    }
    dgSTL::copy_if( evt->vertices().begin() , evt->vertices().end() , back_inserter(vtx_baseline), CUT_ALL(AnaVertex,vtx_sel) );
    if( is_mc ) dg::set_global_weight( 1. );
    dg::fillh("nvtxs_base_unweighted",20,0,20, vtx_baseline.size()<=20 ? vtx_baseline.size() : 20 );
    if( is_mc ) dg::set_global_weight( event_weight );
    dg::fillh("nvtxs_base",20,0,20, vtx_baseline.size()<=20 ? vtx_baseline.size() : 20 );
    if( vtx_baseline.size()>0) {
      dg::fillhw( "h_total_evts_vtx_unweighted" , 1 , 1 , 2 , 1 , 1. );
      dg::fillhw( "h_total_evts_vtx" , 1 , 1 , 2 , 1 , event_weight );
    }
    // create vertex collection to be used for jet corrections
    dgSTL::copy_if( evt->vertices().begin() , evt->vertices().end() , back_inserter(vtx_jetCal), CUT_ALL(AnaVertex,vtx_jetCal_sel) );
    dgSTL::copy_if( evt->vertices().begin() , evt->vertices().end() , back_inserter(vtx_jetcorrection), CUT_ALL(AnaVertex,vtx_jetcorrection_sel) );

    // continue if GRL doesn't pass or vtx cuts fail
    if( vtx_baseline.size()<=0 ) { continue; }
    if( is_mc ) dg::set_global_weight( 1. );
    dg::fillh("nvtxs_grl_unweighted",20,0,20, vtx_baseline.size()<=20 ? vtx_baseline.size() : 20 );
    if( is_mc ) dg::set_global_weight( event_weight );
    dg::fillh("nvtxs_grl",20,0,20, vtx_baseline.size()<=20 ? vtx_baseline.size() : 20 );

    /* STUDY SELECTIONS */
    if( studySelections ) {
      jet_sel.for_each( evt->atlas_anti_kt_4_h1_topo_jets().begin() , evt->atlas_anti_kt_4_h1_topo_jets().end() );
      el_sel.for_each( evt->electrons().begin() , evt->electrons().end() );
      el_qcd_sel.for_each( evt->electrons().begin() , evt->electrons().end() );
      mu_sel.for_each( evt->muons().begin() , evt->muons().end() );
      mu_qcd_sel.for_each( evt->muons().begin() , evt->muons().end() );
    }

    /* ELECTRON SELECTIONS */
    // trigger
    if( is_mc ) {
      if( evt->trigger()->top_electron_trigger_2012_mc11bc(run_number) ) std::copy( evt->electrons().begin() , evt->electrons().end() , back_inserter(el_trigger) );
    } else {
      if( evt->trigger()->top_electron_trigger_2012_data(run_number) ) std::copy( evt->electrons().begin() , evt->electrons().end() , back_inserter(el_trigger) );
    }
    if( el_trigger.size()>0) {
      dg::fillhw( "h_total_evts_eltrig" , 1 , 1 , 2 , 1 , event_weight );
    }      
    // preselection smearing for electrons
    if( is_mc ) {
      size_t iel = 0;
      BOOST_FOREACH( const boost::shared_ptr<const AnaElectron>& el , el_trigger ) {
	el_trigger_smeared.push_back( el->egamma_smeared( conf , el , event_number , iel , "NOMINAL" ) );
	el_trigger_smearedup.push_back( el->egamma_smeared( conf , el , event_number , iel , "SMEARUP" ) );
	el_trigger_smeareddown.push_back( el->egamma_smeared( conf , el , event_number , iel , "SMEARDOWN" ) );
	iel++; }
    }
    // preselection energy corrections for electrons in data
    else {
      size_t iel = 0;
      BOOST_FOREACH( const boost::shared_ptr<const AnaElectron>& el , el_trigger ) {
	el_corrected.push_back( el->egamma_corrected( conf , el , event_number , iel , "NOMINAL" ) );
	iel++; }
    }
    // isolation cut
    if( is_mc ) {
      // smeared
      BOOST_FOREACH( const boost::shared_ptr<const AnaElectron>& el , el_trigger_smeared ) {
	if( m_eiso_mc->Eiso90Etcone20( vtx_jetCal.size() , (el->cluster_pt())*cosh(el->track_eta())*1000 , el->track_eta() , el->cluster_eta() , el->eta2() , el->etap() , el->uncorrected_et_cone(AnaElectron::CONE20)*1000 )==1 ) {
	  if( m_eiso_mc->Eiso90Ptcone30( (el->cluster_pt())*cosh(el->track_eta())*1000 , el->track_eta() , el->cluster_eta() , el->pt_cone(AnaElectron::CONE30)*1000 )==1 ) {
	    el_iso_smeared.push_back( el ); } }
      }
      // smeared up
      BOOST_FOREACH( const boost::shared_ptr<const AnaElectron>& el , el_trigger_smearedup ) {
	if( m_eiso_mc->Eiso90Etcone20( vtx_jetCal.size() , (el->cluster_pt())*cosh(el->track_eta())*1000 , el->track_eta() , el->cluster_eta() , el->eta2() , el->etap() , el->uncorrected_et_cone(AnaElectron::CONE20)*1000 )==1 ) {
	  if( m_eiso_mc->Eiso90Ptcone30( (el->cluster_pt())*cosh(el->track_eta())*1000 , el->track_eta() , el->cluster_eta() , el->pt_cone(AnaElectron::CONE30)*1000 )==1 ) {
	    el_iso_smearedup.push_back( el ); } }
      }
      // smeared down
      BOOST_FOREACH( const boost::shared_ptr<const AnaElectron>& el , el_trigger_smeareddown ) {
	if( m_eiso_mc->Eiso90Etcone20( vtx_jetCal.size() , (el->cluster_pt())*cosh(el->track_eta())*1000 , el->track_eta() , el->cluster_eta() , el->eta2() , el->etap() , el->uncorrected_et_cone(AnaElectron::CONE20)*1000 )==1 ) {
	  if( m_eiso_mc->Eiso90Ptcone30( (el->cluster_pt())*cosh(el->track_eta())*1000 , el->track_eta() , el->cluster_eta() , el->pt_cone(AnaElectron::CONE30)*1000 )==1 ) {
	    el_iso_smeareddown.push_back( el ); } }
      }
    }
    else {
      BOOST_FOREACH( const boost::shared_ptr<const AnaElectron>& el , el_corrected ) {
	if( m_eiso->Eiso90Etcone20( vtx_jetCal.size() , (el->cluster_pt())*cosh(el->track_eta())*1000 , el->track_eta() , el->cluster_eta() , el->eta2() , el->etap() , el->uncorrected_et_cone(AnaElectron::CONE20)*1000 )==1 ) {
	  if( m_eiso->Eiso90Ptcone30( (el->cluster_pt())*cosh(el->track_eta())*1000 , el->track_eta() , el->cluster_eta() , el->pt_cone(AnaElectron::CONE30)*1000 )==1 ) {
	    el_iso.push_back( el ); } }
      }
    }
    // cuts
    if( is_mc ) {
      // the rest of the cuts
      dgSTL::copy_if( el_iso_smeared.begin() , el_iso_smeared.end() , back_inserter(el_baseline_smeared) , CUT_ALL(AnaElectron,el_sel) );
      dgSTL::copy_if( el_iso_smearedup.begin() , el_iso_smearedup.end() , back_inserter(el_baseline_smearedup) , CUT_ALL(AnaElectron,el_sel) );
      dgSTL::copy_if( el_iso_smeareddown.begin() , el_iso_smeareddown.end() , back_inserter(el_baseline_smeareddown) , CUT_ALL(AnaElectron,el_sel) );
      // no need to apply QCD cuts to smeared electrons since QCD comes from data
    }
    else {
      dgSTL::copy_if( el_iso.begin() , el_iso.end() , back_inserter(el_baseline), CUT_ALL(AnaElectron,el_sel) );
      dgSTL::copy_if( el_iso.begin() , el_iso.end() , back_inserter(el_qcd), CUT_ALL(AnaElectron,el_qcd_sel) );
    }
    
    // temporary
    if( el_baseline.size() || el_baseline_smeared.size() ) {
      dg::fillhw( "h_total_evts_el" , 1 , 1 , 2 , 1 , event_weight );
    }
    if( (el_baseline.size()==1) || (el_baseline_smeared.size()==1) ) {
      dg::fillhw( "h_total_evts_1el" , 1 , 1 , 2 , 1 , event_weight );
    }

    /* MUON SELECTION PART 1*/
    if( evt->trigger()->top_muon_trigger_2012(run_number) ) std::copy( evt->muons().begin() , evt->muons().end() , back_inserter(mu_trigger) );
    if( el_trigger.size()==0 && mu_trigger.size()==0 ) continue;  

    // temporary    
    if( mu_trigger.size()>0) {
      dg::fillhw( "h_total_evts_mutrig" , 1 , 1 , 2 , 1 , event_weight );
    }

    // if running over MC, do smearing and scale corrections
    if( is_mc ) {
      size_t imuon = 0;
      for( std::vector< shared_ptr<const AnaMuon> >::const_iterator i=mu_trigger.begin() , f=mu_trigger.end(); i!=f; ++i,++imuon ) {
	const shared_ptr<const AnaMuon> mu(*i);
	mu_smeared.push_back( mu->mcp_smeared(conf,AnaConfiguration::data_range(),mu,mu_type,evt->event_number(),imuon,true,"NOMINAL") );
	mu_msdown.push_back( mu->mcp_smeared(conf,AnaConfiguration::data_range(),mu,mu_type,evt->event_number(),imuon,true,"MSLOW") );
	mu_msup.push_back( mu->mcp_smeared(conf,AnaConfiguration::data_range(),mu,mu_type,evt->event_number(),imuon,true,"MSUP") );
	mu_iddown.push_back( mu->mcp_smeared(conf,AnaConfiguration::data_range(),mu,mu_type,evt->event_number(),imuon,true,"IDLOW") );
	mu_idup.push_back( mu->mcp_smeared(conf,AnaConfiguration::data_range(),mu,mu_type,evt->event_number(),imuon,true,"IDUP") );
	imuon++;
      }
    }

    // temporary
    if( el_baseline.size()==1 || el_baseline_smeared.size()==1) {
      dg::fillhw( "h_total_evts_check" , 1 , 1 , 2 , 1 , event_weight );
    }

    /* JET SELECTION */
    // Use only good jets for overlap removal
    size_t ijet = 0;
    BOOST_FOREACH( const boost::shared_ptr<const AnaJet>& jet , evt->atlas_anti_kt_4_h1_topo_jets() ) {
      if( is_mc ) {
	lepjet_jetCal.push_back( jet->jetetmiss_calibrate( conf , jet , event_number , ijet , average_mu , vtx_jetCal.size() , is_mc , true , true ) );
      } else {
	lepjet_jetCal.push_back( jet->jetetmiss_calibrate( conf , jet , event_number , ijet , average_mu , vtx_jetCal.size() , is_mc , true , false ) );
      }
      ijet++;
    }
    // for metutil
    BOOST_FOREACH( const boost::shared_ptr<const AnaJet>& jet , evt->atlas_anti_kt_4_h1_lc_topo_jets() ) {
      if( is_mc ) {
	lepjet_LC.push_back( jet->jetetmiss_calibrate( conf , jet , event_number , ijet , average_mu , vtx_jetCal.size() , is_mc , true , true ) );
      } else {
	lepjet_LC.push_back( jet->jetetmiss_calibrate( conf , jet , event_number , ijet , average_mu , vtx_jetCal.size() , is_mc , true , false ) );
      }
      ijet++;
    }
    // cut at 7 GeV so drmin is calculated according to recommendation
    // not doing this at the moment since I'm not bothering with the MultijetJES package
    // (how do yo propagate the uncertainty to MET if you use multiJES package, since you're already cutting out jets with pT<7?)
    if( is_mc ) {
      ijet = 0;
      // if njets < 2, then MultijetJES will not be used
      int njets = 1;
      BOOST_FOREACH( boost::shared_ptr<const AnaJet>& jet , lepjet_jetCal ) {
        double drmin = 0.; 
	lepjet_JESup.push_back( jet->jetetmiss_scale( conf , jet , event_number , njets , drmin , ijet , evt->average_mu() , vtx_jetcorrection.size() , "JESUP" ) );
	lepjet_JESdown.push_back( jet->jetetmiss_scale( conf , jet , event_number , njets , drmin , ijet , evt->average_mu() , vtx_jetcorrection.size() , "JESDOWN" ) );
        ijet++;
      }
    }
    // apply cuts
    dgSTL::copy_if( lepjet_jetCal.begin() , lepjet_jetCal.end() , back_inserter(lepjet_baseline) , CUT_ALL(AnaJet,jet_sel) );
    dgSTL::copy_if( lepjet_jetCal.begin() , lepjet_jetCal.end() , back_inserter(badlepjet_baseline) , CUT_ALL(AnaJet,jet_bad_sel) );
    // just for the raw jets (no JES or JER corrected jets)

    if( is_mc ) {
      dgSTL::copy_if( lepjet_JESdown.begin() , lepjet_JESdown.end() , back_inserter(lepjet_baseline_JESdown) , CUT_ALL(AnaJet,jet_sel) );
      dgSTL::copy_if( lepjet_JESup.begin() , lepjet_JESup.end() , back_inserter(lepjet_baseline_JESup) , CUT_ALL(AnaJet,jet_sel) );
      // do overlap removal
      // for this I used nominal smeared electrons and muons
      if( el_baseline_smeared.size() ) {
	overlap_removal_tight( lepjet_baseline.begin() , lepjet_baseline.end() , el_baseline_smeared.begin() , el_baseline_smeared.end() ,
			       0.2 , back_inserter(jet_baseline_el) );
	overlap_removal_tight( lepjet_baseline_JESdown.begin() , lepjet_baseline_JESdown.end() , el_baseline_smeared.begin() , el_baseline_smeared.end() ,
			       0.2 , back_inserter(jet_baseline_JESdown_el) );
	overlap_removal_tight( lepjet_baseline_JESup.begin() , lepjet_baseline_JESup.end() , el_baseline_smeared.begin() , el_baseline_smeared.end() ,
			       0.2 , back_inserter(jet_baseline_JESup_el) );
      }
      // remove muon that is within dR<0.4 with a jet
      if( mu_smeared.size() ) {
	overlap_removal_loose( mu_smeared.begin() , mu_smeared.end() , lepjet_baseline.begin() , lepjet_baseline.end() ,
			       0.4 , back_inserter(mu_smeared_or) );
	overlap_removal_loose( mu_msdown.begin() , mu_msdown.end() , lepjet_baseline.begin() , lepjet_baseline.end() ,
			       0.4 , back_inserter(mu_msdown_or) );
	overlap_removal_loose( mu_msup.begin() , mu_msup.end() , lepjet_baseline.begin() , lepjet_baseline.end() ,
			       0.4 , back_inserter(mu_msup_or) );
	overlap_removal_loose( mu_iddown.begin() , mu_iddown.end() , lepjet_baseline.begin() , lepjet_baseline.end() ,
			       0.4 , back_inserter(mu_iddown_or) );
	overlap_removal_loose( mu_idup.begin() , mu_idup.end() , lepjet_baseline.begin() , lepjet_baseline.end() ,
			       0.4 , back_inserter(mu_idup_or) );
	BOOST_FOREACH( const boost::shared_ptr<const AnaJet>& jet , lepjet_baseline ) {
	  jet_baseline_mu.push_back( jet ); }
	BOOST_FOREACH( const boost::shared_ptr<const AnaJet>& jet , lepjet_baseline_JER ) {
	  jet_baseline_JER_mu.push_back( jet ); }
	BOOST_FOREACH( const boost::shared_ptr<const AnaJet>& jet , lepjet_baseline_JESdown ) {
	  jet_baseline_JESdown_mu.push_back( jet ); }
	BOOST_FOREACH( const boost::shared_ptr<const AnaJet>& jet , lepjet_baseline_JESup ) {
	  jet_baseline_JESup_mu.push_back( jet ); }
      }
    } else { // data
      if( el_baseline.size() ) {
	// Note: if el_baseline.size()>0 then the QCD AnaWprimeCandidate is not built.
	overlap_removal_tight( lepjet_baseline.begin() , lepjet_baseline.end() , el_baseline.begin() , el_baseline.end() , 0.2 , back_inserter(jet_baseline_el) );
      } else {
	overlap_removal_tight( lepjet_baseline.begin() , lepjet_baseline.end() , el_qcd.begin() , el_qcd.end() , 0.2 , back_inserter(jet_baseline_el) );
      }
      // remove muon that is within dR<0.4 with a jet
      if( mu_trigger.size() ) {
	overlap_removal_loose( mu_trigger.begin() , mu_trigger.end() , lepjet_baseline.begin() , lepjet_baseline.end() , 0.4 , back_inserter(mu_trigger_or) );
	BOOST_FOREACH( const boost::shared_ptr<const AnaJet>& jet , lepjet_baseline ) {
	  jet_baseline_mu.push_back( jet );
	  jet_baseline_mu_qcd.push_back( jet );
	  jet_baseline_mu_loose.push_back( jet );
	}
      }
    }

    /* MUON SELECTION PART 2*/
    if( is_mc ) {
      dgSTL::copy_if( mu_smeared_or.begin() , mu_smeared_or.end() , back_inserter(mu_baseline_smeared) , CUT_ALL(AnaMuon, mu_sel) );
      dgSTL::copy_if( mu_smeared_or.begin() , mu_smeared_or.end() , back_inserter(mu_smeared_qcd) , CUT_ALL(AnaMuon, mu_qcd_sel) );
      dgSTL::copy_if( mu_msdown_or.begin() , mu_msdown_or.end() , back_inserter(mu_baseline_msdown) , CUT_ALL(AnaMuon, mu_sel) );
      dgSTL::copy_if( mu_msup_or.begin() , mu_msup_or.end() , back_inserter(mu_baseline_msup) , CUT_ALL(AnaMuon, mu_sel) );
      dgSTL::copy_if( mu_iddown_or.begin() , mu_iddown_or.end() , back_inserter(mu_baseline_iddown) , CUT_ALL(AnaMuon, mu_sel) );
      dgSTL::copy_if( mu_idup_or.begin() , mu_idup_or.end() , back_inserter(mu_baseline_idup) , CUT_ALL(AnaMuon, mu_sel) );
    }
    // apply cuts to unsmeared vector
    dgSTL::copy_if( mu_trigger_or.begin() , mu_trigger_or.end() , back_inserter(mu_baseline) , CUT_ALL(AnaMuon, mu_sel) );
    // loose
    dgSTL::copy_if( mu_trigger_or.begin() , mu_trigger_or.end() , back_inserter(mu_loose) , CUT_ALL(AnaMuon, mu_common_sel) );
    // qcd
    dgSTL::copy_if( mu_trigger_or.begin() , mu_trigger_or.end() , back_inserter(mu_qcd) , CUT_ALL(AnaMuon, mu_qcd_sel) );

    // temporary
    if( mu_baseline.size() || mu_baseline_smeared.size() ) {
      dg::fillhw( "h_total_evts_mu" , 1 , 1 , 2 , 1 , event_weight );
    }
    if( (mu_baseline.size()==1) || (mu_baseline_smeared.size()==1) ) {
      dg::fillhw( "h_total_evts_1mu" , 1 , 1 , 2 , 1 , event_weight );
    }

    
    /* B TAG */
    BOOST_FOREACH( const boost::shared_ptr<const AnaJet>& jet , jet_baseline_el ) {
      if( mv1Eval( jet->tag_ip3d_w(), jet->tag_sv1_w(), jet->tag_cmbnn_w(), jet->pt(), jet->eta() )>0.601713 ) {
	jet_baseline_el_btag.push_back( jet );
      }
    }
    BOOST_FOREACH( const boost::shared_ptr<const AnaJet>& jet , jet_baseline_mu ) {
      if( mv1Eval( jet->tag_ip3d_w(), jet->tag_sv1_w(), jet->tag_cmbnn_w(), jet->pt(), jet->eta() )>0.601713 ) {
	jet_baseline_mu_btag.push_back( jet );
      }
    }
    BOOST_FOREACH( const boost::shared_ptr<const AnaJet>& jet , jet_baseline_mu_qcd ) {
      if( mv1Eval( jet->tag_ip3d_w(), jet->tag_sv1_w(), jet->tag_cmbnn_w(), jet->pt(), jet->eta() )>0.601713 ) {
	jet_baseline_mu_qcd_btag.push_back( jet );
      }
    }
    BOOST_FOREACH( const boost::shared_ptr<const AnaJet>& jet , jet_baseline_mu_loose ) {
      if( mv1Eval( jet->tag_ip3d_w(), jet->tag_sv1_w(), jet->tag_cmbnn_w(), jet->pt(), jet->eta() )>0.601713 ) {
	jet_baseline_mu_loose_btag.push_back( jet );
      }
    }

    //dgSTL::copy_if( jet_baseline_el.begin() , jet_baseline_el.end() , back_inserter(jet_baseline_el_btag) , CUT_ALL(AnaJet,jet_baseline_btag_sel) );
    //dgSTL::copy_if( jet_baseline_mu.begin() , jet_baseline_mu.end() , back_inserter(jet_baseline_mu_btag) , CUT_ALL(AnaJet,jet_baseline_btag_sel) );
    //dgSTL::copy_if( jet_baseline_mu_qcd.begin() , jet_baseline_mu_qcd.end() , back_inserter(jet_baseline_mu_qcd_btag) , CUT_ALL(AnaJet,jet_baseline_btag_sel) );
    //dgSTL::copy_if( jet_baseline_mu_loose.begin() , jet_baseline_mu_loose.end() , back_inserter(jet_baseline_mu_loose_btag) , CUT_ALL(AnaJet,jet_baseline_btag_sel) );

    // preselection MET reconstruction using METUtility
    dgSTL::copy_if( evt->missing_et().begin() , evt->missing_et().end() , back_inserter(met_reffinal_uncorrected) , CUT_ALL(AnaMET,met_rf_sel) );
    dg::down( "metcalib" , "" );
    assert( met_reffinal_uncorrected.size()==1 );
    boost::shared_ptr<const AnaMET> met( *met_reffinal_uncorrected.begin() );
    const float orig_met_pt = met->pt();
    if( is_mc ) {
      metutil->reset();
      float MeV = 1000.;
      metutil->setMETTerm( METUtil::RefGamma , met->refgamma_etx()*MeV , met->refgamma_ety()*MeV , met->refgamma_sumet()*MeV );
      metutil->setMETTerm( METUtil::RefTau , met->reftau_etx()*MeV , met->reftau_ety()*MeV , met->reftau_sumet()*MeV );
      metutil->setMETTerm( METUtil::CellOutEflow , met->cellout_eflow_etx()*MeV , met->cellout_eflow_ety()*MeV , met->cellout_eflow_sumet()*MeV );
      metutil->setMETTerm( METUtil::MuonTotal , met->muonboy_etx()*MeV , met->muonboy_ety()*MeV , met->muonboy_sumet()*MeV );
      metutil->setMETTerm( METUtil::CellOut , met->cellout_etx()*MeV , met->cellout_ety()*MeV , met->cellout_sumet()*MeV );
      metutil->setMETTerm( METUtil::RefMuon , met->refmuon_etx()*MeV , met->refmuon_ety()*MeV , met->refmuon_sumet()*MeV );
      metutil->setMETTerm( METUtil::SoftJets , met->softjets_etx()*MeV , met->softjets_ety()*MeV , met->softjets_sumet()*MeV );
      met_reffinal.push_back( met->jetetmiss_met( met , metutil ,
						  el_trigger_smeared.begin() , el_trigger_smeared.end() , true ,
						  mu_smeared.begin() , mu_smeared.end() , true ,
						  lepjet_LC.begin() , lepjet_LC.end() , true ,
						  METUtil::RefFinal , METUtil::None ) );
      dg::fillh( "delta_pt" , 200 , -20 , 20 , (*met_reffinal.begin())->pt() - (*met_reffinal_uncorrected.begin())->pt() );
      dg::fillh( "delta_eta" , 100 , -5 , 5 , (*met_reffinal.begin())->eta() - (*met_reffinal_uncorrected.begin())->eta() );
      dg::fillh( "delta_phi" , 200 , -M_PI , M_PI , (*met_reffinal.begin())->phi() - (*met_reffinal_uncorrected.begin())->phi() );
    }
    else {
      met_reffinal.push_back( met->jetetmiss_met( met , metutil ,
						  el_corrected.begin() , el_corrected.end() , true ,
						  mu_trigger.begin() , mu_trigger.end() , true ,
						  lepjet_LC.begin() , lepjet_LC.end() , true ,
						  METUtil::RefFinal , METUtil::None ) );
      dg::fillh( "delta_pt" , 200 , -20 , 20 , (*met_reffinal.begin())->pt() - (*met_reffinal_uncorrected.begin())->pt() );
      dg::fillh( "delta_eta" , 100 , -5 , 5 , (*met_reffinal.begin())->eta() - (*met_reffinal_uncorrected.begin())->eta() );
      dg::fillh( "delta_phi" , 200 , -M_PI , M_PI , (*met_reffinal.begin())->phi() - (*met_reffinal_uncorrected.begin())->phi() );
    }
    dg::up();

    // MET cleaning (bad jet veto)
    if( badlepjet_baseline.size() ) { continue; }

    // jet LAr Hole veto
    //if( jet_larhole_veto( jet_baseline_el , is_mc , raw_run_number ) ) { continue; }

    // effscales
    if( is_mc ) {
      if( el_baseline_smeared.size() ) {
	float eleff = 1. , eleff_staterror = 0.;
	float eltrig = 1. , eltrig_staterror = 0.;
	el_eff_scale( el_baseline_smeared , eleff , eleff_staterror );
	el_trig_scale( el_baseline_smeared , el_type , run_number , eltrig , eltrig_staterror );
	dg::down( "effscales" , "" );
	dg::down( "wel" , "" );
	dg::fillh( "el_eff" , 500 , 0 , 1.2 , eleff );
	dg::fillh( "el_trig" , 500 , 0 , 1.2 , eltrig );
	dg::up();
	dg::up();
	dg::set_global_weight( event_weight * eleff * eltrig );
      }
      if( mu_baseline_smeared.size() ) {
	double mueff = 1. , mueff_staterror = 0. , mueff_syserror = 0.;
	double mutrig = 1. , mutrig_staterror = 0.;
	mu_eff_scale( mu_baseline_smeared , run_number , int_lumi , run_periods , mueff , mueff_staterror , mueff_syserror );
	mu_trig_scale( mu_baseline_smeared , el_type , run_number , mutrig , mutrig_staterror );
	dg::down( "effscales" , "" );
	dg::down( "wmu" , "" );
	dg::fillh( "mu_eff" , 500 , 0 , 1.2 , mueff );
	dg::fillh( "mu_trig" , 500 , 0 , 1.2 , mutrig );
	dg::up();
	dg::up();
	dg::set_global_weight( event_weight * mueff * mutrig );
      }
    }

    /* PARTICLE COUNTS */
    if( true ) {
      dg::down( "particle_counts" , "" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      // number of final states selected.
      dg::fillh( "n_vtx" , 10 , 0 , 10 , vtx_baseline.size() );
      dg::fillh( "n_el_trigger" , 10 , 0 , 10 , el_trigger.size() );
      dg::fillh( "n_el_corrected" , 10 , 0 , 10 , el_corrected.size() );
      dg::fillh( "n_el_trigger_smeared" , 10 , 0 , 10 , el_trigger_smeared.size() );
      dg::fillh( "n_el_iso" , 10 , 0 , 10 , el_iso.size() );
      dg::fillh( "n_el_iso_smeared" , 10 , 0 , 10 , el_iso_smeared.size() );
      dg::fillh( "n_el" , 10 , 0 , 10 , el_baseline.size() );
      dg::fillh( "n_el_smeared" , 10 , 0 , 10 , el_baseline_smeared.size() );
      dg::fillh( "n_mu_trigger" , 10 , 0 , 10 , mu_trigger.size() );
      dg::fillh( "n_mu_trigger_or" , 10 , 0 , 10 , mu_trigger_or.size() );
      dg::fillh( "n_mu" , 10 , 0 , 10 , mu_baseline.size() );
      dg::fillh( "n_mu_smeared" , 10 , 0 , 10 , mu_baseline_smeared.size() );
      dg::fillh( "n_elemu" , 10 , 0 , 10 , el_baseline.size() + mu_baseline.size() );
      dg::fillh( "n_met_uncorrected" , 10 , 0 , 10 , met_reffinal_uncorrected.size() );
      dg::fillh( "n_met_corrected" , 10 , 0 , 10 , met_reffinal.size() );
      dg::fillh( "n_lepjet" , 10 , 0 , 10 , lepjet_baseline.size() );
      dg::fillh( "n_jet_el" , 10 , 0 , 10 , jet_baseline_el.size() );
      dg::fillh( "n_jet_mu" , 10 , 0 , 10 , jet_baseline_mu.size() );
      dg::fillh( "n_bjet_el" , 10 , 0 , 10 , jet_baseline_el_btag.size() );
      dg::fillh( "n_bjet_mu" , 10 , 0 , 10 , jet_baseline_mu_btag.size() );
    }

    // to put the order of the electrons/muons according to their Pt
    std::sort( el_baseline.begin(), el_baseline.end(), bind(&AnaElectron::pt,_1) > bind(&AnaElectron::pt,_2) );
    std::sort( el_baseline_smeared.begin(), el_baseline_smeared.end(), bind(&AnaElectron::pt,_1) > bind(&AnaElectron::pt,_2) );
    std::sort( mu_baseline.begin(), mu_baseline.end(), bind(&AnaMuon::pt,_1) > bind(&AnaMuon::pt,_2) );
    std::sort( mu_baseline_smeared.begin(), mu_baseline_smeared.end(), bind(&AnaMuon::pt,_1) > bind(&AnaMuon::pt,_2) );


    /* VERTEX DISTRIBUTIONS */
    // create some vertex distributions for reweighting pileup
    if( true ||  AnaConfiguration::vtx_plots() ) {
      dg::down( "vertex_distributions" , "vertex distributions from events passing different triggers" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      if( is_mc ) dg::set_global_weight( 1. );
      if( evt->trigger()->top_electron_trigger_2012_mc11bc( run_number ) ) dg::fillh( "nvtxs_electron_trigger_unweighted" , 20 , 0 , 20 , vtx_baseline.size() );
      if( evt->trigger()->top_muon_trigger_2012( run_number ) ) dg::fillh( "nvtxs_muon_trigger_unweighted" ,     20 , 0 , 20 , vtx_baseline.size() );
      if( is_mc ) dg::set_global_weight( event_weight );
      if( evt->trigger()->top_electron_trigger_2012_mc11bc( run_number ) ) dg::fillh( "nvtxs_electron_trigger" , 20 , 0 , 20 , vtx_baseline.size() );
      if( evt->trigger()->top_muon_trigger_2012( run_number ) ) dg::fillh( "nvtxs_muon_trigger" ,     20 , 0 , 20 , vtx_baseline.size() );
    }

    /* WRITE NTUPLE (if specified by flag) */
    // save event if it has a W candidate
    if( AnaEventMgr::instance()->ana_event_ntuple_mode() == AnaEventNtuple::WRITE ) {
      AnaEventMgr::instance()->add_to_ana_event_ntuple( ( el_baseline.size() || mu_baseline.size() || el_qcd.size() || mu_qcd.size()) &&
                                                        met_reffinal.size() );
      continue;
    }

    // temporary
    if( (el_baseline.size()==1 || el_baseline_smeared.size()==1) && (mu_baseline.size()==0 && mu_baseline_smeared.size()==0) ) {
      dg::fillh( "h_total_evts_onlyel" , 1 , 1 , 2 , 1 );
    }   
    if( (el_baseline.size()==1 || el_baseline_smeared.size()==1) && (mu_baseline.size()==0 && mu_baseline_smeared.size()==0) && (met_reffinal.size()==1) ) {
      dg::fillh( "h_total_evts_Wenu" , 1 , 1 , 2 , 1 );
    }
    if( (mu_baseline.size()==1 || mu_baseline_smeared.size()==1) && (el_baseline.size()==0 && el_baseline_smeared.size()==0) ) {
      dg::fillh( "h_total_evts_onlymu" , 1 , 1 , 2 , 1 );
    }   
    if( (mu_baseline.size()==1 || mu_baseline_smeared.size()==1) && (el_baseline.size()==0 && el_baseline_smeared.size()==0) && (met_reffinal.size()==1) ) {
      dg::fillh( "h_total_evts_Wmunu" , 1 , 1 , 2 , 1 );
    }

    /* BUILD W CANDIDATES */
    // build W event candidates
    // no smearing / offsets
    build_w_candidates( el_baseline.begin() , el_baseline.end() , met_reffinal.begin() , met_reffinal.end() ,
                        jet_baseline_el.begin() , jet_baseline_el.end() , jet_baseline_el_btag.begin() , jet_baseline_el_btag.end() , vtx_baseline.begin() , vtx_baseline.end() , back_inserter(wall_electron) );
    build_w_candidates( el_qcd.begin() , el_qcd.end() , met_reffinal.begin() , met_reffinal.end() ,
                        jet_baseline_el.begin() , jet_baseline_el.end() , jet_baseline_el_btag.begin() , jet_baseline_el_btag.end() , vtx_baseline.begin() , vtx_baseline.end() ,   back_inserter(wall_elqcd) );
    build_w_candidates( mu_baseline.begin()  , mu_baseline.end() , met_reffinal.begin() , met_reffinal.end() ,
                        jet_baseline_mu.begin() , jet_baseline_mu.end() , jet_baseline_mu_btag.begin() , jet_baseline_mu_btag.end() , vtx_baseline.begin() , vtx_baseline.end() , back_inserter(wall_muon) );
    build_w_candidates( mu_qcd.begin()  , mu_qcd.end() , met_reffinal.begin() , met_reffinal.end() ,
                        jet_baseline_mu_qcd.begin() , jet_baseline_mu_qcd.end() , jet_baseline_mu_qcd_btag.begin() , jet_baseline_mu_qcd_btag.end() , vtx_baseline.begin() , vtx_baseline.end() , back_inserter(wall_muon_qcd) );

    // if running over MC , look at smearing, offsets, etc
    if( is_mc ) {
      // standard smearing
      build_w_candidates( el_baseline_smeared.begin() , el_baseline_smeared.end() , met_reffinal.begin() , met_reffinal.end() ,
                          jet_baseline_el.begin() , jet_baseline_el.end() , jet_baseline_el_btag.begin() , jet_baseline_el_btag.end() , vtx_baseline.begin() , vtx_baseline.end() , back_inserter(wall_electron_smeared) );
      build_w_candidates( mu_baseline_smeared.begin()  , mu_baseline_smeared.end()  , met_reffinal.begin() , met_reffinal.end() ,
                          jet_baseline_mu.begin() , jet_baseline_mu.end() , jet_baseline_mu_btag.begin() , jet_baseline_mu_btag.end() , vtx_baseline.begin() , vtx_baseline.end() , back_inserter(wall_muon_smeared) );
      build_w_candidates( mu_smeared_qcd.begin()  , mu_smeared_qcd.end() , met_reffinal.begin() , met_reffinal.end() ,
			  jet_baseline_mu_qcd.begin() , jet_baseline_mu_qcd.end() , jet_baseline_mu_qcd_btag.begin() , jet_baseline_mu_qcd_btag.end() , vtx_baseline.begin() , vtx_baseline.end() , back_inserter(wall_muon_smeared_qcd) );
      // JER
      build_w_candidates( el_baseline_smeared.begin() , el_baseline_smeared.end() , met_reffinal.begin() , met_reffinal.end() ,
                          jet_baseline_JER_el.begin() , jet_baseline_JER_el.end() , jet_baseline_el_btag.begin() , jet_baseline_el_btag.end() , vtx_baseline.begin() , vtx_baseline.end() , back_inserter(wall_electron_smeared_JER) );
      build_w_candidates( mu_baseline_smeared.begin()  , mu_baseline_smeared.end()  , met_reffinal.begin() , met_reffinal.end() ,
                          jet_baseline_JER_mu.begin() , jet_baseline_JER_mu.end() , jet_baseline_mu_btag.begin() , jet_baseline_mu_btag.end() , vtx_baseline.begin() , vtx_baseline.end() , back_inserter(wall_muon_smeared_JER) );

      // JESdown
      build_w_candidates( el_baseline_smeared.begin() , el_baseline_smeared.end() , met_reffinal.begin() , met_reffinal.end() ,
                          jet_baseline_JESdown_el.begin() , jet_baseline_JESdown_el.end() , jet_baseline_el_btag.begin() , jet_baseline_el_btag.end() , vtx_baseline.begin() , vtx_baseline.end() ,
                          back_inserter(wall_electron_smeared_JESdown) );
      build_w_candidates( mu_baseline_smeared.begin()  , mu_baseline_smeared.end()  , met_reffinal.begin() , met_reffinal.end() ,
                          jet_baseline_JESdown_mu.begin() , jet_baseline_JESdown_mu.end() , jet_baseline_mu_btag.begin() , jet_baseline_mu_btag.end() , vtx_baseline.begin() , vtx_baseline.end() ,
                          back_inserter(wall_muon_smeared_JESdown) );

      // JESup
      build_w_candidates( el_baseline_smeared.begin() , el_baseline_smeared.end() , met_reffinal.begin() , met_reffinal.end() ,
                          jet_baseline_JESup_el.begin() , jet_baseline_JESup_el.end() , jet_baseline_el_btag.begin() , jet_baseline_el_btag.end() , vtx_baseline.begin() , vtx_baseline.end() ,
                          back_inserter(wall_electron_smeared_JESup) );
      build_w_candidates( mu_baseline_smeared.begin()  , mu_baseline_smeared.end()  , met_reffinal.begin() , met_reffinal.end() ,
                          jet_baseline_JESup_mu.begin() , jet_baseline_JESup_mu.end() , jet_baseline_mu_btag.begin() , jet_baseline_mu_btag.end() , vtx_baseline.begin() , vtx_baseline.end() ,
                          back_inserter(wall_muon_smeared_JESup) );
      // LESdown
      build_w_candidates( el_baseline_smeareddown.begin() , el_baseline_smeareddown.end() , met_reffinal.begin() , met_reffinal.end() ,
                          jet_baseline_el.begin() , jet_baseline_el.end() , jet_baseline_el_btag.begin() , jet_baseline_el_btag.end() , vtx_baseline.begin() , vtx_baseline.end() , back_inserter(wall_electron_smeareddown) );
      build_w_candidates( mu_baseline_msdown.begin()  , mu_baseline_msdown.end()  , met_reffinal.begin() , met_reffinal.end() ,
                          jet_baseline_mu.begin() , jet_baseline_mu.end() , jet_baseline_mu_btag.begin() , jet_baseline_mu_btag.end() , vtx_baseline.begin() , vtx_baseline.end() , back_inserter(wall_muon_msdown) );
      build_w_candidates( mu_baseline_iddown.begin()  , mu_baseline_iddown.end()  , met_reffinal.begin() , met_reffinal.end() ,
                          jet_baseline_mu.begin() , jet_baseline_mu.end() , jet_baseline_mu_btag.begin() , jet_baseline_mu_btag.end() , vtx_baseline.begin() , vtx_baseline.end() , back_inserter(wall_muon_iddown) );
      // LESup
      build_w_candidates( el_baseline_smearedup.begin() , el_baseline_smearedup.end() , met_reffinal.begin() , met_reffinal.end() ,
                          jet_baseline_el.begin() , jet_baseline_el.end() , jet_baseline_el_btag.begin() , jet_baseline_el_btag.end() , vtx_baseline.begin() , vtx_baseline.end() , back_inserter(wall_electron_smearedup) );
      build_w_candidates( mu_baseline_msup.begin()  , mu_baseline_msup.end()  , met_reffinal.begin() , met_reffinal.end() ,
                          jet_baseline_mu.begin() , jet_baseline_mu.end() , jet_baseline_mu_btag.begin() , jet_baseline_mu_btag.end() , vtx_baseline.begin() , vtx_baseline.end() , back_inserter(wall_muon_msup) );
      build_w_candidates( mu_baseline_idup.begin()  , mu_baseline_idup.end()  , met_reffinal.begin() , met_reffinal.end() ,
                          jet_baseline_mu.begin() , jet_baseline_mu.end() , jet_baseline_mu_btag.begin() , jet_baseline_mu_btag.end() , vtx_baseline.begin() , vtx_baseline.end() , back_inserter(wall_muon_idup) );
    }

    // build Z tag-and-probe event candidates
    if(mu_loose.size()<3) {
      build_z_candidates( mu_baseline.begin()  , mu_baseline.end() , met_reffinal.begin() , met_reffinal.end() ,
			  jet_baseline_mu.begin() , jet_baseline_mu.end() , vtx_baseline.begin() , vtx_baseline.end() , back_inserter(zall_muon) );
      build_z_candidates( mu_loose.begin()  , mu_loose.end() , met_reffinal.begin() , met_reffinal.end() ,
			  jet_baseline_mu_loose.begin() , jet_baseline_mu_loose.end() , vtx_baseline.begin() , vtx_baseline.end() , back_inserter(zall_muon_loose) );
    }

    // study W cuts
    if( studySelections ) {
      w_sel.for_each( wall_electron.begin() , wall_electron.end() );
      w_elqcd_sel.for_each( wall_elqcd.begin() , wall_elqcd.end() );
    }

    /* W SELECTION */
    // W candidates
    dgSTL::copy_if( wall_muon.begin() , wall_muon.end() , back_inserter(w_muon) , CUT_ALL(AnaWprimeCandidate, w_sel) );
    dgSTL::copy_if( wall_muon_qcd.begin() , wall_muon_qcd.end() , back_inserter(w_muon_qcd) , CUT_ALL(AnaWprimeCandidate, w_sel) );
    dgSTL::copy_if( wall_electron.begin() , wall_electron.end() , back_inserter(w_electron) , CUT_ALL(AnaWprimeCandidate, w_sel) );
    dgSTL::copy_if( wall_elqcd.begin() , wall_elqcd.end() , back_inserter(w_elqcd) , CUT_ALL(AnaWprimeCandidate, w_elqcd_sel) );
    if( is_mc ) {
      // make cuts for smeared / offset objects
      // standard smearing
      dgSTL::copy_if( wall_muon_smeared.begin() , wall_muon_smeared.end() , back_inserter(w_muon_smeared) , CUT_ALL(AnaWprimeCandidate, w_sel) );
      dgSTL::copy_if( wall_muon_smeared_qcd.begin() , wall_muon_smeared_qcd.end() , back_inserter(w_muon_smeared_qcd) , CUT_ALL(AnaWprimeCandidate, w_sel) );
      dgSTL::copy_if( wall_electron_smeared.begin() , wall_electron_smeared.end() , back_inserter(w_electron_smeared) , CUT_ALL(AnaWprimeCandidate, w_sel) );
      // JES/JER
      dgSTL::copy_if( wall_electron_smeared_JESdown.begin() , wall_electron_smeared_JESdown.end() ,
                      back_inserter(w_electron_smeared_JESdown) , CUT_ALL(AnaWprimeCandidate, w_sel) );
      dgSTL::copy_if( wall_electron_smeared_JESup.begin() , wall_electron_smeared_JESup.end() ,
                      back_inserter(w_electron_smeared_JESup) , CUT_ALL(AnaWprimeCandidate, w_sel) );
      dgSTL::copy_if( wall_electron_smeared_JER.begin() , wall_electron_smeared_JER.end() ,
                      back_inserter(w_electron_smeared_JER) , CUT_ALL(AnaWprimeCandidate, w_sel) );
      dgSTL::copy_if( wall_muon_smeared_JESdown.begin() , wall_muon_smeared_JESdown.end() ,
                      back_inserter(w_muon_smeared_JESdown) , CUT_ALL(AnaWprimeCandidate, w_sel) );
      dgSTL::copy_if( wall_muon_smeared_JESup.begin() , wall_muon_smeared_JESup.end() ,
                      back_inserter(w_muon_smeared_JESup) , CUT_ALL(AnaWprimeCandidate, w_sel) );
      dgSTL::copy_if( wall_muon_smeared_JER.begin() , wall_muon_smeared_JER.end() ,
                      back_inserter(w_muon_smeared_JER) , CUT_ALL(AnaWprimeCandidate, w_sel) );
      // LES
      dgSTL::copy_if( wall_electron_smeareddown.begin() , wall_electron_smeareddown.end() ,
                      back_inserter(w_electron_smeareddown) , CUT_ALL(AnaWprimeCandidate, w_sel) );
      dgSTL::copy_if( wall_electron_smearedup.begin() , wall_electron_smearedup.end() ,
                      back_inserter(w_electron_smearedup) , CUT_ALL(AnaWprimeCandidate, w_sel) );
      dgSTL::copy_if( wall_muon_msdown.begin() , wall_muon_msdown.end() ,
                      back_inserter(w_muon_msdown) , CUT_ALL(AnaWprimeCandidate, w_sel) );
      dgSTL::copy_if( wall_muon_msup.begin() , wall_muon_msup.end() ,
                      back_inserter(w_muon_msup) , CUT_ALL(AnaWprimeCandidate, w_sel) );
      dgSTL::copy_if( wall_muon_iddown.begin() , wall_muon_iddown.end() ,
                      back_inserter(w_muon_iddown) , CUT_ALL(AnaWprimeCandidate, w_sel) );
      dgSTL::copy_if( wall_muon_idup.begin() , wall_muon_idup.end() ,
                      back_inserter(w_muon_idup) , CUT_ALL(AnaWprimeCandidate, w_sel) );
    }
    // Z Candidates
    dgSTL::copy_if( zall_muon.begin() , zall_muon.end() , back_inserter(z_muon) , CUT_ALL(AnaZCrossSectionCandidate, z_sel) );
    dgSTL::copy_if( zall_muon_loose.begin() , zall_muon_loose.end() , back_inserter(z_muon_loose) , CUT_ALL(AnaZCrossSectionCandidate, z_sel) );
    
    // enforce the <= 1 lepton rules
    if( true ) {
      bool is_electron = w_electron.size() > 0;
      bool is_muon = w_muon.size() > 0;
      bool is_elqcd = w_elqcd.size() > 0;
      bool is_electron_smeared = w_electron_smeared.size() > 0;
      bool is_electron_smeareddown = w_electron_smeareddown.size() > 0;
      bool is_electron_smearedup = w_electron_smearedup.size() > 0;
      bool is_electron_smeared_JESdown = w_electron_smeared_JESdown.size() > 0;
      bool is_electron_smeared_JESup = w_electron_smeared_JESup.size() > 0;
      bool is_electron_smeared_JER = w_electron_smeared_JER.size() > 0;
      bool is_muon_smeared = w_muon_smeared.size() > 0;
      bool is_muon_msdown = w_muon_msdown.size() > 0;
      bool is_muon_msup = w_muon_msup.size() > 0;
      bool is_muon_iddown = w_muon_iddown.size() > 0;
      bool is_muon_idup = w_muon_idup.size() > 0;
      bool is_muon_smeared_JESdown = w_muon_smeared_JESdown.size() > 0;
      bool is_muon_smeared_JESup = w_muon_smeared_JESup.size() > 0;
      bool is_muon_smeared_JER = w_muon_smeared_JER.size() > 0;
      dg::down( "multi_lepton_events" , "keep track of events with more than one lepton" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      // electrons vs. elqcd
      if( is_electron && is_elqcd ) {
        dg::fillh( "n_electron_and_elqcd" , 1 , 1 , 2 , 1 );
        w_elqcd.clear();
      }
      // muons vs. elqcd
      if( is_muon && is_elqcd ) {
        dg::fillh( "n_muon_and_elqcd" , 1 , 1 , 2 , 1 );
        w_elqcd.clear();
      }
      // electrons vs. muons
      if( is_electron && is_muon ) {
        dg::fillh( "n_electron_and_muon" , 1 , 1 , 2 , 1 );
        w_electron.clear();
        w_muon.clear();
      }
      if( is_mc ) {
        // smeared electron vs. smeared muon
        if( is_electron_smeared && is_muon_smeared ) {
          dg::fillh( "n_electron_smeared_and_muon_smeared" , 1 , 1 , 2 , 1 );
          w_electron_smeared.clear();
          w_muon_smeared.clear();
        }
        // JES/JER
        if( is_electron_smeared_JESdown && is_muon_smeared_JESdown ) {
          dg::fillh( "n_electron_smeared_JESdown_and_muon_smeared_JESdown" , 1 , 1 , 2 , 1 );
          w_electron_smeared_JESdown.clear();
          w_muon_smeared_JESdown.clear();
        }
        if( is_electron_smeared_JESup && is_muon_smeared_JESup ) {
          dg::fillh( "n_electron_smeared_JESup_and_muon_smeared_JESup" , 1 , 1 , 2 , 1 );
          w_electron_smeared_JESup.clear();
          w_muon_smeared_JESup.clear();
        }
        if( is_electron_smeared_JER && is_muon_smeared_JER ) {
          dg::fillh( "n_electron_smeared_JER_and_muon_smeared_JER" , 1 , 1 , 2 , 1 );
          w_electron_smeared_JER.clear();
          w_muon_smeared_JER.clear();
        }
        // electron energy scale
        if( is_electron_smeareddown && is_muon_smeared ) {
          dg::fillh( "n_electron_smeareddown_and_muon_smeared" , 1 , 1 , 2 , 1 );
          w_electron_smeareddown.clear();
        }
        if( is_electron_smearedup && is_muon_smeared ) {
          dg::fillh( "n_electron_smearedup_and_muon_smeared" , 1 , 1 , 2 , 1 );
          w_electron_smearedup.clear();
        }
        // muon energy scale
        if( is_electron_smeared && is_muon_msdown ) {
          dg::fillh( "n_electron_smeared_muon_msdown" , 1 , 1 , 2 , 1 );
          w_muon_msdown.clear();
        }
        if( is_electron_smeared && is_muon_msup ) {
          dg::fillh( "n_electron_smeared_muon_msup" , 1 , 1 , 2 , 1 );
          w_muon_msup.clear();
        }
        if( is_electron_smeared && is_muon_iddown ) {
          dg::fillh( "n_electron_smeared_muon_iddown" , 1 , 1 , 2 , 1 );
          w_muon_iddown.clear();
        }
        if( is_electron_smeared && is_muon_idup ) {
          dg::fillh( "n_electron_smeared_muon_idup" , 1 , 1 , 2 , 1 );
          w_muon_idup.clear();
        }
      }
    }

    // enforce the <= 2 lepton rules for Z's
    if( true ) {
      bool is_electron = z_electron.size() > 0;
      bool is_muon = z_muon.size() > 0;
      bool is_muon_loose = z_muon_loose.size() > 0;
      dg::down( "z_events" , "keep track of events with more than one lepton" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      // electrons vs. muons
      if( is_electron && is_muon ) {
	dg::fillh( "n_electron_and_muon" , 1 , 1 , 2 , 1 );
	z_electron.clear();
        z_muon.clear();
      }
      if( is_electron && is_muon_loose ) {
	dg::fillh( "n_electron_and_muon_loose" , 1 , 1 , 2 , 1 );
        z_electron.clear();
	z_muon_loose.clear();
      }
    }
 
    if( true ) {
      dg::down( "particle_counts" , "" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      // number of W events selected
      dg::fillh( "n_Welec" , 1 , 1 , 2 , w_electron.size() );
      dg::fillh( "n_Wall_elec" , 1 , 1 , 2 , wall_electron.size() );
      dg::fillh( "n_Wmuon" , 1 , 1 , 2 , w_muon.size() );
      dg::fillh( "n_Welqcd" , 1 , 1 , 2 , w_elqcd.size() );
    }

    // ======================= 0's cutflows ============================
    float eff_nobtag_el=0;
    float eff_el=0;
    float eff_nobtag_mu=0;
    float eff_mu=0;
    if(true) {
      StudyWprime stw_el( "wprime_candidates_electron", "");
      stw_el.do_heavyQ_studies( true );
      stw_el.do_2d_plots( false );
      stw_el.do_qcd_2d( false );
      stw_el.do_save_ntuple( false );
      stw_el.add_category( "wmt" , bind(&AnaWprimeCandidate::wmt,_1) );
      stw_el.add_category( "baseline" , bind(&AnaWprimeCandidate::baseline,_1) );
      stw_el.add_category( "baseline_jets" , bind(&AnaWprimeCandidate::baseline_jets,_1) );
      stw_el.add_category( "baseline_1btag_jets" , bind(&AnaWprimeCandidate::baseline_1btag_jets,_1) );
      stw_el.add_category( "baseline_2btag_jets" , bind(&AnaWprimeCandidate::baseline_2btag_jets,_1) );
      stw_el.add_category( "baseline_btag_jets" , bind(&AnaWprimeCandidate::baseline_btag_jets,_1) );
      stw_el.add_category( "baseline_2jets" , bind(&AnaWprimeCandidate::baseline_2jets,_1) );
      stw_el.add_category( "baseline_1btag_2jets" , bind(&AnaWprimeCandidate::baseline_1btag_2jets,_1) );
      stw_el.add_category( "baseline_2btag_2jets" , bind(&AnaWprimeCandidate::baseline_2btag_2jets,_1) );
      stw_el.add_category( "metfit_CR" , bind(&AnaWprimeCandidate::metfit_CR,_1) );
      stw_el.add_category( "wmt_met" , bind(&AnaWprimeCandidate::wmt_met,_1) );
      stw_el.add_category( "metfit_CR_1btag" , bind(&AnaWprimeCandidate::metfit_CR_1btag,_1) );
      stw_el.add_category( "wmt_met_1btag" , bind(&AnaWprimeCandidate::wmt_met_1btag,_1) );
      stw_el.add_category( "metfit_CR_2btag" , bind(&AnaWprimeCandidate::metfit_CR_2btag,_1) );
      stw_el.add_category( "wmt_met_2btag" , bind(&AnaWprimeCandidate::wmt_met_2btag,_1) );


      StudyWprime stw_mu( "wprime_candidates_muon", "");
      stw_mu.do_heavyQ_studies( true );
      stw_mu.do_2d_plots( false );
      stw_mu.do_qcd_2d(true);
      stw_mu.do_save_ntuple(false);
      stw_mu.add_category( "wmt" , bind(&AnaWprimeCandidate::wmt,_1) );
      stw_mu.add_category( "baseline" , bind(&AnaWprimeCandidate::baseline,_1) );
      stw_mu.add_category( "baseline_jets" , bind(&AnaWprimeCandidate::baseline_jets,_1) );
      stw_mu.add_category( "baseline_1btag_jets" , bind(&AnaWprimeCandidate::baseline_1btag_jets,_1) );
      stw_mu.add_category( "baseline_2btag_jets" , bind(&AnaWprimeCandidate::baseline_2btag_jets,_1) );
      stw_mu.add_category( "baseline_2jets" , bind(&AnaWprimeCandidate::baseline_2jets,_1) );
      stw_mu.add_category( "baseline_1btag_2jets" , bind(&AnaWprimeCandidate::baseline_1btag_2jets,_1) );
      stw_mu.add_category( "baseline_2btag_2jets" , bind(&AnaWprimeCandidate::baseline_2btag_2jets,_1) );
      stw_mu.add_category( "events_jets" , bind(&AnaWprimeCandidate::events_jets,_1) );
      stw_mu.add_category( "events_2jets" , bind(&AnaWprimeCandidate::events_2jets,_1) );
      stw_mu.add_category( "qcd_fake" , bind(&AnaWprimeCandidate::qcd_fake,_1) );
      stw_mu.add_category( "qcd_fake_jets" , bind(&AnaWprimeCandidate::qcd_fake_jets,_1) );
      stw_mu.add_category( "qcd_fake_2jets" , bind(&AnaWprimeCandidate::qcd_fake_2jets,_1) );
      stw_mu.add_category( "metfit_CR" , bind(&AnaWprimeCandidate::metfit_CR,_1) );
      stw_mu.add_category( "metfit_CR_1btag" , bind(&AnaWprimeCandidate::metfit_CR_1btag,_1) );
      stw_mu.add_category( "metfit_CR_2btag" , bind(&AnaWprimeCandidate::metfit_CR_2btag,_1) );


      StudyWprime stw_mu_loose( "wprime_candidates_muon_loose", "");
      stw_mu_loose.do_heavyQ_studies( true );
      stw_mu_loose.do_2d_plots( false );
      stw_mu_loose.do_qcd_2d(true);
      stw_mu_loose.do_save_ntuple(false);
      stw_mu_loose.add_category( "baseline" , bind(&AnaWprimeCandidate::baseline,_1) );
      stw_mu_loose.add_category( "baseline_jets" , bind(&AnaWprimeCandidate::baseline_jets,_1) );
      stw_mu_loose.add_category( "baseline_1btag_jets" , bind(&AnaWprimeCandidate::baseline_1btag_jets,_1) );
      stw_mu_loose.add_category( "baseline_2btag_jets" , bind(&AnaWprimeCandidate::baseline_2btag_jets,_1) );
      stw_mu_loose.add_category( "baseline_2jets" , bind(&AnaWprimeCandidate::baseline_2jets,_1) );
      stw_mu_loose.add_category( "baseline_1btag_2jets" , bind(&AnaWprimeCandidate::baseline_1btag_2jets,_1) );
      stw_mu_loose.add_category( "baseline_2btag_2jets" , bind(&AnaWprimeCandidate::baseline_2btag_2jets,_1) );
      stw_mu_loose.add_category( "events_jets" , bind(&AnaWprimeCandidate::events_jets,_1) );
      stw_mu_loose.add_category( "events_2jets" , bind(&AnaWprimeCandidate::events_2jets,_1) );
      stw_mu_loose.add_category( "qcd_fake" , bind(&AnaWprimeCandidate::qcd_fake,_1) );
      stw_mu_loose.add_category( "qcd_fake_jets" , bind(&AnaWprimeCandidate::qcd_fake_jets,_1) );
      stw_mu_loose.add_category( "qcd_fake_2jets" , bind(&AnaWprimeCandidate::qcd_fake_2jets,_1) );
      stw_mu_loose.add_category( "metfit_CR" , bind(&AnaWprimeCandidate::metfit_CR,_1) );
      stw_mu_loose.add_category( "metfit_CR_1btag" , bind(&AnaWprimeCandidate::metfit_CR_1btag,_1) );
      stw_mu_loose.add_category( "metfit_CR_2btag" , bind(&AnaWprimeCandidate::metfit_CR_2btag,_1) );


      StudyZCrossSection stz( "z_tag-and-probe_candidates" , "" );
      stz.do_heavyQ_studies( false );
      stz.do_2d_plots( false );
      stz.do_tag_and_probe_studies( true );
      stz.do_save_0ntuple(false);
      stz.add_category( "opposite_signs" , bind(&AnaZCrossSectionCandidate::exactly_two_opposite_leptons,_1) );
      stz.add_category( "opposite_signs_jets" , bind(&AnaZCrossSectionCandidate::opposite_signs_jets,_1) );

      StudyZCrossSection stz_loose( "z_tag-and-probe_candidates_loose" , "" );
      stz_loose.do_heavyQ_studies( false );
      stz_loose.do_2d_plots( false );
      stz_loose.do_tag_and_probe_studies( true );
      stz_loose.do_save_0ntuple(false);
      stz_loose.add_category( "opposite_signs" , bind(&AnaZCrossSectionCandidate::exactly_two_opposite_leptons,_1) );
      stz_loose.add_category( "opposite_signs_jets" , bind(&AnaZCrossSectionCandidate::opposite_signs_jets,_1) );

      if( is_mc ) {
	if( AnaConfiguration::do_hfor()==true ) {
	  if( hfor_type==0 || hfor_type==1 ) {
	    dg::down( "Wbb_Wcc" , "" );
	    stw_el.repurpose( "wprime_electron_smeared", "");
	    stw_el.for_each( w_electron_smeared.begin(), w_electron_smeared.end() );
	    stw_mu.repurpose( "wprime_muon_smeared", "");
	    stw_mu.for_each( w_muon_smeared.begin(), w_muon_smeared.end() );
	    stw_mu_loose.repurpose( "wprime_muon_smeared_loose", "");
	    stw_mu_loose.for_each( w_muon_smeared.begin() , w_muon_smeared.end() );
	    stw_mu_loose.for_each( w_muon_smeared_qcd.begin() , w_muon_smeared_qcd.end() );
	    dg::up();
	  } else if( hfor_type==2 ) {
	    dg::down( "Wc" , "" );
	    stw_el.repurpose( "wprime_electron_smeared", "");
	    stw_el.for_each( w_electron_smeared.begin(), w_electron_smeared.end() );
	    stw_mu.repurpose( "wprime_muon_smeared", "");
	    stw_mu.for_each( w_muon_smeared.begin(), w_muon_smeared.end() );
	    stw_mu_loose.repurpose( "wprime_muon_smeared_loose", "");
	    stw_mu_loose.for_each( w_muon_smeared.begin() , w_muon_smeared.end() );
	    stw_mu_loose.for_each( w_muon_smeared_qcd.begin() , w_muon_smeared_qcd.end() );
	    dg::up();
	  } else { // hfor_type==3
	    dg::down( "W_light" , "" );
	    stw_el.repurpose( "wprime_electron_smeared", "");
	    stw_el.for_each( w_electron_smeared.begin(), w_electron_smeared.end() );
	    stw_mu.repurpose( "wprime_muon_smeared", "");
	    stw_mu.for_each( w_muon_smeared.begin(), w_muon_smeared.end() );
	    stw_mu_loose.repurpose( "wprime_muon_smeared_loose", "");
	    stw_mu_loose.for_each( w_muon_smeared.begin() , w_muon_smeared.end() );
	    stw_mu_loose.for_each( w_muon_smeared_qcd.begin() , w_muon_smeared_qcd.end() );
	    dg::up();
	  }
	} else {
	  stw_el.repurpose( "wprime_electron_smeared", "");
	  stw_el.for_each( w_electron_smeared.begin(), w_electron_smeared.end() );
	  stw_mu.repurpose( "wprime_muon_smeared", "");
	  stw_mu.for_each( w_muon_smeared.begin(), w_muon_smeared.end() );
	  stw_mu_loose.repurpose( "wprime_muon_smeared_loose", "");
	  stw_mu_loose.for_each( w_muon_smeared.begin() , w_muon_smeared.end() );
	  stw_mu_loose.for_each( w_muon_smeared_qcd.begin() , w_muon_smeared_qcd.end() );
	}
      }
      else {
	stw_el.repurpose( "wprime_electron", "");
	stw_el.for_each( w_electron.begin() , w_electron.end() );
	
	stw_mu.repurpose( "wprime_muon", "");
	stw_mu.for_each( w_muon.begin() , w_muon.end() );
	stw_mu_loose.repurpose( "wprime_muon_loose", "");
	stw_mu_loose.for_each( w_muon.begin() , w_muon.end() );
	stw_mu_loose.for_each( w_muon_qcd.begin() , w_muon_qcd.end() );
	
	stw_el.repurpose( "wprime_elqcd" , "" );
	stw_el.for_each( w_elqcd.begin() , w_elqcd.end() );
	stz.repurpose( "z_muon_tag-and-probe", "");
	stz.for_each( z_muon.begin() , z_muon.end() );
	stz_loose.repurpose( "z_muon_tag-and-probe_loose", "");
	stz_loose.for_each( z_muon_loose.begin() , z_muon_loose.end() );
      }
    }
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
			 jetIterT bjet_begin , jetIterT bjet_end ,
                         vtxIterT vtx_begin , vtxIterT vtx_end ,
                         outputIterT output_iterator )
{
  boost::shared_ptr<AnaWprimeCandidate> cand( AnaWprimeCandidate::build( lep_begin , lep_end ,
									 met_begin , met_end ,
									 jet_begin , jet_end ,
									 bjet_begin , bjet_end ,
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
void overlap_removal_loose( overlapIterT overlap_begin , overlapIterT overlap_end ,
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

template< typename overlapIterT , typename refIterT , typename outputIterT >
void overlap_removal_tight( overlapIterT overlap_begin , overlapIterT overlap_end ,
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
    while( !overlap_coll.empty() ) {
      if( overlap_coll.back()->dist_eta_phi( ref_particle ) <= max_overlap_dr ) {
        overlap_coll.pop_back();
        --overlap_coll_size;
      } else break;
    }
  }
  // the objects in overlap_coll have survived the overlap
  // removal. copy them to the output.
  std::copy( overlap_coll.begin() , overlap_coll.end() , output_iterator );
}

const double jet_drmin( boost::shared_ptr<const AnaJet>& jet , std::vector< boost::shared_ptr<const AnaJet> >& jetcoll )
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

void mu_eff_scale( EventObjectColl< boost::shared_ptr<const AnaMuon> >& mucoll , const unsigned long& run_number , std::vector<double>& int_lumi ,
                   double& eff_weight , double& eff_stat_error , double& eff_sys_error )
{
  eff_weight = 1.;
  eff_stat_error = 0.;
  eff_sys_error = 0.;
  for( std::vector< shared_ptr<const AnaMuon> >::const_iterator imu = mucoll.begin() ; imu != mucoll.end() ; ++imu ) {
    const shared_ptr<const AnaMuon> mu(*imu);
    double wt , stat_err , sys_err;
    AnaMuon::mcp_effscale( AnaConfiguration::conf() , AnaConfiguration::data_range() , mu , mu_type , run_number , int_lumi , wt , stat_err , sys_err );
    eff_weight *= wt;
    eff_stat_error += pow(stat_err/wt,2);
    eff_sys_error += pow(sys_err/wt,2);
  }
  eff_stat_error = sqrt(eff_stat_error);
  eff_sys_error = sqrt(eff_sys_error);
}

void el_eff_scale( EventObjectColl< boost::shared_ptr<const AnaElectron> >& elcoll , float &eff , float& error )
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

void mu_trig_scale( EventObjectColl< boost::shared_ptr<const AnaMuon> >& mucoll , const detector::EGAMMA_TYPE& el_type , const unsigned long& run_number ,
                    double& trig_weight , double& trig_stat_error )
{
  std::vector<TLorentzVector> velectrons; // should be empty
  std::vector<TLorentzVector> vmuons;
  for( std::vector< shared_ptr<const AnaMuon> >::const_iterator imu = mucoll.begin() ; imu != mucoll.end() ; ++imu ) {
    vmuons.push_back( (*imu)->four_vector() );
  }
  AnaMuon::GetTriggerSF_v17( AnaConfiguration::conf() , mu_type , el_type , vmuons , velectrons , run_number , 0 , trig_weight , trig_stat_error );
}

void el_trig_scale( EventObjectColl< boost::shared_ptr<const AnaElectron> >& elcoll , const detector::EGAMMA_TYPE& el_type , const unsigned long& run_number ,
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

#ifdef F77_DUMMY_MAIN
#ifdef __cplusplus
extern "C"
#endif
int F77_DUMMY_MAIN() { abort(); return 1; }
#endif

