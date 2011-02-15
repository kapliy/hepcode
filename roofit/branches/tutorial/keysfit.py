#!/usr/bin/env python

# study muon momentum scale using z->mumu muon spectra
import sys,array
import SimpleProgressBar

def func_SCALE_old(xx,par):
    return par*1.0/xx
def func_SCALE(xx,par):
    return 1.0/par*1.0/xx
def func_SHIFT(xx,par):
    return 1.0/xx + par

from optparse import OptionParser
parser = OptionParser()
# data sources
parser.add_option("--root",dest="root",
                  type="string", default='root_all.root',
                  help="Input ROOT file with all histograms")
parser.add_option("--data",dest="data",
                  type="string", default='data_data.root/dg/dg/st_z_final/BB/graph_lpt_P_N',
                  help="TGraph containing data histograms")
parser.add_option("--ndata",dest="ndata",
                  type="int", default=10000,
                  help="Number of unbinned data points to load")
parser.add_option("--nskip",dest="nskip",
                  type="int", default=0,
                  help="Number of data points to skip load")
parser.add_option("--forcescale",dest="forcescale",
                  type="float", default=1.0,
                  help="Force a particular scale factor on positive muons")
parser.add_option("--mc",dest="mc",
                  type="string", default='mc_zmumu_mc_zmumu.root/dg/dg/st_z_final/BB/graph_lpt_P_N',
                  help="TGraph containing MC histograms")
parser.add_option("--nmc",dest="nmc",
                  type="int", default=1,
                  help="Number of unbinned MC points to load")
parser.add_option("--rookeys",dest="rookeys",
                  type="string", default=None,
                  help="ROOT file from which we load a cached RooKeysPdf")
parser.add_option("--rookeysout",dest="rookeysout",
                  type="string", default='workspace.root',
                  help="ROOT file into which we will save a cached RooKeysPdf")
parser.add_option('-t',"--tag",dest="tag",
                  type="string", default='plot',
                  help="A tag to append to all output plots")
# parameters
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
                  type="int", default=2,
                  help="Smoothing factor for kernel estimation")
parser.add_option("--nbins",dest="nbins",
                  type="int", default=10,
                  help="Binning for chi2 calculation")
parser.add_option("--nscan",dest="nscan",
                  type="int", default=20,
                  help="Number of parameter values to scan")
# enable modules
parser.add_option("--batch", default=False,
                  action="store_true",dest="batch",
                  help="Enable batch mode (all output directly to filesystem)")
parser.add_option("--template", default=False,
                  action="store_true",dest="template",
                  help="Make a plot of the template")
parser.add_option("--scan", default=False,
                  action="store_true",dest="scan",
                  help="Scan through parameter space and determine best value and error")
parser.add_option("--kolmogorov", default=False,
                  action="store_true",dest="kolmogorov",
                  help="Perform unbinned KS comparison of two spectra")
parser.add_option("--ngroups", dest="ngroups",
                  type="int", default=0,
                  help="Split the sample into ngroups subsamples of size ndata and study variations of fitted mean")
parser.add_option("--varbins", default=False,
                  action="store_true",dest="varbins",
                  help="Study systematic effect of bin variation")

(opts, args) = parser.parse_args()

import ROOT
ROOT.gROOT.SetBatch(opts.batch)
from ROOT import RooWorkspace,RooArgSet,RooArgList,RooDataHist,RooDataSet,RooAbsData,RooFormulaVar
from ROOT import RooHistPdf,RooKeysPdf,RooNDKeysPdf
from ROOT import kTRUE,kFALSE,kDashed,gStyle,gPad
from ROOT import TFile,TH1,TH1F,TH1D
from ROOT import RooFit as RF
w = RooWorkspace('w',kTRUE)

try:
    import psyco
    psyco.full()
