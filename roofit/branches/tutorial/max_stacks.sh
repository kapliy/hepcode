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

# Standard stacks
fiducial="x:0:-1:y:0:-1" # 20 GeV
fiducial="x:0:-1:y:2:-1" # 25 GeV
common="-b --input ${input} --lvar d3_abseta_lpt_met:${fiducial} --lbin 100,0,40 --lrebin 2 -o CONTROL -t P -m control_stack --charge 3 --bgqcd ${bgqcd} --metallsys"
if [ "0" == "1" ]; then
    # 0.92,1.095
    ./stack2.py --hsource "d3_eta_lpt_met:y:2:-1:z:0:-1" --bin 10,-2.5,2.5 --refline 0.85,1.175 ${common} &> LOG.stacks.eta &
    #./stack2.py --hsource "d3_abseta_lpt_eta:${fiducial}" --bin 10,-2.5,2.5 --rebin 2 --refline 0.85,1.175 ${common} &> LOG.stacks.eta &
    ./stack2.py --hsource "d3_abseta_lpt_phi:${fiducial}" --bin 10,-3.14,3.14 --rebin 4 --refline 0.85,1.175 ${common} &> LOG.stacks.phi &
    ./stack2.py --hsource "d3_abseta_lpt_wmt:${fiducial}" --bin 100,40,120 --rebin 2 --refline 0.85,1.175 ${common} &> LOG.stacks.wmt &
    ./stack2.py --hsource "d3_abseta_lpt_wpt:${fiducial}" --bin 100,0,120  --rebin 2 --refline 0.85,1.175 ${common} &> LOG.stacks.wpt &
    ./stack2.py --hsource "d3_abseta_lpt_met:${fiducial}" --bin 100,25,120 --rebin 2 --refline 0.85,1.175 ${common} &> LOG.stacks.met &
    ./stack2.py --hsource "lpt" --bin 100,25,120  --rebin 2 --refline 0.85,1.175 ${common} &> LOG.stacks.lpt &
fi

./stack2.py -b --input ${input} --hsource d3_abseta_lpt_met:${fiducial} --bin 100,25,120  --rebin 2 --refline 0.85,1.175 -o CONTROL -t D -m control_stack --charge 0 --bgqcd ${bgqcd} --lvar d3_abseta_lpt_met:${fiducial} --lbin 100,0,40 --lrebin 2 --metallsys  # --nomonly

echo "Please wait..."
wait
