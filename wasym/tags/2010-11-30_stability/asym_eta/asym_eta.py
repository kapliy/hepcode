from MC import *
SetStyle("AtlasStyle.C")
def MakeLegend(label='Data and MC'):
    x = 0.6
    y = 0.23
    l = ROOT.TLegend(x,y,x+0.88-0.55,y+0.18,label,"brNDC")
    l.SetFillStyle(1001)
    l.SetFillColor(10)
    return l

f = ROOT.TFile.Open('plots.root')
f.cd()
f.ls()

heta = f.Get('hasym_asym_eta').Clone()
heta.SetLineColor(ROOT.kRed)
hetam = f.Get('hasym_asym_etam').Clone()
hetam.SetLineColor(ROOT.kBlue)

c = canvas()
heta.Draw()
hetam.Draw('SAME')
heta.GetXaxis().SetRangeUser(0.1,2.4);
heta.GetYaxis().SetRangeUser(0.0,0.4);
heta.GetXaxis().SetTitle('#eta_{#mu}')
heta.GetYaxis().SetTitle('Asymmetry')

l = MakeLegend('')
l.AddEntry(heta,'#eta > 0','L')
l.AddEntry(hetam,'#eta < 0','L')
l.Draw()

if True:
    for ext in ('png','ps','pdf','C'):
        c.SaveAs( "muon_eta_asym.%s"%ext )

