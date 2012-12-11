#!/usr/bin/env python

"""
# INSTRUCTIONS ON HOW TO PREPARE A COMPLETE DATA CUTFLOW FILE
bdir=/share/t3data3/antonk/ana/ana_v29I_12042012_edboard_stacoCB_all
cd ${bdir}
/bin/ls ${bdir}/data_period*/data*root > ALLDATA.list
~/TrigFTKAna/plot/dgadd -f 1 -d ${bdir}/ALLDATA.root -l ${bdir}/ALLDATA.list
"""

data = '/share/t3data3/antonk/ana/ana_v29I_12042012_edboard_stacoCB_all/ALLDATA.root'

import os,subprocess

cmd = '/home/antonk/TrigFTKAna/plot/dgplot -q -b cutflow.dg %s'%data

CUTS = []
p = os.popen(cmd,"r")
while True:
    line = p.readline()
    if not line: break
    words = line.split()
    if len(words)>0 and words[0]=='u':
        CUTS.append(words)

for cut in CUTS:
    print cut
