#!/bin/bash

echo "Setting up X509 proxy"
export X509_USER_PROXY=/home/antonk/.globus/tmp.proxy
(source /share/wlcg-client/setup.sh && voms-proxy-init -pwstdin -voms atlas -valid 999:0 -out ${X509_USER_PROXY} < /home/antonk/setup/info 2>&1)

tmin=0
tmax=1000

if [ "$#" == "2" ]; then
    tmin=$1
    tmax=$2
fi
echo Processing: $tmin to $tmax

anadir=/home/antonk/d3pd/EVGEN
outdir=${anadir}/output
rm -rf ${outdir}
mkdir -p ${outdir}

for dsample in D3PD_HERWIG FLAT_HERWIG FLAT_PYTHIA; do
    id=0
    for f in `cat ${dsample} 2>/dev/null`; do
	echo "Submitting ${dsample} $id"
	mkdir -p $outdir/$dsample
	mkdir -p $outdir/$dsample/logs
	qsub -v anadir=${anadir},outdir=${outdir}/$dsample,fin=${f},fout="${outdir}/${dsample}/output${id}.root" -N ${mc11}${id} -o $outdir/$dsample/logs/log.stdout -e $outdir/$dsample/logs/log.stderr pbs.sh
	((id++))
    done
done
