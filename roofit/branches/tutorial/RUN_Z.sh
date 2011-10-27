#!/bin/bash
source bashmap.sh
ROOTDIR=$PWD

antondb=out1023L7
# Specify the list of tags
i=0

if [ "0" -eq "1" ]; then
    data="--rootdata '/share/ftkdata1/antonk/ana_v26_0930_noscale_stacoCB_10GeV/data_period*/root_data_period*.root'"
    mc="--rootmc '/share/ftkdata1/antonk/ana_v26_0930_noscale_stacoCB_10GeV/mc_zmumu/root_mc_zmumu*.root'"
    gput tags $i r16_default_staco     "${data} ${mc} --func gaus0 --min 70 --max 110"
    ((i++))
    gput tags $i r16_default_staco     "${data} ${mc} --func egge3 --min 70 --max 110"
    ((i++))
    gput tags $i r16_m80100_staco      "${data} ${mc} --func egge3 --min 80 --max 100"
    ((i++))
    data="--rootdata '/share/ftkdata1/antonk/ana_v26_0930_noscale_muidCB_10GeV/data_period*/root_data_period*.root'"
    mc="--rootmc '/share/ftkdata1/antonk/ana_v26_0930_noscale_muidCB_10GeV/mc_zmumu/root_mc_zmumu*.root'"
    gput tags $i r16_default_muid     "${data} ${mc} --func gaus0 --min 70 --max 110"
    ((i++))
    gput tags $i r16_default_muid     "${data} ${mc} --func egge3 --min 70 --max 110"
    ((i++))
    gput tags $i r16_m80100_muid      "${data} ${mc} --func egge3 --min 80 --max 100"
    ((i++))
fi;

if [ "1" -eq "1" ]; then
    data="--rootdata '/share/ftkdata1/antonk/ana_v27_1023_L7_noscale_stacoCB/data_period*/root_data_period*.root'"
    mc="--rootmc '/share/ftkdata1/antonk/ana_v27_1023_L7_noscale_stacoCB/mc_zmumu/root_mc_zmumu*.root'"
    gput tags $i r17_default_staco     "${data} ${mc} --func gaus0 --min 70 --max 110"
    ((i++))
    gput tags $i r17_default_staco     "${data} ${mc} --func egge3 --min 70 --max 110"
    ((i++))
    gput tags $i r17_m80100_staco      "${data} ${mc} --func egge3 --min 80 --max 100"
    ((i++))
    data="--rootdata '/share/ftkdata1/antonk/ana_v27_1023_L7_noscale_muidCB/data_period*/root_data_period*.root'"
    mc="--rootmc '/share/ftkdata1/antonk/ana_v27_1023_L7_noscale_muidCB/mc_zmumu/root_mc_zmumu*.root'"
    gput tags $i r17_default_muid     "${data} ${mc} --func gaus0 --min 70 --max 110"
    ((i++))
    gput tags $i r17_default_muid     "${data} ${mc} --func egge3 --min 70 --max 110"
    ((i++))
    gput tags $i r17_m80100_muid      "${data} ${mc} --func egge3 --min 80 --max 100"
    ((i++))
fi;

tts="cmb id exms"
#regs="AA BB CC Bcc Baa FWC FWA MWC MWA" # "FWC0 FWC1 FWC2 FWC3 FWA0 FWA1 FWA2 FWA3"
regs="`echo E{0..25}E`"
xtra="--ext eps"
i=0

for itag in `gkeys tags`; do
    tag=`ggeta tags $itag`
    opts=`ggetb tags $itag`
    for tt in $tts; do
	for reg in $regs; do
	    JNAME=JOB.Z.$i.$tag_$tt_$reg.sh
	    J=/home/antonk/logs/${JNAME}
	    LOG=/home/antonk/logs/LOG.${JNAME}.out
	    ERR=/home/antonk/logs/LOG.${JNAME}.err
	    rm -f $J; touch $J; chmod +x $J
	    echo "#!/bin/bash" >> $J
	    echo "#PBS -q uct3" >> $J
	    echo "#PBS -l cput=07:00:00" >> $J
	    echo "#PBS -l walltime=07:00:00" >> $J
	    echo "source ~/.bashrc" >> $J
	    echo "anaquick2" >> $J
	    echo "cd ${ROOTDIR}" >> $J
	    nmc=200000
	    ndata=200000
	    cmd="./zpeak.py -b --antondb ${antondb} ${opts} --tt ${tt} --region ${reg} --tag ${tag} --ndata $ndata --nmc $nmc ${xtra}"
	    echo $cmd
	    echo $cmd >> $J
	    qsub -N Z${i} -o ${LOG} -e ${ERR} ${J}
	    ((i++))
	done
    done
done
echo "Done up to ${i}"
