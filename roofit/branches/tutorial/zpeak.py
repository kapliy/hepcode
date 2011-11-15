#!/usr/bin/env python
# fits a single z peak

_QUALITY = ' && lP_idhits==1 && fabs(lP_z0)<10. && fabs(lP_d0sig)<10. && fabs(lP_pt_id-lP_pt_exms)/lP_pt_id<0.5'
_QUALITY += ' && lN_idhits==1 && fabs(lN_z0)<10. && fabs(lN_d0sig)<10. && fabs(lN_pt_id-lN_pt_exms)/lN_pt_id<0.5'
_PRE_PETER='fabs(lP_eta)<2.5 && lP_pt>20.0 && fabs(lN_eta)<2.5 && lN_pt>20.0 && lP_ptiso40<2.0 && lP_etiso40<2.0 && lN_ptiso40<2.0 && lN_etiso40<2.0 && Z_m>60 && (lP_q*lN_q)<0'+_QUALITY

#loosen isolation, tighten d0/z0:
if False:
    _QUALITY = ' && lP_idhits==1 && fabs(lP_z0)<10. && fabs(lP_d0sig)<10. && fabs(lP_pt_id-lP_pt_exms)/lP_pt_id<0.5'
    _QUALITY += ' && lN_idhits==1 && fabs(lN_z0)<10. && fabs(lN_d0sig)<10. && fabs(lN_pt_id-lN_pt_exms)/lN_pt_id<0.5'
    _PRE_PETER='fabs(lP_eta)<2.5 && lP_pt>20.0 && fabs(lN_eta)<2.5 && lN_pt>20.0 && lN_ptiso20/lN_pt<0.1 && Z_m>50 && (lP_q*lN_q)<0 && fabs(lP_z0-lN_z0)<3 && fabs(lP_d0-lN_d0)<2 && fabs(lP_phi-lN_phi)>2.0'+_QUALITY

try:
    import psyco
    psyco.full()
except ImportError:
    pass

import sys,re,array,math,glob
from math import sqrt
import SimpleProgressBar
from load_data import *
from antondb import *
#from fit_defaults import *

def func_SCALE(xx,par):
    return par*xx

from optparse import OptionParser
parser = OptionParser()
# data sources
parser.add_option("--rootdata",dest="rootdata",
                  type="string", default='ROOT/root_all_0630_newiso_noscale_1fb_cmb/data_period*/root_data_period*.root',
                  help="Input ROOT file (primary)")
parser.add_option("--rootmc",dest="rootmc",
                  type="string", default='', #ROOT/root_all_0630_newiso_noscale_1fb_cmb/mc_zmumu/root_*.root',
                  help="Input ROOT file (secondary) - for data/MC zmass mode")
parser.add_option("--tt",dest="tt",
                  type="string", default='cmb',
                  help="Type of muons: {cmb,id,exms}")
parser.add_option("--pre",dest="pre",
                  type="string", default=_PRE_PETER,
                  help="Preliminary cuts to select final W candidates")
parser.add_option("--data",dest="data",
                  type="string", default='dg/st_z_final/ntuple',
                  help="ntuple name for data")
parser.add_option("--mc",dest="mc",
                  type="string", default='dg/st_z_final/ntuple',
                  help="ntuple name for MC")
parser.add_option("--region",dest="region",
                  type="string", default='BB',
                  help="Where each leg of a Z must fall")
parser.add_option("--ndata",dest="ndata",
                  type="int", default=1000000,
                  help="Number of unbinned data points to load")
parser.add_option("--nmc",dest="nmc",
                  type="int", default=100000,
                  help="Number of unbinned MC points to load")
parser.add_option("--nskip",dest="nskip",
                  type="int", default=0,
                  help="Number of data points to skip load")
parser.add_option("--antondb",dest="antondb",
                  type="string", default=None,
                  help="Tag for antondb output container")
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
parser.add_option('-f',"--func",dest="func",
                  type="string", default='gaus',
                  help="func = {gaus,egge,voig,voigbg,bw,bwfull}{0=none;1=gaus;2=double-gaus;3=crystal-ball}")

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
w = RooWorkspace('w',kTRUE)
gbg = []; COUT = []
# antondb containers
VMAP = {}; OMAP = []
VMAP['cmd']=' '.join(sys.argv)

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
    res1st = []
    if gaus:
        nsc = 1.0  #1.25
        mean=w.var('m').getVal()
        sigma=w.var('s').getVal()
        xtra = 'mz(1st) = %.3f +/- %.3f'%(w.var('m').getVal(),w.var('m').getError())
        ndf = r.floatParsFinal().getSize()
        chi2ndf = frame.chiSquare(ndf)
        res1st += [mean,sigma,chi2ndf,ndf]
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
    return (r,frame,chi2ndf,ndf,xtra,res1st)

