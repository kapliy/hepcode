#!/usr/bin/env python

import sys,os
import ROOT

SAVE_NAMES = ['avgintperbx' , 'intlumiruns']

if not len(sys.argv)==2:
    print 'Usage: %s ilumicalc_histograms.root'%sys.argv[0]
    sys.exit(0)
fin=sys.argv[1]

print 'Working on file',fin
f = ROOT.TFile.Open(fin,'READ')
assert f.IsOpen()
ob = [f.Get(name).Clone() for name in SAVE_NAMES]
[o.SetDirectory(0) for o in ob]
f.Close()
if not all(ob):
    print 'One of the objects was not loaded'
    print SAVE_NAMES
    print ob

print 'Ready to overwrite file',fin
fout = ROOT.TFile.Open(fin,'RECREATE')
fout.cd()
[o.Write() for o in ob]
fout.Close()

print 'Done!'
