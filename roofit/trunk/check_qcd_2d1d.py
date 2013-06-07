#!/usr/bin/env python

# Compare QCD for 1D vs SUM_over_pT{2D}
# Note: focus on pt>25 GeV case

import sys
import common
import ROOT

absetabins = ['0.00','0.21','0.42','0.63','0.84','1.05','1.37','1.52','1.74','1.95','2.18','2.40']

q='POS'
if len(sys.argv)>=2:
    q = sys.argv[1]
print 'Charge:',q

f1d = ROOT.TFile.Open('OUT_05182013_fstat.v30.abseta.1D.pt25.root')
assert f1d.IsOpen()
f2d = ROOT.TFile.Open('OUT_05182013_fstat.v30.abseta.2D.pt20.root')
assert f2d.IsOpen()


qcd1d = f1d.Get('%s/qcd_Nominal'%q)
sig1d = f1d.Get('%s/sig_Nominal'%q)
assert qcd1d
qcd2d = f2d.Get('%s/qcd_Nominal'%q)
assert qcd2d

for ieta in xrange( 1 , qcd1d.GetNbinsX()+1 ):
    eta1,eta2 = absetabins[ieta-1:ieta+1]
    vpt = qcd2d.ProjectionY('proj_on_eta%d'%ieta,ieta,ieta)
    df = (qcd1d.GetBinContent(ieta)-vpt.Integral(2,vpt.GetNbinsX()+1))
    print eta1+'-'+eta2+' : 2D='+('+'.join([ '%d'%vpt.GetBinContent(i) for i in xrange(2,vpt.GetNbinsX()+1) ]))+'=%d'%vpt.Integral(2,vpt.GetNbinsX()+1)+' 1D=%d'%qcd1d.GetBinContent(ieta),' DIFF=%.1f%%'%(df / qcd1d.GetBinContent(ieta) * 100.0),' DIFF/NSIG=%.1f%%'%(df / sig1d.GetBinContent(ieta) * 100.0)
