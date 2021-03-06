// @(#)root/hist:$Id: TFractionFitter2.h 38775 2011-04-08 08:27:27Z moneta $
// Author: Frank Filthaut F.Filthaut@science.ru.nl  20/05/2002
   
#ifndef ROOT_TFractionFitter2
#define ROOT_TFractionFitter2

#ifndef ROOT_TVirtualFitter
# include "TVirtualFitter.h"
#endif
#ifndef ROOT_TObjArray
# include "TObjArray.h"
#endif
#include <vector>

class TH1;

///////////////////////////////////////////////////////////////////////////////
// TFractionFitter2
//
// Fits MC fractions to data histogram (a la HMCMLL, see R. Barlow and C. Beeston,
// Comp. Phys. Comm. 77 (1993) 219-228, and http://www.hep.man.ac.uk/~roger/hfrac.f).
// 
///////////////////////////////////////////////////////////////////////////////

class TFractionFitter2 : public TObject {
public:
   TFractionFitter2();
   TFractionFitter2(TH1* data, TObjArray *MCs, Option_t *option="");
   virtual ~TFractionFitter2();

   TVirtualFitter* GetFitter() const;
   void ErrorAnalysis(Double_t UP);
   void SetRangeX(Int_t low, Int_t high);
   void ReleaseRangeX();
   void SetRangeY(Int_t low, Int_t high);
   void ReleaseRangeY();
   void SetRangeZ(Int_t low, Int_t high);
   void ReleaseRangeZ();
   void ExcludeBin(Int_t bin);
   void IncludeBin(Int_t bin);
   void Constrain(Int_t parm, Double_t low, Double_t high);
   void UnConstrain(Int_t parm);
   void SetData(TH1 *data);
   void SetMC(Int_t parm, TH1 *MC);
   void SetWeight(Int_t parm, TH1* weight);
   Int_t Fit();

   void GetResult(Int_t parm, Double_t& value, Double_t& error) const;
   TH1* GetPlot();

   // This global function needs access to computeFCN()
   friend void TFractionFitFCN2(Int_t& npar, Double_t* gin, Double_t& f, Double_t* par, Int_t flag);

   // Goodness of fit
   Double_t GetChisquare() const;
   Int_t GetNDF() const;
   Double_t GetProb() const;

   // MC predictions (smeared templates)
   TH1* GetMCPrediction(Int_t parm) const;

private:
   void CheckParNo(Int_t parm) const;
   void CheckConsistency();
   bool FindPrediction(int bin, double* fractions, double& Ti, int& k0, double& Aki) const;
   void ComputeFCN(Int_t& npar, Double_t* gin, Double_t& f, Double_t* par, Int_t flag);
   void GetRanges(Int_t& minX, Int_t& maxX, Int_t& minY, Int_t& maxY,
                  Int_t& minZ, Int_t& maxZ) const;
   void ComputeChisquareLambda();
   bool IsExcluded(Int_t bin) const;

protected:
   Bool_t   fFitDone;             // flags whether a valid fit has been performed
   Bool_t   fFitInfinite;         // flags whether fit went into an infinite loop, which had to be broken "by hand"
   Int_t    fLowLimitX;           // first bin in X dimension
   Int_t    fHighLimitX;          // last  bin in X dimension
   Int_t    fLowLimitY;           // first bin in Y dimension
   Int_t    fHighLimitY;          // last  bin in Y dimension
   Int_t    fLowLimitZ;           // first bin in Z dimension
   Int_t    fHighLimitZ;          // last  bin in Z dimension
   std::vector<Int_t> fExcludedBins; // bins excluded from the fit

   Int_t    fNpfits;              // Number of points used in the fit
   Int_t    fNDF;                 // Number of degrees of freedom in the fit
   Double_t fChisquare;           // Template fit chisquare

   TObjArray fAji;                // array of pointers to predictions of real template distributions

   // Histograms
   TH1*      fData;               // pointer to the "data" histogram to be fitted to
   TObjArray fMCs;                // array of pointers to template histograms
   TObjArray fWeights;            // array of pointers to corresponding weight factors (may be null)
   Double_t  fIntegralData;       // "data" histogram content integral over allowed fit range
   Double_t* fIntegralMCs;        // same for template histograms (weights not taken into account)
   Double_t* fFractions;          // template fractions scaled to the "data" histogram statistics
   TH1*      fPlot;               // pointer to histogram containing summed template predictions

   Int_t     fNpar;               // number of fit parameters

   ClassDef(TFractionFitter2, 0)
};

//
//  TFractionFitFCN2
//
//  Computes negative log-likelihood for TFractionFitter2
//

void TFractionFitFCN2(Int_t& npar, Double_t* gin, Double_t& f, Double_t* par, Int_t flag);


// @(#)root/hist:$Id: TFractionFitter2.cxx 38775 2011-04-08 08:27:27Z moneta $
// Author: Frank Filthaut F.Filthaut@science.ru.nl  20/05/2002
// with additions by Bram Wijngaarden <dwijngaa@hef.kun.nl>

