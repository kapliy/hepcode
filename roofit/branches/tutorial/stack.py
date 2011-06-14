#!/usr/bin/env python

import sys,re
from optparse import OptionParser
from load_data import SaveAs
parser = OptionParser()
parser.add_option("-m", "--mode",dest="mode",
                  type="int", default=1,
                  help="Plot mode")
parser.add_option("--type",dest="type",
                  type="int", default=1,
                  help="Type = 2 applies pileup weights")
parser.add_option("--input",dest="input",
                  type="string", default='ROOT/root_all_0612',
                  help="Path to input root file with all histos")
parser.add_option("--var",dest="var",
                  type="string", default='l_eta',
                  help="Variable to plot")
parser.add_option("--bin",dest="bin",
                  type="string", default='100,-2.5,2.5',
                  help="Binning for var")
parser.add_option("--cut",dest="cut",
                  type="string", default='1==1',
                  help="Additional cut to select events")
#329602.0 up to F3 (182519)
#490814.0 up to G3 (183021)
parser.add_option("--lumi",dest="lumi",
                  type="float", default=490814.0,
                  help="Integrated luminosity for data (in nb^-1)")
parser.add_option("--qcd-scale",dest="qcdscale",
                  type="string", default='1.6',
                  help="QCD scale factor")
parser.add_option("-o", "--output",dest="output",
                  type="string", default="",
                  help="Name of output dir for plots")
parser.add_option("--root", default=False,
                  action="store_true",dest="root",
                  help="Save final histo in root file")
parser.add_option('-t',"--tag",dest="tag",
                  type="string", default='plot',
                  help="A tag to append to all output plots")
(opts, args) = parser.parse_args()
mode = opts.mode
print "MODE =",mode

from MC import *
#ROOT.TH1.AddDirectory(ROOT.kFALSE)    # ensure that we own all the histograms
#ROOT.gROOT.SetBatch(1)                # uncomment for interactive usage

SetStyle("AtlasStyle.C")
#ROOT.gStyle.SetOptFit(1111);

#SetStyle()
# Determine plot ordering for MC stacks
def save(c,name):
    for ext in ('png','ps','pdf','C'):
        c.SaveAs("%s.%s"%(name,ext))

po = PlotOrder()
if opts.type==1:
    po.Add(name='t#bar{t}',samples='mc_ttbar',color=ROOT.kGreen)
    po.Add(name='Z#rightarrow#tau#tau',samples='mc_ztautau',color=ROOT.kMagenta)
    po.Add(name='W#rightarrow#tau#nu',samples='mc_wtaunu',color=ROOT.kYellow)
    po.Add(name='Z#rightarrow#mu#mu',samples='mc_zmumu',color=ROOT.kRed)
    po.Add(name='bbmu15X/ccmu15X',samples=['mc_bbmu15x','mc_ccmu15x'],color=ROOT.kCyan)
    po.Add(name='W#rightarrow#mu#nu',samples='mc_wmunu',color=10)
if opts.type==2:
    po.Add(name='t#bar{t}',samples='mc2_ttbar',color=ROOT.kGreen)
    po.Add(name='Z#rightarrow#tau#tau',samples='mc2_ztautau',color=ROOT.kMagenta)
    po.Add(name='W#rightarrow#tau#nu',samples='mc2_wtaunu',color=ROOT.kYellow)
    po.Add(name='Z#rightarrow#mu#mu',samples='mc2_zmumu',color=ROOT.kRed)
    po.Add(name='bbmu15X/ccmu15X',samples=['mc2_bbmu15x','mc2_ccmu15x'],color=ROOT.kCyan)
    po.Add(name='W#rightarrow#mu#nu',samples='mc2_wmunu',color=10)
# Determine which data periods to plot
data = ['data']
#data = [ 'period%s'%s for s in ('B','D','E','F','G1','G2','G3') ]
data = [ 'period%s'%s for s in ('B','D','E','F') ]
mc = list(xflatten(po.mcg))
# All histo files that we need to get
allnames = data + mc

# data and mc histograms
hdata = [[] for i in xrange(2)]
hmc = [{} for i in xrange(2)]
# truth level for wmunu only!
htr = [None for i in xrange(2)]

