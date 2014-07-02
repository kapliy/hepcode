#ifndef WRAP_STUDYTRACK_HH
#define WRAP_STUDYTRACK_HH

// "visitor"/"functor" to make standard track parameter plots. also
// serves as an example of how to write a simple study.
// 
// if you don't know what a "visitor" or a "functor" is, don't worry
// too much. the point is that it fills a bunch of histograms in the
// "_study" function, and the rest is boilerplate.
// 
// given a collection of tracks, one can create a directory containing
// plots of the pt, curvature, d0, etc. of the tracks as follows:
//
//   StudyTrack study( "directory_name" , "directory description" );
//   study.for_each( tracks.begin() , tracks.end() );
// 
// instead of plotting all tracks, one can plot only the tracks
// satisfying some specific criteria. for example (after #include <boost/bind.hpp>),
//
//   StudyTrack study( "directory_name" , "directory description" );
//   study.for_each_if( tracks.begin() , tracks.end() , boost::bind(&AnaTrack::pt,_1) > 1. &&
//                                                      boost::bind(&AnaTrack::nmisses,_1) == 0 );
//  
// alternatively, one can make a directory containing plots for all
// tracks in the collection and, separately, subdirectories of plots
// for certain subsets of tracks:
//
//   StudyTrack study( "directory_name" , "directory description" );
//   study.add_category( "subdir_name_1" , boost::bind( &AnaTrack::chi2 ) < 10. );
//   study.add_category( "subdir_name_2" , boost::bind( ptr_fun<AnaTrack::ftype>(std::abs) , boost::bind(&AnaTrack::eta,_1) ) < 1. ); // abs(AnaTrack::eta) < 1
//   study.for_each( tracks.begin() , tracks.end() );
// 

#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/scope_exit.hpp>
#include "DataGraphics/DgStudy.hh"
#include "DataGraphics/DgCollection.hh"
#include "TrigFTKAna/AnaTrack.hpp"
#include "TrigFTKAna/AnaSiCluster.hpp"
#include "TrigFTKAna/StudySiCluster.hpp"
#include "TrigFTKAna/Detector.hpp"
#include "TrigFTKAna/AnaEvent.hpp"

