 
#ifndef STUDYZASYMMETRY_HPP
#define STUDYZASYMMETRY_HPP

#include <cmath>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
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
#include "DataGraphics/DgBin.hh"
#include "TrigFTKAna/AnaZCrossSectionCandidate.hpp"
#include "TrigFTKAna/AnaParticle.hpp"
#include "TrigFTKAna/AnaMET.hpp"
#include "TrigFTKAna/AnaZ.hpp"
#include "TrigFTKAna/AnaMuon.hpp"
#include "TrigFTKAna/AnaElectron.hpp"
#include "TrigFTKAna/Detector.hpp"


class
StudyZAsymmetry : public DataGraphics::DgStudy< boost::shared_ptr<const AnaZCrossSectionCandidate> >
{
public:
  typedef boost::shared_ptr<const AnaZCrossSectionCandidate> type; // must be same type as the template argument for DgStudy.
protected:

  virtual void _study( type const& z ) {
    using namespace DataGraphics;
    if(_do_save_ntuple && uncategorized()) {
      study_ntuple(z);
    }
    // at top level, always require a category [since we broadly pass very loose candidates]
    if(_do_save_histo && categorized()) {
      study_histo_regions(z);
    }
  }

  virtual void study_histo_regions( type const& z ) {
    using namespace std;
    using namespace boost;
    using namespace DataGraphics;
    const boost::shared_ptr<const AnaParticle>& leptonP = z->lepton_pos(); // positive lepton
    const boost::shared_ptr<const AnaParticle>& leptonN = z->lepton_neg(); // negative lepton
    const boost::shared_ptr<const AnaParticle>& lepton1 = z->lepton1(); // higher-pt lepton
    const boost::shared_ptr<const AnaParticle>& lepton2 = z->lepton2(); // lower-pt lepton
    const boost::shared_ptr<const AnaMuon> muonP = boost::dynamic_pointer_cast<const AnaMuon,const AnaParticle>(leptonP);
    const boost::shared_ptr<const AnaMuon> muonN = boost::dynamic_pointer_cast<const AnaMuon,const AnaParticle>(leptonN);
    const boost::shared_ptr<const AnaMuon> muon1 = boost::dynamic_pointer_cast<const AnaMuon,const AnaParticle>(lepton1);
    const boost::shared_ptr<const AnaMuon> muon2 = boost::dynamic_pointer_cast<const AnaMuon,const AnaParticle>(lepton2);
    const boost::shared_ptr<const AnaMET>& met = z->met();
    const AnaParticle::ftype jet_sumpt =  std::accumulate( z->begin_jets() , z->end_jets() , 0. , 
                                                           lambda::bind(&AnaJet::pt,lambda::bind(&shared_ptr<const AnaJet>::get,lambda::_2)) + lambda::_1 );
    // jet ordering
    vector< boost::shared_ptr<const AnaJet> > jets_by_pt( z->begin_jets() , z->end_jets() );
    // make plots
    dg::fillh( "z_m" , 200 , 0 , 200 , z->mass() , "Z Mass (GeV/c^{2})" );
    dg::fillh( "z_pt" , 200 , 0 , 200 , z->pt() , "Z Momentum (GeV/c)" );
    //dg::fillh( "njets" , 10 , 0 , 10 , z->njets() , "N_{jets}" );
    //dg::fillh( "nvtxs_all" , 20 , 0 , 20 , z->nvtxs_all() , "N_{vtxs} (before cuts)" );
    //dg::fillh( "jet_sumpt" , 200 , 0 , 200 , jet_sumpt , "Jet #Sigma p_{T} (GeV)" );
    if( met ) {
      dg::fillh( "met" , 200 , 0 , 200 , met->pt() , "Z Missing ET (GeV)" );
      //dg::fillh( "met_phi", 100 , -M_PI , M_PI , met->phi() , "MET #phi (rad)" );
      //if( _do_2d_plots ) dg::fillh( "zm_vs_met" , 200,0,200,200,0,200,met->pt(),z->m(),"MET", "Z Mass (GeV/c^{2})" );
    } // end met plots
    if( leptonP && leptonN) {
      // mu+
      dg::fillh( "leptonP_pt" , 200 , 0 , 200 , leptonP->pt() , "l+ p_{T} (GeV/c)" ); 
      //dg::fillh( "leptonP_eta" , 100 , -8 , 8 , leptonP->eta() , "l+ #eta" );
      dg::fillh( "leptonP_phi" , 100 , -M_PI , M_PI , leptonP->phi() , "l+ #phi (rad)" );
      dg::fillvh( "leptonP_absetav" , dg::bin().D_abseta.size()-1, dg::bin().D_abseta, std::abs(leptonP->eta()) , "Z l+ |#eta|" );
      dg::fillvh( "leptonP_etav" , dg::bin().D_eta.size()-1, dg::bin().D_eta, leptonP->eta() , "Z l+ #eta" );
      // mu-
      dg::fillh( "leptonN_pt" , 200 , 0 , 500 , leptonN->pt() , "l- p_{T} (GeV/c)" );
      //dg::fillh( "leptonN_eta" , 100 , -8 , 8 , leptonN->eta() , "l- #eta" );
      dg::fillh( "leptonN_phi" , 100 , -M_PI , M_PI , leptonN->phi() , "l- #phi (rad)" );
      dg::fillvh( "leptonN_absetav" , dg::bin().D_abseta.size()-1, dg::bin().D_abseta, std::abs(leptonN->eta()) , "Z l- |#eta|" );
      dg::fillvh( "leptonN_etav" , dg::bin().D_eta.size()-1, dg::bin().D_eta, leptonN->eta() , "Z l- #eta" );
      // both charges
      dg::fillh( "lepton_phi" , 100 , -M_PI , M_PI , leptonP->phi() , "l #phi (rad)" );
      dg::fillh( "lepton_phi" , 100 , -M_PI , M_PI , leptonN->phi() , "l #phi (rad)" );
      // joint quantities for mu+ & mu-
      //dg::fillh( "dphi_leptonP_leptonN" , 100 , -M_PI , M_PI , leptonP->dist_phi( leptonN ) , "#Delta #phi between leptons (rad)" );
      //dg::fillh( "deta_leptonP_leptonN" , 100 , -M_PI , M_PI , leptonP->dist_eta( leptonN ) , "#Delta #eta between leptons (rad)" );
      //dg::fillh( "dr_leptonP_leptonN" , 100 , -M_PI , M_PI , leptonP->dist_eta_phi( leptonN ) , "#Delta r(#phi,#phi) between leptons (rad)" );
      // special quantities for muons
      if(false && muonP && muonN) {
	dg::fillh( "leptonP_id_pt" , 200 , 0 , 200 , muonP->id_pt() , "mu+ ID p_{T} (GeV/c)" );
	dg::fillh( "leptonP_exms_pt" , 200 , 0 , 200 , muonP->exms_pt() , "mu+ ex. MS p_{T} (GeV/c)" ); 
	dg::fillh( "leptonN_id_pt" , 200 , 0 , 200 , muonN->id_pt() , "mu- ID p_{T} (GeV/c)" );
	dg::fillh( "leptonN_exms_pt" , 200 , 0 , 200 , muonN->exms_pt() , "mu- ex. MS p_{T} (GeV/c)" ); 
	dg::fillh( "leptonP_id_invpt" , 200 , 5e-3 , 5e-2 , 1.0/muonP->id_pt() , "mu+ ID p_{T} (GeV/c)" );
	dg::fillh( "leptonP_exms_invpt" , 200 , 5e-3 , 5e-2 , 1.0/muonP->exms_pt() , "mu+ ex. MS p_{T} (GeV/c)" ); 
	dg::fillh( "leptonN_id_invpt" , 200 , 5e-3 , 5e-2 , 1.0/muonN->id_pt() , "mu- ID p_{T} (GeV/c)" );
	dg::fillh( "leptonN_exms_invpt" , 200 , 5e-3 , 5e-2 , 1.0/muonN->exms_pt() , "mu- ex. MS p_{T} (GeV/c)" ); 
      }
    }
    if( _do_yield ) {
      dg::fillhw( "yield" , int( _yield_max - _yield_min ) ,
		   _yield_min , _yield_max , _yield_runnumber ,
		   dg::global_weight() / _yield_lumi , "run number" );
    }
    if( false && z->njets() >=2) {
      dg::fillh( "max_deta_jets" , 100 , -8 , 8 , z->max_deta_jets() , "max #Delta #eta ( leading jet , next two highest pT jets )" );
    }
    if(false) {
      std::sort( jets_by_pt.begin() , jets_by_pt.end() , bind(&AnaJet::pt,_1) > bind(&AnaJet::pt,_2) );
    }
    if( false && z->njets() > 0 && leptonP && leptonN) {
      dg::fillh( "dphi_leptonP_jet0" , 100 , -M_PI , M_PI , leptonP->dist_phi( *(jets_by_pt.begin()) ) , "#Delta #phi LeptonP - Leading Jet (rad)" );
      dg::fillh( "deta_leptonP_jet0" , 100 , -8 , 8 , leptonP->dist_eta( *(jets_by_pt.begin()) ) , "#Delta #eta LeptonP - Leading Jet" );
      dg::fillh( "dr_leptonP_jet0" , 50 , 0 , 5 , leptonP->dist_eta_phi( *(jets_by_pt.begin()) ) , "#Delta r(#eta,#phi) LeptonP - Leading Jet" );
      dg::fillh( "dphi_leptonN_jet0" , 100 , -M_PI , M_PI , leptonN->dist_phi( *(jets_by_pt.begin()) ) , "#Delta #phi LeptonN - Leading Jet (rad)" );
      dg::fillh( "deta_leptonN_jet0" , 100 , -8 , 8 , leptonN->dist_eta( *(jets_by_pt.begin()) ) , "#Delta #eta LeptonN - Leading Jet" );
      dg::fillh( "dr_leptonN_jet0" , 50 , 0 , 5 , leptonN->dist_eta_phi( *(jets_by_pt.begin()) ) , "#Delta r(#eta,#phi) LeptonN - Leading Jet" );
    }
    // study ith-highest pt jet(s)
    if(false) {
      dg::down( "jet_pts" , "ith-highest pt jet(s)" ); BOOST_SCOPE_EXIT(void) { dg::up(); } BOOST_SCOPE_EXIT_END;
      unsigned int ithjet = 0u;
      for( vector< boost::shared_ptr<const AnaJet> >::iterator ijet=jets_by_pt.begin(), fjet=jets_by_pt.end(); ijet!=fjet; ++ijet, ++ithjet ) {
	const boost::shared_ptr<const AnaJet>& j( *ijet );
	if(ithjet<2) { // don't waste memory on fourth and higher pt jets
	  dg::fillh( (format("jet_%|d|thpt_pt") % ithjet ).str() , 200 , 0 , 200 , j->pt() );
	  //dg::fillh( (format("jet_%|d|thpt_phi") % ithjet ).str() , 100 , -M_PI , M_PI , j->phi() );
	  dg::fillh( (format("jet_%|d|thpt_eta") % ithjet ).str() , 100 , -8 , 8 , j->eta() );
	}
      }
    }
  }

  virtual void study_ntuple( type const& z ) {
    using namespace std;
    using namespace boost;
    using namespace DataGraphics;
    const boost::shared_ptr<const AnaParticle>& leptonP = z->lepton_pos(); // positive lepton
    const boost::shared_ptr<const AnaParticle>& leptonN = z->lepton_neg(); // negative lepton
    const boost::shared_ptr<const AnaParticle>& lepton1 = z->lepton1(); // higher-pt lepton
    const boost::shared_ptr<const AnaParticle>& lepton2 = z->lepton2(); // lower-pt lepton
    const boost::shared_ptr<const AnaMuon> muonP = boost::dynamic_pointer_cast<const AnaMuon,const AnaParticle>(leptonP);
    const boost::shared_ptr<const AnaMuon> muonN = boost::dynamic_pointer_cast<const AnaMuon,const AnaParticle>(leptonN);
    const boost::shared_ptr<const AnaMuon> muon1 = boost::dynamic_pointer_cast<const AnaMuon,const AnaParticle>(lepton1);
    const boost::shared_ptr<const AnaMuon> muon2 = boost::dynamic_pointer_cast<const AnaMuon,const AnaParticle>(lepton2);
    const boost::shared_ptr<const AnaMET>& met = z->met();
    if( leptonP && leptonN) {
      // DgNtuple for unbinned likelihood
      using namespace boost::assign; // bring 'operator+=()' into scope
      const short baseline_status = z->asym_baseline() ? 1 : 0;
      // the following selects positive and negative leptons, if possible
      // But for same-sign fake studies, it instead uses higher and lower pt leptons
      const boost::shared_ptr<const AnaParticle>& lP = leptonP && leptonN ? leptonP : lepton1;
      const boost::shared_ptr<const AnaParticle>& lN = leptonP && leptonN ? leptonN : lepton2;
      const boost::shared_ptr<const AnaMuon>& mP = muonP && muonN ? muonP : muon1;
      const boost::shared_ptr<const AnaMuon>& mN = muonP && muonN ? muonN : muon2;
      std::vector<std::string> names = list_of("avgmu")("Z_m")("Z_pt")("lP_q")("lP_pt")("lP_eta")("lP_phi")("lN_q")("lN_pt")("lN_eta")("lN_phi")("njets")("nvtxs")("nvtxs_all")("nmuons")("nmuons25")("mask");
      std::vector<float> values = list_of(_average_mu)(z->m())(z->pt())(lP->scharge()*1.0)(lP->pt())(lP->eta())(lP->phi())(lN->scharge()*1.0)(lN->pt())(lN->eta())(lN->phi())(z->njets()*1.0)(z->nvtxs()*1.0)(z->nvtxs_all()*1.0)(z->nleptons()*1.0)(z->nleptons25()*1.0)(baseline_status*1.0);
      if(met) {
	names += "met","met_phi";
	values += met->pt(),met->phi();
      }
      if(mP && mN) {
	// type
	names += "lP_type","lN_type";
	values += mP->cutflow_type()*1.0,mN->cutflow_type()*1.0;
	// trigger matching
	names += "lP_trigEF"; //,"lP_trigEFb";
	names += "lN_trigEF"; //,"lN_trigEFb";
	const int end_of_periodI = 186493;
	if(_run_number <= end_of_periodI) {
	  values += muonP->trigger_match_dr("EF_mu18_MG"); //,muonP->trigger_match_dr("EF_mu40_MSonly_barrel");
	  values += muonN->trigger_match_dr("EF_mu18_MG"); //,muonN->trigger_match_dr("EF_mu40_MSonly_barrel");
	}
	else {
	  values += muonP->trigger_match_dr("EF_mu18_MG_medium"); //,muonP->trigger_match_dr("EF_mu40_MSonly_barrel_medium");
	  values += muonN->trigger_match_dr("EF_mu18_MG_medium"); //,muonN->trigger_match_dr("EF_mu40_MSonly_barrel_medium");
	}

	// save trigger weight for each individual muon?
	float lP_trigw = 1.0;
	float lN_trigw = 1.0;
	if(_is_mc) {
	  std::vector<TLorentzVector> velectrons; // should be empty
	  std::vector<TLorentzVector> vmuonsP; vmuonsP.push_back(mP->four_vector());
	  std::vector<float> vmuons_chargesP; vmuons_chargesP.push_back(mP->charge());
	  std::vector<TLorentzVector> vmuonsN; vmuonsN.push_back(mN->four_vector());
	  std::vector<float> vmuons_chargesN; vmuons_chargesN.push_back(mN->charge());
	  double Aval = 1.0;
	  double Aerr = 0;
	  AnaMuon::GetTriggerSF_v17_custom2( AnaConfiguration::conf() , detector::MCP_STACO_COMBINED , detector::EGAMMA_MEDIUMPP , 
		 			     vmuonsP , velectrons , vmuons_chargesP, 
					     _run_number , 0 , Aval , Aerr , -1);
	  lP_trigw = Aval;
	  AnaMuon::GetTriggerSF_v17_custom2( AnaConfiguration::conf() , detector::MCP_STACO_COMBINED , detector::EGAMMA_MEDIUMPP , 
		 			     vmuonsN , velectrons , vmuons_chargesN,
					     _run_number , 0 , Aval , Aerr , -1);
	  lN_trigw = Aval;
	}
	names += "lP_trigw","lN_trigw";
	values += lP_trigw,lN_trigw;

	// isolation
	names += "lP_ptiso40","lP_ptiso30","lP_ptiso20","lP_etiso40","lP_etiso30","lP_etiso20";
	values += mP->pt_cone(AnaMuon::CONE40),mP->pt_cone(AnaMuon::CONE30),mP->pt_cone(AnaMuon::CONE20),mP->et_cone(AnaMuon::CONE40),mP->et_cone(AnaMuon::CONE30),mP->et_cone(AnaMuon::CONE20);
	names += "lN_ptiso40","lN_ptiso30","lN_ptiso20","lN_etiso40","lN_etiso30","lN_etiso20";
	values += mN->pt_cone(AnaMuon::CONE40),mN->pt_cone(AnaMuon::CONE30),mN->pt_cone(AnaMuon::CONE20),mN->et_cone(AnaMuon::CONE40),mN->et_cone(AnaMuon::CONE30),mN->et_cone(AnaMuon::CONE20);
	names += "lP_etiso30corr","lN_etiso30corr"; // corrected et isolation from Peter
	values += muonP->et_cone_corr(AnaMuon::CONE30 , z->nvtxs_all() ), muonN->et_cone_corr(AnaMuon::CONE30 , z->nvtxs_all() );
	// vertex and ID variables
	names += "lP_z0","lP_d0","lP_d0sig","lP_idhits";
	values += mP->exPV_z0(),mP->exPV_d0(),mP->exPV_d0_signif(),mP->passes_hits_mcp_v17_moriond()?1.:0.0;
	names += "lN_z0","lN_d0","lN_d0sig","lN_idhits";
	values += mN->exPV_z0(),mN->exPV_d0(),mN->exPV_d0_signif(),mN->passes_hits_mcp_v17_moriond()?1.:0.0;
	// kinematics of ID and EXMS muons
	const TLorentzVector zid = mP->id_muon() + mN->id_muon();
	// const TLorentzVector zms = mP->ms_muon() + mN->ms_muon(); // non-extrapolated
	const TLorentzVector zexms = mP->exms_muon() + mN->exms_muon();
	names += "Z_m_id","Z_pt_id","lP_q_id","lP_pt_id","lP_eta_id","lP_phi_id","lN_q_id","lN_pt_id","lN_eta_id","lN_phi_id";
	values += zid.M(),zid.Pt(),mP->id_charge()*1.0,mP->id_pt(),mP->id_eta(),mP->id_phi(),mN->id_charge()*1.0,mN->id_pt(),mN->id_eta(),mN->id_phi();
	names += "Z_m_exms","Z_pt_exms","lP_q_exms","lP_pt_exms","lP_eta_exms","lP_phi_exms","lN_q_exms","lN_pt_exms","lN_eta_exms","lN_phi_exms";
	values += zexms.M(),zexms.Pt(),mP->exms_charge()*1.0,mP->exms_pt(),mP->exms_eta(),mP->exms_phi(),mN->exms_charge()*1.0,mN->exms_pt(),mN->exms_eta(),mN->exms_phi();
      }
      dg::fillnt("ntuple",values,names);
    }
  }

protected:
  bool _do_save_ntuple;
  bool _do_2d_plots;
  bool _do_tag_and_probe_studies;
  bool _do_save_histo;
  bool _do_yield;
  unsigned long _yield_min;
  unsigned long _yield_max;
  unsigned long _yield_runnumber;
  float _yield_lumi;
  unsigned long _run_number;
  bool _is_mc;
  float _average_mu;
public:
  void do_save_ntuple(bool v=true) { _do_save_ntuple = v; }
  void do_2d_plots(bool v = true) { _do_2d_plots = v; }
  void do_tag_and_probe_studies(bool v) { _do_tag_and_probe_studies = v; }
  void do_save_histo(bool v = true) { _do_save_histo = v; }
  void do_yield(bool v = true) { _do_yield = v; }
  void do_yield( const unsigned long& min , const unsigned long& max , const unsigned long& runnumber , const float& lumi ) { 
    _do_yield = true; _yield_min = min ; _yield_max = max ; _yield_runnumber = runnumber; _yield_lumi = lumi; 
  }
  StudyZAsymmetry( const std::string& name , const std::string& description ) 
    : _do_save_ntuple(false)
    , _do_2d_plots(false)
    , _do_tag_and_probe_studies(false)
    , _do_save_histo(true)
    , _do_yield(false)
    , _yield_min( 0. )
    , _yield_max( 1. )
    , _yield_runnumber( 0. )
    , _yield_lumi( 1. )
    , _run_number(0)
    , _is_mc(false)
    , _average_mu(0)
    , DataGraphics::DgStudy<type>(name,description)
  {
  }
  virtual ~StudyZAsymmetry() {}
  void for_run( unsigned long rnum ) { _run_number = rnum; }
  void for_mc( bool ismc ) { _is_mc = ismc; }
  void for_average_mu(float avgmu) { _average_mu = avgmu; }
};

#endif // STUDYZASYMMETRY_HPP