def load_unbinned(hz,tt,pre,reg,xmin,xmax,ndata,scale=1.0):
    """ Load TGraph from a file. Note that we manually drop the points outside [xmin,xmax] range """
    if hz.ClassName() in ('TNtuple','TTree','TChain'):
        N,v1 = ntuple_to_array1(hz,tt,reg,xmin,xmax,ndata,pre=pre)
    else:
        print 'Problem loading class',hz.ClassName()
        sys.exit(0)
    print 'Loaded raw unbinned data with',N,'entries'
    w.factory('x[%s,%s]'%(xmin,xmax))
    ds1 = RooDataSet('ds1','ds1',RooArgSet(w.var('x')))
    ds2 = RooDataSet('ds2','ds2',RooArgSet(w.var('x')))
    nmax = min(N,ndata); nmaxp10 = nmax if nmax>10 else 10
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
    #getattr(w,'import')(ds1,RF.Rename('ds1'))
    #getattr(w,'import')(ds2,RF.Rename('ds2'))
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

def fortruth(pre):
    """ removes pre variables that are not applicable to truth tracks """
    res = []
    for elm in pre.split(' && '):
        if re.search('iso',elm) or re.search('idhits',elm) or re.search('d0',elm) or re.search('z0',elm) or re.search('exms',elm):
            continue
        res.append(elm)
    out = ' && '.join(res)
    print 'PRE : ', out
    return out

