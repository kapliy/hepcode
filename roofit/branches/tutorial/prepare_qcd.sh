#!/bin/bash

# Saved qcd histograms for Max
input=/share/t3data3/antonk/ana/ana_v29I_10212012_unfold_stacoCB_all  # renamed lepton_pt for consistency
input=/share/t3data3/antonk/ana/ana_v29I_10292012_unfold_newTrigWZPTnoPol_stacoCB_all # new trigger SF
input=/share/t3data3/antonk/ana/ana_v29I_11022012_unfold_stacoCB_all # adding met muon term plots and additional 2d histos. adding pt slices. adding phi plot.
input=/share/t3data3/antonk/ana/ana_v29I_11072012_unfold_stacoCB_all/

input=/share/t3data3/antonk/ana/ana_v29I_11282012_edboard_nophi_stacoCB_all/
input=/share/t3data3/antonk/ana/ana_v29I_11302012_edboard_stacoCB_all

bgqcd=3
name="IN_11282012_ALL.v1"

bgqcd=4
name="IN_11282012_ALL.v2"

./stack2.py -m prepare_qcd_2d --input ${input} -b --var "fabs(l_eta)" --bin 10,0.0,2.5 --hsource "lepton_absetav" -o TEST -t TEST --pre "ptiso40/l_pt<0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1" --cut "mcw*puw*wptw*effw*isow*trigw" --bgsig 5 --bgewk 5 --bgqcd ${bgqcd} --extra "${name}.abseta.2D.root" &> LOG.PREP.1 &
./stack2.py -m prepare_qcd_1d --input ${input} -b --var "fabs(l_eta)" --bin 10,0.0,2.5 --hsource "lepton_absetav" -o TEST -t TEST --pre "ptiso40/l_pt<0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1" --cut "mcw*puw*wptw*effw*isow*trigw" --bgsig 5 --bgewk 5 --bgqcd ${bgqcd} --extra "${name}.abseta.1D.root" &> LOG.PREP.2 &

./stack2.py -m prepare_qcd_2d --input ${input} -b --var "fabs(l_eta)" --bin 10,0.0,2.5 --hsource "lepton_absetav" -o TEST -t TEST --pre "ptiso40/l_pt<0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1" --cut "mcw*puw*wptw*effw*isow*trigw" --bgsig 5 --bgewk 5 --bgqcd ${bgqcd} --extra "${name}.eta.2D.root" --etamode 1 &> LOG.PREP.3 &
./stack2.py -m prepare_qcd_1d --input ${input} -b --var "fabs(l_eta)" --bin 10,0.0,2.5 --hsource "lepton_absetav" -o TEST -t TEST --pre "ptiso40/l_pt<0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1" --cut "mcw*puw*wptw*effw*isow*trigw" --bgsig 5 --bgewk 5 --bgqcd ${bgqcd}  --extra "${name}.eta.1D.root" --etamode 1 &> LOG.PREP.4 &

echo "Please wait..."
wait
echo "DONE"
