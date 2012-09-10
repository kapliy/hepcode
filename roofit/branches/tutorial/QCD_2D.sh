#!/bin/bash
#PBS -q uct3
#PBS -l cput=7:00:00
#PBS -l walltime=7:00:00
#PBS -m e
#qsub -v id=$i -N QCD2D${i} -o /home/antonk/roofit/logs/log.${i}.stdout -e /home/antonk/roofit/logs/log.${i}.stderr QCD_2D.sh

# Submits 2D and 1D QCD systematic histogram-based plots

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
        #id="ALL"
    fi
fi
echo "Processing job id = $id"
source bashmap.sh
SMART_KILLER="./smart_killer.sh"
#SMART_KILLER=""

antondb='HISTO_08272012_MC10'
mkdir -p ${antondb}
input=/share/t3data3/antonk/ana/ana_v29G_07252012_newROOT_stacoCB_all
input=/share/t3data3/antonk/ana/ana_v29G_08242012_allwts_V29I_stacoCB_all  # ALL weights + v29i ntuple + jerup/down
input=/share/t3data3/antonk/ana/ana_v29I_08242012_allwts_wptMC10_stacoCB_all # same, but using MC10 reweighting

bgqcd=3
i=0
irun=0

gput tagis ${i} isofail "--isofail loose_isofail " ; ((i++))
gput tagis ${i} isowind "--isofail isowind " ; ((i++))

gput tagzs ${i} met0to80 "--lvar met --lbin 50,0,80 " ; ((i++))
gput tagzs ${i} wmt40to90 "--lvar wmt --lbin 50,40,90 "  ; ((i++))
gput tagzs ${i} met0to90 "--lvar met --lbin 50,0,90 " ; ((i++))
gput tagzs ${i} wmt35to100 "--lvar wmt --lbin 50,35,100 "  ; ((i++))

for iq in 0 1; do
    for bgsig in 1 4 5; do
	for itagi in `gkeys tagis`; do
	    tagi=`ggeta tagis $itagi`
	    optsi=`ggetb tagis $itagi`
	    for itagz in `gkeys tagzs`; do
		tagz=`ggeta tagzs $itagz`
		optsz=`ggetb tagzs $itagz`
		for bgewk in 5 2; do
		    for ieta in {0..10} ALL; do
			for ipt in {0..6} ALL; do
			    if [ "$id" == "$irun" -o "$id" == "ALL" ]; then
				eval ${SMART_KILLER} ./stack2.py --input ${input} -b --charge $iq ${optsz} ${optsi} -o TEST_Q${iq}/ETA${ieta}/PT${ipt} -t ${tagi}_${tagz} -m qcdfit_2d --bgsig ${bgsig} --bgewk ${bgewk} --bgqcd ${bgqcd} --preNN ${ieta} --preNQ ${ipt} --extra ${antondb}/${antondb}.PART.${iq}.${bgsig}.${bgewk}.${ieta}.${ipt} --xsecerr 0
			    fi
			    ((irun++))
			done
		    done
		done
	    done
	done
    done
done
wait
echo "Maximum irun: $irun"

date
