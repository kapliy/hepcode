# fitz a single z peak

import sys
_fin = 'truthzmumu.root'
_hist = 'mc_zmumu.root/dg/dg/st_z_final/BB/z_m_fine'
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
w = RooWorkspace('w',kTRUE)

# Z mass cannot float if we fit for scale
mZ = '91.1876'
# formula for Z mass:
#formula = 'sqrt((E1+E2)^2-(pz1+pz2)^2-(pt1*cos(phi1)+pt2*cos(phi2))^2-(pt1*sin(phi1)+pt2*sin(phi2))^2)'

# Voigtian with exponential
if True:
    minZ = '70.0'
    maxZ = '104.0'
    voig = "RooVoigtian::voig(x[%s,%s],mean[%s],width[0,0,5.0],sigma[1,0,5])"  #minZ,maxZ,mZ(or range)
    exp  = "RooExponential::exp(x,expar[-0.1,-1,0])"
    # since nfractions = npdfs, this is automatically an extended likelihood fit
    cmd = "SUM::sum(nsig[1,0,1000000]*%s,nbg[0,0,1000000]*%s)"%(voig%(minZ,maxZ,'%s,%s,%s'%(mZ,minZ,maxZ)),exp)
    print cmd
    w.factory(cmd)
    #w.var('width').setConstant(kTRUE) if w.var('width') else None
    isExt = kTRUE

# Simple gaussian
if False:
    minZ = '88.0'
    maxZ = '94.0'
    cmd = "RooGaussian::sum(x[%s,%s],mean[%s],sigma[1,0,5])"%(minZ,maxZ,'%s,%s,%s'%(mZ,minZ,maxZ))
    print cmd
    w.factory(cmd)
    isExt = kFALSE

frange = (float(minZ),float(maxZ))

# make a joint pdf for various signal regions
w.factory("")
w.defineSet('X','x')
model = w.pdf('sum')
nsig = w.var('nsig')
nbg = w.var('nbg')
mean = w.var('mean')
#mean.setConstant(kTRUE)
x = w.var('x')
chi2 = []

def PrintVariables():
    model.Print('t')
    vars = model.getVariables()
    vars.Print('v')

def Fit(data):
    # set some default event fractions based on the histogram
    if nsig and nbg:
        nsig.setVal(data.sumEntries())
        nbg.setVal(0)
    # named ranges can be used in RF.Range in a comma-separated list
    x.setRange('named_range',85,95)
    r = model.fitTo(data,RF.PrintLevel(-1),RF.Range(*frange),RF.Extended(isExt),RF.NumCPU(4),RF.Save())
    frame = x.frame()
    RooAbsData.plotOn(data,frame,RF.Name('dataZ'))
    model.plotOn(frame,RF.Name('modelZ'))
    ndf = r.floatParsFinal().getSize()
    chi2.append(ndf)
    chi2.append(frame.chiSquare(ndf))
    # wildcards or comma-separated components are allowed:
    if isExt:
        model.plotOn(frame,RF.Components('exp*'),RF.LineStyle(kDashed))
    if False:
        model.plotOn(frame,RF.VisualizeError(r))
        model.paramOn(frame,data)
    frame.Draw()
    return (r,frame)

# getting data from histo
if True:
    f = TFile(_fin,'r')
    hz = f.Get(_hist)
    hz.SetDirectory(0)
    print hz.GetEntries()
    f.Close()
    data = RooDataHist('data','Zmumu MC',RooArgList(x),hz)
    r,frame = Fit(data)
    PrintVariables()
    mean=w.var('mean').getVal()
    emean=w.var('mean').getError()
    sigma=w.var('sigma').getVal()
    print 'mW = %.3f +/- %.3f %.2f%% %.2f%%'%(mean,emean,(mean-fmZ)/fmZ*100.0,emean/fmZ*100.0)
    if nbg and nsig:
        print 'nsig = %d, nbg = %d'%(nsig.getVal(),nbg.getVal())
    print 'CHI2 = %.2f, NDF = %d'%(chi2[1],chi2[0])

if False:
    data = model.generate(w.set('X'),2000)
    r,frame = Fit(data)

if True:
    frame = x.frame()
    model.plotOn(frame)
    frame.Draw()
