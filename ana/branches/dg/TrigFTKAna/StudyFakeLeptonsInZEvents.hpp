#ifndef WRAP_STUDYFAKELEPTONSINZEVENTS_HH
#define WRAP_STUDYFAKELEPTONSINZEVENTS_HH


#include <cmath>
#include <string>
#include <vector>
#include <algorithm>
#include <boost/shared_ptr.hpp>
#include "DataGraphics/DgStudy.hh"
#include "DataGraphics/DgCollection.hh"
#include "TrigFTKAna/AnaParticle.hpp"
#include "TrigFTKAna/Detector.hpp"

class AnaComposite;

class
StudyFakeLeptonsInZEvents : public DataGraphics::DgStudy< boost::shared_ptr<const AnaComposite> >
{
public:
  typedef boost::shared_ptr<const AnaComposite>  type; // must be same type as the template argument for DgStudy.
  void study( type const& particle );
protected:
  virtual void _study( type const& particle ) { // the second const is important---matches the base class.
    StudyFakeLeptonsInZEvents::study(particle);
    return;
  } // end study of each particle
private:
  std::vector< boost::shared_ptr<const AnaParticle> > _leptons;
  std::vector< boost::shared_ptr<const AnaParticle> > _denominators;
public:
  StudyFakeLeptonsInZEvents( const std::string& name , const std::string& description ) 
    : DataGraphics::DgStudy<type>(name,description)
    , _leptons()
    , _denominators()
  {}
  virtual ~StudyFakeLeptonsInZEvents() {}
  //
  template< class iteratorT >
  void set_leptons( iteratorT begin , iteratorT end ) {
    _leptons.assign( begin , end );
  }
  template< class iteratorT >
  void set_denominator( iteratorT begin , iteratorT end ) {
    _denominators.assign( begin , end );
  }
};

#endif // WRAP_STUDYFAKELEPTONSINZEVENTS_HH

