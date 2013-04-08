
library_includedir = $(includedir)

library_include_HEADERS = $(top_srcdir)/TrigFTKAna/*.hpp

SUBDIRS = egamma mcp jetetmiss misc ctvmft Trees

bin_PROGRAMS =

ACLOCAL_AMFLAGS = -I$(top_srcdir)/m4

lib_LIBRARIES = libTrigFTKAnaBase.a libTrigFTKAna.a
#noinst_LTLIBRARIES = libTrigFTKAna.la

libTrigFTKAnaBase_a_SOURCES = Detector.cpp GoodRunList.cpp AnaComposite.cpp AnaElectron.cpp AnaMuon.cpp AnaMET.cpp AnaPhoton.cpp AnaJet.cpp AnaW.cpp AnaVertexFit.cpp AnaJetTrackAssociation.cpp ana_streams.cpp StudyTrackingEfficiency.cpp StudyTrackComparison.cpp StudyJetTag.cpp StudyFakeLeptonsInZEvents.cpp AnaConfiguration.cpp AnaMatching.cpp Likelihood1D.cpp LikelihoodParameterization1D.cpp JetTagger.cpp SignedD0JetTagger.cpp SignedD0ParameterizedJetTagger.cpp
libTrigFTKAnaBase_a_CXXFLAGS = $(BOOST_CPPFLAGS) $(DCAP_CPPFLAGS) $(XML_CPPFLAGS) @ROOTCFLAGS@ @ROOTAUXCFLAGS@ $(ROOT_CPPFLAGS) $(FTKSIM_CPPFLAGS) $(COMMONANALYSIS_CPPFLAGS)

libTrigFTKAna_a_SOURCES = AnaEventMgr.cpp AnaEvent.cpp AnaEventNtuple.cpp AnaEventNtuple_read.cpp AnaEventNtuple_write.cpp
libTrigFTKAna_a_CXXFLAGS = $(BOOST_CPPFLAGS) $(DCAP_CPPFLAGS) $(XML_CPPFLAGS) @ROOTCFLAGS@ @ROOTAUXCFLAGS@ $(ROOT_CPPFLAGS) $(FTKSIM_CPPFLAGS)

CLEANFILES              = *Cint.cxx *Cint.h *.o lib*.a $(lib_LIBRARIES)
