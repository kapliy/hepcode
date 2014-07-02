
#include <string>
#include <vector>
#include <boost/format.hpp>
#include <boost/foreach.hpp>
#include <iostream>
#include <algorithm>
#include <cassert>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/algorithm.hpp>
#include <libxml/parser.h>
#include <libxml/xmlmemory.h>
#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>
#include "TrigFTKAna/GoodRunList.hpp"

using namespace std;

// preset labels
const std::string GoodRunList::ELECTRON = "electrons";
const std::string GoodRunList::MUON = "muons";
const std::string GoodRunList::JETMET = "jets+met";
const std::string GoodRunList::GAMMAJET = "gamma+jet";

// uncomment below to compile in standalone mode:
//#define COMPILE_STANDALONE
// g++ -g -I/usr/include/libxml2 -I/usr/local/boost/include -o GoodRunList GoodRunList.cpp -lxml2

using namespace boost;
using namespace boost::lambda;

std::ostream&
LumiBlockRange::print( std::ostream& os ) const
{
  os << boost::format( "run %|7d| %|15t| lumi blocks: %|8d| -- %|8d|" ) % _run % _lb_first % _lb_last << std::endl;
  return os;
}

std::ostream& operator<<( std::ostream& os , const LumiBlockRange& r ) {
  r.print( os );
  return os;
}

const bool
GoodRunList::is_in( const std::string& list_name , const RunNumber& run , const LumiBlock& lumi_block ) const
{
  RunLists::const_iterator ilist = _lists.find( list_name );
  assert( ilist!=_lists.end() && "severe error: request for unrecognized good run list" );
  const RunMap& runmap( ilist->second );
  RunMap::const_iterator irun = runmap.find( run );
  if( irun == runmap.end() ) { return false; }
  const LumiBlockSequence& seq( irun->second );
  if( seq.empty() ) { return false; }
  // find first sequence that starts at lb equal or greater than "lumi_block"
  LumiBlockSequence::const_iterator iblock = seq.lower_bound( lumi_block );
  // possible cases: (here, lb=lumi_block)
  //   lb <= (sb1,se1) < (sb2,se2) < (sb3,se3)
  //   (sb1,se1) < lb(?<se1) <= (sb2,se2) < (sb3,se3)
  //   (sb2,se2) < lb(?<se2)
  if( iblock == seq.begin() ) {
    const LumiBlockRange& r( iblock->second );
    return( lumi_block>= r.iblock() && lumi_block <= r.fblock() );
  } else if( iblock != seq.end() ) {
    assert( iblock != seq.begin() );
    if( seq.find(lumi_block) != seq.end() ) {
      // special case: boundary
      return true;
    } else {
      const LumiBlockRange& r( (--iblock)->second );
      return( lumi_block>= r.iblock() && lumi_block <= r.fblock() );
    }
  } else { 
    assert( iblock == seq.end() );
    const LumiBlockRange& r( seq.rbegin()->second );
    return( lumi_block>= r.iblock() && lumi_block <= r.fblock() );
  }
  assert( !"unhandled case" );
  return false;
}

std::ostream&
GoodRunList::print( const 
std::string& list_name , std::ostream& os ) const
{
  RunLists::const_iterator ilist = _lists.find( list_name );
  if( ilist == _lists.end() ) { 
    os << " good run list " << list_name << " not found." << endl;
    return os;
  }
  const RunMap& runmap( ilist->second );
  os << boost::format( " === %|15s| %|26t| === " ) % list_name << endl;
  os << boost::format( " %|6s| %|3s| %|14s| === " ) % "run" % "lbn" % "range" << endl;
  BOOST_FOREACH( const RunMap::value_type& runpair , runmap ) {
    RunNumber run( runpair.first );
    const LumiBlockSequence& seq( runpair.second );
    BOOST_FOREACH( const LumiBlockSequence::value_type& r , seq ) {
      os << boost::format( " %|6d|     %|5d| -- %|5d|" ) % run % r.second.iblock() % r.second.fblock() << endl;
    }
  }
  os << boost::format( " === %|15s| end %|22t| === " ) % list_name << endl;
  return os;
}


