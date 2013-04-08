
#include <DataGraphics/DgTypes.hh>
#include <DataGraphics/DgCollection.hh>
#include <DataGraphics/DgAbsGraphic.hh>
#include <DataGraphics/DgTH1D.hh>
#include <DataGraphics/DgTH2D.hh>
#include <DataGraphics/DgTH3D.hh>
#include <DataGraphics/DgHistogram1D.hh>
#include <DataGraphics/DgHistogramWeighted1D.hh>
#include <DataGraphics/DgHistogram2D.hh>
#include <DataGraphics/DgHistogramWeighted2D.hh>
#include <DataGraphics/DgProfile1D.hh>
#include <DataGraphics/DgGraph2D.hh>
#include <DataGraphics/DgNtuple.hh>
#include <DataGraphics/DgEfficiency0D.hh>
#include <DataGraphics/DgEfficiency1D.hh>
#include <DataGraphics/DgEfficiency2D.hh>
#include <DataGraphics/DgEfficiencyWeighted0D.hh>
// #include <DataGraphics/DgAxis.hh>
#include <DataGraphics/DgTools.hh>

#include <cassert>
#include <iostream>
#include <cmath>
#include <vector>
#include <stack>
#include <algorithm>
#include <iterator>
#include <ext/functional>
#include <cstring>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/date_time.hpp>
#include <boost/format.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/foreach.hpp>
#include <boost/regex.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <fstream>

#include <TH1F.h>
#include <TH2F.h>
#include <TDirectory.h>
#include <TKey.h>
#include <TCollection.h>
#include <TList.h>
#include <TString.h>
#include <TFile.h>
#include <TClass.h>


using namespace std;
using namespace DataGraphics;
using namespace __gnu_cxx;

// extern TFile* gFile;


DgCollection::DgCollection( DgCollection* parent ,
                            const std::string& sname , 
                            const std::string& description )  
  : _name(sname) 
  , _description(description)
  , _parent(parent)
  , _save_filename()
  , _save_at_exit(false)
  , _save_as_ROOT(false)
  , _save_as_dg(false)
  , _locked(false)
  , _read_on_demand(false)
{
  if( parent ) { parent->_children[sname] = this; }
}

DgCollection::~DgCollection()
{
  //   // automatic save? 
  if( _save_at_exit ) { do_save_at_exit(); }
  // delete graphics objects and subgroups
  // uncomment the following if you get strange ROOT-related segmentation violations in destructors
  for_each( _children.begin() , _children.end() , compose1( mem_fun(&DgCollection::delete_me) , select2nd<SubColl::value_type>() ) );
  for_each( _graphics.begin() , _graphics.end() , compose1( Tools::DeleteFunctor() , select2nd<Coll::value_type>() ) );
}

void
DgCollection::do_save_at_exit()
{
  using namespace boost;
  using namespace boost::posix_time;
  using namespace boost::filesystem;
  using namespace boost::algorithm;
  if( !_save_as_ROOT && !_save_as_dg ) { return; }

  const bool use_serialization = false;

  if( _save_as_ROOT || (!use_serialization && _save_as_dg) ) { 
    cout << " [ dg writing root... ] " << endl;
    // get full save filename. 
    string unique_name = DgCollection::save_unique
      ? DataGraphics::Tools::expand_filename_to_valid_unique( (*_save_filename)+".root" )
      : DataGraphics::Tools::expand_filename_to_valid( (*_save_filename)+".root" );
    cout << " [ dg write: " << unique_name << " ] " << endl;
    boost::scoped_ptr<TFile> f( TFile::Open(unique_name.c_str(),"RECREATE") );
    if( !f || f->IsZombie() || !f->IsOpen() ) {
      cout << " output file " << unique_name << " could not be opened." << endl;
    } else {
      if( _save_as_ROOT ) {
        root()->save_as_ROOT( f.get() );
        cout << " view output with " << endl << "   root -l " << unique_name << endl;
      }
      if( !use_serialization && _save_as_dg ) {
        root()->save( f->mkdir("dg","") );
        cout << " view output with " << endl << "   dgplot " << unique_name << endl;
      
      }
      f->Close();
    }
  }
  if( use_serialization && _save_as_dg ) { 
    cout << " [ dg writing dg... ] " << endl;
    // get full save filename. 
    string unique_name = DgCollection::save_unique
      ? DataGraphics::Tools::expand_filename_to_valid_unique( (*_save_filename)+".dg" )
      : DataGraphics::Tools::expand_filename_to_valid( (*_save_filename)+".dg" );
    cout << " [ dg write: " << unique_name << " ] " << endl;
    std::ofstream ofs( unique_name.c_str() );
    boost::archive::text_oarchive oa( ofs );
    oa << *this;
    cout << " view output with " << endl << "   dgplot " << unique_name << endl;
  }
  
  cout << " [ dg write complete. ] " << endl;
}

DgCollection*
DgCollection::cd(const std::string& dir)
{
  // string name(dir);
  // // root directory?
  // if( name == "/" ) { 
  //   _current = root(); 
  //   return _current;
  // }
  // // absolute or relative path.
  // DgCollection* result = root();
  // int i = 1;
  // if( name.find("/",0) != 0 ) {
  //   result = current();
  //   i = 0;
  // }
  // int n = name.size();
  // int j = name.find( "/" , i+1 );
  // do {
  //   if( j == string::npos ) { j = n+1; }
  //   string thisname( name.substr( i , j-i-1 ) );
  //   i=j;
  //   j = name.find( "/" , i+1 );
  //   if( thisname == "" || thisname == "." ) { continue; }
  //   if( thisname == ".." && result->_parent ) { result = result->_parent; continue; }
  //   SubColl::iterator s = result->_children.find(thisname.c_str());
  //   if( s != result->_children.end() ) {
  //     result = s->second;
  //   }
  // } while( i<n );
  // _current = result;
  // return result;
  if( dir == "" ) { return _current; }
  if( dir == "/" ) { _current = root(); return _current; }
  using namespace boost;
  vector<string> components;
  split( components , dir , is_any_of("/") );
  if( components.empty() ) { return _current; }
  assert( dir.size()>=1 );
  DgCollection* result = _current;
  if( dir.find("/",0) == 0 ) { result = root(); }
  //if( components.back() != "" ) { components.pop_back(); }
  for( vector<string>::const_iterator i=components.begin(), f=components.end(); i!=f; ++i ) {
    string comp( *i );
    if( comp.empty() || comp=="" || comp=="." ) { continue; }
    if( comp == ".." && result->_parent ) { result = result->_parent; continue; }
    SubColl::iterator s = result->_children.find( comp.c_str() );
    if( s!=result->_children.end() ) {
      result = s->second;
    }
  }
  _current = result;
  return result;
}

