
#include <DataGraphics/DgTypes.hh>
#include <DataGraphics/DgCollection.hh>
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
#include <fstream>

#include <TObject.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TH3D.h>
#include <BootstrapGenerator/TH1Bootstrap.h>
#include <BootstrapGenerator/TH2Bootstrap.h>
#include <TNtuple.h>
#include <TDirectory.h>
#include <TKey.h>
#include <TCollection.h>
#include <TList.h>
#include <TString.h>
#include <TFile.h>
#include <TClass.h>

#include <BootstrapGenerator/BootstrapGenerator.h>

using namespace std;
using namespace DataGraphics;
using namespace __gnu_cxx;

void
DgCollection::set_save_filename( const std::string& _filename , bool force_unique ) {
  assert(!_file && "ERROR: DgCollection output name has already been defined");
  std::string unique_name = force_unique
    ? DataGraphics::Tools::expand_filename_to_valid_unique( (_filename)+".root" )
    : DataGraphics::Tools::expand_filename_to_valid( (_filename)+".root" );
  _file = TFile::Open(unique_name.c_str() , "RECREATE");
  _current = _file->GetDirectory("/");
  _current->cd();
  _mapH[ _current->GetPath() ] = std::vector<std::string>();
  _mapB[ _current->GetPath() ] = std::map<std::string,TObject*>();
  _mapT[ _current->GetPath() ] = std::vector<std::string>();
}

void
DgCollection::initialize_bootstrap(const std::string& bname, const int nreplicas){
  if(_boot) {
    std::cerr << "WARNING: overwriting old bootstrap generator" << std::endl;
    delete _boot;
  }
  _replicas = nreplicas;
  _boot = new BootstrapGenerator(bname.c_str(),bname.c_str(),nreplicas);
}

void
DgCollection::generate_bootstrap(unsigned int RunNumber, unsigned int EventNumber) {
  assert(_boot && "ERROR: must call initialize_bootstrap() first");
  _boot->Generate(RunNumber, EventNumber);
}
    
void
DgCollection::dump_names() {
  std::map<std::string, std::vector<string> >::iterator imB;
  std::map<std::string, std::map<string,TObject*> >::iterator imC;
  std::map<string,TObject*>::iterator imD;
  std::cout << "================== DUMP HISTOGRAM NAMES ==================" << std::endl;
  for(imB=_mapH.begin(); imB!=_mapH.end(); imB++) {
    std::cout << " ----> " << imB->first << std::endl;
    for(int i=0; i<imB->second.size(); i++) {
      std::cout << " --> " << imB->second.at(i) << std::endl;
    }
  }
  std::cout << "================== DUMP BOOTSTRAP NAMES ==================" << std::endl;
  for(imC=_mapB.begin(); imC!=_mapB.end(); imC++) {
    std::cout << " ----> " << imC->first << std::endl;
    for(imD=imC->second.begin(); imD!=imC->second.end(); imD++) {
      std::cout << " --> " << imD->first << std::endl;
    }
  }
  std::cout << "================== DUMP NTUPLE NAMES ==================" << std::endl;
  for(imB=_mapT.begin(); imB!=_mapT.end(); imB++) {
    if( !imB->second.empty() ) {
      std::cout << " ----> " << imB->first << std::endl;
      for(int i=0; i<imB->second.size(); i++) {
	std::cout << " --> " << imB->second.at(i) << std::endl;
      }
    }
  }
  std::cout << "================== DUMP DONE ==================" << std::endl;
}

