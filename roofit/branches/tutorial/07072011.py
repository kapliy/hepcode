#!/usr/bin/env python

"""
KEY: TDirectoryFileWP;1WP
KEY: TDirectoryFileWP_sc00;1WP_sc00
KEY: TDirectoryFileWP_sc01;1WP_sc01
KEY: TDirectoryFileWP_sc10;1WP_sc10
KEY: TDirectoryFileWP_sc11;1WP_sc11
KEY: TDirectoryFileWP_sm0;1WP_sm0
KEY: TDirectoryFileWP_sm1;1WP_sm1
KEY: TDirectoryFileWP_sm2;1WP_sm2
KEY: TDirectoryFileWP_sm3;1WP_sm3
KEY: TDirectoryFileWP_sm4;1WP_sm4
KEY: TDirectoryFileWP_sm5;1WP_sm5
KEY: TDirectoryFilemcnlo_WP;1mcnlo_WP
KEY: TDirectoryFilejimmy_WP;1jimmy_WP
KEY: TDirectoryFilemcnlo_WJ;1mcnlo_WJ

f.Get('WP/hmc_wmunuPOS asymmetry')
f.Get('WP/hddata_periodB_updscaleqq0706POS asymmetry')

"""
import sys
import ROOT
from SuCanvas import *
mode = int(sys.argv[1]) if len(sys.argv)>1 else 0

def MakeLegend(label='Data and MC'):
    l = ROOT.TLegend(0.55,0.40,0.88,0.58,label,"brNDC")
    #l = ROOT.TLegend(0.55,0.70,0.88,0.88,label,"brNDC")
    l.SetFillStyle(1001)
    l.SetFillColor(10)
    return l

f=ROOT.TFile.Open('asym07072011.root')
f.ls()

if mode==0:
    canvas = SuCanvas()
    canvas.buildDefault('pt_scales')
    folds = ['WP','WP_sc00','WP_sc01','WP_sc10','WP_sc11']
    labs = ['No scale','Scale w/ wide bins','Scale w/ wide bins (expanded sample)','Scale w/ narrow bins','Scale w/ narrow bins (expanded sample)']
    leg = MakeLegend('Color code')
    title='Variation due to pt scale applied to data'
    for i,n in enumerate(folds):
        hmc = f.Get('%s/hmc_wmunuPOS asymmetry'%n)
        hdata = f.Get('%s/hddata_periodB_updscaleqq0706POS asymmetry'%n)
        assert hdata
        hdata.SetTitle(title)
        hdata.SetLineColor(i+1)
        hdata.Draw() if i==0 else hdata.Draw('SAME')
        hdata.GetYaxis().SetRangeUser(0.05,0.30);
        leg.AddEntry(hdata,labs[i],'L')
    leg.Draw('SAME')
    canvas.SaveAs('ASYM_pt_scales')

if mode==1:
    canvas = SuCanvas()
    canvas.buildDefault('pt_smears')
    folds = ['WP_sm0','WP_sm1','WP_sm2','WP_sm3','WP_sm4','WP_sm5']
    labs = ['Unsmeared','NOMINAL','MSLOW','MSUP','IDLOW','IDUP']
    leg = MakeLegend('Color code')
    title='Variation due to pt smearing applied to MC'
    for i,n in enumerate(folds):
        hmc = f.Get('%s/hmc_wmunuPOS asymmetry'%n)
        hdata = f.Get('%s/hddata_periodB_updscaleqq0706POS asymmetry'%n)
        hdata = hmc
        assert hdata
        hdata.SetTitle(title)
        hdata.SetLineColor(i+1)
        hdata.Draw() if i==0 else hdata.Draw('SAME')
        hdata.GetYaxis().SetRangeUser(0.05,0.30);
        leg.AddEntry(hdata,labs[i],'L')
    leg.Draw('SAME')
    canvas.SaveAs('ASYM_pt_smears')

if mode==2:
    canvas = SuCanvas()
    canvas.buildDefault('mc_samples')
    folds = ['WP','mcnlo_WP','jimmy_WP']
    labs = ['106044.PythiaWmunu','106084.McAtNloWplusmunu','107693.AlpgenJimmyWmunuNp3']
    leg = MakeLegend('Color code')
    title='Variation due various MC samples'
    for i,n in enumerate(folds):
        hdata = None
        if i==0:
            hdata = f.Get('%s/hmc_wmunuPOS asymmetry'%n)
        if i==1:
            hdata = f.Get('%s/hmc_wminmunuPOS asymmetry'%n)
        if i==2:
            hdata = f.Get('%s/hmc_jimmy_wmunu_np0POS asymmetry'%n)
        assert hdata
        hdata.SetTitle(title)
        hdata.SetLineColor(i+1)
        hdata.Draw() if i==0 else hdata.Draw('SAME')
        hdata.GetYaxis().SetRangeUser(0.05,0.30);
        leg.AddEntry(hdata,labs[i],'L')
    leg.Draw('SAME')
    canvas.SaveAs('ASYM_samples')
