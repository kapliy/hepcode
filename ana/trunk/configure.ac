m4_include([version.m4])
AC_INIT(TrigFTKAna,VERSION_NUMBER,[A. Boveia <boveia@hep.uchicago.edu>])
AM_CONFIG_HEADER(TrigFTKAna_config.h)
AM_INIT_AUTOMAKE

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
GPROFFLAGS="-pg"
# defaults
if test X$user_set_cxxflags != Xyes; then
dnl "-O2 -fno-inline" turn on optimizations but do not inline (makes debugging easier)
dnl -Wno-deprecated to avoid warnings about deprecated headers such as
dnl                 hash_map included in boost adjacency list.
  AC_MSG_NOTICE(Hardcoding CXXFLAGS)
  CXXFLAGS="$XTRAARCHFLAGS -O2 -g -fno-inline -Wno-deprecated"
  CXXFLAGS="$XTRAARCHFLAGS -O2 -g"
  LDFLAGS="$XTRAARCHLDFLAGS -O2 -g"
else
  AC_MSG_NOTICE(Relying on system-default CXXFLAGS)
fi
AC_MSG_NOTICE(CXXFLAGS are $CXXFLAGS)
AC_MSG_NOTICE(LDFLAGS are $LDFLAGS)

dnl check for ROOT.
dnl see root-config man page.
AC_MSG_NOTICE(TrigFTKAna requires ROOT. Checking for it...)
ROOT_PATH

dnl check for boost libraries
AC_MSG_NOTICE(TrigFTKAna requires the Boost libraries. Checking for them...)
AX_BOOST_BASE([1.53.0])
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
dnl AC_MSG_NOTICE("Checking for libxml2")
AX_XML

dnl check for TrigFTKSim
AC_MSG_NOTICE(TrigFTKAna can use TrigFTKSim output TObject libraries. Checking for them...)
AX_TRIGFTKSIM

dnl check for libreadline
AX_READLINE

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

# disable trigger matching code (memory-error-prone)
disable_trigger_matching=true
if test "x${disable_trigger_matching}" = "xtrue" ; then
   AC_DEFINE(DISABLE_TRIGGER_MATCHING,,[Disable trigger matching to reduce memory footprint])
fi
AM_CONDITIONAL([DISABLE_TRIGGER_MATCHING], [test x${disable_trigger_matching} = xtrue])

dnl check for CommonAnalysis source
AC_DEFINE([NO_ANALYSIS_MSG],["FATAL: TrigFTKAna/CommonAnalysis/ packages were not compiled, but are necessary to call this function. Please run \"source bootstrap.sh RootCore\" before compiling TrigFTKAna to fix this problem."],[A message that will be printed during runtime if analysis dependencies were not compiled])
AX_COMMONANALYSIS
AC_MSG_NOTICE([RootCore LDFLAGS are $COMMONANALYSIS_LDFLAGS])
AC_MSG_NOTICE([RootCore CPPFLAGS are $COMMONANALYSIS_CPPFLAGS])

AC_CONFIG_FILES([
                Makefile
                m4/Makefile
                dg/Makefile
                src/Makefile
                src/mcp/Makefile
                src/misc/Makefile
                src/Trees/Makefile
                ana/Makefile
           ])

AC_OUTPUT


