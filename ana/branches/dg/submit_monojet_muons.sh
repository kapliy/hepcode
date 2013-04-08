#!/bin/bash

outfolder="/share/t3data2/jwebster/ana/monojet"
if [ $# -eq 0 ]; then
    outfolder="${outfolder}/`date +%Y%m%d`_muons"
else
    outfolder="${outfolder}/${1}_muons"
fi

for iinput in /share/ftkdata/jwebster/ana_input/v1_50/user.*.data12_8TeV.period*Muons*.in; do
    ioutput=`echo ${iinput} | sed -e 's#/share/ftkdata/jwebster/ana_input/v1_50/##g' | sed -e 's#\.in##g'`
    scripts/ana.py -e ana_monojet -s ${iinput} -o ${outfolder} -t ${ioutput} -n 100
done

echo "DONE"
