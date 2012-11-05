#!/usr/bin/env python

_QUALITY = ' && fabs(l_eta)<2.4 && idhits==1 && fabs(z0)<10. && fabs(d0sig)<10. && fabs(l_pt_id-l_pt_exms)/l_pt_id<0.5'
_PRE_PETER  = 'l_pt>20.0 && ptiso40<2.0 && etiso40<2.0 && met>25.0 && w_mt>40.0'+_QUALITY
_PRE_MAX = "ptiso40/l_pt<0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1"

# X - tag muon (doesn't change), Y - probe muon used to measure efficiency ( = Y after/before specific cut)
_BEF = 'lX_idhits==1 && fabs(lP_z0)<10. && fabs(lX_eta)<2.4 && lX_pt>10.0 && lY_idhits==1 && fabs(lN_z0)<10. && fabs(lY_eta)<2.4 && lY_pt>10.0 && Z_m>81.0 && Z_m<101.0 && (lP_q*lN_q)<0 && fabs(lP_z0-lN_z0)<3 && fabs(lP_d0-lN_d0)<2 && fabs(lP_phi-lN_phi)>2.0 && lX_ptiso40<2.0 && lX_etiso40<2.0'
_AFT = _BEF + ' && ' + 'lY_etiso40<2.0'

import sys,re,os
from hashlib import md5
from optparse import OptionParser
import antondb
parser = OptionParser()
parser.add_option("-m", "--mode",dest="mode",
                  type="string", default='1',
                  help="Plot mode")
parser.add_option("--ntuple",dest="ntuple",
                  type="string", default='w',
                  help="What kind of ntuple: 'w' or 'z'?")
parser.add_option("--input",dest="input",
                  type="string", default='ROOT/current/all',
                  help="Path to input root file with all histos")
parser.add_option("--var",dest="var",
                  type="string", default='met',
                  help="Variable to plot")
parser.add_option("--bin",dest="bin",
                  type="string", default='50,-2.5,2.5',
                  help="Binning for var")
parser.add_option("--lvar",dest="lvar",
                  type="string", default='lY_eta',
                  help="Variable that (1) slices the dataset in Z studies, or (2) used to fit QCD")
parser.add_option("--lbin",dest="lbin",
                  type="string", default='25,-2.5,2.5',
                  help="Binning for lvar")
parser.add_option("--llog", default=False,
                  action="store_true",dest="llog",
                  help="If set to true, QCD fit in lvar is plotted on log scale")
parser.add_option("--extra",dest="extra",
                  type="string", default=None,
                  help="General-purpose extra parameter")
parser.add_option("--pre",dest="pre",
                  type="string", default=_PRE_MAX,
                  help="Preliminary cuts to select final W candidates")
parser.add_option("--prebef",dest="prebef",
                  type="string", default=_BEF,
                  help="Cuts BEFORE applying studied cut")
parser.add_option("--preaft",dest="preaft",
                  type="string", default=_AFT,
                  help="Cuts AFTER applying studied cut")
parser.add_option("--preNN",dest="preNN",
                  type="string", default="",
                  help="QCD fits: nominal cut for histogram we are plotting")
parser.add_option("--preNQ",dest="preNQ",
                  type="string", default="",
                  help="QCD fits: QCD template shape cut (eg, anti-isolation)")
parser.add_option("--preFN",dest="preFN",
                  type="string", default=None,
                  help="QCD fits: nominal cut for histogram we are plotting + FIT REGION")
parser.add_option("--preFQ",dest="preFQ",
                  type="string", default=None,
                  help="QCD fits: QCD template shape cut (eg, anti-isolation) + FIT REGION")
parser.add_option("--isofail",dest="isofail",
                  type="string", default='IsoFail20', #IsoWind20
                  help="QCD fits: QCD template anti-isolation")
parser.add_option("--cut",dest="cut",
                  type="string", default='mcw*puw*wzptw*wpolw*vxw*ls1w*ls2w*effw*isow*trigw',
                  help="Additional cut to select events")
parser.add_option("--hsource",dest="hsource",
                  type="string", default='%s/st_%s_final/%s',
                  help="Template for the path to data histograms")
parser.add_option("--etamode",dest="etamode",
                  type="int", default=2,
                  help="Etamode=1 : use eta bins. Etamode=2 : use |eta| bins")
parser.add_option("--metallsys", default=False,
                  action="store_true",dest="metallsys",
                  help="If set to true, re-fits QCD for each systematic")
parser.add_option("--rebin",dest="rebin",
                  type="int", default=1,
                  help="Rebin histograms")
parser.add_option("--refline",dest="refline",
                  type="string", default='0.81,1.19',
                  help="The range for ratio histogram reference line used in stack plots")
parser.add_option("--antondb",dest="antondb",
                  type="string", default=None,
                  help="Tag for antondb output container")
parser.add_option("--effroot",dest="effroot",
                  type="string", default=None,
                  help="Location of acceptance efficiency histograms (correction to truth level)")
parser.add_option("-d","--dataperiods",dest="dataperiods",
                  type="string", default=None,
                  help="Comma-separated list of data periods to process")
_DATA_PERIODS_DEFAULT = ('D','E','F','G','H','I','J','K','L','M') # default
#1340.03*1000.0 - BtoI with pro08
#4713.11*1000.0 - BtoM with pro10
#4701.37*1000.0 - DtoM with pro10, OflLumi-7TeV-002
#4644.00*1000.0 - DtoM with pro10, OflLumi-7TeV-003
parser.add_option("--lumi",dest="lumi",
                  type="float", default=4644.0*1000.0,
                  help="Integrated luminosity for data (in nb^-1)")
parser.add_option("--qcdscale",dest="qcdscale",
                  type="float", default=None,
                  help="QCD scale factor")
parser.add_option("--qcdsource",dest="qcdsource",
                  type="string", default=None,
                  help="A source of QCD histogram in a separate file")
parser.add_option("-o", "--output",dest="output",
                  type="string", default=None,
                  help="Name of output dir for plots")
parser.add_option("--root", default=False,
                  action="store_true",dest="root",
                  help="Save final histo in root file")
parser.add_option('-t',"--tag",dest="tag",
                  type="string", default='plot',
                  help="A tag to append to all output plots")
parser.add_option('-b', "--batch", default=False,
                  action="store_true",dest="batch",
                  help="Enable batch mode (all output directly to filesystem)")
parser.add_option('-v', "--verbose", default=False,
                  action="store_true",dest="verbose",
                  help="Enable verbose printouts")
parser.add_option("-q", "--charge",dest="charge",
                  type="int", default=2,
                  help="Which charge to plot: 0=POS, 1=NEG, 2=ALL")
parser.add_option("--bgqcd",dest="bgqcd",
                  type="int", default=3,
                  help="QCD: 0=Pythia bb/cc mu15X, 1=Pythia bbmu15X, 2=Pythia J0..J5, 3=data-driven, 4=data-driven with bgsub")
parser.add_option("--bgsig",dest="bgsig",
                  type="int", default=2,
                  help="Signal: 0=Pythia, 1=MC@NLO, 2=Alpgen/Herwig, 3=Alpgen/Pythia, 4=PowHeg/Herwig, 5=PowHeg/Pythia")
parser.add_option("--bgewk",dest="bgewk",
                  type="int", default=2,
                  help="EWK: 0=Pythia, 1=MC@NLO, 2=Alpgen/Herwig, 3=Alpgen/Pythia, 4=PowHeg/Herwig, 5=PowHeg/Pythia")
parser.add_option("--xsecerr",dest="xsecerr",
                  type="int", default=0,
                  help="Cross-section uncertainty. 0=nominal, +/-1 = one sigma variations")
parser.add_option('-f',"--func",dest="func",
                  type="string", default='gaus',
                  help="func = {gaus,egge,voig,voigbg,bw,bwfull}{0=none;1=gaus;2=double-gaus;3=crystal-ball}")
(opts, args) = parser.parse_args()
mode = opts.mode
print "MODE =",mode
_DATA_PERIODS = opts.dataperiods.split(',') if opts.dataperiods else _DATA_PERIODS_DEFAULT
gbg = []; COUT = [];
# antondb containers
VMAP = {}; OMAP = []
VMAP['cmd']=' '.join(sys.argv)
print VMAP['cmd']

#import ROOT and disable warning messages
from common import *
from binning import *
#ROOT.TH1.AddDirectory(ROOT.kFALSE)    # ensure that we own all the histograms
ROOT.SetSignalPolicy(ROOT.kSignalFast)
ROOT.gROOT.SetBatch(opts.batch)
ROOT.TH1.SetDefaultSumw2()
#ROOT.gStyle.SetOptFit(1111)

from FileLock import FileLock
from SuCanvas import *
SuCanvas._refLineMin = float(opts.refline.split(',')[0])
SuCanvas._refLineMax = float(opts.refline.split(',')[1])
SuCanvas.savetag     = opts.tag
SuCanvas.savedir = './'
if opts.output:
    SuCanvas.savedir = opts.output+'/'
# overrides for default style:
if True:
    SuCanvas.g_lin_ratio_y_title_offset = 1.7
    SuCanvas.g_marker_size = 0.9
    SuCanvas.g_legend_x1_ndc = 0.55
    SuCanvas.g_text_size = 20
    SuCanvas.g_legend_height_per_entry = 0.04
# Apply plot style
SuCanvas.cgStyle = SuCanvas.ControlPlotStyle()
#SetStyle("AtlasStyle.C")

from SuData import *
SuSample.rootpath = opts.input
SuSample.debug = opts.verbose
SuSample.lumi = opts.lumi
SuSample.xsecerr = opts.xsecerr
SuStackElm.qcdsource = opts.qcdsource
SuStackElm.qcdscale = opts.qcdscale

from ntuple_tools import *
def renormalize(bin='100,5,100'):
    """ Normalizes MET template """
    if not opts.qcdscale in ('AUTO','auto','Auto'):
        return
    if opts.ntuple=='z': # later: choose a good region to normalize Z QCD contribution
        return
    fitpre = metfitreg(opts.pre)
    ctmp,frame,scale = run_fit(pre=fitpre,bin=bin)
    OMAP.append(ctmp)
    # understand why this is an inverse of fitted scale here!
    SuSample.qcdscale = 1.0/scale

def particle(h,inp=opts.input,var=opts.var,bin=opts.bin,q=opts.charge):
    """ Uses pre-computed efficiency histogram to convert a given reco-level quantity to particle-level """
    if opts.effroot and os.path.exists(opts.effroot):
        f = ROOT.TFile.Open(opts.effroot,'READ')
        assert f and f.IsOpen()
        key_str = re.sub(r'[^\w]', '_', 'eff_%s_%s_%s_%d'%(inp,var,bin,q))
        assert f.Get(key_str),'Failed to find key: %s'%key_str
        heff = f.Get(key_str).Clone()
        if heff:
            print '---> Applying efficiency histogram to:',h.GetName()
            h.Divide(heff)
        f.Close()
    return h

# MC stack order
pw,pz = [SuStack() for zz in xrange(2)]
# w samples:
if True:
    #DIBOSON:
    pw.add(name='WW/WZ/ZZ',label='Dibosons',samples=['mc_herwig_ww','mc_herwig_wz','mc_herwig_zz'],color=ROOT.kOrange-4,flags=['bg','mc','ewk','diboson'])
    #ZTAUTAU:
    pw.adn(name='ztautau_alpgen_herwig',label='Z #rightarrow #tau#tau',samples=['mc_alpgen_herwig_ztautau_np%d'%v for v in range(6)],color=ROOT.kViolet,flags=['bg','mc','ewk','ztautau'])
    pw.adn(name='ztautau_pythia',label='Z #rightarrow #tau#tau',samples='mc_pythia_ztautau',color=ROOT.kViolet,flags=['bg','mc','ewk','ztautau'])
    pw.adn(name='ztautau_powheg_pythia',label='Z #rightarrow #tau#tau',samples='mc_powheg_pythia_ztautau',color=ROOT.kViolet,flags=['bg','mc','ewk','ztautau'])
    #TOP:
    if False: # separate ttbar and single top
        pw.add(name='t#bar{t}',label='t #bar{t}',samples='mc_mcnlo_ttbar', color=ROOT.kGreen+1,flags=['bg','mc','ewk'])
        pw.add(name='single-top',label='single top',samples=['mc_acer_schan_munu','mc_acer_schan_taunu','mc_acer_tchan_munu','mc_acer_tchan_taunu','mc_acer_wt'],color=ROOT.kGreen+3,flags=['bg','mc','ewk'])
    else:     # combined tops
        pw.add(name='t#bar{t}+single-top',label='t#bar{t} + single top',samples=['mc_mcnlo_ttbar','mc_acer_schan_munu','mc_acer_schan_taunu','mc_acer_tchan_munu','mc_acer_tchan_taunu','mc_acer_wt'], color=ROOT.kGreen+1,flags=['bg','mc','ewk'])
    #WTAUNU:
    pw.adn(name='wtaunu_alpgen_herwig',label='W #rightarrow #tau#nu',samples=['mc_alpgen_herwig_wtaunu_np%d'%v for v in range(6)],color=ROOT.kYellow-9,flags=['bg','mc','ewk','wtaunu'])
    pw.adn(name='wtaunu_pythia',label='W #rightarrow #tau#nu',samples='mc_pythia_wtaunu',color=ROOT.kYellow-9,flags=['bg','mc','ewk','wtaunu'])
    pw.adn(name='wtaunu_powheg_pythia',label='W #rightarrow #tau#nu',samples=['mc_powheg_pythia_wplustaunu','mc_powheg_pythia_wmintaunu'],color=ROOT.kYellow-9,flags=['bg','mc','ewk','wtaunu'])
    #ZMUMU + DYAN (Zll = 15 .. 60 GeV) :
    if False: # separate Zmumu and Drell-Yan
        pw.adn(name='zmumu_alpgen_herwig',label='Z #rightarrow #mu#mu',samples=['mc_alpgen_herwig_zmumu_np%d'%v for v in range(6)],color=ROOT.kRed,flags=['bg','mc','ewk','zmumu'])
        pw.adn(name='zmumu_pythia',label='Z #rightarrow #mu#mu',samples=['mc_pythia_zmumu'],color=ROOT.kRed,flags=['bg','mc','ewk','zmumu'])
        pw.adn(name='zmumu_powheg_pythia',label='Z #rightarrow #mu#mu',samples='mc_powheg_pythia_zmumu',color=ROOT.kRed,flags=['bg','mc','ewk','zmumu'])
        pw.add(name='dyan_pythia',label='Drell-Yan',samples='mc_pythia_dyan',color=156,flags=['bg','mc','ewk','dyan'])
    else:     # combined zmumu
        pw.adn(name='zmumu_alpgen_herwig',label='Z #rightarrow #mu#mu + Drell-Yan',samples=['mc_pythia_dyan',]+['mc_alpgen_herwig_zmumu_np%d'%v for v in range(6)],color=ROOT.kRed,flags=['bg','mc','ewk','zmumu'])
        pw.adn(name='zmumu_pythia',label='Z #rightarrow #mu#mu + Drell-Yan',samples=['mc_pythia_dyan',]+['mc_pythia_zmumu'],color=ROOT.kRed,flags=['bg','mc','ewk','zmumu'])
        pw.adn(name='zmumu_powheg_pythia',label='Z #rightarrow #mu#mu + Drell-Yan',samples=['mc_pythia_dyan',]+['mc_powheg_pythia_zmumu',],color=ROOT.kRed,flags=['bg','mc','ewk','zmumu'])
    #EWK SEL: (defaults to alpgen, wherever possible)
    if opts.bgewk==0: #pythia
        pw.choose_wtaunu(0)
        pw.choose_zmumu(0)
        pw.choose_ztautau(0)
    elif opts.bgewk==1: # mcnlo
        pw.choose_wtaunu(1)
        pw.choose_zmumu(1)
        pw.choose_ztautau(1)
    elif opts.bgewk==2: # alpgen/herwig
        pw.choose_wtaunu(2)
        pw.choose_zmumu(2)
        pw.choose_ztautau(2)
    elif opts.bgewk==3: # alpgen/pythia
        pw.choose_wtaunu(2) #NA
        pw.choose_zmumu(2) #NA
        pw.choose_ztautau(2) #NA
    elif opts.bgewk==4: # powheg/herwig
        pw.choose_wtaunu(2) #NA
        pw.choose_zmumu(4)
        pw.choose_ztautau(2) #NA
    elif opts.bgewk==5: # powheg/pythia
        pw.choose_wtaunu(5)
        pw.choose_zmumu(5)
        pw.choose_ztautau(5)
    else:
        assert False,'Unknown bgewk option: %s'%opts.bgewk
    #QCD:
    pw.adn(name='qcd_mc',label='QCD (bbmu15X/ccmu15X)',samples=['mc_pythia_bbmu15x','mc_pythia_ccmu15x'],color=ROOT.kAzure-9,flags=['bg','mc','qcd'])
    pw.adn(name='qcd_mc_driven',label='QCD (bbmu15X/ccmu15X)',samples=['mc_pythia_bbmu15x','mc_pythia_ccmu15x'],color=ROOT.kAzure-9,flags=['bg','mc','qcd','driven'])
    pw.adn(name='qcd_bb',label='QCD (bbmu15X)',samples=['mc_pythia_bbmu15x'],color=ROOT.kAzure-9,flags=['bg','mc','qcd'])
    pw.adn(name='qcd_JX',label='QCD (J0..J5)',samples=['mc_pythia_J%d'%v for v in xrange(5)],color=ROOT.kAzure-9,flags=['bg','mc','qcd'])
    pw.adn(name='qcd_driven',label='QCD (template)',samples=['data_period%s'%s for s in _DATA_PERIODS],color=ROOT.kAzure-9,flags=['bg','mc','qcd','driven'])
    pw.adn(name='qcd_driven_sub',label='QCD (template)',samples=['data_period%s'%s for s in _DATA_PERIODS]+['mc_powheg_pythia_wminmunu','mc_powheg_pythia_wplusmunu'  ,  'mc_mcnlo_ttbar','mc_acer_schan_munu','mc_acer_schan_taunu','mc_acer_tchan_munu','mc_acer_tchan_taunu','mc_acer_wt'  ,  'mc_powheg_pythia_ztautau','mc_powheg_pythia_zmumu','mc_powheg_pythia_wplustaunu','mc_powheg_pythia_wmintaunu','mc_pythia_dyan'   ,   'mc_herwig_ww','mc_herwig_wz','mc_herwig_zz'],color=ROOT.kAzure-9,flags=['bg','mc','qcd','driven_sub'],sample_weights_bgsub=True)
    if opts.bgqcd in MAP_BGQCD.keys():
        pw.choose_qcd(opts.bgqcd)
    else:
        assert False,'Unknown bgqcd option: %s'%opts.bgqcd
    #SIG:
    pw.adn(name='sig_pythia',label='W #rightarrow #mu#nu (Pythia)',samples='mc_pythia_wmunu',color=ROOT.kWhite,flags=['sig','mc','ewk','wmunu'])
    pw.adn(name='sig_sherpa',label='W #rightarrow #mu#nu (Sherpa)',samples='mc_sherpa_wmunu',color=ROOT.kWhite,flags=['sig','mc','ewk','wmunu'])
    pw.adn(name='sig_mcnlo',label='W #rightarrow #mu#nu (MC@NLO)',samples=['mc_mcnlo_wminmunu','mc_mcnlo_wplusmunu'],color=ROOT.kWhite,flags=['sig','mc','ewk','wmunu'])
    pw.adn(name='sig_powheg_herwig',label='W #rightarrow #mu#nu (Powheg+Herwig)',samples=['mc_powheg_herwig_wminmunu','mc_powheg_herwig_wplusmunu'],color=ROOT.kWhite,flags=['sig','mc','ewk','wmunu'])
    pw.adn(name='sig_powheg_pythia',label='W #rightarrow #mu#nu (Powheg+Pythia)',samples=['mc_powheg_pythia_wminmunu','mc_powheg_pythia_wplusmunu'],color=ROOT.kWhite,flags=['sig','mc','ewk','wmunu'])
    pw.adn(name='sig_alpgen_herwig',label='W #rightarrow #mu# (Alpgen+Herwig)',samples=['mc_alpgen_herwig_wmunu_np%d'%v for v in range(6)],color=ROOT.kWhite,flags=['sig','mc','ewk','wmunu'])
    pw.adn(name='sig_af_alpgen_herwig',label='W #rightarrow #mu#nu (Alpgen AFII)',samples=['mc_af_alpgen_herwig_wmunu_np%d'%v for v in range(6)],color=ROOT.kWhite,flags=['sig','mc','ewk','wmunu'])
    pw.adn(name='sig_alpgen_pythia',label='W #rightarrow #mu#nu (Alpgen+Pythia)',samples=['mc_alpgen_pythia_wmunu_np%d'%v for v in range(6)],color=ROOT.kWhite,flags=['sig','mc','ewk','wmunu'])
    if opts.bgsig in MAP_BGSIG.keys():
        pw.choose_sig(opts.bgsig)
    else:
        print MAP_BGSIG
        assert False,'Unknown bgsig option: %s'%opts.bgsig

