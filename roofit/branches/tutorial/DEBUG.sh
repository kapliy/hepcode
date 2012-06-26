#!/bin/bash

input=/share/t3data3/antonk/ana/ana_v29D_04292012_DtoM_unfold_stacoCB_all
input=/share/t3data3/antonk/ana/ana_v29D_06092012_DtoM_prejes_stacoCB_all        # new systematics
input=/share/t3data3/antonk/ana/ana_v29D_06162012_DtoM_maxiso_wptw_stacoCB_all   # new tight iso per Max
input=/share/t3data3/antonk/ana/ana_v29D_06252012_DtoM_maxiso_msmetfx_jetordr_stacoCB_all  # fixed jet met recomp (orijets), propagate MCP smear/scale to met, ALL NEW classes - charge-dependent efficiency, new JES/JER etc

# RESURRECTING NTUPLE-BASED PLOTTING:

if [ "1" == "1" ]; then
    ./stack2.py --input ${input} --qcd 1.0 -b --var "fabs(l_eta)" --bin 10,0.0,2.5 --hsource "lepton_absetav" --extra 1 -o TEST_LPT -t TEST --pre "ptiso20/l_pt<0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1 && l_trigEF<0.2" --cut "mcw*puw*effw*trigw" -m ALL --bgsig 2 --bgewk 2 --bgqcd 3 --metallsys &> LOG.TEST.1 &
    ./stack2.py --input ${input} --qcd 1.0 -b --var "fabs(l_eta)" --bin 10,0.0,2.5 --hsource "lepton_absetav" --extra 2 -o TEST_WPT -t TEST --pre "ptiso20/l_pt<0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1 && l_trigEF<0.2" --cut "mcw*puw*effw*trigw" -m ALL --bgsig 2 --bgewk 2 --bgqcd 3 --metallsys &> LOG.TEST.2 &
    ./stack2.py --input ${input} --qcd 1.0 -b --var "fabs(l_eta)" --bin 10,0.0,2.5 --hsource "lepton_absetav" --extra 3 -o TEST_WMT -t TEST --pre "ptiso20/l_pt<0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1 && l_trigEF<0.2" --cut "mcw*puw*effw*trigw" -m ALL --bgsig 2 --bgewk 2 --bgqcd 3 --metallsys &> LOG.TEST.3 &
    ./stack2.py --input ${input} --qcd 1.0 -b --var "fabs(l_eta)" --bin 10,0.0,2.5 --hsource "lepton_absetav" --extra 4 -o TEST_MET -t TEST --pre "ptiso20/l_pt<0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1 && l_trigEF<0.2" --cut "mcw*puw*effw*trigw" -m ALL --bgsig 2 --bgewk 2 --bgqcd 3 --metallsys &> LOG.TEST.4 &
    ./stack2.py --input ${input} --qcd 1.0 -b --var "fabs(l_eta)" --bin 10,0.0,2.5 --hsource "lepton_absetav" --extra 5 -o TEST_NJETS -t TEST --pre "ptiso20/l_pt<0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1 && l_trigEF<0.2" --cut "mcw*puw*effw*trigw" -m ALL --bgsig 2 --bgewk 2 --bgqcd 3 --metallsys &> LOG.TEST.5 &
fi

echo "Waiting for these jobs to finish:"
jobs
wait
echo ".............."
echo DONE

#ANTONIO
#./stack2.py --input ${input} --qcd 1.0 -b --var "fabs(l_eta)" --bin 10,0.0,2.5 --hsource "lepton_absetav" -o TEST -t TEST --pre "ptiso40/l_pt<0.1 && etiso30corr/l_pt<0.1 && met>25.0 && l_pt>20.0 && l_pt<30.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1 && l_trigEF<0.2" --cut "mcw*puw*effw*trigw" -m ALL --bgsig 2 --bgqcd 0 --refline 0.0,3.0 #--metallsys #d0sig was 10, then was fabs(d0sig)<5.0

# USED FOR MOST MAY STUDIES:
#./stack2.py --input ${input}/ -o TEST -b --hsource "lepton_absetav" -t TEST -m ALL --bgqcd 2 --bgsig 2 #--metallsys


# QCD matrix method
#./stack2.py --input /share/ftkdata1/antonk/ana_v28HC_02092012_DtoM_cernupd_stacoCB_all -o TEST -b -m matrix_2010inc --var l_eta --bin 20,-2.5,2.5 -t QCD --pre 'ptiso20/l_pt<0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && fabs(d0sig)<10.0 && nmuons==1 && l_trigEF<0.2' --cut mcw*puw --charge 0
