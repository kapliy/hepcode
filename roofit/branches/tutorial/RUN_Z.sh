#!/bin/bash
source bashmap.sh
ROOTDIR=$PWD

antondb=out0721
# Specify the list of tags
data="--rootdata 'ROOT/current/noscale/data_period*/root_data_period*.root'"
mc="--rootmc 'ROOT/current/noscale/mc_zmumu/root_mc_zmumu*.root'"
i=0
gput tags $i default     "${data} ${mc} --func gaus0 --min 70 --max 110"
((i++))
gput tags $i default     "${data} ${mc} --func egge3 --min 70 --max 110"
((i++))
gput tags $i m80100      "${data} ${mc} --func egge3 --min 80 --max 100"
((i++))
gput tags $i m80100      "${data} ${mc} --func egge1 --min 80 --max 100"

tts="cmb id exms"
regs="AA BB CC Bcc Baa FWC FWA MWC MWA" # "FWC0 FWC1 FWC2 FWC3 FWA0 FWA1 FWA2 FWA3"
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
	    nmc=100000
	    ndata=500000
	    cmd="./zpeak.py -b --antondb ${antondb} ${opts} --tt ${tt} --region ${reg} --tag ${tag} --ndata $ndata --nmc $nmc ${xtra}"
	    echo $cmd
	    echo $cmd >> $J
	    qsub -N Z${i} -o ${LOG} -e ${ERR} ${J}
	    ((i++))
	done
    done
done
echo "Done up to ${i}"