// make sure TTrees are saved as disk-resident
void
DgCollection::finalize() {
  std::map<std::string, std::vector<string> >::iterator imB;
  std::map<std::string, std::map<string,TObject*> >::iterator imC;
  std::map<string,TObject*>::iterator imD;
  // flush ntuples
  for(imB=_mapT.begin(); imB!=_mapT.end(); imB++) {
    for(int i=0; i<imB->second.size(); i++) {
      TNamed *o = (TNamed*) _file->Get( (std::string(imB->first) + "/" + std::string(imB->second.at(i))).c_str() );
      if( o && o->InheritsFrom("TNtuple") ) {
	const bool st = _file->cd((imB->first).c_str());
	assert( st && "error: unable to cd into a directory during finalize() ");
	if(true) {
	  std::cout << "Flushing ntuple: " << (std::string(imB->first) + "/" + std::string(imB->second.at(i))) << std::endl;
	}
	TNtuple *T = (TNtuple*)o;
	T->FlushBaskets();
      }
    }
  }
  // save bootstrap histograms since they are always memory-residet
  for(imC=_mapB.begin(); imC!=_mapB.end(); imC++) {
    if( imC->second.empty() ) continue;
    const bool st = _file->cd((imC->first).c_str());
    assert( st && "error: unable to cd into a directory during finalize() ");
    _current = _file->CurrentDirectory();
    assert( (std::string(_current->GetPath()) == imC->first) && "Critical error: directory name mismatch");
    for(imD=imC->second.begin(); imD!=imC->second.end(); imD++) {
      TH1DBootstrap *hsave = (TH1DBootstrap*)(imD->second);
      if( _current->Get( hsave->GetName() ) ){
	std::cerr << "ERROR: " << hsave->GetName() << " already exists in: "
		  << _current->GetName() << std::endl;
      }
      hsave->Write(hsave->GetName(), TObject::kOverwrite);
    }
  }
}

void
DgCollection::save(bool verbose)
{
  if(verbose) dump_names();
  std::string unique_name ( _file->GetName() );
  cout << " [ dg write: " << unique_name << " ] " << endl;
  cout << " view output with " << endl << "   root -l " << unique_name << endl;
  finalize();
  _file->Write();
  _file->Close();
  delete _file;
  _file = 0;
  _current = 0;
  cout << " [ dg write complete. ] " << endl;
}

void
DgCollection::down( const std::string& sname , const std::string& description ) {
  assert( sname != "" );
  assert( _current && "ERROR: cwd is not set" );
  TDirectory *next = (TDirectory*)_current->Get(sname.c_str());
  if(!next) {
    next = _current->mkdir( sname.c_str() , description.c_str() );
    _mapH[ next->GetPath() ] = std::vector<std::string>();
    _mapB[ next->GetPath() ] = std::map<std::string, TObject*>();
    _mapT[ next->GetPath() ] = std::vector<std::string>();
  }
  _current = next;
  _current->cd();
}
void
DgCollection::up() {
  const bool status = _current->cd("../");
  assert( status );
  _current = _file->CurrentDirectory();
}

/* 1D histograms */
void
DgCollection::fillh( const std::string& sname , const BinSize& nbins , const Value& min , const Value& max ,
		     const Value& x , const std::string& axis_label )
{
  return fillhw(sname,nbins,min,max,x,dg::global_weight(),axis_label);
}
void
DgCollection::fillvh( const std::string& sname , const BinSize& nbins , const std::vector<Value>& bins ,
		      const Value& x , const std::string& axis_label )
{
  return fillvhw(sname,nbins,bins,x,dg::global_weight(),axis_label);
}
void
DgCollection::fillhw( const std::string& sname , const BinSize& nbins , const Value& min , const Value& max , 
		      const Value& x , const WeightedCount& weight, const std::string& axis_label )
{
  TH1* h = (TH1*)_current->Get(sname.c_str());
  if(!h) {
    h = new TH1D(sname.c_str(),axis_label.c_str(),nbins,min,max);
    h->SetDirectory( _current );
    h->Sumw2();
    _mapH[ _current->GetPath() ].push_back( h->GetName() );
  }
  h->Fill( x , weight );
  return;
}
void
DgCollection::fillvhw( const std::string& sname , const BinSize& nbins , const std::vector<Value>& bins ,
		       const Value& x , const WeightedCount& weight, const std::string& axis_label )
{
  TH1* h = (TH1*)_current->Get(sname.c_str());
  if(!h) {
    h = new TH1D(sname.c_str(),axis_label.c_str(),nbins,&bins[0]);
    h->SetDirectory( _current );
    h->Sumw2();
    _mapH[ _current->GetPath() ].push_back( h->GetName() );
  }
  h->Fill( x , weight );
  return;
}

