#!/usr/bin/env python

"""
Quick script to dump scales
"""
import sys,math
import antondb

dbname = 'out0706'
pattern_R0 = '/klu/%s/%s/R0' #'/default/cmb/BB/R0'
pattern_Z  = '/default/%s/%s/Z/egge3'  #'/default/cmb/BB/Z'
dets = ['cmb','exms','id']

a = antondb.antondb(dbname)
a.load()

def scales(R,eR,mz,emz,mz0):
    sqrt=math.sqrt
    km = mz/(mz0*sqrt(R))
    kp = mz*sqrt(R)/(mz0)
    ekm = sqrt( emz**2 * 1/(mz0*sqrt(R))**2 + eR**2 * (mz/mz0)**2 / (4 * R**3) )
    ekp = sqrt( emz**2 * (sqrt(R)/mz0)**2 + eR**2 * (mz/mz0)**2 / (4 * R) )
    ekm1,ekm2 = sqrt(emz**2 * 1/(mz0*sqrt(R))**2),sqrt(eR**2 * (mz/mz0)**2 / (4 * R**3))
    ekp1,ekp2 = sqrt(emz**2 * (sqrt(R)/mz0)**2), sqrt(eR**2 * (mz/mz0)**2 / (4 * R))
    return kp*100.0,km*100.0

def get_scales(regs=['AA','BB','CC']):
    result=[]
    for det in dets:
        for reg in regs:
            aR0 = a.data[pattern_R0%(det,reg)]
            aZ = a.data[pattern_Z%(det,reg)]
            assert aR0
            assert aZ
            pos,neg = scales(aR0['ksf'],aR0['chie'],aZ['data_mz'],aZ['data_emz'],aZ['mc_mz'])
            result.append('%.2f'%pos)
            result.append('%.2f'%neg)
    return result

# create c++ array of scales
coarse = get_scales(['AA','BB','CC'])
fine = get_scales(['FWA','MWA','Baa','Bcc','MWC','FWC'])

print '========================='
print 'COARSE:'
print '========================='
for i,j in enumerate(coarse):
    print j,
    if i%2!=0:
        print ' , ' if i!=len(coarse)-1 else ''
    else:
        print ' , ',
print '========================='

print '========================='
print 'FINE:'
print '========================='
for i,j in enumerate(fine):
    print j,
    if i%2!=0:
        print ' , ' if i!=len(fine)-1 else ''
    else:
        print ' , ',
print '========================='
