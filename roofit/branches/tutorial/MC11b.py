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
        if qcdscale!='AUTO' and sample in ['mc_pythia_bbmu15x','mc_pythia_ccmu15x']:
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
mc.append(MCR(106044,'mc_pythia_wmunu','*',10460/1000,1.000000,6993798))
mc.append(MCR(107054,'mc_pythia_wtaunu','*',10460/1000,1.0,999874)) # inclusive wtaunu
mc.append(MCR(106022,'mc_pythia_wtaunu_1Lepton','*',8916.33*1.17*3.068E-01/1000,1.0,999874)) #actual xs for this is 8.916E3*0.35 (tau is forced to decay into e/mu)
mc.append(MCR(106047,'mc_pythia_zmumu','*',989/1000.0,1.000000,4998410))
mc.append(MCR(106052,'mc_pythia_ztautau','*',989/1000.0,1.000000,1998598))

#QCD jets
# herwig multi-jet (https://savannah.cern.ch/task/?14846, plus request sheet on savannah)
mc.append(MCR(113204,'mc_herwig_J0','*',9.60E06,1.0,500000))
mc.append(MCR(113205,'mc_herwig_J1','*',0.745E06,1.0,500000))
mc.append(MCR(113206,'mc_herwig_J2','*',44.4E03,1.0,500000))
mc.append(MCR(113207,'mc_herwig_J3','*',2.36E03,1.0,500000))
mc.append(MCR(113208,'mc_herwig_J4','*',94.7E00,1.0,500000))
mc.append(MCR(113209,'mc_herwig_J5','*',2.58E00,1.0,500000))
mc.append(MCR(113210,'mc_herwig_J6','*',0.039E00,1.0,500000))

# QCD approximate (from peter):
mc.append(MCR(108405,'mc_pythia_bbmu15x','*',7.39E+04/1000,1.0,500000))
mc.append(MCR(105757,'mc_pythia_mu10mu10x','*',2.83E+03/1000,1.0,500000)) 
mc.append(MCR(106059,'mc_pythia_ccmu15x','*',2.84E+04/1000,1.0,500000))

# MC@NLO samples (updated from Physics_mc11_7TeV.xls, but not usings its 0.9 efficiency factor)
# zmumu sample seems to be wrong based on data/mc agreements
mc.append(MCR(106083,'mc_mcnlo_wminmunu','*',3.99,1.000000,6993798))
mc.append(MCR(106084,'mc_mcnlo_wplusmunu','*',5.87,1.000000,6993798))
mc.append(MCR(106088,'mc_mcnlo_zmumu','*',4.93,0.9,4998410))

# PowHeg NLO samples
# updated Feb 24: copied from AMI. These are NLO, so no K-factors needed?
mc.append(MCR(107391,'mc_powheg_pythia_wmintaunu','*',4.08,1.000000,6993798))
mc.append(MCR(107390,'mc_powheg_pythia_wplustaunu','*',5.98,1.000000,6993798))
mc.append(MCR(107392,'mc_powheg_pythia_ztautau','*',9.6744/10.0,1.0,4998410))
mc.append(MCR(108301,'mc_powheg_pythia_wminmunu','*',4.0891,1.000000,6993798))
mc.append(MCR(108298,'mc_powheg_pythia_wplusmunu','*',5.9826,1.000000,6993798))
mc.append(MCR(108304,'mc_powheg_pythia_zmumu','*',9.6864/10.0,1.0,4998410))
# powheg_herwig xsections missing in AMI; copying from powheg_pythia!
mc.append(MCR(113185,'mc_powheg_herwig_wminmunu','*',4.0891,1.000000,6993798))
mc.append(MCR(113187,'mc_powheg_herwig_wplusmunu','*',5.9826,1.000000,6993798))
mc.append(MCR(126007,'mc_powheg_herwig_zmumu','*',9.6864/10.0,1.0,4998410))

# Sherpa (CTEQ 6L1)
mc.append(MCR(114612,'mc_sherpa_wmunu','*',9.09,1.000000,6993798))
mc.append(MCR(114610,'mc_sherpa_zmumu','*',899.72/1000.0,1.000000,4998410))


