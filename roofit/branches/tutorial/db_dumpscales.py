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

dbname = 'out1023L7' # original presentation in MCP mtg on Nov 01 are from here
dbname = 'out1122v27closure' # closure tests on periods D..K with v27 ntuples
dbname = 'out1113mcpupdate' # published constants in MuonMomentumCorrections come from here
_DISABLE_KLU = False
_DISABLE_CHI = False

from optparse import OptionParser
parser = OptionParser()
parser.add_option("-m", "--mode",dest="mode",
                  type="int", default=2,
                  help="Plot mode")
parser.add_option("-s","--shift",default=False,
                  action="store_true",dest="shift",
                  help="Shift + absolute scale mode?")
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
alg='staco' if algo==0 else 'muid'
rel = opts.rel
shift = opts.shift
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

class Document:
    latex_head = r"""
    \documentclass[a4paper]{article}
    % Shorthand for \phantom to use in tables
    \newcommand{\pho}{\phantom{0}}
    \begin{document}
    """
    latex_tail = r"""
    \end{document}"""
    def __init__(s):
        s.nodes = []
        s.dets = []
    def add_node(s,node,det):
        s.dets.append(det)
        s.nodes.append(node)
    def print_mcp(s,type='KC'):
        if len(s.nodes)==0: return
        node = s.nodes[0]
        if type=='KC':
            print '# Overall scale & Curvature shift in %d bins'%len(node.boundsA)
        else:
            print '# Charge-dependent scales for mu+ and mu- in %d bins'%len(node.boundsA)
        print ' '.join('%.2f'%n for n in node.boundsA)
        for i,det in enumerate(s.dets):
            print det
            node = s.nodes[i]
            node.print_mcp(type)
    def print_txt(s):
        pass
    def print_latex(s):
        pass
        
class Table:
    """ A list of Values across several eta regions
        (CB/MS/ID each gets its own Table level)
    """
    boundsP = [0.30,0.60,0.9,1.30,1.60,2.00,2.50] #[TxT]
    boundsP = [0.50,1.05,1.70,2.51] #[SxS]
    boundsP = [1.05,2.51] #2012
    def ordered_pairs(s,lst):
        """ input: [a1,a2,a3]
        output: [a1,a2], [a2,a3]
        """
        return zip(lst,lst[1:])
    def __init__(s,det='CB'):
        s.det = det
        # overall scale + curvature splitting
        s.K = []
        s.C = []
        # charge-dependent scales
        s.kp = []
        s.km = []
        # bins
        s.boundsA = [-z for z in reversed(s.boundsP)] + [0.00,] + s.boundsP
        s.bounds = s.ordered_pairs(s.boundsA)
        s.bins = s.regsR = range(0,len(bounds))
    def add_all(s,K,C,kp,km):
        s.K.append(K)
        s.C.append(C)
        s.kp.append(kp)
        s.km.append(km)
    def print_mcp(s,type):
        if type=='KC':
            assert(len(s.K)==len(s.C)==len(s.bins))
            for ibin,bin in enumerate(s.bins):
                print s.K[ibin].string_mcp(),'\t',s.C[ibin].string_mcp()
        elif type=='kpkm': 
            assert(len(s.kp)==len(s.km)==len(s.bins))
            for ibin,bin in enumerate(s.bins):
                print s.kp[ibin].string_mcp(),'\t',s.km[ibin].string_mcp()
        else:
            assert False,'Unknown mcp table type; choose "KC" or "kpkm"'
    def print_latex(s):
        pass

