
library_includedir = $(includedir)

library_include_HEADERS = $(top_srcdir)/TrigFTKAna/*.hpp

SUBDIRS = mcp misc Trees

bin_PROGRAMS =

ACLOCAL_AMFLAGS = -I$(top_srcdir)/m4

lib_LIBRARIES = libTrigFTKAnaBase.a libTrigFTKAna.a

libTrigFTKAnaBase_a_SOURCES = Detector.cpp AnaComposite.cpp AnaElectron.cpp AnaMuon.cpp AnaMET.cpp AnaPhoton.cpp AnaJet.cpp AnaW.cpp ana_streams.cpp AnaConfiguration.cpp
libTrigFTKAnaBase_a_CXXFLAGS = $(BOOST_CPPFLAGS) $(DCAP_CPPFLAGS) $(XML_CPPFLAGS) @ROOTCFLAGS@ @ROOTAUXCFLAGS@ $(ROOT_CPPFLAGS) $(FTKSIM_CPPFLAGS) $(COMMONANALYSIS_CPPFLAGS)

libTrigFTKAna_a_SOURCES = AnaEventMgr.cpp AnaEvent.cpp AnaEventNtuple.cpp
libTrigFTKAna_a_CXXFLAGS = $(BOOST_CPPFLAGS) $(DCAP_CPPFLAGS) $(XML_CPPFLAGS) @ROOTCFLAGS@ @ROOTAUXCFLAGS@ $(ROOT_CPPFLAGS) $(FTKSIM_CPPFLAGS)

CLEANFILES              = *Cint.cxx *Cint.h *.o lib*.a $(lib_LIBRARIES)
