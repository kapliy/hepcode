
// ANA STREAMS
// ================================================================
// boost::iostreams::source and sink classes for files accessible with
// libdcap (dCache) and/or gzip, bzip2.
// ================================================================
// 2010-07-02 Enabled maximum compression for gzip and bzip2 output. renamed to "ana_streams"
// 2009-06-18 Added seek operations.
// 2009-05-29 Antonio Boveia (boveia@hep.uchicago.edu)

#include "TrigFTKAna_config.h"
#include "TrigFTKAna/ana_streams.hpp" // for HAVE_BOOST_IOSTREAMS_AND_LIBDCAP

#include <algorithm>
#include <string>
#include <iostream>
#include <ios>
#include <csignal>
#include <boost/version.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/interprocess/sync/file_lock.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#ifdef HAVE_BOOST_IOSTREAMS
#include <boost/array.hpp>
#include <boost/filesystem.hpp> 
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filter/bzip2.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/algorithm/string/trim.hpp>
#ifdef HAVE_LIBDCAP
#include <dcap.h>
#else
#include <boost/iostreams/device/file.hpp>
#endif
#endif
#include <TEnv.h>
#include <TSystem.h>
#include <TFile.h>

using namespace std;
using namespace ana_streams;

extern TSystem* gSystem;
extern TEnv* gEnv;

#ifdef HAVE_BOOST_IOSTREAMS_AND_LIBDCAP

ana_streams::ana_streams_source::ana_streams_source( const std::string& filename )
  :
  _filename( filename )
  , _fd( -1 )
  , _pos( 0 )
  , _debug( false )
{
}

ana_streams::ana_streams_source::~ana_streams_source()
{
  if( _fd>=0 ) {
    dc_close( _fd );
  }
}

const bool
ana_streams::ana_streams_source::open()
{
  if( _debug ) { 
    dc_setDebugLevel(2); 
  }
  // dc_setDebugLevel(32);
  // check that file exists and is readable
  if( dc_access( _filename.c_str() , R_OK ) != 0 ) { 
    if( _debug ) { cerr << " ana_streams_source: file " << _filename << " does not exist or is unreadable." << endl; }
    return false;
  } 
  // set large TCP buffers (32MB).
  dc_setTCPSendBuffer( 33554432u );
  dc_setTCPReceiveBuffer( 33554432u );
  // open the file read-only
  _fd = dc_open_safe( _filename.c_str() , O_RDONLY , S_IRUSR );
  // increase read-ahead buffer to 32MB to reduce load on server from frequency dc_read calls.
  dc_setBufferSize(_fd,33554432u);
  if( _fd < 0 ) { 
    if( _debug ) { cerr << " ana_streams_source: file " << _filename << " cannot be opened." << endl; }
    _fd = -1;
    return false;
  }
  if( _debug ) { cerr << " ana_streams_source: file " << _filename << " is open." << endl; }
  _pos = 0;
  return true;
}

// move read position
boost::iostreams::stream_offset
ana_streams::ana_streams_source::seek( boost::iostreams::stream_offset off ,
                                       std::ios_base::seekdir dir )
{
  if( !is_open() ) { 
    if( !open() ) { return -1; }
  }
  boost::iostreams::stream_offset result;
  if( dir == ios::beg ) {
    result = dc_lseek( _fd , off , SEEK_SET );
  } else if( dir == ios::cur ) {
    result = dc_lseek( _fd , off , SEEK_CUR );
  } else if( dir == ios::end ) {
    result = dc_lseek( _fd , off , SEEK_END );
  } else {
    throw ios::failure("bad seek direction");
  }
  _pos = result;
  return result;
}

// read up to n characters from the underlying data source into the
// result buffer, returning the number of characters read; return -1
// to indicate EOF.
std::streamsize
ana_streams::ana_streams_source::read( char* result , std::streamsize nmax ) 
{
  // dc_setDebugLevel(32);
  if( !is_open() ) { 
    if( !open() ) { return -1; }
  }
  std::streamsize n = dc_read( _fd , result , nmax );
  // dc_setDebugLevel(0);
  return n;
}



