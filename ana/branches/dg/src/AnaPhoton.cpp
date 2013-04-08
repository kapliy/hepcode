#include "TrigFTKAna_config.h"

#include <cmath>
#include <boost/shared_ptr.hpp>
#include "TrigFTKAna/AnaEventMgr.hpp"
#include "TrigFTKAna/AnaEvent.hpp"
#include "TrigFTKAna/Detector.hpp"
#include "TrigFTKAna/AnaPhoton.hpp"

#ifdef HAVE_COMMONANALYSIS
#include "egammaAnalysisUtils/EnergyRescaler.h"
using namespace eg2011;
#endif

using namespace boost;

boost::shared_ptr<const AnaPhoton>
AnaPhoton::cluster_photon_v16( const boost::shared_ptr<const AnaPhoton>& phot )
{
  // make new ele and set four-vector
  boost::shared_ptr<AnaPhoton> new_phot(boost::make_shared<AnaPhoton>(*phot) );
  new_phot->_momentum.SetPtEtaPhiE( phot->cluster_pt() ,
                                    phot->cluster_eta() ,
                                    phot->cluster_phi() ,
                                    phot->cluster_energy() );

  // define parent
  new_phot->_uncorrected_parent = phot;

  // done.
  return boost::const_pointer_cast<const AnaPhoton>( new_phot );
}



boost::shared_ptr<const AnaPhoton>
AnaPhoton::corrected_cluster_photon_v16( const boost::shared_ptr<const AnaPhoton>& phot , const unsigned long& event_number , const size_t& phot_number , const int& corrtype )
{
#ifdef HAVE_COMMONANALYSIS
  static EnergyRescaler *rescaler;
  rescaler = new EnergyRescaler();
  rescaler->useDefaultCalibConstants();
  // different seed specification in new rescaler (20 May 2011)
  rescaler->SetRandomSeed( event_number + phot_number + 171 );
  double corrE = rescaler->applyEnergyCorrectionGeV( phot->cluster_eta() , phot->cluster_phi() , phot->cluster_energy() ,
                                                  phot->cluster_pt() , corrtype , "UNCONVERTED_PHOTON" );
  // make new ele and set four-vector
  boost::shared_ptr<AnaPhoton> new_phot(boost::make_shared<AnaPhoton>(*phot) );
  new_phot->_momentum.SetPtEtaPhiE( corrE / cosh( phot->cluster_eta() ) ,
                                    phot->cluster_eta() ,
                                    phot->cluster_phi() ,
                                    corrE );
  // define parent
  new_phot->_uncorrected_parent = phot;
  delete rescaler;
  // done.
  return boost::const_pointer_cast<const AnaPhoton>( new_phot );
#else
  assert(0&&NO_ANALYSIS_MSG);
  return phot;
#endif
}



boost::shared_ptr<const AnaPhoton>
AnaPhoton::smeared_cluster_photon_v16( const boost::shared_ptr<const AnaPhoton>& phot , const unsigned long& event_number , const size_t& phot_number , const int& corrtype )
{
#ifdef HAVE_COMMONANALYSIS
  static EnergyRescaler *rescaler;
  rescaler = new EnergyRescaler();
  // different seed specification in new rescaler (20 May 2011)
  rescaler->SetRandomSeed( event_number + phot_number + 171 );
  double mcWithConstantTerm = true;
  double smearCorr = 0.;
  smearCorr = rescaler->getSmearingCorrectionGeV( phot->cluster_eta() , phot->cluster_energy() , corrtype , mcWithConstantTerm );
  // make new ele and set four-vector
  boost::shared_ptr<AnaPhoton> new_phot(boost::make_shared<AnaPhoton>(*phot) );
  new_phot->_momentum.SetPtEtaPhiE( smearCorr * phot->cluster_pt() ,
                                    phot->cluster_eta() ,
                                    phot->cluster_phi() ,
                                    smearCorr * phot->cluster_energy() );
  // define parent
  new_phot->_uncorrected_parent = phot;
  delete rescaler;
  // done.
  return boost::const_pointer_cast<const AnaPhoton>( new_phot );
#else
  assert(0&&NO_ANALYSIS_MSG);
  return phot;
#endif
}


boost::shared_ptr<const AnaPhoton>
AnaPhoton::corrected_smeared_cluster_photon_v16( const boost::shared_ptr<const AnaPhoton>& phot , const unsigned long& event_number , const size_t& phot_number , const int& corrtype )
{
#ifdef HAVE_COMMONANALYSIS
  static EnergyRescaler *rescaler;
  rescaler = new EnergyRescaler();
  rescaler->useDefaultCalibConstants();
  // different seed specification in new rescaler (20 May 2011)
  rescaler->SetRandomSeed( event_number + phot_number + 171 );
  double mcWithConstantTerm = true;
  double smearCorr = 0.;
  smearCorr = rescaler->getSmearingCorrectionGeV( phot->cluster_eta() , phot->cluster_energy() , 0 , mcWithConstantTerm );
  double corr_factor  = 1.;
  double corr_nominal = rescaler->applyEnergyCorrectionGeV( phot->cluster_eta() , phot->cluster_phi() , phot->cluster_energy() ,
                                                         phot->cluster_pt() , 0 , "UNCONVERTED_PHOTON" );
  double corr_scaled  = rescaler->applyEnergyCorrectionGeV( phot->cluster_eta() , phot->cluster_phi() , phot->cluster_energy() ,
                                                         phot->cluster_pt() , corrtype , "UNCONVERTED_PHOTON" );
  corr_factor = corr_scaled/corr_nominal;
  // make new ele and set four-vector
  boost::shared_ptr<AnaPhoton> new_phot(boost::make_shared<AnaPhoton>(*phot) );
  new_phot->_momentum.SetPtEtaPhiE( corr_factor * smearCorr * phot->cluster_pt() ,
                                    phot->cluster_eta() ,
                                    phot->cluster_phi() ,
                                    corr_factor * smearCorr * phot->cluster_energy() );
  // define parent
  new_phot->_uncorrected_parent = phot;
  delete rescaler;
  // done.
  return boost::const_pointer_cast<const AnaPhoton>( new_phot );
#else
  assert(0&&NO_ANALYSIS_MSG);
  return phot;
#endif
}