# getting data
if True:
    print 'Ntuple path (data):',opts.data
    # load data
    hdata = ROOT.TChain(opts.data)
    for fname in glob.glob(opts.rootdata):
        print 'Adding to TChain:',fname
        nadd = hdata.Add(fname)
        assert nadd>0,'Failed to add file %s'%fname
    print 'Loaded data trees with %d entries'%hdata.GetEntries()
    assert hdata.GetEntries()>0, 'Error loading data object %s from file %s'%(opts.data,opts.rootdata)
    cname = hdata.ClassName()
    if cname in ('TGraph','TNtuple','TTree','TChain'):
        if re.search('truth',opts.data):
            opts.pre = fortruth(opts.pre)
        data,crap = load_unbinned(hdata,opts.tt,opts.pre,opts.region,opts.min,opts.max,opts.nmc,opts.scale)
    # load MC
    mc = None
    if opts.rootmc:
        print 'Ntuple path (mc):',opts.mc
        hmc = ROOT.TChain(opts.mc)
        for fname in glob.glob(opts.rootmc):
            print 'Adding to TChain:',fname
            nadd = hmc.Add(fname)
            assert nadd>0,'Failed to add file %s'%fname
        print 'Loaded data trees with %d entries'%hmc.GetEntries()
        assert hmc.GetEntries()>0, 'Error loading data object %s from file %s'%(opts.mc,opts.rootmc)
        cname = hmc.ClassName()
        if cname in ('TGraph','TNtuple','TTree','TChain'):
            mc,crap = load_unbinned(hmc,opts.tt,opts.pre,opts.region,opts.min,opts.max,opts.ndata,opts.scale)
    # choose fit shape
    func,res='gaus',0
    if opts.func[-1].isdigit():
        func = opts.func[:-1]
        res = int(opts.func[-1])
    else:
        func = opts.func
    if func=='gaus':
        model,isExt = make_gaus(opts.min,opts.max)
    elif func=='bw':
        model,isExt = make_bw(opts.min,opts.max,res)
    elif func=='egge':
        model,isExt = make_egge(opts.min,opts.max,res)
    elif func=='bwfull':
        model,isExt = make_bwfull(opts.min,opts.max,res)
    elif func=='voig':
        model,isExt = make_voig(opts.min,opts.max)
    elif func=='voigbg':
        model,isExt = make_voigbg(opts.min,opts.max)
    else:
        print 'Wrong --func (%s). Exiting...'%opts.func
        sys.exit(0)
    VMAP['rootdata']=opts.rootdata
    VMAP['rootmc']=opts.rootmc
    VMAP['pre']=opts.pre
    # loop over mc and data
    for dtype in ('mc_','data_'):
        dfit = mc if dtype=='mc_' else data
        if not dfit: continue
        # set some default event fractions
        # the other parameters are computed for mc and used as default in data fits
        if w.var('nsig') and w.var('nbg'):
            w.var('nsig').setVal(dfit.sumEntries())
            w.var('nbg').setVal(0)
        if w.var('C'):
            w.var('C').setVal(dfit.sumEntries())
        # Define fit range. Constrained to the core if using simple Gaussian
        fullbins = (opts.min,opts.max)
        fitbins = fullbins
        if func=='gaus':
            dGeV = opts.auto if opts.auto else 6.0  # 4.0
            peak = dfit.mean(w.var('x'))
            print 'OLD FIT RANGE:',opts.min,opts.max,'; MEAN:',peak
            fitbins = [peak - dGeV, peak + dGeV]
            print 'NEW FIT RANGE:',fitbins[0],fitbins[1]
        # perform the fit
        r,frame,chi2ndf,ndf,xtra,res1st = Fit(dfit,isExt,fullbins,fitbins,ncpus=opts.ncpus,gaus=(func=='gaus'))
        frame.SetTitle(opts.tag)
        # make the plots
        c = ROOT.TCanvas('c%szfit'%dtype,'c%szfit'%dtype); c.cd()
        frame.Draw()
        # print fitted value on canvas
        if w.var('m'):
            m = w.var('m')
            p = ROOT.TPaveText(.7,.75 , (.7+.2),(.75+.15) ,"NDC")
            p.SetTextAlign(11)
            p.SetFillColor(0)
            p.AddText('mz = %.3f +/- %.3f'%(m.getVal(),m.getError()))
            if func=='gaus':
                p.AddText('sigma = %.3f +/- %.3f'%(w.var('s').getVal(),w.var('s').getError()))
            if func=='egge':
                p.AddText('gamma = %.3f +/- %.3f'%(w.var('g').getVal(),w.var('g').getError()))
            if xtra:
                p.AddText(xtra)
            p.AddText('chi2/dof = %.1f. N=%d'%(chi2ndf,dfit.sumEntries()))
            p.Draw()
            gbg.append(p)
        gbg.append(frame)
        OMAP.append(c)
        if False:
            SaveAs(c,'%s_fit'%opts.tag,opts.ext)
        PrintVariables()
        mz=w.var('m').getVal()
        emz=w.var('m').getError()
        VMAP['%sN'%dtype]=dfit.numEntries()
        VMAP['%smz0'%dtype]=mz0
        VMAP['%smz'%dtype]=mz
        VMAP['%semz'%dtype]=emz
        VMAP['%schindf'%dtype]=chi2ndf
        VMAP['%sndf'%dtype]=ndf
        VMAP['%sfullbins'%dtype]=fullbins
        VMAP['%sfitbins'%dtype]=fitbins
        if res1st:
            VMAP['%s1stmz'%dtype]=res1st[0]
            VMAP['%s1stemz'%dtype]=res1st[1]
            VMAP['%s1stchindf'%dtype]=res1st[2]
            VMAP['%s1stndf'%dtype]=res1st[3]
        COUT.append( '%s mz0 = %.3f'%(dtype,mz0) )
        COUT.append( '%s mz = %.3f +/- %.3f'%(dtype,mz,emz) )
        COUT.append( '%s CHI2/NDF = %.2f, NDF = %d'%(dtype,chi2ndf,ndf) )
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
    VMAP['COUT']=[]
    for l in COUT:
        print l
        VMAP['COUT'].append(l)

if not opts.antondb:
    for i,obj in enumerate(OMAP):
        if obj.InheritsFrom('TPad'):
            obj.SaveAs('%s_%s_%s_%s__%d.png'%(opts.tag,opts.tt,opts.region,opts.func,i))

if opts.antondb and (len(VMAP)>0 or len(OMAP)>0):
    a = antondb(opts.antondb)
    path = os.path.join('/zpeak/',opts.tag,opts.tt,opts.region,opts.func)
    if len(VMAP)>0:
        a.add(path,VMAP)
    if len(OMAP)>0:
        a.add_root(path,OMAP)