class
StudyTrack : public DataGraphics::DgStudy< boost::shared_ptr<const AnaTrack> >
{
public:
  typedef boost::shared_ptr<const AnaTrack> type; // must be same type as the template argument for DgStudy.
public:
  static void study( type const& track ) {
    using namespace DataGraphics;
    // fill some one-dimensional histograms. the parameter order is:
    // name , nbins , xmin , xmax , x.  note that one never needs to
    // declare the histogram at some earlier point, or save it at some
    // later point.  those things are taken care of automatically. so,
    // to add a new histogram to what is done below, just add a new
    // line.
    const std::string track_name = track->algorithm().name();
    dg::fillh( "pt" , 100 , 0 , 25. , track->pt() , track_name + " TRACK p_{T} (GeV)" );
    dg::fillh( "curv" , 100 , -0.5 , 0.5 , track->curvature()*1000. , track_name + " TRACK CURVATURE (1/GeV)" );
    dg::fillh( "d0" , 400 , -1 , 1 , track->d0()/10. , track_name + " TRACK d_{0} (cm)" );
    dg::fillh( "z0" , 200 , -30 , 30 , track->z0()/10. , track_name + " TRACK z_{0} (cm)" );
    dg::fillh( "eta" , 60 , -3 , 3. , track->eta() , track_name + " #eta" );
    dg::fillh( "phi" , 48 , -M_PI , M_PI , track->phi0() , track_name + " TRACK #phi_{0} (rad)" );
    dg::fillh( "eta_vs_phi" , 60 , -3 , 3 , 48 , -M_PI , M_PI , track->eta() , track->phi0() , track_name + " #eta" , track_name + " TRACK #phi_{0} (rad)" );
    if( track->barcode().is_valid() ) {
      dg::fillh( "barcode_code_single" , 10 , 10000 , 10010 , track->barcode().barcode() , track_name + " MATCHING BARCODE" );
      dg::fillh( "barcode_code" , 10 , 0 , 1000 , track->barcode().barcode() , track_name + " MATCHING BARCODE" );
      dg::filleff( "barcode_code_zero" , track->barcode().barcode() == 0 );
      dg::fillh( "barcode_event" , 41 , -20  , 20 , track->barcode().event_index() , track_name + " MATCHING PILEUP EVENT INDEX" );
      dg::fillh( "barcode_matching_fraction" , 200 , -0.01 , 1.0001 , track->barcode_matching_fraction() , track_name + " BARCODE MATCHING FRACTION" );
      dg::fillh( "barcode_matching_fraction_vs_curv" , 40 , -0.5 , 0.5 , 200 , -0.01 , 1.0001 , track->curvature()*1000. , 
                 track->barcode_matching_fraction() , track_name + " CURVATURE (1/GeV)" , track_name + " BARCODE MATCHING FRACTION" );
      if( track->barcode().event_index() == 0 && track->barcode().barcode() < 100000 ) {
        dg::fillh( "barcode_matching_fraction_hs" , 200 , -0.01 , 1.0001 , track->barcode_matching_fraction() , track_name + " BARCODE MATCHING FRACTION (Hardscatter Tracks)" );
      }
    }
    dg::fillh( "nhits" , 15 , 0 , 15 , track->nhits() , track_name + " TRACK N HITS" );
    dg::fillh( "nhits_blayer" , 15 , 0 , 15 , track->b_layer_nhits() , track_name + " TRACK N B LAYER HITS" );
    dg::fillh( "nhits_pixel" , 15 , 0 , 15 , track->pixel_nhits() , track_name + " TRACK N PIXEL HITS" );
    dg::fillh( "nhits_sct" , 15 , 0 , 15 , track->sct_nhits() , track_name + " TRACK N SCT HITS" );
    dg::fillh( "nholes" , 15 , 0 , 15 , track->nholes() , track_name + " TRACK N HOLES" );
    dg::fillh( "nholes_pixel" , 15 , 0 , 15 , track->pixel_nholes() , track_name + " TRACK N PIXEL HOLES" );
    dg::fillh( "nholes_sct" , 15 , 0 , 15 , track->sct_nholes() , track_name + " TRACK N SCT HOLES" );
    dg::fillh( "nshared" , 15 , 0 , 15 , track->nshared() , track_name + " TRACK N SHARED" );
    dg::fillh( "nshared_pixel" , 15 , 0 , 15 , track->pixel_nshared() , track_name + " TRACK N PIXEL SHARED" );
    dg::fillh( "nshared_sct" , 15 , 0 , 15 , track->sct_nshared() , track_name + " TRACK N SCT SHARED" );
    dg::fillh( "chi2" , 50 , 0 , 25 , track->chi2() , track_name + " TRACK #chi^{2}" );
    dg::fillh( "chi2ndof" , 100 , 0 , 25 , track->chi2ndof() , track_name + " TRACK #chi^{2} / n_{dof}" );
    dg::fillh( "chi2prob" , 100 , 0 , 1. , track->chi2prob() , track_name + " TRACK probability( #chi^{2} , n_{dof} )" );
    dg::fillh( "logchi2prob" , 100 , -10 , 0.0001 , detector::safe_log10(track->chi2prob()) , track_name + " log TRACK probability( #chi^{2} , n_{dof} )" );
    if( track->is_ftk() ) {
      study_ftk_track( track );
    }
    if( track->is_truth() ) {
      study_truth_track( track );
    }
    dg::fillh( "d0_vs_phi" , 32 , -M_PI , M_PI , 400 , -1 , 1 , track->phi0() , track->d0()/10. , 
               track_name + " TRACK #phi (rad)" , track_name + " TRACK d_{0} (cm)" );
  }
  static void study_wrt_event( type const& track , boost::shared_ptr<const AnaEvent>& event ) {
    using namespace DataGraphics;
    const std::string track_name = track->algorithm().name();
    if( event && event->highest_sum_pt_vertex() ) { 
      const AnaTrack::ftype xv = event->highest_sum_pt_vertex()->x();
      const AnaTrack::ftype yv = event->highest_sum_pt_vertex()->y();
      const AnaTrack::ftype zv = event->highest_sum_pt_vertex()->z();
      dg::fillh( "d0raw" , 400 , -1 , 1 , track->d0_raw()/10. , track_name + " TRACK d_{0} in ATLAS coords (cm)" ); 
      dg::fillh( "z0raw" , 200 , -30 , 30 , track->z0_raw()/10. , track_name + " TRACK z_{0} in ATLAS coords (cm)" );
      //dg::fillh( "d0raw" , 400 , -1 , 1 , track->d0_raw(xv,yv)/10. , track_name + " TRACK d_{0} in ATLAS coords (cm)" ); 
      //dg::fillh( "z0raw" , 200 , -30 , 30 , track->z0_raw(zv)/10. , track_name + " TRACK z_{0} in ATLAS coords (cm)" );
      dg::fillh( "d0corr" , 400 , -1 , 1 , track->d0_corr(xv,yv,zv)/10. , track_name + " TRACK d_{0} (cm)" ); 
      dg::fillh( "d0corr_zoom" , 400 , -50 , 50 , track->d0_corr(xv,yv,zv)*1000. , track_name + " TRACK d_{0} ({#mu}m)" ); 
      if( false ) { 
        // compute higher order corrections. FIXME: is curv_in_mm correct?
        AnaTrack::ftype curv_in_mm = detector::safe_divide( static_cast<AnaTrack::ftype>(0.5) , 
                                                            std::abs( track->pt() / 
                                                                      (detector::curvature_constant_in_gev_mm_tesla<AnaTrack::ftype>()*
                                                                       detector::solenoidal_field_in_tesla<AnaTrack::ftype>()) ) );
        AnaTrack::ftype corr0 = -1.*((xv*std::cos(track->phi0())) - (yv*std::sin(track->phi0()))); // zero-th order in curvature.
        AnaTrack::ftype corr1 = curv_in_mm*detector::pow<2>( (xv*std::cos(track->phi0())) + (yv*std::sin(track->phi0())) ); // 1st order in curvature
        AnaTrack::ftype corr2 = -detector::pow<2>(curv_in_mm)*( ( track->d0() - (yv*std::cos(track->phi0())) + (xv*std::sin(track->phi0())) ) * 
                                                                detector::pow<2>( (xv*std::cos(track->phi0())) + (yv*std::sin(track->phi0())) ) ); //2nd order in curvature
        dg::fillh( "d0corr_0th" , 100 , -2 , 2 , corr0 , track_name + " TRACK d_{0} 0th ORDER CORRECTION (mm)" ); 
        dg::fillh( "d0corr_1st" , 100 , -50 , 50 , corr1*1000. , track_name + " TRACK d_{0} 1st ORDER CORRECTION ({#mu}m)" ); 
        dg::fillh( "d0corr_2nd" , 100 , -50 , 50 , corr2*1000000. , track_name + " TRACK d_{0} 1st ORDER CORRECTION (nm)" ); 
        dg::fillh( "d0corr_zoom_0th" , 400 , -50 , 50 , (track->d0()+corr0)*1000. , track_name + " TRACK d_{0} ({#mu}m)" ); 
        dg::fillh( "d0corr_zoom_1st" , 400 , -1 , 1 , (track->d0()+corr0+corr1)*1000. , track_name + " TRACK d_{0} ({#mu}m)" ); 
        dg::fillh( "d0corr_zoom_2nd" , 400 , -0.01 , 0.01 , (track->d0()+corr0+corr1+corr2)*1000. , track_name + " TRACK d_{0} ({#mu}m)" ); 
      }
      dg::fillh( "d0corr_vs_phi" , 32 , -M_PI , M_PI , 100 , -0.25 , 0.25 , track->phi0() , 
                 track->d0_corr(event->highest_sum_pt_vertex()->x(),event->highest_sum_pt_vertex()->y(),event->highest_sum_pt_vertex()->z())/10. , 
                 track_name + " TRACK #phi (rad)" , track_name + " TRACK CORRECTED d_{0} (cm)" );
    }
  }
  static void study_covariance( type const& track ) {
    // track covariance matrix; error correlations
    using namespace DataGraphics;
    dg::down( "covariance" , "track covariance studies" ); BOOST_SCOPE_EXIT(void) { dg::up(); } BOOST_SCOPE_EXIT_END;
    // covariance estimates quoted by track (may be calculated in the analysis)
    dg::fillh( "cov_cc" , 100 , 0 , 1E-10 , track->covariance_cc() ,
               "Covariance (#Kappa #Kappa) (1/Gev^{2})" );
    dg::fillh( "cov_cd" , 100 , -1E-6 , 1E-6 , track->covariance_cd() ,
               "Covariance (#Kappa d_{0}) (cm/Gev)" );
    dg::fillh( "cov_cp" , 100 , -1E-6 , 1E-6 , track->covariance_cp() ,
               "Covariance (#Kappa #phi_{0}) (rad/Gev)" );
    dg::fillh( "cov_cz" , 100 , -1E-6 , 1E-6 , track->covariance_cz() ,
               "Covariance (#Kappa z_{0}) (cm/Gev)" );
    dg::fillh( "cov_ct" , 100 , -1E-4 , 1E-4 , track->covariance_ct() ,
               "Covariance (#Kappa cot(#theta)) (1/Gev)" );
    dg::fillh( "cov_dd" , 100 , -1E-1 , 1E-1 , track->covariance_dd() ,
               "Covariance (d_{0} d_{0}) (cm^{2})" );
    dg::fillh( "cov_dp" , 100 , -1E-1 , 1E-1 , track->covariance_dp() ,
               "Covariance (d_{0} #phi_{0}) (cm rad)" );
    dg::fillh( "cov_dz" , 100 , -1E-1 , 1E-1 , track->covariance_dz() ,
               "Covariance (d_{0} z_{0}) (cm^{2})" );
    dg::fillh( "cov_dt" , 100 , -1 , 1 , track->covariance_dt() ,
               "Covariance (d_{0} cot(#theta)) (cm)" );
    dg::fillh( "cov_pp" , 100 , -1E-1 , 1E-1 , track->covariance_pp() ,
               "Covariance (#phi_{0} #phi_{0}) (rad^{2})" );
    dg::fillh( "cov_pz" , 100 , -1E-1 , 1E-1 , track->covariance_pz() ,
               "Covariance (#phi_{0} z_{0}) (cm rad)" );
    dg::fillh( "cov_pt" , 100 , -1 , 1 , track->covariance_pt() ,
               "Covariance (#phi_{0} cot(#theta)) (rad)" );
    dg::fillh( "cov_zz" , 100 , -1E-1 , 1E-1 , track->covariance_zz() ,
               "Covariance (z_{0} z_{0}) (cm^{2})" );
    dg::fillh( "cov_zt" , 100 , -1 , 1 , track->covariance_zt() ,
               "Covariance (z_{0} cot(#theta)) (cm)" );
    dg::fillh( "cov_tt" , 100 , -1 , 1 , track->covariance_tt() ,
               "Covariance (cot(#theta) cot(#theta))" );
  }
  static void study_clusters( type const& track ) { 
    const std::vector< boost::shared_ptr<const AnaSiCluster> >& clusters( track->si_clusters() );
    StudySiCluster stclu( "clusters" , "silicon clusters" );
    stclu.for_each( clusters.begin() , clusters.end() );
  }
  static void study_ftk_track( type const& track ) {
    using namespace DataGraphics;
    const std::string track_name = track->algorithm().name();
    for( unsigned int iplane=0, fplane=14; iplane!=fplane; ++iplane ) {
      dg::filleff( "ftk_efficiency_vs_layer" , 14 , 0 , 14 , iplane , detector::ftkHasPlane(track->ftk_bitmask(),iplane) , "FTK LAYER" );
      dg::fillh( "nmisses" , 15 , 0 , 15 , track->nmisses() , track_name + " TRACK N MISSES" );
    }
    dg::filleff( "ftk_is_majority" , track->ftk_is_majority() );
    dg::fillh( "ftk_hole_number" , 13 , -1 , 12 , track->ftk_hole_number() );
    dg::fillh( "ftk_origchi2_ndof" , 50 , 0 , 25 , track->origchi2()/11. , track_name + " INITIAL TRACK #chi^{2}" );
    if( track->ftk_is_majority() ) {
      const AnaTrack::ftype delta_chi2 = track->origchi2() - track->chi2();
      const AnaTrack::ftype delta_chi2ndof = (track->origchi2()/11.) - (track->chi2()/10.);
      const AnaTrack::ftype log_delta_chi2 = detector::safe_log10( track->origchi2() - track->chi2() );
      const AnaTrack::ftype log_delta_chi2ndof = detector::safe_log10( (track->origchi2()/11.) - (track->chi2()/10.) );
      dg::fillh( "ftk_maj_delta_chi2" , 50 , 0 , 25 , delta_chi2 , track_name + " #Delta #chi^{2}" );
      dg::fillh( "ftk_maj_delta_chi2ndof" , 50 , 0 , 25 , delta_chi2ndof , track_name + " #Delta #chi^{2}/ndof" );
      dg::fillh( "ftk_maj_log_delta_chi2" , 100 , -25 , 25 , log_delta_chi2 , track_name + " log #Delta #chi^{2}" );
      dg::fillh( "ftk_maj_log_delta_chi2ndof" , 100 , -25 , 25 , log_delta_chi2ndof , track_name + " log #Delta #chi^{2}/ndof" );
    }
  }
  static void study_truth_track( type const& track ) {
    using namespace DataGraphics;
    const int particle_type = detector::particle_type( track->pdg_code() ) * detector::sign_of( track->pdg_code() );
    dg::fillh( "truth_type" , 13 , -6 , 7 , particle_type , "TRUTH PARTICLE TYPE CODE" );
    dg::fillh( "end_vertex" , 100 , 0 , 1000 , track->end_vertex_radius() , "END VERTEX RADIUS (mm)" );
  }
protected:
  virtual void _study( type const& track ) { // the second const is important---matches the base class.
    study( track );
    study_wrt_event( track , _event );
    if( _do_covariance_study ) { study_covariance( track ); }
    if( _do_cluster_study ) { study_clusters( track ); }
  } // end study of each track
protected:
  boost::shared_ptr<const AnaEvent> _event;
  bool _do_covariance_study;
  bool _do_cluster_study;
public:
  StudyTrack( const std::string& name , const std::string& description ) 
    : _event()
    , _do_covariance_study(false)
    , _do_cluster_study(false)
    , DataGraphics::DgStudy<type>(name,description)
  {}
  virtual ~StudyTrack() {}
  void for_event( const boost::shared_ptr<const AnaEvent>& event ) { _event = event; }
  void do_covariance_study( const bool& yes ) { _do_covariance_study = yes; }
  void do_cluster_study( const bool& yes ) { _do_cluster_study = yes; }
};

#endif // WRAP_STUDYTRACK_HH

