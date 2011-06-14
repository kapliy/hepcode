#!/usr/bin/env python

mu13 = range(165703,165576+1)
mu13_tight = range(167607,167844+1)

import sys,math
from array import array
import ROOT

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
SetStyle("AtlasStyle.C")
#SetStyle()


def canvas_old():
    ROOT.gStyle.SetOptFit()
    ROOT.gStyle.SetStatX(0.9)
    ROOT.gStyle.SetStatY(0.9)
    ROOT.gStyle.SetStatW(0.13)
    ROOT.gStyle.SetStatH(0.07)
    ROOT.gStyle.SetStatColor(20)
    ROOT.gStyle.SetTitleFillColor(20)
    c = ROOT.TCanvas("c1","c1",200,10,800,500)
    c.SetFillColor(42);
    c.SetGrid();
    c.GetFrame().SetFillColor(21);
    c.GetFrame().SetBorderSize(12);
    c.cd()
    return c

def canvas():
    c = ROOT.TCanvas("c1","c1",200,10,800,500)
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
    runs = []
    i = 0
    for line in f:
        l=line.split()
        #x.append(float(l[9]))
        runs.append(int(l[9]))
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
    #g.Draw("ALP")
    #g.GetYaxis().SetRangeUser(0.0,0.5);
    #g.Fit('pol0')
    # total histo
    hall = ROOT.TH1F('hall','hall',n,0,n)
    hall.SetLineColor(ROOT.kBlack)
    hall.SetMarkerColor(ROOT.kBlack)
    rtight=None
    for i in range(n):
        if runs[i] in mu13_tight and not rtight:
            rtight=i
        hall.SetBinContent(i+1,y[i])
        hall.SetBinError(i+1,ey[i])

    # draw
    hall.Fit('pol0','','')
    #hall.Fit('pol0','+','',rtight,n)
    fs = [v for v in hall.GetListOfFunctions()]
    fs[0].SetLineColor(hall.GetLineColor())
    #fs[1].SetLineColor(hall.GetLineColor())
    #range
    ymin = 0.0
    ymax = 0.6
    hall.GetYaxis().SetRangeUser(ymin,ymax);
    hall.SetLabelSize(0.04)
    if False:
        # period-trigger separator
        lsep = ROOT.TLine(rtight,ymin,rtight,ymax)
        lsep.Draw()
    # axis labels
    hall.GetYaxis().SetTitle('Asymmetry: (W^{+} - W^{-})/(W^{+} + W^{-})');
    hall.GetXaxis().SetTitle('Run Number');

    # set pave text
    p = ROOT.TPaveText(.2,.70 , (.2+.30),(.70+.20) ,"NDC")
    p.SetTextAlign(11)
    p.SetFillColor(0)
    z=0
    chi2 = fs[z].GetChisquare()
    ndf = fs[z].GetNDF()
    prob = fs[z].GetProb()
    par = fs[z].GetParameter(0)
    #p.AddText('Period G2-I1 (EF_mu13_MG): yield=%.1f chi2/ndof=%.1f/%d p-value=%.2f'%(par,chi2,ndf,prob))
    p.AddText('Uncorrected asymmetry for periods G2-I2:')
    p.AddText('   asymmetry = %.2f'%par)
    p.AddText('   chi2/ndof = %.1f/%d'%(chi2,ndf))
    p.AddText('   p-value   = %.2f'%prob)
    p.Draw()

    # label x axis
    if True:
        ax = hall.GetXaxis()
        for i in range(len(runs)):
            try:
                ax.SetBinLabel(i+1,str(runs[i]))
                pass
            except:
                print 'ERROR: bin',i
    hall.LabelsOption('v')
    # save output
    def save():
        bname = input.split('.')[0]
        for ext in ('png','ps','pdf','C'):
            c.SaveAs("muon_run_asym_%s.%s"%(bname,ext))

