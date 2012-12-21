#!/bin/bash

# Control plot stacks

source config.sh

# eta with QCD template from external root file
if [ "0" == "1" ]; then
    echo ''
    #./stack2.py -b --input ${input} --hsource "lepton_etav" -o CONTROL -t P -m control_stack --bgsig 5 --bgewk 5 --bgqcd ${bgqcd} --refline 0.94,1.06 --qcdsource "${OUTDB}.eta.1D.root:qcd_MCAverage" &
    # double ratio:
    #./stack2.py -b --input ${input} --hsource "lepton_etav" -o CONTROL -t P_dratio -m control_stack_dratio --bgsig 5 --bgewk 5 --bgqcd ${bgqcd} --refline 0.94,1.06 --qcdsource "${OUTDB}.eta.1D.root:qcd_MCAverage" &
fi

function run() {
    out=CONTROL${PT}
    log=LOG.stacks${PT}
    # 0.92,1.095
    PB=2
    if [ "${PT}" == "20" ]; then PB=0; fi
    ./stack2.py -o ${out} --hsource "d3_eta_lpt_met:y:${PB}:-1:z:0:-1" --bin 10,-2.5,2.5 --refline 0.90,1.11 ${common} &> ${log}.eta &
    #./stack2.py -o ${out} --hsource "d3_abseta_lpt_eta:${fiducial}" --bin 10,-2.5,2.5 --rebin 2 --refline 0.85,1.175 ${common} &> ${log}.eta &
    ./stack2.py -o ${out} --hsource "d3_abseta_lpt_phi:${fiducial}" --bin 10,-3.14,3.14 --rebin 4 --refline 0.85,1.175 ${common} &> ${log}.phi &
    ./stack2.py -o ${out} --hsource "d3_abseta_lpt_wmt:${fiducial}" --bin 100,40,120 --rebin 2 --refline 0.85,1.175 ${common} &> ${log}.wmt &
    ./stack2.py -o ${out} --hsource "d3_abseta_lpt_wpt:${fiducial}" --bin 100,0,120  --rebin 2 --refline 0.85,1.175 ${common} &> ${log}.wpt &
    ./stack2.py -o ${out} --hsource "d3_abseta_lpt_met:${fiducial}" --bin 100,25,120 --rebin 2 --refline 0.85,1.175 ${common} &> ${log}.met &
    ./stack2.py -o ${out} --hsource "lpt" --bin 100,${PT},120  --rebin 2 --refline 0.85,1.175 ${common} &> ${log}.lpt &
}

# Standard stacks
fiducial="x:0:-1:y:0:-1" # 20 GeV
common="-b --input ${input} --lvar d3_abseta_lpt_met:${fiducial} --lbin 100,0,40 --lrebin 2 -t P -m control_stack --charge 3 --bgqcd ${bgqcd} --metallsys"
PT=20
run

fiducial="x:0:-1:y:2:-1" # 25 GeV
common="-b --input ${input} --lvar d3_abseta_lpt_met:${fiducial} --lbin 100,0,40 --lrebin 2 -t P -m control_stack --charge 3 --bgqcd ${bgqcd} --metallsys"
PT=25
run

fiducial="x:0:-1:y:2:-1" # 25 GeV
out=METSHAPE_GEN
log=LOG.stackMC
./stack2.py -b --input ${input} --hsource d3_abseta_lpt_met:${fiducial} --bin 100,25,120  --rebin 2 --refline 0.85,1.175 -o ${out} -t D -m control_stack --charge 0 --bgqcd ${bgqcd} --lvar d3_abseta_lpt_met:${fiducial} --lbin 100,0,40 --lrebin 2 --metallsys  &> ${log}.met &  # --nomonly

echo "Please wait..."
wait
