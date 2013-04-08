 
#ifndef SERIALIZE_TLORENTZVECTOR_HPP
#define SERIALIZE_TLORENTZVECTOR_HPP

#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>
#include <TLorentzVector.h>

namespace boost {
  namespace serialization {
    template<class archiveT>
    void serialize( archiveT & ar , TLorentzVector& p , const unsigned int version ) {
      double pt=0.;
      double eta=0.;
      double phi=0.;
      double m=0.;
      // check components for NaN. 
      const bool found_nan = ((p.E()!=p.E()) || (p.Pz()!=p.Pz()) || (p.Px()!=p.Px()) || (p.Py()!=p.Py()));
      if( (p.Pt()!=0. || p.M()!=0.) && !found_nan ) {
        // wrap this in the if statement to avoid error messages like
        // "Warning in <TVector3::PseudoRapidity>: transvers momentum
        // = 0! return +/- 10e10" when calling p.Eta().  also avoid
        // cases where one of the components is not-a-number. nans can
        // cause hard-to-trace problems in the serialization library.
        pt = p.Pt();
        eta = p.Eta();
        phi = p.Phi();
        m = p.M();
      }
      ar & boost::serialization::make_nvp("pt",pt);
      ar & boost::serialization::make_nvp("eta",eta);
      ar & boost::serialization::make_nvp("phi",phi);
      ar & boost::serialization::make_nvp("m",m);
      p.SetPtEtaPhiM( pt , eta , phi , m );
    }
  } // namespace serialization
} // namespace boost

#endif // SERIALIZE_TLORENTZVECTOR_HPP
