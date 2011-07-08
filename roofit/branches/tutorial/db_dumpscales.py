#!/usr/bin/env python

"""
Quick script to dump scales
"""
import sys,math
import antondb

dbname = 'out0706'
pattern_R0 = '/default/%s/%s/R0' #'/default/cmb/BB/R0'
pattern_Z  = '/default/%s/%s/Z/gaus0'  #'/default/cmb/BB/Z'
mode = int(sys.argv[1]) if len(sys.argv)>1 else 0

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

def latex_R(db,regs):
    for det in dets:
        print r'\multicolumn{3}{|c|}{'+'%s'%dets_map[det]+r'}     \\ \hline'
        for reg in regs:
            s=db[det][reg]['ksf']
            #Endcap-A  &   $100.1 \pm 0.4\%$  &  $    100.0 \pm 0.4\%$     \\ \hline
            print '%s'%regs_map[reg]+r'  &  ' + '%.2f'%(s['R'])+r'\%  &   '+'%.2f'%(s['eR'])+r'\%     \\ \hline'
        print r'\hline'

def latex_k(db,regs):
    for det in dets:
        print r'\multicolumn{3}{|c|}{'+'%s'%dets_map[det]+r'}     \\ \hline'
        for reg in regs:
            s=db[det][reg]['ksf']
            #Endcap-A  &   $100.1 \pm 0.4\%$  &  $    100.0 \pm 0.4\%$     \\ \hline
            pho1='    ' if s['kp']>100.0 else '\pho '
            pho2='    ' if s['km']>100.0 else '\pho '
            print '%s'%regs_map[reg]+r'  &   $' + '%s%.2f'%(pho1,s['kp'])+r' \pm '+'%.2f'%s['ekp']+r'\%$  &  $    '+'%s%.2f'%(pho2,s['km'])+r' \pm '+'%.2f'%s['ekm']+r'\%$     \\ \hline'
        print r'\hline'

# load everything into dictionaries
default = load_all(pattern_R0='/default/%s/%s/R0',pattern_Z='/default/%s/%s/Z/gaus0')
klu_egge = load_all(pattern_R0='/klu/%s/%s/R0',pattern_Z='/default/%s/%s/Z/egge3')

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
