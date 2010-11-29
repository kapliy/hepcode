#!/usr/bin/env python

from optparse import OptionParser
parser = OptionParser()
parser.add_option("--data",dest="data",
                  type="string", default=None,
                  help="Path to data container")
parser.add_option("--mc",dest="mc",
                  type="string", default=None,
                  help="Path to mc container")
parser.add_option("--lumi",dest="lumi",
                  type="float", default=23000.0,
                  help="Integrated luminosity for data (in nb^-1)")
parser.add_option("--qcd-scale",dest="qcdscale",
                  type="float", default=0.62,
                  help="QCD scale factor")
parser.add_option("-o", "--output",dest="output",
                  type="string", default="output",
                  help="Name of output dir for plots")
(opts, args) = parser.parse_args()


ROOT.TH1.AddDirectory(ROOT.kFALSE)    # ensure that we own all the histograms
ROOT.gROOT.SetBatch(1)                # uncomment for interactive usage

def SetStyle(styleMacro=''):
    """ Global style settings for ROOT """
    if styleMacro=='':
        ROOT.gStyle.SetCanvasColor(0);
        ROOT.gStyle.SetCanvasBorderMode(0);
        ROOT.gStyle.SetPadColor(0);
        ROOT.gStyle.SetPadBorderMode(0);
        ROOT.gStyle.SetStatColor(0);
        
        ROOT.gStyle.SetOptStat(111111);
        ROOT.gStyle.SetOptFit(1111);
        ROOT.gStyle.SetHistFillColor(0);
        ROOT.gStyle.SetMarkerStyle(20);
        ROOT.gStyle.SetMarkerSize(.4);
        ROOT.gStyle.SetHistLineWidth(2);
        ROOT.gStyle.SetErrorX(0);
        
        ROOT.gStyle.SetTitleStyle(0);
        
        ROOT.gStyle.SetStatBorderSize(1);
        ROOT.gStyle.SetFrameFillColor(10);
        ROOT.gStyle.SetTitleFillColor(0);
        ROOT.gROOT.ForceStyle()
    else:
        ROOT.gROOT.LoadMacro(styleMacro);
SetStyle()

# Determine plot ordering for MC stacks
po = PlotOrder()
po.Add(name='t#bar{t}',samples='ttbar',color=ROOT.kGreen)
po.Add(name='Z#rightarrow#tau#tau',samples='ztautau',color=ROOT.kMagenta)
po.Add(name='W#rightarrow#tau#nu',samples='wtaunu',color=ROOT.kYellow)
po.Add(name='Z#rightarrow#mu#mu',samples='zmumu',color=ROOT.kRed)
po.Add(name='QCD',samples=['J%d'%z for z in range(6)],color=ROOT.kCyan)
po.Add(name='W#rightarrow#mu#nu',samples='wmunu',color=10)

h = {}
h['data'] = []
h['mc']   = []

f = ROOT.TFile.Open(opts.data)
for sfile in ['periodG','periodH','periodI']:
    g=f.GetDirectory(sfile+'.root').GetDirectory('dg').GetDirectory('dg').GetDirectory('st_w_final')
    pos = g.GetDirectory('POS')
    neg = g.GetDirectory('NEG')
    heta.append(pos.Get('lepton_eta').Clone())
    heta.append(neg.Get('lepton_eta').Clone())



# load all histograms
for attr in files:
    if getattr(opts,attr):
        inputs = getattr(opts,attr).split(',')
        for input in inputs:
            input = os.path.expanduser(input)
            for file in glob.glob(input):
                # skip those mc runs that are not known to MC.py
                if attr=='mc':
                    if not mc09.match_run(file):
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
