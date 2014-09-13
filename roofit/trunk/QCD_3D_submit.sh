#!/bin/bash

# smart script to keep submitting jobs as long as there are queue slots available

ntot=`./QCD_3D_exec.sh | grep Max | awk '{print $3}'`

echo "Total: ${ntot}"

source config.sh
rm -f ${DB}.db
rm -f logs/QCD.*.{OU,ER,LG} logs/*.db
mkdir -p /home/antonk/roofit/logs/

echo """
universe = vanilla

executable = /home/antonk/roofit/QCD_3D_exec.sh
output = /home/antonk/roofit/logs/QCD.\$(Process).OU
error = /home/antonk/roofit/logs/QCD.\$(Process).ER
log = /home/antonk/roofit/logs/QCD.\$(Process).LG

Arguments = \$(Process)

transfer_input_files = /home/antonk/roofit/antondb.py,/home/antonk/roofit/binning.py,/home/antonk/roofit/common.py,/home/antonk/roofit/config.sh,/home/antonk/roofit/FileLock.py,/home/antonk/roofit/ntuple_tools.py,/home/antonk/roofit/stack2.py,/home/antonk/roofit/SuCanvas.py,/home/antonk/roofit/SuData.py,/home/antonk/roofit/SuFit.py,/home/antonk/roofit/MC.py,/home/antonk/roofit/TFractionFitter2.h

should_transfer_files = YES
when_to_transfer_output = ON_EXIT

#Requirements   =  ( (Memory >= 900) && (HAS_CVMFS =?= TRUE) )
#Requirements   =  ( (Memory >= 900) && (HAS_CVMFS =?= TRUE) && (HAS_TIER3_DATA =?= TRUE) )

# This is how we can exclude faulty nodes:
Requirements   =  ( (Memory >= 900) && (HAS_CVMFS =?= TRUE) && (HAS_TIER3_DATA =?= TRUE) && (Machine != \"uct3-c023.uc.mwt2.org\") && (Machine != \"uct3-c024.uc.mwt2.org\") && (Machine != \"uct3-c022.uc.mwt2.org\"))

priority = 10

+AccountingGroup = \"group_uct3.antonk\"

Queue ${ntot}
""" > /home/antonk/roofit/logs/QCD.submit

cd /home/antonk/roofit/logs/
if [ "$HOSTNAME" == "uct3-s1.uchicago.edu" ]; then
    condor_submit /home/antonk/roofit/logs/QCD.submit
else
    condor_submit /home/antonk/roofit/logs/QCD.submit
    # we no longer need to ssh to uct3-s1!
    #ssh uct3-s1 'cd /home/antonk/roofit/logs/ && condor_submit /home/antonk/roofit/logs/QCD.submit'
fi
