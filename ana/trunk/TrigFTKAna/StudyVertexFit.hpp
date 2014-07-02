#ifndef WRAP_STUDYVERTEXFIT_HH
#define WRAP_STUDYVERTEXFIT_HH
// 

#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <cmath>
#include <boost/shared_ptr.hpp>
#include <boost/format.hpp>
#include <boost/foreach.hpp>
#include <boost/bind.hpp>
#include <boost/assign.hpp>
#include "DataGraphics/DgStudy.hh"
#include "DataGraphics/DgCollection.hh"
#include "TrigFTKAna/AnaTrack.hpp"
#include "TrigFTKAna/AnaJet.hpp"
#include "TrigFTKAna/AnaVertexFit.hpp"
#include "TrigFTKAna/Detector.hpp"
#include "TrigFTKAna/LinearFit.hpp"
#include "TrigFTKAna/StudyTrackGroup.hpp"
#include "TrigFTKAna/AnaConfiguration.hpp"


template<typename trackCollT>
class
StudyVertexFit : public DataGraphics::DgStudy<const trackCollT>
{
public:
  typedef const trackCollT type; // must be same type as the template argument for DgStudy.
protected:
  boost::shared_ptr<const AnaJet> _jet;
public:
  static void study_fit_result( const AnaVertexFit& vf , const boost::shared_ptr<const AnaJet>& jet ) {
    using namespace DataGraphics;
    using namespace std;
    // get minimum dr b hadron, if it exists.
    AnaJet::Hadron mini;
    AnaTrack::ftype mini_rad = 0.;
    AnaTrack::ftype mini_phi = 0.;
    AnaTrack::ftype mini_x = 0.;
    AnaTrack::ftype mini_y = 0.;
    AnaTrack::ftype mini_z = 0.;
    const AnaJet::ftype mindr = jet->min_dr_hadron( AnaJet::Hadron::B_QUARK , mini );
    if( mindr < 0.3 ) {
      mini_phi = mini.phi();
      mini_rad = detector::quadrature( mini.x() , mini.y() )/10.;
      mini_x = mini.x()/10.;
      mini_y = mini.y()/10.;
      mini_z = mini.z()/10.;
    }
    dg::fillh( "fit_ntracks" , 100 , 0 , 100 , vf.ntracks() );
    dg::fillh( "fit_x" , 100 , -5 , 5 , vf.x() , "x (cm)" );
    dg::fillh( "fit_y" , 100 , -5 , 5 , vf.y() , "y (cm)" );
    dg::fillh( "fit_z" , 100 , -15 , 15 , vf.z() , "z (cm)" );
    dg::fillh( "fit_dx" , 100 , -0.5 , 0.5 , vf.x() - mini_x , "#Delta x (cm)" );
    dg::fillh( "fit_dy" , 100 , -0.5 , 0.5 , vf.y() - mini_y , "#Delta y (cm)" );
    dg::fillh( "fit_dz" , 100 , -0.5 , 0.5 , vf.z() - mini_z , "#Delta z (cm)" );
    dg::fillh( "fit_z_vs_b_z" , 100 , -5 , 5 , 100 , -5 , 5 , mini_z , vf.z() , "z (cm)" , "z (cm)" );
    dg::fillh( "fit_perp" , 100 , 0 , 5 , vf.perp() , "sqrt(x^2+y^2) (cm)" );
    dg::fillh( "fit_perp_vs_b_rad" , 100 , 0 , 5 , 100 , 0 , 5 , mini_rad , vf.perp() , "b hadron decay radius (cm)" , "sqrt(x^2+y^2) (cm)" );
    dg::fillh( "fit_perp_vs_chi2ndof" , 5 , 0 , 25 , 40 , 0 , 4 , vf.chi2ndof() , vf.perp() , "chi^{2} / n_{dof}" , "sqrt(x^2+y^2) (cm)" );
    dg::fillh( "fit_signed_perp" , 100 , -4 , 4 , vf.signed_perp(jet->phi()) , "sqrt(x^2+y^2) (cm) signed by jet direction" );
    dg::fillh( "fit_signed_perp_vs_b_rad" , 100 , 0 , 4 , 100 , -4 , 4 , mini_rad , vf.signed_perp(jet->phi()) , "b hadron decay radius (cm)" , "sqrt(x^2+y^2) (cm) signed by jet direction" );
    dg::fillh( "fit_mag" , 100 , -20 , 20 , vf.mag() , "r (cm)" );
    dg::fillh( "fit_err_x" , 100 , 0 , 1 , sqrt(vf.cov_xx()) , "#sigma_{x} (cm)" );
    dg::fillh( "fit_err_y" , 100 , 0 , 1 , sqrt(vf.cov_yy()) , "#sigma_{y} (cm)" );
    dg::fillh( "fit_err_z" , 100 , 0 , 1 , sqrt(vf.cov_zz()) , "#sigma_{z} (cm)" );
    dg::fillh( "fit_cov_xy" , 100 , -1 , 1 , vf.cov_xy() , "cov_{xy} (cm^2)" );
    dg::fillh( "fit_cov_xz" , 100 , -1 , 1 , vf.cov_xz() , "cov_{xz} (cm^2)" );
    dg::fillh( "fit_cov_yz" , 100 , -1 , 1 , vf.cov_yz() , "cov_{yz} (cm^2)" );
    dg::fillh( "fit_chi2" , 100 , 0 , 50 , vf.chi2() , "chi2" );
    dg::fillh( "fit_chi2ndof" , 100 , 0 , 25 , vf.chi2ndof() , "chi2ndof" );
    dg::fillh( "fit_chi2prob" , 100 , 0 , 1.01 , vf.chi2prob() , "chi2prob" );
    dg::fillh( "fit_vert_px" , 100 , -20 , 20 , vf.vertex_momentum().Px() , "vertex p_{x} (GeV)" );
    dg::fillh( "fit_vert_py" , 100 , -20 , 20 , vf.vertex_momentum().Py() , "vertex p_{y} (GeV)" );
    dg::fillh( "fit_vert_pz" , 100 , -20 , 20 , vf.vertex_momentum().Pz() , "vertex p_{z} (GeV)" );
    dg::fillh( "fit_vert_pt" , 100 , 0 , 50 , vf.vertex_momentum().Pt() , "vertex p_{T} (GeV)" );
    dg::fillh( "fit_vert_p" , 100 , 0 , 50 , vf.vertex_momentum().P() , "vertex p (GeV)" );
    dg::fillh( "fit_vert_M" , 100 , 0 , 10 , vf.vertex_momentum().M() , "vertex mass (GeV)" );
    dg::fillh( "fit_vert_pdphi" , 100 , -0.5 , 0.5 , detector::delta_phi( static_cast<AnaVertexFit::ftype>(vf.vertex_momentum().Phi()) ,
                                                                          static_cast<AnaVertexFit::ftype>(mini_phi) ) ,
               "#Delta #Phi between vertex momentum and jet (rad)" );
    dg::fillh( "fit_vert_jetpdphi" , 100 , -0.5 , 0.5 , detector::delta_phi( static_cast<AnaVertexFit::ftype>(vf.vertex_momentum().Phi()) ,
                                                                             static_cast<AnaVertexFit::ftype>(jet->phi()) ) ,
               "#Delta #Phi between vertex momentum and jet (rad)" );
    dg::fillh( "fit_vert_signtojet" , 3 , -1 , 2 , detector::directionalD0( static_cast<AnaVertexFit::ftype>(1) , static_cast<AnaVertexFit::ftype>(vf.vertex_momentum().Phi()) ,
                                                                            static_cast<AnaVertexFit::ftype>(jet->phi()) ) ,
               "SIGN OF VERTEX DISPLACEMENT RELATIVE TO JET DIRECTION" );
    dg::fillh( "fit_vert_pdeta" , 50 , -1 , 1 , detector::delta_eta( static_cast<AnaVertexFit::ftype>(vf.vertex_momentum().Eta()) ,
                                                                     static_cast<AnaVertexFit::ftype>(jet->eta()) ) ,
               "#Delta #Eta between vertex momentum and jet" );
    BOOST_FOREACH( const AnaVertexFit::ftype& delta_chi2 , vf.track_delta_chi2s() ) {
      dg::fillh( "fit_track_delta_chi2" , 100 , 0 , 50 , delta_chi2 , "#Delta #chi^{2} per track" );
    }
    BOOST_FOREACH( const TLorentzVector& mom  , vf.track_momenta() ) {
      dg::fillh( "track_fit_px" , 100 , -20 , 20 , mom.Px() , "fitted track p_{x} (GeV)" );
      dg::fillh( "track_fit_py" , 100 , -20 , 20 , mom.Py() , "fitted track p_{y} (GeV)" );
      dg::fillh( "track_fit_pz" , 100 , -20 , 20 , mom.Pz() , "fitted track p_{z} (GeV)" );
      dg::fillh( "track_fit_pt" , 100 , 0 , 50 , mom.Pt() , "fitted track p_{T} (GeV)" );
    }
  }
protected:
  virtual void _study( type const& coll ) { 
    if( coll.empty() ) { return; }
    const bool debug = true;
      
    using namespace DataGraphics;
    using namespace std;
    using boost::format;
    using boost::bind;

    _study_two_track_vertices( coll );

    // const AnaTrack::ftype displaced_signed_d0_minimum_cut = 0.1; // mm
    // const AnaTrack::ftype displaced_d0_maximum_cut = 2; // mm
    const AnaTrack::ftype displaced_signed_d0_minimum_cut = 0.05; // mm
    const AnaTrack::ftype displaced_d0_maximum_cut = 1; // mm
    vector< boost::shared_ptr<const AnaTrack> > displaced_tracks;
    remove_copy_if( coll.begin() , coll.end() , back_inserter(displaced_tracks) ,
                    bind( &AnaTrack::nhits , _1 ) < 9 ||
                    bind( &AnaTrack::signed_d0 , _1 , _jet->phi() ) < displaced_signed_d0_minimum_cut ||
                    bind( &AnaTrack::d0 , _1 ) > displaced_d0_maximum_cut );
    AnaVertexFit vf;
    vf.add( displaced_tracks.begin() , displaced_tracks.end() );
    vf.track_status_all( true ); // fit all tracks

    // some track collection statistics
    AnaTrack::ftype displaced_sum_pt = 0.;
    BOOST_FOREACH( const boost::shared_ptr<const AnaTrack>& track , displaced_tracks ) {
      displaced_sum_pt += track->pt();
    }
    AnaTrack::ftype associated_sum_pt = 0.;
    unsigned int num_tracks_primary = 0u;
    unsigned int num_tracks_secondary = 0u;
    BOOST_FOREACH( const boost::shared_ptr<const AnaTrack>& track , coll ) {
      associated_sum_pt += track->pt();
      ++( ( track->d0_signif_monica() > 3. ) ? num_tracks_secondary : num_tracks_primary );
    }
    const unsigned int num_tracks_diff_ps = std::max( num_tracks_primary - num_tracks_secondary , 1u ); 
    const double btagvrtsec_chi2corr = std::sqrt( detector::chi2ndof(1.-0.1/num_tracks_diff_ps,2) ) - std::sqrt( detector::chi2ndof(1.-0.1,2) );

    // jet truth.
    const bool ok_dr_b = _jet->dr_b_parton() < AnaConfiguration::jet_label_dr_cut();
    const bool ok_dr_notb = !ok_dr_b;

    AnaJet::Hadron mini;
    AnaTrack::ftype mini_rad = 0.;
    AnaTrack::ftype mini_phi = 0.;
    AnaTrack::ftype mini_x = 0.;
    AnaTrack::ftype mini_y = 0.;
    AnaTrack::ftype mini_z = 0.;
    const AnaJet::ftype mindr = _jet->min_dr_hadron( AnaJet::Hadron::B_QUARK , mini );
    if( mindr < 0.3 ) {
      mini_phi = mini.phi();
      mini_rad = detector::quadrature( mini.x() , mini.y() )/10.;
      mini_x = mini.x()/10.;
      mini_y = mini.y()/10.;
      mini_z = mini.z()/10.;
    }

    if( debug ) { 
      cout << (format(" %=25s" ) % "JET VERTEXING") << endl;
      cout << "================================================================" << endl;
      cout << format(" jet (eta,phi) = (%|10f|,%|10f|)  Tracks: %5d associated %5d displaced") % _jet->eta() % _jet->phi() % coll.size() % displaced_tracks.size() << endl;
      cout << format("      truth dr(b,light) = (%|10f|,%|10f|)") % _jet->dr_b_parton() % _jet->dr_light_parton() << endl;
      cout << ( format("      truth b(x,y,z,r,phi) = (%|10f|,%|10f|,%|10f|,%|10f|,%|10f|)")
                % mini.x() % mini.y() % mini.z() % mini_rad % mini_phi )
           << endl;
    }
    bool fit_ok = false;
    if( displaced_tracks.size()>=2 ) { 
      // for( unsigned int nfits=1; nfits<10000; nfits*=10 ) {
      //   TStopwatch sw;
      //   for( unsigned int i=0; i!=nfits; ++i ) { 
      fit_ok = vf.fit();
      //   }    
      //   double rt = sw.RealTime();
      //   cout << " fit took " << (rt*1000.) << " ms total" << (rt*1000./nfits) << " ms per fit" << endl;
      // }
    }
    if( debug ) {
      cout << " fit " << (fit_ok ? "SUCCEEDED" : "FAILED") << endl;
    }

    dg::fillh( "ntracks_prefit" , 100 , 0 , 100 , displaced_tracks.size() );
    dg::fillh( "associated_sum_pt" , 100 , 0 , 100 , associated_sum_pt );
    dg::fillh( "displaced_sum_pt" , 100 , 0 , 100 , displaced_sum_pt );
    dg::fillh( "r_disp_sum_pt" , 100 , 0 , 1 , detector::safe_divide(displaced_sum_pt,associated_sum_pt) );
    dg::fillh( "rj_disp_sum_pt" , 100 , 0 , 1 , detector::safe_divide(displaced_sum_pt,_jet->et()) );
    dg::fillh( "rj_assoc_sum_pt" , 100 , 0 , 1 , detector::safe_divide(associated_sum_pt,_jet->et()) );
    BOOST_FOREACH( const boost::shared_ptr<const AnaTrack>& track , displaced_tracks ) {
      dg::fillh( "track_d0_prefit" , 100 , -0.2 , 0.2 , track->d0()/10. , "Track d_{0} (cm)" );
      dg::fillh( "track_signed_d0_prefit" , 100 , -0.2 , 0.2 , track->signed_d0(_jet->phi())/10. , "Track d_{0} (cm) signed by jet direction" );
      dg::fillh( "track_z0_prefit" , 100 , -2 , 2 , track->z0()/10. , "Track z_{0} (cm)" );
      dg::fillh( "track_pt_prefit" , 50 , 0 , 25 , track->pt() , "Track p_{T} (GeV)" );
      dg::fillh( "track_eta_prefit" , 60 , -3 , 3 , track->eta() , "Track #eta" );
      dg::fillh( "track_phi0_prefit" , 48 , -M_PI , M_PI , track->phi0() , "Track #phi_{0}" );
      dg::fillh( "track_dphi0_prefit" , 20 , -0.1 , 0.1 , detector::delta_phi(track->phi0(),_jet->phi()) , "Track #Delta #phi_{0} between track and jet (rad)" );
      dg::fillh( "track_deta_prefit" , 20 , -0.5 , 0.5 , detector::delta_eta(track->eta(),_jet->eta()) , "Track #Delta #eta_{0} between track and jet" );
    }

    dg::filleff( "fit_ok" , fit_ok );
    if( fit_ok ) {
      assert( vf.is_fitted() );
      study_fit_result( vf , _jet );
      dg::fillh( "fit_vert_ptfrac_jet" , 100 , 0 , 1 , detector::safe_divide(static_cast<AnaVertexFit::ftype>(vf.vertex_momentum().Pt()),_jet->et()) , "Fraction" );
      dg::fillh( "fit_vert_ptfrac_assoc" , 100 , 0 , 1 , detector::safe_divide(static_cast<AnaVertexFit::ftype>(vf.vertex_momentum().Pt()),associated_sum_pt) , "Fraction" );
      dg::fillh( "fit_vert_ptfrac_disp" , 100 , 0 , 1 , detector::safe_divide(static_cast<AnaVertexFit::ftype>(vf.vertex_momentum().Pt()),displaced_sum_pt) , "Fraction" );
      if( debug ) { 
        // dump some fit results
        cout << format( " chi2: %|8f| chi2/ndof: %|5.2f| prob: %|8.7f|" ) % vf.chi2() % vf.chi2ndof() % vf.chi2prob() << endl
             << format( " perp: %|8.6f| %|8.6f|" ) % vf.perp() % vf.signed_perp(_jet->phi()) << endl
             << ( format( " vertex dphi %|8.6f| deta: %|8.6f| pt: %|5.2f| pz: %|5.2f| mass: %|5.2f| " )
                  % detector::delta_phi( static_cast<AnaVertexFit::ftype>(vf.vertex_momentum().Phi()) , static_cast<AnaVertexFit::ftype>(_jet->phi()) )
                  % detector::delta_eta( static_cast<AnaVertexFit::ftype>(vf.vertex_momentum().Eta()) , static_cast<AnaVertexFit::ftype>(_jet->eta()) )
                  % vf.vertex_momentum().Pt() % vf.vertex_momentum().Pz() % vf.vertex_momentum().M()
                  )
             << endl;
        // dump track momenta and chi^2 contribution
        for( unsigned int itrk=0, ftrk=vf.ntracks(); itrk!=ftrk; ++itrk ) {
          TLorentzVector mom( vf.track_momentum(itrk) );
          AnaVertexFit::ftype dchi2( vf.track_delta_chi2(itrk) );
          cout << ( format( " track %|2d| p3 pt=%|5.3d| eta=%|5.3d| phi=%|5.3d| d(chi^2) %|6.2d| " ) 
                    % itrk % mom.Pt() % mom.Eta() % mom.Phi() % dchi2 ) 
               << endl;
        }
      }
      // try to retrieve true vertex position.
      // want to retrieve b hadron.
      //   look for b quarks: type (|pdg code|) == 5
      //   trace each through decay to last (e.g. non-excited) B hadron: Type is
      //      mesons: type |pdg code| % 1000 == 500-599
      //      baryons: type |pdg code| == 5000-5999
      //    and Type[KFDauNt] is not.
      //   retrieve position of b hadron decay:
      // CollectionTree->Scan("Type:KFDauNt:PhiGen:EtaGen:PtGen:MGen:RV[KEndVNt]","MGen>1500")
      // CollectionTree->Scan("Type:PhiGen:EtaGen:PtGen:RV[KEndVNt]","((abs(Type)%1000)>=500 && (abs(Type)%1000)<=599) || (abs(Type)>5000 && abs(Type)<=5999)")
      // CollectionTree->Draw("RVGen>>h","(((abs(Type[KMothNt])>500 && abs(Type[KMothNt])<599) || (abs(Type[KMothNt])>5000 && abs(Type[KMothNt])<5999))) && (abs(Type)<500)")
      // CollectionTree->Scan("Type:PhiGen:EtaGen:PtGen:GenStat:RVGen:Type[KMothNt]")
      // CollectionTree->Scan("Type:PhiGen:EtaGen:PtGen:GenStat:RVGen:Type[KMothNt]", "GenStat==1 && (((abs(Type[KMothNt])>500 && abs(Type[KMothNt])<599) || (abs(Type[KMothNt])>5000 && abs(Type[KMothNt])<5999)))")
      // CollectionTree->Draw("RVGen[KEndVNt]", "GenStat==2 && (((abs(Type)>500 && abs(Type)<599) || (abs(Type)>5000 && abs(Type)<5999)))")

      // Example:
      // Tracks:
      //  Added track 0 with parameters: 
      // cu:  0.000446 d0: 0.006504 z0: 6.585520 p0: 2.200217 ct:0.463489
      // errors: cu  0.000008 d0: 0.002619 z0: 0.010343 p0: 0.000406 ct:0.000857
      //  Added track 1 with parameters: 
      // cu:  0.002729 d0: -0.021459 z0: 6.649011 p0: 2.291030 ct:0.266667
      // errors: cu  0.000053 d0: 0.010206 z0: 0.020291 p0: 0.002117 ct:0.002860
      //  Added track 2 with parameters: 
      // cu: -0.002187 d0: -0.029016 z0: 6.614484 p0: 2.426360 ct:0.379455
      // errors: cu  0.000042 d0: 0.007161 z0: 0.018142 p0: 0.001581 ct:0.002635
      //  Added track 3 with parameters: 
      // cu: -0.000367 d0: 0.017733 z0: 6.609259 p0: 2.172247 ct:0.299927
      // errors: cu  0.000009 d0: 0.001537 z0: 0.010036 p0: 0.000308 ct:0.000838
      //  FIT STATUS: OK
      // ****************************** AnaVertexFit ******************************
      // jet (eta,phi) = (  0.396676,  2.228130)  Tracks:     5 associated     4 displaced
      // Vertex 1 position: -0.128342 0.158122 6.67971 => r=0.20365 cm phi=2.25260935631989456e+00
      /// truth: phi=2.2544698, rv=0.17966 cm

      // Truth:
      // *    Row   * Instance *      Type *    PhiGen *    EtaGen *     PtGen * RV[KEndVN *
      // ***********************************************************************************
      // *        0 *      171 *      -533 * -0.999943 * -0.495843 * 60988.070 * 0.0194043 *
      // *        0 *      293 *       523 * 2.2542932 * 0.3541436 * 39739.484 * 0.0194043 *
      // *        0 *      296 *      -531 * -0.999835 * -0.495212 * 60178.898 * 6.1011052 *
      // *        0 *      384 *       521 * 2.2544689 * 0.3538529 * 39730.777 * 1.7966378 *
    } // fit is ok
    
    // study constrained fit of all two-track pairs
    if( true ) {
      dg::down( "twotrack" , "two track vertices" ); BOOST_SCOPE_EXIT(void) { dg::up(); } BOOST_SCOPE_EXIT_END;
      std::vector< boost::shared_ptr<const AnaTrack> > tracks(2);
      unsigned int npairs = 0u;
      unsigned int n_btagvrtsec_pairs = 0u;
      double best_delta_perp = 9999.;
      AnaVertexFit best_delta_perp_vf;
      for( typename type::const_iterator i=coll.begin(), f=coll.end(); i!=f; ++i ) {
        const boost::shared_ptr<const AnaTrack>&  trkA( *i );
        tracks[0] = trkA;
        for( typename type::const_iterator j=i; j!=f; ++j ) {
          if( i==j ) { continue; }
          ++npairs;
          const boost::shared_ptr<const AnaTrack>& trkB( *j );
          tracks[1] = trkB;
          AnaVertexFit twovf;
          twovf.add( tracks.begin() , tracks.end() );
          twovf.track_status_all( true ); // fit all tracks
          const bool two_ok = twovf.fit();
          dg::filleff( "fit_ok" , two_ok );
          if( two_ok ) {
            study_fit_result( twovf , _jet );
            if( trkA->charge()*trkB->charge()>0 ) { dg::fillh( "m_pipi" , 100 , 0 , 0.8 , twovf.vertex_momentum().M() , "Invariant Mass of Net Neutral Track Pairs" ); }
            const double sellim = 2.5 + btagvrtsec_chi2corr;
            const double adp_trkA = std::sqrt(std::min(trkA->pt(),static_cast<float>(64))/64.)/2.;
            const double adp_trkB = std::sqrt(std::min(trkB->pt(),static_cast<float>(64))/64.)/2.;
            const bool ok_sig_trkA = trkA->d0_signif_monica() > ( sellim + adp_trkA );
            const bool ok_sig_trkB = trkB->d0_signif_monica() > ( sellim + adp_trkB );
            const bool ok_two_track = twovf.chi2ndof()<4.5 && ok_sig_trkA && ok_sig_trkB && (trkA->d0_signif_monica() + trkB->d0_signif_monica()) >= (2.*sellim + 2.5 + adp_trkA + adp_trkB );
            dg::filleff( "ok_two_track" , ok_two_track );
            if( ok_two_track ) { ++n_btagvrtsec_pairs; }
            if( ok_two_track ) {
              dg::down( "signif" , "passes BTagVrtSec significance criteria" ); BOOST_SCOPE_EXIT(void) { dg::up(); } BOOST_SCOPE_EXIT_END;
              study_fit_result( twovf , _jet );
              if( trkA->charge()*trkB->charge()>0 ) { dg::fillh( "m_pipi" , 100 , 0 , 0.8 , twovf.vertex_momentum().M() , "Invariant Mass of Net Neutral Track Pairs" ); }
            }
            // study two track pairings for V0's
            {
              using namespace boost::assign;
              vector< boost::shared_ptr<const AnaTrack> > tmpcoll; tmpcoll += trkA , trkB;
              StudyTrackGroup stgrp( "v0s" , "two track v0 study" );
              stgrp.for_only( tmpcoll );
            }
            // find highest radius vertex for later study
            const double this_delta_perp = twovf.perp() - mini_rad;
            if( best_delta_perp > std::abs(this_delta_perp) ) {
              best_delta_perp = std::abs(this_delta_perp);
              best_delta_perp_vf = twovf;
            }
          }
        }
      }
      dg::fillh( "n_combinations" , 100 , 0 , 100 , npairs , "NUMBER OF TWO TRACK PAIRS" );
      dg::fillh( "n_btagvrtsec_pairs" , 100 , 0 , 100, n_btagvrtsec_pairs , "NUMBER OF TWO TRACK PAIRS" ); // used in SV taggers
      // properties of fit closest to true b vertex or (if not b jet) coordinate origin
      if( best_delta_perp_vf.is_fitted() && best_delta_perp<1. ) {
        dg::fillh( "best_delta_perp" , 100 , 0 , 1. , best_delta_perp , "DISPLACEMENT FROM TRUTH OF BEST TWO TRACK VERTEX" );
        dg::down( "best_delta_perp_fit" , "best two track vertex" ); BOOST_SCOPE_EXIT(void) { dg::up(); } BOOST_SCOPE_EXIT_END;
        study_fit_result( best_delta_perp_vf , _jet );
      }
    }

    // study constrained fit of all three-track pairs
    if( true ) {
      dg::down( "threetrack" , "three track vertices" ); BOOST_SCOPE_EXIT(void) { dg::up(); } BOOST_SCOPE_EXIT_END;
      std::vector< boost::shared_ptr<const AnaTrack> > tracks(3);
      unsigned int ntriplets = 0u;
      double best_delta_perp = 9999.;
      AnaVertexFit best_delta_perp_vf;
      for( typename type::const_iterator i=coll.begin(), f=coll.end(); i!=f; ++i ) {
        const boost::shared_ptr<const AnaTrack>& trkA( *i );
        tracks[0] = trkA;
        for( typename type::const_iterator j=i; j!=f; ++j ) {
          if( i==j ) { continue; }
          const boost::shared_ptr<const AnaTrack>& trkB( *j );
          tracks[1] = trkB;
          for( typename type::const_iterator k=j; k!=f; ++k ) {
            if( i==k ) { continue; }
            if( j==k ) { continue; }
            const boost::shared_ptr<const AnaTrack>& trkC( *k );
            tracks[2] = trkC;
            ++ntriplets;
            AnaVertexFit threevf;
            threevf.add( tracks.begin() , tracks.end() );
            threevf.track_status_all( true ); // fit all tracks
            const bool three_ok = threevf.fit();
            dg::filleff( "fit_ok" , three_ok );
            if( three_ok ) {
              study_fit_result( threevf , _jet );
              const double this_delta_perp = threevf.perp() - mini_rad;
              if( best_delta_perp > std::abs(this_delta_perp) ) {
                best_delta_perp = std::abs(this_delta_perp);
                best_delta_perp_vf = threevf;
              }
            }
          }
        }
      }
      dg::fillh( "n_combinations" , 100 , 0 , 100 , ntriplets , "NUMBER OF THREE TRACK PAIRS" );
      // properties of fit closest to true b vertex or (if not b jet) coordinate origin
      if( best_delta_perp_vf.is_fitted() && best_delta_perp<1. ) {
        dg::fillh( "best_delta_perp" , 100 , 0 , 1. , best_delta_perp , "DISPLACEMENT FROM TRUTH OF BEST THREE TRACK VERTEX" );
        dg::down( "best_delta_perp_fit" , "best two track vertex" ); BOOST_SCOPE_EXIT(void) { dg::up(); } BOOST_SCOPE_EXIT_END;
        study_fit_result( best_delta_perp_vf , _jet );
      }
    }

  } // end study the track collection

  // ================================================================

  void _study_two_track_vertices( type const& coll ) {
    using namespace DataGraphics;
    using namespace std;
    // study two track vertices similar to http://alxr.usatlas.bnl.gov/lxr-stb3/source/atlas/Reconstruction/VKalVrt/BTagVrtSec/src/BTagVrtSec.cxx
    float vertex_mass = -1.;
    float vertex_pt_ratio = -1.;
    unsigned int n_two_track_vertices( 0u );
    AnaVertexFit vf;
    const bool ok_vf = vf.compute_sv_variables( coll.begin() , coll.end() , vertex_mass , vertex_pt_ratio , n_two_track_vertices , &vf );
    dg::filleff( "final_eff" , ok_vf );
    if( ok_vf ) {
      assert( vf.is_fitted() );
      dg::down( "final" , "compute_sv_result" ); BOOST_SCOPE_EXIT(void) { dg::up(); } BOOST_SCOPE_EXIT_END;
      study_fit_result( vf , _jet ); 
      dg::fillh( "good_vertex_mass_vs_pt_ratio" , 10 , 0 , 1 , 20 , 0 , 10 , vertex_pt_ratio , vertex_mass , "VERTEX PT / TRACK SUM PT" , "VERTEX MASS (GeV)" );
      dg::fillh( "good_vertex_mass_vs_n_two" , 25 , 0 , 25 , 20 , 0 , 10 , n_two_track_vertices , vertex_mass , "N TWO TRACK PAIRS" , "VERTEX MASS (GeV)" );
      dg::fillh( "good_vertex_pt_ratio_vs_n_two" , 25 , 0 , 25 , 10 , 0 , 1 , n_two_track_vertices , vertex_pt_ratio , "N TWO TRACK PAIRS" , "VERTEX PT / TRACK SUM PT" );
    }
  }

  // ================================================================

public:
  StudyVertexFit( const std::string& name , const std::string& description , const boost::shared_ptr<const AnaJet>& jet )
    : DataGraphics::DgStudy<type>(name,description)
    , _jet(jet)
  {}
  virtual ~StudyVertexFit() {}
};

#endif // WRAP_STUDYVERTEXFIT_HH

