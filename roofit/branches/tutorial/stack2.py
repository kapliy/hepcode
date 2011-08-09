#!/usr/bin/env python

_QUALITY = ' && idhits==1 && fabs(z0)<10. && fabs(d0sig)<10. && fabs(l_pt_id-l_pt_exms)/l_pt_id<0.5'
_PRE_PETER  = 'l_pt>20.0 && ptiso40<2.0 && etiso40<2.0 && met>25.0 && w_mt>40.0'+_QUALITY
_PRE_JORDAN = 'l_pt>25.0 && ptiso20/l_pt<0.1 && met>25.0 && w_mt>40.0'+_QUALITY
_PRE_JORDANALT = 'l_pt>25.0 && ptiso20/l_pt<0.1 && ptiso30/l_pt<0.15 && met>25.0 && w_mt>40.0'+_QUALITY

# X - tag muon (doesn't change), Y - probe muon used to measure efficiency ( = Y after/before specific cut)
_BEF = '(lP_idhits==1 && fabs(lP_z0)<10. && lP_pt>20.0 && lN_idhits==1 && fabs(lN_z0)<10. && lN_pt>20.0) && Z_m>81.0 && Z_m<101.0 && (lP_q*lN_q)<0 && fabs(lP_z0-lN_z0)<3 && fabs(lP_d0-lN_d0)<2 && fabs(lP_phi-lN_phi)>2.0 && lX_ptiso20/lX_pt<0.1'
_AFT = _BEF + ' && ' + 'lY_ptiso20/lY_pt<0.1'
#_AFT = _BEF + ' && ' + 'lY_ptiso40<2.0 && lY_etiso40<2.0'
_AFT = _BEF + ' && ' + 'lY_etiso40<2.0'

if False:
    _BEF = '(lP_idhits==1 && fabs(lP_z0)<10. && lP_pt>20.0 && fabs(lN_z0)<10. && lN_pt>20.0) && Z_m>81.0 && Z_m<101.0 && (lP_q*lN_q)<0 && fabs(lP_z0-lN_z0)<3 && fabs(lP_d0-lN_d0)<2 && fabs(lP_phi-lN_phi)>2.0 && lX_ptiso20/lX_pt<0.1 && lY_ptiso20/lY_pt<0.1'
    _AFT = _BEF + ' && ' + 'lY_idhits==1'

import sys,re
from optparse import OptionParser
from load_data import SaveAs
import antondb
parser = OptionParser()
parser.add_option("-m", "--mode",dest="mode",
                  type="int", default=1,
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
                  type="string", default='l_eta',
                  help="Variable to plot")
parser.add_option("--bin",dest="bin",
                  type="string", default='50,-2.5,2.5',
                  help="Binning for var")
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
                  type="string", default=None,
                  help="Path to data histogram (if set, use it instead of ntuple")
parser.add_option("--rebin",dest="rebin",
                  type="int", default=1,
                  help="Rebin histograms")
parser.add_option("--antondb",dest="antondb",
                  type="string", default=None,
                  help="Tag for antondb output container")
#329602.0 up to F3 (182519) EF_mu18_MG
#490814.0 up to G3 (183021) EF_mu18_MG
#689279.0 up to G5 (183347) EF_mu20_MG
#832854.0 up to H1 (183602) EF_mu20_MG
#1035040.0 up to H4 (184169) EF_mu18_MG
parser.add_option("--lumi",dest="lumi",
                  type="float", default=1035040.0,
                  help="Integrated luminosity for data (in nb^-1)")
parser.add_option("--qcd",dest="qcdscale",
                  type="string", default='1.0',
                  help="QCD scale factor")
