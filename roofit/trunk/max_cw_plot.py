#!/usr/bin/env python

"""
Checking that the powheg_pythia -> powheg_herwig reweighting works.
IDEA: C_W did not improve. Let's check if normalized distributions got better?
A quick script to plot truth and/or reco-level spectra for:
Powheg+Pythia , Powheg+Pythia (reweighted) , Powheg+Herwig.
./max_cw_plot.py -v l_pt && ./max_cw_plot.py -v l_pt --truth && ./max_cw_plot.py -v met && ./max_cw_plot.py -v met --truth && echo OK

for tag in nu nonint flat; do
   ./max_cw_plot.py -v l_pt -t $tag &
   ./max_cw_plot.py -v l_pt --truth -t $tag &
   ./max_cw_plot.py -v met -t $tag &
   ./max_cw_plot.py -v met --truth -t $tag &
done
wait

"""

import os,sys,glob,re
import array
ptbins_l = [0,5,10,15,20,25,30,35,40,45,50,70,90,120,200]
ptbins_l = range(0,70,2)
ptbins_l = range(0,150,2)
ptbins = array.array('f',ptbins_l)

from optparse import OptionParser
parser = OptionParser()
INPUTS = []
INPUTS.append( '/share/t3data3/antonk/ana/ana_v29I_1011012_PYtoHW_nowzptw_nometutil_stacoCB_all' )
INPUTS.append( '/share/t3data3/antonk/ana/ana_v29I_1011012_PYtoHW_nowzptw_nometutil_trumet_stacoCB_all' )
parser.add_option("-i", "--input",dest="input",
                  type="string", default=INPUTS[1],
                  help="Core input directory containing output from TrigFTKAna_trunk")
parser.add_option("-v", "--var",dest="var",
                  type="string", default='met',
                  help="Variable to plot: met or l_pt")
parser.add_option("-c", "--cut",dest="cut",
                  type="string", default='mask*mcw*puw*vxw*ls1w*ls2w*effw*isow*trigw',
                  help="Ntuple cut, usually a subset of: mask*mcw*puw*wzptw*vxw*ls1w*ls2w*effw*isow*trigw")
parser.add_option("--truth", default=False,
                  action="store_true",dest="truth",
                  help="If set to true, plot at truth (Born) level")
parser.add_option("--rawmet", default=False,
                  action="store_true",dest="rawmet",
                  help="At reco-level, use Rawmet category - i.e., without met recalculation")
parser.add_option("--integral", default=False,
                  action="store_true",dest="integral",
                  help="Normalize samples to their integral?")
parser.add_option('-t',"--tag",dest="tag",
                  type="string", default='nu',
                  help="A tag to append to all output plots")
(opts, args) = parser.parse_args()

def FixEdgeBins(hist,v=1.0):
    for binX in xrange(0,hist.GetNbinsX()+2):
        for binY in xrange(0,hist.GetNbinsY()+2):
            for binZ in xrange(0,hist.GetNbinsZ()+2):
                globalBin = hist.GetBin(binX,binY,binZ)
                if hist.IsBinUnderflow(globalBin) or hist.IsBinOverflow(globalBin):
                    hist.SetBinContent(globalBin,v)
    return True

dbase=opts.input
fns = []
if opts.input == INPUTS[0]:
    fns.append( dbase +'/'+'mc_powheg_herwig_wminmunu/root_mc_powheg_herwig_wminmunu.root' )
    fns.append( dbase+'/'+'mc_powheg_pythia_wminmunu_toherwig2dfine/root_mc_powheg_pythia_wminmunu_toherwig2dfine.root' )
else:
    assert opts.tag in ('nu','nonint','flat')
    fns.append( dbase +'/'+'mc_powheg_herwig_wminmunu/root_mc_powheg_herwig_wminmunu.root' )
    fns.append( dbase+'/'+'mc_powheg_pythia_wminmunu_toherwig2d%s/root_mc_powheg_pythia_wminmunu_toherwig2d%s.root'%(opts.tag,opts.tag) )
    

USE_TRUTH=opts.truth
b=opts.cut
var=opts.var

