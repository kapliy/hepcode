
#include <vector>
#include <algorithm>
#include <numeric>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <TH1F.h>
#include "TrigFTKAna/Likelihood1D.hpp"


using namespace std;

void
Likelihood1D::uniform_binning( const unsigned int& nbins , const float& xmin , const float& xmax )
{
  assert( xmax>xmin );
  _binned_pdf.clear();
  _bin_boundaries.clear();
  _binned_pdf.reserve( nbins+2 );
  _bin_boundaries.reserve( nbins+1 );
  for( unsigned int ibin=0; ibin!=(nbins+2); ++ibin ) {
    _binned_pdf.push_back( 0 );
    if( ibin<=nbins ) {
      _bin_boundaries.push_back( xmin + ((xmax-xmin)/nbins)*ibin );
    }
  }
  assert( _bin_boundaries.size() == _binned_pdf.size()-1 );
}

void
Likelihood1D::trifold_uniform_binning( const unsigned int& nbinsA , const unsigned int& nbinsB , const unsigned int& nbinsC , 
                                       const float& xminA , const float& xmaxA , const float& xmaxB , const float& xmaxC )
{
  assert( xmaxA>xminA );
  assert( xmaxB>xmaxA );
  assert( xmaxC>xmaxB );
  _binned_pdf.clear();
  _bin_boundaries.clear();
  const unsigned int nbinstot = nbinsA+nbinsB+nbinsC;
  _binned_pdf.reserve( nbinstot+2 );
  _bin_boundaries.reserve( nbinstot+1 );
  for( unsigned int ibin=0; ibin!=(nbinstot+2); ++ibin ) {
    _binned_pdf.push_back( 0 );
    if( ibin<=nbinsA ) {
      _bin_boundaries.push_back( xminA + ((xmaxA-xminA)/nbinsA)*ibin );
    } else if( (ibin-nbinsA)<=nbinsB ) {
      _bin_boundaries.push_back( xmaxA + ((xmaxB-xmaxA)/nbinsB)*(ibin-nbinsA) );
    } else if( (ibin-nbinsA-nbinsB)<=nbinsC ) {
      _bin_boundaries.push_back( xmaxB + ((xmaxC-xmaxB)/nbinsC)*(ibin-nbinsB-nbinsA) );
    }
  }
  // xminA xmaxA nbinsA xmaxB nbinsB xmaxC nbinsC 
  // 0     10    1      20    2      30    1
  // content: 0 0 0 0 0 0
  // bounds:  0 10 15 20 30
  // ibin=2: 15 (10 + (20-10)/2 *(2-1)
  // ibin=4: 30 (20 + (30-20)/1 * (4-2-1)
  assert( _bin_boundaries.size() == _binned_pdf.size()-1 );
}


void
Likelihood1D::fill( const float& x )
{
  const unsigned int ibin = find_bin(x);
  assert( ibin<_binned_pdf.size() );
  ++(_binned_pdf[ibin]);
  _stale_integral = true;
}

// get the likelihood
const float
Likelihood1D::lik( const float& x ) const
{
  if( _stale_integral ) {
    _integral = std::accumulate( _binned_pdf.begin() , _binned_pdf.end() , 0. );
    if( !_binned_pdf.empty() ) { _integral -= *(_binned_pdf.begin()); } // subtract underflow bin, as does Monica
    _stale_integral = false;
  }
  const unsigned int ibin = find_bin(x);
  assert( ibin<_binned_pdf.size() );
  const float num = _binned_pdf[ibin];
  const float r = num/_integral;
  return( r );
}

