#ifndef WRAP_ANAVERTEXFIT_HPP
#define WRAP_ANAVERTEXFIT_HPP

// ANAVERTEXFIT
// ==================================================================
// Data for a track vertex.  CTVMFT interface based in part on Athena
// Reconstruction/Vertexing/CtvmftVertexFit/src/CtvmftVertexFit.cxx
// ==================================================================
// 2009-10-24 Antonio Boveia (boveia@hep.uchicago.edu)

#include <list>
#include <numeric>
#include <algorithm>
#include <iterator>
#include <cmath>
#include <string>
#include <vector>
#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/format.hpp>
#include <TLorentzVector.h>
#include <TVector.h>
#include <TMatrix.h>
#include "TrigFTKAna/Detector.hpp"

class AnaTrack;

class
AnaVertexFit
{

public:

  typedef float ftype;
  typedef std::list< boost::shared_ptr<const AnaTrack> > TrackColl;
  typedef std::list<bool> TrackStatusColl;
  typedef std::list<TLorentzVector> MomentumColl;
  typedef std::list<ftype> DeltaChi2Coll;

protected:

  TrackColl _tracks;
  TrackStatusColl _track_statuses;

  // fit results
  bool _complete_fit;
  ftype _x; // cm
  ftype _y; // cm
  ftype _z; // cm
  ftype _cov_xx; // cm
  ftype _cov_xy; // cm
  ftype _cov_xz; // cm
  ftype _cov_yy; // cm
  ftype _cov_yz; // cm
  ftype _cov_zz; // cm
  ftype _chi2;
  unsigned int _ndof;
  MomentumColl _momenta; // gev
  DeltaChi2Coll _delta_chi2s;
  TLorentzVector _sum_momenta; // gev
  
protected:

  // void _set_result( CtvmftWrapper& ctvmft ) {
  //   const int ivtx = 1;
  //   const int ivtxoffset = ctvmft.getVoff( ivtx-1 ); 
  //   _x = ctvmft.getXyzvrt(ivtx,0);
  //   _y = ctvmft.getXyzvrt(ivtx,1);
  //   _z = ctvmft.getXyzvrt(ivtx,2);
  //   _cov_xx = ctvmft.getVmat( ivtxoffset , ivtxoffset );
  //   _cov_xy = ctvmft.getVmat( ivtxoffset , ivtxoffset+1 );
  //   _cov_xz = ctvmft.getVmat( ivtxoffset , ivtxoffset+2 );
  //   _cov_yy = ctvmft.getVmat( ivtxoffset+1 , ivtxoffset+1 );
  //   _cov_yz = ctvmft.getVmat( ivtxoffset+1 , ivtxoffset+2 );
  //   _cov_zz = ctvmft.getVmat( ivtxoffset+2 , ivtxoffset+2 );
  //   _chi2 = ctvmft.getChiv(ivtx);
  //   _ndof = ctvmft.getNdof();
  //   unsigned int ntrk = 0;
  //   _momenta.clear();
  //   _delta_chi2s.clear();
  //   _sum_momenta.SetPxPyPzE( 0 , 0 , 0 , 0 );
  //   for( TrackColl::const_iterator itrk=_tracks.begin(), ftrk=_tracks.end(); itrk!=ftrk; ++itrk, ++ntrk ) {
  //     const ftype px( ctvmft.getTrkp4(0,ntrk) );
  //     const ftype py( ctvmft.getTrkp4(1,ntrk) );
  //     const ftype pz( ctvmft.getTrkp4(2,ntrk) );
  //     const ftype en( ctvmft.getTrkp4(3,ntrk) );
  //     _momenta.push_back( TLorentzVector( px , py , pz , en) );
  //     _sum_momenta.SetPx( _sum_momenta.Px() + px );
  //     _sum_momenta.SetPy( _sum_momenta.Py() + py );
  //     _sum_momenta.SetPz( _sum_momenta.Pz() + pz );
  //     _sum_momenta.SetE( _sum_momenta.E() + en );
  //     // double charge = (ctvmft.getPar(indexTrk,0) <= 0) ? 1 : -1; 
  //     _delta_chi2s.push_back( ctvmft.getChit(ntrk) );
  //   }
  //   assert( _momenta.size() == ntracks() );
  //   assert( _delta_chi2s.size() == ntracks() );
  //   _complete_fit = true;
  // } // end get fit info

  void _add( const boost::shared_ptr<const AnaTrack>& track , const int& ivert , const int& itrk );
  void _print( const int& stat , std::ostream& os ) const;
  void _print_error( const int& stat , std::ostream& os ) const;

public:

  AnaVertexFit()
    : _tracks()
    , _complete_fit( false )
    , _x( std::numeric_limits<ftype>::max() )
    , _y( std::numeric_limits<ftype>::max() )
    , _z( std::numeric_limits<ftype>::max() )
    , _cov_xx( std::numeric_limits<ftype>::max() )
    , _cov_xy( std::numeric_limits<ftype>::max() )
    , _cov_xz( std::numeric_limits<ftype>::max() )
    , _cov_yy( std::numeric_limits<ftype>::max() )
    , _cov_yz( std::numeric_limits<ftype>::max() )
    , _cov_zz( std::numeric_limits<ftype>::max() )
    , _chi2( std::numeric_limits<ftype>::max() )
    , _ndof( 0 )
    , _momenta()
    , _delta_chi2s()
    , _sum_momenta( 0 , 0 , 0 , 0 )
  {}
  virtual ~AnaVertexFit() {}

  const TrackColl& tracks() const { return _tracks; }
  void add( const boost::shared_ptr<const AnaTrack>& track ) { 
    if( track ) { 
      _tracks.push_back( track ); 
      _track_statuses.push_back( true );
    } 
    _complete_fit = false;
  }
  template< typename collIterT >
  void add( collIterT begin , collIterT end ) {
    for( collIterT i=begin; i!=end; ++i ) {
      boost::shared_ptr<const AnaTrack> track( *i ); assert(track);
      _tracks.push_back( track );
      _track_statuses.push_back( true );
    }
    _complete_fit = false;
  }
  void remove( boost::shared_ptr<const AnaTrack> track ) {
    if( !track ) { return; }
    TrackColl::iterator i = std::find( _tracks.begin() , _tracks.end() , track );
    if( i == _tracks.end() ) { return; }
    unsigned int n = std::distance( _tracks.begin() , i );
    _tracks.erase( i );
    TrackStatusColl::iterator istat = _track_statuses.begin();
    std::advance( istat , n );
    _track_statuses.erase( istat );
    _complete_fit = false;
  }
  const unsigned int ntracks() const { return _tracks.size(); }
  const unsigned int multiplicity() const { return ntracks(); }
  const unsigned int mult() const { return ntracks(); }
  const boost::shared_ptr<const AnaTrack>& track( const unsigned int& n ) const { 
    assert(n<ntracks()); 
    TrackColl::const_iterator itrk = _tracks.begin();
    std::advance( itrk , n );
    return *itrk;
  }
  const bool track_status( const unsigned int& n ) const { 
    assert(n<ntracks()); 
    TrackStatusColl::const_iterator itrk = _track_statuses.begin();
    std::advance( itrk , n );
    return *itrk;
  }
  void track_status( const unsigned int& n , const bool& yes ) { 
    assert(n<ntracks()); 
    TrackStatusColl::iterator itrk = _track_statuses.begin();
    std::advance( itrk , n );
    (*itrk) = yes;
  }
  void track_status_all( const bool& yes ) { 
    for( TrackStatusColl::iterator itrk=_track_statuses.begin(), ftrk=_track_statuses.end(); itrk!=ftrk; ++itrk ) {
      (*itrk) = yes;
    }
  }

  // const bool fit() {
  // CtvmftWrapper ctvmft;
  // ctvmft.setParameters();
  // ctvmft.setTracks( _tracks.begin() , _tracks.end() );
  // ctvmft.print();
  // int fitPrintUnit = 6;
  // int fitPrintLevel = 1; 
  // int fitResult;
  // ctvmft.fit( fitPrintUnit , fitPrintLevel , fitResult );
  // _complete_fit = false; // cleared to true by _set_result if that is successful.
  // if( fitResult!=0 ) { _set_result( ctvmft ); }
  // return( _complete_fit );
  //   return false;
  // }
  const bool fit();

  const bool is_fitted() const { return _complete_fit; }
  void assert_fitted() const { assert(_complete_fit && "check AnaVertexFit::is_fitted()!"); }
  const AnaVertexFit::ftype x() const { assert_fitted(); return _x; }
  const AnaVertexFit::ftype y() const { assert_fitted(); return _y; }
  const AnaVertexFit::ftype z() const { assert_fitted(); return _z; }
  const AnaVertexFit::ftype perp() const { assert_fitted(); return detector::quadrature( x() , y() ); }
  const AnaVertexFit::ftype perp_wrt( const AnaVertexFit::ftype& vx , const AnaVertexFit::ftype& vy ) const { assert_fitted(); return detector::quadrature( x()-vx , y()-vy ); }
  const AnaVertexFit::ftype mag() const { assert_fitted(); return detector::quadrature( x() , y() , z() ); }
  const AnaVertexFit::ftype mag_wrt( const AnaVertexFit::ftype& vx , const AnaVertexFit::ftype& vy , const AnaVertexFit::ftype& vz ) const { 
    assert_fitted(); 
    return detector::quadrature( x()-vx , y()-vy , z()-vz );
  }
  const AnaVertexFit::ftype signed_perp( const AnaVertexFit::ftype& phi_dir ) const { assert_fitted(); return detector::directionalD0(perp(),static_cast<AnaVertexFit::ftype>(atan2(_y,_x)),phi_dir); }
  const AnaVertexFit::ftype signed_perp( const AnaVertexFit::ftype& vx, const AnaVertexFit::ftype& vy , const AnaVertexFit::ftype& phi_dir ) const { 
    assert_fitted(); return detector::directionalD0(perp_wrt(vx,vy),static_cast<AnaVertexFit::ftype>(atan2(_y-vy,_x-vx)),phi_dir); 
  }
  const AnaVertexFit::ftype perp_significance( const TVector& pv , const TMatrix& cov_pv ) const;
  const AnaVertexFit::ftype mag_significance( const TVector& pv , const TMatrix& cov_pv ) const;
  const AnaVertexFit::ftype cov_xx() const { assert_fitted(); return _cov_xx; }
  const AnaVertexFit::ftype cov_xy() const { assert_fitted(); return _cov_xy; }
  const AnaVertexFit::ftype cov_xz() const { assert_fitted(); return _cov_xz; }
  const AnaVertexFit::ftype cov_yy() const { assert_fitted(); return _cov_yy; }
  const AnaVertexFit::ftype cov_yz() const { assert_fitted(); return _cov_yz; }
  const AnaVertexFit::ftype cov_zz() const { assert_fitted(); return _cov_zz; }
  const AnaVertexFit::ftype chi2() const { assert_fitted(); return _chi2; }
  const AnaVertexFit::ftype ndof() const { assert_fitted(); return _ndof; }
  const AnaVertexFit::ftype chi2ndof() const { assert_fitted(); return detector::chi2ndof(_chi2,_ndof); }
  const AnaVertexFit::ftype chi2prob() const { assert_fitted(); return detector::chi2prob(_chi2,_ndof); }
  const MomentumColl& track_momenta() const { assert_fitted(); return _momenta; }
  const DeltaChi2Coll& track_delta_chi2s() const { assert_fitted(); return _delta_chi2s; }
  const TLorentzVector& vertex_momentum() const { assert_fitted(); return _sum_momenta; }
  const TLorentzVector& track_momentum( const unsigned int& n ) const {
    assert_fitted();
    assert(n<ntracks());
    MomentumColl::const_iterator i = _momenta.begin();
    std::advance( i , n );
    return *i;
  }
  const AnaVertexFit::ftype track_delta_chi2( const unsigned int& n ) const {
    assert_fitted();
    assert(n<ntracks());
    DeltaChi2Coll::const_iterator i = _delta_chi2s.begin();
    std::advance( i , n );
    return *i;
  }
  

  // template< typename collIterT >
  typedef std::vector< boost::shared_ptr<const AnaTrack> >::const_iterator collIterT;
  static const bool compute_sv_variables( collIterT begin , collIterT end , 
                                          AnaVertexFit::ftype& vertex_mass , 
                                          AnaVertexFit::ftype& vertex_pt_ratio ,
                                          unsigned int& n_two_track_vertices , AnaVertexFit* result = 0 );
  
};

#endif // WRAP_ANAVERTEXFIT_HPP
