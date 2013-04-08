
#ifndef WRAP_DGBINOMIALERRORS_HH
#define WRAP_DGBINOMIALERRORS_HH

// ================================================================
// 
//   BinomialErrors.hh
//
//        Code to compute binomial/asymmetric errors on efficiency.  
//        obtained from Adam Scott, UCSB, with pieces based on
//        techniques in CDF5894
//
//        Adam Scott, UC Santa Barbara
//        Antonio Boveia, UC Santa Barbara
//
// ================================================================

#include <DataGraphics/DgTypes.hh>

class TF1;

namespace 
DataGraphics
{
  void         FindEfficiencyWithAsymmetricError(const Count& numer, const Count& denom, 
                                                 double& efficiency, double& errMin, double& errMax, bool spew = false);
  void         FindEfficiencyWithSymmetricError(const Count& numer, const Count& denom, double& efficiency, double& err);
  const double FindUpperBoundEqualToIntegral(TF1* fxn, const double& lowerBound, const double& integral, const double& precision, bool spew);
  const double FindLowerBoundEqualToIntegral(TF1* fxn, const double& upperBound, const double& integral, const double& precision, bool spew);
  const double BinomialCoeffSmall_n(const Count& n, const Count& N);
  const double BinomialCoeffSmall_N_minus_n(const Count& n, const Count& N);
  const double ApproxBinomialCoeffAllLarge(const Count& n, const Count& N);
};

#endif // WRAP_DGBINOMIALERRORS_HH

