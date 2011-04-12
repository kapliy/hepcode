#!/bin/bash

i=0

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

./KEYS_CMB.sh root_all_0331.root data cmb --shift &> LOG${i} &
((i++))
./KEYS_CMB.sh root_all_0331.root data id --shift &> LOG${i} &
((i++))
./KEYS_CMB.sh root_all_0331.root data ms --shift &> LOG${i} &
((i++))
./KEYS_CMB.sh root_all_0331.root data exms --shift &> LOG${i} &
((i++))
wait
echo "Done up to ${i}"


# old stuff: first run over 2011 data
if [ "0" -eq "1" ]; then
./KEYS_CMB.sh root_all_0331.root data_20110329L1 cmb &> LOG${i} &
((i++))
./KEYS_CMB.sh root_all_0331.root data_20110329L1 id &> LOG${i} &
((i++))
./KEYS_CMB.sh root_all_0331.root data_20110329L1 ms &> LOG${i} &
((i++))
wait
echo "Done up to ${i}"

./KEYS_CMB.sh root_all_0331.root data_20110329 cmb &> LOG${i} &
((i++))
./KEYS_CMB.sh root_all_0331.root data_20110329 id &> LOG${i} &
((i++))
./KEYS_CMB.sh root_all_0331.root data_20110329 ms &> LOG${i} &
((i++))
wait
echo "Done up to ${i}"

./KEYS_CMB.sh root_all_0331.root data_20110331 cmb &> LOG${i} &
((i++))
./KEYS_CMB.sh root_all_0331.root data_20110331 id &> LOG${i} &
((i++))
./KEYS_CMB.sh root_all_0331.root data_20110331 ms &> LOG${i} &
((i++))
./KEYS_CMB.sh root_all_0331.root data_20110331 exms &> LOG${i} &
((i++))
wait
echo "Done up to ${i}"

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
fi;

echo "Completely done!"
