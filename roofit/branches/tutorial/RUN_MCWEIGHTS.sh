#!/bin/bash

# This script runs the stack comparison portion of -m ALL using ntuple, but using different variations of MC weights
# This is to debug weight disagreement recently observed in lepton pT spectrum.

#input=/share/t3data3/antonk/ana/ana_v29G_08182012_max_wpt_wpol_stacoCB_all  # max cutflow + new boson pt & polarization reweighting
#input=/share/t3data3/antonk/ana/ana_v29G_08182012_max_wpt_wpol_newMCP_stacoCB_all   # same, plus my new MCP corrections

#input=/share/t3data3/antonk/ana/ana_v29G_08212012_max_wpt_wpol_2d_ls_stacoCB_all  # add wpt and pol. weight to ntuple
#input=/share/t3data3/antonk/ana/ana_v29G_08222012_max_wpt_wpol_2d_ls_hist_stacoCB_all   # all weights except lineshape applied
#input=/share/t3data3/antonk/ana/ana_v29G_08222012_max_wpt_wpol_2d_ls_hist_transpose_stacoCB_all  #includes the other version of 2d histo
#input=/share/t3data3/antonk/ana/ana_v29G_08242012_allwts_V29G_stacoCB_all  # ALL weights but vxz+ v29g ntuple + jerup/down
#input=/share/t3data3/antonk/ana/ana_v29G_08242012_allwts_V29I_stacoCB_all  # ALL weights + v29i ntuple + jerup/down 
#input=/share/t3data3/antonk/ana/ana_v29I_09082012_allwts_wptMC10_rensys_stacoCB_all # Renamed all systs. fixed isolation weight cutoff at 150 gev. added several new wpt weight systematic variations. 
input=/share/t3data3/antonk/ana/ana_v29I_09082012_allwts_wptPow8_eta_stacoCB_all #changed to powheg8 wpt reweighting. Fixed alpgen. Added eta (vs |eta|) histograms.

itot=0
for bgsig in 4 1; do
    for i in {0..4}; do
	for q in {0,1,2}; do 
	    ./smart_killer2.sh ./stack2.py --input ${input} -q ${q} -b --var "w_pt" --bin 200,0,200 --hsource "lepton_absetav" -o TEST -t Q${q}S${bgsig} --pre "ptiso40/l_pt<0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1" --cut "mcw*puw*wzptw*effw*isow*trigw" -m study_weights --bgsig ${bgsig} --bgewk 5 --bgqcd 3 --extra $i &> LOG.wpt.${itot}.s${bgsig}i${i}q${q} &
	    ./smart_killer2.sh ./stack2.py --input ${input} -q ${q} -b --var "l_pt" --bin 200,0,200 --hsource "lepton_absetav" -o TEST -t Q${q}S${bgsig} --pre "ptiso40/l_pt<0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1" --cut "mcw*puw*wzptw*effw*isow*trigw" -m study_weights --bgsig ${bgsig} --bgewk 5 --bgqcd 3 --extra $i &> LOG.lpt.${itot}.s${bgsig}i${i}q${q} &
	    ./smart_killer2.sh ./stack2.py --input ${input} -q ${q} -b --var "met" --bin 200,0,200 --hsource "lepton_absetav" -o TEST -t Q${q}S${bgsig} --pre "ptiso40/l_pt<0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1" --cut "mcw*puw*wzptw*effw*isow*trigw" -m study_weights --bgsig ${bgsig} --bgewk 5 --bgqcd 3 --extra $i &> LOG.met.${itot}.s${bgsig}i${i}q${q} &
	    ((itot++))
	done
	echo "Waiting for variation: ${bgsig} $i"
	wait
    done
done

# In borderline eta bin
for bgsig in 5; do
    for i in {0..4}; do
	for q in {0,1,2}; do 
	    continue
	    ./smart_killer2.sh ./stack2.py --input ${input} -q ${q} -b --var "w_pt" --bin 200,0,200 --hsource "lepton_absetav" -o TEST2 -t Q${q}S${bgsig} --pre "ptiso40/l_pt<0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1" --cut "mcw*puw*wzptw*effw*isow*trigw" -m study_weights --bgsig ${bgsig} --bgewk 5 --bgqcd 3 --extra $i --func ZEBRA &> LOG2.wpt.${itot}.s${bgsig}i${i}q${q} &
	    ./smart_killer2.sh ./stack2.py --input ${input} -q ${q} -b --var "l_pt" --bin 200,0,200 --hsource "lepton_absetav" -o TEST2 -t Q${q}S${bgsig} --pre "ptiso40/l_pt<0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1" --cut "mcw*puw*wzptw*effw*isow*trigw" -m study_weights --bgsig ${bgsig} --bgewk 5 --bgqcd 3 --extra $i --func ZEBRA &> LOG2.lpt.${itot}.s${bgsig}i${i}q${q} &
	    ./smart_killer2.sh ./stack2.py --input ${input} -q ${q} -b --var "met" --bin 200,0,200 --hsource "lepton_absetav" -o TEST2 -t Q${q}S${bgsig} --pre "ptiso40/l_pt<0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1" --cut "mcw*puw*wzptw*effw*isow*trigw" -m study_weights --bgsig ${bgsig} --bgewk 5 --bgqcd 3 --extra $i --func ZEBRA &> LOG2.met.${itot}.s${bgsig}i${i}q${q} &
	    ((itot++))
	done
	echo "Waiting for variation: ${bgsig} $i"
	wait
    done
done


echo "SUBMITTED ${itot} jobs..."
wait
echo "DONE"
