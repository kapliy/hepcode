#!/bin/bash

# Re-coding QCD fits to use 3d histograms
# Note: qcdfit_sys over-rides most inputs, such as --var and --lvar and --lbin. so feel free to set them to anything.

# THIS SCRIPT CAN BE USED TO PRODUCE THE ACTUAL QCD FITS:
#for f in Q?_Q4S5X5Y2Z2_IsoWind20__Nominal_st_w_final_metfit_NEG_d3_abseta_lpt_met_*; do n=`echo $f | sed -e 's#Q4S5X5Y2Z2_IsoWind20__Nominal_st_w_final_metfit_NEG_d3_abseta_lpt_met_##g' -e 's#_0to40##g'`; mv $f $n; done
#for f in Q?_Q4S5X5Y2Z2_IsoWind20__Nominal_st_w_final_metfit_POS_d3_abseta_lpt_met_*; do n=`echo $f | sed -e 's#Q4S5X5Y2Z2_IsoWind20__Nominal_st_w_final_metfit_POS_d3_abseta_lpt_met_##g' -e 's#_0to40##g'`; mv $f $n; done


input=/share/t3data3/antonk/ana/ana_v29I_12042012_edboard_stacoCB_all
DB=DB_12112012_ALL.v1

iq=1
bgsig=5

if [ "0" == "1" ]; then
    ./stack2.py --input ${input} -b --charge ${iq} --lvar d3_abseta_lpt_met --lbin 50,0,40 --isofail IsoWind20 -o TEST -t Q${iq} -m qcdfit_sys --bgsig ${bgsig} --bgewk 5 --bgtau 2 --bgqcd 4 --ieta 1 --ipt ALL --var d3_abseta_lpt_met --xsecerr 0 --etamode 2 --rebin 4
fi

noplots=" --exit"
nomonly=" --nomonly"
nomonly=" --extra ${DB}"
for iq in 0 1; do 
    for ipt in {1..7} ALL; do
	./stack2.py ${nomonly} ${noplots} --input ${input} -b --charge ${iq} --lvar d3_abseta_lpt_met --lbin 50,0,40 --isofail IsoWind20 -o TEST -t Q${iq} -m qcdfit_sys --bgsig ${bgsig} --bgewk 5 --bgtau 2 --bgqcd 4  --ipt ${ipt} --var d3_abseta_lpt_met --xsecerr 0 --etamode 2 --rebin 4 --ieta LOOP &> LOG.abseta.q${iq}.pt${ipt} &
    done
    ./stack2.py ${nomonly} ${noplots} --input ${input} -b --charge ${iq} --lvar d3_abseta_lpt_met --lbin 50,0,40 --isofail IsoWind20 -o TEST -t Q${iq} -m qcdfit_sys --bgsig ${bgsig} --bgewk 5 --bgtau 2 --bgqcd 4  --ipt ALL --var d3_abseta_lpt_met --xsecerr 0 --etamode 2 --rebin 4 --ieta ALL  &> LOG.abseta.q${iq}.inclusive &
done

echo "Please wait..."
wait
