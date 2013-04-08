#ifndef WRAP_DETECTOR_POWER_HPP
#define WRAP_DETECTOR_POWER_HPP

// take any numeric type to an arbitrary integer power, offloading as
// much of the computation as possible from runtime to the compiler.
// syntax: pow<n>(x)
// i.e. pow<4>(2.) evaluates to 2.*2.*2.*2.=16.

#include <cmath>

namespace
detector
{

  namespace
  _pow_impl
  {

    template<long N,class Y> struct pow_impl;
    template<long N, bool = (N%2==0)> struct pow_impl_pow;

    template<long N>
    struct pow_impl_pow<N,true>
    {
      template<typename Y>
      static inline const Y call(const Y& y)
      {
        const Y result( y*y );
        return result * pow_impl_pow< (N>>1) >::call( y );
      }
    };

    template<>
    struct pow_impl_pow<1,false>
    {
      template<typename Y>
      static inline const Y call(const Y& y)
      {
        const Y result( y );
        return result;
      }
    };

    template<>
    struct pow_impl_pow<2,true>
    {
      template<typename Y>
      static inline const Y call(const Y& y)
      {
        const Y result( y * y );
        return result;
      }
    };

    template<long N>
    struct pow_impl_pow<N,false>
    {
      template<typename Y>
      static inline const Y call(const Y& y)
      {
        return y * pow_impl_pow< (N-1) >::call( y );
      }
    };

    template<long N,class Y> 
    struct pow_impl
    {
      static inline const Y call(const Y& y)
      {
        const Y result( pow_impl_pow<N>::call(y) );
        return result;
      }
    };

    template<class Y>
    struct pow_impl<0,Y>
    {
      typedef Y type;
      static inline const Y call(const Y& y)
      {
        const Y result( 1 );
        return result;
      }
    };

    template<class Y>
    struct pow_impl<1,Y>
    {
      typedef Y type;
      static inline const Y call(const Y& y)
      {
        return y;
      }
    };
  };

};

#endif // WRAP_DETECTOR_POWER_HPP