# load the data inputs
gbg = []
for iname in allnames:
    print 'Loading',iname,
    f = ROOT.TFile.Open('%s/%s/root_%s.root'%(opts.input,iname,iname))
    gbg.append(f)
    nevts = f.Get('dg/nevts').GetEntries()
    print 'with',int(nevts),'events'
    nt = f.Get('dg/st_w_final/ntuple')
    if not nt:
        print 'No events passed cuts in background',iname
        po.Remove(iname)
        continue
    if re.match('mc',iname):
        try:
            hname = 'h%sPOS'%iname
            nt.Draw('%s>>%s(%s)'%(opts.var,hname,opts.bin),'l_q>0 && (%s)'%(opts.cut,),'goff')
            hmc[POS][iname]=ScaleToLumi(ROOT.gDirectory.Get(hname).Clone(),iname,opts.lumi,opts.qcdscale,nevts)
            hmc[POS][iname].Sumw2()
            hname = 'h%sNEG'%iname
            nt.Draw('%s>>%s(%s)'%(opts.var,hname,opts.bin),'l_q<0 && (%s)'%(opts.cut,),'goff')
            hmc[NEG][iname]=ScaleToLumi(ROOT.gDirectory.Get(hname).Clone(),iname,opts.lumi,opts.qcdscale,nevts)
            hmc[NEG][iname].Sumw2()
        except:
            print 'Error: could not find histogram %s in file %s'%(hname,iname)
            po.Remove(iname)
            continue
        if re.search('wmunu',iname):
            # for signal MC, also get truth-level template for efficiency
            nt2 = f.Get('dg/truth/st_truth_reco_w/ntuple')
            hname='htr%sPOS'%iname
            nt2.Draw('%s>>%s(%s)'%(opts.var,hname,opts.bin),'l_q>0 && (%s)'%(opts.cut,),'goff')
            htr[POS] = ScaleToLumi(ROOT.gDirectory.Get(hname).Clone(),iname,opts.lumi,opts.qcdscale,nevts)
            htr[POS].Sumw2()
            hname='htr%sNEG'%iname
            nt2.Draw('%s>>%s(%s)'%(opts.var,hname,opts.bin),'l_q<0 && (%s)'%(opts.cut,),'goff')
            htr[NEG] = ScaleToLumi(ROOT.gDirectory.Get(hname).Clone(),iname,opts.lumi,opts.qcdscale,nevts)
            htr[NEG].Sumw2()
    else: #data:
        hname = 'hd%sPOS'%iname
        nt.Draw('%s>>%s(%s)'%(opts.var,hname,opts.bin),'l_q>0 && (%s)'%(opts.cut,),'goff')
        hdata[POS].append(ROOT.gDirectory.Get(hname).Clone())
        hdata[POS][-1].Sumw2()
        hname = 'hd%sNEG'%iname
        nt.Draw('%s>>%s(%s)'%(opts.var,hname,opts.bin),'l_q<0 && (%s)'%(opts.cut,),'goff')
        hdata[NEG].append(ROOT.gDirectory.Get(hname).Clone())
        hdata[NEG][-1].Sumw2()

def MakeLegend(label='Data and MC'):
    l = ROOT.TLegend(0.55,0.70,0.88,0.88,label,"brNDC")
    l.SetFillStyle(1001)
    l.SetFillColor(10)
    return l

# prepare output holders for summed histos
leg = [MakeLegend() for i in xrange(2)]
leg_eff = [MakeLegend() for i in xrange(2)]
hd = [None for i in xrange(2)]   # data
hd_sig = [None for i in xrange(2)]   # data bg-subtracted
hd_eff = [None for i in xrange(2)]   # data bg-subtracted
hd_sig_eff = [None for i in xrange(2)]   # data bg-subtracted and eff-corrected
hbgs = [ROOT.THStack('eta','eta') for i in xrange(2)]  # all-mc stacks
hbgs_eff = [ROOT.THStack('eta','eta') for i in xrange(2)]  # all-mc stacks, eff-corrected
hbg = [None for i in xrange(2)]  # sum of bg, without wmunu sig
hbg_noqcd = [None for i in xrange(2)]  # sum of bg, without QCD
hbg_qcd = [None for i in xrange(2)]  # sum of QCD bg
hsig = [None for i in xrange(2)] # wmunu sig only
heff = [None for i in xrange(2)] # combined reco efficiency

