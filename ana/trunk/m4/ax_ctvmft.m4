
dnl checks for ctvmft.
dnl if it exists, then CTVMFT_LDFLAGS and CTVMFT_LIBDIR are
dnl set appropriately. 
dnl
dnl 2009-10-23 Antonio Boveia (boveia@hep.uchicago.edu)


AC_DEFUN([AX_CTVMFT],
[

  if test -e "/osg/app/atlas_app/atlas_rel/15.4.0/external/CTVMFT/v1.0/i686-slc4-gcc34-opt/libctvmft.a"; then
    user_ctvmft="/osg/app/atlas_app/atlas_rel/15.4.0/external/CTVMFT/v1.0/i686-slc4-gcc34-opt"
  elif test -d "/osg/app/atlas_app/atlas_rel/15.4.0/external/CTVMFT/v1.0/i686-slc4-gcc34-opt"; then
    user_ctvmft="/osg/app/atlas_app/atlas_rel/15.4.0/external/CTVMFT/v1.0/i686-slc4-gcc34-opt"
  elif test -d "/afs/cern.ch/atlas/software/releases/15.4.0/external/CTVMFT/v1.0/i686-slc4-gcc34-opt"; then
    user_ctvmft="/afs/cern.ch/atlas/software/releases/15.4.0/external/CTVMFT/v1.0/i686-slc4-gcc34-opt"
  elif test -d "${PWD}/CTVMFT"; then
    user_ctvmft="${PWD}/CTVMFT"
  else
    user_ctvmft="none"
  fi

dnl retrieve path to library
  AC_ARG_WITH(ctvmft,
    AC_HELP_STRING([--with-ctvmft  the CTVMFT installation directory]),[
    if test -d "$withval"; then
        user_ctvmft=$withval
    fi
  ]) 

  CTVMFT_LDFLAGS=
  CTVMFT_LIBDIR=

  AC_MSG_CHECKING([for CTVMFT library])
  if test -d "$user_ctvmft" ; then
    CTVMFT_LIBDIR="${user_ctvmft}"
    if test -d "${CTVMFT_LIBDIR}" && test -e "${CTVMFT_LIBDIR}/libctvmft.a"; then
       CTVMFT_LDFLAGS="-L$CTVMFT_LIBDIR -lctvmft"
       AC_DEFINE(HAVE_CTVMFT,,[define if the CTVMFT object library is available])
       AC_MSG_RESULT(yes)
    else
       AC_MSG_RESULT(no)
       AC_MSG_ERROR(CTVMFT library is required, but path {$user_ctvmft} does not contain it. Specify valid path with --with-ctvmft=/path/to/CTVMFT)
    fi
  else
    AC_MSG_RESULT(no)
    AC_MSG_ERROR(CTVMFT library is required. Specify path with --with-ctvmft=/path/to/CTVMFT)
  fi
  
  AC_SUBST(CTVMFT_LIBDIR)
  AC_SUBST(CTVMFT_LDFLAGS)

])
