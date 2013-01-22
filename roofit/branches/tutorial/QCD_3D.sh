#!/bin/bash

# QCD fits that use 3d histograms as an input

# THIS CODE CAN BE USED TO CLEAN UP THE ACTUAL NOMINAL QCD FITS:
if [ "0" == "1" ]; then
    rm -f Q?_stack*
    rm -f  Q?_qcd_syst*
    for f in Q?_Q4S5X5Y2Z2_IsoWind20__Nominal_st_w_final_metfit_NEG_d3_abseta_lpt_met_*; do n=`echo $f | sed -e 's#Q4S5X5Y2Z2_IsoWind20__Nominal_st_w_final_metfit_NEG_d3_abseta_lpt_met_##g' -e 's#_0to40##g'`; mv $f $n; done
    for f in Q?_Q4S5X5Y2Z2_IsoWind20__Nominal_st_w_final_metfit_POS_d3_abseta_lpt_met_*; do n=`echo $f | sed -e 's#Q4S5X5Y2Z2_IsoWind20__Nominal_st_w_final_metfit_POS_d3_abseta_lpt_met_##g' -e 's#_0to40##g'`; mv $f $n; done
fi

source config.sh

# manual run
if [ "0" == "1" ]; then
    iq=1
    ./stack2.py --input ${input} -b --charge ${iq} --lvar d3_abseta_lpt_met --lbin 50,0,40 --isofail IsoWind20 -o TEST -t Q${iq} -m qcdfit --bgsig ${bgsig} --bgewk 5 --bgtau 2 --bgqcd 4 --ieta 11 --ipt ALL25 --var d3_abseta_lpt_met --xsecerr 0 --etamode 2 --rebin 4
fi

#nomonly=" --nomonly"  # to save actual QCD plots [for a big table in the paper]
nomonly=" --extra ${DB} --exit"   # to make systematic summary plots
if [ "1" == "1" ]; then
    for iq in 0 1; do
	# fits in eta slices
	for ipt in {1..7} ALL20 ALL25; do
	    ./stack2.py ${nomonly} --input ${input} -b --charge ${iq} --lvar d3_abseta_lpt_met --lbin 50,0,40 --isofail IsoWind20 -o ${out} -t Q${iq} -m qcdfit_sys --bgsig ${bgsig} --bgewk 5 --bgtau 2 --bgqcd 4  --ipt ${ipt} --var d3_abseta_lpt_met --xsecerr 0 --etamode 2 --rebin 4 --ieta LOOP &> LOG.abseta.q${iq}.pt${ipt} &
	done
	# inclusive fits
	for ipt in ALL20 ALL25; do
	    ./stack2.py ${nomonly} --input ${input} -b --charge ${iq} --lvar d3_abseta_lpt_met --lbin 50,0,40 --isofail IsoWind20 -o ${out} -t Q${iq} -m qcdfit_sys --bgsig ${bgsig} --bgewk 5 --bgtau 2 --bgqcd 4  --ipt ${ipt} --var d3_abseta_lpt_met --xsecerr 0 --etamode 2 --rebin 4 --ieta ALL  &> LOG.abseta.q${iq}.inc25.${ipt} &
	done
    done
fi

echo "Please wait..."
wait
