#!/bin/bash
#PBS -q uct3
#PBS -l cput=7:00:00
#PBS -l walltime=7:00:00
#PBS -m e
#qsub -v id=$i -N QCD${i} -o /home/antonk/roofit/logs/log.${i}.stdout -e /home/antonk/roofit/logs/log.${i}.stderr QCD.sh
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

input=/share/t3data3/antonk/ana/ana_v29G_07112012_DtoM_jerjes_wptw_stacoCB_all
input=/share/t3data3/antonk/ana/ana_v29G_07252012_newROOT_stacoCB_all
bgqcd=3

i=0
preNN='ptiso40/l_pt<0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1 && l_trigEF<0.2'
preNQ='ptiso40/l_pt>0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons<2 && l_trigEF<0.2'
#preNQ='ptiso20/l_pt>0.1 && ptiso20/l_pt<0.2 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons<2 && l_trigEF<0.2'
gput tagz ${i} aiso_met0to120 "--lvar met --lbin 50,0,120 --preNN \"${preNN}\" --preNQ \"${preNQ}\" " ; ((i++))
gput tagz ${i} aiso_wmt40to120 "--lvar w_mt --lbin 50,40,120 --preNN \"${preNN}\" --preNQ \"${preNQ}\" "  ; ((i++))
gput tagz ${i} aiso_wmt30to120 "--lvar w_mt --lbin 55,30,120 --preNN \"${preNN}\" --preNQ \"${preNQ}\" "  ; ((i++))
gput tagz ${i} aiso_lpt20to70 "--lvar l_pt --lbin 50,20,70 --preNN \"${preNN}\" --preNQ \"${preNQ}\" "  ; ((i++))
#gput tagz ${i} aiso_lpt15to70 "--lvar l_pt --lbin 50,15,70 --preNN \"${preNN}\" --preNQ \"${preNQ}\" "  ; ((i++))
#gput tagz ${i} lomet_isor0to1 "--llog --lvar ptiso40/l_pt --lbin 100,0,1 --preNN \"${preNN}\" --preNQ \"${preNQ}\" "  ; ((i++))

ivar=0
gput tagv ${ivar} met "--var met --bin 200,0,200" ; ((ivar++))
gput tagv ${ivar} w_mt "--var w_mt --bin 200,0,200" ; ((ivar++))
gput tagv ${ivar} l_pt "--var l_pt --bin 200,0,200" ; ((ivar++))
gput tagv ${ivar} l_abseta "--var fabs\(l_eta\) --bin 10,0,2.5 --hsource lepton_absetav" ; ((ivar++))

irun=0
for itag in `gkeys tagz`; do
    tag=`ggeta tagz $itag`
    opts=`ggetb tagz $itag`
    for iq in 0 1; do
	for bgsig in 1 4 5; do
	    for ivar in `gkeys tagv`; do
		optsv=`ggetb tagv $ivar`
		if [ "$id" == "$irun" -o "$id" == "ALL" ]; then
		    eval ./stack2.py --input ${input} -b --charge $iq ${opts} ${optsv} -o TEST_Q${iq} -t ${tag} --cut "mcw*puw*effw*trigw*wptw*isow" -m qcdfit --bgsig ${bgsig} --bgewk 5 --bgqcd ${bgqcd}
		fi
		((irun++))
		for ieta in {9..10}; do
		    #continue #disable
		    if [ "$id" == "$irun" -o "$id" == "ALL" ]; then
			eval ./stack2.py --input ${input} -b --charge $iq ${opts} ${optsv} -o TEST_Q${iq}/ETA${ieta} -t ${tag} --cut "mcw*puw*effw*trigw*wptw*isow" -m qcdfit --bgsig ${bgsig} --bgewk 5 --bgqcd ${bgqcd} --extra ${ieta}
		    fi
		    ((irun++))
		done
	    done
	done
    done
done
wait
echo "Maximum irun: $irun"

date
