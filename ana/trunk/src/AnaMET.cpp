#include <cmath>
#include <utility>
#include <limits>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include "TrigFTKAna/Detector.hpp"
#include "TrigFTKAna/AnaMuon.hpp"
#include "TrigFTKAna/AnaMET.hpp"
#include <iostream>

using namespace std;
using namespace boost;

std::pair<AnaMET::ftype,AnaMET::ftype>
AnaMET::_base_met_xy_cef( const boost::shared_ptr<const AnaMET>& met ) 
{
  // first, a sanity check that the calculation correctly reproduces
  // the MET without any changes. An assert here may indicate that the
  // ntuple reader has not properly filled _MET_cal_etx and the
  // other MET component variables. Check that each variable is filled
  // by the ntuple reader, and that the corresponding TBranches are
  // enabled.
  const AnaMET tmp_met( *met );
  // tmp_met.print( std::cout );
  const ftype delta_met_sanity_x = std::abs(met->px()-(met->_MET_cal_etx));
  const ftype delta_met_sanity_y = std::abs(met->py()-(met->_MET_cal_ety));
  // const ftype delta_met_sanity_x = std::abs(met->px()-(met->_MET_cal_etx+met->_MET_MuonBoy_etx-met->_MET_RefMuonTrack_etx));
  // const ftype delta_met_sanity_y = std::abs(met->py()-(met->_MET_cal_ety+met->_MET_MuonBoy_ety-met->_MET_RefMuonTrack_ety));
  assert( (met->met_type()!=LOCHADTOPO && met->met_type()!=LOCHADTOPOCEF )  ||
          ( delta_met_sanity_x<=0.001 && delta_met_sanity_y<=0.001 ) );
  
  // electron energy rescaling correction.
  // ftype sfac=energy/el_cl_E->at(i);
  // MET_EM_dx += (1. - (1./sfac)) * em3tmp.Px() *1000.;//OTHER MET terms are in MeV!!!
  // MET_EM_dy += (1. - (1./sfac)) * em3tmp.Py() *1000.;//OTHER MET terms are in MeV!!!
       
  // |eta|<4.5 cal MET
  // ----------------------------------------------------------------
  const ftype newMET_cal_etx = met->_MET_cal_etx_CentralReg + met->_MET_cal_etx_EndcapRegion + met->_MET_cal_etx_ForwardReg;
  const ftype newMET_cal_ety = met->_MET_cal_ety_CentralReg + met->_MET_cal_ety_EndcapRegion + met->_MET_cal_ety_ForwardReg;

  return std::pair<ftype,ftype>( newMET_cal_etx , newMET_cal_ety );
}

void
AnaMET::_accumulate_muon_delta_met_xy( const boost::shared_ptr<const AnaMuon>& mu , std::pair<AnaMET::ftype,AnaMET::ftype>& delta_met_xy )
{
  boost::shared_ptr<const AnaMuon> mu_uncorrected = mu->uncorrected_parent();
  if( !mu_uncorrected ) { return; } // no MuonBoy MET correction necessary
  while( mu_uncorrected->is_corrected() ) {
    mu_uncorrected = mu_uncorrected->uncorrected_parent();
  }
  delta_met_xy.first += ( mu->px() - mu_uncorrected->px() );
  delta_met_xy.second += ( mu->py() - mu_uncorrected->py() );
}


/*
{
  // LBNL calculation. should be logically the same as the one I implemented for the VLQ search below.
  m_nt_MetX  [iM]    =  fD3PDReader->MET_cal_etx_CentralReg + fD3PDReader->MET_cal_etx_EndcapRegion + fD3PDReader->MET_cal_etx_ForwardReg ;
  m_nt_MetY  [iM]    =  fD3PDReader->MET_cal_ety_CentralReg + fD3PDReader->MET_cal_ety_EndcapRegion + fD3PDReader->MET_cal_ety_ForwardReg ;

  m_nt_MetX  [iM]    += fD3PDReader->MET_MuonBoy_et*cos(fD3PDReader->MET_MuonBoy_phi) - fD3PDReader->MET_RefMuon_Track_et*cos(fD3PDReader->MET_RefMuon_Track_phi);
  m_nt_MetY  [iM]    += fD3PDReader->MET_MuonBoy_et*sin(fD3PDReader->MET_MuonBoy_phi) - fD3PDReader->MET_RefMuon_Track_et*sin(fD3PDReader->MET_RefMuon_Track_phi);
  
  m_nt_MetX [iM]/=1000;
  m_nt_MetY [iM]/=1000;
 
  m_nt_MetX  [iM]= m_nt_MetX [iM]- l0->fDelPx;  // fDelPx calculated as in WW twiki page
  m_nt_MetY  [iM]= m_nt_MetY [iM]- l0->fDelPy;
 
  m_nt_MetDelPhiCloseLep[iM]= 4.;

  TVector2 metP2( m_nt_MetX[iM] , m_nt_MetY[iM] );
  TVector2 l0P2(l0->fP4.Px(),l0->fP4.Py());
  TVector2 l1P2(l1->fP4.Px(),l1->fP4.Py());

  float delPhi = fabs(metP2.DeltaPhi(l0P2));
  if( delPhi < m_nt_MetDelPhiCloseLep[iM] ) { 
    m_nt_MetDelPhiCloseLep[iM]= delPhi;
  }

  delPhi = fabs(metP2.DeltaPhi(l1P2));
  if( delPhi < m_nt_MetDelPhiCloseLep[iM] ) { 
    m_nt_MetDelPhiCloseLep[iM]= delPhi;
  }

   m_nt_MetDelPhiCloseJet [iM] = 4;
   for( unsigned int i=0;i<jetList.size();i++ ) {
     AnaParticle* jet = jetList.at(i);
     TVector2 jetP2(jet->fP4.Px(), jet->fP4.Py());
     delPhi = fabs( metP2.DeltaPhi(jetP2) );
     if(delPhi<m_nt_MetDelPhiCloseJet[iM]) {
       m_nt_MetDelPhiCloseJet[iM]=delPhi;
     }
   }

   if(m_nt_MetDelPhi[iM] < TMath::Pi()/2)
     m_nt_MetRel [iM] =  m_nt_Met[iM]* sin(m_nt_MetDelPhi[iM]);
   else
     m_nt_MetRel [iM] =  m_nt_Met[iM];
}
*/

