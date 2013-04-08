
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

  AnaConfiguration::configure("ana_pgs",argc,argv);
  if( !AnaConfiguration::configured() ) { exit(0); }
  dg::root()->no_save_at_exit();
  BOOST_SCOPE_EXIT() { dg::root()->do_save_at_exit(); } BOOST_SCOPE_EXIT_END;

  // event loop
  // ================================================================
  unsigned long n_events = 0ul;

  AnaEventMgr* evtmgr = AnaEventMgr::instance(); assert(evtmgr);
  for( AnaEventMgr::iterator i=evtmgr->begin(), f=evtmgr->end(AnaConfiguration::user_max_nevents()); i!=f; ++i, ++n_events ) {

    const shared_ptr<const AnaEvent>& evt( *i );
    if( !evt ) { break; } // no AnaEvent means either we're out of events or something went wrong, e.g. premature end of file.
    
    shared_ptr<const AnaMET> met;
    if( !(evt->missing_et().empty()) ) { met = evt->missing_et().back(); }
    dg::filleff( "has_met" , !!met );
    if( !met ) { continue; }

    dg::fillh( "nele" , 10 , 0 , 10 , evt->electrons().size() );
    dg::fillh( "nmu" , 10 , 0 , 10 , evt->muons().size() );
    dg::fillh( "njet" , 30 , 0 , 30 , evt->pgs_jets().size() );
    dg::fillh( "met" , 200 , 0 , 2000 , met->pt() );


    // implement lian-tao's cuts
    const bool passes_met = met->pt() > 100.;
    const unsigned int njets50 = std::count_if( evt->pgs_jets().begin() , evt->pgs_jets().end() , bind( &AnaParticle::pt , _1 ) >= 50. && bind(&AnaParticle::abseta,_1)<2.5 );
    const unsigned int njets20 = std::count_if( evt->pgs_jets().begin() , evt->pgs_jets().end() , bind( &AnaParticle::pt , _1 ) >= 20. && bind(&AnaParticle::abseta,_1)<2.5 );
    const unsigned int nele20 = std::count_if( evt->electrons().begin() , evt->electrons().end() , bind( &AnaParticle::pt , _1 ) >= 20. && bind(&AnaParticle::abseta,_1)<2.5  );
    const unsigned int nmu20 = std::count_if( evt->muons().begin() , evt->muons().end() , bind( &AnaParticle::pt , _1 ) >= 20. && bind(&AnaParticle::abseta,_1)<2.5 );
    const bool has_njet50 = njets50>=4;
    const unsigned int ntags = std::count_if( evt->pgs_jets().begin() , evt->pgs_jets().end() , 
                                              bind( &AnaParticle::pt , _1 ) >= 20. && bind( &AnaJet::tag_pgs , _1 ) > 0. && bind(&AnaParticle::abseta,_1)<2.5  );
    // calculate lepton charge product, event ht, pt of highest pt lepton, etc.
    float max_lepton_pt = 0.;
    double ht = 0.;
    double lepton_charge_product = 1.;
    BOOST_FOREACH( const shared_ptr<const AnaElectron>& ele , evt->electrons() ) { 
      if( ele->pt() < 20. || ele->abseta()>=2.5) { continue; }
      lepton_charge_product *= ( ele->charge()!=0 ? ele->charge() : 2 ); 
      max_lepton_pt = std::max( max_lepton_pt , ele->pt() );
      ht += ele->pt();
    }
    BOOST_FOREACH( const shared_ptr<const AnaMuon>& mu , evt->muons() ) { 
      if( mu->pt() < 20. || mu->abseta()>=2.5) { continue; }
      lepton_charge_product *= ( mu->charge()!=0 ? mu->charge() : 2 ); 
      max_lepton_pt = std::max( max_lepton_pt , mu->pt() );
      ht += mu->pt();
    }
    BOOST_FOREACH( const shared_ptr<const AnaJet>& jet , evt->pgs_jets() ) { 
      if( jet->pt() < 20. || jet->abseta()>=2.5) { continue; }
      ht += jet->pt();
    }

    dg::filleff( "passes_met" , passes_met );
    dg::filleff( "has_njets50" , has_njet50 );
    dg::filleff( "passes_met_and_has_njet50" , passes_met && has_njet50 );
    dg::fillh( "ntags" , 10 , 0 , 10 , ntags );
    dg::fillh( "max_lepton_pt" , 100 , 0 , 1000 , max_lepton_pt );
    dg::fillh( "ht" , 200 , 0 , 2000 , ht );      

    if( !passes_met ) { continue; }
    if( !has_njet50 ) { continue; }

    dg::fillh( "selected_ntags" , 10 , 0 , 10 , ntags );
    dg::fillh( "selected_met" , 100 , 0 , 1000 , met->pt() );
    dg::fillh( "selected_njets50" , 20 , 0 , 20 , njets50 );
    dg::fillh( "selected_njets20" , 20 , 0 , 20 , njets20 );
    dg::fillh( "selected_ntags" , 20 , 0 , 20 , ntags );
    dg::fillh( "selected_max_lepton_pt" , 100 , 0 , 1000 , max_lepton_pt );
    dg::fillh( "selected_ht" , 200 , 0 , 2000 , ht );      

    const bool is_selected_2b_1L =  ntags==2 && (nele20+nmu20)==1 ;
    dg::filleff( "selected_2b_1L" , ntags==2 && (nele20+nmu20)==1 );
    if( is_selected_2b_1L ) {
      dg::down( "for_selected_2b_1L" , "" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      dg::fillh( "met" , 200 , 0 , 2000 , met->pt() );
      dg::fillh( "nele20" , 20 , 0 , 20 , nele20 );
      dg::fillh( "nmu20" , 20 , 0 , 20 , nmu20 );
      dg::fillh( "nlep20" , 20 , 0 , 20 , nele20 + nmu20 );
      dg::fillh( "njets20" , 20 , 0 , 20 , njets20 );
      dg::fillh( "ntags" , 20 , 0 , 20 , ntags );
      dg::fillh( "max_lepton_pt" , 100 , 0 , 1000 , max_lepton_pt );
      dg::fillh( "ht" , 200 , 0 , 2000 , ht );
    }
    const bool is_selected_3b_1L =  ntags==3 && (nele20+nmu20)==1 ;
    dg::filleff( "selected_3b_1L" , ntags==3 && (nele20+nmu20)==1 );
    if( is_selected_3b_1L ) {
      dg::down( "for_selected_3b_1L" , "" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      dg::fillh( "met" , 200 , 0 , 2000 , met->pt() );
      dg::fillh( "nele20" , 20 , 0 , 20 , nele20 );
      dg::fillh( "nmu20" , 20 , 0 , 20 , nmu20 );
      dg::fillh( "nlep20" , 20 , 0 , 20 , nele20 + nmu20 );
      dg::fillh( "njets20" , 20 , 0 , 20 , njets20 );
      dg::fillh( "ntags" , 20 , 0 , 20 , ntags );
      dg::fillh( "max_lepton_pt" , 100 , 0 , 1000 , max_lepton_pt );
      dg::fillh( "ht" , 200 , 0 , 2000 , ht );      
    }
    const bool is_selected_ge4b_1L =  ntags>=4 && (nele20+nmu20)==1 ;
    dg::filleff( "selected_ge4b_1L" , ntags>=4 && (nele20+nmu20)==1 );
    if( is_selected_ge4b_1L ) {
      dg::down( "for_selected_ge4b_1L" , "" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      dg::fillh( "met" , 200 , 0 , 2000 , met->pt() );
      dg::fillh( "nele20" , 20 , 0 , 20 , nele20 );
      dg::fillh( "nmu20" , 20 , 0 , 20 , nmu20 );
      dg::fillh( "nlep20" , 20 , 0 , 20 , nele20 + nmu20 );
      dg::fillh( "njets20" , 20 , 0 , 20 , njets20 );
      dg::fillh( "ntags" , 20 , 0 , 20 , ntags );
      dg::fillh( "max_lepton_pt" , 100 , 0 , 1000 , max_lepton_pt );
      dg::fillh( "ht" , 200 , 0 , 2000 , ht );      
    }

    const bool is_selected_2b_OS =  ntags==2 && (nele20+nmu20)==2 && lepton_charge_product<0. ;
    dg::filleff( "selected_2b_OS" , ntags==2 && (nele20+nmu20)==2 && lepton_charge_product<0. );
    if( is_selected_2b_OS ) {
      dg::down( "for_selected_2b_OS" , "" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      dg::fillh( "met" , 200 , 0 , 2000 , met->pt() );
      dg::fillh( "nele20" , 20 , 0 , 20 , nele20 );
      dg::fillh( "nmu20" , 20 , 0 , 20 , nmu20 );
      dg::fillh( "nlep20" , 20 , 0 , 20 , nele20 + nmu20 );
      dg::fillh( "njets20" , 20 , 0 , 20 , njets20 );
      dg::fillh( "ntags" , 20 , 0 , 20 , ntags );
      dg::fillh( "max_lepton_pt" , 100 , 0 , 1000 , max_lepton_pt );
      dg::fillh( "ht" , 200 , 0 , 2000 , ht );      
    }
    const bool is_selected_3b_OS =  ntags==3 && (nele20+nmu20)==2 && lepton_charge_product<0. ;
    dg::filleff( "selected_3b_OS" , ntags==3 && (nele20+nmu20)==2 && lepton_charge_product<0. );
    if( is_selected_3b_OS ) {
      dg::down( "for_selected_3b_OS" , "" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      dg::fillh( "met" , 200 , 0 , 2000 , met->pt() );
      dg::fillh( "nele20" , 20 , 0 , 20 , nele20 );
      dg::fillh( "nmu20" , 20 , 0 , 20 , nmu20 );
      dg::fillh( "nlep20" , 20 , 0 , 20 , nele20 + nmu20 );
      dg::fillh( "njets20" , 20 , 0 , 20 , njets20 );
      dg::fillh( "ntags" , 20 , 0 , 20 , ntags );
      dg::fillh( "max_lepton_pt" , 100 , 0 , 1000 , max_lepton_pt );
      dg::fillh( "ht" , 200 , 0 , 2000 , ht );      
    }
    const bool is_selected_ge4b_OS =  ntags>=4 && (nele20+nmu20)==2 && lepton_charge_product<0. ;
    dg::filleff( "selected_ge4b_OS" , ntags>=4 && (nele20+nmu20)==2 && lepton_charge_product<0. );
    if( is_selected_ge4b_OS ) {
      dg::down( "for_selected_ge4b_OS" , "" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      dg::fillh( "met" , 200 , 0 , 2000 , met->pt() );
      dg::fillh( "nele20" , 20 , 0 , 20 , nele20 );
      dg::fillh( "nmu20" , 20 , 0 , 20 , nmu20 );
      dg::fillh( "nlep20" , 20 , 0 , 20 , nele20 + nmu20 );
      dg::fillh( "njets20" , 20 , 0 , 20 , njets20 );
      dg::fillh( "ntags" , 20 , 0 , 20 , ntags );
      dg::fillh( "max_lepton_pt" , 100 , 0 , 1000 , max_lepton_pt );
      dg::fillh( "ht" , 200 , 0 , 2000 , ht );      
    }

    const bool is_selected_2b_SS =  ntags==2 && (nele20+nmu20)==2 && lepton_charge_product>0. ;
    dg::filleff( "selected_2b_SS" , ntags==2 && (nele20+nmu20)==2 && lepton_charge_product>0. );
    if( is_selected_2b_SS ) {
      dg::down( "for_selected_2b_SS" , "" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      dg::fillh( "met" , 200 , 0 , 2000 , met->pt() );
      dg::fillh( "nele20" , 20 , 0 , 20 , nele20 );
      dg::fillh( "nmu20" , 20 , 0 , 20 , nmu20 );
      dg::fillh( "nlep20" , 20 , 0 , 20 , nele20 + nmu20 );
      dg::fillh( "njets20" , 20 , 0 , 20 , njets20 );
      dg::fillh( "ntags" , 20 , 0 , 20 , ntags );
      dg::fillh( "max_lepton_pt" , 100 , 0 , 1000 , max_lepton_pt );
      dg::fillh( "ht" , 200 , 0 , 2000 , ht );      
    }
    const bool is_selected_3b_SS =  ntags==3 && (nele20+nmu20)==2 && lepton_charge_product>0. ;
    dg::filleff( "selected_3b_SS" , ntags==3 && (nele20+nmu20)==2 && lepton_charge_product>0. );
    if( is_selected_3b_SS ) {
      dg::down( "for_selected_3b_SS" , "" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      dg::fillh( "met" , 200 , 0 , 2000 , met->pt() );
      dg::fillh( "nele20" , 20 , 0 , 20 , nele20 );
      dg::fillh( "nmu20" , 20 , 0 , 20 , nmu20 );
      dg::fillh( "nlep20" , 20 , 0 , 20 , nele20 + nmu20 );
      dg::fillh( "njets20" , 20 , 0 , 20 , njets20 );
      dg::fillh( "ntags" , 20 , 0 , 20 , ntags );
      dg::fillh( "max_lepton_pt" , 100 , 0 , 1000 , max_lepton_pt );
      dg::fillh( "ht" , 200 , 0 , 2000 , ht );      
    }
    const bool is_selected_ge4b_SS =  ntags>=4 && (nele20+nmu20)==2 && lepton_charge_product>0. ;
    dg::filleff( "selected_ge4b_SS" , ntags>=4 && (nele20+nmu20)==2 && lepton_charge_product>0. );
    if( is_selected_ge4b_SS ) {
      dg::down( "for_selected_ge4b_SS" , "" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      dg::fillh( "met" , 200 , 0 , 2000 , met->pt() );
      dg::fillh( "nele20" , 20 , 0 , 20 , nele20 );
      dg::fillh( "nmu20" , 20 , 0 , 20 , nmu20 );
      dg::fillh( "nlep20" , 20 , 0 , 20 , nele20 + nmu20 );
      dg::fillh( "njets20" , 20 , 0 , 20 , njets20 );
      dg::fillh( "ntags" , 20 , 0 , 20 , ntags );
      dg::fillh( "max_lepton_pt" , 100 , 0 , 1000 , max_lepton_pt );
      dg::fillh( "ht" , 200 , 0 , 2000 , ht );      
    }

    const bool is_selected_2b_3L =  ntags==2 && (nele20+nmu20)==3 ;
    dg::filleff( "selected_2b_3L" , ntags==2 && (nele20+nmu20)==3 );
    if( is_selected_2b_3L ) {
      dg::down( "for_selected_2b_3L" , "" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      dg::fillh( "met" , 200 , 0 , 2000 , met->pt() );
      dg::fillh( "nele20" , 20 , 0 , 20 , nele20 );
      dg::fillh( "nmu20" , 20 , 0 , 20 , nmu20 );
      dg::fillh( "nlep20" , 20 , 0 , 20 , nele20 + nmu20 );
      dg::fillh( "njets20" , 20 , 0 , 20 , njets20 );
      dg::fillh( "ntags" , 20 , 0 , 20 , ntags );
      dg::fillh( "max_lepton_pt" , 100 , 0 , 1000 , max_lepton_pt );
      dg::fillh( "ht" , 200 , 0 , 2000 , ht );      
    }
    const bool is_selected_3b_3L =  ntags==3 && (nele20+nmu20)==3 ;
    dg::filleff( "selected_3b_3L" , ntags==3 && (nele20+nmu20)==3 );
    if( is_selected_3b_3L ) {
      dg::down( "for_selected_4b_3L" , "" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      dg::fillh( "met" , 200 , 0 , 2000 , met->pt() );
      dg::fillh( "nele20" , 20 , 0 , 20 , nele20 );
      dg::fillh( "nmu20" , 20 , 0 , 20 , nmu20 );
      dg::fillh( "nlep20" , 20 , 0 , 20 , nele20 + nmu20 );
      dg::fillh( "njets20" , 20 , 0 , 20 , njets20 );
      dg::fillh( "ntags" , 20 , 0 , 20 , ntags );
      dg::fillh( "max_lepton_pt" , 100 , 0 , 1000 , max_lepton_pt );
      dg::fillh( "ht" , 200 , 0 , 2000 , ht );      
    }
    const bool is_selected_ge4b_3L =  ntags>=4 && (nele20+nmu20)==3 ;
    dg::filleff( "selected_ge4b_3L" , ntags>=4 && (nele20+nmu20)==3 );
    if( is_selected_ge4b_3L ) {
      dg::down( "for_selected_4b_3L" , "" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      dg::fillh( "met" , 200 , 0 , 2000 , met->pt() );
      dg::fillh( "nele20" , 20 , 0 , 20 , nele20 );
      dg::fillh( "nmu20" , 20 , 0 , 20 , nmu20 );
      dg::fillh( "nlep20" , 20 , 0 , 20 , nele20 + nmu20 );
      dg::fillh( "njets20" , 20 , 0 , 20 , njets20 );
      dg::fillh( "ntags" , 20 , 0 , 20 , ntags );
      dg::fillh( "max_lepton_pt" , 100 , 0 , 1000 , max_lepton_pt );
      dg::fillh( "ht" , 200 , 0 , 2000 , ht );      
    }

    
    // // skim events with good W to output ntuple.
    // if( AnaEventMgr::instance()->ana_event_ntuple_mode() == AnaEventNtuple::WRITE ) {
    //   AnaEventMgr::instance()->add_to_ana_event_ntuple( !w_top.empty() );
    // }

  } // end for each event

  AnaEventMgr::instance()->close_sample();
  
}

#ifdef F77_DUMMY_MAIN
#ifdef __cplusplus
extern "C"
#endif
int F77_DUMMY_MAIN() { abort(); return 1; }
#endif