# z samples:
if True:
    #TOP:
    pz.add(name='t#bar{t}',samples='mc_mcnlo_ttbar', color=ROOT.kGreen+1,flags=['bg','mc','ewk'])
    pz.add(name='single-top',label='single-top',samples=['mc_acer_schan_munu','mc_acer_schan_taunu','mc_acer_tchan_munu','mc_acer_tchan_taunu','mc_acer_wt'],color=ROOT.kGreen+3,flags=['bg','mc','ewk'])
    #ZTAUTAU:
    pz.adn(name='ztautau_alpgen_herwig',label='Z #rightarrow #tau#tau',samples=['mc_alpgen_herwig_ztautau_np%d'%v for v in range(6)],color=ROOT.kViolet,flags=['bg','mc','ewk','ztautau'])
    pz.adn(name='ztautau_pythia',label='Z #rightarrow #tau#tau',samples='mc_pythia_ztautau',color=ROOT.kViolet,flags=['bg','mc','ewk','ztautau'])
    pz.adn(name='ztautau_powheg_pythia',label='Z #rightarrow #tau#tau',samples='mc_powheg_pythia_ztautau',color=ROOT.kViolet,flags=['bg','mc','ewk','ztautau'])
    #WTAUNU:
    pz.adn(name='wtaunu_alpgen_herwig',label='W #rightarrow #tau#nu',samples=['mc_alpgen_herwig_wtaunu_np%d'%v for v in range(6)],color=ROOT.kYellow-9,flags=['bg','mc','ewk','wtaunu'])
    pz.adn(name='wtaunu_pythia',label='W #rightarrow #tau#nu',samples='mc_pythia_wtaunu',color=ROOT.kYellow-9,flags=['bg','mc','ewk','wtaunu'])
    pz.adn(name='wtaunu_powheg_pythia',label='W #rightarrow #tau#nu',samples=['mc_powheg_pythia_wplustaunu','mc_powheg_pythia_wmintaunu'],color=ROOT.kYellow-9,flags=['bg','mc','ewk','wtaunu'])
    #WMUNU:
    pz.adn(name='wmumu_alpgen_herwig',label='W #rightarrow #mu#nu',samples=['mc_alpgen_herwig_wmunu_np%d'%v for v in range(6)],color=ROOT.kBlue,flags=['bg','mc','ewk','wmunu'])
    pz.adn(name='wmumu_pythia',label='W #rightarrow #mu#nu',samples=['mc_pythia_wmunu'],color=ROOT.kBlue,flags=['bg','mc','ewk','wmunu'])
    pz.adn(name='wmumu_powheg_herwig',label='W #rightarrow #mu#nu',samples=['mc_powheg_herwig_wminmunu','mc_powheg_herwig_wplusmunu'],color=ROOT.kBlue,flags=['bg','mc','ewk','wmunu'])
    pz.adn(name='wmumu_powheg_pythia',label='W #rightarrow #mu#nu',samples=['mc_powheg_pythia_wminmunu','mc_powheg_pythia_wplusmunu'],color=ROOT.kBlue,flags=['bg','mc','ewk','wmunu'])
    #DYAN (Zll = 15 .. 60 GeV):
    pz.add(name='dyan_pythia',label='Drell-Yan',samples='mc_pythia_dyan',color=156,flags=['bg','mc','ewk','dyan'])
    #DIBOSON:
    pz.add(name='WW/WZ/ZZ',samples=['mc_herwig_ww','mc_herwig_wz','mc_herwig_zz'],color=ROOT.kOrange-4,flags=['bg','mc','ewk','diboson'])
    #EWK SEL: (defaults to alpgen, wherever possible)
    if opts.bgewk==0: #pythia
        pz.choose_wtaunu(0)
        pz.choose_wmunu(0)
        pz.choose_ztautau(0)
    elif opts.bgewk==1: # mcnlo
        pz.choose_wtaunu(1)
        pz.choose_wmunu(1)
        pz.choose_ztautau(1)
    elif opts.bgewk==2: # alpgen/herwig
        pz.choose_wtaunu(2)
        pz.choose_wmunu(2)
        pz.choose_ztautau(2)
    elif opts.bgewk==3: # alpgen/pythia
        pz.choose_wtaunu(2) #NA
        pz.choose_wmunu(2) #NA
        pz.choose_ztautau(2) #NA
    elif opts.bgewk==4: # powheg/herwig
        pz.choose_wtaunu(2) #NA
        pz.choose_wmunu(4)
        pz.choose_ztautau(2) #NA
    elif opts.bgewk==5: # powheg/pythia
        pz.choose_wtaunu(5)
        pz.choose_wmunu(5)
        pz.choose_ztautau(5)
    else:
        assert False,'Unknown bgewk option: %s'%opts.bgewk
    #QCD:
    pz.adn(name='qcd_mc',label='bbmu15X/ccmu15X',samples=['mc_pythia_bbmu15x','mc_pythia_ccmu15x'],color=ROOT.kAzure-9,flags=['bg','mc','qcd'])
    pz.adn(name='qcd_mc_driven',label='bbmu15X/ccmu15X',samples=['mc_pythia_bbmu15x','mc_pythia_ccmu15x'],color=ROOT.kAzure-9,flags=['bg','mc','qcd','driven'])
    pz.adn(name='qcd_bb',label='bbmu15X',samples=['mc_pythia_bbmu15x'],color=ROOT.kAzure-9,flags=['bg','mc','qcd'])
    pz.adn(name='qcd_JX',label='QCD J0..J5',samples=['mc_pythia_J%d'%v for v in xrange(5)],color=ROOT.kAzure-9,flags=['bg','mc','qcd'])
    pz.adn(name='qcd_driven',label='QCD data-driven',samples=['data_period%s'%s for s in _DATA_PERIODS],color=ROOT.kAzure-9,flags=['bg','mc','qcd','driven'])
    pz.adn(name='qcd_driven_sub',label='QCD data-driven',samples=['data_period%s'%s for s in _DATA_PERIODS]+['mc_powheg_pythia_wminmunu','mc_powheg_pythia_wplusmunu'  ,  'mc_mcnlo_ttbar','mc_acer_schan_munu','mc_acer_schan_taunu','mc_acer_tchan_munu','mc_acer_tchan_taunu','mc_acer_wt'  ,  'mc_powheg_pythia_ztautau','mc_powheg_pythia_zmumu','mc_powheg_pythia_wplustaunu','mc_powheg_pythia_wmintaunu','mc_pythia_dyan'   ,   'mc_herwig_ww','mc_herwig_wz','mc_herwig_zz'],color=ROOT.kAzure-9,flags=['bg','mc','qcd','driven_sub'],sample_weights_bgsub=True)
    if opts.bgqcd in MAP_BGQCD.keys():
        pz.choose_qcd(opts.bgqcd)
    else:
        assert False,'Unknown bgqcd option: %s'%opts.bgqcd
    #SIG:
    pz.adn(name='sig_pythia',label='Z #rightarrow #mu#mu',samples='mc_pythia_zmumu',color=ROOT.kWhite,flags=['sig','mc','ewk','zmumu'])
    pz.adn(name='sig_sherpa',label='Z #rightarrow #mu#mu',samples='mc_sherpa_zmumu',color=ROOT.kWhite,flags=['sig','mc','ewk','zmumu'])
    pz.adn(name='sig_mcnlo',label='Z #rightarrow #mu#mu',samples='mc_mcnlo_zmumu',color=ROOT.kWhite,flags=['sig','mc','ewk','zmumu'])
    pz.adn(name='sig_powheg_herwig',label='Z #rightarrow #mu#mu',samples='mc_powheg_herwig_zmumu',color=ROOT.kWhite,flags=['sig','mc','ewk','zmumu'])
    pz.adn(name='sig_powheg_pythia',label='Z #rightarrow #mu#mu',samples='mc_powheg_pythia_zmumu',color=ROOT.kWhite,flags=['sig','mc','ewk','zmumu'])
    pz.adn(name='sig_alpgen_herwig',label='Z #rightarrow #mu#mu+jets',samples=['mc_alpgen_herwig_zmumu_np%d'%v for v in range(6)],color=ROOT.kWhite,flags=['sig','mc','ewk','zmumu'])
    pz.adn(name='sig_alpgen_pythia',label='Z #rightarrow #mu#mu+jets',samples=['mc_alpgen_pythia_zmumu_np%d'%v for v in range(6)],color=ROOT.kWhite,flags=['sig','mc','ewk','zmumu'])
    if opts.bgsig in MAP_BGSIG.keys():
        pz.choose_sig(opts.bgsig)
    else:
        print MAP_BGSIG
        assert False,'Unknown bgsig option: %s'%opts.bgsig

# Pre-load ntuples
path_truth = 'truth/st_%s_final/ntuple'%opts.ntuple
path_reco  = 'Nominal/st_%s_final/ntuple'%opts.ntuple
for it,px in enumerate((pw,pz)):
    ptruth = 'truth/st_%s_final/ntuple'%('w' if it==0 else 'z')
    preco  = 'Nominal/st_%s_final/ntuple'%('w' if it==0 else 'z')
    px.add(name='2011 data',samples=['data_period%s'%s for s in _DATA_PERIODS],color=ROOT.kBlack,flags=['data'])
    px.addchain(ptruth)
    px.addchain(preco)
    px.auto()

# select main ntuple
po = None
if opts.ntuple=='z':
    po = pz
elif opts.ntuple=='w':
    po = pw
else:
    assert False,'Unsupported --ntuple: %s'%opts.ntuple

if False:
    po.print_counts()

gbg = []
q = opts.charge

# Reco-level [histo]
unfmethod = 'RooUnfoldBinByBin'
#unfmethod = 'RooUnfoldBayes'
tightlvl = ''
#tightlvl = 'loose_'
#jetlvl = 'CalJet'
jetlvl = ''
spR = SuPlot()
if True:
    spR.bootstrap(do_unfold=False,
                  unfold={'sysdir':tightlvl+'Nominal'+jetlvl,'histo':'abseta','mc':MAP_BGSIG[opts.bgsig],'method':unfmethod,'par':4},
                  charge=q,var=opts.var,histo=opts.hsource,
                  sysdir=[tightlvl+'Nominal'+jetlvl,tightlvl+'Nominal'+jetlvl,opts.isofail+jetlvl],subdir='st_w_final',basedir='baseline',
                  qcd={'var':'met','nbins':100,'min':0,'max':100,'metfit':'metfit','wmtfit':'wmtfit','forcenominal':False})
else:
    # rawmet
    spR.bootstrap(do_unfold=False,
                  unfold={'sysdir':tightlvl+'Rawmet'+jetlvl,'histo':'abseta','mc':MAP_BGSIG[opts.bgsig],'method':unfmethod,'par':4},
                  charge=q,var=opts.var,histo=opts.hsource,
                  sysdir=[tightlvl+'Rawmet'+jetlvl,tightlvl+'Rawmet'+jetlvl,opts.isofail+jetlvl],subdir='st_w_final',basedir='baseline',
                  qcd={'var':'met','nbins':100,'min':0,'max':100,'metfit':'metfit','wmtfit':'wmtfit','forcenominal':False})
    
SuStack.QCD_SYS_SCALES = opts.metallsys
SuStack.QCD_TF_FITTER = True
SuStack.QCD_STAT_HACK = True
SuStack.QCD_EXC_ZERO_BINS = 1 #FIXME
spR.enable_all()
# Reco-level [ntuple]
spRN = SuPlot()
spRN.bootstrap(ntuple=opts.ntuple,histo=opts.hsource,
               charge=q,var=opts.var,path=path_reco,bin=opts.bin,
               weight=opts.cut,pre=opts.pre,
               qcd={'var':'met','nbins':100,'min':0,'max':100,'metfit':'metfit','wmtfit':'wmtfit','forcenominal':False})
spRN.enable_nominal()
# Truth-level [histo]
spT= SuPlot()
spT.bootstrap(charge=q,var=opts.var,histo=opts.hsource,
              sysdir='truth',subdir='st_w_final',basedir='baseline')
spT.enable_nominal()
# Truth-level [ntuple]
spTN = SuPlot()
spTN.bootstrap(ntuple=opts.ntuple,histo=opts.hsource,
               charge=q,var=opts.var,path=path_truth,bin=opts.bin,
               weight=opts.cut,pre=fortruth(opts.pre))
