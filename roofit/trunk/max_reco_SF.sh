#!/bin/bash

# June 2013: investigating new Eta x Phi reco scale factors (compared to old Eta-only scale factors!)
# Also adding: trigger phi corrections
source config.sh
rebin=15
rebin=2

input1=/share/t3data3/antonk/ana/ana_v29I_06192013_recoBOTH_withZ_stacoCB_all/pt20  # Eta-only as nominal
input2=/share/t3data3/antonk/ana/ana_v29I_06192013_recoBOTH_withZ_recoPhi_stacoCB_all/pt20  # Eta-phi

input1=/share/t3data3/antonk/ana/ana_v29I_06192013_recoBOTH_withZ_anyfit_stacoCB_all/pt20  # nominal: eta x phi; SFphi = eta (buggy)

# naming convention: FF,FE etc. First flag: reco (F=phi corrected, E=only eta)

if [ 1 -eq 1 ]; then
    ./stack2.py -q 0 -o CRAP --rebin ${rebin} --hsource d3_abseta_lpt_phi:y:2:-1:x:0:-1 --bin 10,-3.15,3.15 --refline 0.85,1.175 -b --input ${input1} -m control_stack --bgqcd 0 --qcdscale 1.0 --lnofits --nomonly --subdirmc MuonTriggerSFPhi -t WP_FF &
    ./stack2.py -q 1 -o CRAP --rebin ${rebin} --hsource d3_abseta_lpt_phi:y:2:-1:x:0:-1 --bin 10,-3.15,3.15 --refline 0.85,1.175 -b --input ${input1} -m control_stack --bgqcd 0 --qcdscale 1.0 --lnofits --nomonly --subdirmc MuonTriggerSFPhi -t WN_FF &

    ./stack2.py -q 0 -o CRAP --rebin ${rebin} --hsource d3_abseta_lpt_phi:y:2:-1:x:0:-1 --bin 10,-3.15,3.15 --refline 0.85,1.175 -b --input ${input1} -m control_stack --bgqcd 0 --qcdscale 1.0 --lnofits --nomonly --subdirmc st_w_final -t WP_FE &
    ./stack2.py -q 1 -o CRAP --rebin ${rebin} --hsource d3_abseta_lpt_phi:y:2:-1:x:0:-1 --bin 10,-3.15,3.15 --refline 0.85,1.175 -b --input ${input1} -m control_stack --bgqcd 0 --qcdscale 1.0 --lnofits --nomonly --subdirmc st_w_final -t WN_FE &

    ./stack2.py -q 0 -o CRAP --rebin ${rebin} --hsource d3_abseta_lpt_phi:y:2:-1:x:0:-1 --bin 10,-3.15,3.15 --refline 0.85,1.175 -b --input ${input1} -m control_stack --bgqcd 0 --qcdscale 1.0 --lnofits --nomonly --subdirmc MuonRecoSFPhi -t WP_EE &
    
fi

# let's try to plot Z
if [ 1 -eq 1 ]; then
    ./stack2.py -q -1 --ntuple z -o CRAP --rebin ${rebin} --var "lP_eta" --bin 10,-3.15,3.15 --hsource "leptonP_phi" --refline 0.85,1.175 -b --input ${input1} -m control_stack --bgqcd 0 --qcdscale 1.0 --lnofits --nomonly --subdirmc st_z_final -t ZP_FE &
    ./stack2.py -q -1 --ntuple z -o CRAP --rebin ${rebin} --var "lP_eta" --bin 10,-3.15,3.15 --hsource "leptonP_phi" --refline 0.85,1.175 -b --input ${input1} -m control_stack --bgqcd 0 --qcdscale 1.0 --lnofits --nomonly --subdirmc st_z_trigphi -t ZP_EE & # really, st_z_etaphi
fi

echo "Waiting for `jobs | wc -l` jobs..."
wait

