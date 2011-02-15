#!/usr/bin/env python
# fitz a single z peak

"""
mc_zmumu_mc_zmumu.root/dg/dg/st_z_final/BB/graph_Z_m_eta
mc_zmumu_mc_zmumu.root/dg/dg/st_z_final/BB/z_m_fine

data_data.root/dg/dg/st_z_final/BB/graph_Z_m_eta
data_data.root/dg/dg/st_z_final/BB/z_m_fine

mc_zmumu_mc_zmumu.root/dg/dg/truth/st_truth_reco_z/BB/graph_Z_m_eta
mc_zmumu_mc_zmumu.root/dg/dg/truth/st_truth_reco_z/BB/z_m_fine
"""

import sys,re,array
import SimpleProgressBar

mZ = 91.1876
def func_SCALE(xx,par):
    return par*xx

from optparse import OptionParser
parser = OptionParser()
# data sources
parser.add_option("--root",dest="root",
                  type="string", default='root_all.root',
                  help="Input ROOT file with all histograms")
parser.add_option("--data",dest="data",
                  type="string", default='mc_zmumu_mc_zmumu.root/dg/dg/st_z_final/BB/graph_Z_m_eta',
                  help="TGraph or TH1F containing reconstructed z mass")
parser.add_option("--ndata",dest="ndata",
                  type="int", default=1000000,
                  help="Number of unbinned data points to load")
parser.add_option("--nskip",dest="nskip",
                  type="int", default=0,
                  help="Number of data points to skip load")
# parameters
parser.add_option("--auto",dest="auto",
                  type="float", default=None,
                  help="Automatically deduce range of x using mean and width")
parser.add_option("--min",dest="min",
                  type="float", default=70.0,
                  help="Fit minimum")
parser.add_option("--max",dest="max",
                  type="float", default=110.0,
                  help="Fit maximum")
parser.add_option("--scale",dest="scale",
                  type="float", default=1.0,
                  help="Force a particular scale factor on positive muons")
parser.add_option("--fixw", default=False,
                  action="store_true",dest="fixw",
                  help="Fix width of W")
parser.add_option("--rookeys",dest="rookeys",
                  type="string", default=None,
                  help="ROOT file from which we load a cached RooKeysPdf")
# choose what to run
parser.add_option('-t',"--tag",dest="tag",
                  type="string", default='plot',
                  help="A tag to append to all output plots")
parser.add_option('-b', "--batch", default=False,
                  action="store_true",dest="batch",
                  help="Enable batch mode (all output directly to filesystem)")
parser.add_option("--gaus", default=False,
                  action="store_true",dest="gaus",
                  help="Fit simple gaussian")
parser.add_option("--egge", default=False,
                  action="store_true",dest="egge",
                  help="Fit egge function - similar to bw, but no x^2 in denominator")
parser.add_option("--voigbg", default=False,
                  action="store_true",dest="voigbg",
                  help="Fit voigtian with exponential BG")
parser.add_option("--bw", default=False,
                  action="store_true",dest="bw",
                  help="Fit relativistic breit-wigner")
parser.add_option("--bwfull", default=False,
                  action="store_true",dest="bwfull",
                  help="Fit relativistic breit-wigner (+photon exchange & interference)")
parser.add_option("--res",dest="res",
                  type="int", default=1,
                  help="Resolution model: 0=none, 1=gaus, 2=double-gaus, 3=crystal-ball")

(opts, args) = parser.parse_args()

import ROOT
ROOT.gROOT.SetBatch(opts.batch)
from ROOT import RooWorkspace,RooArgSet,RooArgList,RooDataHist,RooAbsData,RooDataSet,RooFormulaVar
from ROOT import RooGaussModel,RooAddModel,RooRealVar,RooRealSumPdf
from ROOT import kTRUE,kFALSE,kDashed,gStyle,gPad
from ROOT import TFile,TH1,TH1F,TH1D
from ROOT import RooFit as RF
gbg = []
w = RooWorkspace('w',kTRUE)

def cmd_none():
    return ['']
def cmd_gaus():
    """ Standard Gaussian resolution shape """
    cmds = []
    cmds.append("r_m[0.0,-1,1]")
    cmds.append("r_s[2.5,0,10]")
    cmds.append('Gaussian::res(x,r_m,r_s)')
    return cmds
