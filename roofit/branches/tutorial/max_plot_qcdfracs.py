#!/usr/bin/env python

# A small helper script that plots / projects QCD fractions

import sys,os
import common
import ROOT
common.SetStyle("AtlasStyle.C")
ROOT.gROOT.SetBatch(ROOT.kTRUE)

fname = 'OUT_11022012_ALL.v1.abseta.2D.root'  #bgqcd=3
fname = 'OUT_11022012_ALL.v2.abseta.2D.root'  #bgqcd=4
n_frac = 'POS/qcd_Fraction_MCAverage'
n_err  = 'POS/qcd_FractionStackError_MCAverage'

if len(sys.argv)>2:
    fname = sys.argv[1]
print 'Opening:',fname
fin = ROOT.TFile.Open(fname,'READ')
assert fin.IsOpen()
h_frac = fin.Get(n_frac)
assert h_frac
h_err = fin.Get(n_err)

h = h_frac
mode = 'FRAC'
if len(sys.argv)>1:
    h = h_err
    mode = 'ERR'
    print 'Plotting errors, as opposed to fractions'

def sys(obj,dored=True):
    """ set style for uncertainties """
    if not obj: return
    obj.SetFillColor( ROOT.kYellow );
    obj.SetFillStyle( 3001 );
    obj.SetMarkerSize( 0 );
    if dored:
      obj.SetLineColor( ROOT.kRed );
      obj.SetMarkerColor( ROOT.kRed );
    obj.SetLineStyle( 2 );
    obj.SetLineWidth( 2 );

# plots vs eta
ceta = ROOT.TCanvas('ceta','ceta',800,600)
ceta.cd()
leg = ROOT.TLegend(0.55,0.70,0.88,0.88,'QCD %s vs |#eta| in p_{T} bins'%('fraction' if mode=='FRAC' else 'uncertainty'),"brNDC")
px1 = h.ProjectionX('_px1',1,1,'e')
leg.AddEntry(px1,'p_{T} = 20..25','LP')
px2 = h.ProjectionX('_px2',2,2,'e')
px2.SetLineColor(ROOT.kRed)
#px2.SetFillColor(ROOT.kRed)
px2.SetMarkerColor(ROOT.kRed)
leg.AddEntry(px2,'p_{T} = 25..30','LP')
px5 = h.ProjectionX('_px5',5,5,'e')
px5.SetLineColor(ROOT.kBlue)
#px5.SetFillColor(ROOT.kBlue)
px5.SetMarkerColor(ROOT.kBlue)
leg.AddEntry(px5,'p_{T} = 40..45','LP')
px7 = h.ProjectionX('_px7',7,7,'e')
px7.SetLineColor(ROOT.kGreen)
#px7.SetFillColor(ROOT.kGreen)
px7.SetMarkerColor(ROOT.kGreen)
leg.AddEntry(px7,'p_{T} = 50..120','LP')
px1.Draw()
px1.GetYaxis().SetRangeUser(-0.05,1.5*max(o.GetMaximum() for o in (px1,px2,px5,px7)))
px2.Draw('A SAME')
px5.Draw('A SAME')
px7.Draw('A SAME')
leg.SetFillColor(10)
leg.Draw('SAME')
ceta.Modified()
ceta.Update()
ceta.SaveAs('PNG_qcd%s_vs_eta.png'%mode)


# plots vs pt
cpt = ROOT.TCanvas('cpt','cpt',800,600)
cpt.cd()
leg2 = ROOT.TLegend(0.55,0.70,0.88,0.88,'QCD %s vs muon p_{T} in |#eta| bins'%('fraction' if mode=='FRAC' else 'uncertainty'),"brNDC")
py1 = h.ProjectionY('_py1',1,1,'e')
leg2.AddEntry(py1,'|#eta| = 0.00-0.21','LP')
py5 = h.ProjectionY('_py5',5,5,'e')
py5.SetLineColor(ROOT.kBlue)
#py5.SetFillColor(ROOT.kBlue)
py5.SetMarkerColor(ROOT.kBlue)
leg2.AddEntry(py5,'|#eta| = 0.84-1.05','LP')
py11 = h.ProjectionY('_py11',11,11,'e')
py11.SetLineColor(ROOT.kGreen)
#py11.SetFillColor(ROOT.kGreen)
py11.SetMarkerColor(ROOT.kGreen)
leg2.AddEntry(py11,'|#eta| = 2.18-2.4','LP')
py1.Draw()
py1.GetYaxis().SetRangeUser(-0.05,1.2*max(o.GetMaximum() for o in (py1,py5,py11)))
py5.Draw('A SAME')
py11.Draw('A SAME')
leg2.SetFillColor(10)
leg2.Draw('SAME')
cpt.Modified()
cpt.Update()
cpt.SaveAs('PNG_qcd%s_vs_pt.png'%mode)
cpt.SaveAs('PNG_qcd%s_vs_pt.pdf'%mode)
