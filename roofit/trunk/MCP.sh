#!/bin/bash

# Restoring Z peak fits in stack2.py (vs zpeak.py)

input=/share/t3data3/antonk/ana/ana_v29I_11022012_unfold_stacoCB_MCPscale
input=/share/t3data3/antonk/ana/ana_v29I_11212012_edboard_nophi_stacoCB_MCPscale

#./zpeak.py -b --antondb out2011_v29I_11022012_unfold --rootdata "${input}/data*/root_data*.root" --rootmc "${input}/mc_pythia_zmumu/root_mc*.root" --func gaus0 --min 70 --max 110 --tt cmb --region U0U --tag r17_pythia_default_staco --ndata 200000 --nmc 200000 --ext eps

zpre='lP_pt>20.0 && fabs(lP_eta)<2.4 && lN_pt>20.0 && fabs(lN_eta)<2.4    &&    lP_idhits==1 && fabs(lP_z0)<10.   &&   lN_idhits==1 && fabs(lN_z0)<10.   &&   Z_m>70 && Z_m<110    &&    fabs(lP_phi-lN_phi)>0.0 && (lP_q*lN_q)<0 && nmuons==2'

# Z PLOTTER
refline=0.941,1.059
#./stack2.py --input ${input} -b --ntuple z -q -1 --var "lP_eta" --bin 10,0.0,2.5 --hsource "lepton_etav" -o TEST -t TEST --pre "${zpre}" --cut "mcw*puw*wzptw*vxw*ls1w*ls2w*effw*isow*trigw" -m ALL --bgsig 5 --bgewk 5 --bgqcd 0 --qcdscale 1.0 --refline 0.941,1.059
