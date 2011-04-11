#!/bin/bash

wfiles="
wspace_AA_data10000.root
wspace_BB_data10000.root
wspace_CC_data10000.root
wspace_Bcc_data10000.root
wspace_Baa_data10000.root
"

rfile=root_all_0328.root
lbl=data_20110329
tt=cmb

rfile=$1
lbl=$2
tt=$3

ntlbl="_${tt}"
if [ "$tt" == "cmb" ]; then ntlbl=""; fi;

for np in $wfiles; do
    xtra=""
    reg=`echo $np | awk 'BEGIN{FS="_"}{print $2}'`
    nevt=`echo $np | awk 'BEGIN{FS="_"}{print $3}' | sed -e 's#.root##g' -e 's#mc##g' -e 's#data##g'`
    subfile=mc_zmumu_mc_zmumu.root; evtype="mc"
    echo $np | awk 'BEGIN{FS="_"}{print $3}' | grep -q data && { subfile=data_data.root; evtype="data"; }
    echo $np | awk 'BEGIN{FS="_"}{print $3}' | grep -q data && { subfile=${lbl}_${lbl}.root; evtype="data"; }
    if [ ! "$evtype" == "data" ]; then continue; fi;
    m=1.0
    d=0.10
    xtra="--fitmin `echo \"${m}-${d}\" | bc -l` --fitmax `echo \"${m}+${d}\" | bc -l`"
    gr=${subfile}/dg/dg/st_z_final/$reg/graph_lpt_P_N
    gr=${subfile}/dg/dg/st_z_final/ntuple${ntlbl}
    ./keysfit.py --template --region ${reg} --zmin 70 --zmax 110 --root ${rfile} --batch --tag ${rfile}_${lbl}_${tt}_${evtype}_${reg}_${nevt} --data ${gr} --ndata $nevt --rookeysout wspace/$np --nscan 400 --scan ${xtra}
    if [ "$evtype" == "data" -o "$evtype" == "mc" ]; then
	./keysfit.py --region ${reg} --zmin 70 --zmax 110 --root ${rfile} --batch --tag ${rfile}_${lbl}_${tt}_${evtype}_${reg}_${nevt}_ks --data ${gr} --ndata $nevt --nscan 400 --scan ${xtra} --kolmogorov
    fi;
done
