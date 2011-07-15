#!/usr/bin/env python

_PRE_PETER  = 'l_pt>25.0 && ptiso40<2.0 && etiso40<2.0 && met>25.0 && w_mt>40.0'
_PRE_JORDAN = 'l_pt>25.0 && ptiso20/l_pt<0.1 && met>25.0 && w_mt>40.0'
_PRE_JORDANALT = 'l_pt>25.0 && ptiso20/l_pt<0.1 && ptiso30/l_pt<0.15 && met>25.0 && w_mt>40.0'

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
parser.add_option("--input",dest="input",
                  type="string", default='ROOT/root_all_0706_newiso_1fb_cmb', #'ROOT/root_all_0630_newiso_1fb_cmb',
                  help="Path to input root file with all histos")
parser.add_option("--var",dest="var",
                  type="string", default='l_eta',
                  help="Variable to plot")
parser.add_option("--bin",dest="bin",
                  type="string", default='50,-2.5,2.5',
                  help="Binning for var")
parser.add_option("--pre",dest="pre",
                  type="string", default=_PRE_JORDAN,
                  help="Preliminary cuts to select final W candidates")
parser.add_option("--cut",dest="cut",
                  type="string", default='mcw*puw', # *effw*trigw
                  help="Additional cut to select events")
parser.add_option("--hdata",dest="hdata",
                  type="string", default=None,
                  help="Path to data histogram")
parser.add_option("--hmc",dest="hmc",
                  type="string", default=None,
                  help="Path to mc histogram")
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
                  type="float", default=832854.0,
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

from MC import *
from SuCanvas import *
#ROOT.TH1.AddDirectory(ROOT.kFALSE)    # ensure that we own all the histograms
ROOT.gROOT.SetBatch(opts.batch)
ROOT.TH1.SetDefaultSumw2()
SetStyle("AtlasStyle.C")
#ROOT.gStyle.SetOptFit(1111);

#SetStyle()
# Determine plot ordering for MC stacks
def save(c,name):
    for ext in ('png','ps','pdf','C'):
        c.SaveAs("%s.%s"%(name,ext))
def QAPP(path,iq):
    htmp = path.split('/');
    htmp.insert(-1,QMAP[iq][1])
    return '/'.join(htmp) if iq in (0,1) else path

POS,NEG,ALL=range(3)
QMAP = {}
QMAP[POS] = (0,'POS','(l_q>0)','mu+ only')
QMAP[NEG] = (1,'NEG','(l_q<0)','mu- only')
QMAP[ALL] = (2,'ALL','(l_q!=0)','mu+ and mu-')

# MC stack order
po = PlotOrder()
if opts.bgsig!=3:
    po.Add(name='t#bar{t}',samples='mc_jimmy_ttbar',color=ROOT.kGreen)
    po.Add(name='Z#rightarrow#tau#tau',samples='mc_ztautau',color=ROOT.kMagenta)
    po.Add(name='W#rightarrow#tau#nu',samples='mc_wtaunu',color=ROOT.kYellow)
    po.Add(name='Z#rightarrow#mu#mu',samples='mc_zmumu',color=ROOT.kRed)
    if opts.bgqcd==0:
        po.Add(name='bbmu15X/ccmu15X',samples=['mc_bbmu15x','mc_ccmu15x'],color=ROOT.kCyan)
    elif opts.bgqcd==1:
        po.Add(name='QCD J0..J5',samples=['mc_J%d'%v for v in range(5)],color=ROOT.kCyan)
    if opts.bgsig==0:
        po.Add(name='W#rightarrow#mu#nu',samples='mc_wmunu',color=10)
    elif opts.bgsig==1:
        po.Add(name='W#rightarrow#mu#nu',samples=['mc_wminmunu','mc_wplusmunu'],color=10)
