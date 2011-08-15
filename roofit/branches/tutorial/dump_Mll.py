#!/usr/bin/env python

"""
Dumps a series of Mll plots before/after
"""

import sys
import ROOT
ROOT.gROOT.SetBatch(True)

dbname='out0813'
folds= ['default','MCP'] + ['closure_8140%d'%i for i in xrange(3)]
tts=['cmb','id','exms']
regs=['AA','BB','CC','BA','AB','BC','CB']
gbg = []
# zkolmogorov/fold/cmb/BB/zkolmogorov
pattern = 'zkolmogorov/%s/%s/%s/zkolmogorov'

f = ROOT.TFile.Open(dbname+'.root')

iz = 0
for fold in folds[1:]:
    for tt in tts:
        for reg in regs:
            print fold,tt,reg
            c_def=f.Get(pattern%(folds[0],tt,reg))
            c_stu=f.Get(pattern%(fold,tt,reg))
            p_def = [v for v in c_def.GetListOfPrimitives()]
            p_stu = [v for v in c_stu.GetListOfPrimitives()]
            c = ROOT.TCanvas('c%s_%s_%s'%(fold,tt,reg),'c%s_%s_%s'%(fold,tt,reg),1200,600)
            c.Divide(2)
            c.cd(1)
            [p.Draw() for p in p_def]
            c.cd(2)
            [p.Draw() for p in p_stu]
            c.SaveAs('AUG15_%s_%s_%s.png'%(fold,tt,reg))
