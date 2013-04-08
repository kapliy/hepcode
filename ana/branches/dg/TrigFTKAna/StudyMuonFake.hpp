#ifndef WRAP_STUDYMUONFAKE_HH
#define WRAP_STUDYMUONFAKE_HH


#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/any.hpp>
#include <boost/function.hpp>
#include "DataGraphics/DgStudy.hh"
#include "DataGraphics/DgCollection.hh"
#include "TrigFTKAna/AnaMuon.hpp"
#include "TrigFTKAna/Detector.hpp"
#include "TrigFTKAna/StudyParticle.hpp"
#include "TrigFTKAna/StudyTrack.hpp"
#include "TrigFTKAna/StudyMuon.hpp"

class
StudyMuonFake : public DataGraphics::DgStudy< boost::shared_ptr<const AnaMuon> >
{
public:
  typedef boost::shared_ptr<const AnaMuon>  type; // must be same type as the template argument for DgStudy.
  typedef boost::function< bool ( const boost::shared_ptr<const AnaMuon>& ) > Classifier;
  typedef boost::function< bool ( const boost::shared_ptr<const AnaMuon>& ) > FakeablePredicate;
  //
  void study( type const& mu ) {
    using namespace DataGraphics;
    assert( mu->type()==AnaParticle::MUON );
    assert( !_classifier_copy.empty() && "classifier must be set (why study fake rate for no cut?)" );
    //
    // does muon pass the cut?
    const bool passes_classifier = _classifier(mu);
    // if this object does not passes the 'fakeable predicate',
    // exclude it from the denominator of the fake rate calculation.
    if( !_fakeable_predicate_copy.empty() ) {
      const bool is_fakeable = _fakeable_predicate(mu);
      if( _study_fakeable ) {
        dg::filleff( "is_fakeable" , is_fakeable );
        dg::filleff( "fake_all" , passes_classifier );
        if( !is_fakeable ) { dg::filleff( "fake_notfakeable" , passes_classifier ); }
      }
      if( !is_fakeable ) { return; }
    }
    // study cut efficiency vs kinematics
    if( _study_fakeable ) {
      StudyMuon st("fakeable","fakeable denominator candidates");
      st.for_only( mu );
    }
    if( _study_numerator && passes_classifier ) {
      StudyMuon st("numerator","numerator muon candidates");
      st.for_only( mu );
    }
    // fake rate vs kinematic variables
    dg::filleff( "fake_vs_pt" , 50 , 0 , 100 , mu->pt() , passes_classifier , "muon p_{T} (GeV)" );
    dg::filleff( "fake_vs_eta" , 30 , -3 , 3 , mu->eta() , passes_classifier , "muon #eta" );
    if( mu->charge() > 0 ) { dg::filleff( "fake_vs_pt_qp" , 50 , 0 , 100 , mu->pt() , passes_classifier , "muon p_{T} (GeV)" ); }
    else if( mu->charge() < 0 ) { dg::filleff( "fake_vs_pt_qn" , 50 , 0 , 100 , mu->pt() , passes_classifier , "muon p_{T} (GeV)" ); }
    if( mu->abseta() < 1. ) { 
      dg::filleff( "fake_vs_pt_etaLt1" , 50 , 0 , 100 , mu->pt() , passes_classifier , "muon p_{T} (GeV)" );
    } else if( mu->abseta() < 2. ) {
      dg::filleff( "fake_vs_pt_etaGt1Lt2" , 50 , 0 , 100 , mu->pt() , passes_classifier , "muon p_{T} (GeV)" );
    } else if( mu->abseta() < 2.5 ) {
      dg::filleff( "fake_vs_pt_etaGt2Lt2p5" , 50 , 0 , 100 , mu->pt() , passes_classifier , "muon p_{T} (GeV)" );
    }
    // study fake rate vs other variables? (looking for extra background suppression)
    //   (none implemented yet)
  }

protected:
  virtual void _study( type const& muon ) { // the second const is important---matches the base class.
    using namespace DataGraphics;
    StudyMuonFake::study(muon);
  } // end study of each muon
protected:
  // cut to study (bind object etc)
  Classifier _classifier;
  boost::any _classifier_copy;
  FakeablePredicate _fakeable_predicate;
  boost::any _fakeable_predicate_copy;
  // extra studies
  bool _study_fakeable;
  bool _study_numerator;
public:
  StudyMuonFake( const std::string& name , const std::string& description ) 
    : DataGraphics::DgStudy<type>(name,description)
    , _classifier()
    , _classifier_copy()
    , _fakeable_predicate()
    , _fakeable_predicate_copy()
    , _study_fakeable( false )
    , _study_numerator( false )
  {}
  virtual ~StudyMuonFake() {}
  template<typename bindT>
  void set_cut( bindT const& predicate ) {
    _classifier_copy = predicate;
    _classifier = Classifier(boost::any_cast<bindT>(_classifier_copy));
  }
  template<typename bindT>
  void set_fakeable_cut( bindT const& predicate ) {
    _fakeable_predicate_copy = predicate;
    _fakeable_predicate = FakeablePredicate(boost::any_cast<bindT>(_fakeable_predicate_copy));
  }
  void study_fakeable( const bool yes = true ) { _study_fakeable = yes; }
  void study_numerator( const bool yes = true ) { _study_numerator = yes; }
};

#endif // WRAP_STUDYMUONFAKE_HH

