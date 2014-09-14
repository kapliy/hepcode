#!/usr/bin/env python

# adds up histograms in pT slices for Jan/Max reweighting-to-data test
# this is necessary to properly normalize QCD in each slice.

import common
import ROOT

f = ROOT.TFile.Open('jan_wmunu_pt_v6.root','READ')
assert f.IsOpen()
fout = ROOT.TFile.Open('jan_wmunu_pt_v6_out.root','RECREATE')
assert fout.IsOpen()
fout.cd()

for q in ('POS','NEG'):
    for bin in xrange(1,12):
        HS = []
        for sig in ('sig','data_minus_bg'):
            h = f.Get('%s_bin%d_pt2_%s'%(q,bin,sig)).Clone('%s_bin%d_%s'%(q,bin,sig))
            [ h.Add(f.Get('%s_bin%d_pt%d_%s'%(q,bin,pt,sig))) for pt in xrange(3,8) ]
            HS.append( h.Clone('%s_bin%d_data_minus_bg_OVER_sig_%s'%(q,bin,sig)) )
            h.Write(h.GetName())
        HS[0].Scale(HS[0].Integral())
        HS[1].Scale(HS[1].Integral())
        HS[0].Divide(HS[1])
        HS[0].SetName('%s_bin%d_data_minus_bg_OVER_sig'%(q,bin))
        HS[0].Write('%s_bin%d_data_minus_bg_OVER_sig'%(q,bin))
f.Close()
fout.Close()


