
ACLOCAL_AMFLAGS = -I$(top_srcdir)/m4

# if "./configure --allana", then make all executables
if MAKE_ALLANA
bin_PROGRAMS = ana_wbjets ana_wzjets ana_top ana_wasym ana_standard_overview ana_b_tagging ana_pgs ana_vtx ana_hww ana_ftkeffchain ana_monojet
else
bin_PROGRAMS = ana_wasym # ana_monojet ana_standard_overview ana_wzjets ana_monojet
endif

COMMONLIBS = ../src/libTrigFTKAna.a ../src/Trees/libTrigFTKAnaNtFTK.a ../src/Trees/libTrigFTKAnaNtD3PDPeter.a ../src/Trees/libTrigFTKAnaNtD3PDJetMET.a ../src/Trees/libTrigFTKAnaNtMisc.a ../src/libTrigFTKAnaBase.a ../src/misc/libTrigFTKAnaMisc.a ../src/egamma/libTrigFTKAnaBaseEgamma.a ../src/ctvmft/libTrigFTKAnaCTVMFT.a ../dg/libdatagraphics.a

ana_pgs_LDADD = -L@ROOTLIBDIR@ @ROOTLIBS@ $(COMMONLIBS) $(FLIBS)
ana_pgs_SOURCES = ana_pgs.cpp
ana_pgs_CXXFLAGS = $(BOOST_CPPFLAGS) $(DCAP_CPPFLAGS) -I@ROOTINCDIR@ -I$(pgs_srcdir) @ROOTCFLAGS@ @ROOTAUXCFLAGS@ $(FTKSIM_CPPFLAGS) $(COMMONANALYSIS_CPPFLAGS)
ana_pgs_LDFLAGS = $(BOOST_LDFLAGS) $(BOOST_PROGRAM_OPTIONS_LIB) $(BOOST_REGEX_LIB) $(BOOST_IOSTREAMS_LIB) $(BOOST_SYSTEM_LIB) $(BOOST_FILESYSTEM_LIB) $(BOOST_DATE_TIME_LIB) $(BOOST_SERIALIZATION_LIB) $(DCAP_LDFLAGS) $(XML_LDFLAGS) $(XML_LDFLAGS) $(ROOT_LDFLAGS) $(FTKSIM_LDFLAGS) $(FLIBS) $(COMMONANALYSIS_LDFLAGS)

ana_standard_overview_LDADD = -L@ROOTLIBDIR@ @ROOTLIBS@ $(COMMONLIBS) $(ROOFITLIBS)
ana_standard_overview_SOURCES = ana_standard_overview.cpp
ana_standard_overview_CXXFLAGS = $(BOOST_CPPFLAGS) $(DCAP_CPPFLAGS) -I@ROOTINCDIR@ -I$(top_srcdir) @ROOTCFLAGS@ @ROOTAUXCFLAGS@ $(FTKSIM_CPPFLAGS) $(COMMONANALYSIS_CPPFLAGS)
ana_standard_overview_LDFLAGS = @BOOST_LDFLAGS@ $(FLIBS) $(BOOST_PROGRAM_OPTIONS_LIB) $(BOOST_REGEX_LIB) $(BOOST_IOSTREAMS_LIB) $(BOOST_SYSTEM_LIB) $(BOOST_FILESYSTEM_LIB) $(BOOST_DATE_TIME_LIB) $(BOOST_SERIALIZATION_LIB) $(DCAP_LDFLAGS) $(XML_LDFLAGS) $(ROOT_LDFLAGS) $(CTVMFT_LDFLAGS) $(FTKSIM_LDFLAGS) $(COMMONANALYSIS_LDFLAGS)

