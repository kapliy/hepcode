 
#ifndef STUDYZCROSSECTION_HPP
#define STUDYZCROSSECTION_HPP

#include <cmath>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <TRandom.h>
#include <TRandom3.h>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/format.hpp>
#include <boost/scope_exit.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/assign/std/vector.hpp>
#include "DataGraphics/DgStudy.hh"
#include "DataGraphics/DgCollection.hh"
#include "TrigFTKAna/AnaZCrossSectionCandidate.hpp"
#include "TrigFTKAna/AnaParticle.hpp"
#include "TrigFTKAna/AnaMET.hpp"
#include "TrigFTKAna/AnaZ.hpp"
#include "TrigFTKAna/AnaMuon.hpp"
#include "TrigFTKAna/AnaElectron.hpp"
#include "TrigFTKAna/Detector.hpp"


class
StudyZCrossSection : public DataGraphics::DgStudy< boost::shared_ptr<const AnaZCrossSectionCandidate> >
{
public:
  typedef boost::shared_ptr<const AnaZCrossSectionCandidate> type; // must be same type as the template argument for DgStudy.
protected:
  virtual void _study( type const& zevent ) {
    using namespace std;
    using namespace boost;
    using namespace DataGraphics;
    const boost::shared_ptr<const AnaZCrossSectionCandidate>& z = zevent;
    const boost::shared_ptr<const AnaParticle>& leptonP = z->lepton_pos(); // positive lepton
    const boost::shared_ptr<const AnaParticle>& leptonN = z->lepton_neg(); // negative lepton
    const boost::shared_ptr<const AnaParticle>& lepton1 = z->lepton1(); // higher-pt lepton
    const boost::shared_ptr<const AnaParticle>& lepton2 = z->lepton2(); // lower-pt lepton
    const boost::shared_ptr<const AnaMuon> muonP = boost::dynamic_pointer_cast<const AnaMuon,const AnaParticle>(leptonP);
    const boost::shared_ptr<const AnaMuon> muonN = boost::dynamic_pointer_cast<const AnaMuon,const AnaParticle>(leptonN);
    const boost::shared_ptr<const AnaMuon> muon1 = boost::dynamic_pointer_cast<const AnaMuon,const AnaParticle>(lepton1);
    const boost::shared_ptr<const AnaMuon> muon2 = boost::dynamic_pointer_cast<const AnaMuon,const AnaParticle>(lepton2);
    const boost::shared_ptr<const AnaMET>& met = zevent->met();
    const AnaParticle::ftype jet_sumpt =  std::accumulate( zevent->begin_jets() , zevent->end_jets() , 0. , 
                                                           lambda::bind(&AnaJet::pt,lambda::bind(&shared_ptr<const AnaJet>::get,lambda::_2)) + lambda::_1 );
    const AnaParticle::ftype jet_sumpx =  std::accumulate( zevent->begin_jets() , zevent->end_jets() , 0. , 
                                                           lambda::bind(&AnaJet::px,lambda::bind(&shared_ptr<const AnaJet>::get,lambda::_2)) + lambda::_1 );
    const AnaParticle::ftype jet_sumpy =  std::accumulate( zevent->begin_jets() , zevent->end_jets() , 0. , 
                                                           lambda::bind(&AnaJet::py,lambda::bind(&shared_ptr<const AnaJet>::get,lambda::_2)) + lambda::_1 );
    const AnaParticle::ftype jet_vecsumpt = detector::quadrature( jet_sumpx , jet_sumpy );
    const AnaParticle::ftype event_sumpx = z->px() + jet_sumpx;
    const AnaParticle::ftype event_sumpy = z->py() + jet_sumpy;
    const AnaParticle::ftype event_vecsumpt = detector::quadrature( event_sumpx , event_sumpy );
    const AnaParticle::ftype event_vecsumphi = std::atan2( event_sumpy , event_sumpx );
    const AnaParticle::ftype zjets_sumpt = z->pt() + jet_sumpt;
    // jet ordering
    vector< boost::shared_ptr<const AnaJet> > jets_by_pt( zevent->begin_jets() , zevent->end_jets() );
    std::sort( jets_by_pt.begin() , jets_by_pt.end() , bind(&AnaJet::pt,_1) > bind(&AnaJet::pt,_2) );
    // make plots
    dg::fillh( "z_m" , 200 , 0 , 200 , z->mass() , "Z Mass (GeV/c^{2})" );
    dg::fillh( "z_m_fine" , 400 , 0 , 200 , z->mass() , "Z Mass (GeV/c^{2})" );
    dg::fillh( "z_m_fullrange" , 2000 , 0 , 2000 , z->mass() , "Z Mass (GeV/c^{2})" );
    dg::fillh( "z_pt" , 500 , 0 , 500 , z->pt() , "Z Momentum (GeV/c)" );
    dg::fillh( "njets" , 10 , 0 , 10 , zevent->njets() , "N_{jets}" );
    dg::fillh( "nvtxs" , 20 , 0 , 20 , zevent->nvtxs() , "N_{vtxs}" );
    dg::fillhw( "nvtxs_unweighted" , 20 , 0 , 20 , zevent->nvtxs() , 1. , "N_{vtxs}" );
    dg::fillh( "jet_sumpt" , 200 , 0 , 200 , jet_sumpt , "Jet #Sigma p_{T} (GeV)" );
    dg::fillh( "jet_vecsumpt" , 100 , 0 , 100 , jet_vecsumpt , "Jet Vector #Sigma p_{T} Magnitude (GeV)" );
    dg::fillh( "zjets_vecsumpt" , 100 , 0 , 100 , event_vecsumpt , "Event Vector #Sigma p_{T} Magnitude (GeV)" );
    dg::fillh( "zjets_sumpt" , 100 , 0 , 500 , zjets_sumpt , "Event Scaler #Sigma p_{T} (GeV)" );
    dg::fillh( "balance_vecsumpt" , 200 , 0 , 200 , event_vecsumpt , "Event Vector #Sigma p_{T} Magnitude (GeV)" );
    if( leptonP && leptonN ) {
      const AnaParticle::ftype lljets_sumpt = leptonP->pt() + leptonN->pt() + jet_sumpt;
      const AnaParticle::ftype lljets_vecsumpt = detector::quadrature( leptonP->px() + leptonN->px() + jet_sumpx ,
                                                                       leptonP->py() + leptonN->py() + jet_sumpy );
      dg::fillh( "lljets_sumpt" , 2000 , 0 , 2000 , lljets_sumpt , "Leptons and Jets Scaler #Sigma p_{T} (GeV)" );
      dg::fillh( "lljets_vecsumpt" , 2000 , 0 , 2000 , lljets_vecsumpt , "Leptons and Jets Vector #Sigma p_{T} Magnitude (GeV)" );
      if( met ) {
        const AnaParticle::ftype llmetjets_sumpt = leptonP->pt() + leptonN->pt() + met->pt() + jet_sumpt;
        const AnaParticle::ftype llmetjets_vecsumpt = detector::quadrature( leptonP->px() + leptonN->px() + met->px() + jet_sumpx ,
                                                                            leptonP->py() + leptonN->py() + met->py() + jet_sumpy );
        dg::fillh( "llmetjets_sumpt" , 2000 , 0 , 2000 , llmetjets_sumpt , "Leptons, MET, and Jets Scaler #Sigma p_{T} (GeV)" );
        dg::fillh( "llmetjets_vecsumpt" , 2000 , 0 , 2000 , llmetjets_vecsumpt , "Leptons, MET, and Jets Vector #Sigma p_{T} Magnitude (GeV)" );
      }
    }
    if( true ) {
      if( met ) {
	dg::fillh( "met" , 500 , 0 , 500 , met->pt() , "Z Missing ET (GeV)" );
	dg::fillh( "met_phi", 100 , -M_PI , M_PI , met->phi() , "MET #phi (rad)" );
	dg::fillh( "met_absphi", 100 , 0 , M_PI , fabs(met->phi()) , "MET |#phi| (rad)" );
	if( _do_2d_plots ) dg::fillh( "zm_vs_met" , 200,0,200,200,0,200,met->pt(),z->m(),"MET", "Z Mass (GeV/c^{2})" );
      } // end met plots
      if( leptonP && leptonN) {
	// mu+
	dg::fillh( "leptonP_p" , 500 , 0 , 500 , leptonP->p() , "l+ p (GeV/c)" ); 
	dg::fillh( "leptonP_invp" , 200 , 5e-3 , 5e-1 , 1.0/leptonP->p() , "l+ 1/p (GeV^{-1})" ); 
        dg::fillh( "leptonP_pt" , 500 , 0 , 500 , leptonP->pt() , "l+ p_{T} (GeV/c)" ); 
	dg::fillh( "leptonP_invpt" , 200 , 5e-3 , 5e-1 , 1.0/leptonP->pt() , "l+ 1/p_{T} (GeV^{-1})" ); 
        dg::fillh( "leptonP_phi" , 100 , -M_PI , M_PI , leptonP->phi() , "l+ #phi (rad)" );
        dg::fillh( "leptonP_absphi" , 100 , 0 , M_PI , fabs(leptonP->phi()) , "l+ |#phi| (rad)" );
	dg::fillh( "leptonP_eta" , 100 , -8 , 8 , leptonP->eta() , "l+ #eta" );
	// mu-
        dg::fillh( "leptonN_p" , 500 , 0 , 500 , leptonN->p() , "l- p (GeV/c)" );
	dg::fillh( "leptonN_invp" , 200 , 5e-3 , 5e-1 , 1.0/leptonN->p() , "l- 1/p (GeV^{-1})" ); 
	dg::fillh( "leptonN_pt" , 500 , 0 , 500 , leptonN->pt() , "l- p_{T} (GeV/c)" );
	dg::fillh( "leptonN_invpt" , 200 , 5e-3 , 5e-1 , 1.0/leptonN->pt() , "l- 1/p_{T} (GeV^{-1})" ); 
        dg::fillh( "leptonN_phi" , 100 , -M_PI , M_PI , leptonN->phi() , "l- #phi (rad)" );
        dg::fillh( "leptonN_absphi" , 100 , 0 , M_PI , fabs(leptonN->phi()) , "l+ |#phi| (rad)" );
	dg::fillh( "leptonN_eta" , 100 , -8 , 8 , leptonN->eta() , "l- #eta" );
	// joint quantities for mu+ & mu-
	dg::fillh( "dphi_leptonP_leptonN" , 100 , -M_PI , M_PI , leptonP->dist_phi( leptonN ) , "#Delta #phi between leptons (rad)" );
	dg::fillh( "deta_leptonP_leptonN" , 100 , -M_PI , M_PI , leptonP->dist_eta( leptonN ) , "#Delta #eta between leptons (rad)" );
	dg::fillh( "dr_leptonP_leptonN" , 100 , -M_PI , M_PI , leptonP->dist_eta_phi( leptonN ) , "#Delta r(#phi,#phi) between leptons (rad)" );
	if( _do_2d_plots ) {
	  dg::fillh( "leptonP_vs_leptonN_pt" , 200,0,200,200,0,200, leptonN->pt(),leptonP->pt(), "l- #pt", "l+ #pt");
	  dg::fillh( "leptonP_vs_leptonN_eta" , 100,-3.0,3.0,100,-3.0,3.0, leptonN->eta(),leptonP->eta(), "l- #eta", "l+ #eta");
	  dg::fillh( "leptonP_vs_leptonN_phi" , 100,-M_PI,M_PI,100,-M_PI,M_PI, leptonN->phi(),leptonP->phi(), "l- #phi", "l+ #phi");
	}
	// special quantities for muons
	if(muonP && muonN) {
	  dg::fillh( "leptonP_id_pt" , 200 , 0 , 200 , muonP->id_pt() , "mu+ ID p_{T} (GeV/c)" );
	  dg::fillh( "leptonP_ms_pt" , 200 , 0 , 200 , muonP->ms_pt() , "mu+ MS p_{T} (GeV/c)" );
	  dg::fillh( "leptonP_exms_pt" , 200 , 0 , 200 , muonP->exms_pt() , "mu+ ex. MS p_{T} (GeV/c)" ); 
	  dg::fillh( "leptonN_id_pt" , 200 , 0 , 200 , muonN->id_pt() , "mu- ID p_{T} (GeV/c)" );
	  dg::fillh( "leptonN_ms_pt" , 200 , 0 , 200 , muonN->ms_pt() , "mu- MS p_{T} (GeV/c)" );
	  dg::fillh( "leptonN_exms_pt" , 200 , 0 , 200 , muonN->exms_pt() , "mu- ex. MS p_{T} (GeV/c)" ); 
	}
      }
      if( lepton1 && lepton2 ) {
        // higher-pt lepton
        dg::fillh( "lepton1_p" , 500 , 0 , 500 , lepton1->p() , "hard lepton p (GeV/c)" );
        dg::fillh( "lepton1_pt" , 500 , 0 , 500 , lepton1->pt() , "hard lepton p_{T} (GeV/c)" );
        dg::fillh( "lepton1_phi" , 100 , -M_PI , M_PI , lepton1->phi() , "hard lepton #phi (rad)" );
        dg::fillh( "lepton1_absphi" , 100 , 0 , M_PI , fabs(lepton1->phi()) , "hard lepton |#phi| (rad)" );
        dg::fillh( "lepton1_eta" , 100 , -8 , 8 , lepton1->eta() , "hard lepton #eta" );
        // lower-pt lepton
        dg::fillh( "lepton2_p" , 500 , 0 , 500 , lepton2->p() , "soft lepton p (GeV/c)" );
        dg::fillh( "lepton2_pt" , 500 , 0 , 500 , lepton2->pt() , "soft lepton p_{T} (GeV/c)" );
        dg::fillh( "lepton2_phi" , 100 , -M_PI , M_PI , lepton2->phi() , "soft lepton #phi (rad)" );
        dg::fillh( "lepton2_absphi" , 100 , 0 , M_PI , fabs(lepton2->phi()) , "soft lepton |#phi| (rad)" );
        dg::fillh( "lepton2_eta" , 100 , -8 , 8 , lepton2->eta() , "soft lepton #eta" );
      } // end lepton plots
    }
    if( _do_yield ) {
      dg::fillhw( "yield" , int( _yield_max - _yield_min ) ,
		  _yield_min , _yield_max , _yield_runnumber ,
		  dg::global_weight() / _yield_lumi , "run number" );
    }
    if( _do_heavyQ_studies ) {
      dg::fillhw( "global_weight" , 1000 , 0 , 10 , dg::global_weight() , 1. );
      dg::fillh( "total" , 1 , 1 , 2 , 1 );
      boost::shared_ptr<AnaZ> heavyQ;
      if( zevent->njets() > 0 && leptonP && leptonN ) {
        heavyQ.reset( new AnaZ( leptonP , leptonN , *(jets_by_pt.begin()) ) );
        dg::fillh( "Q_m" , 2500 , 0 , 2500 , heavyQ->m() , "Heavy Q Inv Mass (GeV/c^{2})" );
	if( zevent->nvlqtruth()==1 ) {
	  dg::fillh( "truth_Q_m" , 2500 , 0 , 2500 , zevent->truth_m() , "Truth Q Mass" );
	  dg::fillh( "Q_m_resolution" , 1000 , -500 , 500 , heavyQ->m() - zevent->truth_m() , "VLQ mass reco - truth" );
	}
      }
      if( zevent->njets() >=2 ) {
	dg::fillh( "max_deta_jets" , 100 , -8 , 8 , zevent->max_deta_jets() , "max #Delta #eta ( leading jet , next two highest pT jets )" );
      }
      if( zevent->njets() > 0 && leptonP && leptonN ) {
        dg::fillh( "dphi_leptonP_jet0" , 100 , -M_PI , M_PI , leptonP->dist_phi( *(jets_by_pt.begin()) ) , "#Delta #phi LeptonP - Leading Jet (rad)" );
        dg::fillh( "deta_leptonP_jet0" , 100 , -8 , 8 , leptonP->dist_eta( *(jets_by_pt.begin()) ) , "#Delta #eta LeptonP - Leading Jet" );
        dg::fillh( "dr_leptonP_jet0" , 50 , 0 , 5 , leptonP->dist_eta_phi( *(jets_by_pt.begin()) ) , "#Delta r(#eta,#phi) LeptonP - Leading Jet" );
        dg::fillh( "dphi_leptonN_jet0" , 100 , -M_PI , M_PI , leptonN->dist_phi( *(jets_by_pt.begin()) ) , "#Delta #phi LeptonN - Leading Jet (rad)" );
        dg::fillh( "deta_leptonN_jet0" , 100 , -8 , 8 , leptonN->dist_eta( *(jets_by_pt.begin()) ) , "#Delta #eta LeptonN - Leading Jet" );
        dg::fillh( "dr_leptonN_jet0" , 50 , 0 , 5 , leptonN->dist_eta_phi( *(jets_by_pt.begin()) ) , "#Delta r(#eta,#phi) LeptonN - Leading Jet" );
      }
      if ( leptonP && leptonN ) {
        dg::fillh( "dphi_leptons" , 100 , -M_PI , M_PI , leptonP->dist_phi( leptonN ) , "#Delta #phi Leptons (rad)" );
        dg::fillh( "deta_leptons" , 100 , -8 , 8 , leptonP->dist_eta( leptonN ) , "#Delta #eta Leptons" );
        dg::fillh( "dr_leptons" , 50 , 0 , 5 , leptonP->dist_eta_phi( leptonN ) , "#Delta r(#eta,#phi) Leptons" );
      }
      if( zevent->njets() > 1 ) {
        dg::fillh( "dphi_leading_jets" , 100 , 0 , M_PI , fabs((*(jets_by_pt.begin()))->dist_phi(*(jets_by_pt.begin()+1))) , "#Delta #phi Leading Jets (rad)");
        dg::fillh( "deta_leading_jets" , 100 , -8 , 8 , (*(jets_by_pt.begin()))->dist_eta( *(jets_by_pt.begin()+1) ) , "#Delta #eta Leading Jets" );
        dg::fillh( "dr_leading_jets" , 50 , 0 , 5 , (*(jets_by_pt.begin()))->dist_eta_phi( *(jets_by_pt.begin()+1) ) , "#Delta r(#eta,#phi) Leading Jets" );
	dg::fillh( "associated_jet_pt" , 500 , 0 , 500 , zevent->associated_jet()->pt() );
	dg::fillh( "associated_jet_eta" , 100 , -8 , 8 , zevent->associated_jet()->eta() );
	dg::fillh( "associated_jet_phi" , 100 , -M_PI , M_PI , zevent->associated_jet()->phi() );
	dg::fillh( "associated_jet_absphi" , 100 , 0 , M_PI , fabs(zevent->associated_jet()->phi()) );
      }
      // Get delta-R between lepton and nearest jet
      float min_dR1 = std::numeric_limits<float>::max();
      float min_dR2 = std::numeric_limits<float>::max();
      dg::down( "jet_pts" , "ith-highest pt jet(s)" ); BOOST_SCOPE_EXIT(void) { dg::up(); } BOOST_SCOPE_EXIT_END;
      // study ith-highest pt jet(s)
      unsigned int ithjet = 0u;
      for( vector< boost::shared_ptr<const AnaJet> >::iterator ijet=jets_by_pt.begin(), fjet=jets_by_pt.end(); ijet!=fjet; ++ijet, ++ithjet ) {
        const boost::shared_ptr<const AnaJet>& j( *ijet );
        dg::fillh( (format("jet_%|d|thpt_pt") % ithjet ).str() , 500 , 0 , 500 , j->pt() );
        dg::fillh( (format("jet_%|d|thpt_phi") % ithjet ).str() , 100 , -M_PI , M_PI , j->phi() );
        dg::fillh( (format("jet_%|d|thpt_absphi") % ithjet ).str() , 100 , 0 , M_PI , fabs( j->phi() ) );
        dg::fillh( (format("jet_%|d|thpt_eta") % ithjet ).str() , 100 , -8 , 8 , j->eta() );
        if( ithjet==0 ) { dg::fillh( "balance_vecsumphi_leading_jet" , 100 , -M_PI , M_PI ,
                                     detector::delta_phi(event_vecsumphi,j->phi()) , "Event Vector #Sigma p_{T} #phi - Leading Jet #phi (rad)" ); }
        float tmp_dR1 = leptonP->dist_eta_phi( j );
        if( tmp_dR1 < min_dR1 ) { min_dR1 = tmp_dR1; }
        float tmp_dR2 = leptonN->dist_eta_phi( j );
        if( tmp_dR2 < min_dR2 ) { min_dR2 = tmp_dR2; }
      }
      dg::fillh( "dr_leptonP_nearjet" , 100 , 0 , 5 , min_dR1 , "#Delta r(#eta,#phi) LeptonP - Nearest Jet" );
      dg::fillh( "dr_leptonN_nearjet" , 100 , 0 , 5 , min_dR2 , "#Delta r(#eta,#phi) LeptonN - Nearest Jet" );
    }

    // 0's TNtuple for unbinned likelihood
    if(muon1 && muon2 && _do_save_0ntuple && uncategorized()) {
      if( (muon1->pt_cone_ratio(AnaMuon::CONE40) < 0.08) || ( muon2->pt_cone_ratio(AnaMuon::CONE40) < 0.08) ) {
        if( (muon1->scharge())*(muon2->scharge())<0 ) {
	  std::vector<std::string> names = boost::assign::list_of("njets")("Z_m");
	  std::vector<float> values = boost::assign::list_of(zevent->njets()*1.0)(z->m());
          names.push_back("tag_pt");
          names.push_back("tag_eta");
          names.push_back("tag_iso");
          names.push_back("probe_pt");
          names.push_back("probe_eta");
          names.push_back("probe_iso");
          if( (muon1->pt_cone_ratio(AnaMuon::CONE40) < 0.08) && ( muon2->pt_cone_ratio(AnaMuon::CONE40) < 0.08) ) {
            double pick = gRandom->Rndm();
            if(pick<=0.5) {
              values.push_back(muon1->pt());
              values.push_back(muon1->eta());
              values.push_back(muon1->pt_cone_ratio(AnaMuon::CONE40));
              values.push_back(muon2->pt());
              values.push_back(muon2->eta());
              values.push_back(muon2->pt_cone_ratio(AnaMuon::CONE40));
            }
            else {
              values.push_back(muon2->pt());
              values.push_back(muon2->eta());
              values.push_back(muon2->pt_cone_ratio(AnaMuon::CONE40));
              values.push_back(muon1->pt());
              values.push_back(muon1->eta());
              values.push_back(muon1->pt_cone_ratio(AnaMuon::CONE40));
            }
          }
          else if ( muon1->pt_cone_ratio(AnaMuon::CONE40) < 0.08) {
            values.push_back(muon1->pt());
            values.push_back(muon1->eta());
            values.push_back(muon1->pt_cone_ratio(AnaMuon::CONE40));
            values.push_back(muon2->pt());
            values.push_back(muon2->eta());
            values.push_back(muon2->pt_cone_ratio(AnaMuon::CONE40));
          }
          else {
            values.push_back(muon2->pt());
            values.push_back(muon2->eta());
            values.push_back(muon2->pt_cone_ratio(AnaMuon::CONE40));
            values.push_back(muon1->pt());
            values.push_back(muon1->eta());
            values.push_back(muon1->pt_cone_ratio(AnaMuon::CONE40));
          }
          names.push_back("met");
          names.push_back("met_phi");
          if(met) {
            values.push_back(met->pt());
            values.push_back(met->phi());
          }
          else {
            values.push_back(-999);
            values.push_back(-999);
          }
          names.push_back("0thjet_pt");
          names.push_back("0thjet_eta");
          names.push_back("0thjet_phi");
          if(zevent->njets()>0) {
            values.push_back((*(jets_by_pt.begin()))->pt());
            values.push_back((*(jets_by_pt.begin()))->eta());
            values.push_back((*(jets_by_pt.begin()))->phi());
          }
          else {
            values.push_back(-999);
            values.push_back(-999);
            values.push_back(-999);
          }
          names.push_back("1thjet_pt");
          names.push_back("1thjet_eta");
          names.push_back("1thjet_phi");
          if(zevent->njets()>1) {
            values.push_back((*(jets_by_pt.begin()+1))->pt());
            values.push_back((*(jets_by_pt.begin()+1))->eta());
            values.push_back((*(jets_by_pt.begin()+1))->phi());
          }
          else {
            values.push_back(-999);
            values.push_back(-999);
            values.push_back(-999);
          }
          int nbj=0;
          for( vector< boost::shared_ptr<const AnaJet> >::iterator ijet=jets_by_pt.begin(), fjet=jets_by_pt.end(); ijet!=fjet; ++ijet ) {
            const boost::shared_ptr<const AnaJet>& j( *ijet );
            if((j->tag_sv0_w())>5.85) { nbj++; }
          }
          names.push_back("nbjets");
          values.push_back(nbj);
	  dg::fillnt("ntuple_zmumu",values,names);
        }
      }
    }
    if( _do_tag_and_probe_studies ) {
      if( muon1 && muon2 ) {
	dg::down( "tag_and_probe" , "tag and probe method for muons" ); BOOST_SCOPE_EXIT(void) { dg::up(); } BOOST_SCOPE_EXIT_END;
        if( (muon1->pt_cone_ratio(AnaMuon::CONE40) < 0.08) || ( muon2->pt_cone_ratio(AnaMuon::CONE40) < 0.08) ) {
          if(met) dg::fillh( "met" , 100 , 0 , 500 , met->pt() , "missing E_{T} (GeV)" );
	  dg::fillh( "njets" , 10, 0, 10, zevent->njets()>0, "number of jets" );
          if(zevent->njets()>0) {
	    dg::fillh( "leading_jet_pt" , 300 , 0 , 1500 , (*(jets_by_pt.begin()))->pt() , "leading jet P_{T} (GeV)");
	    dg::fillh( "leading_jet_eta" , 200 , -3 , 3 , (*(jets_by_pt.begin()))->eta() , "leading jet #eta");
          }
          if(zevent->njets()>1) {
	    dg::fillh( "subleading_jet_pt" , 300 , 0 , 1500 , (*(jets_by_pt.begin()+1))->pt() , "leading jet P_{T} (GeV)");
	    dg::fillh( "subleading_jet_eta" , 200 , -3 , 3 , (*(jets_by_pt.begin()+1))->eta() , "leading jet #eta (GeV)");
          }
        }
        if( (muon1->pt_cone_ratio(AnaMuon::CONE40) < 0.08) && ( muon2->pt_cone_ratio(AnaMuon::CONE40) < 0.08) ) {
          double pick = gRandom->Rndm();
          if(pick<=0.5) {
	    dg::fillh( "tag_pt", 100, 0, 500, muon1->pt(), "tag muon P_{T}" );
	    dg::fillh( "tag_eta", 200, -3, 3, muon1->eta(), "tag muon #eta" );
	    dg::fillh( "tag_phi", 20, -M_PI, M_PI, muon1->phi(), "tag muon #phi" );
	    dg::fillh( "tag_pt_check", 100, 0, 500, lepton1->pt(), "tag muon P_{T}" );
	    dg::fillh( "tag_eta_check", 200, -3, 3, lepton1->eta(), "tag muon #eta" );
	    dg::fillh( "tag_phi_check", 20, -M_PI, M_PI, lepton1->phi(), "tag muon #phi" );
	    dg::fillh( "tag_pt_cone_40ratio", 100, 0, 5, muon1->pt_cone_ratio(AnaMuon::CONE40), "tag muon P_{T} Cone40 ratio" );
	    dg::fillh( "higher_pt_tag_pt_cone_40ratio", 100, 0, 5, muon1->pt_cone_ratio(AnaMuon::CONE40), "tag muon P_{T} Cone40 ratio" );

	    dg::fillh( "probe_pt", 100, 0, 500, muon2->pt(), "probe muon P_{T}" );
	    dg::fillh( "probe_eta", 200, -3, 3, muon2->eta(), "probe muon #eta" );
	    dg::fillh( "probe_phi", 20, -M_PI, M_PI, muon2->phi(), "probe muon #phi" );
	    dg::fillh( "probe_pt_check", 100, 0, 500, lepton2->pt(), "probe muon P_{T}" );
	    dg::fillh( "probe_eta_check", 200, -3, 3, lepton2->eta(), "probe muon #eta" );
	    dg::fillh( "probe_phi_check", 20, -M_PI, M_PI, lepton2->phi(), "probe muon #phi" );
            //dg::fillh( "probe_pt_cone_40ratio", 100, 0, 5, muon2->pt_cone_ratio(AnaMuon::CONE40), "probe muon P_{T} Cone40 ratio" );
            //dg::fillh( "lower_pt_probe_pt_cone_40ratio", 100, 0, 5, muon2->pt_cone_ratio(AnaMuon::CONE40), "probe muon P_{T} Cone40 ratio" );
            //dg::fillh( "probe_muon_pt_vs_eta" , 100,0,500,50,-8,8,muon2->pt(),muon2->eta(),"probe muon P_{T} (GeV)", "probe muon #eta" );
            //dg::fillh( "probe_muon_pt_vs_abs_eta" , 100,0,500,25,0,8,muon2->pt(),fabs(muon2->eta()),"probe muon P_{T} (GeV)", "probe muon |#eta|" );
          }
          else {
	    dg::fillh( "tag_pt", 100, 0, 500, muon2->pt(), "tag lepton P_{T}" );
	    dg::fillh( "tag_eta", 200, -3, 3, muon2->eta(), "tag muon #eta" );
	    dg::fillh( "tag_phi", 20, -M_PI, M_PI, muon2->phi(), "tag muon #phi" );
	    dg::fillh( "tag_pt_check", 100, 0, 500, lepton2->pt(), "tag lepton P_{T}" );
	    dg::fillh( "tag_eta_check", 200, -3, 3, lepton2->eta(), "tag muon #eta" );
	    dg::fillh( "tag_phi_check", 20, -M_PI, M_PI, lepton2->phi(), "tag muon #phi" );
	    dg::fillh( "tag_pt_cone_40ratio", 100, 0, 5, muon2->pt_cone_ratio(AnaMuon::CONE40), "tag muon P_{T} Cone40 ratio" );
	    dg::fillh( "lower_pt_tag_pt_cone_40ratio", 100, 0, 5, muon2->pt_cone_ratio(AnaMuon::CONE40), "tag muon P_{T} Cone40 ratio" );

	    dg::fillh( "probe_pt", 100, 0, 500, muon1->pt(), "probe lepton P_{T}" );
	    dg::fillh( "probe_eta", 200, -3, 3, muon1->eta(), "probe muon #eta" );
	    dg::fillh( "probe_phi", 20, -M_PI, M_PI, muon1->phi(), "probe muon #phi" );
	    dg::fillh( "probe_pt_check", 100, 0, 500, lepton1->pt(), "probe lepton P_{T}" );
	    dg::fillh( "probe_eta_check", 200, -3, 3, lepton1->eta(), "probe muon #eta" );
	    dg::fillh( "probe_phi_check", 20, -M_PI, M_PI, lepton1->phi(), "probe muon #phi" );
            //dg::fillh( "probe_pt_cone_40ratio", 100, 0, 5, muon1->pt_cone_ratio(AnaMuon::CONE40), "probe muon P_{T} Cone40 ratio" );
            //dg::fillh( "higher_pt_probe_pt_cone_40ratio", 100, 0, 5, muon1->pt_cone_ratio(AnaMuon::CONE40), "probe muon P_{T} Cone40 ratio" );
            //dg::fillh( "probe_muon_pt_vs_eta" , 100,0,500,50,-8,8,muon1->pt(),muon1->eta(),"probe muon P_{T} (GeV)", "probe muon #eta" );
            //dg::fillh( "probe_muon_pt_vs_abs_eta" , 100,0,500,25,0,8,muon1->pt(),fabs(muon1->eta()),"probe muon P_{T} (GeV)", "probe muon |#eta|" );
          }
        }
        else if( muon1->pt_cone_ratio(AnaMuon::CONE40) < 0.08) {
	  dg::fillh( "tag_pt", 100, 0, 500, muon1->pt(), "tag muon P_{T}" );
	  dg::fillh( "tag_eta", 200, -3, 3, muon1->eta(), "tag muon #eta" );
	  dg::fillh( "tag_phi", 20, -M_PI, M_PI, muon1->phi(), "tag muon #phi" );
	  dg::fillh( "tag_pt_check", 100, 0, 500, lepton1->pt(), "tag muon P_{T}" );
	  dg::fillh( "tag_eta_check", 200, -3, 3, lepton1->eta(), "tag muon #eta" );
	  dg::fillh( "tag_phi_check", 20, -M_PI, M_PI, lepton1->phi(), "tag muon #phi" );
	  dg::fillh( "tag_pt_cone_40ratio", 100, 0, 5, muon1->pt_cone_ratio(AnaMuon::CONE40), "tag muon P_{T} Cone40 ratio" );
	  dg::fillh( "higher_pt_tag_pt_cone_40ratio", 100, 0, 5, muon1->pt_cone_ratio(AnaMuon::CONE40), "tag muon P_{T} Cone40 ratio" );

	  dg::fillh( "probe_pt", 100, 0, 500, muon2->pt(), "probe muon P_{T}" );
	  dg::fillh( "probe_eta", 200, -3, 3, muon2->eta(), "probe muon #eta" );
	  dg::fillh( "probe_phi", 20, -M_PI, M_PI, muon2->phi(), "probe muon #phi" );
	  dg::fillh( "probe_pt_check", 100, 0, 500, lepton2->pt(), "probe muon P_{T}" );
	  dg::fillh( "probe_eta_check", 200, -3, 3, lepton2->eta(), "probe muon #eta" );
	  dg::fillh( "probe_phi_check", 20, -M_PI, M_PI, lepton2->phi(), "probe muon #phi" );
          //dg::fillh( "probe_pt_cone_40ratio", 100, 0, 5, muon2->pt_cone_ratio(AnaMuon::CONE40), "probe muon P_{T} Cone40 ratio" );
          //dg::fillh( "lower_pt_probe_pt_cone_40ratio", 100, 0, 5, muon2->pt_cone_ratio(AnaMuon::CONE40), "probe muon P_{T} Cone40 ratio" );
          //dg::fillh( "probe_muon_pt_vs_eta" , 100,0,500,50,-8,8,muon2->pt(),muon2->eta(),"probe muon P_{T} (GeV)", "probe muon #eta" );
          //dg::fillh( "probe_muon_pt_vs_abs_eta" , 100,0,500,25,0,8,muon2->pt(),fabs(muon2->eta()),"probe muon P_{T} (GeV)", "probe muon |#eta|" );
        }
        else {
          /*assert(muon1);
          std::cerr << "1 muon/lepton phi: " << muon1->phi() << " vs " << lepton1->phi() << std::endl;
          std::cerr << "2 muon/lepton phi: " << muon2->phi() << " vs " << lepton2->phi() << std::endl;
          muon1->print();*/
	  dg::fillh( "tag_pt", 100, 0, 500, muon2->pt(), "tag lepton P_{T}" );
	  dg::fillh( "tag_eta", 200, -3, 3, muon2->eta(), "tag muon #eta" );
	  dg::fillh( "tag_phi", 20, -M_PI, M_PI, muon2->phi(), "tag muon #phi" );
	  dg::fillh( "tag_pt_check", 100, 0, 500, lepton2->pt(), "tag lepton P_{T}" );
	  dg::fillh( "tag_eta_check", 200, -3, 3, lepton2->eta(), "tag muon #eta" );
	  dg::fillh( "tag_phi_check", 20, -M_PI, M_PI, lepton2->phi(), "tag muon #phi" );
	  dg::fillh( "tag_pt_cone_40ratio", 100, 0, 5, muon2->pt_cone_ratio(AnaMuon::CONE40), "tag muon P_{T} Cone40 ratio" );
	  dg::fillh( "lower_pt_tag_pt_cone_40ratio", 100, 0, 5, muon2->pt_cone_ratio(AnaMuon::CONE40), "tag muon P_{T} Cone40 ratio" );

	  dg::fillh( "probe_pt", 100, 0, 500, muon1->pt(), "probe lepton P_{T}" );
	  dg::fillh( "probe_eta", 200, -3, 3, muon1->eta(), "probe muon #eta" );
	  dg::fillh( "probe_phi", 20, -M_PI, M_PI, muon1->phi(), "probe muon #phi" );
	  dg::fillh( "probe_pt_check", 100, 0, 500, lepton1->pt(), "probe lepton P_{T}" );
	  dg::fillh( "probe_eta_check", 200, -3, 3, lepton1->eta(), "probe muon #eta" );
	  dg::fillh( "probe_phi_check", 20, -M_PI, M_PI, lepton1->phi(), "probe muon #phi" );
          //dg::fillh( "probe_pt_cone_40ratio", 100, 0, 5, muon1->pt_cone_ratio(AnaMuon::CONE40), "probe muon P_{T} Cone40 ratio" );
          //dg::fillh( "higher_pt_probe_pt_cone_40ratio", 100, 0, 5, muon1->pt_cone_ratio(AnaMuon::CONE40), "probe muon P_{T} Cone40 ratio" );
          //dg::fillh( "probe_muon_pt_vs_eta" , 100,0,500,50,-8,8,muon1->pt(),muon1->eta(),"probe muon P_{T} (GeV)", "probe muon #eta" );
          //dg::fillh( "probe_muon_pt_vs_abs_eta" , 100,0,500,25,0,8,muon1->pt(),fabs(muon1->eta()),"probe muon P_{T} (GeV)", "probe muon |#eta|" );
        }
      }
    }


  } // end study Z cross section event candidate
protected:
  boost::shared_ptr<const AnaEvent> _event;
  bool _do_save_ntuple;
  bool _do_save_0ntuple;
  bool _do_heavyQ_studies;
  bool _do_2d_plots;
  bool _do_tag_and_probe_studies;
  bool _do_yield;
  unsigned long _yield_min;
  unsigned long _yield_max;
  unsigned long _yield_runnumber;
  float _yield_lumi;
  unsigned long _run_number;
public:
  void do_save_0ntuple(bool v) { _do_save_0ntuple = v; }
  void do_save_0ntuple() { do_save_0ntuple(true); }
  void do_heavyQ_studies(bool v = true) { _do_heavyQ_studies = v; }
  void do_2d_plots(bool v = true) { _do_2d_plots = v; }
  void do_tag_and_probe_studies(bool v) { _do_tag_and_probe_studies = v; }
  void do_tag_and_probe_studies() { do_tag_and_probe_studies(false); }
  void do_yield(bool v = true) { _do_yield = v; }
  void do_yield( const unsigned long& min , const unsigned long& max , const unsigned long& runnumber , const float& lumi ) { 
    _do_yield = true; _yield_min = min ; _yield_max = max ; _yield_runnumber = runnumber; _yield_lumi = lumi; 
  }
  StudyZCrossSection( const std::string& name , const std::string& description ) 
    : _event()
    , _do_save_0ntuple(false)
    , _do_heavyQ_studies(false)
    , _do_2d_plots(true)
    , _do_tag_and_probe_studies(false)
    , _do_yield(false)
    , _yield_min( 0. )
    , _yield_max( 1. )
    , _yield_runnumber( 0. )
    , _yield_lumi( 1. )
    , _run_number(0)
    , DataGraphics::DgStudy<type>(name,description)
  {
  }
  virtual ~StudyZCrossSection() {}
  void for_event( const boost::shared_ptr<const AnaEvent>& event ) { _event = event; }
  void for_run( unsigned long rnum ) { _run_number = rnum; }
};

#endif // STUDYZCROSSECTION_HPP
