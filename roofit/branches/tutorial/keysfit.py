#!/usr/bin/env python
# determines R0 (or shift C) and k+*k-

_QUALITY = ' && lP_idhits==1 && fabs(lP_z0)<10. && fabs(lP_d0sig)<10. && fabs(lP_pt_id-lP_pt_exms)/lP_pt_id<0.5'
_QUALITY += ' && lN_idhits==1 && fabs(lN_z0)<10. && fabs(lN_d0sig)<10. && fabs(lN_pt_id-lN_pt_exms)/lN_pt_id<0.5 && nmuons==2'
_PRE_PETER='fabs(lP_eta)<2.4 && lP_pt>20.0 && fabs(lN_eta)<2.4 && lN_pt>20.0 &&  lP_ptiso40/lP_pt<0.1 && lN_ptiso40/lN_pt<0.1  &&  Z_m>65 && (lP_q*lN_q)<0'+_QUALITY

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

# study muon momentum scale using z->mumu muon spectra
import sys,array,glob
import SimpleProgressBar
from load_data import *
from antondb import *

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
                  type="string", default='ROOT/root_all_0626_newiso_noscale_1fb_cmb/data_period*/root_data_period*.root',
                  help="Input ROOT file (primary)")
parser.add_option("--root2",dest="root2",
                  type="string", default='', #default='ROOT/root_all_0621_newiso_cmb/mc_zmumu/root_*.root',
                  help="Input ROOT file (secondary) - for data/MC zmass mode")
parser.add_option("--tt",dest="tt",
                  type="string", default='cmb',
                  help="Type of muons: {cmb,id,exms}")
parser.add_option("--pre",dest="pre",
                  type="string", default=_PRE_PETER,
                  help="Preliminary cuts to select final W candidates")
parser.add_option("--data",dest="data",
                  type="string", default='dg/nominal/st_z_final/ntuple',
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
parser.add_option("--antondb",dest="antondb",
                  type="string", default='output',
                  help="Tag for antondb output container")
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
                  type="int", default=10,
                  help="Binning for chi2 calculation")
