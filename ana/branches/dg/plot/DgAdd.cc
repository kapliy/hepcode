#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <TFile.h>
#include <TDirectory.h>
#include <TObject.h>
#include <TList.h>
#include <TIterator.h>
#include <TKey.h>
#include <TH1.h>
#include <TROOT.h>
#include <TSystem.h>
#include <TClass.h>
#include <TNtuple.h>
#include <TChain.h>
#include <THashList.h>
#include <TString.h>
#include <TObjString.h>
#include <Riostream.h>

#include <DataGraphics/DataGraphics.hh>

#include <boost/shared_ptr.hpp>
#include <boost/any.hpp>
#include <boost/foreach.hpp>
#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>

bool save_only_ntuple = false;
bool skip_ntuple = false;
int MergeRootFile( TDirectory *targetDG, TDirectory *targetROOT, TList *sourcelist);

using namespace std;
using namespace DataGraphics;
using namespace boost;

//extern TDirectory* gDirectory;
//extern TROOT* gROOT;

map<string,bool> allNames;
unsigned int ITER = 0;
std::string TMPROOT = "";

// main loop ////////////////////////////////////////////////////////////////
int main( int argc , char* argv[] )
{

  // parse arguments using boost::program_options
  using namespace boost::program_options;
  options_description cmdline_options( "dgplot options" );
  cmdline_options.add_options()
    ( "help" , "display help message" )
    ( "union,u" , value< int >(), "Take union of objects in N files (instead of relying on first file structure). Set to 999 to use all files" )
    ( "input,i" , value< vector<string> >() , "input filename(s)" )
    ( "compression,f" , value< int >() , "ROOT compression level" )
    ( "list,l" , value< vector<string> >() , "filenames for text files containing lists of input filename(s)" )
    ( "dg,d" , value<string>() , "merge all input files as DG and save as this filename" )
    ( "root,r" , value<string>() , "merge all input files as ROOT and save as this filename." )
    ( "tmp,t" , value<string>() , "name of temporary file used to merge large tress (DgNtuple objects)" )
    ( "ntuple,n" , "save ntuple only" )
    ( "nontuple,g" , "skip ntuples and save histograms only" )
    ;
  positional_options_description cmdline_positional_options;
  cmdline_positional_options.add( "input" , -1 ); // an unnamed argument anywhere on the command line will be treated as an input filename
  variables_map cmdline;
  try {
    parsed_options tmp( command_line_parser(argc,argv).options(cmdline_options).allow_unregistered().positional(cmdline_positional_options).run() ); 
    store( tmp , cmdline ); 
    notify( cmdline ); 
  } catch(...) {
    cerr << "ERROR: " << cmdline_options << endl;
    exit(-1);
  }
  if( cmdline.count( "help" ) ) { 
    cout << cmdline_options << endl;
    exit(-1);
  }

  save_only_ntuple = cmdline.count( "ntuple" )>0;
  skip_ntuple = cmdline.count( "nontuple" )>0;
  int union_size = 1;
  if( cmdline.count( "union" ) ) {
    union_size = cmdline[ "union" ].as<int>();
  }
  int compression = 1;
  if( cmdline.count( "compression" ) ) {
    compression = cmdline[ "compression" ].as<int>();
  }
  vector<string> input_filenames;
  if( cmdline.count( "input" ) ) {
    input_filenames = cmdline[ "input" ].as< vector<string> >();
  }
  if( cmdline.count("list") ) {
    vector<string> inlists = cmdline["list"].as< vector<string> >();
    for( vector<string>::const_iterator i=inlists.begin(), f=inlists.end(); i!=f; ++i ) {
      ifstream inf( (*i).c_str() );
      if(!inf.is_open()) {
	std::cout << "ERROR: cannot open input file list: " <<  (*i).c_str() << std::endl;
	continue;
      }
      while( !(inf.eof()) ) { 
        string line;
        getline( inf , line );
        if( line != "" ) { input_filenames.push_back( line ); }
      }
    }
  }
  if( input_filenames.empty() ) {
    cerr << cmdline_options << endl;
    cerr << endl << " input file list is empty or missing." << endl;
    exit(-1);
  }
  
  const bool save_input_as_dg = true;
  const string save_input_as_dg_filename = cmdline.count("dg") ? cmdline["dg"].as< string >() : string("merged.root");
  const bool save_input_as_root = cmdline.count("root");
  const string save_input_as_root_filename = save_input_as_root ? cmdline["root"].as< string >() : string("");
  const bool manual_root_tmpfile = cmdline.count("tmp");
  TMPROOT = manual_root_tmpfile ? cmdline["tmp"].as< string >() : save_input_as_dg_filename + ".tmp.root";
  if(!save_input_as_dg) {
    cout << cmdline_options << endl;
    cerr << endl << " please specify the name of dg file using --dg" << endl;
    exit(-1);
  }

  // the following line forces loading of ROOT dictionaries
  DgCollection* bg_root = dg::root();

  TList *FileList = new TList();
  BOOST_FOREACH( const string& filename , input_filenames ) {
    TFile *ftmp = TFile::Open(filename.c_str());
    if( !ftmp || !ftmp->IsOpen() || ftmp->IsZombie() ) {
      cerr << " could not open " << filename << "." << endl;
    }
    else {
      FileList->Add( ftmp  );
    }
  }
  if( FileList->GetSize()==0 ) {
    cerr << endl << " None of the specified input files could be opened." << endl;
    exit(-1);
  }

  const Long64_t maxtreesize = 9900000000LL; // ROOT default: 1900000000
  TTree::SetMaxTreeSize(maxtreesize);
  
  TFile * targetDG = TFile::Open( save_input_as_dg_filename.c_str(), "RECREATE" );
  targetDG->SetCompressionLevel(compression);
  TFile * targetROOT = 0;
  if(save_input_as_root) {
    targetROOT = TFile::Open( save_input_as_root_filename.c_str(), "RECREATE" );
    targetROOT->SetCompressionLevel(compression);
  }

  // repeat nfiles times to ensure we save a *union* of objects in all files
  for(unsigned int istart=0; istart < FileList->GetSize(); istart++) {
    cout << "Iteration " << istart << " / " << FileList->GetSize() << endl;
    fflush(stdout);
    // perform circular rotation of files in FileList and save in clist
    TList *clist = new TList();
    for(unsigned int i=istart; i<FileList->GetSize(); i++) {
      TFile *f = dynamic_cast<TFile*>(FileList->At(i));
      assert(f && f->IsOpen() && !f->IsZombie() );
      clist->Add(f);
    }
    for(unsigned int i=0; i<istart; i++) {
      TFile *f = dynamic_cast<TFile*>(FileList->At(i));
      assert(f && f->IsOpen() && !f->IsZombie() );
      clist->Add(f);
    }
    ITER=istart;
    const int nmerged = MergeRootFile(targetDG,targetROOT,clist);
    cout << "Iteration " << istart << " done: merged " << nmerged << " objects" << endl;
    fflush(stdout);
    clist->Clear("nodelete");
    //delete clist;
    if(istart+1>=union_size) break;
  }


  cout << "Finalizing DG file..." << endl; fflush(stdout);
  targetDG->SaveSelf(kTRUE&&!(save_only_ntuple));
  cout << "Finalizing ROOT file..." << endl; fflush(stdout);
  if(targetROOT) targetROOT->SaveSelf(kTRUE);

  fflush(stdout);
  cout << "Done!" << endl;
  // ROOT sometimes crashes in TFile destructors. Prevent this by aborting outright.
  abort();

  cout << "Closing DG file..." << endl; fflush(stdout);
  if(targetDG) targetDG->Close();
  delete targetDG;
  cout << "Closing ROOT file..." << endl; fflush(stdout);
  if(targetROOT) targetROOT->Close();
  delete targetROOT;

  fflush(stdout);

  return 0;
}

