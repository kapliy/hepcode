#ifndef ANAZ_HPP
#define ANAZ_HPP

#include <cmath>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/version.hpp>
#include "TrigFTKAna/AnaComposite.hpp"
#include "TrigFTKAna/AnaElectron.hpp"
#include "TrigFTKAna/AnaMuon.hpp"

class AnaParticle;

class
AnaZ : public AnaComposite
{
private:
  friend class boost::serialization::access;
  template<class archiveT>
  void serialize( archiveT& ar, const unsigned int version ) {
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP( AnaComposite );
  }
public:
  // general case
  AnaZ() : AnaComposite() {}
  AnaZ( iterator begin , iterator end ) : AnaComposite( begin , end , AnaParticle::ZBOSON ) {}
  AnaZ( const_iterator begin , const_iterator end ) : AnaComposite( begin , end , AnaParticle::ZBOSON ) {}
  AnaZ( const boost::shared_ptr<const AnaParticle>& p1 , const boost::shared_ptr<const AnaParticle>& p2 ) : AnaComposite( p1 , p2, AnaParticle::ZBOSON ) {}
  AnaZ( const boost::shared_ptr<const AnaParticle>& p1 , const boost::shared_ptr<const AnaParticle>& p2 ,
        const boost::shared_ptr<const AnaParticle>& p3 )
    : AnaComposite(p1,p2,p3,AnaParticle::ZBOSON) {};
  AnaZ( const AnaZ& rhs ) : AnaComposite( rhs ) {}
  virtual ~AnaZ() {}

  virtual const AnaParticle::ftype max_delta_phi_opening() const { 
    assert( num_constituents()==2 );
    return detector::delta_phi( (*begin_constituent())->phi() , (*(++begin_constituent()))->phi() );
  }
  virtual const AnaParticle::ftype min_delta_phi_opening() const { return max_delta_phi_opening(); }
  virtual const AnaParticle::ftype max_delta_eta_opening() const { 
    assert( num_constituents()==2 );
    return detector::delta_eta( (*begin_constituent())->eta() , (*(++begin_constituent()))->eta() );
  }
  virtual const AnaParticle::ftype min_delta_eta_opening() const { return max_delta_eta_opening(); }

  virtual const AnaParticle::ftype max_delta_r_opening() const { 
    assert( num_constituents()==2 );
    boost::shared_ptr<const AnaParticle> p1( *begin_constituent() );
    boost::shared_ptr<const AnaParticle> p2( *(++begin_constituent()) );
    return detector::delta_eta_phi( p1->eta() , p1->phi() , p2->eta() , p2->phi() );
  }
  virtual const AnaParticle::ftype min_delta_r_opening() const { return max_delta_r_opening(); }


  // highest-pt lepton:
  const boost::shared_ptr<const AnaParticle>& lepton1() const {
    static boost::shared_ptr<const AnaParticle> tmp;
    const_iterator i = AnaComposite::begin_constituent();
    const boost::shared_ptr<const AnaParticle>& p( *i );
    if( p ) {
      assert( p->is_charged_lepton() );
      assert( ((++AnaComposite::begin_constituent())==AnaComposite::end_constituent()) ||
              ((*(++(AnaComposite::begin_constituent())))->pt() <= p->pt()) );
      return p;
    }
    return tmp;
  }
  // the other lepton:
  const boost::shared_ptr<const AnaParticle>& lepton2() const {
    static boost::shared_ptr<const AnaParticle> tmp;
    const_iterator i = AnaComposite::begin_constituent();
    assert( i!=AnaComposite::end_constituent() );
    ++i;
    assert( i!=AnaComposite::end_constituent() );
    const boost::shared_ptr<const AnaParticle>& p( *i ); 
    if( p ) {
      assert( p->is_charged_lepton() );
      assert( *(AnaComposite::begin_constituent()) && 
              (*(AnaComposite::begin_constituent()))->pt() >= p->pt() );
      return p;
    }
    return tmp;
  }
  // lepton by charge:
  const boost::shared_ptr<const AnaParticle>& lepton_q(short q) const {
    static boost::shared_ptr<const AnaParticle> tmp;
    const_iterator i=AnaComposite::begin_constituent();
    const boost::shared_ptr<const AnaParticle>& p( *i );
    if(p && p->scharge()==q) {
      assert( p->is_charged_lepton() );
      return p;
    }
    else {
      i++;
      const boost::shared_ptr<const AnaParticle>& p2( *i );
      if(p2 && p2->scharge() ==q ) {
	assert( p2->is_charged_lepton() );
	return p2;
      }
    }
    return tmp;
  }
  const boost::shared_ptr<const AnaParticle>& lepton_pos() const { return lepton_q(1); }
  const boost::shared_ptr<const AnaParticle>& lepton_neg() const { return lepton_q(-1); }
  const bool opposite_leptons() const {
    return lepton_pos() && lepton_neg();
  }
  const bool samesign_leptons() const {
    return (!opposite_leptons());
  }
  const bool leptons_are_muons() const {
    const boost::shared_ptr<const AnaMuon> muon = boost::dynamic_pointer_cast<const AnaMuon,const AnaParticle>( lepton1() );
    return( muon ? true : false );
  }
  const bool leptons_are_electrons() const {
    const boost::shared_ptr<const AnaElectron> elec = boost::dynamic_pointer_cast<const AnaElectron,const AnaParticle>( lepton1() );
    return( elec ? true : false );
  }
  const AnaParticle::ftype mass_from_id_muons() const {
    const boost::shared_ptr<const AnaMuon> muon1 = boost::dynamic_pointer_cast<const AnaMuon,const AnaParticle>( lepton1() );
    const boost::shared_ptr<const AnaMuon> muon2 = boost::dynamic_pointer_cast<const AnaMuon,const AnaParticle>( lepton2() );
    assert( muon1 && muon2 );
    if( muon1 && muon2) {
      const TLorentzVector zid = muon1->id_muon() + muon2->id_muon();
      return zid.M();
    }
    // fall back to default mass method
    return m();
  }
  const AnaParticle::ftype mass_from_ms_muons() const {
    const boost::shared_ptr<const AnaMuon> muon1 = boost::dynamic_pointer_cast<const AnaMuon,const AnaParticle>( lepton1() );
    const boost::shared_ptr<const AnaMuon> muon2 = boost::dynamic_pointer_cast<const AnaMuon,const AnaParticle>( lepton2() );
    assert( muon1 && muon2 );
    if( muon1 && muon2) {
      const TLorentzVector zms = muon1->ms_muon() + muon2->ms_muon();
      return zms.M();
    }
    // fall back to default mass method
    return m();
  }
  const AnaParticle::ftype mass_from_exms_muons() const {
    const boost::shared_ptr<const AnaMuon> muon1 = boost::dynamic_pointer_cast<const AnaMuon,const AnaParticle>( lepton1() );
    const boost::shared_ptr<const AnaMuon> muon2 = boost::dynamic_pointer_cast<const AnaMuon,const AnaParticle>( lepton2() );
    assert( muon1 && muon2 );
    if( muon1 && muon2) {
      const TLorentzVector zexms = muon1->exms_muon() + muon2->exms_muon();
      return zexms.M();
    }
    // fall back to default mass method
    return m();
  }
  const AnaParticle::ftype lepton_pos_eta() const {
    const boost::shared_ptr<const AnaParticle> lp = lepton_pos();
    return lp ? lp->eta() : std::numeric_limits<AnaParticle::ftype>::max();
  }
  const AnaParticle::ftype lepton_neg_eta() const {
    const boost::shared_ptr<const AnaParticle> lp = lepton_neg();
    return lp ? lp->eta() : std::numeric_limits<AnaParticle::ftype>::max();
  }

  const bool muon_eta_cut() const { 
    if( !leptons_are_muons() ) return true;
    const boost::shared_ptr<const AnaMuon> muon1 = boost::dynamic_pointer_cast<const AnaMuon,const AnaParticle>( lepton1() );
    const boost::shared_ptr<const AnaMuon> muon2 = boost::dynamic_pointer_cast<const AnaMuon,const AnaParticle>( lepton2() );
    if( !muon1 || !muon2 ) return false;
    return std::abs(muon1->eta())<2.4 && std::abs(muon2->eta())<2.4;
  }
  const bool lepton_pt_cut() const {
    return lepton1()->pt() > 50. || lepton2()->pt() > 50.;
  }


public:

  // commonly useful function objects
  // ================================================================

  // Z composition from two leptons
  class Compose {
  public:
    template<typename leptonT>
    const boost::shared_ptr<const AnaZ>
    operator()( const typename boost::shared_ptr<const leptonT>& lepton1 ,
                const typename boost::shared_ptr<const leptonT>& lepton2 ) const {
      if( !lepton1 || !lepton2 ) { return boost::shared_ptr<const AnaZ>(); }
      // sort leptons by pt
      if( lepton1->pt()>=lepton2->pt() ) {
        return boost::make_shared<const AnaZ>( lepton1 , lepton2 );
      } else {
        return boost::make_shared<const AnaZ>( lepton2 , lepton1 );
      }
    }
  };

};

BOOST_CLASS_VERSION( AnaZ , 1 );

#endif // ANAZ_HPP

