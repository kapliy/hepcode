
dnl checks for libxml2. defines HAVE_LIBXML2 in config.h if libxml2 is found.
dnl if it exists, then XML_CPPFLAGS, XML_LDFLAGS, and XML_INCDIR are
dnl set appropriately. based on http://www.mail-archive.com/gnash-dev@gnu.org/msg02205/libXML.m4.
dnl
dnl 2010-06-14 Antonio Boveia (boveia@hep.uchicago.edu)

AC_DEFUN([AX_XML],
[
  AC_PATH_PROG(XML2_CONFIG, xml2-config, ,[/usr/bin:/usr/local/bin])
  # Look for the header
  XML_INCDIR=
  XML_CPPFLAGS=
  # xml2-config will give us the answer
  AC_MSG_CHECKING([for libxml2 headers])
  if test "x$XML2_CONFIG" != "x" ; then
    XML_CPPFLAGS=`$XML2_CONFIG --cflags`
    XML_INCDIR=`xml2-config --cflags | sed 's/\-I\([\/A-Za-z\-_0-9\.]*\).*/\1/'`
    AC_MSG_RESULT($XML_CPPFLAGS)
  else
    # try /usr/include
    if test -e "/usr/include/libxml2/xmlmemory.h" ; then
      XML_INCDIR = "/usr/include"
      XML_CPPFLAGS="-I$XML_INCDIR"
      AC_MSG_RESULT($XML_CPPFLAGS)
    else
      if test -e "/usr/local/include/libxml2/xmlmemory.h" ; then
        XML_INCDIR = "/usr/local/include"
        XML_CPPFLAGS="-I$XML_INCDIR"
        AC_MSG_RESULT($XML_CPPFLAGS)
      fi
    fi
  fi
  # Look for the library
  XML_LDFLAGS=
  # xml2-config will give us the answer
  AC_MSG_CHECKING([for libxml2 library])
  if test "x$XML2_CONFIG" != "x" ; then
    XML_LDFLAGS=`$XML2_CONFIG --libs`
    AC_MSG_RESULT($XML_LDFLAGS)
  else
    # try /usr/lib
    xmldir=`ls /usr/lib/libxml2* | sed 's/\..*//' | head -n1 | sed 's/\n//'`
    if test -d ${xmldir} ; then
      XML_LDFLAGS="-L$xmldir -lxml2"
      AC_MSG_RESULT($XML_LDFLAGS)
    else
      xmldir=`ls /usr/local/lib/libxml2* | sed 's/\..*//' | head -n1 | sed 's/\n//'`
      if test -d ${xmldir} ; then
        XML_LDFLAGS="-L$xmldir -lxml2"
        AC_MSG_RESULT($XML_LDFLAGS)
      fi
    fi
  fi
  if test x"${XML_CPPFLAGS}" != x ; then
    if test x"${XML_LDFLAGS}" != x ; then
      LIBXML_LIBS="${ac_cv_path_libxml_lib}"
       AC_DEFINE(HAVE_LIBXML2,, [define if the XML2 library is available])
    fi
  fi
  AC_SUBST(XML_CPPFLAGS)
  AC_SUBST(XML_LDFLAGS)
  AC_SUBST(XML_INCDIR)
])
