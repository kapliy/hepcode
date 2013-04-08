#include <iostream>
#include "DataGraphics/DgEfficiency0D.hh"
#include "DataGraphics/DgEfficiency1D.hh"
#include "DataGraphics/DgGraph2D.hh"
#include "DataGraphics/DgHistogram1D.hh"
#include "DataGraphics/DgHistogram2D.hh"
#include "DataGraphics/DgHistogramWeighted1D.hh"
#include "DataGraphics/DgProfile1D.hh"

using namespace std;
using namespace DataGraphics;

// template<typename archiveT>
// void 
// DgAbsGraphic::serialize( archiveT& ar , const unsigned int version )
// {
//   DgEfficiency0D* eff0d = dynamic_cast<DgEfficiency0D*>(this);
//   DgEfficiency1D* eff1d = dynamic_cast<DgEfficiency1D*>(this);
//   DgGraph2D* gr2d = dynamic_cast<DgGraph2D*>(this);
//   DgHistogram1D* h1d = dynamic_cast<DgHistogram1D*>(this);
//   DgHistogram2D* h2d = dynamic_cast<DgHistogram2D*>(this);
//   DgHistogramWeighted1D* hw1d = dynamic_cast<DgHistogramWeighted1D*>(this);
//   DgProfile1D* p1d = dynamic_cast<DgProfile1D*>(this);
//   if( h1d ) {
//     ar & h1d;
//   } else if( h2d ) {
//     ar & h2d;
//   } else if( hw1d ) {
//     ar & hw1d;
//   } else if( eff0d ) {
//     ar & eff0d;
//   } else if( eff1d ) {
//     ar & eff1d;
//   } else if( gr2d ) {
//     ar & gr2d;
//   } else if( p1d ) {
//     ar & p1d;
//   } else {
//     cout << "unrecognized graphic type! cannot save." << endl;
//   }
// }
