#!/bin/bash

orig=/share/ftkdata1/antonk/ana_v28_1128_BtoM_stacoCB_all/

d0=/share/ftkdata1/antonk/ana_v28_1128_BtoM_stacoCB_noscale
l0="noscale"

d1=/share/ftkdata1/antonk/ana_v28_1128_BtoM_stacoCB_default
l1="oliver"

d2=/share/ftkdata1/antonk/ana_v28_1128_BtoM_stacoCB_scaleMC_KC
l2="KC"

function run() {
    out=$1
    tag=$2
    ./stack2.py --bgsig 10 -d B,D,E,F,G,H,I,J,K --lumi 2290000 --input ${out} --ntuple z -b --var Z_m --bin 50,70,110 -t ZJORDAN_${tag} -m1 --pre "lP_idhits==1 && fabs(lP_z0)<10. && lP_pt>20.0 && fabs(lP_eta)<2.4 && fabs(lP_pt_id-lP_pt_exms)/lP_pt_id<0.5 && lN_idhits==1 && fabs(lN_z0)<10. && lN_pt>20.0 && fabs(lN_eta)<2.4 && fabs(lN_pt_id-lN_pt_exms)/lN_pt_id<0.5 && Z_m>70 && Z_m<110 && fabs(lP_z0-lN_z0)<3 && fabs(lP_d0-lN_d0)<2 && fabs(lP_phi-lN_phi)>0.0 && (lP_q*lN_q)<0 && lP_ptiso20/lP_pt<0.1 && lN_ptiso20/lN_pt<0.1" --cut "mcw*puw" &
    ./stack2.py --bgsig 10 -d B,D,E,F,G,H,I,J,K --lumi 2290000 --input ${out} --ntuple z -b --var Z_m --bin 50,70,110 -t ZPETER_${tag} -m1 --pre "lP_idhits==1 && fabs(lP_z0)<10. && lP_pt>20.0 && fabs(lP_eta)<2.4 && fabs(lP_pt_id-lP_pt_exms)/lP_pt_id<0.5 && lN_idhits==1 && fabs(lN_z0)<10. && lN_pt>20.0 && fabs(lN_eta)<2.4 && fabs(lN_pt_id-lN_pt_exms)/lN_pt_id<0.5 && Z_m>70 && Z_m<110 && fabs(lP_z0-lN_z0)<3 && fabs(lP_d0-lN_d0)<2 && fabs(lP_phi-lN_phi)>0.0 && (lP_q*lN_q)<0 && lP_ptiso40<2.0 && lP_etiso40<2.0 && lN_ptiso40<2.0 && lN_etiso40<2.0" --cut "mcw*puw" &
    ./stack2.py --bgsig 10 -d B,D,E,F,G,H,I,J,K --lumi 2290000 --input ${out} --ntuple z -b --var Z_m --bin 50,70,110 -t ZNOISO_${tag} -m1 --pre "lP_idhits==1 && fabs(lP_z0)<10. && lP_pt>20.0 && fabs(lP_eta)<2.4 && fabs(lP_pt_id-lP_pt_exms)/lP_pt_id<0.5 && lN_idhits==1 && fabs(lN_z0)<10. && lN_pt>20.0 && fabs(lN_eta)<2.4 && fabs(lN_pt_id-lN_pt_exms)/lN_pt_id<0.5 && Z_m>70 && Z_m<110 && fabs(lP_z0-lN_z0)<3 && fabs(lP_d0-lN_d0)<2 && fabs(lP_phi-lN_phi)>0.0 && (lP_q*lN_q)<0" --cut "mcw*puw" &
    wait
}

run ${d0} ${l0}
run ${d1} ${l1}
run ${d2} ${l2}
