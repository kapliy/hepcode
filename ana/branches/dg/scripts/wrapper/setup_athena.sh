#!/bin/bash
export TestArea="${PWD}/13.0.40"
echo "set CMTSITE STANDALONE" > ${TestArea}/cmthome/requirements
echo "set SITEROOT /nfs/osg/app/atlas_app/atlas_rel/13.0.40" >> ${TestArea}/cmthome/requirements
echo "macro ATLAS_DIST_AREA \${SITEROOT}" >> ${TestArea}/cmthome/requirements
echo -n "macro ATLAS_TEST_AREA " >> ${TestArea}/cmthome/requirements
pwd >> ${TestArea}/cmthome/requirements
echo "apply_tag opt" >> ${TestArea}/cmthome/requirements
echo "apply_tag simpleTest" >> ${TestArea}/cmthome/requirements
echo "use AtlasLogin AtlasLogin-* \$(ATLAS_DIST_AREA)" >> ${TestArea}/cmthome/requirements
echo "set CMTCONFIG i686-slc3-gcc323-opt" >> ${TestArea}/cmthome/requirements
echo "set DBRELEASE_INSTALLED  3.1.1" >> ${TestArea}/cmthome/requirements
cd ${TestArea}/cmthome
cmt config 
source ./setup.sh -tag=AtlasOffline,13.0.40
cd -
export CMTPATH=${PWD}:/osg/app/atlas_app/atlas_rel/13.0.40/AtlasProduction/13.0.40.4
source /osg/app/atlas_app/atlas_rel/13.0.40/AtlasProduction/13.0.40.4/AtlasProductionRunTime/cmt/setup.sh
cd ${TestArea}/Trigger/FastTracker/FastTrackSimWrap/cmt/
source ./setup.sh
cd -
