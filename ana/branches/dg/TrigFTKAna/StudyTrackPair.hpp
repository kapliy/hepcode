#ifndef WRAP_STUDYTRACKCOMPARISON_HH
#define WRAP_STUDYTRACKCOMPARISON_HH

// "visitor"/"functor" to make standard jet tagging plots.

#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/scope_exit.hpp>
#include "DataGraphics/DgStudy.hh"
#include "DataGraphics/DgCollection.hh"
#include "TrigFTKAna/AnaTrack.hpp"
#include "TrigFTKAna/Detector.hpp"

class
TrackIdentification
{
private:
  boost::shared_ptr<const AnaTrack> _reference_track;
  boost::shared_ptr<const AnaTrack> _comparison_track;
public:
  TrackIdentification(  boost::shared_ptr<const AnaTrack>  reference_track ,
                        boost::shared_ptr<const AnaTrack>  comparison_track ) 
    : _reference_track( reference_track )
    , _comparison_track( comparison_track )
  {}
  const boost::shared_ptr<const AnaTrack>& reference_track() const { return _reference_track; }
  const boost::shared_ptr<const AnaTrack>& comparison_track() const { return _comparison_track; }
};

class
StudyTrackComparison : public DataGraphics::DgStudy<TrackIdentification>
{
public:
  typedef TrackIdentification type; // must be same type as the template argument for DgStudy.
protected:
  virtual void _study( type const& pairing ) {
    using namespace std;
    using namespace boost;
    using namespace DataGraphics;
    using namespace detector;
    const boost::shared_ptr<const AnaTrack>& reference_track( pairing.reference_track() );
    const boost::shared_ptr<const AnaTrack>& comparison_track( pairing.comparison_track() );
    // compute actual parameter residuals
    const AnaTrack::ftype res_cu = ( comparison_track->curvature() - reference_track->curvature() );
    const AnaTrack::ftype res_d0 = ( (comparison_track->d0() - reference_track->d0())/10. );
    const AnaTrack::ftype res_p0 = ( (comparison_track->phi0() - reference_track->phi0()) );
    const AnaTrack::ftype res_z0 = ( (comparison_track->z0() - reference_track->z0())/10. );
    const AnaTrack::ftype res_la = ( (comparison_track->lambda() - reference_track->lambda()) );
    const AnaTrack::ftype res_cc = detector::pow<2>( res_cu );
    const AnaTrack::ftype res_dd = detector::pow<2>( res_d0 );
    const AnaTrack::ftype res_pp = detector::pow<2>( res_p0 );
    const AnaTrack::ftype res_zz = detector::pow<2>( res_z0 );
    const AnaTrack::ftype res_tt = detector::pow<2>( res_la );
    const AnaTrack::ftype res_cd = res_cu*res_d0;
    const AnaTrack::ftype res_cp = res_cu*res_p0;
    const AnaTrack::ftype res_cz = res_cu*res_z0;
    const AnaTrack::ftype res_ct = res_cu*res_la;
    const AnaTrack::ftype res_dp = res_d0*res_p0;
    const AnaTrack::ftype res_dz = res_d0*res_z0;
    const AnaTrack::ftype res_dt = res_d0*res_la;
    const AnaTrack::ftype res_pz = res_p0*res_z0;
    const AnaTrack::ftype res_pt = res_p0*res_la;
    const AnaTrack::ftype res_zt = res_z0*res_la;
    {
      dg::down( "estimates" , "covariance estimates quoted by track (may be calculated in the analysis)" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      dg::fillh( "cov_cc" , 100 , 0. , hm_range_cov_cc<AnaTrack::ftype>() , comparison_track->covariance_cc() , "Covariance (#Kappa #Kappa) (1/GeV^{2})" );
      dg::fillh( "cov_cd" , 100 , -hm_range_cov_cd<AnaTrack::ftype>() , hm_range_cov_cd<AnaTrack::ftype>() , comparison_track->covariance_cd() , "Covariance (#Kappa d_{0}) (cm/GeV)" );
      dg::fillh( "cov_cp" , 100 , -hm_range_cov_cp<AnaTrack::ftype>() , hm_range_cov_cp<AnaTrack::ftype>() , comparison_track->covariance_cp() , "Covariance (#Kappa #phi_{0}) (rad/GeV)" );
      dg::fillh( "cov_cz" , 100 , -hm_range_cov_cz<AnaTrack::ftype>() , hm_range_cov_cz<AnaTrack::ftype>() , comparison_track->covariance_cz() , "Covariance (#Kappa z_{0}) (cm/GeV)" );
      dg::fillh( "cov_ct" , 100 , -hm_range_cov_ct<AnaTrack::ftype>() , hm_range_cov_ct<AnaTrack::ftype>() , comparison_track->covariance_ct() , "Covariance (#Kappa cot(#theta)) (1/GeV)" );
      dg::fillh( "cov_dd" , 100 , 0. , hm_range_cov_dd<AnaTrack::ftype>() , comparison_track->covariance_dd() , "Covariance (d_{0} d_{0}) (cm^{2})" );
      dg::fillh( "cov_dp" , 100 , -hm_range_cov_dp<AnaTrack::ftype>() , hm_range_cov_dp<AnaTrack::ftype>() , comparison_track->covariance_dp() , "Covariance (d_{0} #phi_{0}) (cm rad)" );
      dg::fillh( "cov_dz" , 100 , -hm_range_cov_dz<AnaTrack::ftype>() , hm_range_cov_dz<AnaTrack::ftype>() , comparison_track->covariance_dz() , "Covariance (d_{0} z_{0}) (cm^{2})" );
      dg::fillh( "cov_dt" , 100 , -hm_range_cov_dt<AnaTrack::ftype>() , hm_range_cov_dt<AnaTrack::ftype>() , comparison_track->covariance_dt() , "Covariance (d_{0} cot(#theta)) (cm)" );
      dg::fillh( "cov_pp" , 100 , 0. , hm_range_cov_pp<AnaTrack::ftype>() , comparison_track->covariance_pp() , "Covariance (#phi_{0} #phi_{0}) (rad^{2})" );
      dg::fillh( "cov_pz" , 100 , -hm_range_cov_pz<AnaTrack::ftype>() , hm_range_cov_pz<AnaTrack::ftype>() , comparison_track->covariance_pz() , "Covariance (#phi_{0} z_{0}) (cm rad)" );
      dg::fillh( "cov_pt" , 100 , -hm_range_cov_pt<AnaTrack::ftype>() , hm_range_cov_pt<AnaTrack::ftype>() , comparison_track->covariance_pt() , "Covariance (#phi_{0} cot(#theta)) (rad)" );
      dg::fillh( "cov_zz" , 100 , 0. , hm_range_cov_zz<AnaTrack::ftype>() , comparison_track->covariance_zz() , "Covariance (z_{0} z_{0}) (cm^{2})" );
      dg::fillh( "cov_zt" , 100 , -hm_range_cov_zt<AnaTrack::ftype>() , hm_range_cov_zt<AnaTrack::ftype>() , comparison_track->covariance_zt() , "Covariance (z_{0} cot(#theta)) (cm)" );
      dg::fillh( "cov_tt" , 100 , 0. , hm_range_cov_tt<AnaTrack::ftype>() , comparison_track->covariance_tt() , "Covariance (cot(#theta) cot(#theta))" );
    }
    {
      dg::down( "residuals" , "parameter residuals for the track relative to the reference" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      dg::fillh( "res_cd" , 100 , -hm_range_cov_cd<AnaTrack::ftype>() , hm_range_cov_cd<AnaTrack::ftype>() , res_cd , "residual (#Kappa d_{0}) (cm/GeV)" );
      dg::fillh( "res_cp" , 100 , -hm_range_cov_cp<AnaTrack::ftype>() , hm_range_cov_cp<AnaTrack::ftype>() , res_cp , "residual (#Kappa #phi_{0}) (rad/GeV)" );
      dg::fillh( "res_cz" , 100 , -hm_range_cov_cz<AnaTrack::ftype>() , hm_range_cov_cz<AnaTrack::ftype>() , res_cz , "residual (#Kappa z_{0}) (cm/GeV)" );
      dg::fillh( "res_ct" , 100 , -hm_range_cov_ct<AnaTrack::ftype>() , hm_range_cov_ct<AnaTrack::ftype>() , res_ct , "residual (#Kappa cot(#theta)) (1/GeV)" );
      dg::fillh( "res_dp" , 100 , -hm_range_cov_dp<AnaTrack::ftype>() , hm_range_cov_dp<AnaTrack::ftype>() , res_dp , "residual (d_{0} #phi_{0}) (cm rad)" );
      dg::fillh( "res_dz" , 100 , -hm_range_cov_dz<AnaTrack::ftype>() , hm_range_cov_dz<AnaTrack::ftype>() , res_dz , "residual (d_{0} z_{0}) (cm^{2})" );
      dg::fillh( "res_dt" , 100 , -hm_range_cov_dt<AnaTrack::ftype>() , hm_range_cov_dt<AnaTrack::ftype>() , res_dt , "residual (d_{0} cot(#theta)) (cm)" );
      dg::fillh( "res_pz" , 100 , -hm_range_cov_pz<AnaTrack::ftype>() , hm_range_cov_pz<AnaTrack::ftype>() , res_pz , "residual (#phi_{0} z_{0}) (cm rad)" );
      dg::fillh( "res_pt" , 100 , -hm_range_cov_pt<AnaTrack::ftype>() , hm_range_cov_pt<AnaTrack::ftype>() , res_pt , "residual (#phi_{0} cot(#theta)) (rad)" );
      dg::fillh( "res_zt" , 100 , -hm_range_cov_zt<AnaTrack::ftype>() , hm_range_cov_zt<AnaTrack::ftype>() , res_zt , "residual (z_{0} cot(#theta)) (cm)" );
    }
    // dependence of covariance on residuals
    {
      dg::down( "covariance_vs_residuals" , "dependence of covariance estimates on the residuals" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      dg::fillh( "cov_cd_vs_cu" , 
                 100 , -std::sqrt(hm_range_cov_cc<AnaTrack::ftype>()) , std::sqrt(hm_range_cov_cc<AnaTrack::ftype>()) , 
                 100 , -hm_range_cov_cd<AnaTrack::ftype>() , hm_range_cov_cd<AnaTrack::ftype>() , 
                 res_cu , comparison_track->covariance_cd() , "res cu" , "cov cd" );
      dg::fillh( "cov_cd_vs_d0" , 
                 100 , -std::sqrt(hm_range_cov_dd<AnaTrack::ftype>()) , std::sqrt(hm_range_cov_dd<AnaTrack::ftype>()) , 
                 100 , -hm_range_cov_cd<AnaTrack::ftype>() , hm_range_cov_cd<AnaTrack::ftype>() , 
                 res_d0 , comparison_track->covariance_cd() , "res d0" , "cov cd" );
      dg::fillh( "cov_cd_vs_p0" , 
                 100 , -std::sqrt(hm_range_cov_pp<AnaTrack::ftype>()) , std::sqrt(hm_range_cov_pp<AnaTrack::ftype>()) , 
                 100 , -hm_range_cov_cd<AnaTrack::ftype>() , hm_range_cov_cd<AnaTrack::ftype>() , 
                 res_p0 , comparison_track->covariance_cd() , "res p0" , "cov cd" );
      dg::fillh( "cov_cd_vs_z0" , 
                 100 , -std::sqrt(hm_range_cov_pp<AnaTrack::ftype>()) , std::sqrt(hm_range_cov_pp<AnaTrack::ftype>()) , 
                 100 , -hm_range_cov_cd<AnaTrack::ftype>() , hm_range_cov_cd<AnaTrack::ftype>() , 
                 res_z0 , comparison_track->covariance_cd() , "res z0" , "cov cd" );
      dg::fillh( "cov_cd_vs_la" , 
                 100 , -std::sqrt(hm_range_cov_pp<AnaTrack::ftype>()) , std::sqrt(hm_range_cov_pp<AnaTrack::ftype>()) , 
                 100 , -hm_range_cov_cd<AnaTrack::ftype>() , hm_range_cov_cd<AnaTrack::ftype>() , 
                 res_la , comparison_track->covariance_cd() , "res la" , "cov cd" );
      dg::fillh( "cov_cp_vs_cu" , 
                 100 , -std::sqrt(hm_range_cov_cc<AnaTrack::ftype>()) , std::sqrt(hm_range_cov_cc<AnaTrack::ftype>()) , 
                 100 , -hm_range_cov_cp<AnaTrack::ftype>() , hm_range_cov_cp<AnaTrack::ftype>() , 
                 res_cu , comparison_track->covariance_cp() , "res cu" , "cov cp" );
      dg::fillh( "cov_cp_vs_d0" , 
                 100 , -std::sqrt(hm_range_cov_dd<AnaTrack::ftype>()) , std::sqrt(hm_range_cov_dd<AnaTrack::ftype>()) , 
                 100 , -hm_range_cov_cp<AnaTrack::ftype>() , hm_range_cov_cp<AnaTrack::ftype>() , 
                 res_d0 , comparison_track->covariance_cp() , "res d0" , "cov cp" );
      dg::fillh( "cov_cp_vs_p0" , 
                 100 , -std::sqrt(hm_range_cov_pp<AnaTrack::ftype>()) , std::sqrt(hm_range_cov_pp<AnaTrack::ftype>()) , 
                 100 , -hm_range_cov_cp<AnaTrack::ftype>() , hm_range_cov_cp<AnaTrack::ftype>() , 
                 res_p0 , comparison_track->covariance_cp() , "res p0" , "cov cp" );
      dg::fillh( "cov_cp_vs_z0" , 
                 100 , -std::sqrt(hm_range_cov_pp<AnaTrack::ftype>()) , std::sqrt(hm_range_cov_pp<AnaTrack::ftype>()) , 
                 100 , -hm_range_cov_cp<AnaTrack::ftype>() , hm_range_cov_cp<AnaTrack::ftype>() , 
                 res_z0 , comparison_track->covariance_cp() , "res z0" , "cov cp" );
      dg::fillh( "cov_cp_vs_la" , 
                 100 , -std::sqrt(hm_range_cov_pp<AnaTrack::ftype>()) , std::sqrt(hm_range_cov_pp<AnaTrack::ftype>()) , 
                 100 , -hm_range_cov_cp<AnaTrack::ftype>() , hm_range_cov_cp<AnaTrack::ftype>() , 
                 res_la , comparison_track->covariance_cp() , "res la" , "cov cp" );
      dg::fillh( "cov_dp_vs_cu" , 
                 100 , -std::sqrt(hm_range_cov_cc<AnaTrack::ftype>()) , std::sqrt(hm_range_cov_cc<AnaTrack::ftype>()) , 
                 100 , -hm_range_cov_dp<AnaTrack::ftype>() , hm_range_cov_dp<AnaTrack::ftype>() , 
                 res_cu , comparison_track->covariance_dp() , "res cu" , "cov dp" );
      dg::fillh( "cov_dp_vs_d0" , 
                 100 , -std::sqrt(hm_range_cov_dd<AnaTrack::ftype>()) , std::sqrt(hm_range_cov_dd<AnaTrack::ftype>()) , 
                 100 , -hm_range_cov_dp<AnaTrack::ftype>() , hm_range_cov_dp<AnaTrack::ftype>() , 
                 res_d0 , comparison_track->covariance_dp() , "res d0" , "cov dp" );
      dg::fillh( "cov_dp_vs_p0" , 
                 100 , -std::sqrt(hm_range_cov_pp<AnaTrack::ftype>()) , std::sqrt(hm_range_cov_pp<AnaTrack::ftype>()) , 
                 100 , -hm_range_cov_dp<AnaTrack::ftype>() , hm_range_cov_dp<AnaTrack::ftype>() , 
                 res_p0 , comparison_track->covariance_dp() , "res p0" , "cov dp" );
      dg::fillh( "cov_dp_vs_z0" , 
                 100 , -std::sqrt(hm_range_cov_pp<AnaTrack::ftype>()) , std::sqrt(hm_range_cov_pp<AnaTrack::ftype>()) , 
                 100 , -hm_range_cov_dp<AnaTrack::ftype>() , hm_range_cov_dp<AnaTrack::ftype>() , 
                 res_z0 , comparison_track->covariance_dp() , "res z0" , "cov dp" );
      dg::fillh( "cov_dp_vs_la" , 
                 100 , -std::sqrt(hm_range_cov_pp<AnaTrack::ftype>()) , std::sqrt(hm_range_cov_pp<AnaTrack::ftype>()) , 
                 100 , -hm_range_cov_dp<AnaTrack::ftype>() , hm_range_cov_dp<AnaTrack::ftype>() , 
                 res_la , comparison_track->covariance_dp() , "res la" , "cov dp" );
      dg::fillh( "cov_pz_vs_cu" , 
                 100 , -std::sqrt(hm_range_cov_cc<AnaTrack::ftype>()) , std::sqrt(hm_range_cov_cc<AnaTrack::ftype>()) , 
                 100 , -hm_range_cov_pz<AnaTrack::ftype>() , hm_range_cov_pz<AnaTrack::ftype>() , 
                 res_cu , comparison_track->covariance_pz() , "res cu" , "cov pz" );
      dg::fillh( "cov_pz_vs_d0" , 
                 100 , -std::sqrt(hm_range_cov_dd<AnaTrack::ftype>()) , std::sqrt(hm_range_cov_dd<AnaTrack::ftype>()) , 
                 100 , -hm_range_cov_pz<AnaTrack::ftype>() , hm_range_cov_pz<AnaTrack::ftype>() , 
                 res_d0 , comparison_track->covariance_pz() , "res d0" , "cov pz" );
      dg::fillh( "cov_pz_vs_p0" , 
                 100 , -std::sqrt(hm_range_cov_pp<AnaTrack::ftype>()) , std::sqrt(hm_range_cov_pp<AnaTrack::ftype>()) , 
                 100 , -hm_range_cov_pz<AnaTrack::ftype>() , hm_range_cov_pz<AnaTrack::ftype>() , 
                 res_p0 , comparison_track->covariance_pz() , "res p0" , "cov pz" );
      dg::fillh( "cov_pz_vs_z0" , 
                 100 , -std::sqrt(hm_range_cov_pp<AnaTrack::ftype>()) , std::sqrt(hm_range_cov_pp<AnaTrack::ftype>()) , 
                 100 , -hm_range_cov_pz<AnaTrack::ftype>() , hm_range_cov_pz<AnaTrack::ftype>() , 
                 res_z0 , comparison_track->covariance_pz() , "res z0" , "cov pz" );
      dg::fillh( "cov_pz_vs_la" , 
                 100 , -std::sqrt(hm_range_cov_pp<AnaTrack::ftype>()) , std::sqrt(hm_range_cov_pp<AnaTrack::ftype>()) , 
                 100 , -hm_range_cov_pz<AnaTrack::ftype>() , hm_range_cov_pz<AnaTrack::ftype>() , 
                 res_la , comparison_track->covariance_pz() , "res la" , "cov pz" );
      dg::fillh( "cov_zt_vs_cu" , 
                 100 , -std::sqrt(hm_range_cov_cc<AnaTrack::ftype>()) , std::sqrt(hm_range_cov_cc<AnaTrack::ftype>()) , 
                 100 , -hm_range_cov_zt<AnaTrack::ftype>() , hm_range_cov_zt<AnaTrack::ftype>() , 
                 res_cu , comparison_track->covariance_zt() , "res cu" , "cov zt" );
      dg::fillh( "cov_zt_vs_d0" , 
                 100 , -std::sqrt(hm_range_cov_dd<AnaTrack::ftype>()) , std::sqrt(hm_range_cov_dd<AnaTrack::ftype>()) , 
                 100 , -hm_range_cov_zt<AnaTrack::ftype>() , hm_range_cov_zt<AnaTrack::ftype>() , 
                 res_d0 , comparison_track->covariance_zt() , "res d0" , "cov zt" );
      dg::fillh( "cov_zt_vs_p0" , 
                 100 , -std::sqrt(hm_range_cov_pp<AnaTrack::ftype>()) , std::sqrt(hm_range_cov_pp<AnaTrack::ftype>()) , 
                 100 , -hm_range_cov_zt<AnaTrack::ftype>() , hm_range_cov_zt<AnaTrack::ftype>() , 
                 res_p0 , comparison_track->covariance_zt() , "res p0" , "cov zt" );
      dg::fillh( "cov_zt_vs_z0" , 
                 100 , -std::sqrt(hm_range_cov_pp<AnaTrack::ftype>()) , std::sqrt(hm_range_cov_pp<AnaTrack::ftype>()) , 
                 100 , -hm_range_cov_zt<AnaTrack::ftype>() , hm_range_cov_zt<AnaTrack::ftype>() , 
                 res_z0 , comparison_track->covariance_zt() , "res z0" , "cov zt" );
      dg::fillh( "cov_zt_vs_la" , 
                 100 , -std::sqrt(hm_range_cov_pp<AnaTrack::ftype>()) , std::sqrt(hm_range_cov_pp<AnaTrack::ftype>()) , 
                 100 , -hm_range_cov_zt<AnaTrack::ftype>() , hm_range_cov_zt<AnaTrack::ftype>() , 
                 res_la , comparison_track->covariance_zt() , "res la" , "cov zt" );
    }
    {
      dg::down( "residual_correlations" , "correlations between residuals" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      dg::fillh( "res_cu_vs_res_d0" ,
                 100 , -std::sqrt(hm_range_cov_cc<AnaTrack::ftype>()) , std::sqrt(hm_range_cov_cc<AnaTrack::ftype>()) , 
                 100 , -std::sqrt(hm_range_cov_dd<AnaTrack::ftype>()) , std::sqrt(hm_range_cov_dd<AnaTrack::ftype>()) , 
                 res_cu , res_d0 , "res cu" , "res d0" );
      dg::fillh( "res_cu_vs_res_p0" ,
                 100 , -std::sqrt(hm_range_cov_cc<AnaTrack::ftype>()) , std::sqrt(hm_range_cov_cc<AnaTrack::ftype>()) , 
                 100 , -std::sqrt(hm_range_cov_pp<AnaTrack::ftype>()) , std::sqrt(hm_range_cov_pp<AnaTrack::ftype>()) , 
                 res_cu , res_p0 , "res cu" , "res p0" );
      dg::fillh( "res_cu_vs_res_z0" ,
                 100 , -std::sqrt(hm_range_cov_cc<AnaTrack::ftype>()) , std::sqrt(hm_range_cov_cc<AnaTrack::ftype>()) , 
                 100 , -std::sqrt(hm_range_cov_zz<AnaTrack::ftype>()) , std::sqrt(hm_range_cov_zz<AnaTrack::ftype>()) , 
                 res_cu , res_z0 , "res cu" , "res z0" );
      dg::fillh( "res_cu_vs_res_la" ,
                 100 , -std::sqrt(hm_range_cov_cc<AnaTrack::ftype>()) , std::sqrt(hm_range_cov_cc<AnaTrack::ftype>()) , 
                 100 , -std::sqrt(hm_range_cov_tt<AnaTrack::ftype>()) , std::sqrt(hm_range_cov_tt<AnaTrack::ftype>()) , 
                 res_cu , res_la , "res cu" , "res la" );
      dg::fillh( "res_d0_vs_res_p0" ,
                 100 , -std::sqrt(hm_range_cov_dd<AnaTrack::ftype>()) , std::sqrt(hm_range_cov_dd<AnaTrack::ftype>()) , 
                 100 , -std::sqrt(hm_range_cov_pp<AnaTrack::ftype>()) , std::sqrt(hm_range_cov_pp<AnaTrack::ftype>()) , 
                 res_d0 , res_p0 , "res d0" , "res p0" );
      dg::fillh( "res_d0_vs_res_z0" ,
                 100 , -std::sqrt(hm_range_cov_dd<AnaTrack::ftype>()) , std::sqrt(hm_range_cov_dd<AnaTrack::ftype>()) , 
                 100 , -std::sqrt(hm_range_cov_zz<AnaTrack::ftype>()) , std::sqrt(hm_range_cov_zz<AnaTrack::ftype>()) , 
                 res_d0 , res_z0 , "res d0" , "res z0" );
      dg::fillh( "res_d0_vs_res_la" ,
                 100 , -std::sqrt(hm_range_cov_dd<AnaTrack::ftype>()) , std::sqrt(hm_range_cov_dd<AnaTrack::ftype>()) , 
                 100 , -std::sqrt(hm_range_cov_tt<AnaTrack::ftype>()) , std::sqrt(hm_range_cov_tt<AnaTrack::ftype>()) , 
                 res_d0 , res_la , "res d0" , "res la" );
      dg::fillh( "res_p0_vs_res_z0" ,
                 100 , -std::sqrt(hm_range_cov_pp<AnaTrack::ftype>()) , std::sqrt(hm_range_cov_pp<AnaTrack::ftype>()) , 
                 100 , -std::sqrt(hm_range_cov_zz<AnaTrack::ftype>()) , std::sqrt(hm_range_cov_zz<AnaTrack::ftype>()) , 
                 res_p0 , res_z0 , "res p0" , "res z0" );
      dg::fillh( "res_p0_vs_res_la" ,
                 100 , -std::sqrt(hm_range_cov_pp<AnaTrack::ftype>()) , std::sqrt(hm_range_cov_pp<AnaTrack::ftype>()) , 
                 100 , -std::sqrt(hm_range_cov_tt<AnaTrack::ftype>()) , std::sqrt(hm_range_cov_tt<AnaTrack::ftype>()) , 
                 res_p0 , res_la , "res p0" , "res la" );
      dg::fillh( "res_z0_vs_res_la" ,
                 100 , -std::sqrt(hm_range_cov_zz<AnaTrack::ftype>()) , std::sqrt(hm_range_cov_zz<AnaTrack::ftype>()) , 
                 100 , -std::sqrt(hm_range_cov_tt<AnaTrack::ftype>()) , std::sqrt(hm_range_cov_tt<AnaTrack::ftype>()) , 
                 res_z0 , res_la , "res z0" , "res la" );
    }
  }
public:
  StudyTrackComparison( const std::string& name , const std::string& description ) 
    : DataGraphics::DgStudy<type>(name,description)
  {}
  virtual ~StudyTrackComparison() {}
};

#endif // WRAP_STUDYTRACKCOMPARISON_HH

