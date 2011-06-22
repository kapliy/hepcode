#!/usr/bin/env python
import sys,os,re
import ROOT

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

mc09 = MC09_samples()

if False: # from Peter
    mc09.append(MCR(106044,'mc_wmunu','*',10.461,1.000000,6993798))
    mc09.append(MCR(106022,'mc_wtaunu','*',8916.33*1.17/1000,0.87867,999874))
    mc09.append(MCR(106047,'mc_zmumu','*',851.011*1.15/1000,1.000000,4998410))
    mc09.append(MCR(106052,'mc_ztautau','*',856.967*1.15/1000,1.000000,1998598))
    mc09.append(MCR(105861,'mc_ttbar','*',0.1646,5.4258E-01,199838))
    # these are incorrect:
    mc09.append(MCR(113204,'mc_J0','*',9.6139E+06/1000,1.0,500000))
    mc09.append(MCR(113205,'mc_J1','*',7.4366E+05/1000,1.0,500000))
    mc09.append(MCR(113206,'mc_J2','*',4.4307E+04/1000,1.0,500000))
    mc09.append(MCR(113207,'mc_J3','*',2.3576E+03/1000,1.0,500000))
    mc09.append(MCR(113208,'mc_J4','*',9.4236E+01/1000,1.0,500000))
    mc09.append(MCR(113209,'mc_J5','*',2.5813E+00/1000,1.0,500000))
    mc09.append(MCR(113210,'mc_J6','*',3.9439E-02/1000,1.0,500000))
    #from here: http://cdsweb.cern.ch/record/1282370/files/ATL-CAL-PROC-2010-001.pdf?version=3
    mc09.append(MCR(108405,'mc_bbmu15x','*',7.39E+04/1000,1.0,500000)) 
    mc09.append(MCR(106059,'mc_ccmu15x','*',2.84E+04/1000,1.0,500000))

if True: # before peter update
    mc09.append(MCR(106044,'mc_wmunu','*',8894.06*1.17/1000,1.000000,6993798))
    mc09.append(MCR(106022,'mc_wtaunu','*',8916.33*1.17/1000,0.87867,999874))
    mc09.append(MCR(106047,'mc_zmumu','*',851.011*1.15/1000,1.000000,4998410))
    mc09.append(MCR(106052,'mc_ztautau','*',856.967*1.15/1000,1.000000,1998598))
    mc09.append(MCR(105861,'mc_ttbar','*',1.4580E-1,0.54301,199838))
    # these are incorrect:
    mc09.append(MCR(109276,'mc_J0','*',9752970.000000,0.000079,500000))
    mc09.append(MCR(109277,'mc_J1','*',673020.000000,0.001233,500000))
    mc09.append(MCR(109278,'mc_J2','*',41194.700000,0.005443,500000))
    mc09.append(MCR(109279,'mc_J3','*',2193.250000,0.012949,500000))
    mc09.append(MCR(109280,'mc_J4','*',87.848700,0.022156,500000))
    mc09.append(MCR(109281,'mc_J5','*',2.328560,0.029753,500000))
    mc09.append(MCR(108405,'mc_bbmu15x','*',7.39E+04/1000,1.0,500000)) 
    mc09.append(MCR(106059,'mc_ccmu15x','*',2.84E+04/1000,1.0,500000))

# MC@NLO samples
mc09.append(MCR(106083,'mc_wminmunu','*',3.9607,1.000000,6993798))
mc09.append(MCR(106084,'mc_wplusmunu','*',5.8414,1.000000,6993798))

