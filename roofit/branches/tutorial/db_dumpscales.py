#!/usr/bin/env python

"""
Quick script to dump scales
"""
try:
    import psyco
    psyco.full()
except ImportError:
    pass

import os,sys,math
stdout = sys.stdout
import antondb
from optparse import OptionParser

dbname = 'out1023L7'

from optparse import OptionParser
parser = OptionParser()
parser.add_option("-m", "--mode",dest="mode",
                  type="int", default=2,
                  help="Plot mode")
parser.add_option("-a", "--algo",dest="algo",
                  type="int", default=0,
                  help="Algorith: 0=STACO, 1=MuID")
parser.add_option("-d", "--dets",dest="dets",
                  type="int", default=0,
                  help="For printing syst tables: 0=ALL,1=cmb,2=exms,3=id")
parser.add_option("-r", "--release",dest="rel",
                  type="int", default=17,
                  help="Athena release: 16 or 17")
parser.add_option("--antondb",dest="antondb",
                  type="string", default=dbname,
                  help="Tag for antondb output container")
(opts, args) = parser.parse_args()
mode = opts.mode
algo = opts.algo
rel = opts.rel
dbname = opts.antondb

a = antondb.antondb(dbname)
a.load()

os.system('mkdir -p latex/')
algos = { 0 : 'STACO', 1 : 'MuID' }
fname = '/not/exists'
latex_head = r"""
\documentclass[a4paper]{article}
% Shorthand for \phantom to use in tables
\newcommand{\pho}{\phantom{0}}
\begin{document}
"""
latex_tail = r"""
\end{document}"""

dets = ['cmb','exms','id']
dets_map = {'cmb' : 'Combined muons', 'id' : 'ID muons', 'exms' : 'MS muons'}
regs_map = {'AA' : 'Endcap-A', 'BB' : 'Barrel', 'CC' : 'Endcap-C', 'Bcc' : 'Barrel C-side ($-1.05 < \eta < 0.0$)', 'Baa' : 'Barrel A-side ($0.0 < \eta < 1.05$)', 'FWC' : r'Endcap C-side ($-2.4 < \eta < -2.0$)', 'FWA' : r'Endcap A-side ($2.0 < \eta < 2.4$)', 'MWC' : r'Endcap C-side ($-2.0 < \eta < -1.05$)', 'MWA' : r'Endcap A-side ($1.05 < \eta < 2.0$)'}
regsR = range(0,25)
for bn in regsR:
    NN = len(regsR); bwidth = 5.0/NN
    regs_map['E%dE'%bn] = r'$%.1f < \eta < %.1f$'%(-2.5 + 1.0*bn*bwidth , -2.5 + (bn+1.0)*bwidth )

def scales(R,eR,mz,emz,mz0):
    sqrt=math.sqrt
    km = mz/(mz0*sqrt(R))
    kp = mz*sqrt(R)/(mz0)
    ekm = sqrt( emz**2 * 1/(mz0*sqrt(R))**2 + eR**2 * (mz/mz0)**2 / (4 * R**3) )
    ekp = sqrt( emz**2 * (sqrt(R)/mz0)**2 + eR**2 * (mz/mz0)**2 / (4 * R) )
    ekm1,ekm2 = sqrt(emz**2 * 1/(mz0*sqrt(R))**2),sqrt(eR**2 * (mz/mz0)**2 / (4 * R**3))
    ekp1,ekp2 = sqrt(emz**2 * (sqrt(R)/mz0)**2), sqrt(eR**2 * (mz/mz0)**2 / (4 * R))
    t = {}
    t['mz0']=mz0
    t['mz']=mz
    t['emz']=emz
    t['R']=R*100.0
    t['eR']=eR*100.0
    t['kp']=kp*100.0
    t['ekp']=ekp*100.0
    t['km']=km*100.0
    t['ekm']=ekm*100.0
    t['ekp1']=ekp1*100.0
    t['ekp2']=ekp2*100.0
    t['ekm1']=ekm1*100.0
    t['ekm2']=ekm2*100.0
    return t