class Value:
    """ Keeps mean, error, and systematic variations for a variable """
    def __init__(s):
        s.type = None
        s.keys = []
        # mean/error for default and each systematic variation
        s.mean = []
        s.stat = []
        # stat. error may also be split into correlated/uncorrelated component
        # (only for central value)
        s.stat1 = None
        s.stat2 = None
        # list of systematics groups (correlated/anticorrelated)
        s.sysgroups = [ (1,) , (2,3,4) ]
    def string_mcp(s):
        if s.type=='KC':
            return '%.2f\t%.2f\t%.2f'%(s.get_mean(), s.get_tot(), s.get_tot())
        elif s.type=='kpkm':
            return '%.2f\t%.2f\t%.2f'%(s.get_mean(), s.get_tot1(), s.get_tot2())
    def add(s,mean,stat,stat1=None,stat2=None,key='default'):
        assert key not in s.keys, 'Cannot have the same systematic label twice'
        s.keys.append(key)
        s.mean.append(mean)
        s.stat.append(stat)
        if (stat1 and stat2) and not (s.stat1 and s.stat2):
            s.stat1 = stat1
            s.stat2 = stat2
    def add_K(s,data_mz,data_emz,mc_mz,mc_emz=None,key='default'):
        """ specific function to add overall k factor """
        assert key not in s.keys, 'Cannot have the same systematic label twice'
        s.type = 'KC'
        mean = data_mz/mc_mz*100.0
        stat = data_emz/mc_mz*100.0
        s.keys.append(key)
        s.mean.append(mean)
        s.stat.append(stat)
    def add_C(s,C,C_e,key='default'):
        """ specific function to add overall k factor """
        assert key not in s.keys, 'Cannot have the same systematic label twice'
        s.type = 'KC'
        mean = C*1e6/2.0
        stat = C_e*1e6/2.0
        s.keys.append(key)
        s.mean.append(mean)
        s.stat.append(stat)
    def add_kp(s,R,eR,mz,emz,mz0,key='default'):
        """ specific function to add overall k factor """
        assert key not in s.keys, 'Cannot have the same systematic label twice'
        s.type = 'kpkm'
        kp = mz*math.sqrt(R)/(mz0)
        ekp = math.sqrt( emz**2 * (math.sqrt(R)/mz0)**2 + eR**2 * (mz/mz0)**2 / (4 * R) )
        s.keys.append(key)
        s.mean.append(kp*100.0)
        s.stat.append(ekp*100.0)
        if key=='default':
            ekp1,ekp2 = math.sqrt(emz**2 * (math.sqrt(R)/mz0)**2), math.sqrt(eR**2 * (mz/mz0)**2 / (4 * R))
            s.stat1 = ekp1*100.0
            s.stat2 = ekp2*100.0
    def add_km(s,R,eR,mz,emz,mz0,key='default'):
        """ specific function to add overall k factor """
        assert key not in s.keys, 'Cannot have the same systematic label twice'
        s.type = 'kpkm'
        km = mz/(mz0*math.sqrt(R))
        ekm = math.sqrt( emz**2 * 1/(mz0*math.sqrt(R))**2 + eR**2 * (mz/mz0)**2 / (4 * R**3) )
        s.keys.append(key)
        s.mean.append(km*100.0)
        s.stat.append(ekm*100.0)
        if key=='default':
            ekm1,ekm2 = math.sqrt(emz**2 * 1/(mz0*math.sqrt(R))**2),math.sqrt(eR**2 * (mz/mz0)**2 / (4 * R**3))
            s.stat1 = ekm1*100.0
            s.stat2 = ekm2*100.0
    def get_mean(s,key='default'):
        i = s.keys.index(key)
        return s.mean[i]
    def get_stat(s,key='default'):
        i = s.keys.index(key)
        return s.stat[i]
    def get_stat1(s):
        assert s.stat1, 'You need to set s.stat1 to use this'
        return s.stat1 if s.stat1 else 0
    def get_stat2(s):
        assert s.stat2, 'You need to set s.stat2 to use this'
        return s.stat2 if s.stat2 else 0
    def get_sys(s):
        r0 = s.mean[0]
        rsys = s.mean[1:]
        return math.sqrt( sum([(r-r0)**2 for r in rsys]) )
    def get_sys1(s):
        r0 = s.mean[0]
        rsys = [s.mean[i] for i in s.sysgroups[0]]
        return math.sqrt( sum([(r-r0)**2 for r in rsys]) )
    def get_sys2(s):
        r0 = s.mean[0]
        rsys = [s.mean[i] for i in s.sysgroups[1]]
        return math.sqrt( sum([(r-r0)**2 for r in rsys]) )
    def get_tot(s):
        return math.sqrt(s.get_sys()**2 + s.get_stat()**2)
    def get_tot1(s):
        return math.sqrt(s.get_sys1()**2 + s.get_stat1()**2)
    def get_tot2(s):
        return math.sqrt(s.get_sys2()**2 + s.get_stat2()**2)

dets = ['cmb','exms','id']
dets_map = {'cmb' : 'Combined muons', 'id' : 'ID muons', 'exms' : 'MS muons'}
regs_map = {'AA' : 'Endcap-A', 'BB' : 'Barrel', 'CC' : 'Endcap-C', 'Bcc' : 'Barrel C-side ($-1.05 < \eta < 0.0$)', 'Baa' : 'Barrel A-side ($0.0 < \eta < 1.05$)', 'FWC' : r'Endcap C-side ($-2.4 < \eta < -2.0$)', 'FWA' : r'Endcap A-side ($2.0 < \eta < 2.4$)', 'MWC' : r'Endcap C-side ($-2.0 < \eta < -1.05$)', 'MWA' : r'Endcap A-side ($1.05 < \eta < 2.0$)'}
syscap = []
syscap.append(r'when one of the Z legs is allowed to be outside of the fiducial region, expanding the sample size used in $R_{0}$ determination.')
syscap.append(r'associated with the expansion of the mass window used in the $R_{0}$ fit procedure from 80--100 GeV to 70--110 GeV.')
syscap.append(r'associated with the difference between the $\chi^{2}$ and KS methods of finding $R_{0}$')
syscap.append(r'when the fit function is changed from simple Gaussian to full Z lineshape.')

def ordered_pairs(lst):
    """ input: [a1,a2,a3]
        output: [a1,a2], [a2,a3]
    """
    return zip(lst,lst[1:])
boundsP = [0.30,0.60,0.9,1.30,1.60,2.00,2.50] #[TxT]
boundsP = [0.50,1.05,1.70,2.51] #[SxS]
boundsP = [1.05,2.51] #2012
boundsA = [-z for z in reversed(boundsP)] + [0.00,] + boundsP
bounds = ordered_pairs(boundsA)
regsR = range(0,len(bounds))
for bn in regsR:
    bb = bounds[bn]
    regs_map['S%dS'%bn] = r'$%.1f < \eta < %.1f$'%(bb[0],bb[1])

