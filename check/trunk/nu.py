#!/usr/bin/env python

import os,sys
if not 'libPyRoot' in sys.modules: #hack to get rid of TenvRec warnings
    from ROOT import gROOT
    sys.modules['libPyROOT'].gROOT.GetListOfGlobals().FindObject('gErrorIgnoreLevel').GetAddress().__setitem__(0,3000)
    import ROOT
    gErrorIgnoreLevel=ROOT.kInfo
    sys.modules['libPyROOT'].gROOT.GetListOfGlobals().FindObject('gErrorAbortLevel').GetAddress().__setitem__(0,5001)

import ROOT
ROOT.gROOT.SetBatch(True)

import sys
assert len(sys.argv)>1
fname = sys.argv[1]

# small hack to allow reading lists directly
if True:
    flist = open(sys.argv[1])
    fname = flist.readlines()[0].strip()

print fname

f = ROOT.TFile.Open(fname)
t = f.tree
t.Scan('event:mc_pdgid[mc_parent]:mc_pdgid:mc_status:mc_pt','fabs(mc_pdgid)==14 && mc_parent>=0')
#t.Scan('event:mc_pdgid:mc_status:mc_pt','fabs(mc_pdgid)==13')