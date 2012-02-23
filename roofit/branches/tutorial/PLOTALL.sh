#!/bin/bash

cut="mcw*puw"
input=/share/ftkdata1/antonk/ana_v28HC_02092012_DtoM_cernupd_stacoCB_all
tag=FEB14

#for m in wstack zstack truth unfold asym; do
for m in unfold asym wstack; do
    echo "Starting ${m}"
    ./TODAY.sh -i ${input} -o ${tag}_${m} -m ${m} -c "${cut}" &> LOG.${tag}.${m}
    echo "Ending ${m}"
done;
