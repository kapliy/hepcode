#!/bin/bash
#PBS -q uct3
#PBS -l cput=3:00:00
#PBS -l walltime=3:00:00
#PBS -m e
#qsub -v id=$i -N QCD2D${i} -o /home/antonk/roofit/logs/log.${i}.stdout -e /home/antonk/roofit/logs/log.${i}.stderr QCD_2D.sh

# PBS submitter for QCD_3D. For now, only coded with ETAMODE=2 (abseta)

cd /home/antonk/roofit/
source config.sh

mkdir -p /home/antonk/roofit/logs/
if [ -z "$SITEROOT" ]; then
    echo "Sourcing environment..."
    echo $HOSTNAME
    source /home/antonk/.bashrc
    anaquick5
    cd /home/antonk/roofit
    date
    echo "Running id = ${id}"
    #set -x
else
    if [ "$#" -ge "1" ]; then
	id=$1
    else
	id="NONE"
    fi
fi
echo "Processing job id = $id"

nomonly=" --extra ${DB} --exit"
irun=0
for iq in 0 1; do 
    for ipt in {1..7} ALL ALL25; do
	for ieta in {1..11} ALL; do
	    if [ "${ieta}" == "ALL" ]; then
		if [ "${ipt}" != "ALL" -a "${ipt}" != "ALL25" ]; then continue; fi
	    fi
	    if [ "$id" == "$irun" -o "$id" == "ALL" ]; then
		./stack2.py ${nomonly} --input ${input} -b --charge ${iq} --lvar d3_abseta_lpt_met --lbin 50,0,40 --isofail IsoWind20 -o ${out} -t Q${iq} -m qcdfit_sys --bgsig ${bgsig} --bgewk 5 --bgtau 2 --bgqcd 4  --ipt ${ipt} --var d3_abseta_lpt_met --xsecerr 0 --etamode 2 --rebin 4 --ieta ${ieta}
	    fi
	    ((irun++))
	done
    done
done
wait
echo "Maximum irun: $irun"
date
