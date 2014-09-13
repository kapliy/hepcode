#!/bin/bash

if [ "$#" -ge "1" ]; then
    id=$1
else
    id="NONE"
fi
echo "Processing job id = $id"

if [ -z "${SITEROOT}" ]; then
    echo "Sourcing athena"
    export HOME=$PWD
    # NEW CODE: 2014 version
athver=19.0.2.1
export uct3_64=1
export ATLAS_LOCAL_ROOT_BASE=/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase
source ${ATLAS_LOCAL_ROOT_BASE}/user/atlasLocalSetup.sh --quiet
echo "Setting up 64-bit release ${athver} from: ${ATLAS_LOCAL_ROOT_BASE}"
athbase=`echo $athver | awk 'BEGIN{FS="."}; NF==3{print $0}; NF==4{printf("%d.%d.%d\n",$1,$2,$3)}'`
workdir=$HOME/work/${mbits}/$athver
mkdir -p $workdir
pushd $workdir
source $AtlasSetup/scripts/asetup.sh ${athver},64,here #,slc5
export XrdSecGSISRVNAMES=uct2-s5.uchicago.edu\|uct2-s6.uchicago.edu\|uct2-s20.uchicago.edu
export XrdSecGSISRVNAMES='uct2-*.uchicago.edu'
popd
export LD_LIBRARY_PATH=/home/antonk/EWUnfoldingH/Code/RooUnfold:${LD_LIBRARY_PATH}
export XrdSecGSISRVNAMES=uct2-s5.uchicago.edu\|uct2-s6.uchicago.edu\|uct2-s20.uchicago.edu


    #export ATLAS_LOCAL_ROOT_BASE=/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase
    #source ${ATLAS_LOCAL_ROOT_BASE}/user/atlasLocalSetup.sh --quiet
    #source $AtlasSetup/scripts/asetup.sh 17.6.0.1,64,here
fi
if [ -z $ROOTSYS ]; then echo 'ERROR: ROOTSYS undefined. Exiting...'; exit 50; fi
if [ -z $SITEROOT ]; then echo 'ERROR: SITEROOT undefined. Exiting...'; exit 51; fi
echo "PWD = $PWD"
echo "HOSTNAME = $HOSTNAME"

source config.sh

nomonly=" --extra ${DB} --exit"
irun=0
for iq in 0 1; do 
    for ipt in {1..7} ALL20 ALL25; do
	ETAMODES="2"
	if [ "${ipt}" == "ALL25" ]; then
	    ETAMODES=`echo 2 1`
	fi
	for etamode in ${ETAMODES}; do
	    ETAS=`echo {1..11} ALL`;
	    if [ "${etamode}" == "1" ]; then
		ETAS=`echo {1..22}`
	    fi
	    for ieta in ${ETAS}; do
		if [ "${ieta}" == "ALL" ]; then
		    if [ "${ipt}" != "ALL20" -a "${ipt}" != "ALL25" ]; then continue; fi
		fi
		if [ "$id" == "$irun" -o "$id" == "ALL" ]; then
		    ./stack2.py ${nomonly} --input ${input} -b --charge ${iq} --lvar d3_abseta_lpt_met --lbin 50,0,40 --isofail IsoWind20 -o ${out} -t Q${iq} -m qcdfit --bgsig ${bgsig} --bgewk 5 --bgtau 2 --bgqcd 4  --ipt ${ipt} --var d3_abseta_lpt_met --xsecerr 0 --etamode ${etamode} --rebin 4 --ieta ${ieta}
		    st="$?"
		    if [ "${st}" != "0" ]; then
			echo "ERROR: script failed with status code ${st}"
			exit 54
		    fi
		    # if not running from $HOME: rename db file
		    if [ -f ${DB}.db ]; then
			if [ "$PWD" != "/home/antonk/roofit" ]; then
			    mv ${DB}.db ${DB}.pt${id}.db;
			fi
		    else
			echo "ERROR: failed to produce a db file"
			exit 55
		    fi
		fi
		((irun++))
	    done
	done
    done
done
wait
echo "Maximum irun: $irun"
date
