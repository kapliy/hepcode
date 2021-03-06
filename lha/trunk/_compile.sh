#!/bin/bash

####################################################
##################### SETTINGS #####################
####################################################
# Maximum number of *different* PDF families supported in a single linux process.
# On 64-bit systems, LHANMAX cannot be more than 6. Otherwise, this error shows up:
# wrapgjr.f:(.text+0x425): relocation truncated to fit: R_X86_64_PC32 against `.bss'
# Enable only a subset of PDFs to conserve memory
#PDFSETS="--enable-pdfsets=mrst,mrst06,mrst98,mrstqed,cteq,nnpdf,mstw,hera,alekhin"
#PDFSETS="--enable-pdfsets=cteq,nnpdf,mstw,hera"
# Without low-memory option, LHAPDF needs ~3GB of RAM to run
# With low-memory, it can run with 300MB, but cannot process error variations
export LHANMAX=6
LOWMEM="--enable-low-memory"

# CT10 ONLY
export LHANMAX=5
LOWMEM=""
PDFSETS=""

####################################################
##################### INSTALL ######################
####################################################

if [ -z "${mbits}" ]; then
    echo "ERROR: please run this script via compile32.sh or compile64.sh"
fi


ARCHIVE=lhapdf-5.8.8.tar.gz
LHAINST=`echo $ARCHIVE | sed -e 's#.tar.gz##g'`
LHALOC=$PWD/LHAPDF   # final installation location

rm -rf ${LHAINST}
tar xfz ${ARCHIVE}

if [ ! -d ${LHAINST} ]; then
    echo "ERROR: unable to find extracted lhapdf code under $PWD/${LHAINST}"
    exit 1
fi

export CPPFLAGS="$CPPFLAGS -m${mbits}"
export CXXFLAGS="$CXXFLAGS -m${mbits}"
export CFLAGS="$CFLAGS -m${mbits}"
export LDFLAGS="$LDFLAGS -m${mbits}"
export F77="gfortran"
export F90="gfortran"
export FFLAGS="$FFLAGS -m${mbits}"
export FCFLAGS="$FCFLAGS -m${mbits}"

cd ${LHAINST}
./configure --prefix=${LHALOC} --disable-octave --disable-doxygen --disable-pyext --disable-old-ccwrap --disable-lhaglue --with-max-num-pdfsets=${LHANMAX} ${PDFSETS} ${LOWMEM}
ret="$?"

if [ ! "${ret}" == "0" ]; then
    echo "ERROR: configure script failed with error code: [${ret}]"
    cd ../
    exit 3
fi

rm -rf ${LHALOC}
make clean && make -j20 && make install
ret=$?

cd ../
if [ "${ret}" -ne "0" ]; then
    echo "ERROR: make failed"
    exit 4
fi
