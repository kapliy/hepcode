#!/usr/bin/env python

"""
tag-and-probe Z bosons
"""

# X - tag muon (doesn't change), Y - probe muon used to measure efficiency ( = Y after/before specific cut)
_BEF = '(lP_idhits==1 && fabs(lP_z0)<10. && lP_pt>20.0 && lN_idhits==1 && fabs(lN_z0)<10. && lN_pt>20.0) && Z_m>81.0 && Z_m<101.0 && (lP_q*lN_q)<0 && fabs(lP_z0-lN_z0)<3 && fabs(lP_d0-lN_d0)<2 && fabs(lP_phi-lN_phi)>2.0 && lX_ptiso20/lX_pt<0.1'
_AFT = _BEF + ' && ' + 'lY_ptiso20/lY_pt<0.1'
#_AFT = _BEF + ' && ' + 'lY_ptiso40<2.0 && lY_etiso40<2.0'

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
parser.add_option("--input",dest="input",
                  type="string", default='ROOT/current/all',
                  help="Path to input root file with all histos")
parser.add_option("--var",dest="var",
                  type="string", default='lY_eta',
                  help="Variable to plot")
parser.add_option("--bin",dest="bin",
                  type="string", default='50,-2.5,2.5',
                  help="Binning for var")
parser.add_option("--prebef",dest="prebef",
                  type="string", default=_BEF,
                  help="Cuts BEFORE applying studied cut")
parser.add_option("--preaft",dest="preaft",
                  type="string", default=_AFT,
                  help="Cuts AFTER applying studied cut")
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
#1035040.0 up to H4 (184169) EF_mu18_MG
parser.add_option("--lumi",dest="lumi",
                  type="float", default=1035040.0,
                  help="Integrated luminosity for data (in nb^-1)")
parser.add_option("--qcd",dest="qcdscale",
                  type="string", default='1.0',
                  help="QCD scale factor")
parser.add_option('-t',"--tag",dest="tag",
                  type="string", default='plot',
                  help="A tag to append to all output plots")
parser.add_option('-b', "--batch", default=False,
                  action="store_true",dest="batch",
                  help="Enable batch mode (all output directly to filesystem)")
parser.add_option("-q", "--charge",dest="charge",
                  type="int", default=0,
                  help="Which charge is a tag: 0=POS, 1=NEG")
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

POS,NEG=range(2)
QMAP = {}
QMAP[POS] = (0,'POS','lP','mu+')
QMAP[NEG] = (1,'NEG','lN','mu-')

# Determine which data periods to plot
data = [ 'data_period%s'%s for s in ('B','D','E','F','G','H') ]
mc = ['mc_zmumu']
# All histo files that we need to get
allnames = data + mc

# data and mc histograms: 0 if X=mu+, 1 if X=mu-
ada_bef = [{} for i in xrange(2)]
ada_aft = [{} for i in xrange(2)]
amc_bef = [{} for i in xrange(2)]
amc_aft = [{} for i in xrange(2)]
# same as above, after summing
hda_bef = [None for i in xrange(2)]
hda_aft = [None for i in xrange(2)]
hmc_bef = [None for i in xrange(2)]
hmc_aft = [None for i in xrange(2)]

