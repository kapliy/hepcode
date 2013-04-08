
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
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/format.hpp>
#include <boost/interprocess/sync/file_lock.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/filesystem.hpp>
#include <TTree.h>
#include <TFile.h>
#include <TBranch.h>
#include <TROOT.h>
#include <TSystem.h>
#include <TMatrix.h>
#include "TrigFTKAna/Trees/NtCBNT.hpp"
#include "TrigFTKAna/AnaTrack.hpp"
#include "TrigFTKAna/AnaJet.hpp"
#include "TrigFTKAna/Detector.hpp"
#include "TrigFTKAna/UniqueBarcode.hpp"
#include "TrigFTKAna/ana_streams_helper.hpp"
#include "TrigFTKAna/Trees/WHTreeV13.h"
#include "TrigFTKAna/Trees/WHTreeV14.h"

using namespace std;

extern TROOT* gROOT;
extern TSystem* gSystem;

const bool
NtCBNT::begin_file( const std::string& raw_filename , const Mode& mode )
{
  // why oh why is this necessary? answer: it isn't, not yet anyway.
  //   gROOT->ProcessLine("#include <vector>"); 
  //   gROOT->ProcessLine("#include <utility>"); 
  const string filename = ana_streams::root_dcap_filename(raw_filename);

  // lock the file TrigFTKAna/.file_open_lock. this is a signal to all
  // other parallel jobs that we are about to attempt a file open, and
  // they should chill. the motivation for this (and the subsequent
  // random delay) is to reduce the peak load on dcache servers that
  // might otherwise get it with many file open requests
  // simultaneously.
  try { 
    boost::interprocess::file_lock lock( "./.file_open_lock" );
    boost::interprocess::scoped_lock<boost::interprocess::file_lock> sc_lock( lock );
    // wait for up to one second, using unistd.h usleep(microseconds).
    //  usleep( 999999*std::rand() );
    _file.reset( TFile::Open( filename.c_str() ) );
  } catch( ... ) {
    // do nothing and hope for the best.
    _file.reset( TFile::Open( filename.c_str() ) );
  }

  if( !_file ) { 
    cout << " NtCBNT: unable to open file " << filename << endl;
    return false;
  }
  assert( _file );

  TTree* tree = dynamic_cast<TTree*>(_file->Get("CollectionTree"));
  if( !tree ) { 
    cout << " NtCBNT: unable to find CollectionTree tree in file " << filename << endl;
    return false;
  }
  assert( tree );

  _mode = mode;
  if( _mode == MODE_V14 ) { 
    _madeclass_v14.reset( new WHTreeV14(tree) );
  } else if( _mode == MODE_V13 ) { 
    _madeclass_v13.reset( new WHTreeV13(tree) );
  } else {
    assert( !"unhandled mode" );
  }

  _n_events = static_cast<unsigned long>(tree->GetEntries());

  _open_filename = filename;
  return true;
}

const bool
NtCBNT::end_file()
{
  _n_events = 0;
  _open_filename = string();
  return true;
}

void
NtCBNT::set_enabled_branches()
{
}

void
NtCBNT::set_branch_addresses()
{
}


const bool
NtCBNT::get_event( const unsigned long& ievent )
{
  unsigned long nbytes;
  if( _mode==MODE_V14 && !_madeclass_v14 ) { return false; }
  else if( _mode==MODE_V13 && !_madeclass_v13 ) { return false; }
  if( _n_events == 0ul ) { return false; }
  if( ievent>=_n_events ) { 
    cout << "NtCBNT Warning: Tried to get event " << ievent << " with only " << _n_events << " events available." << endl;
    return false; 
  }

  if( _mode==MODE_V14 ) { 
    if( (nbytes = _madeclass_v14->LoadTree( ievent )) < 0 ) { 
      cout << "NtCBNT Warning: Event " << ievent << " LoadTree returned code " << nbytes << endl;
      return false; 
    }
  } else {
    if( (nbytes = _madeclass_v13->LoadTree( ievent )) < 0 ) { 
      cout << "NtCBNT Warning: Event " << ievent << " LoadTree returned code " << nbytes << endl;
      return false; 
    }
  }

  clear();

  WHTreeV13* nt13( _madeclass_v13.get() );
  WHTreeV14* nt14( _madeclass_v14.get() );
  assert( nt14 || nt13 );

  // load active branches
  if( nt14 ) { 
    if( (nbytes = nt14->GetEntry( ievent )) < 1 ) { 
      cout << "NtCBNT Warning: Event " << ievent << " GetEntry returned code " << nbytes << endl;
      return false;
    }
  } else if( nt13 ) {
    if( (nbytes = nt13->GetEntry( ievent )) < 1 ) { 
      cout << "NtCBNT Warning: Event " << ievent << " GetEntry returned code " << nbytes << endl;
      return false; 
    }
  } else {
    assert( !"unhandled" );
  }

  if( _mode==MODE_V14 ) { 
    return _get_event( nt14 );
  } else if( _mode==MODE_V13 ) { 
    return _get_event( nt13 );
  } else {
    assert( !"unhandled" );
  }
}

