
# precompile headers

BUILT_SOURCES := $(patsubst %,%.gch,$(wildcard *.h *.hpp *.hh))

CXXCOMPILE = $(CXX) $(CXXFLAGS) $(BOOST_CPPFLAGS) $(DCAP_CPPFLAGS) @ROOTCFLAGS@ @ROOTAUXCFLAGS@ $(ROOT_CPPFLAGS) $(FTKSIM_CPPFLAGS) -I../

%.hh.gch: %.hh 
	$(CXXCOMPILE) -x c++-header -c $< 

%.hpp.gch: %.hpp
	$(CXXCOMPILE) -x c++-header -c $< 

%.h.gch: %.h
	$(CXXCOMPILE) -x c++-header -c $< 

CLEANFILES = *.gch
