 
#ifndef SERIALIZE_UNORDERED_HPP
#define SERIALIZE_UNORDERED_HPP

#include <boost/config.hpp> 

#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/unordered_map.hpp>
#include <boost/serialization/utility.hpp> 
#include <boost/serialization/collections_save_imp.hpp> 
#include <boost/serialization/collections_load_imp.hpp> 
#include <boost/serialization/split_free.hpp> 

// based on Boost 1.39 boost/serialization/map.hpp.

namespace boost {
  namespace serialization {

    template< class archiveT , typename Key , typename Mapped , typename Hash , typename Pred , typename Alloc > 
    void save( archiveT & ar , const boost::unordered_map<Key,Mapped,Hash,Pred,Alloc>& p , const unsigned int version ) {
      boost::serialization::stl::save_collection<archiveT, boost::unordered_map<Key,Mapped,Hash,Pred,Alloc> >(ar,p);
    }

    template< class archiveT , typename Key , typename Mapped , typename Hash , typename Pred , typename Alloc > 
    void load( archiveT & ar , boost::unordered_map<Key,Mapped,Hash,Pred,Alloc>& p , const unsigned int version ) {
      boost::serialization::stl::load_collection<archiveT, 
                                                 boost::unordered_map<Key,Mapped,Hash,Pred,Alloc> ,
                                                 boost::serialization::stl::archive_input_map<archiveT,boost::unordered_map<Key,Mapped,Hash,Pred,Alloc> > ,
                                                 boost::serialization::stl::no_reserve_imp<boost::unordered_map<Key,Mapped,Hash,Pred,Alloc> > >(ar,p);
    }

    // split non-intrusive serialization function member into separate
    // non intrusive save/load member functions
    template< class archiveT , class Key , typename Mapped , class Hash , class Pred , class Alloc > 
    inline void serialize( archiveT & ar , boost::unordered_map<Key,Mapped,Hash,Pred,Alloc>& p , const unsigned int file_version ) {
      boost::serialization::split_free( ar , p , file_version ); 
    } 

  } // serialization 
} // namespace boost 


#endif // SERIALIZE_TLORENTZVECTOR_HPP
