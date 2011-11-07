#!/usr/bin/env python

import PyCintex;
PyCintex.Cintex.Enable()

import math,re,os,glob,time,sys,array,copy
from math import sqrt,fabs,cos,sin
import ROOT
ROOT.SetSignalPolicy(ROOT.kSignalFast)  # speed-up
ROOT.gErrorIgnoreLevel = 2999           # get rid of verbose crap
ROOT.gROOT.ProcessLine(
    "struct CandStruct {\
    Int_t run;\
    Int_t event;\
    Int_t lb;\
    Int_t charge;\
    Float_t mu_pt;\
    Float_t mu_eta;\
    Float_t mu_phi;\
    Float_t mu_ptcone40;\
    Float_t met;\
    Float_t w_mt;\
    Float_t w_pt;\
    };" );

# constants
wMASS=80.4
zMASS=91.19
muMASS=0.1056584
GeV=1
mm=1

# mass window for W id via transverse mass:
wMIN=40*GeV
wMAX=1000*GeV   # only require mT>40 GeV
zMIN=60*GeV
zMAX=120*GeV

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
# variable binning
_etavarbins = (-2.4,-1.52,-1.37,-1.05,0.0,1.05,1.37,1.52,2.4)
etavarbins = (len(_etavarbins)-1,array.array('f',_etavarbins),None, 'Eta')

chargemap = {'p' : 1, 'm' : -1}

def findBin(bins,val):
    """ finds which bin a value belongs to """
    binw=(asbins[2]-asbins[1])/asbins[0]
    for i in range(0,bins[0]+1):
        lb=bins[1]+i*binw
        if val>=lb and val<lb+binw:
            break
    return i

def xflatten(seq):
    """a generator to flatten a nested list"""
    for x in seq:
        if type(x) is list:
            for y in xflatten(x):
                yield y
        else:
            yield x

_WEIGHT = 1
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
    #libnames = ['/home/antonk/setup/15.5.1/GoodRunsLists/i686-slc4-gcc34-opt/libGoodRunsListsLib.so']
    libnames = ['./libs/i686-slc4-gcc34-opt/libGoodRunsListsLib.so','./libs/i686-slc5-gcc43-opt/libGoodRunsListsLib.so', './libs/i686-slc5-gcc43-opt2/libGoodRunsListsLib.so']
    libnames = [os.path.join(sys.path[0],v) for v in libnames ]
    def __init__(s,xmls=None):
        for i,libname in enumerate(s.libnames):
            try:
                st = ROOT.gSystem.Load(libname)
                print ROOT.Root
            except:
                st = -1
            if st==-1:
                print 'GRL: failed to load GRL library from:',libname
                if i==len(s.libnames):
                    print 'Exhausted all options; exiting...'
                    sys.exit(0)
            else:
                print 'GRL: loaded',libname
                break
        if xmls:
            s.load(xmls)
    def load(s,xmls):
        """ Accepts comma-separated list of xml files """
        s.xmls = xmls
        s.reader = ROOT.Root.TGoodRunsListReader()
        s.nfiles = 0
        for xml in s.xmls.split(','):
            for file in glob.glob(xml):
                print 'GRL: adding',file
                s.reader.AddXMLFile(file)
                s.nfiles+=1
        if s.nfiles==0:
            print "GRL: didn't find any grl files matching requested pattern:",xmls
            sys.exit(0)
        s.reader.Interpret()
        #s.grl = s.reader.GetMergedGoodRunsList()
        s.grl = s.reader.GetMergedGRLCollection()
    def save(s,fname='merged_grl.xml'):
        """ Saves a merged GRL file """
        if not s.grl:
            print 'Warning: no GRLs have been loaded yet. Skipping save...'
            return
        writer = ROOT.Root.TGoodRunsListWriter()
        writer.SetGRLCollection(s.grl)
        writer.SetFilename(fname)
        writer.WriteXMLFile()
    def summary(s):
        s.grl.Summary()
    def passRunLB__real(s,run,lb):
        return s.grl.HasRunLumiBlock(run,lb)
    @staticmethod
    def passRunLB__dummy(*args,**kwargs):
        return 1

