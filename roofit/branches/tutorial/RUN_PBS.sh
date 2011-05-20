#!/bin/bash

ROOTDIR=$PWD
i=0

oft[0]=" "
#oft[1]="--shift"

okl[0]=" "
okl[1]="--kluit"
#okl[2]="--akluit"

xtra="--template --ext eps"
regs="AA BB CC Bcc Baa FWC FWA MWC MWA" # "FWC0 FWC1 FWC2 FWC3 FWA0 FWA1 FWA2 FWA3"

fnames="ROOT/root_all_0516_2010clos.root"
folders="data"

fnames="ROOT/root_all_0519_dataMCclosure.root"
folders="data data_00"

tts="cmb id exms"
for fname in $fnames; do
    for fold in $folders; do
	for tt in $tts; do
	    for reg in $regs; do
		for ift in "${oft[@]}"; do
		    for ikl in "${okl[@]}"; do
			tag="`basename ${fname}`_${fold}_${tt}_${reg}"
			echo $ikl | grep -v akluit | grep -q kluit && tag="KLU_${tag}"
			echo $ikl | grep -q akluit && tag="AKLU_${tag}"
			J=JOB.PBS.$i.$tag.sh
			rm -f $J; touch $J; chmod +x $J
			echo "#!/bin/bash" >> $J
			echo "#PBS -q uct3" >> $J
			echo "#PBS -l cput=24:00:00" >> $J
			echo "#PBS -l walltime=24:00:00" >> $J
			echo "source ~/.bashrc" >> $J
			echo "anaquick2" >> $J
			echo "cd ${ROOTDIR}" >> $J
			echo ./KEYS_CMB.sh ${fname} ${fold} ${tt} data ${reg} $ift $ikl ${xtra} >> $J
			qsub -N keys${i} -o LOG.keys.${i}.$tag.out -e LOG.keys.${i}.$tag.err ${J}
			((i++))
		    done
		done
	    done
	done
    done
done
echo "Done up to ${i}"
