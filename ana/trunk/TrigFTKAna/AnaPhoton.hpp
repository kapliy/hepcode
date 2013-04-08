#ifndef WRAP_ANAPHOTON_HPP
#define WRAP_ANAPHOTON_HPP

// ANAPHOTON
// ================================================================
// Data for a single photon.
// ================================================================
// 2010-12-19 Joseph Tuggle (jtuggle@uchicago.edu)

#include <numeric>
#include <cmath>
#include <string>
#include <iostream>
#include <boost/format.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/version.hpp>
#include "TrigFTKAna/AnaParticle.hpp"
#include "TrigFTKAna/Detector.hpp"

class
AnaPhoton : public AnaParticle
{
public:
  typedef AnaParticle::ftype ftype;
  typedef enum { CONE10=0 , CONE20=1, CONE30=2, CONE40=3 , NSIZES=4 } ConeSize;
protected:

  int _author;
  unsigned int _isEM;
  unsigned int _isgoodoq;
  unsigned int _oq;
  unsigned int _aod_quality;
  int _is_recovered;
  int _convFlag;
  ftype _rconv;
  ftype _zconv;
  ftype _etas2;
  ftype _ethad1;
  ftype _ethad;
  ftype _e277;
  ftype _e237;
  ftype _e233;
  ftype _weta2;
  ftype _f1;
  ftype _emax;
  ftype _emaxs2;
  ftype _demaxs1;
  ftype _fside;
  ftype _wstot;
  ftype _ws3;
  ftype _emins1;
  ftype _etap;
  std::vector<ftype> _pt_cone; // inner detector pt in a given cone
  std::vector<ftype> _et_cone; // calo et in a given cone (corrected)
  std::vector<ftype> _uncorrected_et_cone; // calo et in a given cone (uncorrected)
  TLorentzVector _cluster_momentum;
  int _ntuple_index;
  mutable boost::shared_ptr<const AnaPhoton> _uncorrected_parent;
  friend class NtD3PDphysPeter;
  friend class NtD3PDphysWZ;
  friend class NtD3PDphysEW;
  friend class NtD3PDphysTop;
  friend class NtPGS;
private:
  friend class boost::serialization::access;
  template<class archiveT>
  void serialize( archiveT& ar, const unsigned int version ) {
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(AnaParticle);
    ar & boost::serialization::make_nvp("author",_author);
    ar & boost::serialization::make_nvp("isEM",_isEM);
    ar & boost::serialization::make_nvp("isgoodoq",_isgoodoq);
    ar & boost::serialization::make_nvp("oq",_oq);
    ar & boost::serialization::make_nvp("aod_quality",_aod_quality);
    ar & boost::serialization::make_nvp("is_recovered",_is_recovered);
    ar & boost::serialization::make_nvp("convFlag",_convFlag);
    ar & boost::serialization::make_nvp("rconv",_rconv);
    ar & boost::serialization::make_nvp("zconv",_zconv);
    ar & boost::serialization::make_nvp("etas2",_etas2);
    ar & boost::serialization::make_nvp("ethad1",_ethad1);
    ar & boost::serialization::make_nvp("ethad",_ethad);
    ar & boost::serialization::make_nvp("e277",_e277);
    ar & boost::serialization::make_nvp("e237",_e237);
    ar & boost::serialization::make_nvp("e233",_e233);
    ar & boost::serialization::make_nvp("weta2",_weta2);
    ar & boost::serialization::make_nvp("f1",_f1);
    ar & boost::serialization::make_nvp("emax",_emax);
    ar & boost::serialization::make_nvp("emaxs2",_emaxs2);
    ar & boost::serialization::make_nvp("demaxs1",_demaxs1);
    ar & boost::serialization::make_nvp("fside",_fside);
    ar & boost::serialization::make_nvp("wstot",_wstot);
    ar & boost::serialization::make_nvp("ws3",_ws3);
    ar & boost::serialization::make_nvp("emins1",_emins1);
    ar & boost::serialization::make_nvp("etap",_etap);
    ar & boost::serialization::make_nvp("pt_cone",_pt_cone);
    ar & boost::serialization::make_nvp("et_cone",_et_cone);
    ar & boost::serialization::make_nvp("uncorrected_et_cone",_uncorrected_et_cone);
    ar & boost::serialization::make_nvp("cluster_momentum",_cluster_momentum);
    ar & boost::serialization::make_nvp("ntuple_index",_ntuple_index);
  }
public:
  AnaPhoton()
  
    : _author(0)
    , _isEM(0u)
    , _isgoodoq(0u)
    , _oq(0u)
    , _aod_quality(0u)
    , _is_recovered(0)
    , _convFlag(0)
    , _rconv(0)
    , _zconv(0)
    , _etas2(0)
    , _ethad1(0)
    , _ethad(0)
    , _e277(0)
    , _e237(0)
    , _e233(0)
    , _weta2(0)
    , _f1(0)
    , _emax(0)
    , _emaxs2(0)
    , _demaxs1(0)
    , _fside(0)
    , _wstot(0)
    , _ws3(0)
    , _emins1(0)
    , _etap(0)
    , _pt_cone()
    , _et_cone()
    , _uncorrected_et_cone()
    , _cluster_momentum()
    , _uncorrected_parent()
    , _ntuple_index(-1)
    , AnaParticle(AnaParticle::PHOTON) 
  {}
  virtual ~AnaPhoton() {}

  const int ntuple_index() const { return _ntuple_index; } // location in ntuple photon collection, for deterministic random number seeds etc.
  
  const int author() const { return _author; }
  const unsigned int isEM() const { return _isEM; }
  const unsigned int is_em() const __attribute__((deprecated)) { return isEM(); }
  virtual const TLorentzVector& four_vector_for_composites() const { return four_vector(); }
  virtual const ftype charge() const { return 0.; }
  virtual const short scharge() const __attribute__((deprecated)) { return 0; }
  const ftype cluster_energy() const { return _cluster_momentum.E(); }
  const ftype cluster_pt() const { return _cluster_momentum.Pt(); }
  const ftype cluster_phi() const { return _cluster_momentum.Phi(); }
  const ftype cluster_eta() const { return _cluster_momentum.Eta(); }

  const unsigned int isgoodoq() const { return _isgoodoq; }
  const bool good_oq() const { return _oq & 34214; }
  const unsigned int oq() const { return _oq; }
  const unsigned int aod_quality() const { return _aod_quality; }
  const bool aod_loose() const { return _aod_quality & 1; } // peter ntuple v126
  const bool aod_tight() const { return _aod_quality & 2; }
  const int is_recovered() const { return _is_recovered; }
  const int convFlag() const { return _convFlag; }
  const ftype rconv() const { return _rconv; }
  const ftype zconv() const { return _zconv; }
  const ftype etas2() const { return _etas2; }
  const ftype eta2() const { return _etas2; }
  const ftype phetas2() const { return _etas2; }
  const ftype ethad1() const { return _ethad1; }
  const ftype ethad() const { return _ethad; }
  const ftype et2() const { std::abs(cosh(eta2()))<100 ? cluster_energy()/cosh(eta2()) : 0; }
  const ftype reta() const { _e277 ? _e237 / _e277 : 0; }
  const ftype rphi() const { _e237 ? _e233 / _e237 : 0; }
  const ftype rhad1() const { et2() ? ethad1()/et2() : 0; }
  const ftype rhad() const { et2() ? ethad()/et2() : 0; }
  const ftype phe277() const { return _e277; }
  const ftype phe237() const { return _e237; }
  const ftype phe233() const { return _e233; }
  const ftype phweta2() const { return _weta2; }
  const ftype phf1() const { return _f1; }
  const ftype phemax() const { return _emax; }
  const ftype phemaxs2() const { return _emaxs2; }
  const ftype phdemaxs1() const { return _demaxs1; }
  const ftype phfside() const { return _fside; }
  const ftype phfracs1() const { return _fside; }
  const ftype phwstot() const { return _wstot; }
  const ftype phws3() const { return _ws3; }
  const ftype phemins1() const { return _emins1; }
  const ftype phetap() const { return _etap; }

  // isolation
  const ftype pt_cone( const ConeSize& cone ) const { return _pt_cone[cone]; }
  const ftype pt_cone_ratio( const ConeSize& cone ) const { return detector::safe_divide( pt_cone(cone) , pt() ); }
  const ftype et_cone( const ConeSize& cone ) const { return _et_cone[cone]; }
  const ftype et_cone_ratio( const ConeSize& cone ) const { return detector::safe_divide( et_cone(cone) , pt() ); }
  const ftype uncorrected_et_cone( const ConeSize& cone ) const { return _uncorrected_et_cone[cone]; }
  const ftype uncorrected_et_cone_ratio( const ConeSize& cone ) const { return detector::safe_divide( uncorrected_et_cone(cone) , pt() ); }
  
  // recommended photon corrections
  const bool is_corrected() const { return _uncorrected_parent; }
  const boost::shared_ptr<const AnaPhoton> uncorrected_parent() const { return _uncorrected_parent; }
  static boost::shared_ptr<const AnaPhoton> cluster_photon_v16( const boost::shared_ptr<const AnaPhoton>& phot );
  static boost::shared_ptr<const AnaPhoton> corrected_cluster_photon_v16( const boost::shared_ptr<const AnaPhoton>& phot ,
                                                                          const unsigned long& event_number ,
                                                                          const size_t& phot_number ,
                                                                          const int& corrtype = 0 );
  static boost::shared_ptr<const AnaPhoton> smeared_cluster_photon_v16( const boost::shared_ptr<const AnaPhoton>& phot , 
                                                                        const unsigned long& event_number ,
                                                                        const size_t& phot_number ,
                                                                        const int& corrtype = 0 );
  static boost::shared_ptr<const AnaPhoton> corrected_smeared_cluster_photon_v16( const boost::shared_ptr<const AnaPhoton>& phot ,
                                                                                  const unsigned long& event_number ,
                                                                                  const size_t& phot_number ,
                                                                                  const int& corrtype = 0 );


  virtual std::ostream& print( std::ostream& os = std::cout ) const {
    os << boost::format("AnaPhoton: et=%|+10|(GeV) eta=%|10| phi=%|10|") % et() % eta() % phi() << std::endl;
    return os;
  }

};

BOOST_CLASS_VERSION( AnaPhoton , 3 );

#endif // WRAP_ANAPHOTON_HPP