template<typename vntupleT>
const bool
NtCBNT::_get_event( const vntupleT* nt )
{
  _run_number = nt->RunNumber;
  _event_number = nt->EventNumber;
  
  // get true event primary vertex (highest sum pT if more than one)
  AnaTrack::ftype pvtx_x;
  AnaTrack::ftype pvtx_y;
  AnaTrack::ftype pvtx_z;
  if( !(nt->vxp_vtx_z_truth->empty()) ) {
    // is vtx 0 always the highest pT? (are these sorted?) no way to
    // tell without computing from the associated tracks because
    // the vertex momentum is not stored.
    pvtx_x = (*(nt->vxp_vtx_x_truth))[0];
    pvtx_y = (*(nt->vxp_vtx_y_truth))[0];
    pvtx_z = (*(nt->vxp_vtx_z_truth))[0];
  }


  // fill idscan tracks
  for( unsigned int i=0,f=nt->T2IdNtracks; i!=f ; ++i ) {
    if( (*nt->T2IdAlgo)[i] != 2 ) { continue; }
    // throw out absurdly low momentum tracks (see 20090220 logbook entry).
    if( std::abs( (*nt->T2IdPt)[i] ) < 1. ) { continue; } 
    // throw out gibberish track fits. 
    if( isnan(((*nt->T2IdPt)[i])) ) { continue; }
    // build track
    boost::shared_ptr<AnaTrack> trk( new AnaTrack );
    trk->_alg = AnaTrack::Algorithm::IDSCAN;
    trk->_d0 = (*nt->T2IdD0)[i];
    trk->_z0 = (*nt->T2IdZ0)[i];
    trk->_phi0 = (*nt->T2IdPhi0)[i];
    trk->_curv = 2.*detector::safe_divide(static_cast<float>(1),(*nt->T2IdPt)[i])*detector::sign_of((*nt->T2IdPt)[i]);
    trk->_lambda = detector::cotThetaFromEta((*nt->T2IdEta)[i]);
    trk->_chi2 = (*nt->T2IdChi2)[i];
    trk->_ndof = (*nt->T2IdNDoF)[i];
    trk->_nhits = (*nt->T2IdNSihits)[i] + (*nt->T2IdNTrthits)[i];
    trk->_cov_dd = detector::pow<2>( (*nt->T2IdErrD0)[i] );
    trk->_cov_zz = detector::pow<2>( (*nt->T2IdErrZ0)[i] );
    _l2_idscan_tracks.push_back( trk );
  }

  // Check for inconsistency indicating athena/CBNT error
  if( nt->Trk_totalNumTracks != nt->Trk_d0->size() ) {
    _inconsistentCBNT = true;
    return true; // don't return false since we just want to skip the event, not the file
  }

  // fill offline tracks
  for( unsigned int i=0,f=nt->Trk_totalNumTracks; i!=f ; ++i ) {
    boost::shared_ptr<AnaTrack> trk( new AnaTrack );
    trk->_alg = AnaTrack::Algorithm::OFFL;
    trk->_d0 = (*nt->Trk_d0)[i];
    trk->_z0 = (*nt->Trk_z0)[i];
    TVector3 mom3( (*nt->Trk_px)[i] , (*nt->Trk_py)[i] , (*nt->Trk_pz)[i] );
    trk->_phi0 = mom3.Phi();
    trk->_curv = detector::safe_divide(static_cast<Double_t>(1),mom3.Perp())*detector::sign_of((*nt->Trk_qOverP)[i]); // 1/mev
    trk->_lambda = detector::cot((*nt->Trk_theta)[i]);

    // Even if the container sizes are right, still need to check for
    // errors in low-multiplicity cases
    // Error manifests in several ways; one is by having inconsistent
    // eta from theta compared with the momentum vector

    // Use same matching criteria as parametersEqualV:
    // Note: This check depends on AnaTrack::eta() using the value
    // from AnaTrack::_lambda as set above
    {
      if( std::abs(trk->eta() - mom3.Eta()) > 0.01 ) {
        _inconsistentCBNT = true;
        return true; // don't return false since we just want to skip the event, not the file
      }
    }

    // emulate FTK by randomly throwing out tracks and smearing their d0 resolution
    // trk->_d0 = trk->_d0 + static_cast<AnaTrack::ftype>(gRandom->Gaus(0,0.030));
    // if( gRandom->Rndm()>=(0.976*0.9) ) {
    //   delete trk;
    //   continue;
    // }
    // covariances conventions as at
    //  http://alxr.usatlas.bnl.gov/lxr-stb3/source/atlas/Reconstruction/RecCBNT_Algs/CBNT_Particle/src/CBNTAA_TrackParticle.cxx?v=release_14_2_0
    // convention for covariance matrix indices 0-4:
    //  0=d0 1=z0 2=phi0 3=theta 4=qOverP
    // jacobian for {d0,z0,phi0,theta,qOverP} -> {d0,z0,phi0,lambda,qOverPt}
    //  from http://alxr.usatlas.bnl.gov/lxr-stb3/source/atlas/Reconstruction/Vertexing/CtvmftVertexFit/src/StCommonBlock.cxx#173
    //  cov.similarity(J) = J . cov . J^T
    TMatrixT<AnaTrack::ftype> covATLASbasis(5,5);
    TMatrixT<AnaTrack::ftype> jacobianATLAStoCDF(5,5);
    for( int ii=0; ii!=5; ++ii ) {
      for( int jj=0; jj!=5; ++jj ) {
        covATLASbasis(ii,jj) = ( (ii==jj) ? 1 : 0 );
        jacobianATLAStoCDF(ii,jj) = ( (ii==jj) ? 1 : 0 );
      }
    }
    covATLASbasis(0,0) = detector::pow<2>( (*nt->Trk_sigd0)[i] );
    covATLASbasis(1,1) = detector::pow<2>( (*nt->Trk_sigz0)[i] );
    covATLASbasis(2,2) = detector::pow<2>( (*nt->Trk_sigphi)[i] );
    covATLASbasis(3,3) = detector::pow<2>( (*nt->Trk_sigtheta)[i] );
    covATLASbasis(4,4) = detector::pow<2>( (*nt->Trk_sigqOverP)[i] );
    covATLASbasis(0,1) = covATLASbasis(1,0) = (*nt->Trk_covVert21)[i]; // z0 d0
    covATLASbasis(0,2) = covATLASbasis(2,0) = (*nt->Trk_covVert31)[i]; // phi0 d0
    covATLASbasis(0,3) = covATLASbasis(3,0) = (*nt->Trk_covVert41)[i]; // theta d0
    covATLASbasis(0,4) = covATLASbasis(4,0) = (*nt->Trk_covVert51)[i]; // qOverP d0 
    covATLASbasis(1,2) = covATLASbasis(2,1) = (*nt->Trk_covVert32)[i]; // phi0 z0
    covATLASbasis(1,3) = covATLASbasis(3,1) = (*nt->Trk_covVert42)[i]; // theta z0
    covATLASbasis(1,4) = covATLASbasis(4,1) = (*nt->Trk_covVert52)[i]; // qOverP z0 
    covATLASbasis(2,3) = covATLASbasis(3,2) = (*nt->Trk_covVert43)[i]; // theta phi0
    covATLASbasis(2,4) = covATLASbasis(4,2) = (*nt->Trk_covVert53)[i]; // qOverP phi0 
    covATLASbasis(3,4) = covATLASbasis(4,3) = (*nt->Trk_covVert54)[i]; // qOverP theta 
    // convert to CDF basis (theta->lambda=cot(theta),q/p->q/pt)
    jacobianATLAStoCDF(3,3) = -1. * detector::safe_divide( static_cast<float>(1) , detector::pow<2>(std::sin((*nt->Trk_theta)[i])) );
    jacobianATLAStoCDF(4,3) = -1. * ((*nt->Trk_sigqOverP)[i]) * detector::safe_divide( std::cos( (*nt->Trk_theta)[i] ) , detector::pow<2>(std::sin((*nt->Trk_theta)[i])) );
    jacobianATLAStoCDF(4,4) = detector::safe_divide( static_cast<float>(1) , std::sin((*nt->Trk_theta)[i]) );
    TMatrixT<AnaTrack::ftype> jacTranspose(5,5);
    jacTranspose.Transpose( jacobianATLAStoCDF );
    TMatrixT<AnaTrack::ftype> covCDFbasis = ( jacobianATLAStoCDF * covATLASbasis * jacTranspose );
    // transfer and convert units:
    //  pt: MeV->GeV
    //  d0,z0: mm -> cm
    trk->_cov_cc = covCDFbasis(4,4); // q/pt
    trk->_cov_dd = covCDFbasis(0,0); // d0
    trk->_cov_pp = covCDFbasis(2,2); // phi0
    trk->_cov_zz = covCDFbasis(1,1); // z0
    trk->_cov_tt = covCDFbasis(3,3); // cot(theta)
    trk->_cov_cd = covCDFbasis(4,0);
    trk->_cov_cp = covCDFbasis(4,2);
    trk->_cov_cz = covCDFbasis(4,1);
    trk->_cov_ct = covCDFbasis(4,3);
    trk->_cov_dp = covCDFbasis(2,0);
    trk->_cov_dz = covCDFbasis(1,0);
    trk->_cov_dt = covCDFbasis(3,0);
    trk->_cov_pz = covCDFbasis(2,1);
    trk->_cov_pt = covCDFbasis(3,2);
    trk->_cov_zt = covCDFbasis(3,1);
    if( true ) {
      // check some of the jacobian transformation results against another method.
      AnaTrack::ftype curv_err = std::abs( detector::safe_divide( static_cast<Double_t>((*nt->Trk_sigqOverP)[i]) * mom3.Mag() , mom3.Perp() ) ); /// TMP: REMOVE ME
      AnaTrack::ftype lambda_err = detector::safe_divide( detector::pow<2>(mom3.Mag()) , detector::pow<2>(mom3.Perp()) ) * ((*nt->Trk_sigtheta)[i]); // TMP: REMOVE ME
      // ( (*nt->Trk_sigd0)[i] );
      // ( (*nt->Trk_sigphi)[i] );
      // ( (*nt->Trk_sigz0)[i] );
      if( detector::safe_divide(std::abs(curv_err-std::sqrt(trk->_cov_cc)),curv_err) > 0.1 ) {
        cout << " jacobian transformation curvature uncertainty is wrong!: "
             << std::sqrt(trk->_cov_cc) << " is not " << curv_err
             << " for track with pt=" << trk->pt() << " GeV"
             << endl;
      }
      if( detector::safe_divide(std::abs(lambda_err-std::sqrt(trk->_cov_tt)),lambda_err) > 0.1 ) {
        cout << " jacobian transformation lambda uncertainty is wrong!: "
             << std::sqrt(trk->_cov_tt) << " is not " << lambda_err
             << endl;
      }
    }

    // other track fit properties
    trk->_chi2 = (*nt->Trk_Chi2)[i];
    trk->_ndof = (*nt->Trk_Ndf)[i];
    trk->_nhits = (*nt->Trk_numberOfPixelHits)[i] + (*nt->Trk_numberOfSCTHits)[i] + (*nt->Trk_numberOfTRTHits)[i];
    trk->_nmisses = (*nt->Trk_numberOfPixelHoles)[i] + (*nt->Trk_numberOfSCTHoles)[i];
    trk->_n_b_layer_hits = (*nt->Trk_numberOfBLayerHits)[i];
    trk->_n_pixel_hits = (*nt->Trk_numberOfPixelHits)[i];
    trk->_n_sct_hits = (*nt->Trk_numberOfSCTHits)[i];
    trk->_n_pixel_holes = (*nt->Trk_numberOfPixelHoles)[i];
    trk->_n_sct_holes = (*nt->Trk_numberOfSCTHoles)[i];
    trk->_n_pixel_shared = (*nt->Trk_numberOfPixelSharedHits)[i];
    trk->_n_sct_shared = (*nt->Trk_numberOfSCTSharedHits)[i];
    trk->_barcode = UniqueBarcode( 0 /* ? */ , (*nt->Trk_truthBarcode)[i] );
    trk->_barcode_matching_fraction = 2.;
    _offline_tracks.push_back( trk );
  }

  // build list of b, c, and light partons from higgs decay
  vector<TLorentzVector> b_partons;
  vector<TLorentzVector> c_partons;
  vector<TLorentzVector> light_partons;
  // build list of b, c, and light partons from anything (specifically
  // interested in b's from pileup here)
  vector<TLorentzVector> b_partons_any;
  vector<TLorentzVector> c_partons_any;
  vector<TLorentzVector> light_partons_any;
  
  // identify higgs decay channel.
  typedef enum { UNKNOWN=0 , LIGHT_QUARK , B_QUARK } HiggsDecayType;
  HiggsDecayType higgs_type = UNKNOWN;
  for( unsigned int i=0,f=nt->NPar; i!=f; ++i ) {
    int km = (*nt->KMothNt)[i];
    if( km<0 ) { continue; }
    if( (*nt->Type)[km]!=25 ) { continue; } // take higgs partons only
    if( (*nt->GenStat)[km]!=3 ) { continue; } // direct daughters of higgs, before corrections
    unsigned int abspdg = std::abs( (*nt->Type)[i] );
    if( abspdg == 5 ) { higgs_type = B_QUARK; }
    else if( abspdg==1 || abspdg==2 || abspdg==3 ) { higgs_type = LIGHT_QUARK; }
    else {
      cout << " NtCBNT: could not identify Higgs decay type " << abspdg << endl;
    }
    break;
  }
  if( higgs_type == UNKNOWN ) {
    cout << " NtCBNT: could not identify Higgs decay type " << endl;
  }

  // look for final, corrected partons for the higgs decay.
  //CollectionTree->Scan("Type:Type[KMothNt]:Type[KFDauNt]:Type[KLDauNt]:PhiGen:EtaGen:PtGen:GenStat:RVGen")
  for( unsigned int i=0,f=nt->NPar; i!=f; ++i ) {
    int km = (*nt->KMothNt)[i];
    if( km<0 ) { continue; }
    if( (*nt->GenStat)[km]!=2 ) { continue; } // corrected partons only
    if( std::abs( (*nt->PtGen)[i] ) < 10. ) { continue; } // drop unobservable partons (no chance of silicon charge deposition)
    if( (higgs_type==B_QUARK && std::abs( (*nt->Type)[i] )!=5 ) ||
        (higgs_type==LIGHT_QUARK && std::abs( (*nt->Type)[i] )>3 ) ||
        (higgs_type==UNKNOWN) ) { continue; } // ignore radiated partons of the incorrect type
    TLorentzVector p;
    p.SetPtEtaPhiM( (*nt->PtGen)[i] , (*nt->EtaGen)[i] , (*nt->PhiGen)[i]  ,(*nt->MGen)[i] );
    const unsigned int abspdg = std::abs( (*nt->Type)[i] );
    // const bool is_b = (jet->dr_b_parton()<0.1) && (jet->dr_light_parton()>0.1);
    // const bool is_light = (jet->dr_light_parton()<0.1) && (jet->dr_b_parton()>0.1);
    if( abspdg==1 || abspdg==2 || abspdg==3 ) {
      light_partons_any.push_back( p );
    } else if( abspdg==4 ) {
      c_partons_any.push_back( p );
    } else if( abspdg==5 ) {
      b_partons_any.push_back( p );
    }
    if( (*nt->Type)[km]!=25 ) { continue; } // take higgs partons only
    // cout << boost::format(" higgs parton: pt %|8f| eta %|8f| pdg %|8d| mom %|8d|") % (*nt->PtGen)[i] % (*nt->EtaGen)[i] % (*nt->Type)[i] % (*nt->Type)[km] << endl;
    if( abspdg==1 || abspdg==2 || abspdg==3 ) {
      light_partons.push_back( p );
    } else if( abspdg==4 ) {
      c_partons.push_back( p );
    } else if( abspdg==5 ) {
      b_partons.push_back( p );
    }
  }

  // fill l1 ROI jets
  for( int i=0,f=nt->L1Jet_nRoI; i!=f; ++i ) {
    boost::shared_ptr<AnaJet> jet( new AnaJet );
    const AnaJet::ftype jet_et = (*nt->L1Jet_ET8x8)[i]/1000.;
    const AnaJet::ftype jet_eta = (*nt->L1Jet_eta)[i];
    //AnaJet::ftype energy = jet_et * detector::csc( detector::thetaFromEta( jet_eta ) );
    jet->_momentum.SetPtEtaPhiM( jet_et , jet_eta , (*nt->L1Jet_phi)[i] , 0. );
    jet->_alg = AnaJet::Algorithm::L1_ROI;
    // match partons
    BOOST_FOREACH( const TLorentzVector& p , b_partons ) {
      const AnaJet::ftype dr = std::max( std::abs(static_cast<AnaJet::ftype>(p.Eta())-jet->eta()) , 
                                         std::abs( detector::delta_phi(static_cast<AnaJet::ftype>(p.Phi()),jet->phi()) ) );
      jet->_dr_b_parton = std::min( jet->_dr_b_parton , dr );
    }
    BOOST_FOREACH( const TLorentzVector& p , c_partons ) {
      const AnaJet::ftype dr = std::max( std::abs(static_cast<AnaJet::ftype>(p.Eta())-jet->eta()) , 
                                         std::abs( detector::delta_phi(static_cast<AnaJet::ftype>(p.Phi()),jet->phi()) ) );
      jet->_dr_c_parton = std::min( jet->_dr_c_parton , dr );
    }
    BOOST_FOREACH( const TLorentzVector& p , light_partons ) {
      const AnaJet::ftype dr = std::max( std::abs(static_cast<AnaJet::ftype>(p.Eta())-jet->eta()) , 
                                         std::abs( detector::delta_phi(static_cast<AnaJet::ftype>(p.Phi()),jet->phi()) ) );
      jet->_dr_light_parton = std::min( jet->_dr_light_parton , dr );
    }

    BOOST_FOREACH( const TLorentzVector& p , b_partons_any ) {
      const AnaJet::ftype dr = std::max( std::abs(static_cast<AnaJet::ftype>(p.Eta())-jet->eta()) , 
                                         std::abs( detector::delta_phi(static_cast<AnaJet::ftype>(p.Phi()),jet->phi()) ) );
      jet->_dr_b_parton_any = std::min( jet->_dr_b_parton_any , dr );
    }
    BOOST_FOREACH( const TLorentzVector& p , c_partons_any ) {
      const AnaJet::ftype dr = std::max( std::abs(static_cast<AnaJet::ftype>(p.Eta())-jet->eta()) , 
                                         std::abs( detector::delta_phi(static_cast<AnaJet::ftype>(p.Phi()),jet->phi()) ) );
      jet->_dr_c_parton_any = std::min( jet->_dr_c_parton_any , dr );
    }
    BOOST_FOREACH( const TLorentzVector& p , light_partons_any ) {
      const AnaJet::ftype dr = std::max( std::abs(static_cast<AnaJet::ftype>(p.Eta())-jet->eta()) , 
                                         std::abs( detector::delta_phi(static_cast<AnaJet::ftype>(p.Phi()),jet->phi()) ) );
      jet->_dr_light_parton_any = std::min( jet->_dr_light_parton_any , dr );
    }
    // try to find matching l2 and ef jets
    int il2_best = -1;
    int ief_best = -1;
    double rl2_best = 9999;
    double ref_best = 9999;
    for( int il=0, fl=nt->T2BjN; il!=fl; ++il ) {
      const AnaJet::ftype dr = std::max( std::abs(static_cast<AnaJet::ftype>((*(nt->T2BjEta))[il]) - jet->eta()) , 
                                         std::abs( detector::delta_phi(static_cast<AnaJet::ftype>((*(nt->T2BjPhi0))[il]),jet->phi()) ) );
      if( dr < rl2_best ) { 
        rl2_best = dr;
        il2_best = il;
      }
    }
    for( int il=0, fl=nt->EFBjN; il!=fl; ++il ) {
      const AnaJet::ftype dr = std::max( std::abs(static_cast<AnaJet::ftype>((*(nt->EFBjEta))[il]) - jet->eta()) , 
                                         std::abs( detector::delta_phi(static_cast<AnaJet::ftype>((*(nt->EFBjPhi0))[il]),jet->phi()) ) );
      if( dr < ref_best ) { 
        ref_best = dr;
        ief_best = il;
      }
    }
    //
    if( _mode==MODE_V14 ) { 
      const WHTreeV14* nt14( reinterpret_cast<const WHTreeV14*>(nt) ); 
      if( il2_best>=0 && ref_best<0.1 ) {
        jet->_tag_l2_xd0 = (*(nt14->T2BjXIP1D))[il2_best];
        jet->_tag_l2_xz0 = (*(nt14->T2BjXIP2D))[il2_best];
        jet->_tag_l2_x2d = (*(nt14->T2BjXIP3D))[il2_best];
      }
      if( ief_best>=0 && ref_best<0.1 ) {
        jet->_tag_ef_xd0 = (*(nt14->EFBjXIP1D))[ief_best];
        jet->_tag_ef_xz0 = (*(nt14->EFBjXIP2D))[ief_best];
        jet->_tag_ef_x2d = (*(nt14->EFBjXIP3D))[ief_best];
      }
    } else if( _mode==MODE_V13 ) {
      const WHTreeV13* nt13( reinterpret_cast<const WHTreeV13*>(nt) ); 
      if( il2_best>=0 && ref_best<0.1 ) {
        jet->_tag_l2_xd0 = (*(nt13->T2BjXd0))[il2_best];
        jet->_tag_l2_xz0 = (*(nt13->T2BjXz0))[il2_best];
        jet->_tag_l2_x2d = (*(nt13->T2BjX2d))[il2_best];
      }
      if( ief_best>=0 && ref_best<0.1 ) {
        jet->_tag_ef_xd0 = (*(nt13->EFBjXd0))[ief_best];
        jet->_tag_ef_xz0 = (*(nt13->EFBjXz0))[ief_best];
        jet->_tag_ef_x2d = (*(nt13->EFBjX2d))[ief_best];
      }
    } else {
      assert( !"unhandled ntuple version" );
    }
    _l1_roi_jets.push_back( jet );
  } // end fill L1 jets

  // fill atlas cone 4 jets
  _hadrons_by_index.clear();
  set<unsigned int> hadrons_done;
  if( _mode==MODE_V14 ) { 
    const WHTreeV14* nt14( reinterpret_cast<const WHTreeV14*>(nt) );
    // build list of all hadrons for jet association
    //    CollectionTree->Scan("Type:PhiGen:EtaGen:PtGen:GenStat:RVGen:Type[KMothNt]", "GenStat==1 && (((abs(Type[KMothNt])>500 && abs(Type[KMothNt])<599) || (abs(Type[KMothNt])>5000 && abs(Type[KMothNt])<5999)))")
    for( unsigned int i=0,f=nt14->NPar; i!=f; ++i ) {
      if( std::abs( (*nt->PtGen)[i] ) < 10. ) { continue; } // drop unobservables
      //if( std::abs( (*nt->GenStat)[i] ) != 2 ) { continue; } 
      const int pdg_code( (*nt->Type)[i] );
      int immediate_iparent = (*nt->KMothNt)[i];
      if( immediate_iparent < 0 ) { continue; }
      const int parent_pdg_code( (*nt->Type)[immediate_iparent] );
      // if this is not a displaced hadron, forget it.
      bool is_b = detector::is_b_hadron( parent_pdg_code );
      if( !is_b ) { continue; }
      if( detector::is_b_hadron( pdg_code ) ) { continue; } // want b decay products, not de-excited state.
      // bool is_b = detector::is_b_hadron( pdg_code );
      // bool is_c = detector::is_c_hadron( pdg_code );
      // bool is_light = detector::is_light_hadron( pdg_code );
      // if( !is_b && !is_c && !is_light ) { continue; }
      //if( std::abs( (*nt->RVGen)[i] ) < 0.00
      // figure out whether ultimate parent is a higgs, and if so then
      // record the hadron.
      int iparent = i;
      unsigned int iterations = 0u;
      while( iparent>=0 && iparent<nt14->NPar && (iterations++)<20 ) {
        const int this_pdg_code( (*nt->Type)[iparent] );
        const int this_stat( (*nt->GenStat)[iparent] );
        const int this_parent( (*nt->KMothNt)[iparent] );
        if( this_pdg_code==25 ) {
          iparent = this_parent;
          break;
        }
        if( this_parent>=0 && this_parent<nt14->NPar ) {
          iparent = this_parent;
          continue;
        }
        iparent = -1;
        break;
      }
      // if this isn't a higgs decay product, forget it.
      if( iparent<0 ) { continue; }
      // 
      if( hadrons_done.find(immediate_iparent)!=hadrons_done.end() ) { continue; }
      hadrons_done.insert( immediate_iparent );
      // encode parent
      const AnaJet::ftype eta( (*nt->EtaGen)[immediate_iparent] );
      const AnaJet::ftype phi( (*nt->PhiGen)[immediate_iparent] );
      const AnaJet::ftype rvgen( (*nt->RVGen)[i] );
      const AnaJet::ftype phivgen( (*nt->PhiVGen)[i] );
      const AnaJet::ftype zvgen( (*nt->ZVGen)[i] );
      const AnaJet::ftype xsv( rvgen * std::cos(phivgen) );
      const AnaJet::ftype ysv( rvgen * std::sin(phivgen) );
      const AnaJet::ftype zsv( zvgen );
      AnaJet::Hadron::Type type = AnaJet::Hadron::UNKNOWN;
      type = AnaJet::Hadron::B_QUARK;
      // if( is_b ) { type = AnaJet::Hadron::B_QUARK; }
      // else if( is_c ) { type = AnaJet::Hadron::C_QUARK; }
      // else if( is_light ) { type = AnaJet::Hadron::LIGHT_QUARK; }
      // else { assert( false ); }
      _hadrons_by_index[ i ] = AnaJet::Hadron( type , pdg_code  , xsv , ysv , zsv , eta , phi , 0);
      if( false ) { 
        cout << " added hadron: type=" << type
             << " pdg code=" << pdg_code
             << " sv " << xsv << " " << ysv << " " << zsv
             << " eta " << eta << " phi " << phi
             << endl;
      }
    }
    
    // process all jets.
    for( unsigned int jet_type=0; jet_type!=4; ++jet_type ) {
      std::vector<double>& vec_jeteta( (*nt14->jetEtaCone4H1TowerJets) );
      std::vector<double>& vec_jetphi( (*nt14->jetPhiCone4H1TowerJets) );
      std::vector<double>& vec_jetet( (*nt14->jetEtCone4H1TowerJets) );
      std::vector<double>& vec_jetemf( (*nt14->jetEmfCone4H1TowerJets) );
      unsigned int vec_jet_size( nt14->jetNumCone4H1TowerJets );
      AnaJet::Algorithm jet_alg( AnaJet::Algorithm::ATLAS_CONE_4_TOWER );
      std::vector< boost::shared_ptr<const AnaJet> >* output_jets( &_atlas_cone_4_tower_jets );
      switch( jet_type ) {
      case 0:
        {
          vec_jeteta = (*nt14->jetEtaCone4H1TowerJets);
          vec_jetphi = (*nt14->jetPhiCone4H1TowerJets);
          vec_jetet = (*nt14->jetEtCone4H1TowerJets);
          vec_jetemf = (*nt14->jetEmfCone4H1TowerJets);
          vec_jet_size = nt14->jetNumCone4H1TowerJets;
          jet_alg = AnaJet::Algorithm::ATLAS_CONE_4_TOWER;
          output_jets = &_atlas_cone_4_tower_jets;
        }
        break;
      case 1:
        {
          vec_jeteta = (*nt14->jetEtaCone7H1TowerJets);
          vec_jetphi = (*nt14->jetPhiCone7H1TowerJets);
          vec_jetet = (*nt14->jetEtCone7H1TowerJets);
          vec_jetemf = (*nt14->jetEmfCone7H1TowerJets);
          vec_jet_size = nt14->jetNumCone7H1TowerJets;
          jet_alg = AnaJet::Algorithm::ATLAS_CONE_7_TOWER;
          output_jets = &_atlas_cone_7_tower_jets;
        }
        break;
      case 2:
        {
          vec_jeteta = (*nt14->jetEtaCone4TruthJets);
          vec_jetphi = (*nt14->jetPhiCone4TruthJets);
          vec_jetet = (*nt14->jetEtCone4TruthJets);
          vec_jetemf = (*nt14->jetEmfCone4TruthJets);
          vec_jet_size = nt14->jetNumCone4TruthJets;
          jet_alg = AnaJet::Algorithm::ATLAS_CONE_4_TRUTH;
          output_jets = &_atlas_cone_4_truth_jets;
        }
        break;
      case 3:
        {
          vec_jeteta = (*nt14->jetEtaCone4TruthPileupJets);
          vec_jetphi = (*nt14->jetPhiCone4TruthPileupJets);
          vec_jetet = (*nt14->jetEtCone4TruthPileupJets);
          vec_jetemf = (*nt14->jetEmfCone4TruthPileupJets);
          vec_jet_size = nt14->jetNumCone4TruthPileupJets;
          jet_alg = AnaJet::Algorithm::ATLAS_CONE_4_TRUTH_PILEUP;
          output_jets = &_atlas_cone_4_truth_pileup_jets;
        }
        break;
      default:
        assert( !"unhandled jet type" );
      }
      for( unsigned int i=0,f=vec_jet_size; i!=f; ++i ) {
        boost::shared_ptr<AnaJet> jet( new AnaJet );
        // FIXME: turn on jet mass ntuple variable and include that
        // instead of declaring massless jets.
        // jet->eta() = vec_jeteta[i];
        // jet->phi() = vec_jetphi[i];
        // jet->_et = vec_jetet[i]/1000.;
        jet->_momentum.SetPtEtaPhiM( vec_jetet[i]/1000. , vec_jeteta[i] , vec_jetphi[i] , 0. );
        jet->_emf = vec_jetemf[i];
        jet->_alg = jet_alg;
        jet->_primary_vertex_x = pvtx_x;
        jet->_primary_vertex_y = pvtx_y;
        jet->_primary_vertex_z = pvtx_z;
        // associate hadrons with jets
        for( HadronMap::const_iterator ihad=_hadrons_by_index.begin(), fhad=_hadrons_by_index.end(); ihad!=fhad; ++ihad ) {
          const AnaJet::ftype dr = std::max( std::abs(static_cast<AnaJet::ftype>(ihad->second.eta())-jet->eta()) , 
                                             std::abs( detector::delta_phi(static_cast<AnaJet::ftype>(ihad->second.phi()),jet->phi()) ) );
          if( dr < 0.4 ) {
            jet->_associated_hadrons.push_back( ihad->second );
          }
        }
        // fill parton dr variables.
        double best_parton_pt = 0.;
        double best_parton_eta;
        double best_parton_phi;
        int best_parton_pdg;
        BOOST_FOREACH( const TLorentzVector& p , b_partons ) {
          const AnaJet::ftype dr = std::max( std::abs(static_cast<AnaJet::ftype>(p.Eta())-jet->eta()) , 
                                             std::abs( detector::delta_phi(static_cast<AnaJet::ftype>(p.Phi()),jet->phi()) ) );
          jet->_dr_b_parton = std::min( jet->_dr_b_parton , dr );
          if( p.Pt() > best_parton_pt ) {
            best_parton_pt = p.Pt();
            best_parton_eta = best_parton_eta;
            best_parton_phi = best_parton_phi;
            best_parton_pdg = 5;
          }
        }
        BOOST_FOREACH( const TLorentzVector& p , c_partons ) {
          const AnaJet::ftype dr = std::max( std::abs(static_cast<AnaJet::ftype>(p.Eta())-jet->eta()) , 
                                             std::abs( detector::delta_phi(static_cast<AnaJet::ftype>(p.Phi()),jet->phi()) ) );
          jet->_dr_c_parton = std::min( jet->_dr_c_parton , dr );
          if( p.Pt() > best_parton_pt ) {
            best_parton_pt = p.Pt();
            best_parton_eta = best_parton_eta;
            best_parton_phi = best_parton_phi;
            best_parton_pdg = 4;
          }
        }
        BOOST_FOREACH( const TLorentzVector& p , light_partons ) {
          const AnaJet::ftype dr = std::max( std::abs(static_cast<AnaJet::ftype>(p.Eta())-jet->eta()) , 
                                             std::abs( detector::delta_phi(static_cast<AnaJet::ftype>(p.Phi()),jet->phi()) ) );
          jet->_dr_light_parton = std::min( jet->_dr_light_parton , dr );
          if( p.Pt() > best_parton_pt ) {
            best_parton_pt = p.Pt();
            best_parton_eta = best_parton_eta;
            best_parton_phi = best_parton_phi;
            best_parton_pdg = 2;
          }
        }
        BOOST_FOREACH( const TLorentzVector& p , b_partons_any ) {
          const AnaJet::ftype dr = std::max( std::abs(static_cast<AnaJet::ftype>(p.Eta())-jet->eta()) , 
                                             std::abs( detector::delta_phi(static_cast<AnaJet::ftype>(p.Phi()),jet->phi()) ) );
          jet->_dr_b_parton_any = std::min( jet->_dr_b_parton_any , dr );
        }
        BOOST_FOREACH( const TLorentzVector& p , c_partons_any ) {
          const AnaJet::ftype dr = std::max( std::abs(static_cast<AnaJet::ftype>(p.Eta())-jet->eta()) , 
                                             std::abs( detector::delta_phi(static_cast<AnaJet::ftype>(p.Phi()),jet->phi()) ) );
          jet->_dr_c_parton_any = std::min( jet->_dr_c_parton_any , dr );
        }
        BOOST_FOREACH( const TLorentzVector& p , light_partons_any ) {
          const AnaJet::ftype dr = std::max( std::abs(static_cast<AnaJet::ftype>(p.Eta())-jet->eta()) , 
                                             std::abs( detector::delta_phi(static_cast<AnaJet::ftype>(p.Phi()),jet->phi()) ) );
          jet->_dr_light_parton_any = std::min( jet->_dr_light_parton_any , dr );
        }
        // find eta,phi of highest pt parton within 0.4 of jet centroid
        if( best_parton_pt > 1. ) { 
          jet->_eta_highest_pt_parton = best_parton_eta;
          jet->_phi_highest_pt_parton = best_parton_phi;
        }
        // try to find matching l2 and ef jets
        int il2_best = -1;
        int ief_best = -1;
        double rl2_best = 9999;
        double ref_best = 9999;
        for( int il=0, fl=nt14->T2BjN; il!=fl; ++il ) {
          const AnaJet::ftype dr = std::max( std::abs(static_cast<AnaJet::ftype>((*(nt14->T2BjEta))[il]) - jet->eta()) , 
                                             std::abs( detector::delta_phi(static_cast<AnaJet::ftype>((*(nt14->T2BjPhi0))[il]),jet->phi()) ) );
          if( dr < rl2_best ) { 
            rl2_best = dr;
            il2_best = il;
          }
        }
        for( int il=0, fl=nt14->EFBjN; il!=fl; ++il ) {
          const AnaJet::ftype dr = std::max( std::abs(static_cast<AnaJet::ftype>((*(nt14->EFBjEta))[il]) - jet->eta()) , 
                                             std::abs( detector::delta_phi(static_cast<AnaJet::ftype>((*(nt14->EFBjPhi0))[il]),jet->phi()) ) );
          if( dr < ref_best ) { 
            ref_best = dr;
            ief_best = il;
          }
        }
        if( il2_best>=0 && ref_best<0.1 ) {
          jet->_tag_l2_xd0 = (*(nt14->T2BjXIP1D))[il2_best];
          jet->_tag_l2_xz0 = (*(nt14->T2BjXIP2D))[il2_best];
          jet->_tag_l2_x2d = (*(nt14->T2BjXIP3D))[il2_best];
        }
        if( ief_best>=0 && ref_best<0.1 ) {
          jet->_tag_ef_xd0 = (*(nt14->EFBjXIP1D))[ief_best];
          jet->_tag_ef_xz0 = (*(nt14->EFBjXIP2D))[ief_best];
          jet->_tag_ef_x2d = (*(nt14->EFBjXIP3D))[ief_best];
        }
        output_jets->push_back( jet );
        //     cout << boost::format(" atlas cone jet: %|10g|(GeV) %|10g| %|10g| %|6g| %|6g| %|6g| %|6d| %|6d| %|6d|")
        //       % jet->_et % jet->eta() % jet->phi()
        //       % jet->_dr_b_parton % jet->_dr_c_parton % jet->_dr_light_parton
        //       % b_partons.size() % c_partons.size() % light_partons.size()
        //          << endl;
      } // end fill jets for this algorithm
      assert( output_jets->size() == vec_jet_size ); // should be true unless you insert code to skip some jets above.
    } // end for each algorithm
  } else if( _mode==MODE_V13 ) { 
    const WHTreeV13* nt13( reinterpret_cast<const WHTreeV13*>(nt) );
    for( unsigned int i=0,f=nt13->jetNumC4Jets; i!=f; ++i ) {
      boost::shared_ptr<AnaJet> jet( new AnaJet );
      jet->_momentum.SetPtEtaPhiM( (*nt13->jetEtC4Jets)[i]/1000. , (*nt13->jetEtaC4Jets)[i] , (*nt13->jetPhiC4Jets)[i] , 0. );
      jet->_alg = AnaJet::Algorithm::ATLAS_CONE_4_TOWER;
      BOOST_FOREACH( const TLorentzVector& p , b_partons ) {
        const AnaJet::ftype dr = std::max( std::abs(static_cast<AnaJet::ftype>(p.Eta())-jet->eta()) , 
                                           std::abs( detector::delta_phi(static_cast<AnaJet::ftype>(p.Phi()),jet->phi()) ) );
        jet->_dr_b_parton = std::min( jet->_dr_b_parton , dr );
      }
      BOOST_FOREACH( const TLorentzVector& p , c_partons ) {
        const AnaJet::ftype dr = std::max( std::abs(static_cast<AnaJet::ftype>(p.Eta())-jet->eta()) , 
                                           std::abs( detector::delta_phi(static_cast<AnaJet::ftype>(p.Phi()),jet->phi()) ) );
        jet->_dr_c_parton = std::min( jet->_dr_c_parton , dr );
      }
      BOOST_FOREACH( const TLorentzVector& p , light_partons ) {
        const AnaJet::ftype dr = std::max( std::abs(static_cast<AnaJet::ftype>(p.Eta())-jet->eta()) , 
                                           std::abs( detector::delta_phi(static_cast<AnaJet::ftype>(p.Phi()),jet->phi()) ) );
        jet->_dr_light_parton = std::min( jet->_dr_light_parton , dr );
      }
      // try to find matching l2 and ef jets
      int il2_best = -1;
      int ief_best = -1;
      double rl2_best = 9999;
      double ref_best = 9999;
      for( int il=0, fl=nt13->T2BjN; il!=fl; ++il ) {
        const AnaJet::ftype dr = std::max( std::abs(static_cast<AnaJet::ftype>((*(nt13->T2BjEta))[il]) - jet->eta()) , 
                                           std::abs( detector::delta_phi(static_cast<AnaJet::ftype>((*(nt13->T2BjPhi0))[il]),jet->phi()) ) );
        if( dr < rl2_best ) { 
          rl2_best = dr;
          il2_best = il;
        }
      }
      for( int il=0, fl=nt13->EFBjN; il!=fl; ++il ) {
        const AnaJet::ftype dr = std::max( std::abs(static_cast<AnaJet::ftype>((*(nt13->EFBjEta))[il]) - jet->eta()) , 
                                           std::abs( detector::delta_phi(static_cast<AnaJet::ftype>((*(nt13->EFBjPhi0))[il]),jet->phi()) ) );
        if( dr < ref_best ) { 
          ref_best = dr;
          ief_best = il;
        }
      }
      if( il2_best>=0 && ref_best<0.1 ) {
        jet->_tag_l2_xd0 = (*(nt13->T2BjXd0))[il2_best];
        jet->_tag_l2_xz0 = (*(nt13->T2BjXz0))[il2_best];
        jet->_tag_l2_x2d = (*(nt13->T2BjX2d))[il2_best];
      }
      if( ief_best>=0 && ref_best<0.1 ) {
        jet->_tag_ef_xd0 = (*(nt13->EFBjXd0))[ief_best];
        jet->_tag_ef_xz0 = (*(nt13->EFBjXz0))[ief_best];
        jet->_tag_ef_x2d = (*(nt13->EFBjX2d))[ief_best];
      }
      _atlas_cone_4_tower_jets.push_back( jet );
      //     cout << boost::format(" atlas cone jet: %|10g|(GeV) %|10g| %|10g| %|6g| %|6g| %|6g| %|6d| %|6d| %|6d|")
      //       % jet->_et % jet->eta() % jet->phi()
      //       % jet->_dr_b_parton % jet->_dr_c_parton % jet->_dr_light_parton
      //       % b_partons.size() % c_partons.size() % light_partons.size()
      //          << endl;
    } // end fill cone 4 jets  
  } else {
    assert( !"unhandled mode" );
  }

  return true;  
}

