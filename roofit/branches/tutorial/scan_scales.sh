#!/bin/bash

# run this after running run_scales.sh (in the same directory)

reg=AA
for p in {1.0,1.02,1.05,0.98,0.95}; do
    for n in {1.0,1.02,1.05,0.98,0.95}; do
	
	mzf=`awk '$1=="mz"{print $3}' SCALE.${reg}.p${p}_n${n}_results.rtxt`
	if [ "${p}" == "1.0" -a "${n}" == "1.0" ]; then mz0=$mzf; fi
	mze=`echo "$mz0*sqrt($p*$n)" | bc -l`
	echo $p $n $mze $mzf `awk "BEGIN{print ($mze-$mzf)*1000}"`
    done
done
#awk 'NR==1{mz=$3; erz=$4} {print $1,$2,sqrt($1*$2)*mz,$3,(sqrt($1*$2)*mz-$3)/sqrt(erz*erz+$4*$4)}' RESULTS
