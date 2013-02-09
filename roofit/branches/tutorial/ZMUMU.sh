#!/bin/bash

source config.sh

zpre='lP_pt>20.0 && fabs(lP_eta)<2.4 && lN_pt>20.0 && fabs(lN_eta)<2.4    &&    lP_idhits==1 && fabs(lP_z0)<10.   &&   lN_idhits==1 && fabs(lN_z0)<10.   &&   Z_m>70 && Z_m<110    &&    fabs(lP_phi-lN_phi)>0.0 && (lP_q*lN_q)<0 && nmuons==2'
#zpre='lP_pt>20.0 && fabs(lP_eta)<2.4 && lN_pt>20.0 && fabs(lN_eta)<2.4    &&    lP_idhits==1 && fabs(lP_z0)<10.   &&   lN_idhits==1 && fabs(lN_z0)<10.   &&   Z_m>70 && Z_m<110    &&    fabs(lP_phi-lN_phi)>0.0 && (lP_q*lN_q)<0 && nmuons==2  &&  lP_trigEF<0.2' # with trigger matching - broken???
refline=0.971,1.029
refline=0.941,1.059

# ntuples
if [ "0" == "1" ]; then
    refline=0.991,1.039
    ./stack2.py --input ${input} -b --ntuple z -q -1 --var "lP_eta" --bin 10,-2.5,2.5 --hsource "leptonP_etav" -o ZPLOTS -t N --preNN "${zpre}" --pre "${zpre}" -m one_plot_nt --bgsig 5 --bgewk 5 --bgqcd 0 --qcdscale 1.0 --refline ${refline} &
    ./stack2.py --input ${input} -b --ntuple z -q -1 --var "lN_eta" --bin 10,-2.5,2.5 --hsource "leptonN_etav" -o ZPLOTS -t N --preNN "${zpre}" --pre "${zpre}" -m one_plot_nt --bgsig 5 --bgewk 5 --bgqcd 0 --qcdscale 1.0 --refline ${refline} &

    refline=0.941,1.079
    ./stack2.py --input ${input} -b --ntuple z -q -1 --var "lP_phi" --bin 50,-3.15,3.15 --hsource "leptonP_phi" -o ZPLOTS -t N --preNN "${zpre}" --pre "${zpre}" -m one_plot_nt --bgsig 5 --bgewk 5 --bgqcd 0 --qcdscale 1.0 --refline ${refline} &
    ./stack2.py --input ${input} -b --ntuple z -q -1 --var "lN_phi" --bin 50,-3.15,3.15 --hsource "leptonN_phi" -o ZPLOTS -t N --preNN "${zpre}" --pre "${zpre}" -m one_plot_nt --bgsig 5 --bgewk 5 --bgqcd 0 --qcdscale 1.0 --refline ${refline} &

    refline=0.981,1.079
    ./stack2.py --input ${input} -b --ntuple z -q -1 --var "Z_m" --bin 40,70,110 --hsource "Z_m" -o ZPLOTS -t N --preNN "${zpre}" --pre "${zpre}" -m one_plot_nt --bgsig 5 --bgewk 5 --bgqcd 0 --qcdscale 1.0 --refline ${refline} &
    
fi

# histograms
if [ "1" == "1" ]; then
    mode=one_plot
    mode=control_stack
    refline=0.85,1.175
    xtra="--nomonly"
    #refline=0.991,1.039
    ./stack2.py --input ${input} -b --ntuple z -q -1 --var "lP_eta" --bin 10,-2.5,2.5 --hsource "leptonP_etav" -o ZPLOTS -t H -m ${mode} --bgsig 5 --bgewk 5 --bgqcd 0 --qcdscale 1.0 --refline ${refline} ${xtra} &
    ./stack2.py --input ${input} -b --ntuple z -q -1 --var "lN_eta" --bin 10,-2.5,2.5 --hsource "leptonN_etav" -o ZPLOTS -t H -m ${mode} --bgsig 5 --bgewk 5 --bgqcd 0 --qcdscale 1.0 --refline ${refline} ${xtra} &
    
    #refline=0.981,1.079
    ./stack2.py --input ${input} -b --ntuple z -q -1 --var "Z_m" --bin 10,70,110 --hsource "z_m" -o ZPLOTS -t H -m ${mode} --bgsig 5 --bgewk 5 --bgqcd 0 --qcdscale 1.0 --refline ${refline} ${xtra} &
    #refline=0.901,1.099
    ./stack2.py --input ${input} -b --ntuple z -q -1 --var "Z_pt" --bin 10,0,100 --hsource "z_pt" -o ZPLOTS -t H -m ${mode} --bgsig 5 --bgewk 5 --bgqcd 0 --qcdscale 1.0 --refline ${refline} ${xtra} &
fi;

echo "Please wait..."
wait
echo DONE
