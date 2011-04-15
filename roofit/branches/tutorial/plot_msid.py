#!/usr/bin/env python
import sys,os,math,array
import ROOT

def graph_to_len2(hz,maxdata):
    N = hz.GetN()
    print 'Reading graph with',N,'entries'
    v1 = hz.GetX()
    v2 = hz.GetY()
    nmax = min(N,maxdata);
    etaraw = [v1[i] for i in range(0,nmax)]
    resraw = [v2[i] for i in range(0,nmax)]
    eta,res = [],[]
    if True:  # enable a cut on (MS-ID)/CB to cut out outliers?
        for i in range(len(etaraw)):
            if math.fabs(resraw[i])<0.5:
                eta.append(etaraw[i])
                res.append(resraw[i])
    else:
        eta,res=etaraw,resraw
    assert len(eta) == len(res)
    print 'Cut out',len(etaraw)-len(eta),'outliers'
    return len(eta),eta,res
bins=[-2.5,-1.05,1.05,2.5]
nbins=len(bins)-1
xbins=array.array('f',bins)

fn="root_apr14.root"
fn="root_apr14MSID.root"
f=ROOT.TFile.Open(fn)
gPOS="data_20110331_data_20110331.root/dg/dg/st_z_final/graph_peter_exres_POScmb"
gNEG="data_20110331_data_20110331.root/dg/dg/st_z_final/graph_peter_exres_NEGcmb"
grPOS,grNEG=f.Get(gPOS),f.Get(gNEG)

# res=(pt_MS - pt_ID)/pt_combined
NPOS,etaPOS,resPOS=graph_to_len2(grPOS,10000000)
NNEG,etaNEG,resNEG=graph_to_len2(grNEG,10000000)

prPOS,prNEG=ROOT.TProfile('pos','pos',nbins,xbins),ROOT.TProfile('neg','neg',nbins,xbins)
prPOS.SetLineColor(ROOT.kRed)
prNEG.SetLineColor(ROOT.kBlue)
for eta,res in zip(etaPOS,resPOS):
    prPOS.Fill(eta,res)
for eta,res in zip(etaNEG,resNEG):
    prNEG.Fill(eta,res)

c2 = ROOT.TCanvas('c2','c2',800,600)
grPOS.SetMarkerColor(ROOT.kRed)
grNEG.SetMarkerColor(ROOT.kBlue)
grPOS.Draw('A*')
grNEG.Draw('*SAME')
c2.SaveAs('plot0.png')

c = ROOT.TCanvas('c','c',800,600)
prPOS.SetMarkerStyle(34)
prPOS.GetYaxis().SetRangeUser(-0.1,0.1)
prPOS.Draw()
prNEG.Draw('SAME')
c.SaveAs('plot1.png')
