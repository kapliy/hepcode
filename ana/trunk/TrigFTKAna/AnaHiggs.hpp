
#ifndef ANAHIGGS_HPP
#define ANAHIGGS_HPP

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
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>
#include "DataGraphics/DgSTL.hh"
#include "TrigFTKAna/AnaParticle.hpp"
#include "TrigFTKAna/AnaMET.hpp"
#include "TrigFTKAna/AnaComposite.hpp"
#include "TrigFTKAna/AnaW.hpp"
#include "TrigFTKAna/CompositeGenerator.hpp"
#include "TrigFTKAna/Detector.hpp"

class AnaElectron;
class AnaMuon;
class AnaMET;

class
AnaHiggs : public AnaComposite
{
public:
  typedef AnaParticle::ftype ftype;
  typedef enum { UNKNOWN=0 , WW_DIL , WW_LJETS , ZZ , BB , TAUTAU , GAMMAGAMMA } DecayType;
private:
  DecayType _decay_type;
  mutable std::vector< boost::shared_ptr<const AnaW> > _semileptonic_ws;
  mutable std::vector< boost::shared_ptr<const AnaW> > _hadronic_ws;
  mutable boost::shared_ptr<const AnaComposite> _dilepton_pair;
  mutable boost::shared_ptr<const AnaMET> _dilepton_met;
  mutable boost::shared_ptr<const AnaParticle> _dilepton_leading_lepton;
  mutable boost::shared_ptr<const AnaParticle> _dilepton_subleading_lepton;
  mutable bool _cached_decays;
private:
  friend class boost::serialization::access;
  template<class archiveT>
  void serialize( archiveT& ar, const unsigned int version ) {
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP( AnaComposite );
    ar & boost::serialization::make_nvp("decay_type",_decay_type);
    if( !_cached_decays ) { _classify(); }
  }
  void _classify() const {
    // make full list of all final states involved.
    //
    // if direct constituents are leptons and MET and not W's, this is
    // a dilepton decay reconstruction. if the direct constituents
    // are W's, this is a lepton + jets decay reconstruction, or an
    // all-hadronic reconstruction.
    //
    if( true ) { // debugging sanity check
      unsigned int nleptons = 0u; // numbers of direct constituents
      unsigned int nmet = 0u;
      unsigned int nw = 0u;
      for( AnaComposite::const_iterator i=begin_constituent(), f=end_constituent(); i!=f; ++i ) {
        assert( *i );
        if( (*i)->type() == AnaParticle::ELECTRON || (*i)->type() == AnaParticle::MUON ) { ++nleptons; }
        else if( (*i)->type() == AnaParticle::MET ) { ++nmet; }
        else if( (*i)->type() == AnaParticle::WBOSON ) { ++nw; }
      }
      if( nleptons==2 && nmet==1 ) { assert(_decay_type == WW_DIL); }
      else if( nw == 2 ) { assert( _decay_type == WW_LJETS ); }
    } // end sanity check
    assert( (_decay_type==WW_DIL || _decay_type==WW_LJETS) && "only ww dilepton and lepton+jets decays are implemented!" );
    if( _decay_type==WW_DIL ) {
      _dilepton_leading_lepton.reset();
      _dilepton_subleading_lepton.reset();
      _dilepton_met.reset();
      for( AnaComposite::const_iterator i=begin_constituent(), f=end_constituent(); i!=f; ++i ) {
        const boost::shared_ptr<const AnaParticle>& p( *i );
        if( p->type()==AnaParticle::MET ) { 
          _dilepton_met = boost::dynamic_pointer_cast<const AnaMET>(p);
        } else { // must be lepton
          if( !_dilepton_leading_lepton || _dilepton_leading_lepton->pt() < p->pt() ) {
            _dilepton_subleading_lepton = _dilepton_leading_lepton;
            _dilepton_leading_lepton = p;
          } else if( !_dilepton_subleading_lepton || _dilepton_subleading_lepton->pt() < p->pt() ) {
            _dilepton_subleading_lepton = p;
          }
        }
      }
      assert( _dilepton_leading_lepton && _dilepton_subleading_lepton );
      assert( _dilepton_met );
      _dilepton_pair.reset( new AnaComposite(_dilepton_leading_lepton,_dilepton_subleading_lepton,AnaParticle::GENERIC) );
    } else if( _decay_type==WW_LJETS ) { 
      // get direct decay products
      std::vector< boost::shared_ptr<const AnaW> > decayed_w;
      dgSTL::transform_dynamic_cast<const AnaW>( begin_constituent() , end_constituent() , std::back_inserter(decayed_w) );
      assert( decayed_w.size()==2 && "only H->WW->l+jets currently implemented" );
      _semileptonic_ws.clear();
      _hadronic_ws.clear();
      BOOST_FOREACH( boost::shared_ptr<const AnaW>& w , decayed_w ) {
        if( w->decay_type() == AnaW::SEMILEPTONIC ) {
          _semileptonic_ws.push_back( w );
        } else if( w->decay_type() == AnaW::HADRONIC ) {
          _hadronic_ws.push_back( w );
        } else { assert( !"unhandled case" ); }
      }
    } else { assert( !"unhandled case" ); }
    _cached_decays = true;
  }

  void _compute_transverse_mass() {
    if( !_cached_decays ) { _classify(); }
    _transverse_mass.reset();
    if( _decay_type == WW_DIL ) {
      // compute transverse mass as in arxiv:0902.4864v2
      const ftype visible_mass = _dilepton_pair->mass();
      const ftype eterm = _dilepton_pair->four_vector_for_composites().Pt()*_dilepton_met->pt();
      const ftype pterm = ( (_dilepton_pair->four_vector_for_composites().Px()*_dilepton_met->px()) +
                            (_dilepton_pair->four_vector_for_composites().Py()*_dilepton_met->py()) );
      _transverse_mass.reset( detector::pow<2>(visible_mass) + 2.*(eterm - pterm) );
    } else if( _decay_type == WW_LJETS ) {
      AnaComposite::_compute_transverse_mass();
    }
  }
public:
  AnaHiggs( AnaHiggs::iterator begin , AnaHiggs::iterator end )
    : _decay_type( UNKNOWN )
    , _semileptonic_ws()
    , _hadronic_ws()
    , _dilepton_pair()
    , _dilepton_met()
    , _dilepton_leading_lepton()
    , _dilepton_subleading_lepton()
    , _cached_decays( false )
    , AnaComposite( begin , end , AnaParticle::HIGGS )
  {
    _classify();
    _compute_transverse_mass();
  }
  AnaHiggs( AnaHiggs::const_iterator begin , AnaHiggs::const_iterator end )
    : _decay_type( UNKNOWN )
    , _semileptonic_ws()
    , _hadronic_ws()
    , _dilepton_pair()
    , _dilepton_met()
    , _dilepton_leading_lepton()
    , _dilepton_subleading_lepton()
    , _cached_decays( false )
    , AnaComposite( begin , end , AnaParticle::HIGGS )
  {
    _classify();
    _compute_transverse_mass();
  }
  AnaHiggs( const boost::shared_ptr<const AnaW>& wA , const boost::shared_ptr<const AnaW>& wB ) 
    : _decay_type( WW_LJETS )
    , _semileptonic_ws()
    , _hadronic_ws()
    , _dilepton_pair()
    , _dilepton_met()
    , _dilepton_leading_lepton()
    , _dilepton_subleading_lepton()
    , _cached_decays( false )
    , AnaComposite( wA , wB , AnaParticle::HIGGS )
  {
    _classify();
    _compute_transverse_mass();
  }
  AnaHiggs( const boost::shared_ptr<const AnaParticle>& lepA , const boost::shared_ptr<const AnaParticle>& lepB , 
            const boost::shared_ptr<const AnaParticle>& met )
    : _decay_type( WW_DIL )
    , _semileptonic_ws()
    , _hadronic_ws()
    , _dilepton_pair()
    , _dilepton_met()
    , _dilepton_leading_lepton()
    , _dilepton_subleading_lepton()
    , _cached_decays( false )
    , AnaComposite( lepA , lepB , met , AnaParticle::HIGGS )
  {
    assert( lepA );
    assert( lepB );
    assert( met );
    _classify();
    _compute_transverse_mass();
  }
  AnaHiggs( const AnaHiggs& rhs ) 
    : _decay_type( rhs._decay_type )
    , _semileptonic_ws( rhs._semileptonic_ws )
    , _hadronic_ws( rhs._hadronic_ws )
    , _dilepton_pair( rhs._dilepton_pair )
    , _dilepton_met( rhs._dilepton_met )
    , _dilepton_leading_lepton( rhs._dilepton_leading_lepton )
    , _dilepton_subleading_lepton( rhs._dilepton_subleading_lepton )
    , _cached_decays( rhs._cached_decays )
    , AnaComposite( rhs )
  {
    if( !_cached_decays ) { _classify(); }
  }

  virtual ~AnaHiggs() {}

  const DecayType decay_type() const { return _decay_type; }

  const std::vector< boost::shared_ptr<const AnaW> > semileptonic_ws() const { 
    if( !_cached_decays ) { _classify(); }
    assert( _decay_type==WW_LJETS && "return value is meaningless when type != lepton+jets" );
    return _semileptonic_ws;
  }
  const std::vector< boost::shared_ptr<const AnaW> > hadronic_ws() const { 
    if( !_cached_decays ) { _classify(); }
    assert( _decay_type==WW_LJETS && "return value is meaningless when type != lepton+jets" );
    return _hadronic_ws;
  }

  const ftype dilepton_mass() const {
    if( !_cached_decays ) { _classify(); }
    assert( _decay_type==WW_DIL && "return value is meaningless when type != dilepton" );
    return _dilepton_pair->mass();
  }
  const ftype dilepton_pt() const {
    if( !_cached_decays ) { _classify(); }
    assert( _decay_type==WW_DIL && "return value is meaningless when type != dilepton" );
    return _dilepton_pair->pt();
  }
  const ftype dilepton_dphi() const {
    if( !_cached_decays ) { _classify(); }
    assert( _decay_type==WW_DIL && "return value is meaningless when type != dilepton" );
    return _dilepton_pair->max_delta_phi_opening();
  }
  const boost::shared_ptr<const AnaComposite>& dilepton_pair() const {
    if( !_cached_decays ) { _classify(); }
    assert( _decay_type==WW_DIL && "return value is meaningless when type != dilepton" );
    return _dilepton_pair;
  }
  const boost::shared_ptr<const AnaMET>& dilepton_met() const {
    if( !_cached_decays ) { _classify(); }
    assert( _decay_type==WW_DIL && "return value is meaningless when type != dilepton" );
    return _dilepton_met;
  }
  const boost::shared_ptr<const AnaParticle>& dilepton_leading_lepton() const {
    if( !_cached_decays ) { _classify(); }
    assert( _decay_type==WW_DIL && "return value is meaningless when type != dilepton" );
    return _dilepton_leading_lepton;
  }
  const boost::shared_ptr<const AnaParticle>& dilepton_subleading_lepton() const {
    if( !_cached_decays ) { _classify(); }
    assert( _decay_type==WW_DIL && "return value is meaningless when type != dilepton" );
    return _dilepton_subleading_lepton;
  }

public:

  // commonly useful function objects
  // ================================================================

  // composition
  class Compose {
  public:
    typedef const boost::shared_ptr<const AnaHiggs> result_type;
    template<typename particleAT,typename particleBT>
    result_type
    operator()( const typename boost::shared_ptr<const particleAT>& particleA ,
                const typename boost::shared_ptr<const particleBT>& particleB ) const {
      if( !particleA || !particleB ) { return boost::shared_ptr<const AnaHiggs>(); }
      // only implement H->WW->l+jets for now.
      if( particleA->type() != AnaParticle::WBOSON || particleB->type() != AnaParticle::WBOSON ) { return boost::shared_ptr<const AnaHiggs>(); }
      //
      return boost::make_shared<const AnaHiggs>( particleA , particleB );
    }
    template<typename particleAT,typename particleBT,typename particleCT>
    result_type
    operator()( const typename boost::shared_ptr<const particleAT>& particleA ,
                const typename boost::shared_ptr<const particleBT>& particleB ,
                const typename boost::shared_ptr<const particleCT>& particleC ) const {
      if( !particleA || !particleB || !particleC ) { return boost::shared_ptr<const AnaHiggs>(); }
      // only implement H->WW->ll+MET for now.
      // track leptons will register as "generic"
      const unsigned nleptons = ( static_cast<unsigned int>(particleA->type()==AnaParticle::ELECTRON || particleA->type()==AnaParticle::MUON) +
                                  static_cast<unsigned int>(particleB->type()==AnaParticle::ELECTRON || particleB->type()==AnaParticle::MUON) +
                                  static_cast<unsigned int>(particleC->type()==AnaParticle::ELECTRON || particleC->type()==AnaParticle::MUON) );
      const unsigned ngeneric = ( static_cast<unsigned int>(particleA->type()==AnaParticle::GENERIC) +
                                  static_cast<unsigned int>(particleB->type()==AnaParticle::GENERIC) +
                                  static_cast<unsigned int>(particleC->type()==AnaParticle::GENERIC) );
      const unsigned nmet = ( static_cast<unsigned int>(particleA->type()==AnaParticle::MET) +
                              static_cast<unsigned int>(particleB->type()==AnaParticle::MET) +
                              static_cast<unsigned int>(particleC->type()==AnaParticle::MET) );
      if( (nleptons+ngeneric)!=2 || nmet!=1 ) { return boost::shared_ptr<const AnaHiggs>(); }
      //
      return boost::make_shared<const AnaHiggs>( particleA , particleB , particleC );
    }

  };

};

#endif // ANAHIGGS_HPP
