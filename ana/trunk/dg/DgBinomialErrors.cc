
// ================================================================
// 
//   DgBinomialErrors.cc
//
//        Code to compute binomial/asymmetric errors on efficiency.  
//        obtained from Adam Scott, UCSB, with pieces based on
//        techniques in CDF5894
//
//        Adam Scott, UC Santa Barbara
//        Antonio Boveia, UC Santa Barbara
//
// ================================================================

#include <cassert>
#include <iostream>
#include <cmath>
#include <limits>
#include <TMath.h>
#include <TF1.h>
#include <DataGraphics/DgBinomialErrors.hh>
#include <boost/scoped_ptr.hpp>

using namespace std;
using namespace DataGraphics;

// the error on numer/denom is calculated using the binomial formula
void
DataGraphics::FindEfficiencyWithSymmetricError(const Count& numer, const Count& denom, double& efficiency, double& err)
{
  assert( denom>0. );
  assert( numer>=0. );
  efficiency = ((double)numer)/((double)denom);
  err = efficiency*(1.-efficiency)/denom;
  err = (err>=0.) ? sqrt(err) : 0.;
}

// gets asymmetric errors on an efficiency.  uses the technique
// outlined in cdf note #5894

void
DataGraphics::FindEfficiencyWithAsymmetricError(const Count& numerC, const Count& denomC, 
                                                double& efficiency, double& errMin, double& errMax, bool spew)
{
  Count numer = numerC;
  Count denom = denomC;
  if( numer<0 || denom<=0. ) {
    // efficiency of two weighted numbers makes no sense, especially
    // when one of them is negative. only integers make sense. this
    // doesn't check for integers, but it does check the sign.
    efficiency=0.;
    errMin=0.;
    errMax=0.;
    return;
  }
  
  // the easy part:
  efficiency = ((double)numer)/((double)denom);

  // FIXME: hacked for weights
  numer = std::abs(numer);
  denom = std::abs(denom);
  FindEfficiencyWithSymmetricError(numer,denom,efficiency,errMin);
  errMax=errMin;
  return;

  // the hard part:
  double intervalMin, intervalMax;
  double probability = 0.6827;
  double precision   = 0.00001;
  double normfactor = (denom+1)*TMath::Gamma((double) denom+1)/(TMath::Gamma((double) numer+1)*TMath::Gamma((double) (denom-numer+1)));
  // attention: I'm sure this is a delightfully nonportable if statement.
  // I can't help it that root doesn't have the proper tools
  if (normfactor != normfactor || 1./normfactor==0) {
    // numbers were too big.  Use approximations:
    if (numer < 33) {
      if (spew) { cout << "Calculating binomial coeff using small numer.\n"; }
      normfactor = (denom+1)*BinomialCoeffSmall_n(numer, denom);
    }
    else if (denom-numer < 33) {
      if (spew) { cout << "Calculating binomial coeff using small denom-numer.\n"; }
      normfactor = (denom+1)*BinomialCoeffSmall_N_minus_n(numer, denom);
    }
    else {
      if (spew) { cout << "Calculating binomial coeff using Stirling's approx,\nassuming neither numer nor denom-numer is small.\n"; }
      normfactor = (denom+1)*ApproxBinomialCoeffAllLarge(numer, denom);
      // This case is bad.  The normalization factor gives a non-normalized pdf.
    }
    // if still failed resort to symmetric formula:
    if (normfactor != normfactor || 1./normfactor==0) {
      if (spew) { cout << "resorting to symmetric formula\n"; }
      FindEfficiencyWithSymmetricError(numer, denom, efficiency, errMin);
      errMax = errMin;
      if (spew) {
        cout << endl;
        cout << "efficiency = " << efficiency << endl;
        cout << "intervalMin = " << endl;
        cout << "intervalMax = " << endl;
        cout << "errMin = " << errMin << endl;
        cout << "errMax = " << errMax << endl;
        cout << "effPosteriorProb->Integral(intervalMin, intervalMax) = " << endl;
        cout << "effPosteriorProb->Eval(intervalMin) = " << endl;
        cout << "effPosteriorProb->Eval(intervalMax) = " << endl;
      }
      return;
    }
  }

  // make the TF1 of the posterior density of efficiency (using uniform priors):
  char formula[1000];
  sprintf(formula, "%e*pow(x, %e)*pow(1-x, %e)", normfactor, (double) numer, (double) (denom-numer));
  boost::scoped_ptr<TF1> effPosteriorProb( new TF1("effPosteriorProb", formula, 0, 1) );

  if( spew ) {
    cout << "formula = " << formula << endl;
    double xmin = effPosteriorProb->GetXmin();
    double xmax = effPosteriorProb->GetXmax();
    cout << "xmin = " << xmin << "\t" << "xmax = " << xmax << endl;
    cout << "effPosteriorProb->Integral(xmin, xmax) = " << effPosteriorProb->Integral(xmin, xmax) << endl;
  }

  // Set the range of the posterior density by using the values
  // given by the symmetric error case.
  // This needs to be done because root can't integrate the function
  // without sensible boundary values.
  double acceptSymm, errSymm;
  FindEfficiencyWithSymmetricError(numer, denom, acceptSymm, errSymm);
  double xminNew = acceptSymm-25.*errSymm;
  double xmaxNew = acceptSymm+25.*errSymm;
  if (errSymm<=0) {
    errSymm=sqrt(1./denom);
    xminNew = acceptSymm-5.*errSymm;
    xmaxNew = acceptSymm+5.*errSymm;
  }
  if (xminNew < 0) { xminNew = 0; }
  if (xmaxNew > 1) { xmaxNew = 1; }
  effPosteriorProb->SetRange(xminNew, xmaxNew);
  // Now that we have set the range, renormalize the function to make sure it's integral is 1:
  if (effPosteriorProb->Integral(xminNew, xmaxNew) == 0) {
    cout << "WARNING -- FindEfficiencyWithAsymmetricError() -- about to crash because of a\n";
    cout << "        divide by zero that should never happen\n";
  }
  normfactor = normfactor/effPosteriorProb->Integral(xminNew, xmaxNew);
  sprintf(formula, "%e*pow(x, %e)*pow(1-x, %e)", normfactor, (double) numer, (double) (denom-numer));
  effPosteriorProb.reset( new TF1("effPosteriorProb", formula, xminNew, xmaxNew) );

  if (spew) {
    double xmin = effPosteriorProb->GetXmin();
    double xmax = effPosteriorProb->GetXmax();
    cout << "xmin = " << xmin << "\t" << "xmax = " << xmax << endl;
    cout << "effPosteriorProb->Integral(xmin, xmax) = " << effPosteriorProb->Integral(xmin, xmax) << endl;
  }

  // if the numerator is exactly zero, we are on the boundary:
  if (numer==0) {
    intervalMin = 0;
    intervalMax = FindUpperBoundEqualToIntegral(effPosteriorProb.get(), 0, probability, precision, spew);
  }
  // if the numerator is equal to the denominator, we are on the other boundary:
  else if (numer==denom) {
    intervalMin = FindLowerBoundEqualToIntegral(effPosteriorProb.get(), 1, probability, precision, spew);
    intervalMax = 1;
  }
  else {
    // loop over intervalMin's, and find intervalMax such that
    // integral(effPosteriorProb, intervalMin, intervalMax) == 0.6827
    // and
    // effPosteriorProb(intervalMin) == effPosteriorProb(intervalMax)
    double intervalMinMin = effPosteriorProb->GetXmin();
    double intervalMinMax = efficiency;
    double intervalMinMid = (intervalMinMax+intervalMinMin)/2.;
    double intervalMaxTest = FindUpperBoundEqualToIntegral(effPosteriorProb.get(), intervalMinMid, probability, precision, spew);
    double difference = effPosteriorProb->Eval(intervalMinMid) - effPosteriorProb->Eval(intervalMaxTest);
    double average = effPosteriorProb->Eval(intervalMinMid) + effPosteriorProb->Eval(intervalMaxTest);
    int nIterations=0;
    int nIterationsMax=1000;
    while (fabs(difference) >= average*precision && nIterations < nIterationsMax) {
      if (effPosteriorProb->Eval(intervalMinMid) > effPosteriorProb->Eval(intervalMaxTest)) {
        // intervalMinMin = intervalMinMin;
        intervalMinMax = intervalMinMid;
        intervalMinMid = (intervalMinMax+intervalMinMin)/2.;
      }
      else {
        intervalMinMin = intervalMinMid;
        // intervalMinMax = intervalMinMax;
        intervalMinMid = (intervalMinMax+intervalMinMin)/2.;
      }
      intervalMaxTest = FindUpperBoundEqualToIntegral(effPosteriorProb.get(), intervalMinMid, probability, precision, spew);
      difference = effPosteriorProb->Eval(intervalMinMid) - effPosteriorProb->Eval(intervalMaxTest);
      average = effPosteriorProb->Eval(intervalMinMid) + effPosteriorProb->Eval(intervalMaxTest);
      // cout << "nIterations = " << nIterations << endl;
      // cout << "intervalMinMid = " << intervalMinMid << "\t";
      // cout << "intervalMinMin = " << intervalMinMin << "\t";
      // cout << "intervalMinMax = " << intervalMinMax << endl;
      // cout << "intervalMaxTest = " << intervalMaxTest << endl;
      // cout << "effPosteriorProb->Eval(intervalMinMid)  = " << effPosteriorProb->Eval(intervalMinMid)  << "\t";
      // cout << "effPosteriorProb->Eval(intervalMaxTest) = " << effPosteriorProb->Eval(intervalMaxTest) << endl << endl;
      nIterations++;
    }
    if (spew) {
      if (nIterations >= nIterationsMax) {
        cout << "WARNING -- FindEfficiencyWithAsymmetricError() -- 68.27% interval not found\n";
        cout << "        Hit maximum number of iterations\n";
        cout << "        nIterations = " << nIterations << endl;
        cout << "        intervalMinMid = " << intervalMinMid << "\t";
        cout << "        intervalMinMin = " << intervalMinMin << "\t";
        cout << "        intervalMinMax = " << intervalMinMax << endl;
        cout << "        intervalMaxTest = " << intervalMaxTest << endl;
        cout << "        effPosteriorProb->Eval(intervalMinMid)  = " << effPosteriorProb->Eval(intervalMinMid)  << "\t";
        cout << "        effPosteriorProb->Eval(intervalMaxTest) = " << effPosteriorProb->Eval(intervalMaxTest) << endl << endl;
      }
    }
    // cout << "nIterations = " << nIterations << endl;
    intervalMin = intervalMinMid;
    intervalMax = intervalMaxTest;
  }
  errMin = efficiency-intervalMin;
  errMax = intervalMax-efficiency;

  if (spew) {
    cout << endl;
    cout << "efficiency = " << efficiency << endl;
    cout << "intervalMin = " << intervalMin << endl;
    cout << "intervalMax = " << intervalMax << endl;
    cout << "errMin = " << errMin << endl;
    cout << "errMax = " << errMax << endl;
    cout << "effPosteriorProb->Integral(intervalMin, intervalMax) = " << effPosteriorProb->Integral(intervalMin, intervalMax) << endl;
    cout << "effPosteriorProb->Eval(intervalMin) = " << effPosteriorProb->Eval(intervalMin) << endl;
    cout << "effPosteriorProb->Eval(intervalMax) = " << effPosteriorProb->Eval(intervalMax) << endl;
    effPosteriorProb->Draw();
  }
  return;
}

