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
                  type="string", default='l_eta',
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
parser.add_option("--refline",dest="refline",
                  type="string", default='0.5,1.5',
                  help="The range for ratio histogram reference line used in stack plots")
parser.add_option("--antondb",dest="antondb",
                  type="string", default=None,
                  help="Tag for antondb output container")
parser.add_option("--effroot",dest="effroot",
                  type="string", default=None,
                  help="Location of acceptance efficiency histograms (correction to truth level)")
#329602.0 up to F3 (182519) EF_mu18_MG
#490814.0 up to G3 (183021) EF_mu18_MG
#689279.0 up to G5 (183347) EF_mu20_MG
#832854.0 up to H1 (183602) EF_mu20_MG
#1035040.0 up to H4 (184169) EF_mu18_MG
#1340030.0 up to I4 (186493) EF_mu18_MG
#1469.13+2128.61 = B-I + J-L (L7). mu18_MG, followed by mu18_MG_medium
_DATA_PERIODS = ('B','D','E','F','G','H','I')
parser.add_option("--lumi",dest="lumi",
                  type="float", default=1340030.0,
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
                  help="Background: 0=Pythia, 1=MC@NLO, 2=Alpgen(signal only), 3=Alpgen(all)")
parser.add_option('-f',"--func",dest="func",
                  type="string", default='gaus',
                  help="func = {gaus,egge,voig,voigbg,bw,bwfull}{0=none;1=gaus;2=double-gaus;3=crystal-ball}")
(opts, args) = parser.parse_args()
mode = opts.mode
print "MODE =",mode
#print "PRE =",opts.pre
#print "BEF =",opts.prebef
#print "AFT =",opts.preaft
gbg = []; COUT = [];
# antondb containers
VMAP = {}; OMAP = []
VMAP['cmd']=' '.join(sys.argv)

#import ROOT
import os,sys
if not 'libPyRoot' in sys.modules: #hack to get rid of TenvRec warnings
    from ROOT import gROOT
    sys.modules['libPyROOT'].gROOT.GetListOfGlobals().FindObject('gErrorIgnoreLevel').GetAddress().__setitem__(0,3000)
    import ROOT
    gErrorIgnoreLevel=ROOT.kInfo
    sys.modules['libPyROOT'].gROOT.GetListOfGlobals().FindObject('gErrorAbortLevel').GetAddress().__setitem__(0,5001)

from MC import *
#ROOT.TH1.AddDirectory(ROOT.kFALSE)    # ensure that we own all the histograms
ROOT.SetSignalPolicy(ROOT.kSignalFast)
ROOT.gROOT.SetBatch(opts.batch)
ROOT.TH1.SetDefaultSumw2()
SetStyle("AtlasStyle.C")
#ROOT.gStyle.SetOptFit(1111);

from FileLock import FileLock
from SuCanvas import *
SuCanvas._refLineMin = float(opts.refline.split(',')[0])
SuCanvas._refLineMax = float(opts.refline.split(',')[1])
from SuData import *
SuSample.rootpath = opts.input
SuSample.hsource = opts.hsource
SuSample.hcharge = opts.charge
SuSample.lumi = opts.lumi
SuSample.rebin = opts.rebin
SuSample.qcdscale = float(opts.qcdscale) if not opts.qcdscale in ('AUTO','auto','Auto') else 1.0

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

def prunesub(pre,lvar,sub):
    """ substitutes all instances of 'lvar' in pre with 'sub' """
    res = []
    lvars=[]
    if type(lvar)==type([]) or type(lvar)==type(()):
        lvars = lvar
    else:
        lvars = (lvar,)
    already_subbed = False
    for elm in pre.split(' && '):
        keep = True
        for lvar in lvars:
            if re.match(lvar,elm):
                print '-> pruning away: ',elm
                keep = False
                break
        if keep:
            res.append(elm)
        elif sub and not already_subbed:
            res.append(sub)
            already_subbed = True
    return ' && '.join(res)

def prune(pre,lvar):
    """ removes all instances of 'lvar' from pre """
    return prunesub(pre,lvar,None)

w2zsub = []
w2zsub.append(('ptiso','lX_ptiso'))
w2zsub.append(('etiso','lX_etiso'))
w2zsub.append(('l_q','lX_q'))
w2zsub.append(('l_pt','lX_pt'))
w2zsub.append(('l_eta','lX_eta'))
w2zsub.append(('l_phi','lX_phi'))
w2zsub.append(('idhits','lX_idhits'))
w2zsub.append(('d0','lX_d0'))
w2zsub.append(('z0','lX_z0'))
def pre_w2z(pre):
    """ Converts a pre string for w ntuple into a corresponding one for z """
    pre = prune(pre,('met','w_mt'))
    xtra = "Z_m>70 && Z_m<110 && fabs(lP_z0-lN_z0)<3 && fabs(lP_d0-lN_d0)<2 && fabs(lP_phi-lN_phi)>0.0 && (lP_q*lN_q)<0"
    res = []
    for elm in pre.split(' && '):
        changed = False
        for isub in w2zsub:
            if re.search(isub[0],elm):
                elm = elm.replace(isub[0],isub[1]).replace('l_pt','lX_pt')
                res.append ( elm.replace('lX',QMAPZ[0][2]) + ' && ' + elm.replace('lX',QMAPZ[1][2]) )
                changed = True
                break
        if not changed:
            res.append(elm)
    return ' && '.join(res) + ' && ' + xtra

