.PHONY : all
all: test32 test64

test32: test.cpp PDFReweightTool.hpp
	g++ -I. -I LHAPDF/include/ -L LHAPDF/lib -lLHAPDF test.cpp -m32 -o test32

test64: test.cpp PDFReweightTool.hpp
	g++ -I. -I LHAPDF/include/ -L LHAPDF/lib -lLHAPDF test.cpp -m64 -o test64

.PHONY : clean
clean:
	@rm -f test test32 test64 *.o PDFReweightTool_hpp.so