except ImportError:
    pass

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
    # truth datasets
    #################################################
    f = TFile(opts.root,'r')
    hz = f.Get(opts.mc)
    f.Close()
    N = hz.GetN()
    print 'Loaded truth graph',opts.mc,'with',N,'entries'
    pos = hz.GetX()
    neg = hz.GetY()
    # make truth(x) for positive and negative muons
    truthP = RooDataSet('truthP','Zmumu mu+ MC',RooArgSet(x))
    truthN = RooDataSet('truthN','Zmumu mu- MC',RooArgSet(x))
    nmax = min(N,opts.nmc); nmaxp10 = nmax if nmax>10 else 10
    bar = SimpleProgressBar.SimpleProgressBar(10,nmax)
    for i in range(0,nmax):
        if i%(int(nmaxp10/10))==0 and i>0:
            print bar.show(i)
        x.setVal(1.0/pos[i])
        truthP.add(RooArgSet(x))
        x.setVal(1.0/neg[i])
        truthN.add(RooArgSet(x))
    getattr(w,'import')(truthP,RF.Rename('truthP'))
    getattr(w,'import')(truthN,RF.Rename('truthN'))
    #################################################
    # data datasets
    #################################################
    f = TFile(opts.root,'r')
    hz = f.Get(opts.data)
    f.Close()
    N = hz.GetN()
    nmax = min(N,opts.nskip+opts.ndata)
    nstep = nmax-opts.nskip if nmax-opts.nskip>10 else 10
    ngroups = opts.ngroups if opts.ngroups < int(N/nmax) else int(N/nmax)
    print 'Loaded data graph',opts.data,'with',N,'entries','in',ngroups,'groups'
    pos = hz.GetX()
    neg = hz.GetY()
    # make data(x) for positive and negative muons
    dataP = RooDataSet('dataP','Zmumu mu+ data',RooArgSet(x))
    dataN = RooDataSet('dataN','Zmumu mu- data',RooArgSet(x))
    datasP = [RooDataSet('datasP%s'%i,'Zmumu mu+ data bin %s'%i,RooArgSet(x)) for i in range(opts.nscan)]
    datasN = [RooDataSet('datasN%s'%i,'Zmumu mu- data bin %s'%i,RooArgSet(x)) for i in range(opts.nscan)]
    datasNsp = []
    for j in range(ngroups):
        datasNsp.append([])
        for i in range(opts.nscan):
            datasNsp[j].append(RooDataSet('datasNsp%s_%s'%(i,j),'Zmumu mu- data bin %s %s'%(i,j),RooArgSet(x)))
    # default dataP and dataN
    bar = SimpleProgressBar.SimpleProgressBar(10,nmax)
    print 'Loading default data...'
    for i in range(opts.nskip,nmax):
        x.setVal(1.0/pos[i]*opts.forcescale)
        dataP.add(RooArgSet(x))
        x.setVal(1.0/neg[i])
        dataN.add(RooArgSet(x))
    # scaled shifted versions for manual scans
    if opts.ngroups==0:
        print 'Loading scaled data arrays...'
        for i in range(opts.nskip,nmax):
            if (i-opts.nskip)%(int(nstep/10))==0 and i-opts.nskip>0:
                print bar.show(i)
            for z in range(opts.nscan):
                x.setVal(func(neg[i],scale[z]))
                datasN[z].add(RooArgSet(x))
    # scaled / shifted versions in multiple splits
    else:
        print 'Loading scaled data arrays in %s chunks...'%ngroups
        for j in range(ngroups):
            print 'Loading range %s/%s'%(j,ngroups)
            for i in range(nmax*j,nmax*(j+1)):
                if i>=N: continue
                if (i)%(int(nstep/10))==0 and i>0:
                    print bar.show(i-nmax*j)
                for z in range(opts.nscan):
                    x.setVal(func(neg[i],scale[z]))
                    datasNsp[j][z].add(RooArgSet(x))
    getattr(w,'import')(dataP,RF.Rename('dataP'))
    getattr(w,'import')(dataN,RF.Rename('dataN'))
    # make truth(x*b)
    w.factory("expr::xf('x*1.0/b',x,b)")
    #w.factory("KeysPdf::model(x,dataP,MirrorBoth,2)")
    #w.factory("KeysPdf::model(expr('x*1.0/b',x,b),dataP,MirrorBoth,2)")
    data_model = dataP
    if opts.rookeys:
        fkeys = ROOT.TFile.Open(opts.rookeys)
        assert fkeys.IsOpen(),'Failed to open RooKeysPdf cache file: %s'%opts.rookeys
        model = fkeys.wsave.pdf('model')
        getattr(w,'import')(model)
        fkeys.Close()
    elif not opts.kolmogorov:
        print 'Making RooKeysPdf. Please wait...'
        model = RooKeysPdf('model','model from truth',x,data_model,RooKeysPdf.MirrorBoth,opts.smooth)
        wsave = RooWorkspace('wsave',kTRUE)
        getattr(wsave,'import')(model)
        wsave.writeToFile(opts.rookeysout)
        xf = w.function('xf')
        #model = RooNDKeysPdf('model','model',RooArgList(x),data_model,'am')
        #model = RooNDKeysPdf('model','model',RooArgList(xf),data_model,'am')
        #model = RooHistPdf('model','model',RooArgList(xf),RooArgList(w.var('x')),truth,4)
        getattr(w,'import')(model)
    isExt = kFALSE

