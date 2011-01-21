#!/usr/bin/env python

# fits a single z peak using RooKeysPDF

import sys
_fin = 'root_all.root'
_hist = 'data_data.root/dg/dg/st_z_final/BB/graph_lpt_P_N'
_hist_truth = 'mc_zmumu_mc_zmumu.root/dg/dg/st_z_final/BB/graph_lpt_P_N'
_hist = _hist_truth
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
# formula for Z mass:
#formula = 'sqrt((E1+E2)^2-(pz1+pz2)^2-(pt1*cos(phi1)+pt2*cos(phi2))^2-(pt1*sin(phi1)+pt2*sin(phi2))^2)'

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
    for i in range(0,N):
        if MAXLOAD>0 and i>=MAXLOAD: break
        x.setVal(1.0/pos[i])
        truthP.add(RooArgSet(x))
        x.setVal(1.0/neg[i])
        truthN.add(RooArgSet(x))
    getattr(w,'import')(truthP)
    getattr(w,'import')(truthN)
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
    # make truth(x) for positive and negative muons
    dataP = RooDataSet('dataP','Zmumu mu+ data',RooArgSet(x))
    dataN = RooDataSet('dataN','Zmumu mu- data',RooArgSet(x))
    for i in range(0,N):
        if MAXLOAD>0 and i>=MAXLOAD: break
        x.setVal(1.0/pos[i])
        dataP.add(RooArgSet(x))
        x.setVal(1.0/neg[i])
        dataN.add(RooArgSet(x))
    getattr(w,'import')(dataP)
    getattr(w,'import')(dataN)
    # make truth(x*b)
    w.factory("expr::xf('x*1.0/b',x,b)")
    xf = w.function('xf')
    data = truthP
    model = RooKeysPdf('model','model from truth',x,data,RooKeysPdf.NoMirror,SMOOTHNESS)
    #model = RooHistPdf('model','model from truth',RooArgSet(w.var('x')),truth,2)
    #model = RooHistPdf('model','model from truth',RooArgList(xf),RooArgList(w.var('x')),truth,4)
    getattr(w,'import')(model)
    w.writeToFile('workspace.root')
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

if True:
    frame = x.frame()
    model.plotOn(frame)
    frame.Draw()
