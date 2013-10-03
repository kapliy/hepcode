#!/usr/bin/env python

import sys,os
if not 'libPyRoot' in sys.modules: #hack to get rid of TenvRec warnings
    from ROOT import gROOT
    sys.modules['libPyROOT'].gROOT.GetListOfGlobals().FindObject('gErrorIgnoreLevel').GetAddress().__setitem__(0,3000)
    import ROOT
    gErrorIgnoreLevel=ROOT.kInfo
    sys.modules['libPyROOT'].gROOT.GetListOfGlobals().FindObject('gErrorAbortLevel').GetAddress().__setitem__(0,5001)

f = ROOT.TFile.Open('mg_to_muid_rw.root','UPDATE')

for q in ('POS','NEG'):
    for d in (0,1):
        hmuid = f.Get('%s_%dD_MUID'%(q,d))
        hmgmg = f.Get('%s_%dD_MGMG'%(q,d))
        hp = hmuid.GetName().split('_')
        hratio = hmuid.Clone( 'rw_'+hp[0]+'_'+hp[1] )
        hratio.Divide(hmgmg)
        hratio.SetTitle(hratio.GetName())
        hratio.Write(hratio.GetName(),ROOT.TObject.kOverwrite)

f.Close()
