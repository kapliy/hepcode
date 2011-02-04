#!/usr/bin/env python
# fitz a single z peak

import sys
_fin = 'root_all.root'
_hist = 'mc_zmumu_mc_zmumu.root/dg/dg/st_z_final/BB/z_m_fine'
if len(sys.argv)>=2:
    _fin = sys.argv[1]
if len(sys.argv)>=3:
    _hist = sys.argv[2]

fmZ = 91.1876

import ROOT
from ROOT import RooWorkspace,RooArgSet,RooArgList,RooDataHist,RooAbsData,RooFormulaVar,RooRealSumPdf
from ROOT import RooGaussModel,RooAddModel,RooRealVar
from ROOT import kTRUE,kFALSE,kDashed,gStyle,gPad
from ROOT import TFile,TH1,TH1F,TH1D
from ROOT import RooFit as RF
w = RooWorkspace('w',kTRUE)

# Z mass cannot float if we fit for scale
mZ = '91.1876'
# formula for Z mass:
#formula = 'sqrt((E1+E2)^2-(pz1+pz2)^2-(pt1*cos(phi1)+pt2*cos(phi2))^2-(pt1*sin(phi1)+pt2*sin(phi2))^2)'

# Voigtian with exponential
if False:
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

# fancy shape with interference term, but no BG
# TODO - RooRealSumPdf with amplitudes
# see: http://root.cern.ch/root/html/tutorials/roofit/rf704_amplitudefit.C.html
if False:
    minZ='70.0'
    maxZ='110.0'
    A='1.0,0,1000000'
    B='100.0,0,1000000'
    C='10000.0,0,1000000'
    # Prepare the Z lineshape
    cmd = "EXPR::bw('A/(x*x)+B*(x*x-mean*mean)/((x*x-mean*mean)*(x*x-mean*mean)+width*width*mean*mean)+C*x*x/((x*x-mean*mean)*(x*x-mean*mean)+width*width*mean*mean)',x[{0},{1}],mean[91.18,{0},{1}],width[2.495,0.1,10],A[{2}],B[{3}],C[{4}])".format(minZ,maxZ,A,B,C)
    w.factory(cmd)
    # Prepare the Gaussian
    cmd = "Gaussian::gaus(x,m_r[0.0],sigma[2.5,0.1,10])"
    w.factory(cmd)
    w.var('x').setBins(10000,'cache')
    conv = 'FCONV::sum(x,bw,gaus)'
    w.factory(conv)
    w.var('m_r').setConstant(kTRUE) if w.var('m_r') else None
    isExt = kFALSE

# fancy shape with interference term, but no BG - via AmplitudeFit
if True:
    minZ='70.0'
    maxZ='110.0'
    A='1000.0,0,1000000'
    B='1000.0,0,1000000'
    C='10000.0,0,1000000'
    # Prepare the Z lineshape
    cmds = []
    cmds.append( "expr::z_rad('1/(x^2)',x[%s,%s])"%(minZ,maxZ))
    cmds.append( "expr::z_int('(x^2-mean^2)/((x^2-mean^2)^2+width^2*mean^2)',x,mean[91.18,%s,%s],width[2.495,0.1,10])"%(minZ,maxZ) )
    cmds.append( "expr::z_rbw('x^2/((x^2-mean^2)^2+width^2*mean^2)',x,mean,width)" )
    cmds.append( "A[%s]"%A )
    cmds.append( "B[%s]"%B )
    cmds.append( "C[%s]"%C )
    cmds.append( "Gaussian::gaus(x,m_r[0.0],sigma[2.5,0.1,10])" )
    [w.factory(cmd) for cmd in cmds]
    lshape = RooRealSumPdf('lshape','lshape',RooArgList(w.function('z_rad'),w.function('z_int'),w.function('z_rbw')),RooArgList(w.var('A'),w.var('B'),w.var('C')))
    getattr(w,'import')(lshape)
    if True:  # double-gaussian
        x = w.var('x')
        m_r = w.var('m_r')
        sigma_core = RooRealVar("sigma_core","sigma core",1) ;
        gaussm_core = RooGaussModel("gaussm_core","core gauss",x,m_r,sigma_core) ;
        sigma_tail = RooRealVar("sigma_tail","sigma tail",5) ;
        gaussm_tail = RooGaussModel("gaussm_tail","tail gauss",x,m_r,sigma_tail) ;
        frac_core = RooRealVar("frac_core","core fraction",0.85) ;
        gaussm = RooAddModel("gaussm","core+tail gauss", RooArgList(gaussm_core,gaussm_tail),RooArgList(frac_core)) ; 
        getattr(w,'import')(gaussm)
    w.var('x').setBins(10000,'cache')
    cmd = 'FCONV::sum(x,lshape,gaus)'
    cmd = 'FCONV::sum(x,lshape,gaussm)'
    w.factory(cmd)
    w.var('m_r').setConstant(kTRUE) if w.var('m_r') else None
    isExt = kFALSE

frange = (float(minZ),float(maxZ))

# make a joint pdf for various signal regions
w.factory("")
w.defineSet('X','x')
model = w.pdf('sum')
nsig,nbg = w.var('nsig'),w.var('nbg')
A,B,C = w.var('A'),w.var('B'),w.var('C')
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
    if C:
        C.setVal(data.sumEntries())
    # named ranges can be used in RF.Range in a comma-separated list
    x.setRange('named_range',85,95)
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
    if False:
        model.plotOn(frame,RF.VisualizeError(r))
        model.paramOn(frame,data)
    return (r,frame)

# getting data from histo
if True:
    c = ROOT.TCanvas('c','c'); c.cd()
    f = TFile(_fin,'r')
    hz = f.Get(_hist)
    hz.SetDirectory(0)
    print hz.GetEntries()
    f.Close()
    data = RooDataHist('data','Zmumu MC',RooArgList(x),hz)
    r,frame = Fit(data)
    c.cd()
    frame.Draw()
    c.SaveAs('plot.png')
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

if False:
    frame = x.frame()
    model.plotOn(frame)
    frame.Draw()