def scales(R,eR,mz,emz,mz0):
    """ Does the math to deduce k+ and k-, along with associated errors """
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

def load_discales(regs=[], pattern_R0='/default/%s/%s/R0',pattern_Z='/default/%s/%s/Z/gaus0'):
    """ Loads k+ and k- """
    res = {}
    for det in dets:
        res[det] = {}
        for reg in regs:
            res[det][reg] = {}
            aR0 = a.data[pattern_R0%(det,reg)];  assert aR0
            if True:  # False if z peak fit was not run
                aZ = a.data[pattern_Z%(det,reg)];  assert aZ
                res[det][reg]['ksf'] = scales(aR0['ksf'],aR0['chie'],aZ['data_mz'],aZ['data_emz'],aZ['mc_mz'])
                if _DISABLE_CHI:
                    res[det][reg]['chif'] = scales(aR0['ksf'],aR0['chie'],aZ['data_mz'],aZ['data_emz'],aZ['mc_mz'])
                else:
                    res[det][reg]['chif'] = scales(aR0['chif'],aR0['chie'],aZ['data_mz'],aZ['data_emz'],aZ['mc_mz'])
                res[det][reg]['ksp'] = scales(aR0['ksp'],aR0['chie'],aZ['data_mz'],aZ['data_emz'],aZ['mc_mz'])
                res[det][reg]['chip'] = scales(aR0['chip'],aR0['chie'],aZ['data_mz'],aZ['data_emz'],aZ['mc_mz'])
            else:
                res[det][reg]['ksf'] = scales(aR0['ksf'],aR0['chie'],1.0,0.1,1.01)
                res[det][reg]['chif'] = scales(aR0['chif'],aR0['chie'],1.0,0.1,1.01)
                res[det][reg]['ksp'] = scales(aR0['ksp'],aR0['chie'],1.0,0.1,1.01)
                res[det][reg]['chip'] = scales(aR0['chip'],aR0['chie'],1.0,0.1,1.01)
    return res

def load_shift(regs=[], pattern='/default/%s/%s/R0'):
    """ Loads C - relative curvature shift (splitting) """
    res = {}
    for det in dets:
        res[det] = {}
        for reg in regs:
            res[det][reg] = {}
            d = a.data[pattern%(det,reg)];  assert d
            res[det][reg]['ksf'] = { 's' : d['ksf']*1e6/2.0 , 'es' : d['chie']*1e6/2.0 }
            if _DISABLE_CHI:
                res[det][reg]['chif'] = { 's' : d['ksf']*1e6/2.0 , 'es' : d['chie']*1e6/2.0 }
            else:
                res[det][reg]['chif'] = { 's' : d['chif']*1e6/2.0 , 'es' : d['chie']*1e6/2.0 }
            res[det][reg]['ksp'] = { 's' : d['ksp']*1e6/2.0 , 'es' : d['chie']*1e6/2.0 }
            res[det][reg]['chip'] = { 's' : d['chip']*1e6/2.0 , 'es' : d['chie']*1e6/2.0 }
            if True: # default:
                res[det][reg]['s'] = d['ksf']*1e6/2.0
                res[det][reg]['es'] = d['chie']*1e6/2.0
    return res

def load_scale(regs=[], pattern='/default/%s/%s/Z/gaus0'):
    """ Loads k - ovreall absolute scale that goes with curvature splitting """
    res = {}
    for det in dets:
        res[det] = {}
        for reg in regs:
            res[det][reg] = {}
            d = a.data[pattern%(det,reg)];  assert d
            res[det][reg]['k'] = d['data_mz']/d['mc_mz']*100.0
            res[det][reg]['ek'] = d['data_emz']/d['mc_mz']*100.0
    return res

def max_pair(s):
    s1,s2=s[0],s[1]
    assert len(s1)==len(s2),'Error: wrong array sizes'
    return [max(s) for s in zip([math.fabs(a) for a in s1],[math.fabs(b) for b in s2])]
def first_pair(s):
    s1,s2=s[0],s[1]
    assert len(s1)==len(s2),'Error: wrong array sizes'
    return [s[0] for s in zip([math.fabs(a) for a in s1],[math.fabs(b) for b in s2])]
def second_pair(s):
    s1,s2=s[0],s[1]
    assert len(s1)==len(s2),'Error: wrong array sizes'
    return [s[1] for s in zip([math.fabs(a) for a in s1],[math.fabs(b) for b in s2])]
def min_pair(s):
    s1,s2=s[0],s[1]
    assert len(s1)==len(s2),'Error: wrong array sizes'
    return [min(s) for s in zip([math.fabs(a) for a in s1],[math.fabs(b) for b in s2])]

def sumsq(s):
    grps = zip(*s)
    res = []
    for grp in grps:
        sq=0
        for i in grp:
            sq += i**2
        res.append(math.sqrt(sq))
    return res

def print_discale_cpp(db,regs):
    print '========================='
    print 'C++ array:'
    print '========================='
    i=0; n=len(dets)*len(regs)
    for det in dets:
        for reg in regs:
            print '%.2f , %.2f'%(default[det][reg]['ksf']['kp'],default[det][reg]['ksf']['km']),
            print ' , ' if i!=n-1 else ''
            i+=1

