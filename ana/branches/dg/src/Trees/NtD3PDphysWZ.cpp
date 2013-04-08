#include <string>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <limits>
#include <vector>
#include <map>
#include <set>
#include <cmath>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/format.hpp>
#include <boost/math/special_functions/round.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/filesystem.hpp>
#include <TTree.h>
#include <TFile.h>
#include <TBranch.h>
#include <TROOT.h>
#include <TEnv.h>
#include <TSystem.h>
#include <TMatrix.h>
#include <TLorentzVector.h>
#include "TrigFTKAna/Trees/NtD3PDphysWZ.hpp"
#include "TrigFTKAna/AnaElectron.hpp"
#include "TrigFTKAna/AnaPhoton.hpp"
#include "TrigFTKAna/AnaMuon.hpp"
#include "TrigFTKAna/AnaJet.hpp"
#include "TrigFTKAna/AnaMET.hpp"
#include "TrigFTKAna/AnaTrigger.hpp"
#include "TrigFTKAna/AnaVertex.hpp"
#include "TrigFTKAna/AnaTrack.hpp"
#include "TrigFTKAna/AnaTruthParticle.hpp"
#include "TrigFTKAna/AnaTruthTable.hpp"
#include "TrigFTKAna/ana_streams_helper.hpp"
#include "TrigFTKAna/AnaConfiguration.hpp"
#include "TrigFTKAna/Trees/D3PDWZTreeBase.h"
#include "TrigFTKAna/Trees/D3PDWZTreeV5.h"
#include "TrigFTKAna/tracking/VertexType.hpp"

using namespace std;
using boost::shared_ptr;

extern TROOT* gROOT;
extern TSystem* gSystem;
extern TEnv* gEnv;

const bool
NtD3PDphysWZ::begin_file( const std::string& raw_filename )
{
  // workaround for bug in Athena. symptoms are warning messages like:
  // Error in <TBufferFile::CheckByteCount>: object of class vector<vector<float> > read too few bytes: 6 instead of 22
  if( true ) {
    static bool did = false;
    if( !did ) { gROOT->ProcessLine("#include <vector>"); did = true; }
  }

  const string filename = ana_streams::root_dcap_filename(raw_filename);

  _file.reset( ana_streams::open_root_file( filename ) );
  
  if( !_file ) { 
    cout << " NtD3PDphysWZ: unable to open file " << filename << endl;
    return false;
  }
  assert( _file );
  TTree* tree = dynamic_cast<TTree*>(_file->Get("physics"));
  if( !tree ) { 
    cout << " NtD3PDphysWZ: unable to find physics tree file " << filename << endl;
    return false;
  }

  // Version 1 corresponds to April/May SM D3PDs used for initial W
  // cross section cut flow. Version 2 corresponds to data10 r1239
  // processing from May/June.
  AnaConfiguration::release(17);
  _madeclass_version = V5;
  if( boost::icontains( filename , "WZ" ) ) {
    _madeclass_version = V5;
    AnaConfiguration::release(17);
    cout << " NtD3PDphysWZ: configured for V5 ntuple." << endl;
  }
  
  switch( _madeclass_version ) { 
  case V5: _madeclass.reset( new D3PDWZTreeV5(tree) ); break;
  default: assert( !"unhandled case" );
  }

  _madeclass->Init( tree );
  _n_events = static_cast<unsigned long>( tree->GetEntries() ); // slow for long trees.

  if( !tree->GetCurrentFile() || tree->GetCurrentFile()->IsZombie() || !(tree->GetCurrentFile()->IsOpen()) ) { 
    cout << " could not open WZ D3PD: " << ana_streams::root_dcap_filename(filename) << endl;
    return false;
  }
  if( _n_events == 0 ) {
    cout << " empty WZ D3PD: " << filename << endl;
    return false;
  }

  _open_filename = filename;
  return true;
}

const bool
NtD3PDphysWZ::end_file()
{
  _n_events = 0;
  _open_filename = string();
  if( _file ) { 
    _file->Close();
    _file.reset();
  }
  _madeclass.reset();
  return true;
}

void
NtD3PDphysWZ::set_enabled_branches()
{
}

void
NtD3PDphysWZ::set_branch_addresses()
{
}

const bool
NtD3PDphysWZ::get_event( const unsigned long& ievent )
{
  if( !_file ) { return false; }
  if( !_madeclass ) { return false; }
  if( _n_events == 0ul ) { return false; }
  if( ievent>=_n_events ) { return false; }

  // if( _madeclass->LoadTree( ievent ) < 0 ) { return false; }
  // load active branches
  if( _madeclass->GetEntry( ievent ) < 0 ) { return false; } // returns nbytes

  clear();

  switch( _madeclass_version ) {
  case V5: return _get_event_V5(); break;
  default: assert( !"unhandled case" );
  }

  return false;
}

const unsigned long
NtD3PDphysWZ::count_events( const std::string& filename )
{
  TFile* f = ana_streams::open_root_file( ana_streams::root_dcap_filename(filename) );
  if( !f ) { 
    cout << " NtD3PDphysWZ: unable to open file " << filename << endl;
    return 0;
  }
  TTree* tree = dynamic_cast<TTree*>(f->Get("physics"));
  if( !tree ) { 
    cout << " NtD3PDphysWZ: unable to find physics tree in file " << filename << endl;
    return 0;
  }
  const unsigned long n = tree->GetEntries();
  delete f;
  cout << " NtD3PDphysWZ: file " << filename << " contains " << n << " events." << endl;
  return n;
}


template<typename ntT> 
void
NtD3PDphysWZ::_make_associated_track_for_electron( shared_ptr<AnaElectron>& ele , 
                                                   ntT nt , const unsigned int& i , 
                                                   const shared_ptr<const AnaVertex>& vx )
{
  shared_ptr<AnaTrack> track( new AnaTrack );
  ele->_id_track = track;
  track->_curv = detector::safe_divide(static_cast<AnaTrack::ftype>(1),(*nt->el_trackpt)[i])*detector::sign_of((*nt->el_trackqoverp)[i]); // 1/mev
  track->_d0 = (*nt->el_trackd0)[i];
  track->_phi0 = (*nt->el_trackphi)[i];
  track->_z0 = (*nt->el_trackz0)[i];
  track->_lambda = detector::cot( (*nt->el_tracktheta)[i] );
  if( vx ) { 
    track->_exPV_d0 = detector::corrected_d0( track->_d0 , track->_z0 , track->_phi0 , detector::sign_of(track->_curv) , 
                                              vx->x() , vx->y() , vx->z() );
    track->_exPV_z0 = detector::corrected_z0( track->_d0 , track->_z0 , track->_phi0 , track->_lambda , detector::sign_of(track->_curv) , 
                                              vx->x() , vx->y() , vx->z() );
    // track->_exPV_d0_err = detector::corrected_d0_err( (*(nt->el_trackcov_d0))[i] , vx->x_err() , vx->y_err() , track->_phi0 );
    // track->_exPV_z0_err = detector::corrected_z0_err( (*(nt->el_trackcov_z0))[i] , vx->z_err() );
    // vertex errors not in ntuple?
    track->_exPV_d0_err = detector::corrected_d0_err( (*(nt->el_trackcov_d0))[i] , static_cast<AnaTrack::ftype>(0) , static_cast<AnaTrack::ftype>(0) , track->_phi0 );
    track->_exPV_z0_err = detector::corrected_z0_err( (*(nt->el_trackcov_z0))[i] , static_cast<AnaTrack::ftype>(0) );
  }
  // fill covariance matrix --- todo. see NtCBNT jacobian.
  // _cov_cc; 
  // _cov_cd;
  // _cov_cp;
  // _cov_cz;
  // _cov_ct;
  // _cov_dd;
  // _cov_dp;
  // _cov_dz;
  // _cov_dt;
  // _cov_pp;
  // _cov_pz;
  // _cov_pt;
  // _cov_zz;
  // _cov_zt;
  // _cov_tt;
  // fit properties
  track->_chi2 = (*nt->el_trackfitchi2)[i];
  track->_ndof = (*nt->el_trackfitndof)[i];
  track->_n_b_layer_hits = (*nt->el_nBLHits)[i];
  track->_n_pixel_hits = (*nt->el_nPixHits)[i];
  track->_n_sct_hits = (*nt->el_nSCTHits)[i];
  track->_n_trt_hits = (*nt->el_nTRTHits)[i];
  track->_expect_b_layer_hits = (*nt->el_expectHitInBLayer)[i];
  track->_trratio = (*nt->el_TRTHighTHitsRatio)[i];
  // not available: track->_n_b_layer_outliers = 
  // not available: track->_n_pixel_outliers = 
  track->_n_pixel_holes = (*nt->el_nPixHoles)[i];
  track->_n_sct_holes = (*nt->el_nSCTHoles)[i];
  track->_nhits = (*nt->el_nSiHits)[i] + (*nt->el_nTRTHits)[i];
  track->_nmisses =  (*nt->el_nPixHoles)[i] +  (*nt->el_nSCTHoles)[i];
  track->_n_pixel_shared = (*nt->el_nPixSharedHits)[i];
  track->_n_sct_shared = (*nt->el_nSCTSharedHits)[i];  
}

