#!/usr/bin/env python
# fitz a single z peak

"""
mc_zmumu_mc_zmumu.root/dg/dg/st_z_final/BB/graph_Z_m_eta
data_data.root/dg/dg/st_z_final/BB/z_m_fine
mc_zmumu_mc_zmumu.root/dg/dg/st_z_final/BB/z_m_fine
data_data.root/dg/dg/st_z_final/BB/graph_Z_m_eta
"""

import sys,re,array
import SimpleProgressBar

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
parser.add_option("--batch", default=False,
                  action="store_true",dest="batch",
                  help="Enable batch mode (all output directly to filesystem)")
parser.add_option("--gaus", default=False,
                  action="store_true",dest="gaus",
                  help="Fit simple gaussian")
parser.add_option("--voigbg", default=False,
                  action="store_true",dest="voigbg",
                  help="Fit voigtian with exponential BG")
parser.add_option("--bw", default=False,
                  action="store_true",dest="bw",
                  help="Fit relativistic breit-wigner convolved with gaus")
parser.add_option("--bwfull", default=False,
                  action="store_true",dest="bwfull",
                  help="Fit relativistic breit-wigner (+photon exchange & interference) convolved with gaus")
parser.add_option("--double", default=False,
                  action="store_true",dest="double",
                  help="Use double gaussian resolution model for convolution?")

(opts, args) = parser.parse_args()

mZ = 91.1876

import ROOT
ROOT.gROOT.SetBatch(opts.batch)
from ROOT import RooWorkspace,RooArgSet,RooArgList,RooDataHist,RooAbsData,RooDataSet,RooFormulaVar
from ROOT import RooGaussModel,RooAddModel,RooRealVar,RooRealSumPdf
from ROOT import kTRUE,kFALSE,kDashed,gStyle,gPad
from ROOT import TFile,TH1,TH1F,TH1D
from ROOT import RooFit as RF
w = RooWorkspace('w',kTRUE)

# Voigtian with exponential
def make_voigbg(minZ,maxZ,mZ=mZ,fixw=False):
    """ Returns PDF and YES/NO whether it is an extended likelihood fit"""
    cmds = []
    cmds.append('x[%s,%s]'%(minZ,maxZ))
    cmds.append('mz[%s]'%mZ)
    cmds.append('width[0,0,5.0]')
    cmds.append('sigma[1,0,5.0]')
    cmds.append('expar[-0.1,-1,0]')
    cmds.append("RooVoigtian::voig(x,mz,width,sigma)")
    cmds.append("RooExponential::exp(x,expar)")
    cmds.append('nvoig[1,0,1000000]')
    cmds.append('nexp[1,0,1000000]')
    cmds.append("SUM::voigbg(nvoig*voig,nexp*exp)")
    [w.factory(cmd) for cmd in cmds]
    if fixw:
        w.var('width').setConstant(kTRUE) if w.var('width') else None
    return w.pdf('voigbg'), kTRUE

# Simple gaussian
def make_gaus(minZ,maxZ,mZ=mZ):
    cmds = []
    cmds.append('x[%s,%s]'%(minZ,maxZ))
    cmds.append('mz[%s,%s,%s]'%(mZ,minZ,maxZ))
    cmds.append('sigma[1.0,0,5.0]')
    cmds.append("RooGaussian::gaus(x,mz,sigma)")
    print cmds
    [w.factory(cmd) for cmd in cmds]
    return w.pdf('gaus'), kFALSE

# fancy shape with interference term, but no BG - via AmplitudeFit
def make_bwfull(minZ,maxZ,isDouble=False,mZ=mZ):
    """ isDouble enabled a double-gaussian resolution model """
    cmds = []
    # coefficients for the amplitudes
    cmds.append("A[1,0,1000000]")
    cmds.append("B[1,0,1000000]")
    cmds.append("C[10000.0,0,1000000]")
    # amplitudes
    cmds.append('x[%s,%s]'%(minZ,maxZ))
    cmds.append('mz[%s,%s,%s]'%(mZ,minZ,maxZ))
    cmds.append('g[8,0,100]')
    denom = '(x^2-mz^2)^2+x^4*g^2/mz^2'
    cmds.append("expr::z_rbw('x^2/(%s)^2',x,mz,g)"%denom)
    cmds.append("expr::z_int('(x^2-mz^2)/(%s)^2',x,mz,g)"%denom)
    cmds.append("expr::z_rad('1/(x^2)',x)")
    # gaussian resolution
    cmds.append("m_r[0.0,0,10]")
    cmds.append("sigma[2,0,10]")
    cmds.append( "Gaussian::gaus(x,m_r,sigma)" )
    [w.factory(cmd) for cmd in cmds]
    # any parameter adjustments
    if False:
        w.var('m_r').setConstant(kTRUE) if w.var('m_r') else None
    lshape = RooRealSumPdf('lshape','lshape',RooArgList(w.function('z_rad'),w.function('z_int'),w.function('z_rbw')),RooArgList(w.var('A'),w.var('B'),w.var('C')))
    getattr(w,'import')(lshape)
    lgaus='gaus'
    if isDouble:  # double-gaussian
        x = w.var('x')
        mean_core = RooRealVar("mean_core","mean core",0,0,10) ;
        sigma_core = RooRealVar("sigma_core","sigma core",1.5,0,10) ;
        gaussm_core = RooGaussModel("gaussm_core","core gauss",x,mean_core,sigma_core) ;
        mean_tail = RooRealVar("mean_tail","mean tail",0,0,10) ;
        sigma_tail = RooRealVar("sigma_tail","sigma tail",3,0,10) ;
        gaussm_tail = RooGaussModel("gaussm_tail","tail gauss",x,mean_tail,sigma_tail) ;
        frac_core = RooRealVar("frac_core","core fraction",0.85) ;
        gaussm = RooAddModel("gaussm","core+tail gauss", RooArgList(gaussm_core,gaussm_tail),RooArgList(frac_core)) ; 
        getattr(w,'import')(gaussm)
        if True:
            w.var('mean_core').setConstant(kTRUE) if w.var('mean_core') else None
            w.var('mean_tail').setConstant(kTRUE) if w.var('mean_tail') else None
        lgaus = 'gaussm'
    w.var('x').setBins(10000,'cache')
    cmd = 'FCONV::sum(x,lshape,%s)'%lgaus
    w.factory(cmd)
    return w.pdf('sum'), kFALSE