/* 2D histograms */
void
DgCollection::fillh( const std::string& sname , 
                     const BinSize& nbinsx , const Value& minx , const Value& maxx , 
                     const BinSize& nbinsy , const Value& miny , const Value& maxy , 
                     const Value& x , const Value& y , const std::string& axis_label_x , const std::string& axis_label_y )
{
  return fillhw(sname,nbinsx,minx,maxx,nbinsy,miny,maxy,x,y,dg::global_weight(),axis_label_x,axis_label_y);
}
void
DgCollection::fillvh( const std::string& sname , 
		      const BinSize& nbinsx , const std::vector<Value>& binsx ,
		      const BinSize& nbinsy , const std::vector<Value>& binsy ,
		      const Value& x , const Value& y , const std::string& axis_label_x , const std::string& axis_label_y )
{
  return fillvhw(sname,nbinsx,binsx,nbinsy,binsy,x,y,dg::global_weight(),axis_label_x,axis_label_y);
}
void
DgCollection::fillhw( const std::string& sname , 
		      const BinSize& nbinsx , const Value& minx , const Value& maxx , 
		      const BinSize& nbinsy , const Value& miny , const Value& maxy , 
		      const Value& x , const Value& y , const WeightedCount& weight ,
		      const std::string& axis_label_x , const std::string& axis_label_y )
{
  TH2* h = (TH2*)_current->Get(sname.c_str());
  if(!h) {
    h = new TH2D(sname.c_str(),(axis_label_x+" vs "+axis_label_y).c_str(),
		 nbinsx,minx,miny,nbinsy,miny,maxy);
    h->SetDirectory( _current );
    h->Sumw2();
    _mapH[ _current->GetPath() ].push_back( h->GetName() );
  }
  h->Fill( x , y , weight );
  return;
}
void
DgCollection::fillvhw( const std::string& sname , 
		       const BinSize& nbinsx , const std::vector<Value>& binsx ,
		       const BinSize& nbinsy , const std::vector<Value>& binsy ,
		       const Value& x , const Value& y , const WeightedCount& weight ,
		       const std::string& axis_label_x , const std::string& axis_label_y )
{
  TH2* h = (TH2*)_current->Get(sname.c_str());
  if(!h) {
    h = new TH2D(sname.c_str(),(axis_label_x+" vs "+axis_label_y).c_str(),
		 nbinsx,&binsx[0],nbinsy,&binsy[0]);
    h->SetDirectory( _current );
    h->Sumw2();
    _mapH[ _current->GetPath() ].push_back( h->GetName() );
  }
  h->Fill( x , y , weight );
  return;
}

/* 3d histograms */
void
DgCollection::fillh( const std::string& sname , 
		     const BinSize& nbinsx , const Value& minx , const Value& maxx , 
		     const BinSize& nbinsy , const Value& miny , const Value& maxy ,
		     const BinSize& nbinsz , const Value& minz , const Value& maxz , 
		     const Value& x , const Value& y , const Value& z ,
		     const std::string& axis_label_x , const std::string& axis_label_y , const std::string& axis_label_z )
{
  return fillhw(sname,nbinsx,minx,maxx,nbinsy,miny,maxy,nbinsz,minz,maxz,x,y,z,dg::global_weight(),axis_label_x,axis_label_y,axis_label_z);
}
void
DgCollection::fillvh( const std::string& sname , 
		      const BinSize& nbinsx , const std::vector<Value>& binsx ,
		      const BinSize& nbinsy , const std::vector<Value>& binsy ,
		      const BinSize& nbinsz , const std::vector<Value>& binsz ,
		      const Value& x , const Value& y , const Value& z ,
		      const std::string& axis_label_x , const std::string& axis_label_y , const std::string& axis_label_z )
{
  return fillvhw(sname,nbinsx,binsx,nbinsy,binsy,nbinsz,binsz,x,y,z,dg::global_weight(),axis_label_x,axis_label_y,axis_label_z);
}
void
DgCollection::fillhw( const std::string& sname , 
		      const BinSize& nbinsx , const Value& minx , const Value& maxx , 
		      const BinSize& nbinsy , const Value& miny , const Value& maxy ,
		      const BinSize& nbinsz , const Value& minz , const Value& maxz , 
		      const Value& x , const Value& y , const Value& z , const WeightedCount& weight ,
		      const std::string& axis_label_x , const std::string& axis_label_y , const std::string& axis_label_z )
{
  TH3* h = (TH3*)_current->Get(sname.c_str());
  if(!h) {
    h = new TH3D(sname.c_str(),(axis_label_x+" vs "+axis_label_y+" vs "+axis_label_z).c_str(),
		 nbinsx,minx,maxx,nbinsy,miny,maxy,nbinsz,minz,maxz);
    h->SetDirectory( _current );
    h->Sumw2();
    _mapH[ _current->GetPath() ].push_back( h->GetName() );
  }
  h->Fill( x , y , z, weight );
  return;
}

