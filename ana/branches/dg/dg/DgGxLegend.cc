
#include <cassert>
#include <cmath>
#include <vector>
#include <string>
#include <set>
#include <iostream>
#include <algorithm>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/scope_exit.hpp>
#include <boost/foreach.hpp>
#include <boost/bind.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/any.hpp>
#include <TObject.h>
#include <TPad.h>
#include <TVirtualPad.h>
#include <TH1.h>
#include <TH2.h>
#include <TGraph.h>
#include <TAttLine.h>
#include <TAttFill.h>
#include <TAttMarker.h>
#include <TLatex.h>
#include <TFrame.h>
#include <TList.h>
#include <TBox.h>
#include <THStack.h>
#include "DataGraphics/DgGxLegend.hh"

using namespace std;
using namespace boost;

const std::vector< DataGraphics::DgGxLegend::SymbolEntry >
DataGraphics::DgGxLegend::translate_to_symbols() const
{
  vector<SymbolEntry> results;
  BOOST_FOREACH( const TObject* object , _objects ) {
    // get draw attributes and label
    shared_ptr<TAttLine> att_line;
    shared_ptr<TAttFill> att_fill;
    shared_ptr<TAttMarker> att_marker;
    string label;
    const TH1* th1 = dynamic_cast<const TH1*>( object );
    const TGraph* tgr = dynamic_cast<const TGraph*>( object );
    if( th1 ) {
      att_line = line_attributes( th1 );
      att_fill = fill_attributes( th1 );
      att_marker = marker_attributes( th1 );
      label = th1->GetXaxis()->GetTitle();
    } else if( tgr ) {
      att_line = line_attributes( tgr );
      att_fill = fill_attributes( tgr );
      att_marker = marker_attributes( tgr );
      label = tgr->GetXaxis()->GetTitle();
    }
    results.push_back( make_tuple(att_line,att_fill,att_marker,label) );
  }
  return results;
}


template<class typeT>
const boost::shared_ptr<TAttLine>
DataGraphics::DgGxLegend::line_attributes(const typeT* th1) const
{
  boost::shared_ptr<TAttLine> result( new TAttLine(*dynamic_cast<const TAttLine*>(th1)) );
  return result;
}

template<class typeT>
const boost::shared_ptr<TAttFill>
DataGraphics::DgGxLegend::fill_attributes(const typeT* th1) const
{
  boost::shared_ptr<TAttFill> result( new TAttFill(*dynamic_cast<const TAttFill*>(th1)) );
  return result;
}


template<class typeT>
const boost::shared_ptr<TAttMarker>
DataGraphics::DgGxLegend::marker_attributes(const typeT* th1) const
{
  boost::shared_ptr<TAttMarker> result( new TAttMarker(*dynamic_cast<const TAttMarker*>(th1)) );
  return result;
}


DataGraphics::DgGxLegend::DgGxLegend( TPad* pad )
{
  _pad_x1 = 0.51;
  _pad_x2 = 0.9;
  _pad_y1 = 0.51;
  _pad_y2 = 0.9;
  _margin_x1 = 0.1;
  _margin_x2 = 0.1;
  _margin_y1 = 0.1;
  _margin_y2 = 0.1;
  _placement_horiz_mode = RIGHT;
  _placement_vert_mode = TOP;
  _fill_color = 0;
  _fill_style = 1001;
  _key_fill_style = 1001;
  _key_ratio = 0.9;
  _entry_margin_fraction = 0.05;
  _text_font = 62;
  _text_size = 0.05;
  _text_minimum_size = 0.02;
  _text_color = 12;
  _pad = pad;
}


