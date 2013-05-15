#!/bin/bash

# A few plots showing various QCD templates

source config.sh

fiducial="x:0:-1:y:2:-1" # 25 GeV

./stack2.py -b --input ${input} --hsource "d3_abseta_lpt_met:${fiducial}" --bin 100,25,120 --rebin 2 --refline 0.85,1.175 -o METSHAPE --bgqcd ${bgqcd} --charge 3 -m metplots -t P
