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
xtra=""

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

nevt=1000000
nscan=500
m=1.0
d=0.08
frange="--fitmin `echo \"${m}-${d}\" | bc -l` --fitmax `echo \"${m}+${d}\" | bc -l`"
gr=${subfile}/dg/dg/st_z_final/$reg/graph_lpt_P_N
gr=${subfile}/dg/dg/st_z_final/ntuple${ntlbl}
tag="`basename ${rfile}`_${lbl}_${tt}_${evtype}_${reg}"
echo $xtra | grep -v akluit | grep -q kluit && tag="KLU_${tag}"
echo $xtra | grep -q akluit && tag="AKLU_${tag}"
echo $xtra | grep -q shift && tag="SHF_${tag}"

set -x
#./keysfit.py --region ${reg} --zmin 80 --zmax 100 --root ${rfile} --batch --tag ${tag} --data ${gr} --ndata $nevt --nscan ${nscan} --scan ${frange} ${xtra}
./keysfit.py --region ${reg} --zmin 70 --zmax 110 --root ${rfile} --batch --tag ${tag} --data ${gr} --ndata $nevt --nscan ${nscan} --scan ${frange} ${xtra}
set +x
