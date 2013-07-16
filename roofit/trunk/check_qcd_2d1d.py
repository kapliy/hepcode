#!/usr/bin/env python

# Compare QCD for 1D vs SUM_over_pT{2D}
# Note: focus on pt>25 GeV case

import sys,os
import common
import ROOT

absetabins = ['0.00','0.21','0.42','0.63','0.84','1.05','1.37','1.52','1.74','1.95','2.18','2.40']

q='POS'
if len(sys.argv)>=2:
    q = sys.argv[1]
print 'Charge:',q
qword=r'$\mu^{+}$' if q=='POS' else r'$\mu^{-}$'

z = os.popen('source config.sh && echo $OUTDB','r')
outbase='OUT_05182013_fstat.v30'  # first iteration
outbase = z.readline().strip()

f1d = ROOT.TFile.Open('%s.abseta.1D.pt25.root'%outbase)
assert f1d.IsOpen()
f2d = ROOT.TFile.Open('%s.abseta.2D.pt20.root'%outbase)
assert f2d.IsOpen()

HEAD = r"""
\begin{table}
  \begin{center}
      \begin{tabular}{lrrrr}
      \hline
      \hline
      $\eta$ slice & 1D fit & $\sum$ 2D fits & $(1D-\sum{2D})/1D$ & $(1D-\sum{2D})/N_{signal}$ \\
"""

TAIL = r"""
      \hline
      \hline
  \end{tabular}
\caption{Comparison of data-driven multi-jet counts from single-differential measurement and the sum of double-differential measurements in each $p_T$ bin: %s}
\label{tab:Wmunu:qcd_1d2d_check_%s}
\end{center}
\end{table}
"""

sys='Nominal'

qcd1d = f1d.Get('%s/qcd_%s'%(q,sys))
sig1d = f1d.Get('%s/sig_%s'%(q,sys))
assert qcd1d
qcd2d = f2d.Get('%s/qcd_%s'%(q,sys))
assert qcd2d

print HEAD
for ieta in xrange( 1 , qcd1d.GetNbinsX()+1 ):
    eta1,eta2 = absetabins[ieta-1:ieta+1]
    vpt = qcd2d.ProjectionY('proj_on_eta%d'%ieta,ieta,ieta)
    etaword = eta1+'-'+eta2
    v1d = qcd1d.GetBinContent(ieta)
    v2d = vpt.Integral(2,vpt.GetNbinsX()+1)
    diff = (v1d - v2d)
    diffR1d = diff / v1d
    diffRSig = diff / sig1d.GetBinContent(ieta)
    print etaword +r' & %d'%v1d + r' & %d'%v2d + r' & %.2f'%(diffR1d*100.0) + r'\%% & %.2f'%(diffRSig*100.0) + r'\% \\'
    #print etaword +r' & %d'%v1d + r' & %d'%v2d + r' & %.2f'%(diff*100.0) + r'\\% & %.2f'%(diffSig*100.0) + r'\% \\'
    #print etaword+' : 2D='+('+'.join([ '%d'%vpt.GetBinContent(i) for i in xrange(2,vpt.GetNbinsX()+1) ]))+'=%d'%vpt.Integral(2,vpt.GetNbinsX()+1)+' 1D=%d'%qcd1d.GetBinContent(ieta),' DIFF=%.1f%%'%(diff / qcd1d.GetBinContent(ieta) * 100.0),' DIFF/NSIG=%.1f%%'%(diff / sig1d.GetBinContent(ieta) * 100.0)
print TAIL%(qword,q)
