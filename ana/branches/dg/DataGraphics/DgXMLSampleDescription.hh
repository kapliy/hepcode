#ifndef WRAP_DGXMLSAMPLEDESCRIPTION_HPP
#define WRAP_DGXMLSAMPLEDESCRIPTION_HPP

#include <string>
#include <set>
#include <algorithm>
#include <iterator>
#include <utility>
#include <numeric>
#include <iostream>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/serialization/optional.hpp>
#include <boost/serialization/set.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/version.hpp>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <boost/foreach.hpp>
#include <boost/optional.hpp>
#include "DataGraphics/serialize_unordered.hpp"
#include "DataGraphics/DgSTL.hh"

class
DgXMLSampleDescription
{
public:
  typedef float ftype;
  typedef enum { UNKNOWN , DATA , MC } SampleType;
  typedef enum { NO_NORMALIZATION , ABSOLUTE_NORMALIZATION , NORMALIZATION_TO_DILEPTON_GENERATOR } NormalizationMode;
  typedef enum { NO_STREAM , EGAMMA_STREAM , MUONS_STREAM } StreamType;
  typedef enum { PARTIAL_SAMPLE , FULL_SAMPLE } SplitType;
  typedef enum { STREAM_VERSION=3 } VersionType;
private:
  typedef std::set<std::string> TagColl;
  typedef std::map<std::string,double> WeightedEventsColl;
private:
  std::string _name;
  std::string _legend;
  std::string _description;
  std::string _dg_filename;
  TagColl _tags;
  SampleType _type;
  boost::optional<StreamType> _stream;
  boost::optional<NormalizationMode> _normalization;
  boost::optional<ftype> _cross_section; // cross section in pb, for MC samples
  boost::optional<ftype> _luminosity; // luminosity in /pb, for data
  boost::optional<ftype> _weight; // optional sample re-weighting
  unsigned long _n_events_total; // total number of events processed
  WeightedEventsColl _weighted_nevents; // total number of events processed, reweighted
  SplitType _split_type; // full sample or only a piece of it?
  boost::optional<unsigned int> _n_splits; // number of splits, if partial sample
  boost::optional< std::set<unsigned int> > _this_split; // this split (out of _n_splits), if partial sample
private:
  friend class boost::serialization::access;
  template<class archiveT>
  void serialize( archiveT& ar, const unsigned int version )
  {
    ar & boost::serialization::make_nvp("name",_name);
    ar & boost::serialization::make_nvp("legend",_legend);
    ar & boost::serialization::make_nvp("description",_description);
    ar & boost::serialization::make_nvp("dg_filename",_dg_filename);
    ar & boost::serialization::make_nvp("tags",_tags);
    ar & boost::serialization::make_nvp("type",_type);
    ar & boost::serialization::make_nvp("stream",_stream);
    ar & boost::serialization::make_nvp("normalization",_normalization);
    ar & boost::serialization::make_nvp("cross_section",_cross_section);
    ar & boost::serialization::make_nvp("luminosity",_luminosity);
    ar & boost::serialization::make_nvp("weight",_weight);
    ar & boost::serialization::make_nvp("n_events_total",_n_events_total);
    if( version >= 3 ) { ar & boost::serialization::make_nvp("weighted_nevents",_weighted_nevents); }
    ar & boost::serialization::make_nvp("split_type",_split_type);
    ar & boost::serialization::make_nvp("n_splits",_n_splits);
    ar & boost::serialization::make_nvp("this_split",_this_split);
  }
public:
  DgXMLSampleDescription() 
    : _name()
    , _legend()
    , _description()
    , _dg_filename()
    , _tags()
    , _type( UNKNOWN )
    , _stream()
    , _normalization()
    , _cross_section()
    , _luminosity()
    , _weight()
    , _n_events_total( 0ul )
    , _weighted_nevents()
    , _split_type( FULL_SAMPLE )
    , _n_splits()
    , _this_split()
  {}
  DgXMLSampleDescription( const std::string& name , 
                          const std::string& legend , 
                          const std::string& description ) 
    : _name(name)
    , _legend(legend)
    , _description(description)
    , _dg_filename()
    , _tags()
    , _type( UNKNOWN )
    , _stream()
    , _normalization()
    , _cross_section()
    , _luminosity()
    , _weight()
    , _n_events_total( 0ul )
    , _weighted_nevents()
    , _split_type( FULL_SAMPLE )
    , _n_splits()
    , _this_split()
  {}
  virtual ~DgXMLSampleDescription() {}
  // ================================================================
  const std::string& name() const { return _name; }
  void set_name( const std::string& v ) { _name = v; }

  const std::string& legend() const { return _legend; }
  void set_legend( const std::string& v ) { _legend = v; }

  const std::string& description() const { return _description; }
  void set_description( const std::string& v ) { _description = v; }

  const std::string& dg_filename() const { return _dg_filename; }
  void set_dg_filename( const std::string& v ) { _dg_filename = v; }

  const std::set<std::string> tags() const { return _tags; }
  const bool has_tag( const std::string& tag ) const { return _tags.find(tag)!=_tags.end(); }
  void set_tag( const std::string& tag ) { _tags.insert(tag); }
  
  const SampleType& type() const { return _type; }
  void set_type( const SampleType& v ) { _type = v; }
  const bool is_mc() const { return _type==MC; }
  const bool is_data() const { return _type==DATA; }
  
  const StreamType stream() const { 
    if(is_data()) { assert(_stream); return( *_stream ); }
    return NO_STREAM;
  }
  void set_stream( const StreamType& v ) { _stream.reset( v ); _type = DATA; }

  const NormalizationMode normalization_mode() const { 
    if(is_mc()) { assert(_normalization); return( *_normalization ); }
    return NO_NORMALIZATION;
  }
  void set_normalization( const NormalizationMode& mode ) { _normalization.reset( mode ); } 

  const unsigned long num_events_total() const { return _n_events_total; }
  void set_num_events_total( const unsigned long& v ) { _n_events_total = v; }
  
  const double weighted_nevents( const std::string& tag ) const { 
    WeightedEventsColl::const_iterator i = _weighted_nevents.find(tag);
    if( i == _weighted_nevents.end() ) { return 0.; }
    return i->second;
  }
  void set_weighted_nevents( const std::string& tag , const double& v ) { _weighted_nevents[tag] = v; }

  const ftype cross_section() const { 
    if(is_mc()) { assert(_cross_section); return( *_cross_section ); }
    return 0.;
  }
  void set_cross_section( const ftype& v ) { _cross_section.reset( v ); _type = MC; } 

  const ftype luminosity() const { 
    if(is_data()) { assert(_luminosity); return( *_luminosity ); }
    return 0.;
  }
  void set_luminosity( const ftype& v ) { _luminosity.reset( v ); _type = DATA; } 

  const bool has_weight() const { return( _weight ? *_weight : 1. ); }
  const ftype weight() const { return( _weight ? *_weight : 1. ); }
  void set_weight( const ftype& v ) { _weight.reset( v ); } 

  //
  // full sample or only a piece of it?
  const SplitType split_type() const { return _split_type; }
  const unsigned int n_splits() const { return( _n_splits ? *_n_splits : 0 ); }
  const std::set<unsigned int> this_split() const { return( _this_split ? *_this_split : std::set<unsigned int>() ); }
  const bool has_split( const unsigned int& v ) const { return( _this_split ? _this_split->find(v)!=_this_split->end() : false ); }
  //
  void set_full_sample() { _split_type = FULL_SAMPLE; _n_splits.reset(); _this_split.reset(); }
  void set_partial_sample( const unsigned int& nsplits , const unsigned int& thissplit ) { 
    _split_type = PARTIAL_SAMPLE; 
    _n_splits.reset( nsplits ); 
    _this_split.reset( std::set<unsigned int>() );
    (*_this_split).insert( thissplit );
  }
  // check if all subjobs are present 
  const bool is_complete() const {
    if( ! _n_splits || (*_n_splits)==0 ) { return true; } // trivial case; no subjobs
    for( unsigned int ijob=0, fjob=(*_n_splits); ijob!=fjob; ++ijob ) {
      if( _this_split->find(ijob)==_this_split->end() ) { return false; } // missing this one
    }
    return true;
  }

  // print dataset name, tags, etc to terminal
  std::ostream& print( std::ostream& os ) const { 
    using std::endl;
    using boost::format;
    os << " ---------------------------------------------------------------- " << endl;
    os << format("  dataset %20s" ) % _name << endl;
    os << format("          [ description %40s ]" ) % _description << endl;
    os << format("          [ legend \"%20s\" ] " ) % _legend << endl;
    os << "  filename " << _dg_filename << endl;
    os << "  tags: ";
    BOOST_FOREACH( const std::string&tag , _tags ) {
      os << tag << " ";
    }
    os << endl;
    os << "  number of raw events (unweighted): " << _n_events_total << endl;
    BOOST_FOREACH( const WeightedEventsColl::value_type& v , _weighted_nevents ) {
      os << "   number of events (weighted for " << v.first << "): " << v.second << endl;
    }
    if( _stream ) { os << "  stream: " << (*_stream) << endl; }
    if( _normalization ) { os << "  normalization mode: " << (*_normalization) << endl; }
    if( _cross_section ) { os << "  cross_section: " << (*_cross_section) <<  " pb" << endl; }
    if( _luminosity ) { os << "  luminosity: " << (*_luminosity) << " pb^{-1}" << endl; }
    if( _weight ) { os << "  weight: " << (*_weight) << endl; }
    // print status (complete or incomplete; missing sets)
    if( ! _n_splits || (*_n_splits)==0 ) { 
      // trivial case; no subjobs
      os << " *** all data objects present *** " << endl;
    } else {
      unsigned int misses = 0u;
      for( unsigned int ijob=0, fjob=(*_n_splits); ijob!=fjob; ++ijob ) {
        if( _this_split->find(ijob)==_this_split->end() ) { 
          // missing this one
          ++misses;
          os << "     missing subjob " << ijob << endl;
        }
      }
      if( misses==0 ) { 
        os << " *** all data objects present *** " << endl;
      } else {
        os << " *** missing " << misses << " subjobs *** " << endl;
      }
    }
    return os;
  }


  // ================================================================

  const bool load_old_xml( const std::string& filename ) {
    boost::shared_ptr<std::istream> inf( boost::make_shared<std::ifstream>( filename.c_str() ) );
    if( !inf || !(*inf) ) { 
      std::cout << " WARNING: could not load xml sample description from filename " << filename << std::endl;
      return false; 
    }
    try {
      boost::shared_ptr<boost::archive::xml_iarchive> ari( new boost::archive::xml_iarchive(*inf) );
      VersionType version;
      (*ari) >> boost::serialization::make_nvp("version",version);
      if( version > STREAM_VERSION ) { 
        // newer version than this code; code is not prepared to handle it. (time to update to trunk?)
        std::cout << " version newer than current DataGraphics class version in filename " << filename << std::endl;
        return false;
      }
      (*ari) >> BOOST_SERIALIZATION_NVP(_name);
      (*ari) >> BOOST_SERIALIZATION_NVP(_legend);
      (*ari) >> BOOST_SERIALIZATION_NVP(_description);
      (*ari) >> BOOST_SERIALIZATION_NVP(_dg_filename);
      (*ari) >> BOOST_SERIALIZATION_NVP(_tags);
      (*ari) >> BOOST_SERIALIZATION_NVP(_type);
      (*ari) >> BOOST_SERIALIZATION_NVP(_stream);
      (*ari) >> BOOST_SERIALIZATION_NVP(_normalization);
      (*ari) >> BOOST_SERIALIZATION_NVP(_cross_section);
      (*ari) >> BOOST_SERIALIZATION_NVP(_luminosity);
      (*ari) >> BOOST_SERIALIZATION_NVP(_weight);
      (*ari) >> BOOST_SERIALIZATION_NVP(_n_events_total);
      if( version >= 3 ) { (*ari) >> BOOST_SERIALIZATION_NVP(_weighted_nevents); }
      (*ari) >> BOOST_SERIALIZATION_NVP(_split_type);
      (*ari) >> BOOST_SERIALIZATION_NVP(_n_splits);
      (*ari) >> BOOST_SERIALIZATION_NVP(_this_split);
      ari.reset(); // destroy the iarchive before the stream closes.
      inf.reset(); // inf passed by reference to xml_iarchive; it must be destroyed after archive.
    } catch( ... ) {
      std::cout << " WARNING: exception; could not load xml sample description from filename " << filename << std::endl;
      return false;
    }
    return true;
  }
  const bool load_xml( const std::string& filename ) {
    boost::shared_ptr<std::istream> inf( boost::make_shared<std::ifstream>( filename.c_str() ) );
    if( !inf || !(*inf) ) { 
      std::cout << " WARNING: could not load xml sample description from filename " << filename << std::endl;
      return false; 
    }
    try {
      boost::shared_ptr<boost::archive::xml_iarchive> ari( new boost::archive::xml_iarchive(*inf) );
      (*ari) >> boost::serialization::make_nvp("sample_description",*this);
      ari.reset(); // destroy the iarchive before the stream closes.
      inf.reset(); // inf passed by reference to xml_iarchive; it must be destroyed after archive.
    } catch( ... ) {
      std::cout << " WARNING: exception; could not load xml sample description from filename " << filename << std::endl;
      return false;
    }
    return true;
  }
  const bool merge( const DgXMLSampleDescription& rhs ) {
    if( _name != rhs._name ) { 
      _name += std::string(" && ") + rhs._name;
    }
    if( _description != rhs._description ) {
      _description += std::string(" || ") + rhs._description;
    }
    _dg_filename = "";
    if( _legend != rhs._legend ) {
      // diff and use common subset
      std::string h = dgSTL::common_head( _legend , rhs._legend );
      std::string t = dgSTL::common_tail( _legend , rhs._legend );
      _legend = h+"X"+t;
    }
    TagColl tmptags;
    std::set_union( _tags.begin() , _tags.end() , rhs._tags.begin() , rhs._tags.end() , std::inserter(tmptags,tmptags.begin()) );
    _tags.swap( tmptags );
    if( _type != rhs._type ) { std::cout << " could not merge ; types do not agree" << std::endl; return false; }
    if( (((bool)_stream) ^ ((bool)rhs._stream)) || (_stream && rhs._stream && *_stream != *rhs._stream) ) { 
      std::cout << " could not merge ; streams do not agree" << std::endl; 
      if( _stream && rhs._stream ) {
        std::cout << " stream: " << (*_stream) << " or " << ( *rhs._stream) << std::endl;
      }
      return false; 
    }
    if( ( (bool)_normalization ^ (bool)rhs._normalization) || (_normalization && rhs._normalization && 
                                                               *_normalization != *rhs._normalization) ) { 
      std::cout << " could not merge ; normalizations do not agree" << std::endl; 
      return false; 
    }
    if( ( (bool)_cross_section ^ (bool)rhs._cross_section) || (_cross_section && rhs._cross_section &&
                                                               *_cross_section != *rhs._cross_section) ) { 
      std::cout << " could not merge ; cross sections do not agree" << std::endl; 
      return false;
    }
    if( ((bool)_luminosity ^ (bool)rhs._luminosity) || (_luminosity && rhs._luminosity && 
                                                        (*_luminosity) != (*rhs._luminosity)) ) {
        std::cout << " could not merge ; luminosities are different" << std::endl; 
    }
    if( ((bool)_weight ^ (bool)rhs._weight) || (_weight && rhs._weight && *_weight != *rhs._weight) ) { 
      std::cout << " could not merge ; weights do not agree" << std::endl; 
      return false; 
    }
    _n_events_total += rhs._n_events_total;
    BOOST_FOREACH( const WeightedEventsColl::value_type& v , rhs._weighted_nevents ) {
      WeightedEventsColl::iterator i = _weighted_nevents.find( v.first );
      if( i != _weighted_nevents.end() ) {
        i->second += v.second;
      } else {
        _weighted_nevents[ v.first ] = v.second;
      }
    }
    if( _n_splits && (*_n_splits)>0 ) {
      if( ! rhs._n_splits || (*rhs._n_splits)!=(*_n_splits) ) { 
        std::cout << " could not merge ; number of subjobs does not agree" << std::endl; 
        return false; 
      }
      std::set<unsigned int> overlap;
      std::set_intersection( (*rhs._this_split).begin() , (*rhs._this_split).end() ,
                             (*_this_split).begin() , (*_this_split).end() ,
                             std::inserter(overlap,overlap.begin()) );
      if( ! overlap.empty() ) { 
        std::cout << " could not merge ; subjobs ";
        std::copy( overlap.begin() , overlap.end() , std::ostream_iterator<unsigned int>(std::cout, " ") );
        std::cout << " already present " << std::endl; 
        return false; 
      }
      std::set<unsigned int> all;
      std::set_union( (*rhs._this_split).begin() , (*rhs._this_split).end() ,
                      (*_this_split).begin() , (*_this_split).end() ,
                      std::inserter(all,all.begin()) );
      all.swap( *_this_split );
      if( is_complete() ) {
        // no longer split
        _n_splits.reset();
        _this_split.reset();
      }
    }
    return true;
  }
  const bool merge_old_xml( const std::string& filename ) {
    DgXMLSampleDescription rhs; 
    std::cout << " merging desc for " << filename << std::endl;
    const bool ok = rhs.load_old_xml( filename );
    if( !ok ) { return false; }
    return merge( rhs );
  }
  const bool merge_xml( const std::string& filename ) {
    DgXMLSampleDescription rhs; 
    std::cout << " merging desc for " << filename << std::endl;
    const bool ok = rhs.load_xml( filename );
    if( !ok ) { return false; }
    return merge( rhs );
  }
  void save_old_xml( const std::string& filename ) const {
    boost::shared_ptr<std::ostream> outf( boost::make_shared<std::ofstream>( filename.c_str() ) );
    if( !outf || !(*outf) ) { 
      std::cout << " WARNING: could not save xml sample description to filename " << filename << std::endl;
      return; 
    }
    try { 
      boost::shared_ptr<boost::archive::xml_oarchive> aro( new boost::archive::xml_oarchive(*outf) );
      VersionType version = STREAM_VERSION;
      (*aro) << boost::serialization::make_nvp("version",version);
      (*aro) << BOOST_SERIALIZATION_NVP(_name);
      (*aro) << BOOST_SERIALIZATION_NVP(_legend);
      (*aro) << BOOST_SERIALIZATION_NVP(_description);
      (*aro) << BOOST_SERIALIZATION_NVP(_dg_filename);
      (*aro) << BOOST_SERIALIZATION_NVP(_tags);
      (*aro) << BOOST_SERIALIZATION_NVP(_type);
      (*aro) << BOOST_SERIALIZATION_NVP(_stream);
      (*aro) << BOOST_SERIALIZATION_NVP(_normalization);
      (*aro) << BOOST_SERIALIZATION_NVP(_cross_section);
      (*aro) << BOOST_SERIALIZATION_NVP(_luminosity);
      (*aro) << BOOST_SERIALIZATION_NVP(_weight);
      (*aro) << BOOST_SERIALIZATION_NVP(_n_events_total);
      if( version>=3 ) { (*aro) << BOOST_SERIALIZATION_NVP(_weighted_nevents); }
      (*aro) << BOOST_SERIALIZATION_NVP(_split_type);
      (*aro) << BOOST_SERIALIZATION_NVP(_n_splits);
      (*aro) << BOOST_SERIALIZATION_NVP(_this_split);
      outf->flush();
      aro.reset(); // destroy the oarchive before the stream closes.
      outf.reset(); // outf passed by reference to xml_oarchive; it must be destroyed after archive
    } catch( ... ) {
      std::cout << " WARNING: exception; could not save xml sample description to filename " << filename << std::endl;
      return;
    }
  }
  void save_xml( const std::string& filename ) const {
    boost::shared_ptr<std::ostream> outf( boost::make_shared<std::ofstream>( filename.c_str() ) );
    if( !outf || !(*outf) ) { 
      std::cout << " WARNING: could not save xml sample description to filename " << filename << std::endl;
      return; 
    }
    try { 
      boost::shared_ptr<boost::archive::xml_oarchive> aro( new boost::archive::xml_oarchive(*outf) );
      (*aro) << boost::serialization::make_nvp("sample_description",*this);
      outf->flush();
      aro.reset(); // destroy the oarchive before the stream closes.
      outf.reset(); // outf passed by reference to xml_oarchive; it must be destroyed after archive
    } catch( ... ) {
      std::cout << " WARNING: exception; could not save xml sample description to filename " << filename << std::endl;
      return;
    }
  }

};

BOOST_CLASS_VERSION( DgXMLSampleDescription, 3 );

#endif // WRAP_DGXMLSAMPLEDESCRIPTION_HPP