void
DgCollection::save_as_ROOT( TDirectory* directory ) const
{
  assert( directory );
  // save objects in this directory
  for( Coll::const_iterator i = _graphics.begin() , f = _graphics.end(); i!=f; ++i ) {
    i->second->save_as_ROOT( i->first.c_str() , directory );
  }
  // save subdirectories
  for( SubColl::const_iterator i = _children.begin() , f = _children.end(); i!=f; ++i ) {
    const DgCollection* subcoll = i->second;
    assert( subcoll->_name != "" );
    TDirectory* subdir = directory->mkdir( subcoll->_name.c_str() , subcoll->_description.c_str() );
    assert( subdir );
    subcoll->save_as_ROOT( subdir );
    subdir->Write( subcoll->_name.c_str() , TObject::kOverwrite );
    delete subdir;
  }
  // done
}

void
DgCollection::save( TDirectory* directory ) const
{
  assert( directory );
  // save objects in this directory
  if( !_graphics.empty() ) {
    unsigned int ig = 0;
    unsigned int ng = _graphics.size();
    for( Coll::const_iterator i=_graphics.begin() , f=_graphics.end(); i!=f; ++i ) {
      assert( (++ig)<=ng );
      i->second->save( i->first.c_str() , directory );
    }
  }
  // save subdirectories
  if( !_children.empty() ) {
    unsigned int ichild=0;
    unsigned int nchildren=_children.size();
    for( SubColl::const_iterator i=_children.begin() , f=_children.end(); i!=f; ++i ) {
      assert( (++ichild)<=nchildren );
      const DgCollection* subcoll = i->second;
      assert( subcoll->_name != "" );
      if( subcoll->_children.empty() && subcoll->_graphics.empty() ) { continue; } // skip creating output for empty collections
      TDirectory* subdir = directory->mkdir( subcoll->_name.c_str() , subcoll->_description.c_str() );
      assert( subdir );
      subcoll->save( subdir );
      subdir->Write( subcoll->_name.c_str() , TObject::kOverwrite );
      delete subdir;
    }
  }
  // done
}

template<class tobjT,class graphT>
void
DgCollection::load_obj( const tobjT* bwtobj , const bool merge )
{
  assert( bwtobj );
  const std::string& obj_name = bwtobj->GetName();
  Coll::iterator i = current()->_graphics.find( obj_name );
  if( i!=current()->_graphics.end() ) {
    if( merge ) {
      graphT tmp( *bwtobj );
      (*i).second->merge( &tmp );
    } else {
      cout << "collection " << current()->_name << " already contains a graphic named " << obj_name << "; will not load another." << endl;
    }
  } else {
    graphT* h = new graphT( *bwtobj );
    current()->_graphics[ obj_name ] = h;
  }
}

