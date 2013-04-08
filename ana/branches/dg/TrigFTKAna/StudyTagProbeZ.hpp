#ifndef WRAP_STUDYTAGPROBEZ_HPP
#define WRAP_STUDYTAGPROBEZ_HPP

#include <cmath>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <functional>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/format.hpp>
#include <boost/scope_exit.hpp>
#include <boost/assign/list_of.hpp>
#include "DataGraphics/DgStudy.hh"
#include "DataGraphics/DgCollection.hh"
#include "DataGraphics/DgCutSet.hh"
#include "TrigFTKAna/AnaParticle.hpp"
#include "TrigFTKAna/AnaZ.hpp"
#include "TrigFTKAna/AnaMET.hpp"
#include "TrigFTKAna/AnaMuon.hpp"
#include "TrigFTKAna/AnaElectron.hpp"
#include "TrigFTKAna/Detector.hpp"
#include "TrigFTKAna/StudyParticle.hpp"
#include "TrigFTKAna/StudyMET.hpp"
#include "TrigFTKAna/StudyElectron.hpp"
#include "TrigFTKAna/StudyMuon.hpp"

template< typename tag_type , typename probe_type >
class
StudyTagProbeZ : public DataGraphics::DgStudy< boost::shared_ptr<const AnaZ> >
{
public:
  typedef boost::shared_ptr<const AnaZ> type; // must be same type as the template argument for Dprivate:
  typedef DataGraphics::DgCutSet< typename boost::shared_ptr<tag_type> > TagSelector;
  typedef DataGraphics::DgCutSet< typename boost::shared_ptr<probe_type> > ProbeSelector;
  typedef DataGraphics::DgStudy< typename boost::shared_ptr<tag_type> > TagStudy;
  typedef DataGraphics::DgStudy< typename boost::shared_ptr<probe_type> > ProbeStudy;
private:
  TagSelector* _tag_selector;
  TagStudy* _tag_study;
  std::vector< std::string > _probe_selector_labels;
  std::vector< ProbeSelector* > _probe_selectors;
  ProbeStudy* _probe_study;
protected:
  virtual void _study( type const& z ) { // the second const is important---matches the base class.
    using namespace std;
    using namespace boost;
    using namespace DataGraphics;
    if( !_tag_selector ) { return; }
    // decide on the subcategory
    std::string subcatname = "all";
    // if( z->mass() < 81. ) { subcatname = "low"; }
    // else if( z->mass() <= 101. ) { subcatname = "zwindow"; }
    // else { subcatname = "high"; }
    subcatname = "all";
    dg::down( subcatname , "invariant mass region" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
    //
    // make invariant mass plot etc.
    const boost::shared_ptr<const AnaComposite> cop( boost::dynamic_pointer_cast<const AnaComposite>(z) ); assert( cop );
    if( cop->num_constituents() !=2 ) { return; }
    StudyComposite::study( cop );
    // figure out which leg is the tag and which is the probe. if both
    // qualify as tag legs, take the first as the tag.
    boost::shared_ptr<tag_type> tag_leg;
    boost::shared_ptr<probe_type> probe_leg;
    for( AnaComposite::const_iterator i=cop->begin_constituent(), f=cop->end_constituent(); i!=f; ++i ) {
      boost::shared_ptr<tag_type> tagleg( boost::dynamic_pointer_cast<tag_type>(*i) );
      boost::shared_ptr<probe_type> probeleg( boost::dynamic_pointer_cast<probe_type>(*i) );
      const bool is_tag = tagleg && _tag_selector->passes_all( tagleg );
      if( is_tag && !tag_leg ) { 
        tag_leg = tagleg; continue; 
      }
      // this is not the tag leg 
      assert( !tagleg || tagleg!=tag_leg );
      // is it the probe leg?
      if( probeleg && !probe_leg ) { probe_leg = probeleg; }
    }
    assert( tag_leg );
    assert( probe_leg );
    bool charged = true;
    AnaParticle::ftype q = 1.;
    if( true ) { // determine net charge
      // check that all particles have charge
      for( AnaComposite::const_iterator i=cop->begin_constituent(), f=cop->end_constituent(); i!=f; ++i ) {
        const boost::shared_ptr<const AnaParticle> p( *i );
        // for now, only consider electrons and muons (::charge is defined for both but not necessarily for others)
        if( p->type()!=AnaParticle::ELECTRON && p->type()!=AnaParticle::MUON ) { continue; }
        if( std::abs(p->charge())<0.001 ) { charged = false; break; }
        q *= p->charge();
      }
    }
    // make plots of tag and probe legs
    if( _tag_study ) {
      _tag_study->for_only( tag_leg );
    }
    if( _probe_study ) {
      _probe_study->for_only( probe_leg );
      if( probe_leg->type() == AnaParticle::ELECTRON ) {
        dg::down( "electron" , "probe electron" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
        boost::shared_ptr<const AnaElectron> probe_ele( dynamic_pointer_cast<const AnaElectron>( probe_leg ) );
        assert( probe_ele );
        StudyElectron::study( probe_ele  );
        // check dead FEB from period E/F
        
        if( probe_ele->eta()>=0. && probe_ele->eta()<1.5 &&
            probe_ele->phi()>=-0.9 && probe_ele->phi()<-0.6 ) {
          dg::down( "perEFfeb" , "probe electron in dead FEB" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
          StudyElectron::study( probe_ele );
        }
        // plot invariant mass when passing/failing OTX cut.
        if( probe_ele->pass_otx() ) {
          dg::fillh( "m_probe_passotx", 200 , 0 , 200 , z->mass() , "Invariant Mass (GeV)" );
          if( charged ) { // make os/ss plots
            // plots without cut on os/ss are made by StudyParticle.
            if( q > 0. ) {
              dg::fillh( "m_probe_passotx_ss", 200 , 0 , 200 , z->mass() , "Invariant Mass (GeV)" );
            } else if( q < 0. ) {
              dg::fillh( "m_probe_passotx_os", 200 , 0 , 200 , z->mass() , "Invariant Mass (GeV)" );
            }
          } // end make os/ss plots
        } else {
          dg::fillh( "m_probe_failotx", 200 , 0 , 200 , z->mass() , "Invariant Mass (GeV)" );
          if( charged ) { // make os/ss plots
            // plots without cut on os/ss are made by StudyParticle.
            if( q > 0. ) {
              dg::fillh( "m_probe_failotx_ss", 200 , 0 , 200 , z->mass() , "Invariant Mass (GeV)" );
            } else if( q < 0. ) {
              dg::fillh( "m_probe_failotx_os", 200 , 0 , 200 , z->mass() , "Invariant Mass (GeV)" );
            }
          } // end make os/ss plots
        } // plot pass/fail otx invariant mass
      } else if( probe_leg->type() == AnaParticle::MUON ) {
        dg::down( "muon" , "probe muon" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
        StudyMuon::study( dynamic_pointer_cast<const AnaMuon>( probe_leg ) );
      } else {
        dg::down( "particle" , "probe" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
        StudyParticle::study( dynamic_pointer_cast<const AnaParticle>( probe_leg ) );
      }
    }
    if( !_probe_selectors.empty() ) {
      assert( _probe_selectors.size() == _probe_selector_labels.size() );
      unsigned int n=0u;
      for( typename std::vector< ProbeSelector* >::const_iterator i=_probe_selectors.begin(), f=_probe_selectors.end(); i!=f; ++i,++n ) {
        ProbeSelector* sel( *i );
        if( !sel ) { continue; }
        // disable filling the EventObjectCollection while using the selector for this study.
        const bool tmp = sel->process_attachment();
        sel->process_attachment( false );
        sel->for_only( probe_leg );
        // plot probe selection eff vs dilepton invariant mass (for sideband subtraction)
        assert( n<_probe_selector_labels.size() );
        const bool pass = sel->passes_all( probe_leg );
        if( pass ) { 
          dg::fillh( _probe_selector_labels[n]+"_m" , 200 , 0 , 200 , z->mass() , "Invariant Mass (GeV)" ); 
          if( charged ) { // make os/ss plots
            // plots without cut on os/ss are made by StudyParticle.
            if( q > 0. ) {
              dg::fillh( _probe_selector_labels[n]+"_m_ss" , 200 , 0 , 200 , z->mass() , "Invariant Mass (GeV)" );
            } else if( q < 0. ) {
              dg::fillh( _probe_selector_labels[n]+"_m_os" , 200 , 0 , 200 , z->mass() , "Invariant Mass (GeV)" );
            }
          }
        } // end make os/ss plots
        dg::filleff( _probe_selector_labels[n]+"_eff_vs_m" , 200 , 0 , 200 , z->mass() , pass , "Invariant Mass (GeV)" );
        //
        sel->process_attachment( tmp );
      }
    }
  }
public:
  StudyTagProbeZ( const std::string& name , const std::string& description ) 
    : DataGraphics::DgStudy<type>(name,description)
    , _tag_selector(0)
    , _tag_study(0)
    , _probe_study(0)
  {
    // do not add exclusive subcategories for z window and low, high mass sidebands here.
    // instead, treat the division in the above study function; this allows
    // the user to specify parent categories of which the mass ranges are subcategories.
    // typedef boost::function< bool ( boost::shared_ptr<const AnaZ> const & ) > AnaZBind;
    // add_category( "zwindow" , bind(&AnaZ::mass,_1) > 81. && bind(&AnaZ::mass,_1) < 101.  );
    // add_category( "low" , bind(&AnaZ::mass,_1) < 81. );
    // add_category( "high" , bind(&AnaZ::mass,_1) > 101. );
  }
  void tag_identifier( TagSelector* selector ) { _tag_selector = selector; }
  void tag_study( TagStudy* st ) { _tag_study = st; }
  void probe_study( ProbeStudy* st ) { _probe_study = st; }
  void probe_cut_to_study( const std::string& label , ProbeSelector* selector ) { 
    _probe_selector_labels.push_back( label );
    _probe_selectors.push_back( selector ); 
    assert( _probe_selectors.size() == _probe_selector_labels.size() );
  }
  virtual ~StudyTagProbeZ() {}
};

#endif // WRAP_STUDYTAGPROBEZ_HPP
