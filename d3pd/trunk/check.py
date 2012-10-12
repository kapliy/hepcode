#!/usr/bin/env python

import sys,math
import ROOT
ROOT.gROOT.SetBatch(1)
ROOT.gROOT.ProcessLine('.L loader.C+')
true,false = True,False

try:
    import psyco
    psyco.full()
except:
    print 'Failed to import psyco'
    pass

fname='file.root'
fname = '/home/antonk/d3pd/egamma/mc11_7TeV.107654.AlpgenJimmyZeeNp4_pt20.merge.NTUP_SMWZ.e835_s1299_s1300_r3043_r2993_p833_tid655684_00/NTUP_SMWZ.655684._000005.root.1'
f = ROOT.TFile.Open(fname)
t = f.physics
t.ls()

nentries = t.GetEntries()
nproc = nentries-1
if len(sys.argv)==2:
    try:
        nproc = min(nentries,int(sys.argv[1]))
    except:
        nproc = nentries-1

toterr=0
for evt in xrange(nproc):
    if evt%100==0:
        print 'EVT:',evt
    t.GetEntry(evt)
    assert t.mu_staco_n == t.mu_staco_MET_n
    for obj in ('el','ph','mu_staco','mu_muid','tau','jet_AntiKt4LCTopo'):
        for imu in xrange( getattr(t,'%s_MET_n'%obj) ):
            size = getattr(t,'%s_MET_statusWord'%obj)[imu].size()
            if size!=1 and obj!='mu_staco':
                print 'WARNING:',toterr,' : \t',obj,size
                toterr+=1
print 'ENDED: toterr = ',toterr
