#!/usr/bin/env python
#import ROOT
import sys,math
from array import array

from optparse import OptionParser
parser = OptionParser()
parser.add_option("-i", "--input",dest="input",
                  type="string", default='data.dat',
                  help="Name of the input file to process")
parser.add_option("-m", "--mode",dest="mode",
                  type="int", default=1,
                  help="Plot mode")
parser.add_option("-p", "--period",dest="period",
                  type="string", default='G2-I2',
                  help="Range of data period")

(opts, args) = parser.parse_args()

mode = opts.mode
input = opts.input
period = opts.period
print "MODE =",mode
print "INPUT =",input

def canvas():
    ROOT.gStyle.SetOptFit()
    ROOT.gStyle.SetStatX(0.9)
    ROOT.gStyle.SetStatY(0.9)
    ROOT.gStyle.SetStatW(0.13)
    ROOT.gStyle.SetStatH(0.07)
    ROOT.gStyle.SetStatColor(20)
    ROOT.gStyle.SetTitleFillColor(20)
    c = ROOT.TCanvas("c1","c1",200,10,800,600)
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
    #0.161125 +/- 0.0499114 RAW: 227 + 164 = 391; 165703 87.5158 SCALED: 2.59382 + 1.87395 = 4.46776
    #0.223622 +/- 0.0157906 RAW: 2331 + 1479 = 3810; 165732 902.983 SCALED: 2.58144 + 1.6379 = 4.21935
    title = "Per-run raw asymmetry (W^{+} - W^{-})/(W^{+} + W^{-}) for data periods %s"%period
    f = open(input)
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
        print y[-1],ey[-1]
        i+=1
    assert(len(x)==len(y))
    import ROOT
    c = canvas()
    n=len(x)
    g = graph(n,array('d',x),array('d',y),array('d',ex),array('d',ey))
    g.SetMarkerColor(ROOT.kBlack)
    g.Draw("ALP")
    g.GetYaxis().SetRangeUser(0.0,0.5);
    g.Fit('pol0')
    st = g.GetListOfFunctions().FindObject("stats")
    bname = input.split('.')[0]
    c.SaveAs("_raw_asym_%s.png"%bname)

if mode==2:
    #['0.2256', '+/-', '0.0410053', 'RAW:', '383', '+', '242', '=', '625;', '165591', '147.542', 'SCALED:', '2.59587', '+', '1.64021', '=', '4.23608']
    title = "Per-run event yield per nb^{-1} for W(black), W^{+}(red), W^{-}(blue) for data periods %s"%period
    f = open(input)
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
    n=len(x)
    gall = graph(n,array('d',x),array('d',y[z]),array('d',ex),array('d',ey[z]))
    gall.SetMarkerColor(ROOT.kBlack)
    gall.Fit('pol0')
    st = gall.GetListOfFunctions().FindObject("stats")
    z=1
    gpos = graph(n,array('d',x),array('d',y[z]),array('d',ex),array('d',ey[z]))
    gpos.SetMarkerColor(ROOT.kRed)
    z=2
    gneg = graph(n,array('d',x),array('d',y[z]),array('d',ex),array('d',ey[z]))
    gneg.SetMarkerColor(ROOT.kBlue)
    gall.Draw("ALP")
    gall.GetYaxis().SetRangeUser(0.0,6.0);
    gpos.Draw("PLsames")
    gneg.Draw("PLsames")
    bname = input.split('.')[0]
    c.SaveAs("_raw_yield_%s.png"%bname)

# same as mode=2 (yield), but getting contribution to chi2 from each run
if mode==3:
    #['0.2256', '+/-', '0.0410053', 'RAW:', '383', '+', '242', '=', '625;', '165591', '147.542', 'SCALED:', '2.59587', '+', '1.64021', '=', '4.23608']
    title = "Per-run event yield per nb^{-1} for W(black), W^{+}(red), W^{-}(blue) for data periods %s"%period
    bname = input.split('.')[0]
    f = open(input)
    x = []
    ex = []
    y = [[] for z in xrange(3)]
    ey = [[] for z in xrange(3)]
    runs = []
    i = 0
    for line in f:
        l=line.split()
        #x.append(float(l[9]))
        x.append(i)
        runs.append(int(l[9]))
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
    n=len(x)
    gall = graph(n,array('d',x),array('d',y[z]),array('d',ex),array('d',ey[z]))
    gall.SetMarkerColor(ROOT.kBlack)
    gall.Fit('pol0')
    f = gall.GetFunction('pol0')
    chi2_def = f.GetChisquare()
    ndf_def = f.GetNDF()
    y_def =  f.GetParameter(0)
    print chi2_def, y_def
    chi2s = []
    ys = []
    for i in range(len(runs)):
        tmpy = [yy for yy in y[z]]
        tmpy[i] = y_def  # remove contribution
        tmpey = [yy for yy in ey[z]]
        tmpey[i] = 0.00000001
        g = graph(n,array('d',x),array('d',tmpy),array('d',ex),array('d',tmpey))
        g.Fit('pol0')
        f = g.GetFunction('pol0')
        chi2 = f.GetChisquare()
        ys.append(f.GetParameter(0))
        chi2s.append(chi2)
    print 'chi2 | ndf | chi2_prob | W event yield     (total for period %s)'%period
    print '%.3f %.3f %.3f%% %.3f'%(chi2_def,ndf_def,100.0*ROOT.TMath.Prob(chi2_def,ndf_def),y_def)
    checksum = 0.0
    print 'run_index | run | chi2 contribution from each run'
    for i,chi2 in enumerate(chi2s):
        run = runs[i]
        print '%d %d %.3f'%(i,run,chi2_def-chi2)
        checksum += chi2_def-chi2
    print 'Sanity check: sum of contributions = %.3f'%checksum
    if True:
        st = gall.GetListOfFunctions().FindObject("stats")
        gall.Draw("ALP")
        gall.GetYaxis().SetRangeUser(0.0,6.0);
        c.SaveAs("_raw_yield_%s.png"%bname)
