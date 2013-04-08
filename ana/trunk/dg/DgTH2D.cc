
#include <DataGraphics/DgTH2D.hh>
#include <DataGraphics/DgTypes.hh>

#include <cassert>
#include <cmath>
#include <vector>
#include <algorithm>
#include <iterator>
#include <string.h>
#include <TH2D.h>
#include <TDirectory.h>

using namespace std;
using namespace DataGraphics;

TNamed*
DgTH2D::new_ROOT( const char* name ) const
{
  TH2D* h = (TH2D*)_h.Clone(name);
  h->SetName(name);
  h->SetDirectory(0);
  return h;
}

void
DgTH2D::save_as_ROOT( const char* name , TDirectory* dir ) const
{
  dir->cd();
  TNamed* h = new_ROOT(name);
  h->Write( name );
  delete h;
}

void
DgTH2D::save( const char* name , TDirectory* dir ) const
{
  dir->cd();
  TObjectDgTH2D* h = new TObjectDgTH2D( name , "" , *this );
  h->Write( name );
  delete h;
}

void
DgTH2D::load( const char* name , TDirectory* dir )
{
  dir->cd();
  const TObjectDgTH2D* h = dynamic_cast<const TObjectDgTH2D*>( dir->Get(name) );
  assert( h );
  *this = *h;
  delete h;
}

void
DgTH2D::merge( const DgAbsGraphic* other )
{
  const DgTH2D* hother = dynamic_cast<const DgTH2D*>(other);  assert(hother);
  assert( nbins() == hother->nbins() );
  _h.Add(hother->ptrTH2D());
}

int DgTH2D::nTH2D = 0;

#include <TObject.h>
ClassImp(TObjectDgTH2D)
