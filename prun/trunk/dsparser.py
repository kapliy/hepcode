#!/usr/bin/env python
import re

"""
subst ordering:
%d - reg
%d %d - reg_sub
%d %d %d - grp_reg_sub
%d %d %d %d - grp_reg_sub_pat
""" 

files11 = []
for i in xrange(8):
    for j in xrange(4):
        files11.append('sectors_raw_11L_reg%d_sub%d.patt.bz2'%(i,j))
        files11.append('corrgen_raw_11L_reg%d_sub%d.gcon.bz2'%(i,j))

datasets = "user.kapliy.base11L.06062010:sectors*reg%REG_sub%SUB*.patt:corrgen*reg%REG_sub%SUB*.gcon"

# change all %REG to reg, %SUB to sub

iRegion=1
iSubregion=3

datasets=re.sub('%REG',str(iRegion),datasets)
datasets=re.sub('%SUB',str(iSubregion),datasets)
datasets=re.sub('\.','\.',datasets)
datasets=re.sub('\*','.*',datasets)

print datasets
patterns = datasets.split(':')
patterns = patterns[1:] if len(patterns)>1 else []
for f in files11:
    for pattern in patterns:
        if re.search(pattern,f):
            print f
            break
