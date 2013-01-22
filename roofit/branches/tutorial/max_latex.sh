#!/bin/bash

source config.sh
dout=${OUTDB}.abseta

out=/home/antonk/SupportingDocument/Wmunu/figures/bgcomp

for q in POS NEG; do
    # 1D
    for pt in 20 25; do
	echo "1D: ${q} pt${pt}"
	./max_latex.py ${dout}.1D.pt${pt}.root ${q} > ${out}/bgcomp_${q}_pt${pt}.tex &
    done
    # 2D
    pt=20
    for ipt in {1..7}; do
	echo "2D: ${q} pt${pt}"
	./max_latex.py ${dout}.2D.pt${pt}.root ${q} ${ipt} > ${out}/bgcomp_${q}_${ipt}.tex &
    done
done

wait
