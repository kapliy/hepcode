#!/bin/bash

#PBS -q uct3
#PBS -l cput=08:00:00
#PBS -l walltime=08:00:00
#PBS -m e

rfile=root_all_0328.root
lbl=data_20110329
tt=cmb
evtype=data
reg=BB
xtra="--kolmogorov --shift --kluit"

rfile=$1
lbl=$2
tt=$3
evtype=$4
reg=$5
shift 5
xtra=$@

ntlbl="_${tt}"
if [ "$tt" == "cmb" ]; then ntlbl=""; fi;
subfile=${lbl}_${lbl}.root

nevt=100000
nscan=500
m=1.0
d=0.1
frange="--fitmin `echo \"${m}-${d}\" | bc -l` --fitmax `echo \"${m}+${d}\" | bc -l`"
gr=${subfile}/dg/dg/st_z_final/$reg/graph_lpt_P_N
gr=${subfile}/dg/dg/st_z_final/ntuple${ntlbl}
echo $xtra | grep -q -v kolmogorov && xtra="$xtra --template"
#echo $xtra | grep -q -v kolmogorov && xtra="$xtra --template --rookeysout wspace/wspace_${reg}_data${nevt}.root"
tag="`basename ${rfile}`_${lbl}_${tt}_${evtype}_${reg}"
echo $xtra | grep -q kolmogorov && tag="${tag}_ks"
echo $xtra | grep -q kluit && tag="KLU_${tag}"
echo $xtra | grep -q shift && tag="SHF_${tag}"

set -x
./keysfit.py --region ${reg} --zmin 70 --zmax 110 --root ${rfile} --batch --tag ${tag} --data ${gr} --ndata $nevt --nscan ${nscan} --scan ${frange} ${xtra}
set +x
