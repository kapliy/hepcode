#!/bin/bash

# A script to compile LHAPDF in TrigFTKAna !!!
LHALOC=$PWD/LHAPDF

# On 64-bit systems, LHANMAX cannot be more than 6. Otherwise, this error shows up:
# wrapgjr.f:(.text+0x425): relocation truncated to fit: R_X86_64_PC32 against `.bss'
export LHANMAX=6
LOWMEM="--enable-low-memory"
PDFSETS=""

# Jan 15 2013: 5 sets + all of CT10 variations
export LHANMAX=5
LOWMEM=""
PDFSETS=""

# Mar 14 2013: all of CT10 variations
export LHANMAX=1
LOWMEM=""
PDFSETS="--enable-pdfsets=cteq"

./configure --prefix=${LHALOC} --disable-octave --disable-doxygen --disable-pyext --disable-old-ccwrap --disable-lhaglue --with-max-num-pdfsets=${LHANMAX} ${PDFSETS} ${LOWMEM}
ret=?

if [ "${ret}" -ne "0" ]; then
    echo "ERROR: configure script failed"
    exit 3
fi

rm -rf ${LHALOC}
make clean && make -j20 && make install && make clean

ret=$?
if [ "${ret}" -ne "0" ]; then
    echo "ERROR: make failed"
    exit 4
fi