else:
    po.Add(name='t#bar{t}',samples='mc_jimmy_ttbar',color=ROOT.kGreen)
    po.Add(name='Z#rightarrow#tau#tau+jets',samples=['mc_jimmy_ztautau_np%d'%v for v in range(6)],color=ROOT.kMagenta)
    po.Add(name='W#rightarrow#tau#nu+jets',samples=['mc_jimmy_wtaunu_np%d'%v for v in range(6)],color=ROOT.kYellow)
    po.Add(name='Z#rightarrow#mu#mu+jets',samples=['mc_jimmy_zmumu_np%d'%v for v in range(6)],color=ROOT.kRed)
    po.Add(name='WZ/ZZ',samples=['mc_jimmy_wz_np%d'%v for v in range(4)]+['mc_jimmy_zz_np%d'%v for v in range(4)],color=11)
    po.Add(name='WW',samples=['mc_jimmy_ww_np%d'%v for v in range(4)],color=12)
    po.Add(name='bbmu15X/ccmu15X',samples=['mc_bbmu15x','mc_ccmu15x'],color=ROOT.kCyan)
    po.Add(name='W#rightarrow#mu#nu+jets',samples=['mc_jimmy_wmunu_np%d'%v for v in range(6)],color=10)

# Determine which data periods to plot
data = [ 'data_period%s'%s for s in ('B','D','E','F','G1','G2','G3','G4','G5','G6','H1') ]
data = [ 'data_period%s_updscaleqq0706'%s for s in ('B','D','E','F','G1','G2','G3','G4','G5','G6','H1') ]
mc = list(xflatten(po.mcg))
# All histo files that we need to get
allnames = data + mc

# data and mc histograms
hdata = [[] for i in xrange(3)]
hmc = [{} for i in xrange(3)]
# truth level (for wmunu only)
htr = [None for i in xrange(3)]

# load the data inputs
gbg = []
nentries_data = []
nentries_bg = []
nentries_sig = []
for iname in allnames:
    print 'Loading',iname,
    f = ROOT.TFile.Open('%s/%s/root_%s.root'%(opts.input,iname,iname))
    gbg.append(f)
    topdir = 'dg'
    if not f.Get(topdir):
        ldirs = [z.GetName() for z in f.GetListOfKeys()]
        assert len(ldirs)==1, 'Expecting one folder here: '+' '.join(ldirs)
        topdir=ldirs[0]+'/dg'
    nevts = f.Get('%s/nevts'%topdir).GetEntries()
    print 'with',int(nevts),'events'
    nt = f.Get('%s/st_w_final/ntuple'%topdir)
    if not nt:
        print 'No events passed cuts in background',iname
        po.Remove(iname)
        continue
    if re.match('mc',iname):
        try:
            hname,hpath='',''
            for iq in range(2):
                hname = 'h%s%s'%(iname,QMAP[iq][1])
                if opts.hmc:
                    hpath = QAPP(opts.hmc,iq)
                    # tmp FIXME: wmin mc may not have any events
                    if True and (re.search('wminmunu',iname) or re.search('wplusmunu',iname)) and not f.Get('%s/%s'%(topdir,hpath)):
                        print 'Warning: no events in histogram %s (path %s) in file %s'%(hname,hpath,iname)
                        hmc[iq][iname] = ROOT.TH1F(hname,hname,100,0,3)
                        hmc[iq][iname].Rebin(opts.rebin)
                        continue
                    hmc[iq][iname]=ScaleToLumi(f.Get('%s/%s'%(topdir,hpath)).Clone(hname),iname,opts.lumi,opts.qcdscale,nevts)
                    hmc[iq][iname].Rebin(opts.rebin)
                else:
                    nt.Draw('%s>>%s(%s)'%(opts.var,hname,opts.bin),'(%s) * (%s) * (%s)'%(QMAP[iq][2],opts.cut,opts.pre),'goff')
                    hmc[iq][iname]=ScaleToLumi(ROOT.gDirectory.Get(hname).Clone(),iname,opts.lumi,opts.qcdscale,nevts)
            if is_wmunu(iname):
                nentries_sig.append(hmc[POS][iname].GetEntries())
            else:
                nentries_bg.append(hmc[POS][iname].GetEntries())
            hmc[ALL][iname] = WSum(hmc[POS][iname],hmc[NEG][iname],'h%sALL'%iname)
        except:
            print 'Error: could not find histogram %s (path %s) in file %s'%(hname,hpath,iname)
            po.Remove(iname)
            continue
        if is_wmunu(iname):
            # for signal MC, also get truth-level template for efficiency
            nt2 = f.Get('%s/truth/st_truth_reco_w/ntuple'%topdir)
            if nt2:
                for iq in range(2):
                    hname = 'htr%s%s'%(iname,QMAP[iq][1])
                    # prune out isolation from the list of truth-level cuts
                    pres = opts.pre.split('&&')
                    precut=' && '.join([c for c in pres if not re.search('iso',c)])
                    nt2.Draw('%s>>%s(%s)'%(opts.var,hname,opts.bin),'(%s) * (%s) * (%s)'%(QMAP[iq][2],opts.cut,precut),'goff')
                    if not htr[ALL]:
                        htr[iq]=ScaleToLumi(ROOT.gDirectory.Get(hname).Clone(),iname,opts.lumi,opts.qcdscale,nevts)
                    else:
                        htr[iq].Add( ScaleToLumi(ROOT.gDirectory.Get(hname).Clone(),iname,opts.lumi,opts.qcdscale,nevts) )
                if not htr[ALL]:
                    htr[ALL] = WSum(htr[POS],htr[NEG],'htr%sALL'%iname)
                else:
                    htr[ALL].Add( WSum(htr[POS],htr[NEG],'htr%sALL'%iname) )
    else: #data:
        for iq in range(2):
            hname = 'hd%s%s'%(iname,QMAP[iq][1])
            if opts.hdata:
                hpath = QAPP(opts.hdata,iq)
                print '%s/%s'%(topdir,hpath)
                hdata[iq].append(  f.Get('%s/%s'%(topdir,hpath)).Clone(hname)  )
                hdata[iq][-1].Rebin(opts.rebin)
            else:
                nt.Draw('%s>>%s(%s)'%(opts.var,hname,opts.bin),'(%s) * (%s) * (%s)'%(QMAP[iq][2],opts.cut,opts.pre),'goff')
                hdata[iq].append(ROOT.gDirectory.Get(hname).Clone())
                hdata[iq][-1].Sumw2()
        nentries_data.append(hdata[POS][-1].GetEntries())
        hdata[ALL].append( WSum(hdata[POS][-1],hdata[NEG][-1],'hd%sALL'%iname) )

