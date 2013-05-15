# fitz a single z peak

import sys
_fin = 'root_all.root'
_hist = 'mc_zmumu_mc_zmumu.root/dg/dg/st_z_final/BB/z_m_fine'
if len(sys.argv)>=2:
    _fin = sys.argv[1]
if len(sys.argv)>=3:
    _hist = sys.argv[2]

import ROOT
from ROOT import RooWorkspace,RooArgSet,RooArgList,RooDataHist,RooAbsData,RooFormulaVar
from ROOT import kTRUE,kFALSE,kDashed,gStyle,gPad
from ROOT import TFile,TH1,TH1F,TH1D
from ROOT import RooFit as RF
w = RooWorkspace('w',kTRUE)

# Z mass cannot float if we fit for scale
mZ = '91.1876'
minZ = '70.0'
maxZ = '104.0'
frange = (float(minZ),float(maxZ))

# formula for Z mass:
#formula = 'sqrt((E1+E2)^2-(pz1+pz2)^2-(pt1*cos(phi1)+pt2*cos(phi2))^2-(pt1*sin(phi1)+pt2*sin(phi2))^2)'

# scale factor part of mean
if False:
    voig = "RooVoigtian::voig(x[%s,%s],expr('mean*%s',mean[%s],%s[1.0,0.9,1.1]),width[0,0,5.0],sigma[1,0,5])"  #minZ,maxZ,s,mZ,s
    exp  = "RooExponential::exp(x,expar[-0.1,-1,0])"
    w.factory("SUM::sum(nsig[1,0,1000000]*%s,nbg[0,0,1000000]*%s)"%(voig%(minZ,maxZ,'a0',mZ,'a0'),exp))

# scale factor part of x both in sig and bg
if True:
    voig = "RooVoigtian::voig(expr('x*%s',x[%s,%s],%s[1.0,0.9,1.1]),mean[%s],width[0,0,5.0],sigma[1,0,5])"  #s,minZ,maxZ,s,mZ
    exp  = "RooExponential::exp(expr('x*%s',x,%s),expar[-0.1,-1,0])" # s,s
    w.factory("SUM::sum(nsig[1,0,1000000]*%s,nbg[0,0,1000000]*%s)"%(voig%('a0',minZ,maxZ,'a0',mZ),exp%('a0','a0')))

# make a joint pdf for various signal regions
w.factory("")
w.defineSet('X','x')
model = w.pdf('sum')
nsig = w.var('nsig')
nbg = w.var('nbg')
mean = w.var('mean')
mean.setConstant(kTRUE)
x = w.var('x')

def PrintVariables():
    model.Print('t')
    vars = model.getVariables()
    vars.Print('v')

def Fit(data):
    # set some default event fractions based on the histogram
    nsig.setVal(data.sumEntries())
    nbg.setVal(0)
    # named ranges can be used in RF.Range in a comma-separated list
    x.setRange('named_range',85,95)
    r = model.fitTo(data,RF.PrintLevel(-1),RF.Range(*frange),RF.Extended(kTRUE),RF.NumCPU(4))
    frame = x.frame()
    RooAbsData.plotOn(data,frame)
    model.plotOn(frame)
    # wildcards or comma-separated components are allowed:
    model.plotOn(frame,RF.Components('exp*'),RF.LineStyle(kDashed))
    model.plotOn(frame,RF.VisualizeError(r))
    model.paramOn(frame,data)
    frame.Draw()

# getting data from histo
if True:
    f = TFile(_fin,'r')
    hz = f.Get(_hist)
    hz.SetDirectory(0)
    print hz.GetEntries()
    f.Close()
    data = RooDataHist('data','Zmumu MC',RooArgList(x),hz)
    Fit(data)
    PrintVariables()
    mean=w.var('mean').getVal()
    emean=w.var('mean').getError()
    width=w.var('width').getVal()
    sigma=w.var('sigma').getVal()
    nbg=w.var('nbg').getVal()
    nsig=w.var('nsig').getVal()
    print 'mW = %.3f +\- %.3f'%(mean,emean)
    print 'nsig = %d, nbg = %d'%(nsig,nbg)

if False:
    data = model.generate(w.set('X'),2000)
    Fit(data)
