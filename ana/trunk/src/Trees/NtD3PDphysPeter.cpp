#include "TrigFTKAna_config.h"

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
#include <TTree.h>
#include <TFile.h>
#include <TBranch.h>
#include <TROOT.h>
#include <TSystem.h>
#include <TMatrix.h>
#include <TLorentzVector.h>
#include "TrigFTKAna/Detector.hpp"
#include "TrigFTKAna/Trees/NtD3PDphysPeter.hpp"
#include "TrigFTKAna/AnaElectron.hpp"
#include "TrigFTKAna/AnaMuon.hpp"
#include "TrigFTKAna/AnaJet.hpp"
#include "TrigFTKAna/AnaMET.hpp"
#include "TrigFTKAna/AnaTrigger.hpp"
#include "TrigFTKAna/AnaVertex.hpp"
#include "TrigFTKAna/AnaTrack.hpp"
#include "TrigFTKAna/AnaTruthTable.hpp"
#include "TrigFTKAna/ana_streams_helper.hpp"
#include "TrigFTKAna/AnaConfiguration.hpp"
#include "TrigFTKAna/Conference.hpp"
#include "TrigFTKAna/Trees/D3PDPeterTreeBase.h"
#include "TrigFTKAna/Trees/D3PDPeterTreeV29.h"
#include "TrigFTKAna/Trees/D3PDPeterTreeV29g.h"
#include "TrigFTKAna/Trees/D3PDPeterTreeV29i.h"
#include "TrigFTKAna/Trees/D3PDPeterTreeV29l.h"
#include "TrigFTKAna/Trees/D3PDPeterTreeV30.h"
#include "TrigFTKAna/Trees/D3PDPeterTreeV50.h"
#include "TrigFTKAna/tracking/VertexType.hpp"

using namespace std;

extern TROOT* gROOT;

const bool
NtD3PDphysPeter::begin_file( const std::string& raw_filename )
{

  const string filename = ana_streams::root_dcap_filename(raw_filename);

  _file.reset( ana_streams::open_root_file( filename ) );
  
  if( !_file ) { 
    cout << " NtD3PDphysWZ: unable to open file " << filename << endl;
    return false;
  }
  assert( _file );
  TTree* tree = dynamic_cast<TTree*>(_file->Get("tree"));
  if( !tree ) { 
    cout << " NtD3PDphysWZ: unable to find physics tree file " << filename << endl;
    return false;
  }

  _madeclass_version = V29;
  AnaConfiguration::release(17);
  if(false) { ;
  } else if ( boost::icontains( filename , "1_29l" ) ) {
    _madeclass_version = V29l;
    AnaConfiguration::release(17); 
    std::cout << "Found v 1_29l ntuple, reading using v 1_29l code" << std::endl;
  } else if ( boost::icontains( filename , "1_29i" ) || boost::icontains( filename , "v1_g29i") 
	      || boost::icontains( filename , "v1_e29i") || boost::icontains( filename , "v1_f29i")
	      || boost::icontains( filename , "v1_i29i") ) {
    _madeclass_version = V29i;
    AnaConfiguration::release(17);
    std::cout << "Found v 1_29i ntuple, reading using v 1_29i code" << std::endl;
  } else if ( boost::icontains( filename , "1_29g" ) ) {
    _madeclass_version = V29g;
    AnaConfiguration::release(17);
    std::cout << "Found v 1_29g ntuple, reading using v 1_29g code" << std::endl;
  } else if ( boost::icontains( filename , "1_29" ) ) {
    _madeclass_version = V29;
    AnaConfiguration::release(17);
    std::cout << "Found v 1_29 ntuple, reading using v 1_29 code" << std::endl;
  } else if ( boost::icontains( filename , "1_30" ) ) {
    _madeclass_version = V30;
    AnaConfiguration::release(17);
    std::cout << "Found v1_30 ntuple, reading using v 1_30 code" << std::endl;
  } else if ( boost::icontains( filename , "1_40" ) ) {
    _madeclass_version = V30;
    AnaConfiguration::release(17);
    std::cout << "Found v1_40 ntuple, reading using v 1_30 code" << std::endl;
  } else if ( boost::icontains( filename , "1_50" ) ) {
    _madeclass_version = V50;
    AnaConfiguration::release(17);
    std::cout << "Found v1_50 ntuple, reading using v 1_50 code" << std::endl;
  } else {
    assert(false && "Unknown ntuple version");
  }

    
  switch( _madeclass_version ) {
  case V29: _madeclass.reset( new D3PDPeterTreeV29(tree) ); break;
  case V29g: _madeclass.reset( new D3PDPeterTreeV29g(tree) ); break;
  case V29i: _madeclass.reset( new D3PDPeterTreeV29i(tree) ); break;
  case V29l: _madeclass.reset( new D3PDPeterTreeV29l(tree) ); break;
  case V30: _madeclass.reset( new D3PDPeterTreeV30(tree) ); break;
  case V50: _madeclass.reset( new D3PDPeterTreeV50(tree) ); break;
  default: assert( !"unhandled case" );
  }

  _madeclass->Init( tree );
  _n_events = static_cast<unsigned long>( tree->GetEntries() );

  if( !tree->GetCurrentFile() || tree->GetCurrentFile()->IsZombie() || !(tree->GetCurrentFile()->IsOpen()) ) { 
    cout << " could not open Peter ntuple: " << filename << endl;
    return false;
  }
  if( _n_events == 0 ) {
    cout << " empty Peter ntuple: " << filename << endl;
    return false;
  }

  _open_filename = filename;
  return true;
}

const bool
NtD3PDphysPeter::end_file()
{
  _n_events = 0;
  _open_filename = string();
  if( _file ) {
    // print TTreeCache statistics
    if(true) {
      std::cerr << "INFO: File stats for " << _file->GetName() << ": " << _file->GetBytesRead()
		<< " bytes and " 
		<< _file->GetReadCalls() << " calls" << std::endl;
    }
    _file->Close();
    _file.reset();
  }
  _madeclass.reset();
  return true;
}

void
NtD3PDphysPeter::set_enabled_branches()
{
}

void
NtD3PDphysPeter::set_branch_addresses()
{
}

const bool
NtD3PDphysPeter::get_event( const unsigned long& ievent )
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
  case V29: return _get_event_V29(); break;
  case V29g: return _get_event_V29g(); break;
  case V29i: return _get_event_V29i(); break;
  case V29l: return _get_event_V29l(); break;
  case V30: return _get_event_V30(); break;
  case V50: return _get_event_V50(); break;
  default: assert( !"unhandled case" );
  }

  return false;
}

const unsigned long
NtD3PDphysPeter::count_events( const std::string& filename )
{
  TFile* f = ana_streams::open_root_file( ana_streams::root_dcap_filename(filename) );
  if( !f ) { 
    cout << " NtD3PDphysPeter: unable to open file " << filename << endl;
    return 0;
  }
  TTree* tree = dynamic_cast<TTree*>(f->Get("tree"));
  if( !tree ) { 
    cout << " NtD3PDphysPeter: unable to find physics tree in file " << filename << endl;
    return 0;
  }
  const unsigned long n = tree->GetEntries();
  delete f;
  cout << " NtD3PDphysPeter: file " << filename << " contains " << n << " events." << endl;
  return n;
}


template<typename ntT> 
void 
NtD3PDphysPeter::_make_associated_track_for_muon( boost::shared_ptr<AnaMuon>& mu , 
                                                  ntT nt , const unsigned int& i )
{
  boost::shared_ptr<AnaTrack> track( new AnaTrack );
  mu->_id_track = track;
  const AnaTrack::ftype trackpt = nt->mu_ptid[i];
  track->_curv = detector::safe_divide(static_cast<AnaTrack::ftype>(1),trackpt)*detector::sign_of(nt->mu_qid[i]); // 1/mev
  track->_d0 = nt->mu_d0[i];
  track->_phi0 = nt->mu_phiid[i];
  track->_z0 = nt->mu_z0[i];
  track->_lambda = detector::cotThetaFromEta(nt->mu_etaid[i]);
  if( nt->mu_dvx[i] > -1E6 ) { // -1e6 in flat ntuple flags no vertex or similar error
    track->_exPV_d0 = nt->mu_dxpv[i];
    track->_exPV_d0_err = std::sqrt( nt->mu_covdxpv[i] );
    track->_exPV_z0 = nt->mu_zxpv[i];
  }
  // fit properties
  // track->_chi2 = (*nt->mu_muid_trackfitchi2)[i]; // not available
  // track->_ndof = (*nt->mu_muid_trackfitndof)[i]; // not available
  track->_n_b_layer_hits = nt->mu_nhitbl[i];
  track->_n_pixel_hits = nt->mu_nhitpix[i];
  track->_n_sct_hits = nt->mu_nhitsct[i];
  // track->_n_pixel_holes = // not available
  // track->_n_sct_holes = // not available
  track->_nhits = track->_n_pixel_hits + track->_n_sct_hits;
  // track->_nmisses =  (*nt->mu_muid_nPixHoles)[i] +  (*nt->mu_muid_nSCTHoles)[i]; // not available
  // track->_n_pixel_shared = (*nt->mu_muid_nPixSharedHits)[i]; // not available
  // track->_n_sct_shared = (*nt->mu_muid_nSCTSharedHits)[i]; // not available
}

template<typename ntT> 
void
NtD3PDphysPeter::_make_associated_track_for_electron( boost::shared_ptr<AnaElectron>& ele , 
                                                      ntT nt , const unsigned int& i )
{
  boost::shared_ptr<AnaTrack> track( new AnaTrack );
  ele->_id_track = track;
  // only fill data provided by ntuple here (otherwise do not
  // overwrite defaults)
  track->_d0 = nt->ele_d0[i];
  track->_z0 = nt->ele_z0[i];
  if( nt->mu_dvx[i] > -1E6 ) { // -1e6 in flat ntuple flags no vertex or similar error
    track->_exPV_d0 = nt->ele_dxpv[i];
    track->_exPV_z0 = nt->ele_zxpv[i];
    track->_exPV_d0_err = std::sqrt( nt->ele_covdxpv[i] );
  }
  track->_n_b_layer_hits = nt->ele_nhitbl[i];
  track->_n_pixel_hits = nt->ele_nhitpix[i];
  track->_n_sct_hits = nt->ele_nhitsct[i];
  //track->_n_trt_hits = nt->ele_nhittrt[i]; // not available
  track->_expect_b_layer_hits = nt->ele_expectbl[i];
  track->_trratio = nt->ele_trratio[i];
  track->_n_b_layer_outliers = nt->ele_noutliersbl[i];
  track->_n_pixel_outliers = nt->ele_noutlierspix[i];
  ele->_track_momentum.SetPtEtaPhiM( nt->ele_pttrk[i] , nt->ele_eta[i] , nt->ele_phi[i] , detector::electron_mass( detector::GeV ) );
}

template<typename iteratorT>
const bool
NtD3PDphysPeter::_check_trigger( iteratorT begin , iteratorT end , const char* trig_name ) const
{
  const string tname( boost::algorithm::to_lower_copy(string(trig_name)) );
  for( iteratorT i=begin; i!=end; ++i ) {
    if( boost::algorithm::to_lower_copy(*i) == tname ) { return true; }
  }
  return false;
}

