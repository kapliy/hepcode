#!/bin/bash

# Control plot stacks

source config.sh

# refit qcd for each systematic? As of Jan 22, we decided to NOT refit.
METALLSYS="--metallsys"
METALLSYS=""

# as of Jan 22, 0..60 is the default met fit range
METRANGE="100,0,40"
METRANGE="100,0,60"

# eta with QCD template from external root file
if [ "0" == "1" ]; then
    echo ''
    #./stack2.py -b --input ${input} --hsource "lepton_etav" -o STACKS/CONTROL -t P -m control_stack --bgsig 5 --bgewk 5 --bgqcd ${bgqcd} --refline 0.94,1.06 --qcdsource "${OUTDB}.eta.1D.root:qcd_MCAverage" &
    # double ratio:
    #./stack2.py -b --input ${input} --hsource "lepton_etav" -o STACKS/CONTROL -t P_dratio -m control_stack_dratio --bgsig 5 --bgewk 5 --bgqcd ${bgqcd} --refline 0.94,1.06 --qcdsource "${OUTDB}.eta.1D.root:qcd_MCAverage" &
fi

# INCLUSIVE STACKS
function run() {
    out=STACKS/CONTROL${PT}
    log=LOG.stacks${PT}
    # 0.92,1.095
    PB=2
    if [ "${PT}" == "20" ]; then PB=0; fi
    REFLINE_ETA="0.90,1.12"
    REFLINE_ETA="0.85,1.175"
    ./stack2.py -o ${out} --hsource "d3_eta_lpt_met:y:${PB}:-1:z:0:-1" --bin 10,-2.5,2.5 --refline ${REFLINE_ETA} ${common} &> ${log}.eta &
    ./stack2.py -o ${out} --hsource "d3_abseta_lpt_phi:${fiducial}" --bin 10,-3.14,3.14 --rebin 4 --refline 0.85,1.175 ${common} &> ${log}.phi &
    ./stack2.py -o ${out} --hsource "d3_abseta_lpt_wmt:${fiducial}" --bin 100,40,120 --rebin 2 --refline 0.85,1.175 ${common} &> ${log}.wmt &
    ./stack2.py -o ${out} --hsource "d3_abseta_lpt_wpt:${fiducial}" --bin 100,0,120  --rebin 2 --refline 0.85,1.175 ${common} &> ${log}.wpt &
    ./stack2.py -o ${out} --hsource "d3_abseta_lpt_met:${fiducial}" --bin 100,25,120 --rebin 2 --refline 0.85,1.175 ${common} &> ${log}.met &
    ./stack2.py -o ${out} --hsource "lpt" --bin 100,${PT},120  --rebin 2 --refline 0.85,1.175 ${common} &> ${log}.lpt &
}
# Standard stacks 20 GeV
fiducial="x:0:-1:y:0:-1" # 20 GeV
common="-b --input ${input} --lvar d3_abseta_lpt_met:${fiducial} --lbin ${METRANGE} --lrebin 2 -t P -m control_stack --charge 3 --bgqcd ${bgqcd} ${METALLSYS} --lnofits"
PT=20
run
# Standard stacks 25 GeV
fiducial="x:0:-1:y:2:-1" # 25 GeV
common="-b --input ${input} --lvar d3_abseta_lpt_met:${fiducial} --lbin ${METRANGE} --lrebin 2 -t P -m control_stack --charge 3 --bgqcd ${bgqcd} ${METALLSYS} --lnofits"
PT=25
run

# QCD MET SHAPE PLOTS (DIFFERENT GENERATORS)
# A set of plots to demonstrate differences in MET high tail shapes due to usage of different generators
fiducial="x:0:-1:y:2:-1" # 25 GeV
out=STACKS/METSHAPE_GEN
log=LOG.stackMC
./stack2.py -b --input ${input} --hsource d3_abseta_lpt_met:${fiducial} --bin 100,25,120  --rebin 2 --refline 0.85,1.175 -o ${out} -t D -m control_stack --charge 0 --bgqcd ${bgqcd} --lvar d3_abseta_lpt_met:${fiducial} --lbin ${METRANGE} --lrebin 2 --metallsys  &> ${log}.met &  # --nomonly

# PLOTTING IN VARIOUS PT BINS
function runs() {
    if [ -z "${ipt}" ]; then
	echo "ERROR: ipt not defined"
	exit 0;
    fi
    out=STACKS/CONTROL_BINS
    log=LOG.ipt${ipt}
    fiducial="x:0:-1:y:${ipt}:${ipt}"
    REFLINE_ETA="0.90,1.12"
    REFLINE_ETA="0.85,1.175"
    ./stack2.py -o ${out} --hsource "d3_eta_lpt_met:y:${ipt}:${ipt}:z:0:-1" --bin 10,-2.5,2.5 --refline ${REFLINE_ETA} ${common} &> ${log}.eta &
    ./stack2.py -o ${out} --hsource "d3_abseta_lpt_phi:${fiducial}" --bin 10,-3.14,3.14 --rebin 4 --refline 0.85,1.175 ${common} &> ${log}.phi &
    ./stack2.py -o ${out} --hsource "d3_abseta_lpt_wmt:${fiducial}" --bin 100,40,120 --rebin 2 --refline 0.85,1.175 ${common} &> ${log}.wmt &
    ./stack2.py -o ${out} --hsource "d3_abseta_lpt_wpt:${fiducial}" --bin 100,0,120  --rebin 2 --refline 0.85,1.175 ${common} &> ${log}.wpt &
    ./stack2.py -o ${out} --hsource "d3_abseta_lpt_met:${fiducial}" --bin 100,25,120 --rebin 2 --refline 0.85,1.175 ${common} &> ${log}.met &
}
for ipt in {1..7}; do
    common="-b --input ${input} --lvar d3_abseta_lpt_met:x:0:-1:y:${ipt}:${ipt} --lbin ${METRANGE} --lrebin 2 -t P${ipt} -m control_stack --charge 3 --bgqcd ${bgqcd} ${METALLSYS} --lnofits"
    runs
    if [ "${ipt}" == "2" ]; then
	echo "Waiting mid-way: `jobs | wc -l` jobs..."
	wait
    fi
done

echo "Waiting mid-way (2): `jobs | wc -l` jobs..."
wait

# PLOTTING IN VARIOUS ETA BINS
YRANGE="y:0:-1" # pt20
YRANGE="y:2:-1" # pt25
function runs() {
    if [ -z "${ieta}" ]; then
	echo "ERROR: ieta not defined"
	exit 0;
    fi
    out=STACKS/CONTROL_IETA
    log=LOG.ieta${ieta}
    #hsource="d3_abseta_lpt_lpt:x:${ieta}:${ieta}:${YRANGE}" #FIXME
    hsource="d3_abseta_lpt_met:x:${ieta}:${ieta}:${YRANGE}"
    ./stack2.py -o ${out} --hsource "${hsource}" --bin 120,0,120  --rebin 1 --refline 0.85,1.175 ${common} &> ${log}.lpt &
}
for ieta in {1..11}; do
    # met fits
    # lvar="d3_abseta_lpt_met:x:${ieta}:${ieta}:${YRANGE}"  # fits in eta bins
    lvar="d3_abseta_lpt_met:x:0:-1:${YRANGE}"               # inclusive fits
    common="-b --input ${input} --lvar ${lvar} --lbin ${METRANGE} --lrebin 2 -t P${ieta} -m control_stack --charge 3 --bgqcd ${bgqcd} ${METALLSYS} --nomonly"
    runs
done

echo "Please wait: `jobs | wc -l` jobs..."
wait