void
DataGraphics::DgGxLegend::draw( TPad* pad , std::vector< boost::shared_ptr<boost::any> >& garbage )
{
  return;
  const bool debug = true;
  // coordinates:
  // (u,v) = pad coordinates (i.e. 0->500 GeV, 0->1000 events)
  // (x,y) = NDC coordinates (0-1)
  // (P,Q) = pixel coordinates
  ////////////////////////////////////////////////////////////////
  if( !pad ) { cout << "no pad to draw on" << endl; return; }
  // validate input
  if( _margin_x1 < 0 ) { cout << "bad marginX1" << endl; return; }
  if( _margin_y1 < 0 ) { cout << "bad marginY1" << endl; return; }
  if( _margin_x2 < 0 ) { cout << "bad marginX2" << endl; return; }
  if( _margin_y2 < 0 ) { cout << "bad marginY2" << endl; return; }
  if( (_margin_x1+_margin_x2)>1.0 ) { cout << "marginX1+marginX2 > 1." << endl; return; }
  if( (_margin_y1+_margin_y2)>1.0 ) { cout << "marginY1+marginY2 > 1." << endl; return; }
  // set margins
  const double useable_height_fraction = 1.-_entry_margin_fraction;
  const double useable_width_fraction = 0.95;
  const double text_box_margin_x = 0.05;
  const double text_box_margin_y = 0.04;
  const double n_entries_d = static_cast<double>( _objects.size() );
  // legend text
  scoped_ptr<TLatex> latex( new TLatex() );
  if( !latex ) { return; }
  // translate the input into a set of legend symbol properties
  const std::vector< DataGraphics::DgGxLegend::SymbolEntry > symbols( translate_to_symbols() );
  // figure out labels for each symbol
  // ================================================================
  vector<string> labels;
  for( vector< SymbolEntry >::const_iterator i=symbols.begin(), f=symbols.end(); i!=f; ++i ) {
    const SymbolEntry& sy( *i );
    labels.push_back( sy.get<LABEL>() );
  }
  vector< set<string> > tokens_for_label;
  BOOST_FOREACH( const string label , labels ) {
    set<string> tokens;
    boost::algorithm::split( tokens , label , boost::algorithm::is_any_of(" ") );
    tokens_for_label.push_back( tokens );
  }
  // set<string> common_tokens;
  // if( !tokens_for_label.empty() ) { common_tokens = tokens_for_label.front(); }
  // BOOST_FOREACH( const set<string> label_tokens , tokens_for_label ) {
  //   set<string> tmp;
  //   std::set_union( common_tokens.begin() , common_tokens.end() , label_tokens.begin() , label_tokens.end() , inserter(tmp,tmp.begin()) );
  //   common_tokens.swap( tmp );
  // }
  // // remove common tokens from each label
  // for( vector<string>::iterator i=labels.begin(), f=labels.end(); i!=f; ++i ) {
  //   string& label( *i );
  //   BOOST_FOREACH( const string common_token , common_tokens ) {
  //     boost::algorithm::replace_all( label , common_token , "" );
  //   }
  //   // trim extra whitespace (incl. internal whitespace)
  //   boost::algorithm::trim( label );
  //   while( boost::algorithm::find_first( label , "  " ) ) { boost::algorithm::replace_all( label , "  " , " " ); }
  // }
  // get maximum text height for any given line
  latex->SetTextSize( _text_size );
  latex->SetTextFont( _text_font );
  latex->SetTextColor( _text_color );
  double text_width_u = 0.;
  double text_height_v = 0.;
  // get maximum text width and height
  BOOST_FOREACH( const string label , labels ) {
      latex->SetText( 0 , 0 , label.c_str() );
      text_width_u = std::max( latex->GetXsize() , text_width_u );
      text_height_v = std::max( latex->GetYsize() , text_height_v );
    }
  if( debug ) { 
    cout << "text_width_u="<< text_width_u << endl;
    cout << "text_height_v="<< text_height_v << endl;
  }
  // assume extremely large legend
  double legendWidthX = 0.5;
  double legendHeightY = 1.0;
  double legendHeightV = y_to_v(1.0) - y_to_v(0.0);
  // attempt to resize if text height too big for legend
  while( (n_entries_d*text_height_v/useable_height_fraction) > legendHeightV )
    {
      _text_size = _text_size*0.75;
      if( _text_size < _text_minimum_size ) { break; }
      latex->SetTextSize( _text_size );
      text_width_u = latex->GetXsize();
      text_height_v = latex->GetYsize();
    }
  // set entry size
  double textHeightY = v_to_y(std::abs(text_height_v)) - v_to_y(0);
  double textWidthX = u_to_x(std::abs(text_width_u)) - u_to_x(0);
  double entryHeightY = (textHeightY/(1.-(text_box_margin_y*2.)))  / useable_height_fraction;
  double keySizeXY = entryHeightY*useable_height_fraction;
  double keyLabelSpacingX = keySizeXY*0.2;
  double entryWidthX = ( (textWidthX*(1.+(text_box_margin_x*10.)))+keyLabelSpacingX+keySizeXY) / useable_width_fraction;
  if( debug ) {
    cout << "textHeightY="<< textHeightY<<" U"<<y_to_v(textHeightY) - y_to_v(0) << endl;
    cout << "entryWidthX="<< entryWidthX<<" U"<<x_to_u(entryWidthX) - x_to_u(0) << endl;
    cout << "entryHeightY="<< entryHeightY<<" U"<<y_to_v(entryHeightY) - y_to_v(0) << endl;
    // cout << "entryOffsetX="<< entryOffsetX<<" U"<<entryOffsetX*padWidth << endl;
    // cout << "entryOffsetY="<< entryOffsetY<<" U"<<entryOffsetY*padHeight << endl;
  }
  double neededLegendHeightY = entryHeightY*n_entries_d;
  double neededLegendWidthX = entryWidthX + keyLabelSpacingX;
  if( debug ) { 
    cout << "neededLegendWidthX=" << neededLegendWidthX << " U="<< x_to_u(neededLegendWidthX) - x_to_u(0) << endl;
    cout << "neededLegendHeightY=" << neededLegendHeightY << " V="<< y_to_v(neededLegendHeightY) -y_to_v(0) << endl;
  }
  // place legend
  place_legend( neededLegendWidthX , neededLegendHeightY , pad );
  // compute entry offset
  double entryOffsetX = (1.-useable_width_fraction)*entryWidthX / 2.;
  double entryOffsetY = (1.-useable_height_fraction)*entryHeightY / 2.;
  //
  if( debug ) { 
    cout << "legendWidthX=" << legendWidthX<<" U"<<x_to_u(legendWidthX) -x_to_u(0) << endl;
    cout << "legendHeightY="<< legendHeightY<<" U"<<y_to_v(legendHeightY) -y_to_v(0) << endl;
  }
  // legend bounding box
  if( debug ) { 
    cout << "x1U " << _x1 << " " << x_to_u(_x1) << endl;
    cout << "x2U " << _x2 << " " << x_to_u(_x2) << endl;
    cout << "y1U " << _y1 << " " << y_to_v(_y1) << endl;
    cout << "y2U " << _y2 << " " << y_to_v(_y2) << endl;
  }
//   if( (_fill_style != ROOT_FILL_HOLLOW) && (_fillStyle!=ROOT_FILL_TRANSPARENT) ) {
//       TBox* box = new TBox( x_to_u(_x1) , y_to_v(_y1) , x_to_u(_x2) , y_to_v(_y2) );
//       if( !box ) { DeletePrimitives(); return; }
//       garbage.push_back( box );
//       box->SetFillColor( _fill_color );
//       box->SetFillStyle( _fill_style );
//       box->SetLineColor( _fill_color );
//       box->SetLineStyle( ROOT_LINE_SOLID );
//       box->SetLineWidth( 0 );
//       box->Draw();
//     }
  //
  // make legend keys and text for each entry
  //assert( labels.size() == _objects.size() );
  assert( labels.size() == symbols.size() );
  for( unsigned int i=0, f=symbols.size(); i!=f; ++i ) {
    shared_ptr<TAttLine> att_line( (symbols[i].get<LINE>()) );
    shared_ptr<TAttFill> att_fill( (symbols[i].get<FILL>()) );
    shared_ptr<TAttMarker> att_marker( (symbols[i].get<MARKER>()) );
    
    const string& label = labels[i];
    double dI = static_cast<double>(i);
    // define regions
    double xMinTotalX = _x1;
    double xMaxTotalX = _x1 + entryWidthX;
    double yMinTotalY = _y1 + (dI*entryHeightY);
    double yMaxTotalY = _y1 + ((dI+1.0)*entryHeightY);
    double keyOffsetY = (yMaxTotalY - yMinTotalY - keySizeXY) / 2.;
    if( keyOffsetY<entryOffsetY ) { keyOffsetY = entryOffsetY; }
    double xMinKeyX = xMinTotalX + entryOffsetX;
    double yMinKeyY = yMinTotalY + keyOffsetY;
    double xMaxKeyX = xMinKeyX + keySizeXY;
    double yMaxKeyY = yMaxTotalY - keyOffsetY;
    double keyWidthX = xMaxKeyX - xMinKeyX;
    double keyHeightY = yMaxKeyY - yMinKeyY;
    //
    double labelOffsetX = text_box_margin_x*(entryWidthX-keyWidthX-keyLabelSpacingX);
    double xMinLabelX = xMaxKeyX + keyLabelSpacingX + labelOffsetX;
    double yMinLabelY = yMinTotalY + entryOffsetY + (text_box_margin_y*entryHeightY);
    double xMaxLabelX = xMaxTotalX - entryOffsetX - labelOffsetX;
    double yMaxLabelY = yMaxTotalY - entryOffsetY - (text_box_margin_y*entryHeightY);
    double labelWidthX = std::abs(xMaxLabelX - xMinLabelX);
    double labelHeightY = std::abs(yMaxLabelY - yMinLabelY);
    // do key
    ////////////////////////////////////////////////////////////////
    double keyInsetLengthXY = keyWidthX>keyHeightY ? keyHeightY*(1.-_key_ratio) : keyWidthX*(1.-_key_ratio);
    double keyInsetWidthX = keyInsetLengthXY;
    double keyInsetHeightY = keyInsetLengthXY;
    //
    boost::shared_ptr<TBox> keyBoxBound( new TBox( x_to_u(xMinKeyX+(keyInsetWidthX/2.)), y_to_v(yMinKeyY+(keyInsetHeightY/2.)), 
                                                   x_to_u(xMaxKeyX-(keyInsetWidthX/2.)), y_to_v(yMaxKeyY-(keyInsetHeightY/2.)) ) );
    //garbage.push_back( keyBoxBound ); // FIXME
    keyBoxBound->SetFillStyle( 0 ); //ROOT_FILL_HOLLOW );
    keyBoxBound->SetLineColor( att_line->GetLineColor() );
    keyBoxBound->SetLineStyle( att_line->GetLineStyle() );
    int lineWidth = pad->UtoPixel(keyInsetLengthXY);
    keyBoxBound->SetLineWidth( lineWidth );
    keyBoxBound->Draw();
    boost::shared_ptr<TBox> keyBox( new TBox( x_to_u(xMinKeyX+keyInsetWidthX), y_to_v(yMinKeyY+keyInsetHeightY),
                                              x_to_u(xMaxKeyX-keyInsetWidthX), y_to_v(yMaxKeyY-keyInsetHeightY) ) );
    if( !keyBox ) { continue; }
    //garbage.push_back( keyBox ); // FIXME
    keyBox->SetFillColor( att_fill->GetFillColor() );
    keyBox->SetFillStyle( _key_fill_style );
    keyBox->Draw();
    // do label
    ////////////////////////////////////////////////////////////////
    latex->SetTextSize( _text_size );
    latex->SetTextFont( _text_font );
    latex->SetTextColor( _text_color );
    string txt( label );
    latex->SetText( x_to_u(xMinLabelX) , y_to_v(yMinLabelY), txt.c_str() );
    text_width_u = latex->GetXsize();
    text_height_v = latex->GetYsize();
    // if text width too big for entry size, remove trailing characters one by one
    while( (u_to_x(text_width_u)-u_to_x(0)) > labelWidthX ) {
      unsigned int n = txt.size();
      if( n < 1 ) { break; }
      txt = txt.substr( 0 , n-1 );
      latex->SetText( x_to_u(xMinLabelX) , y_to_v(yMinLabelY), txt.c_str() );
      text_width_u = latex->GetXsize();
      text_height_v = latex->GetYsize();
    }
    // center text top to bottom
    double uText = x_to_u(xMinLabelX);
    double vText = y_to_v(yMinLabelY+(labelHeightY/2.)) - (text_height_v / 2.);
    // draw text
    TObject* prim = latex->DrawLatex( uText , vText, txt.c_str() );
    //garbage.push_back( prim );// FIXME
  } // end for each key to make
}

