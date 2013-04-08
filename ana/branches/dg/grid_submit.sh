#!/bin/bash

# Determine dataset names
USER_ID=`voms-proxy-info -all | grep --color=never nickname | awk '{print $5}'`
if [ ! -n "${USER_ID}" ]; then
    echo 'ERROR: cannot retrieve nickname attribute'
    echo 'Forgot to run voms-proxy-init?'
    echo "If that doesn't help, make sure you have a nickname (https://lcg-voms.cern.ch:8443/vo/atlas/vomrs)"
    exit -1;
fi;
USER_PREFIX=user.${USER_ID}

if [ "$#" -lt "6" ]; then
    echo 'Need at least five arguments: nFilesPerJob inDS outSUFFIX ana_exec dtag ismc [optional args]';
    echo "Example: `basename $0` 20 user.Fermi.dataset wmunu.1 ana/ana_wjets DWZ 0 --apply-pileup";
    exit -1;
fi;
NPERJOB=$1
INDS=$2
INDS_STRIPPED=$(echo $2 | sed 's#/##g')  # strip container trailing slash
OUT_SUFFIX=$3
OUTDS=$USER_PREFIX.$OUT_SUFFIX
EXECUTABLE=$4
DTAG=$5
ISMC=$6
shift 6
EXTRA=$@ # the rest of the arguments

if [ ! -f "$EXECUTABLE" ]; then
    echo "ERROR: cannot find requested executable $EXECUTABLE"
    exit -1;
fi;

# Prepare tarball
#TODO: add a find-rm statement to clean up tarballs older than N hours
if [ ! -f grid/TrigFTKAna.tar.bz2 ]; then
    echo "Making TrigFTKAna tarball. Please wait (this may take a while)..."
    ./grid/make_tarball.sh TrigFTKAna .
    if [ -f "TrigFTKAna.tar.bz2" ]; then
	mv TrigFTKAna.tar.bz2 grid/
    else
	echo "ERROR: couldn't make TrigFTKAna.tar.bz2 tarball"
	exit -1;
    fi;
else
    echo "WARNING: reusing existing TrigFTKAna tarball"
fi;

# Submit the job
# Note 1: that all additional command line parameters are passed along to ana executables
# Note 2: you need to add to --outputs if you want to save additional output files (e.g, aan ntuples)

# Some sites currently have problems: 
# ANALY_LRZ - format for dcache file names doesn't work for us (2011-04-13)
# ANALY_FREIBURG - Doesn't have libz in 32-bit (2011-04-13)
# ANALY_RAL - Doesn't have libz in 32-bit (2011-04-13)
# ANALY_SFU - Doesn't have libz in 32-bit (2011-04-14)
# ANALY_GRIF-LPNHE  - Doesn't have libz in 32-bit (2011-04-14)
# ANALY_DESY-HH - Couldn't even see the build log file for error (2011-04-15)
# ANALY_RHUL - Appears not to have GNU automake? (2011-04-15)
# ANALY_SWT2_CPB - Appears not to have GNU automake? (2011-04-15)
# ANALY_LIV - Doesn't have libz in 32-bit (2011-04-15)
# ANALY_LPC - Doesn't have libz in 32-bit (2011-04-21)
# ANALY_DRESDEN - computer node doesn't have aclocal (2011-04-21)
# ANALY_DESY-ZN - computer node doesn't have aclocal (2011-04-21)
# ANALY_IHEP - computer node doesn't have aclocal (2011-04-21)
# FIXME Temporarily adding ANALY_MWT2 because build jobs are failing due to recent maintenace (2011-04-24)
cd grid
set -x
prun --excludedSite=ANALY_MWT2,ANALY_LRZ,ANALY_FREIBURG,ANALY_RAL,ANALY_SFU,ANALY_GRIF-LPNHE,ANALY_DESY-HH,ANALY_RHUL,ANALY_SWT2_CPB,ANALY_LIV,ANALY_LPC,ANALY_DRESDEN,ANALY_DESY_ZN,ANALY_IHEP --athenaTag 16.0.3 --nFilesPerJob $NPERJOB --inDS $INDS --outDS $OUTDS --writeInputToTxt 'IN:IN.files.dat' --extFile 'TrigFTKAna.tar.bz2' --outputs 'ana.root','ntuples*' --excludeFile '.svn' --bexec './prun_compile.sh' --exec "app=$EXECUTABLE DTAG=$DTAG ISMC=$ISMC ./prun_job.sh $EXTRA"
set +x
cd -
