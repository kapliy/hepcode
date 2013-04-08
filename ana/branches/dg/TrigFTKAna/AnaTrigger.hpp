#ifndef WRAP_ANATRIGGER_HPP
#define WRAP_ANATRIGGER_HPP

// ANATRIGGER
// ================================================================
// Data for trigger decision.
// ================================================================
// 2009-07-15 Antonio Boveia (boveia@hep.uchicago.edu)

#include <numeric>
#include <cmath>
#include <string>
#include <iostream>
#include <string>
#include <boost/format.hpp>
#include <boost/optional.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/optional.hpp>
#include <boost/serialization/version.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/tokenizer.hpp>
#include <boost/foreach.hpp>
#include <TLorentzVector.h>
#include "TrigFTKAna/Detector.hpp"

class
AnaTrigger
{

public:

protected:
  /* egamma part */
  boost::optional<bool> _trig_ef_e22_medium;
  boost::optional<bool> _trig_ef_e20_loose;
  boost::optional<bool> _trig_ef_e20_medium;
  boost::optional<bool> _trig_ef_e10_medium;
  boost::optional<bool> _trig_ef_e10_loose;
  boost::optional<bool> _trig_ef_e5_loose;
  boost::optional<bool> _trig_ef_e15_medium;
  boost::optional<bool> _trig_ef_g20_loose;
  boost::optional<bool> _trig_ef_g40_loose;
  boost::optional<bool> _trig_l1_em3;
  boost::optional<bool> _trig_l1_em5;
  boost::optional<bool> _trig_l1_em10;
  boost::optional<bool> _trig_l1_em14;
  /* muon part */
  boost::optional<bool> _trig_ef_mu18_medium;
  boost::optional<bool> _trig_ef_mu18_mg_medium;
  boost::optional<bool> _trig_ef_mu40_msonly_barrel_medium;
  boost::optional<bool> _trig_ef_mu40_msonly_barrel;
  boost::optional<bool> _trig_ef_mu22_mg;
  boost::optional<bool> _trig_ef_mu20_mg;
  boost::optional<bool> _trig_ef_mu18_mg;
  boost::optional<bool> _trig_ef_mu18;
  boost::optional<bool> _trig_ef_mu13_mg_tight;
  boost::optional<bool> _trig_ef_mu13_mg;
  boost::optional<bool> _trig_ef_mu10_mg;
  boost::optional<bool> _trig_ef_mu10;
  boost::optional<bool> _trig_ef_mu6;
  boost::optional<bool> _trig_ef_mu4;
  boost::optional<bool> _trig_l1_mu10;
  boost::optional<bool> _trig_l1_mu6;
  boost::optional<bool> _trig_l1_mu0;
  // minbias triggers
  boost::optional<bool> _trig_l1_mbts_1_1_col;
  boost::optional<bool> _trig_l1_mbts_1_1_empty;
  boost::optional<bool> _trig_l1_mbts_1_1_unparied;
  boost::optional<bool> _trig_l1_mbts_1_1_unparied1;
  boost::optional<bool> _trig_l1_mbts_1_1_unparied2;
  boost::optional<bool> _trig_l1_mbts_1_1;
  boost::optional<bool> _trig_l1_mbts_1;
  // jets
  boost::optional<bool> _trig_ef_j10_a4tc_effs;
  boost::optional<bool> _trig_ef_fj10_a4tc_effs;
  boost::optional<bool> _trig_ef_j15_a4tc_effs;
  boost::optional<bool> _trig_ef_fj15_a4tc_effs;
  boost::optional<bool> _trig_ef_j20_a4_effs;
  boost::optional<bool> _trig_ef_j20_a4tc_effs;
  boost::optional<bool> _trig_ef_fj20_a4tc_effs;
  // auxiliary triggers
  boost::optional<bool> _trig_ef_e25_medium;
  boost::optional<bool> _trig_ef_e30_loose;
  boost::optional<bool> _trig_ef_2e10_medium;
  boost::optional<bool> _trig_ef_e10_loose_mu6;
  boost::optional<bool> _trig_ef_mu15_mg;
  boost::optional<bool> _trig_ef_mu20;
  boost::optional<bool> _trig_ef_2mu10;
  boost::optional<bool> _trig_ef_2mu10_loose;
  boost::optional<bool> _trig_ef_mu15_mu10_effs;
  boost::optional<bool> _trig_ef_mu15_mu10_effs_medium;
  boost::optional<bool> _trig_ef_2e12_medium;
  boost::optional<bool> _trig_ef_2e12t_medium;
  boost::optional<bool> _trig_ef_2e12tvh_medium;
  boost::optional<bool> _trig_ef_e22_medium1;
  boost::optional<bool> _trig_ef_e22vh_medium1;
  /* met triggers */
  boost::optional<bool> _trig_ef_xe60;
  boost::optional<bool> _trig_ef_xe60t;
  boost::optional<bool> _trig_ef_xe60_tclcw_loose_delayed;
  boost::optional<bool> _trig_ef_xe60t_tclcw_loose_delayed;
  boost::optional<bool> _trig_ef_xe70;
  boost::optional<bool> _trig_ef_xe70_tclcw;
  boost::optional<bool> _trig_ef_xe80;
  boost::optional<bool> _trig_ef_xe80_tclcw;
  boost::optional<bool> _trig_ef_j110_a4tchad_xe75_tclcw;
  boost::optional<bool> _trig_ef_j80_a4tchad_xe100_tclcw_veryloose;
  boost::optional<bool> _trig_ef_mu24i_tight;
  boost::optional<bool> _trig_ef_mu36_tight;

  // trigger matching
  int _trig_EF_el_n;
  std::vector<int> _trig_EF_el_EF_e20_medium;
  std::vector<float> _trig_EF_el_eta;
  std::vector<float> _trig_EF_el_phi;
  std::vector<int>     _trig_L2_combmuonfeature_L2_mu18_MG;
  int          _trig_L2_combmuonfeature_n;
  std::vector<float>   _trig_L2_combmuonfeature_pt;
  std::vector<float>   _trig_L2_combmuonfeature_eta;
  std::vector<float>   _trig_L2_combmuonfeature_phi;
  int          _trig_EF_trigmugirl_n;
  std::vector<int>     _trig_EF_trigmugirl_track_n;
  std::vector<std::vector<float> > _trig_EF_trigmugirl_track_CB_pt;
  std::vector<std::vector<float> > _trig_EF_trigmugirl_track_CB_eta;
  std::vector<std::vector<float> > _trig_EF_trigmugirl_track_CB_phi;
  std::vector<int>     _trig_L2_muonfeature_L2_mu40_MSonly_barrel;
  int          _trig_L2_muonfeature_n;
  std::vector<float>   _trig_L2_muonfeature_pt;
  std::vector<float>   _trig_L2_muonfeature_eta;
  std::vector<float>   _trig_L2_muonfeature_phi;
  int          _trig_EF_trigmuonef_n;
  std::vector<int>     _trig_EF_trigmuonef_track_n;
  std::vector<std::vector<float> > _trig_EF_trigmuonef_track_SA_pt;
  std::vector<std::vector<float> > _trig_EF_trigmuonef_track_SA_eta;
  std::vector<std::vector<float> > _trig_EF_trigmuonef_track_SA_phi;

  // event info
  int _run_number;
  int _lbn;
  unsigned int _error_lar;
  //
  mutable bool _strict_checking; // assert if the trigger does not exist


  friend class NtD3PDphysWZ;
  friend class NtD3PDphysEW;
  friend class NtD3PDphysTop;
  friend class NtD3PDphysPeter;
  friend class NtD3PDphysJetMET;
  friend class NtWrapper;

private:

  friend class boost::serialization::access;
  template<class archiveT>
  void serialize( archiveT& ar, const unsigned int version ) {
    ar & boost::serialization::make_nvp("trig_ef_e20_loose",_trig_ef_e20_loose);
    ar & boost::serialization::make_nvp("trig_ef_e10_medium",_trig_ef_e10_medium);
    ar & boost::serialization::make_nvp("trig_ef_e10_loose",_trig_ef_e10_loose);
    ar & boost::serialization::make_nvp("trig_ef_e5_loose",_trig_ef_e5_loose);
    ar & boost::serialization::make_nvp("trig_ef_e15_medium",_trig_ef_e15_medium);
    ar & boost::serialization::make_nvp("trig_ef_g20_loose",_trig_ef_g20_loose);
    ar & boost::serialization::make_nvp("trig_ef_g40_loose",_trig_ef_g40_loose);
    ar & boost::serialization::make_nvp("trig_l1_em3",_trig_l1_em3);
    ar & boost::serialization::make_nvp("trig_l1_em5",_trig_l1_em5);
    ar & boost::serialization::make_nvp("trig_l1_em10",_trig_l1_em10);
    ar & boost::serialization::make_nvp("trig_l1_em14",_trig_l1_em14);
    ar & boost::serialization::make_nvp("trig_ef_mu13_mg",_trig_ef_mu13_mg);
    ar & boost::serialization::make_nvp("trig_ef_mu13_mg_tight",_trig_ef_mu13_mg_tight);
    ar & boost::serialization::make_nvp("trig_ef_mu10_mg",_trig_ef_mu10_mg);
    ar & boost::serialization::make_nvp("trig_ef_mu10",_trig_ef_mu10);
    ar & boost::serialization::make_nvp("trig_ef_mu6",_trig_ef_mu6);
    ar & boost::serialization::make_nvp("trig_ef_mu4",_trig_ef_mu4);
    ar & boost::serialization::make_nvp("trig_l1_mu10",_trig_l1_mu10);
    ar & boost::serialization::make_nvp("trig_l1_mu6",_trig_l1_mu6);
    ar & boost::serialization::make_nvp("trig_l1_mu0",_trig_l1_mu0);
    ar & boost::serialization::make_nvp("run_number",_run_number);
    ar & boost::serialization::make_nvp("lbn",_lbn);
    ar & boost::serialization::make_nvp("error_lar",_error_lar);
    if( version < 3 ) { return; }
    ar & boost::serialization::make_nvp( "trig_l1_mbts_1_1_col",_trig_l1_mbts_1_1_col);
    ar & boost::serialization::make_nvp( "trig_l1_mbts_1_1_empty",_trig_l1_mbts_1_1_empty);
    ar & boost::serialization::make_nvp( "trig_l1_mbts_1_1_unparied",_trig_l1_mbts_1_1_unparied);
    ar & boost::serialization::make_nvp( "trig_l1_mbts_1_1_unparied1",_trig_l1_mbts_1_1_unparied1);
    ar & boost::serialization::make_nvp( "trig_l1_mbts_1_1_unparied2",_trig_l1_mbts_1_1_unparied2);
    ar & boost::serialization::make_nvp( "trig_l1_mbts_1_1",_trig_l1_mbts_1_1);
    ar & boost::serialization::make_nvp( "trig_l1_mbts_1",_trig_l1_mbts_1);
    if( version < 4 ) { return; }
    ar & boost::serialization::make_nvp("trig_ef_e20_medium",_trig_ef_e20_medium);
    ar & boost::serialization::make_nvp("trig_ef_e25_medium",_trig_ef_e25_medium);
    ar & boost::serialization::make_nvp("trig_ef_e30_loose",_trig_ef_e30_loose);
    ar & boost::serialization::make_nvp("trig_ef_2e10_medium",_trig_ef_2e10_medium);
    ar & boost::serialization::make_nvp("trig_ef_e10_loose_mu6",_trig_ef_e10_loose_mu6);
    ar & boost::serialization::make_nvp("trig_ef_mu15_mg",_trig_ef_mu15_mg);
    ar & boost::serialization::make_nvp("trig_ef_mu20",_trig_ef_mu20);
    ar & boost::serialization::make_nvp("trig_ef_2mu10",_trig_ef_2mu10);
    if( version < 5 ) { return; }
    ar & boost::serialization::make_nvp("trig_ef_mu18_mg",_trig_ef_mu18_mg);
    ar & boost::serialization::make_nvp("trig_ef_mu18",_trig_ef_mu18);
    if( version < 6 ) { return; }
    ar & boost::serialization::make_nvp("trig_ef_mu20_mg",_trig_ef_mu20_mg);
    if( version < 7 ) { return; }
    ar & boost::serialization::make_nvp("trig_ef_mu40_msonly_barrel",_trig_ef_mu40_msonly_barrel);
    ar & boost::serialization::make_nvp("trig_ef_mu20_mg",_trig_ef_mu22_mg);
    ar & boost::serialization::make_nvp("trig_ef_2mu10",_trig_ef_2mu10_loose);
    ar & boost::serialization::make_nvp("trig_ef_j10_a4tc_effs",_trig_ef_j10_a4tc_effs);
    //trigger matching
    ar & boost::serialization::make_nvp("trig_EF_el_n",_trig_EF_el_n);
    ar & boost::serialization::make_nvp("trig_EF_el_EF_e20_medium",_trig_EF_el_EF_e20_medium);
    ar & boost::serialization::make_nvp("trig_EF_el_eta",_trig_EF_el_eta);
    ar & boost::serialization::make_nvp("trig_EF_el_phi",_trig_EF_el_phi);
    ar & boost::serialization::make_nvp("trig_L2_combmuonfeature_L2_mu18_MG",_trig_L2_combmuonfeature_L2_mu18_MG);
    ar & boost::serialization::make_nvp("trig_L2_combmuonfeature_n",_trig_L2_combmuonfeature_n);
    ar & boost::serialization::make_nvp("trig_L2_combmuonfeature_pt",_trig_L2_combmuonfeature_pt);
    ar & boost::serialization::make_nvp("trig_L2_combmuonfeature_eta",_trig_L2_combmuonfeature_eta);
    ar & boost::serialization::make_nvp("trig_L2_combmuonfeature_phi",_trig_L2_combmuonfeature_phi);
    ar & boost::serialization::make_nvp("trig_EF_trigmugirl_n",_trig_EF_trigmugirl_n);
    ar & boost::serialization::make_nvp("trig_EF_trigmugirl_track_n",_trig_EF_trigmugirl_track_n);
    ar & boost::serialization::make_nvp("trig_EF_trigmugirl_track_CB_pt",_trig_EF_trigmugirl_track_CB_pt);
    ar & boost::serialization::make_nvp("trig_EF_trigmugirl_track_CB_eta",_trig_EF_trigmugirl_track_CB_eta);
    ar & boost::serialization::make_nvp("trig_EF_trigmugirl_track_CB_phi",_trig_EF_trigmugirl_track_CB_phi);
    ar & boost::serialization::make_nvp("trig_L2_muonfeature_L2_mu40_MSonly_barrel",_trig_L2_muonfeature_L2_mu40_MSonly_barrel);
    ar & boost::serialization::make_nvp("trig_L2_muonfeature_n",_trig_L2_muonfeature_n);
    ar & boost::serialization::make_nvp("trig_L2_muonfeature_pt",_trig_L2_muonfeature_pt);
    ar & boost::serialization::make_nvp("trig_L2_muonfeature_eta",_trig_L2_muonfeature_eta);
    ar & boost::serialization::make_nvp("trig_L2_muonfeature_phi",_trig_L2_muonfeature_phi);
    ar & boost::serialization::make_nvp("trig_EF_trigmuonef_n",_trig_EF_trigmuonef_n);
    ar & boost::serialization::make_nvp("trig_EF_trigmuonef_track_n",_trig_EF_trigmuonef_track_n);
    ar & boost::serialization::make_nvp("trig_EF_trigmuonef_track_SA_pt",_trig_EF_trigmuonef_track_SA_pt);
    ar & boost::serialization::make_nvp("trig_EF_trigmuonef_track_SA_eta",_trig_EF_trigmuonef_track_SA_eta);
    ar & boost::serialization::make_nvp("trig_EF_trigmuonef_track_SA_phi",_trig_EF_trigmuonef_track_SA_phi);
    if( version < 8 ) { return; }
    ar & boost::serialization::make_nvp("trig_ef_mu18_medium",_trig_ef_mu18_medium);
    ar & boost::serialization::make_nvp("trig_ef_mu18_mg_medium",_trig_ef_mu18_mg_medium);
    ar & boost::serialization::make_nvp("trig_ef_mu40_msonly_barrel_medium",_trig_ef_mu40_msonly_barrel_medium);
    ar & boost::serialization::make_nvp("trig_ef_mu15_mu10_effs",_trig_ef_mu15_mu10_effs);
    ar & boost::serialization::make_nvp("trig_ef_mu15_mu10_effs_medium",_trig_ef_mu15_mu10_effs_medium);
    ar & boost::serialization::make_nvp("trig_ef_e22_medium",_trig_ef_e22_medium);
    ar & boost::serialization::make_nvp("trig_ef_e22_medium1",_trig_ef_e22_medium1);
    ar & boost::serialization::make_nvp("trig_ef_e22vh_medium1",_trig_ef_e22vh_medium1);    
    ar & boost::serialization::make_nvp("trig_ef_2e12_medium",_trig_ef_2e12_medium);
    ar & boost::serialization::make_nvp("trig_ef_2e12t_medium",_trig_ef_2e12t_medium);
    ar & boost::serialization::make_nvp("trig_ef_2e12tvh_medium",_trig_ef_2e12tvh_medium);
    if( version < 9 ) { return; }
    ar & boost::serialization::make_nvp("trig_ef_fj10_a4tc_effs",_trig_ef_fj10_a4tc_effs);
    ar & boost::serialization::make_nvp("trig_ef_j15_a4tc_effs",_trig_ef_j15_a4tc_effs);
    ar & boost::serialization::make_nvp("trig_ef_fj15_a4tc_effs",_trig_ef_fj15_a4tc_effs);
    ar & boost::serialization::make_nvp("trig_ef_j20_a4_effs",_trig_ef_j20_a4_effs);
    ar & boost::serialization::make_nvp("trig_ef_j20_a4tc_effs",_trig_ef_j20_a4tc_effs);
    ar & boost::serialization::make_nvp("trig_ef_fj20_a4tc_effs",_trig_ef_fj20_a4tc_effs);
    if( version < 10 ) { return; }
    ar & boost::serialization::make_nvp("trig_ef_xe60",_trig_ef_xe60);
    ar & boost::serialization::make_nvp("trig_ef_xe60t",_trig_ef_xe60t);
    ar & boost::serialization::make_nvp("trig_ef_xe60_tclcw_loose_delayed",_trig_ef_xe60_tclcw_loose_delayed);
    ar & boost::serialization::make_nvp("trig_ef_xe60t_tclcw_loose_delayed",_trig_ef_xe60t_tclcw_loose_delayed);
    ar & boost::serialization::make_nvp("trig_ef_xe70",_trig_ef_xe70);
    ar & boost::serialization::make_nvp("trig_ef_xe70_tclcw",_trig_ef_xe70_tclcw);
    ar & boost::serialization::make_nvp("trig_ef_xe80",_trig_ef_xe80);
    ar & boost::serialization::make_nvp("trig_ef_xe80_tclcw",_trig_ef_xe80_tclcw);
    ar & boost::serialization::make_nvp("trig_ef_j110_a4tchad_xe75_tclcw",_trig_ef_j110_a4tchad_xe75_tclcw);
    ar & boost::serialization::make_nvp("trig_ef_j80_a4tchad_xe100_tclcw_veryloose",_trig_ef_j80_a4tchad_xe100_tclcw_veryloose);
    ar & boost::serialization::make_nvp("trig_ef_mu24i_tight",_trig_ef_mu24i_tight);
    ar & boost::serialization::make_nvp("trig_ef_mu36_tight",_trig_ef_mu36_tight);
  }

public:

  AnaTrigger()
    : _trig_ef_e22_medium()
    , _trig_ef_e20_loose()
    , _trig_ef_e10_medium()
    , _trig_ef_e10_loose()
    , _trig_ef_e5_loose()
    , _trig_ef_e15_medium()
    , _trig_ef_g20_loose()
    , _trig_ef_g40_loose()
    , _trig_l1_em3()
    , _trig_l1_em5()
    , _trig_l1_em10()
    , _trig_l1_em14()
    , _trig_ef_mu18_medium()
    , _trig_ef_mu18_mg_medium()
    , _trig_ef_mu40_msonly_barrel_medium()
    , _trig_ef_mu40_msonly_barrel()
    , _trig_ef_mu22_mg()
    , _trig_ef_mu20_mg()
    , _trig_ef_mu15_mu10_effs()
    , _trig_ef_mu15_mu10_effs_medium()
    , _trig_ef_mu18_mg()
    , _trig_ef_mu18()
    , _trig_ef_mu13_mg_tight()
    , _trig_ef_mu13_mg()
    , _trig_ef_mu10_mg()
    , _trig_ef_mu10()
    , _trig_ef_mu6()
    , _trig_ef_mu4()
    , _trig_l1_mu10()
    , _trig_l1_mu6()
    , _trig_l1_mu0()
    , _trig_l1_mbts_1_1_col()
    , _trig_l1_mbts_1_1_empty()
    , _trig_l1_mbts_1_1_unparied()
    , _trig_l1_mbts_1_1_unparied1()
    , _trig_l1_mbts_1_1_unparied2()
    , _trig_l1_mbts_1_1()
    , _trig_l1_mbts_1()
    , _trig_ef_e20_medium()
    , _trig_ef_e25_medium()
    , _trig_ef_e30_loose()
    , _trig_ef_2e10_medium()
    , _trig_ef_2e12_medium()
    , _trig_ef_2e12t_medium()
    , _trig_ef_2e12tvh_medium()
    , _trig_ef_e22_medium1()
    , _trig_ef_e22vh_medium1()
    , _trig_ef_e10_loose_mu6()
    , _trig_ef_mu15_mg()
    , _trig_ef_mu20()
    , _trig_ef_2mu10()
    , _trig_ef_2mu10_loose()
    , _trig_ef_j10_a4tc_effs()
    , _trig_ef_fj10_a4tc_effs()
    , _trig_ef_j15_a4tc_effs()
    , _trig_ef_fj15_a4tc_effs()
    , _trig_ef_j20_a4_effs()
    , _trig_ef_j20_a4tc_effs()
    , _trig_ef_fj20_a4tc_effs()
    , _run_number( 0 )
    , _lbn( -1 )
    , _error_lar( 0u ) // default: no error
    , _strict_checking( true )
    // trigger matching
    , _trig_EF_el_n()
    , _trig_EF_el_EF_e20_medium()
    , _trig_EF_el_eta()
    , _trig_EF_el_phi()
    , _trig_L2_combmuonfeature_L2_mu18_MG()
    , _trig_L2_combmuonfeature_n()
    , _trig_L2_combmuonfeature_pt()
    , _trig_L2_combmuonfeature_eta()
    , _trig_L2_combmuonfeature_phi()
    , _trig_EF_trigmugirl_n()
    , _trig_EF_trigmugirl_track_n()
    , _trig_EF_trigmugirl_track_CB_pt()
    , _trig_EF_trigmugirl_track_CB_eta()
    , _trig_EF_trigmugirl_track_CB_phi()
    , _trig_L2_muonfeature_L2_mu40_MSonly_barrel()
    , _trig_L2_muonfeature_n()
    , _trig_L2_muonfeature_pt()
    , _trig_L2_muonfeature_eta()
    , _trig_L2_muonfeature_phi()
    , _trig_EF_trigmuonef_n()
    , _trig_EF_trigmuonef_track_n()
    , _trig_EF_trigmuonef_track_SA_pt()
    , _trig_EF_trigmuonef_track_SA_eta()
    , _trig_EF_trigmuonef_track_SA_phi()
    // MET triggers
    , _trig_ef_xe60()
    , _trig_ef_xe60t()
    , _trig_ef_xe60_tclcw_loose_delayed()
    , _trig_ef_xe60t_tclcw_loose_delayed()
    , _trig_ef_xe70()
    , _trig_ef_xe70_tclcw()
    , _trig_ef_xe80()
    , _trig_ef_xe80_tclcw()
    , _trig_ef_j110_a4tchad_xe75_tclcw()
    , _trig_ef_j80_a4tchad_xe100_tclcw_veryloose()
    , _trig_ef_mu24i_tight()
    , _trig_ef_mu36_tight()
  {}
  ~AnaTrigger() {}

  void strict_checking( const bool yes = true ) const { _strict_checking = yes; }

  /*
    Check if the event passes either of the given triggers (eg: TRIG1|TRIG2|TRIG3)
   */
  const bool passes_multiple(const std::string& name) const {
    bool result(false);
    static boost::char_separator<char> sep("|");
    boost::tokenizer<boost::char_separator<char> > tokens(name, sep);
    BOOST_FOREACH(std::string t, tokens) { result |= passes(t); }
    return result;
  }
  
  const bool passes(const std::string& name) const {
    const std::string upname = boost::to_upper_copy(name);
    try {
      if( upname == "L1_MU0" ) { return l1_mu0(); }
      if( upname == "L1_MU6" ) { return l1_mu6(); }
      if( upname == "L1_MU10" ) { return l1_mu10(); }
      if( upname == "EF_MU4" ) { return ef_mu4(); }
      if( upname == "EF_MU6" ) { return ef_mu6(); }
      if( upname == "EF_MU10" ) { return ef_mu10(); }
      if( upname == "EF_MU10_MG" ) { return ef_mu10_mg(); }
      if( upname == "EF_MU13_MG" ) { return ef_mu13_mg(); }
      if( upname == "EF_MU13_MG_TIGHT" ) { return ef_mu13_mg_tight(); }
      if( upname == "EF_MU15_MG" ) { return ef_mu15_mg(); }
      if( upname == "EF_MU18" ) { return ef_mu18(); }
      if( upname == "EF_MU18_MG" ) { return ef_mu18_mg(); }
      if( upname == "EF_MU20" ) { return ef_mu20(); }
      if( upname == "EF_MU20_MG" ) { return ef_mu20_mg(); }
      if( upname == "EF_MU22_MG" ) { return ef_mu22_mg(); }
      if( upname == "EF_MU40_MSONLY_BARREL" ) { return ef_mu40_msonly_barrel(); }
      if( upname == "EF_MU18_MEDIUM" ) { return ef_mu18_medium(); }
      if( upname == "EF_MU18_MG_MEDIUM" ) { return ef_mu18_mg_medium(); }
      if( upname == "EF_MU40_MSONLY_BARREL_MEDIUM" ) { return ef_mu40_msonly_barrel_medium(); }
      if( upname == "EF_2MU10" ) { return ef_2mu10(); }
      if( upname == "EF_2MU10_LOOSE" ) { return ef_2mu10_loose(); }
      if( upname == "L1_EM14" ) { return l1_em14(); }
      if( upname == "L1_EM10" ) { return l1_em10(); }
      if( upname == "L1_EM5" ) { return l1_em5(); }
      if( upname == "L1_EM3" ) { return l1_em3(); }
      if( upname == "EF_E5_LOOSE" ) { return ef_e5_loose(); }
      if( upname == "EF_E15_MEDIUM" ) { return ef_e15_medium(); }
      if( upname == "EF_E20_MEDIUM" ) { return ef_e20_medium(); }
      if( upname == "EF_E25_MEDIUM" ) { return ef_e25_medium(); }
      if( upname == "EF_E20_LOOSE" ) { return ef_e20_loose(); }
      if( upname == "EF_E30_LOOSE" ) { return ef_e30_loose(); }
      if( upname == "EF_G20_LOOSE" ) { return ef_g20_loose(); }
      if( upname == "EF_G40_LOOSE" ) { return ef_g40_loose(); }
      if( upname == "EF_E10_LOOSE" ) { return ef_e10_loose(); }
      if( upname == "EF_E10_MEDIUM" ) { return ef_e10_medium(); }
      if( upname == "EF_2E10_MEDIUM" ) { return ef_2e10_medium(); }
      if( upname == "EF_E10_LOOSE_MU6" ) { return ef_e10_loose_mu6(); }
      if( upname == "L1_MBTS_1_1_Col" ) { return l1_mbts_1_1_col(); }
      if( upname == "L1_MBTS_1_1_EMPTY" ) { return l1_mbts_1_1_empty(); }
      if( upname == "L1_MBTS_1_1_UNPARIED" ) { return l1_mbts_1_1_unparied(); }
      if( upname == "L1_MBTS_1_1_UNPARIED1" ) { return l1_mbts_1_1_unparied1(); }
      if( upname == "L1_MBTS_1_1_UNPARIED2" ) { return l1_mbts_1_1_unparied2(); }
      if( upname == "L1_MBTS_1_1" ) { return l1_mbts_1_1(); }
      if( upname == "L1_MBTS_1" ) { return l1_mbts_1(); }
      if( upname == "EF_J10_A4TC_EFFS" ) { return ef_j10_a4tc_effs(); }
      if( upname == "EF_FJ10_A4TC_EFFS" ) { return ef_fj10_a4tc_effs(); }
      if( upname == "EF_J15_A4TC_EFFS" ) { return ef_j15_a4tc_effs(); }
      if( upname == "EF_FJ15_A4TC_EFFS" ) { return ef_fj15_a4tc_effs(); }
      if( upname == "EF_J20_A4_EFFS" ) { return ef_j20_a4_effs(); }
      if( upname == "EF_J20_A4TC_EFFS" ) { return ef_j20_a4tc_effs(); }
      if( upname == "EF_FJ20_A4TC_EFFS" ) { return ef_fj20_a4tc_effs(); }
      if( upname == "EF_XE60" ) { return ef_xe60(); }
      if( upname == "EF_XE60T" ) { return ef_xe60t(); }
      if( upname == "EF_XE60_TCLCW_LOOSE_DELAYED" ) { return ef_xe60_tclcw_loose_delayed(); }
      if( upname == "EF_XE60T_TCLCW_LOOSE_DELAYED" ) { return ef_xe60t_tclcw_loose_delayed(); }
      if( upname == "EF_XE70" ) { return ef_xe70(); }
      if( upname == "EF_XE70_TCLCW" ) { return ef_xe70_tclcw(); }
      if( upname == "EF_XE80" ) { return ef_xe80(); }
      if( upname == "EF_XE80_TCLCW" ) { return ef_xe80_tclcw(); }
      if( upname == "EF_J110_A4TCHAD_XE75_TCLCW" ) { return ef_j110_a4tchad_xe75_tclcw(); }
      if( upname == "EF_J80_A4TCHAD_XE100_TCLCW_VERYLOOSE" ) { return ef_j80_a4tchad_xe100_tclcw_veryloose(); }
      if( upname == "EF_MU24I_TIGHT" ) { return ef_mu24i_tight(); }
      if( upname == "EF_MU36_TIGHT" ) { return ef_mu36_tight(); }
      if( upname == "ANY") return true;
      if( upname == "NONE") return false;
      throw std::out_of_range("Trigger "+name+" not defined");
    } catch( std::out_of_range& e ) {
      if( _strict_checking ) { throw e; }
    }
    return false;
  }

  const bool ef_e22_medium() const { 
    if( _trig_ef_e22_medium ) {
      return( *_trig_ef_e22_medium );
    } else {
      throw std::out_of_range("Trigger EF_E22_MEDIUM not defined");
    } 
    return false;
  }
  const bool ef_e20_loose() const { 
    if( _trig_ef_e20_loose ) {
      return( *_trig_ef_e20_loose );
    } else {
      throw std::out_of_range("Trigger EF_E20_LOOSE not defined");
    } 
    return false;
  }
  const bool ef_e10_medium() const { 
    if( _trig_ef_e10_medium ) {
      return( *_trig_ef_e10_medium );
    } else {
      throw std::out_of_range("Trigger EF_E10_MEDIUM not defined");
    }
    return false;
  }
  const bool ef_e10_loose() const { 
    if( _trig_ef_e10_loose ) {
      return( *_trig_ef_e10_loose );
    } else {
      throw std::out_of_range("Trigger EF_E10_LOOSE not defined");
    }
    return false;
  }
  const bool ef_e5_loose() const { 
    if( _trig_ef_e5_loose ) {
      return( *_trig_ef_e5_loose );
    } else {
      throw std::out_of_range("Trigger EF_E5_LOOSE not defined");
    } 
    return false;
  }
  const bool ef_e15_medium() const {
    if( _trig_ef_e15_medium ) {
      return( *_trig_ef_e15_medium );
    } else {
      throw std::out_of_range("Trigger EF_E15_MEDIUM not defined");
    }
    return false;
  }
  const bool ef_g20_loose() const {
    if( _trig_ef_g20_loose ) {
      return( *_trig_ef_g20_loose );
    } else {
      throw std::out_of_range("Trigger EF_G20_LOOSE not defined");
    }
    return false;
  }
  const bool ef_g40_loose() const {
    if( _trig_ef_g40_loose ) {
      return( *_trig_ef_g40_loose );
    } else {
      throw std::out_of_range("Trigger EF_G40_LOOSE not defined");
    }
    return false;
  }
  const bool l1_em3() const { 
    if( _trig_l1_em3 ) {
      return( *_trig_l1_em3 );
    } else {
      throw std::out_of_range("Trigger L1_EM3 not defined");
    } 
    return false;
  }
  const bool l1_em5() const { 
    if( _trig_l1_em5 ) { 
      return( *_trig_l1_em5 );
    } else {
      throw std::out_of_range("Trigger L1_EM5 not defined");
    }
    return false;
  }
  const bool l1_em10() const { 
    if( _trig_l1_em10 ) {
      return( *_trig_l1_em10 );
    } else {
      throw std::out_of_range("Trigger L1_EM10 not defined");
    }
    return false;
  }
  const bool l1_em14() const { 
    if( _trig_l1_em14 ) {
      return( *_trig_l1_em14 );
    } else {
      throw std::out_of_range("Trigger L1_EM14 not defined");
    }
    return false;
  }
  const bool ef_mu18_medium() const { 
    if( _trig_ef_mu18_medium ) {
      return( *_trig_ef_mu18_medium );
    } else {
      throw std::out_of_range("Trigger EF_MU18_MEDIUM not defined");
    } 
    return false;
  }
  const bool ef_mu18_mg_medium() const { 
    if( _trig_ef_mu18_mg_medium ) {
      return( *_trig_ef_mu18_mg_medium );
    } else {
      throw std::out_of_range("Trigger EF_MU18_MG_MEDIUM not defined");
    } 
    return false;
  }
  const bool ef_mu40_msonly_barrel_medium() const { 
    if( _trig_ef_mu40_msonly_barrel_medium ) {
      return( *_trig_ef_mu40_msonly_barrel_medium );
    } else {
      throw std::out_of_range("Trigger EF_MU40_MSONLY_BARREL_MEDIUM not defined");
    } 
    return false;
  }
  const bool ef_mu40_msonly_barrel() const { 
    if( _trig_ef_mu40_msonly_barrel ) {
      return( *_trig_ef_mu40_msonly_barrel );
    } else {
      throw std::out_of_range("Trigger EF_MU40_MSONLY_BARREL not defined");
    } 
    return false;
  }
  const bool ef_mu22_mg() const { 
    if( _trig_ef_mu22_mg ) {
      return( *_trig_ef_mu22_mg );
    } else {
      throw std::out_of_range("Trigger EF_MU22_MG not defined");
    } 
    return false;
  }
  const bool ef_mu20_mg() const { 
    if( _trig_ef_mu20_mg ) {
      return( *_trig_ef_mu20_mg );
    } else {
      throw std::out_of_range("Trigger EF_MU20_MG not defined");
    } 
    return false;
  }
  const bool ef_mu18_mg() const { 
    if( _trig_ef_mu18_mg ) {
      return( *_trig_ef_mu18_mg );
    } else {
      throw std::out_of_range("Trigger EF_MU18_MG not defined");
    } 
    return false;
  }
  const bool ef_mu18() const { 
    if( _trig_ef_mu18 ) {
      return( *_trig_ef_mu18 );
    } else {
      throw std::out_of_range("Trigger EF_MU18 not defined");
    } 
    return false;
  }
  const bool ef_mu15_mu10_effs() const { 
    if( _trig_ef_mu15_mu10_effs ) {
      return( *_trig_ef_mu15_mu10_effs );
    } else {
      throw std::out_of_range("Trigger EF_MU15_MU10_EFFS not defined");
    } 
    return false;
  }
  const bool ef_mu15_mu10_effs_medium() const { 
    if( _trig_ef_mu15_mu10_effs_medium ) {
      return( *_trig_ef_mu15_mu10_effs_medium );
    } else {
      throw std::out_of_range("Trigger EF_MU15_MU10_EFFS_MEDIUM not defined");
    } 
    return false;
  }
  const bool ef_mu13_mg() const { 
    if( _trig_ef_mu13_mg ) {
      return( *_trig_ef_mu13_mg );
    } else {
      throw std::out_of_range("Trigger EF_MU13_MG not defined");
    } 
    return false;
  }
  const bool ef_mu13_mg_tight() const { 
    if( _trig_ef_mu13_mg_tight ) {
      return( *_trig_ef_mu13_mg_tight );
    } else {
      throw std::out_of_range("Trigger EF_MU13_MG_TIGHT not defined");
    } 
    return false;
  }
  const bool ef_mu10_mg() const { 
    if( _trig_ef_mu10_mg ) {
      return( *_trig_ef_mu10_mg );
    } else {
      throw std::out_of_range("Trigger EF_MU10_MG not defined");
    } 
    return false;
  }
  const bool ef_mu10() const { 
    if( _trig_ef_mu10 ) {
      return( *_trig_ef_mu10 );
    } else {
      throw std::out_of_range("Trigger EF_MU10 not defined");
    } 
    return false;
  }
  const bool ef_mu6() const { 
    if( _trig_ef_mu6 ) {
      return( *_trig_ef_mu6 );
    } else {
      throw std::out_of_range("Trigger EF_MU6 not defined");
    } 
    return false;
  }
  const bool ef_mu4() const { 
    if( _trig_ef_mu4 ) {
      return( *_trig_ef_mu4 );
    } else {
      throw std::out_of_range("Trigger EF_MU4 not defined");
    } 
    return false;
  }
  const bool l1_mu10() const { 
    if( _trig_l1_mu10 ) {
      return( *_trig_l1_mu10 );
    } else {
      throw std::out_of_range("Trigger L1_MU10 not defined");
    } 
    return false;
  }
  const bool l1_mu6() const { 
    if( _trig_l1_mu6 ) {
      return( *_trig_l1_mu6 );
    } else {
      throw std::out_of_range("Trigger L1_MU6 not defined");
    } 
    return false;
  }
  const bool l1_mu0() const { 
    if( _trig_l1_mu0 )
      return *_trig_l1_mu0;
    else
      throw std::out_of_range("Trigger L1_MU0 not defined");
    return false;
  }
  const bool ef_xe60() const {
    if( _trig_ef_xe60 ) return *_trig_ef_xe60;
    else throw std::out_of_range("Trigger EF_XE60 not defined");
    return false;
  }
  const bool ef_xe60t() const {
    if( _trig_ef_xe60t ) return *_trig_ef_xe60t;
    else throw std::out_of_range("Trigger EF_XE60T not defined");
    return false;
  }
  const bool ef_xe60_tclcw_loose_delayed() const {
    if( _trig_ef_xe60_tclcw_loose_delayed ) return *_trig_ef_xe60_tclcw_loose_delayed;
    else throw std::out_of_range("Trigger EF_XE60_TCLCW_LOOSE_DELAYED not defined");
    return false;
  }
  const bool ef_xe60t_tclcw_loose_delayed() const {
    if( _trig_ef_xe60t_tclcw_loose_delayed ) return *_trig_ef_xe60t_tclcw_loose_delayed;
    else throw std::out_of_range("Trigger EF_XE60T_TCLCW_LOOSE_DELAYED not defined");
    return false;
  }
  const bool ef_xe70() const {
    if( _trig_ef_xe70 ) return *_trig_ef_xe70;
    else throw std::out_of_range("Trigger EF_XE70 not defined");
    return false;
  }
  const bool ef_xe70_tclcw() const {
    if( _trig_ef_xe70_tclcw ) return *_trig_ef_xe70_tclcw;
    else throw std::out_of_range("Trigger EF_XE70_TCLCW not defined");
    return false;
  }
  const bool ef_xe80() const {
    if( _trig_ef_xe80 ) return *_trig_ef_xe80;
    else throw std::out_of_range("Trigger EF_XE80 not defined");
    return false;
  }
  const bool ef_xe80_tclcw() const {
    if( _trig_ef_xe80_tclcw ) return *_trig_ef_xe80_tclcw;
    else throw std::out_of_range("Trigger EF_XE80_TCLCW not defined");
    return false;
  }
  const bool ef_j110_a4tchad_xe75_tclcw() const {
    if( _trig_ef_j110_a4tchad_xe75_tclcw ) return *_trig_ef_j110_a4tchad_xe75_tclcw;
    else throw std::out_of_range("Trigger EF_J110_A4TCHAD_XE75_TCLCW not defined");
    return false;
  }
  const bool ef_j80_a4tchad_xe100_tclcw_veryloose() const {
    if( _trig_ef_j80_a4tchad_xe100_tclcw_veryloose ) return *_trig_ef_j80_a4tchad_xe100_tclcw_veryloose;
    else throw std::out_of_range("Trigger EF_J80_A4TCHAD_XE100_TCLCW_VERYLOOSE not defined");
    return false;
  }
  const bool ef_mu24i_tight() const {
    if( _trig_ef_mu24i_tight ) return *_trig_ef_mu24i_tight;
    else throw std::out_of_range("Trigger EF_MU24I_TIGHT no defined");
    return false;
  }
  const bool ef_mu36_tight() const {
    if( _trig_ef_mu36_tight ) return *_trig_ef_mu36_tight;
    else throw std::out_of_range("Trigger EF_MU36_TIGHT no defined");
    return false;
  }

  const int lumi_block_number() const { return _lbn; }
  const int run_number() const { return _run_number; }
  const unsigned int error_lar() const { return _error_lar; }

  // Trigger selection specified by the WZ group (2/15/2011)
  // https://twiki.cern.ch/twiki/bin/view/AtlasProtected/WZElectroweakCommonTopics#Trigger
  const bool wz_muon_trigger_v15() const {
    if( _run_number < 152166 || _run_number >= 777777 ) { // signal and MC
      return ef_mu10_mg();
    } else if ( _run_number < 165703 ) { // E4 - G1 (note that this also includes A-E3)
      return ef_mu10_mg();
    } else if ( _run_number < 167607 ) { // G2 to part way through I1
      return ef_mu13_mg();
    } else if( _run_number <= 167844 ) { // up to I2
      return ef_mu13_mg_tight();
    } else assert( false );
  }
  const bool wz_muon_trigger_v16() const {
    if( _run_number < 152166 || _run_number >= 777777 ) { // signal and MC
      return ef_mu10_mg();      
    } else if ( _run_number < 165703 ) { // E4 - G1 (note that this also includes A-E3)
      return ef_mu10_mg();
    } else if ( _run_number < 167607 ) { // G2 to part way through I1
      return ef_mu13_mg();
    } else if( _run_number <= 167844 ) { // up to I2
      return ef_mu13_mg_tight();
    } else assert( false ); 
  }
  const bool wz_electron_trigger_v15() const {
    if( _run_number < 152166 || _run_number >= 777777 ) { // signal and MC
      return l1_em14();
    } else if ( _run_number < 160899 ) { // A - E3
      return l1_em14();
    } else if ( _run_number <= 167844 ) { // E4 - I2
      return ef_e15_medium();
    } else assert( false );
  }
  const bool wz_electron_trigger_v16() const {
    if( _run_number < 152166 || _run_number >= 777777 ) { // signal and MC
      return ef_e15_medium();
    } else if ( _run_number <= 167844 ) { // A - I2
      return ef_e15_medium();
    } else assert( false );
  }
  const bool wz_photon_trigger_v15() const {
    if( _run_number < 152166 || _run_number >= 777777 ) { // signal and MC
      return l1_em14();
    } else if ( _run_number < 160899 ) { // A - E3
      return l1_em14();
    } else if ( _run_number <= 167844 ) { // E4 - I2
      return ef_g20_loose();
    } else assert( false );
  }
  const bool wz_photon_trigger_v16() const {
    if( _run_number < 152166 || _run_number >= 777777 ) { // signal and MC
      return ef_g20_loose();
    } else if ( _run_number <= 167844 ) { // A - I2
      return ef_g20_loose();
    } else assert( false );
  }
  const bool wz_electron_trigger_v16_2011() const {
    return ef_e20_medium();
  }
  const bool wz_muon_trigger_v16_2011() const {
    return ef_mu18_mg() || ef_mu18() || ef_mu40_msonly_barrel();
  }
  const bool wz_single_muon_trigger_v17_2011( unsigned long run_number ) const {
    if( run_number < 186516 ) return ef_mu18_mg() || ef_mu40_msonly_barrel();
    else return ef_mu18_mg_medium() || ef_mu40_msonly_barrel_medium();
  }
  
  // updated 2012-01-26 to WZ EWK selection
  // https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/WZElectroweakCommonTopics2011#Triggers
  const bool wz_single_electron_trigger_2012( unsigned long run_number ) const { 
    if( run_number < 186873 ) return ef_e20_medium(); // A - J (recommended for D-J)
    else if( run_number < 188902 ) return ef_e22_medium(); // K
    else return ef_e22vh_medium1(); // > K
  }
  const bool wz_dielectron_trigger_2012( unsigned long run_number ) const {
    if( run_number < 186873 ) return ef_2e12_medium(); // A - J (recommended for D-J)
    else if( run_number < 188902 ) return ef_2e12t_medium(); // K
    else return ef_2e12tvh_medium();
  }
  const bool wz_single_muon_trigger_2012( unsigned long run_number ) const {
    if( run_number < 186516 ) return ef_mu18_mg() || ef_mu18(); // A - I (recommended for D-I)
    else return ef_mu18_mg_medium() || ef_mu18_medium();
  }
  const bool wz_dimuon_trigger_2012( unsigned long run_number ) const {
    if( run_number < 186516 ) return ef_mu15_mu10_effs(); // A - I (recommended for D-I)
    else return ef_mu15_mu10_effs_medium();
  }
  const bool alljet_trigger_2012() const {
    return ( ef_j10_a4tc_effs() || ef_fj10_a4tc_effs() ||
	     ef_j15_a4tc_effs() || ef_fj15_a4tc_effs() ||
	     ef_j20_a4_effs() || ef_j20_a4tc_effs() || ef_fj20_a4tc_effs() );
  }
  // W charge asymmetry trigger
  const bool wz_asym_muon_trigger_2012( unsigned long run_number ) const {
    if( run_number < 186516 ) return ef_mu18_mg();
    else if (run_number < 200804) return ef_mu18_mg_medium();
    else return ef_mu18_medium(); // 2012 data (testing only)
  }
  // W charge asymmetry trigger (non-mg version)
  const bool wz_asym_muon_trigger_2012_nomg( unsigned long run_number ) const {
    if( run_number < 186516 ) return ef_mu18();
    else if (run_number < 200804) return ef_mu18_medium();
    else return ef_mu18_medium(); // 2012 data (testing only)
  }
  // W charge asymmetry trigger (OR version)
  const bool wz_asym_muon_trigger_2012_or( unsigned long run_number ) const {
    if( run_number < 186516 ) return ef_mu18() || ef_mu18_mg();
    else if (run_number < 200804) return ef_mu18_medium() || ef_mu18_mg_medium();
    else {
      assert(false && "Unknown run number");
    }
    return false;
  }

  // updated 2012-02-07 top group selection
  const bool top_muon_trigger_2012( unsigned long run_number ) const {
    if( run_number < 186516 ) return ef_mu18(); // before J
    else return ef_mu18_medium(); // after J
  }
  const bool top_electron_trigger_2012_data( unsigned long run_number) const {
    if( run_number < 186873 ) return ef_e20_medium(); // before K
    else if( run_number < 188902 ) return ef_e22_medium(); // K
    else return ef_e22vh_medium1(); // after K
  }
  const bool top_electron_trigger_2012_mc11a( unsigned long run_number) const {
    if(run_number < 186873 ) return ef_e20_medium(); // before K
    else if( run_number < 188902 ) return ef_e22_medium(); // K
    else return ef_e22_medium1(); // after K
  }
  const bool top_electron_trigger_2012_mc11bc( unsigned long run_number) const {
    if(run_number < 186873 ) return ef_e20_medium(); // before K
    else if( run_number < 188902 ) return ef_e22_medium(); // K
    else return ef_e22vh_medium1(); // after K
  }

  // composite triggers for 2012 monojet analysis
  const bool monojet_met( unsigned long run_number ) const {
    if( run_number < 202660 ) return ef_xe70(); // A
    return ef_xe80();
  }

  const bool has_l1_mbts_1_1() const { return _trig_l1_mbts_1_1; }
  const bool l1_mbts_1_1_col() const { if( _trig_l1_mbts_1_1_col ) { return  *_trig_l1_mbts_1_1_col; } else { return false; } }
  const bool l1_mbts_1_1_empty() const { if( _trig_l1_mbts_1_1_empty ) { return  *_trig_l1_mbts_1_1_empty; } else { return false; } }
  const bool l1_mbts_1_1_unparied() const { if( _trig_l1_mbts_1_1_unparied ) { return  *_trig_l1_mbts_1_1_unparied; } else { return false; } }
  const bool l1_mbts_1_1_unparied1() const { if( _trig_l1_mbts_1_1_unparied1 ) { return  *_trig_l1_mbts_1_1_unparied1; } else { return false; } }
  const bool l1_mbts_1_1_unparied2() const { if( _trig_l1_mbts_1_1_unparied2 ) { return  *_trig_l1_mbts_1_1_unparied2; } else { return false; } }
  const bool l1_mbts_1_1() const { if( _trig_l1_mbts_1_1 ) { return  *_trig_l1_mbts_1_1; } else { return false; } }
  const bool l1_mbts_1() const { if( _trig_l1_mbts_1 ) { return  *_trig_l1_mbts_1; } else { return false; } }

  const bool ef_e22_medium1() const { if( _trig_ef_e22_medium1 ) { return( *_trig_ef_e22_medium1 ); } else { throw std::out_of_range("Trigger EF_E22_MEDIUM1 not defined"); } }
  const bool ef_e22vh_medium1() const { if( _trig_ef_e22vh_medium1 ) { return( *_trig_ef_e22vh_medium1 ); } else { throw std::out_of_range("Trigger EF_E22VH_MEDIUM1 not defined"); } }
  const bool ef_e20_medium() const { if( _trig_ef_e20_medium ) { return( *_trig_ef_e20_medium ); } else { throw std::out_of_range("Trigger EF_E20_MEDIUM not defined"); } }
  const bool ef_e25_medium() const { if( _trig_ef_e25_medium ) { return( *_trig_ef_e25_medium ); } else { throw std::out_of_range("Trigger EF_E25_MEDIUM not defined"); } }
  const bool ef_e30_loose() const { if( _trig_ef_e30_loose ) { return( *_trig_ef_e30_loose ); } else { throw std::out_of_range("Trigger EF_E30_LOOSE not defined"); } }
  const bool ef_2e10_medium() const { if( _trig_ef_2e10_medium ) { return( *_trig_ef_2e10_medium ); } else { throw std::out_of_range("Trigger EF_2E10_MEDIUM not defined"); } }
  const bool ef_2e12_medium() const { if( _trig_ef_2e12_medium ) { return( *_trig_ef_2e12_medium ); } else { throw std::out_of_range("Trigger EF_2E12_MEDIUM not defined"); } }
  const bool ef_2e12t_medium() const { if( _trig_ef_2e12t_medium ) { return( *_trig_ef_2e12t_medium ); } else { throw std::out_of_range("Trigger EF_2E12T_MEDIUM not defined"); } }
  const bool ef_2e12tvh_medium() const { if( _trig_ef_2e12tvh_medium ) { return( *_trig_ef_2e12tvh_medium ); } else { throw std::out_of_range("Trigger EF_2E12TVH_MEDIUM not defined"); } }
  const bool ef_e10_loose_mu6() const { if( _trig_ef_e10_loose_mu6 ) { return( *_trig_ef_e10_loose_mu6 ); } else { throw std::out_of_range("Trigger EF_E10_LOOSE_MU6 not defined"); } }
  const bool ef_mu15_mg() const { if( _trig_ef_mu15_mg ) { return( *_trig_ef_mu15_mg ); } else { throw std::out_of_range("Trigger EF_MU15_MG not defined"); } }
  const bool ef_mu20() const { if( _trig_ef_mu20 ) { return( *_trig_ef_mu20 ); } else { throw std::out_of_range("Trigger EF_MU20 not defined"); } }
  const bool ef_2mu10() const { if( _trig_ef_2mu10 ) { return( *_trig_ef_2mu10 ); } else { throw std::out_of_range("Trigger EF_2MU10 not defined"); } }
  const bool ef_2mu10_loose() const { if( _trig_ef_2mu10_loose ) { return( *_trig_ef_2mu10_loose ); } else { throw std::out_of_range("Trigger EF_2MU10_LOOSE not defined"); } }

  const bool ef_j10_a4tc_effs() const { { if( _trig_ef_j10_a4tc_effs ) { return( *_trig_ef_j10_a4tc_effs ); } else { throw std::out_of_range("Trigger EF_J10_A4TC_EFFS not defined"); } } }
  const bool ef_fj10_a4tc_effs() const { { if( _trig_ef_fj10_a4tc_effs ) { return( *_trig_ef_fj10_a4tc_effs ); } else { throw std::out_of_range("Trigger EF_FJ10_A4TC_EFFS not defined"); } } }
  const bool ef_j15_a4tc_effs() const { { if( _trig_ef_j15_a4tc_effs ) { return( *_trig_ef_j15_a4tc_effs ); } else { throw std::out_of_range("Trigger EF_J15_A4TC_EFFS not defined"); } } }
  const bool ef_fj15_a4tc_effs() const { { if( _trig_ef_fj15_a4tc_effs ) { return( *_trig_ef_fj15_a4tc_effs ); } else { throw std::out_of_range("Trigger EF_FJ15_A4TC_EFFS not defined"); } } }
  const bool ef_j20_a4_effs() const { { if( _trig_ef_j20_a4_effs ) { return( *_trig_ef_j20_a4_effs ); } else { throw std::out_of_range("Trigger EF_J20_A4_EFFS not defined"); } } }
  const bool ef_j20_a4tc_effs() const { { if( _trig_ef_j20_a4tc_effs ) { return( *_trig_ef_j20_a4tc_effs ); } else { throw std::out_of_range("Trigger EF_J20_A4TC_EFFS not defined"); } } }
  const bool ef_fj20_a4tc_effs() const { { if( _trig_ef_fj20_a4tc_effs ) { return( *_trig_ef_fj20_a4tc_effs ); } else { throw std::out_of_range("Trigger EF_FJ20_A4TC_EFFS not defined"); } } }

  // trigger matching                                                                                                                                                                                    
  const int trig_EF_el_n() const { return _trig_EF_el_n; }
  const std::vector<int> trig_EF_el_EF_e20_medium() const { return _trig_EF_el_EF_e20_medium; }
  const std::vector<float> trig_EF_el_eta() const { return _trig_EF_el_eta; }
  const std::vector<float> trig_EF_el_phi() const { return _trig_EF_el_phi; }
  const std::vector<int>  trig_L2_combmuonfeature_L2_mu18_MG() const { return _trig_L2_combmuonfeature_L2_mu18_MG; }
  const int           trig_L2_combmuonfeature_n() const { return _trig_L2_combmuonfeature_n; }
  const std::vector<float>   trig_L2_combmuonfeature_pt() const { return _trig_L2_combmuonfeature_pt; }
  const std::vector<float>   trig_L2_combmuonfeature_eta() const { return _trig_L2_combmuonfeature_eta; }
  const std::vector<float>   trig_L2_combmuonfeature_phi() const { return _trig_L2_combmuonfeature_phi; }
  const int           trig_EF_trigmugirl_n() const { return _trig_EF_trigmugirl_n; }
  const std::vector<int>     trig_EF_trigmugirl_track_n() const { return _trig_EF_trigmugirl_track_n; }
  const std::vector<std::vector<float> > trig_EF_trigmugirl_track_CB_pt() const { return _trig_EF_trigmugirl_track_CB_pt; }
  const std::vector<std::vector<float> > trig_EF_trigmugirl_track_CB_eta() const { return _trig_EF_trigmugirl_track_CB_eta; }
  const std::vector<std::vector<float> > trig_EF_trigmugirl_track_CB_phi() const { return _trig_EF_trigmugirl_track_CB_phi; }
  const std::vector<int>     trig_L2_muonfeature_L2_mu40_MSonly_barrel() const { return _trig_L2_muonfeature_L2_mu40_MSonly_barrel; }
  const int           trig_L2_muonfeature_n() const { return _trig_L2_muonfeature_n; }
  const std::vector<float>   trig_L2_muonfeature_pt() const { return _trig_L2_muonfeature_pt; }
  const std::vector<float>   trig_L2_muonfeature_eta() const { return _trig_L2_muonfeature_eta; }
  const std::vector<float>   trig_L2_muonfeature_phi() const { return _trig_L2_muonfeature_phi; }
  const int           trig_EF_trigmuonef_n() const { return _trig_EF_trigmuonef_n; }
  const std::vector<int>     trig_EF_trigmuonef_track_n() const { return _trig_EF_trigmuonef_track_n; }
  const std::vector<std::vector<float> > trig_EF_trigmuonef_track_SA_pt() const { return _trig_EF_trigmuonef_track_SA_pt; }
  const std::vector<std::vector<float> > trig_EF_trigmuonef_track_SA_eta() const { return _trig_EF_trigmuonef_track_SA_eta; }
  const std::vector<std::vector<float> > trig_EF_trigmuonef_track_SA_phi() const { return _trig_EF_trigmuonef_track_SA_phi; }

  std::ostream& print( std::ostream& os = std::cout ) const {
    os << boost::format("AnaTrigger: electron: %|3| muon: %|3|") % ef_e10_medium() % ef_mu10_mg() << std::endl;
    return os;
  }

};

BOOST_CLASS_VERSION( AnaTrigger , 10 );

#endif // WRAP_ANATRIGGER_HPP
