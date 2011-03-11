#!/bin/bash

for reg in AA BB CC; do
for p in 0.95 0.98 1.0 1.02 1.05; do
for n in 0.95 0.98 1.0 1.02 1.05; do
    echo $reg $p $n
    leg=p${p}_n${n}
    fname=root_z_${leg}.root
    hist=z_${leg}_z_${leg}.root
    ./zpeak.py --ndata 100000 -b --egge --res 3 --root /share/ftkdata1/antonk/anamar02_zscale_scan/${fname} --data ${hist}/dg/dg/st_z_final/${reg}/graph_Z_m_eta -t SCALE.${reg}.${leg} &> logs/LOG.SCALE.${reg}.${leg}

done
done
done

