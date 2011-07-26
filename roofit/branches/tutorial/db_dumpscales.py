#!/usr/bin/env python

"""
Quick script to dump scales
"""
try:
    import psyco
    psyco.full()
except ImportError:
    pass

import sys,math
import antondb
from optparse import OptionParser

dbname = 'backup/out0721'

from optparse import OptionParser
parser = OptionParser()
parser.add_option("-m", "--mode",dest="mode",
                  type="int", default=0,
                  help="Plot mode")
(opts, args) = parser.parse_args()
mode = opts.mode

a = antondb.antondb(dbname)
a.load()

dets = ['cmb','exms','id']
dets_map = {'cmb' : 'Combined muons', 'id' : 'ID muons', 'exms' : 'MS muons'}
regs_map = {'AA' : 'Endcap-A', 'BB' : 'Barrel', 'CC' : 'Endcap-C', 'Bcc' : 'Barrel C-side ($-1.05 < \eta < 0.0$)', 'Baa' : 'Barrel A-side ($0.0 < \eta < 1.05$)', 'FWC' : r'Endcap C-side ($-2.4 < \eta < -2.0$)', 'FWA' : r'Endcap A-side ($2.0 < \eta < 2.4$)', 'MWC' : r'Endcap C-side ($-2.0 < \eta < -1.05$)', 'MWA' : r'Endcap A-side ($1.05 < \eta < 2.0$)'}

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

def load_all(regs=['AA','BB','CC']+['FWA','MWA','Baa','Bcc','MWC','FWC'], pattern_R0='/default/%s/%s/R0',pattern_Z='/default/%s/%s/Z/gaus0'):
    res = {}
    for det in dets:
        res[det] = {}
        for reg in regs:
            res[det][reg] = {}
            aR0 = a.data[pattern_R0%(det,reg)];  assert aR0
            aZ = a.data[pattern_Z%(det,reg)];  assert aZ
            res[det][reg]['ksf'] = scales(aR0['ksf'],aR0['chie'],aZ['data_mz'],aZ['data_emz'],aZ['mc_mz'])
            res[det][reg]['chif'] = scales(aR0['chif'],aR0['chie'],aZ['data_mz'],aZ['data_emz'],aZ['mc_mz'])
            res[det][reg]['ksp'] = scales(aR0['ksp'],aR0['chie'],aZ['data_mz'],aZ['data_emz'],aZ['mc_mz'])
            res[det][reg]['chip'] = scales(aR0['chip'],aR0['chie'],aZ['data_mz'],aZ['data_emz'],aZ['mc_mz'])
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
    for det in dets:
        print r'\multicolumn{3}{|c|}{'+'%s'%dets_map[det]+r'}     \\ \hline'
        for reg in regs:
            s=db[det][reg][ptype]
            #Endcap-A  &   $100.1 \pm 0.4\%$  &  $    100.0 \pm 0.4\%$     \\ \hline
            print '%s'%regs_map[reg]+r'  &  ' + '%.2f'%(s['R'])+r'\%  &   '+'%.2f'%(s['eR'])+r'\%     \\ \hline'
        print r'\hline'

def latex_k(db,regs,ptype='ksf'):
    for det in dets:
        print r'\multicolumn{3}{|c|}{'+'%s'%dets_map[det]+r'}     \\ \hline'
        for reg in regs:
            s=db[det][reg][ptype]
            #Endcap-A  &   $100.1 \pm 0.4\%$  &  $    100.0 \pm 0.4\%$     \\ \hline
            pho1='    ' if s['kp']>100.0 else '\pho '
            pho2='    ' if s['km']>100.0 else '\pho '
            print '%s'%regs_map[reg]+r'  &   $' + '%s%.2f'%(pho1,s['kp'])+r' \pm '+'%.2f'%s['ekp']+r'\%$  &  $    '+'%s%.2f'%(pho2,s['km'])+r' \pm '+'%.2f'%s['ekm']+r'\%$     \\ \hline'
        print r'\hline'

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
default = load_all(pattern_R0='/keysfit/default/%s/%s/R0',pattern_Z='/zpeak/default/%s/%s/gaus0')
klu = load_all(pattern_R0='/keysfit/klu/%s/%s/R0',pattern_Z='/zpeak/default/%s/%s/gaus0')
R70 = load_all(pattern_R0='/keysfit/m70110/%s/%s/R0',pattern_Z='/zpeak/default/%s/%s/gaus0')
egge = load_all(pattern_R0='/keysfit/default/%s/%s/R0',pattern_Z='/zpeak/default/%s/%s/egge3')

# dump scales as C++ arrays
if mode==0:
    print_cpp(default,['AA','BB','CC'])
if mode==1:
    print_cpp(default,['FWA','MWA','Baa','Bcc','MWC','FWC'])
# latex R0 table
if mode==3:
    latex_R(default,['AA','BB','CC'])
if mode==4:
    latex_R(default,['FWA','MWA','Baa','Bcc','MWC','FWC'])
# latex kp/km table
if mode==5:
    latex_k(default,['AA','BB','CC'])
if mode==6:
    latex_k(default,['FWA','MWA','Baa','Bcc','MWC','FWC'])
    
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
    #dets = ['id',]
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
