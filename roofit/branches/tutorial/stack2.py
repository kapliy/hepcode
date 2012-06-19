#!/usr/bin/env python

_QUALITY = ' && fabs(l_eta)<2.4 && idhits==1 && fabs(z0)<10. && fabs(d0sig)<10. && fabs(l_pt_id-l_pt_exms)/l_pt_id<0.5'
_PRE_PETER  = 'l_pt>20.0 && ptiso40<2.0 && etiso40<2.0 && met>25.0 && w_mt>40.0'+_QUALITY
_PRE_JORDAN = 'l_pt>25.0 && ptiso20/l_pt<0.1 && met>25.0 && w_mt>40.0'+_QUALITY
_PRE_JORDANALT = 'l_pt>25.0 && ptiso20/l_pt<0.1 && ptiso30/l_pt<0.15 && met>25.0 && w_mt>40.0'+_QUALITY

# X - tag muon (doesn't change), Y - probe muon used to measure efficiency ( = Y after/before specific cut)
_BEF = 'lX_idhits==1 && fabs(lP_z0)<10. && fabs(lX_eta)<2.4 && lX_pt>10.0 && lY_idhits==1 && fabs(lN_z0)<10. && fabs(lY_eta)<2.4 && lY_pt>10.0 && Z_m>81.0 && Z_m<101.0 && (lP_q*lN_q)<0 && fabs(lP_z0-lN_z0)<3 && fabs(lP_d0-lN_d0)<2 && fabs(lP_phi-lN_phi)>2.0 && lX_ptiso40<2.0 && lX_etiso40<2.0'
_AFT = _BEF + ' && ' + 'lY_etiso40<2.0'

import sys,re
from hashlib import md5
from optparse import OptionParser
import antondb
parser = OptionParser()
parser.add_option("-m", "--mode",dest="mode",
                  type="string", default='1',
                  help="Plot mode")
parser.add_option("--type",dest="type",
                  type="int", default=1,
                  help="Type = 2 applies pileup weights")
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
                  help="Variable that slices the dataset in Z studies")
parser.add_option("--lbin",dest="lbin",
                  type="string", default='25,-2.5,2.5',
                  help="Binning for lvar")
parser.add_option("--extra",dest="extra",
                  type="string", default=None,
                  help="General-purpose extra parameter")
parser.add_option("--pre",dest="pre",
                  type="string", default=_PRE_PETER,
                  help="Preliminary cuts to select final W candidates")
parser.add_option("--prebef",dest="prebef",
                  type="string", default=_BEF,
                  help="Cuts BEFORE applying studied cut")
parser.add_option("--preaft",dest="preaft",
                  type="string", default=_AFT,
                  help="Cuts AFTER applying studied cut")
parser.add_option("--cut",dest="cut",
                  type="string", default='mcw*puw', # *effw*trigw
                  help="Additional cut to select events")
parser.add_option("--hsource",dest="hsource",
                  type="string", default='%s/st_%s_final/%s',
                  help="Template for the path to data histograms")
parser.add_option("--metallsys", default=False,
                  action="store_true",dest="metallsys",
                  help="If set to true, re-fits QCD for each systematic")
parser.add_option("--rebin",dest="rebin",
                  type="int", default=1,
                  help="Rebin histograms")
parser.add_option("--refline",dest="refline",
                  type="string", default='0.5,1.5',
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
#4701.37*1000.0 - DtoM with pro10
parser.add_option("--lumi",dest="lumi",
                  type="float", default=4701.37*1000.0,
                  help="Integrated luminosity for data (in nb^-1)")
parser.add_option("--qcd",dest="qcdscale",
                  type="string", default='1.0',
                  help="QCD scale factor")
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
                  type="int", default=0,
                  help="QCD: 0=Pythia bb/cc mu15X, 1=Pythia bbmu15X, 2=Pythia J0..J5, 3=data-driven")
parser.add_option("--bgsig",dest="bgsig",
                  type="int", default=2,
                  help="Signal: 0=Pythia, 1=MC@NLO, 2=Alpgen/Herwig, 3=Alpgen/Pythia, 4=PowHeg/Herwig, 5=PowHeg/Pythia")
