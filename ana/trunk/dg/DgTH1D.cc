
#include <DataGraphics/DgTH1D.hh>
#include <DataGraphics/DgTypes.hh>

#include <cassert>
#include <cmath>
#include <vector>
#include <algorithm>
#include <iterator>
#include <string.h>
#include <TH1D.h>
#include <TDirectory.h>

using namespace std;
using namespace DataGraphics;

TNamed*
DgTH1D::new_ROOT( const char* name ) const
{
  TH1D* h = (TH1D*)(_h.Clone(name));
  h->SetName(name);
  h->SetDirectory(0);
  return h;
}

void
DgTH1D::save_as_ROOT( const char* name , TDirectory* dir ) const
{
  dir->cd();
  TNamed* h = new_ROOT(name);
  h->Write( name );
  delete h;
}

void
DgTH1D::save( const char* name , TDirectory* dir ) const
{
  dir->cd();
  TObjectDgTH1D* h = new TObjectDgTH1D( name , "" , *this );
  h->Write( name );
  delete h;
}

void
DgTH1D::load( const char* name , TDirectory* dir )
{
  dir->cd();
  const TObjectDgTH1D* h = dynamic_cast<const TObjectDgTH1D*>( dir->Get(name) );
  assert( h );
  *this = *h;
  delete h;
}

void
DgTH1D::merge( const DgAbsGraphic* other )
{
  const DgTH1D* hother = dynamic_cast<const DgTH1D*>(other);  assert(hother);
  assert( nbins() == hother->nbins() );
  _h.Add(hother->ptrTH1D());
}

int DgTH1D::nTH1D = 0;

#include <TObject.h>
ClassImp(TObjectDgTH1D)
