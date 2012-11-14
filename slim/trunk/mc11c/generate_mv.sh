#!/bin/bash

# A script that generates rename commands 

for dname in `ls -1 *.dat`; do
    dsample=`echo ${dname} | sed -e 's#wasymmetry29I_##g' -e 's#.dat##g'`
    fname=`head -n1 $dname`
    fdirfull=`dirname $fname`
    fdirdirty=`basename ${fdirfull}`
    fdir=`echo "${fdirdirty%.*}"`
    echo mv $dsample $fdir
done