def print_shift_cpp(db_scale,db_shift,regs):
    print '========================='
    print 'C++ array:'
    print '========================='
    i=0; n=len(dets)*len(regs)
    for det in dets:
        for reg in regs:
            print '%.2f , %.2f'%(db_scale[det][reg]['k'],db_shift[det][reg]['s']),
            print ' , ' if i!=n-1 else ''
            i+=1

def latex_R(db,regs,ptype='ksf'):
    for det in dets:
        print r"""
\begin{table}[htb]
\begin{center}
\begin{tabular}{|c|c|c|}
\hline"""
        if shift:
            print r"Region    &   $C$     & $\delta C$   \\"
        else:
            print r"Region    &   $R_{0}$     & $\delta R_{0}$   \\"
        print r"\hline\hline"
        print r'\multicolumn{3}{|c|}{'+'%s'%dets_map[det]+r'}     \\ \hline'
        for reg in regs:
            s=db[det][reg][ptype]
            #Endcap-A  &   $100.1 \pm 0.4\%$  &  $    100.0 \pm 0.4\%$     \\ \hline
            if shift:
                print '%s'%regs_map[reg]+r'  &  $' + '%.1f'%(s['s'])+r'*10^{-6}/GeV$  &   $'+'%.1f'%(s['es'])+r'*10^{-6}/GeV$     \\ \hline'
            else:
                print '%s'%regs_map[reg]+r'  &  ' + '%.2f'%(s['R'])+r'\%  &   '+'%.2f'%(s['eR'])+r'\%     \\ \hline'
        print r'\hline'
        print r"\end{tabular}"
        if shift:
            print r"\caption{%s: release-%d curvature splitting (stat error)}"%(algos[opts.algo],opts.rel)
            print r"\label{tab:" + 'curvsplit%srel%d'%(algos[opts.algo],opts.rel) + r'}'
        else:
            print r"\caption{%s: release-%d relative scales (stat error)}"%(algos[opts.algo],opts.rel)
            print r"\label{tab:" + 'relative%srel%d'%(algos[opts.algo],opts.rel) + r'}'
        print r"\end{center}"
        print r"\end{table}"
        print r"\clearpage"

def latex_C_syst(db,regs,ptype='ksf'):
    assert shift
    err = get_syst_curv(regs)
    z=0
    for det in dets:
        print r"""
\begin{table}[htb]
\begin{center}
\begin{tabular}{|c|c|c|}
\hline"""
        print r"Region    &   $C$     & $\delta C$   \\"
        print r"\hline\hline"
        print r'\multicolumn{3}{|c|}{'+'%s'%dets_map[det]+r'}     \\ \hline'
        for reg in regs:
            s=db[det][reg][ptype]
            #Endcap-A  &   $100.1 \pm 0.4\%$  &  $    100.0 \pm 0.4\%$     \\ \hline
            print '%s'%regs_map[reg]+r'  &  $' + '%.1f'%(s['s'])+r'*10^{-6}/GeV$  &   $'+err[z]+r'*10^{-6}/GeV$     \\ \hline'
            z+=1
        print r'\hline'
        print r"\end{tabular}"
        print r"\caption{%s: release-%d curvature splitting (total error)}"%(algos[opts.algo],opts.rel)
        print r"\label{tab:" + 'curvsplit%srel%d'%(algos[opts.algo],opts.rel) + r'}'
        print r"\end{center}"
        print r"\end{table}"
        print r"\clearpage"

def latex_k(db,regs):
    assert shift
    for det in dets:
        print r"""
\begin{table}[htb]
\begin{center}
\begin{tabular}{|c|c|c|}
\hline
Region    &   $k$     & $\delta k$   \\
\hline\hline"""
        print r'\multicolumn{3}{|c|}{'+'%s'%dets_map[det]+r'}     \\ \hline'
        for reg in regs:
            s=db[det][reg]
            #Endcap-A  &   $100.1 \pm 0.4\%$  &  $    100.0 \pm 0.4\%$     \\ \hline
            print '%s'%regs_map[reg]+r'  &  ' + '%.2f'%(s['k'])+r'\%  &   '+'%.2f'%(s['ek'])+r'\%     \\ \hline'
        print r'\hline'
        print r"\end{tabular}"
        print r"\caption{%s: release-%d overall scale (stat error)}"%(algos[opts.algo],opts.rel)
        print r"\label{tab:" + 'overallscale%srel%d'%(algos[opts.algo],opts.rel) + r'}'
        print r"\end{center}"
        print r"\end{table}"
        print r"\clearpage"

def latex_k_syst(db,regs):
    assert shift
    err = get_syst_scale(regs)
    z=0
    for det in dets:
        print r"""
\begin{table}[htb]
\begin{center}
\begin{tabular}{|c|c|c|}
\hline
Region    &   $K$     & $\delta K$   \\
\hline\hline"""
        print r'\multicolumn{3}{|c|}{'+'%s'%dets_map[det]+r'}     \\ \hline'
        for reg in regs:
            s=db[det][reg]
            #Endcap-A  &   $100.1 \pm 0.4\%$  &  $    100.0 \pm 0.4\%$     \\ \hline
            print '%s'%regs_map[reg]+r'  &  ' + '%.2f'%(s['k'])+r'\%  &   '+err[z]+r'\%     \\ \hline'
            z+=1
        print r'\hline'
        print r"\end{tabular}"
        print r"\caption{%s: release-%d overall scale (total error)}"%(algos[opts.algo],opts.rel)
        print r"\label{tab:" + 'overallscale%srel%d'%(algos[opts.algo],opts.rel) + r'}'
        print r"\end{center}"
        print r"\end{table}"
        print r"\clearpage"

