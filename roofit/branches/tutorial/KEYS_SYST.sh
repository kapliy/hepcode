#!/bin/bash

# study of systematic effects

rfile=root_data_v19.root

np=wspace_BB_data3000.root

xtra=""
reg=`echo $np | awk 'BEGIN{FS="_"}{print $2}'`
nevt=`echo $np | awk 'BEGIN{FS="_"}{print $3}' | sed -e 's#.root##g' -e 's#mc##g' -e 's#data##g'`
subfile=mc_zmumu_mc_zmumu.root; evtype="mc"
echo $np | awk 'BEGIN{FS="_"}{print $3}' | grep -q data && { subfile=data_data.root; evtype="data"; }

m=1.0
d=0.02
# data range corrections
if [ "$reg" == "BB" -a "$evtype" == "data" ]; then
    m=1.005
    d=0.05
fi
if [ "$reg" == "CC" -a "$evtype" == "data" ]; then
    m=1.013
    d=0.05
fi
if [ "$reg" == "AA" -a "$evtype" == "data" ]; then
    m=0.99
    d=0.05
fi
xtra="--fitmin `echo \"${m}-${d}\" | bc -l` --fitmax `echo \"${m}+${d}\" | bc -l`"
gr=${subfile}/dg/dg/st_z_final/$reg/graph_lpt_P_N

# Study the effect of various Keys PDFs: adaptive vs non-adaptive, and different bandwidth smoothing parameters
if [ "0" -eq "1" ]; then
    ./keysfit.py --root ${rfile} --batch --tag sys_ks --data ${gr} --mc ${gr} --ndata $nevt --nscan 100 --scan ${xtra} --kolmogorov
    for sm in 1.0 1.5 2.0 2.5; do
	./keysfit.py --root ${rfile} --batch --template --roomodel 2 --smooth ${sm} --tag sys_chi_KE${sm} --data ${gr} --mc ${gr} --ndata $nevt --nscan 100 --scan ${xtra}
	./keysfit.py --root ${rfile} --batch --template --roomodel 3 --smooth ${sm} --tag sys_chi_ND${sm} --data ${gr} --mc ${gr} --ndata $nevt --nscan 100 --scan ${xtra}
    done
fi;

# Study the effect of fit ranges
if [ "1" -eq "1" ]; then
    echo ./keysfit.py --root ${rfile} --batch --tag sys_chi_KE${sm} --data ${gr} --mc ${gr} --ndata $nevt --nscan 100 --scan ${xtra}
fi