def cmd_dgaus():
    """ Double Gaussian resolution shape """
    cmds = []
    cmds.append("r_m[0.0,-1,1]")
    cmds.append("r_s[1.5,0,10]")
    cmds.append('Gaussian::gaus_c(x,r_m,r_s)')
    cmds.append("rt_m[0.0,-1,1]")
    cmds.append("rt_s[3,0,10]")
    cmds.append('Gaussian::gaus_t(x,rt_m,rt_s)')
    cmds.append("f[0.85]")  # fraction of core
    cmds.append("SUM::res(f*gaus_c,gaus_t)")
    return cmds
def cmd_cb():
    """ Crystal Ball resolution shape """
    cmds = []
    cmds.append("r_m[0.0,-1,1]")
    cmds.append('r_a[1.7,0,10]')
    cmds.append('r_n[1.5,0,10]')
    cmds.append("r_s[2.5,0,10]")
    cmds.append('RooCBShape::res(x,r_m,r_s,r_a,r_n)')
    return cmds
resolutions = (cmd_none,cmd_gaus,cmd_dgaus,cmd_cb)

gam={}
gam['BB'] = -0.37
gam['CC'] = -1.16
gam['AA'] = 1.10

# Voigtian with exponential
def make_voigbg(minZ,maxZ,m=mZ,fixw=False):
    """ Returns PDF and YES/NO whether it is an extended likelihood fit"""
    cmds = []
    cmds.append('m[%s]'%m)
    cmds.append('width[2.49,0,5.0]')
    cmds.append('sigma[1,0,5.0]')
    cmds.append('expar[-0.1,-1,0]')
    cmds.append("RooVoigtian::voig(x,m,width,sigma)")
    cmds.append("RooExponential::exp(x,expar)")
    cmds.append('nvoig[1,0,1000000]')
    cmds.append('nexp[1,0,1000000]')
    cmds.append("SUM::voigbg(nvoig*voig,nexp*exp)")
    [w.factory(cmd) for cmd in cmds]
    if fixw:
        w.var('width').setConstant(kTRUE) if w.var('width') else None
    return w.pdf('voigbg'), kTRUE

# Simple gaussian
def make_gaus(minZ,maxZ,m=mZ):
    cmds = []
    cmds.append('m[%s,%s,%s]'%(m,minZ,maxZ))
    cmds.append('s[1.0,0,5.0]')
    cmds.append("RooGaussian::gaus(x,m,s)")
    print cmds
    [w.factory(cmd) for cmd in cmds]
    return w.pdf('gaus'), kFALSE

# fancy shape with interference term, but no BG - via AmplitudeFit
#https://kyoko.web.cern.ch/KYOKO/DiffZ/KyokoYamamoto_DiffZ_20101217.pdf
def make_egge(minZ,maxZ,ires=1,m=mZ):
    """ Dimuon mass spectrum before FSR - includes photon and interference terms """
    cmds = []
    # coefficients for the amplitudes
    cmds.append("A[1,0,1000000]")
    cmds.append("B[1,0,1000000]")
    cmds.append("C[10000.0,0,1000000]")
    # amplitudes
    cmds.append('m[%s,%s,%s]'%(m,minZ,maxZ))
    cmds.append('g[8,0,100]')
    denom = '((x^2-m^2)^2+g^2*m^2)'
    cmds.append("expr::z_rbw('x^2/%s',x,m,g)"%denom)
    cmds.append("expr::z_int('(x^2-m^2)/%s',x,m,g)"%denom)
    cmds.append("expr::z_rad('1/(x^2+1)',x)")
    # resolution model
    cmds += resolutions[ires]()
    [w.factory(cmd) for cmd in cmds]
    # sum-of-amplitudes pdf
    lshape = RooRealSumPdf('lshape','lshape',RooArgList(w.function('z_rad'),w.function('z_int'),w.function('z_rbw')),RooArgList(w.var('A'),w.var('B'),w.var('C')))
    getattr(w,'import')(lshape)
    # convolution
    pdf = w.pdf('lshape')
    if w.pdf('res'):
        w.var('x').setBins(10000,'cache')
        cmd = 'FCONV::sum(x,lshape,res)'
        w.factory(cmd)
        pdf = w.pdf('sum')
    return pdf, kFALSE