//#include <iostream>
void
DgCollection::load( TDirectory* directory , const bool merge , const std::string parent_path , const bool skip_path_substitutions )
{
  assert( current() );
  assert( directory );
  // figure out full path string to this collection
  const bool on_demand = is_read_on_demand();
  // determine full path. path passed may start with 'root' - if that
  // is the case, it must be removed.
  string full_path = parent_path;
  // if( on_demand ) {
  //   const DgCollection* coll = current();
  //   do {
  //     full_path = coll->name() + "/" + full_path;
  //   } while( coll!=dg::root() && (coll=coll->parent()) );
  // }
  if( !skip_path_substitutions && (full_path[0] == 'r' || full_path[0]=='R') ) {
    // this comparison is expensive, so only do it when necessary.
    if( boost::istarts_with( full_path , "root/" ) ) { 
      boost::replace_first( full_path , "root/" , "/" );
    }
  }
  // filename is the filename set up by dgplot, i.e. the full path with '_' substituted for '_'.
  const string filename = on_demand && gFile ? gFile->GetName() : "";
  if( filename != "" && !skip_path_substitutions ) {
    string tmpfn( filename );
    boost::replace_all( tmpfn , "/" , "_" );
    if( boost::istarts_with( full_path , string("/")+tmpfn ) ) {
      boost::replace_first( full_path , tmpfn , "" );
      boost::trim_left_if( full_path , boost::is_any_of("/") );
    }
  }
  const DgResourceMgr::ID file_id = on_demand ? DgResourceMgr::add_file_resource( filename ) : 0;
  // cout << " file id: " << file_id << " for " << filename << endl;
  //
  //  cout << " at directory " << directory->GetName() << endl; 
  TIter iObject( directory->GetListOfKeys() );
  while( TObject* keyobj = iObject.Next() ) {
#ifdef DATA_GRAPHICS_DEBUG
    cout << " at " << keyobj->GetName() << " in directory " << directory->GetName() << endl; 
#endif
    TKey* tkey = dynamic_cast<TKey*>( keyobj ); 
    assert( tkey );
#ifdef DATA_GRAPHICS_DEBUG
    cout << "key: " << tkey->GetName() << " from full path: " << full_path << endl;
    cout << "key: " << tkey->GetTitle() << " from full path: " << full_path << endl;
#endif
    // load subdirectories
    string classname( tkey->GetClassName() );
    // tkey->IsFolder segfaults... how to figure out whether this is a directory/file or an object?
    // works but very slow: if( boost::icontains(classname,"directory") || boost::icontains(classname,"file") ) { // obj->InheritsFrom("TDirectory") ) {
    if( classname=="TDirectoryFile" || classname=="TFile" ) {
//       // hack
//       if( string(tkey->GetName()) != "dg" &&
//           string(tkey->GetName()) != "plhc_hsg3_cutflow" &&
//           !boost::icontains(full_path,"plhc_hsg3") ) { continue; }
      TObject* obj = tkey->ReadObj();  assert(obj);
      TDirectory* subdir = dynamic_cast<TDirectory*>( obj );
      assert( subdir );
      //if( merge && string(subdir->GetName()) == "statistics" ) { continue; } // hack
      //if( merge && string(subdir->GetName()).find( "improvement" )!=string::npos ) { continue; } // hack
      //if( directory->GetListOfKeys()->GetSize()>1 || subdir->GetName()!="dg" ) { 
      down( subdir->GetName() , subdir->GetTitle() );
      //}
      // if the parent directory is read on demand, the children are also.
      if( on_demand ) { current()->read_on_demand( true ); }
      // cout << "subdir: " << subdir->GetName() << " from full path: " << full_path << endl;
      load( subdir , merge , full_path + "/" + subdir->GetName() , true /*skip path substitutions*/ );
      // if( directory->GetListOfKeys()->GetSize()>1 || subdir->GetName()!="dg" ) { 
      up();
      //      }
      //
#ifdef DATA_GRAPHICS_DEBUG
      cout << " loaded directory " << directory->GetName() << endl; 
#endif
      delete obj;
    } else { // load data graphics object
      if( ! _read_on_demand ) {
        // load all objects into memory. if you're adding something here, also add it in DgResourceMgr.cc.
        TObject* obj = tkey->ReadObj(); assert(obj);
        if( obj->IsA() == TObjectDgProfile1D::Class() ) {
          load_obj<TObjectDgProfile1D,DgProfile1D>( dynamic_cast<const TObjectDgProfile1D*>(obj) , merge );
        } else if( obj->IsA() == TObjectDgTH1D::Class() ) {
          load_obj<TObjectDgTH1D,TObjectDgTH1D>( dynamic_cast<const TObjectDgTH1D*>(obj) , merge );
        } else if( obj->IsA() == TObjectDgTH2D::Class() ) {
          load_obj<TObjectDgTH2D,TObjectDgTH2D>( dynamic_cast<const TObjectDgTH2D*>(obj) , merge );
        } else if( obj->IsA() == TObjectDgTH3D::Class() ) {
          load_obj<TObjectDgTH3D,TObjectDgTH3D>( dynamic_cast<const TObjectDgTH3D*>(obj) , merge );
        } else if( obj->IsA() == TObjectDgHistogram1D::Class() ) {
          load_obj<TObjectDgHistogram1D,DgHistogram1D>( dynamic_cast<const TObjectDgHistogram1D*>(obj) , merge );
        } else if( obj->IsA() == TObjectDgHistogramWeighted1D::Class() ) {
          load_obj<TObjectDgHistogramWeighted1D,DgHistogramWeighted1D>( dynamic_cast<const TObjectDgHistogramWeighted1D*>(obj) , merge );
        } else if( obj->IsA() == TObjectDgHistogram2D::Class() ) {
          load_obj<TObjectDgHistogram2D,DgHistogram2D>( dynamic_cast<const TObjectDgHistogram2D*>(obj) , merge );
        } else if( obj->IsA() == TObjectDgHistogramWeighted2D::Class() ) {
          load_obj<TObjectDgHistogramWeighted2D,DgHistogramWeighted2D>( dynamic_cast<const TObjectDgHistogramWeighted2D*>(obj) , merge );
        } else if( obj->IsA() == TObjectDgGraph2D::Class() ) {
          load_obj<TObjectDgGraph2D,DgGraph2D>( dynamic_cast<const TObjectDgGraph2D*>(obj) , merge );
        } else if( obj->IsA() == TObjectDgNtuple::Class() ) {
          load_obj<TObjectDgNtuple,DgNtuple>( dynamic_cast<const TObjectDgNtuple*>(obj) , merge );
        } else if( obj->IsA() == TObjectDgEfficiency0D::Class() ) {
          load_obj<TObjectDgEfficiency0D,DgEfficiency0D>( dynamic_cast<const TObjectDgEfficiency0D*>(obj) , merge );
        } else if( obj->IsA() == TObjectDgEfficiency1D::Class() ) {
          load_obj<TObjectDgEfficiency1D,DgEfficiency1D>( dynamic_cast<const TObjectDgEfficiency1D*>(obj) , merge );
        } else if( obj->IsA() == TObjectDgEfficiency2D::Class() ) {
          load_obj<TObjectDgEfficiency2D,DgEfficiency2D>( dynamic_cast<const TObjectDgEfficiency2D*>(obj) , merge );
        } else if( obj->IsA() == TObjectDgEfficiencyWeighted0D::Class() ) {
          load_obj<TObjectDgEfficiencyWeighted0D,DgEfficiencyWeighted0D>( dynamic_cast<const TObjectDgEfficiencyWeighted0D*>(obj) , merge );
        } else {
          cout << "don't know what to do with " << classname << " at line " << __LINE__ << " in " << __FILE__ << endl;
        }
#ifdef DATA_GRAPHICS_DEBUG
        cout << " loaded " << obj->GetName() << " from directory " << directory->GetName() << endl; 
#endif
        delete obj;
        // end read all objects
      } else {
        // read on demand only. store resource handle to the data object, but not the object itself.
        boost::trim( full_path );
        string sname = full_path!="" ? (full_path+"/"+tkey->GetName()) : string(tkey->GetName());
        const DgResourceMgr::ID id = DgResourceMgr::add_graphic_resource( filename , sname , file_id );
        current()->_resource_id_map[ tkey->GetName() ] = id;
      } // done read on demand setup
    } // figure out what to do with this key
  } // look at each key in the ROOT TDirectory
  // don't try to automatically save a loaded tree
  _save_at_exit = false;
  // lock to prevent changes
  lock();
}

/* 1D histograms */
void
DgCollection::fillh( const std::string& sname , const BinSize& nbins , const Value& min , const Value& max , 
                     const Value& x )
{
  static const std::string empty_string;
  fillh(sname,nbins,min,max,x,empty_string);
}

void
DgCollection::fillh2( const std::string& sname , const BinSize& nbins , const Value& min , const Value& max , 
		      const Value& x )
{
  static const std::string empty_string;
  fillh2(sname,nbins,min,max,x,empty_string);
}

void
DgCollection::fillh( const std::string& sname , const BinSize& nbins , const Value& min , const Value& max ,
		     const Value& x , const std::string& axis_label )
{
  if( current()->_locked ) { return; } // no changes allowed.
  if( dg::has_global_weight() ) {
    fillhw(sname,nbins,min,max,x,dg::global_weight(),axis_label);
    return;
  }
  assert( current() );
  Coll::iterator i = current()->_graphics.find( sname );
  if( i!=current()->_graphics.end() ) {
    i->second->fill( x );
    return;
  }
  DgHistogram1D *h = new DgHistogram1D( nbins , min , max );
  h->set_axis_label( axis_label );
  assert( h );
  h->fill( x );
  current()->_graphics[ sname ] = h;
}

void
DgCollection::fillh2( const std::string& sname , const BinSize& nbins , const Value& min , const Value& max ,
		      const Value& x , const std::string& axis_label )
{
  if( current()->_locked ) { return; } // no changes allowed.
  if( dg::has_global_weight() ) {
    fillhw2(sname,nbins,min,max,x,dg::global_weight(),axis_label);
    return;
  }
  assert( current() );
  Coll::iterator i = current()->_graphics.find( sname );
  if( i!=current()->_graphics.end() ) {
    i->second->fill( x );
    return;
  }
  DgTH1D *h = new DgTH1D( nbins , min , max );
  h->set_axis_label( axis_label );
  assert( h );
  h->fill( x );
  current()->_graphics[ sname ] = h;
}

void
DgCollection::fillvh( const std::string& sname , const BinSize& nbins , const std::vector<Value>& bins ,
		      const Value& x )
{
  static const std::string empty_string;
  fillvh(sname,nbins,bins,x,empty_string);
}

void
DgCollection::fillvh2( const std::string& sname , const BinSize& nbins , const std::vector<Value>& bins ,
		       const Value& x )
{
  static const std::string empty_string;
  fillvh2(sname,nbins,bins,x,empty_string);
}

