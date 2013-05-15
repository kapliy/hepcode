#!/usr/bin/env python

# fits a single z peak using a histogram template

import sys
_fin = 'root_all.root'
_hist = 'data_data.root/dg/dg/st_z_final/BB/leptonP_invp'
_hist_truth = 'mc_zmumu_mc_zmumu.root/dg/dg/st_z_final/BB/leptonP_invp'
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
from ROOT import RooWorkspace,RooArgSet,RooArgList,RooDataHist,RooAbsData,RooFormulaVar,RooHistPdf
from ROOT import kTRUE,kFALSE,kDashed,gStyle,gPad
from ROOT import TFile,TH1,TH1F,TH1D
from ROOT import RooFit as RF
w = RooWorkspace('w',kTRUE)

# Z mass cannot float if we fit for scale
mZ = '91.1876'
# formula for Z mass:
#formula = 'sqrt((E1+E2)^2-(pz1+pz2)^2-(pt1*cos(phi1)+pt2*cos(phi2))^2-(pt1*sin(phi1)+pt2*sin(phi2))^2)'

# preparing a PDF from truth histogram
if True:
    f = TFile(_fin,'r')
    hz = f.Get(_hist_truth)
    hz.SetDirectory(0)
    f.Close()
    print 'Loaded histo',_hist_truth,'with',int(hz.GetEntries()),'entries'
    # variables and parameters
    w.factory('x[%s,%s]'%(minZ,maxZ))
    w.factory('b[1.0,0.9,1.1]')
    # make truth(x)
    truth = RooDataHist('truth_data','Zmumu MC',RooArgList(w.var('x')),hz)
    getattr(w,'import')(truth)
    # make truth(x*b)
    w.factory("expr::xf('x*1.0/b',x,b)")
    #model = RooHistPdf('model_truth','model from truth',RooArgSet(w.var('x')),truth,2)
    model = RooHistPdf('model','model from truth',RooArgList(w.function('xf')),RooArgList(w.var('x')),truth,4)
    getattr(w,'import')(model)
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
if True:
    f = TFile(_fin,'r')
    hz = f.Get(_hist)
    hz.SetDirectory(0)
    f.Close()
    print hz.GetEntries()
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
