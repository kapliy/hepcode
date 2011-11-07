#!/usr/bin/env python
_INPUTS='/home/melachri/analysis/user.Constantinos.Jul01.group.phys-top.group10.phys-top.D2PDs_rel15.6.10.4.1_data10_7TeV.periodA.physics_MuonswBeam.PhysCont.AOD.repro04_v01.20100621_D2AODM_TOPMU.d3pd._00001.root'

try:
    import psyco
    psyco.full()
    print 'Using psyco'
except:
    pass

import math,sys,glob,re,os
import ROOT, PyCintex
PyCintex.Cintex.Enable()

ROOT.gROOT.SetBatch(1) #uncomment for interactive usage
ROOT.SetSignalPolicy( ROOT.kSignalFast )

from optparse import OptionParser
parser = OptionParser()
parser.add_option("-n", "--nevents",dest="nevents",
                  type="int", default=-1,
                  help="Maximum number of events to process")
parser.add_option("-i", "--input",dest="input",
                  type="string", default=_INPUTS,
                  help="Full path to ntuple to process")
parser.add_option("-o", "--output",dest="output",
                  type="string", default="outdir",
                  help="Name of output dir for plots and cutflow")

(opts, args) = parser.parse_args()
_INPUTS = opts.input
outdir=opts.output


ch0 = ROOT.TChain('InfoTree')
ch0.Add(_INPUTS)
nentries = ch0.GetEntries()
ch1 = ROOT.TChain('RecoTree')
ch1.Add(_INPUTS)
ch1.GetEntries()
ch2 = ROOT.TChain('TriggerTree')
ch2.Add(_INPUTS)
ch2.GetEntries()

ch0.SetBranchStatus("*", 0)
ch1.SetBranchStatus("*", 0)
ch2.SetBranchStatus("*", 0)
# Info Tree
infovars=["RunNumber","EventNumber","LumiBlock","McEventWeight","PrimaryVertex_position_*"]
for i in infovars:
    ch0.SetBranchStatus(i,1)
# Reco Tree
recovars=["MET_RefFinal_*","MuidMuon_N","MuidMuon_pt","MuidMuon_eta","MuidMuon_phi","MuidMuon_e","MuidMuon_charge","MuidMuon_author","MuidMuon_etcone20","MuidMuon_etcone30","MuidMuon_ptcone30","MuidMuon_*pt","MuidMuon_*z0","MuidMuon_bestMatch","MuidMuon_TopInputs*","Electron_N","Electron_pt","Electron_eta","Electron_phi","Electron_e","Electron_TopInputs*","AntiKt4TopoEMJESJets_N","AntiKt4TopoEMJESJets_*Weight","AntiKt4TopoEMJESJets_is*","AntiKt4TopoEMJESJets_pt","AntiKt4TopoEMJESJets_eta","AntiKt4TopoEMJESJets_phi","AntiKt4TopoEMJESJets_e","AntiKt4TopoEMJESJets_et","Jet_N","Jet_pt","Jet_eta","Jet_phi","Jet_e","Jet_et","Track_N","Track_pt","Track_eta","Track_phi","Track_e","Track_vertex_Type","Track_vertex_position_z","EF_mu10_*"]
for i in recovars:
    ch1.SetBranchStatus(i,1)
# TriggerTree
trigvars=["EF_mu10_EFMuon_*","Passed_EF_mu10","EF_e10_medium_EFElectron_*","Passed_EF_e10_medium","Passed_L1_mu6"]
for i in trigvars:
    ch2.SetBranchStatus(i,1)


# Write to new file
if not os.path.isdir(outdir):
    os.makedirs(outdir)
outFile = outdir + '/' + _INPUTS.split('/')[-1].strip('.root') + '.slim.root'
newFile = ROOT.TFile(outFile, "RECREATE")

ch0_new = ch0.CloneTree(0)
ch1_new = ch1.CloneTree(0)
ch2_new = ch2.CloneTree(0)

print 'Starting loop over',(opts.nevents if (opts.nevents<nentries and opts.nevents!=-1) else nentries),'/',nentries,'entries'

for i in range(nentries):
    if i>=opts.nevents and opts.nevents!=-1:
        break
    ch0.GetEntry(i)
    ch1.GetEntry(i)
    ch2.GetEntry(i)
    ch0_new.Fill()
    ch1_new.Fill()
    ch2_new.Fill()

# use GetCurrentFile just in case we went over the
# (customizable) maximum file size 
ch0_new.GetCurrentFile().Write()
ch0_new.GetCurrentFile().Close()
