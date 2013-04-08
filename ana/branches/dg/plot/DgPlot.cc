
// Garbage bin of code to analyze and plot a file with DgCollections of data.
// Someday I'll find the time to blow this away and replace it with something better.

#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <list>
#include <string>
#include <stack>
#include <queue>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <functional>
#include <ext/functional>
#include <limits>
#include <iomanip>
#include <cstdio>
#include <cstdlib>
#include <stdexcept>
#include <TFile.h>
#include <TDirectory.h>
#include <TObject.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TCollection.h>
#include <TList.h>
#include <TIterator.h>
#include <TH1.h>
#include <TH2.h>
#include <TGraph.h>
#include <TGraphAsymmErrors.h>
#include <TProfile.h>
#include <TLegend.h>
#include <TAxis.h>
#include <TApplication.h>
#include <TRint.h>
#include <TStyle.h>
#include <TColor.h>
#include <TLatex.h>
#include <TRegexp.h>
#include <TRandom.h>
#include <TRandom3.h>
#include <TF2.h>
#include <TMath.h>
#include <TROOT.h>
#include <TObjectTable.h>
#include <TVirtualPad.h>
#include <TTree.h>
#include <TBranch.h>
#include <TLeaf.h>
#include <TGraphErrors.h>
#include <THStack.h>
#include <TMultiGraph.h>
#include <DataGraphics/DataGraphics.hh>
#include <DataGraphics/DgBinomialErrors.hh>
#include <DataGraphics/DgTools.hh>
#include <DataGraphics/DgGxLegend.hh>
#include <DataGraphics/DgXMLMap.hh>
#include <DataGraphics/DgXMLSampleDescription.hh>
#include <DataGraphics/DgResourceMgr.hh>
#ifdef HAVE_READLINE
#include <readline/readline.h>
#include <readline/history.h>
#endif
#include <boost/version.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/any.hpp>
#include <boost/scope_exit.hpp>
#include <boost/foreach.hpp>
#include <boost/bind.hpp>
#include <boost/format.hpp>
#include <boost/unordered_map.hpp>
#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/math/special_functions/round.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/serialization/optional.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/scoped_ptr.hpp>
#include <boost/serialization/version.hpp>
#include <DataGraphics/serialize_tatt.hh>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>
//#include <boost/thread.hpp>
#include <boost/type_traits.hpp>

// roofit
#include <RooRealVar.h>
#include <RooDataHist.h>
#include <RooVoigtian.h>
#include <RooCBShape.h>
#include <RooAddPdf.h>
#include <RooExponential.h>
#include <RooPlot.h>
#include <RooArgSet.h>
#include <RooArgList.h>
#include <RooGlobalFunc.h>
#include <RooFitResult.h>
#include <RooCBShape.h>
#include <RooDataSet.h>
#include <RooBinning.h>
#include <RooArgSet.h>
#include <RooSimultaneous.h>
#include <RooCategory.h>
#include <RooExtendPdf.h>
#include <RooGenericPdf.h>
#include <RooSimPdfBuilder.h>
#include <TVirtualFitter.h>
#include <RooGaussian.h>
#if ROOT_VERSION_CODE >= ROOT_VERSION(5,24,0)
#include <RooAbsDataStore.h>
#include <RooTreeDataStore.h>
#endif
// plplot
#ifdef HAVE_PLPLOT
#include <plplot/plstream.h>
#endif


using namespace std;
using namespace DataGraphics;
using namespace boost;

/*
  extern TDirectory* gDirectory;
  extern TStyle* gStyle;
  extern TRandom* gRandom;
  extern TROOT* gROOT;
  extern TObjectTable* gObjectTable;
*/

class DgPlotApp;
class AbsPlot;
class AbsCommand;

typedef enum { SOLID , HOLLOW , POINTS } HistogramStyle;
typedef enum { DEFAULT_OVERLAY=0 , DATAMC_OVERLAY , COMPARISON_OVERLAY , SHAPE_COMPARISON_OVERLAY } OverlayMode;
typedef enum { AUTO_RANGE=0 , FIXED_RANGE=1 } RangeMode;

boost::filesystem::path history_filename = "./.dgplot_history";

class
DgPlot {
public:
  void start( const int& argc , char* argv[] );
};

// global settings and flags
bool g_quit = false;
bool g_debug_mode = false;
unsigned int g_terminal_height = 30;

// plot settings
TCanvas*         g_canvas = 0;       // the drawing canvas
vector< shared_ptr<AbsPlot> > g_plot_stack;       // stuff to plot.
vector<TObject*> g_garbage_stack_tobject;    // stuff to delete when redrawing the plot stack or clearing it.
vector<AbsPlot*> g_garbage_stack_absplot;    // stuff to delete when redrawing the plot stack or clearing it.
vector<boost::any> g_garbage_stack_any;    // stuff to delete when redrawing the plot stack or clearing it.

vector<Int_t>    g_color_palette;    // color palette. defined in BwPlotStyle.o
double           g_aspect_ratio = 16/9.; // x/y -- widescreen aspect ratio is default.

unsigned int     g_display_width = 1024;
unsigned int     g_display_height = 700;

int              g_plot_dominant_color = 12; // 38
HistogramStyle   g_histogram_style = SOLID;
bool             g_histogram_unitnormalized = false;
int              g_plot_data_linewidth = 2;
int              g_axis_color = 16;
int              g_axis_title_color = 14;
int              g_axis_label_color = 12;
OverlayMode      g_overlay_mode = DEFAULT_OVERLAY;
RangeMode        g_range_mode = AUTO_RANGE;
vector<int>      g_plot_palette;

typedef struct {
  shared_ptr<DgXMLSampleDescription> desc;
  string path;
  DgCollection* coll;
  const shared_ptr<DgXMLSampleDescription>& description() const { return desc; }
} DescValue;
typedef map<string,DescValue> DescColl; // name , description
DescColl g_descriptions;

// global user-settable values
vector< shared_ptr<const DgAbsGraphic> > g_working_stack;  // working stack of values
boost::unordered_map<string,string> g_glob_pars;
optional<double> g_luminosity; // in 1/pb

// plot sample tree
class
PlotGroup
{
public:
  typedef enum { SAMPLE , OVERLAY , STACK , MERGE , UNKNOWN } Type;
  typedef list< shared_ptr<PlotGroup> > Coll;
private:
  Type _type;
  string _description_name; 
  shared_ptr<DgXMLSampleDescription> _group_description; // dummy description representing the group. if specified, replaces global description object.
  string _legend_text;
  Coll _leaves;
  shared_ptr<PlotGroup> _parent;
  optional<float> _reweight; // reweighting factor
  bool _enabled; // enable/disable plotting of this group
  // style
  shared_ptr<TAttMarker> _marker_style;
  shared_ptr<TAttLine> _line_style;
  shared_ptr<TAttFill> _fill_style;
  shared_ptr<TAttMarker> _graph_marker_style;
  shared_ptr<TAttLine> _graph_line_style;
  string _draw_options;
  string _graph_draw_options;
  bool _show_components;
private:
  friend class boost::serialization::access;
  template<class archiveT>
  void serialize( archiveT& ar, const unsigned int version )
  {
    ar & boost::serialization::make_nvp("type",_type);
    ar & boost::serialization::make_nvp("description_name",_description_name);
    if( version >=2 ) { ar & boost::serialization::make_nvp("group_description",_group_description); }
    ar & boost::serialization::make_nvp("legend_text",_legend_text);
    ar & boost::serialization::make_nvp("leaves",_leaves);
    ar & boost::serialization::make_nvp("parent",_parent);
    ar & boost::serialization::make_nvp("reweight",_reweight);
    ar & boost::serialization::make_nvp("marker_style",_marker_style);
    ar & boost::serialization::make_nvp("line_style",_line_style);
    ar & boost::serialization::make_nvp("fill_style",_fill_style);
    ar & boost::serialization::make_nvp("draw_options",_draw_options);
    if( version >=2 ) { 
      ar & boost::serialization::make_nvp("show_components",_show_components); 
      ar & boost::serialization::make_nvp("enabled",_enabled); 
    }
    if( version>=3 ) {
      ar & boost::serialization::make_nvp("graph_marker_style",_graph_marker_style);
      ar & boost::serialization::make_nvp("graph_line_style",_graph_line_style);
    ar & boost::serialization::make_nvp("graph_draw_options",_graph_draw_options);
    }
  }
  // default constructor only for use by boost::serialization::access
  PlotGroup() 
    : _type(UNKNOWN)
    , _description_name( )
    , _group_description()
    , _legend_text( )
    , _leaves()
    , _parent()
    , _reweight()
    , _marker_style()
    , _line_style()
    , _fill_style()
    , _draw_options("9H")
    , _graph_marker_style()
    , _graph_line_style()
    , _graph_draw_options("p")
    , _show_components(true)
    , _enabled(true)
  {}
public:
  PlotGroup( const shared_ptr<PlotGroup>& parent , const shared_ptr<DgXMLSampleDescription>& desc ) 
    : _type(SAMPLE)
    , _description_name( desc->name() )
    , _group_description()
    , _legend_text( desc->legend() )
    , _leaves()
    , _parent(parent)
    , _reweight()
    , _marker_style()
    , _line_style()
    , _fill_style()
    , _draw_options("9H")
    , _graph_marker_style()
    , _graph_line_style()
    , _graph_draw_options("p")
    , _show_components(true)
    , _enabled(true)
  {}
  PlotGroup( const shared_ptr<PlotGroup>& parent , const Type& type , const shared_ptr<DgXMLSampleDescription>& group_description , 
             Coll& grouped_leaves ) 
    : _type(type)
    , _description_name( group_description->name() )
    , _group_description( group_description )
    , _legend_text( group_description->legend() )
    , _leaves( grouped_leaves )
    , _parent(parent)
    , _reweight()
    , _marker_style()
    , _line_style()
    , _fill_style()
    , _draw_options("9H")
    , _graph_marker_style()
    , _graph_line_style()
    , _graph_draw_options("p")
    , _show_components(true)
    , _enabled(true)
  {
    if( type==MERGE ) { 
      _group_description.reset( new DgXMLSampleDescription( *group_description ) );
      _show_components = false;
    }
    // if description is stored in the global description collection,
    // we don't need to store a pointer and can just look it up by
    // name when needed. otherwise, keep a local description (it is a
    // placeholder to store group properties).
    if( find_if( g_descriptions.begin() , g_descriptions.end() ,
                 bind( &DescValue::description , bind( &DescColl::value_type::second , _1 ) ) == group_description ) != g_descriptions.end() ) {
      _group_description.reset();
    }
  }
public:
  // this sample
  Type& type() { return _type; }
  void set_type( const Type& v ) { _type = v; }
  shared_ptr<PlotGroup>& parent() { return _parent; }
  // shared_ptr<DgXMLSampleDescription>& description() { return _description; }
  const std::string description_name() { 
    if( ! _group_description ) { return _description_name; }
    return _group_description->name();
  }
  const shared_ptr<DgXMLSampleDescription> description() {
    if( _group_description ) { return _group_description; }
    DescColl::iterator i=g_descriptions.find( _description_name );
    if( i!=g_descriptions.end() ) { return (i->second.description()); }
    return shared_ptr<DgXMLSampleDescription>(); 
  }
  const std::string legend_text() const { return _legend_text; }
  void set_legend_text( const std::string& v ) { _legend_text = v; }
  void set_reweight( const float& v ) { _reweight.reset(v); }
  const float reweight() const { return( _reweight ? (*_reweight) : 1.f ); }
  // hierarchy
  Coll& group() { return _leaves; }
  // get style. if not defined for this subsample, inherited from the parent group
  shared_ptr<TAttMarker>& marker_style() { 
    if( _marker_style ) { return _marker_style; }
    shared_ptr<PlotGroup> p( _parent );
    while( p ) { 
      if( p->_marker_style ) { return p->_marker_style; }
      p = p->_parent;
    }
    return _marker_style; 
  }
  shared_ptr<TAttLine>& line_style() { 
    if( _line_style ) { return _line_style; }
    shared_ptr<PlotGroup> p( _parent );
    while( p ) { 
      if( p->_line_style ) { return p->_line_style; }
      p = p->_parent;
    }
    return _line_style; 
  }
  shared_ptr<TAttFill>& fill_style() { 
    if( _fill_style ) { return _fill_style; }
    shared_ptr<PlotGroup> p( _parent );
    while( p ) { 
      if( p->_fill_style ) { return p->_fill_style; }
      p = p->_parent;
    }
    return _fill_style; 
  }
  string& draw_options() { 
    if( _draw_options!="" ) { return _draw_options; }
    shared_ptr<PlotGroup> p( _parent );
    while( p ) { 
      if( p->_draw_options!="" ) { return p->_draw_options; }
      p = p->_parent;
    }
    return _draw_options;
  }
  shared_ptr<TAttMarker>& graph_marker_style() { 
    if( _graph_marker_style ) { return _graph_marker_style; }
    shared_ptr<PlotGroup> p( _parent );
    while( p ) { 
      if( p->_graph_marker_style ) { return p->_graph_marker_style; }
      p = p->_parent;
    }
    return _graph_marker_style; 
  }
  shared_ptr<TAttLine>& graph_line_style() { 
    if( _graph_line_style ) { return _graph_line_style; }
    shared_ptr<PlotGroup> p( _parent );
    while( p ) { 
      if( p->_graph_line_style ) { return p->_graph_line_style; }
      p = p->_parent;
    }
    return _graph_line_style; 
  }
  string& graph_draw_options() { 
    if( _graph_draw_options!="" ) { return _graph_draw_options; }
    shared_ptr<PlotGroup> p( _parent );
    while( p ) { 
      if( p->_graph_draw_options!="" ) { return p->_graph_draw_options; }
      p = p->_parent;
    }
    return _graph_draw_options;
  }
  // set style
  void set_default_style() {
    _marker_style.reset( new TAttMarker );
    _line_style.reset( new TAttLine );
    _fill_style.reset( new TAttFill );
    _marker_style->SetMarkerColor( gStyle->GetMarkerColor() );
    _marker_style->SetMarkerStyle( gStyle->GetMarkerStyle() );
    _marker_style->SetMarkerSize( gStyle->GetMarkerSize() );
    _line_style->SetLineColor( gStyle->GetLineColor() );
    _line_style->SetLineStyle( gStyle->GetLineStyle() );
    _line_style->SetLineWidth( gStyle->GetLineWidth() );
    _fill_style->SetFillColor( gStyle->GetFillColor() );
    _fill_style->SetFillStyle( gStyle->GetFillStyle() );
    _draw_options = "9h";
    _graph_marker_style->SetMarkerColor( gStyle->GetMarkerColor() );
    _graph_marker_style->SetMarkerStyle( gStyle->GetMarkerStyle() );
    _graph_marker_style->SetMarkerSize( gStyle->GetMarkerSize() );
    _graph_line_style->SetLineColor( gStyle->GetLineColor() );
    _graph_line_style->SetLineStyle( gStyle->GetLineStyle() );
    _graph_line_style->SetLineWidth( gStyle->GetLineWidth() );
    _draw_options = "p";
  }
  void set_data_style( const int& dominant_color ) {
    _line_style.reset();
    _fill_style.reset();
    _marker_style.reset( new TAttMarker );
    _marker_style->SetMarkerColor( dominant_color );
    _marker_style->SetMarkerStyle( 7 );
    _draw_options = "9e ";
    _graph_marker_style->SetMarkerColor( gStyle->GetMarkerColor() );
    _graph_marker_style->SetMarkerStyle( gStyle->GetMarkerStyle() );
    _graph_marker_style->SetMarkerSize( gStyle->GetMarkerSize() );
    _graph_line_style->SetLineColor( gStyle->GetLineColor() );
    _graph_line_style->SetLineStyle( gStyle->GetLineStyle() );
    _graph_line_style->SetLineWidth( gStyle->GetLineWidth() );
    _draw_options = "p";
  }
  void set_mc_style( const int& dominant_color ) {
    _line_style.reset( new TAttLine );
    _line_style->SetLineColor( dominant_color );
    _line_style->SetLineWidth( 1.5 );
    _line_style->SetLineStyle( 1 );
    _fill_style.reset( new TAttFill );
    _fill_style->SetFillColor( dominant_color );
    _fill_style->SetFillStyle( 1001 );
    _marker_style.reset( new TAttMarker );
    _marker_style->SetMarkerStyle( 7 );
    _draw_options = "9H ";
    _graph_marker_style->SetMarkerColor( gStyle->GetMarkerColor() );
    _graph_marker_style->SetMarkerStyle( gStyle->GetMarkerStyle() );
    _graph_marker_style->SetMarkerSize( gStyle->GetMarkerSize() );
    _graph_line_style->SetLineColor( gStyle->GetLineColor() );
    _graph_line_style->SetLineStyle( gStyle->GetLineStyle() );
    _graph_line_style->SetLineWidth( gStyle->GetLineWidth() );
    _draw_options = "p";
  }
  void set_show_components( const bool& yes ) { _show_components = yes; }
  const bool show_components() const { return _show_components; }
  void set_enabled( const bool& yes ) { _enabled = yes; }
  const bool enabled() const {
    shared_ptr<PlotGroup> group( _parent );
    if( ! _enabled ) { return false; }
    while( group ) { 
      if( ! group->_enabled ) { return false; } 
      group = group->parent();
    }
    if( group && ! group->_enabled ) { return false; }
    return true;
  }
  void add( shared_ptr<PlotGroup>& v ) { _leaves.push_back(v); }
};
BOOST_CLASS_VERSION( PlotGroup , 3 );

PlotGroup::Coll g_plot_group_tree;
stack<int> g_available_dominant_color;


DgPlotApp* app = 0;
class
DgPlotApp : public TRint
{
protected:
  // maintain a list of commands to execute.  for interactive jobs, this container will hold only the current command
  // to execute.  for batch jobs, it contains the backlog of commands read from a file.
  queue<string> _commands;
  bool _quit_when_no_commands;
public:
  DgPlotApp(const char* appClassName, Int_t* argc, char** argv) : TRint(appClassName,argc,argv) , _quit_when_no_commands(false) {}
  void LoadCommandsFromFile( const char* filename );
  void ClearQueue() { 
    while( !_commands.empty() ) { _commands.pop(); }
  }
  void Run( Bool_t ret = kFALSE );
  void RunRoot( Bool_t ret = kFALSE ) { TRint::Run(ret); }
  const bool batching_commands() const { return !_commands.empty(); }
  void quit_when_no_commands_batched( const bool& yes ) { _quit_when_no_commands = yes; }
};

class
SampleNormalization : public DgXMLMap<double>
{
protected:
  static SampleNormalization* _instance;
public:
  const double rescale_factor( const string& sample_name , const double& luminosity_in_pb ) const {
    const string i = key(sample_name);
    return i.empty() ? _default : (_data.find(i)->second)*luminosity_in_pb;
  }
  SampleNormalization() : DgXMLMap<double>(1.0) {}
  virtual ~SampleNormalization() {}
  static SampleNormalization* instance() { 
    if( !_instance ) { _instance = new SampleNormalization(); }
    return _instance; 
  }
  void save_example( const string& filename ) {
    apply_example_anton();
    apply_example_antonio();
    save( filename );
  }
  void apply_example_anton() {
    // scale per pb: xsection in pb * MC filter efficiency / nevents
    _data["mc_wmunu"] = 8.894060*1000.0/6993798;
    _data["mc_wtaunu"] = 8.916330*0.876600*1000.0/999874;
    _data["mc_ztautau"] = 0.854173*1000.0/1998598.;
    _data["mc_zmumu"] = 0.851011*1000.0/4998410;
    _data["mc_ttbar"] = 0.145642*0.538200*1000.0/199838;
    _data["mc_J0"] = 9752970.0*0.000079*1000.0/500000;
    _data["mc_J1"] = 673020.0*0.001233*1000.0/500000;
    _data["mc_J2"] = 41194.70*0.005443*1000.0/500000;
    _data["mc_J3"] = 2193.25*0.012949*1000.0/500000;
    _data["mc_J4"] = 87.8487*0.022156*1000.0/500000;
    _data["mc_J5"] = 2.328560*0.029753*1000.0/500000;
  }
  void apply_example_antonio() {
    // _data["top"] = 85./499509.;     // cross section: 85pb * 1000/pb  85k
    // _data["wjets"] = 0.746;
    // _data["susy"] = 3.3E-6;
    // _data["madgraph.tbarbWplus"] =  93.974 / 19672 ;
    // _data["madgraph.tbarbZ"] =  0.86868E-3 / 45210 ;
    // _data["madgraph.tbbarWminus"] = 93.954 / 50000 ;
    // _data["madgraph.tbbarZ"] =  0.20991E-02 / 20749 ;
    // _data["madgraph.tt"] = 0.89953E+02 / 50000 ;
    // _data["madgraph.ttWminus"] = 0.39476E-01 / 8077 ;
    // _data["madgraph.ttWplus"] = 0.10218E+00 / 15158 ;
    // _data["madgraph.ttZ"] =  .10395E+00 / 50000 ;
  }
};
SampleNormalization* SampleNormalization::_instance = 0;

class
RunLumis : public DgXMLMap<double>
{
protected:
  static RunLumis* _instance;
public:
  RunLumis() : DgXMLMap<double>(1.0) {}
  virtual ~RunLumis() {}
  static RunLumis* instance() { 
    if( !_instance ) { _instance = new RunLumis; }
    return _instance; 
  }
  void save_example( const string& filename ) {
    apply_example();
    save( filename );
  }
  void apply_example() {
    _data["152844"]=0.000005528;
    _data["152845"]=0.000026225;
    _data["152878"]=0.000025046;
    _data["152933"]=0.000021068;
    _data["152994"]=0.000005392;
    _data["153030"]=0.000022498;
    _data["153134"]=0.000001348;
    _data["153136"]=0.000000677;
    _data["153159"]=0.000011344;
    _data["153200"]=0.000007377;
    _data["153565"]=0.000664095;
    _data["154810"]=0.000152803;
    _data["154813"]=0.000262390;
    _data["154815"]=0.000062452;
    _data["154817"]=0.000533888;
    _data["155073"]=0.001157791;
    _data["155112"]=0.003411170;
    _data["155116"]=0.000499986;
    _data["155160"]=0.001329210;
    _data["155228"]=0.000041021;
    _data["155280"]=0.000028416;
    _data["155569"]=0.000990926;
    _data["155634"]=0.001064694;
    _data["155669"]=0.000424034;
    _data["155678"]=0.001192450;
    _data["155697"]=0.003422603;
    _data["156682"]=0.001307760;
    _data["158045"]=0.000720508;
    _data["158116"]=0.014191042;
    _data["158269"]=0.003418944;
    _data["158299"]=0.001065140;
    _data["158392"]=0.007496348;
    _data["158443"]=0.001005510;
    _data["158466"]=0.001078360;
    _data["158545"]=0.001333747;
    _data["158548"]=0.009805258;
    _data["158582"]=0.016025237;
    _data["158632"]=0.005241888;
    _data["158801"]=0.007164100;
    _data["158975"]=0.020448692;
    _data["159041"]=0.025454036;
    _data["159086"]=0.054046868;
    _data["159113"]=0.028036890;
    _data["159179"]=0.015016396;
    _data["159202"]=0.009799840;
    _data["159203"]=0.008289830;
    _data["159224"]=0.063948638;
    _data["160387"]=0.058194960;
    _data["160472"]=0.076092440;
    _data["160479"]=0.004698520;
    _data["160530"]=0.092452200;
    _data["160613"]=0.047797810;
    _data["160736"]=0.016828600;
    _data["160800"]=0.016129087;
    _data["160801"]=0.075785075;
    _data["160879"]=0.080618723;
    _data["160899"]=0.004806420;
    _data["160953"]=0.019410770;
    _data["160954"]=0.008079622;
    _data["160958"]=0.040630547;
    _data["160963"]=0.002333970;
    _data["160975"]=0.004475560;
    _data["160980"]=0.008986220;
    _data["161118"]=0.034041800;
    _data["161379"]=0.095666935;
    _data["161407"]=0.040030520;
    _data["161520"]=0.113855241;
    _data["161562"]=0.072325200;
    _data["161948"]=0.089077800;
    _data["162347"]=0.216753830;
    _data["162526"]=0.251527540;
    _data["162576"]=0.037234330;
    _data["162577"]=0.006416770;
    _data["162620"]=0.067138360;
    _data["162623"]=0.232677450;
    _data["162690"]=0.321428830;
    _data["162764"]=0.079460790;
    _data["162843"]=0.305641920;
    _data["162882"]=0.290397000;
    _data["165591"]=0.147542076;
    _data["165632"]=0.534380428;
    _data["165703"]=0.087515800;
    _data["165732"]=0.902983450;
    _data["165767"]=0.922977750;
    _data["165815"]=0.131366440;
    _data["165817"]=0.000233098;
    _data["165818"]=0.213741000;
    _data["165821"]=0.211115700;
    _data["165954"]=0.151721780;
    _data["165956"]=0.090653800;
    _data["166097"]=0.231127000;
    _data["166142"]=0.649588590;
    _data["166143"]=0.254093600;
    _data["166198"]=1.209441390;
    _data["166466"]=1.298725880;
    _data["166658"]=1.931019230;
    _data["166786"]=1.841479400;
    _data["166850"]=0.006457310;
    _data["166856"]=0.293829200;
    _data["166924"]=1.182574300;
    _data["166927"]=0.544956700;
    _data["166964"]=0.147430200;
    _data["167575"]=0.037718400;
    _data["167576"]=3.488406500;
    _data["167607"]=4.981125121;
    _data["167661"]=1.283975000;
    _data["167680"]=3.518534000;
    _data["167776"]=5.789155000;
  }
};
RunLumis* RunLumis::_instance = 0;

// plotting ////////////////////////////////////////////////////////////////

// plot base class
class
AbsPlot
{
protected:
  string _description;
  string _subsample;
  string _file;
  double  _rescale;
  int _override_color;
public:
  AbsPlot(const string& description, const string& subsample, const string& sourcefile)
    : _description(description) 
    , _subsample(subsample)
    , _file(sourcefile)
    , _rescale(1.) , _override_color(0) {}
  virtual ~AbsPlot() {}
  virtual bool isData() const { return false; } // is a plot and not an axis, label, caption, etc.
  virtual bool isFrame() const { return false; } // is not a plot; an axis, label, caption, etc.
  virtual const TObject* draw() const = 0; // called at draw time.
  void rescale( const double& factor ) { _rescale = factor; }
  void recolor( const int& k ) { _override_color = k; }
  const bool override_color() const { return _override_color!=0; }
  const int get_override_color() const { return _override_color; }
  const string& description() const { return _description; }
  const string& subsample() const { return _subsample; }
  const string& source_file() const { return _file; }
  int nDataGraphics() const { return count_if( g_plot_stack.begin() , g_plot_stack.end() , bind(&AbsPlot::isData,_1)==true ); }
  bool isFirstGraphic() const {
    vector< shared_ptr<AbsPlot> >::const_iterator i = find_if( g_plot_stack.begin() , g_plot_stack.end() , bind(&AbsPlot::isData,_1)==true );
    if( i == g_plot_stack.end() ) { return false; }
    shared_ptr<AbsPlot> p = *i;
    return( p.get()==this );
    // return( g_canvas->GetListOfPrimitives()->GetSize() == 0 );
  }
};

// command infrastructure ////////////////////////////////////////////////////////////////

class AbsCommand;
typedef map<string,AbsCommand*> CommandList;
CommandList g_commands;

// if readline library is not available, define a bare-bones replacement
// ================================================================
#ifndef HAVE_READLINE
#include <cstring>
char* readline( const char* prompt ) {
  // readline returns a raw pointer; receiving code is expected to manage the memory,
  // so do not use smart pointers.
  std::cout << prompt;
  string inputline;
  std::cin >> inputline;
  char* result = new char[ inputline.size() + 1 ];
  std::strcpy( result , inputline.c_str() );
  return result;
}
void add_history( const char* string ) {
  // history not implemented without readline.
  return;
}
const int read_history( const char* filename ) {
  // history not implemented without readline.
  return 0;
}
const bool write_history( const char* filename ) { 
  // history not implemented without readline.
  return 0;
}
#endif // end if readline library is not available, define a bare-bones replacement
// ================================================================

class AbsCommand;

// an option to a command
class
CommandOption
{
public:
  typedef enum { BOOL , INTEGER , FLOAT , RANGE , STRING } OptionType;
protected:
  OptionType _type;
  string _name;
  string _desc;
  boost::any _val;
  bool _found;
public:
  CommandOption( AbsCommand* command ,
                 const OptionType& type ,
                 const string& option_name ,
                 const string& option_desc );

  ostream& print( ostream& os ) {
    os << " opt: " << _name << " : " << _desc << endl;
    if( _found ) { 
      try { 
        os << "      value: ";
        switch( _type ) { 
        case BOOL: os << value_bool() << endl; break;
        case INTEGER: os << value_int() << endl; break;
        case FLOAT: os << value_float() << endl; break;
        case RANGE: os << value_range_begin() << " -- " << value_range_end() << endl; break;
        case STRING:
        default: 
          os << value_string() << endl;
        }
      } catch( std::exception& e ) {
        cout << " exception: " << e.what() << endl;
      }
    }
    return os;
  }

  void parse_command_line( string cmd ) {
    const bool debug = g_debug_mode;
    // look for the option. veto on a "(" without a ")" before the command name.
    trim( cmd );
    string escaped_name = dgSTL::escape_for_regex(_name);
    if( debug ) { 
      cout << "escaped option name: " << escaped_name << " for command: " << cmd << endl;
    }
    switch( _type ) {
    case BOOL:
      {
        // non-existence => false
        boost::regex opt_regex( escaped_name+".*\\((.*)\\).*" );
        boost::regex opt_regex_name_only( string(".*")+escaped_name+".*" );
        boost::smatch what;
        const bool found_option = boost::regex_match( cmd , what , opt_regex );
        const bool found_name = boost::regex_match( cmd , what , opt_regex_name_only );
        if( ! found_option && ! found_name ) { 
          _found = false;
          _val = false;
          return;
        }
        if( ! found_option || ! what[1].matched ) {
          _found = true; 
          _val = true;
          return; 
        }
        _found = true;
        string exp( what[1].first , what[1].second );
        boost::trim( exp );
        if( exp=="1" || exp=="true" ) { _val = true; }
        else if( exp=="0" || exp=="false") { _val = false; }
        else { _val = true; }
        if( debug ) { cout << "option " << _name << " has value " << boost::any_cast<bool>(_val) << endl; }
        break;
      }
    case INTEGER:
      {
        boost::regex opt_regex( escaped_name+".*\\((.+)\\)" );
        boost::smatch what;
        _found = boost::regex_match( cmd , what , opt_regex ) && what[1].matched;
        if( ! _found ) { return;  }
        string exp( what[1].first , what[1].second );
        boost::trim( exp );
        _val = std::atoi( exp.c_str() );
        if( debug ) { cout << "option " << _name << " has value " << boost::any_cast<int>(_val) << endl; }
        break;
      }
    case FLOAT:
      {
        boost::regex opt_regex( escaped_name+".*\\((.+)\\).*" );
        boost::smatch what;
        _found = boost::regex_match( cmd , what , opt_regex ) && what[1].matched;
        if( ! _found ) { return; }
        string exp( what[1].first , what[1].second );
        boost::trim( exp );
        _val = std::atof( exp.c_str() );
        if( debug ) { cout << "option " << _name << " has value " << boost::any_cast<float>(_val) << endl; }
        break;
      }
    case RANGE:
      {
        boost::regex opt_regex( escaped_name+".*\\((.+),(.+)\\).*" );
        boost::smatch what;
        _found = boost::regex_match( cmd , what , opt_regex ) && what[1].matched || what[2].matched;
        if( ! _found ) { return; }
        string expA( what[1].first , what[1].second );
        string expB( what[2].first , what[2].second );
        boost::trim( expA );
        boost::trim( expB );
        std::pair<float,float> val( std::atof( expA.c_str() ) , std::atof( expB.c_str() ) );
        _val = val;
        if( debug ) { 
          cout << "option " << _name << " has value " << val.first << " -- " << val.second << endl;
        }
        break;
      }
    case STRING: // fall to default
    default: // treat as string
      {
        boost::regex opt_regex( escaped_name+".*\\((.*)\\)" );
        boost::smatch what;
        _found = boost::regex_match( cmd , what , opt_regex ) && what[1].matched;
        if( ! _found ) { return; }
        _val = string( what[1].first , what[1].second );
        if( debug ) { cout << "option " << _name << " has value " << boost::any_cast<string>(_val) << endl; }
        break;
      }
    }
  }
  const OptionType& type() const { return _type; }
  const bool found() const { return _found; }
  const string& name() const { return _name; }
  const string& description() const { return _desc; }

  template<class valueT>
  const valueT value() const {
    return boost::any_cast<valueT>(_val);
  }
  
  const bool value_bool() const { 
    assert( _type == BOOL );
    return( _found && boost::any_cast<bool>(_val) );
  }
  const int value_int() const { 
    assert( _type == INTEGER );
    return( boost::any_cast<int>(_val) );
  }
  const float value_float() const { 
    assert( _type == FLOAT );
    return( boost::any_cast<float>(_val) );
  }
  const float value_range_begin() const { 
    assert( _type == RANGE );
    return( (boost::any_cast< pair<float,float> >(_val)).first );
  }
  const float value_range_end() const { 
    assert( _type == RANGE );
    return( (boost::any_cast< pair<float,float> >(_val)).second );
  }
  const string value_string() const { 
    assert( _type == STRING );
    return( boost::any_cast<string>(_val) );
  }

};

// prompt helper functions

// prompt the user with a yes/no question.  return true if yes.
const bool askYesNo( const char* prompt = 0 ) {
  bool result = false;
  char tmpbuf[512];
  sprintf( tmpbuf, "%s [y/N]: ", prompt );
  boost::scoped_ptr<char> input_line( readline(tmpbuf) );
  result = ( input_line && (input_line.get()[0] == 'y' || input_line.get()[0] == 'Y') );
  return result;
}

const int askInteger( const char* prompt = 0 ) {
  int result;
  boost::scoped_ptr<char> input_line( readline( prompt ) );
  assert( input_line );
  result = atoi( input_line.get() );
  return result;
}

const double askFloat( const char* prompt = 0 ) {
  double result;
  boost::scoped_ptr<char> input_line( readline( prompt ) );
  assert( input_line );
  result = atof( input_line.get() );
  return result;
}

const string askString( const char* prompt = 0 ) {
  boost::scoped_ptr<char> input_line( readline( prompt ) );
  return string( input_line.get() );
}

  //   bool hasIt = command.Contains(option);
  //   if( hasIt ) {
  //     // removeIt
  //     command.ReplaceAll( option , "" );
  //     command.ReplaceAll( "  " , " " );
  //     //      cout << "hasOption " << option << endl;
  //   }
  //   return hasIt;
  // }
  // static int intOption( string& command ) {
  //   string stcom( command.Data() );
  //   int result = atoi( stcom.c_str() );
  //   // remove the integer
  //   char tmpbuf[512];
  //   sprintf( tmpbuf , "%d" , result );
  //   size_t i = stcom.find( tmpbuf );
  //   size_t j = stcom.find( " " , i );
  //   command = stcom.substr( j+1 , stcom.size() - j - 1 );
  //   return result;
  // }
//   static double floatOption( string& command ) {
//     string stcom( command.Data() );
//     double result;
//     sscanf( "%f" , stcom.c_str() , &result );
//     // remove the float
//     char tmpbuf[512];
//     sprintf( tmpbuf , "%f" , result );
//     size_t i = stcom.find( tmpbuf );
//     size_t j = stcom.find( " " , i );
//     command = stcom.substr( j+1 , stcom.size() - j - 1 );
//     return result;
//   }
  // static bool rangeOption( string& command , const string& option , double& min , double& max ) {
  //   bool hasIt = command.Contains(option);
  //   if( hasIt ) {
  //     string stcom( command.Data() );
  //     size_t i = stcom.find( option.Data() , 0 );
  //     assert( i!=string::npos );
  //     i += option.Sizeof() - 1;
  //     size_t j = stcom.find("(",i);
  //     size_t m = stcom.find(",",j);
  //     size_t k = stcom.find(")",j);
  //     if( j==string::npos || k==string::npos || m==string::npos || k<j || m<j || m>k ) {
  //       cout << " range incorrectly specified" << endl;
  //       return false;
  //     }
  //     string par1 = stcom.substr( j+1 , m-j-1 );
  //     string par2 = stcom.substr( m+1 , k-m-1 );
  //     min = atof(par1.c_str());
  //     max = atof(par2.c_str());
  //     cout << " range " << min << " to " << max << endl;
  //     // remove the range
  //     j = stcom.find( option.Data() , 0 );
  //     assert( j!=string::npos );
  //     k = stcom.find( ")" , j );
  //     command = stcom.substr(0,j);
  //     command += stcom.substr(k+1,stcom.size()-k-1);
  //     //      cout << "removed. command is now: " << command << endl;
  //     return true;
  //   }
  //   return false;
  // }


// a command (base class)

class 
AbsCommand
{
private:
  string _name;
  string _description;
  map< string , CommandOption* > _options;
protected:
  // abstract interface: this is where you implement your command.
  virtual void operator()( string& command ) = 0;  
  // add an option
  friend class CommandOption; // to call add_option in CommandOption constructor
  void add_option( CommandOption* option ) {
    _options[ option->name() ] = option;
  }
  // retrieve status of an option
  // boost::optional<CommandOption> get_option( const string& name ) const {
  //   map< string , CommandOption >::const_iterator i=_options.find(name); 
  //   if( i==_options.end() ) { return boost::optional<CommandOption>(); }
  //   return boost::optional<CommandOption>(i->second);
  // }
  // print avaiable options
  ostream& print_options( ostream& os ) const {
    for( map< string , CommandOption* >::const_iterator i=_options.begin(), f=_options.end(); i!=f; ++i ) {
      i->second->print(os);
    }
    return os;
  }

  // choose from list.  a return code of zero means
  // no choice made.  
  // functor for incremental search
  template<typename inT,typename outT>
  class MatchesString : public std::unary_function<inT,outT> {
  protected:
    boost::regex _pattern;
  public:
    MatchesString(const string& pat) : _pattern(pat) {}
    bool operator()(inT input) const {
      try {
        return regex_match(input,_pattern);
      } catch( std::exception& e ) {
        if( g_debug_mode ) {
          cout << " caught exception: " << e.what() << endl;
        }
      }
      return false;
    }
  };
  static int chooseFromList( const vector<string>& const_choices ) {
    vector<string> choices( const_choices );
    size_t maxPerPage = g_terminal_height-2;
    int ichoice = 1; // begin page
    int fchoice;
    while( true ) {
      fchoice = std::min(choices.size(),ichoice+maxPerPage); // end bage
      // show choices ichoice through fchoice.  if more than maxPerPage, show a range of choices.
      vector<string>::iterator i=choices.begin();
      advance( i , ichoice-1 );
      vector<string>::iterator f=choices.end();
      if( distance(i,f) > maxPerPage ) {
        f = choices.begin();
        advance( f , fchoice ); 
      }
      for( ; i!=f; ++i ) {
        int nchoice = (distance(choices.begin(),i)+1);
        cout << setw(4) << nchoice << "   " 
             << setiosflags(ios::left) << setw(60) << (*i)
             << " " << nchoice
             << resetiosflags(ios::left) << endl;
      }
      boost::scoped_ptr<char> buf( readline( f==choices.end() ? " which one? (negative=none)" : " which one? (;=more choices,negative=none)" ) );
      int choice = atoi(buf.get());
      const bool is_number = !isalpha(buf.get()[0]);
      if( is_number && choice<0 ) { return -1; }
      if( !is_number || choice==0 ) { // text, net, or empty choice
        if( buf.get()[0] == ';' ) { // next page
          ichoice += maxPerPage;
          if( ichoice>choices.size() ) { ichoice = 1; }
        } else {
          // try to map this to a unique choice (incremental search)
          string tmpbuf(buf.get());
          string tmpbufesc( dgSTL::escape_for_regex(tmpbuf) );
          MatchesString<const string,bool> strfunc(".*"+tmpbufesc+".*");
          vector<string> tmpchoices( choices );
          tmpchoices.erase( remove_if( tmpchoices.begin() , tmpchoices.end() , not1(strfunc) ) , tmpchoices.end() );
          if( tmpchoices.empty() ) {
            cout << " no matches.  choose again." << endl;
          } else {
            // if one choice left, this is it.
            if( tmpchoices.size()==1 ) {
              // convert to index into original vector
              int result = distance( const_choices.begin() , find(const_choices.begin(),const_choices.end(),*tmpchoices.begin()) ) + 1;
              return result;
            } else {
              // narrowed the range but not yet unique.  
              choices.swap( tmpchoices );
              ichoice = 1;
            }
          }
        }
      } else if( is_number ) { // numerical choice
        if( choice<0 ) { return 0; }
//         cout << " choice: " << choice << endl;
        if( choice<=choices.size() ) {
          // look up string from choices, then convert to index into original vector
          vector<string>::iterator j = choices.begin();
          advance(j,choice-1 );
          int result = distance( const_choices.begin() , find(const_choices.begin(),const_choices.end(),*j) ) + 1;
//           if( result>=1 ) { cout << " chose " << const_choices[result-1] << endl; }
          return result;
        } 
        cout << " not a good choice.  choose again." << endl;
      }
    } // until a choice is made
  }
public:
  static void clearCanvas() {
    g_plot_stack.clear();
  }
public:
  // options should be public
  CommandOption opt_clear;
  CommandOption opt_debug;
public:
  AbsCommand( const char* name , const char* description )
    : _name(name)
    , _description(description)
    , opt_debug( this , CommandOption::BOOL , "debug" , "verbose debugging output" )
    , opt_clear( this , CommandOption::BOOL , "clear" , "clear the plot stack before the command" )
  {
    g_commands[ name ] = this;    
  }
  virtual ~AbsCommand() {}
  const string& name() const { return _name; }
  const string& description() const { return _description; }
  void execute( string& cmd ) {
    // set all option variables
    for( map<string,CommandOption*>::iterator i=_options.begin(), f=_options.end(); i!=f; ++i ) {
      try { 
        i->second->parse_command_line( cmd );
      } catch( std::exception& e ) {
        cout << " warning: confusion while parsing for option " << i->first << endl;
        if( opt_debug.value_bool() || g_debug_mode ) {
          cout << e.what() << endl;
        }
      }
    }
    //
    if( opt_clear.value_bool() ) { clearCanvas(); }
    // execute derived command.
    operator()( cmd );
  }
  virtual void showHelp() const {
    cout << _name << " " << _description << endl;
  }
  virtual void showMoreHelp() const {
    cout << _name << endl;
    cout << _description << endl;
    cout << " with options " << endl;
    // show in reverse order (specialization then base command
    // options), since these are most relevant to the derived command
    for( map<string,CommandOption*>::const_reverse_iterator i=_options.rbegin(), f=_options.rend(); i!=f; ++i ) {
      i->second->print( cout );
    }
  }
};

CommandOption::CommandOption( AbsCommand* command , 
                              const OptionType& type ,
                              const string& option_name ,
                              const string& option_desc )
  : _type(type)
  , _name(option_name)
  , _desc(option_desc)
  , _val()
  , _found(false)
{
  assert( command );
  command->add_option( this );
}


string
first_token( string command )
{
  using namespace boost;
  trim( command );
  vector<string> tokens;
  split( tokens , command , is_any_of(" \t") , token_compress_on );
  if( tokens.empty() ) { return string(""); }
  return tokens.front();
}

AbsCommand*
match_command( string command )
{
  // get first part of command.
  trim( command );
  vector<string> tokens;
  split( tokens , command , is_any_of(" \t") , token_compress_on );
  if( tokens.empty() ) { return 0; }
  //
  string command_name = tokens.front();
  // look for complete match to list of available commands
  CommandList::iterator i = g_commands.find( command_name );
  if( i!=g_commands.end() ) { return i->second; }
  //
  // full command not found.  try to identify single possible name anyway.
  int nmatches = 0;
  AbsCommand* result = 0;
  for( CommandList::iterator ic=g_commands.begin(), fc=g_commands.end(); ic!=fc; ++ic ) {
    AbsCommand* cmd = ic->second;
    if( istarts_with( cmd->name() , command ) ) { 
      result = cmd;
      ++nmatches;
    }
  }
  if( nmatches>1 ) { 
    cout << command << " is ambiguous." << endl;
    result = 0;
  }
  return result;
}


// how to plot each type of graphics object

// change the histogram color
class
PlotColor : public AbsPlot
{
protected:
  int _color;
public:
  PlotColor( const int& col ) : _color( col ) , AbsPlot("plot color adjustment","","") {}
  virtual const TObject* draw() const {
    g_plot_dominant_color = _color;
    return 0;
  }
};

// change the histogram style
class
PlotStyle : public AbsPlot
{
protected:
  HistogramStyle _style;
public:
  PlotStyle( const HistogramStyle& sty ) : _style( sty ) , AbsPlot("plot style adjustment","","") {}
  virtual const TObject* draw() const {
    g_histogram_style = _style;
    return 0;
  }
};

class 
PlotStyleCommand : public AbsCommand
{
protected:
  virtual void operator()( string& command , const HistogramStyle& sty ) {
    shared_ptr<PlotStyle> p( make_shared<PlotStyle>( sty ) );
    p->draw();
    g_plot_stack.push_back( p );
  }
public:
  PlotStyleCommand() : AbsCommand( "style" , "change plot style" ) {}
  static void default_format( TH1* h , const unsigned int& c ) {
    h->SetLineColor(c);
    h->SetMarkerColor(c);
    h->SetLineWidth(g_plot_data_linewidth);
  }
  static void default_format_2d( TH2* h , const unsigned int& c ) {
    h->SetLineColor(c);
    h->SetMarkerColor(c);
    //    h->SetFillStyle(0);
    h->SetLineWidth(g_plot_data_linewidth);
  }
  static void default_format( TGraph* g , const unsigned int& c ) {
    g->SetLineColor(c);
    g->SetMarkerColor(c);
    g->SetLineWidth(g_plot_data_linewidth);
    g->SetMarkerStyle(8);
    g->SetMarkerSize(0.02);
  }
};

// change an axis range.
class
PlotRange : public AbsPlot
{
public:
  typedef enum { X , Y , Z } Axis;
protected:
  Axis   _axis;
  double _minimum;
  double _maximum;
public: 
  static void SetPlotRange( const Axis& axis , 
                            const double minimum , const double maximum ) {
    if( !g_canvas ) { return; }
    TH1* hframe = 0;
    TIter iPrim( g_canvas->GetListOfPrimitives() );
    // loop over all the histograms.  set the range for every one,
    // e.g. so that calls to Integral will integrate over the proper range.
    while( TObject* obj = iPrim.Next() ) {
      if( obj->InheritsFrom("TH1") ) {
        hframe = dynamic_cast<TH1*>(obj);
        bool isTh1 = hframe->InheritsFrom("TH1");
        bool isTh2 = hframe->InheritsFrom("TH2");
        bool isTh3 = hframe->InheritsFrom("TH3");
        if( isTh2 ) { isTh1 = false; }
        if( isTh3 ) { isTh1 = isTh2 = false; }
        if( isTh3 ) {
          TAxis* taxis = 0;
          if( axis == X ) {
            taxis = hframe->GetXaxis();
          } else if( axis == Y ) {
            taxis = hframe->GetYaxis();
          } else if( axis == Z ) {
            taxis = hframe->GetZaxis();
          } else {
            assert( !"unhandled axis type" );
          }
          assert(taxis);
          Axis_t axmin = std::max( minimum , taxis->GetXmin() );
          Axis_t axmax = std::min( maximum , taxis->GetXmax() );
          taxis->SetRangeUser(axmin,axmax);
        } else if( isTh2 ) {
          TAxis* taxis = 0;
          if( axis == X ) {
            taxis = hframe->GetXaxis();
          } else if( axis == Y ) {
            taxis = hframe->GetYaxis();
          } else {
            assert( !"unhandled axis type" );
          }
          assert(taxis);
//           Axis_t axmin = std::max( minimum , axis->GetXmin() );
//           Axis_t axmax = std::min( maximum , axis->GetXmax() );
//           axis->SetRangeUser(axmin,axmax);
          taxis->SetRangeUser(minimum,maximum);
        } else if( isTh1 ) {
          if( axis == X ) {
            TAxis* taxis = hframe->GetXaxis();
            //             Axis_t axmin = std::max( minimum , axis->GetXmin() );
//             Axis_t axmax = std::min( maximum , axis->GetXmax() );
//             axis->SetRangeUser(axmin,axmax);
            taxis->SetRangeUser(minimum,maximum);
          } else if( axis == Y ) {
//             Axis_t axmin = std::max( minimum , hframe->GetMinimum() );
//             Axis_t axmax = std::min( maximum , hframe->GetMaximum() );            
//             hframe->SetMinimum(axmin);
//             hframe->SetMaximum(axmax);
//             TAxis* axis = hframe->GetXaxis();
//             axmin = std::max( minimum , axis->GetXmin() );
//             axmax = std::min( maximum , axis->GetXmax() );            
//             axis->SetRangeUser(axmin,axmax);
            hframe->SetMinimum(minimum);
            hframe->SetMaximum(maximum);
          } else {
            assert( !"unhandled axis type" );
          }
        }
        // end if this is a histogram
      } else if( obj->InheritsFrom("TGraph") ) {
        TGraph* gr = dynamic_cast<TGraph*>( obj );
        assert( gr );
        if( axis == X ) {
          TAxis* ax = gr->GetXaxis();
          if( ax ) {
            Axis_t axmin = std::max( minimum , ax->GetXmin() );
            Axis_t axmax = std::min( maximum , ax->GetXmax() );
            ax->SetRangeUser( axmin , axmax );
          }
        } else if( axis == Y ) {
          gr->SetMinimum( minimum );
          gr->SetMaximum( maximum );
        } else if( axis == Z ) {
          cout << "will not set Z axis range for TGraph." << endl;
        }
      }
    } // for each plot object
  }
  PlotRange( const Axis& axis , 
             const double minimum = -std::numeric_limits<double>::max() , 
             const double maximum = std::numeric_limits<double>::max() )
    : _axis(axis) , _minimum(minimum) , _maximum(maximum) , AbsPlot("axis adjustment","","") {}
  virtual const TObject* draw() const {
    SetPlotRange( _axis , _minimum , _maximum );
    return 0;
  } // draw function
};


// change an axis label.
class
PlotAxisLabel : public AbsPlot
{
public:
  typedef enum { X , Y , Z } Axis;
protected:
  Axis    _axis;
  string _label;
public: 
  PlotAxisLabel( const Axis& axis , const string& label )
    : _axis(axis) , _label(label) , AbsPlot("axis adjustment","","") {}
  virtual bool isFrame() const { return true; }
  virtual const TObject* draw() const {
    if( !g_canvas ) { return 0; }
    TH1* hframe = 0;
    TIter iPrim( g_canvas->GetListOfPrimitives() );
    // loop over all the histograms.  
    TAxis* axis = 0;
    while( TObject* obj = iPrim.Next() ) {
      if( obj->InheritsFrom("TH1") ) {
        hframe = dynamic_cast<TH1*>(obj);
        bool isTh1 = hframe->InheritsFrom("TH1");
        bool isTh2 = hframe->InheritsFrom("TH2");
        bool isTh3 = hframe->InheritsFrom("TH3");
        if( isTh2 ) { isTh1 = false; }
        if( isTh3 ) { isTh1 = isTh2 = false; }
        if( isTh3 ) {
          if( _axis == X ) {
            axis = hframe->GetXaxis();
          } else if( _axis == Y ) {
            axis = hframe->GetYaxis();
          } else if( _axis == Z ) {
            axis = hframe->GetZaxis();
          } else {
            assert( !"unhandled axis type" );
          }
        } else if( isTh2 ) {
          axis = 0;
          if( _axis == X ) {
            axis = hframe->GetXaxis();
          } else if( _axis == Y ) {
            axis = hframe->GetYaxis();
          } else {
            assert( !"unhandled axis type" );
          }
        } else if( isTh1 ) {
          if( _axis == X ) {
            axis = hframe->GetXaxis();
          } else if( _axis == Y ) {
            axis = hframe->GetYaxis();
          } else {
            assert( !"unhandled axis type" );
          }
        }
        assert(axis);
        axis->SetTitle( _label.c_str() );
        axis->CenterTitle(true);
        axis->SetLabelFont(42);
        axis->SetLabelColor(16);
        break;
      } // if this is a histogram
    } // for each plot object
    return 0;
  } // draw function
};

 
template<class histogramT> 
class
PlotHistogram1D : public histogramT , public AbsPlot
{
public:
  virtual bool isData() const { return true; }
  virtual const TObject* draw() const {
    TH1* h = dynamic_cast<TH1*>( histogramT::new_ROOT( "tmphist" ) );  assert( h );
    h->Scale( _rescale );
    if( _rescale != 1. ) { cout << format(" rescaling %|s| %|s| by %|10g| ") % _description % _subsample % _rescale << endl; }
    draw_root( h , isFirstGraphic() );
    h->SetTitle( (_description+" "+_subsample+" "+_file).c_str() ); // label used for legend
    return h;
  }
  static void draw_root( TH1* h , bool firstGraphic ) {
    if( g_histogram_style == SOLID ) { 
      h->SetFillColor( g_plot_dominant_color );
      h->SetFillStyle( 1001 );
      h->SetLineColor( g_plot_dominant_color );
      h->SetLineWidth( g_plot_data_linewidth );
      h->SetMarkerColor( g_plot_dominant_color );
    } else if( g_histogram_style == HOLLOW ) {
      h->SetFillStyle( 0 );
      h->SetLineColor( g_plot_dominant_color );
      h->SetLineWidth( g_plot_data_linewidth );
      h->SetMarkerColor( g_plot_dominant_color );
    } else if( g_histogram_style == POINTS ) {
      h->SetFillStyle( 0 );
      h->SetLineColor( g_plot_dominant_color );
      h->SetLineWidth( g_plot_data_linewidth );
      h->SetMarkerStyle( 20 );
      h->SetMarkerColor( g_plot_dominant_color );
      h->SetMarkerSize( 1 );
    } else {
      cerr << " unrecognized histogram style in PlotHistogram1D." << endl;
    }
    h->SetAxisColor( g_axis_color );
    h->SetLabelColor( g_axis_label_color );
    h->GetXaxis()->SetAxisColor( g_axis_color );
    h->GetXaxis()->SetLabelColor( g_axis_label_color );
    h->GetYaxis()->SetAxisColor( g_axis_color );
    h->GetYaxis()->SetLabelColor( g_axis_label_color );
    assert( g_canvas );
    static double x_axis_min = 9999;
    static double x_axis_max = -9999;
    static double y_axis_min = 9999;
    static double y_axis_max = 0;
    static TH1* hframe = 0;
    if( firstGraphic ) {
      if( g_histogram_style == POINTS ) {
        if( g_histogram_unitnormalized ) { 
          h->DrawNormalized("e");
        } else {
          h->Draw("e");
        }
      } else {
        if( g_histogram_unitnormalized ) { 
          h->DrawNormalized("");
        } else {
          h->Draw("");
        }
      }
      // reset x axis range to fit available data
      int iminbin=0, imaxbin=h->GetNbinsX()+1;
      for( int iBin=1, fBin=h->GetNbinsX()+1; iBin!=fBin; ++iBin ) {
        if( h->GetBinContent(iBin) > 0 ) {
          iminbin=iBin-1;
          break;
        }
      }
      for( int iBin=h->GetNbinsX(), fBin=0; iBin!=fBin; --iBin ) {
        if( h->GetBinContent(iBin) > 0 ) {
          imaxbin=iBin+1;
          break;
        }
      }
      double minval,maxval;
      minval = (iminbin>0) ? h->GetXaxis()->GetBinLowEdge(iminbin) : h->GetXaxis()->GetBinLowEdge(1);
      maxval = (imaxbin<=h->GetNbinsX()) ? h->GetXaxis()->GetBinLowEdge(imaxbin) + h->GetXaxis()->GetBinWidth(imaxbin) 
        : (h->GetXaxis()->GetBinLowEdge(h->GetNbinsX()) + h->GetXaxis()->GetBinWidth(h->GetNbinsX()));
      //      cout << " minval=" << minval << " maxval=" << maxval << endl;
      h->GetXaxis()->SetRangeUser( minval , maxval );
      x_axis_min = minval;
      x_axis_max = maxval;
      if( h->GetNbinsZ()<2 ) { 
        y_axis_min = h->GetMinimum();
        y_axis_max = h->GetMaximum();
        hframe = h;
      }
    } else {
      if( g_histogram_style == POINTS ) {
        if( g_histogram_unitnormalized ) { 
          h->DrawNormalized("SAME e");
        } else {
          h->Draw("SAME e");
        }
      } else {
        if( g_histogram_unitnormalized ) { 
          h->DrawNormalized("same");
        } else {
          h->Draw("same");
        }
      }
      // reset x axis range to fit available data
      int iminbin=0, imaxbin=h->GetNbinsX()+1;
      for( int iBin=1, fBin=h->GetNbinsX()+1; iBin!=fBin; ++iBin ) {
        if( h->GetBinContent(iBin) > 0 ) {
          iminbin=iBin-1;
          break;
        }
      }
      for( int iBin=h->GetNbinsX(), fBin=0; iBin!=fBin; --iBin ) {
        if( h->GetBinContent(iBin) > 0 ) {
          imaxbin=iBin+1;
          break;
        }
      }
      double minval,maxval;
      if( g_range_mode == AUTO_RANGE ) {
        minval = (iminbin>0) ? h->GetXaxis()->GetBinLowEdge(iminbin) : h->GetXaxis()->GetBinLowEdge(1);
        maxval = (imaxbin<=h->GetNbinsX()) ? h->GetXaxis()->GetBinLowEdge(imaxbin) + h->GetXaxis()->GetBinWidth(imaxbin) 
          : (h->GetXaxis()->GetBinLowEdge(h->GetNbinsX()) + h->GetXaxis()->GetBinWidth(h->GetNbinsX()));
        x_axis_min = std::min(minval,x_axis_min);
        x_axis_max = std::max(maxval,x_axis_max);
      } else if( g_range_mode == FIXED_RANGE ) {
        minval = h->GetXaxis()->GetXmin();
        minval = h->GetXaxis()->GetXmax();
      }
      //      cout << " minval=" << minval << " maxval=" << maxval << endl;
      h->GetXaxis()->SetRangeUser( x_axis_min , x_axis_max );
      // if this is a 1D histogram, scale the y axis to fit.
      if( h->GetNbinsZ()<2 && hframe ) {
        y_axis_min = std::min( h->GetMinimum() , y_axis_min );
        y_axis_max = std::max( h->GetMaximum() , y_axis_max );
        hframe->SetMinimum( y_axis_min );
        hframe->SetMaximum( y_axis_max );
      }
    }
    g_garbage_stack_tobject.push_back( h );
  }
  PlotHistogram1D( const histogramT& h1d , const string& desc , const string& subsample , const string& sourcefile )
    : histogramT(h1d) 
    , AbsPlot(desc,subsample,sourcefile) {}
};

// auto y axis range
class
AutoPlotRange : public AbsPlot
{
protected:
public:
  AutoPlotRange() : AbsPlot("auto plot range","","") {}
  virtual const TObject* draw() const {
    // loop over all plots in the draw stack and choose an appropriate range.
    double minimum = -std::numeric_limits<double>::max(); 
    double maximum = std::numeric_limits<double>::max();
    minimum = 0;
    maximum = 0;
    for( vector< shared_ptr<AbsPlot> >::iterator i=g_plot_stack.begin() , f=g_plot_stack.end(); i!=f; ++i ) {
      if( (*i)->isData() ) {
        shared_ptr<const PlotHistogram1D<DgHistogram1D> > p( dynamic_pointer_cast<const PlotHistogram1D<DgHistogram1D> >( *i ) );
        if( !p ) { continue; }
        maximum = std::max( (double)(p->maximum_count()) , maximum );
      }
    }
    // set the plot range
    PlotRange::SetPlotRange( PlotRange::Y , minimum , maximum*1.1 );
    return 0;
  }
};

class
PlotProfile : public AbsPlot
{
public:
  typedef enum { X , Y } Type;
protected:
  Type _type;
public:
  virtual bool isData() const { return true; }
  virtual const TObject* draw() const {
    TIter iPrim( g_canvas->GetListOfPrimitives() );
    // loop over all the histograms.  profile the first 2d histogram found.
    while( TObject* obj = iPrim.Next() ) {
      if( obj->InheritsFrom("TH2") ) {
        TH2* h = dynamic_cast<TH2*>(obj);
        TProfile* prof = 0;
        TGraphAsymmErrors* graph = new TGraphAsymmErrors;
        TAxis* axis = 0;
        if( _type==Y ) {
          prof = h->ProfileY();
          axis = h->GetYaxis();
        } else {
          prof = h->ProfileX();
          axis = h->GetXaxis();
        }
        assert( axis && prof );
        for( int iBin=1, fBin=axis->GetNbins()+1; iBin!=fBin; ++iBin ) {
          graph->SetPoint(iBin-1,axis->GetBinCenter(iBin),prof->GetBinContent(iBin));
          graph->SetPointError(iBin-1,0.,0.,prof->GetBinError(iBin),prof->GetBinError(iBin));
        }
        gStyle->SetLineColor(16);
        graph->SetMarkerColor(kBlack);
        graph->SetMarkerStyle(8);
        graph->SetMarkerSize(0.4);
        graph->SetLineColor(kBlack);
        graph->Draw("p");
        g_garbage_stack_tobject.push_back( prof );
        g_garbage_stack_tobject.push_back( graph );
        graph->SetTitle( _description.c_str() ); // label used for legend
        return graph; // was return prof
        break;
      }
    }
    return 0;
  }
  PlotProfile( const Type& type , const string& desc , const string& subsample , const string& sourcefile ) 
    : _type(type) , AbsPlot(desc,subsample,sourcefile) {}
};


template<class graph0dT>
class
PlotNumber : public graph0dT , public AbsPlot
{
public:
  virtual bool isData() const { return true; }
  virtual const TObject* draw() const {
    TObject* obj = graph0dT::new_ROOT( "tmpgraph" );
    if( !obj ) { return 0; }
    TGraphAsymmErrors* g = dynamic_cast<TGraphAsymmErrors*>( obj );  assert( g );
    int n = g->GetN();
    if( n!=1 ) { delete obj; return 0; }
    cout << format( " %|10g| +%|10g| -%|10g|" ) % g->GetY()[0] % g->GetErrorYhigh(0) % g->GetErrorYlow(0) << endl;
    g_garbage_stack_tobject.push_back( g );
    g->SetTitle( _description.c_str() ); // label used for legend
    return g;
  }
  PlotNumber( const graph0dT& gr , const string& desc , const string& subsample , const string& sourcefile )
    : graph0dT(gr) 
    , AbsPlot(desc,subsample,sourcefile) {}
};

template<typename baseT>
TGraph* get_tgraph( const baseT& dgobj , const unsigned int dummy ) {
  return dynamic_cast<TGraph*>( dgobj.new_ROOT( "tmpgraph" ) );    
}
template<>
TGraph* get_tgraph<DgProfile1D>( const DgProfile1D& dgprof , const unsigned int dummy ) {
  return dynamic_cast<TGraph*>( dgprof.new_ROOT( "tmpprof" ) );    
}
template<>
TGraph* get_tgraph<DgEfficiency1D>( const DgEfficiency1D& dgeff , const unsigned int bin_group_size ) {
  return dynamic_cast<TGraph*>( dgeff.new_ROOT( "efficiency" , bin_group_size ) );
}  
// template<>
// TGraph* get_tgraph<DgEfficiency2D>( const DgEfficiency2D& dgeff , const unsigned int dummy ) {
//   return dynamic_cast<TGraph*>( dgeff.new_ROOT( "efficiency" , 1 ) );
// }

template<class graphT>
class
PlotGraph : public graphT , public AbsPlot
{
private:
  bool _override_y_range;
  double _override_miny;
  double _override_maxy;
  unsigned int _bin_group_size;
public:
  virtual bool isData() const { return true; }
  virtual TObject* process( TGraph*& g ) const {
    int n = g->GetN();
    if( n < 1 ) { delete g; return 0; }
    g->SetLineColor( g_plot_dominant_color );
    g->SetMarkerColor( g_plot_dominant_color );
    g->SetTitle( _description.c_str() ); // label used for legend
    assert( g_canvas );
    if( isFirstGraphic() ) {
      // make frame histogram, then draw
      // get auto-range.
      const Double_t* x = g->GetX();
      const Double_t* y = g->GetY();
      double minx = *(min_element(x,x+n));
      double maxx = *(max_element(x,x+n));
      double miny = *(min_element(y,y+n));
      double maxy = *(max_element(y,y+n));
      double f_minx = maxx - (2*(maxx-minx));
      double f_maxx = minx + (2*(maxx-minx));
      double f_miny = maxy - (2*(maxy-miny));
      double f_maxy = miny + (2*(maxy-miny));
      if( _override_y_range ) {
        miny = f_miny = _override_miny;
        maxy = f_maxy = _override_maxy;
      }
      TH2* hframe = new TH2F( "tmpframehist" , "" , 200 , f_minx , f_maxx , 200 , f_miny , f_maxy );
      hframe->GetXaxis()->SetRangeUser( minx , maxx );
      hframe->GetYaxis()->SetRangeUser( miny , maxy );
      //       hframe->SetMinimum( miny );
      //       hframe->SetMaximum( maxy );
      hframe->SetAxisColor( g_axis_color );
      hframe->SetLabelColor( g_axis_label_color );
      hframe->SetLineColor( g_axis_color );
      hframe->GetXaxis()->SetTitle( graphT::get_axis_label().c_str() ); // hack to fix missing titles for new TGraphs that haven't been drawn yet.
      if( dynamic_cast<const DgEfficiency1D*>(this) || dynamic_cast<const DgEfficiency0D*>(this) ) {
        hframe->GetYaxis()->SetTitle( "EFFICIENCY" ); // hack to fix missing titles for new TGraphs that haven't been drawn yet.
      }
      hframe->Draw("AH");
      //g_garbage_stack_tobject.push_back( hframe ); // deleted with TGraph?
      gStyle->SetLineColor(16);
      assert(0 && "Fix and then uncomment the following line. It was causing failures in 64-bit ROOT 5.30 builds");
      //g->SetHistogram(hframe);
      g->SetMarkerColor(kBlack);
      g->SetMarkerStyle(2);
      g->SetMarkerSize(1);
      g->SetLineColor(18);
      g->SetMarkerColor(kBlack);
      g->SetLineColor(kBlack);
      g->SetLineWidth( g_plot_data_linewidth );
      g->Draw("apo");
    } else {
      // just draw on top of existing frame
      g->Draw("p");
    }
    g_garbage_stack_tobject.push_back( g );
    return g;
  }
  virtual const TObject* draw() const {
    TGraph* g = get_tgraph<graphT>(*this,_bin_group_size); assert( g );
    if( !g ) { g = new TGraph; }
    return process( g );
  }
  PlotGraph( const graphT& gr , const string& desc , const string& subsample , const string& sourcefile )
    : graphT(gr) 
    , AbsPlot(desc,subsample,sourcefile) 
    , _override_y_range( false )
    , _override_miny( 0. )
    , _override_maxy (1. )
    , _bin_group_size( 1u )
  {}
  void efficiency_range() { 
    _override_y_range = true;
    _override_miny = 0.;
    _override_maxy = 1.;
  }
  void override_y_range( const double& miny , const double& maxy ) { 
    _override_y_range = true;
    _override_miny = miny;
    _override_maxy = maxy;
  }
  void set_bin_group_size( const unsigned int& val ) { _bin_group_size = val; }
};

template<class graphT>
class
PlotProfile1D : public PlotGraph<graphT>
{
protected:
  string _mode;
public:
  PlotProfile1D( const graphT& gr , const string& desc , const string& subsample , const string& sourcefile , const string& mode )
    : PlotGraph<graphT>(gr,desc,subsample,sourcefile)
    , _mode(mode)
  {}
  virtual TGraph* get_tgraph() const { 
    TGraph* gr = 0;
    const bool do_default = !( istarts_with(_mode,"mean") || istarts_with(_mode,"rms") );
    if( istarts_with(_mode,"prof") || do_default ) {
      unsigned int ipoint = 0;
      TGraphAsymmErrors* gras = new TGraphAsymmErrors;
      // g_garbage_stack_tobject.push_back( gras );  // handled by PlotGraph::process
      for( BinSize ibin=0; ibin!=graphT::nbins(); ++ibin ) {
        if( graphT::bin_weight(ibin) > 0. ) {
          gras->SetPoint( ipoint , graphT::bin_center(ibin) , graphT::bin_mean(ibin) );
          const double rms = graphT::bin_rms(ibin);
          gras->SetPointError( ipoint , 0.0001 , 0.0001 , rms , rms );
          ++ipoint;
        }
      }
      gr = gras;
    } else if( istarts_with(_mode,"mean") ) {
      gr = new TGraph();
      // g_garbage_stack_tobject.push_back( gr );  // handled by PlotGraph::process
      unsigned int ipoint = 0;
      for( BinSize ibin=0; ibin!=graphT::nbins(); ++ibin ) {
        if( graphT::bin_weight(ibin) > 0. ) {
          gr->SetPoint( ipoint++ , graphT::bin_center(ibin) , graphT::bin_mean(ibin) );
        }
      }
    } else if( istarts_with(_mode,"rms") ) {
      gr = new TGraph();
      // g_garbage_stack_tobject.push_back( gr ); // handled by PlotGraph::process
      unsigned int ipoint = 0;
      for( BinSize ibin=0; ibin!=graphT::nbins(); ++ibin ) {
        if( graphT::bin_weight(ibin) > 0. ) {
          cout << format( "%|3d| %|10g| %|10g| %|10g| %|10g| %|10g|" )
            % ibin
            % graphT::bin_center(ibin) 
            % graphT::bin_rms(ibin)
            % graphT::bin_weight(ibin)
            % graphT::bin_sumy(ibin)
            % graphT::bin_sumysq(ibin)
               << endl;
          gr->SetPoint( ipoint++ , graphT::bin_center(ibin) , graphT::bin_rms(ibin) );
        }
      }
    } else {
      gr = new TGraph;
      // g_garbage_stack_tobject.push_back( gr );  // handled by PlotGraph::process
    }
    gr->SetName( "Graph" );
    return gr;
  }
  virtual const TObject* draw() const {
    TGraph* g = get_tgraph(); assert( g );
    if( !g ) { g = new TGraph; }
    return PlotGraph<graphT>::process( g );
  }
};

class
PlotHistogram2D : public DgHistogram2D , public AbsPlot
{
public:
  typedef enum { STANDARD , XPROJECTION , YPROJECTION } Mode;
protected:
  Mode _mode;
  int _minimum_bin;
  int _maximum_bin;
public:
  virtual double integral() const {
    double total( 0. );
    for( int ibiny = 0; ibiny<_nbinsy; ++ibiny ) {
      for( int ibinx = 0; ibinx<_nbinsx; ++ibinx ) {
        int ibin = (ibiny*_nbinsx)+ibinx;
        total += _count[ibin];
      }
    }
    total += ( _outofbounds_xminymin + _outofbounds_ymin +
               _outofbounds_xmaxymin + _outofbounds_xmin +
               _outofbounds_xmaxymin + _outofbounds_xminymax +
               _outofbounds_ymax + _outofbounds_xmaxymax );
    return total;
  }
  virtual bool isData() const { return true; }
  void set_projection_limits( const int& min_bin , const int& max_bin ) {
    _minimum_bin = min_bin;
    _maximum_bin = max_bin;
  }
  virtual const TObject* draw() const {
    TH2* h = dynamic_cast<TH2*>( new_ROOT( "tmphist" ) );  assert( h );
    g_garbage_stack_tobject.push_back( h );
    assert( g_canvas );
    h->SetTitle( _description.c_str() ); // label used for legend
    if( _mode == STANDARD ) {
      if( isFirstGraphic() ) {
        h->Draw("colz");
      } else {
        cout << " I don't know how to plot a 2D histogram on top of another object." << endl;
      }
    } else if( _mode == XPROJECTION || _mode == YPROJECTION ) {
      TH1* h1 = 0;
      if( _mode == XPROJECTION ) {
        h1 = h->ProjectionX("px",_minimum_bin,_maximum_bin);
      } else if( _mode == YPROJECTION ) {
        h1 = h->ProjectionY("py",_minimum_bin,_maximum_bin);
      }
      if( h1 ) { PlotHistogram1D<DgHistogram1D>::draw_root(h1,isFirstGraphic()); }
      g_garbage_stack_tobject.push_back( h1 );
      return h1;
    }
    return h;
  }
  TH1* slice_x( const unsigned int& ibin ) const {
    TH2* h = dynamic_cast<TH2*>( new_ROOT( "tmphist" ) );  assert( h );    
    TH1* slice_h = h->ProjectionY("tmphist_slice_x",ibin,ibin);
    delete h;
    g_garbage_stack_tobject.push_back( slice_h );
    return slice_h;
  }
  const unsigned int nbinsx() const { return _nbinsx; }
  const unsigned int nbinsy() const { return _nbinsy; }
  const double miny() const { return _miny; }
  const double maxy() const { return _maxy; }
  const double minx() const { return _minx; }
  const double maxx() const { return _maxx; }
  PlotHistogram2D( const DgHistogram2D& h2d , const string& desc , const string& subsample , const string& sourcefile ,const Mode mode = STANDARD )
    : DgHistogram2D(h2d) 
    , _mode(mode)
    , _minimum_bin(0)
    , _maximum_bin(std::numeric_limits<int>::max())
    , AbsPlot(desc,subsample,sourcefile) {}
};

class
PlotEfficiency2D : public DgEfficiency2D , public AbsPlot
{
public:
  virtual double integral() const {
    double total( 0. );
    for( int ibiny = 0; ibiny<_nbinsy; ++ibiny ) {
      for( int ibinx = 0; ibinx<_nbinsx; ++ibinx ) {
        int ibin = (ibiny*_nbinsx)+ibinx;
        total += _count[ibin];
      }
    }
    total += ( _outofbounds_xminymin + _outofbounds_ymin +
               _outofbounds_xmaxymin + _outofbounds_xmin +
               _outofbounds_xmaxymin + _outofbounds_xminymax +
               _outofbounds_ymax + _outofbounds_xmaxymax );
    return total;
  }
  virtual bool isData() const { return true; }
  virtual const TObject* draw() const {
    TH2* h = dynamic_cast<TH2*>( new_ROOT( "tmphist" ) );  assert( h );
    g_garbage_stack_tobject.push_back( h );
    assert( g_canvas );
    h->SetTitle( _description.c_str() ); // label used for legend
    if( isFirstGraphic() ) {
      h->Draw("colz");
    } else {
      cout << " I don't know how to plot a 2D efficiency histogram on top of another object." << endl;
    }
    return h;
  }
  const unsigned int nbinsx() const { return _nbinsx; }
  const unsigned int nbinsy() const { return _nbinsy; }
  const double miny() const { return _miny; }
  const double maxy() const { return _maxy; }
  const double minx() const { return _minx; }
  const double maxx() const { return _maxx; }
  PlotEfficiency2D( const DgEfficiency2D& h2d , const string& desc , const string& subsample , const string& sourcefile )
    : DgEfficiency2D(h2d) 
    , AbsPlot(desc,subsample,sourcefile) {}
};

// command definitions ////////////////////////////////////////////////////////////////

class 
PwdCommand : public AbsCommand
{
protected:
  virtual void operator()( string& command ) { pwd(); }
public:
  PwdCommand() : AbsCommand( "pwd" , "show graphics current collection" ) {}
  static void pwd() { 
    stack<string> hierarchy;
    const DgCollection* coll = dg::current();
    do {
      string line(coll->name());
      line += ",  ";
      line += coll->description();
      hierarchy.push( line );
    } while( coll!=dg::root() && (coll=coll->parent()) );
    int indent = 0;
    while( !hierarchy.empty() ) {
      cout << "   ";
      for( int i=0; i<indent; ++i ) {
        cout << "  ";
      }
      cout << hierarchy.top() << endl;
      hierarchy.pop();
      ++indent;
    }
  }
};

class 
LsCollCommand : public AbsCommand
{
protected:
  virtual void operator()( string& command ) { ls(); }
public:
  static void ls() { 
    const dg::ResourceColl& coll = dg::current()->graphics();
    const dg::SubColl& subcoll = dg::current()->subcollections();
    std::vector<string> graphic_names;
    std::vector<string> coll_names;
    for( dg::ResourceColl::const_iterator icoll=coll.begin(), fcoll=coll.end(); icoll!=fcoll; ++icoll ) {
      graphic_names.push_back( dg::current()->graphic_name(icoll) );
    }
    for( dg::SubColl::const_iterator isubcoll=subcoll.begin(), fsubcoll=subcoll.end(); isubcoll!=fsubcoll; ++isubcoll ) {
      coll_names.push_back( dg::current()->subcollection_name(isubcoll) );
    }
    // sort directory entries alphabetically.
    std::sort( graphic_names.begin() , graphic_names.end() );
    std::sort( coll_names.begin() , coll_names.end() );
    std::vector<string>::const_iterator icoll=graphic_names.begin(), fcoll=graphic_names.end();
    std::vector<string>::const_iterator isub=coll_names.begin(), fsub=coll_names.end();
    while( icoll!=fcoll || isub!=fsub ) {
      cout << "    " ;
      if( icoll!=fcoll ) {
        cout << setiosflags(ios::left) << setw(50) << (*icoll) << resetiosflags(ios::left);
        ++icoll;
      } else {
        cout << setw(50) << "  ";
      }
      if( isub!=fsub ) {
        cout << setw(20) << "  ";
        cout << setiosflags(ios::left) << setw(40) << (*isub) << resetiosflags(ios::left);
        ++isub;
      }
      cout << endl;
    }
  }
  LsCollCommand() : AbsCommand( "ls" , "list graphics objects in the current collection" ) {}
};

class 
RootCollCommand : public AbsCommand
{
protected:
  virtual void operator()( string& command ) { 
    dg::root()->cd();
    PwdCommand::pwd();
  }
public:
  RootCollCommand() : AbsCommand( "home" , "change to root collection" ) {};
};

class 
CdCommand : public AbsCommand
{
protected:
  virtual void operator()( string& command ) {
    const char* collname = command.c_str();
    boost::regex patt( first_token(command) );
    // look for matching subdirectories
    vector<string> choices_string;
    for( dg::SubColl::const_iterator i=dg::current()->subcollections().begin(), f=dg::current()->subcollections().end(); i!=f; ++i ) {
      string subname = dg::current()->subcollection_name(i);
      if( command=="" || regex_match( subname , patt ) ) {
        choices_string.push_back(subname);
      }
    }
    vector<string> choices_unsorted( choices_string );
    std::sort( choices_string.begin() , choices_string.end() );
    vector<string> choices( choices );
    // list subcollections and let the user choose
    if( choices.empty() ) { cout << " no matches." << endl; return; }
    int choice = 1;
    if( choices.size()>1 ) { 
      choice = chooseFromList( choices );
      if( choice<1 ) { return; }
      vector<string>::const_iterator ci = find( choices_unsorted.begin() , choices_unsorted.end() , choices[choice-1] );
      assert( ci != choices_unsorted.end() );
      dg::current()->cd( (*ci).c_str() );
    } else {
      dg::current()->cd( (*choices.begin()) );
    }
    PwdCommand::pwd();
    LsCollCommand::ls();
  }
public:
  CdCommand()
    : AbsCommand( "cd" , "change the graphics current collection" ) {}
};

class
QuitCommand : public AbsCommand
{
protected:
  void operator()( string& command ) { 
    string cmd(command);
    if( opt_yes.value_bool() ) { 
      g_quit = true;
    } else {
      if( askYesNo("really quit?") ) {
        g_quit = true;
      }
    }
  }
public:
  CommandOption opt_yes;
public:
  QuitCommand() 
    : AbsCommand( "quit" , "exit the program" )
    , opt_yes( this , CommandOption::BOOL , "y" , "yes, quit" )
  {
  }
};

// save the current canvas.
class
SaveCommand : public AbsCommand
{
protected:
  void operator()( string& command ) { save(command); }
public:
  SaveCommand() : AbsCommand( "save" , "save the current canvas" ) {}
  static void save( string& command ) {
    if( !g_canvas ) { cout << " no canvas to save." << endl; return; }
    string plotname = first_token( command );
    g_canvas->Print( plotname.c_str() );
  }
};

// save current collection and all subcollections as ROOT objects in a file.
class
SaveCollectionAsROOTCommand : public AbsCommand
{
protected:
  void operator()( string& command ) { save(command); }
public:
  SaveCollectionAsROOTCommand() : AbsCommand( "convertroot" , "save the current collection and all subcollections as a ROOT file" ) {}
  static void save( string& command) {
    const string filename = first_token( command );
    TFile* f = TFile::Open( filename.c_str() , "NEW" );
    if( !f ) { cout << " could not open " << filename.c_str() << " for output; maybe it already exists." << endl; return; }
    DgCollection* coll = dg::current();
    if( !coll ) { coll = dg::root(); }
    coll->save_as_ROOT( f );
    f->Close();
  }
};


// show the current canvas. if no argument, show in an X11 window.
// otherwise, any argument(s) provided are filenames which 
// should receive a TCanvas::Print(filename);
class
ShowCommand : public AbsCommand
{
public:
  static TVirtualPad* current_pad() { return TVirtualPad::Pad(); }
  static TVirtualPad* current_canvas() { return g_canvas; }
  static void new_canvas() {
    // delete the previous canvas
    if( g_canvas ) {
      delete g_canvas;
      g_canvas = 0;
    }
    // clean up the garbage
    unsigned int remaining_garbage = 0u;
    BOOST_FOREACH( TObject* ptr , g_garbage_stack_tobject ) {
      //try { cout << ((TNamed*)ptr)->GetName() << " " << ((TNamed*)ptr)->GetTitle() << endl; } catch(...) {}
      delete ptr;
    }
    g_garbage_stack_tobject.clear();
    BOOST_FOREACH( AbsPlot* ptr , g_garbage_stack_absplot ) {
      delete ptr;
    }
    g_garbage_stack_absplot.clear();
    BOOST_FOREACH( boost::any ptr , g_garbage_stack_any ) {
      try {
        DgGxLegend* lg = boost::any_cast<DgGxLegend*>(ptr);
        //cout << " deleting DgGxLegend" << endl;
        delete lg;
      } catch( const boost::bad_any_cast & ) {
        ++remaining_garbage;
      }
    }
    if( remaining_garbage>0 ) { cout << " warning: some garbage (" << remaining_garbage << ")could not be deleted." << endl; }
    vector<boost::any> tmp;
    g_garbage_stack_any.swap( tmp );
    g_garbage_stack_any.clear();

    double nheight = g_display_height;
    double nwidth = nheight*g_aspect_ratio;
    if( nwidth > (g_display_width*31./32.) ) {
      nwidth = std::floor(g_display_width*31/32.);
      nheight = nwidth / g_aspect_ratio;
    }
//     double nwidth = 1024;
//     double nheight = nwidth/g_aspect_ratio;
//     if( nheight > 700. ) {
//       nheight = 700.;
//       nwidth = 700*g_aspect_ratio;
//     }
    assert( !g_canvas );
    g_canvas = new TCanvas( "dg_plot" , "" , (int)floor(nwidth) , (int)floor(nheight) );
    g_canvas->UseCurrentStyle();
    if( !_xshow || !_show_override ) { 
      gROOT->SetBatch();
      g_canvas->SetBatch(); 
    } else {
      gROOT->SetBatch( false );
      g_canvas->SetBatch( false ); 
    }
  }
  static void update_canvas() {
    if( !g_canvas ) { return; }
    g_canvas->Modified();
    g_canvas->Update();
  }
protected:
  void operator()( string& cmd ) { 
    show( opt_logx.value_bool() , opt_logy.value_bool() , opt_logz.value_bool() ,
          opt_unit.value_bool() , opt_solidify.value_bool() );
  }
  static bool _xshow;
  static bool _show_override; // used by batch mode.
public:
  CommandOption opt_logx;
  CommandOption opt_logy;
  CommandOption opt_logz;
  CommandOption opt_unit;
  CommandOption opt_solidify;
public:
  ShowCommand() 
    : opt_logx( this , CommandOption::BOOL , "logx" , "x log scale" )
    , opt_logy( this , CommandOption::BOOL , "logy" , "y log scale" )
    , opt_logz( this , CommandOption::BOOL , "logz" , "z log scale" )
    , opt_unit( this , CommandOption::BOOL , "unit" , "normalize data object to unit area" )
    , opt_solidify( this , CommandOption::BOOL , "solidify" , "force solid histograms" )
    , AbsCommand( "show" , "show the current canvas" ) {}
  static void xshow( const bool& yes ) { _xshow = yes; }
  static void show_override( const bool& yes ) { _show_override = yes; }
  static void show( const bool logx = false , const bool logy = false , const bool logz = false , const bool unit = false , const bool solidify = false ) {
    if( g_plot_stack.empty() ) {
      cout << " nothing to show." << endl;
      return;
    }
    new_canvas();
    bool firstFrame = true;
    bool dataToShow = false;
    // count number of graphics to draw
    unsigned int ndata = 0;
    unsigned int nmc = 0;
    for( vector< shared_ptr<AbsPlot> >::iterator i=g_plot_stack.begin() , f=g_plot_stack.end(); i!=f; ++i ) {
      if( (*i)->isData() ) { ++ndata; }
      const bool ismc = false;
      if( ismc ) { ++nmc; }
    }
    unsigned int graphic_number = 0;
    // legend for multiple graphic objects
    DgGxLegend* legend = new DgGxLegend( g_canvas );
    g_garbage_stack_any.push_back( boost::any(legend) );
    vector<format> desc_lines;
    for( vector< shared_ptr<AbsPlot> >::iterator i=g_plot_stack.begin() , f=g_plot_stack.end(); i!=f; ++i ) {
      desc_lines.push_back( format("%|50s| %|20s| %|100s|") % (*i)->source_file() % (*i)->subsample() % (*i)->description() );
      // set plot color
      int tmp1 = g_plot_dominant_color;
      int tmp4 = g_plot_data_linewidth;
      g_plot_data_linewidth = 3;
      if( (*i)->override_color() ) { g_plot_dominant_color = (*i)->get_override_color(); }
      if( graphic_number==0 ) { 
      } else if( graphic_number<g_plot_palette.size() ) { 
        g_plot_dominant_color = g_plot_palette[graphic_number];
      } else { 
        g_plot_dominant_color = 11+((graphic_number-g_plot_palette.size())*2);
      }
      // if( g_overlay_mode==DEFAULT_OVERLAY ) {
      //   switch( graphic_number ) {
      //   case 0: 
      //     break;
      //   case 1:
      //     g_plot_dominant_color = kRed; break;
      //   case 2:
      //     g_plot_dominant_color = kBlue; break;
      //   default:
      //     g_plot_dominant_color = 11+((graphic_number-g_plot_palette.size())*2);
      //   }
      // } else if( g_overlay_mode==DATAMC_OVERLAY ) {
      //   switch( graphic_number ) {
      //   case 0: 
      //     g_plot_dominant_color = g_plot_palette[0]; break;
      //   case 1:
      //     g_plot_dominant_color = g_plot_palette[1]; break;
      //   case 2:
      //     g_plot_dominant_color = g_plot_palette[2]; break;
      //   case 3:
      //     g_plot_dominant_color = g_plot_palette[3]; break;
      //   default:
      //     g_plot_dominant_color = 11+((graphic_number-g_plot_palette.size())*2);
      //   }
      // } else if( g_overlay_mode==COMPARISON_OVERLAY || g_overlay_mode==SHAPE_COMPARISON_OVERLAY ) {
      //   if( ndata<=g_plot_palette.size() ) { 
      //     g_plot_dominant_color = g_plot_palette[graphic_number]; break;
      //   } else { 
      //     g_plot_dominant_color = 11+((graphic_number-g_plot_palette.size())*2);
      //   }
      // }
      // draw
      const bool pop_unitnorm = g_histogram_unitnormalized;
      g_histogram_unitnormalized = unit;
      const bool ismc = false;
      if( !ismc && ndata>1 && g_overlay_mode==DEFAULT_OVERLAY ) {
        HistogramStyle tmp2 = g_histogram_style;
        g_histogram_style = solidify ? SOLID : HOLLOW;
        legend->add( (*i)->draw() );
        g_histogram_style = tmp2;
      } else if( ismc && ndata==2 && nmc==1 && g_overlay_mode==DEFAULT_OVERLAY ) {
        HistogramStyle tmp2 = g_histogram_style;
        g_histogram_style = SOLID;
        legend->add( (*i)->draw() );
        g_histogram_style = tmp2;
      } else if( COMPARISON_OVERLAY || SHAPE_COMPARISON_OVERLAY) {
        HistogramStyle tmp2 = g_histogram_style;
        if( graphic_number==0 ) { 
          g_histogram_style = SOLID;
        } else {
          g_histogram_style = HOLLOW;
        }
        legend->add( (*i)->draw() );
        g_histogram_style = tmp2;        
      } else {
        legend->add( (*i)->draw() );
      }
      g_histogram_unitnormalized = pop_unitnorm;
      if( (*i)->isData() ) { ++graphic_number; }
      g_plot_dominant_color = tmp1;
      g_plot_data_linewidth = tmp4;
      // set frame
      if( (*i)->isData() && firstFrame ) {
        // get frame histogram.  if you're writing a graph or something,
        // create a frame histogram first.
        TH1* hframe = 0;
        TIter iPrim( g_canvas->GetListOfPrimitives() );
        while( TObject* obj = iPrim.Next() ) {
          if( obj->InheritsFrom("TH1") ) {
            hframe = dynamic_cast<TH1*>(obj);
            break;
          }
        }
        if( hframe ) {
          if( logx ) { g_canvas->SetLogx(); }
          if( logy ) { 
            g_canvas->SetLogy(); 
            if( g_histogram_unitnormalized ) { 
              hframe->SetMinimum(0.0001); 
            } else {
              hframe->SetMinimum(0.0001); // be sure to show bins with 1 event.
            }
          }
          if( logz ) {
            g_canvas->SetLogz(); 
          }
          g_canvas->Modified();
          firstFrame = false;
          if( g_histogram_unitnormalized ) {
            hframe->SetMinimum(0.0001);
            hframe->SetMaximum(1.);
          }
        }
      }
    }
    //
    g_canvas->Draw();
    g_canvas->Update();
    if( ndata>1 ) {
      TPad* pad = g_canvas;
      std::vector< boost::shared_ptr<boost::any> > tmpp;
      legend->draw( pad , tmpp ); // FIXME
    }
    g_canvas->Update();
    //
    // if( first_token(command) != "" ) {
    //   if( first_token(command) != "unit" && first_token(command) != "solidify" ) {
    //     SaveCommand::save(command);
    //   }
    // }
    update_canvas();
    cout << " show: " << endl;
    BOOST_FOREACH( const format& fmt , desc_lines ) { 
      cout << fmt << endl;
    }
  }
};
bool ShowCommand::_xshow = true;
bool ShowCommand::_show_override = true;

template<class graphicsT>
class
PlotGroupStack : public graphicsT , public AbsPlot
{
private:
  string _path_to_graphic;
  typedef shared_ptr<graphicsT> graphicsTptr;
  typedef pair< shared_ptr<PlotGroup> , graphicsTptr > plotVecValue;
  typedef vector< plotVecValue > plotVec;
  mutable plotVec _stack;
  mutable plotVec _overlay;
private:
  // get sample graphic given the corresponding PlotGroup
  graphicsTptr get_sample_graphic( shared_ptr<PlotGroup>& pg ) const {
    assert( pg->type() == PlotGroup::SAMPLE );
    if( ! pg->enabled() ) { return graphicsTptr(); }
    // retrieve the graphics object corresponding to this 
    DgCollection* start_dir( dg::current() );
    // find DescValue corresponding to this sample
    DescColl::iterator idesc = g_descriptions.find( pg->description_name() );
    // DescColl::iterator idesc = find_if( g_descriptions.begin() , g_descriptions.end() ,
    //                                     bind(&DgXMLSampleDescription::name,bind(&DescValue::description,_1))==pg->description()->name() );
    // if the description is not present, the sample is not
    // present. this may happen if the user has loaded a
    // predefined group tree but not one or more of the expected
    // samples. warn the user the sample is missing and then skip it.
    if( idesc == g_descriptions.end() ) {
      cout << " (warning) sample " << pg->description_name() << " not loaded" << endl;
      return graphicsTptr();
    }
    DgCollection* coll = (*idesc).second.coll;
    // retreive the graphic using the path
    coll->cd(); 
    // if( g_debug_mode ) { 
    //   cout << " coll for graphic: " << coll << " " << (coll ? coll->name() : string("not found")) << endl;
    //   cout << " current: " << dg::current() << " " << (dg::current() ? dg::current()->name() : string("not found")) << endl;
    // }
    // string fullpath( _path_to_graphic );
    string fullpath( string("dg/dg/")+_path_to_graphic );
    DgCollection::View view( dg::get( fullpath ) );
    // if( g_debug_mode ) { 
    //   cout << " path to graphic: " << fullpath << endl;
    //   cout << " found " << view.size() << " graphics at that path" << endl;
    // }
    if( view.empty() || view.size()>1 ) {
      cout << " could not find graphic for " << pg->description_name() << endl;
      cout << "                at " << _path_to_graphic << endl;
      start_dir->cd();
      return graphicsTptr();
    }
    if( g_debug_mode ) { 
      // cout << "  returning graphic: " << dynamic_pointer_cast<graphicsT>(view.front().graphic()) << endl;
    }      
    start_dir->cd();
    return dynamic_pointer_cast<graphicsT>(view.front().graphic());
  } // end get_sample_graphic
  // index is passed by reference; a counter incremented each time a new group stack is created.
  void build_groups( shared_ptr<PlotGroup> group = shared_ptr<PlotGroup>() ) const {
    if( !group ) {
      _stack.clear();
      _overlay.clear();
    }
    if( group ) { assert( group->type() != PlotGroup::SAMPLE ); }
    if( group && group->type() == PlotGroup::STACK && group->group().empty() ) { // this is a single sample, and we're supposed to stack it along with the rest.
      shared_ptr<PlotGroup> pg( new PlotGroup(*group) );
      pg->set_type( PlotGroup::SAMPLE );
      graphicsTptr ptr =  get_sample_graphic( pg );
      _stack.push_back( pair< shared_ptr<PlotGroup> , graphicsTptr >( pg , ptr ) ); // assign parent group
      return;
    }
    PlotGroup::Coll::reverse_iterator i = group ? group->group().rbegin() : g_plot_group_tree.rbegin();
    PlotGroup::Coll::reverse_iterator f = group ? group->group().rend() : g_plot_group_tree.rend();
    for( ; i!=f; ++i ) {
      if( ! (*i)->enabled() ) { continue; }
      if( (*i)->type() != PlotGroup::SAMPLE && (*i)->type() != PlotGroup::MERGE ) {
        build_groups( *i );
      } else if( (*i)->type() == PlotGroup::MERGE ) {
        // don't descend. instead, collect the objects and sum them
        // now. if data, collect luminosities.
        shared_ptr< typename remove_const<graphicsT>::type > summed;
        vector< float > sum_lumi;
        unsigned int nmerge = 0u;
        if( g_debug_mode ) { 
          cout << " encountered merge group with " << (*i)->group().size() << " components" << endl;
        }
        unsigned int n_with_graphic = 0u;
        BOOST_FOREACH( shared_ptr<PlotGroup>& pgg , (*i)->group() ) {
          graphicsTptr pj( get_sample_graphic(pgg) );
          // check whether pj is null or not. null may indicate a
          // serious problem, or it may simply indicate that there was
          // no relevant data found in this particular subsample. in
          // the latter case, we still want to sum the luminosity. for
          // example, the graphic may be the dimuon invariant mass
          // around the z peak, and the current sample may not contain
          // any z candidates but will contribute some small piece of
          // integrated luminosity to the total. to normalize mc to
          // the correct total luminosity, this contribution must be included.
          if( pj ) { 
            if( !summed ) {
              summed.reset( new typename remove_const<graphicsT>::type( *pj ) );
              if( g_debug_mode ) { cout << " dg new merge from " << pgg->description_name() << endl; }
            } else {
              summed->merge( pj.get() );
              if( g_debug_mode ) { cout << " dg merged " << pgg->description_name() << endl; }
            }
            ++n_with_graphic;
          } else {
            cout << " warning no group for " << pgg->description_name() << endl; 
          }
          //
          if( pgg->description() && pgg->description()->is_data() ) {
            sum_lumi.push_back( pgg->description()->luminosity() );
            if( g_debug_mode ) {
              cout << " merging sample " << pgg->description()->name() << " with lumi " << pgg->description()->luminosity() << " pb^(-1)" << endl;
            }
          } else {
            if( pgg->description() ) { 
              cout << " skipped lumi for sample " << pgg->description()->name() << " with lumi " << pgg->description()->luminosity() << " pb^(-1)" << endl;
            } else {
              cout << " skipped lumi sample with " << pgg->description_name() << endl;
            }
          }
        } // end for each sample to merge
        if( ! sum_lumi.empty() ) {
          if( sum_lumi.size() != n_with_graphic ) {
            cout << " cannot merge objects unless either all or none have a luminosity." << endl;
            if( g_debug_mode ) {
              cout << " number of objects with this graphic: " << n_with_graphic << endl;
              cout << " number of graphics with luminosity assigned: " << sum_lumi.size() << endl;
            }
            continue;
          }
          const float total_lumi = std::accumulate( sum_lumi.begin() , sum_lumi.end() , 0. );
          if( g_debug_mode ) {
            cout << " merged " << sum_lumi.size() << " is_data graphics with total lumi " << total_lumi
                 << " for group " << (*i)->description_name();
          }
          // set total merged sample luminosity
          (*i)->description()->set_luminosity( total_lumi );
        }
        // fixme: how to handle groups of overlays? all of this code maybe needs to be rewritten recursively.
        _overlay.push_back( pair< shared_ptr<PlotGroup> , graphicsTptr >( (*i) , summed ) );
      } else {
        assert( (*i)->type() == PlotGroup::SAMPLE );
        graphicsTptr ptr =  get_sample_graphic( *i );
        // FIXME: check that STACK is not applied to a graph. graphs should always be overlays.
        if( (group && group->type() == PlotGroup::STACK) || (!group && (*i)->type()==PlotGroup::STACK) ) {
          _stack.push_back( pair< shared_ptr<PlotGroup> , graphicsTptr >( (*i) , ptr ) ); // assign parent group
        } else if( (group && group->type() == PlotGroup::OVERLAY) || (!group && (*i)->type()==PlotGroup::OVERLAY) || (!group && (*i)->type()==PlotGroup::SAMPLE) ) {
          _overlay.push_back( pair< shared_ptr<PlotGroup> , graphicsTptr > ( (*i)->type()==PlotGroup::SAMPLE ? (*i) : group , ptr ) ); // assign parent group
        } else {
          if( g_debug_mode ) { 
            cout << " I don't know what to do with this thing with name " << (*i)->description_name() << " at line " << __LINE__ << endl;
          }
        }
      }
    }
  }
protected:
  void format_th1( TH1* h , const shared_ptr<PlotGroup>& group ) const {
    h->SetMarkerColor( group->marker_style() ? group->marker_style()->GetMarkerColor() : gStyle->GetMarkerColor() );
    h->SetMarkerStyle( group->marker_style() ? group->marker_style()->GetMarkerStyle() : gStyle->GetMarkerStyle() );
    h->SetMarkerSize( group->marker_style() ? group->marker_style()->GetMarkerSize() : gStyle->GetMarkerSize() );
    h->SetLineColor( group->line_style() ? group->line_style()->GetLineColor() : gStyle->GetLineColor() );
    h->SetLineStyle( group->line_style() ? group->line_style()->GetLineStyle() : gStyle->GetLineStyle() );
    h->SetLineWidth( group->line_style() ? group->line_style()->GetLineWidth() : gStyle->GetLineWidth() );
    h->SetFillColor( group->fill_style() ? group->fill_style()->GetFillColor() : gStyle->GetFillColor() );
    h->SetFillStyle( group->fill_style() ? group->fill_style()->GetFillStyle() : gStyle->GetFillStyle() );
    h->SetDrawOption( group->draw_options().c_str() );
    // remove line and marker from constituents of a stack
    THStack* hstack = dynamic_cast<THStack*>(h);
    if( hstack ) {
      TList* list = hstack->GetHists();
      TIterator* iter = list ? list->MakeIterator() : 0;
      while( iter ) { 
        TObject* obj = iter->Next();
        if( !obj ) { break; }
        TH1* hco = dynamic_cast<TH1*>( obj );
        if( !hco ) { continue; }
        hco->SetLineStyle( 0 );
        hco->SetMarkerStyle( 0 );
      }
      //hstack->SetMarkerStyle( 0 );
    }
    // give histogram a unique name
    static unsigned int index = 0u;
    string unique_name( (format("%s_%ld_%d") % h->GetName() % (long int)h % (index++)).str() );
    h->SetName( unique_name.c_str() );
    if( g_debug_mode ) {
      cout << " applied style from group " << group->description_name() << " " << group->description()->name() << " to " << unique_name << endl;
      cout << " fill color: " << h->GetFillColor() << " style: " << h->GetFillStyle() << endl;
      cout << " histogram has integral: " << h->Integral() << " mean: " << h->GetMean() << " rms: " << h->GetRMS() << endl;
    }
  }
  void format_tgraph( TGraph* h , const shared_ptr<PlotGroup>& group ) const {
    h->SetMarkerColor( group->marker_style() ? group->marker_style()->GetMarkerColor() : gStyle->GetMarkerColor() );
    h->SetMarkerStyle( group->marker_style() ? group->marker_style()->GetMarkerStyle() : gStyle->GetMarkerStyle() );
    h->SetMarkerSize( group->marker_style() ? group->marker_style()->GetMarkerSize() : gStyle->GetMarkerSize() );
    h->SetLineColor( group->line_style() ? group->line_style()->GetLineColor() : gStyle->GetLineColor() );
    h->SetLineStyle( group->line_style() ? group->line_style()->GetLineStyle() : gStyle->GetLineStyle() );
    h->SetLineWidth( group->line_style() ? group->line_style()->GetLineWidth() : gStyle->GetLineWidth() );
    h->SetDrawOption( group->draw_options().c_str() );
    // give a unique name
    static unsigned int index = 0u;
    string unique_name( (format("%s_%ld_%d") % h->GetName() % (long int)h % (index++)).str() );
    h->SetName( unique_name.c_str() );
    if( g_debug_mode ) {
      cout << " applied style from group " << group->description_name() << " " << group->description()->name() << " to " << unique_name << endl;
      cout << " histogram has integral: " << h->Integral() << " mean: " << h->GetMean() << " rms: " << h->GetRMS() << endl;
    }
  }
  optional<double> determine_luminosity() const {
    optional<double> result = g_luminosity;
    if( result ) { return result; } // global luminosity overrides automatic one.
    // get list of data samples
    list< shared_ptr<PlotGroup> > data_groups;
    BOOST_FOREACH( plotVecValue& v , _stack ) { if( v.first->description() && v.first->description()->is_data() ) { data_groups.push_back(v.first); } }
    BOOST_FOREACH( plotVecValue& v , _overlay ) { if( v.first->description() && v.first->description()->is_data() ) { data_groups.push_back(v.first); } }
    // if there are multiple data samples, they must have the same luminosity (or all should be in a single merge group).
    // -- collect luminosities and parents.
    result.reset();
    BOOST_FOREACH( shared_ptr<PlotGroup>& v , data_groups ) { 
      if( ! v->description() ) { continue; }
      if( ! v->description()->is_data() ) { continue; }
      if( ! v->enabled() ) { continue; }
      if( result && ( (*result) != (v->description()->luminosity()) ) ) {
        cout << " attempt to plot multiple data graphics corresponding to different luminosities." << endl;
        cout << " all must have the same luminosity, or they should all be in a single merge group." << endl;
        cout << " group: " << v->description_name() << endl;
        return optional<double>();
      }
      result.reset( v->description()->luminosity() );
    }
    if( !result ) {
      double lumi = askFloat( "luminosity to assume, in /pb? " );
      if( lumi < 0 ) {
        if( g_luminosity ) { lumi = *g_luminosity; } else { lumi = 0.; }
      }
      result.reset( lumi );
    }
    return result;
  }
  void apply_scale( TH1* h , const shared_ptr<PlotGroup>& group , const optional<double>& lumi_pb ) const {
    assert( group->type() == PlotGroup::SAMPLE || group->type() == PlotGroup::MERGE );
    const shared_ptr<DgXMLSampleDescription>& desc( group->description() );
    float weight = group->reweight();
    weight *= desc->weight();
    if( desc->is_mc() ) { //&& desc->normalization_mode() != DgXMLSampleDescription::NO_NORMALIZATION ) { 
      if( g_debug_mode ) { cout << " checking scale mode for " << group->description_name() << endl; }
      if( lumi_pb ) { // && desc->normalization_mode() == DgXMLSampleDescription::ABSOLUTE_NORMALIZATION ) {
        // weight to luminosity. the number of MC events must be the
        // number of events after all reweighting factors have been
        // applied. in the absence of any, the description's
        // num_events_total is the correct number. 
        const double nevents_rw = desc->weighted_nevents( "mc_pu_reweighted_nevents" );
        const double nevents_mc = nevents_rw > 0. ? nevents_rw : static_cast<double>(desc->num_events_total());
        if( nevents_mc < 1. ) { 
          cout << " unknown number of mc events. can't weight to luminosity." << endl;
          return;
        }
        const double cross_section_pb = desc->cross_section();
        const double lumiweight = cross_section_pb * (*lumi_pb) / nevents_mc;
        const double additional_weight = desc->has_weight() ? desc->weight() : 1.;
        h->Scale( lumiweight*additional_weight );
        if( g_debug_mode ) {
          cout << " scaling " << group->description_name() << " with cross section " << cross_section_pb << "/pb" << endl
               << " and ngen=" << nevents_mc << " to " << (*lumi_pb) << ": multiply by " << lumiweight  << endl
               << " with additional weight " << additional_weight << " for a total " << (additional_weight*lumiweight )
               << endl;
          cout << " total num. events (unweighted): " << desc->num_events_total() << endl;
          cout << " MC + pileup reweighted num. events: " << nevents_rw << endl;
        }
      }
    } else {
      if( g_debug_mode ) { cout << " applied no scale to " << group->description_name() << endl; }
    }
  }
public:
  typedef enum { MODE_HISTOGRAM , MODE_GRAPH } Mode;
  virtual const TObject* draw() const{
    ShowCommand::new_canvas(); // do this here, since it cleans up the garbage we'll add.
    // treat everything as a 1D histogram for now.
    // build the stack of histograms and list of overlays
    build_groups();
    // look for data (luminosities) amongst the sample descriptions
    optional<double> lumi = determine_luminosity();
    if( g_debug_mode ) { cout << " automatic luminosity is " << (lumi ? (*lumi) : -1 ) << endl; }
    // construct the TH1s, format them, and stack them. prepare legend. If plot stack contains a graph,
    // plot it last, as an overlay
    string same_opt = "";
    THStack* full_stack = new THStack("tmphist","");
    TLegend* legend = new TLegend( 0.8 , 0.8 , 1 , 1 );
    unsigned int n=0;
    // axis range variables
    optional<double> xmin, xmax, ymin, ymax;
    // process stack
    set< shared_ptr<PlotGroup> > groups_added_to_legend;
    for( typename vector< pair< shared_ptr<PlotGroup> , graphicsTptr > >::reverse_iterator i=_stack.rbegin(), f=_stack.rend(); i!=f; ++i,++n ) {
      shared_ptr<PlotGroup>& group( (*i).first );
      if( ! group->enabled() ) { continue; }
      // while( ! group->show_components() && group->parent() ) { group = group->parent(); }
      assert( group->description_name() !="" );
      string hname = (format("n%d_%s") % n % group->description_name()).str();
      graphicsTptr p( get_sample_graphic(group) );
      if( !p ) { 
        if( g_debug_mode ) { cout << " sample graphic for " << group->description_name() << " / " << hname << " not available" << endl; }
        continue;
      }
      TH1* h = dynamic_cast<TH1*>( p->new_ROOT( hname.c_str() ) ); 
      assert( h );
      if( !h ) { continue; } // other types unsupported.
      shared_ptr<PlotGroup> legend_group( group );
      while( !legend_group->show_components() && legend_group->parent() && legend_group->parent()->legend_text()!="" ) { legend_group = legend_group->parent(); }
      format_th1( h , legend_group );
      apply_scale( h , group , lumi );
      // grab sample description for normalization.
      h->SetMarkerStyle( 0 );
      full_stack->Add( h , legend_group->draw_options().c_str() );
      // add a legend entry once for each group
      if( groups_added_to_legend.find( legend_group ) == groups_added_to_legend.end() ) {
        string legopt;
        if( legend_group->line_style() ) { legopt += "l"; }
        if( legend_group->fill_style() ) { legopt += "f"; }
        if( legend_group->marker_style() ) { legopt += "p"; }
        legend->AddEntry( h , legend_group->legend_text().c_str() ,legopt.c_str() );
        groups_added_to_legend.insert( legend_group );
      }
    } // end for each graphic in stack
    g_garbage_stack_tobject.push_back( full_stack );
    g_garbage_stack_tobject.push_back( legend );
    // draw, if necessary
    if( full_stack && full_stack->GetHists() && full_stack->GetHists()->GetSize()!=0 ) {
      assert( g_canvas );
      full_stack->Draw();
      same_opt = " same";
      if( !ymin ) { ymin.reset( full_stack->GetMinimum() ); }
      if( !ymax ) { ymax.reset( full_stack->GetMaximum() ); }
    } else {
      TLegend* legend = new TLegend( 0.8 , 0.8 , 1 , 1 );
      g_garbage_stack_tobject.push_back( legend );
    }
    // do overlays
    for( typename vector< pair< shared_ptr<PlotGroup> , graphicsTptr > >::reverse_iterator i=_overlay.rbegin(), f=_overlay.rend(); i!=f; ++i,++n ) {
      shared_ptr<PlotGroup>& group( (*i).first );
      if( ! group->enabled() ) { continue; }
      if( ! (*i).second ) { continue; }
      assert( group->description_name() !="" );
      string hname = (format("n%d_%s") % n % group->description_name()).str();
      TObject* obj = (*i).second->new_ROOT( hname.c_str() );
      TH1* h = dynamic_cast<TH1*>( obj );
      TGraph* g = dynamic_cast<TGraph*>( obj );
      if( !h && !g ) { continue; } // unsupported.
      shared_ptr<PlotGroup> legend_group( group );
      while( !legend_group->show_components() && legend_group->parent() && legend_group->parent()->legend_text()!="" ) { legend_group = legend_group->parent(); }
      if( h ) { 
        format_th1( h , legend_group );
        apply_scale( h , group , lumi );
      } else if( g ) { 
        format_tgraph( g , legend_group );
        // FIXME: apply scale to graph
      }
      // grab sample description for normalization.
      if( h ) { 
        h->Draw( (legend_group->draw_options() + same_opt).c_str() );
        same_opt = " same";
        string legopt;
        if( legend_group->line_style() ) { legopt += "l"; }
        if( legend_group->fill_style() ) { legopt += "f"; }
        if( legend_group->marker_style() ) { legopt += "p"; }
        // 
        legend->AddEntry( h , legend_group->legend_text().c_str() ,legopt.c_str() );
        g_garbage_stack_tobject.push_back( h );
        if( !ymin || (*ymin)>h->GetMinimum() ) { ymin.reset( h->GetMinimum() ); }
        if( !ymax || (*ymax)<h->GetMaximum() ) { ymax.reset( h->GetMaximum() ); }
      } else if( g ) { 
        if( boost::icontains( same_opt , "same" ) ) { 
          g->Draw( (legend_group->graph_draw_options()).c_str() );
        } else { 
          g->Draw( (legend_group->graph_draw_options() + " a").c_str() );
        }
        same_opt = " same";
        string legopt;
        if( boost::icontains(legend_group->graph_draw_options(),"l") ) { legopt += "l"; }
        if( boost::icontains(legend_group->graph_draw_options(),"p") ) { legopt += "p"; }
        // 
        legend->AddEntry( g , legend_group->legend_text().c_str() ,legopt.c_str() );
        g_garbage_stack_tobject.push_back( g );
        if( !ymin || (*ymin)>g->GetMinimum() ) { ymin.reset( g->GetMinimum() ); }
        if( !ymax || (*ymax)<g->GetMaximum() ) { ymax.reset( g->GetMaximum() ); }
      }
    } // end for each graphic in stack
    if( legend ) { 
      legend->SetFillColor( kWhite );
      legend->SetLineStyle( 0 );
      legend->SetLineWidth( 0 );
      legend->SetTextSize(0.04);
      legend->SetTextFont(6);
      legend->Draw("same"); 
    }
    TH1* frame = 0;
    TIterator* iter = g_canvas->GetListOfPrimitives()->MakeIterator();
    while( !frame ) {
      TObject* obj = iter->Next();
      if( !obj ) { break; }
      TH1* hist = static_cast<TH1*>(obj);
      if( !hist ) { continue; }
      frame = hist;
    }
    g_canvas->Draw();
    // if( frame ) { 
    //   if( ! dynamic_cast<TH2*>(frame) ) {
    //     TAxis* xaxis = frame->GetXaxis();
    //     if( ymin ) { frame->SetMinimum( *ymin ); }
    //     if( ymax ) { frame->SetMaximum( *ymax ); }
    //     if( xmin ) { xaxis->SetRangeUser( *xmin , xaxis->GetXmax() ); }
    //     if( xmax ) { xaxis->SetRangeUser( xaxis->GetXmin() , *xmax ); }
    //   } else {
    //     TAxis* xaxis = frame->GetXaxis();
    //     TAxis* yaxis = frame->GetYaxis();
    //     if( xmin ) { xaxis->SetRangeUser( *xmin , xaxis->GetXmax() ); }
    //     if( xmax ) { xaxis->SetRangeUser( xaxis->GetXmin() , *xmax ); }
    //     if( ymin ) { yaxis->SetRangeUser( *ymin , yaxis->GetXmax() ); }
    //     if( ymax ) { yaxis->SetRangeUser( yaxis->GetXmin() , *ymax ); }
    //   }
    // }
    g_canvas->Draw();
    ShowCommand::update_canvas();
    // describe what you've just plotted
    cout << format(" plotted %|s| \n %|20t| %|5.2f|/fb luminosity") % _path_to_graphic % ((lumi ? *lumi : 0.)/1000.) << endl;
    if( true ) { 
      TLatex lt;
      lt.SetNDC(); // specify position in (0,0)-(1,1) box rather than
                   // in terms of plot-specific coordinate range
      lt.SetTextSize(0.02);
      lt.SetTextFont(10);
      lt.SetTextColor(kRed+1);
      lt.DrawLatex(0.05,0.05,_path_to_graphic.c_str());
    }
  }
  PlotGroupStack( const graphicsT& gr , const string& path )
    : graphicsT(gr)
    , _path_to_graphic( path )
    , AbsPlot("gplot","","") {}
};

class 
RangeCommand : public AbsCommand
{
protected:
  virtual void operator()( string& command ) { range(command); }
public:
  CommandOption opt_xrange;
  CommandOption opt_yrange;
  CommandOption opt_zrange;
public:
  void range( string& command ) {
    if( opt_xrange.found() ) {
      shared_ptr<PlotRange> p( make_shared<PlotRange>( PlotRange::X , opt_xrange.value_range_begin() , opt_xrange.value_range_end() ) );
      g_plot_stack.push_back(p);
    }
    if( opt_yrange.found() ) {
      shared_ptr<PlotRange> p( make_shared<PlotRange>( PlotRange::Y , opt_yrange.value_range_begin() , opt_yrange.value_range_end() ) );
      g_plot_stack.push_back(p);
    }
    if( opt_zrange.found() ) {
      shared_ptr<PlotRange> p( make_shared<PlotRange>( PlotRange::Z , opt_zrange.value_range_begin() , opt_zrange.value_range_end() ) );
      g_plot_stack.push_back(p);
    }
  }
  RangeCommand() 
  : opt_xrange( this , CommandOption::RANGE , "x" , "set x axis range" )
  , opt_yrange( this , CommandOption::RANGE , "y" , "set y ayis range" )
  , opt_zrange( this , CommandOption::RANGE , "z" , "set z azis range" )
  , AbsCommand( "range" , "adjust plotting range" ) 
  {}
};

class 
GroupCommand : public AbsCommand
{
public:
  CommandOption opt_define;
  CommandOption opt_print;
  CommandOption opt_save;
  CommandOption opt_load;
public:
  static void print_intro() {
    cout << " sample grouping mode. " << endl;
    cout << " type group(<group name>,<stack/overlay/merge>,<legend text>,<name1/number1>,<name2/number2>,...)" << endl;
    cout << "            to create a group" << endl;
    cout << "      group(<group name>,<stack/overlay/merge>,<legend text>,<tagname1>,<tagname2>,...) " << endl;
    cout << "            to create a group from samples with all of the given tags" << endl;
    cout << " type ungroup(<group name/number>) to remove a group" << endl;
    cout << " type reorder(<group name/number>) to reorder elements in a group" << endl;
    cout << "            no name to reorder top level" << endl;
    cout << " type add(<group name/number>,<name1/number1>. to add an element to a group" << endl;
    cout << " type attrib(<group name/number>) to set style attributes for a group" << endl;
    cout << " type enable(<group name/number>) or disable(<group>) to turn plotting of the group on/off" << endl;
    cout << " type done to return to dgplot command line" << endl;
  }
  static const vector<string> get_sample_names() {
    vector<string> result;
    // transform( g_descriptions.begin() , g_descriptions.end() , back_inserter(result) , bind(&DgXMLSampleDescription::name,bind(&DescValue::description,_1)) );
    transform( g_descriptions.begin() , g_descriptions.end() , back_inserter(result) , bind( &DescColl::value_type::first , _1 ) );
    return result;
  }
  static const vector<string> get_group_names( const shared_ptr<PlotGroup> head = shared_ptr<PlotGroup>() ) {
    vector<string> result;
    if( head ) { 
      if( ! head->parent() || head->type() != PlotGroup::SAMPLE ) {
        result.push_back( head->description_name() );
      }
      if( ! head->group().empty() ) {
        BOOST_FOREACH( const shared_ptr<PlotGroup>& ps , head->group() ) {
          vector<string> tmp( get_group_names( ps ) );
          copy( tmp.begin() , tmp.end() , back_inserter(result) );
        }
      }
    } else {
      BOOST_FOREACH( const shared_ptr<PlotGroup>& ps , g_plot_group_tree ) {
        vector<string> tmp( get_group_names( ps ) );
        copy( tmp.begin() , tmp.end() , back_inserter(result) );
      }
    }
    return result;
  }
  static const vector<string> get_tree_names( const shared_ptr<PlotGroup> head = shared_ptr<PlotGroup>() ) {
    vector<string> result;
    if( head ) { 
      result.push_back( head->description_name() );
      if( ! head->group().empty() ) {
        BOOST_FOREACH( const shared_ptr<PlotGroup>& ps , head->group() ) {
          vector<string> tmp( get_tree_names( ps ) );
          copy( tmp.begin() , tmp.end() , back_inserter(result) );
        }
      }
    } else {
      BOOST_FOREACH( const shared_ptr<PlotGroup>& ps , g_plot_group_tree ) {
        vector<string> tmp( get_tree_names( ps ) );
        copy( tmp.begin() , tmp.end() , back_inserter(result) );
      }
    }
    return result;
  }
  static const shared_ptr<PlotGroup> find_group( const string& group_name , const shared_ptr<PlotGroup> head = shared_ptr<PlotGroup>() ) {
    shared_ptr<PlotGroup> result;
    if( head ) { 
      if( head->type() == PlotGroup::SAMPLE ) { return result; }
      if( head->description_name() == group_name ) { return head; }
      if( ! head->group().empty() ) {
        BOOST_FOREACH( const shared_ptr<PlotGroup>& ps , head->group() ) {
          result = find_group( group_name , ps );
          if( result ) { return result; }
        }
      }
    } else {
      BOOST_FOREACH( const shared_ptr<PlotGroup>& ps , g_plot_group_tree ) {
        result = find_group( group_name , ps );
        if( result ) { return result; }
      }
    }
    return result;
  }
  static const shared_ptr<PlotGroup> find_tree_node( const string& node_name , const shared_ptr<PlotGroup> head = shared_ptr<PlotGroup>() ) {
    shared_ptr<PlotGroup> result;
    if( head ) { 
      if( head->description_name() == node_name ) { return head; }
      if( ! head->group().empty() ) {
        BOOST_FOREACH( const shared_ptr<PlotGroup>& ps , head->group() ) {
          result = find_tree_node( node_name , ps );
          if( result ) { return result; }
        }
      }
    } else {
      BOOST_FOREACH( const shared_ptr<PlotGroup>& ps , g_plot_group_tree ) {
        result = find_tree_node( node_name , ps );
        if( result ) { return result; }
      }
    }
    return result;
  }
  static void remove_group( const string& group_name , shared_ptr<PlotGroup> head = shared_ptr<PlotGroup>() ) {
    if( head ) { 
      for( PlotGroup::Coll::iterator i=head->group().begin(), f=head->group().end(); i!=f; ++i ) {
        if( (*i)->description_name() == group_name ) {
          // ok to delete right away; group should only appear once in the tree
          head->group().erase( i );
          return;
        }
        // recurse
        remove_group( group_name , *i );
      }
    } else {
      for( PlotGroup::Coll::iterator i=g_plot_group_tree.begin(), f=g_plot_group_tree.end(); i!=f; ++i ) {
        if( (*i)->description_name() == group_name ) {
          // ok to delete right away; group should only appear once in the tree
          g_plot_group_tree.erase( i );
          return;
        }
        // recurse
        remove_group( group_name , *i );
      }
    }
  }
  template<class iteratorT>
  const string parse_name_or_number( string arg , iteratorT begin_names , iteratorT end_names ) {
    trim( arg );
    if( ! boost::all( arg , is_digit() ) ) {
      // arg is a string. match to names, or return empty if no match.
      iteratorT i = find(begin_names,end_names,arg);
      if( i!=end_names ) { return arg; }
    }
    int num = std::atoi( arg.c_str() );
    if( num>=1 && num <= std::distance(begin_names,end_names) ) {
      std::advance( begin_names , num-1 );
      return *begin_names;
    }
    return string("");
  }
  static void reset_tree() {
    g_plot_group_tree.clear();
    g_available_dominant_color = stack<int>();
    for( int i=3; i!=-1; --i ) {
      g_available_dominant_color.push( 16 );
      g_available_dominant_color.push( 20 );
    g_available_dominant_color.push( 24 );
    }
  }
  void define_tree() {
    // get command line
    bool introduction = true;
    while( true ) { 
      vector<string> sample_names = get_sample_names();
      vector<string> group_names = get_group_names();
      vector<string> all_names( sample_names );
      copy( group_names.begin() , group_names.end() , back_inserter(all_names) );
      vector<string> tree_names = get_tree_names(); //  names actually used in the tree
      {
        cout << " current grouping tree: " << endl;;
        print_tree();
      }
      const unsigned int n_names_per_line = 4;
      {
        cout << " defined samples: " << endl;;
        vector<string> tmp( sample_names );
        reverse( tmp.begin() , tmp.end() );
        unsigned int ntmp=1;
        while( ! tmp.empty() ) {
          cout << format( "%3d) %20s " ) % ntmp % (tmp.back());
          if( ntmp%n_names_per_line == 0 ) { cout << endl; }
          tmp.pop_back();
          ++ntmp;
        }
        if( sample_names.empty() || sample_names.size()%n_names_per_line != 0 ) { cout << endl; }
      }
      {
        cout << " defined groups: " << endl;;
        vector<string> tmp( group_names );
        reverse( tmp.begin() , tmp.end() );
        unsigned int ntmp=1+sample_names.size();
        while( ! tmp.empty() ) {
          cout << format( "%3d) %20s " ) % ntmp % (tmp.back());
          if( ntmp%n_names_per_line == 0 ) { cout << endl; }
          tmp.pop_back();
          ++ntmp;
        }
        //if( group_names.empty() || group_names.size()%n_names_per_line != 0 ) { cout << endl; }
        cout << endl;
      }
      if( introduction ) {
        print_intro();
        introduction = false;
      }
      boost::scoped_ptr<char> input_line( readline("group mode] ") );
      string cmd( input_line.get() );
      if( input_line && *input_line ) { add_history( input_line.get() ); }
      trim( cmd ); // remove leading and trailing whitespace
      replace_all( cmd , " " , "" ); // remove whitespace
      to_lower( cmd ); // convert to lower-case for regex matching
      // process input
      // ----------------------------------------------------------------
      // define command matching strings
      boost::smatch what;
      static boost::regex regex_ungroup( "ungroup\\(([A-Za-z0-9_]+)\\)" );
      static boost::regex regex_begin_group( "group\\(.+\\)" );
      static boost::regex regex_reorder( "reorder\\(([A-Za-z0-9_]*)\\)" );
      static boost::regex regex_add( "add\\(([A-Za-z0-9_]+),([A-Za-z0-9_]+)\\)" );
      static boost::regex regex_attrib( "attrib[A-Za-z]*\\(([A-Za-z0-9_]+)\\)" );
      static boost::regex regex_enable( "enable\\(([A-Za-z0-9_]+)\\)" );
      static boost::regex regex_disable( "disable\\(([A-Za-z0-9_]+)\\)" );
      static boost::regex regex_components( "enable\\(([A-Za-z0-9_]+),([A-Za-z0-9_]+)\\)" );
      // process command
      // ==== quit ===============
      if( istarts_with( cmd , "done" ) ) { break; }
      if( istarts_with( cmd , "help" ) ) { print_intro(); continue; }
      if( istarts_with( cmd , "print" ) || istarts_with( cmd , "list" ) ) { print_tree(); continue; }
      // ==== ungroup ===============
      if( boost::regex_match( cmd , what , regex_ungroup ) && what[1].matched ) {
        const string arg( what[1].first , what[1].second );
        string gname = parse_name_or_number( arg , all_names.begin() , all_names.end() );
        if( gname=="" ) { cout << "could not parse group name/number " << arg << endl; continue; }
        if( find(group_names.begin(),group_names.end(),gname) == group_names.end() ) {
          cout << gname << " not a known group" << endl;
          continue;
        }
        // remove the group
        remove_group( gname );
        // ==== reorder ================
      } else if( boost::regex_match( cmd , what , regex_reorder ) ) {
        string group_name = "";
        if( what[1].matched && what[1].first!=what[1].second ) { 
          const string arg( what[1].first , what[1].second );
          if( arg=="all" || arg=="" ) { group_name = ""; }
          else {
            group_name = parse_name_or_number( arg , all_names.begin() , all_names.end() );
            if( group_name=="" ) { 
              cout << "could not parse group name/number " << arg << endl; 
              continue; 
            }
            if( find(group_names.begin(),group_names.end(),group_name) == group_names.end() ) {
              cout << group_name << " not a known group" << endl;
              continue;
            }
          }
        }
        // group_name=="all" indicates top level group
        PlotGroup::Coll* coll = 0;
        if( group_name!="" ) { 
          shared_ptr<PlotGroup> p = find_group(group_name);
          if( p ) { coll = &(p->group()); }
          if( !coll ) { cout << " could not find group " << group_name << endl; continue; }
        }
        if( !coll ) { coll = &g_plot_group_tree; }
        // reorder the group
        list<string> node_names;
        transform( coll->begin() , coll->end() , back_inserter(node_names) , bind(&PlotGroup::description_name,_1) );
        cout << " old order: " << endl;
        unsigned int i=1;
        BOOST_FOREACH( const string& n , node_names ) { cout << (i++) << ") " << n << endl; }
        list<string> old_order( node_names );
        list<string> new_order;
        bool abort_reset = false;
        while( ! node_names.empty() && !abort_reset ) {
          cout << " current new order: " << endl;
          unsigned int j=1;
          BOOST_FOREACH( const string& n , new_order ) { cout << (j++) << ") " << n << endl; }
          cout << " remaining unassigned: " << endl;
          unsigned int k=1;
          BOOST_FOREACH( const string& n , node_names ) { cout << (k++) << ") " << n << endl; }
          cout << " next in new order (quit/- to quit): ";
          string nextline;
          getline( cin , nextline );
          trim( nextline );
          if( istarts_with( nextline , "quit" ) || istarts_with( nextline , "-" ) ) { abort_reset = true; break; }
          string choice = parse_name_or_number( nextline , node_names.begin() , node_names.end() );
          if( choice == "" ) { continue; }
          new_order.push_back( choice );
          node_names.erase( find( node_names.begin() , node_names.end() , choice ) );
        }
        if( abort_reset ) { continue; }
        // reorder coll according to new list
        PlotGroup::Coll new_coll;
        for( list<string>::iterator i=new_order.begin(), f=new_order.end(); i!=f; ++i ) {
          const string& n( *i );
          PlotGroup::Coll::iterator k = find_if( coll->begin() , coll->end() , bind(&PlotGroup::description_name,_1)==n );
          assert( k!=coll->end() );
          new_coll.push_back( *k );
        }
        coll->swap( new_coll );
        // done with reordering
        // ==== attributes ================
      } else if( boost::regex_match( cmd , what , regex_attrib ) && what[1].matched ) {
        const string arg( what[1].first , what[1].second );
        string gname = parse_name_or_number( arg , all_names.begin() , all_names.end() );
        if( gname=="" ) { cout << "could not parse group name/number " << arg << endl; continue; }
        if( find(tree_names.begin(),tree_names.end(),gname) == tree_names.end() ) {
          cout << gname << " not a known tree group" << endl;
          continue;
        }
        shared_ptr<PlotGroup> group = find_tree_node( gname );
        assert( group );
        // check for dominant color argument, if style isn't already provided.
        smatch whatd;
        int dominant_color = 0;
        if( group->line_style() ) { dominant_color = group->line_style()->GetLineColor(); }
        else if( group->fill_style() ) { dominant_color = group->fill_style()->GetFillColor(); }
        else if( group->marker_style() ) { dominant_color = group->marker_style()->GetMarkerColor(); }
        else if( boost::regex_match( cmd , whatd , regex("color\\(([0-9]+)\\)") ) && whatd[1].matched ) {
          int icol = std::atoi( string(whatd[1].first,whatd[1].second).c_str() );
          if( icol>=16 && icol<50 ) { 
          } else {
            dominant_color = g_available_dominant_color.top(); g_available_dominant_color.pop();
          }
        }
        // check for defaults
        const bool defaults_as_mc = istarts_with( cmd , "attribmc" );
        const bool defaults_as_data = istarts_with( cmd , "attribdata" );
        const bool defaults_as_clear = istarts_with( cmd , "attribclear" );
        const bool showcomp = istarts_with( cmd , "attribcshowcomp" );
        const bool hidecomp = istarts_with( cmd , "attribchidecomp" );
        if( showcomp ) { group->set_show_components( true ); } 
        else if( hidecomp ) { group->set_show_components( false ); }
        if( defaults_as_mc ) { group->set_mc_style(dominant_color); }
        else if( defaults_as_data ) { group->set_data_style(dominant_color); }
        else if( defaults_as_clear ) { group->set_default_style(); }
        // tell user to edit attributes of the following histogram
        ShowCommand::new_canvas();
        scoped_ptr<TH1F> h( new TH1F("h" , "style histogram" , 100 , 0 , 1 ) );
        if( group->legend_text() != "" ) { h->SetTitle( group->legend_text().c_str() ); }
        h->FillRandom("gaus",1000);
        h->SetMarkerColor( group->marker_style() ? group->marker_style()->GetMarkerColor() : gStyle->GetMarkerColor() );
        h->SetMarkerStyle( group->marker_style() ? group->marker_style()->GetMarkerStyle() : gStyle->GetMarkerStyle() );
        h->SetMarkerSize( group->marker_style() ? group->marker_style()->GetMarkerSize() : gStyle->GetMarkerSize() );
        h->SetLineColor( group->line_style() ? group->line_style()->GetLineColor() : gStyle->GetLineColor() );
        h->SetLineStyle( group->line_style() ? group->line_style()->GetLineStyle() : gStyle->GetLineStyle() );
        h->SetLineWidth( group->line_style() ? group->line_style()->GetLineWidth() : gStyle->GetLineWidth() );
        h->SetFillColor( group->fill_style() ? group->fill_style()->GetFillColor() : gStyle->GetFillColor() );
        h->SetFillStyle( group->fill_style() ? group->fill_style()->GetFillStyle() : gStyle->GetFillStyle() );
        h->Draw( group->draw_options().c_str() );
        cout << " edit the properties of the \"h\" histogram (drawn) and then type \".q\" to quit root and continue." << endl;
        app->RunRoot( true );
        if( h->GetTitle() != group->legend_text() ) { group->set_legend_text( h->GetTitle() ); }
        group->marker_style().reset( new TAttMarker );
        group->line_style().reset( new TAttLine );
        group->fill_style().reset( new TAttFill );
        group->marker_style()->SetMarkerColor( h->GetMarkerColor() );
        group->marker_style()->SetMarkerStyle( h->GetMarkerStyle() );
        group->marker_style()->SetMarkerSize( h->GetMarkerSize() );
        group->line_style()->SetLineColor( h->GetLineColor() );
        group->line_style()->SetLineStyle( h->GetLineStyle() );
        group->line_style()->SetLineWidth( h->GetLineWidth() );
        group->fill_style()->SetFillColor( h->GetFillColor() );
        group->fill_style()->SetFillStyle( h->GetFillStyle() );
        group->draw_options() = h->GetDrawOption();
      // ==== enable/disable ================
      } else if( boost::regex_match( cmd , what , regex_enable ) && what[1].matched ) {
        const string arg( what[1].first , what[1].second );
        string gname = parse_name_or_number( arg , all_names.begin() , all_names.end() );
        if( gname=="" ) { cout << "could not parse group name/number " << arg << endl; continue; }
        if( find(tree_names.begin(),tree_names.end(),gname) == tree_names.end() ) {
          cout << gname << " not a known tree group" << endl;
          continue;
        }
        shared_ptr<PlotGroup> group = find_tree_node( gname );
        assert( group );
        group->set_enabled( true );
      } else if( boost::regex_match( cmd , what , regex_disable ) && what[1].matched ) {
        const string arg( what[1].first , what[1].second );
        string gname = parse_name_or_number( arg , all_names.begin() , all_names.end() );
        if( gname=="" ) { cout << "could not parse group name/number " << arg << endl; continue; }
        if( find(tree_names.begin(),tree_names.end(),gname) == tree_names.end() ) {
          cout << gname << " not a known tree group" << endl;
          continue;
        }
        shared_ptr<PlotGroup> group = find_tree_node( gname );
        assert( group );
        group->set_enabled( false );
        // show/hide components
      } else if( boost::regex_match( cmd , what , regex_components ) && what[1].matched && what[2].matched ) {
        const string arg( what[1].first , what[1].second );
        string gname = parse_name_or_number( arg , all_names.begin() , all_names.end() );
        if( gname=="" ) { cout << "could not parse group name/number " << arg << endl; continue; }
        if( find(tree_names.begin(),tree_names.end(),gname) == tree_names.end() ) {
          cout << gname << " not a known tree group" << endl;
          continue;
        }
        shared_ptr<PlotGroup> group = find_tree_node( gname );
        assert( group );
        string arg2( what[2].first , what[2].second );
        trim( arg2 );
        unsigned int iarg2 = std::atoi( arg2.c_str() );
        bool show = icontains(arg2,"true") && ! icontains(arg2,"false");
        if( arg2[0] == '0' || arg2[0] == '1' ) { show = (bool)iarg2; }
        group->set_show_components( show );
      // add to group
      } else if( boost::regex_match( cmd , what , regex_add ) ) {
        const string arg( what[1].first , what[1].second );
        string gname = parse_name_or_number( arg , all_names.begin() , all_names.end() );
        if( gname=="" ) { cout << "could not parse group name/number " << arg << endl; continue; }
        if( find(tree_names.begin(),tree_names.end(),gname) == tree_names.end() ) {
          cout << gname << " not a known tree group" << endl;
          continue;
        }
        shared_ptr<PlotGroup> group = find_tree_node( gname );
        assert( group );
        string arg2( what[2].first , what[2].second );
        trim( arg2 );
        // is this a sample or group name?
        string nm( parse_name_or_number( arg2 , all_names.begin() , all_names.end() ) );
        shared_ptr<PlotGroup> p;
        // retrieve group and make sample
        if( find(group_names.begin(),group_names.end(),nm)!=group_names.end() ) {
          PlotGroup::Coll::iterator igrp = find_if( g_plot_group_tree.begin(),g_plot_group_tree.end(),bind(&PlotGroup::description_name,_1)==nm );
          assert( igrp != g_plot_group_tree.end() );
          p = *igrp;
          // remove group from the top level of the tree.
          g_plot_group_tree.erase( igrp );
        } else {
          // retrieve the description
          // DescColl::iterator idesc = find_if( g_descriptions.begin(),g_descriptions.end(),
          //                                     bind(&DgXMLSampleDescription::name,bind(&DescValue::description,_1))==nm);
          DescColl::iterator idesc = g_descriptions.find( nm );
          assert( idesc != g_descriptions.end() ); //cout << " could not find description for sample with name " << nm << endl;
          // make new plotsample for this description
          p.reset( new PlotGroup( shared_ptr<PlotGroup>() , (*idesc).second.description() ) );
        }
        // add to group
        group->add( p );
        p->parent() = group;
      // ==== group ================
      } else if( boost::regex_match( cmd , what , regex_begin_group ) ) {
        // group command must occur as last check with "group(.+)" match
        // the following supports up to 20 arguments. trivial expansion to more
        const unsigned int max_arguments = 18u;
        trim( cmd );
        replace_all( cmd , " " , "" );
        // check matching expression
        bool match = false;
        int nargs = 0;
        for( int check_arg=max_arguments; check_arg!=3; --check_arg ) {
          string full_group_st( "group\\(" );
          string sub_group_st( "([A-Za-z0-9_]+)," );
          string sub_group_end_st( "([A-Za-z0-9_]+)" );
          for( unsigned int i=0; i!=check_arg; ++i ) {
            if( i!=(check_arg-1) ) {
              full_group_st += sub_group_st; 
            } else {
              full_group_st += sub_group_end_st; 
            }
          }
          full_group_st += "\\)";
          boost::regex regex_full_group( full_group_st );
          bool ok = boost::regex_match( cmd , what , regex_full_group );
          if( ok ) { nargs = check_arg; match = true; break; }
        }
        if( ! match ) { continue; }
        // make new group.
        //   get group name
        if( ! what[1].matched || what[1].first==what[1].second ) { cout << " provide a group name!" << endl; continue; }
        std::string groupname( what[1].first , what[1].second );
        trim( groupname );
        if( find( tree_names.begin() , tree_names.end() , groupname ) != tree_names.end() ) { 
          cout << " group name " << groupname << " already in use" << endl;
          continue;
        }
        // get grouping mode
        if( ! what[2].matched || what[2].first==what[2].second ) { cout << " provide a grouping mode type \"stack\" or \"overlya\"" << endl; continue; }
        std::string mode( what[2].first , what[2].second );
        trim( mode );
        PlotGroup::Type type;
        if( istarts_with(mode,"stack") ) { type = PlotGroup::STACK; }
        else if( istarts_with(mode,"over") ) { type = PlotGroup::OVERLAY; }
        else if( istarts_with(mode,"merge") ) { type = PlotGroup::MERGE; }
        else {
          cout << " provide a valid grouping mode (\"stack\", \"overlay\", or \"merge\") " << endl;
          continue;
        }
        //   get legend text
        if( ! what[3].matched || what[3].first==what[3].second ) { cout << " provide a group name!" << endl; continue; }
        std::string legend_text( what[3].first , what[3].second );
        trim( legend_text );
        // get up to max_arguments-3 more arguments. if these are not
        // sample or group names, they will be interpreted as tags to be found in the sample description.
        list< shared_ptr<PlotGroup> > samples_for_arguments;
        bool tag_mode = false;
        set<string> tags;
        vector< DescValue > tagged_desc;
        for( unsigned int iarg=3; iarg!=max_arguments; ++iarg ) {
          const unsigned int match_arg=iarg+1;
          if( ! what[match_arg].matched ) { break; }
          // get argument text
          const string arg( what[match_arg].first , what[match_arg].second );
          // is this a sample or group name?
          string nm( parse_name_or_number( arg , all_names.begin() , all_names.end() ) );
          if( tag_mode || nm=="" ) { // not a known sample or group name. look for tags.
            tag_mode = true;
            BOOST_FOREACH( const DescColl::value_type& vt , g_descriptions ) {
              const DescValue& v( vt.second );
              if( v.desc->has_tag(arg) ) { 
                tags.insert( arg ); 
                tagged_desc.push_back( v );
              }
            }
          } else {
            // a known sample. make sure it isn't already in the list
            // of plot tree samples, and then add it
            assert( nm!="" );
            if( find(tree_names.begin(),tree_names.end(),nm)!=tree_names.end() ) {
              cout << " sample " << nm << " is already used in the plot tree." << endl;
              continue;
            }
            // is this a group name? if so, it must be at the top level of the tree, since it is unused.
            if( find(group_names.begin(),group_names.end(),nm)!=group_names.end() ) {
              PlotGroup::Coll::iterator igrp = find_if( g_plot_group_tree.begin(),g_plot_group_tree.end(),bind(&PlotGroup::description_name,_1)==nm );
              assert( igrp != g_plot_group_tree.end() );
              samples_for_arguments.push_back( *igrp );
              // remove group from the top level of the tree.
              g_plot_group_tree.erase( igrp );
            } else {
              // retrieve the description
              // DescColl::iterator idesc = find_if( g_descriptions.begin(),g_descriptions.end(),
              //                                     bind(&DgXMLSampleDescription::name,bind(&DescValue::description,_1))==nm);
              DescColl::iterator idesc = g_descriptions.find( nm );
              assert( idesc != g_descriptions.end() ); //cout << " could not find description for sample with name " << nm << endl;
              // make new plotsample for this description
              shared_ptr<PlotGroup> p( new PlotGroup( shared_ptr<PlotGroup>() , (*idesc).second.description() ) );
              samples_for_arguments.push_back( p );
            }
          } // end explicit mode
        } // end for each group/sample/tag argument
        // tagged mode or explicit mode? if tag mode, tagged_desc
        // currently has the union of descriptions matching any
        // tag. need to take the intersection.
        if( tag_mode ) {
          vector<DescValue> intdesc;
          BOOST_FOREACH( DescValue& v , tagged_desc ) {
            bool has_tags = true;
            BOOST_FOREACH( const string& tag , tags ) {
              if( ! v.description()->has_tag( tag ) ) { has_tags = false; break; }
            }
            if( ! has_tags ) { continue; }
            intdesc.push_back( v );
          }
          // make plotsamples for each fully-tagged description.
          BOOST_FOREACH( DescValue& v , intdesc ) {
            shared_ptr<PlotGroup> p( new PlotGroup( shared_ptr<PlotGroup>() , v.description() ) );
            samples_for_arguments.push_back( p );
          }
        }
        if( samples_for_arguments.empty() || samples_for_arguments.size()<1 ) {
          cout << " provide at least one thing to group!" << endl;
          continue;
        }
        // create the new group, adding it to the top level of the tree.
        if( samples_for_arguments.size()==1 ) { 
          // single graphic. copy sample description. overlay mode = sample
          g_plot_group_tree.push_back( samples_for_arguments.front() );
        } else {
          shared_ptr<DgXMLSampleDescription> d( new DgXMLSampleDescription );
          d->set_name( groupname );
          d->set_legend( legend_text );
          shared_ptr<PlotGroup> pq( new PlotGroup( shared_ptr<PlotGroup>() , type , d , samples_for_arguments ) );
          BOOST_FOREACH( shared_ptr<PlotGroup>& p , samples_for_arguments ) {
            p->parent() = pq;
          }
          g_plot_group_tree.push_back( pq );
        }
        // done adding group to top level
      } // end if match to a command
    } // while stay in group mode
  } // done with group mode
  void print_tree( const shared_ptr<PlotGroup> head = shared_ptr<PlotGroup>() , string prefix = "" ) {
    if( head ) { 
      if( head->type() != PlotGroup::SAMPLE ) {
        cout << prefix << head->description_name() << " (group,";
        if( head->type() == PlotGroup::OVERLAY ) { cout << "overlay"; }
        else if( head->type() == PlotGroup::STACK ) { cout << "stack"; }
        else if( head->type() == PlotGroup::MERGE ) { cout << "merge"; }
        else { cout << "unknown"; }
        cout << ")" << endl;
        if( ! head->enabled() ) { cout << " *disabled*" << endl; }
        prefix += "   ";
      } else {
        cout << prefix << head->description_name() << " (sample)" << endl;
        if( ! head->enabled() ) { cout << " *disabled*" << endl; }
      }
      if( ! head->group().empty() ) {
        BOOST_FOREACH( const shared_ptr<PlotGroup>& ps , head->group() ) {
          print_tree( ps , prefix );
        }
      }
    } else {
      BOOST_FOREACH( const shared_ptr<PlotGroup>& ps , g_plot_group_tree ) {
        print_tree( ps , prefix );
      }
    }
    return;
  }
  const bool save_tree( const std::string& filename ) {
    if( filename == "" ) { return false; }
    boost::shared_ptr<std::ostream> outf( boost::make_shared<std::ofstream>( filename.c_str() ) );
    if( !outf || !(*outf) ) { 
      std::cout << " WARNING: could not save sample tree to filename " << filename << std::endl;
      return false; 
    }
    try { 
      boost::shared_ptr<boost::archive::xml_oarchive> aro( new boost::archive::xml_oarchive(*outf) );
      (*aro) << boost::serialization::make_nvp("sample_tree",g_plot_group_tree);
      outf->flush();
      aro.reset(); // destroy the oarchive before the stream closes.
      outf.reset(); // outf passed by reference to xml_oarchive; it must be destroyed after archive
    } catch( ... ) {
      std::cout << " WARNING: exception; could not save sample tree to filename " << filename << std::endl;
      return false;
    }
    cout << " saved groupings to " << filename << endl;
    return true;
  }
  const bool load_tree( const std::string& filename ) {
    if( filename == "" ) { return false; }
    boost::shared_ptr<std::istream> inf( boost::make_shared<std::ifstream>( filename.c_str() ) );
    if( !inf || !(*inf) ) { 
      std::cout << " WARNING: could not load sample tree from filename " << filename << std::endl;
      return false; 
    }
    try {
      boost::shared_ptr<boost::archive::xml_iarchive> ari( new boost::archive::xml_iarchive(*inf) );
      (*ari) >> boost::serialization::make_nvp("sample_tree",g_plot_group_tree);
      ari.reset(); // destroy the iarchive before the stream closes.
      inf.reset(); // inf passed by reference to xml_iarchive; it must be destroyed after archive.
    } catch( ... ) {
      std::cout << " WARNING: exception; could not load sample tree from filename " << filename << std::endl;
      return false;
    }
    cout << " loaded groupings from " << filename << endl;
    return true;
  }
protected:
  virtual void operator()( string& command ) { 
    if( opt_print.value_bool() ) { print_tree(); }
    if( opt_define.value_bool() ) { define_tree(); }
    if( opt_save.found() ) { save_tree(opt_save.value_string()); }
    if( opt_load.found() ) { load_tree(opt_load.value_string()); }
  }
public:
  GroupCommand() 
    : opt_define( this , CommandOption::BOOL , "def" , "define sample tree" )
    , opt_print( this , CommandOption::BOOL , "print" , "print sample tree" )
    , opt_save( this , CommandOption::STRING , "save" , "save sample tree to given filename" )
    , opt_load( this , CommandOption::STRING , "load" , "load sample tree from given filename" )
    , AbsCommand( "group" , "manipulate group tree" ) 
  {}
};

// add a graphics object to the current canvas, old style
class
OldPlotCommand : public AbsCommand
{
public:
  CommandOption opt_overlay;
  CommandOption opt_noshow;
  CommandOption opt_alldirs;
  CommandOption opt_allfiles;
  CommandOption opt_askb;
  CommandOption opt_reorder;
  CommandOption opt_depth;
  CommandOption opt_projx;
  CommandOption opt_projy;
  CommandOption opt_projrange;
  CommandOption opt_unscaled;
  CommandOption opt_recolor;
  CommandOption opt_rebin;
  CommandOption opt_yrange;
  CommandOption opt_profmode;
  CommandOption opt_exactmatch;
  CommandOption opt_desc;
  CommandOption opt_logx;
  CommandOption opt_logy;
  CommandOption opt_logz;
  CommandOption opt_unit;
public:
  void plot( shared_ptr<const DgAbsGraphic> gr , const string& description , const string& subsample , const string& sourcefile ) {
    bool projectionx = opt_projx.value_bool();
    bool projectiony = opt_projy.value_bool();
    bool norescale = opt_unscaled.value_bool();
    bool recolor = opt_recolor.found();
    int the_recolor = recolor ? opt_recolor.value_int() : 0;
    // plot the object
    shared_ptr<const DgHistogram1D> bwh1d = dynamic_pointer_cast<const DgHistogram1D>(gr);
    shared_ptr<const DgEfficiency0D> bwe0d = dynamic_pointer_cast<const DgEfficiency0D>(gr);
    shared_ptr<const DgEfficiency1D> bwe1d = dynamic_pointer_cast<const DgEfficiency1D>(gr);
    shared_ptr<const DgEfficiency2D> bwe2d = dynamic_pointer_cast<const DgEfficiency2D>(gr);
    shared_ptr<const DgHistogramWeighted1D> bwhw1d = dynamic_pointer_cast<const DgHistogramWeighted1D>(gr);
    shared_ptr<const DgHistogram2D> bwh2d = dynamic_pointer_cast<const DgHistogram2D>(gr);
    shared_ptr<const DgGraph2D> bg2d = dynamic_pointer_cast<const DgGraph2D>(gr);
    shared_ptr<const DgProfile1D> bwp1d = dynamic_pointer_cast<const DgProfile1D>(gr);
    if( bwe2d ) { 
      shared_ptr<PlotEfficiency2D> p( make_shared<PlotEfficiency2D>(*bwe2d,description,subsample,sourcefile) );
      g_plot_stack.push_back(p);
    } else if( bwe1d ) {
      shared_ptr< PlotGraph<DgEfficiency1D> > p( make_shared< PlotGraph<DgEfficiency1D > >(*bwe1d,description,subsample,sourcefile) );
      p->efficiency_range();
      unsigned int bingroupsize = opt_rebin.found() ? opt_rebin.value_int() : 1;
      if( bingroupsize > 1 ) { cout << " grouping " << bingroupsize << " 1-D efficiency bins" << endl; }
      p->set_bin_group_size( bingroupsize );
      if( opt_yrange.found() ) { p->override_y_range( opt_yrange.value_range_begin() , opt_yrange.value_range_end() ); }
      if( recolor ) { p->recolor(the_recolor); }
      g_plot_stack.push_back( p );
    } else if( bwe0d ) {
      shared_ptr< PlotNumber<DgEfficiency0D> > p( make_shared< PlotNumber<DgEfficiency0D > >(*bwe0d,description,subsample,sourcefile) );
      p->draw();
      //delete p;
      if( recolor ) { p->recolor(the_recolor); }
      g_plot_stack.push_back(p);
    } else if( bwhw1d ) {
      shared_ptr< PlotHistogram1D<DgHistogramWeighted1D> > p( make_shared< PlotHistogram1D<DgHistogramWeighted1D > >(*bwhw1d,description,subsample,sourcefile) );
      if( !norescale && g_luminosity ) {
        const double sf = SampleNormalization::instance()->rescale_factor(subsample,*g_luminosity);
        p->rescale( sf );
      }
      if( recolor ) { p->recolor(the_recolor); }
      g_plot_stack.push_back(p);
    } else if( bwh1d ) {
      shared_ptr< PlotHistogram1D<DgHistogram1D> > p( make_shared< PlotHistogram1D<DgHistogram1D > >(*bwh1d,description,subsample,sourcefile) );
      if( !norescale && g_luminosity ) {
        const double sf = SampleNormalization::instance()->rescale_factor(subsample,*g_luminosity);
        p->rescale( sf );
      }
      if( recolor ) { p->recolor(the_recolor); }
      g_plot_stack.push_back(p);
    } else if( bwh2d ) {
      PlotHistogram2D::Mode mode;
      int minimum_bin = 0;
      int maximum_bin = std::numeric_limits<int>::max();
      bool use_range = false;
      if( !projectionx && !projectiony ) { 
        mode = PlotHistogram2D::STANDARD;
      } else {
        if( projectionx ) { mode = PlotHistogram2D::XPROJECTION; }
        else if( projectiony ) { mode = PlotHistogram2D::YPROJECTION; }
        if( opt_projrange.found() ) {
          minimum_bin = (int)floor(opt_projrange.value_range_begin());
          maximum_bin = (int)ceil(opt_projrange.value_range_end());
        }
      }
      shared_ptr<PlotHistogram2D> p( make_shared<PlotHistogram2D>(*bwh2d,description,subsample,sourcefile,mode) );
      if( use_range ) { p->set_projection_limits(minimum_bin,maximum_bin); }
      g_plot_stack.push_back(p);
//     } else if( bphw ) {
//       PlotGraph<DgPolarWeightedHistogram>* p = new PlotGraph<DgPolarWeightedHistogram>(*bphw,description,subsample,sourcefile);
//       g_plot_stack.push_back(p);
//     } else if( bph ) {
//       PlotGraph<DgPolarHistogram>* p = new PlotGraph<DgPolarHistogram>(*bph,description,subsample,sourcefile);
//       g_plot_stack.push_back(p);
    } else if( bg2d ) { 
        shared_ptr< PlotGraph<DgGraph2D> > p( make_shared< PlotGraph<DgGraph2D > >(*bg2d,description,subsample,sourcefile) );
      if( recolor ) { p->recolor(the_recolor); }
      g_plot_stack.push_back(p);
    } else if( bwp1d ) { 
      string mode;
      if( istarts_with(opt_profmode.value_string(),"meanrms") ) { mode = "prof"; }
      else if( istarts_with(opt_profmode.value_string(),"mean") ) { mode = "mean"; }
      else if( istarts_with(opt_profmode.value_string(),"rms") ) { mode = "rms"; }
      else { mode = "meanrms"; }
      cout << " mode=" << mode << endl;
      shared_ptr< PlotProfile1D<DgProfile1D> > p( make_shared< PlotProfile1D<DgProfile1D > >(*bwp1d,description,subsample,sourcefile,mode) );
      if( recolor ) { p->recolor(the_recolor); }
      g_plot_stack.push_back(p);
    } else {
      assert( !"unrecognized graphics type" );
    } 
  }
public:
  static const bool find_single_plot( const string& command , string& pathto , shared_ptr<const DgAbsGraphic>& plot , const bool exact_match = false ) {
    string plotname = first_token( command );
    replace_all( plotname , " " , "" );
    replace_all( plotname , "!" , "" ); // terminal ! indicates exact match
    if( plotname == "" ) { 
      plotname == ".+";
    }
    // figure out which object to plot
    dg::View view;
    int n = dg::search_down( view , plotname );
    if( n<1 ) { cout << " plot not found" << endl; return false; }
    shared_ptr<const DgAbsGraphic> gr;
    if( n>1 && !exact_match ) { 
      // more than one name matches.  ask user which one to plot.
      // note that none of these matches may be the one the user wants.
      // for example, a search for "mass" may return only "mass_150".
      vector<string> choices_unsorted;
      for( dg::View::const_iterator i=view.begin(), f=view.end(); i!=f; ++i ) {
        assert( i->valid_graphic() ); // do not call View::graphic,
                                      // since that triggers retreival
                                      // of the graphic from disk
        string p( dg::path_to(i->name(),i->collection()) );
        if( p == "" ) { continue; }
        // remove current path from the name string
        replace_all( p , dg::current()->path_string() , "" );
        trim_left_if( p , is_any_of("/") );
        //
        choices_unsorted.push_back( p );
      }
      vector<string> choices_sorted( choices_unsorted );
      std::sort( choices_sorted.begin() , choices_sorted.end() );
      vector<string> choices( choices_sorted ) ;
      if( choices.empty() ) { cout << " plot not found" << endl; return false; }
      int choice = chooseFromList( choices );
      if( choice<=0 ) { return false; }
      vector<string>::iterator iunsorted = find( choices_unsorted.begin() , choices_unsorted.end() , choices_sorted[choice-1] );
      assert( iunsorted != choices_unsorted.end() );
      unsigned int ind = std::distance( choices_unsorted.begin() , iunsorted );
      gr = view[ind].graphic();
      pathto = dg::path_to( view[ind].name() , view[ind].collection() );
    } else {
      if( n==1 ) {
        gr = view[0].graphic();
        pathto = dg::path_to(view[0].name(),view[0].collection());
      } else {
        // get first exact match
        for( dg::View::const_iterator i=view.begin(), f=view.end(); i!=f; ++i ) {
          if( string(i->name()) == plotname ) {
            gr = i->graphic();
            pathto = dg::path_to(i->name(),i->collection());
            break;
          }
        }
        if( !gr ) { return false; }
      }
    }
    plot = gr;
    return true;
  }
protected:
  void operator()( string& command ) { 
    if( !opt_overlay.value_bool() ) { clearCanvas(); }
    // look for plot to plot.
    string description;
    string subsample;
    string sourcefile;
    // retrieve description from command line, if provided
    // syntax is "plot gobbledigook [description]"
    // closing bracket is optional
    if( true ) {
      if( opt_desc.found() ) {
        description = opt_desc.value_string();
        trim_right_if( description , is_any_of("]") );
      }
    }
    shared_ptr<const DgAbsGraphic> gr;
    const bool do_all_files = opt_allfiles.value_bool();
    const bool do_all_dirs = opt_alldirs.value_bool();
    if( !do_all_dirs && !do_all_files ) { 
      string pathto = dg::path_to( "" , dg::current() );
      list<string> splits;
      split( splits , pathto , is_any_of("/") , boost::token_compress_on );
      if( !splits.empty() ) { 
        sourcefile = splits.front(); splits.pop_front();
        if( !splits.empty() ) {
          subsample = join( splits , "/" );
        }
      }
    }
    if( do_all_dirs || do_all_files ) {
      string pathto;
      bool ok = find_single_plot( command , pathto , gr , opt_exactmatch.value_bool() );
      if( !ok ) { return; }
      trim( pathto );
      DgCollection* start_dir = dg::current();
      if( !start_dir ) { start_dir = dg::root(); }
      if( g_debug_mode ) { cout << "path to: " << pathto << endl; }
      // pathto contains the graphic name. get it.
      string gname = pathto;
      string path = ".";
      vector<string> pathtokens;
      split( pathtokens , pathto , is_any_of("/") , token_compress_on );
      if( !pathtokens.empty() ) {
        gname = pathtokens.back();
        pathtokens.pop_back();
        path = join( pathtokens ,  "/" );
      }
      if( g_debug_mode ) { 
        cout << "path: " << path << endl; 
        cout << "gname: " << gname << endl; 
      }
      // int lslash = find_last(pathto,"/");
      // if( lslash >=0 && lslash<pathto.size() ) {
      //   string tpath = pathto;
      //   tpath.Resize( lslash );
      //   path = tpath.Data();
      //   gname = pathto.substr( lslash+1 , pathto.size() - lslash -1 );
      // }
      // move to path
      if( g_debug_mode ) {
        cout << format( " cd to path %|50s| " ) % path << endl
             << format( "       name %|50s| " ) % gname << endl;
      }
      if( path != "." ) {
        dg::root()->cd( path );
      }
      //
      dg::View view;
      // ensure gname is the right name. current directory should contain it.
      view = dg::lookup_graphic( gname ); 
      if( view.empty() ) {
        cout << " couldn't find " << gname << " to plot. fix me." << endl;
        start_dir->cd();
        return;
      }
      cout << gname << endl;
      //      g_overlay_mode = COMPARISON_OVERLAY;
      // get depth.
      int depth = 0;
      if( opt_depth.found() ) { depth = opt_depth.value_int(); }
      else if( do_all_files ) {
        depth = std::numeric_limits<int>::max();
      } else {
        while( depth==0 ) {
          depth = askInteger( "depth? " );
        }
      }
      if( depth<=0 ) { 
        start_dir->cd();
        return; 
      }
      // retrieve all matching histograms for this depth.
      DgCollection* old = dg::current();
      int idep=depth;
      stack<string> depthstack;
      while( (idep--)>0 && dg::current()!=dg::root() ) { 
        // don't track the last collection before the trunk. this is the thing that changes from branch to branch.
        if( idep>=1 ) { depthstack.push( dg::current()->name() ); }
        dg::up(); 
      }
      if( dg::current()==dg::root() ) {
        depthstack.pop();
      }
      DgCollection* trunk = dg::current();
      // for each subdirectory of the trunk, descend to the same structure (as recorded in depthstack) and add the relevant collection
      vector< pair<string,DgCollection*> > leaves;
      vector< string > leaves_descriptions;
      vector< string > leaves_sourcefiles;
      for( dg::SubColl::const_iterator i = trunk->subcollections().begin() , f = trunk->subcollections().end(); i!=f; ++i ) {
        stack<string> tmpstack( depthstack );
        //assert( tmpstack.size() == depth-1 );
        DgCollection* leaf = i->second; assert(leaf);
        string leafname = leaf->name();
        leaf->cd();
        cout << " trying to descend branch " << leafname << endl;
        while( !(tmpstack.empty()) ) {
          leaf->cd( tmpstack.top() );
          cout << tmpstack.top() << endl;
          if( dg::current()->name() != tmpstack.top() ) { break; } // cd did not work 
          //if( dg::current() == leaf ) { break; } // cd did not work.
          tmpstack.pop();
          leaf = dg::current();
        }
        // if tmpstack is empty, then leaf points to the right collection. if not, then skip this branch
        if( !(tmpstack.empty()) ) { continue; }
        if( opt_askb.value_bool() ) {
          if( !askYesNo( leafname.c_str() ) ) { continue; }
        }
        leaves.push_back( pair<string,DgCollection*>(leafname,leaf) );
        {
          string tmp(description);
          string fpath( dg::path_to("",trunk) );
          if( tmp=="" ) { tmp=fpath; }
          leaves_descriptions.push_back( tmp );
          vector<string> splits;
          split( splits , fpath , is_any_of("/ ") ,  boost::token_compress_on );
          string filename = "";
          if( do_all_files ) { 
            if( !splits.empty() ) { filename = splits.front(); }
            if( splits.size()>1 && splits.front()=="" ) { filename = splits[1]; }
          } // else leave empty
          leaves_sourcefiles.push_back( filename );
        }
      }
      // "leaves" now contains collections which should have a graphic named 'gname'. grab each.
      multimap< double , pair<string, shared_ptr<const DgAbsGraphic> > > plots; // sorted by integral or some other ranking
      map< string , string > plots_descriptions;
      map< string , string > plots_files;
      const bool ordered = opt_reorder.value_bool();
      for( vector< pair<string,DgCollection*> >::const_iterator i=leaves.begin(), f=leaves.end(); i!=f; ++i ) {
        DgCollection* coll = i->second;
        coll->cd();
        view = dg::lookup_graphic( gname );
        if( view.empty() ) { continue; }
        shared_ptr<const DgAbsGraphic> graphic = view.front().graphic();
        shared_ptr<const DgHistogram1D> d1d = dynamic_pointer_cast<const DgHistogram1D>( graphic );
        shared_ptr<const DgHistogramWeighted1D> d1dw = dynamic_pointer_cast<const DgHistogramWeighted1D>( graphic );
        shared_ptr<const DgGraph2D> d1g2d = dynamic_pointer_cast<const DgGraph2D>( graphic );
        double rank = 0.;
        if( ordered ) {
          rank = askInteger( (string("order for " )+i->first+":" ).c_str() );
        } else {
          if( d1d ) {
            rank = d1d->integrate();
          } else if( d1dw ) { 
            rank = d1dw->integrate_weight();
          } else if( d1g2d ) { 
            rank = d1g2d->number();
          }
        }
        if( rank < 0. ) { continue; } // skip negatively ranked plots
        plots.insert( pair<double,pair<string, shared_ptr<const DgAbsGraphic> > >( rank , pair<string, shared_ptr<const DgAbsGraphic> >(i->first,graphic)) );
        plots_descriptions[ i->first ] = description;
        plots_files[ i->first ] = sourcefile;
      }
      // plot each graphic
      for( multimap< double , pair<string, shared_ptr<const DgAbsGraphic> > >::reverse_iterator i=plots.rbegin(), f=plots.rend(); i!=f; ++i ) {
        plot( i->second.second , plots_descriptions[i->second.first] , i->second.first , plots_files[i->second.first] ); 
      }
      // restore old directory
      //old->cd();
      start_dir->cd();
    }//  else if( hasOption( command , "allfiles" ) ) {
    //   bool ok = find_single_plot( command , description , gr );
    //   if( !ok ) { return; }
    //   assert( gr );
    //   string tmpname( description.Data() );
    //   size_t k = tmpname.find(".root/");
    //   if( k == string::npos ) { return; }
    //   string plotname = tmpname.substr( k+6 , tmpname.size() - k - 6 );
    //   // treat root subcollections as separate files
    //   vector<string> v1;
    //   vector<DgCollection*> v2;
    //   for( dg::SubColl::const_iterator i = dg::root()->subcollections().begin() , f = dg::root()->subcollections().end(); i!=f; ++i ) {
    //     v1.push_back( i->first );
    //     v2.push_back( i->second );
    //   }
    //   //      g_overlay_mode = COMPARISON_OVERLAY;
    //   // sort so that mc always comes first
    //   if( true ) {
    //     vector<string> tmp1;
    //     vector<DgCollection*> tmp2;
    //     vector<string> tmp3;
    //     vector<DgCollection*> tmp4;
    //     for( unsigned int i=0, f=v1.size(); i!=f; ++i ) {
    //       if( v1[i].find("zewk")!=string::npos || v1[i].find("gzz")!=string::npos ) {
    //         tmp1.push_back( v1[i] );
    //         tmp2.push_back( v2[i] );
    //       } else {
    //         tmp3.push_back( v1[i] );
    //         tmp4.push_back( v2[i] );
    //       }
    //     }
    //     v1.clear();
    //     v2.clear();
    //     copy( tmp1.begin(), tmp1.end() , back_inserter(v1) );
    //     copy( tmp3.begin(), tmp3.end() , back_inserter(v1) );
    //     copy( tmp2.begin(), tmp2.end() , back_inserter(v2) );
    //     copy( tmp4.begin(), tmp4.end() , back_inserter(v2) );
    //   }
    //   // done sort so that mc always comes first
    //   string tmpst( v1[0] );
    //   if( tmpst.find("v6") == string::npos ) {
    //     vector<string> tmpv1;
    //     vector<DgCollection*> tmpv2;
    //     tmpv1.push_back( v1[1] );
    //     tmpv2.push_back( v2[1] );
    //     tmpv1.push_back( v1[0] );
    //     tmpv2.push_back( v2[0] );
    //     v1 = tmpv1;
    //     v2 = tmpv2;
    //   }
    //   for( int icol=0; icol!=v2.size(); ++icol ) {
    //     const string& collname = v1[icol];
    //     string fullpath = collname + "/" + plotname;
    //     size_t j = fullpath.find("/");
    //     dg::root()->cd();
    //     while( j != string::npos ) {
    //       string dirname = fullpath.substr(0,j);
    //       dg::cd( dirname.c_str() );
    //       fullpath = fullpath.substr(j+1,fullpath.size()-j-1);
    //       j = fullpath.find("/");
    //     }
    //     dg::View view( dg::lookup_graphic( fullpath.c_str() ) );
    //     if( view.size()<1 ) { cerr << " can't find " << plotname << " in " << collname << endl; continue; }
    //     assert( view.size()==1 );
    //     description = dg::path_to(view[0].name,view[0].collection);
    //     gr = view[0].graphic;
    //     assert( gr );
    //     cout << " adding plot " << description << endl;
    //     assert( icol < g_plot_palette.size() );
    //     PlotColor* p = new PlotColor( g_plot_palette[icol]  );
    //     p->draw();
    //     g_plot_stack.push_back( p );
    //     PlotStyle* ps = new PlotStyle( icol==0 ? SOLID : POINTS );
    //     ps->draw();
    //     g_plot_stack.push_back( ps );
    //     plot( gr , command , description );
    //   }
    //   AutoPlotRange* apr = new AutoPlotRange();
    //   g_plot_stack.push_back( apr );
    // }
    else {
      g_histogram_style = SOLID;
      string pathto;
      bool ok = find_single_plot( command , pathto , gr , opt_exactmatch.value_bool() );
      if( !ok ) { return; }
      assert( gr );
      // plot the plot
      plot( gr , description , subsample , sourcefile );
    }
    // show the new canvas, unless we're told not to.
    if( ! opt_noshow.value_bool() ) {
      string tmpst("");
      ShowCommand::show( opt_logx.value_bool() , opt_logy.value_bool() , opt_logz.value_bool() ,
                         opt_unit.value_bool() , false );
    }
  }
public:
  OldPlotCommand() 
    : opt_overlay( this , CommandOption::BOOL , "overlay" , "overlay this graphic on the earlier stack" )
    , opt_noshow( this , CommandOption::BOOL , "noshow" , "add to draw stack only; don't draw yet" )
    , opt_alldirs( this , CommandOption::BOOL , "alldirs" , "scan all directories for identically-named graphics and plot" )
    , opt_allfiles( this , CommandOption::BOOL , "alldirs" , "scan all loaded files for identically-named graphics and plot" )
    , opt_askb( this , CommandOption::BOOL , "askb" , "ask the user to confirm/veto each directory considered by alldirs" )
    , opt_reorder( this , CommandOption::BOOL , "reorder" , "reorder directories considered by alldirs" )
    , opt_depth( this , CommandOption::INTEGER , "depth" , "depth to move up for alldirs search" )
    , opt_projx( this , CommandOption::BOOL , "projx" , "project 2D graphic onto its x axis" )
    , opt_projy( this , CommandOption::BOOL , "projy" , "project 2D graphic onto its y axis" )
    , opt_projrange( this , CommandOption::RANGE , "projrange" , "subrange (in bin units) of axis to be projected away" )
    , opt_unscaled( this , CommandOption::BOOL , "unscaled" , "do not apply meta-data scaling" )
    , opt_recolor( this , CommandOption::INTEGER , "recolor" , "specify a different plot color" )
    , opt_rebin( this , CommandOption::INTEGER , "rebin" , "specify rebinning by grouping n consecutive bins" )
    , opt_yrange( this , CommandOption::RANGE , "yrange" , "adjust y plotting range (for graphs)" )
    , opt_profmode( this , CommandOption::STRING , "profmode" , "set error bars for profile plotting mode (MEANRMS,MEAN,or RMS)" )
    , opt_exactmatch( this , CommandOption::BOOL , "!" , "match plot name exactly" )
    , opt_desc( this , CommandOption::STRING , "[" , "specify a different plot description" )
    , opt_logx( this , CommandOption::BOOL , "logx" , "x log scale" )
    , opt_logy( this , CommandOption::BOOL , "logy" , "y log scale" )
    , opt_logz( this , CommandOption::BOOL , "logz" , "z log scale" )
    , opt_unit( this , CommandOption::BOOL , "unit" , "normalize data object to unit area" )
    , AbsCommand( "plot" , "add a grpahics object to the current canvas" ) 
  {}
};

// define a plot
class
PlotCommand : public AbsCommand
{
public:
  // CommandOption opt_projx;
  // CommandOption opt_projy;
  // CommandOption opt_projrange;
  CommandOption opt_unscaled;
  // CommandOption opt_rebin;
  // CommandOption opt_yrange;
  CommandOption opt_exactmatch;
protected:
  void gplot( const string& path , shared_ptr<const DgAbsGraphic> example ) {
    shared_ptr<const DgHistogram1D> bwh1d = dynamic_pointer_cast<const DgHistogram1D>(example);
    shared_ptr<const DgHistogramWeighted1D> bwhw1d = dynamic_pointer_cast<const DgHistogramWeighted1D>(example);
    shared_ptr<const DgEfficiency1D> bweff1d = dynamic_pointer_cast<const DgEfficiency1D>(example);
    if( bweff1d ) { 
      shared_ptr< PlotGroupStack<const DgEfficiency1D> > p( new PlotGroupStack<const DgEfficiency1D>(*bweff1d,path) );
      p->draw();
    } else if( bwhw1d ) { 
      shared_ptr< PlotGroupStack<const DgHistogramWeighted1D> > p( new PlotGroupStack<const DgHistogramWeighted1D>(*bwhw1d,path) );
      p->draw();
    } else if( bwh1d ) {
      shared_ptr< PlotGroupStack<const DgHistogram1D> > p( new PlotGroupStack<const DgHistogram1D>(*bwh1d,path) );
      p->draw();
    }
  }
  // ================================================================
  void operator()( string& command ) { 
    clearCanvas();

    shared_ptr<const DgAbsGraphic> gr;
    string pathto;
    bool ok = OldPlotCommand::find_single_plot( command , pathto , gr , opt_exactmatch.value_bool() );
    if( !ok ) { return; }
    assert( gr );

    // plot the plot.
    // -- extract the relative path by removing the filename and leading "dg"s
    string relpathto( pathto );
    trim( relpathto );
    if( opt_debug.value_bool() ) { cout << " stack plotting absolute " << relpathto << endl; }
    if( istarts_with(relpathto,"root/") ) { 
      relpathto = regex_replace( relpathto , regex("root.*/dg/dg/(.*)") , "\\1" );
    } else {
      relpathto = regex_replace( relpathto , regex("^/.*/dg/dg/(.*)") , "\\1" );
    }
    if( opt_debug.value_bool() ) { cout << " stack plotting relative " << relpathto << endl; }
    gplot( relpathto , gr );

    // show the new canvas, unless we're told not to.
    // if( ! opt_noshow.value_bool() ) {
    //ShowCommand::show();
    // }
  }
public:
  PlotCommand() 
    : opt_unscaled( this , CommandOption::BOOL , "unscaled" , "do not apply meta-data scaling" )
      // , opt_rebin( this , CommandOption::INTEGER , "rebin" , "specify rebinning by grouping n consecutive bins" )
      //    , opt_yrange( this , CommandOption::RANGE , "yrange" , "adjust y plotting range (for graphs)" )
    , opt_exactmatch( this , CommandOption::BOOL , "!" , "match plot name exactly; do not try partial match completion" )
    , AbsCommand( "gplot" , "plot a graphics object using the sample tree" ) 
  {}
};


class 
DownCollCommand : public AbsCommand
{
public:
  static const bool down( const string& command ) {
    if( dg::current()->subcollections().empty() ) { return false; }
    string collname = command;
    boost::trim( collname );
    // cout << command << endl;
    if( command == "" ) {
      if( g_debug_mode ) { cout << " no collection provided for down. asking for new collection." << endl; }
      // list subcollections and let the user choose
      vector<string> choices_string;
      for( dg::SubColl::const_iterator i=dg::current()->subcollections().begin(), f=dg::current()->subcollections().end(); i!=f; ++i) {
        choices_string.push_back( dg::current()->subcollection_name(i) );
        if( g_debug_mode ) { cout << " collection option:" << choices_string.back() << endl; }
      }
      std::sort( choices_string.begin() , choices_string.end() );
      vector<string> choices;
      for( vector<string>::const_iterator i=choices_string.begin(), f=choices_string.end(); i!=f; ++i ) {
        choices.push_back( (*i).c_str() );
      }
      if( choices.empty()) { cout << " no subcollections found." << endl; return false; }
      int choice = 1;
      if( choices.size()>1 ) { choice = chooseFromList(choices); }
      if( choice<1 || choice>choices.size() ) { 
        if( g_debug_mode ) { cout << " invalid choice " << choice << endl; }
        return false;
      }
      collname = choices[choice-1];
//       cout << " CHOICE: " << choices[choice-1] << endl;
//       cout << "         " << collname << endl;
    }
    if( g_debug_mode ) { cout << " cd to " << collname << endl; }
    DgCollection* newcoll = dg::current()->cd( collname.c_str() );
    if( g_debug_mode ) { 
      if( newcoll ) { 
        cout << " now at " << newcoll->name() << endl; 
      } else {
        cout << " cd failed " << endl;
      }
    }
    return true;
  }
protected:
  virtual void operator()( string& command ) {
    //     if( true ) {
    //       cout << "test" << endl;
    //     }
    const bool ok = down(command);
    if( ok && dg::current()->graphics().empty() && !dg::current()->subcollections().empty() && !(app->batching_commands()) ) {
      string tmp( "" );
      operator()( tmp );
    } else {
      PwdCommand::pwd();
      LsCollCommand::ls();
    }
  }
public:
  DownCollCommand()
    : AbsCommand( "down" , "descend to a subgroup of the current graphics collection" ) {}
};

class 
UpCollCommand : public AbsCommand
{
public:
  static void up( string& command ) {
    assert( dg::current() );
    unsigned int count=0;
    unsigned int steps = atoi( command.c_str() );
    while( dg::current() != dg::root() ) { 
      assert( dg::current() );
      dg::current()->up();
      if( (++count)>=steps ) { break; }
      if( steps<1 && !(dg::current()->graphics().empty()) ) { break; }
      if( app->batching_commands() ) { break; }
    }
  }
protected:
  virtual void operator()( string& command ) {
    up(command);
    PwdCommand::pwd();
    LsCollCommand::ls();
  }
public:
  UpCollCommand()
    : AbsCommand( "up" , "ascend to the parent of the current graphics collection" ) {}
};

class 
AcrossCollCommand : public AbsCommand
{
protected:
  virtual void operator()( string& command ) {
    if( dg::current() == dg::root() ) { return; }
    string upone("1") ;
    UpCollCommand::up( upone );
    DownCollCommand::down( command );
  }
public:
  AcrossCollCommand()
    : AbsCommand( "across" , "move to a neighbor of the current graphics collection" ) {}
};

class 
CountCollCommand : public AbsCommand
{
protected:
  virtual void operator()( string& command ) {
    cout << dg::current()->count_collections() << " collections" << endl;
  }
public:
  CountCollCommand()
    : AbsCommand( "countcoll" , "count number of collections contained here" ) {}
};

class 
CountDataCommand : public AbsCommand
{
protected:
  virtual void operator()( string& command ) {
    cout << dg::current()->count_data_objects() << " data objects" << endl;
  }
public:
  CountDataCommand()
    : AbsCommand( "countdata" , "count number of data objects contained here" ) {}
};

class
ShowCollectionTreeCommand : public AbsCommand
{
protected:
  void show_level( const DgCollection* coll , const unsigned long depth ) {
    if( !coll ) { return; }
    for( unsigned int d=0; d!=depth; ++d ) { cout << "-"; }
    cout << " + " << coll->name() << " " << coll->description() << " " << coll->graphics().size() << endl;
    for( dg::SubColl::const_iterator i=dg::current()->subcollections().begin() , f=dg::current()->subcollections().end(); i!=f; ++i ) {
      show_level( dg::subcollection_ptr( i ) , depth+1 );
    }
  }
  virtual void operator()( string& command ) {
    const unsigned long mdepth( dg::current()->max_depth() );
    cout << mdepth << " depth remaining" << endl;
    show_level( dg::current() , 0 );
  }
public:
  ShowCollectionTreeCommand()
    : AbsCommand( "showcolltree" , "show collection hierarchy for current collection and children" ) {}
};



class 
HelpCommand : public AbsCommand
{
protected:
  virtual void operator()( string& command ) {
    // either list all available commands or call ::showHelp() for a particular command.
    if( command == "" ) {
      for( CommandList::iterator i=g_commands.begin() , f=g_commands.end(); i!=f; ++i ) {
        cout << setw(20) << i->second->name() << setw(4) << " " << setw(66) << i->second->description() << endl;
      }
      return;
    } else {
      string firsttok = first_token( command );
      AbsCommand* cmd = match_command( firsttok );
      if( !cmd ) {
        cout << " no help available for unrecognized commands." << endl;
        return;
      }
      cmd->showMoreHelp();
      return;
    }
  }
public:
  HelpCommand() : AbsCommand( "help" , "display available commands or specific info on a particular command" ) {}
};


class 
CleanCommand : public AbsCommand
{
protected:
  virtual void operator()( string& command ) { clearCanvas(); }
public:
  CleanCommand() : AbsCommand( "clean" , "clear the canvas." ) {}
};

class 
PaletteCommand : public AbsCommand
{
public:
  CommandOption opt_color;
  CommandOption opt_show;
public:
  typedef enum { RED , GREEN , BLUE } Palette;
protected:
  virtual void operator()( string& cmd ) {
    if( opt_color.found() ) {
      if( istarts_with(opt_color.value_string(),"blue") ) { palette(BLUE); }
      else if( istarts_with(opt_color.value_string(),"green") ) { palette(GREEN); }
      else if( istarts_with(opt_color.value_string(),"red") ) { palette(RED); }
    }
    if( opt_show.value_bool() ) { show_palette(); }
  }
public:
  static double sigmoid( const double& s0 , const double& s1 , const double& m , const double& x12 , const double& x ) {
    double result = s0 + ((s1-s0) / (1. + exp(-m*(x-x12))));
    return result;
  }
  static void palette( const Palette& palette ) {
    // reserve the first 50 colors for framework uses (axes labels, lines, etc.)
    // define these colors here.
    //      TColor* color = gROOT->GetColor(0); 
    // etc.
    // set a custom default color palette for some of the reserved colors
#define PAL_SET_TCOLOR(n,r,g,b) gROOT->GetColor(n)->SetRGB(r/255.,g/255.,b/255.)
    // PAL_SET_TCOLOR(16,27,36,38);
    // PAL_SET_TCOLOR(17,74,103,146);
    // PAL_SET_TCOLOR(18,163,85,99);
    // PAL_SET_TCOLOR(19,103,173,169);
    // PAL_SET_TCOLOR(20,225,130,86);
    // PAL_SET_TCOLOR(21,115,163,83);
    // PAL_SET_TCOLOR(22,227,188,61);
    // PAL_SET_TCOLOR(25,255,239,235);

    PAL_SET_TCOLOR(16,232,87,76); // orangeish
    PAL_SET_TCOLOR(17,242,123,41);
    PAL_SET_TCOLOR(18,229,165,27);
    PAL_SET_TCOLOR(19,217,204,60);

    PAL_SET_TCOLOR(20,57,153,119); // greenish
    PAL_SET_TCOLOR(21,159,210,17);
    PAL_SET_TCOLOR(22,61,96,102);
    PAL_SET_TCOLOR(23,52,63,79);

    PAL_SET_TCOLOR(24,201,46,100);
    PAL_SET_TCOLOR(25,220,37,77);
    PAL_SET_TCOLOR(26,111,107,156);
    PAL_SET_TCOLOR(27,326,50,66);

    // PAL_SET_TCOLOR(25,220,37,77);
    // PAL_SET_TCOLOR(26,111,107,156);
    // PAL_SET_TCOLOR(27,326,50,66);


    // PAL_SET_TCOLOR(16,27,36,38);
    // PAL_SET_TCOLOR(17,103,178,143);
    // PAL_SET_TCOLOR(18,0,144,180);
    // PAL_SET_TCOLOR(19,65,42,88);
    // PAL_SET_TCOLOR(20,225,0,85);
    // PAL_SET_TCOLOR(21,222,140,174);
    // PAL_SET_TCOLOR(22,255,34,0);
    // PAL_SET_TCOLOR(23,165,116,63);
    // PAL_SET_TCOLOR(24,253,163,72);
    // PAL_SET_TCOLOR(25,167,205,229);
    // PAL_SET_TCOLOR(26,255,239,235);

    // PAL_SET_TCOLOR(16,0,0,0);
    // PAL_SET_TCOLOR(17,255,0,0);
    // PAL_SET_TCOLOR(18,0,0,255);
    // PAL_SET_TCOLOR(19,0,255,0);
    // PAL_SET_TCOLOR(20,0,255,255);
    // PAL_SET_TCOLOR(21,255,0,255);
    // PAL_SET_TCOLOR(22,255,255,0);

    // PAL_SET_TCOLOR(16,27,36,38);
    // PAL_SET_TCOLOR(17,49,67,102);
    // PAL_SET_TCOLOR(18,71,89,34);
    // PAL_SET_TCOLOR(19,140,55,39);
    // PAL_SET_TCOLOR(20,178,58,9);
    // PAL_SET_TCOLOR(25,255,239,235);
    // PAL_SET_TCOLOR(30,26,40,64);
    // PAL_SET_TCOLOR(31,71,89,34);
    // PAL_SET_TCOLOR(32,229,141,0);
    // define the hist 2d color spectrum palette with 256 colors.
    Int_t ncol( 256 );    // number of colors should never decrease from palette to palette.
    g_color_palette.clear();
    g_color_palette.reserve( ncol );
    for( int j=0; j<ncol; ++j ) {
      TColor* color = gROOT->GetColor(j+50);
      if( !color ) { color = new TColor(j+50,0,0,0,""); }
      Float_t r,g,b;
      Float_t x = j/(float)ncol;
      x = 1. - x;
      // palette-specific function here
      switch( palette ) {
      case BLUE:
        r = sigmoid(0,0.178,5.55,0.816,x);
        g = sigmoid(0,1.,-5.8,0.29,x);
        b = sigmoid(0.63,1.,-7.8,0.566,x);
        break;
      case GREEN:
        r = sigmoid(0,0.178,5.55,0.816,x);
        b = sigmoid(0,1.,-5.8,0.29,x);
        g = sigmoid(0.63,1.,-7.8,0.566,x);
        break;
      case RED:
        b = sigmoid(0,0.178,5.55,0.816,x);
        g = sigmoid(0,1.,-5.8,0.29,x);
        r = sigmoid(0.63,1.,-7.8,0.566,x);
        break;
      }
      // palette-specific function here
      color->SetRGB(r,g,b);
      g_color_palette[j] = color->GetNumber();
    }
    // done
    gStyle->SetPalette(ncol,&(*(g_color_palette.begin())));
  }
  static void show_palette() {
    ShowCommand::new_canvas();
    TBox box;
    unsigned int ncol = gStyle->GetNumberOfColors();
    unsigned int nside = static_cast<unsigned int>(std::ceil( std::sqrt( static_cast<double>(ncol) ) ));
    for( unsigned int i=0,f=ncol; i!=f; ++i ) { 
      Float_t x1 = ( (i%nside) / (1.*nside) );
      Float_t y1 = ( std::floor(i/nside) / (1.*nside) );
      Float_t x2 = x1 + 1./nside;
      Float_t y2 = y1 + 1./nside;
      box.SetFillColor(i);
      box.SetFillStyle(kSolid);
      box.SetLineColor(i);
      box.DrawBox(x1,y1,x2,y2);
    }
    ShowCommand::update_canvas();
  }
  PaletteCommand() 
    : opt_color( this , CommandOption::STRING , "to" , "set color palette: blue, green, red, rainbow, sea" )
    , opt_show( this , CommandOption::BOOL , "show" , "show the current palette" )
    , AbsCommand( "palette" , "change/show current color palette" ) {}
};

class 
BatchCommand : public AbsCommand
{
protected:
  virtual void operator()( string& command ) {
    string fname( first_token(command) );
    app->LoadCommandsFromFile(fname.c_str());
  }
public:
  BatchCommand() : AbsCommand( "batch" , "run commands from a batch file" ) {}
};

class
ResolutionVsXCommand : public AbsCommand
{
public:
  CommandOption opt_d0ptmodel;
  CommandOption opt_autobin;
  CommandOption opt_constbin;
  CommandOption opt_askres;
  CommandOption opt_askx;
  CommandOption opt_notweighted;
  CommandOption opt_secondgaus;
protected:
  bool _use_limits_x;
  double _min_x;
  double _max_x;
  bool _use_limits_y;
  double _min_y;
  double _max_y;
  mutable string _x_label;
  mutable string _y_label;
public:
  // takes a graph of resolution vs x and fits it to a parameterization.
  const unsigned int fill_dataset( RooDataSet* data , const DgGraph2D& ntuple , 
                                   RooRealVar& x , RooRealVar& y ) const {
    unsigned int n( 0u );
    for( DgGraph2D::const_iterator i=ntuple.begin(), f=ntuple.end(); i!=f; ++i ) {
      if( _use_limits_x ) {
        if( (*i)._x < _min_x ) { continue; }
        if( (*i)._x > _max_x ) { continue; }
      }
      if( _use_limits_y ) {
        if( (*i)._y < _min_y ) { continue; }
        if( (*i)._y > _max_y ) { continue; }
      }
      x = (*i)._x;
      y = (*i)._y;
      data->add( RooArgSet(x,y) );
      ++n;
    }
    return n;
  }

  RooDataSet* get_dataset( RooRealVar& x , RooRealVar& y ) const {
    RooDataSet* data = new RooDataSet( "data" , "data" , RooArgSet(x,y) );
    DgCollection::View view;
    shared_ptr<const DgGraph2D> gr;
    for( vector< shared_ptr<AbsPlot> >::iterator i=g_plot_stack.begin() , f=g_plot_stack.end(); i!=f; ++i ) {
      if( (*i)->isData() ) {
        shared_ptr<const DgGraph2D> g = dynamic_pointer_cast<const DgGraph2D>( *i );
        if( !g ) { continue; }
        gr = g;
        break;
      }
    }
    if( !gr ) { cout << " can't find graph to fit " << endl; return 0; }
    fill_dataset( data ,*gr , x , y );
    _x_label = gr->get_x_axis_label();
    _y_label = gr->get_y_axis_label();
    return data;
  }  

  virtual void operator()( string& command ) {
    if( opt_d0ptmodel.value_bool() ) {
      // dataset variables
      const double xmin = 1.; 
      const double xmax = 20;
      const double ymin = -1;
      const double ymax = 1;
      _use_limits_x = true;
      _min_x = xmin;
      _max_x = xmax;
      _use_limits_y = true;
      _min_y = ymin;
      _max_y = ymax;
      RooRealVar* x = new RooRealVar( "x" , "x", xmin , xmax, "" );
      RooRealVar* y = new RooRealVar( "y" , "y", ymin , ymax, "" );
      // get dataset
      RooDataSet* all_data = get_dataset( *x , *y );
      if( !all_data ) { cout << " couldn't find data to fit" << endl; return; }
      // construct d0 resolution vs pt pdf
      // estimate initial width with single gaussian
      RooRealVar* gsmean  = new RooRealVar( "gsmean" , "mean of single gaussian" , 0. );
      RooRealVar* gssigma  = new RooRealVar( "gssigma" , "width of single gaussian" , 1 , 0. , xmax );
      RooGaussian* gspdf = new RooGaussian( "gspdf" , "estimation pdf" , *y , *gsmean , *gssigma );
      RooFitResult* sfit = gspdf->fitTo( *all_data , RooFit::Save(true) );
      cout << " initial width estimate: " << gssigma->getVal() << endl;
      // gaussian with pt-dependent width
      RooRealVar* p0  = new RooRealVar( "p0" , "p0 res pdf component" , 0.020 , 0.005 , 0.2 );
      RooRealVar* p1  = new RooRealVar( "p1" , "p1 res pdf component" , 5. , 0.01 , 20 );
      // RooRealVar* p0  = new RooRealVar( "p0" , "p0 res pdf component" , gssigma->getVal() , 0.030 , 1 );
      // RooRealVar* p1  = new RooRealVar( "p1" , "p1 res pdf component" , 0.  , 0. , 1 );
      RooRealVar* p2  = new RooRealVar( "p2" , "p2 res pdf component" , -0.5  , -10 , 0. );
      RooRealVar* p3  = new RooRealVar( "p3" , "p3 res pdf component" , -1E-3 , -10E-3 , 0. );
      // RooGenericPdf* res_vs_pt_pdf = new RooGenericPdf( "res_vs_pt_pdf" , 
      //                                                   "p0 + (p1*exp(p2*x)) + (p3*x)" ,
      //                                                   RooArgSet(*x,*p0,*p1,*p2,*p3) );
      RooGenericPdf* res_vs_pt_pdf = new RooGenericPdf( "res_vs_pt_pdf" , 
                                                        "p0 * ( 1 + (p1/x) )" ,
                                                        RooArgSet(*x,*p0,*p1) );
      TF1* fitfunc = new TF1("fitfunc","[0] * (1 + ([1]/x))",xmin,xmax);
      // gaussian pdf
      RooRealVar* gmean  = new RooRealVar( "gmean" , "gaussian mean" , 0. );
      RooGaussian* res_pdf = new RooGaussian( "res_pdf" , "res_pdf" , *y , *gmean , *res_vs_pt_pdf );
      // normalization
      RooRealVar* a0  = new RooRealVar( "a0" , "a0" , 2*all_data->numEntries() , 0 , 2*all_data->numEntries() );
      // total pdf
      RooExtendPdf* pdf = new RooExtendPdf( "pdf" , "total resolution PDF" , *res_pdf , *a0 );
      //RooAbsPdf* pdf = res_pdf;
      // do fit
      RooFitResult* fitResult = pdf->fitTo( *all_data , RooFit::Save(true), 
                                            RooFit::Minos(false), RooFit::Hesse(true) ,
                                            RooFit::Extended(true) );
      // RooFitResult* fitResult = pdf->fitTo( *all_data , RooFit::Save(true), 
      //                                       RooFit::Minos(false), RooFit::Hesse(true), 
      //                                       RooFit::Extended(true) );
      // print results
      cout << " Fit Results: " << endl;
      fitResult->Print();
      // show results

      // draw result
      const bool noshow = false;
      ShowCommand::new_canvas();
      if( !noshow && g_canvas ) {
        g_canvas->Divide( 3 , 2 );
        vector<double> xval;
        vector<double> yval;
        {
          g_canvas->cd(1);
          const char* xcut = "x < 2.";
          RooPlot* y_frame = y->frame( 40 );
          all_data->plotOn( y_frame , RooFit::Cut( xcut ) , 
                            RooFit::LineColor(kBlack) , RooFit::MarkerColor(kBlack) );
          RooDataSet* nds = dynamic_cast<RooDataSet*>( all_data->reduce( RooFit::Cut( xcut ) ) );
          pdf->plotOn( y_frame , RooFit::Slice(RooArgSet(xcut)), 
                       RooFit::ProjWData(RooArgSet(xcut) , *all_data ) ,
                       RooFit::Normalization(nds->numEntries(),RooAbsReal::NumEvent) , 
                       RooFit::LineColor(kRed) );
          RooFitResult* sfit = gspdf->fitTo( *nds , RooFit::Save(true) );
          xval.push_back( 1 );
          yval.push_back( gssigma->getVal() );
          gspdf->plotOn( y_frame , RooFit::LineColor(kBlue) );
          y_frame->Draw();
        }
        {
          g_canvas->cd(2);
          const char* xcut = "x >= 4.5 && x < 5.5";
          RooPlot* y_frame = y->frame( 40 );
          all_data->plotOn( y_frame , RooFit::Cut( xcut ) , 
                            RooFit::LineColor(kBlack) , RooFit::MarkerColor(kBlack) );
          RooDataSet* nds = dynamic_cast<RooDataSet*>( all_data->reduce( RooFit::Cut( xcut ) ) );
          pdf->plotOn( y_frame , RooFit::Slice(RooArgSet(xcut)), 
                       RooFit::ProjWData(RooArgSet(xcut) , *all_data ) ,
                       RooFit::Normalization(nds->numEntries(),RooAbsReal::NumEvent) , 
                       RooFit::LineColor(kRed) );
          RooFitResult* sfit = gspdf->fitTo( *nds , RooFit::Save(true) );
          xval.push_back( 5 );
          yval.push_back( gssigma->getVal() );
          gspdf->plotOn( y_frame , RooFit::LineColor(kBlue) );
          y_frame->Draw();
        }
        {
          g_canvas->cd(3);
          const char* xcut = "x >= 9.5 && x < 10.5";
          RooPlot* y_frame = y->frame( 40 );
          all_data->plotOn( y_frame , RooFit::Cut( xcut ) , 
                            RooFit::LineColor(kBlack) , RooFit::MarkerColor(kBlack) );
          RooDataSet* nds = dynamic_cast<RooDataSet*>( all_data->reduce( RooFit::Cut( xcut ) ) );
          pdf->plotOn( y_frame , RooFit::Slice(RooArgSet(xcut)), 
                       RooFit::ProjWData(RooArgSet(xcut) , *all_data ) ,
                       RooFit::Normalization(nds->numEntries(),RooAbsReal::NumEvent) , 
                       RooFit::LineColor(kRed) );
          RooFitResult* sfit = gspdf->fitTo( *nds , RooFit::Save(true) );
          xval.push_back( 10 );
          yval.push_back( gssigma->getVal() );
          gspdf->plotOn( y_frame , RooFit::LineColor(kBlue) );
          y_frame->Draw();
        }
        {
          g_canvas->cd(4);
          const char* xcut = "x >= 15";
          RooPlot* y_frame = y->frame( 40 );
          all_data->plotOn( y_frame , RooFit::Cut( xcut ) , 
                            RooFit::LineColor(kBlack) , RooFit::MarkerColor(kBlack) );
          RooDataSet* nds = dynamic_cast<RooDataSet*>( all_data->reduce( RooFit::Cut( xcut ) ) );
          pdf->plotOn( y_frame , RooFit::Slice(RooArgSet(xcut)), 
                       RooFit::ProjWData(RooArgSet(xcut) , *all_data ) ,
                       RooFit::Normalization(nds->numEntries(),RooAbsReal::NumEvent) , 
                       RooFit::LineColor(kRed) );
          RooFitResult* sfit = gspdf->fitTo( *nds , RooFit::Save(true) );
          xval.push_back( 15 );
          yval.push_back( gssigma->getVal() );
          gspdf->plotOn( y_frame , RooFit::LineColor(kBlue) );
          y_frame->Draw();
        }
        {
          g_canvas->cd(5);
          // plot res vs pt
          TGraph* gr = new TGraph;
          unsigned int n=0;
          // TF1* fitfunc = new TF1("fitfunc","[0] + ([1]*exp([2]*x)) + ([3]*x)",xmin,xmax);
          fitfunc->SetParameter(0,p0->getVal());;
          fitfunc->SetParameter(1,p1->getVal());;
          // fitfunc->SetParameter(2,p2->getVal());;
          // fitfunc->SetParameter(3,p3->getVal());;
          fitfunc->Print("v");
          for( double pt=1.; pt<20; pt+=0.5,++n ) {
            gr->SetPoint( n , pt , fitfunc->Eval(pt) ); 
          }
          gr->SetMarkerStyle(8);
          gr->SetLineColor(kRed);
          gr->SetMarkerColor(kRed);
          gr->SetMaximum(0.5);
          gr->SetMinimum(0.);
          gr->Draw("apl");
          TGraph* grdat = new TGraph;
          for( unsigned int i=0, f=xval.size(); i!=f; ++i ) {
            grdat->SetPoint(i,xval[i],yval[i]);
          }
          grdat->SetMarkerColor(kBlack);
          grdat->SetMarkerStyle(8);
          grdat->Draw("p");
          grdat->Fit( fitfunc );
        
        }
        ShowCommand::update_canvas();
      } // end show results
    } else {
      // default is resolution vs pt in variable width bins
      const bool debug = true;
      int nminperbin = 10;
      double constantbinsize = 1.;
      if( opt_autobin.found() ) { nminperbin = opt_autobin.value_int(); }
      else if( opt_constbin.found() ) { constantbinsize = opt_constbin.value_float(); } 
      double ymax = 10000.;
      if( opt_askres.value_bool() ) {
        ymax = askFloat( "maximum allowed |residual| size: " );
      }
      if( debug ) {
        cout << "nminperbin: " << nminperbin << endl;
        cout << "constantbinsize: " << constantbinsize << endl;
        cout << "ymax: " << ymax << endl;
      }
      // dataset variables
      RooRealVar* x = new RooRealVar( "x" , "P_{T}", 0 , 1000, "GeV" );
      RooRealVar* y = new RooRealVar( "y" , "Residual", -ymax , ymax, "" );
      RooDataSet* all_data = get_dataset( *x , *y );
      if( !all_data ) { cout << " couldn't find data to fit" << endl; return; }
      if( debug ) {
        cout << " n data points: " << all_data->numEntries() << endl;
      }
      if( all_data->numEntries() < 1 ) { return ; }
      // compute appropriate binninge. the current implementation of
      // this routine requires access to the underlying TTree
      // representation of the RooDataSet, but as of the RooFit 3
      // rewrite (ROOT 5.24) the underlying data structure is no
      // longer guaranteed (see
      // http://root.cern.ch/root/v524/Version524.news.html, "Redesign
      // of RooFit dataset class structure"). this means that the
      // following may need to be rewritten at some point. for now,
      // just check that the underlying structure is still a TTree and
      // grab it.
#if ROOT_VERSION_CODE >= ROOT_VERSION(5,24,0)
      RooAbsDataStore* dstore = all_data->store();
      assert( dstore );
      assert( dstore->InheritsFrom(RooTreeDataStore::Class())==kTRUE );
      TTree* tree( dynamic_cast<TTree*>( dstore->tree()->Clone()) );
#else
      TTree* tree( dynamic_cast<TTree*>(all_data->tree().Clone()) );
#endif
      vector<double> ptvals;
      for( unsigned int i=0, f=tree->GetEntries(); i!=f; ++i ) { 
        tree->GetEntry(i);
        ptvals.push_back( tree->GetLeaf("x")->GetValue() );
      }
      std::sort( ptvals.begin() , ptvals.end() , std::greater<double>() );
      vector<double> ptbin_low_edges; ptbin_low_edges.reserve( ptvals.size() );
      ptbin_low_edges.clear();
      ptbin_low_edges.push_back( ptvals.back() );
      if( debug ) { cout << ptbin_low_edges.back() << endl; }
      if( nminperbin >= 1 ) { 
        // automatic variable bins
        if( debug ) { cout << " bin boundaries (left edge): " << endl; }
        unsigned int iperbin = 0u;
        while( !ptvals.empty() ) {
          ++iperbin;
          if( iperbin >= nminperbin ) {
            ptbin_low_edges.push_back( ptvals.back() );
            if( debug ) { cout << ptbin_low_edges.back() << endl; }
            iperbin = 1u;
          }
          ptvals.pop_back();
        }
        // while( ptvals.size() >= nminperbin ) {
        //   ptbin_low_edges.push_back( ptvals[nminperbin-1] ); //+ std::numeric_limits<double>::min()
        //   if( debug ) { cout << ptbin_low_edges.back() << endl; }
        //   vector<double>::iterator ierase = ptvals.begin();
        //   std::advance( ierase , nminperbin );
        //   ptvals.erase( ptvals.begin() , ierase );
        // }
      } else {
        // uniform binning
        double maxpt = ptvals.front();
        if( opt_askx.value_bool() ) {
          maxpt = askFloat( "maximum x value to use:" );
        }
        const unsigned int nbins = static_cast<unsigned int>(std::ceil( maxpt / constantbinsize ));
        for( unsigned int ibin=1; ibin!=nbins; ++ibin ) {
          ptbin_low_edges.push_back( ibin*constantbinsize );
          if( debug ) { cout << ptbin_low_edges.back() << endl; }
        }
      }
      if( debug ) { cout << " n bins: " << ptbin_low_edges.size() << endl; }
      // measure mean and width per bin using a gaussian pdf
      RooRealVar* uMeanA = new RooRealVar( "uMeanA" , "u template PDF mean" , 0. , -ymax , ymax );
      // RooRealVar* uMeanC = new RooRealVar( "uMeanC" , "u template PDF mean C" , 0. , -ymax , ymax );
      RooRealVar* uWidthA = new RooRealVar( "uWidthA" , "u template PDF width A" , ymax/2. , 0 , ymax );
      // RooRealVar* uWidthB = new RooRealVar( "uWidthB" , "u template PDF width B" , 1.5 , 0 , ymax );
      RooRealVar* uWidthC = new RooRealVar( "uWidthC" , "u template PDF width C" , ymax/2. , 0 , ymax );
      // RooGaussian* uGaussB = new RooGaussian( "uGaussB" , "u template PDF gaussian B" , *y , *uMean , *uWidthB );
      RooGaussian* uGaussA = new RooGaussian( "uGaussA" , "u template PDF gaussian A" , *y , *uMeanA , *uWidthA );
      RooGaussian* uGaussC = new RooGaussian( "uGaussC" , "u template PDF gaussian C" , *y , *uMeanA , *uWidthC );
      RooRealVar* nFA = new RooRealVar( "nFA" , "number of gaussian A events" , all_data->numEntries() , 0 , all_data->numEntries() );
      RooRealVar* nFC = new RooRealVar( "nFC" , "number of gaussian C events" , 0 , 0 , all_data->numEntries() );
      // RooRealVar* uFC = new RooRealVar( "uFC" , "fraction of core gaussian" , 0.25 , 0 , 1. );
      // RooAddPdf* uModel = new RooAddPdf( "uModel" , "u template double gaussian" , RooArgList(*uGaussA,*uGaussB,*uGaussC) , RooArgList(*uFA,*uFC) );
      RooAddPdf* uModel = new RooAddPdf( "uModel" , "u template double gaussian" , RooArgList(*uGaussA,*uGaussC) , RooArgList(*nFA,*nFC) );
      
      // RooRealVar* gsmean  = new RooRealVar( "gsmean" , "mean of single gaussian" , 0. , -ymax , ymax );
      // RooRealVar* gssigma  = new RooRealVar( "gssigma" , "width of single gaussian" , 1 , 0. , ymax );
      // RooGaussian* gspdf = new RooGaussian( "gspdf" , "gaussian pdf" , *y , *gsmean , *gssigma );
      RooRealVar* gsmean  = uMeanA;
      RooRealVar* gssigma  = uWidthA;
      RooAbsPdf* gspdf = uModel;
      //
      RooRealVar* mvar_all = all_data->meanVar(*x);
      RooRealVar* rvar_all = all_data->rmsVar(*x);
      //
      const bool xweighted = ! opt_notweighted.value_bool(); 
      //
      vector<double> ptavg;
      vector<double> ptrms;
      vector<double> ptmean;
      vector<double> ptmean_err;
      vector<double> ptwidth;
      vector<double> ptwidth_err;
      for( unsigned int ibin=0, fbin=ptbin_low_edges.size(); ibin!=fbin; ++ibin ) {
        // get subset of data corresponding to this bin
        const double low_edge = ptbin_low_edges[ibin];
        const double high_edge = (ibin+1)!=fbin ? ptbin_low_edges[ibin+1] : std::numeric_limits<double>::max();
        string cutstring;
        if( (ibin+1)!=fbin ) { 
          cutstring = (format( "x>=%f && x<%f" ) % low_edge % high_edge ).str();
        } else {
          cutstring = (format( "x>=%f" ) % low_edge).str();
        }
        RooDataSet* subset = dynamic_cast<RooDataSet*>(all_data->reduce( RooFit::Cut(cutstring.c_str()) )); assert( subset );
        if( debug ) { cout << " bin " << ibin << " cutstring: " << cutstring.c_str() << " has " << subset->numEntries() << " entries." << endl; }
        if( subset->numEntries() < 10 ) { continue; }
        RooRealVar* mvar = subset->meanVar(*x);
        RooRealVar* rvar = subset->rmsVar(*x);
        uMeanA->setVal( mvar->getVal() );
        // uMeanC->setVal( mvar->getVal() );
        uWidthA->setVal( rvar->getVal() );
        uWidthC->setVal( 4.*(rvar->getVal()) );
        nFA->setVal( subset->numEntries()*0.95 );
        nFC->setVal( subset->numEntries()*0.1 );
        RooFitResult* sfit = gspdf->fitTo( *subset , RooFit::Save(true) , RooFit::Verbose(false) , RooFit::Extended(true) );
        RooArgList final_pars( sfit->floatParsFinal() );
        RooRealVar* fit_mean = (RooRealVar*)final_pars.at( 2 );
        RooRealVar* fit_width = (RooRealVar*)final_pars.at( 3 );
        if( opt_secondgaus.value_bool() ) {
          fit_mean = (RooRealVar*)final_pars.at( 2 );
          fit_width = (RooRealVar*)final_pars.at( 4 );
        }
        if( std::abs(mvar->getVal()) >= std::numeric_limits<double>::max()-10. ) { continue; } // suppress weird, pathological behavior
        if( xweighted ) { 
          ptavg.push_back( mvar->getVal()  );
          ptrms.push_back( rvar->getVal() );
        } else { 
          if( (ibin+1)!=fbin ) { ptavg.push_back( 0.5*(high_edge+low_edge) ); } else { ptavg.push_back( low_edge ); }
          if( (ibin+1)!=fbin ) { ptrms.push_back( 0.5*(high_edge-low_edge) ); } else { ptrms.push_back( 0. ); }
        }
        ptmean.push_back( fit_mean->getVal() );
        ptmean_err.push_back( fit_mean->getError() );
        ptwidth.push_back( fit_width->getVal() );
        ptwidth_err.push_back( fit_width->getError() );
        if( debug ) {
          cout << " DEBUGGING RESULT FOR BIN " << ibin << endl;
          cout << " mean: " << ptmean.back() << " " << ptmean_err.back() << endl;
          cout << " width: " << ptwidth.back() << " " << ptwidth_err.back() << endl;
          sfit->Print();
        }
      }
      // plot fit results
      ShowCommand::new_canvas();
      g_canvas->Divide(2,2);
      TGraphErrors* grresult_mean = new TGraphErrors;
      TGraphErrors* grresult_width = new TGraphErrors;
      for( unsigned int ibin=0, fbin=ptavg.size(); ibin!=fbin; ++ibin ) {
        grresult_mean->SetPoint(ibin,ptavg[ibin],ptmean[ibin]);
        grresult_width->SetPoint(ibin,ptavg[ibin],ptwidth[ibin]);
        grresult_mean->SetPointError(ibin,ptrms[ibin],ptmean_err[ibin]);
        grresult_width->SetPointError(ibin,ptrms[ibin],ptwidth_err[ibin]);
      }
      grresult_mean->SetLineColor(kRed);
      grresult_mean->SetMarkerColor(kRed);
      grresult_width->SetLineColor(kRed);
      grresult_width->SetMarkerColor(kRed);
      // figure out axes labels
      string ylabelmean;
      string ylabelwidth;
      {
        string ylabelunits = "";
        string tmp = _y_label;
        vector<string> splits;
        boost::split( splits , _y_label , boost::is_any_of("()") );
        ylabelmean = "Resolution Bias";
        ylabelwidth = "Core Resolution";
        if( splits.size()>1 ) {
          ylabelunits = splits[1];
          ylabelmean += " (" + ylabelunits + ")";
          ylabelwidth += " (" + ylabelunits + ")";
        }
      }
      grresult_mean->GetXaxis()->SetTitle( _x_label.c_str() );
      grresult_width->GetXaxis()->SetTitle( _x_label.c_str() );
      grresult_mean->GetYaxis()->SetTitle( ylabelmean.c_str() );
      grresult_width->GetYaxis()->SetTitle( ylabelwidth.c_str() );
      if( debug ) { 
        cout << " result nbins: " << grresult_mean->GetN() << " " << grresult_width->GetN() << endl; 
      }
      g_canvas->cd(3);
      // grresult_mean->SetMinimum( -ymax );
      // grresult_mean->SetMaximum( ymax );
      grresult_mean->Draw("ap");
      g_canvas->cd(4);
      // grresult_width->SetMinimum( -ymax );
      // grresult_width->SetMaximum( ymax );
      grresult_width->Draw("ap");
      // plot example fits
      if( ptbin_low_edges.size()>2 ) {
        g_canvas->cd(1);
        const double low_edge = ptbin_low_edges[0];
        const double high_edge = ptbin_low_edges[1];
        string cutstring = (format( "x>=%f && x<%f" ) % low_edge % high_edge ).str();
        RooDataSet* subset = dynamic_cast<RooDataSet*>(all_data->reduce( RooFit::Cut(cutstring.c_str()) )); assert( subset );
        RooFitResult* sfit = gspdf->fitTo( *subset , RooFit::Save(true) , RooFit::Verbose(false) );
        RooPlot* y_frame = y->frame( 40 );
        subset->plotOn( y_frame , RooFit::LineColor(kRed) , RooFit::MarkerColor(kRed) );
        gspdf->plotOn( y_frame , RooFit::LineColor(kBlue) );
        y_frame->Draw();
      }
      if( ptbin_low_edges.size()>2 ) {
        g_canvas->cd(2);
        const double low_edge = ptbin_low_edges[ptbin_low_edges.size()-2];
        const double high_edge = ptbin_low_edges[ptbin_low_edges.size()-1];
        string cutstring = (format( "x>=%f && x<%f" ) % low_edge % high_edge ).str();
        RooDataSet* subset = dynamic_cast<RooDataSet*>(all_data->reduce( RooFit::Cut(cutstring.c_str()) )); assert( subset );
        RooFitResult* sfit = gspdf->fitTo( *subset , RooFit::Save(true) , RooFit::Verbose(false) );
        RooPlot* y_frame = y->frame( 40 );
        subset->plotOn( y_frame , RooFit::LineColor(kRed) , RooFit::MarkerColor(kRed) );
        gspdf->plotOn( y_frame , RooFit::LineColor(kBlue) );
        y_frame->Draw();
      }
      ShowCommand::update_canvas();
    }
  } // done with operator()

public:
  ResolutionVsXCommand() 
    : AbsCommand( "xresfit" , "RooFit resolution vs x pdf (currently d0 vs pt only)" ) 
    , opt_d0ptmodel( this , CommandOption::BOOL , "d0ptmodel" , "use d0 pt model" )
    , opt_autobin( this , CommandOption::INTEGER , "autobin" , "automatically min with the minimum number of entries per x bin specified" )
    , opt_constbin( this , CommandOption::FLOAT , "constbin" , "use uniform bin size (x)" )
    , opt_askres( this , CommandOption::BOOL , "askres" , "ask for maximum allowed |residual| size" )
    , opt_askx( this , CommandOption::BOOL , "askx" , "ask for maximum x value to use" )
    , opt_notweighted( this , CommandOption::BOOL , "notweighted" , "don't weight" )
    , opt_secondgaus( this , CommandOption::BOOL , "secondgaus" , "use a double gaussian" )
    , _use_limits_x(false)
    , _use_limits_y(false)
  {}

};

class 
TaggerResponseCurveCommand : public AbsCommand
{
public:
  CommandOption opt_current;
  CommandOption opt_swap;
protected:
  virtual void operator()( string& command ) {
    TH1* h1 = 0;
    TH1* h2 = 0;
    if( opt_current.value_bool() ) { 
      // b and u likelihood results are overlaid on the current plot stack, so just grab them.
      vector< shared_ptr<AbsPlot> >::const_iterator i = std::find_if( g_plot_stack.begin() , g_plot_stack.end() , bind(&AbsPlot::isFirstGraphic,_1)==true );
      if( i == g_plot_stack.end() ) { return; }
      shared_ptr< const PlotHistogram1D<DgHistogram1D> > hbw = dynamic_pointer_cast<const PlotHistogram1D<DgHistogram1D> >( *i );
      if( !hbw ) { return; }
      if( hbw ) { h1 = dynamic_cast<TH1*>( hbw->new_ROOT( "tmproohist" ) ); }
      if( !h1 ) { return; }
      for( vector< shared_ptr<AbsPlot> >::const_iterator i2=g_plot_stack.begin(), f=g_plot_stack.end(); i2!=f; ++i2 ) {
        if( i2==i ) { continue; }
        if( !( (*i2)->isData()) ) { continue; }
        shared_ptr< const PlotHistogram1D<DgHistogram1D> > h2bw = dynamic_pointer_cast<const PlotHistogram1D<DgHistogram1D> >( *i2 );
        if( h2bw ) { h2 = dynamic_cast<TH1*>( h2bw->new_ROOT( "tmproohist" ) ); }
      }
      if( !h2 ) { return; }
    } else {
      // try to retrieve b and u likelihood results from subcollections.
      // dg::SubColl::const_iterator ib=dg::current()->subcollections().find( "bjets" );
      // dg::SubColl::const_iterator ilight=dg::current()->subcollections().find( "lightjets" );
      // if( ib==dg::current()->subcollections().end() || ilight==dg::current()->subcollections().end() ) {
      //   cout << " can't find b or light collections" << endl;
      //   return;
      // }
      // // get b and u jet histograms
      // dg::SubColl::const_iterator ibjets = dg::subcollection_ptr(ib).subcollections()->find( "jets" );
      // if( ibjets == dg::subcollection_ptr(ib)->end() ) {
      //   cout << " can't find b jet results" << endl;
      //   return;
      // }
      // dg::SubColl::const_iterator ilightjets = dg::subcollection_ptr(ilight)->subcollections().find( "jets" );
      // if( ilightjets == dg::subcollection_ptr(ilight)->end() ) {
      //   cout << " can't find light jet results" << endl;
      //   return;
      // }
      // string histname = "r_classifier_x";
      // dg::ResourceColl::const_iterator ibhist = dg::subcollection_ptr(ibjets)->find( histname );
      // dg::ResourceColl::const_iterator ilighthist = dg::subcollection_ptr(ilightjets)->find( histname );
      // if( ibhist == dg::subcollection_ptr(ibjets)->end() ) {
      //   cout << " can't find b jet result histogram" << endl;
      //   return;
      // }
      // if( ilighthist == dg::subcollection_ptr(ilightjets)->end() ) {
      //   cout << " can't find light jet result histogram" << endl;
      //   return;
      // }
      DgCollection::View bv = dg::current()->get( "tags/bjets/jets/r_x" );
      DgCollection::View lightv = dg::current()->get( "tags/lightjets/jets/r_x" );
      if( bv.empty() && lightv.empty() ) { 
        // try old location
        bv = dg::current()->get( "bjets/jets/r_classifier_x" );
        lightv = dg::current()->get( "lightjets/jets/r_classifier_x" );
      }
      if( bv.empty() ) {
        cout << " can't find b jet result histogram" << endl;
        return;
      }
      if( lightv.empty() ) {
        cout << " can't find light jet result histogram" << endl;
        return;
      }
      h1 = bv.front().graphic() ? dynamic_cast<TH1*>(bv.front().graphic()->new_ROOT("tmpbhist")) : 0 ;
      h2 = lightv.front().graphic() ? dynamic_cast<TH1*>(lightv.front().graphic()->new_ROOT("tmplighthist")) : 0 ;
      if( !h1 ) { cout << " logic error: no b hist" << endl; }
      if( !h2 ) { cout << " logic error: no light hist" << endl; }
    }
    if( opt_swap.value_bool() ) { std::swap( h1 , h2 ); }
    TGraphAsymmErrors* gr = new TGraphAsymmErrors;
    TH1* hb = h1;
    TH1* hu = h2;
    // taken from Andrea Coccaro's TrigBjetAnalysis code.
    // taken from Andrea Coccaro's TrigBjetAnalysis code, then modified.
    double totb = hb->Integral()+hb->GetBinContent(hb->GetNbinsX()+1); //hb->GetEntries();
    double totu = hu->Integral()+hu->GetBinContent(hu->GetNbinsX()+1); //hu->GetEntries();
    double effu,effb,seffu,seffb;
    double sw(0.), sx(0.), sy(0.), sxx(0.), sxy(0.);
    int ind=0;
    const double xend = hb->GetXaxis()->GetBinLowEdge(hb->GetNbinsX()) + hb->GetXaxis()->GetBinWidth(hb->GetNbinsX());
    vector<double> beff; // ordered with decreasing tagging eff
    vector<double> urej;
    for( unsigned int ibin=0, fbin=hb->GetNbinsX()+1; ibin!=fbin; ++ibin ) {
      const double xbegin = hb->GetXaxis()->GetBinLowEdge(ibin);
      hb->GetXaxis()->SetRangeUser(xbegin,xend);
      hu->GetXaxis()->SetRangeUser(xbegin,xend);
      double sumb = hb->Integral()+hb->GetBinContent(hb->GetNbinsX()+1);
      double sumu = hu->Integral()+hu->GetBinContent(hu->GetNbinsX()+1);
      double b_frac , b_frac_err_min , b_frac_err_max ;
      double u_frac , u_frac_err_min , u_frac_err_max ;
      DataGraphics::FindEfficiencyWithAsymmetricError(sumb,totb,b_frac,b_frac_err_min,b_frac_err_max);
      DataGraphics::FindEfficiencyWithAsymmetricError(sumu,totu,u_frac,u_frac_err_min,u_frac_err_max);
      effb  = b_frac;
      effu  = u_frac;
      if( effu!=0 && effb!=0 ) { 
        gr->SetPoint(ind,effb,1./effu);
        gr->SetPointError(ind,b_frac_err_min,b_frac_err_max,u_frac_err_min/(effu*effu),u_frac_err_max/(effu*effu));
        ++ind;
        cout << setw(10) << effb << " " << setw(10) << (1./effu) << endl;
        beff.push_back( effb );
        urej.push_back( 1./effu );
        if( effb>0. && effb<1. ) {
          const double w=(u_frac_err_max/(effu*effu))*(u_frac_err_max/(effu*effu));
          sw += w;
          sx += w*effb;
          sy += w*std::log(1./effu);
          sxx += w*effb*effb;
          sxy += w*effb*std::log(1./effu);
        }
      }
    }
    ShowCommand::new_canvas();
    gr->SetMarkerStyle( 8 );
    gr->SetLineWidth( 3 );
    gr->SetMarkerColor( kBlack );
    gr->SetLineColor( kBlack );
    gr->SetMinimum(0.5);
    gr->GetXaxis()->SetTitle( "b tagging efficiency" );
    gr->GetYaxis()->SetTitle( "light quark rejection" );
    gr->Draw("ap");
    if( g_canvas ) { g_canvas->SetLogy(1); }
    const double delta = (sw*sxx) - (sx*sx);
    if( true && !beff.empty() && beff.size()==urej.size() ) {
      // const double intercept( (sxx*sy - sx*sxy)/delta );
      // const double slope( (sw*sxy - sx*sy)/delta );
      // cout << " slope: " << slope << " intercept: " << intercept << endl;
      // interpolate between closest points to each decade of efficiency.
      cout << boost::format( " %|16s| %|20t| %|15s|" ) % "rejection factor" % "tagging efficiency" << endl;
      for( int i=4; i<=10; ++i ) {
        const double x = i*0.1;
        //        const double rej = std::exp( intercept + x*slope );
        // find point with beff at or just below x.
        vector<double>::reverse_iterator iabove = std::find_if( beff.rbegin() , beff.rend() , boost::bind(greater_equal<double>(),_1,x) );
        vector<double>::iterator ibelow = std::find_if( beff.begin() , beff.end() , boost::bind(less_equal<double>(),_1,x) );
        double x2,y2;
        if( iabove==beff.rend() ) {
          x2 = beff.front();
          y2 = urej.front();
        } else {
          x2 = *iabove;
          y2 = urej[ beff.size() - std::distance(beff.rbegin(),iabove) - 1 ];
        }
        double x1,y1;
        if( ibelow==beff.end() ) {
          x1 = beff.back();
          y1 = urej.back();
        } else {
          x1 = *ibelow;
          y1 = urej[ std::distance(beff.begin(),ibelow) ];
        }
        // vector<double>::reverse_iterator iabove = std::find_if( beff.rbegin() , beff.rend() , boost::bind(greater_equal<double>(),_1,x) );
        // if( iabove == beff.rend() ) { continue; }
        // double x1 = *iabove;
        // double y1 = urej[ beff.size()-std::distance(beff.rbegin(),iabove)-1 ];
        // vector<double>::reverse_iterator ibelow( iabove );
        // ++ibelow;
        // double x2 = (ibelow!=beff.rend()) ? *ibelow : beff.front();
        // double y2 = (ibelow!=beff.rend()) ? urej[ beff.size()-std::distance(beff.rbegin(),ibelow)-1 ] : urej.front();
        // interpolate with an exponential;
        double l1 = y1>0. ? std::log( y1 ) : -9999.;
        double l2 = y2>0. ? std::log( y2 ) : -9999.;
        double slope = (l2-l1)/(x2-x1);
        double rej = std::exp( (slope*(x-x1)) + l1 );
        cout << boost::format( " %|16f| %|20t| %|15f| %|15f| %|15f| %|15f| %|15f| " ) % rej % x % x1 % y1 % x2 % y2 << endl;
      }
      // if( istarts_with(command, "dofit" ) && delta!=0. ) {
      //   TF1* fitfunc = new TF1("fitfunc","expo",0.,1.);
      //   fitfunc->SetParameter(0,intercept);
      //   fitfunc->SetParameter(1,slope);
      //   fitfunc->Draw("same");
      // }
    }
    gr->SetName( "grtag" );
    ShowCommand::update_canvas();
  }
public:
  TaggerResponseCurveCommand() 
    : opt_current( this , CommandOption::BOOL , "current" , "grab b and u likelihood from current plot stack" )
    , opt_swap( this , CommandOption::BOOL , "swap" , "swap the b and u histograms" )
    , AbsCommand( "tagcurve" , "compute tagger response curve given b and u quark result histograms" ) {}
};


class 
IntegralCommand : public AbsCommand
{
public:
  CommandOption opt_thint;
protected:
  virtual void operator()( string& command ) {
    if( ! opt_thint.value_bool() ) {
      // get data object integral.
      for( vector< shared_ptr<AbsPlot> >::const_iterator i=g_plot_stack.begin() , f=g_plot_stack.end(); i!=f; ++i ) {
        shared_ptr<const AbsPlot> p( *i );
        shared_ptr<const DgHistogram1D> h1 = dynamic_pointer_cast<const DgHistogram1D>(p);
        shared_ptr<const DgHistogramWeighted1D> h1w = dynamic_pointer_cast<const DgHistogramWeighted1D>(p);
        const double sf = g_luminosity ? (SampleNormalization::instance()->rescale_factor(p->subsample(),*g_luminosity)) : 1.;
        if( h1w ) {
          double intw = h1w->integrate_weight();
          cout << " " << setw(30) << p->description() << " " << setw(10) << intw << " " << setw(10) << (intw*sf) << endl;
        } else if( h1 ) {
          DataGraphics::Count intw = h1->integrate();
          cout << " " << setw(30) << p->description() << " " << setw(10) << intw << " " << setw(10) << (intw*sf) << endl;
        }
      }
    } else {
      if( g_canvas ) {
        TList* prims = g_canvas->GetListOfPrimitives();
        TIterator* iter = prims ? prims->MakeIterator() : 0;
        while( iter ) {
          TObject* obj = iter->Next();
          if( !obj ) { break; }
          TH1* h = dynamic_cast<TH1*>(obj);
          if( !h ) { continue; }
          cout << " " << setw(30) << h->GetTitle() << " " << setw(10) << h->Integral() << endl;
        }
      }
    }
  }
public:
  IntegralCommand() 
  : AbsCommand( "integral" , "compute integral for each relevant plot object" )
  , opt_thint( this , CommandOption::BOOL , "thint" , "use TH1::Integral() instead of dg integral" )
  {}
};

class 
MeanCommand : public AbsCommand
{
protected:
  virtual void operator()( string& command ) {
    for( vector< shared_ptr<AbsPlot> >::const_iterator i=g_plot_stack.begin() , f=g_plot_stack.end(); i!=f; ++i ) {
      shared_ptr<const AbsPlot> p( *i );
      shared_ptr<const DgHistogram1D> h1 = dynamic_pointer_cast<const DgHistogram1D>(p);
      if( h1 ) {
        cout << " " << setw(30) << p->description() << " " << setw(10) << h1->mean() << endl;
      }
    }
  }
public:
  MeanCommand() : AbsCommand( "mean" , "compute mean for each relevant plot object" ) {}
};

class 
RMSCommand : public AbsCommand
{
protected:
  virtual void operator()( string& command ) {
    for( vector< shared_ptr<AbsPlot> >::const_iterator i=g_plot_stack.begin() , f=g_plot_stack.end(); i!=f; ++i ) {
      shared_ptr<const AbsPlot> p( *i );
      shared_ptr<const DgHistogram1D> h1 = dynamic_pointer_cast<const DgHistogram1D>(p);
      if( h1 ) {
        cout << " " << setw(30) << p->description() << " " << setw(10) << h1->rms() << endl;
      }
    }
  }
public:
  RMSCommand() : AbsCommand( "rms" , "compute RMS for each relevant plot object" ) {}
};

class 
UnderflowCommand : public AbsCommand
{
protected:
  virtual void operator()( string& command ) {
    for( vector< shared_ptr<AbsPlot> >::const_iterator i=g_plot_stack.begin() , f=g_plot_stack.end(); i!=f; ++i ) {
      shared_ptr<const AbsPlot> p( *i );
      shared_ptr<const DgHistogram1D> h1 = dynamic_pointer_cast<const DgHistogram1D>(p);
      shared_ptr<const DgHistogram2D> h2 = dynamic_pointer_cast<const DgHistogram2D>(p);
      if( h1 ) {
        cout << " " << setw(30) << p->description() << " " << setw(10) << h1->underflow() << endl;
      } else if( h2 ) {
        cout << " " << setw(30) << p->description() << " " << setw(10) << h2->underflow_x() << " " << h2->underflow_y() << endl;
      }
    }
  }
public:
  UnderflowCommand() : AbsCommand( "underflow" , "compute underflow for each relevant plot object" ) {}
};

class 
OverflowCommand : public AbsCommand
{
protected:
  virtual void operator()( string& command ) {
    for( vector< shared_ptr<AbsPlot> >::const_iterator i=g_plot_stack.begin() , f=g_plot_stack.end(); i!=f; ++i ) {
      shared_ptr<const AbsPlot> p( *i );
      shared_ptr<const DgHistogram1D> h1 = dynamic_pointer_cast<const DgHistogram1D>(p);
      shared_ptr<const DgHistogram2D> h2 = dynamic_pointer_cast<const DgHistogram2D>(p);
      if( h1 ) {
        cout << " " << setw(30) << p->description() << " " << setw(10) << h1->overflow() << endl;
      } else if( h2 ) {
        cout << " " << setw(30) << p->description() << " " << setw(10) << h2->overflow_x() << " " << h2->overflow_y() << endl;
      }
    }
  }
public:
  OverflowCommand() : AbsCommand( "overflow" , "compute overflow for each relevant plot object" ) {}
};

class 
LsPlotCommand : public AbsCommand
{
protected:
  virtual void operator()( string& command ) {
    for( vector< shared_ptr<AbsPlot> >::iterator i=g_plot_stack.begin() , f=g_plot_stack.end(); i!=f; ++i ) {
      cout << format("%|50s| %|20s| %|100s|") % (*i)->source_file() % (*i)->subsample() % (*i)->description() << endl;
    }
  }
public:
  LsPlotCommand() : AbsCommand( "lsplot" , "list current plot stack" ) {}
};

class 
AspectRatioCommand : public AbsCommand
{
public:
  CommandOption opt_aspect;
  CommandOption opt_monitor;
protected:
  virtual void operator()( string& cmd ) {
    string command(cmd);
    if( opt_aspect.found() ) { 
      if( istarts_with(opt_aspect.value_string(),"square") ) {
        g_aspect_ratio = 1.;
      } else if( istarts_with(opt_aspect.value_string(),"widescreen") ||
                 istarts_with(opt_aspect.value_string(),"default") ) {
        g_aspect_ratio = 16/9.;
      } else if( istarts_with(opt_aspect.value_string(),"tufte") ) {
        g_aspect_ratio = 1.75;
      } else if( istarts_with(opt_aspect.value_string(),"long") ) {
        g_aspect_ratio = 0.5;
      } else if( istarts_with(opt_aspect.value_string(),"gotham") ) {
        g_aspect_ratio = 1024/640.;
      } else if( istarts_with(opt_aspect.value_string(),"def52") ) {
        g_aspect_ratio = 1.43;
      } else if( istarts_with(opt_aspect.value_string(),"custom") ) {
        g_aspect_ratio = askFloat( "aspect ratio: " );
      }
    }
    cout << " aspect ratio is now: " << g_aspect_ratio << endl;
    if( opt_monitor.found() ) { 
      // set screen size
      if( istarts_with(opt_monitor.value_string(),"wide_monitor") ) {
        g_display_width = 1600;
        g_display_height = 1100;
      } else if( istarts_with(opt_monitor.value_string(),"fnal_monitor") ) {
        g_display_width = 1024;
        g_display_height = 700;      
      } else if( istarts_with(opt_monitor.value_string(),"ibook_monitor") ) {
        g_display_width = 1024;
        g_display_height = 700;      
      } else if( istarts_with(opt_monitor.value_string(),"macbook_monitor") ) {
        g_display_width = 1440;
        g_display_height = 800;      
      }
    }
  }
public:
  AspectRatioCommand() 
  : AbsCommand( "aspect" , "set the aspect ratio.  valid options are \"widescreen\", \"square\", and \"tufte\"." ) 
  , opt_aspect( this , CommandOption::STRING , "aspect" , "aspect ratio: square, widescreen, tufte, long, gotham, def52, custom" )
  , opt_monitor( this , CommandOption::STRING , "monitor" , "screen resolution: wide_monitor, fnal_monitor, ibook_monitor, macbook_monitor" )
  {}
};

class 
LabelCommand : public AbsCommand
{
public:
  CommandOption opt_xlabel;
  CommandOption opt_ylabel;
  CommandOption opt_zlabel;
protected:
  virtual void operator()( string& command ) {
    if( opt_xlabel.found() ) { 
      shared_ptr<AbsPlot> p( make_shared<PlotAxisLabel>( PlotAxisLabel::X , opt_xlabel.value_string() ) );
      g_plot_stack.push_back(p);
    }
    if( opt_ylabel.found() ) { 
      shared_ptr<AbsPlot> p( make_shared<PlotAxisLabel>( PlotAxisLabel::Y , opt_ylabel.value_string() ) );
      g_plot_stack.push_back(p);
    }
    if( opt_zlabel.found() ) { 
      shared_ptr<AbsPlot> p( make_shared<PlotAxisLabel>( PlotAxisLabel::Z , opt_zlabel.value_string() ) );
      g_plot_stack.push_back(p);
    }
  }
public:
  LabelCommand() 
    : opt_xlabel( this , CommandOption::STRING , "xlabel" , "x axis label" )
    , opt_ylabel( this , CommandOption::STRING , "ylabel" , "y axis label" )
    , opt_zlabel( this , CommandOption::STRING , "zlabel" , "z axis label" )
    , AbsCommand( "label" , "label an axis." ) 
  {}
};

class 
ClearQueueCommand : public AbsCommand
{
protected:
  virtual void operator()( string& command ) {
    app->ClearQueue();
  }
public:
  ClearQueueCommand() : AbsCommand( "clearqueue" , "clear the command queue." ) {}
};

class 
ProfileXCommand : public AbsCommand
{
protected:
  virtual void operator()( string& command ) {
    shared_ptr<AbsPlot> p( make_shared<PlotProfile>( PlotProfile::X , "profx" , "" , "" ) );
    g_plot_stack.push_back(p);
  }
public:
  ProfileXCommand() : AbsCommand( "profilex" , "overlay an x profile histogram" ) {}
};

class 
ProfileYCommand : public AbsCommand
{
protected:
  virtual void operator()( string& command ) {
    shared_ptr<AbsPlot> p( make_shared<PlotProfile>( PlotProfile::Y , "profy" , "" , "" ) );
    g_plot_stack.push_back(p);
  }
public:
  ProfileYCommand() : AbsCommand( "profiley" , "overlay an y profile histogram" ) {}
};

class 
FakeMatrixCommand : public AbsCommand
{
public:
  CommandOption opt_fakerate;
protected:
  virtual void operator()( string& command ) {
    if( opt_fakerate.value_bool() ) { 
      // show fake rates and exit.
    }
  }
public:
  FakeMatrixCommand()
    : AbsCommand( "fakematrix" , "compute matrix method fake rates etc." )
    , opt_fakerate( this , CommandOption::BOOL , "fakerate" , "compute fake rate" )
  {}
};

class 
CutflowCommand : public AbsCommand
{
public:
  CommandOption opt_suffix;
  CommandOption opt_notnum;
protected:
  virtual void operator()( string& command ) {
    const bool debug = opt_debug.value_bool();
    string suffix = opt_suffix.found() ? opt_suffix.value_string() : "";
    const bool numbered = ! opt_notnum.value_bool();
    // retreive all 0-D efficiency objects in current collection matching the regular expression.
    // the expression contains an (optional) string and then a number.
    boost::regex cut_regex( string("([A-Za-z]*)(\\d+)_?(.*)")+suffix );
    const dg::ResourceColl& coll = dg::current()->graphics();
    typedef map< string , map<unsigned int, shared_ptr<const DgAbsGraphic> > > cut_coll;
    cut_coll cuts;
    map< string , map<unsigned int, string> > cutdesc;
    //
    const unsigned int begin_spare_int = 10000;
    vector< unsigned int > spare_ints; spare_ints.reserve(100);
    for( unsigned int i=begin_spare_int+100; i!=begin_spare_int; --i ) { spare_ints.push_back(i); }
    //
    for( dg::ResourceColl::const_iterator icoll=coll.begin(), fcoll=coll.end(); icoll!=fcoll; ++icoll ) {
      const string& name( dg::current()->graphic_name(icoll) );
      // bool numbered = boost::regex_match( name , boost::regex("\\d") );
      boost::smatch what;
      const bool is_matched = boost::regex_match( name , what , cut_regex );
      if( numbered && ! is_matched ) { continue; }
      shared_ptr<const DgAbsGraphic> ptr( dg::current()->graphic_resource(icoll) );
      shared_ptr<const DgEfficiency0D> bw = dynamic_pointer_cast<const DgEfficiency0D>( ptr );
      shared_ptr<const DgEfficiencyWeighted0D> bww = dynamic_pointer_cast<const DgEfficiencyWeighted0D>( ptr );
      if( !bw && !bww ) { continue; }
      // figure out cut order label string, e.g. "c1_" or "cut2_".
      if( numbered ) {
        string cutlabel;
        if( what[1].matched ) { cutlabel = string(what[1].first , what[1].second ); } else { cutlabel = ""; }
        unsigned int cutint;
        assert(  what[2].matched );
        cutint = std::atoi(string(what[2].first,what[2].second).c_str());
        cuts[ cutlabel ][ cutint ] = ptr;
        if( what[3].matched ) { 
          string desc(what[3].first,what[3].second);
          boost::trim_if(desc,is_any_of("_"));
          cutdesc[ cutlabel ][ cutint ] = desc;
        }
      } else {
        unsigned int cutint = spare_ints.back();
        spare_ints.pop_back();
        cuts[ "" ][ cutint ] = ptr;
        cutdesc[ "" ][ cutint ] = name;
      }
    }
    // reject empty list
    if( cuts.empty() ) { 
      cout << " no matching 0D efficiency histograms in current directory." << endl; 
      cout << "  use the 'notnum' option if efficiency histograms are not numbered (e.g. n-1 DgCutSet)" << endl;
      return;
    }
    // present list of cuts, sort first by cut label string, then by numeric order.
    cout << ( boost::format( "%5s  %2s  %6s  %6s  %6s  %11s  %11s" ) %
              "Name" % "" % "Eff" % "+" % "-" % "Denom." % "Numer." )
         << endl;
    for( cut_coll::const_iterator ilabel=cuts.begin(), flabel=cuts.end(); ilabel!=flabel; ++ilabel ) {
      for( map<unsigned int, shared_ptr<const DgAbsGraphic> >::const_iterator icut=ilabel->second.begin(), fcut=ilabel->second.end(); icut!=fcut; ++icut ) {
        shared_ptr<const DgAbsGraphic> ptr = icut->second;
        shared_ptr<const DgEfficiency0D> bw = dynamic_pointer_cast<const DgEfficiency0D>( ptr );
        shared_ptr<const DgEfficiencyWeighted0D> bww = dynamic_pointer_cast<const DgEfficiencyWeighted0D>( ptr );
        const unsigned int width_xmin = 15;
        const unsigned int width_xmax = 15;
        const unsigned int width_central = 15;
        const unsigned int width_elow = 15;
        const unsigned int width_ehigh = 15;
        double xmin, xmax, central, elow, ehigh;
        const unsigned int bingroupsize = 1;
        DataGraphics::Count num = 0;
        DataGraphics::Count den = 0;
	if( bww ) { 
          bww->efficiency( central , elow , ehigh );
          num = bww->numerator();
          den = bww->denominator();
        } else if( bw ) {
          bw->efficiency( central , elow , ehigh );          
          num = bw->numerator();
          den = bw->denominator();
        } else { cout << " error at " << __LINE__ << endl; return; } // not handled
        int most_decade, least_decade, sigfigs, dec_col;
        //TableCommand::determine_precision_from_uncertainties( central , elow , ehigh , most_decade , least_decade , sigfigs , dec_col );
        // cout << std::left << setw(width_xmin) << xmin << " " << std::left << setw(width_xmax) << xmax << "   "
        //      << std::left << setw(width_central) << TableCommand::align_decimal(round_value(central,sigfigs,most_decade),dec_col)
        //      << " + " << std::left << setw(width_ehigh) << TableCommand::align_decimal(round_value(ehigh,sigfigs,most_decade),dec_col)
        //      << " - " << std::left << setw(width_elow) << TableCommand::align_decimal(round_value(elow,sigfigs,most_decade),dec_col);
        string desc = cutdesc[ilabel->first][icut->first];
        cout << (boost::format( "%5s  %2d  %6.4f  %6.4f  %6.4f  %11.3f  %11.3f  %s"  )  
                 % ilabel->first
                 % (icut->first < begin_spare_int ? icut->first : 000 )
                 % central
                 % elow
                 % ehigh 
                 % den
                 % num
                 % desc )
             << endl;
      }
    }
  }
public:
  CutflowCommand() 
  : AbsCommand( "cutflow" , "dumps cutflow in numeric order" )
  , opt_suffix( this , CommandOption::STRING , "suffix" , "select data objects with this suffix" )
  , opt_notnum( this , CommandOption::BOOL , "notnum" , "do not look for a number in the data object name" )
  {}
};


class 
DumpToWebCommand : public AbsCommand
{
protected:
  virtual void spit_header( ostream& os ) {
    os << "<head></head>" << endl;
    os << "<body>" << endl;
  }
  virtual void spit_footer( ostream& os ) {
    os << "</body>" << endl;
  }
  virtual const string save_collection( const DgCollection* coll , string prefix ) {
    replace_all(prefix," ","");
    // save objects in this directory
    string htmlname( "web/index.html" );
    ofstream outf( htmlname.c_str() );
    if( !outf.is_open() ) { cout << " could not open " << htmlname << " for web output." << endl; return ""; }
    spit_header(outf);
    // save subdirectories
    for( dg::SubColl::const_iterator i = coll->subcollections().begin() , f = coll->subcollections().end(); i!=f; ++i ) {
      const DgCollection* subcoll = i->second;
      assert( subcoll->name() != "" );
      //      TDirectory* subdir = directory->mkdir( subcoll->_name , subcoll->_description );
      //      subcoll->save( subdir );
      string subhtmlname = save_collection( subcoll , prefix + subcoll->name() );
      cout << " <a href=\"" << subhtmlname << "\">subdirectory " << i->first << "</a><br>" << endl;
    }
    // save these plots
    ShowCommand::xshow( false );
    for( dg::ResourceColl::const_iterator i = coll->graphics().begin() , f = coll->graphics().end(); i!=f; ++i ) {
      clearCanvas();
      // FIXME: interface has changed
      //      OldPlotCommand::plot( i->second , "" , "" , "" , "" );
      string tmpst("");
      ShowCommand::show();
      string filename( "web/"+prefix + "_" + i->first );
      filename += ".png";
      SaveCommand::save( filename );
      filename = prefix + "_" + i->first + ".png";
      outf << "<a href=\"" << filename << "\">" << i->first << "</a><br>" << endl;
    }
    ShowCommand::xshow( true );
    spit_footer(outf);
    outf.close();
    return htmlname;
  }
  virtual void operator()( string& command ) {
    DgCollection* top_coll = dg::current();
    assert( top_coll );
    string prefix = command;
    prefix += "web";
    string outputname = save_collection( top_coll , prefix );
  }
public:
  DumpToWebCommand() : AbsCommand( "webpage" , "dump directory and all subdirectories to a web page." ) {}
};

class 
RootCintCommand : public AbsCommand
{
protected:
  virtual void operator()( string& cmd ) {
    assert(app);
    string command( cmd );
    boost::trim( command );
    if( !command.empty() ) { 
      cout << " rootcint] " << command << endl;
      app->ProcessLine( command.c_str() );
    } else {
      cout << " entering root cint mode. \".q\" to return to dgplot." << endl;
      app->RunRoot( true );
    }
  }
public:
  RootCintCommand() : AbsCommand( "rootcint" , "process CINT command line" ) {}
};

class 
PlotOperationCommand : public AbsCommand
{
protected:
  virtual void operator()( string& command ) {
//       for( vector<AbsPlot*>::const_iterator i=g_plot_stack.begin() , f=g_plot_stack.end(); i!=f; ++i ) {
//         const AbsPlot* p = *i;
//         const DgHistogram1D* h1 = dynamic_cast<const DgHistogram1D*>(p);
//         const DgHistogramWeighted1D* h1w = dynamic_cast<const DgHistogramWeighted1D*>(p);
//         app->ProcessLine( command.c_str() );
//         if( h1w ) {
//           double intw = h1w->integrate_weight();
//           cout << " " << setw(30) << p->description() << " " << setw(10) << intw << " " << setw(10) << (intw*sf) << endl;
//         } else if( h1 ) {
//           DataGraphics::Count intw = h1->integrate();
//           cout << " " << setw(30) << p->description() << " " << setw(10) << intw << " " << setw(10) << (intw*sf) << endl;
//         }
  }
public:
  PlotOperationCommand() : AbsCommand( "operate" , "do something to each histogram/graph" ) {}
};

class 
DescriptionCommand : public AbsCommand
{
public:
  CommandOption opt_list;
public:
  static void list_descriptions() {
    cout << " list of sample descriptions: " << endl;
    BOOST_FOREACH( const DescColl::value_type& v , g_descriptions ) {
      v.second.desc->print( cout );
      cout << " path: " << v.second.path << endl;
      cout << " ptr: " << v.second.coll << endl;
    }
  }
protected:
  virtual void operator()( string& command ) {
    if( opt_list.value_bool() ) { list_descriptions(); }
  }
public:
  DescriptionCommand() 
    : opt_list( this , CommandOption::BOOL , "list" , "list loaded descriptions" )
    , AbsCommand( "desc" , "manipulate sample descriptions" ) {}
};


class 
ExampleCommand : public AbsCommand
{
protected:
  virtual void operator()( string& command ) {
  }
public:
  ExampleCommand() : AbsCommand( "example" , "does nothing. example command" ) {}
};

class 
DebugCommand : public AbsCommand
{
public:
  CommandOption opt_set;
protected:
  virtual void operator()( string& command ) {
    g_debug_mode = opt_set.value_bool();
    cout << " global debug mode is " << (g_debug_mode ? "ON" : "off") << endl;
  }
public:
  DebugCommand() 
    : opt_set( this , CommandOption::BOOL , "set" , "set/unset global debug mode" )
    , AbsCommand( "dbg" , "debug command" ) {}
};

class 
TerminalCommand : public AbsCommand
{
public:
  CommandOption opt_height;
protected:
  virtual void operator()( string& command ) {
    if( opt_height.found() ) {
      g_terminal_height = opt_height.value_int();
    }
    if( g_debug_mode || opt_debug.value_bool() ) {
      cout << " terminal parameters: " 
           << " height: " << g_terminal_height
           << endl;
    }
  }
public:
  TerminalCommand() 
    : opt_height( this , CommandOption::INTEGER , "height" , "set terminal height (in lines)" )
    , AbsCommand( "terminal" , "terminal settings command" ) {}
};

class 
ResourceMgrCommand : public AbsCommand
{
public:
  // CommandOption opt_height;
protected:protected:
  virtual void operator()( string& command ) {
    DgResourceMgr::instance()->print( cout );
  }
public:
  ResourceMgrCommand() 
    : AbsCommand( "resource" , "display current resource information" ) {}
};

class 
SvgCommand : public AbsCommand
{
// public:
//   typedef enum { SINGLE_HISTOGRAM=0 } PlotTemplate;
// protected:
//   double _minx;
//   double _maxx;
//   double _miny;
//   double _maxy;
//   double _minz;
//   double _maxz;
//   bool _logx;
//   bool _logy;
//   bool _logz;
//   vector<double> _xticks;
//   vector<double> _yticks;
// protected:
//   const double pad_x( const double& x ) const { return x/std::abs(_maxx-_minx); }
//   const double pad_y( const double& y ) const { return y/(std::abs(_maxy-_miny)/g_aspect_ratio); }
//   void set_h1d_style( BoardLib::Board& board , const unsigned int item = 0 ) {
//     if( item == 0 ) {
//       board.setPenColorRGBf( 0. , 0. , 0. );
//       board.setLineWidth( 0.1 );
//       board.setFillColorRGBf( 0. , 0. , 0. );
//     }
//   }
//   void set_axis_style( BoardLib::Board& board ) {
//     board.setPenColorRGBf( 0.25 , 0.25 , 0.25 ); 
//     board.setLineWidth( 0.5 );
//     board.setFont( "Gotham-Book" , 14. );
//   }
//   void draw_x_axis( BoardLib::Board& board ) {
//     set_axis_style( board );
//     board.drawLine( pad_x(_minx) , pad_y(0) , pad_x(_maxx) , pad_y(0) );
//     double tickheight = 0.05;
//     double labeloffset = 0.08;
//     vector<double> ticks( _xticks );
//     if( _xticks.empty() ) {
//       // default is 3 uniformly-spaced ticks
//       for( unsigned int itick=0; itick!=3; ++itick ) {
//         double xt = _minx + (itick+1)*(1./4.)*std::abs(_maxx-_minx);
//         ticks.push_back( xt );
//       }
//     }
//     for( unsigned int itick=0, ftick=ticks.size(); itick!=ftick; ++itick ) {
//       double xt = ticks[itick];
//       board.drawLine( pad_x(xt) , pad_y(0) , pad_x(xt) , pad_y(0) -tickheight );
//       string labeltext = "";
//       labeltext += xt;
//       board.drawText( pad_x(xt) , pad_y(0) - labeloffset , labeltext.Data() );
//     }
//   }
//   void draw_y_axis( BoardLib::Board& board ) {
//     set_axis_style( board );
//     board.drawLine( pad_x(0) , pad_y(_miny) , pad_x(0) , pad_y(_maxy) );
//     double tickheight = 0.05/g_aspect_ratio;
//     double labeloffset = 0.08/g_aspect_ratio;
//     vector<double> ticks( _yticks );
//     if( _yticks.empty() ) {
//       // default is 3 uniformly-spaced ticks
//       for( unsigned int itick=0; itick!=3; ++itick ) {
//         double yt = _miny + (itick+1)*(1./4.)*std::abs(_maxy-_miny);
//         ticks.push_back( yt );
//       }
//     }
//     for( unsigned int itick=0, ftick=ticks.size(); itick!=ftick; ++itick ) {
//       double yt = ticks[itick];
//       board.drawLine( pad_x(0) , pad_y(yt) , pad_x(0) - tickheight , pad_y(yt) );
//       string labeltext = "";
//       labeltext += yt;
//       board.drawText( pad_x(0) - labeloffset , pad_y(yt) , labeltext.Data() );
//     }
//   }
//   void draw_histogram_1d( const DgHistogram1D& h , BoardLib::Board& board ) const {
//     bool close_polygon = false;
//     vector<BoardLib::Point> points_edge;
//     vector<BoardLib::Point> points_fill;
//     for( BinSize ibin=0,fbin=h.nbins(); ibin!=fbin; ++ibin ) {
//       Count bincount = h.bin_count(ibin);
//       if( bincount > 0 ) {
//         if( !close_polygon ) { 
//           if( ibin!=0 || h.underflow()<1 ) {
//             points_edge.push_back( BoardLib::Point(pad_x(h.bin_min(ibin)),pad_y(0)) );
//           }
//         }
//         points_edge.push_back( BoardLib::Point(pad_x(h.bin_min(ibin)),pad_y(bincount)) );
//         points_edge.push_back( BoardLib::Point(pad_x(h.bin_max(ibin)),pad_y(bincount)) );
//         close_polygon = true;
//         points_fill.push_back( BoardLib::Point(pad_x(h.bin_min(ibin)),pad_y(0)) );
//         points_fill.push_back( BoardLib::Point(pad_x(h.bin_min(ibin)),pad_y(bincount)) );
//         points_fill.push_back( BoardLib::Point(pad_x(h.bin_max(ibin)),pad_y(bincount)) );
//         points_fill.push_back( BoardLib::Point(pad_x(h.bin_max(ibin)),pad_y(0)) );
//       }
//       if( bincount==0 && close_polygon ) {
//         points_edge.push_back( BoardLib::Point(pad_x(h.bin_min(ibin)),pad_y(0)) );
//       } else if( close_polygon && ibin==(fbin-1) ) {
//         if( h.overflow() < 1 ) {
//           points_edge.push_back( BoardLib::Point(pad_x(h.bin_max(ibin)),pad_y(0)) );
//         }
//       }
//     }
//     board.drawClosedPolyline( points_edge );
//   }
//   virtual void operator()( string& command ) {
//     // make a plot based on various templates. pick data graphics for templates from current plot stack.
//     PlotTemplate plate( SINGLE_HISTOGRAM );
//     if( command.Contains( "singleh" ) ) { plate = SINGLE_HISTOGRAM; }
//     // coordinate system variables
//     _minx = 0.;
//     _maxx = 1.;
//     _miny = 0.;
//     _maxy = 1.;
//     _minz = 0.;
//     _maxz = 1.;
//     _logx = false;
//     _logy = false;
//     _logz = false;
//     // template slots
//     vector<AbsPlot*> template_slots;
//     unsigned int nslots = 0;
//     if( plate==SINGLE_HISTOGRAM ) { nslots = 1; }
//     else { cout << " template type not implemented." << __LINE__ << endl; }
//     // get plots for each slot
//     vector<AbsPlot*> tmpstack;
//     for( vector<AbsPlot*>::const_iterator istack=g_plot_stack.begin(), fstack=g_plot_stack.end(); istack!=fstack; ++istack ) {
//       if( !(*istack) ) { continue; }
//       if( !((*istack)->isData()) ) { continue; }
//       tmpstack.push_back( *istack );
//     }
//     for( unsigned int islot=0; islot!=nslots; ++islot ) {
//       cout << " slot " << (islot+1) << " / " << nslots << endl;
//       unsigned int nstack = 1;
//       for( vector<AbsPlot*>::const_iterator istack=tmpstack.begin(), fstack=tmpstack.end(); istack!=fstack; ++istack, ++nstack ) {
//         cout << nstack << " " << (*istack)->description() << endl;
//       }
//       int iplot = askInteger( "data for slot (negative to cancel)? " );
//       if( iplot<0 ) { return; }
//       if( iplot<1 || iplot>tmpstack.size() ) { continue; }
//       template_slots.push_back( tmpstack[iplot-1] );
//     }
//     assert( template_slots.size() == nslots );
//     BoardLib::Board board;
//     _xticks.clear();
//     _yticks.clear();
//     if( plate==SINGLE_HISTOGRAM ) { 
//   // single histogram template
//       const DgHistogram1D* h1d = dynamic_cast<const DgHistogram1D*>( template_slots.front() );
//       if( h1d ) {
//         _minx = h1d->bin_min( 0 );
//         _maxx = h1d->bin_max( h1d->nbins()-1 );
//         _miny = 0;
//         _maxy = h1d->maximum_count();
//         draw_x_axis( board );
//         draw_y_axis( board );
//         set_h1d_style( board , 0 );
//         draw_histogram_1d( *h1d , board );
//       }
//     }
//     board.saveEPS( "testboard2.eps" );
//     board.saveSVG( "testboard2.svg" );
// //     BoardLib::Board board;
// //     board.setPenColorRGBf( 0 , 0 , 0 );
// //     board.setLineWidth( 0.5 );
// //     board.drawRectangle( 0 , 0 , 1000 , -500 );
// //     vector<BoardLib::Point> points;
// //     board.setPenColorRGBf( 0.25 , 0.25 , 0.25 ); 
// //     points.push_back( BoardLib::Point(0,0) );
// //     points.push_back( BoardLib::Point(100,0) );
// //     points.push_back( BoardLib::Point(100,23) );
// //     points.push_back( BoardLib::Point(150,23) );
// //     points.push_back( BoardLib::Point(150,80) );
// //     points.push_back( BoardLib::Point(250,80) );
// //     points.push_back( BoardLib::Point(250,190) );
// //     points.push_back( BoardLib::Point(350,190) );
// //     points.push_back( BoardLib::Point(350,342) );
// //     points.push_back( BoardLib::Point(450,342) );
// //     points.push_back( BoardLib::Point(450,210) );
// //     points.push_back( BoardLib::Point(550,210) );
// //     points.push_back( BoardLib::Point(550,0) );
// // //     points.push_back( BoardLib::Point(100,0) )
// //     board.setLineWidth( 2. );
// //     board.drawClosedPolyline( points );
// //     board.saveEPS( "testboard.eps" );
// //     board.saveSVG( "testboard.svg" );
//   }
public:
  SvgCommand() : AbsCommand( "svg" , "writes svg output (unimplemented)" ) {}
  virtual void operator()( string& command ) {}
};


class 
GraphToHist2DCommand : public AbsCommand
{
public:
  CommandOption opt_xrange;
  CommandOption opt_yrange;
  CommandOption opt_autorange;
  CommandOption opt_nbinsx;
  CommandOption opt_nbinsy;
  CommandOption opt_noshow;
protected:
  virtual void operator()( string& command ) {
    vector< shared_ptr<AbsPlot> >::const_iterator i = std::find_if( g_plot_stack.begin() , g_plot_stack.end() , bind(&AbsPlot::isFirstGraphic,_1)==true );
    if( i == g_plot_stack.end() ) { return; }
    shared_ptr<const PlotGraph<DgGraph2D> > hbw = dynamic_pointer_cast<const PlotGraph<DgGraph2D> >( *i );
    if( !hbw ) { return; }
    unsigned int nbinsx = 20;
    unsigned int nbinsy = 20;
    double minx=999999;
    double miny=999999;
    double maxx=-999999;
    double maxy=-999999;
    bool autorange = opt_autorange.value_bool();
    if( !autorange && (opt_xrange.value_bool() || opt_yrange.value_bool()) ) {
      // get x and y ranges
      if( opt_xrange.value_bool() ) {
        minx = opt_xrange.value_range_begin();
        maxx = opt_xrange.value_range_end();
      }
      if( opt_yrange.value_bool() ) { 
        miny = opt_yrange.value_range_begin();
        maxy = opt_yrange.value_range_end();
      }
    } else if( autorange ) {
      for( DgGraph2D::const_iterator i=hbw->begin(), f=hbw->end(); i!=f; ++i ) {
        double x = (*i)._x;
        double y = (*i)._y;
        minx = std::min( x , minx );
        miny = std::min( y , miny );
        maxx = std::max( x , maxx );
        maxy = std::max( y , maxy );
      }
    }
    if( opt_nbinsx.found() ) { nbinsx = opt_nbinsx.value_int(); }
    if( opt_nbinsy.found() ) { nbinsy = opt_nbinsy.value_int(); }
    TH2D* h = new TH2D( "tmphist" , "tmphist" , nbinsx , minx , maxx , nbinsy , miny , maxy );
    for( DgGraph2D::const_iterator i=hbw->begin(), f=hbw->end(); i!=f; ++i ) {
      double x = (*i)._x;
      double y = (*i)._y;
      h->Fill( x , y );
    }
    PlotStyleCommand::default_format_2d( h , kBlack );
    h->Draw("colz");
    if( ! opt_noshow.value_bool() ) { 
      ShowCommand::update_canvas();
    }
  }
public:
  GraphToHist2DCommand() 
    : opt_xrange( this , CommandOption::RANGE , "xrange" , "select x range" )
    , opt_yrange( this , CommandOption::RANGE , "yrange" , "select y range" )
    , opt_autorange( this , CommandOption::BOOL , "autorange" , "select range automatically" )
    , opt_nbinsx( this , CommandOption::INTEGER , "nbinsx" , "number of x bins" )
    , opt_nbinsy( this , CommandOption::INTEGER , "nbinsy" , "number of y bins" )
    , opt_noshow( this , CommandOption::BOOL , "noshow" , "add to stack; don't show yet" )
    , AbsCommand( "graphtohist2d" , "convert graph to 2D histogram" ) {}
};


class 
OverlayModeCommand : public AbsCommand
{
public:
  CommandOption opt_mode;
protected:
  virtual void operator()( string& command ) {
    if( opt_mode.found() ) {
      if( istarts_with( opt_mode.value_string() , "def" ) ) {
        g_overlay_mode = DEFAULT_OVERLAY;
      } else if( istarts_with( opt_mode.value_string() , "datamc" ) ) {
        g_overlay_mode = DATAMC_OVERLAY;
      } else if( istarts_with( opt_mode.value_string() , "comp" ) ) {
        g_overlay_mode = COMPARISON_OVERLAY;
      } else if( istarts_with( opt_mode.value_string() , "shape" ) ) {
        g_overlay_mode = SHAPE_COMPARISON_OVERLAY;
      }
    }
  }
public:
  OverlayModeCommand() 
    :  opt_mode( this , CommandOption::STRING , "mode" , "select overlay mode" )
    , AbsCommand( "overlaymode" , "set plot overlay mode." ) {}
};

class 
RangeModeCommand : public AbsCommand
{
protected:
  virtual void operator()( string& command ) {
    if( icontains( command , "auto" ) ) {
      g_range_mode = AUTO_RANGE;
    } else if( icontains( command , "fixed" ) ) {
      g_range_mode = FIXED_RANGE;
    }
  }
public:
  RangeModeCommand() : AbsCommand( "rangemode" , "set plot range mode." ) {}
};

class 
UpdateCanvasCommand : public AbsCommand
{
protected:
  virtual void operator()( string& command ) {
    ShowCommand::update_canvas();
  }
public:
  UpdateCanvasCommand() : AbsCommand( "update" , "updates the current canvas" ) {}
};

class 
PoissonSignificanceCommand : public AbsCommand
{
protected:
  virtual void operator()( string& command ) {
    double nbkg = askFloat( "background prediction: " );
    double nobs = askInteger( "observed number of events: " );
    double prob = TMath::Gamma( nobs , nbkg );
    double sig = sqrt(2.)*TMath::ErfInverse(1.-prob);
    cout << " Chance of Poisson fluctuate from mean " << nbkg << " to at least " << nobs << " is " << prob << " or " << sig << " sigma." << endl;
  }
public:
  PoissonSignificanceCommand() : AbsCommand( "poissig" , "calculate the probability to observe at least n events given background m." ) {}
};

class 
RooMassFitZmumuCommand : public AbsCommand
{
public:
  CommandOption opt_nobkg;
  CommandOption opt_hasbkg;
  CommandOption opt_81101;
  CommandOption opt_66116;
  CommandOption opt_save;
protected:
  static bool _fix_zmass;
  static bool _zero_background;
  static double _fitted_zmass;
  static double _fitted_resolution;
  static double _fitted_signal;
  static double _fitted_background;
  static double _fitted_zmass_error;
  static double _fitted_resolution_error;
  static double _fitted_signal_error;
  static double _fitted_background_error;
  static double _mass_min;
  static double _mass_max;
public:
  static void float_zmass() { _fix_zmass = false; }
  static void fix_zmass() { _fix_zmass = true; }
  static void float_background() { _zero_background = false; }
  static void zero_background() { _zero_background = true; }
  static void standard_zwindow() { _mass_min = 66.; _mass_max = 116.; }
  static void trigger_zwindow() { _mass_min = 81.; _mass_max = 101.; }
  static const double fitted_signal() { return _fitted_signal; }
  static double fitted_background() { return _fitted_background; }
  static double fitted_resolution() { return _fitted_resolution; }
  static const double fitted_zmass() { return _fitted_zmass; }
  static const double fitted_signal_error() { return _fitted_signal_error; }
  static double fitted_background_error() { return _fitted_background_error; }
  static double fitted_resolution_error() { return _fitted_resolution_error; }
  static const double fitted_zmass_error() { return _fitted_zmass_error; }
  static const bool fit_zmass( const DgHistogram1D* h1d , const bool dodraw = true ) {
    TH1* h = dynamic_cast<TH1*>( h1d->new_ROOT( "tmproohist" ) );
    fit_zmass( h , dodraw );
    delete h;
  }
  static const bool fit_zmass( const TH1* h , const bool dodraw = true ) {
    // setup pdf
    const double zMeanMin = _fix_zmass ? 89.1876 : 81.1876;
    const double zMeanMax = _fix_zmass ? 93.1876 : 101.1876;
    RooRealVar mass( "dimuon_mass" , "dimuon mass" , _mass_min , _mass_max , "GeV" );
    RooDataHist data( "dimuonData" , "dimuon track mass spectrum" , mass , h );
    RooRealVar zMean( "zMean" , "signal PDF mean" , 91.1876 , zMeanMin , zMeanMax ); // mass floats to allow for detector-induced mass shift.
    RooRealVar zWidth( "zWidth" , "signal PDF width" , 2.4952 );
    RooRealVar zAlpha( "trackAlpha" , "track signal crystal ball tail parameter" , 1. , 0.2 , 20. );
    RooRealVar zOrder( "trackOrder" , "track signal crystal ball order parameter" , 2.8 , 0.75 , 5. );
    RooRealVar zSigma( "trackSigma" , "track signal resolution parameter" , 2.5 , 0.75 , 20 , "GeV" );
//     RooCBShape sigPdf( "sigPdf" , "track signal PDF" , mass , zMean , zSigma , zAlpha , zOrder );
    RooVoigtian sigPdf( "sigPdf" , "track signal PDF" , mass , zMean , zWidth , zSigma );
    RooRealVar tau( "tau" , "track background exponential decay constant" , 0. , -10. , 0., "GeV" );
    RooExponential bkgPdf( "bkgPdf" , "track background PDF" , mass , tau );
    RooRealVar nbkg( "nbkg" , "total number of background events" , 1. , 0. , h->Integral() );
    RooRealVar nsig( "nsig" , "total number of signal events" , h->Integral() , 0. , h->Integral() );
    RooAddPdf model( "model" , "combined signal and background PDF", _zero_background ? RooArgList(sigPdf) : RooArgList(sigPdf,bkgPdf) , _zero_background ? RooArgList(nsig) : RooArgList(nsig,nbkg) );
//     if( _zero_background ) {
//       nbkg.setVal(0);
//       nbkg.setRange(0.,0.);
//     }
    // fit
    RooFitResult* result = model.fitTo( data , RooFit::Save(true) ,
                                        RooFit::InitialHesse(kTRUE) , RooFit::Hesse(kTRUE) , 
                                        RooFit::Minos(kTRUE) , RooFit::Extended(kTRUE) );
    const RooArgSet* model_parms = dynamic_cast<RooArgSet*>( model.getVariables()->snapshot() ); assert( model_parms );
    if( dodraw ) {
      RooPlot* frame = mass.frame();
      frame->SetMarkerColor( kBlack );
      frame->SetMaximum( 93. );
      gStyle->SetMarkerColor( kBlack );
      data.plotOn( frame , RooFit::MarkerColor(kBlack) );
      model.plotOn( frame , RooFit::LineColor(12) );
      if( !_zero_background ) {
        model.plotOn( frame , RooFit::Components("sigPdf") , RooFit::LineColor(kBlue) );
        model.plotOn( frame , RooFit::Components("bkgPdf") , RooFit::LineColor(kRed) );
      }
      frame->Draw();
      model_parms->Print("v");
      ShowCommand::update_canvas();
    }
    //cerr << " warning: this routine leaks like a sieve" << endl;
    RooArgList final_pars( result->floatParsFinal() );
    final_pars.Print("v");
    RooRealVar* fit_bkg = _zero_background ? 0 : (RooRealVar*)final_pars.at( 0 );
    RooRealVar* fit_sig = (RooRealVar*)final_pars.at( _zero_background ? 0 : 1 );
    RooRealVar* fit_tau = _zero_background ? 0 : (RooRealVar*)final_pars.at( 2 );
    RooRealVar* fit_sigma = (RooRealVar*)final_pars.at( _zero_background ? 1 : 3 );
    RooRealVar* fit_mean = (RooRealVar*)final_pars.at( _zero_background ? 2 : 4 );
    if( _zero_background ) {
      _fitted_signal = fit_sig->getVal();
      _fitted_signal_error = sqrt( _fitted_signal );
    } else {
      _fitted_signal = fit_sig->getVal();
      _fitted_signal_error = fit_sig->getError();
    }
    if( !_zero_background ) {
      _fitted_background = fit_bkg->getVal();
      _fitted_background_error = fit_bkg->getError();
    }
    _fitted_zmass = fit_mean->getVal();
    _fitted_zmass_error = fit_mean->getError();
    _fitted_resolution = fit_sigma->getVal();
    _fitted_resolution_error = fit_sigma->getError();
    // central oi cotst, v7: 
    /* 
  2) RooRealVar::0x963c520::nbkg        :  371.32 +/-  (-36.670, 37.661) L(0 - 1e+09) 
  3) RooRealVar::0x9649228::nsig        :  4827.7 +/-  (-76.084, 76.718) L(0 - 1e+09) 
  4) RooRealVar::0x96494d0::tau         : -0.0232618 +/-  (-0.0057732, 0.0053192) L(-10 - 10) // [GeV]
  5) RooRealVar::0x95f57a0::trackSigma  :  2.0054 +/-  (-0.058495, 0.059042) L(0.1 - 20) // [GeV]
  6) RooRealVar::0x961b120::zMean       :  90.783 +/-  (-0.048963, 0.048975) L(89.1876 - 93.1876) 
  7) RooRealVar::0x961acc0::zWidth      :  2.4952 C L(-INF - +INF) 
     */
    // bwio, v7
    /*
  2) RooRealVar::0x968a098::nbkg        :  328.16 +/-  (-40.670, 41.273) L(0 - 1e+09) 
  3) RooRealVar::0x9688bf8::nsig        :  976.86 +/-  (-47.742, 48.709) L(0 - 1e+09) 
  4) RooRealVar::0x968a488::tau         : -0.0230788 +/-  (-0.0063586, 0.0056255) L(-10 - 10) // [GeV]
  5) RooRealVar::0x9689a40::trackSigma  :  5.6255 +/-  (-0.31776, 0.33020) L(0.1 - 20) // [GeV]
  6) RooRealVar::0x9688798::zMean       :  91.153 +/-  (-0.27894, 0.27914) L(89.1876 - 93.1876) 
  7) RooRealVar::0x968aa28::zWidth      :  2.4952 C L(-INF - +INF) 
     */

    // central oi cotst, v6:
    /*
   1  nbkg         1.07811e+01   4.44419e+00  -4.10486e+00   4.79418e+00
                                 WARNING -   - ABOVE PARAMETER IS AT LIMIT.
   2  nsig         6.17627e+03   7.86464e+01  -7.83439e+01   7.89460e+01
   3  tau         -3.99559e+00   7.71382e+00  -1.60209e-01   4.35713e+00
   4  trackSigma   2.14120e+00   5.41756e-02  -5.37736e-02   5.45968e-02
   5  zMean        9.07988e+01   4.48308e-02  -4.48948e-02   4.47991e-02
     */
    
    // forward io, v6:
    /*
  2) RooRealVar::0x8fe6d58::nbkg        :  271.07 +/-  (-37.421, 38.259) L(0 - 1e+09) 
  3) RooRealVar::0x8fe5890::nsig        :  954.93 +/-  (-45.539, 46.441) L(0 - 1e+09) 
  4) RooRealVar::0x8fe7128::tau         : -0.00795951 +/-  (-0.0057904, 0.0055602) L(-10 - 10) // [GeV]
  5) RooRealVar::0x8fe6658::trackSigma  :  5.7903 +/-  (-0.32015, 0.33540) L(0.1 - 20) // [GeV]
  6) RooRealVar::0x8fee878::zMean       :  90.608 +/-  (-0.28882, 0.28910) L(89.1876 - 93.1876) 
  7) RooRealVar::0x8fe5388::zWidth      :  2.4952 C L(-INF - +INF) 
     */
    return true;
  }
protected:
  virtual void operator()( string& command ) {
    vector< shared_ptr<AbsPlot> >::const_iterator i = std::find_if( g_plot_stack.begin() , g_plot_stack.end() , bind(&AbsPlot::isFirstGraphic,_1)==true );
    if( i == g_plot_stack.end() ) { 
      if( opt_debug.value_bool() ) { cout << " nothing in plot stack." << endl; }
      return; 
    }
    shared_ptr< const PlotHistogram1D<DgHistogram1D> > hbw = dynamic_pointer_cast<const PlotHistogram1D<DgHistogram1D> >( *i );
    if( !hbw ) { 
      if( opt_debug.value_bool() ) { cout << " current plot is not a 1D histogram." << endl; }
      return; 
    }
    TH1* h = dynamic_cast<TH1*>( hbw->new_ROOT( "tmproohist" ) );
    if( !h ) { 
      if( opt_debug.value_bool() ) { cout << " no ROOT object from plot." << endl; }
      return; 
    }
    RooMassFitZmumuCommand::float_background();
    RooMassFitZmumuCommand::standard_zwindow();
    if( opt_nobkg.value_bool() ) { RooMassFitZmumuCommand::zero_background(); }
    if( opt_hasbkg.value_bool() ) { RooMassFitZmumuCommand::float_background(); }
    if( opt_81101.value_bool() ) { RooMassFitZmumuCommand::trigger_zwindow(); }
    if( opt_66116.value_bool() ) { RooMassFitZmumuCommand::standard_zwindow(); }
    fit_zmass(h,true);
    if( opt_save.value_bool() ) {
      string filename( (*i)->description() );
      replace_all(filename, "/" , "_" );
      if( istarts_with(filename,"_") ) { erase_head(filename,1); }
      string plotname = filename; plotname += ".gif";
      SaveCommand::save( plotname );
      string statname = filename; statname += ".txt";
      ofstream ofile( statname.c_str() , ios::out );
      ofile << filename << " "
            << "zmass " << _fitted_zmass << " " << _fitted_zmass_error << " "
            << "zres " << _fitted_resolution << " " << _fitted_resolution_error << " "
            << "nsig " << _fitted_signal << " " << _fitted_signal_error << " "
            << "nbkg " << _fitted_background << " " << _fitted_background_error << " "
            << endl;
      ofile.close();
      cout << " saved fit output to " << endl << "  " << plotname << endl << "  " << statname << endl;
    }
  }
public:
  RooMassFitZmumuCommand() 
    : opt_nobkg( this , CommandOption::BOOL , "nobkg" , "zero background" )
    , opt_hasbkg( this , CommandOption::BOOL , "hasbkg" , "float background" )
    , opt_81101( this , CommandOption::BOOL , "81101" , "fit in trigger 81-101 window" )
    , opt_66116( this , CommandOption::BOOL , "66116" , "fit in standard 66-116 window" )
    , opt_save( this , CommandOption::BOOL , "save" , "save the fit to files" )
    , AbsCommand( "fitzmumu" , "RooFit the plotted histogram using a Z to mumu PDF." )
  {}
};

bool RooMassFitZmumuCommand::_fix_zmass = false;
bool RooMassFitZmumuCommand::_zero_background = false;
double RooMassFitZmumuCommand::_fitted_zmass = 0.;
double RooMassFitZmumuCommand::_fitted_resolution = 0.;
double RooMassFitZmumuCommand::_fitted_signal = 0.;
double RooMassFitZmumuCommand::_fitted_background = 0.;
double RooMassFitZmumuCommand::_fitted_zmass_error = 0.;
double RooMassFitZmumuCommand::_fitted_resolution_error = 0.;
double RooMassFitZmumuCommand::_fitted_signal_error = 0.;
double RooMassFitZmumuCommand::_fitted_background_error = 0.;
double RooMassFitZmumuCommand::_mass_min = 66.;
double RooMassFitZmumuCommand::_mass_max = 116.;

class 
RooMassFitZeeCommand : public AbsCommand
{
public:
  CommandOption opt_nobkg;
  CommandOption opt_hasbkg;
  CommandOption opt_81101;
  CommandOption opt_66116;
  CommandOption opt_caltrk;
  CommandOption opt_track;
  CommandOption opt_calo;
  CommandOption opt_bw;
  CommandOption opt_save;
public:
  typedef enum { FIT_CAL=0 , FIT_TRK=1 , FIT_BW=2 , FIT_CALTRK=3 } FitMode;
protected:
  static FitMode _mode;
  static bool _fix_zmass;
  static bool _zero_background;
  static double _fitted_zmass;
  static double _fitted_resolution;
  static double _fitted_signal;
  static double _fitted_background;
  static double _fitted_zmass_error;
  static double _fitted_resolution_error;
  static double _fitted_signal_error;
  static double _fitted_background_error;
  static double _mass_min;
  static double _mass_max;
public:
  static void cal_fit() { _mode = FIT_CAL; }
  static void track_fit() { _mode = FIT_TRK; }
  static void bw_fit() { _mode = FIT_BW; }
  static void caltrk_fit() { _mode = FIT_CALTRK; }
  static void float_zmass() { _fix_zmass = false; }
  static void fix_zmass() { _fix_zmass = true; }
  static void float_background() { _zero_background = false; }
  static void zero_background() { _zero_background = true; }
  static void standard_zwindow() { _mass_min = 66.; _mass_max = 116.; }
  static void trigger_zwindow() { _mass_min = 81.; _mass_max = 101.; }
  static const double fitted_signal() { return _fitted_signal; }
  static double fitted_background() { return _fitted_background; }
  static double fitted_resolution() { return _fitted_resolution; }
  static const double fitted_zmass() { return _fitted_zmass; }
  static const double fitted_signal_error() { return _fitted_signal_error; }
  static double fitted_background_error() { return _fitted_background_error; }
  static double fitted_resolution_error() { return _fitted_resolution_error; }
  static const double fitted_zmass_error() { return _fitted_zmass_error; }
  static const bool fit_zmass( const DgHistogram1D* h1d , const bool dodraw = true ) {
    TH1* h = dynamic_cast<TH1*>( h1d->new_ROOT( "tmproohist" ) );
    fit_zmass( h , dodraw );
    delete h;
  }
  static const bool fit_zmass( const TH1* h , const bool dodraw = true ) {
    // setup pdf
    const double zMeanMin = _fix_zmass ? 89.1876 : 81.1876;
    const double zMeanMax = _fix_zmass ? 93.1876 : 101.1876;
    RooRealVar mass( "dimuon_mass" , "dimuon mass" , _mass_min , _mass_max , "GeV" );
    RooDataHist data( "dimuonData" , "dimuon track mass spectrum" , mass , h );
    RooRealVar zMean( "zMean" , "signal PDF mean" , 91.1876 , zMeanMin , zMeanMax ); // mass floats to allow for detector-induced mass shift.
    RooRealVar zWidth( "zWidth" , "signal PDF width" , 2.4952 );
    RooRealVar zWidthFloat( "zWidthFloat" , "floating signal PDF width" , 2.4952 , 1.5 , 5. );
    RooRealVar zAlpha( "trackAlpha" , "track signal crystal ball tail parameter" , 1. , 0.2 , 20. );
    RooRealVar zOrder( "trackOrder" , "track signal crystal ball order parameter" , 2.8 , 0.75 , 8. );
    RooRealVar zSigmaCal( "calSigma" , "calorimeter signal resolution parameter" , 2.5 , 1.1 , 20 , "GeV" ); // minimum resolution must be ~ bin width to avoid spiky solutions
    RooRealVar zSigmaTrk( "trackSigma" , "track signal resolution parameter" , 2.5 , 1.1 , 20 , "GeV" );
    RooVoigtian sigCalPdf( "sigCalPdf" , "calorimeter signal PDF" , mass , zMean , zWidth , zSigmaCal );
    RooCBShape sigTrkPdf( "sigTrkPdf" , "track signal PDF" , mass , zMean , zSigmaTrk , zAlpha , zOrder );
    RooVoigtian sigBwPdf( "sigBwPdf" , "bw signal PDF" , mass , zMean , zWidthFloat , zSigmaCal );
    RooRealVar fCalSig( "fCalSig" , "fraction of calorimeter signal events" , 1 , 0 , 1. );
    RooAddPdf sigCalTrkPdf( "sigCalTrkPdf" , "calorimeter + signal PDF" , sigCalPdf , sigTrkPdf , fCalSig );
    RooAbsPdf* sigPdf = 0;
    if( _mode==FIT_CAL ) { sigPdf = (RooAbsPdf*)&sigCalPdf; }
    else if( _mode==FIT_TRK ) { sigPdf = (RooAbsPdf*)&sigTrkPdf; }
    else if( _mode==FIT_BW ) { sigPdf = (RooAbsPdf*)&sigBwPdf; }
    else if( _mode==FIT_CALTRK ) { sigPdf = (RooAbsPdf*)&sigCalTrkPdf; }
    else { return false; }
    RooRealVar tau( "tau" , "track background exponential decay constant" , -0.000000001 , -10. , 0., "GeV" );
    RooExponential bkgPdf( "bkgPdf" , "track background PDF" , mass , tau );
    RooRealVar nbkg( "nbkg" , "total number of background events" , 1. , 0. , h->Integral() );
    RooRealVar nsig( "nsig" , "total number of signal events" , h->Integral() , 0. , h->Integral() );
    RooAddPdf model( "model" , "combined signal and background PDF", _zero_background ? RooArgList(*sigPdf) : RooArgList(*sigPdf,bkgPdf) , _zero_background ? RooArgList(nsig) : RooArgList(nsig,nbkg) );
    // fit
    RooFitResult* result = model.fitTo( data , RooFit::Save(true) ,
                                        RooFit::InitialHesse(kTRUE) , RooFit::Hesse(kTRUE) , 
                                        RooFit::Minos(kTRUE) , RooFit::Extended(kTRUE) );
    const RooArgSet* model_parms = dynamic_cast<RooArgSet*>( model.getVariables()->snapshot() ); assert( model_parms );
    if( dodraw ) {
      RooPlot* frame = mass.frame();
      frame->SetMarkerColor( kBlack );
      frame->SetMaximum( 93. );
      gStyle->SetMarkerColor( kBlack );
      data.plotOn( frame , RooFit::MarkerColor(kBlack) );
      model.plotOn( frame , RooFit::LineColor(12) );
      if( !_zero_background ) {
        model.plotOn( frame , RooFit::Components(sigPdf->GetName()) , RooFit::LineColor(kBlue) );
        model.plotOn( frame , RooFit::Components("bkgPdf") , RooFit::LineColor(kRed) );
      }
      frame->Draw();
      model_parms->Print("v");
      ShowCommand::update_canvas();
    }
    //cerr << " warning: this routine leaks like a sieve" << endl;
    RooArgList final_pars( result->floatParsFinal() );
    final_pars.Print("v");
//     RooRealVar* fit_bkg = _zero_background ? 0 : (RooRealVar*)final_pars.at( 0 );
//     RooRealVar* fit_sig = (RooRealVar*)final_pars.at( _zero_background ? 0 : 1 );
//     RooRealVar* fit_tau = _zero_background ? 0 : (RooRealVar*)final_pars.at( 2 );
//     RooRealVar* fit_sigma = (RooRealVar*)final_pars.at( _zero_background ? 1 : 3 );
//     RooRealVar* fit_mean = (RooRealVar*)final_pars.at( _zero_background ? 2 : 4 );
    RooRealVar* fit_bkg=0;
    RooRealVar* fit_sig=0;
    RooRealVar* fit_tau=0;
    RooRealVar* fit_sigma=0;
    RooRealVar* fit_mean=0;
    if( _mode==FIT_TRK && !_zero_background ) {
      fit_bkg = (RooRealVar*)final_pars.at( 0 );
      fit_sig = (RooRealVar*)final_pars.at( 1 );
      fit_tau = (RooRealVar*)final_pars.at( 2 );
      fit_sigma = (RooRealVar*)final_pars.at( 5 );
      fit_mean = (RooRealVar*)final_pars.at( 6 );
    } else if( _mode==FIT_TRK && _zero_background ) {
      fit_bkg = 0;
      fit_sig = (RooRealVar*)final_pars.at( 0 );
      fit_tau = 0;
      fit_sigma = (RooRealVar*)final_pars.at( 3 );
      fit_mean = (RooRealVar*)final_pars.at( 4 );
    } else if( _mode==FIT_CAL && !_zero_background ) {
      fit_bkg = (RooRealVar*)final_pars.at( 1 );
      fit_sig = (RooRealVar*)final_pars.at( 2 );
      fit_tau = (RooRealVar*)final_pars.at( 3 );
      fit_sigma = (RooRealVar*)final_pars.at( 0 );
      fit_mean = (RooRealVar*)final_pars.at( 4 );
    } else if( _mode==FIT_CAL && _zero_background ) {
      fit_bkg = 0;
      fit_sig = (RooRealVar*)final_pars.at( 1 );
      fit_tau = 0;
      fit_sigma = (RooRealVar*)final_pars.at( 0 );
      fit_mean = (RooRealVar*)final_pars.at( 2 );
    } else if( _mode==FIT_BW && !_zero_background ) {
      fit_bkg = (RooRealVar*)final_pars.at( 1 );
      fit_sig = (RooRealVar*)final_pars.at( 2 );
      fit_tau = (RooRealVar*)final_pars.at( 3 );
      fit_sigma = (RooRealVar*)final_pars.at( 0 );
      fit_mean = (RooRealVar*)final_pars.at( 4 );
    } else if( _mode==FIT_BW && _zero_background ) {
      fit_bkg = 0;
      fit_sig = (RooRealVar*)final_pars.at( 1 );
      fit_tau = 0;
      fit_sigma = (RooRealVar*)final_pars.at( 0 );
      fit_mean = (RooRealVar*)final_pars.at( 2 );
    } else if( _mode==FIT_CALTRK && !_zero_background ) {
      fit_bkg = (RooRealVar*)final_pars.at( 2 );
      fit_sig = (RooRealVar*)final_pars.at( 3 );
      fit_tau = (RooRealVar*)final_pars.at( 4 );
      fit_sigma = (RooRealVar*)final_pars.at( 7 ); // trk sigma
      fit_mean = (RooRealVar*)final_pars.at( 8 );
    } else if( _mode==FIT_CALTRK && _zero_background ) {
      fit_bkg = 0;
      fit_sig = (RooRealVar*)final_pars.at( 2 );
      fit_tau = 0;
      fit_sigma = (RooRealVar*)final_pars.at( 5 ); // trk sigma
      fit_mean = (RooRealVar*)final_pars.at( 6 );
    } else {
      cout << " unhandled fit mode " << __LINE__ << endl;
      return false;
    }

    if( _zero_background ) {
      _fitted_signal = fit_sig->getVal();
      _fitted_signal_error = sqrt( _fitted_signal );
    } else {
      _fitted_signal = fit_sig->getVal();
      _fitted_signal_error = fit_sig->getError();
    }
    if( !_zero_background ) {
      _fitted_background = fit_bkg->getVal();
      _fitted_background_error = fit_bkg->getError();
    }
    _fitted_zmass = fit_mean->getVal();
    _fitted_zmass_error = fit_mean->getError();
    _fitted_resolution = fit_sigma->getVal();
    _fitted_resolution_error = fit_sigma->getError();
    return true;
  }
protected:
  virtual void operator()( string& command ) {
    vector< shared_ptr<AbsPlot> >::const_iterator i = std::find_if( g_plot_stack.begin() , g_plot_stack.end() , bind(&AbsPlot::isFirstGraphic,_1)==true );
    if( i == g_plot_stack.end() ) { return; }
    shared_ptr< const PlotHistogram1D<DgHistogram1D> > hbw = dynamic_pointer_cast<const PlotHistogram1D<DgHistogram1D> >( *i );
    if( !hbw ) { return; }
    TH1* h = dynamic_cast<TH1*>( hbw->new_ROOT( "tmproohist" ) );
    if( !h ) { return; }
    RooMassFitZeeCommand::float_background();
    RooMassFitZeeCommand::standard_zwindow();
    if( opt_nobkg.value_bool() ) {  RooMassFitZeeCommand::zero_background(); }
    if( opt_hasbkg.value_bool() ) { RooMassFitZeeCommand::float_background(); }
    if( opt_81101.value_bool() ) { RooMassFitZeeCommand::trigger_zwindow(); }
    if( opt_66116.value_bool() ) { RooMassFitZeeCommand::standard_zwindow(); }
    if( opt_caltrk.value_bool() ) { RooMassFitZeeCommand::caltrk_fit(); }
    else if( opt_calo.value_bool() ) { RooMassFitZeeCommand::cal_fit(); }
    else if( opt_track.value_bool() ) { RooMassFitZeeCommand::track_fit(); }
    else if( opt_bw.value_bool() ) { RooMassFitZeeCommand::bw_fit(); }
    fit_zmass(h,true);
    if( opt_save.value_bool() ) {
      string filename( (*i)->description() );
      replace_all(filename, "/" , "_" );
      if( istarts_with(filename,"_") ) { erase_head(filename,1); }
      string plotname = filename; plotname += ".gif";
      SaveCommand::save( plotname );
      string statname = filename; statname += ".txt";
      ofstream ofile( statname.c_str() , ios::out );
      ofile << filename << " "
            << "zmass " << _fitted_zmass << " " << _fitted_zmass_error << " "
            << "zres " << _fitted_resolution << " " << _fitted_resolution_error << " "
            << "nsig " << _fitted_signal << " " << _fitted_signal_error << " "
            << "nbkg " << _fitted_background << " " << _fitted_background_error << " "
            << endl;
      ofile.close();
      cout << " saved fit output to " << endl << "  " << plotname << endl << "  " << statname << endl;
    }
  }
public:
  RooMassFitZeeCommand() 
    : opt_nobkg( this , CommandOption::BOOL , "nobkg" , "zero background" )
    , opt_hasbkg( this , CommandOption::BOOL , "hasbkg" , "float background" )
    , opt_81101( this , CommandOption::BOOL , "81101" , "fit in trigger 81-101 window" )
    , opt_66116( this , CommandOption::BOOL , "66116" , "fit in standard 66-116 window" )
    , opt_caltrk( this , CommandOption::BOOL , "caltrk" , "use voightian + crystal ball signal" )
    , opt_track( this , CommandOption::BOOL , "track" , "use crystal ball signal" )
    , opt_calo( this , CommandOption::BOOL , "calo" , "use voightian signal" )
    , opt_bw( this , CommandOption::BOOL , "bw" , "use bw signal" )
    , opt_save( this , CommandOption::BOOL , "save" , "save the fit to files" )
    , AbsCommand( "fitzee" , "RooFit the plotted histogram using a Z to ee PDF." )
  {}
};

RooMassFitZeeCommand::FitMode RooMassFitZeeCommand::_mode = RooMassFitZeeCommand::FIT_CAL;
bool RooMassFitZeeCommand::_fix_zmass = false;
bool RooMassFitZeeCommand::_zero_background = false;
double RooMassFitZeeCommand::_fitted_zmass = 0.;
double RooMassFitZeeCommand::_fitted_resolution = 0.;
double RooMassFitZeeCommand::_fitted_signal = 0.;
double RooMassFitZeeCommand::_fitted_background = 0.;
double RooMassFitZeeCommand::_fitted_zmass_error = 0.;
double RooMassFitZeeCommand::_fitted_resolution_error = 0.;
double RooMassFitZeeCommand::_fitted_signal_error = 0.;
double RooMassFitZeeCommand::_fitted_background_error = 0.;
double RooMassFitZeeCommand::_mass_min = 66.;
double RooMassFitZeeCommand::_mass_max = 116.;



// class 
// RooMassFunctionCommand : public AbsCommand
// {
// protected:
//   virtual void operator()( string& command ) {
//     vector<AbsPlot*>::const_iterator i = std::find_if( g_plot_stack.begin() , g_plot_stack.end() , bind(&AbsPlot::isFirstGraphic,_1)==true );
//     if( i == g_plot_stack.end() ) { return; }
//     const PlotHistogram2D* hbw = dynamic_cast< const PlotHistogram2D* >( *i );
//     if( !hbw ) { return; }
//     bool doDraw = command.Contains("show");
//     bool range81101 = command.Contains("81101");
//     RooMassFitZmumuCommand::float_zmass();
//     TGraphAsymmErrors* gr_sig = new TGraphAsymmErrors;
//     TGraphAsymmErrors* gr_bkg = new TGraphAsymmErrors;
//     TGraphAsymmErrors* gr_mass = new TGraphAsymmErrors;
//     TGraphAsymmErrors* gr_resolution = new TGraphAsymmErrors;
//     for( unsigned int ibin=1, fbin=hbw->nbinsy()+1; ibin!=fbin; ++ibin ) {
//       // slice 2d histogram
//       TH1* hslice = hbw->slice_x( ibin );
//       const double x = hbw->minx() + ( (hbw->maxx()-hbw->minx())/(hbw->nbinsx()) * (ibin-0.5) );
//       if( command.Contains("bkg") ) { RooMassFitZmumuCommand::float_background(); } else { RooMassFitZmumuCommand::zero_background(); }
//       if( range81101 ) { 
//         RooMassFitZmumuCommand::trigger_zwindow();
//       } else {
//         RooMassFitZmumuCommand::standard_zwindow();
//       }
//       RooMassFitZmumuCommand::fit_zmass( hslice , doDraw );
//       gr_sig->SetPoint(ibin-1,x,RooMassFitZmumuCommand::fitted_signal());
//       gr_sig->SetPointError(ibin-1,0,0,RooMassFitZmumuCommand::fitted_signal_error(),RooMassFitZmumuCommand::fitted_signal_error());
//       gr_bkg->SetPoint(ibin-1,x,RooMassFitZmumuCommand::fitted_background());
//       gr_bkg->SetPointError(ibin-1,0,0,RooMassFitZmumuCommand::fitted_background_error(),RooMassFitZmumuCommand::fitted_background_error());
//       gr_mass->SetPoint(ibin-1,x,RooMassFitZmumuCommand::fitted_zmass());
//       gr_mass->SetPointError(ibin-1,0,0,RooMassFitZmumuCommand::fitted_zmass_error(),RooMassFitZmumuCommand::fitted_zmass_error());
//       gr_resolution->SetPoint(ibin-1,x,RooMassFitZmumuCommand::fitted_resolution());
//       gr_resolution->SetPointError(ibin-1,0,0,RooMassFitZmumuCommand::fitted_resolution_error(),RooMassFitZmumuCommand::fitted_resolution_error());
//       delete hslice;
//     }
//     PlotStyleCommand::default_format( gr_sig , kBlack );
//     PlotStyleCommand::default_format( gr_bkg , kBlack );
//     PlotStyleCommand::default_format( gr_mass , kBlack );
//     PlotStyleCommand::default_format( gr_resolution , kBlack );
//     ShowCommand::new_canvas();
//     g_canvas->Divide(2,2);
//     g_canvas->cd(1);
//     gr_sig->Draw("ap");
//     g_canvas->cd(2);
//     gr_bkg->Draw("ap");
//     g_canvas->cd(3);
//     gr_mass->Draw("ap");
//     g_canvas->cd(4);
//     gr_resolution->Draw("ap");
//     ShowCommand::update_canvas();
//   }
// public:
//   RooMassFunctionCommand() : AbsCommand( "roomfunc" , "fit distribution of mass vs variable for yields and fit parameters vs that variable." ) {}
// };


class 
FitCoreGaussianCommand : public AbsCommand
{
public:
  CommandOption opt_unitgaus;
  CommandOption opt_range;
  CommandOption opt_prob;
protected:
  virtual void operator()( string& command ) {
    TH1* h = 0;
    vector< shared_ptr<AbsPlot> >::const_iterator i = std::find_if( g_plot_stack.begin() , g_plot_stack.end() , bind(&AbsPlot::isFirstGraphic,_1)==true );
    if( i == g_plot_stack.end() ) { return; }
    shared_ptr< const PlotHistogram1D<DgHistogram1D> > hbw = dynamic_pointer_cast<const PlotHistogram1D<DgHistogram1D> >( *i );
    if( !hbw ) { return; }
    h = dynamic_cast<TH1*>( hbw->new_ROOT( "tmproohist" ) );
    if( !h ) { return; }
    // if option is "unitgaus", just draw a unit gaussian on top
    if( opt_unitgaus.value_bool() ) {
      const double xmin = h->GetXaxis()->GetXmin();
      const double xmax = h->GetXaxis()->GetXmax();
      const unsigned int nbins = h->GetNbinsX();
      if( nbins < 1 ) { return; }
      if( std::abs(h->GetXaxis()->GetXmin() + h->GetXaxis()->GetXmax())>0.1 ) { 
        cout << "use only on a symmetric range about zero." << endl;
        return;
      }
      const double binsize = (xmax-xmin)/nbins;
      const double norm_range_min = -binsize;
      const double norm_range_max = binsize;
      TH1F* hunit = new TH1F("hunit","", nbins , xmin , xmax );
      TF1* gf = new TF1("tmpgf","gaus",h->GetXaxis()->GetXmin() , h->GetXaxis()->GetXmax());
      // set normalization so that the integral of the two bins on other side of zero is the same.
      // assumes the parent histogram is symmetric
      h->GetXaxis()->SetRangeUser(norm_range_min,norm_range_max-(binsize/0.6));
      double new_norm = h->Integral() / TMath::Erf( binsize / sqrt(2.) );
      h->GetXaxis()->SetRangeUser(norm_range_min,norm_range_max);
      gf->SetParameter( 0 , 2.*new_norm );
      gf->SetParameter( 1 , 0. );
      gf->SetParameter( 2 , 1. );
      hunit->FillRandom( "tmpgf" , (Int_t)new_norm*2 );
      delete gf;
      hunit->SetFillStyle( 0 );
      hunit->SetLineColor( kRed );
      hunit->SetLineWidth( 3 );
      hunit->Draw("same");
      return;
    }
    double allowed_xmin = 0;
    double allowed_xmax = 0;
    bool allowed_x = opt_range.found();
    if( allowed_x ) { allowed_xmin = opt_range.value_range_begin(); allowed_xmax = opt_range.value_range_end(); }
    double acceptable_prob_min = 0.1;
    double acceptable_prob_max = 1.0;
    if( opt_prob.found() ) { acceptable_prob_min = opt_prob.value_range_begin(); acceptable_prob_max = opt_prob.value_range_end(); }
    // start fit
    h->Fit("gaus");
    TF1* last_f = h->GetFunction("gaus");
    double fit_min = h->GetXaxis()->GetXmin();
    double fit_max = h->GetXaxis()->GetXmax();
    double bin_width = (h->GetXaxis()->GetXmax() - h->GetXaxis()->GetXmin())/h->GetNbinsX();
    double last_chisq = last_f->GetChisquare();
    double last_ndof = last_f->GetNDF();
    double last_prob = last_ndof > 0. ? (TMath::Prob( last_chisq , (Int_t)(last_ndof) )) : 999;
    while( true ) {
      h->Fit("gaus","","",fit_min,fit_max);
      TF1* f = h->GetFunction("gaus");
      double chisq = f->GetChisquare();
      double ndof = f->GetNDF();
      double prob = ndof>0. ? (TMath::Prob(chisq,(Int_t)ndof)) : 999;
      if( prob<(last_prob) || (prob>=acceptable_prob_min && prob<=acceptable_prob_max) ) { break; }
      fit_min = fit_min + bin_width;
      fit_max = fit_max - bin_width;
      if( fit_min >= fit_max ) { break; }
      if( allowed_x && (fit_min>=allowed_xmin || fit_max<=allowed_xmax) ) { break; }
      last_f = f;
      last_chisq = chisq;
      last_ndof = ndof;
      last_prob = prob;
    }
    if( last_f ) {
      cout << " Gaussian core: <x>=" << last_f->GetParameter(1) << " sigma=" << last_f->GetParameter(2) << endl;
      cout << "                chi^2=" << last_chisq << " ndof=" << last_ndof << " prob=" << last_prob << endl;
    }
    ShowCommand::update_canvas();
  }
public:
  FitCoreGaussianCommand() 
    : opt_unitgaus( this , CommandOption::BOOL , "unitgaus" , "just draw a unit gaussian on top" )
    , opt_range( this , CommandOption::RANGE , "range" , "restrict x range to this" )
    , opt_prob( this , CommandOption::RANGE , "prob" , "allowed fit probability" )
    , AbsCommand( "fitcoregaus" , "fits core of distribution to gaussian (centered at halfway through histgoram range)" ) {}
};


class 
TaggerInputFitCommand : public AbsCommand
{
public:
  void bfit( const TH1* h , const bool draw = false ) {
    RooRealVar* trackx = new RooRealVar( "trackx" , "likeliihod input" , -40 , 40 , "" );
    RooDataHist* template_bjets = new RooDataHist( "template_bjets" , "b jet likelihood input template" , *trackx , h );
    RooRealVar* bMean = new RooRealVar( "bMean" , "b input PDF mean" , 0. , -1 , 1. );
    RooRealVar* bSigma = new RooRealVar( "bSigma" , "b input PDF width" , 1. , 0 , 2. );
    RooRealVar* bAlpha = new RooRealVar( "bAlpha" , "b input PDF crystal ball tail parameter" , -5 , -10. , 0. );
    RooRealVar* bOrder = new RooRealVar( "bOrder" , "b input PDF crystal ball order parameter" , 0.5 , 0. , 25. );
    RooCBShape* bModelA = new RooCBShape( "bModelA" , "b template PDF" , *trackx , *bMean , *bSigma , *bAlpha , *bOrder );
    RooRealVar* bWidthB = new RooRealVar( "bWidthB" , "b template PDF width B" , 2.5*h->GetRMS() , 0.04 , 40. );
    RooRealVar* bWidthC = new RooRealVar( "bWidthC" , "b template PDF width C" , 2.5*h->GetRMS() , 0.04, 40. );
    RooGaussian* bGaussB = new RooGaussian( "bGaussB" , "b template PDF gaussian B" , *trackx , *bMean , *bWidthB );
    RooGaussian* bGaussC = new RooGaussian( "bGaussC" , "b template PDF gaussian C" , *trackx , *bMean , *bWidthC );
    RooRealVar* bFA = new RooRealVar( "bFA" , "fraction of core gaussian" , 0.40 , 0 , 1. );
    RooRealVar* bFC = new RooRealVar( "bFC" , "fraction of core gaussian" , 0.05 , 0 , 1. );
    RooAddPdf* bModel = new RooAddPdf( "bModel" , "b template" , RooArgList(*bModelA,*bGaussB,*bGaussC) , RooArgList(*bFA,*bFC) );
    RooFitResult* bresult = bModel->fitTo( *template_bjets , RooFit::Save(true) , RooFit::InitialHesse(kTRUE) , RooFit::Hesse(kTRUE) , RooFit::Minos(kTRUE) );
    bresult->Print("v");
    if( draw ) {
      ShowCommand::new_canvas();
      RooPlot* frame1 = trackx->frame();
      template_bjets->plotOn( frame1 , RooFit::MarkerColor(kRed) , RooFit::LineColor(kRed)  );
      bModel->plotOn( frame1 , RooFit::LineColor(13) );
      TVirtualPad::Pad()->SetLogy(1);
      frame1->Draw();
      ShowCommand::update_canvas();
    }
    {
      trackx->setRange( -40 , 40 );
      RooAbsReal* intvar = bModel->createIntegral( *trackx );
      const double intgrl = intvar->getVal();
      cout << " integral: " << intgrl << endl;
      trackx->setVal( 0. );
      cout << " value at x=0: " << bModel->getVal() / intgrl << endl;
      trackx->setVal( 10. );
      cout << " value at x=10: " << bModel->getVal() / intgrl << endl;
    }
    // memory leak. ignore.
  }
  void ufit( const TH1* h , const bool draw = false ) {
    RooRealVar* trackx = new RooRealVar( "trackx" , "likeliihod input" , -40 , 40 , "" );
    RooDataHist* template_ujets = new RooDataHist( "template_ujets" , "b jet likelihood input template" , *trackx , h );

    RooRealVar* uMean = new RooRealVar( "uMean" , "u template PDF mean" , 0. );
    RooRealVar* uMeanC = new RooRealVar( "uMean" , "u template PDF mean C" , 0. , -5 , 5 );
    RooRealVar* uWidthA = new RooRealVar( "uWidthA" , "u template PDF width A" , 0.5*h->GetRMS() , 0 , 40. );
    RooRealVar* uWidthB = new RooRealVar( "uWidthB" , "u template PDF width B" , 1.5*h->GetRMS() , 0 , 40. );
    RooRealVar* uWidthC = new RooRealVar( "uWidthC" , "u template PDF width C" , 2.5*h->GetRMS() , 0 , 40. );
    RooGaussian* uGaussA = new RooGaussian( "uGaussA" , "u template PDF gaussian A" , *trackx , *uMean , *uWidthA );
    RooGaussian* uGaussB = new RooGaussian( "uGaussB" , "u template PDF gaussian B" , *trackx , *uMean , *uWidthB );
    RooGaussian* uGaussC = new RooGaussian( "uGaussC" , "u template PDF gaussian C" , *trackx , *uMeanC , *uWidthC );
    RooRealVar* uFA = new RooRealVar( "uFA" , "fraction of core gaussian" , 0.75 , 0 , 1. );
    RooRealVar* uFC = new RooRealVar( "uFC" , "fraction of core gaussian" , 0.25 , 0 , 1. );
    RooAddPdf* uModel = new RooAddPdf( "uModel" , "u template double gaussian" , RooArgList(*uGaussA,*uGaussB,*uGaussC) , RooArgList(*uFA,*uFC) );
    RooFitResult* uresult = uModel->fitTo( *template_ujets , RooFit::Save(true) , RooFit::InitialHesse(kTRUE) , RooFit::Hesse(kTRUE) , RooFit::Minos(kTRUE) );
    uresult->Print("v");
    if( draw ) {
      ShowCommand::new_canvas();
      RooPlot* frame1 = trackx->frame();
      template_ujets->plotOn( frame1 , RooFit::MarkerColor(kRed) , RooFit::LineColor(kRed)  );
      uModel->plotOn( frame1 , RooFit::LineColor(13) );
      TVirtualPad::Pad()->SetLogy(1);
      frame1->Draw();
      ShowCommand::update_canvas();
    }
    // memory leak. ignore.
  }
public:
  CommandOption opt_draw;
protected:
  virtual void operator()( string& command ) {
    vector< shared_ptr<AbsPlot> >::const_iterator i = std::find_if( g_plot_stack.begin() , g_plot_stack.end() , bind(&AbsPlot::isFirstGraphic,_1)==true );
    if( i == g_plot_stack.end() ) { return; }
    shared_ptr< const PlotHistogram1D<DgHistogram1D> > hbw = dynamic_pointer_cast<const PlotHistogram1D<DgHistogram1D> >( *i );
    if( !hbw ) { return; }
    TH1* h1 = 0;
    if( hbw ) { h1 = dynamic_cast<TH1*>( hbw->new_ROOT( "tmproohist" ) ); }
    if( !h1 ) { return; }
    // h1 is the binned input variable. fit to a smooth function
    const bool dodraw = opt_draw.found();
    if( istarts_with(opt_draw.value_string(),"b") ) { bfit(h1,dodraw); }
    if( istarts_with(opt_draw.value_string(),"u") ) { ufit(h1,dodraw); }
  }
public:
  TaggerInputFitCommand() 
    : opt_draw( this , CommandOption::STRING , "draw" , "draw (b) or (u) fit" )
    , AbsCommand( "taginfit" , "fit tagger input to smooth parameterization (options: bjet,ujet; cfunc)" ) {}
};

class 
FindIntervalCommand : public AbsCommand
{
public:
  CommandOption opt_valcon;
protected:
  virtual void operator()( string& command ) {
    TH1* h = 0;
    vector< shared_ptr<AbsPlot> >::const_iterator i = std::find_if( g_plot_stack.begin() , g_plot_stack.end() , bind(&AbsPlot::isFirstGraphic,_1)==true );
    if( i == g_plot_stack.end() ) { return; }
    shared_ptr<const PlotHistogram1D<DgHistogram1D> > hbw = dynamic_pointer_cast<const PlotHistogram1D<DgHistogram1D> >( *i );
    if (  hbw ) { 
      h = dynamic_cast<TH1*>( hbw->new_ROOT( "tmproohist" ) );
    }
    if( !h ) { return; }
    double central = 0.;
    double goalfrac = 0.;
    if( opt_valcon.found() ) {
      central = opt_valcon.value_range_begin();
      goalfrac = opt_valcon.value_range_end();
      cout << " central value = " << central << " containment = " << goalfrac << endl;
    } else {
      central = askFloat( "central value: " );;
      goalfrac = askFloat( "containment (0.99730=3s,0.95450=2s,0.68269=1s): " );
    }
    double totalint = h->Integral();
    if( totalint < 1E-3 ) { return; }
    double totalwidth = (h->GetXaxis()->GetXmax() - h->GetXaxis()->GetXmin());
    double step = totalwidth / (h->GetNbinsX()*2.);
    double halfwidth = step;
    double hxmin = h->GetXaxis()->GetXmin();
    double hxmax = h->GetXaxis()->GetXmax();
    double xmin = hxmax;
    double xmax = hxmin;
    double frac = 0.;
    while( (2.*halfwidth) < totalwidth ) {
      xmin = std::max( central - halfwidth , hxmin );
      xmax = std::min( central + (halfwidth*0.9999) , hxmax );
      h->GetXaxis()->SetRangeUser(xmin,xmax);
      double thisint = h->Integral();
      double thisfrac = thisint/totalint;
      if( thisfrac > goalfrac ) {
        frac = thisfrac;
        break;
      }
      halfwidth += step;
    }
    if( frac > 0. ) {
      cout << " xmin = " << setw(10) << xmin << endl;
      cout << " xmax = " << setw(10) << xmax << endl;
      cout << " hw   = " << setw(10) << halfwidth << endl;
      cout << " frac = " << setw(10) << frac << endl;
    }
  }
public:
  FindIntervalCommand() 
    : opt_valcon( this , CommandOption::RANGE , "valcon" , "specify (central value, containment)" )
    , AbsCommand( "findinterval" , "find interval which contains a given percentage of the total distribution" )
  {}
};


// class 
// PlotColorCommand : public AbsCommand
// {
// protected:
//   virtual void operator()( string& command ) {
//     int c = AbsCommand::intOption(command);
//     PlotColor* p = new PlotColor( c );
//     p->draw();
//     g_plot_stack.push_back( p );
//   }
// public:
//   PlotColorCommand() : AbsCommand( "color" , "change plot color" ) {}
// };

class 
PseudoExperimentsCommand : public AbsCommand
{
protected:
  virtual void operator()( string& cmd ) {
    string command(cmd);
    if( !g_canvas ) { return; }
    TIter iPrim( g_canvas->GetListOfPrimitives() );
    dg::remove_graphic( "pseudoexperiment_results" );
    // loop over all the histograms.  set the range for every one,
    // e.g. so that calls to Integral will integrate over the proper range.
    while( TObject* obj = iPrim.Next() ) {
      if( obj->InheritsFrom("TH1") ) {
        TH1* h = dynamic_cast<TH1*>(obj);
        if( dynamic_cast<TH2*>(obj) ) { continue; }
        int n = h->GetNbinsX();
        int trials = askInteger( "ntrials?" );
        double pmean = askFloat( "poissonmean?" );
        // throw pseudo experiments
        for( int it=0; it!=trials; ++it ) {
          int pn = gRandom->Poisson(pmean); // number of samples to sum
          while( pn==0 ) { pn = gRandom->Poisson(pmean); }
          double sum = 0; // sample sum
          for( int ip=0; ip!=pn; ++ip ) {
            int in = (int)floor(gRandom->Rndm()*h->GetEntries());
            double x = h->GetXaxis()->GetXmin()-100;
            int count=0;
            for( int iBin=1; iBin!=(n+1); ++iBin ) {
              if( count >= in ) {
                x = h->GetXaxis()->GetBinCenter(iBin);
                break;
              }
              count += (int)(h->GetBinContent(iBin));
            }
            if( x < h->GetXaxis()->GetXmin() ) { continue; }
            sum += x;
          }
          dg::fillh( "pseudoexperiment_results" , n , 0 , (int)ceil(h->GetXaxis()->GetXmax()*pmean*3) , sum );
        }
//         clearCanvas();
//         OldPlotCommand::plot( dg::get , "" , "" );
//         ShowCommand::show("");
        return;
      }
    }
    cout << " nothing to throw." << endl;
  }
public:
  PseudoExperimentsCommand() : AbsCommand( "pseudo" , "throw pseudo experiments from the first histogram on the current canvas." ) {}
};

class 
TableCommand : public AbsCommand
{
public:
  typedef enum { ASCII } Mode;
  static const unsigned int digit( const double& val , const int decade ) {
    const double powd = pow(10.,(double)decade);
    const double x = std::floor( val / powd );
    const double y = std::floor( val / (powd*10) ) * 10.;
    double z = x - y;
    const double zf = std::floor(z);
    const double zc = std::ceil(z);
    if( (zc-z)<(z-zf) ) { z = zc; } else { z = zf; }
    return( (unsigned int) z );
  }
  static const int decade_most( const double& val ) {
    double decade = std::log10( val );
    // round to nearest unit decade
    const int dlow = (int)std::floor(decade);
    const int dhigh = (int)std::ceil(decade);
    if( (decade-dlow) < (dhigh-decade) ) { decade = dlow; } else { decade = dhigh; }
    return( (int)decade );
  }
  static const string round_value( const double val , const unsigned int figures = 5 , int most_decade = -std::numeric_limits<int>::max() ) {
    string result;
    if( most_decade <= -999999 ) {     
      most_decade = static_cast<int>(std::floor( std::log10(val) ));
      if( val==0. ) { 
        return( (boost::format("%|g|") % 0).str() );
      }
    }
    const int round_to_this_multiple_log10 = most_decade - figures + 1;
    const double tmp = std::pow(10.,round_to_this_multiple_log10);
    const double result_val = boost::math::iround( val / tmp ) * tmp;
    result = (boost::format("%|g|") % result_val).str();
    return result;
//     // find most significant digit. compute least significant digit
//     if( most_decade == -99999999 ) { most_decade = decade_most(val); }
//     const int least_decade = most_decade - figures + 1;
//     // construct string
//     string result = "";
//     // round result using least insignificant digit
//     double result_val = val;
//     const unsigned int lid = digit(val,least_decade-1);
//     if( lid>=5 ) { result_val += 1.*pow(10.,(double)least_decade); }
//     for( int idec = most_decade; idec >= least_decade; --idec ) {
//       if( most_decade<0 ) {
//         if( idec==most_decade ) { result += "0."; }
//         for( int ipad=-1; ipad!=most_decade; --ipad ) { result +="0"; }
//       }
//       else if( idec==-1 ) { result +="."; }
//       result += digit(result_val,idec);
//     }
//     return string(result.Data());
  }
  static void determine_precision_from_uncertainties( const double& value , const double& elow , const double& ehigh ,
                                                      int& most_decade , int& least_decade , int& sigfigs , int& dec_col ) {
    if( std::abs(elow) <= std::numeric_limits<double>::min() && std::abs(ehigh) <= std::numeric_limits<double>::min() ) {
      if( std::abs(value) <= std::numeric_limits<double>::min() ) {
        most_decade = 0;
      } else if( std::abs(value-1.) <= std::numeric_limits<double>::min() ) {
        most_decade = 0;
      } else {
        most_decade = decade_most( value );
      }
      least_decade = -2;
      sigfigs = most_decade - least_decade + 1;
      dec_col = std::max( 0 , most_decade+1 );
      return;
    }
    least_decade = std::min( (int)std::floor(std::log10(elow)) , (int)std::floor(std::log10(ehigh)) ) - 1;
    most_decade = decade_most( value );
    sigfigs = most_decade - least_decade + 1;
    dec_col = -999;
    if( least_decade < 0 ) { 
      dec_col = std::max( 0 , most_decade+1 );
    } else {
      dec_col = 0;
    }
  }
  static void determine_precision_from_uncertainties( const double& value , const std::vector<double>& errs , int& most_decade , int& least_decade , int& sigfigs , int& dec_col ) {
    for( std::vector<double>::const_iterator i=errs.begin(), f=errs.end(); i!=f; ++i ) {
      if( i==errs.begin() ) { least_decade = (int)std::floor(std::log10(*i)); }
      else { least_decade = std::max( least_decade , (int)std::floor(std::log10((*i))) ); }
    }
    --least_decade;
    // if value is zero, chop it
    if( value <= std::numeric_limits<double>::round_error() ) {
      most_decade = 0;
    } else {
      most_decade = decade_most( value );
    }
    sigfigs = most_decade - least_decade + 1;
    dec_col = -999;
    if( least_decade < 0 ) { 
      dec_col = std::max( 0 , most_decade+1 );
    }
  }
  static string align_decimal( const string& inst , const unsigned int col ) {
    if( inst == "" ) { return inst; }
    if( inst.size()>=2 && inst.substr(col,1) == "." ) { return inst; }
    if( col > 999 ) { return inst; }
    string::size_type i = inst.find(".");
    assert( i!=col );
    string result( inst );
    if( i==string::npos ) {
      for( unsigned int npad=0; npad<(inst.size()-col); ++npad ) { result += " "; }
    } else if( i<col ) {
      for( unsigned int npad=0; npad<(col-i); ++npad ) { result = string(" ")+result; }
    } else {
      for( unsigned int npad=0; npad<(i-col); ++npad ) { result += " "; }
    }
    return result;
  }
  static void display_value( const double& central , const double& err ) {
    int most_decade, least_decade, sigfigs, dec_col;
    TableCommand::determine_precision_from_uncertainties( central , err , err , most_decade , least_decade , sigfigs , dec_col);
    cout << round_value(central,sigfigs,most_decade) << " +/- " << round_value(err,sigfigs,most_decade) << endl;
  }
  static void table( shared_ptr<const DgEfficiency0D> d , const Mode mode = TableCommand::ASCII ) {
    assert( d );
    double central, elow, ehigh;
    d->efficiency( central , elow , ehigh );
    int most_decade, least_decade, sigfigs, dec_col;
    TableCommand::determine_precision_from_uncertainties( central , elow , ehigh , most_decade , least_decade , sigfigs , dec_col );
    const unsigned int width_central = 10;
    const unsigned int width_elow = 10;
    const unsigned int width_ehigh = 10;
    if( mode == ASCII ) {
      cout << setw(width_central) << round_value(central,sigfigs,most_decade)
           << " + " << setw(width_ehigh) << round_value(ehigh,sigfigs,most_decade) << " - " << setw(width_elow) << round_value(elow,sigfigs,most_decade) 
           << " = " << setw(width_central) << d->numerator() << " / " << setw(width_central) << d->denominator() 
           << endl;
    } else { assert( !"unhandled mode" ); }
  }
  static void table( shared_ptr<const DgEfficiency1D> d , const unsigned int bingroupsize = 1 , const Mode mode = TableCommand::ASCII ) {
    assert( d );
    assert( bingroupsize>0 );
    const unsigned int width_xmin = 15;
    const unsigned int width_xmax = 15;
    const unsigned int width_central = 15;
    const unsigned int width_elow = 15;
    const unsigned int width_ehigh = 15;
    double xmin, xmax, central, elow, ehigh;
    for( unsigned int ibin=0, fbin=d->nbins(); ibin<fbin; ibin+=bingroupsize ) {
      if( bingroupsize==1 ) {
        d->efficiency( ibin , xmin , xmax , central , elow , ehigh );
      } else {
        int f = std::min(ibin+bingroupsize,fbin);
        d->efficiency( ibin , f , xmin , xmax , central , elow , ehigh );
      }
      int most_decade, least_decade, sigfigs, dec_col;
      determine_precision_from_uncertainties( central , elow , ehigh , most_decade , least_decade , sigfigs , dec_col );
      if( mode == ASCII ) {
        cout << std::left << setw(width_xmin) << xmin << " " << std::left << setw(width_xmax) << xmax << "   "
             << std::left << setw(width_central) << align_decimal(round_value(central,sigfigs,most_decade),dec_col)
             << " + " << std::left << setw(width_ehigh) << align_decimal(round_value(ehigh,sigfigs,most_decade),dec_col)
             << " - " << std::left << setw(width_elow) << align_decimal(round_value(elow,sigfigs,most_decade),dec_col);
        if( true ) {
          DataGraphics::Count num = 0;
          DataGraphics::Count den = 0;
          for( unsigned int i=ibin, f=std::min(ibin+bingroupsize,fbin); i!=f; ++i ) {
            num += d->numerator(i);
            den += d->denominator(i);
          }
          cout << " " << num << " " << den;
        }
        cout << endl;
      } else { assert( !"unhandled mode" ); }
      const double emax = std::max( elow , ehigh );
      const double w = emax>0. ? 1./(emax*emax) : 999999999999.;
    }
//     if( central_weight > 0. ) {
//       central_mean /= central_weight;
//       const double central_error = sqrt( 1./central_weight );
//       int most_decade, least_decade, sigfigs, dec_col;
//       determine_precision_from_uncertainties( central_mean , central_error , central_error , most_decade , least_decade , sigfigs , dec_col );
//       cout << " average: "
//            << std::left << setw(width_central) << align_decimal(round_value(central_mean,sigfigs,most_decade),dec_col)
//            << " +/- " << std::left << setw(width_ehigh) << align_decimal(round_value(central_error,sigfigs,most_decade),dec_col)
//            << endl;
//     }
    if( true ) {
      double central, xmin, xmax, elow, ehigh;
      d->efficiency( 0 , d->nbins() , xmin , xmax , central , elow , ehigh );
      cout << boost::format( "central=%|6.6g| elow=%|6.6g| ehigh=%|6.6g|" ) % central % elow % ehigh << endl;
      int most_decade, least_decade, sigfigs, dec_col;
      determine_precision_from_uncertainties( central , elow , ehigh , most_decade , least_decade , sigfigs , dec_col );
//       cout << boost::format( "central=%|6.6g| elow=%|6.6g| ehigh=%|6.6g|" ) % central % elow % ehigh << endl;
//       cout << boost::format( "central=%|6.6g| elow=%|6.6g| ehigh=%|6.6g|" ) % round_value(central,sigfigs,most_decade) % round_value(elow,sigfigs,most_decade) % round_value(ehigh,sigfigs,most_decade) << endl;
      //cout << "sigfigs: " << sigfigs << " most_decade:" << most_decade << endl;
      cout << " average " 
           << std::left << setw(width_central) << align_decimal(round_value(central,sigfigs,most_decade),dec_col)
           << " - " << std::left << setw(width_elow) << align_decimal(round_value(elow,sigfigs,most_decade),dec_col)
           << " + " << std::left << setw(width_ehigh) << align_decimal(round_value(ehigh,sigfigs,most_decade),dec_col)
           << endl;
    }
  }
protected:
  virtual void operator()( string& command ) {
    string pathto;
    shared_ptr<const DgAbsGraphic> gr;
    bool ok = OldPlotCommand::find_single_plot( command , pathto , gr );
    if( !ok ) { return; }
    assert( gr );
    shared_ptr<const DgGraph2D> bg2d = dynamic_pointer_cast<const DgGraph2D>(gr);
    // plot the plot
    if( dynamic_pointer_cast<const DgEfficiency0D>(gr) ) { table( dynamic_pointer_cast<const DgEfficiency0D>(gr) ); }
    else if( dynamic_pointer_cast<const DgEfficiency1D>(gr) ) {
      unsigned int bingroupsize = 1;
      vector<string> tokens;
      string cmd(command);
      boost::split( tokens , cmd , boost::is_any_of(" \t") );
      vector<string>::iterator i=find( tokens.begin() , tokens.end() , "rebin" );
      if( i!=tokens.end() && std::distance(i,tokens.end())>1 ) {
        bingroupsize = atoi( (*(++i)).c_str() );
        if( bingroupsize<1 ) { bingroupsize = 1; }
        cout << " grouping " << bingroupsize << " 1-D efficiency bins" << endl;
      }
      table( dynamic_pointer_cast<const DgEfficiency1D>(gr) , bingroupsize );
    } else if( bg2d ) {
      cout << " (x,y) : " << pathto << endl;
      for( DgGraph2D::const_iterator i=bg2d->begin(), f=bg2d->end(); i!=f; ++i ) {
        cout << format("         ( %|10g| , %|10g| )" ) %  (*i)._x % (*i)._y << endl;
      }
    }
  }
public:
  TableCommand() : AbsCommand( "table" , "dump data in table form" ) {}
};

class 
AsymmetricEfficiencyCommand : public AbsCommand
{
protected:
  virtual void operator()( string& command ) {
    double den = askInteger( "denominator: " );
    double num = askInteger( "numerator: " );
    double frac_pred , frac_err_min , frac_err_max ;
    DataGraphics::FindEfficiencyWithAsymmetricError(num,den,frac_pred,frac_err_min,frac_err_max);

    int most_decade, least_decade, sigfigs, dec_col;
    TableCommand::determine_precision_from_uncertainties( frac_pred , frac_err_min , frac_err_max , most_decade , least_decade , sigfigs , dec_col );
    cout << " efficiency = " << TableCommand::round_value(frac_pred,sigfigs,most_decade) 
         << " + " << TableCommand::round_value(frac_err_max,sigfigs,most_decade)
         << " - " << TableCommand::round_value(frac_err_min,sigfigs,most_decade)
         << endl;
  }
public:
  AsymmetricEfficiencyCommand() : AbsCommand( "asymmeff" , "compute efficiency and asymmetric errors from binomial distribution" ) {}
};

// setter/getter functors for the map of values
class
SetValueCommand : public AbsCommand
{
protected:
  virtual void operator()( string& command ) {
    string tmp( command );
    vector<string> tokens;
    split( tokens , tmp, is_any_of(" ") );
    if( tokens.size()==2 ) { 
      const string& key = tokens[0];
      const string& value = tokens[1];
      if(g_glob_pars.find(key) != g_glob_pars.end()) {
        std::cout << "Replacing " << key << " = " << g_glob_pars[key] << " ---> " << value << std::endl;
      }
      g_glob_pars[key] = value;
      // HACK: override integrated luminosity
      if( istarts_with(key,"lumi") ) {
        float f = atof(value.c_str());
        if( f > 0. ) { g_luminosity.reset(f); } else { g_luminosity.reset(); }
      }
    }
  }
public:
  SetValueCommand() : AbsCommand( "set" , "set value of a parameter" ) {}
};

class 
GetValueCommand : public AbsCommand
{
protected:
  virtual void operator()( string& command ) {
    string tmp( command );
    vector<string> tokens;
    split( tokens , tmp, is_any_of(" ") );
    for(unsigned int ip=0; ip<tokens.size(); ip++) {
      const string& key = tokens[ip];
      if(g_glob_pars.find(key) != g_glob_pars.end()) {
        std::cout << key << " = " << g_glob_pars[key] << std::endl;
      }
      else {
        std::cout << key << " = <UNDEFINED>" << std::endl;
      }
    }
  }
public:
  GetValueCommand() : AbsCommand( "get" , "print values of all requested parameters" ) {}
};

class 
PushStackCommand : public AbsCommand
{
public:
  static void process( string& command) {
    // split into tokens
    string tmp( command );
    vector<string> tokens;
    split( tokens , tmp, is_any_of(" ") );
    const int ntokens = tokens.size();
    // retrieve each histogram
    vector<string> pathto(ntokens);
    vector< shared_ptr<const DgAbsGraphic> > gr(ntokens);
    bool ok[ntokens];
    for(int i=0;i<ntokens;i++) {
      if(tokens[i].find("/")==string::npos) {
        ok[i] = OldPlotCommand::find_single_plot( tokens[i] , pathto[i] , gr[i] );
      }
      else {
        // example: "/periodI.root/dg/dg/RUNS/167575/d02_found_w_pos"
        dg::View view = dg::get(tokens[i]);
        if(view.size()>0) {
          gr[i] = view[0].graphic();
          pathto[i] = tokens[i];
          if(gr[i]) ok[i] = true; else ok[i]=false;
        }
        else ok[i] = false;
      }
      if( ok[i]) {
        g_working_stack.push_back(gr[i]);
      }
    }
    
    assert( gr[0] );
    assert( gr[1] );
  }
protected:
  virtual void operator()( string& command ) {
    std::cout << "Original stack size: " << g_working_stack.size() << std::endl;
    process(command);
    std::cout << "Final stack size: " << g_working_stack.size() << std::endl;
  }
public:
  PushStackCommand() : AbsCommand( "pushstack" , "Push a DataGraphics object onto the user's working stack" ) {}
};


class 
ClearStackCommand : public AbsCommand
{
protected:
  virtual void operator()( string& command ) {
    g_working_stack.clear();
  }
public:
  ClearStackCommand() : AbsCommand( "clearstack" , "clears custom plot stack" ) {}
};

// work in progress - w asymmetry plots
class 
AsymmetryCommand : public AbsCommand
{
public:
  typedef enum { ASCII, GRAPHIC } Mode;
  static double asymmetry(double npos, double nneg) {
    return (npos-nneg)/(npos+nneg);
  }
  // assume sqrt(N) large-N poisson uncertainty in each variable
  static void asymmetry_err(double npos, double nneg, double& f, double& err) {
    if(npos==0 || nneg==0) {
      f=0.; err=0.;
      return;
    }
    const double n = npos - nneg;
    const double d = npos + nneg;
    f = n/d;
    err = 2*sqrt(npos*nneg/pow(d,3));
    return;
  }
  static void asymmetry_dump(double npos, double nneg, string path) {
    const double ntot = npos + nneg;
    // get asymmetry and simple error
    double f(0); double err(0);
    asymmetry_err(npos,nneg,f,err);
    std::cout << f << " +/- " << err;
    std::cout << " RAW: " << npos << " + " << nneg << " = " << ntot;
    // get run integrated luminosity in 1/nb
    const string rnum = RunLumis::instance()->key(path);
    const double lumi = RunLumis::instance()->value(path)*1000.0;
    if(!rnum.empty()) {
      std::cout << "; " << rnum << " " << lumi << " SCALED: " << npos/lumi << " + " << nneg/lumi << " = " << ntot/lumi;
    }
    std::cout << std::endl;
  }
  static void process_pair( string& command) {
    // split into tokens
    string tmp( command );
    vector<string> tokens;
    split( tokens , tmp, is_any_of(" ") );
    const int ntokens = tokens.size();
    if(ntokens < 2) {
      std::cout << "Please provide two arguments:" << std::endl;
      std::cout << "     asym effPOS effNEG" << endl;
      return;
    }
    // retrieve each histogram
    vector<string> pathto(ntokens);
    vector< shared_ptr<const DgAbsGraphic> > gr(ntokens);
    bool ok[ntokens];
    for(int i=0;i<ntokens;i++) {
      if(tokens[i].find("/")==string::npos) {
        ok[i] = OldPlotCommand::find_single_plot( tokens[0] , pathto[0] , gr[0] );
        ok[i] = OldPlotCommand::find_single_plot( tokens[1] , pathto[0] , gr[1] );
      }
      else {
        // example: "/periodI.root/dg/dg/RUNS/167575/d02_found_w_pos"
        dg::View view = dg::get(tokens[i]);
        if(view.size()>0) {
          gr[i] = view[0].graphic();
          pathto[i] = tokens[i];
          if(gr[i]) ok[i] = true; else ok[i]=false;
        }
        else ok[i] = false;
      }
    }
    
    if( !ok[0] || !ok[1] ) { return; }
    assert( gr[0] );
    assert( gr[1] );
    
    shared_ptr<const DgEfficiency0D> eff0d0 = dynamic_pointer_cast<const DgEfficiency0D>(gr[0]);
    shared_ptr<const DgEfficiency0D> eff0d1 = dynamic_pointer_cast<const DgEfficiency0D>(gr[1]);
    if( eff0d0 && eff0d1 ) {
      const double npos = eff0d0->numerator();
      const double nneg = eff0d1->numerator();
      asymmetry_dump(npos,nneg,pathto[0]);
    }
    shared_ptr<const DgHistogram1D> h1d0 = dynamic_pointer_cast<const DgHistogram1D>(gr[0]);
    shared_ptr<const DgHistogram1D> h1d1 = dynamic_pointer_cast<const DgHistogram1D>(gr[1]);
    if( h1d0 && h1d1 ) {
      const double npos = h1d0->integrate();
      const double nneg = h1d1->integrate();
      asymmetry_dump(npos,nneg,pathto[0]);
    }
  }
protected:
  virtual void operator()( string& command ) {
    process_pair(command);
  }
public:
  AsymmetryCommand() : AbsCommand( "asym" , "W charge asymmetry studies" ) {}
};

// load DgXMLSampleDescription ////////////////////////////////////////

const shared_ptr<DgXMLSampleDescription> 
read_sample_description( const string& root_filename )
{
  // write sample description once per event.
  string xml_filename( root_filename );
  replace_last( xml_filename , ".root" , ".xml" );
  boost::filesystem::path idescpath( xml_filename );
  if( ! filesystem::exists( idescpath ) ) {
    cout << " could not find " << xml_filename << " sample dscription " << idescpath.string() << endl;
    return shared_ptr<DgXMLSampleDescription>();
  }
  shared_ptr<DgXMLSampleDescription> desc( new DgXMLSampleDescription );
  const bool ok = desc->load_xml( idescpath.string() );
  if( ! ok ) {
    cout << " could not load sample dscription " << idescpath.string() << endl;
    return shared_ptr<DgXMLSampleDescription>();
  }
  return desc;
}

// load a file into the DgCollection::current() /////////////////////////////

const bool
load_file_into_current( const string& fname , const bool merge )
{
  if( boost::iends_with(fname,"dg") ) {
    // treat as boost serialization i/o
    ifstream inf( fname.c_str() );
    if( inf ) { 
      boost::archive::text_iarchive iar( inf );
      iar >> (*dg::current());
      return true;
    } else {
      cout << " could not open " << fname << "." << endl;
      return false;
    }
  } else { // treat as ROOT i/o
    boost::filesystem::path pathname( fname );
    if( boost::filesystem::exists( pathname ) ) {
      TFile* file = TFile::Open( fname.c_str() );
      if( !file || !file->IsOpen() || file->IsZombie() ) {
        cout << " could not open " << fname << "." << endl;
        return false;
      }
      file->cd();
      // activate dg read on demand functionality. now mandatory for
      // viewing (but does not allow merging).
      if( !merge ) { 
        dg::current()->read_on_demand( true ); 
        dg::root()->read_on_demand( true );
      }
      dg::current()->load( dynamic_cast<TDirectory*>(file) , merge );
      file->Close();
      delete file;
      // load sample description from neighboring file
      DescValue v;
      v.desc = read_sample_description( fname );
      if( ! v.desc ) { 
        cout << " could not read sample description for " << fname << endl;
        return false;
      } 
      v.coll = dg::current();
      v.path = dg::current()->path_string();
      // cout << " added sample description for " << fname << endl;
      // cout << "     coll=" << v.coll << " path=" << v.path << endl;
      if( merge ) { 
        // look for description to merge
        // DescColl::iterator icoll = find_if( g_descriptions.begin() , g_descriptions.end() , bind( &DescValue::coll , _1 ) == v.coll );
        // DescColl::iterator icoll = find_if( g_descriptions.begin() , g_descriptions.end() , bind( &DescValue::coll , bind( &DescColl::value_type::second , _1 ) ) == v.coll );
        DescColl::iterator icoll = g_descriptions.find( v.desc->name() );
        if( icoll != g_descriptions.end() ) {
          icoll->second.desc->merge( *v.desc );
        } else {
          g_descriptions[ v.desc->name() ] = v;
        }
      } else {
        DescColl::iterator icoll = g_descriptions.find( v.desc->name() );
        if( icoll != g_descriptions.end() ) {
          cout << " warning: overwriting sample description " << endl;
          icoll->second.desc->print( cout );
          cout << " with new " << v.desc->name() << endl;
          v.desc->print( cout );
        }
        g_descriptions[ v.desc->name() ] = v;
      }
      return true;
    } else {
      cout << " could not find file " << fname << "." << endl;
      return false;
    }
  }
  return false;
}

// main loop ////////////////////////////////////////////////////////////////

void
DgPlot::start( const int& argc , char* argv[] )
{

  // parse arguments using boost::program_options
  using namespace boost::program_options;
  options_description cmdline_options( "dgplot options" );
  cmdline_options.add_options()
    ( "help" , "display help message" )
    ( "input,i" , value< vector<string> >() , "input filename(s)" )
    ( "list,l" , value< vector<string> >() , "filenames for text files containing lists of input filename(s)" )
    ( "batch,b" , value< vector<string> >() , "batch command filename(s) containing commands to execute" )
    ( "merge,m" , value<string>() , "merge all input files and save as this filename" )
    ( "rootout,r" , value<string>() , "convert input or merge to root output and store to this filename." )
    ( "quit,q" , "quit after all batch/merge operations have completed" )
    ;
  positional_options_description cmdline_positional_options;
  cmdline_positional_options.add( "input" , -1 ); // an unnamed argument anywhere on the command line will be treated as an input filename
  variables_map cmdline;
  try {
    parsed_options tmp( command_line_parser(argc,argv).options(cmdline_options).allow_unregistered().positional(cmdline_positional_options).run() ); 
    store( tmp , cmdline ); 
    notify( cmdline ); 
  } catch(...) {
    cout << "ERROR: " << cmdline_options << endl;
    exit(-1);
  }
  if( cmdline.count( "help" ) ) { 
    cout << cmdline_options << endl;
    exit(-1);
  }
  app = new DgPlotApp("dgplot",0,0);
  vector<string> input_filenames;
  vector<string> batch_filenames;
  bool merge_all_files = false;
  string merge_filename = "merged.root";
  bool quit_when_exhausted = false;
  if( cmdline.count( "merge" ) ) {
    merge_all_files = true;
    merge_filename = cmdline["merge"].as<string>();
  }
  if( cmdline.count( "input" ) ) {
    input_filenames = cmdline[ "input" ].as< vector<string> >();
  }
  if( cmdline.count("list") ) {
    vector<string> inlists = cmdline["list"].as< vector<string> >();
    for( vector<string>::const_iterator i=inlists.begin(), f=inlists.end(); i!=f; ++i ) {
      ifstream inf( (*i).c_str() );
      while( !(inf.eof()) ) { 
        string line;
        getline( inf , line );
        if( line != "" ) { input_filenames.push_back( line ); }
      }
    }
  }
  if( cmdline.count( "batch" ) ) {
    batch_filenames = cmdline[ "batch" ].as< vector<string> >();
  }
  if( cmdline.count( "quit" ) ) {
    quit_when_exhausted = true;
  }

  //  app = new DgPlotApp("dg_plot",&nargs,argv);

  if( quit_when_exhausted ) {
    app->quit_when_no_commands_batched( true );
  }

  if( input_filenames.empty() ) {
    cout << cmdline_options << endl;
    cout << endl << " specify input files." << endl;
    exit(-1);
  }

  BOOST_FOREACH( const string& batch_filename , batch_filenames ) {
    cout << "loading batch commands from " << batch_filename << endl;
    app->LoadCommandsFromFile( batch_filename.c_str() );
  }

  const bool save_input_as_root = cmdline.count("rootout");
  const string save_input_as_root_filename = save_input_as_root ? cmdline[ "rootout"].as< string >() : string("");

  DgCollection* bg_root = dg::root();
  if( input_filenames.size()==1 ) {
    string fname = input_filenames.back();
    string coll_filename = fname;
    boost::replace_all( coll_filename , "/" , "_" );
    dg::down( coll_filename , "file" );
    load_file_into_current( fname , false );
    dg::up();
  } else if( merge_all_files ) {
    string merge_collname = merge_filename;
    boost::replace_all( merge_collname , "/" , "_" );
    if( merge_collname=="" ) { merge_collname = "merge"; }
    //dg::down( merge_collname , "merged collection" );
    BOOST_FOREACH( const string& filename , input_filenames ) {
      cout << " merging " << filename << endl;
      load_file_into_current( filename , true );
    }
    //dg::up();
    cout << " saving merged file " << merge_filename << "..." << endl;
    { 
      dg::root()->set_save_filename( merge_filename );
      dg::root()->no_root_save();
    } // end save merged file.
    cout << " saved " << merge_filename << endl;
    if( g_descriptions.size()==1 ) {
      string s( merge_filename );
      if( icontains( s , ".root " ) ) { 
        erase_last( s , ".root" );
      }
      s += ".xml";
      cout << " saving merged description " << s << endl;
      (*g_descriptions.begin()).second.desc->set_dg_filename( merge_filename+".root" );
      (*g_descriptions.begin()).second.desc->save_xml( s );
    }
    if( quit_when_exhausted ) { 
      if( save_input_as_root ) { 
        string tstr(save_input_as_root_filename.c_str());
        SaveCollectionAsROOTCommand::save( tstr );
        cout << " saved as " << tstr << endl;
      }
      return; 
    }
  } else {
    cout << " [ loading " << input_filenames.size() << " files ]" << endl;
    unsigned int n = 0u;
    BOOST_FOREACH( const string& filename , input_filenames ) {
      cout << " [ loading " << filename << " (" << (++n) << "/" << input_filenames.size() << ") ]" << endl;
      string coll_filename = filename;
      boost::replace_all( coll_filename , "/" , "_" );
      dg::down( coll_filename , "file" );
      load_file_into_current( filename , false );
      dg::up();
    }
  }
  if( save_input_as_root ) { 
    string tstr(save_input_as_root_filename.c_str());
    SaveCollectionAsROOTCommand::save( tstr );
    cout << " saved as " << tstr << endl;
  }
  // hooks for commands
  QuitCommand cmdQuit; // quit
  HelpCommand cmdHelp; // describe commands
  BatchCommand cmdBatch; // run batch commands
  ClearQueueCommand cmdClearQueue; // clear all commands from the batch queue before they have been executed.
  RootCintCommand cmdRootCint; // run command using CINT
  // plotting
  PlotCommand cmdPlot; // plot a graphics object on the current canvas
  OldPlotCommand cmdPlotOld; // plot a graphics object on the current canvas, old style
  ShowCommand cmdShow; // show the current canvas
  UpdateCanvasCommand cmdUpdate; // update the current canvas
  SaveCommand cmdSave; // save the current canvas
  SaveCollectionAsROOTCommand cmdCollAsROOT; // save the current collection and subcollections as a ROOT TDirectory.
  CleanCommand cmdClean; // clear the current canvas
  RangeCommand cmdRange; // set the x, y, and/or z axis ranges
  IntegralCommand cmdIntegral; // display the integral of each object
  MeanCommand cmdMean; // display the mean of each object
  RMSCommand cmdRMS; // display the mean of each object
  UnderflowCommand cmdUnderflow; // display the underflow of each object
  OverflowCommand cmdOverflow; // display the underflow of each object
  PaletteCommand cmdPal; // set the current color palette
  LsPlotCommand cmdLsPlot; // list current plot stack
  AspectRatioCommand cmdAspectRatio; // set the canvas aspect ratio
  LabelCommand cmdXLabel; // label axes
  // PlotColorCommand cmdPColor;
  ProfileXCommand cmdProfX; // draw profile of 2d histogram
  ProfileYCommand cmdProfY; // draw profile of 2d histogram
  OverlayModeCommand cmdOverlayMode; // set plot overlay mode
  RangeModeCommand cmdRangeMode; // set plot range mode
  DumpToWebCommand cmdWeb; // dump all plots in or below the current subdirectory to a web page.
  TableCommand cmdTable; // list data in a table
  CutflowCommand cmdCutflow; // list cuts in a table.
  GraphToHist2DCommand cmdGraphToHist2DCommand; // convert TGraph into TH2D
  SvgCommand cmdSvg; // write data graphics as svg output for Adobe Illustrator
  // fits and other graphical calculations
  PseudoExperimentsCommand cmdPseudo; // throw pseudoexperiments
  RooMassFitZmumuCommand cmdRooMassFitZmumu; // fit histogram to a Z(mumu) PDF
  RooMassFitZeeCommand cmdRooMassFitZee; // fit histogram to a Z(ee) PDF
  // RooMassFunctionCommand cmdRooMassFunction; // fit Z(mumu) dependence on some variable
  FitCoreGaussianCommand cmdFitCoreGaussian; // fit centered distribution to gaussian core.
  FindIntervalCommand cmdFindInterval; // find interval containing desired fraction of a histogram.
  TaggerResponseCurveCommand cmdTaggerResponseCurve; // compute tagger response curve given output
                                                     // for or b and u jet samples.
  ResolutionVsXCommand cmdResolutionVsX; // fit resolution vs x pdfs
  TaggerInputFitCommand cmdTaggerInputFit; // fit tagger input variable to smooth parameterization (and optionally output C function)
  // table commands
  // other calculations
  PoissonSignificanceCommand cmdPoissonSignificance; // compute significance of a simple poisson fluctuation
  AsymmetricEfficiencyCommand cmdAsymmEff; // compute efficiency with asymmetric errors
  PlotOperationCommand cmdOperation;
  AsymmetryCommand cmdAsym;
  // object manipulation commands
  SetValueCommand cmdSetValue;             // set a value for some key
  GetValueCommand cmdGetValue;             // get a value from some key
  PushStackCommand cmdPushStack;
  ClearStackCommand cmdClearStack;
  // navigation
  PwdCommand cmdPwd; // show the current graphics collection
  RootCollCommand cmdRootColl; // change to the root graphics collection
  CdCommand cmdCd; // change the current graphics collection
  DownCollCommand cmdDownColl; // descend to subcollection of the current collection
  UpCollCommand cmdUpColl; // ascend to the parent of the current collection
  AcrossCollCommand cmdAcrossColl; // move to neighbor of the current collection
  LsCollCommand cmdLsColl; // list graphics objects in current collection  
  CountCollCommand cmdCountColl; // count number of collections contained in current collection
  CountDataCommand cmdCountData; // count number of data objects contained in current collection
  ShowCollectionTreeCommand cmdShowCollectionTree; // show the collection hierarchy downward from the current collection
  GroupCommand cmdGroup; // plot grouping tree manipulation
  DescriptionCommand cmdDescription; // manipulate sample descriptions
  ResourceMgrCommand cmdResourceMgr; // manipulate resource manager
  TerminalCommand cmdTerminal; // manipulate terminal settings
  DebugCommand cmdDebugCommand; // manipulate debug mode

  // main interactive loop
  // ================================================================
  app->Run();

}

class
BwPlotStyle : public TStyle
{
public:
  void set_publication() {
    g_axis_color = 12;
    SetNdivisions(103, "x");
    SetNdivisions(103, "y");
    SetNdivisions(510, "z");
    SetAxisColor(g_axis_color, "x");
    SetAxisColor(g_axis_color, "y");
    SetAxisColor(g_axis_color, "z");
    SetLabelColor(g_axis_label_color, "x");
    SetLabelColor(g_axis_label_color, "y");
    SetLabelColor(g_axis_label_color, "z");
    SetLabelFont(62, "x");
    SetLabelFont(62, "y");
    SetLabelFont(62, "z");
    SetLabelOffset(0.01, "x");
    SetLabelOffset(0.01, "y");
    SetLabelOffset(0.01, "z");
    SetLabelSize(0.06, "x");
    SetLabelSize(0.06, "y");
    SetLabelSize(0.06, "z");
    SetTickLength(0.03, "x");
    SetTickLength(0.03, "y");
    SetTickLength(0.03, "z");
    SetTitleOffset(1.4, "x");
    SetTitleOffset(1.4, "y");
    SetTitleOffset(1.05, "z");
    SetTitleSize(0.06, "x");
    SetTitleSize(0.06, "y");
    SetTitleSize(0.06, "z");
    SetTitleColor(g_axis_title_color, "x");
    SetTitleColor(g_axis_title_color, "y");
    SetTitleColor(g_axis_title_color, "z");
    SetTitleFont(62, "x");
    SetTitleFont(62, "y");
    SetTitleFont(62, "z");
    SetBarWidth(1);
    SetBarOffset(0);
    SetDrawBorder(0);
    SetOptLogx(0);
    SetOptLogy(0);
    SetOptLogz(0);
    SetOptDate(0);
    SetOptStat(0);
    SetOptTitle(kFALSE);
    //SetOptFit(0);
    //    SetNumberContours(100);
    GetAttDate()->SetTextFont(42);
    GetAttDate()->SetTextSize(0.025);
    GetAttDate()->SetTextAngle(0);
    GetAttDate()->SetTextAlign(11);
    GetAttDate()->SetTextColor(1);
    SetDateX(0.01);
    SetDateY(0.01);
    SetEndErrorSize(2);
    SetErrorX(0.5);
    SetFuncColor(kRed);
    SetFuncStyle(1);
    SetFuncWidth(2);
    SetGridColor(0);
    SetGridStyle(3);
    SetGridWidth(0);
    //    SetLegendBorderSize(1);
    //     SetHatchesLineWidth(1);
    //     SetHatchesSpacing(0.8);
    SetFrameFillColor(0);
    SetFrameLineColor(0);
    SetFrameFillStyle(1001);
    SetFrameLineStyle(1);
    SetFrameLineWidth(1);
    SetFrameBorderSize(1);
    SetFrameBorderMode(0);
    SetHistFillColor(14);
    SetHistLineColor(14);
    SetHistFillStyle(1001);
    SetHistLineStyle(1);
    SetHistLineWidth(2);
    //     SetHistMinimumZero(kFALSE);
    //     SetCanvasPreferGL(kFALSE);
    SetCanvasColor(0);
    SetCanvasBorderSize(1);
    SetCanvasBorderMode(0);
    SetCanvasDefH(500);
    SetCanvasDefW(700);
    SetCanvasDefX(10);
    SetCanvasDefY(10);
    SetPadColor(0);
    SetPadBorderSize(1);
    SetPadBorderMode(0);
    SetPadBottomMargin(0.20);
    SetPadTopMargin(0.1);
    SetPadLeftMargin(0.18);
    SetPadRightMargin(0.13);
    SetPadGridX(kFALSE);
    SetPadGridY(kFALSE);
    SetPadTickX(0);
    SetPadTickY(0);
    SetPaperSize(20, 26);
    SetScreenFactor(1);
    //SetScreenFactor(0.8);
    SetStatColor(0);
    SetStatTextColor(1);
    SetStatBorderSize(2);
    SetStatFont(42);
    SetStatFontSize(0);
    SetStatStyle(1001);
    SetStatFormat("6.4g");
    SetStatX(0.98);
    SetStatY(0.995);
    SetStatW(0.2);
    SetStatH(0.16);
    SetStripDecimals(kTRUE);
    //     SetTitleAlign(13);
    SetTitleFillColor(0);
    SetTitleTextColor(1);
    SetTitleBorderSize(1);
    SetTitleFont(42);
    SetTitleFontSize(0);
    SetTitleStyle(1001);
    SetTitleX(0.01);
    SetTitleY(0.995);
    SetTitleW(0);
    SetTitleH(0);
    SetLegoInnerR(0.5);

    string fLineStyleArrayTmp[30] = {"", "     ", "    12 12", "    4 8", 
                                      "    12 16 4 16", "    20 12 4 12", "    20 12 4 12 4 12 4 12", "    20 20", "    20 12 4 12 4 12", 
                                      "    80 20", "    80 40 4 40", "     ", "     ", "     ", 
                                      "     ", "     ", "     ", "     ", "     ", 
                                      "     ", "     ", "     ", "     ", "     ", 
                                      "     ", "     ", "     ", "     ", "     ", "     "};
    for( Int_t i=0; i!=30; ++i ) {
      SetLineStyleString(i, fLineStyleArrayTmp[i].c_str() );
    }

    SetHeaderPS("");
    SetTitlePS("");
    SetFitFormat("5.4g");
    SetPaintTextFormat("g");
    SetLineScalePS(3);
    //     SetColorModelPS(0);
    SetTimeOffset(788918400);

    SetLineColor(1);
    SetLineStyle(1);
    SetLineWidth(1);
    SetFillColor(19); // to correctly plot 2d histograms (e.g. with zcol), this must not be zero.
    SetFillStyle(1001);
    SetMarkerColor(4);
    SetMarkerSize(1.2);
    SetMarkerStyle(8);
    SetTextAlign(11);
    SetTextAngle(0);
    SetTextColor(g_axis_title_color);
    SetTextFont(62);
    SetTextSize(0.05);
  }
  void set_presentation() {
    g_axis_color = 16;
    SetNdivisions(310, "x");
    SetNdivisions(510, "y");
    SetNdivisions(510, "z");
    SetAxisColor(12, "x");
    SetAxisColor(12, "y");
    SetAxisColor(12, "z");
    SetLabelColor(g_axis_label_color, "x");
    SetLabelColor(g_axis_label_color, "y");
    SetLabelColor(g_axis_label_color, "z");
    SetLabelFont(42, "x");
    SetLabelFont(42, "y");
    SetLabelFont(42, "z");
    SetLabelOffset(0.01, "x");
    SetLabelOffset(0.01, "y");
    SetLabelOffset(0.01, "z");
    SetLabelSize(0.04, "x");
    SetLabelSize(0.04, "y");
    SetLabelSize(0.04, "z");
    SetTickLength(0.03, "x");
    SetTickLength(0.03, "y");
    SetTickLength(0.03, "z");
    SetTitleOffset(1.4, "x");
    SetTitleOffset(1.4, "y");
    SetTitleOffset(1.05, "z");
    SetTitleSize(0.04, "x");
    SetTitleSize(0.04, "y");
    SetTitleSize(0.04, "z");
    SetTitleColor(g_axis_title_color, "x");
    SetTitleColor(g_axis_title_color, "y");
    SetTitleColor(g_axis_title_color, "z");
    SetTitleFont(42, "x");
    SetTitleFont(42, "y");
    SetTitleFont(42, "z");
    SetBarWidth(1);
    SetBarOffset(0);
    SetDrawBorder(0);
    SetOptLogx(0);
    SetOptLogy(0);
    SetOptLogz(0);
    SetOptDate(0);
    SetOptStat(0);
    SetOptTitle(kFALSE);
    //SetOptFit(0);
    //    SetNumberContours(100);
    GetAttDate()->SetTextFont(42);
    GetAttDate()->SetTextSize(0.025);
    GetAttDate()->SetTextAngle(0);
    GetAttDate()->SetTextAlign(11);
    GetAttDate()->SetTextColor(1);
    SetDateX(0.01);
    SetDateY(0.01);
    SetEndErrorSize(2);
    SetErrorX(0.5);
    SetFuncColor(2);
    SetFuncStyle(1);
    SetFuncWidth(2);
    SetGridColor(0);
    SetGridStyle(3);
    SetGridWidth(0);
    //    SetLegendBorderSize(1);
    //     SetHatchesLineWidth(1);
    //     SetHatchesSpacing(0.8);
    SetFrameFillColor(0);
    SetFrameLineColor(0);
    SetFrameFillStyle(1001);
    SetFrameLineStyle(1);
    SetFrameLineWidth(1);
    SetFrameBorderSize(1);
    SetFrameBorderMode(0);
    SetHistFillColor(14);
    SetHistLineColor(1);
    SetHistFillStyle(1001);
    SetHistLineStyle(1);
    SetHistLineWidth(1);
    //     SetHistMinimumZero(kFALSE);
    //     SetCanvasPreferGL(kFALSE);
    SetCanvasColor(0);
    SetCanvasBorderSize(1);
    SetCanvasBorderMode(0);
    SetCanvasDefH(500);
    SetCanvasDefW(700);
    SetCanvasDefX(10);
    SetCanvasDefY(10);
    SetPadColor(0);
    SetPadBorderSize(1);
    SetPadBorderMode(0);
    SetPadBottomMargin(0.15);
    SetPadTopMargin(0.1);
    SetPadLeftMargin(0.13);
    SetPadRightMargin(0.13);
    SetPadGridX(kFALSE);
    SetPadGridY(kFALSE);
    SetPadTickX(0);
    SetPadTickY(0);
    SetPaperSize(20, 26);
    SetScreenFactor(1);
    //SetScreenFactor(0.8);
    SetStatColor(0);
    SetStatTextColor(1);
    SetStatBorderSize(2);
    SetStatFont(42);
    SetStatFontSize(0);
    SetStatStyle(1001);
    SetStatFormat("6.4g");
    SetStatX(0.98);
    SetStatY(0.995);
    SetStatW(0.2);
    SetStatH(0.16);
    SetStripDecimals(kTRUE);
    //     SetTitleAlign(13);
    SetTitleFillColor(0);
    SetTitleTextColor(1);
    SetTitleBorderSize(1);
    SetTitleFont(42);
    SetTitleFontSize(0);
    SetTitleStyle(1001);
    SetTitleX(0.01);
    SetTitleY(0.995);
    SetTitleW(0);
    SetTitleH(0);
    SetLegoInnerR(0.5);

    string fLineStyleArrayTmp[30] = {"", "     ", "    12 12", "    4 8", 
                                      "    12 16 4 16", "    20 12 4 12", "    20 12 4 12 4 12 4 12", "    20 20", "    20 12 4 12 4 12", 
                                      "    80 20", "    80 40 4 40", "     ", "     ", "     ", 
                                      "     ", "     ", "     ", "     ", "     ", 
                                      "     ", "     ", "     ", "     ", "     ", 
                                      "     ", "     ", "     ", "     ", "     ", "     "};
    for( Int_t i=0; i!=30; ++i ) {
      SetLineStyleString(i, fLineStyleArrayTmp[i].c_str() );
    }

    SetHeaderPS("");
    SetTitlePS("");
    SetFitFormat("5.4g");
    SetPaintTextFormat("g");
    SetLineScalePS(3);
    //     SetColorModelPS(0);
    SetTimeOffset(788918400);

    SetLineColor(1);
    SetLineStyle(1);
    SetLineWidth(1);
    SetFillColor(19); // to correctly plot 2d histograms (e.g. with zcol), this must not be zero.
    SetFillStyle(1001);
    SetMarkerColor(4);
    SetMarkerSize(1.2);
    SetMarkerStyle(8);
    SetTextAlign(11);
    SetTextAngle(0);
    SetTextColor(1);
    SetTextFont(42);
    SetTextSize(0.05);
  }
  void set_atlas() {
    cout << "Applying ATLAS style settings..." << endl;
    // use plain black on white colors
    Int_t icol=0; // WHITE
    SetFrameBorderMode(icol);
    SetFrameFillColor(icol);
    SetCanvasBorderMode(icol);
    SetCanvasColor(icol);
    SetPadBorderMode(icol);
    SetPadColor(icol);
    SetStatColor(icol);
    //SetFillColor(icol); // don't use: white fill color for *all* objects
    // set the paper & margin sizes
    SetPaperSize(20,26);
    // set margin sizes
    SetPadTopMargin(0.05);
    SetPadRightMargin(0.05);
    SetPadBottomMargin(0.16);
    SetPadLeftMargin(0.16);
    // set title offsets (for axis label)
    SetTitleXOffset(1.4);
    SetTitleYOffset(1.4);
    // use large fonts
    //Int_t font=72; // Helvetica italics
    Int_t font=42; // Helvetica
    Double_t tsize=0.05;
    SetTextFont(font);
    SetTextSize(tsize);
    SetLabelFont(font,"x");
    SetTitleFont(font,"x");
    SetLabelFont(font,"y");
    SetTitleFont(font,"y");
    SetLabelFont(font,"z");
    SetTitleFont(font,"z");
    SetLabelSize(tsize,"x");
    SetTitleSize(tsize,"x");
    SetLabelSize(tsize,"y");
    SetTitleSize(tsize,"y");
    SetLabelSize(tsize,"z");
    SetTitleSize(tsize,"z");
    // use bold lines and markers
    SetMarkerStyle(20);
    SetMarkerSize(1.2);
    SetHistLineWidth(2.);
    SetLineStyleString(2,"[12 12]"); // postscript dashes
    // get rid of X error bars 
    //SetErrorX(0.001);
    // get rid of error bar caps
    SetEndErrorSize(0.);
    // do not display any of the standard histogram decorations
    SetOptTitle(0);
    //SetOptStat(1111);
    SetOptStat(0);
    //SetOptFit(1111);
    SetOptFit(0);
    // put tick marks on top and RHS of plots
    SetPadTickX(1);
    SetPadTickY(1);
  }
  BwPlotStyle() : TStyle( "BwPlotStyle" , "style used for bw data graphics" ) 
  {
    //set_publication();
    //set_presentation();
    set_atlas();
  }
};

void
DgPlotApp::LoadCommandsFromFile( const char* filename )
{
  ifstream inf( filename );
  if( !inf.is_open() ) { 
    cout << " could not load batch commands from " << filename << endl;
    return;
  }
  char line[1024];
  cout << "loaded batch commands: " << endl;
  while( inf.getline( line , sizeof(line) ) > 0 ) {
    string cmd(line);
    _commands.push( cmd );
    cout << setw(10) << _commands.size() << " " << cmd << endl;
  }
}

void
DgPlotApp::Run( bool ret )
{
#if BOOST_VERSION>=104800
  const std::string fstring = history_filename.string();
#else
  const std::string fstring = history_filename.file_string();
#endif
  if( boost::filesystem::exists( fstring.c_str() ) ) {
    // load readline history
    int ok = read_history(fstring.c_str());
    if( ok ) {
      cerr << "could not read command line history from " << fstring << " (" << ok << "). if this problem persists, delete that file to reset." << endl;
    }
  }
  // load sample normalization
  SampleNormalization::instance()->save_example( ".dgplot_normalization" );
  RunLumis::instance()->save_example( ".dgplot_lumi" );

  //dgplot -b tmpbatch tagging/../overview/whaa0e34.dat.20100302.v1/whaa0e34.dat.20100302.v1.root
  // using_history();
  // stifle_history( 10000 ); // stop recording history after 10k entries.

  gRandom = new TRandom3(4376);

  g_plot_palette.clear();
  g_plot_palette.push_back( 16 );
  g_plot_palette.push_back( 17 );
  g_plot_palette.push_back( 18 );
  g_plot_palette.push_back( 19 );
  g_plot_palette.push_back( 20 );

  //plstream* plotter = new plstream;
  //plotter->gdev("pdf");
  //plotter->sfnam("output.svg");

  // set current plotting style
  gStyle = new BwPlotStyle();
  PaletteCommand::palette( PaletteCommand::BLUE );

  string last_command = "";

  cout << endl << endl << endl
       << " =========================================================" << endl
       << " dgplot command line plotting and fitting routines for the" << endl
       << "        DataGraphics package" << endl
       << endl
       << "        boveia@hep.uchicago.edu" << endl
       << "        portions (c) University of California 2003-2008" << endl
       << "        portions (c) University of Chicago 2009-2011" << endl
       << endl
       << " type help for more information" << endl
       << " run with --help for command line options" << endl
       << " =========================================================" << endl;
    


  while( !g_quit && 
         (!_quit_when_no_commands || !_commands.empty()) ) {

    // if batching commands, don't show the xwindow.
    if( _commands.empty() ) {
      ShowCommand::show_override( true ); // true = show
    } else {
      ShowCommand::show_override( false ); // false = don't show
    }

    // start resource cleanup here, then stop once command has been typed.
    // boost::thread resource_maintenance_thread( function<void()>(&DgResourceMgr::maintenance) );
    DgResourceMgr::maintenance();

    if( _commands.empty() ) {
      boost::scoped_ptr<char> c( readline("dg] ") );
      string inputcmd( c.get() );
      if( c && *c ) { add_history( c.get() ); }
      trim( inputcmd );
      // handle special cases
      if( inputcmd == "" || istarts_with(inputcmd,"\n") ) { continue; }
      // remove everything after comment
      inputcmd = regex_replace( inputcmd , regex("#(.*)") , "" );
      // if( istarts_with(inputcmd,".") ) {
      //   replace_all(inputcmd,".","");
      //   inputcmd = last_command + inputcmd;
      //   replace_all(inputcmd,"  "," ");
      // }
      vector<string> cmd_list;
      split( cmd_list , inputcmd , is_any_of(";") , token_compress_on );
      BOOST_FOREACH( const string& cmd , cmd_list ) { _commands.push(cmd); }
    }

    // resource_maintenance_thread.join(); // wait for thread to complete. don't want dg manipulations while cleanup is in progress.
    
    if( !_commands.empty() ) { // execute the next command
      string next_command( _commands.front() );
      if( g_debug_mode ) { cout << " batch: " << dg::current()->path_string() << "] " << _commands.front() << endl; }
      _commands.pop();
      trim( next_command );
      // handle special cases
      if( istarts_with( next_command , "#" ) ) { continue; } // ignore comments
      // handle normal command cases
      string command_name = first_token( next_command );
      AbsCommand* cmd = match_command( command_name );
      if( !cmd ) { 
        cout << "not understood. type help for a list of valid commands." << endl;
        continue;
      }
      // chop off the command name and pass the rest to the command
      last_command = next_command;
      size_t isp = next_command.find(" "); 
      if( isp==string::npos ) { next_command.clear(); } else { erase_head( next_command , isp+1 ); }
      //    cout << " executing " << next_command;
      if( g_debug_mode ) { 
        cout << " executing command " << cmd->name() << " with line:" << next_command << endl; 
      }
      cmd->execute(next_command);
    } // execute the next command

  } // main loop

  if( _quit_when_no_commands && _commands.empty() ) {
    cout << "finished with batch commands." << endl;
  }

  // save history to history file (non-batch-mode only)
  // if( !_quit_when_no_commands ) { 
  
  // if( !boost::filesystem::exists( history_filename ) ) {
  //   ofstream of( history_filename.c_str() , ios::out );
  //   of << "";
  //   of.close();
  // }
  

  cout << "saving history to " << fstring << " " << 
    write_history( fstring.c_str() )
       << endl;
  // }
  
  //delete gRandom;
}

int
main( int argc , char* argv[] )
{
  DgPlot m;
  m.start( argc , argv );
}
