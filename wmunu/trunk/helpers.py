#!/usr/bin/env python

import math,re,os,glob,time
from math import sqrt,fabs,cos,sin
import ROOT
import PyCintex;
PyCintex.Cintex.Enable()

# constants
wMASS=80.4
zMASS=91.19
muMASS=0.1056584
GeV=1
mm=1

# mass window for W id via transverse mass:
wMIN=40*GeV
wMAX=1000*GeV   # only require mT>40 GeV

# binning (default)
nobjbins = (21,-0.5,20.5,'N')
ptbins = (100,0.,200.,'Pt, [GeV]')
dptbins = (20,-50,50.,'delta-Pt, [GeV]')
mtbins = (100,0.,200.,'Mt, [GeV]')
etisobins = (100,0.,8.,'Et isolation, [GeV]')
ptisobins = (20,0.,1.,'Pt isolation')
wmassbins = (100,0.,200.,'Mass [GeV]');
metbins = (100,0.,200.,'Missing Et [GeV]')
etabins = (50,-3.0,3.0,'Eta')
ybins = (50,-3.0,3.0,'Rapidity')
asbins = (50,-3.0,3.0,'Eta')
phibins = (50,-math.pi,math.pi,'Phi')
qbins = (3,-1.01,1.01,'Charge')

# binning (recommendations from WZ early observation papers)
# https://twiki.cern.ch/twiki/bin/view/AtlasProtected/WZObservationWithMuons
ptbins = (20,0.,100.,'Pt, [GeV]')
mtbins = (24,0.,120.,'Mt, [GeV]')
metbins = (20,0.,100.,'Missing Et [GeV]')
etabins = (20,-2.4,2.4,'Eta')
phibins = (20,-math.pi,math.pi,'Phi')


def findBin(bins,val):
    """ finds which bin a value belongs to """
    binw=(asbins[2]-asbins[1])/asbins[0]
    for i in range(0,bins[0]+1):
        lb=bins[1]+i*binw
        if val>=lb and val<lb+binw:
            break
    return i

h = {}
_ext = 'png'

# a few PDG ids
DOWN=1
UP=2
STRANGE=3
CHARM=4
BOTTOM=5
TOP=6
PROTON=2212
NEUTRON=2112
ELECTRON=11
POSITRON=-11
MUON=13
ANTIMUON=-13
TAU=15
NEUTRINOS=(12,14,16,-12,-14,-16)
PHOTON=22
Z0=23
WPLUS=24
WMINUS=-24
GLUON=21
HIGGS=25

class GoodRunsList:
    """ General GRL abstraction """
    libname = 'libGoodRunsLists.so'
    def __init__(s,xmls):
        if not ROOT.gSystem.Load(s.libname)==0:
            print 'Failed to load',s.libname
            sys.exit(0)
        s.load(xmls)
    def load(s,xmls):
        """ Accepts comma-separated list of xml files """
        s.xmls = xmls
        s.reader = ROOT.Root.TGoodRunsListReader()
        for xml in s.xmls.split(','):
            for file in glob.glob(xml):
                s.reader.AddXMLFile(file)
        s.reader.Interpret()
        s.grl = s.reader.GetMergedGRLCollection()
    def summary(s):
        s.grl.Summary()
    def passRunLB__real(s,run,lb):
        return s.grl.HasRunLumiBlock(run,lb)
    @staticmethod
    def passRunLB__dummy(*args,**kwargs):
        return 1

def RecoW(emu,pmux,pmuy,pmuz,pnux,pnuy,mW=wMASS):
    """ Reconstructs W from Wmunu assuming W mass value
    by solving a quadratic equation. Sometimes both solutions are acceptable.
    """
    def lhs(enu):
        return sqrt((emu+enu)**2-(pmux+pnux)**2-(pmuy+pnuy)**2-(pmuz+sqrt(enu**2-pnux**2-pnuy**2))**2)
    ws = []
    enus = []
    ptnu=sqrt(pnux**2+pnuy**2)
    ptmu=sqrt(pmux**2+pmuy**2)
    alpha=(pmux+pnux)**2+(pmuy+pnuy)**2
    beta=emu**2-alpha-pmuz**2+ptnu**2-mW**2
    # ax^2+bx+c=0
    a=4*(emu**2-pmuz**2)
    b=4*emu*beta
    c=beta**2+4*pmuz**2*ptnu**2
    D=b**2-4*a*c
    if D<0:
        return ws
    # first solution
    enu=(-b+sqrt(D))/(2*a)
    if enu**2-ptnu**2 >= 0 and fabs((lhs(enu)-mW))<0.1:
        w1 = ROOT.TLorentzVector()
        w1.SetPxPyPzE(pmux+pnux,pmuy+pnuy,pmuz+sqrt(enu**2-ptnu**2),emu+enu)
        enus.append(enu)
        ws.append(w1)
    enu=(-b-sqrt(D))/(2*a)
    if enu**2-ptnu**2 >= 0 and fabs((lhs(enu)-mW))<0.1:
        w2 = ROOT.TLorentzVector()
        w2.SetPxPyPzE(pmux+pnux,pmuy+pnuy,pmuz+sqrt(enu**2-ptnu**2),emu+enu)
        enus.append(enu)
        ws.append(w2)
    return ws