# W/Z + jets samples:
# https://svnweb.cern.ch/trac/atlasgrp/browser/Physics/StandardModel/Common/Winter2012/mc11c_p833_info.txt
#wmunu (mc11c_p833_info.txt, ATL-COM-PHYS-2011-1522.pdf)
mc.append(MCR(107690,'mc_jimmy_wmunu_np0','*',6.91960,1.2,500000))
mc.append(MCR(107691,'mc_jimmy_wmunu_np1','*',1.30420,1.2,500000))
mc.append(MCR(107692,'mc_jimmy_wmunu_np2','*',0.37783,1.2,500000))
mc.append(MCR(107693,'mc_jimmy_wmunu_np3','*',0.10188,1.2,500000))
mc.append(MCR(107694,'mc_jimmy_wmunu_np4','*',0.02575,1.2,500000))
mc.append(MCR(107695,'mc_jimmy_wmunu_np5','*',0.00692,1.2,500000))
# alpgen pythia (copied from alpgen-jimmy)
mc.append(MCR(117690,'mc_alpgen_pythia_wmunu_np0','*',6.91960,1.2,500000))
mc.append(MCR(117691,'mc_alpgen_pythia_wmunu_np1','*',1.30420,1.2,500000))
mc.append(MCR(117692,'mc_alpgen_pythia_wmunu_np2','*',0.37783,1.2,500000))
mc.append(MCR(117693,'mc_alpgen_pythia_wmunu_np3','*',0.10188,1.2,500000))
mc.append(MCR(117694,'mc_alpgen_pythia_wmunu_np4','*',0.02575,1.2,500000))
mc.append(MCR(117695,'mc_alpgen_pythia_wmunu_np5','*',0.00692,1.2,500000))
#wtaunu (mc11c_p833_info.txt, ATL-COM-PHYS-2011-1522.pdf)
mc.append(MCR(107700,'mc_jimmy_wtaunu_np0','*',6918.60*1.2/1000.0,1.0,500000))
mc.append(MCR(107701,'mc_jimmy_wtaunu_np1','*',1303.20*1.2/1000.0,1.0,500000))
mc.append(MCR(107702,'mc_jimmy_wtaunu_np2','*',378.18*1.2/1000.0,1.0,500000))
mc.append(MCR(107703,'mc_jimmy_wtaunu_np3','*',101.51*1.2/1000.0,1.0,500000))
mc.append(MCR(107704,'mc_jimmy_wtaunu_np4','*',25.64*1.2/1000.0,1.0,500000))
mc.append(MCR(107705,'mc_jimmy_wtaunu_np5','*',7.04*1.2/1000.0,1.0,500000))
#zmumu (mc11c_p833_info.txt, ATL-COM-PHYS-2011-1522.pdf)
mc.append(MCR(107660,'mc_jimmy_zmumu_np0','*',0.66868*1.25,1.0,500000))
mc.append(MCR(107661,'mc_jimmy_zmumu_np1','*',0.13414*1.25,1.0,500000))
mc.append(MCR(107662,'mc_jimmy_zmumu_np2','*',0.04033*1.25,1.0,500000))
mc.append(MCR(107663,'mc_jimmy_zmumu_np3','*',0.01119*1.25,1.0,500000))
mc.append(MCR(107664,'mc_jimmy_zmumu_np4','*',0.00275*1.25,1.0,500000))
mc.append(MCR(107665,'mc_jimmy_zmumu_np5','*',0.00077*1.25,1.0,500000))
#ztautau (mc11c_p833_info.txt, ATL-COM-PHYS-2011-1522.pdf)
mc.append(MCR(107670,'mc_jimmy_ztautau_np0','*',6.684E+2*1.25/1000.0,1.0,6600000))
mc.append(MCR(107671,'mc_jimmy_ztautau_np1','*',1.3481E+2*1.25/1000.0,1.0,1300000))
mc.append(MCR(107672,'mc_jimmy_ztautau_np2','*',4.036E+1*1.25/1000.0,1.0,400000))
mc.append(MCR(107673,'mc_jimmy_ztautau_np3','*',1.125E+1*1.25/1000.0,1.0,110000))
mc.append(MCR(107674,'mc_jimmy_ztautau_np4','*',2.79E+0*1.25/1000.0,1.0,30000))
mc.append(MCR(107675,'mc_jimmy_ztautau_np5','*',7.7E-1*1.25/1000.0,1.0,10000))