template<> 
void
NtD3PDphysWZ::_make_associated_track_for_electron( shared_ptr<AnaElectron>& ele , 
                                                   const D3PDWZTreeV2* nt , const unsigned int& i , 
                                                   const shared_ptr<const AnaVertex>& vx )
{
  shared_ptr<AnaTrack> track( new AnaTrack );
  ele->_id_track = track;
  track->_curv = detector::safe_divide(static_cast<AnaTrack::ftype>(1),(*nt->el_trackpt)[i])*detector::sign_of((*nt->el_trackqoverp)[i]); // 1/mev
  track->_d0 = (*nt->el_trackd0)[i];
  track->_phi0 = (*nt->el_trackphi)[i];
  track->_z0 = (*nt->el_trackz0)[i];
  track->_lambda = detector::cot( (*nt->el_tracktheta)[i] );
  if( vx ) { 
    track->_exPV_d0 = detector::corrected_d0( track->_d0 , track->_z0 , track->_phi0 , detector::sign_of(track->_curv) , 
                                              vx->x() , vx->y() , vx->z() );
    track->_exPV_z0 = detector::corrected_z0( track->_d0 , track->_z0 , track->_phi0 , track->_lambda , detector::sign_of(track->_curv) , 
                                              vx->x() , vx->y() , vx->z() );
    // track->_exPV_d0_err = detector::corrected_d0_err( (*(nt->el_trackcov_d0))[i] , vx->x_err() , vx->y_err() , track->_phi0 );
    // track->_exPV_z0_err = detector::corrected_z0_err( (*(nt->el_trackcov_z0))[i] , vx->z_err() );
    // vertex errors not in ntuple?
    track->_exPV_d0_err = detector::corrected_d0_err( (*(nt->el_trackcov_d0))[i] , static_cast<AnaTrack::ftype>(0) , static_cast<AnaTrack::ftype>(0) , track->_phi0 );
    track->_exPV_z0_err = detector::corrected_z0_err( (*(nt->el_trackcov_z0))[i] , static_cast<AnaTrack::ftype>(0) );
  }
  // fill covariance matrix --- todo. see NtCBNT jacobian.
  // _cov_cc; 
  // _cov_cd;
  // _cov_cp;
  // _cov_cz;
  // _cov_ct;
  // _cov_dd;
  // _cov_dp;
  // _cov_dz;
  // _cov_dt;
  // _cov_pp;
  // _cov_pz;
  // _cov_pt;
  // _cov_zz;
  // _cov_zt;
  // _cov_tt;
  // fit properties
  track->_chi2 = (*nt->el_trackfitchi2)[i];
  track->_ndof = (*nt->el_trackfitndof)[i];
  track->_n_b_layer_hits = (*nt->el_nBLHits)[i];
  track->_n_pixel_hits = (*nt->el_nPixHits)[i];
  track->_n_sct_hits = (*nt->el_nSCTHits)[i];
  track->_n_trt_hits = (*nt->el_nTRTHits)[i];
  track->_trratio = (*nt->el_TRTHighTHitsRatio)[i];
  // not available: track->_n_b_layer_outliers = 
  // not available: track->_n_pixel_outliers = 
  track->_n_pixel_holes = (*nt->el_nPixHoles)[i];
  track->_n_sct_holes = (*nt->el_nSCTHoles)[i];
  track->_nhits = (*nt->el_nSiHits)[i] + (*nt->el_nTRTHits)[i];
  track->_nmisses =  (*nt->el_nPixHoles)[i] +  (*nt->el_nSCTHoles)[i];
  track->_n_pixel_shared = (*nt->el_nPixSharedHits)[i];
  track->_n_sct_shared = (*nt->el_nSCTSharedHits)[i];  
}

template<> 
void
NtD3PDphysWZ::_make_associated_track_for_electron<const D3PDWZTree*>( shared_ptr<AnaElectron>& ele , 
                                                                      const D3PDWZTree* nt , const unsigned int& i , 
                                                                      const shared_ptr<const AnaVertex>& vx )
{
  shared_ptr<AnaTrack> track( new AnaTrack );
  ele->_id_track = track;
  track->_curv = detector::safe_divide(static_cast<AnaTrack::ftype>(1),(*nt->el_trackpt)[i])*detector::sign_of((*nt->el_trackqoverp)[i]); // 1/mev
  track->_d0 = (*nt->el_trackd0)[i];
  track->_phi0 = (*nt->el_trackphi)[i];
  track->_z0 = (*nt->el_trackz0)[i];
  track->_lambda = detector::cot( (*nt->el_tracktheta)[i] );
  if( vx ) { 
    track->_exPV_d0 = detector::corrected_d0( track->_d0 , track->_z0 , track->_phi0 , detector::sign_of(track->_curv) , 
                                              vx->x() , vx->y() , vx->z() );
    track->_exPV_z0 = detector::corrected_z0( track->_d0 , track->_z0 , track->_phi0 , track->_lambda , detector::sign_of(track->_curv) , 
                                              vx->x() , vx->y() , vx->z() );
    track->_exPV_d0_err = detector::corrected_d0_err( (*(nt->el_trackcovd0))[i] , vx->x_err() , vx->y_err() , track->_phi0 );
    track->_exPV_z0_err = detector::corrected_z0_err( (*(nt->el_trackcovz0))[i] , vx->z_err() );
  }
  // fill covariance matrix --- todo. see NtCBNT jacobian.
  // _cov_cc; 
  // _cov_cd;
  // _cov_cp;
  // _cov_cz;
  // _cov_ct;
  // _cov_dd;
  // _cov_dp;
  // _cov_dz;
  // _cov_dt;
  // _cov_pp;
  // _cov_pz;
  // _cov_pt;
  // _cov_zz;
  // _cov_zt;
  // _cov_tt;
  // fit properties
  track->_chi2 = (*nt->el_trackfitchi2)[i];
  track->_ndof = (*nt->el_trackfitndof)[i];
  track->_n_b_layer_hits = (*nt->el_nBLHits)[i];
  track->_n_pixel_hits = (*nt->el_nPixHits)[i];
  track->_n_sct_hits = (*nt->el_nSCTHits)[i];
  track->_n_trt_hits = (*nt->el_nTRTHits)[i];
  track->_n_pixel_holes = (*nt->el_nPixHoles)[i];
  track->_n_sct_holes = (*nt->el_nSCTHoles)[i];
  track->_nhits = (*nt->el_nSiHits)[i] + (*nt->el_nTRTHits)[i];
  track->_nmisses =  (*nt->el_nPixHoles)[i] +  (*nt->el_nSCTHoles)[i];
  track->_n_pixel_shared = (*nt->el_nPixSharedHits)[i];
  track->_n_sct_shared = (*nt->el_nSCTSharedHits)[i];  
}

template<typename ntT> 
void 
NtD3PDphysWZ::_make_associated_track_for_muid_muon( shared_ptr<AnaMuon>& mu , 
                                                    ntT nt , const unsigned int& i , 
                                                    const shared_ptr<const AnaVertex>& vx )
{
  shared_ptr<AnaTrack> track( new AnaTrack );
  mu->_id_track = track;
  const AnaTrack::ftype trackpt = std::sin((*nt->mu_muid_tracktheta)[i]) * std::abs( detector::safe_divide(static_cast<AnaTrack::ftype>(1),(*nt->mu_muid_trackqoverp)[i]) );
  track->_curv = detector::safe_divide(static_cast<AnaTrack::ftype>(1),trackpt)*detector::sign_of((*nt->mu_muid_trackqoverp)[i]); // 1/mev
  track->_d0 = (*nt->mu_muid_trackd0)[i];
  track->_phi0 = (*nt->mu_muid_trackphi)[i];
  track->_z0 = (*nt->mu_muid_trackz0)[i];
  track->_lambda = detector::cot((*nt->mu_muid_tracktheta)[i]);
  if( vx ) { 
    track->_exPV_d0 = detector::corrected_d0( track->_d0 , track->_z0 , track->_phi0 , detector::sign_of(track->_curv) , 
                                              vx->x() , vx->y() , vx->z() );
    track->_exPV_z0 = detector::corrected_z0( track->_d0 , track->_z0 , track->_phi0 , track->_lambda , detector::sign_of(track->_curv) , 
                                              vx->x() , vx->y() , vx->z() );
    track->_exPV_d0_err = detector::corrected_d0_err( (*(nt->mu_muid_trackcov_d0))[i] , vx->x_err() , vx->y_err() , track->_phi0 );
    track->_exPV_z0_err = detector::corrected_z0_err( (*(nt->mu_muid_trackcov_z0))[i] , vx->z_err() );
  }
  // fill covariance matrix --- todo. see NtCBNT jacobian.
  // _cov_cc; 
  // _cov_cd;
  // _cov_cp;
  // _cov_cz;
  // _cov_ct;
  // _cov_dd;
  // _cov_dp;
  // _cov_dz;
  // _cov_dt;
  // _cov_pp;
  // _cov_pz;
  // _cov_pt;
  // _cov_zz;
  // _cov_zt;
  // _cov_tt;
  // fit properties
  track->_chi2 = (*nt->mu_muid_trackfitchi2)[i];
  track->_ndof = (*nt->mu_muid_trackfitndof)[i];
  track->_n_b_layer_hits = (*nt->mu_muid_nBLHits)[i];
  track->_n_pixel_hits = (*nt->mu_muid_nPixHits)[i];
  track->_n_sct_hits = (*nt->mu_muid_nSCTHits)[i];
  track->_n_trt_hits = (*nt->mu_muid_nTRTHits)[i];
  track->_n_pixel_holes = (*nt->mu_muid_nPixHoles)[i];
  track->_n_sct_holes = (*nt->mu_muid_nSCTHoles)[i];
  track->_nhits = (*nt->mu_muid_nPixHits)[i] + (*nt->mu_muid_nSCTHits)[i] + (*nt->mu_muid_nTRTHits)[i] + (*nt->mu_muid_nTRTOutliers)[i];
  track->_nmisses =  (*nt->mu_muid_nPixHoles)[i] +  (*nt->mu_muid_nSCTHoles)[i];
  track->_n_pixel_shared = (*nt->mu_muid_nPixSharedHits)[i];
  track->_n_sct_shared = (*nt->mu_muid_nSCTSharedHits)[i];
}

