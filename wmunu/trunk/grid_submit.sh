#!/bin/bash

USER_ID=`. $PATHENA_GRID_SETUP_SH; voms-proxy-info -all | grep --color=never nickname | awk '{print $5}'`
if [ ! -n "${USER_ID}" ]; then
    echo 'ERROR: cannot retrieve nickname attribute'
    echo 'Forgot to run voms-proxy-init?'
    echo "If that doesn't help, make sure you have a nickname (https://lcg-voms.cern.ch:8443/vo/atlas/vomrs)"
    exit -1;
fi;
USER_PREFIX=user.${USER_ID}

if [ "$#" -lt "2" ]; then
    echo 'Need at least two arguments:'
    echo 'INPUT_DS OUTPUT_TAG';
    exit -1;
fi;
    
INDS=$1
USER_SUFFIX=$2
INDS_STRIPPED=$(echo $1 | sed 's#/##g')  # strip container trailing slash
shift 2

USERDS=$USER_PREFIX.$INDS_STRIPPED.$USER_SUFFIX
set -x
prun --inDS $INDS --outDS $USERDS --notSkipMissing --noBuild --extOutFile 'out_files.tar.bz2' --exec './looper.py -i %IN -x "*GRL*.xml" -o output' $@
set +x
