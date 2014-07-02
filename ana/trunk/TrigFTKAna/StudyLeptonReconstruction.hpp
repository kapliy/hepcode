#ifndef WRAP_STUDYTRACKINGEFFICIENCY_HPP
#define WRAP_STUDYTRACKINGEFFICIENCY_HPP

// "visitor"/"functor" to make lepton reconstruction plots.

#include <string>
#include <cmath>
#include <utility>
#include <vector>
#include <algorithm>
#include <boost/shared_ptr.hpp>
#include <boost/scope_exit.hpp>
#include "DataGraphics/DgStudy.hh"
#include "DataGraphics/DgCollection.hh"
#include "TrigFTKAna/AnaParticle.hpp"
#include "TrigFTKAna/AnaElectron.hpp"
#include "TrigFTKAna/AnaMuon.hpp"
#include "TrigFTKAna/AnaTrack.hpp"
#include "TrigFTKAna/AnaTruthParticle.hpp"
#include "TrigFTKAna/StudyElectron.hpp"
#include "TrigFTKAna/StudyMuon.hpp"
#include "TrigFTKAna/Detector.hpp"

template<typename input_type = const AnaParticle>
class
StudyLeptonReconstruction : public DataGraphics::DgStudy< boost::shared_ptr<input_type> >
{
public:
  typedef boost::shared_ptr<input_type> type; // must be same type as the template argument for DgStudy.
protected:
  void _study_electron( type const& p ) { 
    // plot electron reconstruction efficiency, properties.
    using namespace DataGraphics;
    using boost::shared_ptr;
    dg::down( "ele" , "electron reconstruction study" ); BOOST_SCOPE_EXIT(void) { dg::up(); } BOOST_SCOPE_EXIT_END;
    // look for delta-phi-eta match in electron reconstruction output.
    typename std::vector<type>::const_iterator iele = std::min_element( _reconstructed_electrons.begin() , 
                                                                        _reconstructed_electrons.end() , 
                                                                        AnaParticle::DeltaEtaPhiLessThan( p->eta() , p->phi() ) );
    const double ele_delta_phi = iele!=_reconstructed_electrons.end() ? p->dist_phi( *iele ) : 99999.;
    const double ele_delta_eta = iele!=_reconstructed_electrons.end() ? p->dist_eta( *iele ) : 99999.;
    const double ele_delta_r = detector::quadrature( ele_delta_phi , ele_delta_eta );
    shared_ptr<const AnaElectron> ele; if( iele!=_reconstructed_electrons.end() ) { ele = boost::dynamic_pointer_cast<const AnaElectron>( *iele ); }
    if( !ele ) { return; }
    // make electron-specific plots
    StudyElectron::study( ele );
    // plot ele eff vs eta
    dg::fillh( "dr" , 100 , 0 , 4 , ele_delta_r , "#Delta R" );
    dg::fillh( "dr_small" , 100 , 0 , 0.4 , ele_delta_r , "#Delta R" );
    dg::fillh( "dphi" , 100 , 0 , 4 , ele_delta_r , "#Delta #phi" );
    dg::fillh( "dphi_small" , 100 , 0 , 0.4 , ele_delta_r , "#Delta #phi" );
    dg::fillh( "deta" , 100 , 0 , 4 , ele_delta_r , "#Delta #eta" );
    dg::fillh( "deta_small" , 100 , 0 , 0.4 , ele_delta_r , "#Delta #eta" );
    dg::fillh( "dr_vs_eta" , 80 , -4 , 4 , 100 , 0 , 4 , p->eta() , ele_delta_r , "#eta" , "#Delta R" );
    dg::fillh( "deta_vs_eta" , 80 , -4 , 4 , 100 , -4 , 4 , p->eta() , ele_delta_eta , "#eta" , "#Delta #eta" );
    dg::fillh( "dphi_vs_eta" , 80 , -4 , 4 , 100 , -M_PI , M_PI , p->eta() , ele_delta_phi , "#eta" , "#Delta #phi" );
    dg::fillh( "dr_vs_phi" , 96 , -M_PI , M_PI , 100 , 0 , 4 , p->phi() , ele_delta_r , "#phi" , "#Delta R" );
    dg::fillh( "deta_vs_phi" , 96 , -M_PI , M_PI , 100 , -4 , 4 , p->phi() , ele_delta_eta , "#phi" , "#Delta #eta" );
    dg::fillh( "dphi_vs_phi" , 96 , -M_PI , M_PI , 100 , -M_PI , M_PI , p->phi() , ele_delta_phi , "#phi" , "#Delta #phi" );
    dg::filleff( "eff_vs_eta" , 100 , -4 , 4 , p->eta() , ele_delta_r<0.05 , "#eta" );
    dg::filleff( "eff_vs_eta_0crack" , 100 , -0.25, 0.25 , p->eta() , ele_delta_r<0.05 , "#eta" );
    dg::filleff( "eff_vs_eta_14crack" , 100 , 1.4 , 1.6 , std::abs(p->eta()) , ele_delta_r<0.05 , "#eta" );
    dg::filleff( "eff_vs_phi" , 100 , -M_PI , M_PI , p->phi() , ele_delta_r<0.05 , "#phi" );
    dg::filleff( "eff_vs_phimod24" , 100 , 0 , M_PI/12. , detector::fmodd(static_cast<double>(p->phi()),M_PI/12.,0.) , ele_delta_r<0.05 , "#phi mod 24" );
    if( std::abs(p->eta()) < 1.4 ) { dg::filleff( "eff_vs_phimod24_central" , 100 , 0 , M_PI/12. , detector::fmodd(static_cast<double>(p->phi()),M_PI/12.,0.) , ele_delta_r<0.05 , "#phi mod 24" ); }
    dg::filleff( "eff_vs_pt" , 100 , 0 , 100 , p->pt() , ele_delta_r<0.05 , "p_{T} (GeV)" );
    dg::filleff( "eff_vs_eta_phi" , 80 , -4 , 4 , 48 , -M_PI , M_PI , p->eta() , p->phi() , ele_delta_r<0.05 , "#eta" , "#phi" );

    if( ele_delta_r<0.05 ) { 
      dg::fillh( "found_eta_vs_phi" , 48 , -M_PI , M_PI , 80 , -4 , 4 , p->phi() , p->eta() , "#phi" , "#eta" );
      if( ele ) { 
        dg::fillh( "found_author" , 10 , 0 , 10 , ele->author() ); 
        if( ele->author() == 1 ) { dg::fillh( "found_eta_vs_phi_auth1" , 48 , -M_PI , M_PI , 80 , -4 , 4 , p->phi() , p->eta() , "#phi" , "#eta" ); }
        if( ele->author() == 3 ) { dg::fillh( "found_eta_vs_phi_auth3" , 48 , -M_PI , M_PI , 80 , -4 , 4 , p->phi() , p->eta() , "#phi" , "#eta" ); }
        if( ele->pass_otx() ) { 
          dg::fillh( "found_eta_vs_phi_passotx" , 48 , -M_PI , M_PI , 80 , -4 , 4 , p->phi() , p->eta() , "#phi" , "#eta" );
        } else {
          dg::fillh( "found_eta_vs_phi_failotx" , 48 , -M_PI , M_PI , 80 , -4 , 4 , p->phi() , p->eta() , "#phi" , "#eta" );
        }
        dg::filleff( "found_dr_eff_is_ElectronLoose" , ele->is_ElectronLoose_v16() );
        if( ele->is_ElectronLoose_v16() ) { 
          dg::fillh( "found_eta_vs_phi_loose" , 48 , -M_PI , M_PI , 80 , -4 , 4 , p->phi() , p->eta() , "#phi" , "#eta" );
        } else {
          dg::fillh( "found_eta_vs_phi_notloose" , 48 , -M_PI , M_PI , 80 , -4 , 4 , p->phi() , p->eta() , "#phi" , "#eta" );
        }
        dg::filleff( "found_dr_eff_is_ElectronLoosePlusPlus" , ele->is_ElectronLoosePlusPlus_v16() );
        if( ele->is_ElectronLoosePlusPlus_v16() ) { 
          dg::fillh( "found_eta_vs_phi_loose" , 48 , -M_PI , M_PI , 80 , -4 , 4 , p->phi() , p->eta() , "#phi" , "#eta" );
        } else {
          dg::fillh( "found_eta_vs_phi_notloose" , 48 , -M_PI , M_PI , 80 , -4 , 4 , p->phi() , p->eta() , "#phi" , "#eta" );
        }
        dg::filleff( "found_dr_eff_is_ElectronMedium" , ele->is_ElectronMedium_v16() );
        if( ele->is_ElectronMedium_v16() ) { 
          dg::fillh( "found_eta_vs_phi_medium" , 48 , -M_PI , M_PI , 80 , -4 , 4 , p->phi() , p->eta() , "#phi" , "#eta" );
        } else {
          dg::fillh( "found_eta_vs_phi_notmedium" , 48 , -M_PI , M_PI , 80 , -4 , 4 , p->phi() , p->eta() , "#phi" , "#eta" );
        }
        dg::filleff( "found_dr_eff_is_ElectronMediumPlusPlus" , ele->is_ElectronMediumPlusPlus_v16() );
        if( ele->is_ElectronMediumPlusPlus_v16() ) { 
          dg::fillh( "found_eta_vs_phi_medium" , 48 , -M_PI , M_PI , 80 , -4 , 4 , p->phi() , p->eta() , "#phi" , "#eta" );
        } else {
          dg::fillh( "found_eta_vs_phi_notmedium" , 48 , -M_PI , M_PI , 80 , -4 , 4 , p->phi() , p->eta() , "#phi" , "#eta" );
        }
        dg::filleff( "found_dr_eff_is_ElectronTight_v16" , ele->is_ElectronTight_v16() );
        if( ele->is_ElectronTight_v16() ) { 
          dg::fillh( "found_eta_vs_phi_tightv16" , 48 , -M_PI , M_PI , 80 , -4 , 4 , p->phi() , p->eta() , "#phi" , "#eta" );
        } else {
          dg::fillh( "found_eta_vs_phi_nottightv16" , 48 , -M_PI , M_PI , 80 , -4 , 4 , p->phi() , p->eta() , "#phi" , "#eta" );
        }
      } // end if this is really an AnaElectron and not something else (e.g. a track)
    } else {
      dg::fillh( "not_found_eta_vs_phi" , 48 , -M_PI , M_PI , 80 , -4 , 4 , p->phi() , p->eta() , "#phi" , "#eta" );
    }
    if( p->pt() > 20. ) { 
      dg::filleff( "eff_vs_phimod24_pt20" , 150 , 0 , M_PI/12. , detector::fmodd(static_cast<double>(p->phi()),M_PI/12.,0.) , ele_delta_r<0.05 , "#phi mod 24" ); 
      if( ele_delta_r<0.05 ) { 
        dg::fillh( "found_eta_vs_phi_pt20" , 48 , -M_PI , M_PI , 80 , -4 , 4 , p->phi() , p->eta() , "#phi" , "#eta" );
      } else {
        dg::fillh( "not_found_eta_vs_phi_pt20" , 48 , -M_PI , M_PI , 80 , -4 , 4 , p->phi() , p->eta() , "#phi" , "#eta" );
      }
    }
    // plot efficiency inside electron acceptance
    if( p->pt() > 10. && std::abs(p->eta()) < 2.7 ) {
      dg::down( "fid" , "fiducial truth electrons: |eta|<2.7, pT>10 GeV" ); BOOST_SCOPE_EXIT(void) { dg::up(); } BOOST_SCOPE_EXIT_END;
      dg::filleff( "eff_vs_eta" , 100 , -4 , 4 , p->eta() , ele_delta_r<0.05 , "#eta" );
      dg::filleff( "eff_vs_eta_0crack" , 100 , -0.25, 0.25 , p->eta() , ele_delta_r<0.05 , "#eta" );
      dg::filleff( "eff_vs_eta_14crack" , 100 , 1.4 , 1.6 , std::abs(p->eta()) , ele_delta_r<0.05 , "#eta" );
      dg::filleff( "eff_vs_phi" , 100 , -M_PI , M_PI , p->phi() , ele_delta_r<0.05 , "#phi" );
      dg::filleff( "eff_vs_phimod24" , 100 , 0 , M_PI/12. , detector::fmodd(static_cast<double>(p->phi()),M_PI/12.,0.) , ele_delta_r<0.05 , "#phi mod 24" );
      if( std::abs(p->eta()) < 1.4 ) { dg::filleff( "eff_vs_phimod24_central" , 100 , 0 , M_PI/12. , detector::fmodd(static_cast<double>(p->phi()),M_PI/12.,0.) , ele_delta_r<0.05 , "#phi mod 24" ); }
      dg::filleff( "eff_vs_pt" , 100 , 0 , 100 , p->pt() , ele_delta_r<0.05 , "p_{T} (GeV)" );
      dg::filleff( "eff_vs_eta_phi" , 80 , -4 , 4 , 48 , -M_PI , M_PI , p->eta() , p->phi() , ele_delta_r<0.05 , "#eta" , "#phi" );
      if( ele_delta_r<0.05 ) { 
        dg::fillh( "found_eta_vs_phi" , 48 , -M_PI , M_PI , 80 , -4 , 4 , p->phi() , p->eta() , "#phi" , "#eta" );
      } else {
        dg::fillh( "not_found_eta_vs_phi" , 48 , -M_PI , M_PI , 80 , -4 , 4 , p->phi() , p->eta() , "#phi" , "#eta" );
      }
    }
  } // end of ele rec study
  void _study_muon( type const& p ) { 
    // plot muon reconstruction efficiency, properties.
    using namespace DataGraphics;
    using boost::shared_ptr;
    dg::down( "mu" , "muon reconstruction study" ); BOOST_SCOPE_EXIT(void) { dg::up(); } BOOST_SCOPE_EXIT_END;
    // look for delta-phi-eta match in muon reconstruction output.
    typename std::vector<type>::const_iterator imu = std::min_element( _reconstructed_muons.begin() , 
                                                                       _reconstructed_muons.end() , 
                                                                       AnaParticle::DeltaEtaPhiLessThan( p->eta() , p->phi() ) );
    const double mu_delta_phi = imu!=_reconstructed_muons.end() ? p->dist_phi( *imu ) : 99999.;
    const double mu_delta_eta = imu!=_reconstructed_muons.end() ? p->dist_eta( *imu ) : 99999.;
    const double mu_delta_r = detector::quadrature( mu_delta_phi , mu_delta_eta );
    shared_ptr<const AnaMuon> mu; if( imu!=_reconstructed_muons.end() ) { mu = boost::dynamic_pointer_cast<const AnaMuon>( *imu ); }
    if( !mu ) { return; }
    // make muon-specific plots
    StudyMuon::study( mu );
    // plot mu eff vs eta
    dg::fillh( "dr" , 100 , 0 , 4 , mu_delta_r , "#Delta R" );
    dg::fillh( "dr_small" , 100 , 0 , 0.4 , mu_delta_r , "#Delta R" );
    dg::fillh( "dphi" , 100 , 0 , 4 , mu_delta_r , "#Delta #phi" );
    dg::fillh( "dphi_small" , 100 , 0 , 0.4 , mu_delta_r , "#Delta #phi" );
    dg::fillh( "deta" , 100 , 0 , 4 , mu_delta_r , "#Delta #eta" );
    dg::fillh( "deta_small" , 100 , 0 , 0.4 , mu_delta_r , "#Delta #eta" );
    dg::fillh( "dr_vs_eta" , 80 , -4 , 4 , 100 , 0 , 4 , p->eta() , mu_delta_r , "#eta" , "#Delta R" );
    dg::fillh( "deta_vs_eta" , 80 , -4 , 4 , 100 , -4 , 4 , p->eta() , mu_delta_eta , "#eta" , "#Delta #eta" );
    dg::fillh( "dphi_vs_eta" , 80 , -4 , 4 , 100 , -M_PI , M_PI , p->eta() , mu_delta_phi , "#eta" , "#Delta #phi" );
    dg::fillh( "dr_vs_phi" , 96 , -M_PI , M_PI , 100 , 0 , 4 , p->phi() , mu_delta_r , "#phi" , "#Delta R" );
    dg::fillh( "deta_vs_phi" , 96 , -M_PI , M_PI , 100 , -4 , 4 , p->phi() , mu_delta_eta , "#phi" , "#Delta #eta" );
    dg::fillh( "dphi_vs_phi" , 96 , -M_PI , M_PI , 100 , -M_PI , M_PI , p->phi() , mu_delta_phi , "#phi" , "#Delta #phi" );
    dg::filleff( "eff_vs_eta" , 100 , -4 , 4 , p->eta() , mu_delta_r<0.05 , "#eta" );
    dg::filleff( "eff_vs_eta_0crack" , 100 , -0.25, 0.25 , p->eta() , mu_delta_r<0.05 , "#eta" );
    dg::filleff( "eff_vs_eta_15crack" , 100 , 1. , 1.5 , std::abs(p->eta()) , mu_delta_r<0.05 , "#eta" );
    dg::filleff( "eff_vs_phi" , 100 , -M_PI , M_PI , p->phi() , mu_delta_r<0.05 , "#phi" );
    dg::filleff( "eff_vs_phimod24" , 100 , 0 , M_PI/12. , detector::fmodd(static_cast<double>(p->phi()),M_PI/12.,0.) , mu_delta_r<0.05 , "#phi mod 24" );
    dg::filleff( "eff_vs_pt" , 100 , 0 , 100 , p->pt() , mu_delta_r<0.05 , "p_{T} (GeV)" );
    if( mu_delta_r<0.05 ) { 
      dg::fillh( "found_eta_vs_phi" , 48 , -M_PI , M_PI , 80 , -4 , 4 , p->phi() , p->eta() , "#phi" , "#eta" );
      if( mu ) { 
        dg::filleff( "found_eta_vs_phi_is_combined" , mu->is_combined() );
        if( mu->is_combined() ) { 
          dg::fillh( "found_eta_vs_phi_combined" , 48 , -M_PI , M_PI , 80 , -4 , 4 , p->phi() , p->eta() , "#phi" , "#eta" );
        } else {
          dg::fillh( "found_eta_vs_phi_notcombined" , 48 , -M_PI , M_PI , 80 , -4 , 4 , p->phi() , p->eta() , "#phi" , "#eta" );
        }
      } // end if this is really an AnaMuon and not something else (e.g. an AnaTrack).
    } else {
      dg::fillh( "not_found_eta_vs_phi" , 48 , -M_PI , M_PI , 80 , -4 , 4 , p->phi() , p->eta() , "#phi" , "#eta" );
    }
    if( p->pt() > 20. ) { 
      dg::filleff( "eff_vs_phimod24_pt20" , 150 , 0 , M_PI/12. , detector::fmodd(static_cast<double>(p->phi()),M_PI/12.,0.) , mu_delta_r<0.05 , "#phi mod 24" ); 
      if( mu_delta_r<0.05 ) { 
        dg::fillh( "found_eta_vs_phi_pt20" , 48 , -M_PI , M_PI , 80 , -4 , 4 , p->phi() , p->eta() , "#phi" , "#eta" );
      } else {
        dg::fillh( "not_found_eta_vs_phi_pt20" , 48 , -M_PI , M_PI , 80 , -4 , 4 , p->phi() , p->eta() , "#phi" , "#eta" );
      }
    }
    // plot efficiency inside muon acceptance
    if( p->pt() > 10. && std::abs(p->eta()) < 2.7 ) {
      dg::down( "fid" , "fiducial truth muons: |eta|<2.7, pT>10 GeV" ); BOOST_SCOPE_EXIT(void) { dg::up(); } BOOST_SCOPE_EXIT_END;
      dg::filleff( "eff_vs_eta" , 100 , -4 , 4 , p->eta() , mu_delta_r<0.05 , "#eta" );
      dg::filleff( "eff_vs_eta_0crack" , 100 , -0.25, 0.25 , p->eta() , mu_delta_r<0.05 , "#eta" );
      dg::filleff( "eff_vs_eta_15crack" , 100 , 1. , 1.5 , std::abs(p->eta()) , mu_delta_r<0.05 , "#eta" );
      dg::filleff( "eff_vs_phi" , 100 , -M_PI , M_PI , p->phi() , mu_delta_r<0.05 , "#phi" );
      dg::filleff( "eff_vs_phimod24" , 100 , 0 , M_PI/12. , detector::fmodd(static_cast<double>(p->phi()),M_PI/12.,0.) , mu_delta_r<0.05 , "#phi mod 24" );
      dg::filleff( "eff_vs_pt" , 100 , 0 , 100 , p->pt() , mu_delta_r<0.05 , "p_{T} (GeV)" );
      if( mu_delta_r<0.05 ) { 
        dg::fillh( "found_eta_vs_phi" , 48 , -M_PI , M_PI , 80 , -4 , 4 , p->phi() , p->eta() , "#phi" , "#eta" );
      } else {
        dg::fillh( "not_found_eta_vs_phi" , 48 , -M_PI , M_PI , 80 , -4 , 4 , p->phi() , p->eta() , "#phi" , "#eta" );
      }
    }
  } // end of muon rec study
  virtual void _study( type const& p ) { // the second const is important---matches the base class.
    if( p->pt() < 0.0001 ) { return; } // skip zero-pt inputs.
    _study_electron( p );
    _study_muon( p );
  } // end of rec study 
private:
  std::vector< boost::shared_ptr<input_type> > _reconstructed_electrons;
  std::vector< boost::shared_ptr<input_type> > _reconstructed_muons;
public:
  StudyLeptonReconstruction( const std::string& name , const std::string& description ) 
    : DataGraphics::DgStudy<type>(name,description)
  {}
  virtual ~StudyLeptonReconstruction() {}
  template<typename eleIterT>
  void reconstructed_electrons( eleIterT begin , eleIterT end ) { _reconstructed_electrons.assign( begin , end ); }
  template<typename muIterT>
  void reconstructed_muons( muIterT begin , muIterT end ) { _reconstructed_muons.assign( begin , end ); }
};

#endif // WRAP_STUDYTRACKINGEFFICIENCY_HH

