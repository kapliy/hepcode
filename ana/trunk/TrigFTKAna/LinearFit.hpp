#ifndef WRAP_LINEARFIT_HH
#define WRAP_LINEARFIT_HH

#include <vector>
#include <list>
#include <utility>
#include <algorithm>
#include <cmath>
#include <boost/optional.hpp>
#include <numeric>
#include "TrigFTKAna/Detector.hpp"

// compute linear fit for a set of inputs. expects an STL collection
// of "std::pair<ftype,ftype>"'s where pair.first = x and pair.second
// = y. the result is then a slope (y/x) and x-intercept.

template<typename ftype>
class
LinearFit
{

private:

  bool _valid_fit;
  ftype _slope;
  ftype _intercept;
  ftype _slope_err;
  ftype _intercept_err;

  bool _compute_details;
  std::vector<ftype> _slope_unbiased;
  std::vector<ftype> _intercept_unbiased;
  std::vector<ftype> _residual_unbiased;
  std::vector<ftype> _chi2_term;

public:

  template<typename collT>
  const bool fit( typename collT::const_iterator begin , typename collT::const_iterator end ) {
    _valid_fit = false;
    if( begin==end ) { return false; }
    if( std::distance(begin,end)<2 ) { return false; }
    unsigned int n = 0u;
    ftype sw = 0;
    ftype swx = 0;
    ftype swy = 0;
    ftype swxx = 0;
    ftype swxy = 0;
    for( typename collT::const_iterator i=begin, f=end; i!=f; ++i,++n ) {
      const std::pair<ftype,ftype>& doublet( *i );
      const ftype& x( doublet.first );
      const ftype& y( doublet.second );
      ++sw;
      swx += x;
      swy += y;
      swxx += (x*x);
      swxy += (x*y);
    }
    ftype delta = (sw*swxx) - (swx*swx);
    if( std::abs(delta)<1E-10 || std::abs(sw)<1E-10 ) { return false; }
    _slope = - ((sw*swxy) - (swx*swy));
    _slope /= delta;
    _slope_err = sw/delta; if(_slope_err>0.) { _slope_err = std::sqrt(_slope_err); }
    _intercept = (swxx*swy) - (swx*swxy);
    _intercept /= delta;
    _intercept_err = swxx / delta; if(_intercept_err>0.) { _intercept_err = std::sqrt(_intercept_err); }
    _valid_fit = true;
    if( _compute_details ) {
      // compute n-1 fits and unbiased residuals
      _slope_unbiased.clear();
      _intercept_unbiased.clear();
      _residual_unbiased.clear();
      _slope_unbiased.reserve(n);
      _intercept_unbiased.reserve(n);
      _residual_unbiased.reserve(n);
      std::list< std::pair<ftype,ftype> > rotation( begin , end );
      // std::list< std::pair<ftype,ftype> > rotation;
      // std::copy( rotation.begin() , rotation.end() , inserter(rotation,rotation.begin()) );
      LinearFit subfitter; subfitter.compute_details(false);
      typename collT::const_iterator iter = begin;
      for( unsigned int i=0; i!=n; ++i,++iter ) {
        if( i!=0 ) { std::rotate( rotation.begin() , ++rotation.begin() , rotation.end() ); }
        const std::pair<ftype,ftype>& doublet( *iter );
        const bool ok_sub = subfitter.fit< std::list< std::pair<ftype,ftype> > >( ++rotation.begin() , rotation.end() );
        if( ok_sub ) { 
          const ftype this_slope = subfitter.slope();
          const ftype this_intercept = subfitter.intercept();
          _slope_unbiased.push_back( this_slope );
          _intercept_unbiased.push_back( this_intercept );
          _residual_unbiased.push_back( doublet.second - ( (this_slope*doublet.first) + this_intercept ) );
        } else { // these things are undefined with two or fewer input points.
          _slope_unbiased.push_back( 99999. );
          _intercept_unbiased.push_back( -99999. );
          _residual_unbiased.push_back( 0. );
        }
      }
      // compute chi^2 contribution for each term (input point)
      _chi2_term.clear();
      _chi2_term.reserve(n);
      for( typename collT::const_iterator i=begin, f=end; i!=f; ++i ) {
        const std::pair<ftype,ftype>& doublet( *i );
        _chi2_term.push_back( detector::pow<2>(doublet.second - ( (_slope*doublet.first) + _intercept )) ); // uniform unit weight as above.
      }
    }
    return _valid_fit;
  }
  const ftype& slope() const { assert(_valid_fit); return _slope; }
  const ftype& slope_err() const { assert(_valid_fit); return _slope_err; }
  const ftype& intercept() const { assert(_valid_fit); return _intercept; }
  const ftype& intercept_err() const { assert(_valid_fit); return _intercept_err; }
  const ftype chi2() const { assert(_valid_fit&&_compute_details); return std::accumulate( _chi2_term.begin() , _chi2_term.end() , static_cast<ftype>(0) ); }
  const ftype unbiased_slope( const unsigned int& ipoint ) const { assert(_valid_fit&&_compute_details); assert(ipoint<_slope_unbiased.size()); return _slope_unbiased[ipoint]; }
  const ftype unbiased_intercept( const unsigned int& ipoint ) const { assert(_valid_fit&&_compute_details); assert(ipoint<_intercept_unbiased.size()); return _intercept_unbiased[ipoint]; }
  const ftype unbiased_residual( const unsigned int& ipoint ) const { assert(_valid_fit&&_compute_details); assert(ipoint<_residual_unbiased.size()); return _residual_unbiased[ipoint]; }
  const ftype& chi2( const unsigned int& iterm ) const { assert(_valid_fit&&_compute_details); assert(iterm<_chi2_term.size()); return _chi2_term[iterm]; }

  LinearFit() : _valid_fit(false) , _compute_details(false) {}
  virtual ~LinearFit() {}

  void compute_details( const bool& yes ) { _compute_details = yes; _valid_fit = false; }
  const bool compute_details() const { return _compute_details; }
  
};

#endif // WRAP_LINEARFIT_HH