def fortruth(pre):
    """ removes pre variables that are not applicable to truth tracks """
    res = []
    for elm in pre.split(' && '):
        if re.search('iso',elm) or re.match('idhits',elm) or re.search('d0',elm) or re.search('z0',elm) or re.search('exms',elm):
            continue
        res.append(elm)
    return ' && '.join(res)

def metfitreg(pre):
    """ met fit region from w+jets 2010 analysis """
    res = []
    for elm in pre.split(' && '):
        if re.match('w_mt',elm):
            res.append('w_mt>35.0')
        elif re.match('met',elm):
            res.append('met>15.0')
        else:
            res.append(elm)
    return ' && '.join(res)

def qcdreg(pre):
    """ qcd-enriched sample to get data-driven template """
    res = []
    for elm in pre.split(' && '):
        if re.match('fabs\(d0',elm) or re.match('d0',elm):
            res.append('fabs(d0sig)>3.0 && fabs(d0)>0.1 && fabs(d0)<0.4')
        else:
            res.append(elm)
    return ' && '.join(res)

def revisoreg(pre):
    """ qcd-enriched sample to get data-driven template
    This version reverses isolation, which allegedly affects pt spectrum
    ptiso20/l_pt<0.1
    ptiso40<2.0 && etiso40<2.0
    """
    res = []
    for elm in pre.split(' && '):
        if re.match('ptiso',elm) or re.match('etiso',elm):
            ws = elm.split('<')
            assert len(ws)==2
            res.append( ws[0] + '>' + ws[1] )
        else:
            res.append(elm)
    return ' && '.join(res)

def looseisoreg(pre):
    """ Loose sample needed for matrix method QCD estimation """
    return prune(pre, ('ptiso','lP_ptiso','l_ptiso','etiso','lP_etiso','lN_etiso') )

def run_fit(pre,var='met',bin='100,5,100',cut='mcw*puw'):
    import SuFit
    f = SuFit.SuFit()
    f.addFitVar( var, float(bin.split(',')[1]) , float(bin.split(',')[2]) , var );
    # get histograms
    hdata   = po.data('data',var,bin,'(%s) * (%s) * (%s)'%(QMAP[q][2],cut,pre))
    hfixed = po.ewk('bgfixed',var,bin,'(%s) * (%s) * (%s)'%(QMAP[q][2],cut,pre))
    hfree = po.qcd('bgfree',var,bin,'(%s) * (%s) * (%s)'%(QMAP[q][2],cut,pre))
    assert hfixed,'Failed to find fixed backgrounds'
    # run SuFit
    hdata.getLegendName = lambda : 'DATA'
    hfixed.getLegendName = lambda : 'EWK backgrounds'
    hfree.getLegendName = lambda : 'QCD (bb/cc mu15X)'
    f.setDataBackgrounds(hdata,hfixed,hfree)
    f.doFit()
    tmp = f.drawFits()
    gbg.append((f,hdata,hfixed,hfree,tmp))
    return tmp[0],tmp[1],f.scales[0]

def renormalize():
    """ Normalizes MET template """
    if not opts.qcdscale in ('AUTO','auto','Auto'):
        return
    fitpre = metfitreg(opts.pre)
    dum1,dum2,scale = run_fit(pre=fitpre)
    SuSample.qcdscale = 1.0

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
if opts.bgsig in (0,1,2): # w inclusive
    pw.add(label='t#bar{t}',samples='mc_jimmy_ttbar',color=ROOT.kGreen,flags=['bg','mc','ewk'])
    if False: # until this is run by Antonio in v1_26
        pw.add(label='Z#rightarrow#tau#tau',samples=['mc_jimmy_ztautau_np%d'%v for v in range(6)],color=ROOT.kMagenta,flags=['bg','mc','ewk'])
    else:
        pw.add(label='Z#rightarrow#tau#tau',samples='mc_ztautau',color=ROOT.kMagenta,flags=['bg','mc','ewk'])
    pw.add(label='W#rightarrow#tau#nu',samples='mc_wtaunu',color=ROOT.kYellow,flags=['bg','mc','ewk'])
    pw.add(label='Z#rightarrow#mu#mu',samples='mc_zmumu',color=ROOT.kRed,flags=['bg','mc','ewk'])
    if opts.bgqcd==0:
        pw.add(label='bbmu15X/ccmu15X',samples=['mc_bbmu15x','mc_ccmu15x'],color=ROOT.kCyan,flags=['bg','mc','qcd'])
    elif opts.bgqcd==1:
        pw.add(label='QCD J0..J5',samples=['mc_J%d'%v for v in xrange(5)],color=ROOT.kCyan,flags=['bg','mc','qcd'])
    if opts.bgsig==0:
        pw.add(label='W#rightarrow#mu#nu',samples='mc_wmunu',color=10,flags=['sig','mc','ewk'])
    elif opts.bgsig==1:
        pw.add(label='W#rightarrow#mu#nu',samples=['mc_wminmunu','mc_wplusmunu'],color=10,flags=['sig','mc','ewk'])
    elif opts.bgsig==2:
        pw.add(label='W#rightarrow#mu#nu+jets',samples=['mc_jimmy_wmunu_np%d'%v for v in range(6)],color=10,flags=['sig','mc','ewk'])
    # cache some samples for special studies. Disabled from stacks using a 'no' flag.
    pw.add(label='pythia',samples='mc_wmunu',color=10,flags=['sig','mc','ewk','no'])
    pw.add(label='mcnlo',samples=['mc_wminmunu','mc_wplusmunu'],color=10,flags=['sig','mc','ewk','no'])
    pw.add(label='alpgen',samples=['mc_jimmy_wmunu_np%d'%v for v in range(6)],color=10,flags=['sig','mc','ewk','no'])
    pw.add(label='qcd',samples=['mc_bbmu15x'],color=ROOT.kCyan,flags=['bg','mc','qcd','no'])
