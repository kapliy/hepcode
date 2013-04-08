#ifndef WRAP_UNIQUEBARCODE_HPP
#define WRAP_UNIQUEBARCODE_HPP

#include <cmath>
#include <boost/tuple/tuple.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/version.hpp>

class
UniqueBarcode
{

private:

  mutable bool _valid;
  int _event_index;
  int _barcode;

private:

  friend class boost::serialization::access;
  template<class archiveT>
  void serialize( archiveT& ar, const unsigned int version ) {
    ar & boost::serialization::make_nvp("valid",_valid);
    ar & boost::serialization::make_nvp("event_index",_event_index);
    ar & boost::serialization::make_nvp("barcode",_barcode);
  }

public:

  UniqueBarcode() : _valid( false ) {}

  UniqueBarcode( const int& event_index , const int& barcode ) 
  : _valid(true) 
  , _event_index( event_index )
  , _barcode( barcode )
  {}

  UniqueBarcode( const signed long& ucode )
  : _valid(true) 
  {
    const boost::tuple<int,int> tmp( barcode_and_event_index(ucode) );
    _barcode = tmp.get<0>();
    _event_index = tmp.get<1>();
  }

  virtual ~UniqueBarcode() {}

  static const boost::tuple<int,int> barcode_and_event_index( const signed long& ucode ) { 
    ldiv_t tmp = std::div( ucode , 100000l );
    return boost::tuple<int,int>(tmp.rem,tmp.quot); 
  }

  const bool is_valid() const { return _valid; }

  const int event_index() const { return _event_index; }
  const int barcode() const { return _barcode; }

  const bool is_primary() const { return _event_index==0; }

  const bool operator==(const UniqueBarcode& rhs) const { return( _valid && rhs._valid && _event_index==rhs._event_index && _barcode==rhs._barcode ); }
  const bool operator!=(const UniqueBarcode& rhs) const { return !(operator==(rhs)); }
  const bool operator<(const UniqueBarcode& rhs) const { return (_event_index == rhs._event_index) ? (_barcode < rhs._barcode) : (_event_index < rhs._event_index); }
  
};

BOOST_CLASS_VERSION( UniqueBarcode , 1 );

#endif // WRAP_UNIQUEBARCODE_HPP
