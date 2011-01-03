#!/usr/bin/env python
# fitz a single z peak

import sys
_fin = 'root_all.root'
if len(sys.argv)>=2:
    _fin = sys.argv[1]

import ROOT
ROOT.PyConfig.IgnoreCommandLineOptions = True
from ROOT import RooWorkspace,RooArgSet,RooArgList,RooDataHist,RooDataSet,RooAbsData,RooAbsReal,RooFormulaVar,RooSimWSTool
from ROOT import kTRUE,kFALSE,kDashed,gStyle,gPad,gDirectory
from ROOT import TFile,TH1,TH1F,TH1D
from ROOT import RooFit as RF

# Z mass cannot float if we fit for scale
mZ = '91.1876'
minZ = '70.0'
maxZ = '104.0'
frange = (float(minZ),float(maxZ))

# formula for Z mass:
#formula = 'sqrt((E1+E2)^2-(pz1+pz2)^2-(pt1*cos(phi1)+pt2*cos(phi2))^2-(pt1*sin(phi1)+pt2*sin(phi2))^2)'

voig = "RooVoigtian::voig%s(expr('x*sqrt(%s*%s)',x[%s,%s],%s[1.0,0.9,1.1],%s[1.0,0.9,1.1]),mean[%s],width[3.4,0,6],sigma%s[2.0,0,5])"  #name,s1,s2,minZ,maxZ,s1,s2,mZ
exp  = "RooExponential::exp%s(expr('x*sqrt(%s*%s)',x,%s,%s),expar[-0.05,-1.0,0])" # name,s1,s2,s1,s2

w = RooWorkspace('w',kTRUE)
regmap = {'AA':0,'AB':1,'AC':2,'BA':3,'BB':4,'BC':5,'CA':6,'CB':7,'CC':8}
regions = ('AA','AB','AC','BA','BB','BC','CA','CB','CC')
regions = ('AA','AB','BA','BB','BC','CB','CC')
#regions = ('AA','AB',)
# create PDFs for each region
for reg in regions:
    p = 'p%s'%reg[0]
    n = 'n%s'%reg[1]
    # since nfractions = npdfs, this is automatically an extended likelihood fit
    cmd = "SUM::pdf%s(nsig%s[1,0,1000000]*%s,nbg%s[0,0,1000000]*%s)"%(reg,reg,voig%(reg,p,n,minZ,maxZ,p,n,mZ,reg),reg,exp%(reg,p,n,p,n))
    print cmd
    w.factory(cmd)
# fix Z mass mean
mean = w.var('mean')
mean.setConstant(kTRUE)

cmd = "SIMUL::model(k[%s],%s)"%(','.join(["k%s"%reg for reg in regions]),','.join(["k%s=pdf%s"%(reg,reg) for reg in regions]))
print cmd
w.factory(cmd)
# make workspace visible on command line
gDirectory.Add(w)

w.defineSet('X','x')
model = w.pdf('model')
x = w.var('x')

def PrintVariables():
    #model.Print('t')
    vars = model.getVariables()
    vars.Print('v')

def Fit(data):
    # named ranges can be used in RF.Range in a comma-separated list
    x.setRange('named_range',85,95)
    r = model.fitTo(data,RF.PrintLevel(-1),RF.Range(*frange),RF.Extended(kTRUE),RF.NumCPU(4))
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
        nevt = w.var('nbg%s'%reg).getVal() + w.var('nsig%s'%reg).getVal()
        #norm = RF.Normalization(1.0,RooAbsReal.RelativeExpected)
        norm = RF.Normalization(nevt,RooAbsReal.NumEvent)
        model.plotOn(frame,slice,proj)
        model.plotOn(frame,slice,proj,RF.Components('exp%s*'%reg),RF.LineStyle(kDashed))
        model.plotOn(frame,slice,proj,RF.VisualizeError(r))
        #model.paramOn(frame,data)
        frame.Draw()
    return c

if True:
    # getting histograms from file
    f = TFile(_fin,'r')
    hz = {}
    hargs = []
    for reg in regions:
        hz[reg] = f.Get('data.root/dg/dg/st_z_final/%s/z_m'%reg)
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
        w.var('nsig%s'%reg).setVal(_nsig)
        w.var('nbg%s'%reg).setVal(_nsig/20.0)
    c = Fit(data)
    c.Update()
    PrintVariables()