const bool
NtD3PDphysPeter::_get_event_V29()
{
  const D3PDPeterTreeV29* nt( dynamic_cast<const D3PDPeterTreeV29*>(_madeclass.get()) );
  assert( nt );
  assert(nt->nele < D3PDPeterTreeV29::nele_max);
  assert(nt->njet < D3PDPeterTreeV29::njet_max);
  assert(nt->nmu < D3PDPeterTreeV29::nmu_max);
  assert(nt->nvx < D3PDPeterTreeV29::nvx_max);
  // matchvectors are really large, so this is important
  assert(nt->mu_trig_l1matchvector_ < D3PDPeterTreeV29::matchvector_max);
  assert(nt->mu_trig_l2matchvector_ < D3PDPeterTreeV29::matchvector_max);
  assert(nt->mu_trig_l2samatchvector_ < D3PDPeterTreeV29::matchvector_max);
  assert(nt->mu_trig_efmatchvector_ < D3PDPeterTreeV29::matchvector_max);
  assert(nt->ele_trig_l1matchvector_ < D3PDPeterTreeV29::matchvector_max);
  assert(nt->ele_trig_l2matchvector_ < D3PDPeterTreeV29::matchvector_max);
  assert(nt->ele_trig_efmatchvector_ < D3PDPeterTreeV29::matchvector_max);

  _run_number = nt->run;
  _event_number = nt->event;
  _lumi_block = nt->lb;
  _average_mu = nt->lumi_mu_average; // encoded in a dedicated variable as of mc11a
  _mc_weight = nt->mc_weight;
  _pdf_weights = boost::assign::list_of(nt->lha_cteq6ll)(nt->lha_MRSTMCal)(nt->lha_CT10)(nt->lha_MSTW2008lo68cl)(nt->lha_MSTW2008nlo68cl)(nt->lha_NNPDF21_100)(nt->lha_HERAPDF10_EIG)(nt->lha_cteq66);
  _mc_channel = nt->mc_channel;
  _mc_hfor = nt->mc_hfor;

  // fill vertex information
  assert( nt );
  _vertices.reserve( static_cast<int>( nt->nvx ) );
  for( int iv=0, fv=nt->nvx; iv!=fv; ++iv ) {
    boost::shared_ptr<AnaVertex> v( new AnaVertex );
    v->_x = nt->vx_x[iv];
    v->_y = nt->vx_y[iv];
    v->_z = nt->vx_z[iv];
    v->_ntracks = nt->vx_ntracks[iv];
    v->_sum_pt = nt->vx_sumpt[iv];
    v->_type = detector::remap_vertex_type_enum( nt->vx_type[iv] );
    _vertices.push_back( v );
  }

  // fill muons
  assert( nt );
  _muons.reserve( static_cast<int>( nt->nmu ) );
  for( int i=0, f=nt->nmu; i!=f; ++i ) {
    // for each muon in the ntuple, create a separate AnaMuon for 
    // combined, ID, MS, and extrapolated MS muons
    boost::shared_ptr<AnaMuon> mu( new AnaMuon );
    mu->_all_author_bitmask = AnaMuon::MuonTypeBitset( nt->mu_author[i] );
    // override some definitions per Peter ntuple coding to ensure that we have *combined* muons
    mu->_all_author_bitmask.set(MuonAllAuthor::STACO,mu->_all_author_bitmask.test(MuonAllAuthor::STACO) && nt->mu_class[i]==1 && nt->mu_ptid[i]!=-1);
    mu->_all_author_bitmask.set(MuonAllAuthor::MuidCo,mu->_all_author_bitmask.test(MuonAllAuthor::MuidCo) && nt->mu_class[i]==2 && nt->mu_ptid[i]!=-1);
    // other muon variables
    mu->_primary_author = nt->mu_primauthor[i];
    mu->_aod_quality = nt->mu_quality[i];
    switch( nt->mu_class[i] ) { // other possibility is to use mu_type here
    case 1: mu->_aod_class = AnaMuon::AOD_STACO; break;
    case 2: mu->_aod_class = AnaMuon::AOD_MUID; break;
    case 3: mu->_aod_class = AnaMuon::AOD_CALO; break;
    default: mu->_aod_class = AnaMuon::AOD_UNKNOWN; break;
    }
    mu->_ntuple_index = i;
    mu->_z0 = nt->mu_z0[i];
    mu->_d0 = nt->mu_d0[i];
    mu->_d0_exPV_err = std::sqrt(nt->mu_covdxpv[i]);
    mu->_z0_exPV = nt->mu_zxpv[i];
    mu->_d0_exPV = nt->mu_dxpv[i];
    mu->_z0_vx = nt->mu_zvx[i];
    mu->_d0_vx = nt->mu_dvx[i];
    mu->_d0_vx_err = std::sqrt(nt->mu_covdvx[i]);
    mu->_charge = nt->mu_q[i];
    mu->_matchchi2 = nt->mu_matchchi2[i];
    mu->_id_charge = nt->mu_qid[i];
    mu->_ms_charge = nt->mu_qms[i];
    mu->_exms_charge = nt->mu_qexms[i];
    mu->_momentum.SetPtEtaPhiM( nt->mu_pt[i] ,nt->mu_eta[i] ,
                                nt->mu_phi[i], detector::muon_mass( detector::GeV ) );
    // isolation variables
    mu->_pt_cone = boost::assign::list_of(nt->mu_ptcone10[i])(nt->mu_ptcone20[i])(nt->mu_ptcone30[i])(nt->mu_ptcone40[i]);
    mu->_et_cone = boost::assign::list_of(nt->mu_etcone10[i])(nt->mu_etcone20[i])(nt->mu_etcone30[i])(nt->mu_etcone40[i]);
    // nhits, nholes et cetera
    mu->_expect_b_layer = nt->mu_expectbl[i];
    mu->_nhits = boost::assign::list_of(nt->mu_nhitpix[i])(nt->mu_nhitsct[i])(nt->mu_nhittrt[i])(nt->mu_noutlierstrt[i])(nt->mu_nhitbl[i]);
    mu->_nholes = boost::assign::list_of(nt->mu_nholespix[i])(nt->mu_nholessct[i])(nt->mu_nholestrt[i]);
    mu->_ndead = boost::assign::list_of(nt->mu_ndeadsensorspix[i])(nt->mu_ndeadsensorssct[i])(-1);
    // MET reconstruction
    mu->_met_wet = nt->mu_MET_wet[i];
    mu->_met_wpx = nt->mu_MET_wpx[i];
    mu->_met_wpy = nt->mu_MET_wpy[i];
    mu->_met_status = nt->mu_MET_status[i];
    mu->_met_wet2 = nt->mu_MET_wet2[i];
    mu->_met_wpx2 = nt->mu_MET_wpx2[i];
    mu->_met_wpy2 = nt->mu_MET_wpy2[i];
    mu->_met_status2 = nt->mu_MET_status2[i];
    // extrapolated muon spectrometer momentum:
    mu->_id_muon.SetPtEtaPhiM( nt->mu_ptid[i] ,nt->mu_etaid[i] ,
                               nt->mu_phiid[i], detector::muon_mass( detector::GeV ) );
    mu->_ms_muon.SetPtEtaPhiM( nt->mu_ptms[i] ,nt->mu_etams[i] ,
                               nt->mu_phims[i], detector::muon_mass( detector::GeV ) );
    mu->_exms_muon.SetPtEtaPhiM( nt->mu_ptexms[i] ,nt->mu_etaexms[i] ,
                                 nt->mu_phiexms[i], detector::muon_mass( detector::GeV ) );
    _make_associated_track_for_muon( mu , nt , i ); 
    _muons.push_back( mu );
  }

  // fill inner-detector tracks as ID-only muons [ for tag-and-probe ]
  // as an alternative, one can use CaloTag muons as probes
  if (false) {
    assert(nt->ntrk < D3PDPeterTreeV29::ntrk_max);
    for( int i=0, f=nt->ntrk; i!=f; ++i ) {
      boost::shared_ptr<AnaMuon> mu( new AnaMuon );
      // other muon variables
      mu->_primary_author = MuonPrimAuthor::IDtrack;
      mu->_aod_class = AnaMuon::AOD_TRACK;
      mu->_ntuple_index = -1;
      mu->_d0_exPV_err = std::sqrt(nt->trk_covdxpv[i]);
      mu->_z0_exPV = nt->trk_zxpv[i];
      mu->_d0_exPV = nt->trk_dxpv[i];
      mu->_charge = nt->trk_q[i];
      const float sf = 1.0;
      mu->_momentum.SetPtEtaPhiM( sf*nt->trk_pt[i] ,nt->trk_eta[i] ,
				  nt->trk_phi[i], detector::muon_mass( detector::GeV ) );
      // isolation variables
      mu->_pt_cone = boost::assign::list_of(nt->trk_ptcone10[i])(nt->trk_ptcone20[i])(nt->trk_ptcone30[i])(nt->trk_ptcone40[i]);
      // nhits, nholes et cetera
      mu->_expect_b_layer = nt->trk_expectbl[i];
      mu->_nhits = boost::assign::list_of(nt->trk_nhitpix[i])(nt->trk_nhitsct[i])(nt->trk_nhittrt[i])(nt->trk_noutlierstrt[i])(nt->trk_nhitbl[i]);
      mu->_nholes = boost::assign::list_of(nt->trk_nholespix[i])(nt->trk_nholessct[i])(nt->trk_nholestrt[i]);
      mu->_ndead = boost::assign::list_of(nt->trk_ndeadsensorspix[i])(nt->trk_ndeadsensorssct[i])(-1);
      _muons.push_back( mu );
    }
  }

  // fill electrons
  if( true ) {
    for( int i=0, f=nt->nele; i!=f; ++i ) { 
      boost::shared_ptr<AnaElectron> ele( new AnaElectron );
      ele->_author = nt->ele_author[i];
      ele->_charge = nt->ele_q[i];
      ele->_momentum.SetPtEtaPhiM( nt->ele_et[i] , nt->ele_eta[i] , nt->ele_phi[i] , detector::electron_mass(detector::GeV) );
      ele->_is_em = nt->ele_isEM[i];
      ele->_oq = nt->ele_isgoodoq[i] ? 0 : 1446; // FIXME: store full bitmask once it is in the ntuple
      ele->_aod_quality = nt->ele_quality[i];
      ele->_rhad1 = nt->ele_rhad1[i];
      ele->_rhad = nt->ele_rhad[i];
      ele->_reta = nt->ele_reta[i];
      ele->_weta2 = nt->ele_w2[i];
      ele->_wstot = nt->ele_wstot[i];
      ele->_deltaeta = nt->ele_deltaeta[i];
      ele->_deltaphi = nt->ele_deltaphi[i];
      ele->_eoverp = detector::safe_divide( nt->ele_etclus[i] , nt->ele_pttrk[i] );
      ele->_demaxs1 = nt->ele_demaxs1[i]/1000.;
      ele->_eta2 = nt->ele_eta2[i];
      ele->_etap = nt->ele_etap[i];
      ele->_et2 = nt->ele_et2[i]/1000.;
      ele->_d0_exPV_err = std::sqrt(nt->ele_covdxpv[i]);
      ele->_z0_exPV = nt->ele_zxpv[i];
      ele->_d0_exPV = nt->ele_dxpv[i];
      ele->_d0_vx = nt->ele_dvx[i];
      ele->_d0_vx_err = std::sqrt(nt->ele_covdvx[i]);
      ele->_z0_vx = nt->ele_zvx[i];
      // MET reconstruction
      ele->_met_wet = nt->ele_MET_wet[i];
      ele->_met_wpx = nt->ele_MET_wpx[i];
      ele->_met_wpy = nt->ele_MET_wpy[i];
      ele->_met_status = nt->ele_MET_status[i];
      ele->_ntuple_index = i;
      ele->_pt_cone = boost::assign::list_of( -999. )( nt->ele_ptcone20[i] )( nt->ele_ptcone30[i] )( nt->ele_ptcone40[i] );
      ele->_uncorrected_et_cone = boost::assign::list_of( -999. )( nt->ele_etcone20[i] )( nt->ele_etcone30[i] )( nt->ele_etcone40[i] );
      ele->_et_cone = boost::assign::list_of( -999. )( nt->ele_etcone20_cor[i] )( nt->ele_etcone30_cor[i] )( nt->ele_etcone40_cor[i] );
      ele->_cluster_momentum.SetPtEtaPhiM( nt->ele_etclus[i] , nt->ele_etaclus[i] , nt->ele_phiclus[i] , detector::electron_mass(detector::GeV) );
      _make_associated_track_for_electron( ele , nt , i );
      ele->_nhits = boost::assign::list_of(nt->ele_nhitpix[i])(nt->ele_nhitsct[i])(nt->ele_nhitbl[i])(nt->ele_nhittrt[i]);
      ele->_noutliers = boost::assign::list_of(nt->ele_noutlierspix[i])(nt->ele_noutlierssct[i])(nt->ele_noutliersbl[i])(0);
      ele->_expect_b_layer = nt->ele_expectbl[i];
      _electrons.push_back( ele );
    }
  }

  // fill photons
  if( false ) {
    assert(nt->nph < D3PDPeterTreeV29::nph_max);
    for( int i=0, f=nt->nph; i!=f; ++i ) { 
      boost::shared_ptr<AnaPhoton> ph( new AnaPhoton );
      ph->_author = nt->ph_author[i];
      ph->_momentum.SetPtEtaPhiM( nt->ph_et[i] , nt->ph_eta[i] , nt->ph_phi[i] , 0. );
      ph->_cluster_momentum.SetPtEtaPhiE( nt->ph_eclus[i] * std::sin(detector::thetaFromEta(nt->ph_etaclus[i])) , 
                                          nt->ph_etaclus[i] , nt->ph_phiclus[i] , nt->ph_eclus[i] );
      ph->_isEM = nt->ph_isEM[i];
      ph->_oq = nt->ph_oq[i];
      ph->_isgoodoq = nt->ph_isgoodoq[i];
      ph->_aod_quality = nt->ph_quality[i];
      ph->_et_cone = boost::assign::list_of( -999. )( nt->ph_etcone20_cor[i] )( nt->ph_etcone30_cor[i] )( nt->ph_etcone40_cor[i] );
      ph->_uncorrected_et_cone = boost::assign::list_of( -999. )( nt->ph_etcone20[i] )( nt->ph_etcone30[i] )( nt->ph_etcone40[i] );
      ph->_pt_cone = boost::assign::list_of( -999. )( nt->ph_ptcone20[i] )( nt->ph_ptcone30[i] )( nt->ph_ptcone40[i] );
      ph->_is_recovered = nt->ph_isRecovered[i];
      ph->_convFlag = nt->ph_convFlag[i];
      ph->_rconv = nt->ph_rconv[i];
      ph->_zconv = nt->ph_zconv[i];
      ph->_etas2 = nt->ph_etas2[i];
      ph->_ethad1 = nt->ph_ethad1[i];
      ph->_ethad = nt->ph_ethad[i];

      ph->_e277 = nt->ph_e277[i];
      ph->_e237 = nt->ph_e237[i];
      ph->_e233 = nt->ph_e233[i];
      ph->_weta2 = nt->ph_weta2[i];
      ph->_f1 = nt->ph_f1[i];
      ph->_emax = nt->ph_emax[i];
      ph->_emaxs2 = nt->ph_emaxs2[i];
      ph->_demaxs1 = nt->ph_demaxs1[i];
      ph->_fside = nt->ph_fside[i];
      ph->_wstot = nt->ph_wstot[i];
      ph->_ws3 = nt->ph_ws3[i];
      ph->_emins1 = nt->ph_emins1[i];
      ph->_etap = nt->ph_etap[i];
      ph->_ntuple_index = i;
      _photons.push_back( ph );
    }
  }

  // fill inner detector tracks as AnaTracks
  if( false ) {
    for( int i=0, f=nt->ntrk; i!=f; ++i ) { 
      boost::shared_ptr<AnaTrack> track( new AnaTrack );
      const AnaTrack::ftype trackpt = nt->trk_pt[i];
      track->_curv = detector::safe_divide(static_cast<AnaTrack::ftype>(1),trackpt)*detector::sign_of(nt->trk_q[i]); // 1/mev
      track->_phi0 = nt->trk_phi[i];
      track->_lambda = detector::cotThetaFromEta(nt->trk_eta[i]);
      if( nt->trk_dxpv[i] > -1E6 ) { // -1e6 in flat ntuple flags no vertex or similar error
	track->_exPV_d0 = nt->trk_dxpv[i];
	track->_exPV_d0_err = std::sqrt( nt->trk_covdxpv[i] );
	track->_exPV_z0 = nt->trk_zxpv[i];
      }
      track->_n_b_layer_hits = nt->trk_nhitbl[i];
      track->_n_pixel_hits = nt->trk_nhitpix[i];
      track->_n_sct_hits = nt->trk_nhitsct[i];
      track->_n_pixel_holes = nt->trk_nholespix[i];
      track->_n_sct_holes = nt->trk_nholessct[i];
      track->_nhits = track->_n_pixel_hits + track->_n_sct_hits;
      track->_nmisses =  track->_n_pixel_holes + track->_n_sct_holes;
      _offline_tracks.push_back(track);
    }
  }

  // fill jets
  assert( nt );
  _jets.reserve( static_cast<int>( nt->njet ) );
  for( int i=0, f=nt->njet; i!=f; ++i ) {
    boost::shared_ptr<AnaJet> jet( new AnaJet );
    switch( nt->jet_type[i] ) {
    case 0: jet->_alg = AnaJet::Algorithm::ATLAS_ANTI_KT_CONE_4_H1_TOPO; break;
    case 1: jet->_alg = AnaJet::Algorithm::ATLAS_ANTI_KT_CONE_6_H1_TOPO; break;
    case 2: jet->_alg = AnaJet::Algorithm::ATLAS_ANTI_KT_CONE_4_H1_LC_TOPO; break;
    case 3: jet->_alg = AnaJet::Algorithm::ATLAS_ANTI_KT_CONE_6_H1_LC_TOPO; break;
    case 4: jet->_alg = AnaJet::Algorithm::ATLAS_CONE_4_TRUTH; break;
    case 5: jet->_alg = AnaJet::Algorithm::ATLAS_CONE_4_TRUTH_PILEUP; break;
    case 6: jet->_alg = AnaJet::Algorithm::ATLAS_CONE_6_TRUTH; break;
    case 7: jet->_alg = AnaJet::Algorithm::ATLAS_CONE_6_TRUTH_PILEUP; break;
    default: assert(0&&"Unrecognized jet_type"); break;
    }
    if (jet->_alg == AnaJet::Algorithm::ATLAS_CONE_4_TRUTH_PILEUP) continue;
    if (jet->_alg == AnaJet::Algorithm::ATLAS_CONE_6_TRUTH_PILEUP) continue;
    jet->_jet_index = i;
    jet->_vtx_fraction = nt->jet_jvf[i];
    jet->_n90 = nt->jet_n90[i];
    jet->_timing = nt->jet_time[i];
    jet->_quality = nt->jet_larquality[i];
    jet->_HECQuality = nt->jet_hecquality[i];
    jet->_LArQuality = nt->jet_larquality[i];
    jet->_negativeE = nt->jet_negativee[i];
    jet->_sumPtTrack = nt->jet_sumpttrk[i];
    jet->_ntrkjetalg = nt->jet_ntrk[i];
    jet->_AverageLArQF = nt->jet_averagelarqf[i];
    jet->_emf = nt->jet_emf[i];
    jet->_fmax = nt->jet_fmax[i];
    jet->_BCH_CORR_CELL = nt->jet_bchcorr[i];
    jet->_BCH_CORR_JET = nt->jet_bchcorrjet[i];
    jet->_hecf = nt->jet_hecf[i];
    jet->_tgap3f = nt->jet_tgap3f[i];
    jet->_full_scale_momentum.SetPtEtaPhiM(nt->jet_pt[i],nt->jet_eta[i],nt->jet_phi[i],nt->jet_m[i]);
    // em scale for met cleaning cuts
    // NOTE: the 1000. here for eta and phi is a bug fix. REMEMBER TO REMOVE IT FOR THE NEXT VERSION OF NTUPLES!!!!!!!
    jet->_em_scale_momentum.SetPtEtaPhiM(nt->jet_pt_em[i],nt->jet_eta_em[i]*1000.,nt->jet_phi_em[i]*1000.,nt->jet_m_em[i]); 
    // origin values
    jet->_EtaOriginEM = nt->jet_eta_origin_em[i];
    jet->_PhiOriginEM = nt->jet_phi_origin_em[i];
    jet->_MOriginEM = nt->jet_m_origin_em[i];
    jet->_EtaOrigin = nt->jet_eta_origin[i];
    jet->_PhiOrigin = nt->jet_phi_origin[i];
    jet->_MOrigin = nt->jet_m_origin[i];
    // MET reconstruction
    jet->_met_wet = nt->jet_MET_wet[i];
    jet->_met_wpx = nt->jet_MET_wpx[i];
    jet->_met_wpy = nt->jet_MET_wpy[i];
    jet->_met_status = nt->jet_MET_status[i];
    jet->_gcwjes = nt->jet_gcwjes[i];
    // other stuff
    jet->_tag_ip2d_w = nt->jet_tagIP2D[i];
    jet->_tag_ip3d_w = nt->jet_tagIP3D[i];
    jet->_tag_sv0_w = nt->jet_tagSV0[i];
    jet->_tag_sv1_w = nt->jet_tagSV1[i];
    jet->_tag_sv2_w = nt->jet_tagSV2[i];
    jet->_tag_jetp_w = nt->jet_tagJetProb[i];
    jet->_tag_cmbnn_w = nt->jet_tagJetFitterCOMBNN[i];
    jet->_tag_cmb_w = nt->jet_tagSV1[i]+nt->jet_tagIP3D[i];
    jet->_em_scale_correction = nt->jet_emjes[i];
    // finish off
    jet->_momentum = jet->_full_scale_momentum;
    _jets.push_back( jet );
  }


  // fill met
  assert( nt );
  assert( _met.empty() );
  boost::shared_ptr<AnaMET> met( new AnaMET );
  met->_met_type = AnaMET::REF_FINAL;
  met->_refgamma = nt->met_refgamma;
  met->_refgamma_phi = nt->met_refgamma_phi;
  met->_refgamma_sumet = nt->met_refgamma_sumet;
  met->_reftau = nt->met_reftau;
  met->_reftau_phi = nt->met_reftau_phi;
  met->_reftau_sumet = nt->met_reftau_sumet;
  met->_refmuon = nt->met_refmuon;
  met->_refmuon_phi = nt->met_refmuon_phi;
  met->_refmuon_sumet = nt->met_refmuon_sumet;
  met->_refele = nt->met_refele;
  met->_refele_phi = nt->met_refele_phi;
  met->_refele_sumet = nt->met_refele_sumet;
  met->_refjet = nt->met_refjet;
  met->_refjet_phi = nt->met_refjet_phi;
  met->_refjet_sumet = nt->met_refjet_sumet;
  met->_cellout_eflow = nt->met_cellout_eflow;
  met->_cellout_eflow_phi = nt->met_cellout_eflow_phi;
  met->_cellout_eflow_sumet = nt->met_cellout_eflow_sumet;
  met->_cellout = nt->met_cellout;
  met->_cellout_phi = nt->met_cellout_phi;
  met->_cellout_sumet = nt->met_cellout_sumet;  
  met->_softjets = nt->met_softjets;
  met->_softjets_phi = nt->met_softjets_phi;
  met->_softjets_sumet = nt->met_softjets_sumet;
  met->_cryo = nt->met_cryo;
  met->_cryo_phi = nt->met_cryo_phi;
  met->_cryo_sumet = nt->met_cryo_sumet;
  met->_muonboy = nt->met_muonboy;
  met->_muonboy_phi = nt->met_muonboy_phi;
  met->_muonboy_sumet = nt->met_muonboy_sumet;
  met->_lochad_topo = nt->met_lochadtopo;
  met->_lochad_topo_phi = nt->met_lochadtopo_phi;
  met->_lochad_topo_sumet = nt->met_lochadtopo_sumet;
  met->_momentum.SetPtEtaPhiM( nt->met_reffinal , 0. , nt->met_reffinal_phi , 0. );
  _met.push_back( met );
  if( true ) {
    boost::shared_ptr<AnaMET> met( new AnaMET );
    met->_met_type = AnaMET::LOCHADTOPO;
    met->_momentum.SetPtEtaPhiM( nt->met_lochadtopo , 0. , nt->met_lochadtopo_phi , 0. );
    _met.push_back( met );
  }
  if( true ) { 
    boost::shared_ptr<AnaMET> met( new AnaMET );
    met->_met_type = AnaMET::TRUTH;
    met->_momentum.SetPtEtaPhiM( nt->met_truth , 0. , nt->met_truth_phi , 0. );
    _met.push_back( met );
  }
  if( true ) { 
    boost::shared_ptr<AnaMET> met( new AnaMET );
    met->_met_type = AnaMET::REF_FINAL_45;
    met->_momentum.SetPtEtaPhiM( nt->met_reffinal_eta45 , 0. , nt->met_reffinal_eta45_phi , 0. );
    _met.push_back( met );
  }

  // fill trigger decision
  assert( nt );
  _trigger.reset( new AnaTrigger );
  _trigger->_lbn = nt->lb;
  _trigger->_run_number = nt->run;
  _trigger->_error_lar = nt->error_lar;
  // the following vector contain the entire event trigger record
  const std::vector<std::string>& L1 = *nt->trig_l1;
  const std::vector<std::string>& L2 = *nt->trig_l2;
  const std::vector<std::string>& EF = *nt->trig_ef;
  _trigger->_trig_l1_mu0 = _check_trigger(L1.begin(),L1.end(),"L1_MU0");
  _trigger->_trig_l1_mu6 = _check_trigger(L1.begin(),L1.end(),"L1_MU6");
  _trigger->_trig_l1_mu10 = _check_trigger(L1.begin(),L1.end(),"L1_MU10");
  _trigger->_trig_ef_mu10 = _check_trigger(EF.begin(),EF.end(),"EF_mu10");
  _trigger->_trig_ef_mu10_mg = _check_trigger(EF.begin(),EF.end(),"EF_mu10_MG");
  _trigger->_trig_ef_mu13_mg = _check_trigger(EF.begin(),EF.end(),"EF_mu13_MG");
  _trigger->_trig_ef_mu13_mg_tight = _check_trigger(EF.begin(),EF.end(),"EF_mu13_MG_tight");
  _trigger->_trig_ef_mu15_mu10_effs = _check_trigger(EF.begin(),EF.end(),"EF_mu15_mu10_EFFS");
  _trigger->_trig_ef_mu15_mu10_effs_medium = _check_trigger(EF.begin(),EF.end(),"EF_mu15_mu10_EFFS_medium");
  _trigger->_trig_ef_mu18 = _check_trigger(EF.begin(),EF.end(),"EF_mu18");
  _trigger->_trig_ef_mu18_mg = _check_trigger(EF.begin(),EF.end(),"EF_mu18_MG");
  _trigger->_trig_ef_mu20_mg = _check_trigger(EF.begin(),EF.end(),"EF_mu20_MG");
  _trigger->_trig_ef_mu22_mg = _check_trigger(EF.begin(),EF.end(),"EF_mu22_MG");
  _trigger->_trig_ef_mu40_msonly_barrel = _check_trigger(EF.begin(),EF.end(),"EF_mu40_MSonly_barrel");
  _trigger->_trig_ef_mu18_medium = _check_trigger(EF.begin(),EF.end(),"EF_mu18_medium");
  _trigger->_trig_ef_mu18_mg_medium = _check_trigger(EF.begin(),EF.end(),"EF_mu18_MG_medium");
  _trigger->_trig_ef_mu40_msonly_barrel_medium = _check_trigger(EF.begin(),EF.end(),"EF_mu40_MSonly_barrel_medium");
  _trigger->_trig_l1_em14 = _check_trigger(L1.begin(),L1.end(),"L1_EM14");
  _trigger->_trig_ef_e15_medium = _check_trigger(EF.begin(),EF.end(),"EF_e15_medium");
  _trigger->_trig_ef_e20_medium = _check_trigger(EF.begin(),EF.end(),"EF_e20_medium");
  _trigger->_trig_ef_e22_medium = _check_trigger(EF.begin(),EF.end(),"EF_e22_medium");
  _trigger->_trig_ef_e22vh_medium1 = _check_trigger(EF.begin(),EF.end(),"EF_e22vh_medium1");
  _trigger->_trig_ef_2e12_medium = _check_trigger(EF.begin(),EF.end(),"EF_2e12_medium");
  _trigger->_trig_ef_2e12t_medium = _check_trigger(EF.begin(),EF.end(),"EF_2e12T_medium");
  _trigger->_trig_ef_2e12tvh_medium = _check_trigger(EF.begin(),EF.end(),"EF_2e12Tvh_medium");
  _trigger->_trig_ef_2e10_medium = _check_trigger(EF.begin(),EF.end(),"EF_2e10_medium");
  _trigger->_trig_ef_2mu10 = _check_trigger(EF.begin(),EF.end(),"EF_2mu10");
  _trigger->_trig_ef_2mu10_loose = _check_trigger(EF.begin(),EF.end(),"EF_2mu10_loose");
  _trigger->_trig_ef_mu6 = _check_trigger(EF.begin(),EF.end(),"EF_mu6");
  _trigger->_trig_ef_g20_loose = _check_trigger(EF.begin(),EF.end(),"EF_g20_loose");
  _trigger->_trig_ef_j10_a4tc_effs = _check_trigger(EF.begin(),EF.end(),"EF_j10_a4tc_EFFS");
  _trigger->_trig_ef_fj10_a4tc_effs = _check_trigger(EF.begin(),EF.end(),"EF_fj10_a4tc_EFFS");
  _trigger->_trig_ef_j15_a4tc_effs = _check_trigger(EF.begin(),EF.end(),"EF_j15_a4tc_EFFS");
  _trigger->_trig_ef_fj15_a4tc_effs = _check_trigger(EF.begin(),EF.end(),"EF_fj15_a4tc_EFFS");
  _trigger->_trig_ef_j20_a4_effs = _check_trigger(EF.begin(),EF.end(),"EF_j20_a4_EFFS");
  _trigger->_trig_ef_j20_a4tc_effs = _check_trigger(EF.begin(),EF.end(),"EF_j20_a4tc_EFFS");
  _trigger->_trig_ef_fj20_a4tc_effs = _check_trigger(EF.begin(),EF.end(),"EF_fj20_a4tc_EFFS");
  _trigger->_trig_ef_xe70 = _check_trigger(EF.begin(),EF.end(),"EF_xe70");
  _trigger->_trig_ef_xe70_tclcw = _check_trigger(EF.begin(),EF.end(),"EF_xe70_tclcw");
  _trigger->_trig_ef_xe60 = _check_trigger(EF.begin(),EF.end(),"EF_xe60");
  _trigger->_trig_ef_xe60t = _check_trigger(EF.begin(),EF.end(),"EF_xe60T");
  _trigger->_trig_ef_j110_a4tchad_xe75_tclcw = _check_trigger(EF.begin(),EF.end(),"EF_j110_a4tchad_xe75_tclcw");
  _trigger->_trig_ef_j80_a4tchad_xe100_tclcw_veryloose = _check_trigger(EF.begin(),EF.end(),"EF_j80_a4tchad_xe100_tclcw_veryloose");


  // fill with Peter's trigger matching info
  for( int i=0; i!=nt->mu_trig_l1matchvector_; ++i ) { 
    const int i_ntuple_mu = nt->mu_trig_l1matchvector_first[i];
    const int i_trigger = nt->mu_trig_l1matchvector_second[i];
    const AnaMuon::ftype dr = (*(nt->mu_trig_l1matchdelr))[i];
    std::vector< boost::shared_ptr<const AnaMuon> >::iterator imu = 
      std::find_if( _muons.begin() , _muons.end() , boost::bind(&AnaMuon::ntuple_index,_1)==i_ntuple_mu );
    assert( i_trigger < L1.size() );
    if( imu != _muons.end() ) { (*imu)->_trigger_match_dr[ L1[i_trigger] ] = dr; }
  }
  for( int i=0; i!=nt->mu_trig_l2matchvector_; ++i ) { 
    const int i_ntuple_mu = nt->mu_trig_l2matchvector_first[i];
    const int i_trigger = nt->mu_trig_l2matchvector_second[i];
    const AnaMuon::ftype dr = (*(nt->mu_trig_l2matchdelr))[i];
    std::vector< boost::shared_ptr<const AnaMuon> >::iterator imu = 
      std::find_if( _muons.begin() , _muons.end() , boost::bind(&AnaMuon::ntuple_index,_1)==i_ntuple_mu );
    assert( i_trigger < L2.size() );
    if( imu != _muons.end() ) { (*imu)->_trigger_match_dr[ L2[i_trigger] ] = dr; }
  }
  for( int i=0; i!=nt->mu_trig_l2samatchvector_; ++i ) { 
    const int i_ntuple_mu = nt->mu_trig_l2samatchvector_first[i];
    const int i_trigger = nt->mu_trig_l2samatchvector_second[i];
    const AnaMuon::ftype dr = (*(nt->mu_trig_l2samatchdelr))[i];
    std::vector< boost::shared_ptr<const AnaMuon> >::iterator imu = 
      std::find_if( _muons.begin() , _muons.end() , boost::bind(&AnaMuon::ntuple_index,_1)==i_ntuple_mu );
    assert( i_trigger < L2.size() );
    if( imu != _muons.end() ) { (*imu)->_trigger_match_dr[ L2[i_trigger] ] = dr; }
  }
  for( int i=0; i!=nt->mu_trig_efmatchvector_; ++i ) { 
    const int i_ntuple_mu = nt->mu_trig_efmatchvector_first[i];
    const int i_trigger = nt->mu_trig_efmatchvector_second[i];
    const AnaMuon::ftype dr = (*(nt->mu_trig_efmatchdelr))[i];
    std::vector< boost::shared_ptr<const AnaMuon> >::iterator imu = 
      std::find_if( _muons.begin() , _muons.end() , boost::bind(&AnaMuon::ntuple_index,_1)==i_ntuple_mu );
    assert( i_trigger < EF.size() );
    if( imu != _muons.end() ) { (*imu)->_trigger_match_dr[ EF[i_trigger] ] = dr; }
  }

  for( int i=0; i!=nt->ele_trig_l1matchvector_; ++i ) { 
    const int i_ntuple_ele = nt->ele_trig_l1matchvector_first[i];
    const int i_trigger = nt->ele_trig_l1matchvector_second[i];
    const AnaElectron::ftype dr = (*(nt->ele_trig_l1matchdelr))[i];
    std::vector< boost::shared_ptr<const AnaElectron> >::iterator iele = 
      std::find_if( _electrons.begin() , _electrons.end() , boost::bind(&AnaElectron::ntuple_index,_1)==i_ntuple_ele );
    assert( i_trigger < L1.size() );
    if( iele != _electrons.end() ) { (*iele)->_trigger_match_dr[ L1[i_trigger] ] = dr; }
  }
  for( int i=0; i!=nt->ele_trig_l2matchvector_; ++i ) { 
    const int i_ntuple_ele = nt->ele_trig_l2matchvector_first[i];
    const int i_trigger = nt->ele_trig_l2matchvector_second[i];
    const AnaElectron::ftype dr = (*(nt->ele_trig_l2matchdelr))[i];
    std::vector< boost::shared_ptr<const AnaElectron> >::iterator iele = 
      std::find_if( _electrons.begin() , _electrons.end() , boost::bind(&AnaElectron::ntuple_index,_1)==i_ntuple_ele );
    assert( i_trigger < L2.size() );
    if( iele != _electrons.end() ) { (*iele)->_trigger_match_dr[ L2[i_trigger] ] = dr; }
  }
  for( int i=0; i!=nt->ele_trig_efmatchvector_; ++i ) { 
    const int i_ntuple_ele = nt->ele_trig_efmatchvector_first[i];
    const int i_trigger = nt->ele_trig_efmatchvector_second[i];
    const AnaElectron::ftype dr = (*(nt->ele_trig_efmatchdelr))[i];
    std::vector< boost::shared_ptr<const AnaElectron> >::iterator iele = 
      std::find_if( _electrons.begin() , _electrons.end() , boost::bind(&AnaElectron::ntuple_index,_1)==i_ntuple_ele );
    assert( i_trigger < EF.size() );
    if( iele != _electrons.end() ) { (*iele)->_trigger_match_dr[ EF[i_trigger] ] = dr; }
  }

  // fill truth particles
  assert( nt );
  assert(nt->nmc < D3PDPeterTreeV29::nmc_max);
  _truth_particles.reserve( static_cast<int>( nt->nmc ) );
  for( int imc=0, fmc=nt->nmc; imc!=fmc; ++imc ) {
    boost::shared_ptr<AnaTruthParticle> v( new AnaTruthParticle );
    v->_pdg_id = (nt->mc_pdgid)[imc];
    const int jmc = (nt->mc_parent)[imc];
    v->_parent_index = jmc;
    v->_parent_pdg_id = jmc==-1 ? -1 : (nt->mc_pdgid)[jmc];
    v->_charge = detector::charge_from_pdg((nt->mc_pdgid)[imc]);
    v->_status = (nt->mc_status)[imc];
    v->_parent = (nt->mc_parent)[imc];
    v->_momentum.SetPtEtaPhiE( (nt->mc_pt)[imc] , (nt->mc_eta)[imc] , 
			       (nt->mc_phi)[imc] , (nt->mc_e)[imc] );
    _truth_particles.push_back( v );
  }
  
  // fill truth table
  if( false ) {
    _truth_table.reset( new AnaTruthTable );
    for( AnaTruthTable::index_type imc=0, fmc=nt->nmc; imc!=fmc; ++imc ) {
      static TLorentzVector mom;
      const int dummy(0);
      mom.SetPtEtaPhiE( (nt->mc_pt)[imc] , (nt->mc_eta)[imc] , 
                        (nt->mc_phi)[imc] , (nt->mc_e)[imc] );
      _truth_table->add_particle( imc , 
				  (nt->mc_pdgid)[imc] , 
				  boost::math::iround(detector::charge_from_pdg((nt->mc_pdgid)[imc])) ,
				  (nt->mc_status)[imc] ,
				  mom ,
				  (nt->mc_parent)[imc]==-1 ? &dummy : &((nt->mc_parent)[imc]),
				  (nt->mc_parent)[imc]==-1 ? &dummy : &((nt->mc_parent)[imc+1]),
				  &dummy, &dummy );
    }
    _truth_table->display_decay_tree();
    //_truth_table->write_graphviz( "./graph.dot" );
  }

  return true;
}

