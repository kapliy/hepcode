#!/usr/bin/env python
import sys,os,re
import ROOT

POS=0
NEG=1

seen_samples = []
def ScaleToLumi(h,name,lumi,qcdscale,nevts=None):
    mrun = mc09.match_sample(name)
    if mrun:
        xsec = mrun.xsec*mrun.filteff
        nevents = nevts if nevts else mrun.nevents
        sample = mrun.sample
        if sample not in seen_samples:
            print 'MC %s: \t xsec=%.1f nb \t nevts=%d/%d'%(sample,xsec,nevents,mrun.nevents)
            seen_samples.append(sample)
        h.Scale(1.0/nevents*lumi*xsec)
        if qcdscale!='AUTO' and sample in ['mc_J%d'%z for z in range(10)]:
            h.Scale(qcdscale)
    return h

def xflatten(seq):
    """a generator to flatten a nested list"""
    for x in seq:
        if type(x) is list:
            for y in xflatten(x):
                yield y
        else:
            yield x

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

class MCR:
    """ dataset """
    def __init__(s,rnum,sample,tag,xsec,filteff,nevents,ntupleDS=None):
        s.rnum = rnum
        s.sample = sample
        s.tag = tag
        s.nevents = nevents
        s.filteff = filteff
        s.xsec = xsec   # in nanobarns   # /1000.0 # convert nanobarns to microbarns
        s.ntupleDS=ntupleDS
    def dq2_ls(s):
        print 'Trying to find',s.rnum
        os.system('dq2-ls -L ROAMING '+s.dataset_pattern%(s.rnum,s.sample,s.tag))
    def pathena_submit(s):
        print './grid_submit '+s.dataset_pattern%(s.rnum,s.sample,s.tag)+' --nEventsPerJob 30000'
    def path(s):
        return '/pnfs/uct3/data/users/antonk/ANALYSIS/ICHEP_MC/'+s.ntupleDS+'/*root*'

class MC09_samples:
    def __init__(s):
        s.runs = []
    def append(s,run):
        run.dataset_pattern='mc09_7TeV.%d.%s.merge.AOD.%s/'
        run.ntupleDS='user.ponyisi.'+(run.dataset_pattern.strip('/'))%(run.rnum,run.sample,run.tag)+'.ntuple.v1_11'
        s.runs.append(run)
    def nruns(s):
        return len(s.runs)
    def nevents(s):
        return sum([r.nevents for r in s.runs])
    def match_run(s,path):
        runs = [a for a in s.runs if re.search('%s'%a.rnum,str(path))]
        if len(runs)==1:
            return runs[0]
        return None
    def match_sample(s,path):
        runs = [a for a in s.runs if re.search(str(a.sample),str(path))]
        if len(runs)==1:
            return runs[0]
        return None
    def get_xsec(s,path):
        return s.match_run(path).xsec
    def get_nevents(s,path):
        return s.match_run(path).nevents
    def get_sample(s,path):
        return s.match_run(path).sample

mc09 = MC09_samples()
mc09.append(MCR(106044,'mc_wmunu','*',10.461,1.000000,6993798))
mc09.append(MCR(106022,'mc_wtaunu','*',10.461,0.873,999874))
mc09.append(MCR(106047,'mc_zmumu','*',0.989,1.000000,4998410))
mc09.append(MCR(106052,'mc_ztautau','*',0.989,1.000000,1998598))
mc09.append(MCR(105861,'mc_ttbar','*',0.165,0.5426,199838))
mc09.append(MCR(109276,'mc_J0','*',9.8605E+06,8.5572E-05,500000))
mc09.append(MCR(109277,'mc_J1','*',6.7804E+05,1.2111E-03,500000))
mc09.append(MCR(109278,'mc_J2','*',4.0968E+04,5.4310E-03,500000))
mc09.append(MCR(109279,'mc_J3','*',2.1929E+03,1.3051E-02,500000))
mc09.append(MCR(109280,'mc_J4','*',8.7665E+01,2.2357E-02,500000))
mc09.append(MCR(109281,'mc_J5','*',2.3488E+00,3.0230E-02,500000))

if False: # original from twiki
    mc09.append(MCR(106044,'mc_wmunu','*',8.894060,1.000000,6993798))
    mc09.append(MCR(106022,'mc_wtaunu','*',8.916330,0.876600,999874))
    mc09.append(MCR(106047,'mc_zmumu','*',0.851011,1.000000,4998410))
    mc09.append(MCR(106052,'mc_ztautau','*',0.854173,1.000000,1998598))
    mc09.append(MCR(105861,'mc_ttbar','*',0.145642,0.538200,199838))
    mc09.append(MCR(109276,'mc_J0','*',9752970.000000,0.000079,500000))
    mc09.append(MCR(109277,'mc_J1','*',673020.000000,0.001233,500000))
    mc09.append(MCR(109278,'mc_J2','*',41194.700000,0.005443,500000))
    mc09.append(MCR(109279,'mc_J3','*',2193.250000,0.012949,500000))
    mc09.append(MCR(109280,'mc_J4','*',87.848700,0.022156,500000))
    mc09.append(MCR(109281,'mc_J5','*',2.328560,0.029753,500000))

if False:
    print 'Registered',mc09.nruns(),'runs:'
    print 'nevents   =',mc09.nevents()
    #[r.pathena_submit() for r in mc09.runs]
    #for r in mc09.runs: print r.path()

def SetStyle(styleMacro=''):
    """ Global style settings for ROOT """
    if styleMacro=='':
        ROOT.gStyle.SetCanvasColor(0);
        ROOT.gStyle.SetCanvasBorderMode(0);
        ROOT.gStyle.SetPadColor(0);
        ROOT.gStyle.SetPadBorderMode(0);
        ROOT.gStyle.SetStatColor(0);
        
        #ROOT.gStyle.SetOptStat(111111);
        ROOT.gStyle.SetOptStat(0);
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
        ROOT.SetAtlasStyle()

def WRatio(hplus,hminus,title,label='ratio'):
    """ Ratio of W+ to W- workhorse function
    Alternatively, can be used to calculate efficiency
    """
    hratio = hplus.Clone(hplus.GetName()+"_"+label)
    hratio.Divide(hminus)
    hratio.SetTitle(title)
    return hratio

def WAsymmetry(hplus,hminus,title='asymmetry'):
    """ (W+ - W-)/(W+ + W-) workhorse function """
    hsum = hplus.Clone(hplus.GetName()+" asymmetry")
    hsum.Add(hminus,-1.0)
    hdiff = hplus.Clone(hplus.GetName()+" asymmetry diff")
    hdiff.Add(hminus)
    hsum.Divide(hdiff)
    hsum.SetTitle(title)
    return hsum

def canvas():
    c = ROOT.TCanvas("c1","c1",200,10,800,500)
    c.cd()
    return c

