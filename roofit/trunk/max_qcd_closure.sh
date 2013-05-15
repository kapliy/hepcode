#!/bin/bash

# a script to run a simple closure test on bb/cc QCD Monte-Carlo:
# whether inverting isolation biases MET

source config.sh

# MAIN PLOTTER
for mode in qcd_isolation qcd_charge; do
    #./stack2.py -q 0 --refline 0.06,1.94 --input ${input} -b --var "met" --bin 50,0,50 --rebin 2 --hsource "lepton_NOTabsetav" -o METSHAPE -t MET_NOMINAL --pre "ptiso40/l_pt<0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1" -m qcd_isolation --bgsig 5 --bgqcd 0 --extra 1 --lvar met
    ./stack2.py -q 0 --refline 0.7,1.34 --input ${input} -b --var "met" --bin 50,0,50 --rebin 2 --hsource "lepton_NOTabsetav" -o METSHAPE -t ${mode} --pre "ptiso40/l_pt<0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>0.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1" -m ${mode} --bgsig 5 --bgqcd 0 --extra 1 --lvar met
done

for ipt in 0 1 3 6 7; do
    continue
    ./stack2.py -q 0 --refline 0.06,1.94 --input ${input} -b --var "met" --bin 50,0,50 --rebin 2 --hsource "lepton_NOTabsetav" -o METSHAPE -t MET_NOMINAL_pt${ipt} --pre "ptiso40/l_pt<0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1" -m qcd_isolation --bgsig 5 --bgqcd 0 --extra 1 --lvar met --ipt ${ipt}&
    ./stack2.py --refline 0.06,1.94 --input ${input} -b --var "met" --bin 50,0,50 --rebin 2 --hsource "lepton_NOTabsetav" -o METSHAPE -t MET_WMT20_pt${ipt} --pre "ptiso40/l_pt<0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>20.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1" -m qcd_isolation --bgsig 5 --bgqcd 0 --extra 1 --lvar met --ipt ${ipt} &
done

for ieta in 0 1 5 9 10; do
    continue
    ./stack2.py -q 0 --refline 0.06,1.94 --input ${input} -b --var "met" --bin 50,0,50 --rebin 2 --hsource "lepton_NOTabsetav" -o METSHAPE -t MET_NOMINAL_eta${ieta} --pre "ptiso40/l_pt<0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1" -m qcd_isolation --bgsig 5 --bgqcd 0 --extra 1 --lvar met --ieta ${ieta}&
    ./stack2.py --refline 0.06,1.94 --input ${input} -b --var "met" --bin 50,0,50 --rebin 2 --hsource "lepton_NOTabsetav" -o METSHAPE -t MET_WMT20_eta${ieta} --pre "ptiso40/l_pt<0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>20.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1" -m qcd_isolation --bgsig 5 --bgqcd 0 --extra 1 --lvar met --ieta ${ieta} &
done

wait
