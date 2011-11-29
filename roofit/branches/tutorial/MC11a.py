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
#http://www.dspace.cam.ac.uk/bitstream/1810/236979/1/tomthesis.pdf

# THESE ARE NOT UPDATED!
mc.append(MCR(106044,'mc_wmunu','*',8874.06*1.17/1000,1.000000,6993798))
mc.append(MCR(107054,'mc_wtaunu','*',8941.45*1.17/1000,1.0,999874)) # inclusive wtaunu
mc.append(MCR(106022,'mc_wtaunu_1Lepton','*',8916.33*1.17*3.068E-01/1000,1.0,999874)) #actual xs for this is 8.916E3*0.35 (tau is forced to decay into e/mu)
mc.append(MCR(106047,'mc_zmumu','*',851.011*1.17/1000.0,1.000000,4998410))
mc.append(MCR(106052,'mc_ztautau','*',8.56967E+2*1.25/1000.0,1.000000,1998598))

#QCD
# herwig multi-jet (https://savannah.cern.ch/task/?14846, plus request sheet on savannah)
mc.append(MCR(113204,'mc_herwig_J0','*',9.60E06,1.0,500000))
mc.append(MCR(113205,'mc_herwig_J1','*',0.745E06,1.0,500000))
mc.append(MCR(113206,'mc_herwig_J2','*',44.4E03,1.0,500000))
mc.append(MCR(113207,'mc_herwig_J3','*',2.36E03,1.0,500000))
mc.append(MCR(113208,'mc_herwig_J4','*',94.7E00,1.0,500000))
mc.append(MCR(113209,'mc_herwig_J5','*',2.58E00,1.0,500000))
mc.append(MCR(113210,'mc_herwig_J6','*',0.039E00,1.0,500000))
# approximate (from peter):
mc.append(MCR(108405,'mc_bbmu15x','*',7.39E+04/1000,1.0,500000)) 
mc.append(MCR(106059,'mc_ccmu15x','*',2.84E+04/1000,1.0,500000))

# MC@NLO samples (updated from Physics_mc11_7TeV.xls, but not usings its 0.9 efficiency factor)
mc.append(MCR(106083,'mc_wminmunu','*',3.99,1.000000,6993798))
mc.append(MCR(106084,'mc_wplusmunu','*',5.87,1.000000,6993798))
mc.append(MCR(106088,'mc_mcnlo_zmumu','*',0.952,1.000000,4998410))

# W/Z + jets samples:
# From Jordan's SuPlot; also xcheck with Sam and webpage
# https://svnweb.cern.ch/trac/atlasgrp/browser/Physics/StandardModel/ElectroWeak/Analyses/Winter2012/Common/mc11a_p756_info.txt
# https://svnweb.cern.ch/trac/atlasgrp/browser/Physics/StandardModel/ElectroWeak/Analyses/Summer2011/Common/mc10b_p591_info.txt
#ttbar (ATL-COM-PHYS-2011-1522.pdf)
mc.append(MCR(105200,'mc_jimmy_ttbar','*',0.16457,0.5551,500000))
#wmunu (ATL-COM-PHYS-2011-1522.pdf)
mc.append(MCR(107690,'mc_jimmy_wmunu_np0','*',6.91960,1.2,500000))
mc.append(MCR(107691,'mc_jimmy_wmunu_np1','*',1.30420,1.2,500000))
mc.append(MCR(107692,'mc_jimmy_wmunu_np2','*',0.37783,1.2,500000))
mc.append(MCR(107693,'mc_jimmy_wmunu_np3','*',0.10188,1.2,500000))
mc.append(MCR(107694,'mc_jimmy_wmunu_np4','*',0.02575,1.2,500000))
mc.append(MCR(107695,'mc_jimmy_wmunu_np5','*',0.00692,1.2,500000))
#wtaunu (ATL-COM-PHYS-2011-1522.pdf)
mc.append(MCR(107700,'mc_jimmy_wtaunu_np0','*',6918.60*1.2/1000.0,1.0,500000))
mc.append(MCR(107701,'mc_jimmy_wtaunu_np1','*',1303.20*1.2/1000.0,1.0,500000))
mc.append(MCR(107702,'mc_jimmy_wtaunu_np2','*',378.18*1.2/1000.0,1.0,500000))
mc.append(MCR(107703,'mc_jimmy_wtaunu_np3','*',101.51*1.2/1000.0,1.0,500000))
mc.append(MCR(107704,'mc_jimmy_wtaunu_np4','*',25.64*1.2/1000.0,1.0,500000))
mc.append(MCR(107705,'mc_jimmy_wtaunu_np5','*',7.04*1.2/1000.0,1.0,500000))
#zmumu (mc11a_p756_info.txt, ATL-COM-PHYS-2011-1522.pdf)
mc.append(MCR(107660,'mc_jimmy_zmumu_np0','*',0.66868*1.25,1.0,500000))
mc.append(MCR(107661,'mc_jimmy_zmumu_np1','*',0.13414*1.25,1.0,500000))
mc.append(MCR(107662,'mc_jimmy_zmumu_np2','*',0.04033*1.25,1.0,500000))
mc.append(MCR(107663,'mc_jimmy_zmumu_np3','*',0.01119*1.25,1.0,500000))
mc.append(MCR(107664,'mc_jimmy_zmumu_np4','*',0.00275*1.25,1.0,500000))
mc.append(MCR(107665,'mc_jimmy_zmumu_np5','*',0.00077*1.25,1.0,500000))
#ztautau (mc11a_p756_info.txt, ATL-COM-PHYS-2011-1522.pdf)
mc.append(MCR(107670,'mc_jimmy_ztautau_np0','*',6.684E+2*1.25/1000.0,1.0,6600000))
mc.append(MCR(107671,'mc_jimmy_ztautau_np1','*',1.3481E+2*1.25/1000.0,1.0,1300000))
mc.append(MCR(107672,'mc_jimmy_ztautau_np2','*',4.036E+1*1.25/1000.0,1.0,400000))
mc.append(MCR(107673,'mc_jimmy_ztautau_np3','*',1.125E+1*1.25/1000.0,1.0,110000))
mc.append(MCR(107674,'mc_jimmy_ztautau_np4','*',2.79E+0*1.25/1000.0,1.0,30000))
mc.append(MCR(107675,'mc_jimmy_ztautau_np5','*',7.7E-1*1.25/1000.0,1.0,10000))