const bool
NtD3PDphysPeter::_get_event_V29g()
{
  const D3PDPeterTreeV29g* nt( dynamic_cast<const D3PDPeterTreeV29g*>(_madeclass.get()) );
  assert( nt );
  assert(nt->nele < D3PDPeterTreeV29g::nele_max);
  assert(nt->njet < D3PDPeterTreeV29g::njet_max);
  assert(nt->nmu < D3PDPeterTreeV29g::nmu_max);
  assert(nt->nvx < D3PDPeterTreeV29g::nvx_max);
  // matchvectors are really large, so this is important
  assert(nt->mu_trig_l1matchvector_ < D3PDPeterTreeV29g::matchvector_max);
  assert(nt->mu_trig_l2matchvector_ < D3PDPeterTreeV29g::matchvector_max);
  assert(nt->mu_trig_l2samatchvector_ < D3PDPeterTreeV29g::matchvector_max);
  assert(nt->mu_trig_efmatchvector_ < D3PDPeterTreeV29g::matchvector_max);
  assert(nt->ele_trig_l1matchvector_ < D3PDPeterTreeV29g::matchvector_max);
  assert(nt->ele_trig_l2matchvector_ < D3PDPeterTreeV29g::matchvector_max);
  assert(nt->ele_trig_efmatchvector_ < D3PDPeterTreeV29g::matchvector_max);

  _run_number = nt->run;
  _event_number = nt->event;
  _lumi_block = nt->lb;
  _average_mu = nt->lumi_mu_average; // encoded in a dedicated variable as of mc11a
  _actual_mu = nt->lumi_mu_actual;
  _mc_weight = nt->mc_weight;
  if(false) { // LHAPDF weights are disabled in V29g ntuple!
    _pdf_weights = boost::assign::list_of(nt->lha_cteq6ll)(nt->lha_MRSTMCal)(nt->lha_CT10)(nt->lha_MSTW2008lo68cl)(nt->lha_MSTW2008nlo68cl)(nt->lha_NNPDF21_100)(nt->lha_HERAPDF10_EIG)(nt->lha_cteq66);
  } else {
    _pdf_weights = boost::assign::list_of(1.0)(1.0)(1.0)(1.0)(1.0)(1.0)(1.0)(1.0);
  }
  _mc_channel = nt->mc_channel;
  _mc_hfor = nt->mc_hfor;

  // fill mcevt information
  _mcevt_event_scale = nt->mcevt_event_scale;
  _mcevt_alphaQCD = nt->mcevt_alphaQCD;
  _mcevt_alphaQED = nt->mcevt_alphaQED;
  _mcevt_pdf_scale = nt->mcevt_pdf_scale;
  _mcevt_id1 = nt->mcevt_id1;
  _mcevt_id2 = nt->mcevt_id2;
  _mcevt_pdf_x1 = nt->mcevt_pdf_x1;
  _mcevt_pdf_x2 = nt->mcevt_pdf_x2;
  _mcevt_pdf1 = nt->mcevt_pdf1;
  _mcevt_pdf2 = nt->mcevt_pdf2;

  // fill vertex information
  assert( nt );
  _vertices.reserve( static_cast<int>( nt->nvx ) );
  for( int iv=0, fv=nt->nvx; iv!=fv; ++iv ) {
    boost::shared_ptr<AnaVertex> v( new AnaVertex );
    v->_x = nt->vx_x[iv];
    v->_y = nt->vx_y[iv];
    v->_z = nt->vx_z[iv];
    v->_ntracks = nt->vx_ntracks[iv];
    v->_sum_pt = nt->vx_sumpt[iv];
    v->_type = detector::remap_vertex_type_enum( nt->vx_type[iv] );
    _vertices.push_back( v );
  }

  // fill muons
  assert( nt );
  _muons.reserve( static_cast<int>( nt->nmu ) );
  for( int i=0, f=nt->nmu; i!=f; ++i ) {
    // for each muon in the ntuple, create a separate AnaMuon for 
    // combined, ID, MS, and extrapolated MS muons
    boost::shared_ptr<AnaMuon> mu( new AnaMuon );
    mu->_all_author_bitmask = AnaMuon::MuonTypeBitset( nt->mu_author[i] );
    // override some definitions per Peter ntuple coding to ensure that we have *combined* muons
    mu->_all_author_bitmask.set(MuonAllAuthor::STACO,mu->_all_author_bitmask.test(MuonAllAuthor::STACO) && nt->mu_class[i]==1 && nt->mu_ptid[i]!=-1);
    mu->_all_author_bitmask.set(MuonAllAuthor::MuidCo,mu->_all_author_bitmask.test(MuonAllAuthor::MuidCo) && nt->mu_class[i]==2 && nt->mu_ptid[i]!=-1);
    // other muon variables
    mu->_primary_author = nt->mu_primauthor[i];
    mu->_aod_quality = nt->mu_quality[i];
    switch( nt->mu_class[i] ) { // other possibility is to use mu_type here
    case 1: mu->_aod_class = AnaMuon::AOD_STACO; break;
    case 2: mu->_aod_class = AnaMuon::AOD_MUID; break;
    case 3: mu->_aod_class = AnaMuon::AOD_CALO; break;
    default: mu->_aod_class = AnaMuon::AOD_UNKNOWN; break;
    }
    mu->_ntuple_index = i;
    mu->_z0 = nt->mu_z0[i];
    mu->_d0 = nt->mu_d0[i];
    mu->_d0_exPV_err = std::sqrt(nt->mu_covdxpv[i]);
    mu->_z0_exPV = nt->mu_zxpv[i];
    mu->_d0_exPV = nt->mu_dxpv[i];
    mu->_z0_vx = nt->mu_zvx[i];
    mu->_d0_vx = nt->mu_dvx[i];
    mu->_d0_vx_err = std::sqrt(nt->mu_covdvx[i]);
    mu->_charge = nt->mu_q[i];
    mu->_matchchi2 = nt->mu_matchchi2[i];
    mu->_id_charge = nt->mu_qid[i];
    mu->_ms_charge = nt->mu_qms[i];
    mu->_exms_charge = nt->mu_qexms[i];
    mu->_momentum.SetPtEtaPhiM( nt->mu_pt[i] ,nt->mu_eta[i] ,
                                nt->mu_phi[i], detector::muon_mass( detector::GeV ) );
    // isolation variables
    mu->_pt_cone = boost::assign::list_of(nt->mu_ptcone10[i])(nt->mu_ptcone20[i])(nt->mu_ptcone30[i])(nt->mu_ptcone40[i]);
    mu->_et_cone = boost::assign::list_of(nt->mu_etcone10[i])(nt->mu_etcone20[i])(nt->mu_etcone30[i])(nt->mu_etcone40[i]);
    // nhits, nholes et cetera
    mu->_expect_b_layer = nt->mu_expectbl[i];
    mu->_nhits = boost::assign::list_of(nt->mu_nhitpix[i])(nt->mu_nhitsct[i])(nt->mu_nhittrt[i])(nt->mu_noutlierstrt[i])(nt->mu_nhitbl[i]);
    mu->_nholes = boost::assign::list_of(nt->mu_nholespix[i])(nt->mu_nholessct[i])(nt->mu_nholestrt[i]);
    mu->_ndead = boost::assign::list_of(nt->mu_ndeadsensorspix[i])(nt->mu_ndeadsensorssct[i])(-1);
    // MET reconstruction
    mu->_met_wet = nt->mu_MET_wet[i];
    mu->_met_wpx = nt->mu_MET_wpx[i];
    mu->_met_wpy = nt->mu_MET_wpy[i];
    mu->_met_status = nt->mu_MET_status[i];
    mu->_met_wet2 = nt->mu_MET_wet2[i];
    mu->_met_wpx2 = nt->mu_MET_wpx2[i];
    mu->_met_wpy2 = nt->mu_MET_wpy2[i];
    mu->_met_status2 = nt->mu_MET_status2[i];
    // extrapolated muon spectrometer momentum:
    mu->_id_muon.SetPtEtaPhiM( nt->mu_ptid[i] ,nt->mu_etaid[i] ,
                               nt->mu_phiid[i], detector::muon_mass( detector::GeV ) );
    mu->_ms_muon.SetPtEtaPhiM( nt->mu_ptms[i] ,nt->mu_etams[i] ,
                               nt->mu_phims[i], detector::muon_mass( detector::GeV ) );
    mu->_exms_muon.SetPtEtaPhiM( nt->mu_ptexms[i] ,nt->mu_etaexms[i] ,
                                 nt->mu_phiexms[i], detector::muon_mass( detector::GeV ) );
    _make_associated_track_for_muon( mu , nt , i ); 
    _muons.push_back( mu );
  }

  // fill inner-detector tracks as ID-only muons [ for tag-and-probe ]
  // as an alternative, one can use CaloTag muons as probes
  if (false) {
    assert(nt->ntrk < D3PDPeterTreeV29g::ntrk_max);
    for( int i=0, f=nt->ntrk; i!=f; ++i ) {
      boost::shared_ptr<AnaMuon> mu( new AnaMuon );
      // other muon variables
      mu->_primary_author = MuonPrimAuthor::IDtrack;
      mu->_aod_class = AnaMuon::AOD_TRACK;
      mu->_ntuple_index = -1;
      mu->_d0_exPV_err = std::sqrt(nt->trk_covdxpv[i]);
      mu->_z0_exPV = nt->trk_zxpv[i];
      mu->_d0_exPV = nt->trk_dxpv[i];
      mu->_charge = nt->trk_q[i];
      const float sf = 1.0;
      mu->_momentum.SetPtEtaPhiM( sf*nt->trk_pt[i] ,nt->trk_eta[i] ,
				  nt->trk_phi[i], detector::muon_mass( detector::GeV ) );
      // isolation variables
      mu->_pt_cone = boost::assign::list_of(nt->trk_ptcone10[i])(nt->trk_ptcone20[i])(nt->trk_ptcone30[i])(nt->trk_ptcone40[i]);
      // nhits, nholes et cetera
      mu->_expect_b_layer = nt->trk_expectbl[i];
      mu->_nhits = boost::assign::list_of(nt->trk_nhitpix[i])(nt->trk_nhitsct[i])(nt->trk_nhittrt[i])(nt->trk_noutlierstrt[i])(nt->trk_nhitbl[i]);
      mu->_nholes = boost::assign::list_of(nt->trk_nholespix[i])(nt->trk_nholessct[i])(nt->trk_nholestrt[i]);
      mu->_ndead = boost::assign::list_of(nt->trk_ndeadsensorspix[i])(nt->trk_ndeadsensorssct[i])(-1);
      _muons.push_back( mu );
    }
  }

  // fill electrons
  if( true ) {
    for( int i=0, f=nt->nele; i!=f; ++i ) { 
      boost::shared_ptr<AnaElectron> ele( new AnaElectron );
      ele->_author = nt->ele_author[i];
      ele->_charge = nt->ele_q[i];
      ele->_momentum.SetPtEtaPhiM( nt->ele_et[i] , nt->ele_eta[i] , nt->ele_phi[i] , detector::electron_mass(detector::GeV) );
      ele->_is_em = nt->ele_isEM[i];
      ele->_oq = nt->ele_isgoodoq[i] ? 0 : 1446; // FIXME: store full bitmask once it is in the ntuple
      ele->_aod_quality = nt->ele_quality[i];
      ele->_rhad1 = nt->ele_rhad1[i];
      ele->_rhad = nt->ele_rhad[i];
      ele->_reta = nt->ele_reta[i];
      ele->_weta2 = nt->ele_w2[i];
      ele->_wstot = nt->ele_wstot[i];
      ele->_deltaeta = nt->ele_deltaeta[i];
      ele->_deltaphi = nt->ele_deltaphi[i];
      ele->_eoverp = detector::safe_divide( nt->ele_etclus[i] , nt->ele_pttrk[i] );
      ele->_demaxs1 = nt->ele_demaxs1[i]/1000.;
      ele->_eta2 = nt->ele_eta2[i];
      ele->_etap = nt->ele_etap[i];
      ele->_et2 = nt->ele_et2[i]/1000.;
      ele->_d0_exPV_err = std::sqrt(nt->ele_covdxpv[i]);
      ele->_z0_exPV = nt->ele_zxpv[i];
      ele->_d0_exPV = nt->ele_dxpv[i];
      ele->_d0_vx = nt->ele_dvx[i];
      ele->_d0_vx_err = std::sqrt(nt->ele_covdvx[i]);
      ele->_z0_vx = nt->ele_zvx[i];
      // MET reconstruction
      ele->_met_wet = nt->ele_MET_wet[i];
      ele->_met_wpx = nt->ele_MET_wpx[i];
      ele->_met_wpy = nt->ele_MET_wpy[i];
      ele->_met_status = nt->ele_MET_status[i];
      ele->_ntuple_index = i;
      ele->_pt_cone = boost::assign::list_of( -999. )( nt->ele_ptcone20[i] )( nt->ele_ptcone30[i] )( nt->ele_ptcone40[i] );
      ele->_uncorrected_et_cone = boost::assign::list_of( -999. )( nt->ele_etcone20[i] )( nt->ele_etcone30[i] )( nt->ele_etcone40[i] );
      ele->_et_cone = boost::assign::list_of( -999. )( nt->ele_etcone20_cor[i] )( nt->ele_etcone30_cor[i] )( nt->ele_etcone40_cor[i] );
      ele->_cluster_momentum.SetPtEtaPhiM( nt->ele_etclus[i] , nt->ele_etaclus[i] , nt->ele_phiclus[i] , detector::electron_mass(detector::GeV) );
      _make_associated_track_for_electron( ele , nt , i );
      ele->_nhits = boost::assign::list_of(nt->ele_nhitpix[i])(nt->ele_nhitsct[i])(nt->ele_nhitbl[i])(nt->ele_nhittrt[i]);
      ele->_noutliers = boost::assign::list_of(nt->ele_noutlierspix[i])(nt->ele_noutlierssct[i])(nt->ele_noutliersbl[i])(0);
      ele->_expect_b_layer = nt->ele_expectbl[i];
      _electrons.push_back( ele );
    }
  }

  // fill photons
  if( false ) {
    assert(nt->nph < D3PDPeterTreeV29g::nph_max);
    for( int i=0, f=nt->nph; i!=f; ++i ) { 
      boost::shared_ptr<AnaPhoton> ph( new AnaPhoton );
      ph->_author = nt->ph_author[i];
      ph->_momentum.SetPtEtaPhiM( nt->ph_et[i] , nt->ph_eta[i] , nt->ph_phi[i] , 0. );
      ph->_cluster_momentum.SetPtEtaPhiE( nt->ph_eclus[i] * std::sin(detector::thetaFromEta(nt->ph_etaclus[i])) , 
                                          nt->ph_etaclus[i] , nt->ph_phiclus[i] , nt->ph_eclus[i] );
      ph->_isEM = nt->ph_isEM[i];
      ph->_oq = nt->ph_oq[i];
      ph->_isgoodoq = nt->ph_isgoodoq[i];
      ph->_aod_quality = nt->ph_quality[i];
      ph->_et_cone = boost::assign::list_of( -999. )( nt->ph_etcone20_cor[i] )( nt->ph_etcone30_cor[i] )( nt->ph_etcone40_cor[i] );
      ph->_uncorrected_et_cone = boost::assign::list_of( -999. )( nt->ph_etcone20[i] )( nt->ph_etcone30[i] )( nt->ph_etcone40[i] );
      ph->_pt_cone = boost::assign::list_of( -999. )( nt->ph_ptcone20[i] )( nt->ph_ptcone30[i] )( nt->ph_ptcone40[i] );
      ph->_is_recovered = nt->ph_isRecovered[i];
      ph->_convFlag = nt->ph_convFlag[i];
      ph->_rconv = nt->ph_rconv[i];
      ph->_zconv = nt->ph_zconv[i];
      ph->_etas2 = nt->ph_etas2[i];
      ph->_ethad1 = nt->ph_ethad1[i];
      ph->_ethad = nt->ph_ethad[i];

      ph->_e277 = nt->ph_e277[i];
      ph->_e237 = nt->ph_e237[i];
      ph->_e233 = nt->ph_e233[i];
      ph->_weta2 = nt->ph_weta2[i];
      ph->_f1 = nt->ph_f1[i];
      ph->_emax = nt->ph_emax[i];
      ph->_emaxs2 = nt->ph_emaxs2[i];
      ph->_demaxs1 = nt->ph_demaxs1[i];
      ph->_fside = nt->ph_fside[i];
      ph->_wstot = nt->ph_wstot[i];
      ph->_ws3 = nt->ph_ws3[i];
      ph->_emins1 = nt->ph_emins1[i];
      ph->_etap = nt->ph_etap[i];
      ph->_ntuple_index = i;
      _photons.push_back( ph );
    }
  }

  // fill inner detector tracks as AnaTracks
  if( false ) {
    for( int i=0, f=nt->ntrk; i!=f; ++i ) { 
      boost::shared_ptr<AnaTrack> track( new AnaTrack );
      const AnaTrack::ftype trackpt = nt->trk_pt[i];
      track->_curv = detector::safe_divide(static_cast<AnaTrack::ftype>(1),trackpt)*detector::sign_of(nt->trk_q[i]); // 1/mev
      track->_phi0 = nt->trk_phi[i];
      track->_lambda = detector::cotThetaFromEta(nt->trk_eta[i]);
      if( nt->trk_dxpv[i] > -1E6 ) { // -1e6 in flat ntuple flags no vertex or similar error
	track->_exPV_d0 = nt->trk_dxpv[i];
	track->_exPV_d0_err = std::sqrt( nt->trk_covdxpv[i] );
	track->_exPV_z0 = nt->trk_zxpv[i];
      }
      track->_n_b_layer_hits = nt->trk_nhitbl[i];
      track->_n_pixel_hits = nt->trk_nhitpix[i];
      track->_n_sct_hits = nt->trk_nhitsct[i];
      track->_n_pixel_holes = nt->trk_nholespix[i];
      track->_n_sct_holes = nt->trk_nholessct[i];
      track->_nhits = track->_n_pixel_hits + track->_n_sct_hits;
      track->_nmisses =  track->_n_pixel_holes + track->_n_sct_holes;
      _offline_tracks.push_back(track);
    }
  }

  // fill jets
  assert( nt );
  _jets.reserve( static_cast<int>( nt->njet ) );
  for( int i=0, f=nt->njet; i!=f; ++i ) {
    boost::shared_ptr<AnaJet> jet( new AnaJet );
    switch( nt->jet_type[i] ) {
    case 0: jet->_alg = AnaJet::Algorithm::ATLAS_ANTI_KT_CONE_4_H1_TOPO; break;
    case 1: jet->_alg = AnaJet::Algorithm::ATLAS_ANTI_KT_CONE_6_H1_TOPO; break;
    case 2: jet->_alg = AnaJet::Algorithm::ATLAS_ANTI_KT_CONE_4_H1_LC_TOPO; break;
    case 3: jet->_alg = AnaJet::Algorithm::ATLAS_ANTI_KT_CONE_6_H1_LC_TOPO; break;
    case 4: jet->_alg = AnaJet::Algorithm::ATLAS_CONE_4_TRUTH; break;
    case 5: jet->_alg = AnaJet::Algorithm::ATLAS_CONE_4_TRUTH_PILEUP; break;
    case 6: jet->_alg = AnaJet::Algorithm::ATLAS_CONE_6_TRUTH; break;
    case 7: jet->_alg = AnaJet::Algorithm::ATLAS_CONE_6_TRUTH_PILEUP; break;
    case 8: jet->_alg = AnaJet::Algorithm::ATLAS_ANTI_KT_CONE_10_H1_LC_TOPO; break;
    default: assert(0&&"Unrecognized jet_type"); break;
    }
    if (jet->_alg == AnaJet::Algorithm::ATLAS_CONE_4_TRUTH_PILEUP) continue;
    if (jet->_alg == AnaJet::Algorithm::ATLAS_CONE_6_TRUTH_PILEUP) continue;
    jet->_jet_index = i;
    jet->_vtx_fraction = nt->jet_jvf[i];
    jet->_n90 = nt->jet_n90[i];
    jet->_timing = nt->jet_time[i];
    jet->_quality = nt->jet_larquality[i];
    jet->_HECQuality = nt->jet_hecquality[i];
    jet->_LArQuality = nt->jet_larquality[i];
    jet->_negativeE = nt->jet_negativee[i];
    jet->_sumPtTrack = nt->jet_sumpttrk[i];
    jet->_ntrkjetalg = nt->jet_ntrk[i];
    jet->_AverageLArQF = nt->jet_averagelarqf[i];
    jet->_emf = nt->jet_emf[i];
    jet->_fmax = nt->jet_fmax[i];
    jet->_BCH_CORR_CELL = nt->jet_bchcorr[i];
    jet->_BCH_CORR_JET = nt->jet_bchcorrjet[i];
    jet->_hecf = nt->jet_hecf[i];
    jet->_tgap3f = nt->jet_tgap3f[i];
    jet->_full_scale_momentum.SetPtEtaPhiM(nt->jet_pt[i],nt->jet_eta[i],nt->jet_phi[i],nt->jet_m[i]);
    // em scale for met cleaning cuts
    jet->_em_scale_momentum.SetPtEtaPhiM(nt->jet_pt_em[i],nt->jet_eta_em[i],nt->jet_phi_em[i],nt->jet_m_em[i]); 
    // const scale for LC jet calibration (used in MET computation)
    jet->_has_cs_scale = true;
    jet->_cs_scale_momentum.SetPtEtaPhiM(nt->jet_pt_cs[i],nt->jet_eta_cs[i],nt->jet_phi_cs[i],nt->jet_m_cs[i]); 
    // origin values
    jet->_EtaOriginEM = nt->jet_eta_origin_em[i];
    jet->_PhiOriginEM = nt->jet_phi_origin_em[i];
    jet->_MOriginEM = nt->jet_m_origin_em[i];
    jet->_EtaOrigin = nt->jet_eta_origin[i];
    jet->_PhiOrigin = nt->jet_phi_origin[i];
    jet->_MOrigin = nt->jet_m_origin[i];
    // MET reconstruction
    jet->_met_wet = nt->jet_MET_wet[i];
    jet->_met_wpx = nt->jet_MET_wpx[i];
    jet->_met_wpy = nt->jet_MET_wpy[i];
    jet->_met_status = nt->jet_MET_status[i];
    jet->_gcwjes = nt->jet_gcwjes[i];
    // other stuff
    jet->_tag_ip2d_w = nt->jet_tagIP2D[i];
    jet->_tag_ip3d_w = nt->jet_tagIP3D[i];
    jet->_tag_sv0_w = nt->jet_tagSV0[i];
    jet->_tag_sv1_w = nt->jet_tagSV1[i];
    jet->_tag_sv2_w = nt->jet_tagSV2[i];
    jet->_tag_jetp_w = nt->jet_tagJetProb[i];
    jet->_tag_cmbnn_w = nt->jet_tagJetFitterCOMBNN[i];
    jet->_tag_cmb_w = nt->jet_tagSV1[i]+nt->jet_tagIP3D[i];
    jet->_em_scale_correction = nt->jet_emjes[i];
    // finish off
    jet->_momentum = jet->_full_scale_momentum;
    _jets.push_back( jet );
  }


  // fill met
  assert( nt );
  assert( _met.empty() );
  boost::shared_ptr<AnaMET> met( new AnaMET );
  met->_met_type = AnaMET::REF_FINAL;
  met->_refgamma = nt->met_refgamma;
  met->_refgamma_phi = nt->met_refgamma_phi;
  met->_refgamma_sumet = nt->met_refgamma_sumet;
  met->_reftau = nt->met_reftau;
  met->_reftau_phi = nt->met_reftau_phi;
  met->_reftau_sumet = nt->met_reftau_sumet;
  met->_refmuon = nt->met_refmuon;
  met->_refmuon_phi = nt->met_refmuon_phi;
  met->_refmuon_sumet = nt->met_refmuon_sumet;
  
  met->_refmuon_track = nt->met_refmuon_track;
  met->_refmuon_track_phi = nt->met_refmuon_track_phi;
  met->_refmuon_track_sumet = nt->met_refmuon_track_sumet;
  TLorentzVector lochadtopo , muonboy , refmuon_track;
  lochadtopo.SetPtEtaPhiM( nt->met_lochadtopo , 0. , nt->met_lochadtopo_phi , 0. );
  muonboy.SetPtEtaPhiM( nt->met_muonboy , 0. , nt->met_muonboy_phi , 0. );
  refmuon_track.SetPtEtaPhiM( nt->met_refmuon_track , 0. , nt->met_refmuon_track_phi , 0. );
  TLorentzVector lochadtopo2 = lochadtopo + muonboy - refmuon_track;
  met->_lochad_topo_ichep = lochadtopo2.Pt();
  met->_lochad_topo_ichep_phi = lochadtopo2.Phi();
  
  met->_refele = nt->met_refele;
  met->_refele_phi = nt->met_refele_phi;
  met->_refele_sumet = nt->met_refele_sumet;
  met->_refjet = nt->met_refjet;
  met->_refjet_phi = nt->met_refjet_phi;
  met->_refjet_sumet = nt->met_refjet_sumet;
  met->_cellout_eflow = nt->met_cellout_eflow;
  met->_cellout_eflow_phi = nt->met_cellout_eflow_phi;
  met->_cellout_eflow_sumet = nt->met_cellout_eflow_sumet;
  met->_cellout = nt->met_cellout;
  met->_cellout_phi = nt->met_cellout_phi;
  met->_cellout_sumet = nt->met_cellout_sumet;  
  met->_softjets = nt->met_softjets;
  met->_softjets_phi = nt->met_softjets_phi;
  met->_softjets_sumet = nt->met_softjets_sumet;
  met->_cryo = nt->met_cryo;
  met->_cryo_phi = nt->met_cryo_phi;
  met->_cryo_sumet = nt->met_cryo_sumet;
  met->_muonboy = nt->met_muonboy;
  met->_muonboy_phi = nt->met_muonboy_phi;
  met->_muonboy_sumet = nt->met_muonboy_sumet;
  met->_lochad_topo = nt->met_lochadtopo;
  met->_lochad_topo_phi = nt->met_lochadtopo_phi;
  met->_lochad_topo_sumet = nt->met_lochadtopo_sumet;
  met->_truth_nonint = nt->met_truth_nonint;
  met->_truth_nonint_phi = nt->met_truth_nonint_phi;
  met->_truth_nonint_sumet = nt->met_truth_nonint_sumet;
  met->_momentum.SetPtEtaPhiM( nt->met_reffinal , 0. , nt->met_reffinal_phi , 0. );
  _met.push_back( met );
  if( true ) { 
    boost::shared_ptr<AnaMET> met( new AnaMET );
    met->_met_type = AnaMET::REF_FINAL_45;
    met->_momentum.SetPtEtaPhiM( nt->met_reffinal_eta45 , 0. , nt->met_reffinal_eta45_phi , 0. );
    _met.push_back( met );
  }

  if( true ) {
    boost::shared_ptr<AnaMET> met( new AnaMET );
    met->_met_type = AnaMET::LOCHADTOPO;
    met->_momentum.SetPtEtaPhiM( nt->met_lochadtopo , 0. , nt->met_lochadtopo_phi , 0. );
    _met.push_back( met );
  }
  if( true ) { 
    boost::shared_ptr<AnaMET> met( new AnaMET );
    met->_met_type = AnaMET::TRUTH;
    met->_momentum.SetPtEtaPhiM( nt->met_truth , 0. , nt->met_truth_phi , 0. );
    _met.push_back( met );
  }
  if( true ) { 
    boost::shared_ptr<AnaMET> met( new AnaMET );
    met->_met_type = AnaMET::TRUTH_NONINT;
    met->_momentum.SetPtEtaPhiM( nt->met_truth_nonint , 0. , nt->met_truth_nonint_phi , 0. );
    _met.push_back( met );
  }
  if( true ) { 
    boost::shared_ptr<AnaMET> met( new AnaMET );
    met->_met_type = AnaMET::TRUTH_INT;
    // met_truth is defined for Peter ntuples as MET_INT_ex + MET_MUONS_ex == MET_TRUTH_ex
    // so in order to get MET_INT component, we need to do a little math:
    double pt_x = nt->met_truth * cos(nt->met_truth_phi) - nt->met_truth_muons* cos(nt->met_truth_muons_phi);
    double pt_y = nt->met_truth * sin(nt->met_truth_phi) - nt->met_truth_muons* sin(nt->met_truth_muons_phi);
    met->_momentum.SetPxPyPzE( pt_x , pt_y , 0 , 0 );
    _met.push_back( met );
  }
  if( true ) { 
    boost::shared_ptr<AnaMET> met( new AnaMET );
    met->_met_type = AnaMET::TRUTH_MUONS;
    met->_momentum.SetPtEtaPhiM( nt->met_truth_muons , 0. , nt->met_truth_muons_phi , 0. );
    _met.push_back( met );
  }
  // fill trigger decision
  assert( nt );
  _trigger.reset( new AnaTrigger );
  _trigger->_lbn = nt->lb;
  _trigger->_run_number = nt->run;
  _trigger->_error_lar = nt->error_lar;
  // the following vector contain the entire event trigger record
  const std::vector<std::string>& L1 = *nt->trig_l1;
  const std::vector<std::string>& L2 = *nt->trig_l2;
  const std::vector<std::string>& EF = *nt->trig_ef;
  _trigger->_trig_l1_mu0 = _check_trigger(L1.begin(),L1.end(),"L1_MU0");
  _trigger->_trig_l1_mu6 = _check_trigger(L1.begin(),L1.end(),"L1_MU6");
  _trigger->_trig_l1_mu10 = _check_trigger(L1.begin(),L1.end(),"L1_MU10");
  _trigger->_trig_ef_mu10 = _check_trigger(EF.begin(),EF.end(),"EF_mu10");
  _trigger->_trig_ef_mu10_mg = _check_trigger(EF.begin(),EF.end(),"EF_mu10_MG");
  _trigger->_trig_ef_mu13_mg = _check_trigger(EF.begin(),EF.end(),"EF_mu13_MG");
  _trigger->_trig_ef_mu13_mg_tight = _check_trigger(EF.begin(),EF.end(),"EF_mu13_MG_tight");
  _trigger->_trig_ef_mu15_mu10_effs = _check_trigger(EF.begin(),EF.end(),"EF_mu15_mu10_EFFS");
  _trigger->_trig_ef_mu15_mu10_effs_medium = _check_trigger(EF.begin(),EF.end(),"EF_mu15_mu10_EFFS_medium");
  _trigger->_trig_ef_mu18 = _check_trigger(EF.begin(),EF.end(),"EF_mu18");
  _trigger->_trig_ef_mu18_mg = _check_trigger(EF.begin(),EF.end(),"EF_mu18_MG");
  _trigger->_trig_ef_mu20_mg = _check_trigger(EF.begin(),EF.end(),"EF_mu20_MG");
  _trigger->_trig_ef_mu22_mg = _check_trigger(EF.begin(),EF.end(),"EF_mu22_MG");
  _trigger->_trig_ef_mu40_msonly_barrel = _check_trigger(EF.begin(),EF.end(),"EF_mu40_MSonly_barrel");
  _trigger->_trig_ef_mu18_medium = _check_trigger(EF.begin(),EF.end(),"EF_mu18_medium");
  _trigger->_trig_ef_mu18_mg_medium = _check_trigger(EF.begin(),EF.end(),"EF_mu18_MG_medium");
  _trigger->_trig_ef_mu40_msonly_barrel_medium = _check_trigger(EF.begin(),EF.end(),"EF_mu40_MSonly_barrel_medium");
  _trigger->_trig_l1_em14 = _check_trigger(L1.begin(),L1.end(),"L1_EM14");
  _trigger->_trig_ef_e15_medium = _check_trigger(EF.begin(),EF.end(),"EF_e15_medium");
  _trigger->_trig_ef_e20_medium = _check_trigger(EF.begin(),EF.end(),"EF_e20_medium");
  _trigger->_trig_ef_e22_medium = _check_trigger(EF.begin(),EF.end(),"EF_e22_medium");
  _trigger->_trig_ef_e22vh_medium1 = _check_trigger(EF.begin(),EF.end(),"EF_e22vh_medium1");
  _trigger->_trig_ef_2e12_medium = _check_trigger(EF.begin(),EF.end(),"EF_2e12_medium");
  _trigger->_trig_ef_2e12t_medium = _check_trigger(EF.begin(),EF.end(),"EF_2e12T_medium");
  _trigger->_trig_ef_2e12tvh_medium = _check_trigger(EF.begin(),EF.end(),"EF_2e12Tvh_medium");
  _trigger->_trig_ef_2e10_medium = _check_trigger(EF.begin(),EF.end(),"EF_2e10_medium");
  _trigger->_trig_ef_2mu10 = _check_trigger(EF.begin(),EF.end(),"EF_2mu10");
  _trigger->_trig_ef_2mu10_loose = _check_trigger(EF.begin(),EF.end(),"EF_2mu10_loose");
  _trigger->_trig_ef_mu6 = _check_trigger(EF.begin(),EF.end(),"EF_mu6");
  _trigger->_trig_ef_g20_loose = _check_trigger(EF.begin(),EF.end(),"EF_g20_loose");
  _trigger->_trig_ef_j10_a4tc_effs = _check_trigger(EF.begin(),EF.end(),"EF_j10_a4tc_EFFS");
  _trigger->_trig_ef_fj10_a4tc_effs = _check_trigger(EF.begin(),EF.end(),"EF_fj10_a4tc_EFFS");
  _trigger->_trig_ef_j15_a4tc_effs = _check_trigger(EF.begin(),EF.end(),"EF_j15_a4tc_EFFS");
  _trigger->_trig_ef_fj15_a4tc_effs = _check_trigger(EF.begin(),EF.end(),"EF_fj15_a4tc_EFFS");
  _trigger->_trig_ef_j20_a4_effs = _check_trigger(EF.begin(),EF.end(),"EF_j20_a4_EFFS");
  _trigger->_trig_ef_j20_a4tc_effs = _check_trigger(EF.begin(),EF.end(),"EF_j20_a4tc_EFFS");
  _trigger->_trig_ef_fj20_a4tc_effs = _check_trigger(EF.begin(),EF.end(),"EF_fj20_a4tc_EFFS");
  _trigger->_trig_ef_xe70 = _check_trigger(EF.begin(),EF.end(),"EF_xe70");
  _trigger->_trig_ef_xe70_tclcw = _check_trigger(EF.begin(),EF.end(),"EF_xe70_tclcw");
  _trigger->_trig_ef_xe60 = _check_trigger(EF.begin(),EF.end(),"EF_xe60");
  _trigger->_trig_ef_xe60t = _check_trigger(EF.begin(),EF.end(),"EF_xe60T");
  _trigger->_trig_ef_j110_a4tchad_xe75_tclcw = _check_trigger(EF.begin(),EF.end(),"EF_j110_a4tchad_xe75_tclcw");
  _trigger->_trig_ef_j80_a4tchad_xe100_tclcw_veryloose = _check_trigger(EF.begin(),EF.end(),"EF_j80_a4tchad_xe100_tclcw_veryloose");


  // fill with Peter's trigger matching info
  for( int i=0; i!=nt->mu_trig_l1matchvector_; ++i ) { 
    const int i_ntuple_mu = nt->mu_trig_l1matchvector_first[i];
    const int i_trigger = nt->mu_trig_l1matchvector_second[i];
    const AnaMuon::ftype dr = (*(nt->mu_trig_l1matchdelr))[i];
    std::vector< boost::shared_ptr<const AnaMuon> >::iterator imu = 
      std::find_if( _muons.begin() , _muons.end() , boost::bind(&AnaMuon::ntuple_index,_1)==i_ntuple_mu );
    assert( i_trigger < L1.size() );
    if( imu != _muons.end() ) { (*imu)->_trigger_match_dr[ L1[i_trigger] ] = dr; }
  }
  for( int i=0; i!=nt->mu_trig_l2matchvector_; ++i ) { 
    const int i_ntuple_mu = nt->mu_trig_l2matchvector_first[i];
    const int i_trigger = nt->mu_trig_l2matchvector_second[i];
    const AnaMuon::ftype dr = (*(nt->mu_trig_l2matchdelr))[i];
    std::vector< boost::shared_ptr<const AnaMuon> >::iterator imu = 
      std::find_if( _muons.begin() , _muons.end() , boost::bind(&AnaMuon::ntuple_index,_1)==i_ntuple_mu );
    assert( i_trigger < L2.size() );
    if( imu != _muons.end() ) { (*imu)->_trigger_match_dr[ L2[i_trigger] ] = dr; }
  }
  for( int i=0; i!=nt->mu_trig_l2samatchvector_; ++i ) { 
    const int i_ntuple_mu = nt->mu_trig_l2samatchvector_first[i];
    const int i_trigger = nt->mu_trig_l2samatchvector_second[i];
    const AnaMuon::ftype dr = (*(nt->mu_trig_l2samatchdelr))[i];
    std::vector< boost::shared_ptr<const AnaMuon> >::iterator imu = 
      std::find_if( _muons.begin() , _muons.end() , boost::bind(&AnaMuon::ntuple_index,_1)==i_ntuple_mu );
    assert( i_trigger < L2.size() );
    if( imu != _muons.end() ) { (*imu)->_trigger_match_dr[ L2[i_trigger] ] = dr; }
  }
  for( int i=0; i!=nt->mu_trig_efmatchvector_; ++i ) { 
    const int i_ntuple_mu = nt->mu_trig_efmatchvector_first[i];
    const int i_trigger = nt->mu_trig_efmatchvector_second[i];
    const AnaMuon::ftype dr = (*(nt->mu_trig_efmatchdelr))[i];
    std::vector< boost::shared_ptr<const AnaMuon> >::iterator imu = 
      std::find_if( _muons.begin() , _muons.end() , boost::bind(&AnaMuon::ntuple_index,_1)==i_ntuple_mu );
    assert( i_trigger < EF.size() );
    if( imu != _muons.end() ) { (*imu)->_trigger_match_dr[ EF[i_trigger] ] = dr; }
  }

  for( int i=0; i!=nt->ele_trig_l1matchvector_; ++i ) { 
    const int i_ntuple_ele = nt->ele_trig_l1matchvector_first[i];
    const int i_trigger = nt->ele_trig_l1matchvector_second[i];
    const AnaElectron::ftype dr = (*(nt->ele_trig_l1matchdelr))[i];
    std::vector< boost::shared_ptr<const AnaElectron> >::iterator iele = 
      std::find_if( _electrons.begin() , _electrons.end() , boost::bind(&AnaElectron::ntuple_index,_1)==i_ntuple_ele );
    assert( i_trigger < L1.size() );
    if( iele != _electrons.end() ) { (*iele)->_trigger_match_dr[ L1[i_trigger] ] = dr; }
  }
  for( int i=0; i!=nt->ele_trig_l2matchvector_; ++i ) { 
    const int i_ntuple_ele = nt->ele_trig_l2matchvector_first[i];
    const int i_trigger = nt->ele_trig_l2matchvector_second[i];
    const AnaElectron::ftype dr = (*(nt->ele_trig_l2matchdelr))[i];
    std::vector< boost::shared_ptr<const AnaElectron> >::iterator iele = 
      std::find_if( _electrons.begin() , _electrons.end() , boost::bind(&AnaElectron::ntuple_index,_1)==i_ntuple_ele );
    assert( i_trigger < L2.size() );
    if( iele != _electrons.end() ) { (*iele)->_trigger_match_dr[ L2[i_trigger] ] = dr; }
  }
  for( int i=0; i!=nt->ele_trig_efmatchvector_; ++i ) { 
    const int i_ntuple_ele = nt->ele_trig_efmatchvector_first[i];
    const int i_trigger = nt->ele_trig_efmatchvector_second[i];
    const AnaElectron::ftype dr = (*(nt->ele_trig_efmatchdelr))[i];
    std::vector< boost::shared_ptr<const AnaElectron> >::iterator iele = 
      std::find_if( _electrons.begin() , _electrons.end() , boost::bind(&AnaElectron::ntuple_index,_1)==i_ntuple_ele );
    assert( i_trigger < EF.size() );
    if( iele != _electrons.end() ) { (*iele)->_trigger_match_dr[ EF[i_trigger] ] = dr; }
  }

  // fill truth particles
  assert( nt );
  assert(nt->nmc < D3PDPeterTreeV29g::nmc_max);
  _truth_particles.reserve( static_cast<int>( nt->nmc ) );
  for( int imc=0, fmc=nt->nmc; imc!=fmc; ++imc ) {
    boost::shared_ptr<AnaTruthParticle> v( new AnaTruthParticle );
    v->_pdg_id = (nt->mc_pdgid)[imc];
    const int jmc = (nt->mc_parent)[imc];
    v->_parent_index = jmc;
    v->_parent_pdg_id = jmc==-1 ? -1 : (nt->mc_pdgid)[jmc];
    v->_charge = detector::charge_from_pdg((nt->mc_pdgid)[imc]);
    v->_status = (nt->mc_status)[imc];
    v->_parent = (nt->mc_parent)[imc];
    v->_momentum.SetPtEtaPhiE( (nt->mc_pt)[imc] , (nt->mc_eta)[imc] , 
			       (nt->mc_phi)[imc] , (nt->mc_e)[imc] );
    _truth_particles.push_back( v );
  }
  
  // fill truth table
  if( false ) {
    _truth_table.reset( new AnaTruthTable );
    for( AnaTruthTable::index_type imc=0, fmc=nt->nmc; imc!=fmc; ++imc ) {
      static TLorentzVector mom;
      const int dummy(0);
      mom.SetPtEtaPhiE( (nt->mc_pt)[imc] , (nt->mc_eta)[imc] , 
                        (nt->mc_phi)[imc] , (nt->mc_e)[imc] );
      _truth_table->add_particle( imc , 
				  (nt->mc_pdgid)[imc] , 
				  boost::math::iround(detector::charge_from_pdg((nt->mc_pdgid)[imc])) ,
				  (nt->mc_status)[imc] ,
				  mom ,
				  (nt->mc_parent)[imc]==-1 ? &dummy : &((nt->mc_parent)[imc]),
				  (nt->mc_parent)[imc]==-1 ? &dummy : &((nt->mc_parent)[imc+1]),
				  &dummy, &dummy );
    }
    _truth_table->display_decay_tree();
    //_truth_table->write_graphviz( "./graph.dot" );
  }

  return true;
}

