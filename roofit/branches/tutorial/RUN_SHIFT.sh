#!/bin/bash
source bashmap.sh
ROOTDIR=$PWD

antondb=out0721
# Specify the list of tags
data="--root 'ROOT/current/noscale/data_period*/root_data_period*.root'"
i=0
gput tags $i default    "${data} --zmin 80 --zmax 100"
((i++))
gput tags $i m70110     "${data} --zmin 70 --zmax 110"
((i++))
gput tags $i klu        "${data} --zmin 80 --zmax 100 --kluit"
((i++))
gput tags $i aklu       "${data} --zmin 80 --zmax 100 --akluit"

tts="cmb id exms"
regs="AA BB CC Bcc Baa FWC FWA MWC MWA" # "FWC0 FWC1 FWC2 FWC3 FWA0 FWA1 FWA2 FWA3"
xtra="--template --ext eps --shift"
i=0

for itag in `gkeys tags`; do
    tag=`ggeta tags $itag`
    opts=`ggetb tags $itag`
    for tt in $tts; do
	for reg in $regs; do
	    JNAME=JOB.SHT.$i.$tag_$tt_$reg.sh
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
	    nevt=1000000
	    nscan=500
	    m=1.0
	    d=0.08
	    frange="--fitmin `echo \"${m}-${d}\" | bc -l` --fitmax `echo \"${m}+${d}\" | bc -l`"
	    cmd="./keysfit.py -b --antondb ${antondb} ${opts} --tt ${tt} --region ${reg} --tag ${tag} --ndata $nevt --nscan ${nscan} --scan ${frange} ${xtra}"
	    echo $cmd
	    echo $cmd >> $J
	    qsub -N SHT${i} -o ${LOG} -e ${ERR} ${J}
	    ((i++))
	done
    done
done
echo "Done up to ${i}"