const double
DataGraphics::DgGxLegend::u_to_x( const double& u ) const
{
  return ( (u - _pad_x1) / ( _pad_x2 - _pad_x1 ) );
}

const double
DataGraphics::DgGxLegend::v_to_y( const double& v ) const
{
  return ( (v - _pad_y1) / ( _pad_y2 - _pad_y1 ) );
}

const double
DataGraphics::DgGxLegend::x_to_u( const double& x ) const
{
  return ( (x*( _pad_x2 - _pad_x1 )) + _pad_x1 );
}

const double
DataGraphics::DgGxLegend::y_to_v( const double& y ) const
{
  return ( (y*( _pad_y2 - _pad_y1 )) + _pad_y1 );
}


// get overlap of b with h (maximum is area of h)
const double
DataGraphics::DgGxLegend::get_box_overlap( const double& hU1 , const double& hU2 , const double& hV1 , const double& hV2 ,
                                           const double& bU1 , const double& bU2 , const double& bV1 , const double& bV2 ) const
{
  // check for intersection
  if( hV2 < bV1 ) { return 0; }
  if( hU2 < bU1 ) { return 0; }
  if( hV1 > bV2 ) { return 0; }
  if( hU1 > bU2 ) { return 0; }
  // compute intersection
  double widthU = 0;
  if( (bU1<hU1) && (bU2>hU2) ) { widthU = hU2-hU1; }
  else if( (bU1>=hU1) && (bU2<=hU2) ) { widthU = bU2-bU1; }
  else if( (bU1>=hU1) && (bU2>hU2) ) { widthU = hU2-bU1; }
  else if( (bU1<hU1) && (bU2<=hU2) ) { widthU = bU2-hU1; }
  double heightV = 0;
  if( (bV1<hV1) && (bV2>hV2) ) { heightV = hV2-hV1; }
  else if( (bV1>=hV1) && (bV2<=hV2) ) { heightV = bV2-bV1; }
  else if( (bV1>=hV1) && (bV2>hV2) ) { heightV = hV2-bV1; }
  else if( (bV1<hV1) && (bV2<=hV2) ) { heightV = bV2-hV1; }
  // overlap
  double dOverlap = widthU*heightV;
  //cout << "area,overlap" << /*dArea <<*/ ","<<dOverlap<<endl;
  return dOverlap;
}

