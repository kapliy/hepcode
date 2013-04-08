 
#ifndef GOODRUNLIST_HPP
#define GOODRUNLIST_HPP

// class to parse the good run xml file(s) for one or more good run
// lists and provide user code an easy way to check whether or not a
// given run+lumi_block is included in a list.

#include <string>
#include <iostream>
#include <map>
#include <utility>
#include <boost/unordered_map.hpp>

class
LumiBlockRange
{
public:
  typedef int RunNumber;
  typedef int LumiBlock;
protected:
  RunNumber _run;
  LumiBlock _lb_first;
  LumiBlock _lb_last;
public:
  LumiBlockRange() : _run(0), _lb_first(0), _lb_last(0) {}
  LumiBlockRange( const RunNumber& run , const LumiBlock& first_lumi_block , const LumiBlock& last_lumi_block )
    : _run(run)
    , _lb_first(first_lumi_block)
    , _lb_last(last_lumi_block)
  {}

  const LumiBlock& first_lumi_block() const { return _lb_first; }
  const LumiBlock& last_lumi_block() const { return _lb_last; }
  const LumiBlock& iblock() const { return _lb_first; }
  const LumiBlock& fblock() const { return _lb_last; }
  
  const bool is_in( const int& run , const LumiBlock& lb ) const {
    return( run==_run && lb>=_lb_first && lb<=_lb_last );
  }
  
  std::ostream& print( std::ostream& os ) const;
};

std::ostream& operator<<( std::ostream& os , const LumiBlockRange& r );

// ================================================================

class
GoodRunList
{
public:
  typedef LumiBlockRange::RunNumber RunNumber;
  typedef LumiBlockRange::LumiBlock LumiBlock;
private:
  typedef std::map<LumiBlock,LumiBlockRange> LumiBlockSequence;
  typedef boost::unordered_map<RunNumber,LumiBlockSequence> RunMap;
  typedef boost::unordered_map<std::string,RunMap> RunLists;
  RunLists _lists;
public:
  GoodRunList() {}
  virtual ~GoodRunList() {}

  // adds a single GRL file
  const bool add( const std::string& list_name , const std::string& xml_filename );
  // adds a collection of globbed GRL files
  const bool add_multiple( const std::string& list_name , const std::string& xml_pattern );

  const bool is_in( const std::string& list_name , 
                    const RunNumber& run , 
                    const LumiBlock& lumi_block ) const;

  std::ostream& print( const std::string& list_name , std::ostream& os ) const;
  
public:
  // some preset labels are defined here for user convenience.
  static const std::string ELECTRON;
  static const std::string MUON;
  static const std::string JETMET;
  static const std::string GAMMAJET;
};

#endif // GOODRUNLIST_HPP
