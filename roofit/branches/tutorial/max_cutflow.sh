#!/bin/bash

# RUNNING CUTFLOW
source config.sh
bdir=`dirname ${input}`

echo "Input directory: ${bdir}"

if [ "$#" -ge "1" ]; then
    echo "Merging files for combined cutflow:"
    cd ${bdir}
    for pt in 20 25; do
	sdir=${bdir}/pt${pt}
	echo "Processing PT=${pt}"
	/bin/ls ${sdir}/data_period*/data*root > ${sdir}/ALLDATA.list
	~/TrigFTKAna/plot/dgadd -f 1 -d ${sdir}/ALLDATA.root -l ${sdir}/ALLDATA.list
    done
else
    echo "Printing latex tables:"
    out=/home/antonk/SupportingDocument/Wmunu/figures/cutflow
    mkdir -p $out
    for pt in 20 25; do
    	sdir=${bdir}/pt${pt}
	./max_cutflow.py ${sdir} 1 POS ${pt} > ${out}/data_pt${pt}.tex && ./max_cutflow.py ${sdir} 0 POS ${pt} > ${out}/mc_POS_pt${pt}.tex && ./max_cutflow.py ${sdir} 0 NEG ${pt} > ${out}/mc_NEG_pt${pt}.tex && echo OK
    done
fi