def MakeAsymmRatioHistos(hplus,hminus,label):
    """ Makes an asymmetry histogram from a + and - histo """
    histo_asymm=WAsymmetry(hplus,hminus,"ASYMM_%s"%label)
    Histo(histo_asymm.GetTitle(),histo=histo_asymm)
    histo_ratio=WRatio(hplus,hminus,"RATIO_%s"%label)
    Histo(histo_ratio.GetTitle(),histo=histo_ratio)

def MakeEffHistos(hreco,htruth,label):
    """ Makes efficiency histograms in various parameter bins """
    histo_eff=WRatio(hreco,htruth,"EFF_%s"%label,"eff")
    Histo(histo_eff.GetTitle(),histo=histo_eff)

def WRatio(hplus,hminus,title,label='ratio'):
    """ Ratio of W+ to W- workhorse function
    Alternatively, can be used to calculate efficiency
    """
    hratio = hplus.Clone(hplus.GetName()+"_"+label)
    hratio.Divide(hminus)
    hratio.SetTitle(title)
    return hratio

def WAsymmetry(hplus,hminus,title):
    """ (W+ - W-)/(W+ + W-) workhorse function """
    hsum = hplus.Clone(hplus.GetName()+" asymmetry")
    hsum.Add(hminus,-1.0)
    hdiff = hplus.Clone(hplus.GetName()+" asymmetry diff")
    hdiff.Add(hminus)
    hsum.Divide(hdiff)
    hsum.SetTitle(title)
    return hsum

def Print4vec(label,v):
    print '%s: M=%.2f Eta=%.2f Phi=%.2f Pt=%.2f'%(label,v.M(),v.Eta(),v.Phi(),v.Pt())

def SetStyle(styleMacro=''):
    """ Global style settings for ROOT """
    if styleMacro=='':
        ROOT.gStyle.SetCanvasColor(0);
        ROOT.gStyle.SetCanvasBorderMode(0);
        ROOT.gStyle.SetPadColor(0);
        ROOT.gStyle.SetPadBorderMode(0);
        ROOT.gStyle.SetStatColor(0);
        
        ROOT.gStyle.SetOptStat(111111);
        ROOT.gStyle.SetOptFit(1111);
        ROOT.gStyle.SetHistFillColor(0);
        ROOT.gStyle.SetMarkerStyle(20);
        ROOT.gStyle.SetMarkerSize(.4);
        ROOT.gStyle.SetHistLineWidth(2);
        ROOT.gStyle.SetErrorX(0);
        
        ROOT.gStyle.SetTitleStyle(0);
        
        ROOT.gStyle.SetStatBorderSize(1);
        ROOT.gStyle.SetFrameFillColor(10);
        ROOT.gStyle.SetTitleFillColor(0);
        ROOT.gROOT.ForceStyle()
    else:
        ROOT.gROOT.LoadMacro(styleMacro);

def Legend(hreco,htruth):
    """ Draws a legend with two histograms on gPad """
    leg = ROOT.TLegend(0.518321,0.715889,0.835008,0.853519,NULL,"brNDC")
    leg.SetBorderSize(0)
    leg.SetFillColor(0)
    leg.AddEntry(hReco, "Reco","l")
    leg.AddEntry(hTruth,"Truth","l")
    leg.Draw()

def hgood(title):
    return (title in h) and h[title].GetEntries()

def FillQ(title,q,*args):
    """ Fill per-charge histograms """
    h['%s_%s'%(title,q)].Fill(*args)

_qhistos = []
def HistoQ(*args,**kwargs):
    """ Books general and per-charge histograms"""
    title = args[0]
    _qhistos.append(title)
    for q in ('_p','_m'):
        newargs = [a for a in args]
        newargs[0]=title+q
        color = ROOT.kBlue if q == '_p' else ROOT.kRed
        kwargs['color']=color
        Histo(*newargs,**kwargs)
def MergeHistoQ():
    """ Makes a joint histogram out of p and m histograms """
    for label in _qhistos:
        try:
            h1 = h[label+'_m'].Clone(label)
            h1.SetTitle(label)
            h1.Add(h[label+'_p'])
            Histo(label,histo=h1,color=ROOT.kBlack)
        except KeyError:
            continue

def Histo(title,bins=None,bins2=None,bins3=None,histo=None,color=None):
    """ Register a histogram in the global histogram dictionary """
    if histo:
        h[title]=histo
        h[title].SetName(title)
    elif bins3:
        h[title] = ROOT.TH3F("%s" % title,title,bins[0],bins[1],bins[2],bins2[0],bins2[1],bins2[2],bins3[0],bins3[1],bins3[2])
        h[title].GetXaxis().SetTitle(bins[3])
        h[title].GetYaxis().SetTitle(bins2[3])
        h[title].GetZaxis().SetTitle(bins3[3])
    elif bins2:
        h[title] = ROOT.TH2F("%s" % title,title,bins[0],bins[1],bins[2],bins2[0],bins2[1],bins2[2])
        h[title].GetXaxis().SetTitle(bins[3])
        h[title].GetYaxis().SetTitle(bins2[3])
    else:
        assert bins!=None,'You need to provide at least one binning argument to Histo()'
        h[title] = ROOT.TH1F("%s" % title,title,bins[0],bins[1],bins[2])
        h[title].GetXaxis().SetTitle(bins[3])
        h[title].SetLineWidth(2)
    if color:
        h[title].SetLineColor(color)
    if h[title].GetSumw2N()==0:
        h[title].Sumw2()