spTN.enable_nominal()

def plot_any(spR2,spT2=None,m=2,var='lepton_absetav',do_errorsDA=False,do_errorsMC=False,do_unfold=False,do_data=True,do_mc=True,do_ratios=False,do_summary=False,new_scales=None,name=''):
    """ Plots histograms with multiple Monte-Carlos overlayed
    m = 1   :  data
    m = 2   :  data_sub
    m = 10  :  asym_data
    m = 20  :  asym_data_sub
    """
    assert m in (1,2,10,20)
    is_asym = m in (10,20)
    assert opts.ntuple=='w','ERROR: asymmetry can only be computed for the w ntuple'
    if new_scales!=None: SuStackElm.new_scales = new_scales
    if spR2 and var!=None: spR2.update_var( var )
    if spT2 and var!=None: spT2.update_var( var )
    c = SuCanvas('P%d_%s_%s'%(m,name,'det' if do_unfold==False else 'unf'))
    M = PlotOptions()
    if do_mc:
        M.prefill_mc(err=do_errorsMC if do_unfold==False else False) # if unfolded, only show errors on final data
    if do_data:
        M.prefill_data(err=do_errorsDA)
    if not do_ratios:
        M.disable_ratios() #this should be applied for distributions studies
    h = []
    for i in range(M.ntot()):
        if do_data and i==M.ntot()-1: #data
            if m==1:
                h.append( po.data('data',spR2.clone(do_unfold=do_unfold)) )
            elif m==2:
                h.append( po.data_sub('data',spR2.clone(do_unfold=do_unfold)) )
            elif m==10:
                h.append( po.asym_data('data',spR2.clone(do_unfold=do_unfold)) )
            elif m==20:
                h.append( po.asym_data_sub('data',spR2.clone(do_unfold=do_unfold)) )
        elif do_mc: #MC: use truth-level histograms if possible
            if m in (1,2):
                h.append( po.mc('mc', spT2.clone() if spT2 else spR2.clone() , name=M.names[i]) )
            elif m in (10,20):
                h.append( po.asym_mc('mc', spT2.clone() if spT2 else spR2.clone() , name=M.names[i]) )
        else:
            assert False, 'ERROR: this can never happen'
    height = 'asym' if is_asym else 1.7
    title = var
    if is_asym:
        title='Detector-level asymmetry' if do_unfold==False else 'Born-level asymmetry'
    c.plotAny(h,M=M,height=height,title=title)
    if do_summary:
        h[-1].summary_bin(fname='index_%s_%s'%(opts.tag,name))
        pass
    OMAP.append(c)
    return h[-1]

LABELMAP = {}
LABELMAP['lepton_etav'] = ['#eta',None]
LABELMAP['l_eta'] = ['#eta',None]
LABELMAP['lP_eta'] = ['#eta',None]
LABELMAP['lN_eta'] = ['#eta',None]
LABELMAP['lepton_etavHP'] = ['#eta',None]
LABELMAP['lepton_absetav'] = ['|#eta|',None]
LABELMAP['fabs(l_eta)'] = ['|#eta|',None]
LABELMAP['fabs(lP_eta)'] = ['|#eta|',None]
LABELMAP['fabs(lN_eta)'] = ['|#eta|',None]
LABELMAP['wmt'] = ['m_{T}^{W}','GeV']
LABELMAP['w_mt'] = ['m_{T}^{W}','GeV']
LABELMAP['met'] = ['E_{T}^{Miss}','GeV']
LABELMAP['lpt'] = ['p_{T}','GeV']
LABELMAP['l_pt'] = ['p_{T}','GeV']
LABELMAP['lP_pt'] = ['p_{T}','GeV']
LABELMAP['lN_pt'] = ['p_{T}','GeV']
LABELMAP['wpt'] = ['p_{T}^{W}','GeV']
LABELMAP['w_pt'] = ['p_{T}^{W}','GeV']
LABELMAP['nvtxs_all'] = ['Number of vertices',None]
LABELMAP['lepton_phi'] = ['#phi',None]
LABELMAP['l_phi'] = ['#phi',None]

def plot_stack(spR2,var,bin=None,q=2,m=0,new_scales=None,pave=False,name=''):
    if new_scales!=None: SuStackElm.new_scales = new_scales
    spR2.update_var( var , bin )
    c = SuCanvas('stack_'+var+'_'+SuSys.QMAP[q][1]+('_'+name if name !='' else ''))
    leg = []
    hstack = po.stack('mc',spR2.clone(q=q),leg=leg)
    hdata = po.data('data',spR2.clone(q=q),leg=leg)
    xaxis_info = LABELMAP[var] if var in LABELMAP else None
    c.plotStack(hstack,hdata,mode=m,leg=leg,height=2.0,pave=pave,rebin=opts.rebin,xaxis_info=xaxis_info)
    OMAP.append(c)
    return hdata,hstack

def plot_stacks(spR2,histos,bin=None,m=0,new_scales=None,name='',qs=(0,1,2)):
    """ A wrapper to make multiple stack plots for variables listed in histos[] array """
    for q in qs:
        for var in histos:
            plot_stack(spR2,var,bin=None,q=q,m=m,new_scales=new_scales,name=name)

def test_unfolding(spR2,spT2,asym=True,name='test_unfolding'):
    """ tests unfolding on one signal monte-carlo """
    #SuSample.debug = True
    #spR2.enable_nominal()
    var = 'lepton_absetav'
    spR2.update_var( var )
    spT2.update_var( var )
    c = SuCanvas(name)
    h = []
    if asym:
        h.append( po.asym_mc('h',spT2.clone(q=0),name='sig_pythia') )
        h.append( po.asym_mc('h',spR2.clone(q=0),name='sig_pythia') )
        h.append( po.asym_mc('h',spR2.clone(q=0,do_unfold=True),name='sig_pythia') )
    else:
        h.append( po.mc('h',spT2.clone(q=1),name='sig_pythia') )
        h.append( po.mc('h',spR2.clone(q=1),name='sig_pythia') )
        h.append( po.mc('h',spR2.clone(q=1,do_unfold=True),name='sig_pythia') )
    M = PlotOptions()
    M.add('truth','Pythia truth',color=1)
    M.add('reco','Pythia reco',color=2)
    M.add('unfold','Pythia unfold',color=3)
    c.plotMany(h,M=M,mode=0,height=1.6)
    OMAP.append(c)
    if False:
        hpos_u = po.sig('pos',spR.clone(q=0,do_unfold=True))
        hpos_tru = po.sig('pos',spT.clone(q=0,do_unfold=False))

def test_from_slices(spR2,spT2,mode=1,name='test_slices'):  # test: reconstruction in eta slices
    c = SuCanvas(name)
    h1,h2,h3=None,None,None
    mode=100
    if mode==0:
        h1 = po.sig('pos',spT2.clone(q=0,do_unfold=False))
        h2 = po.sig('pos',spT2.clone(q=0,do_unfold=False,histo='d2_abseta_lpt:y:0:8',sliced_2d=True))
        h = [h1,h2]
    elif mode==1:
        h1 = po.data('pos',spR2.clone(q=0,do_unfold=False))
        h2 = po.data('pos',spR2.clone(q=0,do_unfold=False,histo='bin_%d/lepton_pt:0:8',sliced_1d=True))
        h3 = po.data('pos',spR2.clone(q=0,do_unfold=False,histo='d2_abseta_lpt:y:0:8',sliced_2d=True))
        h = [h1,h2,h3]
    elif mode==2:
        h1 = po.sig('pos',spR2.clone(q=0,do_unfold=False))
        h2 = po.sig('pos',spR2.clone(q=0,do_unfold=False,histo='d2_abseta_lpt:y:0:8',sliced_2d=True))
        h = [h1,h2]
    elif mode==10:
        SuStackElm.new_scales = False
        h1 = po.data_sub('pos',spR2.clone(q=0,do_unfold=False))
        h2 = po.data_sub('pos',spR2.clone(q=0,do_unfold=False,histo='d2_abseta_lpt:y:0:8',sliced_2d=True))
        h = [h1,h2]
    elif mode==100:
        SuStackElm.new_scales = False
        spR2.enable_nominal()
        h1 = po.data_sub('pos',spR2.clone(q=0,do_unfold=True))
        h2 = po.data_sub('pos',spR2.clone(q=0,do_unfold=True,histo='d2_abseta_lpt:y:0:8',sliced_2d=True))
        h = [h1,h2]
    else:
        assert False,'Unsupported test_from_slices mode'
    M = PlotOptions()
    if len(h)>=2:
        M.add('default','Default',size=1.2)
    if len(h)>=3:
        M.add('fromslices1d','From slices - 1D',size=0.9)
    M.add('fromslices2d','From slices - 2D',size=0.5)
    c.plotAny(h,M=M,height=1.7)
    #c.plotOne(h1,mode=2,height=1.7)
    OMAP.append(c)

def test_ntuple_histo(spR2,var='lepton_absetav',new_scales=None,name='ntuple_histo'):
    assert opts.ntuple=='w','ERROR: asymmetry can only be computed for the w ntuple'
    if new_scales!=None: SuStackElm.new_scales = new_scales
    if spR2: spR2.update_var( var )
    c = SuCanvas('%s_%s'%(name,'detector'))
    M = PlotOptions()
    M.prefill_mc()
    M.prefill_data()
    h = []
    for i in range(M.ntot()):
        if i==M.ntot()-1: #data
            h.append( po.asym_data_sub('data',spR2.clone()) )
        else: #MC
            h.append( po.asym_mc('mc', spR2.clone() ,name=M.names[i]) )
    c.plotAny(h,M=M,height='asym',title='Ntuple-Histo comparison')
    #c.plotAny(h,M=M,height=4000*1000,title='Ntuple-Histo comparison')
    OMAP.append(c)

def june26_asymmetry_all_slices():
    if True: # FINAL asymmetry in slices of various variables
        plot_any(spR.clone(),spT.clone(),var=None,m=20,do_unfold=True,do_errorsDA=True,do_summary=True,name='INCLUSIVE_DIRECT')
        if opts.extra=='1':
            histo = 'bin_%d/lepton_pt:0:8'
            plot_any(spR.clone(histo=histo,sliced_1d=True),spT.clone(histo=histo,sliced_1d=True),var=None,m=20,do_unfold=True,do_errorsDA=True,do_summary=True,name='INCLUSIVE_SLICES')
            histo = 'bin_%d/lepton_pt:1:1'
            plot_any(spR.clone(histo=histo,sliced_1d=True),spT.clone(histo=histo,sliced_1d=True),var=None,m=20,do_unfold=True,do_errorsDA=True,do_summary=True,name='LEPTON_PT2025')
            histo = 'bin_%d/lepton_pt:1:2'
            plot_any(spR.clone(histo=histo,sliced_1d=True),spT.clone(histo=histo,sliced_1d=True),var=None,m=20,do_unfold=True,do_errorsDA=True,do_summary=True,name='LEPTON_PT2040')
            histo = 'bin_%d/lepton_pt:3:3'
            plot_any(spR.clone(histo=histo,sliced_1d=True),spT.clone(histo=histo,sliced_1d=True),var=None,m=20,do_unfold=True,do_errorsDA=True,do_summary=True,name='LEPTON_PT4080')
        elif opts.extra=='2':
            histo = 'bin_%d/wpt:1:1'
            plot_any(spR.clone(histo=histo,sliced_1d=True),spT.clone(histo=histo,sliced_1d=True),var=None,m=20,do_unfold=True,do_errorsDA=True,do_summary=True,name='WPT0010')
            histo = 'bin_%d/wpt:2:2'
            plot_any(spR.clone(histo=histo,sliced_1d=True),spT.clone(histo=histo,sliced_1d=True),var=None,m=20,do_unfold=True,do_errorsDA=True,do_summary=True,name='WPT1020')
            histo = 'bin_%d/wpt:3:3'
            plot_any(spR.clone(histo=histo,sliced_1d=True),spT.clone(histo=histo,sliced_1d=True),var=None,m=20,do_unfold=True,do_errorsDA=True,do_summary=True,name='WPT20200')
        elif opts.extra=='3':
            histo = 'bin_%d/wmt:1:1'
            plot_any(spR.clone(histo=histo,sliced_1d=True),spT.clone(histo=histo,sliced_1d=True),var=None,m=20,do_unfold=True,do_errorsDA=True,do_summary=True,name='WMT4060')
            histo = 'bin_%d/wmt:2:2'
            plot_any(spR.clone(histo=histo,sliced_1d=True),spT.clone(histo=histo,sliced_1d=True),var=None,m=20,do_unfold=True,do_errorsDA=True,do_summary=True,name='WMT6080')
            histo = 'bin_%d/wmt:3:3'
            plot_any(spR.clone(histo=histo,sliced_1d=True),spT.clone(histo=histo,sliced_1d=True),var=None,m=20,do_unfold=True,do_errorsDA=True,do_summary=True,name='WMT80200')
        elif opts.extra=='4':
            histo = 'bin_%d/met:1:1'
            plot_any(spR.clone(histo=histo,sliced_1d=True),spT.clone(histo=histo,sliced_1d=True),var=None,m=20,do_unfold=True,do_errorsDA=True,do_summary=True,name='MET2540')
            histo = 'bin_%d/met:2:2'
            plot_any(spR.clone(histo=histo,sliced_1d=True),spT.clone(histo=histo,sliced_1d=True),var=None,m=20,do_unfold=True,do_errorsDA=True,do_summary=True,name='MET40200')
        elif opts.extra=='5':
            histo = 'bin_%d/njets:1:1'
            plot_any(spR.clone(histo=histo,sliced_1d=True),spT.clone(histo=histo,sliced_1d=True),var=None,m=20,do_unfold=True,do_errorsDA=True,do_summary=True,name='NJETS0')
            histo = 'bin_%d/njets:2:2'
            plot_any(spR.clone(histo=histo,sliced_1d=True),spT.clone(histo=histo,sliced_1d=True),var=None,m=20,do_unfold=True,do_errorsDA=True,do_summary=True,name='NJETS1')
            histo = 'bin_%d/njets:3:3'
            plot_any(spR.clone(histo=histo,sliced_1d=True),spT.clone(histo=histo,sliced_1d=True),var=None,m=20,do_unfold=True,do_errorsDA=True,do_summary=True,name='NJETS2')
            histo = 'bin_%d/njets:4:5'
            plot_any(spR.clone(histo=histo,sliced_1d=True),spT.clone(histo=histo,sliced_1d=True),var=None,m=20,do_unfold=True,do_errorsDA=True,do_summary=True,name='NJETS3UP')

def july02_summarize_qcd_fits(fitvar,fitrange):
    """ DEPRECATED: now we save fit results in a pickle file and use a separate script to post-process it """
    fitreg = 'metfit' if fitvar=='met' else 'baseline'
    assert len(fitrange)==2
    #TEST_Q3S2X5Y5Z5_isofail__tight_nominal_st_w_final_metfit_bin_8_lepton_pt_4_NEG_met_0to100.png
    #TEST_Q3S5X5Y5Z5_loose_isofail__nominal_st_w_final_metfit_bin_0_lepton_pt_4_POS_met_0to100.png
    n = 'Q3S%dX5Y5Z5_'%opts.bgsig+'loose_isofail__nominal_st_w_final_' + fitreg + '_bin_%d_lepton_pt_%d_%s_' + fitvar + '_' + str(fitrange[0]) + 'to' + str(fitrange[1])
    f = open('qcd.html','w')

    print >>f,'<HTML><BODY>'
    for iq in ('POS','NEG'):
        print >>f,'<HR>'
        print >>f,iq
        print >>f,'<HR>'
        #print table
        print >>f,'<TABLE border="1" width="900">'
        print >>f,'<TR>'
        print >>f, '<TD width="100">pT/eta</TD>'
        for ipt in xrange(0,len(ptbins)-1):
            print >>f,'<TD width="50">','%d&lt;pT&lt;%d'%(ptbins[ipt],ptbins[ipt+1]),'</TD>'
        print >>f,'</TR>'
        for ieta in xrange(0,len(absetabins)-1):
            print >>f,'<TR>'
            print >>f, '<TD width="100">','%.2f&lt;|eta|&lt;%.2f'%(absetabins[ieta],absetabins[ieta+1]),"</TD>"
            for ipt in xrange(0,len(ptbins)-1):
                key = n%(ieta,ipt,iq)
                v = -1.0
                if key in po.scales:
                    v = po.scales[key][0]  #scale factor
                    v = po.scales[key][2]*100.0  #fraction
                print >>f,'<TD width="50">','%.1f%%'%(v),'</TD>'
            print >>f,'</TR>'
        print >>f,'</TABLE>'
        # print images
        print >>f,'<BR>'
        print >>f,'<TABLE border="0" width="2240">'
        NF='http://www.wzone.com/myimages/PageNotFound-Man.jpg'
        for ieta in xrange(0,len(absetabins)-1):
            print >>f,'<TR>'
            for ipt in xrange(0,len(ptbins)-1):
                key = n%(ieta,ipt,iq)
                print >>f,'<TD width="280" align="center"><img src="TEST/%s_%s.png" width="270"/></TD>'%(opts.tag,SuCanvas.cleanse(key))
            print >>f,'</TR>'
        print >>f,'</TABLE>'
    print >>f,'</BODY></HTML>'
    f.close()