void
DgCollection::fillvh( const std::string& sname , const BinSize& nbins , const std::vector<Value>& bins ,
		      const Value& x , const std::string& axis_label )
{
  if( current()->_locked ) { return; } // no changes allowed
  if( dg::has_global_weight() ) {
    fillvhw(sname,nbins,bins,x,dg::global_weight(),axis_label);
    return;
  }
  assert( current() );
  Coll::iterator i = current()->_graphics.find( sname );
  if( i!=current()->_graphics.end() ) {
    i->second->fill( x );
    return;
  }
  DgHistogram1D *h = new DgHistogram1D( nbins , bins );
  h->set_axis_label( axis_label );
  assert( h );
  h->fill( x );
  current()->_graphics[ sname ] = h;
}

void
DgCollection::fillvh2( const std::string& sname , const BinSize& nbins , const std::vector<Value>& bins ,
		      const Value& x , const std::string& axis_label )
{
  if( current()->_locked ) { return; } // no changes allowed
  if( dg::has_global_weight() ) {
    fillvhw2(sname,nbins,bins,x,dg::global_weight(),axis_label);
    return;
  }
  assert( current() );
  Coll::iterator i = current()->_graphics.find( sname );
  if( i!=current()->_graphics.end() ) {
    i->second->fill( x );
    return;
  }
  DgTH1D *h = new DgTH1D( nbins , bins );
  h->set_axis_label( axis_label );
  assert( h );
  h->fill( x );
  current()->_graphics[ sname ] = h;
}

void
DgCollection::fillhw( const std::string& sname , const BinSize& nbins , const Value& min , const Value& max , 
		      const Value& x , const WeightedCount& weight )
{
  static const std::string empty_string;
  fillhw(sname,nbins,min,max,x,weight,empty_string);
}

void
DgCollection::fillhw2( const std::string& sname , const BinSize& nbins , const Value& min , const Value& max , 
		       const Value& x , const WeightedCount& weight )
{
  static const std::string empty_string;
  fillhw2(sname,nbins,min,max,x,weight,empty_string);
}

void
DgCollection::fillhw( const std::string& sname , const BinSize& nbins , const Value& min , const Value& max , 
		      const Value& x , const WeightedCount& weight, const std::string& axis_label )
{
  if( current()->_locked ) { return; } // no changes allowed.
  assert( current() );
  Coll::iterator i = current()->_graphics.find(sname);
  if( i!=current()->_graphics.end() ) {
    i->second->fillw( x , weight );
    return;
  }
  DgHistogramWeighted1D *h = new DgHistogramWeighted1D( nbins , min , max );
  h->set_axis_label( axis_label );
  assert( h );
  h->fillw( x , weight );
  current()->_graphics[ sname ] = h;
}

void
DgCollection::fillhw2( const std::string& sname , const BinSize& nbins , const Value& min , const Value& max , 
		       const Value& x , const WeightedCount& weight, const std::string& axis_label )
{
  if( current()->_locked ) { return; } // no changes allowed.
  assert( current() );
  Coll::iterator i = current()->_graphics.find(sname);
  if( i!=current()->_graphics.end() ) {
    i->second->fillw( x , weight );
    return;
  }
  DgTH1D *h = new DgTH1D( nbins , min , max );
  h->Sumw2();
  h->set_axis_label( axis_label );
  assert( h );
  h->fillw( x , weight );
  current()->_graphics[ sname ] = h;
}

void
DgCollection::fillvhw( const std::string& sname , const BinSize& nbins , const std::vector<Value>& bins ,
		       const Value& x , const WeightedCount& weight )
{
  static const std::string empty_string;
  fillvhw(sname,nbins,bins,x,weight,empty_string);
}

void
DgCollection::fillvhw2( const std::string& sname , const BinSize& nbins , const std::vector<Value>& bins ,
			const Value& x , const WeightedCount& weight )
{
  static const std::string empty_string;
  fillvhw2(sname,nbins,bins,x,weight,empty_string);
}

void
DgCollection::fillvhw( const std::string& sname , const BinSize& nbins , const std::vector<Value>& bins ,
		       const Value& x , const WeightedCount& weight, const std::string& axis_label )
{
  if( current()->_locked ) { return; } // no changes allowed.
  assert( current() );
  Coll::iterator i = current()->_graphics.find(sname);
  if( i!=current()->_graphics.end() ) {
    i->second->fillw( x , weight );
    return;
  }
  DgHistogramWeighted1D *h = new DgHistogramWeighted1D( nbins , bins );
  h->set_axis_label( axis_label );
  assert( h );
  h->fillw( x , weight );
  current()->_graphics[ sname ] = h;
}

void
DgCollection::fillvhw2( const std::string& sname , const BinSize& nbins , const std::vector<Value>& bins ,
			const Value& x , const WeightedCount& weight, const std::string& axis_label )
{
  if( current()->_locked ) { return; } // no changes allowed.
  assert( current() );
  Coll::iterator i = current()->_graphics.find(sname);
  if( i!=current()->_graphics.end() ) {
    i->second->fillw( x , weight );
    return;
  }
  DgTH1D *h = new DgTH1D( nbins , bins );
  h->Sumw2();
  h->set_axis_label( axis_label );
  assert( h );
  h->fillw( x , weight );
  current()->_graphics[ sname ] = h;
}

/* 2D histograms */
void
DgCollection::fillh( const std::string& sname , 
                     const BinSize& nbinsx , const Value& minx , const Value& maxx , 
                     const BinSize& nbinsy , const Value& miny , const Value& maxy , 
                     const Value& x , const Value& y , const std::string& axis_label_x , const std::string& axis_label_y )
{
  if( current()->_locked ) { return; } // no changes allowed.
  if( dg::has_global_weight() ) {
    fillhw(sname,nbinsx,minx,maxx,nbinsy,miny,maxy,x,y,dg::global_weight(),axis_label_x,axis_label_y);
    return;
  }
  assert( current() );
  Coll::iterator i = current()->_graphics.find(sname);
  if( i!=current()->_graphics.end() ) {
    i->second->fill( x , y );
    return;
  }
  DgHistogram2D* h = new DgHistogram2D( nbinsx , minx , maxx , nbinsy , miny , maxy );
  assert( h );
  h->set_x_axis_label( axis_label_x );
  h->set_y_axis_label( axis_label_y );
  h->fill( x , y );
  current()->_graphics[ sname ] = h;
}