ana_ftkeffchain_LDADD = -L@ROOTLIBDIR@ @ROOTLIBS@ $(COMMONLIBS) $(ROOFITLIBS) $(FTKSIM_LIBS)
ana_ftkeffchain_SOURCES = ana_ftkeffchain.cpp
ana_ftkeffchain_CXXFLAGS = $(BOOST_CPPFLAGS) $(DCAP_CPPFLAGS) -I@ROOTINCDIR@ -I$(top_srcdir) @ROOTCFLAGS@ @ROOTAUXCFLAGS@ $(FTKSIM_CPPFLAGS) $(COMMONANALYSIS_CPPFLAGS)
ana_ftkeffchain_LDFLAGS = @BOOST_LDFLAGS@ $(FLIBS) $(BOOST_PROGRAM_OPTIONS_LIB) $(BOOST_REGEX_LIB) $(BOOST_IOSTREAMS_LIB) $(BOOST_SYSTEM_LIB) $(BOOST_FILESYSTEM_LIB) $(BOOST_DATE_TIME_LIB) $(BOOST_SERIALIZATION_LIB) $(DCAP_LDFLAGS) $(XML_LDFLAGS) $(ROOT_LDFLAGS) $(CTVMFT_LDFLAGS) $(FTKSIM_LDFLAGS) $(COMMONANALYSIS_LDFLAGS)

ana_b_tagging_LDADD = -L@ROOTLIBDIR@ @ROOTLIBS@ $(COMMONLIBS) $(ROOFITLIBS) $(FLIBS)
ana_b_tagging_SOURCES = ana_b_tagging.cpp
ana_b_tagging_CXXFLAGS = $(BOOST_CPPFLAGS) $(DCAP_CPPFLAGS) -I@ROOTINCDIR@ -I$(top_srcdir) @ROOTCFLAGS@ @ROOTAUXCFLAGS@ $(FTKSIM_CPPFLAGS) $(COMMONANALYSIS_CPPFLAGS)
ana_b_tagging_LDFLAGS = $(BOOST_LDFLAGS) $(BOOST_PROGRAM_OPTIONS_LIB) $(BOOST_REGEX_LIB) $(BOOST_IOSTREAMS_LIB) $(BOOST_SYSTEM_LIB) $(BOOST_FILESYSTEM_LIB) $(BOOST_DATE_TIME_LIB) $(BOOST_SERIALIZATION_LIB) $(DCAP_LDFLAGS) $(XML_LDFLAGS) $(ROOT_LDFLAGS) $(FTKSIM_LDFLAGS) $(FLIBS) $(COMMONANALYSIS_LDFLAGS)

ana_vtx_LDADD = -L@ROOTLIBDIR@ @ROOTLIBS@ $(COMMONLIBS) $(ROOFITLIBS) $(FLIBS)
ana_vtx_SOURCES = ana_vtx.cpp
ana_vtx_CXXFLAGS = $(BOOST_CPPFLAGS) $(DCAP_CPPFLAGS) -I@ROOTINCDIR@ -I$(top_srcdir) @ROOTCFLAGS@ @ROOTAUXCFLAGS@ $(FTKSIM_CPPFLAGS) $(COMMONANALYSIS_CPPFLAGS)
ana_vtx_LDFLAGS = $(BOOST_LDFLAGS) $(BOOST_PROGRAM_OPTIONS_LIB) $(BOOST_REGEX_LIB) $(BOOST_IOSTREAMS_LIB) $(BOOST_SYSTEM_LIB) $(BOOST_FILESYSTEM_LIB) $(BOOST_DATE_TIME_LIB) $(BOOST_SERIALIZATION_LIB) $(DCAP_LDFLAGS) $(XML_LDFLAGS) $(ROOT_LDFLAGS) $(FTKSIM_LDFLAGS) $(FLIBS) $(COMMONANALYSIS_LDFLAGS)

ana_wbjets_LDADD = -L@ROOTLIBDIR@ @ROOTLIBS@ $(COMMONLIBS) $(ROOFITLIBS) $(FLIBS)
ana_wbjets_SOURCES = ana_wbjets.cpp
ana_wbjets_CXXFLAGS = $(BOOST_CPPFLAGS) $(DCAP_CPPFLAGS) -I@ROOTINCDIR@ -I$(top_srcdir) @ROOTCFLAGS@ @ROOTAUXCFLAGS@ $(FTKSIM_CPPFLAGS) $(COMMONANALYSIS_CPPFLAGS)
ana_wbjets_LDFLAGS = $(BOOST_LDFLAGS) $(BOOST_PROGRAM_OPTIONS_LIB) $(BOOST_REGEX_LIB) $(BOOST_IOSTREAMS_LIB) $(BOOST_SYSTEM_LIB) $(BOOST_FILESYSTEM_LIB) $(BOOST_DATE_TIME_LIB) $(BOOST_SERIALIZATION_LIB) $(DCAP_LDFLAGS) $(XML_LDFLAGS) $(XML_LDFLAGS) $(ROOT_LDFLAGS) $(FTKSIM_LDFLAGS) $(FLIBS) $(COMMONANALYSIS_LDFLAGS)