void
DgCollection::fillvhw( const std::string& sname ,
		       const BinSize& nbinsx , const std::vector<Value>& binsx ,
		       const BinSize& nbinsy , const std::vector<Value>& binsy ,
		       const BinSize& nbinsz , const std::vector<Value>& binsz ,
		       const Value& x , const Value& y , const Value& z , const WeightedCount& weight ,
		       const std::string& axis_label_x , const std::string& axis_label_y , const std::string& axis_label_z )
{
  TH3* h = (TH3*)_current->Get(sname.c_str());
  if(!h) {
    h = new TH3D(sname.c_str(),(axis_label_x+" vs "+axis_label_y+" vs "+axis_label_z).c_str(),
		 nbinsx,&binsx[0],nbinsy,&binsy[0],nbinsz,&binsz[0]);
    h->SetDirectory( _current );
    h->Sumw2();
    _mapH[ _current->GetPath() ].push_back( h->GetName() );
  }
  h->Fill( x , y , z, weight );
  return;
}

/* profile histograms */ 
void
DgCollection::fillf( const std::string& sname ,
		     const BinSize& nbins , const Value& min , const Value& max ,
		     const Value& x , const Value& y ,  
                     const std::string& axis_label_x , const std::string& axis_label_y )
{
  return fillfw(sname,nbins,min,max,x,y,dg::global_weight(),axis_label_x,axis_label_y);
}
void
DgCollection::fillfw( const std::string& sname ,
		      const BinSize& nbins , const Value& min , const Value& max ,
		      const Value& x , const Value& y , const WeightedCount& weight ,
		      const std::string& axis_label_x , const std::string& axis_label_y )
{
  assert(0 && "fillfw not implemented");
}


/* ntuples (arrays of floats) */
void
DgCollection::fillnt( const std::string& sname ,
		      const std::vector<float>& x, const std::vector<std::string>& names )
{
  return fillntw(sname,x,names,dg::event_info());
}
const std::string nt_get_varlist(const std::vector<string>& names) { 
  if (names.empty()) return "";
  std::string v = names.at(0);
  for(unsigned int i=1; i<names.size(); i++) {
    v += ":" + names.at(i);
  }
  return v;
};
void
DgCollection::fillntw( const std::string& sname , 
		       const std::vector<float>& x, const std::vector<std::string>& names, 
		       const DgEventInfo& w )
{
  assert(x.size() == names.size());
  TNtuple* h = (TNtuple*)_current->Get(sname.c_str());
  if(!h) {
    std::vector<std::string> nn = w.get_names();
    nn.insert(nn.end(),names.begin(),names.end());
    const string varlist_str = nt_get_varlist(nn);
    h = new TNtuple(sname.c_str(), "" , varlist_str.c_str());
    _mapT[ _current->GetPath() ].push_back( h->GetName() );
  }
  std::vector<float> xx = w.get_values();
  xx.insert(xx.end(),x.begin(),x.end());
  assert( h->GetNvar() == xx.size() );
  h->Fill(&(xx[0]));
  return;
}

void
DgCollection::fillgraph( const std::string& sname , const Value& x, const Value& y , const std::string& axis_label_x , const std::string& axis_label_y )
{
  assert(0 && "fillgraph not implemented");
}

/* 0D efficiency */
void
DgCollection::filleff( const std::string& sname , const bool& pass )
{
  return filleffw(sname,pass,dg::global_weight());
}

/* 0D efficiency for weighted events
   The objective is really to save numerator and denominator
 */
void
DgCollection::filleffw( const std::string& sname , const bool& pass , const WeightedCount& weight )
{
  std::string snum = sname + "_num";
  std::string sden = sname + "_den";
  TH1* hnum = (TH1*)_current->Get(snum.c_str());
  TH1* hden = (TH1*)_current->Get(sden.c_str());
  if(!hnum) {
    assert(!hden);
    hnum = new TH1D(snum.c_str(),snum.c_str(),1,0,1);
    hnum->SetDirectory( _current );
    hnum->Sumw2();
    _mapH[ _current->GetPath() ].push_back( hnum->GetName() );
    hden = new TH1D(sden.c_str(),sden.c_str(),1,0,1);
    hden->SetDirectory( _current );
    hden->Sumw2();
    _mapH[ _current->GetPath() ].push_back( hden->GetName() );
  }
  hden->Fill( 0.5 , weight );
  if(pass) hnum->Fill( 0.5 , weight );
  return;
}

