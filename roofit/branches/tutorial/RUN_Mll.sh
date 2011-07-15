#!/bin/bash
source bashmap.sh
ROOTDIR=$PWD

antondb=out0714
# Specify the list of tags
data="--rootdata 'ROOT/root_all_0706_newiso_noscale_1fb_cmb/data_period*/root_data_period*.root'"
mc="--rootmc 'ROOT/root_all_0706_newiso_noscale_1fb_cmb/mc_zmumu/root_mc_zmumu*.root'"
data00="--rootdata 'ROOT/root_all_0706_newiso_closureqqnoscale_1fb_cmb/data_period*_00/root_data_period*.root'"
data01="--rootdata 'ROOT/root_all_0706_newiso_closureqqnoscale_1fb_cmb/data_period*_01/root_data_period*.root'"

gput tags 0 default     "${data} ${mc} --min 70 --max 110"
gput tags 1 00          "${data00} ${mc} --min 70 --max 110"
gput tags 2 01          "${data01} ${mc} --min 70 --max 110"

tts="cmb id exms"
regs="AA BB CC AB BA CB BC AC CA" # "FWC0 FWC1 FWC2 FWC3 FWA0 FWA1 FWA2 FWA3"
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
	    echo "#PBS -l cput=8:00:00" >> $J
	    echo "#PBS -l walltime=8:00:00" >> $J
	    echo "source ~/.bashrc" >> $J
	    echo "anaquick2" >> $J
	    echo "cd ${ROOTDIR}" >> $J
	    ndata=50000
	    cmd="./zkolmogorov.py -b --antondb ${antondb} ${opts} --tt ${tt} --region ${reg} --tag ${tag} --ndata $ndata ${xtra}"
	    echo $cmd
	    echo $cmd >> $J
	    qsub -N Z${i} -o LOG.${J}.out -e LOG.${J}.err ${J}
	    ((i++))
	done
    done
done
echo "Done up to ${i}"
