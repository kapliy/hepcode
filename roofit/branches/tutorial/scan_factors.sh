rm -f RESULTS
for pos in {1.0,1.02,1.05,0.98,0.95}; do
    for neg in {1.0,1.02,1.05,0.98,0.95}; do
	res=`python zpeak.py scales.root z_p${pos}_n${neg}.root/dg/dg/st_z_final/z_m 2>/dev/null | grep mW | awk '{print $3,$5}'`
	echo $pos $neg $res >> RESULTS
    done
done
awk 'NR==1{mz=$3; erz=$4} {print $1,$2,sqrt($1*$2)*mz,$3,(sqrt($1*$2)*mz-$3)/sqrt(erz*erz+$4*$4)}' RESULTS