# HEAVY FLAVOR
#zmumu->bb (mc11c_p833_info.txt)
mc.append(MCR(109305,'mc_jimmy_zmumubb_np0','*',6.56/1000.0*1.25,1.0,500000))
mc.append(MCR(109306,'mc_jimmy_zmumubb_np1','*',2.47/1000.0*1.25,1.0,500000))
mc.append(MCR(109307,'mc_jimmy_zmumubb_np2','*',0.89/1000.0*1.25,1.0,500000))
mc.append(MCR(109308,'mc_jimmy_zmumubb_np3','*',0.39/1000.0*1.25,1.0,500000))
#Wc (mc11c_p833_info.txt)
mc.append(MCR(117293,'mc_jimmy_wc_np0','*',650.0/1000.0,1.2,500000))
mc.append(MCR(117294,'mc_jimmy_wc_np1','*',205.0/1000.0,1.2,500000))
mc.append(MCR(117295,'mc_jimmy_wc_np2','*',50.8/1000.0,1.2,500000))
mc.append(MCR(117296,'mc_jimmy_wc_np3','*',11.4/1000.0,1.2,500000))
mc.append(MCR(117297,'mc_jimmy_wc_np4','*',2.8/1000.0,1.2,500000))
#Wbb (mc11c_p833_info.txt)
mc.append(MCR(107280,'mc_jimmy_wbb_np0','*',47.32/1000.0,1.2,500000))
mc.append(MCR(107281,'mc_jimmy_wbb_np1','*',35.77/1000.0,1.2,500000))
mc.append(MCR(107282,'mc_jimmy_wbb_np2','*',17.34/1000.0,1.2,500000))
mc.append(MCR(107283,'mc_jimmy_wbb_np3','*',6.63/1000.0,1.2,500000))
#Wcc (mc11c_p833_info.txt)
mc.append(MCR(117284,'mc_jimmy_wcc_np0','*',127.53/1000.0,1.2,500000))
mc.append(MCR(117285,'mc_jimmy_wcc_np1','*',104.68/1000.0,1.2,500000))
mc.append(MCR(117286,'mc_jimmy_wcc_np2','*',52.08/1000.0,1.2,500000))
mc.append(MCR(117287,'mc_jimmy_wcc_np3','*',16.96/1000.0,1.2,500000))

# McAtNLO diboson (mc11c_p833_info.txt, ATL-COM-PHYS-2011-1522.pdf)
mc.append(MCR(105921,'mc_mcnlo_WpWm_enuenu','*',0.50377/1000.0,1.0,200000))
mc.append(MCR(105922,'mc_mcnlo_WpWm_enumunu','*',0.50377/1000.0,1.0,200000))
mc.append(MCR(105923,'mc_mcnlo_WpWm_enutaunu','*',0.50377/1000.0,1.0,200000))
mc.append(MCR(105924,'mc_mcnlo_WpWm_munumunu','*',0.50377/1000.0,1.0,200000))
mc.append(MCR(105925,'mc_mcnlo_WpWm_munuenu','*',0.50377/1000.0,1.0,200000))
mc.append(MCR(105926,'mc_mcnlo_WpWm_munutaunu','*',0.50377/1000.0,1.0,200000))
mc.append(MCR(105927,'mc_mcnlo_WpWm_taunutaunu','*',0.50377/1000.0,1.0,200000))
mc.append(MCR(105928,'mc_mcnlo_WpWm_taunuenu','*',0.50377/1000.0,1.0,200000))
mc.append(MCR(105929,'mc_mcnlo_WpWm_taunumunu','*',0.50377/1000.0,1.0,200000))

# Herwig diboson (mc11c_p833_info.txt)
mc.append(MCR(105985,'mc_herwig_ww','*',29.592*1.52/1000.0,0.38863,250000))
mc.append(MCR(105987,'mc_herwig_wz','*',3.432*1.58/1000.0,1.0,250000))
mc.append(MCR(105986,'mc_herwig_zz','*',4.60*1.41/1000.0,0.21152,250000))

# ttbar (mc11c_p833_info.txt, ATL-COM-PHYS-2011-1522.pdf)
mc.append(MCR(105200,'mc_jimmy_ttbar','*',0.16457,0.5551,500000))

# single top  (mc11c_p833_info.txt)
mc.append(MCR(105500,'mc_acer_wt','*',15.74/1000.0,1.0,200000))
mc.append(MCR(117360,'mc_acer_tchan_enu','*',6.94/1000.0,1.0,200000))
mc.append(MCR(117361,'mc_acer_tchan_munu','*',6.83/1000.0,1.0,200000))
mc.append(MCR(117362,'mc_acer_tchan_taunu','*',7.26/1000.0,1.0,200000))
mc.append(MCR(117363,'mc_acer_schan_enu','*',0.498/1000.0,1.0,300000))
mc.append(MCR(117364,'mc_acer_schan_munu','*',0.498/1000.0,1.0,300000))
mc.append(MCR(117365,'mc_acer_schan_taunu','*',0.498/1000.0,1.0,300000))

if False:
    print 'Registered',mc.nruns(),'runs:'
    print 'nevents   =',mc.nevents()
    #[r.pathena_submit() for r in mc.runs]
    #for r in mc.runs: print r.path()

def is_wmunu(iname):
    return any( [re.search(v,iname) for v in ('wmunu','wminmunu','wplusmunu')] )

def is_qcd(iname):
    return any( [re.search(v,iname) for v in ('bbmu15x','ccmu15x','herwig_J','pythia_J')] )

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
