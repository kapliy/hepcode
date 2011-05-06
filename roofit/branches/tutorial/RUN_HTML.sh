fnames="root_0505_closure.root"

folders[0]="data mc_zmumu"
folders[1]="data_20110425 mc_20110425zmumu"
folders[2]="data_scaled_00 mc_zmumu"
folders[3]="data_scaled_11 mc_zmumu"
folders[4]="data_20110425_scaled_00 mc_20110425zmumu"
folders[5]="data_20110425_scaled_11 mc_20110425zmumu"

#fnames="root_0505_preclosure.root"
#folders="data data_20110425"

for fname in $fnames; do
    for fold in "${folders[@]}"; do
	dfold=`echo ${fold} | cut -d ' ' -f 1`
	echo ./html_relative.py $fname $dfold
	./html_relative.py $fname $dfold > AAA_${fname}_${dfold}_rel.html
	echo ./html_absolute.py $fname $fold
	./html_absolute.py $fname $fold > AAA_${fname}_${dfold}_abs.html
    done
done
