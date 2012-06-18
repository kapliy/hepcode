#!/usr/bin/env python

"""
Comparison of scale factors between powheg and pythia
This is from 05/22/2012, in preparation for MCP discussion.
"""

import antondb
db = 'out2011_v29D_05222012_MCP2011'
a = antondb.antondb(db)
a.load()

# this shows that the pythia-powheg difference is covered by existing systematics (O of 0.05).
# But there is one bin, W1W, where things go slightly crazy. I assume this bin would have a large error anyways
print 'Egge combined staco'
for i in range(10):
    py_data = a.data['/zpeak/r17_default_staco/cmb/W%dW/egge3'%i]['data_mz']
    py_mc = a.data['/zpeak/r17_default_staco/cmb/W%dW/egge3'%i]['mc_mz']
    po_data = a.data['/zpeak/r17_powheg_staco/cmb/W%dW/egge3'%i]['data_mz']
    po_mc = a.data['/zpeak/r17_powheg_staco/cmb/W%dW/egge3'%i]['mc_mz']
    py_scale = py_data/py_mc*100.0
    po_scale = po_data/po_mc*100.0
    print 'bin %d: %.2f'%(i,py_scale-po_scale)

# same conclusion here:
print 'Gaus combined staco'
for i in range(10):
    py_data = a.data['/zpeak/r17_default_staco/cmb/W%dW/gaus0'%i]['data_mz']
    py_mc = a.data['/zpeak/r17_default_staco/cmb/W%dW/gaus0'%i]['mc_mz']
    po_data = a.data['/zpeak/r17_powheg_staco/cmb/W%dW/gaus0'%i]['data_mz']
    po_mc = a.data['/zpeak/r17_powheg_staco/cmb/W%dW/gaus0'%i]['mc_mz']
    py_scale = py_data/py_mc*100.0
    po_scale = po_data/po_mc*100.0
    print 'bin %d: %.2f'%(i,py_scale-po_scale)