const double
DataGraphics::DgGxLegend::get_th1_overlap( const TPad* pad , const TH1* hist ,
                                           const double& x1 , const double& x2 , const double& y1 , const double& y2 ) const
{
  if( !hist ) { return 0; }
  //
  double uMin = pad->GetX1(); //cout << uMin << " ";
  double uMax = pad->GetX2(); //cout << uMax << " ";
  double vMin = pad->GetY1(); //cout << vMin << " ";
  double vMax = pad->GetY2(); //cout << vMax << endl;
  //
  double bU1 = (x1*(uMax-uMin))+uMin;
  double bU2 = (x2*(uMax-uMin))+uMin;
  double bV1 = (y1*(vMax-vMin))+vMin;
  double bV2 = (y2*(vMax-vMin))+vMin;
  //  cout << "box: " << bU1 << "," <<bV1<<" " <<bU2<<","<<bV2<< endl;
  //   TBox* bBox = new TBox( bU1,bV1,bU2,bV2);
  //   bBox->SetLineColor(kBlue);
  //   bBox->SetLineStyle(2);
  //   bBox->SetFillStyle(0);
  //   bBox->Draw();
  //   TLatex latex;
  //   latex->SetTextSize(0.04);
  //   latex->SetTextFont(62);
  //
  Int_t n = hist->GetNbinsX();
  double overlap = 0.;
  double area = 0.;
  //
  for( Int_t i=0; i<n; ++i )
    {
      // cout << "bin " << i;
      double low = hist->GetBinLowEdge(i);
      double high = hist->GetBinLowEdge(i) + hist->GetBinWidth(i);
      double c = hist->GetBinContent(i);
      double hU1 = low;
      double hU2 = high;
      double hV1 = 0;
      double hV2 = c;
      //       cout << "hbin: " << hU1 << "," <<hV1<<" " <<hU2<<","<<hV2<< endl;
      //       TBox* hBox = new TBox( hU1,hV1,hU2,hV2 );
      //       hBox->SetLineColor(kBlack);
      //       hBox->SetFillStyle(0);
      //       hBox->SetLineStyle(1);
      //       hBox->Draw();
      // area
      double dArea = (hU2-hU1)*(hV2-hV1);
      area += dArea;
      if( dArea == 0 ) { continue; }
      double dOverlap = get_box_overlap( hU1 , hU2 , hV1 , hV2 , bU1 , bU2 , bV1 , bV2 );
      overlap += dOverlap;
      //       char areaStr[512], overlapStr[512];
      //       sprintf(areaStr,"area: %f" , dArea);
      //       sprintf(overlapStr,"overlap: %f" , dOverlap);
      //      TLatex* latexA = latex.DrawLatex( hU2, hV2 , areaStr);
      //      TLatex* latexO = latex.DrawLatex( hU2, hV1+((hV2-hV1)/2.), overlapStr);
      //      pad->Update();
      //      Wait();
      //      delete latexA;
      //      delete latexO;
      //      delete hBox;
    }
  //  cout << "result: area,overlap=" << area << "," << overlap << " -> " << (overlap/area) << endl;
  //  Wait();
  //  delete bBox;
  if( area == 0 ) { return 0.; }
  if( overlap == 0 ) { return 0.; }
  return (overlap/area);
  //  cout << endl;
}

