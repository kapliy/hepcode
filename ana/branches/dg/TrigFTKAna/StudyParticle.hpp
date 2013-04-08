#ifndef WRAP_STUDYPARTICLE_HH
#define WRAP_STUDYPARTICLE_HH


#include <cmath>
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/assign/std/vector.hpp>
#include "DataGraphics/DgStudy.hh"
#include "DataGraphics/DgCollection.hh"
#include "TrigFTKAna/AnaParticle.hpp"
#include "TrigFTKAna/Detector.hpp"

class
StudyParticle : public DataGraphics::DgStudy< boost::shared_ptr<const AnaParticle> >
{
public:
  typedef boost::shared_ptr<const AnaParticle>  type; // must be same type as the template argument for DgStudy.

  static void study( type const& particle ) {
    using namespace DataGraphics;
    dg::fillh( "type" , 10 , 0 , 10 , particle->type() , "Particle Type");
    dg::fillh( "eta" , 100 , -3.0 , 3.0 , particle->pt() > 0. ? particle->eta() : 9999999., "#eta" );
    dg::fillh( "abseta" , 100 , 0 , 3.0 , particle->pt() > 0. ? std::abs( particle->eta() ) : 9999999. , "|#eta|" );
    dg::fillh( "phi" , 100 , -M_PI , M_PI , particle->phi() , "#phi (rad)" );
    dg::fillh( "pt" , 200 , 0 , 200 , particle->pt() , "p_{T} (GeV/c)" );
    dg::fillh( "invpt" , 200 , 5e-3 , 5e-2 , 1.0/particle->pt(), "1/p_{T} (GeV/c)^{-1}" );
    dg::fillh( "p" , 200 , 0 , 200 , particle->p() , "p (GeV/c)" );
    dg::fillh( "invp" , 200 , 5e-3 , 5e-2 , 1.0/particle->p() , "1/p (GeV/c)^{-1}" );
    dg::fillh( "m" , 200 , 0 , 200 , particle->m() , "Mass (GeV/c^{2})" );
    dg::fillh( "m_zregion" , 80 , 81. , 101. , particle->m() , "Mass (GeV/c^{2})" );
    dg::fillh( "m_upsilonregion" , 100 , 5. , 15. , particle->m() , "Mass (GeV/c^{2})" );
    dg::fillh( "m_jpsiregion" , 40 , 2. , 4. , particle->m() , "Mass (GeV/c^{2})" );
    if( particle->type()==AnaParticle::WBOSON ) { 
      dg::fillh( "mt" , 200 , 0 , 200 , particle->mt_self() , "Four-vector Transverse Mass (GeV/c^{2})" );
      dg::fillh( "mt_composite" , 200 , 0 , 200 , particle->transverse_mass() , "W M_{T} (GeV/c^{2})" ); 
    }
  }

protected:
  virtual void _study( type const& particle ) { // the second const is important---matches the base class.
    using namespace DataGraphics;
    StudyParticle::study(particle);
    return;
  } // end study of each particle

protected:

public:
  StudyParticle( const std::string& name , const std::string& description ) 
    : DataGraphics::DgStudy<type>(name,description) {}
  virtual ~StudyParticle() {}
};

#endif // WRAP_STUDYPARTICLE_HH

