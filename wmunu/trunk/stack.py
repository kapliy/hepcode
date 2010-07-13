#!/usr/bin/env python

try:
    import psyco
    psyco.full()
except ImportError:
    pass

from optparse import OptionParser
parser = OptionParser()
parser.add_option("--data",dest="data",
                  type="string", default=None,
                  help="Path to data folder")
parser.add_option("--lumi",dest="lumi",
                  type="float", default=16.6,
                  help="Integrated luminosity for data (in nb^-1)")
parser.add_option("--qcd-scale",dest="qcdscale",
                  type="float", default=0.575,
                  help="QCD scale factor")
parser.add_option("--mc",dest="mc",
                  type="string", default=None,
                  help="Path to mc folder")
parser.add_option("-o", "--output",dest="output",
                  type="string", default="output",
                  help="Name of output dir for plots and cutflow")
(opts, args) = parser.parse_args()

from helpers import *
from MC import *
ROOT.TH1.AddDirectory(ROOT.kFALSE)  # ensure that we own all the histograms
ROOT.gROOT.SetBatch(1) #uncomment for interactive usage
SetStyle()

# Determine plot ordering for MC stacks
plot = PlotOrder()
plot.Add(name='t#bar{t}',samples='TTbar_PowHeg_Pythia',color=ROOT.kGreen)
plot.Add(name='Z#rightarrow#tau#tau',samples='PythiaZtautau',color=ROOT.kMagenta)
plot.Add(name='W#rightarrow#tau#nu',samples='PythiaWtaunu_1Lepton',color=ROOT.kYellow)
plot.Add(name='Z#rightarrow#mu#mu',samples='PythiaZmumu_no_filter',color=ROOT.kRed)
plot.Add(name='QCD',samples=['J%d_pythia_jetjet_1muon'%z for z in range(6)],color=ROOT.kCyan)
plot.Add(name='W#rightarrow#mu#nu',samples='PythiaWmunu_no_filter',color=10)

class AnaFile():
    """ Histogram holder file """
    def __str__(s):
        return s.path
    def __init__(s,path,mode='READ'):
        s.path = path
        s.file = ROOT.TFile.Open(path,mode)
        s.keys = []
        s.h = {}
        s.mrun = None
        s.xsec = None
        s.nevents = None
        s.sample = None
    def LoadHistos(s):
        hl=s.file.GetListOfKeys()
        for obj in hl:
            key = obj.GetName()
            if key in ('_meta','_cutflow'):
                continue
            s.keys.append(key)
            hobj = s.file.Get(key)
            s.h[key] = hobj.Clone()
    def ScaleToLumi(s,lumi):
        mrun = s.mrun = mc09.match_run(s.path)
        if mrun:
            xsec = s.xsec = mrun.xsec*mrun.filteff
            nevents = s.nevents = mrun.nevents
            sample = s.sample = mrun.sample
            print 'MC %s: \t\t xsec=%.1f nb'%(sample,xsec)
            for hh in s.h.values():
                hh.Scale(1.0/nevents*lumi*xsec)
                if sample in ['J%d_pythia_jetjet_1muon'%z for z in range(10)]:
                    hh.Scale(opts.qcdscale)
                    pass
        else:
            print 'Failed to look up xsection for:',s.path
    def ChangeColor(s,color):
        [hh.SetLineColor(ROOT.kBlack) for hh in s.h.values()]
        [hh.SetFillColor(color) for hh in s.h.values()]
    def AddHistos(s,file):
        """ Add to itself all histograms from another AnaFile """
        [s.h[key].Add(file.h[key]) for key in s.keys if key in file.h]
    def InitStack(s):
        s.hs = {}
        s.leg = {}
        for key in s.keys:
            s.leg[key] = ROOT.TLegend(0.55,0.70,0.88,0.88,"Data and MC","brNDC")
            s.leg[key].SetFillStyle(1001);
            s.leg[key].SetFillColor(10);
            s.hs[key] = ROOT.THStack(key,key)
    def AddToStack(s,file):
        [s.hs[key].Add(file.h[key]) for key in s.keys if key in file.h]
    def AddToLegend(s,file,name,type='F'):
        [s.leg[key].AddEntry(file.h[key],name,type) for key in s.keys if key in file.h]
    def Draw(s,key,savedir=None,log=False):
        if key in s.h and key in s.hs:
            maximum = max((s.h[key].GetMaximum(),s.hs[key].GetMaximum()))
            # prepare the canvas
            c = ROOT.TCanvas('c'+key,key,800,600)
            c.cd()
            outext='.png'
            # mc
            s.hs[key].Draw("H")
            s.hs[key].SetMinimum(0.1)
            s.hs[key].SetMaximum(maximum*1.25)
            #s.hs[key].GetHistogram().GetYaxis().SetRangeUser(0.0,maximum*1.25)
            #data
            s.h[key].SetMarkerSize(1.0)
            s.h[key].Draw("Lsame")
            s.leg[key].Draw("same")
            # save the plot?
            if savedir:
                if not os.path.exists(savedir):
                    os.makedirs(savedir)
                c.SaveAs(os.path.join(savedir,key+outext))
                if log:
                    ROOT.gPad.SetLogy(ROOT.kTRUE)
                    outext='_log%s'%outext
                    c.SaveAs(os.path.join(savedir,key+outext))
            return c
    def Close(s):
        if s.file.IsOpen():
            s.file.Close()

files = {}
files['data'] = []
files['mc']   = []
# load all histograms
for attr in files:
    if getattr(opts,attr):
        inputs = getattr(opts,attr).split(',')
        for input in inputs:
            input = os.path.expanduser(input)
            for file in glob.glob(input):
                # skip those mc runs that are not known to MC.py
                if attr=='mc':
                    if  not mc09.match_run(file):
                        continue
                    if mc09.get_sample(file) not in list(xflatten(plot.mcg)):
                        continue
                file = addDcachePrefix(file)
                files[attr].append(AnaFile(file))
                files[attr][-1].LoadHistos()
                if attr=='mc':
                    files[attr][-1].ScaleToLumi(opts.lumi)
                files[attr][-1].Close()

# prepare output
out = AnaFile("fout.root","RECREATE")
if len(files['mc'])>0:
    out.keys = files['mc'][0].keys
elif len(files['data'])>0:
    out.keys = files['data'][0].keys
else:
    print 'Nothing to do...'
    sys.exit(0)
out.InitStack()

# create mc histogram stacks
for i,bgnames in enumerate(plot.mcg):
    filecands = [f for f in files['mc'] if f.sample in bgnames]
    if len(filecands)!=len(bgnames):
        print 'WARNING: skipping',bgnames
        continue
    [filecands[0].AddHistos(fc) for fc in filecands[1:]]
    filecands[0].ChangeColor(plot.mcgc[i])
    out.AddToStack(filecands[0])
    out.AddToLegend(filecands[0],plot.mcg_name[i],'F')
# create data histograms
[files['data'][0].AddHistos(fc) for fc in files['data'][1:]]
out.h = files['data'][0].h
out.AddToLegend(out,'Data(%.1f nb^{-1})'%opts.lumi,'LP')

print 'Saving final plots...'
import SimpleProgressBar
bar = SimpleProgressBar.SimpleProgressBar(20,len(out.keys))
for i,key in enumerate(out.keys):
    print bar.show(i),key
    out.Draw(key,savedir=opts.output,log=True)
