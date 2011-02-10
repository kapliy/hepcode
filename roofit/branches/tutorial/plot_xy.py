#!/usr/bin/env python

#for f in cmb_mc_BB_*_results.rtxt; do n=`echo $f | awk 'BEGIN{FS="_"}{print $4}'`; err=`cat $f | awk '{print $5}'`; echo $n $err; done | sort -n

import sys,array
import ROOT


def SetStyle(styleMacro=''):
    """ Global style settings for ROOT """
    if styleMacro=='':
        ROOT.gStyle.SetCanvasColor(0);
        ROOT.gStyle.SetCanvasBorderMode(0);
        ROOT.gStyle.SetPadColor(0);
        ROOT.gStyle.SetPadBorderMode(0);
        ROOT.gStyle.SetStatColor(0);
        
        #ROOT.gStyle.SetOptStat(111111);
        ROOT.gStyle.SetOptStat(0);
        ROOT.gStyle.SetOptFit(1111);
        ROOT.gStyle.SetHistFillColor(0);
        ROOT.gStyle.SetMarkerStyle(20);
        ROOT.gStyle.SetMarkerSize(.4);
        ROOT.gStyle.SetHistLineWidth(2);
        ROOT.gStyle.SetErrorX(0);
        
        ROOT.gStyle.SetTitleStyle(0);
        
        ROOT.gStyle.SetStatBorderSize(1);
        ROOT.gStyle.SetFrameFillColor(10);
        ROOT.gStyle.SetTitleFillColor(0);
        ROOT.gROOT.ForceStyle()
    else:
        ROOT.gROOT.LoadMacro(styleMacro);
        ROOT.SetAtlasStyle()
SetStyle()

print len(sys.argv),sys.argv

fname  = sys.argv[1] if len(sys.argv)>1 else sys.exit(1)
xtitle = sys.argv[2] if len(sys.argv)>2 else 'x'
ytitle = sys.argv[3] if len(sys.argv)>3 else 'y'

f=open(fname,'r')

x=[]
y=[]
for line in f:
    l = line.split()
    x.append(int(l[0]))
    y.append(float(l[1]))
    
h = ROOT.TGraph(len(x),array.array('f',x),array.array('f',y))

c = ROOT.TCanvas('c','c',800,600); c.cd()
h.Draw('ALP*')
h.SetTitle('%s vs %s'%(xtitle,ytitle))
h.GetXaxis().SetTitle(xtitle)
h.GetYaxis().SetTitle(ytitle)
c.SaveAs('%s.png'%fname)

