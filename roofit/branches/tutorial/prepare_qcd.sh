#!/bin/bash

# Saved qcd histograms for Max

#input=/share/t3data3/antonk/ana/ana_v29G_08182012_max_wpt_wpol_stacoCB_all  # max cutflow + new boson pt & polarization reweighting
#input=/share/t3data3/antonk/ana/ana_v29G_08182012_max_wpt_wpol_newMCP_stacoCB_all   # same, plus my new MCP corrections

#input=/share/t3data3/antonk/ana/ana_v29G_08242012_allwts_V29G_stacoCB_all  # ALL weights but vxz+ v29g ntuple + jerup/down
#input=/share/t3data3/antonk/ana/ana_v29G_08242012_nowts_V29I_stacoCB_all  # NO weights
#input=/share/t3data3/antonk/ana/ana_v29G_08242012_allwts_V29I_stacoCB_all  # ALL weights + v29i ntuple + jerup/down
#input=/share/t3data3/antonk/ana/ana_v29I_08242012_allwts_wptMC10_stacoCB_all   # ALL weights but using wptMC10 reweighting
#input=/share/t3data3/antonk/ana/ana_v29I_09082012_allwts_wptPow8_eta_stacoCB_all #changed to powheg8 wpt reweighting. Fixed alpgen. Added eta (vs |eta|) histograms.
#input=/share/t3data3/antonk/ana/ana_v29I_09082012_allwts_wptPow8_eta_newSF_stacoCB_all # added new reco-level scale factors in measurement bins
#input=/share/t3data3/antonk/ana/ana_v29I_09272012_allwts_wptPow8_eta_newSFTF_stacoCB_all # added trigger SF, too (from Max)
input=/share/t3data3/antonk/ana/ana_v29I_09282012_allwts_wptPow8_eta_newSFTFQ_stacoCB_all  # added charge-dependent trigger SF from Max
input=/share/t3data3/antonk/ana/ana_v29I_1002012_allwts_wptPow8_eta_newSFTFQ_stacoCB_all #fixed

input=/share/t3data3/antonk/ana/ana_v29I_10212012_unfold_stacoCB_all  # renamed lepton_pt for consistency
input=/share/t3data3/antonk/ana/ana_v29I_10292012_unfold_newTrigWZPTnoPol_stacoCB_all # new trigger SF


bgqcd=3
name="IN_10122012_ALL.v1"

#bgqcd=4
#name="IN_09282012.newSFTFQ_QCD4.v1"

./stack2.py -m prepare_qcd_2d --input ${input} -b --var "fabs(l_eta)" --bin 10,0.0,2.5 --hsource "lepton_absetav" -o TEST -t TEST --pre "ptiso40/l_pt<0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1" --cut "mcw*puw*wptw*effw*isow*trigw" --bgsig 5 --bgewk 5 --bgqcd ${bgqcd} --extra "${name}.abseta.2D.root" &> LOG.PREP.1 &
./stack2.py -m prepare_qcd_1d --input ${input} -b --var "fabs(l_eta)" --bin 10,0.0,2.5 --hsource "lepton_absetav" -o TEST -t TEST --pre "ptiso40/l_pt<0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1" --cut "mcw*puw*wptw*effw*isow*trigw" --bgsig 5 --bgewk 5 --bgqcd ${bgqcd} --extra "${name}.abseta.1D.root" &> LOG.PREP.2 &

./stack2.py -m prepare_qcd_2d --input ${input} -b --var "fabs(l_eta)" --bin 10,0.0,2.5 --hsource "lepton_absetav" -o TEST -t TEST --pre "ptiso40/l_pt<0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1" --cut "mcw*puw*wptw*effw*isow*trigw" --bgsig 5 --bgewk 5 --bgqcd ${bgqcd} --extra "${name}.eta.2D.root" --etamode 1 &> LOG.PREP.3 &
./stack2.py -m prepare_qcd_1d --input ${input} -b --var "fabs(l_eta)" --bin 10,0.0,2.5 --hsource "lepton_absetav" -o TEST -t TEST --pre "ptiso40/l_pt<0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1" --cut "mcw*puw*wptw*effw*isow*trigw" --bgsig 5 --bgewk 5 --bgqcd ${bgqcd}  --extra "${name}.eta.1D.root" --etamode 1 &> LOG.PREP.4 &

echo "Please wait..."
wait
echo "DONE"
