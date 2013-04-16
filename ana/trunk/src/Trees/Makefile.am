
library_includedir = $(includedir)

library_include_HEADERS = $(top_srcdir)/TrigFTKAna/*.hpp

bin_PROGRAMS =

ACLOCAL_AMFLAGS = -I$(top_srcdir)/m4

lib_LIBRARIES = libTrigFTKAnaNtD3PDPeter.a

libTrigFTKAnaNtD3PDPeter_a_SOURCES = D3PDPeterTreeV29g.cpp D3PDPeterTreeV29i.cpp D3PDPeterTreeV29l.cpp NtD3PDphysPeter.cpp
libTrigFTKAnaNtD3PDPeter_a_CXXFLAGS = $(BOOST_CPPFLAGS) $(DCAP_CPPFLAGS) $(XML_CPPFLAGS) @ROOTCFLAGS@ @ROOTAUXCFLAGS@ $(ROOT_CPPFLAGS) $(FTKSIM_CPPFLAGS) -fno-inline

CLEANFILES              = *Cint.cxx *Cint.h *.o lib*.a $(lib_LIBRARIES)
