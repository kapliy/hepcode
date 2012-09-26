#!/usr/bin/env python

"""
This script makes reco-level plots, comparing background-subtracted data with PowhegPythia MC
"""

import sys,re
import ROOT
ROOT.gROOT.SetBatch(True)

q = 'POS'
if len(sys.argv)>=2:
    q = sys.argv[1]
    assert q in ('POS','NEG','ALL'), 'Argument must be one of: POS NEG ALL'
print 'Charge =',q

fname = 'OUT_09082012.v1.eta.1D.root'
#fname = 'OUT_09212012.v1.eta.1D.root'

nominal='totalbg_Nominal'
#nominal='totalbg_Nominal_qcdaverage'

if len(sys.argv)>=3:
    fname = sys.argv[2]
print 'File =',fname

DIM = 2 if re.search('2D',fname) else 1
f = ROOT.TFile.Open(fname,"READ")
assert f.IsOpen(),'ERROR: unable to open file %s'%fname

data,bg,sig = None,None,None
if q in ('POS','NEG'): # grab mu+ or mu- histograms directly
    zdata = f.Get("%s/data"%q); assert zdata
    zbg   = f.Get("%s/%s"%(q,nominal)); assert zbg
    zsig =  f.Get("%s/wmunu_PowhegPythia_Nominal"%q); assert zsig
    data,bg,sig = zdata,zbg,zsig
else: # OR: add up mu+ and mu- histograms to get combined charge histograms
    qorig = q
    q = 'POS'
    data = f.Get("%s/data"%q); assert data
    bg   = f.Get("%s/%s"%(q,nominal)); assert bg
    sig =  f.Get("%s/wmunu_PowhegPythia_Nominal"%q); assert sig
    q = 'NEG'
    zdata = f.Get("%s/data"%q); assert zdata
    zbg   = f.Get("%s/%s"%(q,nominal)); assert zbg
    zsig =  f.Get("%s/wmunu_PowhegPythia_Nominal"%q); assert zsig
    data.Add(zdata)
    bg.Add(zbg)
    sig.Add(zsig)
    q = qorig
    
sig1d = sig.ProjectionX("_px",0,-1) if DIM==2 else sig.Clone()
sig1d.SetLineColor(ROOT.kRed)

bgsub = data.Clone()
bgsub.Add(bg,-1.0)

bgsub1d = bgsub.ProjectionX("_px",0,-1) if DIM==2 else bgsub.Clone()
bgsub1d.SetLineColor(ROOT.kBlue)
bgsub1d.SetMarkerStyle(21)
bgsub1d.SetMarkerSize(1)

c = ROOT.TCanvas("c","c",800,800)
c.Divide(1,2)

c.cd(1)
sig1d.Draw("HISTO")
bgsub1d.Draw("A SAME")
sig1d.GetYaxis().SetRangeUser(0.0,max(sig1d.GetMaximum(),bgsub1d.GetMaximum())*1.4)

c.cd(2)
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
    refLine.GetYaxis().SetTitle( "data / mc" );
    refLine.SetLineWidth( 3 );
    refLine.SetLineStyle( ROOT.kDashed );
    return refLine
ratio = bgsub1d.Clone()
ratio.Divide(sig1d)
refLine = refline(ratio)
refLine.Draw( "HIST" );
ratio.Draw('A SAME')

c.SaveAs("reco_eta_%s.png"%q)
