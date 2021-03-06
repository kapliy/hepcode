#!/bin/bash

cut="mcw*puw*effw*trigw"
input=/share/ftkdata1/antonk/ana_v28HC_02092012_DtoM_cernupd_stacoCB_all
input=/share/ftkdata1/antonk/ana_v29D_02222012_DtoM_pdfw_stacoCB_all
input=/share/t3data3/antonk/ana/ana_v29D_03092012_DtoM_64bit_noc022_stacoCB_all
input=/share/t3data3/antonk/ana/ana_v29D_03102012_DtoM_64bit_fixMETrecomp_stacoCB_all
tag=MAR11_TRUTHPLOTS

input=/share/t3data3/antonk/ana/ana_v29D_03132012_DtoM_64bit_allsyst_withsoftjup_stacoCB_all
tag=MAR14_ALLSYST_UPDATE

input=/share/t3data3/antonk/ana/ana_v29D_03152012_DtoM_64bit_allsyst_stacoCB_all
tag=MAR18_ALLSYST_UPDATE

input=/share/t3data3/antonk/ana/ana_v29D_04092012_DtoM_64bit_unfold1d_etaslices_stacoCB_all
tag=APR10_REPLOT

input=/share/t3data3/antonk/ana/ana_v29D_04112012_DtoM_64bit_unfold1d_etaslices_numet_stacoCB_all
tag=APR10_NUMET

for m in sig_reco wstack zstack truth unfold asym; do
    echo "Starting ${m}"
    ./TODAY.sh -i ${input} -o ${tag}_${m} -m ${m} -c "${cut}" &> LOG.${tag}.${m}
    echo "Ending ${m}"
done;
