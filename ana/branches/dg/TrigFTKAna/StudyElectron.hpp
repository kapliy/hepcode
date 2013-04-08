#ifndef WRAP_STUDYELECTRON_HH
#define WRAP_STUDYELECTRON_HH


#include <cmath>
#include <string>
#include <boost/shared_ptr.hpp>
#include "DataGraphics/DgStudy.hh"
#include "DataGraphics/DgCollection.hh"
#include "TrigFTKAna/AnaElectron.hpp"
#include "TrigFTKAna/StudyParticle.hpp"
#include "TrigFTKAna/StudyTrack.hpp"
#include "TrigFTKAna/Detector.hpp"

class
StudyElectron : public DataGraphics::DgStudy< boost::shared_ptr<const AnaElectron> >
{
public:
  typedef boost::shared_ptr<const AnaElectron>  type; // must be same type as the template argument for DgStudy.
  static void study( type const& ele ) {
    using namespace DataGraphics;
    assert( ele->type()==AnaParticle::ELECTRON );
    // fill standard kinematic plots
    StudyParticle::study( ele );
    // fill electron-specific plots
    dg::fillh( "author" , 10 , 0 , 10 , ele->author() , "ELECTRON AUTHOR" );
    dg::fillh( "is_em" , 10 , 0 , 512 , ele->is_em() , "ELECTRON IS_EM" );

    dg::filleff( "is_ElectronLoose_v16" , ele->is_ElectronLoose_v16() );
    dg::filleff( "is_ElectronMedium_v16" , ele->is_ElectronMedium_v16() );
    dg::filleff( "is_ElectronTight_v16" , ele->is_ElectronTight_v16() );
    dg::filleff( "is_ElectronLooseForward_v16" , ele->is_ElectronLooseForward_v16() );
    dg::filleff( "is_ElectronTightForward_v16" , ele->is_ElectronTightForward_v16() );
    dg::filleff( "is_ElectronLoosePlusPlus_v16" , ele->is_ElectronLoosePlusPlus_v16() );
    dg::filleff( "is_ElectronMediumPlusPlus_v16" , ele->is_ElectronMediumPlusPlus_v16() );
    dg::filleff( "is_ElectronLooseNotTight_v16" , ele->is_ElectronLooseNotTight_v16() );
    dg::filleff( "is_QCD_v16" , ele->is_QCD_v16() );
    dg::filleff( "is_PhotonLoose" , ele->is_PhotonLoose() );
    dg::fillh( "rhad1" , 100 , 0 , 1 , ele->rhad1() );
    dg::fillh( "rhad" , 100 , 0 , 1 , ele->rhad() );
    dg::fillh( "reta" , 100 , 0 , 1.5 , ele->reta() );
    dg::fillh( "weta2" , 100 , 0 , 0.5 , ele->weta2() );
    dg::fillh( "wstot" , 100 , 0 , 5 , ele->wstot() );
    dg::fillh( "deltaeta" , 100 , -0.05 , 0.05 , ele->deltaeta() );
    dg::fillh( "deltaphi" , 100 , -0.2 , 0.2 , ele->deltaphi() );
    dg::fillh( "eoverp" , 100 , 0 , 5 , ele->eoverp() );
    dg::fillh( "demaxs1" , 100 , 0 , 0.04 , ele->demaxs1() );
    dg::fillh( "eta2" , 100 , -5 , 5 , ele->eta2() );
    dg::fillh( "et2" , 100 , 0 , 100 , ele->et2() );
    if( ele->has_track() ) { 
      dg::fillh( "trt_nhits" , 40 , 0 , 40 , ele->track()->trt_nhits() );
      dg::fillh( "trratio" , 100 , 0 , 1 , ele->track()->trratio() );
      dg::fillh( "expect_b_layer_hits" , 5 , 0 , 5 , ele->track()->expect_b_layer_hits() );
      dg::fillh( "n_b_layer_outliers" , 5 , 0 , 5 , ele->track()->n_b_layer_outliers() );
      dg::fillh( "n_pixel_outliers" , 10 , 0 , 10 , ele->track()->n_pixel_outliers() );
    }
    dg::filleff( "pass_otx" , ele->pass_otx() );
    dg::fillh( "perEF_feb_eta_phi" , 160 , -0.1 , 1.5 , 100 , -1 , -0.5 , ele->pt() > 0. ? ele->eta() : 9999999. , ele->phi(), "#eta", "#phi (rad)" );
    dg::filleff( "is_corrected" , ele->is_corrected() );
    dg::fillh( "charge" , 3 , -1 , 2 , ele->charge() , "ELECTRON CHARGE" );
    dg::fillh( "composite_pt" , 100 , 0 , 100 , ele->four_vector_for_composites().Pt() , "p_{T} (GeV)" );
    dg::fillh( "composite_eta" , 40 , -4 , 4 , detector::etaFromTLorentzVector<AnaElectron::ftype>( ele->four_vector_for_composites() ) , "#eta" );
    dg::fillh( "composite_phi" , 48 , -M_PI , M_PI , ele->four_vector_for_composites().Phi() , "#phi" );
    dg::fillh( "cluster_pt" , 100 , 0 , 100 , ele->cluster_momentum().Pt() , "p_{T} (GeV)" );
    dg::fillh( "cluster_eta" , 40 , -4 , 4 , detector::etaFromTLorentzVector<AnaElectron::ftype>( ele->cluster_momentum() ) , "#eta" );
    dg::fillh( "cluster_phi" , 48 , -M_PI , M_PI , ele->cluster_momentum().Phi() , "#phi" );
    dg::fillh( "pt_cone_10" , 100 , 0 , 50 , ele->pt_cone( AnaElectron::CONE10 ) , "p_{T} Cone 10 (GeV)" );
    dg::fillh( "pt_cone_20" , 100 , 0 , 50 , ele->pt_cone( AnaElectron::CONE20 ) , "p_{T} Cone 20 (GeV)" );
    dg::fillh( "pt_cone_30" , 100 , 0 , 50 , ele->pt_cone( AnaElectron::CONE30 ) , "p_{T} Cone 30 (GeV)" );
    dg::fillh( "pt_cone_40" , 100 , 0 , 50 , ele->pt_cone( AnaElectron::CONE40 ) , "p_{T} Cone 40 (GeV)" );
    dg::fillh( "pt_cone_ratio_10" , 100 , 0 , 4 , ele->pt_cone_ratio( AnaElectron::CONE10 ) , "p_{T} Cone 10 Ratio (GeV)" );
    dg::fillh( "pt_cone_ratio_20" , 100 , 0 , 4 , ele->pt_cone_ratio( AnaElectron::CONE20 ) , "p_{T} Cone 20 Ratio (GeV)" );
    dg::fillh( "pt_cone_ratio_30" , 100 , 0 , 4 , ele->pt_cone_ratio( AnaElectron::CONE30 ) , "p_{T} Cone 30 Ratio (GeV)" );
    dg::fillh( "pt_cone_ratio_40" , 100 , 0 , 4 , ele->pt_cone_ratio( AnaElectron::CONE40 ) , "p_{T} Cone 40 Ratio (GeV)" );
    dg::fillh( "et_cone_10" , 100 , 0 , 50 , ele->et_cone( AnaElectron::CONE10 ) , "E_{T} Cone 10 (GeV)" );
    dg::fillh( "et_cone_20" , 100 , 0 , 50 , ele->et_cone( AnaElectron::CONE20 ) , "E_{T} Cone 20 (GeV)" );
    dg::fillh( "et_cone_30" , 100 , 0 , 50 , ele->et_cone( AnaElectron::CONE30 ) , "E_{T} Cone 30 (GeV)" );
    dg::fillh( "et_cone_40" , 100 , 0 , 50 , ele->et_cone( AnaElectron::CONE40 ) , "E_{T} Cone 40 (GeV)" );
    dg::fillh( "et_cone_ratio_10" , 100 , 0 , 4 , ele->et_cone_ratio( AnaElectron::CONE10 ) , "E_{T} Cone 10 Ratio (GeV)" );
    dg::fillh( "et_cone_ratio_20" , 100 , 0 , 4 , ele->et_cone_ratio( AnaElectron::CONE20 ) , "E_{T} Cone 20 Ratio (GeV)" );
    dg::fillh( "et_cone_ratio_30" , 100 , 0 , 4 , ele->et_cone_ratio( AnaElectron::CONE30 ) , "E_{T} Cone 30 Ratio (GeV)" );
    dg::fillh( "et_cone_ratio_40" , 100 , 0 , 4 , ele->et_cone_ratio( AnaElectron::CONE40 ) , "E_{T} Cone 40 Ratio (GeV)" );
    dg::fillh( "nhits_pixel" , 10 , 0 , 10 , ele->nhits( AnaElectron::PIXEL ) , "Number of pixel hits" );
    dg::fillh( "nhits_bl" , 10 , 0 , 10 , ele->nhits( AnaElectron::BL ) , "Number of b layer pixel hits" );
    dg::fillh( "nhits_sct" , 10 , 0 , 10 , ele->nhits( AnaElectron::SCT ) , "Number of SCT hits" );
    dg::fillh( "nhits_trt" , 10 , 0 , 10 , ele->nhits( AnaElectron::TRT ) , "Number of TRT hits" );
    dg::filleff( "has_track" , ele->has_track() );
    if( ele->has_track() ) { 
      StudyTrack::study( ele->track() ); 
      dg::fillh( "track_pt_to_cluster_pt" , 100 , 0 , 2 , detector::safe_divide( ele->track_pt() , ele->cluster_pt() ) , "track p_{T} / cluster p_{T}" );
      dg::fillh( "track_pt_to_ele_pt" , 100 , 0 , 2 , detector::safe_divide( ele->track_pt() , ele->pt() ) , "track p_{T} / ele p_{T}" );
      dg::fillh( "track_pt" , 100 , 0 , 100 , ele->track_momentum().Pt() , "p_{T} (GeV)" );
      dg::fillh( "track_eta" , 40 , -4 , 4 , detector::etaFromTLorentzVector<AnaElectron::ftype>( ele->track_momentum() ) , "#eta" );
      dg::fillh( "track_phi" , 48 , -M_PI , M_PI , ele->track_momentum().Phi() , "#phi" );
      dg::fillh( "track_d0_pv" , 100 , -1 , 1 , ele->trackd0pv() , "Track Vertex-Corrected d_{0} (mm)" );
      dg::fillh( "track_d0_pv_err" , 100 , -1 , 1 , ele->trackerrd0pv() , "Track Vertex-Corrected d_{0} error" );
      dg::fillh( "track_z0_pv" , 100 , -10 , 10 , ele->trackz0pv() , "Track Vertex-Corrected z_{0} (mm)" );
    }
  }
protected:
  virtual void _study( type const& ele ) { // the second const is important---matches the base class.
    StudyElectron::study(ele);
    return;
  } // end study of each electron
public:
  StudyElectron( const std::string& name , const std::string& description ) 
    : DataGraphics::DgStudy<type>(name,description)
  {}
  virtual ~StudyElectron() {}
};

#endif // WRAP_STUDYELECTRON_HH

