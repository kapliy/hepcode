#ifndef WRAP_ANATAGGERMGR_HH
#define WRAP_ANATAGGERMGR_HH

#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/serialization/map.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/config.hpp>
#include BOOST_HASH_MAP_HEADER
#include <boost/functional/hash.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/hash_map.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include "TrigFTKAna/JetTagger.hpp"
#include "TrigFTKAna/SignedD0JetTagger.hpp"
#include "TrigFTKAna/SignedD0ParameterizedJetTagger.hpp"
#include "TrigFTKAna/AnaJet.hpp"
#include "TrigFTKAna/AnaTrack.hpp"
#include "TH1F.h"

class DgCollection;

class
AnaTaggerMgr
{

public:

  typedef BOOST_STD_EXTENSION_NAMESPACE::hash_map<std::string,boost::shared_ptr<JetTagger>,boost::hash<std::string> > Coll;
  typedef Coll::const_iterator const_iterator;

private:

  Coll _coll;

private:

  friend class boost::serialization::access;
  template<typename archiveT>
  void serialize( archiveT& ar , const unsigned int version ) {
    ar.register_type(static_cast<SignedD0JetTagger*>(0));
    ar.register_type(static_cast<SignedD0ParameterizedJetTagger*>(0));
    ar & _coll;
  }

public:

  AnaTaggerMgr() {}
  virtual ~AnaTaggerMgr() {}

  template<typename jetTaggerT>
  boost::shared_ptr<jetTaggerT> add( const std::string& name ) {
    boost::shared_ptr<jetTaggerT> result( new jetTaggerT() );
    result->name( name );
    result->use_sv_variables( boost::algorithm::icontains( name , "sv1" ) );
    result->use_d0( boost::algorithm::icontains( name , "d0" ) );
    result->use_z0( boost::algorithm::icontains( name , "z0" ) , true );
    result->use_sign( boost::algorithm::icontains( name , "signed" ) );
    result->use_significance( boost::algorithm::icontains( name , "signif" ) );
    result->use_track_jet_direction( boost::algorithm::icontains( name , "trackcone" ) );
    result->use_impure_tracks( boost::algorithm::icontains( name , "purity" ) );
    _coll.insert( Coll::value_type(name,result) );
    return result;
  }
  template<typename jetTaggerT>
  boost::shared_ptr<jetTaggerT> add( const std::string& name , DgCollection* dgcoll ) {
    boost::shared_ptr<jetTaggerT> result( new jetTaggerT(dgcoll) );
    result->name( name );
    result->use_sv_variables( boost::algorithm::icontains( name , "sv1" ) );
    result->use_d0( boost::algorithm::icontains( name , "d0" ) );
    result->use_z0( boost::algorithm::icontains( name , "z0" ) , true );
    result->use_sign( boost::algorithm::icontains( name , "signed" ) );
    result->use_significance( boost::algorithm::icontains( name , "signif" ) );
    result->use_track_jet_direction( boost::algorithm::icontains( name , "trackcone" ) );
    result->use_impure_tracks( boost::algorithm::icontains( name , "purity" ) );
    _coll.insert( Coll::value_type(name,result) );
    return result;
  }
  template<typename jetTaggerT, typename jetParameterizedTaggerT>
  boost::shared_ptr<jetParameterizedTaggerT> add_parameterization( const std::string& name ) {
    boost::shared_ptr<jetParameterizedTaggerT> result( new jetParameterizedTaggerT );
    result->use_sign( boost::algorithm::icontains( name , "signed" ) );
    result->use_significance( boost::algorithm::icontains( name , "signif" ) );
    result->use_track_jet_direction( boost::algorithm::icontains( name , "trackcone" ) );
    //if( boost::algorithm::icontains( name , "purity" ) ) { result->use_impure_tracks( true ); } else { result->use_impure_tracks( false ); }
    const string new_name = name + "_PARAMETERIZED";
    result->name( new_name );
    const boost::shared_ptr<JetTagger>& binned_tagger( get_tagger(name) );
    if( binned_tagger && dynamic_cast<const jetTaggerT*>(binned_tagger.get()) ) { 
      result->fit( *dynamic_cast<const jetTaggerT*>(binned_tagger.get()) );
      _coll.insert( Coll::value_type(new_name,result) );
    }
    return result;
  }
  template<typename jetTaggerT, typename jetParameterizedTaggerT>
  boost::shared_ptr<jetParameterizedTaggerT> add_parameterization( const std::string& name , DgCollection* dgcoll ) {
    boost::shared_ptr<jetParameterizedTaggerT> result( new jetParameterizedTaggerT(dgcoll) );
    result->use_sign( boost::algorithm::icontains( name , "signed" ) );
    result->use_significance( boost::algorithm::icontains( name , "signif" ) );
    result->use_track_jet_direction( boost::algorithm::icontains( name , "trackcone" ) );
    //if( boost::algorithm::icontains( name , "purity" ) ) { result->use_impure_tracks( true ); } else { result->use_impure_tracks( false ); }
    const string new_name = name + "_PARAMETERIZED";
    result->name( new_name );
    const boost::shared_ptr<JetTagger>& binned_tagger( dynamic_cast<const jetTaggerT>(get_tagger(name)) );
    if( binned_tagger && dynamic_cast<const jetTaggerT*>(binned_tagger.get()) ) { 
      result->fit( *dynamic_cast<const jetTaggerT*>(binned_tagger.get()) );
      _coll.insert( Coll::value_type(new_name,result) );
    }
    return result;
  }
  
  const_iterator begin() const { return _coll.begin(); }
  const_iterator end() const { return _coll.end(); }
  const std::string& get_name( const_iterator i ) const { return i->first; }
  const boost::shared_ptr<JetTagger>& get_tagger( const_iterator i ) const { return i->second; }
  const boost::shared_ptr<JetTagger>& get_tagger( const std::string& name ) const { 
    const_iterator i = _coll.find(name);
    if( i==_coll.end() ) {
      static boost::shared_ptr<JetTagger> empty;
      return empty;
    }
    return i->second;
  }

  void train( const std::string& tagger_name ,  
              const boost::shared_ptr<const AnaJet>& jet , 
              const std::vector< boost::shared_ptr<const AnaTrack> >& tracks , 
              const std::string dgcoll = std::string() ) {
    Coll::iterator i=_coll.find(tagger_name);
    assert( i!=_coll.end() );
    i->second->train( jet , tracks , dgcoll );
  }
  const double tag( const std::string& tagger_name , 
                    const boost::shared_ptr<const AnaJet>& jet , 
                    const std::vector< boost::shared_ptr<const AnaTrack> >& tracks , 
                    const std::string dgcoll = std::string() ) {
    Coll::iterator i=_coll.find(tagger_name);
    assert( i!=_coll.end() );
    return i->second->tag( jet , tracks , dgcoll );
  }

  const bool save( const std::string& filename ) {
    ofstream outf( filename.c_str() );
    if( !outf ) { return false; }
    boost::archive::text_oarchive oar( outf );
    oar << (*this);
    //boost::archive::xml_oarchive oar( outf );
    //oar << tagger_mgr;
    return true;
  }
  
  const bool load( const std::string& filename ) {
    ifstream inf( filename.c_str() );
    if( !inf ) { return false; }
    boost::archive::text_iarchive oar( inf );
    oar >> (*this);
    //boost::archive::xml_iarchive iar( inf );
    //iar >> tagger_mgr;
    return true;
  }

  // display
  TH1F* likelihood( const std::string& tagger_name , const bool get_b ) const {
    Coll::const_iterator i=_coll.find(tagger_name);
    if( i==_coll.end() ) { return 0; }
    const SignedD0JetTagger* tagger = dynamic_cast<const SignedD0JetTagger*>(i->second.get());
    if( !tagger ) { return 0; }
    if( get_b ) { return tagger->get_th1_b_d0_likelihood(); }
    return tagger->get_th1_notb_d0_likelihood();
  }
  
  // merging
  AnaTaggerMgr& operator+=( const AnaTaggerMgr& rhs ) {
    BOOST_FOREACH( const Coll::value_type pr , rhs._coll ) {
      Coll::iterator i( _coll.find(pr.first) );
      if( i != _coll.end() ) { // merge
        i->second.get()->operator+=( pr.second.get() );
      } else { // new
        _coll[ pr.first ] = boost::shared_ptr<JetTagger>( pr.second->clone() );
      }
    }
  }
  
};

BOOST_CLASS_VERSION( AnaTaggerMgr , 1 );

#endif // WRAP_ANATAGGERMGR_HH