ana_wzjets_LDADD = -L@ROOTLIBDIR@ @ROOTLIBS@ $(COMMONLIBS) $(ROOFITLIBS) $(FLIBS) -lTMVA
ana_wzjets_SOURCES = ana_wzjets.cpp
ana_wzjets_CXXFLAGS = $(BOOST_CPPFLAGS) $(DCAP_CPPFLAGS) -I@ROOTINCDIR@ -I$(top_srcdir) @ROOTCFLAGS@ @ROOTAUXCFLAGS@ $(FTKSIM_CPPFLAGS) $(COMMONANALYSIS_CPPFLAGS)
ana_wzjets_LDFLAGS = $(BOOST_LDFLAGS) $(BOOST_PROGRAM_OPTIONS_LIB) $(BOOST_REGEX_LIB) $(BOOST_IOSTREAMS_LIB) $(BOOST_SYSTEM_LIB) $(BOOST_FILESYSTEM_LIB) $(BOOST_DATE_TIME_LIB) $(BOOST_SERIALIZATION_LIB) $(DCAP_LDFLAGS) $(XML_LDFLAGS) $(XML_LDFLAGS) $(ROOT_LDFLAGS) $(FTKSIM_LDFLAGS) $(FLIBS) $(COMMONANALYSIS_LDFLAGS)

ana_monojet_LDADD = -L@ROOTLIBDIR@ @ROOTLIBS@ $(COMMONLIBS) $(ROOFITLIBS) $(FLIBS)
ana_monojet_SOURCES = ana_monojet.cpp
ana_monojet_CXXFLAGS = $(BOOST_CPPFLAGS) $(DCAP_CPPFLAGS) -I@ROOTINCDIR@ -I$(top_srcdir) @ROOTCFLAGS@ @ROOTAUXCFLAGS@ $(FTKSIM_CPPFLAGS) $(COMMONANALYSIS_CPPFLAGS)
ana_monojet_LDFLAGS = $(BOOST_LDFLAGS) $(BOOST_PROGRAM_OPTIONS_LIB) $(BOOST_REGEX_LIB) $(BOOST_IOSTREAMS_LIB) $(BOOST_SYSTEM_LIB) $(BOOST_FILESYSTEM_LIB) $(BOOST_DATE_TIME_LIB) $(BOOST_SERIALIZATION_LIB) $(DCAP_LDFLAGS) $(XML_LDFLAGS) $(XML_LDFLAGS) $(ROOT_LDFLAGS) $(FTKSIM_LDFLAGS) $(FLIBS) $(COMMONANALYSIS_LDFLAGS)

ana_top_LDADD = -L@ROOTLIBDIR@ @ROOTLIBS@ $(COMMONLIBS) $(ROOFITLIBS) $(FLIBS)
ana_top_SOURCES = ana_top.cpp
ana_top_CXXFLAGS = $(BOOST_CPPFLAGS) $(DCAP_CPPFLAGS) -I@ROOTINCDIR@ -I$(top_srcdir) @ROOTCFLAGS@ @ROOTAUXCFLAGS@ $(FTKSIM_CPPFLAGS) $(COMMONANALYSIS_CPPFLAGS)
ana_top_LDFLAGS = $(BOOST_LDFLAGS) $(BOOST_PROGRAM_OPTIONS_LIB) $(BOOST_REGEX_LIB) $(BOOST_IOSTREAMS_LIB) $(BOOST_SYSTEM_LIB) $(BOOST_FILESYSTEM_LIB) $(BOOST_DATE_TIME_LIB) $(BOOST_SERIALIZATION_LIB) $(DCAP_LDFLAGS) $(XML_LDFLAGS) $(XML_LDFLAGS) $(ROOT_LDFLAGS) $(FTKSIM_LDFLAGS) $(FLIBS) $(COMMONANALYSIS_LDFLAGS)

