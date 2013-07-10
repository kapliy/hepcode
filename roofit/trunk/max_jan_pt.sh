#!/bin/bash

# Similar to pT control stacks: pT in fine bins in each |Eta| slice

source config.sh

# refit qcd for each systematic? As of Jan 22, we decided to NOT refit.
METALLSYS=""
METRANGE="100,0,60"

YRANGE="y:0:-1" # pt20
YRANGE="y:2:-1" # pt25

rm -f jan_wmunu_pt_v?.root

function runs() {
    if [ -z "${ieta}" ]; then
	echo "ERROR: ieta not defined"
	exit 0;
    fi
    out=JAN_REWEIGHT
    log=LOG.ipt${ipt}.ieta${ieta}
    hsource="d3_abseta_lpt_lpt:x:${ieta}:${ieta}:${YRANGE}"
    REFLINE_ETA="0.85,1.175"
    ./stack2.py -o ${out} --hsource "${hsource}" --bin 120,0,120  --rebin 1 --refline 0.85,1.175 ${common} &> ${log}.dat &
}
# TODO - add a loop over ipt, and then save ALL ipt/ieta variations. Lastly, write a small pyroot script to add up pt slices
for ipt in {2..7}; do
    YRANGE="y:${ipt}:${ipt}"
    for ieta in {1..11}; do
        # met fits
	lvar="d3_abseta_lpt_met:x:${ieta}:${ieta}:${YRANGE}"
        #lvar="d3_abseta_lpt_met:x:0:-1:${YRANGE}"
	common="-b --input ${input} --lvar ${lvar} --lbin ${METRANGE} --lrebin 2 -t PJan_pt${ipt}_eta${ieta} -m control_stack --charge 3 --bgqcd ${bgqcd} ${METALLSYS} --nomonly"
	runs
    done
    if [ "${ipt}" == "3" ]; then
	echo "Please wait (mid-way): `jobs | wc -l` jobs..."
	wait
    fi
done
echo "Please wait: `jobs | wc -l` jobs..."
wait
