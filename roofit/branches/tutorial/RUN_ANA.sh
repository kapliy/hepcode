#!/bin/bash

i=0

./KEYS_ANA.sh root_all_0331.root data mc_zmumu cmb &> LOG.ANA.${i} &
((i++))
./KEYS_ANA.sh root_all_0331.root data mc_zmumu id &> LOG.ANA.${i} &
((i++))
./KEYS_ANA.sh root_all_0331.root data mc_zmumu ms &> LOG.ANA.${i} &
((i++))
./KEYS_ANA.sh root_all_0331.root data mc_zmumu exms &> LOG.ANA.${i} &
((i++))
wait
echo "Done up to ${i}"

echo "Completely done!"
