localmflag=m32
ifeq (${uct3_64},1)
	localmflag=m64
endif
ifeq (${macosx_64},1)
	localmflag=m64
endif

.PHONY: all

all: ../libfrtbegin.a

frtbegin.o: main.c
	gcc -$(localmflag) -c -I. -DSTDC_HEADERS=1 -DRETSIGTYPE=void -DIEEE_drem=1 -DSkip_f2c_Undefs=1 -DIEEE_COMPLEX_DIVIDE=1 -g -O2 ./main.c -o frtbegin.o

../libfrtbegin.a : frtbegin.o
	rm -f ../libfrtbegin.a
	ar rc ../libfrtbegin.a frtbegin.o
	ranlib ../libfrtbegin.a

clean:
	rm -f ../libfrtbegin.a frtbegin.o
