// macro to merge RooUnfoldResponse matrices, with weights

#include <iostream>
#include <fstream>
#include <cassert>

#include <string.h>
#include "TChain.h"
#include "TFile.h"
#include "TH1.h"
#include "TTree.h"
#include "TKey.h"
#include "Riostream.h"
#include "src/RooUnfoldResponse.h"

bool MERGE_TREES = false;

using namespace std;

TList *FileList;
std::vector<Double_t> FileWeight;
TFile *Target;

void MergeRootfile( TDirectory *target );

void hadd(const char* flist , const char* fout) {
  gSystem->Load("libRooUnfold.so");

  // root hadd.C("unfold_pythia.list","unfold_pythia.root")
  
  Target = TFile::Open( fout , "RECREATE" );
  FileList = new TList();

  std::cout << "Opening file list: " << flist << std::endl;
  ifstream fin(flist);
  assert(fin.is_open());
  std::string tmpname; Double_t weight = 11;
  while( fin >> tmpname >> weight ) {
    FileList->Add( TFile::Open(tmpname.c_str()) );
    FileWeight.push_back(weight);
  }

  // re-calculate weights with respect to the first file
  assert(FileWeight.size()>1);
  const Double_t norm = FileWeight[0];
  for(int i=0 ; i < FileWeight.size(); i++) {
    FileWeight[i] = FileWeight[i]/norm;
    std::cout << gSystem->BaseName(FileList->At(i)->GetName()) << " : " << FileWeight[i] << std::endl;
  }
  
  MergeRootfile( Target , 0 );

  std::cout << "Closing ROOT file..." << std::endl; fflush(stdout);
  Target->Close();

  std::cout << "All done!" << std::endl; fflush(stdout);
  exit(0);
  abort(); // remove exit(0) if gROOT garbage collector goes into infinite loops
}

void MergeRootfile( TDirectory *target , int depth ) {
  
  //  cout << "Target path: " << target->GetPath() << endl;
  TString path( (char*)strstr( target->GetPath(), ":" ) );
  path.Remove( 0, 2 );
  
  TFile *first_source = (TFile*)FileList->First();
  first_source->cd( path );
  TDirectory *current_sourcedir = gDirectory;
  //gain time, do not add the objects in the list in memory
  Bool_t status = TH1::AddDirectoryStatus();
  TH1::AddDirectory(kFALSE);
  
  // loop over all keys in this directory
  TChain *globChain = 0;
  TIter nextkey( current_sourcedir->GetListOfKeys() );
  TKey *key, *oldkey=0;
  while ( (key = (TKey*)nextkey())) {
    
    //keep only the highest cycle number for each key
    if (oldkey && !strcmp(oldkey->GetName(),key->GetName())) continue;
    
    // read object from first source file
    first_source->cd( path );
    TObject *obj = key->ReadObj();
    
    if ( obj->IsA()->InheritsFrom( TH1::Class() ) ) {
      // descendant of TH1 -> merge it
      
      //      cout << "Merging histogram " << obj->GetName() << endl;
      TH1 *h1 = (TH1*)obj;
      
      // loop over all source files and add the content of the
      // correspondant histogram to the one pointed to by "h1"
      TFile *nextsource = (TFile*)FileList->After( first_source );
      int ifile = 1;
      while ( nextsource ) {
	
	// make sure we are at the correct directory level by cd'ing to path
	nextsource->cd( path );
	TKey *key2 = (TKey*)gDirectory->GetListOfKeys()->FindObject(h1->GetName());
	if (key2) {
	  TH1 *h2 = (TH1*)key2->ReadObj();
	  h1->Add( h2 , FileWeight[ifile]);
	  delete h2;
	}
	
	nextsource = (TFile*)FileList->After( nextsource );
	ifile++;
      }
    }
    else if ( obj->IsA()->InheritsFrom( TTree::Class() ) ) {
      const char* obj_name= obj->GetName();
      if(MERGE_TREES) {
	// loop over all source files create a chain of Trees "globChain"
	globChain = new TChain(obj_name);
	globChain->Add(first_source->GetName());
	TFile *nextsource = (TFile*)FileList->After( first_source );
	//      const char* file_name = nextsource->GetName();
	// cout << "file name  " << file_name << endl;
	while ( nextsource ) {
	  globChain->Add(nextsource->GetName());
	  nextsource = (TFile*)FileList->After( nextsource );
	}
      } else {
	std::cout << "INFO: skipping tree " << obj_name << std::endl;
      }
    } else if ( obj->IsA()->InheritsFrom( TDirectory::Class() ) ) {
      // it's a subdirectory

      if(depth==0) cout << "Found directory " << obj->GetName() << endl;
      
      // create a new subdir of same name and title in the target file
      target->cd();
      TDirectory *newdir = target->mkdir( obj->GetName(), obj->GetTitle() );
      
      // newdir is now the starting point of another round of merging
      // newdir still knows its depth within the target file via
      // GetPath(), so we can still figure out where we are in the recursion
      MergeRootfile( newdir , depth+1 );
      
    } else if ( obj->IsA()->InheritsFrom( RooUnfoldResponse::Class() ) ) {
      // descendant of RooUnfoldResponse -> merge it
      
      RooUnfoldResponse *r1 = (RooUnfoldResponse*)obj;
      
      // loop over all source files and add the content of the
      // correspondant histogram to the one pointed to by "r1"
      TFile *nextsource = (TFile*)FileList->After( first_source );
      int ifile = 1;
      while ( nextsource ) {
	
	// make sure we are at the correct directory level by cd'ing to path
	nextsource->cd( path );
	TKey *key2 = (TKey*)gDirectory->GetListOfKeys()->FindObject(key->GetName());
	if (key2) {
	  RooUnfoldResponse *r2 = (RooUnfoldResponse*)key2->ReadObj();
	  r1->Add( *r2 ); // FIXME - needs a custom-compiled version of RooUnfold!!!
	  //r1->Add( *r2 , FileWeight[ifile]);
	  delete r2;
	}
	
	nextsource = (TFile*)FileList->After( nextsource );
	ifile++;
      }
      
    } else {
      
      // object is of no type that we know or can handle
      cout << "Unknown object type, name: "
	   << obj->GetName() << " title: " << obj->GetTitle() << endl;
    }
    
    // now write the merged histogram (which is "in" obj) to the target file
    // note that this will just store obj in the current directory level,
    // which is not persistent until the complete directory itself is stored
    // by "target->Write()" below
    if ( obj ) {
      target->cd();
      
      //!!if the object is a tree, it is stored in globChain...
      // note that globChain may be NULL if tree merging was disabled
      if(obj->IsA()->InheritsFrom( TTree::Class() )) {
	if(globChain) globChain->Merge(target->GetFile(),0,"keep");
      }
      else {
	obj->Write( key->GetName() );
      }
    }
    
  } // while ( ( TKey *key = (TKey*)nextkey() ) )
  
  // save modifications to target file
  target->SaveSelf(kTRUE);
  TH1::AddDirectory(status);
}
