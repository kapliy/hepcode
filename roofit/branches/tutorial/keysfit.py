#!/usr/bin/env python

# fits a single z peak using RooKeysPDF

import sys,array
import SimpleProgressBar

def func_SCALE(xx,par):
    return par*1.0/xx
def func_SHIFT(xx,par):
    return 1.0/xx + par

_fin = 'root_all.root'
_hist = 'data_data.root/dg/dg/st_z_final/BB/graph_lpt_P_N'
_hist_truth = 'mc_zmumu_mc_zmumu.root/dg/dg/st_z_final/BB/graph_lpt_P_N'
_tag = 'plot'
SS=0
#_hist = _hist_truth
if len(sys.argv)>=2:
    _fin = sys.argv[1]
if len(sys.argv)>=3:
    _hist = sys.argv[2]
if len(sys.argv)>=4:
    _hist_truth = sys.argv[3]
if len(sys.argv)>=5:
    _tag = sys.argv[4]
if len(sys.argv)>=6:
    SS=1

fmZ = 91.1876
minZ = 0.005
maxZ = 0.05

import ROOT
ROOT.gROOT.SetBatch(1)
from ROOT import RooWorkspace,RooArgSet,RooArgList,RooDataHist,RooDataSet,RooAbsData,RooFormulaVar
from ROOT import RooHistPdf,RooKeysPdf,RooNDKeysPdf
from ROOT import kTRUE,kFALSE,kDashed,gStyle,gPad
from ROOT import TFile,TH1,TH1F,TH1D
from ROOT import RooFit as RF
w = RooWorkspace('w',kTRUE)

# Z mass cannot float if we fit for scale
mZ = '91.1876'
if SS==0:
    NSCALE = 20
    func = func_SCALE
    scale = [(0.93 + 0.14*zz/NSCALE) for zz in xrange(NSCALE)]
    FITMIN=0.95
    FITMAX=1.05
else:
    NSCALE = 20
    func = func_SHIFT
    scale = [(-0.002 + 0.004*zz/NSCALE) for zz in xrange(NSCALE)]
    FITMIN = -0.001
    FITMAX =  0.001
print ['%.2f'%(z*100) for z in scale[5:]]
print ['%.2f'%(z*100) for z in scale[:5]]

# Load all datasets
if True:
    MAXLOAD=10000
    SMOOTHNESS=2
    # variables and parameters
    w.factory('x[%s,%s]'%(minZ,maxZ)); x = w.var('x')
    w.factory('b[1.0,0.9,1.1]')
    #################################################
    # truth datasets
    #################################################
    f = TFile(_fin,'r')
    hz = f.Get(_hist_truth)
    f.Close()
    N = hz.GetN()
    print 'Loaded truth graph',_hist_truth,'with',N,'entries'
    pos = hz.GetX()
    neg = hz.GetY()
    # make truth(x) for positive and negative muons
    truthP = RooDataSet('truthP','Zmumu mu+ MC',RooArgSet(x))
    truthN = RooDataSet('truthN','Zmumu mu- MC',RooArgSet(x))
    bar = SimpleProgressBar.SimpleProgressBar(20,N)
    for i in range(0,N):
        break #FIXME
        if MAXLOAD>0 and i>=MAXLOAD: break
        if i%(int(N/20))==0:
            print bar.show(i)
        x.setVal(1.0/pos[i])
        truthP.add(RooArgSet(x))
        x.setVal(1.0/neg[i])
        truthN.add(RooArgSet(x))
    getattr(w,'import')(truthP,RF.Rename('truthP'))
    getattr(w,'import')(truthN,RF.Rename('truthN'))
    #################################################
    # data datasets
    #################################################
    f = TFile(_fin,'r')
    hz = f.Get(_hist)
    f.Close()
    N = hz.GetN()
    print 'Loaded data graph',_hist,'with',N,'entries'
    pos = hz.GetX()
    neg = hz.GetY()
    # make data(x) for positive and negative muons
    dataP = RooDataSet('dataP','Zmumu mu+ data',RooArgSet(x))
    dataN = RooDataSet('dataN','Zmumu mu- data',RooArgSet(x))
    datasP = [RooDataSet('datasP%s'%i,'Zmumu mu+ data bin %s'%i,RooArgSet(x)) for i in range(NSCALE)]
    datasN = [RooDataSet('datasN%s'%i,'Zmumu mu- data bin %s'%i,RooArgSet(x)) for i in range(NSCALE)]
    bar = SimpleProgressBar.SimpleProgressBar(20,N)
    for i in range(0,N):
        if MAXLOAD>0 and i>=MAXLOAD: break
        if i%(int(N/20))==0:
            print bar.show(i)
        x.setVal(1.0/pos[i])
        dataP.add(RooArgSet(x))
        x.setVal(1.0/neg[i])
        dataN.add(RooArgSet(x))
        if True: # scaled or shifted versions
            for z in range(NSCALE):
                x.setVal(func(pos[i],scale[z]))
                datasP[z].add(RooArgSet(x))
                x.setVal(func(neg[i],scale[z]))
                datasN[z].add(RooArgSet(x))
    getattr(w,'import')(dataP,RF.Rename('dataP'))
    getattr(w,'import')(dataN,RF.Rename('dataN'))
    # make truth(x*b)
    w.factory("expr::xf('x*1.0/b',x,b)")
    #w.factory("KeysPdf::model(x,dataP,MirrorBoth,2)")
    #w.factory("KeysPdf::model(expr('x*1.0/b',x,b),dataP,MirrorBoth,2)")
    if True:
        data_model = dataP
        model = RooKeysPdf('model','model from truth',x,data_model,RooKeysPdf.MirrorBoth,SMOOTHNESS)
        xf = w.function('xf')
        #model = RooNDKeysPdf('model','model',RooArgList(x),data_model,'am')
        #model = RooNDKeysPdf('model','model',RooArgList(xf),data_model,'am')
        #model = RooHistPdf('model','model',RooArgList(xf),RooArgList(w.var('x')),truth,4)
        getattr(w,'import')(model)
    #w.writeToFile('workspace.root')
    isExt = kFALSE