def study_jet_calibration_effects():
    if True:   # manually plot MET shapes to study jet calibratione effects
        spR.enable_nominal()
        VARMAP = {}
        VARMAP['met'] = 'Missing ET'
        VARMAP['lpt'] = 'Muon p_{T}'
        VARMAP['lepton_abseta_fine'] = 'Muon |#eta|'
        for var in ('met','lpt','lepton_abseta_fine'):
            spR.update_var( var )
            c = SuCanvas('JetCal_data_%s'%var)
            h = []
            M = PlotOptions()
            M.add('default','MET from default jets',size=0.4)
            M.add('calib','MET from calibrated jets',size=0.2)
            h.append( po.data('data',spR.clone(q=2,sysdir='nominal')) )
            h.append( po.data('data',spR.clone(q=2,sysdir='nominal_caljet')) )
            c.plotAny(h,M=M,height=1.7,title='Legend',xtitle=VARMAP[var])
            OMAP.append(c)
        for var in ('met','lpt','lepton_abseta_fine'):
            spR.update_var( var )
            po.choose_sig(5)
            c = SuCanvas('JetCal_mc_%s'%var)
            h = []
            M = PlotOptions()
            M.add('default','MET from default jets',size=0.4)
            M.add('calib','MET from calibrated jets',size=0.2)
            #M.add('default','MET from smeared jets',size=0.2)
            M.add('default','MET from JES+UP jets',size=0.2)
            M.add('default','MET from JES+DOWN jets',size=0.2)
            h.append( po.sig('sig',spR.clone(q=2,sysdir='nominal')) )
            h.append( po.sig('sig',spR.clone(q=2,sysdir='nominal_caljet')) )
            #h.append( po.sig('sig',spR.clone(q=2,sysdir='jet_jer')) )
            h.append( po.sig('sig',spR.clone(q=2,sysdir='jet_jesup')) )
            h.append( po.sig('sig',spR.clone(q=2,sysdir='jet_jesdown')) )
            c.plotAny(h,M=M,height=1.7,title='Legend',xtitle=VARMAP[var])
            OMAP.append(c)

# combined plots
if mode=='ALL' or mode=='all':
    if False:
        plots = ['lepton_absetav','lpt','met','wmt']
        plots = [opts.hsource,]
        plot_stacks(spR.clone(),plots,m=1,qs=(0,1,2))
    if False:
        spR.enable_nominal()
        plots = [opts.hsource,]
        plot_stacks(spR.clone(),plots,m=1,qs=(0,1))
    if False: # inclusive reco-level and truth-level asymmetry
        plot_any(spR.clone(q=0),spT.clone(q=0),m=2,do_unfold=True,do_errorsDA=True,do_summary=True,name='POS')
        plot_any(spR.clone(q=1),spT.clone(q=1),m=2,do_unfold=True,do_errorsDA=True,do_summary=True,name='NEG')
        plot_any(spR.clone(),spT.clone(),m=20,do_unfold=True,do_errorsDA=True,do_summary=True,name='ASY')
        plot_any(spR.clone(),None,m=20,do_unfold=False,do_errorsDA=True,do_errorsMC=True,do_summary=False)
        if False: # validate TH1 vs ntuple MC-only asymmetries. Small difference see in truth tree -not sure why
            plot_any(spRN.clone(path=path_reco),None,m=20,name='reco_ntuple',do_data=False,new_scales=False)
            plot_any(spR.clone(),None,name='reco_histo',m=20,do_data=False,new_scales=False)
            plot_any(spTN.clone(),None,name='truth_ntuple',m=20,do_data=False,new_scales=False)
            plot_any(spT.clone(),None,name='truth_histo',m=20,do_data=False,new_scales=False)
    if False: # studying differences in generators - RECO LEVEL
        po.choose_qcd(0)
        SuCanvas._refLineMin = 0.8
        SuCanvas._refLineMax = 1.2
        x= ''
        ie = 99
        if opts.extra!=None:
            ie = int(opts.extra)
            x = ' && fabs(l_eta)>=%.2f && fabs(l_eta)<=%.2f'%(absetabins[ie],absetabins[ie+1])
            print 'Limiting eta:',x
        plots = [ ]
        #plots.append( ('fabs(l_eta)',None,opts.pre+x) )
        plots.append( ('l_pt','100,20,200',prunesub((opts.pre),'l_pt','l_pt>20 && l_pt<200')+x) )
        plots.append( ('met','100,0,200',prunesub((opts.pre),'met','met>0 && met<200')+x) )
        plots.append( ('w_mt','100,0,200',prunesub((opts.pre),'w_mt','w_mt>0 && w_mt<200')+x) )
        plots.append( ('w_pt','100,0,200',opts.pre+x) )
        for bla in plots:
            var=bla[0]
            bin=bla[1]
            pre=bla[2]
            plot_any(spRN.clone(path=path_reco,var=var,pre=pre,bin=bin),None,m=2,var=None,name='reco_'+var,do_data=False,do_ratios=True,new_scales=False)
    if False: # studying differences in generators - TRUTH LEVEL
        po.choose_qcd(0)
        SuCanvas._refLineMin = 0.8
        SuCanvas._refLineMax = 1.2
        x= ''
        ie = 99
        if opts.extra!=None:
            ie = int(opts.extra)
            x = ' && fabs(l_eta)>=%.2f && fabs(l_eta)<=%.2f'%(absetabins[ie],absetabins[ie+1])
            print 'Limiting eta:',x
        plots = [ ]
        #plots.append( ('fabs(l_eta)',None,fortruth(opts.pre)+x) )
        plots.append( ('l_pt','100,20,200',prunesub(fortruth(opts.pre),'l_pt','l_pt>20 && l_pt<200')+x) )
        plots.append( ('met','100,0,200',prunesub(fortruth(opts.pre),'met','met>0 && met<200')+x) )
        plots.append( ('w_mt','100,0,200',prunesub(fortruth(opts.pre),'w_mt','w_mt>0 && w_mt<200')+x) )
        plots.append( ('w_pt','100,0,200',fortruth(opts.pre)+x) )
        plots.append( ('cos3d','100,-1.0,1.0',fortruth(opts.pre)+x) )
        for bla in plots:
            var=bla[0]
            bin=bla[1]
            pre=bla[2]
            weight = "mcw*puw*wptw*wpolw*vxw"
            plot_any(spTN.clone(var=var,pre=pre,bin=bin,weight=weight),None,m=2,var=None,name='WALL_reco_'+var,do_data=False,do_ratios=True,new_scales=False)
            if True:
                weight = "mcw*puw*wptw*wpolw"
                plot_any(spTN.clone(var=var,pre=pre,bin=bin,weight=weight),None,m=2,var=None,name='WNOZ_reco_'+var,do_data=False,do_ratios=True,new_scales=False)
                weight = "mcw*puw"
                plot_any(spTN.clone(var=var,pre=pre,bin=bin,weight=weight),None,m=2,var=None,name='WNON_reco_'+var,do_data=False,do_ratios=True,new_scales=False)
                weight = "mcw*puw*wptw"
                plot_any(spTN.clone(var=var,pre=pre,bin=bin,weight=weight),None,m=2,var=None,name='WWPT_reco_'+var,do_data=False,do_ratios=True,new_scales=False)
                weight = "mcw*puw*wpolw"
                plot_any(spTN.clone(var=var,pre=pre,bin=bin,weight=weight),None,m=2,var=None,name='WPOL_reco_'+var,do_data=False,do_ratios=True,new_scales=False)
    if False: # QCD systematic in 2d study: does |eta| slice plot to force QCD fits in |eta| x pT bins. Dumps fit scales to pickle file
        spR.enable_nominal()
        histo = 'bin_%d/lepton_pt:0:8'
        qcdadds = []
        qcdadds.append( {'var':'met','min':0,'max':80} )
        qcdadds.append( {'var':'wmt','min':40,'max':90} )
        for bgsig in (1,4,5):
            po.choose_sig(bgsig)
            for iqcd,qcdadd in enumerate(qcdadds):
                plot_any(spR.clone(histo=histo,qcdadd=qcdadd,sliced_1d=True),None,var=None,m=20,do_unfold=False,do_errorsDA=True,do_errorsMC=True,do_summary=False,name='SLICES_sig%d_%d'%(bgsig,iqcd))
        # dump all stats into a pickle file
        dump_pickle(po.scales)
    if False: # reconstruction in |eta| slices + QCD fits in |eta| x pT bins
        spR.enable_nominal()
        plot_any(spR.clone(),None,var=None,m=20,do_unfold=False,do_errorsDA=True,do_errorsMC=True,do_summary=False,name='INCLUSIVE_DIRECT')
        histo = 'bin_%d/lepton_pt:0:8'
        qcdadd={'var':'met','min':0,'max':80}
        #qcdadd={'var':'wmt','min':40,'max':90}
        if True:
            plot_any(spR.clone(histo=histo,sliced_1d=True,qcdadd=qcdadd),None,var=None,m=20,do_unfold=False,do_errorsDA=True,do_errorsMC=True,do_summary=False,name='INCLUSIVE_SLICES')
            july02_summarize_qcd_fits(qcdadd['var'],(qcdadd['min'],qcdadd['max']))
    if False: # simple histogram comparison of TH1 and ntuple-based histograms: one MC and Data only
        c = SuCanvas('TEST')
        M = PlotOptions()
        M.add('h','histo',ratio=1,size=1.0)
        M.add('nt','ntuple',ratio=1,size=0.5)
        h = []
        pre = 'ptiso40/l_pt<0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1'
        SuSample.debug = True
        #SuSample.GLOBAL_CACHE = None
        spR.enable_nominal()
        weight = "mcw*puw*effw*trigw*isow"
        weight += "*wptw*wzptw*wpolw*vxw*ls1w*ls2w"
        SuCanvas._refLineMin = 0.95
        SuCanvas._refLineMax = 1.05
        if False:
            h.append( po.data('mcd',spR.clone(q=2)) )
            h.append( po.data('mcd',spRN.clone(q=2,pre=pre,weight=weight)) )
        else:
            h.append( po.mc('mc',spR.clone(histo='lpt',bin='200,0,200',q=2),name='sig_pythia') )
            h.append( po.mc('mc',spRN.clone(var='l_pt',bin='200,0,200',q=2,pre=pre,weight=weight),name='sig_pythia') )
        print 'NBINS:',h[-2].nominal().h.GetNbinsX(),h[-1].nominal().h.GetNbinsX()
        c.plotAny(h,M=M,height=1.6,title='Ntuple-Histo comparison')
        OMAP.append(c)
    if False: # bg-subtracted asymmetry comparison of TH1 vs ntuple based basic histogram. Ignores QCD normalization issues
        SuSample.debug = True
        spR.enable_nominal()
        po.choose_qcd(0)
        pre = 'ptiso20/l_pt<0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1 && l_trigEF<0.2'
        test_ntuple_histo(spR.clone(),name='asym_histo',new_scales=False)
        test_ntuple_histo(spRN.clone(path=path_reco,pre=pre),name='asym_ntuple_all',new_scales=False)
        if False: # study effect of d0 cuts
            newpre=opts.pre + ' && ' + 'fabs(d0sig)<5.0'
            test_ntuple_histo(spRN.clone(path=path_reco,pre=newpre),name='asym_ntuple_d05',new_scales=False)
            newpre=opts.pre + ' && ' + 'fabs(d0sig)<10.0'
            test_ntuple_histo(spRN.clone(path=path_reco,pre=newpre),name='asym_ntuple_d10',new_scales=False)
    if False: # rudimentary QCD studies: comparing effects of fit range
        plots = ['met']
        spR.enable_nominal()
        bgsig = 2
        bgqcd = 3
        po.choose_sig(bgsig)
        po.choose_qcd(bgqcd)
        #plot_stacks(spR.clone(),plots,m=0,name='SIG_%s__QCD_%s'%(MAP_BGSIG[bgsig],MAP_BGQCD[bgqcd]))
        res = []
        def q(n,qcdadd):
            d = plot_any(spR.clone(qcdadd=qcdadd),None,m=20,do_unfold=False,do_mc=False,do_summary=False,name=n)
            res.append( (n,d.nominal_h().GetBinContent(1),d.nominal_h().GetBinContent(6),d.nominal_h().GetBinContent(11)) )
        q('met0to100',{'var':'met','min':0,'max':100})
        q('met5to100',{'var':'met','min':5,'max':100})
        q('met10to100',{'var':'met','min':10,'max':100})
        q('met25to100',{'var':'met','min':25,'max':100})
        q('met0to200',{'var':'met','min':0,'max':200})
        q('met25to200',{'var':'met','min':25,'max':200})
        q('lpt20to100',{'var':'lpt','min':20,'max':100})
        q('lpt25to100',{'var':'lpt','min':25,'max':100})
        q('lpt20to200',{'var':'lpt','min':20,'max':200})
        q('wmt40to100',{'var':'wmt','min':40,'max':100})
        q('wmt40to200',{'var':'wmt','min':40,'max':200})
        print res
    if False: # rudimentary QCD studies: comparing various template sources (both QCD and EWK)
        plots = ['lepton_absetav','lpt','met','wmt']
        plot_sys = False
        #Note: despite enable_nominal, we are seeing systematic bands is m=1 (must be stat. error on BG - but are we double-counting?)
        spR.enable_nominal()
        qcdadds=[]
        qcdadds.append( {'var':'met','min':0,'max':100} )
        qcdadds.append( {'var':'wmt','min':40,'max':100} )
        for bgewk in (2,5):
            po.choose_ewk(bgewk)
            for bgsig in (1,2,4,5):
                po.choose_sig(bgsig)
                for bgqcd in (3,):
                    po.choose_qcd(bgqcd)
                    print 'Working on:','SIG:',MAP_BGSIG[bgsig],'QCD:',MAP_BGQCD[bgqcd],'EWK:',MAP_BGSIG[bgewk]
                    for iqcdadd,qcdadd in enumerate(qcdadds):
                        plot_stacks(spR.clone(qcdadd=qcdadd),plots,m=1 if plot_sys==True else 0, name='%s_qcd%d'%(po.get_flagsum(),iqcdadd) )
    if False: # mel test: look at systematic variations in signal only, see if it is one-sided
        spR.update_var( 'lepton_absetav' )
        c = SuCanvas('systematics')
        if False: # in asymmetry
            h = po.asym_sig('asym',spR.clone(do_unfold=0))
            c.plotOne(h,mode=2,height='asym',title='Signal MC asymmetry: systematics')
        else:     # in signal
            #h = po.sig('asym',spR.clone(q=0))
            h = po.mc('asym',spR.clone(q=1),name='sig_pythia')
            c.plotOne(h,mode=2,height=1.5,title='Signal MC asymmetry: systematics')
        OMAP.append(c)
        h.summary_bin(fname='index')
    if False: # stopped working 06/19/2012. I think before it worked "almost" correctly, but now is substantially off
        test_unfolding(spR.clone(),spT.clone(),asym=False)
    if False: # make sure rebuilding of abseta from bin-by-bin slices is identical to direct histogram
        test_from_slices(spR.clone(),spT.clone(),mode=10)
    if False: # same as above, but compaing at unfolded level. I.e., this also validates direct unfolding vs pt-unfolding inside eta slices
        c = SuCanvas('test_slices_norm')
        SuStack.QCD_SYS_SCALES = False
        spR.enable_nominal()
        print '--------->', 'Making default'
        do_unfold = True
        h1R = po.asym_data_sub('pos',spR.clone(do_unfold=do_unfold))
        print '--------->', 'Making inbins combined'
        h2R = po.asym_data_sub('pos',spR.clone(do_unfold=do_unfold,histo='bin_%d/lepton_pt:0:8',sliced_1d=True))
        hpt = []
        print '--------->', 'Making inbins pt20..25'
        #hpt.append( po.asym_data_sub('pos',spR.clone(do_unfold=False,histo='bin_%d/lepton_pt:1:1',sliced_1d=True)) )
        M = PlotOptions()
        M.add('default','default')
        M.add('inbins','inbins')
        #M.add('pt2025','pt2025')
        #M.add('pt2540','pt2540')
        #M.add('pt4080','pt4080')
        #M.add('pt80200','pt80200')
        print '--------->', 'Starting plotting...'
        c.plotMany([h1R,h2R]+hpt,M=M,mode=0,height='asym')
        #c.plotOne(h1R,mode=2,height=1.7)
        OMAP.append(c)
    if False: # test: reconstruction in eta slices, also visualizes a gazillion SCALE refits
        c = SuCanvas('test_slices_norm')
        SuStack.QCD_SYS_SCALES = True
        h1R = po.qcd('pos',spR.clone(q=0,do_unfold=False))
        #SuSample.debug = True
        #spR.enable_nominal()
        h2R = po.qcd('pos',spR.clone(q=0,do_unfold=False,histo='bin_%d/lepton_pt:0:8',sliced_1d=True))
        M = PlotOptions()
        M.add('default','default')
        M.add('inbins','inbins')
        c.plotMany([h1R,h2R],M=M,mode=0,height=1.7)
        #c.plotOne(h1R,mode=2,height=1.7)
        OMAP.append(c)
    if False: # antonio study of muon exms_pt/id_pt in low-pt muon region. refLine = 0.0,3.0
        plot_stack(spRN.clone(var='l_pt_exms/l_pt_id',histo='',bin='50,0.5,1.5',q=2),var='l_pt_exms/l_pt_id',q=2,new_scales=False,name='antonio_ptEXMS_ptID')
    if False: # june17 plots without data: asymmetries at truth and a bit at reco level
        cmd="""
        ./stack2.py --input ${input} --qcd 1.0 -b --var "fabs(l_eta)" --bin 10,0.0,2.5 --hsource "lepton_absetav" -o TEST -t TEST --pre "ptiso20/l_pt<0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1 && l_trigEF<0.2" --cut "mcw*puw*effw*trigw" -m ALL --bgsig 2 --bgqcd 0
        """
        june17_asymmetry()

