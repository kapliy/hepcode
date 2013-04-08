#ifndef WRAP_STUDYCOMPOSITE_HH
#define WRAP_STUDYCOMPOSITE_HH


#include <cmath>
#include <string>
#include <vector>
#include <algorithm>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include "DataGraphics/DgStudy.hh"
#include "DataGraphics/DgCollection.hh"
#include "TrigFTKAna/AnaComposite.hpp"
#include "TrigFTKAna/StudyParticle.hpp"
#include "TrigFTKAna/Detector.hpp"

class
StudyComposite : public DataGraphics::DgStudy< boost::shared_ptr<const AnaComposite> >
{
public:
  typedef boost::shared_ptr<const AnaComposite>  type; // must be same type as the template argument for DgStudy.

  static void study( type const& particle ) {
    using namespace DataGraphics;
    using namespace boost;
    StudyParticle::study( particle );
    boost::shared_ptr<const AnaComposite> cop( dynamic_pointer_cast<const AnaComposite>(particle) );
    const unsigned int nprong = cop->num_constituents();
    dg::fillh( "n_prong_decay" , 5 , 0 , 5 , nprong );
    dg::fillh( "delta_phi" , 100 , -M_PI , M_PI , cop->max_delta_phi_opening() );
    dg::fillh( "delta_eta" , 60 , -6 , 6 , cop->max_delta_eta_opening() );
    dg::fillh( "delta_r" , 100 , -6 , 6 , cop->max_delta_r_opening() );
    // if two prong decay, make mass plots for various resonances
    if( nprong==2 ) {
      if( true ) { // make os/ss plots
        // check that all particles have charge
        bool charged = true;
        AnaParticle::ftype q = 1.;
        for( AnaComposite::const_iterator i=cop->begin_constituent(), f=cop->end_constituent(); i!=f; ++i ) {
          const boost::shared_ptr<const AnaParticle> p( *i );
          // for now, only consider electrons and muons (::charge is defined for both but not necessarily for others)
          if( p->type()!=AnaParticle::ELECTRON && p->type()!=AnaParticle::MUON ) { continue; }
          if( std::abs(p->charge())<0.001 ) { charged = false; break; }
          q *= p->charge();
        }
        // plots without cut on os/ss are made by StudyParticle.
        if( charged && q > 0. ) {
          dg::fillh( "m_ss" , 1000 , 0 , 1000 , particle->m() , "Mass (GeV/c^{2})" );
          // dg::fillh( "m_zregion_ss" , 80 , 81. , 101. , particle->m() , "Mass (GeV/c^{2})" );
          // dg::fillh( "m_upsilonregion_ss" , 100 , 5. , 15. , particle->m() , "Mass (GeV/c^{2})" );
          // dg::fillh( "m_jpsiregion_ss" , 40 , 2. , 4. , particle->m() , "Mass (GeV/c^{2})" );
        } else if( charged && q < 0. ) {
          dg::fillh( "m_os" , 1000 , 0 , 1000 , particle->m() , "Mass (GeV/c^{2})" );
          // dg::fillh( "m_zregion_os" , 80 , 81. , 101. , particle->m() , "Mass (GeV/c^{2})" );
          // dg::fillh( "m_upsilonregion_os" , 100 , 5. , 15. , particle->m() , "Mass (GeV/c^{2})" );
          // dg::fillh( "m_jpsiregion_os" , 40 , 2. , 4. , particle->m() , "Mass (GeV/c^{2})" );
        }
      } // end make os/ss plots
    } // end make 2-prong plots
    //
    // plots of leading and subleading constituents
    std::vector< boost::shared_ptr<const AnaParticle> > daughters( std::min(nprong,2u) ); // <=2 particles to sort
    std::partial_sort_copy( cop->begin_constituent() , cop->end_constituent() , 
                            daughters.begin() , daughters.end() , 
                            boost::bind(&AnaParticle::pt,_1) > boost::bind(&AnaParticle::pt,_2) );
    unsigned int npart=0;
    for( std::vector< boost::shared_ptr<const AnaParticle> >::const_iterator iparticle = daughters.begin(), 
           fparticle=daughters.end(); iparticle!=fparticle; ++iparticle, ++npart ) {
      switch( npart ) {
      case 0: dg::down( "const_leading_pt" , "leading pt constituent" ); break;
      case 1: dg::down( "const_subleading_pt" , "subleading pt constituent" ); break;
      }
      StudyParticle::study( *iparticle );
      dg::up();
    }
  } // end do study
protected:
  virtual void _study( type const& particle ) { // the second const is important---matches the base class.
    StudyComposite::study(particle);
    return;
  } // end study of each particle
public:
  StudyComposite( const std::string& name , const std::string& description ) 
    : DataGraphics::DgStudy<type>( name , description )
  {}
  virtual ~StudyComposite() {}
};

#endif // WRAP_STUDYCOMPOSITE_HH

