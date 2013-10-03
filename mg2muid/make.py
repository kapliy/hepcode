#!/usr/bin/env python

""" MuGirl vs MuID trigger and W cross-section """

import sys,os
if not 'libPyRoot' in sys.modules: #hack to get rid of TenvRec warnings
    from ROOT import gROOT
    sys.modules['libPyROOT'].gROOT.GetListOfGlobals().FindObject('gErrorIgnoreLevel').GetAddress().__setitem__(0,3000)
    import ROOT
    gErrorIgnoreLevel=ROOT.kInfo
    sys.modules['libPyROOT'].gROOT.GetListOfGlobals().FindObject('gErrorAbortLevel').GetAddress().__setitem__(0,5001)

assert len(sys.argv)==2,'USAGE: %s filename.root'%sys.argv[0]
fin = sys.argv[1]
assert os.path.isfile(fin)
fina = os.path.splitext(fin)
fbase = fina[0]
fout = 'mg_to_muid_rw.root'

def make_habseta(name,xsec,TYPE=None):
    """ makes an instance of abseta histogram with detector-motivated binning """
    import array
    assert TYPE in (0,1)
    bins = [0.0,0.21,0.42,0.63,0.84,1.05,1.37,1.52,1.74,1.95,2.18,2.4] if TYPE==1 else [0.0,2.4]
    a = array.array('d',bins)
    out =  ROOT.TH1D(name,name,len(a)-1,a)
    assert out.GetNbinsX() == len(xsec)
    for ibin in xrange(1,out.GetNbinsX()+1):
        out.SetBinContent(ibin,xsec[ibin-1])
        #print ibin,xsec[ibin-1]
        out.SetBinError(ibin,0)
    return out

TYPE = None
XSEC = []
for rl in open(fin):
    rl = rl.strip()
    l = rl.translate(None,'$<').split()
    if len(l)<13: continue
    if TYPE==None and len(l)==13: TYPE = 0
    elif TYPE==None and len(l)==16: TYPE = 1
    #print l
    XSEC.append( float(l[4]) if TYPE==1 else float(l[1]) )

h = make_habseta(fbase,XSEC,TYPE)

print fbase,' '.join(['%.1f'%x for x in XSEC])
f = ROOT.TFile.Open(fout,'UPDATE')
f.cd()
h.Write(fbase,ROOT.TObject.kOverwrite)
f.Close()