// recursive merge function
// based on code from $ROOTSYS/tutorials/io/hadd.C
int MergeRootFile( TDirectory *targetDG, TDirectory *targetROOT, TList *sourcelist) {
  //  cout << "Target path: " << targetDG->GetPath() << endl;

  TString path( (char*)strstr( targetDG->GetPath(), ":" ) );
  path.Remove( 0, 2 );
  int nobj = 0;
  TFile *first_source = (TFile*)sourcelist->First();
  
  first_source->cd( path );
  TDirectory *current_sourcedir = gDirectory;
  Bool_t status = TH1::AddDirectoryStatus();
  TH1::AddDirectory(kFALSE);
  // loop over all keys in this directory
  TIter nextkey( current_sourcedir->GetListOfKeys() );
  TKey *key;
  while ( (key = (TKey*)nextkey())) {
    // read object from first source file
    first_source->cd( path );
    const char* name = key->GetName();
    string fullpath = string(path.Data()) + string("/") + string(name);
    TObject *obj = key->ReadObj();
    assert(obj);
    DgAbsGraphic* gobj = dynamic_cast<DgAbsGraphic*>(obj);
    DgNtuple* ntobj = dynamic_cast<DgNtuple*>(obj);

    // a subdirectory
    if ( obj->IsA()->InheritsFrom( "TDirectory" ) ) {
      //cout << "Directory: " << name << endl;
      // create a new subdir of same name and title in the target file
      targetDG->cd();
      TDirectory *newdirDG = 0;
      TDirectory *newdirROOT = 0;
      if(allNames.find(fullpath)==allNames.end()) {
	newdirDG = targetDG->mkdir( name, obj->GetTitle() );
      }
      else newdirDG = targetDG->GetDirectory(name);
      if(targetROOT) {
	targetROOT->cd();
	if (allNames.find(fullpath)==allNames.end()) {
	  cout << "DIR (" << ITER << ") : " << fullpath << endl;
	  newdirROOT = targetROOT->mkdir( name, obj->GetTitle() );
	}
	else newdirROOT = targetROOT->GetDirectory(name);
      }
      allNames[fullpath]=true;

      // newdir is now the starting point of another round of merging
      // newdir still knows its depth within the target file via
      // GetPath(), so we can still figure out where we are in the recursion

      nobj += MergeRootFile( newdirDG, newdirROOT, sourcelist);
    }
    // a DgNtuple
    else if(ntobj){
      if (skip_ntuple) { delete obj; continue; }
      if (allNames.find(fullpath)!=allNames.end()) { delete obj; continue; }
      allNames[fullpath]=true;
      if(targetROOT) { // only save in ROOT format
	// merge multiple DgNtuples via a temporary file (to conserve RAM)
	TFile *dump = TFile::Open(TMPROOT.c_str(),"RECREATE");
	assert(dump->IsOpen());
	assert(!dump->IsZombie());
	dump->cd();
	int imerge = 0;
	int ifile = 0;
	ntobj->save_as_ROOT(TString::Format("tmp%d",imerge) , gDirectory );
	delete obj; obj = 0;
	cout << "--> " << imerge << " | " << ifile << " : " << fullpath << endl;
	imerge++; ifile++;
	TFile *nextsource = (TFile*)sourcelist->After( first_source );
	while ( nextsource ) {
	  if(nextsource->GetDirectory(path)) {
	    nextsource->cd( path );
	    TKey *key2 = (TKey*)gDirectory->GetListOfKeys()->FindObject(name);
	    if (key2) {
	      const DgNtuple* ntobj2 = dynamic_cast<const DgNtuple*>(key2->ReadObj());
	      assert(ntobj2);
	      dump->cd();
	      ntobj2->save_as_ROOT(TString::Format("tmp%d",imerge) , gDirectory );
	      cout << "--> " << imerge << " | " << ifile << " : " << nextsource->GetName() << endl;
	      imerge++;
	      delete ntobj2;
	    }
	  }
	  nextsource = (TFile*)sourcelist->After( nextsource );
	  ifile++;
	}
	dump->Close();
	// save all temporary trees to the final location
	targetROOT->cd();
	TChain *globChain = new TChain(name);
	for(int jmerge=0; jmerge<imerge; jmerge++) {
	  globChain->AddFile(TMPROOT.c_str(),TChain::kBigNumber,TString::Format("tmp%d",jmerge));
	}
	targetROOT->cd();
	globChain->Merge(targetROOT->GetFile(),0,"keep fast");
	delete globChain;
	gSystem->Unlink(TMPROOT.c_str());
      }
    }
    // a generic DgAbsGraphic
    else if(gobj){
      if(save_only_ntuple) continue; // cy
      if (allNames.find(fullpath)!=allNames.end()) { delete obj; continue; }
      allNames[fullpath]=true;
      // loop over all source files and add the content of the
      // correspondant histogram to the one pointed to by "h1"
      TFile *nextsource = (TFile*)sourcelist->After( first_source );
      while ( nextsource ) {
        // make sure we are at the correct directory level by cd'ing to path
	if(nextsource->GetDirectory(path)) {
	  nextsource->cd( path );
	  TKey *key2 = (TKey*)gDirectory->GetListOfKeys()->FindObject(name);
	  if (key2) {
	    TObject *obj2 = key2->ReadObj();
	    const DgAbsGraphic* gobj2 = dynamic_cast<const DgAbsGraphic*>(obj2);
	    assert(gobj2);
	    gobj->merge(gobj2);
	    delete obj2;
	  }
	}
        nextsource = (TFile*)sourcelist->After( nextsource );
      }
    }
    // anything else - ignore
    else {
      // object is of no type that we know or can handle
      cerr << "Stumbled on an object that doesn't derive from DgAbsGraphic. Current directory: " << path << endl;
      exit(-1);
    }
      
    // now write the merged object to the target file
    // Note that DgNtuples have already been saved, and its obj deleted
    if(ntobj) {
      nobj++;
    }
    else if ( obj ) {
      if ( !obj->IsA()->InheritsFrom( "TDirectory" ) ) nobj++;
      targetDG->cd();
      obj->Write( name, TObject::kOverwrite );
      if(targetROOT) {
	targetROOT->cd();
	if ( obj->IsA()->InheritsFrom( "TDirectory" ) ) {
	  obj->Write( name );
	}
	else if(gobj) {
	  if(save_only_ntuple) continue;
	  gobj->save_as_ROOT(name , gDirectory );
	}
	else assert(0);
      }
      delete obj;
    }
    
  } // end looping over all objects
  
  targetDG->SaveSelf(kTRUE&&!(save_only_ntuple));
  if(targetROOT) targetROOT->SaveSelf(kTRUE);

  TH1::AddDirectory(status);

  return nobj;
}