template<> 
void 
NtD3PDphysWZ::_make_associated_track_for_muid_muon( shared_ptr<AnaMuon>& mu , 
                                                    const D3PDWZTree* nt , const unsigned int& i , 
                                                    const shared_ptr<const AnaVertex>& vx )
{
  shared_ptr<AnaTrack> track( new AnaTrack );
  mu->_id_track = track;
  const AnaTrack::ftype trackpt = std::sin((*nt->mu_muid_tracktheta)[i]) * std::abs( detector::safe_divide(static_cast<AnaTrack::ftype>(1),(*nt->mu_muid_trackqoverp)[i]) );
  track->_curv = detector::safe_divide(static_cast<AnaTrack::ftype>(1),trackpt)*detector::sign_of((*nt->mu_muid_trackqoverp)[i]); // 1/mev
  track->_d0 = (*nt->mu_muid_trackd0)[i];
  track->_phi0 = (*nt->mu_muid_trackphi)[i];
  track->_z0 = (*nt->mu_muid_trackz0)[i];
  track->_lambda = detector::cot((*nt->mu_muid_tracktheta)[i]);
  if( vx ) { 
    track->_exPV_d0 = detector::corrected_d0( track->_d0 , track->_z0 , track->_phi0 , detector::sign_of(track->_curv) , 
                                              vx->x() , vx->y() , vx->z() );
    track->_exPV_z0 = detector::corrected_z0( track->_d0 , track->_z0 , track->_phi0 , track->_lambda , detector::sign_of(track->_curv) , 
                                              vx->x() , vx->y() , vx->z() );
    track->_exPV_d0_err = detector::corrected_d0_err( (*(nt->mu_muid_trackcovd0))[i] , vx->x_err() , vx->y_err() , track->_phi0 );
    track->_exPV_z0_err = detector::corrected_z0_err( (*(nt->mu_muid_trackcovz0))[i] , vx->z_err() );
  }
  // fill covariance matrix --- todo. see NtCBNT jacobian.
  // _cov_cc; 
  // _cov_cd;
  // _cov_cp;
  // _cov_cz;
  // _cov_ct;
  // _cov_dd;
  // _cov_dp;
  // _cov_dz;
  // _cov_dt;
  // _cov_pp;
  // _cov_pz;
  // _cov_pt;
  // _cov_zz;
  // _cov_zt;
  // _cov_tt;
  // fit properties
  track->_chi2 = (*nt->mu_muid_trackfitchi2)[i];
  track->_ndof = (*nt->mu_muid_trackfitndof)[i];
  track->_n_b_layer_hits = (*nt->mu_muid_nBLHits)[i];
  track->_n_pixel_hits = (*nt->mu_muid_nPixHits)[i];
  track->_n_sct_hits = (*nt->mu_muid_nSCTHits)[i];
  track->_n_trt_hits = (*nt->mu_muid_nTRTHits)[i];
  track->_n_pixel_holes = (*nt->mu_muid_nPixHoles)[i];
  track->_n_sct_holes = (*nt->mu_muid_nSCTHoles)[i];
  track->_nhits = (*nt->mu_muid_nPixHits)[i] + (*nt->mu_muid_nSCTHits)[i] + (*nt->mu_muid_nTRTHits)[i]+ (*nt->mu_muid_nTRTOutliers)[i];
  track->_nmisses =  (*nt->mu_muid_nPixHoles)[i] +  (*nt->mu_muid_nSCTHoles)[i];
  track->_n_pixel_shared = (*nt->mu_muid_nPixSharedHits)[i];
  track->_n_sct_shared = (*nt->mu_muid_nSCTSharedHits)[i];
}

template<typename ntT> 
void
NtD3PDphysWZ::_make_associated_track_for_staco_muon( shared_ptr<AnaMuon>& mu , 
                                                     ntT nt , const unsigned int& i , 
                                                     const shared_ptr<const AnaVertex>& vx )
{
  shared_ptr<AnaTrack> track( new AnaTrack );
  mu->_id_track = track;
  const AnaTrack::ftype trackpt = std::sin((*nt->mu_staco_tracktheta)[i]) * std::abs( detector::safe_divide(static_cast<AnaTrack::ftype>(1),(*nt->mu_staco_trackqoverp)[i]) );
  track->_curv = detector::safe_divide(static_cast<AnaTrack::ftype>(1),trackpt)*detector::sign_of((*nt->mu_staco_trackqoverp)[i]); // 1/mev
  track->_d0 = (*nt->mu_staco_trackd0)[i];
  track->_phi0 = (*nt->mu_staco_trackphi)[i];
  track->_z0 = (*nt->mu_staco_trackz0)[i];
  track->_lambda = detector::cot((*nt->mu_staco_tracktheta)[i]);
  if( vx ) { 
    track->_exPV_d0 = detector::corrected_d0( track->_d0 , track->_z0 , track->_phi0 , detector::sign_of(track->_curv) , 
                                              vx->x() , vx->y() , vx->z() );
    track->_exPV_z0 = detector::corrected_z0( track->_d0 , track->_z0 , track->_phi0 , track->_lambda , detector::sign_of(track->_curv) , 
                                              vx->x() , vx->y() , vx->z() );
    track->_exPV_d0_err = detector::corrected_d0_err( (*(nt->mu_staco_trackcov_d0))[i] , vx->x_err() , vx->y_err() , track->_phi0 );
    track->_exPV_z0_err = detector::corrected_z0_err( (*(nt->mu_staco_trackcov_z0))[i] , vx->z_err() );
  }
  // fill covariance matrix --- todo. see NtCBNT jacobian.
  // _cov_cc; 
  // _cov_cd;
  // _cov_cp;
  // _cov_cz;
  // _cov_ct;
  // _cov_dd;
  // _cov_dp;
  // _cov_dz;
  // _cov_dt;
  // _cov_pp;
  // _cov_pz;
  // _cov_pt;
  // _cov_zz;
  // _cov_zt;
  // _cov_tt;
  // fit properties
  track->_chi2 = (*nt->mu_staco_trackfitchi2)[i];
  track->_ndof = (*nt->mu_staco_trackfitndof)[i];
  track->_n_b_layer_hits = (*nt->mu_staco_nBLHits)[i];
  track->_n_pixel_hits = (*nt->mu_staco_nPixHits)[i];
  track->_n_sct_hits = (*nt->mu_staco_nSCTHits)[i];
  track->_n_trt_hits = (*nt->mu_staco_nTRTHits)[i];
  track->_n_pixel_holes = (*nt->mu_staco_nPixHoles)[i];
  track->_n_sct_holes = (*nt->mu_staco_nSCTHoles)[i];
  track->_nhits = (*nt->mu_staco_nPixHits)[i] + (*nt->mu_staco_nSCTHits)[i] + (*nt->mu_staco_nTRTHits)[i]+ (*nt->mu_staco_nTRTOutliers)[i];
  track->_nmisses =  (*nt->mu_staco_nPixHoles)[i] +  (*nt->mu_staco_nSCTHoles)[i];
  track->_n_pixel_shared = (*nt->mu_staco_nPixSharedHits)[i];
  track->_n_sct_shared = (*nt->mu_staco_nSCTSharedHits)[i];
}