///////////////////////////////////////////////////////////////////////////////
//
// Fits MC fractions to data histogram (a la HMCMLL, see R. Barlow and C. Beeston,
// Comp. Phys. Comm. 77 (1993) 219-228, and http://www.hep.man.ac.uk/~roger/hfrac.f).
//
// The virtue of this fit is that it takes into account both data and Monte Carlo
// statistical uncertainties. The way in which this is done is through a standard
// likelihood fit using Poisson statistics; however, the template (MC) predictions
// are also varied within statistics, leading to additional contributions to the
// overall likelihood. This leads to many more fit parameters (one per bin per
// template), but the minimisation with respect to these additional parameters is
// done analytically rather than introducing them as formal fit parameters. Some
// special care needs to be taken in the case of bins with zero content. For more
// details please see the original publication cited above.
//
// An example application of this fit is given below. For a TH1* histogram
// ("data") fitted as the sum of three Monte Carlo sources ("mc"):
//
// {
//   TH1F *data;                              //data histogram
//   TH1F *mc0;                               // first MC histogram
//   TH1F *mc1;                               // second MC histogram
//   TH1F *mc2;                               // third MC histogram
//   ....                                     // retrieve histograms
//   TObjArray *mc = new TObjArray(3);        // MC histograms are put in this array
//   mc->Add(mc0);
//   mc->Add(mc1);
//   mc->Add(mc2);
//   TFractionFitter2* fit = new TFractionFitter2(data, mc); // initialise
//   fit->Constrain(1,0.0,1.0);               // constrain fraction 1 to be between 0 and 1
//   fit->SetRangeX(1,15);                    // use only the first 15 bins in the fit
//   Int_t status = fit->Fit();               // perform the fit
//   cout << "fit status: " << status << endl;
//   if (status == 0) {                       // check on fit status
//     TH1F* result = (TH1F*) fit->GetPlot();
//     data->Draw("Ep");
//     result->Draw("same");
//   }
// }
//
//
// Assumptions
// ===========
// A few assumptions need to be made for the fit procedure to be carried out:
//
// (1) The total number of events in each template is not too small
//     (so that its Poisson uncertainty can be neglected).
// (2) The number of events in each bin is much smaller than the total
//     number of events in each template (so that multinomial
//     uncertainties can be replaced with Poisson uncertainties).
//
// Biased fit uncertainties may result if these conditions are not fulfilled
// (see e.g. arXiv:0803.2711).
//
// Instantiation
// =============
// A fit object is instantiated through
//     TFractionFitter2* fit = new TFractionFitter2(data, mc);
// A number of basic checks (intended to ensure that the template histograms
// represent the same "kind" of distribution as the data one) are carried out.
// The TVirtualFitter object is then addressed and all fit parameters (the
// template fractions) declared (initially unbounded).
//
// Applying constraints
// ====================
// Fit parameters can be constrained through
//     fit->Constrain(parameter #, lower bound, upper bound);
// Setting lower bound = upper bound = 0 removes the constraint (a la Minuit);
// however, a function
//     fit->Unconstrain(parameter #)
// is also provided to simplify this.
//
// Setting parameter values
// ========================
// The function
//     TVirtualFitter* vFit = fit->GetFitter();
// is provided for direct access to the TVirtualFitter object. This allows to
// set and fix parameter values, and set step sizes directly.
//
// Restricting the fit range
// =========================
// The fit range can be restricted through
//     fit->SetRangeX(first bin #, last bin #);
// and freed using
//     fit->ReleaseRangeX();
// For 2D histograms the Y range can be similarly restricted using
//     fit->SetRangeY(first bin #, last bin #);
//     fit->ReleaseRangeY();
// and for 3D histograms also
//     fit->SetRangeZ(first bin #, last bin #);
//     fit->ReleaseRangeZ();
// It is also possible to exclude individual bins from the fit through
//     fit->ExcludeBin(bin #);
// where the given bin number is assumed to follow the TH1::GetBin() numbering.
// Any bins excluded in this way can be included again using the corresponding
//     fit->IncludeBin(bin #);
//
// Weights histograms
// ==================
// Weights histograms (for a motivation see the above publication) can be specified
// for the individual MC sources through
//     fit->SetWeight(parameter #, pointer to weights histogram);
// and unset by specifying a null pointer.
//
// Obtaining fit results
// =====================
// The fit is carried out through
//     Int_t status = fit->Fit();
// where  status  is the code returned from the "MINIMIZE" command. For fits
// that converged, parameter values and errors can be obtained through
//     fit->GetResult(parameter #, value, error);
// and the histogram corresponding to the total Monte Carlo prediction (which
// is not the same as a simple weighted sum of the input Monte Carlo distributions)
// can be obtained by
//     TH1* result = fit->GetPlot();
//
// Using different histograms
// ==========================
// It is possible to change the histogram being fitted through
//     fit->SetData(TH1* data);
// and to change the template histogram for a given parameter number through
//     fit->SetMC(parameter #, TH1* MC);
// This can speed up code in case of multiple data or template histograms;
// however, it should be done with care as any settings are taken over from
// the previous fit. In addition, neither the dimensionality nor the numbers of
// bins of the histograms should change (in that case it is better to instantiate
// a new TFractionFitter2 object).
//
// Errors
// ======
// Any serious inconsistency results in an error.
//
///////////////////////////////////////////////////////////////////////////////

