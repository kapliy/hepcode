#!/bin/bash

# Similar to pT control stacks: pT in fine bins in each |Eta| slice

source config.sh

# refit qcd for each systematic? As of Jan 22, we decided to NOT refit.
METALLSYS=""
METRANGE="100,0,60"

YRANGE="y:0:-1" # pt20
YRANGE="y:2:-1" # pt25

# PLOTTING IN VARIOUS PT BINS
function runs() {
    if [ -z "${ieta}" ]; then
	echo "ERROR: ieta not defined"
	exit 0;
    fi
    out=JAN_REWEIGHT
    log=LOG.ieta${ieta}
    hsource="d3_abseta_lpt_lpt:x:${ieta}:${ieta}:${YRANGE}"
    #hsource="d3_abseta_lpt_met:x:${ieta}:${ieta}:${YRANGE}"
    REFLINE_ETA="0.85,1.175"
    ./stack2.py -o ${out} --hsource "${hsource}" --bin 120,0,120  --rebin 1 --refline 0.85,1.175 ${common} &> ${log}.wpt &
}
for ieta in {1..11}; do
    # met fits
    #lvar="d3_abseta_lpt_met:x:${ieta}:${ieta}:${YRANGE}"
    lvar="d3_abseta_lpt_met:x:0:-1:${YRANGE}"
    # pt fits
    #lvar="d3_abseta_lpt_lpt:x:${ieta}:${ieta}:${YRANGE}"
    #lvar="d3_abseta_lpt_lpt:x:0:-1:${YRANGE}"
    common="-b --input ${input} --lvar ${lvar} --lbin ${METRANGE} --lrebin 2 -t P${ipt} -m control_stack --charge 3 --bgqcd ${bgqcd} ${METALLSYS} --nomonly"
    runs
done

echo "Please wait: `jobs | wc -l` jobs..."
wait