def latex_kpkm(db,regs,ptype='ksf'):
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
        print r"\caption{%s: release-%d absolute scales (stat error)}"%(algos[opts.algo],opts.rel)
        print r"\label{tab:" + 'absolute%srel%d'%(algos[opts.algo],opts.rel) + r'}'
        print r"\end{center}"
        print r"\end{table}"
        print r"\clearpage"

def latex_kpkm_syst(db,regs,ptype='ksf'):
    corr1,acorr1,corr2,acorr2 = get_syst_kpkm(regs)
    z=0
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
            print '%s'%regs_map[reg]+r'  &   $' + '%s%.2f'%(pho1,s['kp'])+r' \pm '+corr1[z]+r' \pm '+acorr1[z]+r'\%$  &  $    '+'%s%.2f'%(pho2,s['km'])+r' \pm '+corr2[z]+r' \pm '+acorr2[z]+r'\%$     \\ \hline'
            z+=1
        print r'\hline'
        print r"\end{tabular}"
        print r"\caption{%s: release-%d absolute scales (total error)}"%(algos[opts.algo],opts.rel)
        print r"\label{tab:" + 'absolute%srel%d'%(algos[opts.algo],opts.rel) + r'}'
        print r"\end{center}"
        print r"\end{table}"
        print r"\clearpage"

def get_statdev_kpkm(db,regs,ptype='ksf'):
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
def get_sysdev_kpkm(db1,db2,regs,ptype1='ksf',ptype2='ksf',caption=""):
    mysys = [],[]
    for det in dets:
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
    return mysys
def latex_sysdev_kpkm(db1,db2,regs,ptype1='ksf',ptype2='ksf',caption=""):
    print r"""
\begin{table}[htb]
\begin{center}
\begin{tabular}{|c|c|c|}
\hline
Region  & $\Delta k_{+}$ & $\Delta k_{-}$ \\
\hline\hline"""                            
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
    print r"\end{tabular}"
    print r'\caption{Systematic change in $k_\pm$ %s}'%caption
    print r'\label{tab:systvarkpkm}'
    print r'\end{center}'
    print r'\end{table}'
    print r"\clearpage"
    return mysys

def get_statdev_k(db_scale,db_shift,regs,ptype='ksf'):
    stat1 = [],[]
    for det in dets:
        for reg in regs:
            stat1[0].append(db_scale[det][reg]['ek']) #scale
            stat1[1].append(db_shift[det][reg][ptype]['es']) #shift
    return stat1
def get_sysdev_k(db1_scale,db1_shift,db2_scale,db2_shift,regs,ptype1='ksf',ptype2='ksf',caption=""):
    mysys = [],[]
    for det in dets:
        for reg in regs:
            s1_scale=db1_scale[det][reg]
            s2_scale=db2_scale[det][reg]
            s1_shift=db1_shift[det][reg][ptype1]
            s2_shift=db2_shift[det][reg][ptype2]
            s=[]
            dlP=s2_scale['k']-s1_scale['k']
            dlN=s2_shift['s']-s1_shift['s']
            mysys[0].append(dlP)
            mysys[1].append(dlN)
            s.append('%s%.2f'%('' if dlP<0 else '+',dlP))
            s.append('%s%.1f'%('' if dlN<0 else '+',dlN))
    return mysys
def latex_sysdev_k(db1_scale,db1_shift,db2_scale,db2_shift,regs,ptype1='ksf',ptype2='ksf',caption=""):
    print r"""
\begin{table}[htb]
\begin{center}
\begin{tabular}{|c|c|c|}
\hline
Region  & $\Delta k$ & $\Delta C$ \\
\hline\hline"""                           
    mysys = [],[]
    for det in dets:
        print r'\multicolumn{3}{|c|}{'+'%s'%dets_map[det]+r'}     \\ \hline'
        for reg in regs:
            s1_scale=db1_scale[det][reg]
            s2_scale=db2_scale[det][reg]
            s1_shift=db1_shift[det][reg][ptype1]
            s2_shift=db2_shift[det][reg][ptype2]
            s=[]
            dlP=s2_scale['k']-s1_scale['k']
            dlN=s2_shift['s']-s1_shift['s']
            mysys[0].append(dlP)
            mysys[1].append(dlN)
            s.append('%s%.2f'%('' if dlP<0 else '+',dlP))
            s.append('%s%.1f'%('' if dlN<0 else '+',dlN))
            #Endcap-A  &   $+0.05\%$  &  $-0.05\%$     \\ \hline
            print '%s'%regs_map[reg]+r'  &   $' + '%s'%s[0]+r'\%$ & $'+'%s'%s[1]+r'*10^{-6}/GeV$   \\ \hline'
        print r'\hline'
        print r"\end{tabular}"
        print r'\caption{Systematic change in $k$ and $C$ %s}'%caption
        print r"""
\label{tab:systvarkC}
\end{center}
\end{table}"""
        print r"\clearpage"
    return mysys

