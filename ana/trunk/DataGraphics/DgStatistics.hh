#ifndef WRAP_DGDATAGRAPHICS_STATISTICS_HH
#define WRAP_DGDATAGRAPHICS_STATISTICS_HH

#include <cmath>
#include <DataGraphics/DgTypes.hh>
#include <boost/optional.hpp>
#include <boost/scoped_ptr.hpp>
#include <TObject.h>

namespace
DataGraphics
{

  // class DgStatMean
  // {
  // private:
  //   Value _x;
  //   Value _w;
  // public:
  //   DgStatMean() : _x(0) , _n(0) {}
  //   void update( const Value& x ) { _x += x; ++_w; }
  //   void update( const Value& x , const Value& w ) { _x += w*x; _w+=w; }
  //   const Value mean() const { return _x/_w; }
  //   ClassDef(DgStatMean,1);
  // };

  // class DgStatRMS
  // {
  // private:
  //   Value _xsq;
  //   Value _w;
  // public:
  //   DgStatRMS() : _xsq(0) , _n(0) {}
  //   void update( const Value& x ) { _xsq += (x*x); ++_w; }
  //   void update( const Value& x , const Value& w ) { _xsq += (x*x*w); _w+=w; }
  //   const Value rms2() const { return _xsq/_w; }
  //   const Value rms() const { return std::sqrt(rms2()); }
  //   ClassDef(DgStatRMS,1);
  // };

  // class DgStatVariance
  // {
  // private:
  //   boost::scoped_ptr<DgStatMean> _internal_mean;
  //   boost::scoped_ptr<DgStatRMS> _internal_rms;
  //   const DgStatMean* _mean;
  //   const DgStatRMS* _rms;
  // public:
  //   DgStatVariance() : _internal_mean(new DgStatMean) , _internal_rms(new DgStatRMS) {
  //     _mean = _internal_mean.get();
  //     _rms = _internal_rms.get();
  //   }
  //   DgStatVariance(const DgStatMean* mean,const DgStatRMS* rms) : _internal_mean() , _internal_rms() , _mean(mean) , _rms(rms) {}
  //   void update( const Value& x ) {
  //     if( !_internal_mean ) { return; }
  //     _internal_mean->update( x );
  //     _internal_rms->update( x );
  //   }
  //   void update( const Value& x , const Value& w ) {
  //     if( !_internal_mean ) { return; }
  //     _internal_mean->update( x , w );
  //     _internal_rms->update( x , w );
  //   }
  //   const Value variance() const { 
  //     const Value mu = _mean->mean();
  //     return( _rms->rms2() + (mu*mu) );
  //   }
  //   const Value sqrt_variance() const { 
  //     return( std::sqrt( variance() ) );
  //   }
  //   ClassDef(DgStatVariance,1);
  // };

  class DgStatPearsonCoeff
  {
  private:
    Value _w;
    Value _x;
    Value _y;
    Value _x2;
    Value _y2;
    Value _xy;
  public:
    DgStatPearsonCoeff() 
      : _w(0) , _x(0) , _y(0) , _x2(0) , _y2(0) , _xy(0)
    {}
    void update( const Value& x , const Value& y ) {
      ++_w;
      _x += x;
      _y += y;
      _x2 += x*x;
      _y2 += y*y;
      _xy += x*y;
    }
    void update( const Value& x , const Value& y , const Value& w ) {
      ++_w;
      _x += x;
      _y += y;
      _x2 += x*x;
      _y2 += y*y;
      _xy += x*y;
    }
    const Value r() const {
      const Value den_x = std::sqrt( _w * _x2 - (_x*_x) );
      const Value den_y = std::sqrt( _w * _y2 - (_y*_y) );
      const Value num = (_w * _xy) - (_x*_y);
      if( den_x>0. && den_y>0. ) {
        return( num / (den_x*den_y) );
      }
      return -1.;
    }
    ClassDef(DgStatPearsonCoeff,1);
  };

};

#endif // WRAP_DGDATAGRAPHICS_STATISTICS_HH
