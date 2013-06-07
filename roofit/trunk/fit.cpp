#include <TFile.h>
#include <TH1.h>
#include "VFitter.h"
#include<TObjArray.h>
#include<TFractionFitter.h>

#include<cassert>
#include<iostream>

int main() {

  TFile *f = TFile::Open("SYS.root","READ");
  TH1 *data = static_cast<TH1*>(f->Get("data"));  assert(data);
  TH1 *ewk = static_cast<TH1*>(f->Get("ewk"));  assert(ewk);
  TH1 *qcd = static_cast<TH1*>(f->Get("qcd"));  assert(qcd);
  TObjArray *mc = new TObjArray(2);
  mc->Add(ewk);
  mc->Add(qcd);

  VFitter *fit = new VFitter(data,mc);
  //TFractionFitter *fit = new TFractionFitter(data,mc);
  const int fitmin=1, fitmax=31;
  const double ewkfrac = 0.76333228808940889465;
  const double qcdfrac = 0.23666771191059110535;
  fit->SetRangeX(fitmin,fitmax);
  assert(fit->GetFitter());
  fit->GetFitter()->SetParameter(0,"ewkfrac",ewkfrac,0.01,0.0,1.0);
  fit->GetFitter()->SetParameter(1,"qcdfrac",qcdfrac,0.01,0.0,1.0);
  int status = fit->Fit();
  std::cout << "status = " << status << std::endl;
  double value,error;
  fit->GetResult(0, value, error);
  std::cout << "value = " << value << " error = " << error << std::endl;
  return 0;
}
