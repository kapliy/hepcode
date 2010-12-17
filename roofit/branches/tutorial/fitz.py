import sys
_fin = 'zspectrum.root'
if len(sys.argv)>=2:
    _fin = sys.argv[1]

import ROOT
from ROOT import RooWorkspace,RooArgSet,RooArgList,RooDataHist,RooAbsData
from ROOT import kTRUE,kDashed
from ROOT import TFile,TH1,TH1F,TH1D
from ROOT import RooFit as RF

w = RooWorkspace('w',kTRUE)
#w.factory('RooVoigtian::voig(x[60.0,120.0],mean[87,85,95],width[2.5],sigma[3,0,10])')
w.factory('RooVoigtian::voig(x[84.0,97.0],mean[87,85,95],width[2.5],sigma[3,0,10])')
w.factory('RooExponential::exp(x,expar[-0.1])')
w.factory('SUM::sum(nsig[385000,0,400000]*voig,nbg[1,0,1000]*exp)')
w.defineSet('X','x')
w.defineSet('bg','exp')
x = w.var('x')

def Fit(data):
    w.pdf('sum').fitTo(data)
    mesframe = x.frame()
    RooAbsData.plotOn(data,mesframe)
    w.pdf('sum').plotOn(mesframe)
    w.pdf('sum').plotOn(mesframe,RF.Components(w.set('bg')),RF.LineStyle(kDashed))
    mesframe.Draw()

def FitSIG(data):
    w.pdf('voig').fitTo(data) ;
    mesframe = x.frame()
    RooAbsData.plotOn(data,mesframe)
    w.pdf('voig').plotOn(mesframe)
    w.pdf('voig').plotOn(mesframe,RF.Components(w.set('bg')),RF.LineStyle(kDashed))
    mesframe.Draw()

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
