.PHONY : all
all: test32 test64

test32: test.cpp PDFReweightTool.hpp
	g++ -I. -I LHAPDF/include/ -L LHAPDF/lib -lLHAPDF test.cpp -m32 -o test32
	@echo "32-bit compilation SUCCEEDED"

test64: test.cpp PDFReweightTool.hpp
	g++ -I. -I LHAPDF/include/ -L LHAPDF/lib -lLHAPDF test.cpp -m64 -o test64
	@echo "64-bit compilation SUCCEEDED"

.PHONY : clean
clean:
	@rm -f test test32 test64 *.o PDFReweightTool_hpp.so PDFReweightTool_hpp.d
