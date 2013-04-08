#!/bin/bash

echo "DATE AT START:"
date

ROOTDIR=`pwd`
# override some dcache settings
export DCACHE_DEBUG=0
#unset DC_LOCAL_CACHE_BUFFER

# Process panda-created list of input files (prun --writeInputToTxt option)
RAWLIST=${ROOTDIR}/IN.files.dat         # raw list from panda
INPLIST=${ROOTDIR}/MY.files.dat         # cleaned list, ready-to-use in analysis
function prepareInputs () {
    rm -f $INPLIST
    touch $INPLIST
    if [ -s $RAWLIST ];
    then
        # parse comma-separated input, remove dcap://server/ prefix,
        # remove whitespace; sort; convert to full linux path (if not on dcache)
        cat $RAWLIST 2>/dev/null | awk 'function c(p){delete r;split(p,r,"/pnfs/");return r[2] ? sprintf("/pnfs/%s",r[2]) : p;}BEGIN{FS=","};{for(i=1;i<=NF;i++)print c($i);}' | sed 's/^[ \t]*//;s/[ \t]*$//' | sort | awk -v base=${ROOTDIR} 'BEGIN{FS="/"};NF>1{print};NF<=1{printf("%s/%s\n",base,$0)}' > ${INPLIST}
        for ff in `cat $STAGED_PATHS`; do
            basename $ff >> $INPLIST
        done
        echo "LIST OF %IN% FILE PATHS (from Panda):"
        cat $RAWLIST
        echo "LIST OF %IN% FILE PATHS (after processing):"
        cat $INPLIST
    else
        echo "ERROR: missing $RAWLIST file. Forgot --writeInputToTxt argument to prun?"
	set -x
	cat $RAWLIST
	set +x
        exit 1
    fi;
    # final check that we have at least one input file
    NFILES=`wc -l < $INPLIST`
    if [ "$NFILES" -eq "0" ]; then
        echo "ERROR: no input files to process"
        exit 1
    fi;
}

# get list of files to merge
cd $ROOTDIR
prepareInputs
stage=0
sf=${ROOTDIR}/initial_merge.in
touch $sf
for f in `cat $INPLIST | grep ana.root`; do
    echo "$f" >> $sf
done
echo "============================================="
echo "Merging Files:"
echo "---------------------------------------------"
cat $sf
echo "============================================="
echo

# merge files in sets of length $mergesize
cd ${ROOTDIR}

while [ 1 ]; do
    let stage++
    mkdir -p ${ROOTDIR}/merge_stage${stage}
    totalfiles=`cat $sf | wc -l`
    echo "total files to merge for stage ${stage} = ${totalfiles}"
    haddstr=""
    partcount=0
    ifile=0
    cat $sf | while read line; do
        let ifile++
        haddstr="${line} ${haddstr}"
        if [ $ifile -eq $totalfiles -o $(( $ifile % $mergesize )) -eq 0 ]; then
            let partcount++
            echo "merging part ${partcount} of stage ${stage}"
	    mergeoutput=${ROOTDIR}/merge_stage${stage}/merge_part${partcount}_ana.root
	    #touch ${ROOTDIR}/merge_stage${stage}/merge_part${partcount}_ana.root
	    if [ "$dgroot" == "0" ]; then
		dglistfile=${ROOTDIR}/dgmerge_stage${stage}_part${partcount}
		touch $dglistfile
		echo ${haddstr} | sed -e 's#\ #\n#g' > ${dglistfile}
		set -x
		${ROOTDIR}/TrigFTKAna/dg/dgplot -m ${mergeoutput} -q -l ${dglistfile}
		set +x
		haddstr=""
            elif [ "$dgroot" == "1" ]; then 
		set -x
                hadd ${mergeoutput} ${haddstr}
                set +x
                haddstr=""
	    else
		echo "ERROR: in prun_merge.sh value of dgroot = $dgroot not understood"
		exit 1
	    fi
        fi
    done
    # check if merging is done
    echo "Checking if another stage of merging is necessary"
    if [ `ls ${ROOTDIR}/merge_stage${stage} | wc -l` -lt 2 ]; then
        if [ -f "${ROOTDIR}/merge_stage${stage}/merge_part0_ana.root" ]; then
            echo "Merging successfully finished"
            ls ${ROOTDIR}/merge_stage*/*
            echo "Cleaning up"
            mv ${ROOTDIR}/merge_stage${stage}/merge_part0_ana.root ${ROOTDIR}/merged_ana.root
            #rm -r merge_stage*
            break;
        else
            echo "ERROR: job failed - were zero files found?"
            ls ${ROOTDIR}/*
	    ls ${ROOTDIR}/*/*
            exit 1
        fi
    else
        # if merging isn't done then create a new list of files to merge
        sf=${ROOTDIR}/stage${stage}_merge_list.in
        touch $sf
        ls ${ROOTDIR}/merge_stage${stage}/*.root >> $sf
        echo "===================================="
	echo "Beginning next merging stage"
	cat $sf
	echo "===================================="
    fi
    if [ $stage -gt 10 ]; then
	echo "ERROR: merge stage greater than 10"
	exit 1
    fi
done

echo "Final space usage:"
du -shc *

echo "DATE AT END:"
date