parser.add_option("--nscan",dest="nscan",
                  type="int", default=200,
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
dmc = opts.root2

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
gbg = []; COUT = [];
# antondb containers
VMAP = {}; OMAP = []
VMAP['cmd']=' '.join(sys.argv)

# Z mass cannot float if we fit for scale
mZ = '91.1876'
if not opts.shift:
    func = func_SCALE
    scale = [(opts.fitmin + (opts.fitmax-opts.fitmin)*zz/opts.nscan) for zz in xrange(opts.nscan)]
    FITMIN=opts.fitmin  #parameter fitting minimum
    FITMAX=opts.fitmax
else:
    func = func_SHIFT
    scale = [(-0.002 + 0.004*zz/opts.nscan) for zz in xrange(opts.nscan)]
    FITMIN = -0.001
    FITMAX =  0.001
print ['%.2f'%(z*100) for z in scale[:3]]
print ['%.2f'%(z*100) for z in scale[-3:]]

# Load all datasets
if True:
    # variables and parameters
    w.factory('x[%s,%s]'%(opts.xmin,opts.xmax)); x = w.var('x')
    w.factory('b[1.0,0.9,1.1]')
    #################################################
    # data datasets
    #################################################
    print 'Ntuple path:',opts.data
    if dmc:  #data-mc mode: comparing mZ spectrum in data and MC
        hdata = ROOT.TChain(opts.data)
        for fname in glob.glob(opts.root):
            print 'Adding to TChain:',fname
            nadd = hdata.Add(fname)
            assert nadd>0,'Failed to add file %s'%fname
        print 'Loaded data trees with %d entries'%hdata.GetEntries()
        assert hdata.GetEntries()>0, 'Error loading data object %s from file %s'%(opts.data,opts.root)
        hmc = ROOT.TChain(opts.data)
        for fname in glob.glob(opts.root2):
            hmc.Add(fname)
        print 'Loaded MC trees with %d entries'%hmc.GetEntries()
        assert hmc.GetEntries()>0, 'Error loading data object %s from file %s'%(opts.data,opts.root2)
        Np,pos = ntuple_to_array1(hmc,opts.tt,opts.region,opts.zmin,opts.zmax,opts.ndata,pre=opts.pre)
        Nn,neg = ntuple_to_array1(hdata,opts.tt,opts.region,opts.zmin,opts.zmax,opts.ndata,pre=opts.pre)
        N=min(Np,Nn)
        assert N>0,'Something failed: NPOS=%d NNEG=%d'%(Np,Nn)
    else:    #regular mode: comparing 1/pt for mu+ and mu-
        hz = ROOT.TChain(opts.data)
        for fname in glob.glob(opts.root):
            print 'Adding to TChain:',fname
            nadd = hz.Add(fname)
            assert nadd>0,'Failed to add file %s'%fname
        print 'Loaded trees with %d entries'%hz.GetEntries()
        assert hz.GetEntries()>0, 'Error loading data object %s from file %s'%(opts.data,opts.root)
        if opts.kluit:
            Np,Nn,N,pos,neg = ntuple_to_array_kluit(hz,opts.tt,opts.region,opts.zmin,opts.zmax,opts.ndata,opts.nskip,pre=opts.pre)
        elif opts.akluit:
            Np,Nn,N,pos,neg = ntuple_to_array_akluit(hz,opts.tt,opts.region,opts.zmin,opts.zmax,opts.ndata,opts.nskip,pre=opts.pre)
        else:
            Np,Nn,N,pos,neg = ntuple_to_array_etalim(hz,opts.tt,opts.region,opts.zmin,opts.zmax,opts.ndata,opts.nskip,pre=opts.pre)
    avg_pos = sum(pos)/len(pos)
    avg_neg = sum(neg)/len(neg)
    if opts.shift:
        avg_pos = sum([1/z for z in pos])/len(pos)
        avg_neg = sum([1/z for z in neg])/len(neg)
    avg_scale = avg_pos/avg_neg
    avg_shift = avg_pos - avg_neg
    nmax = min(N,opts.ndata)
    VMAP['avg']=avg_scale
    VMAP['nmax']=nmax
    VMAP['np']=Np
    VMAP['nn']=Nn
    VMAP['root']=opts.root
    VMAP['root2']=opts.root2
    VMAP['pre']=opts.pre
    if nmax == 0:
        print 'Error: no data passed the cuts'
        sys.exit(0)
    nstep = nmax if nmax>10 else 10
    ngroups = int(nmax/opts.npergroup) if opts.npergroup>0 else 0
    print 'Loaded data object',opts.data,'with',N,'entries','in',ngroups,'groups'
    # make data(x) for positive and negative muons
    dataP = RooDataSet('dataP','Zmumu mu+ data',RooArgSet(x))
    dataN = RooDataSet('dataN','Zmumu mu- data',RooArgSet(x))
    datasP = [RooDataSet('datasP%s'%i,'Zmumu mu+ data bin %s'%i,RooArgSet(x)) for i in range(opts.nscan)]
    datasN = [RooDataSet('datasN%s'%i,'Zmumu mu- data bin %s'%i,RooArgSet(x)) for i in range(opts.nscan)]
    # same as dataP, but limited to 10k events max (otherwise keys generation is too slow)
    data_model = RooDataSet('data_model','Zmumu mu+ data',RooArgSet(x))
    ndata_model_max = 10000
    # reserve arrays of ngroups scans
    datasNsp = []
    datasPsp = []
    for j in range(ngroups):
        datasNsp.append([])
        datasPsp.append(RooDataSet('datasPsp%s'%(j),'Zmumu mu+ data bin %s'%(j),RooArgSet(x)))
        for i in range(opts.nscan):
            datasNsp[j].append(RooDataSet('datasNsp%s_%s'%(i,j),'Zmumu mu- data bin %s %s'%(i,j),RooArgSet(x)))
    # default dataP and dataN
    bar = SimpleProgressBar.SimpleProgressBar(10,nmax)
    print 'Loading default data...'
    for i in range(0,nmax):
        x.setVal(1.0/pos[i])
        dataP.add(RooArgSet(x))
        if i<ndata_model_max:
            data_model.add(RooArgSet(x))
        x.setVal(1.0/neg[i])
        dataN.add(RooArgSet(x))
    # scaled / shifted versions for manual scans
    if ngroups==0:
        print 'Loading scaled data arrays...'
        for i in range(0,nmax):
            if (i)%(int(nstep/10))==0 and i>0:
                print bar.show(i)
            for z in range(opts.nscan):
                x.setVal(func(neg[i]*opts.forcescale,scale[z]))
                datasN[z].add(RooArgSet(x))
    # scaled / shifted versions in multiple splits
    else:
        print 'Loading scaled data arrays in %s chunks...'%ngroups
        for j in range(ngroups):
            print 'Loading range %s/%s'%(j,ngroups)
            for i in range(opts.npergroup*j,opts.npergroup*(j+1)):
                if (i)%(int(nstep/10))==0 and i>0:
                    print bar.show(i-opts.npergroup*j)
                x.setVal(1.0/pos[i])
                datasPsp[j].add(RooArgSet(x))
                for z in range(opts.nscan):
                    x.setVal(func(neg[i],scale[z]))
                    datasNsp[j][z].add(RooArgSet(x))
    getattr(w,'import')(dataP,RF.Rename('dataP'))
    getattr(w,'import')(dataN,RF.Rename('dataN'))
    # make truth(x*b)
    w.factory("expr::xf('x*1.0/b',x,b)")
    xf = w.function('xf')
    models = []
    if opts.rookeys and opts.npergroup==0:
        fkeys = ROOT.TFile.Open(opts.rookeys)
        assert fkeys.IsOpen(),'Failed to open RooKeysPdf cache file: %s'%opts.rookeys
        model = fkeys.wsave.pdf('model')
        getattr(w,'import')(model)
        w.model = model
        fkeys.Close()
    else:
        if opts.npergroup==0: #otherwise too time consuming!
            print 'Making model PDF. Please wait...'
            # default (all data in one group)
            if opts.roomodel==1:
                model = RooHistPdf('model','model',RooArgList(xf),RooArgList(w.var('x')),data_model,opts.smooth)
            elif opts.roomodel==2:
                model = RooKeysPdf('model','model from truth',x,data_model,RooKeysPdf.MirrorBoth,opts.smooth)
            elif opts.roomodel==3:
                model = RooNDKeysPdf('model','model',RooArgList(x),data_model,'am',opts.smooth)
        else:
            model = None
        # multiple groups
        for j in range(ngroups):
            print 'Making model PDF for group',j,'/',ngroups
            if opts.roomodel==1:
                models.append(RooHistPdf('model','model',RooArgList(xf),RooArgList(w.var('x')),datasPsp[j],opts.smooth))
            elif opts.roomodel==2:
                models.append(RooKeysPdf('model','model from truth',x,datasPsp[j],RooKeysPdf.MirrorBoth,opts.smooth))
            elif opts.roomodel==3:
                models.append(RooNDKeysPdf('model','model',RooArgList(x),datasPsp[j],'am',opts.smooth))           
        w.model = model
        cname = model.ClassName() if model else None
        print 'Made model PDF of type:',cname
        if not cname=='RooNDKeysPdf' and opts.rookeysout:
            wsave = RooWorkspace('wsave',kTRUE)
            getattr(wsave,'import')(model)
            wsave.writeToFile(opts.rookeysout)
            getattr(w,'import')(model)
        #model = RooNDKeysPdf('model','model',RooArgList(x),data_model,'am')
        #model = RooNDKeysPdf('model','model',RooArgList(xf),data_model,'am')
        #model = RooHistPdf('model','model',RooArgList(xf),RooArgList(w.var('x')),truth,4)
    isExt = kFALSE

# make a joint pdf for various signal regions
w.factory("")
w.defineSet('X','x')
model = w.model
x = w.var('x')
chi2 = []

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

def plot_data(model,data,color=ROOT.kBlack,nbins=10):
    """ Plot data and model for a particular parameter value, and return chi2 """
    frame = x.frame(RF.Title('1/p_{T}'))
    frame_ret = x.frame(RF.Title('1/p_{T}'))
    # chi2 frame
    derr = RF.DataError(RooAbsData.SumW2)
    RooAbsData.plotOn(data,frame,RF.LineColor(color),RF.MarkerColor(color),RF.Binning(nbins))
    model.plotOn(frame)
    chi = frame.chiSquare(1)*(nbins-1.0)
    # plotting frame (errors removed)
    RooAbsData.plotOn(data,frame_ret,RF.LineColor(color),RF.MarkerColor(color),RF.Binning(nbins),RF.XErrorSize(0))
    model.plotOn(frame_ret)
    #model.paramOn(frame,data)
    return frame_ret,chi
def runscan(model,dataP,data,ks,nbins=opts.nbins):
    """ Scan through parameter space and compute chi2 for each parameter value """
    res = []
    frames = []
    for z in xrange(opts.nscan):
        if ks:
            frame,chi = None,p_value(dataP,data[z])
        else:
            frame,chi = plot_data(model,data[z],nbins=nbins);
        res.append(chi)
        frames.append(frame)
    return res,frames
def make_graph(n,xs,ys):
    """ Make a TGraph for scanned_parameters -vs- chi2"""
    h = ROOT.TGraph(n)
    #h.SetName(rand_name())
    h.SetMarkerColor(4);
    h.SetMarkerSize(1.5);
    h.SetMarkerStyle(21);
    for z in xrange(n):
        h.SetPoint(z,xs[z],ys[z])
    return h
def fitgraph(h,ks,FITMIN,FITMAX):
    """ Fit chi2 TGraph to a parabola and extract chi2 minimum and error band around (chi2-1) """
    if ks:
        # fit gaussian core as an approximation for KS probability distribution
        rms,mean=h.GetRMS(),h.GetMean()
        rms,pc=h.GetRMS(),max([(h.GetY()[i],h.GetX()[i]) for i in xrange(h.GetN())])
        peak,chimean = pc[1],pc[0]
        xmin = peak
        print 'peak and rms:',peak,rms
        rms_scale=0.33
        FITMIN=peak-rms*rms_scale
        FITMAX=peak+rms*rms_scale
        print 'Fitting range:',FITMIN,FITMAX
        fr = h.Fit('gaus','S','',FITMIN,FITMAX)
        f = h.GetFunction('gaus')
        chimin = f.GetMaximum(FITMIN,FITMAX)
        fitted_xmin = f.GetParameter(1)
        err = f.GetParameter(2)
        xleft,xright = xmin-err,xmin+err
        lbl_xmin = 0.50
    else:
        # fit parabola
        pc=min([(h.GetY()[i],h.GetX()[i]) for i in xrange(h.GetN())])
        peak,chimean = pc[1],pc[0]
        xmin = peak
        print 'RMS fit x,y',xmin,chimean
        # first, rough fit (to get an estimate of sigma)
        print 'Original fit range:',FITMIN,FITMAX
        fr = h.Fit('pol2','S0','',FITMIN,FITMAX)
        f = h.GetFunction('pol2').Clone('stage1')
        f.SetLineColor(30)
        f.SetLineStyle(2)
        f.Draw('SAME')
        gbg.append(f)
        chimin = f.GetMinimum(FITMIN,FITMAX)
        fitted_xmin = f.GetMinimumX(FITMIN,FITMAX)
        chirange=70 #50
        xleft = f.GetX(chimin+chirange,FITMIN,fitted_xmin)
        xright = f.GetX(chimin+chirange,fitted_xmin,FITMAX)
        err = 0.5*((fitted_xmin-xleft)+(xright-fitted_xmin))
        del fr; #del f;
        # second, refined fit
        FITMIN=fitted_xmin-err
        FITMAX=fitted_xmin+err
        print 'Refined fit range:',FITMIN,FITMAX
        fr = h.Fit('pol2','S0','',FITMIN,FITMAX)
        f = h.GetFunction('pol2').Clone('stage2')
        f.SetLineColor(3)
        f.SetLineStyle(2)
        f.Draw('SAME')
        gbg.append(f)
        chimin = f.GetMinimum(FITMIN,FITMAX)
        fitted_xmin = f.GetMinimumX(FITMIN,FITMAX)
        chirange=20 #20
        xleft = f.GetX(chimin+chirange,FITMIN,fitted_xmin)
        xright = f.GetX(chimin+chirange,fitted_xmin,FITMAX)
        err = 0.5*((fitted_xmin-xleft)+(xright-fitted_xmin))
        del fr; #del f;
        # third, final fit
        FITMIN=fitted_xmin-err
        FITMAX=fitted_xmin+err
        print 'Re-refined fit range:',FITMIN,FITMAX
        fr = h.Fit('pol2','S0','',FITMIN,FITMAX)
        f = h.GetFunction('pol2').Clone('stage3')
        f.SetLineColor(1)
        f.SetLineStyle(1)
        f.Draw('SAME')
        gbg.append(f)
        chimin = f.GetMinimum(FITMIN,FITMAX)
        fitted_xmin = f.GetMinimumX(FITMIN,FITMAX)
        xleft = f.GetX(chimin+1,FITMIN,fitted_xmin)
        xright = f.GetX(chimin+1,fitted_xmin,FITMAX)
        err = 0.5*((fitted_xmin-xleft)+(xright-fitted_xmin))
        lbl_xmin = 0.30
    return xmin,err,xleft,xright,fitted_xmin,lbl_xmin,chimin

# Scan the parameter space and determine best value & error
def create_scan_graph(model,datasN,nbins,ks,nscan,shift,tag,c2):
    kss = 'ks' if ks else 'chi'
    global gbg
    c1 = None
    if not ks:
        c1 = ROOT.TCanvas(kss+'scan',kss+'scan',1024,768)
        c1.Divide(3,3)
    zplot = 1
    chi2,frames = runscan(model,dataP,datasN,ks,nbins=nbins)
    for z in xrange(nscan):
        step = nscan/9 or 1
        if z%step==0 and frames[z]:
            # plot
            c1.cd(zplot)
            zplot+=1
            frames[z].Draw()
            # set pave text
            p = ROOT.TPaveText(.6,.70 , (.6+.40),(.70+.20) ,"NDC")
            p.SetName(rand_name())
            p.SetTextAlign(11)
            p.SetFillColor(0)
            p.AddText('Scale=%.2f, chi2=%.2f'%(scale[z],chi2[z]))
            p.Draw()
            gbg.append(p)
    gbg += frames
    if not ks:
        c1.Update()
    c2.cd()
    h = make_graph(nscan,scale,chi2)
    h.SetTitle('Momentum %s: best value and error (%s)'%('scale' if not shift else 'shift',tag))
    h.Draw('ACP')
    if ks:
        h.GetYaxis().SetRangeUser(0,1.3)
    gbg.append(h)
    xmin,err,xleft,xright,fitted_xmin,lbl_xmin,chimin=fitgraph(h,ks,FITMIN,FITMAX)
    if not ks:
        line = ROOT.TGraph(2)
        line.SetPoint(0,xleft,chimin+1)
        line.SetPoint(1,xright,chimin+1)
        line.SetLineWidth(4)
        line.SetLineColor(ROOT.kRed)
        line.Draw('l')
        gbg.append(line)
    p = ROOT.TPaveText(lbl_xmin,.79 , (lbl_xmin+.40),(.79+.10) ,"NDC")
    p.SetTextAlign(11)
    p.SetFillColor(0)
    if not shift:
        lbl = 'R0=%.2f%%+/-%.2f%%. Fit=%.2f%%. Avg=%.2f%%'%(xmin*100.0,err*100.0,fitted_xmin*100.0,avg_scale*100.0)
    else:
        lbl = 'S0=%.2f+/-%.2f(1e-6/GeV). Fit = %.2f. Avg=%.2f%%'%(xmin*1e6,err*1e6,fitted_xmin*1e6,avg_shift*1e6)
    p.AddText(lbl)
    p.AddText('N = %d (%d & %d). dN=%d sqrt=%d'%(nmax,Np,Nn,Np-Nn,int(math.sqrt(nmax))))
    p.SetName(rand_name())
    p.Draw()
    c2.Update()
    gbg.append(p); gbg.append(c2)
    COUT.append('%d peak = %.10f +/- %.10f ; chi2 = %.10f ; fitted_mean = %.10f ; avg_scale = %.10f ; avg_shift = %.10f ; nmax = %d ; npos = %d ; nneg = %d'%(ks,xmin,err,chimin,fitted_xmin,avg_scale,avg_shift,nmax,Np,Nn))
    # save results to antondb
    VMAP['%sp'%kss]=xmin
    VMAP['%sf'%kss]=fitted_xmin
    VMAP['%se'%kss]=err
    VMAP['%sval'%kss]=chimin
    return c1,fitted_xmin

if opts.scan:
    print 'Entering parameter scan studies'
    c = ROOT.TCanvas('fit','fit',600,800); c.Divide(1,2)
    crap, fitted_xmin_ks  = create_scan_graph(model,datasN,opts.nbins,True,opts.nscan,opts.shift,opts.tag+' '+opts.tt,c.cd(1))
    cscan,fitted_xmin_chi = create_scan_graph(model,datasN,opts.nbins,False,opts.nscan,opts.shift,opts.tag+' '+opts.tt,c.cd(2))
    c.Update()
    OMAP.append(c)
    OMAP.append(cscan)
    if False:
        SaveAs(c,'%s_fit'%opts.tag,opts.ext)
        if opts.savegrid:
            SaveAs(cscan,'%s_scan'%opts.tag,opts.ext)

# study statistical stability of distributions in subsamples
if opts.npergroup>0:
    print 'Entering splitted-sample studies in %s groups'%ngroups
    # chi2 method:
    ks = False
    chi_means = []
    chi_fitted = []
    chi_errors = []
    for j in range(ngroups):
        h = make_graph(opts.nscan,scale,runscan(models[j],datasPsp[j],datasNsp[j],ks)[0])
        xmin,err,xleft,xright,fitted_xmin,lbl_xmin,chimin=fitgraph(h,ks,FITMIN,FITMAX)
        chi_means.append(xmin)
        chi_fitted.append(fitted_xmin)
        chi_errors.append(err)
    # KS method:
    ks = True
    ks_means = []
    ks_fitted = []
    ks_errors = []
    for j in range(ngroups):
        h = make_graph(opts.nscan,scale,runscan(models[j],datasPsp[j],datasNsp[j],ks)[0])
        xmin,err,xleft,xright,fitted_xmin,lbl_xmin,chimin=fitgraph(h,ks,FITMIN,FITMAX)
        ks_means.append(xmin)
        ks_fitted.append(fitted_xmin)
        ks_errors.append(err)
    # plot
    cgroup = ROOT.TCanvas('cgroups','cgroups',600,800); cgroup.Divide(1,2)
    cgroup.cd(1)
    h_chi_mean = ROOT.TGraphErrors(len(chi_means))
    h_chi_mean.SetTitle("Chi2 method: %d subsamples of size %d"%(ngroups,opts.npergroup))
    h_chi_mean.SetMarkerColor(ROOT.kRed);
    h_chi_mean.SetLineColor(ROOT.kRed);
    h_chi_mean.SetMarkerSize(1.5);
    h_chi_mean.SetMarkerStyle(21);
    h_chi_fitted = ROOT.TGraphErrors(len(chi_fitted))
    h_chi_fitted.SetTitle("Chi2 method: %d subsamples of size %d"%(ngroups,opts.npergroup))
    h_chi_fitted.SetMarkerColor(ROOT.kBlue);
    h_chi_fitted.SetLineColor(ROOT.kBlue);
    h_chi_fitted.SetMarkerSize(1.0);
    h_chi_fitted.SetMarkerStyle(21);
    for z in xrange(len(chi_means)):
        h_chi_mean.SetPoint(z,z+1,chi_means[z])
        h_chi_mean.SetPointError(z,0,chi_errors[z])
        h_chi_fitted.SetPoint(z,z+1,chi_fitted[z])
        h_chi_fitted.SetPointError(z,0,chi_errors[z])
    oldf = ROOT.gStyle.GetOptFit()
    ROOT.gStyle.SetOptFit(1111);
    h_chi_mean.Fit("pol0")
    h_chi_mean.Draw('A*')
    h_chi_fitted.Draw('*SAMES')
    #h_chi_fitted.Draw('A*SAMES')
    OMAP.append(cgroup)
    if False:
        SaveAs(cgroup,'%s_statcheck'%opts.tag,opts.ext)
    ROOT.gStyle.SetOptFit(oldf);

if opts.varbins:
    print 'Studying variation of results on number of bins'
    means = []
    errors = []
    bins = range(3,opts.nbins)
    for ibins in bins:
        h = make_graph(opts.nscan,scale,runscan(model,dataP,datasN,opts.ks,ibins)[0])
        xmin,err,xleft,xright,xtra,lbl_xmin,chimin=fitgraph(h,False,FITMIN,FITMAX)
        means.append(xmin)
        errors.append(err)
    cvarbins = ROOT.TCanvas('cvarbins','cvarbins',800,600); cvarbins.cd()
    h = ROOT.TGraphErrors(len(bins))
    h.SetTitle("Effect of chi2 binning")
    h.SetMarkerColor(4);
    h.SetMarkerSize(1.5);
    h.SetMarkerStyle(21);
    for z in xrange(len(bins)):
        h.SetPoint(z,bins[z],means[z])
        h.SetPointError(z,0,errors[z])
    oldf = ROOT.gStyle.GetOptFit()
    ROOT.gStyle.SetOptFit(1111);
    h.Fit("pol0")
    h.Draw('A*')
    #ROOT.gPad.SetLogy(ROOT.kTRUE)
    OMAP.append(cvarbins)    
    if False:
        SaveAs(cvarbins,'%s_varbins'%opts.tag,opts.ext)
    ROOT.gStyle.SetOptFit(oldf);

# Plot template shape
if opts.template:
    tmponly = False  # for making plots
    # plot model for POS and data for POS
    if not tmponly:
        cmodel = ROOT.TCanvas('ctemplate','ctemplate',600,800)
        cmodel.Divide(1,2)
    else:
        cmodel = ROOT.TCanvas('ctemplate','ctemplate',640,480)
    if not opts.ks:
        cmodel.cd(1)
        frame = x.frame()
        color=ROOT.kBlue
        RooAbsData.plotOn(data_model,frame,RF.LineColor(color),RF.MarkerColor(color),RF.Binning(20))
        model.plotOn(frame,RF.LineColor(color))
        frame.SetTitle('mu+ 1/pt and the derived smooth PDF')
        frame.Draw()
        gbg.append(frame)
    # plot default and best-fitted spectra on top of each other
    if opts.scan and not tmponly:
        cmodel.cd(2)
        # create dataN shifted by "best-fitted" value
        dataN_chi = RooDataSet('dataN','Zmumu mu- data',RooArgSet(x))
        dataN_ks  = RooDataSet('dataN','Zmumu mu- data',RooArgSet(x))
        for i in range(0,nmax):
            x.setVal(func(neg[i]*opts.forcescale,fitted_xmin_chi))
            dataN_chi.add(RooArgSet(x))
            x.setVal(func(neg[i]*opts.forcescale,fitted_xmin_ks))
            dataN_ks.add(RooArgSet(x))
        # make the plot with 3 histograms overlayed
        frame = x.frame()
        color=ROOT.kRed
        RooAbsData.plotOn(dataP,frame,RF.LineColor(color),RF.MarkerColor(color),RF.Binning(20),RF.MarkerSize(2))
        color=ROOT.kBlue
        RooAbsData.plotOn(dataN,frame,RF.LineColor(color),RF.MarkerColor(color),RF.Binning(20),RF.MarkerSize(1.5))
        color=8
        RooAbsData.plotOn(dataN_ks,frame,RF.LineColor(color),RF.MarkerColor(color),RF.Binning(20),RF.MarkerSize(1.0))
        color=5
        RooAbsData.plotOn(dataN_ks,frame,RF.LineColor(color),RF.MarkerColor(color),RF.Binning(20),RF.MarkerSize(0.5))
        frame.SetTitle('Green = KS scale (%.2f%%). Yellow = chi scale (%.2f%%)'%(fitted_xmin_ks*100.0,fitted_xmin_chi*100.0))
        frame.Draw()
    OMAP.append(cmodel)
    if False:
        SaveAs(cmodel,'%s_template'%opts.tag,opts.ext)

# save to text file
if len(COUT)>0:
    VMAP['COUT']=[]
    for l in COUT:
        print l
        VMAP['COUT'].append(l)

if len(VMAP)>0 or len(OMAP)>0:
    a = antondb(opts.antondb)
    sample_path = '/default/cmb/BB/R0'
    mtype = 'KK' if opts.root2 else 'R0'
    if opts.shift:
        mtype = 'SHIFT'
    path = os.path.join('/keysfit/',opts.tag,opts.tt,opts.region,mtype)
    if len(VMAP)>0:
        a.add(path,VMAP)
    if len(OMAP)>0:
        a.add_root(path,OMAP)
