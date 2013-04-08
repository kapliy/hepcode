#include "TrigFTKAna_config.h"

#include <cmath>
#include <boost/shared_ptr.hpp>
#include "TrigFTKAna/AnaEventMgr.hpp"
#include "TrigFTKAna/AnaEvent.hpp"
#include "TrigFTKAna/Detector.hpp"
#include "TrigFTKAna/AnaElectron.hpp"
#include "TrigFTKAna/AnaConfiguration.hpp"

#ifdef HAVE_COMMONANALYSIS
#include "egammaAnalysisUtils/checkOQ.h"
#include "egammaAnalysisUtils/EnergyRescaler.h"
#include "egammaAnalysisUtils/egammaSFclass.h"
using namespace eg2011;
#endif

using namespace boost;

void
AnaElectron::_update_cache() const
{
  // four vector for mass calculations: cluster energy measurement + track direction measurement
  _cluster_energy_particle_angle = four_vector();
  const double ratio = detector::safe_divide( static_cast<double>(_cluster_momentum.E()) , static_cast<double>(energy()) );
  _cluster_energy_particle_angle *= ratio;
  // make OTX check
  AnaEventMgr* mgr = AnaEventMgr::instance(); assert( mgr );
  shared_ptr<const AnaEvent> event( mgr->current_event() );
  if( !event ) { return; }
  const int run_number = event->run_number();
  // MC11C: no need to apply OQ check anymore:
  _passes_lar_check = true;
}

// A wrapper function that selects the correct smearing class
boost::shared_ptr<const AnaElectron>
AnaElectron::egamma_smeared( const CONF::ConfType& conf , 
			     const boost::shared_ptr<const AnaElectron>& electron , 
			     const unsigned long& event_number , const size_t& electron_number , 
			     const std::string& fun ) {
#ifdef HAVE_COMMONANALYSIS
  // options for string 'fun':
  //   NOMINAL , SMEARDOWN , SMEARUP , SCALEDOWN , SCALEUP
  switch(conf) {
  case CONF::SUSI:
  case CONF::LATEST:
    // MC11C is produced without CT (constant term)
    // https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/EnergyScaleResolutionRecommendations#Energy_resolution
    return AnaElectron::egamma_smeared_apply_v17<eg2011::EnergyRescaler>(electron,event_number,electron_number,false,fun);
    break;
  default:
    assert(0&&"Unsupported --conf");
  }
#else
  assert(0&&NO_ANALYSIS_MSG);
  return electron;
#endif
}

// A wrapper function that selections the correct correcting class
boost::shared_ptr<const AnaElectron>
AnaElectron::egamma_corrected( const CONF::ConfType& conf ,
			       const boost::shared_ptr<const AnaElectron>& electron , 
			       const unsigned long& event_number , const size_t& electron_number , 
			       const std::string& fun ) {
#ifdef HAVE_COMMONANALYSIS
  // options for string 'fun':
  //   NOMINAL , DOWN , UP
  //   NOTE (2012-01-31): the current recommendation is to apply corrections only to data, so UP and DOWN should not be used
  //   https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/EnergyScaleResolutionRecommendations#Energy_scale
  switch(conf) {
  case CONF::SUSI:
  case CONF::LATEST:
    // MC11C is produced without CT (constant term)
    // https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/EnergyScaleResolutionRecommendations#Energy_resolution
    return AnaElectron::egamma_corrected_apply_v17<eg2011::EnergyRescaler>(electron,event_number,electron_number,fun);
    break;
  default:
    assert(0&&"Unsupported --conf");
  }
#else
  assert(0&&NO_ANALYSIS_MSG);
  return electron;
#endif
}

// A wrapper class that uses the current scaling class to set recon eff and corresponding uncertainty
void
AnaElectron::egamma_effscale( const CONF::ConfType& conf ,
			      const boost::shared_ptr<const AnaElectron>& electron ,
			      float& eff , float& error )
{
#ifdef HAVE_COMMONANALYSIS
  switch(conf) {
  case CONF::SUSI:
  case CONF::LATEST:
    AnaElectron::egamma_effscale_apply_v17<egammaSFclass>(electron,eff,error);
    break;
  default:
    assert(0&&"Unsupported --conf");
  }
#else
  assert(0&&NO_ANALYSIS_MSG);
#endif
}
			      
