#ifndef WRAP_JETTAGGER_HPP
#define WRAP_JETTAGGER_HPP

#include <vector>
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/assume_abstract.hpp>
#include <boost/serialization/version.hpp>
#include "DataGraphics/DgCollection.hh"

class AnaJet;
class AnaTrack;

class
JetTagger
{
public:
  typedef enum { TRAINING = 1 , TAGGING = 2 } TaggerState;
  typedef enum { NOMINAL_CENTROID = 1 , TRACK_CENTROID = 2 } JetCentroidOption;
protected:
  std::string _name;
  DataGraphics::DgCollection* _dgcoll;
  TaggerState _state;
  JetCentroidOption _jet_centroid_option;
protected:
  virtual const bool save( const std::string& filename ) = 0;
  virtual const bool load( const std::string& filename ) = 0;
  virtual const bool _train( const boost::shared_ptr<const AnaJet>& jet , const std::vector< boost::shared_ptr<const AnaTrack> >& tracks ) = 0;
  virtual const double _tag( const boost::shared_ptr<const AnaJet>& jet , const std::vector< boost::shared_ptr<const AnaTrack> >& tracks ) const = 0;
public:
  static const bool compute_jet_eta_phi( const boost::shared_ptr<const AnaJet>& jet , 
                                         const std::vector< boost::shared_ptr<const AnaTrack> >& tracks , 
                                         const JetCentroidOption& option , double& eta , double& phi );
private:
  friend class boost::serialization::access;
  template<typename archiveT>
  void serialize( archiveT& ar , const unsigned int version ) {
    ar & _name;
    ar & _state;
    ar & _jet_centroid_option;
  }
public:
  JetTagger() 
    : _state( TRAINING )
    , _jet_centroid_option( NOMINAL_CENTROID )
    , _dgcoll( DataGraphics::dg::root() )
  {}
  JetTagger( DataGraphics::DgCollection* coll ) 
    : _state( TRAINING )
    , _jet_centroid_option( NOMINAL_CENTROID )
    , _dgcoll( coll )
  {}
  virtual ~JetTagger() {}

  DataGraphics::DgCollection* dgcoll() const { return _dgcoll; }

  const std::string& name() const { return _name; }
  void name( const std::string& name ) { _name = name; }

  const TaggerState state() const { return _state; }
  void state( const TaggerState& new_state ) { _state = new_state; }

  const JetCentroidOption jet_centroid_option() const { return _jet_centroid_option; }
  JetTagger* jet_centroid_option( const JetCentroidOption& new_jet_centroid_option ) { 
    _jet_centroid_option = new_jet_centroid_option;
    return this;
  }

  const bool save_training( const std::string& filename ) { return save(filename); }
  const bool load_training( const std::string& filename ) { 
    const bool ok = load(filename);
    if( ok ) { _state = TAGGING; }
    return ok;
  }

  const bool train( const boost::shared_ptr<const AnaJet>& jet , 
                    const std::vector< boost::shared_ptr<const AnaTrack> >& tracks , 
                    const std::string dgcoll = std::string() ) {
    if( _dgcoll ) { 
      _dgcoll->forward();
    }
    DataGraphics::DgCollection::down( _name , "named tagger" );
    if( !dgcoll.empty() ) {
      DataGraphics::dg::down( dgcoll , "tagging subset" );
    }
    _train(jet,tracks);
    if( !dgcoll.empty() ) { DataGraphics::dg::up(); }
    if( _dgcoll ) { 
      DataGraphics::DgCollection::back();
    }
  }
  const double tag( const boost::shared_ptr<const AnaJet>& jet , 
                    const std::vector< boost::shared_ptr<const AnaTrack> >& tracks , 
                    const std::string dgcoll = std::string() ) const {
    if( _dgcoll ) { 
        _dgcoll->forward();
    }
    DataGraphics::DgCollection::down( _name , "named tagger" );
    if( !dgcoll.empty() ) {
        DataGraphics::dg::down( dgcoll , "tagging subset" );
    }
    _tag(jet,tracks);
    if( !dgcoll.empty() ) { DataGraphics::dg::up(); }
    if( _dgcoll ) { 
      DataGraphics::DgCollection::back(); 
    }
  }

  virtual const bool compute_jet_phi( const boost::shared_ptr<const AnaJet>& jet , 
                                      const std::vector< boost::shared_ptr<const AnaTrack> >& tracks , 
                                      const JetCentroidOption& option , double& phi ) const {
    double tmp;
    return compute_jet_eta_phi( jet , tracks , option , tmp , phi );
  }
  virtual const bool compute_jet_eta( const boost::shared_ptr<const AnaJet>& jet , 
                                      const std::vector< boost::shared_ptr<const AnaTrack> >& tracks ,
                                      const JetCentroidOption& option , double& eta ) const {
    double tmp;
    return compute_jet_eta_phi( jet , tracks , option , eta , tmp );
  }
  // merging
  virtual JetTagger* operator+=( const JetTagger* rhs ) {
    assert( _name == rhs->_name );
    assert( _jet_centroid_option = rhs->_jet_centroid_option );
    return this;
  }

protected:
  friend class AnaTaggerMgr;
  virtual JetTagger* clone() const = 0;

};

BOOST_SERIALIZATION_ASSUME_ABSTRACT( JetTagger );
BOOST_CLASS_VERSION( JetTagger , 1 );

#endif // WRAP_JETTAGGER_HPP
