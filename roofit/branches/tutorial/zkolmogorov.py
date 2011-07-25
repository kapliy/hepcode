#!/usr/bin/env python
# fits a single z peak and plots cdf

_QUALITY = ' && lP_idhits==1 && fabs(lP_z0)<10. && fabs(lP_d0sig)<10. && fabs(lP_pt_id-lP_pt_exms)/lP_pt_id<0.5'
_QUALITY += ' && lN_idhits==1 && fabs(lN_z0)<10. && fabs(lN_d0sig)<10. && fabs(lN_pt_id-lN_pt_exms)/lN_pt_id<0.5'
_PRE_PETER='lP_pt>20.0 && lN_pt>20.0 && lP_ptiso40<2.0 && lP_etiso40<2.0 && lN_ptiso40<2.0 && lN_etiso40<2.0 && Z_m>50 && (lP_q*lN_q)<0'+_QUALITY

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
                  help="TGraph containing data histograms")
parser.add_option("--region",dest="region",
                  type="string", default='BB',
                  help="Where each leg of a Z must fall")
parser.add_option("--ndata",dest="ndata",
                  type="int", default=1000000,
                  help="Number of unbinned data points to load")
parser.add_option("--nskip",dest="nskip",
                  type="int", default=0,
                  help="Number of data points to skip load")