template<> 
void
NtD3PDphysWZ::_make_associated_track_for_staco_muon( shared_ptr<AnaMuon>& mu , 
                                                     const D3PDWZTree* nt , const unsigned int& i , 
                                                     const shared_ptr<const AnaVertex>& vx )
{
  shared_ptr<AnaTrack> track( new AnaTrack );
  mu->_id_track = track;
  const AnaTrack::ftype trackpt = std::sin((*nt->mu_staco_tracktheta)[i]) * std::abs( detector::safe_divide(static_cast<AnaTrack::ftype>(1),(*nt->mu_staco_trackqoverp)[i]) );
  track->_curv = detector::safe_divide(static_cast<AnaTrack::ftype>(1),trackpt)*detector::sign_of((*nt->mu_staco_trackqoverp)[i]); // 1/mev
  track->_d0 = (*nt->mu_staco_trackd0)[i];
  track->_phi0 = (*nt->mu_staco_trackphi)[i];
  track->_z0 = (*nt->mu_staco_trackz0)[i];
  track->_lambda = detector::cot((*nt->mu_staco_tracktheta)[i]);
  if( vx ) { 
    track->_exPV_d0 = detector::corrected_d0( track->_d0 , track->_z0 , track->_phi0 , detector::sign_of(track->_curv) , 
                                              vx->x() , vx->y() , vx->z() );
    track->_exPV_z0 = detector::corrected_z0( track->_d0 , track->_z0 , track->_phi0 , track->_lambda , detector::sign_of(track->_curv) , 
                                              vx->x() , vx->y() , vx->z() );
    track->_exPV_d0_err = detector::corrected_d0_err( (*(nt->mu_staco_trackcovd0))[i] , vx->x_err() , vx->y_err() , track->_phi0 );
    track->_exPV_z0_err = detector::corrected_z0_err( (*(nt->mu_staco_trackcovz0))[i] , vx->z_err() );
  }
  // fill covariance matrix --- todo. see NtCBNT jacobian.
  // _cov_cc; 
  // _cov_cd;
  // _cov_cp;
  // _cov_cz;
  // _cov_ct;
  // _cov_dd;
  // _cov_dp;
  // _cov_dz;
  // _cov_dt;
  // _cov_pp;
  // _cov_pz;
  // _cov_pt;
  // _cov_zz;
  // _cov_zt;
  // _cov_tt;
  // fit properties
  track->_chi2 = (*nt->mu_staco_trackfitchi2)[i];
  track->_ndof = (*nt->mu_staco_trackfitndof)[i];
  track->_n_b_layer_hits = (*nt->mu_staco_nBLHits)[i];
  track->_n_pixel_hits = (*nt->mu_staco_nPixHits)[i];
  track->_n_sct_hits = (*nt->mu_staco_nSCTHits)[i];
  track->_n_trt_hits = (*nt->mu_staco_nTRTHits)[i];
  track->_n_pixel_holes = (*nt->mu_staco_nPixHoles)[i];
  track->_n_sct_holes = (*nt->mu_staco_nSCTHoles)[i];
  track->_nhits = (*nt->mu_staco_nPixHits)[i] + (*nt->mu_staco_nSCTHits)[i] + (*nt->mu_staco_nTRTHits)[i]+ (*nt->mu_staco_nTRTOutliers)[i];
  track->_nmisses =  (*nt->mu_staco_nPixHoles)[i] +  (*nt->mu_staco_nSCTHoles)[i];
  track->_n_pixel_shared = (*nt->mu_staco_nPixSharedHits)[i];
  track->_n_sct_shared = (*nt->mu_staco_nSCTSharedHits)[i];
}

template<typename ntT> 
void
NtD3PDphysWZ::_make_associated_track_for_calo_muon( shared_ptr<AnaMuon>& mu , 
                                                    ntT nt , const unsigned int& i , 
                                                    const shared_ptr<const AnaVertex>& vx )
{
  if( nt->trkpt5_n == 0 ) { return; }
  int itrack = -1;
  AnaMuon::ftype dr = std::numeric_limits<AnaMuon::ftype>::max();
  for( int j=0, jf=nt->trkpt5_n; j!=jf; ++j ) { 
    const AnaMuon::ftype this_dr = detector::delta_eta_phi( (*nt->mu_calo_eta)[i] , (*nt->mu_calo_phi)[i] ,
                                                            (*(nt->trkpt5_eta))[j] , (*(nt->trkpt5_phi))[j] );
    if( this_dr < dr ) { 
      dr = this_dr;
      itrack = j;
    }
  }
  assert( itrack >= 0 );
  if( dr > 0.1 ) { return; }
  shared_ptr<AnaTrack> track( new AnaTrack );
  mu->_id_track = track;
  const AnaTrack::ftype trackpt = std::sin((*nt->trkpt5_theta)[itrack]) * std::abs( detector::safe_divide(static_cast<AnaTrack::ftype>(1),(*nt->trkpt5_qoverp)[itrack]) );
  track->_curv = detector::safe_divide(static_cast<AnaTrack::ftype>(1),trackpt)*detector::sign_of((*nt->trkpt5_qoverp)[itrack]); // 1/mev
  track->_d0 = (*nt->trkpt5_d0)[itrack];
  track->_phi0 = (*nt->trkpt5_phi)[itrack];
  track->_z0 = (*nt->trkpt5_z0)[itrack];
  track->_lambda = detector::cot((*nt->trkpt5_theta)[itrack]);
  // if( vx ) { 
  //   track->_exPV_d0 = detector::corrected_d0( track->_d0 , track->_z0 , track->_phi0 , detector::sign_of(track->_curv) , 
  //                                             vx->x() , vx->y() , vx->z() );
  //   track->_exPV_z0 = detector::corrected_z0( track->_d0 , track->_z0 , track->_phi0 , track->_lambda , detector::sign_of(track->_curv) , 
  //                                             vx->x() , vx->y() , vx->z() );
  //   track->_exPV_d0_err = detector::corrected_d0_err( (*(nt->mu_staco_trackcov_d0))[i] , vx->x_err() , vx->y_err() , track->_phi0 );
  //   track->_exPV_z0_err = detector::corrected_z0_err( (*(nt->mu_staco_trackcov_z0))[i] , vx->z_err() );
  // }
  track->_exPV_d0 = (*nt->trkpt5_d0_wrtPV)[itrack];
  track->_exPV_z0 = (*nt->trkpt5_z0_wrtPV)[itrack];
  track->_exPV_d0_err = (*nt->trkpt5_err_d0_wrtPV)[itrack];
  track->_exPV_z0_err = (*nt->trkpt5_err_z0_wrtPV)[itrack];
  // fill covariance matrix --- todo. see NtCBNT jacobian.
  // fit properties
  track->_chi2 = (*nt->trkpt5_chi2)[itrack];
  track->_ndof = (*nt->trkpt5_ndof)[itrack];
  track->_n_b_layer_hits = (*nt->trkpt5_nBLHits)[itrack];
  track->_n_pixel_hits = (*nt->trkpt5_nPixHits)[itrack];
  track->_n_sct_hits = (*nt->trkpt5_nSCTHits)[itrack];
  track->_n_trt_hits = (*nt->trkpt5_nTRTHits)[itrack];
  track->_n_pixel_holes = (*nt->trkpt5_nPixHoles)[itrack];
  track->_n_sct_holes = (*nt->trkpt5_nSCTHoles)[itrack];
  track->_nhits = (*nt->trkpt5_nPixHits)[itrack] + (*nt->trkpt5_nSCTHits)[itrack] + (*nt->trkpt5_nTRTHits)[itrack] + 0;
  track->_nmisses =  (*nt->trkpt5_nPixHoles)[itrack] +  (*nt->trkpt5_nSCTHoles)[itrack];
  track->_n_pixel_shared = 0;
  track->_n_sct_shared = 0;
}

template<typename ntT> 
void
NtD3PDphysWZ::_make_vertices( ntT nt )
{
  assert( nt );
  assert( nt->vxp_x->size() == nt->vxp_n );
  assert( nt->vxp_y->size() == nt->vxp_n );
  assert( nt->vxp_z->size() == nt->vxp_n );
  assert( nt->vxp_sumPt->size() == nt->vxp_n );
  assert( nt->vxp_nTracks->size() == nt->vxp_n );
  _vertices.reserve( static_cast<int>( (*nt).vxp_n ) );
  for( int iv=0, fv=nt->vxp_n; iv!=fv; ++iv ) {
    shared_ptr<AnaVertex> v( new AnaVertex );
    v->_x = (*(nt->vxp_x))[iv];
    v->_y = (*(nt->vxp_y))[iv];
    v->_z = (*(nt->vxp_z))[iv];
    v->_type = detector::remap_vertex_type_enum( (*(nt->vxp_type))[iv] );
    v->_sum_pt = (*(nt->vxp_sumPt))[iv]/1000.;
    v->_ntracks = static_cast<unsigned int>( (*(nt->vxp_nTracks))[iv] );
    _vertices.push_back( v );
  }  
}

template<typename ntT>
void
 NtD3PDphysWZ::_make_vertices_V5( ntT nt )
{
   assert( nt );
//+  assert( nt->vxp_x->size() == nt->vxp_n );
//+  assert( nt->vxp_y->size() == nt->vxp_n );
   assert( nt->vxp_z->size() == nt->vxp_n );
//+  assert( nt->vxp_sumPt->size() == nt->vxp_n );
   assert( nt->vxp_nTracks->size() == nt->vxp_n );
   _vertices.reserve( static_cast<int>( (*nt).vxp_n ) );
   for( int iv=0, fv=nt->vxp_n; iv!=fv; ++iv ) {
     shared_ptr<AnaVertex> v( new AnaVertex );
//+    v->_x = (*(nt->vxp_x))[iv];
//+    v->_y = (*(nt->vxp_y))[iv];
     v->_z = (*(nt->vxp_z))[iv];
//+    v->_x_err = (*(nt->vxp_cov_x))[iv]; // fixme: are these err or err^2?
//+    v->_y_err = (*(nt->vxp_cov_y))[iv];
//+    v->_z_err = (*(nt->vxp_cov_z))[iv];
//+    v->_type = detector::remap_vertex_type_enum( (*(nt->vxp_type))[iv] );
//+    v->_sum_pt = (*(nt->vxp_sumPt))[iv]/1000.;
     v->_ntracks = static_cast<unsigned int>( (*(nt->vxp_nTracks))[iv] );
     _vertices.push_back( v );
   }
}

