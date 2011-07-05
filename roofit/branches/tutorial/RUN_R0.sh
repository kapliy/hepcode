#!/bin/bash
source bashmap.sh
ROOTDIR=$PWD

antondb=out0630
# Specify the list of tags
data="--root 'ROOT/root_all_0630_newiso_noscale_1fb_cmb/data_period*/root_data_period*.root'"
gput tags 0 default    "${data} --zmin 80 --zmax 100"
gput tags 1 m70110     "${data} --zmin 70 --zmax 110"
gput tags 2 klu        "${data} --zmin 80 --zmax 100 --kluit"
gput tags 3 aklu       "${data} --zmin 80 --zmax 100 --akluit"

tts="cmb id exms"
regs="AA BB CC Bcc Baa FWC FWA MWC MWA" # "FWC0 FWC1 FWC2 FWC3 FWA0 FWA1 FWA2 FWA3"
xtra="--template --ext eps"
i=0

for itag in `gkeys tags`; do
    tag=`ggeta tags $itag`
    opts=`ggetb tags $itag`
    for tt in $tts; do
	for reg in $regs; do
	    J=JOB.R0.$i.$tag_$tt_$reg.sh
	    rm -f $J; touch $J; chmod +x $J
	    echo "#!/bin/bash" >> $J
	    echo "#PBS -q uct3" >> $J
	    echo "#PBS -l cput=24:00:00" >> $J
	    echo "#PBS -l walltime=24:00:00" >> $J
	    echo "source ~/.bashrc" >> $J
	    echo "anaquick2" >> $J
	    echo "cd ${ROOTDIR}" >> $J
	    nevt=1000000
	    nscan=500
	    m=1.0
	    d=0.08
	    frange="--fitmin `echo \"${m}-${d}\" | bc -l` --fitmax `echo \"${m}+${d}\" | bc -l`"
	    cmd="./keysfit.py -b --antondb ${antondb} ${opts} --tt ${tt} --region ${reg} --tag ${tag} --ndata $nevt --nscan ${nscan} --scan ${frange} ${xtra}"
	    echo $cmd
	    echo $cmd >> $J
	    qsub -N R0${i} -o LOG.${J}.out -e LOG.${J}.err ${J}
	    ((i++))
	done
    done
done
echo "Done up to ${i}"