parser.add_option("--bgewk",dest="bgewk",
                  type="int", default=2,
                  help="EWK: 0=Pythia, 1=MC@NLO, 2=Alpgen/Herwig, 3=Alpgen/Pythia, 4=PowHeg/Herwig, 5=PowHeg/Pythia")
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

#import ROOT and disable warning messages
from common import *
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

SetStyle("AtlasStyle.C")
from SuData import *
SuSample.rootpath = opts.input
SuSample.debug = opts.verbose
SuSample.lumi = opts.lumi
SuSample.rebin = opts.rebin
SuSample.qcdscale = float(opts.qcdscale) if not opts.qcdscale in ('AUTO','auto','Auto') else 1.0

#SetStyle()

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
    #TOP:
    pw.add(name='t#bar{t}',samples='mc_jimmy_ttbar',color=ROOT.kGreen,flags=['bg','mc','ewk'])
    #ZTAUTAU:
    pw.adn(name='ztautau_alpgen_herwig',label='Z#rightarrow#tau#tau',samples=['mc_jimmy_ztautau_np%d'%v for v in range(6)],color=ROOT.kMagenta,flags=['bg','mc','ewk','ztautau'])
    pw.adn(name='ztautau_pythia',label='Z#rightarrow#tau#tau',samples='mc_pythia_ztautau',color=ROOT.kMagenta,flags=['bg','mc','ewk','ztautau'])
    #WTAUNU:
    pw.adn(name='wtaunu_alpgen_herwig',label='W#rightarrow#tau#nu',samples=['mc_jimmy_wtaunu_np%d'%v for v in range(6)],color=ROOT.kYellow,flags=['bg','mc','ewk','wtaunu'])
    pw.adn(name='wtaunu_pythia',label='W#rightarrow#tau#nu',samples='mc_pythia_wtaunu',color=ROOT.kYellow,flags=['bg','mc','ewk','wtaunu'])
    pw.adn(name='wtaunu_powheg_pythia',label='W#rightarrow#tau#nu',samples=['mc_powheg_pythia_wplustaunu','mc_powheg_pythia_wmintaunu'],color=ROOT.kYellow,flags=['bg','mc','ewk','wtaunu'])
    #ZMUMU:
    pw.adn(name='zmumu_alpgen_herwig',label='Z#rightarrow#mu#mu',samples=['mc_jimmy_zmumu_np%d'%v for v in range(6)],color=ROOT.kRed,flags=['bg','mc','ewk','zmumu'])
    pw.adn(name='zmumu_pythia',label='Z#rightarrow#mu#mu',samples='mc_pythia_zmumu',color=ROOT.kRed,flags=['bg','mc','ewk','zmumu'])
    pw.adn(name='zmumu_powheg_pythia',label='Z#rightarrow#mu#mu',samples='mc_powheg_pythia_zmumu',color=ROOT.kRed,flags=['bg','mc','ewk','zmumu'])
    #DIBOSON:
    pw.add(name='WW/WZ/ZZ',samples=['mc_herwig_ww','mc_herwig_wz','mc_herwig_zz'],color=11,flags=['bg','mc','ewk','diboson'])
    #EWK SEL: (defaults to alpgen, wherever possible)
    if opts.bgewk==0:
        pw.choose_wtaunu('wtaunu_pythia')
        pw.choose_zmumu('zmumu_pythia')
        pw.choose_ztautau('ztautau_pythia')
    elif opts.bgewk==1: # mcnlo
        pw.choose_wtaunu('wtaunu_alpgen_herwig') #NA
        pw.choose_zmumu('zmumu_mcnlo')
        pw.choose_ztautau('ztautau_alpgen_herwig') #NA
    elif opts.bgewk==2: # alpgen/herwig
        pw.choose_wtaunu('wtaunu_alpgen_herwig')
        pw.choose_zmumu('zmumu_alpgen_herwig')
        pw.choose_ztautau('ztautau_alpgen_herwig')
    elif opts.bgewk==3: # alpgen/pythia
        pw.choose_wtaunu('wtaunu_alpgen_herwig') #NA
        pw.choose_zmumu('zmumu_alpgen_herwig') #NA
        pw.choose_ztautau('ztautau_alpgen_herwig') #NA
    elif opts.bgewk==4: # powheg/herwig
        pw.choose_wtaunu('wtaunu_alpgen_herwig') #NA
        pw.choose_zmumu('zmumu_powheg_herwig')
        pw.choose_ztautau('ztautau_alpgen_herwig') #NA
    elif opts.bgewk==5: # powheg/pythia
        pw.choose_wtaunu('wtaunu_powheg_herwig')
        pw.choose_zmumu('zmumu_powheg_herwig')
        pw.choose_ztautau('ztautau_alpgen_herwig') #NA
    else:
        assert False,'Unknown bgewk option: %s'%opts.bgewk
    #QCD:
    pw.adn(name='qcd_mc',label='bbmu15X/ccmu15X',samples=['mc_pythia_bbmu15x','mc_pythia_ccmu15x'],color=ROOT.kCyan,flags=['bg','mc','qcd'])
    pw.adn(name='qcd_bb',label='bbmu15X',samples=['mc_pythia_bbmu15x'],color=ROOT.kCyan,flags=['bg','mc','qcd'])
    pw.adn(name='qcd_JX',label='QCD J0..J5',samples=['mc_pythia_J%d'%v for v in xrange(5)],color=ROOT.kCyan,flags=['bg','mc','qcd'])
    pw.adn(name='qcd_driven',label='QCD data-driven',samples=['data_period%s'%s for s in _DATA_PERIODS],color=ROOT.kCyan,flags=['bg','mc','qcd','driven'])
    if opts.bgqcd==0:
        pw.choose_qcd('qcd_mc')
    elif opts.bgqcd==1:
        pw.choose_qcd('qcd_bb')
    elif opts.bgqcd==2:
        pw.choose_qcd('qcd_JX')
    elif opts.bgqcd==3:
        pw.choose_qcd('qcd_driven')
    else:
        assert False,'Unknown bgqcd option: %s'%opts.bgqcd
    #SIG:
    pw.adn(name='sig_pythia',label='W#rightarrow#mu#nu',samples='mc_pythia_wmunu',color=10,flags=['sig','mc','ewk','wmunu'])
    pw.adn(name='sig_sherpa',label='W#rightarrow#mu#nu',samples='mc_sherpa_wmunu',color=10,flags=['sig','mc','ewk','wmunu'])
    pw.adn(name='sig_mcnlo',label='W#rightarrow#mu#nu',samples=['mc_mcnlo_wminmunu','mc_mcnlo_wplusmunu'],color=10,flags=['sig','mc','ewk','wmunu'])
    pw.adn(name='sig_powheg_herwig',label='W#rightarrow#mu#nu',samples=['mc_powheg_herwig_wminmunu','mc_powheg_herwig_wplusmunu'],color=10,flags=['sig','mc','ewk','wmunu'])
    pw.adn(name='sig_powheg_pythia',label='W#rightarrow#mu#nu',samples=['mc_powheg_pythia_wminmunu','mc_powheg_pythia_wplusmunu'],color=10,flags=['sig','mc','ewk','wmunu'])
    pw.adn(name='sig_alpgen_herwig',label='W#rightarrow#mu#nu+jets',samples=['mc_jimmy_wmunu_np%d'%v for v in range(6)],color=10,flags=['sig','mc','ewk','wmunu'])
    pw.adn(name='sig_alpgen_pythia',label='W#rightarrow#mu#nu+jets',samples=['mc_alpgen_pythia_wmunu_np%d'%v for v in range(6)],color=10,flags=['sig','mc','ewk','wmunu'])
    if opts.bgsig==0:
        pw.choose_sig('sig_pythia')
    elif opts.bgsig==1:
        pw.choose_sig('sig_mcnlo')
    elif opts.bgsig==2:
        pw.choose_sig('sig_alpgen_herwig')
    elif opts.bgsig==3:
        pw.choose_sig('sig_alpgen_pythia')
    elif opts.bgsig==4:
        pw.choose_sig('sig_powheg_herwig')
    elif opts.bgsig==5:
        pw.choose_sig('sig_powheg_pythia')
    else:
        assert False,'Unknown bgsig option: %s'%opts.bgsig

