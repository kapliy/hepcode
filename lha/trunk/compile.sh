#!/bin/bash

# On 64-bit systems, LHANMAX cannot be more than 6. Otherwise, this error shows up:
# wrapgjr.f:(.text+0x425): relocation truncated to fit: R_X86_64_PC32 against `.bss'
export LHANMAX=4
LHALOC=$PWD/../LHAPDF

PDFSETS="--enable-pdfsets=mrst,mrst06,mrst98,mrstqed,cteq,nnpdf,mstw,hera,alekhin"
PDFSETS="--enable-pdfsets=cteq,nnpdf,mstw,hera"
LOWMEM="--enable-low-memory"

./configure --prefix=${LHALOC} --disable-octave --disable-doxygen --disable-pyext --disable-old-ccwrap --disable-lhaglue --with-max-num-pdfsets=${LHANMAX} ${PDFSETS} ${LOWMEM}
ret=?

if [ "${ret}" -ne "0" ]; then
    echo "ERROR: configure script failed"
    exit 3
fi

rm -rf ${LHALOC}
make clean && make -j20 && make install

ret=$?
if [ "${ret}" -ne "0" ]; then
    echo "ERROR: make failed"
    exit 4
fi