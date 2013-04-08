#ifndef ANATRUTHTABLE_HPP
#define ANATRUTHTABLE_HPP

#include <cassert>
#include <map>
#include <set>
#include <utility>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <boost/format.hpp>
#include <boost/foreach.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/bind.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/transitive_closure.hpp>
#include <boost/graph/topological_sort.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/iteration_macros.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/visitors.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/version.hpp>
#include <boost/graph/adj_list_serialize.hpp>
#include <boost/bimap.hpp>
#include <boost/unordered_set.hpp>
#include <TLorentzVector.h>
#include <TDatabasePDG.h>
#include "TrigFTKAna/AnaTruthParticle.hpp"
#include "TrigFTKAna/AnaParticle.hpp"

// two utility classes for writing vertices and edges to GraphViz files
template<class graphT,typename ptrT>
class truth_table_vertex_writer {
private:
  graphT _graph;
  const TDatabasePDG* _pdg;
public:
  truth_table_vertex_writer( graphT graph ) : _graph(graph) , _pdg(TDatabasePDG::Instance()) {}
  template<class vertexT> void operator()( std::ostream& os , const vertexT& v ) const {
    ptrT ptr( _graph[v].ptr );
    std::string k = "#2a2c2b";
    std::string n = (boost::format( "#%d" ) %v).str();;
    std::string tooltip = "";
    if( ptr ) { 
      // gray(2a2c2b) black(1e1e20) slate(374140) khaki(d9cb9e) red(dc3522) lightgray(aecdca) medgray(788e8b) blue(4b7fa7) 
      // wine(521000) reddish(712800) brown(744e1d) olive(879666)  tan(f1d98c)
      switch( ptr->status() ) {
      case 1: k="#dc3522"; break; // red
      case 2: k="#d9cb9e"; break; // khaki
      case 3: k="#374140"; break; // slate
      default: break;
      }
      n = (boost::format( "%s (b%d)" ) % ptr->pdg_name() % ptr->index()).str();
      if( ptr->pt()>0 ) { tooltip = (boost::format( "Pt %g Eta %g Phi %g %g" ) % ptr->pt() % ptr->eta() % ptr->phi() % ptr->mass()).str(); }
    }
    os << boost::format( "[ label=\"%s\", shape=\"box\" , color=\"%s\" , tooltip=\"%s\"] , style=\"filled\" , fillcolor=\"%s\" , fontcolor=\"white\"" ) % n % k % tooltip % k;
  }
};


// two utility classes for writing vertices and edges to GraphViz files
template<class graphT,typename ptrT>
class truth_table_edge_writer {
private:
  graphT _graph;
  const TDatabasePDG* _pdg;
public:
  truth_table_edge_writer( graphT graph ) : _graph(graph) , _pdg(TDatabasePDG::Instance()) {}
  template<class edgeT> void operator()( std::ostream& os , const edgeT& e ) const {
    // get vertices along the edge
    ptrT source_ptr = _graph[ boost::source(e,_graph) ].ptr;
    ptrT target_ptr = _graph[ boost::target(e,_graph) ].ptr;
    std::string source_name = source_ptr ? source_ptr->pdg_name() : "";
    std::string target_name = target_ptr ? target_ptr->pdg_name() : "";
    // set color and vertex display properties
    std::string k = "#9f9f9f";
    int weight = 1;
    if( target_ptr && target_ptr->is_lepton() ) { k = "#1f1f1f"; }
    if( (source_ptr && source_ptr->is_boson()) || (target_ptr && target_ptr->is_boson()) ) { 
      k = "#ff0000"; 
      weight = 2;
    }
    if( target_ptr && target_ptr->status()==1 ) { weight=2; }
    // write label.
    os << boost::format( "[ label=\"%s=>%s\" , color=\"%s\" , weight=\"%d\"] " ) % source_name % target_name % k % weight;
  }
};

// visitor to construct vertex reachability cache. this is a list of
// pairs denoting all (u,v) for which there is a path u->v along
// out-edges in the graph.
template< typename vertex_type , typename edge_type , typename outputIterT >
class dfs_reachability_visitor : public boost::default_dfs_visitor
{
private:
  outputIterT _out;
  std::vector< vertex_type > _ancestors;
public:
  dfs_reachability_visitor( outputIterT output_iterator )
    : _out(output_iterator) {}
  template< typename vertexT , typename graphT >
  void discover_vertex( vertexT v , const graphT& g ) {
    //std::cout << "discovered vertex " << v << std::endl;
    BOOST_FOREACH( const vertexT& u , _ancestors ) {
      (*(_out++)) = edge_type(u,v);
    }
    _ancestors.push_back( v );
  }
  template< typename vertexT , typename graphT >
  void finish_vertex( vertexT v , const graphT& g ) {
    //std::cout << "finished vertex " << v << std::endl;
    assert( _ancestors.back() == v );
    _ancestors.pop_back();
  }
  // template< typename edgeT , typename graphT >
  // void back_edge( edgeT e, const graphT& g ) const {
  //   //std::cout << " back edge: " << boost::source(e,g) << " to " << boost::target(e,g) << std::endl;
  // }
  // template< typename edgeT , typename gT >
  // void tree_edge( edgeT e, const gT& g ) const {
  //   //std::cout << " tree edge: " << boost::source(e,g) << " to " << boost::target(e,g) << std::endl;
  // }
};

