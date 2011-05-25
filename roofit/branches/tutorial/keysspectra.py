#!/usr/bin/env python

try:
    import psyco
    psyco.full()
except ImportError:
    pass

# study muon momentum scale using z->mumu muon spectra
import sys,array
import SimpleProgressBar
from load_data import *

def func_SCALE_old(xx,par):
    return par*1.0/xx
def func_SCALE(xx,par):
    return 1.0/(par*1.0*xx)
def func_SHIFT(xx,par):
    return 1.0/(xx) + par

from optparse import OptionParser
parser = OptionParser()
# data sources
parser.add_option("--root",dest="root",
                  type="string", default='root_all.root',
                  help="Input ROOT file with all histograms")
parser.add_option("--data0",dest="data0",
                  type="string", default='data_data.root/dg/dg/st_z_final/ntuple',
                  help="Default data pt spectra")
parser.add_option("--data1",dest="data1",
                  type="string", default='data_00_data_00.root/dg/dg/st_z_final/ntuple',
                  help="Scaled data pt spectra")
parser.add_option("--region",dest="region",
                  type="string", default='BB',
                  help="Where each leg of a Z must fall")
parser.add_option("--ndata",dest="ndata",
                  type="int", default=1000000,
                  help="Number of unbinned data points to load")
parser.add_option("--nskip",dest="nskip",
                  type="int", default=0,
                  help="Number of data points to skip load")
parser.add_option("--forcescale",dest="forcescale",
                  type="float", default=1.0,
                  help="Force a particular scale factor on negative muons")
parser.add_option("--roomodel",dest="roomodel",
                  type="int", default=2,
                  help="Which model to use: 1=RooHist, 2=RooKeys, 3=RooNDKeys")
parser.add_option("--rookeys",dest="rookeys",
                  type="string", default=None,
                  help="ROOT file from which we load a cached RooKeysPdf")
parser.add_option("--rookeysout",dest="rookeysout",
                  type="string", default='',
                  help="ROOT file into which we will save a cached RooKeysPdf")
parser.add_option('-t',"--tag",dest="tag",
                  type="string", default='plot',
                  help="A tag to append to all output plots")
parser.add_option("--ext",dest="ext",
                  type="string", default='png',
                  help="Extension for all output")
# parameters
parser.add_option("--zmin",dest="zmin",
                  type="float", default=66.0,
                  help="Minimum value for z mass window")
parser.add_option("--zmax",dest="zmax",
                  type="float", default=116.0,
                  help="Maximum value for z mass window")
parser.add_option("--xmin",dest="xmin",
                  type="string", default='0.005',
                  help="Minimum value for 1/pt spectrum")
parser.add_option("--xmax",dest="xmax",
                  type="string", default='0.05',
                  help="Maximum value for 1/pt spectrum")
parser.add_option("--fitmin",dest="fitmin",
                  type="float", default=0.98,
                  help="Minimum bound of fit range")
parser.add_option("--fitmax",dest="fitmax",
                  type="float", default=1.02,
                  help="Maximum bound of fit range")
parser.add_option("--shift", default=False,
                  action="store_true",dest="shift",
                  help="Fit for shift, rather than scale")
parser.add_option("--smooth",dest="smooth",
                  type="float", default=2.0,
                  help="Smoothing factor (aka bandwidth) for kernel estimation")
parser.add_option("--nbins",dest="nbins",
                  type="int", default=50,
                  help="Binning for chi2 calculation")
parser.add_option("--nscan",dest="nscan",
                  type="int", default=20,
                  help="Number of parameter values to scan")
parser.add_option("--kluit", default=False,
                  action="store_true",dest="kluit",
                  help="If kluit is enabled, muons are not required to be both in the same detector region")
parser.add_option("--akluit", default=False,
                  action="store_true",dest="akluit",
                  help="If akluit is enabled, muons are required to NOT be both in the same detector region")
parser.add_option("--debug", default=False,
                  action="store_true",dest="debug",
                  help="General-use flag to assist with debugging")
# enable modules
parser.add_option('-b', "--batch", default=False,
                  action="store_true",dest="batch",
                  help="Enable batch mode (all output directly to filesystem)")
parser.add_option("--template", default=False,
                  action="store_true",dest="template",
                  help="Make a plot of the template")
parser.add_option("--scan", default=False,
                  action="store_true",dest="scan",
                  help="Scan through parameter space and determine best value and error")
parser.add_option("--ks", default=False,
                  action="store_true",dest="ks",
                  help="Perform unbinned KS comparison of two spectra")
parser.add_option("--npergroup", dest="npergroup",
                  type="int", default=0,
                  help="Split the sample into several subsamples with npergroup in each")
parser.add_option("--varbins", default=False,
                  action="store_true",dest="varbins",
                  help="Study systematic effect of bin variation")
parser.add_option("--savegrid", default=False,
                  action="store_true",dest="savegrid",
                  help="Plot the grid of 9 scanned parameter values")

(opts, args) = parser.parse_args()

import ROOT
ROOT.gROOT.SetBatch(opts.batch)
ROOT.gROOT.LoadMacro("AtlasStyle.C")
ROOT.SetAtlasStyle()
from ROOT import RooWorkspace,RooArgSet,RooArgList,RooDataHist,RooDataSet,RooAbsData,RooFormulaVar
from ROOT import RooHistPdf,RooKeysPdf,RooNDKeysPdf
from ROOT import kTRUE,kFALSE,kDashed,gStyle,gPad
from ROOT import TFile,TH1,TH1F,TH1D
from ROOT import RooFit as RF
w = RooWorkspace('w',kTRUE); w.model = None
gbg = []; COUT = []

