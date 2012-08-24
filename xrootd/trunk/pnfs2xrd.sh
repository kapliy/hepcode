#!/bin/bash

# replicate datasets from LOCALGROUPDISK pnfs to xrootd

NTHREADS=40
HOST1=root://xrddc.mwt2.org:1096
HOST2=root://uct3-xrd.mwt2.org/

function copy_pnfs_to_xrd() {
    origpath=$1
    DESTDIR=$2
    origfile=`basename $origpath`
    origdir=`dirname $origpath`
    origsubdir=`basename $origdir`
    mkdir ${DESTDIR}/${origsubdir} 2>/dev/null && sleep 1
    
    origfilexrd=${HOST1}${origpath}
    destfilexrd=${HOST2}/${DESTDIR}/${origsubdir}/${origfile}
    
    echo xrdcp -s -f ${origfilexrd} ${destfilexrd}
    xrdcp -s -f ${origfilexrd} ${destfilexrd}
    echo "copied: ${destfilexrd} $?"
}

# require uct3-s1 or uct3-s3
if [ "${HOSTNAME}" != "uct3-s1.uchicago.edu" -a "${HOSTNAME}" != "uct3-s3.uchicago.edu" ]; then
    echo "ERROR: must run on uct3-s1.uchicago.edu"
    exit 2
fi

# require exactly two arguments
if [ "$#" != "2" ]; then
    # if the script is sourced, we want to "return"
    return 0 &> /dev/null
    # if the script is invoked, we want to "exit"
    exit 0
fi

DESTDIR=$1
CACHE=$2

mkdir -p ${DESTDIR} && sleep 1

(for origpath in `grep mc11 cache.dat`; do echo $origpath; done) | xargs -i -n1 -P${NTHREADS} bash -c "source $0; copy_pnfs_to_xrd {} ${DESTDIR}"

#(for origpath in `grep user.kapliy.UCNTUP.mc11_7TeV.113712.McAtNloDrellYan15_60_2mu3.merge.AOD.e1131_s1372_s1370_r3043_r2993.j.mc11c.v1_29g cache.dat`; do echo $origpath; done) | xargs -i -n1 -P${NTHREADS} bash -c "source $0; copy_pnfs_to_xrd {} ${DESTDIR}"
#(for origpath in `grep user.kapliy.UCNTUP.mc11_7TeV.106088.McAtNloZmumu_no_filter.merge.AOD.e1096_s1372_s1370_r3043_r2993.j.mc11c.v1_29g cache.dat`; do echo $origpath; done) | xargs -i -n1 -P${NTHREADS} bash -c "source $0; copy_pnfs_to_xrd {} ${DESTDIR}"


