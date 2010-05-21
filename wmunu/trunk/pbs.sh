#!/bin/sh
#PBS -q uct3
#PBS -l cput=8:00:00
#PBS -l walltime=8:00:00
#PBS -m e

date
cd /home/antonk
source setup/setup.sh
echo $anadir
echo $outdir
echo $rnum
mkdir -p ${outdir}
cd ${outdir}/
date
time python ${anadir}/looper.py -g ${rnum}
date
