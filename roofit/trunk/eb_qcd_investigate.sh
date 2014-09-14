#!/bin/bash

# dumps all nominal QCD fits for paper appendix

source config.sh

nomonly=" --nomonly"  # to save actual QCD plots [for a big grid of plots in the paper]
shack=""
#shack=" --stathack 2"   # scale to lower-stat MC (wtaunu)

out=EB_FITS_qcdfits
if [ "1" == "1" ]; then
    for iq in 0 1; do
	# dump systematics for inclusive fits
	./stack2.py ${shack} --input ${input} -b --charge ${iq} --lvar d3_abseta_lpt_met --lbin 60,0,60 --isofail IsoWind20 -o ${out}_allsys -t Q${iq} -m qcdfit_sys --bgsig ${bgsig} --bgewk 5 --bgtau 2 --bgqcd 4  --ipt ALL25 --var d3_abseta_lpt_met --xsecerr 0 --etamode 2 --rebin 4 --ieta ALL  &> LOG.dump.syst.q${iq}.inc25 &
	# dump systematics for inclusive fits using qcdhack=2 (to be consistent with Adrian/Sebastian)
	./stack2.py --stathack 2 --input ${input} -b --charge ${iq} --lvar d3_abseta_lpt_met --lbin 60,0,60 --isofail IsoWind20 -o ${out}_inc -t Q${iq} -m qcdfit_sys --bgsig ${bgsig} --bgewk 5 --bgtau 2 --bgqcd 4  --ipt ALL25 --var d3_abseta_lpt_met --xsecerr 0 --etamode 2 --rebin 4 --ieta ALL  &> LOG.statdump.syst.q${iq}.inc25 &

    done
fi

echo "Please wait..."
wait
echo "ALL DONE"