# z samples:
if opts.bgsig in (0,1,2,3,4,5,6): # z inclusive
    #assert False,'To be updated - similar to pw case'
    pz.add(name='t#bar{t}',samples='mc_jimmy_ttbar',color=ROOT.kGreen,flags=['bg','mc','ewk'])
    pz.add(name='W#rightarrow#mu#nu',samples=['mc_jimmy_wmunu_np%d'%v for v in range(6)],color=10,flags=['sig','mc','ewk'])
    #pz.add(name='W#rightarrow#mu#nu',samples='mc_pythia_wmunu',color=10,flags=['bg','mc','ewk'])
    pz.add(name='Z#rightarrow#tau#tau',samples=['mc_jimmy_ztautau_np%d'%v for v in range(6)],color=ROOT.kMagenta,flags=['bg','mc','ewk'])
    #pz.add(name='Z#rightarrow#tau#tau',samples='mc_pythia_ztautau',color=ROOT.kMagenta,flags=['bg','mc','ewk'])
    pz.add(name='W#rightarrow#tau#nu',samples=['mc_jimmy_wtaunu_np%d'%v for v in range(6)],color=ROOT.kYellow,flags=['bg','mc','ewk'])
    #pz.add(name='W#rightarrow#tau#nu',samples='mc_pythia_wtaunu',color=ROOT.kYellow,flags=['bg','mc','ewk'])
    pz.add(name='WW/WZ/ZZ',samples=['mc_herwig_ww','mc_herwig_wz','mc_herwig_zz'],color=11,flags=['bg','mc','ewk'])
    if opts.bgqcd==0:
        pz.add(name='bbmu15X/ccmu15X',samples=['mc_pythia_bbmu15x','mc_pythia_ccmu15x'],color=ROOT.kCyan,flags=['bg','mc','qcd'])
    elif opts.bgqcd==1:
        pz.add(name='QCD J0..J5',samples=['mc_pythia_J%d'%v for v in xrange(5)],color=ROOT.kCyan,flags=['bg','mc','qcd'])
    elif opts.bgqcd==2:
        pz.add(name='QCD data-driven',samples=['data_period%s'%s for s in _DATA_PERIODS],color=ROOT.kCyan,flags=['bg','mc','qcd','driven'])
    if opts.bgsig==0:
        pz.add(name='Z#rightarrow#mu#mu',samples='mc_pythia_zmumu',color=ROOT.kRed,flags=['sig','mc','ewk'])
    elif opts.bgsig==1:
        pz.add(name='Z#rightarrow#mu#mu',samples='mc_mcnlo_zmumu',color=ROOT.kRed,flags=['sig','mc','ewk'])
    elif opts.bgsig==2:
        pz.add(name='Z#rightarrow#mu#mu+jets',samples=['mc_jimmy_zmumu_np%d'%v for v in range(6)],color=ROOT.kRed,flags=['sig','mc','ewk'])
    elif opts.bgsig==4:
        pz.add(name='Z#rightarrow#mu#mu',samples='mc_powheg_herwig_zmumu',color=ROOT.kRed,flags=['sig','mc','ewk'])
    elif opts.bgsig==5:
        pz.add(name='Z#rightarrow#mu#mu',samples='mc_powheg_pythia_zmumu',color=ROOT.kRed,flags=['sig','mc','ewk'])
    elif opts.bgsig==6:
        pz.add(name='Z#rightarrow#mu#mu',samples='mc_sherpa_zmumu',color=10,flags=['sig','mc','ewk'])
    pz.add(name='pythia',samples='mc_pythia_zmumu',color=10,flags=['sig','mc','ewk','no'])
    pz.add(name='sherpa',samples='mc_sherpa_zmumu',color=10,flags=['sig','mc','ewk','no'])
    pz.add(name='mcnlo',samples='mc_mcnlo_zmumu',color=10,flags=['sig','mc','ewk','no'])
    pz.add(name='powheg_herwig',samples='mc_powheg_herwig_zmumu',color=10,flags=['sig','mc','ewk','no'])
    pz.add(name='powheg_pythia',samples='mc_powheg_pythia_zmumu',color=10,flags=['sig','mc','ewk','no'])
    pz.add(name='alpgen_herwig',samples=['mc_jimmy_zmumu_np%d'%v for v in range(6)],color=10,flags=['sig','mc','ewk','no'])
