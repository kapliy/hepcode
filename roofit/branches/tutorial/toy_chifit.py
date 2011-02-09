#!/usr/bin/env python

from optparse import OptionParser
parser = OptionParser()
# data sources
parser.add_option('-n',"--nevents",dest="nevents",
                  type="int", default=10000,
                  help="Number of events to generate")
parser.add_option('-t',"--tag",dest="tag",
                  type="string", default='plot',
                  help="A tag to append to all output plots")
parser.add_option("--forcescale",dest="forcescale",
                  type="float", default=1.0,
                  help="Force a particular scale factor on positive muons")
parser.add_option("--fitmin",dest="fitmin",
                  type="float", default=0.98,
                  help="Minimum bound of fit range")
parser.add_option("--fitmax",dest="fitmax",
                  type="float", default=1.02,
                  help="Maximum bound of fit range")
parser.add_option("--maxbins",dest="maxbins",
                  type="int", default=20,
                  help="Binning for chi2 calculation")
parser.add_option("--nscan",dest="nscan",
                  type="int", default=50,
                  help="Number of parameter values to scan")
parser.add_option("--seed",dest="seed",
                  type="int", default=0,
                  help="Random seed")
parser.add_option("--batch", default=False,
                  action="store_true",dest="batch",
                  help="Enable batch mode (all output directly to filesystem)")
(opts, args) = parser.parse_args()

import sys
import ROOT
ROOT.gROOT.SetBatch(opts.batch)
from ROOT import RooWorkspace,RooArgSet,RooArgList,RooDataHist,RooDataSet,RooAbsData,RooFormulaVar,RooRandom
from ROOT import RooHistPdf,RooKeysPdf,RooNDKeysPdf,RooCachedPdf
from ROOT import kTRUE,kFALSE,kDashed,gStyle,gPad
from ROOT import TFile,TH1,TH1F,TH1D
from ROOT import RooFit as RF
g=[]
w = RooWorkspace('w',kTRUE)

binning = range(2,opts.maxbins,2)

# make a joint pdf for various signal regions
w.factory("")
w.factory("x[0,30]")
w.factory("mean[15]")
w.factory("sigma[3]")
w.factory("sf[1.0,0.8,1.2]")
w.factory('Gaussian::gaus(x,mean,sigma)')
w.factory("Gaussian::gaus2(expr('x*sf',x,sf),mean,sigma)")
model = w.pdf('gaus')
model_s = w.pdf('gaus2')
x = w.var('x')
xset = RooArgSet(x)

#generate data
RooRandom.randomGenerator().SetSeed(opts.seed)
data = model.generate(xset,opts.nevents)
#extract data
out = []
NE = data.numEntries()
for ii in range(NE):
    aset=data.get(ii)
    out.append( aset.getRealValue('x') )
# generate scaled data
scale = [(opts.fitmin + (opts.fitmax-opts.fitmin)*zz/opts.nscan) for zz in xrange(opts.nscan)]
step = opts.nscan/9 or 1
datas = [RooDataSet('datas%s'%i,'Scaled data in bin %s'%i,xset) for i in range(opts.nscan)]
for i in range(opts.nscan):
    for v in out:
        x.setVal(v*1.0*scale[i])
        datas[i].add(xset)

# Scan the parameter space and determine best value & error
def plot_data(data,color=ROOT.kBlack,nbins=10):
    frame = x.frame(RF.Title('1/p_{T}'))
    RooAbsData.plotOn(data,frame,RF.LineColor(color),RF.MarkerColor(color),RF.Binning(nbins),RF.DataError(RooAbsData.SumW2))
    model.plotOn(frame)
    chi = frame.chiSquare(1)
    #model.paramOn(frame,data)
    return frame,chi
def make_graph(n,xs,ys):
    h = ROOT.TGraph(n)
    h.SetMarkerColor(4);
    h.SetMarkerSize(1.5);
    h.SetMarkerStyle(21);
    for z in xrange(n):
        h.SetPoint(z,xs[z],ys[z])
    return h
def make_canvas(name):
    c2 = ROOT.TCanvas(name,name,800,600); c2.cd()
    return c2
def fitgraph(h,FITMIN,FITMAX):
    # fit parabola
    fr = h.Fit('pol2','S','',FITMIN,FITMAX)
    f = h.GetFunction('pol2')
    chimin = f.GetMinimum(FITMIN,FITMAX)
    xmin = f.GetMinimumX(FITMIN,FITMAX)
    xleft = f.GetX(chimin+1,FITMIN,xmin)
    xright = f.GetX(chimin+1,xmin,FITMAX)
    err = 0.5*((xmin-xleft)+(xright-xmin))
    lbl_xmin = 0.35
    xtra=''
    return xmin,err,xleft,xright,xtra,lbl_xmin,chimin

if True:
    chi2s = []
    c0 = ROOT.TCanvas('c0','c0',1024,768)
    c0.Divide(3,3)
    zplot=1
    for z in xrange(opts.nscan):
        frame,chi = plot_data(datas[z],nbins=opts.maxbins); g.append(frame)
        chi2s.append(chi)
        if z%step==0 and frame:
            print 'Plotting',zplot
            # plot
            c0.cd(zplot)
            zplot+=1
            frame.Draw()
            # set pave text
            p = ROOT.TPaveText(.6,.70 , (.6+.30),(.70+.20) ,"NDC")
            p.SetTextAlign(11)
            p.SetFillColor(0)
            p.AddText('Scale=%.2f%%, chi2=%.2f'%(100.0*scale[z],chi2s[z]))
            p.Draw()
            g.append(p)
    g.append(c0)
    c0.Modified(); c0.Update();
    c1 = make_canvas('cscan')
    h = make_graph(opts.nscan,scale,chi2s)
    h.Draw('A*'); g.append(h)
    h.GetYaxis().SetRangeUser(-5,20.0);
    xmin,err,xleft,xright,xtra,lbl_xmin,chimin=fitgraph(h,opts.fitmin,opts.fitmax)
    if True:
        line = ROOT.TGraph(2)
        line.SetPoint(0,xleft,chimin+1)
        line.SetPoint(1,xright,chimin+1)
        line.SetLineWidth(4)
        line.SetLineColor(ROOT.kRed)
        line.Draw('l')
    if True:
        p = ROOT.TPaveText(lbl_xmin,.80 , (lbl_xmin+.35),(.80+.10) ,"NDC")
        p.SetTextAlign(11)
        p.SetFillColor(0)
        lbl = 'Best value (scale) = %.4f%% +/- %.4f%% %s'%(xmin,err,xtra)
        p.AddText(lbl)
        p.Draw()
    c1.Modified(); c1.Update()
    
if True:
    c2 = make_canvas('cfit')
    data.get().find("x").setBins(opts.maxbins)
    data_binned = data.binnedClone()
    r = model_s.fitTo(data_binned,RF.PrintLevel(1),RF.NumCPU(1),RF.Save())
    #r.correlationMatrix().Print()
    frame = x.frame()
    color=ROOT.kBlack
    RooAbsData.plotOn(data_binned,frame,RF.MarkerColor(color),RF.Name('bins%d'%opts.maxbins))
    model.plotOn(frame,RF.Name('model_ren'))
    frame.Draw()
    print frame.chiSquare(1)
    c2.Modified(); c2.Update()
