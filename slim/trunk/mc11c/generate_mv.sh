#!/bin/bash

# A script that generates rename commands 

for dname in `ls -1 *.dat`; do
    dsample=`echo ${dname} | sed -e 's#wasymmetry29I_##g' -e 's#.dat##g'`
    fname=`head -n1 $dname`
    fdirfull=`dirname $fname`
    fdirdirty=`basename ${fdirfull}`
    fdir=`echo "${fdirdirty%.*}"`
    zdir=`echo $fdir | sed -e 's#v1_29i#v1_29l#g' -e 's#v1_29g#v1_29l#g'`
    echo mv $dsample $zdir
done