elif opts.bgsig in (10,11,12,13,14): # Z->mumu signal only (for normalized plots)
    if opts.bgsig==10: #Pythia
        pz.add(name='Z#rightarrow#mu#mu',samples='mc_pythia_zmumu',color=ROOT.kRed,flags=['sig','mc','ewk'])
    elif opts.bgsig==11: # MC@NLO
        pz.add(name='Z#rightarrow#mu#mu',samples='mc_mcnlo_zmumu',color=ROOT.kRed,flags=['sig','mc','ewk'])
    elif opts.bgsig in (12,13): #Alpgen
        pz.add(name='Z#rightarrow#mu#mu+jets',samples=['mc_jimmy_zmumu_np%d'%v for v in range(6)],color=ROOT.kRed,flags=['sig','mc','ewk'])
    elif opts.bgsig==14: #PowHeg Herwig
        pz.add(name='Z#rightarrow#mu#mu',samples='mc_powheg_herwig_zmumu',color=ROOT.kRed,flags=['sig','mc','ewk'])
    elif opts.bgsig==15: #PowHeg Pythia
        pz.add(name='Z#rightarrow#mu#mu',samples='mc_powheg_pythia_zmumu',color=ROOT.kRed,flags=['sig','mc','ewk'])
    SuSample.unitize = True

