#!/usr/bin/env python
import common
import ROOT
ROOT.gROOT.SetBatch(True)

import os,sys

"""
# Adrian's script to plot purity
"""

ROOT.gStyle.SetPaintTextFormat(".2f")
ROOT.gStyle.SetPalette(1)
ROOT.gStyle.SetOptStat(0)

# from config.sh
bgqcd = 4
z = os.popen('source config.sh && echo $input','r')
fin_dir = z.readline().strip()
z.close()

OUTDIR='/home/antonk/SupportingDocument/Wmunu/figures/purity'
num = "purityNum"
purityDenomName = "purityDenom"
stabilityDenomName = "stabilityDenom"

f = None
for sign in ["pos", "neg"]:
    print 'Processing:',sign
    if f: f.Close()
    filename = os.path.join(fin_dir,'unfold_histo_powheg_pythia_%s.root'%('plus' if sign=='pos' else 'min'))

    f = ROOT.TFile(filename, "READ")
    assert f.IsOpen() and not f.IsZombie()

    indir = "%s/Nominal/abseta_lpt"%('POS' if sign=='pos' else 'NEG')

    purityHist = f.Get(indir+"/"+num).Clone("purityNum")
    stabilityHist = f.Get(indir+"/"+num).Clone("stabilityNum")

    purityDenom = f.Get(indir+"/"+purityDenomName).Clone("purityDenom")
    stabilityDenom = f.Get(indir+"/"+stabilityDenomName).Clone("stabilityDenom")

    purityHist.Divide(purityDenom)
    stabilityHist.Divide(stabilityDenom)

    purityHist.Scale(100)
    stabilityHist.Scale(100)

    purityHist.SetTitle("")
    purityHist.GetXaxis().SetTitle("|#eta|")
    purityHist.GetYaxis().SetTitle("p_{T} [GeV]")
    purityHist.GetYaxis().SetTitleOffset(1.3)

    stabilityHist.SetTitle("")
    stabilityHist.GetXaxis().SetTitle("|#eta|")
    stabilityHist.GetYaxis().SetTitle("p_{T} [GeV]")
    stabilityHist.GetYaxis().SetTitleOffset(1.3)

    c = ROOT.TCanvas("canvas", "canvas", 600, 600)
    purityHist.Draw("COLZTEXT")
    c.Print(OUTDIR+"/purity_2D_{0}.pdf".format(sign))
    c.Print("DELME_purity_2D_{0}.png".format(sign))

    c2 = ROOT.TCanvas("canvas", "canvas", 600, 600)
    stabilityHist.Draw("COLZTEXT")
    c2.Print(OUTDIR+"/stability_2D_{0}.pdf".format(sign))
    c2.Print("DELME_stability_2D_{0}.png".format(sign))