ana_streams::ana_streams_sink::ana_streams_sink( const std::string& filename )
  : _filename( filename )
  , _fd( -1 )
  , _pos( 0 )
  , _debug( false )
{
}

ana_streams::ana_streams_sink::~ana_streams_sink()
{
  if( _fd>=0 ) {
    dc_close( _fd );
  }
}

const bool
ana_streams::ana_streams_sink::open()
{
  if( _debug ) { 
    dc_setDebugLevel(2); 
  }
  // try to delete the file if it exists
  if( dc_access( _filename.c_str() , R_OK ) == 0 ) { 
    if( _debug ) { cerr << " ana_streams_sink: file " << _filename << " already exists. deleting it." << endl; }
    if( dc_access( _filename.c_str() , W_OK ) != 0 ) { 
      if( _debug ) { cerr << " ana_streams_sink: cannot delete " << _filename << " (read only)." << endl; }
      return false;
    }
    dc_unlink( _filename.c_str() );
    if( dc_access( _filename.c_str() , R_OK ) == 0 ) { 
      if( _debug ) { cerr << " ana_streams_sink: file " << _filename << " still exists after attempt to delete it." << endl; }
      return false;
    }
  }
  // open the file write-only
  _fd = dc_open_safe( _filename.c_str() , O_WRONLY | O_CREAT | O_TRUNC , 0x1A4 );
  if( _fd < 0 ) { 
    if( _debug ) { cerr << " ana_streams_sink: file " << _filename << " cannot be opened." << endl; }
    _fd = -1;
    return false;
  }
  if( _debug ) { cerr << " ana_streams_sink: file " << _filename << " is open." << endl; }
  _pos = 0;
  return true;
}

// move write position
boost::iostreams::stream_offset
ana_streams::ana_streams_sink::seek( boost::iostreams::stream_offset off ,
                                 std::ios_base::seekdir dir )
{
  if( !is_open() ) { 
    if( !open() ) { return -1; }
  }
  boost::iostreams::stream_offset result;
  if( dir == ios::beg ) {
    result = dc_lseek( _fd , off , SEEK_SET );
  } else if( dir == ios::cur ) {
    result = dc_lseek( _fd , off , SEEK_CUR );
  } else if( dir == ios::end ) {
    result = dc_lseek( _fd , off , SEEK_END );
  } else {
    throw ios::failure("bad seek direction");
  }
  _pos = result;
  return result;
}

// write up to n characters from result into the underlying data sink
// returning the number of characters written.
std::streamsize
ana_streams::ana_streams_sink::write( const char* result , std::streamsize nmax ) 
{
  if( !is_open() ) { 
    if( !open() ) { return -1; }
  }
  const std::streamsize n_to_write( nmax );
  return( dc_write( _fd , result , n_to_write ) );
}

boost::shared_ptr<ana_streams::istream>
ana_streams::open_for_read( const std::string& filename )
{
  boost::shared_ptr<ana_streams::istream> inf( new boost::iostreams::filtering_istream );
  if( inf ) { 
    const bool ok = open_for_read( filename , inf );
    if( !ok || !inf || !(*inf) ) { inf.reset(); }
  }
  return inf;  
}

