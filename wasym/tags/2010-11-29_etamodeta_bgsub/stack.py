#!/usr/bin/env python

import sys,re
from optparse import OptionParser
parser = OptionParser()
parser.add_option("--input",dest="input",
                  type="string", default='root_G2I2_mc.root',
                  help="Path to input root file with all histos")
parser.add_option("--hname",dest="hname",
                  type="string", default='lepton_eta',
                  help="Histogram name under consideration")
parser.add_option("--lumi",dest="lumi",
                  type="float", default=31401.9,
                  help="Integrated luminosity for data (in nb^-1)")
parser.add_option("--qcd-scale",dest="qcdscale",
                  type="float", default=0.62,
                  help="QCD scale factor")
parser.add_option("-o", "--output",dest="output",
                  type="string", default="output",
                  help="Name of output dir for plots")
(opts, args) = parser.parse_args()

import ROOT
ROOT.TH1.AddDirectory(ROOT.kFALSE)    # ensure that we own all the histograms
#ROOT.gROOT.SetBatch(1)                # uncomment for interactive usage

def SetStyle(styleMacro=''):
    """ Global style settings for ROOT """
    if styleMacro=='':
        ROOT.gStyle.SetCanvasColor(0);
        ROOT.gStyle.SetCanvasBorderMode(0);
        ROOT.gStyle.SetPadColor(0);
        ROOT.gStyle.SetPadBorderMode(0);
        ROOT.gStyle.SetStatColor(0);
        
        ROOT.gStyle.SetOptStat(111111);
        ROOT.gStyle.SetOptFit(1111);
        ROOT.gStyle.SetHistFillColor(0);
        ROOT.gStyle.SetMarkerStyle(20);
        ROOT.gStyle.SetMarkerSize(.4);
        ROOT.gStyle.SetHistLineWidth(2);
        ROOT.gStyle.SetErrorX(0);
        
        ROOT.gStyle.SetTitleStyle(0);
        
        ROOT.gStyle.SetStatBorderSize(1);
        ROOT.gStyle.SetFrameFillColor(10);
        ROOT.gStyle.SetTitleFillColor(0);
        ROOT.gROOT.ForceStyle()
    else:
        ROOT.gROOT.LoadMacro(styleMacro);
SetStyle()

# Determine plot ordering for MC stacks
from MC import *

po = PlotOrder()
#po.Add(name='t#bar{t}',samples='mc_ttbar',color=ROOT.kGreen)
#po.Add(name='Z#rightarrow#tau#tau',samples='mc_ztautau',color=ROOT.kMagenta)
#po.Add(name='W#rightarrow#tau#nu',samples='mc_wtaunu',color=ROOT.kYellow)
#po.Add(name='Z#rightarrow#mu#mu',samples='mc_zmumu',color=ROOT.kRed)
#po.Add(name='QCD',samples=['mc_J%d'%z for z in range(6)],color=ROOT.kCyan)
po.Add(name='W#rightarrow#mu#nu',samples='mc_wmunu',color=10)
# Determine which data periods to plot
data = ['periodG','periodH','periodI']
mc = list(xflatten(po.mcg))
# All histo files that we need to get
allnames = data + mc

hdataP = []
hdataM = []
hmcP = {}
hmcM = {}
# truth level for wmunu only!
htrP = None
htrM = None

# load the inputs
f = ROOT.TFile.Open(opts.input)
for iname in allnames:
    print 'Loading',iname
    g=f.GetDirectory(iname+'.root').GetDirectory('dg').GetDirectory('dg').GetDirectory('st_w_final')
    pos = g.GetDirectory('POS')
    neg = g.GetDirectory('NEG')
    if re.match('mc',iname):
        hmcP[iname]=ScaleToLumi(pos.Get('lepton_eta').Clone(),iname,opts.lumi,opts.qcdscale)
        hmcM[iname]=ScaleToLumi(neg.Get('lepton_eta').Clone(),iname,opts.lumi,opts.qcdscale)
        if re.search('wmunu',iname):
            # for signal MC, also get truth-level template for efficiency
            g=f.GetDirectory(iname+'.root').GetDirectory('dg').GetDirectory('dg').GetDirectory('truth').GetDirectory('st_truth_mu')
            pos = g.GetDirectory('POS')
            neg = g.GetDirectory('NEG')
            htrP = ScaleToLumi(pos.Get('eta').Clone(),iname,opts.lumi,opts.qcdscale)
            htrM = ScaleToLumi(neg.Get('eta').Clone(),iname,opts.lumi,opts.qcdscale)
    else: #data:
        hdataP.append(pos.Get('lepton_eta').Clone())
        hdataM.append(neg.Get('lepton_eta').Clone())

# prepare output holders
leg = [ROOT.TLegend(0.55,0.70,0.88,0.88,"Data and MC","brNDC"),ROOT.TLegend(0.55,0.70,0.88,0.88,"Data and MC","brNDC")]
[l.SetFillStyle(1001) for l in leg]
[l.SetFillColor(10) for l in leg]
hs = [ROOT.THStack('eta','eta'), ROOT.THStack('eta','eta')]

# create mc histogram stacks
for i,bgnames in enumerate(po.mcg):
    hP = [hmcP[f] for f in bgnames]
    [hP[0].Add(fc) for fc in hP[1:]]
    hP[0].SetLineColor(ROOT.kBlack)
    hP[0].SetFillColor(po.mcgc[i])
    hs[0].Add(hP[0])
    leg[0].AddEntry(hP[0],po.mcg_name[i],'F')
# create data histograms
[hdataP[0].Add(fc) for fc in hdataP[1:]]
leg[0].AddEntry(hdataP[0],'Data(#int L dt = %.1f nb^{-1})'%opts.lumi,'LP')

maximum = max((hdataP[0].GetMaximum(),hs[0].GetMaximum()))
# prepare the canvas
c = ROOT.TCanvas('c','c',800,600)
c.cd()
# mc
hs[0].Draw("H")
hs[0].SetMinimum(0.1)
hs[0].SetMaximum(maximum*1.5)
#s.hs[key].GetHistogram().GetYaxis().SetRangeUser(0.0,maximum*1.25)
#data
hdataP[0].SetMarkerSize(1.0)
hdataP[0].Draw("Lsame")
leg[0].Draw("same")