# Z mass cannot float if we fit for scale
mZ = '91.1876'
func = func_SCALE
FITMIN=opts.fitmin  #parameter fitting minimum
FITMAX=opts.fitmax

# Load all datasets
if True:
    # variables and parameters
    w.factory('x[%s,%s]'%(opts.xmin,opts.xmax)); x = w.var('x')
    w.factory('b[1.0,0.9,1.1]')
    #################################################
    # data datasets
    #################################################
    f = TFile(opts.root,'r')
    hdata0 = f.Get(opts.data0)
    hdata1 = f.Get(opts.data1)
    assert hdata0 and hdata1
    assert hdata0.ClassName() == 'TNtuple' and hdata1.ClassName() == 'TNtuple'
    nload = ntuple_to_array_etalim
    if opts.kluit:
        nload = ntuple_to_array_kluit
    elif opts.akluit:
        nload = ntuple_to_array_akluit
    Np0,Nn0,N0,pos0,neg0 = nload(hdata0,opts.region,opts.zmin,opts.zmax,opts.ndata,opts.nskip)
    Np1,Nn1,N1,pos1,neg1 = nload(hdata1,opts.region,opts.zmin,opts.zmax,N0,opts.nskip)
    pos = [pos0,pos1]
    neg = [neg0,neg1]
    N = [N0,N1]
    f.Close()
    print N0,N1
    nmax = min(N0,N1,opts.ndata)
    if nmax == 0:
        print 'Error: no data passed the cuts'
        sys.exit(0)
    nstep = nmax if nmax>10 else 10
    print 'Loaded data object',opts.data0,'with',nmax,'entries'
    # make data(x) for positive and negative muons
    dataP = [RooDataSet('data0P','Zmumu mu+ data0',RooArgSet(x)), RooDataSet('data0P','Zmumu mu+ data1',RooArgSet(x))]
    dataN = [RooDataSet('data0N','Zmumu mu- data0',RooArgSet(x)), RooDataSet('data0N','Zmumu mu- data1',RooArgSet(x))]
    print 'Loading default data...'
    for i in range(0,nmax):
        for zz in (0,1):
            x.setVal(1.0/pos[zz][i])
            dataP[zz].add(RooArgSet(x))
            x.setVal(1.0/neg[zz][i])
            dataN[zz].add(RooArgSet(x))
# make a joint pdf for various signal regions
w.factory("")
w.defineSet('X','x')
x = w.var('x')

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

if opts.scan:
    c = ROOT.TCanvas(rand_name(),rand_name(),1024,768)
    c.Divide(2,2)
    frame = [x.frame(RF.Title('Data 1/p_{T} (uncorrected)')),x.frame(RF.Title('Data 1/p_{T} (scaled)'))]
    # plot raw data
    for i in (0,1):
        c.cd(i+1)
        color=ROOT.kRed
        RooAbsData.plotOn(dataP[i],frame[i],RF.LineColor(color),RF.MarkerColor(color),RF.Binning(opts.nbins),RF.MarkerSize(1.0)) #,RF.DrawOption('C'))
        color=ROOT.kBlue
        RooAbsData.plotOn(dataN[i],frame[i],RF.LineColor(color),RF.MarkerColor(color),RF.Binning(opts.nbins),RF.MarkerSize(0.75))
        frame[i].Draw()
        p = ROOT.TPaveText(.7,.78 , (.7+.23),(.78+.12) ,"NDC")
        p.SetTextAlign(11)
        p.SetFillColor(0)
        pval = p_value(dataP[i],dataN[i])
        p.AddText('# of events = %d'%(nmax))
        p.AddText('KS probability = %.2f%%'%(pval*100.0))
        p.Draw()
        gbg.append((p,frame[i]))
    # plot the CDFs
    x.setBins(opts.nbins)
    dataP_b = [dataP[i].binnedClone() for i in (0,1)]
    dataN_b = [dataN[i].binnedClone() for i in (0,1)]
    histP = [RooHistPdf('histP%d'%i,'histP%d'%i,RooArgSet(x),dataP_b[i],2) for i in (0,1)]
    histN = [RooHistPdf('histN%d'%i,'histN%d'%i,RooArgSet(x),dataN_b[i],2) for i in (0,1)]
    for i in (0,1):
        c.cd(i+3)
        frame = x.frame(RF.Title('cdf (%s)'%('uncorrected' if i==0 else 'scaled')))
        cdfP = histP[i].createCdf(RooArgSet(x))
        cdfN = histN[i].createCdf(RooArgSet(x))
        cdfP.plotOn(frame,RF.LineColor(ROOT.kRed))
        cdfN.plotOn(frame,RF.LineColor(ROOT.kBlue)) #,RF.LineStyle(ROOT.kDashed))
        frame.Draw()
    # save
    SaveAs(c,'%s_ptspectra'%opts.tag,opts.ext)

# study statistical properties of KS statistic
if opts.ks:
    x.setBins(opts.nbins)
    data = dataP[0].binnedClone()
    pdf = RooHistPdf('hist','hist',RooArgSet(x),data,2)
    h = ROOT.TH1F('h','h',50,0.0,1.0)
    for i in range(2000):
        if i%10 == 0:
            print i
        ds1 = pdf.generate(RooArgSet(x),N0)
        ds2 = pdf.generate(RooArgSet(x),N0)
        pval = p_value(ds1,ds2)
        h.Fill( pval )
    c = ROOT.TCanvas(rand_name(),rand_name(),640,480)
    h.Draw()
    h.GetYaxis().SetRangeUser(0,100)
    SaveAs(c,'%s_ksstudy'%opts.tag,opts.ext)

# save to text file
if len(COUT)>0:
    fout = open('%s_ptspectra.rtxt'%opts.tag,'w')
    for l in COUT:
        print l
        print >>fout,l
    fout.close()
