#!/usr/bin/env python

import sys,re,glob
import ROOT

fnames='/share/ftkdata/antonk/DATA/v1_26/user.ponyisi.mc11_7TeV.106047.PythiaZmumu_no_filter.merge.AOD.e815_s1272_s1274_r2730_r2700*.ntuple.v1_27*/user.ponyisi.*.flatntuple.*.root'
fnames='/share/ftkdata1/boveia/samples/v1_26/user.boveia.mc10_7TeV.106044.PythiaWmunu_no_filter.merge.AOD.e574_s933_s946_r2302_r2300.ntuple.v1_26.110804091002_r1/user.boveia.00*.flatntuple._006*.root*'

fnames='/atlas/uct3/data/users/antonk/NTUPLE/v1_27/user.kapliy.mc11_7TeV.106044.PythiaWmunu_no_filter.merge.AOD.*v1_27*/user.kapliy*flatntuple._*.root*'

t = ROOT.TChain('tree','tree')
itot=0
flb = glob.glob(fnames)
[t.Add('root://uct3-xrd.mwt2.org/%s'%f) for f in flb]
print 'Added',len(flb),'files'

if not re.search('v1_26',flb[0]):
    print 'Will use [lumi_mu_average]...'
    t.Draw('lumi_mu_average>>mu_mc11(21,-0.5,20.5)','','goff')
else:
    print 'Will use [lb]...'
    t.Draw('lb>>mu_mc11(21,-0.5,20.5)','','goff')
hout = ROOT.gDirectory.Get('mu_mc11')

if hout.Integral()==0:
    print 'ERROR: failed to create a histogram'
    sys.exit(1)

# find first non-empty bin - HACK, DO NOT USE
if False:
    ifirst = -1
    for i in xrange(int(hout.GetNbinsX()/2.0),0,-1):
        if hout.GetBinContent(i)==0:
            ifirst=i+1
        if not ifirst==-1:
            print 'Overriding bin',i,'with bin',ifirst
            hout.SetBinContent(i,hout.GetBinContent(ifirst) )

# normalize
hout.Scale(1.0/hout.Integral())
hout.SetEntries(40)
hout.SetTitle('')
hout.SetLineColor(4)
hout.SetLineWidth(4)
hout.SetName('mu_mc11a')

fout = ROOT.TFile.Open('mu_mc11a.root','RECREATE')
fout.cd()
hout.Write()
fout.Close()
