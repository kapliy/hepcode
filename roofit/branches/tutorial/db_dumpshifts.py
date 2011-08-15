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

dbname = 'out0813'

from optparse import OptionParser
parser = OptionParser()
parser.add_option("-m", "--mode",dest="mode",
                  type="int", default=111,
                  help="Plot mode")
parser.add_option("-a", "--algo",dest="algo",
                  type="int", default=0,
                  help="Algorith: 0=STACO, 1=MuID")
parser.add_option("-d", "--dets",dest="dets",
                  type="int", default=0,
                  help="For printing syst tables: 0=ALL,1=cmb,2=exms,3=id")
(opts, args) = parser.parse_args()
mode = opts.mode
algo = opts.algo

a = antondb.antondb(dbname)
a.load()

dets = ['cmb','exms','id']
dets_map = {'cmb' : 'Combined muons', 'id' : 'ID muons', 'exms' : 'MS muons'}
regs_map = {'AA' : 'Endcap-A', 'BB' : 'Barrel', 'CC' : 'Endcap-C', 'Bcc' : 'Barrel C-side ($-1.05 < \eta < 0.0$)', 'Baa' : 'Barrel A-side ($0.0 < \eta < 1.05$)', 'FWC' : r'Endcap C-side ($-2.4 < \eta < -2.0$)', 'FWA' : r'Endcap A-side ($2.0 < \eta < 2.4$)', 'MWC' : r'Endcap C-side ($-2.0 < \eta < -1.05$)', 'MWA' : r'Endcap A-side ($1.05 < \eta < 2.0$)'}

def load_shifts(regs=['AA','BB','CC']+['FWA','MWA','Baa','Bcc','MWC','FWC'], pattern='/default/%s/%s/R0'):
    res = {}
    for det in dets:
        res[det] = {}
        for reg in regs:
            res[det][reg] = {}
            d = a.data[pattern%(det,reg)];  assert d
            res[det][reg]['ksf'] = { 's' : d['ksf']*1e6/2.0 , 'es' : d['chie']*1e6/2.0 }
            res[det][reg]['chif'] = { 's' : d['chif']*1e6/2.0 , 'es' : d['chie']*1e6/2.0 }
            res[det][reg]['ksp'] = { 's' : d['ksp']*1e6/2.0 , 'es' : d['chie']*1e6/2.0 }
            res[det][reg]['chip'] = { 's' : d['chip']*1e6/2.0 , 'es' : d['chie']*1e6/2.0 }
            if True: # default:
                res[det][reg]['s'] = d['ksf']*1e6/2.0
                res[det][reg]['es'] = d['chie']*1e6/2.0
    return res

def load_scales(regs=['AA','BB','CC']+['FWA','MWA','Baa','Bcc','MWC','FWC'], pattern='/default/%s/%s/Z/gaus0'):
    res = {}
    for det in dets:
        res[det] = {}
        for reg in regs:
            res[det][reg] = {}
            d = a.data[pattern%(det,reg)];  assert d
            res[det][reg]['k'] = d['data_mz']/d['mc_mz']*100.0
            res[det][reg]['ek'] = d['data_emz']/d['mc_mz']*100.0
    return res

def latex_R(db,regs,ptype='ksf'):
    for det in dets:
        print r'\multicolumn{3}{|c|}{'+'%s'%dets_map[det]+r'}     \\ \hline'
        for reg in regs:
            s=db[det][reg][ptype]
            #Endcap-A  &   $100.1 \pm 0.4\%$  &  $    100.0 \pm 0.4\%$     \\ \hline
            print '%s'%regs_map[reg]+r'  &  $' + '%.1f'%(s['s'])+r'*10^{-6}/GeV$  &   $'+'%.1f'%(s['es'])+r'*10^{-6}/GeV$     \\ \hline'
        print r'\hline'

def latex_k(db,regs):
    for det in dets:
        print r'\multicolumn{3}{|c|}{'+'%s'%dets_map[det]+r'}     \\ \hline'
        for reg in regs:
            s=db[det][reg]
            #Endcap-A  &   $100.1 \pm 0.4\%$  &  $    100.0 \pm 0.4\%$     \\ \hline
            print '%s'%regs_map[reg]+r'  &  ' + '%.2f'%(s['k'])+r'\%  &   '+'%.2f'%(s['ek'])+r'\%     \\ \hline'
        print r'\hline'

def get_statdev(db_scale,db_shift,regs,ptype='ksf'):
    stat1 = [],[]
    for det in dets:
        for reg in regs:
            stat1[0].append(db_scale[det][reg]['ek']) #scale
            stat1[1].append(db_shift[det][reg][ptype]['es']) #shift
    return stat1