elif opts.bgsig in (3,): # w+jets
    pw.add(label='t#bar{t}',samples='mc_jimmy_ttbar',color=ROOT.kGreen,flags=['bg','mc','ewk'])
    #pw.add(label='Z#rightarrow#tau#tau+jets',samples=['mc_jimmy_ztautau_np%d'%v for v in range(6)],color=ROOT.kMagenta)
    pw.add(label='Z#rightarrow#tau#tau',samples='mc_ztautau',color=ROOT.kMagenta,flags=['bg','mc','ewk'])
    #pw.add(label='W#rightarrow#tau#nu+jets',samples=['mc_jimmy_wtaunu_np%d'%v for v in range(6)],color=ROOT.kYellow)
    pw.add(label='W#rightarrow#tau#nu',samples='mc_wtaunu',color=ROOT.kYellow,flags=['bg','mc','ewk'])
    pw.add(label='Z#rightarrow#mu#mu+jets',samples=['mc_jimmy_zmumu_np%d'%v for v in range(6)],color=ROOT.kRed,flags=['bg','mc','ewk'])
    pw.add(label='WZ/ZZ',samples=['mc_jimmy_wz_np%d'%v for v in range(4)]+['mc_jimmy_zz_np%d'%v for v in range(4)],color=11,flags=['bg','mc','ewk'])
    pw.add(label='WW',samples=['mc_jimmy_ww_np%d'%v for v in range(4)],color=12,flags=['bg','mc','ewk'])
    pw.add(label='bbmu15X/ccmu15X',samples=['mc_bbmu15x','mc_ccmu15x'],color=ROOT.kCyan,flags=['bg','mc','qcd'])
    pw.add(label='W#rightarrow#mu#nu+jets',samples=['mc_jimmy_wmunu_np%d'%v for v in range(6)],color=10,flags=['sig','mc','ewk'])
# z samples:
if opts.bgsig in (0,1,2): # z inclusive
    pz.add(label='t#bar{t}',samples='mc_jimmy_ttbar',color=ROOT.kGreen,flags=['bg','mc','ewk'])
    pz.add(label='W#rightarrow#mu#nu',samples='mc_wmunu',color=10,flags=['bg','mc','ewk'])
    if False: # until this is run by Antonio in v1_26
        pz.add(label='Z#rightarrow#tau#tau',samples=['mc_jimmy_ztautau_np%d'%v for v in range(6)],color=ROOT.kMagenta,flags=['bg','mc','ewk'])
    else:
        pz.add(label='Z#rightarrow#tau#tau',samples='mc_ztautau',color=ROOT.kMagenta,flags=['bg','mc','ewk'])
    pz.add(label='W#rightarrow#tau#nu',samples='mc_wtaunu',color=ROOT.kYellow,flags=['bg','mc','ewk'])
    if opts.bgqcd==0:
        pz.add(label='bbmu15X/ccmu15X',samples=['mc_bbmu15x','mc_ccmu15x'],color=ROOT.kCyan,flags=['bg','mc','qcd'])
    elif opts.bgqcd==1:
        pz.add(label='QCD J0..J5',samples=['mc_J%d'%v for v in xrange(5)],color=ROOT.kCyan,flags=['bg','mc','qcd'])
    elif opts.bgqcd==2:
        pz.add(label='QCD data-driven',samples=['data_period%s'%s for s in _DATA_PERIODS],color=ROOT.kCyan,flags=['bg','mc','qcd','driven'])
    if opts.bgsig==0:
        pz.add(label='Z#rightarrow#mu#mu',samples='mc_zmumu',color=ROOT.kRed,flags=['sig','mc','ewk'])
    elif opts.bgsig==2:
        pz.add(label='Z#rightarrow#mu#mu+jets',samples=['mc_jimmy_zmumu_np%d'%v for v in range(6)],color=ROOT.kRed,flags=['sig','mc','ewk'])
