#!/bin/bash

for d in powheg_herwig_wminmunu powheg_herwig_wplusmunu mcnlo_wminmunu mcnlo_wplusmunu; do
#for d in {D..M} powheg_pythia_wminmunu powheg_pythia_wplusmunu powheg_herwig_wminmunu powheg_herwig_wplusmunu mcnlo_wminmunu mcnlo_wplusmunu; do
    echo "Working on: $d ..."
    ./counts.py $d > COUNTS.${d} &
done
echo "DONE!"
wait
