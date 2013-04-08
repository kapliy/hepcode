
# precompile headers
#  exclude fortran headers.
FORTRAN_CTVMFT = ctvmco.h ctvmdi.h ctvmfi.h ctvmft.h ctvmpr.h ctvmtr.h ctvmuv.h
BUILT_SOURCES := $(patsubst %,%.gch, $(filter-out $(FORTRAN_CTVMFT) Ctvm*,$(wildcard *.h *.hpp *.hh)))

CXXCOMPILE = $(CXX) $(CXXFLAGS) $(BOOST_CPPFLAGS) $(DCAP_CPPFLAGS) $(XML_CPPFLAGS) @ROOTCFLAGS@ @ROOTAUXCFLAGS@ -lTMVA $(ROOT_CPPFLAGS) $(FTKSIM_CPPFLAGS) -I../

%.hh.gch: %.hh 
	$(CXXCOMPILE) -x c++-header -c $< 

%.hpp.gch: %.hpp
	$(CXXCOMPILE) -x c++-header -c $< 

%.h.gch: %.h
	$(CXXCOMPILE) -x c++-header -c $< 

CLEANFILES = *.gch