# Pre-load ntuples
path_truth = 'truth/st_%s_final/ntuple'%opts.ntuple
path_reco  = 'nominal/st_%s_final/ntuple'%opts.ntuple
for it,px in enumerate((pw,pz)):
    ptruth = 'truth/st_%s_final/ntuple'%('w' if it==0 else 'z')
    preco  = 'nominal/st_%s_final/ntuple'%('w' if it==0 else 'z')
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
tightlvl = 'tight_'
tightlvl = ''
spR = SuPlot()
spR.bootstrap(do_unfold=False,
              unfold={'sysdir':tightlvl+'nominal','mc':'pythia','method':'RooUnfoldBinByBin','par':4},
              charge=q,var=opts.var,histo=opts.hsource,
              sysdir=[tightlvl+'nominal',tightlvl+'nominal','isowind'],subdir='st_w_final',basedir='baseline',
              qcd={'metfit':'metfit'})
SuStack.QCD_SYS_SCALES = opts.metallsys
SuStack.QCD_TF_FITTER = False
spR.enable_all()
# Reco-level [ntuple]
spRN = SuPlot()
spRN.bootstrap(ntuple=opts.ntuple,histo=opts.hsource,
               charge=q,var=opts.var,path=path_reco,bin=opts.bin,
               weight=opts.cut,pre=opts.pre)
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

def plot_any(spR2,spT2=None,m=2,var='lepton_absetav',do_errorsDA=False,do_errorsMC=False,do_unfold=False,do_data=True,do_summary=False,new_scales=None,name=''):
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
    if spR2: spR2.update_var( var )
    if spT2: spT2.update_var( var )
    c = SuCanvas('P%d_%s_%s'%(m,name,'det' if do_unfold==False else 'unf'))
    M = PlotOptions()
    M.prefill_mc(err=do_errorsMC if do_unfold==False else False) # if unfolded, only show errors on final data
    if do_data:
        M.prefill_data(err=do_errorsDA) 
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
        else: #MC: use truth-level histograms if possible
            if m in (1,2):
                h.append( po.mc('mc', spT2.clone() if spT2 else spR2.clone() , name=M.names[i]) )
            elif m in (10,20):
                h.append( po.asym_mc('mc', spT2.clone() if spT2 else spR2.clone() , name=M.names[i]) )
    height = 'asym' if is_asym else 1.7
    title = var
    if is_asym:
        title='Detector-level asymmetry' if do_unfold==False else 'Truth-level asymmetry'
    c.plotAny(h,M=M,height=height,title=title)
    if do_summary:
        # do summary bin here!
        pass
    OMAP.append(c)

