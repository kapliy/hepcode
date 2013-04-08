#ifndef WRAP_STUDYJET_HH
#define WRAP_STUDYJET_HH


#include <string>
#include <boost/shared_ptr.hpp>
#include "DataGraphics/DgStudy.hh"
#include "DataGraphics/DgCollection.hh"
#include "TrigFTKAna/AnaJet.hpp"
#include "TrigFTKAna/Detector.hpp"

class
StudyJet : public DataGraphics::DgStudy< boost::shared_ptr<const AnaJet> >
{
public:
  typedef boost::shared_ptr<const AnaJet>  type; // must be same type as the template argument for DgStudy.
protected:
  bool _study_b_tagging;
  bool _study_truth_composition;
  bool _study_jet_quality;
protected:
  virtual void _study( type const& jet ) { // the second const is important---matches the base class.
    using namespace DataGraphics;
    dg::fillh( "eta" , 100 , -4 , 4 , jet->eta() );
    dg::fillh( "phi" , 96 , -M_PI , M_PI , jet->phi() );
    dg::fillh( "et" , 200 , 0 , 100 , jet->et() );
    dg::fillh( "jet_dr_b" , 100 , 0 , 1.0 , jet->dr_b_parton() );
    dg::fillh( "jet_dr_c" , 100 , 0 , 1.0 , jet->dr_c_parton() );
    dg::fillh( "jet_dr_light" , 100 , 0 , 1.0 , jet->dr_light_parton() );
    dg::fillh( "jet_dr_any_b" , 100 , 0 , 1.0 , jet->dr_b_parton_any() );
    dg::fillh( "jet_dr_any_c" , 100 , 0 , 1.0 , jet->dr_c_parton_any() );
    dg::fillh( "jet_dr_any_light" , 100 , 0 , 1.0 , jet->dr_light_parton_any() );
    if( _study_b_tagging ) {
      dg::fillh( "tag_ip2d_w" , 110 , -10 , 100 , jet->tag_ip2d_w() ); 
      dg::fillh( "tag_ip3d_w" , 110 , -10 , 100 , jet->tag_ip3d_w() ); 
      dg::fillh( "tag_sv0_w" , 125 , -50 , 200 , jet->tag_sv0_w() ); 
      dg::fillh( "tag_sv1_w" , 100 , -10 , 20 , jet->tag_sv1_w() ); 
      dg::fillh( "tag_sv2_w" , 100 , -10 , 20 , jet->tag_sv2_w() ); 
      dg::fillh( "tag_cmb_w" , 120 , -20 , 100 , jet->tag_cmb_w() ); 
      dg::fillh( "tag_jetp_w" , 100 , 0 , 1 , jet->tag_jetp_w() ); 
      dg::fillh( "tag_trkc_w" , 100 , 0 , 1 , jet->tag_trkc_w() ); 
    }
    if( _study_truth_composition ) {
      const double min_dr_b = jet->dr_b_parton();
      const double min_dr_c = jet->dr_c_parton();
      const double min_dr_light = jet->dr_light_parton();
      // is this a b jet?
      const bool is_b = min_dr_b < 0.1;
      // is a b jet nearby?
      const bool is_light = !is_b;
      const bool is_pure_light = is_light && min_dr_b > 0.3;
      dg::filleff( "is_b_vs_et" , 100 , 0 , 100 , jet->et() , is_b , "JET E_{T}" );
      dg::filleff( "is_light_vs_et" , 100 , 0 , 100 , jet->et() , is_light , "JET E_{T}" );
      dg::filleff( "is_pure_light_vs_et" , 100 , 0 , 100 , jet->et() , is_pure_light , "JET E_{T}" );
      dg::filleff( "is_b_vs_min_dr_b" , 70 , 0 , 0.7 , min_dr_b , is_b , "MIN DR(jet,b jet)" );
      dg::filleff( "is_light_vs_min_dr_b" , 70 , 0 , 0.7 , min_dr_b , is_light , "MIN DR(jet,b jet)" );
      dg::filleff( "is_pure_light_vs_min_dr_b" , 70 , 0 , 0.7 , min_dr_b , is_pure_light , "MIN DR(jet,b jet)" );
    }
    if( _study_jet_quality ) { 
      dg::fillh( "quality_hecf" , 100 , 0 , 1 , jet->hecf() ? *(jet->hecf()) : -1. , "HECF" );
      dg::fillh( "quality_n90" , 100 , 0 , 100 , jet->n90() , "N90" );
      dg::fillh( "quality_emf" , 100 , 0 , 1 , jet->emfrac() , "EM Fraction" );
      dg::fillh( "quality_timing" , 100 , -100 , 100 , jet->timing() ? *(jet->timing()) : -1. , "Timing (ns)" );
      dg::filleff( "quality_is_ugly" , jet->is_ugly() );
      dg::filleff( "quality_is_bad" , jet->is_bad() );
      dg::filleff( "quality_is_good" , jet->is_good() );
      dg::filleff( "quality_is_good_for_met" , jet->is_good_for_met() );
      dg::fillh( "quality_vertex_fraction" , 100 , 0 , 1 , jet->vtx_fraction() , "Jet Charged Track Vertex Fraction");
    }
  } // end study of each jet
public:
  StudyJet( const std::string& name , const std::string& description ) 
    : DataGraphics::DgStudy<type>(name,description)
    , _study_b_tagging(false)
    , _study_truth_composition(false)
    , _study_jet_quality(false)
  {}
  virtual ~StudyJet() {}
  void study_b_tagging( const bool yes = true ) { _study_b_tagging = yes; }
  void study_truth_composition( const bool yes = true ) { _study_truth_composition = yes; }
  void study_jet_quality( const bool yes = true ) { _study_jet_quality = yes; }
};

#endif // WRAP_STUDYJET_HH

