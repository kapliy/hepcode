funcs[0]="egge3"
funcs[1]="gaus0"

fnames="root_all_0516_2010clos.root"
folders[0]="data mc_zmumu"
folders[1]="data mc_zmumu_00"

fnames="root_all_0522_dataMCclosure.root"
i=0
folders[((i++))]="data mc_zmumu"
folders[((i++))]="data_00 mc_zmumu"
folders[((i++))]="data_00 mc_zmumuO"
folders[((i++))]="data_33 mc_zmumuO"
folders[((i++))]="data mc_zmumuO"
folders[((i++))]="data mc_zmumu_00"
folders[((i++))]="data mc_zmumuO_00"
folders[((i++))]="data mc_zmumuO_22"

for fname in $fnames; do
    for fold in "${folders[@]}"; do
	for func in "${funcs[@]}"; do
	    dfold=`echo ${fold} | cut -d ' ' -f 1`
	    dmc=`echo ${fold} | cut -d ' ' -f 2`
	    ./html_relative.py $fname $dfold > AAA_${fname}_${dfold}_${dmc}_rel_${func}.html
	    ./html_absolute.py $fname $func $fold  > AAA_${fname}_${dfold}_${dmc}_abs_${func}.html
	done
    done
done
