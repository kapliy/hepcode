#!/usr/bin/env python
import sys,os,re
import ROOT

# THIS VERSION INCLUDES INTEGRATED CROSS-SECTIONS FROM MASSIMILIANO BELLOMO

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
    def __init__(s,rnum,sample,xsec,filteff=1.0,nevents=0,ntupleDS=None):
        s.rnum = rnum
        s.sample = sample
        s.tag = 'deprecated'
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

# VALUES FROM MASSIMILIANO BELLOMO (June 27)
m_xsec = {}
m_xsec["DrellYan"]      = 1250.0;
m_xsec["Zll"]           = 990.0;
m_xsec["W+ln"]          = 6160.0;
m_xsec["W-ln"]          = 4300.0;
m_xsec["Wlnu"]          = 10460.0;
m_xsec["TTbar"]         = 165.0*0.555;
m_xsec["Wtaunu"]        = 10460.0;
m_xsec["Ztautau"]       = 990.0;
m_xsec["bbmu15"]        = 73900.0;
m_xsec["ccmu15"]        = 28400.0;
m_xsec["J0"]            = 1.2032e10;
m_xsec["J1"]            = 8.0714e8;
m_xsec["J2"]            = 4.8027e7;
m_xsec["J3"]            = 2.5364e6;
m_xsec["J4"]            = 9.9605e4;
m_xsec["J5"]            = 2.5947e3;
m_xsec["J6"]            = 3.5457e1;
m_xsec["WW"]            = 44.9*0.3888;
m_xsec["WZ"]            = 18.5*0.30976;
m_xsec["ZZ"]            = 6.02*0.2121;

mc = MC_samples()

#from http://cdsweb.cern.ch/record/1282370/files/ATL-CAL-PROC-2010-001.pdf?version=3
#from http://cdsweb.cern.ch/record/1298803/files/ATL-COM-PHYS-2010-836.pdf
#http://www.dspace.cam.ac.uk/bitstream/1810/236979/1/tomthesis.pdf
mc.append(MCR(106044,'mc_pythia_wmunu',m_xsec["Wlnu"]/1000.0))
mc.append(MCR(107054,'mc_pythia_wtaunu',m_xsec["Wtaunu"]/1000.0)) # inclusive wtaunu
mc.append(MCR(106047,'mc_pythia_zmumu',m_xsec["Zll"]/1000.0))
mc.append(MCR(106052,'mc_pythia_ztautau',m_xsec["Ztautau"]/1000.0))
mc.append(MCR(108319,'mc_pythia_dyan',m_xsec["DrellYan"]/1000.0))

# QCD approximate (from peter):
mc.append(MCR(108405,'mc_pythia_bbmu15x',m_xsec["bbmu15"]/1000.0))
mc.append(MCR(106059,'mc_pythia_ccmu15x',m_xsec["ccmu15"]/1000.0))

# MC@NLO samples (updated from Physics_mc11_7TeV.xls, but not usings its 0.9 efficiency factor)
# zmumu sample seems to be wrong based on data/mc agreements
mc.append(MCR(106083,'mc_mcnlo_wplusmunu',m_xsec["W+ln"]/1000.0))
mc.append(MCR(106084,'mc_mcnlo_wminmunu',m_xsec["W-ln"]/1000.0))
mc.append(MCR(106088,'mc_mcnlo_zmumu',m_xsec["Zll"]/1000.0))
mc.append(MCR(108328,'mc_mcnlo_wplustaunu',m_xsec["W+ln"]/1000.0))
mc.append(MCR(108329,'mc_mcnlo_wmintaunu',m_xsec["W-ln"]/1000.0))
mc.append(MCR(106062,'mc_mcnlo_ztautau',m_xsec["Zll"]/1000.0))

