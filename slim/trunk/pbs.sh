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

echo "====================================================================="
echo ./slim.py -i ${inlist} -o ${tmpdir}/${outfile} --tree tree --var vars.txt  ${extras}
echo xrdcp -s -f -md5 ${tmpdir}/${outfile} ${OUTDIR}/${outfile}
echo "====================================================================="
md5source="SOURCEMD5"
md5dest="DESTMD5"
./slim.py -i ${inlist} -o ${tmpdir}/${outfile} --tree tree --var vars.txt  ${extras} && {
    md5source=`md5sum ${tmpdir}/${outfile} | awk '{print $1}'`
    md5dest=`xrdcp -s -f -md5 ${tmpdir}/${outfile} ${OUTDIR}/${outfile} | awk '{print $2}'`
}
if [ "${md5source}" == "{md5dest}" ]; then
    echo "SUCCESS: ${outdir}/${outfile} ${md5source} "
else
    echo "File sizes:"
    ls -l ${tmpdir}/${outfile}
    ls -l ${outdir}/${outfile}
    echo "ERROR: ${outdir}/${outfile} ${md5source} ${md5dest}"
    rm -f ${outdir}/${outfile}
fi
rm -f ${tmpdir}/${outfile}
