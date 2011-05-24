#!/usr/bin/env python
# fits a single z peak

"""
mc_zmumu_mc_zmumu.root/dg/dg/st_z_final/ntuple
data_data.root/dg/dg/st_z_final/ntuple
"""

try:
    import psyco
    psyco.full()
except ImportError:
    pass

import sys,re,array,math
from math import sqrt
import SimpleProgressBar
from load_data import *
from fit_defaults import *

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
parser.add_option("--mc",dest="mc",
                  type="string", default='mc_zmumu_mc_zmumu.root/dg/dg/st_z_final/ntuple',
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
def load_unbinned(hz,name,xmin,xmax,scale=1.0,nmaxT=-1):
    """ Load TGraph from a file. Note that we manually drop the points outside [xmin,xmax] range """
    if hz.ClassName() == 'TGraph':
        N,v1 = graph_to_array1(hz,opts.ndata)
    elif hz.ClassName() == 'TNtuple':
        N,v1 = ntuple_to_array1(hz,name,xmin,xmax,nmaxT if nmaxT>0 else opts.ndata)
    else:
        print 'Problem loading class',hz.ClassName()
        sys.exit(0)
    print 'Loaded raw unbinned data with',N,'entries'
    w.factory('x[%s,%s]'%(xmin,xmax))
    ds1 = RooDataSet('ds1','ds1',RooArgSet(w.var('x')))
    ds2 = RooDataSet('ds2','ds2',RooArgSet(w.var('x')))
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
        if nmaxT>0 and nf>=nmaxT:
            break
    print 'Final count of unbinned data:',nf
    #getattr(w,'import')(ds1,RF.Rename('ds1'))
    return ds1,nf

if True:
    # load the data
    f = TFile(opts.root,'r')
    hdata,hmc = f.Get(opts.data),f.Get(opts.mc)
    assert (hdata and hmc), 'Error loading data objects %s %s from file %s'%(opts.data,opts.mc,opts.root)
    assert hdata.ClassName() in ('TGraph','TNtuple','TTree','TChain')
    assert hmc.ClassName() in ('TGraph','TNtuple','TTree','TChain')
    data,ndata = load_unbinned(hdata,opts.region,opts.min,opts.max,scale=opts.scale)
    mc,nmc = load_unbinned(hmc,opts.region,opts.min,opts.max,scale=opts.scale,nmaxT=ndata)
    assert ndata == nmc, 'Error: ndata=%d not equal to nmc=%d'%(ndata,nmc)
    x = w.var('x')
    frame = x.frame(RF.Title('Dimuon invariant mass'))
    fullbins = (opts.min,opts.max)
    # save plot
    c = ROOT.TCanvas('c','c'); c.cd()
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
    SaveAs(c,'%s_kshape'%opts.tag,opts.ext)

# save to text file
if len(COUT)>0:
    fout = open('%s_kshape.rtxt'%opts.tag,'w')
    for l in COUT:
        print l
        print >>fout,l
    fout.close()
