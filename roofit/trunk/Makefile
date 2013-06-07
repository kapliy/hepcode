.PHONY : all
all: fit

VFitterDict.C: VFitter.h VFitterLinkDef.h
	rootcint -f $@ -c $^

fit: fit.o VFitter.o VFitterDict.o
	g++ -g $(shell root-config --libs) $^ -o $@

fit.o: fit.cpp
	g++ -fPIC -c -o $@ $< -g $(shell root-config --cflags)

VFitter.o: VFitter.cpp
	g++ -fPIC -c -o $@ $< -g $(shell root-config --cflags)

VFitterDict.o: VFitterDict.C
	g++ -fPIC -c -o $@ $< -g $(shell root-config --cflags)

.PHONY : clean
clean:
	@rm -f fit *.o VFitterDict.{h,C}
