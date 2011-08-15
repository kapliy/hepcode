#!/bin/bash
source bashmap.sh
ROOTDIR=$PWD

antondb=out0813
# Specify the list of tags

i=0
mc="--rootmc 'ROOT/current/noscale/mc_zmumu/root_mc_zmumu*.root'"
data="--rootdata '/share/ftkdata1/antonk/ana_v26_0813_closure_stacoCB_fixmetcln_fixsht/data_period*_81400/root_data_period*.root'"
gput tags $i closurefx_81400     "${data} ${mc} --func gaus0 --min 70 --max 110"
((i++))
data="--rootdata '/share/ftkdata1/antonk/ana_v26_0813_closure_stacoCB_fixmetcln_fixsht/data_period*_81401/root_data_period*.root'"
gput tags $i closurefx_81401     "${data} ${mc} --func gaus0 --min 70 --max 110"
((i++))
data="--rootdata '/share/ftkdata1/antonk/ana_v26_0813_closure_stacoCB_fixmetcln_fixsht/data_period*_81402/root_data_period*.root'"
gput tags $i closurefx_81402     "${data} ${mc} --func gaus0 --min 70 --max 110"
((i++))

if [ "0" -eq "1" ]; then
    i=0
    data="--rootdata 'ROOT/current/noscale/data_period*/root_data_period*.root'"
    mc="--rootmc 'ROOT/current/noscale/mc_zmumu/root_mc_zmumu*.root'"
    gput tags $i default     "${data} ${mc} --func gaus0 --min 70 --max 110"
    ((i++))
    gput tags $i default     "${data} ${mc} --func egge3 --min 70 --max 110"
    ((i++))
    gput tags $i m80100      "${data} ${mc} --func egge3 --min 80 --max 100"
    ((i++))
    gput tags $i m80100      "${data} ${mc} --func egge1 --min 80 --max 100"
    ((i++))
    
    data="--rootdata 'ROOT/current/noscale_muid/data_period*/root_data_period*.root'"
    mc="--rootmc 'ROOT/current/noscale_muid/mc_zmumu/root_mc_zmumu*.root'"
    gput tags $i muid_default     "${data} ${mc} --func gaus0 --min 70 --max 110"
    ((i++))
    gput tags $i muid_default     "${data} ${mc} --func egge3 --min 70 --max 110"
    ((i++))
    gput tags $i muid_m80100      "${data} ${mc} --func egge3 --min 80 --max 100"
    ((i++))
    gput tags $i muid_m80100      "${data} ${mc} --func egge1 --min 80 --max 100"
    ((i++))
fi;

if [ "0" -eq "1" ]; then
    data="--rootdata 'ROOT/current/noscale/data_period*/root_data_period*.root'"
    mc="--rootmc 'ROOT/current/noscale/mc_zmumu/root_mc_zmumu*.root'"
    gput tags $i default     "${data} ${mc} --func gaus0 --min 70 --max 110"
    ((i++))
    gput tags $i default     "${data} ${mc} --func egge3 --min 70 --max 110"
    ((i++))
    gput tags $i m80100      "${data} ${mc} --func egge3 --min 80 --max 100"
    ((i++))
    gput tags $i m80100      "${data} ${mc} --func egge1 --min 80 --max 100"
fi

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