boost::shared_ptr<const AnaMET> 
AnaMET::_make_recomputed_met_hww( const boost::shared_ptr<const AnaMET>& met , 
                                  const std::pair<AnaMET::ftype,AnaMET::ftype>& met_xy ,
                                  const std::pair<AnaMET::ftype,AnaMET::ftype>& muon_delta_met_xy )
{
  const ftype muonboy_etx_corr = met->_MET_MuonBoy_etx - muon_delta_met_xy.first;
  const ftype muonboy_ety_corr = met->_MET_MuonBoy_ety - muon_delta_met_xy.second;

  const bool use_muon_term = met->met_type()!=LOCHADTOPOELHIGHETA;

  const ftype new_MET_x = met_xy.first + (use_muon_term ? muonboy_etx_corr - met->_MET_RefMuonTrack_etx : 0.);
  const ftype new_MET_y = met_xy.second + (use_muon_term ? muonboy_ety_corr - met->_MET_RefMuonTrack_ety : 0.);
  
  const ftype new_met_pt = detector::quadrature( new_MET_x , new_MET_y );
  
  boost::shared_ptr<AnaMET> new_met( boost::make_shared<AnaMET>( *met ) );
  new_met->_momentum.SetPxPyPzE( new_MET_x , new_MET_y , 0 , new_met_pt );
  new_met->_uncorrected_parent = met;
  
  return boost::const_pointer_cast<const AnaMET>( new_met );
}

boost::shared_ptr<const AnaMET>
AnaMET::remove_four_vector( const boost::shared_ptr<const AnaMET>& met , const TLorentzVector& vec )
{
  // make new MET and fill four-vector
  boost::shared_ptr<AnaMET> newMET(boost::make_shared<AnaMET>(*met) );
  newMET->_momentum.SetPxPyPzE( met->_momentum.Px() + vec.Px() ,
				met->_momentum.Py() + vec.Py() ,
				0. ,
				detector::quadrature( met->_momentum.Px() + vec.Px() , met->_momentum.Py() + vec.Py() ) );

  if( false ) {
    std::cout << "DEBUG AnaMET::remove_four_vector init_met = ( " << met->_momentum.Px() << " , " << met->_momentum.Py() << " ) = " << met->pt() << std::endl;
    std::cout << "                                      vec = ( " << vec.Px() << " , " << vec.Py() << " )" << std::endl;
    std::cout << "                                final_met = ( " << newMET->_momentum.Px() << " , " << newMET->_momentum.Py() << " ) = " << newMET->pt() << std::endl;
  }

  // define parent
  newMET->_uncorrected_parent = met;
  return boost::const_pointer_cast<const AnaMET>( newMET );
}

boost::shared_ptr<const AnaMET>
AnaMET::add_four_vector( const boost::shared_ptr<const AnaMET>& met , const TLorentzVector& vec )
{
  // make new MET and fill four-vector
  boost::shared_ptr<AnaMET> newMET(boost::make_shared<AnaMET>(*met) );
  newMET->_momentum.SetPxPyPzE( met->_momentum.Px() - vec.Px() ,
				met->_momentum.Py() - vec.Py() ,
				0. ,
				detector::quadrature( met->_momentum.Px() - vec.Px() , met->_momentum.Py() - vec.Py() ) );

  if( false ) {
    std::cout << "DEBUG    AnaMET::add_four_vector init_met = ( " << met->_momentum.Px() << " , " << met->_momentum.Py() << " ) = " << met->pt() << std::endl;
    std::cout << "                                      vec = ( " << vec.Px() << " , " << vec.Py() << " )" << std::endl;
    std::cout << "                                final_met = ( " << newMET->_momentum.Px() << " , " << newMET->_momentum.Py() << " ) = " << newMET->pt() << std::endl;
  }

  // define parent
  newMET->_uncorrected_parent = met;
  return boost::const_pointer_cast<const AnaMET>( newMET );
}

