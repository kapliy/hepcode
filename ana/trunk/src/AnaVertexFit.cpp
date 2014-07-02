
#include <iterator>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/scope_exit.hpp>
#include <iostream>
#include <TMath.h>
#include "TrigFTKAna_config.h"
#include "TrigFTKAna/AnaTrack.hpp"
#include "TrigFTKAna/ctvmft/CtvmftDimensions.h"
#include "TrigFTKAna/AnaVertexFit.hpp"

#include "DataGraphics/DgCollection.hh"

using namespace std;
using namespace DataGraphics;

extern "C" void ctvmft_(int&,int&,int&);
extern "C" float prob_(float&,int&);
extern "C" bool mcalc_(int&,int*,float&,float&,double*);
extern "C" void dcalc_(int&,int&,float*,float&,float&,float*);

// Fortran routines to get address of the start of the
// ctvmq and ctvmfr common blocks
#ifdef HAVE_GFORTRAN
// gfortran name mangling
#define ctvmq_address_ ctvmq_address_
#define ctvmfr_address_ ctvmfr_address_
#define fiddle_address_ fiddle_address_
#define trkprm_address_ trkprm_address_
#else
// f77-like name mangling
#define ctvmq_address_ ctvmq_address__
#define ctvmfr_address_ ctvmfr_address__
#define fiddle_address_ fiddle_address__
#define trkprm_address_ trkprm_address__
#endif
extern "C" {
   int ctvmq_address_(void);
   int ctvmfr_address_(void);
   int fiddle_address_(void);
   int trkprm_address_(void);
}
extern "C" void* address_(void* x) {
  return x;
}

static const int _maxvtx = CTVMFT_MAXVTX;//  6;  // Maximum number of vertices
static const int _maxmcn = CTVMFT_MAXMCN;//  4;  // Maximum number of mass constraints
static const int _maxtrk = CTVMFT_MAXTRK;// 50;  // Maximum number of tracks
static const int _maxitr = CTVMFT_MAXITR;// 10;  // Maximum number of iteration steps
static const int _maxdim = 5*(_maxvtx+1)+3*_maxtrk+_maxmcn;

struct
CTVMQ
{
  int runnum;
  int trgnum;
  int iter;
  int ntscut;
  int nvertx;
  int nmassc;
  int ntrack;
  int trkvtx[_maxvtx][_maxtrk];  // This is actually a logical variable in FORTRAN, but is integer here to get the size right.
  int trkmcn[_maxmcn][_maxtrk];  // This is actually a logical variable in FORTRAN, but is integer here to get the size right.
  int vtxpnt[2][_maxvtx];
  float cmass[_maxmcn];
  int cvtx[_maxvtx];
  int vtxvtx[_maxvtx][_maxvtx];  // This is actually a logical variable in FORTRAN, but is integer here to get the size right.
  char tkbank[_maxtrk][4];
  int list[_maxtrk];
  float tmass[_maxtrk];
  int matdim;
  int tkerr[_maxtrk];
  int ndof;
  float chisqr[_maxitr+1];
  float chit[_maxtrk];
  float chiv[_maxvtx+1];
  float chim[_maxmcn];
  float xyzpv0[3];
  float exyzpv[3][3];
  float xzslope;
  float yzslope;
  float xyzvrt[_maxvtx+1][3];
  float dxyzpv[3];
  float par[_maxtrk][5];
  float g[_maxtrk][5][5];
  float trkp4[6][_maxtrk];
  float vtxp4[_maxvtx][4];
  float mcnp4[_maxmcn][4];
  float dda[8][_maxtrk];
  int voff[_maxvtx];
  int toff[_maxtrk];
  int poff[_maxvtx];
  int coff[_maxvtx];
  int moff;
  float par0[_maxtrk][5];
  float pardif[_maxtrk][5];
  float fmcdif[_maxmcn];
  float pcon[2][_maxvtx];
  float sang[2][_maxvtx];
  float drmax;
  float rvmax;
  float dzmax;
  float trnmax;
  float dsmin;
  int ijkerr[3];
  float pscale;
};

struct
CTVMFR
{
  double vmat[_maxdim+1][_maxdim];
};

struct
FIDDLE
{
  int excuse;
  float chisqmax;
};

struct TRKPRM {
  float trhelix[_maxtrk][5];
  float trem[_maxtrk][5][5];
};

