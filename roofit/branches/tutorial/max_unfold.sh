#!/bin/bash

# Unfolded distributions with systematics.
# For now, this is using MC-based QCD only
# Currently, studying specifically MCP scale systematics

input=/share/t3data3/antonk/ana/ana_v29I_10212012_unfold_stacoCB_all  # renamed lepton_pt for consistency

# MAIN PLOTTER
for q in 0 1; do
    for ii in ALL {1..7}; do
	./stack2.py --input ${input} -q $q -b --hsource 'd2_eta_lpt:y:%d:%d' -o TEST -t UNF_Q${q}_${ii} -m unfold2d --bgsig 5 --bgewk 5 --bgqcd 0 --ipt ${ii} &> LOG.unf.Q${q}_${ii} &
    done
done

echo "Waiting..."
wait

