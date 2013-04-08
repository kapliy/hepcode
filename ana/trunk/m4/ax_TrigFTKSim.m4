
dnl checks for TrigFTKSim. this must exist.
dnl if it exists, then FTKSIM_CPPFLAGS, FTKSIM_LDFLAGS, FTKSIM_INCDIR, and FTKSIM_LIBDIR are
dnl set appropriately. 
dnl
dnl 2009-04-30 Antonio Boveia (boveia@hep.uchicago.edu)



AC_DEFUN([AX_TRIGFTKSIM],
[
  dnl retrieve path to ftksim base directory.
  AC_ARG_WITH(ftksim,
    AC_HELP_STRING([--with-ftksim  the TrigFTKSim installation directory]),[
    if test -d "$withval"; then
        user_ftksim=$withval
    elif test -d "../TrigFTKSim"; then
        user_ftksim="${PWD}/../TrigFTKSim"
    elif test -d "${PWD}/TrigFTKSim"; then
        user_ftksim="${PWD}/TrigFTKSim"
    else
        user_ftksim="none"
    fi
  ]) 

  FTKSIM_INCDIR=
  FTKSIM_LIBDIR=
  FTKSIM_LDFLAGS=
  FTKSIM_CPPFLAGS=
  FTKSIM_LIBS=

  AC_MSG_CHECKING([for ftksim headers and libraries])
  if test -d "$user_ftksim" ; then
    FTKSIM_INCDIR="${user_ftksim}"
    FTKSIM_LIBDIR="${user_ftksim}/standalone"
    if test -d "${FTKSIM_INCDIR}" && test -d "${FTKSIM_LIBDIR}" && test -e "${FTKSIM_LIBDIR}/libftk_classes.so"; then
       FTKSIM_LDFLAGS="-L$FTKSIM_LIBDIR -lftk_classes"
       FTKSIM_CPPFLAGS="-I$FTKSIM_INCDIR -DFTK_STANDALONE"
       FTKSIM_LIBS="${FTKSIM_LIBDIR}/libTrigFTKSim.a"
       AC_DEFINE(HAVE_LIBFTKSIM,,[define if the TrigFTKSim object library is available])
       AC_MSG_RESULT(yes)
    else
       AC_MSG_RESULT(no)
       #AC_MSG_ERROR(TrigFTKSim library is required, but path {$user_ftksim} does not contain it. Specify valid path with --with-ftksim=/path/to/TrigFTKSim)
    fi
  else
    AC_MSG_RESULT(no)
    #AC_MSG_ERROR(TrigFTKSim library is required. Specify path with --with-ftksim=/path/to/TrigFTKSim)
  fi
  
  AC_SUBST(FTKSIM_INCDIR)
  AC_SUBST(FTKSIM_LIBDIR)
  AC_SUBST(FTKSIM_LIBS)
  AC_SUBST(FTKSIM_LDFLAGS)
  AC_SUBST(FTKSIM_CPPFLAGS)

])
