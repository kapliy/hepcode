#!/bin/bash
source bashmap.sh
ROOTDIR=$PWD
antondb=out1003
# Specify the list of tags

data="--rootdata '/share/ftkdata1/antonk/ana_v26_0930_noscale_stacoCB_10GeV/data_period*/root_data_period*.root'"
mc="--rootmc '/share/ftkdata1/antonk/ana_v26_0930_noscale_stacoCB_10GeV/mc_zmumu/root_mc_zmumu*.root'"

if [ "1" -eq "1" ]; then
    dataMCP="--rootdata 'ROOT/current/all/data_period*/root_data_period*.root'"
    data="--rootdata 'ROOT/current/noscale/data_period*/root_data_period*.root'"
    mcMCP="--rootmc 'ROOT/current/all/mc_zmumu/root_mc_zmumu*.root'"
    mc="--rootmc 'ROOT/current/noscale/mc_zmumu/root_mc_zmumu*.root'"
    dataSHT00="--rootdata 'ROOT/current/closure/data_period*_72000/root_data_period*.root'"
    dataSHT10="--rootdata 'ROOT/current/closure/data_period*_72010/root_data_period*.root'"
    dataSC00="--rootdata 'ROOT/current/closure/data_period*_7201100/root_data_period*.root'"
    dataSC10="--rootdata 'ROOT/current/closure/data_period*_7201110/root_data_period*.root'"
    i=0
    gput tags $i default     "${data} ${mc} --min 70 --max 110"
    ((i++))
fi;

tts="cmb id exms"
regs="AA BB CC AB BA CB BC AC CA" # "FWC0 FWC1 FWC2 FWC3 FWA0 FWA1 FWA2 FWA3"
regs="AA BB CC" # "FWC0 FWC1 FWC2 FWC3 FWA0 FWA1 FWA2 FWA3"
xtra="--ext eps"
i=0

for itag in `gkeys tags`; do
    tag=`ggeta tags $itag`
    opts=`ggetb tags $itag`
    for tt in $tts; do
	for reg in $regs; do
	    JNAME=JOB.Mll.$i.$tag_$tt_$reg.sh
	    J=/home/antonk/logs/${JNAME}
	    LOG=/home/antonk/logs/LOG.${JNAME}.out
	    ERR=/home/antonk/logs/LOG.${JNAME}.err
 	    rm -f $J; touch $J; chmod +x $J
	    echo "#!/bin/bash" >> $J
	    echo "#PBS -q uct3" >> $J
	    echo "#PBS -l cput=8:00:00" >> $J
	    echo "#PBS -l walltime=8:00:00" >> $J
	    echo "source ~/.bashrc" >> $J
	    echo "anaquick2" >> $J
	    echo "cd ${ROOTDIR}" >> $J
	    ndata=100000
	    cmd="./zkolmogorov.py -b --antondb ${antondb} ${opts} --tt ${tt} --region ${reg} --tag ${tag} --ndata $ndata ${xtra}"
	    echo $cmd
	    echo $cmd >> $J
	    qsub -N Mll${i} -o ${LOG} -e ${ERR} ${J}
	    ((i++))
	done
    done
done
echo "Done up to ${i}"