const bool
GoodRunList::add( const std::string& list_name , const std::string& xml_filename )
{
  RunMap& runmap( _lists[list_name] );

  xmlDocPtr doc = xmlParseFile( xml_filename.c_str() );
  // scope exit does not play well with xmlFreeDoc. maybe xmlFreeDoc is a macro?
  // BOOST_SCOPE_EXIT(void) { 
  //   if( doc ) { xmlFreeDoc(doc); } 
  // } BOOST_SCOPE_EXIT_END;
  if( !doc ) {
    cout << "document not parsed successfully." << endl;
    return false;
  }
  xmlNodePtr cur = xmlDocGetRootElement( doc );
  if( !cur ) { 
    cout << "document not parsed successfully." << endl;
    if( doc ) { xmlFreeDoc(doc); } 
    return false;
  }
  // descend to NamedLumiRange
  if( xmlStrcmp(cur->name, (const xmlChar*)("LumiRangeCollection")) ) {
    cout << "document does not contain a root LumiRangeCollection" << endl;
    if( doc ) { xmlFreeDoc(doc); } 
    return false;
  }
    /*
  cur = cur->children;
  while( cur && xmlStrcmp(cur->name, (const xmlChar *)("NamedLumiRange")) ) {
    cur = cur->next;
  }
  if( !cur ) {
    cout << "document does not contain a root NamedLumiRange" << endl;
    if( doc ) { xmlFreeDoc(doc); } 
    return false;
  }
    */
  unsigned int nruns = 0ul;
  unsigned int nlumiblocks = 0ul;
  vector<LumiBlockRange> ranges;
  // Iterate over <NamedLumiRange> nodes
  for( xmlNodePtr k=cur->children; k; k=k->next ) { 
    if( xmlStrcmp(k->name,(const xmlChar *)("NamedLumiRange")) ) { continue; }
    // Iterate over <LumiBlockCollection> nodes
    for( xmlNodePtr i=k->children; i; i=i->next ) { 
      if( xmlStrcmp(i->name,(const xmlChar *)("LumiBlockCollection")) ) { continue; }
      int run = -1;
      int lb_begin = -1;
      int lb_end = -1;
      // get Run and LBRange
      xmlNodePtr j=i->children;
      while( j ) {
	// <Run>
	if( !xmlStrcmp(j->name,(const xmlChar *)("Run")) && j->children ) {
	  //cout << "Run: " << j->children->content << endl;
	  run = atoi((const char*)(j->children->content));
	}
	// <LBRange>
	else if( !xmlStrcmp(j->name,(const xmlChar *)("LBRange")) ) {
	  xmlAttr* prop = j->properties;
	  while( prop ) {
	    // <LBRange Start="">
	    if( !xmlStrcmp(prop->name,(const xmlChar *)("Start")) && prop->children ) {
	      //cout << "LB Start: " << prop->children->content << endl;
	      lb_begin = atoi((const char*)(prop->children->content));
	    }
	    // <LBRange End="">
	    else if( !xmlStrcmp(prop->name,(const xmlChar *)("End")) && prop->children ) {
	      //cout << "LB End: " << prop->children->content << endl;
	      lb_end = atoi((const char*)(prop->children->content));
	      // now that we've ended an <LBRange>, let's save it
	      if( run>0 && lb_begin>0 && lb_end>0 && lb_end>=lb_begin ) {
		// ensure map consistency. new lumi block end of range should not lie in the map
		if( is_in(list_name,run,lb_end) ) {
		  cout << " warning: corrupt lumi block list. " 
		       << lb_begin << "--" << lb_end
		       << " already at least partially good."
		       << endl;
		  cout << " skipping." << endl;
		} else {
		  if( runmap.find(run) == runmap.end() ) {
		    // first lumi range for this run:
		    runmap[run] = LumiBlockSequence();
		    nruns++;
		  }
		  runmap[run][lb_begin] = LumiBlockRange(run,lb_begin,lb_end);
		  nlumiblocks += (lb_end-lb_begin)+1;
		}
		//ranges.push_back( LumiBlockRange(run,lb_begin,lb_end) );
	      } else {
		cout << " error parsing luminosity block for run: " << run << " range: " << lb_begin << " to " << lb_end << endl;
	      }
	    }
	    prop = prop->next;
	  }
	} // end parse LBRange
	j=j->next;
      } // end parse LumiRangeCollection
    } // end parse LumiBlockCollection
  } // end parse NamedLumiBlocks
  // for_each( ranges.begin() , ranges.end() , cout << "\n" << _1 );
  if( doc ) { xmlFreeDoc(doc); } 
  cout << "good run list " << xml_filename << " loaded with " << nruns << " runs and " << nlumiblocks << " luminosity blocks." << endl;
  return true;
}

const bool
GoodRunList::add_multiple( const std::string& list_name , const std::string& xml_pattern )
{
  // need to come up with a portable globbing solution
  return false;
}

#ifdef COMPILE_STANDALONE
int
main( void )
{
  GoodRunList list;
  
  list.add( GoodRunList::MUON , "./good_run_2010A_152166_153200_ele_may.xml"  );
  
  vector<int> runs;
  runs.push_back( 152508 );
  runs.push_back( 152845 );
  runs.push_back( 152846 );

  for( vector<int>::const_iterator i=runs.begin() , f=runs.end(); i!=f; ++i ) {
    for( int lb=0; lb!=1000; ++lb ) {
      const bool ok = list.is_good( GoodRunList::ELECTRON , *i , lb );
      cout << (boost::format( "%|6d| %|3d|: %|6s|" ) % (*i) % lb % (ok?"good":"bad")) << endl;
    }
  }
  return 0;
}
#endif
