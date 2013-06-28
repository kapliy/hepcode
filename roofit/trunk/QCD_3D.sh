#!/bin/bash

# QCD fits that use 3d histograms as an input
# this script makes all qcd summary plots, plus qcd/ewk fraction plots.

source config.sh

# manual run
if [ "0" == "1" ]; then
    iq=1
    ./stack2.py --input ${input} -b --charge ${iq} --lvar d3_abseta_lpt_met --lbin 60,0,60 --isofail IsoWind20 -o TEST -t Q${iq} -m qcdfit --bgsig ${bgsig} --bgewk 5 --bgtau 2 --bgqcd 4 --ieta 11 --ipt ALL25 --var d3_abseta_lpt_met --xsecerr 0 --etamode 2 --rebin 4
fi

function make_pos_neg() {
    FPOS=$1
    FNEG=$2
    echo """
    \begin{tabular}{lrr|rr}
      \hline
      \hline
       & \multicolumn{2}{c|}{\$W^+ \rightarrow \mu^+ \nu$} & \multicolumn{2}{c}{\$W^- \rightarrow \mu^- \nu$} \\\\
 Variation & $\delta N_{\mbox{QCD}} / N_{\mbox{QCD}}$ & $\delta N_{\mbox{QCD}} / N_{W+}^{\mbox{Cand.}}$ & $\delta N_{\mbox{QCD}} / N_{\mbox{QCD}}$ & $\delta N_{\mbox{QCD}} / N_{W^-}^{\mbox{Cand.}}$ \\\\
      \hline
    """
    awk 'BEGIN{FS="&"}NF==3{printf("%s & %s\n",$2,$3); }NF==1{print $0}' ${FNEG} > ${FNEG}.tmp
    cat ${FPOS} | sed -e 's# \\\\# \& #g' > ${FPOS}.tmp
    paste ${FPOS}.tmp ${FNEG}.tmp
    rm -f ${FNEG}.tmp ${FPOS}.tmp
    echo """
      \hline
      \hline
    \end{tabular}
    """
}

nomonly=" --extra ${DB} --exit"   # to make systematic summary plots
out=FITS
if [ "1" == "1" ]; then
    for iq in 0 1; do
	# fits in eta slices
	for ipt in {1..7} ALL20 ALL25; do
	    ./stack2.py ${nomonly} --input ${input} -b --charge ${iq} --lvar d3_abseta_lpt_met --lbin 60,0,60 --isofail IsoWind20 -o ${out} -t Q${iq} -m qcdfit_sys --bgsig ${bgsig} --bgewk 5 --bgtau 2 --bgqcd 4  --ipt ${ipt} --var d3_abseta_lpt_met --xsecerr 0 --etamode 2 --rebin 4 --ieta LOOP &> LOG.abseta.q${iq}.pt${ipt} &
	done
	# inclusive fits
	for ipt in ALL20 ALL25; do
	    ./stack2.py ${nomonly} --input ${input} -b --charge ${iq} --lvar d3_abseta_lpt_met --lbin 60,0,60 --isofail IsoWind20 -o ${out} -t Q${iq} -m qcdfit_sys --bgsig ${bgsig} --bgewk 5 --bgtau 2 --bgqcd 4  --ipt ${ipt} --var d3_abseta_lpt_met --xsecerr 0 --etamode 2 --rebin 4 --ieta ALL  &> LOG.abseta.q${iq}.inc25.${ipt} &
	done
	# adrian's plot of qcd and ewk fractions
	./stack2.py ${nomonly} --input ${input} -b --charge ${iq} --lvar d3_abseta_lpt_met --lbin 60,0,60 --isofail IsoWind20 -o ${out} -t Q${iq} -m qcdfit_adrian --bgsig ${bgsig} --bgewk 5 --bgtau 2 --bgqcd 4  --ipt ALL20 --var d3_abseta_lpt_met --xsecerr 0 --etamode 2 --rebin 4 --ieta LOOP &> LOG.abseta.q${iq}.adrian &
    done
    # combine charges in the inclusive table
    wait
    cd FITS
    make_pos_neg tableQCD_POS_pt20.tex tableQCD_NEG_pt20.tex > tableQCD_ALL_pt20.tex
    make_pos_neg tableQCD_POS_pt25.tex tableQCD_NEG_pt25.tex > tableQCD_ALL_pt25.tex
fi

echo "Please wait..."
wait
