
dnl checks for libdcap. defines HAVE_LIBDCAP in config.h if libdcap is found.
dnl if it exists, then DCAP_CPPFLAGS, DCAP_LDFLAGS, DCAP_INCDIR, and DCAP_LIBDIR are
dnl set appropriately. 
dnl
dnl 2009-04-30 Antonio Boveia (boveia@hep.uchicago.edu)



AC_DEFUN([AX_DCAP],
[
  dnl retrieve path to libdcap base directory.
  AC_ARG_WITH(dcap,
    AC_HELP_STRING([--with-dcap  top of the dcap installation directory]),[
    if test -d "$withval"; then
        user_dcap=$withval
    elif test -d "$DCACHEDIR/dcap"; then
        user_dcap="$DCACHEDIR/dcap"
    elif test -d "/opt/d-cache/dcap"; then
        user_dcap="/opt/d-cache/dcap"
    else
        user_dcap="none"
    fi
  ]) 

  DCAP_INCDIR=
  DCAP_LIBDIR=
  DCAP_LDFLAGS=
  DCAP_CPPFLAGS=

  AC_MSG_CHECKING([for dcache dcap headers and libraries])
  if test -d "$user_dcap" ; then
    DCAP_INCDIR="${user_dcap}/include"
    DCAP_LIBDIR="${user_dcap}/lib"
    if test x[$1] = xyes ; then
       DCAP_LIBDIR="${user_dcap}/lib64"
    fi
    if test -d "${DCAP_INCDIR}" && test -d "${DCAP_LIBDIR}" ; then
       DCAP_LDFLAGS="-L$DCAP_LIBDIR -ldcap"
       DCAP_CPPFLAGS="-I$DCAP_INCDIR"
       AC_DEFINE(HAVE_LIBDCAP,,[define if the DCAP dCache library is available])
       AC_MSG_RESULT(yes)
    else
       AC_MSG_RESULT(no)
    fi
  else
    AC_MSG_RESULT(no)
  fi
  
  AC_SUBST(DCAP_INCDIR)
  AC_SUBST(DCAP_LIBDIR)
  AC_SUBST(DCAP_LDFLAGS)
  AC_SUBST(DCAP_CPPFLAGS)

])
