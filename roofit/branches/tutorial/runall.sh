#!/bin/bash
# Reconstructs Z peak for ALL Z's (full fiducial eta range)

fnames="ROOT/root_all_0522_dataMCclosure.root"
funcs="gaus"
folders="data data_00 data_33 mc_zmumu mc_zmumu_00 mc_zmumuO mc_zmumuO_00 mc_zmumuO_22"
for fname in $fnames; do
    for func in $funcs; do
	for fold in ${folders}; do
	    ./zpeak.py -b --root ${fname} --res 3 --${func} --region ALL --data ${fold}_${fold}.root/dg/dg/st_z_final/ntuple --ndata 100000 -t ALL_${fold}_${func}
	done
    done
done
