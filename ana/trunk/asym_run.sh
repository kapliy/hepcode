#!/bin/bash

export MODE=pbs
source asym_common.sh

i=$1
if [ "$#" -eq "1" ]; then
    echo "Muon type: $i"
else
    i=0
    echo "Reverting to default muon type: $i"
fi

if [ -z "${muname[$i]}" ]; then
    echo "UNKNOWN MUON TYPE"
    exit 1
fi
out=${coredir}/ana_v${ntuple}_${lbl}_${muname[$i]}_all/pt${PT}
xtra="${common_opts} --muon-offset 1 --muon-type $i --muon-scale 3"

echo "Submitting data and MC11C ..."

#submit_sample wasymmetry29I_data_periodJ.dat
#submit_sample wasymmetry29I_mc_powheg_pythia_wminmunu.dat
#submit_zmumu_all
#submit_mc
###submit_all
submit_wmumu_nlo

PT=25
out=${coredir}/ana_v${ntuple}_${lbl}_${muname[$i]}_all/pt${PT}
xtra="${common_opts} --muon-offset 1 --muon-type $i --muon-scale 1000"
submit_wmumu_nlo
###submit_data # for cutflow