# out-of-date jimmy diboson samples:
if False:
    #WW
    mc.append(MCR(107100,'mc_jimmy_ww_np0','*',0.002095,1.0,500000))
    mc.append(MCR(107101,'mc_jimmy_ww_np1','*',0.00099623,1.0,500000))
    mc.append(MCR(107102,'mc_jimmy_ww_np2','*',0.0004547,1.0,500000))
    mc.append(MCR(107103,'mc_jimmy_ww_np3','*',0.0001581,1.0,500000))
    #WZ
    mc.append(MCR(107104,'mc_jimmy_wz_np0','*',0.00067183,1.0,500000))
    mc.append(MCR(107105,'mc_jimmy_wz_np1','*',0.00041376,1.0,500000))
    mc.append(MCR(107106,'mc_jimmy_wz_np2','*',0.00022493,1.0,500000))
    mc.append(MCR(107107,'mc_jimmy_wz_np3','*',0.000094977,1.0,500000))
    #ZZ
    mc.append(MCR(107108,'mc_jimmy_zz_np0','*',0.00050861,1.0,500000))
    mc.append(MCR(107109,'mc_jimmy_zz_np1','*',0.00023417,1.0,500000))
    mc.append(MCR(107110,'mc_jimmy_zz_np2','*',0.000088623,1.0,500000))
    mc.append(MCR(107111,'mc_jimmy_zz_np3','*',0.000031388,1.0,500000))

# McAtNLO diboson (mc11a_p756_info.txt, ATL-COM-PHYS-2011-1522.pdf)
mc.append(MCR(105922,'mc_mcnlo_WpWm_enumunu','*',0.50377/1000.0,1.0,200000))
mc.append(MCR(105923,'mc_mcnlo_WpWm_enutaunu','*',0.50377/1000.0,1.0,200000))
mc.append(MCR(105924,'mc_mcnlo_WpWm_munumunu','*',0.50377/1000.0,1.0,200000))
mc.append(MCR(105925,'mc_mcnlo_WpWm_munuenu','*',0.50377/1000.0,1.0,200000))
mc.append(MCR(105926,'mc_mcnlo_WpWm_munutaunu','*',0.50377/1000.0,1.0,200000))
mc.append(MCR(105927,'mc_mcnlo_WpWm_taunutaunu','*',0.50377/1000.0,1.0,200000))
mc.append(MCR(105928,'mc_mcnlo_WpWm_taunuenu','*',0.50377/1000.0,1.0,200000))
mc.append(MCR(105929,'mc_mcnlo_WpWm_taunumunu','*',0.50377/1000.0,1.0,200000))

# Herwig diboson (mc11a_p756_info.txt, except ww)
mc.append(MCR(105985,'mc_herwig_ww','*',29.592*1.52/1000.0,0.38863,250000))
mc.append(MCR(105987,'mc_herwig_wz','*',11.245*1.58/1000.0,0.1924,250000))
mc.append(MCR(105986,'mc_herwig_zz','*',4.596*1.41/1000.0,0.2784,250000))

# single top (mc11a_p756_info.txt)
if False:
    mc.append(MCR(117361,'mc_acer_tchan_munu','*',8.0/1000.0,1.0,200000))
    mc.append(MCR(117362,'mc_acer_tchan_taunu','*',8.0/1000.0,1.0,200000))
    mc.append(MCR(108344,'mc_acer_schan_munu','*',0.468/1000.0,1.0,300000))
    mc.append(MCR(108345,'mc_acer_schan_taunu','*',0.468/1000.0,1.0,300000))
    mc.append(MCR(105500,'mc_acer_wt','*',14.7/1000.0,1.0,200000))
# single top (ATL-COM-PHYS-2011-1522.pdf)
mc.append(MCR(117361,'mc_acer_tchan_munu','*',6.93/1000.0,1.0,200000))
mc.append(MCR(117362,'mc_acer_tchan_taunu','*',6.93/1000.0,1.0,200000))
mc.append(MCR(108344,'mc_acer_schan_munu','*',0.5/1000.0,1.0,300000))
mc.append(MCR(108345,'mc_acer_schan_taunu','*',0.5/1000.0,1.0,300000))
mc.append(MCR(105500,'mc_acer_wt','*',15.6/1000.0,1.0,200000))

if False:
    print 'Registered',mc.nruns(),'runs:'
    print 'nevents   =',mc.nevents()
    #[r.pathena_submit() for r in mc.runs]
    #for r in mc.runs: print r.path()

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
