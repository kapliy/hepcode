#!/usr/bin/env python

import sys,os,math

klus = ['','KLU_']
dets = ['cmb','exms','id']
regs = ["AA","BB","CC"]
fname_pat='root_0505_closure.root'
resmodel='gaus0'  #egge3
folder_pat='data' 
mc_pat='mc_zmumu'

pattern = 'ZDATA_%s%s_%s_%s_%s_%s_%s'
mc_pattern = 'ZMC_%s%s_%s_%s_%s_%s_%s'

ddefault='70to110/ks/'
dchi='70to110/chi/'
dnarrow='80to100/ks/'

dets_map = {'cmb' : 'Combined muons', 'id' : 'ID muons', 'exms' : 'MS muons'}
regs_map = {'AA' : 'Endcap-A', 'BB' : 'Barrel', 'CC' : 'Endcap-C', 'Bcc' : 'Barrel C-side', 'Baa' : 'Barrel A-side', 'FWC' : 'Endcap C-side (-2.4 < eta < -2.0)', 'FWA' : 'Endcap A-side (2.0 < eta < 2.4)', 'MWC' : 'Endcap C-side (-2.0 < eta < -1.05)', 'MWA' : 'Endcap A-side (1.05 < eta < 2.0)'}


def get_means(lines):
    """ Returns an array of means computed in different ways
    mz0 = 89.932
    mz = 87.566 +/- 0.999
    CHI2/NDF = 0.48, NDF = 8
    R = 0.974 +/- 0.008
    k+ = 96.071 +/- 1.195
    k- = 98.684 +/- 1.163
    OR:
    k+ = 100.969 +/- 0.303 ( 0.176 0.247 )
    k- = 99.257 +/- 0.298 ( 0.173 0.243 )
    """
    mz0 = float(lines[0][2])
    mz = float(lines[1][2])
    R = float(lines[3][2])
    kp = float(lines[4][2])
    ekp = float(lines[4][4])
    kn = float(lines[5][2])
    ekn = float(lines[5][4])
    try:
        ekp1,ekp2 = float(lines[4][6]),float(lines[4][7])
        ekn1,ekn2 = float(lines[5][6]),float(lines[5][7])
    except:
        ekp1,ekp2,ekn1,ekn2 = [None]*4
    return mz0,mz,R,kp,ekp,kn,ekn, ekp1,ekp2,ekn1,ekn2 #7,8,9,10

def max_pair(s):
    s1,s2=s[0],s[1]
    assert len(s1)==len(s2),'Error: wrong array sizes'
    return [max(s) for s in zip([math.fabs(a) for a in s1],[math.fabs(b) for b in s2])]

def load_all(f1,resmodel):
    res = {}
    # load the data
    for det in dets:
        res[det] = {}
        for reg in regs:
            res[det][reg] = {}
            for klu in klus:
                res[det][reg][klu] = None
                fname = pattern%(klu,fname_pat,folder_pat,det,reg,resmodel,'results.rtxt')
                fname = os.path.join(f1,fname)
                good = os.path.isfile(fname)
                if good:
                    f = open(fname,'r')
                    lines = []
                    for l in f:
                        lines.append(l.split())
                    res[det][reg][klu] = get_means(lines)
    return res

def print_syst(leg1,s1,leg2,s2):
    mysys = [],[]
    klu=''
    for det in dets:
        print r'\multicolumn{3}{|c|}{'+'%s'%dets_map[det]+r'}     \\ \hline'
        for reg in regs:
            d1=leg1[det][reg][s1]
            d2=leg2[det][reg][s2]
            s=[]
            dlP=d2[3]-d1[3]
            dlN=d2[5]-d1[5]
            mysys[0].append(dlP)
            mysys[1].append(dlN)
            s.append('%s%.2f'%('' if dlP<0 else '+',dlP))
            s.append('%s%.2f'%('' if dlN<0 else '+',dlN))
            #Endcap-A  &   $+0.05\%$  &  $-0.05\%$     \\ \hline
            print '%s'%regs_map[reg]+r'  &   $' + '%s'%s[0]+r'\%$ & $'+'%s'%s[1]+r'\%$   \\ \hline'
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


default=load_all(ddefault,resmodel)
narrow=load_all(dnarrow,resmodel)
chi=load_all(dchi,resmodel)
egge=load_all(ddefault,'egge3')

print
print 'RESULTS AND STATISTICAL UNCERTAINTIES'
stat1=[],[]  #stat due to mz
stat2=[],[]  #stat due to R0
if True:
    klu=''
    for det in dets:
        print r'\multicolumn{3}{|c|}{'+'%s'%dets_map[det]+r'}     \\ \hline'
        for reg in regs:
            s=default[det][reg][klu]
            #ekp1,ekp2,ekn1,ekn2 #7,8,9,10
            stat1[0].append(s[7]) #k+
            stat1[1].append(s[9]) #k-
            stat2[0].append(s[8])
            stat2[1].append(s[10])
            #Endcap-A  &   $100.1 \pm 0.4\%$  &  $    100.0 \pm 0.4\%$     \\ \hline
            pho1='    ' if s[3]>100.0 else '\pho '
            pho2='    ' if s[5]>100.0 else '\pho '
            print '%s'%regs_map[reg]+r'  &   $' + '%s%.2f'%(pho1,s[3])+r' \pm '+'%.2f'%s[4]+r'\%$  &  $    '+'%s%.2f'%(pho2,s[5])+r' \pm '+'%.2f'%s[6]+r'\%$     \\ \hline'
        print r'\hline'


print
print 'SYSTEMATIC KLU'
sys1 = print_syst(default,'',default,'KLU_')

print
print 'SYSTEMATIC Z MASS WINDOW'
sys2 = print_syst(default,'',narrow,'')

print 
print 'SYSTEMATIC KS vs CHI2'
sys3 = print_syst(default,'',chi,'')

print 
print 'SYSTEMATIC GAUS vs EGGE'
sys4 = print_syst(default,'',egge,'')

print
print 'Final systematic table'

print r'$R_0$ determination & ' + ' & '.join(['%.2f'%s for s in max_pair(stat2)])+r'\\'
print r'             Z mass & ' + ' & '.join(['%.2f'%s for s in max_pair(stat1)])+r'\\'
print 'Expanded sample & ' + ' & '.join(['%.2f'%s for s in max_pair(sys1)])+r'\\'
print 'Mass window for Z selection & ' + ' & '.join(['%.2f'%s for s in max_pair(sys2)])+r'\\'
print 'Shape comparison method & ' + ' & '.join(['%.2f'%s for s in max_pair(sys3)])+r'\\'
print 'Z mass fit function & ' + ' & '.join(['%.2f'%s for s in max_pair(sys4)])+r'\\'
print
print r'     Correlated (constant $k_+/k_-$) & ' + ' & '.join(['%.2f'%s for s in sumsq([max_pair(stat1),max_pair(sys4)])]) + r'\\'
print r'Anti-correlated (constant $k_+ k_-$) & ' + ' & '.join(['%.2f'%s for s in sumsq([max_pair(stat2),max_pair(sys1),max_pair(sys2),max_pair(sys3)])]) + r'\\'