const bool
AnaVertexFit::fit()
{
  _complete_fit = false;
  // count number of tracks to fit. if this number is less than two, the fit will fail.
  unsigned int ntracks_to_fit = std::count( _track_statuses.begin() , _track_statuses.end() , true );
  if( ntracks_to_fit<2 ) { return false; }

  static CTVMQ* ctvmq = (CTVMQ*) ctvmq_address_();
  static CTVMFR* ctvmfr = (CTVMFR*) ctvmfr_address_();
  static FIDDLE* fiddle = (FIDDLE*) fiddle_address_();
  static TRKPRM* trkprm = (TRKPRM*) trkprm_address_();
  fiddle->excuse=1;
  ctvmq->runnum=1;
  ctvmq->trgnum=100;
  // bmag in Tesla
   const AnaVertexFit::ftype BFieldTesla = 2.; // FIXME
  ctvmq->pscale=0.00149896*BFieldTesla;
  // set the default maximum chi-square per dof.
  fiddle->chisqmax = 10000.; //225.;
  // We also need to get the primary vertex from the right place, but
  // for now we put in (0,0,0).
   ctvmq->xyzpv0[0] = 0;
   ctvmq->xyzpv0[1] = 0;
   ctvmq->xyzpv0[2] = 0;
   // Set the error matrix for the primary vertex.
   for(int j=0; j<3; ++j) {
     for(int k=0; k<3; ++k) {
       ctvmq->exyzpv[j][k] = 0;
     }
   }
   ctvmq->exyzpv[0][0] = 0.005;
   ctvmq->exyzpv[1][1] = 0.005;
   ctvmq->exyzpv[2][2] = 6.;
   // zero number of tracks, vertices and mass constraints.
   ctvmq->ntrack = 0;
   ctvmq->nvertx = 0;
   ctvmq->nmassc = 0;
   // zero track list and arrays containing the vertex and mass
   // constraint configuration.
   for( int j=0; j!=_maxtrk; ++j ) {
     ctvmq->list[j] = 0;
     for( int jv=0; jv!=_maxvtx; ++jv ) {
       ctvmq->trkvtx[jv][j]=false;
     }
     for( int jmc=0; jmc!=_maxmcn; ++jmc ) {
       ctvmq->trkmcn[jmc][j]=false;
     }
   }
   // initialize the conversion and vertex pointing arrays.
   for( int jv=0; jv!=_maxvtx; ++jv ) {
     ctvmq->cvtx[jv]=0;
     ctvmq->vtxpnt[0][jv]=-1;
     ctvmq->vtxpnt[1][jv]=0;
   }
   ctvmq->drmax = 2.;
   ctvmq->dzmax = 20.;
   ctvmq->rvmax = 70.;
   ctvmq->trnmax = 0.5;
   ctvmq->dsmin = -2.;
   // set stat to -999  and chisqq to -1 to signify that no fit has yet been done.
   ctvmq->chisqr[0] = -1.;
   // set the common block track info
   int ivert = 0;
   int ntrk = 0;
   TrackStatusColl::const_iterator istat=_track_statuses.begin();
   for( TrackColl::const_iterator itrk=_tracks.begin(), ftrk=_tracks.end(); itrk!=ftrk; ++itrk,++istat ) {
     _add( *itrk , ivert , ntrk++ );
   }
   ctvmq->ntrack = ntrk;
   ctvmq->nvertx = 1; // not // primary + 1 secondary

   // do the fit
   int print = 0;//1;
   int level = 0;//999
   int stat = -999;
   ctvmft_(print,level,stat);
   const bool fit_ok = (stat==0);
   if( false ) { 
     cout << " FIT STATUS: " << (fit_ok ? "OK" : "FAILED") << endl;
     _print( stat , cout );
     // if( !fit_ok ) {
     //   _print_error( cout );
     // }
   }
   if( fit_ok ) { // retrieve results
     const int ivertoffset = ctvmq->poff[ivert-1];
     _x = ctvmq->xyzvrt[ivert+1][0];
     _y = ctvmq->xyzvrt[ivert+1][1];
     _z = ctvmq->xyzvrt[ivert+1][2];
     _cov_xx = ctvmfr->vmat[ivertoffset][ivertoffset];
     _cov_xy = ctvmfr->vmat[ivertoffset][ivertoffset+1];
     _cov_xz = ctvmfr->vmat[ivertoffset][ivertoffset+2];
     _cov_yy = ctvmfr->vmat[ivertoffset+1][ivertoffset+1];
     _cov_yz = ctvmfr->vmat[ivertoffset+1][ivertoffset+2];
     _cov_zz = ctvmfr->vmat[ivertoffset+2][ivertoffset+2];
     _chi2 = ctvmq->chiv[ivert+1];
     _ndof = ctvmq->ndof;
     unsigned int ntrk = 0;
     _momenta.clear();
     _delta_chi2s.clear();
     _sum_momenta.SetPxPyPzE( 0 , 0 , 0 , 0 );
     for( TrackColl::const_iterator itrk=_tracks.begin(), ftrk=_tracks.end(); itrk!=ftrk; ++itrk, ++ntrk ) {
       const ftype px( ctvmq->trkp4[0][ntrk] );
       const ftype py( ctvmq->trkp4[1][ntrk] );
       const ftype pz( ctvmq->trkp4[2][ntrk] );
       const ftype en( ctvmq->trkp4[3][ntrk] );
       _momenta.push_back( TLorentzVector( px , py , pz , en) );
       _sum_momenta.SetPx( _sum_momenta.Px() + px );
       _sum_momenta.SetPy( _sum_momenta.Py() + py );
       _sum_momenta.SetPz( _sum_momenta.Pz() + pz );
       _sum_momenta.SetE( _sum_momenta.E() + en );
       // double charge = (ctvmft.getPar(indexTrk,0) <= 0) ? 1 : -1; 
       _delta_chi2s.push_back( ctvmq->chit[ntrk] );
     }
     assert( _momenta.size() == ntracks() );
     assert( _delta_chi2s.size() == ntracks() );
     _complete_fit = true;
   }
   return _complete_fit;
}


