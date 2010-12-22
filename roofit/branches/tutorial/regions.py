# separating Z's into 9 detector regions

import sys
_fin = 'root_all.root'
reg='AA'
if len(sys.argv)>=2:
    reg = sys.argv[1]

mZ = 91.1876

import ROOT
from ROOT import RooWorkspace,RooArgSet,RooArgList,RooDataHist,RooAbsData
from ROOT import kTRUE,kDashed
from ROOT import TFile,TH1,TH1F,TH1D
from ROOT import RooFit as RF

w = RooWorkspace('w',kTRUE)
#w.factory('RooVoigtian::voig(x[60.0,120.0],mean[87,85,95],width[2.5],sigma[3,0,10])')
w.factory('RooVoigtian::voig(x[84.0,97.0],mean[87,85,95],width[2.5],sigma[3,0,10])')
w.factory('RooExponential::exp(x,expar[-0.1,-1,0])')
w.factory('SUM::sum(nsig[385000,0,400000]*voig,nbg[1,0,1000]*exp)')  # automatically extended likelihood!
w.defineSet('X','x')
w.defineSet('bg','exp')
x = w.var('x')

def Fit(data):
    r = w.pdf('sum').fitTo(data)
    frame = x.frame(RF.Title("Detector region: %s"%reg))
    RooAbsData.plotOn(data,frame)
    w.pdf('sum').plotOn(frame)
    w.pdf('sum').plotOn(frame,RF.Components(w.set('bg')),RF.LineStyle(kDashed))
    w.pdf('sum').plotOn(frame,RF.VisualizeError(r))
    #w.pdf('sum').paramOn(frame,data)
    frame.Draw()
    return frame

# getting data from histo
def FitHisto(hz):
    n = hz.GetEntries()
    data = RooDataHist('data','Zmumu MC',RooArgList(x),hz)
    frame = Fit(data)
    mean=w.var('mean').getVal()
    emean=w.var('mean').getError()
    width=w.var('width').getVal()
    sigma=w.var('sigma').getVal()
    nbg=w.var('nbg').getVal()
    nsig=w.var('nsig').getVal()
    print 'mW = %.3f +\- %.3f'%(mean,emean)
    print 'nsig = %d, nbg = %d'%(nsig,nbg)
    print 'reg',reg,n,'%.3f +/- %.3f %.2f%% %.2f%%'%(mean,emean,(mean-mZ)/mZ*100.0,emean/mZ*100.0)
    return frame

f = ROOT.TFile.Open(_fin,'r')
c = ROOT.TCanvas('c','c',1024,768)
c.Divide(3,3)
g = f.GetDirectory('data.root').GetDirectory('dg').GetDirectory('dg').GetDirectory('st_z_final')
regs = ('AA','AB','AC','BA','BB','BC','CA','CB','CC')
for i,reg in enumerate(regs):
    c.cd(i+1)
    hz = g.Get('%s/z_m'%reg)
    hz.SetDirectory(0)
    FitHisto(hz)
c.SaveAs('reg_all.png')
f.Close()
