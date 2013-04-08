#ifndef WRAP_DGGXLEGEND_HH
#define WRAP_DGGXLEGEND_HH

#include <vector>
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/any.hpp>
#include <TPad.h>

class TObject;
class TPad;
class TH1;
// class TAttLine;
// class TAttFill;
// class TAttMarker;
#include <TAttLine.h>
#include <TAttFill.h>
#include <TAttMarker.h>

namespace
DataGraphics
{

  class
  DgGxLegend
  {
    
  public:
    
    typedef enum { LEFT=0, CENTER=1, RIGHT=2 , TOP=0 , BOTTOM=2 , AUTO=3 } PlacementMode;
    typedef boost::shared_ptr<TAttLine> PtrLine;
    typedef boost::shared_ptr<TAttFill> PtrFill;
    typedef boost::shared_ptr<TAttMarker> PtrMarker;
    typedef boost::tuple< PtrLine , PtrFill, PtrMarker , std::string > SymbolEntry;
    typedef enum { LINE=0 , FILL=1 , MARKER=2 , LABEL=3 } SymbolAttribute;
    
  private:
    
    std::vector<const TObject*> _objects;
    
    PlacementMode _placement_vert_mode, _placement_horiz_mode;
    double _pad_x1, _pad_x2, _pad_y1, _pad_y2;
    double _margin_x1, _margin_x2, _margin_y1, _margin_y2;
    double _x1, _x2, _y1, _y2;
    int _fill_color, _fill_style;
    int _text_font, _text_color;
    double _text_size, _text_minimum_size;
    double _entry_margin_fraction;
    int _key_fill_style;
    double _key_ratio;

    TPad* _pad;
    
  protected:
    
    const std::vector< DataGraphics::DgGxLegend::SymbolEntry > translate_to_symbols() const;
    template<typename rootT> const boost::shared_ptr<TAttLine> line_attributes(const rootT* th1) const;
    template<typename rootT> const boost::shared_ptr<TAttFill> fill_attributes(const rootT* th1) const;
    template<typename rootT> const boost::shared_ptr<TAttMarker> marker_attributes(const rootT* th1) const;
    const double u_to_x( const double& u ) const;
    const double v_to_y( const double& v ) const;
    const double x_to_u( const double& x ) const;
    const double y_to_v( const double& y ) const;
    const double get_box_overlap(  const double& bX1 , const double& bX2 , const double& bY1 , const double& bY2 ,
                                   const double& cX1 , const double& cX2 , const double& cY1 , const double& cY2 ) const;
    const double get_th1_overlap( const TPad* pad , const TH1* hist , 
                                  const double& x1 , const double& x2 , const double& y1 , const double& y2 ) const;
    void place_legend( const double& width , const double& height , TPad* pad );
    
    void set_pad_x1( const double& val ) { _pad_x1 = val; }
    void set_pad_y1( const double& val ) { _pad_y1 = val; }
    void set_pad_x2( const double& val ) { _pad_x2 = val; }
    void set_pad_y2( const double& val ) { _pad_y2 = val; }
    void set_margin_x1( const double& val ) { _margin_x1 = val; }
    void set_margin_y1( const double& val ) { _margin_y1 = val; }
    void set_margin_x2( const double& val ) { _margin_x2 = val; }
    void set_margin_y2( const double& val ) { _margin_y2 = val; }
    void set_fill_color( const int& val ) { _fill_color = val; }
    void set_fill_style( const int& val ) { _fill_style = val; }
    void set_text_font( const int& val ) { _text_font = val; }
    void set_text_size( const double& val ) { _text_size = val; }
    void set_text_color( const int& val ) { _text_color = val; }
    void set_text_minimum_size( const double& val ) { _text_minimum_size = val; }
    void set_entry_margin_fraction( const double& val ) { _entry_margin_fraction = val; }
    void set_key_fill_style( const int& val ) { _key_fill_style = val; }
    void set_key_ratio( const double& val ) { _key_ratio = val; }
    void override_placement( const PlacementMode& vert_mode , const PlacementMode& horiz_mode ) {
      _placement_vert_mode = vert_mode;
      _placement_horiz_mode = horiz_mode;
    }
    
  public:
    DgGxLegend( TPad* pad );
    virtual ~DgGxLegend() {}
    void add( const TObject* const object ) { _objects.push_back(object); }
    //void draw( TPad* tpad , std::vector<boost::any>& cleanup_stack ) {}
    void draw( TPad* tpad , std::vector< boost::shared_ptr<boost::any> >& garbage );
  };

};

#endif // WRAP_DGGXLEGEND_HH
