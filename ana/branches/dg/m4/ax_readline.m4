
dnl checks for libreadline. defines HAVE_LIBREADLINE in config.h if
dnl libreadline is found.  if it exists, then READLINE_LDFLAGS is set
dnl appropriately.
dnl
dnl 2011-02-07 Antonio Boveia (boveia@hep.uchicago.edu)
dnl from code at http://ac-archive.sourceforge.net/ac-archive/vl_lib_readline.html


AC_DEFUN([AX_READLINE], [

  dnl retrieve path to libreadline base directory.
  readline_libdir=
  AC_ARG_WITH(readline,
    AC_HELP_STRING([--with-readline top of the readline installation directory]),[
    if test -d "$withval"; then
        readline_libdir=$withval
    fi
  ]) 
  dnl try to guess libreadline path if it hasn't been provided
  if test -e `echo $readline_libdir/libreadline* | awk '{print @S|@1}'`; then     
     AC_MSG_NOTICE(using --with-readline $readline_libdir)
  elif test -e `echo /opt/local/lib/libreadline* | awk '{print @S|@1}'`; then
     readline_libdir="/opt/local/lib"
  elif test -e `echo /usr/lib/libreadline* | awk '{print @S|@1}'`; then
     readline_libdir="/usr/lib/"
  else
     readline_libdir="none"
  fi

  READLINE_LDFLAGS=
  tmp_readline_ldflags=
  AC_CACHE_CHECK([for a readline compatible library],
                 vl_cv_lib_readline, [
    ORIG_LIBS="$LIBS"
    for readline_lib in readline edit editline; do
      for termcap_lib in "" termcap curses ncurses; do
        if test -z "$termcap_lib"; then
          TRY_LIB="-l$readline_lib"
        else
          TRY_LIB="-l$readline_lib -l$termcap_lib"
        fi
        tmp_readline_ldflags="-L$readline_libdir $TRY_LIB"
        LIBS="$ORIG_LIBS $tmp_readline_ldflags"
        dnl AC_MSG_NOTICE(trying $LIBS with $TRY_LIB)
        AC_TRY_LINK_FUNC(readline, vl_cv_lib_readline="$TRY_LIB")
        if test -n "$vl_cv_lib_readline"; then
          break
        fi
      done
      if test -n "$vl_cv_lib_readline"; then
        break
      fi
    done
    if test -z "$vl_cv_lib_readline"; then
      vl_cv_lib_readline="no"
      LIBS="$ORIG_LIBS"
    fi
  ])

  if test "$vl_cv_lib_readline" != "no"; then
    AC_DEFINE(HAVE_READLINE, 1,
              [Defined if readline compatible library exists])
    READLINE_LDFLAGS=$tmp_readline_ldflags
    AC_MSG_RESULT(yes)
    dnl AC_CHECK_HEADERS(readline.h readline/readline.h)
    dnl AC_CACHE_CHECK([whether readline supports history],
    dnl                vl_cv_lib_readline_history, [
    dnl   vl_cv_lib_readline_history="no"
    dnl   AC_TRY_LINK_FUNC(add_history, vl_cv_lib_readline_history="yes")
    dnl ])
    dnl if test "$vl_cv_lib_readline_history" = "yes"; then
    dnl   AC_DEFINE(HAVE_READLINE_HISTORY, 1,
    dnl             [Define if your readline library has \`add_history'])
    dnl   AC_CHECK_HEADERS(history.h readline/history.h)
    dnl fi
  else
    AC_MSG_RESULT(no)
  fi
  AC_SUBST(READLINE_LDFLAGS)
])



dnl AC_DEFUN([AX_READLINE],
dnl [
dnl   dnl retrieve path to libreadline base directory.
dnl   AC_ARG_WITH(readline,
dnl     AC_HELP_STRING([--with-readline top of the readline installation directory]),[
dnl     if test -d "$withval"; then
dnl         user_readline=$withval
dnl     elif test -d "/usr/lib"; then
dnl         user_readline="/opt/local/lib"
dnl     else
dnl         user_readline="none"
dnl     fi
dnl   ]) 
dnl   READLINE_LDFLAGS=
dnl   AC_MSG_CHECKING([for readline library])
dnl   if test -d "$user_readline" ; then
dnl     READLINE_LIBDIR="${user_readline}"
dnl     if test -d "${READLINE_LIBDIR}" ; then
dnl        READLINE_LDFLAGS="-L$READLINE_LIBDIR -lreadline"
dnl        AC_REQUIRE([AC_PROG_CC])
dnl             export want_readline
dnl             LDFLAGS_SAVED="$LDFLAGS"
dnl             LDFLAGS="$LDFLAGS $READLINE_LDFLAGS"
dnl             export LDFLAGS
dnl             AC_CACHE_CHECK([whether the readline library is available],
dnl                             ax_cv_readline,
dnl                            [AC_LANG_PUSH(C++)
dnl                                 AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[@%:@include <boost/program_options.hpp>]],
dnl                                                                   [[boost::program_options::options_description generic("Generic options");
dnl                                                                   return 0;]])],
dnl                                                   ax_cv_boost_program_options=yes, ax_cv_boost_program_options=no)
dnl                                 AC_LANG_POP([C++])
dnl                 ])
dnl                 if test "$ax_cv_boost_program_options" = yes; then




dnl        AC_DEFINE(HAVE_LIBREADLINE,,[define if the READLINE dCache library is available])
dnl        AC_MSG_RESULT(yes)
dnl     else
dnl        AC_MSG_RESULT(no)
dnl     fi
dnl   else
dnl     AC_MSG_RESULT(no)
dnl   fi
  
dnl   AC_SUBST(READLINE_LDFLAGS)

dnl ])