# load the data inputs
gbg = []
nentries_da_bef = [[] for i in xrange(2)]
nentries_da_aft = [[] for i in xrange(2)]
nentries_mc_bef = [[] for i in xrange(2)]
nentries_mc_aft = [[] for i in xrange(2)]

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
    nt = f.Get('%s/st_z_final/ntuple'%topdir)
    if not nt:
        assert False,'No events passed in %s'%iname
    hname_bef,hname_aft='',''
    try:
        for iq in range(2): # probe charge (what we're measuring)
            iqp = iq
            iqt = 0 if iq==1 else 1
            var = opts.var.replace('lY',QMAP[iqp][2])
            hname_bef = 'h%s%s_bef'%(iname,QMAP[iq][1])
            hname_aft = 'h%s%s_aft'%(iname,QMAP[iq][1])
            pre_bef = opts.prebef.replace('lX',QMAP[iqt][2]).replace('lY',QMAP[iqp][2])
            pre_aft = opts.preaft.replace('lX',QMAP[iqt][2]).replace('lY',QMAP[iqp][2])
            nt.Draw('%s>>%s(%s)'%(var,hname_bef,opts.bin),'(%s) * (%s)'%(opts.cut,pre_bef),'goff')
            nt.Draw('%s>>%s(%s)'%(var,hname_aft,opts.bin),'(%s) * (%s)'%(opts.cut,pre_aft),'goff')
            if re.match('mc',iname): #mc
                if False:
                    amc_bef[iq][iname]=ScaleToLumi(ROOT.gDirectory.Get(hname_bef).Clone(),iname,opts.lumi,0,nevts)
                    amc_aft[iq][iname]=ScaleToLumi(ROOT.gDirectory.Get(hname_aft).Clone(),iname,opts.lumi,0,nevts)
                else:
                    amc_bef[iq][iname]=ROOT.gDirectory.Get(hname_bef).Clone()
                    amc_aft[iq][iname]=ROOT.gDirectory.Get(hname_aft).Clone()
                if not hmc_bef[iq]:
                    hmc_bef[iq] = amc_bef[iq][iname].Clone()
                    hmc_bef[iq].SetLineColor(ROOT.kBlue)
                    hmc_bef[iq].SetMarkerColor(ROOT.kBlue)
                    hmc_bef[iq].SetMarkerSize(0.4);
                else:
                    hmc_bef[iq].Add(amc_bef[iq][iname])
                if not hmc_aft[iq]:
                    hmc_aft[iq] = amc_aft[iq][iname].Clone()
                    hmc_aft[iq].SetLineColor(ROOT.kBlue)
                    hmc_aft[iq].SetMarkerColor(ROOT.kBlue)
                    hmc_aft[iq].SetMarkerSize(0.4);
                else:
                    hmc_aft[iq].Add(amc_aft[iq][iname])
                nentries_mc_bef[iq].append(amc_bef[iq][iname].GetEntries())
                nentries_mc_aft[iq].append(amc_aft[iq][iname].GetEntries())
            else:                    #data
                ada_bef[iq][iname]=ROOT.gDirectory.Get(hname_bef).Clone()
                ada_aft[iq][iname]=ROOT.gDirectory.Get(hname_aft).Clone()
                if not hda_bef[iq]:
                    hda_bef[iq] = ada_bef[iq][iname].Clone()
                    hda_bef[iq].SetLineColor(ROOT.kRed)
                    hda_bef[iq].SetMarkerColor(ROOT.kRed)
                    hda_bef[iq].SetMarkerSize(0.6);
                else:
                    hda_bef[iq].Add(ada_bef[iq][iname])
                if not hda_aft[iq]:
                    hda_aft[iq] = ada_aft[iq][iname].Clone()
                    hda_aft[iq].SetLineColor(ROOT.kRed)
                    hda_aft[iq].SetMarkerColor(ROOT.kRed)
                    hda_aft[iq].SetMarkerSize(0.6);
                else:
                    hda_aft[iq].Add(ada_aft[iq][iname])
                nentries_da_bef[iq].append(ada_bef[iq][iname].GetEntries())
                nentries_da_aft[iq].append(ada_aft[iq][iname].GetEntries())
    except:
        assert False, 'Error: could not create histogram (%s and %s) in file %s'%(hname_bef,hname_aft,iname)

print 'DATA BEF (mu+):',sum(nentries_da_bef[0])
print 'DATA AFT (mu+):',sum(nentries_da_aft[0])
print 'MC   BEF (mu+):',sum(nentries_mc_bef[0])
print 'MC   AFT (mu+):',sum(nentries_mc_aft[0])
print 'DATA BEF (mu-):',sum(nentries_da_bef[1])
print 'DATA AFT (mu-):',sum(nentries_da_aft[1])
print 'MC   BEF (mu-):',sum(nentries_mc_bef[1])
print 'MC   AFT (mu-):',sum(nentries_mc_aft[1])

