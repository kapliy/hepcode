#!/bin/bash

./filter-and-merge-d3pd.py -i LIST -o out.root --tree tree --var vars.txt -m 2000 -s file:sel_mc.py 
#./filter-and-merge-d3pd.py -i LIST -o out.root --tree tree --var vars.txt -m 2000 -s file:sel_data.py --grl "/home/antonk/TrigFTKAna/good_run_lists/asym_data11_7TeV.pro10.DtoM.xml"
