#!/bin/bash

wfiles="
wspace_AA_data1200.root
wspace_AA_mc12000.root
wspace_AA_mc43000.root
wspace_BB_data3000.root
wspace_BB_mc100000.root
wspace_BB_mc12000.root
wspace_BB_mc24000.root
wspace_BB_mc30000.root
wspace_BB_mc3000.root
wspace_BB_mc48000.root
wspace_BB_mc6000.root
wspace_CC_data1200.root
wspace_CC_mc12000.root
wspace_CC_mc43000.root
"

for np in $wfiles; do
    xtra=""
    reg=`echo $np | awk 'BEGIN{FS="_"}{print $2}'`
    nevt=`echo $np | awk 'BEGIN{FS="_"}{print $3}' | sed -e 's#.root##g' -e 's#mc##g' -e 's#data##g'`
    subfile=mc_zmumu_mc_zmumu.root; evtype="mc"
    echo $np | awk 'BEGIN{FS="_"}{print $3}' | grep -q data && { subfile=data_data.root; evtype="data"; }
    #if [ ! "$evtype" == "data" ]; then continue; fi;
    m=1.0
    d=0.02
    # data range corrections
    if [ "$reg" == "BB" -a "$evtype" == "data" ]; then
	m=1.0
	d=0.05
    fi
    if [ "$reg" == "CC" -a "$evtype" == "data" ]; then
	m=1.0
	d=0.05
    fi
    if [ "$reg" == "AA" -a "$evtype" == "data" ]; then
	m=1.0
	d=0.05
    fi
    xtra="--fitmin `echo \"${m}-${d}\" | bc -l` --fitmax `echo \"${m}+${d}\" | bc -l`"
    ./keysfit.py --batch --tag cmb_${evtype}_${reg}_${nevt} --data ${subfile}/dg/dg/st_z_final/$reg/graph_lpt_P_N --ndata $nevt --rookeys wspace/$np --nscan 100 --scan ${xtra}
    if [ "$evtype" == "data" -o "$evtype" == "mc" ]; then
	./keysfit.py --batch --tag cmb_${evtype}_${reg}_${nevt}_ks --data ${subfile}/dg/dg/st_z_final/$reg/graph_lpt_P_N --ndata $nevt --rookeys wspace/$np --nscan 100 --scan ${xtra} --kolmogorov
    fi;
done
