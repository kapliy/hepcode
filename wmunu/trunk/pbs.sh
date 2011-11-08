#!/bin/sh
#PBS -q uct3
#PBS -l cput=28:00:00
#PBS -l walltime=28:00:00
#PBS -m e

echo HOSTNAME=$HOSTNAME
date
source /home/antonk/.bashrc
anaquick
echo "${anadir}"
echo "${outdir}"
mkdir -p ${outdir}
cd ${outdir}/
date
time python ${anadir}/looper.py -o ${outdir} ${extras}
date