// For a given function, lower bound, and integral, finds the upper
// bound at which integral of function from lower bound to upper bound
// equals integral to specified precision

const double
DataGraphics::FindUpperBoundEqualToIntegral(TF1 * fxn, const double& lowerBound, const double& integral, const double& precision, bool spew)
{
  // double xmin = fxn->GetXmin();
  double xmax = fxn->GetXmax();
  double upperBoundMin = lowerBound;
  double upperBoundMax = xmax;
  double upperBoundMid = (upperBoundMax + upperBoundMin)/2.;
  // double integralTestMin = 0;
  double maxIntegral = fxn->Integral(lowerBound, upperBoundMax);
  double integralTestMid = fxn->Integral(lowerBound, upperBoundMid);

  if (integral > maxIntegral) {
    // Can't find an upper bound equal to the integral in this range
    if (spew) {
      cout << "WARNING -- FindUpperBoundEqualToIntegral() -- finding given upper bound failed\n";
      cout << "        lowerBound = " << lowerBound << endl;
      cout << "        upperBoundMax = " << upperBoundMax << endl;
      cout << "        integral = " << integral << endl;
      cout << "        maxIntegral = " << maxIntegral << endl;
    }
    // exit with closest value possible:
    return xmax;
  }

  int nIterations=0;
  int nIterationsMax=1000;

  while (fabs(integral-integralTestMid) >= precision && nIterations < nIterationsMax) {
    if (integralTestMid > integral) {
      // upperBoundMin = upperBoundMin;
      upperBoundMax = upperBoundMid;
      upperBoundMid = (upperBoundMax + upperBoundMin)/2.;
    }
    else {
      upperBoundMin = upperBoundMid;
      // upperBoundMax = upperBoundMax;
      upperBoundMid = (upperBoundMax + upperBoundMin)/2.;
    }
    integralTestMid = fxn->Integral(lowerBound, upperBoundMid);
    nIterations++;
  }
  // cout << "nIterations = " << nIterations << endl;
  // cout << "integralTestMid = " << integralTestMid << endl;
  return upperBoundMid;
}

