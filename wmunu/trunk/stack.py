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
                  type="float", default=16.5,
                  help="Integrated luminosity for data (in nb^-1)")
parser.add_option("--qcdscale",dest="qcdscale",
                  type="float", default=0.575,
                  help="QCD scale factor")
parser.add_option("--mc",dest="mc",
                  type="string", default=None,
                  help="Path to mc folder")
(opts, args) = parser.parse_args()

from helpers import *
from MC import *
ROOT.TH1.AddDirectory(ROOT.kFALSE)  # ensure that we own all the histograms
SetStyle(styleMacro='AtlasStyleWZ.C')

# mc ordering
mcg = [ ['TTbar_PowHeg_Pythia'], ['PythiaZtautau'], ['PythiaWtaunu_1Lepton'], ['PythiaZmumu_no_filter'], ['J%d_pythia_jetjet_1muon'%z for z in range(6)], ['PythiaWmunu_no_filter'] ]
# mc colors
mcgc = [ ROOT.kGreen,ROOT.kMagenta,ROOT.kYellow,ROOT.kRed,ROOT.kCyan,ROOT.kWhite ]

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
            s.keys.append(key)
            hobj = s.file.Get(key)
            s.h[key] = hobj.Clone()
    def ScaleToLumi(s,lumi):
        mrun = s.mrun = mc09.match_run(s.path)
        if mrun:
            xsec = s.xsec = mrun.xsec
            nevents = s.nevents = mrun.nevents
            sample = s.sample = mrun.sample
            for hh in s.h.values():
                hh.Scale(1.0/nevents*lumi*xsec)
                if sample in ['J%d_pythia_jetjet_1muon'%z for z in range(6)]:
                    #hh.Scale(opts.qcd_scale)
                    pass
        else:
            print 'Failed to look up xsection for:',s.path
    def ChangeColor(s,color):
        [hh.SetLineColor(color) for hh in s.h.values()]
    def AddHistos(s,file):
        """ Add to itself all histograms from another AnaFile """
        [s.h[key].Add(file.h[key]) for key in s.keys if key in file.h]
    def InitStack(s):
        s.hs = {}
        for key in s.keys:
            s.hs[key] = ROOT.THStack(key,key)
    def AddToStack(s,file):
        [s.hs[key].Add(file.h[key]) for key in s.keys if key in file.h]
    def Write(s):
        pass
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
                if attr=='mc' and not mc09.match_run(file):
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

# plot mc using mcg ordering
for i,bgnames in enumerate(mcg):
    filecands = [f for f in files['mc'] if f.sample in bgnames]
    if len(filecands)!=len(bgnames):
        print 'WARNING: skipping',bgnames
        continue
    [filecands[0].AddHistos(fc) for fc in filecands[1:]]
    filecands[0].ChangeColor(mcgc[i])
    out.AddToStack(filecands[0])
