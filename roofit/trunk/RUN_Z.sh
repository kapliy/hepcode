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
    data="--rootdata '/share/t3data3/antonk/ana/ana_${flabel}_stacoCB_MCPscale/data*/root_data*.root'"
    mc1="--rootmc '/share/t3data3/antonk/ana/ana_${flabel}_stacoCB_MCPscale/mc_pythia_zmumu/root_mc*.root'"
    mc2="--rootmc '/share/t3data3/antonk/ana/ana_${flabel}_stacoCB_MCPscale/mc_powheg_pythia_zmumu/root_mc*.root'"
    mc3="--rootmc '/share/t3data3/antonk/ana/ana_${flabel}_stacoCB_MCPscale/mc_powheg_herwig_zmumu/root_mc*.root'"
    #mc4="--rootmc '/share/t3data3/antonk/ana/ana_${flabel}_stacoCB_MCPscale/mc_mcnlo_zmumu/root_mc*.root'"
    # PYTHIA
    gput tags $i r17_pythia_default_staco     " --antondb ${antondb} ${data} ${mc1} --func gaus0 --min 70 --max 110"
    ((i++))
    gput tags $i r17_pythia_default_staco     " --antondb ${antondb} ${data} ${mc1} --func egge3 --min 70 --max 110"
    ((i++))
    gput tags $i r17_pythia_m80100_staco      " --antondb ${antondb} ${data} ${mc1} --func egge3 --min 80 --max 100"
    ((i++))
    # POWHEG+PYTHIA
    gput tags $i r17_powheg_pythia_default_staco     " --antondb ${antondb} ${data} ${mc2} --func gaus0 --min 70 --max 110"
    ((i++))
    gput tags $i r17_powheg_pythia_default_staco     " --antondb ${antondb} ${data} ${mc2} --func egge3 --min 70 --max 110"
    ((i++))
    gput tags $i r17_powheg_pythia_m80100_staco      " --antondb ${antondb} ${data} ${mc2} --func egge3 --min 80 --max 100"
    ((i++))
    # POWHEG+HERWIG
    gput tags $i r17_powheg_herwig_default_staco     " --antondb ${antondb} ${data} ${mc3} --func gaus0 --min 70 --max 110"
    ((i++))
    gput tags $i r17_powheg_herwig_default_staco     " --antondb ${antondb} ${data} ${mc3} --func egge3 --min 70 --max 110"
    ((i++))
    gput tags $i r17_powheg_herwig_m80100_staco      " --antondb ${antondb} ${data} ${mc3} --func egge3 --min 80 --max 100"
    ((i++))
fi
done

tts="cmb id exms"
#regs="AA BB CC Bcc Baa FWC FWA MWC MWA" # "FWC0 FWC1 FWC2 FWC3 FWA0 FWA1 FWA2 FWA3"
regs="`echo E{0..25}E`"
regs="`echo T{0..13}T`"
regs="AA BB CC Bcc Baa" #2012
regs="`echo W{0..9}W`"
regs="`echo U{0..21}U`"
regs="`echo S{0..7}S`"  #2011 published
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
	    echo "anaquick5" >> $J
	    echo "cd ${ROOTDIR}" >> $J
	    nmc=200000
	    ndata=200000
	    nmc=500000
	    ndata=500000
	    cmd="./zpeak.py -b ${opts} --tt ${tt} --region ${reg} --tag ${tag} --ndata $ndata --nmc $nmc ${xtra}"
	    echo $cmd
	    echo $cmd >> $J
	    jid=`qsub -l mem=3000mb -N Z${i} -o ${LOG} -e ${ERR} ${J}`
	    if [ "$jid" == "" ]; then
		echo "Retrying submission: $i"
		sleep 10
		jid=`qsub -l mem=3000mb -N Z${i} -o ${LOG} -e ${ERR} ${J}`
		if [ "$jid" == "" ]; then
		    echo "ERROR: submission ${i} failed"
		fi
	    fi
	    ((i++))
	done
    done
done
echo "Done up to ${i}"
