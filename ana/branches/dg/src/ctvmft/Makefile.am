
library_includedir = $(includedir)

library_include_HEADERS = $(top_srcdir)/TrigFTKAna/*.hpp

bin_PROGRAMS =

ACLOCAL_AMFLAGS = -I$(top_srcdir)/m4

lib_LIBRARIES = libTrigFTKAnaCTVMFT.a

libTrigFTKAnaCTVMFT_a_SOURCES = ctvmfr_address.F ctvmft.F ctvmq_address.F dfact.F dfinv.F dinv.F diuapp.F fiddle_address.F gettrk.F mydeqinv.F mydeqn.F mydfact.F mydfeqn.F mydfinv.F trkprm_address.F vzero.cpp
libTrigFTKAnaCTVMFT_a_CXXFLAGS = @ROOTCFLAGS@ @ROOTAUXCFLAGS@ $(ROOT_CPPFLAGS)

CLEANFILES              = *Cint.cxx *Cint.h *.o $(lib_LIBRARIES)
