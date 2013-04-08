#ifndef WRAP_SIGNEDD0JETTAGGER_HH
#define WRAP_SIGNEDD0JETTAGGER_HH

#include <boost/shared_ptr.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/version.hpp>
#include "TrigFTKAna/JetTagger.hpp"
#include "TrigFTKAna/Likelihood1D.hpp"

class AnaTrack;
//namespace DataGraphics { class DgCollection; }

class
SignedD0JetTagger : public JetTagger
{
public:
protected:
  Likelihood1D _lik_d0_b;
  Likelihood1D _lik_d0_notb;
  Likelihood1D _lik_d0_b_impure;
  Likelihood1D _lik_d0_notb_impure;
  Likelihood1D _lik_z0_b;
  Likelihood1D _lik_z0_notb;
  bool         _use_sign;
  bool         _use_significance;
  bool         _use_track_jet_direction;
  bool         _use_impure_tracks;
  bool         _use_d0; // hack; in general don't use this
  bool         _use_z0;
  bool         _use_z0_significance;
  bool         _use_sv_variables;
  Likelihood1D _lik_vertex_mass_b;
  Likelihood1D _lik_pt_ratio_b;
  Likelihood1D _lik_n_two_track_b;
  Likelihood1D _lik_has_vertex_b;
  Likelihood1D _lik_vertex_mass_notb;
  Likelihood1D _lik_pt_ratio_notb;
  Likelihood1D _lik_n_two_track_notb;
  Likelihood1D _lik_has_vertex_notb;
protected:
  virtual const bool save( const std::string& filename );
  virtual const bool load( const std::string& filename );
  virtual const bool _train( const boost::shared_ptr<const AnaJet>& jet , const std::vector< boost::shared_ptr<const AnaTrack> >& tracks );
  virtual const double _tag( const boost::shared_ptr<const AnaJet>& jet , const std::vector< boost::shared_ptr<const AnaTrack> >& tracks ) const;
private:  
  friend class boost::serialization::access;
  template<typename archiveT>
  void serialize( archiveT& ar , const unsigned int version ) {
    ar & boost::serialization::base_object<JetTagger>(*this);    
    ar & _lik_d0_b;
    ar & _lik_d0_notb;
    ar & _use_sign;
    ar & _use_significance;
    ar & _use_track_jet_direction;
    ar & _use_impure_tracks;
    if( _use_impure_tracks ) {
      ar & _lik_d0_b_impure;
      ar & _lik_d0_notb_impure;
    }
    ar & _use_d0;
    ar & _use_z0;
    ar & _use_z0_significance;
    if( _use_z0 ) {
      ar & _lik_z0_b;
      ar & _lik_z0_notb;
    }
    ar & _use_sv_variables;
    if( _use_sv_variables ) {
      ar & _lik_vertex_mass_b;
      ar & _lik_pt_ratio_b;
      ar & _lik_n_two_track_b;
      ar & _lik_has_vertex_b;
      ar & _lik_vertex_mass_notb;
      ar & _lik_pt_ratio_notb;
      ar & _lik_n_two_track_notb;
      ar & _lik_has_vertex_notb;
    }
  }
public:
  SignedD0JetTagger();
  SignedD0JetTagger( DataGraphics::DgCollection* coll );
  virtual ~SignedD0JetTagger() {}

  SignedD0JetTagger* use_significance( const bool& yes );
  SignedD0JetTagger* use_sign( const bool& yes );
  SignedD0JetTagger* use_track_jet_direction( const bool& yes );
  SignedD0JetTagger* use_impure_tracks( const bool& yes );
  SignedD0JetTagger* use_d0( const bool& yes ) { _use_d0 = yes; }
  SignedD0JetTagger* use_z0( const bool& yes , const bool& yes_significance );
  SignedD0JetTagger* use_sv_variables( const bool& yes );

  TH1F* get_th1_b_d0_likelihood() const { return _lik_d0_b.th1(); }
  TH1F* get_th1_notb_d0_likelihood() const { return _lik_d0_notb.th1(); }
  const Likelihood1D& b_d0_likelihood() const { return _lik_d0_b; }
  const Likelihood1D& notb_d0_likelihood() const { return _lik_d0_notb; }

  TH1F* get_th1_b_impure_d0_likelihood() const { return _lik_d0_b_impure.th1(); }
  TH1F* get_th1_notb_impure_d0_likelihood() const { return _lik_d0_notb_impure.th1(); }
  const Likelihood1D& b_impure_d0_likelihood() const { return _lik_d0_b_impure; }
  const Likelihood1D& notb_impure_d0_likelihood() const { return _lik_d0_notb_impure; }

  // merging
  virtual JetTagger* operator+=( const JetTagger* rhs_base ) {
    const SignedD0JetTagger* rhs( dynamic_cast<const SignedD0JetTagger*>( rhs_base ) );
    assert( _use_sign == rhs->_use_sign );
    assert( _use_significance == rhs->_use_significance );
    assert( _use_track_jet_direction == rhs->_use_track_jet_direction );
    assert( _use_impure_tracks == rhs->_use_impure_tracks );
    assert( _use_d0 == rhs->_use_d0 );
    assert( _use_z0 == rhs->_use_z0 );
    assert( _use_z0_significance == rhs->_use_z0_significance );
    assert( _use_sv_variables == rhs->_use_sv_variables );
    JetTagger::operator+=(rhs_base);
    _lik_d0_b += rhs->_lik_d0_b;
    _lik_d0_notb += rhs->_lik_d0_notb;
    _lik_d0_b_impure += rhs->_lik_d0_b_impure;
    _lik_d0_notb_impure += rhs->_lik_d0_notb_impure;
    _lik_z0_b += rhs->_lik_z0_b;
    _lik_z0_notb += rhs->_lik_z0_notb;
    _lik_vertex_mass_b += rhs->_lik_vertex_mass_b;
    _lik_pt_ratio_b += rhs->_lik_pt_ratio_b;
    _lik_n_two_track_b += rhs->_lik_n_two_track_b;
    _lik_has_vertex_b += rhs->_lik_has_vertex_b;
    _lik_vertex_mass_notb += rhs->_lik_vertex_mass_notb;
    _lik_pt_ratio_notb += rhs->_lik_pt_ratio_notb;
    _lik_n_two_track_notb += rhs->_lik_n_two_track_notb;
    _lik_has_vertex_notb += rhs->_lik_has_vertex_notb;
    return this;
  }

protected:
  friend class AnaTaggerMgr;
  virtual JetTagger* clone() const { return new SignedD0JetTagger(*this); }

};

BOOST_CLASS_VERSION( SignedD0JetTagger , 1 );

#endif // WRAP_SIGNEDD0JETTAGGER_HH
