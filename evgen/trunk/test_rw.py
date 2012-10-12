#!/usr/bin/env python

"""
A quick test of reweighting
"""

import array
ptbins_l = [0,5,10,15,20,25,30,35,40,45,50,120,200]
ptbins = array.array('f',ptbins_l)

import os,sys,re,glob
if not 'libPyRoot' in sys.modules: #hack to get rid of TenvRec warnings
    from ROOT import gROOT
    sys.modules['libPyROOT'].gROOT.GetListOfGlobals().FindObject('gErrorIgnoreLevel').GetAddress().__setitem__(0,3000)
    import ROOT
    gErrorIgnoreLevel=ROOT.kInfo
    sys.modules['libPyROOT'].gROOT.GetListOfGlobals().FindObject('gErrorAbortLevel').GetAddress().__setitem__(0,5001)

def FixEdgeBins(hist,v=1.0):
    for binX in xrange(0,hist.GetNbinsX()+2):
        for binY in xrange(0,hist.GetNbinsY()+2):
            for binZ in xrange(0,hist.GetNbinsZ()+2):
                globalBin = hist.GetBin(binX,binY,binZ)
                if hist.IsBinUnderflow(globalBin) or hist.IsBinOverflow(globalBin):
                    hist.SetBinContent(globalBin,v)
    return True

def means(h1,h2,n='Comparison'):
    print '%s: '%n,h1.GetMean(),h2.GetMean(),h1.Chi2Test(h2,'UU CHI2/NDF')

if __name__=='__main__':
    fherwig = ROOT.TFile.Open('root_D3PD_HERWIG.root'); assert fherwig.IsOpen()
    herwig = fherwig.Get('hmupt').Clone('herwig'); assert herwig
    herwig.SetDirectory(0)
    herwig.SetLineColor(ROOT.kBlue)
    fherwig.Close()

    fpythia = ROOT.TFile.Open('root_FLAT_PYTHIA.root'); assert fpythia.IsOpen()
    pythia = fpythia.Get('hmupt').Clone('pythia'); assert pythia
    pythia.SetDirectory(0)
    pythia.SetLineColor(ROOT.kRed)
    fpythia.Close()

    means(herwig,pythia,'Original')

    c = ROOT.TCanvas('c','c',1024,600)
    c.Divide(2,1)

    c.cd(1)
    herwig.DrawCopy('H')
    pythia.DrawCopy('A same H')

    DOSCALE=True
    hs = herwig.Clone('hs')
    FixEdgeBins(hs,0.0)
    if DOSCALE:
        hs.Scale(1.0/hs.Integral())
    ps = pythia.Clone('ps')
    FixEdgeBins(ps,0.0)
    if DOSCALE:
        ps.Scale(1.0/ps.Integral())
    ratio = hs.Clone('ratio')
    ratio.Divide(ps)

    closure = pythia.Clone('closure')
    closure.Multiply(ratio)
    closure.SetLineColor(ROOT.kGreen)

    c.cd(2)
    means(herwig,closure,'Herwig/Closure')
    herwig.DrawCopy('H')
    closure.DrawCopy('A same H')
    c.Modified()
    c.Update()