boost::shared_ptr<const AnaMET>
AnaMET::met_corrected( const boost::shared_ptr<const AnaMET>& met , const float& et , const float& phi )
{
  boost::shared_ptr<AnaMET> newMET( boost::make_shared<AnaMET>( *met ) );
  newMET->_momentum.SetPtEtaPhiM( et , 0. , phi , 0. );
  newMET->_uncorrected_parent = met;
  return boost::const_pointer_cast<const AnaMET>( newMET );
}

  // template< typename muonIterT >
  // const boost::shared_ptr<const AnaMET> 
  // AnaMET::recompute_for_hww( const boost::shared_ptr<const AnaMET>& met , muonIterT begin_muons , muonIterT end_muons )
  // {
  //   // correct met for muons (and in the future other objects) according to the
  //   // H->WW instructions at https://twiki.cern.ch/twiki/bin/view/AtlasProtected/BaslinecutsforWWCONF.

  //   // first, a sanity check that the calculation correctly reproduces
  //   // the MET without any changes.  this code should be commented out.
  //   assert( (met->_type!=LOCHADTOPO && met->_type!=LOCHADTOPOMUHIGHETA && met->_type!=LOCHADTOPOCEF )  ||
  //           ( (met->_et_x==met->_MET_cal_etx+met->_MET_MuonBoy_etx-met->_MET_RefMuonTrack_etx) &&
  //             (met->_et_y==met->_MET_cal_ety+met->_MET_MuonBoy_ety-met->_MET_RefMuonTrack_ety) ) );
  
  //   // electron energy rescaling correction.
  //   // ftype sfac=energy/el_cl_E->at(i);
  //   // MET_EM_dx += (1. - (1./sfac)) * em3tmp.Px() *1000.;//OTHER MET terms are in MeV!!!
  //   // MET_EM_dy += (1. - (1./sfac)) * em3tmp.Py() *1000.;//OTHER MET terms are in MeV!!!
       
  //   // |eta|<4.5 cal MET
  //   // ----------------------------------------------------------------
  //   const ftype newMET_cal_etx = met->_MET_cal_etx_CentralReg + met->_MET_cal_etx_EndcapRegion + met->_MET_cal_etx_ForwardReg;
  //   const ftype newMET_cal_ety = met->_MET_cal_ety_CentralReg + met->_MET_cal_ety_EndcapRegion + met->_MET_cal_ety_ForwardReg;

  //   // correct muon boy term
  //   // ----------------------------------------------------------------
  //   // MET_MuonBoy_etx_corr = MET_MuonBoy_etx - dpx;
  //   // MET_MuonBoy_ety_corr = MET_MuonBoy_ety - dpy;

  //   ftype delta_muonboy_etx = 0.;
  //   ftype delta_muonboy_ety = 0.;
  //   for( muonIterT i=begin_muons, f=end_muons; i!=f; ++i ) {
  //     const boost::shared_ptr<const AnaMuon> mu( *i ); assert( mu );
  //     const boost::shared_ptr<const AnaMuon> mu_uncorrected = mu->uncorrected_parent();
  //     if( !mu_uncorrected ) { continue; } // no MuonBoy MET correction necessary
  //     while( mu_uncorrected->is_corrected() ) {
  //       mu_uncorrected = mu_uncorrected->uncorrected_parent();
  //     }
  //     delta_muonboy_etx += ( mu->px() - mu_uncorrected->px() );
  //     delta_muonboy_ety += ( mu->py() - mu_uncorrected->py() );
  //   }
  //   const ftype muonboy_etx_corr = met->_MET_MuonBoy_etx - delta_muonboy_etx;
  //   const ftype muonboy_ety_corr = met->_MET_MuonBoy_ety - delta_muonboy_ety;

  //   const bool use_muon_term = met->_type!=LOCHADTOPOELHIGHETA;

  //   const ftype new_MET_x = newMET_cal_etx + (use_muon_term ? muonboy_etx_corr - _MET_RefMuonTrack_etx : 0.);
  //   const ftype new_MET_y = newMET_cal_ety + (use_muon_term ? muonboy_ety_corr - _MET_RefMuonTrack_ety : 0.);
  
  //   const ftype new_met_pt = detector::quadrature( new_MET_x , new_MET_y );
  
  //   boost::shared_ptr<AnaMET> new_met( boost::make_shared<AnaMET>( *met ) );
  //   new_met->_momentum.SetPxPyPzE( new_MET_x , new_MET_y , 0 , new_met_pt );
  //   new_met->_sum_et = new_met_pt;
  //   new_met->_sum_et_x = -new_MET_x;
  //   new_met->_sum_et_y = -new_MET_y;
  //   new_met->_uncorrected_parent = met;
  
  //   return boost::const_pointer_cast<const AnaMET>( new_met );
  // }

