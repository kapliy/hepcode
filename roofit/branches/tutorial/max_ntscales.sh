#!/bin/bash

# a script to do closure tests on bb/cc QCD Monte-Carlo. E.g., whether inverting isolation biases MET.
# This version evaluates the effect of reverted-isolation bb/cc QCD on QCD fraction.

#input=/share/t3data3/antonk/ana/ana_v29I_09082012_allwts_wptPow8_eta_stacoCB_all #changed to powheg8 wpt reweighting. Fixed alpgen. Added eta (vs |eta|) histograms.
input=/share/t3data3/antonk/ana/ana_v29I_09082012_allwts_wptPow8_eta_newSF_stacoCB_all # added new reco-level scale factors in measurement bins

# MAIN PLOTTER
if [ "1" == "1" ]; then
    ./stack2.py --input ${input} -b --var "met" --bin 100,0,120 --hsource "lepton_NOTabsetav" -o TEST -t QCDSIG --pre "ptiso40/l_pt<0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1" --preNN 'ptiso40/l_pt<0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1' --preNQ 'ptiso40/l_pt<0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1' --preFN 'ptiso40/l_pt<0.1 && met>0.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1' --preFQ 'ptiso40/l_pt<0.1 && met>0.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1'  --cut "mcw*puw*wzptw*wpolw*vxw*ls1w*ls2w*effw*isow*trigw" -m study_weights --bgsig 5 --bgewk 5 --bgqcd 10 --extra 0
    
    ./stack2.py --input ${input} -b --var "met" --bin 100,0,120 --hsource "lepton_NOTabsetav" -o TEST -t QCDCTL --pre "ptiso40/l_pt<0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1" --preNN 'ptiso40/l_pt<0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1' --preNQ 'ptiso20/l_pt>0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons<2' --preFN 'ptiso40/l_pt<0.1 && met>0.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1' --preFQ 'ptiso20/l_pt>0.1 && met>0.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons<2'  --cut "mcw*puw*wzptw*wpolw*vxw*ls1w*ls2w*effw*isow*trigw" -m study_weights --bgsig 5 --bgewk 5 --bgqcd 10 --extra 0
fi

