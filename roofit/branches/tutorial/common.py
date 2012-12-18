#!/usr/bin/env python

import os,sys
if not 'libPyRoot' in sys.modules: #hack to get rid of TenvRec warnings
    from ROOT import gROOT
    sys.modules['libPyROOT'].gROOT.GetListOfGlobals().FindObject('gErrorIgnoreLevel').GetAddress().__setitem__(0,3000)
    import ROOT
    gErrorIgnoreLevel=ROOT.kInfo
    sys.modules['libPyROOT'].gROOT.GetListOfGlobals().FindObject('gErrorAbortLevel').GetAddress().__setitem__(0,5001)

def dump_pickle(data,name='data.pkl'):
    import pickle,FileLock
    with FileLock.FileLock(name):
        output = open(name, 'wb')
        pickle.dump(data,output)
        output.close()

def handle_pdb(sig, frame):
        import pdb
        pdb.Pdb().set_trace(frame)

def xflatten(seq):
    """a generator to flatten a nested list"""
    for x in seq:
        if type(x) is list:
            for y in xflatten(x):
                yield y
        else:
            yield x

def rand_name(ln=10):
  import random,string
  return ''.join(random.choice(string.ascii_uppercase + string.digits) for x in range(ln))

def SetStyle(styleMacro=''):
    """ Global style settings for ROOT """
    if styleMacro=='':
        ROOT.gStyle.SetCanvasColor(0);
        ROOT.gStyle.SetCanvasBorderMode(0);
        ROOT.gStyle.SetPadColor(0);
        ROOT.gStyle.SetPadBorderMode(0);
        ROOT.gStyle.SetStatColor(0);
        #ROOT.gStyle.SetOptStat(111111);
        ROOT.gStyle.SetOptStat(0);
        ROOT.gStyle.SetOptFit(1111);
        ROOT.gStyle.SetHistFillColor(0);
        ROOT.gStyle.SetMarkerStyle(20);
        ROOT.gStyle.SetMarkerSize(.4);
        ROOT.gStyle.SetHistLineWidth(2);
        ROOT.gStyle.SetErrorX(0);
        
        ROOT.gStyle.SetTitleStyle(0);
        
        ROOT.gStyle.SetStatBorderSize(1);
        ROOT.gStyle.SetFrameFillColor(10);
        ROOT.gStyle.SetTitleFillColor(0);
        ROOT.gROOT.ForceStyle()
    else:
        ROOT.gROOT.LoadMacro(styleMacro);
        ROOT.SetAtlasStyle()

def MakeLegend(label='Data and MC'):
    l = ROOT.TLegend(0.55,0.70,0.88,0.88,label,"brNDC")
    l.SetFillStyle(1001)
    l.SetFillColor(10)
    return l
