#!/bin/bash

# THIS VERSION USES anyfit FIT REGION - i.e., WMT CUT RELAXED TO ZERO IN ADDITION TO MET CUT

# Re-coding QCD fits to use 3d histograms
# Note: qcdfit_sys over-rides most inputs, such as --var and --lvar and --lbin. so feel free to set them to anything.

input=/share/t3data3/antonk/ana/ana_v29I_12042012_edboard_stacoCB_all

iq=1
bgsig=5

for iq in 0 1; do 
    for ipt in {1..7} ALL; do
	./_stack3.py --exit --extra DB_12102012_ANY.v1 --input ${input} -b --charge ${iq} --lvar d3_abseta_lpt_met --lbin 50,0,40 --isofail IsoWind20 -o TEST_ANY -t Q${iq} -m qcdfit_sys --bgsig ${bgsig} --bgewk 5 --bgtau 2 --bgqcd 4  --ipt ${ipt} --var d3_abseta_lpt_met --xsecerr 0 --etamode 2 --rebin 4 --ieta LOOP &> LOG.anyeta.q${iq}.pt${ipt} &
    done
done
./_stack3.py --exit --extra DB_12102012_ANY.v1 --input ${input} -b --charge ${iq} --lvar d3_abseta_lpt_met --lbin 50,0,40 --isofail IsoWind20 -o TEST_ANY -t Q${iq} -m qcdfit_sys --bgsig ${bgsig} --bgewk 5 --bgtau 2 --bgqcd 4  --ipt ALL --var d3_abseta_lpt_met --xsecerr 0 --etamode 2 --rebin 4 --ieta ALL  &> LOG.anyeta.inclusive &

echo "Please wait..."
wait