print 'DATA      ENTRIES:',sum(nentries_data)
print 'SIGNAL MC ENTRIES:',sum(nentries_sig)

# prepare output holders for summed histos
leg = [MakeLegend(QMAP[i][3]) for i in xrange(3)]
leg_eff = [MakeLegend(QMAP[i][3]) for i in xrange(3)]
hd = [None for i in xrange(3)]   # data
hd_sig = [None for i in xrange(3)]   # data bg-subtracted
hd_eff = [None for i in xrange(3)]   # data/truth
hd_sig_eff = [None for i in xrange(3)]   # data bg-subtracted / truth
hbgs = [ROOT.THStack('eta','eta') for i in xrange(3)]  # all-mc stacks
hbgs_eff = [ROOT.THStack('eta','eta') for i in xrange(3)]  # all-mc stacks / truth
hbg = [None for i in xrange(3)]  # sum of bg, without wmunu sig
hbg_noqcd = [None for i in xrange(3)]  # sum of bg, without QCD
hbg_qcd = [None for i in xrange(3)]  # sum of QCD bg
hsig = [None for i in xrange(3)] # wmunu sig only
heff = [None for i in xrange(3)] # hsig / truth

# create mc histogram stacks and mc & data combined histos
for q in xrange(3):
    for i,inames in enumerate(po.mcg):
        hh = [hmc[q][f].Clone() for f in inames]
        [hh[0].Add(fc) for fc in hh[1:]]  # add J1..J5
        hh[0].SetLineColor(ROOT.kBlack)
        hh[0].SetFillColor(po.mcgc[i])
        hh[0].SetMarkerSize(0)
        # make a sum of all bg
        if not is_wmunu(inames[0]):
            if hbg[q]:
                hbg[q].Add(hh[0])
            else:
                hbg[q] = hh[0].Clone()
        else:
            if hsig[q]:
                hsig[q].Add(hh[0])
            else:
                hsig[q]=hh[0].Clone()
        # make BG with and without QCD (for TFractionFitter)
        if not is_qcd(inames[0]):
            if hbg_noqcd[q]:
                hbg_noqcd[q].Add(hh[0])
            else:
                hbg_noqcd[q] = hh[0].Clone()
        else:
            hbg_qcd[q]=hh[0].Clone()
        # make a stack of all bg + sig
        hbgs[q].Add(hh[0])  
        leg[q].AddEntry(hh[0],po.mcg_name[i],'F')
    # create data histograms
    hd[q]=hdata[q][0].Clone()
    [hd[q].Add(fc) for fc in hdata[q][1:]]
    leg[q].AddEntry(hd[q],'Data(#int L dt = %.1f pb^{-1})'%(opts.lumi/1000.0),'LP')
    # create efficiency histograms:  sig/truth;  to use: hdata/eff
    if htr[q]:
        heff[q] = hsig[q].Clone()
        heff[q].Divide(htr[q])
        # make a eff-corrected version of stack plot
        for i,inames in enumerate(po.mcg):
            hh = [hmc[q][f].Clone() for f in inames]
            [ih.Divide(heff[q]) for ih in hh] # correct for efficiency
            [hh[0].Add(fc) for fc in hh[1:]]  # add J1..J5
            hh[0].SetLineColor(ROOT.kBlack)
            hh[0].SetFillColor(po.mcgc[i])
            hh[0].SetMarkerSize(0)
            # make a stack of all bg + sig
            hbgs_eff[q].Add(hh[0])
            leg_eff[q].AddEntry(hh[0],po.mcg_name[i],'F')
    # make a bg-subtracted and eff-corrected version of data plot
    hd_sig[q] = hd[q].Clone()
    hd_sig[q].Add(hbg[q],-1)
    if heff[q]:
        hd_sig_eff[q] = hd_sig[q].Clone()
        hd_sig_eff[q].Divide(heff[q])
        hd_eff[q] = hd[q].Clone()
        hd_eff[q].Divide(heff[q])