def addDcachePrefix(p):
    """If path starts with /pnfs, prepend dcap:// to it"""
    if re.match('/pnfs',p):
        return 'dcap://'+p
    else:
        return p
    
def pairs(lst):
    """ easy loop over all unique pairs of objects """
    n = len(lst)
    for i in range(n):
        for j in range(0,i):
            yield lst[i],lst[j]

def closest(target,cands):
    """ transverses the list to find the member closest to target """
    return min(  (abs(target - cand[1]), cand[1], j) for j,cand in enumerate(cands)  )

def closest2(target,cands):
    """ transverses the list to find the member closest to target """
    return min(  (abs(target - cand), cand, j) for j,cand in enumerate(cands)  )

class EventFlow:
    """ trivial event flow manager """
    def __init__(s):
        s.ok=0
        s.truthmet=0
        s.truthmu=0
        s.truthwmt=0
        s.trigger=0
        s.grl=0
        s.bcid=0
        s.jetclean=0
        s.vertex=0
        s.muonpresel=0
        s.muonqual=0
        s.muonpt=0
        s.muoniso=0
        s.met=0
        s.zcut=0
        s.metmuphi=0
        s.wmt=0
    def Print(s,fout):
        o=[s.truthmet,s.truthmu,s.truthwmt,s.grl,s.bcid,s.jetclean,s.trigger,s.vertex,s.muonpresel,s.muonqual,s.muonpt,s.muoniso,s.met,s.zcut,s.wmt,s.ok]
        print >>fout,o
        def cut(o,i):
            try:
                return '%d \t %.2f%%'%(sum(o)-sum(o[:i]),100.0*(sum(o)-sum(o[:i]))/sum(o))
            except ZeroDivisionError:
                return 'ZERO DIVISION ERROR'
        i=0
        print >>fout, 'Total events         :',cut(o,i); i+=1
        print >>fout, 'After true-met cuts  :',cut(o,i); i+=1        
        print >>fout, 'After true-mu cuts   :',cut(o,i); i+=1
        print >>fout, 'After true-w mT cuts :',cut(o,i); i+=1
        print >>fout, 'After applying GRL   :',cut(o,i); i+=1
        print >>fout, 'After bcid!=0 cut    :',cut(o,i); i+=1
        print >>fout, 'After jet cleaning   :',cut(o,i); i+=1
        print >>fout, 'After trigger        :',cut(o,i); i+=1
        print >>fout, 'After primary vtx    :',cut(o,i); i+=1
        print >>fout, '-------------------------------'
        print >>fout, 'After muon presel    :',cut(o,i); i+=1
        print >>fout, 'After muon quality   :',cut(o,i); i+=1
        print >>fout, 'After muon pt>20     :',cut(o,i); i+=1
        print >>fout, 'After muon iso       :',cut(o,i); i+=1
        print >>fout, 'After MET cuts       :',cut(o,i); i+=1
        print >>fout, 'After z bg cut       :',cut(o,i); i+=1
        print >>fout, 'After mT[W] >40      :',cut(o,i); i+=1
        fout.close()

def makeCanvas(label):
    c = ROOT.TCanvas(label,label,1024,768);
    c.cd();
    #ROOT.gPad.SetGridx();  ROOT.gPad.SetGridy();
    return c

def PlotOverlayHistos(h1in,h2in,label,fname,norm=None,plotdir='plots'):
    """ Plots two histograms on top of each other (option=normalized)"""
    if not os.path.isdir(plotdir):
        os.makedirs(plotdir)
    h1=h1in.Clone(h1in.GetName()+"ol")
    h2=h2in.Clone(h2in.GetName()+"ol")
    h1.df,h2.df=h1.Draw,h2.Draw
    if norm:
        h1.df,h2.df=h1.DrawNormalized,h2.DrawNormalized
    c = makeCanvas(label)
    h1.df("H")
    h2.df("HSAME")
    h1.GetYaxis().SetRangeUser(0.0,h1.GetMaximum()*1.2);
    c.SaveAs('%s/%s.%s'%(plotdir,fname,_ext))

def savePlot(o,plotdir='plots'):
    if not os.path.isdir(plotdir):
        os.makedirs(plotdir)
    if o.GetEntries()==0:
        return
    title = o.GetTitle()
    c = makeCanvas(title)
    if re.match("TH1",o.ClassName()):
        o.Draw("H")
    else:
        o.Draw()
    if re.match("TH1",o.ClassName()):
        o.GetYaxis().SetRangeUser(0.0,o.GetMaximum()*1.2);
    c.SaveAs('%s/%s.%s'%(plotdir,title,_ext))
