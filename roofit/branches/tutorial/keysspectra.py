#!/usr/bin/env python
# plots curvature spectra for mu+ and mu- for two different files (e.g., release 16 vs release 17)

_QUALITY = ' && lP_idhits==1 && fabs(lP_z0)<10. && fabs(lP_d0sig)<10. && fabs(lP_pt_id-lP_pt_exms)/lP_pt_id<0.5'
_QUALITY += ' && lN_idhits==1 && fabs(lN_z0)<10. && fabs(lN_d0sig)<10. && fabs(lN_pt_id-lN_pt_exms)/lN_pt_id<0.5'
_PRE_PETER='fabs(lP_eta)<2.4 && lP_pt>20.0 && fabs(lN_eta)<2.4 && lN_pt>20.0 && lP_ptiso40<2.0 && lP_etiso40<2.0 && lN_ptiso40<2.0 && lN_etiso40<2.0 && Z_m>60 && (lP_q*lN_q)<0'+_QUALITY

#loosen isolation, tighten d0/z0:
if False:
    _QUALITY = ' && lP_idhits==1 && fabs(lP_z0)<10. && fabs(lP_d0sig)<10. && fabs(lP_pt_id-lP_pt_exms)/lP_pt_id<0.5'
    _QUALITY += ' && lN_idhits==1 && fabs(lN_z0)<10. && fabs(lN_d0sig)<10. && fabs(lN_pt_id-lN_pt_exms)/lN_pt_id<0.5'
    _PRE_PETER='fabs(lP_eta)<2.4 && lP_pt>20.0 && fabs(lN_eta)<2.4 && lN_pt>20.0 && lN_ptiso20/lN_pt<0.1 && Z_m>50 && (lP_q*lN_q)<0 && fabs(lP_z0-lN_z0)<3 && fabs(lP_d0-lN_d0)<2 && fabs(lP_phi-lN_phi)>2.0'+_QUALITY

try:
    import psyco
    psyco.full()
except ImportError:
    pass

# study muon momentum scale using z->mumu muon spectra
import sys,array,glob
import SimpleProgressBar
from load_data import *
from antondb import *

from optparse import OptionParser
parser = OptionParser()
# data sources
parser.add_option("--data",dest="data",
                  type="string", default='dg/st_z_final/ntuple',
                  help="TGraph containing data histograms")
parser.add_option("--root0",dest="root0",
                  type="string", default=None,
                  help="Input ROOT file (primary)")
parser.add_option("--root1",dest="root1",
                  type="string", default=None,
                  help="Input ROOT file (secondary) - for data/MC zmass mode")
parser.add_option("--label0",dest="label0",
                  type="string", default='2011, Rel16',
                  help="Label for first file")
parser.add_option("--label1",dest="label1",
                  type="string", default='2011, Rel17',
                  help="Label for second file")
parser.add_option("--pre",dest="pre",
                  type="string", default=_PRE_PETER,
                  help="Preliminary cuts to select final Z candidates")
parser.add_option("--tt",dest="tt",
                  type="string", default='cmb',
                  help="Type of muons: {cmb,id,exms}")
parser.add_option("--region",dest="region",
                  type="string", default='BB',
                  help="Where each leg of a Z must fall")
parser.add_option("--ndata",dest="ndata",
                  type="int", default=1000000,
                  help="Number of unbinned data points to load")
parser.add_option('-t',"--tag",dest="tag",
                  type="string", default='plot',
                  help="A tag to append to all output plots")
parser.add_option("--ext",dest="ext",
                  type="string", default='png',
                  help="Extension for all output")
parser.add_option("--antondb",dest="antondb",
                  type="string", default=None,
                  help="Tag for antondb output container")
# parameters
parser.add_option("--zmin",dest="zmin",
                  type="float", default=80.0,
                  help="Minimum value for z mass window")
parser.add_option("--zmax",dest="zmax",
                  type="float", default=100.0,
                  help="Maximum value for z mass window")
parser.add_option("--xmin",dest="xmin",
                  type="string", default='0.005',
                  help="Minimum value for 1/pt spectrum")
parser.add_option("--xmax",dest="xmax",
                  type="string", default='0.05',
                  help="Maximum value for 1/pt spectrum")
parser.add_option("--nbins",dest="nbins",
                  type="int", default=50,
                  help="Binning for chi2 calculation")
