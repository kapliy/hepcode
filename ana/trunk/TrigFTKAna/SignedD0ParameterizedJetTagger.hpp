#ifndef WRAP_SIGNEDD0PARAMETERIZEDJETTAGGER_HH
#define WRAP_SIGNEDD0PARAMETERIZEDJETTAGGER_HH

#include <boost/shared_ptr.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/version.hpp>
#include "TrigFTKAna/JetTagger.hpp"
#include "TrigFTKAna/LikelihoodParameterization1D.hpp"
#include "TrigFTKAna/SignedD0JetTagger.hpp"

class AnaTrack;

class
SignedD0ParameterizedJetTagger : public JetTagger
{
public:
protected:
  LikelihoodParameterization1D _lik_d0_b;
  LikelihoodParameterization1D _lik_d0_notb;
  bool         _use_sign;
  bool         _use_significance;
  bool         _use_track_jet_direction;
protected:
  virtual const bool _train( const boost::shared_ptr<const AnaJet>& jet , const std::vector< boost::shared_ptr<const AnaTrack> >& tracks );
  virtual const double _tag( const boost::shared_ptr<const AnaJet>& jet , const std::vector< boost::shared_ptr<const AnaTrack> >& tracks ) const;
private:  
  friend class boost::serialization::access;
  template<typename archiveT>
  void serialize( archiveT& ar , const unsigned int version ) {
    ar & boost::serialization::base_object<JetTagger>(*this);    
    ar & _lik_d0_b;
    ar & _lik_d0_notb;
    ar & _use_sign;
    ar & _use_significance;
    ar & _use_track_jet_direction;
  }
public:
  SignedD0ParameterizedJetTagger();
  SignedD0ParameterizedJetTagger( DataGraphics::DgCollection* coll );
  virtual ~SignedD0ParameterizedJetTagger() {}

  SignedD0ParameterizedJetTagger* use_significance( const bool& yes );
  SignedD0ParameterizedJetTagger* use_sign( const bool& yes );
  SignedD0ParameterizedJetTagger* use_track_jet_direction( const bool& yes );
  
  void fit( const SignedD0JetTagger& tagger );
  
  const LikelihoodParameterization1D& b_d0_likelihood() const { return _lik_d0_b; }
  const LikelihoodParameterization1D& notb_d0_likelihood() const { return _lik_d0_notb; }
  
  virtual const bool save(const std::string&) { return true; }
  virtual const bool load(const std::string&) { return true; }


protected:
  friend class AnaTaggerMgr;
  virtual JetTagger* clone() const { return new SignedD0ParameterizedJetTagger(*this); }

};

BOOST_CLASS_VERSION( SignedD0ParameterizedJetTagger , 1 );

#endif // WRAP_SIGNEDD0PARAMETERIZEDJETTAGGER_HH
