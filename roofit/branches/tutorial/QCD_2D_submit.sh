#!/bin/bash
ntot=`./QCD_2D.sh | grep Max | awk '{print $3}'`
echo $ntot
for i in `seq 0 ${ntot}`; do
    rem=`expr $i % 400`
    if [ "$i" -ne "0" -a "$rem" -eq "0" ]; then
	echo "Current job: ${i}. Sleeping for 1 minute ..."
	sleep 100
    fi
    jid=`qsub -v id=$i -N QCD2D_${i} -o /home/antonk/roofit/logs/log.QCD2D.${i}.stdout -e /home/antonk/roofit/logs/log.QCD2D.${i}.stderr QCD_2D.sh`
    if [ "$jid" == "" ]; then
	echo "Retrying submission: $i"
	sleep 5
	jid=`qsub -v id=$i -N QCD2D_${i} -o /home/antonk/roofit/logs/log.QCD2D.${i}.stdout -e /home/antonk/roofit/logs/log.QCD2D.${i}.stderr QCD_2D.sh`
    fi
done