def load_all(regs=[], pattern_R0='/default/%s/%s/R0',pattern_Z='/default/%s/%s/Z/gaus0'):
    res = {}
    for det in dets:
        res[det] = {}
        for reg in regs:
            res[det][reg] = {}
            aR0 = a.data[pattern_R0%(det,reg)];  assert aR0
            if True:  # False if z peak fit was not run
                aZ = a.data[pattern_Z%(det,reg)];  assert aZ
                res[det][reg]['ksf'] = scales(aR0['ksf'],aR0['chie'],aZ['data_mz'],aZ['data_emz'],aZ['mc_mz'])
                res[det][reg]['chif'] = scales(aR0['chif'],aR0['chie'],aZ['data_mz'],aZ['data_emz'],aZ['mc_mz'])
                res[det][reg]['ksp'] = scales(aR0['ksp'],aR0['chie'],aZ['data_mz'],aZ['data_emz'],aZ['mc_mz'])
                res[det][reg]['chip'] = scales(aR0['chip'],aR0['chie'],aZ['data_mz'],aZ['data_emz'],aZ['mc_mz'])
            else:
                res[det][reg]['ksf'] = scales(aR0['ksf'],aR0['chie'],1.0,0.1,1.01)
                res[det][reg]['chif'] = scales(aR0['chif'],aR0['chie'],1.0,0.1,1.01)
                res[det][reg]['ksp'] = scales(aR0['ksp'],aR0['chie'],1.0,0.1,1.01)
                res[det][reg]['chip'] = scales(aR0['chip'],aR0['chie'],1.0,0.1,1.01)
    return res

def max_pair(s):
    s1,s2=s[0],s[1]
    assert len(s1)==len(s2),'Error: wrong array sizes'
    return [max(s) for s in zip([math.fabs(a) for a in s1],[math.fabs(b) for b in s2])]

def sumsq(s):
    grps = zip(*s)
    res = []
    for grp in grps:
        sq=0
        for i in grp:
            sq += i**2
        res.append(math.sqrt(sq))
    return res

def print_cpp(db,regs):
    print '========================='
    print 'C++ array:'
    print '========================='
    i=0; n=len(dets)*len(regs)
    for det in dets:
        for reg in regs:
            print '%.2f , %.2f'%(default[det][reg]['ksf']['kp'],default[det][reg]['ksf']['km']),
            print ' , ' if i!=n-1 else ''
            i+=1

def latex_R(db,regs,ptype='ksf'):
    print latex_head
    for det in dets:
        print r"""
\begin{table}[htb]
\begin{center}
\begin{tabular}{|c|c|c|}
\hline
Region    &   $R_{0}$     & $\delta R_{0}$   \\
\hline\hline"""
        print r'\multicolumn{3}{|c|}{'+'%s'%dets_map[det]+r'}     \\ \hline'
        for reg in regs:
            s=db[det][reg][ptype]
            #Endcap-A  &   $100.1 \pm 0.4\%$  &  $    100.0 \pm 0.4\%$     \\ \hline
            print '%s'%regs_map[reg]+r'  &  ' + '%.2f'%(s['R'])+r'\%  &   '+'%.2f'%(s['eR'])+r'\%     \\ \hline'
        print r'\hline'
        print r"\end{tabular}"
        print r"\caption{%s: release-%d relative scales}"%(algos[opts.algo],opts.rel)
        print r"\label{tab:" + 'relative%srel%d'%(algos[opts.algo],opts.rel) + r'}'
        print r"\end{center}"
        print r"\end{table}"
    print latex_tail

