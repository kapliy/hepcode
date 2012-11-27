#!/usr/bin/env python

""" Debugging systematic trees """
import ROOT

import sys

fname = '/home/antonk/d3pd/SYS/AnalysisManager.Wmunu.mc11_7TeV.p833_v0115_plus_syst_powheg_pythia.root'
if len(sys.argv)>1:
    fname = '/home/antonk/d3pd/SYS/20121126.unfold.root'

print 'Opening:',fname
f = ROOT.TFile.Open(fname)

def run_tree(name):
    t = f.Get(name)
    N = [ 0,  0,0,0]
    h  = [ ROOT.TH1D('hT'+name,'hT'+name,200,0,100) ]
    h += [ ROOT.TH1D('hR'+name,'hR'+name,200,0,100) ]
    if not t:
        #print 'ERROR: cannot find tree',name
        return None,None
    N[0] = t.GetEntries()
    for i in xrange(0,N[0]):
        t.GetEntry(i)
        if t.Truth_Is_Fiducial:
            N[1] += t.Truth_Mc_Weight
        if t.Reco_isReconstructed:
            N[2] += t.Reco_Weight
            if t.Truth_Is_Fiducial:
                N[3] += t.Reco_Weight
        if t.Truth_Is_Fiducial and t.Reco_isReconstructed:
            h[0].Fill(t.Truth_PtLep,t.Truth_Mc_Weight)
            h[1].Fill(t.Reco_PtLep,t.Reco_Weight)
    return N,h

TREES = ['physics_Nominal',
         'physics_NominalWptSherpa',
         'physics_NominalWptPythiaMC10',
         'physics_NominalWptAlpgenMC11',
         'physics_NominalPdfCT10',
         'physics_NominalPdfMSTW',
         'physics_NominalPdfNNPDF',
         'physics_NominalPdfHERA',
         'physics_MuonRecoSFUp',
         'physics_MuonRecoSFDown',
         'physics_MuonTriggerSFUp',
         'physics_MuonTriggerSFDown',
         'physics_MuonIsoSFUp',
         'physics_MuonIsoSFDown',
         'physics_ResoSoftTermsUp_ptHard',
         'physics_ResoSoftTermsDown_ptHard',
         'physics_ScaleSoftTermsUp_ptHard',
         'physics_ScaleSoftTermsDown_ptHard',
         'physics_JetResolUp',
         'physics_JetResolDown',
         'physics_JetScaleUp',
         'physics_JetScaleDown',
         'physics_JetNPVUp',
         'physics_JetNPVDown',
         'physics_JetMUUp',
         'physics_JetMUDown',
         'physics_MuonResMSUp',
         'physics_MuonResMSDown',
         'physics_MuonResIDUp',
         'physics_MuonResIDDown',
         'physics_MuonNoScale',
         'physics_MuonKScaleUp',
         'physics_MuonKScaleDown',
         'physics_MuonCScaleUp',
         'physics_MuonCScaleDown',
         'physics_MuonScaleKUp',
         'physics_MuonScaleKDown',
         'physics_MuonScaleCUp',
         'physics_MuonScaleCDown' ]

for tree in TREES:
    N,h = run_tree(tree)
    if N:
        print tree,N,h[0].GetMean(),h[1].GetMean()

