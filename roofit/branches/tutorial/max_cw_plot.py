#!/usr/bin/env python

"""
Checking that the powheg_pythia -> powheg_herwig reweighting works.
IDEA: C_W did not improve. Let's check if normalized distributions got better?
A quick script to plot truth and/or reco-level spectra for:
Powheg+Pythia , Powheg+Pythia (reweighted) , Powheg+Herwig.
"""

import os,sys,glob,re
import common
import ROOT
ROOT.gROOT.SetBatch(True)
ROOT.TH1.SetDefaultSumw2(ROOT.kTRUE)
import array
ptbins_l = [0,5,10,15,20,25,30,35,40,45,50,70,90,120,200]
ptbins_l = range(0,70)
ptbins = array.array('f',ptbins_l)

def FixEdgeBins(hist,v=1.0):
    for binX in xrange(0,hist.GetNbinsX()+2):
        for binY in xrange(0,hist.GetNbinsY()+2):
            for binZ in xrange(0,hist.GetNbinsZ()+2):
                globalBin = hist.GetBin(binX,binY,binZ)
                if hist.IsBinUnderflow(globalBin) or hist.IsBinOverflow(globalBin):
                    hist.SetBinContent(globalBin,v)
    return True

dbase = '/share/t3data3/antonk/ana/ana_v29I_1011012_PYtoHW_nowzptw_stacoCB_all'
fnameH = dbase +'/'+'mc_powheg_herwig_wminmunu/root_mc_powheg_herwig_wminmunu.root'
fnameP = dbase+'/'+'mc_powheg_pythia_wminmunu_toherwig/root_mc_powheg_pythia_wminmunu_toherwig.root'

MAXENTRIES = 10000
MAXENTRIES = 100000000

fH = ROOT.TFile.Open(fnameH); assert fH.IsOpen()
fP = ROOT.TFile.Open(fnameP); assert fP.IsOpen()

TH = fH.Get('dg/truth/st_w_final/ntuple'); assert TH
TP = fP.Get('dg/truth/st_w_final/ntuple'); assert TP
RH = fH.Get('dg/Nominal/st_w_final/ntuple'); assert RH
RP = fP.Get('dg/Nominal/st_w_final/ntuple'); assert RP

# truth histos: PowhegPythia, PowhegPythia_rw, PowhegHerwig
cT = ROOT.TCanvas('cT','cT',800,800)
cT.Divide(1,2)
cT.cd(1)
hT = [None,None,None]
leg = ROOT.TLegend(0.55,0.70,0.88,0.88,'Muon pT',"brNDC")

COLORS = [ 1,2,3,4,6,8]
STYLES = [ 20,21,22,23,29,33,34 ]
SIZES = [1.5,1.4,1.3,1.2,1.1,1.0]

def cr(name,T,cut = '(mcw*puw*wzptw*wpolw*vxw*ls1w*ls2w*effw*isow*trigw)*(mask)',idx = 0):
    h = ROOT.TH1D('h'+name,'h'+name,len(ptbins)-1,ptbins)
    print 'Making ',name
    T.Draw('l_pt>>h%s'%name,cut,'goff',MAXENTRIES)
    leg.AddEntry(h,'PowhegHerwig','LP')
    FixEdgeBins(h,0.0)
    h.SetLineColor(COLORS[idx])
    h.SetMarkerColor(COLORS[idx])
    h.SetMarkerStyle(STYLES[idx])
    h.SetMarkerSize(SIZES[idx])
    h.Scale(1.0/h.Integral())
    return h
    
hT[0] = ROOT.TH1D('hTH','hTH',len(ptbins)-1,ptbins)
print 'Making PowhegHerwig...'
TH.Draw('l_pt>>hTH','(mcw*puw*wzptw*wpolw*vxw*ls1w*ls2w*effw*isow*trigw)','goff',MAXENTRIES)
leg.AddEntry(hT[0],'PowhegHerwig','LP')
FixEdgeBins(hT[0],0.0)
hT[0].SetLineColor(ROOT.kBlack)
hT[0].SetFillColor(ROOT.kBlack)
hT[0].Scale(1.0/hT[0].Integral())

hT[1] = ROOT.TH1D('hTP','hTP',len(ptbins)-1,ptbins)
print 'Making PowhegPythia...'
TP.Draw('l_pt>>hTP','(mcw*puw*wzptw*wpolw*vxw*ls1w*ls2w*effw*isow*trigw)','goff',MAXENTRIES)
leg.AddEntry(hT[1],'PowhegPythia','LP')
FixEdgeBins(hT[1],0.0)
hT[1].SetLineColor(ROOT.kBlue)
hT[1].SetFillColor(ROOT.kBlue)
hT[1].Scale(1.0/hT[1].Integral())

hT[2] = ROOT.TH1D('hTPrw','hTPrw',len(ptbins)-1,ptbins)
print 'Making PowhegPythia(rw)...'
TP.Draw('l_pt>>hTPrw','(mcw*puw*wzptw*wpolw*vxw*ls1w*ls2w*effw*isow*trigw*phw)','goff',MAXENTRIES)
leg.AddEntry(hT[2],'PowhegPythia(RW)','LP')
FixEdgeBins(hT[2],0.0)
hT[2].SetLineColor(ROOT.kRed)
hT[2].SetFillColor(ROOT.kRed)
hT[2].Scale(1.0/hT[2].Integral())

hT[0].Draw()
hT[1].Draw('A SAME')
hT[2].Draw('A SAME')

hT[0].GetYaxis().SetRangeUser(0.0,1.3*max(o.GetMaximum() for o in hT))

# RATIOS
cT.cd(2)
def refline(h):
    refLine = h.Clone()
    refLine.Reset()
    for ibin in range (1,refLine.GetNbinsX()+1):
      refLine.SetBinContent( ibin , 1.0 )
    refLine.SetLineColor( ROOT.kAzure-7 );
    refLine.SetFillColor( 10 );
    refLine.SetMinimum( 0.9 )
    refLine.SetMaximum( 1.1 )
    refLine.GetYaxis().SetNdivisions( 406 );
    refLine.GetYaxis().SetTitle( "ratio" );
    refLine.SetLineWidth( 3 );
    refLine.SetLineStyle( ROOT.kDashed );
    return refLine
ratio1 = hT[1].Clone()
ratio1.Divide(hT[0])
ratio2 = hT[2].Clone()
ratio2.Divide(hT[0])
refLine = refline(ratio1)
refLine.Draw( "HIST" );
ratio1.Draw('A SAME')
ratio2.Draw('A SAME')

cT.Modified()
cT.Update()
cT.SaveAs('plot.png')
