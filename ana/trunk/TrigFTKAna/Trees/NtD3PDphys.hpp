#ifndef WRAP_NTD3PDPHYS_HPP
#define WRAP_NTD3PDPHYS_HPP

// NTD3PDPHYS
// ================================================================
// base class for ATLAS physics D3PDs
// ================================================================
// 2010-05-25 Antonio Boveia (boveia@hep.uchicago.edu)

#include <cassert>
#include <string>
#include <list>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/algorithm/string.hpp>

class AnaTruthTable;
class AnaTruthParticle;
class AnaElectron;
class AnaPhoton;
class AnaMuon;
class AnaTrack;
class AnaJet;
class AnaMET;
class AnaTrigger;
class AnaVertex;
class AnaTruthTable;

class
NtD3PDphys
{

public:

  typedef enum { NONE=0, WZ=1, TOP=2, PETER=3, SGTOP=4, EW=5, JETMET=6 } Type;
  typedef float ftype;

protected:
  
  Type _type;

public:

  NtD3PDphys() : _type(NONE) {}
  NtD3PDphys( const Type& type ) : _type(type) {}
  virtual ~NtD3PDphys() {}

  const Type type() const { return _type; }

  static std::string filename_to_metaname( const std::string& filename ,
                                           const Type type ) {
    std::string tname = "NONE";
    switch( type ) {
    case WZ: tname = "WZ"; break;
    case EW: tname = "EW"; break;
    case TOP: tname = "TOP"; break;
    case PETER: tname = "PETER"; break;
    case SGTOP: tname = "SGTOP"; break;
    case JETMET: tname = "JETMET"; break;
    case NONE: break;
    default:
      assert( !"unhandled case" );
    }
    return( tname + "_" + filename );
  }

  static const Type type_from_metaname( const std::string& metaname ) {
    std::list< std::string > tokens;
    boost::split( tokens , metaname , boost::is_any_of("_") );
    assert( tokens.size() > 1 );
    const std::string& tname( tokens.front() );
    if( tname=="WZ" ) { return WZ; }
    if( tname=="EW" ) { return EW; }
    if( tname=="TOP" ) { return TOP; }
    if( tname=="PETER" ) { return PETER; }
    if( tname=="SGTOP" ) { return SGTOP; }
    if( tname=="JETMET" ) { return JETMET; }
    if( tname=="NONE" ) { return NONE; }
    assert( !"unhandled case" );
    return NONE;
  }

  static const std::string filename_from_metaname( const std::string& metaname ) {
    std::list< std::string > tokens;
    boost::split( tokens , metaname , boost::is_any_of("_") );
    assert( tokens.size() > 1 );
    tokens.pop_front();
    return( boost::join( tokens , "_" ) );
  }

  static const bool type_and_filename_from_metaname( const std::string& metaname , Type& type , std::string& filename ) {
    std::list< std::string > tokens;
    boost::split( tokens , metaname , boost::is_any_of("_") );
    assert( tokens.size() > 1 );
    const std::string& tname( tokens.front() );
    if( tname=="WZ" ) { type = WZ; }
    else if( tname=="EW" ) { type = EW; }
    else if( tname=="TOP" ) { type = TOP; }
    else if( tname=="PETER" ) { type = PETER; }
    else if( tname=="SGTOP" ) { type = SGTOP; }
    else if( tname=="JETMET" ) { type = JETMET; }
    else if( tname=="NONE" ) { type = NONE; }
    else { assert( !"unhandled case" ); }
    tokens.pop_front();
    filename = boost::join( tokens , "_" );
    return true;
  }

  virtual const bool begin_file( const std::string& filename ) { return false; }
  virtual const unsigned long num_events() const { return 0; }
  virtual const std::string open_filename() const { return std::string(""); }
  virtual const bool end_file() { return false; }

  virtual const bool get_event( const unsigned long& ievent ) { return false; }

  virtual const unsigned long run_number() const { return 0; }
  virtual const unsigned long event_number() const { return 0; }
  virtual const unsigned long lumi_block() const { return 0; }
  virtual const double average_mu() const { return 0.0; }
  virtual const double actual_mu() const { return 0.0; }
  virtual const ftype mc_weight() const { return 1.; }
  virtual const std::vector<ftype>& pdf_weights() const { static std::vector<ftype> empty; return empty; }
  virtual const unsigned long mc_channel() const { return 0; }
  virtual const int mc_hfor() const { return 0; }
  virtual const bool lar_error() const { return false; }
  virtual const int bdff() const { return 0; }
  virtual const float mcevt_event_scale() const { return 0; }
  virtual const float mcevt_alphaQCD() const { return 0; }
  virtual const float mcevt_alphaQED() const { return 0; }
  virtual const float mcevt_pdf_scale() const { return 0; }
  virtual const int mcevt_id1() const { return 0; }
  virtual const int mcevt_id2() const { return 0; }
  virtual const float mcevt_pdf_x1() const { return 0; }
  virtual const float mcevt_pdf_x2() const { return 0; }
  virtual const float mcevt_pdf1() const { return 0; }
  virtual const float mcevt_pdf2() const { return 0; }

  virtual const unsigned long nbytes() const { return 0; }

  virtual const std::vector< boost::shared_ptr<const AnaElectron> >& electrons() const { 
    static std::vector< boost::shared_ptr<const AnaElectron> > tmp;
    return tmp;
  }
  virtual const std::vector< boost::shared_ptr<const AnaPhoton> >& photons() const { 
    static std::vector< boost::shared_ptr<const AnaPhoton> > tmp;
    return tmp;
  }
  virtual const std::vector< boost::shared_ptr<const AnaMuon> >& muons() const { 
    static std::vector< boost::shared_ptr<const AnaMuon> > tmp;
    return tmp;
  }
  virtual const std::vector< boost::shared_ptr<const AnaTrack> >& offline_tracks() const { 
    static std::vector< boost::shared_ptr<const AnaTrack> > tmp;
    return tmp;
  }
  virtual const std::vector< boost::shared_ptr<const AnaJet> >& jets() const { 
    static std::vector< boost::shared_ptr<const AnaJet> > tmp;
    return tmp;
  }
  virtual const std::vector< boost::shared_ptr<const AnaVertex> >& vertices() const { 
    static std::vector< boost::shared_ptr<const AnaVertex> > tmp;
    return tmp;
  }
  virtual const std::vector< boost::shared_ptr<const AnaMET> >& missing_et() const { 
    static std::vector< boost::shared_ptr<const AnaMET> > tmp;
    return tmp;
  }
  virtual const boost::shared_ptr<const AnaTrigger> trigger() const { 
    static const boost::shared_ptr<const AnaTrigger> null;
    return null;
  }
  virtual const boost::shared_ptr<const AnaTruthTable> truth_table() const {
    static const boost::shared_ptr<const AnaTruthTable> null;
    return null;
  }
  virtual const std::vector< boost::shared_ptr<const AnaTruthParticle> >& truth_particles() const { 
    static std::vector< boost::shared_ptr<const AnaTruthParticle> > tmp;
    return tmp;
  }

};

#endif // WRAP_NTD3PDPHYS_HPP