# W/Z + jets samples:
# http://cdsweb.cern.ch/record/1298803/files/ATL-COM-PHYS-2010-836.pdf
mc09.append(MCR(105200,'mc_jimmy_ttbar','*',80.201/1000,1.11,500000))
#wmunu
mc09.append(MCR(107690,'mc_jimmy_wmunu_np0','*',6935.4/1000,1.22,500000))
mc09.append(MCR(107691,'mc_jimmy_wmunu_np1','*',1281.2/1000,1.22,500000))
mc09.append(MCR(107692,'mc_jimmy_wmunu_np2','*',375.3/1000,1.22,500000))
mc09.append(MCR(107693,'mc_jimmy_wmunu_np3','*',101.1/1000,1.22,500000))
mc09.append(MCR(107694,'mc_jimmy_wmunu_np4','*',25.7/1000,1.22,500000))
mc09.append(MCR(107695,'mc_jimmy_wmunu_np5','*',7.0/1000,1.22,500000))
#wtaunu
mc09.append(MCR(107700,'mc_jimmy_wtaunu_np0','*',6835.8/1000,1.22,500000))
mc09.append(MCR(107701,'mc_jimmy_wtaunu_np1','*',1276.8/1000,1.22,500000))
mc09.append(MCR(107702,'mc_jimmy_wtaunu_np2','*',376.6/1000,1.22,500000))
mc09.append(MCR(107703,'mc_jimmy_wtaunu_np3','*',100.8/1000,1.22,500000))
mc09.append(MCR(107704,'mc_jimmy_wtaunu_np4','*',25.7/1000,1.22,500000))
mc09.append(MCR(107705,'mc_jimmy_wtaunu_np5','*',7.0/1000,1.22,500000))
#zmumu
mc09.append(MCR(107660,'mc_jimmy_zmumu_np0','*',657.7/1000,1.22,500000))
mc09.append(MCR(107661,'mc_jimmy_zmumu_np1','*',132.8/1000,1.22,500000))
mc09.append(MCR(107662,'mc_jimmy_zmumu_np2','*',39.6/1000,1.22,500000))
mc09.append(MCR(107663,'mc_jimmy_zmumu_np3','*',11.1/1000,1.22,500000))
mc09.append(MCR(107664,'mc_jimmy_zmumu_np4','*',2.8/1000,1.22,500000))
mc09.append(MCR(107665,'mc_jimmy_zmumu_np5','*',0.8/1000,1.22,500000))
#ztautau
mc09.append(MCR(107670,'mc_jimmy_ztautau_np0','*',657.4/1000,1.22,500000))
mc09.append(MCR(107671,'mc_jimmy_ztautau_np1','*',133.0/1000,1.22,500000))
mc09.append(MCR(107672,'mc_jimmy_ztautau_np2','*',40.4/1000,1.22,500000))
mc09.append(MCR(107673,'mc_jimmy_ztautau_np3','*',11.0/1000,1.22,500000))
mc09.append(MCR(107674,'mc_jimmy_ztautau_np4','*',2.9/1000,1.22,500000))
mc09.append(MCR(107675,'mc_jimmy_ztautau_np5','*',0.7/1000,1.22,500000))
#WW
mc09.append(MCR(107100,'mc_jimmy_ww_np0','*',2.049/1000,1.21,500000))
mc09.append(MCR(107101,'mc_jimmy_ww_np1','*',0.987/1000,1.21,500000))
mc09.append(MCR(107102,'mc_jimmy_ww_np2','*',0.441/1000,1.21,500000))
mc09.append(MCR(107103,'mc_jimmy_ww_np3','*',0.178/1000,1.21,500000))
#WZ
mc09.append(MCR(107104,'mc_jimmy_wz_np0','*',0.664/1000,1.21,500000))
mc09.append(MCR(107105,'mc_jimmy_wz_np1','*',0.399/1000,1.21,500000))
mc09.append(MCR(107106,'mc_jimmy_wz_np2','*',0.221/1000,1.21,500000))
mc09.append(MCR(107107,'mc_jimmy_wz_np3','*',0.093/1000,1.21,500000))
#ZZ
mc09.append(MCR(107108,'mc_jimmy_zz_np0','*',0.494/1000,1.21,500000))
mc09.append(MCR(107109,'mc_jimmy_zz_np1','*',0.225/1000,1.21,500000))
mc09.append(MCR(107110,'mc_jimmy_zz_np2','*',0.088/1000,1.21,500000))
mc09.append(MCR(107111,'mc_jimmy_zz_np3','*',0.028/1000,1.21,500000))


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

def canvas():
    c = ROOT.TCanvas("c1","c1",200,10,800,500)
    c.cd()
    return c
