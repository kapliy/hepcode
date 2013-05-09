#!/usr/bin/env python

# creates latex cutflow tables.
# see examples in max_cutflow.sh

import sys,os

datadir = None
is_data = 0
charge = 0
pt = 20
if len(sys.argv)>=2:
    datadir = sys.argv[1]
if len(sys.argv)>=3:
    is_data = int(sys.argv[2])
if len(sys.argv)>=4:
    charge = sys.argv[3]
    assert charge in ('POS','NEG')
    charge = 0 if charge=='POS' else 1
if len(sys.argv)>=5:
    pt = int(sys.argv[4])
    assert pt in (20,25)

assert os.path.isdir(datadir),'ERROR: cannot locate directory %s'%datadir

data = None
dataR = None
qword = 'plus' if charge==0 else 'min'
if is_data:
    data = datadir + '/' + 'ALLDATA.root'
    dataR = data
else:
    data = datadir + '/' + 'mc_powheg_pythia_w%smunu/mc_powheg_pythia_w%smunu.root'%(qword,qword)
    dataR = datadir + '/' + 'mc_powheg_pythia_w%smunu/root_mc_powheg_pythia_w%smunu.root'%(qword,qword)
    
print >>sys.stderr, 'SETTINGS: ',is_data,charge,data

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
\\begin{tabular}{lrrr}
\hline
\hline
Cut & Events & Cut Efficiency & Cumulative Efficiency \\\\
\hline"""
PH= ' \phantom{0} '
TAILER = """\hline
\hline
\end{tabular}
"""

TOT = None
def eff(v,bef,aft):
    """ efficiency line """    
    res = ''
    assert v in CUTMAP
    mcur = CUTMAP[v]
    if v=='passes_w_pt' and pt==25:
        mcur = CUTMAP['passes_w_pt_25']
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
    if v=='passes_w_pt' and pt==25:
        mcur = CUTMAP['passes_w_pt_25']
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

import common
import ROOT
ff = None
if os.path.isfile(data):
    ff = ROOT.TFile.Open(data,'READ')
else:
    assert os.path.isfile(dataR), 'ERROR: failed to load either:\n%s \nor:\n%s'%(data,dataR)
    ff = ROOT.TFile.Open(dataR,'READ')
if ff.Get('dg'): # parsing dgplot
    print 'Opened as dgplot'
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
else: # parsing root output
    assert ff.Get('wcut')
    assert ff.Get('Nominal/wcut')
    print 'Opened as ROOT'
    def make_xx(t,DATA):
        """ t is one of: u,v,w """
        xx = sorted([ z.GetName().replace('_num','') for z in ff.Get('wcut').GetListOfKeys() if z.GetName()[0]==t and z.GetName()[-3:]=='num'])
        xy = sorted([ z.GetName().replace('_num','') for z in ff.Get('Nominal/wcut').GetListOfKeys() if z.GetName()[0]==t and z.GetName()[-3:]=='num'])
        for x in (xx+xy):
            sfold = 'wcut' if (x in xx) else 'Nominal/wcut'
            assert ff.Get(sfold).Get(x+'_den')
            assert ff.Get(sfold).Get(x+'_num')
            words = [t, 'NONE', 'NONE', 'NONE', 'NONE',
                     ff.Get(sfold).Get(x+'_den').GetBinContent(1),
                     ff.Get(sfold).Get(x+'_num').GetBinContent(1),
                     x[4:] ]
            DATA.append( words )
        return xx
    make_xx('u',DATAU)
    make_xx('v',DATAV)
    make_xx('w',DATAW)
    pass
ff.Close()

NUM,DEN,NAME = 6,5,7
def find(cut,DATA):
    """ ['u', '17', '0.5988', '0.0001', '0.0001', '17772223.000', '10642556.000', 'passes_w_pos'] """
    for line in DATA:
        if line[NAME]==cut:
            return line
    return None

PREV = None
print HEADER
if is_data:
    NALL = 440214121.0
    L = find(CUTS[0],DATAU if is_data else DATAV)
    print eff( 'all_events',NALL,NALL)
    TOT = 440214121.0
    print eff( 'passes_ntuple',440214121.0,float(L[DEN]) )
for i,cut in enumerate(CUTS):
    L = find(cut,DATAU if is_data else DATAV)
    print eff( L[NAME], float(L[DEN]), float(L[NUM]) )
    PREV = L[NUM]
    if not TOT:
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

print TAILER
