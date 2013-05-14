
ACLOCAL_AMFLAGS = -I$(top_srcdir)/m4

bin_PROGRAMS =

library_includedir = $(includedir)
library_include_HEADERS = $(top_srcdir)/DataGraphics/*.hh

lib_LIBRARIES = libdatagraphics.a
libdatagraphics_a_SOURCES = DgMemory.cpp DgTools.cc DgCollection.cc \
	DgBinomialErrors.cc DgBin.cc
libdatagraphics_a_CXXFLAGS = $(BOOST_CPPFLAGS) $(DCAP_CPPFLAGS) @ROOTCFLAGS@ @ROOTAUXCFLAGS@ $(ROOT_CPPFLAGS) $(FTKSIM_CPPFLAGS) $(COMMONANALYSIS_CPPFLAGS)

CLEANFILES              = *Cint.cxx *Cint.h *.o $(lib_LIBRARIES)
