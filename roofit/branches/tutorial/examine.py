#!/usr/bin/env python

"""
Examining the MCP systematic histograms to understand if they are indeed one-sided.
This is for Pythia.
"""
import sys,os,re
import ROOT

fname = '/share/t3data3/antonk/ana/ana_v29D_04292012_DtoM_unfold_stacoCB_all/mc_pythia_wmunu/root_mc_pythia_wmunu.root'

if len(sys.argv)>1:
    v = sys.argv[1]
    if v=='0':
        fname = '/home/antonk/TrigFTKAna/results/ana_wasym/wasymmetry29D_mc_pythia_wmunu.dat/20120605.root'
    elif v=='1':
        fname = '/home/antonk/TrigFTKAna/results/ana_wasym/wasymmetry29D_mc_pythia_wmunu.dat/20120605.1.root'
    else:
        assert False

print 'Opening:',fname
f = ROOT.TFile.Open(fname,'READ')
assert f.IsOpen()

hnames = [ 'dg/nominal/st_w_final/baseline/POS/lepton_absetav',
           'dg/mcp_msup/st_w_final/baseline/POS/lepton_absetav',
           'dg/mcp_msdown/st_w_final/baseline/POS/lepton_absetav' ]

h = [ f.Get(n) for n in hnames ]
assert all(h)
for i,hh in enumerate(h):
    hh.SetLineColor(i+1)
    hh.SetMarkerColor(i+1)

h[0].Draw()
[hh.Draw('A SAME') for hh in h[1:]]

leg = ROOT.TLegend(0.55,0.70,0.88,0.88,'MS smearing systematic',"brNDC")
leg.AddEntry(h[0],'nominal','LP')
leg.AddEntry(h[1],'msup','LP')
leg.AddEntry(h[2],'msdown','LP')
leg.Draw('same')

