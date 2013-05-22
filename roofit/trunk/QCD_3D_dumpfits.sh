#!/bin/bash

# dumps all nominal QCD fits for paper appendix

source config.sh

nomonly=" --nomonly"  # to save actual QCD plots [for a big grid of plots in the paper]
out=FITS_qcdfits
if [ "1" == "1" ]; then
    for iq in 0 1; do
	# fits in eta slices
	for ipt in {1..7} ALL20 ALL25; do
	    ./stack2.py ${nomonly} --input ${input} -b --charge ${iq} --lvar d3_abseta_lpt_met --lbin 60,0,60 --isofail IsoWind20 -o ${out} -t Q${iq} -m qcdfit_sys --bgsig ${bgsig} --bgewk 5 --bgtau 2 --bgqcd 4  --ipt ${ipt} --var d3_abseta_lpt_met --xsecerr 0 --etamode 2 --rebin 4 --ieta LOOP &> LOG.dump.abseta.q${iq}.pt${ipt} &
	done
	# inclusive fits
	for ipt in ALL20 ALL25; do
	    ./stack2.py ${nomonly} --input ${input} -b --charge ${iq} --lvar d3_abseta_lpt_met --lbin 60,0,60 --isofail IsoWind20 -o ${out} -t Q${iq} -m qcdfit_sys --bgsig ${bgsig} --bgewk 5 --bgtau 2 --bgqcd 4  --ipt ${ipt} --var d3_abseta_lpt_met --xsecerr 0 --etamode 2 --rebin 4 --ieta ALL  &> LOG.dump.abseta.q${iq}.inc25.${ipt} &
	done
	# dump systematics for inclusive fits
	./stack2.py --input ${input} -b --charge ${iq} --lvar d3_abseta_lpt_met --lbin 60,0,60 --isofail IsoWind20 -o ${out}_allsys -t Q${iq} -m qcdfit_sys --bgsig ${bgsig} --bgewk 5 --bgtau 2 --bgqcd 4  --ipt ALL25 --var d3_abseta_lpt_met --xsecerr 0 --etamode 2 --rebin 4 --ieta ALL  &> LOG.dump.syst.q${iq}.inc25 &
	# dump systematics for inclusive fits using qcdhack=2 (to be consistent with Adrian/Sebastian)
	./stack2.py --stathack 2 --input ${input} -b --charge ${iq} --lvar d3_abseta_lpt_met --lbin 60,0,60 --isofail IsoWind20 -o ${out}_inc -t Q${iq} -m qcdfit_sys --bgsig ${bgsig} --bgewk 5 --bgtau 2 --bgqcd 4  --ipt ALL25 --var d3_abseta_lpt_met --xsecerr 0 --etamode 2 --rebin 4 --ieta ALL  &> LOG.statdump.syst.q${iq}.inc25 &

    done
fi

echo "Please wait..."
wait

echo "Cleaning up the directory: ${out}"
if [ -d "${out}" ]; then
    cd ${out}
    rm -f Q?_stack*
    rm -f  Q?_qcd_*
    for f in Q?_Q4S5X5Y2Z2_IsoWind20__Nominal_st_w_final_metfit_NEG_d3_abseta_lpt_met_*; do n=`echo $f | sed -e 's#Q4S5X5Y2Z2_IsoWind20__Nominal_st_w_final_metfit_NEG_d3_abseta_lpt_met_##g' -e 's#_0to60##g'`; mv $f $n; done
    for f in Q?_Q4S5X5Y2Z2_IsoWind20__Nominal_st_w_final_metfit_POS_d3_abseta_lpt_met_*; do n=`echo $f | sed -e 's#Q4S5X5Y2Z2_IsoWind20__Nominal_st_w_final_metfit_POS_d3_abseta_lpt_met_##g' -e 's#_0to60##g'`; mv $f $n; done
    cd -
fi

out="${out}_inc"
echo "Cleaning up the directory: ${out}"
if [ -d "${out}" ]; then
    cd ${out}
    rm -f Q?_stack*
    rm -f  Q?_qcd_*
    mkdir -p tmp
    for iq in 0 1; do
	q=POS
	if [ "$iq" == "1" ]; then q=NEG; fi
	for ext in pdf png; do
	    mv Q${iq}_Q4S1X5Y2Z2_IsoWind20__Nominal_st_w_final_metfit_${q}_d3_abseta_lpt_met_x_1_11_y_2_7_0to60.${ext} tmp/Q${iq}_mcnlo.${ext}
	    mv Q${iq}_Q4S4X5Y2Z2_IsoWind20__Nominal_st_w_final_metfit_${q}_d3_abseta_lpt_met_x_1_11_y_2_7_0to60.${ext} tmp/Q${iq}_powheg_herwig.${ext}
	    mv Q${iq}_Q4S5X5Y2Z2_IsoWind20__Nominal_MuonIsoSFUp_metfit_${q}_d3_abseta_lpt_met_x_1_11_y_2_7_0to60.${ext} tmp/Q${iq}_powheg_pythia.${ext}
	done
    done
    rm -f *.png *.pdf *.tex
    mv tmp/* .
    rmdir tmp
    cd -
fi

echo "ALL DONE"
