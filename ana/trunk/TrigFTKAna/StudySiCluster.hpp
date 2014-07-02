#ifndef WRAP_STUDYSICLUSTER_HH
#define WRAP_STUDYSICLUSTER_HH

// make standard cluster plots

#include <string>
#include <vector>
#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/scope_exit.hpp>
#include <boost/format.hpp>
#include "DataGraphics/DgStudy.hh"
#include "DataGraphics/DgCollection.hh"
#include "TrigFTKAna/AnaTrack.hpp"
#include "TrigFTKAna/AnaSiCluster.hpp"
#include "TrigFTKAna/StudyTrackComparison.hpp"
#include "TrigFTKAna/Detector.hpp"

class
StudySiCluster : public DataGraphics::DgStudy< boost::shared_ptr<const AnaSiCluster> >
{
public:
  typedef boost::shared_ptr<const AnaSiCluster> type; // must be same type as the template argument for DgStudy.
protected:
  boost::shared_ptr<const AnaTrack> _reference_track;
  const std::vector< boost::shared_ptr<const AnaSiCluster> >* _ref_clusters;
  boost::shared_ptr<const AnaTrack>  _comparison_track;
protected:
  virtual void _study( type const& cluster ) {
    using namespace DataGraphics;
    const bool is_pixel = cluster->cluster_type()==AnaSiChannel::PIXEL;
    dg::filleff( "is_pixel" , is_pixel );
    dg::filleff( "barrel_ec" , cluster->barrel_ec() );
    dg::fillh( "layer_or_disk" , 14 , 0 , 14 , cluster->layer_or_disk() );
    dg::fillh( "ftk_logical_layer" , 14 , 0 , 14 , cluster->ftk_logical_layer() );
    dg::fillh( "phi_module" , 52 , 0 , 52 , cluster->phi_module() );
    dg::fillh( "eta_module" , 50 , -50 , 50 , cluster->eta_module() );
    dg::fillh( "phi_index" , 512 , 0 , 512 , cluster->phi_index() );
    if( is_pixel ) { dg::fillh( "eta_index" , 512 , 0 , 512 , cluster->eta_index() ); }
    if( !is_pixel ) { dg::fillh( "sct_side" , 3 , 0 , 3 , cluster->sct_side() ); }
    if( is_pixel ) { dg::fillh( "local_x_pixel" , 400 , 0 , 400 , cluster->local_x() ); }
    else { dg::fillh( "local_x_sct" , 1024 , 0 , 1024 , cluster->local_x() ); }
    dg::fillh( "width_x" , 20 , 0 , 20 , cluster->width_x() );
    if( is_pixel ) { 
      dg::fillh( "local_x_vs_z" , 148 , 0 , 148 , 400 , 0 , 400 , cluster->local_z() , cluster->local_x() , "LOCAL Z" , "LOCAL X" );
      dg::fillh( "local_z" , 145 , 0 , 145 , cluster->local_z() );
      dg::fillh( "width_z" , 20 , 0 , 20 , cluster->width_z() );
    }
    dg::fillh( "nchannels" , 20 , 0 , 20 , cluster->n_channels() );
    dg::fillh( "barcode_training" , 2 , 10000 , 10002 , cluster->barcode().barcode() );
    // cluster position differences and (possibly) correlations with track parameters
    if( _ref_clusters ) {
      _study_residuals( cluster );
    }
  } // end study of each cluster
  void _study_residuals( type const& cluster ) const {
    using namespace DataGraphics;
    if( !_ref_clusters ) { return; }
    const bool is_pixel = cluster->cluster_type()==AnaSiChannel::PIXEL;
    // find closest matching reference cluster on same
    // layer. typically the reference cluster list will be the list of
    // clusters on an offline track matching the track providing the
    // comparison cluster, in which case there should be little ambiguity. 
    AnaSiCluster::ftype best_distance = 99999.;
    const boost::shared_ptr<const AnaSiCluster>& match = cluster->find_matching_cluster( _ref_clusters->begin() , _ref_clusters->end() , best_distance );
    dg::filleff( "residual_match" , (bool)match );
    dg::filleff( "residual_match_dist1c" , match && best_distance<1 );
    dg::filleff( "residual_match_distHalfc" , match && best_distance<0.5 );
    dg::filleff( "residual_match_distFourthc" , match && best_distance<0.25 );
    if( !match ) { return; }
    const bool truth_matches = cluster->barcode() == match->barcode();
    const double dx = cluster->local_x() - match->local_x();
    const double dz = cluster->local_z() - match->local_z();
    dg::filleff( "residual_match_truth" , (bool)truth_matches );
    dg::fillh( "residual_dist" , 100 , -20 , 20 , best_distance );
    dg::fillh( "residual_local_dx" , 100 , -20 , 20 , dx );
    if( is_pixel ) { dg::fillh( "residual_local_dz" , 100 , -20 , 20 , dz ); }
    if( best_distance<1 ) {
      dg::fillh( "residual_tight_local_dx" , 100 , -1 , 1 , dx);
      if( is_pixel ) { dg::fillh( "residual_tight_local_dz" , 100 , -1 , 1 , dz); }
    }
    if( truth_matches ) {
      dg::fillh( "residual_truth_local_dx" , 100 , -1 , 1 , dx );
      dg::fillh( "residual_truth_local_dx_wide" , 100 , -10 , 10 , dx );
      if( match->width_x() == 1 ) { dg::fillh( "residual_truth_local_dx_singlex" , 100 , -1 , 1 , dx ); }
      if( is_pixel ) { 
        dg::fillh( "residual_truth_local_dz" , 100 , -1 , 1 , dz );
        dg::fillh( "residual_truth_local_dz_wide" , 100 , -10 , 10 , dz );
        if( match->width_z() == 1 ) { dg::fillh( "residual_truth_local_dz_singlez" , 100 , -1 , 1 , dz ); }
        if( match->width_x() == 1 && match->width_z() == 1 ) { 
          dg::fillh( "residual_truth_local_dx_singlexz" , 100 , -1 , 1 , dx );
          dg::fillh( "residual_truth_local_dz_singlexz" , 100 , -1 , 1 , dz );
        }
      }
      dg::fillh( "truth_ref_wx_vs_x" , 100 , 0 , 1000 , 10 , 0 , 10 , match->local_x() , match->width_x() , "LOCAL X (CHANNELS)" , "WIDTH X (CHANNELS)" );
      if( match->is_pixel() ) { dg::fillh( "truth_ref_wz_vs_z" , 256 , 0 , 256 , 10 , 0 , 10 , match->local_z() , match->width_z() , "LOCAL Z (CHANNELS)" , "WIDTH Z (CHANNELS)" ); }
      if( match->is_pixel() ) {
        if( std::abs(dx) > 2. ) {
          dg::fillh( "truth_ref_phi_badresidual_channelnumber" , 769 , 0 , 769 , match->phi_index() , "PHI INDEX (CHANNELS)" );
          dg::fillh( "truth_ref_eta_badresidual_channelnumber" , 769 , 0 , 769 , match->eta_index() , "ETA INDEX (CHANNELS)" );
        } else {
          dg::fillh( "truth_ref_phi_goodresidual_channelnumber" , 769 , 0 , 769 , match->phi_index() , "PHI INDEX (CHANNELS)" );
          dg::fillh( "truth_ref_eta_goodresidual_channelnumber" , 769 , 0 , 769 , match->eta_index() , "ETA INDEX (CHANNELS)" );
        }
      }
      if( std::abs(dx) > 0.25 ) {
        dg::fillh( "truth_ref_x_badresidual" , 1024 , 0 , 1024 , match->local_x() , "LOCAL X (CHANNELS)" );
        if( match->is_pixel() ) { dg::fillh( "truth_ref_x_vs_z_badresidual" , 148 , 0 , 148 , 384 , 0 , 384 , match->local_z() , match->local_x() , "LOCAL Z (CHANNELS)" , "LOCAL X (CHANNELS)" ); }
      }
      // check side convention for each SCT layer, barrel_ec, layer, module
      if( match->is_sct() && best_distance<1 ) {
        dg::down( "side_matching" , "checks of side matching for each sensor" );  BOOST_SCOPE_EXIT(void) { dg::up(); } BOOST_SCOPE_EXIT_END;
        const bool correct_layer = cluster->ftk_logical_layer()==match->ftk_logical_layer();
        const int group_idA = (static_cast<int>((cluster->ftk_logical_layer()-1)/2))*2 + 1;
        const int group_idB = group_idA + 1;
        const bool other_layer = !correct_layer && ( cluster->ftk_logical_layer() == group_idA || cluster->ftk_logical_layer() != group_idB );
        if( cluster->barrel_ec()==0 ) { dg::filleff( "correct_side_vs_layer_barrel" , 12 , 0 , 12 , cluster->ftk_logical_layer() , correct_layer , "FTK LOGICAL LAYER" ); }
        if( cluster->barrel_ec()!=0 ) { dg::filleff( "correct_side_vs_layer_endcap" , 12 , 0 , 12 , cluster->ftk_logical_layer() , correct_layer , "FTK LOGICAL LAYER" ); }
        dg::filleff( "correct_side_vs_layer" , 12 , 0 , 12 , cluster->ftk_logical_layer() , correct_layer , "FTK LOGICAL LAYER" );
        if( cluster->ftk_logical_layer()==3 ) {
          if( cluster->barrel_ec()==0 ) { dg::filleff( "correct_side_vs_phi_module_barrel_layer3" , 52 , 0 , 52 , cluster->phi_module() , correct_layer , "FTK #phi MODULE NUMBER" ); }
          if( cluster->barrel_ec()!=0 ) { dg::filleff( "correct_side_vs_phi_module_endcap_layer3" , 52 , 0 , 52 , cluster->phi_module() , correct_layer , "FTK #phi MODULE NUMBER" ); }
          dg::filleff( "correct_side_vs_phi_module_layer3" , 52 , 0 , 52 , cluster->phi_module() , correct_layer , "FTK #phi MODULE NUMBER" );
          if( cluster->barrel_ec()==0 ) { dg::filleff( "correct_side_vs_eta_module_barrel_layer3" , 52 , 0 , 52 , cluster->eta_module() , correct_layer , "FTK #eta MODULE NUMBER" ); }
          if( cluster->barrel_ec()!=0 ) { dg::filleff( "correct_side_vs_eta_module_endcap_layer3" , 52 , 0 , 52 , cluster->eta_module() , correct_layer , "FTK #eta MODULE NUMBER" ); }
          dg::filleff( "correct_side_vs_eta_module_layer3" , 52 , 0 , 52 , cluster->eta_module() , correct_layer , "FTK #eta MODULE NUMBER" );
        }
        if( cluster->ftk_logical_layer()==4 ) {
          if( cluster->barrel_ec()==0 ) { dg::filleff( "correct_side_vs_phi_module_barrel_layer4" , 52 , 0 , 52 , cluster->phi_module() , correct_layer , "FTK #phi MODULE NUMBER" ); }
          if( cluster->barrel_ec()!=0 ) { dg::filleff( "correct_side_vs_phi_module_endcap_layer4" , 52 , 0 , 52 , cluster->phi_module() , correct_layer , "FTK #phi MODULE NUMBER" ); }
          dg::filleff( "correct_side_vs_phi_module_layer4" , 52 , 0 , 52 , cluster->phi_module() , correct_layer , "FTK #phi MODULE NUMBER" );
          if( cluster->barrel_ec()==0 ) { dg::filleff( "correct_side_vs_eta_module_barrel_layer4" , 52 , 0 , 52 , cluster->eta_module() , correct_layer , "FTK #eta MODULE NUMBER" ); }
          if( cluster->barrel_ec()!=0 ) { dg::filleff( "correct_side_vs_eta_module_endcap_layer4" , 52 , 0 , 52 , cluster->eta_module() , correct_layer , "FTK #eta MODULE NUMBER" ); }
          dg::filleff( "correct_side_vs_eta_module_layer4" , 52 , 0 , 52 , cluster->eta_module() , correct_layer , "FTK #eta MODULE NUMBER" );
        }
        if( cluster->ftk_logical_layer()==5 ) {
          if( cluster->barrel_ec()==0 ) { dg::filleff( "correct_side_vs_phi_module_barrel_layer5" , 52 , 0 , 52 , cluster->phi_module() , correct_layer , "FTK #phi MODULE NUMBER" ); }
          if( cluster->barrel_ec()!=0 ) { dg::filleff( "correct_side_vs_phi_module_endcap_layer5" , 52 , 0 , 52 , cluster->phi_module() , correct_layer , "FTK #phi MODULE NUMBER" ); }
          dg::filleff( "correct_side_vs_phi_module_layer5" , 52 , 0 , 52 , cluster->phi_module() , correct_layer , "FTK #phi MODULE NUMBER" );
          if( cluster->barrel_ec()==0 ) { dg::filleff( "correct_side_vs_eta_module_barrel_layer5" , 52 , 0 , 52 , cluster->eta_module() , correct_layer , "FTK #eta MODULE NUMBER" ); }
          if( cluster->barrel_ec()!=0 ) { dg::filleff( "correct_side_vs_eta_module_endcap_layer5" , 52 , 0 , 52 , cluster->eta_module() , correct_layer , "FTK #eta MODULE NUMBER" ); }
          dg::filleff( "correct_side_vs_eta_module_layer5" , 52 , 0 , 52 , cluster->eta_module() , correct_layer , "FTK #eta MODULE NUMBER" );
        }
        if( cluster->ftk_logical_layer()==6 ) {
          if( cluster->barrel_ec()==0 ) { dg::filleff( "correct_side_vs_phi_module_barrel_layer6" , 52 , 0 , 52 , cluster->phi_module() , correct_layer , "FTK #phi MODULE NUMBER" ); }
          if( cluster->barrel_ec()!=0 ) { dg::filleff( "correct_side_vs_phi_module_endcap_layer6" , 52 , 0 , 52 , cluster->phi_module() , correct_layer , "FTK #phi MODULE NUMBER" ); }
          dg::filleff( "correct_side_vs_phi_module_layer6" , 52 , 0 , 52 , cluster->phi_module() , correct_layer , "FTK #phi MODULE NUMBER" );
          if( cluster->barrel_ec()==0 ) { dg::filleff( "correct_side_vs_eta_module_barrel_layer6" , 52 , 0 , 52 , cluster->eta_module() , correct_layer , "FTK #eta MODULE NUMBER" ); }
          if( cluster->barrel_ec()!=0 ) { dg::filleff( "correct_side_vs_eta_module_endcap_layer6" , 52 , 0 , 52 , cluster->eta_module() , correct_layer , "FTK #eta MODULE NUMBER" ); }
          dg::filleff( "correct_side_vs_eta_module_layer6" , 52 , 0 , 52 , cluster->eta_module() , correct_layer , "FTK #eta MODULE NUMBER" );
        }
        if( cluster->ftk_logical_layer()==7 ) {
          if( cluster->barrel_ec()==0 ) { dg::filleff( "correct_side_vs_phi_module_barrel_layer7" , 52 , 0 , 52 , cluster->phi_module() , correct_layer , "FTK #phi MODULE NUMBER" ); }
          if( cluster->barrel_ec()!=0 ) { dg::filleff( "correct_side_vs_phi_module_endcap_layer7" , 52 , 0 , 52 , cluster->phi_module() , correct_layer , "FTK #phi MODULE NUMBER" ); }
          dg::filleff( "correct_side_vs_phi_module_layer7" , 52 , 0 , 52 , cluster->phi_module() , correct_layer , "FTK #phi MODULE NUMBER" );
          if( cluster->barrel_ec()==0 ) { dg::filleff( "correct_side_vs_eta_module_barrel_layer7" , 52 , 0 , 52 , cluster->eta_module() , correct_layer , "FTK #eta MODULE NUMBER" ); }
          if( cluster->barrel_ec()!=0 ) { dg::filleff( "correct_side_vs_eta_module_endcap_layer7" , 52 , 0 , 52 , cluster->eta_module() , correct_layer , "FTK #eta MODULE NUMBER" ); }
          dg::filleff( "correct_side_vs_eta_module_layer7" , 52 , 0 , 52 , cluster->eta_module() , correct_layer , "FTK #eta MODULE NUMBER" );
        }
        if( cluster->ftk_logical_layer()==8 ) {
          if( cluster->barrel_ec()==0 ) { dg::filleff( "correct_side_vs_phi_module_barrel_layer8" , 52 , 0 , 52 , cluster->phi_module() , correct_layer , "FTK #phi MODULE NUMBER" ); }
          if( cluster->barrel_ec()!=0 ) { dg::filleff( "correct_side_vs_phi_module_endcap_layer8" , 52 , 0 , 52 , cluster->phi_module() , correct_layer , "FTK #phi MODULE NUMBER" ); }
          dg::filleff( "correct_side_vs_phi_module_layer8" , 52 , 0 , 52 , cluster->phi_module() , correct_layer , "FTK #phi MODULE NUMBER" );
          if( cluster->barrel_ec()==0 ) { dg::filleff( "correct_side_vs_eta_module_barrel_layer8" , 52 , 0 , 52 , cluster->eta_module() , correct_layer , "FTK #eta MODULE NUMBER" ); }
          if( cluster->barrel_ec()!=0 ) { dg::filleff( "correct_side_vs_eta_module_endcap_layer8" , 52 , 0 , 52 , cluster->eta_module() , correct_layer , "FTK #eta MODULE NUMBER" ); }
          dg::filleff( "correct_side_vs_eta_module_layer8" , 52 , 0 , 52 , cluster->eta_module() , correct_layer , "FTK #eta MODULE NUMBER" );
        }
        if( cluster->ftk_logical_layer()==9 ) {
          if( cluster->barrel_ec()==0 ) { dg::filleff( "correct_side_vs_phi_module_barrel_layer9" , 52 , 0 , 52 , cluster->phi_module() , correct_layer , "FTK #phi MODULE NUMBER" ); }
          if( cluster->barrel_ec()!=0 ) { dg::filleff( "correct_side_vs_phi_module_endcap_layer9" , 52 , 0 , 52 , cluster->phi_module() , correct_layer , "FTK #phi MODULE NUMBER" ); }
          dg::filleff( "correct_side_vs_phi_module_layer9" , 52 , 0 , 52 , cluster->phi_module() , correct_layer , "FTK #phi MODULE NUMBER" );
          if( cluster->barrel_ec()==0 ) { dg::filleff( "correct_side_vs_eta_module_barrel_layer9" , 52 , 0 , 52 , cluster->eta_module() , correct_layer , "FTK #eta MODULE NUMBER" ); }
          if( cluster->barrel_ec()!=0 ) { dg::filleff( "correct_side_vs_eta_module_endcap_layer9" , 52 , 0 , 52 , cluster->eta_module() , correct_layer , "FTK #eta MODULE NUMBER" ); }
          dg::filleff( "correct_side_vs_eta_module_layer9" , 52 , 0 , 52 , cluster->eta_module() , correct_layer , "FTK #eta MODULE NUMBER" );
        }
        if( cluster->ftk_logical_layer()==10 ) {
          if( cluster->barrel_ec()==0 ) { dg::filleff( "correct_side_vs_phi_module_barrel_layer10" , 52 , 0 , 52 , cluster->phi_module() , correct_layer , "FTK #phi MODULE NUMBER" ); }
          if( cluster->barrel_ec()!=0 ) { dg::filleff( "correct_side_vs_phi_module_endcap_layer10" , 52 , 0 , 52 , cluster->phi_module() , correct_layer , "FTK #phi MODULE NUMBER" ); }
          dg::filleff( "correct_side_vs_phi_module_layer10" , 52 , 0 , 52 , cluster->phi_module() , correct_layer , "FTK #phi MODULE NUMBER" );
          if( cluster->barrel_ec()==0 ) { dg::filleff( "correct_side_vs_eta_module_barrel_layer10" , 52 , 0 , 52 , cluster->eta_module() , correct_layer , "FTK #eta MODULE NUMBER" ); }
          if( cluster->barrel_ec()!=0 ) { dg::filleff( "correct_side_vs_eta_module_endcap_layer10" , 52 , 0 , 52 , cluster->eta_module() , correct_layer , "FTK #eta MODULE NUMBER" ); }
          dg::filleff( "correct_side_vs_eta_module_layer10" , 52 , 0 , 52 , cluster->eta_module() , correct_layer , "FTK #eta MODULE NUMBER" );
        }
        if( cluster->barrel_ec()==0 ) { dg::filleff( "other_side_vs_layer_barrel" , 12 , 0 , 12 , cluster->ftk_logical_layer() , other_layer , "FTK LOGICAL LAYER" ); }
        if( cluster->barrel_ec()!=0 ) { dg::filleff( "other_side_vs_layer_endcap" , 12 , 0 , 12 , cluster->ftk_logical_layer() , other_layer , "FTK LOGICAL LAYER" ); }
        dg::filleff( "other_side_vs_layer" , 12 , 0 , 12 , cluster->ftk_logical_layer() , other_layer , "FTK LOGICAL LAYER" );
        if( cluster->ftk_logical_layer()==3 ) {
          if( cluster->barrel_ec()==0 ) { dg::filleff( "other_side_vs_phi_module_barrel_layer3" , 52 , 0 , 52 , cluster->phi_module() , other_layer , "FTK #phi MODULE NUMBER" ); }
          if( cluster->barrel_ec()!=0 ) { dg::filleff( "other_side_vs_phi_module_endcap_layer3" , 52 , 0 , 52 , cluster->phi_module() , other_layer , "FTK #phi MODULE NUMBER" ); }
          dg::filleff( "other_side_vs_phi_module_layer3" , 52 , 0 , 52 , cluster->phi_module() , other_layer , "FTK #phi MODULE NUMBER" );
          if( cluster->barrel_ec()==0 ) { dg::filleff( "other_side_vs_eta_module_barrel_layer3" , 52 , 0 , 52 , cluster->eta_module() , other_layer , "FTK #eta MODULE NUMBER" ); }
          if( cluster->barrel_ec()!=0 ) { dg::filleff( "other_side_vs_eta_module_endcap_layer3" , 52 , 0 , 52 , cluster->eta_module() , other_layer , "FTK #eta MODULE NUMBER" ); }
          dg::filleff( "other_side_vs_eta_module_layer3" , 52 , 0 , 52 , cluster->eta_module() , other_layer , "FTK #eta MODULE NUMBER" );
        }
        if( cluster->ftk_logical_layer()==4 ) {
          if( cluster->barrel_ec()==0 ) { dg::filleff( "other_side_vs_phi_module_barrel_layer4" , 52 , 0 , 52 , cluster->phi_module() , other_layer , "FTK #phi MODULE NUMBER" ); }
          if( cluster->barrel_ec()!=0 ) { dg::filleff( "other_side_vs_phi_module_endcap_layer4" , 52 , 0 , 52 , cluster->phi_module() , other_layer , "FTK #phi MODULE NUMBER" ); }
          dg::filleff( "other_side_vs_phi_module_layer4" , 52 , 0 , 52 , cluster->phi_module() , other_layer , "FTK #phi MODULE NUMBER" );
          if( cluster->barrel_ec()==0 ) { dg::filleff( "other_side_vs_eta_module_barrel_layer4" , 52 , 0 , 52 , cluster->eta_module() , other_layer , "FTK #eta MODULE NUMBER" ); }
          if( cluster->barrel_ec()!=0 ) { dg::filleff( "other_side_vs_eta_module_endcap_layer4" , 52 , 0 , 52 , cluster->eta_module() , other_layer , "FTK #eta MODULE NUMBER" ); }
          dg::filleff( "other_side_vs_eta_module_layer4" , 52 , 0 , 52 , cluster->eta_module() , other_layer , "FTK #eta MODULE NUMBER" );
        }
        if( cluster->ftk_logical_layer()==5 ) {
          if( cluster->barrel_ec()==0 ) { dg::filleff( "other_side_vs_phi_module_barrel_layer5" , 52 , 0 , 52 , cluster->phi_module() , other_layer , "FTK #phi MODULE NUMBER" ); }
          if( cluster->barrel_ec()!=0 ) { dg::filleff( "other_side_vs_phi_module_endcap_layer5" , 52 , 0 , 52 , cluster->phi_module() , other_layer , "FTK #phi MODULE NUMBER" ); }
          dg::filleff( "other_side_vs_phi_module_layer5" , 52 , 0 , 52 , cluster->phi_module() , other_layer , "FTK #phi MODULE NUMBER" );
          if( cluster->barrel_ec()==0 ) { dg::filleff( "other_side_vs_eta_module_barrel_layer5" , 52 , 0 , 52 , cluster->eta_module() , other_layer , "FTK #eta MODULE NUMBER" ); }
          if( cluster->barrel_ec()!=0 ) { dg::filleff( "other_side_vs_eta_module_endcap_layer5" , 52 , 0 , 52 , cluster->eta_module() , other_layer , "FTK #eta MODULE NUMBER" ); }
          dg::filleff( "other_side_vs_eta_module_layer5" , 52 , 0 , 52 , cluster->eta_module() , other_layer , "FTK #eta MODULE NUMBER" );
        }
        if( cluster->ftk_logical_layer()==6 ) {
          if( cluster->barrel_ec()==0 ) { dg::filleff( "other_side_vs_phi_module_barrel_layer6" , 52 , 0 , 52 , cluster->phi_module() , other_layer , "FTK #phi MODULE NUMBER" ); }
          if( cluster->barrel_ec()!=0 ) { dg::filleff( "other_side_vs_phi_module_endcap_layer6" , 52 , 0 , 52 , cluster->phi_module() , other_layer , "FTK #phi MODULE NUMBER" ); }
          dg::filleff( "other_side_vs_phi_module_layer6" , 52 , 0 , 52 , cluster->phi_module() , other_layer , "FTK #phi MODULE NUMBER" );
          if( cluster->barrel_ec()==0 ) { dg::filleff( "other_side_vs_eta_module_barrel_layer6" , 52 , 0 , 52 , cluster->eta_module() , other_layer , "FTK #eta MODULE NUMBER" ); }
          if( cluster->barrel_ec()!=0 ) { dg::filleff( "other_side_vs_eta_module_endcap_layer6" , 52 , 0 , 52 , cluster->eta_module() , other_layer , "FTK #eta MODULE NUMBER" ); }
          dg::filleff( "other_side_vs_eta_module_layer6" , 52 , 0 , 52 , cluster->eta_module() , other_layer , "FTK #eta MODULE NUMBER" );
        }
        if( cluster->ftk_logical_layer()==7 ) {
          if( cluster->barrel_ec()==0 ) { dg::filleff( "other_side_vs_phi_module_barrel_layer7" , 52 , 0 , 52 , cluster->phi_module() , other_layer , "FTK #phi MODULE NUMBER" ); }
          if( cluster->barrel_ec()!=0 ) { dg::filleff( "other_side_vs_phi_module_endcap_layer7" , 52 , 0 , 52 , cluster->phi_module() , other_layer , "FTK #phi MODULE NUMBER" ); }
          dg::filleff( "other_side_vs_phi_module_layer7" , 52 , 0 , 52 , cluster->phi_module() , other_layer , "FTK #phi MODULE NUMBER" );
          if( cluster->barrel_ec()==0 ) { dg::filleff( "other_side_vs_eta_module_barrel_layer7" , 52 , 0 , 52 , cluster->eta_module() , other_layer , "FTK #eta MODULE NUMBER" ); }
          if( cluster->barrel_ec()!=0 ) { dg::filleff( "other_side_vs_eta_module_endcap_layer7" , 52 , 0 , 52 , cluster->eta_module() , other_layer , "FTK #eta MODULE NUMBER" ); }
          dg::filleff( "other_side_vs_eta_module_layer7" , 52 , 0 , 52 , cluster->eta_module() , other_layer , "FTK #eta MODULE NUMBER" );
        }
        if( cluster->ftk_logical_layer()==8 ) {
          if( cluster->barrel_ec()==0 ) { dg::filleff( "other_side_vs_phi_module_barrel_layer8" , 52 , 0 , 52 , cluster->phi_module() , other_layer , "FTK #phi MODULE NUMBER" ); }
          if( cluster->barrel_ec()!=0 ) { dg::filleff( "other_side_vs_phi_module_endcap_layer8" , 52 , 0 , 52 , cluster->phi_module() , other_layer , "FTK #phi MODULE NUMBER" ); }
          dg::filleff( "other_side_vs_phi_module_layer8" , 52 , 0 , 52 , cluster->phi_module() , other_layer , "FTK #phi MODULE NUMBER" );
          if( cluster->barrel_ec()==0 ) { dg::filleff( "other_side_vs_eta_module_barrel_layer8" , 52 , 0 , 52 , cluster->eta_module() , other_layer , "FTK #eta MODULE NUMBER" ); }
          if( cluster->barrel_ec()!=0 ) { dg::filleff( "other_side_vs_eta_module_endcap_layer8" , 52 , 0 , 52 , cluster->eta_module() , other_layer , "FTK #eta MODULE NUMBER" ); }
          dg::filleff( "other_side_vs_eta_module_layer8" , 52 , 0 , 52 , cluster->eta_module() , other_layer , "FTK #eta MODULE NUMBER" );
        }
        if( cluster->ftk_logical_layer()==9 ) {
          if( cluster->barrel_ec()==0 ) { dg::filleff( "other_side_vs_phi_module_barrel_layer9" , 52 , 0 , 52 , cluster->phi_module() , other_layer , "FTK #phi MODULE NUMBER" ); }
          if( cluster->barrel_ec()!=0 ) { dg::filleff( "other_side_vs_phi_module_endcap_layer9" , 52 , 0 , 52 , cluster->phi_module() , other_layer , "FTK #phi MODULE NUMBER" ); }
          dg::filleff( "other_side_vs_phi_module_layer9" , 52 , 0 , 52 , cluster->phi_module() , other_layer , "FTK #phi MODULE NUMBER" );
          if( cluster->barrel_ec()==0 ) { dg::filleff( "other_side_vs_eta_module_barrel_layer9" , 52 , 0 , 52 , cluster->eta_module() , other_layer , "FTK #eta MODULE NUMBER" ); }
          if( cluster->barrel_ec()!=0 ) { dg::filleff( "other_side_vs_eta_module_endcap_layer9" , 52 , 0 , 52 , cluster->eta_module() , other_layer , "FTK #eta MODULE NUMBER" ); }
          dg::filleff( "other_side_vs_eta_module_layer9" , 52 , 0 , 52 , cluster->eta_module() , other_layer , "FTK #eta MODULE NUMBER" );
        }
        if( cluster->ftk_logical_layer()==10 ) {
          if( cluster->barrel_ec()==0 ) { dg::filleff( "other_side_vs_phi_module_barrel_layer10" , 52 , 0 , 52 , cluster->phi_module() , other_layer , "FTK #phi MODULE NUMBER" ); }
          if( cluster->barrel_ec()!=0 ) { dg::filleff( "other_side_vs_phi_module_endcap_layer10" , 52 , 0 , 52 , cluster->phi_module() , other_layer , "FTK #phi MODULE NUMBER" ); }
          dg::filleff( "other_side_vs_phi_module_layer10" , 52 , 0 , 52 , cluster->phi_module() , other_layer , "FTK #phi MODULE NUMBER" );
          if( cluster->barrel_ec()==0 ) { dg::filleff( "other_side_vs_eta_module_barrel_layer10" , 52 , 0 , 52 , cluster->eta_module() , other_layer , "FTK #eta MODULE NUMBER" ); }
          if( cluster->barrel_ec()!=0 ) { dg::filleff( "other_side_vs_eta_module_endcap_layer10" , 52 , 0 , 52 , cluster->eta_module() , other_layer , "FTK #eta MODULE NUMBER" ); }
          dg::filleff( "other_side_vs_eta_module_layer10" , 52 , 0 , 52 , cluster->eta_module() , other_layer , "FTK #eta MODULE NUMBER" );
        }
      }
      // plot reference cluster properties vs track properties
      if( _reference_track ) {
        dg::fillh( "residual_truth_ref_wx_vs_pt" , 5 , 0 , 25 , 10 , 0 , 10 , _reference_track->pt() , match->width_x() , "PT (GeV)" , "WIDTH X (CHANNELS)");
        dg::fillh( "residual_truth_ref_wx_vs_phi" , 104 , -M_PI , M_PI , 10 , 0 , 10 , _reference_track->phi0() , match->width_x() , "#phi (rad)" , "WIDTH X (CHANNELS)" );
        dg::fillh( "residual_truth_ref_wx_vs_abslambda" , 20 , 0 , 5 , 10 , 0 , 10 , std::abs(_reference_track->lambda()) , match->width_x() , "cot(#theta)" , "WIDTH X (CHANNELS)" );
        dg::fillh( "residual_truth_local_dx_vs_pt" , 5 , 0 , 25 , 100 , -1 , 1 , _reference_track->pt() , dx , "PT (GeV)" , "#Delta X (CHANNELS)" );
        dg::fillh( "residual_truth_local_dx_vs_phi" , 104 , -M_PI , M_PI , 50 , -1 , 1 , _reference_track->phi0() , dx , "#phi (rad)" , "#Delta X (CHANNELS)"  );
        dg::fillh( "residual_truth_local_dx_vs_abslambda" , 20 , 0 , 5 , 100 , -1 , 1 , std::abs(_reference_track->lambda()) , dx , "cot(#theta)" , "#Delta X (CHANNELS)"  );
      }
      if( _reference_track && _comparison_track ) { 
        // plot track comparisons for "good" and "bad" cluster matches
        TrackIdentification tid( _reference_track , _comparison_track );
        StudyTrackComparison sttc( "track_residuals" , "track residual dependence on cluster residuals" );
        sttc.do_residuals( true );
        sttc.do_cluster_study( false );
        sttc.do_estimates( false );
        sttc.do_covariance_vs_residuals( false );
        sttc.do_residual_correlations( false );
        sttc.do_pulls( true );
        sttc.add_constant_category( "good_match_ftk_l0_dx" , match->ftk_logical_layer()==0 && std::abs(dx)<0.1 );
        sttc.add_constant_category( "good_match_ftk_l0_dz" , match->ftk_logical_layer()==0 && std::abs(dz)<0.1 );
        sttc.add_constant_category( "bad_match_ftk_l0_dx" , match->ftk_logical_layer()==0 && std::abs(dx)>0.1 );
        sttc.add_constant_category( "bad_match_ftk_l0_dz" , match->ftk_logical_layer()==0 && std::abs(dz)>0.1 );
        sttc.add_constant_category( "good_match_ftk_l3" , match->ftk_logical_layer()==3 && std::abs(dx)<0.1 );
        sttc.add_constant_category( "bad_match_ftk_l3" , match->ftk_logical_layer()==3 && std::abs(dx)>0.1 );
        sttc.add_constant_category( "good_match_ftk_l4" , match->ftk_logical_layer()==4 && std::abs(dx)<0.1 );
        sttc.add_constant_category( "bad_match_ftk_l4" , match->ftk_logical_layer()==4 && std::abs(dx)>0.1 );
        sttc.add_constant_category( "good_match_ftk_l5" , match->ftk_logical_layer()==5 && std::abs(dx)<0.1 );
        sttc.add_constant_category( "bad_match_ftk_l5" , match->ftk_logical_layer()==5 && std::abs(dx)>0.1 );
        sttc.add_constant_category( "good_match_ftk_l6" , match->ftk_logical_layer()==6 && std::abs(dx)<0.1 );
        sttc.add_constant_category( "bad_match_ftk_l6" , match->ftk_logical_layer()==6 && std::abs(dx)>0.1 );
        sttc.add_constant_category( "good_match_ftk_l7" , match->ftk_logical_layer()==7 && std::abs(dx)<0.1 );
        sttc.add_constant_category( "bad_match_ftk_l7" , match->ftk_logical_layer()==7 && std::abs(dx)>0.1 );
        sttc.add_constant_category( "good_match_ftk_l8" , match->ftk_logical_layer()==8 && std::abs(dx)<0.1 );
        sttc.add_constant_category( "bad_match_ftk_l8" , match->ftk_logical_layer()==8 && std::abs(dx)>0.1 );
        sttc.add_constant_category( "good_match_ftk_l9" , match->ftk_logical_layer()==9 && std::abs(dx)<0.1 );
        sttc.add_constant_category( "bad_match_ftk_l9" , match->ftk_logical_layer()==9 && std::abs(dx)>0.1 );
        sttc.add_constant_category( "good_match_ftk_l10" , match->ftk_logical_layer()==10 && std::abs(dx)<0.1 );
        sttc.add_constant_category( "bad_match_ftk_l10" , match->ftk_logical_layer()==10 && std::abs(dx)>0.1 );
        sttc.study_uncategorized( false );
        sttc.for_only( tid );
      }
    } // end if truth for reference cluster matches truth for comparison cluster
    if( false ) { 
      using namespace boost;
      using namespace std;
      cout << "cluster comparison: "
           << ( format("d:%|1d| ll:%|2d| ly:%|2d| m:%|2g|,%|2g| c:%|3d|,%|3d| lx:%|5.1g|,%|5.1g| w:%|2d|,%|2d| t:%|3d|,%|6d|") 
                % (int)cluster->cluster_type() % cluster->ftk_logical_layer() % -1 % -1 % -1
                % cluster->phi_index() % (cluster->is_pixel() ? cluster->eta_index() : cluster->sct_side())
                % cluster->local_x() % (cluster->is_pixel() ? cluster->local_z() : cluster->sct_side())
                % cluster->width_x() % (cluster->is_pixel() ? cluster->width_z() : 0.) 
                % cluster->barcode().event_index() % cluster->barcode().barcode()
                )

           << endl
           << "                    "
           << ( format("d:%|1d| ll:%|2d| ly:%|2d| m:%|2g|,%|2g| c:%|3d|,%|3d| lx:%|5.1g|,%|5.1g| w:%|2d|,%|2d| t:%|3d|,%|6d|") 
                % (int)match->cluster_type() % match->ftk_logical_layer() % match->layer_or_disk() % match->phi_module() % match->eta_module()
                % match->phi_index() % (match->is_pixel() ? match->eta_index() : match->sct_side())
                % match->local_x() % (match->is_pixel() ? match->local_z() : match->sct_side())
                % match->width_x() % (match->is_pixel() ? match->width_z() : 0.) 
                % cluster->barcode().event_index() % cluster->barcode().barcode()
                )
           << endl
           << "                                                "
           << ( format("dx:%|4.1g|,%|4.1g| w:%|2d|,%|2d|")
                % (cluster->local_x() - match->local_x())
                % (cluster->is_pixel() ? (cluster->local_z() - match->local_z()) : 0.)
                % (cluster->width_x() - match->width_x())
                % (cluster->is_pixel() ? (cluster->width_z() - match->width_z()) : 0.)
                )
           << endl;
    }
  }
public:
  StudySiCluster( const std::string& name , const std::string& description ) 
    : DataGraphics::DgStudy<type>(name,description)
    , _reference_track()
    , _ref_clusters()
    , _comparison_track()
  {}
  virtual ~StudySiCluster() {}
  void reference_track( const boost::shared_ptr<const AnaTrack>& track ) { _reference_track = track; }
  void reference_clusters( const std::vector< boost::shared_ptr<const AnaSiCluster> >* reference_clusters ) { _ref_clusters = reference_clusters; }
  void comparison_track( const boost::shared_ptr<const AnaTrack>& track ) { _comparison_track = track; }
};

#endif // WRAP_STUDYSICLUSTER_HH