def latex_k(db,regs,ptype='ksf'):
    print latex_head
    for det in dets:
        print r"""
\begin{table}[htb]
\begin{center}
\begin{tabular}{|c|c|c|}
\hline
Region    &  $k_{+}$ & $k_{-}$ \\
\hline\hline"""
        print r'\multicolumn{3}{|c|}{'+'%s'%dets_map[det]+r'}     \\ \hline'
        for reg in regs:
            s=db[det][reg][ptype]
            #Endcap-A  &   $100.1 \pm 0.4\%$  &  $    100.0 \pm 0.4\%$     \\ \hline
            pho1='    ' if s['kp']>100.0 else '\pho '
            pho2='    ' if s['km']>100.0 else '\pho '
            print '%s'%regs_map[reg]+r'  &   $' + '%s%.2f'%(pho1,s['kp'])+r' \pm '+'%.2f'%s['ekp']+r'\%$  &  $    '+'%s%.2f'%(pho2,s['km'])+r' \pm '+'%.2f'%s['ekm']+r'\%$     \\ \hline'
        print r'\hline'
        print r"\end{tabular}"
        print r"\caption{%s: release-%d absolute scales}"%(algos[opts.algo],opts.rel)
        print r"\label{tab:" + 'absolute%srel%d'%(algos[opts.algo],opts.rel) + r'}'
        print r"\end{center}"
        print r"\end{table}"
    print latex_tail

def get_statdev(db,regs,ptype='ksf'):
    stat1 = [],[]
    stat2 = [],[]
    for det in dets:
        for reg in regs:
            s=db[det][reg][ptype]
            stat1[0].append(s['ekp1']) #k+
            stat1[1].append(s['ekm1']) #k-
            stat2[0].append(s['ekp2'])
            stat2[1].append(s['ekm2'])
    return stat1,stat2
def latex_sysdev(db1,db2,regs,ptype1='ksf',ptype2='ksf'):
    mysys = [],[]
    for det in dets:
        print r'\multicolumn{3}{|c|}{'+'%s'%dets_map[det]+r'}     \\ \hline'
        for reg in regs:
            s1=db1[det][reg][ptype1]
            s2=db2[det][reg][ptype2]
            s=[]
            dlP=s2['kp']-s1['kp']
            dlN=s2['km']-s1['km']
            mysys[0].append(dlP)
            mysys[1].append(dlN)
            s.append('%s%.2f'%('' if dlP<0 else '+',dlP))
            s.append('%s%.2f'%('' if dlN<0 else '+',dlN))
            #Endcap-A  &   $+0.05\%$  &  $-0.05\%$     \\ \hline
            print '%s'%regs_map[reg]+r'  &   $' + '%s'%s[0]+r'\%$ & $'+'%s'%s[1]+r'\%$   \\ \hline'
        print r'\hline'
    return mysys

# load everything into dictionaries
alg='staco' if algo==0 else 'muid'
default,klu,R70,egge = [None]*4
regs = ['E%dE'%i for i in regsR] # FIXME - add other regions
try:
    default = load_all(regs=regs,pattern_R0='/keysfit/r%d_default_%s'%(rel,alg)+'/%s/%s/R0',pattern_Z='/zpeak/r%d_default_%s'%(rel,alg)+'/%s/%s/gaus0')
except:
    print 'Failed to load default values'
    raise
try:
    klu = load_all(regs=regs,pattern_R0='/keysfit/r%d_klu_%s'%(rel,alg)+'/%s/%s/R0',pattern_Z='/zpeak/r%d_default_%s'%(rel,alg)+'/%s/%s/gaus0')
except:
    print 'Failed to load klu'
try:
    R70 = load_all(regs=regs,pattern_R0='/keysfit/r%d_m70110_%s'%(rel,alg)+'/%s/%s/R0',pattern_Z='/zpeak/r%d_default_%s'%(rel,alg)+'/%s/%s/gaus0')
except:
    print 'Failed to load R70'
try:
    egge = load_all(regs=regs,pattern_R0='/keysfit/r%d_default_%s'%(rel,alg)+'/%s/%s/R0',pattern_Z='/zpeak/r%d_default_%s'%(rel,alg)+'/%s/%s/egge3')
except:
    print 'Failed to load egge'
    
# dump scales as C++ arrays
if mode==0:
    print_cpp(default,['AA','BB','CC'])
if mode==1:
    print_cpp(default,['FWA','MWA','Baa','Bcc','MWC','FWC'])
if mode==2:
    print_cpp(default,['E%dE'%i for i in regsR])
# latex R0 table
if mode==3:
    latex_R(default,['AA','BB','CC'])
if mode==4:
    latex_R(default,['FWA','MWA','Baa','Bcc','MWC','FWC'])
