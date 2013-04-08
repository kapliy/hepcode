
#include <string>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <limits>
#include <vector>
#include <map>
#include <cmath>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/format.hpp>
#include <TChain.h>
#include <TFile.h>
#include <TBranch.h>
#include <TROOT.h>
#include <TSystem.h>
#include <TMatrix.h>
#include "TrigFTKAna/Trees/NtJetTag.hpp"
#include "TrigFTKAna/AnaTrack.hpp"
#include "TrigFTKAna/AnaJet.hpp"
#include "TrigFTKAna/Detector.hpp"
#include "TrigFTKAna/UniqueBarcode.hpp"
#include "TrigFTKAna/ana_streams_helper.hpp"
#include "TrigFTKAna/Trees/TagTree.h"

using namespace std;

extern TROOT* gROOT;
extern TSystem* gSystem;

const bool
NtJetTag::begin_file( const std::string& filename )
{
  // why oh why is this necessary? answer: it isn't, not yet anyway.
  //   gROOT->ProcessLine("#include <vector>"); 
  //   gROOT->ProcessLine("#include <utility>"); 

  _chain.reset( new TChain("vtuple") ); 
  _chain->SetDirectory( 0 );
  assert( _chain );

  _chain->Reset();
  _chain->AddFile( ana_streams::root_dcap_filename(filename).c_str() );

  _madeclass.reset( new TagTree(_chain.get()) );
  _madeclass->Init( _chain.get() );

  _n_events = static_cast<unsigned long>(_chain->GetEntries()); // slow for long chains.

  _open_filename = ana_streams::root_dcap_filename(filename);
  return true;
}

const bool
NtJetTag::end_file()
{
  _n_events = 0;
  _open_filename = string();
  return true;
}

void
NtJetTag::set_enabled_branches()
{
}

void
NtJetTag::set_branch_addresses()
{
}


const bool
NtJetTag::get_event( const unsigned long& ievent )
{
  if( !_chain ) { return false; }
  if( !_madeclass ) { return false; }
  if( _n_events == 0ul ) { return false; }
  if( ievent>=_n_events ) { return false; }

  if( _madeclass->LoadTree( ievent ) < 0 ) { return false; }
  _madeclass->GetEntry( ievent ); // returns nbytes

  clear();

  assert( _madeclass.get() );

  // load active branches
  _chain->GetEntry( ievent );

  return _get_event( _madeclass.get() );
}

template<typename vntupleT>
const bool
NtJetTag::_get_event( const vntupleT* nt )
{
  _run_number = nt->run;
  _event_number = nt->evt;
  return true;  
}

const unsigned long
NtJetTag::count_events( const std::string& filename )
{
  TChain chain( "vtuple" );
  chain.SetDirectory( 0 );
  chain.Reset();
  chain.AddFile( filename.c_str() );
  return( chain.GetEntries() );
}


