#!/usr/bin/env python
#import ROOT
import sys,math
from array import array

mode = 1
if len(sys.argv)>1:
    try:
        mode = int(sys.argv[1])
    except:
        mode = 1
print "MODE =",mode

def canvas():
    c = ROOT.TCanvas("c1","c1",200,10,1024,768)
    c.SetFillColor(42);
    c.SetGrid();
    c.GetFrame().SetFillColor(21);
    c.GetFrame().SetBorderSize(12);
    c.cd()
    return c

def graph(*args):
    g = ROOT.TGraphErrors(*args)
    g.SetTitle(title);
    g.SetMarkerColor(4);
    g.SetMarkerStyle(21);
    return g
    
if mode==1:
    #['0.2256', '+/-', '0.0410053', 'RAW:', '383', '+', '242', '=', '625;', '165591', '147.542', 'SCALED:', '2.59587', '+', '1.64021', '=', '4.23608']
    title = "Per-run raw asymmetry (W^{+} - W^{-})/(W^{+} + W^{-}) for data periods G-I"
    f = open('data.dat')
    x = []
    ex = []
    y = []
    ey = []
    i = 0
    for line in f:
        l=line.split()
        #x.append(float(l[9]))
        x.append(i)
        ex.append(0.0)
        y.append(float(l[0]))
        ey.append(float(l[2]))
        i+=1
    import ROOT
    c = canvas()
    g = graph(i,array('d',x),array('d',y),array('d',ex),array('d',ey))
    g.SetMarkerColor(ROOT.kBlack)
    g.Draw("ALP")
    g.GetYaxis().SetRangeUser(0.0,0.5);
    c.SaveAs("_raw_asym.png")

if mode==2:
    #['0.2256', '+/-', '0.0410053', 'RAW:', '383', '+', '242', '=', '625;', '165591', '147.542', 'SCALED:', '2.59587', '+', '1.64021', '=', '4.23608']
    title = "Per-run event yield per nb^{-1} for W(black), W^{+}(red), W^{-}(blue) for data periods G-I"
    f = open('data.dat')
    x = []
    ex = []
    y = [[] for z in xrange(3)]
    ey = [[] for z in xrange(3)]
    i = 0
    for line in f:
        l=line.split()
        #x.append(float(l[9]))
        x.append(i)
        ex.append(0.0)
        y[0].append(float(l[16]))
        y[1].append(float(l[12]))
        y[2].append(float(l[14]))
        l[8] = l[8].replace(';','')
        sf = float(l[8])/float(l[16])
        ey[0].append(math.sqrt(float(l[8]))/sf)
        ey[1].append(math.sqrt(float(l[4]))/sf)
        ey[2].append(math.sqrt(float(l[6]))/sf)
        i+=1
    import ROOT
    c = canvas()
    z=0
    gall = graph(i,array('d',x),array('d',y[z]),array('d',ex),array('d',ey[z]))
    gall.SetMarkerColor(ROOT.kBlack)
    z=1
    gpos = graph(i,array('d',x),array('d',y[z]),array('d',ex),array('d',ey[z]))
    gpos.SetMarkerColor(ROOT.kRed)
    z=2
    gneg = graph(i,array('d',x),array('d',y[z]),array('d',ex),array('d',ey[z]))
    gneg.SetMarkerColor(ROOT.kBlue)
    gall.Draw("ALP")
    gall.GetYaxis().SetRangeUser(0.0,6.0);
    gpos.Draw("PLsames")
    gneg.Draw("PLsames")
    c.SaveAs("_raw_yield.png")
    