parser.add_option("-o", "--output",dest="output",
                  type="string", default="",
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
parser.add_option("-q", "--charge",dest="charge",
                  type="int", default=2,
                  help="Which charge to plot: 0=POS, 1=NEG, 2=ALL")
parser.add_option("--bgqcd",dest="bgqcd",
                  type="int", default=0,
                  help="QCD: 0=Pythia mu15x, 1=Pythia J0..J5")
parser.add_option("--bgsig",dest="bgsig",
                  type="int", default=0,
                  help="Background: 0=Pythia, 1=MC@NLO, 3=W+jets Jimmy")
(opts, args) = parser.parse_args()
mode = opts.mode
print "MODE =",mode
gbg = []; COUT = [];
# antondb containers
VMAP = {}; OMAP = []
VMAP['cmd']=' '.join(sys.argv)

import ROOT
from MC import *
#ROOT.TH1.AddDirectory(ROOT.kFALSE)    # ensure that we own all the histograms
ROOT.gROOT.SetBatch(opts.batch)
ROOT.TH1.SetDefaultSumw2()
SetStyle("AtlasStyle.C")
#ROOT.gStyle.SetOptFit(1111);

from SuCanvas import *
from SuData import *
SuSample.rootpath = opts.input
SuSample.hsource = opts.hsource
SuSample.hcharge = opts.charge
SuSample.lumi = opts.lumi
SuSample.rebin = opts.rebin

#SetStyle()
def QAPP(path,iq):
    htmp = path.split('/');
    htmp.insert(-1,QMAP[iq][1])
    return '/'.join(htmp) if iq in (0,1) else path

POS,NEG,ALL=range(3)
QMAP = {}
QMAP[POS] = (0,'POS','(l_q>0)','mu+ only')
QMAP[NEG] = (1,'NEG','(l_q<0)','mu- only')
QMAP[ALL] = (2,'ALL','(l_q!=0)','mu+ and mu-')
QMAPZ = {}
QMAPZ[POS] = (0,'POS','lP','mu+')
QMAPZ[NEG] = (1,'NEG','lN','mu-')

# MC stack order
po = SuStack()
if opts.ntuple=='w':
    if opts.bgsig in (0,1): # w inclusive
        po.add(label='t#bar{t}',samples='mc_jimmy_ttbar',color=ROOT.kGreen,flags=['bg','mc','ewk'])
        po.add(label='Z#rightarrow#tau#tau',samples='mc_ztautau',color=ROOT.kMagenta,flags=['bg','mc','ewk'])
        po.add(label='W#rightarrow#tau#nu',samples='mc_wtaunu',color=ROOT.kYellow,flags=['bg','mc','ewk'])
        po.add(label='Z#rightarrow#mu#mu',samples='mc_zmumu',color=ROOT.kRed,flags=['bg','mc','ewk'])
        if opts.bgqcd==0:
            po.add(label='bbmu15X/ccmu15X',samples=['mc_bbmu15x','mc_ccmu15x'],color=ROOT.kCyan,flags=['bg','mc','qcd'])
        elif opts.bgqcd==1:
            po.Add(label='QCD J0..J5',samples=['mc_J%d'%v for v in xrange(5)],color=ROOT.kCyan,flags=['bg','mc','qcd'])
        if opts.bgsig==0:
            po.add(label='W#rightarrow#mu#nu',samples='mc_wmunu',color=10,flags=['sig','mc','ewk'])
        elif opts.bgsig==1:
            po.Add(label='W#rightarrow#mu#nu',samples=['mc_wminmunu','mc_wplusmunu'],color=10,flags=['sig','mc','ewk'])
    elif opts.bgsig in (3,): # w+jets
        po.Add(label='t#bar{t}',samples='mc_jimmy_ttbar',color=ROOT.kGreen,flags=['bg','mc','ewk'])
        #po.Add(label='Z#rightarrow#tau#tau+jets',samples=['mc_jimmy_ztautau_np%d'%v for v in range(6)],color=ROOT.kMagenta)
        po.Add(label='Z#rightarrow#tau#tau',samples='mc_ztautau',color=ROOT.kMagenta,flags=['bg','mc','ewk'])
        #po.Add(label='W#rightarrow#tau#nu+jets',samples=['mc_jimmy_wtaunu_np%d'%v for v in range(6)],color=ROOT.kYellow)
        po.Add(label='W#rightarrow#tau#nu',samples='mc_wtaunu',color=ROOT.kYellow,flags=['bg','mc','ewk'])
        po.Add(label='Z#rightarrow#mu#mu+jets',samples=['mc_jimmy_zmumu_np%d'%v for v in range(6)],color=ROOT.kRed,flags=['bg','mc','ewk'])
        po.Add(label='WZ/ZZ',samples=['mc_jimmy_wz_np%d'%v for v in range(4)]+['mc_jimmy_zz_np%d'%v for v in range(4)],color=11,flags=['bg','mc','ewk'])
        po.Add(label='WW',samples=['mc_jimmy_ww_np%d'%v for v in range(4)],color=12,flags=['bg','mc','ewk'])
        po.Add(label='bbmu15X/ccmu15X',samples=['mc_bbmu15x','mc_ccmu15x'],color=ROOT.kCyan,flags=['bg','mc','qcd'])
        po.Add(label='W#rightarrow#mu#nu+jets',samples=['mc_jimmy_wmunu_np%d'%v for v in range(6)],color=10,flags=['sig','mc','ewk'])
elif opts.ntuple=='z':
    if opts.bgsig in (0,1): # z inclusive
        po.add(label='t#bar{t}',samples='mc_jimmy_ttbar',color=ROOT.kGreen,flags=['bg','mc','ewk'])
        po.add(label='W#rightarrow#mu#nu',samples='mc_wmunu',color=10,flags=['bg','mc','ewk'])
        po.add(label='Z#rightarrow#tau#tau',samples='mc_ztautau',color=ROOT.kMagenta,flags=['bg','mc','ewk'])
        po.add(label='W#rightarrow#tau#nu',samples='mc_wtaunu',color=ROOT.kYellow,flags=['bg','mc','ewk'])
        if opts.bgqcd==0:
            po.add(label='bbmu15X/ccmu15X',samples=['mc_bbmu15x','mc_ccmu15x'],color=ROOT.kCyan,flags=['bg','mc','qcd'])
        elif opts.bgqcd==1:
            po.Add(label='QCD J0..J5',samples=['mc_J%d'%v for v in xrange(5)],color=ROOT.kCyan,flags=['bg','mc','qcd'])
        po.add(label='Z#rightarrow#mu#mu',samples='mc_zmumu',color=ROOT.kRed,flags=['sig','mc','ewk'])
    elif opts.bgsig in (3,): # z+jets
        po.Add(label='t#bar{t}',samples='mc_jimmy_ttbar',color=ROOT.kGreen,flags=['bg','mc','ewk'])
        po.Add(label='W#rightarrow#mu#nu+jets',samples=['mc_jimmy_wmunu_np%d'%v for v in range(6)],color=10,flags=['bg','mc','ewk'])
        #po.Add(label='Z#rightarrow#tau#tau+jets',samples=['mc_jimmy_ztautau_np%d'%v for v in range(6)],color=ROOT.kMagenta)
        po.Add(label='Z#rightarrow#tau#tau',samples='mc_ztautau',color=ROOT.kMagenta,flags=['bg','mc','ewk'])
        #po.Add(label='W#rightarrow#tau#nu+jets',samples=['mc_jimmy_wtaunu_np%d'%v for v in range(6)],color=ROOT.kYellow)
        po.Add(label='W#rightarrow#tau#nu',samples='mc_wtaunu',color=ROOT.kYellow,flags=['bg','mc','ewk'])
        po.Add(label='WZ/ZZ',samples=['mc_jimmy_wz_np%d'%v for v in range(4)]+['mc_jimmy_zz_np%d'%v for v in range(4)],color=11,flags=['bg','mc','ewk'])
        po.Add(label='WW',samples=['mc_jimmy_ww_np%d'%v for v in range(4)],color=12,flags=['bg','mc','ewk'])
        po.Add(label='bbmu15X/ccmu15X',samples=['mc_bbmu15x','mc_ccmu15x'],color=ROOT.kCyan,flags=['bg','mc','qcd'])
        po.Add(label='Z#rightarrow#mu#mu+jets',samples=['mc_jimmy_zmumu_np%d'%v for v in range(6)],color=ROOT.kRed,flags=['sig','mc','ewk'])
else:
    assert False,'Unsupported --ntuple: %s'%opts.ntuple
po.add(label='2011 data',samples=['data_period%s'%s for s in ('B','D','E','F','G','H')],color=ROOT.kBlack,flags=['data'])

# Pre-load the ntuples
po.addchain('st_%s_final/ntuple'%opts.ntuple)
po.auto()
po.print_counts()

gbg = []
q = opts.charge

if mode==1: # total stack histo
    c = SuCanvas()
    leg = ROOT.TLegend(0.55,0.70,0.88,0.88,QMAP[q][3],"brNDC")
    hmc = po.stack('mc',opts.var,opts.bin,'(%s) * (%s) * (%s)'%(QMAP[q][2],opts.cut,opts.pre),leg=leg)
    hdata   = po.data('data',opts.var,opts.bin,'(%s) * (%s) * (%s)'%(QMAP[q][2],opts.cut,opts.pre),leg=leg)
    c.plotStackHisto(hmc,hdata,leg)

if mode==11: # asymmetry (not bg-subtracted)
    assert opts.ntuple=='w','ERROR: asymmetry can only be computed for the w ntuple'
    hsig,hd    = [None]*2,[None]*2
    hsig[POS]  = po.sig('signalPOS',opts.var,opts.bin,'(%s) * (%s) * (%s)'%(QMAP[POS][2],opts.cut,opts.pre))
    hd[POS]    = po.data('dataPOS',opts.var,opts.bin,'(%s) * (%s) * (%s)'%(QMAP[POS][2],opts.cut,opts.pre))
    hsig[NEG]  = po.sig('signalNEG',opts.var,opts.bin,'(%s) * (%s) * (%s)'%(QMAP[NEG][2],opts.cut,opts.pre))
    hd[NEG]    = po.data('dataNEG',opts.var,opts.bin,'(%s) * (%s) * (%s)'%(QMAP[NEG][2],opts.cut,opts.pre))
    c = SuCanvas()
    c.plotAsymmetry(hd[POS],hd[NEG],hsig[POS],hsig[NEG])

if mode==12: # asymmetry (bg-subtracted)
    assert opts.ntuple=='w','ERROR: asymmetry can only be computed for the w ntuple'
    hsig,hd_sig  = [None]*2,[None]*2
    hsig[POS]    = po.sig('signalPOS',opts.var,opts.bin,'(%s) * (%s) * (%s)'%(QMAP[POS][2],opts.cut,opts.pre))
    hd_sig[POS]  = po.data_sub('dataPOS',opts.var,opts.bin,'(%s) * (%s) * (%s)'%(QMAP[POS][2],opts.cut,opts.pre))
    hsig[NEG]    = po.sig('signalNEG',opts.var,opts.bin,'(%s) * (%s) * (%s)'%(QMAP[NEG][2],opts.cut,opts.pre))
    hd_sig[NEG]  = po.data_sub('dataNEG',opts.var,opts.bin,'(%s) * (%s) * (%s)'%(QMAP[NEG][2],opts.cut,opts.pre))
    c = SuCanvas()
    c.plotAsymmetry(hd_sig[POS],hd_sig[NEG],hsig[POS],hsig[NEG])

if mode in (101,102): # tag and probe
    assert opts.ntuple=='z','ERROR: tag-and-probe can only be computed for the z ntuple'
    hda_bef = [None]*2
    hda_aft = [None]*2
    hmc_bef = [None]*2
    hmc_aft = [None]*2
    for iq in xrange(2): # probe charge (what we're measuring)
        iqp = iq
        iqt = 0 if iq==1 else 1
        var = opts.var.replace('lY',QMAPZ[iqp][2])
        pre_bef = opts.prebef.replace('lX',QMAPZ[iqt][2]).replace('lY',QMAPZ[iqp][2])
        pre_aft = opts.preaft.replace('lX',QMAPZ[iqt][2]).replace('lY',QMAPZ[iqp][2])
        hmc_bef[iq] = po.sig('mc_bef_%s'%QMAPZ[iq][1],var,opts.bin,'(%s) * (%s)'%(opts.cut,pre_bef))
        hmc_aft[iq] = po.sig('mc_aft_%s'%QMAPZ[iq][1],var,opts.bin,'(%s) * (%s)'%(opts.cut,pre_aft))
        if mode==101:   # default data
            hda_bef[iq] = po.data('da_bef_%s'%QMAPZ[iq][1],var,opts.bin,'(%s) * (%s)'%(opts.cut,pre_bef))
            hda_aft[iq] = po.data('da_aft_%s'%QMAPZ[iq][1],var,opts.bin,'(%s) * (%s)'%(opts.cut,pre_aft))
        elif mode==102: # bg-subtracted data
            hda_bef[iq] = po.data_sub('da_bef_%s'%QMAPZ[iq][1],var,opts.bin,'(%s) * (%s)'%(opts.cut,pre_bef))
            hda_aft[iq] = po.data_sub('da_aft_%s'%QMAPZ[iq][1],var,opts.bin,'(%s) * (%s)'%(opts.cut,pre_aft))
        else:
            assert False,'Unknown tag-and-probe mode'
    c = SuCanvas()
    c.plotTagProbe(hda_bef,hda_aft,hmc_bef,hmc_aft,xtitle=opts.var)

if mode==99: # Floating QCD normalization
    import SuFit
    f = SuFit.SuFit()
    f.addFitVar( opts.var, float(opts.bin.split(',')[1]) , float(opts.bin.split(',')[2]) , opts.var );
    # get histograms
    hdata   = po.data('data',opts.var,opts.bin,'(%s) * (%s) * (%s)'%(QMAP[q][2],opts.cut,opts.pre))
    hfixed = po.ewk('bgfixed',opts.var,opts.bin,'(%s) * (%s) * (%s)'%(QMAP[q][2],opts.cut,opts.pre))
    hfree = po.qcd('bgfree',opts.var,opts.bin,'(%s) * (%s) * (%s)'%(QMAP[q][2],opts.cut,opts.pre))
    # run SuFit
    hdata.getLegendName = lambda : 'DATA'
    hfixed.getLegendName = lambda : 'EWK backgrounds'
    hfree.getLegendName = lambda : 'QCD (bb/cc mu15X)'
    f.setDataBackgrounds(hdata,hfixed,hfree)
    f.doFit()
    c,frame = f.drawFits()

if not opts.antondb:
    c.SaveAs('%s_%s_%s_%s_%s_%d'%(opts.tag,opts.input,QMAP[opts.charge][1],opts.var,opts.cut,mode),'png')

# save everything
if len(COUT)>0:
    VMAP['COUT']=[]
    for l in COUT:
        print l
        VMAP['COUT'].append(l)
c._canvas.SetName('%s_m%d_%s'%(opts.tag,mode,opts.var))
OMAP += [c._canvas,]
if (len(VMAP)>0 or len(OMAP)>0) and opts.antondb:
    a = antondb.antondb(opts.antondb)
    path = os.path.join('/stack/',opts.tag,QMAP[opts.charge][1])
    if len(VMAP)>0:
        a.add(path,VMAP)
    if len(OMAP)>0:
        a.add_root(path,OMAP)
