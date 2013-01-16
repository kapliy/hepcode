#!/bin/bash

# RUNNING CUTFLOW
bdir=/share/t3data3/antonk/ana/ana_v29I_12042012_edboard_stacoCB_all
bdir=/share/t3data3/antonk/ana/ana_v29I_01072013_edboard_stacoCB_all

if [ "$#" -ge "1" ]; then
    # INSTRUCTIONS ON HOW TO PREPARE A COMPLETE DATA CUTFLOW FILE
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
    for pt in 20 25; do
    	sdir=${bdir}/pt${pt}
	./max_cutflow.py ${sdir} 1 POS ${pt} > ${out}/data_pt${pt}.tex && ./max_cutflow.py ${sdir} 0 POS ${pt} >> ${out}/mc_POS_pt${pt}.tex && ./max_cutflow.py ${sdir} 0 NEG ${pt} >> ${out}/mc_NEG_pt${pt}.tex && echo OK
    done
fi
