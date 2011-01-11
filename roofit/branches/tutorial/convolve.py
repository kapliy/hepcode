#!/usr/bin/env python

# playing with roofit convolution

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

mZ = '91.1876'
minZ = '70.0'
maxZ = '104.0'

# example convolution
if False:
    cmd = "BreitWigner::bw(x[%s,%s],mean[91.18],width[2.495,0.1,10])"%(minZ,maxZ)
    cmd = 'Landau::T(x[-10,30],m_t[5,-20,20],s_t[1,0.1,10])'
    w.factory(cmd)
    cmd = 'Gaussian::R(x,m_r[0],s_r[2,0.1,10])'
    #cmd = 'Gaussian::R(x,m_r[0],s_r[0.001,0.1,10])' # test narrow resolution
    w.factory(cmd)
    w.var('x').setBins(10000,'cache')
    cmd = 'FCONV::model(x,T,R)'
    w.factory(cmd)

# manual again
if True:
    minZ='70.0'
    maxZ='110.0'
    A='1.0'
    B='100.0'
    C='10000.0'
    # Prepare the Z lineshape
    cmd = "EXPR::bw('A/(x*x)+B*(x*x-mean*mean)/((x*x-mean*mean)*(x*x-mean*mean)+width*width*mean*mean)+C*x*x/((x*x-mean*mean)*(x*x-mean*mean)+width*width*mean*mean)',x[{0},{1}],mean[91.18,{0},{1}],width[2.495,0.1,10],A[{2}],B[{3}],C[{4}])".format(minZ,maxZ,A,B,C)
    w.factory(cmd)
    # Prepare the Gaussian
    cmd = "Gaussian::gaus(x,m_r[0.0],sigma[2.5,0.1,10])"
    w.factory(cmd)
    w.var('x').setBins(10000,'cache')
    conv = 'FCONV::model(x,bw,gaus)'
    w.factory(conv)
    w.var('m_r').setConstant(kTRUE) if w.var('m_r') else None

# make a joint pdf for various signal regions
frange = (float(minZ),float(maxZ))
w.factory("")
w.defineSet('X','x')
model = w.pdf('model')
nsig = w.var('nsig')
nbg = w.var('nbg')
mean = w.var('mean')
x = w.var('x')
chi2 = []


def pdf_components():
    cmds = []
    cmds.append( "EXPR::z_rad('1/(x^2)',x[%s,%s])"%(minZ,maxZ))
    cmds.append( "EXPR::z_int('(x^2-mean^2)/((x^2-mean^2)^2+width^2*mean^2)',x,mean[91.18,%s,%s],width[2.495,0.1,10])"%(minZ,maxZ) )
    cmds.append( "EXPR::z_rbw('x^2/((x^2-mean^2)^2+width^2*mean^2)',x,mean,width)" )
    cmds.append( "SUM::bw(A[1.0]*z_rad,B[1.0]*z_int,C[10000]*z_rbw)" )
    # better way:
    minZ='70.0'
    maxZ='104.0'
    A='1.0'
    B='1.0'
    C='10000.0'
    cmd = "EXPR::bw('A/(x^2)+B*(x^2-mean^2)/((x^2-mean^2)^2+width^2*mean^2)+C*x^2/((x^2-mean^2)^2+width^2*mean^2)',x[{0},{1}],mean[91.18,{0},{1}],width[2.495,0.1,10],A[{2}],B[{3}],C[{4}])".format(minZ,maxZ,A,B,C)
    return [cmd,]

def PrintVariables():
    #model.Print('t')
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
    model.paramOn(frame,data)
    frame.Draw()
    return (r,frame)

if True:
    frame = x.frame()
    model.plotOn(frame)
    frame.Draw()
