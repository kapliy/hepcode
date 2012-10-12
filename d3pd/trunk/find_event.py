#!/usr/bin/env python

import sys
run,event = -1,-1
if len(sys.argv) == 3:
    run = int(sys.argv[1])
    event = int(sys.argv[2])

import ROOT
ROOT.gROOT.SetBatch(1)
import glob

t = ROOT.TChain('physics','physics')
for f in glob.glob('samples/mc11_7TeV.107664.AlpgenJimmyZmumuNp4_pt20.merge.NTUP_SMWZ.e835_s1299_s1300_r3043_r2993_p833_tid655691_00/NTUP_SMWZ.655691._000002.root*'):
    print 'Adding:',f
    t.AddFile(f)

print t.GetEntries()

if run==-1 and event==-1:
    t.Scan('RunNumber:EventNumber','')
elif event==-1:
    t.Scan('RunNumber:EventNumber','RunNumber==%d'%run)
elif run==-1:
    t.Scan('RunNumber:EventNumber','EventNumber==%d'%(event,))
else:
    t.Scan('RunNumber:EventNumber','RunNumber==%d && EventNumber==%d'%(run,event))