void
NtJetTag::fill_jet_tag_info( vector< boost::shared_ptr<const AnaJet> >::iterator begin , vector< boost::shared_ptr<const AnaJet> >::iterator end ) const
{
  // loop over all AnaJet's in collT. find matching jet and fill AnaJet offline tagging info.
  const TagTree* nt( _madeclass.get() ); assert( nt );
  if( nt->njetsC4H1TJ>=10 ) { 
    cout << "warning: jet tag info collection has hit the limit for njetsC4H1TJ" << endl;
  }
  for( vector< boost::shared_ptr<const AnaJet> >::iterator i=begin; i!=end; ++i ) { 
    const boost::shared_ptr<const AnaJet>& jet( *i ); assert( jet );
    // match jet to tag info.
    int i_matching_jet = -1;
    double best_dr = 9999.;
    for( unsigned int intjet=0, fntjet=std::min(50u,nt->njetsC4H1TJ); intjet!=fntjet; ++intjet ) {
      const AnaJet::ftype absdphi = std::abs( detector::delta_phi( nt->phiC4H1TJ[intjet] , jet->phi() ) );
      const AnaJet::ftype absdeta = std::abs( detector::delta_eta( nt->etaC4H1TJ[intjet] , jet->eta() ) );
      if( jet->is_atlas_cone_4_tower() ) {
        if( absdphi > 0.01 ) { continue; }
        if( absdeta > 0.01 ) { continue; }
      } else if( jet->is_atlas_cone_7_tower() ) {
        if( absdphi > 0.01 ) { continue; }
        if( absdeta > 0.01 ) { continue; }
      } else if( jet->is_atlas_cone_4_truth() ) {
        if( nt->ptC4H1TJ[intjet] < 10E3 ) { continue; }
        if( absdphi > 0.4 ) { continue; }
        if( absdeta > 0.4 ) { continue; }
      } else if( jet->is_atlas_cone_4_truth_pileup() ) {
        if( nt->ptC4H1TJ[intjet] < 10E3 ) { continue; }
        if( absdphi > 0.4 ) { continue; }
        if( absdeta > 0.4 ) { continue; }
      } else {
        continue;
      }
      const AnaJet::ftype dr = detector::quadrature( absdphi , absdeta );
      if( dr > best_dr ) { continue; }
      best_dr = dr;
      i_matching_jet = intjet;
    } // end for each tag ntuple jet
    if( best_dr > 0.4 ) { continue; }
    assert( i_matching_jet>=0 );
    // fill offline tagging info.
    jet->_tag_ip2d_w = nt->ip2d_wC4H1TJ[i_matching_jet];
    jet->_tag_ip3d_w = nt->ip3d_wC4H1TJ[i_matching_jet]; 
    jet->_tag_sv0_w = nt->sv0_wC4H1TJ[i_matching_jet]; 
    jet->_tag_sv1_w = nt->sv1_wC4H1TJ[i_matching_jet]; 
    jet->_tag_sv2_w = nt->sv2_wC4H1TJ[i_matching_jet]; 
    jet->_tag_cmb_w = nt->cmb_wC4H1TJ[i_matching_jet]; 
    jet->_tag_jetp_w = nt->jetp_wC4H1TJ[i_matching_jet]; 
    jet->_tag_jetpneg_w = nt->jetpneg_wC4H1TJ[i_matching_jet]; 
    jet->_tag_trkc_w = nt->trkc_wC4H1TJ[i_matching_jet]; 
  } // end for each AnaJet
  if( true ) {
    // dump debugging output.
    cout << " ALL JETS IN TAG NTUPLE " << endl;
    for( unsigned int ijet=0, fjet=nt->njetsC4H1TJ; ijet!=fjet; ++ijet ) {
      cout << (boost::format( " %|8f| %|4f| %|4f| ip2d:%|8f| ip3d:%|8f| cmb:%|8f| lb:%|5d| drb:%|8f| pdg:%|8d|")
               % nt->ptC4H1TJ[ijet] % nt->etaC4H1TJ[ijet] % nt->phiC4H1TJ[ijet]
               % nt->ip2d_wC4H1TJ[ijet]
               % nt->ip3d_wC4H1TJ[ijet]
               % nt->cmb_wC4H1TJ[ijet]
               % nt->labelC4H1TJ[ijet]
               % nt->drminbC4H1TJ[ijet]
               % nt->truebpdgC4H1TJ[ijet]
               )
           << endl;
    }
    cout << " ALL JETS IN CBNT" << endl;
    for( vector< boost::shared_ptr<const AnaJet> >::iterator i=begin; i!=end; ++i ) { 
      const boost::shared_ptr<const AnaJet>& jet( *i ); assert( jet );
      cout << (boost::format( " %|8f| %|4f| %|4f| ip2d:%|8f| ip3d:%|8f| cmb:%|8f| drb:%|8f| dru:%|8f| drba:%|8f| drua:%|8f|")
               % jet->et()
               % jet->eta()
               % jet->phi()
               % jet->tag_ip2d_w()
               % jet->tag_ip3d_w()
               % jet->tag_cmb_w()
               % jet->dr_b_parton()
               % jet->dr_light_parton()
               % jet->dr_b_parton_any()
               % jet->dr_light_parton_any()
               ) << endl;
    }
  }
}