def plot_stack(spR2,var,q=2,m=0,new_scales=None,name=''):
    if new_scales!=None: SuStackElm.new_scales = new_scales
    spR2.update_var( var )
    c = SuCanvas('stack_'+var+'_'+SuSys.QMAP[q][1]+('_'+name if name !='' else ''))
    leg = ROOT.TLegend(0.55,0.70,0.88,0.88,var,"brNDC")
    hstack = po.stack('mc',spR2.clone(q=q),leg=leg)
    hdata = po.data('data',spR2.clone(q=q),leg=leg)
    c.plotStack(hstack,hdata,mode=m,leg=leg,height=1.7)
    OMAP.append(c)

def plot_stacks(spR2,histos,m=0,new_scales=None,name='',qs=(0,1,2)):
    """ A wrapper to make multiple stack plots for variables listed in histos[] array """
    for q in qs:
        for var in histos:
            plot_stack(spR2,var,q=q,m=m,new_scales=new_scales,name=name)

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

def test_from_slices(spR2,spT2,mode=0,name='test_slices'):  # test: reconstruction in eta slices
    c = SuCanvas(name)
    h1,h2=None,None
    if mode==0:
        h1 = po.sig('pos',spT.clone(q=0,do_unfold=False))
        h2 = po.sig('pos',spT.clone(q=0,do_unfold=False,histo='bin_%d/lpt:0:5')) #5 is overflow bin; -1 maps to 4 and show slight disagreement
    elif mode==1:
        h1 = po.data('pos',spR.clone(q=0,do_unfold=False))
        h2 = po.data('pos',spR.clone(q=0,do_unfold=False,histo='bin_%d/lpt:0:5')) #5 is overflow bin; -1 maps to 4 and show slight disagreement
    else:
        assert False,'Unsupported test_from_slices mode'
    M = PlotOptions()
    M.add('default','Default')
    M.add('fromslices','From slices')
    c.plotAny([h1,h2],M=M,height=1.7)
    #c.plotOne(h1,mode=2,height=1.7)
    OMAP.append(c)

def test_from_slices_sys(spR):
    c = SuCanvas('test_slices_sys_inbins')
    h1R = po.sig('pos',spR.clone(q=0,do_unfold=False))
    h2R = po.sig('pos',spR.clone(q=0,do_unfold=False,histo='bin_%d/lpt:0:5'))
    M = PlotOptions()
    M.add('default','default')
    M.add('inbins','inbins')
    c.plotOne(h2R,mode=2,height=1.7)
    OMAP.append(c)
    c = SuCanvas('test_slices_sys_default')
    c.plotOne(h1R,mode=2,height=1.7)
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

def june17_asymmetry_do(spTN2,pre,name='truth_'):
    pre2040=prunesub(pre,'l_pt','l_pt>20 && l_pt<40')
    pre4080=prunesub(pre,'l_pt','l_pt>40 && l_pt<80')
    plot_any(spTN2.clone(pre=pre),m=10,name=name+'_fiducial',do_data=False,new_scales=False)
    plot_any(spTN2.clone(pre=pre2040),m=10,name=name+'_lpt2040',do_data=False,new_scales=False)
    plot_any(spTN2.clone(pre=pre4080),m=10,name=name+'_lpt4080',do_data=False,new_scales=False)
