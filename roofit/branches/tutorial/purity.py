import ROOT

"""
# Adrian's script to plot purity
"""

ROOT.gStyle.SetPaintTextFormat(".2f")
ROOT.gStyle.SetPalette(1)
ROOT.gStyle.SetOptStat(0)

filename = "/Users/adrian/data/AnalysisOutput/hists_19.1.2013_All/Nominal/sign_combined/hists_mc11c_v4_powheg_pythia_Wenu__NOMINAL.root"

num = "purityNum"
purityDenomName = "purityDenom"
stabilityDenomName = "stabilityDenom"

f = ROOT.TFile(filename, "READ")

for sign in ["pos", "neg"]:
    indir = "UnfoldingHists_{0}_vsEta_vsPt".format(sign)

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
    c.Print("plots/note_plots/purity_2D_{0}.pdf".format(sign))

    c2 = ROOT.TCanvas("canvas", "canvas", 600, 600)
    stabilityHist.Draw("COLZTEXT")
    c2.Print("plots/note_plots/stability_2D_{0}.pdf".format(sign))
