#ifndef ANAUTILITY_H
#define ANAUTILITY_H

#include <iostream>
#include <algorithm>
#include <string>
#include <list>
#include <iterator>
#include <stdexcept>

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>

/* A conditional version of STL transform() */
template<class In,class Out,class Pred, class Op>
Out transform_if(In first,In last,Out res,Pred p,Op op)
{
  while(first!=last){
    if (p(*first)) *res = op(*first); ++first;++res; 
  }
  return res; 
}

/* A class with system utility functions */
class AnaUtility
{
private:
  class glob_match : public std::unary_function<boost::filesystem::directory_entry,bool> {
  private:
    const std::string _pattern;
  public:
    glob_match(std::string pattern) : _pattern(pattern),std::unary_function<boost::filesystem::directory_entry,bool>() {}
    bool operator()(const boost::filesystem::directory_entry& d) const {
      std::string fn(d.filename());
      return boost::regex_match(fn.begin(), fn.end(), boost::regex(_pattern));
    }
  };
public:
  AnaUtility() {}
  ~AnaUtility() {}
  // glob a list of files in path. Note that the glob pattern is only supported after the last slash
  static const std::list<std::string> glob(std::string path) {
    size_t found = path.rfind("/");
    if (found==std::string::npos) throw;
    return glob_folder(path.substr(0,found),path.substr(found+1,path.size()));
  }
  // glob within a particular folder
  static const std::list<std::string> glob_folder(std::string folder, std::string pattern) {
    std::list<std::string> out;
    transform_if(boost::filesystem::directory_iterator(folder), boost::filesystem::directory_iterator(),
		 back_inserter(out),
		 glob_match(pattern),
		 mem_fun_ref(&boost::filesystem::directory_entry::filename)
		 );
    return out;
  }
};

#endif
