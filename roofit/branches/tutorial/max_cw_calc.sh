#!/bin/bash

# Computes a C_W scale factor for a given cut, using the unfolding TTree

zdir=/share/t3data3/antonk/ana/ana_v29I_1009012_allwts_wptPow8_eta_newSFTF_ETAPTQ_stacoCB_all  # default
zdir=/share/t3data3/antonk/ana/ana_v29I_1011012_PYtoHW_nowzptw_stacoCB_all # no wzptw
./max_cw_calc.py "${zdir}/mc_powheg_herwig_wminmunu/*.unfold.root" &
./max_cw_calc.py "${zdir}/mc_powheg_pythia_wminmunu/*.unfold.root" &
./max_cw_calc.py "${zdir}/mc_powheg_pythia_wminmunu_toherwig1dfine/*.unfold.root" &
./max_cw_calc.py "${zdir}/mc_powheg_pythia_wminmunu_toherwig2dfine/*.unfold.root" &
./max_cw_calc.py "${zdir}/mc_powheg_pythia_wminmunu_toherwig1dcoarse/*.unfold.root" &
./max_cw_calc.py "${zdir}/mc_powheg_pythia_wminmunu_toherwig2dcoarse/*.unfold.root" &

wait
