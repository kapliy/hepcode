#!/bin/sh
#PBS -q uct3
#PBS -l cput=7:00:00
#PBS -l walltime=7:00:00
#PBS -m e

echo HOSTNAME=$HOSTNAME
date
source /home/antonk/.bashrc
anaquick5

date
echo "anadir=${anadir}"
echo "outdir=${outdir}"
echo "outfile=${outfile}"
echo "inlist=${inlist}"
echo "extras=${extras}"

OUTHOST=root://uct3-xrd.mwt2.org
OUTDIR="${OUTHOST}/${outdir}"

cd ${anadir}
tmpdir=/tmp/$USER/
mkdir -p ${tmpdir}
rm -f ${tmpdir}/${outfile}
./slim.py -i ${inlist} -o ${tmpdir}/${outfile} --tree tree --var vars.txt  ${extras} && xrdcp -s -f -md5 ${tmpdir}/${outfile} ${OUTDIR}/${outfile}
rm -f ${tmpdir}/${outfile}
echo "ALL DONE: $?"
