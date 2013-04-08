
library_includedir = $(includedir)

library_include_HEADERS = $(top_srcdir)/TrigFTKAna/*.hpp

bin_PROGRAMS =

ACLOCAL_AMFLAGS = -I$(top_srcdir)/m4

lib_LIBRARIES = libTrigFTKAnaMisc.a

libTrigFTKAnaMisc_a_SOURCES = UnfoldingHistogramTool.cpp
libTrigFTKAnaMisc_a_CXXFLAGS = $(BOOST_CPPFLAGS) @ROOTCFLAGS@ @ROOTAUXCFLAGS@ $(ROOT_CPPFLAGS)

CLEANFILES              = *Cint.cxx *Cint.h *.o $(lib_LIBRARIES)