// compute legend position based on histogram and plot primitives distributions
void
DataGraphics::DgGxLegend::place_legend( const double& widthX , const double& heightY , TPad* pad )
{
  cout << "placing legend for widthX=" << widthX << " heightY=" << heightY << endl;
  if( !pad ) { _x1 = 0.5; _y1 = 0.5; _x2 = _margin_x2; _y2 = _margin_y2; return; }
  // define placement options
  //unsigned int   nPlace = 9;
  double placeX1[3][3];
  double placeY1[3][3];
  double placeX2[3][3];
  double placeY2[3][3];
  bool  validPlace[3][3]; // for user control
  placeX1[TOP][RIGHT] = 1. - _margin_x2 - widthX;
  placeX2[TOP][RIGHT] = 1. - _margin_x2;
  placeY1[TOP][RIGHT] = 1. - _margin_y2 - heightY;
  placeY2[TOP][RIGHT] = 1. - _margin_y2;
  placeX1[TOP][CENTER] = 0.5 - (widthX/2.); 
  placeX2[TOP][CENTER] = 0.5 + (widthX/2.);
  placeY1[TOP][CENTER] = 1. - _margin_y2 - heightY;
  placeY2[TOP][CENTER] = 1. - _margin_y2;
  placeX1[TOP][LEFT] = 0 + _margin_x1;
  placeX2[TOP][LEFT] = 0 + _margin_x1 + widthX;
  placeY1[TOP][LEFT] = 1. - _margin_y2 - heightY;
  placeY2[TOP][LEFT] = 1. - _margin_y2;
  placeX1[CENTER][RIGHT] = 1. - _margin_x2 - widthX;
  placeX2[CENTER][RIGHT] = 1. - _margin_x2;
  placeY1[CENTER][RIGHT] = 0.5 - (heightY/2.);
  placeY2[CENTER][RIGHT] = 0.5 + (heightY/2.);
  placeX1[CENTER][CENTER] = 0.5 - (widthX/2.); 
  placeX2[CENTER][CENTER] = 0.5 + (widthX/2.);
  placeY1[CENTER][CENTER] = 0.5 - (heightY/2.);
  placeY2[CENTER][CENTER] = 0.5 + (heightY/2.);
  placeX1[CENTER][LEFT] = 0 + _margin_x1;
  placeX2[CENTER][LEFT] = 0 + _margin_x1 + widthX;
  placeY1[CENTER][LEFT] = 0.5 - (heightY/2.);
  placeY2[CENTER][LEFT] = 0.5 + (heightY/2.);
  placeX1[BOTTOM][RIGHT] = 1. - _margin_x2 - widthX;
  placeX2[BOTTOM][RIGHT] = 1. - _margin_x2;
  placeY1[BOTTOM][RIGHT] = _margin_y1;
  placeY2[BOTTOM][RIGHT] = _margin_y1 + heightY;
  placeX1[BOTTOM][CENTER] = 0.5 - (widthX/2.); 
  placeX2[BOTTOM][CENTER] = 0.5 + (widthX/2.);
  placeY1[BOTTOM][CENTER] = _margin_y1;
  placeY2[BOTTOM][CENTER] = _margin_y1 + heightY;
  placeX1[BOTTOM][LEFT] = 0 + _margin_x1;
  placeX2[BOTTOM][LEFT] = 0 + _margin_x1 + widthX;
  placeY1[BOTTOM][LEFT] = _margin_y1;
  placeY2[BOTTOM][LEFT] = _margin_y1 + heightY;
  // legend placement weights (higher means worse spot)
  double placeBadness[3][3]; 
  for( unsigned int i=0; i<3; ++i )
    for( unsigned int j=0; j<3; ++j )
      {
        placeBadness[i][j] = 0.;
        validPlace[i][j] = 0.;
      }
  // eliminate places ruled out by user settings
  if( (_placement_vert_mode != AUTO) || (_placement_horiz_mode != AUTO) )
    {
      for(unsigned int i=0; i<3; ++i)
        for( unsigned int j=0; j<3; ++j )
          {
            if( (_placement_vert_mode!=AUTO && _placement_vert_mode!=i) ||
                (_placement_horiz_mode!=AUTO && _placement_horiz_mode!=j) )
              { validPlace[i][j] = false; }
          }
    }
  // loop over primitives and compute weights
  TList* list = pad->GetListOfPrimitives();
  TIterator* iter = list ? list->MakeIterator() : 0;
  if( (!list) || (!iter) )
    { // flying blind
      for( unsigned int i=0; i<3; ++i )
        for( unsigned int j=0; j<3; ++j )
          {
            placeBadness[i][j] = 1000.;
          }
      placeBadness[TOP][RIGHT] = 0.0; 
    }
  else
    { 
      while( true )
        {
          TObject* prim = iter->Next();
          if( !prim ) { break; }
          if( dynamic_cast<TH1*>( prim ) && !dynamic_cast<TH2*>( prim ) ) // 1D histograms
            {
              TH1* hist = dynamic_cast<TH1*>( prim );
              cout << "checking histogram " << hist->GetName() << endl;
              for( unsigned int i=0; i<3; ++i )
                for( unsigned int j=0; j<3; ++j )
                  {
                    if( !validPlace[i][j] ) { continue; }
                    placeBadness[i][j] += get_th1_overlap( pad , hist ,
                                                          placeX1[i][j] , placeX2[i][j] ,
                                                          placeY1[i][j] , placeY2[i][j] );
                  }
            }
          else if( prim->InheritsFrom("TBox") )
            {
              if( prim->IsA() == TFrame::Class() ) { continue; }
              TBox* box = dynamic_cast<TBox*>( prim );
              cout << "checking TBox " << box->GetName() << endl;
              if( !box ) { continue; }
              double hU1 = box->GetX1();
              double hU2 = box->GetX2();
              double hV1 = box->GetY1();
              double hV2 = box->GetY2();
              for( unsigned int i=0; i<3; ++i )
                for( unsigned int j=0; j<3; ++j )
                  {
                    if( !validPlace[i][j] ) { continue; }
                    placeBadness[i][j] += get_box_overlap(  hU1 , hU2 , hV1 , hV2 ,
                                                          placeX1[i][j] , placeX2[i][j] ,
                                                          placeY1[i][j] , placeY2[i][j] );
                  }
            }
          else if( prim->InheritsFrom("THStack") )
            {
              THStack* stack = dynamic_cast<THStack*>( prim );
              if( !stack ) { continue; }
              cout << "checking THStack" << endl;
              TList* hists = stack->GetHists();
              if( !hists ) { continue; }
              TIterator* hIter = hists->MakeIterator();
              TH1* sum = 0;
              unsigned int nHists = 0;
              while( true )
                {
                  TH1* hist = dynamic_cast<TH1*>( hIter->Next() );
                  if( !hist ) { break; }
                  if( !sum ) { sum = dynamic_cast<TH1*>( hist->Clone() ); }
                  else { sum->Add(hist); }
                  ++nHists;
                }
              if( !sum ) { continue; }
              for( unsigned int i=0; i<3; ++i )
                for( unsigned int j=0; j<3; ++j )
                  {
                    if( !validPlace[i][j] ) { continue; }
                    placeBadness[i][j] += get_th1_overlap( pad , sum ,
                                                          placeX1[i][j] , placeX2[i][j] ,
                                                          placeY1[i][j] , placeY2[i][j] );
                  }
              cout << "processed THStack with " << nHists << " histograms" << endl;
            }
          cout << "badness after primitive:" << endl;
          for( unsigned int i=0; i<3; ++i )
            for( unsigned int j=0; j<3; ++j )
              { cout << placeBadness[i][j] << " "; }
          cout<<endl;
        }
    }
  // decide placement to minimize overlap
  unsigned int iPlace = TOP;
  unsigned int jPlace = RIGHT;
  for( unsigned int i=0; i<3; ++i )
    for( unsigned int j=0; j<3; ++j )
      {
        if( !validPlace[i][j] ) { continue; }
        bool min = true;
        for( unsigned int k=1; k<3; k++ )
          for( unsigned int m=1; m<3; m++ )
            {
              if( !validPlace[k][m] ) { continue; }
              min = min && ( placeBadness[i][j] <= placeBadness[(i+k)%3][(j+m)%3] );
            }
        if( min ) { iPlace = i; jPlace = j; break; }
      }
  cout << "decided on place "<<iPlace << " " << jPlace << endl;
  // override if necessary
  if( _placement_horiz_mode != AUTO ) { jPlace = _placement_horiz_mode; cout << "override to " << jPlace << endl; }
  if( _placement_vert_mode != AUTO ) { iPlace = _placement_vert_mode; cout << "override to " << iPlace << endl; }
  // set place
  _x1 = placeX1[iPlace][jPlace];
  _x2 = placeX2[iPlace][jPlace];
  _y1 = placeY1[iPlace][jPlace];
  _y2 = placeY2[iPlace][jPlace];
  cout << "with " << _x1 <<"," <<_y1<<" "<<_x2<<","<<_y2 << endl;
}