const bool
NtD3PDphysPeter::_get_event_V29i()
{
  const D3PDPeterTreeV29i* nt( dynamic_cast<const D3PDPeterTreeV29i*>(_madeclass.get()) );
  assert( nt );
  assert(nt->nele < D3PDPeterTreeV29i::nele_max);
  assert(nt->njet < D3PDPeterTreeV29i::njet_max);
  assert(nt->nmu < D3PDPeterTreeV29i::nmu_max);
  assert(nt->nvx < D3PDPeterTreeV29i::nvx_max);
  // matchvectors are really large, so this is important
  assert(nt->mu_trig_l1matchvector_ < D3PDPeterTreeV29i::matchvector_max);
  assert(nt->mu_trig_l2matchvector_ < D3PDPeterTreeV29i::matchvector_max);
  assert(nt->mu_trig_l2samatchvector_ < D3PDPeterTreeV29i::matchvector_max);
  assert(nt->mu_trig_efmatchvector_ < D3PDPeterTreeV29i::matchvector_max);
  assert(nt->ele_trig_l1matchvector_ < D3PDPeterTreeV29i::matchvector_max);
  assert(nt->ele_trig_l2matchvector_ < D3PDPeterTreeV29i::matchvector_max);
  assert(nt->ele_trig_efmatchvector_ < D3PDPeterTreeV29i::matchvector_max);

  _run_number = nt->run;
  _event_number = nt->event;
  _lumi_block = nt->lb;
  _average_mu = nt->lumi_mu_average; // encoded in a dedicated variable as of mc11a
  _actual_mu = nt->lumi_mu_actual;
  _mc_weight = nt->mc_weight;
  if(false) { // LHAPDF weights are disabled in V29i ntuple!
    _pdf_weights = boost::assign::list_of(nt->lha_cteq6ll)(nt->lha_MRSTMCal)(nt->lha_CT10)(nt->lha_MSTW2008lo68cl)(nt->lha_MSTW2008nlo68cl)(nt->lha_NNPDF21_100)(nt->lha_HERAPDF10_EIG)(nt->lha_cteq66);
  } else {
    _pdf_weights = boost::assign::list_of(1.0)(1.0)(1.0)(1.0)(1.0)(1.0)(1.0)(1.0);
  }
  _mc_channel = nt->mc_channel;
  _mc_hfor = nt->mc_hfor;

  // fill mcevt information
  _mcevt_event_scale = nt->mcevt_event_scale;
  _mcevt_alphaQCD = nt->mcevt_alphaQCD;
  _mcevt_alphaQED = nt->mcevt_alphaQED;
  _mcevt_pdf_scale = nt->mcevt_pdf_scale;
  _mcevt_id1 = nt->mcevt_id1;
  _mcevt_id2 = nt->mcevt_id2;
  _mcevt_pdf_x1 = nt->mcevt_pdf_x1;
  _mcevt_pdf_x2 = nt->mcevt_pdf_x2;
  _mcevt_pdf1 = nt->mcevt_pdf1;
  _mcevt_pdf2 = nt->mcevt_pdf2;

  // fill vertex information
  assert( nt );
  _vertices.reserve( static_cast<int>( nt->nvx ) );
  for( int iv=0, fv=nt->nvx; iv!=fv; ++iv ) {
    boost::shared_ptr<AnaVertex> v( new AnaVertex );
    v->_x = nt->vx_x[iv];
    v->_y = nt->vx_y[iv];
    v->_z = nt->vx_z[iv];
    v->_ntracks = nt->vx_ntracks[iv];
    v->_sum_pt = nt->vx_sumpt[iv];
    v->_type = detector::remap_vertex_type_enum( nt->vx_type[iv] );
    _vertices.push_back( v );
  }

  // fill muons
  assert( nt );
  _muons.reserve( static_cast<int>( nt->nmu ) );
  for( int i=0, f=nt->nmu; i!=f; ++i ) {
    // for each muon in the ntuple, create a separate AnaMuon for 
    // combined, ID, MS, and extrapolated MS muons
    boost::shared_ptr<AnaMuon> mu( new AnaMuon );
    mu->_all_author_bitmask = AnaMuon::MuonTypeBitset( nt->mu_author[i] );
    // override some definitions per Peter ntuple coding to ensure that we have *combined* muons
    mu->_all_author_bitmask.set(MuonAllAuthor::STACO,mu->_all_author_bitmask.test(MuonAllAuthor::STACO) && nt->mu_class[i]==1 && nt->mu_ptid[i]!=-1);
    mu->_all_author_bitmask.set(MuonAllAuthor::MuidCo,mu->_all_author_bitmask.test(MuonAllAuthor::MuidCo) && nt->mu_class[i]==2 && nt->mu_ptid[i]!=-1);
    // other muon variables
    mu->_primary_author = nt->mu_primauthor[i];
    mu->_aod_quality = nt->mu_quality[i];
    switch( nt->mu_class[i] ) { // other possibility is to use mu_type here
    case 1: mu->_aod_class = AnaMuon::AOD_STACO; break;
    case 2: mu->_aod_class = AnaMuon::AOD_MUID; break;
    case 3: mu->_aod_class = AnaMuon::AOD_CALO; break;
    default: mu->_aod_class = AnaMuon::AOD_UNKNOWN; break;
    }
    mu->_ntuple_index = i;
    mu->_z0 = nt->mu_z0[i];
    mu->_d0 = nt->mu_d0[i];
    mu->_d0_exPV_err = std::sqrt(nt->mu_covdxpv[i]);
    mu->_z0_exPV = nt->mu_zxpv[i];
    mu->_d0_exPV = nt->mu_dxpv[i];
    mu->_z0_vx = nt->mu_zvx[i];
    mu->_d0_vx = nt->mu_dvx[i];
    mu->_d0_vx_err = std::sqrt(nt->mu_covdvx[i]);
    mu->_charge = nt->mu_q[i];
    mu->_matchchi2 = nt->mu_matchchi2[i];
    mu->_id_charge = nt->mu_qid[i];
    mu->_ms_charge = nt->mu_qms[i];
    mu->_exms_charge = nt->mu_qexms[i];
    mu->_momentum.SetPtEtaPhiM( nt->mu_pt[i] ,nt->mu_eta[i] ,
                                nt->mu_phi[i], detector::muon_mass( detector::GeV ) );
    // isolation variables
    mu->_pt_cone = boost::assign::list_of(nt->mu_ptcone10[i])(nt->mu_ptcone20[i])(nt->mu_ptcone30[i])(nt->mu_ptcone40[i]);
    mu->_et_cone = boost::assign::list_of(nt->mu_etcone10[i])(nt->mu_etcone20[i])(nt->mu_etcone30[i])(nt->mu_etcone40[i]);
    // nhits, nholes et cetera
    mu->_expect_b_layer = nt->mu_expectbl[i];
    mu->_nhits = boost::assign::list_of(nt->mu_nhitpix[i])(nt->mu_nhitsct[i])(nt->mu_nhittrt[i])(nt->mu_noutlierstrt[i])(nt->mu_nhitbl[i]);
    mu->_nholes = boost::assign::list_of(nt->mu_nholespix[i])(nt->mu_nholessct[i])(nt->mu_nholestrt[i]);
    mu->_ndead = boost::assign::list_of(nt->mu_ndeadsensorspix[i])(nt->mu_ndeadsensorssct[i])(-1);
    // MET reconstruction
    mu->_met_wet = nt->mu_MET_wet[i];
    mu->_met_wpx = nt->mu_MET_wpx[i];
    mu->_met_wpy = nt->mu_MET_wpy[i];
    mu->_met_status = nt->mu_MET_status[i];
    mu->_met_wet2 = nt->mu_MET_wet2[i];
    mu->_met_wpx2 = nt->mu_MET_wpx2[i];
    mu->_met_wpy2 = nt->mu_MET_wpy2[i];
    mu->_met_status2 = nt->mu_MET_status2[i];
    // extrapolated muon spectrometer momentum:
    mu->_id_muon.SetPtEtaPhiM( nt->mu_ptid[i] ,nt->mu_etaid[i] ,
                               nt->mu_phiid[i], detector::muon_mass( detector::GeV ) );
    mu->_ms_muon.SetPtEtaPhiM( nt->mu_ptms[i] ,nt->mu_etams[i] ,
                               nt->mu_phims[i], detector::muon_mass( detector::GeV ) );
    mu->_exms_muon.SetPtEtaPhiM( nt->mu_ptexms[i] ,nt->mu_etaexms[i] ,
                                 nt->mu_phiexms[i], detector::muon_mass( detector::GeV ) );
    _make_associated_track_for_muon( mu , nt , i ); 
    _muons.push_back( mu );
  }

  // fill inner-detector tracks as ID-only muons [ for tag-and-probe ]
  // as an alternative, one can use CaloTag muons as probes
  if (false) {
    assert(nt->ntrk < D3PDPeterTreeV29i::ntrk_max);
    for( int i=0, f=nt->ntrk; i!=f; ++i ) {
      boost::shared_ptr<AnaMuon> mu( new AnaMuon );
      // other muon variables
      mu->_primary_author = MuonPrimAuthor::IDtrack;
      mu->_aod_class = AnaMuon::AOD_TRACK;
      mu->_ntuple_index = -1;
      mu->_d0_exPV_err = std::sqrt(nt->trk_covdxpv[i]);
      mu->_z0_exPV = nt->trk_zxpv[i];
      mu->_d0_exPV = nt->trk_dxpv[i];
      mu->_charge = nt->trk_q[i];
      const float sf = 1.0;
      mu->_momentum.SetPtEtaPhiM( sf*nt->trk_pt[i] ,nt->trk_eta[i] ,
				  nt->trk_phi[i], detector::muon_mass( detector::GeV ) );
      // isolation variables
      mu->_pt_cone = boost::assign::list_of(nt->trk_ptcone10[i])(nt->trk_ptcone20[i])(nt->trk_ptcone30[i])(nt->trk_ptcone40[i]);
      // nhits, nholes et cetera
      mu->_expect_b_layer = nt->trk_expectbl[i];
      mu->_nhits = boost::assign::list_of(nt->trk_nhitpix[i])(nt->trk_nhitsct[i])(nt->trk_nhittrt[i])(nt->trk_noutlierstrt[i])(nt->trk_nhitbl[i]);
      mu->_nholes = boost::assign::list_of(nt->trk_nholespix[i])(nt->trk_nholessct[i])(nt->trk_nholestrt[i]);
      mu->_ndead = boost::assign::list_of(nt->trk_ndeadsensorspix[i])(nt->trk_ndeadsensorssct[i])(-1);
      _muons.push_back( mu );
    }
  }

  // fill electrons
  if( true ) {
    for( int i=0, f=nt->nele; i!=f; ++i ) { 
      boost::shared_ptr<AnaElectron> ele( new AnaElectron );
      ele->_author = nt->ele_author[i];
      ele->_charge = nt->ele_q[i];
      ele->_momentum.SetPtEtaPhiM( nt->ele_et[i] , nt->ele_eta[i] , nt->ele_phi[i] , detector::electron_mass(detector::GeV) );
      ele->_is_em = nt->ele_isEM[i];
      ele->_oq = nt->ele_isgoodoq[i] ? 0 : 1446; // FIXME: store full bitmask once it is in the ntuple
      ele->_aod_quality = nt->ele_quality[i];
      ele->_rhad1 = nt->ele_rhad1[i];
      ele->_rhad = nt->ele_rhad[i];
      ele->_reta = nt->ele_reta[i];
      ele->_weta2 = nt->ele_w2[i];
      ele->_wstot = nt->ele_wstot[i];
      ele->_deltaeta = nt->ele_deltaeta[i];
      ele->_deltaphi = nt->ele_deltaphi[i];
      ele->_eoverp = detector::safe_divide( nt->ele_etclus[i] , nt->ele_pttrk[i] );
      ele->_demaxs1 = nt->ele_demaxs1[i]/1000.;
      ele->_eta2 = nt->ele_eta2[i];
      ele->_etap = nt->ele_etap[i];
      ele->_et2 = nt->ele_et2[i]/1000.;
      ele->_d0_exPV_err = std::sqrt(nt->ele_covdxpv[i]);
      ele->_z0_exPV = nt->ele_zxpv[i];
      ele->_d0_exPV = nt->ele_dxpv[i];
      ele->_d0_vx = nt->ele_dvx[i];
      ele->_d0_vx_err = std::sqrt(nt->ele_covdvx[i]);
      ele->_z0_vx = nt->ele_zvx[i];
      // MET reconstruction
      ele->_met_wet = nt->ele_MET_wet[i];
      ele->_met_wpx = nt->ele_MET_wpx[i];
      ele->_met_wpy = nt->ele_MET_wpy[i];
      ele->_met_status = nt->ele_MET_status[i];
      ele->_ntuple_index = i;
      ele->_pt_cone = boost::assign::list_of( -999. )( nt->ele_ptcone20[i] )( nt->ele_ptcone30[i] )( nt->ele_ptcone40[i] );
      ele->_uncorrected_et_cone = boost::assign::list_of( -999. )( nt->ele_etcone20[i] )( nt->ele_etcone30[i] )( nt->ele_etcone40[i] );
      ele->_et_cone = boost::assign::list_of( -999. )( nt->ele_etcone20_cor[i] )( nt->ele_etcone30_cor[i] )( nt->ele_etcone40_cor[i] );
      ele->_cluster_momentum.SetPtEtaPhiM( nt->ele_etclus[i] , nt->ele_etaclus[i] , nt->ele_phiclus[i] , detector::electron_mass(detector::GeV) );
      _make_associated_track_for_electron( ele , nt , i );
      ele->_nhits = boost::assign::list_of(nt->ele_nhitpix[i])(nt->ele_nhitsct[i])(nt->ele_nhitbl[i])(nt->ele_nhittrt[i]);
      ele->_noutliers = boost::assign::list_of(nt->ele_noutlierspix[i])(nt->ele_noutlierssct[i])(nt->ele_noutliersbl[i])(0);
      ele->_expect_b_layer = nt->ele_expectbl[i];
      _electrons.push_back( ele );
    }
  }

  // fill photons
  if( false ) {
    assert(nt->nph < D3PDPeterTreeV29i::nph_max);
    for( int i=0, f=nt->nph; i!=f; ++i ) { 
      boost::shared_ptr<AnaPhoton> ph( new AnaPhoton );
      ph->_author = nt->ph_author[i];
      ph->_momentum.SetPtEtaPhiM( nt->ph_et[i] , nt->ph_eta[i] , nt->ph_phi[i] , 0. );
      ph->_cluster_momentum.SetPtEtaPhiE( nt->ph_eclus[i] * std::sin(detector::thetaFromEta(nt->ph_etaclus[i])) , 
                                          nt->ph_etaclus[i] , nt->ph_phiclus[i] , nt->ph_eclus[i] );
      ph->_isEM = nt->ph_isEM[i];
      ph->_oq = nt->ph_oq[i];
      ph->_isgoodoq = nt->ph_isgoodoq[i];
      ph->_aod_quality = nt->ph_quality[i];
      ph->_et_cone = boost::assign::list_of( -999. )( nt->ph_etcone20_cor[i] )( nt->ph_etcone30_cor[i] )( nt->ph_etcone40_cor[i] );
      ph->_uncorrected_et_cone = boost::assign::list_of( -999. )( nt->ph_etcone20[i] )( nt->ph_etcone30[i] )( nt->ph_etcone40[i] );
      ph->_pt_cone = boost::assign::list_of( -999. )( nt->ph_ptcone20[i] )( nt->ph_ptcone30[i] )( nt->ph_ptcone40[i] );
      ph->_is_recovered = nt->ph_isRecovered[i];
      ph->_convFlag = nt->ph_convFlag[i];
      ph->_rconv = nt->ph_rconv[i];
      ph->_zconv = nt->ph_zconv[i];
      ph->_etas2 = nt->ph_etas2[i];
      ph->_ethad1 = nt->ph_ethad1[i];
      ph->_ethad = nt->ph_ethad[i];

      ph->_e277 = nt->ph_e277[i];
      ph->_e237 = nt->ph_e237[i];
      ph->_e233 = nt->ph_e233[i];
      ph->_weta2 = nt->ph_weta2[i];
      ph->_f1 = nt->ph_f1[i];
      ph->_emax = nt->ph_emax[i];
      ph->_emaxs2 = nt->ph_emaxs2[i];
      ph->_demaxs1 = nt->ph_demaxs1[i];
      ph->_fside = nt->ph_fside[i];
      ph->_wstot = nt->ph_wstot[i];
      ph->_ws3 = nt->ph_ws3[i];
      ph->_emins1 = nt->ph_emins1[i];
      ph->_etap = nt->ph_etap[i];
      ph->_ntuple_index = i;
      _photons.push_back( ph );
    }
  }

  // fill inner detector tracks as AnaTracks
  if( false ) {
    for( int i=0, f=nt->ntrk; i!=f; ++i ) { 
      boost::shared_ptr<AnaTrack> track( new AnaTrack );
      const AnaTrack::ftype trackpt = nt->trk_pt[i];
      track->_curv = detector::safe_divide(static_cast<AnaTrack::ftype>(1),trackpt)*detector::sign_of(nt->trk_q[i]); // 1/mev
      track->_phi0 = nt->trk_phi[i];
      track->_lambda = detector::cotThetaFromEta(nt->trk_eta[i]);
      if( nt->trk_dxpv[i] > -1E6 ) { // -1e6 in flat ntuple flags no vertex or similar error
	track->_exPV_d0 = nt->trk_dxpv[i];
	track->_exPV_d0_err = std::sqrt( nt->trk_covdxpv[i] );
	track->_exPV_z0 = nt->trk_zxpv[i];
      }
      track->_n_b_layer_hits = nt->trk_nhitbl[i];
      track->_n_pixel_hits = nt->trk_nhitpix[i];
      track->_n_sct_hits = nt->trk_nhitsct[i];
      track->_n_pixel_holes = nt->trk_nholespix[i];
      track->_n_sct_holes = nt->trk_nholessct[i];
      track->_nhits = track->_n_pixel_hits + track->_n_sct_hits;
      track->_nmisses =  track->_n_pixel_holes + track->_n_sct_holes;
      _offline_tracks.push_back(track);
    }
  }

  // fill jets
  assert( nt );
  _jets.reserve( static_cast<int>( nt->njet ) );
  for( int i=0, f=nt->njet; i!=f; ++i ) {
    boost::shared_ptr<AnaJet> jet( new AnaJet );
    switch( nt->jet_type[i] ) {
    case 0: jet->_alg = AnaJet::Algorithm::ATLAS_ANTI_KT_CONE_4_H1_TOPO; break;
    case 1: jet->_alg = AnaJet::Algorithm::ATLAS_ANTI_KT_CONE_6_H1_TOPO; break;
    case 2: jet->_alg = AnaJet::Algorithm::ATLAS_ANTI_KT_CONE_4_H1_LC_TOPO; break;
    case 3: jet->_alg = AnaJet::Algorithm::ATLAS_ANTI_KT_CONE_6_H1_LC_TOPO; break;
    case 4: jet->_alg = AnaJet::Algorithm::ATLAS_CONE_4_TRUTH; break;
    case 5: jet->_alg = AnaJet::Algorithm::ATLAS_CONE_4_TRUTH_PILEUP; break;
    case 6: jet->_alg = AnaJet::Algorithm::ATLAS_CONE_6_TRUTH; break;
    case 7: jet->_alg = AnaJet::Algorithm::ATLAS_CONE_6_TRUTH_PILEUP; break;
    case 8: jet->_alg = AnaJet::Algorithm::ATLAS_ANTI_KT_CONE_10_H1_LC_TOPO; break;
    default: assert(0&&"Unrecognized jet_type"); break;
    }
    if (jet->_alg == AnaJet::Algorithm::ATLAS_CONE_4_TRUTH_PILEUP) continue;
    if (jet->_alg == AnaJet::Algorithm::ATLAS_CONE_6_TRUTH_PILEUP) continue;
    jet->_jet_index = i;
    jet->_vtx_fraction = nt->jet_jvf[i];
    jet->_n90 = nt->jet_n90[i];
    jet->_timing = nt->jet_time[i];
    jet->_quality = nt->jet_larquality[i];
    jet->_HECQuality = nt->jet_hecquality[i];
    jet->_LArQuality = nt->jet_larquality[i];
    jet->_negativeE = nt->jet_negativee[i];
    jet->_sumPtTrack = nt->jet_sumpttrk[i];
    jet->_ntrkjetalg = nt->jet_ntrk[i];
    jet->_AverageLArQF = nt->jet_averagelarqf[i];
    jet->_emf = nt->jet_emf[i];
    jet->_fmax = nt->jet_fmax[i];
    jet->_BCH_CORR_CELL = nt->jet_bchcorr[i];
    jet->_BCH_CORR_JET = nt->jet_bchcorrjet[i];
    jet->_hecf = nt->jet_hecf[i];
    jet->_tgap3f = nt->jet_tgap3f[i];
    jet->_full_scale_momentum.SetPtEtaPhiM(nt->jet_pt[i],nt->jet_eta[i],nt->jet_phi[i],nt->jet_m[i]);
    // em scale for met cleaning cuts
    jet->_em_scale_momentum.SetPtEtaPhiM(nt->jet_pt_em[i],nt->jet_eta_em[i],nt->jet_phi_em[i],nt->jet_m_em[i]); 
    // const scale for LC jet calibration (used in MET computation)
    jet->_has_cs_scale = true;
    jet->_cs_scale_momentum.SetPtEtaPhiM(nt->jet_pt_cs[i],nt->jet_eta_cs[i],nt->jet_phi_cs[i],nt->jet_m_cs[i]); 
    // origin values
    jet->_EtaOriginEM = nt->jet_eta_origin_em[i];
    jet->_PhiOriginEM = nt->jet_phi_origin_em[i];
    jet->_MOriginEM = nt->jet_m_origin_em[i];
    jet->_EtaOrigin = nt->jet_eta_origin[i];
    jet->_PhiOrigin = nt->jet_phi_origin[i];
    jet->_MOrigin = nt->jet_m_origin[i];
    // MET reconstruction
    jet->_met_wet = nt->jet_MET_wet[i];
    jet->_met_wpx = nt->jet_MET_wpx[i];
    jet->_met_wpy = nt->jet_MET_wpy[i];
    jet->_met_status = nt->jet_MET_status[i];
    jet->_gcwjes = nt->jet_gcwjes[i];
    // other stuff
    jet->_tag_ip2d_w = nt->jet_tagIP2D[i];
    jet->_tag_ip3d_w = nt->jet_tagIP3D[i];
    jet->_tag_sv0_w = nt->jet_tagSV0[i];
    jet->_tag_sv1_w = nt->jet_tagSV1[i];
    jet->_tag_sv2_w = nt->jet_tagSV2[i];
    jet->_tag_jetp_w = nt->jet_tagJetProb[i];
    jet->_tag_cmbnn_w = nt->jet_tagJetFitterCOMBNN[i];
    jet->_tag_cmb_w = nt->jet_tagSV1[i]+nt->jet_tagIP3D[i];
    jet->_em_scale_correction = nt->jet_emjes[i];
    // finish off
    jet->_momentum = jet->_full_scale_momentum;
    _jets.push_back( jet );
  }


  // fill met
  assert( nt );
  assert( _met.empty() );
  boost::shared_ptr<AnaMET> met( new AnaMET );
  met->_met_type = AnaMET::REF_FINAL;
  met->_refgamma = nt->met_refgamma;
  met->_refgamma_phi = nt->met_refgamma_phi;
  met->_refgamma_sumet = nt->met_refgamma_sumet;
  met->_reftau = nt->met_reftau;
  met->_reftau_phi = nt->met_reftau_phi;
  met->_reftau_sumet = nt->met_reftau_sumet;
  met->_refmuon = nt->met_refmuon;
  met->_refmuon_phi = nt->met_refmuon_phi;
  met->_refmuon_sumet = nt->met_refmuon_sumet;

  met->_refmuon_track = nt->met_refmuon_track;
  met->_refmuon_track_phi = nt->met_refmuon_track_phi;
  met->_refmuon_track_sumet = nt->met_refmuon_track_sumet;
  TLorentzVector lochadtopo , muonboy , refmuon_track;
  lochadtopo.SetPtEtaPhiM( nt->met_lochadtopo , 0. , nt->met_lochadtopo_phi , 0. );
  muonboy.SetPtEtaPhiM( nt->met_muonboy , 0. , nt->met_muonboy_phi , 0. );
  refmuon_track.SetPtEtaPhiM( nt->met_refmuon_track , 0. , nt->met_refmuon_track_phi , 0. );
  TLorentzVector lochadtopo2 = lochadtopo + muonboy - refmuon_track;
  met->_lochad_topo_ichep = lochadtopo2.Pt();
  met->_lochad_topo_ichep_phi = lochadtopo2.Phi();

  met->_refele = nt->met_refele;
  met->_refele_phi = nt->met_refele_phi;
  met->_refele_sumet = nt->met_refele_sumet;
  met->_refjet = nt->met_refjet;
  met->_refjet_phi = nt->met_refjet_phi;
  met->_refjet_sumet = nt->met_refjet_sumet;
  met->_cellout_eflow = nt->met_cellout_eflow;
  met->_cellout_eflow_phi = nt->met_cellout_eflow_phi;
  met->_cellout_eflow_sumet = nt->met_cellout_eflow_sumet;
  met->_cellout = nt->met_cellout;
  met->_cellout_phi = nt->met_cellout_phi;
  met->_cellout_sumet = nt->met_cellout_sumet;  
  met->_softjets = nt->met_softjets;
  met->_softjets_phi = nt->met_softjets_phi;
  met->_softjets_sumet = nt->met_softjets_sumet;
  met->_cryo = nt->met_cryo;
  met->_cryo_phi = nt->met_cryo_phi;
  met->_cryo_sumet = nt->met_cryo_sumet;
  met->_muonboy = nt->met_muonboy;
  met->_muonboy_phi = nt->met_muonboy_phi;
  met->_muonboy_sumet = nt->met_muonboy_sumet;
  met->_lochad_topo = nt->met_lochadtopo;
  met->_lochad_topo_phi = nt->met_lochadtopo_phi;
  met->_lochad_topo_sumet = nt->met_lochadtopo_sumet;
  met->_truth_nonint = nt->met_truth_nonint;
  met->_truth_nonint_phi = nt->met_truth_nonint_phi;
  met->_truth_nonint_sumet = nt->met_truth_nonint_sumet;
  met->_momentum.SetPtEtaPhiM( nt->met_reffinal , 0. , nt->met_reffinal_phi , 0. );
  _met.push_back( met );
  if( true ) { 
    boost::shared_ptr<AnaMET> met( new AnaMET );
    met->_met_type = AnaMET::REF_FINAL_45;
    met->_momentum.SetPtEtaPhiM( nt->met_reffinal_eta45 , 0. , nt->met_reffinal_eta45_phi , 0. );
    _met.push_back( met );
  }

  if( true ) {
    boost::shared_ptr<AnaMET> met( new AnaMET );
    met->_met_type = AnaMET::LOCHADTOPO;
    met->_momentum.SetPtEtaPhiM( nt->met_lochadtopo , 0. , nt->met_lochadtopo_phi , 0. );
    _met.push_back( met );
  }
  if( true ) { 
    boost::shared_ptr<AnaMET> met( new AnaMET );
    met->_met_type = AnaMET::TRUTH;
    met->_momentum.SetPtEtaPhiM( nt->met_truth , 0. , nt->met_truth_phi , 0. );
    _met.push_back( met );
  }
  if( true ) { 
    boost::shared_ptr<AnaMET> met( new AnaMET );
    met->_met_type = AnaMET::TRUTH_NONINT;
    met->_momentum.SetPtEtaPhiM( nt->met_truth_nonint , 0. , nt->met_truth_nonint_phi , 0. );
    _met.push_back( met );
  }
  if( true ) { 
    boost::shared_ptr<AnaMET> met( new AnaMET );
    met->_met_type = AnaMET::TRUTH_INT;
    // met_truth is defined for Peter ntuples as MET_INT_ex + MET_MUONS_ex == MET_TRUTH_ex
    // so in order to get MET_INT component, we need to do a little math:
    double pt_x = nt->met_truth * cos(nt->met_truth_phi) - nt->met_truth_muons* cos(nt->met_truth_muons_phi);
    double pt_y = nt->met_truth * sin(nt->met_truth_phi) - nt->met_truth_muons* sin(nt->met_truth_muons_phi);
    met->_momentum.SetPxPyPzE( pt_x , pt_y , 0 , 0 );
    _met.push_back( met );
  }
  if( true ) { 
    boost::shared_ptr<AnaMET> met( new AnaMET );
    met->_met_type = AnaMET::TRUTH_MUONS;
    met->_momentum.SetPtEtaPhiM( nt->met_truth_muons , 0. , nt->met_truth_muons_phi , 0. );
    _met.push_back( met );
  }
  // fill trigger decision
  assert( nt );
  _trigger.reset( new AnaTrigger );
  _trigger->_lbn = nt->lb;
  _trigger->_run_number = nt->run;
  _trigger->_error_lar = nt->error_lar;
  // the following vector contain the entire event trigger record
  const std::vector<std::string>& L1 = *nt->trig_l1;
  const std::vector<std::string>& L2 = *nt->trig_l2;
  const std::vector<std::string>& EF = *nt->trig_ef;
  _trigger->_trig_l1_mu0 = _check_trigger(L1.begin(),L1.end(),"L1_MU0");
  _trigger->_trig_l1_mu6 = _check_trigger(L1.begin(),L1.end(),"L1_MU6");
  _trigger->_trig_l1_mu10 = _check_trigger(L1.begin(),L1.end(),"L1_MU10");
  _trigger->_trig_ef_mu10 = _check_trigger(EF.begin(),EF.end(),"EF_mu10");
  _trigger->_trig_ef_mu10_mg = _check_trigger(EF.begin(),EF.end(),"EF_mu10_MG");
  _trigger->_trig_ef_mu13_mg = _check_trigger(EF.begin(),EF.end(),"EF_mu13_MG");
  _trigger->_trig_ef_mu13_mg_tight = _check_trigger(EF.begin(),EF.end(),"EF_mu13_MG_tight");
  _trigger->_trig_ef_mu15_mu10_effs = _check_trigger(EF.begin(),EF.end(),"EF_mu15_mu10_EFFS");
  _trigger->_trig_ef_mu15_mu10_effs_medium = _check_trigger(EF.begin(),EF.end(),"EF_mu15_mu10_EFFS_medium");
  _trigger->_trig_ef_mu18 = _check_trigger(EF.begin(),EF.end(),"EF_mu18");
  _trigger->_trig_ef_mu18_mg = _check_trigger(EF.begin(),EF.end(),"EF_mu18_MG");
  _trigger->_trig_ef_mu20_mg = _check_trigger(EF.begin(),EF.end(),"EF_mu20_MG");
  _trigger->_trig_ef_mu22_mg = _check_trigger(EF.begin(),EF.end(),"EF_mu22_MG");
  _trigger->_trig_ef_mu40_msonly_barrel = _check_trigger(EF.begin(),EF.end(),"EF_mu40_MSonly_barrel");
  _trigger->_trig_ef_mu18_medium = _check_trigger(EF.begin(),EF.end(),"EF_mu18_medium");
  _trigger->_trig_ef_mu18_mg_medium = _check_trigger(EF.begin(),EF.end(),"EF_mu18_MG_medium");
  _trigger->_trig_ef_mu40_msonly_barrel_medium = _check_trigger(EF.begin(),EF.end(),"EF_mu40_MSonly_barrel_medium");
  _trigger->_trig_l1_em14 = _check_trigger(L1.begin(),L1.end(),"L1_EM14");
  _trigger->_trig_ef_e15_medium = _check_trigger(EF.begin(),EF.end(),"EF_e15_medium");
  _trigger->_trig_ef_e20_medium = _check_trigger(EF.begin(),EF.end(),"EF_e20_medium");
  _trigger->_trig_ef_e22_medium = _check_trigger(EF.begin(),EF.end(),"EF_e22_medium");
  _trigger->_trig_ef_e22vh_medium1 = _check_trigger(EF.begin(),EF.end(),"EF_e22vh_medium1");
  _trigger->_trig_ef_2e12_medium = _check_trigger(EF.begin(),EF.end(),"EF_2e12_medium");
  _trigger->_trig_ef_2e12t_medium = _check_trigger(EF.begin(),EF.end(),"EF_2e12T_medium");
  _trigger->_trig_ef_2e12tvh_medium = _check_trigger(EF.begin(),EF.end(),"EF_2e12Tvh_medium");
  _trigger->_trig_ef_2e10_medium = _check_trigger(EF.begin(),EF.end(),"EF_2e10_medium");
  _trigger->_trig_ef_2mu10 = _check_trigger(EF.begin(),EF.end(),"EF_2mu10");
  _trigger->_trig_ef_2mu10_loose = _check_trigger(EF.begin(),EF.end(),"EF_2mu10_loose");
  _trigger->_trig_ef_mu6 = _check_trigger(EF.begin(),EF.end(),"EF_mu6");
  _trigger->_trig_ef_g20_loose = _check_trigger(EF.begin(),EF.end(),"EF_g20_loose");
  _trigger->_trig_ef_j10_a4tc_effs = _check_trigger(EF.begin(),EF.end(),"EF_j10_a4tc_EFFS");
  _trigger->_trig_ef_fj10_a4tc_effs = _check_trigger(EF.begin(),EF.end(),"EF_fj10_a4tc_EFFS");
  _trigger->_trig_ef_j15_a4tc_effs = _check_trigger(EF.begin(),EF.end(),"EF_j15_a4tc_EFFS");
  _trigger->_trig_ef_fj15_a4tc_effs = _check_trigger(EF.begin(),EF.end(),"EF_fj15_a4tc_EFFS");
  _trigger->_trig_ef_j20_a4_effs = _check_trigger(EF.begin(),EF.end(),"EF_j20_a4_EFFS");
  _trigger->_trig_ef_j20_a4tc_effs = _check_trigger(EF.begin(),EF.end(),"EF_j20_a4tc_EFFS");
  _trigger->_trig_ef_fj20_a4tc_effs = _check_trigger(EF.begin(),EF.end(),"EF_fj20_a4tc_EFFS");
  _trigger->_trig_ef_xe70 = _check_trigger(EF.begin(),EF.end(),"EF_xe70");
  _trigger->_trig_ef_xe70_tclcw = _check_trigger(EF.begin(),EF.end(),"EF_xe70_tclcw");
  _trigger->_trig_ef_xe60 = _check_trigger(EF.begin(),EF.end(),"EF_xe60");
  _trigger->_trig_ef_xe60t = _check_trigger(EF.begin(),EF.end(),"EF_xe60T");
  _trigger->_trig_ef_j110_a4tchad_xe75_tclcw = _check_trigger(EF.begin(),EF.end(),"EF_j110_a4tchad_xe75_tclcw");
  _trigger->_trig_ef_j80_a4tchad_xe100_tclcw_veryloose = _check_trigger(EF.begin(),EF.end(),"EF_j80_a4tchad_xe100_tclcw_veryloose");


  // fill with Peter's trigger matching info
  for( int i=0; i!=nt->mu_trig_l1matchvector_; ++i ) { 
    const int i_ntuple_mu = nt->mu_trig_l1matchvector_first[i];
    const int i_trigger = nt->mu_trig_l1matchvector_second[i];
    const AnaMuon::ftype dr = (*(nt->mu_trig_l1matchdelr))[i];
    std::vector< boost::shared_ptr<const AnaMuon> >::iterator imu = 
      std::find_if( _muons.begin() , _muons.end() , boost::bind(&AnaMuon::ntuple_index,_1)==i_ntuple_mu );
    assert( i_trigger < L1.size() );
    if( imu != _muons.end() ) { (*imu)->_trigger_match_dr[ L1[i_trigger] ] = dr; }
  }
  for( int i=0; i!=nt->mu_trig_l2matchvector_; ++i ) { 
    const int i_ntuple_mu = nt->mu_trig_l2matchvector_first[i];
    const int i_trigger = nt->mu_trig_l2matchvector_second[i];
    const AnaMuon::ftype dr = (*(nt->mu_trig_l2matchdelr))[i];
    std::vector< boost::shared_ptr<const AnaMuon> >::iterator imu = 
      std::find_if( _muons.begin() , _muons.end() , boost::bind(&AnaMuon::ntuple_index,_1)==i_ntuple_mu );
    assert( i_trigger < L2.size() );
    if( imu != _muons.end() ) { (*imu)->_trigger_match_dr[ L2[i_trigger] ] = dr; }
  }
  for( int i=0; i!=nt->mu_trig_l2samatchvector_; ++i ) { 
    const int i_ntuple_mu = nt->mu_trig_l2samatchvector_first[i];
    const int i_trigger = nt->mu_trig_l2samatchvector_second[i];
    const AnaMuon::ftype dr = (*(nt->mu_trig_l2samatchdelr))[i];
    std::vector< boost::shared_ptr<const AnaMuon> >::iterator imu = 
      std::find_if( _muons.begin() , _muons.end() , boost::bind(&AnaMuon::ntuple_index,_1)==i_ntuple_mu );
    assert( i_trigger < L2.size() );
    if( imu != _muons.end() ) { (*imu)->_trigger_match_dr[ L2[i_trigger] ] = dr; }
  }
  for( int i=0; i!=nt->mu_trig_efmatchvector_; ++i ) { 
    const int i_ntuple_mu = nt->mu_trig_efmatchvector_first[i];
    const int i_trigger = nt->mu_trig_efmatchvector_second[i];
    const AnaMuon::ftype dr = (*(nt->mu_trig_efmatchdelr))[i];
    std::vector< boost::shared_ptr<const AnaMuon> >::iterator imu = 
      std::find_if( _muons.begin() , _muons.end() , boost::bind(&AnaMuon::ntuple_index,_1)==i_ntuple_mu );
    assert( i_trigger < EF.size() );
    if( imu != _muons.end() ) { (*imu)->_trigger_match_dr[ EF[i_trigger] ] = dr; }
  }

  for( int i=0; i!=nt->ele_trig_l1matchvector_; ++i ) { 
    const int i_ntuple_ele = nt->ele_trig_l1matchvector_first[i];
    const int i_trigger = nt->ele_trig_l1matchvector_second[i];
    const AnaElectron::ftype dr = (*(nt->ele_trig_l1matchdelr))[i];
    std::vector< boost::shared_ptr<const AnaElectron> >::iterator iele = 
      std::find_if( _electrons.begin() , _electrons.end() , boost::bind(&AnaElectron::ntuple_index,_1)==i_ntuple_ele );
    assert( i_trigger < L1.size() );
    if( iele != _electrons.end() ) { (*iele)->_trigger_match_dr[ L1[i_trigger] ] = dr; }
  }
  for( int i=0; i!=nt->ele_trig_l2matchvector_; ++i ) { 
    const int i_ntuple_ele = nt->ele_trig_l2matchvector_first[i];
    const int i_trigger = nt->ele_trig_l2matchvector_second[i];
    const AnaElectron::ftype dr = (*(nt->ele_trig_l2matchdelr))[i];
    std::vector< boost::shared_ptr<const AnaElectron> >::iterator iele = 
      std::find_if( _electrons.begin() , _electrons.end() , boost::bind(&AnaElectron::ntuple_index,_1)==i_ntuple_ele );
    assert( i_trigger < L2.size() );
    if( iele != _electrons.end() ) { (*iele)->_trigger_match_dr[ L2[i_trigger] ] = dr; }
  }
  for( int i=0; i!=nt->ele_trig_efmatchvector_; ++i ) { 
    const int i_ntuple_ele = nt->ele_trig_efmatchvector_first[i];
    const int i_trigger = nt->ele_trig_efmatchvector_second[i];
    const AnaElectron::ftype dr = (*(nt->ele_trig_efmatchdelr))[i];
    std::vector< boost::shared_ptr<const AnaElectron> >::iterator iele = 
      std::find_if( _electrons.begin() , _electrons.end() , boost::bind(&AnaElectron::ntuple_index,_1)==i_ntuple_ele );
    assert( i_trigger < EF.size() );
    if( iele != _electrons.end() ) { (*iele)->_trigger_match_dr[ EF[i_trigger] ] = dr; }
  }

  // fill truth particles
  assert( nt );
  assert(nt->nmc < D3PDPeterTreeV29i::nmc_max);
  _truth_particles.reserve( static_cast<int>( nt->nmc ) );
  for( int imc=0, fmc=nt->nmc; imc!=fmc; ++imc ) {
    boost::shared_ptr<AnaTruthParticle> v( new AnaTruthParticle );
    v->_pdg_id = (nt->mc_pdgid)[imc];
    const int jmc = (nt->mc_parent)[imc];
    v->_parent_index = jmc;
    v->_parent_pdg_id = jmc==-1 ? -1 : (nt->mc_pdgid)[jmc];
    v->_charge = detector::charge_from_pdg((nt->mc_pdgid)[imc]);
    v->_status = (nt->mc_status)[imc];
    v->_vx_z = (nt->mc_vx_z)[imc];
    v->_parent = (nt->mc_parent)[imc];
    v->_momentum.SetPtEtaPhiE( (nt->mc_pt)[imc] , (nt->mc_eta)[imc] , 
			       (nt->mc_phi)[imc] , (nt->mc_e)[imc] );
    _truth_particles.push_back( v );
  }
  
  // fill truth table
  if( false ) {
    _truth_table.reset( new AnaTruthTable );
    for( AnaTruthTable::index_type imc=0, fmc=nt->nmc; imc!=fmc; ++imc ) {
      static TLorentzVector mom;
      const int dummy(0);
      mom.SetPtEtaPhiE( (nt->mc_pt)[imc] , (nt->mc_eta)[imc] , 
                        (nt->mc_phi)[imc] , (nt->mc_e)[imc] );
      _truth_table->add_particle( imc , 
				  (nt->mc_pdgid)[imc] , 
				  boost::math::iround(detector::charge_from_pdg((nt->mc_pdgid)[imc])) ,
				  (nt->mc_status)[imc] ,
				  mom ,
				  (nt->mc_parent)[imc]==-1 ? &dummy : &((nt->mc_parent)[imc]),
				  (nt->mc_parent)[imc]==-1 ? &dummy : &((nt->mc_parent)[imc+1]),
				  &dummy, &dummy );
    }
    _truth_table->display_decay_tree();
    //_truth_table->write_graphviz( "./graph.dot" );
  }

  return true;
}

