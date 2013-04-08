
#include <DataGraphics/DgBin.hh>

#include <cassert>
#include <cmath>
#include <vector>
#include <algorithm>
#include <iterator>
#include <string.h>

#include <TH1.h>
#include <TH2.h>

#include <boost/assign/list_of.hpp>
#include <boost/assign/std/vector.hpp>

using namespace std;
using namespace DataGraphics;

DgBin::DgBin() {
  // Fill all std::vector's
#define FILL_VECTORS(x)							\
  x## _intabseta = boost::assign::list_of(0.0)(2.4);			\
  x## _uintabseta = boost::assign::list_of(0.0)(1.0);			\
  x## _abseta = boost::assign::list_of(0.0)(0.21)(0.42)(0.63)(0.84)(1.05)(1.37)(1.52)(1.74)(1.95)(2.18)(2.4); \
  for(unsigned int ii=x## _abseta.size()-1;ii>=1;ii--) x## _eta.push_back(-x## _abseta[ii]); \
  for(unsigned int ii=0;ii<x## _abseta.size();ii++) x## _eta.push_back(x## _abseta[ii]); \
  x## _absetaf = boost::assign::list_of(0)(0.105)(0.21)(0.315)(0.42)(0.525)(0.63)(0.735)(0.84)(0.945)(1.05)(1.21)(1.37)(1.445)(1.52)(1.63)(1.74)(1.845)(1.95)(2.065)(2.18)(2.29)(2.4); \
  for(unsigned int ii=x## _absetaf.size()-1;ii>=1;ii--) x## _etaf.push_back(-x## _absetaf[ii]); \
  for(unsigned int ii=0;ii<x## _absetaf.size();ii++) x## _etaf.push_back(x## _absetaf[ii]); \
  x## _lpt = boost::assign::list_of(20.0)(25.0)(30.0)(35.0)(40.0)(45.0)(50.0)(120.0); \
  x## _wpt = boost::assign::list_of(0.0)(10.0)(20.0)(30.0)(40.0)(50.0)(120.0); \
  x## _wmt = boost::assign::list_of(40.0)(60.0)(80.0)(200.0); \
  x## _met = boost::assign::list_of(25.0)(40.0)(60.0)(200.0); \
  x## _nj  = boost::assign::list_of(0.0)(1.0)(2.0)(3.0)(4.0)
  FILL_VECTORS(D);
#undef FILL_VECTORS

#define FILL_TH1(x)							\
  h1_ ##x = new TH1D( "unf1_" #x, "unf1_" #x, D_ ##x .size()-1,&(D_ ##x [0]) ); \
  h1_ ##x->Sumw2();							\
  h1_ ##x->SetDirectory(0)
#define FILL_TH2(x)							\
  h2_ ##x = new TH2D( "unf2_" #x, "unf2_" #x, D_abseta.size()-1,&(D_abseta[0]), D_ ##x .size()-1,&(D_ ##x [0]) ); \
  h2_ ##x->Sumw2();							\
  h2_ ##x->SetDirectory(0)
#define FILL_TH2_X(x)							\
  h2_x_ ##x = new TH2D( "unf2x_" #x, "unf2x_" #x, D_ ##x .size()-1,&(D_ ##x [0]) , D_ ##x .size()-1,&(D_ ##x [0]) ); \
  h2_x_ ##x->Sumw2();							\
  h2_x_ ##x->SetDirectory(0)
  //1D
  FILL_TH1(intabseta);  // used for integrated (0D) measurement
  FILL_TH1(uintabseta); // used for integrated (0D) measurement
  FILL_TH1(abseta);
  FILL_TH1(absetaf);
  FILL_TH1(eta);
  FILL_TH1(etaf);
  FILL_TH1(lpt);
  FILL_TH1(wpt);
  FILL_TH1(wmt);
  FILL_TH1(met);
  FILL_TH1(nj);
  //2D
  FILL_TH2(lpt);
  FILL_TH2(wpt);
  FILL_TH2(wmt);
  FILL_TH2(met);
  FILL_TH2(nj);
  //2D special
  FILL_TH2_X(lpt);
#undef FILL_TH1
#undef FILL_TH2
#undef FILL_TH2_X

  // manually fill additional special binnings
  for(int ii=0;ii<=120;ii++) {
    D_pt.push_back(ii*1.0); //fine-binned pt
    D_lphi.push_back( -3.15 + ii/120.0*(3.15+3.15) );
    D_leta.push_back( -2.4 + ii/120.0*(2.4+2.4) );
  }
}

DgBin::~DgBin() {
  delete h1_intabseta;
  delete h1_uintabseta;
  delete h1_abseta;
  delete h1_absetaf;
  delete h1_eta;
  delete h1_etaf;
  // 1d versions (to be used in conjunction with manual |eta| binning)
  delete h1_lpt;
  delete h1_wpt;
  delete h1_wmt;
  delete h1_met;
  delete  h1_nj;
  // 2d versions
  delete h2_lpt;
  delete h2_wpt;
  delete h2_wmt;
  delete h2_met;
  delete h2_nj;
  // special histograms
  delete h2_x_lpt; // lpt x lpt
}
