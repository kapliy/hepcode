#!/bin/bash
for v in {D..M}; do 
    (grep DPETER /home/antonk/TrigFTKAna/samples/wasymmetry29I_data_period${v}.dat | cut -d ' ' -f 2) > ${v};
    echo $v;
done