#include "TH1.h"
#include "TMath.h"
#include "TClass.h"

TVirtualFitter *fractionFitter=0;

//______________________________________________________________________________
TFractionFitter2::TFractionFitter2() :
   fFitDone(kFALSE), fFitInfinite(kFALSE),
   fLowLimitX(0), fHighLimitX(0),
   fLowLimitY(0), fHighLimitY(0),
   fLowLimitZ(0), fHighLimitZ(0),
   fData(0), fIntegralData(0),
   fPlot(0) 
{
   // TFractionFitter2 default constructor.

   fractionFitter = 0;
   fIntegralMCs   = 0;
   fFractions     = 0;

   fNpfits        = 0;
   fNDF           = 0;
   fChisquare     = 0;
   fNpar          = 0;
}

//______________________________________________________________________________
TFractionFitter2::TFractionFitter2(TH1* data, TObjArray  *MCs, Option_t *option) :
  fFitDone(kFALSE), fFitInfinite(kFALSE), fChisquare(0), fPlot(0)  {
   // TFractionFitter2 constructor. Does a complete initialisation (including
   // consistency checks, default fit range as the whole histogram but without
   // under- and overflows, and declaration of the fit parameters). Note that
   // the histograms are not copied, only references are used.
   // Arguments:
   //     data: histogram to be fitted
   //     MCs:  array of TH1* corresponding template distributions
   //    Option:  can be used to control the print level of the minimization algorithm
   //             option = "Q"  : quite - no message is printed
   //             option = "V"  : verbose - max print out 
   //             option = ""   : default: print initial fraction values and result 

   fData = data;
   // Default: include all of the histogram (but without under- and overflows)
   fLowLimitX = 1;
   fHighLimitX = fData->GetNbinsX();
   if (fData->GetDimension() > 1) {
      fLowLimitY = 1;
      fHighLimitY = fData->GetNbinsY();
      if (fData->GetDimension() > 2) {
         fLowLimitZ = 1;
         fHighLimitZ = fData->GetNbinsZ();
      }
   }
   fNpar = MCs->GetEntries();
   Int_t par;
   for (par = 0; par < fNpar; ++par) {
      fMCs.Add(MCs->At(par));
      // Histogram containing template prediction
      TString s = Form("Prediction for MC sample %i",par);
      TH1* pred = (TH1*) ((TH1*)MCs->At(par))->Clone(s);
      pred->SetTitle(s);
      fAji.Add(pred);
   }
   fIntegralMCs = new Double_t[fNpar];
   fFractions = new Double_t[fNpar];

   CheckConsistency();
   fWeights.Expand(fNpar);

   fractionFitter = TVirtualFitter::Fitter(this, fNpar);
   fractionFitter->Clear();
   fractionFitter->SetObjectFit(this);
   fractionFitter->SetFCN(TFractionFitFCN2);

   // set print level 
   TString opt(option);
   opt.ToUpper();
   double plist[1];
   if (opt.Contains("Q") ) { 
      plist[0] = -1;
      fractionFitter->ExecuteCommand("SET PRINT",plist,1);
      fractionFitter->ExecuteCommand("SET NOW",plist,0);
   }
   else if (opt.Contains("V") ) { 
      plist[0] =  1;
      fractionFitter->ExecuteCommand("SET PRINT",plist,1);
   }

   Double_t defaultFraction = 1.0/((Double_t)fNpar);
   Double_t defaultStep = 0.01;
   for (par = 0; par < fNpar; ++par) {
      TString name("frac"); name += par;
      fractionFitter->SetParameter(par, name.Data(), defaultFraction, defaultStep, 0, 0);
   }
}

//______________________________________________________________________________
TFractionFitter2::~TFractionFitter2() {
  // TFractionFitter2 default destructor

   delete fractionFitter;
   delete[] fIntegralMCs;
   delete[] fFractions;
}

//______________________________________________________________________________
void TFractionFitter2::SetData(TH1* data) {
   // Change the histogram to be fitted to. Notes:
   // - Parameter constraints and settings are retained from a possible previous fit.
   // - Modifying the dimension or number of bins results in an error (in this case
   //   rather instantiate a new TFractionFitter2 object)

   fData = data;
   fFitDone = kFALSE;
   fFitInfinite = kFALSE;
   CheckConsistency();
}

//______________________________________________________________________________
void TFractionFitter2::SetMC(Int_t parm, TH1* MC) {
   // Change the histogram for template number <parm>. Notes:
   // - Parameter constraints and settings are retained from a possible previous fit.
   // - Modifying the dimension or number of bins results in an error (in this case
   //   rather instantiate a new TFractionFitter2 object)

   CheckParNo(parm);
   fMCs.RemoveAt(parm);
   fMCs.AddAt(MC,parm);
   fFitDone = kFALSE;
   fFitInfinite = kFALSE;
   CheckConsistency();
}

