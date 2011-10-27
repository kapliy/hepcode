#!/bin/bash
source bashmap.sh
ROOTDIR=$PWD

antondb=out1023L7
# Specify the list of tags
i=0

if [ "0" -eq "1" ]; then
    data="--root '/share/ftkdata1/antonk/ana_v26_0930_noscale_stacoCB_10GeV/data_period*/root_data_period*.root'"
    gput tags $i r16_default_staco    "${data} --zmin 80 --zmax 100"
    ((i++))
    gput tags $i r16_m70110_staco     "${data} --zmin 70 --zmax 110"
    ((i++))
    gput tags $i r16_klu_staco        "${data} --zmin 80 --zmax 100 --kluit"
    ((i++))
    data="--root '/share/ftkdata1/antonk/ana_v26_0930_noscale_muidCB_10GeV/data_period*/root_data_period*.root'"
    gput tags $i r16_default_muid    "${data} --zmin 80 --zmax 100"
    ((i++))
    gput tags $i r16_m70110_muid     "${data} --zmin 70 --zmax 110"
    ((i++))
    gput tags $i r16_klu_muid        "${data} --zmin 80 --zmax 100 --kluit"
    ((i++))
fi;

if [ "1" -eq "1" ]; then
    data="--root '/share/ftkdata1/antonk/ana_v27_1023_L7_noscale_stacoCB/data_period*/root_data_period*.root'"
    gput tags $i r17_default_staco    "${data} --zmin 80 --zmax 100"
    ((i++))
    gput tags $i r17_m70110_staco     "${data} --zmin 70 --zmax 110"
    ((i++))
    gput tags $i r17_klu_staco        "${data} --zmin 80 --zmax 100 --kluit"
    ((i++))
    data="--root '/share/ftkdata1/antonk/ana_v27_1023_L7_noscale_muidCB/data_period*/root_data_period*.root'"
    gput tags $i r17_default_muid    "${data} --zmin 80 --zmax 100"
    ((i++))
    gput tags $i r17_m70110_muid     "${data} --zmin 70 --zmax 110"
    ((i++))
    gput tags $i r17_klu_muid        "${data} --zmin 80 --zmax 100 --kluit"
    ((i++))
fi;

tts="cmb id exms"
#regs="AA BB CC Bcc Baa FWC FWA MWC MWA" # "FWC0 FWC1 FWC2 FWC3 FWA0 FWA1 FWA2 FWA3"
regs="`echo E{0..25}E`"
xtra="--template --ext eps"
i=0

for itag in `gkeys tags`; do
    tag=`ggeta tags $itag`
    opts=`ggetb tags $itag`
    for tt in $tts; do
	for reg in $regs; do
	    JNAME=JOB.R0.$i.$tag_$tt_$reg.sh
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
	    qsub -N R0${i} -o ${LOG} -e ${ERR} ${J}
	    ((i++))
	done
    done
done
echo "Done up to ${i}"
