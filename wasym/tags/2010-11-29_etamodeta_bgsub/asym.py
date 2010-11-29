#!/usr/bin/env python
#import ROOT
import sys,math
from array import array

from optparse import OptionParser
parser = OptionParser()
parser.add_option("-i", "--input",dest="input",
                  type="string", default='root_periodAI.root',
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

def WRatio(hplus,hminus,title,label='ratio'):
    """ Ratio of W+ to W- workhorse function
    Alternatively, can be used to calculate efficiency
    """
    hratio = hplus.Clone(hplus.GetName()+"_"+label)
    hratio.Divide(hminus)
    hratio.SetTitle(title)
    return hratio

def WAsymmetry(hplus,hminus,title='asymmetry'):
    """ (W+ - W-)/(W+ + W-) workhorse function """
    hsum = hplus.Clone(hplus.GetName()+" asymmetry")
    hsum.Add(hminus,-1.0)
    hdiff = hplus.Clone(hplus.GetName()+" asymmetry diff")
    hdiff.Add(hminus)
    hsum.Divide(hdiff)
    hsum.SetTitle(title)
    return hsum

if mode==1:
    sfile = 'periodI.root'
    bname = sfile.split('.')[0]
    import ROOT
    f = ROOT.TFile.Open(input)
    g=f.GetDirectory(sfile).GetDirectory('dg').GetDirectory('dg').GetDirectory('st_w_final')
    pos = g.GetDirectory('POS')
    neg = g.GetDirectory('NEG')
    heta=[]
    heta.append(pos.Get('lepton_eta').Clone())
    heta.append(neg.Get('lepton_eta').Clone())
    heta.append(pos.Get('lepton_modeta').Clone())  # |eta|, same binning
    heta.append(neg.Get('lepton_modeta').Clone())
    heta.append(pos.Get('lepton_modetam').Clone())  # -|eta|, same binning
    heta.append(neg.Get('lepton_modetam').Clone())
    # make a pos-only and neg-only part of each histo
    if True:
        h = heta[0].Clone()
        for i in range(0,h.GetNbinsX()/2+1):
            h.SetBinContent(i,0)
        heta.append(h)
        h = heta[1].Clone()
        for i in range(0,h.GetNbinsX()/2+1):
            h.SetBinContent(i,0)
        heta.append(h)
    [h.Sumw2() for h in heta]
    [h.Rebin(2) for h in heta]
    h1 = WAsymmetry(heta[6],heta[7])
    h1.SetMarkerColor(ROOT.kRed)
    h2 = WAsymmetry(heta[2],heta[3])
    h2.SetMarkerColor(ROOT.kBlue)
    c = canvas()
    h1.Draw()
    h2.Draw("SAME")
    kt = h1.KolmogorovTest(h2)
    print kt
    h1.SetTitle('Uncorrected asymmetry: Red=eta, Blue=|eta|. Kolmogorov p=%.2f'%kt)
    c.SaveAs("_raw_asym_etapos%s.png"%bname)

if mode==2:
    sfile = 'periodI.root'
    bname = sfile.split('.')[0]
    import ROOT
    f = ROOT.TFile.Open(input)
    g=f.GetDirectory(sfile).GetDirectory('dg').GetDirectory('dg').GetDirectory('st_w_final')
    pos = g.GetDirectory('POS')
    neg = g.GetDirectory('NEG')
    heta=[]
    heta.append(pos.Get('lepton_eta').Clone())
    heta.append(neg.Get('lepton_eta').Clone())
    heta.append(pos.Get('lepton_modeta').Clone())  # |eta|, same binning
    heta.append(neg.Get('lepton_modeta').Clone())
    heta.append(pos.Get('lepton_modetam').Clone())  # -|eta|, same binning
    heta.append(neg.Get('lepton_modetam').Clone())
    # make a pos-only and neg-only part of each histo
    if True:
        h = heta[0].Clone()
        for i in range(h.GetNbinsX()/2,h.GetNbinsX()):
            h.SetBinContent(i,0)
        if False:  # check: that blown-up errors make test compatible!
            for i in range(0,h.GetNbinsX()/2+1):
                h.SetBinError(i,h.GetBinError(i)*2)
        heta.append(h)
        h = heta[1].Clone()
        for i in range(h.GetNbinsX()/2,h.GetNbinsX()):
            h.SetBinContent(i,0)
        heta.append(h)
    [h.Sumw2() for h in heta]
    [h.Rebin(2) for h in heta]
    h1 = WAsymmetry(heta[6],heta[7])
    h1.SetMarkerColor(ROOT.kRed)
    h2 = WAsymmetry(heta[4],heta[5])
    h2.SetMarkerColor(ROOT.kBlue)
    c = canvas()
    h1.Draw()
    h2.Draw("SAME")
    kt = h1.KolmogorovTest(h2)
    print kt
    h1.SetTitle('Uncorrected asymmetry: Red=eta, Blue=|eta|. Kolmogorov p=%.2f'%kt)
    c.SaveAs("_raw_asym_etaneg%s.png"%bname)
