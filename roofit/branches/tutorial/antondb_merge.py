#!/usr/bin/env python

""" 
A simple tool to merge a bunch of antondb's
"""

import antondb
import sys,glob

assert len(sys.argv)>=3,'Arguments: save_name glob_pattern'
outname = sys.argv[1]
patterns = sys.argv[2:]

files1 = []
for pattern in patterns:
    files1 += glob.glob(pattern)

files = [ff[:-3] for ff in files1] # strip .db
assert len(files)>0,'Nothing to merge...'
print 'Merging',len(files),'files'

a = antondb.antondb(outname)
a.merge(files,outname)
print 'DONE'
