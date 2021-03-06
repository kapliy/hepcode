m4_include([version.m4])
AC_INIT(TrigFTKAna,VERSION_NUMBER,[A. Boveia <boveia@hep.uchicago.edu>])
dnl          AC_CONFIG_SRCDIR([src/foo.c])
AM_CONFIG_HEADER(TrigFTKAna_config.h)
AM_INIT_AUTOMAKE

dnl AC_INIT(TrigFTKAna/AnaEventMgr.hpp,VERSION_NUMBER,[A. Boveia <boveia@hep.uchicago.edu>])
dnl AM_INIT_AUTOMAKE(TrigFTKAna,VERSION_NUMBER)
AC_CANONICAL_HOST
AC_PROG_INSTALL
AC_PROG_RANLIB
dnl Add support for shared libraries. disable AC_PROG_RANLIB above.
dnl AC_DISABLE_STATIC
dnl AC_PROG_LIBTOOL
AC_CONFIG_MACRO_DIR([m4])
AC_HEADER_STDC
AC_C_CONST
AC_TYPE_SIZE_T

AC_MSG_NOTICE(TrigFTKAna can be built 64-bit or 32-bit;)
enable_64bit=no
AC_ARG_ENABLE([64-bit],
    [AS_HELP_STRING([--enable-64-bit],
        [build and link against 64-bit code @<:@default=no@:>@])],
    [enable_64bit=yes]
)
XTRAARCHFLAGS="-m32"
XTRAARCHLDFLAGS="-m32 -L../"
if test "x$enable_64bit" = xno; then
   dnl defaults are for 32 bit binaries
   AC_MSG_NOTICE(building 32-bit binaries.)
else
   XTRAARCHFLAGS="-m64"
   XTRAARCHLDFLAGS="-m64 -L../ -L /usr/lib64"
   AC_MSG_NOTICE(building 64-bit binaries.)
fi

AC_MSG_NOTICE(TrigFTKAna requires a c++ compiler. Checking for it...)
# set optimization level
if test -n "${CXXFLAGS}"; then
  user_set_cxxflags=yes
fi
AC_PROG_CC
AC_PROG_CXX
dnl flags for optional gcov output
GCOVFLAGS="-fprofile-arcs -ftest-coverage -O0"
dnl flags for optional gprof output
GPROFFLAGS="-pg -O2 -g"
if test X$user_set_cxxflags != Xyes; then
dnl force 32-bit binary, and link against 32-bit libraries. see also
dnl fortran flags.
dnl "-O2 -fno-inline" turn on optimizations but do not inline (makes debugging easier)
dnl -Wno-deprecated to avoid warnings about deprecated headers such as
dnl                 hash_map included in boost adjacency list.
  CXXFLAGS="$XTRAARCHFLAGS -O0 -g -fno-inline -Wno-deprecated"
  CXXFLAGS="$XTRAARCHFLAGS -O2 -g -fno-inline -Wno-deprecated"
  CXXFLAGS="$XTRAARCHFLAGS -O3 -g -fno-inline -Wno-deprecated"
  CXXFLAGS="$XTRAARCHFLAGS -O2 -g -fno-inline -Wno-deprecated"
  LDFLAGS="$XTRAARCHLDFLAGS" 