# make a joint pdf for various signal regions
w.factory("")
w.defineSet('X','x')
model = w.pdf('model')
x = w.var('x')
chi2 = []

# Plot template shape
if opts.template:
    c3 = ROOT.TCanvas('c3','c3',640,480); c3.cd()
    frame = x.frame()
    color=ROOT.kBlue
    RooAbsData.plotOn(data_model,frame,RF.LineColor(color),RF.MarkerColor(color),RF.Binning(20))
    model.plotOn(frame,RF.LineColor(color))
    frame.Draw()
    c3.SaveAs('%s_template.png'%opts.tag)

# Perform unbinned Kolmogorov comparison
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
    assert len(arrayP)==len(arrayN),'ERROR: different array sizes'
    pval = ROOT.TMath.KolmogorovTest(len(arrayP),arrayP,len(arrayN),arrayN,'')
    return pval
if False:
    print 'Entering Kolmogorov studies'
    print p_value(dataP,dataN)

def plot_data(data,color=ROOT.kBlack,nbins=10):
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
def runscan(data,is_kolmogorov,nbins=opts.nbins):
    """ Scan through parameter space and compute chi2 for each parameter value """
    res = []
    frames = []
    for z in xrange(opts.nscan):
        if is_kolmogorov:
            frame,chi = None,p_value(dataP,data[z])
        else:
            frame,chi = plot_data(data[z],nbins=nbins);
        res.append(chi)
        frames.append(frame)
    return res,frames
def make_graph(n,xs,ys):
    """ Make a TGraph for scanned_parameters -vs- chi2"""
    h = ROOT.TGraph(n)
    h.SetMarkerColor(4);
    h.SetMarkerSize(1.5);
    h.SetMarkerStyle(21);
    for z in xrange(n):
        h.SetPoint(z,xs[z],ys[z])
    return h
def fitgraph(h,is_kolmogorov,FITMIN,FITMAX):
    """ Fit chi2 TGraph to a parabola and extract chi2 minimum and error band around (chi2-1) """
    if is_kolmogorov:
        # fit gaussian core as an approximation for KS probability distribution
        rms,mean=h.GetRMS(),h.GetMean()
        rms,pc=h.GetRMS(),max([(h.GetY()[i],h.GetX()[i]) for i in xrange(h.GetN())])
        peak,chimean = pc[1],pc[0]
        print 'peak and rms:',peak,rms
        FITMIN=peak-rms
        FITMAX=peak+rms
        print 'Fitting range:',FITMIN,FITMAX
        fr = h.Fit('gaus','S','',FITMIN,FITMAX)
        f = h.GetFunction('gaus')
        chimin = f.GetMaximum(FITMIN,FITMAX)
        xmin = peak
        fitted_xmin = f.GetParameter(1)
        err = f.GetParameter(2)
        xleft,xright = xmin-err,xmin+err
        lbl_xmin = 0.55
        xtra='fitmean=%.10f%%'%(fitted_xmin*100.0)
    else:
        # fit parabola
        fr = h.Fit('pol2','S','',FITMIN,FITMAX)
        f = h.GetFunction('pol2')
        chimin = f.GetMinimum(FITMIN,FITMAX)
        xmin = f.GetMinimumX(FITMIN,FITMAX)
        xleft = f.GetX(chimin+1,FITMIN,xmin)
        xright = f.GetX(chimin+1,xmin,FITMAX)
        err = 0.5*((xmin-xleft)+(xright-xmin))
        lbl_xmin = 0.35
        xtra=''
    return xmin,err,xleft,xright,xtra,lbl_xmin,chimin
    
