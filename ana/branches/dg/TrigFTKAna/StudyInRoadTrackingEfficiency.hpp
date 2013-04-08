#ifndef WRAP_STUDYINROADTRACKINGEFFICIENCY_HPP
#define WRAP_STUDYINROADTRACKINGEFFICIENCY_HPP

// "visitor"/"functor" to make standard tracking efficiency plots. pass pairings of
// a reference track and a bool that indicates whether or not that reference track has a match.

// See StudyTrack.hpp for usage instructions. The only difference for StudyTrackingEfficiency is that the
// input collection consists of std::pair<const AnaTrack*,bool> instead of simply const AnaTrack* objects,
// where the bool indiciates whether the efficiency numerator should be incremented or not.


#include <string>
#include <cmath>
#include <utility>
#include <boost/shared_ptr.hpp>
#include "DataGraphics/DgStudy.hh"
#include "DataGraphics/DgCollection.hh"
#include "TrigFTKAna/AnaEvent.hpp"
#include "TrigFTKAna/AnaTrack.hpp"

class
StudyInRoadTrackingEfficiency : public DataGraphics::DgStudy< std::pair< boost::shared_ptr<const AnaTrack> , bool > >
{
public:
  typedef std::pair< boost::shared_ptr<const AnaTrack> , bool > type; // must be same type as the template argument for DgStudy.
  // ok, ignore this gobbledigook. this is me wriggling through a really tight passageway.
  //  static boost::tuples::element<0,StudyTrackingEfficiency::type>::type const & (*getTrack)( StudyTrackingEfficiency::type::inherited const & );
protected:
  virtual void _study( type const& value ) { // the second const is important---matches the base class.
    using namespace DataGraphics;
    const boost::shared_ptr<const AnaTrack>& reference_track = value.first;//value.get<0>();
    const bool success = value.second; //value.get<1>();
    // fill some efficiency plots. the first just records an overall efficiency number.
    // the remaining filleff plots record the efficiency in bins of pt, curvature, etc. with
    // a syntax similar to the one-dimensional histogram fillh function.
    const std::string ref_name = reference_track->algorithm().name();
    dg::filleff( "eff" , success );
    dg::filleff( "eff_vs_pt" , 50 , 0 , 25. , 
                 reference_track->pt() , success , reference_track->algorithm().name() + " p_{T} (GeV)" );
    dg::filleff( "eff_vs_curv" , 100 , -0.5 , 0.5 , 
                 reference_track->curvature()*1000. , success , reference_track->algorithm().name() + " CURVATURE (1/GeV)" );
    dg::filleff( "eff_vs_d0" , 400 , -1 , 1 , 
                 reference_track->d0()/10. , success , reference_track->algorithm().name() + " d_{0} (cm)" );
    if( _event && _event->highest_sum_pt_vertex() ) { 
      dg::filleff( "eff_vs_d0corr" , 100 , -0.25 , 0.25 , 
                   reference_track->d0_corr(_event->highest_sum_pt_vertex()->x(),
                                            _event->highest_sum_pt_vertex()->y(),
                                            _event->highest_sum_pt_vertex()->z())/10. ,
                   success , reference_track->algorithm().name() + " d_{0} (cm)" );
    }
//     if( _event ) {
//       dg::filleff( "eff_vs_event_number" , 10 , 0 , 1000 , success , _event->event_manager_index() );
//     }
    dg::filleff( "eff_vs_z0" , 100 , -15 , 15 , 
                 reference_track->z0()/10. , success , reference_track->algorithm().name() + " z_{0} (cm)" );
    dg::filleff( "eff_vs_eta" , 60 , -3 , 3 , 
                 reference_track->eta() , success , reference_track->algorithm().name() + " #eta" );
    dg::filleff( "eff_vs_eta_z0" , 13 , -2.5 , 2.5 , 
		 10, -12, 12, 
                 reference_track->eta() , reference_track->z0()/10.,
                 success , reference_track->algorithm().name() + " #eta",
		 reference_track->algorithm().name() + " z_{0} (cm)" );
    dg::filleff( "eff_vs_phi" , 48 , -M_PI , M_PI , 
                 reference_track->phi0() , success , reference_track->algorithm().name() + " #phi_{0} (rad)" );
    dg::filleff( "eff_vs_charge" , 3 , -1 , 1 ,
                 detector::sign_of(reference_track->curvature()) , success , reference_track->algorithm().name() + " CHARGE" );
    if( reference_track->is_truth() ) { 
      dg::filleff( "eff_vs_particle_type" , (detector::NPARTICLES*2)+1 , -detector::NPARTICLES , detector::NPARTICLES , 
                   detector::sign_of(reference_track->curvature())*detector::particle_type(reference_track->pdg_code()) , success , reference_track->algorithm().name() + " CHARGE" );
    }
    dg::filleff( "eff_vs_eta_vs_z0" , 12 , -12 , 12 , 10 , -2.5 , 2.5 , 
                 reference_track->z0()/10. , reference_track->eta() , success , 
                 reference_track->algorithm().name() + " #z_{0} (cm)" ,
                 reference_track->algorithm().name() + " #eta" );
    if( std::abs(reference_track->z0())/10. < 3. ) {
      dg::filleff( "eff_vs_eta_smallz0" , 60 , -3 , 3 , 
                   reference_track->eta() , success , reference_track->algorithm().name() + " #eta" );
    } else if( std::abs(reference_track->z0())/10. > 7. ) {
      dg::filleff( "eff_vs_eta_largez0" , 60 , -3 , 3 , 
                   reference_track->eta() , success , reference_track->algorithm().name() + " #eta" );
      dg::filleff( "eff_vs_eta_largez0_ss" , 60 , -3 , 3 , 
                   reference_track->eta()*detector::sign_of(reference_track->z0()) , success , reference_track->algorithm().name() + " #eta * sign(z_{0})" );
    }
    

    if( reference_track->is_ftk() ) {
      dg::filleff( "eff_vs_fittype" , 2 , -0.5 , 1.5 , 
                   (int)(reference_track->nhits()==detector::ftk_nplanes()) , success , 
                   reference_track->algorithm().name() + " Majority / Full Fit" );
      dg::filleff( "eff_vs_chi2" , 50 , 0 , 25. , 
                   reference_track->chi2() , success , reference_track->algorithm().name() + " #chi^{2}" );
      dg::filleff( "eff_vs_chi2prob" , 50 , 0 , 1. , 
                   reference_track->chi2prob() , success , reference_track->algorithm().name() + " p(#chi^{2},ndof)" );
      // if majority fit, get missing hit layer
      if( reference_track->nhits()<14 ) {
        int inner_missing_plane=999, outer_missing_plane=-1;
        for( int iplane=0, fplane=detector::ftk_nplanes(); iplane!=fplane; ++iplane ) {
          if( detector::ftkMissedPlane(reference_track->ftk_bitmask(),iplane) ) {
            inner_missing_plane = std::min( inner_missing_plane , iplane );
            outer_missing_plane = std::max( outer_missing_plane , iplane );
          }
        }
        dg::filleff( "eff_vs_innermost_missing_plane" , 15 , 0 , 15 , inner_missing_plane , success , 
                     reference_track->algorithm().name() + " Innermost Missing Plane" );
        dg::filleff( "eff_vs_outermost_missing_plane" , 15 , 0 , 15 , outer_missing_plane , success , 
                     reference_track->algorithm().name() + " Outermost Missing Plane" );
      }
    }

    dg::filleff( "begin_beampipe" , reference_track->begin_vertex_radius() < 27 && std::abs(reference_track->begin_vertex_z()) < 150. );
    if( reference_track->begin_vertex_radius() < 27 && std::abs(reference_track->begin_vertex_z()) < 150. ) { 
      dg::filleff( "eff_vs_eta_inbeampipe" , 60 , -3 , 3 , 
                   reference_track->eta() , success , reference_track->algorithm().name() + " #eta" );      
      dg::filleff( "intinpixels" , reference_track->end_vertex_radius() < 122.5 && reference_track->end_vertex_radius() > 50.5 );
      dg::filleff( "intinsct" , reference_track->end_vertex_radius() < 514 && reference_track->end_vertex_radius() > 299 );
      if( reference_track->end_vertex_radius() < 122.5 && reference_track->end_vertex_radius() > 50.5 ) {
        dg::filleff( "eff_vs_eta_intinpixels" , 60 , -3 , 3 , 
                     reference_track->eta() , success , reference_track->algorithm().name() + " #eta" );
      }
      if( reference_track->end_vertex_radius() < 514 && reference_track->end_vertex_radius() > 299 ) {
        dg::filleff( "eff_vs_eta_intinsct" , 60 , -3 , 3 , 
                     reference_track->eta() , success , reference_track->algorithm().name() + " #eta" );
      }
      if( reference_track->end_vertex_radius() > 122.5 ) {
        dg::filleff( "eff_vs_eta_nointpix" , 60 , -3 , 3 , 
                     reference_track->eta() , success , reference_track->algorithm().name() + " #eta" );
      }
      if( reference_track->end_vertex_radius() > 514 ) {
        dg::filleff( "eff_vs_eta_nointsctpix" , 60 , -3 , 3 , 
                     reference_track->eta() , success , reference_track->algorithm().name() + " #eta" );
      }
      if( reference_track->end_vertex_radius() < 514 ) {
        dg::filleff( "eff_vs_eta_intbsctout" , 60 , -3 , 3 , 
                     reference_track->eta() , success , reference_track->algorithm().name() + " #eta" );
      }
      if( reference_track->end_vertex_radius() < 299 ) {
        dg::filleff( "eff_vs_eta_intbsct" , 60 , -3 , 3 , 
                     reference_track->eta() , success , reference_track->algorithm().name() + " #eta" );
      }
      if( reference_track->end_vertex_radius() < 50.5 ) {
        dg::filleff( "eff_vs_eta_intbpix" , 60 , -3 , 3 , 
                     reference_track->eta() , success , reference_track->algorithm().name() + " #eta" );
      }
    } // end if track origin is in beampipe

    if (_match_type) { 
      // fill plot to take track of the relative composition of the road matched to this track
      for (unsigned ibit=0;ibit!=4;++ibit) { // bit loop
	if (_match_type&(1<<ibit)) dg::fillh("match_type",4,0,4,ibit,"Match types");
      }
    }
  } // end study of each track
private:
  boost::shared_ptr<const AnaEvent> _event;
  bool _is_fake_rate;
  unsigned _match_type; // bitmask with the type of match
public:
  StudyInRoadTrackingEfficiency( const std::string& name , const std::string& description ) 
    : DataGraphics::DgStudy<type>(name,description)
    , _event()
    , _is_fake_rate( false )
  {}
  virtual ~StudyInRoadTrackingEfficiency() {}
  void set_fake_rate( const bool& ok ) { _is_fake_rate = ok; }
  void set_match_type(const unsigned &match_type) { _match_type = match_type; }
  void for_event( const boost::shared_ptr<const AnaEvent>& event ) { _event = event; }
};

#endif // WRAP_STUDYINROADTRACKINGEFFICIENCY_HH

