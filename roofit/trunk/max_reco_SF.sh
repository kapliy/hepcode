#!/bin/bash

# June 2013: investigating new Eta x Phi reco scale factors (compared to old Eta-only scale factors!)

source config.sh

input1=/share/t3data3/antonk/ana/ana_v29I_06192013_recoBOTH_withZ_stacoCB_all/pt20  # Eta-only as nominal
input2=/share/t3data3/antonk/ana/ana_v29I_06192013_recoBOTH_withZ_recoPhi_stacoCB_all/pt20  # Eta-phi

if [ 1 -eq 1 ]; then
    ./stack2.py -q 0 -o CRAP --rebin 2 --hsource d3_abseta_lpt_phi:y:2:-1:x:0:-1 --bin 10,-3.15,3.15 --refline 0.85,1.175 -b --input ${input1} -m control_stack --bgqcd 0 --qcdscale 1.0 --lnofits --nomonly --subdirmc st_w_final -t WP_old &
    ./stack2.py -q 0 -o CRAP --rebin 2 --hsource d3_abseta_lpt_phi:y:2:-1:x:0:-1 --bin 10,-3.15,3.15 --refline 0.85,1.175 -b --input ${input2} -m control_stack --bgqcd 0 --qcdscale 1.0 --lnofits --nomonly --subdirmc st_w_final -t WP_new &
    ./stack2.py -q 1 -o CRAP --rebin 2 --hsource d3_abseta_lpt_phi:y:2:-1:x:0:-1 --bin 10,-3.15,3.15 --refline 0.85,1.175 -b --input ${input1} -m control_stack --bgqcd 0 --qcdscale 1.0 --lnofits --nomonly --subdirmc st_w_final -t WN_old &
    ./stack2.py -q 1 -o CRAP --rebin 2 --hsource d3_abseta_lpt_phi:y:2:-1:x:0:-1 --bin 10,-3.15,3.15 --refline 0.85,1.175 -b --input ${input2} -m control_stack --bgqcd 0 --qcdscale 1.0 --lnofits --nomonly --subdirmc st_w_final -t WN_new &

fi

# let's try to plot Z
if [ 1 -eq 1 ]; then
./stack2.py -q -1 --ntuple z -o CRAP --rebin 2 --var "lP_eta" --bin 10,-3.15,3.15 --hsource "leptonP_phi" --refline 0.85,1.175 -b --input ${input1} -m control_stack --bgqcd 0 --qcdscale 1.0 --lnofits --nomonly -t ZP_old &
./stack2.py -q -1 --ntuple z -o CRAP --rebin 2 --var "lP_eta" --bin 10,-3.15,3.15 --hsource "leptonN_phi" --refline 0.85,1.175 -b --input ${input1} -m control_stack --bgqcd 0 --qcdscale 1.0 --lnofits --nomonly -t ZN_old &
./stack2.py -q -1 --ntuple z -o CRAP --rebin 2 --var "lP_eta" --bin 10,-3.15,3.15 --hsource "lepton_phi" --refline 0.85,1.175 -b --input ${input1} -m control_stack --bgqcd 0 --qcdscale 1.0 --lnofits --nomonly -t Z_old &

./stack2.py -q -1 --ntuple z -o CRAP --rebin 2 --var "lP_eta" --bin 10,-3.15,3.15 --hsource "leptonP_phi" --refline 0.85,1.175 -b --input ${input2} -m control_stack --bgqcd 0 --qcdscale 1.0 --lnofits --nomonly -t ZP_new &
./stack2.py -q -1 --ntuple z -o CRAP --rebin 2 --var "lP_eta" --bin 10,-3.15,3.15 --hsource "leptonN_phi" --refline 0.85,1.175 -b --input ${input2} -m control_stack --bgqcd 0 --qcdscale 1.0 --lnofits --nomonly -t ZN_new &
./stack2.py -q -1 --ntuple z -o CRAP --rebin 2 --var "lP_eta" --bin 10,-3.15,3.15 --hsource "lepton_phi" --refline 0.85,1.175 -b --input ${input2} -m control_stack --bgqcd 0 --qcdscale 1.0 --lnofits --nomonly -t Z_new &
fi

echo "Waiting for `jobs | wc -l` jobs..."
wait

