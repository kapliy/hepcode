#!/bin/bash
source bashmap.sh
ROOTDIR=$PWD

antondb=out2011
flabel=NONE
i=0

# Specify the list of tags
for flabel in v29I_11212012_edboard_nophi; do
antondb=out2011_${flabel}
if [ "1" -eq "1" ]; then
    data="--root '/share/t3data3/antonk/ana/ana_${flabel}_stacoCB_MCPscale/data*/root_data*.root'"
    gput tags $i r17_powheg_pythia_default_staco    " --antondb ${antondb} ${data} --zmin 80 --zmax 100"
    ((i++))
    gput tags $i r17_powheg_pythia_m70110_staco     " --antondb ${antondb} ${data} --zmin 70 --zmax 110"
    ((i++))
    #gput tags $i r17_powheg_pythia_klu_staco        " --antondb ${antondb} ${data} --zmin 80 --zmax 100 --kluit"
    #((i++))
fi;
done

tts="cmb id exms"
#regs="AA BB CC Bcc Baa FWC FWA MWC MWA" # "FWC0 FWC1 FWC2 FWC3 FWA0 FWA1 FWA2 FWA3"
regs="`echo E{0..25}E`"
regs="`echo T{0..13}T`"
regs="AA BB CC Bcc Baa"
regs="`echo W{0..9}W`"
regs="`echo U{0..21}U`"
regs="`echo S{0..7}S`"  #2011 published
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
	    echo "anaquick5" >> $J
	    echo "cd ${ROOTDIR}" >> $J
	    nevt=1000000
	    nscan=500
	    m=1.0
	    d=0.08
	    frange="--fitmin `echo \"${m}-${d}\" | bc -l` --fitmax `echo \"${m}+${d}\" | bc -l`"
	    cmd="./keysfit.py -b ${opts} --tt ${tt} --region ${reg} --tag ${tag} --ndata $nevt --nscan ${nscan} --scan ${frange} ${xtra}"
	    echo $cmd
	    echo $cmd >> $J
	    jid=`qsub -l mem=3000mb -N SHT${i} -o ${LOG} -e ${ERR} ${J}`
	    if [ "$jid" == "" ]; then
		echo "Retrying submission: $i"
		sleep 10
		jid=`qsub -l mem=3000mb -N SHT${i} -o ${LOG} -e ${ERR} ${J}`
		if [ "$jid" == "" ]; then
		    echo "ERROR: submission ${i} failed"
		fi
	    fi
	    ((i++))
	done
    done
done
echo "Done up to ${i}"
