
dnl checks for CommonHiggs.  if it exists, then COMMONHIGGS_CPPFLAGS,
dnl COMMONHIGGS_LDFLAGS, COMMONHIGGS_INCDIR, and COMMONHIGGS_LIBDIR
dnl are set appropriately, and TrigFTKAna will attempt to build the
dnl CommonHiggs libraries.
dnl
dnl 2011-05-20 Antonio Boveia (boveia@hep.uchicago.edu)


AC_DEFUN([AX_COMMONHIGGS],
[
  dnl retrieve path to CommonHiggs base directory.
  user_commonhiggs="${PWD}/CommonHiggs"
  AC_ARG_WITH(commonhiggs,
    AC_HELP_STRING([--with-commonhiggs the CommonHiggs directory]),[
    if test -d "$withval"; then
        user_commonhiggs=$withval
    else
        user_commonhiggs="none"
    fi
  ])

  COMMONHIGGS_INCDIR=
  COMMONHIGGS_LIBDIR=
  COMMONHIGGS_LDFLAGS=
  COMMONHIGGS_CPPFLAGS=

  dnl disabled for now. no need.

  AC_MSG_CHECKING([for CommonHiggs headers and libraries])
  if test -d "$user_commonhiggs/EnergyRescalerTool" ; then
    dnl COMMONHIGGS_INCDIR="${user_commonhiggs}/EnergyRescalerTool"
    dnl COMMONHIGGS_LIBDIR="${user_commonhiggs}/"
    if test -d "${COMMONHIGGS_INCDIR}" && test -d "${COMMONHIGGS_LIBDIR}"; then
       COMMONHIGGS_LDFLAGS="-L$COMMONHIGGS_LIBDIR -lCommonHiggs"
       COMMONHIGGS_CPPFLAGS="-I$COMMONHIGGS_INCDIR"
       AC_DEFINE(HAVE_COMMONHIGGS,,[define if the CommonHiggs package is available])
       AC_MSG_RESULT(yes)
    else
       AC_MSG_RESULT(no)
    fi
  else
    AC_MSG_RESULT(no)
  fi
  AC_SUBST(COMMONHIGGS_INCDIR)
  AC_SUBST(COMMONHIGGS_LIBDIR)
  AC_SUBST(COMMONHIGGS_LDFLAGS)
  AC_SUBST(COMMONHIGGS_CPPFLAGS)

])