//______________________________________________________________________________
void TFractionFitter2::SetWeight(Int_t parm, TH1* weight) {
   // Set bin by bin weights for template number <parm> (the parameter numbering
   // follows that of the input template vector).
   // Weights can be "unset" by passing a null pointer.
   // Consistency of the weights histogram with the data histogram is checked at
   // this point, and an error in case of problems.

   CheckParNo(parm);
   if (fWeights[parm]) {
      fWeights.RemoveAt(parm);
   }
   if (weight) {
      if (weight->GetNbinsX() != fData->GetNbinsX() ||
          (fData->GetDimension() > 1 && weight->GetNbinsY() != fData->GetNbinsY()) ||
          (fData->GetDimension() > 2 && weight->GetNbinsZ() != fData->GetNbinsZ())) {
         Error("SetWeight","Inconsistent weights histogram for source %d", parm);
         return;
      }
      TString ts = "weight hist: "; ts += weight->GetName();
      fWeights.AddAt(weight,parm);
   }
}

//______________________________________________________________________________
TVirtualFitter* TFractionFitter2::GetFitter() const {
   // Give direct access to the underlying minimisation class. This can be
   // used e.g. to modify parameter values or step sizes.

   return fractionFitter;
}

//______________________________________________________________________________
void TFractionFitter2::CheckParNo(Int_t parm) const {
   // Function for internal use, checking parameter validity
   // An invalid parameter results in an error.

   if (parm < 0 || parm > fNpar) {
      Error("CheckParNo","Invalid parameter number %d",parm);
   }
}

//______________________________________________________________________________
void TFractionFitter2::SetRangeX(Int_t low, Int_t high) {
   // Set the X range of the histogram to be used in the fit.
   // Use ReleaseRangeX() to go back to fitting the full histogram.
   // The consistency check ensures that no empty fit range occurs (and also
   // recomputes the bin content integrals).
   // Arguments:
   //     low:  lower X bin number
   //     high: upper X bin number

   fLowLimitX = (low > 0 ) ? low : 1;
   fHighLimitX = ( high > 0 && high <= fData->GetNbinsX()) ? high : fData->GetNbinsX();
   CheckConsistency();
}

//______________________________________________________________________________
void TFractionFitter2::ReleaseRangeX() {
   // Release restrictions on the X range of the histogram to be used in the fit.

   fLowLimitX = 1;
   fHighLimitX = fData->GetNbinsX();
   CheckConsistency();
}

//______________________________________________________________________________
void TFractionFitter2::SetRangeY(Int_t low, Int_t high) {
   // Set the Y range of the histogram to be used in the fit (2D or 3D histograms only).
   // Use ReleaseRangeY() to go back to fitting the full histogram.
   // The consistency check ensures that no empty fit range occurs (and also
   // recomputes the bin content integrals).
   // Arguments:
   //     low:  lower Y bin number
   //     high: upper Y bin number

   if (fData->GetDimension() < 2) {
      Error("SetRangeY","Y range cannot be set for 1D histogram");
      return;
   }

   fLowLimitY = (low > 0) ? low : 1;
   fHighLimitY = (high > 0 && high <= fData->GetNbinsY()) ? high : fData->GetNbinsY();
   CheckConsistency();
}

//______________________________________________________________________________
void TFractionFitter2::ReleaseRangeY() {
   // Release restrictions on the Y range of the histogram to be used in the fit.

   fLowLimitY = 1;
   fHighLimitY = fData->GetNbinsY();
   CheckConsistency();
}


//______________________________________________________________________________
void TFractionFitter2::SetRangeZ(Int_t low, Int_t high) {
   // Set the Z range of the histogram to be used in the fit (3D histograms only).
   // Use ReleaseRangeY() to go back to fitting the full histogram.
   // The consistency check ensures that no empty fit range occurs (and also
   // recomputes the bin content integrals).
   // Arguments:
   //     low:  lower Y bin number
   //     high: upper Y bin number

   if (fData->GetDimension() < 3) {
      Error("SetRangeZ","Z range cannot be set for 1D or 2D histogram");
      return;
   }


   fLowLimitZ = (low > 0) ? low : 1;
   fHighLimitZ = (high > 0 && high <= fData->GetNbinsZ()) ? high : fData->GetNbinsZ();
   CheckConsistency();
}

//______________________________________________________________________________
void TFractionFitter2::ReleaseRangeZ() {
   // Release restrictions on the Z range of the histogram to be used in the fit.

   fLowLimitZ = 1;
   fHighLimitZ = fData->GetNbinsZ();
   CheckConsistency();
}

//______________________________________________________________________________
void TFractionFitter2::ExcludeBin(Int_t bin) {
   // Exclude the given bin from the fit. The bin numbering to be used is that
   // of TH1::GetBin().

   int excluded = fExcludedBins.size();
   for (int b = 0; b < excluded; ++b) {
      if (fExcludedBins[b] == bin) {
	 Error("ExcludeBin", "bin %d already excluded", bin);
	 return;
      }
   }
   fExcludedBins.push_back(bin);
   // This call serves to properly (re)determine the number of degrees of freeom
   CheckConsistency();
}

