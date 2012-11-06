#!/bin/bash

# Studying the effect of background-subtraction from data-driven QCD template

#input=/share/t3data3/antonk/ana/ana_v29I_09272012_allwts_wptPow8_eta_newSFTF_stacoCB_all # added trigger SF, too (from Max)
input=/share/t3data3/antonk/ana/ana_v29I_09282012_allwts_wptPow8_eta_newSFTFQ_stacoCB_all  # added charge-dependent trigger SF from Max BUGGY
input=/share/t3data3/antonk/ana/ana_v29I_1002012_allwts_wptPow8_eta_newSFTFQ_stacoCB_all #fixed
input=/share/t3data3/antonk/ana/ana_v29I_1009012_allwts_wptPow8_eta_newSFTF_ETAPTQ_stacoCB_all/  #first time re-plotting after changing unfolding structure

# MAIN PLOTTER
./stack2.py -q 0 --refline 0.85,1.15 --input ${input} -b --var "fabs(l_eta)" --bin 10,0.0,2.5 --hsource "lepton_etav" -o TEST -t TEST -m qcd_bgsub --bgsig 5 --bgewk 5 --bgqcd 3 #--metallsys
