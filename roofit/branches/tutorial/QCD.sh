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
    anaquick
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

input=/share/t3data3/antonk/ana/ana_v29G_07102012_DtoM_jerjes_wptw_stacoCB_all
bgqcd=0

i=0
preNN='ptiso40/l_pt<0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1 && l_trigEF<0.2'
preNQ='ptiso40/l_pt>0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons<2 && l_trigEF<0.2'
preFN='ptiso40/l_pt<0.1 && met>0.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1 && l_trigEF<0.2'
preFQ='ptiso40/l_pt>0.1 && met>0.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons<2 && l_trigEF<0.2'
#gput tagz ${i} aiso_met0to100 "--lvar met --lbin 200,0,100 --preNN \"${preNN}\" --preNQ \"${preNQ}\" --preFN \"${preFN}\" --preFQ \"${preFQ}\" " ; ((i++))
gput tagz ${i} aiso_met0to120 "--lvar met --lbin 50,0,120 --preNN \"${preNN}\" --preNQ \"${preNQ}\" --preFN \"${preFN}\" --preFQ \"${preFQ}\" " ; ((i++))
gput tagz ${i} aiso_wmt40to120 "--lvar w_mt --lbin 50,40,120 --preNN \"${preNN}\" --preNQ \"${preNQ}\" --preFN \"${preFN}\" --preFQ \"${preFQ}\" "  ; ((i++))
gput tagz ${i} aiso_lpt20to70 "--lvar l_pt --lbin 50,20,70 --preNN \"${preNN}\" --preNQ \"${preNQ}\" --preFN \"${preFN}\" --preFQ \"${preFQ}\" "  ; ((i++))
preNN='ptiso40/l_pt<0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1 && l_trigEF<0.2'
preNQ='ptiso40/l_pt<1.0 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons<2 && l_trigEF<0.2'
preFN='ptiso40/l_pt<0.1 && met<25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1 && l_trigEF<0.2'
preFQ='ptiso40/l_pt<1.0 && met<25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons<2 && l_trigEF<0.2'
gput tagz ${i} lomet_isor0to1 "--llog --lvar ptiso40/l_pt --lbin 100,0,1 --preNN \"${preNN}\" --preNQ \"${preNQ}\" --preFN \"${preFN}\" --preFQ \"${preFQ}\" "  ; ((i++))

irun=0
for itag in `gkeys tagz`; do
    tag=`ggeta tagz $itag`
    opts=`ggetb tagz $itag`
    for bgsig in 0 1 4 5; do
	for iq in 0 1 2; do
	    if [ "$id" == "$irun" -o "$id" == "ALL" ]; then
		eval ./stack2.py --input ${input} -b --charge $iq ${opts} --var "met" --bin "200,0,200" -o TEST -t ${tag} --cut "mcw*puw*effw*trigw*wptw" -m qcdfit --bgsig ${bgsig} --bgewk 5 --bgqcd ${bgqcd}
	    fi
	    ((irun++))
	    for ieta in {0..10}; do
		continue
		if [ "$id" == "$irun" -o "$id" == "ALL" ]; then
		    eval ./stack2.py --input ${input} -b --charge $iq ${opts} --var "met" --bin "200,0,200" -o TEST -t ${tag}_ETA${ieta} --cut "mcw*puw*effw*trigw*wptw" -m qcdfit --bgsig ${bgsig} --bgewk 5 --bgqcd ${bgqcd} --extra ${ieta}
		fi
		((irun++))
	    done
	done
    done
done
wait
echo "Maximum irun: $irun"

date
