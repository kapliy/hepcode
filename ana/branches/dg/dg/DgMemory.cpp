
#include <cstddef> // size_t
#include <cstdlib> // malloc
#include <cstring>
#include <cassert>
#include <new>
#include <list>
#include <boost/format.hpp>

#include "DataGraphics/DgMemory.hpp"

// as the following code attempts to track every memory allocation
// made by the c++ program, some pieces are executed during every
// "new" operation anywhere in the program, and thus these pieces must
// be careful about not causing additional memory allocations to be
// made lest an infinite recursive loop develop. in practice, this
// means no memory allocations (e.g. no use of STL) except in the
// special debugging status routines such as "print" and "leak_check",
// and even there one should in principle disable tracking of or
// adjust for the additional memory allocations of std::list, cout,
// and boost::format so as to report accurate figures.

using namespace std;

#ifndef DGMEMORY_OFF

inline void* operator new( std::size_t size , DgHeap* heap )
{
#ifdef DGMEMORY_DEBUG
  // requested allocation size + header size + end of block marker size
  std::size_t buffer_size = size + sizeof(DgAllocHeader) + sizeof(int);
  char* buffer = 0;
  try {
    buffer = static_cast<char*>( std::malloc(buffer_size) );
  } catch( std::bad_alloc ) {
    throw DgMemoryManager::bad_alloc( "could not allocate enough heap memory" );
  }
  if( !buffer ) { throw DgMemoryManager::bad_alloc( "could not allocate enough heap memory" ); }
#else
  // requested allocation size + minimal header size
  std::size_t buffer_size = size + sizeof(DgAllocHeader);
  char* buffer = static_cast<char*>( std::malloc(buffer_size) );
#endif
  DgAllocHeader* header = reinterpret_cast<DgAllocHeader*>(buffer);
#ifdef DGMEMORY_DEBUG
  header->set_signature();
#endif
  header->_size = size;
  header->_heap = heap;
  heap->add( header );
  void* start_block = buffer + sizeof(DgAllocHeader);
#ifdef DGMEMORY_DEBUG
  int* end_marker_block = reinterpret_cast<int*>( reinterpret_cast<char*>(start_block) + size );
  (*end_marker_block) = DgAllocHeader::END_SIGNATURE;
#endif
  return start_block;
}

inline void operator delete( void* buffer )
{
  if( !buffer ) { return; }
  DgAllocHeader* header = reinterpret_cast<DgAllocHeader*>( static_cast<char*>(buffer) - sizeof(DgAllocHeader) );
#ifdef DGMEMORY_DEBUG
  if( !(header->check_signature()) ) {
    //throw DgMemoryManager::bad_dealloc("signature check failed");
    // this isn't my memory.
    return;
  }
  int* end_marker_block = reinterpret_cast<int*>( reinterpret_cast<char*>(buffer) + header->size() );
  if( (*end_marker_block) != DgAllocHeader::END_SIGNATURE ) {
    throw DgMemoryManager::bad_dealloc("end marker check failed");
  }
  assert( header->heap() );
#endif
  header->heap()->remove( header );
#ifdef DGMEMORY_DEBUG
  // fill buffer with 'dead zone' code
  std::memset( buffer , DgAllocHeader::FREE_SIGNATURE , header->size() );
#endif
  free( header );
}

inline void* operator new[]( std::size_t size , DgHeap* heap )
{
  return operator new( size , heap );
}

inline void* operator new( std::size_t size )
{
  return operator new( size , DgMemoryManager::global_heap() );
}

inline void* operator new[]( std::size_t size )
{
  return operator new[]( size , DgMemoryManager::global_heap() );
}

const unsigned long
DgMemoryManager::num_bytes_allocated()
{
  unsigned long result = 0ul;
  for( std::list< boost::shared_ptr<DgHeap> >::const_iterator i=_heaps.begin(), f=_heaps.end(); i!=f; ++i ) {
    const boost::shared_ptr<DgHeap>& sheap( *i );
    const DgHeap* heap( sheap.get() );
    assert( heap );  
    result += heap->total_size();
  }
  return result;
}

const unsigned long
DgMemoryManager::num_allocations()
{
  unsigned long result = 0ul;
  for( std::list< boost::shared_ptr<DgHeap> >::const_iterator i=_heaps.begin(), f=_heaps.end(); i!=f; ++i ) {
    const boost::shared_ptr<DgHeap>& sheap( *i );
    const DgHeap* heap( sheap.get() );
    assert( heap );  
    result += heap->total_allocations();
  }
  return result;
}

std::ostream&
DgHeap::print( std::ostream& os ) const
{
  // walk the heap and print node allocations
  os << boost::format( "heap: %|20s| " ) % name() << std::endl;
  os << boost::format( "mem: %|10t| %|10.1f| kbytes" ) % (total_size()/1024.) << std::endl;
  os << boost::format( "act: %|10t| %|10d| active" ) % total_allocations() << std::endl;
  os << boost::format( "tot: %|10t| %|10d| allocations" ) % next_allocation_number() << std::endl;
  return os;
}

