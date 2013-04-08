
ACLOCAL_AMFLAGS = -I$(top_srcdir)/m4


bin_PROGRAMS =

library_includedir = $(includedir)
library_include_HEADERS = $(top_srcdir)/DataGraphics/*.hh

lib_LIBRARIES = libdatagraphics.a
#noinst_LTLIBRARIES = libdatagraphics.la
libdatagraphics_a_SOURCES = DataGraphicsCint.cxx DgMemory.cpp DgTools.cc DgCollection.cc DgAbsGraphic.cc DgBinomialErrors.cc \
	DgEfficiency0D.cc DgEfficiency1D.cc DgEfficiency2D.cc \
	DgEfficiencyWeighted0D.cc \
	DgGraph2D.cc DgNtuple.cc \
	DgTH1D.cc DgTH2D.cc DgTH3D.cc \
	DgBin.cc \
	DgHistogram1D.cc DgHistogram2D.cc \
	DgHistogramWeighted1D.cc DgHistogramWeighted2D.cc \
	DgProfile1D.cc DgGxLegend.cc DgResourceMgr.cc
libdatagraphics_a_CXXFLAGS = $(BOOST_CPPFLAGS) $(DCAP_CPPFLAGS) @ROOTCFLAGS@ @ROOTAUXCFLAGS@ $(ROOT_CPPFLAGS) $(FTKSIM_CPPFLAGS)

DATAGRAPHICS_TOBJECTS = $(top_srcdir)/DataGraphics/DgEfficiency0D.hh \
	$(top_srcdir)/DataGraphics/DgEfficiency1D.hh \
	$(top_srcdir)/DataGraphics/DgEfficiency2D.hh \
	$(top_srcdir)/DataGraphics/DgEfficiencyWeighted0D.hh \
	$(top_srcdir)/DataGraphics/DgTH1D.hh \
	$(top_srcdir)/DataGraphics/DgTH2D.hh \
	$(top_srcdir)/DataGraphics/DgTH3D.hh \
	$(top_srcdir)/DataGraphics/DgBin.hh \
	$(top_srcdir)/DataGraphics/DgHistogram1D.hh \
	$(top_srcdir)/DataGraphics/DgHistogram2D.hh \
	$(top_srcdir)/DataGraphics/DgGraph2D.hh \
	$(top_srcdir)/DataGraphics/DgNtuple.hh \
	$(top_srcdir)/DataGraphics/DgHistogramWeighted1D.hh \
	$(top_srcdir)/DataGraphics/DgHistogramWeighted2D.hh \
	$(top_srcdir)/DataGraphics/DgProfile1D.hh

# OK we have to make this whole makefile non-parallel, otherwise DataGraphicsCint.cxx errors out
.NOTPARALLEL: DataGraphicsCint.cxx
DataGraphicsCint.cxx DataGraphicsCint.h : $(DATAGRAPHICS_TOBJECTS) DataGraphics_LinkDef.h
	@ROOTCINT@ -f DataGraphicsCint.cxx -c -I../ $(BOOST_CPPFLAGS) $(DATAGRAPHICS_TOBJECTS) DataGraphics_LinkDef.h

CLEANFILES              = *Cint.cxx *Cint.h *.o $(lib_LIBRARIES)

