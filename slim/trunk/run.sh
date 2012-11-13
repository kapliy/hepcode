#!/bin/bash

function run_old() {
    ./filter-and-merge-d3pd.py -i LIST -o out_v1_29l_old.root --tree tree --var vars.txt -m 2000 --grl "/home/antonk/TrigFTKAna/good_run_lists/asym_data11_7TeV.pro10.DtoM.xml" -s file:sel_data.py
}

function run_new() {
    ./slim.py -i LIST -o out_v1_29l.root --tree tree --var vars.txt --grl /home/antonk/TrigFTKAna/good_run_lists/asym_data11_7TeV.pro10.DtoM.xml --pt 19.99 --trigger
}
run_new &> LOG.run_new &
run_old &> LOG.run_old &
wait
