#!/bin/bash

# Saved qcd histograms for Max
source config.sh

name=${INDB}

# unfolding in |eta|
./stack2.py -m prepare_qcd_2d --input ${input} -b -o TEST -t TEST --bgqcd ${bgqcd} --extra "${name}.abseta.2D.pt20.root" --ipt 20 --etamode 2 &> LOG.PREP.1 &

./stack2.py -m prepare_qcd_1d --input ${input} -b -o TEST -t TEST --bgqcd ${bgqcd} --extra "${name}.abseta.1D.pt20.root" --ipt 20 --etamode 2 &> LOG.PREP.2 &
./stack2.py -m prepare_qcd_1d --input ${input} -b -o TEST -t TEST --bgqcd ${bgqcd} --extra "${name}.abseta.1D.pt25.root" --ipt 25 --etamode 2 &> LOG.PREP.3 &

./stack2.py -m prepare_qcd_0d --input ${input} -b -o TEST -t TEST --bgqcd ${bgqcd} --extra "${name}.abseta.0D.pt20.root" --ipt 20 --etamode 2 &> LOG.PREP.4 &
./stack2.py -m prepare_qcd_0d --input ${input} -b -o TEST -t TEST --bgqcd ${bgqcd} --extra "${name}.abseta.0D.pt25.root" --ipt 25 --etamode 2 &> LOG.PREP.5 &

# experimental: signed eta to check A-vs-C side differences
./stack2.py -m prepare_qcd_1d --input ${input} -b -o TEST -t TEST --bgqcd ${bgqcd} --extra "${name}.eta.1D.pt25.root" --ipt 25 --etamode 1 &> LOG.PREP.6 &

# DEPRECATED: unfolding in eta, as opposed to |eta|
if [ "0" == "1" ]; then
    ./stack2.py -m prepare_qcd_2d --input ${input} -b -o TEST -t TEST --bgqcd ${bgqcd} --extra "${name}.eta.2D.root" --etamode 1 &> LOG.PREP.8 &
    ./stack2.py -m prepare_qcd_1d --input ${input} -b -o TEST -t TEST --bgqcd ${bgqcd} --extra "${name}.eta.1D.root" --etamode 1 &> LOG.PREP.9 &
fi

echo "Please wait..."
wait
echo "DONE"
