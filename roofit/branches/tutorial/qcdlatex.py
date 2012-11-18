#!/usr/bin/env python

"""
Makes a latex table of:
eta_range   data \pm err   ew \pm err   qcd \pm err    [CW - not included yet!]
"""

import os,sys,re,math

fname = 'OUT_11022012_ALL.v2.abseta.1D.root'

import common
import ROOT
import binning
bins = binning.absetabins

qs='POS'
if len(sys.argv)>1:
    qs = sys.argv[1]
assert qs in ('POS','NEG')

if len(sys.argv)>2:
    fname = sys.argv[2]

NDATA = 'data'
NQCD = 'qcd'
NEWK = 'ewk'
# these are unused
NBG = 'totalbg'
NSIG = 'wmunu_PowhegPythia'

f = ROOT.TFile.Open(fname,'READ')
assert f.IsOpen()

# EWK systematics list
EWK = []
EWK.append( ['Nominal'] )
EWK.append( ['MuonResMSUp','MuonResMSDown'] )
EWK.append( ['MuonResIDUp','MuonResIDDown'] )
EWK.append( ['MuonScaleKUp','MuonScaleKDown' ] )
EWK.append( ['MuonScaleCUp','MuonScaleCDown' ] )
EWK.append( ['MuonRecoSFUp','MuonRecoSFDown'] )
EWK.append( ['MuonTriggerSFUp','MuonTriggerSFDown'] )
#EWK.append( ['MuonIsoSFUp','MuonIsoSFDown'] )
EWK.append( ['JetResolUp','JetResolDown'] )
EWK.append( ['JetScaleUp','JetScaleDown'] )
EWK.append( ['ResoSoftTermsUp_ptHard','ResoSoftTermsDown_ptHard'] )
EWK.append( ['ScaleSoftTermsUp_ptHard','ScaleSoftTermsDown_ptHard'] )
EWK.append( ['Nominal_ewk_xsecdown','Nominal_ewk_xsecup'] )
EWK.append( ['Nominal_unfoldPowhegJimmy', 'Nominal_unfoldMCNLO'] )
#TODO: substitute the following with proper PDF reweighting?
#EWK.append( ['Nominal_ewk_alpgen'] )

# QCD systematics list (no need to correlate them here!)
QCD = []
QCD.append( ['MCAverage'] ) # average of 3 generators!
QCD.append( ['PowhegPythia','PowhegHerwig','McAtNlo'] )

def get(x):
    """ retrieves one histogram and makes sure it is valid """
    nm = '%s/%s'%(qs,x)
    tmp = f.Get(nm)
    if not tmp:
        print 'ERROR: cannot find histogram',nm
        os._exit(0)
    return tmp

def getStatSys(NSYS,SYS):
    """ returns both statistical and systematic histograms """
    nom = get(NSYS + '_' + SYS[0][0])
    sys = nom.Clone(nom.GetName()+'_sys')
    # zero out stat. error in sys, since it will contain systematic ONLY
    [sys.SetBinError(ii,0) for ii in xrange(0,sys.GetNbinsX()+2)]
    for hss in SYS[1:]:
        bdiffs = [[] for z in xrange(0,sys.GetNbinsX()+2)]
        # loop over systematics in this group
        for hs in hss:
            h = get(NSYS + '_' + hs)
            for ibin in xrange(0,sys.GetNbinsX()+2):
                bdiffs[ibin].append ( abs(nom.GetBinContent(ibin)-h.GetBinContent(ibin)) )
        # loop over bins and add new error (max. deviation) in quadrature
        # in other words, we symmetrize up/down deviations here
        for ibin in xrange(0,sys.GetNbinsX()+2):
            newerr = max(bdiffs[ibin]) if len(bdiffs[ibin])>0 else 0
            # sys
            olderr = sys.GetBinError(ibin)
            sys.SetBinError(ibin,1.0*math.sqrt(olderr*olderr + 1.0*newerr*newerr))
    return nom,sys

def pho(c,m):
    """ returns the correct amount of whitespace \pho """
    phox = '\\pho'
    npho = 0
    cl = int( math.log10(c) )
    ml = int( math.log10(m) )
    npho = ml - cl if ml>cl else 0
    return phox*npho + ' '

def line(i,data,ewk,qcd):
    """
    $0.00..0.21$       & $11663 \pm 108$       & $420.7 \pm 8.4 \pm 22.0$      & $250.6 \pm 17.0 \pm 33.5$     & $0.6830 \pm 0.0069 \pm 0.0143$\\
    """
    low = '%.2f'%(data.GetBinLowEdge(i))
    high = '%.2f'%(data.GetBinLowEdge(i)+data.GetBinWidth(i))
    maxd = max( [data.GetBinContent(ii) for ii in xrange(1,data.GetNbinsX()+1)] )
    maxd1 = max( [data.GetBinError(ii) for ii in xrange(1,data.GetNbinsX()+1)] )
    maxe = max( [ewk[0].GetBinContent(ii) for ii in xrange(1,ewk[0].GetNbinsX()+1)] )
    maxe1 = max( [ewk[0].GetBinError(ii) for ii in xrange(1,ewk[0].GetNbinsX()+1)] )
    maxe2 = max( [ewk[1].GetBinError(ii) for ii in xrange(1,ewk[0].GetNbinsX()+1)] )
    maxq = max( [qcd[0].GetBinContent(ii) for ii in xrange(1,qcd[0].GetNbinsX()+1)] )
    maxq1 = max( [qcd[0].GetBinError(ii) for ii in xrange(1,qcd[0].GetNbinsX()+1)] )
    maxq2 = max( [qcd[0].GetBinError(ii) for ii in xrange(1,qcd[0].GetNbinsX()+1)] )
    print '$%s..%s$'%(low,high),
    print '     & ',
    def d(v,m): return '%s%d'%(pho(v,m),v)
    def f(v,m): return '%s%.1f'%(pho(v,m),v)
    print '$%s'%d(data.GetBinContent(i),maxd),'\pm','%s$'%(d(data.GetBinError(i),maxd1)),
    print '     & ',
    print '$%s'%f(ewk[0].GetBinContent(i),maxe),'\pm','%s'%f(ewk[0].GetBinError(i),maxe1),'\pm','%s$'%f(ewk[1].GetBinError(i),maxe2),
    print '     & ',
    print '$%s'%f(qcd[0].GetBinContent(i),maxq),'\pm','%s'%f(qcd[0].GetBinError(i),maxq1),'\pm','%s$'%f(qcd[1].GetBinError(i),maxq2),
    print '\\\\'

if __name__ == '__main__':
    data = get(NDATA)
    ewk = getStatSys(NEWK,EWK)
    qcd = getStatSys(NQCD,QCD)
    for i in xrange(1,data.GetNbinsX()+1):
        line(i,data,ewk,qcd)