def june17_asymmetry():
    pre = fortruth(opts.pre)
    # truth level:
    if True:
        june17_asymmetry_do(spTN.clone(weight=opts.cut),pre,name='truth_wADEF')
        june17_asymmetry_do(spTN.clone(weight=opts.cut+'*'+'lha_CT10'),pre,name='truth_wCT10')
        june17_asymmetry_do(spTN.clone(weight=opts.cut+'*'+'wptw'),pre,name='truth_wWPTW')
    # reco level:
    if True:
        june17_asymmetry_do(spRN.clone(weight=opts.cut),opts.pre,name='reco_wADEF')
        june17_asymmetry_do(spRN.clone(weight=opts.cut+'*'+'lha_CT10'),opts.pre,name='reco_wCT10')
        june17_asymmetry_do(spRN.clone(weight=opts.cut+'*'+'wptw'),opts.pre,name='reco_wWPTW')
    # histograms
    # truth
    plot_any(spTN.clone(weight=opts.cut,pre=pre,var='w_pt',histo='',bin='50,0,100'),var='w_pt',m=1,name='truth_fiducial_wpt_wADEF',do_data=False,new_scales=False)
    plot_any(spTN.clone(weight=opts.cut+'*'+'wptw',pre=pre,var='w_pt',histo='',bin='50,0,100'),var='w_pt',m=1,name='truth_fiducial_wpt_wWPTW',do_data=False,new_scales=False)
    # reco
    plot_any(spRN.clone(weight=opts.cut,pre=opts.pre,var='w_pt',histo='',bin='50,0,100',q=2),var='w_pt',m=1,name='reco_fiducial_wpt_wADEF',do_data=False,new_scales=False)
    plot_any(spRN.clone(weight=opts.cut+'*'+'wptw',pre=opts.pre,var='w_pt',histo='',bin='50,0,100',q=2),var='w_pt',m=1,name='reco_fiducial_wpt_wWPTW',do_data=False,new_scales=False)
    plot_stack(spRN.clone(weight=opts.cut,pre=opts.pre,var='w_pt',histo='',bin='50,0,100',q=2),var='w_pt',q=2,m=0,new_scales=False,name='wpt_stack_wADEF')
    plot_stack(spRN.clone(weight=opts.cut+'*'+'wptw',pre=opts.pre,var='w_pt',histo='',bin='50,0,100',q=2),var='w_pt',q=2,m=0,new_scales=False,name='wpt_stack_wWPTW')

# combined plots
if mode=='ALL' or mode=='all':
    if False: # june17 plots without data: asymmetries at truth and a bit at reco level
        cmd="""
        ./stack2.py --input ${input} --qcd 1.0 -b --var "fabs(l_eta)" --bin 10,0.0,2.5 --hsource "lepton_absetav" -o TEST -t TEST --pre "ptiso20/l_pt<0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1 && l_trigEF<0.2" --cut "mcw*puw*effw*trigw" -m ALL --bgsig 2 --bgqcd 0 #--metallsys #d0sig was 10, then was fabs(d0sig)<5.0
        """
        june17_asymmetry()
    if False:
        plots = ['lepton_absetav']
        #plots = ['lepton_absetav','lepton_pt','met','w_mt',"lepton_ptiso20r","lepton_ptiso30r","lepton_etiso30rcorr","njets"]
        plot_stacks(spR.clone(),plots,m=1)
    if True:
        plot_any(spR.clone(),spT.clone(),m=20,do_unfold=True,do_errorsDA=False)
        if False: # validate TH1 vs ntuple MC-only asymmetries. Small difference see in truth tree -not sure why
            plot_any(spRN.clone(path=path_reco),None,m=20,name='reco_ntuple',do_data=False,new_scales=False)
            plot_any(spR.clone(),None,name='reco_histo',m=20,do_data=False,new_scales=False)
            plot_any(spTN.clone(),None,name='truth_ntuple',m=20,do_data=False,new_scales=False)
            plot_any(spT.clone(),None,name='truth_histo',m=20,do_data=False,new_scales=False)
    if False: # compares TH1 vs ntuple based basic histogram results, ignoring QCD normalization issues
        spR.enable_nominal()
        test_ntuple_histo(spR.clone(),name='asym_histo',new_scales=False)
        test_ntuple_histo(spRN.clone(path=path_reco),name='asym_ntuple_all',new_scales=False)
        newpre=opts.pre + ' && ' + 'fabs(d0sig)<5.0'
        test_ntuple_histo(spRN.clone(path=path_reco,pre=newpre),name='asym_ntuple_d05',new_scales=False)
        newpre=opts.pre + ' && ' + 'fabs(d0sig)<10.0'
        test_ntuple_histo(spRN.clone(path=path_reco,pre=newpre),name='asym_ntuple_d10',new_scales=False)
    if False: # studying effects of QCD normalization in histograms
        spR.enable_nominal()
        # FIXME TODO: mechanism to try out different QCD backgrounds. Plus: different EWK (for qcdsub) - both should be in systematics!
        plots = ['lepton_absetav','lepton_pt','met','w_mt',"lepton_ptiso20r","lepton_ptiso30r","lepton_etiso30rcorr"]
        plot_any(spR.clone(),spT.clone(),m=20,name='bbmu15_default',new_scales=False)
        plot_stacks(spR.clone(),plots,name='bbmu15_default')
        plot_any(spR.clone(),spT.clone(),m=20,name='bbmu15_qcdfit',new_scales=True)
        plot_stacks(spR.clone(),plots,name='bbmu15_qcdfit')
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
        h.summary_bin(fname='index.html')
    if False:
        test_unfolding(spR.clone(),spT.clone(),asym=False)
    if False:
        test_from_slices(spR.clone(),spT.clone(),1)
    if False:
        c = SuCanvas('test_slices_norm')
        SuStack.QCD_SYS_SCALES = True
        print '--------->', 'Making default'
        h1R = po.asym_data_sub('pos',spR.clone(q=0,do_unfold=False))
        print '--------->', 'Making inbins combined'
        h2R = po.asym_data_sub('pos',spR.clone(q=0,do_unfold=False,histo='bin_%d/lpt:0:5'))
        hpt = []
        print '--------->', 'Making inbins pt20..25'
        hpt.append( po.asym_data_sub('pos',spR.clone(q=0,do_unfold=False,histo='bin_%d/lpt:1:1')) )
        print '--------->', 'Making inbins pt25..40'
        hpt.append( po.asym_data_sub('pos',spR.clone(q=0,do_unfold=False,histo='bin_%d/lpt:2:2')) )
        #hpt.append( po.asym_data_sub('pos',spR.clone(q=0,do_unfold=False,histo='bin_%d/lpt:3:3')) )
        #hpt.append( po.asym_data_sub('pos',spR.clone(q=0,do_unfold=False,histo='bin_%d/lpt:4:4')) )
        M = PlotOptions()
        M.add('default','default')
        M.add('inbins','inbins')
        M.add('pt2025','pt2025')
        M.add('pt2540','pt2540')
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
        h2R = po.qcd('pos',spR.clone(q=0,do_unfold=False,histo='bin_%d/lpt:0:5'))
        M = PlotOptions()
        M.add('default','default')
        M.add('inbins','inbins')
        c.plotMany([h1R,h2R],M=M,mode=0,height=1.7)
        #c.plotOne(h1R,mode=2,height=1.7)
        OMAP.append(c)
    if False: # antonio study of muon exms_pt/id_pt in low-pt muon region. refLine = 0.0,3.0
        plot_stack(spRN.clone(var='l_pt_exms/l_pt_id',histo='',bin='50,0.5,1.5',q=2),var='l_pt_exms/l_pt_id',q=2,m=0,new_scales=False,name='antonio_ptEXMS_ptID')
    