# create mc histogram stacks and mc & data combined histos
for q in xrange(2):
    for i,inames in enumerate(po.mcg):
        hh = [hmc[q][f].Clone() for f in inames]
        [hh[0].Add(fc) for fc in hh[1:]]  # add J1..J5
        hh[0].SetLineColor(ROOT.kBlack)
        hh[0].SetFillColor(po.mcgc[i])
        hh[0].SetMarkerSize(0)
        # make a sum of all bg
        if not re.search('wmunu',inames[0]):
            if hbg[q]:
                hbg[q].Add(hh[0])
            else:
                hbg[q] = hh[0].Clone()
        else:
            hsig[q]=hh[0].Clone()
        # make BG with and without QCD (for TFractionFitter)
        if not ( re.search('mu15x',inames[0]) or inames[0] in ['mc_J%d'%r for r in range(0,7)] ):
            if hbg_noqcd[q]:
                hbg_noqcd[q].Add(hh[0])
            else:
                hbg_noqcd[q] = hh[0].Clone()
        else:
            hbg_qcd[q]=hh[0].Clone()
        # make a stack of all bg + sig
        hbgs[q].Add(hh[0])  
        leg[q].AddEntry(hh[0],po.mcg_name[i],'F')
    # create data histograms
    hd[q]=hdata[q][0].Clone()
    [hd[q].Add(fc) for fc in hdata[q][1:]]
    leg[q].AddEntry(hd[q],'Data(#int L dt = %.1f pb^{-1})'%(opts.lumi/1000.0),'LP')
    # create efficiency histograms:  sig/truth;  to use: hdata/eff
    heff[q] = hsig[q].Clone()
    heff[q].Divide(htr[q])
    # make a eff-corrected version of stack plot
    for i,inames in enumerate(po.mcg):
        hh = [hmc[q][f].Clone() for f in inames]
        [ih.Divide(heff[q]) for ih in hh] # correct for efficiency
        [hh[0].Add(fc) for fc in hh[1:]]  # add J1..J5
        hh[0].SetLineColor(ROOT.kBlack)
        hh[0].SetFillColor(po.mcgc[i])
        hh[0].SetMarkerSize(0)
        # make a stack of all bg + sig
        hbgs_eff[q].Add(hh[0])
        leg_eff[q].AddEntry(hh[0],po.mcg_name[i],'F')
    # make a bg-subtracted and eff-corrected version of data plot
    hd_sig[q] = hd[q].Clone()
    hd_sig[q].Add(hbg[q],-1)
    hd_sig_eff[q] = hd_sig[q].Clone()
    hd_sig_eff[q].Divide(heff[q])
    hd_eff[q] = hd[q].Clone()
    hd_eff[q].Divide(heff[q])

if mode==1: # total stack histo
    c = canvas()
    #ROOT.gPad.SetLogy(ROOT.kTRUE)
    # mc
    hbgs[0].Draw("H")
    hbgs[0].SetMinimum(0.1)
    maximum = max((hd[0].GetMaximum(),hbgs[0].GetMaximum()))
    hbgs[0].SetMaximum(maximum*1.5)
    #data
    hd[0].SetMarkerSize(1.0)
    hd[0].Draw("Lsame")
    leg[0].Draw("same")
    c.Modified()

if mode==2: # total stack histo (eff-corrected from MC)
    c = canvas()
    #ROOT.gPad.SetLogy(ROOT.kTRUE)
    # mc
    hbgs_eff[0].Draw("H")
    hbgs_eff[0].SetMinimum(0.1)
    maximum = max((hd_eff[0].GetMaximum(),hbgs_eff[0].GetMaximum()))
    hbgs_eff[0].SetMaximum(maximum*1.5)
    #data
    hd_eff[0].SetMarkerSize(1.0)
    hd_eff[0].Draw("Lsame")
    leg_eff[0].Draw("same")
    c.Modified()

if mode==11: # asymmetry (not corrected for eff)
    c = canvas()
    hasym = WAsymmetry(hd_sig[0],hd_sig[1])
    if re.search('phi',hname):
        hasym.Fit("pol0")
    else:
        hasym.Draw()
    hasym.GetYaxis().SetRangeUser(0.0,0.5);
    hasym.GetYaxis().SetTitle('Asymmetry');
    if re.search('mod',hname):
        hasym.GetXaxis().SetRangeUser(0.0,2.4);
    c.Modified()
    if opts.root:
        f = ROOT.TFile.Open('plots.root','UPDATE')
        f.cd()
        hasym.Write('hasym_%s'%hname)
        f.Close()