# W yield as a function of run - with verbose tpavetext summary printout
# done separately for I1-I2 tight trigger period
if mode==2:
    #['0.2256', '+/-', '0.0410053', 'RAW:', '383', '+', '242', '=', '625;', '165591', '147.542', 'SCALED:', '2.59587', '+', '1.64021', '=', '4.23608']
    title = "Per-run event yield per nb^{-1} for W(black), W^{+}(red), W^{-}(blue) for data periods %s"%period
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
        ex.append(0.0)
        runs.append(int(l[9]))
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
    # total graph
    z=0
    n=len(x)
    print 'Loaded',n,'runs!'
    gall = graph(n,array('d',x),array('d',y[z]),array('d',ex),array('d',ey[z]))
    gall.SetMarkerColor(ROOT.kBlack)
    # total histo
    hall = ROOT.TH1F('hall','hall',n,0,n)
    hall.SetLineColor(ROOT.kBlack)
    hall.SetMarkerColor(ROOT.kBlack)
    rtight=None
    for i in range(n):
        if runs[i] in mu13_tight and not rtight:
            rtight=i
        hall.SetBinContent(i+1,y[z][i])
        hall.SetBinError(i+1,ey[z][i])
    # w+ graph
    z=1
    gpos = graph(n,array('d',x),array('d',y[z]),array('d',ex),array('d',ey[z]))
    gpos.SetMarkerColor(ROOT.kRed)
    # w+ histo
    hpos = ROOT.TH1F('hpos','hpos',n,0,n)
    hpos.SetLineColor(ROOT.kRed)
    hpos.SetMarkerColor(ROOT.kRed)
    for i in range(n):
        hpos.SetBinContent(i+1,y[z][i])
        hpos.SetBinError(i+1,ey[z][i])
    # w- graph
    z=2
    gneg = graph(n,array('d',x),array('d',y[z]),array('d',ex),array('d',ey[z]))
    gneg.SetMarkerColor(ROOT.kBlue)
    # w- histo
    hneg = ROOT.TH1F('hneg','hneg',n,0,n)
    hneg.SetLineColor(ROOT.kBlue)
    hneg.SetMarkerColor(ROOT.kBlue)
    for i in range(n):
        hneg.SetBinContent(i+1,y[z][i])
        hneg.SetBinError(i+1,ey[z][i])
    
    # draw
    hall.Fit('pol0','','',0,rtight)
    hall.Fit('pol0','+','',rtight,n)
    fs = [v for v in hall.GetListOfFunctions()]
    fs[0].SetLineColor(hall.GetLineColor())
    fs[1].SetLineColor(hall.GetLineColor())
    #range
    ymin = 3.0
    ymax = 8.0
    hall.GetYaxis().SetRangeUser(ymin,ymax);
    hall.SetLabelSize(0.04)
    # period-trigger separator
    lsep = ROOT.TLine(rtight,ymin,rtight,ymax)
    lsep.Draw()
    # axis labels
    hall.GetYaxis().SetTitle('W event yield per nb^{-1}');
    hall.GetXaxis().SetTitle('Run Number');

    # set pave text
    p = ROOT.TPaveText(.2,.50 , (.2+.30),(.50+.40) ,"NDC")
    p.SetTextAlign(11)
    p.SetFillColor(0)
    z=0
    chi2 = fs[z].GetChisquare()
    ndf = fs[z].GetNDF()
    prob = fs[z].GetProb()
    par = fs[z].GetParameter(0)
    #p.AddText('Period G2-I1 (EF_mu13_MG): yield=%.1f chi2/ndof=%.1f/%d p-value=%.2f'%(par,chi2,ndf,prob))
    p.AddText('Period G2-I1 (EF_mu13_MG):')
    p.AddText('   yield     = %.2f'%par)
    p.AddText('   chi2/ndof = %.1f/%d'%(chi2,ndf))
    p.AddText('   p-value   = %.2f'%prob)
    z=1
    chi2 = fs[z].GetChisquare()
    ndf = fs[z].GetNDF()
    prob = fs[z].GetProb()
    par = fs[z].GetParameter(0)
    #p.AddText('Period I1-I2 (EF_mu13_MG_tight): yield=%.1f chi2/ndof=%.1f/%d, p-value=%.2f'%(par,chi2,ndf,prob))
    p.AddText('Period I1-I2 (EF_mu13_MG_tight):')
    p.AddText('   yield     = %.2f'%par)
    p.AddText('   chi2/ndof = %.1f/%d'%(chi2,ndf))
    p.AddText('   p-value   = %.2f'%prob)
    p.Draw()

    if False:
        hall.GetYaxis().SetRangeUser(0.0,6.0);
        hpos.Fit('pol0','',"same")
        st = hpos.GetListOfFunctions().FindObject("pol0")
        st.SetLineColor(hpos.GetLineColor())
        hneg.Fit('pol0','',"same")
        st = hneg.GetListOfFunctions().FindObject("pol0")
        st.SetLineColor(hneg.GetLineColor())
    #gall.Draw("ALP")
    #gall.GetYaxis().SetRangeUser(0.0,6.0);
    #gpos.Draw("PLsames")
    #gneg.Draw("PLsames")

    # label x axis
    if True:
        ax = hall.GetXaxis()
        for i in range(len(runs)):
            try:
                ax.SetBinLabel(i+1,str(runs[i]))
                pass
            except:
                print 'ERROR: bin',i
    hall.LabelsOption('v')
    # save output
    def save():
        bname = input.split('.')[0]
        for ext in ('png','ps','pdf','C'):
            c.SaveAs("muon_run_yield_%s.%s"%(bname,ext))