# create final efficiency and ratio histograms
hda_r = [None for i in xrange(2)]
hmc_r = [None for i in xrange(2)]
hsc_r = [None for i in xrange(2)]
for iq in xrange(2):
    hda_r[iq] = hda_aft[iq].Clone()
    hda_r[iq].Divide(hda_aft[iq],hda_bef[iq],1.0,1.0,"B")
    hmc_r[iq] = hmc_aft[iq].Clone()
    hmc_r[iq].Divide(hmc_aft[iq],hmc_bef[iq],1.0,1.0,"B")
    hsc_r[iq] = hda_r[iq].Clone()
    hsc_r[iq].Divide(hda_r[iq],hmc_r[iq],1.0,1.0,"")
    #hda_r[iq].Clone()
    #hsc_r[iq].Divide(hmc_r[iq])
    hsc_r[iq].SetLineColor(ROOT.kBlack)
    hsc_r[iq].SetMarkerColor(ROOT.kBlack)
    hsc_r[iq].SetMarkerSize(0.4);

if mode==1:
    c = ROOT.TCanvas('c','c',1200,600)
    c.Divide(2,2)
    c.cd(1)
    hda_r[0].Draw("")
    hmc_r[0].Draw("A same")
    hda_r[0].GetYaxis().SetRangeUser(0.50,1.10);
    hda_r[0].GetYaxis().SetTitle('mu+ efficiency');
    hda_r[0].GetXaxis().SetTitle(opts.var);
    c.cd(2)
    hda_r[1].Draw("")
    hmc_r[1].Draw("A same")
    hda_r[1].GetYaxis().SetRangeUser(0.50,1.10);
    hda_r[1].GetYaxis().SetTitle('mu- efficiency');
    hda_r[1].GetXaxis().SetTitle(opts.var);
    c.cd(3)
    hsc_r[0].Draw("")
    hsc_r[0].GetYaxis().SetRangeUser(0.90,1.10);
    hsc_r[0].GetYaxis().SetTitle('mu+ scale factor');
    hsc_r[0].GetXaxis().SetTitle(opts.var);
    l0 = ROOT.TGraph(2)
    l0.SetPoint(0,hsc_r[0].GetXaxis().GetXmin(),1.0)
    l0.SetPoint(1,hsc_r[0].GetXaxis().GetXmax(),1.0)
    l0.SetLineWidth(1)
    l0.SetLineColor(ROOT.kRed)
    l0.SetLineStyle(2)
    l0.Draw('l')
    c.cd(4)
    hsc_r[1].Draw("")
    hsc_r[1].GetYaxis().SetRangeUser(0.90,1.10);
    hsc_r[1].GetYaxis().SetTitle('mu- scale factor');
    hsc_r[1].GetXaxis().SetTitle(opts.var);
    l1 = ROOT.TGraph(2)
    l1.SetPoint(0,hsc_r[0].GetXaxis().GetXmin(),1.0)
    l1.SetPoint(1,hsc_r[0].GetXaxis().GetXmax(),1.0)
    l1.SetLineWidth(1)
    l1.SetLineColor(ROOT.kRed)
    l1.SetLineStyle(2)
    l1.Draw('l')
    if not opts.antondb:
        c.SaveAs('probe_%s.png'%(opts.tag))
    OMAP += [c]

# save everything
if len(COUT)>0:
    VMAP['COUT']=[]
    for l in COUT:
        print l
        VMAP['COUT'].append(l)
if (len(VMAP)>0 or len(OMAP)>0) and opts.antondb:
    a = antondb.antondb(opts.antondb)
    path = os.path.join('/stack/',opts.tag,QMAP[opts.charge][1])
    if len(VMAP)>0:
        a.add(path,VMAP)
    if len(OMAP)>0:
        a.add_root(path,OMAP)