const bool
ana_streams::open_for_read( const std::string& filename , boost::shared_ptr<ana_streams::istream>& inf )
{
  if( !inf ) { return false; }
  inf->reset();
  //
  //std::string basefilename = filename;
  // Remove grid-generated numbers like .bz2.1
  // std::vector<std::string> splitresult;
  // boost::algorithm::split( splitresult, filename,
  //                          boost::algorithm::is_any_of(".") );
  // std::string laststr( splitresult.back() );
  // if( splitresult.size() && laststr.size() &&
  //     boost::algorithm::all( laststr, boost::algorithm::is_digit() ) ) {
  //   basefilename = filename.substr(0, filename.size() - laststr.size() - 1);
  // }
  boost::filesystem::path ipath( filename );
  // check the filename extension and turn any decompression that is needed.
#if BOOST_VERSION>=104800
  if( boost::algorithm::icontains( ipath.filename().string() , ".bz2" ) ) {    
#else
  if( boost::algorithm::icontains( ipath.filename() , ".bz2" ) ) {
#endif
    // note that the filtering stream crashes in the destructor if
    // bzip2_decompressor attached but never used. see bug
    //  https://svn.boost.org/trac/boost/attachment/ticket/3348
    // configure large (32MB) read buffer to reduce frequency of dcache dc_read 
    // calls, which incur massive performance penalties by overloading
    // the dcache server.
    inf->push( boost::iostreams::bzip2_decompressor(false,33554432u) , 33554432u , 33554432u );
#if BOOST_VERSION>=104800
  } else if( boost::algorithm::icontains( ipath.filename().string() , ".gz" ) ) {
#else
  } else if( boost::algorithm::icontains( ipath.filename() , ".gz" ) ) {
#endif
    inf->push( boost::iostreams::gzip_decompressor(15,33554432u) , 33554432u , 33554432u ); 
  }
  inf->push( ana_streams_source(filename) , 33554432u , 33554432u ); // open the file with a 32MB buffer.
  return true;
}


boost::shared_ptr<ana_streams::ostream>
ana_streams::open_for_write( const std::string& filename )
{
  boost::shared_ptr<boost::iostreams::filtering_ostream> outf( new boost::iostreams::filtering_ostream );
  if( outf ) { 
    const bool ok = open_for_write( filename , outf );
    if( !ok ) { outf.reset(); }
  }
  return outf;  
}

const bool
ana_streams::open_for_write( const std::string& filename , boost::shared_ptr<ana_streams::ostream>& outf )
{
  outf->reset();
  // check the filename extension and turn any compression that is needed.
  if( boost::algorithm::iends_with(filename,".gz") ) { 
    // passing parameters to gzip is not implemented in boost 1.33 / Athena 13.0.40
    // this is ok in newer versions of Athena, e.g. 14.2.25.10.
    boost::iostreams::gzip_params params;
    params.level = 9;
    outf->push( boost::iostreams::gzip_compressor(params) ); 
  } else if( boost::algorithm::iends_with(filename,".bz2") ) { 
    boost::iostreams::bzip2_params params;
    params.block_size = 9;
    outf->push( boost::iostreams::bzip2_compressor(params) ); 
  }
  outf->push( ana_streams_sink(filename) ); // open the file
  return true;
}

// an approach suggested by Charles Waldman. avoid stalls on dc_open
// when server process is slow to respond.
const int
ana_streams::dc_open_safe( const char *filename , const int& oflag , const mode_t& mode )
{
  int fd = -1;
  dc_setOpenTimeout( 20 );
  for( int retry=0; retry!=10; ++retry ) {
    fd = dc_open(filename,oflag,mode);
    if( fd >= 0 ) { break; }
    // else failed.
    cout << "Failed to open file " << filename << " (attempt #" << retry << ")" << endl;
    sleep( 10 );
  }
  return fd;
}


#else // HAVE_BOOST_IOSTREAMS_AND_DCAP is not defined
#ifdef HAVE_BOOST_IOSTREAMS


boost::shared_ptr<ana_streams::istream>
ana_streams::open_for_read( const std::string& filename )
{
  boost::shared_ptr<boost::iostreams::filtering_istream> inf( new boost::iostreams::filtering_istream );
  if( inf ) { 
    const bool ok = open_for_read( filename , inf );
    if( !ok || !(*inf) ) { inf.reset(); }
  }
  return inf;  
}

const bool
ana_streams::open_for_read( const std::string& filename , boost::shared_ptr<ana_streams::istream>& inf )
{
  inf->reset();
  std::string basefilename = filename;
  // Remove grid-generated numbers like .bz2.1
  std::vector<std::string> splitresult;
  boost::algorithm::split( splitresult, filename, 
                           boost::algorithm::is_any_of(".") );
  std::string laststr( splitresult.back() );
  if( splitresult.size() && laststr.size() &&
      boost::algorithm::all( laststr, boost::algorithm::is_digit() ) ) {
    basefilename = filename.substr(0, filename.size() - laststr.size() - 1);
  }
  // check the filename extension and turn any decompression that is needed.
  if( boost::algorithm::iends_with(basefilename,".gz") ) { inf->push( boost::iostreams::gzip_decompressor() ); }
  else if( boost::algorithm::iends_with(basefilename,".bz2") ) { inf->push( boost::iostreams::bzip2_decompressor() ); }
  boost::iostreams::file_source ifile(filename); // open the file
  if (!ifile.is_open()) return false;

  inf->push(ifile); // push the file in the chain
  
  return true;
}

boost::shared_ptr<ana_streams::ostream>
ana_streams::open_for_write( const std::string& filename )
{
  boost::shared_ptr<boost::iostreams::filtering_ostream> outf( new boost::iostreams::filtering_ostream );
  if( outf ) { 
    const bool ok = open_for_write( filename , outf );
    if( !ok || !(*outf) ) { outf.reset(); }
  }
  return outf;  
}

const bool
ana_streams::open_for_write( const std::string& filename , boost::shared_ptr<ana_streams::ostream>& outf )
{
  outf->reset();
  // check the filename extension and turn any compression that is needed.
  if( boost::algorithm::iends_with(filename,".gz") ) { 
    // passing parameters to gzip is not implemented in boost 1.33 / Athena 13.0.40
    //     boost::iostreams::gzip::gzip_params params;
    //     params.level = 9;
    //     outf->push( boost::iostreams::gzip_compressor(params) ); 
    outf->push( boost::iostreams::gzip_compressor() ); 
  } else if( boost::algorithm::iends_with(filename,".bz2") ) { 
    outf->push( boost::iostreams::bzip2_compressor() ); 
  }
  outf->push( boost::iostreams::file_sink(filename) ); // open the file
  return true;
}

const std::string
ana_streams::root_dcap_filename( const std::string& filename )
{

  if( !boost::istarts_with( filename , "/pnfs" ) ) { return filename; }
  if( boost::istarts_with( filename , "dcap://" ) || boost::istarts_with( filename , "dcache:/" ) ) { return filename; }
  std::string result( filename );
  boost::replace_first( result , "/pnfs" , "dcache:/pnfs" );
  return result;
}

#else

boost::shared_ptr<ana_streams::istream>
ana_streams::open_for_read( const std::string& filename ) 
{
  return( boost::make_shared<ana_streams::istream>( filename.c_str() ) );
}


ana_streams::ostream* 
ana_streams::open_for_write( const std::string& filename )
{
  return( boost::make_shared<ana_streams::ostream>( filename.c_str() ) );
}

const bool
ana_streams::open_for_read( const std::string& filename , boost::shared_ptr<ana_streams::istream>& inf ) 
{
  if( inf->is_open() ) { inf->close(); }
  inf->clear();
  try {
     inf->open( filename.c_str() );
  } catch(...) {
    cout << " open file " << filename << " failed." << endl;
    return false;
  }
  return true;
}

const bool
ana_streams::open_for_write( const std::string& filename , boost::shared_ptr<ana_streams::ostream>& outf )
{
  outf->clear();
  try {
    outf->open( filename.c_str() );
  } catch(...) { 
    return false; 
  }
  return true;
}

#endif
#endif // end if / else HAVE_BOOST_IOSTREAMS_AND_DCAP

const std::string
ana_streams::root_dcap_filename( const std::string& filename )
{
  // update 20110314: xrootd now preferred for ROOT file /pnfs/uchicago.edu access.
  if( boost::istarts_with( filename , "root://" ) ) { return filename; }
  if( boost::contains( filename , "/pnfs/uchicago.edu" ) && boost::icontains( filename , ".root" ) ) {
    string xrootname( filename );
    {  // new version, via instructions on: http://twiki.mwt2.org/bin/view/UCTier3/UsingXrootDTier3
      static const std::string gate1 = "root://xrddc.mwt2.org:1096//atlas/dq2";
      static const std::string gate2 = "root://xrd-central.usatlasfacility.org//atlas/dq2";
      static const std::string gate3 = "root://uct2-grid5.uchicago.edu//atlas/dq2";
      const std::string gate = gate3;
      static const std::string patt = "/user/";
      size_t pos = filename.find(patt);
      if(false && pos!=std::string::npos) { // if this is a user dataset, try to establish Federated access
	const std::string TBASE = filename.substr(pos);
	return gate+TBASE;
      } else {                     // else revert to regular xrootd gate
	boost::replace_regex( xrootname , boost::regex(".*/pnfs/uchicago.edu") , string("root://xrddc.mwt2.org:1096/pnfs/uchicago.edu") );
      }
    }
    // cout << "accessing file via xrootd: " << xrootname << endl;
    return xrootname;
  }
  // update 20110504: update old /pnfs/uct3 to new xrootd access
  if( boost::contains( filename , "/pnfs/uct3" ) && boost::icontains( filename , ".root" ) ) {
    string xrootname( filename );
    boost::replace_regex( xrootname , boost::regex(".*/pnfs/uct3") , string("root://uct3-xrd.mwt2.org//atlas/uct3") );
    // cout << "accessing file via xrootd: " << xrootname << endl;
    return xrootname;
  }
  // update 20111017: update  /atlas/ to explicit xrootd access (as opposed to VFS)
  if( boost::starts_with( filename , "/atlas" ) && boost::icontains( filename , ".root" ) ) {
    string xrootname( filename );
    boost::replace_first( xrootname , "/atlas" , string("root://uct3-xrd.mwt2.org//atlas") );
    // append in the end to control cache size: ?cachesz=2000000 ?readaheadsz=100000
    // cout << "accessing file via xrootd: " << xrootname << endl;
    return xrootname;
  }
  
  // fallback to dcache access
  if( boost::istarts_with( filename , "dcap://" ) || boost::istarts_with( filename , "dcache:/" ) ) { return filename; }
  if( boost::contains( filename , "/pnfs" ) && boost::icontains( filename , ".root" ) ) {
    std::string result( filename );
    boost::replace_first( result , "/pnfs" , "dcache:/pnfs" );
    return result;
  }
  return filename;
}

TFile* 
ana_streams::open_root_file( const std::string& filename )
{
  if( string(boost::algorithm::trim_copy(filename))=="" ) { return 0; }

  const bool debug = false;
  TFile* result = 0;
  if( debug ) {
    if( gEnv ) { gEnv->Print(); }
  }
  const int MAXITERATIONS = 200;
  int iterations = 0;
  std::signal( SIGPIPE , SIG_IGN );
  while( !result && (iterations++)<MAXITERATIONS ) {
    // lock the file TrigFTKAna/.file_open_lock. this is a signal to all
    // other parallel jobs that we are about to attempt a file open, and
    // they should chill. the motivation for this (and the subsequent
    // random delay) is to reduce the peak load on dcache servers that
    // might otherwise get it with many file open requests
    // simultaneously.
    if( debug ) { 
      cout << "open_root_file: " << filename << " iteration " << iterations << endl; 
    }
    // choose sleep delay 50-1000 milliseconds
    int slp = std::rand();
    slp = slp < 50000 ? 50000 : (slp>1000000 ? 1000000 : slp);
    // attempt TFile::Open
    const bool USE_FILE_LOCK = false;
    if(USE_FILE_LOCK) { // use file locking to moderate the rate of TFile::Open requests
      try { 
	boost::interprocess::file_lock lock( "./.file_open_lock" );
	boost::interprocess::scoped_lock<boost::interprocess::file_lock> sc_lock( lock );
	if( gSystem ) { gSystem->IgnoreSignal(kSigPipe); }
	if( debug ) { 
	  if( gEnv ) { gEnv->SetValue("XNet.Debug",2); }
	  cout << " attempting to open file: " << filename << endl; 
	}
	result = TFile::Open( filename.c_str() );
	usleep(slp);
      } catch( ... ) {
	cerr << "WARNING: open_root_file failure for file: " << filename << endl;
	// do nothing and hope for the best.
	if( gSystem ) { gSystem->IgnoreSignal(kSigPipe); }
	if( debug ) { 
	  if( gEnv ) { gEnv->SetValue("XNet.Debug",2); }
	  cout << " attempting to open file: " << filename << endl; 
	}
	result = TFile::Open( filename.c_str() );
      }
    } else { // simply open the file
      result = TFile::Open( filename.c_str() );
      if(!result) {
	cerr << "WARNING: open_root_file failure for file: " << filename << endl;
	usleep(slp);
      }
    }
  } // end of TFile::Open attempts

  return result;
}