/* Functions for 2011 analyses, Peter ntuples (no cluster corrections are made) */
/* ============================================================================ */
// For Data
boost::shared_ptr<const AnaElectron>
AnaElectron::corrected_electron_2011( const boost::shared_ptr<const AnaElectron>& ele , const unsigned long& event_number , const size_t& elec_number , const int& corrtype )
{
#ifdef HAVE_COMMONANALYSIS
  // ( https://twiki.cern.ch/twiki/bin/view/AtlasProtected/DibosonResonances#Electron_Energy_Smearing )
  static EnergyRescaler *rescaler = NULL;
  if( rescaler==NULL ) {
    rescaler = new EnergyRescaler();
    rescaler->useDefaultCalibConstants( "2011" );
  }
  rescaler->SetRandomSeed( event_number*1000 + elec_number );

  double corrE = 0.;
  if( ele->nhits(PIXEL) + ele->nhits(SCT) >=4 ) {
    corrE = rescaler->applyEnergyCorrectionGeV( ele->_cluster_momentum.Eta() , ele->_cluster_momentum.Phi() , ele->_cluster_momentum.E() ,
						ele->_cluster_momentum.E() / cosh( ele->eta() ) , corrtype , "ELECTRON" );
  } else {
    corrE = rescaler->applyEnergyCorrectionGeV( ele->_cluster_momentum.Eta() , ele->_cluster_momentum.Phi() , ele->_cluster_momentum.E() ,
						ele->_cluster_momentum.Pt() , corrtype , "ELECTRON" );
  }

  // make new ele and set four-vector                                                                             
  boost::shared_ptr<AnaElectron> new_ele(boost::make_shared<AnaElectron>(*ele) );
  // for peter ntuple, just get the correction
  // constantinos's version (broken; no track momentum filled in ntuple)
  //    new_ele->_momentum.SetPtEtaPhiE( corrE / cosh( ele->_track_momentum.Eta() ) , ele->_track_momentum.Eta() , ele->_track_momentum.Phi() , corrE );
  // my version: starts out identical to above. any modification needed?
  new_ele->_momentum.SetPtEtaPhiE( corrE / cosh( ele->eta() ) , ele->eta() , ele->phi() , corrE );

  // define parent
  new_ele->_uncorrected_parent = ele;

  return boost::const_pointer_cast<const AnaElectron>( new_ele );
#else
  assert(0&&NO_ANALYSIS_MSG);
  return ele;
#endif
}

// For MC
boost::shared_ptr<const AnaElectron>
AnaElectron::smeared_electron_2011( const boost::shared_ptr<const AnaElectron>& ele , const unsigned long& event_number , const size_t& elec_number , const int& corrtype , const bool& les_uncert )
{
#ifdef HAVE_COMMONANALYSIS
  // if les_uncert is equal to true, then corrtype controls the LES uncertainty. otherwise, corrtype controls the LER uncertainty.

  static EnergyRescaler *rescaler = NULL;
  if( rescaler==NULL ) {
    rescaler = new EnergyRescaler();
    rescaler->useDefaultCalibConstants( "2011" );
  }
  rescaler->SetRandomSeed( event_number*1000 + elec_number );
  bool mcWithConstantTerm = true;
  double smearCorr = 0.;

  int LERcorrtype = les_uncert ? 0 : corrtype; // if we want +/- 1 sigma shift in LES, then don't do shifts for LER
  smearCorr = rescaler->getSmearingCorrectionGeV( ele->_cluster_momentum.Eta() , ele->_cluster_momentum.E() , LERcorrtype , mcWithConstantTerm ); // corrtype(0=nominal ,1=error_down, 2=error_up)
  if( les_uncert && corrtype!=0 ) {
    if( ele->nhits(PIXEL) + ele->nhits(SCT) >= 4 ) {
      // calculate the +/- 1 sigma shift by dividing the shifted corrected energy by the corrected energy. This is the most correct method for the time being
      smearCorr *= ( rescaler->applyEnergyCorrectionGeV( ele->_cluster_momentum.Eta() , ele->_cluster_momentum.Phi() , ele->_cluster_momentum.E() , 
							 ele->_cluster_momentum.E() / cosh( ele->eta() ) , corrtype , "ELECTRON" ) /
		     rescaler->applyEnergyCorrectionGeV( ele->_cluster_momentum.Eta() , ele->_cluster_momentum.Phi() , ele->_cluster_momentum.E() , 
							 ele->_cluster_momentum.E() / cosh( ele->eta() ) , 0 , "ELECTRON" ) );
    } else {
      smearCorr *= ( rescaler->applyEnergyCorrectionGeV( ele->_cluster_momentum.Eta() , ele->_cluster_momentum.Phi() , ele->_cluster_momentum.E() , 
							 ele->_cluster_momentum.Pt() , corrtype , "ELECTRON" ) /
		     rescaler->applyEnergyCorrectionGeV( ele->_cluster_momentum.Eta() , ele->_cluster_momentum.Phi() , ele->_cluster_momentum.E() , 
							 ele->_cluster_momentum.Pt() , 0 , "ELECTRON" ) );
    }
  }

  boost::shared_ptr<AnaElectron> new_ele(boost::make_shared<AnaElectron>(*ele) );
  // for peter ntuple just get the correction
  new_ele->_momentum.SetPtEtaPhiE( smearCorr * ele->pt() , ele->eta() , ele->phi() , smearCorr * ele->energy() );

  // define parent
  new_ele->_uncorrected_parent = ele;

  return boost::const_pointer_cast<const AnaElectron>( new_ele );
#else
  assert(0&&NO_ANALYSIS_MSG);
  return ele;
#endif
}
/* End Functions for 2011 analyses */
