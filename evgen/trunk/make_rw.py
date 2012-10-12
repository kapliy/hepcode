#!/usr/bin/env python

"""
Creates powheg_pythia -> powheg_herwig reweighting map.
Reweighting is done via a 2D histogram: mu_pt vs nu_pt
"""

import array
ptbins_l = [0,5,10,15,20,25,30,35,40,45,50,70,90,120,200]
ptbins_l = range(0,70)
ptbins = array.array('f',ptbins_l)

# Normally this should be set to True. Only set it to False when running a closure test.
DOSCALE = True
# Set this to a small number for quicker tests
MAXENTRIES = 100000000
#MAXENTRIES = 10000
# Re-read old results from root file?
mode = 'RECREATE' # 'UPDATE'

import os,sys,re,glob
if not 'libPyRoot' in sys.modules: #hack to get rid of TenvRec warnings
    from ROOT import gROOT
    sys.modules['libPyROOT'].gROOT.GetListOfGlobals().FindObject('gErrorIgnoreLevel').GetAddress().__setitem__(0,3000)
    import ROOT
    gErrorIgnoreLevel=ROOT.kInfo
    sys.modules['libPyROOT'].gROOT.GetListOfGlobals().FindObject('gErrorAbortLevel').GetAddress().__setitem__(0,5001)

def FixEdgeBins(hist,v=1.0):
    for binX in xrange(0,hist.GetNbinsX()+2):
        for binY in xrange(0,hist.GetNbinsY()+2):
            for binZ in xrange(0,hist.GetNbinsZ()+2):
                globalBin = hist.GetBin(binX,binY,binZ)
                if hist.IsBinUnderflow(globalBin) or hist.IsBinOverflow(globalBin):
                    hist.SetBinContent(globalBin,v)
    return True

def means(h1,h2,n='Comparison',b1=1,b2=3):
    print '%s: '%n,h1.GetBinContent(b1),h2.GetBinContent(b1),'|',h1.GetBinContent(b2),h2.GetBinContent(b2),'|',h1.Chi2Test(h2,'UU CHI2/NDF'),h1.KolmogorovTest(h2)

