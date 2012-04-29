#!/bin/bash
source bashmap.sh
ROOTDIR=$PWD

antondb=out2012
flabel=NONE
i=0

# Specify the list of tags
for flabel in v29D_MCP2012_04162012 v2012_MCP2012_04162012; do
antondb=out2012_${flabel}
if [ "1" -eq "1" ]; then
    data="--root '/share/t3data3/antonk/ana/ana_${flabel}_stacoCB_MCPscale/data*/root_data*.root'"
    gput tags $i r17_default_staco    " --antondb ${antondb} ${data} --zmin 80 --zmax 100"
    ((i++))
    gput tags $i r17_m70110_staco     " --antondb ${antondb} ${data} --zmin 70 --zmax 110"
    ((i++))
    gput tags $i r17_klu_staco        " --antondb ${antondb} ${data} --zmin 80 --zmax 100 --kluit"
    ((i++))
    data="--root '/share/t3data3/antonk/ana/ana_${flabel}_muidCB_MCPscale/data*/root_data*.root'"
    gput tags $i r17_default_muid    " --antondb ${antondb} ${data} --zmin 80 --zmax 100"
    ((i++))
    gput tags $i r17_m70110_muid     " --antondb ${antondb} ${data} --zmin 70 --zmax 110"
    ((i++))
    gput tags $i r17_klu_muid        " --antondb ${antondb} ${data} --zmin 80 --zmax 100 --kluit"
    ((i++))
fi;
done

if [ "0" -eq "1" ]; then
    data="--root '/share/ftkdata1/antonk/ana_${flabel}_stacoCB_default/data_period*/root_data_period*.root'"
    gput tags $i r17_default_staco    " --antondb ${antondb} ${data} --zmin 80 --zmax 100"
    ((i++))
    gput tags $i r17_m70110_staco     " --antondb ${antondb} ${data} --zmin 70 --zmax 110"
    ((i++))
    gput tags $i r17_klu_staco        " --antondb ${antondb} ${data} --zmin 80 --zmax 100 --kluit"
    ((i++))
    #data="--root '/share/ftkdata1/antonk/ana_${flabel}_stacoCB_scaleDATA_KC/data_period*/root_data_period*.root'"
    #gput tags $i r17_default_staco_KC    " --antondb ${antondb} ${data} --zmin 80 --zmax 100"
    #((i++))
    
fi;

# [DEPRECATED/BROKEN] - test on Zmumu MC
if [ "0" -eq "1" ]; then
    data="--root '/share/ftkdata1/antonk/ana_${flabel}_noscale_stacoCB/mc_zmumu/root_mc_zmumu*.root'"
    gput tags $i r17_MCdefault_staco    " --antondb ${antondb} ${data} --zmin 80 --zmax 100"
    ((i++))
    gput tags $i r17_MCm70110_staco     " --antondb ${antondb} ${data} --zmin 70 --zmax 110"
    ((i++))
    gput tags $i r17_MCklu_staco        " --antondb ${antondb} ${data} --zmin 80 --zmax 100 --kluit"
    ((i++))
    data="--root '/share/ftkdata1/antonk/ana_${flabel}_noscale_muidCB/mc_zmumu/root_mc_zmumu*.root'"
    gput tags $i r17_MCdefault_muid    " --antondb ${antondb} ${data} --zmin 80 --zmax 100"
    ((i++))
    gput tags $i r17_MCm70110_muid     " --antondb ${antondb} ${data} --zmin 70 --zmax 110"
    ((i++))
    gput tags $i r17_MCklu_muid        " --antondb ${antondb} ${data} --zmin 80 --zmax 100 --kluit"
    ((i++))
fi;

tts="cmb id exms"
#regs="AA BB CC Bcc Baa FWC FWA MWC MWA" # "FWC0 FWC1 FWC2 FWC3 FWA0 FWA1 FWA2 FWA3"
regs="`echo E{0..25}E`"
regs="`echo T{0..13}T`"
regs="`echo S{0..7}S`"
regs="AA BB CC Bcc Baa" #2012
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
	    cmd="./keysfit.py -b ${opts} --tt ${tt} --region ${reg} --tag ${tag} --ndata $nevt --nscan ${nscan} --scan ${frange} ${xtra}"
	    echo $cmd
	    echo $cmd >> $J
	    qsub -N SHT${i} -o ${LOG} -e ${ERR} ${J}
	    ((i++))
	done
    done
done
echo "Done up to ${i}"
