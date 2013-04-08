#!/bin/bash

source asym_common.sh
lbl=01292013_paper  # just a typo fix in uint histograms

common_opts="--release ${release} --save-ntuples 7 --apply-pileup --data-range DtoM --noSys"

echo "Submitting..."

#for i in 0 100; do
for i in 0; do
    out=${coredir}/ana_v${ntuple}_${lbl}_${muname[$i]}_MCPscale
    xtra="${common_opts} --muon-offset 1 --muon-scale 999 --muon-type ${i}"
    # nominal/noscale (to derive KC corrections) + datascaled (for closure_C tests):
    #submit_data
    # nominal/smeared-scaled (for closure_K tests) + no scale at all/smearonly (to derive KC corrections)
    #submit_zmumu
    submit_all
done