parser.add_option("--antondb",dest="antondb",
                  type="string", default='output',
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
from ROOT import RooGaussModel,RooAddModel,RooRealVar,RooAbsReal,RooRealSumPdf,RooHistPdf
from ROOT import kTRUE,kFALSE,kDashed,gStyle,gPad
from ROOT import TFile,TH1,TH1F,TH1D
from ROOT import RooFit as RF
w = RooWorkspace('w',kTRUE)
gbg = []; COUT = []
# antondb containers
VMAP = {}; OMAP = []

# Perform unbinned Ks comparison
def make_array_sorted(data,var='x'):
    """ Converts RooDataSet to python array"""
    out = []
    NE = data.numEntries()
    for ii in range(NE):
        aset=data.get(ii)
        out.append( aset.getRealValue(var) )
    return array.array('d',sorted(out))
def p_value(dP,dN):
    """ KS p-value for two RooDataSets """
    arrayP = make_array_sorted(dP)
    arrayN = make_array_sorted(dN)
    assert len(arrayP)==len(arrayN),'ERROR: different array sizes: %d %d'%(len(arrayP),len(arrayN))
    pval = ROOT.TMath.KolmogorovTest(len(arrayP),arrayP,len(arrayN),arrayN,'')
    return pval

# Load unbinned data
def load_unbinned(hz,tt,pre,reg,xmin,xmax,nmaxT,scale=1.0):
    """ Load TGraph from a file. Note that we manually drop the points outside [xmin,xmax] range """
    if hz.ClassName() in ('TNtuple','TTree','TChain'):
        N,v1 = ntuple_to_array1(hz,tt,reg,xmin,xmax,nmaxT,pre=pre)
    else:
        print 'Problem loading class',hz.ClassName()
        sys.exit(0)
    print 'Loaded raw unbinned data with',N,'entries'
    w.factory('x[%s,%s]'%(xmin,xmax))
    ds1 = RooDataSet('ds1','ds1',RooArgSet(w.var('x')))
    nmax = min(N,nmaxT); nmaxp10 = nmax if nmax>10 else 10
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
        if nmaxT>0 and nf>=nmaxT:
            break
    print 'Final count of unbinned data:',nf
    return ds1,nf

if True:
    print 'Ntuple path:',opts.data
    # load data
    hdata = ROOT.TChain(opts.data)
    for fname in glob.glob(opts.rootdata):
        print 'Adding to TChain:',fname
        nadd = hdata.Add(fname)
        assert nadd>0,'Failed to add file %s'%fname
    print 'Loaded data trees with %d entries'%hdata.GetEntries()
    assert hdata.GetEntries()>0, 'Error loading data object %s from file %s'%(opts.data,opts.rootdata)
    # load MC
    hmc = ROOT.TChain(opts.data)
    for fname in glob.glob(opts.rootmc):
        print 'Adding to TChain:',fname
        nadd = hmc.Add(fname)
        assert nadd>0,'Failed to add file %s'%fname
    print 'Loaded data trees with %d entries'%hmc.GetEntries()
    assert hmc.GetEntries()>0, 'Error loading data object %s from file %s'%(opts.data,opts.rootmc)
    # read ntuples
    assert hdata.ClassName() in ('TGraph','TNtuple','TTree','TChain')
    assert hmc.ClassName() in ('TGraph','TNtuple','TTree','TChain')
    data,ndata = load_unbinned(hdata,opts.tt,opts.pre,opts.region,opts.min,opts.max,opts.ndata,scale=opts.scale)
    mc,nmc = load_unbinned(hmc,opts.tt,opts.pre,opts.region,opts.min,opts.max,ndata,scale=opts.scale)
    if nmc < ndata:
        print 'WARNING: nmc = %d is less than ndata = %d. Reducing ndata to nmc'
        data,ndata = data.reduce(RF.EventRange(0,nmc)),nmc
    assert ndata == nmc, 'Error: ndata=%d not equal to nmc=%d'%(ndata,nmc)
    x = w.var('x')
    # save plot
    c = ROOT.TCanvas('zkolmogorov','zkolmogorov',480,768)
    c.Divide(1,2)
    # plot histos
    c.cd(1)
    frame = x.frame(RF.Title('Dimuon invariant mass'))
    fullbins = (opts.min,opts.max)
    nbins = int((fullbins[1]-fullbins[0])/0.5)
    RooAbsData.plotOn(data,frame,RF.Name('dataZ'),RF.Binning(int((fullbins[1]-fullbins[0])/0.5)) , RF.MarkerColor(ROOT.kRed) )
    RooAbsData.plotOn(mc,frame,RF.Name('mcZ'),RF.Binning(int((fullbins[1]-fullbins[0])/0.5)) , RF.MarkerColor(ROOT.kBlue) )
    frame.Draw()
    p = ROOT.TPaveText(.7,.75 , (.7+.25),(.75+.15) ,"NDC")
    p.SetTextAlign(11)
    p.SetFillColor(0)
    pval = p_value(data,mc)
    p.AddText('# of events = %d'%(ndata))
    p.AddText('KS probability = %.2f%%'%(pval*100.0))
    p.Draw()
    # antondb values
    VMAP['ndata']=ndata
    VMAP['pval']=pval
    VMAP['rootdata']=opts.rootdata
    VMAP['rootmc']=opts.rootmc
    VMAP['pre']=opts.pre
    # plot cdf
    c.cd(2)
    frame = x.frame(RF.Title('cdf'))
    x.setBins(nbins)
    data_b = data.binnedClone()
    mc_b = mc.binnedClone()
    hdata = RooHistPdf('histdata','histdata',RooArgSet(x),data_b,2)
    hmc = RooHistPdf('histmc','histmc',RooArgSet(x),mc_b,2)
    cdfdata = hdata.createCdf(RooArgSet(x))
    cdfmc = hmc.createCdf(RooArgSet(x))
    cdfdata.plotOn(frame,RF.LineColor(ROOT.kRed))
    cdfmc.plotOn(frame,RF.LineColor(ROOT.kBlue))
    frame.Draw()
    #OMAP.append(hdata)
    #OMAP.append(hmc)
    OMAP.append(c)
    if False:
        SaveAs(c,'%s_kshape'%opts.tag,opts.ext)

# save to text file
if len(COUT)>0:
    VMAP['COUT']=[]
    for l in COUT:
        print l
        VMAP['COUT'].append(l)

if len(VMAP)>0 or len(OMAP)>0:
    a = antondb(opts.antondb)
    path = os.path.join('/zkolmogorov/',opts.tag,opts.tt,opts.region)
    print VMAP
    if len(VMAP)>0:
        a.add(path,VMAP)
    if len(OMAP)>0:
        a.add_root(path,OMAP)
