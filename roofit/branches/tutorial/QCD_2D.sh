#!/bin/bash
#PBS -q uct3
#PBS -l cput=3:00:00
#PBS -l walltime=3:00:00
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

antondb='HISTO_11282012_ALL.v3'
mkdir -p ${antondb}
input=/share/t3data3/antonk/ana/ana_v29I_11072012_unfold_stacoCB_all/
input=/share/t3data3/antonk/ana/ana_v29I_11282012_edboard_nophi_stacoCB_all/

ETAMODES="1 2" # 2=|eta| bins, 1=eta bins
bgsigs="1 4 5"
bgqcds="4 3" # 3=regular, 4=bgsub in qcd template
bgtaus="5 2 0"
ipts=`echo {1..5} ALL`

i=0
irun=0

gput tagis ${i} isofail "--isofail IsoFail20 " ; ((i++))
gput tagis ${i} isowind "--isofail IsoWind20 " ; ((i++))

if [ "0" == "1" ]; then # old (now disabled) binning. in particular, this utilizes the badly modeled high-tail
    gput tagzs ${i} met0to80 "--lvar met --lbin 50,0,80 " ; ((i++))
    gput tagzs ${i} met0to90 "--lvar met --lbin 50,0,90 " ; ((i++))
    gput tagzs ${i} wmt40to90 "--lvar wmt --lbin 50,40,90 "  ; ((i++))
    gput tagzs ${i} wmt35to100 "--lvar wmt --lbin 50,35,100 "  ; ((i++))
    gput tagzs ${i} met10to50 "--lvar met --lbin 50,10,50 " ; ((i++))
    gput tagzs ${i} wmt40to70 "--lvar wmt --lbin 50,40,70 "  ; ((i++))
    gput tagzs ${i} wmt35to80 "--lvar wmt --lbin 50,35,80 "  ; ((i++))
    gput tagzs ${i} met0to60 "--lvar met --lbin 50,0,60 " ; ((i++))
    gput tagzs ${i} wmt35to70 "--lvar wmt --lbin 50,5,70 "  ; ((i++))
fi

gput tagzs ${i} met0to50 "--lvar d3_abseta_lpt_met --lbin 50,0,50 " ; ((i++))
gput tagzs ${i} met5to60 "--lvar d3_abseta_lpt_met --lbin 50,5,60 " ; ((i++))
gput tagzs ${i} wmt40to80 "--lvar d3_abseta_lpt_wmt --lbin 50,40,80 "  ; ((i++))

for ETAMODE in ${ETAMODES}; do
    ietas=`echo {1..11} ALL`
    var='d3_abseta_lpt_met'  # plotting var (not relevant for scale definition; only for fraction)
    if [ "${ETAMODE}" == "1" ]; then
	ietas=`echo {1..22} ALL`
	var='d3_eta_lpt_met'
    fi
    for iq in 0 1; do
	for bgqcd in $bgqcds; do
	    for bgsig in $bgsigs; do
		for itagi in `gkeys tagis`; do
		    tagi=`ggeta tagis $itagi`
		    optsi=`ggetb tagis $itagi`
		    for itagz in `gkeys tagzs`; do
			tagz=`ggeta tagzs $itagz`
			optsz=`ggetb tagzs $itagz`
			if [ "${ETAMODE}" == "1" ]; then
			    optsz=`echo ${optsz} | sed -e 's#abseta#eta#g'`
			fi
			for bgtau in $bgtaus; do
			    for ieta in ${ietas}; do
				for ipt in ${ipts}; do
				    if [ "$id" == "$irun" -o "$id" == "ALL" ]; then
					eval ${SMART_KILLER} ./stack2.py --input ${input} -b --charge $iq ${optsz} ${optsi} -o TEST_EM${ETAMODE}_QCD${bgqcd}_Q${iq}/ETA${ieta}/PT${ipt} -t ${tagi}_${tagz} -m qcdfit_2d --bgsig ${bgsig} --bgewk ${bgsig} --bgtau ${bgtau} --bgqcd ${bgqcd} --ieta ${ieta} --ipt ${ipt} --extra ${antondb}/${antondb}.PART.${iq}.${bgsig}.${bgtau}.${ieta}.${ipt} --var ${var} --xsecerr 0 --etamode ${ETAMODE} --rebin 4
					if [ ! "$id" == "ALL" ]; then
					    exit 0
					fi
				    fi
				    ((irun++))
				done
			    done
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