//______________________________________________________________________________
void TFractionFitter2::IncludeBin(Int_t bin) {
   // Include the given bin in the fit, if it was excluded before using ExcludeBin().
   // The bin numbering to be used is that of TH1::GetBin().

   for (std::vector<Int_t>::iterator it = fExcludedBins.begin();
	it != fExcludedBins.end(); ++it) {
      if (*it == bin) {
  	 fExcludedBins.erase(it);
	 // This call serves to properly (re)determine the number of degrees of freeom
	 CheckConsistency();
	 return;
      }
   }
   Error("IncludeBin", "bin %d was not excluded", bin);
}

//______________________________________________________________________________
bool TFractionFitter2::IsExcluded(Int_t bin) const {
   // Function for internal use, checking whether the given bin is
   // excluded from the fit or not.

   for (unsigned int b = 0; b < fExcludedBins.size(); ++b) 
      if (fExcludedBins[b] == bin) return true;
   return false;
}

//______________________________________________________________________________
void TFractionFitter2::Constrain(Int_t parm, Double_t low, Double_t high) {
   // Constrain the values of parameter number <parm> (the parameter numbering
   // follows that of the input template vector).
   // Use UnConstrain() to remove this constraint.

   CheckParNo(parm);
   Double_t plist[3];
   plist[0] = (Double_t) parm;
   plist[1] = low;
   plist[2] = high;
   fractionFitter->ExecuteCommand("SET LIMIT", plist, 3);
}

//______________________________________________________________________________
void TFractionFitter2::UnConstrain(Int_t parm) {
   // Remove the constraints on the possible values of parameter <parm>.

   CheckParNo(parm);
   Double_t plist[3];
   plist[0] = (Double_t) parm;
   plist[1] = 0;
   plist[2] = 0;
   fractionFitter->ExecuteCommand("SET LIMIT", plist, 3);
}

//______________________________________________________________________________
void TFractionFitter2::CheckConsistency() {
   // Function used internally to check the consistency between the
   // various histograms. Checks are performed on nonexistent or empty
   // histograms, the precise histogram class, and the number of bins.
   // In addition, integrals over the "allowed" bin ranges are computed.
   // Any inconsistency results in a error.

   if (! fData) {
      Error("CheckConsistency","Nonexistent data histogram");
      return;
   }
   Int_t minX, maxX, minY, maxY, minZ, maxZ;
   Int_t x,y,z,par;
   GetRanges(minX, maxX, minY, maxY, minZ, maxZ);
   fIntegralData = 0;
   fNpfits = 0;
   for (z = minZ; z <= maxZ; ++z) {
      for (y = minY; y <= maxY; ++y) {
         for (x = minX; x <= maxX; ++x) {
	    if (IsExcluded(fData->GetBin(x, y, z))) continue;
            fNpfits++;
            fIntegralData += fData->GetBinContent(x, y, z);
         }
      }
   }
   if (fIntegralData <= 0) {
      Error("CheckConsistency","Empty data histogram");
      return;
   }
   TClass* cl = fData->Class();

   fNDF = fNpfits - fNpar;

   if (fNpar < 2) {
      Error("CheckConsistency","Need at least two MC histograms");
      return;
   }

   for (par = 0; par < fNpar; ++par) {
      TH1 *h = (TH1*)fMCs.At(par);
      if (! h) {
         Error("CheckConsistency","Nonexistent MC histogram for source #%d",par);
         return;
      }
      if ((! h->Class()->InheritsFrom(cl)) || h->GetNbinsX() != fData->GetNbinsX() ||
          (fData->GetDimension() > 1 && h->GetNbinsY() != fData->GetNbinsY()) ||
          (fData->GetDimension() > 2 && h->GetNbinsZ() != fData->GetNbinsZ())) {
         Error("CheckConsistency","Histogram inconsistency for source #%d",par);
         return;
      }
      fIntegralMCs[par] = 0;
      for (z = minZ; z <= maxZ; ++z) {
         for (y = minY; y <= maxY; ++y) {
            for (x = minX; x <= maxX; ++x) {
	       if (IsExcluded(fData->GetBin(x, y, z))) continue;
               fIntegralMCs[par] += h->GetBinContent(x, y, z);
            }
         }
      }
      if (fIntegralMCs[par] <= 0) {
         Error("CheckConsistency","Empty MC histogram #%d",par);
      }
   }
}

//______________________________________________________________________________
Int_t TFractionFitter2::Fit() {
   // Perform the fit with the default UP value.
   // The value returned is the minimisation status.

  fFitInfinite = false; // reset

   Double_t plist[1];
   plist[0] = 0.5;
   // set the UP value to 0.5
   fractionFitter->ExecuteCommand("SET ERRDEF",plist,1);

   // remove any existing output histogram
   if (fPlot) {
      delete fPlot; fPlot = 0;
   }

   // Make sure the correct likelihood computation is used
   fractionFitter->SetObjectFit(this);

   // fit
   Int_t status = fractionFitter->ExecuteCommand("MINIMIZE",0,0);
   if ( fFitInfinite ) status = 999;
   if (status == 0) fFitDone = kTRUE;

   // determine goodness of fit
   ComputeChisquareLambda();

   return status;
}