if mode=='1': # total stack histo
    spR2 = spR
    c = SuCanvas()
    leg = ROOT.TLegend(0.55,0.70,0.88,0.88,QMAP[q][3],"brNDC")
    hmc,hdata = None,None
    hmc = po.stack('mc',spR2,leg=leg)
    hdata = po.data('data',spR2,leg=leg)
    c.plotStackHisto(hmc.flat[0].stack,hdata.flat[0].h,leg)

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
    leg = ROOT.TLegend(0.55,0.70,0.88,0.88,QMAP[q][3],"brNDC")
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
    leg = ROOT.TLegend(0.55,0.70,0.88,0.88,QMAP[q][3],"brNDC")
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
    leg = ROOT.TLegend(0.55,0.70,0.88,0.88,QMAP[q][3],"brNDC")
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
    leg = ROOT.TLegend(0.55,0.70,0.88,0.88,QMAP[q][3],"brNDC")
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

for key,val in po.fits.iteritems():
    print 'Adding a normalization fit:',key
    val.savename = 'qcdfit_'+key
    OMAP.append(val)

# save images
if not opts.antondb:
    if not os.path.isdir(opts.output):
        os.makedirs(SuCanvas.savedir)
    # save main plot
    #c.SaveAs('%s_%s_%s_%s_%s_%s'%(opts.tag,os.path.basename(opts.input),QMAP[opts.charge][1],opts.var,opts.cut,mode),'png',DIR=DIR)
    # save all plots
    for i,obj in enumerate(OMAP):
        savename = obj.savename
        assert hasattr(obj,'SaveAs') # SuCanvas
        obj.SaveSelf()

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
