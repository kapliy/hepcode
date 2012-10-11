#!/bin/bash

# a script to study reco-level histograms vs eta

#input=/share/t3data3/antonk/ana/ana_v29I_09082012_allwts_wptMC10_rensys_stacoCB_all # Renamed all systs. fixed isolation weight cutoff at 150 gev. added several new wpt weight systematic variations. 
#input=/share/t3data3/antonk/ana/ana_v29I_09082012_allwts_wptPow8_eta_stacoCB_all #changed to powheg8 wpt reweighting. Fixed alpgen. Added eta (vs |eta|) histograms.
input=/share/t3data3/antonk/ana/ana_v29I_09082012_allwts_wptPow8_eta_newSF_stacoCB_all # added new reco-level scale factors in measurement bins
input=/share/t3data3/antonk/ana/ana_v29I_09282012_allwts_wptPow8_eta_newSFTFQ_stacoCB_all  # added charge-dependent trigger SF from Max
input=/share/t3data3/antonk/ana/ana_v29I_1002012_allwts_wptPow8_eta_newSFTF_ETAPTQ_stacoCB_all   # eta x pt trigger SF from Max
#input=/share/t3data3/antonk/ana/ana_v29I_1002012_allwts_wptPow8_eta_newSFTF_ETAPTQ_FINEMCPSCALE_stacoCB_all # same, but using super-fine MCP scale

input=/share/t3data3/antonk/ana/ana_v29I_1009012_allwts_wptPow8_eta_newSFTF_ETAPTQ_stacoCB_all/  #first time re-plotting after changing unfolding structure

# MAIN PLOTTER
./stack2.py --refline 0.85,1.15 --input ${input} -b --hsource "lepton_etav" -o TEST -t EWK5 -m one_plot --bgsig 5 --bgewk 5 --bgqcd 0 &> LOG.ETA.${i} &
((i++))

# NTUPLE PLOTTER
./stack2.py --refline 0.85,1.15 --input ${input} -b --var "l_eta" --bin 100,-2.5,2.5 --hsource "lepton_etav" --cut "mcw*puw*wzptw*wpolw*vxw*ls1w*ls2w*effw*isow*trigw" -o TEST -t NT -m one_plot_nt --bgsig 5 --bgewk 5 --bgqcd 0 &> LOG.ETA.${i} &
((i++))
./stack2.py --refline 0.85,1.15 --input ${input} -b --var "l_phi" --hsource "l_phi" --bin 30,-3.15,3.15 --cut "mcw*puw*wzptw*wpolw*vxw*ls1w*ls2w*effw*isow*trigw" -o TEST -t NT -m one_plot_nt --bgsig 5 --bgewk 5 --bgqcd 0 &> LOG.ETA.${i} &
((i++))
./stack2.py --refline 0.85,1.15 --input ${input} -b --var "l_pt" --hsource "l_pt" --bin 40,0,200 --cut "mcw*puw*wzptw*wpolw*vxw*ls1w*ls2w*effw*isow*trigw" -o TEST -t NT -m one_plot_nt --bgsig 5 --bgewk 5 --bgqcd 0 &> LOG.ETA.${i} &
((i++))


# NTUPLE PLOTTER: STUDYING A BAD BIN
./stack2.py --refline 0.85,1.15 --input ${input} -b --var "l_eta" --bin 100,-2.5,2.5 --hsource "lepton_etav" --cut "mcw*puw*wzptw*wpolw*vxw*ls1w*ls2w*effw*isow*trigw" -o TEST -t NT_E -m one_plot_nt --bgsig 5 --bgewk 5 --bgqcd 0 --pre "ptiso40/l_pt<0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1 && l_eta>=-2.18 && l_eta<-1.95" &> LOG.ETA.${i} &
((i++))
./stack2.py --refline 0.85,1.15 --input ${input} -b --var "l_phi" --hsource "l_phi" --bin 30,-3.15,3.15 --cut "mcw*puw*wzptw*wpolw*vxw*ls1w*ls2w*effw*isow*trigw" -o TEST -t NT_E -m one_plot_nt --bgsig 5 --bgewk 5 --bgqcd 0 --pre "ptiso40/l_pt<0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1 && l_eta>=-2.18 && l_eta<-1.95" &> LOG.ETA.${i} &
((i++))
./stack2.py --refline 0.85,1.15 --input ${input} -b --var "l_pt" --hsource "l_pt" --bin 40,0,200 --cut "mcw*puw*wzptw*wpolw*vxw*ls1w*ls2w*effw*isow*trigw" -o TEST -t NT_E -m one_plot_nt --bgsig 5 --bgewk 5 --bgqcd 0 --pre "ptiso40/l_pt<0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1 && l_eta>=-2.18 && l_eta<-1.95" &> LOG.ETA.${i} &
((i++))


wait
echo DONE
