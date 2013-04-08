
#ifndef DGSTL_HPP
#define DGSTL_HPP

#include <iterator>
#include <algorithm>
#include <boost/shared_ptr.hpp>
#include <boost/regex.hpp>

namespace 
dgSTL
{
  // copy_if, missing from STL standard.
  template<class inputIterator,class outputIterator,class unaryPredicate>
  outputIterator
  copy_if( inputIterator first , inputIterator last , outputIterator result , unaryPredicate predicate ) {
    while( first!=last ) {
      if( predicate(*first) ) { *result++ = *first; }
      ++first;
    }
    return result;
  }

  // transform_if - selectively transform elements
  template<class inputIterator,class outputIterator,class unaryOperator,class unaryPredicate>
  outputIterator
  transform_if( inputIterator first , inputIterator last , outputIterator result , unaryOperator op , unaryPredicate predicate ) {
    while( first!=last ) {
      if( predicate(*first) ) { 
        *result++ = op(*first);
      }
      ++first;
    }
    return result;
  }

  // dynamic cast transform of shared ptrs
  template<class newPointerT,class inputIterator,class outputIterator>
  outputIterator
  transform_dynamic_cast( inputIterator first , inputIterator last , outputIterator result ) {
    while( first!=last ) {
      boost::shared_ptr< newPointerT > newp( boost::dynamic_pointer_cast<newPointerT>(*first) );
      if( newp ) {
        *result++ = newp;
      }
      ++first;
    }
    return result;
  }

  // min_element_if with default comparison operator
  template < class ForwardIterator, class Predicate >
  ForwardIterator
  min_element_if(ForwardIterator first,
		 ForwardIterator last,
		 Predicate pred)
  {
    ForwardIterator result = first = std::find_if(first, last, pred);
    if (first != last)
      while (++first != last)
	if (*first < *result && pred(*first))
	  result = first;
    return result;
  }
  
  // min_element_if with custom comparison operator
  template < class ForwardIterator, class Compare, class Predicate >
  ForwardIterator
  min_element_if(ForwardIterator first,
		 ForwardIterator last,
		 Compare comp,
		 Predicate pred)
  {
    ForwardIterator result = first = std::find_if(first, last, pred);
    if (first != last)
      while (++first != last)
	if (comp(*first, *result) && pred(*first))
	  result = first;
    return result;
  }

  // max_element_if with default comparison operator
  template < class ForwardIterator, class Predicate >
  ForwardIterator
  max_element_if(ForwardIterator first,
		 ForwardIterator last,
		 Predicate pred)
  {
    ForwardIterator result = first = std::find_if(first, last, pred);
    if (first != last)
      while (++first != last)
	if (*result < *first && pred(*first))
	  result = first;
    return result;
  }

  // max_element_if with custom comparison operator  
  template < class ForwardIterator, class Compare, class Predicate >
  ForwardIterator
  max_element_if(ForwardIterator first,
		 ForwardIterator last,
		 Compare comp,
		 Predicate pred)
  {
    ForwardIterator result = first = std::find_if(first, last, pred);
    if (first != last)
      while (++first != last)
	if (comp(*result, *first) && pred(*first))
	  result = first;
    return result;
  }

  // min_max_element with default comparison operators
  template < class ForwardIterator >
  std::pair< ForwardIterator, ForwardIterator >
  min_max_element(ForwardIterator first, ForwardIterator last)
  {
    typedef std::pair< ForwardIterator, ForwardIterator > FP;
    FP result(first, first);
    if (first != last)
      while (++first != last) {
	if (*first < *result.first)
	  result.first = first;
	if (*result.second < *first)
	  result.second = first;
      }
    return result;
  }
  
  // min_max_element with custom comparison operators
  template < class ForwardIterator, class CompareMin, class CompareMax >
  std::pair< ForwardIterator, ForwardIterator >
  min_max_element(ForwardIterator first,
		  ForwardIterator last,
		  CompareMin comp_min,
		  CompareMax comp_max)
  {
    typedef std::pair< ForwardIterator, ForwardIterator > FP;
    FP result(first, first);
    if (first != last)
      while (++first != last) {
	if (comp_min(*first, *result.first))
	  result.first = first;
	if (comp_max(*result.second, *first))
	  result.second = first;
      }
    return result;
  }
  
  // abs(v1-v2) that can be used with boost::bind(abs_minus<IN1,IN2,OUT>,_1,_2)
  template<typename IN, typename OUT>
  OUT abs_minus(IN v1, IN v2) {
    return static_cast<OUT>(fabs(v1-v2));
  }

  // abs(v1-v2)/v3 that can be used with boost::bind(abs_minus<IN1,IN2,IN3,OUT>,_1,_2,_3)
  template<typename IN, typename OUT>
  OUT abs_minus_over(IN v1, IN v2, IN v3) {
    return static_cast<OUT>(fabs(v1-v2)/v3);
  }

  // functions to find a common head and a common tail of two strings.
  // e.g. if strings a = "onetwothreesix" and b = "onefourfivesix"
  // then the common head is "one" and the common tail is "six".
  template< typename stringT >
  const stringT common_head( const stringT& a , const stringT& b ) {
    stringT result;
    for( unsigned int i=0, fa=a.size(), fb=b.size(); i!=fa && i!=fb; ++i ) {
      if( a[i]==b[i] ) { result += a[i]; }
    }
    return result;
  }
  template< typename stringT >
  const stringT common_tail( const stringT& a , const stringT& b ) {
    stringT ra(a) , rb(b);
    std::reverse( ra.begin() , ra.end() );
    std::reverse( rb.begin() , rb.end() );
    return( common_head(ra,rb) );
  }

  // several common casts of abs functor
  static float (*absF)(float) = &std::abs;
  static double (*absD)(double) = &std::abs;
  static int (*absI)(int) = &std::abs;

  // escape string for use in boost::regex
  //  based on http://stackoverflow.com/questions/1252992/how-to-escape-a-string-for-use-in-boost-regex
  template<class stringT>
  stringT escape_for_regex( const stringT& string_to_escape ) {
    static const boost::regex re_boostRegexEscape( "[\\^\\.\\$\\|\\(\\)\\[\\]\\*\\+\\?\\/\\\\]" );
    const stringT rep( "\\\\\\1&" );
    stringT result( boost::regex_replace( string_to_escape , re_boostRegexEscape , rep, boost::match_default | boost::format_sed ) );
    return result;
  }

};

#endif // DGSTL_HPP