//______________________________________________________________________________
void TFractionFitter2::ErrorAnalysis(Double_t UP) {
   // Set UP to the given value (see class TMinuit), and perform a MINOS minimisation.

   if (! fFitDone) {
      Error("ErrorAnalysis","Fit not yet performed");
      return;
   }

   // Make sure the correct likelihood computation is used
   fractionFitter->SetObjectFit(this);

   Double_t plist[1];
   plist[0] = UP > 0 ? UP : 0.5;
   fractionFitter->ExecuteCommand("SET ERRDEF",plist,1);
   Int_t status = fractionFitter->ExecuteCommand("MINOS",0,0);
   if (status != 0) {
       Error("ErrorAnalysis","Error return from MINOS: %d",status);
   }
}

//______________________________________________________________________________
void TFractionFitter2::GetResult(Int_t parm, Double_t& value, Double_t& error) const {
   // Obtain the fit result for parameter <parm> (the parameter numbering
   // follows that of the input template vector).

   CheckParNo(parm);
   if (! fFitDone) {
      Error("GetResult","Fit not yet performed");
      return;
   }
   char parname[100];
   Double_t vlow, vhigh;

   fractionFitter->GetParameter(parm, parname, value, error, vlow, vhigh);
}

//______________________________________________________________________________
TH1* TFractionFitter2::GetPlot() {
   // Return the "template prediction" corresponding to the fit result (this is not
   // the same as the weighted sum of template distributions, as template statistical
   // uncertainties are taken into account).
   // Note that the name of this histogram will simply be the same as that of the
   // "data" histogram, prefixed with the string "Fraction fit to hist: ".

   if (! fFitDone) {
      Error("GetPlot","Fit not yet performed");
      return 0;
   }
   if (! fPlot) {
      Double_t plist[1];
      plist[0] = 3;
      fractionFitter->ExecuteCommand("CALL FCN", plist, 1);
   }
   return fPlot;
}

//______________________________________________________________________________
void TFractionFitter2::GetRanges(Int_t& minX, Int_t& maxX, Int_t& minY, Int_t& maxY,
                                Int_t& minZ, Int_t& maxZ) const {
   // Used internally to obtain the bin ranges according to the dimensionality of
   // the histogram and the limits set by hand.

   if (fData->GetDimension() < 2) {
      minY = maxY = minZ = maxZ = 0;
      minX = fLowLimitX;
      maxX = fHighLimitX;
   } else if (fData->GetDimension() < 3) {
      minZ = maxZ = 0;
      minX = fLowLimitX;
      maxX = fHighLimitX;
      minY = fLowLimitY;
      maxY = fHighLimitY;
   } else {
      minX = fLowLimitX;
      maxX = fHighLimitX;
      minY = fLowLimitY;
      maxY = fHighLimitY;
      minZ = fLowLimitZ;
      maxZ = fHighLimitZ;
   }
}

//______________________________________________________________________________
void TFractionFitter2::ComputeFCN(Int_t& /*npar*/, Double_t* /*gin*/,
                                 Double_t& f, Double_t* xx, Int_t flag)
{
   // Used internally to compute the likelihood value.

   // normalise the fit parameters
   Int_t bin, mc;
   Int_t minX, maxX, minY, maxY, minZ, maxZ;
   Int_t x,y,z;
   GetRanges(minX, maxX, minY, maxY, minZ, maxZ);
   for (mc = 0; mc < fNpar; ++mc) {
      Double_t tot;
      TH1 *h  = (TH1*)fMCs[mc];
      TH1 *hw = (TH1*)fWeights[mc];
      if (hw) {
         tot = 0;
         for (z = minZ; z <= maxZ; ++z) {
            for (y = minY; y <= maxY; ++y) {
               for (x = minX; x <= maxX; ++x) {
		  if (IsExcluded(fData->GetBin(x, y, z))) continue;
                  Double_t weight = hw->GetBinContent(x, y, z);
                  if (weight <= 0) {
                     Error("ComputeFCN","Invalid weight encountered for MC source %d",mc);
                     return;
                  }
                  tot += weight * h->GetBinContent(x, y, z);
               }
            }
         }
      } else tot = fIntegralMCs[mc];
      fFractions[mc] = xx[mc] * fIntegralData / tot;
   }

   if (flag == 3) {
      TString ts = "Fraction fit to hist: "; ts += fData->GetName();
      fPlot = (TH1*) fData->Clone(ts.Data());
      fPlot->Reset();
   }
   // likelihood computation
   Double_t result = 0;
   for (z = minZ; z <= maxZ; ++z) {
      for (y = minY; y <= maxY; ++y) {
         for (x = minX; x <= maxX; ++x) {
            bin = fData->GetBin(x, y, z);
	    if (IsExcluded(bin)) continue;

            // Solve for the "predictions"
            int k0;
            Double_t ti; Double_t aki;
            const bool status = FindPrediction(bin, fFractions, ti, k0, aki);
	    if(!status) {
	      // skip this bin, and label the entire fit as Infinite (aka failed)
	      fFitInfinite = true;
	      continue;
	    }

            Double_t prediction = 0;
            for (mc = 0; mc < fNpar; ++mc) {
               TH1 *h  = (TH1*)fMCs[mc];
               TH1 *hw = (TH1*)fWeights[mc];
               Double_t binPrediction;
               Double_t binContent = h->GetBinContent(bin);
               Double_t weight = hw ? hw->GetBinContent(bin) : 1;
               if (k0 >= 0 && fFractions[mc] == fFractions[k0]) {
                  binPrediction = aki;
               } else {
                  binPrediction = binContent > 0 ? binContent / (1+weight*fFractions[mc]*ti) : 0;
               }

               prediction += fFractions[mc]*weight*binPrediction;
               result -= binPrediction;
               if (binContent > 0 && binPrediction > 0)
                 result += binContent*TMath::Log(binPrediction);
 
               if (flag == 3) {
                  ((TH1*)fAji.At(mc))->SetBinContent(bin, binPrediction);
               }
            }

            if (flag == 3) {
               fPlot->SetBinContent(bin, prediction);
            }

            result -= prediction;
            Double_t found = fData->GetBinContent(bin);
            if (found > 0 && prediction > 0)
               result += found*TMath::Log(prediction);
         }
      }
   }

   f = -result;
}