void
DgCollection::fillh2( const std::string& sname , 
		      const BinSize& nbinsx , const Value& minx , const Value& maxx , 
		      const BinSize& nbinsy , const Value& miny , const Value& maxy , 
		      const Value& x , const Value& y , const std::string& axis_label_x , const std::string& axis_label_y )
{
  if( current()->_locked ) { return; } // no changes allowed.
  if( dg::has_global_weight() ) {
    fillhw2(sname,nbinsx,minx,maxx,nbinsy,miny,maxy,x,y,dg::global_weight(),axis_label_x,axis_label_y);
    return;
  }
  assert( current() );
  Coll::iterator i = current()->_graphics.find(sname);
  if( i!=current()->_graphics.end() ) {
    i->second->fill( x , y );
    return;
  }
  DgTH2D* h = new DgTH2D( nbinsx , minx , maxx , nbinsy , miny , maxy );
  assert( h );
  h->set_x_axis_label( axis_label_x );
  h->set_y_axis_label( axis_label_y );
  h->fill( x , y );
  current()->_graphics[ sname ] = h;
}

void
DgCollection::fillvh2( const std::string& sname , 
		       const BinSize& nbinsx , const std::vector<Value>& binsx ,
		       const BinSize& nbinsy , const std::vector<Value>& binsy ,
		       const Value& x , const Value& y , const std::string& axis_label_x , const std::string& axis_label_y )
{
  if( current()->_locked ) { return; } // no changes allowed.
  if( dg::has_global_weight() ) {
    fillvhw2(sname,nbinsx,binsx,nbinsy,binsy,x,y,dg::global_weight(),axis_label_x,axis_label_y);
    return;
  }
  assert( current() );
  Coll::iterator i = current()->_graphics.find(sname);
  if( i!=current()->_graphics.end() ) {
    i->second->fill( x , y );
    return;
  }
  DgTH2D* h = new DgTH2D( nbinsx , binsx , nbinsy , binsy );
  assert( h );
  h->set_x_axis_label( axis_label_x );
  h->set_y_axis_label( axis_label_y );
  h->fill( x , y );
  current()->_graphics[ sname ] = h;
}

void
DgCollection::fillhw( const std::string& sname , 
		      const BinSize& nbinsx , const Value& minx , const Value& maxx , 
		      const BinSize& nbinsy , const Value& miny , const Value& maxy , 
		      const Value& x , const Value& y , const WeightedCount& weight ,
		      const std::string& axis_label_x , const std::string& axis_label_y )
{
  if( current()->_locked ) { return; } // no changes allowed.
  assert( current() );
  Coll::iterator i = current()->_graphics.find(sname);
  if( i!=current()->_graphics.end() ) {
    i->second->fillw( x , y , weight );
    return;
  }
  DgHistogramWeighted2D* h = new DgHistogramWeighted2D( nbinsx , minx , maxx , nbinsy , miny , maxy );
  assert( h );
  h->set_x_axis_label( axis_label_x );
  h->set_y_axis_label( axis_label_y );
  h->fillw( x , y , weight );
  current()->_graphics[ sname ] = h;
}

void
DgCollection::fillhw2( const std::string& sname , 
		       const BinSize& nbinsx , const Value& minx , const Value& maxx , 
		       const BinSize& nbinsy , const Value& miny , const Value& maxy , 
		       const Value& x , const Value& y , const WeightedCount& weight ,
		       const std::string& axis_label_x , const std::string& axis_label_y )
{
  if( current()->_locked ) { return; } // no changes allowed.
  assert( current() );
  Coll::iterator i = current()->_graphics.find(sname);
  if( i!=current()->_graphics.end() ) {
    i->second->fillw( x , y , weight );
    return;
  }
  DgTH2D* h = new DgTH2D( nbinsx , minx , maxx , nbinsy , miny , maxy );
  assert( h );
  h->set_x_axis_label( axis_label_x );
  h->set_y_axis_label( axis_label_y );
  h->fillw( x , y , weight );
  current()->_graphics[ sname ] = h;
}

void
DgCollection::fillvhw2( const std::string& sname , 
			const BinSize& nbinsx , const std::vector<Value>& binsx ,
			const BinSize& nbinsy , const std::vector<Value>& binsy ,
			const Value& x , const Value& y , const WeightedCount& weight ,
			const std::string& axis_label_x , const std::string& axis_label_y )
{
  if( current()->_locked ) { return; } // no changes allowed.
  assert( current() );
  Coll::iterator i = current()->_graphics.find(sname);
  if( i!=current()->_graphics.end() ) {
    i->second->fillw( x , y , weight );
    return;
  }
  DgTH2D* h = new DgTH2D( nbinsx , binsx , nbinsy , binsy );
  assert( h );
  h->set_x_axis_label( axis_label_x );
  h->set_y_axis_label( axis_label_y );
  h->fillw( x , y , weight );
  current()->_graphics[ sname ] = h;
}

/* 3d histograms (only via DgTH3D) */
void
DgCollection::fillh2( const std::string& sname , 
		      const BinSize& nbinsx , const Value& minx , const Value& maxx , 
		      const BinSize& nbinsy , const Value& miny , const Value& maxy ,
		      const BinSize& nbinsz , const Value& minz , const Value& maxz , 
		      const Value& x , const Value& y , const Value& z ,
		      const std::string& axis_label_x , const std::string& axis_label_y , const std::string& axis_label_z )
{
  if( current()->_locked ) { return; } // no changes allowed.
  if( dg::has_global_weight() ) {
    fillhw2(sname,nbinsx,minx,maxx,nbinsy,miny,maxy,nbinsz,minz,maxz,x,y,z,dg::global_weight(),axis_label_x,axis_label_y,axis_label_z);
    return;
  }
  assert( current() );
  Coll::iterator i = current()->_graphics.find(sname);
  if( i!=current()->_graphics.end() ) {
    i->second->fill( x , y );
    return;
  }
  DgTH3D* h = new DgTH3D( nbinsx , minx , maxx , nbinsy , miny , maxy , nbinsz , minz , maxz );
  assert( h );
  h->set_x_axis_label( axis_label_x );
  h->set_y_axis_label( axis_label_y );
  h->set_z_axis_label( axis_label_z );
  h->fill( x , y , z );
  current()->_graphics[ sname ] = h;
}

void
DgCollection::fillvh2( const std::string& sname , 
		       const BinSize& nbinsx , const std::vector<Value>& binsx ,
		       const BinSize& nbinsy , const std::vector<Value>& binsy ,
		       const BinSize& nbinsz , const std::vector<Value>& binsz ,
		       const Value& x , const Value& y , const Value& z ,
		       const std::string& axis_label_x , const std::string& axis_label_y , const std::string& axis_label_z )
{
  if( current()->_locked ) { return; } // no changes allowed.
  if( dg::has_global_weight() ) {
    fillvhw2(sname,nbinsx,binsx,nbinsy,binsy,nbinsz,binsz,x,y,z,dg::global_weight(),axis_label_x,axis_label_y,axis_label_z);
    return;
  }
  assert( current() );
  Coll::iterator i = current()->_graphics.find(sname);
  if( i!=current()->_graphics.end() ) {
    i->second->fill( x , y , z );
    return;
  }
  DgTH3D* h = new DgTH3D( nbinsx , binsx , nbinsy , binsy , nbinsz , binsz );
  assert( h );
  h->set_x_axis_label( axis_label_x );
  h->set_y_axis_label( axis_label_y );
  h->set_z_axis_label( axis_label_z );
  h->fill( x , y , z );
  current()->_graphics[ sname ] = h;
}