// truth table manager
class
AnaTruthTable
{

public:

  typedef AnaTruthParticle truth_type;
  typedef boost::shared_ptr<truth_type> truth_ptr;
  typedef boost::shared_ptr<const truth_type> truth_const_ptr;

private:

  class vertex_data {
  public:
    truth_ptr ptr;
    //friend class boost::serialization::access;
    template<class archiveT>
    void serialize( archiveT& ar, const unsigned int version ) {
      ar & boost::serialization::make_nvp("ptr",ptr);
    }
  };
  
  typedef boost::adjacency_list<boost::vecS,boost::vecS,boost::bidirectionalS,vertex_data> graph_type;
  typedef boost::graph_traits<graph_type>::vertex_descriptor vertex_type;
  typedef std::pair< vertex_type , vertex_type > edge_type;
  typedef std::map<vertex_type,truth_ptr> truth_map;
  typedef boost::bimap<vertex_type,vertex_type> graph_barcode_bimap;
  typedef boost::unordered_set< edge_type > vertex_reachability;

  class truth_ptr_lessthan { public: const bool operator()( const truth_ptr& a , const truth_ptr& b ) const { return a->index() < b->index(); }; };
  typedef std::map<AnaParticle::Type, std::set< truth_ptr > > reco_truths;
  //typedef std::set< truth_ptr , truth_ptr_lessthan > truth_particle_coll;
  typedef std::map< vertex_type , truth_ptr > truth_particle_coll; // particle by barcode
  typedef std::vector< truth_ptr > truth_particle_cache; // used for truth particle results caches

public:

  typedef vertex_type index_type;
  typedef std::set< truth_ptr > reco_truth_coll;

  // Drell-Yan == ZBOSON
  typedef enum { WBOSON , ZBOSON , TTBAR , SMWW , SMWZ , SMZZ , SINGLETOP , OTHER } EventType;
  typedef enum { ALL_ELECTRON , ALL_MUON , ELECTRON_MUON , 
                 ELECTRON_PLUS_JETS , MUON_PLUS_JETS , ALL_HADRONIC , UNCLASSIFIED } EventDecayType;

private:

  // internal graph
  graph_type _graph;
  truth_particle_coll _truth_particles;
  mutable vertex_reachability _vertex_reachability_cache; // pair of gids: is second reachable by out-edges of first?
  mutable truth_particle_cache _final_states_cache; // all final states
  mutable truth_particle_cache _heavy_bosons_cache; // all heavy gauge bosons
  mutable truth_particle_cache _quarks_or_gluons_cache; // all quarks or gluons
  mutable EventType _event_type; // event process type as computed from truth table
  mutable EventDecayType _event_decay_type; // decay channel for the event process type, as computed from the truth table
  graph_barcode_bimap _graph_barcode_bimap; // bimap to translate graph vertex ids into particle barcodes and vice versa.
  vertex_type _next_vertex_gid; // next unused vertex id
  // truth_map _map_particle;
  reco_truths _reco_truth_for_kind;

private:

  friend class boost::serialization::access;
  template<class archiveT>
  void serialize( archiveT& ar, const unsigned int version ) {
    ar & boost::serialization::make_nvp("graph",_graph);
    ar & boost::serialization::make_nvp("truth_particles",_truth_particles);
    ar & boost::serialization::make_nvp("graph_barcode_bimap",_graph_barcode_bimap);
    ar & boost::serialization::make_nvp("next_vertex_gid",_next_vertex_gid);
    ar & boost::serialization::make_nvp("reco_truth_for_kind",_reco_truth_for_kind);
    // rebuild reachability cache
    finalize();
  }

private:

  const index_type barcode_for_gid( const index_type& gid ) const {
    graph_barcode_bimap::left_map::const_iterator ibar = _graph_barcode_bimap.left.find( gid );
    assert( ibar != _graph_barcode_bimap.left.end() );
    return ibar->second;
  }
  const index_type gid_for_barcode( const index_type& barcode ) {
    graph_barcode_bimap::right_map::const_iterator igid = _graph_barcode_bimap.right.find( barcode );
    index_type event_index;
    if( igid != _graph_barcode_bimap.right.end() ) {
      return igid->second;
    }
    index_type gid = _next_vertex_gid++;
    _graph_barcode_bimap.insert( graph_barcode_bimap::value_type( gid , barcode ) );
    return gid;
  }
  const index_type gid_for_barcode_check( const index_type& barcode ) const {
    graph_barcode_bimap::right_map::const_iterator igid = _graph_barcode_bimap.right.find( barcode );
    index_type event_index;
    assert( igid != _graph_barcode_bimap.right.end() );
    return igid->second;
  }

  void _refresh_all_final_states() const {
    _final_states_cache.clear();
    BOOST_FOREACH( const truth_map::value_type& p , _truth_particles ) {
      const truth_ptr& ptr( p.second );
      if( ptr->status() != 1 ) { continue; }
      _final_states_cache.push_back( ptr );
    }
  }

  void _refresh_all_heavy_bosons() const {
    _heavy_bosons_cache.clear();
    BGL_FORALL_VERTICES( v , _graph , graph_type ) {
      truth_ptr ptr( _graph[v].ptr );
      if( !ptr ) { continue; }
      if( !(ptr->is_heavy_boson()) ) { continue; }
      // skip bosons with no parents. e.g. in addition to the w+w-
      // from ttbar decay, pythia will produce an extra W+W- pair
      // with no parents which decays to high-ID-numbered final
      // states (looks like photons, etc.)
      if( boost::in_degree( v , _graph ) == 0 ) { continue; }
      // skip mc placeholders with no decay products
      const unsigned int ndecayed = boost::out_degree( v , _graph );
      if( ndecayed == 0 ) { continue; }
      // skip extremely virtual W's and Z's radiating off the generator process in Pythia
      if( ptr->is_w_boson() && ptr->mass() < 15. && ndecayed<2 ) { continue; }
      if( ptr->is_z_boson() && ptr->mass() < 15. && ndecayed<2 ) { continue; }
      // look at decay particles.
      bool reject = false;
      BGL_FORALL_OUTEDGES( v , e , _graph , graph_type ) {
        truth_ptr target_ptr( _graph[ boost::target(e,_graph) ].ptr );
        if( !target_ptr ) { continue; }
        if( target_ptr == ptr ) { continue; }
        if( target_ptr->index() == ptr->index() ) { continue; }
        if( target_ptr->pdg_id() == ptr->pdg_id() ) { 
          // look for direct cycles (e.g. those in Pythia MC where W(status==2) -> W(status==3) => W(status==2) )
          if( target_ptr->status()%100==3 && ptr->status()%100==2 ) { continue; }
          reject = true; 
          break;
        }
      }
      if( reject ) { continue; }
      _heavy_bosons_cache.push_back( ptr );
    } // end for all vertices
  }

  void _refresh_all_quarks_or_gluons() const {
    _quarks_or_gluons_cache.clear();
    BGL_FORALL_VERTICES( v , _graph , graph_type ) {
      truth_ptr ptr( _graph[v].ptr );
      if( !ptr ) { continue; }
      if( !(ptr->is_quark_or_gluon()) ) { continue; }
      // reject incoming partons that have no links to the initial protons
      // (e.g. the partial info provided by the Top D3PDs)
      bool reject = false;
      BGL_FORALL_INEDGES( v , e , _graph , graph_type ) {
        truth_ptr source_ptr( _graph[ boost::source(e,_graph) ].ptr );
        if( !source_ptr ) { continue; }
        if( source_ptr->is_incoming_proton() ) { 
          if( !(source_ptr->is_top_quark()) ) {
            reject = true; break; 
          }
        }
      }
      if( !reject && boost::in_degree( v , _graph ) == 0 && 
          ptr->is_quark_or_gluon() && ptr->index()<=5 ) {
        reject = true;
      }
      if( reject ) { continue; }
      // reject particles which decay to themselves.
      // also reject particles which decay to barcode>=10000 final states.
      BGL_FORALL_OUTEDGES( v , e , _graph , graph_type ) {
        truth_ptr target_ptr( _graph[ boost::target(e,_graph) ].ptr );
        if( !target_ptr ) { continue; }
        if( target_ptr == ptr ) { continue; }
        if( target_ptr->index() == ptr->index() ) { continue; }
        if( target_ptr->pdg_id() == ptr->pdg_id() ) { reject = true; break; }
        if( target_ptr->index() > 10000 ) { reject = true; break; }
      }
      if( reject ) { continue; }
      _quarks_or_gluons_cache.push_back( ptr );
    } // end for all vertices
  }

  void _refresh_event_type() const {
    using namespace std;
    using namespace boost;
    _event_type = OTHER;
    return;
    // count numbers of W, Z, and top.
    const unsigned int nw = num_w_bosons();
    const unsigned int nz = num_z_bosons();
    const unsigned int nt = num_t_quarks();
    if( false ) { // print w's, z's, tops, and daughters
      const vector<truth_ptr> w_daughters( all_from_decays_of_type< vector<truth_ptr> >(24u,
                                                                                        bind(&AnaTruthParticle::is_final_state,_1)==true ||
                                                                                        bind(&AnaTruthParticle::is_quark,_1)==true) );
      const vector<truth_ptr> z_daughters( all_from_decays_of_type< vector<truth_ptr> >(23u,
                                                                                        bind(&AnaTruthParticle::is_final_state,_1)==true ||
                                                                                        bind(&AnaTruthParticle::is_quark,_1)==true) );
      const vector<truth_ptr> top_daughters( all_from_decays_of_type< vector<truth_ptr> >(6u,
                                                                                          bind(&AnaTruthParticle::is_final_state,_1)==true ||
                                                                                          bind(&AnaTruthParticle::is_quark,_1)==true) );
      cout << " new event with "
           << _heavy_bosons_cache.size() << " heavy bosons, "
           << _quarks_or_gluons_cache.size() << " quarks or gluons " 
           << _final_states_cache.size() << " final states"
           << endl;
      print_cdf_style(cout);
      cout << "heavy bosons: " << endl;
      BOOST_FOREACH( const truth_ptr& ptr , _heavy_bosons_cache ) { ptr->print(cout); }
      cout << "quarks or gluons: " << endl;
      BOOST_FOREACH( const truth_ptr& ptr , _quarks_or_gluons_cache ) { ptr->print(cout); }
      cout << "final states: " << endl;
      BOOST_FOREACH( const truth_ptr& ptr , _final_states_cache ) { ptr->print(cout); }
      cout << "w daughters: " << endl;
      BOOST_FOREACH( const truth_ptr& ptr , w_daughters ) { ptr->print(cout); }
      cout << "z daughters: " << endl;
      BOOST_FOREACH( const truth_ptr& ptr , z_daughters ) { ptr->print(cout); }
      cout << " top daughters: " << endl;
      BOOST_FOREACH( const truth_ptr& ptr , top_daughters ) { ptr->print(cout); }
      cout << "================================================================" << endl;
    }
    // decide. does not handle all possibilities and is intended to
    // evolve as analyses get more complicated.
    if( nt==2 && nw>=2 ) { _event_type = TTBAR; }
    else if( nw>=2 ) { _event_type = SMWW; }
    else if( nw>=1 && nz>=1 ) { _event_type = SMWZ; }
    else if( nz>=2 ) { _event_type = SMZZ; }
    else if( nt==1 && nw>=1 ) { _event_type = SINGLETOP; }
    else if( nw==1 ) { _event_type = WBOSON; }
    else if( nz==1 ) { _event_type = ZBOSON; }
    _event_decay_type = UNCLASSIFIED;
    if( _event_type == TTBAR || _event_type==SMWW ) {
      // type depends on W decays.
      const vector<truth_ptr> w_daughters( all_from_decays_of_type< vector<truth_ptr> >(24u,
                                                                                        bind(&AnaTruthParticle::is_final_state,_1)==true ||
                                                                                        bind(&AnaTruthParticle::is_quark,_1)==true) );
      //assert( w_daughters.size()==4 );
      if( w_daughters.size()<4 ) { 
        cout << " TTBAR/WW w daughters not correct; truth table is broken. event dump follows." << endl; 
        print_cdf_style( std::cout );
      }
      // can have more than 4 w daughters, e.g. fsr from leptons
      // count numbers of electrons, muons, and jets.
      const unsigned int nele = count_if( w_daughters.begin() , w_daughters.end() , bind(&AnaTruthParticle::is_electron,_1)==true );
      const unsigned int nmu = count_if( w_daughters.begin() , w_daughters.end() , bind(&AnaTruthParticle::is_muon,_1)==true );
      const unsigned int njet = count_if( w_daughters.begin() , w_daughters.end() , bind(&AnaTruthParticle::is_quark,_1)==true );
      if( nele==2 ) { _event_decay_type = ALL_ELECTRON; }
      else if( nmu==2 ) { _event_decay_type = ALL_MUON; }
      else if( nele==1 && nmu==1 ) { _event_decay_type = ELECTRON_MUON; }
      else if( nele==1 && njet>=2 ) { _event_decay_type = ELECTRON_PLUS_JETS; }
      else if( nmu==1 && njet>=2 ) { _event_decay_type = MUON_PLUS_JETS; }
      else if( njet>=4 ) { _event_decay_type = ALL_HADRONIC; }
    } else if( _event_type == WBOSON || _event_type == SINGLETOP ) {
      // type depends on W decays.
      const vector<truth_ptr> w_daughters( all_from_decays_of_type< vector<truth_ptr> >(24u,
                                                                                        bind(&AnaTruthParticle::is_final_state,_1)==true ||
                                                                                        bind(&AnaTruthParticle::is_quark,_1)==true) );
      // assert( w_daughters.size()>=2 ); // some FSR photons allowed
      if( w_daughters.size() < 2 ) { 
        cout << " W/Single Top truth table recognition broken: no W daughters. event dump follows" << endl; print_cdf_style(std::cout); 
      }
      // count numbers of electrons, muons, and jets.
      const unsigned int nele = count_if( w_daughters.begin() , w_daughters.end() , bind(&AnaTruthParticle::is_electron,_1)==true );
      const unsigned int nmu = count_if( w_daughters.begin() , w_daughters.end() , bind(&AnaTruthParticle::is_muon,_1)==true );
      const unsigned int njet = count_if( w_daughters.begin() , w_daughters.end() , bind(&AnaTruthParticle::is_quark,_1)==true );
      if( nele==1 ) { _event_decay_type = ALL_ELECTRON; }
      else if( nmu==1 ) { _event_decay_type = ALL_MUON; }
      else if( njet>=2 ) { _event_decay_type = ALL_HADRONIC; }
    } else if( _event_type == ZBOSON ) { 
      // type depends on Z decays.
      const vector<truth_ptr> z_daughters( all_from_decays_of_type< vector<truth_ptr> >(23u,
                                                                                        bind(&AnaTruthParticle::is_final_state,_1)==true ||
                                                                                        bind(&AnaTruthParticle::is_quark,_1)==true) );
      if( z_daughters.size() < 2 ) { cout << " Z truth table recognition broken: no Z daughters. event dump follows" << endl; print_cdf_style(std::cout); }
      //assert( z_daughters.size()>=2 ); // some FSR photons allowed
      // count numbers of electrons, muons, and jets.
      const unsigned int nele = count_if( z_daughters.begin() , z_daughters.end() , bind(&AnaTruthParticle::is_electron,_1)==true );
      const unsigned int nmu = count_if( z_daughters.begin() , z_daughters.end() , bind(&AnaTruthParticle::is_muon,_1)==true );
      const unsigned int njet = count_if( z_daughters.begin() , z_daughters.end() , bind(&AnaTruthParticle::is_quark,_1)==true );
      if( nele==1 ) { _event_decay_type = ALL_ELECTRON; }
      else if( nmu==1 ) { _event_decay_type = ALL_MUON; }
      else if( njet>=2 ) { _event_decay_type = ALL_HADRONIC; }
    } else if( _event_type == SMWZ ) {
      // type depends on W and Z decays.
      const vector<truth_ptr> w_daughters( all_from_decays_of_type< vector<truth_ptr> >(24u,
                                                                                        bind(&AnaTruthParticle::is_final_state,_1)==true ||
                                                                                        bind(&AnaTruthParticle::is_quark,_1)==true) );
      const vector<truth_ptr> z_daughters( all_from_decays_of_type< vector<truth_ptr> >(23u,
                                                                                        bind(&AnaTruthParticle::is_final_state,_1)==true ||
                                                                                        bind(&AnaTruthParticle::is_quark,_1)==true) );
      if( w_daughters.size() < 2 ) { cout << " WZ truth table recognition broken: no W daughters. event dump follows" << endl; print_cdf_style(std::cout); }
      if( z_daughters.size() < 2 ) { cout << " WZ truth table recognition broken: no Z daughters. event dump follows" << endl; print_cdf_style(std::cout); }
      // assert( w_daughters.size()>=2 ); // some FSR photons allowed
      // assert( z_daughters.size()>=2 ); // some FSR photons allowed
      // count numbers of electrons, muons, and jets.
      const unsigned int nwele = count_if( w_daughters.begin() , w_daughters.end() , bind(&AnaTruthParticle::is_electron,_1)==true );
      const unsigned int nwmu = count_if( w_daughters.begin() , w_daughters.end() , bind(&AnaTruthParticle::is_muon,_1)==true );
      const unsigned int nwjet = count_if( w_daughters.begin() , w_daughters.end() , bind(&AnaTruthParticle::is_quark,_1)==true );
      const unsigned int nzele = count_if( z_daughters.begin() , z_daughters.end() , bind(&AnaTruthParticle::is_electron,_1)==true );
      const unsigned int nzmu = count_if( z_daughters.begin() , z_daughters.end() , bind(&AnaTruthParticle::is_muon,_1)==true );
      const unsigned int nzjet = count_if( z_daughters.begin() , z_daughters.end() , bind(&AnaTruthParticle::is_quark,_1)==true );
      if( nwele>=1 && nzele>=2 ) { _event_decay_type = ALL_ELECTRON; }
      else if( nwmu>=1 && nzmu>=2 ) { _event_decay_type = ALL_MUON; }
      else if( nwele>=1 && nzmu>=2 ) { _event_decay_type = ELECTRON_MUON; }
      else if( nwmu>=1 && nzele>=2 ) { _event_decay_type = ELECTRON_MUON; } // or should we distinguish with MUON_ELECTRON?
      else if( nwele>=1 && nzjet>=2 ) { _event_decay_type = ELECTRON_PLUS_JETS; }
      else if( nwmu>=1 && nzjet>=2 ) { _event_decay_type = MUON_PLUS_JETS; }
      else if( nwjet>=2 && nzele>=2 ) { _event_decay_type = ELECTRON_PLUS_JETS; }
      else if( nwjet>=2 && nzmu>=2 ) { _event_decay_type = MUON_PLUS_JETS; }
      else if( nwjet>=2 && nzjet>=2 ) { _event_decay_type = ALL_HADRONIC; }
    } else if( _event_type == SMZZ ) {
      // type depends on Z decays.
      const vector<truth_ptr> z_daughters( all_from_decays_of_type< vector<truth_ptr> >(24u,
                                                                                        bind(&AnaTruthParticle::is_final_state,_1)==true ||
                                                                                        bind(&AnaTruthParticle::is_quark,_1)==true) );
      if( z_daughters.size() < 4 ) { cout << " ZZ truth table recognition broken: no z daughters. event dump follows" << endl; print_cdf_style(std::cout); }
      // assert( z_daughters.size()>=4 ); // some FSR photons allowed
      // count numbers of electrons, muons, and jets.
      const unsigned int nele = count_if( z_daughters.begin() , z_daughters.end() , bind(&AnaTruthParticle::is_electron,_1)==true );
      const unsigned int nmu = count_if( z_daughters.begin() , z_daughters.end() , bind(&AnaTruthParticle::is_muon,_1)==true );
      const unsigned int njet = count_if( z_daughters.begin() , z_daughters.end() , bind(&AnaTruthParticle::is_quark,_1)==true );
      if( nele==4 ) { _event_decay_type = ALL_ELECTRON; }
      else if( nmu==4 ) { _event_decay_type = ALL_MUON; }
      else if( nele==2 && nmu==2 ) { _event_decay_type = ELECTRON_MUON; }
      else if( nele==2 && njet>=2 ) { _event_decay_type = ELECTRON_PLUS_JETS; }
      else if( nmu==2 && njet>=2 ) { _event_decay_type = MUON_PLUS_JETS; }
      else if( njet>=4 ) { _event_decay_type = ALL_HADRONIC; }
    }
  }

public:
  AnaTruthTable() : _next_vertex_gid(0) {}
  virtual ~AnaTruthTable() {}

  // index must be unique amongst all particles in the table
  template<typename linkCollIterT>
  void add_particle( const index_type& barcode , 
                     const int& pdg_id , const AnaTruthParticle::ftype& charge , const int& status ,
                     const TLorentzVector& momentum , 
                     const linkCollIterT& iparent , const linkCollIterT& fparent ,
                     const linkCollIterT& ichild , const linkCollIterT& fchild ) {
    // ignore particles with no specified relationship to other
    // particles. a particle with no mothers or daughters is likely an
    // intermediate MC internal state. in any case, it is not
    // interesting.
    if( ichild==fchild && iparent==fparent ) { return; }
    // if particle barcode has already been seen, use that. barcode==0
    // is a special code, however.
    // if( barcode == 0 ) { return; }
    truth_ptr ptr;
    truth_particle_coll::iterator iptr = _truth_particles.find( barcode );
    if( iptr != _truth_particles.end() ) {
      ptr = iptr->second;
    } else {
      ptr = boost::make_shared<AnaTruthParticle>();
      ptr->_momentum = momentum;
      ptr->_charge = charge;
      ptr->_pdg_id = pdg_id;
      ptr->_status = status;
      ptr->_particle_event_index = barcode;
    }
    add_particle( ptr , iparent , fparent , ichild , fchild );
  }

  template<typename linkCollIterT>
  void add_particle( const boost::shared_ptr<AnaTruthParticle>& specified_ptr ,
                     const linkCollIterT& iparent , const linkCollIterT& fparent ,
                     const linkCollIterT& ichild , const linkCollIterT& fchild ) {
    // ignore particles with no specified relationship to other
    // particles. a particle with no mothers or daughters is likely an
    // intermediate MC internal state. in any case, it is not
    // interesting.
    if( !specified_ptr ) { return; }
    if( ichild==fchild && iparent==fparent ) { return; }
    truth_particle_coll::iterator iptr = _truth_particles.find( specified_ptr->index() );
    truth_ptr ptr;
    if( iptr != _truth_particles.end() ) {
      ptr = iptr->second;
    } else {
      ptr = specified_ptr;
      _truth_particles.insert( truth_particle_coll::value_type(ptr->index(),ptr) );
    }
    // add parents and children. ignore self-referential particle 'links'.
    index_type gid( gid_for_barcode( ptr->index() ) );
    for( linkCollIterT i=iparent, f=fparent; i!=f; ++i ) {
      index_type parent_barcode(*i);
      if( parent_barcode == ptr->index() ) { continue; }
      boost::add_edge( gid_for_barcode( parent_barcode ) , gid , _graph );
    }
    for( linkCollIterT i=ichild, f=fchild; i!=f; ++i ) {
      index_type child_barcode(*i);
      if( child_barcode == ptr->index() ) { continue; }
      boost::add_edge( gid , gid_for_barcode( child_barcode ) , _graph );
    }
    _graph[ gid ].ptr = ptr;
  }
  void add_reco_truth_for_kind( const AnaParticle::Type& type , 
                                const boost::shared_ptr<AnaTruthParticle>& ptr ) {
    _reco_truth_for_kind[type].insert( ptr );
  }
  
  const reco_truth_coll& reco_truth_for_kind( const AnaParticle::Type& type ) const {
    static reco_truth_coll null;
    reco_truths::const_iterator i = _reco_truth_for_kind.find( type );
    if( i==_reco_truth_for_kind.end() ) { return null; }
    return( i->second );
  }

  void finalize() {
    // prune redundant paths, etc. to make user operations faster.
    // this particular operation is SLLOOOOW though.
    graph_type tmp;
    boost::copy_graph( _graph , tmp );
    // copy vertex properties. in boost 1.39, this does not appear to happen.
    BGL_FORALL_VERTICES( v , _graph , graph_type ) {
      truth_ptr ptr( _graph[v].ptr );
      tmp[v].ptr = ptr;
    }
    _graph.clear();
    // remove redundant edges between vertices.
    boost::transitive_closure( tmp , _graph );
    // copy vertex properties. in boost 1.39, this does not appear to happen.
    BGL_FORALL_VERTICES( v , tmp , graph_type ) {
      truth_ptr ptr( tmp[v].ptr );
      _graph[v].ptr = ptr;
    }
    if( false )  { // for debugging
      BGL_FORALL_VERTICES( v , _graph , graph_type ) {
        truth_ptr ptr( _graph[v].ptr );
        std::cout << v << std::endl;
        if( !ptr ) { continue; }
        std::cout << "AnaTruthTable: finalized [particle " << v
                  << "] [pdg " << ptr->pdg_name()
                  << "] [pt " << ptr->pt()
                  << "] [eta " << ptr->eta() 
                  << "] [phi " << ptr->phi() 
                  << "] [mass " << ptr->mass() 
                  << "]" << std::endl;
      }
    } // end debugging
    // check for any particles which have no parent. these should
    // include the incoming protons but explicitly exclude copies of
    // the main decay tree that have been used to record showering
    // information.  for example, pythia ttbar events will have a
    // second ww pair with FSR corrections (at least, that is what it
    // looks like).
    if( false ) { 
      std::vector< vertex_type > excluded_vertices;
      BGL_FORALL_VERTICES( v , _graph , graph_type ) {
        truth_ptr ptr( _graph[v].ptr );
        if( !ptr ) { continue; }
        if( in_degree( v , _graph) == 0 && !(ptr->is_incoming_proton()) ) { 
          excluded_vertices.push_back( v ); 
        }
      }
      // remove all particles descendend from the excluded vertices and
      // those vertices themselves.
      while( !(excluded_vertices.empty()) && num_vertices(_graph)!=0 ) {
        boost::graph_traits<graph_type>::vertex_iterator vi, vi_end;
        for( boost::tie(vi, vi_end) = vertices(_graph); vi!=vi_end; ++vi ) {
          std::vector< vertex_type >::iterator iex = std::find( excluded_vertices.begin() , excluded_vertices.end() , *vi );
          if( iex != excluded_vertices.end() ) {
            excluded_vertices.erase( iex );
            remove_vertex( *vi , _graph );
            break;
          }
        }
      }
      assert( excluded_vertices.empty() );
    }
    // construct vertex reachability cache. this
    // is a list of pairs denoting all (u,v) for which there is a path
    // u->v along out-edges in the graph.
    dfs_reachability_visitor< vertex_type , edge_type , std::insert_iterator<vertex_reachability> > 
      rc_visitor( std::inserter(_vertex_reachability_cache,_vertex_reachability_cache.begin()) );
    boost::depth_first_search( _graph , boost::visitor( rc_visitor ) );
    if( false ) {
      BOOST_FOREACH( const edge_type& e , _vertex_reachability_cache ) {
        std::cout << " vertex " << e.second << " descends from " << e.first << std::endl;
      }
    }
    // refresh particle types caches
    _refresh_all_final_states();
    _refresh_all_heavy_bosons();
    _refresh_all_quarks_or_gluons();
    _refresh_event_type();
  }

  void cleanup() {
    // remove all vertices of degree zero. removing one vertex
    // invalidates all iterators. slooow.
    using namespace boost;
    graph_traits<graph_type>::vertex_iterator vi, vi_end, next;
    tie(vi, vi_end) = vertices(_graph);
    while( vi!=vi_end ) {
      tie(vi, vi_end) = vertices(_graph);
      bool did_remove = false;
      for( ; vi!=vi_end; ++vi ) {
        if( (out_degree(*vi,_graph)+in_degree(*vi,_graph)) == 0 ) {
          remove_vertex( *vi , _graph );
          did_remove = true;
          break;
        }
      }
      if( did_remove ) { continue; }
      break;
    }
  }

  void write_graphviz( const std::string& filename ) { 
    //cleanup();
    ofstream outf( filename.c_str() );
    boost::write_graphviz( outf , _graph , truth_table_vertex_writer<graph_type,truth_ptr>(_graph) , truth_table_edge_writer<graph_type,truth_ptr>(_graph) );
    outf.flush();
    outf.close();
  }

  void display_decay_tree() const {
    // list all vertices and edges
    return;
    typedef std::list<vertex_type> topo_order;
    topo_order ordering;
    boost::topological_sort( _graph , std::front_inserter( ordering ) );
    std::cout << " TRUTH PARTICLE DECAY TABLE (topological sort) " << std::endl;
    std::cout << boost::format( "%|5s| %|5s| %|5s|" ) % "id" % "i" % "name" << std::endl;
    // std::cout << boost::format( "%|5s| %|5s| %|5s| %|5s| %|5s| %|5s| " ) % "id" % "i" % "ip1" % "ip2" % "ic1" % "ic2" << std::endl;
    BOOST_FOREACH( vertex_type v , ordering ) {
      // truth_map::const_iterator iv = _map_particle.find( v ); assert( iv!=_map_particle.end() );
      // const truth_ptr& tp( iv->second );
      const truth_ptr& tp( _graph[v].ptr );
      std::cout << ( boost::format( "%|5d| %|5d| %|5s| " )  
      // std::cout << ( boost::format( "%|5d| %|5d| %|5d| %|5d| %|5d| %|5d| " ) 
                     % tp->pdg_id() 
                     % tp->index() 
                     % tp->pdg_name()
                     // % (*(tp->begin_parent()))
                     // % (*++(tp->begin_parent()))
                     // % (*(tp->begin_child()))
                     // % (*++(tp->begin_child()))
                     ) << std::endl;
    }
  }

  std::ostream& print( std::ostream& os , const int level = 3 ) const {
    using namespace boost;
    if( level<1 ) { return os; }
    os << "AnaTruthTable:   particles in truth table: " << num_vertices(_graph) << std::endl;
    os << "AnaTruthTable: connections in truth table: " << num_edges(_graph) << std::endl;
    for( reco_truths::const_iterator i=_reco_truth_for_kind.begin(), f=_reco_truth_for_kind.end(); i!=f; ++i ) {
      os << "AnaTruthTable: reconstruction counterparts in truth table for type " << i->first << ": " << i->second.size() << std::endl;
    }
    if( level>1 ) {
      int num = 0;
      BGL_FORALL_VERTICES( v , _graph , graph_type ) {
        truth_ptr ptr( _graph[v].ptr );
        if( !ptr ) { continue; }
        os << "AnaTruthTable: [particle " << num
           << "] [pdg " << ptr->pdg_name()
           << "] [pt " << ptr->pt()
           << "] [eta " << (ptr->pt()>0 ? ptr->eta() : 0)
           << "] [phi " << ptr->phi() 
           << "] [mass " << ptr->mass() 
           << "]" << std::endl;
        ++num;
      }
    }
    if( level>2 ) { 
      BGL_FORALL_EDGES(e, _graph, graph_type) {
        truth_ptr source_ptr = _graph[ boost::source(e,_graph) ].ptr;
        truth_ptr target_ptr = _graph[ boost::target(e,_graph) ].ptr;
        unsigned long source_index = source_ptr ? source_ptr->index() : -1;
        unsigned long target_index = target_ptr ? target_ptr->index() : -1;
        const std::string source_name = source_ptr ? source_ptr->pdg_name() : "";
        const std::string target_name = target_ptr ? target_ptr->pdg_name() : "";
        os << ( boost::format("AnaTruthTable: connection [id %|d| -> %|d|] [name %|s| %|s|]") 
                % source_index % target_index
                % source_name % target_name )
           << std::endl;
      }
    }
    if( level>1 ) {
      os << "rec. counterparts: " << std::endl;
      for( reco_truths::const_iterator i=_reco_truth_for_kind.begin(), f=_reco_truth_for_kind.end(); i!=f; ++i ) {
        for( reco_truth_coll::const_iterator j=i->second.begin(), jf=i->second.end(); j!=jf; ++j ) { 
          truth_ptr ptr( *j );
          if( !ptr ) { continue; }
          os << " [type  " << i->first << "] ";
          ptr->print(os);
        }
      }
    }
    return os;
  }

  std::ostream& print_cdf_style( std::ostream& os ) const {
    using namespace boost;
    os << ( boost::format( "%|5s| %|5s| %|7t|%|5s| %|13t|%|9s| %|23t|%|9s| %|33t|%|9s| %|43t|%|9s| %|53t|%|5s| %|58t|%|6s| %|65t|%|6s| %|72t|%|6s| %|79t|%|6s|" )
            % "ID" % "GID" % "PDG"     % "PT"      % "ETA"     % "PHI"      % "M"     % "STAT"   % "MO1"      % "MO2"     % "DA1" %    "DA2" ) << std::endl;
    std::map< index_type , std::string > table_entries;
    BGL_FORALL_VERTICES( v , _graph , graph_type ) {
      truth_ptr ptr( _graph[v].ptr );
      if( !ptr ) { continue; }
      // get mother and daughter barcodes
      std::vector<index_type> mos;
      BGL_FORALL_INEDGES( v , e , _graph , graph_type ) {
        mos.push_back( barcode_for_gid( boost::source(e,_graph) ) );
      }
      std::vector<index_type> das;
      BGL_FORALL_OUTEDGES( v , e , _graph , graph_type ) {
        das.push_back( barcode_for_gid( boost::target(e,_graph) ) );
      }
      table_entries[ ptr->index() ] = ( boost::format( "%|5d| %|5d| %|7t|%|5d| %|13t|%|9.2f| %|23t|%|9.2f| %|33t|%|9.2f| %|43t|%|9.2f| %|53t|%|5d| %|58t|%|6d| %|65t|%|6d| %|72t|%|6d| %|79t|%|6d|" )
                                        % ptr->index() % v
                                        % ptr->pdg_id() % ptr->pt() % (ptr->pt()>0. ? ptr->eta() : 0.) % ptr->phi() % ptr->mass() % ptr->status() 
                                        % (mos.empty() ? -1 : static_cast<int>(mos.front()))
                                        % (mos.size()<2 ? -1 : static_cast<int>(mos[1]))
                                        % (das.empty() ? -1 : static_cast<int>(das.front())) 
                                        % (das.size()<2 ? -1 : static_cast<int>(das[1])) ).str();
    }
    for( std::map< index_type , std::string >::iterator i=table_entries.begin(), f=table_entries.end(); i!=f; ++i ) {
      os << i->second << std::endl;
    }
    return os;
  }

  std::ostream& print_gprof_style( std::ostream& os ) const {
    using namespace boost;
    // display graph connections vertex by vertex, listing all immediate parents and children for each vertex.
    BGL_FORALL_VERTICES( v , _graph , graph_type ) {
      truth_ptr ptr( _graph[v].ptr );
      if( !ptr ) { continue; }
      os << ( boost::format( "%|1s| %|5s| %|5s| %|7t|%|5s| %|13t|%|9s| %|23t|%|9s| %|33t|%|9s| %|43t|%|9s| %|53t|%|5s|" )
              % " " % "ID" % "GID" % "PDG"     % "PT"      % "ETA"     % "PHI"      % "M"     % "STAT" ) << std::endl;
      // get mothers
      BGL_FORALL_INEDGES( v , e , _graph , graph_type ) {
        truth_ptr parent_ptr( _graph[boost::source(e,_graph)].ptr );
        if( parent_ptr ) {
          os << ( boost::format( "%|1s| %|5s| %|5s| %|7t|%|5s| %|13t|%|9s| %|23t|%|9s| %|33t|%|9s| %|43t|%|9s| %|53t|%|5s|" )
                  % "p"
                  % parent_ptr->index() % boost::source(e,_graph)
                  % parent_ptr->pdg_id() % parent_ptr->pt() % (parent_ptr->pt()>0. ? parent_ptr->eta() : 0.) % parent_ptr->phi() % parent_ptr->mass() % parent_ptr->status() )
             << std::endl;
        } else {
          os << "x        " << boost::source(e,_graph) << " (missing)" << std::endl;
        }
      }
      // print this particle
      os << ( boost::format( "%|1s| %|5s| %|5s| %|7t|%|5s| %|13t|%|9s| %|23t|%|9s| %|33t|%|9s| %|43t|%|9s| %|53t|%|5s|" )
              % "*"
              % ptr->index() % v
              % ptr->pdg_id() % ptr->pt() % (ptr->pt()>0. ? ptr->eta() : 0.) % ptr->phi() % ptr->mass() % ptr->status() 
              )
         << std::endl;
      BGL_FORALL_OUTEDGES( v , e , _graph , graph_type ) {
        truth_ptr child_ptr( _graph[boost::target(e,_graph)].ptr );
        if( child_ptr ) {
          os << ( boost::format( "%|1s| %|5s| %|5s| %|7t|%|5s| %|13t|%|9s| %|23t|%|9s| %|33t|%|9s| %|43t|%|9s| %|53t|%|5s|" )
                  % "c"
                  % child_ptr->index() % boost::target(e,_graph)
                  % child_ptr->pdg_id() % child_ptr->pt() % (child_ptr->pt()>0. ? child_ptr->eta() : 0.) % child_ptr->phi() % child_ptr->mass() % child_ptr->status() )
             << std::endl;
        } else {
          os << "x        " << boost::target(e,_graph) << " (missing)" << std::endl;
        }
      }
    }
    return os;
  }


  // characterize event type and particle types
  // ================================================================-

  // is the truth table filled?
  const bool empty() const { return boost::num_vertices(_graph)==0; }

  // type of event, based on number and type of bosons, quarks, etc.
  const EventType& event_type() const { return _event_type; }
  // event decay type; meaning depends on value of event_type. for example,
  // if event_type==ZBOSON, then event_decay_type==ALL_ELECTRON means Z->ee.
  // if event_type==TTBAR, then event_decay_type==ALL_ELECTRON means ttbar->eenunubb
  // etc.
  const EventDecayType& event_decay_type() const { return _event_decay_type; }

  // check if child_particle is (indirect or direct) decay product of
  // parent_particle.
  const bool is_decay_from( const truth_const_ptr& child_particle ,
                            const truth_const_ptr& parent_particle ) const {
    if( !child_particle || !parent_particle ) { return false; }
    if( child_particle==parent_particle ) { return true; }
    const index_type child_gid( gid_for_barcode_check(child_particle->index()) );
    const index_type parent_gid( gid_for_barcode_check(parent_particle->index()) );
    return( _vertex_reachability_cache.find( edge_type(parent_gid,child_gid) ) != _vertex_reachability_cache.end() );
  }

  // get collection of particles of type abs_pdg_id
  template<typename collT> const collT all_of_type( const unsigned int& abs_pdg_id ) const {
    collT result;
    BOOST_FOREACH( const truth_particle_coll::value_type v , _truth_particles ) {
      const truth_ptr& ptr( v.second );
      if( std::abs( ptr->pdg_id() ) != abs_pdg_id ) { continue; }
      result.push_back( ptr );
    }
    return result;
  }

  // get collection of all final state truth particles 
  template<typename collT> const collT all_final_states() const {
    collT result;
    std::copy( _final_states_cache.begin() , _final_states_cache.end() , std::back_inserter(result) );
    return result;
  }
  //template<> const std::vector<truth_ptr> all_final_states() const { return _final_states_cache; }

  // get collection of all final state particles satisfying predicate
  template<typename collT,typename bindT> const collT all_with( bindT predicate ) const {
    collT result;
    BOOST_FOREACH( const truth_particle_coll::value_type v , _truth_particles ) {
      const truth_ptr& ptr( v.second );
      if( !predicate(ptr) ) { continue; }
      result.push_back( ptr );
    }
    return result;
  }

  // get collection of all truth particles 
  template<typename collT> const collT all() const {
    collT result;
    BOOST_FOREACH( const truth_particle_coll::value_type v , _truth_particles ) {
      const truth_ptr& ptr( v.second );
      result.push_back( ptr );
    }
    return result;
  }

  // check if child_particle is (indirect or direct) decay product of 
  // a particle of parent_type.
  const bool is_decay_from_type( const truth_const_ptr& child_particle ,
                                 const unsigned int& abs_pdg_id ) const {
    if( !child_particle ) { return false; }
    if( _truth_particles.find( child_particle->index() ) == _truth_particles.end() ) { return false; }
    const index_type gid( gid_for_barcode_check( child_particle->index() ) );
    BOOST_FOREACH( const edge_type& e , _vertex_reachability_cache ) {
      if( e.second != gid ) { continue; }
      assert( e.second < _next_vertex_gid );
      assert( _graph[ e.second ].ptr );
      if( std::abs( _graph[ e.first ].ptr->pdg_id() ) == abs_pdg_id ) { return true; }
    }
    return false;
  }

  // get collection of all truth particles which are decays from type parent_type
  template<typename collT> const collT all_from_decays_of_type( const unsigned int& abs_pdg_id ) const {
    collT result;
    BOOST_FOREACH( const truth_particle_coll::value_type& v , _truth_particles ) {
      const truth_ptr& ptr( v.second );
      // if( std::abs(ptr->pdg_id()) == abs_pdg_id ) { continue; } // skip particles which have decayed from themselves 
      if( !is_decay_from_type( ptr , abs_pdg_id ) ) { continue; }
      result.push_back( ptr );
    }
    return result;
  }

  // get collection of all truth particles which are decays from type parent_type
  template<typename collT, typename bindT> const collT all_from_decays_of_type( const unsigned int& abs_pdg_id , 
                                                                                bindT predicate ) const {
    collT result;
    BOOST_FOREACH( const truth_particle_coll::value_type& v , _truth_particles ) {
      const truth_ptr& ptr( v.second );
      if( !predicate( ptr ) ) { continue; }
      if( !is_decay_from_type( ptr , abs_pdg_id ) ) { continue; }
      result.push_back( ptr );
    }
    return result;
  }

  // identify distinct heavy (e.g. W,Z,graviton) bosons in the decay
  // tree.  criteria is to pick boson which has no out-edges to
  // another boson of the same type.
  template<typename collT> const collT all_heavy_bosons() const {
    collT result;
    std::copy( _heavy_bosons_cache.begin() , _heavy_bosons_cache.end() , std::back_inserter(result) );
    return result;
  }
  //template<> const std::vector<truth_ptr> all_heavy_bosons() const { return _heavy_bosons_cache; }

  // count number of heavy bosons in the event
  const unsigned int num_w_bosons() const {
    return std::count_if( _heavy_bosons_cache.begin() , _heavy_bosons_cache.end() , 
                          boost::bind( &AnaTruthParticle::is_w_boson , _1 ) == true );
  }
  const unsigned int num_z_bosons() const {
    return std::count_if( _heavy_bosons_cache.begin() , _heavy_bosons_cache.end() , 
                          boost::bind( &AnaTruthParticle::is_z_boson , _1 ) == true );
  }

  // identify distinct quarks or gluons in the decay
  // tree. criteria is to pick particle which has no out-edges to
  // another particle of the same type.
  template<typename collT> const collT all_quarks_or_gluons() const {
    collT result;
    std::copy( _quarks_or_gluons_cache.begin() , _quarks_or_gluons_cache.end() , std::back_inserter(result) );
    return result;
  }
  //template<> const std::vector<truth_ptr> all_quarks_or_gluons() const { return _quarks_or_gluons_cache; }
  const unsigned int num_final_b_quarks() const {
    return std::count_if( _quarks_or_gluons_cache.begin() , _quarks_or_gluons_cache.end() , 
                          boost::bind( &AnaTruthParticle::is_bottom_quark , _1 ) == true &&
                          boost::bind( &AnaTruthParticle::status , _1 ) == 2 );
  }
  const unsigned int num_t_quarks() const {
    return std::count_if( _quarks_or_gluons_cache.begin() , _quarks_or_gluons_cache.end() , 
                          boost::bind( &AnaTruthParticle::is_top_quark , _1 ) == true );
  }
  
};

BOOST_CLASS_VERSION( AnaTruthTable , 1 );
// don't define a separate version for vertex_type

#endif // ANATRUTHTABLE_HPP

