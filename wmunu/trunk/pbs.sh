#!/bin/sh
#PBS -q uct3
#PBS -l cput=8:00:00
#PBS -l walltime=8:00:00
#PBS -m e

date
source /home/antonk/work/current/cmtsite/setup.sh -tag=15.6.9.13
echo "${anadir}"
echo "${outdir}"
echo "${inpfiles}"
mkdir -p ${outdir}
cd ${outdir}/
date
time python ${anadir}/looper.py -i "${inpfiles}" -x "${anadir}/xml/*.xml" -o ${outdir} ${extras}
date