# Just clear bw with gaussian
def make_bw(minZ,maxZ,mZ=mZ):
    cmds = []
    # amplitudes
    cmds.append('x[%s,%s]'%(minZ,maxZ))
    cmds.append('mz[%s,%s,%s]'%(mZ,minZ,maxZ))
    cmds.append('g[2.495,0,100]')
    cmds.append("EXPR::z_rbw('x^2/((x^2-mz^2)^2+x^2*g^2/mz^2)',x,mz,g)")
    # gaussian resolution
    cmds.append("m_r[0.0]")
    cmds.append("sigma[1.5,0,100]")
    cmds.append( "Gaussian::gaus(x,m_r,sigma)" )
    [w.factory(cmd) for cmd in cmds]
    # any parameter adjustments
    w.var('m_r').setConstant(kTRUE) if w.var('m_r') else None
    w.var('x').setBins(10000,'cache')
    cmd = 'FCONV::sum(x,z_rbw,gaus)'
    w.factory(cmd)
    return w.pdf('sum'), kFALSE

def PrintVariables():
    #model.Print('t')
    vars = model.getVariables()
    vars.Print('v')

def Fit(data,isExt,bins,extras=False):
    # named ranges can be used in RF.Range in a comma-separated list
    x = w.var('x')
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

def load_graph(hz,xmin,xmax,scale=1.0):
    """ Load TGraph from a file. Note that we manually drop the points outside [xmin,xmax] range """
    N = hz.GetN()
    print 'Loading graph with',N,'entries'
    v1 = hz.GetX()
    v2 = hz.GetY()
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

def load_histo(hz):
    """ Load TH1 histogram from a file """
    print 'Loading histogram with',hz.GetEntries(),'entries'
    data = RooDataHist('data','Zmumu MC',RooArgList(w.var('x')),hz)
    return data

# getting data
if True:
    # choose fit shape
    if opts.gaus:
        model,isExt = make_gaus(opts.min,opts.max)
    elif opts.bw:
        model,isExt = make_bw(opts.min,opts.max)
    elif opts.bwfull:
        model,isExt = make_bwfull(opts.min,opts.max,opts.double)
    elif opts.voigbg:
        model,isExt = make_voigbg(opts.min,opts.max)
    else:
        print 'Need at least one shape flag. Exiting...'
        sys.exit(0)
    # load the data
    f = TFile(opts.root,'r')
    hz = f.Get(opts.data)
    cname = hz.ClassName()
    if cname == 'TGraph':
        data,crap = load_graph(hz,opts.min,opts.max,opts.scale)
    elif re.search('TH',cname):
        hz.SetDirectory(0)
        data = load_histo(hz)
    f.Close()
    # set some default event fractions
    if w.var('nsig') and w.var('nbg'):
        w.var('nsig').setVal(data.sumEntries())
        w.var('nbg').setVal(0)
    if w.var('C'):
        w.var('C').setVal(data.sumEntries())
    # perform the fit
    bins = (opts.min,opts.max)
    r,frame,chi2ndf,ndf = Fit(data,isExt,bins)
    # make the plots
    c = ROOT.TCanvas('c','c'); c.cd()
    frame.Draw()
    c.SaveAs('%s_fit.png'%opts.tag)
    PrintVariables()
    mz=w.var('mz').getVal()
    emz=w.var('mz').getError()
    print 'mZ = %.3f +/- %.3f'%(mz,emz)
    print 'CHI2/NDF = %.2f, NDF = %d'%(chi2ndf,ndf)
