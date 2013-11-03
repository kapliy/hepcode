#!/usr/bin/env python

import common
import ROOT
ROOT.gROOT.SetBatch(1)
ROOT.gStyle.SetPalette(1)
ROOT.gStyle.SetOptStat(0)
ROOT.gStyle.SetOptFit(111)

f = ROOT.TFile.Open('histsR.root')

t = '#mu^{+}: #frac{Data(MG)}{MC(MG)} / #frac{Data(Muid)}{MC(Muid)}'
common.dump_plot( [f.Get('MUMG_stack_lpt_POS'),f.Get('MUID_stack_lpt_POS')] , 'dratio_data_POS', do_ratio=True, do_fit=False, titles=[t,t]  )
t = '#mu^{-}: #frac{Data(MG)}{MC(MG)} / #frac{Data(Muid)}{MC(Muid)}'
common.dump_plot( [f.Get('MUMG_stack_lpt_NEG'),f.Get('MUID_stack_lpt_NEG')] , 'dratio_data_NEG', do_ratio=True, do_fit=False, titles=[t,t]  )


