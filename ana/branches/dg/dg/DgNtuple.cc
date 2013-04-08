
#include <DataGraphics/DgNtuple.hh>
#include <DataGraphics/DgTypes.hh>

#include <cassert>
#include <cmath>
#include <vector>
#include <algorithm>
#include <iterator>
#include <string.h>
#include <TNtuple.h>
#include <TDirectory.h>

using namespace std;
using namespace DataGraphics;

// simply save the array of floats into ntuple
void
DgNtuple::fillnt( const std::vector<float>& x )
{  
  assert(x.size() == _names.size());
  _data.push_back( Tuple(x) );
}

// event weight takes the first position in the ntuple
void
DgNtuple::fillntw( const std::vector<float>& x, const std::vector<float>& w )
{
  assert(x.size() + w.size() == _names.size() );
  std::vector<float> res;
  res.insert(res.end(),w.begin(), w.end());
  res.insert(res.end(),x.begin(), x.end());
  _data.push_back( Tuple(res) );
}

TNamed*
DgNtuple::new_ROOT( const char* name ) const
{
  const std::string varlist = get_varlist();
  const char *varlist_str = varlist.c_str();
  TNtuple *nt = new TNtuple(name, "" , varlist_str );
  unsigned int n = 1;
  if (_data.size()>0) n = _data[0]._vals.size();
  Float_t* x = new Float_t[n];
  unsigned int itot = 0;
  for( vector<Tuple>::const_iterator j=_data.begin(); j!=_data.end(); j++ ) {
    assert( j->_vals.size() == _names.size() );
    for(unsigned int ivar=0; ivar < j->_vals.size(); ivar++) {
      x[ivar] = j->_vals.at(ivar);
    }
    nt->Fill(x);
    itot++;
    if(itot%1000==0) nt->FlushBaskets();
  }
  delete[] x;
  return nt;
}

void
DgNtuple::save_as_ROOT( const char* name , TDirectory* dir ) const
{
  dir->cd();
  TNamed* nt = new_ROOT( name );
  nt->Write( name );
  // TTree/TNtuple is owned by the file it is written to,
  // so there is no need to delete it manually
  //  delete nt;
}

void
DgNtuple::save( const char* name , TDirectory* dir ) const
{
  dir->cd();
  TObjectDgNtuple* h = new TObjectDgNtuple( name , "" , *this );
  h->Write( name );
  delete h;
}

void
DgNtuple::load( const char* name , TDirectory* dir )
{
  dir->cd();
  const TObjectDgNtuple* h = dynamic_cast<const TObjectDgNtuple*>(dir->Get(name));
  assert( h );
  *this = *h;
  delete h;
}

void
DgNtuple::merge( const DgAbsGraphic* other )
{
  const DgNtuple* hother = dynamic_cast<const DgNtuple*>(other);  assert(hother);
  copy( hother->_data.begin() , hother->_data.end() , back_inserter(_data) );
}

#include <TObject.h>
ClassImp(TObjectDgNtuple)
