#!/bin/bash

ROOTDIR=$PWD
i=0

oft[0]=" "
#oft[1]="--shift"

okl[0]=""
okl[1]="KLU_"
okl[2]="AKLU_"

regs="AA BB CC Bcc Baa FWC FWA MWC MWA" # "FWC0 FWC1 FWC2 FWC3 FWA0 FWA1 FWA2 FWA3"

fnames="ROOT/root_0505_closure.root"
folders[0]="data mc_zmumu"
folders[1]="data_20110425 mc_20110425zmumu"
#folders[2]="data_scaled_00 mc_zmumu"
#folders[3]="data_scaled_11 mc_zmumu"
#folders[4]="data_20110425_scaled_00 mc_20110425zmumu"
#folders[5]="data_20110425_scaled_11 mc_20110425zmumu"

tts="cmb id exms"

for fname in $fnames; do
    for fold in "${folders[@]}"; do
 	for tt in $tts; do
	    for reg in $regs; do
		for ikl in "${okl[@]}"; do
		    tag="${ikl}`basename ${fname}`_`echo ${fold} | cut -d ' ' -f 1`_${tt}_${reg}"
		    J=JOB.ANA.$i.$tag.sh
		    rm -f $J; touch $J; chmod +x $J
		    echo "#!/bin/bash" >> $J
		    echo "#PBS -q uct3" >> $J
		    echo "#PBS -l cput=24:00:00" >> $J
		    echo "#PBS -l walltime=24:00:00" >> $J
		    echo "source ~/.bashrc" >> $J
		    echo "anaquick2" >> $J
		    echo "cd ${ROOTDIR}" >> $J
		    echo ./KEYS_ANA.sh ${fname} ${fold} ${tt} ${reg} ${ikl} >> $J
		    qsub -N ANA${i} -o LOG.ANA.${i}.$tag.out -e LOG.ANA.${i}.$tag.err ${J}
		    ((i++))
		done
	    done
	done
    done
done
echo "Done up to ${i}"