# PowHeg+Pythia NLO samples
# updated with Max's NNLO values
mc.append(MCR(108298,'mc_powheg_pythia_wplusmunu',m_xsec["W+ln"]/1000.0))
mc.append(MCR(108301,'mc_powheg_pythia_wminmunu',m_xsec["W-ln"]/1000.0))
mc.append(MCR(108304,'mc_powheg_pythia_zmumu',m_xsec["Zll"]/1000.0))
mc.append(MCR(107390,'mc_powheg_pythia_wplustaunu',m_xsec["W+ln"]/1000.0))
mc.append(MCR(107391,'mc_powheg_pythia_wmintaunu',m_xsec["W-ln"]/1000.0))
mc.append(MCR(107392,'mc_powheg_pythia_ztautau',m_xsec["Zll"]/1000.0))
# PowHeg+Herwig NLO samples
mc.append(MCR(113187,'mc_powheg_herwig_wplusmunu',m_xsec["W+ln"]/1000.0))
mc.append(MCR(113185,'mc_powheg_herwig_wminmunu',m_xsec["W-ln"]/1000.0))
mc.append(MCR(126007,'mc_powheg_herwig_zmumu',m_xsec["Zll"]/1000.0))

# Herwig diboson (mc11c_p833_info.txt) - updated with Max's values
mc.append(MCR(105985,'mc_herwig_ww',m_xsec["WW"]/1000.0))
mc.append(MCR(105987,'mc_herwig_wz',m_xsec["WZ"]/1000.0))
mc.append(MCR(105986,'mc_herwig_zz',m_xsec["ZZ"]/1000.0))

# ttbar (mc11c_p833_info.txt, ATL-COM-PHYS-2011-1522.pdf) - mc@nlo, actually
mc.append(MCR(105200,'mc_alpgen_herwig_ttbar',m_xsec["TTbar"]/1000.0))
mc.append(MCR(105200,'mc_mcnlo_ttbar',m_xsec["TTbar"]/1000.0))

# single top  (mc11c_p833_info.txt)
mc.append(MCR(105500,'mc_acer_wt',15.74/1000.0))
mc.append(MCR(117360,'mc_acer_tchan_enu',6.94/1000.0))
mc.append(MCR(117361,'mc_acer_tchan_munu',6.83/1000.0))
mc.append(MCR(117362,'mc_acer_tchan_taunu',7.26/1000.0))
mc.append(MCR(117363,'mc_acer_schan_enu',0.498/1000.0))
mc.append(MCR(117364,'mc_acer_schan_munu',0.498/1000.0))
mc.append(MCR(117365,'mc_acer_schan_taunu',0.498/1000.0))

# single top mc@nlo (used by Max)
mc.append(MCR(108346,'mc_mcnlo_wt',15.74/1000.0))
mc.append(MCR(108344,'mc_mcnlo_schan_munu',0.498/1000.0))
mc.append(MCR(108341,'mc_mcnlo_tchan_munu',6.83/1000.0))