class PlotOrder:
    """ A class that specifies stack ordering and colors """
    def __init__(s):
        s.mcg_name = []
        s.mcg = []
        s.mcgc = []
    def Check(s):
        assert len(s.mcg)==len(s.mcg_name)==len(s.mcgc), 'PlotOrder internal error: wrong list sizes'
    def Add(s,name,samples,color):
        s.mcg_name.append(name)
        if isinstance(samples,list):
            s.mcg.append(samples)
        else:
            s.mcg.append([samples])
        s.mcgc.append(color)

def SetTreeBranches_V27(t,doTruth=True,doReco=False):
    """ Sets branches for v11 peter ntuple """
    t.SetBranchStatus("*", 0)
    br = []
    br.append(['run','event'])
    if doReco:
        br.append(['lb','bcid'])
        br.append(['njet','jet_n90','jet_quality','jet_time','jet_emf','jet_hecf','jet_pt_em'])
        br.append(['trig_l1','trig_ef'])
        br.append(['nvx','vx_type','vx_ntracks','vx_sumpt','vx_z'])
        br.append(['met_reffinal','met_reffinal_phi'])
        br.append(['nmu','mu_author','mu_class','mu_q','mu_pt','mu_eta','mu_phi','mu_ptcone40','mu_ptms','mu_ptexms','mu_qms','mu_ptid','mu_qid','mu_z0'])
    if doTruth:
        br.append(['nmc','mc_status','mc_pdgid','mc_e','mc_pt','mc_eta','mc_phi','mc_parent'])
        br.append(['met_truth','met_truth_phi'])
    [t.SetBranchStatus(v,1) for v in xflatten(br)]

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

def Fill(title,*args):
    """ Fill per-charge histograms """
    #h[title].Fill(*args,_WEIGHT)
    h[title].Fill(*args)
def FillQ(title,q,*args):
    """ Fill per-charge histograms """
    h['%s_%s'%(title,q)].Fill(*args)
    #h['%s_%s'%(title,q)].Fill(*args,_WEIGHT)

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
        if isinstance(bins[1],array.array):  # variable bins
            h[title] = ROOT.TH1F("%s" % title,title,bins[0],bins[1])
        else:
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
    elif re.match('/atlas',p):
        return 'root://uct3-xrd.mwt2.org//atlas'+p
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
    def __init__(s,cuts,values=None):
        s.cuts = cuts  # cut names
        [setattr(s,cut,0) for cut in s.cuts]
        if values:
            s.ForceValues(values)
    def ForceValues(s,values):
        [setattr(s,cut,values[i]) for i,cut in enumerate(s.cuts)]
    def __add__(s,c):
        """ Overloaded '+' operator to merge two cutflow lists """
        assert s.cuts==c.cuts, 'ERROR: different cut names in EventFlow::<operator>plus'
        out = EventFlow(s.cuts)
        [setattr(out,cut,getattr(s,cut)+getattr(c,cut)) for cut in s.cuts]
        return out
    def CutList(s):
        return s.CutNameList(),s.CutValueList()
    def CutNameList(s):
        """ Returns a list containing cut names """
        cflist = ROOT.TList()
        [cflist.Add(ROOT.TObjString(cut)) for cut in s.cuts]
        return cflist
    def CutValueList(s):
        """ Returns a list containing cut values """
        cflist = ROOT.TList()
        [cflist.Add(ROOT.TObjString(str(getattr(s,cut)))) for cut in s.cuts]
        return cflist
    def Print(s,fout=sys.stdout):
        """ Prints complete event cutflow into file fout
            Note: the function closes fout in the end!
        """
        o=[getattr(s,cut) for cut in s.cuts]
        print >>fout,s.cuts
        print >>fout,o
        def cut_tuple(o,i):
            """ Returns a tuple: nevents,total_efficiency,relative_efficiency """
            try:
                ntotal = sum(o)
                npassed = sum(o)-sum(o[:i])
                npassed_prev = sum(o) - (sum(o[:i-1]) if i>0 else sum(o[:i]))
                return (npassed),100.0*npassed/ntotal,100.0*npassed/npassed_prev
            except ZeroDivisionError:
                print 'WARNING: zero division error'
                return (0,0,0)
        for icut,cut in enumerate((['total']+s.cuts)[:-1]):
            print >>fout,str('After %s:'%cut).ljust(22),
            v1,v2,v3 = cut_tuple(o,icut)
            print >>fout,str('%d'%v1).ljust(8),
            print >>fout,str('%.2f%%'%v2).rjust(9),
            print >>fout,str('%.2f%%'%v3).rjust(9)
        if not fout==sys.stdout:
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