elif opts.bgsig in (3,): # z+jets
    pz.add(label='t#bar{t}',samples='mc_jimmy_ttbar',color=ROOT.kGreen,flags=['bg','mc','ewk'])
    pz.add(label='W#rightarrow#mu#nu+jets',samples=['mc_jimmy_wmunu_np%d'%v for v in range(6)],color=10,flags=['bg','mc','ewk'])
    #pz.add(label='Z#rightarrow#tau#tau+jets',samples=['mc_jimmy_ztautau_np%d'%v for v in range(6)],color=ROOT.kMagenta)
    pz.add(label='Z#rightarrow#tau#tau',samples='mc_ztautau',color=ROOT.kMagenta,flags=['bg','mc','ewk'])
    #pz.add(label='W#rightarrow#tau#nu+jets',samples=['mc_jimmy_wtaunu_np%d'%v for v in range(6)],color=ROOT.kYellow)
    pz.add(label='W#rightarrow#tau#nu',samples='mc_wtaunu',color=ROOT.kYellow,flags=['bg','mc','ewk'])
    pz.add(label='WZ/ZZ',samples=['mc_jimmy_wz_np%d'%v for v in range(4)]+['mc_jimmy_zz_np%d'%v for v in range(4)],color=11,flags=['bg','mc','ewk'])
    pz.add(label='WW',samples=['mc_jimmy_ww_np%d'%v for v in range(4)],color=12,flags=['bg','mc','ewk'])
    pz.add(label='bbmu15X/ccmu15X',samples=['mc_bbmu15x','mc_ccmu15x'],color=ROOT.kCyan,flags=['bg','mc','qcd'])
    pz.add(label='Z#rightarrow#mu#mu+jets',samples=['mc_jimmy_zmumu_np%d'%v for v in range(6)],color=ROOT.kRed,flags=['sig','mc','ewk'])
# Pre-load the ntuples
path_truth = 'truth/st_truth_reco_%s/ntuple'%opts.ntuple
path_reco  = 'st_%s_final/ntuple'%opts.ntuple
for it,px in enumerate((pw,pz)):
    ptruth = 'truth/st_truth_reco_%s/ntuple'%('w' if it==0 else 'z')
    preco  = 'st_%s_final/ntuple'%('w' if it==0 else 'z')
    px.add(label='2011 data',samples=['data_period%s'%s for s in _DATA_PERIODS],color=ROOT.kBlack,flags=['data'])
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

if mode in ('922','gen_det_kin'): # compares, at truth level, different monte-carlos.
    renormalize()
    c = SuCanvas()
    c.buildDefault(width=800,height=600)
    cc = c.cd_canvas()
    cc.cd(1)
    pre = '(%s) * (%s) * (%s)'%(QMAP[q][2],opts.cut,fortruth(opts.pre))
    hpythia = po.histo('pythia','truth_pythia',opts.var,opts.bin,pre,path=path_truth,norm=True)
    hmcnlo  = po.histo('mcnlo', 'truth_mcnlo', opts.var,opts.bin,pre,path=path_truth,norm=True)
    halpgen = po.histo('alpgen','truth_alpgen',opts.var,opts.bin,pre,path=path_truth,norm=True)
    mstyle = 20
    msize = 1.5
    hpythia.SetLineColor(ROOT.kRed)
    hpythia.SetMarkerColor(ROOT.kRed)
    hpythia.SetMarkerStyle(mstyle)
    hpythia.SetMarkerSize(msize*1.0)
    hpythia.Draw('')
    hpythia.GetYaxis().SetRangeUser(0,max(hpythia.GetMaximum(),hmcnlo.GetMaximum(),halpgen.GetMaximum())*1.5)
    hpythia.GetXaxis().SetTitle(opts.var);
    hmcnlo.SetLineColor(ROOT.kBlue)
    hmcnlo.SetMarkerColor(ROOT.kBlue)
    hmcnlo.SetMarkerStyle(mstyle)
    hmcnlo.SetMarkerSize(msize*0.6)
    hmcnlo.Draw('A same')
    halpgen.SetLineColor(8)
    halpgen.SetMarkerColor(8)
    halpgen.SetMarkerStyle(mstyle)
    halpgen.SetMarkerSize(msize*0.30)
    halpgen.Draw('A same')
    leg = ROOT.TLegend(0.55,0.70,0.88,0.88,QMAP[q][3],"brNDC")
    leg.SetHeader('Different generators:')
    lab = ('Pythia(MRST)','MC@NLO(CTEQ6.6)','Alpgen(CTEQ6.1)')
    leg.AddEntry(hpythia,lab[0],'LP')
    leg.AddEntry(hmcnlo,lab[1],'LP')
    leg.AddEntry(halpgen,lab[2],'LP')
    leg.Draw('same')

