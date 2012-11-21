#!/usr/bin/env python

""" A script to repackage histograms and perform proper QCD averaging """

import sys,os,re,math,copy
import antondb
import common
import ROOT

class histoQCD:
    """ A QCD histogram container - performs average+rms over multiple systematics
    NOTE: this class is not responsible for setting the values of each histogram
    """
    def __init__(s):
        s.data = [] # TH1 objects
        s.memo = [] # descriptions
    def add0(s,h,memo=''):
        """ Add a histogram directly """
        s.data.append(h.Clone())
        s.memo.append(memo)
    def add1(s,h,scale=1.0,memo=''):
        """ Add a histogram with a global scale for all pt/eta bins """
        hh = h.Clone()
        hh.Scale(scale)
        s.data.append(hh)
        s.memo.append(memo)
    def add2(s,h,scales={},memo=''):
        """ Add a histogram with a separate scale in each pt/eta bin """
        hh = h.Clone()
        for ibin in xrange( 0 , hh.GetNbinsX()+1 ):
            if ibin in scales:
                v = scales[ibin]
                hh.SetBinContent(ibin, hh.GetBinContent(ibin)*v)
                hh.SetBinError(ibin, hh.GetBinError(ibin)*v)
        s.data.append(hh)
        s.memo.append(memo)
    def mean(s,x):
        if len(x)==0: return 0
        return sum(x)/len(x)
    def rms(s,x):
        if len(x)==0: return 0
        m = mean(x)*1.0
        return math.sqrt( sum([ (xx-m)**2 for xx in x])/len(x) )
    def getH(s,sigma=0):
        """ Get a mean histogram with 1-sigma up or down variation """
        if len(s.data)==0: return None
        nom = s.data[0]
        res = nom.Clone()
        res.Reset()
        # for each bin, perform the averaging
        for ibin in xrange( 0 , res.GetNbinsX()+1 ):
            x = [ h.GetBinContent(ibin) for h in s.data ]
            res.SetBinContent(ibin , s.mean(x)+sigma*s.rms(x) )
            res.SetBinError( ibin , nom.GetBinError(ibin) ) # statistical error only - from nominal case
        return
        

DIM = 2  # 2d fits in pt x eta
if len(sys.argv)>=2 and sys.argv[1]=='1':
    DIM = 1  # 1d fits in eta only
print 'DIMENSION:',DIM
ETAMODE = 2 # use |eta|
if len(sys.argv)>=3 and sys.argv[2]=='1':
    ETAMODE = 1 # use eta
print 'ETAMODE:',ETAMODE
eword = 'eta' if ETAMODE==1 else 'abseta'

import binning
etabins = binning.setabins if ETAMODE==1 else binning.absetabins # N=12
ptbins = binning.ptbins # N=8

db_name = 'DB_11022012_ALL'
fin_name = 'IN_11022012_ALL.v1.%s.%dD.root'%(eword,DIM)

if __name__ == '__main__':
    fout_name = re.sub('IN_','OUT_',fin_name)
    assert os.path.exists(fin_name)
    print 'ROOT repackaging: %s -> %s'%(fin_name,fout_name)
    fin,fout = None,None
    fout_D = []
    fin = ROOT.TFile.Open(fin_name,"READ")
    fout = ROOT.TFile.Open(fout_name,"RECREATE")
    fout_D.append(fout.mkdir('POS'))
    fout_D.append(fout.mkdir('NEG'))
    fin.Close()
    fout.Close()