std::ostream& 
DgMemoryManager::print( std::ostream& os )
{
  // walk the heaps and print node allocations.
  for( std::list< boost::shared_ptr<DgHeap> >::const_iterator i=_heaps.begin(), f=_heaps.end(); i!=f; ++i ) {
    const boost::shared_ptr<DgHeap>& sheap( *i );
    const DgHeap* heap( sheap.get() );
    assert( heap );
    heap->print( os );
    os << std::endl;
  }
  _global_heap.print( os );
  os << std::endl;
}


std::ostream&
DgHeap::leak_check( std::ostream& os , const LeakDetailLevel level , unsigned long bookmark_allocation ) const
{
  // walk the heap and print any node allocations with allocation number > bookmark_allocation
  cout << " leak check for heap: " << name() << endl;
  unsigned long tot_leak_bytes = 0ul;
  unsigned long tot_leak_alloc = 0ul;
  for( const DgAllocHeader* header=_list_head; header; header=header->_next ) {
    assert( header );
    if( header->allocation_number() < bookmark_allocation ) { continue; }
    tot_leak_bytes += header->size();
    ++tot_leak_alloc;
    if( level < INDIVIDUAL ) { continue; }
    cout << "leak: alloc: " << header->allocation_number() << " nbytes: " << header->size() << endl;
    if( level < INDIVIDUAL_WITH_DATA ) { continue; }
    cout << "      data: ";
    const char* start_block = reinterpret_cast<const char*>(header) + sizeof(DgAllocHeader);
    unsigned int wrap = 0u;
    for( const char* end_block = reinterpret_cast<const char*>(header) + sizeof(DgAllocHeader) + header->size(); start_block!=end_block; ++start_block,++wrap ) {
      if( wrap >= 16 ) {
        cout << endl << "            ";
        wrap = 0u;
      }
      cout << boost::format( "%|02x| " ) % (static_cast<char>(*start_block)%0x100);
    }
    cout << endl;
  }
  cout << boost::format( "heap %|20s| total leaked: %|10d| bytes %|10d| allocations" ) % name() % tot_leak_bytes % tot_leak_alloc << endl;
}

std::ostream& 
DgMemoryManager::leak_check( std::ostream& os , const DgHeap::LeakDetailLevel level )
{
  // walk the heaps and print a leak check for each
  for( std::list< boost::shared_ptr<DgHeap> >::const_iterator i=_heaps.begin(), f=_heaps.end(); i!=f; ++i ) {
    const boost::shared_ptr<DgHeap>& sheap( *i );
    const DgHeap* heap( sheap.get() );
    assert( heap );
    heap->leak_check( os , level , 0 );
    os << std::endl;
  }
  _global_heap.leak_check( os , level , 0 );
  os << std::endl;
}


#ifdef DGMEMORY_DEBUG
const int DgAllocHeader::HEADER_SIGNATURE = 0xDECAF;
const int DgAllocHeader::END_SIGNATURE = 0xDEADBEEF;
const char DgAllocHeader::FREE_SIGNATURE = 0xAA;
#endif

DgHeap DgMemoryManager::_global_heap( "global" );
std::list< boost::shared_ptr<DgHeap> > DgMemoryManager::_heaps;

#else // ifndef DGMEMORY_OFF

DgHeap DgMemoryManager::_global_heap;

#endif // ifndef DGMEMORY_OFF



// ================================================================

// class
// Large
// {
// public:
//   char buffer[256];
// };
//
// class
// Object
// {
// protected:
//   unsigned int _a;
//   boost::shared_ptr<Large> _f;
// public:
//   Object(unsigned int a) : _a(a) , _f(new Large) {}
//   const unsigned int a() const { return _a; }
//
//   DECLARE_DGHEAP;
// };
//
// DEFINE_DGHEAP(Object,"object heap");
//
// the following is test program for this class, intended to be
// compiled standalone like
//  g++ -g -I/usr/local/boost/include -o DgMemory dg/DgMemory.cpp
// from the root TrigFTKAna directory.
//
// #include <iostream>
// #include <list>
// #include <cassert>
//
// using namespace std;
//
// int 
// main(void)
// {
//   {
//     vector<Object*> tmp;
//     tmp.push_back( new Object(1) );
//     tmp.push_back( new Object(2) );
//     tmp.push_back( new Object(3) );
//     tmp.push_back( new Object(4) );
//     tmp.push_back( new Object(5) );
//     tmp.push_back( new Object(6) );
//     DgMemoryManager::print( std::cout );
//     while( !tmp.empty() ) { 
//       tmp.pop_back();
//     }
//   }
//   // leaked 6 objects.
//   DgMemoryManager::leak_check( std::cout );
// }