if mode=='921': # asymmetry, at truth level, of different monte-carlos.
    renormalize()
    c = SuCanvas()
    c.buildDefault(width=800,height=600)
    cc = c.cd_canvas()
    cc.cd(1)
    names = ('pythia','mcnlo','alpgen')
    labels = ('Pythia(MRST)','MC@NLO(CTEQ6.6)','Alpgen(CTEQ6.1)')
    mstyle = 20
    msize = 1.5
    colors = (ROOT.kRed,ROOT.kBlue,8)
    sizes = (msize*1.0,msize*0.6,msize*0.3)
    h = []
    hasym = []
    leg = ROOT.TLegend(0.55,0.70,0.88,0.88,QMAP[q][3],"brNDC")
    leg.SetHeader('Asymmetry:')
    for i in range(3):
        h.append([None,None])
        for q in (0,1):
            print 'Creating:',i,q
            pre = '(%s) * (%s) * (%s)'%(QMAP[q][2],opts.cut,fortruth(opts.pre))
            h[i][q] = po.histo(names[i],'truth_%s_%d'%(names[i],q),opts.var,opts.bin,pre,path=path_truth)
            h[i][q].SetLineColor(colors[i])
            h[i][q].SetMarkerColor(colors[i])
            h[i][q].SetMarkerStyle(mstyle)
            h[i][q].SetMarkerSize(sizes[i])
        hasym.append(c.WAsymmetry(h[i][POS],h[i][NEG]))
        hasym[i].Draw() if i==0 else hasym[i].Draw('A same')
        leg.AddEntry(hasym[i],labels[i],'LP')
    hasym[0].GetYaxis().SetRangeUser(0,max(hasym[0].GetMaximum(),hasym[1].GetMaximum(),hasym[2].GetMaximum())*1.5)
    leg.Draw('same')

if mode=='923': # asymmetry, at reco/particle level, of different monte-carlos. Compared to BG-subtracted data
    renormalize()
    c = SuCanvas()
    c.buildDefault(width=800,height=600)
    cc = c.cd_canvas()
    cc.cd(1)
    names = ('pythia','mcnlo','alpgen','datasub')
    labels = ('Pythia(MRST)','MC@NLO(CTEQ6.6)','Alpgen(CTEQ6.1)','Data')
    msize = 1.5
    colors = (ROOT.kRed,ROOT.kBlue,8,ROOT.kBlack)
    sizes = (msize*0.7,msize*0.7,msize*0.7,msize*0.5)
    mstyles = (20,20,20,21)
    h = []
    hasym = []
    leg = ROOT.TLegend(0.55,0.70,0.88,0.88,QMAP[q][3],"brNDC")
    leg.SetHeader('Asymmetry:')
    for i in range(len(names)):
        h.append([None,None])
        for q in (0,1):
            print 'Creating:',i,q
            pre = '(%s) * (%s) * (%s)'%(QMAP[q][2],opts.cut,opts.pre)
            if i==len(names)-1: #data
                h[i][q] = particle(po.data_sub('datasub_%s_%d'%(names[i],q),opts.var,opts.bin,pre,path=path_reco),q=q)
            else: #MC
                if opts.effroot: # just get the particle truth histo directly (correctly handles the errors)
                    pre = '(%s) * (%s) * (%s)'%(QMAP[q][2],opts.cut,fortruth(opts.pre))
                    h[i][q] = po.histo(names[i],'recomc_%s_%d'%(names[i],q),opts.var,opts.bin,pre,path=path_truth)
                else:
                    h[i][q] = particle(po.histo(names[i],'recomc_%s_%d'%(names[i],q),opts.var,opts.bin,pre,path=path_reco),q=q)
            h[i][q].SetLineColor(colors[i])
            h[i][q].SetMarkerColor(colors[i])
            h[i][q].SetMarkerStyle(mstyles[i])
            h[i][q].SetMarkerSize(sizes[i])
        hasym.append(c.WAsymmetry(h[i][POS],h[i][NEG]))
        if not i==len(names)-1:
            hasym[i].Draw() if i==0 else hasym[i].Draw('A same')
        else:
            hasym[i].Draw('A same')
        leg.AddEntry(hasym[i],labels[i],'LP')
    hasym[0].GetYaxis().SetRangeUser(0,max(hasym[0].GetMaximum(),hasym[1].GetMaximum(),hasym[2].GetMaximum())*1.5)
    leg.Draw('same')

