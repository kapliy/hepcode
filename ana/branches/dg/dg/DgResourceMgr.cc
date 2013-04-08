
#include <iostream>
#include <string>
#include <list>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/algorithm/string.hpp>
#include <TFile.h>
#include <TDirectory.h>
#include <TObject.h>
#include <TKey.h>
#include "DataGraphics/DataGraphics.hh"
#include "DataGraphics/DgResourceMgr.hh"

// load the data object from disk

using namespace std;
using namespace boost;
using namespace DataGraphics;

//extern TDirectory* gDirectory;

const unsigned long // returns graphic size in bytes
DgResourceMgr::DgResource::load() 
{
  if( type==DgResource::GRAPHIC ) { 
    // if this is ever used beyond dgplot, the open files should probably be managed, cached resources too.
    shared_ptr<TFile> f( DgResourceMgr::get_file(file_id) );
    if( !f ) { 
      cout << " warning: could not open file " << filename << " to retrieve " << path << endl;
      return 0;
    }
    if( iends_with(path,"/") ) {
      cout << " warning: invalid path for file " << filename << " to retrieve " << path << endl;
      return 0;
    }
    TDirectory* dir = f.get();
    list<string> dirnames;
    split( dirnames , path , is_any_of("/") , token_compress_on );
    // dirnames.push_front( "dg" ); // add the 'dg'
    // dirnames.push_front( "dg" ); // add the 'dg'
    string object_name = dirnames.back();
    if( object_name == "" ) {
      cout << " warning: could not open file " << filename << " to retrieve " << path << endl;
      return 0;
    }
    //
    dirnames.pop_back(); // remove the name of the object
    while( ! dirnames.empty() ) {
      string dirname( dirnames.front() ); dirnames.pop_front();
      if( ! dir->cd( dirname.c_str() ) ) {
        cout << " warning: lost at " << dir->GetName() << " in file " << filename << " to retrieve " << path << endl;
        return 0;
      }
      dir = gDirectory;
    }
    // retrieve object key
    assert( dir );
    TKey* key = dir->GetKey( object_name.c_str() );
    if( !key ) { 
      cout << dir->GetName() << " " << path << endl;
      cout << " warning: lost graphic key in file " << filename << " to retrieve " << path << endl;
      return 0;
    }
    //
    scoped_ptr<TObject> obj( key->ReadObj() );
    if( ! obj ) { 
      cout << " warning: lost graphic in file " << filename << " to retrieve " << path << endl;
      return 0;
    }
    // DgAbsGraphic* g = dynamic_cast<DgAbsGraphic*>( obj );
    // if( ! g ) {
    //   cout << " warning: not a graphic in file " << filename << " to retrieve " << path << endl;
    //   return 0;
    // }
    // got it. clone it and manually free the pointers in the correct order.
#define load_obj( tobject , object , ptr ) graphic.reset( dynamic_cast<DgAbsGraphic*>( new object(* dynamic_cast<const tobject*>(ptr)) ) )
    TObject* ptr = obj.get();
    if( obj->IsA() == TObjectDgProfile1D::Class() ) {
      load_obj(TObjectDgProfile1D,DgProfile1D,ptr);
    } else if( obj->IsA() == TObjectDgTH1D::Class() ) {
      load_obj(TObjectDgTH1D,TObjectDgTH1D,ptr);
    } else if( obj->IsA() == TObjectDgTH2D::Class() ) {
      load_obj(TObjectDgTH2D,TObjectDgTH2D,ptr);
    } else if( obj->IsA() == TObjectDgTH3D::Class() ) {
      load_obj(TObjectDgTH3D,TObjectDgTH3D,ptr);
    } else if( obj->IsA() == TObjectDgHistogram1D::Class() ) {
      load_obj(TObjectDgHistogram1D,DgHistogram1D,ptr);
    } else if( obj->IsA() == TObjectDgHistogramWeighted1D::Class() ) {
      load_obj(TObjectDgHistogramWeighted1D,DgHistogramWeighted1D,ptr);
    } else if( obj->IsA() == TObjectDgHistogram2D::Class() ) {
      load_obj(TObjectDgHistogram2D,DgHistogram2D,ptr);
    } else if( obj->IsA() == TObjectDgHistogramWeighted2D::Class() ) {
      load_obj(TObjectDgHistogramWeighted2D,DgHistogramWeighted2D,ptr);
    } else if( obj->IsA() == TObjectDgGraph2D::Class() ) {
      load_obj(TObjectDgGraph2D,DgGraph2D,ptr);
    } else if( obj->IsA() == TObjectDgNtuple::Class() ) {
      load_obj(TObjectDgNtuple,DgNtuple,ptr);
    } else if( obj->IsA() == TObjectDgEfficiency0D::Class() ) {
      load_obj(TObjectDgEfficiency0D,DgEfficiency0D,ptr);
    } else if( obj->IsA() == TObjectDgEfficiency1D::Class() ) {
      load_obj(TObjectDgEfficiency1D,DgEfficiency1D,ptr);
    } else if( obj->IsA() == TObjectDgEfficiency2D::Class() ) {
      load_obj(TObjectDgEfficiency2D,DgEfficiency2D,ptr);
    } else if( obj->IsA() == TObjectDgEfficiencyWeighted0D::Class() ) {
      load_obj(TObjectDgEfficiencyWeighted0D,DgEfficiencyWeighted0D,ptr);
    } else {
      // unrecognized
      cout << " warning: not a recognized graphic in file " << filename << " to retrieve " << path << endl;
      obj.reset();
      return 0;
    }
    //
    unsigned int obj_size = key->GetObjlen(); // uncompressed object size
    obj.reset();
    return obj_size;
  } else if( type == DgResource::FILE ) {
    //
    shared_ptr<TFile> f( TFile::Open( filename.c_str() ) ); 
    if( !f || !f->IsOpen() || f->IsZombie() ) {
      cout << " warning: could not open file " << filename << endl;
      return 0;
    }
    file = f;
    return 1; // make up nonzero size (small, low weight)
  }
  assert( !"unhandled type" );
}