void
AnaVertexFit::_add( const boost::shared_ptr<const AnaTrack>& track , const int& ivert , const int& itrk ) // ivert=1
{
  const bool debug = false;
  static CTVMQ* ctvmq = (CTVMQ*) ctvmq_address_();
  static CTVMFR* ctvmfr = (CTVMFR*) ctvmfr_address_();
  static FIDDLE* fiddle = (FIDDLE*) fiddle_address_();
  static TRKPRM* trkprm = (TRKPRM*) trkprm_address_();
  if( itrk >= _maxtrk ) { 
    if( debug ) { cout << " maximum number of tracks for CTVMFT fit reached" << endl; }
    return; 
  }

  ctvmq->list[itrk] = itrk+1; // fortran counting id from 1 to n (id=0 means not present/invalid in this context)
  // parameter basis: 0: curv
  //                  1: phi0
  //                  2: cotTheta
  //                  3: d0
  //                  4: z0
  ctvmq->tkbank[itrk][0] = 'Q';
  ctvmq->tkbank[itrk][1] = 'T';
  ctvmq->tkbank[itrk][2] = 'R';
  ctvmq->tkbank[itrk][3] = 'K';
  ctvmq->tmass[itrk] = 0.13957018; // pion mass
  ctvmq->trkvtx[ivert][itrk] = true; // FIXME?

  // Put this track's helix parameters and error matrix into a
  // fortran common block so that they can be accessed by CTVMFT.

  const unsigned int ict = 0;
  const unsigned int icu = 1;
  const unsigned int iz0 = 2;
  const unsigned int id0 = 3;
  const unsigned int ip0 = 4;

  // radius(cm) = pt(GeV) / (const*B) => const is in GeV/(T*cm)
  // 0.5/curv = pt / (const*B)
  // curv = 0.5 const*B / pt
  const AnaVertexFit::ftype BFieldTesla = 2.; // FIXME
  AnaVertexFit::ftype CurvatureConstant = 0.0029979; // GeV/(T*cm)
  const AnaVertexFit::ftype facInvGeVtoInvCm = 0.5 * CurvatureConstant * BFieldTesla;

  trkprm->trhelix[itrk][icu] = facInvGeVtoInvCm*track->curvature()*1000.; // convert from 1/MeV to 1/cm.
  trkprm->trhelix[itrk][ip0] = track->phi0();
  trkprm->trhelix[itrk][ict] = track->lambda();
  trkprm->trhelix[itrk][id0] = track->d0()/10.;
  trkprm->trhelix[itrk][iz0] = track->z0()/10.;
  // covariance matrix
  trkprm->trem[itrk][icu][icu] = track->covariance_cc() * detector::pow<2>(facInvGeVtoInvCm*1000.);
  trkprm->trem[itrk][icu][id0] = trkprm->trem[itrk][id0][icu] = track->covariance_cd() * facInvGeVtoInvCm * 1000. * 0.1;
  trkprm->trem[itrk][icu][iz0] = trkprm->trem[itrk][iz0][icu] = track->covariance_cz() * facInvGeVtoInvCm * 1000. * 0.1;
  trkprm->trem[itrk][icu][ip0] = trkprm->trem[itrk][ip0][icu] = track->covariance_cp() * facInvGeVtoInvCm * 1000.;
  trkprm->trem[itrk][icu][ict] = trkprm->trem[itrk][ict][icu] = track->covariance_ct() * facInvGeVtoInvCm * 1000.;
  trkprm->trem[itrk][id0][id0] = track->covariance_dd() * detector::pow<2>(0.1);
  trkprm->trem[itrk][id0][iz0] = trkprm->trem[itrk][iz0][id0] = track->covariance_dz() * 0.1 * 0.1;
  trkprm->trem[itrk][id0][ip0] = trkprm->trem[itrk][ip0][id0] = track->covariance_dp() * 0.1;
  trkprm->trem[itrk][id0][ict] = trkprm->trem[itrk][ict][id0] = track->covariance_dt() * 0.1;
  trkprm->trem[itrk][iz0][iz0] = track->covariance_zz() * 0.1 * 0.1;
  trkprm->trem[itrk][iz0][ip0] = trkprm->trem[itrk][ip0][iz0] = track->covariance_pz() * 0.1;
  trkprm->trem[itrk][iz0][ict] = trkprm->trem[itrk][ict][iz0] = track->covariance_zt() * 0.1;
  trkprm->trem[itrk][ip0][ip0] = track->covariance_pp();
  trkprm->trem[itrk][ip0][ict] = trkprm->trem[itrk][ict][ip0] = track->covariance_pt();
  trkprm->trem[itrk][ict][ict] = track->covariance_tt();
  
  if( debug ) {
    using boost::format;
    cout << " Added track " << itrk << " with parameters: " << endl
         << format( "cu: %|9f| d0: %|7f| z0: %|7f| p0: %|7f| ct:%|7f|") 
      % trkprm->trhelix[itrk][icu] 
      % trkprm->trhelix[itrk][id0] 
      % trkprm->trhelix[itrk][iz0]
      % trkprm->trhelix[itrk][ip0]
      % trkprm->trhelix[itrk][ict]
         << endl
         << " truth barcode: " << track->barcode().barcode() << " " << track->barcode().event_index() << " " << track->barcode_matching_fraction() 
         << endl
      //    << format( "cu: %|9f| d0: %|7f| z0: %|7f| p0: %|7f| ct:%|7f|") 
      // % track->curvature()
      // % track->d0()
      // % track->z0()
      // % track->phi0()
      // % track->lambda()
      //    << endl
         << format( "errors: cu %|9f| d0: %|7f| z0: %|7f| p0: %|7f| ct:%|7f|") 
      % std::sqrt(trkprm->trem[itrk][icu][icu])
      % std::sqrt(trkprm->trem[itrk][id0][id0])
      % std::sqrt(trkprm->trem[itrk][iz0][iz0])
      % std::sqrt(trkprm->trem[itrk][ip0][ip0])
      % std::sqrt(trkprm->trem[itrk][ict][ict])
         << endl
      //    << format( "errors: cu %|9f| d0: %|7f| z0: %|7f| p0: %|7f| ct:%|7f|") 
      // % track->curv_err()
      // % track->d0_err()
      // % track->z0_err()
      // % track->phi0_err()
      // % track->lambda_err()
         << endl;
  }

}


