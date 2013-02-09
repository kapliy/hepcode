#!/bin/bash

# smart script to keep submitting jobs as long as there are queue slots available

ntot=`./QCD_3D_exec.sh | grep Max | awk '{print $3}'`
nperblk=800 # njobs per block
nsubmin=300 # resubmit when jobs fall below this
nblk=`expr ${ntot} / ${nperblk}` # total number of blocks
nperlast=`expr ${ntot} % ${nperblk}` # njobs in last block

echo "Total: ${ntot} Blocks: ${nblk}"

source config.sh
rm -f ${DB}.db

for iblk in `seq 0 ${nblk}`; do
    ijmin=`expr ${iblk} \* ${nperblk}`
    ijmax=`expr ${iblk} \* ${nperblk} + ${nperblk} - 1`
    if [ "${iblk}" == "${nblk}" ]; then
	ijmax=`expr ${iblk} \* ${nperblk} + ${nperlast} - 1`
    fi
    # keep waiting until there are enough job slots
    islp=0
    while [ "1" -eq "1" ]; do
	nactive=`qstat -u $USER 2>/dev/null | grep short | grep -c $USER`
	#nactive=`qstat -u $USER 2>/dev/null | grep -c $USER`
	if [ "${nactive}" -lt "${nsubmin}" ]; then
	    break
	fi
	echo "Sleeping sequence: $islp because ${nactive} active jobs exceed ${nsubmin} job limit"
	((islp++))
	sleep 8
    done
    echo "BLOCK ${iblk} / ${nblk} : `expr ${ijmax} - ${ijmin} + 1` jobs: ${ijmin} to ${ijmax}"
    # submit jobs in this group
    for i in `seq ${ijmin} ${ijmax}`; do
	JID="qsub -v id=$i -N QCD3D_${i} -o /home/antonk/roofit/logs/log.QCD3D.${i}.stdout -e /home/antonk/roofit/logs/log.QCD3D.${i}.stderr QCD_3D_exec.sh"
	jid=`${JID}`
	echo "JOB: $i"
	if [ "$jid" == "" ]; then
	    echo "Retrying submission: $i"
	    sleep 10
	    jid=`${JID}`
	    if [ "$jid" == "" ]; then
		echo "Re-Retrying submission: $i"
		sleep 20
		jid=`${JID}`
	    fi
	fi
    done
done