// super-slimmed version of v29i for w asymmetry analysis
const bool
NtD3PDphysPeter::_get_event_V29l()
{
  const D3PDPeterTreeV29l* nt( dynamic_cast<const D3PDPeterTreeV29l*>(_madeclass.get()) );
  assert( nt );
  assert(nt->njet < D3PDPeterTreeV29l::njet_max);
  assert(nt->nmu < D3PDPeterTreeV29l::nmu_max);
  assert(nt->nvx < D3PDPeterTreeV29l::nvx_max);
  // matchvectors are really large, so this is important
  assert(nt->mu_trig_efmatchvector_ < D3PDPeterTreeV29l::matchvector_max);

  _run_number = nt->run;
  _event_number = nt->event;
  _lumi_block = nt->lb;
  _average_mu = nt->lumi_mu_average; // encoded in a dedicated variable as of mc11a
  _actual_mu = nt->lumi_mu_actual;
  _mc_weight = nt->mc_weight;
  _pdf_weights = boost::assign::list_of(1.0)(1.0)(1.0)(1.0)(1.0)(1.0)(1.0)(1.0);
  _mc_channel = nt->mc_channel;
  _mc_hfor = nt->mc_hfor;

  // fill mcevt information
  _mcevt_event_scale = nt->mcevt_event_scale;
  _mcevt_alphaQCD = nt->mcevt_alphaQCD;
  _mcevt_alphaQED = nt->mcevt_alphaQED;
  _mcevt_pdf_scale = nt->mcevt_pdf_scale;
  _mcevt_id1 = nt->mcevt_id1;
  _mcevt_id2 = nt->mcevt_id2;
  _mcevt_pdf_x1 = nt->mcevt_pdf_x1;
  _mcevt_pdf_x2 = nt->mcevt_pdf_x2;
  _mcevt_pdf1 = nt->mcevt_pdf1;
  _mcevt_pdf2 = nt->mcevt_pdf2;

  // fill vertex information
  assert( nt );
  _vertices.reserve( static_cast<int>( nt->nvx ) );
  for( int iv=0, fv=nt->nvx; iv!=fv; ++iv ) {
    boost::shared_ptr<AnaVertex> v( new AnaVertex );
    v->_x = nt->vx_x[iv];
    v->_y = nt->vx_y[iv];
    v->_z = nt->vx_z[iv];
    v->_ntracks = nt->vx_ntracks[iv];
    v->_sum_pt = nt->vx_sumpt[iv];
    v->_type = detector::remap_vertex_type_enum( nt->vx_type[iv] );
    _vertices.push_back( v );
  }

  // fill muons
  assert( nt );
  _muons.reserve( static_cast<int>( nt->nmu ) );
  for( int i=0, f=nt->nmu; i!=f; ++i ) {
    // for each muon in the ntuple, create a separate AnaMuon for 
    // combined, ID, MS, and extrapolated MS muons
    boost::shared_ptr<AnaMuon> mu( new AnaMuon );
    mu->_all_author_bitmask = AnaMuon::MuonTypeBitset( nt->mu_author[i] );
    // override some definitions per Peter ntuple coding to ensure that we have *combined* muons
    mu->_all_author_bitmask.set(MuonAllAuthor::STACO,mu->_all_author_bitmask.test(MuonAllAuthor::STACO) && nt->mu_class[i]==1 && nt->mu_ptid[i]!=-1);
    mu->_all_author_bitmask.set(MuonAllAuthor::MuidCo,mu->_all_author_bitmask.test(MuonAllAuthor::MuidCo) && nt->mu_class[i]==2 && nt->mu_ptid[i]!=-1);
    // other muon variables
    mu->_primary_author = nt->mu_primauthor[i];
    mu->_aod_quality = nt->mu_quality[i];
    switch( nt->mu_class[i] ) { // other possibility is to use mu_type here
    case 1: mu->_aod_class = AnaMuon::AOD_STACO; break;
    case 2: mu->_aod_class = AnaMuon::AOD_MUID; break;
    case 3: mu->_aod_class = AnaMuon::AOD_CALO; break;
    default: mu->_aod_class = AnaMuon::AOD_UNKNOWN; break;
    }
    mu->_ntuple_index = i;
    mu->_z0 = nt->mu_z0[i];
    mu->_d0 = nt->mu_d0[i];
    mu->_d0_exPV_err = std::sqrt(nt->mu_covdxpv[i]);
    mu->_z0_exPV = nt->mu_zxpv[i];
    mu->_d0_exPV = nt->mu_dxpv[i];
    mu->_z0_vx = nt->mu_zvx[i];
    mu->_d0_vx = nt->mu_dvx[i];
    mu->_d0_vx_err = std::sqrt(nt->mu_covdvx[i]);
    mu->_charge = nt->mu_q[i];
    mu->_id_charge = nt->mu_qid[i];
    mu->_ms_charge = nt->mu_qms[i];
    mu->_exms_charge = nt->mu_qexms[i];
    mu->_momentum.SetPtEtaPhiM( nt->mu_pt[i] ,nt->mu_eta[i] ,
                                nt->mu_phi[i], detector::muon_mass( detector::GeV ) );
    // isolation variables
    mu->_pt_cone = boost::assign::list_of(nt->mu_ptcone10[i])(nt->mu_ptcone20[i])(nt->mu_ptcone30[i])(nt->mu_ptcone40[i]);
    mu->_et_cone = boost::assign::list_of(nt->mu_etcone10[i])(nt->mu_etcone20[i])(nt->mu_etcone30[i])(nt->mu_etcone40[i]);
    // nhits, nholes et cetera
    mu->_expect_b_layer = nt->mu_expectbl[i];
    mu->_nhits = boost::assign::list_of(nt->mu_nhitpix[i])(nt->mu_nhitsct[i])(nt->mu_nhittrt[i])(nt->mu_noutlierstrt[i])(nt->mu_nhitbl[i]);
    mu->_nholes = boost::assign::list_of(nt->mu_nholespix[i])(nt->mu_nholessct[i])(nt->mu_nholestrt[i]);
    mu->_ndead = boost::assign::list_of(nt->mu_ndeadsensorspix[i])(nt->mu_ndeadsensorssct[i])(-1);
    // MET reconstruction
    mu->_met_wet = nt->mu_MET_wet[i];
    mu->_met_wpx = nt->mu_MET_wpx[i];
    mu->_met_wpy = nt->mu_MET_wpy[i];
    mu->_met_status = nt->mu_MET_status[i];
    mu->_met_wet2 = nt->mu_MET_wet2[i];
    mu->_met_wpx2 = nt->mu_MET_wpx2[i];
    mu->_met_wpy2 = nt->mu_MET_wpy2[i];
    mu->_met_status2 = nt->mu_MET_status2[i];
    // extrapolated muon spectrometer momentum:
    mu->_id_muon.SetPtEtaPhiM( nt->mu_ptid[i] ,nt->mu_etaid[i] ,
                               nt->mu_phiid[i], detector::muon_mass( detector::GeV ) );
    mu->_ms_muon.SetPtEtaPhiM( nt->mu_ptms[i] ,nt->mu_etams[i] ,
                               nt->mu_phims[i], detector::muon_mass( detector::GeV ) );
    mu->_exms_muon.SetPtEtaPhiM( nt->mu_ptexms[i] ,nt->mu_etaexms[i] ,
                                 nt->mu_phiexms[i], detector::muon_mass( detector::GeV ) );
    _make_associated_track_for_muon( mu , nt , i ); 
    _muons.push_back( mu );
  }

  // fill electrons (just enough info for met reconstruction)
  if( true ) {
    for( int i=0, f=nt->nele; i!=f; ++i ) { 
      boost::shared_ptr<AnaElectron> ele( new AnaElectron );
      ele->_momentum.SetPtEtaPhiM( nt->ele_et[i] , nt->ele_eta[i] , nt->ele_phi[i] , detector::electron_mass(detector::GeV) );
      // MET reconstruction
      ele->_met_wet = nt->ele_MET_wet[i];
      ele->_met_wpx = nt->ele_MET_wpx[i];
      ele->_met_wpy = nt->ele_MET_wpy[i];
      ele->_met_status = nt->ele_MET_status[i];
      ele->_ntuple_index = i;
      _electrons.push_back( ele );
    }
  }

  // fill jets
  assert( nt );
  _jets.reserve( static_cast<int>( nt->njet ) );
  for( int i=0, f=nt->njet; i!=f; ++i ) {
    boost::shared_ptr<AnaJet> jet( new AnaJet );
    switch( nt->jet_type[i] ) {
    case 0: jet->_alg = AnaJet::Algorithm::ATLAS_ANTI_KT_CONE_4_H1_TOPO; break;
    case 1: jet->_alg = AnaJet::Algorithm::ATLAS_ANTI_KT_CONE_6_H1_TOPO; break;
    case 2: jet->_alg = AnaJet::Algorithm::ATLAS_ANTI_KT_CONE_4_H1_LC_TOPO; break;
    case 3: jet->_alg = AnaJet::Algorithm::ATLAS_ANTI_KT_CONE_6_H1_LC_TOPO; break;
    case 4: jet->_alg = AnaJet::Algorithm::ATLAS_CONE_4_TRUTH; break;
    case 5: jet->_alg = AnaJet::Algorithm::ATLAS_CONE_4_TRUTH_PILEUP; break;
    case 6: jet->_alg = AnaJet::Algorithm::ATLAS_CONE_6_TRUTH; break;
    case 7: jet->_alg = AnaJet::Algorithm::ATLAS_CONE_6_TRUTH_PILEUP; break;
    case 8: jet->_alg = AnaJet::Algorithm::ATLAS_ANTI_KT_CONE_10_H1_LC_TOPO; break;
    default: assert(0&&"Unrecognized jet_type"); break;
    }
    if (jet->_alg == AnaJet::Algorithm::ATLAS_ANTI_KT_CONE_6_H1_TOPO) continue;
    if (jet->_alg == AnaJet::Algorithm::ATLAS_ANTI_KT_CONE_6_H1_LC_TOPO) continue;
    if (jet->_alg == AnaJet::Algorithm::ATLAS_CONE_6_TRUTH) continue;
    if (jet->_alg == AnaJet::Algorithm::ATLAS_CONE_4_TRUTH_PILEUP) continue;
    if (jet->_alg == AnaJet::Algorithm::ATLAS_CONE_6_TRUTH_PILEUP) continue;
    if (jet->_alg == AnaJet::Algorithm::ATLAS_ANTI_KT_CONE_10_H1_LC_TOPO) continue;
    jet->_jet_index = i;
    jet->_vtx_fraction = nt->jet_jvf[i];
    jet->_n90 = nt->jet_n90[i];
    jet->_timing = nt->jet_time[i];
    jet->_quality = nt->jet_larquality[i];
    jet->_HECQuality = nt->jet_hecquality[i];
    jet->_LArQuality = nt->jet_larquality[i];
    jet->_negativeE = nt->jet_negativee[i];
    jet->_sumPtTrack = nt->jet_sumpttrk[i];
    jet->_ntrkjetalg = nt->jet_ntrk[i];
    jet->_AverageLArQF = nt->jet_averagelarqf[i];
    jet->_emf = nt->jet_emf[i];
    jet->_fmax = nt->jet_fmax[i];
    jet->_BCH_CORR_CELL = nt->jet_bchcorr[i];
    jet->_BCH_CORR_JET = nt->jet_bchcorrjet[i];
    jet->_hecf = nt->jet_hecf[i];
    jet->_tgap3f = nt->jet_tgap3f[i];
    jet->_full_scale_momentum.SetPtEtaPhiM(nt->jet_pt[i],nt->jet_eta[i],nt->jet_phi[i],nt->jet_m[i]);
    // em scale for met cleaning cuts
    jet->_em_scale_momentum.SetPtEtaPhiM(nt->jet_pt_em[i],nt->jet_eta_em[i],nt->jet_phi_em[i],nt->jet_m_em[i]); 
    // const scale for LC jet calibration (used in MET computation)
    jet->_has_cs_scale = true;
    jet->_cs_scale_momentum.SetPtEtaPhiM(nt->jet_pt_cs[i],nt->jet_eta_cs[i],nt->jet_phi_cs[i],nt->jet_m_cs[i]); 
    // origin values
    jet->_EtaOriginEM = nt->jet_eta_origin_em[i];
    jet->_PhiOriginEM = nt->jet_phi_origin_em[i];
    jet->_MOriginEM = nt->jet_m_origin_em[i];
    jet->_EtaOrigin = nt->jet_eta_origin[i];
    jet->_PhiOrigin = nt->jet_phi_origin[i];
    jet->_MOrigin = nt->jet_m_origin[i];
    // MET reconstruction
    jet->_met_wet = nt->jet_MET_wet[i];
    jet->_met_wpx = nt->jet_MET_wpx[i];
    jet->_met_wpy = nt->jet_MET_wpy[i];
    jet->_met_status = nt->jet_MET_status[i];
    jet->_gcwjes = nt->jet_gcwjes[i];
    jet->_em_scale_correction = nt->jet_emjes[i];
    // finish off
    jet->_momentum = jet->_full_scale_momentum;
    _jets.push_back( jet );
  }


  // fill met
  assert( nt );
  assert( _met.empty() );
  boost::shared_ptr<AnaMET> met( new AnaMET );
  met->_met_type = AnaMET::REF_FINAL;
  met->_refgamma = nt->met_refgamma;
  met->_refgamma_phi = nt->met_refgamma_phi;
  met->_refgamma_sumet = nt->met_refgamma_sumet;
  met->_reftau = nt->met_reftau;
  met->_reftau_phi = nt->met_reftau_phi;
  met->_reftau_sumet = nt->met_reftau_sumet;
  met->_refmuon = nt->met_refmuon;
  met->_refmuon_phi = nt->met_refmuon_phi;
  met->_refmuon_sumet = nt->met_refmuon_sumet;

  met->_refmuon_track = nt->met_refmuon_track;
  met->_refmuon_track_phi = nt->met_refmuon_track_phi;
  met->_refmuon_track_sumet = nt->met_refmuon_track_sumet;
  TLorentzVector lochadtopo , muonboy , refmuon_track;
  lochadtopo.SetPtEtaPhiM( nt->met_lochadtopo , 0. , nt->met_lochadtopo_phi , 0. );
  muonboy.SetPtEtaPhiM( nt->met_muonboy , 0. , nt->met_muonboy_phi , 0. );
  refmuon_track.SetPtEtaPhiM( nt->met_refmuon_track , 0. , nt->met_refmuon_track_phi , 0. );
  TLorentzVector lochadtopo2 = lochadtopo + muonboy - refmuon_track;
  met->_lochad_topo_ichep = lochadtopo2.Pt();
  met->_lochad_topo_ichep_phi = lochadtopo2.Phi();

  met->_refele = nt->met_refele;
  met->_refele_phi = nt->met_refele_phi;
  met->_refele_sumet = nt->met_refele_sumet;
  met->_refjet = nt->met_refjet;
  met->_refjet_phi = nt->met_refjet_phi;
  met->_refjet_sumet = nt->met_refjet_sumet;
  met->_cellout_eflow = nt->met_cellout_eflow;
  met->_cellout_eflow_phi = nt->met_cellout_eflow_phi;
  met->_cellout_eflow_sumet = nt->met_cellout_eflow_sumet;
  met->_cellout = nt->met_cellout;
  met->_cellout_phi = nt->met_cellout_phi;
  met->_cellout_sumet = nt->met_cellout_sumet;  
  met->_softjets = nt->met_softjets;
  met->_softjets_phi = nt->met_softjets_phi;
  met->_softjets_sumet = nt->met_softjets_sumet;
  met->_cryo = nt->met_cryo;
  met->_cryo_phi = nt->met_cryo_phi;
  met->_cryo_sumet = nt->met_cryo_sumet;
  met->_muonboy = nt->met_muonboy;
  met->_muonboy_phi = nt->met_muonboy_phi;
  met->_muonboy_sumet = nt->met_muonboy_sumet;
  met->_lochad_topo = nt->met_lochadtopo;
  met->_lochad_topo_phi = nt->met_lochadtopo_phi;
  met->_lochad_topo_sumet = nt->met_lochadtopo_sumet;
  met->_truth_nonint = nt->met_truth_nonint;
  met->_truth_nonint_phi = nt->met_truth_nonint_phi;
  met->_truth_nonint_sumet = nt->met_truth_nonint_sumet;
  met->_momentum.SetPtEtaPhiM( nt->met_reffinal , 0. , nt->met_reffinal_phi , 0. );
  _met.push_back( met );
  if( true ) { 
    boost::shared_ptr<AnaMET> met( new AnaMET );
    met->_met_type = AnaMET::REF_FINAL_45;
    met->_momentum.SetPtEtaPhiM( nt->met_reffinal_eta45 , 0. , nt->met_reffinal_eta45_phi , 0. );
    _met.push_back( met );
  }

  if( true ) {
    boost::shared_ptr<AnaMET> met( new AnaMET );
    met->_met_type = AnaMET::LOCHADTOPO;
    met->_momentum.SetPtEtaPhiM( nt->met_lochadtopo , 0. , nt->met_lochadtopo_phi , 0. );
    _met.push_back( met );
  }
  if( true ) { 
    boost::shared_ptr<AnaMET> met( new AnaMET );
    met->_met_type = AnaMET::TRUTH;
    met->_momentum.SetPtEtaPhiM( nt->met_truth , 0. , nt->met_truth_phi , 0. );
    _met.push_back( met );
  }
  if( true ) { 
    boost::shared_ptr<AnaMET> met( new AnaMET );
    met->_met_type = AnaMET::TRUTH_NONINT;
    met->_momentum.SetPtEtaPhiM( nt->met_truth_nonint , 0. , nt->met_truth_nonint_phi , 0. );
    _met.push_back( met );
  }
  if( true ) { 
    boost::shared_ptr<AnaMET> met( new AnaMET );
    met->_met_type = AnaMET::TRUTH_INT;
    // met_truth is defined for Peter ntuples as MET_INT_ex + MET_MUONS_ex == MET_TRUTH_ex
    // so in order to get MET_INT component, we need to do a little math:
    double pt_x = nt->met_truth * cos(nt->met_truth_phi) - nt->met_truth_muons* cos(nt->met_truth_muons_phi);
    double pt_y = nt->met_truth * sin(nt->met_truth_phi) - nt->met_truth_muons* sin(nt->met_truth_muons_phi);
    met->_momentum.SetPxPyPzE( pt_x , pt_y , 0 , 0 );
    _met.push_back( met );
  }
  if( true ) { 
    boost::shared_ptr<AnaMET> met( new AnaMET );
    met->_met_type = AnaMET::TRUTH_MUONS;
    met->_momentum.SetPtEtaPhiM( nt->met_truth_muons , 0. , nt->met_truth_muons_phi , 0. );
    _met.push_back( met );
  }
  // fill trigger decision
  assert( nt );
  _trigger.reset( new AnaTrigger );
  _trigger->_lbn = nt->lb;
  _trigger->_run_number = nt->run;
  _trigger->_error_lar = nt->error_lar;
  // the following vector contain the entire event trigger record
  const std::vector<std::string>& EF = *nt->trig_ef;
  _trigger->_trig_ef_mu18 = _check_trigger(EF.begin(),EF.end(),"EF_mu18");
  _trigger->_trig_ef_mu18_mg = _check_trigger(EF.begin(),EF.end(),"EF_mu18_MG");
  _trigger->_trig_ef_mu40_msonly_barrel = _check_trigger(EF.begin(),EF.end(),"EF_mu40_MSonly_barrel");
  _trigger->_trig_ef_mu18_medium = _check_trigger(EF.begin(),EF.end(),"EF_mu18_medium");
  _trigger->_trig_ef_mu18_mg_medium = _check_trigger(EF.begin(),EF.end(),"EF_mu18_MG_medium");
  _trigger->_trig_ef_mu40_msonly_barrel_medium = _check_trigger(EF.begin(),EF.end(),"EF_mu40_MSonly_barrel_medium");

  // fill with Peter's trigger matching info
  for( int i=0; i!=nt->mu_trig_efmatchvector_; ++i ) { 
    const int i_ntuple_mu = nt->mu_trig_efmatchvector_first[i];
    const int i_trigger = nt->mu_trig_efmatchvector_second[i];
    const AnaMuon::ftype dr = (*(nt->mu_trig_efmatchdelr))[i];
    std::vector< boost::shared_ptr<const AnaMuon> >::iterator imu = 
      std::find_if( _muons.begin() , _muons.end() , boost::bind(&AnaMuon::ntuple_index,_1)==i_ntuple_mu );
    assert( i_trigger < EF.size() );
    if( imu != _muons.end() ) { (*imu)->_trigger_match_dr[ EF[i_trigger] ] = dr; }
  }

  // fill truth particles
  assert( nt );
  assert(nt->nmc < D3PDPeterTreeV29l::nmc_max);
  _truth_particles.reserve( static_cast<int>( nt->nmc ) );
  for( int imc=0, fmc=nt->nmc; imc!=fmc; ++imc ) {
    boost::shared_ptr<AnaTruthParticle> v( new AnaTruthParticle );
    v->_pdg_id = (nt->mc_pdgid)[imc];
    const int jmc = (nt->mc_parent)[imc];
    v->_parent_index = jmc;
    v->_parent_pdg_id = jmc==-1 ? -1 : (nt->mc_pdgid)[jmc];
    v->_charge = detector::charge_from_pdg((nt->mc_pdgid)[imc]);
    v->_status = (nt->mc_status)[imc];
    v->_vx_z = (nt->mc_vx_z)[imc];
    v->_parent = (nt->mc_parent)[imc];
    v->_momentum.SetPtEtaPhiE( (nt->mc_pt)[imc] , (nt->mc_eta)[imc] , 
			       (nt->mc_phi)[imc] , (nt->mc_e)[imc] );
    _truth_particles.push_back( v );
  }
  
  return true;
}

