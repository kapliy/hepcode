#ifndef WRAP_STUDYMUON_HH
#define WRAP_STUDYMUON_HH


#include <string>
#include <boost/shared_ptr.hpp>
#include "DataGraphics/DgStudy.hh"
#include "DataGraphics/DgCollection.hh"
#include "TrigFTKAna/AnaMuon.hpp"
#include "TrigFTKAna/Detector.hpp"
#include "TrigFTKAna/StudyParticle.hpp"
#include "TrigFTKAna/StudyTrack.hpp"

class
StudyMuon : public DataGraphics::DgStudy< boost::shared_ptr<const AnaMuon> >
{
public:
  typedef boost::shared_ptr<const AnaMuon>  type; // must be same type as the template argument for DgStudy.
  static void study( type const& mu ) {
    using namespace DataGraphics;
    assert( mu->type()==AnaParticle::MUON );
    // fill standard kinematic plots
    StudyParticle::study( mu );
    // fill muon-specific plots
    dg::fillh( "primary_author" , 15 , 0 , 15 , mu->primary_author() , "MUON PRIMARY AUTHOR" );
    dg::fillh( "all_author_bitmask" , 15 , 0 , 15 , mu->all_author_bitmask() , "MUON ALL AUTHOR BITMASK" );
    dg::filleff( "is_corrected" , mu->is_corrected() );
    dg::filleff( "is_muonboy" , mu->is_muonboy() );
    dg::filleff( "is_staco_combined" , mu->is_staco_combined() );
    dg::filleff( "is_staco_any" , mu->is_staco_any() );
    dg::filleff( "is_mutag" , mu->is_mutag() );
    dg::filleff( "is_muid_standalone" , mu->is_muid_standalone() );
    dg::filleff( "is_muid_combined" , mu->is_muid_combined() );
    dg::filleff( "is_mugirl" , mu->is_mugirl() );
    dg::filleff( "is_calo_muon_id" , mu->is_calo_muon_id() );
    dg::filleff( "is_calo_muon_tag" , mu->is_calo_muon_tag() );
    dg::filleff( "is_calo_muon_likelihood" , mu->is_calo_muon_likelihood() );
    dg::filleff( "is_mutag_imo" , mu->is_mutag_imo() );
    dg::filleff( "is_combined_refit" , mu->is_combined_refit() );
    dg::filleff( "is_ip_extrapolated" , mu->is_ip_extrapolated() );
    dg::filleff( "is_muid" , mu->is_muid() );
    dg::filleff( "is_combined" , mu->is_combined() );
    dg::filleff( "is_calo" , mu->is_calo() );
    dg::fillh( "charge" , 3 , -1 , 2 , mu->charge() , "MUON CHARGE" );
    dg::fillh( "ms_charge" , 3 , -1 , 2 , mu->ms_charge() , "MS MUON CHARGE" );
    dg::fillh( "exms_charge" , 3 , -1 , 2 , mu->exms_charge() , "EXMS MUON CHARGE" );
    dg::fillh( "id_charge" , 3 , -1 , 2 , mu->id_charge() , "ID MUON CHARGE" );
    dg::fillh( "nhits_pixel" , 15 , 0 , 15 , mu->nhits( AnaMuon::PIXEL ) , "Number of pixel hits" );
    dg::fillh( "nhits_bl" , 15 , 0 , 15 , mu->nhits( AnaMuon::BL ) , "Number of b layer pixel hits" );
    dg::fillh( "nhits_sct" , 15 , 0 , 15 , mu->nhits( AnaMuon::SCT ) , "Number of SCT hits" );
    dg::fillh( "nhits_silicon" , 15 , 0 , 15 , mu->nhits_silicon() , "Number of silicon hits" );
    dg::fillh( "nhits_trt" , 15 , 0 , 15 , mu->nhits( AnaMuon::TRT ) , "Number of TRT hits" );
    dg::fillh( "nhits_trt_outlier" , 15 , 0 , 15 , mu->nhits( AnaMuon::TRTOUTLIER ) , "Number of TRT outlier hits" );
    dg::fillh( "nhole_pixel" , 15 , 0 , 15 , mu->nholes( AnaMuon::PIXHOLE ) , "Number of pixel holes" );
    dg::fillh( "nhole_sct" , 15 , 0 , 15 , mu->nholes( AnaMuon::SCTHOLE ) , "Number of SCT holes" );
    dg::fillh( "ndead_pixel" , 15 , 0 , 15 , mu->ndead( AnaMuon::PIXDEAD ) , "Number of pixel dead" );
    dg::fillh( "ndead_sct" , 15 , 0 , 15 , mu->ndead( AnaMuon::SCTDEAD ) , "Number of SCT dead" );
    dg::fillh( "ndead_trt" , 15 , 0 , 15 , mu->ndead( AnaMuon::TRTDEAD ) , "Number of TRT dead" );
    dg::filleff( "passes_hits_mcp_v17" , mu->passes_hits_mcp_v17_moriond() );

    dg::fillh( "pt_IDMS_diff" , 100 , -10 , 10 , mu->pt_difference_id_ms() , "ID p_{T} - EXMS p_{T} (GeV)" );
    dg::fillh( "pt_IDMS_diff_ratio" , 100 , -2 , 2 , mu->pt_diff_ratio() , "ID p_{T} - EXMS p_{T} / ID p_{T}" );
    dg::fillh( "ms_pt" , 100 , 0 , 100 , mu->ms_muon().Pt() , "MS MUON p_{T}" );
    dg::fillh( "ms_eta" , 80 , -4 , 40 , detector::etaFromTLorentzVector<AnaMuon::ftype>( mu->ms_muon() ) , "MS MUON #eta");
    dg::fillh( "ms_phi" , 48 , -M_PI , M_PI , mu->ms_muon().Phi() , "MS MUON #phi (rad)");
    dg::fillh( "exms_pt" , 100 , 0 , 100 , mu->exms_muon().Pt() , "MS MUON p_{T}" );
    dg::fillh( "exms_eta" , 80 , -4 , 40 , detector::etaFromTLorentzVector<AnaMuon::ftype>( mu->exms_muon() ) , "MS MUON #eta");
    dg::fillh( "exms_phi" , 48 , -M_PI , M_PI , mu->exms_muon().Phi() , "MS MUON #phi (rad)");
    dg::fillh( "id_pt" , 100 , 0 , 100 , mu->id_muon().Pt() , "MS MUON p_{T}" );
    dg::fillh( "id_eta" , 80 , -4 , 40 , detector::etaFromTLorentzVector<AnaMuon::ftype>( mu->id_muon() ) , "MS MUON #eta");
    dg::fillh( "id_phi" , 48 , -M_PI , M_PI , mu->id_muon().Phi() , "MS MUON #phi (rad)");

    dg::filleff( "has_track" , mu->has_track() );
    if( mu->has_track() ) { 
      StudyTrack::study( mu->track() ); 
      dg::fillh( "track_d0_pv" , 100 , -1 , 1 , mu->exPV_d0() , "Track Vertex-Corrected d_{0} (mm)" );
      dg::fillh( "track_d0_pv_err" , 100 , -1 , 1 , mu->exPV_d0_err() , "Track Vertex-Corrected d_{0} uncertainty (mm)" );
      dg::fillh( "track_d0_pv_signif" , 100 , -10 , 10 , mu->exPV_d0_signif() , "Track Vertex-Corrected d_{0} significance (#sigma)" );
      dg::fillh( "track_z0_pv" , 100 , -10 , 10 , mu->exPV_z0() , "Track Vertex-Corrected z_{0} (mm)" );
    }
    dg::fillh( "d0" , 100 , -15 , 15 , mu->d0());
    dg::fillh( "z0" , 100 , -150 , 150 , mu->z0());
    dg::fillh( "d0_exPV" , 100 , -15 , 15 , mu->d0_exPV());
    dg::fillh( "z0_exPV" , 100 , -150 , 150 , mu->z0_exPV());
    dg::fillh( "matchchi2" , 100 , 0 , 25 , mu->matchchi2() , "MATCH #chi^{2}" );

    dg::fillh( "z0" , 100 , -150 , 150 , mu->z0());
    
    dg::fillh( "pt_cone_10" , 100 , 0 , 50 , mu->pt_cone( AnaMuon::CONE10 ) , "p_{T} Cone 10 (GeV)" );
    dg::fillh( "pt_cone_20" , 100 , 0 , 50 , mu->pt_cone( AnaMuon::CONE20 ) , "p_{T} Cone 20 (GeV)" );
    dg::fillh( "pt_cone_30" , 100 , 0 , 50 , mu->pt_cone( AnaMuon::CONE30 ) , "p_{T} Cone 30 (GeV)" );
    dg::fillh( "pt_cone_40" , 100 , 0 , 50 , mu->pt_cone( AnaMuon::CONE40 ) , "p_{T} Cone 40 (GeV)" );
    dg::fillh( "pt_cone_ratio_10" , 100 , 0 , 4 , mu->pt_cone_ratio( AnaMuon::CONE10 ) , "p_{T} Cone 10 Ratio (GeV)" );
    dg::fillh( "pt_cone_ratio_20" , 100 , 0 , 4 , mu->pt_cone_ratio( AnaMuon::CONE20 ) , "p_{T} Cone 20 Ratio (GeV)" );
    dg::fillh( "pt_cone_ratio_30" , 100 , 0 , 4 , mu->pt_cone_ratio( AnaMuon::CONE30 ) , "p_{T} Cone 30 Ratio (GeV)" );
    dg::fillh( "pt_cone_ratio_40" , 100 , 0 , 4 , mu->pt_cone_ratio( AnaMuon::CONE40 ) , "p_{T} Cone 40 Ratio (GeV)" );
    dg::fillh( "et_cone_10" , 100 , 0 , 50 , mu->et_cone( AnaMuon::CONE10 ) , "E_{T} Cone 10 (GeV)" );
    dg::fillh( "et_cone_20" , 100 , 0 , 50 , mu->et_cone( AnaMuon::CONE20 ) , "E_{T} Cone 20 (GeV)" );
    dg::fillh( "et_cone_30" , 100 , 0 , 50 , mu->et_cone( AnaMuon::CONE30 ) , "E_{T} Cone 30 (GeV)" );
    dg::fillh( "et_cone_40" , 100 , 0 , 50 , mu->et_cone( AnaMuon::CONE40 ) , "E_{T} Cone 40 (GeV)" );
    dg::fillh( "et_cone_ratio_10" , 100 , 0 , 4 , mu->et_cone_ratio( AnaMuon::CONE10 ) , "E_{T} Cone 10 Ratio (GeV)" );
    dg::fillh( "et_cone_ratio_20" , 100 , 0 , 4 , mu->et_cone_ratio( AnaMuon::CONE20 ) , "E_{T} Cone 20 Ratio (GeV)" );
    dg::fillh( "et_cone_ratio_30" , 100 , 0 , 4 , mu->et_cone_ratio( AnaMuon::CONE30 ) , "E_{T} Cone 30 Ratio (GeV)" );
    dg::fillh( "et_cone_ratio_40" , 100 , 0 , 4 , mu->et_cone_ratio( AnaMuon::CONE40 ) , "E_{T} Cone 40 Ratio (GeV)" );

  }

protected:
  virtual void _study( type const& muon ) { // the second const is important---matches the base class.
    using namespace DataGraphics;
    StudyMuon::study(muon);
    // if MET was provided, make some extra 2d plots
    if(_met) {
      dg::fillh( "pt_vs_MET" , 200 , 0 , 200 , 200, 0, 200, _met->met(), muon->pt(), "MET", "Muon pT");
      dg::fillh( "iso40_vs_MET" , 200 , 0 , 200 , 200, 0, 1, _met->met(), muon->pt_cone_ratio(AnaMuon::CONE40), "MET", "Muon pt iso40");
    }
  } // end study of each muon
protected:
  boost::shared_ptr<const AnaEvent> _event;
  boost::shared_ptr<const AnaMET> _met;
public:
  StudyMuon( const std::string& name , const std::string& description ) 
    : _event()
    , _met()
    , DataGraphics::DgStudy<type>(name,description)
  {}
  virtual ~StudyMuon() {}
  void for_event( const boost::shared_ptr<const AnaEvent>& event ) { _event = event; }
  void for_met( const boost::shared_ptr<const AnaMET>& met ) { _met = met; }
};

#endif // WRAP_STUDYMUON_HH

