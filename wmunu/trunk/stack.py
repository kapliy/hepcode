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
                  type="float", default=310.01,
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
from MC import mc09
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
#plot.Add(name='QCD',samples=['J%d_pythia_jetjet_1muon'%z for z in range(6)]+['PythiaB_ccmu15X','PythiaB_bbmu15X'],color=ROOT.kCyan)
plot.Add(name='W#rightarrow#mu#nu',samples='PythiaWmunu_no_filter',color=10)

# Determine which plots to make
whitelist = ['ANA_*','PRESEL_*','EVENT*']
blacklist = ['*_vs_*']

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
                files[attr][-1].Load(whitelist,blacklist)
                if attr=='mc':
                    files[attr][-1].ScaleToLumi(opts.lumi,opts.qcdscale)
                files[attr][-1].Close()
print 'Loaded %d MC files and %d DATA files'%(len(files['mc']),len(files['data']))

# prepare output
out = AnaFile("fout.root",mode="RECREATE")
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
out.AddToLegend(out,'Data(#int L dt = %.1f nb^{-1})'%opts.lumi,'LP')

print 'Saving final plots...'
import SimpleProgressBar
bar = SimpleProgressBar.SimpleProgressBar(20,len(out.keys))

for i,key in enumerate(out.keys):
    print bar.show(i),key
    out.Draw(key,savedir=opts.output,log=True)
