#ifndef WRAP_PARSE_ARCHIVE_EXCEPTION_HPP
#define WRAP_PARSE_ARCHIVE_EXCEPTION_HPP

#include <iostream>
#include <stdexcept>
#include <boost/archive/archive_exception.hpp>

namespace
parse_archive_exception
{
  void parse( boost::archive::archive_exception& e ) {
    std::cout << e.code << std::endl;
    std::cout << e.what() << std::endl;
  };
};

#endif // WRAP_PARSE_ARCHIVE_EXCEPTION_HPP