def latex_sysdev(db1_scale,db1_shift,db2_scale,db2_shift,regs,ptype1='ksf',ptype2='ksf'):
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
    return mysys

def sumsq(s):
    grps = zip(*s)
    res = []
    for grp in grps:
        sq=0
        for i in grp:
            sq += i**2
        res.append(math.sqrt(sq))
    return res

def print_cpp(db_scale,db_shift,regs):
    print '========================='
    print 'C++ array:'
    print '========================='
    i=0; n=len(dets)*len(regs)
    for det in dets:
        for reg in regs:
            print '%.2f , %.2f'%(db_scale[det][reg]['k'],db_shift[det][reg]['s']),
            print ' , ' if i!=n-1 else ''
            i+=1

# load everything into dictionaries
if algo==0: # STACO
    default_scales = load_scales(pattern='/zpeak/default/%s/%s/gaus0')
    default_shifts = load_shifts(pattern='/keysfit/default/%s/%s/SHIFT')
    klu_shifts = load_shifts(pattern='/keysfit/klu/%s/%s/SHIFT')
    R70_shifts = load_shifts(pattern='/keysfit/m70110/%s/%s/SHIFT')
    egge_scales = load_scales(pattern='/zpeak/default/%s/%s/egge3')
elif algo==1: #MuID
    default_scales = load_scales(pattern='/zpeak/muid_default/%s/%s/gaus0')
    default_shifts = load_shifts(pattern='/keysfit/muid_default/%s/%s/SHIFT')
    klu_shifts = load_shifts(pattern='/keysfit/muid_klu/%s/%s/SHIFT')
    R70_shifts = load_shifts(pattern='/keysfit/muid_m70110/%s/%s/SHIFT')
    egge_scales = load_scales(pattern='/zpeak/muid_default/%s/%s/egge3')
else:
    assert False, 'Unknown algorithm'

# latex R0 table
if mode==0:
    regs = ['AA','BB','CC']
    print_cpp(default_scales,default_shifts,regs)
if mode==1:
    regs = ['FWA','MWA','Baa','Bcc','MWC','FWC']
    print_cpp(default_scales,default_shifts,regs)
if mode==3:
    regs = ['AA','BB','CC']
    print '================SHIFTS==================='
    latex_R(default_shifts,regs)
    print '================SCALES==================='
    latex_k(default_scales,regs)
if mode==4:
    regs = ['FWA','MWA','Baa','Bcc','MWC','FWC']
    print '================SHIFTS==================='
    latex_R(default_shifts,regs)
    print '================SCALES==================='
    latex_k(default_scales,regs)

def print_syst_curv(regs):
    print '==================SYS1================='
    sys1 = latex_sysdev(default_scales,default_shifts,default_scales,klu_shifts,regs)
    print '==================SYS2================='
    sys2 = latex_sysdev(default_scales,default_shifts,default_scales,R70_shifts,regs)
    print '==================SYS3================='
    sys3 = latex_sysdev(default_scales,default_shifts,default_scales,default_shifts,regs,ptype2='chif')
    print '==================STAT================='
    stat1 = get_statdev(default_scales,default_shifts,regs)
    print '==================SUMMARY================='
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

def print_syst_scale(regs):
    print '==================SYS4================='
    sys4 = latex_sysdev(default_scales,default_shifts,egge_scales,default_shifts,regs)
    print '==================STAT================='
    stat1 = get_statdev(default_scales,default_shifts,regs)
    print '==================SUMMARY================='
    print r'  $k$ determination & ' + ' & '.join(['%.2f'%s for s in stat1[0]])+r'\\'
    print r'\hline'
    print r'\multicolumn{'+'10'+r'}{c}{Systematic Uncertainties}\\'
    print r'\hline'
    print 'Z mass fit function & ' + ' & '.join(['%.2f'%s for s in sys4[0]])+r'\\'
    print r'\hline'
    print r'\multicolumn{'+'10'+r'}{c}{Summary}\\'
    print r'\hline'
    print r'     Total uncertainty & ' + ' & '.join(['%.2f'%s for s in sumsq([stat1[0],sys4[0]])]) + r'\\'

if mode==10:
    regs = ['AA','BB','CC']
    print_syst_curv(regs)
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
    print_syst_curv(regs)

if mode==20:
    regs = ['AA','BB','CC']
    print_syst_scale(regs)
if mode==21:
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
    print_syst_scale(regs)
