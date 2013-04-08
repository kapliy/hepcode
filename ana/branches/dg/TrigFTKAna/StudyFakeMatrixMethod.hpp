#ifndef WRAP_STUDYFAKEMATRIXMETHOD_HH
#define WRAP_STUDYFAKEMATRIXMETHOD_HH


#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/any.hpp>
#include <boost/function.hpp>
#include "DataGraphics/DgStudy.hh"
#include "DataGraphics/DgCollection.hh"
#include "TrigFTKAna/AnaComposite.hpp"
#include "TrigFTKAna/AnaParticle.hpp"
#include "TrigFTKAna/Detector.hpp"
#include "TrigFTKAna/StudyParticle.hpp"

template<class legAT,class legBT>
class
StudyFakeMatrixMethod : public DataGraphics::DgStudy< boost::shared_ptr<const AnaComposite> >
{
public:
  typedef boost::shared_ptr<const AnaComposite>  type; // must be same type as the template argument for DgStudy.
  typedef boost::function< bool ( const boost::shared_ptr<legAT>& ) > LegClassifierA;
  typedef boost::function< bool ( const boost::shared_ptr<legBT>& ) > LegClassifierB;
  //
  void study( type const& cop ) {
    using namespace DataGraphics;
    assert( !_leg_classifierA_copy.empty() && "classifier A must be set (why study matrix method for no cut?)" );
    assert( !_leg_classifierB_copy.empty() && "classifier B must be set (why study matrix method for no cut?)" );
    // retrieve legs.
    if( !cop || cop->num_constituents()!=2 ) { return; }
    boost::shared_ptr<const AnaParticle> leg0( cop->ith_constituent(0) );
    boost::shared_ptr<const AnaParticle> leg1( cop->ith_constituent(1) );
    // assign leading and subleading legs. if leg A is the same flavor
    // as leg B, order by pT. otherwise, call leg of type A the
    // leading leg and leg of type B the subleading leg.
    if( leg0->type() == leg1->type() ) {
      if( leg0->pt() < leg1->pt() ) {
        std::swap( leg0 , leg1 );
      }
    }
    boost::shared_ptr<const legAT> legA( boost::dynamic_pointer_cast<const legAT>(leg0) );
    boost::shared_ptr<const legBT> legB( boost::dynamic_pointer_cast<const legBT>(leg1) );
    // if the following assert, the most likely problem is that leg0
    // doesn't have type "legAT", or leg1 doesn't have type "legBT".
    assert( legA );
    assert( legB );
    // is leg A tight?
    const bool tightA = _leg_classifierA(legA);
    // is leg B tight?
    const bool tightB = _leg_classifierB(legB);
    // histogram event counts.
    if( tightA && tightB ) { dg::fillh( "nTT" , 1 , 0 , 1 , 0.5 ); }
    if( tightA && !tightB ) { dg::fillh( "nTL" , 1 , 0 , 1 , 0.5 ); }
    if( !tightA && tightB ) { dg::fillh( "nLT" , 1 , 0 , 1 , 0.5 ); }
    if( !tightA && !tightB ) { dg::fillh( "nLL" , 1 , 0 , 1 , 0.5 ); }
    dg::fillh( "nAB" , 2 , 0 , 2 , 2 , 0 , 2 , tightA , tightB , "A is tight" , "B is tight" );
    StudyParticle stp( "two_body_kin" , "two body kinematics" );
    stp.add_constant_category( "isTT" , tightA && tightB );
    stp.add_constant_category( "isTL" , tightA && !tightB );
    stp.add_constant_category( "isLT" , !tightA && tightB );
    stp.add_constant_category( "isLL" , !tightA && !tightB );
    stp.for_only( cop );
  } // end study
protected:
  virtual void _study( type const& cop ) { // the second const is important---matches the base class.
    using namespace DataGraphics;
    StudyFakeMatrixMethod::study(cop);
  } // end study of each muon
protected:
  // cut to study (bind object etc)
  LegClassifierA _leg_classifierA;
  boost::any _leg_classifierA_copy;
  LegClassifierB _leg_classifierB;
  boost::any _leg_classifierB_copy;
public:
  StudyFakeMatrixMethod( const std::string& name , const std::string& description ) 
    : DataGraphics::DgStudy<type>(name,description)
    , _leg_classifierA()
    , _leg_classifierA_copy()
    , _leg_classifierB()
    , _leg_classifierB_copy()
  {}
  virtual ~StudyFakeMatrixMethod() {}

  template<typename bindT>
  void set_cut_legA( bindT const& predicate ) {
    _leg_classifierA_copy = predicate;
    _leg_classifierA = LegClassifierA(boost::any_cast<bindT>(_leg_classifierA_copy));
  }
  template<typename bindT>
  void set_cut_legB( bindT const& predicate ) {
    _leg_classifierB_copy = predicate;
    _leg_classifierB = LegClassifierB(boost::any_cast<bindT>(_leg_classifierB_copy));
  }
};

#endif // WRAP_STUDYFAKEMATRIXMETHOD_HH