# saves a collection of 2D or 1D histograms to be used as input into Max's unfolding
if mode=='prepare_qcd_1d' or mode=='prepare_qcd_2d':
    var = None
    if opts.etamode==2: # |eta| bins
        var = 'd2_abseta_lpt' if mode=='prepare_qcd_2d' else 'lepton_absetav'
    elif opts.etamode==1:
        var = 'd2_eta_lpt' if mode=='prepare_qcd_2d' else 'lepton_etav'
    assert var
    # disable QCD scaling since here we are just dumping histograms
    SuStackElm.new_scales = False
    assert opts.extra
    fname = opts.extra
    itot = 0
    # prepare for nominal
    sig=5
    ewk=5
    po.choose_sig(sig)
    po.choose_ewk(ewk)
    assert opts.bgqcd in (3,4), 'Possible error: prepare_qcd should operate on bgqcd = 3 (anti-iso) or 4 (anti-iso + bg subtraction)'
    po.choose_qcd(opts.bgqcd)
    DONE = []
    # variation of ewk subtraction
    sig=5
    for ewk in (5,2,1):
        po.choose_sig(sig)
        po.choose_ewk(ewk)
        if (sig,ewk) not in DONE:
            po.SaveROOT(fname,spR.clone(q=0,histo=var,var=var),mode='RECREATE' if itot==0 else 'UPDATE',dname='POS_sig%d_ewk%d'%(sig,ewk)); itot+=1
            po.SaveROOT(fname,spR.clone(q=1,histo=var,var=var),dname='NEG_sig%d_ewk%d'%(sig,ewk));  itot+=1
        DONE.append( (sig,ewk) )
    # variation of signal
    ewk=5
    for sig in (5,4,1):
        po.choose_sig(sig)
        po.choose_ewk(ewk)
        if (sig,ewk) not in DONE:
            po.SaveROOT(fname,spR.clone(q=0,histo=var,var=var),dname='POS_sig%d_ewk%d'%(sig,ewk)); itot+=1
            po.SaveROOT(fname,spR.clone(q=1,histo=var,var=var),dname='NEG_sig%d_ewk%d'%(sig,ewk));  itot+=1
        DONE.append( (sig,ewk) )
    # variation of qcd template shape (iso-window instead of iso-reversion)
    if True:
        sig=5
        ewk=5
        po.choose_sig(sig)
        po.choose_ewk(ewk)
        sysdir_wind=[tightlvl+'Nominal'+jetlvl,tightlvl+'Nominal'+jetlvl,'IsoWind20'+jetlvl]
        po.SaveROOT(fname,spR.clone(q=0,histo=var,var=var,sysdir=sysdir_wind),dname='POS_sig%d_ewk%d_qcdwind'%(sig,ewk)); itot+=1
        po.SaveROOT(fname,spR.clone(q=1,histo=var,var=var,sysdir=sysdir_wind),dname='NEG_sig%d_ewk%d_qcdwind'%(sig,ewk)); itot+=1
    # variation of qcd template shape: monte-carlo-driven template. This is automatically pre-normalized!
    if True:
        SuStackElm.new_scales = True
        sig=5
        ewk=5
        po.choose_sig(sig)
        po.choose_ewk(ewk)
        po.choose_qcd(0)
        po.SaveROOT(fname,spR.clone(q=0,histo=var,var=var),dname='POS_sig%d_ewk%d_qcdmc'%(sig,ewk)); itot+=1
        po.SaveROOT(fname,spR.clone(q=1,histo=var,var=var),dname='NEG_sig%d_ewk%d_qcdmc'%(sig,ewk)); itot+=1
        SuStackElm.new_scales = False
        po.choose_qcd(opts.bgqcd)
    # variation of electroweak normalization
    if True:
        ewk=5
        sig=5
        po.choose_sig(sig)
        po.choose_ewk(ewk)
        SuSample.xsecerr = 1
        po.SaveROOT(fname,spR.clone(q=0,histo=var,var=var),dname='POS_sig%d_ewk%d_xsecup'%(sig,ewk)); itot+=1
        po.SaveROOT(fname,spR.clone(q=1,histo=var,var=var),dname='NEG_sig%d_ewk%d_xsecup'%(sig,ewk));  itot+=1
        SuSample.xsecerr = -1
        po.SaveROOT(fname,spR.clone(q=0,histo=var,var=var),dname='POS_sig%d_ewk%d_xsecdown'%(sig,ewk)); itot+=1
        po.SaveROOT(fname,spR.clone(q=1,histo=var,var=var),dname='NEG_sig%d_ewk%d_xsecdown'%(sig,ewk));  itot+=1

# comprehensive study of qcd fits in 2d: pt x eta bins, using histograms.
# UPD: now also supports 1D and 0D (all-inclusive) fits
if mode=='qcdfit_2d':
    spR.enable_nominal()
    #nominal/st_w_final/wmtfit/NEG/bin_8/lpt_4/wmt
    iq = opts.charge
    ieta = opts.preNN if opts.preNN=='ALL' else int(opts.preNN)
    ipt  = opts.preNQ if opts.preNQ=='ALL' else int(opts.preNQ)
    bgqcd = opts.bgqcd
    po.choose_qcd(bgqcd)
    bgsig = opts.bgsig
    po.choose_sig(bgsig)
    bgewk = opts.bgewk
    po.choose_ewk(bgewk)
    ivar = opts.lvar
    ibin = opts.lbin
    imin = int(ibin.split(',')[1])
    imax = int(ibin.split(',')[2])
    var = None
    pvar = None
    if ieta=='ALL' and ipt=='ALL':
        var  = '%s'%(ivar)
        pvar = '%s'%(ivar)
    elif (ieta!='ALL') and (ipt=='ALL'):
        var  = 'bin%s_%d/%s'%('' if opts.etamode==2 else 'e',ieta,ivar)
        pvar = 'bin%s_%d/%s'%('' if opts.etamode==2 else 'e',ieta,opts.var)
    elif (ipt!='ALL') and (ieta!='ALL'):
        var = 'bin%s_%d/lpt_%d/%s'%('' if opts.etamode==2 else 'e',ieta,ipt,ivar)
        pvar = 'bin%s_%d/lpt_%d/%s'%('' if opts.etamode==2 else 'e',ieta,ipt,opts.var)
    elif (ieta=='ALL') and (ipt!='ALL'):
        var = 'bin%s_%d/%s'%('p',ipt,ivar)
        pvar = 'bin%s_%d/%s'%('p',ipt,opts.var)
    else:
        assert False,'Unsupported ieta/ipt choice for mode qcdfit_2d.'
    qcdadd={'var':var,'min':imin,'max':imax,'rebin':2}
    hdata,hstack = plot_stack(spR.clone(qcdadd=qcdadd),var=pvar,q=iq,m=1,new_scales=True,pave=True,name=po.get_flagsum()+'_F'+ivar)
    hfrac=hstack.nominal().stack_bg_frac()
    key = po.scalekeys[-1]
    scales = po.scales[key]
    # save the results
    skey = '/iq%d/X%d/bgqcd%d/bgewk%d/bgsig%d/iso%s/ivar%s/ibin%s/%s%s/ipt%s'%(iq,opts.xsecerr,bgqcd,bgewk,bgsig,opts.isofail,ivar,ibin,'ieta' if opts.etamode==2 else 'iseta',ieta,ipt)
    DATAOUT = { 'frac' : hfrac, 'scales' : scales }
    ROOTOUT = [ OMAP[-1]._canvas , po.fits[key]._canvas ]
    a = antondb.antondb(opts.extra)
    a.add(skey,DATAOUT)
    if True:
        a.add_root(skey,ROOTOUT)
    if False:
        sys.exit(0)

# Study different MC weights.
# Also can be used to perform stack comparison of TH1 and ntuple-based histograms
if mode=='study_weights':
    spR.enable_nominal()
    po.choose_qcd(opts.bgqcd)
    SuStackElm.new_scales = True
    SuSample.debug = True
    x = '' # disable eta cut!
    if opts.func != 'gaus': # HACK: enable an eta cut
        func = int(opts.func)
        assert func>=0 and func<len(absetabins)-1,'Out of range func: %d'%func  #12
        x = ' && fabs(l_eta)>=%f && fabs(l_eta)<=%f'%(absetabins[func],absetabins[func+1])
    preNN = opts.preNN + x
    preNQ = opts.preNQ + x
    preFN = opts.preFN + x
    preFQ = opts.preFQ + x
    presN = (preNN,preNN,preNQ) # pre strings for normal plots   (e.g., nominal or anti-isolation)
    presF = (preFN,preFN,preFQ) # pre strings for QCD fit region (e.g., lowering MET cut to zero)
    #qcdadd={'var':'w_mt','min':35,'max':100,'nbins':50,'descr':'antiso20_0p1_wmt','pre':presF}
    qcdadd={'var':'met','min':0,'max':100,'nbins':50,'descr':'antiso20_0p1_met','pre':presF}
    if False: # histo-based plotting
        var='l_pt'
        var='lpt'
        plot_stack(spR.clone(),var,q=opts.charge,m=0,name='histo')
    var=opts.var
    bin = opts.bin
    assert opts.extra!=None,'EXITING - please set --extra to 0'
    extra = int(opts.extra)
    if extra==0:
        weight = "mcw*puw*effw*trigw*isow*wzptw*wpolw*vxw*ls1w*ls2w"
        qcdadd['descr'] = 'WALL'
        plot_stack(spRN.clone(pre=presN,weight=weight,var=var,bin=bin,qcdadd=qcdadd),var,q=opts.charge,m=0,name='ntuple_'+qcdadd['descr'])
    if extra==1:
        weight = "mcw*puw*effw*trigw*isow"
        qcdadd['descr'] = 'WNON'
        plot_stack(spRN.clone(pre=presN,weight=weight,var=var,bin=bin,qcdadd=qcdadd),var,q=opts.charge,m=0,name='ntuple_'+qcdadd['descr'])
    if extra==2:
        weight = "mcw*puw*effw*trigw*isow*wzptw*vxw*ls1w*ls2w"
        qcdadd['descr'] = 'WALL_NOPOL'
        plot_stack(spRN.clone(pre=presN,weight=weight,var=var,bin=bin,qcdadd=qcdadd),var,q=opts.charge,m=0,name='ntuple_'+qcdadd['descr'])
    if extra==3:
        weight = "mcw*puw*effw*trigw*isow*wzptw2*wpolw*vxw*ls1w*ls2w"
        qcdadd['descr'] = 'WALL_PYTHIA10'
        plot_stack(spRN.clone(pre=presN,weight=weight,var=var,bin=bin,qcdadd=qcdadd),var,q=opts.charge,m=0,name='ntuple_'+qcdadd['descr'])
    if extra==4:
        weight = "mcw*puw*effw*trigw*isow*wzptw3*wpolw*vxw*ls1w*ls2w"
        qcdadd['descr'] = 'WALL_SHERPA'
        plot_stack(spRN.clone(pre=presN,weight=weight,var=var,bin=bin,qcdadd=qcdadd),var,q=opts.charge,m=0,name='ntuple_'+qcdadd['descr'])


