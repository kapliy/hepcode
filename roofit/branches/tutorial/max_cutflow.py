#!/usr/bin/env python

"""
# INSTRUCTIONS ON HOW TO PREPARE A COMPLETE DATA CUTFLOW FILE
bdir=/share/t3data3/antonk/ana/ana_v29I_12042012_edboard_stacoCB_all
cd ${bdir}
/bin/ls ${bdir}/data_period*/data*root > ALLDATA.list
~/TrigFTKAna/plot/dgadd -f 1 -d ${bdir}/ALLDATA.root -l ${bdir}/ALLDATA.list

"""
import sys

is_data = 0
charge = 0
if len(sys.argv)>=2:
    is_data = int(sys.argv[1])
if len(sys.argv)>=3:
    charge = int( sys.argv[2])

datadir = '/share/t3data3/antonk/ana/ana_v29I_12042012_edboard_stacoCB_all'
data = None
qword = 'plus' if charge==0 else 'min'
if is_data:
    data = datadir + '/' + 'ALLDATA.root'
else:
    data = datadir + '/' + 'mc_powheg_pythia_w%smunu/mc_powheg_pythia_w%smunu.root'%(qword,qword)
    
print 'SETTINGS: ',is_data,charge,data

import os,subprocess
import binning
CUTMAP = binning.CUTMAP

cmd = '/home/antonk/TrigFTKAna/plot/dgplot -q -b cutflow.dg %s'%data

CUTS = []
CUTS.append('passes_e_grl')
CUTS.append('passes_e_lar_cleaning')
CUTS.append('passes_e_primary_vertex')
CUTS.append('passes_e_trigger')
CUTS.append('passes_w_qual')
CUTS.append('passes_w_pt')
CUTS.append('passes_w_eta')
CUTS.append('passes_w_iso')
CUTS.append('passes_w_nmuons')
CUTS.append('passes_w_metclean')
CUTS.append('passes_w_larhole')
CUTS.append('passes_w_met')
CUTS.append('passes_w_mt')
CUTSQ = []
CUTSQ.append('passes_w_pos')
CUTSQ.append('passes_w_neg')
    

HEADER = """
\\begin{table}
\\begin{center}
\\begin{tabular}{lrrr}
\hline
\hline
Cut & Events & Cut Efficiency & Cumulative Efficiency \\\\
\hline"""
PH= ' \phantom{0} '
TAILER = """\hline
\hline
\end{tabular}
\caption{%s}
\label{tab:Wmunu:%s}
\end{center}
\end{table}
"""
tab_mc = 'Cutflow for $W^{%s} \\rightarrow \mu^{%s} \\nu$ events in the nominal Monte Carlo signal sample~(Powheg+Pythia). Pileup, vertex position, and $p_T^{W}$  weights are applied. The number of selected events after the application of efficiency scale factors is shown in the last row.' % ('+' if charge==0 else '-','+' if charge==0 else '-')
tab_data = 'Cutflow for \Wmn\ candidate events in data.'

TOT = None
def eff(v,bef,aft):
    """ efficiency line """    
    res = ''
    assert v in CUTMAP
    mcur = CUTMAP[v]
    mmax = max(CUTMAP.values(), key=len)
    xtra = len(mmax) - len(mcur)
    res += mcur + ' '*xtra + '  & '
    res += '%.1f'%aft
    if TOT:
        assert bef; assert aft
        effcur = 100.0*aft/bef
        efftot = 100.0*aft/TOT
        ph1 = '' if effcur>=100 else (PH if effcur>=10 else PH+PH)
        ph2 = '' if efftot>=100 else (PH if efftot>=10 else PH+PH)
        res += ' & ' + '%s%.2f\%%'%(ph1,effcur) + ' & ' + '%s%.2f\%%'%(ph2,efftot)
    else:
        res += ' & - & - '
    res += ' \\\\'
    return res
def tot(v,aft,xtra=''):
    """ totals line """
    res = ''
    assert v in CUTMAP
    mcur = CUTMAP[v]+xtra
    mmax = max(CUTMAP.values(), key=len)
    xtra = len(mmax) - len(mcur)
    if xtra<0: xtra = 1
    res += mcur + ' '*xtra + '  & '
    res += '%.1f'%aft
    res += ' \\\\'
    return res

DATAU = []
DATAV = []
DATAW = []
p = os.popen(cmd,"r")
while True:
    line = p.readline()
    if not line: break
    words = line.split()
    if len(words)>0 and words[0]=='u':
        DATAU.append(words)
    if len(words)>0 and words[0]=='v':
        DATAV.append(words)
    if len(words)>0 and words[0]=='w':
        DATAW.append(words)

NUM,DEN,NAME = 6,5,7
def find(cut,DATA):
    """ ['u', '17', '0.5988', '0.0001', '0.0001', '17772223.000', '10642556.000', 'passes_w_pos'] """
    for line in DATA:
        if line[NAME]==cut:
            return line
    return None

PREV = None
print HEADER
for i,cut in enumerate(CUTS):
    L = find(cut,DATAU if is_data else DATAV)
    print eff( L[NAME], float(L[DEN]), float(L[NUM]) )
    PREV = L[NUM]
    if i==0:
        TOT = float(L[NUM])

if is_data:
    print '\hline'
    L = find(CUTSQ[0],DATAU)
    print tot( L[NAME], float(L[NUM]) )
    L = find(CUTSQ[1],DATAU)
    print tot( L[NAME], float(L[NUM]) )
else:
    print '\hline'
    L = find(CUTS[-1],DATAW)
    print tot( L[NAME], float(L[NUM]) , xtra=' (Efficiency Corrected)')

print TAILER%(tab_data if is_data else tab_mc,'cutflow_data' if is_data else 'cutflow_mc_%s'%qword)