const double
DataGraphics::FindLowerBoundEqualToIntegral(TF1 * fxn, const double& upperBound, const double& integral, const double& precision, bool spew)
{
  double xmin = fxn->GetXmin();
  // double xmax = fxn->GetXmax();
  double lowerBoundMin = xmin;
  double lowerBoundMax = upperBound;
  double lowerBoundMid = (lowerBoundMax + lowerBoundMin)/2.;
  // double integralTestMin = 0;
  double maxIntegral = fxn->Integral(lowerBoundMin, upperBound);
  double integralTestMid = fxn->Integral(lowerBoundMid, upperBound);

  if (integral > maxIntegral) {
    // Can't find an upper bound equal to the integral in this range
    if (spew) {
      cout << "WARNING -- FindLowerBoundEqualToIntegral() -- finding given upper bound failed\n";
    }
    // exit with closest value possible:
    return xmin;
  }

  int nIterations=0;
  int nIterationsMax=1000;

  while (fabs(integral-integralTestMid) >= precision && nIterations < nIterationsMax) {
    if (integralTestMid > integral) {
      lowerBoundMin = lowerBoundMid;
      // lowerBoundMax = lowerBoundMax;
      lowerBoundMid = (lowerBoundMax + lowerBoundMin)/2.;
    }
    else {
      // lowerBoundMin = lowerBoundMin;
      lowerBoundMax = lowerBoundMid;
      lowerBoundMid = (lowerBoundMax + lowerBoundMin)/2.;
    }
    integralTestMid = fxn->Integral(lowerBoundMid, upperBound);
    nIterations++;
  }
  // cout << "nIterations = " << nIterations << endl;
  // cout << "integralTestMid = " << integralTestMid << endl;
  return lowerBoundMid;
}

const double
DataGraphics::BinomialCoeffSmall_n(const Count& n, const Count& N)
{
  assert( n <= std::numeric_limits<unsigned long>::max() );
  assert( N <= std::numeric_limits<unsigned long>::max() );
  assert( n<=N );
  double numer = 1.;
  double denom = 1.;
  for( unsigned long ii=static_cast<unsigned long>(N), fi=static_cast<unsigned long>(N-n); ii>fi; --ii ) {
    numer *= ((double)ii);
  }
  for( unsigned long ii=static_cast<unsigned long>(n); ii; --ii) {
    denom *= ((double)ii);
  }
  assert( denom > 0. );
  return numer/denom;
}

const double
DataGraphics::BinomialCoeffSmall_N_minus_n(const Count& n, const Count& N)
{
  return BinomialCoeffSmall_n(N-n, N);
}

const double
DataGraphics::ApproxBinomialCoeffAllLarge(const Count& n, const Count& N)
{
  return pow( (double)(N/(N-n)), (double)(N-n))*pow((double)(N/n), (double)(n) );
}