ana_wasym_LDADD = -L@ROOTLIBDIR@ @ROOTLIBS@ $(COMMONLIBS) $(ROOFITLIBS) $(FLIBS) -L../lhapdf-5.8.8/LHAPDF/lib -lLHAPDF
ana_wasym_SOURCES = ana_wasym.cpp
ana_wasym_CXXFLAGS = $(BOOST_CPPFLAGS) $(DCAP_CPPFLAGS) -I@ROOTINCDIR@ -I$(top_srcdir) @ROOTCFLAGS@ @ROOTAUXCFLAGS@ $(FTKSIM_CPPFLAGS) $(COMMONANALYSIS_CPPFLAGS) -I../lhapdf-5.8.8/LHAPDF/include
ana_wasym_LDFLAGS = $(BOOST_LDFLAGS) $(BOOST_PROGRAM_OPTIONS_LIB) $(BOOST_REGEX_LIB) $(BOOST_IOSTREAMS_LIB) $(BOOST_SYSTEM_LIB) $(BOOST_FILESYSTEM_LIB) $(BOOST_DATE_TIME_LIB) $(BOOST_SERIALIZATION_LIB) $(DCAP_LDFLAGS) $(XML_LDFLAGS) $(XML_LDFLAGS) $(ROOT_LDFLAGS) $(FTKSIM_LDFLAGS) $(FLIBS) $(COMMONANALYSIS_LDFLAGS)

ana_hww_LDADD = -L@ROOTLIBDIR@ @ROOTLIBS@ $(COMMONLIBS) $(ROOFITLIBS) $(FLIBS)
ana_hww_SOURCES = ana_hww.cpp
ana_hww_CXXFLAGS = $(BOOST_CPPFLAGS) $(DCAP_CPPFLAGS) -I@ROOTINCDIR@ -I$(top_srcdir) @ROOTCFLAGS@ @ROOTAUXCFLAGS@ $(FTKSIM_CPPFLAGS) $(COMMONANALYSIS_CPPFLAGS)
ana_hww_LDFLAGS = $(BOOST_LDFLAGS) $(BOOST_PROGRAM_OPTIONS_LIB) $(BOOST_REGEX_LIB) $(BOOST_IOSTREAMS_LIB) $(BOOST_SYSTEM_LIB) $(BOOST_FILESYSTEM_LIB) $(BOOST_DATE_TIME_LIB) $(BOOST_SERIALIZATION_LIB) $(DCAP_LDFLAGS) $(XML_LDFLAGS) $(XML_LDFLAGS) $(ROOT_LDFLAGS) $(FTKSIM_LDFLAGS) $(FLIBS) $(COMMONANALYSIS_LDFLAGS)

ana_lowmh_LDADD = -L@ROOTLIBDIR@ @ROOTLIBS@ $(COMMONLIBS) $(ROOFITLIBS) $(FLIBS)
ana_lowmh_SOURCES = ana_lowmh.cpp
ana_lowmh_CXXFLAGS = $(BOOST_CPPFLAGS) $(DCAP_CPPFLAGS) -I@ROOTINCDIR@ -I$(top_srcdir) @ROOTCFLAGS@ @ROOTAUXCFLAGS@ $(FTKSIM_CPPFLAGS) $(COMMONHIGGS_CPPFLAGS) $(COMMONANALYSIS_CPPFLAGS)
ana_lowmh_LDFLAGS = $(BOOST_LDFLAGS) $(BOOST_PROGRAM_OPTIONS_LIB) $(BOOST_REGEX_LIB) $(BOOST_IOSTREAMS_LIB) $(BOOST_SYSTEM_LIB) $(BOOST_FILESYSTEM_LIB) $(BOOST_DATE_TIME_LIB) $(BOOST_SERIALIZATION_LIB) $(DCAP_LDFLAGS) $(XML_LDFLAGS) $(XML_LDFLAGS) $(ROOT_LDFLAGS) $(FTKSIM_LDFLAGS) $(FLIBS) $(COMMONHIGGS_LDFLAGS) $(COMMONANALYSIS_LDFLAGS)

CLEANFILES = *.o $(bin_PROGRAMS)
