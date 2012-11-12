#!/bin/bash

function run_sig() {
    ./filter-and-merge-d3pd.py -i LIST -o out_v1_29l.root --tree tree --var vars.txt -m 2000
}

function run_mc() {
    ./filter-and-merge-d3pd.py -i LIST -o out_v1_29l.root --tree tree --var vars.txt -m 2000 -s file:sel_mc.py 
}

function run_data() {
    ./filter-and-merge-d3pd.py -i LIST -o out_v1_29l.root --tree tree --var vars.txt -m 2000 -s file:sel_data.py --grl "/home/antonk/TrigFTKAna/good_run_lists/asym_data11_7TeV.pro10.DtoM.xml"
}

run_data
