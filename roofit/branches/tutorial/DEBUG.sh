#!/bin/bash

# This is to make comparisons of different generators in certain key variables

input=/share/t3data3/antonk/ana/ana_v29G_07252012_newROOT_stacoCB_all
input=/share/t3data3/antonk/ana/ana_v29G_08012012_efffixed_stacoCB_all #eff scales fixed [actually, broken!]
input=/share/t3data3/antonk/ana/ana_v29G_08162012_MCP_max_stacoCB_all/ #near-convergence to max cutflow (different efficiency weight: mine more correct)

# RESURRECTING NTUPLE-BASED PLOTTING:
for q in 0 1 2; do
./stack2.py --input ${input} --qcd 1.0 -b --var "fabs(l_eta)" --bin 10,0.0,2.5 --hsource "lepton_absetav" -o TEST -t Q${q}_ebin$all --pre "ptiso40/l_pt<0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1 && l_trigEF<0.2" --cut "mcw*puw*effw*trigw*wptw" -m ALL --bgsig 5 --bgewk 5 --bgqcd 3 --charge ${q} &
for i in 9 10; do
    ./stack2.py --input ${input} --qcd 1.0 -b --var "fabs(l_eta)" --bin 10,0.0,2.5 --hsource "lepton_absetav" -o TEST -t Q${q}_ebin${i} --pre "ptiso40/l_pt<0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1 && l_trigEF<0.2" --cut "mcw*puw*effw*trigw*wptw" -m ALL --bgsig 5 --bgewk 5 --bgqcd 3 --charge ${q} --extra $i &
    #break;
done
done

wait