def get_syst_kpkm(regs):
    sys1 = get_sysdev_kpkm(default,klu,regs,caption=syscap[0])
    sys2 = get_sysdev_kpkm(default,R70,regs,caption=syscap[1])
    sys3 = get_sysdev_kpkm(default,default,regs,ptype2='chif',caption=syscap[2])
    sys4 = get_sysdev_kpkm(default,egge,regs,caption=syscap[3])
    stat1,stat2 = get_statdev_kpkm(default,regs)
    corr1 =  ['%.2f'%s for s in sumsq([first_pair(stat1),first_pair(sys4)])]
    acorr1 = ['%.2f'%s for s in sumsq([first_pair(stat2),first_pair(sys1),first_pair(sys2),first_pair(sys3)])]
    corr2 =  ['%.2f'%s for s in sumsq([second_pair(stat1),second_pair(sys4)])]
    acorr2 = ['%.2f'%s for s in sumsq([second_pair(stat2),second_pair(sys1),second_pair(sys2),second_pair(sys3)])]
    return corr1,acorr1,corr2,acorr2

def print_syst_kpkm(regs):
    sys1 = latex_sysdev_kpkm(default,klu,regs,caption=syscap[0])
    sys2 = latex_sysdev_kpkm(default,R70,regs,caption=syscap[1])
    sys3 = latex_sysdev_kpkm(default,default,regs,ptype2='chif',caption=syscap[2])
    sys4 = latex_sysdev_kpkm(default,egge,regs,caption=syscap[3])
    stat1,stat2 = get_statdev_kpkm(default,regs)
    print ''
    print r'\begin{table}'
    print r'\begin{center}'
    print r'\begin{tabular}{l%s}'%(''.join(['c']*len(dets)*len(regs)))
    print r'\hline\hline'
    for det in dets:
        print r'& \multicolumn{%d}{c}{%s} '%(len(regs),dets_map[det]),
    print r' \\'
    for det in dets:
        for reg in regs:
            print r'& %s'%reg,
    print r' \\'
    print r'\hline'
    print r'\multicolumn{%d}{c}{Statistical Uncertainties}\\'%(len(regs)*len(dets)+1)
    print r'\hline'
    print r'$R_0$ determination & ' + ' & '.join(['%.2f'%s for s in max_pair(stat2)])+r'\\'
    print r'             Z mass & ' + ' & '.join(['%.2f'%s for s in max_pair(stat1)])+r'\\'
    print r'\hline'
    print r'\multicolumn{%d}{c}{Systematic Uncertainties}\\'%(len(regs)*len(dets)+1)
    print r'\hline'
    print 'Expanded sample & ' + ' & '.join(['%.2f'%s for s in max_pair(sys1)])+r'\\'
    print 'Mass window for Z selection & ' + ' & '.join(['%.2f'%s for s in max_pair(sys2)])+r'\\'
    print 'Shape comparison method & ' + ' & '.join(['%.2f'%s for s in max_pair(sys3)])+r'\\'
    print 'Z mass fit function & ' + ' & '.join(['%.2f'%s for s in max_pair(sys4)])+r'\\'
    print r'\hline'
    print r'\multicolumn{%d}{c}{Summary}\\'%(len(regs)*len(dets)+1)
    print r'\hline'
    print r'     Correlated (constant $k_+/k_-$) & ' + ' & '.join(['%.2f'%s for s in sumsq([max_pair(stat1),max_pair(sys4)])]) + r'\\'
    print r'Anti-correlated (constant $k_+ k_-$) & ' + ' & '.join(['%.2f'%s for s in sumsq([max_pair(stat2),max_pair(sys1),max_pair(sys2),max_pair(sys3)])]) + r'\\'
    print r'\hline\hline'
    print r'\end{tabular}'
    print r'\caption{Summary of uncertainties on $k_\pm$.}'
    print r'\label{tab:systSummary}'
    print r'\end{center}'
    print r'\end{table}'

def get_syst_curv(regs):
    sys1 = get_sysdev_k(default_scales,default_shifts,default_scales,klu_shifts,regs,caption=syscap[0])
    sys2 = get_sysdev_k(default_scales,default_shifts,default_scales,R70_shifts,regs,caption=syscap[1])
    sys3 = get_sysdev_k(default_scales,default_shifts,default_scales,default_shifts,regs,ptype2='chif',caption=syscap[2])
    stat1 = get_statdev_k(default_scales,default_shifts,regs)
    return ['%.1f'%s for s in sumsq([stat1[1],sys1[1],sys2[1],sys3[1]])]
def get_syst_scale(regs):
    sys4 = get_sysdev_k(default_scales,default_shifts,egge_scales,default_shifts,regs,caption=syscap[3])
    stat1 = get_statdev_k(default_scales,default_shifts,regs)
    return ['%.2f'%s for s in sumsq([stat1[0],sys4[0]])]