void
DgCollection::fillhw2( const std::string& sname , 
		       const BinSize& nbinsx , const Value& minx , const Value& maxx , 
		       const BinSize& nbinsy , const Value& miny , const Value& maxy ,
		       const BinSize& nbinsz , const Value& minz , const Value& maxz , 
		       const Value& x , const Value& y , const Value& z , const WeightedCount& weight ,
		       const std::string& axis_label_x , const std::string& axis_label_y , const std::string& axis_label_z )
{
  if( current()->_locked ) { return; } // no changes allowed.
  assert( current() );
  Coll::iterator i = current()->_graphics.find(sname);
  if( i!=current()->_graphics.end() ) {
    i->second->fillw( x , y , z , weight );
    return;
  }
  DgTH3D* h = new DgTH3D( nbinsx , minx , maxx , nbinsy , miny , maxy , nbinsz , minz , maxz );
  assert( h );
  h->set_x_axis_label( axis_label_x );
  h->set_y_axis_label( axis_label_y );
  h->set_z_axis_label( axis_label_z );
  h->fillw( x , y , z , weight );
  current()->_graphics[ sname ] = h;
}

void
DgCollection::fillvhw2( const std::string& sname ,
			const BinSize& nbinsx , const std::vector<Value>& binsx ,
			const BinSize& nbinsy , const std::vector<Value>& binsy ,
			const BinSize& nbinsz , const std::vector<Value>& binsz ,
			const Value& x , const Value& y , const Value& z , const WeightedCount& weight ,
			const std::string& axis_label_x , const std::string& axis_label_y , const std::string& axis_label_z )
{
  if( current()->_locked ) { return; } // no changes allowed.
  assert( current() );
  Coll::iterator i = current()->_graphics.find(sname);
  if( i!=current()->_graphics.end() ) {
    i->second->fillw( x , y , z , weight );
    return;
  }
  DgTH3D* h = new DgTH3D( nbinsx , binsx , nbinsy , binsy , nbinsz , binsz );
  assert( h );
  h->set_x_axis_label( axis_label_x );
  h->set_y_axis_label( axis_label_y );
  h->set_z_axis_label( axis_label_z );
  h->fillw( x , y , z , weight );
  current()->_graphics[ sname ] = h;
}

/* profile histograms */ 
void
DgCollection::fillf( const std::string& sname , const BinSize& nbins , const Value& min , const Value& max , const Value& x , const Value& y ,  
                     const std::string& axis_label_x , const std::string& axis_label_y )
{
  if( current()->_locked ) { return; } // no changes allowed.
  assert( current() );
  Coll::iterator i = current()->_graphics.find(sname);
  if( i!=current()->_graphics.end() ) {
    if( dg::has_global_weight() ) { i->second->fillw( x , y , dg::global_weight() ); } else { i->second->fill( x , y ); }
    return;
  }
  DgProfile1D* h = new DgProfile1D( nbins , min , max );
  assert( h );
  if( dg::has_global_weight() ) { h->fillw( x , y , dg::global_weight() ); } else { h->fill( x , y ); }
  current()->_graphics[ sname ] = h;
}

void
DgCollection::fillfw( const std::string& sname , const BinSize& nbins , const Value& min , const Value& max , const Value& x , const Value& y , const WeightedCount& weight )
{
  if( current()->_locked ) { return; } // no changes allowed.
  assert( current() );
  Coll::iterator i = current()->_graphics.find(sname);
  if( i!=current()->_graphics.end() ) {
//     DgProfile1D* g = dynamic_cast<DgProfile1D*>(i->second);
//     assert( g );
//     g->fillw( x , y , weight );
    i->second->fillw( x , y , weight );
    return;
  }
  DgProfile1D* h = new DgProfile1D( nbins , min , max );
  assert( h );
  h->fillw( x , y , weight );
  current()->_graphics[ sname ] = h;
}

/* ntuples (arrays of floats) */
void
DgCollection::fillnt( const std::string& sname , const std::vector<float>& x, const std::vector<std::string>& names )
{
  if( current()->_locked ) { return; } // no changes allowed.
  if( dg::has_global_weight() ) {
    fillntw(sname,x,names,dg::event_info());
    return;
  }
  assert( current() );
  assert(x.size() == names.size());
  Coll::iterator i = current()->_graphics.find(sname);
  if( i!=current()->_graphics.end() ) {
    i->second->fillnt( x );
    return;
  }
  DgNtuple* g = new DgNtuple();
  assert( g );
  g->set_names( names );
  g->fillnt( x );
  current()->_graphics[ sname ] = g;
}
void
DgCollection::fillntw( const std::string& sname , const std::vector<float>& x, const std::vector<std::string>& names, const DgEventInfo& w )
{
  if( current()->_locked ) { return; } // no changes allowed.
  assert( current() );
  assert(x.size() == names.size());
  Coll::iterator i = current()->_graphics.find(sname);
  if( i!=current()->_graphics.end() ) {
    i->second->fillntw( x , w.get_values() );
    return;
  }
  DgNtuple* g = new DgNtuple();
  assert( g );
  g->add_names( w.get_names() );
  g->add_names( names );
  g->fillntw( x , w.get_values() );
  current()->_graphics[ sname ] = g;
}

void
DgCollection::fillgraph( const std::string& sname , const Value& x, const Value& y , const std::string& axis_label_x , const std::string& axis_label_y )
{
  if( current()->_locked ) { return; } // no changes allowed.
  assert( current() );
  Coll::iterator i = current()->_graphics.find(sname);
  if( i!=current()->_graphics.end() ) {
    if( dg::has_global_weight() ) { 
      i->second->fillw( x , y , dg::global_weight() );
      return;
    } else {
      i->second->fill( x , y );
      return;
    }
    return;
  }
  DgGraph2D* g = new DgGraph2D();
  assert( g );
  g->set_x_axis_label( axis_label_x );
  g->set_y_axis_label( axis_label_y );
  if( dg::has_global_weight() ) { 
    g->fillw( x , y , dg::global_weight() );
  } else {
    g->fill( x , y );
  }
  current()->_graphics[ sname ] = g;
}

