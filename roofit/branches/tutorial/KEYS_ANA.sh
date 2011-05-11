#!/bin/bash

rfile=$1
lbl_data=$2
lbl_mc=$3
tt=$4
reg=$5
pfx=$6

ntlbl="_${tt}"
if [ "$tt" == "cmb" ]; then ntlbl=""; fi;

nmc=100000
func=egge   #gaus
res=3

sfx="_results.rtxt"

# truth on full dataset
if [ "1" -eq "1" ]; then
    subfile=${lbl_mc}_${lbl_mc}.root;
    gr=${subfile}/dg/dg/st_z_final/ntuple${ntlbl}
    tag="ZMC_${pfx}`basename ${rfile}`_${lbl_mc}_${tt}_${reg}"
    xtra=""
    if [ "${res}" == "1" ]; then xtra="--min 80 --max 100"; fi
    if [ "${func}" == "gaus" ]; then xtra="--min 88 --max 94"; fi
    ./zpeak.py -b --root ${rfile} --res ${res} --${func} --region ${reg} --data ${gr} --ndata ${nmc} -t ${tag} ${xtra} --ext eps
fi;

# data, using MC fitted value from truth and relative scale (R0/eR0) from keysfit.py
if [ "1" -eq "1" ]; then
    subfile=${lbl_data}_${lbl_data}.root;
    gr=${subfile}/dg/dg/st_z_final/ntuple${ntlbl}
    RZ=ZMC_${pfx}`basename ${rfile}`_${lbl_mc}_${tt}_${reg}${sfx}
    mz0=`tail -n2 ${RZ} | head -n1 | cut -d ' ' -f 3`
    RTXT=${pfx}`basename ${rfile}`_${lbl_data}_${tt}_data_${reg}${sfx}
    R=`head -n1 ${RTXT} | cut -d ' ' -f 14`
    eR=`tail -n1 ${RTXT} | cut -d ' ' -f 6`
    tag="ZDATA_${pfx}`basename ${rfile}`_${lbl_data}_${tt}_${reg}"
    echo "Printing contents of input files for region ${reg}:"
    echo ${RZ} ${RTXT}
    cat ${RZ} ${RTXT} /dev/null
    if [ -z "${R}" -o -z "${eR}" -o -z "${mz0}" ]; then
	echo "Skipping region ${reg} - missing some inputs"
	echo "R=|${R}| eR=|${eR}| mz0=|${mz0}|"
    else
	xtra=""
	if [ "${res}" == "1" ]; then xtra="--min 80 --max 100"; fi
	if [ "${func}" == "gaus" ]; then xtra="--min 88 --max 94"; fi
	./zpeak.py -b --root ${rfile} --res ${res} --${func} --region ${reg} --mz0 ${mz0} --R ${R} --eR ${eR} --data ${gr} ${xtra} -t ${tag} --ext eps
    fi;
fi;

#BARREL-ENDCAP CROSS-CHECK:
if [ "0" -eq "1" ]; then
    func=egge
    reg=BA
    ./zpeak.py -b --root ${rfile} --res ${res} --${func} --mz0 90.963 --kp 1.00114 --ekp 0.00191 --data data_data.root/dg/dg/st_z_final/$reg/graph_Z_m_eta --ndata 10000 -t data_${reg}_${func}
    reg=BC
    ./zpeak.py -b --root ${rfile} --res ${res} --${func} --mz0 90.950 --kp 1.00114 --ekp 0.00191 --data data_data.root/dg/dg/st_z_final/$reg/graph_Z_m_eta --ndata 10000 -t data_${reg}_${func}
    reg=AB
    ./zpeak.py -b --root ${rfile} --res ${res} --${func} --mz0 90.914 --km 0.996 --ekm 0.002 --data data_data.root/dg/dg/st_z_final/$reg/graph_Z_m_eta --ndata 10000 -t data_${reg}_${func}
    reg=CB
    ./zpeak.py -b --root ${rfile} --res ${res} --${func} --mz0 90.902 --km 0.996 --ekm 0.002 --data data_data.root/dg/dg/st_z_final/$reg/graph_Z_m_eta --ndata 10000 -t data_${reg}_${func}
fi
