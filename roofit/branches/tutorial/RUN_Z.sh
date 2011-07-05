#!/bin/bash
source bashmap.sh
ROOTDIR=$PWD

antondb=out0630
# Specify the list of tags
data="--rootdata 'ROOT/root_all_0630_newiso_noscale_1fb_cmb/data_period*/root_data_period*.root'"
mc="--rootmc 'ROOT/root_all_0630_newiso_noscale_1fb_cmb/mc_zmumu/root_mc_zmumu*.root'"
gput tags 0 default     "${data} ${mc} --func gaus0 --min 70 --max 110"
gput tags 1 default     "${data} ${mc} --func egge3 --min 70 --max 110"
gput tags 2 m80100      "${data} ${mc} --func egge3 --min 80 --max 100"
gput tags 3 m80100      "${data} ${mc} --func egge1 --min 80 --max 100"

tts="cmb id exms"
regs="AA BB CC Bcc Baa FWC FWA MWC MWA" # "FWC0 FWC1 FWC2 FWC3 FWA0 FWA1 FWA2 FWA3"
xtra="--ext eps"
i=0

for itag in `gkeys tags`; do
    tag=`ggeta tags $itag`
    opts=`ggetb tags $itag`
    for tt in $tts; do
	for reg in $regs; do
	    J=JOB.Z.$i.$tag_$tt_$reg.sh
	    rm -f $J; touch $J; chmod +x $J
	    echo "#!/bin/bash" >> $J
	    echo "#PBS -q uct3" >> $J
	    echo "#PBS -l cput=24:00:00" >> $J
	    echo "#PBS -l walltime=24:00:00" >> $J
	    echo "source ~/.bashrc" >> $J
	    echo "anaquick2" >> $J
	    echo "cd ${ROOTDIR}" >> $J
	    nmc=100000
	    ndata=500000
	    cmd="./zpeak.py -b --antondb ${antondb} ${opts} --tt ${tt} --region ${reg} --tag ${tag} --ndata $ndata --nmc $nmc ${xtra}"
	    echo $cmd
	    echo $cmd >> $J
	    qsub -N Z${i} -o LOG.${J}.out -e LOG.${J}.err ${J}
	    ((i++))
	done
    done
done
echo "Done up to ${i}"