void
AnaVertexFit::_print( const int& stat , std::ostream& os ) const
{
  static CTVMQ* ctvmq = (CTVMQ*) ctvmq_address_();
  static CTVMFR* ctvmfr = (CTVMFR*) ctvmfr_address_();
  static FIDDLE* fiddle = (FIDDLE*) fiddle_address_();
  static TRKPRM* trkprm = (TRKPRM*) trkprm_address_();

  os << "****************************** AnaVertexFit "
     << "******************************" << std::endl;
  os << "Number of tracks: " << ctvmq->ntrack << std::endl;
  os << "   Tracks: ";
  for(int jt=0; jt<ctvmq->ntrack; ++jt) {
    if(jt != 0) os << ",  ";
    cout << (ctvmq->list[jt]-1);
  }
  os << std::endl;
  os << "Number of vertices: " << ctvmq->nvertx << std::endl;
  for(int jv=0; jv<ctvmq->nvertx; ++jv) {
    os << "   Vertex " << jv+1 << " tracks: ";
    for(int jt=0; jt<ctvmq->ntrack; ++jt) {
      if(ctvmq->trkvtx[jv][jt]) {
        os << " " << (ctvmq->list[jt]-1);
      }
    }
    os << std::endl;
  }
  for(int jv=0; jv<ctvmq->nvertx; ++jv) {
    if(ctvmq->vtxpnt[0][jv]==0) {
      os << "   Vertex " << jv+1 << " points to the primary vertex ";
    }
    else if(ctvmq->vtxpnt[0][jv]>0) {
      os << "   Vertex " << jv+1 << " points to vertex "
         << ctvmq->vtxpnt[0][jv];
    }
    if(ctvmq->vtxpnt[1][jv]==1) {
      os << " in 2 dimensions" << std::endl;
    }
    else if(ctvmq->vtxpnt[1][jv]==2) {
      os << " in 3 dimensions" << std::endl;
    }
    else if(ctvmq->vtxpnt[1][jv]==3) {
      os << ", a single track vertex" << std::endl;
    }
    if(ctvmq->cvtx[jv]>0) {
      os << "   Vertex " << jv+1 << " is a conversion" << std::endl;
    }
  }
  os << "Number of mass constraints: " << ctvmq->nmassc << std::endl;
  for(int jmc=0; jmc<ctvmq->nmassc; ++jmc) {
    os << "   Tracks ";
    for(int jt=0; jt<ctvmq->ntrack; ++jt) {
      if(ctvmq->trkmcn[jmc][jt]) {
        os << " " << (ctvmq->list[jt]-1);
      }
    }
    os << " constrained to mass " << ctvmq->cmass[jmc]
       << " Gev/c^2" << std::endl;
  }
  if(stat==-999) {
    os << "No fit has been done." << std::endl;
  }
  else {
    os << "***** Results of Fit *****" << std::endl;
    _print_error( stat , os );
    os << "   Status = " << stat << std::endl;
    os << "   Chi-square = " << ctvmq->chisqr[0]
       << " for " << ctvmq->ndof << " degrees of freedom." << std::endl;
    os << "   => probability = " << TMath::Prob(ctvmq->chisqr[0],ctvmq->ndof) << std::endl;
    for(int jv=0; jv<ctvmq->nvertx; ++jv) {
      os << "Vertex " << jv+1 << " position: "
         << ctvmq->xyzvrt[jv+1][0] << " "
         << ctvmq->xyzvrt[jv+1][1] << " "
         << ctvmq->xyzvrt[jv+1][2] << std::endl;
    }
    for(int jt=0; jt<ctvmq->ntrack; ++jt) {
      os << "Track " << (ctvmq->list[jt]-1) << " P4: " 
         << ctvmq->trkp4[0][jt] << " "
         << ctvmq->trkp4[1][jt] << " "
         << ctvmq->trkp4[2][jt] << " "
         << ctvmq->trkp4[3][jt] << " " << std::endl;
    }
  }
  os << "****************************************"
     << "**************************" << std::endl;
}

