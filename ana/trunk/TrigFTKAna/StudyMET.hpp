#ifndef WRAP_STUDYMET_HH
#define WRAP_STUDYMET_HH


#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/foreach.hpp>
#include "DataGraphics/DgStudy.hh"
#include "DataGraphics/DgCollection.hh"
#include "TrigFTKAna/AnaMET.hpp"
#include "TrigFTKAna/AnaParticle.hpp"
#include "TrigFTKAna/Detector.hpp"
#include "TrigFTKAna/StudyParticle.hpp"

template<typename T> class EventObjectColl;

class
StudyMET : public DataGraphics::DgStudy< boost::shared_ptr<const AnaMET> >
{
public:
  typedef boost::shared_ptr<const AnaMET> type; // must be same type as the template argument for DgStudy.
  static void study( type const& missinget ) {
    using namespace DataGraphics;
    dg::fillh( "missing_et" , 100 , 0 , 1000 , missinget->met() , "Missing E_{T} (GeV)" );
    dg::fillh( "phi" , 48 , -M_PI , M_PI , missinget->metphi() );
    dg::fillh( "phi_mod_48" , 100 , 0 , M_PI/48. , detector::fmodd(missinget->metphi(),static_cast<AnaParticle::ftype>(M_PI/48.),static_cast<AnaParticle::ftype>(0)) , "Local #phi (rad)" );
    dg::fillh( "missing_et_vs_phi" , 48 , -M_PI , M_PI , 100 , 0 , 1000 , missinget->metphi() , missinget->met() , "#phi (rad)" , "Missing E_{T} (GeV)" );
    dg::filleff( "is_good" , missinget->is_good() );
    dg::filleff( "is_good_vs_phi" , 48 , -M_PI , M_PI , missinget->is_good() , "#phi (rad)" , "Is Good" );
  }
  static void study_correlations( type const& met , const boost::shared_ptr<const AnaParticle>& particle ) {
    using namespace DataGraphics;
    const AnaParticle::ftype dphi = detector::delta_phi( met->metphi() , particle->phi() );
    dg::fillh( "particle_delta_phi" , 96 , -M_PI , M_PI , dphi , "#Delta #phi (rad)" );
    dg::fillh( "met_vs_particle_delta_phi" , 96 , -M_PI , M_PI , 100 , 0 , 100 , dphi , met->met() , "#Delta #phi (rad)" , "Missing E_{T} (GeV)" );
    dg::fillh( "met_vs_particle_phi" , 96 , -M_PI , M_PI , 100 , 0 , 100 , particle->phi() , met->met() , "#phi (rad)" , "Missing E_{T} (GeV)" );
  }
protected:
  virtual void _study( type const& met ) { // the second const is important---matches the base class.
    using namespace DataGraphics;
    StudyParticle::study(met);
    StudyMET::study(met);
    if( _particles && !(_particles->empty()) ) {
      // look at correlations with each of these particles
      for( EventObjectColl< boost::shared_ptr<const AnaParticle> >::const_iterator i=_particles->cbegin(), f=_particles->cend(); i!=f; ++i ) {
        const boost::shared_ptr<const AnaParticle>& particle( *i );
        study_correlations( met , particle );
      }
    }
  } // end study of each MET
protected:
  boost::optional< const EventObjectColl< boost::shared_ptr<const AnaParticle> >& > _particles;
public:
  StudyMET( const std::string& name , const std::string& description ) 
    : _particles()
    , DataGraphics::DgStudy<type>(name,description)
  {}
  virtual ~StudyMET() {}
  void for_particles( const EventObjectColl< boost::shared_ptr<const AnaParticle> >& particles ) { _particles.reset( particles ); }
};

#endif // WRAP_STUDYMET_HH

