#!/bin/bash

ntver=29L

for f in pythia* acer* alpgen* herwig* mcnlo* powheg*; do 
    if [ -s $f ]; then 
	./mc.sh $f /home/antonk/TrigFTKAna/samples/wasymmetry${ntver}_mc_$f.dat; 
    fi;
done

for f in D E F G H I J K L M; do
    if [ -s $f ]; then 
	./data.sh $f /home/antonk/TrigFTKAna/samples/wasymmetry${ntver}_data_period$f.dat; 
    fi
done
