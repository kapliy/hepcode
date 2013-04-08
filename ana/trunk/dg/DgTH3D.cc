
#include <DataGraphics/DgTH3D.hh>
#include <DataGraphics/DgTypes.hh>

#include <cassert>
#include <cmath>
#include <vector>
#include <algorithm>
#include <iterator>
#include <string.h>
#include <TH3D.h>
#include <TDirectory.h>

using namespace std;
using namespace DataGraphics;

TNamed*
DgTH3D::new_ROOT( const char* name ) const
{
  TH3D* h = (TH3D*)_h.Clone(name);
  h->SetName(name);
  h->SetDirectory(0);
  return h;
}

void
DgTH3D::save_as_ROOT( const char* name , TDirectory* dir ) const
{
  dir->cd();
  TNamed* h = new_ROOT(name);
  h->Write( name );
  delete h;
}

void
DgTH3D::save( const char* name , TDirectory* dir ) const
{
  dir->cd();
  TObjectDgTH3D* h = new TObjectDgTH3D( name , "" , *this );
  h->Write( name );
  delete h;
}

void
DgTH3D::load( const char* name , TDirectory* dir )
{
  dir->cd();
  const TObjectDgTH3D* h = dynamic_cast<const TObjectDgTH3D*>( dir->Get(name) );
  assert( h );
  *this = *h;
  delete h;
}

void
DgTH3D::merge( const DgAbsGraphic* other )
{
  const DgTH3D* hother = dynamic_cast<const DgTH3D*>(other);  assert(hother);
  assert( nbins() == hother->nbins() );
  _h.Add(hother->ptrTH3D());
}

int DgTH3D::nTH3D = 0;

#include <TObject.h>
ClassImp(TObjectDgTH3D)