# WARNING: this function is very specific to the TrigFTKAna ntuple (ie, manually refers to many folders!)
if mode=='924': # asymmetry, at reco/particle level: systematic variations in bg-subtracted data
    renormalize()
    qcdscale = SuSample.qcdscale
    c = SuCanvas()
    c.buildDefault(width=800,height=600)
    cc = c.cd_canvas()
    cc.cd(1)
    # variations
    msize = 1.5
    sizes = [msize*0.7]*20
    colors = [i for i in xrange(20,40)]
    mstyles=[20]*20
    h = []
    hasym = []
    leg = ROOT.TLegend(0.55-0.4,0.70-0.11,0.88-0.4,0.92,QMAP[q][3],"brNDC")
    leg.SetHeader('Systematic variations:')
    hs = [('nominal','WJ','WJ',''),]  # format: (data,mc) histo
    hs += [('mu scale 00','WJ_sc00','WJ',''),('mu scale 10','WJ_sc10','WJ',''),('mu scale 11','WJ_sc11','WJ','')] # data scales
    hs += [('mu smear %d'%i,'WJ','WJ_sm%d'%i,'') for i in xrange(2,6)] # mc smears
    hs += [('QCD +20%','WJ','WJ','qcd+'),('QCD -20%','WJ','WJ','qcd-')]
    hs += [('Isolation','WP','WP','qcdP'),]
    SuSample.GLOBAL_CACHE = None
    SuSample.rebin = 1
    SuSample.hsource = '%s/st_w_final/00_wmt/asym_abseta'
    for i,hh in enumerate(hs):
        SuSample.hsourcedata = SuSample.hsource%hh[1]
        SuSample.hsourcemc = SuSample.hsource%hh[2]
        SuSample.qcdscale = qcdscale
        if hh[3]=='qcd+':
            SuSample.qcdscale = qcdscale*1.2
        elif hh[3]=='qcd-':
            SuSample.qcdscale = qcdscale/1.2
        elif hh[3]=='qcdP':
            SuSample.qcdscale = qcdscale*2.967  # from qcd fit
        h.append([None,None])
        for q in (0,1):
            SuSample.hcharge = q
            pre = '(%s) * (%s) * (%s)'%(QMAP[q][2],opts.cut,opts.pre)
            pre += hh[0]+'_'+SuSample.hsourcedata + '_' + SuSample.hsourcemc
            h[i][q] = particle(po.data_sub('datasub_%s_%d'%(hh[0],q),opts.var,opts.bin,pre,path=path_reco),q=q)
            h[i][q].SetLineColor(colors[i])
            h[i][q].SetMarkerColor(colors[i])
            h[i][q].SetMarkerStyle(mstyles[i])
            h[i][q].SetMarkerSize(sizes[i])
        hasym.append(c.WAsymmetry(h[i][POS],h[i][NEG]))
        hasym[i].Draw() if i==0 else hasym[i].Draw('A same')
        leg.AddEntry(hasym[i],hh[0],'LP')    
    hasym[0].GetXaxis().SetRangeUser(0,2.4)
    # overlay signal templates?
    if True:
        SuSample.hsource = None
        names_mc = ('pythia','mcnlo','alpgen')
        labels_mc = ('Pythia(MRST)','MC@NLO(CTEQ6.6)','Alpgen(CTEQ6.1)')
        colors_mc = (ROOT.kRed,ROOT.kBlue,8,ROOT.kBlack)
        sizes_mc = (msize*0.7,msize*0.7,msize*0.7,msize*0.5)
        mstyle_mc = 22
        hasym_mc = []
        h_mc = []
        for i in range(3):
            h_mc.append([None,None])
            for q in (0,1):
                pre = '(%s) * (%s) * (%s)'%(QMAP[q][2],opts.cut,opts.pre)
                if opts.effroot and False:
                    pre = '(%s) * (%s) * (%s)'%(QMAP[q][2],opts.cut,fortruth(opts.pre))
                    h_mc[i][q] = po.histo(names_mc[i],'truth_%s_%d'%(names_mc[i],q),opts.var,opts.bin,pre,path=path_truth)
                else:
                    h_mc[i][q] = po.histo(names_mc[i],'truth_%s_%d'%(names_mc[i],q),opts.var,opts.bin,pre,path=path_reco)
                h_mc[i][q].SetLineColor(colors_mc[i])
                h_mc[i][q].SetMarkerColor(colors_mc[i])
                h_mc[i][q].SetMarkerStyle(mstyle_mc)
                h_mc[i][q].SetMarkerSize(sizes_mc[i])
            hasym_mc.append(c.WAsymmetry(h_mc[i][POS],h_mc[i][NEG]))
            hasym_mc[i].Draw('A same')
            leg.AddEntry(hasym_mc[i],labels_mc[i],'LP')
    # plot everything and scale y axis
    def hmaximum(h):
        res=[]
        for i in xrange(1,h.GetNbinsX()+1):
            if h.GetBinCenter(i)<2.4:
                res.append(h.GetBinContent(i))
        return max(res)
    hasym[0].GetYaxis().SetRangeUser(0,max([hmaximum(hh) for hh in hasym])*1.5)
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
    pre_fake_winc = prunesub(opts.pre,('l_pt','met','w_mt','fabs\(d0sig\)'),'l_pt>15 && l_pt<20 && met<25 && w_mt<40')
    pre_fake_aft = '(%s) * (%s) * (%s)'%(QMAP[q][2],opts.cut,pre_fake_winc)
    pre_fake_bef = '(%s) * (%s) * (%s)'%(QMAP[q][2],opts.cut,prune(pre_fake_winc,'ptiso'))
    hfake_bef = pw.data('fdata_bef',opts.var,opts.bin,pre_fake_bef)
    hfake_aft = pw.data('fdata_aft',opts.var,opts.bin,pre_fake_aft)
    heff_fake = c.Efficiency(hfake_aft,hfake_bef)
    heff_fake.Draw('')
    heff_fake.GetYaxis().SetRangeUser(0.3,0.5)
    heff_fake.GetYaxis().SetTitle('Efficiency for QCD')
    heff_fake.GetXaxis().SetTitle(opts.var)
    # Get h_isol, h_loose directly from data
    print 'Obtaining overall loose and isolated templates from data'
    cc.cd(3)
    pre_isol = '(%s) * (%s) * (%s)'%(QMAP[q][2],opts.cut,opts.pre)
    pre_loose = '(%s) * (%s) * (%s)'%(QMAP[q][2],opts.cut,looseisoreg(opts.pre))
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