void
NtCBNT::fill_truth_track_endvertex( std::vector< boost::shared_ptr<const AnaTrack> >& tracks ) const
{
  const WHTreeV14* nt14( _madeclass_v14.get() );
  for( std::vector< boost::shared_ptr<const AnaTrack> >::iterator i=tracks.begin(), f=tracks.end(); i!=f; ++i ) {
    const boost::shared_ptr<const AnaTrack>& track( *i );
    if( track->barcode().event_index()!=0 ) { continue; } // don't bother for pileup
    if( track->barcode().barcode()==0 ) { continue; } // don't bother 
    if( track->barcode().barcode()>200000 ) { continue; } // don't bother 
    // find this barcode in the CBNT truth. if found, fill it.
    for( unsigned int i=0, f=nt14->NPar; i!=f; ++i ) {
      if( (*nt14->GenRef)[i] != track->barcode().barcode() ) { continue; }
      if( (*nt14->KOriVNt)[i] > -1 && (*nt14->KOriVNt)[i] < nt14->NVer ) {
	track->_beginvert_radius = (*nt14->RV)[((*nt14->KOriVNt)[i])];
	track->_beginvert_z = (*nt14->ZV)[((*nt14->KOriVNt)[i])];
      }
      if( (*nt14->KEndVNt)[i] > -1 && (*nt14->KEndVNt)[i] < nt14->NVer ) {
	track->_endvert_radius = (*nt14->RV)[((*nt14->KEndVNt)[i])];
      }
      break;
    }
  }
}



const unsigned long
NtCBNT::count_events( const std::string& filename , const Mode& mode )
{
  TFile* f = TFile::Open( ana_streams::root_dcap_filename(filename).c_str() );
  if( !f ) { 
    cout << " NtCBNT: unable to open file " << filename << endl;
    return 0;
  }
  TTree* tree = dynamic_cast<TTree*>(f->Get("CollectionTree"));
  if( !tree ) { 
    cout << " NtCBNT: unable to find CollectionTree in file " << filename << endl;
    return 0;
  }
  const unsigned long n = tree->GetEntries();
  delete f;
  cout << " NtCBNT: file " << filename << " contains " << n << " events." << endl;
  return n;
}
