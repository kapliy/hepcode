#!/usr/bin/env python

# fits a single z peak using RooKeysPDF

import sys
import SimpleProgressBar

_fin = 'root_all.root'
_hist = 'data_data.root/dg/dg/st_z_final/BB/graph_lpt_P_N'
_hist_truth = 'mc_zmumu_mc_zmumu.root/dg/dg/st_z_final/BB/graph_lpt_P_N'
_plotname = 'plot.png'
#_hist = _hist_truth
if len(sys.argv)>=2:
    _fin = sys.argv[1]
if len(sys.argv)>=3:
    _hist = sys.argv[2]
if len(sys.argv)>=4:
    _hist_truth = sys.argv[3]

fmZ = 91.1876
minZ = 0.005
maxZ = 0.05

import ROOT
from ROOT import RooWorkspace,RooArgSet,RooArgList,RooDataHist,RooDataSet,RooAbsData,RooFormulaVar
from ROOT import RooHistPdf,RooKeysPdf,RooNDKeysPdf
from ROOT import kTRUE,kFALSE,kDashed,gStyle,gPad
from ROOT import TFile,TH1,TH1F,TH1D
from ROOT import RooFit as RF
w = RooWorkspace('w',kTRUE)

# Z mass cannot float if we fit for scale
mZ = '91.1876'
scale = [0.96,0.97,0.98,0.99,1.0,1.01,1.02,1.03,1.04]
# formula for Z mass:
#formula = 'sqrt((E1+E2)^2-(pz1+pz2)^2-(pt1*cos(phi1)+pt2*cos(phi2))^2-(pt1*sin(phi1)+pt2*sin(phi2))^2)'

# Load all datasets
if True:
    MAXLOAD=10000
    #MAXLOAD=-1
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
    datasP = [RooDataSet('datasP%s'%i,'Zmumu mu+ data bin %s'%i,RooArgSet(x)) for i in range(9)]
    datasN = [RooDataSet('datasN%s'%i,'Zmumu mu- data bin %s'%i,RooArgSet(x)) for i in range(9)]
    bar = SimpleProgressBar.SimpleProgressBar(20,N)
    for i in range(0,N):
        if MAXLOAD>0 and i>=MAXLOAD: break
        if i%(int(N/20))==0:
            print bar.show(i)
        x.setVal(1.0/pos[i])
        dataP.add(RooArgSet(x))
        x.setVal(1.0/neg[i])
        dataN.add(RooArgSet(x))
        if True: # scaled versions +/- 3%
            for z in range(9):
                x.setVal(scale[z]*1.0/pos[i])
                datasP[z].add(RooArgSet(x))
                x.setVal(scale[z]*1.0/neg[i])
                datasN[z].add(RooArgSet(x))
    getattr(w,'import')(dataP,RF.Rename('dataP'))
    getattr(w,'import')(dataN,RF.Rename('dataN'))
    # make truth(x*b)
    w.factory("expr::xf('x*1.0/b',x,b)")
    #w.factory("KeysPdf::model(x,dataP,MirrorBoth,2)")
    #w.factory("KeysPdf::model(expr('x*1.0/b',x,b),dataP,MirrorBoth,2)")
    if True:
        data_model = dataP
        xf = w.function('xf')
        model = RooKeysPdf('model','model from truth',x,data_model,RooKeysPdf.MirrorBoth,SMOOTHNESS)
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

def PrintVariables():
    #model.Print('t')
    vars = model.getVariables()
    vars.Print('v')

def Fit(data):
    # named ranges can be used in RF.Range in a comma-separated list
    #RF.Binning
    r = model.fitTo(data,RF.PrintLevel(1),RF.Range(*frange),RF.Extended(isExt),RF.NumCPU(4),RF.Save())
    frame = x.frame()
    RooAbsData.plotOn(data,frame,RF.Name('dataZ'))
    model.plotOn(frame,RF.Name('modelZ'))
    ndf = r.floatParsFinal().getSize()
    chi2.append(ndf)
    chi2.append(frame.chiSquare(ndf))
    # wildcards or comma-separated components are allowed:
    if isExt:
        model.plotOn(frame,RF.Components('exp*'),RF.LineStyle(kDashed))
    if True:
        #model.plotOn(frame,RF.VisualizeError(r))
        model.paramOn(frame,data)
    return (r,frame)

# getting data from histo
if False:
    f = TFile(_fin,'r')
    hz = f.Get(_hist_truth)
    f.Close()
    N = hz.GetN()
    print 'Loaded graph',_hist_truth,'with',N,'entries'
    pos = hz.GetX()
    neg = hz.GetY()
    dataP = RooDataSet('truth_dataP','Zmumu mu+ MC',RooArgSet(x))
    dataN = RooDataSet('truth_dataN','Zmumu mu- MC',RooArgSet(x))
    for i in range(0,N):
        if i==10000: break
        x.setVal(1.0/pos[i])
        truthP.add(RooArgSet(x))
        x.setVal(1.0/neg[i])
        truthN.add(RooArgSet(x))

    data = RooDataHist('data','Data histo',RooArgList(x),hz)
    r,frame = Fit(data)
    c = ROOT.TCanvas('c','c',800,600)
    c.cd()
    frame.Draw()
    c.SaveAs('plot.png')
    PrintVariables()
    print 'CHI2 = %.2f, NDF = %d'%(chi2[1],chi2[0])

if False:
    data = model.generate(w.set('X'),2000)
    r,frame = Fit(data)

def plot_data(data,color=ROOT.kBlack):
    frame = x.frame()
    RooAbsData.plotOn(data,frame,RF.LineColor(color),RF.MarkerColor(color))
    model.plotOn(frame)
    #model.paramOn(frame,data)
    frame.Draw()
    chi = frame.chiSquare(1)
    chi2.append(chi)
    print chi
    return frame

if True:
    c = ROOT.TCanvas('c','c',1024,768)
    c.Divide(3,3)
    ps = []
    for z in range(9):
        c.cd(z+1)
        frame = plot_data(datasN[z])
        # set pave text
        p = ROOT.TPaveText(.6,.70 , (.6+.30),(.70+.20) ,"NDC")
        p.SetTextAlign(11)
        p.SetFillColor(0)
        p.AddText('Scale=%.2f, chi2=%.2f'%(scale[z],chi2[z]))
        p.Draw()
        ps.append(p)
    c.Update()
    c.SaveAs(_plotname)
    for z in range(9):
        print scale[z],chi2[z]
