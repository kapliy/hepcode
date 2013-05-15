#!/bin/bash

# plots 1/pt spectra for mu+ and mu- muons from Z's for two different root files
# formerly called RUN_SPECTRA_LOCAL.sh

source bashmap.sh
ROOTDIR=$PWD
# Specify the list of tags

data0_rel16="--root0 '/share/ftkdata1/antonk/ana_v26_0930_all_stacoCB_10GeV/data_period*/root*.root'"
data1_rel17="--root1 '/share/ftkdata1/antonk/ana_v27_0930_all_stacoCB_10GeV/data_period*/root*.root'"

staco_data0="--root0 '/share/ftkdata1/antonk/ana_v28_1128_BtoM_stacoCB_default/data_period[BDEFGHIJK]*/root*.root'"
staco_data1KC="--root1 '/share/ftkdata1/antonk/ana_v28_1128_BtoM_stacoCB_scaleDATA_KC/data_period[BDEFGHIJK]*/root*.root'"

staco_data0indep="--root0 '/share/ftkdata1/antonk/ana_v28_1128_BtoM_stacoCB_default/data_period[LM]*/root*.root'"
staco_data1KCindep="--root1 '/share/ftkdata1/antonk/ana_v28_1128_BtoM_stacoCB_scaleDATA_KC/data_period[LM]*/root*.root'"

staco_mcnoscale0="--root0 '/share/ftkdata1/antonk/ana_v28_1128_BtoM_stacoCB_noscale/mc_zmumu/root_mc_zmumu.root'"
staco_mcdefault1="--root1 '/share/ftkdata1/antonk/ana_v28_1128_BtoM_stacoCB_default/mc_zmumu/root_mc_zmumu.root'"
staco_mcKC1="--root1 '/share/ftkdata1/antonk/ana_v28_1128_BtoM_stacoCB_scaleMC_KC/mc_zmumu/root_mc_zmumu.root'"

#2012/2011 data comparison:
flabel=v29D_MCP2012_04162012
staco_2011="--root0 '/share/t3data3/antonk/ana/ana_${flabel}_stacoCB_MCPscale/data*/root*.root'"
muid_2011="--root0 '/share/t3data3/antonk/ana/ana_${flabel}_muidCB_MCPscale/data*/root*.root'"
flabel=v2012_MCP2012_04162012
staco_2012="--root1 '/share/t3data3/antonk/ana/ana_${flabel}_stacoCB_MCPscale/data*/root*.root'"
muid_2012="--root1 '/share/t3data3/antonk/ana/ana_${flabel}_muidCB_MCPscale/data*/root*.root'"

if [ "1" -eq "1" ]; then
    flabel=v29D_MCP2012_04162012
    staco_2011="--root0 '/share/t3data3/antonk/ana/ana_${flabel}_stacoCB_MCPscale/data*/root*.root'"
    muid_2011="--root0 '/share/t3data3/antonk/ana/ana_${flabel}_muidCB_MCPscale/data*/root*.root'"
    flabel=v2012_MCP2012_04162012
    staco_2012="--root1 '/share/t3data3/antonk/ana/ana_${flabel}_stacoCB_MCPscale/data*/root*.root'"
    muid_2012="--root1 '/share/t3data3/antonk/ana/ana_${flabel}_muidCB_MCPscale/data*/root*.root'"

    i=0
    gput tags $i staco_2011_2012      "${staco_2011} ${staco_2012} --zmin 80 --zmax 100 --label0 \"STACO, 2011 data\" --label1 \"STACO, 2012 data\" "
    ((i++))
    gput tags $i muid_2011_2012       "${muid_2011} ${muid_2012} --zmin 80 --zmax 100 --label0 \"MuID, 2011 data\" --label1 \"MuID, 2012 data\" "
    ((i++))
    flabel=v29D_MCP2012_04162012
    staco_2011="--root1 '/share/t3data3/antonk/ana/ana_${flabel}_stacoCB_MCPscale/data*/root*.root'"
    muid_2011="--root0 '/share/t3data3/antonk/ana/ana_${flabel}_muidCB_MCPscale/data*/root*.root'"
    flabel=v2012_MCP2012_04162012
    staco_2012="--root1 '/share/t3data3/antonk/ana/ana_${flabel}_stacoCB_MCPscale/data*/root*.root'"
    muid_2012="--root0 '/share/t3data3/antonk/ana/ana_${flabel}_muidCB_MCPscale/data*/root*.root'"
    gput tags $i muid_staco_2012      "${muid_2012} ${staco_2012} --zmin 80 --zmax 100 --label0 \"MuID, 2012 data\" --label1 \"STACO, 2012 data\" "
    ((i++))
    gput tags $i muid_staco_2011      "${muid_2011} ${staco_2011} --zmin 80 --zmax 100 --label0 \"MuID, 2011 data\" --label1 \"STACO, 2011 data\" "
    ((i++))
fi;

# validation of independent sample (second 2/fb from 2011 data)
if [ "0" -eq "1" ]; then
    i=0
    gput tags $i staco_data_KC       "${staco_data0} ${staco_data1KC} --zmin 80 --zmax 100"
    ((i++))
    gput tags $i staco_data_indepKC       "${staco_data0indep} ${staco_data1KCindep} --zmin 80 --zmax 100"
    ((i++))
    #gput tags $i staco_mc_default    "${staco_mcnoscale0} ${staco_mcdefault1} --zmin 80 --zmax 100"
    ((i++))
    #gput tags $i staco_mc_KC         "${staco_mcnoscale0} ${staco_mcKC1} --zmin 80 --zmax 100"
    ((i++))
fi;

tts="cmb id exms"
regs="AA BB CC Bcc Baa FWC FWA MWC MWA"
regs="`echo S{0..7}S`"
regs="AA BB CC Bcc Baa"
xtra=""
i=0

for itag in `gkeys tags`; do
    tag=`ggeta tags $itag`
    opts=`ggetb tags $itag`
    for tt in $tts; do
	for reg in $regs; do
	    JNAME=JOB.SPECTRA.$i.$tag_$tt_$reg.sh
	    J=/home/antonk/logs/${JNAME}
	    LOG=/home/antonk/logs/LOG.${JNAME}.out
	    ERR=/home/antonk/logs/LOG.${JNAME}.err
 	    rm -f $J; touch $J; chmod +x $J
	    echo "#!/bin/bash" >> $J
	    echo "#PBS -q uct3" >> $J
	    echo "#PBS -l cput=18:00:00" >> $J
	    echo "#PBS -l walltime=18:00:00" >> $J
	    echo "source ~/.bashrc" >> $J
	    echo "anaquick5" >> $J
	    echo "cd ${ROOTDIR}" >> $J
	    ndata=100000
	    cmd="./keysspectra.py -b ${opts} --tt ${tt} --region ${reg} --tag ${tag} --ndata $ndata ${xtra}"
	    eval $cmd &
	    ((i++))
	done
    done
    wait
done
wait
echo "Done up to ${i}"
