#!/bin/bash

input=/share/ftkdata1/antonk/ana_v28HC_02042012_DtoM_cernupd_hforfix_trig40_stacoCB_all
input=/share/ftkdata1/antonk/ana_v28HC_02042012_DtoM_cernupd_hforfix_stacoCB_all
tag=FEB09

#for m in wstack zstack truth unfold asym; do
for m in wstack; do
    echo "Starting ${m}"
    ./TODAY.sh -i ${input} -o ${tag}_${m} -m ${m} &> LOG.${tag}.${m}
    echo "Ending ${m}"
done;