const bool
NtD3PDphysPeter::_get_event_V30()
{
  const D3PDPeterTreeV30* nt( dynamic_cast<const D3PDPeterTreeV30*>(_madeclass.get()) );
  assert( nt );
  assert(nt->nele < D3PDPeterTreeV30::nele_max);
  assert(nt->njet < D3PDPeterTreeV30::njet_max);
  assert(nt->nmu < D3PDPeterTreeV30::nmu_max);
  assert(nt->nvx < D3PDPeterTreeV30::nvx_max);
  // matchvectors are really large, so this is important
  assert(nt->mu_trig_l1matchvector_ < D3PDPeterTreeV30::matchvector_max);
  assert(nt->mu_trig_l2matchvector_ < D3PDPeterTreeV30::matchvector_max);
  assert(nt->mu_trig_l2samatchvector_ < D3PDPeterTreeV30::matchvector_max);
  assert(nt->mu_trig_efmatchvector_ < D3PDPeterTreeV30::matchvector_max);
  assert(nt->ele_trig_l1matchvector_ < D3PDPeterTreeV30::matchvector_max);
  assert(nt->ele_trig_l2matchvector_ < D3PDPeterTreeV30::matchvector_max);
  assert(nt->ele_trig_efmatchvector_ < D3PDPeterTreeV30::matchvector_max);

  _run_number = nt->run;
  _event_number = nt->event;
  _lumi_block = nt->lb;
  _average_mu = nt->lumi_mu_average; // encoded in a dedicated variable as of mc11a
  _mc_weight = nt->mc_weight;
  _mc_channel = nt->mc_channel;
  _mc_hfor = nt->mc_hfor;
  _bdff = nt->bdff;

  // fill vertex information
  assert( nt );
  _vertices.reserve( static_cast<int>( nt->nvx ) );
  for( int iv=0, fv=nt->nvx; iv!=fv; ++iv ) {
    boost::shared_ptr<AnaVertex> v( new AnaVertex );
    v->_x = nt->vx_x[iv];
    v->_y = nt->vx_y[iv];
    v->_z = nt->vx_z[iv];
    v->_ntracks = nt->vx_ntracks[iv];
    v->_sum_pt = nt->vx_sumpt[iv];
    v->_type = detector::remap_vertex_type_enum( nt->vx_type[iv] );
    _vertices.push_back( v );
  }

  // fill muons
  assert( nt );
  _muons.reserve( static_cast<int>( nt->nmu ) );
  for( int i=0, f=nt->nmu; i!=f; ++i ) {
    // for each muon in the ntuple, create a separate AnaMuon for 
    // combined, ID, MS, and extrapolated MS muons
    boost::shared_ptr<AnaMuon> mu( new AnaMuon );
    mu->_all_author_bitmask = AnaMuon::MuonTypeBitset( nt->mu_author[i] );
    // override some definitions per Peter ntuple coding to ensure that we have *combined* muons
    mu->_all_author_bitmask.set(MuonAllAuthor::STACO,mu->_all_author_bitmask.test(MuonAllAuthor::STACO) && nt->mu_class[i]==1 && nt->mu_ptid[i]!=-1);
    mu->_all_author_bitmask.set(MuonAllAuthor::MuidCo,mu->_all_author_bitmask.test(MuonAllAuthor::MuidCo) && nt->mu_class[i]==2 && nt->mu_ptid[i]!=-1);
    // other muon variables
    mu->_primary_author = nt->mu_primauthor[i];
    mu->_aod_quality = nt->mu_quality[i];
    switch( nt->mu_class[i] ) { // other possibility is to use mu_type here
    case 1: mu->_aod_class = AnaMuon::AOD_STACO; break;
    case 2: mu->_aod_class = AnaMuon::AOD_MUID; break;
    case 3: mu->_aod_class = AnaMuon::AOD_CALO; break;
    default: mu->_aod_class = AnaMuon::AOD_UNKNOWN; break;
    }
    mu->_ntuple_index = i;
    mu->_z0 = nt->mu_z0[i];
    mu->_d0 = nt->mu_d0[i];
    mu->_d0_exPV_err = std::sqrt(nt->mu_covdxpv[i]);
    mu->_z0_exPV = nt->mu_zxpv[i];
    mu->_d0_exPV = nt->mu_dxpv[i];
    mu->_z0_vx = nt->mu_zvx[i];
    mu->_d0_vx = nt->mu_dvx[i];
    mu->_d0_vx_err = std::sqrt(nt->mu_covdvx[i]);
    mu->_charge = nt->mu_q[i];
    mu->_matchchi2 = nt->mu_matchchi2[i];
    mu->_id_charge = nt->mu_qid[i];
    mu->_ms_charge = nt->mu_qms[i];
    mu->_exms_charge = nt->mu_qexms[i];
    mu->_momentum.SetPtEtaPhiM( nt->mu_pt[i] ,nt->mu_eta[i] ,
                                nt->mu_phi[i], detector::muon_mass( detector::GeV ) );
    // isolation variables
    mu->_pt_cone = boost::assign::list_of(nt->mu_ptcone10[i])(nt->mu_ptcone20[i])(nt->mu_ptcone30[i])(nt->mu_ptcone40[i]);
    mu->_et_cone = boost::assign::list_of(nt->mu_etcone10[i])(nt->mu_etcone20[i])(nt->mu_etcone30[i])(nt->mu_etcone40[i]);
    // nhits, nholes et cetera
    mu->_expect_b_layer = nt->mu_expectbl[i];
    mu->_nhits = boost::assign::list_of(nt->mu_nhitpix[i])(nt->mu_nhitsct[i])(nt->mu_nhittrt[i])(nt->mu_noutlierstrt[i])(nt->mu_nhitbl[i]);
    mu->_nholes = boost::assign::list_of(nt->mu_nholespix[i])(nt->mu_nholessct[i])(nt->mu_nholestrt[i]);
    mu->_ndead = boost::assign::list_of(nt->mu_ndeadsensorspix[i])(nt->mu_ndeadsensorssct[i])(-1);
    // MET reconstruction
    mu->_met_wet = nt->mu_MET_wet[i];
    mu->_met_wpx = nt->mu_MET_wpx[i];
    mu->_met_wpy = nt->mu_MET_wpy[i];
    mu->_met_status = nt->mu_MET_status[i];
    mu->_met_wet2 = nt->mu_MET_wet2[i];
    mu->_met_wpx2 = nt->mu_MET_wpx2[i];
    mu->_met_wpy2 = nt->mu_MET_wpy2[i];
    mu->_met_status2 = nt->mu_MET_status2[i];
    // extrapolated muon spectrometer momentum:
    mu->_id_muon.SetPtEtaPhiM( nt->mu_ptid[i] ,nt->mu_etaid[i] ,
                               nt->mu_phiid[i], detector::muon_mass( detector::GeV ) );
    mu->_ms_muon.SetPtEtaPhiM( nt->mu_ptms[i] ,nt->mu_etams[i] ,
                               nt->mu_phims[i], detector::muon_mass( detector::GeV ) );
    mu->_exms_muon.SetPtEtaPhiM( nt->mu_ptexms[i] ,nt->mu_etaexms[i] ,
                                 nt->mu_phiexms[i], detector::muon_mass( detector::GeV ) );
    _make_associated_track_for_muon( mu , nt , i ); 
    _muons.push_back( mu );
  }

  // fill inner-detector tracks as ID-only muons [ for tag-and-probe ]
  // as an alternative, one can use CaloTag muons as probes
  if (false) {
    assert(nt->ntrk < D3PDPeterTreeV30::ntrk_max);
    for( int i=0, f=nt->ntrk; i!=f; ++i ) {
      boost::shared_ptr<AnaMuon> mu( new AnaMuon );
      // other muon variables
      mu->_primary_author = MuonPrimAuthor::IDtrack;
      mu->_aod_class = AnaMuon::AOD_TRACK;
      mu->_ntuple_index = -1;
      mu->_d0_exPV_err = std::sqrt(nt->trk_covdxpv[i]);
      mu->_z0_exPV = nt->trk_zxpv[i];
      mu->_d0_exPV = nt->trk_dxpv[i];
      mu->_charge = nt->trk_q[i];
      const float sf = 1.0;
      mu->_momentum.SetPtEtaPhiM( sf*nt->trk_pt[i] ,nt->trk_eta[i] ,
				  nt->trk_phi[i], detector::muon_mass( detector::GeV ) );
      // isolation variables
      mu->_pt_cone = boost::assign::list_of(nt->trk_ptcone10[i])(nt->trk_ptcone20[i])(nt->trk_ptcone30[i])(nt->trk_ptcone40[i]);
      // nhits, nholes et cetera
      mu->_expect_b_layer = nt->trk_expectbl[i];
      mu->_nhits = boost::assign::list_of(nt->trk_nhitpix[i])(nt->trk_nhitsct[i])(nt->trk_nhittrt[i])(nt->trk_noutlierstrt[i])(nt->trk_nhitbl[i]);
      mu->_nholes = boost::assign::list_of(nt->trk_nholespix[i])(nt->trk_nholessct[i])(nt->trk_nholestrt[i]);
      mu->_ndead = boost::assign::list_of(nt->trk_ndeadsensorspix[i])(nt->trk_ndeadsensorssct[i])(-1);
      _muons.push_back( mu );
    }
  }

  // fill electrons
  if( true ) {
    for( int i=0, f=nt->nele; i!=f; ++i ) { 
      boost::shared_ptr<AnaElectron> ele( new AnaElectron );
      ele->_author = nt->ele_author[i];
      ele->_charge = nt->ele_q[i];
      ele->_momentum.SetPtEtaPhiM( nt->ele_et[i] , nt->ele_eta[i] , nt->ele_phi[i] , detector::electron_mass(detector::GeV) );
      ele->_is_em = nt->ele_isEM[i];
      ele->_oq = nt->ele_isgoodoq[i] ? 0 : 1446; // FIXME: store full bitmask once it is in the ntuple
      ele->_aod_quality = nt->ele_quality[i];
      ele->_rhad1 = nt->ele_rhad1[i];
      ele->_rhad = nt->ele_rhad[i];
      ele->_reta = nt->ele_reta[i];
      ele->_weta2 = nt->ele_w2[i];
      ele->_wstot = nt->ele_wstot[i];
      ele->_deltaeta = nt->ele_deltaeta[i];
      ele->_deltaphi = nt->ele_deltaphi[i];
      ele->_eoverp = detector::safe_divide( nt->ele_etclus[i] , nt->ele_pttrk[i] );
      ele->_demaxs1 = nt->ele_demaxs1[i]/1000.;
      ele->_eta2 = nt->ele_eta2[i];
      ele->_etap = nt->ele_etap[i];
      ele->_et2 = nt->ele_et2[i]/1000.;
      ele->_d0_exPV_err = std::sqrt(nt->ele_covdxpv[i]);
      ele->_z0_exPV = nt->ele_zxpv[i];
      ele->_d0_exPV = nt->ele_dxpv[i];
      ele->_d0_vx = nt->ele_dvx[i];
      ele->_d0_vx_err = std::sqrt(nt->ele_covdvx[i]);
      ele->_z0_vx = nt->ele_zvx[i];
      // MET reconstruction
      ele->_met_wet = nt->ele_MET_wet[i];
      ele->_met_wpx = nt->ele_MET_wpx[i];
      ele->_met_wpy = nt->ele_MET_wpy[i];
      ele->_met_status = nt->ele_MET_status[i];
      ele->_ntuple_index = i;
      ele->_pt_cone = boost::assign::list_of( -999. )( nt->ele_ptcone20[i] )( nt->ele_ptcone30[i] )( nt->ele_ptcone40[i] );
      ele->_uncorrected_et_cone = boost::assign::list_of( -999. )( nt->ele_etcone20[i] )( nt->ele_etcone30[i] )( nt->ele_etcone40[i] );
      ele->_et_cone = boost::assign::list_of( -999. )( nt->ele_etcone20_cor[i] )( nt->ele_etcone30_cor[i] )( nt->ele_etcone40_cor[i] );
      ele->_cluster_momentum.SetPtEtaPhiM( nt->ele_etclus[i] , nt->ele_etaclus[i] , nt->ele_phiclus[i] , detector::electron_mass(detector::GeV) );
      _make_associated_track_for_electron( ele , nt , i );
      ele->_nhits = boost::assign::list_of(nt->ele_nhitpix[i])(nt->ele_nhitsct[i])(nt->ele_nhitbl[i])(nt->ele_nhittrt[i]);
      ele->_noutliers = boost::assign::list_of(nt->ele_noutlierspix[i])(nt->ele_noutlierssct[i])(nt->ele_noutliersbl[i])(0);
      ele->_expect_b_layer = nt->ele_expectbl[i];
      _electrons.push_back( ele );
    }
  }

  // fill jets
  assert( nt );
  _jets.reserve( static_cast<int>( nt->njet ) );
  for( int i=0, f=nt->njet; i!=f; ++i ) {
    boost::shared_ptr<AnaJet> jet( new AnaJet );
    switch( nt->jet_type[i] ) {
    case 0: jet->_alg = AnaJet::Algorithm::ATLAS_ANTI_KT_CONE_4_H1_TOPO; break;
    case 1: jet->_alg = AnaJet::Algorithm::ATLAS_ANTI_KT_CONE_6_H1_TOPO; break;
    case 2: jet->_alg = AnaJet::Algorithm::ATLAS_ANTI_KT_CONE_4_H1_LC_TOPO; break;
    case 3: jet->_alg = AnaJet::Algorithm::ATLAS_ANTI_KT_CONE_6_H1_LC_TOPO; break;
    case 4: jet->_alg = AnaJet::Algorithm::ATLAS_CONE_4_TRUTH; break;
    case 5: jet->_alg = AnaJet::Algorithm::ATLAS_CONE_4_TRUTH_PILEUP; break;
    case 6: jet->_alg = AnaJet::Algorithm::ATLAS_CONE_6_TRUTH; break;
    case 7: jet->_alg = AnaJet::Algorithm::ATLAS_CONE_6_TRUTH_PILEUP; break;
    default: assert(0&&"Unrecognized jet_type"); break;
    }
    if (jet->_alg == AnaJet::Algorithm::ATLAS_CONE_4_TRUTH_PILEUP) continue;
    if (jet->_alg == AnaJet::Algorithm::ATLAS_CONE_6_TRUTH_PILEUP) continue;
    jet->_jet_index = i;
    jet->_vtx_fraction = nt->jet_jvf[i];
    jet->_n90 = nt->jet_n90[i];
    jet->_timing = nt->jet_time[i];
    jet->_quality = nt->jet_larquality[i];
    jet->_HECQuality = nt->jet_hecquality[i];
    jet->_LArQuality = nt->jet_larquality[i];
    jet->_negativeE = nt->jet_negativee[i];
    jet->_sumPtTrack = nt->jet_sumpttrk[i];
    jet->_ntrkjetalg = nt->jet_ntrk[i];
    jet->_AverageLArQF = nt->jet_averagelarqf[i];
    jet->_emf = nt->jet_emf[i];
    jet->_fmax = nt->jet_fmax[i];
    jet->_BCH_CORR_CELL = nt->jet_bchcorr[i];
    jet->_BCH_CORR_JET = nt->jet_bchcorrjet[i];
    jet->_hecf = nt->jet_hecf[i];
    jet->_tgap3f = nt->jet_tgap3f[i];
    jet->_full_scale_momentum.SetPtEtaPhiM(nt->jet_pt[i],nt->jet_eta[i],nt->jet_phi[i],nt->jet_m[i]);
    // em scale for met cleaning cuts
    jet->_em_scale_momentum.SetPtEtaPhiM(nt->jet_pt_em[i],nt->jet_eta_em[i],nt->jet_phi_em[i],nt->jet_m_em[i]); 
    // origin values
    jet->_EtaOriginEM = nt->jet_eta_origin_em[i];
    jet->_PhiOriginEM = nt->jet_phi_origin_em[i];
    jet->_MOriginEM = nt->jet_m_origin_em[i];
    jet->_EtaOrigin = nt->jet_eta_origin[i];
    jet->_PhiOrigin = nt->jet_phi_origin[i];
    jet->_MOrigin = nt->jet_m_origin[i];
    // MET reconstruction
    jet->_met_wet = nt->jet_MET_wet[i];
    jet->_met_wpx = nt->jet_MET_wpx[i];
    jet->_met_wpy = nt->jet_MET_wpy[i];
    jet->_met_status = nt->jet_MET_status[i];
    jet->_gcwjes = nt->jet_gcwjes[i];
    // other stuff
    jet->_tag_ip2d_w = nt->jet_tagIP2D[i];
    jet->_tag_ip3d_w = nt->jet_tagIP3D[i];
    jet->_tag_sv0_w = nt->jet_tagSV0[i];
    jet->_tag_sv1_w = nt->jet_tagSV1[i];
    jet->_tag_sv2_w = nt->jet_tagSV2[i];
    jet->_tag_jetp_w = nt->jet_tagJetProb[i];
    jet->_tag_cmbnn_w = nt->jet_tagJetFitterCOMBNN[i];
    jet->_tag_cmb_w = nt->jet_tagSV1[i]+nt->jet_tagIP3D[i];
    jet->_em_scale_correction = nt->jet_emjes[i];
    // finish off
    jet->_momentum = jet->_full_scale_momentum;
    _jets.push_back( jet );
  }


  // fill met
  assert( nt );
  assert( _met.empty() );
  boost::shared_ptr<AnaMET> met( new AnaMET );
  met->_met_type = AnaMET::REF_FINAL;
  met->_refgamma = nt->met_refgamma;
  met->_refgamma_phi = nt->met_refgamma_phi;
  met->_refgamma_sumet = nt->met_refgamma_sumet;
  met->_reftau = nt->met_reftau;
  met->_reftau_phi = nt->met_reftau_phi;
  met->_reftau_sumet = nt->met_reftau_sumet;
  met->_refmuon = nt->met_refmuon;
  met->_refmuon_phi = nt->met_refmuon_phi;
  met->_refmuon_sumet = nt->met_refmuon_sumet;
  met->_refele = nt->met_refele;
  met->_refele_phi = nt->met_refele_phi;
  met->_refele_sumet = nt->met_refele_sumet;
  met->_refjet = nt->met_refjet;
  met->_refjet_phi = nt->met_refjet_phi;
  met->_refjet_sumet = nt->met_refjet_sumet;
  met->_cellout_eflow = nt->met_cellout_eflow;
  met->_cellout_eflow_phi = nt->met_cellout_eflow_phi;
  met->_cellout_eflow_sumet = nt->met_cellout_eflow_sumet;
  met->_cellout = nt->met_cellout;
  met->_cellout_phi = nt->met_cellout_phi;
  met->_cellout_sumet = nt->met_cellout_sumet;  
  met->_softjets = nt->met_softjets;
  met->_softjets_phi = nt->met_softjets_phi;
  met->_softjets_sumet = nt->met_softjets_sumet;
  met->_cryo = nt->met_cryo;
  met->_cryo_phi = nt->met_cryo_phi;
  met->_cryo_sumet = nt->met_cryo_sumet;
  met->_muonboy = nt->met_muonboy;
  met->_muonboy_phi = nt->met_muonboy_phi;
  met->_muonboy_sumet = nt->met_muonboy_sumet;
  met->_lochad_topo = nt->met_lochadtopo;
  met->_lochad_topo_phi = nt->met_lochadtopo_phi;
  met->_lochad_topo_sumet = nt->met_lochadtopo_sumet;
  met->_momentum.SetPtEtaPhiM( nt->met_reffinal , 0. , nt->met_reffinal_phi , 0. );
  met->_sumet = nt->met_reffinal_sumet;
  _met.push_back( met );

  if( true ) {
    boost::shared_ptr<AnaMET> met( new AnaMET );
    met->_met_type = AnaMET::LOCHADTOPO;
    met->_momentum.SetPtEtaPhiM( nt->met_lochadtopo , 0. , nt->met_lochadtopo_phi , 0. );
    met->_sumet = nt->met_lochadtopo_sumet;
    _met.push_back( met );
  }

  // fill trigger decision
  assert( nt );
  _trigger.reset( new AnaTrigger );
  _trigger->_lbn = nt->lb;
  _trigger->_run_number = nt->run;
  _trigger->_error_lar = nt->error_lar;
  // the following vector contain the entire event trigger record
  const std::vector<std::string>& L1 = *nt->trig_l1;
  const std::vector<std::string>& L2 = *nt->trig_l2;
  const std::vector<std::string>& EF = *nt->trig_ef;
  _trigger->_trig_l1_mu0 = _check_trigger(L1.begin(),L1.end(),"L1_MU0");
  _trigger->_trig_l1_mu6 = _check_trigger(L1.begin(),L1.end(),"L1_MU6");
  _trigger->_trig_l1_mu10 = _check_trigger(L1.begin(),L1.end(),"L1_MU10");
  _trigger->_trig_ef_mu10 = _check_trigger(EF.begin(),EF.end(),"EF_mu10");
  _trigger->_trig_ef_mu10_mg = _check_trigger(EF.begin(),EF.end(),"EF_mu10_MG");
  _trigger->_trig_ef_mu13_mg = _check_trigger(EF.begin(),EF.end(),"EF_mu13_MG");
  _trigger->_trig_ef_mu13_mg_tight = _check_trigger(EF.begin(),EF.end(),"EF_mu13_MG_tight");
  _trigger->_trig_ef_mu15_mu10_effs = _check_trigger(EF.begin(),EF.end(),"EF_mu15_mu10_EFFS");
  _trigger->_trig_ef_mu15_mu10_effs_medium = _check_trigger(EF.begin(),EF.end(),"EF_mu15_mu10_EFFS_medium");
  _trigger->_trig_ef_mu18 = _check_trigger(EF.begin(),EF.end(),"EF_mu18");
  _trigger->_trig_ef_mu18_mg = _check_trigger(EF.begin(),EF.end(),"EF_mu18_MG");
  _trigger->_trig_ef_mu20_mg = _check_trigger(EF.begin(),EF.end(),"EF_mu20_MG");
  _trigger->_trig_ef_mu22_mg = _check_trigger(EF.begin(),EF.end(),"EF_mu22_MG");
  _trigger->_trig_ef_mu40_msonly_barrel = _check_trigger(EF.begin(),EF.end(),"EF_mu40_MSonly_barrel");
  _trigger->_trig_ef_mu18_medium = _check_trigger(EF.begin(),EF.end(),"EF_mu18_medium");
  _trigger->_trig_ef_mu18_mg_medium = _check_trigger(EF.begin(),EF.end(),"EF_mu18_MG_medium");
  _trigger->_trig_ef_mu40_msonly_barrel_medium = _check_trigger(EF.begin(),EF.end(),"EF_mu40_MSonly_barrel_medium");
  _trigger->_trig_l1_em14 = _check_trigger(L1.begin(),L1.end(),"L1_EM14");
  _trigger->_trig_ef_e15_medium = _check_trigger(EF.begin(),EF.end(),"EF_e15_medium");
  _trigger->_trig_ef_e20_medium = _check_trigger(EF.begin(),EF.end(),"EF_e20_medium");
  _trigger->_trig_ef_e22_medium = _check_trigger(EF.begin(),EF.end(),"EF_e22_medium");
  _trigger->_trig_ef_e22vh_medium1 = _check_trigger(EF.begin(),EF.end(),"EF_e22vh_medium1");
  _trigger->_trig_ef_2e12_medium = _check_trigger(EF.begin(),EF.end(),"EF_2e12_medium");
  _trigger->_trig_ef_2e12t_medium = _check_trigger(EF.begin(),EF.end(),"EF_2e12T_medium");
  _trigger->_trig_ef_2e12tvh_medium = _check_trigger(EF.begin(),EF.end(),"EF_2e12Tvh_medium");
  _trigger->_trig_ef_2e10_medium = _check_trigger(EF.begin(),EF.end(),"EF_2e10_medium");
  _trigger->_trig_ef_2mu10 = _check_trigger(EF.begin(),EF.end(),"EF_2mu10");
  _trigger->_trig_ef_2mu10_loose = _check_trigger(EF.begin(),EF.end(),"EF_2mu10_loose");
  _trigger->_trig_ef_mu6 = _check_trigger(EF.begin(),EF.end(),"EF_mu6");
  _trigger->_trig_ef_g20_loose = _check_trigger(EF.begin(),EF.end(),"EF_g20_loose");
  _trigger->_trig_ef_j10_a4tc_effs = _check_trigger(EF.begin(),EF.end(),"EF_j10_a4tc_EFFS");
  _trigger->_trig_ef_fj10_a4tc_effs = _check_trigger(EF.begin(),EF.end(),"EF_fj10_a4tc_EFFS");
  _trigger->_trig_ef_j15_a4tc_effs = _check_trigger(EF.begin(),EF.end(),"EF_j15_a4tc_EFFS");
  _trigger->_trig_ef_fj15_a4tc_effs = _check_trigger(EF.begin(),EF.end(),"EF_fj15_a4tc_EFFS");
  _trigger->_trig_ef_j20_a4_effs = _check_trigger(EF.begin(),EF.end(),"EF_j20_a4_EFFS");
  _trigger->_trig_ef_j20_a4tc_effs = _check_trigger(EF.begin(),EF.end(),"EF_j20_a4tc_EFFS");
  _trigger->_trig_ef_fj20_a4tc_effs = _check_trigger(EF.begin(),EF.end(),"EF_fj20_a4tc_EFFS");
  _trigger->_trig_ef_xe80 = _check_trigger(EF.begin(),EF.end(),"EF_xe80");
  _trigger->_trig_ef_xe80_tclcw = _check_trigger(EF.begin(),EF.end(),"EF_xe80_tclcw");
  _trigger->_trig_ef_xe70 = _check_trigger(EF.begin(),EF.end(),"EF_xe70");
  _trigger->_trig_ef_xe70_tclcw = _check_trigger(EF.begin(),EF.end(),"EF_xe70_tclcw");
  _trigger->_trig_ef_xe60 = _check_trigger(EF.begin(),EF.end(),"EF_xe60");
  _trigger->_trig_ef_xe60t = _check_trigger(EF.begin(),EF.end(),"EF_xe60T");
  _trigger->_trig_ef_xe60_tclcw_loose_delayed = _check_trigger(EF.begin(),EF.end(),"EF_xe60_tclcw_loose_delayed");
  _trigger->_trig_ef_xe60t_tclcw_loose_delayed = _check_trigger(EF.begin(),EF.end(),"EF_xe60T_tclcw_loose_delayed");
  _trigger->_trig_ef_j110_a4tchad_xe75_tclcw = _check_trigger(EF.begin(),EF.end(),"EF_j110_a4tchad_xe75_tclcw");
  _trigger->_trig_ef_j80_a4tchad_xe100_tclcw_veryloose = _check_trigger(EF.begin(),EF.end(),"EF_j80_a4tchad_xe100_tclcw_veryloose");
  _trigger->_trig_ef_mu24i_tight = _check_trigger(EF.begin(),EF.end(),"EF_mu24i_tight");
  _trigger->_trig_ef_mu36_tight = _check_trigger(EF.begin(),EF.end(),"EF_mu36_tight");
  


  // fill truth particles
  assert( nt );
  assert(nt->nmc < D3PDPeterTreeV30::nmc_max);
  _truth_particles.reserve( static_cast<int>( nt->nmc ) );
  for( int imc=0, fmc=nt->nmc; imc!=fmc; ++imc ) {
    boost::shared_ptr<AnaTruthParticle> v( new AnaTruthParticle );
    v->_pdg_id = (nt->mc_pdgid)[imc];
    const int jmc = (nt->mc_parent)[imc];
    v->_parent_index = jmc;
    v->_parent_pdg_id = jmc==-1 ? -1 : (nt->mc_pdgid)[jmc];
    v->_charge = detector::charge_from_pdg((nt->mc_pdgid)[imc]);
    v->_status = (nt->mc_status)[imc];
    v->_parent = (nt->mc_parent)[imc];
    v->_momentum.SetPtEtaPhiE( (nt->mc_pt)[imc] , (nt->mc_eta)[imc] , 
			       (nt->mc_phi)[imc] , (nt->mc_e)[imc] );
    _truth_particles.push_back( v );
  }
  
  // fill truth table
  if( false ) {
    _truth_table.reset( new AnaTruthTable );
    for( AnaTruthTable::index_type imc=0, fmc=nt->nmc; imc!=fmc; ++imc ) {
      static TLorentzVector mom;
      const int dummy(0);
      mom.SetPtEtaPhiE( (nt->mc_pt)[imc] , (nt->mc_eta)[imc] , 
                        (nt->mc_phi)[imc] , (nt->mc_e)[imc] );
      _truth_table->add_particle( imc , 
				  (nt->mc_pdgid)[imc] , 
				  boost::math::iround(detector::charge_from_pdg((nt->mc_pdgid)[imc])) ,
				  (nt->mc_status)[imc] ,
				  mom ,
				  (nt->mc_parent)[imc]==-1 ? &dummy : &((nt->mc_parent)[imc]),
				  (nt->mc_parent)[imc]==-1 ? &dummy : &((nt->mc_parent)[imc+1]),
				  &dummy, &dummy );
    }
    _truth_table->display_decay_tree();
    //_truth_table->write_graphviz( "./graph.dot" );
  }

  return true;
}


