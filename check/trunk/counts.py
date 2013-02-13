#!/usr/bin/env python
import os,sys,re

if not 'libPyRoot' in sys.modules: #hack to get rid of TenvRec warnings
    import ROOT
    ROOT.gROOT.SetBatch(True)
    from ROOT import gROOT
    sys.modules['libPyROOT'].gROOT.GetListOfGlobals().FindObject('gErrorIgnoreLevel').GetAddress().__setitem__(0,3000)
    gErrorIgnoreLevel=ROOT.kInfo
    sys.modules['libPyROOT'].gROOT.GetListOfGlobals().FindObject('gErrorAbortLevel').GetAddress().__setitem__(0,5001)

import sys,re,time
assert len(sys.argv)==2

def get_total(line):
    l=line
    if re.match('/atlas',l):
        l = 'root://uct3-xrd.mwt2.org/%s'%l
    if re.match('/pnfs',l):
        #l = 'dcache://%s'%l
        l = 'root://xrddc.mwt2.org:1096%s'%l
    # try up to 10 times
    for i in range(10):
        try:
            f=ROOT.TFile.Open(l);
            break
        except:
            print >>sys.stderr,'WARNING |%s| opening:'%sys.argv[1],l
            f = None
            time.sleep(2)
            continue
    if not f:
        print >>sys.stderr,'ERROR |%s| opening:'%sys.argv[1],l
        return (0,0)
    try:
        total = f.tree.GetUserInfo()[0].GetValue('nevents_total')
        saved = f.tree.GetUserInfo()[0].GetValue('nevents_saved')
    except:
        total = 0
        saved = 0
        print >>sys.stderr,'ERROR |%s|   tree:'%sys.argv[1],l
    f.Close()
    print >>sys.stderr,'INFO: opened',l
    return int(str(total)),int(str(saved))

ff = open(sys.argv[1])
TOTAL,SAVED = [],[]
i=0
for ll in ff:
    l=ll.strip()
    total,saved =  get_total(l)
    TOTAL.append(total)
    SAVED.append(saved)
    i+=1

print sys.argv[1],sum(SAVED),sum(TOTAL)
