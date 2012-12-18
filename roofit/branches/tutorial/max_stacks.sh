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
common="-b --input ${input} --lvar 'd3_abseta_lpt_met:x:0:-1:y:0:-1' --lbin 100,0,40 --lrebin 2 -o CONTROL -t P -m control_stack --charge BOTH --bgqcd ${bgqcd}"
if [ "0" == "1" ]; then
    ./stack2.py --hsource "lepton_etav" --bin 10,-2.5,2.5 --refline 0.92,1.095 ${common} &
    ./stack2.py --hsource "lepton_phi" --bin 10,-3.14,3.14 --rebin 2 --refline 0.92,1.095 ${common} &
    ./stack2.py --hsource "wmt" --bin 100,40,120 --rebin 2 --refline 0.85,1.175 ${common} &
    ./stack2.py --hsource "wpt" --bin 100,0,120  --rebin 2 --refline 0.85,1.175 ${common} &
    ./stack2.py --hsource "met" --bin 100,25,120 --rebin 2 --refline 0.85,1.175 ${common} &
    ./stack2.py --hsource "lpt" --bin 100,0,120  --rebin 2 --refline 0.85,1.175 ${common} &
fi

./stack2.py -b --input ${input} --hsource "d3_abseta_lpt_met:x:0:-1:y:0:-1" --bin 100,25,120  --rebin 2 --refline 0.85,1.175 -o CONTROL -t D -m control_stack --charge 0 --bgqcd ${bgqcd} --lvar "d3_abseta_lpt_met:x:0:-1:y:0:-1" --lbin 100,0,40 --lrebin 2

echo "Please wait..."
wait
