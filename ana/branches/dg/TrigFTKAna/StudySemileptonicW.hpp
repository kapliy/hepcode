 
#ifndef STUDYSEMILEPTONICW_HPP
#define STUDYSEMILEPTONICW_HPP

#include <string>
#include <boost/shared_ptr.hpp>
#include "DataGraphics/DgStudy.hh"
#include "DataGraphics/DgCollection.hh"
#include "TrigFTKAna/AnaParticle.hpp"
#include "TrigFTKAna/AnaMET.hpp"
#include "TrigFTKAna/AnaW.hpp"
#include "TrigFTKAna/Detector.hpp"

class
StudySemileptonicW : public DataGraphics::DgStudy< boost::shared_ptr<const AnaW> >
{
public:
  typedef boost::shared_ptr<const AnaW> type;
protected:
  virtual void _study( type const& w ) {
    using namespace std;
    using namespace boost;
    using namespace DataGraphics;
    const boost::shared_ptr<const AnaParticle>& lepton = w->lepton();
    const boost::shared_ptr<const AnaMET>& met = w->met();
    dg::filleff( "sanity_has_lep_and_met" , lepton && met );
    if( !lepton || !met ) { return; }
    dg::fillh( "mt" , 100 , 0 , 100 , w->transverse_mass() , "W Transverse Mass (GeV/c^{2})" );
    dg::fillh( "pt" , 100 , 0 , 100 , w->pt() , "W Transverse Momentum (GeV/c)" );
    dg::fillh( "met" , 100 , 0 , 100 , met->pt() , "W Missing ET (GeV)" );
    dg::fillh( "lepton_pt" , 100 , 0 , 100 , lepton->pt() , "W Lepton p_{T} (GeV/c)" ); 
    dg::fillh( "lepton_phi" , 64 , -M_PI , M_PI , lepton->phi() , "W Lepton #phi (rad)" );
    dg::fillh( "dphi_lepton_met" , 48 , -M_PI , M_PI , lepton->dist_phi( met ) , "#Delta #phi Lepton - MET (rad)" );
    dg::fillh( "deta_lepton_met" , 50 , -5 , 5 , lepton->dist_eta( met ) , "#Delta #eta Lepton - MET" );
    dg::fillh( "dr_lepton_met" , 50 , -5 , 5 , lepton->dist_eta_phi( met ) , "#Delta r(#eta,#phi) Lepton - MET" );
  } // end study semileptonic W
public:
  StudySemileptonicW( const std::string& name , const std::string& description ) 
    : DataGraphics::DgStudy<type>(name,description)
  {}
  virtual ~StudySemileptonicW() {}

};

#endif // STUDYSEMILEPTONICW_HPP