fi
AC_MSG_NOTICE(CXXFLAGS are $CXXFLAGS)
AC_MSG_NOTICE(LDFLAGS are $LDFLAGS)
AC_MSG_NOTICE(TrigFTKAna requires a FORTRAN compiler. Checking for it...)
AC_F77_WRAPPERS
AC_F77_LIBRARY_LDFLAGS
AC_F77_MAIN([MAIN__])
dnl FORTRAN linking for CTVMFT
if test -n "${FLIBS}"; then
   FFLAGS="$XTRAARCHFLAGS"
   FLIBS="$XTRAARCHLDFLAGS"
   dnl AK FIXME - remove /usr/lib/gcc/* stuff from FLIBS on 64-bit uct3-c001 (TODO - make more general - maybe in m4)
   dnl FLIBS="$(FLIBS) $XTRAARCHLDFLAGS -L./ -lfrtbegin -lg2c -lm -lgcc_s"   
   dnl FLIBS="-lfrtbegin -lm $XTRAARCHLDFLAGS"
   FLIBS="-lfrtbegin -lg2c -lm -lgcc_s $XTRAARCHLDFLAGS"
   dnl FLIBS="-L/usr/local/lib -lgfortran $XTRAARCHLDFLAGS"
   AC_MSG_NOTICE(FFLAGS set to $FFLAGS)
   AC_MSG_NOTICE(FLIBS set to $FLIBS)
else
   AC_MSG_NOTICE(setting FLIBS and FFLAGS)
   FFLAGS="$(FFLAGS) $XTRAARCHFLAGS"
   FLIBS="$(FLIBS) $XTRAARCHLDFLAGS -L./ -lfrtbegin -lg2c -lm -lgcc_s"
   dnl FLIBS="$(FLIBS) $XTRAARCHLDFLAGS -L./ -L/usr/local/lib -lgfortran"
   dnl FLIBS="$(FLIBS) $XTRAARCHLDFLAGS -L./ -lfrtbegin -lm"
fi
if test "x$F77" = "xgfortran" ; then
   AC_DEFINE(HAVE_GFORTRAN,,[define using gfortran to compile ctvmft])
fi

dnl check for ROOT.
dnl see root-config man page.
AC_MSG_NOTICE(TrigFTKAna requires ROOT. Checking for it...)
ROOT_PATH

dnl check for boost libraries
AC_MSG_NOTICE(TrigFTKAna requires the Boost libraries. Checking for them...)
AX_BOOST_BASE([1.42.0])
AX_BOOST_REGEX
AX_BOOST_PROGRAM_OPTIONS
AX_BOOST_IOSTREAMS
AX_BOOST_SYSTEM
AX_BOOST_FILESYSTEM
AX_BOOST_DATE_TIME
AX_BOOST_SERIALIZATION

dnl check for libdcap
AC_MSG_NOTICE(TrigFTKAna can use libdcap to access dCache filesystems. Checking for it...)
AX_DCAP(["$enable_64bit"])

dnl check for libxml2
dnl AC_MSG_NOTICE("TrigFTKAna can use libXML to read XML good run lists.")
dnl AC_MSG_NOTICE("If not found, the reads will return empty lists.")
dnl AC_MSG_NOTICE("Checking for it...")
AX_XML

dnl disabled check for CTVMFT. 
dnl the source for CTVMFT is now included within TrigFTKAna.
dnl AX_CTVMFT

dnl check for TrigFTKSim
AC_MSG_NOTICE(TrigFTKAna can use TrigFTKSim output TObject libraries. Checking for them...)
AX_TRIGFTKSIM

dnl check for libreadline
AX_READLINE

dnl check for plplot
dnl AX_PLPLOT

use_enable_all_ana=false
AC_ARG_ENABLE([allana],
	[AS_HELP_STRING([--enable-allana],[Enable the compilation of all the analysis code])],
    AM_CONDITIONAL([MAKE_ALLANA], [test xtrue = xtrue])
    use_enable_all_ana=true
)
AM_CONDITIONAL([MAKE_ALLANA], [test xtrue = x$use_enable_all_ana])

# optional CommonHiggs library for ana_lowmh executable
use_commonhiggs_build=xfalse
AM_CONDITIONAL([USE_COMMONHIGGS], [test x$use_commonhiggs_build = xtrue])

# disable old code to reduce compilation time
remove_ancient_code=true
if test "x${remove_ancient_code}" = "xtrue" ; then
   AC_DEFINE(DISABLE_ANCIENT,,[Disable older versions of various packages to reduce compilation time])
fi
AM_CONDITIONAL([DISABLE_ANCIENT], [test x${remove_ancient_code} = xtrue])

dnl check for CommonAnalysis source
AC_DEFINE([NO_ANALYSIS_MSG],["FATAL: TrigFTKAna/CommonAnalysis/ packages were not compiled, but are necessary to call this function. Please run \"source bootstrap.sh RootCore\" before compiling TrigFTKAna to fix this problem."],[A message that will be printed during runtime if analysis dependencies were not compiled])
AX_COMMONANALYSIS
AC_MSG_NOTICE([RootCore LDFLAGS are $COMMONANALYSIS_LDFLAGS])
AC_MSG_NOTICE([RootCore CPPFLAGS are $COMMONANALYSIS_CPPFLAGS])

AC_CONFIG_FILES([
                Makefile
                m4/Makefile
                dg/Makefile
                plot/Makefile
                src/Makefile
                src/egamma/Makefile
                src/mcp/Makefile
                src/jetetmiss/Makefile
                src/misc/Makefile
                src/ctvmft/Makefile
                src/Trees/Makefile
                test/Makefile
                ana/Makefile
           ])

AC_OUTPUT


