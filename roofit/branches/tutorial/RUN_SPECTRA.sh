#!/bin/bash

# plots 1/pt spectra for mu+ and mu- muons from Z's for two different root files
# Eg, compare rel16 and rel17!

source bashmap.sh
ROOTDIR=$PWD
antondb=out1006alt
# Specify the list of tags

data0="--root0 '/share/ftkdata1/antonk/ana_v26_0930_all_stacoCB_10GeV/data_period*/root*.root'"
data1="--root1 '/share/ftkdata1/antonk/ana_v27_0930_all_stacoCB_10GeV/data_period*/root*.root'"

if [ "1" -eq "1" ]; then
    i=0
    gput tags $i default     "${data0} ${data1} --zmin 80 --zmax 100"
    gput tags $i kluit     "${data0} ${data1} --zmin 80 --zmax 100 --kluit"
    ((i++))
fi;

tts="cmb id exms"
regs="AA BB CC Bcc Baa FWC FWA MWC MWA" # "FWC0 FWC1 FWC2 FWC3 FWA0 FWA1 FWA2 FWA3"
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
	    cmd="./keysspectra.py -b --antondb ${antondb} ${opts} --tt ${tt} --region ${reg} --tag ${tag} --ndata $ndata ${xtra}"
	    echo $cmd
	    echo $cmd >> $J
	    qsub -N Spectra${i} -o ${LOG} -e ${ERR} ${J}
	    ((i++))
	done
    done
done
echo "Done up to ${i}"