//______________________________________________________________________________
bool TFractionFitter2::FindPrediction(int bin, Double_t *fractions, Double_t &ti, int& k0, Double_t &aki) const {
   // Function used internally to obtain the template prediction in the individual bins

   // Sanity check: none of the fractions should be =0
   Int_t par;
   TH1 *hw;
   for (par = 0; par < fNpar; ++par) {
      hw = (TH1*)fWeights.At(par);
      Double_t weightedFraction = hw ?
         hw->GetBinContent(bin) * fractions[par] : fractions[par];
      if (weightedFraction == 0) {
         Error("FindPrediction","Fraction[%d] = 0!",par);
         return true;
      }
   }

   // Case data = 0
   if (TMath::Nint(fData->GetBinContent(bin)) == 0) {
      ti = 1;
      k0 = -1;
      aki=0;
      return true;
   }

   // Case one or more of the MC bin contents = 0: find largest fraction
   k0 = 0;
   TH1 *hw0 = (TH1*)fWeights.At(0);
   Double_t refWeightedFraction = hw0 ?
      hw0->GetBinContent(bin) * fractions[0] : fractions[0];
   for (par = 1; par < fNpar; ++par) {
      hw = (TH1*)fWeights.At(par);
      Double_t weightedFraction = hw ?
       hw->GetBinContent(bin) * fractions[par] : fractions[par];
      if (weightedFraction > refWeightedFraction) {
         k0 = par;
         refWeightedFraction = weightedFraction;
      }
   }
   int nMax = 1;
   Double_t contentsMax = ((TH1*)fMCs.At(k0))->GetBinContent(bin);
   for (par = 0; par < fNpar; ++par) {
      if (par == k0) continue;
      hw = (TH1*)fWeights.At(par);
      Double_t weightedFraction = hw ?
       hw->GetBinContent(bin) * fractions[par] : fractions[par];
      if (weightedFraction == refWeightedFraction) {
         nMax++;
         contentsMax += ((TH1*)fMCs.At(par))->GetBinContent(bin);
      }
   }
   Double_t tmin = -1/refWeightedFraction;

   if (contentsMax == 0) {
      aki = fData->GetBinContent(bin)/(1+refWeightedFraction);
      for (par = 0; par < fNpar; ++par) {
         hw = (TH1*)fWeights.At(par);
         if (par != k0) {
            Double_t weightedFraction = hw ?
              hw->GetBinContent(bin) * fractions[par] : fractions[par];
            if (weightedFraction != refWeightedFraction)
              aki -= ((TH1*)fMCs.At(par))->GetBinContent(bin)*weightedFraction/ (refWeightedFraction - weightedFraction);
         }
      }
      if (aki > 0) {
         if (nMax) aki /= nMax;
         ti = tmin;
         return true;
      }
   }
   k0 = -1;

   // Case of nonzero histogram contents: solve for Ti using Newton's method
   ti = 0;
   const long NLOOPS_MAX = 100000;
   long nloops = 0;
   for (Double_t step = 0.2;;) {
      if (ti >= 1 || ti < tmin) {
         step /= 10;
         ti = 0;
      }
      Double_t aFunction   = - fData->GetBinContent(bin)/(1-ti);
      Double_t aDerivative = aFunction / (1-ti);
      for (par = 0; par < fNpar; ++par) {
         TH1 *h = (TH1*)fMCs.At(par);
         hw = (TH1*)fWeights.At(par);
         Double_t weightedFraction = hw ?
           hw->GetBinContent(bin) * fractions[par] : fractions[par];
         Double_t d = 1/(ti+1/weightedFraction);
         aFunction   += h->GetBinContent(bin)*d;
         aDerivative -= h->GetBinContent(bin)*d*d;
      }
      if (TMath::Abs(aFunction) < 1e-12) break;
      Double_t delta = -aFunction/aDerivative;
      if (TMath::Abs(delta) > step)
         delta = (delta > 0) ? step : -step;
      ti += delta;
      if (TMath::Abs(delta) < 1e-13) break;
      nloops++;
      if(nloops>NLOOPS_MAX) { return false; }
   }

   return true;
}