template<> 
void
NtD3PDphysWZ::_make_vertices<const D3PDWZTree*>( const D3PDWZTree* nt )
{
  assert( nt );
  assert( nt->vxp_vertx->size() == nt->vxp_n );
  assert( nt->vxp_verty->size() == nt->vxp_n );
  assert( nt->vxp_vertz->size() == nt->vxp_n );
  assert( nt->vxp_vertSumPt->size() == nt->vxp_n );
  assert( nt->vxp_vertNtrk->size() == nt->vxp_n );
  _vertices.reserve( static_cast<int>( (*nt).vxp_n ) );
  for( int iv=0, fv=nt->vxp_n; iv!=fv; ++iv ) {
    shared_ptr<AnaVertex> v( new AnaVertex );
    v->_x = (*(nt->vxp_vertx))[iv];
    v->_y = (*(nt->vxp_verty))[iv];
    v->_z = (*(nt->vxp_vertz))[iv];
    v->_x_err = (*(nt->vxp_errx))[iv];
    v->_y_err = (*(nt->vxp_erry))[iv];
    v->_z_err = (*(nt->vxp_errz))[iv];
    v->_type = ATLAS::PriVtx; // Not in D3PDV1
    v->_sum_pt = (*(nt->vxp_vertSumPt))[iv]/1000.;
    v->_ntracks = static_cast<unsigned int>( (*(nt->vxp_vertNtrk))[iv] );
    _vertices.push_back( v );
  }
}

