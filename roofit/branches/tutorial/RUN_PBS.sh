#!/bin/bash

ROOTDIR=$PWD
i=0

xtra="--kolmogorov --shift --kluit"
oks[0]=" "
oks[1]="--kolmogorov"

oft[0]=" "
oft[1]="--shift"

okl[0]=" "
okl[1]="--kluit"

fname=ROOT/root_all_apr26data2011BD3.root
regs="AA BB CC Bcc Baa FWC FWA MWC MWA"
folders="data_2010repro  data_20110331 data_20110425B  data_20110425D"
tts="cmb id ms exms"
for fold in $folders; do
    for tt in $tts; do
	for reg in $regs; do
	    for iok in "${oks[@]}"; do
		for ift in "${oft[@]}"; do
		    for ikl in "${okl[@]}"; do
			J=JOB$i.sh
			rm -f $J; touch $J; chmod +x $J
			echo "#!/bin/bash" >> $J
			echo "#PBS -q uct3" >> $J
			echo "#PBS -l cput=24:00:00" >> $J
			echo "#PBS -l walltime=24:00:00" >> $J
			echo "source ~/.bashrc" >> $J
			echo "anaquick" >> $J
			echo "cd ${ROOTDIR}" >> $J
			echo ./KEYS_CMB.sh ${fname} ${fold} ${tt} data ${reg} $iok $ift $ikl >> $J
			qsub -N keys${i} -o LOG${i}.out -e LOG${i}.err ${J}
			((i++))
		    done
		done
	    done
	done
    done
done
echo "Done up to ${i}"
