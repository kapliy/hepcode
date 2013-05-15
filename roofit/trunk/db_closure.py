#!/usr/bin/env python

"""
Validates scale factors computed from out1113mcpupdate.root
and applied in out1122v27closure.root
>>> a.ls('/zpeak/')
/zpeak/r17_default_staco
/zpeak/r17_default_staco_dataKC
/zpeak/r17_default_staco_datakpkm
/zpeak/r17_default_staco_mcKC
/zpeak/r17_default_staco_mcdefault
/zpeak/r17_default_staco_mckpkm
/zpeak/r17_m80100_staco
"""

import antondb,math,sys
db = 'out1122v27closure'
a = antondb.antondb(db)
a.load()

def ordered_pairs(lst):
    """ input: [a1,a2,a3]
        output: [a1,a2], [a2,a3]
    """
    return zip(lst,lst[1:])
boundsP = [0.30,0.60,0.9,1.30,1.60,2.00,2.50] #[TxT]
boundsP = [0.50,1.05,1.70,2.51] #[SxS]
boundsA = [-z for z in reversed(boundsP)] + [0.00,] + boundsP
bounds = ordered_pairs(boundsA)
regsR = range(0,len(bounds))
regs_map = {}
for bn in regsR:
    bb = bounds[bn]
    regs_map['S%dS'%bn] = r'$%.1f < \eta < %.1f$'%(bb[0],bb[1])
dets_map = {'cmb' : 'Combined muons', 'id' : 'ID muons', 'exms' : 'MS muons'}

MZ0=91.1876

def s(v):
    return v['data_mz']/v['mc_mz']*100.0
def d(v):
    return v['data_mz']
def m(v):
    return v['mc_mz']
def ed(v):
    return v['data_emz']
def em(v):
    return v['mc_emz']

for det in ['cmb','exms','id']:
    print '====================='
    print dets_map[det]
    print '---------------------'
    for i,reg in enumerate(['S%dS'%zz for zz in range(0,8)]):
        vdef = a.data['/zpeak/r17_default_staco/%s/%s/gaus0'%(det,reg)]
        vKCdata = a.data['/zpeak/r17_default_staco_dataKC/%s/%s/gaus0'%(det,reg)]
        vkpkmdata = a.data['/zpeak/r17_default_staco_datakpkm/%s/%s/gaus0'%(det,reg)]
        vKCmc = a.data['/zpeak/r17_default_staco_mcKC/%s/%s/gaus0'%(det,reg)]
        vkpkmmc = a.data['/zpeak/r17_default_staco_mckpkm/%s/%s/gaus0'%(det,reg)]
        vdefaultmc = a.data['/zpeak/r17_default_staco_mcdefault/%s/%s/gaus0'%(det,reg)]
        pad1 = '' if i<=2 else (' ' if i==3 else '  ')
        pad2 = '' if float(s(vdef))>=100.0 else ' '
        pad3 = '' if float(s(vKCmc))>=100.0 else ' '
        if True: # main closure result
            print regs_map[reg],pad1,'\t\t','%.2f%%'%s(vdef),pad2,'\t','%.2f%%'%s(vKCmc),pad3,'\t','%.2f%%'%s(vdefaultmc)
        else:     # comparison with true z mass
            pad15 = '' if float(m(vdef))>=100.0 else ' '
            pad16 = '' if float(m(vdef))-MZ0<0 else ' '
            pad2 = '' if float(d(vdef))>=100.0 else ' '
            pad3 = '' if float(m(vKCmc))>=100.0 else ' '
            #errt=math.sqrt(ed(vdef)**2 + em(vKCmc)**2)
            errt=math.sqrt(ed(vdef)**2 + 0)
            pad4 = '' if float(errt)>=100.0 else ' '
            #print regs_map[reg],pad1,'\t\t','%.2f'%d(vdef),pad2,'\t','%.2f'%m(vKCmc),pad3,'\t','%.2f'%errt,pad4,'%.2f'%(m(vKCmc)-MZ0)
            print regs_map[reg],pad1,'\t\t','%.2f'%m(vdef),pad15,'\t','%.2f'%(m(vdef)-MZ0),pad16,'\t\t','%.2f'%d(vdef),pad2,'\t','%.2f'%(d(vdef)-MZ0)
