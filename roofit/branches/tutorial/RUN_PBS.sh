#!/bin/bash

ROOTDIR=$PWD
i=0

oft[0]=" "
#oft[1]="--shift"

okl[0]=" "
okl[1]="--kluit"
#okl[2]="--akluit"

xtra="--template"
regs="AA BB CC Bcc Baa FWC FWA MWC MWA" # "FWC0 FWC1 FWC2 FWC3 FWA0 FWA1 FWA2 FWA3"

fnames="ROOT/root_0505_closure.root"
folders="data data_scaled_00 data_scaled_11 data_scaled_99 data_20110425 data_20110425_scaled_00 data_20110425_scaled_01 data_20110425_scaled_11"

tts="cmb id exms"
for fname in $fnames; do
    for fold in $folders; do
	for tt in $tts; do
	    for reg in $regs; do
		for ift in "${oft[@]}"; do
		    for ikl in "${okl[@]}"; do
			J=JOB.PBS.$i.sh
			rm -f $J; touch $J; chmod +x $J
			echo "#!/bin/bash" >> $J
			echo "#PBS -q uct3" >> $J
			echo "#PBS -l cput=24:00:00" >> $J
			echo "#PBS -l walltime=24:00:00" >> $J
			echo "source ~/.bashrc" >> $J
			echo "anaquick2" >> $J
			echo "cd ${ROOTDIR}" >> $J
			echo ./KEYS_CMB.sh ${fname} ${fold} ${tt} data ${reg} $ift $ikl ${xtra} >> $J
			qsub -N keys${i} -o LOG.keys.${i}.out -e LOG.keys.${i}.err ${J}
			((i++))
		    done
		done
	    done
	done
    done
done
echo "Done up to ${i}"
