#!/bin/bash

echo "DATE AT START:"
date

source prun_compile.sh 1
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
	exit 1
    fi;
    # final check that we have at least one input file
    NFILES=`wc -l < $INPLIST`
    if [ "$NFILES" -eq "0" ]; then
	echo "ERROR: no input files to process"
	exit 1
    fi;
}

# prepare input configuration file
cd $ROOTDIR
cd TrigFTKAna
prepareInputs
sf=job_input.in
touch $sf
echo "V 2" > $sf
echo "Fb mc $ISMC" >> $sf
for f in `cat $INPLIST | grep -v GLOBAL`; do
    echo "$DTAG $f" >> $sf
    echo "E" >> $sf
done
echo "============================================="
echo "Job configuration:"
echo "---------------------------------------------"
cat $sf
echo "============================================="

# run the analysis
cd ${ROOTDIR}
mkdir -p ${ROOTDIR}/ntuples
cd TrigFTKAna
set -x
$app -i $sf --force-output-filename $ROOTDIR/ana --out-ntuple $ROOTDIR/ntuples --noROOT $@
set +x

# massage the output into a form that is understood by panda
cd $ROOTDIR
if [ -f "ana.root" ]; then 
    echo "Job finished successfully"
    echo "Trying to retrieve ntuple files:"
    ls -l ntuples*
    #tar cvjf ntuple.tar.bz2 ntuples*
    #find ntuples.aen -type f -name \*root\* -exec mv {} . \;
    # (make sure to add the ntuple naming pattern to prun --outputs)
else
    echo "ERROR: job failed - output file ana.root is missing"
    exit 1
fi;

echo "Final space usage:"
du -shc *

echo "DATE AT END:"
date