/* 0D efficiency */
void
DgCollection::filleff( const std::string& sname , const bool& pass )
{
  if( current()->_locked ) { return; } // no changes allowed.
  if( dg::has_global_weight() ) {
    filleffw(sname,pass,dg::global_weight());
    return;
  }
  assert( current() );
  Coll::iterator i = current()->_graphics.find(sname);
  if( i!=current()->_graphics.end() ) {
    i->second->fille( pass );
    return;
  }
  DgEfficiency0D* h = new DgEfficiency0D();
  assert( h );
  h->fille( pass );
  current()->_graphics[ sname ] = h;
}

/* 1D efficiency */
void
DgCollection::filleff( const std::string& sname , const BinSize& nbins , const Value& min , const Value& max ,
		       const Value& x , const bool& pass )
{
  if( current()->_locked ) { return; } // no changes allowed.
  static const std::string empty_string;
  filleff(sname,nbins,min,max,x,pass,empty_string);
}

void
DgCollection::filleff( const std::string& sname , const BinSize& nbins , const Value& min , const Value& max , 
		       const Value& x , const bool& pass , 
                       const std::string& axis_label)
{
  if( current()->_locked ) { return; } // no changes allowed.
  // weighted efficiency not defined for arbitrarily large set of unique weights
  // if( dg::has_global_weight() ) {
  //   filleffw(sname,nbins,min,max,x,pass,dg::global_weight(),axis_label);
  //   return;
  // }
  assert( current() );
  Coll::iterator i = current()->_graphics.find(sname);
  if( i!=current()->_graphics.end() ) {
    i->second->fille( x , pass );
    return;
  }
  DgEfficiency1D* h = new DgEfficiency1D( nbins , min , max );
  assert( h );
  h->fille( x , pass );
  h->set_axis_label( axis_label );
  current()->_graphics[ sname ] = h;
}

/* 2D efficiency */
void
DgCollection::filleff( const std::string& sname , 
                       const BinSize& nbinsx , const Value& minx , const Value& maxx , 
                       const BinSize& nbinsy , const Value& miny , const Value& maxy , 
                       const Value& x , const Value& y , const bool& pass , 
                       const std::string& axis_label_x , const std::string& axis_label_y )
{
  if( current()->_locked ) { return; } // no changes allowed.
  assert( current() );
  Coll::iterator i = current()->_graphics.find(sname);
  if( i!=current()->_graphics.end() ) {
    i->second->fille( x , y , pass );
    return;
  }
  DgEfficiency2D* h = new DgEfficiency2D( nbinsx , minx , maxx , nbinsy , miny , maxy );
  assert( h );
  h->fille( x , y , pass );
  h->set_x_axis_label( axis_label_x );
  h->set_y_axis_label( axis_label_y );
  current()->_graphics[ sname ] = h;  
}

/* 0D efficiency for weighted events */

void
DgCollection::filleffw( const std::string& sname , const bool& pass , const WeightedCount& weight )
{
  if( current()->_locked ) { return; } // no changes allowed.
  assert( current() );
  Coll::iterator i = current()->_graphics.find(sname);
  if( i!=current()->_graphics.end() ) {
    i->second->fillew( pass , weight );
    return;
  }
  DgEfficiencyWeighted0D* h = new DgEfficiencyWeighted0D();
  assert( h );
  h->fillew( pass , weight );
  current()->_graphics[ sname ] = h;
}

// remove a graphics object
void
DgCollection::remove_graphic( const std::string& sname )
{
  if( current()->_locked ) { return; } // no changes allowed
  assert( current() );
  Coll::iterator i = current()->_graphics.find(sname);
  if( i!=current()->_graphics.end() ) {
    delete i->second;
    current()->_graphics.erase( i );
  }
}

unsigned int
DgCollection::search_down( DgCollection::View& view , const std::string& pattern , const DgCollection* icoll )
{
  unsigned int count = 0;
  const DgCollection* head = icoll ? icoll : current();
  assert( head );
  count += head->search( view , pattern );
  for( SubColl::const_iterator i=head->_children.begin(), f=head->_children.end(); i!=f; ++i ) {
    const DgCollection* coll = i->second;
    count += search_down(view,pattern,coll);
  }
  return count;
}

unsigned int
DgCollection::search( DgCollection::View& view , const std::string& pattern ) const
{
  assert( _read_on_demand ); // not implemented for use otherwise
  unsigned int count = 0;
  boost::regex regpat( pattern );
  for( ResourceIDMap::const_iterator i=_resource_id_map.begin(), f=_resource_id_map.end(); i!=f; ++i ) {
    const std::string& sname = i->first;
    assert( i->second );
    if( boost::regex_match( sname , regpat ) ) {
      view.push_back( View::value_type(this,i->first,i->second) );
      //       cout << " " << _name << " contains " << pattern << " as " << strname << endl;
      ++count;
    }
  }
  return count;
}

string
DgCollection::path_to( const std::string& sname , const DgCollection* coll )
{
  const DgCollection* head = coll ? coll : current();
  assert( head );
  string fullpath( sname );
  if( sname != "" ) { 
    //  cout << " name " << sname << " coll " << head->name();
    if( head->is_read_on_demand() ) { 
      ResourceIDMap::const_iterator i = head->_resource_id_map.find(sname);
      if( i==head->_resource_id_map.end() ) { return ""; }
    } else {
      Coll::const_iterator i = head->_graphics.find(sname);
      //assert( i!=head->_graphics.end() );
      if( i==head->_graphics.end() ) { return ""; }
    }
  }
  while( head!=root() ) {
    fullpath = string(head->_name) + "/" + fullpath;
    assert( head->_parent );
    head = head->_parent;
  }
  fullpath = string("/") + fullpath;
  return fullpath;
}

// retrieve graphics object with absolute or relative path.
const DgCollection::View
DgCollection::get( const std::string& path )
{
  // DgCollection* old = current();
  // std::string tmp( path );
  // DgCollection* coll = current() ? current() : root();
  // if( tmp.find("/")==0 ) { 
  //   coll = root();
  //   tmp = tmp.substr(1,tmp.size()-1);
  // }
  // assert( coll );
  // _current = coll;
  // View result;
  // do {
  //   string::size_type slashpos = tmp.find("/");
  //   if( slashpos!=string::npos ) {
  //     string dirname = tmp.substr(0,slashpos);
  //     DgCollection* newcoll = coll->cd( dirname );
  //     if( newcoll ) { coll = newcoll; } else { return View(); }
  //     tmp = tmp.substr(slashpos+1,tmp.size()-slashpos-1);
  //     continue;
  //   }
  //   _current = coll;
  //   result = lookup_graphic( tmp ); 
  //   break;
  // } while( true );
  // _current = old;
  // return result;
  DgCollection* old = current();
  DgCollection* coll = current();
  vector<string> names;
  boost::split( names , path , boost::is_any_of("/") , boost::token_compress_on );
  if( boost::istarts_with( path , "/" ) ) {
    coll = root();
  }
  assert( coll );
  _current = coll;
  View result;
  std::reverse( names.begin() , names.end() );
  while( names.size()>1 ) {
    const string& dirn( names.back() );
    DgCollection* newcoll = coll->cd( dirn );
    if( newcoll ) { coll = newcoll; } else { return View(); }
    names.pop_back();
  }
  _current = coll;
  result = lookup_graphic( names.front() );
  _current = old;
  return result;
}

