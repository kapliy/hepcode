#!/usr/bin/env python

"""
examining how to draw a TGraph that acts like a line
"""
import sys,os,re
import common
import ROOT

c = ROOT.TCanvas('c','c',800,600)
c.cd()

h = ROOT.TH1F('h','h',100,-1,1)
h.FillRandom('gaus',100)

h.Draw()
c.Update()

gr = ROOT.TGraph(2)
gr.SetPoint(0,0.4,c.GetFrame().GetY1())
gr.SetPoint(1,0.4,c.GetFrame().GetY2())
gr.SetLineColor(ROOT.kRed)
gr.SetLineStyle(2)
gr.Draw('L')

c.SaveAs('DELETEME.pdf')
