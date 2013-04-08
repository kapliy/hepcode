#ifndef WRAP_LIKELIHOOD1D_HPP
#define WRAP_LIKELIHOOD1D_HPP

#include <vector>
#include <algorithm>
#include <numeric>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/version.hpp>

class TH1F;

class
Likelihood1D
{
public:
  typedef std::vector<unsigned int>::const_iterator const_iterator;
protected:
  // binning scheme allows for 1...n variable width bins. 
  // extra bins: bin 0 is an underflow bin and bin n+1 is an overflow bin
  // x<_bin_boundaries[0] is underflow and x>=_bin_boundaries[n] is the overflow
  typedef std::vector<unsigned int> BinnedPDFType;
  BinnedPDFType             _binned_pdf;
  std::vector<float>        _bin_boundaries;
  mutable bool              _stale_integral;
  mutable float             _integral;
protected:
  inline const unsigned int find_bin( const float& x ) const {
    unsigned int result = 0u;
    for( std::vector<float>::const_iterator i=_bin_boundaries.begin(), f=_bin_boundaries.end(); i!=f; ++i,++result ) {
      if( x<(*i) ) { return result; }
    }
    return( _binned_pdf.size()-1 );
  }
protected:
  friend class boost::serialization::access;
  template<typename archiveT>
  void serialize( archiveT& ar , const unsigned int version ) {
    ar & _binned_pdf;
    ar & _bin_boundaries;
    ar & _stale_integral;
    ar & _integral;
  }
public:
  Likelihood1D() : _stale_integral(false) , _integral(0.) {}
  virtual ~Likelihood1D() {}
  // define the binning
  void uniform_binning( const unsigned int& nbins , const float& xmin , const float& xmax );
  void trifold_uniform_binning( const unsigned int& nbinsA , const unsigned int& nbinsB , const unsigned int& nbinsC , 
                                const float& xminA , const float& xmaxA , const float& xmaxB , const float& xmaxC );
  // populate the likelihood 
  void fill( const float& x );
  // evaluate the likelihood for a specific input
  const float lik( const float& x ) const;
  // iterator over the binned likelihood function
  const_iterator begin() const { return _binned_pdf.begin(); }
  const_iterator end() const { return _binned_pdf.end(); }
  const float bin_frac( const_iterator i ) const { 
    return( (*i) / static_cast<float>(std::accumulate(_binned_pdf.begin(),_binned_pdf.end(),0ul)) );
  }
  const unsigned int bin_value( const_iterator i ) const { return( *i ); }
  const float xmin() const { return _bin_boundaries.front(); }
  const float xmax() const { return _bin_boundaries.back(); }
  const unsigned int nbins() const { return std::max(_binned_pdf.size()-2,static_cast<BinnedPDFType::size_type>(0)); }

  // save/load the likelihood from a text file
  const bool load( const char* filename );
  const bool save( const char* filename ) const;
  // convert to a TH1 histogram
  TH1F* th1() const;

  // merge
  Likelihood1D& operator+=( const Likelihood1D& rhs ) {
    assert( _binned_pdf.size() == rhs._binned_pdf.size() );
    assert( _bin_boundaries.size() == rhs._bin_boundaries.size() );
    assert( std::equal( _bin_boundaries.begin() , _bin_boundaries.end() , rhs._bin_boundaries.begin() ) );
    _stale_integral = true;
    std::vector<unsigned int>::const_iterator j=rhs._binned_pdf.begin();
    for( std::vector<unsigned int>::iterator i=_binned_pdf.begin(), f=_binned_pdf.end(); i!=f; ++i, ++j ) {
      (*i) += (*j);
    }
  }
};

BOOST_CLASS_VERSION( Likelihood1D , 1 );

#endif // WRAP_LIKELIHOOD1D_HPP
