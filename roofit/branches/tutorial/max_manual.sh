#!/bin/bash

# manual plotter of individual samples.
# Presently, this is configured to compare atlfast samples with fullsim samples at reco level.

input=/share/t3data3/antonk/ana/ana_v29I_10212012_unfold_stacoCB_all  # renamed lepton_pt for consistency

# MAIN PLOTTER
for q in 0 1; do
    ./stack2.py -b --input ${input} -q ${q} --refline 0.85,1.15 --hsource "lepton_absetav" -o TEST -t TEST -m manual --bgsig 5 --bgewk 5 --bgqcd 0 &
    ./stack2.py -b --input ${input} -q ${q} --refline 0.85,1.15 --hsource "lepton_etav" -o TEST -t TEST -m manual --bgsig 5 --bgewk 5 --bgqcd 0 &
    ./stack2.py -b --input ${input} -q ${q} --refline 0.85,1.15 --hsource "wmt" -o TEST -t TEST -m manual --bgsig 5 --bgewk 5 --bgqcd 0 &
    ./stack2.py -b --input ${input} -q ${q} --refline 0.85,1.15 --hsource "met" -o TEST -t TEST -m manual --bgsig 5 --bgewk 5 --bgqcd 0 &
    ./stack2.py -b --input ${input} -q ${q} --refline 0.85,1.15 --hsource "lpt" -o TEST -t TEST -m manual --bgsig 5 --bgewk 5 --bgqcd 0 &
done

wait


