
#include <DataGraphics/DgGraph2D.hh>
#include <DataGraphics/DgTypes.hh>

#include <cassert>
#include <cmath>
#include <vector>
#include <algorithm>
#include <iterator>
#include <string.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TDirectory.h>

using namespace std;
using namespace DataGraphics;

void
DgGraph2D::fill( const Value& x , const Value& y )
{  
  _data.push_back( Tuple(x,y) );
}

void
DgGraph2D::fillw( const Value& x , const Value& y , const WeightedCount& w )
{  
  _data.push_back( Tuple(x,y,w) );
}

TNamed*
DgGraph2D::new_ROOT( const char* name ) const
{
  Int_t n = _data.size();
  Double_t* x = new Double_t[n];
  Double_t* y = new Double_t[n];
  Int_t i = 0;
  bool do_weighted = false;
  for( vector<Tuple>::const_iterator j=_data.begin() , f=_data.end(); j!=f && i<n; ++j,++i ) {
    if( j->_w != 1. ) { do_weighted = true; break; }
  }
  Double_t* w = 0;
  if( do_weighted ) { 
    w = new Double_t[n];
  }
  for( vector<Tuple>::const_iterator j=_data.begin() , f=_data.end(); j!=f && i<n; ++j,++i ) {
    x[i] = j->_x;
    y[i] = j->_y;
    if( w ) { w[i] = j->_w; }
  }
  TGraph* graph = 0;
  if( do_weighted ) { 
    graph = new TGraphErrors( i , x , y , w , w );
  } else { 
    graph = new TGraph( i , x , y ); 
  }
  if( w ) { delete w; }
  delete y;
  delete x;
  return graph;
}

void
DgGraph2D::save_as_ROOT( const char* name , TDirectory* dir ) const
{
  dir->cd();
  TNamed* graph = new_ROOT( name );
  graph->Write( name );
  delete graph;
}

void
DgGraph2D::save( const char* name , TDirectory* dir ) const
{
  dir->cd();
  TObjectDgGraph2D* h = new TObjectDgGraph2D( name , "" , *this );
  h->Write( name );
  delete h;
}

void
DgGraph2D::load( const char* name , TDirectory* dir )
{
  dir->cd();
  const TObjectDgGraph2D* h = dynamic_cast<const TObjectDgGraph2D*>(dir->Get(name));
  assert( h );
  *this = *h;
  delete h;
}

void
DgGraph2D::merge( const DgAbsGraphic* other )
{
  const DgGraph2D* hother = dynamic_cast<const DgGraph2D*>(other);  assert(hother);
  copy( hother->_data.begin() , hother->_data.end() , back_inserter(_data) );
}

#include <TObject.h>
ClassImp(TObjectDgGraph2D)
