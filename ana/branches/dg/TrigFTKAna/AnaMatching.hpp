#ifndef WRAP_ANAMATCHING_HH
#define WRAP_ANAMATCHING_HH

#include <vector>

// class which represents a match between a "reference" object
// (e.g. a truth track) to one or more "comparison" objects (e.g. FTK tracks).

template<typename refObjT,typename compObjT = refObjT>
class
AnaMatch
{
  typedef refObjT RefType;
  typedef compObjT CompType;
private:
  RefType _reference;
  std::vector<CompType> _matches;
public:
  AnaMatch( RefType const& reference , const std::vector<CompType>& matches ) 
    : _reference( reference )
    , _matches( matches )
  {} 
  AnaMatch( RefType const& reference ) 
    : _reference( reference )
    , _matches()
  {} 
  void add_match( CompType const& obj ) { _matches.push_back(obj); }
  const RefType& reference() const { return _reference; }
  const unsigned int num_matches() const { return _matches.size(); }
  const CompType& match( const unsigned int& i ) const { return _matches[i]; }
};


// // base class outlining the procedure for matching tracks from a "reference" collection
// // (e.g. truth tracks) to a "comparison" collection (e.g. FTK tracks).
// // 

// class
// AnaMatchingAlgorithm
// {
// public:
//   AnaMatchingAlgorithm() {}
//   virtual ~AnaMatchingAlgorithm() {}

//   template< typename objRefInputIteratorT , typename objCompInputIteratorT , typename objOutputIteratorT >
//   void match( objRefInputIteratorT begin_reference , objRefInputIteratorT end_reference ,
//               objCompInputIteratorT begin_comparison , objCompInputIteratorT end_comparison ,
//               objOutputIteratorT output_iterator );
// };

class
AnaGeantMatchingAlgorithm// : public AnaMatchingAlgorithm
{
public:
  AnaGeantMatchingAlgorithm() {}
  virtual ~AnaGeantMatchingAlgorithm() {}

  template< typename objRefInputIteratorT , typename objCompInputIteratorT , typename objOutputIteratorT >
  void match( objRefInputIteratorT begin_reference , objRefInputIteratorT end_reference ,
              objCompInputIteratorT begin_comparison , objCompInputIteratorT end_comparison ,
              objOutputIteratorT output_iterator );
};

class
AnaGuidoMatchingAlgorithm// : public AnaMatchingAlgorithm
{
public:
  AnaGuidoMatchingAlgorithm() {}
  virtual ~AnaGuidoMatchingAlgorithm() {}

  template< typename objRefInputIteratorT , typename objCompInputIteratorT , typename objOutputIteratorT >
  void match( objRefInputIteratorT begin_reference , objRefInputIteratorT end_reference ,
              objCompInputIteratorT begin_comparison , objCompInputIteratorT end_comparison ,
              objOutputIteratorT output_iterator );
};

#endif // WRAP_ANAMATCHING_HH
