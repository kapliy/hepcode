 
#ifndef SERIALIZE_TATT_HPP
#define SERIALIZE_TATT_HPP

#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>
#include <TAttMarker.h>
#include <TAttLine.h>
#include <TAttFill.h>

namespace boost {
  namespace serialization {

    template<class archiveT>
    void serialize( archiveT & ar , TAttMarker& p , const unsigned int version ) {
      Color_t color = p.GetMarkerColor();
      Style_t style = p.GetMarkerStyle();
      Size_t size = p.GetMarkerSize();
      ar & boost::serialization::make_nvp("color",color);
      ar & boost::serialization::make_nvp("style",style);
      ar & boost::serialization::make_nvp("size",size);
      p.SetMarkerColor( color );
      p.SetMarkerStyle( style );
      p.SetMarkerSize( size );
    }

    template<class archiveT>
    void serialize( archiveT & ar , TAttLine& p , const unsigned int version ) {
      Color_t color = p.GetLineColor();
      Style_t style = p.GetLineStyle();
      Width_t width = p.GetLineWidth();
      ar & boost::serialization::make_nvp("color",color);
      ar & boost::serialization::make_nvp("style",style);
      ar & boost::serialization::make_nvp("width",width);
      p.SetLineColor( color );
      p.SetLineStyle( style );
      p.SetLineWidth( width );
    }

    template<class archiveT>
    void serialize( archiveT & ar , TAttFill& p , const unsigned int version ) {
      Color_t color = p.GetFillColor();
      Style_t style = p.GetFillStyle();
      ar & boost::serialization::make_nvp("color",color);
      ar & boost::serialization::make_nvp("style",style);
      p.SetFillColor( color );
      p.SetFillStyle( style );
    }

  } // namespace serialization
} // namespace boost


#endif // SERIALIZE_TATT_HPP
