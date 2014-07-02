#!/bin/bash

ROOTDIR=$PWD
LOCDIR=results/ana_wasym
RESHOST=root://uct3-xrd.mwt2.org/
export XrdSecGSISRVNAMES=uct2-s5.uchicago.edu\|uct2-s6.uchicago.edu\|uct2-s20.uchicago.edu
export ATLAS_LOCAL_ROOT_BASE=/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase
source ${ATLAS_LOCAL_ROOT_BASE}/user/atlasLocalSetup.sh --quiet
asetup 17.6.0.1,64,here
if [ -z $ROOTSYS ]; then echo 'ERROR: ROOTSYS undefined. Exiting...'; exit 2; fi
if [ -z $SITEROOT ]; then echo 'ERROR: SITEROOT undefined. Exiting...'; exit 2; fi
export uct3_64=1
export ROOTCORE_NCPUS=24
source ${ROOTDIR}/scripts/dependencies.sh &> /dev/null
export ROOTCOREDIR=${ROOTDIR}/CommonAnalysis/RootCore
export ROOTCOREBIN=${ROOTDIR}/CommonAnalysis/RootCore
$ROOTCOREDIR/scripts/link.sh
export PATH=$ROOTCOREBIN/bin:$PATH
export LD_LIBRARY_PATH=$ROOTCOREBIN/lib:$LD_LIBRARY_PATH
export DYLD_LIBRARY_PATH=$ROOTCOREBIN/lib:$DYLD_LIBRARY_PATH
export PYTHONPATH=$ROOTCOREBIN/python:$PYTHONPATH
export LD_LIBRARY_PATH=${ROOTDIR}/LHAPDF-6.1.3/LHAPDF/lib:${LD_LIBRARY_PATH}
export LD_LIBRARY_PATH=${ROOTDIR}/LIBXML/:${LD_LIBRARY_PATH}
