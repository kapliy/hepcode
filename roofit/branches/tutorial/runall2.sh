fnames="ROOT/root_all_0522_dataMCclosure.root"
funcs="gaus"
regs="ALL AA BB CC"
tts="cmb id exms"

i=0
folders[((i++))]="data mc_zmumu"
#folders[((i++))]="data mc_zmumu_00"
folders[((i++))]="data mc_zmumuO"
folders[((i++))]="data mc_zmumuO_00"
folders[((i++))]="data mc_zmumuO_22"

for tt in $tts; do
    ntlbl="_${tt}"
    if [ "$tt" == "cmb" ]; then ntlbl=""; fi;
    for reg in $regs; do
	for fname in $fnames; do
	    for func in $funcs; do
		for fold in "${folders[@]}"; do
		    data=`echo ${fold} | cut -d ' ' -f 1`
		    mc=`echo ${fold} | cut -d ' ' -f 2`
		    ./zkolmogorov.py -b --root ${fname} --region ${reg} --data ${data}_${data}.root/dg/dg/st_z_final/ntuple${ntlbl} --mc ${mc}_${mc}.root/dg/dg/st_z_final/ntuple${ntlbl} --min 70 --max 110 -t KS_${tt}_${reg}_${data}_${mc}
		done
	    done
	done
    done
done
