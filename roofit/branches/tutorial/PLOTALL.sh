#!/bin/bash

cut="mcw*puw*effw*trigw"
input=/share/ftkdata1/antonk/ana_v28HC_02092012_DtoM_cernupd_stacoCB_all
input=/share/ftkdata1/antonk/ana_v29D_02222012_DtoM_pdfw_stacoCB_all
tag=FEB28

#for m in sig_reco wstack zstack truth unfold asym; do
for m in truth asym; do
    echo "Starting ${m}"
    ./TODAY.sh -i ${input} -o ${tag}_${m} -m ${m} -c "${cut}" &> LOG.${tag}.${m}
    echo "Ending ${m}"
done;
