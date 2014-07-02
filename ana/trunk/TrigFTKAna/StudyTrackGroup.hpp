#ifndef WRAP_STUDYTRACKGROUP_HH
#define WRAP_STUDYTRACKGROUP_HH

// plots of track groups, e.g. two tracks from a 

#include <string>
#include <vector>
#include <utility>
#include <boost/shared_ptr.hpp>
#include <boost/scope_exit.hpp>
#include "DataGraphics/DgStudy.hh"
#include "DataGraphics/DgCollection.hh"
#include "TrigFTKAna/AnaTrack.hpp"
#include "TrigFTKAna/Detector.hpp"
#include "TLorentzVector.h"

class
StudyTrackGroup : public DataGraphics::DgStudy< std::vector< boost::shared_ptr<const AnaTrack> > >
{
public:
  typedef std::vector< boost::shared_ptr<const AnaTrack> > type; // must be same type as the template argument for DgStudy.
protected:
  virtual void _study( type const& coll ) {
    if( coll.size()==2 ) { _study_pair(coll); }
  }
  void _study_pair( type const& coll ) {
    using namespace std;
    using namespace boost;
    using namespace DataGraphics;
    using namespace detector;
    dg::down( "pairs" , "two track" ); BOOST_SCOPE_EXIT(void) { dg::up(); } BOOST_SCOPE_EXIT_END;
    assert( coll.size()>=2 );
    const boost::shared_ptr<const AnaTrack>& trkA( coll[0] );
    const boost::shared_ptr<const AnaTrack>& trkB( coll[1] );
    const AnaTrack::ftype charge_product( trkA->charge()*trkB->charge() );
    dg::fillh( "charge" , 3 , -1 , 2 , charge_product );
    if( charge_product <= 0. ) { return; }
    TLorentzVector pA( trkA->four_momentum() );
    TLorentzVector pB( trkB->four_momentum() );
    TLorentzVector q( pA+pB );
    AnaTrack::ftype massA = detector::proton_mass(detector::GeV);
    AnaTrack::ftype massB = detector::pion_mass(detector::GeV);
    // lambda hypothesis. assume highest momentum particle is the proton
    if( pA.P() < pB.P() ) { std::swap( massA , massB ); }
    const AnaTrack::ftype m_Ppi( detector::two_track_mass( pA , pB , massA , massB ) );
    // conversion hypothesis
    const AnaTrack::ftype m_ee( detector::two_track_mass( pA , pB , detector::electron_mass(detector::GeV) , detector::electron_mass(detector::GeV) ) );
    //
    dg::fillh( "Mpipi" , 100 , 0 , 1 , q.M() );
    dg::fillh( "MPpi" , 100 , 0 , 10 , m_Ppi );
    dg::fillh( "Mee" , 100 , 0 , 1 , m_ee );
    dg::fillh( "Mpipi_K0" , 22 , detector::K0_mass(detector::GeV)-0.044 , detector::K0_mass(detector::GeV)+0.044 , q.M() );
    dg::fillh( "Mee_gamma" , 20 , 0 , 0.1 , m_ee );
    dg::fillh( "MPpi_lambda" , 64 , detector::lambda_mass(detector::GeV)-0.016 , detector::lambda_mass(detector::GeV)+0.016 , m_Ppi );
  }
public:
  StudyTrackGroup( const std::string& name , const std::string& description ) 
    : DataGraphics::DgStudy<type>(name,description)
  {}
  virtual ~StudyTrackGroup() {}
};

#endif // WRAP_STUDYTRACKGROUP_HH

