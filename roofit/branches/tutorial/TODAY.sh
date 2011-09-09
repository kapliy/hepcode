#!/bin/bash

#./stack2.py -b -t TBNEW --var 'lY_eta' --bin '50,-2.5,2.5' -m101 --ntuple z
wpre_jordan='l_pt>20.0 && ptiso20/l_pt<0.1 && met>25.0 && w_mt>40.0 && idhits==1 && fabs(z0)<10. && fabs(d0sig)<10. && fabs(l_pt_id-l_pt_exms)/l_pt_id<0.5'
wpre_peter='l_pt>20.0 && ptiso40<2.0 && etiso40<2.0 && met>25.0 && w_mt>40.0 && idhits==1 && fabs(z0)<10. && fabs(d0sig)<10. && fabs(l_pt_id-l_pt_exms)/l_pt_id<0.5'
common="--input /share/ftkdata1/antonk/ana_v26_0813_all_stacoCB_fixmetcln/"
common="--input /share/ftkdata1/antonk/ana_v26_0830_all_stacoCB_fixmetcln/"

# W stack histos
function run_w_stacks () {
    ./stack2.py ${common} -b -m1 --var 'l_pt' --bin '50,0,100' -t ${tag} --pre "${pre}" --cut "${cut}" &
    ./stack2.py ${common} -b -m1 --var 'met' --bin '50,0,200' -t ${tag} --pre "${pre}" --cut "${cut}" &
    ./stack2.py ${common} -b -m1 --var 'w_mt' --bin '50,0,200' -t ${tag} --pre "${pre}" --cut "${cut}" &
    ./stack2.py ${common} -b -m1 --var 'l_eta' --bin '50,-2.5,2.5' -t ${tag} --pre "${pre}" --cut "${cut}" &
    ./stack2.py ${common} -b -m1 --var 'd0sig' --bin '100,-10.0,10.0' -t ${tag} --pre "${pre}" --cut "${cut}" &
    ./stack2.py ${common} -b -m1 --var 'd0' --bin '100,-5.0,5.0' -t ${tag} --pre "${pre}" --cut "${cut}" &
    ./stack2.py ${common} -b -m1 --var 'z0' --bin '100,-10.0,10.0' -t ${tag} --pre "${pre}" --cut "${cut}" &
}
if [ "1" -eq "1" ]; then
    pre="${wpre_jordan}"

    cut="mcw*puw"
    tag=DEF
    run_w_stacks
    wait
    
    cut="mcw*puw*effw"
    tag=EFFW
    run_w_stacks
    wait

    cut="mcw*puw*effw*trigw"
    tag=EFFTRIGW
    run_w_stacks
    wait
    
    echo DONE
fi

# W QCD FITS
if [ "0" -eq "1" ]; then
    #./stack2.py -b -m99 --var 'met' --bin '50,25,100' --hsource WJ/st_w_final/00_wmt/met --rebin 4
    tag=qcdfit_jordan20
    ./stack2.py ${common} -b -m99 --var 'met' --bin '100,5,100' -t ${tag} --pre ${pre_jordan} &
    tag=qcdfit_peter20
    ./stack2.py ${common} -b -m99 --var 'met' --bin '100,5,100' -t ${tag} --pre ${pre_peter} &
    wait
    echo DONE
fi

# Z stack histos
if [ "0" -eq "1" ]; then
    tag=zplots
    ./stack2.py -m1 --ntuple z -b --var 'Z_m' --bin '100,60,120' -t ${tag}_zm --pre '(lP_idhits==1 && fabs(lP_z0)<10. && lP_pt>20.0 && lN_idhits==1 && fabs(lN_z0)<10. && lN_pt>20.0) && Z_m<150.0 && (lP_q*lN_q)<0 && lP_ptiso40<2.0 && lP_etiso40<2.0 && lN_ptiso40<2.0 && lN_etiso40<2.0'
    wait
    echo DONE
fi

# Z tag and probe
if [ "0" -eq "1" ]; then
    tag=etiso40_sub
    #tag=mcphits
    ./stack2.py -m102 --ntuple z -b --var 'lY_phi' --bin '30,-3.14,3.14' -t ${tag}_phi &
    ./stack2.py -m102 --ntuple z -b --var 'lY_pt'  --bin '30,20,140' -t ${tag}_pt &
    ./stack2.py -m102 --ntuple z -b --var 'lY_eta' --bin '30,-2.5,2.5' -t ${tag}_eta &
    wait
    echo DONE
fi

