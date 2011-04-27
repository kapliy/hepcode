#!/bin/bash

i=0

# 2010 old - sanity check
./KEYS_CMB.sh root_all_0331.root data cmb &> LOG${i} &
((i++))
./KEYS_CMB.sh root_all_0331.root data id &> LOG${i} &
((i++))
./KEYS_CMB.sh root_all_0331.root data ms &> LOG${i} &
((i++))
./KEYS_CMB.sh root_all_0331.root data exms &> LOG${i} &
((i++))
wait
echo "Done up to ${i}"

# 2011 (and latest rerun of 2010)
fname=root_all_apr26data2011BD3.root
folders="data_2010repro  data_20110331  data_20110425  data_20110425B  data_20110425D"
for fold in $folders; do 
    echo "STATUS: $fold"
    ./KEYS_CMB.sh ${fname} ${fold} cmb &> LOG${i} &
    ((i++))
    ./KEYS_CMB.sh ${fname} ${fold} id &> LOG${i} &
    ((i++))
    ./KEYS_CMB.sh ${fname} ${fold} ms &> LOG${i} &
    ((i++))
    ./KEYS_CMB.sh ${fname} ${fold} exms &> LOG${i} &
    ((i++))
    wait
    echo "Done up to ${i}"
done