MAXENTRIES = 10000
MAXENTRIES = 100000000
COLORS = [ 1,2,3,4,6,8]
STYLES = [ 20,21,22,23,29,33,34 ]
SIZES = [1.5,1.4,1.3,1.2,1.1,1.0]

import common
import ROOT
ROOT.gROOT.SetBatch(True)
ROOT.TH1.SetDefaultSumw2(ROOT.kTRUE)

fs = [ROOT.TFile.Open(fn,'READ') for fn in fns]
Ts = [] # truth trees
Rs = [] # reco  trees
hT = [] # histos
for f in fs:
    assert f.IsOpen()
    Ts.append( f.Get('dg/truth/st_w_final/ntuple') ); assert Ts[-1]
    if opts.rawmet:
        Rs.append( f.Get('dg/Rawmet/st_w_final/ntuple') ); assert Rs[-1]
    else:
        Rs.append( f.Get('dg/Nominal/st_w_final/ntuple') ); assert Rs[-1]

# truth histos: PowhegPythia, PowhegPythia_rw, PowhegHerwig
cT = ROOT.TCanvas('cT','cT',800,800)
cT.Divide(1,2)
cT.cd(1)
leg = ROOT.TLegend(0.55,0.70,0.88,0.88,'Muon pT',"brNDC")

def cr(name,T,cut = '(mcw*puw*wzptw*vxw*ls1w*ls2w*effw*isow*trigw)*(mask)',idx=0):
    h = ROOT.TH1D('h'+name,'h'+name,len(ptbins)-1,ptbins)
    print 'Making ',name
    T.Draw('%s>>h%s'%(var,name),cut,'goff',MAXENTRIES)
    leg.AddEntry(h,name,'LP')
    FixEdgeBins(h,0.0)
    h.SetLineColor(COLORS[idx])
    h.SetMarkerColor(COLORS[idx])
    h.SetMarkerStyle(STYLES[idx])
    h.SetMarkerSize(SIZES[idx])
    if not opts.integral:
        INT_HERWIG = 2955907.0
        INT_PYTHIA = 16744582.0
        if opts.input == INPUTS[1]: # hack: federated access problems in INPUTS[1]
            INT_HERWIG = 2998607.0
            INT_PYTHIA = 11990723.0
        x = INT_HERWIG if re.search('Herwig',name) else INT_PYTHIA
        print 'STORED  :',name,x
        h.Scale(1.0/x)
    else:
        print 'INTEGRAL:',name,h.Integral()
        h.Scale(1.0/h.Integral())
    h.Draw() if idx==0 else h.Draw('A SAME')
    return h

Zs = Ts if USE_TRUTH else Rs
hT.append( cr('PowhegHerwig',   Zs[0],b+'',0) )
hT.append( cr('PowhegPythia',   Zs[1],b+'',1) )
hT.append( cr('PowhegPythia_rw',Zs[1],b+'*phw',2) )
hT[0].GetYaxis().SetRangeUser(0.0,1.5*max(o.GetMaximum() for o in hT))
leg.Draw('SAME')

# RATIOS
cT.cd(2)
def refline(h):
    refLine = h.Clone()
    refLine.Reset()
    for ibin in range (1,refLine.GetNbinsX()+1):
      refLine.SetBinContent( ibin , 1.0 )
    refLine.SetLineColor( ROOT.kAzure-7 );
    refLine.SetFillColor( 10 );
    refLine.SetMinimum( 0.9 )
    refLine.SetMaximum( 1.1 )
    refLine.GetYaxis().SetNdivisions( 406 );
    refLine.GetYaxis().SetTitle( "ratio" );
    refLine.SetLineWidth( 3 );
    refLine.SetLineStyle( ROOT.kDashed );
    return refLine
ratios = []
ref = refline(hT[0])
ref.Draw( "HIST" );
for h in hT[1:]:
    ratio = h.Clone()
    ratio.Divide(hT[0])
    ratio.Draw('A SAME')
    ratios.append(ratio)

cT.Modified()
cT.Update()
save_mask = '_mask' if re.search('mask',b) else ''
save_reco = '_true' if opts.truth else '_reco'
cT.SaveAs('CW_plot_%s%s%s_%s.png'%(opts.var,save_reco,save_mask,opts.tag))