parser.add_option("--kluit", default=False,
                  action="store_true",dest="kluit",
                  help="If kluit is enabled, muons are not required to be both in the same detector region")
parser.add_option("--akluit", default=False,
                  action="store_true",dest="akluit",
                  help="If akluit is enabled, muons are required to NOT be both in the same detector region")
# enable modules
parser.add_option('-b', "--batch", default=False,
                  action="store_true",dest="batch",
                  help="Enable batch mode (all output directly to filesystem)")
parser.add_option("--scan", default=True,
                  action="store_true",dest="scan",
                  help="Scan through parameter space and determine best value and error")
parser.add_option("--ks", default=False,
                  action="store_true",dest="ks",
                  help="Perform unbinned KS comparison of two spectra")

(opts, args) = parser.parse_args()

import ROOT
ROOT.gROOT.SetBatch(opts.batch)
ROOT.gROOT.LoadMacro("AtlasStyle.C")
ROOT.SetAtlasStyle()
from ROOT import RooWorkspace,RooArgSet,RooArgList,RooDataHist,RooDataSet,RooAbsData,RooFormulaVar
from ROOT import RooHistPdf
from ROOT import kTRUE,kFALSE,kDashed,gStyle,gPad
from ROOT import TFile,TH1,TH1F,TH1D
from ROOT import RooFit as RF
w = RooWorkspace('w',kTRUE); w.model = None
gbg = []; COUT = []
# antondb containers
VMAP = {}; OMAP = []
VMAP['cmd']=' '.join(sys.argv)

# Load all datasets
if True:
    # variables and parameters
    w.factory('x[%s,%s]'%(opts.xmin,opts.xmax)); x = w.var('x')
    #################################################
    # data datasets
    #################################################
    hdata0 = ROOT.TChain(opts.data)
    for fname in glob.glob(opts.root0):
        print 'Adding to TChain:',fname
        nadd = hdata0.Add(fname)
        assert nadd>0,'Failed to add file %s'%fname
    assert hdata0.GetEntries()>0, 'Error loading data object %s from file %s'%(opts.root0,opts.data)
    hdata1 = ROOT.TChain(opts.data)
    for fname in glob.glob(opts.root1):
        print 'Adding to TChain:',fname
        nadd = hdata1.Add(fname)
        assert nadd>0,'Failed to add file %s'%fname
    assert hdata1.GetEntries()>0, 'Error loading data object %s from file %s'%(opts.root1,opts.data)
    nload = ntuple_to_array_etalim
    if opts.kluit:
        nload = ntuple_to_array_kluit
    elif opts.akluit:
        nload = ntuple_to_array_akluit
    Np0,Nn0,N0,pos0,neg0 = nload(hdata0,opts.tt,opts.region,opts.zmin,opts.zmax,opts.ndata,opts.pre)
    Np1,Nn1,N1,pos1,neg1 = nload(hdata1,opts.tt,opts.region,opts.zmin,opts.zmax,N0,opts.pre)
    pos = [pos0,pos1]
    neg = [neg0,neg1]
    N = [N0,N1]
    print N0,N1
    nmax = min(N0,N1,opts.ndata)
    if nmax == 0:
        print 'Error: no data passed the cuts'
        sys.exit(0)
    nstep = nmax if nmax>10 else 10
    print 'Loaded data object',opts.data,'with',nmax,'entries'
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
    c = ROOT.TCanvas('keysspectra','keysspectra',1024,768)
    c.Divide(2,2)
    frame = [x.frame(RF.Title('1/p_{T} (0)')),x.frame(RF.Title('1/p_{T} (1)'))]
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
        if i==0 and opts.label0:
            p.AddText(opts.label0)
        if i==1 and opts.label1:
            p.AddText(opts.label1)
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
    OMAP.append(c)

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

if not opts.antondb:
    fname = 'CURV_%s_%s_%s.png'%(opts.tag,opts.tt,opts.region)
    c.SaveAs(fname)

# save to text file
if len(COUT)>0:
    VMAP['COUT']=[]
    for l in COUT:
        print l
        VMAP['COUT'].append(l)

if (len(VMAP)>0 or len(OMAP)>0) and opts.antondb:
    a = antondb(opts.antondb)
    path = os.path.join('/keysspectra/',opts.tag,opts.tt,opts.region)
    print VMAP
    if len(VMAP)>0:
        a.add(path,VMAP)
    if len(OMAP)>0:
        a.add_root(path,OMAP)