# DEPRECATED: ntuple version of QCD fits in one dimension
if mode=='qcdfit_1d_nt' or mode=='qcdfit_1d_nt_summary': # to study QCD fits
    cmd="""
    ./stack2.py --input /share/t3data3/antonk/ana/ana_v29G_07102012_DtoM_jerjes_wptw_stacoCB_all -b --charge 0 --lvar met --lbin 50,0,120 --preNN "ptiso40/l_pt<0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1 && l_trigEF<0.2" --preNQ "ptiso40/l_pt>0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons<2 && l_trigEF<0.2" --var 'fabs(l_eta)' --bin 10,0,2.5 --hsource lepton_absetav -o TEST -t aiso_met0to120 --cut mcw*puw*effw*trigw*wptw -m qcdfit --bgsig 5 --bgewk 5 --bgqcd 3
    """
    spR.enable_nominal()
    SuStackElm.new_scales = True
    SuSample.debug = True
    var = opts.var
    bin = opts.bin
    # QCD fit variable and range
    lvar = opts.lvar
    assert len(opts.lbin.split(','))==3,'Wrong format of --lbin argument. Example: 100,-2.5,2.5'
    nbins = int(opts.lbin.split(',')[0])
    lmin = float(opts.lbin.split(',')[1])
    lmax = float(opts.lbin.split(',')[2])
    lpre = '%s>=%.2f && %s<=%.2f'%(lvar,lmin,lvar,lmax)
    # cut string for the ntuple
    x= ''
    ebin = 99
    if opts.extra!=None:
        ie = int(opts.extra)
        x = ' && fabs(l_eta)>=%.2f && fabs(l_eta)<=%.2f'%(absetabins[ie],absetabins[ie+1])
        ebin = ie
    preNN = opts.preNN + x   # regular cut
    if opts.preFN!=None:     # qcd cut
        preFN = opts.preFN + x
    else:
        preFN = prunesub(opts.preNN,lvar,lpre) + x
    preNQ = opts.preNQ + x   # regular cut (fit region)
    if opts.preFQ!=None:     # qcd cut (fit region)
        preFQ = opts.preFQ + x
    else:
        preFQ = prunesub(opts.preNQ,lvar,lpre) + x
    presN = (preNN,preNN,preNQ) # pre strings for normal plots   (e.g., nominal or anti-isolation)
    presF = (preFN,preFN,preFQ) # pre strings for QCD fit region (e.g., lowering MET cut to zero)
    qcdadd={'var':lvar,'nbins':nbins,'min':lmin,'max':lmax,'log':opts.llog,'descr':'ebin%d'%ebin,'pre':presF}
    PLOT_ETA_NORMS = (mode=='qcdfit_1d_nt_summary')  #this is only for QCD shape calculation and comparison [to send to Max]
    if PLOT_ETA_NORMS:
        if opts.etamode==2:
            qcdadd['absetabins']=True
        elif opts.etamode==1:
            qcdadd['etabins']=True
    weight = opts.cut
    #SuSample.GLOBAL_CACHE = None
    hdata,hstack = plot_stack(spRN.clone(pre=presN,weight=weight,var=var,bin=bin,qcdadd=qcdadd),var,bin=bin,q=opts.charge,m=0,name='ebin%d'%ebin+'_'+po.get_flagsum()+'_'+opts.lvar+'_'+opts.lbin)
    # BIG LOOP TO SAVE ALL OBJECTS INTO A ROOT FILE:
    if PLOT_ETA_NORMS:
        ff = ROOT.TFile.Open("file_"+SuSys.QMAP[q][1]+".root","RECREATE")
        ff.cd()
        adir = ff.mkdir(SuSys.QMAP[q][1])
        adir.cd()
        hstack = hstack.nominal().stack ; hstack.SetTitle("stack")
        hdata = hdata.nominal().h ; hdata.SetTitle("data")
        hdata.Write("data",ROOT.TObject.kOverwrite)
        # prepare an array containing statistical uncertainties on QCD fit in each |eta| bin
        STATMAP = [-1.0]*(len(absetabins)-1)
        for key,val in po.fits.iteritems():
            savename = po.fitnames[key] # NEG_ebin99_eta0_Q3S5X5Y5Z5_met_0_0to120_0.png
            etabin = [el for el in savename.split('_') if re.match('eta',el)]
            assert len(etabin)==1
            ieta = int(  re.sub('eta','',etabin[0])  )
            assert ieta < len(STATMAP)
            assert STATMAP[ieta]<0
            STATMAP[ieta] = po.scales[key][1]/po.scales[key][0]
        # save all backgrounds from the stack
        NBG = hstack.GetStack().GetLast()
        for i in xrange(0,NBG+1):
            hh =  hstack.GetHists().At(i)  # TList::At(NBG) = top-most
            if re.search('mc_data_period',hh.GetTitle()): hh.SetTitle('bg_QCD')
            else:
                newt = hh.GetTitle()
                hh.SetTitle(re.sub('mc_mc_','bg_',newt))
            if hh.GetTitle()=='bg_herwig_ww': hh.SetTitle('bg_herwig_diboson')
            if hh.GetTitle()=='bg_pythia_dyan': hh.SetTitle('bg_pythia_DrellYan')
            if hh.GetTitle()=='bg_powheg_pythia_wplustaunu': hh.SetTitle('bg_powheg_pythia_wtaunu')
            if hh.GetTitle()=='bg_powheg_pythia_wmintaunu': hh.SetTitle('bg_powheg_pythia_wtaunu')
            if hh.GetTitle()=='bg_powheg_pythia_wplusmunu': hh.SetTitle('bg_powheg_pythia_wmunu')
            if hh.GetTitle()=='bg_powheg_pythia_wminmunu': hh.SetTitle('bg_powheg_pythia_wmunu')
            hh.Write(hh.GetTitle(),ROOT.TObject.kOverwrite)
            # save variations of QCD template: statup and statdown (reflecting stat. uncertainty on QCD fit)
            if hh.GetTitle() == 'bg_QCD':
                hup = hh.Clone('bg_QCD_statup')
                hup.SetTitle('bg_QCD_statup')
                hdown = hh.Clone('bg_QCD_statdown')
                hdown.SetTitle('bg_QCD_statdown')
                assert (hh.GetNbinsX() == len(STATMAP))
                for ieta in xrange(0,hh.GetNbinsX()):
                    assert STATMAP[ieta]>=0
                    hup.SetBinContent(ieta+1 , hup.GetBinContent(ieta+1) * (1.0+STATMAP[ieta]))
                    hup.SetBinError(ieta+1 , hup.GetBinError(ieta+1) * (1.0+STATMAP[ieta]))
                    hdown.SetBinContent(ieta+1 , hdown.GetBinContent(ieta+1) * (1.0-STATMAP[ieta]))
                    hdown.SetBinError(ieta+1 , hdown.GetBinError(ieta+1) * (1.0-STATMAP[ieta]))
                hup.Write(hup.GetTitle(),ROOT.TObject.kOverwrite)
                hdown.Write(hdown.GetTitle(),ROOT.TObject.kOverwrite)
        OMAP[-1]._canvas.Write("canvas_stack",ROOT.TObject.kOverwrite)
        hstack.Write("stack",ROOT.TObject.kOverwrite)
        # QCD fits
        for key,val in po.fits.iteritems():
            savename = po.fitnames[key] #            NEG_ebin99_eta0_Q3S5X5Y5Z5_met_0_0to120_0.png
            etabin = [el for el in savename.split('_') if re.match('eta',el)]
            assert len(etabin)==1
            val._canvas.Write('QCD_FIT_bin'+etabin[0],ROOT.TObject.kOverwrite)
        ff.Close()
        pass
# DEPRECATED: NTUPLE VERSION AGAIN - TO POST-PROCESS RESULTS
# to study QCD fit systematic due to fitting in different variables, in eta bins
# this assumes that all ntuples have already been cached by running qcdfit action
if mode=='qcdsys_1d_nt':
    spR.enable_nominal()
    SuStackElm.new_scales = True
    #SuSample.debug = True
    var = opts.var
    bin = opts.bin
    LVARS = [ ('met','50,0,120') , ('w_mt','50,40,120') , ('l_pt','50,20,70') ]
    EXC = []
    EXC.append( (POS,7,'met',1) )
    RES = {}
    eloop = range(0,len(absetabins)-1)  # 0 to 10
    if opts.extra: eloop = [int(opts.extra),]
    for charge in (0,1,2):
        RES[charge] = {}
        for ebin in eloop:
            RES[charge][ebin] = {}
            for lvpair in LVARS:
                lvar = lvpair[0]
                lbin = lvpair[1]
                RES[charge][ebin][lvar] = {}
                for bgsig in (1,4,5):
                    # different MC
                    po.choose_sig(bgsig)
                    # QCD fit variable and range
                    assert len(lbin.split(','))==3,'Wrong format of --lbin argument. Example: 100,-2.5,2.5'
                    nbins = int(lbin.split(',')[0])
                    lmin = float(lbin.split(',')[1])
                    lmax = float(lbin.split(',')[2])
                    lpre = '%s>=%.2f && %s<=%.2f'%(lvar,lmin,lvar,lmax)
                    # restrict --pre string to a particular eta bin
                    x = ''
                    ie = ebin
                    x = ' && fabs(l_eta)>=%.2f && fabs(l_eta)<=%.2f'%(absetabins[ie],absetabins[ie+1])
                    # cut string for the ntuple
                    preNN = opts.preNN + x   # regular cut
                    if opts.preFN!=None:     # qcd cut
                        preFN = opts.preFN + x
                    else:
                        preFN = prunesub(opts.preNN,lvar,lpre) + x
                    preNQ = opts.preNQ + x   # regular cut (fit region)
                    if opts.preFQ!=None:     # qcd cut (fit region)
                        preFQ = opts.preFQ + x
                    else:
                        preFQ = prunesub(opts.preNQ,lvar,lpre) + x
                    presN = (preNN,preNN,preNQ) # pre strings for normal plots   (e.g., nominal or anti-isolation)
                    presF = (preFN,preFN,preFQ) # pre strings for QCD fit region (e.g., lowering MET cut to zero)
                    qcdadd={'var':lvar,'nbins':nbins,'min':lmin,'max':lmax,'log':opts.llog,'descr':'ebin%d'%ebin,'pre':presF}
                    weight = opts.cut
                    #SuSample.GLOBAL_CACHE = None
                    print '============ RUNNING: Q/ebin/lvar/bgsig =',charge,ebin,lvar,bgsig
                    curex=(charge,ebin,lvar,bgsig)
                    if curex in EXC:
                        print 'WARNING: skipping',curex
                        hfrac = -1
                    else:
                        hdata,hstack = plot_stack(spRN.clone(pre=presN,weight=weight,var=var,bin=bin,qcdadd=qcdadd),var,bin=bin,q=charge,m=0,name='ebin%d'%ebin+'_'+po.get_flagsum()+'_'+lvar+'_'+lbin)
                        hfrac=hstack.nominal().stack_bg_frac()
                    RES[charge][ebin][lvar][bgsig] = hfrac
    import pickle
    pickle.dump( RES, open( "save.pickle", "w" ) )

# studying ABCD method (closure) on BB/CC monte-carlo
if mode=='abcd_mc':
    SuStackElm.new_scales = False
    iq = opts.charge
    ieta = int(opts.preNN)
    ipt  = opts.preNQ if opts.preNQ=='ALL' else int(opts.preNQ)
    bgsig = opts.bgsig
    po.choose_sig(bgsig)
    bgewk = opts.bgewk
    po.choose_ewk(bgewk)
    pre = opts.pre
    def make_A(a):
        v = prunesub(a,['nmuons','w_mt','met','ptiso40/l_pt'],'w_mt>40.0 && met>25.0 && ptiso40/l_pt<0.1 && nmuons<2')
        print 'A:',v
        return v
    def make_B(a):
        v = prunesub(a,['nmuons','w_mt','met','ptiso40/l_pt'],'w_mt>40.0 && met>25.0 && ptiso40/l_pt>=0.1 && nmuons<2')
        print 'B:',v
        return v
    def make_C(a):
        v = prunesub(a,['nmuons','w_mt','met','ptiso40/l_pt'],'w_mt<=40 && met<=25.0 && ptiso40/l_pt<0.1 && nmuons<2')
        print 'C:',v
        return v
    def make_D(a):
        v = prunesub(a,['nmuons','w_mt','met','ptiso40/l_pt'],'w_mt<=40 && met<=25.0 && ptiso40/l_pt>=0.1 && nmuons<2')
        print 'D:',v
        return v
    x =  ' && fabs(l_eta)>=%.2f && fabs(l_eta)<=%.2f'%(absetabins[ieta],absetabins[ieta+1])
    x += ' && l_pt>=%.2f && l_pt<=%.2f'%(ptbins[ipt],ptbins[ipt+1])
    x=' '
    def I(a):
        return a.nominal().h.Integral()
    hA = po.qcd('A',spRN.clone(pre = make_A(pre)+x))
    hB = po.qcd('B',spRN.clone(pre = make_B(pre)+x))
    hC = po.qcd('C',spRN.clone(pre = make_C(pre)+x))
    hD = po.qcd('D',spRN.clone(pre = make_D(pre)+x))
    print 'Integrals:',I(hA),I(hB),I(hC),I(hD)
    R = I(hB)*I(hC)/I(hD) if I(hD)>0 else 0
    print 'Results:',I(hA),R,'%.1f%%'%((R-I(hA))/I(hA)*100.0)
    c = SuCanvas('ABCD_mc')
    M = PlotOptions()
    M.add('A','Region A',color=1)
    M.add('B','Region B',color=2)
    M.add('C','Region C',color=3)
    M.add('D','Region D',color=4)
    hs = [hA,hB,hC,hD]
    c.plotAny(hs,M=M)
    OMAP.append(c)

if mode=='qcd_isolation':
    SuStackElm.new_scales = False
    iq = opts.charge
    ieta = opts.preNN
    ipt  = opts.preNQ
    bgsig = opts.bgsig
    po.choose_sig(bgsig)
    bgewk = opts.bgewk
    po.choose_ewk(bgewk)
    pre = opts.pre
    def make_H(a):
        v = prunesub(a,['met','ptiso40/l_pt'],'ptiso40/l_pt<0.1')
        print 'H:',v
        return v
    def make_A(a):
        v = prunesub(a,['nmuons','met','ptiso40/l_pt'],'ptiso20/l_pt>0.1 && nmuons<2')
        print 'A:',v
        return v
    #x =  ' && fabs(l_eta)>=%.2f && fabs(l_eta)<=%.2f'%(absetabins[ieta],absetabins[ieta+1])
    #x += ' && l_pt>=%.2f && l_pt<=%.2f'%(ptbins[ipt],ptbins[ipt+1])
    x=' '
    hH = po.qcd('default',spRN.clone(pre = make_H(pre)+x))
    hA = po.qcd('inverted',spRN.clone(pre = make_A(pre)+x))
    hH.Unitize()
    hA.Unitize()
    c = SuCanvas('MET_SHAPE')
    M = PlotOptions()
    M.add('h','Signal-region MET',color=1,ratio=True)
    M.add('hA','Anti-isolated region MET',color=2)
    hs = [hH,hA]
    c.plotAny(hs,M=M)
    OMAP.append(c)

if mode=='manual':
    SuStackElm.new_scales = False
    spR.enable_nominal()
    plots = [opts.hsource,]
    s1 = po.find_sample('mc_alpgen_herwig_wmunu_np0')
    h1 = s1.histo('s1',spR.clone())
    assert s1
    s2 = po.find_sample('mc_af_alpgen_herwig_wmunu_np0')
    assert s2
    h2 = s2.histo('s2',spR.clone())
    M = PlotOptions()
    M.add('s1','Full sim',color=2,size=1.0,ratio=True)
    M.add('s2','Fast sim',color=3,size=0.5)
    hs = [h1,h2]
    c = SuCanvas('FASTSIM_'+QMAP[q][1]+'_'+opts.hsource)
    c._ratioName = 'Fast/Full'
    leg = ROOT.TLegend()
    leg.SetFillColor(0)
    c.plotAny(hs,M=M,leg=leg,w=800,h=600)
    OMAP.append(c)
    
if mode=='one_plot':
    if True:
        SuStackElm.new_scales = False
        spR.enable_nominal()
    plots = [opts.hsource,]
    plot_stacks(spR.clone(),plots,m=1,qs=(0,1,2))

if mode=='one_plot_nt':
    SuStackElm.new_scales = False
    plots = [opts.hsource,]
    plot_stacks(spRN.clone(),plots,m=1,qs=(0,1,2))

if mode=='qcd_bgsub':
    # studying background subtraction in data-driven qcd template
    spR.enable_nominal()
    plots = [opts.hsource,]
    po.choose_qcd(3)
    plot_stacks(spR.clone(),plots,m=1,qs=(opts.charge,),name='QCD3')
    #FIXME TODO: work in progress
    qcd3 = po.qcd('qcd3',opts.var,opts.bin,pre_isol,path=path_reco)
    po.choose_qcd(4)
    plot_stacks(spR.clone(),plots,m=1,qs=(opts.charge,),name='QCD4')

if mode=='unfold2d':
    c = SuCanvas(mode)
    SuStackElm.new_scales = False
    ipt = opts.preNN if opts.preNN=='ALL' else int(opts.preNN)
    imin,imax = (ipt,ipt) if ipt!='ALL' else (0,8)
    spR.enable_names(['Nominal','MuonScaleKUp','MuonScaleKDown','MuonScaleCUp','MuonScaleCDown'])
    #h = po.data_sub('pos',spR.clone(q=0,do_unfold=True,histo='d2_abseta_lpt:y:%d:%d'%(imin,imax),sliced_2d=True))
    #h = po.sig('Q%d'%opts.charge,spR.clone(q=opts.charge,do_unfold=False,histo='d2_abseta_lpt:y:%d:%d'%(imin,imax),sliced_2d=True))
    h = po.sig('Q%d'%opts.charge,spR.clone(q=opts.charge,do_unfold=False,histo=opts.hsource%(imin,imax),sliced_2d=True))
    M = PlotOptions()
    M.add('fromslices2d','From slices - 2D',size=0.5)
    c.plotAny([h,],M=M,height=1.7)
    OMAP.append(c)
    OMAP.append( h.individual_systematics(canvas_name=c.namebase()+'_SYS') )

if mode=='control_stack':
    spR.enable_nominal()
    plots = [opts.hsource,]
    plot_stacks(spR.clone(),plots,m=1,qs=(0,1))

if mode=='control_stack_nonorm':
    assert False,'Make sure to change baseline to metfit region'
    spR.enable_nominal()
    SuStackElm.new_scales = False
    plots = [opts.hsource,]
    plot_stacks(spR.clone(),plots,m=1,qs=(1,))

if mode=='100': # creates efficiency histogram (corrects back to particle level)
    renormalize()
    c = SuCanvas()
    c.buildDefault(width=1024,height=400)
    cc = c.cd_canvas()
    cc.Divide(2,1)
    cc.cd(1)
    pre = '(%s) * (%s) * (%s)'%(QMAP[q][2],opts.cut,fortruth(opts.pre))
    htruth = po.sig('truth',opts.var,opts.bin,pre,path=path_truth)
    pre = '(%s) * (%s) * (%s)'%(QMAP[q][2],opts.cut,opts.pre)
    hreco = po.sig('reco',opts.var,opts.bin,pre,path=path_reco)
    htruth.SetLineColor(ROOT.kBlack)
    htruth.Draw('')
    hreco.SetLineColor(ROOT.kRed)
    hreco.SetLineWidth(hreco.GetLineWidth()*2)
    hreco.Draw('A same')
    cc.cd(2)
    # efficiency histogram for bin-by-bin unfolding: hreco == htruth * heff
    heff = hreco.Clone('heff')
    heff.Divide(htruth)
    heff.SetLineColor(ROOT.kBlack)
    heff.Draw('')
    # save efficiency histogram in a dedicated efficiency file
    if opts.effroot:
        with FileLock(opts.effroot):
            f = ROOT.TFile.Open(opts.effroot,'UPDATE')
            assert f and f.IsOpen()
            f.cd()
            key_str = re.sub(r'[^\w]', '_', 'eff_%s_%s_%s_%d'%(opts.input,opts.var,opts.bin,opts.charge))
            heff.Write(key_str,ROOT.TObject.kOverwrite)
            f.Close()


