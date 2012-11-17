.PHONY : all
all: test

test: test.cpp PDFReweightTool.hpp
	g++ -I. -I LHAPDF/include/ -L LHAPDF/lib -lLHAPDF test.cpp -o test

testold: test.cpp PDFReweight.cpp
	g++ -I LHAPDF/include/ -c PDFReweight.cpp -o PDFReweight.o
	g++ -I. -I LHAPDF/include/ -L LHAPDF/lib -lLHAPDF PDFReweight.o test.cpp -o test

.PHONY : clean
clean:
	@rm -f test *.o

