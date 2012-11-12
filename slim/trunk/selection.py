#!/usr/bin/env python

_PTMIN = 13
_GRLNAME = '/home/antonk/TrigFTKAna/good_run_lists/asym_data11_7TeV.pro10.DtoM.xml'
_GRL = None

def make_grl():
    global _GRL
    if _GRL: return
    import ROOT
    ROOT.gSystem.Load('/home/antonk/TrigFTKAna/CommonAnalysis/GoodRunsLists/StandAlone/libGoodRunsLists.so')
    grlR = ROOT.Root.TGoodRunsListReader()
    grl = ROOT.Root.TGoodRunsList()
    grlR.SetXMLFile(_GRLNAME)
    res = grlR.Interpret()
    assert res,'Failed to interpret GRL'
    grl = grlR.GetMergedGoodRunsList()
    _GRL = grl

def filter_fct(t):
    trig = ('EF_mu18_MG_medium' in t.trig_ef) or ('EF_mu18_MG' in t.trig_ef)
    if not trig: return False
    return True
    GRL = make_grl()
    grl = GRL.HasRunLumiBlock(t.run,t.lb)
    if not grl: return False
    return True
