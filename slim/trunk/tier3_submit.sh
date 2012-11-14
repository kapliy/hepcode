#!/bin/bash

echo "Setting up X509 proxy"
export X509_USER_PROXY=/home/antonk/.globus/tmp.proxy
(source /share/wlcg-client/setup.sh && voms-proxy-init -pwstdin -voms atlas -valid 999:0 -out ${X509_USER_PROXY} < /home/antonk/setup/info 2>&1)

rm -f lists/*dat
outdir=/atlas/uct3/data/users/antonk/NTUPLE/v1_29l
anadir=/home/antonk/slim

function get_extras() {
    d=$1
    # data: apply all cuts
    echo $d | grep -q data_ && {
	echo "--grl /home/antonk/TrigFTKAna/good_run_lists/asym_data11_7TeV.pro10.DtoM.xml --pt 15.0 --trigger"
	return
    }
    # signal MC: apply nothing, just remove useless branches
    echo $d | egrep -q 'wmunu|wminmunu|wplusmunu' && {
	echo ""
	return
    }
    # any other MC: apply relaxed cuts (because pt can swing up during smearing/scaling), but no GRL
    echo "--pt 13.0 --trigger"
    return
}

mc11="mc11c"
for dname in `cd ${mc11} && ls -1 *.dat`; do
    dsample=`echo ${dname} | sed -e 's#wasymmetry29I_##g' -e 's#.dat##g'`
    if [ "$dsample" == "" ]; then continue; fi
    outsubdir="${outdir}/${dsample}"
    if [ ! -d "${outsubdir}" ]; then
	echo "Making directory: ${outsubdir}"
	mkdir -p ${outsubdir}
    fi
    nfiles=`wc -l < ${mc11}/${dname}`;
    nper=15
    njobs=$(($nfiles / $nper))
    nrem=$(($nfiles % $nper))
    gmax="$(expr $njobs)"
    if [ "$nrem" == "0" ]; then
	gmax="$(expr $njobs - 1)"
    fi
    echo "Submitting sample ${dsample}: nfiles=$nfiles njobs=$njobs gmax=$gmax"
    for igroup in `seq 0 $gmax`; do
	jmin=$(expr $igroup \* $nper)
	ntail=$nper
	if [ "$igroup" == "$gmax" -a "$nrem" -gt "0" ]; then ntail=$(expr $nrem); fi	
	jmax=$(expr $jmin + $ntail - 1)
	#echo $igroup $jmin $jmax
	JBASE="${dsample}_J${igroup}"
	JNAME="lists/${JBASE}.dat"
	outfile="${dsample}_J${igroup}.root"
	cat ${mc11}/${dname} | head -n $(expr $jmax + 1) | tail -n $ntail > ${JNAME}
	echo "   -> Submitting ${JBASE}"
	extras=`get_extras $dsample`
	jid=`qsub -v anadir=${anadir},outdir=${outdir}/$dsample,inlist=${JNAME},outfile=${outfile},extras="$extras" -N ${JBASE} -o $anadir/logs/log.${JBASE}.stdout -e $anadir/logs/log.${JBASE}.stderr pbs.sh`
	if [ "$jid" == "" ]; then
	    echo "Retrying submission: $i"
	    sleep 10
	    jid=`qsub -v anadir=${anadir},outdir=${outdir}/$dsample,inlist=${JNAME},outfile=${outfile},extras="$extras" -N ${JBASE} -o $anadir/logs/log.${JBASE}.stdout -e $anadir/logs/log.${JBASE}.stderr pbs.sh`
	    if [ "$jid" == "" ]; then
		echo "Re-Retrying submission: $i"
		sleep 20
		jid=`qsub -v anadir=${anadir},outdir=${outdir}/$dsample,inlist=${JNAME},outfile=${outfile},extras="$extras" -N ${JBASE} -o $anadir/logs/log.${JBASE}.stdout -e $anadir/logs/log.${JBASE}.stderr pbs.sh`
		if [ "$jid" == "" ]; then
		    echo "ERROR: FAILED TO SUBMIT ${JBASE}"
		fi
	    fi
	fi
    done
done
