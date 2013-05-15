#!/bin/bash

#input=/share/t3data3/antonk/ana/ana_v29G_08242012_allwts_V29G_stacoCB_all  # ALL weights but vxz+ v29g ntuple + jerup/down
#input=/share/t3data3/antonk/ana/ana_v29G_08242012_nowts_V29I_stacoCB_all  # NO weights
#input=/share/t3data3/antonk/ana/ana_v29G_08242012_allwts_V29I_stacoCB_all  # ALL weights + v29i ntuple + jerup/down
input=/share/t3data3/antonk/ana/ana_v29I_08242012_allwts_wptMC10_stacoCB_all   # ALL weights but using wptMC10 reweighting

# ABCD tester: does it close in bb/cc monte-carlo?
ieta=0
ipt=0
./stack2.py -m abcd_mc --input ${input} --charge 0 --preNN ${ieta} --preNQ ${ipt} -b --var "l_pt" --bin 100,0,200 -o TEST -t TEST --pre "ptiso40/l_pt<0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1" --cut "mcw*puw" --bgsig 5 --bgewk 5 --bgqcd 1
