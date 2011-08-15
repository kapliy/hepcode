#!/bin/bash
source bashmap.sh
ROOTDIR=$PWD

antondb=out0813
# Specify the list of tags
i=0
data="--root '/share/ftkdata1/antonk/ana_v26_0813_closure_stacoCB_fixmetcln_fixsht/data_period*_81400/root_data_period*.root'"
gput tags $i closurefx_81400    "${data} --zmin 80 --zmax 100"
((i++))
data="--root '/share/ftkdata1/antonk/ana_v26_0813_closure_stacoCB_fixmetcln_fixsht/data_period*_81401/root_data_period*.root'"
gput tags $i closurefx_81401    "${data} --zmin 80 --zmax 100"
((i++))
data="--root '/share/ftkdata1/antonk/ana_v26_0813_closure_stacoCB_fixmetcln_fixsht/data_period*_81402/root_data_period*.root'"
gput tags $i closurefx_81402    "${data} --zmin 80 --zmax 100"
((i++))

if [ "0" -eq "1" ]; then
    data="--root 'ROOT/current/noscale/data_period*/root_data_period*.root'"
    i=0
    gput tags $i default    "${data} --zmin 80 --zmax 100"
    ((i++))
    gput tags $i m70110     "${data} --zmin 70 --zmax 110"
    ((i++))
    gput tags $i klu        "${data} --zmin 80 --zmax 100 --kluit"
    ((i++))
    data="--root 'ROOT/current/noscale_muid/data_period*/root_data_period*.root'"
    gput tags $i muid_default    "${data} --zmin 80 --zmax 100"
    ((i++))
    gput tags $i muid_m70110     "${data} --zmin 70 --zmax 110"
    ((i++))
    gput tags $i muid_klu        "${data} --zmin 80 --zmax 100 --kluit"
    ((i++))
fi;
if [ "0" -eq "1" ]; then
    data="--root 'ROOT/current/noscale/data_period*/root_data_period*.root'"
    i=0
    gput tags $i default    "${data} --zmin 80 --zmax 100"
    ((i++))
    gput tags $i m70110     "${data} --zmin 70 --zmax 110"
    ((i++))
    gput tags $i klu        "${data} --zmin 80 --zmax 100 --kluit"
    ((i++))
    gput tags $i aklu       "${data} --zmin 80 --zmax 100 --akluit"
    ((i++))
    prenomsid="'lP_pt>20.0 && lN_pt>20.0 && lP_ptiso40<2.0 && lP_etiso40<2.0 && lN_ptiso40<2.0 && lN_etiso40<2.0 && Z_m>50 && (lP_q*lN_q)<0 && lP_idhits==1 && fabs(lP_z0)<10. && fabs(lP_d0sig)<10. && lN_idhits==1 && fabs(lN_z0)<10. && fabs(lN_d0sig)<10.'"
    gput tags $i nomsid     "${data} --zmin 80 --zmax 100 --pre ${prenomsid}"
fi;

tts="cmb id exms"
regs="AA BB CC Bcc Baa FWC FWA MWC MWA" # "FWC0 FWC1 FWC2 FWC3 FWA0 FWA1 FWA2 FWA3"
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