if mode=='2': # signal - directly from MC, or bg-subtracted data - allow application of efficiency histogram
    assert opts.ntuple=='w','Only w ntuple supported for now'
    renormalize()
    c = SuCanvas()
    c.buildDefault(width=1024,height=400)
    cc = c.cd_canvas()
    cc.Divide(2,1)
    cc.cd(1)
    pre = '(%s) * (%s) * (%s)'%(QMAP[q][2],opts.cut,fortruth(opts.pre))
    htruth = po.sig('part_truth',opts.var,opts.bin,pre,path=path_truth)
    htruth.SetLineColor(ROOT.kBlack)
    hsig  = po.sig('signal',opts.var,opts.bin,'(%s) * (%s) * (%s)'%(QMAP[q][2],opts.cut,opts.pre))
    hsig.SetLineColor(ROOT.kRed)
    hsig_eff = particle(hsig.Clone('signal_eff'))
    htruth.Draw('')
    hsig.Draw('A same')
    if False:
        hd_sig = po.data_sub('bgsub_data',opts.var,opts.bin,'(%s) * (%s) * (%s)'%(QMAP[q][2],opts.cut,opts.pre))
        hd_sig.SetLineColor(ROOT.kBlue)
        hd_sig_eff = particle(hd_sig.Clone('bgsub_data_eff'))
        hd_sig.Draw('A same')
    if opts.effroot:
        cc.cd(2)
        htruth.Draw('')
        hsig_eff.Draw('A same')
        #hd_sig_eff.Draw('A same')

def asymmetry_old():
    assert opts.ntuple=='w','ERROR: asymmetry can only be computed for the w ntuple'
    hsig = po.sig('signalPOS',spR.clone(q=0))
    hd   = po.data_sub('dataPOS',spR.clone(q=0))
    c = SuCanvas()
    c.plotAsymmetryFromComponents(hd[POS],hd[NEG],hsig[POS],hsig[NEG])
    OMAP.append(c)
    return c

if mode=='11': # asymmetry (bg-subtracted)
    asymmetry_old()

if mode in ('sig_reco','sig_truth'): # compares distributions in different signal monte-carlos.
    is_truth = (mode=='sig_truth')
    c = SuCanvas()
    c.buildDefault(width=1024,height=768)
    cc = c.cd_canvas()
    cc.cd(1)
    h = []
    leg = ROOT.TLegend()
    leg.SetHeader(opts.var)
    SuSample.unitize = True
    for i in xrange(M.ntot()):
        print 'Creating:',i
        cut = opts.cut+'*'+M.cuts[i] if M.cuts[i] else opts.cut
        pre = '(%s) * (%s) * (%s)'%(QMAP[q][2],opts.cut,fortruth(opts.pre) if is_truth else opts.pre)
        h.append( po.histo(M.names[i],'plot_%s_%s_%d'%(M.names[i],M.cuts[i] if M.cuts[i] else '',q),opts.var,opts.bin,pre,path=(path_truth if is_truth else path_reco)) )
        h[-1].SetLineColor(M.colors[i])
        h[-1].SetMarkerColor(M.colors[i])
        h[-1].SetMarkerStyle(M.styles[i])
        h[-1].SetMarkerSize(M.sizes[i])
        h[-1].Draw() if i==0 else h[-1].Draw('A same')
        leg.AddEntry(h[-1],M.labels[i],'LP')
    maxh = max([htmp.GetMaximum() for htmp in h])*1.5
    h[0].GetYaxis().SetRangeUser(0,maxh)
    leg.Draw('same')

if mode=='asym_truth': # asymmetry, at truth level, of different monte-carlos.
    c = SuCanvas()
    c.buildDefault(width=1024,height=768)
    cc = c.cd_canvas()
    cc.cd(1)
    h = []
    hasym = []
    leg = ROOT.TLegend()
    leg.SetHeader('Asymmetry:')
    for i in xrange(M.ntot()):
        h.append([None,None])
        for q in (0,1):
            print 'Creating:',i,q
            cut = opts.cut+'*'+M.cuts[i] if M.cuts[i] else opts.cut
            pre = '(%s) * (%s) * (%s)'%(QMAP[q][2],cut,fortruth(opts.pre))
            h[i][q] = po.histo(M.names[i],'truth_%s_%s_%d'%(M.names[i],M.cuts[i] if M.cuts[i] else '',q),opts.var,opts.bin,pre,path=path_truth)
            h[i][q].SetLineColor(M.colors[i])
            h[i][q].SetMarkerColor(M.colors[i])
            h[i][q].SetMarkerStyle(M.styles[i])
            h[i][q].SetMarkerSize(M.sizes[i])
        hasym.append(c.WAsymmetry(h[i][POS],h[i][NEG]))
        hasym[i].Draw() if i==0 else hasym[i].Draw('A same')
        leg.AddEntry(hasym[i],M.labels[i],'LP')
    maxh = max([htmp.GetMaximum() for htmp in hasym])*1.5
    hasym[0].GetYaxis().SetRangeUser(0,maxh)
    leg.Draw('same')

if mode=='matrix_2010inc': # QCD estimation using matrix method
    c = SuCanvas()
    c.buildDefault(width=1600,height=768)
    cc = c.cd_canvas()
    cc.Divide(3,2)
    cc.cd(1)
    # Get heff_real from Z tag and probe
    print 'Obtaining heff_real template via Z tag and probe'
    pre_z_aft = pre_w2z(opts.pre)
    hz_bef = []
    hz_aft = []
    for iq in xrange(2):
        iqp = iq
        iqt = 0 if iq==1 else 1
        var = None
        for isub in w2zsub:
            if re.search(isub[0],opts.var):
                var = opts.var.replace(isub[0],isub[1]).replace('lX',QMAPZ[iqp][2])
                break
        if not var:
            print 'This variable cannot be automatrically translated between w and z ntuples: %s'%opts.var
            var = opts.var
        pre_z_bef = prune(pre_z_aft,'lX_ptiso20'.replace('lX',QMAPZ[iqp][2]))
        print 'BKK: isolation efficiency for Z events'
        print pre_z_bef
        print pre_z_aft
        hz_bef.append( pz.data('data_bef_%s'%QMAPZ[iq][1],var,opts.bin,'(%s) * (%s)'%(opts.cut,pre_z_bef)) )
        hz_aft.append( pz.data('data_aft_%s'%QMAPZ[iq][1],var,opts.bin,'(%s) * (%s)'%(opts.cut,pre_z_aft)) )
    iq = 1 if opts.charge==1 else 0  # both-charges case: use mu+ template
    heff_real = c.Efficiency(hz_aft[0],hz_bef[0])
    heff_real.Draw('')
    heff_real.GetYaxis().SetRangeUser(0.98,1.02)
    heff_real.GetYaxis().SetTitle('Efficiency for non-QCD')
    heff_real.GetXaxis().SetTitle(opts.var)
    #pw.data = pw.qcd
    # Get heff_fake from a fakes-dominated region
    print 'Obtaining heff_fake template using W candidates in a QCD-enriched region'
    cc.cd(2)
    pre_fake_toppair1 = prunesub(opts.pre,('fabs\(d0sig\)'),'fabs(d0sig)>5')
    pre_fake_winc = prunesub(opts.pre,('l_pt','met','w_mt','fabs\(d0sig\)','l_trigEF','nmuons'),'l_pt>15 && l_pt<20 && met<25 && w_mt<40')
    pre_fake_aft = '(%s) * (%s) * (%s)'%(QMAP[q][2],opts.cut,pre_fake_winc)
    pre_fake_bef = '(%s) * (%s) * (%s)'%(QMAP[q][2],opts.cut,prune(pre_fake_winc,'ptiso'))
    print 'AKK: isolation efficiency for QCD events'
    print pre_fake_bef
    print pre_fake_aft
    hfake_bef = pw.data('fdata_bef',opts.var,opts.bin,pre_fake_bef)
    hfake_aft = pw.data('fdata_aft',opts.var,opts.bin,pre_fake_aft)
    heff_fake = c.Efficiency(hfake_aft,hfake_bef)
    heff_fake.Draw('')
    heff_fake.GetYaxis().SetRangeUser(0.2,0.5)
    heff_fake.GetYaxis().SetTitle('Efficiency for QCD')
    heff_fake.GetXaxis().SetTitle(opts.var)
    # Get h_isol, h_loose directly from data
    print 'Obtaining overall loose and isolated templates from data'
    cc.cd(3)
    pre_isol = '(%s) * (%s) * (%s)'%(QMAP[q][2],opts.cut,opts.pre)
    pre_loose = '(%s) * (%s) * (%s)'%(QMAP[q][2],opts.cut,looseisoreg(opts.pre))
    print 'CKK: overall loose and isolated samples'
    print pre_isol
    print pre_loose
    pres = (pre_isol,pre_loose)
    hNdata = []
    for i in range(len(pres)):
        hNdata.append( po.data('data%d'%i,opts.var,opts.bin,pres[i],path=path_reco) )
    hN_isol,hN_loose = hNdata[0],hNdata[1]
    msize=1.3
    hN_loose.SetLineColor(ROOT.kGray)
    hN_isol.SetLineColor(ROOT.kBlack)
    hN_loose.Draw('')
    hN_loose.GetXaxis().SetTitle(opts.var)
    hN_loose.GetYaxis().SetRangeUser(0,max(hN_loose.GetMaximum(),hN_isol.GetMaximum())*1.5)
    hN_loose.GetYaxis().SetTitle('Loose and Isolated muons')
    hN_isol.Draw('same A')
    leg = ROOT.TLegend()
    leg.AddEntry(hN_loose,'Loose muons','LP')
    leg.AddEntry(hN_isol,'Isolated muons','LP')
    leg.Draw('same')
    gbg.append(leg)
    # Pre-isolated QCD template histogram
    cc.cd(4)
    hqcd_loose = c.Matrix_loose(hN_isol,hN_loose,heff_real,heff_fake)
    hqcd_loose.Draw()
    hqcd_loose.GetYaxis().SetRangeUser(0,hqcd_loose.GetMaximum()*1.2)
    hqcd_loose.GetYaxis().SetTitle('QCD template before isolation cut')
    hqcd_loose.GetXaxis().SetTitle(opts.var)
    # Final QCD template histogram, overlaid on top of the bb/cc prediction
    cc.cd(5)
    hqcd_tight = c.Matrix_tight(hN_isol,hN_loose,heff_real,heff_fake)
    hqcd_mc = po.qcd('data_mcqcd',opts.var,opts.bin,pre_isol,path=path_reco)
    hqcd_mc.SetLineColor(ROOT.kGray)
    haxis = hqcd_tight.DrawNormalized()
    haxis2 = hqcd_mc.DrawNormalized('same A')
    haxis.GetYaxis().SetRangeUser(0,max(haxis.GetMaximum(),haxis2.GetMaximum())*1.5)
    haxis.GetYaxis().SetTitle('QCD template after isolation cut')
    haxis.GetXaxis().SetTitle(opts.var)
    leg = ROOT.TLegend()
    leg.AddEntry(hqcd_tight,'Matrix-method QCD','LP')
    leg.AddEntry(hqcd_mc,'MC QCD (bb/cc mu15x)','LP')
    leg.Draw('same')
    gbg.append(leg)
    
if mode in ('101','102','103'): # tag and probe
    assert opts.ntuple=='z','ERROR: tag-and-probe can only be computed for the z ntuple'
    #renormalize()
    hda_bef = [None]*2
    hda_aft = [None]*2
    hmc_bef = [None]*2
    hmc_aft = [None]*2
    hda_bef_pl = [None]*2
    hda_aft_pl = [None]*2
    hmc_bef_pl = [None]*2
    hmc_aft_pl = [None]*2
    for iq in xrange(2): # probe charge (what we're measuring)
        iqp = iq
        iqt = 0 if iq==1 else 1
        var = opts.var.replace('lY',QMAPZ[iqp][2])
        pre_bef = opts.prebef.replace('lX',QMAPZ[iqt][2]).replace('lY',QMAPZ[iqp][2])
        pre_aft = opts.preaft.replace('lX',QMAPZ[iqt][2]).replace('lY',QMAPZ[iqp][2])
        hmc_bef[iq] = po.sig('mc_bef_%s'%QMAPZ[iq][1],var,opts.bin,'(%s) * (%s)'%(opts.cut,pre_bef))
        hmc_aft[iq] = po.sig('mc_aft_%s'%QMAPZ[iq][1],var,opts.bin,'(%s) * (%s)'%(opts.cut,pre_aft))
        if mode=='101':   # default data
            hda_bef[iq] = po.data('da_bef_%s'%QMAPZ[iq][1],var,opts.bin,'(%s) * (%s)'%(opts.cut,pre_bef))
            hda_aft[iq] = po.data('da_aft_%s'%QMAPZ[iq][1],var,opts.bin,'(%s) * (%s)'%(opts.cut,pre_aft))
        elif mode=='102': # bg-subtracted data (using MC)
            hda_bef[iq] = po.data_sub('da_bef_%s'%QMAPZ[iq][1],var,opts.bin,'(%s) * (%s)'%(opts.cut,pre_bef))
            hda_aft[iq] = po.data_sub('da_aft_%s'%QMAPZ[iq][1],var,opts.bin,'(%s) * (%s)'%(opts.cut,pre_aft))
        elif mode=='103': # bg-subtracted data (using same-sign Z pairs in data)
            hda_bef[iq] = po.data('da_bef_%s'%QMAPZ[iq][1],var,opts.bin,'(%s) * (%s)'%(opts.cut,pre_bef))
            hda_aft[iq] = po.data('da_aft_%s'%QMAPZ[iq][1],var,opts.bin,'(%s) * (%s)'%(opts.cut,pre_aft))
            pre_bef_ss = samesignreg(pre_bef)
            pre_aft_ss = samesignreg(pre_aft)
            hda_bef_ss = po.data('da_bef_%s'%QMAPZ[iq][1],var,opts.bin,'(%s) * (%s)'%(opts.cut,pre_bef_ss))
            hda_aft_ss = po.data('da_aft_%s'%QMAPZ[iq][1],var,opts.bin,'(%s) * (%s)'%(opts.cut,pre_aft_ss))
            hda_bef[iq].Add(hda_bef_ss,-1)
            hda_aft[iq].Add(hda_aft_ss,-1)
        else:
            assert False,'Unknown tag-and-probe mode'
        if True: # extra plots
            cn = SuCanvas()
            cn.buildDefault(width=1024,height=600,title='ztagprobe%d'%iq)
            cc = cn.cd_canvas()
            cc.Divide(2,1)
            color = (ROOT.kRed,ROOT.kBlue,46,38)
            # copy
            hmc_bef_pl[iq] = hmc_bef[iq].Clone(hmc_bef[iq].GetName()+'_pl')
            hmc_aft_pl[iq] = hmc_aft[iq].Clone(hmc_aft[iq].GetName()+'_pl')
            hda_bef_pl[iq] = hda_bef[iq].Clone(hda_bef[iq].GetName()+'_pl')
            hda_aft_pl[iq] = hda_aft[iq].Clone(hda_aft[iq].GetName()+'_pl')
            # scale
            hmc_bef_pl[iq].Scale(1/hmc_bef_pl[iq].Integral())
            hmc_aft_pl[iq].Scale(1/hmc_aft_pl[iq].Integral())
            hda_bef_pl[iq].Scale(1/hda_bef_pl[iq].Integral())
            hda_aft_pl[iq].Scale(1/hda_aft_pl[iq].Integral())
            # plot style
            hmc_bef_pl[iq].SetLineColor(color[iq+2])
            hmc_bef_pl[iq].SetMarkerColor(color[iq+2])
            hmc_bef_pl[iq].SetMarkerSize(0.8);
            hda_bef_pl[iq].SetLineColor(color[iq])
            hda_bef_pl[iq].SetMarkerColor(color[iq])
            hda_bef_pl[iq].SetMarkerSize(1.0);
            hda_bef_pl[iq].SetMarkerStyle(22);
            hmc_aft_pl[iq].SetLineColor(color[iq+2])
            hmc_aft_pl[iq].SetMarkerColor(color[iq+2])
            hmc_aft_pl[iq].SetMarkerSize(0.8);
            hda_aft_pl[iq].SetLineColor(color[iq])
            hda_aft_pl[iq].SetMarkerColor(color[iq])
            hda_aft_pl[iq].SetMarkerSize(1.0);
            hda_aft_pl[iq].SetMarkerStyle(22);
            # drawing
            cc.cd(1)
            hda_bef_pl[iq].Draw('')
            hmc_bef_pl[iq].Draw('A SAME')
            hda_bef_pl[iq].GetYaxis().SetRangeUser(0.0,max(hda_bef_pl[iq].GetMaximum(),hmc_bef_pl[iq].GetMaximum())*1.3);
            hda_bef_pl[iq].GetXaxis().SetTitle('%s before cut'%var);
            cc.cd(2)
            hda_aft_pl[iq].Draw('')
            hmc_aft_pl[iq].Draw('A SAME')
            hda_aft_pl[iq].GetYaxis().SetRangeUser(0.0,max(hda_aft_pl[iq].GetMaximum(),hmc_aft_pl[iq].GetMaximum())*1.3);
            hda_aft_pl[iq].GetXaxis().SetTitle('%s after cut'%var);
            gbg.append(cn)
            OMAP.append( cc )
    c = SuCanvas()
    c.plotTagProbe(hda_bef,hda_aft,hmc_bef,hmc_aft,xtitle=opts.var)

