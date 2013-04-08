
library_includedir = $(includedir)

library_include_HEADERS = $(top_srcdir)/TrigFTKAna/*.hpp

SUBDIRS = 

bin_PROGRAMS =

ACLOCAL_AMFLAGS = -I$(top_srcdir)/m4

lib_LIBRARIES = libTrigFTKAnaBaseEgamma.a

libTrigFTKAnaBaseEgamma_a_SOURCES = ScaleWjets.cpp 
libTrigFTKAnaBaseEgamma_a_CXXFLAGS = $(BOOST_CPPFLAGS) @ROOTCFLAGS@ @ROOTAUXCFLAGS@ $(ROOT_CPPFLAGS)

CLEANFILES              = *Cint.cxx *Cint.h *.o $(lib_LIBRARIES)
