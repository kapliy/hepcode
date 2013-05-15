#!/usr/bin/env python
""" Studies unfolding response matrices from different generators """

import os,sys,ROOT

fnames = ['unfold_powheg_pythia.root','unfold_powheg_herwig.root','unfold_mcnlo.root']
fs = [ ROOT.TFile.Open(fname) for fname in fnames]
rs = [ f.Get('nominal/POS/abseta') for f in fs ]

recos  = [r.Hmeasured() for r in rs]
fakes  = [r.Hfakes() for r in rs]
truths = [r.Htruth() for r in rs]
assert all(recos) and all(fakes) and all(truths)

frates = []
feffs = []

for i in (0,1,2):
    frates.append( fakes[i].Clone() )
    frates[-1].Divide( recos[i] )
    frates[-1].SetLineColor(i+1)
    feffs.append( recos[i].Clone() )
    feffs[-1].Add(fakes[i],-1)
    feffs[-1].Divide(truths[i])
    feffs[-1].SetLineColor(i+1)

c = ROOT.TCanvas('c','c',800,600)
c.cd()
leg = ROOT.TLegend(0.55,0.30,0.88,0.48,'MC:',"brNDCDD")
leg.AddEntry(feffs[0],'Powheg+Pythia','LP')
leg.AddEntry(feffs[1],'Powheg+Herwig','LP')
leg.AddEntry(feffs[2],'MC@NLO','LP')
feffs[0].SetTitle('Reco efficiency')
feffs[0].Draw()
feffs[1].Draw('A SAME')
feffs[2].Draw('A SAME')
leg.Draw('same')
c.SaveAs('plot.png')


