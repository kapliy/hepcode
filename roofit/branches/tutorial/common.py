#!/usr/bin/env python

import os,sys
if not 'libPyRoot' in sys.modules: #hack to get rid of TenvRec warnings
    from ROOT import gROOT
    sys.modules['libPyROOT'].gROOT.GetListOfGlobals().FindObject('gErrorIgnoreLevel').GetAddress().__setitem__(0,3000)
    import ROOT
    gErrorIgnoreLevel=ROOT.kInfo
    sys.modules['libPyROOT'].gROOT.GetListOfGlobals().FindObject('gErrorAbortLevel').GetAddress().__setitem__(0,5001)


# a few functions to manipulate QCD fit results saved via antondb in the MSYS/MGROUPS format
def qcdfit_sys_deviations_subset(MSYS,MGROUPS,allowed,idx=11):
    """ Returns an array of systematic deviations from nominal
    #       [0]    [1]     [2]     [3]     [4]         [5] [6]   [7] [8]   [9]  [10]  [11] [12]     [13]         [14] [15]   [16]
    return [name,  ndata,ndataE,ndatasub,ndatasubE,  ntot,ntotE,nsig,nsigE,newk,newkE,nqcd,nqcdE,   delqcd     , chi2,ndf,   iref]
    """
    res = []
    assert len(MSYS[0])==1
    # loop over all groups, beyond Nominal
    for IGRP in MSYS[1:]:
        devs = []
        for INAME,ISYS in IGRP.iteritems():
            if INAME not in allowed: continue
            idx_nom = ISYS[-1] # normaly this is zero, except for PS/ME error
            NOM = MSYS[idx_nom].values()[0]
            devs.append( abs(   ISYS[idx]-NOM[idx]   ) )
        res.append(  max(devs) if len(devs)>0 else 0.0 )
    assert len(MGROUPS)-1 == len(res)
    return res
def qcdfit_sys_deviations(MSYS,MGROUPS,idx=11):
    """ 
    shortcut to qcdfit_sys_deviations_subset() that enables ALL systematics
    """
    allowed = []
    for IGRP in MSYS[1:]:
        for INAME,ISYS in IGRP.iteritems():
            allowed.append(INAME)
    return qcdfit_sys_deviations_subset(MSYS,MGROUPS,allowed,idx)
def qcdfit_sys_flatten(MSYS,MGROUPS):
    """ Converts the list-of-dics into a plan dic """
    res = {}
    for IGRP in MSYS:
        for INAME,ISYS in IGRP.iteritems():
            assert INAME not in res
            res[INAME] = ISYS
    return res


# other stuff
def dump_pickle(data,name='data.pkl'):
    import pickle,FileLock
    with FileLock.FileLock(name):
        output = open(name, 'wb')
        pickle.dump(data,output)
        output.close()

def dump_plot(h,name='SYS',titles=[],fmode='RECREATE',opts='',title_common='',do_plot=True,do_ratio=False):
    """ A generic mini-function to plot / save a collection of TObjects """
    o = []
    if isinstance(h,list) or isinstance(h,tuple):
      o = h
    else:
      o = [h,]
    assert len(o)>0
    if len(titles)>0:
      assert len(titles)==len(h)
    else:
      for oo in o:
        titles.append(oo.GetTitle())
    if do_ratio:
        assert len(h)==2,'dump_plot: do_ratio can only be applied to a list of TWO histograms, not %d'%len(h)
        hratio = h[0].Clone(h[0].GetName()+'_ratio')
        hratio.Divide(h[1])
        o.append(hratio)
        titles.append( titles[0] + '_OVER_' + titles[1] )
    titles = [ title_common+tt for tt in titles ]
    # save in ROOT
    import FileLock
    with FileLock.FileLock(name+'.root'):
        f = ROOT.TFile.Open(name+'.root',fmode)
        f.cd()
        [ oo.Write(titles[i]) for i,oo in enumerate(o) ]
        f.Close()
    # save as a plot
    if do_plot:
        c = ROOT.TCanvas(name,name,800,600)
        c.cd()
        [ oo.SetLineColor(i+1) for i,oo in enumerate(o) ]
        o[0].Draw(opts)
        [ oo.Draw('A SAME %s'%opts) for oo in o[1:] ]
        maxh = max([htmp.GetMaximum() for htmp in o])*1.3
        o[0].GetYaxis().SetRangeUser(0,maxh)
        c.SaveAs(name+'.png')

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
