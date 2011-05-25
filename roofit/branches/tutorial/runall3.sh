#!/bin/bash
# KS comparison of 1/pt in data and mc for various scale corrections

fnames="ROOT/root_all_0522_dataMCclosure.root"
funcs="gaus"
regs="ALL AA BB CC"
tts="cmb id exms"
i=0
folders[((i++))]="data data_00"
folders[((i++))]="data data_22"
folders[((i++))]="data data_33"

for tt in $tts; do
    ntlbl="_${tt}"
    if [ "$tt" == "cmb" ]; then ntlbl=""; fi;
    for reg in $regs; do
	for fname in $fnames; do
	    for func in $funcs; do
		for fold in "${folders[@]}"; do
		    data=`echo ${fold} | cut -d ' ' -f 1`
		    mc=`echo ${fold} | cut -d ' ' -f 2`
		    ./keysspectra.py -b --scan --root ${fname} --region ${reg} --data0 ${data}_${data}.root/dg/dg/st_z_final/ntuple${ntlbl} --data1 ${mc}_${mc}.root/dg/dg/st_z_final/ntuple${ntlbl} -t CURV_${tt}_${reg}_${mc} &
		done
		wait
	    done
	done
    done
done
