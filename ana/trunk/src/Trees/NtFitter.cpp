#include "TrigFTKAna/Trees/NtFitter.hpp"
#include "TrigFTKAna_config.h"
#ifdef HAVE_LIBFTKSIM

#include <string>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>

#include "TChain.h"
#include "TrigFTKSim/FTKTrackStream.h"
#include "TrigFTKSim/FTKTrack.h"
#include "TrigFTKAna/ana_streams_helper.hpp"

using namespace std;

const bool
NtFitter::begin_file( const int& bank , const std::string& filename )
{
  if( !_chain ) { 
    _chain.reset( new TChain("ftkdata") ); 
    _chain->SetDirectory( 0 );
  }
  if( !_track_stream ) { 
    // abuse shared_ptr so that ROOT SetBranchAddress behaves correctly.
    _track_stream_ptr = new FTKTrackStream;
    _track_stream.reset( _track_stream_ptr ); 
  }

  _bank = bank;
  _chain->Reset();
  _chain->AddFile( ana_streams::root_dcap_filename(filename).c_str() );
  _n_events = _chain->GetEntries();

  if( _bank==MERGED_BANK ) {
    _chain->SetBranchStatus( "*" , 1 );
    _chain->SetBranchAddress( "FTKBankMerged" , &_track_stream_ptr );
  } else {
    stringstream bankstream;
    stringstream bankbranch;
    bankstream << "FTKBank" << _bank << ".";
    bankbranch << bankstream.str() << "*";
    _chain->SetBranchStatus( "*" , 0 );
    _chain->SetBranchStatus( bankbranch.str().c_str() , 1 );
    _chain->SetBranchAddress( bankstream.str().c_str() , &_track_stream_ptr );
  }


  _n_ftk_tracks = 0ul;
  _open_filename = ana_streams::root_dcap_filename(filename);
  return true;
}

const bool
NtFitter::end_file()
{
  _open_filename = string();
  return true;
}

