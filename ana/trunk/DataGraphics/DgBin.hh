#ifndef WRAP_DGBIN_HH
#define WRAP_DGBIN_HH

#include <DataGraphics/DgTypes.hh>
#include <vector>
#include <string>
#include <cassert>

#include <TH1F.h>
#include <TH2D.h>

// This class is NOT serialized. It is resident in-memory

namespace
DataGraphics
{

  // A class that will store kinematic slice binning
  // all members are public - this is just a container
  class DgBin {
  public:
    typedef enum { INTABSETA,UINTABSETA,ABSETA,ABSETAF,ETA,ETAF,LPT,WPT,WMT,MET,NJ } BinType;
  public:
    // bins with doubles
    std::vector<double> D_intabseta;
    std::vector<double> D_uintabseta;
    std::vector<double> D_abseta;
    std::vector<double> D_absetaf; // fine |eta| binning (by a factor of 2)
    std::vector<double> D_eta;
    std::vector<double> D_etaf; // fine eta binning (by a factor of 2)
    std::vector<double> D_lpt;
    std::vector<double> D_wpt;
    std::vector<double> D_wmt;
    std::vector<double> D_met;
    std::vector<double> D_nj;

    // fine bins
    std::vector<double> D_pt;
    std::vector<double> D_lphi;
    std::vector<double> D_leta;

    // abseta
    TH1D* h1_intabseta;
    TH1D* h1_uintabseta;
    TH1D* h1_abseta;
    TH1D* h1_absetaf;
    TH1D* h1_eta;
    TH1D* h1_etaf;
    // 1d versions (to be used in conjunction with manual |eta| binning)
    TH1D* h1_lpt;
    TH1D* h1_wpt;
    TH1D* h1_wmt;
    TH1D* h1_met;
    TH1D* h1_nj;
    // 2d versions
    TH2D* h2_lpt;
    TH2D* h2_wpt;
    TH2D* h2_wmt;
    TH2D* h2_met;
    TH2D* h2_nj;
    // special histograms
    TH2D* h2_x_lpt; // lpt x lpt

  public:
    
    TH1D* H1(const BinType& t) {
      switch(t) {
      case INTABSETA: return h1_intabseta; break;
      case UINTABSETA: return h1_uintabseta; break;
      case ABSETA: return h1_abseta; break;
      case ABSETAF: return h1_absetaf; break;
      case ETA: return h1_eta; break;
      case ETAF: return h1_etaf; break;
      case LPT: return h1_lpt; break;
      case WPT: return h1_wpt; break;
      case WMT: return h1_wmt; break;
      case MET: return h1_met; break;
      case NJ: return h1_nj; break;
      }
      return 0;
    }
    
    DgBin();
    ~DgBin();
  };

};

#endif //WRAP_DGBIN_HH
