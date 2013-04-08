#ifndef WRAP_EXTEND_BIND_HPP
#define WRAP_EXTEND_BIND_HPP

// increase number of bind arguments possible.
// currently adds 10 and 11 argument options.

#include <boost/bind/storage.hpp>
#include <boost/bind/bind.hpp>

namespace
boost
{

  namespace
  _bi
  {

    // storage10, storage11, storage12

    // 10

    template<class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10> struct storage10: public storage9< A1, A2, A3, A4, A5, A6, A7, A8, A9 >
    {
      typedef storage9<A1, A2, A3, A4, A5, A6, A7, A8, A9> inherited;

      storage10( A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10 ): storage9<A1, A2, A3, A4, A5, A6, A7, A8, A9>( a1, a2, a3, a4, a5, a6, a7, a8, a9), a10_( a10 ) {}

      template<class V> void accept(V & v) const
      {
        inherited::accept(v);
        BOOST_BIND_VISIT_EACH(v, a10_, 0);
      }

      A10 a10_;
    };

#if !defined( BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION )

    template<class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, int I> struct storage10< A1, A2, A3, A4, A5, A6, A7, A8, A9, boost::arg<I> >: public storage9< A1, A2, A3, A4, A5, A6, A7, A8, A9 >
    {
      typedef storage9<A1, A2, A3, A4, A5, A6, A7, A8, A9> inherited;

      storage10( A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, boost::arg<I> ): storage9<A1, A2, A3, A4, A5, A6, A7, A8, A9>( a1, a2, a3, a4, a5, a6, a7, a8, a9 ) {}

      template<class V> void accept(V & v) const
      {
        inherited::accept(v);
      }

      static boost::arg<I> a10_() { return boost::arg<I>(); }
    };

    template<class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, int I> struct storage10< A1, A2, A3, A4, A5, A6, A7, A8, A9, boost::arg<I> (*) () >: public storage9< A1, A2, A3, A4, A5, A6, A7, A8, A9 >
    {
      typedef storage9<A1, A2, A3, A4, A5, A6, A7, A8, A9> inherited;

      storage10( A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, boost::arg<I> (*) () ): storage9<A1, A2, A3, A4, A5, A6, A7, A8, A9>( a1, a2, a3, a4, a5, a6, a7, a8, a9 ) {}

      template<class V> void accept(V & v) const
      {
        inherited::accept(v);
      }

      static boost::arg<I> a10_() { return boost::arg<I>(); }
    };

#endif

    // 11

    template<class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11> struct storage11: public storage10< A1, A2, A3, A4, A5, A6, A7, A8, A9, A10 >
    {
      typedef storage10<A1, A2, A3, A4, A5, A6, A7, A8, A9, A10> inherited;

      storage11( A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11 ): storage10<A1, A2, A3, A4, A5, A6, A7, A8, A9, A10>( a1, a2, a3, a4, a5, a6, a7, a8, a9, a10), a11_( a11 ) {}

      template<class V> void accept(V & v) const
      {
        inherited::accept(v);
        BOOST_BIND_VISIT_EACH(v, a11_, 0);
      }

      A11 a11_;
    };

#if !defined( BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION )

    template<class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, int I> struct storage11< A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, boost::arg<I> >: public storage10< A1, A2, A3, A4, A5, A6, A7, A8, A9, A10 >
    {
      typedef storage10<A1, A2, A3, A4, A5, A6, A7, A8, A9, A10> inherited;

      storage11( A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, boost::arg<I> ): storage10<A1, A2, A3, A4, A5, A6, A7, A8, A9, A10>( a1, a2, a3, a4, a5, a6, a7, a8, a9, a10 ) {}

      template<class V> void accept(V & v) const
      {
        inherited::accept(v);
      }

      static boost::arg<I> a11_() { return boost::arg<I>(); }
    };

    template<class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, int I> struct storage11< A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, boost::arg<I> (*) () >: public storage10< A1, A2, A3, A4, A5, A6, A7, A8, A9, A10 >
    {
      typedef storage10<A1, A2, A3, A4, A5, A6, A7, A8, A9, A10> inherited;

      storage11( A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, boost::arg<I> (*) () ): storage10<A1, A2, A3, A4, A5, A6, A7, A8, A9, A10>( a1, a2, a3, a4, a5, a6, a7, a8, a9, a10 ) {}

      template<class V> void accept(V & v) const
      {
        inherited::accept(v);
      }

      static boost::arg<I> a11_() { return boost::arg<I>(); }
    };

#endif

    // 12

    template<class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12> struct storage12: public storage11< A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11 >
    {
      typedef storage11<A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11> inherited;

      storage12( A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12 ): storage11<A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11>( a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11), a12_( a12 ) {}

      template<class V> void accept(V & v) const
      {
        inherited::accept(v);
        BOOST_BIND_VISIT_EACH(v, a12_, 0);
      }

      A12 a12_;
    };

#if !defined( BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION )

    template<class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, int I> struct storage12< A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, boost::arg<I> >: public storage11< A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11 >
    {
      typedef storage11<A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11> inherited;

      storage12( A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, boost::arg<I> ): storage11<A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11>( a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11 ) {}

      template<class V> void accept(V & v) const
      {
        inherited::accept(v);
      }

      static boost::arg<I> a12_() { return boost::arg<I>(); }
    };

    template<class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, int I> struct storage12< A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, boost::arg<I> (*) () >: public storage11< A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11 >
    {
      typedef storage11<A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11> inherited;

      storage12( A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, boost::arg<I> (*) () ): storage11<A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11>( a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11 ) {}

      template<class V> void accept(V & v) const
      {
        inherited::accept(v);
      }

      static boost::arg<I> a12_() { return boost::arg<I>(); }
    };

#endif

    // list10, list11, list12

    template< class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8 , class A9 , class A10 > class list10: private storage10< A1, A2, A3, A4, A5, A6, A7, A8, A9 , A10 >
    {
    private:

      typedef storage10< A1, A2, A3, A4, A5, A6, A7, A8 , A9 , A10 > base_type;

    public:

      list10( A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10 ): base_type( a1, a2, a3, a4, a5, a6, a7, a8, a9, a10 ) {}

      A1 operator[] (boost::arg<1>) const { return base_type::a1_; }
      A2 operator[] (boost::arg<2>) const { return base_type::a2_; }
      A3 operator[] (boost::arg<3>) const { return base_type::a3_; }
      A4 operator[] (boost::arg<4>) const { return base_type::a4_; }
      A5 operator[] (boost::arg<5>) const { return base_type::a5_; }
      A6 operator[] (boost::arg<6>) const { return base_type::a6_; }
      A7 operator[] (boost::arg<7>) const { return base_type::a7_; }
      A8 operator[] (boost::arg<8>) const { return base_type::a8_; }
      A9 operator[] (boost::arg<9>) const { return base_type::a9_; }
      A10 operator[] (boost::arg<10>) const { return base_type::a10_; }

      A1 operator[] (boost::arg<1> (*) ()) const { return base_type::a1_; }
      A2 operator[] (boost::arg<2> (*) ()) const { return base_type::a2_; }
      A3 operator[] (boost::arg<3> (*) ()) const { return base_type::a3_; }
      A4 operator[] (boost::arg<4> (*) ()) const { return base_type::a4_; }
      A5 operator[] (boost::arg<5> (*) ()) const { return base_type::a5_; }
      A6 operator[] (boost::arg<6> (*) ()) const { return base_type::a6_; }
      A7 operator[] (boost::arg<7> (*) ()) const { return base_type::a7_; }
      A8 operator[] (boost::arg<8> (*) ()) const { return base_type::a8_; }
      A9 operator[] (boost::arg<9> (*) ()) const { return base_type::a9_; }
      A10 operator[] (boost::arg<10> (*) ()) const { return base_type::a10_; }

      template<class T> T & operator[] (_bi::value<T> & v) const { return v.get(); }

      template<class T> T const & operator[] (_bi::value<T> const & v) const { return v.get(); }

      template<class T> T & operator[] (reference_wrapper<T> const & v) const { return v.get(); }

      template<class R, class F, class L> typename result_traits<R, F>::type operator[] (bind_t<R, F, L> & b) const { return b.eval(*this); }

      template<class R, class F, class L> typename result_traits<R, F>::type operator[] (bind_t<R, F, L> const & b) const { return b.eval(*this); }

      template<class R, class F, class A> R operator()(type<R>, F & f, A & a, long)
      {
        return unwrapper<F>::unwrap(f, 0)(a[base_type::a1_], a[base_type::a2_], a[base_type::a3_], a[base_type::a4_], a[base_type::a5_], a[base_type::a6_], a[base_type::a7_], a[base_type::a8_], a[base_type::a9_], a[base_type::a10_]);
      }

      template<class R, class F, class A> R operator()(type<R>, F const & f, A & a, long) const
      {
        return unwrapper<F const>::unwrap(f, 0)(a[base_type::a1_], a[base_type::a2_], a[base_type::a3_], a[base_type::a4_], a[base_type::a5_], a[base_type::a6_], a[base_type::a7_], a[base_type::a8_], a[base_type::a9_], a[base_type::a10_]);
      }

      template<class F, class A> void operator()(type<void>, F & f, A & a, int)
      {
        unwrapper<F>::unwrap(f, 0)(a[base_type::a1_], a[base_type::a2_], a[base_type::a3_], a[base_type::a4_], a[base_type::a5_], a[base_type::a6_], a[base_type::a7_], a[base_type::a8_], a[base_type::a9_], a[base_type::a10_]);
      }

      template<class F, class A> void operator()(type<void>, F const & f, A & a, int) const
      {
        unwrapper<F const>::unwrap(f, 0)(a[base_type::a1_], a[base_type::a2_], a[base_type::a3_], a[base_type::a4_], a[base_type::a5_], a[base_type::a6_], a[base_type::a7_], a[base_type::a8_], a[base_type::a9_], a[base_type::a10_]);
      }

      template<class V> void accept(V & v) const
      {
        base_type::accept(v);
      }

      bool operator==(list10 const & rhs) const
      {
        return
            
          ref_compare( base_type::a1_, rhs.a1_, 0 ) &&
          ref_compare( base_type::a2_, rhs.a2_, 0 ) &&
          ref_compare( base_type::a3_, rhs.a3_, 0 ) &&
          ref_compare( base_type::a4_, rhs.a4_, 0 ) &&
          ref_compare( base_type::a5_, rhs.a5_, 0 ) &&
          ref_compare( base_type::a6_, rhs.a6_, 0 ) &&
          ref_compare( base_type::a7_, rhs.a7_, 0 ) &&
          ref_compare( base_type::a8_, rhs.a8_, 0 ) &&
          ref_compare( base_type::a9_, rhs.a9_, 0 ) &&
          ref_compare( base_type::a10_, rhs.a10_, 0 );
      }
    };

    template<class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11> class list11: private storage11< A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11 >
    {
    private:

      typedef storage11< A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11 > base_type;

    public:

      list11( A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11 ): base_type( a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11 ) {}

      A1 operator[] (boost::arg<1>) const { return base_type::a1_; }
      A2 operator[] (boost::arg<2>) const { return base_type::a2_; }
      A3 operator[] (boost::arg<3>) const { return base_type::a3_; }
      A4 operator[] (boost::arg<4>) const { return base_type::a4_; }
      A5 operator[] (boost::arg<5>) const { return base_type::a5_; }
      A6 operator[] (boost::arg<6>) const { return base_type::a6_; }
      A7 operator[] (boost::arg<7>) const { return base_type::a7_; }
      A8 operator[] (boost::arg<8>) const { return base_type::a8_; }
      A9 operator[] (boost::arg<9>) const { return base_type::a9_; }
      A10 operator[] (boost::arg<10>) const { return base_type::a10_; }
      A11 operator[] (boost::arg<11>) const { return base_type::a11_; }

      A1 operator[] (boost::arg<1> (*) ()) const { return base_type::a1_; }
      A2 operator[] (boost::arg<2> (*) ()) const { return base_type::a2_; }
      A3 operator[] (boost::arg<3> (*) ()) const { return base_type::a3_; }
      A4 operator[] (boost::arg<4> (*) ()) const { return base_type::a4_; }
      A5 operator[] (boost::arg<5> (*) ()) const { return base_type::a5_; }
      A6 operator[] (boost::arg<6> (*) ()) const { return base_type::a6_; }
      A7 operator[] (boost::arg<7> (*) ()) const { return base_type::a7_; }
      A8 operator[] (boost::arg<8> (*) ()) const { return base_type::a8_; }
      A9 operator[] (boost::arg<9> (*) ()) const { return base_type::a9_; }
      A10 operator[] (boost::arg<10> (*) ()) const { return base_type::a10_; }
      A11 operator[] (boost::arg<11> (*) ()) const { return base_type::a11_; }

      template<class T> T & operator[] (_bi::value<T> & v) const { return v.get(); }

      template<class T> T const & operator[] (_bi::value<T> const & v) const { return v.get(); }

      template<class T> T & operator[] (reference_wrapper<T> const & v) const { return v.get(); }

      template<class R, class F, class L> typename result_traits<R, F>::type operator[] (bind_t<R, F, L> & b) const { return b.eval(*this); }

      template<class R, class F, class L> typename result_traits<R, F>::type operator[] (bind_t<R, F, L> const & b) const { return b.eval(*this); }

      template<class R, class F, class A> R operator()(type<R>, F & f, A & a, long)
      {
        return unwrapper<F>::unwrap(f, 0)(a[base_type::a1_], a[base_type::a2_], a[base_type::a3_], a[base_type::a4_], a[base_type::a5_], a[base_type::a6_], a[base_type::a7_], a[base_type::a8_], a[base_type::a9_], a[base_type::a10_], a[base_type::a11_]);
      }

      template<class R, class F, class A> R operator()(type<R>, F const & f, A & a, long) const
      {
        return unwrapper<F const>::unwrap(f, 0)(a[base_type::a1_], a[base_type::a2_], a[base_type::a3_], a[base_type::a4_], a[base_type::a5_], a[base_type::a6_], a[base_type::a7_], a[base_type::a8_], a[base_type::a9_], a[base_type::a10_], a[base_type::a11_]);
      }

      template<class F, class A> void operator()(type<void>, F & f, A & a, int)
      {
        unwrapper<F>::unwrap(f, 0)(a[base_type::a1_], a[base_type::a2_], a[base_type::a3_], a[base_type::a4_], a[base_type::a5_], a[base_type::a6_], a[base_type::a7_], a[base_type::a8_], a[base_type::a9_], a[base_type::a10_], a[base_type::a11_]);
      }

      template<class F, class A> void operator()(type<void>, F const & f, A & a, int) const
      {
        unwrapper<F const>::unwrap(f, 0)(a[base_type::a1_], a[base_type::a2_], a[base_type::a3_], a[base_type::a4_], a[base_type::a5_], a[base_type::a6_], a[base_type::a7_], a[base_type::a8_], a[base_type::a9_], a[base_type::a10_], a[base_type::a11_]);
      }

      template<class V> void accept(V & v) const
      {
        base_type::accept(v);
      }

      bool operator==(list11 const & rhs) const
      {
        return

          ref_compare( base_type::a1_, rhs.a1_, 0 ) &&
          ref_compare( base_type::a2_, rhs.a2_, 0 ) &&
          ref_compare( base_type::a3_, rhs.a3_, 0 ) &&
          ref_compare( base_type::a4_, rhs.a4_, 0 ) &&
          ref_compare( base_type::a5_, rhs.a5_, 0 ) &&
          ref_compare( base_type::a6_, rhs.a6_, 0 ) &&
          ref_compare( base_type::a7_, rhs.a7_, 0 ) &&
          ref_compare( base_type::a8_, rhs.a8_, 0 ) &&
          ref_compare( base_type::a9_, rhs.a9_, 0 ) &&
          ref_compare( base_type::a10_, rhs.a10_, 0 ) &&
          ref_compare( base_type::a11_, rhs.a11_, 0 );
      }
    };

    template<class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12> class list12: private storage12< A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12 >
    {
    private:

      typedef storage12< A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12 > base_type;

    public:

      list12( A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12 ): base_type( a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12 ) {}

      A1 operator[] (boost::arg<1>) const { return base_type::a1_; }
      A2 operator[] (boost::arg<2>) const { return base_type::a2_; }
      A3 operator[] (boost::arg<3>) const { return base_type::a3_; }
      A4 operator[] (boost::arg<4>) const { return base_type::a4_; }
      A5 operator[] (boost::arg<5>) const { return base_type::a5_; }
      A6 operator[] (boost::arg<6>) const { return base_type::a6_; }
      A7 operator[] (boost::arg<7>) const { return base_type::a7_; }
      A8 operator[] (boost::arg<8>) const { return base_type::a8_; }
      A9 operator[] (boost::arg<9>) const { return base_type::a9_; }
      A10 operator[] (boost::arg<10>) const { return base_type::a10_; }
      A11 operator[] (boost::arg<11>) const { return base_type::a11_; }
      A12 operator[] (boost::arg<12>) const { return base_type::a12_; }

      A1 operator[] (boost::arg<1> (*) ()) const { return base_type::a1_; }
      A2 operator[] (boost::arg<2> (*) ()) const { return base_type::a2_; }
      A3 operator[] (boost::arg<3> (*) ()) const { return base_type::a3_; }
      A4 operator[] (boost::arg<4> (*) ()) const { return base_type::a4_; }
      A5 operator[] (boost::arg<5> (*) ()) const { return base_type::a5_; }
      A6 operator[] (boost::arg<6> (*) ()) const { return base_type::a6_; }
      A7 operator[] (boost::arg<7> (*) ()) const { return base_type::a7_; }
      A8 operator[] (boost::arg<8> (*) ()) const { return base_type::a8_; }
      A9 operator[] (boost::arg<9> (*) ()) const { return base_type::a9_; }
      A10 operator[] (boost::arg<10> (*) ()) const { return base_type::a10_; }
      A11 operator[] (boost::arg<11> (*) ()) const { return base_type::a11_; }
      A12 operator[] (boost::arg<12> (*) ()) const { return base_type::a12_; }

      template<class T> T & operator[] (_bi::value<T> & v) const { return v.get(); }

      template<class T> T const & operator[] (_bi::value<T> const & v) const { return v.get(); }

      template<class T> T & operator[] (reference_wrapper<T> const & v) const { return v.get(); }

      template<class R, class F, class L> typename result_traits<R, F>::type operator[] (bind_t<R, F, L> & b) const { return b.eval(*this); }

      template<class R, class F, class L> typename result_traits<R, F>::type operator[] (bind_t<R, F, L> const & b) const { return b.eval(*this); }

      template<class R, class F, class A> R operator()(type<R>, F & f, A & a, long)
      {
        return unwrapper<F>::unwrap(f, 0)(a[base_type::a1_], a[base_type::a2_], a[base_type::a3_], a[base_type::a4_], a[base_type::a5_], a[base_type::a6_], a[base_type::a7_], a[base_type::a8_], a[base_type::a9_], a[base_type::a10_], a[base_type::a11_], a[base_type::a12_]);
      }

      template<class R, class F, class A> R operator()(type<R>, F const & f, A & a, long) const
      {
        return unwrapper<F const>::unwrap(f, 0)(a[base_type::a1_], a[base_type::a2_], a[base_type::a3_], a[base_type::a4_], a[base_type::a5_], a[base_type::a6_], a[base_type::a7_], a[base_type::a8_], a[base_type::a9_], a[base_type::a10_], a[base_type::a11_], a[base_type::a12_]);
      }

      template<class F, class A> void operator()(type<void>, F & f, A & a, int)
      {
        unwrapper<F>::unwrap(f, 0)(a[base_type::a1_], a[base_type::a2_], a[base_type::a3_], a[base_type::a4_], a[base_type::a5_], a[base_type::a6_], a[base_type::a7_], a[base_type::a8_], a[base_type::a9_], a[base_type::a10_], a[base_type::a11_], a[base_type::a12_]);
      }

      template<class F, class A> void operator()(type<void>, F const & f, A & a, int) const
      {
        unwrapper<F const>::unwrap(f, 0)(a[base_type::a1_], a[base_type::a2_], a[base_type::a3_], a[base_type::a4_], a[base_type::a5_], a[base_type::a6_], a[base_type::a7_], a[base_type::a8_], a[base_type::a9_], a[base_type::a10_], a[base_type::a11_], a[base_type::a12_]);
      }

      template<class V> void accept(V & v) const
      {
        base_type::accept(v);
      }

      bool operator==(list12 const & rhs) const
      {
        return

          ref_compare( base_type::a1_, rhs.a1_, 0 ) &&
          ref_compare( base_type::a2_, rhs.a2_, 0 ) &&
          ref_compare( base_type::a3_, rhs.a3_, 0 ) &&
          ref_compare( base_type::a4_, rhs.a4_, 0 ) &&
          ref_compare( base_type::a5_, rhs.a5_, 0 ) &&
          ref_compare( base_type::a6_, rhs.a6_, 0 ) &&
          ref_compare( base_type::a7_, rhs.a7_, 0 ) &&
          ref_compare( base_type::a8_, rhs.a8_, 0 ) &&
          ref_compare( base_type::a9_, rhs.a9_, 0 ) &&
          ref_compare( base_type::a10_, rhs.a10_, 0 ) &&
          ref_compare( base_type::a11_, rhs.a11_, 0 ) &&
          ref_compare( base_type::a12_, rhs.a12_, 0 );
      }
    };

  }; // end namespace _bi

}; // end namespace _boost
 
#endif // WRAP_EXTEND_BIND_HPP