void
AnaVertexFit::_print_error( const int& stat , std::ostream& os ) const
{
  static CTVMQ* ctvmq = (CTVMQ*) ctvmq_address_();
  static CTVMFR* ctvmfr = (CTVMFR*) ctvmfr_address_();
  static FIDDLE* fiddle = (FIDDLE*) fiddle_address_();
  static TRKPRM* trkprm = (TRKPRM*) trkprm_address_();
  os << "AnaVertexFit: status = " << stat << endl;
  os << "AnaVertexFit: IJKERR = " << ctvmq->ijkerr[0] << ", "
     << ctvmq->ijkerr[1] << ", "
     << ctvmq->ijkerr[2] << std::endl;
  if(stat==0 && ctvmq->ijkerr[0]==0) return;
  if(ctvmq->ijkerr[0] == -1) {
    os << "   Problem with GETTRK:  track requested is not in list."
       << std::endl
       << "   This should not happen!" << std::endl;
  }
  else if(ctvmq->ijkerr[0]==1) {
    os << "   Problem in CTVM00:" << std::endl;
    if(ctvmq->ijkerr[1]==1) {
      os << "      Number of tracks is " << ctvmq->ntrack
         << "." << std::endl;
      if(ctvmq->ntrack < 2) {
	    os << ", which is too few (must be at least 2)." << std::endl;
      }
      else if(ctvmq->ntrack > _maxtrk) {
	    os << ", which is too many (maximum is " << _maxtrk
	       << ")." << std::endl;
      }
      else {
	    os << "      Problem with number of tracks"
	       << " for unknown reasons." << std::endl;
      }
    }
    else if(ctvmq->ijkerr[1]==2) {
      os << "      Number of vertices is " << ctvmq->nvertx
         << "." << std::endl;
      if(ctvmq->nvertx < 1) {
	    os << ", which is too few (must be at least 1)." << std::endl;
      }
      else if(ctvmq->nvertx > _maxvtx) {
	    os << ", which is too many (maximum is " << _maxvtx
	       << ")." << std::endl;
      }
      else {
	    os << std::endl << "      Problem with number of vertices"
	       << " for unknown reasons." << std::endl;
      }
    }
    else if(ctvmq->ijkerr[1]==3) {
      os << "      Number of mass constraints is " << ctvmq->nmassc
         << "." << std::endl;
      if(ctvmq->nmassc < 0) {
	    os << ", which is negative." << std::endl;
      }
      else if(ctvmq->nmassc > _maxmcn) {
	    os << ", which is too many (maximum is " << _maxmcn
	       << ")." << std::endl;
      }
      else {
	    os << std::endl << "      Problem with number of mass"
	       << " constraints for unknown reasons." << std::endl;
      }
    }
    else if(ctvmq->ijkerr[1]==11) {
      os << "      Vertex " << ctvmq->ijkerr[2]
         << " has less than one track." << std::endl;
    }
    else if(ctvmq->ijkerr[1]==12) {
      os << "      Vertex " << ctvmq->ijkerr[2]
         << " is a conversion vertex with a number of tracks"
         << " different than two." << std::endl;
    }
    else if(ctvmq->ijkerr[1]==13) {
      os << "      Vertex " << ctvmq->ijkerr[2]
         << " is a one track vertex that has no multi-track"
         << " descendents." << std::endl;
    }
    else if(ctvmq->ijkerr[1]==14) {
      os << "      Vertex " << ctvmq->ijkerr[2]
         << " does not point at a vertex with a lower number."
         << std::endl;
    }
    else if(ctvmq->ijkerr[1]==15) {
      os << "      Vertex " << ctvmq->ijkerr[2]
         << " has a parent vertex that is a conversion." << std::endl;
    }
    else if(ctvmq->ijkerr[1]==16) {
      os << "      Vertex " << ctvmq->ijkerr[2]
         << " does 1 track pointing to a vertex with"
         << " more than 1 track." << std::endl;
    }
    else if(ctvmq->ijkerr[1]==17) {
      os << "      Vertex " << ctvmq->ijkerr[2]
         << " does 0 track pointing to a vertex with"
         << " more than 0 track (?)." << std::endl;
    }
    else if(ctvmq->ijkerr[1]==19) {
      os << "      Primary vertex error matrix is singular." << std::endl;
    }
    else if(ctvmq->ijkerr[1]==20) {
      os << "      Track is not specified correctly in bank." << std::endl;
    }
    else if(ctvmq->ijkerr[1]==21) {
      os << "      Track with Id " << ctvmq->ijkerr[2]
         << "is not in any vertex." << std::endl;
    }
    else if(ctvmq->ijkerr[1]==22) {
      os << "      Track with Id " << ctvmq->ijkerr[2]
         << "is in multiple vertices." << std::endl;
    }
    else if(ctvmq->ijkerr[1]==23) {
      os << "      Track with Id " << ctvmq->ijkerr[2]
         << "occurs more than once." << std::endl;
    }
    else if(ctvmq->ijkerr[1]==31) {
      os << "      A mass constraint has less than 2 tracks." << std::endl;
    }
    else if(ctvmq->ijkerr[1]==32) {
      os << "      The sum masses of the tracks in a mass constraint"
         << " exceeds the constraint mass." << std::endl;
    }
    else if(ctvmq->ijkerr[1]==33) {
      os << "      Beamline constraint. Beam covariance not set properly."
         << " Negative diagonal elements." << std::endl;
    }
    else if(ctvmq->ijkerr[1]==34) {
      os << "      Beamline constraint. Beam covariance not set properly."
         << " Off-diagonal elements not zero." << std::endl;
    }
    else if(ctvmq->ijkerr[1]==36) {
      os << "      Beamline constraint. Number of vertices = " 
         << ctvmq->nvertx << " Should be 1." << std::endl;
    }
  }
  else if(ctvmq->ijkerr[0] == 2) {
    if(ctvmq->ijkerr[1] == 20) {
      os << "   Problem in CTVM00: " << std::endl;
      os << "      Track has negative Id = "
         << (ctvmq->list[ctvmq->ijkerr[2]-1]-1) << "." << std::endl;
    }
    else {
      os << "   Problem in CTVMFA with vertex "
         << ctvmq->ijkerr[2] << ": " << std::endl;
      os << "      Failure in vertex first approximation." << std::endl;
      if(ctvmq->ijkerr[1] == 1) {
        os << "      Tracks are concentric circles." << std::endl;
      }
      if(ctvmq->ijkerr[1] == 2) {
        os << "      Conversion vertex has widely separated"
           << " exterior circles at midpoint." << std::endl;
      }
      if(ctvmq->ijkerr[1] == 3) {
	    os << "      Conversion vertex has widely separated"
	       << " interior circles at midpoint." << std::endl;
      }
      if(ctvmq->ijkerr[1] == 4) {
	    os << "      Vertex has widely separated"
	       << " exterior circles at approximate vertex." << std::endl;
      }
      if(ctvmq->ijkerr[1] == 5) {
	    os << "      Vertex has widely separated"
	       << " interior circles at approximate vertex." << std::endl;
      }
      if(ctvmq->ijkerr[1] == 6) {
	    os << "      Rv is too large at the chosen"
	       << " intersection point." << std::endl;
      }
      if(ctvmq->ijkerr[1] == 7) {
	    os << "      Delta z is too large at the chosen"
	       << " intersection point." << std::endl;
      }
      if(ctvmq->ijkerr[1] == 8) {
	    os << "      A track's turning to the chosen vertex"
	       << " is too large." << std::endl;
      }
      if(ctvmq->ijkerr[1] == 9) {
	    os << "      There is no solution with an adequately"
	       << " positive arc length." << std::endl;
      }
      if(ctvmq->ijkerr[1] == 21) {
	    os << "      zero-track vertexing: either/both vertex "
	       << " momenta are too small (<0.01 MeV)." << std::endl;
      }
      if(ctvmq->ijkerr[1] == 22) {
	    os << "      zero-track vertexing: Two lines (tracks) are "
	       << " parallel/antiparallel." << std::endl;
      }

    }
  }
  else if(ctvmq->ijkerr[0] == 3) {
    os << "   Problem in CTVM01 with track with Id = "
       << (ctvmq->list[ctvmq->ijkerr[2]-1]-1) << ": " << std::endl;
    if(ctvmq->ijkerr[1] == 1) {
      os << "      GETTRK cannot find Id in list." << std::endl;
    }
    if(ctvmq->ijkerr[1] == 2) {
      os << "      Covariance matrix could not be inverted."  << endl 
         << "      Offending track number (in order addded) is "
         << ctvmq->ijkerr[2] << "." << std::endl;
    }
    if(ctvmq->ijkerr[1] == 3) {
      os << "      Track turns through too large an angle"
         << " to the vertex." << std::endl;
    }
    if(ctvmq->ijkerr[1] == 4) {
      os << "      Track moves too far backward to vertex." << std::endl;
    }
    if(ctvmq->ijkerr[1] == 5) {
      os << "      Track with curvature > 0.01." << std::endl
         << "      Offending track number is "
         << ctvmq->ijkerr[2] << "." << std::endl;
    }
  }
  else if(stat == 9) {
    os << "   General fit problem: " << std::endl;
    if(ctvmq->ijkerr[1] == 1) {
      os << "      Singular solution matrix." << std::endl;
    }
    if(ctvmq->ijkerr[1] == 2 || ctvmq->ijkerr[1] == 3) {
      os << "      Too many iterations ( "
         << ctvmq->ijkerr[2] << "(." << std::endl;
    }
    if(ctvmq->ijkerr[1] == 4) {
      os << "      Convergence failure." << std::endl;
    }
    if(ctvmq->ijkerr[1] == 5) {
      os << "      Bad convergence." << std::endl;
    }
    if(ctvmq->ijkerr[1] == 9) {
      os << "      Ill-formed  covariance matrix." << std::endl;
    }
  }
  else {
    os << "   The error codes above are not recognized." << std::endl
       << "   Contact AnaVertexFit expert!" << std::endl;
  }
  return;
}


