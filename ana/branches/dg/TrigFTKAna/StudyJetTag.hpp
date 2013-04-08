#ifndef WRAP_STUDYJETTAG_HH
#define WRAP_STUDYJETTAG_HH

// "visitor"/"functor" to make standard jet tagging plots.

#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include "DataGraphics/DgStudy.hh"
#include "DataGraphics/DgCollection.hh"
#include "TrigFTKAna/AnaTrack.hpp"
#include "TrigFTKAna/AnaJet.hpp"
#include "TrigFTKAna/Likelihood1D.hpp"
#include "TrigFTKAna/Detector.hpp"

class AnaVertexFit;

class
JetTrackTag
{
private:
  boost::shared_ptr<const AnaJet>  _jet;
  const AnaTrack::ftype _jet_result;
  const AnaJet::ftype _jet_eta;
  const AnaJet::ftype _jet_phi;
  const std::vector< boost::shared_ptr<const AnaTrack> >& _tracks;
  const std::vector<AnaTrack::ftype>& _lik_b;
  const std::vector<AnaTrack::ftype>& _lik_notb;
  boost::shared_ptr<const AnaVertexFit>  _vertex_fit;
public:
  JetTrackTag( const boost::shared_ptr<const AnaJet>& jet , 
               const AnaTrack::ftype jet_result , 
               const AnaJet::ftype jet_eta ,
               const AnaJet::ftype jet_phi ,
               const std::vector< boost::shared_ptr<const AnaTrack> >& tracks ,
               const std::vector<AnaTrack::ftype>& b_lik ,
               const std::vector<AnaTrack::ftype>& notb_lik )
    : _jet( jet )
    , _jet_result( jet_result )
    , _jet_eta( jet_eta )
    , _jet_phi( jet_phi )
    , _tracks( tracks )
    , _lik_b( b_lik )
    , _lik_notb( notb_lik )
    , _vertex_fit()
  {
    assert( b_lik.size() == tracks.size() );
    assert( notb_lik.size() == tracks.size() );
  }
  const boost::shared_ptr<const AnaJet>& jet() const { return _jet; }
  const AnaJet::ftype jet_eta() const { return _jet_eta; }
  const AnaJet::ftype jet_phi() const { return _jet_phi; }
  const AnaTrack::ftype jet_result() const { return _jet_result; }
  const unsigned int ntracks() const { return _tracks.size(); }
  const std::vector< boost::shared_ptr<const AnaTrack> >& tracks() const { return _tracks; }
  const boost::shared_ptr<const AnaTrack>& track( const unsigned int& i ) const { assert( i<ntracks() ); return _tracks[i]; }
  const AnaTrack::ftype lik_b( const unsigned int& i ) const { assert( i<ntracks() ); return _lik_b[i]; }
  const AnaTrack::ftype lik_notb( const unsigned int& i ) const { assert( i<ntracks() ); return _lik_notb[i]; }
  const boost::shared_ptr<const AnaVertexFit>& vertex_fit() const { return _vertex_fit; }
};

class
StudyJetTag : public DataGraphics::DgStudy<JetTrackTag>
{
public:
  typedef JetTrackTag type; // must be same type as the template argument for DgStudy.
protected:
  virtual void _study( type const& tag );
  virtual void _study_jet( type const& tag );
  virtual void _study_tracks( type const& tag );
  virtual void _study_resolutions( type const& tag );
  virtual void _study_tag( type const& tag );
public:
  StudyJetTag( const std::string& name , const std::string& description ) 
    : DataGraphics::DgStudy<type>(name,description)
  {}
  virtual ~StudyJetTag() {}
};

#endif // WRAP_STUDYJETTAG_HH

