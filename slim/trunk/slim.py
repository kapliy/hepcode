#!/usr/bin/env python

""" A slimming script """

import math,sys,glob,re,os
import common
import ROOT
ROOT.gROOT.SetBatch(1)
ROOT.gROOT.ProcessLine('.L loader.C+')
ROOT.SetSignalPolicy( ROOT.kSignalFast )

_INPUT = '/home/antonk/slim/flatntuple.root'
_INPUT = 'LIST'
from optparse import OptionParser
parser = OptionParser()
parser.add_option("-n", "--nevents",dest="nevents",
                  type="int", default=-1,
                  help="Maximum number of events")
parser.add_option("-i", "--input",dest="input",
                  type="string", default=_INPUT,
                  help="ROOT file or input file list")
parser.add_option("-o", "--output",dest="output",
                  type="string", default=None,
                  help="Name of output dir for plots and cutflow")
parser.add_option( "--tree",dest="tree",
                  type="string", default="tree",
                  help="Name of TTree")
parser.add_option( "--var",dest="var",
                  type="string", default=None,
                  help="Text file listing variables")
parser.add_option( "--grl",dest="grl",
                  type="string", default=None,
                  help="Path to good run list")
parser.add_option( "--pt",dest="pt",
                  type="float", default=None,
                  help="Minimum pt cut to apply")
parser.add_option('--trigger', default=False,
                  action="store_true",dest="trigger",
                  help="Apply trigger selection?")
(opts, args) = parser.parse_args()

# variable list
varlist = []
if opts.var:
    f = open(opts.var,'read')
    varlist += [ff.strip() for ff in f]
    f.close()

# File list
flist = []
if re.search('.root',opts.input):
    flist.append(opts.input)
else:
    f = open(opts.input,'read')
    flist += [ff.strip() for ff in f]
    f.close()

# Input TChain
ch = ROOT.TChain(opts.tree,opts.tree)
[ch.Add(ff) for ff in flist]
nentries = ch.GetEntries()
if nentries==0:
    print 'Nothing to do: found zero entries in file list:'
    print flist
    sys.exit(0)
ch.LoadTree(0)
ch.SetBranchStatus("*", 1)
if len(varlist)>0:
    all_br_names = list(set(br.GetName() for br in ch.GetListOfBranches()))
    for var in all_br_names:
        if var not in varlist:
            ch.SetBranchStatus(var,0)
            if re.search('mu_trig_l1match',var):
                ch.SetBranchStatus("mu_trig_l1match*",0);
            if re.search('mu_trig_l2match',var):
                ch.SetBranchStatus("mu_trig_l2match*",0);
            if re.search('mu_trig_l2samatch',var):
                ch.SetBranchStatus("mu_trig_l2samatch*",0);

# TTreeCache on input TChain
tree = ch.GetTree()
cachesize = 100000000
tree.SetCacheSize(cachesize);
for sub in tree.GetListOfBranches():
    if tree.GetBranchStatus(sub.GetName()):
        tree.AddBranchToCache(sub.GetName(),ROOT.kTRUE)
tree.StopCacheLearningPhase()

# GoodRunList
grl = None
if opts.grl:
    #ROOT.gSystem.Load('/home/antonk/usr/LIBXML/lib/libxml2.so')
    ROOT.gSystem.Load('/cvmfs/atlas.cern.ch/repo/sw/software/x86_64-slc5-gcc43-opt/17.6.0/sw/lcg/app/releases/ROOT/5.34.00/x86_64-slc5-gcc43-opt/root/lib/libXMLParser.so')
    ROOT.gSystem.Load('/home/antonk/TrigFTKAna/CommonAnalysis/GoodRunsLists/StandAlone/libGoodRunsLists.so')
    grlR = ROOT.Root.TGoodRunsListReader()
    grl = ROOT.Root.TGoodRunsList()
    grlR.SetXMLFile(opts.grl)
    grlR.Interpret()
    grl = grlR.GetMergedGoodRunsList()

# Output chain
outFile = (flist[0].split('/'))[-1].strip('.root') + '.slim.root' if not opts.output else opts.output
newFile = ROOT.TFile(outFile, "RECREATE")
ch_new = ch.GetTree().CloneTree(0)

# Copy loop
print 'Processing %d entries...'%nentries
isaved = 0
for i in range(nentries):
    ch.GetEntry(i)
    # trigger
    if opts.trigger and not (  ('EF_mu18_MG_medium' in ch.trig_ef) or ('EF_mu18_MG' in ch.trig_ef)  ):
        continue
    # grl (optional)
    if grl and not grl.HasRunLumiBlock(ch.run,ch.lb):
        continue
    # pt cut (optional)
    if opts.pt!=None and len( [z for z in xrange(ch.nmu) if ch.mu_pt[z]>=opts.pt] )==0:
        continue
    ch_new.Fill()
    isaved += 1

# use GetCurrentFile just in case we went over the
# (customizable) maximum file size 
ch_new.GetCurrentFile().Write()
ch_new.GetCurrentFile().Close()

print 'Saved %d / %d events (efficiency = %.1f%%)'%(isaved,nentries,isaved*100.0/nentries)