# fancy shape with interference term, but no BG - via AmplitudeFit
def make_bwfull(minZ,maxZ,ires=1,fixw=False,m=mZ):
    """ isDouble enabled a double-gaussian resolution model """
    cmds = []
    # coefficients for the amplitudes
    cmds.append("A[1,0,1000000]")
    cmds.append("B[1,0,1000000]")
    cmds.append("C[10000.0,0,1000000]")
    # amplitudes
    cmds.append('m[%s,%s,%s]'%(m,minZ,maxZ))
    cmds.append('g[2.49,0,10]')
    denom = '((x^2-m^2)^2+x^4*g^2/m^2)'
    cmds.append("expr::z_rbw('x^2/%s',x,m,g)"%denom)
    cmds.append("expr::z_int('(x^2-m^2)/%s',x,m,g)"%denom)
    cmds.append("expr::z_rad('1/(x^2)',x)")
    # resolution model
    cmds += resolutions[ires]()
    [w.factory(cmd) for cmd in cmds]
    # any parameter adjustments
    if True:
        w.var('r_m').setConstant(kTRUE) if w.var('r_m') else None
        w.var('rt_m').setConstant(kTRUE) if w.var('rt_m') else None
        w.var('g').setConstant(kTRUE) if w.var('g') and fixw else None
    # sum-of-amplitudes pdf
    lshape = RooRealSumPdf('lshape','lshape',RooArgList(w.function('z_rad'),w.function('z_int'),w.function('z_rbw')),RooArgList(w.var('A'),w.var('B'),w.var('C')))
    getattr(w,'import')(lshape)
    # convolution
    pdf = w.pdf('lshape')
    if w.pdf('res'):
        w.var('x').setBins(10000,'cache')
        cmd = 'FCONV::sum(x,lshape,res)'
        w.factory(cmd)
        pdf = w.pdf('sum')
    return pdf, kFALSE

# Just clear bw with gaussian
def make_bw(minZ,maxZ,ires=0,m=mZ):
    cmds = []
    # amplitudes
    cmds.append('m[%s,%s,%s]'%(m,minZ,maxZ))
    cmds.append('g[2.495,0,100]')
    cmds.append("EXPR::z_rbw('x^2/((x^2-m^2)^2+x^2*g^2/m^2)',x,m,g)")
    # resolution model
    cmds += resolutions[ires]()
    [w.factory(cmd) for cmd in cmds]
    # any parameter adjustments
    w.var('r_m').setConstant(kTRUE) if w.var('r_m') else None
    pdf = w.pdf('z_rbw')
    if w.pdf('res'):
        w.var('x').setBins(10000,'cache')
        cmd = 'FCONV::sum(x,z_rbw,res)'
        w.factory(cmd)
        pdf = w.pdf('sum')
    return pdf, kFALSE

def PrintVariables():
    #model.Print('t')
    vars = model.getVariables()
    vars.Print('v')

def Fit(data,isExt,bins,extras=False):
    # named ranges can be used in RF.Range in a comma-separated list
    x = w.var('x')
    RF.Hesse(kTRUE)
    r = model.fitTo(data,RF.PrintLevel(1),RF.Extended(isExt),RF.NumCPU(4),RF.Save())
    frame = x.frame()
    if data.ClassName()=='RooDataSet':
        RooAbsData.plotOn( data,frame,RF.Name('dataZ'),RF.Binning(int((bins[1]-bins[0])/0.5)) )
    else:
        RooAbsData.plotOn(data,frame,RF.Name('dataZ'))
    model.plotOn(frame,RF.Name('modelZ'))
    ndf = r.floatParsFinal().getSize()
    chi2ndf = frame.chiSquare(ndf)
    # wildcards or comma-separated components are allowed:
    if isExt:
        model.plotOn(frame,RF.Components('exp*'),RF.LineStyle(kDashed))
    if extras:
        model.plotOn(frame,RF.VisualizeError(r))
        model.paramOn(frame,data)
    return (r,frame,chi2ndf,ndf)

