#!/usr/bin/env python

# repeats z peak after manually smearing muon pt
# the objective is to demonstrate the effect of smearing on fitted z peak position.


try:
    import psyco
    psyco.full()
except ImportError:
    pass

import sys,re,array,math
from math import sqrt,fabs
import SimpleProgressBar
from load_data import *
#from fit_defaults import *

def func_SCALE(xx,par):
    return par*xx

from optparse import OptionParser
parser = OptionParser()
# data sources
parser.add_option("--root",dest="root",
                  type="string", default='root_all.root',
                  help="Input ROOT file with all histograms")
parser.add_option("--region",dest="region",
                  type="string", default='BB',
                  help="Where each leg of a Z must fall")
parser.add_option("--data",dest="data",
                  type="string", default='data_data.root/dg/dg/st_z_final/ntuple',
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
parser.add_option("--ncpus",dest="ncpus",
                  type="int", default=4,
                  help="Number of CPUs to use in fitting")
parser.add_option("--rookeys",dest="rookeys",
                  type="string", default=None,
                  help="ROOT file from which we load a cached RooKeysPdf")
# for scale fit
parser.add_option("--mz0",dest="mz0",
                  type="float", default=91.1876,
                  help="Nominal value of z mass")
parser.add_option("--R",dest="R",
                  type="float", default=None,
                  help="Value of R=k+/k- scale factor")
parser.add_option("--eR",dest="eR",
                  type="float", default=None,
                  help="Error on the value of R=k+/k- scale factor")
parser.add_option("--kp",dest="kp",
                  type="float", default=None,
                  help="Value of k+ scale factor")
parser.add_option("--km",dest="km",
                  type="float", default=None,
                  help="Value of k- scale factor")
parser.add_option("--ekp",dest="ekp",
                  type="float", default=None,
                  help="Error of k+ scale factor")
parser.add_option("--ekm",dest="ekm",
                  type="float", default=None,
                  help="Error of k- scale factor")
# choose what to run
parser.add_option('-t',"--tag",dest="tag",
                  type="string", default='plot',
                  help="A tag to append to all output plots")
parser.add_option("--ext",dest="ext",
                  type="string", default='png',
                  help="Extension for all output")
parser.add_option('-b', "--batch", default=False,
                  action="store_true",dest="batch",
                  help="Enable batch mode (all output directly to filesystem)")
parser.add_option("--gaus", default=False,
                  action="store_true",dest="gaus",
                  help="Fit simple gaussian")
parser.add_option("--egge", default=False,
                  action="store_true",dest="egge",
                  help="Fit egge function - similar to bw, but no x^2 in denominator")
parser.add_option("--voig", default=False,
                  action="store_true",dest="voig",
                  help="Fit voigtian without BG")
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
mz0 = opts.mz0

import ROOT
ROOT.gROOT.SetBatch(opts.batch)
ROOT.gROOT.LoadMacro("AtlasStyle.C")
ROOT.SetAtlasStyle()
from ROOT import RooWorkspace,RooArgSet,RooArgList,RooDataHist,RooAbsData,RooDataSet,RooFormulaVar
from ROOT import RooGaussModel,RooAddModel,RooRealVar,RooAbsReal,RooRealSumPdf
from ROOT import kTRUE,kFALSE,kDashed,gStyle,gPad
from ROOT import TFile,TH1,TH1F,TH1D
from ROOT import RooFit as RF
gbg = []; COUT = []
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
    #cmds.append("r_m[0.0,-1,1]")
    cmds.append("r_m[0.0]")
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
def make_voigbg(minZ,maxZ,m=mz0,fixw=False):
    """ Returns PDF and YES/NO whether it is an extended likelihood fit"""
    cmds = []
    cmds.append('m[%s,%s,%s]'%(m,minZ,maxZ))
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

# Simple voigtian
def make_voig(minZ,maxZ,m=mz0,fixw=False):
    """ Returns PDF and YES/NO whether it is an extended likelihood fit"""
    cmds = []
    cmds.append('m[%s,%s,%s]'%(m,minZ,maxZ))
    cmds.append('width[2.49,0,5.0]')
    cmds.append('sigma[1,0,5.0]')
    cmds.append("RooVoigtian::voig(x,m,width,sigma)")
    [w.factory(cmd) for cmd in cmds]
    if fixw:
        w.var('width').setConstant(kTRUE) if w.var('width') else None
    return w.pdf('voig'), kFALSE

# Simple gaussian
def make_gaus(minZ,maxZ,m=mz0):
    cmds = []
    cmds.append('m[%s,%s,%s]'%(m,minZ,maxZ))
    cmds.append('s[1.0,0,5.0]')
    cmds.append("RooGaussian::gaus(x,m,s)")
    [w.factory(cmd) for cmd in cmds]
    return w.pdf('gaus'), kFALSE

# fancy shape with interference term, but no BG - via AmplitudeFit
#https://kyoko.web.cern.ch/KYOKO/DiffZ/KyokoYamamoto_DiffZ_20101217.pdf
def make_egge(minZ,maxZ,ires=1,m=mz0):
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
def make_bwfull(minZ,maxZ,ires=1,fixw=False,m=mz0):
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
def make_bw(minZ,maxZ,ires=0,m=mz0):
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

def Fit(data,isExt,fullbins,fitbins,ncpus=4,extras=False,gaus=False):
    # plot the data first
    x = w.var('x'); xtra = None
    frame = x.frame(RF.Title('Invariant mass fit using'))
    if data.ClassName()=='RooDataSet':
        RooAbsData.plotOn(data,frame,RF.Name('dataZ'),RF.Binning(int((fullbins[1]-fullbins[0])/0.5)) )
    else:
        RooAbsData.plotOn(data,frame,RF.Name('dataZ'))
    x.setRange('it1',fitbins[0],fitbins[1])
    RF.Hesse(kTRUE)
    r = model.fitTo(data,RF.PrintLevel(1),RF.Extended(isExt),RF.NumCPU(ncpus),RF.Save(),RF.Range('it1'))
    model.plotOn(frame,RF.Name('mit1'),RF.Range('it1'),RF.NormRange('it1'))
    if gaus:
        nsc = 1.0  #1.25
        mean=w.var('m').getVal()
        sigma=w.var('s').getVal()
        xtra = 'mz(1st) = %.3f +/- %.3f'%(w.var('m').getVal(),w.var('m').getError())
        ndf = r.floatParsFinal().getSize()
        chi2ndf = frame.chiSquare(ndf)
        print 'SECOND ITERATION: mean =',mean,'sigma =',sigma,'chi2dof =',chi2ndf
        x.setRange('it2',mean-sigma*nsc,mean+sigma*nsc)
        r = model.fitTo(data,RF.LineColor(ROOT.kRed),RF.PrintLevel(1),RF.Extended(isExt),RF.NumCPU(ncpus),RF.Save(),RF.Range('it2'))
        model.plotOn(frame,RF.Name('mit2'),RF.Range('it2'),RF.NormRange('it2'),RF.LineColor(ROOT.kRed))
    ndf = r.floatParsFinal().getSize()
    chi2ndf = frame.chiSquare(ndf)
    # wildcards or comma-separated components are allowed:
    if isExt:
        model.plotOn(frame,RF.Components('exp*'),RF.LineStyle(kDashed))
    if extras:
        model.plotOn(frame,RF.VisualizeError(r))
        model.paramOn(frame,data)
    return (r,frame,chi2ndf,ndf,xtra)

def load_unbinned(N,v1,xmin,xmax,scale=1.0):
    """ Load TGraph from a file. Note that we manually drop the points outside [xmin,xmax] range """
    w.factory('x[%s,%s]'%(xmin,xmax))
    ds1 = RooDataSet('ds1','ds1',RooArgSet(w.var('x')))
    nmax = min(N,opts.ndata); nmaxp10 = nmax if nmax>10 else 10
    bar = SimpleProgressBar.SimpleProgressBar(10,nmax)
    x = w.var('x')
    nf=0
    for i in range(0,nmax):
        if i%(int(nmaxp10/10))==0 and i>0:
            print bar.show(i)
        xv = func_SCALE(v1[i],scale)
        if xmin < xv < xmax:
            x.setVal(xv)
            ds1.add(RooArgSet(x))
            nf+=1
    print 'Final count of unbinned data:',nf
    return ds1

if True:
    # load the data
    f = TFile(opts.root,'r')
    hz = f.Get(opts.data)
    assert hz, 'Error loading data object %s from file %s'%(opts.data,opts.root)
    cname = hz.ClassName()
    assert cname in ('TGraph','TNtuple','TTree','TChain')
    N,Zs = ntuple_to_4vectors(hz,opts.region,opts.min,opts.max,maxdata=opts.ndata)
    POS,NEG = ROOT.TLorentzVector(), ROOT.TLorentzVector()
    gRand = ROOT.TRandom3(1)
    MS_MS      = [0.02428,0.0678,0.0362,0.0237]
    MS_AL      = [0.000191,0.00019,0.00012,0.000669]
    def psm(pt,eta):
        detRegion = -1
        if (fabs(eta)<1.05):
            detRegion=0
        elif (fabs(eta)<1.7):
            detRegion=1
        elif (fabs(eta)<2.0):
            detRegion=2;
        elif (fabs(eta)<2.5):
            detRegion=3;
        g1,g2 = [gRand.Gaus(0,1) for ij in range(2)]
        return MS_MS[detRegion]*g1+MS_AL[detRegion]*g2*pt
    ms = []
    for i in range(0,N):
        POS.SetPtEtaPhiM(*Zs[i][0])
        NEG.SetPtEtaPhiM(*Zs[i][1])
        ptPOS,ptNEG = Zs[i][0][0],Zs[i][1][0]
        etaPOS,etaNEG = Zs[i][0][1],Zs[i][1][1]
        POS.SetPerp(ptPOS * (1.0 + psm(ptPOS,etaPOS)))
        NEG.SetPerp(ptNEG * (1.0 + psm(ptNEG,etaNEG)))
        ms.append((POS+NEG).M())
    data = load_unbinned(N, array.array('f',ms), opts.min , opts.max)
    # choose fit shape
    if opts.gaus:
        model,isExt = make_gaus(opts.min,opts.max)
    elif opts.bw:
        model,isExt = make_bw(opts.min,opts.max,opts.res)
    elif opts.egge:
        model,isExt = make_egge(opts.min,opts.max,opts.res)
    elif opts.bwfull:
        model,isExt = make_bwfull(opts.min,opts.max,opts.res)
    elif opts.voig:
        model,isExt = make_voig(opts.min,opts.max)
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
    # Define fit range. Constrained to the core if using simple Gaussian
    fullbins = (opts.min,opts.max)
    fitbins = fullbins
    if opts.gaus:
        dGeV = opts.auto if opts.auto else 6.0  # 4.0
        peak = data.mean(w.var('x'))
        print 'OLD FIT RANGE:',opts.min,opts.max,'; MEAN:',peak
        fitbins = [peak - dGeV, peak + dGeV]
        print 'NEW FIT RANGE:',fitbins[0],fitbins[1]
    # perform the fit
    r,frame,chi2ndf,ndf,xtra = Fit(data,isExt,fullbins,fitbins,ncpus=opts.ncpus,gaus=opts.gaus)
    frame.SetTitle(opts.tag)
    # make the plots
    c = ROOT.TCanvas('c','c'); c.cd()
    frame.Draw()
    # print fitted value on canvas
    if w.var('m'):
        m = w.var('m')
        p = ROOT.TPaveText(.7,.75 , (.7+.2),(.75+.15) ,"NDC")
        p.SetTextAlign(11)
        p.SetFillColor(0)
        p.AddText('mz = %.3f +/- %.3f'%(m.getVal(),m.getError()))
        if opts.gaus:
            p.AddText('sigma = %.3f +/- %.3f'%(w.var('s').getVal(),w.var('s').getError()))
        if opts.egge:
            p.AddText('gamma = %.3f +/- %.3f'%(w.var('g').getVal(),w.var('g').getError()))
        if xtra:
            p.AddText(xtra)
        p.AddText('chi2/dof = %.1f. N=%d'%(chi2ndf,data.sumEntries()))
        p.Draw()
        gbg.append(p)
    SaveAs(c,'%s_fit'%opts.tag,opts.ext)
    PrintVariables()
    mz=w.var('m').getVal()
    emz=w.var('m').getError()
    COUT.append( 'mz0 = %.3f'%(mz0) )
    COUT.append( 'mz = %.3f +/- %.3f'%(mz,emz) )
    COUT.append( 'CHI2/NDF = %.2f, NDF = %d'%(chi2ndf,ndf) )
    # Solve for scales given a ratio factor and one detector region
    if opts.R and opts.eR:
        R,eR = opts.R,opts.eR
        km = mz/(mz0*sqrt(R))
        kp = mz*sqrt(R)/(mz0)
        ekm = sqrt( emz**2 * 1/(mz0*sqrt(R))**2 + eR**2 * (mz/mz0)**2 / (4 * R**3) )
        ekp = sqrt( emz**2 * (sqrt(R)/mz0)**2 + eR**2 * (mz/mz0)**2 / (4 * R) )
        ekm1,ekm2 = sqrt(emz**2 * 1/(mz0*sqrt(R))**2),sqrt(eR**2 * (mz/mz0)**2 / (4 * R**3))
        ekp1,ekp2 = sqrt(emz**2 * (sqrt(R)/mz0)**2), sqrt(eR**2 * (mz/mz0)**2 / (4 * R))
        COUT.append( 'R = %.3f +/- %.3f'%(R,eR) )
        COUT.append( 'k+ = %.3f +/- %.3f ( %.3f %.3f )'%(kp*100.0,ekp*100.0,ekp1*100.0,ekp2*100.0) )
        COUT.append( 'k- = %.3f +/- %.3f ( %.3f %.3f )'%(km*100.0,ekm*100.0,ekm1*100.0,ekm2*100.0) )
    # Solve for k-, given k+ (e.g., one leg in barrel, another leg in endcap) 
    if opts.kp and opts.ekp:
        kp,ekp = opts.kp,opts.ekp
        km = (mz/mz0)**2/kp
        ekm = sqrt( emz**2 * (2*mz/(mz0**2*kp))**2 + ekp**2 * ((mz/mz0)**2/kp**2)**2 )
        COUT.append( 'k- = %.3f +/- %.3f'%(km*100.0,ekm*100.0) )
    # Solve for k+, given k- (e.g., one leg in barrel, another leg in endcap) 
    if opts.km and opts.ekm:
        km,ekm = opts.km,opts.ekm
        kp = (mz/mz0)**2/km
        ekp = sqrt( emz**2 * (2*mz/(mz0**2*km))**2 + ekm**2 * ((mz/mz0)**2/km**2)**2 )
        COUT.append( 'k+ = %.3f +/- %.3f'%(kp*100.0,ekp*100.0) )

# save to text file
if len(COUT)>0:
    fout = open('%s_results.rtxt'%opts.tag,'w')
    for l in COUT:
        print l
        print >>fout,l
    fout.close()