if mode=='1': # total stack histo
    renormalize()
    c = SuCanvas()
    leg = ROOT.TLegend(0.55,0.70,0.88,0.88,QMAP[q][3],"brNDC")
    hmc,hdata = None,None
    if opts.ntuple=='w':
        hmc = po.stack('mc',opts.var,opts.bin,'(%s) * (%s) * (%s)'%(QMAP[q][2],opts.cut,opts.pre),leg=leg)
        hdata = po.data('data',opts.var,opts.bin,'(%s) * (%s) * (%s)'%(QMAP[q][2],opts.cut,opts.pre),leg=leg)
    else:
        hmc = po.stack('mc',opts.var,opts.bin,'(%s) * (%s)'%(opts.cut,opts.pre),leg=leg)
        hdata = po.data('data',opts.var,opts.bin,'(%s) * (%s)'%(opts.cut,opts.pre),leg=leg)
    c.plotStackHisto(hmc,hdata,leg)

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

if mode=='11': # asymmetry (not bg-subtracted)
    assert opts.ntuple=='w','ERROR: asymmetry can only be computed for the w ntuple'
    hsig,hd    = [None]*2,[None]*2
    SuSample.hcharge = POS
    hsig[POS]  = particle(po.sig('signalPOS',opts.var,opts.bin,'(%s) * (%s) * (%s)'%(QMAP[POS][2],opts.cut,opts.pre)),q=POS)
    hd[POS]    = particle(po.data('dataPOS',opts.var,opts.bin,'(%s) * (%s) * (%s)'%(QMAP[POS][2],opts.cut,opts.pre)),q=POS)
    SuSample.hcharge = NEG
    hsig[NEG]  = particle(po.sig('signalNEG',opts.var,opts.bin,'(%s) * (%s) * (%s)'%(QMAP[NEG][2],opts.cut,opts.pre)),q=NEG)
    hd[NEG]    = particle(po.data('dataNEG',opts.var,opts.bin,'(%s) * (%s) * (%s)'%(QMAP[NEG][2],opts.cut,opts.pre)),q=NEG)
    c = SuCanvas()
    c.plotAsymmetry(hd[POS],hd[NEG],hsig[POS],hsig[NEG])

if mode=='12': # asymmetry (bg-subtracted)
    assert opts.ntuple=='w','ERROR: asymmetry can only be computed for the w ntuple'
    renormalize()
    hsig,hd_sig  = [None]*2,[None]*2
    SuSample.hcharge = POS
    hsig[POS]    = particle(po.sig('signalPOS',opts.var,opts.bin,'(%s) * (%s) * (%s)'%(QMAP[POS][2],opts.cut,opts.pre)),q=POS)
    hd_sig[POS]  = particle(po.data_sub('dataPOS',opts.var,opts.bin,'(%s) * (%s) * (%s)'%(QMAP[POS][2],opts.cut,opts.pre)),q=POS)
    SuSample.hcharge = NEG
    hsig[NEG]    = particle(po.sig('signalNEG',opts.var,opts.bin,'(%s) * (%s) * (%s)'%(QMAP[NEG][2],opts.cut,opts.pre)),q=NEG)
    hd_sig[NEG]  = particle(po.data_sub('dataNEG',opts.var,opts.bin,'(%s) * (%s) * (%s)'%(QMAP[NEG][2],opts.cut,opts.pre)),q=NEG)
    c = SuCanvas()
    c.plotAsymmetry(hd_sig[POS],hd_sig[NEG],hsig[POS],hsig[NEG])

