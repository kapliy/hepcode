#ifndef ANATRUTHPARTICLE_HPP
#define ANATRUTHPARTICLE_HPP

#include <vector>
#include <string>
#include <iostream>
#include <boost/format.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <TLorentzVector.h>
#include <TDatabasePDG.h>
#include <TParticlePDG.h>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/version.hpp>
#include "TrigFTKAna/AnaParticle.hpp"

class
AnaTruthParticle : public AnaParticle
{
private:
  typedef std::vector<long> IndexList;
  typedef AnaParticle::ftype ftype;
  long _pdg_id;
  int _parent;
  long _parent_pdg_id;
  ftype _charge;
  int _status;
  ftype _vx_z;
  long _particle_event_index;
  long _parent_index;
  mutable const TParticlePDG* _pdg_particle;
  friend class AnaTruthTable;
  friend class NtD3PDphysTop;
  friend class NtD3PDphysPeter;
  friend class NtD3PDphysWZ;
  friend class NtD3PDphysEW;
  friend class NtD3PDphysJetMET;
private:
  friend class boost::serialization::access;
  template<class archiveT>
  void serialize( archiveT& ar, const unsigned int version ) {
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP( AnaParticle );
    ar & boost::serialization::make_nvp("pdg_id",_pdg_id);
    ar & boost::serialization::make_nvp("parent_pdg_id",_parent_pdg_id);
    ar & boost::serialization::make_nvp("parent",_parent);
    ar & boost::serialization::make_nvp("charge",_charge);
    ar & boost::serialization::make_nvp("status",_status);
    ar & boost::serialization::make_nvp("vx_z",_vx_z);
    ar & boost::serialization::make_nvp("particle_event_index",_particle_event_index);
    ar & boost::serialization::make_nvp("parent_index",_parent_index);
  }
public:
  AnaTruthParticle() 
    : _pdg_id( -1 )
    , _parent_pdg_id( -1 )
    , _parent( -1 )
    , _charge( -999 )
    , _status( -1e6 )
    , _vx_z( -1e6 )
    , _particle_event_index( -1 )
    , _parent_index( -1 )
    // , _parents()
    // , _children()
    , _pdg_particle( 0 )
    , AnaParticle( AnaParticle::TRUTH )
  {}
  virtual ~AnaTruthParticle() {}

  virtual const ftype charge() const { return _charge; }
  virtual const short scharge() const { return static_cast<short>(_charge); }
  
  virtual const bool has_truth() const { return true; }
  virtual const boost::shared_ptr<const AnaTruthParticle> truth() const __attribute__((__deprecated__)) { 
    // this object is an AnaTruthParticle; prefer to cast it rather than call truth().
    // create a temporary.
    return( boost::make_shared<const AnaTruthParticle>(*this) );
  }
  
  const long pdg_id() const { return _pdg_id; }
  const long parent_pdg_id() const { return _parent_pdg_id; }
  const int parent() const { return _parent; }
  const std::string pdg_name() const { 
    if( !_pdg_particle ) { _pdg_particle = TDatabasePDG::Instance()->GetParticle(_pdg_id); }
    if( !_pdg_particle ) { return (boost::format("%d")%_pdg_id).str(); }
    return _pdg_particle->GetName();
  }
  const bool is_final_state() const { return _status==1; }
  const bool is_status_pythia_born() const { return _status==3; }
  const bool is_status_herwig_born() const { return _status==123 || _status==124; }
  const bool is_status_equal(int st) const {
    const int STATUS_123_124 = 123124;
    if(st==STATUS_123_124) return is_status_herwig_born();
    return st==_status;
  }
  const bool is_boson() const { 
    return( (std::abs(_pdg_id)>=21 && std::abs(_pdg_id)<=39) );
  }
  const bool is_gluon() const { return( std::abs(_pdg_id)==21 ); }
  const bool is_photon() const { return( std::abs(_pdg_id)==22 ); }
  const bool is_heavy_boson() const { 
    return( (std::abs(_pdg_id)>=23 && std::abs(_pdg_id)<=39) );
  }
  const bool is_w_boson() const { return( std::abs(_pdg_id)==24 ); }
  const bool is_z_boson() const { return( std::abs(_pdg_id)==23 ); }
  const bool is_graviton() const { return( std::abs(_pdg_id)==39 ); }
  const bool is_lepton() const { 
    return( (std::abs(_pdg_id)>=11 && std::abs(_pdg_id)<=19) );
  }
  const bool is_charged_lepton() const { return( is_lepton() && (std::abs(_pdg_id)%2)==1 ); }
  const bool is_electron() const { return( std::abs(_pdg_id)==11 ); }
  const bool is_muon() const { return( std::abs(_pdg_id)==13 ); }
  const bool is_tau() const { return( std::abs(_pdg_id)==15 ); }
  const bool is_neutrino() const { return( is_lepton() && (std::abs(_pdg_id)%2)==0 ); }
  const bool is_electron_neutrino() const { return( std::abs(_pdg_id)==12 ); }
  const bool is_muon_neutrino() const { return( std::abs(_pdg_id)==14 ); }
  const bool is_tau_neutrino() const { return( std::abs(_pdg_id)==16 ); }
  const bool is_lepton_or_heavy_boson() const {
    return is_lepton() || is_heavy_boson();
  }
  const bool is_quark() const {
    return( (std::abs(_pdg_id)>=1 && std::abs(_pdg_id)<=9) );
  }
  const bool is_top_quark() const { return( std::abs(_pdg_id)==6 ); }
  const bool is_bottom_quark() const { return( std::abs(_pdg_id)==5 ); }
  const bool is_charm_quark() const { return( std::abs(_pdg_id)==4 ); }
  const bool is_strange_quark() const { return( std::abs(_pdg_id)==3 ); }
  const bool is_up_quark() const { return( std::abs(_pdg_id)==2 ); }
  const bool is_down_quark() const { return( std::abs(_pdg_id)==1 ); }
  const bool is_light_quark() const { return( std::abs(_pdg_id)<=3 ); }
  const bool is_quark_or_gluon() const { return( is_quark() || _pdg_id==21 ); }
  const bool is_incoming_proton() const {
    return( _particle_event_index<=2 && std::abs(_pdg_id)==2212 );
  }

  const int status() const { return _status; }
  const ftype vx_z() const { return _vx_z; }
  const long index() const { return _particle_event_index; }
  const long parent_index() const { return _parent_index; }
  
  // typedef IndexList::const_iterator index_const_iterator;
  // index_const_iterator begin_parent() const { return _parents.begin(); }
  // index_const_iterator end_parent() const { return _parents.end(); }
  // index_const_iterator begin_child() const { return _children.begin(); }
  // index_const_iterator end_child() const { return _children.end(); }

  virtual std::ostream& print( std::ostream& os ) const {
    os << "AnaTruthParticle: [particle " << index()
       << "] [pdg " << pdg_name()
       << "] [pt " << pt()
       << "] [eta " << (pt()>0. ? eta() : 0.)
       << "] [phi " << phi() 
       << "] [mass " << mass() 
       << "]" << std::endl;
    return os;
  }
  
};

BOOST_CLASS_VERSION( AnaTruthParticle , 1 );

#endif // ANATRUTHPARTICLE_HPP

