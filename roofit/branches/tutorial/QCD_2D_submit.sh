#!/bin/bash
ntot=`./QCD_2D.sh | grep Max | awk '{print $3}'`
echo $ntot
for i in `seq 0 ${ntot}`; do
    rem=`expr $i % 500`
    if [ "$i" -ne "0" -a "$rem" -eq "0" ]; then
	echo "Current job: ${i}. Sleeping for 1 minute ..."
	sleep 60
    fi
    qsub -v id=$i -N QCD2D_${i} -o /home/antonk/roofit/logs/log.QCD2D.${i}.stdout -e /home/antonk/roofit/logs/log.QCD2D.${i}.stderr QCD_2D.sh
done
