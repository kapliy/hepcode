#ifndef WRAP_ANA_STREAMS_HELPER_HPP
#define WRAP_ANA_STREAMS_HELPER_HPP

// FTK DCAP 
// ================================================================
// see ana_streams.h for more information.
// ================================================================
// 2009-06-20 Antonio Boveia (boveia@hep.uchicago.edu)

#include <boost/shared_ptr.hpp>
#include "TrigFTKAna_config.h"

#ifndef HAVE_BOOST_IOSTREAMS_AND_LIBDCAP
#ifdef HAVE_BOOST_IOSTREAMS
#ifdef HAVE_LIBDCAP
#define HAVE_BOOST_IOSTREAMS_AND_LIBDCAP
#endif
#endif
#endif

#ifdef HAVE_BOOST_IOSTREAMS
#include <boost/iostreams/filtering_stream.hpp>
#else
#warning compiling against std::fstream instead of boost::iostreams::filtering_stream.
#include <fstream>
#endif

class TFile;

namespace 
ana_streams
{

#ifdef HAVE_BOOST_IOSTREAMS
  typedef boost::iostreams::filtering_istream istream;
  typedef boost::iostreams::filtering_ostream ostream;
#else 
  typedef std::ifstream istream;
  typedef std::ofstream ostream;
#endif

  boost::shared_ptr<ana_streams::istream> open_for_read( const std::string& filename );
  boost::shared_ptr<ana_streams::ostream> open_for_write( const std::string& filename );
  const bool open_for_read( const std::string& filename , boost::shared_ptr<ana_streams::istream>& );
  const bool open_for_write( const std::string& filename , boost::shared_ptr<ana_streams::ostream>& );

  const std::string root_dcap_filename( const std::string& filename );
  TFile* open_root_file( const std::string& filename );

};

#endif // WRAP_ANA_STREAMS_HELPER_HPP

