#!/bin/bash

# plots 1/pt spectra for mu+ and mu- muons from Z's for two different root files
# Eg, compare rel16 and rel17!

source bashmap.sh
ROOTDIR=$PWD
# Specify the list of tags

data0_rel16="--root0 '/share/ftkdata1/antonk/ana_v26_0930_all_stacoCB_10GeV/data_period*/root*.root'"
data1_rel17="--root1 '/share/ftkdata1/antonk/ana_v27_0930_all_stacoCB_10GeV/data_period*/root*.root'"

staco_data0="--root0 '/share/ftkdata1/antonk/ana_v27_1118_newpu_stacoCB_all/data_period*/root*.root'"
staco_data1kpkm="--root1 '/share/ftkdata1/antonk/ana_v27_1118_newpu_stacoCB_scaleDATA_kpkm/data_period*/root*.root'"
staco_data1KC="--root1 '/share/ftkdata1/antonk/ana_v27_1118_newpu_stacoCB_scaleDATA_KC/data_period*/root*.root'"

staco_mcnoscale0="--root0 '/share/ftkdata1/antonk/ana_v27_1118_newpu_stacoCB_noscale/mc_zmumu/root_mc_zmumu.root'"
staco_mcdefault1="--root1 '/share/ftkdata1/antonk/ana_v27_1118_newpu_stacoCB_all/mc_zmumu/root_mc_zmumu.root'"
staco_mckpkm1="--root1 '/share/ftkdata1/antonk/ana_v27_1118_newpu_stacoCB_scaleMC_kpkm/mc_zmumu/root_mc_zmumu.root'"
staco_mcKC1="--root1 '/share/ftkdata1/antonk/ana_v27_1118_newpu_stacoCB_scaleMC_KC/mc_zmumu/root_mc_zmumu.root'"


if [ "1" -eq "1" ]; then
    i=0
    gput tags $i staco_data_kpkm     "${staco_data0} ${staco_data1kpkm} --zmin 80 --zmax 100"
    ((i++))
    gput tags $i staco_data_KC       "${staco_data0} ${staco_data1KC} --zmin 80 --zmax 100"
    ((i++))
    gput tags $i staco_mc_default    "${staco_mcnoscale0} ${staco_mcdefault1} --zmin 80 --zmax 100"
    ((i++))
    #gput tags $i staco_mc_kpkm       "${staco_mcnoscale0} ${staco_mckpkm1} --zmin 80 --zmax 100"
    ((i++))
    #gput tags $i staco_mc_KC         "${staco_mcnoscale0} ${staco_mcKC1} --zmin 80 --zmax 100"
    ((i++))
fi;

tts="cmb id exms"
regs="AA BB CC Bcc Baa FWC FWA MWC MWA"
regs="`echo S{0..7}S`"
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
	    echo "anaquick2" >> $J
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
