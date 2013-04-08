
library_includedir = $(includedir)

library_include_HEADERS = $(top_srcdir)/TrigFTKAna/*.hpp

bin_PROGRAMS =

ACLOCAL_AMFLAGS = -I$(top_srcdir)/m4

lib_LIBRARIES = libTrigFTKAnaNtFTK.a libTrigFTKAnaNtD3PDPeter.a libTrigFTKAnaNtD3PDJetMET.a libTrigFTKAnaNtMisc.a

libTrigFTKAnaNtFTK_a_SOURCES = NtWrapper.cpp NtTrigFTKLib.cpp NtRoad.cpp NtFitter.cpp NtCBNT.cpp
libTrigFTKAnaNtFTK_a_CXXFLAGS = $(BOOST_CPPFLAGS) $(DCAP_CPPFLAGS) $(XML_CPPFLAGS) @ROOTCFLAGS@ @ROOTAUXCFLAGS@ $(ROOT_CPPFLAGS) $(FTKSIM_CPPFLAGS)

# disable inlining for these large TTree::MakeClass headers. since the
# Nt* source files include the TTree::MakeClass header files, these
# will need to be compiled without inlining also. this should be ok,
# since only AnaEventMgr calls the Nt* class functions and this
# happens only during the event load.

if DISABLE_ANCIENT
libTrigFTKAnaNtD3PDPeter_a_SOURCES = D3PDPeterTreeV29.cpp D3PDPeterTreeV29g.cpp D3PDPeterTreeV29i.cpp D3PDPeterTreeV29l.cpp D3PDPeterTreeV30.cpp D3PDPeterTreeV50.cpp NtD3PDphysPeter.cpp
else
libTrigFTKAnaNtD3PDPeter_a_SOURCES = D3PDPeterTreeV29.cpp D3PDPeterTreeV29g.cpp D3PDPeterTreeV30.cpp D3PDPeterTreeV50.cpp NtD3PDphysPeter.cpp 
endif
libTrigFTKAnaNtD3PDPeter_a_CXXFLAGS = $(BOOST_CPPFLAGS) $(DCAP_CPPFLAGS) $(XML_CPPFLAGS) @ROOTCFLAGS@ @ROOTAUXCFLAGS@ $(ROOT_CPPFLAGS) $(FTKSIM_CPPFLAGS) -fno-inline

libTrigFTKAnaNtD3PDJetMET_a_SOURCES = NtD3PDphysJetMET.cpp D3PDJetMETTreeV1.cpp
libTrigFTKAnaNtD3PDJetMET_a_CXXFLAGS = $(BOOST_CPPFLAGS) $(DCAP_CPPFLAGS) $(XML_CPPFLAGS) @ROOTCFLAGS@ @ROOTAUXCFLAGS@ $(ROOT_CPPFLAGS) $(FTKSIM_CPPFLAGS) -fno-inline

libTrigFTKAnaNtMisc_a_SOURCES = NtJetTag.cpp WHTreeV13.cxx WHTreeV14.cxx TagTree.cxx NtPGS.cpp
libTrigFTKAnaNtMisc_a_CXXFLAGS = $(BOOST_CPPFLAGS) $(DCAP_CPPFLAGS) $(XML_CPPFLAGS) @ROOTCFLAGS@ @ROOTAUXCFLAGS@ $(ROOT_CPPFLAGS) $(FTKSIM_CPPFLAGS) -fno-inline

CLEANFILES              = *Cint.cxx *Cint.h *.o lib*.a $(lib_LIBRARIES)