class AnaFile():
    """ Histogram / cutflow holder file (out.root abstraction) """
    def __str__(s):
        return s.path
    def __init__(s,path=None,mode='READ'):
        s.path = path
        if s.path:
            s.file = ROOT.TFile.Open(path,mode)
            s.file.cd()
            try:
                s.file._cands.GetEntries()
            except:
                pass
        s.keys = []
        s.h = {}
        s.mrun = None
        s.xsec = None
        s.nevents = None
        s.sample = None
        # meta and cuts
        s.meta=None
        s.ef = None
        # candidates
        s.MakeCandTree()
    def cd(s):
        """ wrapper around TDirectory::cd() """
        if s.file:
            s.file.cd()
    def MakeCandTree(s,name="_cands"):
        """ Prepares a tree to store final candidate events """
        s.cands = ROOT.TTree(name,'Tree with event candidates')
        s.cand = ROOT.CandStruct()
        s.cands.Branch("tree",s.cand,'run/I:event/I:lb/I:charge/I:mu_pt/F:mu_eta/F:mu_phi/F:mu_ptcone40/F:met/F:w_mt/F:w_pt/F')
    def AddCand(s,**kwargs):
        """ Adds one candidate to ouput tree """
        for key,value in kwargs.iteritems():
            setattr(s.cand,key,value)
        s.cands.Fill()
    def SaveCandTree(s):
        s.cands.Write()
    def SaveHistos(s,h):
        """ Saves all histograms into out.root file """
        s.h,s.keys = h,h.keys()
        for key,val in s.h.iteritems():
            val.Write()
    def PrintHistos(s,plotdir):
        """ Saves a png copy of all histograms """
        for hh in s.h.values():
            savePlot(hh,plotdir)
    def SaveCutFlow(s,ef):
        """ serializes cutflow into _cutnames and _cutvalues """
        s.ef = ef
        cutnames,cutvalues = s.ef.CutList()
        cutnames.Write('_cutnames',1)
        cutvalues.Write('_cutvalues',1)
    def SaveMeta(s,**kwargs):
        """ Saves event metadata into a TMap """
        uimap = ROOT.TMap()
        for key,value in kwargs.iteritems():
            uimap.Add(ROOT.TObjString(key),ROOT.TObjString(str(value)))
        uimap.Write('_meta',1)
    def match_list(s,key,whitelist=['*'],blacklist=[]):
        """ Matches key in list of patterns; blacklist patterns_black """
        return not any( [re.search(re.sub('\*','.*',pattern),key) for pattern in blacklist] ) and any( [re.search(re.sub('\*','.*',pattern),key) for pattern in whitelist] )
    def Load(s,whitelist=['*'],blacklist=[]):
        hl=s.file.GetListOfKeys()
        #histos
        for obj in hl:
            key = obj.GetName()
            if key in ('_meta','_cutnames','_cutvalues','_cands'):
                continue
            if not s.match_list(key,whitelist,blacklist):
                continue
            s.keys.append(key)
            hobj = s.file.Get(key)
            s.h[key] = hobj.Clone()
        #metadata
        s.meta = s.file._meta.Clone()
        try:
            s.nevents = int(str(s.file._meta.GetValue('nevents')))
        except:
            print 'WARNING: failed to get nevents from file; will use default values in MC.py'
            s.nevents = None
        # event flow
        s.ef = EventFlow([str(c) for c in s.file._cutnames],[int(str(c)) for c in s.file._cutvalues])
        # candidates
        s.cand = ROOT.CandStruct()
        s.cands = s.file._cands
    def __add__(s,c):
        """ Overloaded '+' operator to merge two files """
        if isinstance(c,int):
            return s
        ana = AnaFile()
        # merge event flows
        ana.ef = s.ef + c.ef
        # update event counters
        ana.nevents = s.nevents + c.nevents
        # merge histograms
        ana.keys = s.keys
        for key in s.keys:
            ana.h[key]=s.h[key].Clone()
        ana.AddHistos(c)
        # merge candidate tree
        ana.MakeCandTree()
        #ana.cands.CopyEntries(s.cands)
        #ana.cands.CopyEntries(c.cands)
        return ana
    def __radd__(self, other):
        return self.__add__(other)
    def ScaleToLumi(s,lumi,qcdscale):
        from MC import mc09
        mrun = s.mrun = mc09.match_run(s.path)
        if mrun:
            xsec = s.xsec = mrun.xsec*mrun.filteff
            if not s.nevents:
                s.nevents = mrun.nevents
            nevents = s.nevents
            sample = s.sample = mrun.sample
            print 'MC %s: \t\t xsec=%.1f nb'%(sample,xsec)
            for hh in s.h.values():
                hh.Scale(1.0/nevents*lumi*xsec)
                if qcdscale!='AUTO' and sample in ['J%d_pythia_jetjet_1muon'%z for z in range(10)]:
                    hh.Scale(qcdscale)
                    pass
        else:
            print 'Failed to look up xsection for:',s.path
    def ChangeColor(s,color):
        [hh.SetLineColor(ROOT.kBlack) for hh in s.h.values()]
        [hh.SetFillColor(color) for hh in s.h.values()]
    def AddHistos(s,file):
        """ Add to itself all histograms from another AnaFile """
        [s.h[key].Add(file.h[key]) for key in s.keys if key in file.h]
    def InitStack(s):
        s.hs = {}
        s.leg = {}
        for key in s.keys:
            s.leg[key] = ROOT.TLegend(0.55,0.70,0.88,0.88,"Data and MC","brNDC")
            s.leg[key].SetFillStyle(1001);
            s.leg[key].SetFillColor(10);
            s.hs[key] = ROOT.THStack(key,key)
    def AddToStack(s,file):
        [s.hs[key].Add(file.h[key]) for key in s.keys if key in file.h]
    def AddToLegend(s,file,name,type='F'):
        [s.leg[key].AddEntry(file.h[key],name,type) for key in s.keys if key in file.h]
    def Draw(s,key,savedir=None,log=False):
        if key in s.h and key in s.hs:
            maximum = max((s.h[key].GetMaximum(),s.hs[key].GetMaximum()))
            # prepare the canvas
            c = ROOT.TCanvas('c'+key,key,800,600)
            c.cd()
            outext='.png'
            # mc
            s.hs[key].Draw("H")
            s.hs[key].SetMinimum(0.1)
            s.hs[key].SetMaximum(maximum*1.5)
            #s.hs[key].GetHistogram().GetYaxis().SetRangeUser(0.0,maximum*1.25)
            #data
            s.h[key].SetMarkerSize(1.0)
            s.h[key].Draw("Lsame")
            s.leg[key].Draw("same")
            # save the plot?
            if savedir:
                if not os.path.exists(savedir):
                    os.makedirs(savedir)
                c.SaveAs(os.path.join(savedir,key+outext))
                if log:
                    ROOT.gPad.SetLogy(ROOT.kTRUE)
                    outext='_log%s'%outext
                    c.SaveAs(os.path.join(savedir,key+outext))
            return c
    def Close(s):
        if s.file.IsOpen():
            s.file.Close()