//______________________________________________________________________________
void TFractionFitFCN2(Int_t& npar, Double_t* gin, Double_t& f, Double_t* par, Int_t flag) {
   // Function called by the minimisation package. The actual functionality is passed
   // on to the TFractionFitter2::ComputeFCN member function.

   TFractionFitter2* fitter = dynamic_cast<TFractionFitter2*>(fractionFitter->GetObjectFit());
   if (!fitter) {
      Error("TFractionFitFCN2","Invalid fit object encountered!");
      return;
   }
   fitter->ComputeFCN(npar, gin, f, par, flag);
}


//______________________________________________________________________________
Double_t TFractionFitter2::GetChisquare() const
{
   // Return the likelihood ratio Chi-squared (chi2) for the fit.
   // The value is computed when the fit is executed successfully.
   // Chi2 calculation is based on the "likelihood ratio" lambda,
   // lambda = L(y;n) / L(m;n),
   // where L(y;n) is the likelihood of the fit result <y> describing the data <n>
   // and L(m;n) is the likelihood of an unknown "true" underlying distribution
   // <m> describing the data <n>. Since <m> is unknown, the data distribution is
   // used instead,
   // lambda = L(y;n) / L(n;n).
   // Note that this ratio is 1 if the fit is perfect. The chi2 value is then
   // computed according to
   // chi2 = -2*ln(lambda).
   // This parameter can be shown to follow a Chi-square distribution. See for
   // example S. Baker and R. Cousins, "Clarification of the use of chi-square
   // and likelihood functions in fits to histograms", Nucl. Instr. Meth. A221, 
   // pp. 437-442 (1984)

   return fChisquare;
}

//______________________________________________________________________________
Int_t TFractionFitter2::GetNDF() const
{
   // return the number of degrees of freedom in the fit
   // the fNDF parameter has been previously computed during a fit.
   // The number of degrees of freedom corresponds to the number of points
   // used in the fit minus the number of templates.

   if (fNDF == 0) return fNpfits-fNpar;
   return fNDF;
}

//______________________________________________________________________________
Double_t TFractionFitter2::GetProb() const
{
   // return the fit probability
   
   Int_t ndf = fNpfits - fNpar;
   if (ndf <= 0) return 0;
   return TMath::Prob(fChisquare,ndf);
}

//______________________________________________________________________________
void TFractionFitter2::ComputeChisquareLambda()
{
   // Method used internally to compute the likelihood ratio chi2
   // See the function GetChisquare() for details

   if ( !fFitDone ) {
      Error("ComputeChisquareLambda","Fit not yet (successfully) performed");
      fChisquare = 0;
      return;
   }

   // fPlot must be initialized and filled. Leave this to the GetPlot() method.
   if (! fPlot) 
     GetPlot();

   Int_t minX, maxX, minY, maxY, minZ, maxZ;
   GetRanges(minX, maxX, minY, maxY, minZ, maxZ);
   
   Double_t logLyn = 0; // likelihood of prediction
   Double_t logLmn = 0; // likelihood of data ("true" distribution)
   for(Int_t x = minX; x <= maxX; x++) {
      for(Int_t y = minY; y <= maxY; y++) {
         for(Int_t z = minZ; z <= maxZ; z++) {
	    if (IsExcluded(fData->GetBin(x, y, z))) continue;
            Double_t di = fData->GetBinContent(x, y, z);
            Double_t fi = fPlot->GetBinContent(x, y, z);
            if(fi != 0) logLyn += di * TMath::Log(fi) - fi;
            if(di != 0) logLmn += di * TMath::Log(di) - di;
            for(Int_t j = 0; j < fNpar; j++) {
               Double_t aji = ((TH1*)fMCs.At(j))->GetBinContent(x, y, z);
               Double_t bji = ((TH1*)fAji.At(j))->GetBinContent(x, y, z);
               if(bji != 0) logLyn += aji * TMath::Log(bji) - bji;
               if(aji != 0) logLmn += aji * TMath::Log(aji) - aji;
            }
         }
      }
   }

   fChisquare = -2*logLyn + 2*logLmn;

   return;
}

//______________________________________________________________________________
TH1* TFractionFitter2::GetMCPrediction(Int_t parm) const
{
   // Return the adjusted MC template (Aji) for template (parm).
   // Note that the (Aji) times fractions only sum to the total prediction
   // of the fit if all weights are 1.

   CheckParNo(parm);
   if ( !fFitDone ) {
      Error("GetMCPrediction","Fit not yet performed");
      return 0;
   }
   return (TH1*) fAji.At(parm);
}

#endif // ROOT_TFractionFitter2
