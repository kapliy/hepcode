#!/bin/bash

# a script to run a simple closure test on bb/cc QCD Monte-Carlo:
# whether inverting isolation biases MET

input=/share/t3data3/antonk/ana/ana_v29I_12042012_edboard_stacoCB_all

# MAIN PLOTTER
./stack2.py -q 0 --refline 0.06,1.94 --input ${input} -b --var "met" --bin 50,0,50 --rebin 2 --hsource "lepton_NOTabsetav" -o METSHAPE -t MET_NOMINAL --pre "ptiso40/l_pt<0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1" -m qcd_isolation --bgsig 5 --bgqcd 0 --extra 1 --lvar met
./stack2.py --refline 0.06,1.94 --input ${input} -b --var "met" --bin 50,0,50 --rebin 2 --hsource "lepton_NOTabsetav" -o METSHAPE -t MET_WMT20 --pre "ptiso40/l_pt<0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>20.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1" -m qcd_isolation --bgsig 5 --bgqcd 0 --extra 1 --lvar met

for ipt in 0 1 3 6 7; do
    ./stack2.py -q 0 --refline 0.06,1.94 --input ${input} -b --var "met" --bin 50,0,50 --rebin 2 --hsource "lepton_NOTabsetav" -o METSHAPE -t MET_NOMINAL_pt${ipt} --pre "ptiso40/l_pt<0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1" -m qcd_isolation --bgsig 5 --bgqcd 0 --extra 1 --lvar met --ipt ${ipt}&
    ./stack2.py --refline 0.06,1.94 --input ${input} -b --var "met" --bin 50,0,50 --rebin 2 --hsource "lepton_NOTabsetav" -o METSHAPE -t MET_WMT20_pt${ipt} --pre "ptiso40/l_pt<0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>20.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1" -m qcd_isolation --bgsig 5 --bgqcd 0 --extra 1 --lvar met --ipt ${ipt} &
done

for ieta in 0 1 5 9 10; do
    ./stack2.py -q 0 --refline 0.06,1.94 --input ${input} -b --var "met" --bin 50,0,50 --rebin 2 --hsource "lepton_NOTabsetav" -o METSHAPE -t MET_NOMINAL_eta${ieta} --pre "ptiso40/l_pt<0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1" -m qcd_isolation --bgsig 5 --bgqcd 0 --extra 1 --lvar met --ieta ${ieta}&
    ./stack2.py --refline 0.06,1.94 --input ${input} -b --var "met" --bin 50,0,50 --rebin 2 --hsource "lepton_NOTabsetav" -o METSHAPE -t MET_WMT20_eta${ieta} --pre "ptiso40/l_pt<0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>20.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1" -m qcd_isolation --bgsig 5 --bgqcd 0 --extra 1 --lvar met --ieta ${ieta} &
done

wait


#./stack2.py --refline 0.06,1.94 --input ${input} -b --var "met" --bin 50,0,50 --rebin 2 --hsource "lepton_NOTabsetav" -o METSHAPE -t MET_WMT30   --pre "ptiso40/l_pt<0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>30.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1" -m qcd_isolation --bgsig 5 --bgqcd 0 --extra 1 --lvar met
#./stack2.py --refline 0.86,1.24 --input ${input} -b --var "met" --bin 50,0,50 --rebin 2 --hsource "lepton_NOTabsetav" -o METSHAPE -t MET_WMT00 --pre "ptiso40/l_pt<0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>0.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1" -m qcd_isolation --bgsig 5 --bgqcd 0 --extra 1 --lvar met
#./stack2.py -q 0 --refline 0.86,1.24 --input ${input} -b --var "w_mt" --bin 50,0,50 --rebin 2 --hsource "lepton_NOTabsetav" -o METSHAPE -t WMT_NOMINAL --pre "ptiso40/l_pt<0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1" -m qcd_isolation --bgsig 5 --bgqcd 0 --extra 1 --lvar w_mt
#./stack2.py -q 0 --refline 0.86,1.24 --input ${input} -b --var "w_mt" --bin 50,0,50 --rebin 2 --hsource "lepton_NOTabsetav" -o METSHAPE -t WMT_MET00 --pre "ptiso40/l_pt<0.1 && met>0.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1" -m qcd_isolation --bgsig 5 --bgqcd 0 --extra 1 --lvar w_mt
