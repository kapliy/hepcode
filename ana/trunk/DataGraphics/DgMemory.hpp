 
#ifndef DGMEMORY_HPP
#define DGMEMORY_HPP

#include <cstddef> // size_t
#include <string>
#include <stdexcept>
#include <cassert>
#include <iostream>
#include <list>
#include <boost/shared_ptr.hpp>

// heap memory manager which provides some valgrind-like
// functionality.  much faster than valgrind because it is
// compiled-in.  initial design borrowed from M. Dickheiser ISBN
// 1584504528 (2007).

// if DGMEMORY_DEBUG is not defined, various safeguards will be
// removed to improve performance.
#define DGMEMORY_DEBUG
// if DGMEMORY_OFF is defined, the entire class will disappear,
// replaced by the c++ memory manager
//#undef DGMEMORY_OFF
#define DGMEMORY_OFF

// ================================================================

#ifndef DGMEMORY_OFF

class DgHeap;

class
DgAllocHeader
{
public:
#ifdef DGMEMORY_DEBUG
  static const int HEADER_SIGNATURE;
  static const int END_SIGNATURE;
  static const char FREE_SIGNATURE;
#endif
private:
#ifdef DGMEMORY_DEBUG
  int _signature;
#endif
  int _size;
  DgHeap* _heap;
#ifdef DGMEMORY_DEBUG
  // custom linked list. std::list allocates heap 
  DgAllocHeader* _next;
  DgAllocHeader* _prev;
  unsigned long _allocation_number;
#endif
private:
  DgAllocHeader() {} // not for public use
  friend class DgHeap;
  friend void* operator new( std::size_t size , DgHeap* heap );
public:
#ifdef DGMEMORY_DEBUG
  inline void set_signature() { _signature = HEADER_SIGNATURE; }
  inline const bool check_signature() { return( _signature == HEADER_SIGNATURE ); }
  inline const unsigned long& allocation_number() const { return _allocation_number; }
#endif
  inline DgHeap* heap() { return _heap; }
  inline const int& size() const { return _size; }
};

// ================================================================

class
DgHeap
{
public:
  typedef enum { NONE=0 , SUMMARY=1 , INDIVIDUAL=2 , INDIVIDUAL_WITH_DATA=3 } LeakDetailLevel;
private:
  const char* _name;
  unsigned long _total_size;
  unsigned long _total_alloc;
  DgAllocHeader* _list_head; // front of doubly linked list of alloc headers
  DgAllocHeader* _list_tail; // back of doubly linked list of alloc headers
  unsigned long _next_allocation_number;
private:
  DgHeap( const char* name ) 
  : _name(name)
  , _total_size(0ul)
  , _total_alloc(0ul)
  , _list_head(0)
  , _list_tail(0)
  , _next_allocation_number(0ul)
  {}
  friend class DgMemoryManager;
  inline void add( DgAllocHeader* header ) {
    _total_size += header->size();
    ++_total_alloc;
    header->_allocation_number = _next_allocation_number++;
    header->_prev = 0;
    header->_next = 0;
    if( !_list_head || !_list_tail ) {
      assert( _total_alloc == 1 );
      assert( !_list_head );
      assert( !_list_tail );
      _list_head = header;
      _list_tail = header;
    } else {
      // add to the end of the list
      assert( _total_alloc >= 1 );
      assert( _list_tail );
      assert( _list_head );
      _list_tail->_next = header;
      header->_prev = _list_tail;
      _list_tail = header;
      header->_next = 0;
    }
  }
  inline void remove( DgAllocHeader* header ) {
    assert( header );
    _total_size -= header->size();
    --_total_alloc;
    assert( _total_size >= 0 );
    if( _list_head==header && header==_list_tail ) {
      _list_head = _list_tail = 0;
    } else if( _list_head==header ) { 
      _list_head=header->_next;
    } else if( _list_tail==header ) {
      _list_tail=header->_prev; 
    }
    if( header->_prev ) { header->_prev->_next = header->_next; }
    if( header->_next ) { header->_next->_prev = header->_prev; }
    assert( !(_list_head==0 ^ _list_tail==0) );
  }
#ifndef DGMEMORY_OFF
  friend void* operator new( std::size_t size , DgHeap* heap );
  friend void operator delete( void* buffer );
#endif
public:
  const char* name() const { return _name; }
  const unsigned long total_size() const { return _total_size; }
  const unsigned long total_allocations() const { return _total_alloc; }
  const unsigned long next_allocation_number() const { return _next_allocation_number; }
  std::ostream& print( std::ostream& os ) const;
  std::ostream& leak_check( std::ostream& os , const LeakDetailLevel level = NONE , unsigned long bookmark_allocation = 0 ) const;
};

// ================================================================

class
DgMemoryManager
{
public:
  class bad_alloc : public std::runtime_error {
  public: 
    explicit bad_alloc(const std::string& desc) : std::runtime_error(desc) {}
  };
  class bad_dealloc : public std::runtime_error {
  public: 
    explicit bad_dealloc(const std::string& desc) : std::runtime_error(desc) {}
  };
private:
  static DgHeap _global_heap;
  static std::list< boost::shared_ptr<DgHeap> > _heaps;
public:
  static DgHeap* global_heap() { return &_global_heap; }
  static DgHeap* create_heap( const char* name ) {
    _heaps.push_back( boost::shared_ptr<DgHeap>( new DgHeap(name) ) );
    return _heaps.back().get();
  }
  static const unsigned long num_bytes_allocated();
  static const unsigned long num_allocations();
  static std::ostream& print( std::ostream& os );
  static std::ostream& leak_check( std::ostream& os , const DgHeap::LeakDetailLevel level = DgHeap::NONE );
};

