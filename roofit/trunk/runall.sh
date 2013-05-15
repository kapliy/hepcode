#!/bin/bash
# Reconstructs Z peak for ALL Z's (full fiducial eta range)

fnames="ROOT/root_all_0522_dataMCclosure.root"
funcs="gaus egge"
folders="data data_00 data_33 mc_zmumu mc_zmumuO"
regs="AB BA CB BC BB AA CC ALL"
tts="cmb id exms"

for fname in $fnames; do
    for tt in $tts; do
	ntlbl="_${tt}"
	if [ "$tt" == "cmb" ]; then ntlbl=""; fi;
	for reg in $regs; do
	    for func in $funcs; do
		for fold in ${folders}; do
		    ./zpeak.py -b --root ${fname} --res 3 --${func} --region ${reg} --data ${fold}_${fold}.root/dg/dg/st_z_final/ntuple${ntlbl} --ndata 100000 -t ZPEAK_${tt}_${reg}_${fold}_${func} &
		done
	    done
	    wait
	done
    done
done

