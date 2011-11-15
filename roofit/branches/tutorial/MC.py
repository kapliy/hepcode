#!/usr/bin/env python
import sys,os,re
import ROOT

seen_samples = []
def ScaleToLumi(h,name,lumi,qcdscale,nevts=None):
    mrun = mc.match_sample(name)
    if mrun:
        xsec = mrun.xsec*mrun.filteff
        nevents = nevts if nevts else mrun.nevents
        sample = mrun.sample
        if sample not in seen_samples:
            print 'MC %s: \t xsec=%.1f (%.1f*%.1f) nb \t nevts=%d/%d'%(sample,xsec,mrun.xsec,mrun.filteff,nevents,mrun.nevents)
            seen_samples.append(sample)
        h.Sumw2()
        h.Scale(1.0/nevents*lumi*xsec)
        if qcdscale!='AUTO' and sample in ['mc_J%d'%z for z in range(10)]:
            h.Scale(float(qcdscale))
        if qcdscale!='AUTO' and sample in ['mc_bbmu15x','mc_ccmu15x']:
            h.Scale(float(qcdscale))
    else:
        print 'WARNING: unable to find sample definiction for',name
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
    def Remove(s,name):
        """ removes a particular plot from the stack """
        mcgs = s.mcg[:]
        for i,mcg in enumerate(mcgs):
            if name in mcg:
                idxs = [z for z in range(len(s.mcg[i])) if name==s.mcg[i][z]]
                for idx in idxs:
                    del s.mcg[i][idx]
                if len(s.mcg[i])==0:
                    del s.mcg[i]
                    del s.mcgc[i]
                    del s.mcg_name[i]
        
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

class MC_samples:
    def __init__(s):
        s.runs = []
    def append(s,run):
        run.dataset_pattern='mc_7TeV.%d.%s.merge.AOD.%s/'
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
        runs = [a for a in s.runs if str(a.sample)==str(path)]
        #runs = [a for a in s.runs if re.search(str(a.sample),str(path))]
        if len(runs)==1:
            return runs[0]
        assert False,'Failed to find sample %s'%path
        return None
    def get_xsec(s,path):
        return s.match_run(path).xsec
    def get_nevents(s,path):
        return s.match_run(path).nevents
    def get_sample(s,path):
        return s.match_run(path).sample

mc = MC_samples()

#from http://cdsweb.cern.ch/record/1282370/files/ATL-CAL-PROC-2010-001.pdf?version=3
#from http://cdsweb.cern.ch/record/1298803/files/ATL-COM-PHYS-2010-836.pdf

mc.append(MCR(106044,'mc_wmunu','*',8894.06*1.17/1000,1.000000,6993798))
mc.append(MCR(106022,'mc_wtaunu','*',8916.33*1.17*3.068E-01/1000,1.0,999874)) #actual xs for this is 8.916E3*0.35 (tau is forced to decay into e/mu)
mc.append(MCR(106047,'mc_zmumu','*',851.011*1.15/1000,1.000000,4998410))
mc.append(MCR(106047,'mc_zmumua','*',851.011/2.0*1.15/1000,1.000000,4998410))
mc.append(MCR(106047,'mc_zmumub','*',851.011/2.0*1.15/1000,1.000000,4998410))
mc.append(MCR(106052,'mc_ztautau','*',989.0/1000,1.000000,1998598))
mc.append(MCR(105861,'mc_ttbar','*',1.4580E-1,0.54301,199838))  # NOTE: this is not the sample used by others
# these are likely incorrect:
mc.append(MCR(109276,'mc_J0','*',9752970.000000,0.000079,500000))
mc.append(MCR(109277,'mc_J1','*',673020.000000,0.001233,500000))
mc.append(MCR(109278,'mc_J2','*',41194.700000,0.005443,500000))
mc.append(MCR(109279,'mc_J3','*',2193.250000,0.012949,500000))
mc.append(MCR(109280,'mc_J4','*',87.848700,0.022156,500000))
mc.append(MCR(109281,'mc_J5','*',2.328560,0.029753,500000))
# approximate (from peter):
mc.append(MCR(108405,'mc_bbmu15x','*',7.39E+04/1000,1.0,500000)) 
mc.append(MCR(106059,'mc_ccmu15x','*',2.84E+04/1000,1.0,500000))

# MC@NLO samples (must be wrong)
mc.append(MCR(106083,'mc_wminmunu','*',3.9607,1.000000,6993798))
mc.append(MCR(106084,'mc_wplusmunu','*',5.8414,1.000000,6993798))