if mode == '1012': # 10/12/2011: MCP group studies of Z mass peak in data and MC
    assert opts.ntuple=='z','ERROR: MCP Z studies can only be computed for the z ntuple'
    tagcentral = False
    c = SuCanvas()
    c.buildDefault(title='mZ',width=800,height=600)
    cc = c.cd_canvas()
    pre_tag = 'lX_idhits==1 && fabs(lX_z0)<10. && fabs(lX_eta)<%.1f && lX_pt>20.0 && (lX_q*lY_q)<0 && fabs(lX_z0-lY_z0)<3 && fabs(lX_d0-lY_d0)<2 && lX_ptiso20/lX_pt<0.1&& Z_m>70 && Z_m<110'%(1.0 if tagcentral else 2.4)
    pre_pro = 'lY_idhits==1 && fabs(lY_z0)<10. && lY_pt>20.0 && lX_ptiso20/lX_pt<0.1'
    bin = '100,70,110'
    mymin=float(bin.split(',')[1])
    mymax=float(bin.split(',')[2])
    rms_scale=1.0
    hPOS = ROOT.TH1F('mZ+','mZ+',25,-2.5,2.5)
    hNEG = ROOT.TH1F('mZ-','mZ-',25,-2.5,2.5)
    hPOS.SetLineColor(ROOT.kRed)
    hPOS.SetMarkerColor(ROOT.kRed)
    hNEG.SetLineColor(ROOT.kBlue)
    hNEG.SetMarkerColor(ROOT.kBlue)
    result = [[],[]]
    cscan = ROOT.TCanvas('scan','scan',1500,1500)
    cscan.Divide(5,5)
    import Fit
    from Fit import w
    w.factory('x[%s,%s]'%(mymin,mymax))
    # choose fit shape
    func,res='gaus',0
    if opts.func[-1].isdigit():
        func = opts.func[:-1]
        res = int(opts.func[-1])
    else:
        func = opts.func
    if func=='gaus':
        model,isExt = Fit.make_gaus(mymin,mymax)
    elif func=='bw':
        model,isExt = Fit.make_bw(mymin,mymax,res)
    elif func=='egge':
        model,isExt = Fit.make_egge(mymin,mymax,res)
    elif func=='bwfull':
        model,isExt = Fit.make_bwfull(mymin,mymax,res)
    elif func=='voig':
        model,isExt = Fit.make_voig(mymin,mymax)
    elif func=='voigbg':
        model,isExt = Fit.make_voigbg(mymin,mymax)
    else:
        print 'Wrong --func (%s). Exiting...'%opts.func
        sys.exit(0)
    for i,eta in enumerate([-2.5+ibin*0.2 for ibin in range(25)]):
        cscan.cd(i+1)
        h = []
        f = []
        for iqp in (POS,NEG):
            iqt = 0 if iqp==1 else 1
            var = 'Z_m'
            pre = (' && '.join([pre_tag,pre_pro,'lY_eta>%f && lY_eta<%f'%(eta,eta+0.2)])).replace('lX',QMAPZ[iqt][2]).replace('lY',QMAPZ[iqp][2])
            h.append( po.data('data%d_q%d'%(i,iqp),var,bin,pre) )
            rdata = Fit.import_histo( h[-1],mymin,mymax)
            fullbins = fitbins = (mymin,mymax)
            r,frame,chi2ndf,ndf,xtra,res1st = Fit.Fit(model,rdata,isExt,fullbins,fitbins,ncpus=16,gaus=(func=='gaus'))
            frame.SetTitle(opts.tag)
            if iqp==NEG:
                frame.Draw()
            gbg.append(frame)
            print 'SAVING VALUES: ',w.var('m').getVal(),w.var('m').getError()
            if iqp==POS:
                hPOS.SetBinContent(i,w.var('m').getVal())
                hPOS.SetBinError(i,w.var('m').getError())
            else:
                hNEG.SetBinContent(i,w.var('m').getVal())
                hNEG.SetBinError(i,w.var('m').getError())
    cscan.SaveAs('scan_mZ.png')
    cc.cd()
    hPOS.Draw()
    hPOS.GetYaxis().SetRangeUser(89,93)
    hNEG.Draw('SAME')
    if True:
        line = ROOT.TGraph(2)
        line.SetPoint(0,-2.5,0)
        line.SetPoint(1,2.5,0)
        line.SetLineWidth(1)
        line.SetLineColor(ROOT.kBlack)
        line.Draw('l')
        gbg.append(line)

def loop_zbins(w, pre_in,
               lvar='lY_eta',lvarbins='25,-2.5,2.5',
               pvar='Z_m',pvarbins='50,70,110',
               tagreq=False):
    """ A general-purpose function to loop over Z events in bins of something
        Supports requiring both legs to be in a particular region.
    """
    import Fit
    print 'ORIGINAL pre string:',pre_in
    pre_in = prune(pre_in,[lvar+'>',lvar+'<'])
    print 'PRUNED pre string:',pre_in
    lbins,lmin,lmax = [ int(zz) if i==0 else float(zz) for i,zz in enumerate(lvarbins.split(','))]
    lbinw = (lmax-lmin)*1.0/lbins
    pbins,pmin,pmax = [ int(zz) if i==0 else float(zz) for i,zz in enumerate(pvarbins.split(','))]
    pbinw = (pmax-pmin)*1.0/pbins
    npads = int(math.ceil(math.sqrt(lbins)))
    cscan = SuCanvas()
    cscan.buildDefault(title='scan',width=1500,height=1500)
    ccscan = cscan.cd_canvas()
    ccscan.Divide(npads,npads)
    hPOS = ROOT.TH1F('mu+','mu+',lbins,lmin,lmax)
    hNEG = ROOT.TH1F('mu-','mu-',lbins,lmin,lmax)
    hPOS.SetLineColor(ROOT.kRed)
    hPOS.SetMarkerColor(ROOT.kRed)
    hNEG.SetLineColor(ROOT.kBlue)
    hNEG.SetMarkerColor(ROOT.kBlue)
    for i in xrange(lbins):
        lL = lmin + i*lbinw
        lR = lL + lbinw
        h = []
        f = []
        for iqp in (POS,NEG):
            ccscan.cd(i+1)
            iqt = 0 if iqp==1 else 1
            var = lvar.replace('lX',QMAPZ[iqt][2]).replace('lY',QMAPZ[iqp][2])
            prelist = [pre_in,'(%s)>%.2f'%(lvar,lL),'(%s)<=%.2f'%(lvar,lR)]
            if tagreq:
                varX = lvar.replace('Y','X')
                prelist += ['(%s)>%.2f'%(lvar,lL),'(%s)<=%.2f'%(lvar,lR)]
            pre = ' && '.join(prelist).replace('lX',QMAPZ[iqt][2]).replace('lY',QMAPZ[iqp][2])
            if opts.extra=='data':
                h.append( po.data('data%d_q%d'%(i,iqp),pvar,pvarbins,pre) )
            else:
                h.append( po.histo('pythia','zmumu%d_q%d'%(i,iqp),pvar,pvarbins,pre,path=path_reco) )
            ipvarbins = ifitbins = (pbins,pmin,pmax)
            if w.func=='gaus':
                dGeV = 6.0  # 4.0
                peak = h[-1].GetMean()
                ifitbins = (int(2*dGeV/pbinw),peak - dGeV, peak + dGeV)
                ifitbins = (-1,80.00,100.00)
                print 'Reducing gaus0 fit window: [%.2f,%.2f] --> [%.2f,%.2f]'%(ipvarbins[1],ipvarbins[2],ifitbins[1],ifitbins[2])
            r,frame,chi2ndf,ndf,xtra,res1st = Fit.Fit(w,h[-1],ipvarbins,ifitbins,ncpus=16)
            frame.Draw()
            ci = SuCanvas()
            ci.buildDefault(title='scan%dq%d'%(i,iqp),width=800,height=600)
            cci = ci.cd_canvas()
            frame.Draw()
            OMAP.append(ci)
            if iqp==POS:
                hPOS.SetBinContent(i+1,w.var('m').getVal())
                hPOS.SetBinError(i+1,w.var('m').getError())
            else:
                hNEG.SetBinContent(i+1,w.var('m').getVal())
                hNEG.SetBinError(i+1,w.var('m').getError())
    return cscan,hPOS,hNEG

if mode == '1111':
    pvar,pvarbins = opts.var,opts.bin
    lvar,lvarbins = opts.lvar,opts.lbin
    pbins,pmin,pmax = [ int(zz) if i==0 else float(zz) for i,zz in enumerate(pvarbins.split(','))]
    import Fit
    w = Fit.RooWorkspace('w',ROOT.kTRUE)
    w.factory('x[%s,%s]'%(pmin,pmax))
    w.model = Fit.make_fit_function(w,opts.func,pvarbins)
    c = SuCanvas()
    c.buildDefault(title='canvas',width=800,height=600)
    cc = c.cd_canvas()
    tagcentral = False
    etalim = 1.0 if tagcentral else 2.4
    pre = 'lX_idhits==1 && fabs(lX_z0)<10. && fabs(lX_eta)<%f && lX_pt>20.0 && (lX_q*lY_q)<0 && fabs(lX_z0-lY_z0)<3 && fabs(lX_d0-lY_d0)<2 && Z_m>70 && Z_m<110'%etalim
    pre += '&& lY_idhits==1 && fabs(lY_z0)<10. && fabs(lY_eta)<%f && lY_pt>20.0'%etalim
    # isolation
    isoreq = 'lX_ptiso20/lX_pt<0.1 && lY_ptiso20/lY_pt<0.1'
    #isoreq = 'lX_ptiso40<2.0 && lX_etiso40<2.0 && lY_ptiso40<2.0 && lY_etiso40<2.0'
    pre += ' && ' + isoreq
    cscan,hPOS,hNEG = loop_zbins(w,pre,pvar=pvar,pvarbins=pvarbins,lvar=lvar,lvarbins=lvarbins)
    cc.cd()
    hPOS.Draw()
    hNEG.Draw('A SAME')
    hPOS.GetYaxis().SetRangeUser(90.5,91.5)
    if opts.refline!='0.5,1.5':
        ymin=float(opts.refline.split(',')[0])
        ymax=float(opts.refline.split(',')[1])
        hPOS.GetYaxis().SetRangeUser(ymin,ymax)
    OMAP.append( cscan )
    pass

if mode == '1013': # 10/13/2011: MCP group studies that do not require a Z peak (MS-ID-CB comparisons)
    # we still use Z events to ensure little QCD contamination
    assert opts.ntuple=='z','ERROR: MCP Z studies can only be computed for the z ntuple'
    tagcentral = False
    rms_scale=0.7
    c = SuCanvas()
    c.buildDefault(title='pT_CB-pT_ID',width=800,height=600)
    cc = c.cd_canvas()
    pre_tag = 'lX_idhits==1 && fabs(lX_z0)<10. && fabs(lX_eta)<%f && lX_pt>20.0 && (lX_q*lY_q)<0 && fabs(lX_z0-lY_z0)<3 && fabs(lX_d0-lY_d0)<2 && lX_ptiso20/lX_pt<0.1 && Z_m>70 && Z_m<110'%(1.0 if tagcentral else 2.4)
    pre_pro = 'lY_idhits==1 && fabs(lY_z0)<10. && lY_pt>20.0 && lX_ptiso20/lX_pt<0.1'
    bin = '200,-10,10'
    hPOS = ROOT.TH1F('ptCB_ptID+','ptCB_ptID+',25,-2.5,2.5)
    hNEG = ROOT.TH1F('ptCB_ptID-','ptCB_ptID-',25,-2.5,2.5)
    hPOS.SetLineColor(ROOT.kRed)
    hPOS.SetMarkerColor(ROOT.kRed)
    hNEG.SetLineColor(ROOT.kBlue)
    hNEG.SetMarkerColor(ROOT.kBlue)
    result = [[],[]]
    cscan = SuCanvas()
    cscan.buildDefault(title='scan',width=1500,height=1500)
    ccscan = cscan.cd_canvas()
    ccscan.Divide(5,5)
    for i,eta in enumerate([-2.5+ibin*0.2 for ibin in range(25)]):
        ccscan.cd(i+1)
        h = []
        f = []
        for iqp in (POS,NEG):
            iqt = 0 if iqp==1 else 1
            var = 'lY_pt-lY_pt_id'.replace('lX',QMAPZ[iqt][2]).replace('lY',QMAPZ[iqp][2])
            pre = (' && '.join([pre_tag,pre_pro,'lY_eta>%f && lY_eta<%f'%(eta,eta+0.2)])).replace('lX',QMAPZ[iqt][2]).replace('lY',QMAPZ[iqp][2])
            h.append( po.data('data%d_q%d'%(i,iqp),var,bin,pre) )
            rms,mean=h[-1].GetRMS(),h[-1].GetMean()
            FITMIN=mean-rms*rms_scale
            FITMAX=mean+rms*rms_scale
            h[-1].Fit('gaus','S','',FITMIN,FITMAX)
            f.append( h[-1].GetFunction('gaus') )
        f[0].SetLineColor(ROOT.kRed)
        f[1].SetLineColor(ROOT.kBlue)
        hPOS.SetBinContent(i,f[0].GetParameter(1))
        hPOS.SetBinError(i,f[0].GetParError(1))
        hNEG.SetBinContent(i,f[1].GetParameter(1))
        hNEG.SetBinError(i,f[1].GetParError(1))
        if False: # simple mean
            hPOS.SetBinContent(i,h[0].GetMean())
            hNEG.SetBinContent(i,h[1].GetMean())
        h[0].SetLineColor(ROOT.kRed)
        h[0].SetMarkerColor(ROOT.kRed)
        h[1].SetLineColor(ROOT.kBlue)
        h[1].SetMarkerColor(ROOT.kBlue)
        h[0].Draw()
        h[1].Draw('SAME')
        gbg += h
    cscan.SaveAs('%s%s_cscan_%s'%(opts.tag,opts.input,mode),'png')
    cc.cd()
    hPOS.Draw()
    hPOS.GetYaxis().SetRangeUser(-0.5,0.5)
    hNEG.Draw('SAME')
    if True:
        line = ROOT.TGraph(2)
        line.SetPoint(0,-2.5,0)
        line.SetPoint(1,2.5,0)
        line.SetLineWidth(1)
        line.SetLineColor(ROOT.kBlack)
        line.Draw('l')
        gbg.append(line)

if mode=='99': # Floating QCD normalization
    renormalize()  # for testing - only activated when --qcd=auto
    c,frame,scalef = run_fit(metfitreg(opts.pre),opts.var,opts.bin,opts.cut)

####################################
######## SAVING OUTPUT
####################################

sys.stdout.flush()
for key,val in po.fits.iteritems():
    print 'Adding a normalization fit:',po.fitnames[key]
    val.savename = po.fitnames[key]
    OMAP.append(val)

# save images
if not opts.antondb:
    if not os.path.isdir(SuCanvas.savedir):
        import time
        time.sleep(0.1)
        if not os.path.isdir(SuCanvas.savedir):
            time.sleep(0.3)
            if not os.path.isdir(SuCanvas.savedir):
                os.makedirs(SuCanvas.savedir)
    # save all plots
    for i,obj in enumerate(OMAP):
        savename = obj.savename
        assert hasattr(obj,'SaveAs') # SuCanvas
        obj.SaveSelf()
sys.stdout.flush()

# save ROOT files
if opts.antondb:
    if len(COUT)>0:
        VMAP['COUT']=[]
        for l in COUT:
            print l
            VMAP['COUT'].append(l)
    if (len(VMAP)>0 or len(OMAP)>0):
        a = antondb.antondb(opts.antondb)
        path = os.path.join('/stack/',opts.tag)
        if len(VMAP)>0:
            a.add(path,VMAP)
        if len(OMAP)>0:
            a.add_root(path,[oo._canvas for oo in OMAP if hasattr(oo,'_canvas')])
sys.stdout.flush()

print 'stack.py finished'
sys.stdout.flush()
os._exit(0)
