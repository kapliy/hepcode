 
#ifndef STUDYW_HPP
#define STUDYW_HPP

#include <cmath>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/format.hpp>
#include <boost/scope_exit.hpp>
#include <boost/assign/list_of.hpp>
#include "DataGraphics/DgStudy.hh"
#include "DataGraphics/DgCollection.hh"
#include "TrigFTKAna/AnaParticle.hpp"
#include "TrigFTKAna/AnaMET.hpp"
#include "TrigFTKAna/AnaW.hpp"
#include "TrigFTKAna/AnaMuon.hpp"
#include "TrigFTKAna/AnaElectron.hpp"
#include "TrigFTKAna/Detector.hpp"
#include "TrigFTKAna/StudyParticle.hpp"
#include "TrigFTKAna/StudyMET.hpp"
#include "TrigFTKAna/StudyElectron.hpp"
#include "TrigFTKAna/StudyMuon.hpp"


class
StudyW : public DataGraphics::DgStudy< boost::shared_ptr<const AnaW> >
{
public:
  typedef boost::shared_ptr<const AnaW> type; // must be same type as the template argument for DgStudy.
protected:
  virtual void _study( type const& w ) { // the second const is important---matches the base class.
    using namespace std;
    using namespace boost;
    using namespace DataGraphics;
    const boost::shared_ptr<const AnaComposite > cop( boost::dynamic_pointer_cast<const AnaComposite>(w) ); assert( cop );
    const boost::shared_ptr<const AnaParticle>& lepton = w->lepton();
    const boost::shared_ptr<const AnaMuon> muon = boost::dynamic_pointer_cast<const AnaMuon,const AnaParticle>(lepton);
    const boost::shared_ptr<const AnaElectron> electron = boost::dynamic_pointer_cast<const AnaElectron,const AnaParticle>(lepton);
    //const boost::shared_ptr<const AnaPhoton> photon = boost::dynamic_pointer_cast<const AnaPhoton,const AnaParticle>(lepton);
    const boost::shared_ptr<const AnaMET>& met = w->met();
    assert( lepton );
    assert( met );
    StudyComposite::study( cop );
    if( true ) { 
      dg::down( "met" , "met" ); BOOST_SCOPE_EXIT(void) { dg::up(); } BOOST_SCOPE_EXIT_END;
      dg::fillh( "met" , 100 , 0 , 200 , met->pt() , "Missing ET (GeV)" );
      dg::fillh( "met_phi", 64 , 0 , M_PI , fabs( met->phi() ) , "MET #phi (rad)" );
      dg::fillh( "met_eta", 60 , -3 , 3 , met->eta() , "MET #eta" );
      dg::fillh( "wmt_vs_met" , 200,0,200,200,0,200,met->pt(),w->transverse_mass(),"MET", "W Transverse Mass (GeV/c^{2})" );
      StudyMET::study( met );
      // relative MET.
      const AnaMET::ftype met_rel = met->pt() * std::sin( lepton->dist_phi( met ) );
      dg::fillh( "met_rel" , 100 , 0 , 100 , met_rel , "Lepton Missing ET Projection (GeV)" );
   }
    if( true ) { 
      dg::down( "lepton" , "lepton" ); BOOST_SCOPE_EXIT(void) { dg::up(); } BOOST_SCOPE_EXIT_END;
      StudyParticle::study( lepton );
    }
    // make plots
    dg::fillh( "w_mt" , 200 , 0 , 200 , w->transverse_mass() , "W Transverse Mass (GeV/c^{2})" );
    dg::fillh( "mt_self" , 200 , 0 , 200 , w->mt_self() , "W M_{T} (GeV/c^{2})" );
    dg::fillh( "w_pt" , 200 , 0 , 200 , w->pt() , "W Transverse Momentum (GeV/c)" );
    dg::fillh( "lepton_phi_vs_eta" , 100 , -3.0 , 3.0 , 100, -M_PI, M_PI, lepton->eta() , lepton->phi(), "W Lepton #eta", "W Lepton #phi");
    // make lepton-specific plots
    if( electron ) { dg::down( "electron" , "w electron" ); BOOST_SCOPE_EXIT(void) { dg::up(); } BOOST_SCOPE_EXIT_END; StudyElectron::study( electron ); }
    if( muon ) { dg::down( "muon" , "w muon" ); BOOST_SCOPE_EXIT(void) { dg::up(); } BOOST_SCOPE_EXIT_END;  StudyMuon::study( muon ); }
  } // end study W
protected:
public:
  StudyW( const std::string& name , const std::string& description ) 
    : DataGraphics::DgStudy<type>(name,description)
  {
  }
  virtual ~StudyW() {}
};

#endif // STUDYW_HPP
