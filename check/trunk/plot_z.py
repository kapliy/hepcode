#!/usr/bin/env python

import os,sys
if not 'libPyRoot' in sys.modules: #hack to get rid of TenvRec warnings
    from ROOT import gROOT
    sys.modules['libPyROOT'].gROOT.GetListOfGlobals().FindObject('gErrorIgnoreLevel').GetAddress().__setitem__(0,3000)
    import ROOT
    gErrorIgnoreLevel=ROOT.kInfo
    sys.modules['libPyROOT'].gROOT.GetListOfGlobals().FindObject('gErrorAbortLevel').GetAddress().__setitem__(0,5001)

import ROOT
#ROOT.gROOT.SetBatch(True)

import sys
assert len(sys.argv)>1
fname = sys.argv[1]

t = ROOT.TChain('tree','tree')

# small hack to allow reading lists directly
if True:
    flist = open(sys.argv[1])
    for line in flist.readlines()[0:1]:
        l = line.strip()
        print 'Adding:',l
        t.AddFile(l)

if len(sys.argv)>2:
    t.Scan('event:mc_m:mc_pdgid:mc_status:mc_pt','fabs(mc_pdgid)==23')

# REMEMBER TO CHANGE STATUS BETWEEN 3 and 155, DEPENDING ON MC (PYTHIA VS POWHEG)
t.Draw('mc_m>>htot(8000,0,400)','(fabs(mc_pdgid)==23 && mc_status==155)*mc_weight','goff')
t.Draw('mc_m>>hlow(1200,0,60)','(fabs(mc_pdgid)==23 && mc_status==155)*mc_weight','goff')

c = ROOT.TCanvas('c','c',1024,768)
c.Divide(2,1)
c.cd(1)
ROOT.htot.Draw()
c.cd(2)
ROOT.hlow.Draw()

X = ROOT.htot.Integral() - ROOT.hlow.Integral()
Y = ROOT.hlow.Integral()
print 'LOW = %.3f ; HIGH = %.3f ; RATIO = %.5f'%(Y,X,(X+Y)/X)