# Scan the parameter space and determine best value & error
if opts.scan:
    print 'Entering parameter scan studies'
    if not opts.kolmogorov:
        c = ROOT.TCanvas('c','c',1024,768)
        c.Divide(3,3)
    ps = [] # garbage collector
    zplot = 1
    chi2,frames = runscan(datasN,opts.kolmogorov,nbins=opts.nbins)
    for z in xrange(opts.nscan):
        step = opts.nscan/9 or 1
        if z%step==0 and frames[z]:
            # plot
            c.cd(zplot)
            zplot+=1
            frames[z].Draw()
            # set pave text
            p = ROOT.TPaveText(.6,.70 , (.6+.30),(.70+.20) ,"NDC")
            p.SetTextAlign(11)
            p.SetFillColor(0)
            p.AddText('Scale=%.2f, chi2=%.2f'%(scale[z],chi2[z]))
            p.Draw()
            ps.append(p)
    if not opts.kolmogorov:
        c.Update()
        c.SaveAs('%s_scan.png'%opts.tag)
    c2 = ROOT.TCanvas('c2','c2',800,600); c2.cd()
    h = make_graph(opts.nscan,scale,chi2)
    h.SetTitle('Momentum %s: best value and error (%s)'%('scale' if not opts.shift else 'shift',opts.tag))
    h.Draw('ACP')
    xmin,err,xleft,xright,xtra,lbl_xmin,chimin=fitgraph(h,opts.kolmogorov,FITMIN,FITMAX)
    if not opts.kolmogorov:
        line = ROOT.TGraph(2)
        line.SetPoint(0,xleft,chimin+1)
        line.SetPoint(1,xright,chimin+1)
        line.SetLineWidth(4)
        line.SetLineColor(ROOT.kRed)
        line.Draw('l')
    p = ROOT.TPaveText(lbl_xmin,.80 , (lbl_xmin+.35),(.80+.10) ,"NDC")
    p.SetTextAlign(11)
    p.SetFillColor(0)
    if not opts.shift:
        lbl = 'Best value (scale) = %.2f%% +/- %.2f%%'%(xmin*100.0,err*100.0)
    else:
        lbl = 'Best value (shift) = %.3f +/- %.3f (1000/GeV)'%(xmin*1000.0,err*1000.0)
    p.AddText(lbl)
    p.Draw()
    c2.SaveAs('%s_chi2.png'%opts.tag)
    print lbl,'chi2 = %.2f'%chimin
    # save in the output file
    fout = open('%s_results.rtxt'%opts.tag,'w')
    print >>fout,'value = %.10f +/- %.10f ; chi2 = %.10f %s'%(xmin,err,chimin,xtra)
    fout.close()

# study statistical stability of distributions
if opts.ngroups>0:
    print 'Entering splitted-sample studies in %s groups'%ngroups
    means = []
    errors = []
    for j in range(ngroups):
        h = make_graph(opts.nscan,scale,runscan(datasNsp[j],opts.kolmogorov)[0])
        xmin,err,xleft,xright,xtra,lbl_xmin,chimin=fitgraph(h,opts.kolmogorov,FITMIN,FITMAX)
        means.append(xmin)
        errors.append(err)
    mean = sum(means)/len(means)
    err = sum(errors)/len(errors)
    c2 = ROOT.TCanvas('c2','c2',800,600); c2.cd()
    h = ROOT.TH1F('h','h',20,mean-err,mean+err)
    [h.Fill(m) for m in means]
    h.Draw()
    c2.SaveAs('%s_statcheck.png'%opts.tag)

if opts.varbins:
    print 'Entering splitted-sample studies in %s groups'%ngroups
    means = []
    errors = []
    bins = range(2,opts.nbins)
    for ibins in bins:
        h = make_graph(opts.nscan,scale,runscan(datasN,opts.kolmogorov,ibins)[0])
        xmin,err,xleft,xright,xtra,lbl_xmin,chimin=fitgraph(h,False,FITMIN,FITMAX)
        means.append(xmin)
        errors.append(err)
    c2 = ROOT.TCanvas('c2','c2',800,600); c2.cd()
    h = ROOT.TGraphErrors(len(bins))
    h.SetMarkerColor(4);
    h.SetMarkerSize(1.5);
    h.SetMarkerStyle(21);
    for z in xrange(len(bins)):
        h.SetPoint(z,bins[z],means[z])
        h.SetPointError(z,0,errors[z])
    h.Draw('A*')
    #ROOT.gPad.SetLogy(ROOT.kTRUE)
    c2.SaveAs('%s_varbins.png'%opts.tag)
