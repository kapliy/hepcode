
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
#include "TrigFTKAna/AnaJetTrackAssociation.hpp"
#include "TrigFTKAna/AnaMET.hpp"
#include "TrigFTKAna/AnaW.hpp"
#include "TrigFTKAna/EventObjectColl.hpp"
#include "TrigFTKAna/GoodRunList.hpp"
#include "TrigFTKAna/CompositeGenerator.hpp"
#include "TrigFTKAna/StudySemileptonicW.hpp"
#include "TrigFTKAna/StudyJet.hpp"
#include "TrigFTKAna/StudyJetTag.hpp"

using namespace std;
using namespace DataGraphics;
using namespace boost;
using namespace boost::assign;
using dgSTL::copy_if;

////////////////////////////////////////////////////////////////

TRIGFTKANA_MAIN( int argc , char* argv[] )
{

  ANA_CONFIGURE( "ana_vtx" );

  // define object selection
  // ================================================================

  EventObjectColl< shared_ptr<const AnaJet> > taggable_jets;  // taggable jet selection
  DgCutSet< shared_ptr<const AnaJet> > taggable_jet_selector( "taggable_jet_selector" , taggable_jets.attach() );
  StudyJet st_jet( "jet_properties" , "jet properties and composition study" );
  taggable_jet_selector.add_uu( "algorithm_antikt4" , bind(&AnaJet::is_atlas_anti_kt_cone_4_h1_topo,_1)==true ,
                                 bind(&dg::filleff,_1,bind(&AnaJet::is_atlas_anti_kt_cone_4_h1_topo,_2)==true) );
  taggable_jet_selector.add_uu( "eta" , bind( std::ptr_fun<AnaJet::ftype,AnaJet::ftype>(std::abs) , bind( &AnaParticle::eta , _1 ) ) < 0.8 ,
                                bind( dg::fillh , _1 , 80 , -4 , 4 , bind( std::ptr_fun<AnaJet::ftype,AnaJet::ftype>(std::abs) , bind( &AnaParticle::eta , _2 ) ) ) );
  taggable_jet_selector.add_uu( "pt" , bind( std::ptr_fun<AnaJet::ftype,AnaJet::ftype>(std::abs) , bind( &AnaParticle::pt , _1 ) ) > 20. ,
                                bind( dg::fillh , _1 , 100 , 0 , 100 , bind( std::ptr_fun<AnaJet::ftype,AnaJet::ftype>(std::abs) , bind( &AnaParticle::pt , _2 ) ) ) );
  taggable_jet_selector.do_extra_study( st_jet );

  EventObjectColl< shared_ptr<const AnaJet> > b_jets;  // taggable b jet selection
  EventObjectColl< shared_ptr<const AnaJet> > light_jets;  // taggable light jet selection
  EventObjectColl< shared_ptr<const AnaJet> > pure_light_jets;  // taggable light jet with no nearby b quark selection
  DgCutSet< shared_ptr<const AnaJet> > taggable_b_jet_selector( "taggable_b_jet_selector" , b_jets.attach() );
  DgCutSet< shared_ptr<const AnaJet> > taggable_light_jet_selector( "taggable_light_jet_selector" , light_jets.attach() );
  DgCutSet< shared_ptr<const AnaJet> > taggable_pure_light_jet_selector( "taggable_pure_light_jet_selector" , pure_light_jets.attach() );
  taggable_b_jet_selector.based_on( taggable_jet_selector );
  taggable_b_jet_selector.add_uu( "near_b_quark" , 
                                  bind( &AnaJet::dr_b_parton , _1 ) < AnaConfiguration::jet_label_dr_cut() ,
                                  bind( dg::fillh , _1 , 100 , 0 , 1.0 , bind ( &AnaJet::dr_b_parton , _2 ) ) );
  taggable_b_jet_selector.do_extra_study( st_jet );
  taggable_light_jet_selector.based_on( taggable_jet_selector );
  taggable_light_jet_selector.add_uu( "not_near_b_quark" , 
                                      bind( &AnaJet::dr_b_parton , _1 ) > AnaConfiguration::jet_label_dr_cut() ,
                                      bind( dg::fillh , _1 , 100 , 0 , 1.0 , bind ( &AnaJet::dr_b_parton , _2 ) ) );
  taggable_light_jet_selector.do_extra_study( st_jet );
  taggable_pure_light_jet_selector.based_on( taggable_light_jet_selector );
  taggable_pure_light_jet_selector.add_uu( "light_purity_requirement" , 
                                           bind( &AnaJet::dr_b_parton , _1 ) > 0.3 ,
                                           bind( dg::fillh , _1 , 100 , 0 , 1.0 , bind ( &AnaJet::dr_b_parton , _2 ) ) );
  taggable_pure_light_jet_selector.do_extra_study( st_jet );

  // track collections
  // ----------------------------------------------------------------
  EventObjectColl< shared_ptr<const AnaTrack> > offline_tracks;  // global offline track selection
  DgCutSet< shared_ptr<const AnaTrack> > offline_track_selector( "offline_track_selector" , offline_tracks.attach() );
  offline_track_selector.add_uu( "pt" , bind( &AnaTrack::pt , _1 ) > 1. , bind( &dg::fillh , _1 , 100 , 0 , 100 , bind( &AnaTrack::pt , _2 ) ) );

  // Jet-track associations.
  EventObjectColl< shared_ptr<const AnaJetTrackAssociation> > offline_jet_track_associations;

  // event loop
  // ================================================================
  TRIGFTKANA_FOREACHEVENT( evt , n_events ) {

    if( !evt ) { break; } // no evt means we're out of events to read, or a serious error.

    // select taggable jets
    taggable_jet_selector.for_each( evt->atlas_anti_kt_4_h1_topo_jets().begin() , evt->atlas_anti_kt_4_h1_topo_jets().end() );
    taggable_b_jet_selector.for_each( evt->atlas_anti_kt_4_h1_topo_jets().begin() , evt->atlas_anti_kt_4_h1_topo_jets().end() );
    taggable_light_jet_selector.for_each( evt->atlas_anti_kt_4_h1_topo_jets().begin() , evt->atlas_anti_kt_4_h1_topo_jets().end() );
    taggable_pure_light_jet_selector.for_each( evt->atlas_anti_kt_4_h1_topo_jets().begin() , evt->atlas_anti_kt_4_h1_topo_jets().end() );

    // select tracks
    offline_track_selector.for_each( evt->offline_tracks().begin() , evt->offline_tracks().end() );

    // associate tracks with each taggable jet.
    // AnaJetTrackAssociation::build_associations( taggable_jets.begin() , taggable_jets.end() , offline_tracks.begin() , offline_tracks.end() ,
    //                                             0.4 , offline_jet_track_associations.back_inserter() );
    
    // loop over jets in event and identify b's and non-b's
    BOOST_FOREACH( const shared_ptr<const AnaJet>& jet , taggable_jets ) {


      // // associate tracks
      // // ================================================================
      // // grrr. let's associate 'em.
      // TrackAssociator< const vector< boost::shared_ptr<const AnaTrack> > > assoc( evt->offline_tracks() );
      // vector< shared_ptr<const AnaTrack> > associated_tracks( assoc(jet) );
      // if( false ) { // prune large |dz0| tracks
      //   // remove all tracks with |dz0|>prune_dz0_cutmm around highest pt track
      //   TrackPruner pruner;
      //   const double prune_dz0_cut = AnaConfiguration::jet_dz0_prune(); // e.g. 5 or 9999 mm
      //   pruner.prune( associated_tracks , prune_dz0_cut );
      // }
      // vector< AnaTrack::ftype > null_lik( associated_tracks.size() , 1. );
      // JetTrackTag tag( jet , 0 , jet->eta() , jet->phi() , associated_tracks , null_lik , null_lik );
      // StudyJetTag st( "central_jets" , "all loose jets with |eta|<=1" );
      // st.add_constant_category( "b_jet" , is_b );
      // st.add_constant_category( "light_jet" , is_light );
      // st.add_constant_category( "pure_light_jet" , is_pure_light );
      // st.for_only( tag );
      // if( jet->et() < 20. ) { 
      //   st.repurpose( "central_jets_pt_0_20" , "all loose jets with |eta|<=1 and pT<20" );
      // } else if( jet->et() < 40. ) { 
      //   st.repurpose( "central_jets_pt_20_40" , "all loose jets with |eta|<=1 and 20<=pT<40" );
      // } else if( jet->et() < 60. ) { 
      //   st.repurpose( "central_jets_pt_40_60" , "all loose jets with |eta|<=1 and 40<=pT<60" );
      // } else if( jet->et() < 100. ) { 
      //   st.repurpose( "central_jets_pt_60_100" , "all loose jets with |eta|<=1 and 60<=pT<100" );
      // } else {
      //   st.repurpose( "central_jets_pt_100_inf" , "all loose jets with |eta|<=1 and pT>=100" );
      // }
      // st.for_only( tag );
    }
    
  } // end for each event

  AnaEventMgr::instance()->close_sample();
  
}

