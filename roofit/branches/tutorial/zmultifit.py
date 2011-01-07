#!/usr/bin/env python
# fitz a single z peak

import sys
_fin = 'root_all.root'
_hist = 'data.root/dg/dg/st_z_final/%s/z_m_fine'
if len(sys.argv)>=2:
    _fin = sys.argv[1]
if len(sys.argv)>=3:
    _hist = sys.argv[2]
print 'INPUTS SETTINGS:'
print _fin
print _hist

import ROOT
ROOT.PyConfig.IgnoreCommandLineOptions = True
from ROOT import RooWorkspace,RooArgSet,RooArgList,RooDataHist,RooDataSet,RooAbsData,RooAbsReal,RooFormulaVar,RooSimWSTool
from ROOT import kTRUE,kFALSE,kDashed,gStyle,gPad,gDirectory
from ROOT import TFile,TH1,TH1F,TH1D
from ROOT import RooFit as RF
w = RooWorkspace('w',kTRUE)

# Z mass cannot float if we fit for scale
mZ = '91.1876'
# formula for Z mass:
#formula = 'sqrt((E1+E2)^2-(pz1+pz2)^2-(pt1*cos(phi1)+pt2*cos(phi2))^2-(pt1*sin(phi1)+pt2*sin(phi2))^2)'

regmap = {'AA':0,'AB':1,'AC':2,'BA':3,'BB':4,'BC':5,'CA':6,'CB':7,'CC':8}
regions = ('AA','AB','AC','BA','BB','BC','CA','CB','CC')
regions = ('AA','AB','BA','BB','BC','CB','CC')

if True:  # fixed Z width
    # default best range:
    minZ = '70.0'
    maxZ = '104.0'
    # narrow range test:
    #minZ = '85.0'
    #maxZ = '96.0'
    minZ = '82.0'
    maxZ = '98.0'
    voig = "RooVoigtian::voig%s(expr('x*sqrt(%s*%s)',x[%s,%s],%s[1.0,0.9,1.1],%s[1.0,0.9,1.1]),mean[%s],width[2.495,0,6],sigma%s[2.0,0,5])"  #name,s1,s2,minZ,maxZ,s1,s2,mZ
    exp  = "RooExponential::exp%s(expr('x*sqrt(%s*%s)',x,%s,%s),expar[-0.05,-1.0,0])" # name,s1,s2,s1,s2
    # create PDFs for each region
    for reg in regions:
        p = 'p%s'%reg[0]
        n = 'n%s'%reg[1]
        # since nfractions = npdfs, this is automatically an extended likelihood fit
        cmd = "SUM::pdf%s(nsig%s[1,0,1000000]*%s,nbg%s[0,0,1000000]*%s)"%(reg,reg,voig%(reg,p,n,minZ,maxZ,p,n,mZ,reg),reg,exp%(reg,p,n,p,n))
        print cmd
        w.factory(cmd)
        w.pdf('voig%s'%reg).selectDefaultAlgorithm() if w.pdf('voig%s'%reg) else None
    # fix Z mass mean
    w.var('mean').setConstant(kTRUE) if w.var('mean') else None
    # fix Z linewidth
    #w.var('width').setConstant(kTRUE) if w.var('width') else None
    isExt = kTRUE

if False:  # simple gaussian
    minZ = '86.0'
    maxZ = '96.0'
    gaus = "RooGaussian::pdf%s(expr('x*sqrt(%s*%s)',x[%s,%s],%s[1.0,0.9,1.1],%s[1.0,0.9,1.1]),mean[%s],sigma%s[2.0,0,5])"  #name,s1,s2,minZ,maxZ,s1,s2,mZ
    # create PDFs for each region
    for reg in regions:
        p = 'p%s'%reg[0]
        n = 'n%s'%reg[1]
        # since nfractions = npdfs, this is automatically an extended likelihood fit
        cmd = gaus%(reg,p,n,minZ,maxZ,p,n,mZ,reg)
        print cmd
        w.factory(cmd)
    # fix Z mass mean
    mean = w.var('mean')
    mean.setConstant(kTRUE)
    isExt = kFALSE

cmd = "SIMUL::model(k[%s],%s)"%(','.join(["k%s"%reg for reg in regions]),','.join(["k%s=pdf%s"%(reg,reg) for reg in regions]))
print cmd
w.factory(cmd)
# make workspace visible on command line
gDirectory.Add(w)

frange = (float(minZ),float(maxZ))
w.defineSet('X','x')
model = w.pdf('model')
x = w.var('x')
chi2s = {}

def PrintVariables():
    #model.Print('t')
    vars = model.getVariables()
    vars.Print('v')

def Fit(data):
    # named ranges can be used in RF.Range in a comma-separated list
    x.setRange('named_range',85,95)
    # set print level to -1 for non-verbose
    r = model.fitTo(data,RF.PrintLevel(1),RF.Range(*frange),RF.Extended(isExt),RF.NumCPU(4),RF.Save())
    r.correlationMatrix().Print()
    ndf = r.floatParsFinal().getSize()
    print 'NDF =',ndf
    c = ROOT.TCanvas('c','c',1024,768)
    c.Divide(3,3)
    k = w.cat('k')
    for reg in regions:
        c.cd(regmap[reg]+1)
        frame = x.frame(RF.Title("Detector region: %s"%reg))
        cut = RF.Cut("k==k::k%s"%reg)
        RooAbsData.plotOn(data,frame,cut)
        k.setLabel('k%s'%reg)
        slice = RF.Slice(k,'k%s'%reg)
        proj = RF.ProjWData(RooArgSet(k),data)
        if isExt:
            nevt = w.var('nbg%s'%reg).getVal() + w.var('nsig%s'%reg).getVal()
            norm = RF.Normalization(1.0,RooAbsReal.RelativeExpected)
            norm = RF.Normalization(nevt,RooAbsReal.NumEvent)
        model.plotOn(frame,slice,proj)
        chi2s[reg] = frame.chiSquare(6)  #voig(3) + exp(1) + scale(1) + norm(1)
        if isExt:
            model.plotOn(frame,slice,proj,RF.Components('exp%s*'%reg),RF.LineStyle(kDashed))
        if False:
            model.plotOn(frame,slice,proj,RF.VisualizeError(r))
            model.paramOn(frame,data)
        frame.Draw()
    return c,r

if True:
    # getting histograms from file
    f = TFile(_fin,'r')
    hz = {}
    hargs = []
    for reg in regions:
        hz[reg] = f.Get(_hist%reg)
        hz[reg].SetDirectory(0)
        hargs.append(RF.Import('k%s'%reg,hz['%s'%reg]))
    f.Close()
    data = RooDataHist('data','Zmumu joint data',RooArgList(x),RF.Index(w.cat('k')),*hargs)
    data.Print()
    # set some default event fractions based on the histogram
    for reg in regions:
        _nsig = hz[reg].GetEntries()
        _nsig = hz[reg].Integral(hz[reg].FindBin(float(minZ)),hz[reg].FindBin(float(maxZ)))
        print 'Default nsig (%s): %s'%(reg,_nsig)
        w.var('nsig%s'%reg).setVal(_nsig) if w.var('nsig%s'%reg) else None
        w.var('nbg%s'%reg).setVal(_nsig/20.0) if w.var('nbg%s'%reg) else None
    c,r = Fit(data)
    c.Update()
    c.SaveAs('plot.png')
    print 'Fit status & cov quality & num invalid NLL & edm:',r.status(),r.covQual(),r.numInvalidNLL(),r.edm()
    for reg in regions:
        print 'CHI2[%s]=%s'%(reg,chi2s[reg])
    PrintVariables()