def load_graph(hz,xmin,xmax,auto=None,scale=1.0):
    """ Load TGraph from a file. Note that we manually drop the points outside [xmin,xmax] range """
    N = hz.GetN()
    print 'Loading graph with',N,'entries'
    v1 = hz.GetX()
    v2 = hz.GetY()
    if auto:
        mean,width = hz.GetMean(),hz.GetRMS()
        xmin = mean-width*auto
        xmax = mean+width*auto
        print 'Limiting range to:',xmin,xmax
    w.factory('x[%s,%s]'%(xmin,xmax))
    ds1 = RooDataSet('ds1','ds1',RooArgSet(w.var('x')))
    ds2 = RooDataSet('ds2','ds2',RooArgSet(w.var('x')))
    nmax = min(N,opts.ndata); nmaxp10 = nmax if nmax>10 else 10
    bar = SimpleProgressBar.SimpleProgressBar(10,nmax)
    x = w.var('x')
    for i in range(0,nmax):
        if i%(int(nmaxp10/10))==0 and i>0:
            print bar.show(i)
        xv = func_SCALE(v1[i],scale)
        if xmin < xv < xmax:
            x.setVal(xv)
            ds1.add(RooArgSet(x))
    getattr(w,'import')(ds1,RF.Rename('ds1'))
    getattr(w,'import')(ds2,RF.Rename('ds2'))
    return ds1,ds2

def load_histo(hz,xmin,xmax,auto=None):
    """ Load TH1 histogram from a file """
    print 'Loading histogram with',hz.GetEntries(),'entries'
    if auto:
        mean,width = hz.GetMean(),hz.GetRMS()
        xmin = mean-width*auto
        xmax = mean+width*auto
        print 'Limiting range to:',xmin,xmax
    w.factory('x[%s,%s]'%(xmin,xmax))
    data = RooDataHist('data','Zmumu MC',RooArgList(w.var('x')),hz)
    return data

# getting data
if True:
    # load the data
    f = TFile(opts.root,'r')
    hz = f.Get(opts.data)
    cname = hz.ClassName()
    if cname == 'TGraph':
        data,crap = load_graph(hz,opts.min,opts.max,opts.auto,opts.scale)
    elif re.search('TH',cname):
        hz.SetDirectory(0)
        data = load_histo(hz,opts.min,opts.max,opts.auto)
    f.Close()
    # choose fit shape
    if opts.gaus:
        model,isExt = make_gaus(opts.min,opts.max)
    elif opts.bw:
        model,isExt = make_bw(opts.min,opts.max,opts.res)
    elif opts.egge:
        model,isExt = make_egge(opts.min,opts.max,opts.res)
    elif opts.bwfull:
        model,isExt = make_bwfull(opts.min,opts.max,opts.res)
    elif opts.voigbg:
        model,isExt = make_voigbg(opts.min,opts.max)
    else:
        print 'Need at least one shape flag. Exiting...'
        sys.exit(0)
    # set some default event fractions
    if w.var('nsig') and w.var('nbg'):
        w.var('nsig').setVal(data.sumEntries())
        w.var('nbg').setVal(0)
    if w.var('C'):
        w.var('C').setVal(data.sumEntries())
    # perform the fit
    bins = (opts.min,opts.max)
    r,frame,chi2ndf,ndf = Fit(data,isExt,bins)
    frame.SetTitle(opts.tag)
    # make the plots
    c = ROOT.TCanvas('c','c'); c.cd()
    frame.Draw()
    # print fitted value on canvas
    if w.var('m'):
        m = w.var('m')
        p = ROOT.TPaveText(.7,.8 , (.7+.2),(.8+.1) ,"NDC")
        p.SetTextAlign(11)
        p.SetFillColor(0)
        p.AddText('mZ = %.3f +/- =%.3f'%(m.getVal(),m.getError()))
        p.AddText('chi2/dof = %.1f'%(chi2ndf))
        p.Draw()
        gbg.append(p)
    c.SaveAs('%s_fit.png'%opts.tag)
    PrintVariables()
    m=w.var('m').getVal()
    em=w.var('m').getError()
    print 'mz = %.3f +/- %.3f'%(m,em)
    print 'CHI2/NDF = %.2f, NDF = %d'%(chi2ndf,ndf)
