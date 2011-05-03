#!/bin/bash

ROOTDIR=$PWD
i=0

oft[0]=" "
#oft[1]="--shift"

okl[0]=""
okl[1]="KLU_"
okl[2]="AKLU_"

fnames="ROOT/root_all_0428.root"
regs="AA BB CC Bcc Baa FWC FWA MWC MWA" # "FWC0 FWC1 FWC2 FWC3 FWA0 FWA1 FWA2 FWA3"
folders[0]="data mc_zmumu"
folders[1]="data_20110425 mc_20110425zmumu"
tts="cmb id exms"

for fname in $fnames; do
    for fold in "${folders[@]}"; do
 	for tt in $tts; do
	    for reg in $regs; do
		for ikl in "${okl[@]}"; do
		    J=JOB$i.sh
		    rm -f $J; touch $J; chmod +x $J
		    echo "#!/bin/bash" >> $J
		    echo "#PBS -q uct3" >> $J
		    echo "#PBS -l cput=24:00:00" >> $J
		    echo "#PBS -l walltime=24:00:00" >> $J
		    echo "source ~/.bashrc" >> $J
		    echo "anaquick2" >> $J
		    echo "cd ${ROOTDIR}" >> $J
		    echo ./KEYS_ANA.sh ${fname} ${fold} ${tt} ${reg} ${ikl} >> $J
		    qsub -N zfits${i} -o LOG.zfits.${i}.out -e LOG.zfits.${i}.err ${J}
		    ((i++))
		done
	    done
	done
    done
done
echo "Done up to ${i}"

#./KEYS_ANA.sh ROOT/root_all_0428.root data mc_zmumu cmb BB ''
#./KEYS_ANA.sh ROOT/root_all_0428.root data_20110425 mc_20110425zmumu cmb BB 'KLU_'
