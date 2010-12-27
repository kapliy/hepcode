# fitz a single z peak

import sys
_fin = 'zspectrum.root'
_hist = 'z_m'
if len(sys.argv)>=2:
    _fin = sys.argv[1]
if len(sys.argv)>=3:
    _hist = sys.argv[2]

fmZ = 91.1876

import ROOT
from ROOT import RooWorkspace,RooArgSet,RooArgList,RooDataHist,RooAbsData,RooFormulaVar
from ROOT import kTRUE,kFALSE,kDashed,gStyle,gPad
from ROOT import TFile,TH1,TH1F,TH1D
from ROOT import RooFit as RF

# Z mass cannot float if we fit for scale
mZ = '91.1876'
minZ = '70.0'
maxZ = '104.0'
frange = (float(minZ),float(maxZ))

# formula for Z mass:
#formula = 'sqrt((E1+E2)^2-(pz1+pz2)^2-(pt1*cos(phi1)+pt2*cos(phi2))^2-(pt1*sin(phi1)+pt2*sin(phi2))^2)'

voig = "RooVoigtian::voig(x[%s,%s],mean[%s],width[0,0,5.0],sigma[1,0,5])"  #minZ,maxZ,mZ(or range)
exp  = "RooExponential::exp(x,expar[-0.1,-1,0])"

w = RooWorkspace('w',kTRUE)
# since nfractions = npdfs, this is automatically an extended likelihood fit
cmd = "SUM::sum(nsig[1,0,1000000]*%s,nbg[0,0,1000000]*%s)"%(voig%(minZ,maxZ,'%s,%s,%s'%(mZ,minZ,maxZ)),exp)
print cmd
w.factory(cmd)

# make a joint pdf for various signal regions
w.factory("")
w.defineSet('X','x')
model = w.pdf('sum')
nsig = w.var('nsig')
nbg = w.var('nbg')
mean = w.var('mean')
#mean.setConstant(kTRUE)
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
    print 'mW = %.3f +/- %.3f %.2f%% %.2f%%'%(mean,emean,(mean-fmZ)/fmZ*100.0,emean/fmZ*100.0)
    print 'nsig = %d, nbg = %d'%(nsig,nbg)

if False:
    data = model.generate(w.set('X'),2000)
    Fit(data)
