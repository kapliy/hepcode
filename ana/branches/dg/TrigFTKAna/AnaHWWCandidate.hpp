
#ifndef ANAHWWCANDIDATE_HPP
#define ANAHWWCANDIDATE_HPP

// A wrapper class around AnaW that contains additional event-wide information
// relevant for x-section and ratio analyses (e.g., jets)

#include <cassert>
#include <vector>
#include <iterator>
#include <numeric>
#include <algorithm>
#include <boost/shared_ptr.hpp>
#include <boost/foreach.hpp>
#include <boost/bind.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>
#include "TrigFTKAna/AnaHiggs.hpp"
#include "TrigFTKAna/AnaW.hpp"
#include "TrigFTKAna/AnaJet.hpp"
#include "TrigFTKAna/AnaVertex.hpp"
#include "TrigFTKAna/AnaParticle.hpp"
#include "TrigFTKAna/AnaComposite.hpp"
#include "TrigFTKAna/CompositeGenerator.hpp"

class AnaElectron;
class AnaMuon;
class AnaMET;
class AnaVertex;

class
AnaHWWCandidate : public AnaHiggs
{
public:
  typedef AnaParticle::ftype ftype;
  typedef std::vector< boost::shared_ptr<const AnaJet> > JetColl;
  typedef JetColl::const_iterator jet_const_iterator;
private:
  std::vector< boost::shared_ptr<const AnaParticle> > _event_leptons; // pt-ordered
  std::vector< boost::shared_ptr<const AnaJet> > _event_jets; // pt-ordered
  std::vector< boost::shared_ptr<const AnaMET> > _event_met;
  std::vector< boost::shared_ptr<const AnaVertex> > _event_vertices;
  mutable boost::shared_ptr<const AnaW> _wln; // for l+jets only
  mutable boost::shared_ptr<const AnaW> _wjj; // w->jj for l+jets
  mutable bool _cached;
private:
  friend class boost::serialization::access;
  template<class archiveT>
  void serialize( archiveT& ar, const unsigned int version ) {
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP( AnaHiggs );
    ar & boost::serialization::make_nvp("event_leptons",_event_leptons);
    ar & boost::serialization::make_nvp("event_jets",_event_jets);
    ar & boost::serialization::make_nvp("event_met",_event_met);
    ar & boost::serialization::make_nvp("event_vertices",_event_vertices);
  }
  void _refresh_cache() const {
    if( _cached ) { return; }
    _wln.reset();
    _wjj.reset();
    for( AnaComposite::const_iterator i=begin_constituent(), f=end_constituent(); i!=f; ++i ) {
      const boost::shared_ptr<const AnaW>& w( boost::dynamic_pointer_cast<const AnaW>(*i) ); assert( w );
      if( w->decay_type() == AnaW::SEMILEPTONIC ) {
        if( !_wln ) { _wln = w; }
      } else if( w->decay_type() == AnaW::HADRONIC ) {
        assert( !_wjj );
        _wjj = w;
      } else { assert( !"unhandled type" ); }
    }
    _cached = true;
  }
public:

  // AnaHWWCandidate( const boost::shared_ptr<const AnaW>& wA , const boost::shared_ptr<const AnaW>& wB )
  //   : _leptons()
  //   , _jets()
  //   , _mets()
  //   , _wln()
  //   , _wjj()
  //   , _cached(false)
  //   , AnaHiggs( wA , wB )
  // {
  //   _refresh_cache();
  // }
  AnaHWWCandidate( const boost::shared_ptr<const AnaHiggs>& h )
    : _event_leptons()
    , _event_jets()
    , _event_met()
    , _event_vertices()
    , _wln()
    , _wjj()
    , _cached(false)
    , AnaHiggs( *h )
  {
    _refresh_cache();
  }
  virtual ~AnaHWWCandidate() {}

  const boost::shared_ptr<const AnaW> semileptonic_w() const { _refresh_cache(); assert( _wln ); return _wln; }
  const boost::shared_ptr<const AnaW> hadronic_w() const { _refresh_cache(); assert( _wjj ); return _wjj; }
  const boost::shared_ptr<const AnaW> wln() const { return semileptonic_w(); }
  const boost::shared_ptr<const AnaW> wjj() const { return hadronic_w(); }

  const unsigned int event_nleptons() const { return _event_leptons.size(); }
  const bool exactly_one_event_lepton() const { return event_nleptons()==1; }
  const boost::shared_ptr<const AnaMET> event_met() const { assert( _event_met.size()==1 ); return _event_met.front(); }
  const unsigned int event_njets() const { return _event_jets.size(); }
  jet_const_iterator begin_event_jets() const { return _event_jets.begin(); }
  jet_const_iterator end_event_jets() const { return _event_jets.end(); }
  const unsigned int event_nvertices() const { return _event_vertices.size(); }

  const boost::shared_ptr<const AnaParticle> lepton() const { _refresh_cache(); assert( _wln ); return _wln->lepton(); }
  const boost::shared_ptr<const AnaParticle> met() const { _refresh_cache(); assert( _wln ); return _wln->met(); }
  const AnaParticle::ftype lepton_eta() const { return lepton()->eta(); }
  const AnaParticle::ftype lepton_phi() const { return lepton()->phi(); }
  const AnaParticle::ftype lepton_pt() const { return lepton()->pt(); }
  const bool lepton_is_muon() const {
    const boost::shared_ptr<const AnaMuon> muon = boost::dynamic_pointer_cast<const AnaMuon,const AnaParticle>( lepton() );
    return( muon ? true : false );
  }
  const bool lepton_is_electron() const {
    const boost::shared_ptr<const AnaElectron> electron = boost::dynamic_pointer_cast<const AnaElectron,const AnaParticle>( lepton() );
    return( electron ? true : false );
  }
  const AnaParticle::ftype met_phi() const { return met()->phi(); }
  const AnaParticle::ftype dphi_lepton_met() const { _refresh_cache(); assert( _wln ); return _wln->max_delta_phi_opening(); }
  const unsigned int nmets() const { return _event_met.size(); }
  const bool exactly_one_met() const { return _event_met.size()==1; }
  const AnaParticle::ftype met_reconstructed_pz_low() const {
    const boost::shared_ptr<const AnaParticle> mp = met();
    const boost::shared_ptr<const AnaParticle> lp = lepton();
    return detector::reconstructedMETpz( lp->px() , lp->py() , lp->pz() , mp->px() , mp->py() , 0 );
  }
  const AnaParticle::ftype met_reconstructed_pz_high() const {
    const boost::shared_ptr<const AnaParticle> mp = met();
    const boost::shared_ptr<const AnaParticle> lp = lepton();
    return detector::reconstructedMETpz( lp->px() , lp->py() , lp->pz() , mp->px() , mp->py() , 1 );
  }
  const AnaParticle::ftype met_preferred_pz() const { 
    return met_reconstructed_pz_low();
  }
  const AnaParticle::ftype met_eta( const AnaParticle::ftype pz ) const {
    const boost::shared_ptr<const AnaParticle> mp = met();
    return detector::etaFromPxPyPz( mp->px() , mp->py() , pz );
  }
  TLorentzVector met_reconstructed_four_vector( const AnaParticle::ftype pz ) const {
    TLorentzVector q(0,0,0,0);
    q.SetPtEtaPhiM( met()->pt() , met_eta(pz) , met_phi() , 0. );
    return q;
  }
  const ftype wln_m() const {
    TLorentzVector q( 0 , 0 , 0 , 0 );
    q += lepton()->four_vector_for_composites();
    q += met_reconstructed_four_vector( met_preferred_pz() );
    return q.M();
  }
  const ftype higgs_m() const {
    if( event_njets()>1 ) {
      TLorentzVector q( 0 , 0 , 0 , 0 );
      q += lepton()->four_vector_for_composites();
      q += met_reconstructed_four_vector( met_preferred_pz() );
      q += wjj()->four_vector_for_composites();
      return q.M();
    } else return 0.;
  }
  const ftype tagjj_m() const {
    if( event_njets()>3 ) {
      const boost::shared_ptr<const AnaComposite> tagjj = tagjjcand();
      return tagjj->m();
    } else return 0.;
  }

  template< typename jetIterT >
  static const boost::shared_ptr<const AnaW> find_closest_wjjcand( jetIterT begin , jetIterT end ) {
    if( std::distance(begin,end) < 2 ) { return boost::shared_ptr<const AnaW>(); }
    static CompositeGenerator< const AnaParticle , const AnaW > gen;
    std::vector< boost::shared_ptr<const AnaW> > wjjcands;
    gen.generate( begin , end , begin , end , AnaW::Compose() , wjjcands );
    std::vector< boost::shared_ptr<const AnaW> >::iterator iwjj = std::min_element( wjjcands.begin() , wjjcands.end() , 
                                                                                    AnaParticle::ClosestMassToW<const AnaW>() );
    assert( iwjj != wjjcands.end() );
    return( *iwjj );
  }

  const boost::shared_ptr<const AnaW> find_closest_wjjcand() const {
    return find_closest_wjjcand( _event_jets.begin() , _event_jets.end() );
  }
  const ftype wjj_m() const {
    return wjj()->m();
  }

  // VBF cuts
  const bool wjj_eithercentral() const {
    _refresh_cache();
    if ( std::abs( _wjj->ith_constituent(0)->eta() ) < 1.8 ) return true;
    if ( std::abs( _wjj->ith_constituent(1)->eta() ) < 1.8 ) return true;
    return false;
  }
  const boost::shared_ptr<const AnaComposite> tagjjcand() const {
    static boost::shared_ptr<const AnaComposite> null;
    std::vector< boost::shared_ptr<const AnaComposite> > tagjjcands;
    if( _event_jets.empty() ) { return boost::shared_ptr<const AnaComposite>(); }
    std::vector< boost::shared_ptr<const AnaJet> > jets_by_pt( _event_jets );
    for (unsigned int i=0; i<_event_jets.size(); i++){
      if ( _event_jets[i] == _wjj->ith_constituent(0) || _event_jets[i] == _wjj->ith_constituent(1))
	continue;
      for (unsigned int j=i+1; j<_event_jets.size(); j++){
	if ( _event_jets[j] == _wjj->ith_constituent(0) || _event_jets[j] == _wjj->ith_constituent(1))
	  continue;
	tagjjcands.push_back(boost::make_shared<const AnaComposite>(_event_jets[i],_event_jets[j],AnaParticle::GENERIC));
      }
    }
    return tagjjcands[0];
  }
  const bool tagjj_opposite_hemis() const {
    const boost::shared_ptr<const AnaComposite> tagjj = tagjjcand();
    if ( tagjj->ith_constituent(0)->eta() * tagjj->ith_constituent(1)->eta() < 0 ) return true;
    return false;
  }
  const ftype tagjj_deta() const {
    const boost::shared_ptr<const AnaComposite> tagjj = tagjjcand();
    return std::abs(tagjj->ith_constituent(0)->eta()-tagjj->ith_constituent(1)->eta());
  }
  const bool lepton_eta_between_tagjj() const {
    const boost::shared_ptr<const AnaComposite> tagjj = tagjjcand();
    if ( lepton_eta() < tagjj->ith_constituent(0)->eta() && lepton_eta() > tagjj->ith_constituent(1)->eta() ) return true;
    if ( lepton_eta() > tagjj->ith_constituent(0)->eta() && lepton_eta() < tagjj->ith_constituent(1)->eta() ) return true;
    return false;
  }
  const ftype tagjj_mass() const {
    const boost::shared_ptr<const AnaComposite> tagjj = tagjjcand();
    return tagjj->mass();
  }
  const bool no_add_jets_eta_lt32() const {
    _refresh_cache();
    const boost::shared_ptr<const AnaComposite> tagjj = tagjjcand();
    BOOST_FOREACH( const boost::shared_ptr<const AnaJet>& j , _event_jets ) { 
      if (j == _wjj->ith_constituent(0) || j == _wjj->ith_constituent(1) 
	  || j == tagjj->ith_constituent(0) || j == tagjj->ith_constituent(1) )
	continue;
      if ( std::abs( j->eta() ) < 3.2)
	return false;
    }
    return true;
  }

  // GG cuts
  const bool wjj_j0central() const {
    _refresh_cache();
    if ( std::abs( _wjj->ith_constituent(0)->eta() ) >= 2.8 ) return false;
    return true;
  }  
  const bool wjj_j1central() const {
    _refresh_cache();
    if ( std::abs( _wjj->ith_constituent(1)->eta() ) >= 2.8 ) return false;
    return true;
  }  
  const bool no_btag_event_jet() const {
    return( !(std::find_if( _event_jets.begin() , _event_jets.end() , boost::bind( &AnaJet::tag_sv0_w , _1 ) > 5.72 ) != _event_jets.end() ) );
  }
  const boost::shared_ptr<const AnaJet> leading_jet() const { 
    if( _event_jets.empty() ) { return boost::shared_ptr<const AnaJet>(); }
    std::vector< boost::shared_ptr<const AnaJet> > jets_by_pt( _event_jets );
    return( *jets_by_pt.begin() );
  }
    const boost::shared_ptr<const AnaJet> subleading_jet() const {
        static boost::shared_ptr<const AnaJet> null;
        std::vector< boost::shared_ptr<const AnaJet> > jets_by_pt = _event_jets;
        std::sort( jets_by_pt.begin() , jets_by_pt.end() , bind(&AnaJet::pt,_1) > bind(&AnaJet::pt,_2) );
        const boost::shared_ptr<const AnaJet>& secondjet( *(++(jets_by_pt.begin())) );
        return( !jets_by_pt.empty() ? secondjet : null );
    }
    const boost::shared_ptr<const AnaJet> subsubleading_jet() const {
        static boost::shared_ptr<const AnaJet> null;
        if( event_njets()<=2 ) return null;
        std::vector< boost::shared_ptr<const AnaJet> > jets_by_pt = _event_jets;
        std::sort( jets_by_pt.begin() , jets_by_pt.end() , bind(&AnaJet::pt,_1) > bind(&AnaJet::pt,_2) );
        return *( jets_by_pt.begin() + 2 );
    }
    
    // max deta between leading jet and the next two highest pT jets
    const AnaParticle::ftype max_deta_jets() const {
        if( event_njets()<=1 ) return 0.;
        if( event_njets()<=2 ) return detector::delta_eta( leading_jet()->eta() , subleading_jet()->eta() );
        if( detector::delta_eta( leading_jet()->eta() , subleading_jet()->eta() ) < detector::delta_eta( leading_jet()->eta() , subsubleading_jet()->eta() ) ) {
            return  detector::delta_eta( leading_jet()->eta() , subsubleading_jet()->eta() );
        } 
        return detector::delta_eta( leading_jet()->eta() , subleading_jet()->eta() );
    }
    

  template<typename iteratorLepT,typename iteratorMetT,typename iteratorJetT,typename iteratorVertexT>
  class Compose {
  private:
    iteratorLepT _begin_event_leptons, _end_event_leptons;
    iteratorMetT _begin_mets, _end_mets;
    iteratorJetT _begin_jets, _end_jets;
    iteratorVertexT _begin_vertices, _end_vertices;
  public:
    typedef boost::shared_ptr<const AnaHWWCandidate> result_type;
    Compose( iteratorLepT begin_event_leptons , iteratorLepT end_event_leptons ,
             iteratorMetT begin_mets , iteratorMetT end_mets ,
             iteratorJetT begin_jets , iteratorJetT end_jets ,
             iteratorVertexT begin_vertices , iteratorVertexT end_vertices )
      : _begin_event_leptons(begin_event_leptons), _end_event_leptons(end_event_leptons)
      , _begin_mets(begin_mets), _end_mets(end_mets)
      , _begin_jets(begin_jets), _end_jets(end_jets)
      , _begin_vertices(begin_vertices), _end_vertices(end_vertices)
    {
      assert( begin_event_leptons != end_event_leptons );
      assert( begin_mets != end_mets );
      assert( begin_jets != end_jets );
      assert( begin_vertices != end_vertices );
      assert( std::distance( begin_mets , end_mets ) == 1 );
      assert( std::distance( begin_jets , end_jets ) >=2 );
    }
    result_type operator()( const boost::shared_ptr<const AnaHiggs>& h ) const {
      boost::shared_ptr<AnaHWWCandidate> result( new AnaHWWCandidate(h) ); assert( result );
      result->_event_leptons.assign( _begin_event_leptons , _end_event_leptons );
      result->_event_met.assign( _begin_mets , _end_mets );
      result->_event_jets.assign( _begin_jets , _end_jets );
      result->_event_vertices.assign( _begin_vertices , _end_vertices );
      std::sort( result->_event_leptons.begin() , result->_event_leptons.end() , boost::bind( &AnaParticle::pt , _1 ) > boost::bind( &AnaParticle::pt , _2 ) );
      std::sort( result->_event_jets.begin() , result->_event_jets.end() , boost::bind( &AnaParticle::pt , _1 ) > boost::bind( &AnaParticle::pt , _2 ) );
      // result->_wln.reset( new AnaW( *begin_event_leptons , *begin_mets ) );
      // assert( result->_wln );
      // result->_wjj = find_closest_wjjcand(begin_jets,end_jets);
      return result;
    }
  };

};

#endif // ANAHWWCANDIDATE_HPP
