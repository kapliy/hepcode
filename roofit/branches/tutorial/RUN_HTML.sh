fnames="root_0505_closure.root"
folders[0]="data mc_zmumu"

funcs[0]="egge3"
funcs[1]="gaus0"


fnames="root_all_0516_2010clos.root"
folders[0]="data mc_zmumu"
folders[1]="data mc_zmumu_00"

#folders[1]="data_20110425 mc_20110425zmumu"
#folders[2]="data_scaled_00 mc_zmumu"
#folders[3]="data_scaled_11 mc_zmumu"
#folders[4]="data_20110425_scaled_00 mc_20110425zmumu"
#folders[5]="data_20110425_scaled_11 mc_20110425zmumu"

#folders[0]="data_20110505BE1 mc_20110425zmumu"
#folders[1]="data_20110505D5E1 mc_20110425zmumu"
#folders[2]="data_20110505D5E1_scaled_00 mc_20110425zmumu"
#folders[3]="data_20110505D5E1_scaled_11 mc_20110425zmumu"

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