if mode==5:
    fname = 'latex/table_R0_r%d_%s.tex'%(opts.rel,opts.algo)
    fout = open(fname,'w')
    sys.stdout = fout
    latex_R(default,['E%dE'%i for i in regsR])
    fout.close()
    sys.stdout = stdout
# latex kp/km table
if mode==6:
    latex_k(default,['AA','BB','CC'])
if mode==7:
    latex_k(default,['FWA','MWA','Baa','Bcc','MWC','FWC'])
if mode==8:
    fname = 'latex/table_kpkm_r%d_%s.tex'%(opts.rel,opts.algo)
    fout = open(fname,'w')
    sys.stdout = fout
    latex_k(default,['E%dE'%i for i in regsR])
    fout.close()
    sys.stdout = stdout

def print_syst(regs):
    print '==================SYS1================='
    sys1 = latex_sysdev(default,klu,regs)
    print '==================SYS2================='
    sys2 = latex_sysdev(default,R70,regs)
    print '==================SYS3================='
    sys3 = latex_sysdev(default,default,regs,ptype2='chif')
    print '==================SYS4================='
    sys4 = latex_sysdev(default,egge,regs)
    print '==================SUMMARY================='
    stat1,stat2 = get_statdev(default,regs)
    print r'$R_0$ determination & ' + ' & '.join(['%.2f'%s for s in max_pair(stat2)])+r'\\'
    print r'             Z mass & ' + ' & '.join(['%.2f'%s for s in max_pair(stat1)])+r'\\'
    print r'\hline'
    print r'\multicolumn{'+'10'+r'}{c}{Systematic Uncertainties}\\'
    print r'\hline'
    print 'Expanded sample & ' + ' & '.join(['%.2f'%s for s in max_pair(sys1)])+r'\\'
    print 'Mass window for Z selection & ' + ' & '.join(['%.2f'%s for s in max_pair(sys2)])+r'\\'
    print 'Shape comparison method & ' + ' & '.join(['%.2f'%s for s in max_pair(sys3)])+r'\\'
    print 'Z mass fit function & ' + ' & '.join(['%.2f'%s for s in max_pair(sys4)])+r'\\'
    print r'\hline'
    print r'\multicolumn{'+'10'+r'}{c}{Summary}\\'
    print r'\hline'
    print r'     Correlated (constant $k_+/k_-$) & ' + ' & '.join(['%.2f'%s for s in sumsq([max_pair(stat1),max_pair(sys4)])]) + r'\\'
    print r'Anti-correlated (constant $k_+ k_-$) & ' + ' & '.join(['%.2f'%s for s in sumsq([max_pair(stat2),max_pair(sys1),max_pair(sys2),max_pair(sys3)])]) + r'\\'

if mode==10:
    regs = ['AA','BB','CC']
    print_syst(regs)
    
if mode==11:
    if opts.dets==0:
        pass
    elif opts.dets==1:
        dets = ['cmb',]
    elif opts.dets==2:
        dets = ['exms',]
    elif opts.dets==3:
        dets = ['id',]
    else:
        assert False,'Unknown --dets'
    regs = ['FWA','MWA','Baa','Bcc','MWC','FWC']
    print_syst(regs)


if mode==0725:
    nomsid=load_all(pattern_R0='/keysfit/nomsid/%s/%s/R0',pattern_Z='/zpeak/default/%s/%s/gaus0')
    print 'DEFAULT:'
    latex_R(default,['AA','BB','CC'])
    print 'NOMSID:'
    latex_R(nomsid,['AA','BB','CC'])
if mode==07252:
    nomsid=load_all(pattern_R0='/keysfit/nomsid/%s/%s/R0',pattern_Z='/zpeak/default/%s/%s/gaus0')
    print 'DEFAULT:'
    latex_R(default,['FWA','MWA','Baa','Bcc','MWC','FWC'])
    print 'NOMSID:'
    latex_R(nomsid,['FWA','MWA','Baa','Bcc','MWC','FWC'])

os.system('if [ -f %s ]; then pdflatex -output-directory latex/ %s; fi'%(fname,fname))