frange = (float(minZ),float(maxZ))

# make a joint pdf for various signal regions
w.factory("")
w.defineSet('X','x')
model = w.pdf('model')
x = w.var('x')
chi2 = []

# Plot template shape
if True:
    c3 = ROOT.TCanvas('c3','c3',640,480); c3.cd()
    frame = x.frame()
    color=ROOT.kBlue
    RooAbsData.plotOn(data_model,frame,RF.LineColor(color),RF.MarkerColor(color),RF.Binning(20))
    model.plotOn(frame,RF.LineColor(color))
    frame.Draw()
    c3.SaveAs('%s_template.png'%_tag)

# Scan the parameter space and determine best value & error
def plot_data(data,color=ROOT.kBlack,nbins=10):
    frame = x.frame(RF.Title('1/p_{T}'))
    RooAbsData.plotOn(data,frame,RF.LineColor(color),RF.MarkerColor(color),RF.Binning(nbins))
    model.plotOn(frame)
    #model.paramOn(frame,data)
    return frame
if True:
    c = ROOT.TCanvas('c','c',1024,768)
    c.Divide(3,3)
    ps = [] # garbage collector
    zplot = 1
    for z in xrange(NSCALE):
        frame = plot_data(datasN[z],nbins=10); ps.append(frame)
        chi = frame.chiSquare(1)
        chi2.append(chi)
        step = NSCALE/9 or 1
        if z%step==0:
            # plot
            c.cd(zplot)
            zplot+=1
            frame.Draw()
            # set pave text
            p = ROOT.TPaveText(.6,.70 , (.6+.30),(.70+.20) ,"NDC")
            p.SetTextAlign(11)
            p.SetFillColor(0)
            p.AddText('Scale=%.2f, chi2=%.2f'%(scale[z],chi2[z]))
            p.Draw()
            ps.append(p)
    c.Update()
    c.SaveAs('%s_scan.png'%_tag)
    c2 = ROOT.TCanvas('c2','c2',800,600); c2.cd()
    h = ROOT.TGraph(NSCALE)
    h.SetMarkerColor(4);
    h.SetMarkerSize(1.5);
    h.SetMarkerStyle(21);
    h.SetTitle('Momentum %s: best value and error (%s)'%('scale' if SS==0 else 'shift',_tag))
    for z in xrange(NSCALE):
        h.SetPoint(z,scale[z],chi2[z])
        #print scale[z],chi2[z]
    h.Draw('ACP')
    fr = h.Fit('pol2','S','',FITMIN,FITMAX)
    f = h.GetFunction('pol2')
    chimin = f.GetMinimum(FITMIN,FITMAX)
    xmin = f.GetMinimumX(FITMIN,FITMAX)
    xleft = f.GetX(chimin+1,FITMIN,xmin)
    xright = f.GetX(chimin+1,xmin,FITMAX)
    err = 0.5*((xmin-xleft)+(xright-xmin))
    line = ROOT.TGraph(2)
    line.SetPoint(0,xleft,chimin+1)
    line.SetPoint(1,xright,chimin+1)
    line.SetLineWidth(2)
    line.SetLineColor(ROOT.kRed)
    line.Draw('l')
    p = ROOT.TPaveText(.35,.80 , (.35+.30),(.80+.10) ,"NDC")
    p.SetTextAlign(11)
    p.SetFillColor(0)
    p.AddText('Best value = %.3f +/- %.3f'%(xmin,err))
    p.Draw()
    c2.SaveAs('%s_chi2.png'%_tag)
    print 'value = %.3f +%.3f -%.3f; chi2 = %.2f'%(xmin,xmin-xleft,xright-xmin,chimin)
    fout = open('%s_results.rtxt'%_tag,'w')
    print >>fout,'value = %.3f +%.3f -%.3f; chi2 = %.2f'%(xmin,xmin-xleft,xright-xmin,chimin)
    fout.close()