# TODO: add proper latex to all of these
def print_syst_curv(regs):
    sys1 = latex_sysdev_k(default_scales,default_shifts,default_scales,klu_shifts,regs,caption=syscap[0])
    sys2 = latex_sysdev_k(default_scales,default_shifts,default_scales,R70_shifts,regs,caption=syscap[1])
    sys3 = latex_sysdev_k(default_scales,default_shifts,default_scales,default_shifts,regs,ptype2='chif',caption=syscap[2])
    stat1 = get_statdev_k(default_scales,default_shifts,regs)
    print r'$C$ determination & ' + ' & '.join(['%.1f'%s for s in stat1[1]])+r'\\'
    print r'\hline'
    print r'\multicolumn{'+'10'+r'}{c}{Systematic Uncertainties}\\'
    print r'\hline'
    print 'Expanded sample & ' + ' & '.join(['%.1f'%s for s in sys1[1]])+r'\\'
    print 'Mass window for Z selection & ' + ' & '.join(['%.1f'%s for s in sys2[1]])+r'\\'
    print 'Shape comparison method & ' + ' & '.join(['%.1f'%s for s in sys3[1]])+r'\\'
    print r'\hline'
    print r'\multicolumn{'+'10'+r'}{c}{Summary}\\'
    print r'\hline'
    print r'Total uncertainty & ' + ' & '.join(['%.1f'%s for s in sumsq([stat1[1],sys1[1],sys2[1],sys3[1]])]) + r'\\'
# TODO: add proper latex to all of these
def print_syst_scale(regs):
    sys4 = latex_sysdev_k(default_scales,default_shifts,egge_scales,default_shifts,regs,caption=syscap[3])
    stat1 = get_statdev_k(default_scales,default_shifts,regs)
    print r'  $k$ determination & ' + ' & '.join(['%.2f'%s for s in stat1[0]])+r'\\'
    print r'\hline'
    print r'\multicolumn{'+'10'+r'}{c}{Systematic Uncertainties}\\'
    print r'\hline'
    print 'Z mass fit function & ' + ' & '.join(['%.2f'%s for s in sys4[0]])+r'\\'
    print r'\hline'
    print r'\multicolumn{'+'10'+r'}{c}{Summary}\\'
    print r'\hline'
    print r'     Total uncertainty & ' + ' & '.join(['%.2f'%s for s in sumsq([stat1[0],sys4[0]])]) + r'\\'

# load everything into dictionaries
default,klu,R70,egge = [None]*4
default_shifts,klu_shifts,R70_shifts,egge_shifts = [None]*4
default_scales,klu_scales,R70_scales,egge_scales = [None]*4
regs = ['S%dS'%i for i in regsR]
regs = ['CC','Bcc','Baa','AA'] #2012
try:
    if shift:
        default_scales = load_scale(regs=regs,pattern='/zpeak/r%d_default_%s'%(rel,alg)+'/%s/%s/gaus0')
        default_shifts = load_shift(regs=regs,pattern='/keysfit/r%d_default_%s'%(rel,alg)+'/%s/%s/SHIFT')
    else:
        default = load_discales(regs=regs,pattern_R0='/keysfit/r%d_default_%s'%(rel,alg)+'/%s/%s/R0',
                                pattern_Z='/zpeak/r%d_default_%s'%(rel,alg)+'/%s/%s/gaus0')
except:
    print 'Failed to load default values'
    raise
try:
    if shift:
        klu_shifts = load_shift(regs=regs,pattern='/keysfit/r%d_klu_%s'%(rel,alg)+'/%s/%s/SHIFT')
    else:
        klu = load_discales(regs=regs,pattern_R0='/keysfit/r%d_klu_%s'%(rel,alg)+'/%s/%s/R0',
                            pattern_Z='/zpeak/r%d_default_%s'%(rel,alg)+'/%s/%s/gaus0')
    if _DISABLE_KLU:
        klu_shifts = default_shifts
        klu = default
except:
    print 'Failed to load klu'
try:
    if shift:
        R70_shifts = load_shift(regs=regs,pattern='/keysfit/r%d_m70110_%s'%(rel,alg)+'/%s/%s/SHIFT')
    else:
        R70 = load_discales(regs=regs,pattern_R0='/keysfit/r%d_m70110_%s'%(rel,alg)+'/%s/%s/R0',
                            pattern_Z='/zpeak/r%d_default_%s'%(rel,alg)+'/%s/%s/gaus0')
except:
    print 'Failed to load R70'
try:
    if shift:
        egge_scales = load_scale(regs=regs,pattern='/zpeak/r%d_default_%s'%(rel,alg)+'/%s/%s/egge3')
    else:
        egge = load_discales(regs=regs,pattern_R0='/keysfit/r%d_default_%s'%(rel,alg)+'/%s/%s/R0',
                             pattern_Z='/zpeak/r%d_default_%s'%(rel,alg)+'/%s/%s/egge3')
except:
    print 'Failed to load egge'
    
# dump scales as C++ arrays
if mode==0:
    if shift:
        print_shift_cpp(default_scales,default_shifts,regs)
    else:
        print_discale_cpp(default,regs)
# latex R0 table
if mode==5:
    ttype = 'C' if shift else 'R0' 
    fname = 'latex/table_%s_r%d_%s.tex'%(ttype,opts.rel,algos[algo])
    fout = open(fname,'w')
    sys.stdout = fout
    print latex_head
    if shift:
        latex_R(default_shifts,regs)
    else:
        latex_R(default,regs)
    print latex_tail
    fout.close()
    sys.stdout = stdout