/* 1D efficiency (unweighted) */
void
DgCollection::filleff( const std::string& sname , const BinSize& nbins , const Value& min , const Value& max , 
		       const Value& x , const bool& pass , 
                       const std::string& axis_label)
{
  std::string snum = sname + "_num";
  std::string sden = sname + "_den";
  TH1* hnum = (TH1*)_current->Get(snum.c_str());
  TH1* hden = (TH1*)_current->Get(sden.c_str());
  if(!hnum) {
    assert(!hden);
    hnum = new TH1D(snum.c_str(),snum.c_str(),nbins,min,max);
    hnum->SetDirectory( _current );
    _mapH[ _current->GetPath() ].push_back( hnum->GetName() );
    hden = new TH1D(sden.c_str(),sden.c_str(),nbins,min,max);
    hden->SetDirectory( _current );
    _mapH[ _current->GetPath() ].push_back( hden->GetName() );
  }
  hden->Fill( x );
  if(pass) hnum->Fill( x );
  return;
}

/* 2D efficiency (unweighted) */
void
DgCollection::filleff( const std::string& sname , 
                       const BinSize& nbinsx , const Value& minx , const Value& maxx , 
                       const BinSize& nbinsy , const Value& miny , const Value& maxy , 
                       const Value& x , const Value& y , const bool& pass , 
                       const std::string& axis_label_x , const std::string& axis_label_y )
{
  std::string snum = sname + "_num";
  std::string sden = sname + "_den";
  TH2* hnum = (TH2*)_current->Get(snum.c_str());
  TH2* hden = (TH2*)_current->Get(sden.c_str());
  if(!hnum) {
    assert(!hden);
    hnum = new TH2D(snum.c_str(),snum.c_str(),nbinsx,minx,maxx,nbinsy,miny,maxy);
    hnum->SetDirectory( _current );
    _mapH[ _current->GetPath() ].push_back( hnum->GetName() );
    hden = new TH2D(sden.c_str(),sden.c_str(),nbinsx,minx,maxx,nbinsy,miny,maxy);
    hden->SetDirectory( _current );
    _mapH[ _current->GetPath() ].push_back( hden->GetName() );
  }
  hden->Fill( x , y );
  if(pass) hnum->Fill( x , y );
  return;
}

/*
  -----------------------------------------
  BOOTSTRAP HISTOGRAMS (resident in memory until writing)
  -----------------------------------------
*/

/* 1D histograms */
void
DgCollection::bfillh( const std::string& sname , const BinSize& nbins , const Value& min , const Value& max ,
		      const Value& x , const std::string& axis_label )
{
  return bfillhw(sname,nbins,min,max,x,dg::global_weight(),axis_label);
}
void
DgCollection::bfillvh( const std::string& sname , const BinSize& nbins , const std::vector<Value>& bins ,
		       const Value& x , const std::string& axis_label )
{
  return bfillvhw(sname,nbins,bins,x,dg::global_weight(),axis_label);
}
void
DgCollection::bfillhw( const std::string& sname , const BinSize& nbins , const Value& min , const Value& max , 
		       const Value& x , const WeightedCount& weight, const std::string& axis_label )
{
  std::map<std::string,TObject*>& dlist = _mapB.find(_current->GetPath())->second;
  TH1DBootstrap* h = (dlist.find(sname)!=dlist.end()) ? (TH1DBootstrap*)dlist.find(sname)->second : 0;
  if(!h) {
    h = new TH1DBootstrap(sname.c_str(),axis_label.c_str(),nbins,min,max, _replicas,_boot );
    //h->SetDirectory( _current );
    //h->Sumw2();
    _mapB[ _current->GetPath() ][ h->GetName() ] = h;
  }
  h->Fill( x , weight );
  return;
}
void
DgCollection::bfillvhw( const std::string& sname , const BinSize& nbins , const std::vector<Value>& bins ,
			const Value& x , const WeightedCount& weight, const std::string& axis_label )
{
  assert(_mapB.find(_current->GetPath()) != _mapB.end() );
  std::map<std::string,TObject*>& dlist = _mapB.find(_current->GetPath())->second;
  TH1DBootstrap* h = (dlist.find(sname)!=dlist.end()) ? (TH1DBootstrap*)dlist.find(sname)->second : 0;
  if(!h) {
    h = new TH1DBootstrap(sname.c_str(),axis_label.c_str(),nbins,&bins[0], _replicas,_boot);
    //h->SetDirectory( _current );
    //h->Sumw2();
    _mapB[ _current->GetPath() ][ h->GetName() ] = h;
  }
  h->Fill( x , weight );
  return;
}

