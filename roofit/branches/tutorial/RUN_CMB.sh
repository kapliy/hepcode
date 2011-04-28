#!/bin/bash

i=0

# 2010 old - sanity check
./KEYS_CMB.sh ROOT/root_all_0331.root data cmb data &> LOG${i} &
((i++))
./KEYS_CMB.sh ROOT/root_all_0331.root data id data &> LOG${i} &
((i++))
./KEYS_CMB.sh ROOT/root_all_0331.root data ms data &> LOG${i} &
((i++))
./KEYS_CMB.sh ROOT/root_all_0331.root data exms data &> LOG${i} &
((i++))
wait
echo "Done up to ${i}"