# W/Z + jets samples:
# From Jordan's SuPlot
#ttbar
mc.append(MCR(105200,'mc_jimmy_ttbar','*',0.16457,0.5551,500000))
#s-top
mc.append(MCR(117361,'mc_acer_tchan_munu','*',0.00693,1.0,500000))
mc.append(MCR(117362,'mc_acer_tchan_taunu','*',0.00693,1.0,500000))
mc.append(MCR(117364,'mc_acer_schan_munu','*',0.0003558,1.0,500000))
mc.append(MCR(117365,'mc_acer_schan_taunu','*',0.0003555,1.0,500000))
mc.append(MCR(105500,'mc_acer_wt','*',0.0156,1.0,500000))
#wmunu
mc.append(MCR(107690,'mc_jimmy_wmunu_np0','*',6.91960,1.2,500000))
mc.append(MCR(107691,'mc_jimmy_wmunu_np1','*',1.30420,1.2,500000))
mc.append(MCR(107692,'mc_jimmy_wmunu_np2','*',0.37783,1.2,500000))
mc.append(MCR(107693,'mc_jimmy_wmunu_np3','*',0.10188,1.2,500000))
mc.append(MCR(107694,'mc_jimmy_wmunu_np4','*',0.02575,1.2,500000))
mc.append(MCR(107695,'mc_jimmy_wmunu_np5','*',0.00692,1.2,500000))
#wtaunu - not updated!
mc.append(MCR(107700,'mc_jimmy_wtaunu_np0','*',8.339700,1.0,500000))
mc.append(MCR(107701,'mc_jimmy_wtaunu_np1','*',1.557700,1.0,500000))
mc.append(MCR(107702,'mc_jimmy_wtaunu_np2','*',0.459500,1.0,500000))
mc.append(MCR(107703,'mc_jimmy_wtaunu_np3','*',0.123000,1.0,500000))
mc.append(MCR(107704,'mc_jimmy_wtaunu_np4','*',0.031400,1.0,500000))
mc.append(MCR(107705,'mc_jimmy_wtaunu_np5','*',0.008500,1.0,500000))
#zmumu
mc.append(MCR(107660,'mc_jimmy_zmumu_np0','*',0.66868,1.25,500000))
mc.append(MCR(107661,'mc_jimmy_zmumu_np1','*',0.13414,1.25,500000))
mc.append(MCR(107662,'mc_jimmy_zmumu_np2','*',0.04033,1.25,500000))
mc.append(MCR(107663,'mc_jimmy_zmumu_np3','*',0.01119,1.25,500000))
mc.append(MCR(107664,'mc_jimmy_zmumu_np4','*',0.00275,1.25,500000))
mc.append(MCR(107665,'mc_jimmy_zmumu_np5','*',0.00077,1.25,500000))
#ztautau - not updated!
mc.append(MCR(107670,'mc_jimmy_ztautau_np0','*',0.802028,1.0,500000))
mc.append(MCR(107671,'mc_jimmy_ztautau_np1','*',0.162260,1.0,500000))
mc.append(MCR(107672,'mc_jimmy_ztautau_np2','*',0.049288,1.0,500000))
mc.append(MCR(107673,'mc_jimmy_ztautau_np3','*',0.013420,1.0,500000))
mc.append(MCR(107674,'mc_jimmy_ztautau_np4','*',0.003538,1.0,500000))
mc.append(MCR(107675,'mc_jimmy_ztautau_np5','*',0.000854,1.0,500000))
#WW - not updated!
mc.append(MCR(107100,'mc_jimmy_ww_np0','*',0.002095,1.0,500000))
mc.append(MCR(107101,'mc_jimmy_ww_np1','*',0.00099623,1.0,500000))
mc.append(MCR(107102,'mc_jimmy_ww_np2','*',0.0004547,1.0,500000))
mc.append(MCR(107103,'mc_jimmy_ww_np3','*',0.0001581,1.0,500000))
#WZ - not updated!
mc.append(MCR(107104,'mc_jimmy_wz_np0','*',0.00067183,1.0,500000))
mc.append(MCR(107105,'mc_jimmy_wz_np1','*',0.00041376,1.0,500000))
mc.append(MCR(107106,'mc_jimmy_wz_np2','*',0.00022493,1.0,500000))
mc.append(MCR(107107,'mc_jimmy_wz_np3','*',0.000094977,1.0,500000))
#ZZ - not updated!
mc.append(MCR(107108,'mc_jimmy_zz_np0','*',0.00050861,1.0,500000))
mc.append(MCR(107109,'mc_jimmy_zz_np1','*',0.00023417,1.0,500000))
mc.append(MCR(107110,'mc_jimmy_zz_np2','*',0.000088623,1.0,500000))
mc.append(MCR(107111,'mc_jimmy_zz_np3','*',0.000031388,1.0,500000))

if False:
    print 'Registered',mc.nruns(),'runs:'
    print 'nevents   =',mc.nevents()
    #[r.pathena_submit() for r in mc.runs]
    #for r in mc.runs: print r.path()

def MakeLegend(label='Data and MC'):
    l = ROOT.TLegend(0.55,0.70,0.88,0.88,label,"brNDC")
    l.SetFillStyle(1001)
    l.SetFillColor(10)
    return l

def is_wmunu(iname):
    return any( [re.search(v,iname) for v in ('wmunu','wminmunu','wplusmunu')] )

def is_qcd(iname):
    return any( [re.search(v,iname) for v in ('mc_bbmu15x','mc_ccmu15x','mc_J')] )

def WSum(hplus,hminus,hname):
    """ Just a sum of two histograms """
    h = hplus.Clone(hname)
    h.Add(hminus)
    return h

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

def canvas():
    c = ROOT.TCanvas("c1","c1",200,10,800,500)
    c.cd()
    return c
