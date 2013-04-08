#!/bin/bash

outfolder="/share/t3data2/jwebster/ana/monojet"
if [ $# -eq 0 ]; then
    outfolder="${outfolder}/`date +%Y%m%d`_mc"
else
    outfolder="${outfolder}/${1}_mc"
fi

for iinput in /share/ftkdata/jwebster/ana_input/v1_50/user.*mc12*.in; do
    ioutput=`echo ${iinput} | sed -e 's#/share/ftkdata/jwebster/ana_input/v1_50/##g' | sed -e 's#\.in##g'`
    nfiles="(`cat ${iinput} | wc -l`-3)/2"
    nfiles=`echo ${nfiles} | bc`
    nsplits=25
    if [ $nfiles -lt 10 ]; then nsplits=1;
    elif [ $nfiles -gt 100 ]; then nsplits=50; fi
    echo "$ioutput : $nfiles : $nsplits"
    let totaljobs+=$nsplits
    scripts/ana.py -e ana_monojet -s ${iinput} -o ${outfolder} -t ${ioutput} -n $nsplits
done

echo "DONE"
