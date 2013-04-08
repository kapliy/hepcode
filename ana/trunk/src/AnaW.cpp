
#include <cassert>
#include <list>
#include <algorithm>
#include <boost/shared_ptr.hpp>
#include <boost/foreach.hpp>
#include <boost/bind.hpp>
#include <TLorentzVector.h>
#include "TrigFTKAna/AnaParticle.hpp"
#include "TrigFTKAna/AnaTruthParticle.hpp"
#include "TrigFTKAna/AnaElectron.hpp"
#include "TrigFTKAna/AnaMET.hpp"
#include "TrigFTKAna/AnaJet.hpp"
#include "TrigFTKAna/AnaW.hpp"
#include "TrigFTKAna/Detector.hpp"

AnaW::AnaW( AnaW::iterator begin , AnaW::iterator end )
  : AnaComposite( begin , end , AnaParticle::WBOSON )
  , _cached_decays( false )
{
  _classify();
  _compute_transverse_mass();
}

AnaW::AnaW( AnaW::const_iterator begin , AnaW::const_iterator end )
  : AnaComposite( begin , end , AnaParticle::WBOSON )
  , _cached_decays( false )
{
  _classify();
  _compute_transverse_mass();
}

AnaW::AnaW( const boost::shared_ptr<const AnaParticle>& p1 , const boost::shared_ptr<const AnaParticle>& p2 )
  : AnaComposite( p1 , p2 , AnaParticle::WBOSON )
  , _cached_decays( false )
{
  _classify();
  _compute_transverse_mass();
}

// for crosscheck of transverse mass calculation in heavy Q analysis
AnaW::AnaW( const boost::shared_ptr<const AnaParticle>& p1 , const boost::shared_ptr<const AnaParticle>& p2 ,
            const boost::shared_ptr<const AnaParticle>& p3 )
  : AnaComposite( p1 , p2 , p3 , AnaParticle::WBOSON )
  , _cached_decays( false )
{
  _classify();
  _compute_transverse_mass();
}

AnaW::AnaW( const AnaW& rhs ) 
    : _decay_type( rhs._decay_type )
    , _lepton( rhs._lepton )
    , _met( rhs._met )
    , _jets( rhs._jets )
    , _cached_decays( rhs._cached_decays )
    , AnaComposite( rhs )
{
  if( !_cached_decays ) { 
    _classify();
    _compute_transverse_mass();
  }
}

void
AnaW::_classify()
{
  _lepton.reset();
  _met.reset();
  _jets.clear();
  for( const_iterator i=begin_constituent() , f=end_constituent(); i!=f; ++i ) { 
    const boost::shared_ptr<const AnaParticle> p( *i );
    const boost::shared_ptr<const AnaTruthParticle> p_truth = boost::dynamic_pointer_cast<const AnaTruthParticle,const AnaParticle>(p);
    if( p->type()==AnaParticle::ELECTRON || p->type()==AnaParticle::MUON || p->type()==AnaParticle::TAU 
	|| (p->is_truth() && p_truth->is_charged_lepton()) ) {
      assert( !_lepton );
      _lepton = p;
    } else if( p->type()==AnaParticle::MET ) {
      boost::shared_ptr<const AnaMET> nmet( boost::dynamic_pointer_cast<const AnaMET>(p) );
      assert( !_met ); 
      assert( nmet );
      _met = nmet;
    } else if( p->type()==AnaParticle::JET ) {
      boost::shared_ptr<const AnaJet> njet( boost::dynamic_pointer_cast<const AnaJet>(p) );
      _jets.push_back( njet );
    } else if( p->type()==AnaParticle::PHOTON || (p->is_truth() && p_truth->is_photon()) ) {
      // this is used for gettting QCD template from data by tagging photons:
      assert( !_lepton );
      _lepton = p;
    } else { assert( !"unhandled case" ); }
  }
  // hadronic or semileptonic?
  if( _lepton && _met ) {
    _decay_type = SEMILEPTONIC;
  } else if( _jets.size()>=2 ) {
    _decay_type = HADRONIC;
    // pt-order jets
    std::sort( _jets.begin() , _jets.end() , boost::bind( &AnaJet::pt , _1 ) > boost::bind( &AnaJet::pt , _2 ) );
  } else { assert( !"unhandled case"); }
  _cached_decays = true;
}
