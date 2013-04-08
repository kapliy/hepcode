
#include <vector>
#include <algorithm>
#include <ext/algorithm>
#include <functional>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include "TrigFTKAna/JetTagger.hpp"
#include "TrigFTKAna/AnaJet.hpp"
#include "TrigFTKAna/AnaTrack.hpp"

using namespace std;

template<typename candidateT>
class AndreaCentroidCalculator
{
 protected:
  double         _dr_cut;
  TLorentzVector _p;
  unsigned int   _n_cands;
 public:
  AndreaCentroidCalculator(const double dr_cut = 0.4) 
    : _p(0.,0.,0.,0.) 
    , _n_cands( 0u )
    , _dr_cut( dr_cut )
  {}
  virtual ~AndreaCentroidCalculator() {}
  const double eta() const { return _p.Eta(); }
  const double phi() const { return _p.Phi(); }
  const double pt() const { return _p.Pt(); }
  const unsigned int n_cands() const { return _n_cands; }
  const bool incorporate(  boost::shared_ptr<const candidateT>  const cand ) {
    assert( cand );
    if( cand->pt() < 0.5 ) { return false; }
    if( !_n_cands ) {
      ++_n_cands;
      _p.SetPtEtaPhiM( cand->pt() , cand->eta() , cand->phi0() , 0. );
      return true;
    }
    const double deta = detector::delta_eta( cand->eta() , static_cast<AnaTrack::ftype>(_p.Eta()) );
    const double dphi = detector::delta_phi( cand->phi0() , static_cast<AnaTrack::ftype>(_p.Phi()) );
    const double dr = sqrt( deta*deta + dphi*dphi );
    if( dr>_dr_cut ) { return false; }
    ++_n_cands;
    TLorentzVector q;
    q.SetPtEtaPhiM( cand->pt() , cand->eta() , cand->phi0() , 0. );
    _p += q;
  }
};

const bool
JetTagger::compute_jet_eta_phi( const boost::shared_ptr<const AnaJet>& jet , const std::vector< boost::shared_ptr<const AnaTrack> >& tracks ,
                                const JetCentroidOption& jet_centroid_option , double& eta , double& phi )
{
  // for now, just return (roi, cone) jet centroid or use track clustering algorithm.
  if( jet_centroid_option == JetTagger::NOMINAL_CENTROID ) {
    phi = jet->phi();
    eta = jet->eta();
    return true;
  } else if( jet_centroid_option == JetTagger::TRACK_CENTROID ) {
    vector< boost::shared_ptr<const AnaTrack> > pt_ordered_tracks( tracks );
    if( !__gnu_cxx::is_sorted( pt_ordered_tracks.begin() , pt_ordered_tracks.end() , boost::bind(&AnaTrack::pt,_1) < boost::bind(&AnaTrack::pt,_2) ) ) { 
      std::sort( pt_ordered_tracks.begin() , pt_ordered_tracks.end() , boost::bind(&AnaTrack::pt,_1) < boost::bind(&AnaTrack::pt,_2) );
    }
    AndreaCentroidCalculator<AnaTrack> calc;
    for( vector< boost::shared_ptr<const AnaTrack> >::reverse_iterator i=pt_ordered_tracks.rbegin(),f=pt_ordered_tracks.rend(); i!=f; ++i ) {
      const boost::shared_ptr<const AnaTrack>& track( *i );
      calc.incorporate( track );
    }
    if( !(calc.n_cands()) ) { return false; }
    phi = calc.phi();
    eta = calc.eta();
    return true;
  } else {
    assert( !"unhandled centroid type" );
  }
  return false;
}

