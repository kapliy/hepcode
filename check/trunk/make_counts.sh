#!/bin/bash

for d in {D..M} powheg_pythia_wminmunu powheg_pythia_wplusmunu; do
    echo "Working on: $d ..."
    ./counts.py $d > COUNTS.${d} &
done
echo "DONE!"
wait