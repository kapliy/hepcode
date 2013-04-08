#ifndef WRAP_STUDYTRACKDISPLACEMENT_HH
#define WRAP_STUDYTRACKDISPLACEMENT_HH
// 

#include <string>
#include <vector>
#include <utility>
#include <cmath>
#include <boost/shared_ptr.hpp>
#include <boost/assign/list_of.hpp>
#include "DataGraphics/DgStudy.hh"
#include "DataGraphics/DgCollection.hh"
#include "TrigFTKAna/AnaTrack.hpp"
#include "TrigFTKAna/Detector.hpp"
#include "TrigFTKAna/LinearFit.hpp"


template<typename trackCollT>
class
StudyTrackDisplacement : public DataGraphics::DgStudy<const trackCollT>
{
public:
  typedef const trackCollT type; // must be same type as the template argument for DgStudy.
protected:
  AnaTrack::ftype _jet_phi;
  AnaTrack::ftype _b_decay_radius;
  AnaTrack::ftype _b_decay_phi;
protected:
  virtual void _study( type const& coll ) { // the second const is important---matches the base class.
    if( coll.empty() ) { return; }
    using namespace DataGraphics;
    using namespace std;
    // fit d0 vs phi.
    LinearFit<AnaTrack::ftype> fitter;
    // fit in a coordinate system rotated to the mean phi angle.
    AnaTrack::ftype mean_sin = 0.;
    AnaTrack::ftype mean_cos = 0.;
    unsigned int n = 0u;
    for( typename type::const_iterator i=coll.begin(), f=coll.end(); i!=f; ++i ) {
      const boost::shared_ptr<const AnaTrack>& track( *i );
      mean_cos += std::cos( track->phi0() );
      mean_sin += std::sin( track->phi0() );
      ++n;
    }
    mean_cos /= n;
    mean_sin /= n;
    const AnaTrack::ftype mean_phi0 = std::atan2( mean_sin , mean_cos );
    dg::fillh( "mean_cos" , 50 , -1 , 1 , mean_cos );
    dg::fillh( "mean_sin" , 50 , -1 , 1 , mean_sin );
    dg::fillh( "ntracks" , 20 , 0 , 20 , n );
    dg::fillh( "mean_phi0" , 48 , -M_PI , M_PI , mean_phi0 );
    std::vector< std::pair<AnaTrack::ftype,AnaTrack::ftype> > data;
    for( typename type::const_iterator i=coll.begin(), f=coll.end(); i!=f; ++i ) {
      const boost::shared_ptr<const AnaTrack>& track( *i );
      const AnaTrack::ftype delta_phi0 = detector::delta_phi(track->phi0(),mean_phi0);
      data.push_back( std::pair<AnaTrack::ftype,AnaTrack::ftype>( delta_phi0 , track->d0() ) );
      dg::fillh( "delta_phi0" , 50 , -0.5 , 0.5 , delta_phi0 );
    }
    fitter.compute_details( true );
    const bool ok = fitter.fit< std::vector< std::pair<AnaTrack::ftype,AnaTrack::ftype> > >( data.begin() , data.end() );
    dg::filleff( "ok" , ok );
    if( !ok ) { return; }
    const AnaTrack::ftype Rb = fitter.slope();
    const AnaTrack::ftype Rb_err = fitter.slope_err();
    const AnaTrack::ftype phib = detector::canonical_phi( detector::safe_divide(fitter.intercept(),Rb) + mean_phi0 );
    dg::fillh( "fit_phib" , 48 , -M_PI , M_PI , phib );
    dg::fillh( "fit_phib_m_meanphi" , 50 , -0.5 , 0.5 , detector::canonical_phi( detector::safe_divide(fitter.intercept(),Rb) ) );
    dg::fillh( "fit_phib_m_jetphi" , 50 , -0.5 , 0.5 , detector::delta_phi(phib,_jet_phi) );
    dg::fillh( "fit_phib_m_bphi" , 50 , -0.5 , 0.5 , detector::delta_phi(phib,_b_decay_phi) );
    dg::fillh( "fit_rb" , 100 , -2 , 2 , Rb/10. , "R_{b} (cm)" );
    dg::fillh( "fit_rb_pull_zero" , 100 , -0.5 , 0.5 , detector::safe_divide(Rb,Rb_err) , "R_{b} pull (relative to zero)" );
    dg::fillh( "fit_rb_vs_truth" , 50 , 0 , 1 , 100 , -1 , 1 , _b_decay_radius/10. , Rb/10. , "B DECAY RADIUS (cm)" , "R_{b} (cm)" );
    dg::fillh( "fit_rb_pull_truth" , 100 , -5 , 5 , detector::safe_divide(Rb-_b_decay_radius,Rb_err) , "R_{b} pull (relative to truth)" );
    // plot residuals for each track.
    for( typename type::const_iterator i=coll.begin(), f=coll.end(); i!=f; ++i ) {
      const boost::shared_ptr<const AnaTrack>& track( *i );
      const AnaTrack::ftype delta_phi0 = detector::delta_phi(track->phi0(),mean_phi0);
      const AnaTrack::ftype d0_expected = (fitter.slope()*delta_phi0) + fitter.intercept();
      const AnaTrack::ftype d0_expected_err = std::abs(fitter.slope_err()*delta_phi0);
      const AnaTrack::ftype d0_res = track->d0() - d0_expected;
      const AnaTrack::ftype d0_pull = detector::safe_divide( d0_res , detector::quadrature(d0_expected_err,track->d0_err()) );
      dg::fillh( "residual_d0_expected" , 100 , -0.1 , 0.1 , d0_expected/10. , "(cm)" );
      dg::fillh( "residual_d0_expected_err" , 100 , 0 , 1 , d0_expected_err/10. , "(cm)" );
      dg::fillh( "residual_d0_res" , 200 , -1 , 1 , d0_res/10. , "(cm)" );
      dg::fillh( "residual_d0_pull" , 100 , -10 , 10 , d0_pull , "#sigma" );
    }
    if( fitter.compute_details()  ) {
      // plot total fit chi^2
      dg::fillh( "fit_chi2" , 100 , 0 , 1 , fitter.chi2() );
      if( data.size()>=3 ) { 
        // plot unbiased residuals for each track, and collect some statistics
        unsigned int n = 0u;
        AnaTrack::ftype max_unbiased_d0_absresidual = 0;
        AnaTrack::ftype max_chi2_term = 0;
        for( typename type::const_iterator i=coll.begin(), f=coll.end(); i!=f; ++i,++n ) {
          const boost::shared_ptr<const AnaTrack>& track( *i );
          const AnaTrack::ftype delta_phi0 = detector::delta_phi(track->phi0(),mean_phi0);
          const AnaTrack::ftype d0_expected = (fitter.unbiased_slope(n)*delta_phi0) + fitter.unbiased_intercept(n);
          const AnaTrack::ftype d0_res = track->d0() - d0_expected;
          const AnaTrack::ftype chi2n = fitter.chi2(n);
          dg::fillh( "unbiased_residual_d0_expected" , 100 , -0.01 , 0.01 , d0_expected/10. , "(cm)" );
          dg::fillh( "unbiased_residual_d0_res" , 200 , -0.1 , 0.1 , d0_res/10. , "(cm)" );
          dg::fillh( "unbiased_residual_termchi2" , 100 , 0 , 05 , chi2n , "#chi^{2} contribution" );
          max_unbiased_d0_absresidual = std::max( max_unbiased_d0_absresidual , std::abs(d0_res) );
          max_chi2_term = std::max( max_chi2_term , chi2n );
        }
        dg::fillh( "max_unbiased_d0_absresidual_vs_Rb_res" , 50 , -0.5 , 0.5 , 50 , 0 , 0.2 , (Rb-_b_decay_radius)/10. , max_unbiased_d0_absresidual/10. , 
                   "B DECAY RESIDUAL (cm)" , "MAXIMUM UNBIASED |D0 RESIDUAL| (cm)");
        dg::fillh( "max_chi2term_vs_Rb_res" , 50 , -5 , 5 , 50 , 0 , 1 , (Rb-_b_decay_radius)/10. , max_chi2_term , "B DECAY RESIDUAL (cm)" , "MAXIMUM CHI^2 CONTRIBUTION" );
      }
    }
    // study two track pairs.
    AnaTrack::ftype max_absrb_two = 0.;
    unsigned int n_rb_two_high = 0u;
    unsigned int n_two_disp = 0u;
    for( typename type::const_iterator i=coll.begin(), f=coll.end(); i!=f; ++i ) {
      const boost::shared_ptr<const AnaTrack>& trkA( *i );
      for( typename type::const_iterator j=i; j!=f; ++j ) {
        if( i==j ) { continue; }
        const boost::shared_ptr<const AnaTrack>& trkB( *j );
        const AnaTrack::ftype this_rb = detector::safe_divide( trkA->d0() - trkB->d0() , detector::delta_phi(trkA->phi0(),trkB->phi0()) );
        dg::fillh( "two_rb" , 400 , -10 , 10 , this_rb/10. , "TWO TRACK FITTED R_{b} (cm)" );
        max_absrb_two = std::max( std::abs(this_rb) , max_absrb_two );
        if( std::abs(this_rb) > 0.1 ) { ++n_rb_two_high; }
        if( std::abs(trkA->d0())>0.1 || std::abs(trkB->d0())>0.1 ) { ++n_two_disp; }
      }
    }
    dg::fillh( "two_rb_max" , 200 , 0 , 5 , max_absrb_two/10. , "MAX TWO TRACK R_{b} (cm)");
    dg::fillh( "two_n_rb_high" , 50 , 0 , 50 , n_rb_two_high , "N LARGE R_{b} TWO TRACK PAIRS" );
    dg::fillh( "two_n_disp" , 50 , 0 , 50 , n_two_disp , "N DISPLACED TWO TRACK PAIRS" );
    // study three track pairs
    unsigned int n_good_three = 0u;
    AnaTrack::ftype three_minimum_chi2 = 9999.;
    AnaTrack::ftype three_minimum_chi2_rb = 0.;
    AnaTrack::ftype three_maximum_rb = 0.;
    AnaTrack::ftype three_maximum_rb_chi2 = 99999.;
    if( coll.size()>=3 ) { 
      vector< std::pair<AnaTrack::ftype,AnaTrack::ftype> > tracks3(3);
      for( typename type::const_iterator i=coll.begin(), f=coll.end(); i!=f; ++i ) {
        const boost::shared_ptr<const AnaTrack>& trkA( *i );
        tracks3[0] = std::pair<AnaTrack::ftype,AnaTrack::ftype>(trkA->d0()/10.,detector::delta_phi(trkA->phi0(),mean_phi0));
        for( typename type::const_iterator j=i; j!=f; ++j ) {
          if( i==j ) { continue; }
          const boost::shared_ptr<const AnaTrack>& trkB( *j );
          tracks3[1] = std::pair<AnaTrack::ftype,AnaTrack::ftype>(trkB->d0()/10.,detector::delta_phi(trkB->phi0(),mean_phi0));
          for( typename type::const_iterator k=j; k!=f; ++k ) {
            if( j==k ) { continue; }
            assert( i!=k );
            const boost::shared_ptr<const AnaTrack>& trkC( *k );
            tracks3[2] = std::pair<AnaTrack::ftype,AnaTrack::ftype>(trkC->d0()/10.,detector::delta_phi(trkC->phi0(),mean_phi0));
            LinearFit<AnaTrack::ftype> fitter3;
            fitter3.compute_details( true );
            const bool fit3_ok = fitter3.fit< std::vector< std::pair<AnaTrack::ftype,AnaTrack::ftype> > >( tracks3.begin() , tracks3.end() );
            if( !fit3_ok ) { continue; }
            const AnaTrack::ftype this_rb = fitter3.slope();
            const AnaTrack::ftype this_phib = detector::canonical_phi( detector::safe_divide(fitter3.intercept(),this_rb) + mean_phi0 );
            const AnaTrack::ftype this_chi2 = fitter3.chi2();
            dg::fillh( "three_rb" , 400 , -10 , 10 , this_rb/10. , "THREE TRACK FITTED R_{b} (cm)" );
            dg::fillh( "three_rb_wide" , 100 , -100 , 100 , this_rb/10. , "THREE TRACK FITTED R_{b} (cm)" );
            dg::fillh( "three_phib_m_mean" , 100 , -0.5 , 0.5 , detector::delta_phi( this_phib , mean_phi0 ) , "THREE TRACK FITTED #Delta #phi_{b} (rad)" );
            dg::fillh( "three_chi2" , 100 , 0 , 1 , this_chi2 , "THREE TRACK FIT #chi^{2}" );
            for( unsigned int itrack=0; itrack!=3; ++itrack ) { 
              dg::fillh( "three_chi2_individual" , 100 , 0 , 0.5 , fitter3.chi2(itrack) , "THREE TRACK FIT #chi^{2} CONTRIBUTION" );
            }
            if( this_chi2 < 0.1 ) { ++n_good_three; }
            if( this_chi2 < three_minimum_chi2 ) {
              three_minimum_chi2 = this_chi2;
              three_minimum_chi2_rb = this_rb;
            }
            if( this_rb > three_maximum_rb ) { 
              three_maximum_rb = this_rb;
              three_maximum_rb_chi2 = this_chi2;
            }
          }
        }
      }
    }
    dg::fillh( "three_n_good" , 50 , 0 , 50 , n_good_three , "NUMBER OF GOOD THREE TRACK VERTICES" );
    dg::fillh( "three_minimum_chi2" , 100 , 0 , 0.5 , three_minimum_chi2 , "MINIMUM THREE TRACK FIT CHI^2" );
    dg::fillh( "three_minimum_chi2_rb" , 100 , -2 , 50 , three_minimum_chi2_rb/10. , "R_{b} (cm) AT MINIMUM THREE TRACK FIT CHI^2" );
    dg::fillh( "three_maximum_rb" , 100 , 0 , 50 , three_maximum_rb/10. , "MAXIMUM THREE TRACK FIT R_{b} (cm)" );
    dg::fillh( "three_maximum_rb_chi2" , 100 , 0 , 0.5 , three_maximum_rb_chi2 , "#chi^{2} AT MAXIMUM THREE TRACK FIT R_{b}" );
  } // end study the track collection
public:
  StudyTrackDisplacement( const std::string& name , const std::string& description , 
                          const AnaTrack::ftype& jet_phi , const AnaTrack::ftype& b_decay_radius , const AnaTrack::ftype& b_decay_phi ) 
    : DataGraphics::DgStudy<type>(name,description)
    , _jet_phi(jet_phi)
    , _b_decay_radius(b_decay_radius)
    , _b_decay_phi(b_decay_phi)
  {}
  virtual ~StudyTrackDisplacement() {}
};

#endif // WRAP_STUDYTRACKDISPLACEMENT_HH

