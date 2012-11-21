#!/bin/bash

#input=/share/t3data3/antonk/ana/ana_v29I_11072012_unfold_stacoCB_all/
#input=/share/t3data3/antonk/ana/ana_v29I_11172012_edboard_stacoCB_all
input=/share/t3data3/antonk/ana/ana_v29I_11172012_edboard_nophi_stacoCB_all

# Validate z vertex reweighting, <mu> reweighting, and nvtx reweighting

refline="0.041,2.059"

# Z0 vertex reweighting
if [ "1" == "1" ]; then
    ./stack2.py --input ${input} -b --var "vxz0" --hsource "vxz0" -t TEST -m plot_z0 --bgsig 5 --bgewk 5 --bgqcd 0 --refline ${refline}  -o TEST --qcdscale 1.0 --sysdir '' --subdir '' --basedir '' --charge -1 &> LOG.dis.1 &
    ./stack2.py --input ${input} -b --var "vxz0_unw" --hsource "vxz0_unw" -t TEST -m plot_z0 --bgsig 5 --bgewk 5 --bgqcd 0 --refline ${refline}  -o TEST --qcdscale 1.0 --sysdir '' --subdir '' --basedir '' --charge -1 &> LOG.dis.2 &
fi

refline="0.741,1.259"
# <mu>
if [ "1" == "1" ]; then
    ./stack2.py --input ${input} -b --var "avgmu" --hsource "avgmu" -t TEST -m plot_z0 --bgsig 5 --bgewk 5 --bgqcd 0 --refline ${refline}  -o TEST --qcdscale 1.0 --sysdir '' --subdir '' --basedir '' --charge -1 &> LOG.dis.3 &
    ./stack2.py --input ${input} -b --var "avgmu_unw" --hsource "avgmu_unw" -t TEST -m plot_z0 --bgsig 5 --bgewk 5 --bgqcd 0 --refline ${refline}  -o TEST --qcdscale 1.0 --sysdir '' --subdir '' --basedir '' --charge -1 &> LOG.dis.4 &
fi

# <nvtx>
if [ "1" == "1" ]; then
    ./stack2.py --input ${input} -b --var "nvtx_grltrig" --hsource "nvtx_grltrig" -t TEST -m plot_z0 --bgsig 5 --bgewk 5 --bgqcd 0 --refline ${refline}  -o TEST --qcdscale 1.0 --sysdir '' --subdir '' --basedir '' --charge -1 &> LOG.dis.5 &
    ./stack2.py --input ${input} -b --var "nvtx_grltrig_unw" --hsource "nvtx_grltrig_unw" -t TEST -m plot_z0 --bgsig 5 --bgewk 5 --bgqcd 0 --refline ${refline}  -o TEST --qcdscale 1.0 --sysdir '' --subdir '' --basedir '' --charge -1 &> LOG.dis.6 &
fi

refline="0.541,1.559"
if [ "0" == "1" ]; then # not available!
    ./stack2.py --input ${input} -b --var "avgmu" --hsource "avgmu" -t TEST -m one_plot --bgsig 5 --bgewk 5 --bgqcd 0 --refline ${refline}  -o TEST --qcdscale 1.0  --subdir '' --basedir '' --charge -1
fi


echo "Please wait..."
wait
