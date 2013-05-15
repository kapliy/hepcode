#!/usr/bin/env python

"""
Makes 2D histogram of events NOT triggered by the other trigger

./plot_notin.py dump_MG_dataD_w_POS.dat dump_MU_dataD_w_POS.dat &
./plot_notin.py dump_MG_dataD_w_NEG.dat dump_MU_dataD_w_NEG.dat &
./plot_notin.py dump_MG_dataL_w_POS.dat dump_MU_dataL_w_POS.dat &
./plot_notin.py dump_MG_dataL_w_NEG.dat dump_MU_dataL_w_NEG.dat &
wait

"""

import common
import ROOT
ROOT.gROOT.SetBatch(True)
ROOT.gStyle.SetOptStat(11)
import sys,os

assert len(sys.argv)==3

f1 = open(sys.argv[1])  #MG
f2 = open(sys.argv[2])  #MUID

def load(f):
    """
************************************************************
*    Row   *       run *     event *     l_eta *     l_phi *
0     1    2        3  4       5   6       7   8       9
************************************************************
    """
    m=0
    res = {}
    for lines in f:
        l = lines.split()
        if len(l)!=11: continue
        if l[1]=='Row': continue
        e = l[3]+l[5]
        res[e] = ( float(l[7]) , float(l[9]) )
    return res

nA = 'mu18_MG, not mu18'
nB = 'mu18, not mu18_MG'
hA = ROOT.TH2F('hA',nA,100,-2.5,2.5,100,-3.14,3.14)
hB = ROOT.TH2F('hB',nB,100,-2.5,2.5,100,-3.14,3.14)
res1 = load(f1)
res2 = load(f2)

def hist(h,title):
    h.SetTitle(title)
    h.GetXaxis().SetTitle('#eta')
    h.GetYaxis().SetTitle('#phi')
    h.GetXaxis().SetTitleOffset(2)
    h.GetYaxis().SetTitleOffset(2)


# trigger MG, but not MUID
for e,v in res1.iteritems():
    if e not in res2:
        hA.Fill(res1[e][0], res1[e][1])
hist(hA,nA)
for e,v in res2.iteritems():
    if e not in res1:
        hB.Fill(res2[e][0], res2[e][1])
hist(hB,nB)

cA = ROOT.TCanvas('cA','cA',800,600)
cA.cd()
hA.Draw('LEGO2')
cA.SaveAs('%s__MG_NOT_MUID.png'%(sys.argv[1]))
cA.SaveAs('%s__MG_NOT_MUID.pdf'%(sys.argv[1]))

cB = ROOT.TCanvas('cB','cB',800,600)
cB.cd()
hB.Draw('LEGO2')
cB.SaveAs('%s__MUID_NOT_MG.png'%(sys.argv[1]))
cB.SaveAs('%s__MUID_NOT_MG.pdf'%(sys.argv[1]))
