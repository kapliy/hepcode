#ifndef WRAP_ANACANDIDATEEVENT_HPP
#define WRAP_ANACANDIDATEEVENT_HPP

// class used to record lists of selected objects, trigger bits, etc.
// in order to pass them to a study. the information provided is
// generic on purpose; the studies which operate on this class should not assume the 'electrons' provided are specific

#include <boost/shared_ptr.hpp>
#include <boost/range/iterator_range.hpp>
#include "TrigFTKAna/EventObjectColl.hpp"

// class AnaElectron;
// class AnaMuon;
// class AnaJet;
class AnaMET;
class AnaTrigger;
class AnaTruthTable;

class
AnaCandidateEvent
{
public:
  typedef EventObjectColl< boost::shared_ptr<const AnaParticle> >::const_iterator const_particle_iterator;
  typedef boost::iterator_range<const_particle_iterator> const_particle_iterator_range;
  typedef EventObjectColl< boost::shared_ptr<const AnaElectron> >::const_iterator const_electron_iterator;
  typedef boost::iterator_range<const_electron_iterator> const_electron_iterator_range;
  typedef EventObjectColl< boost::shared_ptr<const AnaMuon> >::const_iterator const_muon_iterator;
  typedef boost::iterator_range<const_muon_iterator> const_muon_iterator_range;
  typedef EventObjectColl< boost::shared_ptr<const AnaJet> >::const_iterator const_jet_iterator;
  typedef boost::iterator_range<const_jet_iterator> const_jet_iterator_range;
  typedef EventObjectColl< boost::shared_ptr<const AnaVertex> >::const_iterator const_vertex_iterator;
  typedef boost::iterator_range<const_vertex_iterator> const_vertex_iterator_range;
private:
  AnaCandidateEvent::const_electron_iterator_range _electrons;
  AnaCandidateEvent::const_muon_iterator_range _muons;
  AnaCandidateEvent::const_jet_iterator_range _jets;
  AnaCandidateEvent::const_vertex_iterator_range _vertices;
  boost::shared_ptr<const AnaMET> _met;
  boost::shared_ptr<const AnaTrigger> _trigger;
  boost::shared_ptr<const AnaTruthTable> _truth_table;
public:
  AnaCandidateEvent() {}
  virtual ~AnaCandidateEvent() {}

  void electrons( AnaCandidateEvent::const_electron_iterator begin , AnaCandidateEvent::const_electron_iterator end ) { _electrons = boost::make_iterator_range( begin , end ); }
  void muons( AnaCandidateEvent::const_muon_iterator begin , AnaCandidateEvent::const_muon_iterator end ) { _muons = boost::make_iterator_range( begin , end ); }
  void jets( AnaCandidateEvent::const_jet_iterator begin , AnaCandidateEvent::const_jet_iterator end ) { _jets = boost::make_iterator_range( begin , end ); }
  void vertices( AnaCandidateEvent::const_vertex_iterator begin , AnaCandidateEvent::const_vertex_iterator end ) { _vertices = boost::make_iterator_range( begin , end ); }
  void met( const boost::shared_ptr<const AnaMET>& m ) { _met = m; }
  void trigger( const boost::shared_ptr<const AnaTrigger>& t ) { _trigger = t; }

  const AnaCandidateEvent::const_electron_iterator_range electrons() const { return _electrons; }
  const AnaCandidateEvent::const_muon_iterator_range muons() const { return _muons; }
  const AnaCandidateEvent::const_jet_iterator_range jets() const { return _jets; }
  const AnaCandidateEvent::const_vertex_iterator_range vertices() const { return _vertices; }
  const boost::shared_ptr<const AnaMET>& met() const { return _met; }
  const boost::shared_ptr<const AnaTrigger>& trigger() const { return _trigger; }
  const boost::shared_ptr<const AnaTruthTable>& truth_table() const { return _truth_table; }

  const bool empty_electrons() const { return _electrons.empty(); } // is_singular in boost 1.42
  const bool empty_muons() const { return _muons.empty(); }
  const bool empty_jets() const { return _jets.empty(); }
  const bool empty_vertices() const { return _vertices.empty(); }
  const bool empty_met() const { return !_met; }
  const bool empty_trigger() const { return !_trigger; }
  const bool empty_truth_table() const { return !_truth_table; }

  const AnaCandidateEvent::const_electron_iterator begin_electrons() const { return _electrons.begin(); }
  const AnaCandidateEvent::const_electron_iterator end_electrons() const { return _electrons.end(); }
  const AnaCandidateEvent::const_muon_iterator begin_muons() const { return _muons.begin(); }
  const AnaCandidateEvent::const_muon_iterator end_muons() const { return _muons.end(); }
  const AnaCandidateEvent::const_jet_iterator begin_jets() const { return _jets.begin(); }
  const AnaCandidateEvent::const_jet_iterator end_jets() const { return _jets.end(); }
  const AnaCandidateEvent::const_vertex_iterator begin_vertices() const { return _vertices.begin(); }
  const AnaCandidateEvent::const_vertex_iterator end_vertices() const { return _vertices.end(); }
};

#endif // WRAP_ANACANDIDATEEVENT_HPP
