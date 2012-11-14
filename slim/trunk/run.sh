#!/bin/bash

function run_old() {
l    ./filter-and-merge-d3pd.py -i LIST -o out_v1_29l_old.root --tree tree --var vars_old.txt -m 2000 --grl "/home/antonk/TrigFTKAna/good_run_lists/asym_data11_7TeV.pro10.DtoM.xml" #-s file:sel_data.py
}

function run_new() {
    ./slim.py -i LIST -o out_v1_29l.root --tree tree --var vars.txt --grl /home/antonk/TrigFTKAna/good_run_lists/asym_data11_7TeV.pro10.DtoM.xml --trigger --pt 15.0 
}
function runmc_new() {
    ./slim.py -i LIST2 -o out_v1_29l_user.kapliy.UCNTUP.mc11_7TeV.108298.PowHegWplusmunuPythia.merge.AOD.e825_s1372_s1370_r3043_r2993.j.mc11c.v1_29i.120811072053_user.kapliy.010562.flatntuple._00001.root --tree tree --var vars.txt --grl /home/antonk/TrigFTKAna/good_run_lists/asym_data11_7TeV.pro10.DtoM.xml --trigger --pt 15.0 
}

runmc_new &> LOG.run_new &

wait