# W, W+, W- yields as a function of run number
if mode==22:
    #['0.2256', '+/-', '0.0410053', 'RAW:', '383', '+', '242', '=', '625;', '165591', '147.542', 'SCALED:', '2.59587', '+', '1.64021', '=', '4.23608']
    title = "Per-run event yield per nb^{-1} for W(black), W^{+}(red), W^{-}(blue) for data periods %s"%period
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
        ex.append(0.0)
        runs.append(int(l[9]))
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
    # total graph
    z=0
    n=len(x)
    print 'Loaded',n,'runs!'
    gall = graph(n,array('d',x),array('d',y[z]),array('d',ex),array('d',ey[z]))
    gall.SetMarkerColor(ROOT.kBlack)
    # total histo
    hall = ROOT.TH1F('hall','hall',n,0,n)
    hall.SetLineColor(ROOT.kBlack)
    hall.SetMarkerColor(ROOT.kBlack)
    rtight=None
    for i in range(n):
        if runs[i] in mu13_tight and not rtight:
            rtight=i
        hall.SetBinContent(i+1,y[z][i])
        hall.SetBinError(i+1,ey[z][i])
    # w+ graph
    z=1
    gpos = graph(n,array('d',x),array('d',y[z]),array('d',ex),array('d',ey[z]))
    gpos.SetMarkerColor(ROOT.kRed)
    # w+ histo
    hpos = ROOT.TH1F('hpos','hpos',n,0,n)
    hpos.SetLineColor(ROOT.kRed)
    hpos.SetMarkerColor(ROOT.kRed)
    for i in range(n):
        hpos.SetBinContent(i+1,y[z][i])
        hpos.SetBinError(i+1,ey[z][i])
    # w- graph
    z=2
    gneg = graph(n,array('d',x),array('d',y[z]),array('d',ex),array('d',ey[z]))
    gneg.SetMarkerColor(ROOT.kBlue)
    # w- histo
    hneg = ROOT.TH1F('hneg','hneg',n,0,n)
    hneg.SetLineColor(ROOT.kBlue)
    hneg.SetMarkerColor(ROOT.kBlue)
    for i in range(n):
        hneg.SetBinContent(i+1,y[z][i])
        hneg.SetBinError(i+1,ey[z][i])
    
    # draw
    hall.Fit('pol0','','',0,rtight)
    st = hall.GetListOfFunctions().FindObject("pol0")
    st.SetLineColor(hall.GetLineColor())
    hall.Fit('pol0','+','',rtight,n)
    #st = hall.GetListOfFunctions().FindObject("pol0")
    #st.SetLineColor(hall.GetLineColor())
    ymin=0.0
    ymax=6.0
    hall.GetYaxis().SetRangeUser(ymin,ymax)
    hall.SetLabelSize(0.04)
    hpos.Fit('pol0','',"same",0,rtight)
    hpos.Fit('pol0','+',"same",rtight,n)
    [st.SetLineColor(hpos.GetLineColor()) for st in hpos.GetListOfFunctions()]
    hneg.Fit('pol0','',"same",0,rtight)
    hneg.Fit('pol0','+',"same",rtight,n)
    [st.SetLineColor(hneg.GetLineColor()) for st in hneg.GetListOfFunctions()]

    # period-trigger separator
    lsep = ROOT.TLine(rtight,ymin,rtight,ymax)
    lsep.Draw()
    # axis labels
    hall.GetYaxis().SetTitle('W event yield per nb^{-1}');
    hall.GetXaxis().SetTitle('Run Number');

    # label x axis
    if True:
        ax = hall.GetXaxis()
        for i in range(len(runs)):
            try:
                ax.SetBinLabel(i+1,str(runs[i]))
                pass
            except:
                print 'ERROR: bin',i
    hall.LabelsOption('v')
    # save output
    def save():
        bname = input.split('.')[0]
        for ext in ('png','ps','pdf','C'):
            c.SaveAs("muon_run_yield_%s_bycharge.%s"%(bname,ext))

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
    print 'Loaded',n,'runs!'
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
