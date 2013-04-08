#ifndef WRAP_STUDYHWWTRUTHPRODUCTS_HH
#define WRAP_STUDYHWWTRUTHPRODUCTS_HH


#include <vector>
#include <string>
#include <cmath>
#include <functional>
#include <boost/shared_ptr.hpp>
#include <boost/foreach.hpp>
#include <boost/bind.hpp>
#include <TVector3.h>
#include <TLorentzVector.h>
#include "DataGraphics/DgStudy.hh"
#include "DataGraphics/DgCollection.hh"
#include "DataGraphics/DgCutSet.hh"
#include "TrigFTKAna/AnaTruthParticle.hpp"
#include "TrigFTKAna/AnaTruthTable.hpp"
#include "TrigFTKAna/AnaHiggs.hpp"
#include "TrigFTKAna/Detector.hpp"

template< class collT >
class
StudyHwwTruthProducts : public DataGraphics::DgStudy< collT >
{
public:
  // typedef boost::shared_ptr<const AnaParticle>  type; // must be same type as the template argument for DgStudy.
  typedef collT type;
  typedef typename collT::value_type ptr_type;

  static void study( type const& coll ) { // must be same type as the template argument for DgStudy.
    using namespace DataGraphics;
    using boost::shared_ptr;
    using boost::bind;
    const unsigned int n = coll.size();
    dg::fillh( "nbodies" , 10 , 0 , 10 , n , "Number of Higgs Final States" );
    // find highest pt lepton
    // find highest eta lepton
    // etc.
    shared_ptr<const AnaTruthParticle> leading_pt_lepton;
    shared_ptr<const AnaTruthParticle> subleading_pt_lepton;
    shared_ptr<const AnaTruthParticle> lowest_eta_lepton;
    shared_ptr<const AnaTruthParticle> highest_eta_lepton;
    shared_ptr<const AnaTruthParticle> highest_eta_lepton_pt20;
    shared_ptr<const AnaTruthParticle> highest_eta_lepton_pt10;
    std::vector< shared_ptr<const AnaTruthParticle> > neutrinos;
    TVector3 sum_missing(0,0,0);
    unsigned int n_leptons_pt_gt_40 = 0u;
    unsigned int n_leptons_pt_gt_30 = 0u;
    unsigned int n_leptons_pt_gt_20 = 0u;
    unsigned int n_leptons_pt_gt_15 = 0u;
    unsigned int n_leptons_pt_gt_10 = 0u;
    unsigned int n_leptons_pt_gt_5 = 0u;
    unsigned int n_leptons_eta_gt_3 = 0u;
    unsigned int n_leptons_eta_gt_25 = 0u;
    unsigned int n_leptons_eta_gt_2 = 0u;
    unsigned int n_leptons_eta_gt_15 = 0u;
    unsigned int n_leptons_eta_gt_1 = 0u;
    unsigned int n_leptons = 0u;
    BOOST_FOREACH( const ptr_type& p , coll ) {
      if( p->is_neutrino() ) {
        neutrinos.push_back( p );
        sum_missing += p->four_vector().Vect();
      }
      if( p->is_charged_lepton() ) {
        ++n_leptons;
        if( !leading_pt_lepton || p->pt() > leading_pt_lepton->pt() ) {
          subleading_pt_lepton = leading_pt_lepton;
          leading_pt_lepton = p; 
        } else if( !subleading_pt_lepton || p->pt() > subleading_pt_lepton->pt() ) {
          subleading_pt_lepton = p; 
        }
        if( !highest_eta_lepton || std::abs(p->eta()) > std::abs(highest_eta_lepton->eta()) ) {
          highest_eta_lepton = p; 
        }
        if( p->pt()>20. && (!highest_eta_lepton_pt20 || std::abs(p->eta()) > std::abs(highest_eta_lepton_pt20->eta())) ) {
          highest_eta_lepton_pt20 = p; 
        }
        if( p->pt()>10. && (!highest_eta_lepton_pt10 || std::abs(p->eta()) > std::abs(highest_eta_lepton_pt10->eta())) ) {
          highest_eta_lepton_pt10 = p; 
        }
        if( !lowest_eta_lepton || std::abs(p->eta()) < std::abs(lowest_eta_lepton->eta()) ) {
          lowest_eta_lepton = p; 
        }
        if( p->pt() > 40. ) { ++n_leptons_pt_gt_40; }
        if( p->pt() > 30. ) { ++n_leptons_pt_gt_30; }
        if( p->pt() > 20. ) { ++n_leptons_pt_gt_20; }
        if( p->pt() > 15. ) { ++n_leptons_pt_gt_15; }
        if( p->pt() > 10. ) { ++n_leptons_pt_gt_10; }
        if( p->pt() >  5. ) { ++n_leptons_pt_gt_5; }
        if( std::abs(p->eta()) > 3. ) { ++n_leptons_eta_gt_3; }
        if( std::abs(p->eta()) > 2.5 ) { ++n_leptons_eta_gt_25; }
        if( std::abs(p->eta()) > 2. ) { ++n_leptons_eta_gt_2; }
        if( std::abs(p->eta()) > 1.5 ) { ++n_leptons_eta_gt_15; }
        if( std::abs(p->eta()) > 1. ) { ++n_leptons_eta_gt_1; }
      }
    }
    if( leading_pt_lepton ) { 
      dg::fillh( "leading_pt_lepton_pt" , 100 , 0 , 200 , leading_pt_lepton->pt() , "GeV" ); 
      dg::fillh( "leading_pt_lepton_abspdgcode" , 6 , 11 , 17 , std::abs(leading_pt_lepton->pdg_id()) , "|pdg code|" ); 
    }
    if( subleading_pt_lepton ) { 
      dg::fillh( "subleading_pt_lepton_pt" , 100 , 0 , 200 , subleading_pt_lepton->pt() , "GeV" ); 
      dg::fillh( "subleading_pt_lepton_abspdgcode" , 6 , 11 , 17 , std::abs(subleading_pt_lepton->pdg_id()) , "|pdg code|" ); 
    }
    if( lowest_eta_lepton ) { dg::fillh( "lowest_eta_lepton_eta" , 64 , -4 , 4 , lowest_eta_lepton->eta() ); }
    if( highest_eta_lepton ) { dg::fillh( "highest_eta_lepton_eta" , 64 , -4 , 4 , highest_eta_lepton->eta() ); }
    dg::fillh( "n_leptons" , 10 , 0 , 10 , n_leptons );
    dg::fillh( "n_leptons_pt_gt_40" , 10 , 0 , 10 , n_leptons_pt_gt_40 );
    dg::fillh( "n_leptons_pt_gt_30" , 10 , 0 , 10 , n_leptons_pt_gt_30 );
    dg::fillh( "n_leptons_pt_gt_20" , 10 , 0 , 10 , n_leptons_pt_gt_20 );
    dg::fillh( "n_leptons_pt_gt_15" , 10 , 0 , 10 , n_leptons_pt_gt_15 );
    dg::fillh( "n_leptons_pt_gt_10" , 10 , 0 , 10 , n_leptons_pt_gt_10 );
    dg::fillh( "n_leptons_pt_gt_5" , 10 , 0 , 10 , n_leptons_pt_gt_5 );
    dg::fillh( "n_leptons_eta_gt_3" , 10 , 0 , 10 , n_leptons_eta_gt_3 );
    dg::fillh( "n_leptons_eta_gt_25" , 10 , 0 , 10 , n_leptons_eta_gt_25 );
    dg::fillh( "n_leptons_eta_gt_2" , 10 , 0 , 10 , n_leptons_eta_gt_2 );
    dg::fillh( "n_leptons_eta_gt_15" , 10 , 0 , 10 , n_leptons_eta_gt_15 );
    dg::fillh( "n_leptons_eta_gt_1" , 10 , 0 , 10 , n_leptons_eta_gt_1 );
    dg::fillh( "true_met" , 100 , 0 , 100 , sum_missing.Pt() , "GeV" );
    // correlations between leading and subleading lepton
    if( leading_pt_lepton && subleading_pt_lepton ) { 
      // lepton polar angle correlations in lab frame
      TLorentzVector p1(leading_pt_lepton->four_vector());
      TLorentzVector p2(subleading_pt_lepton->four_vector());
      dg::fillh( "cl2_vs_cl1" , 100 , -1 , 1 , 100 , -1 , 1 , p1.CosTheta() , p2.CosTheta() , "cos( #theta_{1} )" , "cos( #theta_{2} )" );
      dg::fillh( "pt2_vs_pt1" , 100 , 0 , 100 , 100 , 0 , 100 , p1.Pt() , p2.Pt() , "Leading Lepton p_{T} (GeV)" , "Subleading Lepton p_{T} (GeV)" );
      // lepton polar angle correlations in rest frame (~zero transverse boost, nonzero longitudinal boost)
      TLorentzVector qz = p1+p2;
      TLorentzVector p1q(p1);
      TLorentzVector p2q(p2);
      p1q.Boost( -qz.BoostVector() );
      p2q.Boost( -qz.BoostVector() );
      dg::fillh( "cl2rest_vs_cl1rest" , 100 , -1 , 1 , 100 , -1 , 1 , p1q.CosTheta() , p2q.CosTheta() , "cos( #theta_{1}^{rest} )" , "cos( #theta_{2}^{rest} )" );
      // lepton polar angle correlations in z-boost-only rest frame
      TLorentzVector p1z(p1);
      TLorentzVector p2z(p2);
      TVector3 zboost( qz.BoostVector() );
      zboost.SetX( 0. );
      zboost.SetY( 0. );
      p1z.Boost( -zboost );
      p2z.Boost( -zboost );
      dg::fillh( "cl20_vs_cl10" , 100 , -1 , 1 , 100 , -1 , 1 , p1z.CosTheta() , p2z.CosTheta() , "cos( #theta_{1}^{0} )" , "cos( #theta_{2}^{0} )" );
      // peter event selection
      if( true ) {
        static DgCutSet< shared_ptr<const AnaHiggs> > selector( "generator_hww_peter_selector" );
        static bool initialized = false;
        if( !initialized ) {
          double mhiggs = 120.;
          selector.add_uu( "leading_eta" , 
                           bind( &AnaParticle::abseta , bind( &AnaHiggs::dilepton_leading_lepton,_1) ) < 2.5 , 
                           bind( &dg::fillh , _1 , 80 , -4 , 4 , bind( &AnaParticle::eta , bind(&AnaHiggs::dilepton_leading_lepton,_2) ) , "leading lepton #eta" ) );
          selector.add_uu( "subleading_eta" , 
                           bind( &AnaParticle::abseta , bind( &AnaHiggs::dilepton_subleading_lepton,_1) ) < 2.5 , 
                           bind( &dg::fillh , _1 , 80 , -4 , 4 , bind( &AnaParticle::eta , bind(&AnaHiggs::dilepton_subleading_lepton,_2) ) , "subleading lepton #eta" ) );
          selector.add_uu( "mll" , 
                           bind( &AnaParticle::mass , bind(&AnaHiggs::dilepton_pair,_1) ) > 15. && bind( &AnaParticle::mass , bind(&AnaHiggs::dilepton_pair,_1) ) < 50. ,
                           bind( &dg::fillh , _1 , 100 , 0 , 100 , bind( &AnaParticle::mass , bind(&AnaHiggs::dilepton_pair,_2) ) , "dilepton mass (GeV)" ) );
          selector.add_uu( "met" , 
                           bind( &AnaParticle::pt , bind(&AnaHiggs::dilepton_met,_1) ) > 30. ,
                           bind( &dg::fillh , _1 , 100 , 0 , 100 , bind( &AnaParticle::pt , bind(&AnaHiggs::dilepton_met,_2) ) , "MET (GeV)" ) );
          selector.add_uu( "ptll" , 
                           bind( &AnaParticle::pt , bind(&AnaHiggs::dilepton_pair,_1) ) > 30. ,
                           bind( &dg::fillh , _1 , 100 , 0 , 100 , bind( &AnaParticle::pt , bind(&AnaHiggs::dilepton_pair,_2) ) , "dilepton system p_{T} (GeV)" ) );
          selector.add_uu( "dphill" , 
                           bind( std::ptr_fun<AnaParticle::ftype,AnaParticle::ftype>(std::abs) , bind( &AnaHiggs::dilepton_dphi,_1) ) < 1.3 ,
                           bind( &dg::fillh , _1 , 96 , -M_PI , M_PI , bind( &AnaHiggs::dilepton_dphi,_2) , "#Delta #phi between leptons (rad)" ) );
          selector.add_uu( "leading_pt" , 
                           bind( &AnaParticle::pt , bind(&AnaHiggs::dilepton_leading_lepton,_1) ) > 20. ,
                           bind( &dg::fillh , _1 , 100 , 0 , 100 , bind( &AnaParticle::pt , bind(&AnaHiggs::dilepton_leading_lepton,_2) ) , "p_{T} (GeV)" ) );
          selector.add_uu( "subleading_pt" , 
                           bind( &AnaParticle::pt , bind(&AnaHiggs::dilepton_subleading_lepton,_1) ) > 15. ,
                           bind( &dg::fillh , _1 , 100 , 0 , 100 , bind( &AnaParticle::pt , bind(&AnaHiggs::dilepton_subleading_lepton,_2) ) , "p_{T} (GeV)" ) );
          // selector.add_uu( "hmt" , 
          //                  bind( &AnaParticle::transverse_mass , _1 ) > 0.75*mhiggs && bind( &AnaParticle::transverse_mass , _1 ) <= 1.*mhiggs ,
          //                  bind( &dg::fillh , _1 , 100 , 0 , 100 , bind( &AnaParticle::transverse_mass,_2) , "higgs transverse mass (GeV)" ) );
          initialized = true;
        }
        // make a higgs candidate out of the lepton and MET truth and see if it passes the peter selection
        shared_ptr<AnaMET> met_particle( new AnaMET );
        met_particle->setup_met_for_truth( sum_missing.Pt() , sum_missing.Phi() );
        // std::vector< boost::shared_ptr<const AnaParticle> > rec_final_states;
        // rec_final_states.push_back( leading_pt_lepton );
        // rec_final_states.push_back( subleading_pt_lepton );
        // rec_final_states.push_back( boost::const_pointer_cast< const AnaMET >(met_particle) );
        shared_ptr<AnaHiggs> cand( new AnaHiggs(leading_pt_lepton,subleading_pt_lepton,boost::const_pointer_cast< const AnaMET >(met_particle)) );
        // check 
        selector.for_only( cand );
      } // do peter selection study
    } // if leading and subleading truth lepton
  } // end do study
protected:
  virtual void _study( type const& coll ) { // the second const is important---matches the base class.
    StudyHwwTruthProducts::study(coll);
    return;
  } // end study of each coll
public:
  StudyHwwTruthProducts( const std::string& name , const std::string& description ) 
    : DataGraphics::DgStudy<type>(name,description)
  {}
  virtual ~StudyHwwTruthProducts() {}
};

#endif // WRAP_STUDYHWWTRUTHPRODUCTS_HH

