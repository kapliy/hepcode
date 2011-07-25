#!/bin/bash

# makes a standard set of plots using stack.py scripts

source bashmap.sh
ROOTDIR=$PWD

antondb=out0721

# Specify the list of tags (e.g., preselection cuts)
i=0
input="ROOT/current/all"

pre="'l_pt>20.0 && ptiso40<2.0 && etiso40<2.0 && met>25.0 && w_mt>40.0 && idhits==1 && fabs(z0)<10. && fabs(d0sig)<10. && fabs(l_pt_id-l_pt_exms)/l_pt_id<0.5'"
gput tags ${i} peter_20GeV    "--input ${input} --pre ${pre}"
((i++))
pre="'l_pt>20.0 && ptiso40<2.0 && etiso40<2.0 && met>25.0 && w_mt>40.0 && idhits==1 && fabs(z0)<10. && fabs(d0sig)<10.'"
gput tags ${i} peter_20GeV_nomsid    "--input ${input} --pre ${pre}"
((i++))
pre="'l_pt>25.0 && ptiso40<2.0 && etiso40<2.0 && met>25.0 && w_mt>40.0 && idhits==1 && fabs(z0)<10. && fabs(d0sig)<10. && fabs(l_pt_id-l_pt_exms)/l_pt_id<0.5'"
gput tags ${i} peter_25GeV     "--input ${input} --pre ${pre}"
((i++))
pre="'l_pt>25.0 && ptiso20/l_pt<0.1 && met>25.0 && w_mt>40.0 && idhits==1 && fabs(z0)<10. && fabs(d0sig)<10. && fabs(l_pt_id-l_pt_exms)/l_pt_id<0.5'"
gput tags ${i} jordan_25GeV   "--input ${input} --pre ${pre}"
((i++))
pre="'l_pt>20.0 && ptiso20/l_pt<0.1 && met>25.0 && w_mt>40.0 && idhits==1 && fabs(z0)<10. && fabs(d0sig)<10. && fabs(l_pt_id-l_pt_exms)/l_pt_id<0.5'"
gput tags ${i} jordan_20GeV   "--input ${input} --pre ${pre}"
((i++))
pre="'l_pt>25.0 && ptiso20/l_pt<0.1 && met>25.0 && w_mt>40.0 && idhits==1 && fabs(z0)<10. && fabs(d0sig)<10. && fabs(l_pt_id-l_pt_exms)/l_pt_id<0.5'"
gput tags ${i} jordan_25GeV_NLO   "--input ${input} --pre ${pre} --bgsig 1"
((i++))
pre="'l_pt>25.0 && ptiso20/l_pt<0.1 && met>25.0 && w_mt>40.0 && idhits==1 && fabs(z0)<10. && fabs(d0sig)<10. && fabs(l_pt_id-l_pt_exms)/l_pt_id<0.5'"
gput tags ${i} jordan_25GeV_JIMMY   "--input ${input} --pre ${pre} --bgsig 3"

# Specify the list of plots
i=0
tts[((i++))]="-m1 --var 'l_eta' --bin '50,-2.5,2.5'"
tts[((i++))]="-m1 --var 'l_pt' --bin '50,0,200'"
tts[((i++))]="-m1 --var 'met' --bin '50,0,200'"
tts[((i++))]="-m1 --var 'w_mt' --bin '50,0,200'"
tts[((i++))]="-m1 --var 'w_pt' --bin '50,0,200'"
tts[((i++))]="-m1 --var 'njets' --bin '10,0,10'"
tts[((i++))]="-m1 --var 'ptiso20/l_pt' --bin '100,0,0.1'"
tts[((i++))]="-m1 --var 'ptiso40' --bin '100,0,2.0'"
tts[((i++))]="-m1 --var 'z0' --bin '100,-10,10'"
tts[((i++))]="-m1 --var 'd0' --bin '100,-2,2'"
tts[((i++))]="-m1 --var 'd0sig' --bin '100,-10,10'"
tts[((i++))]="-m11 --var 'l_eta' --bin '50,-2.5,2.5'"
tts[((i++))]="-m11 --var 'fabs(l_eta)' --bin '50,0,2.5'"

# Charges
qs="0 1 2"

xtra="--template --ext eps"
i=0

for itag in `gkeys tags`; do
    tag=`ggeta tags $itag`
    opts=`ggetb tags $itag`
    for tt in "${tts[@]}"; do
	for q in $qs; do
	    JNAME=JOB.PL.$i.$tag_$tt_$q.sh
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
	    cmd="./stack.py -b -t ${tag} --antondb ${antondb} ${tt} -q ${q} ${opts}"
	    echo $cmd
	    echo $cmd >> $J
	    qsub -N PL${i} -o ${LOG} -e ${ERR} ${J}
	    ((i++))
	done
    done
done
echo "Done up to ${i}"
