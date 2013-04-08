#ifndef WRAP_EVENTOBJECTCOLL_HPP
#define WRAP_EVENTOBJECTCOLL_HPP

#include <stdexcept>
#include <vector>
#include <algorithm>
#include <iterator>
#include <functional>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/version.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/equal_to.hpp> 
#include <boost/mpl/comparison.hpp>
#include "TrigFTKAna/AnaEventMgr.hpp"

template<typename objectT>
class
EventObjectColl
{
private:
  std::vector<objectT> _coll;
  bool _filled_for_event;
  mutable bool _attached;
private:
  friend class boost::serialization::access;
  template<class archiveT>
  void serialize( archiveT& ar, const unsigned int version ) {
    ar & boost::serialization::make_nvp("coll",_coll);
    ar & boost::serialization::make_nvp("filled_for_event",_filled_for_event);
  }
// private:
//   // define these here for use in push_back_all
//   class identity : public boost::function< objectT ( objectT ) > {
//   public:
//     objectT operator()( objectT it ) const { return it; }
//   };
//   template<typename iteratorValueT>
//   class recast : public boost::function< objectT ( iteratorValueT ) > {
//   public:
//     objectT operator()( iteratorValueT it ) const { return boost::dynamic_pointer_cast<objectT>(it); };
//   };
public:
  typedef typename std::vector<objectT> Coll;
  typedef typename Coll::value_type value_type;
  typedef typename Coll::pointer pointer;
  typedef typename Coll::reference reference;
  typedef typename Coll::const_reference const_reference;
  typedef typename Coll::size_type size_type;
  typedef typename Coll::difference_type difference_type;
  typedef typename Coll::iterator iterator;
  typedef typename Coll::const_iterator const_iterator;

  EventObjectColl()
    : _coll() 
    , _filled_for_event(false)
    , _attached(false) {
    // instruct event manager to clear this collection when loading a new event
    AnaEventMgr::add_begin_event_callback( boost::bind( &EventObjectColl::clear , this ) );
    _coll.clear();
  }
  EventObjectColl( const Coll& coll) 
    : _coll(coll) , 
      _filled_for_event(false) 
    , _attached(false) {
    // instruct event manager to clear this collection when loading a new event
    AnaEventMgr::add_begin_event_callback( boost::bind( &EventObjectColl::clear , this ) );
    _coll.clear();
  }
  virtual ~EventObjectColl() {}

  EventObjectColl( const EventObjectColl& rhs) 
    : _coll(rhs._coll)
    , _filled_for_event(false)
    , _attached(false) {
    throw;
  }
  
  void clear() {
    _coll.clear(); 
    // filled for event should be true on collection initialization, and then
    // it will be set to false for every subsequent clear.
    if( !_filled_for_event ) {
      if( false ) std::cout << "WARNING: EventObjectCollection not filled for this event." << std::endl;
    }
    _filled_for_event = false;
  }

  void erase( iterator i ) { _coll.erase(i); }
  void pop_back() { _coll.pop_back(); }

  // DgCutSet attachment tool
  const boost::function< void ( objectT const& ) > attach() {
    _attached = true;
    return boost::bind( &EventObjectColl::push_back , this , _1 );
  }

  template<typename sourceIterT,typename cutsetT>
  void fill_for_each_if( sourceIterT begin , sourceIterT end , const cutsetT& cutset ) {
    copy_if( begin , end , std::back_inserter(*this) , bind( &cutsetT::passes_all , cutset , _1 ) == true );
    _filled_for_event = true;
  }

  // template<typename iteratorT> iteratorT begin() const { return _coll.begin(); }
  // template<typename iteratorT> iteratorT end() const { return _coll.end(); }
  iterator begin() { return _coll.begin(); }
  iterator end() { return _coll.end(); }
  const_iterator cbegin() const { return _coll.begin(); }
  const_iterator cend() const { return _coll.end(); }
  const size_type size() const { return _coll.size(); }
  const objectT& back() const { return _coll.back(); }
  const objectT& front() const { return _coll.front(); }
  const bool empty() const { return _coll.empty(); }
  const bool contains( const objectT& obj ) const {
    return( std::find(_coll.begin(),_coll.end(),obj) != _coll.end() );
  }
  void push_back( const objectT& obj ) { 
    _coll.push_back( obj ); 
    _filled_for_event = true;
  }
  template<typename iteratorT> void push_back_all( iteratorT begin , iteratorT end ) {
    // screw it. no MPL, just call dynamic cast on everything.
    typedef typename std::iterator_traits<iteratorT>::value_type iterator_value_type;
    for( iteratorT i=begin; i!=end; ++i ) { 
      _coll.push_back( boost::dynamic_pointer_cast<typename objectT::element_type>(*i) );
    }
    _filled_for_event = true;
    // // assumes iteratorT points to shared_ptrs. if the shared_ptrs
    // // don't point to types of objectT, but rather of some other type,
    // // we'll need to cast them to objectT. the following code checks
    // // for this case and defines a "converter" functor that is either
    // // the identity or the cast. if your code fails to compile here,
    // // check that your iterators point to a collection of shared_ptrs
    // // and that the shared_ptrs can be recast to objectT.
    // using namespace boost;
    // using namespace boost::mpl;
    // typedef typename std::iterator_traits<iteratorT>::value_type iterator_value_type;
    // typedef typename if_< equal_to<iterator_value_type,objectT> , identity , recaster<iterator_value_type> >::type optional_recast_pointer;
    // std::transform( begin , end , std::back_inserter( _coll ) , boost::bind( optional_recast_pointer() , ::_1 ) );
    //std::copy( begin , end , std::back_inserter( _coll ) );
  }
  std::back_insert_iterator< Coll > back_inserter() { return std::back_inserter(_coll); }

  objectT const& operator[]( const unsigned int& index ) { return _coll[index]; }

  template< typename predicateT >
  const bool contains_if( predicateT op ) const {
    return( std::find_if(_coll.begin(),_coll.end(),op) != _coll.end() );
  }

  template< typename binaryPredicateT >
  const bool is_minimum( const objectT& obj , binaryPredicateT lessThan ) const {
    const_iterator i = std::min_element( _coll.begin() , _coll.end() , lessThan );
    if( i==_coll.end() ) { return false; }
    return( *i == obj );
  }

  template< typename binaryPredicateT , typename predicateT >
  const bool is_minimum_and( const objectT& obj , binaryPredicateT lessThan , predicateT requirement ) const {
    const_iterator i = std::min_element( _coll.begin() , _coll.end() , lessThan );
    if( i==_coll.end() ) { return false; }
    return( requirement(*i) );
  }

  template< typename transformT >
  typename transformT::result_type minimum( transformT op ) const {
    typename transformT::result_type result;
    if( _coll.result() ) { return result; }
    for( const_iterator i=_coll.begin(), f=_coll.end(); i!=f; ++i ) {
      if( i==_coll.begin() ) { result = op(*i); continue; }
      typename transformT::result_type tmp( op(*i) );
      result = std::min( tmp , result );
    }
    return result;
  }

  template< typename transformT >
  typename transformT::result_type maximum( transformT op ) const {
    typename transformT::result_type result;
    if( _coll.result() ) { return result; }
    for( const_iterator i=_coll.begin(), f=_coll.end(); i!=f; ++i ) {
      if( i==_coll.begin() ) { result = op(*i); continue; }
      typename transformT::result_type tmp( op(*i) );
      result = std::max( tmp , result );
    }
    return result;
  }

  template< typename transformT , typename lessThanBinaryT >
  typename transformT::result_type minimum( transformT op , lessThanBinaryT lt ) const {
    typename transformT::result_type result;
    if( _coll.result() ) { return result; }
    for( const_iterator i=_coll.begin(), f=_coll.end(); i!=f; ++i ) {
      if( i==_coll.begin() ) { result = op(*i); continue; }
      typename transformT::result_type tmp( op(*i) );
      if( lt(tmp,result) ) { 
        result = tmp;
      }
    }
    return result;
  }

};

namespace boost { 
  namespace serialization {
    template<class T>
    struct version< EventObjectColl<T> >
    {
      BOOST_STATIC_CONSTANT(unsigned int, value = 1); // BOOST_CLASS VERSION == value
    };
  } // namespace serialization
} // namespace boost

#endif // WRAP_EVENTOBJECTCOLL_HPP
