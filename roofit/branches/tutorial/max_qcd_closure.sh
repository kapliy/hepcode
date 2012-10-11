#!/bin/bash

# a script to run a simple closure test on bb/cc QCD Monte-Carlo:
# whether inverting isolation biases MET

#input=/share/t3data3/antonk/ana/ana_v29I_09082012_allwts_wptPow8_eta_stacoCB_all #changed to powheg8 wpt reweighting. Fixed alpgen. Added eta (vs |eta|) histograms.
input=/share/t3data3/antonk/ana/ana_v29I_09082012_allwts_wptPow8_eta_newSF_stacoCB_all # added new reco-level scale factors in measurement bins
input=/share/t3data3/antonk/ana/ana_v29I_09282012_allwts_wptPow8_eta_newSFTFQ_stacoCB_all  # added charge-dependent trigger SF from Max

# MAIN PLOTTER
./stack2.py --input ${input} -b --var "met" --bin 100,0,120 --hsource "lepton_NOTabsetav" -o TEST -t TEST --pre "ptiso40/l_pt<0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1" --cut "mcw*puw*wzptw*wpolw*vxw*ls1w*ls2w*effw*isow*trigw" -m qcd_isolation --bgsig 5 --bgewk 5 --bgqcd 0

#./stack2.py --input ${input} -b --var "ptiso40/l_pt" --bin 200,0,10.0 --hsource "lepton_NOTabsetav" -o TEST -t TEST --pre "ptiso40/l_pt<0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1" --cut "mcw*puw*wzptw*wpolw*vxw*ls1w*ls2w*effw*isow*trigw" -m qcd_isolation --bgsig 5 --bgewk 5 --bgqcd 0
