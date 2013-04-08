#ifndef WRAP_LIKELIHOODPARAMETERIZATION1D_HPP
#define WRAP_LIKELIHOODPARAMETERIZATION1D_HPP

#include <string>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/serialization/version.hpp>
#include <RooRealVar.h>
#include <RooGaussian.h>
#include <RooCBShape.h>
#include <RooAbsPdf.h>
#include <iostream>
#include "TrigFTKAna/Likelihood1D.hpp"

class TH1;

class
LikelihoodParameterization1D
{
private:
  // template<typename T>
  // class normal_ptr
  // {
  // private:
  //   T* _t;
  // public:
  //   normal_ptr() : _t(0) {}
  //   normal_ptr( T* t ) : _t(t) {}
  //   T* operator->() const { return _t; }
  //   T* get() const { return _t; }
  //   T& operator*() const { return *_t; }
  //   operator bool () const { return (bool)_t; }
  //   void reset() { _t = 0; }
  //   void reset( T* t ) { _t=t; }
  // };
public:
  typedef enum { UNKNOWN=0, LONG_LIVED=1 , PROMPT=2 } Type;
protected:
  Type _type;
  boost::shared_ptr<RooRealVar> _x;
  boost::shared_ptr<RooRealVar> _mean;
  boost::shared_ptr<RooRealVar> _mean3;
  boost::shared_ptr<RooRealVar> _cb_sigma;
  boost::shared_ptr<RooRealVar> _cb_alpha;
  boost::shared_ptr<RooRealVar> _cb_order;
  boost::shared_ptr<RooRealVar> _gaus1_sigma;
  boost::shared_ptr<RooRealVar> _gaus2_sigma;
  boost::shared_ptr<RooRealVar> _gaus3_sigma;
  boost::shared_ptr<RooGaussian> _gaus1;
  boost::shared_ptr<RooGaussian> _gaus2;
  boost::shared_ptr<RooGaussian> _gaus3;
  boost::shared_ptr<RooCBShape> _cb;
  boost::shared_ptr<RooRealVar> _frac2;
  boost::shared_ptr<RooRealVar> _frac3;
  boost::shared_ptr<RooAbsPdf> _pdf;
  double _xmax;
  double _xmin;
  double _integral;
protected:
  void resetRoos( const TH1* h = 0 );
  void claimRooOwnership();
protected:
  friend class boost::serialization::access;
  template<typename archiveT>
  void serialize( archiveT& ar , const unsigned int version ) {
    ar & _type;
    if( _type == LONG_LIVED ) { 
      double mean, cb_sigma, cb_alpha, cb_order, 
        gaus2_sigma, gaus3_sigma, frac2, frac3;
      mean = (_mean ? _mean->getVal() : 0.);
      cb_sigma = (_cb_sigma ? _cb_sigma->getVal() : 0);
      cb_alpha = (_cb_alpha ? _cb_alpha->getVal() : 0);
      cb_order = (_cb_order ? _cb_order->getVal() : 0);
      gaus2_sigma = (_gaus2_sigma ? _gaus2_sigma->getVal() : 0);
      gaus3_sigma = (_gaus3_sigma ? _gaus3_sigma->getVal() : 0);
      frac2 = (_frac2 ? _frac2->getVal() : 0);
      frac3 = (_frac3 ? _frac3->getVal() : 0);
      double integral = _integral;
      double xmax = _xmax;
      double xmin = _xmin;
      ar & mean;
      ar & cb_sigma;
      ar & cb_alpha;
      ar & cb_order;
      ar & gaus2_sigma;
      ar & gaus3_sigma;
      ar & frac2;
      ar & frac3;
      ar & integral;
      ar & xmax;
      ar & xmin;
      resetRoos();
      _mean->setVal(mean);
      _cb_sigma->setVal(cb_sigma);
      _cb_alpha->setVal(cb_alpha);
      _cb_order->setVal(cb_order);
      _gaus2_sigma->setVal(gaus2_sigma);
      _gaus3_sigma->setVal(gaus3_sigma);
      _frac2->setVal(frac2);
      _frac3->setVal(frac3);
      _integral = integral;
      _xmax = xmax;
      _xmin = xmin;
    } else if( _type == PROMPT ) {
      double mean = (_mean ? _mean->getVal() : 0);
      double mean3 = (_mean3 ? _mean3->getVal() : 0);
      double gaus1_sigma = (_gaus1_sigma ? _gaus1_sigma->getVal() : 0);
      double gaus2_sigma = (_gaus2_sigma ? _gaus2_sigma->getVal() : 0);
      double gaus3_sigma = (_gaus3_sigma ? _gaus3_sigma->getVal() : 0);
      double frac2 = (_frac2 ? _frac2->getVal() : 0);
      double frac3 = (_frac3 ? _frac3->getVal() : 0);
      double integral = _integral;
      double xmax = _xmax;
      double xmin = _xmin;
      ar & mean;
      ar & mean3;
      ar & gaus1_sigma;
      ar & gaus2_sigma;
      ar & gaus3_sigma;
      ar & frac2;
      ar & frac3;
      ar & integral;
      ar & xmax;
      ar & xmin;
      resetRoos();
      _mean->setVal( mean );
      _mean3->setVal( mean );
      _gaus1_sigma->setVal( gaus1_sigma );
      _gaus2_sigma->setVal( gaus2_sigma );
      _gaus3_sigma->setVal( gaus3_sigma );
      _frac2->setVal( frac2 );
      _frac3->setVal( frac3 );
      _integral = integral;
      _xmax = xmax;
      _xmin = xmin;
    }
  }
public:
  LikelihoodParameterization1D() : _type(UNKNOWN) {}
  virtual ~LikelihoodParameterization1D() {}

  // populate the likelihood 
  void fit( const Type liktype , const TH1* hist , const std::string fitname = std::string() );
  // evaluate the likelihood for a specific input
  const float lik( const float& x ) const;

  void print( std::ostream& os = std::cout ) const;
};

BOOST_CLASS_VERSION( LikelihoodParameterization1D , 1 );

#endif // WRAP_LIKELIHOODPARAMETERIZATION1D_HPP