if __name__=='__main__':
    ROOT.gROOT.SetBatch(ROOT.kTRUE)
    ROOT.TH1.SetDefaultSumw2(ROOT.kTRUE)
    fout = ROOT.TFile.Open('pythia_to_herwig_rw.root',mode)
    fout.cd()

    fherwig = ROOT.TFile.Open('root_D3PD_HERWIG.root'); assert fherwig.IsOpen()
    therwig = fherwig.Get('tree'); assert therwig

    fpythia = ROOT.TFile.Open('root_FLAT_PYTHIA.root'); assert fpythia.IsOpen()
    tpythia = fpythia.Get('tree'); assert tpythia

    # this one is just for sanity check
    fferwig = ROOT.TFile.Open('root_FLAT_HERWIG.root'); assert fferwig.IsOpen()
    tferwig = fferwig.Get('tree'); assert tferwig

    hherwig = fout.Get('herwig')
    hherwig2 = fout.Get('herwig2')
    if not hherwig:
        print 'Making herwig histograms...'
        fout.cd()
        # 1D version (lepton pT only)
        hherwig = ROOT.TH1D('herwig','herwig',len(ptbins)-1,ptbins)
        therwig.Draw('mu_pt>>herwig','','goff',MAXENTRIES)
        FixEdgeBins(hherwig,0.0)
        hherwig.SetLineColor(ROOT.kBlue)
        hherwig.Write(hherwig.GetName(),ROOT.TObject.kOverwrite)
        # 2D version (lepton pT and neutrino pT)
        hherwig2 = ROOT.TH2D('herwig2','herwig2',len(ptbins)-1,ptbins,len(ptbins)-1,ptbins)
        therwig.Draw('nu_pt:mu_pt>>herwig2','','goff',MAXENTRIES)
        FixEdgeBins(hherwig2,0.0)
        hherwig2.SetLineColor(ROOT.kBlue)
        hherwig2.Write(hherwig2.GetName(),ROOT.TObject.kOverwrite)
    hferwig = fout.Get('ferwig')
    hferwig2 = fout.Get('ferwig2')
    if not hferwig:
        print 'Making ferwig histograms...'
        fout.cd()
        # 1D version (lepton pT only)
        hferwig = ROOT.TH1D('ferwig','ferwig',len(ptbins)-1,ptbins)
        tferwig.Draw('mu_pt>>ferwig','','goff',MAXENTRIES)
        FixEdgeBins(hferwig,0.0)
        hferwig.SetLineColor(ROOT.kBlack)
        hferwig.Write(hferwig.GetName(),ROOT.TObject.kOverwrite)
        # 2D version (lepton pT and neutrino pT)
        hferwig2 = ROOT.TH2D('ferwig2','ferwig2',len(ptbins)-1,ptbins,len(ptbins)-1,ptbins)
        tferwig.Draw('nu_pt:mu_pt>>ferwig2','','goff',MAXENTRIES)
        FixEdgeBins(hferwig2,0.0)
        hferwig2.SetLineColor(ROOT.kBlack)
        hferwig2.Write(hferwig2.GetName(),ROOT.TObject.kOverwrite)
    hpythia = fout.Get('pythia')
    hpythia2 = fout.Get('pythia2')
    hratio = fout.Get('scale')
    hratio2 = fout.Get('scale2')
    if not hpythia:
        print 'Making pythia histograms...'
        fout.cd()
        # 1D version (lepton pT only)
        hpythia = ROOT.TH1D('pythia','pythia',len(ptbins)-1,ptbins)
        tpythia.Draw('mu_pt>>pythia','','goff',MAXENTRIES)
        FixEdgeBins(hpythia,0.0)
        hpythia.SetLineColor(ROOT.kRed)
        hpythia.Write(hpythia.GetName(),ROOT.TObject.kOverwrite)
        # make ratio histogram
        if True:
            hratio = hherwig.Clone('scale')
            if DOSCALE:
                hratio.Scale(1.0/hratio.Integral())
            hpythia_sc = hpythia.Clone();
            if DOSCALE:
                hpythia_sc.Scale(1.0/hpythia_sc.Integral())
            hratio.Divide(hpythia_sc)
            FixEdgeBins(hratio,1.0) # in case 0/0 gives ambiguity
            hratio.Write(hratio.GetName(),ROOT.TObject.kOverwrite)
        # 2D version (lepton pT and neutrino pT)
        hpythia2 = ROOT.TH2D('pythia2','pythia2',len(ptbins)-1,ptbins,len(ptbins)-1,ptbins)
        tpythia.Draw('nu_pt:mu_pt>>pythia2','','goff',MAXENTRIES)
        FixEdgeBins(hpythia2,0.0)
        hpythia2.SetLineColor(ROOT.kRed)
        hpythia2.Write(hpythia2.GetName(),ROOT.TObject.kOverwrite)
        # make ratio histogram
        if True:
            hratio2 = hherwig2.Clone('scale2')
            hratio2.SetLineColor(ROOT.kBlack)
            if DOSCALE:
                hratio2.Scale(1.0/hratio2.Integral())
            hpythia2_sc = hpythia2.Clone()
            if DOSCALE:
                hpythia2_sc.Scale(1.0/hpythia2_sc.Integral())
            hratio2.Divide(hpythia2_sc)
            FixEdgeBins(hratio2,1.0) # in case 0/0 gives ambiguity
            hratio2.Write(hratio2.GetName(),ROOT.TObject.kOverwrite)
    else:
        assert hratio
        assert hratio2
    means(hherwig,hpythia,  '1D Herwig/Pythia')
    means(hherwig2,hpythia2,'2D Herwig/Pythia',b1=152,b2=167)
    fout.cd()
    def test(dim=1):
        """ Closure test """
        # event-by-event closure test, and total-histogram closure test
        hrw = hpythia.Clone('closure')           # hrw via event-by-event weight
        hrw.Reset()
        san_hrw = hpythia.Clone('san_hrw')   # hrw     via global Multiply()
        san_hrw.Reset()
        hrw.SetLineColor(ROOT.kGreen);
        san_hrw.SetLineColor(ROOT.kMagenta)
        hrw2 = hpythia2.Clone('closure2')
        hrw2.Reset()
        san_hrw2 = hpythia2.Clone('san_hrw2')   # hrw2     via global Multiply()
        san_hrw2.Reset()
        t = tpythia
        print 'Making closure...'
        for x in xrange(t.GetEntries()):
            if x%1000000==0:
                print 'Passed %dM / %dM'%(x/1000000,t.GetEntries()/1000000)
            t.GetEntry(x)
            mu,nu = t.mu_pt,t.nu_pt
            hrw.Fill(mu,1.0*hratio.GetBinContent(hratio.FindBin(mu)))
            san_hrw.Fill(mu,1.0)
            hrw2.Fill(mu,nu,1.0*hratio2.GetBinContent(hratio2.FindBin(mu,nu)))
            san_hrw2.Fill(mu,nu,1.0)
            if MAXENTRIES>0 and x+1>=MAXENTRIES: break
        FixEdgeBins(hrw,0.0)
        FixEdgeBins(san_hrw,0.0)
        san_pythia = san_hrw.Clone('san_pythia')
        san_pythia.Write('san_pythia',ROOT.TObject.kOverwrite) # re-built pythia histogram
        san_hrw.Multiply(hratio)
        FixEdgeBins(hrw2,0.0)
        FixEdgeBins(san_hrw2,0.0)
        san_pythia2 = san_hrw2.Clone('san_pythia2')
        san_pythia2.Write('san_pythia2',ROOT.TObject.kOverwrite) # re-built pythia histogram
        san_hrw2.Multiply(hratio2)
        # write extra histograms
        hrw.Write('hrw',ROOT.TObject.kOverwrite)
        san_hrw.Write('san_hrw',ROOT.TObject.kOverwrite)
        hrw2.Write('hrw2',ROOT.TObject.kOverwrite)
        san_hrw2.Write('san_hrw2',ROOT.TObject.kOverwrite)
        # print out some sanity printout
        print '======MEANS1D======'
        means(hpythia,san_pythia,'1D Pythia/Rebuilt-Pythia')
        means(hrw,san_hrw,'1D Weighted/ReScaled (Sanity)')
        means(hrw,hherwig,'1D Weighted/Herwig')
        print '======MEANS2D======'
        means(hpythia2,san_pythia2,'2D Pythia/Rebuilt-Pythia',b1=152,b2=167)
        means(hrw2,san_hrw2,'2D Weighted/ReScaled (Sanity)',b1=152,b2=167)
        means(hrw2,hherwig2,'2D Weighted/Herwig',b1=152,b2=167)
        # draw several normalized histograms on canvas
        c = ROOT.TCanvas('c','c',800,600)
        c.cd()
        hherwig.Draw('H')
        hpythia.Draw('A SAME H')
        hrw.Draw('A SAME H')
        san_hrw.Draw('A SAME H')
        c.Modified()
        c.Update()
        c.Write('canvas',ROOT.TObject.kOverwrite)
    test()
    fout.Close()
