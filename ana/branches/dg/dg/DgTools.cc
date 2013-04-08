#include "DataGraphics/DgTools.hh"

#include <iostream>
#include <cassert>
#include <cstdlib>
#include <string>
#include <boost/version.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/date_time.hpp>
#include <boost/format.hpp>

using namespace std;
using namespace boost;
using namespace boost::posix_time;
using namespace boost::filesystem;
using namespace boost::algorithm;

const int MAX_MKDIR_ATTEMPTS = 10;

const std::string
DataGraphics::Tools::expand_filename_to_valid( std::string pattern )
{
  // substitute current date and/or time if desired
  if( ifind_first(pattern,"%DATE") || ifind_first(pattern,"%TIME") ) {
    // retrieve date and time strings in YYYYMMDD and HHMMSS format
    ptime now( second_clock::local_time() );
    string temp( to_iso_string(now) );
    vector<string> isonow;
    split( isonow , temp , is_any_of("T") );
    assert( isonow.size()==2 );
    // replace all occurences of %DATE and %TIME with their values as of now.
    boost::replace_all(pattern,"%DATE", isonow.front() );
    boost::replace_all(pattern,"%TIME", isonow.back() );
  }
  // filename should now be valid. don't want to overwrite an extant
  // file, so check whether or not pattern already exists. if it
  // does, append an integer (before the extension) that makes it unique.
  path unique_name( pattern );
  path unique_directory( pattern );
  unique_directory.remove_filename();
  // ensure the output directory exists. create it if it does not.
  for(int i=0;i<MAX_MKDIR_ATTEMPTS; i++) {
    try {
      create_directories( unique_directory );
    }
    catch ( filesystem_error x ) {
      if(i<MAX_MKDIR_ATTEMPTS-1) {
	std::cout << "WARNING: failed to create directory " << unique_directory << ": attempt " << i << std::endl;
	usleep( 100000);
	continue;
      }
      else { // ok, on last attempt, we must throw the exception
	std::cout << "ERROR: failed to create directory " << unique_directory << ": exhausted all attempts" << std::endl;
	throw;
      }
    }
  }
  unique_name = pattern;
  return unique_name.string();
}

const std::string
DataGraphics::Tools::expand_filename_to_valid_unique( std::string pattern )
{
  // substitute current date and/or time if desired
  if( ifind_first(pattern,"%DATE") || ifind_first(pattern,"%TIME") ) {
    // retrieve date and time strings in YYYYMMDD and HHMMSS format
    ptime now( second_clock::local_time() );
    string temp( to_iso_string(now) );
    vector<string> isonow;
    split( isonow , temp , is_any_of("T") );
    assert( isonow.size()==2 );
    // replace all occurences of %DATE and %TIME with their values as of now.
    boost::replace_all(pattern,"%DATE", isonow.front() );
    boost::replace_all(pattern,"%TIME", isonow.back() );
  }
  // filename should now be valid. don't want to overwrite an extant
  // file, so check whether or not pattern already exists. if it
  // does, append an integer (before the extension) that makes it unique.
  path unique_name( pattern );
  path unique_directory( pattern );
  unique_directory.remove_filename();
  // ensure the output directory exists. create it if it does not.
  for(int i=0;i<MAX_MKDIR_ATTEMPTS; i++) {
    try {
      create_directories( unique_directory );
    }
    catch ( filesystem_error x ) {
      if(i<MAX_MKDIR_ATTEMPTS-1) {
	std::cout << "WARNING: failed to create directory " << unique_directory << ": attempt " << i << std::endl;
	usleep( 100000);
	continue;
      }
      else { // ok, on last attempt, we must throw the exception
	std::cout << "ERROR: failed to create directory " << unique_directory << ": exhausted all attempts" << std::endl;
	throw;
      }
    }
  }
  unique_name = pattern;
  // append the integer if necessary.
  unsigned int version_number = 0;
  while( exists( unique_name ) ) {
    unique_name = pattern;
    ++version_number;
#if BOOST_VERSION>=104800
    string new_extension = "." + (format("%1d%2s") % version_number % extension(unique_name)).str();
    path raw_dir = unique_name.parent_path();
    path raw_name = unique_name.stem();
    if(version_number>1) raw_name = raw_name.stem();
    unique_name = path(raw_dir.string() + "/" + raw_name.string() + new_extension);
#else
    string new_extension = (format("%1d%2s") % version_number % extension(unique_name)).str();
    unique_name.replace_extension( new_extension );
#endif
    if(version_number>100) {
      std::cout << "WARNING: file version number exceeded 100: " << unique_name << std::endl;
      break;
    }
  }
  return unique_name.string();
}
