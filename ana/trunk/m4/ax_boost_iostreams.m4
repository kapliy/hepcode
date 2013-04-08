dnl autoconf macro for boost::iostreams. relies on ax_boost_base.m4
dnl based on version at http://autoconf-archive.cryp.to/ax_boost_base.html.
dnl 2009-04-20 Antonio Boveia <boveia@hep.uchicago.edu>

AC_DEFUN([AX_BOOST_IOSTREAMS],
[
   AC_ARG_WITH([boost-iostreams],
     AS_HELP_STRING([--with-boost-iostreams@<:@=special-lib@:>@],
                    [use the IOStreams library from boost - it is possible to specify a certain library for the linker
                     e.g. --with-boost-iostreams=boost_iostreams-gcc-mt-d-1_33_1 ]),
     [  if test "$withval" = "no"; then
            want_boost="no"
        elif test "$withval" = "yes"; then
            want_boost="yes"
            ax_boost_user_iostreams_lib=""
        else
            want_boost="yes"
            ax_boost_user_iostreams_lib="$withval"
        fi
     ],
     [want_boost="yes"]
   )

   if test "x$want_boost" = "xyes"; then
     AC_REQUIRE([AC_PROG_CC])
     CPPFLAGS_SAVED="$CPPFLAGS"
     CPPFLAGS="$CPPFLAGS $BOOST_CPPFLAGS"
     export CPPFLAGS

     LDFLAGS_SAVED="$LDFLAGS"
     LDFLAGS="$LDFLAGS $BOOST_LDFLAGS"
     export LDFLAGS

        AC_CACHE_CHECK(boost::iostreams library is available,ax_cv_boost_iostreams,[
           AC_LANG_PUSH([C++])
           AC_COMPILE_IFELSE([AC_LANG_SOURCE([[@%:@include <boost/iostreams/filtering_stream.hpp>
                                              @%:@include <boost/range/iterator_range.hpp>
                                             ]],
                                  [[std::string  input = "Hello World!";
                                    namespace io = boost::iostreams;
                                    io::filtering_istream  in(boost::make_iterator_range(input));
                                    return 0;
                                   ]])],
                             ax_cv_boost_iostreams=yes, ax_cv_boost_iostreams=no)
dnl           AC_MSG_NOTICE(boost::iostreams is $ax_cv_boost_iostreams with $BOOST_LDFLAGS $BOOST_CPPFLAGS)
          AC_LANG_POP([C++])
          ]
        )

        if test "x$ax_cv_boost_iostreams" = "xyes"; then
          AC_DEFINE(HAVE_BOOST_IOSTREAMS,,[define the boost::iostreams library is available])
          BOOSTLIBDIR=`echo $BOOST_LDFLAGS | sed -e 's/@<:@^\/@:>@*//'`
          if test "x$ax_boost_user_iostreams_lib" = "x"; then
             for libextension in `ls $BOOSTLIBDIR/libboost_iostreams*.{so,a}* 2>/dev/null | sed 's,.*/,,' | sed -e 's;^lib\(boost_iostreams.*\)\.so.*$;\1;' -e 's;^lib\(boost_iostreams.*\)\.a*$;\1;'` ; do
                ax_lib=${libextension}
                AC_CHECK_LIB($ax_lib, exit,[BOOST_IOSTREAMS_LIB="-l$ax_lib"; AC_SUBST(BOOST_IOSTREAMS_LIB) link_iostreams="yes"; break],[link_iostreams="no"])
             done
             if test "x$link_iostreams" != "xyes"; then
                for libextension in `ls $BOOSTLIBDIR/boost_iostreams*.{dll,a}* 2>/dev/null | sed 's,.*/,,' | sed -e 's;^\(boost_iostreams.*\)\.dll.*$;\1;' -e 's;^\(boost_iostreams.*\)\.a*$;\1;'` ; do
                     ax_lib=${libextension}
                     AC_CHECK_LIB($ax_lib, exit,
                     [BOOST_IOSTREAMS_LIB="-l$ax_lib"; AC_SUBST(BOOST_IOSTREAMS_LIB) link_iostreams="yes"; break],
                     [link_iostreams="no"])
                done
             fi
        else
             for ax_lib in $ax_boost_user_iostreams_lib boost_iostreams-$ax_boost_user_iostreams_lib; do
                AC_CHECK_LIB($ax_lib, main,
                [BOOST_IOSTREAMS_LIB="-l$ax_lib"; AC_SUBST(BOOST_IOSTREAMS_LIB) link_iostreams="yes"; break],
                [link_iostreams="no"])
             done
        fi
        if test "x$link_iostreams" != "xyes"; then
             AC_MSG_ERROR(Could not link against $ax_lib !)
        fi
     fi

     CPPFLAGS="$CPPFLAGS_SAVED"
     LDFLAGS="$LDFLAGS_SAVED"
  fi
])
