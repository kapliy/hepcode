#!/usr/bin/env python

import os,sys
if not 'libPyRoot' in sys.modules: #hack to get rid of TenvRec warnings
    from ROOT import gROOT
    sys.modules['libPyROOT'].gROOT.GetListOfGlobals().FindObject('gErrorIgnoreLevel').GetAddress().__setitem__(0,3000)
    import ROOT
    gErrorIgnoreLevel=ROOT.kInfo
    sys.modules['libPyROOT'].gROOT.GetListOfGlobals().FindObject('gErrorAbortLevel').GetAddress().__setitem__(0,5001)

# a few functions to manipulate QCD fit results saved via antondb in the MSYS/MGROUPS format
def qcdfit_sys_deviations(MSYS,MGROUPS,idx=6):
    """ Returns an array of systematic deviations from nominal
    If idx=6, this computes deviations for QCD, and thus includes a special fit error category
    [0]    [1]     [2]       [3]  [4]  [5] [6]       [7]        [8] [9]    [10]
    [name,  ndata,ndatasub,  ntot,nsig,newk,nqcd,  nqcd*relerr, chi2,ndf,   iref]
    """
    res = []
    assert len(MSYS[0])==1
    # loop over all groups, beyond Nominal
    for IGRP in MSYS[1:]:
        devs = []
        for INAME,ISYS in IGRP.iteritems():
            idx_nom = ISYS[-1] # normaly this is zero, except for PS/ME error
            NOM = MSYS[idx_nom].values()[0]
            devs.append( abs(   ISYS[idx]-NOM[idx]   ) )
        res.append(  max(devs)  )
    assert len(MGROUPS)-1 == len(res)
    return res

# other stuff
def dump_pickle(data,name='data.pkl'):
    import pickle,FileLock
    with FileLock.FileLock(name):
        output = open(name, 'wb')
        pickle.dump(data,output)
        output.close()

def dexit(v=0):
    sys.stdout.flush()
    os._exit(v)

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