void
DgResourceMgr::DgResource::unload()
{
  if( type == DgResource::GRAPHIC ) { 
    assert( graphic );
    assert( graphic.unique() );
    graphic.reset();
    return;
  } 
  if( type == DgResource::FILE ) { 
    assert( file );
    assert( file.unique() );
    file->Close();
    file.reset();
    return;
  }
}

const boost::shared_ptr<TFile>
DgResourceMgr::get_file( const ID& id ) {
  DgResourceMgr* mgr = instance();
  ++(mgr->_requests);
  ResourceColl::iterator i = mgr->_resources.find( id );
  if( i==mgr->_resources.end() ) {       
    return boost::shared_ptr<TFile>(); 
  }
  unsigned long& freq = mgr->_freqs[ id ];
  ++freq;
  mgr->_max_freq = std::max( freq , mgr->_max_freq );
  DgResource& res( i->second );
  if( res.type != DgResource::FILE ) { 
    return boost::shared_ptr<TFile>(); 
  }
  if( res.file ) { 
    ++mgr->_hits;
    mgr->_history[ id ] = (mgr->_requests)-1;
    return res.file; 
  }
  // not loaded yet. load
  ++mgr->_misses;
  const unsigned long nbytes = res.load();
  mgr->_sizes[ id ] = nbytes;
  mgr->_history[ id ] = (mgr->_requests)-1;
  mgr->_total_mem += nbytes;
  assert( res.file );
  return res.file;
}

// release all resources (e.g. close all open file handles) before exit.
DgResourceMgr::~DgResourceMgr()
{
  // for( ResourceColl::iterator i=_resources.begin(), f=_resources.end(); i!=f; ++i ) {
  //   DgResource& res( i->second );
  //   if( res.type != DgResource::FILE ) { continue; }
  //   res.unload();
  // }
}


DgResourceMgr* DgResourceMgr::_instance = 0;