const AnaVertexFit::ftype
AnaVertexFit::perp_significance( const TVector& pv , const TMatrix& cov_pv ) const
{
  assert_fitted();
  TVector dx = pv;
  dx(0) -= _x;
  dx(1) -= _y;
  dx(2) -= _z;
  TMatrix cov(2,2); // TMatrixTSym is broken (not always symmetric)?
  cov(0,0) = _cov_xx + cov_pv(0,0);
  cov(0,1) = _cov_xy + cov_pv(0,1);
  cov(1,1) = _cov_yy + cov_pv(1,1);
  cov(1,0) = cov(0,1);
  bool inv_fail = false;
  try {
    cov.Invert();
  } catch(...) { inv_fail = true; }
  if( inv_fail ) { return std::numeric_limits<AnaVertexFit::ftype>::max(); }
  TMatrix& w( cov ); // cov is not the covariance anymore.
  AnaVertexFit::ftype sig = static_cast<AnaVertexFit::ftype>(0);
  for( unsigned int i=0; i!=2; ++i ) { 
    for( unsigned int j=i; j!=2; ++j ) { 
      sig += static_cast<AnaVertexFit::ftype>( dx(i)*dx(j)*w(i,j) );
    }
  }
  return detector::safe_sqrt(sig);
}

const AnaVertexFit::ftype
AnaVertexFit::mag_significance( const TVector& pv , const TMatrix& cov_pv ) const
{
  assert_fitted();
  TVector dx = pv;
  dx(0) -= _x;
  dx(1) -= _y;
  dx(2) -= _z;
  TMatrix cov(3,3); // TMatrixTSym is broken (not always symmetric)?
  cov(0,0) = _cov_xx + cov_pv(0,0);
  cov(0,1) = _cov_xy + cov_pv(0,1);
  cov(0,2) = _cov_xz + cov_pv(0,2);
  cov(1,1) = _cov_yy + cov_pv(1,1);
  cov(1,2) = _cov_yz + cov_pv(1,2);
  cov(2,2) = _cov_zz + cov_pv(2,2);
  cov(1,0) = cov(0,1);
  cov(2,0) = cov(0,2);
  cov(2,1) = cov(1,2);
  bool inv_fail = false;
  try {
    cov.Invert();
  } catch(...) { inv_fail = true; }
  if( inv_fail ) { return std::numeric_limits<AnaVertexFit::ftype>::max(); }
  TMatrix& w( cov ); // cov is not the covariance anymore.
  AnaVertexFit::ftype sig = static_cast<AnaVertexFit::ftype>(0);
  for( unsigned int i=0; i!=3; ++i ) { 
    for( unsigned int j=i; j!=3; ++j ) { 
      sig += static_cast<AnaVertexFit::ftype>( dx(i)*dx(j)*w(i,j) );
    }
  }
  return detector::safe_sqrt(sig);
}