// ================================================================

void* operator new( std::size_t size , DgHeap* heap );
void* operator new[]( std::size_t size , DgHeap* heap );
void* operator new( std::size_t size );
void* operator new[]( std::size_t size );
void operator delete( void* buffer );

#else  // #ifndef DGMEMORY_OFF

class DgHeap {};

class
DgMemoryManager
{
private:
  static DgHeap _global_heap;
public:
  static DgHeap* global_heap() { return &_global_heap; }
  static DgHeap* create_heap( const char* name ) {
    return &_global_heap;
  }
  static std::ostream& print( std::ostream& os ) {}
  static std::ostream& leak_check( std::ostream& os ) {}
};

#endif // #ifndef DGMEMORY_OFF




// ================================================================
// helper macros for "instrumenting" a class 
// 
// header file:
//
// #include "DataGraphics/DgMemory.hpp"
//
// class Object
// {
//   // class declarations
//   // ...
//   // and then
//   DECLARE_DGHEAP;
// }
//
// source file:
//
// #include <object header file> 
// 
// DEFINE_DGHEAP(Object,"object heap name");
//
#ifndef DGMEMORY_OFF
#define DECLARE_DGHEAP                                          \
public:                                                         \
  static void* operator new( std::size_t size );                \
  static void* operator new[]( std::size_t size );              \
  static void operator delete( void* p );    \
private:                                                        \
  static DgHeap* _static_class_dgheap;

#define DEFINE_DGHEAP(object,name)                                      \
  DgHeap* object::_static_class_dgheap = 0;                             \
  void* object::operator new( std::size_t size )                        \
  {                                                                     \
    if( !_static_class_dgheap ) { _static_class_dgheap = DgMemoryManager::create_heap( name ); } \
    return ::operator new( size , _static_class_dgheap );               \
  }                                                                     \
  void* object::operator new[]( std::size_t size )                      \
  {                                                                     \
    if( !_static_class_dgheap ) { _static_class_dgheap = DgMemoryManager::create_heap( name ); } \
    return ::operator new[]( size , _static_class_dgheap );             \
  }                                                                     \
  void object::operator delete( void* p )            \
  {                                                                     \
    ::operator delete( p );                                             \
  }
#else
#define DECLARE_DGHEAP
#define DEFINE_DGHEAP(a,b)
#endif

/*

// The following code example is taken from the book
//  * "The C++ Standard Library - A Tutorial and Reference"
//  * by Nicolai M. Josuttis, Addison-Wesley, 1999
//  *
//  * (C) Copyright Nicolai M. Josuttis 1999.
//  * Permission to copy, use, modify, sell and distribute this software
//  * is granted provided this copyright notice appears in all copies.
//  * This software is provided "as is" without express or implied
//  * warranty, and with no claim as to its suitability for any purpose.

#include <vector>
#include <limits>
#include <iostream>

   template<class T>
   class DgAlloc {
     public:
       // type definitions
       typedef T        value_type;
       typedef T*       pointer;
       typedef const T* const_pointer;
       typedef T&       reference;
       typedef const T& const_reference;
       typedef std::size_t    size_type;
       typedef std::ptrdiff_t difference_type;

       // rebind allocator to type U
       template<class U>
       struct rebind {
           typedef DgAlloc<U> other;
       };

       // return address of values
       pointer address( reference value ) const {
           return &value;
       }
       const_pointer address( const_reference value ) const {
           return &value;
       }

       // constructors and destructor - nothing to do because the
       // allocator has no state
       DgAlloc() throw() {}
       DgAlloc(const DgAlloc&) throw() {}
       template<class U> DgAlloc( const DgAlloc<U>& ) throw() {}
       ~DgAlloc() throw() {}

       // return maximum number of elements that can be allocated
       size_type max_size () const throw() {
         return std::numeric_limits<std::size_t>::max() / sizeof(T);
       }

       // allocate but don't initialize num elements of type T
       pointer allocate (size_type num, const void* = 0) {
           // print message and allocate memory with global new
           std::cerr << "allocate " << num << " element(s)"
                     << " of size " << sizeof(T) << std::endl;
           pointer ret = (pointer)(::operator new(num*sizeof(T)));
           std::cerr << " allocated at: " << (void*)ret << std::endl;
           return ret;
       }

       // initialize elements of allocated storage p with value value
       void construct (pointer p, const T& value) {
           // initialize memory with placement new
           new((void*)p)T(value);
       }

       // destroy elements of initialized storage p
       void destroy (pointer p) {
           // destroy objects by calling their destructor
           p->~T();
       }

       // deallocate storage p of deleted elements
       void deallocate (pointer p, size_type num) {
           // print message and deallocate memory with global delete
           std::cerr << "deallocate " << num << " element(s)"
                     << " of size " << sizeof(T)
                     << " at: " << (void*)p << std::endl;
           ::operator delete((void*)p);
       }
   };

   // return that all specializations of this allocator are interchangeable
   template <class T1, class T2>
   bool operator== (const DgAlloc<T1>&,
                    const DgAlloc<T2>&) throw() {
       return true;
   }
   template <class T1, class T2>
   bool operator!= (const DgAlloc<T1>&,
                    const DgAlloc<T2>&) throw() {
       return false;
   }

*/
  
#endif // DGMEMORY_HPP
