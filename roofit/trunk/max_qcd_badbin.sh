#!/bin/bash

# Control plot stacks

input=/share/t3data3/antonk/ana/ana_v29I_11012012_unfold_stacoCB_all/ # studying met term
input=/share/t3data3/antonk/ana/ana_v29I_11072012_unfold_stacoCB_all

rebin=4
for ibin in 1 20; do
    ./stack2.py -b --input ${input} --hsource "bine_${ibin}/db_met_muonboy" -o TERMS -t EBIN${ibin} -m control_stack_nonorm --bgsig 5 --bgewk 5 --bgqcd 1 --refline 0.51,1.49 --rebin ${rebin} &
    ./stack2.py -b --input ${input} --hsource "bine_${ibin}/db_met_reffinal" -o TERMS -t EBIN${ibin} -m control_stack_nonorm --bgsig 5 --bgewk 5 --bgqcd 1 --refline 0.51,1.49 --rebin ${rebin} &
    ./stack2.py -b --input ${input} --hsource "bine_${ibin}/db_lpt" -o TERMS -t EBIN${ibin} -m control_stack_nonorm --bgsig 5 --bgewk 5 --bgqcd 1 --refline 0.51,1.49 --rebin ${rebin} &
    ./stack2.py -b --input ${input} --hsource "bine_${ibin}/db_lpt_id" -o TERMS -t EBIN${ibin} -m control_stack_nonorm --bgsig 5 --bgewk 5 --bgqcd 1 --refline 0.51,1.49 --rebin ${rebin} &
    ./stack2.py -b --input ${input} --hsource "bine_${ibin}/db_lpt_exms" -o TERMS -t EBIN${ibin} -m control_stack_nonorm --bgsig 5 --bgewk 5 --bgqcd 1 --refline 0.51,1.49 --rebin ${rebin} &
    ./stack2.py -b --input ${input} --hsource "bine_${ibin}/db_phi" -o TERMS -t EBIN${ibin} -m control_stack_nonorm --bgsig 5 --bgewk 5 --bgqcd 1 --refline 0.51,1.49 --rebin ${rebin} &
    ./stack2.py -b --input ${input} --hsource "bine_${ibin}/db_ptiso40r" -o TERMS -t EBIN${ibin} -m control_stack_nonorm --bgsig 5 --bgewk 5 --bgqcd 1 --refline 0.51,1.49 --rebin ${rebin} &
done



echo "Please wait..."
wait
