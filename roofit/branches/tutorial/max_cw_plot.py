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
#dbase='/share/t3data3/antonk/ana/ana_v29I_1011012_PYtoHW_nowzptw_nometutil_stacoCB_all'
fns = []
fns.append( dbase +'/'+'mc_powheg_herwig_wminmunu/root_mc_powheg_herwig_wminmunu.root' )
fns.append( dbase+'/'+'mc_powheg_pythia_wminmunu_toherwig2dfine/root_mc_powheg_pythia_wminmunu_toherwig2dfine.root' )
fns.append( dbase+'/'+'mc_powheg_pythia_wminmunu_toherwig1dfine/root_mc_powheg_pythia_wminmunu_toherwig1dfine.root' )

USE_TRUTH=True
b='mcw*puw*wzptw*wpolw*vxw*ls1w*ls2w*effw*isow*trigw'
b='1.0'
b='mask*mcw*puw*wpolw*vxw*ls1w*ls2w*effw*isow*trigw'
b='mask*mcw*puw*vxw*ls1w*ls2w*effw*isow*trigw'
var='met'
var='l_pt'

MAXENTRIES = 10000
MAXENTRIES = 100000000
COLORS = [ 1,2,3,4,6,8]
STYLES = [ 20,21,22,23,29,33,34 ]
SIZES = [1.5,1.4,1.3,1.2,1.1,1.0]

fs = [ROOT.TFile.Open(fn,'READ') for fn in fns]
Ts = [] # truth trees
Rs = [] # reco  trees
hT = [] # histos
for f in fs:
    assert f.IsOpen()
    Ts.append( f.Get('dg/truth/st_w_final/ntuple') ); assert Ts[-1]
    Rs.append( f.Get('dg/Nominal/st_w_final/ntuple') ); assert Rs[-1]

# truth histos: PowhegPythia, PowhegPythia_rw, PowhegHerwig
cT = ROOT.TCanvas('cT','cT',800,800)
cT.Divide(1,2)
cT.cd(1)
leg = ROOT.TLegend(0.55,0.70,0.88,0.88,'Muon pT',"brNDC")

def cr(name,T,cut = '(mcw*puw*wzptw*wpolw*vxw*ls1w*ls2w*effw*isow*trigw)*(mask)',idx=0):
    h = ROOT.TH1D('h'+name,'h'+name,len(ptbins)-1,ptbins)
    print 'Making ',name
    T.Draw('%s>>h%s'%(var,name),cut,'goff',MAXENTRIES)
    leg.AddEntry(h,name,'LP')
    FixEdgeBins(h,0.0)
    h.SetLineColor(COLORS[idx])
    h.SetMarkerColor(COLORS[idx])
    h.SetMarkerStyle(STYLES[idx])
    h.SetMarkerSize(SIZES[idx])
    if True:
        INT_HERWIG = 2955907.0
        INT_PYTHIA = 16744582.0
        x = INT_HERWIG if re.search('Herwig',name) else INT_PYTHIA
        print 'STORED  :',name,x
        h.Scale(1.0/x)
    else:
        print 'INTEGRAL:',name,h.Integral()
        h.Scale(1.0/h.Integral())
    h.Draw() if idx==0 else h.Draw('A SAME')
    return h

Zs = Ts if USE_TRUTH else Rs
hT.append( cr('PowhegHerwig',   Zs[0],b+'',0) )
hT.append( cr('PowhegPythia',   Zs[1],b+'',1) )
hT.append( cr('PowhegPythia_rw2d',Zs[1],b+'*phw',2) )
hT[0].GetYaxis().SetRangeUser(0.0,1.5*max(o.GetMaximum() for o in hT))
leg.Draw('SAME')

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
ratios = []
ref = refline(hT[0])
ref.Draw( "HIST" );
for h in hT[1:]:
    ratio = h.Clone()
    ratio.Divide(hT[0])
    ratio.Draw('A SAME')
    ratios.append(ratio)

cT.Modified()
cT.Update()
cT.SaveAs('plot.png')