# W/Z + jets samples:
# https://svnweb.cern.ch/trac/atlasgrp/browser/Physics/StandardModel/Common/Winter2012/mc11c_p833_info.txt
#wmunu (mc11c_p833_info.txt, ATL-COM-PHYS-2011-1522.pdf)
mc.append(MCR(107690,'mc_alpgen_herwig_wmunu_np0',6.91960,1.2))
mc.append(MCR(107691,'mc_alpgen_herwig_wmunu_np1',1.30420,1.2))
mc.append(MCR(107692,'mc_alpgen_herwig_wmunu_np2',0.37783,1.2))
mc.append(MCR(107693,'mc_alpgen_herwig_wmunu_np3',0.10188,1.2))
mc.append(MCR(107694,'mc_alpgen_herwig_wmunu_np4',0.02575,1.2))
mc.append(MCR(107695,'mc_alpgen_herwig_wmunu_np5',0.00692,1.2))
# alpgen pythia (copied from alpgen-jimmy)
mc.append(MCR(117690,'mc_alpgen_pythia_wmunu_np0',6.91960,1.2))
mc.append(MCR(117691,'mc_alpgen_pythia_wmunu_np1',1.30420,1.2))
mc.append(MCR(117692,'mc_alpgen_pythia_wmunu_np2',0.37783,1.2))
mc.append(MCR(117693,'mc_alpgen_pythia_wmunu_np3',0.10188,1.2))
mc.append(MCR(117694,'mc_alpgen_pythia_wmunu_np4',0.02575,1.2))
mc.append(MCR(117695,'mc_alpgen_pythia_wmunu_np5',0.00692,1.2))
#wtaunu (mc11c_p833_info.txt, ATL-COM-PHYS-2011-1522.pdf)
mc.append(MCR(107700,'mc_alpgen_herwig_wtaunu_np0',6918.60*1.2/1000.0))
mc.append(MCR(107701,'mc_alpgen_herwig_wtaunu_np1',1303.20*1.2/1000.0))
mc.append(MCR(107702,'mc_alpgen_herwig_wtaunu_np2',378.18*1.2/1000.0))
mc.append(MCR(107703,'mc_alpgen_herwig_wtaunu_np3',101.51*1.2/1000.0))
mc.append(MCR(107704,'mc_alpgen_herwig_wtaunu_np4',25.64*1.2/1000.0))
mc.append(MCR(107705,'mc_alpgen_herwig_wtaunu_np5',7.04*1.2/1000.0))
#zmumu (mc11c_p833_info.txt, ATL-COM-PHYS-2011-1522.pdf)
mc.append(MCR(107660,'mc_alpgen_herwig_zmumu_np0',0.66868*1.25))
mc.append(MCR(107661,'mc_alpgen_herwig_zmumu_np1',0.13414*1.25))
mc.append(MCR(107662,'mc_alpgen_herwig_zmumu_np2',0.04033*1.25))
mc.append(MCR(107663,'mc_alpgen_herwig_zmumu_np3',0.01119*1.25))
mc.append(MCR(107664,'mc_alpgen_herwig_zmumu_np4',0.00275*1.25))
mc.append(MCR(107665,'mc_alpgen_herwig_zmumu_np5',0.00077*1.25))
#ztautau (mc11c_p833_info.txt, ATL-COM-PHYS-2011-1522.pdf)
mc.append(MCR(107670,'mc_alpgen_herwig_ztautau_np0',6.684E+2*1.25/1000.0))
mc.append(MCR(107671,'mc_alpgen_herwig_ztautau_np1',1.3481E+2*1.25/1000.0))
mc.append(MCR(107672,'mc_alpgen_herwig_ztautau_np2',4.036E+1*1.25/1000.0))
mc.append(MCR(107673,'mc_alpgen_herwig_ztautau_np3',1.125E+1*1.25/1000.0))
mc.append(MCR(107674,'mc_alpgen_herwig_ztautau_np4',2.79E+0*1.25/1000.0))
mc.append(MCR(107675,'mc_alpgen_herwig_ztautau_np5',7.7E-1*1.25/1000.0))

# HEAVY FLAVOR
#zmumu->bb (mc11c_p833_info.txt)
mc.append(MCR(109305,'mc_alpgen_herwig_zmumubb_np0',6.56/1000.0*1.25))
mc.append(MCR(109306,'mc_alpgen_herwig_zmumubb_np1',2.47/1000.0*1.25))
mc.append(MCR(109307,'mc_alpgen_herwig_zmumubb_np2',0.89/1000.0*1.25))
mc.append(MCR(109308,'mc_alpgen_herwig_zmumubb_np3',0.39/1000.0*1.25))
#Wc (mc11c_p833_info.txt)
mc.append(MCR(117293,'mc_alpgen_herwig_wc_np0',650.0/1000.0,1.2))
mc.append(MCR(117294,'mc_alpgen_herwig_wc_np1',205.0/1000.0,1.2))
mc.append(MCR(117295,'mc_alpgen_herwig_wc_np2',50.8/1000.0,1.2))
mc.append(MCR(117296,'mc_alpgen_herwig_wc_np3',11.4/1000.0,1.2))
mc.append(MCR(117297,'mc_alpgen_herwig_wc_np4',2.8/1000.0,1.2))
#Wbb (mc11c_p833_info.txt)
mc.append(MCR(107280,'mc_alpgen_herwig_wbb_np0',47.32/1000.0,1.2))
mc.append(MCR(107281,'mc_alpgen_herwig_wbb_np1',35.77/1000.0,1.2))
mc.append(MCR(107282,'mc_alpgen_herwig_wbb_np2',17.34/1000.0,1.2))
mc.append(MCR(107283,'mc_alpgen_herwig_wbb_np3',6.63/1000.0,1.2))
#Wcc (mc11c_p833_info.txt)
mc.append(MCR(117284,'mc_alpgen_herwig_wcc_np0',127.53/1000.0,1.2))
mc.append(MCR(117285,'mc_alpgen_herwig_wcc_np1',104.68/1000.0,1.2))
mc.append(MCR(117286,'mc_alpgen_herwig_wcc_np2',52.08/1000.0,1.2))
mc.append(MCR(117287,'mc_alpgen_herwig_wcc_np3',16.96/1000.0,1.2))