// retrieve graphics object with absolute or relative path.
const DgCollection*
DgCollection::get_coll( const std::string& path )
{
  DgCollection* old = current();
  std::string tmp( path );
  const DgCollection* coll = current() ? current() : root();
  if( tmp.find("/")==0 ) { 
    coll = root();
    tmp = tmp.substr(1,tmp.size()-1);
  }
  assert( coll );
  do {
    string::size_type slashpos = tmp.find("/");
    if( slashpos!=string::npos ) {
      string dirname = tmp.substr(0,slashpos);
      const DgCollection* newcoll = coll->cd( dirname );
      if( newcoll ) { coll = newcoll; }
      tmp = tmp.substr(slashpos+1,tmp.size()-slashpos-1);
      continue;
    }
    _current = old;
    return coll;
  } while( true );
}


template<class archiveT>
void
DgCollection::save( archiveT & ar , const unsigned int version ) const {
  ar & _name;
  ar & _description;
  Coll::size_type n = _graphics.size();
  ar & n;
  typedef pair<string,DgAbsGraphic*> DgAbsPair;
  BOOST_FOREACH( DgAbsPair dgabspair , _graphics ) {
    string sname( dgabspair.first );
    DgEfficiency0D* eff0d = dynamic_cast<DgEfficiency0D*>(dgabspair.second);
    DgEfficiency1D* eff1d = dynamic_cast<DgEfficiency1D*>(dgabspair.second);
    DgEfficiency2D* eff2d = dynamic_cast<DgEfficiency2D*>(dgabspair.second);
    DgGraph2D* gr2d = dynamic_cast<DgGraph2D*>(dgabspair.second);
    DgNtuple* nt = dynamic_cast<DgNtuple*>(dgabspair.second);
    DgHistogram1D* h1d = dynamic_cast<DgHistogram1D*>(dgabspair.second);
    DgHistogram2D* h2d = dynamic_cast<DgHistogram2D*>(dgabspair.second);
    DgHistogramWeighted1D* hw1d = dynamic_cast<DgHistogramWeighted1D*>(dgabspair.second);
    DgProfile1D* p1d = dynamic_cast<DgProfile1D*>(dgabspair.second);
    unsigned int code;
    if( eff1d ) {
      code = 4;
      ar & code;
      ar & sname;
      ar & eff1d;
    } else if( eff2d ) {
      code = 7;
      ar & code;
      ar & sname;
      ar & eff2d;
    } else if( hw1d ) {
      code = 2;
      ar & code;
      ar & sname;
      ar & hw1d;
    } else if( h1d ) {
      code = 0;
      ar & code;
      ar & sname;
      ar & h1d;
    } else if( h2d ) {
      code = 1;
      ar & code;
      ar & sname;
      ar & h2d;
    } else if( eff0d ) {
      code = 3;
      ar & code;
      ar & sname;
      ar & eff0d;
    } else if( gr2d ) {
      code = 5;
      ar & code;
      ar & sname;
      ar & gr2d;
    } else if( nt ) {
      code = 11;
      ar & code;
      ar & sname;
      ar & gr2d;
    } else if( p1d ) {
      code = 6;
      ar & code;
      ar & sname;
      ar & p1d;
    } else {
      cout << "unrecognized graphic type! cannot save." << endl;
    }    
  }
  ar & _children;
}

template<class archiveT>
void
DgCollection::load( archiveT & ar , unsigned int version ) {
  if( current()->_locked ) { return; } // no changes allowed
  ar & _name;
  ar & _description;
  Coll::size_type ngraphics;
  ar & ngraphics;
  for( Coll::size_type i=0; i!=ngraphics; ++i ) {
    unsigned int code;
    string sname;
    ar & code;
    ar & sname;
    switch( code ) {
    case 0:
      {
        DgEfficiency0D* eff0d = new DgEfficiency0D;
        ar & (*eff0d);
        _graphics[ sname ] = eff0d;
        break;
      }
    case 1:
      {
        DgEfficiency1D* eff1d = new DgEfficiency1D(0,0,0);
        ar & (*eff1d);
        _graphics[ sname ] = eff1d;
        break;
      }
    case 2:
      {
        DgGraph2D* gr2d = new DgGraph2D;
        ar & (*gr2d);
        _graphics[ sname ] = gr2d;
        break;
      }
    case 3:
      {
        DgHistogram1D* h1d = new DgHistogram1D(0,0,0);
        ar & (*h1d);
        _graphics[ sname ] = h1d;
        break;
      }
    case 4:
      {
        DgHistogram2D* h2d = new DgHistogram2D(0,0,0,0,0,0);
        ar & (*h2d);
        _graphics[ sname ] = h2d;
        break;
      }
    case 5:
      {
        DgHistogramWeighted1D* hw1d = new DgHistogramWeighted1D(0,0,0);
        ar & (*hw1d);
        _graphics[ sname ] = hw1d;
        break;
      }
    case 6:
      {
        DgProfile1D* p1d = new DgProfile1D(0,0,0);
        ar & (*p1d);
        _graphics[ sname ] = p1d;
        break;
      }
    case 7:
      {
        DgEfficiency2D* ef2d = new DgEfficiency2D(0,0,0,0,0,0);
        ar & (*ef2d);
        _graphics[ sname ] = ef2d;
        break;
      }
    case 11:
      {
        DgNtuple* gr2d = new DgNtuple;
        ar & (*gr2d);
        _graphics[ sname ] = gr2d;
        break;
      }
    default:
      {
        cout << "unrecognized graphic type! cannot save." << endl;
      }
    }    
  }
  ar & _children;
}

// void
// DgCollection::add_axis( DgAxis* x ) 
// { 
//   assert( current() );
//   assert( x );
//   AxisColl::const_iterator i=_current->_axes.find( x->name() );
//   assert( i==_current->_axes.end() );
//   _current->_axes[ x->name() ] = x;
// }



template void DgCollection::load<boost::archive::text_iarchive>( boost::archive::text_iarchive & ar , unsigned int version );

DgCollection DgCollection::_base( 0 , "root" , "root collection" );
DgCollection* const DgCollection::_root = &DgCollection::_base;
DgCollection* DgCollection::_current = DgCollection::_root;
boost::optional<WeightedCount> DgCollection::_global_weight = boost::optional<WeightedCount>();
std::stack<DgCollection*> DgCollection::_history;
bool DgCollection::save_unique = true;
DgEventInfo DgCollection::_event_info = DgEventInfo();
DgBin DgCollection::_binning = DgBin();