# latex kp/km or K/C table (stat only)
if mode==8:
    ttype = 'kC' if shift else 'kpkm' 
    fname = 'latex/table_%s_r%d_%s_STAT.tex'%(ttype,opts.rel,algos[algo])
    fout = open(fname,'w')
    sys.stdout = fout
    print latex_head
    if shift:
        latex_R(default_shifts,regs)
        latex_k(default_scales,regs)
    else:
        latex_kpkm(default,regs)
    print latex_tail
    fout.close()
    sys.stdout = stdout
if mode==80:
    ttype = 'kC' if shift else 'kpkm' 
    fname = 'latex/table_%s_r%d_%s_TOT.tex'%(ttype,opts.rel,algos[algo])
    fout = open(fname,'w')
    sys.stdout = fout
    print latex_head
    if shift:
        latex_C_syst(default_shifts,regs)
        latex_k_syst(default_scales,regs)
    else:
        latex_kpkm_syst(default,regs)
    print latex_tail
    fout.close()
    sys.stdout = stdout

# todo: add shift version
if mode==10:
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
    fname = 'latex/table_syst%d_r%d_%s.tex'%(opts.dets,opts.rel,algos[algo])
    fout = open(fname,'w')
    sys.stdout = fout
    print latex_head
    print_syst_kpkm(regs)
    print latex_tail
    fout.close()
    sys.stdout = stdout

t = Document()
dets = ['cmb','exms','id']
for det in dets:
    node = Table()
    for reg in regs:
        v_K = Value()
        v_C = Value()
        v_kp = Value()
        v_km = Value()
        Cdef = a.data['/keysfit/r%d_default_%s/%s/%s/SHIFT'%(rel,alg,det,reg)]
        Cklu = a.data['/keysfit/r%d_klu_%s/%s/%s/SHIFT'%(rel,alg,det,reg)]
        CR70 = a.data['/keysfit/r%d_m70110_%s/%s/%s/SHIFT'%(rel,alg,det,reg)]
        Kdef = a.data['/keysfit/r%d_default_%s/%s/%s/R0'%(rel,alg,det,reg)]
        Kklu = a.data['/keysfit/r%d_klu_%s/%s/%s/R0'%(rel,alg,det,reg)]
        KR70 = a.data['/keysfit/r%d_m70110_%s/%s/%s/R0'%(rel,alg,det,reg)]
        zft0 = a.data['/zpeak/r%d_default_%s/%s/%s/gaus0'%(rel,alg,det,reg)]
        zft3 = a.data['/zpeak/r%d_default_%s/%s/%s/egge3'%(rel,alg,det,reg)]
        keys = ['default','chi2','klu','R70','egge']
        v_C.add_C(Cdef['ksf'], Cdef['chie'],key=keys[0])
        v_C.add_C(Cdef['chif'],Cdef['chie'],key=keys[1])
        v_C.add_C(Cklu['ksf'], Cklu['chie'],key=keys[2])
        v_C.add_C(CR70['ksf'], CR70['chie'],key=keys[3])
        v_K.add_K(zft0['data_mz'],zft0['data_emz'],zft0['mc_mz'],key=keys[0])
        v_K.add_K(zft3['data_mz'],zft3['data_emz'],zft3['mc_mz'],key=keys[4])
        v_kp.add_kp(Kdef['ksf'], Kdef['chie'],zft0['data_mz'],zft0['data_emz'],zft0['mc_mz'],key=keys[0])
        v_kp.add_kp(Kdef['chif'],Kdef['chie'],zft0['data_mz'],zft0['data_emz'],zft0['mc_mz'],key=keys[1])
        v_kp.add_kp(Kklu['ksf'], Kklu['chie'],zft0['data_mz'],zft0['data_emz'],zft0['mc_mz'],key=keys[2])
        v_kp.add_kp(KR70['ksf'], KR70['chie'],zft0['data_mz'],zft0['data_emz'],zft0['mc_mz'],key=keys[3])
        v_kp.add_kp(Kdef['ksf'], Kdef['chie'],zft3['data_mz'],zft3['data_emz'],zft3['mc_mz'],key=keys[4])
        v_km.add_km(Kdef['ksf'], Kdef['chie'],zft0['data_mz'],zft0['data_emz'],zft0['mc_mz'],key=keys[0])
        v_km.add_km(Kdef['chif'],Kdef['chie'],zft0['data_mz'],zft0['data_emz'],zft0['mc_mz'],key=keys[1])
        v_km.add_km(Kklu['ksf'], Kklu['chie'],zft0['data_mz'],zft0['data_emz'],zft0['mc_mz'],key=keys[2])
        v_km.add_km(KR70['ksf'], KR70['chie'],zft0['data_mz'],zft0['data_emz'],zft0['mc_mz'],key=keys[3])
        v_km.add_km(Kdef['ksf'], Kdef['chie'],zft3['data_mz'],zft3['data_emz'],zft3['mc_mz'],key=keys[4])
        node.add_all(v_K,v_C,v_kp,v_km)
    t.add_node(node,det)

if mode==15:
    t.print_mcp(type='kpkm' if not opts.shift else 'KC')

os.system('if [ -f %s ]; then pdflatex -output-directory latex/ %s &>%s.log; fi'%(fname,fname,fname))
