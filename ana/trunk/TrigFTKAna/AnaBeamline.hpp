 
#ifndef ANABEAMLINE_HPP
#define ANABEAMLINE_HPP

#include <limits>
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/serialization/version.hpp>

class
AnaBeamline
{
public:
  typedef float ftype;
private:
  ftype _x0;
  ftype _y0;
  ftype _z0;
  ftype _sigma_x;
  ftype _sigma_y;
  ftype _sigma_z;
  ftype _cov_xy;
  ftype _tilt_xz;
  ftype _tilt_yz;
  friend class NtWrapper;
private:
  friend class boost::serialization::access;
  template<class archiveT>
  void serialize( archiveT& ar, const unsigned int version ) {
    ar & boost::serialization::make_nvp("x0",_x0);
    ar & boost::serialization::make_nvp("y0",_y0);
    ar & boost::serialization::make_nvp("z0",_z0);
    ar & boost::serialization::make_nvp("sigma_x",_sigma_x);
    ar & boost::serialization::make_nvp("sigma_y",_sigma_y);
    ar & boost::serialization::make_nvp("sigma_z",_sigma_z);
    ar & boost::serialization::make_nvp("cov_xy",_cov_xy);
    ar & boost::serialization::make_nvp("tilt_xz",_tilt_xz);
    ar & boost::serialization::make_nvp("tilt_yz",_tilt_yz);
  }
public:
  AnaBeamline() 
    : _x0( -std::numeric_limits<ftype>::max() )
    , _y0( -std::numeric_limits<ftype>::max() )
    , _z0( -std::numeric_limits<ftype>::max() )
    , _sigma_x( std::numeric_limits<ftype>::max() )
    , _sigma_y( std::numeric_limits<ftype>::max() )
    , _sigma_z( std::numeric_limits<ftype>::max() )
    , _cov_xy( 0. )
    , _tilt_xz( 0. )
    , _tilt_yz( 0. )
  {}
  virtual ~AnaBeamline() {}
  const ftype x0() const { return _x0; }
  const ftype y0() const { return _y0; }
  const ftype z0() const { return _z0; }
  const ftype sigma_x() const { return _sigma_x; }
  const ftype sigma_y() const { return _sigma_y; }
  const ftype sigma_z() const { return _sigma_z; }
  const ftype cov_xy() const { return _cov_xy; }
  const ftype tilt_xz() const { return _tilt_xz; }
  const ftype tilt_yz() const { return _tilt_yz; }
};

BOOST_CLASS_VERSION( AnaBeamline , 1 );

#endif // ANABEAMLINE_HPP