if mode==12: # asymmetry (not corrected for eff) -vs- truth
    c = canvas()
    hasym = WAsymmetry(hd_sig[0],hd_sig[1])
    hasym.SetMarkerColor(ROOT.kBlue)
    if re.search('phi',hname):
        hasym.Fit("pol0")
    else:
        hasym.Draw()
    htasym = WAsymmetry(htr[0],htr[1])
    htasym.Draw("same")
    hasym.GetYaxis().SetRangeUser(0.0,0.5);
    hasym.GetYaxis().SetTitle('Asymmetry');
    if re.search('mod',hname):
        hasym.GetXaxis().SetRangeUser(0.0,2.4);
    c.Modified()
    if opts.root:
        f = ROOT.TFile.Open('plots.root','UPDATE')
        f.cd()
        hasym.Write('hasym_%s'%hname)
        f.Close()

if mode==13: # asymmetry (fully corrected for eff)
    c = canvas()
    hasym = WAsymmetry(hd_sig_eff[0],hd_sig_eff[1])
    if re.search('phi',hname):
        hasym.Fit("pol0")
    else:
        hasym.Draw()
    hasym.GetYaxis().SetRangeUser(0.0,0.5);
    hasym.GetYaxis().SetTitle('Asymmetry');
    hasym.GetXaxis().SetTitle('#eta_{#mu}');
    if re.search('mod',hname):
        hasym.GetXaxis().SetRangeUser(0.0,2.4);
    # for phi: print fit results
    if re.search('phi',hname) and False:
        hasym.GetYaxis().SetRangeUser(0.0,0.6);
        fs = [v for v in hasym.GetListOfFunctions()]
        fs[0].SetLineColor(hasym.GetLineColor())
        p = ROOT.TPaveText(.2,.70 , (.2+.30),(.70+.20) ,"NDC")
        p.SetTextAlign(11)
        p.SetFillColor(0)
        z=0
        chi2 = fs[z].GetChisquare()
        ndf = fs[z].GetNDF()
        prob = fs[z].GetProb()
        par = fs[z].GetParameter(0)
        p.AddText('Asymmetry fit to a straight line:')
        p.AddText('   asymmetry = %.2f'%par)
        p.AddText('   chi2/ndof = %.1f/%d'%(chi2,ndf))
        p.AddText('   p-value   = %.3f'%prob)
        p.Draw()
    c.Modified()
    if opts.root:
        f = ROOT.TFile.Open('plots.root','UPDATE')
        f.cd()
        hasym.Write('hasym_%s'%hname)
        f.Close()
    if opts.output:
        save(c,'muon_%s'%hname)

if mode==14: # asymmetry (fully corrected for eff) -vs- truth
    c = canvas()
    hasym = WAsymmetry(hd_sig_eff[0],hd_sig_eff[1])
    hasym.SetMarkerColor(ROOT.kBlue)
    if re.search('phi',hname):
        hasym.Fit("pol0")
    else:
        hasym.Draw()
    htasym = WAsymmetry(htr[0],htr[1])
    htasym.Draw("same")
    hasym.GetYaxis().SetRangeUser(0.0,0.5);
    if re.search('mod',hname):
        hasym.GetXaxis().SetRangeUser(0.0,2.4);
    hasym.GetYaxis().SetTitle('Asymmetry');
    hasym.GetXaxis().SetTitle('#eta_{#mu}');
    c.Modified()
    if opts.root:
        f = ROOT.TFile.Open('plots.root','UPDATE')
        f.cd()
        hasym.Write('hasym_%s'%hname)
        f.Close()
    if opts.output:
        save(c,'muon_%s'%hname)

if mode==99: # TFractionFitter for QCD contribution
    c = canvas()
    hbg_noqcd[0].Add(hbg_noqcd[1])
    hbg_qcd[0].Add(hbg_qcd[1])
    hd[0].Add(hd[1])
    # mc
    mc = ROOT.TObjArray(2)
    mc.Add(hbg_qcd[0])
    mc.Add(hbg_noqcd[0])
    fit = ROOT.TFractionFitter(hd[0],mc)
    fit.Constrain(1,0.0,1.0);
    status = fit.Fit();
    if status == 0:
        result = fit.GetPlot();
        hd[0].Draw("Ep");
        result.Draw("same");
    else:
        print 'Fit failed!'
    c.Modified()

SaveAs(c,'%s_%s'%(opts.tag,opts.var),'png')
