#!/usr/bin/env python

import glob,sys
import ROOT
"""
Finds maximum njet nmu etc
"""

vars = ['nmu','njet','nph','nele','nvx','nmc','ntrk','njpsi']
families=[ 'pythia_*', 'jimmy_*', 'acer_*', 'mcnlo_*', 'herwig_*', 'powheg_*', 'sherpa_*' , '[B-M]' ]

#vars = ['mu_trig_l1matchvector_','mu_trig_l2matchvector_','mu_trig_l2samatchvector_','mu_trig_efmatchvector_']

res = {}
for var in vars:
    res[var] = {}
    
for fam in families:
    for sample in sorted(glob.glob(fam)):
        files = ['root://uct3-xrd.mwt2.org/'+f.strip() for f in open(sample) if f.strip()]
        nt = ROOT.TChain('tree','tree')
        [nt.AddFile(f) for f in files]
        nfiles = len(files)
        for var in vars:
            print 'Working on: %s %s (%d files)'%(sample,var,nfiles)
            sys.stdout.flush()
            res[var][sample] = nt.GetMaximum(var)
            print '----> SAMPLE %s VAR %s: %d'%(sample,var,res[var][sample])
            sys.stdout.flush()

for var in vars:
    print '====> MAX %s : %d'%(var,max([v for v in res[var] ]))
