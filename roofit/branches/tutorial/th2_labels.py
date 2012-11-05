#!/usr/bin/env python

""" Prints a 2D migration matrix with percentage labels in each row """
import common
common.SetStyle("AtlasStyle.C")

import ROOT
import binning

if __name__=='__main__':
    fname = '/share/t3data3/antonk/ana/ana_v29I_10212012_unfold_AFII_stacoCB_all/unfold_powheg_pythia_min.root'
    hname = 'HGLB_MuonScaleKUp'
    fname = '/home/antonk/TrigFTKAna/results/ana_wasym/wasymmetry29I_mc_powheg_pythia_wminmunu.dat/20121030.unfold.root'
    fname = '/share/t3data3/antonk/ana/ana_v29I_10292012_unfold_newTrigWZPTnoPol_wmin1p37_stacoCB_all/unfold_powheg_pythia.root'
    hname = 'HGLB_MuonScaleCUp_bin37'
    f = ROOT.TFile.Open(fname)
    assert f.IsOpen()
    h = f.Get(hname)
    assert h
    hh = h.Clone('rows')
    hh.Reset()
    if True:
        bins = binning.ptbins[:]
        # go column by column and print totals
        for xpt in bins: # loop over original pt bins
            print xpt,'\t',
            ysum = 0
            for ypt in bins:
                gbin = h.FindBin(xpt+0.1,ypt+0.1)
                ysum += h.GetBinContent(gbin)
            print int(ysum),'\t',
            for ypt in bins:
                gbin = h.FindBin(xpt+0.1,ypt+0.1)
                # convert to percentage!
                v = h.GetBinContent(gbin)*100.0/ysum
                hh.SetBinContent(gbin,v)
                print ('%.3f'%v if v<10.0 else '%.2f'%v),
            print ''
    c = ROOT.TCanvas('c','c',800,600)
    c.cd()
    hh.Draw("colz")
    c.SaveAs('plot.png')
    