#####################################################################################
# RARELY USED SAMPLES
#####################################################################################
mc.append(MCR(105757,'mc_pythia_mu10mu10x',2.83E+03/1000)) 
#QCD jets
# herwig multi-jet (https://savannah.cern.ch/task/?14846, plus request sheet on savannah)
mc.append(MCR(113204,'mc_herwig_J0',9.60E06))
mc.append(MCR(113205,'mc_herwig_J1',0.745E06))
mc.append(MCR(113206,'mc_herwig_J2',44.4E03))
mc.append(MCR(113207,'mc_herwig_J3',2.36E03))
mc.append(MCR(113208,'mc_herwig_J4',94.7E00))
mc.append(MCR(113209,'mc_herwig_J5',2.58E00))
mc.append(MCR(113210,'mc_herwig_J6',0.039E00))
# pythia multi-jet [via MAX]
mc.append(MCR(113204,'mc_pythia_J0',m_xsec["J0"]/1000.0))
mc.append(MCR(113205,'mc_pythia_J1',m_xsec["J1"]/1000.0))
mc.append(MCR(113206,'mc_pythia_J2',m_xsec["J2"]/1000.0))
mc.append(MCR(113207,'mc_pythia_J3',m_xsec["J3"]/1000.0))
mc.append(MCR(113208,'mc_pythia_J4',m_xsec["J4"]/1000.0))
mc.append(MCR(113209,'mc_pythia_J5',m_xsec["J5"]/1000.0))
mc.append(MCR(113210,'mc_pythia_J6',m_xsec["J6"]/1000.0))
#actual xs for this is 8.916E3*0.35 (tau is forced to decay into e/mu):
mc.append(MCR(106022,'mc_pythia_wtaunu_1Lepton',8916.33*1.17*3.068E-01/1000))
# McAtNLO diboson (mc11c_p833_info.txt, ATL-COM-PHYS-2011-1522.pdf)
mc.append(MCR(105921,'mc_mcnlo_WpWm_enuenu',0.50377/1000.0))
mc.append(MCR(105922,'mc_mcnlo_WpWm_enumunu',0.50377/1000.0))
mc.append(MCR(105923,'mc_mcnlo_WpWm_enutaunu',0.50377/1000.0))
mc.append(MCR(105924,'mc_mcnlo_WpWm_munumunu',0.50377/1000.0))
mc.append(MCR(105925,'mc_mcnlo_WpWm_munuenu',0.50377/1000.0))
mc.append(MCR(105926,'mc_mcnlo_WpWm_munutaunu',0.50377/1000.0))
mc.append(MCR(105927,'mc_mcnlo_WpWm_taunutaunu',0.50377/1000.0))
mc.append(MCR(105928,'mc_mcnlo_WpWm_taunuenu',0.50377/1000.0))
mc.append(MCR(105929,'mc_mcnlo_WpWm_taunumunu',0.50377/1000.0))
# Sherpa (CTEQ 6L1)
mc.append(MCR(114612,'mc_sherpa_wmunu',9.09))
mc.append(MCR(114610,'mc_sherpa_zmumu',899.72/1000.0))


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