//template< typename collIterT >
typedef std::vector< boost::shared_ptr<const AnaTrack> >::const_iterator collIterT;
const bool
AnaVertexFit::compute_sv_variables( collIterT begin , collIterT end , 
                                    AnaVertexFit::ftype& vertex_mass , 
                                    AnaVertexFit::ftype& vertex_pt_ratio ,
                                    unsigned int& n_two_track_vertices ,
                                    AnaVertexFit* result )
{
  
  dg::down( "compute_sv" , "plots SV1 variable computation" ); BOOST_SCOPE_EXIT(void) { dg::up(); } BOOST_SCOPE_EXIT_END;
  dg::fillh( "n_tracks" , 100 , 0, 100 , std::distance(begin,end) );

  // compute SV1 tagging variables from a collection of tracks.
  // mimics portions of http://alxr.usatlas.bnl.gov/lxr-stb3/source/atlas/Reconstruction/VKalVrt/BTagVrtSec/src/BTagVrtSec.cxx#233
  vertex_mass = -1.;
  vertex_pt_ratio = 1.;
  n_two_track_vertices = 0u;
  if( std::distance(begin,end)<2 ) { return false; }

  // loop over tracks and identify z vertex as z0 of highest pT track.
  // this is biased for b jets and dilutes the z0 significance.
  double zvert_pt = 0.;
  double zvert_z = 0.;
  double zvert_zerr = 0.;
  for( collIterT itrack=begin, ftrack=end; itrack!=ftrack; ++itrack ) {
    const boost::shared_ptr<const AnaTrack>& track( *itrack );
    if( track->pt() > zvert_pt ) { 
      zvert_pt = track->pt();
      zvert_z = track->z0()/10.; // cm
      zvert_zerr = track->z0_err()/10.; // cm
    }
  }
  dg::fillh( "zvert_pt" , 100 , 0 , 20 , zvert_pt );
  dg::fillh( "zvert_z" , 100 , -10 , 10 , zvert_z );
  dg::fillh( "zvert_zerr" , 100 , 0 , 5 , zvert_zerr );
  
  // loop over tracks and find displaced two track vertices. identify
  // tracks for secondary vertexing using criteria similar to
  // Reconstruction/VKalVrt/BTagVrtSec/src/BTagVrtSec.cxx#667
  unsigned int n_prompt_two = 0u;
  unsigned int n_displaced_two = 0u;
  unsigned int n_good_two = 0u;
  double coll_sumpt = 0.;
  const double sellim = 2.0/sqrt(2.);
  vector< boost::shared_ptr<const AnaTrack> > seed_tracks(2);
  vector< boost::shared_ptr<const AnaTrack> > vertex_tracks; 
  vertex_tracks.reserve( detector::pow<2>(std::distance(begin,end)) );
  for( collIterT itrack=begin, ftrack=end; itrack!=ftrack; ++itrack ) {
    seed_tracks[0] = *itrack;
    boost::shared_ptr<const AnaTrack> trkA( seed_tracks[0] );
    ++( ( trkA->d0_signif_monica() > 3 ) ? n_displaced_two : n_prompt_two );
    coll_sumpt += trkA->pt();
    const double adp_trkA = std::sqrt(std::min(trkA->pt(),static_cast<float>(64))/64.)/2.;
    const bool ok_sig_trkA = std::abs( trkA->d0_signif_monica() ) > ( sellim + adp_trkA );
    dg::fillh( "seed_trkA_d0sig_pre" , 100 , -25 , 25 , trkA->d0_signif_monica() );
    dg::fillh( "seed_trkA_adp_pre" , 100 , 0 , 1 , adp_trkA );
    dg::filleff( "ok_sig_trkA" , ok_sig_trkA );
    if( !ok_sig_trkA ) { continue; }
    dg::fillh( "seed_trkA_d0sig" , 100 , -25 , 25 , trkA->d0_signif_monica() );
    dg::fillh( "seed_trkA_adp" , 100 , 0 , 1 , adp_trkA );
    for( collIterT jtrack=itrack; jtrack!=ftrack; ++jtrack ) {
      if( jtrack==itrack ) { continue; }
      seed_tracks[1] = *jtrack;
      boost::shared_ptr<const AnaTrack> trkB( seed_tracks[1] );
      const double adp_trkB = std::sqrt(std::min(trkB->pt(),static_cast<float>(64))/64.)/2.;
      const bool ok_sig_trkB = std::abs( trkB->d0_signif_monica() ) > ( sellim + adp_trkB );
      dg::fillh( "seed_trkB_d0sig_pre" , 100 , -25 , 25 , trkB->d0_signif_monica() );
      dg::fillh( "seed_trkB_adp_pre" , 100 , 0 , 1 , adp_trkB );
      dg::filleff( "ok_sig_trkB" , ok_sig_trkB );
      if( !ok_sig_trkB ) { continue; }
      dg::fillh( "seed_trkB_d0sig" , 100 , -25 , 25 , trkB->d0_signif_monica() );
      dg::fillh( "seed_trkB_adp" , 100 , 0 , 1 , adp_trkB );
      AnaVertexFit vf;
      vf.add( seed_tracks.begin() , seed_tracks.end() );
      vf.track_status_all( true ); // fit all tracks
      const bool fit_ok = vf.fit();
      dg::filleff( "seed_fit_ok" , fit_ok );
      if( !fit_ok ) { continue; }
      const double sum_track_d0sig = (std::abs(trkA->d0_signif_monica()) + std::abs(trkB->d0_signif_monica()));
      dg::fillh( "seed_fit_chi2ndof" , 100 , 0 , 25 , vf.chi2ndof() );
      dg::fillh( "seed_fit_sum_track_d0sig" , 100 , 0 , 25 , sum_track_d0sig );
      dg::fillh( "seed_fit_sum_track_d0sig_vec" , 100 , -25 , 25 , (trkA->d0_signif_monica()) + (trkB->d0_signif_monica()) );
      // check fit chi^2
      const bool ok_fit_chi2 = vf.chi2ndof()<4.5;
      // check fit displacement
      const double displacement_cut = (2.*sellim + 2.5 + adp_trkA + adp_trkB );
      const bool ok_tracks_displaced = sum_track_d0sig >= displacement_cut;
      // ignore V0 rejection for now. is this not a small effect?
      const bool ok_two_track = ok_fit_chi2 && ok_tracks_displaced;
      dg::filleff( "seed_ok_fit_chi2" , ok_fit_chi2 );
      dg::filleff( "seed_ok_tracks_displaced" , ok_tracks_displaced );
      dg::fillh( "seed_displacement_cut" , 100 , 0 , 50 , displacement_cut );
      if( !ok_two_track ) { continue; }
      // 3d displacement and significance. use crude estimate of vertex z and ignore full covariance.
      TVector fake_pv(3); fake_pv.Zero(); fake_pv(2) = zvert_z;
      TMatrix fake_pv_cov(3,3); fake_pv_cov.Zero(); fake_pv_cov(2,2) = detector::pow<2>( zvert_zerr );
      const AnaVertexFit::ftype sig3d = vf.mag_significance( fake_pv , fake_pv_cov );
      const bool displaced_sig3d = sig3d > 2.5;
      dg::fillh( "seed_sig3d" , 200 , -100 , 100 , sig3d );
      dg::filleff( "seed_displaced_sig3d" , displaced_sig3d );
      if( displaced_sig3d ) {
        vertex_tracks.push_back( trkA ); 
        vertex_tracks.push_back( trkB ); 
        ++n_good_two;
      }
    } // end for each second track
  } // end for each first track

  // weed out duplicates from vertex track list
  std::sort( vertex_tracks.begin() , vertex_tracks.end() );
  vector< boost::shared_ptr<const AnaTrack> >::iterator vertex_tracks_end_unique = std::unique( vertex_tracks.begin() , vertex_tracks.end() );
  vertex_tracks.erase( vertex_tracks_end_unique , vertex_tracks.end() );
  // vertex_tracks now contains at most one entry for each track.

  const unsigned int n_vertex_tracks_start = vertex_tracks.size();
  dg::fillh( "n_prompt_two" , 100 , 0, 100 , n_prompt_two );
  dg::fillh( "n_displaced_two" , 100 , 0, 100 , n_displaced_two );
  dg::fillh( "n_good_two" , 100 , 0 , 100 , n_good_two );
  dg::fillh( "n_vertex_tracks_start" , 100 , 0 , 100 , n_vertex_tracks_start );
  dg::fillh( "coll_sumpt" , 100 , 0 , 50 , coll_sumpt );

  // nothing to do for one-track "vertices"
  if( vertex_tracks.size() < 2 ) { return false; }
  
  // fit a common vertex to all tracks. if the chi^2 of the vertex is
  // bad, prune the track with the largest contribution to the chi^2
  // and refit.
  
  AnaVertexFit combined_vf;
  while( vertex_tracks.size() >= 2 ) {
    AnaVertexFit this_vf;
    this_vf.add( vertex_tracks.begin() , vertex_tracks.end() );
    this_vf.track_status_all( true ); // fit all tracks
    const bool this_fit_ok = this_vf.fit();
    if( !this_fit_ok ) { break; }
    const bool good_vertex_mass = this_vf.vertex_momentum().M() < 6.;
    const bool good_chi2prob = this_vf.chi2prob() > 0.001;
    dg::filleff( "combined_iteration_good_vertex_mass" , good_vertex_mass );
    dg::filleff( "combined_iteration_good_chi2prob" , good_chi2prob );
    dg::fillh( "combined_iteration_vertex_mass" , 100 , 0 , 10 , this_vf.vertex_momentum().M() );
    dg::fillh( "combined_iteration_chi2prob" , 100 , 0 , 1 , this_vf.chi2prob() );
    dg::fillh( "combined_iteration_chi2ndof" , 100 , 0 , 25 , this_vf.chi2ndof() );
    // check fit chi^2 probability. if the fit is good, keep it.
    if( good_vertex_mass && good_chi2prob ) {
      combined_vf = this_vf;
      break;
    }
    // the fit is bad. prune the worst track and repeat.
    AnaVertexFit::DeltaChi2Coll::const_iterator iworst = std::max_element( this_vf.track_delta_chi2s().begin() , this_vf.track_delta_chi2s().end() );
    assert( iworst!=this_vf.track_delta_chi2s().end() );
    unsigned int nworst = std::distance( this_vf.track_delta_chi2s().begin() , iworst );
    dg::fillh( "combined_iteration_worst_dchi2" , 100 , 0 , 25 , *iworst );
    vector< boost::shared_ptr<const AnaTrack> >::iterator ierase = vertex_tracks.begin(); 
    std::advance( ierase , nworst );
    vertex_tracks.erase( ierase );
  } // iterate until fit converges

  dg::filleff( "combined_converged" , combined_vf.is_fitted() );
  dg::fillh( "n_vertex_tracks_end" , 100 , 0 , 100 , vertex_tracks.size() );
  dg::fillh( "n_vertex_tracks_begin_m_end" , 100 , 0 , 100 , vertex_tracks.size()-n_vertex_tracks_start );

  // did the fit converge?
  if( !(combined_vf.is_fitted()) ) { return false; }

  // reject any conversions that have survived.
  bool conversion_flag = false;
  if( vertex_tracks.size()==2 ) {
    const bool large_2d = combined_vf.perp()>2.; // cm
    if( large_2d ) { 
      const bool small_mass = detector::two_track_mass( combined_vf.track_momentum(0) , combined_vf.track_momentum(1) , 
                                                        detector::electron_mass(detector::GeV) , detector::electron_mass(detector::GeV) ) < 0.1;
      if( small_mass ) { conversion_flag = true; }
    }
  } // end reject residual conversions
  dg::filleff( "conversion_flag" , conversion_flag );
  if( conversion_flag ) { 
    return false;
  }

  // vertex fit is good. compute sv variables.
  vertex_mass = combined_vf.vertex_momentum().M();
  // const double vertex_track_pt = std::accumulate( vertex_tracks.begin() , vertex_tracks.end() , 0. ,
  //                                                 boost::bind( std::plus<AnaTrack::ftype>() , 
  //                                                              boost::bind(&AnaTrack::pt,_1) ,
  //                                                              boost::bind(&AnaTrack::pt,_2) ) );
  double vertex_track_pt = 0.;
  for( vector< boost::shared_ptr<const AnaTrack> >::iterator i=vertex_tracks.begin(), f=vertex_tracks.end(); i!=f; ++i ) { vertex_track_pt += (*i)->pt(); }
  vertex_pt_ratio = detector::safe_divide( vertex_track_pt , coll_sumpt );
  n_two_track_vertices = n_good_two;
  
  dg::fillh( "good_vertex_mass" , 100 , 0 , 10 , vertex_mass );
  dg::fillh( "good_vertex_pt_ratio" , 100 , 0 , 1 , vertex_pt_ratio );
  dg::fillh( "good_vertex_n_two" , 100 , 0 , 100 , n_two_track_vertices );
  dg::fillh( "good_vertex_mass_vs_pt_ratio" , 10 , 0 , 1 , 20 , 0 , 10 , vertex_pt_ratio , vertex_mass , "VERTEX PT / TRACK SUM PT" , "VERTEX MASS (GeV)" );
  dg::fillh( "good_vertex_mass_vs_n_two" , 25 , 0 , 25 , 20 , 0 , 10 , n_two_track_vertices , vertex_mass , "N TWO TRACK PAIRS" , "VERTEX MASS (GeV)" );
  dg::fillh( "good_vertex_pt_ratio_vs_n_two" , 25 , 0 , 25 , 10 , 0 , 1 , n_two_track_vertices , vertex_pt_ratio , "N TWO TRACK PAIRS" , "VERTEX PT / TRACK SUM PT" );

  if( result ) { (*result) = combined_vf; }

  return true;
}