const bool
NtFitter::get_event( const unsigned long& ievent )
{
  if( !_chain ) { return false; }
  if( _n_events == 0ul ) { return false; }
  if( ievent>=_n_events ) { return false; }

  _chain->GetEntry( ievent );
  clear();

  _run_number = _track_stream->runNumber();
  _event_number = _track_stream->eventNumber();

  // convert ftk tracks in track stream to internal representations
  _n_ftk_tracks = _track_stream->getNTracks();
  _n_ftk_combinations = _track_stream->getNCombs();
  _n_ftk_fits = _track_stream->getNFits();
  _n_ftk_fits_majority = _track_stream->getNFitsMajority();
  _n_ftk_fits_rec = _track_stream->getNFitsRecovery();
  _n_ftk_fits_addrec = _track_stream->getNAddFitsRecovery();
  _n_ftk_fits_bad = _track_stream->getNFitsBad();
  _n_ftk_fits_bad_majority = _track_stream->getNFitsBadMajority();
  _n_ftk_fits_hitwarrior_rejected = _track_stream->getNFitsHWRejected();
  _n_ftk_fits_bad_majority = _track_stream->getNFitsBadMajority();
  _n_ftk_fits_hitwarrior_rejected_majority = _track_stream->getNFitsHWRejectedMajority();
  // Incomplete tracks (option A):
  _n_ftk_tracksI = _track_stream->getNTracksI();
  _n_ftk_combinationsI = _track_stream->getNCombsI();
  _n_ftk_fitsI = _track_stream->getNFitsI();
  _n_ftk_fits_majorityI = _track_stream->getNFitsMajorityI();
  _n_ftk_fits_recI = _track_stream->getNFitsRecoveryI();
  _n_ftk_fits_addrecI = _track_stream->getNAddFitsRecoveryI();
  _n_ftk_fits_badI = _track_stream->getNFitsBadI();
  _n_ftk_fits_bad_majorityI = _track_stream->getNFitsBadMajorityI();
  _n_ftk_fits_hitwarrior_rejectedI = _track_stream->getNFitsHWRejectedI();
  _n_ftk_fits_bad_majorityI = _track_stream->getNFitsBadMajorityI();
  _n_ftk_fits_hitwarrior_rejected_majorityI = _track_stream->getNFitsHWRejectedMajorityI();

  for( int incomplete = 0; incomplete <= 1; incomplete++ ) {
    boost::function<FTKTrack* (unsigned long) > trackGetter; // behold the boost magic
    if( incomplete ) trackGetter = boost::bind(&FTKTrackStream::getTrackI,_track_stream,_1);
    else trackGetter = boost::bind(&FTKTrackStream::getTrack,_track_stream,_1);
    for( unsigned long itrack=0; itrack!=(incomplete?_n_ftk_tracksI:_n_ftk_tracks); ++itrack ) {
      const FTKTrack* track( trackGetter(itrack) ); assert( track );
      boost::shared_ptr<AnaTrack> tmp( new AnaTrack );
      tmp->_alg = AnaTrack::Algorithm::FTK;
      tmp->_curv = 2.*track->getInvPt();
      tmp->_d0 = track->getIP();
      tmp->_d0_raw = track->getIPRaw();
      tmp->_phi0 = track->getPhi();
      tmp->_phi0_raw = track->getPhiRaw();
      tmp->_z0 = track->getZ0();
      tmp->_z0_raw = track->getZ0Raw();
      tmp->_lambda = track->getCotTheta();
      tmp->_ftk_bitmask = track->getBitmask();
      // Turn to 11L bitmask. This might get untenable if we keep
      // coming up with more plane maps.
      // Option B, SCT stage
      if( track->getNCoords() == 8 ) {
	tmp->_ftk_bitmask = ((tmp->_ftk_bitmask)<<6);
      }
      // Option B, pixel+SCTTrk stage
      else if( track->getNCoords() == 9 ) { // 3*2 pix + 1*3 SCTtrk
	tmp->_ftk_bitmask = ((tmp->_ftk_bitmask)&0x3f);
      }
      // Option A, 7L
      else if( track->getNCoords() == 10 ) { // 3*2 pix + 4 SCT
	tmp->_ftk_bitmask = ((tmp->_ftk_bitmask)&0x3f);
	for(int i = 6; i < 10; i++) {
	  tmp->_ftk_bitmask |= ((track->getBitmask()&(1<<i))<<(i-6));
	}
      }
      tmp->_ftk_bank = _bank;
      tmp->_ftk_bank_id = track->getBankID();
      tmp->_ftk_road_id = track->getRoadID();
      tmp->_ftk_bank_track_id = track->getTrackID();
      tmp->_ftk_hf_rejected = track->getHFRejected();
      tmp->_ftk_hw_rejected = track->getHWRejected();
      tmp->_ftk_hw_track_id = track->getHWTrackID();
      tmp->_nhits = detector::ftkNumLayersHit(tmp->_ftk_bitmask);
      tmp->_n_b_layer_hits = static_cast<unsigned int>(detector::ftkBLayerHit(tmp->_ftk_bitmask));
      tmp->_n_pixel_hits = detector::ftkNumLayersHitPixels(tmp->_ftk_bitmask);
      tmp->_n_sct_hits = detector::ftkNumLayersHitSCT(tmp->_ftk_bitmask);
      tmp->_nmisses = track->getNMissing();
      tmp->_n_pixel_holes = detector::ftkNumLayersMissedPixels(tmp->_ftk_bitmask);
      tmp->_n_sct_holes = detector::ftkNumLayersMissedSCT(tmp->_ftk_bitmask);
      if( track->getNCoords() == 8 ) { // SCT-only
	//tmp->_nmisses = tmp->_nmisses - 3;
	tmp->_n_pixel_holes = 0;
      // } else if( track->getNCoords() == 9 ) { // 3*2 pix + 1*3 SCTtrk
      // 	tmp->_nmisses = tmp->_nmisses - 8;
      } else if( track->getNCoords() == 10 ) { // 3*2 pix + 1*4 SCT (7L config with SCT r-phi only)
	//tmp->_nmisses = tmp->_nmisses - 4;
	tmp->_n_sct_holes -= 4;
      }
      tmp->_chi2 = track->getChi2();
      tmp->_origchi2 = track->getOrigChi2();
      tmp->_ndof =  static_cast<unsigned int>(std::max( static_cast<int>( track->getNCoords()-track->getNMissing()-5 ), 0 ));
      tmp->_barcode = UniqueBarcode(track->getEventIndex(),track->getBarcode());
      tmp->_barcode_matching_fraction = track->getBarcodeFrac();
      tmp->_ftk_coords.clear();
      tmp->_ftk_coords.assign( const_cast<FTKTrack*>(track)->getCoords() , const_cast<FTKTrack*>(track)->getCoords() + const_cast<FTKTrack*>(track)->getNCoords() );
      if( incomplete ) _ftk_tracks_incomplete.push_back( tmp );
      else _ftk_tracks.push_back( tmp );
      //     cout << " AnaTrack (FTK)    :    " 
      //          << " pt: " << setw(10) << tmp->pt0()
      //          << " c0: " << setw(10) << tmp->c0()
      //          << " p0: " << setw(10) << tmp->phi0()
      //          << " d0: " << setw(10) << tmp->d0()
      //          << " z0: " << setw(10) << tmp->z0()
      //          << " ct: " << setw(10) << tmp->ct0()
      //          << endl;
    } // end for each FTKTrack in stream
  }
  return true;
}

const unsigned long
NtFitter::count_events( const std::string& filename )
{
  TChain chain( "ftkdata" );
  chain.SetDirectory( 0 );
  chain.Reset();
  chain.AddFile( ana_streams::root_dcap_filename(filename).c_str() );
  return( chain.GetEntries() );
}

#else // not HAVE_LIBFTKSIM

#include <iostream>
#include "TrigFTKAna/Trees/NtFitter.hpp"

using namespace std;

const bool
NtFitter::begin_file( const int& bank , const std::string& filename )
{
  cout << " support for FTK tracks ntuple not compiled." << endl;
  return false;
}

const bool
NtFitter::end_file()
{ 
  return false;
}

const bool
NtFitter::get_event( const unsigned long& ievent )
{
  return false;
}

const unsigned long
NtFitter::count_events( const std::string& filename )
{
  cout << " support for FTK tracks ntuple not compiled." << endl;
  return 0;
}

#endif // if else HAVE_LIBFTKSIM

const int NtFitter::MERGED_BANK = -2;

