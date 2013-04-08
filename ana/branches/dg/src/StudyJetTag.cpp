#include <iostream>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <string>
#include <utility>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/format.hpp>
#include <boost/foreach.hpp>
#include <boost/scope_exit.hpp>
#include "TrigFTKAna/StudyJetTag.hpp"
#include "TrigFTKAna/StudyTrack.hpp"
#include "TrigFTKAna/StudyTrackDisplacement.hpp"
#include "DataGraphics/DgCollection.hh"
#include "TrigFTKAna/AnaTrack.hpp"
#include "TrigFTKAna/AnaJet.hpp"
#include "TrigFTKAna/Likelihood1D.hpp"
#include "TrigFTKAna/Detector.hpp"
#include "TrigFTKAna/AnaVertexFit.hpp"
#include "TrigFTKAna/StudyVertexFit.hpp"

using namespace std;
using namespace DataGraphics;

void
StudyJetTag::_study( StudyJetTag::type const& tag )
{
  _study_jet( tag );
  _study_tracks( tag );
  _study_resolutions( tag );
  _study_tag( tag );
}

void
StudyJetTag::_study_jet( StudyJetTag::type const& tag )
{
  const boost::shared_ptr<const AnaJet>& jet( tag.jet() );
  dg::fillh( "jet_et" , 50 , 0 , 50 , jet->et() , "jet et" );
  dg::fillh( "jet_eta" , 160 , -4 , 4 , jet->eta() , "jet eta" );
  dg::fillh( "jet_dr_b" , 100 , 0 , 1.0 , jet->dr_b_parton() );
  dg::fillh( "jet_dr_c" , 100 , 0 , 1.0 , jet->dr_c_parton() );
  dg::fillh( "jet_dr_light" , 100 , 0 , 1.0 , jet->dr_light_parton() );
  dg::fillh( "jet_dr_any_b" , 100 , 0 , 1.0 , jet->dr_b_parton_any() );
  dg::fillh( "jet_dr_any_c" , 100 , 0 , 1.0 , jet->dr_c_parton_any() );
  dg::fillh( "jet_dr_any_light" , 100 , 0 , 1.0 , jet->dr_light_parton_any() );
  dg::fillh( "jet_tag_l2_xd0" , 100 , 0 , 1 , jet->tag_l2_xd0() );
  dg::fillh( "jet_tag_l2_xz0" , 100 , 0 , 1 , jet->tag_l2_xz0() );
  dg::fillh( "jet_tag_l2_x2d" , 100 , 0 , 1 , jet->tag_l2_x2d() );
  dg::fillh( "jet_tag_ef_xd0" , 100 , 0 , 1 , jet->tag_ef_xd0() );
  dg::fillh( "jet_tag_ef_xz0" , 100 , 0 , 1 , jet->tag_ef_xz0() );
  dg::fillh( "jet_tag_ef_x2d" , 100 , 0 , 1 , jet->tag_ef_x2d() );
  dg::fillh( "tag_ip2d_w" , 110 , -10 , 100 , jet->tag_ip2d_w() ); 
  dg::fillh( "tag_ip3d_w" , 110 , -10 , 100 , jet->tag_ip3d_w() ); 
  dg::fillh( "tag_sv0_w" , 125 , -50 , 200 , jet->tag_sv0_w() ); 
  dg::fillh( "tag_sv1_w" , 100 , -10 , 20 , jet->tag_sv1_w() ); 
  dg::fillh( "tag_sv2_w" , 100 , -10 , 20 , jet->tag_sv2_w() ); 
  dg::fillh( "tag_cmb_w" , 120 , -20 , 100 , jet->tag_cmb_w() ); 
  dg::fillh( "tag_jetp_w" , 100 , 0 , 1 , jet->tag_jetp_w() ); 
  dg::fillh( "tag_jetpneg_w" , 100 , 0 , 1 , jet->tag_jetpneg_w() ); 
  dg::fillh( "tag_trkc_w" , 100 , 0 , 1 , jet->tag_trkc_w() ); 
}

