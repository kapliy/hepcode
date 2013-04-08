#ifndef WRAP_STUDYTRUTHPARTICLE_HH
#define WRAP_STUDYTRUTHPARTICLE_HH


#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/assign/list_of.hpp>
#include "DataGraphics/DgStudy.hh"
#include "DataGraphics/DgCollection.hh"
#include "DataGraphics/DgBin.hh"
#include "TrigFTKAna/AnaTruthParticle.hpp"
#include "TrigFTKAna/Detector.hpp"
#include "TrigFTKAna/StudyParticle.hpp"

class
StudyTruthParticle : public DataGraphics::DgStudy< boost::shared_ptr<const AnaTruthParticle> >
{
public:
  typedef boost::shared_ptr<const AnaTruthParticle>  type; // must be same type as the template argument for DgStudy.

  static void study( type const& particle ) {
    using namespace DataGraphics;
    dg::fillh( "charge" , 3 , -1 , 2 , particle->charge() );
    dg::fillh( "pdg" , 100 , -50 , 50 , particle->pdg_id() );
    dg::fillh( "parent_pdg" , 101 , -50 , 51 , particle->parent_pdg_id() );
    dg::fillh( "eta_full" , 100 , -6.0 , 6.0 , particle->eta() );
    if( particle->is_w_boson() ) {
      dg::fillh( "mt_truth_self" , 200 , 0 , 200 , particle->mt_self() , "Four-vector Transverse Mass (GeV/c^{2})" );
    }
  }

protected:
  virtual void _study( type const& particle ) { // the second const is important---matches the base class.
    using namespace DataGraphics;
    StudyParticle::study(particle);
    StudyTruthParticle::study(particle);
    return;
  } // end study of each particle

public:
  StudyTruthParticle( const std::string& name , const std::string& description ) 
    : DataGraphics::DgStudy<type>(name,description) {}
  virtual ~StudyTruthParticle() {}
};

#endif // WRAP_STUDYTRUTHPARTICLE_HH