if mode in ('101','102'): # tag and probe
    assert opts.ntuple=='z','ERROR: tag-and-probe can only be computed for the z ntuple'
    #renormalize()
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
        if mode=='101':   # default data
            hda_bef[iq] = po.data('da_bef_%s'%QMAPZ[iq][1],var,opts.bin,'(%s) * (%s)'%(opts.cut,pre_bef))
            hda_aft[iq] = po.data('da_aft_%s'%QMAPZ[iq][1],var,opts.bin,'(%s) * (%s)'%(opts.cut,pre_aft))
        elif mode=='102': # bg-subtracted data
            hda_bef[iq] = po.data_sub('da_bef_%s'%QMAPZ[iq][1],var,opts.bin,'(%s) * (%s)'%(opts.cut,pre_bef))
            hda_aft[iq] = po.data_sub('da_aft_%s'%QMAPZ[iq][1],var,opts.bin,'(%s) * (%s)'%(opts.cut,pre_aft))
        else:
            assert False,'Unknown tag-and-probe mode'
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
    pre_in = prune(pre_in,lvar)
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
        ccscan.cd(i+1)
        h = []
        f = []
        for iqp in (POS,NEG):
            iqt = 0 if iqp==1 else 1
            var = lvar.replace('lX',QMAPZ[iqt][2]).replace('lY',QMAPZ[iqp][2])
            prelist = [pre_in,'(%s)>%.2f'%(lvar,lL),'(%s)<=%.2f'%(lvar,lR)]
            if tagreq:
                varX = lvar.replace('Y','X')
                prelist += ['(%s)>%.2f'%(lvar,lL),'(%s)<=%.2f'%(lvar,lR)]
            pre = ' && '.join(prelist).replace('lX',QMAPZ[iqt][2]).replace('lY',QMAPZ[iqp][2])
            h.append( po.data('data%d_q%d'%(i,iqp),pvar,pvarbins,pre) )
            ipvarbins = ifitbins = (pbins,pmin,pmax)
            if w.func=='gaus':
                dGeV = 6.0  # 4.0
                peak = h[-1].GetMean()
                ifitbins = (int(2*dGeV/pbinw),peak - dGeV, peak + dGeV)
                ifitbins = (-1,80.00,100.00)
                print 'Reducing gaus0 fit window: [%.2f,%.2f] --> [%.2f,%.2f]'%(ipvarbins[1],ipvarbins[2],ifitbins[1],ifitbins[2])
            r,frame,chi2ndf,ndf,xtra,res1st = Fit.Fit(w,h[-1],ipvarbins,ifitbins)
            frame.Draw()
            if iqp==POS:
                hPOS.SetBinContent(i,w.var('m').getVal())
                hPOS.SetBinError(i,w.var('m').getError())
            else:
                hNEG.SetBinContent(i,w.var('m').getVal())
                hNEG.SetBinError(i,w.var('m').getError())
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
    pre = 'lX_idhits==1 && fabs(lX_z0)<10. && fabs(lX_eta)<%f && lX_pt>20.0 && (lX_q*lY_q)<0 && fabs(lX_z0-lY_z0)<3 && fabs(lX_d0-lY_d0)<2 && lX_ptiso20/lX_pt<0.1 && Z_m>70 && Z_m<110'%(1.0 if tagcentral else 2.4)
    pre += '&& lY_idhits==1 && fabs(lY_z0)<10. && lY_pt>20.0 && lX_ptiso20/lX_pt<0.1'
    cscan,hPOS,hNEG = loop_zbins(w,pre,pvar=pvar,pvarbins=pvarbins,lvar=lvar,lvarbins=lvarbins)
    cc.cd()
    hPOS.Draw()
    hNEG.Draw('SAME')
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

if not opts.antondb:
    c.SaveAs('%s_%s_%s_%s_%s_%s'%(opts.tag,opts.input,QMAP[opts.charge][1],opts.var,opts.cut,mode),'png')
    for i,obj in enumerate(OMAP):
        if hasattr(obj,'InheritsFrom') and obj.InheritsFrom('TPad'):
            obj.SaveAs('%s_%s_%s_%s_%s_%s__%d.png'%(opts.tag,opts.input,QMAP[opts.charge][1],opts.var,opts.cut,mode,i))
        elif hasattr(obj,'SaveAs'): # SuCanvas
            obj.SaveAs('%s_%s_%s_%s_%s_%s__%d'%(opts.tag,opts.input,QMAP[opts.charge][1],opts.var,opts.cut,mode,i),'png')

# save everything
if len(COUT)>0:
    VMAP['COUT']=[]
    for l in COUT:
        print l
        VMAP['COUT'].append(l)
c._canvas.SetName('%s_m%s_%s'%(opts.tag,mode,opts.var))
OMAP += [c._canvas,]
if (len(VMAP)>0 or len(OMAP)>0) and opts.antondb:
    a = antondb.antondb(opts.antondb)
    path = os.path.join('/stack/',opts.tag,QMAP[opts.charge][1])
    if len(VMAP)>0:
        a.add(path,VMAP)
    if len(OMAP)>0:
        a.add_root(path,OMAP)
