#include <vector>
#include <algorithm>
#include <boost/shared_ptr.hpp>
#include <boost/scope_exit.hpp>
#include <boost/bind.hpp>
#include <boost/format.hpp>
#include <TrigFTKAna/StudyTrackComparison.hpp>
#include <TrigFTKAna/AnaTrack.hpp>
#include <TrigFTKAna/AnaSiCluster.hpp>
#include <TrigFTKAna/StudySiCluster.hpp>
#include <DataGraphics/DataGraphics.hh>
#include <TrigFTKAna/AnaEventMgr.hpp>
#include <TrigFTKAna/AnaConfiguration.hpp>

using namespace std;
using namespace DataGraphics;
using boost::bind;
using boost::shared_ptr;
using boost::format;

void
StudyTrackComparison::_cluster_study( const shared_ptr<const AnaTrack>& comparison_track , 
                                      const shared_ptr<const AnaTrack>& reference_track )
{ 
  // skip events without cluster information for both tracks
  if( comparison_track->si_clusters().empty() || reference_track->si_clusters().empty() ) { return; }

  bool dump_cluster_data = false;

  shared_ptr<const AnaTrack> truth_track;
  if( true ) { // look for truth match
    vector< shared_ptr<const AnaTrack> > matches;
    std::remove_copy_if( AnaEventMgr::current_event()->truth_tracks().begin() , 
                         AnaEventMgr::current_event()->truth_tracks().end() , 
                         back_inserter(matches) , 
                         bind(&AnaTrack::barcode,_1) != reference_track->barcode()
                         || bind(&AnaTrack::barcode_matching_fraction,_1) < AnaConfiguration::barcode_matching_minimum_cut() );
    if( !matches.empty() ) { 
      vector< shared_ptr<const AnaTrack> >::const_iterator i = 
        std::max_element( matches.begin() , matches.end() ,
                          bind(&AnaTrack::barcode_matching_fraction,_1) < 
                          bind(&AnaTrack::barcode_matching_fraction,_2) );
      assert( i != matches.end() );
      truth_track = *i;
    }
  }

  const std::vector< shared_ptr<const AnaSiCluster> >& clusters( comparison_track->si_clusters() );

  if( true ) { 
    StudySiCluster stclu( "clusters_offline_wrt_ftk" , "silicon clusters: offline wrt FTK" );
    stclu.reference_track( reference_track );
    stclu.comparison_track( comparison_track );
    stclu.reference_clusters( &(reference_track->si_clusters()) );
    stclu.add_category( "pixels" , bind( &AnaSiCluster::is_pixel , _1 ) == true );
    stclu.add_category( "sct" , bind( &AnaSiCluster::is_pixel , _1 ) == false );
    // stclu.add_category( "pixels_single_x" , bind( &AnaSiCluster::width_x , _1 ) == 1 && bind( &AnaSiCluster::is_pixel , _1 ) == true );
    // stclu.add_category( "pixels_single_z" , bind( &AnaSiCluster::width_z , _1 ) == 1 && bind( &AnaSiCluster::is_pixel , _1 ) == true );
    stclu.add_category( "strips_axial" , bind( &AnaSiCluster::is_sct_phi , _1 ) == true );
    stclu.add_category( "strips_stereo" , bind( &AnaSiCluster::is_sct_sas , _1 ) == true );
    stclu.add_category( "ftk_layer_0" , bind( &AnaSiCluster::ftk_logical_layer , _1 ) == 0 );
    stclu.add_category( "ftk_layer_1" , bind( &AnaSiCluster::ftk_logical_layer , _1 ) == 1 );
    stclu.add_category( "ftk_layer_2" , bind( &AnaSiCluster::ftk_logical_layer , _1 ) == 2 );
    stclu.add_category( "ftk_layer_3" , bind( &AnaSiCluster::ftk_logical_layer , _1 ) == 3 );
    stclu.add_category( "ftk_layer_4" , bind( &AnaSiCluster::ftk_logical_layer , _1 ) == 4 );
    stclu.add_category( "ftk_layer_5" , bind( &AnaSiCluster::ftk_logical_layer , _1 ) == 5 );
    stclu.add_category( "ftk_layer_6" , bind( &AnaSiCluster::ftk_logical_layer , _1 ) == 6 );
    stclu.add_category( "ftk_layer_7" , bind( &AnaSiCluster::ftk_logical_layer , _1 ) == 7 );
    stclu.add_category( "ftk_layer_8" , bind( &AnaSiCluster::ftk_logical_layer , _1 ) == 8 );
    stclu.add_category( "ftk_layer_9" , bind( &AnaSiCluster::ftk_logical_layer , _1 ) == 9 );
    stclu.add_category( "ftk_layer_10" , bind( &AnaSiCluster::ftk_logical_layer , _1 ) == 10 );
    // stclu.add_category( "strips_single" , bind( &AnaSiCluster::width_x , _1 ) == 1 && bind( &AnaSiCluster::is_pixel , _1 ) == false );
    stclu.for_each( clusters.begin() , clusters.end() );
  }

  if( true ) { 
    StudySiCluster stoff( "clusters_ftk_wrt_offline" , "silicon clusters: FTK wrt offline" );
    stoff.reference_track( comparison_track );
    stoff.comparison_track( reference_track );
    stoff.reference_clusters( &(comparison_track->si_clusters()) );
    const unsigned int nll = comparison_track->si_clusters_logical_layers_present();
    const unsigned int nll_sct = comparison_track->si_clusters_sct_logical_layers_present();
    stoff.add_constant_category( "ref_all_layers" , nll==11 );
    stoff.add_constant_category( "ref_missing_1_sct" , nll_sct==7 );
    stoff.add_constant_category( "ref_missing_2_sct" , nll_sct==6 );
    stoff.add_category( "pixels" , bind( &AnaSiCluster::is_pixel , _1 ) == true );
    stoff.add_category( "sct" , bind( &AnaSiCluster::is_pixel , _1 ) == false );
    stoff.add_category( "strips_axial" , bind( &AnaSiCluster::is_sct_phi , _1 ) == true );
    stoff.add_category( "strips_stereo" , bind( &AnaSiCluster::is_sct_sas , _1 ) == true );
    stoff.add_category( "ftk_layer_0" , bind( &AnaSiCluster::ftk_logical_layer , _1 ) == 0 );
    stoff.add_category( "ftk_layer_1" , bind( &AnaSiCluster::ftk_logical_layer , _1 ) == 1 );
    stoff.add_category( "ftk_layer_2" , bind( &AnaSiCluster::ftk_logical_layer , _1 ) == 2 );
    stoff.add_category( "ftk_layer_3" , bind( &AnaSiCluster::ftk_logical_layer , _1 ) == 3 );
    stoff.add_category( "ftk_layer_4" , bind( &AnaSiCluster::ftk_logical_layer , _1 ) == 4 );
    stoff.add_category( "ftk_layer_5" , bind( &AnaSiCluster::ftk_logical_layer , _1 ) == 5 );
    stoff.add_category( "ftk_layer_6" , bind( &AnaSiCluster::ftk_logical_layer , _1 ) == 6 );
    stoff.add_category( "ftk_layer_7" , bind( &AnaSiCluster::ftk_logical_layer , _1 ) == 7 );
    stoff.add_category( "ftk_layer_8" , bind( &AnaSiCluster::ftk_logical_layer , _1 ) == 8 );
    stoff.add_category( "ftk_layer_9" , bind( &AnaSiCluster::ftk_logical_layer , _1 ) == 9 );
    stoff.add_category( "ftk_layer_10" , bind( &AnaSiCluster::ftk_logical_layer , _1 ) == 10 );
    stoff.for_each( reference_track->si_clusters().begin() , reference_track->si_clusters().end() );
  }

  // clustering efficiencies. rates at which no comparison cluster is
  // attached when reference has one, and vice versa. when cluster is
  // completely missing from one, where is the cluster for the other?
  // what is its width? what are the resolutions of the tracks which do have the cluster?
  if( true ) {
    dg::down( "clustering_efficiencies" , "clustering efficiencies" );
    BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
    for( unsigned int ftkll=0; ftkll!=11; ++ftkll ) { 
      dg::down( (format("ftk_layer_%d") % ftkll).str() ,
                "cluster comparison plots for layer" ); 
      BOOST_SCOPE_EXIT() { dg::up(); ; } BOOST_SCOPE_EXIT_END;
      
      vector< shared_ptr<const AnaSiCluster> > comparison_clusters;
      vector< shared_ptr<const AnaSiCluster> > reference_clusters;
      vector< shared_ptr<const AnaSiCluster> > all_offline_clusters;
      std::remove_copy_if( comparison_track->si_clusters().begin() ,
                           comparison_track->si_clusters().end() ,
                           back_inserter( comparison_clusters ) ,
                           bind(&AnaSiCluster::ftk_logical_layer,_1) != ftkll );
      std::remove_copy_if( reference_track->si_clusters().begin() ,
                           reference_track->si_clusters().end() ,
                           back_inserter( reference_clusters ) ,
                           bind(&AnaSiCluster::ftk_logical_layer,_1) != ftkll );
      std::remove_copy_if( AnaEventMgr::current_event()->si_clusters().begin() , 
                           AnaEventMgr::current_event()->si_clusters().end() , 
                           back_inserter(all_offline_clusters) ,
                           bind(&AnaSiCluster::algorithm,_1) != AnaSiCluster::OFFLINE ||
                           bind(&AnaSiCluster::ftk_logical_layer,_1) != ftkll );
      if( !reference_clusters.empty() ) { 
        dg::filleff( "comp_eff_wrt_ref" , !comparison_clusters.empty() );
        BOOST_FOREACH( shared_ptr<const AnaSiCluster>& clu , reference_clusters ) {
          dg::fillh( "ref_z_vs_x" ,
                     400 , 0 , 400 , 145 , 0 , 145 ,
                     clu->local_x() , clu->local_z() , "LOCAL X" , "LOCAL Z" );
          dg::fillh( "ref_x" , 768 , 0 , 768 , clu->local_x() , "LOCAL X" );
          dg::fillh( "ref_width_z_vs_x" ,
                     400 , 0 , 400 , 145 , 0 , 145 ,
                     clu->width_x() , clu->width_z() , "WIDTH X" , "WIDTH Z" );
        }
        if( comparison_clusters.empty() ) {
          BOOST_FOREACH( shared_ptr<const AnaSiCluster>& clu , reference_clusters ) {
            dg::fillh( "ref_missing_comp_z_vs_x" ,
                       400 , 0 , 400 , 145 , 0 , 145 ,
                       clu->local_x() , clu->local_z() , "LOCAL X" , "LOCAL Z" );
            dg::fillh( "ref_missing_comp_x" , 768 , 0 , 768 , clu->local_x() , "LOCAL X" );
            dg::fillh( "ref_missing_comp_width_z_vs_x" ,
                       400 , 0 , 400 , 145 , 0 , 145 ,
                       clu->width_x() , clu->width_z() , "WIDTH X" , "WIDTH Z" );
          }
          if( truth_track ) { 
            dg::down( "ref_res_vs_truth_no_comp_for_ref" , "resolutions" ); 
            BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;            
            _residual_study( truth_track , reference_track );
          }
        }
      } // end if reference cluster
      if( !all_offline_clusters.empty() ) { 
        dg::filleff( "comp_eff_wrt_all" , !comparison_clusters.empty() );
        if( comparison_clusters.empty() ) {
          BOOST_FOREACH( shared_ptr<const AnaSiCluster>& clu , all_offline_clusters ) {
            dg::fillh( "all_missing_comp_z_vs_x" ,
                       400 , 0 , 400 , 145 , 0 , 145 ,
                       clu->local_x() , clu->local_z() , "LOCAL X" , "LOCAL Z" );
            dg::fillh( "all_missing_comp_x" , 768 , 0 , 768 , clu->local_x() , "LOCAL X" );
            dg::fillh( "all_missing_comp_width_z_vs_x" ,
                       400 , 0 , 400 , 145 , 0 , 145 ,
                       clu->width_x() , clu->width_z() , "WIDTH X" , "WIDTH Z" );
          }
          if( truth_track ) { 
            dg::down( "ref_res_vs_truth_no_comp_for_all" , "resolutions" ); 
            BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;            
            _residual_study( truth_track , reference_track );
          }
        }
      } // end if all_offline cluster
      if( !comparison_clusters.empty() ) { 
        dg::filleff( "ref_eff_wrt_comp" , !reference_clusters.empty() );
        if( reference_clusters.empty() ) {
          BOOST_FOREACH( shared_ptr<const AnaSiCluster>& clu , comparison_clusters ) {
            dg::fillh( "comp_missing_ref_z_vs_x" ,
                       400 , 0 , 400 , 145 , 0 , 145 ,
                       clu->local_x() , clu->local_z() , "LOCAL X" , "LOCAL Z" );
            dg::fillh( "comp_missing_ref_x" , 768 , 0 , 768 , clu->local_x() , "LOCAL X" );
            dg::fillh( "comp_missing_ref_width_z_vs_x" ,
                       400 , 0 , 400 , 145 , 0 , 145 ,
                       clu->width_x() , clu->width_z() , "WIDTH X" , "WIDTH Z" );
          }
          if( truth_track ) { 
            dg::down( "comp_res_vs_truth_no_ref" , "resolutions" ); 
            BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;            
            _residual_study( truth_track , comparison_track );
          }
        }
      } // end if comparison cluster
      if( reference_clusters.empty() ) {
        dg::filleff( "comp_empty_when_ref_empty" , comparison_clusters.empty() );
        BOOST_FOREACH( shared_ptr<const AnaSiCluster>& clu , comparison_clusters ) {
          dg::fillh( "comp_for_ref_empty_z_vs_x" ,
                     400 , 0 , 400 , 145 , 0 , 145 ,
                     clu->local_x() , clu->local_z() , "LOCAL X" , "LOCAL Z" );
        }
      }
      if( all_offline_clusters.empty() ) {
        dg::filleff( "comp_empty_when_all_empty" , comparison_clusters.empty() );
        BOOST_FOREACH( shared_ptr<const AnaSiCluster>& clu , comparison_clusters ) {
          dg::fillh( "comp_for_all_empty_z_vs_x" ,
                     400 , 0 , 400 , 145 , 0 , 145 ,
                     clu->local_x() , clu->local_z() , "LOCAL X" , "LOCAL Z" );
        }
      }
      if( comparison_clusters.empty() ) {
        dg::filleff( "ref_empty_when_comp_empty" , reference_clusters.empty() );
        dg::filleff( "all_empty_when_comp_empty" , all_offline_clusters.empty() );
        BOOST_FOREACH( shared_ptr<const AnaSiCluster>& clu , reference_clusters ) {
          dg::fillh( "ref_for_comp_empty_z_vs_x" ,
                     400 , 0 , 400 , 145 , 0 , 145 ,
                     clu->local_x() , clu->local_z() , "LOCAL X" , "LOCAL Z" );
        }
        BOOST_FOREACH( shared_ptr<const AnaSiCluster>& clu , all_offline_clusters ) {
          dg::fillh( "all_for_comp_empty_z_vs_x" ,
                     400 , 0 , 400 , 145 , 0 , 145 ,
                     clu->local_x() , clu->local_z() , "LOCAL X" , "LOCAL Z" );
        }
      }
    }
  }

  // count fraction of tracks with at least one n-channel cluster.
  if( true && !reference_track->si_clusters().empty() ) {
    dg::down( "nchannel_clusters" , "cluster comparison plots" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
    unsigned int nw[6] = { 0 , 0 , 0 , 0 , 0 , 0 };
    BOOST_FOREACH( const shared_ptr<const AnaSiCluster>& clu , reference_track->si_clusters() ) {
      // fall through here on purpose so that case n-1,n-2,n-3 etc are also incremented.
      switch( std::min(clu->width_x(),6) ) {
      case 6: ++(nw[5]); // fall through to next case
      case 5: ++(nw[4]); // fall through to next case
      case 4: ++(nw[3]); // fall through to next case
      case 3: ++(nw[2]); // fall through to next case
      case 2: ++(nw[1]); // fall through to next case
      case 1: ++(nw[0]); 
        break;
      case 0: break; // strange but ignore it for now (FIXME!)
      default: assert( !"unhandled case" );
      }
      dg::fillh( "n_width_x" , 100 , 0 , 100 , clu->width_x() );
    }
    const unsigned int nhits = reference_track->si_clusters().size();
    dg::filleff( "has_a_6_channel_cluster" , nw[5]>0 );
    dg::filleff( "has_a_5_channel_cluster" , nw[4]>0 );
    dg::filleff( "has_a_4_channel_cluster" , nw[3]>0 );
    dg::filleff( "has_a_3_channel_cluster" , nw[2]>0 );
    dg::filleff( "has_a_2_channel_cluster" , nw[1]>0 );
    dg::filleff( "has_a_1_channel_cluster" , nw[0]>0 );
    dg::filleff( "has_multiple_6_channel_clusters" , nw[5]>1 );
    dg::filleff( "has_multiple_5_channel_clusters" , nw[4]>1 );
    dg::filleff( "has_multiple_4_channel_clusters" , nw[3]>1 );
    dg::filleff( "has_multiple_3_channel_clusters" , nw[2]>1 );
    dg::filleff( "has_multiple_2_channel_clusters" , nw[1]>1 );
    if( nw[1] == nhits ) { 
      dg::down( "res_when_all_1_channel_cluster" , "resolution comparison plots" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      _residual_study( truth_track , reference_track );
    }
    if( nw[1] > 0 ) { 
      dg::down( "res_when_a_2_channel_cluster" , "resolution comparison plots" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      _residual_study( truth_track , reference_track );
    }
    if( nw[3] > 0 ) { 
      dg::down( "res_when_a_4_channel_cluster" , "resolution comparison plots" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      _residual_study( truth_track , reference_track );
    }
    if( nw[2] > 1 ) { 
      dg::down( "res_when_many_3_channel_cluster" , "resolution comparison plots" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      _residual_study( truth_track , reference_track );
    }
  }

  // compare track si clusters with all offline clusters in the same logic layer from wrapper.
  if( true && !comparison_track->si_clusters().empty() ) {
    dg::down( "cluster_comparison_with_all_offline" , "cluster comparison plots" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
    const shared_ptr<const AnaEvent>& ev = AnaEventMgr::current_event(); assert( ev );
    if( !(ev->si_clusters().empty()) ) { 
      unsigned int nbad_pixel = 0u;
      unsigned int nbad_sct = 0u;
      unsigned int nbad_sct_extreme = 0u;
      unsigned int nbad_pixel_dx = 0u;
      unsigned int nbad_pixel_dz = 0u;
      unsigned int nbad_sct_dx = 0u;
      unsigned int nmultichannel_pixels = 0u;
      unsigned int nmultichannel_strips = 0u;
      for( std::vector< shared_ptr<const AnaSiCluster> >::const_iterator 
             icompclu=comparison_track->si_clusters().begin(), 
             fcompclu=comparison_track->si_clusters().end();
           icompclu!=fcompclu; ++icompclu ) {
        const shared_ptr<const AnaSiCluster>& comp_cluster( *icompclu );
        dg::down( comp_cluster->is_pixel() ? "pixel_clusters" : "sct_clusters" , 
                  "cluster comparison plots" ); 
        BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
        // look for matching event clusters on same logical layer
        vector< shared_ptr<const AnaSiCluster> > offline_clusters;
        std::remove_copy_if( ev->si_clusters().begin() , ev->si_clusters().end() , 
                             back_inserter(offline_clusters) ,
                             bind(&AnaSiCluster::algorithm,_1) != AnaSiCluster::OFFLINE ||
                             bind(&AnaSiCluster::ftk_logical_layer,_1) != comp_cluster->ftk_logical_layer() );
        // if SCT, look for matching event clusters on either side logical layer
        vector< shared_ptr<const AnaSiCluster> > offline_clusters_anyside;
        if( !(comp_cluster->is_pixel()) ) { 
          const int group_idA = (static_cast<int>((comp_cluster->ftk_logical_layer()-1)/2))*2 + 1;
          const int group_idB = group_idA + 1;
          BOOST_FOREACH( const shared_ptr<const AnaSiCluster>& offclu , ev->si_clusters() ) {
            if( offclu->ftk_logical_layer() != group_idA && 
                offclu->ftk_logical_layer() != group_idB ) { continue; }
            offline_clusters_anyside.push_back( offclu );
          }
        } // end if fill offline_clusters_anyside with SCT strips
        dg::fillh( "n_offline_clusters" , 10 , 0 , 10 , offline_clusters.size() );
        dg::fillh( "n_offline_clusters_vs_local_x" , 768 , 0 , 768 , 10 , 0 , 10 , 
                   comp_cluster->local_x() , offline_clusters.size() ,
                   "LOCAL X (READOUT CHANNELS)" , "NUMBER OF OFFLINE CLUSTERS" );
        if( comp_cluster->is_pixel() ) { 
          dg::fillh( "n_offline_clusters_vs_local_z" , 768 , 0 , 768 , 10 , 0 , 10 , 
                     comp_cluster->local_z() , offline_clusters.size() ,
                     "LOCAL Z (READOUT CHANNELS)" , "NUMBER OF OFFLINE CLUSTERS" );
        } else {
          dg::fillh( "n_offline_clusters_anyside" , 10 , 0 , 10 , offline_clusters_anyside.size() );
          dg::fillh( "n_offline_clusters_anyside_vs_local_x" , 768 , 0 , 768 , 10 , 0 , 10 , 
                     comp_cluster->local_x() , offline_clusters_anyside.size() ,
                     "LOCAL X (READOUT CHANNELS)" , "NUMBER OF OFFLINE CLUSTERS ON EITHER SIDE" );
        }
        // find best and worst dx and dz
        double best_dx = std::numeric_limits<double>::max();
        double best_dz = std::numeric_limits<double>::max();
        double best_dx_w[6] = { std::numeric_limits<double>::max() , 
                                std::numeric_limits<double>::max() , 
                                std::numeric_limits<double>::max() ,
                                std::numeric_limits<double>::max() ,
                                std::numeric_limits<double>::max() ,
                                std::numeric_limits<double>::max() };
        shared_ptr<const AnaSiCluster> best_dx_clu;
        shared_ptr<const AnaSiCluster> best_dz_clu;
        double worst_dx = 0.;
        double worst_dz = 0.;
        BOOST_FOREACH( const shared_ptr<const AnaSiCluster>& offclu , offline_clusters ) {
          const double dx = comp_cluster->local_x() - offclu->local_x();
          const double dz = ( comp_cluster->is_pixel() ? comp_cluster->local_z() - offclu->local_z() : -1. );
          if( std::abs(dx) < std::abs(best_dx) ) { 
            best_dx = dx;
            best_dx_clu = offclu;
          }
          const unsigned int nw = std::min( offclu->width_x() , 6 ) - 1;
          if( std::abs(dx) < std::abs(best_dx_w[nw]) ) { 
            assert( nw>=0 && nw<6 );
            best_dx_w[nw] = dx;
          }
          if( std::abs(dx) > std::abs(worst_dx) ) { 
            worst_dx = dx;
          }
          if( comp_cluster->is_pixel() ) {
            if( std::abs(dz) < std::abs(best_dz) ) { 
              best_dz = dz;
              best_dz_clu = offclu;
            }
            if( std::abs(dz) > std::abs(worst_dz) ) { 
              worst_dz = dz;
            }
          }
        } // end look for best and worst cluster matches in both dimensions
        //assert( offline_clusters.empty() || best_dx_clu ); // if there are any clusters, there should be a minimum cluster.
        // plot best and worst cluster position differences
        dg::filleff( "found_cluster" , !offline_clusters.empty() );
        dg::filleff( "found_cluster_vs_local_x" , 768 , 0 , 768 , comp_cluster->local_x() , !offline_clusters.empty() );
        if( comp_cluster->is_pixel() ) { dg::filleff( "found_cluster_vs_local_z" , 768 , 0 , 768 , comp_cluster->local_z() , !offline_clusters.empty() ); }
        if( std::abs(best_dx)<std::numeric_limits<double>::max() ) {
          dg::fillh( "best_dx" , 160 , -20 , 20 , best_dx );
          if( best_dx_clu->width_x() == 1 ) { dg::fillh( "best_dx_1w_offline" , 160 , -20 , 20 , best_dx ); }
          if( best_dx_clu->width_x() == 2 ) { dg::fillh( "best_dx_2w_offline" , 160 , -20 , 20 , best_dx ); }
          if( best_dx_clu->width_x() == 3 ) { dg::fillh( "best_dx_3w_offline" , 160 , -20 , 20 , best_dx ); }
          if( best_dx_clu->width_x() == 4 ) { dg::fillh( "best_dx_4w_offline" , 160 , -20 , 20 , best_dx ); }
          if( best_dx_clu->width_x() == 5 ) { dg::fillh( "best_dx_5w_offline" , 160 , -20 , 20 , best_dx ); }
          if( best_dx_clu->width_x() >= 6 ) { dg::fillh( "best_dx_6wp_offline" , 160 , -20 , 20 , best_dx ); }
          dg::fillh( "worst_dx" , 160 , -20 , 20 , worst_dx );
          if( std::abs(best_dx)>2 && std::abs(best_dx)<2.5 ) { dump_cluster_data = true; }
          if( comp_cluster->is_pixel() ) {
            dg::fillh( "best_dz" , 160 , -20 , 20 , best_dz );
            dg::fillh( "worst_dz" , 160 , -20 , 20 , worst_dz );
            if( std::abs(best_dx) > 0.2 || std::abs(best_dz) > 0.2 ) { 
              ++nbad_pixel; 
              dg::fillh( "n_offline_clusters_bad_pixel" , 10 , 0 , 10 , offline_clusters.size() , "NUMBER OF OFFLINE CLUSTERS" );
              dg::fillh( "n_offline_clusters_bad_pixel_vs_local_x" , 768 , 0 , 768 , 10 , 0 , 10 , comp_cluster->local_x() , offline_clusters.size() ,
                         "LOCAL X (READOUT CHANNELS)" , "NUMBER OF OFFLINE CLUSTERS" );
            }
          }
        }
        for( unsigned int iw=0; iw!=6; ++iw ) { 
          if( std::abs(best_dx_w[iw])<std::numeric_limits<double>::max() ) {
            dg::fillh( (format("best_dx_amongst_%dw_offline") % iw).str() , 160 , -20 , 20 , best_dx_w[iw] );
          }
        }
        if( best_dx_clu ) {
          if( best_dx_clu->is_pixel() && best_dz_clu && best_dx_clu == best_dz_clu ) {
            if( best_dx_clu->width_x()>1 ) { ++nmultichannel_pixels; }
          } else if( !(best_dx_clu->is_pixel()) ) {
            if( best_dx_clu->width_x()>1 ) { ++nmultichannel_strips; }
          }
        }
        if( !(comp_cluster->is_pixel()) ) {
          double best_dx_anyside = std::numeric_limits<double>::max();
          double worst_dx_anyside = 0.;
          BOOST_FOREACH( const shared_ptr<const AnaSiCluster>& offclu , offline_clusters_anyside ) {
            const double dx = comp_cluster->local_x() - offclu->local_x();
            if( std::abs(dx) < std::abs(best_dx_anyside) ) { 
              best_dx_anyside = dx;
            }
            if( std::abs(dx) > std::abs(worst_dx_anyside) ) { 
              worst_dx_anyside = dx;
            }
          }
          dg::filleff( "found_cluster_anyside" , !offline_clusters_anyside.empty() );
          dg::filleff( "found_cluster_anyside_vs_local_x" , 768 , 0 , 768 , comp_cluster->local_x() , !offline_clusters_anyside.empty() );
          if( !offline_clusters_anyside.empty() ) {
            dg::fillh( "best_dx_anyside" , 160 , -20 , 20 , best_dx_anyside );
            dg::fillh( "worst_dx_anyside" , 160 , -20 , 20 , worst_dx_anyside );
            if( std::abs(best_dx_anyside) > 0.2 ) { 
              ++nbad_sct; 
              dg::fillh( "n_offline_clusters_bad_sct" , 10 , 0 , 10 , offline_clusters.size() , "NUMBER OF OFFLINE CLUSTERS" );
              dg::fillh( "n_offline_clusters_bad_sct_anyside_vs_local_x" , 768 , 0 , 768 , 10 , 0 , 10 , comp_cluster->local_x() , offline_clusters_anyside.size() ,
                         "LOCAL X (READOUT CHANNELS)" , "NUMBER OF OFFLINE CLUSTERS ON EITHER SIDE" );
            }
            if( std::abs(best_dx_anyside) > 2. ) { ++nbad_sct_extreme; }
          }
        } // end look for best and worst residuals on either side
        // plot cluster parameters for reference and comparison in
        // cases which agree or disagree
        if( true && best_dx_clu ) { 
          dg::down( "bad_and_good_clusters" , "cluster comparison plots" ); 
          dg::down( best_dx_clu->is_pixel() ? "pixels" : "sct" , "cluster comparison plots" ); 
          dg::down( std::abs(best_dx) < 0.25 ? "dx_lt0.25" : "dx_gte0.25" , "bad or good clusters" );
          BOOST_SCOPE_EXIT() { dg::up(); dg::up(); dg::up(); } BOOST_SCOPE_EXIT_END;
          dg::fillh( "n_offline_clusters" , 10 , 0 , 10 , offline_clusters.size() );
          dg::fillh( "best_dx" , 160 , -20 , 20 , best_dx );
          if( comp_cluster->is_pixel() ) { dg::fillh( "best_dz" , 160 , -20 , 20 , best_dz ); }
          dg::fillh( "reference_z_vs_x" , 400 , 0 , 400 , 145 , 0  , 145 ,
                     best_dx_clu->local_x() , best_dx_clu->local_z() ,
                     "BEST X (1/PITCH)" , "BEST Z (1/PITCH)" );
          dg::fillh( "comparison_z_vs_x" , 400 , 0 , 400 , 145 , 0  , 145 ,
                     comp_cluster->local_x() , comp_cluster->local_z() ,
                     "BEST X (1/PITCH)" , "BEST Z (1/PITCH)" );
          dg::fillh( "reference_width_z_vs_x" , 10 , 0 , 10 , 10 , 0 , 10 ,
                     best_dx_clu->width_x() , best_dx_clu->width_z() ,
                     "WIDTH X (1/PITCH)" , "WIDTH Z (1/PITCH)" );
          dg::fillh( "comparison_width_z_vs_x" , 10 , 0 , 10 , 10 , 0 , 10 ,
                     comp_cluster->width_x() , comp_cluster->width_z() ,
                     "WIDTH X (1/PITCH)" , "WIDTH Z (1/PITCH)" );
          if( std::abs(best_dx) >= 0.25 ) { 
            if( comp_cluster->is_pixel() ) {
              ++nbad_pixel_dx;
            } else {
              ++nbad_sct_dx;
            }
          }
          if( comp_cluster->is_pixel() && std::abs(best_dz) >= 0.25 ) { 
            ++nbad_pixel_dz; 
          }
        }
        if( true && best_dz_clu ) { 
          dg::down( "bad_and_good_clusters" , "cluster comparison plots" ); 
          dg::down( best_dz_clu->is_pixel() ? "pixels" : "sct" , "cluster comparison plots" ); 
          dg::down( std::abs(best_dz) < 0.25 ? "dz_lt0.25" : "dz_gte0.25" , "bad or good clusters" );
          BOOST_SCOPE_EXIT() { dg::up(); dg::up(); dg::up(); } BOOST_SCOPE_EXIT_END;
          dg::fillh( "n_offline_clusters" , 10 , 0 , 10 , offline_clusters.size() );
          dg::fillh( "best_dz" , 160 , -20 , 20 , best_dz );
          if( comp_cluster->is_pixel() ) { dg::fillh( "best_dz" , 160 , -20 , 20 , best_dz ); }
          dg::fillh( "reference_z_vs_x" , 400 , 0 , 400 , 145 , 0  , 145 ,
                     best_dz_clu->local_x() , best_dz_clu->local_z() ,
                     "BEST Z (1/PITCH)" , "BEST X (1/PITCH)" );
          dg::fillh( "comparison_z_vs_x" , 400 , 0 , 400 , 145 , 0  , 145 ,
                     comp_cluster->local_x() , comp_cluster->local_z() ,
                     "BEST Z (1/PITCH)" , "BEST X (1/PITCH)" );
          dg::fillh( "reference_width_z_vs_x" , 10 , 0 , 10 , 10 , 0 , 10 ,
                     best_dz_clu->width_x() , best_dz_clu->width_z() ,
                     "WIDTH Z (1/PITCH)" , "WIDTH X (1/PITCH)" );
          dg::fillh( "comparison_width_z_vs_x" , 10 , 0 , 10 , 10 , 0 , 10 ,
                     comp_cluster->width_x() , comp_cluster->width_z() ,
                     "WIDTH Z (1/PITCH)" , "WIDTH X (1/PITCH)" );
        }
      } // end for each comparison track cluster
      if( true ) {
        // resolutions (params - turth, offline) for high/low pt
        // tracks when a pixel is bad in x, z or either
        dg::down( "resolutions_with_bad_clusters" , "cluster comparison plots" ); 
        BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
        dg::fillh( "nbad_pixels_dx" , 10 , 0 , 10 , nbad_pixel_dx );
        dg::fillh( "nbad_pixels_dz" , 10 , 0 , 10 , nbad_pixel_dz );
        dg::fillh( "nbad_scts_dx" , 10 , 0 , 10 , nbad_sct_dx );
        dg::fillh( "nmultichannel_pixels" , 5 , 0 , 5 , nmultichannel_pixels );
        dg::fillh( "nmultichannel_strips" , 5 , 0 , 5 , nmultichannel_strips );
        if( nmultichannel_pixels==0 && nmultichannel_strips==0 ) {
          dg::down( "all_single_channel" , "track resolution plots" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
          dg::down( reference_track->pt() > 10. ? "ref_pt_gt10" : "ref_pt_lte10" , "track resolution plots" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
          if( true ) {
            dg::down( "vs_reference" , "resolutions" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
            _residual_study( reference_track , comparison_track );
          }
          if( truth_track ) {
            dg::down( "vs_truth" , "resolutions" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
            _residual_study( truth_track , comparison_track );
          }
        }
        if( nmultichannel_pixels>0 ) {
          dg::down( "multichannel_pixels" , "track resolution plots" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
          dg::down( reference_track->pt() > 10. ? "ref_pt_gt10" : "ref_pt_lte10" , "track resolution plots" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
          if( true ) {
            dg::down( "vs_reference" , "resolutions" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
            _residual_study( reference_track , comparison_track );
          }
          if( truth_track ) {
            dg::down( "vs_truth" , "resolutions" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
            _residual_study( truth_track , comparison_track );
          }
        }
        if( nmultichannel_strips>0 ) {
          dg::down( "multichannel_strips" , "track resolution plots" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
          dg::down( reference_track->pt() > 10. ? "ref_pt_gt10" : "ref_pt_lte10" , "track resolution plots" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
          if( true ) {
            dg::down( "vs_reference" , "resolutions" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
            _residual_study( reference_track , comparison_track );
          }
          if( truth_track ) {
            dg::down( "vs_truth" , "resolutions" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
            _residual_study( truth_track , comparison_track );
          }
        }
        if( nbad_pixel_dx==0 && nbad_pixel_dz==0 && nbad_sct_dx==0 ) {
          dg::down( "all_good" , "track resolution plots" ); 
          BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
          dg::down( reference_track->pt() > 10. ? "ref_pt_gt10" : "ref_pt_lte10" ,
                    "track resolution plots" ); 
          BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
          if( true ) {
            dg::down( "vs_reference" , "resolutions" ); 
            BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
            _residual_study( reference_track , comparison_track );
          }
          if( truth_track ) {
            dg::down( "vs_truth" , "resolutions" ); 
            BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
            _residual_study( truth_track , comparison_track );
          }
        }
        if( nbad_pixel_dx>0 ) {
          dg::down( "bad_pixel_dx" , "track resolution plots" ); 
          BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
          dg::down( reference_track->pt() > 10. ? "ref_pt_gt10" : "ref_pt_lte10" ,
                    "track resolution plots" ); 
          BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
          if( true ) {
            dg::down( "vs_reference" , "resolutions" ); 
            BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
            _residual_study( reference_track , comparison_track );
          }
          if( truth_track ) {
            dg::down( "vs_truth" , "resolutions" ); 
            BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
            _residual_study( truth_track , comparison_track );
          }
        }
        if( nbad_pixel_dz>0 ) {
          dg::down( "bad_pixel_dz" , "track resolution plots" ); 
          BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
          dg::down( reference_track->pt() > 10. ? "ref_pt_gt10" : "ref_pt_lte10" ,
                    "track resolution plots" ); 
          BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
          if( true ) {
            dg::down( "vs_reference" , "resolutions" ); 
            BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
            _residual_study( reference_track , comparison_track );
          }
          if( truth_track ) {
            dg::down( "vs_truth" , "resolutions" ); 
            BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
            _residual_study( truth_track , comparison_track );
          }
        }
        if( nbad_sct_dx>0 ) {
          dg::down( "bad_sct_dx" , "track resolution plots" ); 
          BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
          dg::down( reference_track->pt() > 10. ? "ref_pt_gt10" : "ref_pt_lte10" ,
                    "track resolution plots" ); 
          BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
          if( true ) {
            dg::down( "vs_reference" , "resolutions" ); 
            BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
            _residual_study( reference_track , comparison_track );
          }
          if( truth_track ) {
            dg::down( "vs_truth" , "resolutions" ); 
            BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
            _residual_study( truth_track , comparison_track );
          }
        }        
      }
      dg::fillh( "nbad_pixel", 4 , 0 , 4 , nbad_pixel );
      dg::fillh( "nbad_sct", 4 , 0 , 4 , nbad_sct );
      dg::fillh( "nbad_sct_extreme", 4 , 0 , 4 , nbad_sct_extreme );
      dg::fillh( "nbad_both", 12 , 0 , 12 , nbad_pixel+nbad_sct );
      if( true ) { 
        if( nbad_pixel > 0 ) { 
          dg::down( "bad_pixel" , "one or more bad pixels residuals" ); 
          BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
          _residual_study( reference_track , comparison_track );
        }
        if( nbad_sct > 0 ) { 
          dg::down( "bad_sct" , "one or more bad sct residuals" ); 
          BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
          _residual_study( reference_track , comparison_track );
        }
        if( nbad_sct_extreme > 0 ) { 
          dg::down( "bad_sct_extreme" , "one or more extremely bad sct residuals" ); 
          BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
          _residual_study( reference_track , comparison_track );
        }
        if( (nbad_pixel+nbad_sct) > 2 ) { 
          dg::down( "bad_many" , "one or more bad silicon residuals" ); 
          BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
          _residual_study( reference_track , comparison_track );
        }
        if( (nbad_pixel+nbad_sct) > 0 ) { 
          dg::down( "bad_any" , "one or more bad silicon residuals" ); 
          BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
          _residual_study( reference_track , comparison_track );
        } else {
          dg::down( "good_all" , "no bad silicon residuals" ); 
          BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
          _residual_study( reference_track , comparison_track );
        }
        if( nbad_pixel>0 || nbad_sct>0 ) {
          AnaEventMgr* mgr = AnaEventMgr::instance();
          if( mgr->ana_event_ntuple_mode() == AnaEventNtuple::WRITE ) {
            mgr->add_to_ana_event_ntuple( true );
          }
        }
      } // end if make plots for tracks with/without bad residuals
    } // end if offline clusters to compare against
  } // if compare with offline clusters

   // track parameters vs magnitude and position of worst cluster mistake
  if( true && !comparison_track->si_clusters().empty() && !reference_track->si_clusters().empty() ) {
    // compare all cluster positions
    shared_ptr<const AnaSiCluster> worst_comp_cluster;
    shared_ptr<const AnaSiCluster> worst_ref_cluster;
    double worst_dx = 0.;
    double worst_dz = 0.;
    for( std::vector< shared_ptr<const AnaSiCluster> >::const_iterator icompclu=clusters.begin(), fcompclu=clusters.end(); icompclu!=fcompclu; ++icompclu ) {
      const shared_ptr<const AnaSiCluster>& comp_cluster( *icompclu );
      AnaSiCluster::ftype best_distance = 99999.;
      const shared_ptr<const AnaSiCluster>& match = comp_cluster->find_matching_cluster( reference_track->si_clusters().begin() , reference_track->si_clusters().end() , best_distance );
      if( !match ) { continue; }
      if( !worst_comp_cluster && !worst_ref_cluster ) { 
        worst_comp_cluster = comp_cluster;
        worst_ref_cluster = match;
        continue;
      }
      const double this_dx = std::abs( comp_cluster->local_x() - match->local_x() );
      const double this_dz = comp_cluster->cluster_type() == AnaSiChannel::PIXEL ? std::abs( comp_cluster->local_z() - match->local_z() ) : 0.;
      if( this_dx > worst_dx || this_dz > worst_dz ) {
        worst_dx = this_dx;
        worst_dz = this_dz;
        worst_comp_cluster = comp_cluster;
        worst_ref_cluster = match;
      }
    }
    dg::fillh( "worst_dx" , 100 , 0 , 20 , worst_dx );
    dg::fillh( "worst_dz" , 100 , 0 , 20 , worst_dz );
    dg::fillh( "worst_dx_vs_dz" , 100 , 0 , 20 , 100 , 0 , 20 , worst_dz , worst_dx , "DZ (CHANNELS)" , "DX (CHANNELS)" );
    if( worst_comp_cluster && worst_ref_cluster ) { 
      StudySiCluster stcluw( "clusters_worst" , "silicon clusters" );
      stcluw.reference_track( reference_track );
      stcluw.comparison_track( comparison_track );
      stcluw.reference_clusters( &(reference_track->si_clusters()) );
      stcluw.add_constant_category( "worst_cluster_is_sct_good" , worst_ref_cluster->cluster_type()==AnaSiChannel::STRIP && worst_dx<0.1 );
      stcluw.add_constant_category( "worst_cluster_is_sct_ok" , worst_ref_cluster->cluster_type()==AnaSiChannel::STRIP && worst_dx>0.1 && worst_dx<1 );
      stcluw.add_constant_category( "worst_cluster_is_sct_bad" , worst_ref_cluster->cluster_type()==AnaSiChannel::STRIP && worst_dx>1 );
      stcluw.add_constant_category( "worst_cluster_is_pixel_good" , worst_ref_cluster->cluster_type()==AnaSiChannel::PIXEL && worst_dx<0.1 );
      stcluw.add_constant_category( "worst_cluster_is_pixel_ok" , worst_ref_cluster->cluster_type()==AnaSiChannel::PIXEL && worst_dx>0.1 && worst_dx<1 );
      stcluw.add_constant_category( "worst_cluster_is_pixel_bad" , worst_ref_cluster->cluster_type()==AnaSiChannel::PIXEL && worst_dx>1 );
      stcluw.repurpose( "worst_comp_cluster" , "cluster with largest dx or dz residual wrt reference" );
      stcluw.for_only( worst_comp_cluster );
      stcluw.repurpose( "worst_ref_cluster" , "cluster with largest dx or dz residual wrt reference" );
      stcluw.for_only( worst_ref_cluster );
      // do track comparison for subset
      if( true ) { 
        dg::down( worst_ref_cluster->ftk_logical_layer()<3 ? "worst_cluster_pixel" : "worst_cluster_sct" , "cluster efficiency plots" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
        TrackIdentification tid( reference_track , comparison_track );
        StudyTrackComparison sttc( "worst_cluster_track_residuals" , "track residual dependence on worst cluster residuals amongst all layers" );
        sttc.do_residuals( true );
        sttc.do_cluster_study( false );
        sttc.do_estimates( false );
        sttc.do_covariance_vs_residuals( false );
        sttc.do_residual_correlations( false );
        sttc.do_pulls( true );
        sttc.add_constant_category( "ok_worst_dx" , worst_dx > 0.1 && worst_dx < 1 );
        sttc.add_constant_category( "bad_worst_dx" , worst_dx > 1 );
        sttc.add_constant_category( "good_worst_dx" , worst_dx < 0.1 );
        sttc.add_constant_category( "ok_worst_dz" , worst_dz > 0.1 && worst_dz < 1 );
        sttc.add_constant_category( "bad_worst_dz" , worst_dz > 1 );
        sttc.add_constant_category( "good_worst_dz" , worst_dz < 0.1 );
        sttc.for_only( tid );
        if( !(reference_track->is_truth()) ) { 
          // look for truth track matching reference. if found, plot track parameter residuals wrt truth.
          const shared_ptr<const AnaEvent>& ev = AnaEventMgr::current_event(); assert( ev );
          std::vector<  shared_ptr<const AnaTrack>  > truth_tracks( ev->truth_tracks(AnaEventMgr::DEFAULT_SOURCE) );
          std::vector<  shared_ptr<const AnaTrack>  >::const_iterator itruth = std::find_if( truth_tracks.begin() , truth_tracks.end() , 
                                                                                boost::bind( &AnaTrack::barcode , _1 ) == reference_track->barcode() );
          const shared_ptr<const AnaTrack> reference_track_truth = (itruth!=truth_tracks.end()) ? *itruth : shared_ptr<const AnaTrack>();
          dg::filleff( "worst_cluster_track_residuals_wrt_truth_has_eff" , reference_track_truth );
          if( reference_track_truth ) { 
            TrackIdentification tidt( reference_track_truth , comparison_track );
            sttc.repurpose( "worst_cluster_track_residuals_wrt_truth" , "track residual dependence on worst cluster residuals amongst all layers" );
            sttc.for_only( tidt );
          }
        }
      }
      if( worst_dx > 0 ) { dg::fillh( "worst_dx_vs_layer" , 12 , 0 , 12 , 50 , 0 , 5 , worst_ref_cluster->ftk_logical_layer() , worst_dx , "FTK LOGICAL LAYER" , "WORST DX (CHANNELS)" ); }
      if( worst_dz > 0 ) { dg::fillh( "worst_dz_vs_layer" , 12 , 0 , 12 , 50 , 0 , 5 , worst_ref_cluster->ftk_logical_layer() , worst_dz , "FTK LOGICAL LAYER" , "WORST DZ (CHANNELS)" ); }
    } // end study worst cluster
  } // end study track parameters vs size and position of worst cluster mistake
  // study cluster attachment efficiency relative to offline
  if( true && !comparison_track->si_clusters().empty() && !reference_track->si_clusters().empty() ) {
    dg::down( "cluster_eff" , "cluster efficiency plots" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
    for( std::vector< shared_ptr<const AnaSiCluster> >::const_iterator irefclu=reference_track->si_clusters().begin(), 
           frefclu=reference_track->si_clusters().end(); irefclu!=frefclu; ++irefclu ) {
      const shared_ptr<const AnaSiCluster>& ref_cluster( *irefclu );
      // look for matching comparison cluster
      AnaSiCluster::ftype best_distance = 99999.;
      const shared_ptr<const AnaSiCluster>& match = ref_cluster->find_matching_cluster( clusters.begin() , clusters.end() , best_distance );
      // this may be a layer with more than one reference hit, whereas FTK does not make use of multiple hits per layer.
      // if there is still a better match available, skip to the next hit
      AnaSiCluster::ftype other_distance = 999999.;
      if( true ) {
        assert( irefclu!=frefclu );
        // get remaining hits on same logical layer
        for( std::vector< shared_ptr<const AnaSiCluster> >::const_iterator j=clusters.begin(), jf=clusters.end(); j!=jf; ++j ) { 
          const shared_ptr<const AnaSiCluster>& jclu( *j );
          if( jclu == ref_cluster ) { continue; }
          if( jclu->ftk_logical_layer() != ref_cluster->ftk_logical_layer() ) { continue ; } 
          AnaSiCluster::ftype jdist = 9999999.;
          const shared_ptr<const AnaSiCluster>& match = (*j)->find_matching_cluster( clusters.begin() , clusters.end() , jdist );
          other_distance = std::min( jdist , other_distance );
        }
        if( other_distance < best_distance ) { continue; }
      }
      // is there a match? is it good?
      dg::filleff( "any_compclu_eff_vs_layer" , 12 , 0 , 12 , ref_cluster->ftk_logical_layer() , match );
      dg::filleff( "ok_compclu_eff_vs_layer" , 12 , 0 , 12 , ref_cluster->ftk_logical_layer() , match && best_distance<1 );
      dg::filleff( "good_compclu_eff_vs_layer" , 12 , 0 , 12 , ref_cluster->ftk_logical_layer() , match && best_distance<0.2 );
      if( match ) { dg::fillh( "best_distance" , 100 , 0 , 10 , best_distance ); }
      if( comparison_track->is_ftk() && comparison_track->ftk_is_majority() ) {
        dg::filleff( "any_compclu_eff_vs_layer_majority" , 12 , 0 , 12 , ref_cluster->ftk_logical_layer() , match );
        dg::filleff( "ok_compclu_eff_vs_layer_majority" , 12 , 0 , 12 , ref_cluster->ftk_logical_layer() , match && best_distance<1 );
        dg::filleff( "good_compclu_eff_vs_layer_majority" , 12 , 0 , 12 , ref_cluster->ftk_logical_layer() , match && best_distance<0.2 );
        if( match ) { dg::fillh( "best_distance_majority" , 100 , 0 , 10 , best_distance ); }
      } else if( comparison_track->is_ftk() && !comparison_track->ftk_is_majority() ) { 
        dg::filleff( "any_compclu_eff_vs_layer_full" , 12 , 0 , 12 , ref_cluster->ftk_logical_layer() , match );
        dg::filleff( "ok_compclu_eff_vs_layer_full" , 12 , 0 , 12 , ref_cluster->ftk_logical_layer() , match && best_distance<1 );
        dg::filleff( "good_compclu_eff_vs_layer_full" , 12 , 0 , 12 , ref_cluster->ftk_logical_layer() , match && best_distance<0.2 );
        if( match ) { dg::fillh( "best_distance_full" , 100 , 0 , 10 , best_distance ); }        
      }
      // if there isn't a match, where is the reference cluster?
      StudySiCluster stbadmatch( "missing" , "studies of reference cluster when comparison cluster is missing" );
      stbadmatch.add_constant_category( "pixel" , ref_cluster->cluster_type()==AnaSiChannel::PIXEL );
      stbadmatch.add_constant_category( "sct" , ref_cluster->cluster_type()==AnaSiChannel::STRIP );
      stbadmatch.add_constant_category( "pixel_majority" , ref_cluster->cluster_type()==AnaSiChannel::PIXEL && comparison_track->is_ftk() && comparison_track->ftk_is_majority() );
      stbadmatch.add_constant_category( "sct_majority" , ref_cluster->cluster_type()==AnaSiChannel::STRIP && comparison_track->is_ftk() && comparison_track->ftk_is_majority() );
      stbadmatch.add_constant_category( "pixel_full" , ref_cluster->cluster_type()==AnaSiChannel::PIXEL && comparison_track->is_ftk() && !comparison_track->ftk_is_majority() );
      stbadmatch.add_constant_category( "sct_full" , ref_cluster->cluster_type()==AnaSiChannel::STRIP && comparison_track->is_ftk() && !comparison_track->ftk_is_majority() );
      stbadmatch.study_uncategorized( false );
      if( !match ) {
        stbadmatch.for_only( ref_cluster );
      }
      if( match && best_distance > 1 ) {
        stbadmatch.repurpose( "bad" , "studies of reference cluster when comparison cluster isn't in the same place" );
        stbadmatch.for_only( ref_cluster );
      }
      if( match && best_distance > 0.2 && best_distance < 1 ) {
        stbadmatch.repurpose( "ok" , "studies of reference cluster when comparison cluster centroid is slightly displaced from reference" );
        stbadmatch.for_only( ref_cluster );
      }
      if( match && best_distance < 0.2 ) {
        stbadmatch.repurpose( "good" , "studies of reference cluster when comparison cluster centroid is in good agreement with reference" );
        stbadmatch.for_only( ref_cluster );
      }
      // dump layer 9 and 10 clusters when there is no match and the track is a full fit.
      if( false && comparison_track->is_ftk() && !comparison_track->ftk_is_majority() && !match && ref_cluster->ftk_logical_layer()>=9 ) {
        cout << " FTK TRACK MISSING FULL FIT MATCH FOR LAYER " << ref_cluster->ftk_logical_layer() << endl;
        cout << " FTK track: " << endl;
        comparison_track->print();
        cout << " offline track: " << endl;
        reference_track->print();
      }
    } // end for each reference cluster
  } // end study cluster attachment efficiency relative to offline


  // dump cluster data for majority FTK tracks where SCT |dx| ~ 2.5
  if( true && dump_cluster_data ) { 
    
    cout << " reference track: ";
    reference_track->print();
    // cout << " reference track clusters: " << endl;
    // BOOST_FOREACH( const shared_ptr<const AnaSiCluster>& clu , reference_track->si_clusters() ) {
    //   clu->print();
    // }
    cout << " comparison track: ";
    if( comparison_track->is_ftk() ) { cout << comparison_track->ftk_is_majority(); }
    comparison_track->print();
    cout << " comparison track clusters: " << endl;
    BOOST_FOREACH( const shared_ptr<const AnaSiCluster>& clu , comparison_track->si_clusters() ) {
      clu->print();
    }
  }
}
