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
    bname = input.split('.')[0]
    import ROOT
    f = ROOT.TFile.Open(input)
    sfile = 'periodI.root'
    g=f.GetDirectory(sfile).GetDirectory('dg').GetDirectory('dg').GetDirectory('st_w_final')
    pos = g.GetDirectory('POS')
    neg = g.GetDirectory('NEG')
    heta=[];
    heta.append(pos.Get('lepton_eta').Clone())
    heta.append(neg.Get('lepton_eta').Clone())
    heta.append(pos.Get('lepton_modeta').Clone())
    heta.append(neg.Get('lepton_modeta').Clone())
    [h.Sumw2() for h in heta]
    [h.Rebin(2) for h in heta]
    h1 = WAsymmetry(heta[0],heta[1])
    h1.SetMarkerColor(ROOT.kRed)
    h2 = WAsymmetry(heta[2],heta[3])
    h2.SetMarkerColor(ROOT.kBlue)
    c = canvas()
    h1.Draw()
    h2.Draw("SAME")
    #c.SaveAs("_raw_asym_eta%s.png"%bname)
