#!/bin/bash

# MUID vs MG for Joao, particularly to study pT dependence

# grep SuCanvas.py and stack2.py for JOAO - currently parts saving root files are commented out

function make_pair() {
    tag=$1
    din=$2
    xtra=" --antondb JOAO/all.db"
    xtra=" --nomonly"  # for absolute normalization
    xtra=""

    ./stack2.py ${xtra} -o JOAO --hsource lpt --bin 100,25,120 --rebin 2 --refline 0.85,1.175 -b --input ${din} --lvar d3_abseta_lpt_met:x:0:-1:y:2:-1 --lbin 100,0,60 --lrebin 2 -t ${tag} -m control_stack --charge 3 --bgqcd 4 --lnofits &
    ./stack2.py ${xtra} -o JOAO --hsource d3_eta_lpt_met:y:2:-1:z:0:-1 --bin 10,-2.5,2.5 --refline 0.85,1.175 -b --input ${din} --lvar d3_abseta_lpt_met:x:0:-1:y:2:-1 --lbin 100,0,60 --lrebin 2 -t ${tag} -m control_stack --charge 3 --bgqcd 4 --lnofits &
}

#make_pair STAT /share/t3data3/antonk/ana/ana_v29I_06252013_newrecoSF_rep_stacoCB_all/pt20

make_pair MUID /share/t3data3/antonk/ana/ana_v29I_06252013_newrecoSF_rep_stacoCB_all/pt20
make_pair MUMG /share/t3data3/antonk/ana/ana_v29I_06252013_newrecoSF_MG_stacoCB_all/pt20

wait
echo DONE
