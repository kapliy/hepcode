#ifndef ANAVERTEX_HPP
#define ANAVERTEX_HPP

#include <iostream>
#include <limits>
#include <boost/shared_ptr.hpp>
#include <boost/format.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/version.hpp>
#include "TrigFTKAna/tracking/VertexType.hpp"
#include "TrigFTKAna/Detector.hpp"

class
AnaVertex
{
public:
  typedef float ftype;
  typedef ATLAS::VertexType Type;
private:
  ftype _x; // cm
  ftype _y; // cm
  ftype _z; // cm
  ftype _x_err; // cm
  ftype _y_err; // cm
  ftype _z_err; // cm
  Type _type;
  ftype _sum_pt; // gev
  unsigned int _ntracks;
  friend class NtD3PDphysWZ;
  friend class NtD3PDphysEW;
  friend class NtD3PDphysTop;
  friend class NtD3PDphysPeter;
  friend class NtD3PDphysJetMET;
  friend class NtWrapper;
private:
  friend class boost::serialization::access;
  template<class archiveT>
  void serialize( archiveT& ar, const unsigned int version ) {
    ar & boost::serialization::make_nvp("x",_x);
    ar & boost::serialization::make_nvp("y",_y);
    ar & boost::serialization::make_nvp("z",_z);
    ar & boost::serialization::make_nvp("type",_type);
    ar & boost::serialization::make_nvp("sum_pt",_sum_pt);
    ar & boost::serialization::make_nvp("ntracks",_ntracks);
    if( version>=2 ) {
      ar & boost::serialization::make_nvp("x_err",_x_err);
      ar & boost::serialization::make_nvp("y_err",_y_err);
      ar & boost::serialization::make_nvp("z_err",_z_err);
    }
  }
public:
  AnaVertex() 
    : _x( std::numeric_limits<ftype>::max() )
    , _y( std::numeric_limits<ftype>::max() )
    , _z( std::numeric_limits<ftype>::max() )
    , _x_err( std::numeric_limits<ftype>::max() )
    , _y_err( std::numeric_limits<ftype>::max() )
    , _z_err( std::numeric_limits<ftype>::max() )
    , _type( ATLAS::NoVtx )
    , _sum_pt( 0. )
    , _ntracks( 0u )
  {}
  virtual ~AnaVertex() {}
  static const boost::shared_ptr<const AnaVertex>& nullVertex() {
    static boost::shared_ptr<const AnaVertex> null;
    return null;
  }
  const ftype x() const { return _x; }
  const ftype y() const { return _y; }
  const ftype z() const { return _z; }
  const ftype x_err() const { return _x_err; }
  const ftype y_err() const { return _y_err; }
  const ftype z_err() const { return _z_err; }
  const Type type() const { return _type; }
  const std::string& type_string() const { return detector::vertex_type_to_string(_type); }

  const ftype sum_pt() const { return _sum_pt; }
  const unsigned int ntracks() const { return _ntracks; }
  
  // 2010 WZ requirement on primary vertex (relaxed from z=150mm to 200mm)
  const bool is_good_primary_candidate() const {
    return (_type == ATLAS::PriVtx && _ntracks>=3 && fabs(_z)<200); // mm
  }
  
  virtual std::ostream& print( std::ostream& os = std::cout ) const {
    os << boost::format("AnaVertex: type=%|+10| x=%|10| y=%|10| z=%|10| ntracks=%|10|") % type_string() % x() % y() % z() % ntracks() << std::endl;
    return os;
  }

};


BOOST_CLASS_VERSION( AnaVertex , 2 );

#endif // ANAVERTEX_HPP
