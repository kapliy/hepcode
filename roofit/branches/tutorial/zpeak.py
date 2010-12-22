# fitz a single z peak

import sys
_fin = 'zspectrum.root'
if len(sys.argv)>=2:
    _fin = sys.argv[1]

import ROOT
from ROOT import RooWorkspace,RooArgSet,RooArgList,RooDataHist,RooAbsData,RooFormulaVar
from ROOT import kTRUE,kDashed
from ROOT import TFile,TH1,TH1F,TH1D
from ROOT import RooFit as RF

mZ = 91.1876

# formula for Z mass:
#formula = 'sqrt((E1+E2)^2-(pz1+pz2)^2-(pt1*cos(phi1)+pt2*cos(phi2))^2-(pt1*sin(phi1)+pt2*sin(phi2))^2)'
#RooGenericPdf model_mean("model_mean","abs(mean)<a",RooArgList(mean,a))
#w.factory("CEXPR::GC('x*sqrt(pos*neg)',{x,pox,neg})")


w = RooWorkspace('w',kTRUE)
#w.factory('RooVoigtian::voig(x[84.0,97.0],mean[87,85,95],width[2.5],sigma[3,0,10])')
w.factory("RooVoigtian::voig(expr('x*a0',x[84.0,97.0],a0[1.0,0.9,1.1]),mean[%f],width[2.5],sigma[3,0,10])"%mZ)
w.factory('RooExponential::exp(x,expar[-0.1,-1,0])')
w.factory('SUM::sum(nsig[385000,0,400000]*voig,nbg[1,0,1000]*exp)')  # automatically extended likelihood!
w.defineSet('X','x')
w.defineSet('bg','exp')
x = w.var('x')

def Fit(data):
    r = w.pdf('sum').fitTo(data)
    frame = x.frame()
    RooAbsData.plotOn(data,frame)
    w.pdf('sum').plotOn(frame)
    w.pdf('sum').plotOn(frame,RF.Components(w.set('bg')),RF.LineStyle(kDashed))
    w.pdf('sum').plotOn(frame,RF.VisualizeError(r))
    w.pdf('sum').paramOn(frame,data)
    frame.Draw()

# getting data from histo
if True:
    f = TFile(_fin,'r')
    hz = f.Get('z_m')
    hz.SetDirectory(0)
    print hz.GetEntries()
    f.Close()
    data = RooDataHist('data','Zmumu MC',RooArgList(x),hz)
    Fit(data)
    mean=w.var('mean').getVal()
    emean=w.var('mean').getError()
    width=w.var('width').getVal()
    sigma=w.var('sigma').getVal()
    nbg=w.var('nbg').getVal()
    nsig=w.var('nsig').getVal()
    print 'mW = %.3f +\- %.3f'%(mean,emean)
    print 'nsig = %d, nbg = %d'%(nsig,nbg)

if False:
    data = w.pdf('sum').generate(w.set('X'),2000)
    Fit(data)