/* 2D histograms */
void
DgCollection::bfillh( const std::string& sname , 
		      const BinSize& nbinsx , const Value& minx , const Value& maxx , 
		      const BinSize& nbinsy , const Value& miny , const Value& maxy , 
		      const Value& x , const Value& y , const std::string& axis_label_x , const std::string& axis_label_y )
{
  return bfillhw(sname,nbinsx,minx,maxx,nbinsy,miny,maxy,x,y,dg::global_weight(),axis_label_x,axis_label_y);
}
void
DgCollection::bfillvh( const std::string& sname , 
		       const BinSize& nbinsx , const std::vector<Value>& binsx ,
		       const BinSize& nbinsy , const std::vector<Value>& binsy ,
		       const Value& x , const Value& y , const std::string& axis_label_x , const std::string& axis_label_y )
{
  return bfillvhw(sname,nbinsx,binsx,nbinsy,binsy,x,y,dg::global_weight(),axis_label_x,axis_label_y);
}
void
DgCollection::bfillhw( const std::string& sname , 
		       const BinSize& nbinsx , const Value& minx , const Value& maxx , 
		       const BinSize& nbinsy , const Value& miny , const Value& maxy , 
		       const Value& x , const Value& y , const WeightedCount& weight ,
		       const std::string& axis_label_x , const std::string& axis_label_y )
{
  std::map<std::string,TObject*>& dlist = _mapB.find(_current->GetPath())->second;
  TH2DBootstrap* h = (dlist.find(sname)!=dlist.end()) ? (TH2DBootstrap*)dlist.find(sname)->second : 0;
  if(!h) {
    h = new TH2DBootstrap(sname.c_str(),(axis_label_x+" vs "+axis_label_y).c_str(),
			  nbinsx,minx,miny,nbinsy,miny,maxy,
			  _replicas,_boot);
    //h->SetDirectory( _current );
    //h->Sumw2();
    _mapB[ _current->GetPath() ][ h->GetName() ] = h;
  }
  h->Fill( x , y , weight );
  return;
}
void
DgCollection::bfillvhw( const std::string& sname , 
			const BinSize& nbinsx , const std::vector<Value>& binsx ,
			const BinSize& nbinsy , const std::vector<Value>& binsy ,
			const Value& x , const Value& y , const WeightedCount& weight ,
			const std::string& axis_label_x , const std::string& axis_label_y )
{
  std::map<std::string,TObject*>& dlist = _mapB.find(_current->GetPath())->second;
  TH2DBootstrap* h = (dlist.find(sname)!=dlist.end()) ? (TH2DBootstrap*)dlist.find(sname)->second : 0;
  if(!h) {
    h = new TH2DBootstrap(sname.c_str(),(axis_label_x+" vs "+axis_label_y).c_str(),
			  nbinsx,&binsx[0],nbinsy,&binsy[0],
			  _replicas,_boot);
    //h->SetDirectory( _current );
    //h->Sumw2();
    _mapB[ _current->GetPath() ][ h->GetName() ] = h;
  }
  h->Fill( x , y , weight );
  return;
}

DgCollection DgCollection::_base = DgCollection();
TFile* DgCollection::_file = 0;
TDirectory* DgCollection::_current = 0;
WeightedCount DgCollection::_global_weight = 1.0;
DgEventInfo DgCollection::_event_info = DgEventInfo();
DgBin DgCollection::_binning = DgBin();
BootstrapGenerator* DgCollection::_boot = 0;
int DgCollection::_replicas = 0;
std::map< std::string , std::vector<std::string> > DgCollection::_mapH = std::map< std::string , std::vector<std::string> >();
std::map< std::string , std::map<std::string,TObject*> > DgCollection::_mapB = std::map< std::string , std::map<std::string,TObject*> >();
std::map< std::string , std::vector<std::string> > DgCollection::_mapT = std::map< std::string , std::vector<std::string> >();
