 
#ifndef STUDYGGHIGGS_HPP
#define STUDYGGHIGGS_HPP

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
#include "TrigFTKAna/AnaHWWCandidate.hpp"
#include "TrigFTKAna/AnaParticle.hpp"
#include "TrigFTKAna/AnaMET.hpp"
#include "TrigFTKAna/AnaW.hpp"
#include "TrigFTKAna/AnaMuon.hpp"
#include "TrigFTKAna/AnaElectron.hpp"
#include "TrigFTKAna/StudyComposite.hpp"
#include "TrigFTKAna/Detector.hpp"

class
StudyGGHiggs : public DataGraphics::DgStudy< boost::shared_ptr<const AnaHWWCandidate> >
{
public:
  typedef boost::shared_ptr<const AnaHWWCandidate> type; // must be same type as the template argument for DgStudy.
protected:
  virtual void _study( type const& hww ) { // the second const is important---matches the base class.
    using namespace std;
    using namespace boost;
    using namespace DataGraphics;
    shared_ptr< const AnaComposite > cop( boost::dynamic_pointer_cast<const AnaComposite>(hww) ); assert( cop );
    StudyComposite::study( cop );
    {
      dg::down( "wlnu" , "w->l+MET studies" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      shared_ptr< const AnaComposite > cop_wlnu( dynamic_pointer_cast<const AnaComposite>(hww->wln()) );
      StudyComposite::study( cop_wlnu );
    }
    {
      dg::down( "wjj" , "w->jj studies" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      shared_ptr< const AnaComposite > cop_wjj( dynamic_pointer_cast<const AnaComposite>(hww->wjj()) );
      StudyComposite::study( cop_wjj );
    }
    if( hww->event_njets() >=4 ) { 
      dg::down( "tagjj" , "tag jj studies" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      shared_ptr< const AnaComposite > cop_tagjj( dynamic_pointer_cast<const AnaComposite>(hww->tagjjcand()) );
      StudyComposite::study( cop_tagjj );
    }
    //
    const boost::shared_ptr<const AnaW>& wlnu = hww->wln();
    const boost::shared_ptr<const AnaW>& wjj = hww->wjj();
    const boost::shared_ptr<const AnaParticle>& lepton = wlnu->lepton();
    const boost::shared_ptr<const AnaMuon> muon = boost::dynamic_pointer_cast<const AnaMuon,const AnaParticle>(lepton);
    const boost::shared_ptr<const AnaElectron> electron = boost::dynamic_pointer_cast<const AnaElectron,const AnaParticle>(lepton);
    const boost::shared_ptr<const AnaMET>& met = wlnu->met();
    
    
    const AnaParticle::ftype jet_sumpt =  std::accumulate( hww->begin_event_jets() , hww->end_event_jets() , 0. , 
                                                           lambda::bind(&AnaJet::pt,lambda::bind(&shared_ptr<const AnaJet>::get,lambda::_2)) + lambda::_1 );
    const AnaParticle::ftype jet_sumpx =  std::accumulate( hww->begin_event_jets() , hww->end_event_jets() , 0. , 
                                                           lambda::bind(&AnaJet::px,lambda::bind(&shared_ptr<const AnaJet>::get,lambda::_2)) + lambda::_1 );
    const AnaParticle::ftype jet_sumpy =  std::accumulate( hww->begin_event_jets() , hww->end_event_jets() , 0. , 
                                                           lambda::bind(&AnaJet::py,lambda::bind(&shared_ptr<const AnaJet>::get,lambda::_2)) + lambda::_1 );
    const AnaParticle::ftype event_sumpx = wlnu->px() + jet_sumpx;
    const AnaParticle::ftype event_sumpy = wlnu->py() + jet_sumpy;
    const AnaParticle::ftype event_vecsumpt = detector::quadrature( event_sumpx , event_sumpy );
    const AnaParticle::ftype event_vecsumphi = std::atan2( event_sumpy , event_sumpx );
    // jet ordering
    vector< boost::shared_ptr<const AnaJet> > jets_by_pt( hww->begin_event_jets() , hww->end_event_jets() );
    std::sort( jets_by_pt.begin() , jets_by_pt.end() , bind(&AnaJet::pt,_1) > bind(&AnaJet::pt,_2) );
    // make plots
    dg::fillh( "njets" , 10 , 0 , 10 , hww->event_njets() , "Number of Jets" );
    dg::fillh( "nvertices" , 10 , 0 , 10 , hww->event_nvertices() , "Number of Good Vertices" );
    dg::fillh( "jet_sumpt" , 100 , 0 , 100 , jet_sumpt , "Jet #Sigma p_{T} (GeV)" );
    dg::fillh( "balance_vecsumpt" , 100 , 0 , 100 , event_vecsumpt , "Event Vector #Sigma p_{T} Magnitude (GeV)" );
    if( true ) { 
      if( met ) {
        dg::fillh( "met" , 100 , 0 , 200 , met->pt() , "Missing ET (GeV)" );
        dg::fillh( "met_phi", 64 , 0 , M_PI , fabs( met->phi() ) , "MET #phi (rad)" );
        dg::fillh( "met_eta", 60 , -3 , 3 , met->eta() , "MET #eta" );
        dg::fillh( "wlnumt_vs_met" , 100 , 0 , 200 , 100 , 0 , 200 , met->pt(), wlnu->transverse_mass() , "MET (GeV)" , "W->l#nu Transverse Mass (GeV/c^{2})" );
      } // end met plots
      if( lepton ) {
        dg::fillh( "lepton_pt" , 100 , 0 , 200 , lepton->pt() , "W Lepton p_{T} (GeV/c)" );
        dg::fillh( "lepton_invpt" , 100 , 5e-3 , 5e-5 , 1./lepton->pt() , "W Lepton 1/p_{T} (GeV^{-1})" ); 
        dg::fillh( "lepton_phi" , 100 , -M_PI , M_PI , lepton->phi() , "W Lepton #phi (rad)" );
        dg::fillh( "lepton_eta" , 100 , -3.0 , 3.0 , lepton->eta() , "W Lepton #eta" );
        dg::fillh( "lepton_phi_vs_eta" , 100 , -3.0 , 3.0 , 100, -M_PI, M_PI, lepton->eta() , lepton->phi(), "W Lepton #eta", "W Lepton #phi");
        if( met ) {
          dg::fillh( "lepton_pt_vs_met" , 100 , 0, 200, 100, 0 , 200 , met->pt(), lepton->pt() , "MET", "W Lepton #pt" );
          dg::fillh( "dphi_lepton_met" , 100 , -M_PI , M_PI , lepton->dist_phi( met ) , "#Delta #phi Lepton - MET (rad)" );
          dg::fillh( "deta_lepton_met" , 100 , -5 , 5 , lepton->dist_eta( met ) , "#Delta #eta Lepton - MET" );
          dg::fillh( "dr_lepton_met" , 100 , -5 , 5 , lepton->dist_eta_phi( met ) , "#Delta r(#eta,#phi) Lepton - MET" );
          dg::fillh( "balance_vecsumphi_lepton" , 100 , -M_PI , M_PI , detector::delta_phi(event_vecsumphi,lepton->phi()) , "Event Vector #Sigma p_{T} #phi - Lepton #phi (rad)" );
          dg::fillh( "balance_vecsumphi_met" , 100 , -M_PI , M_PI , detector::delta_phi(event_vecsumphi,met->phi()) , "Event Vector #Sigma p_{T} #phi - MET #phi (rad)" );
          dg::fillh( "met_vs_dphi_lepton_met", 48, -M_PI, M_PI, 100, 0 , 200 , lepton->dist_phi( met ),
                     met->pt() , "#Delta #phi Lepton - MET (rad)" , "MET [GeV]" );
        }
      } // end lepton-met plots
    } // end lepton plots
    if( true ) { 
      // Get delta-R between lepton and nearest jet
      float min_dR = std::numeric_limits<float>::max();
      dg::down( "jet_pts" , "ith-highest pt jet(s)" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      // study ith-highest pt jet(s)
      unsigned int ithjet = 0u;
      for( vector< boost::shared_ptr<const AnaJet> >::iterator ijet=jets_by_pt.begin(), fjet=jets_by_pt.end(); ijet!=fjet; ++ijet, ++ithjet ) {
        const boost::shared_ptr<const AnaJet>& j( *ijet );
        dg::fillh( (format("jet_%|d|thpt_pt") % ithjet ).str() , 100 , 0 , 200 , j->pt() );
        dg::fillh( (format("jet_%|d|thpt_phi") % ithjet ).str() , 100 , 0 , M_PI , fabs( j->phi() ) );
        dg::fillh( (format("jet_%|d|thpt_eta") % ithjet ).str() , 100 , -3 , 3 , j->eta() );
        if( ithjet==0 ) { 
          dg::fillh( "balance_vecsumphi_leading_jet" , 100 , -M_PI , M_PI , 
                     detector::delta_phi(event_vecsumphi,j->phi()) , "Event Vector #Sigma p_{T} #phi - Leading Jet #phi (rad)" ); 
        }
        float tmp_dR = lepton->dist_eta_phi( j );
        if( tmp_dR < min_dR ) { min_dR = tmp_dR; }
      }
      dg::fillh( "dr_lepton_jet" , 100 , -5 , 5 , min_dR , "#Delta r(#eta,#phi) Lepton - Nearest Jet" );
    }
    if( true && met && lepton ) { 
      // Get delta-phi between met and nearest jet/lepton
      float min_dPhi = std::numeric_limits<float>::max();
      // study ith-highest pt jet(s)
      unsigned int ithjet = 0u;
      for( vector< boost::shared_ptr<const AnaJet> >::iterator ijet=jets_by_pt.begin(), fjet=jets_by_pt.end(); ijet!=fjet; ++ijet, ++ithjet ) {
        const boost::shared_ptr<const AnaJet>& j( *ijet );
	float tmp_dPhi = fabs(met->dist_phi( j ));
	if( tmp_dPhi < min_dPhi ) { min_dPhi = tmp_dPhi; }
      }
      float tmp_dPhi = fabs(met->dist_phi( lepton ));
      if( tmp_dPhi < min_dPhi ) { min_dPhi = tmp_dPhi; }
      dg::fillh( "dphi_met_leptonjet" , 100 , 0 , M_PI , min_dPhi , "#Delta #phi MET - Nearest Lepton/Jet" );
      float metrel=met->pt();
      if (min_dPhi < M_PI/2) { metrel = met->pt() * sin(min_dPhi); }
      dg::fillh( "met_rel" , 200 , 0 , 200 , metrel , "W Missing ET Projection(GeV)" );
    }
    
    if( hww->event_njets()>1 && lepton && met ) {
      dg::fillh( "wjj_mt" , 500 , 0 , 500 ,  wjj->mt() , "Transverse Mass of 2 W-Jets (GeV/c^{2})" );
      dg::fillh( "wjj_m" , 500 , 0 , 500 , wjj->m() , "Invariant Mass of 2 W-Jets (GeV/c^{2})" );
      dg::fillh( "wlnu_mt" , 200 , 0 , 200 ,  wlnu->mt() , "Transverse Mass of W->lnu (GeV/c^{2})" );
      dg::fillh( "wlnu_m" , 200 , 0 , 200 , hww->wln_m() , "Invariant Mass of W->lnu (GeV/c^{2})" );
      dg::fillh( "higgs_mt" , 1000 , 0 , 1000 ,  hww->higgs_mt() , "Transverse Mass of Higgs (GeV/c^{2})" );
      dg::fillh( "higgs_m" , 1000 , 0 , 1000 , hww->higgs_m() , "Invariant Mass of Higgs (GeV/c^{2})" );
      if( hww->event_njets()>3){
	dg::fillh( "tagjj_m" , 1000 , 0 , 1000 , hww->tagjj_m() , "Invariant Mass of 2 tag-jets (GeV/c^{2})" );
      }
    }

    if( false ) { 
      dg::down( "jet_loweta" , "ith-lowest eta jet(s)" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      // study ith-lowest eta jet(s)
      vector< boost::shared_ptr<const AnaJet> > jets_by_eta( hww->begin_event_jets() , hww->end_event_jets() );
      std::sort( jets_by_eta.begin() , jets_by_eta.end() , 
                 bind( ptr_fun<AnaJet::ftype,AnaJet::ftype>(abs) , bind(&AnaJet::eta,_1) ) <
                 bind( ptr_fun<AnaJet::ftype,AnaJet::ftype>(abs) , bind(&AnaJet::eta,_2) ) );
      unsigned int ithjet = 0u;
      for( vector< boost::shared_ptr<const AnaJet> >::iterator ijet=jets_by_eta.begin(), fjet=jets_by_eta.end(); ijet!=fjet; ++ijet, ++ithjet ) {
        const boost::shared_ptr<const AnaJet>& j( *ijet );
        dg::fillh( (format("jet_%|d|theta_pt") % ithjet ).str() , 100 , 0 , 200 , j->pt() );
        dg::fillh( (format("jet_%|d|theta_eta") % ithjet ).str() , 100 , -3 , 3 , j->eta() );
      }
    }
  } // end study W cross section event candidates
public:
  StudyGGHiggs( const std::string& name , const std::string& description ) 
    : DataGraphics::DgStudy<type>(name,description)
  {}
  virtual ~StudyGGHiggs() {}
};

#endif // STUDYGGHIGGS_HPP