const bool
NtD3PDphysWZ::_get_event_V5() 
{

  const D3PDWZTreeV5* nt( dynamic_cast<const D3PDWZTreeV5*>(_madeclass.get()) );
  assert( nt );

  _run_number = nt->RunNumber;
  _event_number = nt->EventNumber;
  _lumi_block = nt->lbn;
  _mc_weight = nt->mcevt_n!=0 ? (*nt->mcevt_weight)[0][0] : 1. ;
  // fill vertices
  _make_vertices_V5( nt );
  // identify highest-sum-pt primary vertex for lepton track exPV d0/z0                                                                                                                                   
  shared_ptr<const AnaVertex> highest_sumpt_privertex;
  if( !_vertices.empty() ) {
    std::vector< shared_ptr<const AnaVertex> >::iterator i = std::max_element( _vertices.begin() , _vertices.end() ,
                                                                               boost::bind( &AnaVertex::sum_pt , _1 ) < boost::bind( &AnaVertex::sum_pt , _2 ) );
    assert( i!=_vertices.end() );
    highest_sumpt_privertex = *i;
  }


  // fill electrons
  assert( nt );
  assert( nt->el_author->size() == nt->el_n );
  assert( nt->el_charge->size() == nt->el_n );
  assert( nt->el_eta->size() == nt->el_n );
  assert( nt->el_phi->size() == nt->el_n );
  assert( nt->el_pt->size() == nt->el_n );
  assert( nt->el_isEM->size() == nt->el_n );
  assert( nt->el_cl_eta->size() == nt->el_n );
  assert( nt->el_cl_phi->size() == nt->el_n );
  assert( nt->el_cl_pt->size() == nt->el_n );
  assert( nt->el_cl_E->size() == nt->el_n );
  assert( nt->el_trackz0pv->size() == nt->el_n );
  assert( nt->el_trackd0pv->size() == nt->el_n );
  assert( nt->el_tracksigd0pv->size() == nt->el_n );
  assert( nt->el_ptcone20->size() == nt->el_n );
  assert( nt->el_ptcone30->size() == nt->el_n );
  assert( nt->el_ptcone40->size() == nt->el_n );
  assert( nt->el_Etcone20->size() == nt->el_n );
  assert( nt->el_Etcone30->size() == nt->el_n );
  assert( nt->el_Etcone40->size() == nt->el_n );
  for( int i=0, f=nt->el_n; i!=f; ++i ) { 
    shared_ptr<AnaElectron> ele( new AnaElectron );
    ele->_author = (*nt->el_author)[i];
    ele->_charge = (*nt->el_charge)[i];
    ele->_tight = (*nt->el_tight)[i];
    ele->_momentum.SetPtEtaPhiM( (*nt->el_pt)[i]/1000. , (*nt->el_eta)[i] , (*nt->el_phi)[i] , detector::electron_mass(detector::GeV) );
    ele->_is_em = (*nt->el_isEM)[i]; // FIXME
    ele->_oq = (*nt->el_goodOQ)[i] ? 0 : 1446; // FIXME: store full bitmask once it is in the ntuple
    ele->_pt_cone = boost::assign::list_of( -999. ) ( (*nt->el_ptcone20)[i]/1000. ) ( (*nt->el_ptcone30)[i]/1000. ) ( (*nt->el_ptcone40)[i]/1000. );
    ele->_et_cone = boost::assign::list_of( -999. ) ( (*nt->el_Etcone20)[i]/1000. ) ( (*nt->el_Etcone30)[i]/1000. ) ( (*nt->el_Etcone40)[i]/1000. );
    ele->_nhits = boost::assign::list_of ( (*nt->el_nPixHits)[i] ) ( (*nt->el_nSCTHits)[i] ) ( 0 ) ( 0 );
    ele->_cluster_momentum.SetPtEtaPhiE( (*nt->el_cl_pt)[i]/1000. , (*nt->el_cl_eta)[i] , (*nt->el_cl_phi)[i] , (*nt->el_cl_E)[i]/1000. );
    ele->_track_momentum.SetPtEtaPhiM( (*nt->el_trackpt)[i]/1000. , (*nt->el_tracketa)[i] , (*nt->el_trackphi)[i] , detector::electron_mass( detector::GeV ) );
    ele->_z0_exPV = (*nt->el_trackz0pv)[i];
    ele->_d0_exPV = (*nt->el_trackd0pv)[i];
    ele->_d0_exPV_err = (*nt->el_tracksigd0pv)[i];
    _electrons.push_back( ele );
  }
   
  // FIXME need to add photons to ntuple

  // fill muons
  assert( nt );
  _muons.reserve( static_cast<int>( nt->mu_staco_n ) );
  assert( nt->mu_staco_author->size() == nt->mu_staco_n );
  assert( nt->mu_staco_eta->size() == nt->mu_staco_n );
  assert( nt->mu_staco_phi->size() == nt->mu_staco_n );
  assert( nt->mu_staco_etcone20->size() == nt->mu_staco_n );
  assert( nt->mu_staco_ptcone20->size() == nt->mu_staco_n );
  assert( nt->mu_staco_ptcone30->size() == nt->mu_staco_n );
  assert( nt->mu_staco_ptcone40->size() == nt->mu_staco_n );
  assert( nt->mu_staco_id_z0_exPV->size() == nt->mu_staco_n );
  assert( nt->mu_staco_id_d0_exPV->size() == nt->mu_staco_n );
  assert( nt->mu_staco_id_cov_d0_exPV->size() == nt->mu_staco_n );
  for( int i=0, f=nt->mu_staco_n; i!=f; ++i ) { 
    shared_ptr<AnaMuon> mu( new AnaMuon );
    if( (*nt->mu_staco_isCombinedMuon)[i] ) { 
      mu->_all_author_bitmask.set( MuonAllAuthor::STACO );
      mu->_all_author_bitmask.set( MuonAllAuthor::MuonCombinedRefit );
    } else {
      mu->_all_author_bitmask.reset( MuonAllAuthor::STACO );
      mu->_all_author_bitmask.reset( MuonAllAuthor::MuonCombinedRefit );
    }
    mu->_primary_author = (*nt->mu_staco_author)[i];
    mu->_charge = (*nt->mu_staco_charge)[i];
    mu->_z0_exPV = (*nt->mu_staco_id_z0_exPV)[i];
    mu->_d0_exPV = (*nt->mu_staco_id_d0_exPV)[i];
    mu->_d0_exPV_err = (*nt->mu_staco_id_cov_d0_exPV)[i]; // FIXME: is this nt variable cov (in which case err = sqrt(cov)) or is it already sqrt(cov)?
    mu->_mu_phi = (*nt->mu_staco_phi)[i]; // FIXME
    mu->_expect_b_layer = (*nt->mu_staco_expectBLayerHit)[i];
    mu->_pt_cone = boost::assign::list_of( -999. ) ( (*nt->mu_staco_ptcone20)[i]/1000. ) ( (*nt->mu_staco_ptcone30)[i]/1000. ) ( (*nt->mu_staco_ptcone40)[i]/1000. );
    mu->_et_cone = boost::assign::list_of( -999. ) ( (*nt->mu_staco_etcone20)[i]/1000. ) ( -999. ) ( -999. );
    mu->_nhits = boost::assign::list_of ( (*nt->mu_staco_nPixHits)[i] )
      ( (*nt->mu_staco_nSCTHits)[i] )
      ( (*nt->mu_staco_nTRTHits)[i] )
      ( (*nt->mu_staco_nTRTOutliers)[i] )
      ( (*nt->mu_staco_nBLHits)[i] );
    mu->_nholes = boost::assign::list_of ( (*nt->mu_staco_nPixHoles)[i] )
      ( (*nt->mu_staco_nSCTHoles)[i] );
    mu->_ndead = boost::assign::list_of ( (*nt->mu_staco_nPixelDeadSensors)[i] )
      ( (*nt->mu_staco_nSCTDeadSensors)[i] )
      ( -999. );
    mu->_momentum.SetPtEtaPhiM( (*nt->mu_staco_pt)[i]/1000. ,
                                (*nt->mu_staco_eta)[i] ,
                                (*nt->mu_staco_phi)[i] ,
                                detector::muon_mass( detector::GeV ) );
    mu->_ms_muon.SetPtEtaPhiM( detector::ptFromChargeThetaQoverP( (*nt->mu_staco_charge)[i] ,
                                                                  (*nt->mu_staco_ms_theta)[i] , (*nt->mu_staco_ms_qoverp)[i] ) / 1000.,
                               detector::etaFromTheta( (*nt->mu_staco_ms_theta)[i] ),
                               (*nt->mu_staco_ms_phi)[i] ,
                               detector::muon_mass( detector::GeV ) );
    mu->_exms_muon.SetPtEtaPhiM( detector::ptFromChargeThetaQoverP( (*nt->mu_staco_charge)[i] ,
                                                                    (*nt->mu_staco_me_theta)[i] , (*nt->mu_staco_me_qoverp)[i] ) / 1000.,
                                 detector::etaFromTheta( (*nt->mu_staco_me_theta)[i] ),
                                 (*nt->mu_staco_me_phi)[i] ,
                                 detector::muon_mass( detector::GeV ) );
    mu->_id_muon.SetPtEtaPhiM( detector::ptFromChargeThetaQoverP( (*nt->mu_staco_charge)[i] ,
                                                                  (*nt->mu_staco_id_theta)[i] , (*nt->mu_staco_id_qoverp)[i] ) / 1000.,
                               detector::etaFromTheta( (*nt->mu_staco_id_theta)[i] ),
                               (*nt->mu_staco_id_phi)[i] ,
                               detector::muon_mass( detector::GeV ) );
    // some muons may not have ID tracks                                                                                                                                                               
    if( (*(nt->mu_staco_trackfitndof))[i] > 0 ) {
      _make_associated_track_for_staco_muon( mu , nt , i , highest_sumpt_privertex );
    }
    _muons.push_back( mu );
  }


  // fill jets
  assert( nt );
  // set up variables for MET clean cut (this is not used currently)
  bool met_is_good = false;
  // Anti-k_T cone 0.4 h1 topo
  //AntiKt4TopoEM
  assert( nt->jet_AntiKt4TopoEM_eta->size() == nt->jet_AntiKt4TopoEM_n );
  assert( nt->jet_AntiKt4TopoEM_phi->size() == nt->jet_AntiKt4TopoEM_n );
  assert( nt->jet_AntiKt4TopoEM_pt->size() == nt->jet_AntiKt4TopoEM_n );
  assert( nt->jet_AntiKt4TopoEM_m->size() == nt->jet_AntiKt4TopoEM_n );
  assert( nt->jet_AntiKt4TopoEM_emscale_eta->size() == nt->jet_AntiKt4TopoEM_n );
  assert( nt->jet_AntiKt4TopoEM_emscale_phi->size() == nt->jet_AntiKt4TopoEM_n );
  assert( nt->jet_AntiKt4TopoEM_emscale_pt->size() == nt->jet_AntiKt4TopoEM_n );
  assert( nt->jet_AntiKt4TopoEM_emscale_m->size() == nt->jet_AntiKt4TopoEM_n );
  _jets.reserve( static_cast<int>( (*nt).jet_AntiKt4TopoEM_n) );
  for( int i=0, f=nt->jet_AntiKt4TopoEM_n; i!=f; ++i ) {
    shared_ptr<AnaJet> jet( new AnaJet );
    jet->_alg = AnaJet::Algorithm::ATLAS_ANTI_KT_CONE_4_H1_TOPO;
    jet->_jet_index = i;
    jet->_vtx_fraction = (*nt->jet_AntiKt4TopoEM_jvtxf)[i];
    jet->_n90 = (*nt->jet_AntiKt4TopoEM_n90)[i];
    jet->_LArQuality = (*nt->jet_AntiKt4TopoEM_LArQuality)[i];
    jet->_HECQuality = (*nt->jet_AntiKt4TopoEM_HECQuality)[i];
    jet->_negativeE = (*nt->jet_AntiKt4TopoEM_NegativeE)[i]/1000.;
    jet->_sumPtTrack = (*nt->jet_AntiKt4TopoEM_sumPtTrk)[i]/1000.;
    jet->_emf = (*nt->jet_AntiKt4TopoEM_emfrac)[i];
    jet->_timing = (*nt->jet_AntiKt4TopoEM_Timing)[i];
    jet->_fmax = (*nt->jet_AntiKt4TopoEM_fracSamplingMax)[i];
    jet->_BCH_CORR_CELL = (*nt->jet_AntiKt4TopoEM_BCH_CORR_CELL)[i];
    jet->_hecf = (*nt->jet_AntiKt4TopoEM_hecf)[i];
    jet->_tgap3f = (*nt->jet_AntiKt4TopoEM_tgap3f)[i];
    jet->_full_scale_E = (*nt->jet_AntiKt4TopoEM_E)[i]/1000.;
    jet->_full_scale_momentum.SetPtEtaPhiM( (*nt->jet_AntiKt4TopoEM_pt)[i]/1000. , (*nt->jet_AntiKt4TopoEM_eta)[i] , (*nt->jet_AntiKt4TopoEM_phi)[i] , (*nt->jet_AntiKt4TopoEM_m)[i]/1000. );
    jet->_em_scale_momentum.SetPtEtaPhiM( (*nt->jet_AntiKt4TopoEM_emscale_pt)[i]/1000. , (*nt->jet_AntiKt4TopoEM_emscale_eta)[i] ,
                                          (*nt->jet_AntiKt4TopoEM_emscale_phi)[i] , (*nt->jet_AntiKt4TopoEM_emscale_m)[i]/1000. );
    jet->_EtaOrigin = (*nt->jet_AntiKt4TopoEM_EtaOrigin)[i];
    jet->_PhiOrigin = (*nt->jet_AntiKt4TopoEM_PhiOrigin)[i];
    jet->_MOrigin = (*nt->jet_AntiKt4TopoEM_MOrigin)[i]/1000.;
    jet->_is_bad_loose = (*nt->jet_AntiKt4TopoEM_isBadLoose)[i];
    jet->_em_scale_correction = (*nt->jet_AntiKt4TopoEM_EMJES)[i];
    jet->_momentum = jet->_full_scale_momentum;
    _jets.push_back( jet );
  }

  // fill met
  assert( nt );
  assert( _met.empty() );
  if( true ) { 
    shared_ptr<AnaMET> met( new AnaMET );
    met->_met_type = AnaMET::REF_FINAL;
    met->_is_good = met_is_good;
    met->_momentum.SetPtEtaPhiM( nt->MET_RefFinal_et/1000. , 0. , nt->MET_RefFinal_phi , 0. );
    _met.push_back( met );
  }
  if( true ) {
    shared_ptr<AnaMET> met( new AnaMET );
    met->_met_type = AnaMET::LOCHADTOPOMUHIGHETA;
    met->_MET_cal_etx = (nt->MET_LocHadTopo_et*std::cos(nt->MET_LocHadTopo_phi))/1000.;
    met->_MET_cal_ety = (nt->MET_LocHadTopo_et*std::sin(nt->MET_LocHadTopo_phi))/1000.;
    met->_MET_cal_etx_CentralReg = nt->MET_LocHadTopo_etx_CentralReg/1000.;
    met->_MET_cal_ety_CentralReg = nt->MET_LocHadTopo_ety_CentralReg/1000.;
    met->_MET_cal_etx_EndcapRegion = nt->MET_LocHadTopo_etx_EndcapRegion/1000.;
    met->_MET_cal_ety_EndcapRegion = nt->MET_LocHadTopo_ety_EndcapRegion/1000.;
    met->_MET_cal_etx_ForwardReg = nt->MET_LocHadTopo_etx_ForwardReg/1000.;
    met->_MET_cal_ety_ForwardReg = nt->MET_LocHadTopo_ety_ForwardReg/1000.;
    met->_MET_MuonBoy_etx = (nt->MET_MuonBoy_et*std::cos(nt->MET_MuonBoy_phi))/1000.;
    met->_MET_MuonBoy_ety = (nt->MET_MuonBoy_et*std::sin(nt->MET_MuonBoy_phi))/1000.;
    met->_MET_RefMuonTrack_etx = (nt->MET_RefMuon_Track_et*std::cos(nt->MET_RefMuon_Track_phi))/1000.;
    met->_MET_RefMuonTrack_ety = (nt->MET_RefMuon_Track_et*std::sin(nt->MET_RefMuon_Track_phi))/1000.;
    met->_is_good = met_is_good;
    met->_momentum.SetPtEtaPhiM( detector::quadrature( (nt->MET_LocHadTopo_etx_CentralReg + nt->MET_LocHadTopo_etx_ForwardReg + 
                                                        nt->MET_LocHadTopo_etx_EndcapRegion + 
                                                        ( nt->MET_MuonBoy_et * std::cos(nt->MET_MuonBoy_phi) ) - 
                                                        ( nt->MET_RefMuon_Track_et * std::cos(nt->MET_RefMuon_Track_phi) )  )/1000. ,
                                                       (nt->MET_LocHadTopo_ety_CentralReg + nt->MET_LocHadTopo_ety_ForwardReg +
                                                        nt->MET_LocHadTopo_ety_EndcapRegion + 
                                                        ( nt->MET_MuonBoy_et * std::sin(nt->MET_MuonBoy_phi) ) -
                                                        ( nt->MET_RefMuon_Track_et * std::sin(nt->MET_RefMuon_Track_phi) )  )/1000. ) ,
                                 0. ,
                                 detector::phiFromXY( (nt->MET_LocHadTopo_etx_CentralReg + nt->MET_LocHadTopo_etx_ForwardReg +
                                                       nt->MET_LocHadTopo_etx_EndcapRegion + 
                                                       ( nt->MET_MuonBoy_et * std::cos(nt->MET_MuonBoy_phi) ) -
                                                       ( nt->MET_RefMuon_Track_et * std::cos(nt->MET_RefMuon_Track_phi) )  )/1000. ,
                                                      (nt->MET_LocHadTopo_ety_CentralReg + nt->MET_LocHadTopo_ety_ForwardReg +
                                                       nt->MET_LocHadTopo_ety_EndcapRegion + 
                                                       ( nt->MET_MuonBoy_et * std::sin(nt->MET_MuonBoy_phi) ) -
                                                       ( nt->MET_RefMuon_Track_et * std::sin(nt->MET_RefMuon_Track_phi) )  )/1000. ) ,
                                 0. );
    _met.push_back( met );
  }
  if( true ) {
    shared_ptr<AnaMET> met( new AnaMET );
    met->_met_type = AnaMET::LOCHADTOPOELHIGHETA;
    met->_MET_cal_etx = (nt->MET_LocHadTopo_et*std::cos(nt->MET_LocHadTopo_phi))/1000.;
    met->_MET_cal_ety = (nt->MET_LocHadTopo_et*std::sin(nt->MET_LocHadTopo_phi))/1000.;
    met->_MET_cal_etx_CentralReg = nt->MET_LocHadTopo_etx_CentralReg/1000.;
    met->_MET_cal_ety_CentralReg = nt->MET_LocHadTopo_ety_CentralReg/1000.;
    met->_MET_cal_etx_EndcapRegion = nt->MET_LocHadTopo_etx_EndcapRegion/1000.;
    met->_MET_cal_ety_EndcapRegion = nt->MET_LocHadTopo_ety_EndcapRegion/1000.;
    met->_MET_cal_etx_ForwardReg = nt->MET_LocHadTopo_etx_ForwardReg/1000.;
    met->_MET_cal_ety_ForwardReg = nt->MET_LocHadTopo_ety_ForwardReg/1000.;
    met->_MET_MuonBoy_etx = (nt->MET_MuonBoy_et*std::cos(nt->MET_MuonBoy_phi))/1000.;
    met->_MET_MuonBoy_ety = (nt->MET_MuonBoy_et*std::sin(nt->MET_MuonBoy_phi))/1000.;
    met->_MET_RefMuonTrack_etx = (nt->MET_RefMuon_Track_et*std::cos(nt->MET_RefMuon_Track_phi))/1000.;
    met->_MET_RefMuonTrack_ety = (nt->MET_RefMuon_Track_et*std::sin(nt->MET_RefMuon_Track_phi))/1000.;
    met->_is_good = met_is_good;
    met->_momentum.SetPtEtaPhiM( detector::quadrature( (nt->MET_LocHadTopo_etx_CentralReg + nt->MET_LocHadTopo_etx_ForwardReg +
                                                        nt->MET_LocHadTopo_etx_EndcapRegion)/1000.,
                                                       (nt->MET_LocHadTopo_ety_CentralReg + nt->MET_LocHadTopo_ety_ForwardReg +
                                                        nt->MET_LocHadTopo_ety_EndcapRegion)/1000. ) ,
                                 0. ,
                                 detector::phiFromXY( (nt->MET_LocHadTopo_etx_CentralReg + nt->MET_LocHadTopo_etx_ForwardReg +
                                                       nt->MET_LocHadTopo_etx_EndcapRegion)/1000.,
                                                      (nt->MET_LocHadTopo_ety_CentralReg + nt->MET_LocHadTopo_ety_ForwardReg +
                                                       nt->MET_LocHadTopo_ety_EndcapRegion)/1000. ) ,
                                 0. );
    _met.push_back( met );
  }
  if( true ) {
    shared_ptr<AnaMET> met( new AnaMET );
    met->_met_type = AnaMET::LOCHADTOPOCEF;
    met->_MET_cal_etx_CentralReg = nt->MET_LocHadTopo_etx_CentralReg/1000.;
    met->_MET_cal_ety_CentralReg = nt->MET_LocHadTopo_ety_CentralReg/1000.;
    met->_MET_cal_etx_EndcapRegion = nt->MET_LocHadTopo_etx_EndcapRegion/1000.;
    met->_MET_cal_ety_EndcapRegion = nt->MET_LocHadTopo_ety_EndcapRegion/1000.;
    met->_MET_cal_etx_ForwardReg = nt->MET_LocHadTopo_etx_ForwardReg/1000.;
    met->_MET_cal_ety_ForwardReg = nt->MET_LocHadTopo_ety_ForwardReg/1000.;
    met->_MET_MuonBoy_etx = (nt->MET_MuonBoy_et*std::cos(nt->MET_MuonBoy_phi))/1000.;
    met->_MET_MuonBoy_ety = (nt->MET_MuonBoy_et*std::sin(nt->MET_MuonBoy_phi))/1000.;
    met->_MET_RefMuonTrack_etx = (nt->MET_RefMuon_Track_et*std::cos(nt->MET_RefMuon_Track_phi))/1000.;
    met->_MET_RefMuonTrack_ety = (nt->MET_RefMuon_Track_et*std::sin(nt->MET_RefMuon_Track_phi))/1000.;
    met->_MET_cal_etx = met->_MET_cal_etx_CentralReg + met->_MET_cal_etx_EndcapRegion + met->_MET_cal_etx_ForwardReg;
    met->_MET_cal_ety = met->_MET_cal_ety_CentralReg + met->_MET_cal_ety_EndcapRegion + met->_MET_cal_ety_ForwardReg;
    met->_is_good = met_is_good;
    met->_momentum.SetPtEtaPhiM( detector::quadrature( (nt->MET_LocHadTopo_etx_CentralReg + nt->MET_LocHadTopo_etx_ForwardReg +
                                                        nt->MET_LocHadTopo_etx_EndcapRegion)/1000.,
                                                       (nt->MET_LocHadTopo_ety_CentralReg + nt->MET_LocHadTopo_ety_ForwardReg +
                                                        nt->MET_LocHadTopo_ety_EndcapRegion)/1000. ) ,
                                 0. ,
                                 detector::phiFromXY( (nt->MET_LocHadTopo_etx_CentralReg + nt->MET_LocHadTopo_etx_ForwardReg +
                                                       nt->MET_LocHadTopo_etx_EndcapRegion)/1000.,
                                                      (nt->MET_LocHadTopo_ety_CentralReg + nt->MET_LocHadTopo_ety_ForwardReg +
                                                       nt->MET_LocHadTopo_ety_EndcapRegion)/1000. ) ,
                                 0. );
    _met.push_back( met );
  }
  // fill trigger decision
  assert( nt );
  _trigger.reset( new AnaTrigger );
  _trigger->_run_number = (nt->RunNumber);
  _trigger->_trig_ef_e15_medium = (nt->EF_e15_medium)!=0;
  _trigger->_trig_ef_e20_medium = (nt->EF_e20_medium)!=0;
  _trigger->_trig_ef_mu13_mg = (nt->EF_mu13_MG)!=0;
  _trigger->_trig_ef_mu13_mg_tight = (nt->EF_mu13_MG_tight)!=0;
  _trigger->_trig_ef_mu18_mg = (nt->EF_mu18_MG)!=0;
  _trigger->_trig_ef_mu40_msonly_barrel = (nt->EF_mu40_MSonly_barrel)!=0;
  _trigger->_trig_ef_mu10 = (nt->EF_mu10)!=0;
  _trigger->_trig_ef_mu10_mg = (nt->EF_mu10_MG)!=0;
  _trigger->_trig_l1_mu10 = (nt->L1_MU10)!=0;
  _trigger->_trig_l1_em10 = (nt->L1_EM10)!=0;
  _trigger->_trig_l1_em14 = (nt->L1_EM14)!=0;
  _trigger->_trig_ef_g20_loose = (nt->EF_g20_loose)!=0;
  _trigger->_lbn = nt->lbn;
  _trigger->_trig_EF_el_n = nt->trig_EF_el_n;
  _trigger->_trig_EF_el_EF_e20_medium = (*(nt->trig_EF_el_EF_e20_medium));
  _trigger->_trig_EF_el_eta = (*(nt->trig_EF_el_eta));
  _trigger->_trig_EF_el_phi = (*(nt->trig_EF_el_phi));
  _trigger->_trig_L2_combmuonfeature_L2_mu18_MG = (*(nt->trig_L2_combmuonfeature_L2_mu18_MG));
  _trigger->_trig_L2_combmuonfeature_n = nt->trig_L2_combmuonfeature_n ;
  _trigger->_trig_L2_combmuonfeature_pt = (*(nt->trig_L2_combmuonfeature_pt ));
  _trigger->_trig_L2_combmuonfeature_eta = (*(nt->trig_L2_combmuonfeature_eta ));
  _trigger->_trig_L2_combmuonfeature_phi = (*(nt->trig_L2_combmuonfeature_phi ));
  _trigger->_trig_EF_trigmugirl_n = nt->trig_EF_trigmugirl_n ;
  _trigger->_trig_EF_trigmugirl_track_n = (*(nt->trig_EF_trigmugirl_track_n ));
  _trigger->_trig_EF_trigmugirl_track_CB_pt = (*(nt->trig_EF_trigmugirl_track_CB_pt ));
  _trigger->_trig_EF_trigmugirl_track_CB_eta = (*(nt->trig_EF_trigmugirl_track_CB_eta ));
  _trigger->_trig_EF_trigmugirl_track_CB_phi = (*(nt->trig_EF_trigmugirl_track_CB_phi ));
  _trigger->_trig_L2_muonfeature_L2_mu40_MSonly_barrel = (*(nt->trig_L2_muonfeature_L2_mu40_MSonly_barrel ));
  _trigger->_trig_L2_muonfeature_n = nt->trig_L2_muonfeature_n ;
  _trigger->_trig_L2_muonfeature_pt = (*(nt->trig_L2_muonfeature_pt ));
  _trigger->_trig_L2_muonfeature_eta = (*(nt->trig_L2_muonfeature_eta ));
  _trigger->_trig_L2_muonfeature_phi = (*(nt->trig_L2_muonfeature_phi ));
  _trigger->_trig_EF_trigmuonef_n = nt->trig_EF_trigmuonef_n ;
  _trigger->_trig_EF_trigmuonef_track_n = (*(nt->trig_EF_trigmuonef_track_n ));
  _trigger->_trig_EF_trigmuonef_track_SA_pt = (*(nt->trig_EF_trigmuonef_track_SA_pt ));
  _trigger->_trig_EF_trigmuonef_track_SA_eta = (*(nt->trig_EF_trigmuonef_track_SA_eta ));
  _trigger->_trig_EF_trigmuonef_track_SA_phi = (*(nt->trig_EF_trigmuonef_track_SA_phi ));

  // fill truth particles
  assert( nt );
  _truth_particles.reserve( static_cast<int>( nt->mc_n ) );
  for( int imc=0, fmc=nt->mc_n; imc!=fmc; ++imc ) {
    shared_ptr<AnaTruthParticle> v( new AnaTruthParticle );
    v->_pdg_id = (*(nt->mc_pdgId))[imc];
    const vector<int>& parents( (*(nt->mc_parent_index))[imc] );
    const int jmc = parents.empty() ? -1 : parents.front(); // take first parent in list. see truth table for comprehensive parentage.
    v->_parent_pdg_id = jmc==-1 ? -1 : (*(nt->mc_pdgId))[jmc];
    v->_charge = detector::charge_from_pdg( (*nt->mc_pdgId)[imc] );
    v->_status = (*(nt->mc_status))[imc];
    v->_momentum.SetPtEtaPhiM( (*(nt->mc_pt))[imc]/1000. , (*(nt->mc_eta))[imc] , 
                               (*(nt->mc_phi))[imc] , (*(nt->mc_m))[imc]/1000. );
    _truth_particles.push_back( v );
  }

  // fill truth table
  if( true ) { // FIXME this section causes crashes when running over some backgrounds, can switch to false temporarily
    _truth_table.reset( new AnaTruthTable );
    std::map< int , std::string > table_entries; // for debugging only
    for( AnaTruthTable::index_type imc=0, fmc=nt->mc_n; imc!=fmc; ++imc ) {
      static TLorentzVector mom;
      mom.SetPtEtaPhiM( (*nt->mc_pt)[imc]/1000. , (*nt->mc_eta)[imc] , 
                        (*nt->mc_phi)[imc] , (*nt->mc_m)[imc]/1000. );
      std::vector<int> mos;
      std::vector<int> das;
      for( vector<int>::const_iterator im=(*nt->mc_parent_index)[imc].begin(), fm=(*nt->mc_parent_index)[imc].end(); im!=fm; ++im ) {
        assert( (*im)<nt->mc_n );
        mos.push_back( (*nt->mc_barcode)[ *im ] );
      }
      for( vector<int>::const_iterator im=(*nt->mc_child_index)[imc].begin(), fm=(*nt->mc_child_index)[imc].end(); im!=fm; ++im ) {
        assert( (*im)<nt->mc_n );
        das.push_back( (*nt->mc_barcode)[ *im ] );
      }
      _truth_table->add_particle( (*nt->mc_barcode)[imc] , 
                                  (*nt->mc_pdgId)[imc] , 
                                  boost::math::iround((*nt->mc_charge)[imc]) , 
                                  (*nt->mc_status)[imc] ,
                                  mom , 
                                  mos.begin() , mos.end() , das.begin() , das.end() );
      if( false ) { 
        // for debugging only. get mother and daughter barcodes
        /*table_entries[ barcode ] = */
        std::cout << ( boost::format( "%|5d| %|7t|%|5d| %|13t|%|9.2f| %|23t|%|9.2f| %|33t|%|9.2f| %|43t|%|9.2f| %|53t|%|4d| %|58t|%|6d| %|65t|%|6d| %|72t|%|6d| %|79t|%|6d|" )
                       % (*nt->mc_barcode)[imc] % (*nt->mc_pdgId)[imc] % mom.Pt() % (mom.Pt()>0. ? mom.Eta() : 0.) % mom.Phi() % mom.M() % (*nt->mc_status)[imc]
                       % (mos.empty() ? -1 : static_cast<int>(mos.front()))
                       % (mos.size()<2 ? -1 : static_cast<int>(mos[1]))
                       % (das.empty() ? -1 : static_cast<int>(das.front())) 
                       % (das.size()<2 ? -1 : static_cast<int>(das[1])) ).str()
                  << std::endl;
      }
    }
    // _truth_table->print_cdf_style( std::cout );
    // _truth_table->print_gprof_style( std::cout );
    // _truth_table->write_graphviz( "./graph_raw.dot" );
    _truth_table->finalize();
    if( false ) { // for debugging
      std::cout << " begin truth table for NtD3PDphysWZ " << endl;
      _truth_table->print_cdf_style( std::cout );
      static unsigned int iev=0;
      std::string filename = ( boost::format("graphviz%d.dot") % (iev++) ).str();
      _truth_table->write_graphviz( filename );
      cout << " bosons: " << endl;
      std::vector< shared_ptr<const AnaTruthParticle> > heavy_bosons( _truth_table->all_heavy_bosons< vector< shared_ptr<const AnaTruthParticle> > >() );
      BOOST_FOREACH( const shared_ptr<const AnaTruthParticle>& ptr , heavy_bosons ) {
        ptr->print( std::cout );
      }
      cout << " quarks_or_gluons: " << endl;
      std::vector< shared_ptr<const AnaTruthParticle> > quarks_or_gluons( _truth_table->all_quarks_or_gluons< vector< shared_ptr<const AnaTruthParticle> > >() );
      BOOST_FOREACH( const shared_ptr<const AnaTruthParticle>& ptr , quarks_or_gluons ) {
        ptr->print( std::cout );
      }
      cout << " final states: " << endl;
      std::vector< shared_ptr<const AnaTruthParticle> > final_states( _truth_table->all_final_states< vector< shared_ptr<const AnaTruthParticle> > >() );
      BOOST_FOREACH( const shared_ptr<const AnaTruthParticle>& ptr , final_states ) {
        ptr->print( std::cout );
      }
      std::cout << " end truth table for NtD3PDphysWZ " << endl;
    }
    // _truth_table->display_decay_tree();
  } // end fill truth

  return true;  
}
