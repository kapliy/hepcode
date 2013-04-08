
dnl checks for plplot. if it exists, then HAVE_PLPLOT, PLPLOT_CPPFLAGS,
dnl PLPLOT_LDFLAGS, PLPLOT_INCDIR, and PLPLOT_LIBDIR are set
dnl appropriately.
dnl
dnl 2010-02-01 Antonio Boveia (boveia@hep.uchicago.edu)


AC_DEFUN([AX_PLPLOT],
[

  if test -e "/share/ftkdata/boveia/plplot/lib/libplplotcxxd.so"; then
    user_plplot="/share/ftkdata/boveia/plplot"
  else
    user_plplot="none"
  fi

dnl retrieve path to library
  AC_ARG_WITH(plplot,
    AC_HELP_STRING([--with-plplot  the PLPLOT installation directory]),[
    if test -d "$withval"; then
        user_plplot=$withval
    fi
  ]) 

  PLPLOT_CPPFLAGS=
  PLPLOT_LDFLAGS=
  PLPLOT_INCDIR=
  PLPLOT_LIBDIR=

  AC_MSG_CHECKING([for PLPLOT libraries])
  if test -d "$user_plplot" ; then
    PLPLOT_INCDIR="${user_plplot}/include"
    PLPLOT_LIBDIR="${user_plplot}/lib"
    if test -d "${PLPLOT_INCDIR}" && test -d "${PLPLOT_LIBDIR}" && test -e "${PLPLOT_LIBDIR}/libplplotcxxd.so"; then
       dnl PLPLOT_CPPFLAGS="-I${PLPLOT_INCDIR}"
       dnl PLPLOT_LDFLAGS="-L${PLPLOT_LIBDIR} -lplplotcxxd"
       dnl AC_DEFINE(HAVE_PLPLOT,,[define if the PLPLOT object library is available])
       dnl AC_MSG_RESULT(yes)
       dnl disable plplot for now due to fortran incompatibilities
       AC_MSG_RESULT(no)
    else
       AC_MSG_RESULT(no)
    fi
  else
    AC_MSG_RESULT(no)
  fi
  
  AC_SUBST(PLPLOT_INCDIR)
  AC_SUBST(PLPLOT_LIBDIR)
  AC_SUBST(PLPLOT_CPPFLAGS)
  AC_SUBST(PLPLOT_LDFLAGS)

])