const bool
NtD3PDphysPeter::_get_event_V50()
{
  const D3PDPeterTreeV50* nt( dynamic_cast<const D3PDPeterTreeV50*>(_madeclass.get()) );
  assert( nt );
  assert(nt->nele < D3PDPeterTreeV50::nele_max);
  assert(nt->njet < D3PDPeterTreeV50::njet_max);
  assert(nt->nmu < D3PDPeterTreeV50::nmu_max);
  assert(nt->nvx < D3PDPeterTreeV50::nvx_max);
  // matchvectors are really large, so this is important
  assert(nt->mu_trig_l1matchvector_ < D3PDPeterTreeV50::matchvector_max);
  assert(nt->mu_trig_l2matchvector_ < D3PDPeterTreeV50::matchvector_max);
  assert(nt->mu_trig_l2samatchvector_ < D3PDPeterTreeV50::matchvector_max);
  assert(nt->mu_trig_efmatchvector_ < D3PDPeterTreeV50::matchvector_max);
  assert(nt->ele_trig_l1matchvector_ < D3PDPeterTreeV50::matchvector_max);
  assert(nt->ele_trig_l2matchvector_ < D3PDPeterTreeV50::matchvector_max);
  assert(nt->ele_trig_efmatchvector_ < D3PDPeterTreeV50::matchvector_max);

  _run_number = nt->run;
  _event_number = nt->event;
  _lumi_block = nt->lb;
  _average_mu = nt->lumi_mu_average; // encoded in a dedicated variable as of mc11a
  _mc_weight = nt->mc_weight;
  _mc_channel = nt->mc_channel;
  _mc_hfor = nt->mc_hfor;
  _bdff = nt->bdff;

  // fill vertex information
  assert( nt );
  _vertices.reserve( static_cast<int>( nt->nvx ) );
  for( int iv=0, fv=nt->nvx; iv!=fv; ++iv ) {
    boost::shared_ptr<AnaVertex> v( new AnaVertex );
    v->_x = nt->vx_x[iv];
    v->_y = nt->vx_y[iv];
    v->_z = nt->vx_z[iv];
    v->_ntracks = nt->vx_ntracks[iv];
    v->_sum_pt = nt->vx_sumpt[iv];
    v->_type = detector::remap_vertex_type_enum( nt->vx_type[iv] );
    _vertices.push_back( v );
  }

  // fill muons
  assert( nt );
  _muons.reserve( static_cast<int>( nt->nmu ) );
  for( int i=0, f=nt->nmu; i!=f; ++i ) {
    // for each muon in the ntuple, create a separate AnaMuon for 
    // combined, ID, MS, and extrapolated MS muons
    boost::shared_ptr<AnaMuon> mu( new AnaMuon );
    mu->_all_author_bitmask = AnaMuon::MuonTypeBitset( nt->mu_author[i] );
    // override some definitions per Peter ntuple coding to ensure that we have *combined* muons
    mu->_all_author_bitmask.set(MuonAllAuthor::STACO,mu->_all_author_bitmask.test(MuonAllAuthor::STACO) && nt->mu_class[i]==1 && nt->mu_ptid[i]!=-1);
    mu->_all_author_bitmask.set(MuonAllAuthor::MuidCo,mu->_all_author_bitmask.test(MuonAllAuthor::MuidCo) && nt->mu_class[i]==2 && nt->mu_ptid[i]!=-1);
    // other muon variables
    mu->_primary_author = nt->mu_primauthor[i];
    mu->_aod_quality = nt->mu_quality[i];
    switch( nt->mu_class[i] ) { // other possibility is to use mu_type here
    case 1: mu->_aod_class = AnaMuon::AOD_STACO; break;
    case 2: mu->_aod_class = AnaMuon::AOD_MUID; break;
    case 3: mu->_aod_class = AnaMuon::AOD_CALO; break;
    default: mu->_aod_class = AnaMuon::AOD_UNKNOWN; break;
    }
    mu->_ntuple_index = i;
    mu->_z0 = nt->mu_z0[i];
    mu->_d0 = nt->mu_d0[i];
    mu->_d0_exPV_err = std::sqrt(nt->mu_covdxpv[i]);
    mu->_z0_exPV = nt->mu_zxpv[i];
    mu->_d0_exPV = nt->mu_dxpv[i];
    mu->_z0_vx = nt->mu_zvx[i];
    mu->_d0_vx = nt->mu_dvx[i];
    mu->_d0_vx_err = std::sqrt(nt->mu_covdvx[i]);
    mu->_charge = nt->mu_q[i];
    mu->_matchchi2 = nt->mu_matchchi2[i];
    mu->_id_charge = nt->mu_qid[i];
    mu->_ms_charge = nt->mu_qms[i];
    mu->_exms_charge = nt->mu_qexms[i];
    mu->_momentum.SetPtEtaPhiM( nt->mu_pt[i] ,nt->mu_eta[i] ,
                                nt->mu_phi[i], detector::muon_mass( detector::GeV ) );
    // isolation variables
    mu->_pt_cone = boost::assign::list_of(nt->mu_ptcone10[i])(nt->mu_ptcone20[i])(nt->mu_ptcone30[i])(nt->mu_ptcone40[i]);
    mu->_et_cone = boost::assign::list_of(nt->mu_etcone10[i])(nt->mu_etcone20[i])(nt->mu_etcone30[i])(nt->mu_etcone40[i]);
    // nhits, nholes et cetera
    mu->_expect_b_layer = nt->mu_expectbl[i];
    mu->_nhits = boost::assign::list_of(nt->mu_nhitpix[i])(nt->mu_nhitsct[i])(nt->mu_nhittrt[i])(nt->mu_noutlierstrt[i])(nt->mu_nhitbl[i]);
    mu->_nholes = boost::assign::list_of(nt->mu_nholespix[i])(nt->mu_nholessct[i])(nt->mu_nholestrt[i]);
    mu->_ndead = boost::assign::list_of(nt->mu_ndeadsensorspix[i])(nt->mu_ndeadsensorssct[i])(-1);
    // MET reconstruction
    mu->_met_wet = nt->mu_MET_wet[i];
    mu->_met_wpx = nt->mu_MET_wpx[i];
    mu->_met_wpy = nt->mu_MET_wpy[i];
    mu->_met_status = nt->mu_MET_status[i];
    mu->_met_wet2 = nt->mu_MET_wet2[i];
    mu->_met_wpx2 = nt->mu_MET_wpx2[i];
    mu->_met_wpy2 = nt->mu_MET_wpy2[i];
    mu->_met_status2 = nt->mu_MET_status2[i];
    // extrapolated muon spectrometer momentum:
    mu->_id_muon.SetPtEtaPhiM( nt->mu_ptid[i] ,nt->mu_etaid[i] ,
                               nt->mu_phiid[i], detector::muon_mass( detector::GeV ) );
    mu->_ms_muon.SetPtEtaPhiM( nt->mu_ptms[i] ,nt->mu_etams[i] ,
                               nt->mu_phims[i], detector::muon_mass( detector::GeV ) );
    mu->_exms_muon.SetPtEtaPhiM( nt->mu_ptexms[i] ,nt->mu_etaexms[i] ,
                                 nt->mu_phiexms[i], detector::muon_mass( detector::GeV ) );
    _make_associated_track_for_muon( mu , nt , i ); 
    _muons.push_back( mu );
  }

  // fill inner-detector tracks as ID-only muons [ for tag-and-probe ]
  // as an alternative, one can use CaloTag muons as probes
  if (false) {
    assert(nt->ntrk < D3PDPeterTreeV50::ntrk_max);
    for( int i=0, f=nt->ntrk; i!=f; ++i ) {
      boost::shared_ptr<AnaMuon> mu( new AnaMuon );
      // other muon variables
      mu->_primary_author = MuonPrimAuthor::IDtrack;
      mu->_aod_class = AnaMuon::AOD_TRACK;
      mu->_ntuple_index = -1;
      mu->_d0_exPV_err = std::sqrt(nt->trk_covdxpv[i]);
      mu->_z0_exPV = nt->trk_zxpv[i];
      mu->_d0_exPV = nt->trk_dxpv[i];
      mu->_charge = nt->trk_q[i];
      const float sf = 1.0;
      mu->_momentum.SetPtEtaPhiM( sf*nt->trk_pt[i] ,nt->trk_eta[i] ,
				  nt->trk_phi[i], detector::muon_mass( detector::GeV ) );
      // isolation variables
      mu->_pt_cone = boost::assign::list_of(nt->trk_ptcone10[i])(nt->trk_ptcone20[i])(nt->trk_ptcone30[i])(nt->trk_ptcone40[i]);
      // nhits, nholes et cetera
      mu->_expect_b_layer = nt->trk_expectbl[i];
      mu->_nhits = boost::assign::list_of(nt->trk_nhitpix[i])(nt->trk_nhitsct[i])(nt->trk_nhittrt[i])(nt->trk_noutlierstrt[i])(nt->trk_nhitbl[i]);
      mu->_nholes = boost::assign::list_of(nt->trk_nholespix[i])(nt->trk_nholessct[i])(nt->trk_nholestrt[i]);
      mu->_ndead = boost::assign::list_of(nt->trk_ndeadsensorspix[i])(nt->trk_ndeadsensorssct[i])(-1);
      _muons.push_back( mu );
    }
  }

  // fill electrons
  if( true ) {
    for( int i=0, f=nt->nele; i!=f; ++i ) { 
      boost::shared_ptr<AnaElectron> ele( new AnaElectron );
      ele->_author = nt->ele_author[i];
      ele->_charge = nt->ele_q[i];
      ele->_momentum.SetPtEtaPhiM( nt->ele_et[i] , nt->ele_eta[i] , nt->ele_phi[i] , detector::electron_mass(detector::GeV) );
      ele->_is_em = nt->ele_isEM[i];
      ele->_oq = nt->ele_isgoodoq[i] ? 0 : 1446; // FIXME: store full bitmask once it is in the ntuple
      ele->_aod_quality = nt->ele_quality[i];
      ele->_rhad1 = nt->ele_rhad1[i];
      ele->_rhad = nt->ele_rhad[i];
      ele->_reta = nt->ele_reta[i];
      ele->_weta2 = nt->ele_w2[i];
      ele->_wstot = nt->ele_wstot[i];
      ele->_deltaeta = nt->ele_deltaeta[i];
      ele->_deltaphi = nt->ele_deltaphi[i];
      ele->_eoverp = detector::safe_divide( nt->ele_etclus[i] , nt->ele_pttrk[i] );
      ele->_demaxs1 = nt->ele_demaxs1[i]/1000.;
      ele->_eta2 = nt->ele_eta2[i];
      ele->_etap = nt->ele_etap[i];
      ele->_et2 = nt->ele_et2[i]/1000.;
      ele->_d0_exPV_err = std::sqrt(nt->ele_covdxpv[i]);
      ele->_z0_exPV = nt->ele_zxpv[i];
      ele->_d0_exPV = nt->ele_dxpv[i];
      ele->_d0_vx = nt->ele_dvx[i];
      ele->_d0_vx_err = std::sqrt(nt->ele_covdvx[i]);
      ele->_z0_vx = nt->ele_zvx[i];
      // MET reconstruction
      ele->_met_wet = nt->ele_MET_wet[i];
      ele->_met_wpx = nt->ele_MET_wpx[i];
      ele->_met_wpy = nt->ele_MET_wpy[i];
      ele->_met_status = nt->ele_MET_status[i];
      ele->_ntuple_index = i;
      ele->_pt_cone = boost::assign::list_of( -999. )( nt->ele_ptcone20[i] )( nt->ele_ptcone30[i] )( nt->ele_ptcone40[i] );
      ele->_uncorrected_et_cone = boost::assign::list_of( -999. )( nt->ele_etcone20[i] )( nt->ele_etcone30[i] )( nt->ele_etcone40[i] );
      ele->_et_cone = boost::assign::list_of( -999. )( nt->ele_etcone20_cor[i] )( nt->ele_etcone30_cor[i] )( nt->ele_etcone40_cor[i] );
      ele->_cluster_momentum.SetPtEtaPhiM( nt->ele_etclus[i] , nt->ele_etaclus[i] , nt->ele_phiclus[i] , detector::electron_mass(detector::GeV) );
      _make_associated_track_for_electron( ele , nt , i );
      ele->_nhits = boost::assign::list_of(nt->ele_nhitpix[i])(nt->ele_nhitsct[i])(nt->ele_nhitbl[i])(nt->ele_nhittrt[i]);
      ele->_noutliers = boost::assign::list_of(nt->ele_noutlierspix[i])(nt->ele_noutlierssct[i])(nt->ele_noutliersbl[i])(0);
      ele->_expect_b_layer = nt->ele_expectbl[i];
      _electrons.push_back( ele );
    }
  }

  // fill jets
  assert( nt );
  _jets.reserve( static_cast<int>( nt->njet ) );
  for( int i=0, f=nt->njet; i!=f; ++i ) {
    boost::shared_ptr<AnaJet> jet( new AnaJet );
    switch( nt->jet_type[i] ) {
    case 0: jet->_alg = AnaJet::Algorithm::ATLAS_ANTI_KT_CONE_4_H1_TOPO; break;
    case 1: jet->_alg = AnaJet::Algorithm::ATLAS_ANTI_KT_CONE_6_H1_TOPO; break;
    case 2: jet->_alg = AnaJet::Algorithm::ATLAS_ANTI_KT_CONE_4_H1_LC_TOPO; break;
    case 3: jet->_alg = AnaJet::Algorithm::ATLAS_ANTI_KT_CONE_6_H1_LC_TOPO; break;
    case 4: jet->_alg = AnaJet::Algorithm::ATLAS_CONE_4_TRUTH; break;
    case 5: jet->_alg = AnaJet::Algorithm::ATLAS_CONE_4_TRUTH_PILEUP; break;
    case 6: jet->_alg = AnaJet::Algorithm::ATLAS_CONE_6_TRUTH; break;
    case 7: jet->_alg = AnaJet::Algorithm::ATLAS_CONE_6_TRUTH_PILEUP; break;
    default: assert(0&&"Unrecognized jet_type"); break;
    }
    if (jet->_alg == AnaJet::Algorithm::ATLAS_CONE_4_TRUTH_PILEUP) continue;
    if (jet->_alg == AnaJet::Algorithm::ATLAS_CONE_6_TRUTH_PILEUP) continue;
    jet->_jet_index = i;
    jet->_vtx_fraction = nt->jet_jvf[i];
    jet->_n90 = nt->jet_n90[i];
    jet->_timing = nt->jet_time[i];
    jet->_quality = nt->jet_larquality[i];
    jet->_HECQuality = nt->jet_hecquality[i];
    jet->_LArQuality = nt->jet_larquality[i];
    jet->_negativeE = nt->jet_negativee[i];
    jet->_sumPtTrack = nt->jet_sumpttrk[i];
    jet->_sumPtTrack_allpv = nt->jet_sumpttrk_allpv[i];
    jet->_ntrkjetalg = nt->jet_ntrk[i];
    jet->_ntrkjetalg_allpv = nt->jet_ntrk_allpv[i];
    jet->_AverageLArQF = nt->jet_averagelarqf[i];
    jet->_emf = nt->jet_emf[i];
    jet->_fmax = nt->jet_fmax[i];
    jet->_BCH_CORR_CELL = nt->jet_bchcorr[i];
    jet->_BCH_CORR_JET = nt->jet_bchcorrjet[i];
    jet->_hecf = nt->jet_hecf[i];
    jet->_tgap3f = nt->jet_tgap3f[i];
    jet->_full_scale_momentum.SetPtEtaPhiM(nt->jet_pt[i],nt->jet_eta[i],nt->jet_phi[i],nt->jet_m[i]);
    // em scale for met cleaning cuts
    jet->_em_scale_momentum.SetPtEtaPhiM(nt->jet_pt_em[i],nt->jet_eta_em[i],nt->jet_phi_em[i],nt->jet_m_em[i]); 
    // origin values
    jet->_EtaOriginEM = nt->jet_eta_origin_em[i];
    jet->_PhiOriginEM = nt->jet_phi_origin_em[i];
    jet->_MOriginEM = nt->jet_m_origin_em[i];
    jet->_EtaOrigin = nt->jet_eta_origin[i];
    jet->_PhiOrigin = nt->jet_phi_origin[i];
    jet->_MOrigin = nt->jet_m_origin[i];
    // MET reconstruction
    jet->_met_wet = nt->jet_MET_wet[i];
    jet->_met_wpx = nt->jet_MET_wpx[i];
    jet->_met_wpy = nt->jet_MET_wpy[i];
    jet->_met_status = nt->jet_MET_status[i];
    jet->_gcwjes = nt->jet_gcwjes[i];
    // other stuff
    jet->_tag_ip2d_w = nt->jet_tagIP2D[i];
    jet->_tag_ip3d_w = nt->jet_tagIP3D[i];
    jet->_tag_sv0_w = nt->jet_tagSV0[i];
    jet->_tag_sv1_w = nt->jet_tagSV1[i];
    jet->_tag_sv2_w = nt->jet_tagSV2[i];
    jet->_tag_jetp_w = nt->jet_tagJetProb[i];
    jet->_tag_cmbnn_w = nt->jet_tagJetFitterCOMBNN[i];
    jet->_tag_cmb_w = nt->jet_tagSV1[i]+nt->jet_tagIP3D[i];
    jet->_em_scale_correction = nt->jet_emjes[i];
    jet->_calowidth = nt->jet_width[i];
    jet->_trackwidth = nt->jet_trackwidth[i];
    jet->_trackwidth_allpv = nt->jet_trackwidth_allpv[i];
    // finish off
    jet->_momentum = jet->_full_scale_momentum;
    _jets.push_back( jet );
  }


  // fill met
  assert( nt );
  assert( _met.empty() );
  if( true ) {
    boost::shared_ptr<AnaMET> met( new AnaMET );
    met->_met_type = AnaMET::REF_FINAL;
    /*
      met->_refgamma = nt->met_refgamma;
      met->_refgamma_phi = nt->met_refgamma_phi;
      met->_refgamma_sumet = nt->met_refgamma_sumet;
      met->_reftau = nt->met_reftau;
      met->_reftau_phi = nt->met_reftau_phi;
      met->_reftau_sumet = nt->met_reftau_sumet;
      met->_refmuon = nt->met_refmuon;
      met->_refmuon_phi = nt->met_refmuon_phi;
      met->_refmuon_sumet = nt->met_refmuon_sumet;
      met->_refele = nt->met_refele;
      met->_refele_phi = nt->met_refele_phi;
      met->_refele_sumet = nt->met_refele_sumet;
      met->_refjet = nt->met_refjet;
      met->_refjet_phi = nt->met_refjet_phi;
      met->_refjet_sumet = nt->met_refjet_sumet;
      met->_cellout_eflow = nt->met_cellout_eflow;
      met->_cellout_eflow_phi = nt->met_cellout_eflow_phi;
      met->_cellout_eflow_sumet = nt->met_cellout_eflow_sumet;
      met->_cellout = nt->met_cellout;
      met->_cellout_phi = nt->met_cellout_phi;
      met->_cellout_sumet = nt->met_cellout_sumet;  
      met->_softjets = nt->met_softjets;
      met->_softjets_phi = nt->met_softjets_phi;
      met->_softjets_sumet = nt->met_softjets_sumet;
      met->_cryo = nt->met_cryo;
      met->_cryo_phi = nt->met_cryo_phi;
      met->_cryo_sumet = nt->met_cryo_sumet;
      met->_muonboy = nt->met_muonboy;
      met->_muonboy_phi = nt->met_muonboy_phi;
      met->_muonboy_sumet = nt->met_muonboy_sumet;
      met->_lochad_topo = nt->met_lochadtopo;
      met->_lochad_topo_phi = nt->met_lochadtopo_phi;
      met->_lochad_topo_sumet = nt->met_lochadtopo_sumet;
    */
    met->_momentum.SetPtEtaPhiM( nt->met_reffinal , 0. , nt->met_reffinal_phi , 0. );
    met->_sumet = nt->met_reffinal_sumet;
    _met.push_back( met );
  }

  if( true ) {
    boost::shared_ptr<AnaMET> met( new AnaMET );
    met->_met_type = AnaMET::FINAL;
    TLorentzVector reffinal , muonboy , refmuon;
    reffinal.SetPtEtaPhiM( nt->met_reffinal , 0. , nt->met_reffinal_phi , 0. );
    muonboy.SetPtEtaPhiM( nt->met_muonboy , 0. , nt->met_muonboy_phi , 0. );
    refmuon.SetPtEtaPhiM( nt->met_refmuon , 0. , nt->met_refmuon_phi , 0. );
    met->_momentum = reffinal - muonboy - refmuon;
    _met.push_back( met );
  }

  if( true ) {
    boost::shared_ptr<AnaMET> met( new AnaMET );
    met->_met_type = AnaMET::LOCHADTOPO;
    met->_momentum.SetPtEtaPhiM( nt->met_lochadtopo , 0. , nt->met_lochadtopo_phi , 0. );
    met->_sumet = nt->met_lochadtopo_sumet;
    _met.push_back( met );
  }

  // fill trigger decision
  assert( nt );
  _trigger.reset( new AnaTrigger );
  _trigger->_lbn = nt->lb;
  _trigger->_run_number = nt->run;
  _trigger->_error_lar = nt->error_lar;
  // the following vector contain the entire event trigger record
  const std::vector<std::string>& L1 = *nt->trig_l1;
  const std::vector<std::string>& L2 = *nt->trig_l2;
  const std::vector<std::string>& EF = *nt->trig_ef;
  _trigger->_trig_l1_mu0 = _check_trigger(L1.begin(),L1.end(),"L1_MU0");
  _trigger->_trig_l1_mu6 = _check_trigger(L1.begin(),L1.end(),"L1_MU6");
  _trigger->_trig_l1_mu10 = _check_trigger(L1.begin(),L1.end(),"L1_MU10");
  _trigger->_trig_ef_mu10 = _check_trigger(EF.begin(),EF.end(),"EF_mu10");
  _trigger->_trig_ef_mu10_mg = _check_trigger(EF.begin(),EF.end(),"EF_mu10_MG");
  _trigger->_trig_ef_mu13_mg = _check_trigger(EF.begin(),EF.end(),"EF_mu13_MG");
  _trigger->_trig_ef_mu13_mg_tight = _check_trigger(EF.begin(),EF.end(),"EF_mu13_MG_tight");
  _trigger->_trig_ef_mu15_mu10_effs = _check_trigger(EF.begin(),EF.end(),"EF_mu15_mu10_EFFS");
  _trigger->_trig_ef_mu15_mu10_effs_medium = _check_trigger(EF.begin(),EF.end(),"EF_mu15_mu10_EFFS_medium");
  _trigger->_trig_ef_mu18 = _check_trigger(EF.begin(),EF.end(),"EF_mu18");
  _trigger->_trig_ef_mu18_mg = _check_trigger(EF.begin(),EF.end(),"EF_mu18_MG");
  _trigger->_trig_ef_mu20_mg = _check_trigger(EF.begin(),EF.end(),"EF_mu20_MG");
  _trigger->_trig_ef_mu22_mg = _check_trigger(EF.begin(),EF.end(),"EF_mu22_MG");
  _trigger->_trig_ef_mu40_msonly_barrel = _check_trigger(EF.begin(),EF.end(),"EF_mu40_MSonly_barrel");
  _trigger->_trig_ef_mu18_medium = _check_trigger(EF.begin(),EF.end(),"EF_mu18_medium");
  _trigger->_trig_ef_mu18_mg_medium = _check_trigger(EF.begin(),EF.end(),"EF_mu18_MG_medium");
  _trigger->_trig_ef_mu40_msonly_barrel_medium = _check_trigger(EF.begin(),EF.end(),"EF_mu40_MSonly_barrel_medium");
  _trigger->_trig_l1_em14 = _check_trigger(L1.begin(),L1.end(),"L1_EM14");
  _trigger->_trig_ef_e15_medium = _check_trigger(EF.begin(),EF.end(),"EF_e15_medium");
  _trigger->_trig_ef_e20_medium = _check_trigger(EF.begin(),EF.end(),"EF_e20_medium");
  _trigger->_trig_ef_e22_medium = _check_trigger(EF.begin(),EF.end(),"EF_e22_medium");
  _trigger->_trig_ef_e22vh_medium1 = _check_trigger(EF.begin(),EF.end(),"EF_e22vh_medium1");
  _trigger->_trig_ef_2e12_medium = _check_trigger(EF.begin(),EF.end(),"EF_2e12_medium");
  _trigger->_trig_ef_2e12t_medium = _check_trigger(EF.begin(),EF.end(),"EF_2e12T_medium");
  _trigger->_trig_ef_2e12tvh_medium = _check_trigger(EF.begin(),EF.end(),"EF_2e12Tvh_medium");
  _trigger->_trig_ef_2e10_medium = _check_trigger(EF.begin(),EF.end(),"EF_2e10_medium");
  _trigger->_trig_ef_2mu10 = _check_trigger(EF.begin(),EF.end(),"EF_2mu10");
  _trigger->_trig_ef_2mu10_loose = _check_trigger(EF.begin(),EF.end(),"EF_2mu10_loose");
  _trigger->_trig_ef_mu6 = _check_trigger(EF.begin(),EF.end(),"EF_mu6");
  _trigger->_trig_ef_g20_loose = _check_trigger(EF.begin(),EF.end(),"EF_g20_loose");
  _trigger->_trig_ef_j10_a4tc_effs = _check_trigger(EF.begin(),EF.end(),"EF_j10_a4tc_EFFS");
  _trigger->_trig_ef_fj10_a4tc_effs = _check_trigger(EF.begin(),EF.end(),"EF_fj10_a4tc_EFFS");
  _trigger->_trig_ef_j15_a4tc_effs = _check_trigger(EF.begin(),EF.end(),"EF_j15_a4tc_EFFS");
  _trigger->_trig_ef_fj15_a4tc_effs = _check_trigger(EF.begin(),EF.end(),"EF_fj15_a4tc_EFFS");
  _trigger->_trig_ef_j20_a4_effs = _check_trigger(EF.begin(),EF.end(),"EF_j20_a4_EFFS");
  _trigger->_trig_ef_j20_a4tc_effs = _check_trigger(EF.begin(),EF.end(),"EF_j20_a4tc_EFFS");
  _trigger->_trig_ef_fj20_a4tc_effs = _check_trigger(EF.begin(),EF.end(),"EF_fj20_a4tc_EFFS");
  _trigger->_trig_ef_xe80 = _check_trigger(EF.begin(),EF.end(),"EF_xe80");
  _trigger->_trig_ef_xe80_tclcw = _check_trigger(EF.begin(),EF.end(),"EF_xe80_tclcw");
  _trigger->_trig_ef_xe70 = _check_trigger(EF.begin(),EF.end(),"EF_xe70");
  _trigger->_trig_ef_xe70_tclcw = _check_trigger(EF.begin(),EF.end(),"EF_xe70_tclcw");
  _trigger->_trig_ef_xe60 = _check_trigger(EF.begin(),EF.end(),"EF_xe60");
  _trigger->_trig_ef_xe60t = _check_trigger(EF.begin(),EF.end(),"EF_xe60T");
  _trigger->_trig_ef_xe60_tclcw_loose_delayed = _check_trigger(EF.begin(),EF.end(),"EF_xe60_tclcw_loose_delayed");
  _trigger->_trig_ef_xe60t_tclcw_loose_delayed = _check_trigger(EF.begin(),EF.end(),"EF_xe60T_tclcw_loose_delayed");
  _trigger->_trig_ef_j110_a4tchad_xe75_tclcw = _check_trigger(EF.begin(),EF.end(),"EF_j110_a4tchad_xe75_tclcw");
  _trigger->_trig_ef_j80_a4tchad_xe100_tclcw_veryloose = _check_trigger(EF.begin(),EF.end(),"EF_j80_a4tchad_xe100_tclcw_veryloose");
  _trigger->_trig_ef_mu24i_tight = _check_trigger(EF.begin(),EF.end(),"EF_mu24i_tight");
  _trigger->_trig_ef_mu36_tight = _check_trigger(EF.begin(),EF.end(),"EF_mu36_tight");
  


  // fill truth particles
  assert( nt );
  assert(nt->nmc < D3PDPeterTreeV50::nmc_max);
  _truth_particles.reserve( static_cast<int>( nt->nmc ) );
  for( int imc=0, fmc=nt->nmc; imc!=fmc; ++imc ) {
    boost::shared_ptr<AnaTruthParticle> v( new AnaTruthParticle );
    v->_pdg_id = (nt->mc_pdgid)[imc];
    const int jmc = (nt->mc_parent)[imc];
    v->_parent_index = jmc;
    v->_parent_pdg_id = jmc==-1 ? -1 : (nt->mc_pdgid)[jmc];
    v->_charge = detector::charge_from_pdg((nt->mc_pdgid)[imc]);
    v->_status = (nt->mc_status)[imc];
    v->_parent = (nt->mc_parent)[imc];
    v->_momentum.SetPtEtaPhiE( (nt->mc_pt)[imc] , (nt->mc_eta)[imc] , 
			       (nt->mc_phi)[imc] , (nt->mc_e)[imc] );
    _truth_particles.push_back( v );
  }
  
  // fill truth table
  if( false ) {
    _truth_table.reset( new AnaTruthTable );
    for( AnaTruthTable::index_type imc=0, fmc=nt->nmc; imc!=fmc; ++imc ) {
      static TLorentzVector mom;
      const int dummy(0);
      mom.SetPtEtaPhiE( (nt->mc_pt)[imc] , (nt->mc_eta)[imc] , 
                        (nt->mc_phi)[imc] , (nt->mc_e)[imc] );
      _truth_table->add_particle( imc , 
				  (nt->mc_pdgid)[imc] , 
				  boost::math::iround(detector::charge_from_pdg((nt->mc_pdgid)[imc])) ,
				  (nt->mc_status)[imc] ,
				  mom ,
				  (nt->mc_parent)[imc]==-1 ? &dummy : &((nt->mc_parent)[imc]),
				  (nt->mc_parent)[imc]==-1 ? &dummy : &((nt->mc_parent)[imc+1]),
				  &dummy, &dummy );
    }
    _truth_table->display_decay_tree();
    //_truth_table->write_graphviz( "./graph.dot" );
  }

  return true;
}