const bool
Likelihood1D::load( const char* filename )
{
  if( std::accumulate( _binned_pdf.begin() , _binned_pdf.end() , 0u )>0u ) { 
    cerr << __FILE__ << " " << __LINE__ << " likelihood function already present during load." << endl;
    return false;
  }
  ifstream inf( filename , ios::in );
  if( !inf ) { 
    cerr << __FILE__ << " " << __LINE__ << " could not open " << filename << " to load likelihood." << endl;
    return false;
  }
  unsigned int liksize = 0u;
  inf >> liksize;
  _bin_boundaries.clear();
  _binned_pdf.clear();
  _bin_boundaries.reserve( liksize-1 );
  _binned_pdf.reserve( liksize );
  for( unsigned int i=0, f=liksize-1; i!=f; ++i ) {
    if( inf.eof() ) { 
      cerr << __FILE__ << " " << __LINE__ << " premature end of file while reading likelihood" << endl;
      _bin_boundaries.clear();
      _binned_pdf.clear();
      return false;
    }
    float tmp;
    inf >> tmp;
    _bin_boundaries.push_back( tmp );
  }
  for( unsigned int i=0, f=liksize; i!=f; ++i ) {
    if( inf.eof() ) { 
      cerr << __FILE__ << " " << __LINE__ << " premature end of file while reading likelihood" << endl;
      _bin_boundaries.clear();
      _binned_pdf.clear();
      return false;
    }
    unsigned int tmp2;
    inf >> tmp2;
    _binned_pdf.push_back( tmp2 );
  }
  inf.close();  
  _stale_integral = true;
  if( std::accumulate( _binned_pdf.begin() , _binned_pdf.end() , 0u ) == 0u ) {
    cout << " likelihood pdf " << filename << " loaded with zero entries " << endl;
  }
  if( false ) {
    cout << " likelihood " << filename << " loaded: " << endl;
    cout << " bin boundaries: " << endl;
    for( vector<float>::const_iterator i=_bin_boundaries.begin(), f=_bin_boundaries.end(); i!=f; ++i ) {
      cout << setw(5) << (*i) << " ";
    }
    cout << " bin contents: " << endl;
    for( vector<unsigned int>::const_iterator i=_binned_pdf.begin(), f=_binned_pdf.end(); i!=f; ++i ) {
      cout << setw(5) << (*i) << " ";
    }
    cout << endl;
  }
  return true;
}

const bool
Likelihood1D::save( const char* filename ) const
{
  if( _binned_pdf.empty() ) { 
    cerr << __FILE__ << " " << __LINE__ << " no likelihood function to save." << endl;
    return false;
  }
  ofstream outf( filename , ios::out );
  if( !outf ) { 
    cerr << __FILE__ << " " << __LINE__ << " could not open " << filename << " to save likelihood." << endl;
    return false;
  }
  assert( _bin_boundaries.size() == _binned_pdf.size()-1 );
  outf << _binned_pdf.size() << endl;
  for( vector<float>::const_iterator i=_bin_boundaries.begin(), f=_bin_boundaries.end(); i!=f; ++i ) {
    outf << (*i) << endl;
  }
  for( vector<unsigned int>::const_iterator i=_binned_pdf.begin(), f=_binned_pdf.end(); i!=f; ++i ) {
    outf << (*i) << endl;
  }
  outf.close();
  return true;
}

TH1F*
Likelihood1D::th1() const
{
  // create histogram
  Int_t nbins = _binned_pdf.size()-2;
  Float_t* bin_low_edge = new Float_t[nbins+1];
  unsigned int ibin = 0u;
  assert( _bin_boundaries.size() == (unsigned int)(nbins+1) );
  for( vector<float>::const_iterator i=_bin_boundaries.begin(), f=_bin_boundaries.end(); i!=f; ++i,++ibin ) {
//     cout << " bin low edge: bin=" << ibin << " x=" << (*i) << endl;
    bin_low_edge[ibin] = (*i);
  }
  TH1F* result = new TH1F( "likelihood1d" , "" , nbins , bin_low_edge );
  assert( result );
  ibin = 0u;
  for( vector<unsigned int>::const_iterator j=_binned_pdf.begin(), jf=_binned_pdf.end(); j!=jf; ++j,++ibin ) {
    result->SetBinContent( ibin , *j );
  }
  delete bin_low_edge;
  return result;
}
