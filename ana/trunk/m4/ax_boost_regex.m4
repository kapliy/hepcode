dnl autoconf macro for boost regex. relies on ax_boost_base.m4
dnl based on version at http://autoconf-archive.cryp.to/ax_boost_base.html.
dnl 2009-04-20 Antonio Boveia <boveia@hep.uchicago.edu>

AC_DEFUN([AX_BOOST_REGEX],
[
        AC_ARG_WITH([boost-regex],
                AS_HELP_STRING([--with-boost-regex@<:@=special-lib@:>@],
                       [use the regex library from boost - it is possible to specify a certain library for the linker
                        e.g. --with-boost-regex=boost_regex-gcc-mt-1_33_1 ]),
        [
        if test "$withval" = "no"; then
                        want_boost="no"
        elif test "$withval" = "yes"; then
            want_boost="yes"
            ax_boost_user_regex_lib=""
        else
                    want_boost="yes"
                ax_boost_user_regex_lib="$withval"
                fi
        ],
        [want_boost="yes"]
        )

        if test "x$want_boost" = "xyes"; then
        AC_REQUIRE([AC_PROG_CC])
            export want_boost
                CPPFLAGS_SAVED="$CPPFLAGS"
                CPPFLAGS="$CPPFLAGS $BOOST_CPPFLAGS"
                export CPPFLAGS
                LDFLAGS_SAVED="$LDFLAGS"
                LDFLAGS="$LDFLAGS $BOOST_LDFLAGS"
                export LDFLAGS
                AC_CACHE_CHECK([whether the boost::regex library is available],
                               ax_cv_boost_regex,
                               [AC_LANG_PUSH(C++)
                                AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[@%:@include <boost/regex.hpp>]],
                                                                  [[boost::regex::basic_regex generic;
                                                                  return 0;]])],
                                                  ax_cv_boost_regex=yes, ax_cv_boost_regex=no)
                                AC_LANG_POP([C++])
                ])
                if test "$ax_cv_boost_regex" = yes; then
                                AC_DEFINE(HAVE_BOOST_REGEX,,[define if the Boost::regex library is available])
                                BOOSTLIBDIR=`echo $BOOST_LDFLAGS | sed -e 's/@<:@^\/@:>@*//'`
                                if test "x$ax_boost_user_regex_lib" = "x"; then
                for libextension in `ls $BOOSTLIBDIR/libboost_regex*.{so,a}* 2>/dev/null | sed 's,.*/,,' | sed -e 's;^lib\(boost_regex.*\)\.so.*$;\1;' -e 's;^lib\(boost_regex.*\)\.a*$;\1;'` ; do
                     ax_lib=${libextension}
                                    AC_CHECK_LIB($ax_lib, exit,
                                 [BOOST_REGEX_LIB="-l$ax_lib"; AC_SUBST(BOOST_REGEX_LIB) link_regex="yes"; break],
                                 [link_regex="no"])
                                done
                if test "x$link_regex" != "xyes"; then
                for libextension in `ls $BOOSTLIBDIR/boost_regex*.{dll,a}* 2>/dev/null | sed 's,.*/,,' | sed -e 's;^\(boost_regex.*\)\.dll.*$;\1;' -e 's;^\(boost_regex.*\)\.a*$;\1;'` ; do
                     ax_lib=${libextension}
                                    AC_CHECK_LIB($ax_lib, exit,
                                 [BOOST_REGEX_LIB="-l$ax_lib"; AC_SUBST(BOOST_REGEX_LIB) link_regex="yes"; break],
                                 [link_regex="no"])
                                done
                fi
                else
                  for ax_lib in $ax_boost_user_regex_lib boost_regex-$ax_boost_user_regex_lib; do
                                      AC_CHECK_LIB($ax_lib, main,
                                   [BOOST_REGEX_LIB="-l$ax_lib"; AC_SUBST(BOOST_REGEX_LIB) link_regex="yes"; break],
                                   [link_regex="no"])
                  done
                fi
                                if test "x$link_regex" != "xyes"; then
                                        AC_MSG_ERROR([Could not link against [$ax_lib] !])
                                fi
                fi
                CPPFLAGS="$CPPFLAGS_SAVED"
        LDFLAGS="$LDFLAGS_SAVED"
        fi
])
