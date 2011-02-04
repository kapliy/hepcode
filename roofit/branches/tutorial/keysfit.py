#!/usr/bin/env python

# study muon momentum scale using z->mumu muon spectra
import sys,array
import SimpleProgressBar

def func_SCALE(xx,par):
    return par*1.0/xx
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
parser.add_option("--mc",dest="mc",
                  type="string", default='mc_zmumu_mc_zmumu.root/dg/dg/st_z_final/BB/graph_lpt_P_N',
                  help="TGraph containing MC histograms")
parser.add_option("--nmc",dest="nmc",
                  type="int", default=1,
                  help="Number of unbinned MC points to load")
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
    scale = [(0.93 + 0.14*zz/opts.nscan) for zz in xrange(opts.nscan)]
    FITMIN=0.95  #parameter fitting minimum
    FITMAX=1.05
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
    print 'Loaded data graph',opts.data,'with',N,'entries'
    pos = hz.GetX()
    neg = hz.GetY()
    # make data(x) for positive and negative muons
    dataP = RooDataSet('dataP','Zmumu mu+ data',RooArgSet(x))
    dataN = RooDataSet('dataN','Zmumu mu- data',RooArgSet(x))
    datasP = [RooDataSet('datasP%s'%i,'Zmumu mu+ data bin %s'%i,RooArgSet(x)) for i in range(opts.nscan)]
    datasN = [RooDataSet('datasN%s'%i,'Zmumu mu- data bin %s'%i,RooArgSet(x)) for i in range(opts.nscan)]
    nmax = min(N,opts.ndata); nmaxp10 = nmax if nmax>10 else 10
    bar = SimpleProgressBar.SimpleProgressBar(10,nmax)
    for i in range(0,nmax):
        if i%(int(nmaxp10/10))==0 and i>0:
            print bar.show(i)
        x.setVal(1.0/pos[i])
        dataP.add(RooArgSet(x))
        x.setVal(1.0/neg[i])
        dataN.add(RooArgSet(x))
        if True: # scaled or shifted versions
            for z in range(opts.nscan):
                x.setVal(func(pos[i],scale[z]))
                datasP[z].add(RooArgSet(x))
                x.setVal(func(neg[i],scale[z]))
                datasN[z].add(RooArgSet(x))
    getattr(w,'import')(dataP,RF.Rename('dataP'))
    getattr(w,'import')(dataN,RF.Rename('dataN'))
    # make truth(x*b)
    w.factory("expr::xf('x*1.0/b',x,b)")
    #w.factory("KeysPdf::model(x,dataP,MirrorBoth,2)")
    #w.factory("KeysPdf::model(expr('x*1.0/b',x,b),dataP,MirrorBoth,2)")
    if not opts.kolmogorov:
        data_model = dataP
        print 'Making RooKeysPdf. Please wait...'
        # todo - caching mechanism!
        model = RooKeysPdf('model','model from truth',x,data_model,RooKeysPdf.MirrorBoth,opts.smooth)
        xf = w.function('xf')
        #model = RooNDKeysPdf('model','model',RooArgList(x),data_model,'am')
        #model = RooNDKeysPdf('model','model',RooArgList(xf),data_model,'am')
        #model = RooHistPdf('model','model',RooArgList(xf),RooArgList(w.var('x')),truth,4)
        getattr(w,'import')(model)
    #w.writeToFile('workspace.root')
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
if opts.kolmogorov:
    print 'Entering Kolmogorov studies'
    print p_value(dataP,dataN)

# Scan the parameter space and determine best value & error
def plot_data(data,color=ROOT.kBlack,nbins=10):
    frame = x.frame(RF.Title('1/p_{T}'))
    RooAbsData.plotOn(data,frame,RF.LineColor(color),RF.MarkerColor(color),RF.Binning(nbins))
    model.plotOn(frame)
    chi = frame.chiSquare(1)
    #model.paramOn(frame,data)
    return frame,chi
if opts.scan:
    print 'Entering parameter scan studies'
    if not opts.kolmogorov:
        c = ROOT.TCanvas('c','c',1024,768)
        c.Divide(3,3)
    ps = [] # garbage collector
    zplot = 1
    for z in xrange(opts.nscan):
        if opts.kolmogorov:
            frame,chi = None,p_value(dataP,datasN[z])
        else:
            frame,chi = plot_data(datasN[z],nbins=opts.nbins); ps.append(frame)
        chi2.append(chi)
        step = opts.nscan/9 or 1
        if z%step==0 and frame:
            # plot
            c.cd(zplot)
            zplot+=1
            frame.Draw()
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
    h = ROOT.TGraph(opts.nscan)
    h.SetMarkerColor(4);
    h.SetMarkerSize(1.5);
    h.SetMarkerStyle(21);
    h.SetTitle('Momentum %s: best value and error (%s)'%('scale' if not opts.shift else 'shift',opts.tag))
    for z in xrange(opts.nscan):
        h.SetPoint(z,scale[z],chi2[z])
        #print scale[z],chi2[z]
    h.Draw('ACP')
    xtra=''
    if opts.kolmogorov:
        # fit gaussian core
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
        xmin = f.GetParameter(1)
        err = f.GetParameter(2)
        lbl_xmin = 0.55
        xtra='max=%.2f%%'%(peak*100.0)
    else:
        # fit parabola
        fr = h.Fit('pol2','S','',FITMIN,FITMAX)
        f = h.GetFunction('pol2')
        chimin = f.GetMinimum(FITMIN,FITMAX)
        xmin = f.GetMinimumX(FITMIN,FITMAX)
        xleft = f.GetX(chimin+1,FITMIN,xmin)
        xright = f.GetX(chimin+1,xmin,FITMAX)
        err = 0.5*((xmin-xleft)+(xright-xmin))
        line = ROOT.TGraph(2)
        line.SetPoint(0,xleft,chimin+1)
        line.SetPoint(1,xright,chimin+1)
        line.SetLineWidth(4)
        line.SetLineColor(ROOT.kRed)
        line.Draw('l')
        lbl_xmin = 0.35
    p = ROOT.TPaveText(lbl_xmin,.80 , (lbl_xmin+.35),(.80+.10) ,"NDC")
    p.SetTextAlign(11)
    p.SetFillColor(0)
    if not opts.shift:
        lbl = 'Best value (scale) = %.2f%% +/- %.2f%% %s'%(xmin*100.0,err*100.0,xtra)
    else:
        lbl = 'Best value (shift) = %.3f +/- %.3f (1000/GeV) %s'%(xmin*1000.0,err*1000.0,xtra)
    p.AddText(lbl)
    p.Draw()
    c2.SaveAs('%s_chi2.png'%opts.tag)
    print lbl,'chi2 = %.2f'%chimin
    # save in the output file
    fout = open('%s_results.rtxt'%opts.tag,'w')
    print >>fout,'value = %.10f +/- %.10f ; chi2 = %.10f %s'%(xmin,err,chimin,xtra)
    fout.close()