void
StudyJetTag::_study_tracks( StudyJetTag::type const& tag )
{
  // plot properties of associated tracks

  //   get minimum dr b hadron, if it exists.
  AnaJet::Hadron mini;
  AnaTrack::ftype mini_rad = 0.;
  AnaTrack::ftype mini_phi = 0.;
  AnaTrack::ftype mini_z = 0.;
  const AnaJet::ftype mindr = tag.jet()->min_dr_hadron( AnaJet::Hadron::B_QUARK , mini );
  if( mindr < 0.3 ) {
    mini_phi = mini.phi();
    mini_rad = detector::quadrature( mini.x() , mini.y() );
    mini_z = mini.z();
  }

  // general track statistics
  dg::fillh( "jet_ntracks" , 100 , 0 , 100 , tag.tracks().size() , "number of tracks associated with jet" );
  dg::fillh( "jet_ntracks_d0sig3" , 25 , 0 , 25 ,
             std::count_if( tag.tracks().begin() , tag.tracks().end() , 
                            bind( std::ptr_fun<AnaTrack::ftype,AnaTrack::ftype>(std::abs) , bind(&AnaTrack::signed_d0_signif_monica,_1,tag.jet_phi())) > 3 ) ,
             "number of tracks associated with jet" );
  dg::fillh( "jet_ntracks_d0sig5" , 25 , 0 , 25 ,
             std::count_if( tag.tracks().begin() , tag.tracks().end() , 
                            bind( std::ptr_fun<AnaTrack::ftype,AnaTrack::ftype>(std::abs) , bind(&AnaTrack::signed_d0_signif_monica,_1,tag.jet_phi())) > 5 ) ,
             "number of tracks associated with jet" );
  dg::fillh( "jet_ntracks_d0f200um" , 25 , 0 , 25 ,
             std::count_if( tag.tracks().begin() , tag.tracks().end() , 
                            bind( std::ptr_fun<AnaTrack::ftype,AnaTrack::ftype>(std::abs) , bind(&AnaTrack::signed_d0,_1,tag.jet_phi())) > 0.200 ) ,
             "number of tracks associated with jet" );  

  StudyTrack study_track( "jet_tracks" , "plots of tracks associated with the jet" );
  study_track.add_category( "prompt_tracks" , bind( std::ptr_fun<AnaTrack::ftype,AnaTrack::ftype>(std::abs) , bind(&AnaTrack::d0,_1)) < 0.100 );
  study_track.add_category( "displaced_tracks_200um" , bind( std::ptr_fun<AnaTrack::ftype,AnaTrack::ftype>(std::abs) , bind(&AnaTrack::d0,_1)) > 0.200 );
  study_track.add_category( "displaced_tracks_400um" , bind( std::ptr_fun<AnaTrack::ftype,AnaTrack::ftype>(std::abs) , bind(&AnaTrack::d0,_1)) > 0.400 );
  study_track.add_category( "displaced_tracks_600um" , bind( std::ptr_fun<AnaTrack::ftype,AnaTrack::ftype>(std::abs) , bind(&AnaTrack::d0,_1)) > 0.600 );
  study_track.for_each( tag.tracks().begin() , tag.tracks().end() );

  dg::down( "jet_tracks" , "plots of tracks associated with the jet" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;

  // loop over tracks and identify z vertex as z0 of highest pT track.
  double zvert_pt = 0.;
  double zvert_z = 0.;
  double zvert_zerr = 0.;
  for( vector< boost::shared_ptr<const AnaTrack> >::const_iterator itrack=tag.tracks().begin(), ftrack=tag.tracks().end(); itrack!=ftrack; ++itrack ) {
    const boost::shared_ptr<const AnaTrack>& track( *itrack );
    if( track->pt() > zvert_pt ) { 
      zvert_pt = track->pt();
      zvert_z = track->z0();
      zvert_zerr = track->z0_err();
    }
  }
  // 20100228 - override vertex z with "primary vertex z" associated with the jet at ntuple read time. 
  // at present this is the truth z position of the first vertex in the event, but that may change.
  zvert_z = tag.jet()->primary_vertex_z();

  unsigned int n=0u;
  for( vector< boost::shared_ptr<const AnaTrack> >::const_iterator itrack=tag.tracks().begin() , ftrack=tag.tracks().end(); itrack!=ftrack; ++itrack, ++n ) {

    const boost::shared_ptr<const AnaTrack>& track( *itrack );
    const AnaTrack::ftype b_result = tag.lik_b( n );
    const AnaTrack::ftype notb_result = tag.lik_notb( n );
    const AnaTrack::ftype r_result = detector::safe_divide( b_result , notb_result );
    const AnaTrack::ftype b_x = detector::likx( b_result );
    const AnaTrack::ftype notb_x = detector::likx( notb_result );
    const AnaTrack::ftype r_x = detector::likx( r_result );
    const double d0 = track->d0();
    const double d0_signif = track->d0_signif();
    const double signed_d0 = track->signed_d0( tag.jet_phi() );
    const double signed_d0_signif = track->signed_d0_signif( tag.jet_phi() );
    const double signed_d0_signif_monica = track->signed_d0_signif_monica( tag.jet_phi() );
    const double z0c = track->z0c( zvert_z );
    const double z0cL = track->z0cL( zvert_z );
    const double z0cL_signif = track->z0cL_signif( zvert_z , zvert_zerr );
    const double signed_z0cL = track->signed_z0cL( tag.jet_eta() , zvert_z );
    const double signed_z0cL_signif = track->signed_z0cL_signif( tag.jet_eta() , zvert_z , zvert_zerr );

    const double z0c_tz = track->z0c( mini_z );
    const double z0cL_tz = track->z0cL( mini_z );
    const double z0cL_signif_tz = track->z0cL_signif( mini_z , zvert_zerr );
    const double signed_z0cL_tz = track->signed_z0cL( tag.jet_eta() , mini_z );
    const double signed_z0cL_signif_tz = track->signed_z0cL_signif( tag.jet_eta() , mini_z , zvert_zerr );

    dg::fillh( "signed_d0" , 100 , -1 , 1 , signed_d0 , "signed d_{0} (mm)" );
    dg::fillh( "signed_d0_likbin" , 108 , -2.5 , 2.5 , signed_d0 , "signed d_{0} (mm)" );
    dg::fillh( "signed_d0_high" , 20 , -5 , 5 , signed_d0 , "signed d_{0} (mm)" );
    dg::fillh( "signed_d0_signif" , 100 , -25 , 25 , signed_d0_signif , "signed d_{0} significance" );
    dg::fillh( "signed_d0_signif_monica" , 100 , -25 , 25 , signed_d0_signif_monica , "signed d_{0} significance" );
    dg::fillh( "signed_d0_signif_monica_likbin" , 50 , -25 , 25 , signed_d0 , "signed d_{0} significance" );

    dg::fillh( "z0c" , 100 , -10 , 10 , z0c , "z_{0} corrected for vertex (mm)" );
    dg::fillh( "z0cL" , 100 , -10 , 10 , z0cL , "z_{0} corrected for vertex and angle (mm)" );
    dg::fillh( "z0cLsignif" , 100 , -25 , 25 , z0cL_signif , "corrected z_{0} significance" );
    dg::fillh( "signed_z0cL" , 100 , -10 , 10 , signed_z0cL , "signed z_{0} corrected for vertex and angle (mm)" );
    dg::fillh( "signed_z0cLsignif" , 100 , -25 , 25 , signed_z0cL_signif , "corrected signed z_{0} significance" );

    dg::fillh( "z0c_tz" , 100 , -100 , 100 , z0c_tz , "z_{0} corrected for true vertex (mm)" );
    dg::fillh( "z0cL_tz" , 100 , -100 , 100 , z0cL_tz , "z_{0} corrected for true vertex and angle (mm)" );
    dg::fillh( "z0cLsignif_tz" , 100 , -25 , 25 , z0cL_signif_tz , "truth-corrected z_{0} significance" );
    dg::fillh( "signed_z0cL_tz" , 100 , -10 , 10 , signed_z0cL_tz , "signed z_{0} corrected for truth vertex and angle (mm)" );
    dg::fillh( "signed_z0cLsignif_tz" , 100 , -25 , 25 , signed_z0cL_signif_tz , "truth-corrected signed z_{0} significance" );

    const double jet_deta = detector::delta_eta( track->eta() , tag.jet_eta() );
    const double jet_dphi = detector::delta_phi( track->phi0() , tag.jet_phi() );
    const double jet_dr = detector::quadrature( jet_deta , jet_dphi );

    dg::fillh( "track_jet_deta" , 100 , -1 , 1 , jet_deta , "#Delta #eta" );
    dg::fillh( "track_jet_dphi" , 100 , -1 , 1 , jet_dphi , "#Delta #phi" );
    dg::fillh( "track_jet_dr" , 100 , 0 , 1 , jet_dr , "#Delta r" );

    dg::fillh( "b" , 100 , -5 , 5 , detector::safe_log10(b_result) , "track log b likelihood" );
    dg::fillh( "notb" , 100 , -5 , 5 , detector::safe_log10(notb_result) , "track log not b likelihood" );
    dg::fillh( "b_x" , 100 , 0 , 1 , b_x , "x for track b" );
    dg::fillh( "notb_x" , 100 , 0 , 1 , notb_x , "x for track notb" );
    dg::fillh( "r" , 100 , -5 , 5 , detector::safe_log10(r_result) , "track log likelihood ratio" );
    dg::fillh( "r_x" , 100 , 0 , 1 , r_x , "track x likelihood ratio" );

    dg::fillh( "b_x_vs_signed_d0" , 40 , -1 , 1 , 25 , 0 , 1 , signed_d0 , b_x , "signed d_{0}" , "x for track b" );
    dg::fillh( "notb_x_vs_signed_d0" , 40 , -1 , 1 , 25 , 0 , 1 , signed_d0 , notb_x , "signed d_{0}" , "x for track notb" );
    dg::fillh( "r_x_vs_signed_d0" , 40 , -1 , 1 , 25 , 0 , 1 , signed_d0 , r_x , "signed d_{0}" , "x for track ratio" );
    dg::fillh( "r_x_vs_jet_dphi" , 40 , -1 , 1 , 100 , 0 , 1 , jet_dphi , r_x , "jet d#phi" , "track x likelihood ratio" );

    if( std::abs( b_x ) < 0.1 ) { dg::fillh( "signed_d0_low_b_x" , 500 , -5 , 5 , signed_d0 , "signed d_{0}" ); }
    if( std::abs( notb_x ) < 0.1 ) { dg::fillh( "signed_d0_low_notb_x" , 500 , -5 , 5 , signed_d0 , "signed d_{0}" ); }
    if( std::abs( b_x ) > 0.75 ) { dg::fillh( "signed_d0_high_b_x" , 500 , -5 , 5 , signed_d0 , "signed d_{0}"  ); }
    if( std::abs( notb_x ) > 0.75 ) { dg::fillh( "signed_d0_high_notb_x" , 500 , -5 , 5 , signed_d0 , "signed d_{0}" ); }
    
  } // end for each track

  if( true ) {
      dg::down( "trackcounting" , "plots for track counting algorithm" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      // study track counting, e.g. http://indico.cern.ch/getFile.py/access?contribId=61&sessionId=3&resId=0&materialId=slides&confId=31799
      //  retrieve list of signed d0 significance
      vector<AnaTrack::ftype> ips;
      transform( tag.tracks().begin() , tag.tracks().end() , back_inserter(ips) , bind( &AnaTrack::signed_d0_signif_monica , _1 , tag.jet_phi() ) );
      //  order with decreasing value
      std::sort( ips.begin() , ips.end() , std::greater<AnaTrack::ftype>() );
      if( !ips.empty() ) {
        unsigned int ips_size = ips.size();
        dg::fillh( "1st_p" , 100 , -25 , 25 , ips.front() );
        if( ips_size>1 ) { dg::fillh( "2nd_p" , 100 , -25 , 25 , ips[1] ); }
        if( ips_size>2 ) { dg::fillh( "3rd_p" , 100 , -25 , 25 , ips[2] ); }
        if( ips_size>3 ) { dg::fillh( "4th_p" , 100 , -25 , 25 , ips[3] ); }
        // n=2, n=3, ... discriminators. clip value at below zero.
        const AnaTrack::ftype n2x = (ips_size>=2) ? std::max(ips[1],static_cast<AnaTrack::ftype>(0)) : -1;
        const AnaTrack::ftype n3x = (ips_size>=3) ? std::max(ips[2],static_cast<AnaTrack::ftype>(0)) : -1;
        const AnaTrack::ftype n4x = (ips_size>=4) ? std::max(ips[3],static_cast<AnaTrack::ftype>(0)) : -1;
        dg::fillh( "n2x" , 52 , -1 , 26 , n2x );
        dg::fillh( "n3x" , 52 , -1 , 26 , n3x );
        dg::fillh( "n4x" , 52 , -1 , 26 , n4x );
      }
  } // end study track counting

  // study Rb linear vertex fit
  StudyTrackDisplacement< vector< boost::shared_ptr<const AnaTrack> > >  study_disp( "D0_VS_PHI" , "d0 vs phi studies" , tag.jet()->phi() , mini_rad , mini_phi );
  study_disp.add_constant_category( "high_tag_lik" , detector::likx( tag.jet_result() ) > 0.7 );
  study_disp.add_constant_category( "low_tag_lik" , detector::likx( tag.jet_result() ) < 0.3 );
  study_disp.for_only( tag.tracks() );
  
}

void
StudyJetTag::_study_resolutions( StudyJetTag::type const& tag )
{
  if( tag.jet()->dr_light_parton() > 0.5 ) { return; }

  dg::down( "resolution_measurement" , "pt-dependent resolution measurement" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;

  BOOST_FOREACH( const boost::shared_ptr<const AnaTrack>& track , tag.tracks() ) {
    const double d0 = track->d0();
    const double d0_signif = track->d0_signif();
    const double signed_d0 = track->signed_d0( tag.jet_phi() );
    const double signed_d0_signif = track->signed_d0_signif( tag.jet_phi() );
    const double signed_d0_signif_monica = track->signed_d0_signif_monica( tag.jet_phi() );
    dg::fillh( "d0" , 100 , -0.5 , 0.5 , track->d0() , "track d_{0}" );
    dg::fillh( "d0_vs_nhits" , 10 , 5 , 15 , 100 , -0.5 , 0.5 , track->nhits() , track->d0() , "N HITS" , "d_{0}" );
    dg::fillh( "d0_vs_chi2ndof" , 50 , 0 , 25 , 50 , -0.5 , 0.5 , track->chi2ndof() , track->d0() , "#chi^{2}/ndof" , "d_{0}" );
    dg::fillh( "d0_err" , 100 , 0. , 1 , track->d0_err() , "track d_{0} error" );
    dg::fillh( "d0_err_monica" , 100 , 0. , 1 , track->monica_d0_err() , "track d_{0} error, Monica's parameterization" );
    dg::fillh( "d0_vs_pt" , 100 , 0 , 50 , 100 , -0.5 , 0.5 , track->pt() , track->d0() , "p_{T}" , "d_{0}" );
    dg::fillgraph( "d0_vs_pt_gr" , track->pt() , track->d0() , "p_{T}" , "d_{0}" );
    if( track->nhits()>=11 ) { dg::fillgraph( "d0_vs_pt_nhitsge11_gr" , track->pt() , track->d0() , "p_{T}" , "d_{0}" ); }
    if( track->nhits()<11 ) { dg::fillgraph( "d0_vs_pt_nhitslt11_gr" , track->pt() , track->d0() , "p_{T}" , "d_{0}" ); }
    dg::fillh( "signed_d0_vs_pt" , 75 , 0 , 15 , 100 , -0.5 , 0.5 , track->pt() , signed_d0 , "p_{T}" , "signed d_{0}" );
    dg::fillh( "signed_d0_vs_nhits" , 20 , 0 , 20 , 100 , -0.5 , 0.5 , track->nhits() , signed_d0 , "N hits" , "signed d_{0}" );
    dg::fillh( "signed_d0_signif" , 100 , -25 , 25 , signed_d0_signif , "signed d_{0} sig." );
    dg::fillh( "signed_d0_signif_monica" , 100 , -25 , 25 , signed_d0_signif_monica , "signed d_{0} sig." );
    if( track->pt() < 2. ) { 
      dg::fillh( "signed_d0_signif_pt2gev" , 100 , -25 , 25 , signed_d0_signif , "signed d_{0} sig." );
      dg::fillh( "signed_d0_signif_monica_pt2gev" , 100 , -25 , 25 , signed_d0_signif_monica , "signed d_{0} sig." );
    }
    if( track->pt() >= 2. && track->pt() < 5. ) { 
      dg::fillh( "signed_d0_signif_pt5gev" , 100 , -25 , 25 , signed_d0_signif , "signed d_{0} sig." );
      dg::fillh( "signed_d0_signif_monica_pt5gev" , 100 , -25 , 25 , signed_d0_signif_monica , "signed d_{0} sig." );
    }
    if( track->pt() >= 5. && track->pt() < 10. ) { 
      dg::fillh( "signed_d0_signif_pt10gev" , 100 , -25 , 25 , signed_d0_signif , "signed d_{0} sig." );
      dg::fillh( "signed_d0_signif_monica_pt10gev" , 100 , -25 , 25 , signed_d0_signif_monica , "signed d_{0} sig." );
    }
    if( track->pt() >= 10. ) { 
      dg::fillh( "signed_d0_signif_pta10gev" , 100 , -25 , 25 , signed_d0_signif , "signed d_{0} sig." );
      dg::fillh( "signed_d0_signif_monica_pta10gev" , 100 , -25 , 25 , signed_d0_signif_monica , "signed d_{0} sig." );
    }
    if( std::abs(signed_d0_signif)>5.5 ) {
      dg::fillh( "d0sigtails_d0" , 100 , -1 , 1 , track->d0() , "track d_{0}" );
      dg::fillh( "d0sigtails_z0" , 100 , -150 , 150 , track->z0() , "track z_{0}" );
      dg::fillh( "d0sigtails_nhits" , 15 , 0 , 15 , track->nhits() , "n hits" );
      dg::fillh( "d0sigtails_chi2ndof" , 50 , 0 , 10 , track->chi2ndof() , "#chi^{2}" );
      dg::fillh( "d0sigtails_npixholes" , 4 , 0 , 4 , track->pixel_nholes() , "n holes" );
      dg::fillh( "d0sigtails_nsctholes" , 4 , 0 , 4 , track->sct_nholes() , "n holes" );
      dg::fillh( "d0sigtails_pt" , 100 , 0 , 20 , track->pt() , "p_{T}" );
    }
    if( track->is_offline() ) {
      if( track->nshared()==0 ) { dg::fillh( "signed_d0_no_shared" , 100 , -1 , 1 ,  signed_d0 , "signed d_{0}" ); }
      if( track->pixel_nshared()==0 ) { dg::fillh( "signed_d0_no_pixel_shared" , 100 , -1 , 1 ,  signed_d0 , "signed d_{0}" ); }
      if( track->sct_nshared()==0 ) { dg::fillh( "signed_d0_no_sct_shared" , 100 , -1 , 1 ,  signed_d0 , "signed d_{0}" ); }
      if( track->pixel_nshared()==1 ) { dg::fillh( "signed_d0_eq1_pixel_shared" , 100 , -1 , 1 ,  signed_d0 , "signed d_{0}" ); }
      if( track->pixel_nshared()>=2 ) { dg::fillh( "signed_d0_ge2_pixel_shared" , 100 , -1 , 1 ,  signed_d0 , "signed d_{0}" ); }
      if( track->sct_nshared()==1 ) { dg::fillh( "signed_d0_eq1_sct_shared" , 100 , -1 , 1 ,  signed_d0 , "signed d_{0}" ); }
      if( track->sct_nshared()>=2 ) { dg::fillh( "signed_d0_ge2_sct_shared" , 100 , -1 , 1 ,  signed_d0 , "signed d_{0}" ); }
    }
    if( track->is_ftk() ) {
      if( track->nshared()==0 ) { dg::fillh( "signed_d0_no_shared" , 100 , -1 , 1 ,  signed_d0 , "signed d_{0}" ); }
      if( track->pixel_nshared()==0 ) { dg::fillh( "signed_d0_no_pixel_shared" , 100 , -1 , 1 ,  signed_d0 , "signed d_{0}" ); }
      if( track->sct_nshared()==0 ) { dg::fillh( "signed_d0_no_sct_shared" , 100 , -1 , 1 ,  signed_d0 , "signed d_{0}" ); }
      if( track->pixel_nshared()>0 && track->pixel_nshared()<=2 ) { dg::fillh( "signed_d0_eq1_pixel_shared" , 100 , -1 , 1 ,  signed_d0 , "signed d_{0}" ); }
      if( track->pixel_nshared()>2 ) { dg::fillh( "signed_d0_ge2_pixel_shared" , 100 , -1 , 1 ,  signed_d0 , "signed d_{0}" ); }
      if( track->sct_nshared()==1 ) { dg::fillh( "signed_d0_eq1_sct_shared" , 100 , -1 , 1 ,  signed_d0 , "signed d_{0}" ); }
      if( track->sct_nshared()>=2 ) { dg::fillh( "signed_d0_ge2_sct_shared" , 100 , -1 , 1 ,  signed_d0 , "signed d_{0}" ); }
    }
    //   if( track->is_ftk() ) {
    //     unsigned int nmissedpix = detector::ftkNumLayersMissedPixels(track->ftk_bitmask());
    //     if( nmissedpix > 0 ) {
    //       dg::fillh( "d0_misspix" , 100 , -0.5 , 0.5 , track->d0() , "track d_{0}" );
    //       dg::fillh( "signed_d0_misspix" , 100 , -0.5 , 0.5 , signed_d0 , "signed d_{0}" );
    //       if( !detector::ftkHasLayer(track->ftk_bitmask(),1) ) {
    //         dg::fillh( "d0_misspix1L" , 100 , -0.5 , 0.5 , track->d0() , "track d_{0}" );
    //         dg::fillh( "signed_d0_misspix1L" , 100 , -0.5 , 0.5 , signed_d0 , "signed d_{0}" );
    //       } else if( !detector::ftkHasLayer(track->ftk_bitmask(),2) ) {
    //         dg::fillh( "d0_misspix2L" , 100 , -0.5 , 0.5 , track->d0() , "track d_{0}" );
    //         dg::fillh( "signed_d0_misspix2L" , 100 , -0.5 , 0.5 , signed_d0 , "signed d_{0}" );
    //       }
    //     } else {
    //       dg::fillh( "d0_allpix" , 100 , -0.5 , 0.5 , track->d0() , "track d_{0}" );
    //       dg::fillh( "signed_d0_allpix" , 100 , -0.5 , 0.5 , signed_d0 , "signed d_{0}" );
    //     }
    //     if( !detector::ftkHasLayer(track->ftk_bitmask(),3) ) {
    //       dg::fillh( "d0_misssct1L" , 100 , -0.5 , 0.5 , track->d0() , "track d_{0}" );
    //       dg::fillh( "signed_d0_misssct1L" , 100 , -0.5 , 0.5 , signed_d0 , "signed d_{0}" );
    //     } else if( !detector::ftkHasLayer(track->ftk_bitmask(),9) ) {
    //       dg::fillh( "d0_misssct4L" , 100 , -0.5 , 0.5 , track->d0() , "track d_{0}" );
    //       dg::fillh( "signed_d0_misssct4L" , 100 , -0.5 , 0.5 , signed_d0 , "signed d_{0}" );
    //     }
    //   }
    //   // if( signed_d0 < 0. ) { 
    if( track->pt() < 2 ) { dg::fillh( "ptdep_d0_neg_pt2gev" , 200 , -1.0 , 1.0 , track->d0() ); }
    if( track->pt() >= 2 && track->pt() <= 3 ) { dg::fillh( "ptdep_d0_neg_pt2_3gev" , 200 , -1.0 , 1.0 , track->d0() , "d_{0} (mm)" ); }
    if( track->pt() >= 3 && track->pt() <= 4 ) { dg::fillh( "ptdep_d0_neg_pt3_4gev" , 200 , -1.0 , 1.0 , track->d0() , "d_{0} (mm)" ); }
    if( track->pt() >= 4 && track->pt() <= 6 ) { dg::fillh( "ptdep_d0_neg_pt4_6gev" , 200 , -1.0 , 1.0 , track->d0() , "d_{0} (mm)" ); }
    if( track->pt() >= 6 && track->pt() <= 8 ) { dg::fillh( "ptdep_d0_neg_pt6_8gev" , 200 , -1.0 , 1.0 , track->d0() , "d_{0} (mm)" ); }
    if( track->pt() >= 8 && track->pt() <= 10 ) { dg::fillh( "ptdep_d0_neg_pt8_10gev" , 200 , -1.0 , 1.0 , track->d0() , "d_{0} (mm)" ); }
    if( track->pt() >= 10 && track->pt() <= 15 ) { dg::fillh( "ptdep_d0_neg_pt10_15gev" , 200 , -1.0 , 1.0 , track->d0() , "d_{0} (mm)" ); }
    if( track->pt() >= 15 && track->pt() <= 20 ) { dg::fillh( "ptdep_d0_neg_pt15_20gev" , 200 , -1.0 , 1.0 , track->d0() , "d_{0} (mm)" ); }
    if( track->pt() >= 20 && track->pt() <= 40 ) { dg::fillh( "ptdep_d0_neg_pt20_40gev" , 200 , -1.0 , 1.0 , track->d0() , "d_{0} (mm)" ); }
    if( track->pt() >= 40 ) { dg::fillh( "ptdep_d0_neg_pt40gev" , 200 , -1.0 , 1.0 , track->d0() , "d_{0} (mm)" ); }
    if( track->pt() < 4 ) { 
      if( track->pixel_nhits() < 3 ) { 
        dg::fillh( "ptdep_d0_pixelmiss_lowpt" , 200 , -1.0 , 1.0 , track->d0() );
      } else {
        dg::fillh( "ptdep_d0_nopixelmiss_lowpt" , 200 , -1.0 , 1.0 , track->d0() );
      }
      if( track->is_ftk() ) {
        if( track->ftk_hole_number()==2 ) {
          dg::fillh( "ptdep_d0_misspix2_lowpt" , 200 , -1.0 , 1.0 , track->d0() );
        } else {
          dg::fillh( "ptdep_d0_notmisspix2_lowpt" , 200 , -1.0 , 1.0 , track->d0() );
        }
      }
    } else {
      if( track->pixel_nhits() < 3 ) { 
        dg::fillh( "ptdep_d0_pixelmiss_highpt" , 200 , -1.0 , 1.0 , track->d0() );
      } else {
        dg::fillh( "ptdep_d0_nopixelmiss_highpt" , 200 , -1.0 , 1.0 , track->d0() );
      }
      if( track->is_ftk() ) {
        if( track->ftk_hole_number()==2 ) {
          dg::fillh( "ptdep_d0_misspix2_highpt" , 200 , -1.0 , 1.0 , track->d0() );
        } else {
          dg::fillh( "ptdep_d0_notmisspix2_highpt" , 200 , -1.0 , 1.0 , track->d0() );
        }
      }
    }
  } // end for each track
}


void
StudyJetTag::_study_tag( StudyJetTag::type const& tag )
{
  dg::down( "jets" , "per jet tagger results" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;

  dg::fillh( "r" , 100 , -10 , 10 , tag.jet_result() , "jet result" );
  dg::fillh( "lnr" , 60 , -10 , 20 , detector::safe_ln(tag.jet_result()) , "ln jet result" );
  // dg::fillh( "b" , 100 , -1 , 1 , product_b_lik , "b likelihood result" );
  // dg::fillh( "notb" , 100 , -1 , 1 , product_notb_lik , "not b likelihood result" );
  // dg::fillh( "notb_vs_b" , 100 , -1 , 1 , 100 , -1 , 1 , product_b_lik , product_notb_lik , "b likelihood result" , "notb likelihood result" );
  dg::fillh( "r_x" , 100 , 0 , 1 , detector::likx( tag.jet_result() ) , "likelihood ratio / ( 1 + likelihood ratio )" );
  dg::fillh( "r_x_monica" , 15 , 0 , 1 , detector::likx( tag.jet_result() ) , "likelihood ratio / ( 1 + likelihood ratio )" );


  // get vertex fit for this jet
  AnaVertexFit vf;
  // if( tag.vertex_fit() ) { 
  //   vf = *tag.vertex_fit();
  // } else { 
  float vertex_mass = -1.;
  float vertex_pt_ratio = -1.;
  unsigned int n_two_track_vertices( 0u );
  const bool ok_vf = vf.compute_sv_variables( tag.tracks().begin() , tag.tracks().end() , vertex_mass , vertex_pt_ratio , n_two_track_vertices , &vf );
  dg::filleff( "vertex_eff" , ok_vf );
  if( ok_vf ) {
    dg::down( "vertex_fit" , "vertex fit" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
    assert( vf.is_fitted() );
    dg::down( "final" , "compute_sv_result" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
    StudyVertexFit< vector< boost::shared_ptr<const AnaTrack> > >::study_fit_result( vf , tag.jet() ); 
    dg::fillh( "good_vertex_mass_vs_pt_ratio" , 10 , 0 , 1 , 20 , 0 , 10 , vertex_pt_ratio , vertex_mass , "VERTEX PT / TRACK SUM PT" , "VERTEX MASS (GeV)" );
    dg::fillh( "good_vertex_mass_vs_n_two" , 25 , 0 , 25 , 20 , 0 , 10 , n_two_track_vertices , vertex_mass , "N TWO TRACK PAIRS" , "VERTEX MASS (GeV)" );
    dg::fillh( "good_vertex_pt_ratio_vs_n_two" , 25 , 0 , 25 , 10 , 0 , 1 , n_two_track_vertices , vertex_pt_ratio , "N TWO TRACK PAIRS" , "VERTEX PT / TRACK SUM PT" );
  }
  //   StudyVertexFit< vector< boost::shared_ptr<const AnaTrack> > >< st( "vertex_fit" , "tag vertex fit" );
  //   st.for_only( *tag.vertex_fit() );
  // }

  // correlations with other taggers
  dg::filleff( "eff_0.4" , detector::likx( tag.jet_result() ) > 0.4 );
  dg::filleff( "eff_0.5" , detector::likx( tag.jet_result() ) > 0.5 );
  dg::filleff( "eff_0.6" , detector::likx( tag.jet_result() ) > 0.6 );
  dg::filleff( "eff_0.7" , detector::likx( tag.jet_result() ) > 0.7 );
  dg::filleff( "eff_0.8" , detector::likx( tag.jet_result() ) > 0.8 );

  {
    dg::fillh( "correl_tag_ip2d_w" , 110 , -10 , 100 , tag.jet()->tag_ip2d_w() ); 
    dg::fillh( "correl_tag_ip3d_w" , 110 , -10 , 100 , tag.jet()->tag_ip3d_w() ); 
    dg::fillh( "correl_tag_sv0_w" , 125 , -50 , 200 , tag.jet()->tag_sv0_w() ); 
    dg::fillh( "correl_tag_sv1_w" , 100 , -10 , 20 , tag.jet()->tag_sv1_w() ); 
    dg::fillh( "correl_tag_sv2_w" , 100 , -10 , 20 , tag.jet()->tag_sv2_w() ); 
    dg::fillh( "correl_tag_cmb_w" , 120 , -20 , 100 , tag.jet()->tag_cmb_w() ); 
    dg::fillh( "correl_tag_jetp_w" , 100 , 0 , 1 , tag.jet()->tag_jetp_w() ); 
    dg::fillh( "correl_tag_jetpneg_w" , 100 , 0 , 1 , tag.jet()->tag_jetpneg_w() ); 
    dg::fillh( "correl_tag_trkc_w" , 100 , 0 , 1 , tag.jet()->tag_trkc_w() ); 
  }

  if( detector::likx( tag.jet_result() ) > 0.8 ) { 
    dg::fillh( "correl_likx0.8_tag_ip2d_w" , 110 , -10 , 100 , tag.jet()->tag_ip2d_w() ); 
    dg::fillh( "correl_likx0.8_tag_ip3d_w" , 110 , -10 , 100 , tag.jet()->tag_ip3d_w() ); 
    dg::fillh( "correl_likx0.8_tag_sv0_w" , 125 , -50 , 200 , tag.jet()->tag_sv0_w() ); 
    dg::fillh( "correl_likx0.8_tag_sv1_w" , 100 , -10 , 20 , tag.jet()->tag_sv1_w() ); 
    dg::fillh( "correl_likx0.8_tag_sv2_w" , 100 , -10 , 20 , tag.jet()->tag_sv2_w() ); 
    dg::fillh( "correl_likx0.8_tag_cmb_w" , 120 , -20 , 100 , tag.jet()->tag_cmb_w() ); 
    dg::fillh( "correl_likx0.8_tag_jetp_w" , 100 , 0 , 1 , tag.jet()->tag_jetp_w() ); 
    dg::fillh( "correl_likx0.8_tag_jetpneg_w" , 100 , 0 , 1 , tag.jet()->tag_jetpneg_w() ); 
    dg::fillh( "correl_likx0.8_tag_trkc_w" , 100 , 0 , 1 , tag.jet()->tag_trkc_w() ); 
  }

  if( detector::likx( tag.jet_result() ) > 0.7 ) { 
    dg::fillh( "correl_likx0.7_tag_ip2d_w" , 110 , -10 , 100 , tag.jet()->tag_ip2d_w() ); 
    dg::fillh( "correl_likx0.7_tag_ip3d_w" , 110 , -10 , 100 , tag.jet()->tag_ip3d_w() ); 
    dg::fillh( "correl_likx0.7_tag_sv0_w" , 125 , -50 , 200 , tag.jet()->tag_sv0_w() ); 
    dg::fillh( "correl_likx0.7_tag_sv1_w" , 100 , -10 , 20 , tag.jet()->tag_sv1_w() ); 
    dg::fillh( "correl_likx0.7_tag_sv2_w" , 100 , -10 , 20 , tag.jet()->tag_sv2_w() ); 
    dg::fillh( "correl_likx0.7_tag_cmb_w" , 120 , -20 , 100 , tag.jet()->tag_cmb_w() ); 
    dg::fillh( "correl_likx0.7_tag_jetp_w" , 100 , 0 , 1 , tag.jet()->tag_jetp_w() ); 
    dg::fillh( "correl_likx0.7_tag_jetpneg_w" , 100 , 0 , 1 , tag.jet()->tag_jetpneg_w() ); 
    dg::fillh( "correl_likx0.7_tag_trkc_w" , 100 , 0 , 1 , tag.jet()->tag_trkc_w() ); 
  // } else {
  //   dg::fillh( "correl_ltlikx0.7_tag_ip2d_w" , 110 , -10 , 100 , tag.jet()->tag_ip2d_w() ); 
  //   dg::fillh( "correl_ltlikx0.7_tag_cmb_w" , 120 , -20 , 100 , tag.jet()->tag_cmb_w() ); 
  //   dg::fillh( "correl_ltlikx0.7_tag_sv1_w" , 100 , -10 , 20 , tag.jet()->tag_sv1_w() ); 
  }

  if( detector::likx( tag.jet_result() ) > 0.6 ) { 
    dg::fillh( "correl_likx0.6_tag_ip2d_w" , 110 , -10 , 100 , tag.jet()->tag_ip2d_w() ); 
    dg::fillh( "correl_likx0.6_tag_ip3d_w" , 110 , -10 , 100 , tag.jet()->tag_ip3d_w() ); 
    dg::fillh( "correl_likx0.6_tag_sv0_w" , 125 , -50 , 200 , tag.jet()->tag_sv0_w() ); 
    dg::fillh( "correl_likx0.6_tag_sv1_w" , 100 , -10 , 20 , tag.jet()->tag_sv1_w() ); 
    dg::fillh( "correl_likx0.6_tag_sv2_w" , 100 , -10 , 20 , tag.jet()->tag_sv2_w() ); 
    dg::fillh( "correl_likx0.6_tag_cmb_w" , 120 , -20 , 100 , tag.jet()->tag_cmb_w() ); 
    dg::fillh( "correl_likx0.6_tag_jetp_w" , 100 , 0 , 1 , tag.jet()->tag_jetp_w() ); 
    dg::fillh( "correl_likx0.6_tag_jetpneg_w" , 100 , 0 , 1 , tag.jet()->tag_jetpneg_w() ); 
    dg::fillh( "correl_likx0.6_tag_trkc_w" , 100 , 0 , 1 , tag.jet()->tag_trkc_w() ); 
  }

  if( detector::likx( tag.jet_result() ) > 0.5 ) { 
    dg::fillh( "correl_likx0.5_tag_ip2d_w" , 110 , -10 , 100 , tag.jet()->tag_ip2d_w() ); 
    dg::fillh( "correl_likx0.5_tag_ip3d_w" , 110 , -10 , 100 , tag.jet()->tag_ip3d_w() ); 
    dg::fillh( "correl_likx0.5_tag_sv0_w" , 125 , -50 , 200 , tag.jet()->tag_sv0_w() ); 
    dg::fillh( "correl_likx0.5_tag_sv1_w" , 100 , -10 , 20 , tag.jet()->tag_sv1_w() ); 
    dg::fillh( "correl_likx0.5_tag_sv2_w" , 100 , -10 , 20 , tag.jet()->tag_sv2_w() ); 
    dg::fillh( "correl_likx0.5_tag_cmb_w" , 120 , -20 , 100 , tag.jet()->tag_cmb_w() ); 
    dg::fillh( "correl_likx0.5_tag_jetp_w" , 100 , 0 , 1 , tag.jet()->tag_jetp_w() ); 
    dg::fillh( "correl_likx0.5_tag_jetpneg_w" , 100 , 0 , 1 , tag.jet()->tag_jetpneg_w() ); 
    dg::fillh( "correl_likx0.5_tag_trkc_w" , 100 , 0 , 1 , tag.jet()->tag_trkc_w() ); 
  }

  if( detector::likx( tag.jet_result() ) > 0.4 ) { 
    dg::fillh( "correl_likx0.4_tag_ip2d_w" , 110 , -10 , 100 , tag.jet()->tag_ip2d_w() ); 
    dg::fillh( "correl_likx0.4_tag_ip3d_w" , 110 , -10 , 100 , tag.jet()->tag_ip3d_w() ); 
    dg::fillh( "correl_likx0.4_tag_sv0_w" , 125 , -50 , 200 , tag.jet()->tag_sv0_w() ); 
    dg::fillh( "correl_likx0.4_tag_sv1_w" , 100 , -10 , 20 , tag.jet()->tag_sv1_w() ); 
    dg::fillh( "correl_likx0.4_tag_sv2_w" , 100 , -10 , 20 , tag.jet()->tag_sv2_w() ); 
    dg::fillh( "correl_likx0.4_tag_cmb_w" , 120 , -20 , 100 , tag.jet()->tag_cmb_w() ); 
    dg::fillh( "correl_likx0.4_tag_jetp_w" , 100 , 0 , 1 , tag.jet()->tag_jetp_w() ); 
    dg::fillh( "correl_likx0.4_tag_jetpneg_w" , 100 , 0 , 1 , tag.jet()->tag_jetpneg_w() ); 
    dg::fillh( "correl_likx0.4_tag_trkc_w" , 100 , 0 , 1 , tag.jet()->tag_trkc_w() ); 
  }

  if( detector::likx( tag.jet_result() ) > 0.7 ) {  
    dg::down( "tagged_vertex_fit" , "vertex fit" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
    dg::filleff( "vertex_eff" , vf.is_fitted() );
    if( vf.is_fitted() ) {
      dg::down( "final" , "compute_sv_result" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      StudyVertexFit< vector< boost::shared_ptr<const AnaTrack> > >::study_fit_result( vf , tag.jet() ); 
      dg::fillh( "good_vertex_mass_vs_pt_ratio" , 10 , 0 , 1 , 20 , 0 , 10 , vertex_pt_ratio , vertex_mass , "VERTEX PT / TRACK SUM PT" , "VERTEX MASS (GeV)" );
      dg::fillh( "good_vertex_mass_vs_n_two" , 25 , 0 , 25 , 20 , 0 , 10 , n_two_track_vertices , vertex_mass , "N TWO TRACK PAIRS" , "VERTEX MASS (GeV)" );
      dg::fillh( "good_vertex_pt_ratio_vs_n_two" , 25 , 0 , 25 , 10 , 0 , 1 , n_two_track_vertices , vertex_pt_ratio , "N TWO TRACK PAIRS" , "VERTEX PT / TRACK SUM PT" );    
    }
  } else {
    dg::down( "nottagged_vertex_fit" , "vertex fit" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
    dg::filleff( "vertex_eff" , vf.is_fitted() );
    if( vf.is_fitted() ) {
      dg::down( "final" , "compute_sv_result" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      StudyVertexFit< vector< boost::shared_ptr<const AnaTrack> > >::study_fit_result( vf , tag.jet() ); 
      dg::fillh( "good_vertex_mass_vs_pt_ratio" , 10 , 0 , 1 , 20 , 0 , 10 , vertex_pt_ratio , vertex_mass , "VERTEX PT / TRACK SUM PT" , "VERTEX MASS (GeV)" );
      dg::fillh( "good_vertex_mass_vs_n_two" , 25 , 0 , 25 , 20 , 0 , 10 , n_two_track_vertices , vertex_mass , "N TWO TRACK PAIRS" , "VERTEX MASS (GeV)" );
      dg::fillh( "good_vertex_pt_ratio_vs_n_two" , 25 , 0 , 25 , 10 , 0 , 1 , n_two_track_vertices , vertex_pt_ratio , "N TWO TRACK PAIRS" , "VERTEX PT / TRACK SUM PT" );    
    }
  }
  
  // if( !xes.empty() ) {
  //   dg::fillh( "r_classifier_vs_x_max" , 10 , -1 , 1 , 50 , -10 , 10 , xes.front() , tag.jet_result() , "signed_d0" , "likelihood ratio" );
  //   dg::fillh( "r_classifier_x_vs_x_max" , 10 , -1 , 1 , 100 , 0 , 1 , xes.front() , detector::likx( tag.jet_result() ) , "signed_d0" , "likelihood ratio / ( 1 + likelihood ratio )" );
  // }
  // // look at correlations amongst x's in a jet.
  // if( xes.size()>=2 ) {
  //   //partial_sort( xes.begin() , advance(xes.begin(),2) , xes.end() , greater<double>() );
  //   dg::fillh( "x2_vs_x1" , 100 , -10 , 10 , 100 , -10 , 10 , *(xes.begin()) , *(++xes.begin()) , "highest x" , "next highest x" );
  // }
}
