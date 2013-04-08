#ifndef WRAP_ANA_STREAMS_HPP
#define WRAP_ANA_STREAMS_HPP

// ANA STREAMS
// ================================================================
// boost::iostreams::source and sink classes for files accessible with
// libdcap (dCache) and/or gzip, bzip2.
// ================================================================
// 2010-07-02 renamed to "ana_streams"
// 2009-06-18 Added seek operations.
// 2009-05-29 Antonio Boveia (boveia@hep.uchicago.edu)

#include "TrigFTKAna/ana_streams_helper.hpp"

#include <iosfwd>
#include <string>
#include <fstream>
#ifdef HAVE_BOOST_IOSTREAMS
#include <boost/iostreams/concepts.hpp>
#include <boost/iostreams/categories.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#endif

namespace 
ana_streams
{

#ifdef HAVE_BOOST_IOSTREAMS
#ifdef HAVE_BOOST_IOSTREAMS_AND_LIBDCAP

  const int dc_open_safe( const char *filename , const int& oflag , const mode_t& mode );
#endif

  class
  ana_streams_source
  {
  public:
    typedef char char_type;
    typedef boost::iostreams::seekable_device_tag category;
    typedef boost::iostreams::stream_offset size_type;
  private:
    std::string _filename;
    int _fd;
    size_type _pos;
    bool _debug;
  public:
    ana_streams_source( const std::string& filename );
    virtual ~ana_streams_source();
    void debug() { _debug = true; }
    void nodebug() { _debug = false; }
    const bool open();
    const bool is_open() const { return _fd>=0; }
    std::streamsize read( char* s , std::streamsize n );
    std::streamsize write( const char* s , std::streamsize n ) { return 0; }
    boost::iostreams::stream_offset seek( boost::iostreams::stream_offset off ,
                                          std::ios_base::seekdir dir );
  };

  class
  ana_streams_sink
  {
  public:
    typedef char char_type;
    typedef boost::iostreams::seekable_device_tag category;
    typedef boost::iostreams::stream_offset size_type;
  private:
    std::string _filename;
    int _fd;
    size_type _pos;
    bool _debug;
  public:
    ana_streams_sink( const std::string& filename );
    virtual ~ana_streams_sink();
    void debug() { _debug = true; }
    void nodebug() { _debug = false; }
    const bool open();
    const bool is_open() const { return _fd>=0; }
    std::streamsize read( const char* s , std::streamsize n ) { return 0; }
    std::streamsize write( const char* s , std::streamsize n );
    boost::iostreams::stream_offset seek( boost::iostreams::stream_offset off ,
                                          std::ios_base::seekdir dir );
  };

#endif // end if HAVE_BOOST_IOSTREAMS_AND_DCAP

};

#endif // WRAP_ANA_STREAMS_HPP
