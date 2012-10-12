#!/bin/sh

#PBS -q uct3
#PBS -l cput=7:00:00
#PBS -l walltime=7:00:00
#PBS -m e

echo HOSTNAME=$HOSTNAME
date
source /home/antonk/.bashrc
anaquick5
echo "${anadir}"
echo "${outdir}"
mkdir -p ${outdir}
cd ${anadir}/
date
pwd
set +x
${anadir}/TruthReweight.py ${fin} ${fout}
set -x
date