q = opts.charge

if mode==1: # total stack histo
    c = SuCanvas()
    c.plotStackHisto(hbgs[q],hd[q],leg[q])

if mode==2: # total stack histo (eff-corrected from MC)
    c = SuCanvas()
    c.plotStackHisto(hbgs_eff[q],hd_eff[q],leg[q])

if mode==11: # asymmetry (not bg-subtracted)
    c = SuCanvas()
    c.plotAsymmetry(hd[POS],hd[NEG],hsig[POS],hsig[NEG])
    if opts.antondb:
        a = antondb.antondb(opts.antondb)
        a.add_root(opts.tag,[c.hmcasym,c.hdasym,c._canvas])

if mode==12: # asymmetry (bg-subtracted)
    c = SuCanvas()
    c.plotAsymmetry(hd_sig[POS],hd_sig[NEG],hsig[POS],hsig[NEG])

if mode==13: # asymmetry (bg-subtracted, eff-corrected)
    c = SuCanvas()
    c.plotAsymmetry(hd_sig_eff[POS],hd_sig_eff[NEG],htr[POS],htr[NEG])

if mode==99: # TFractionFitter for QCD contribution
    c = SuCanvas()
    c.buildDefault()
    # mc
    mc = ROOT.TObjArray(2)
    mc.Add(hbg_qcd[ALL])
    mc.Add(hbg_noqcd[ALL])
    fit = ROOT.TFractionFitter(hd[ALL],mc)
    fit.Constrain(1,0.0,1.0);
    status = fit.Fit();
    if status == 0:
        result = fit.GetPlot();
        hd[ALL].Draw("Ep");
        result.Draw("same");
        print 'Fit chi2 =',fit.GetChisquare()
        print 'Fit prob =',fit.GetProb()
    else:
        print 'Fit failed!'

c.SaveAs('%s_%s_%s_%s_%s_%d'%(opts.tag,opts.input,QMAP[opts.charge][1],opts.var,opts.cut,mode),'png')
