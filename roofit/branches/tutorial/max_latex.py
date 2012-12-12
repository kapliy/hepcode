#!/usr/bin/env python

"""
Makes a latex table of:
eta_range   data \pm err   ew \pm err   qcd \pm err    [CW - not included yet!]
"""

import os,sys,re,math

fname = 'OUT_11022012_ALL.v2.abseta.1D.root'
fname = 'OUT_12102012_ALL.v2.abseta.%sD.root'

import common
import ROOT
import binning
etabins = binning.absetabins
ptbins = binning.ptbins

DIM = 1
if len(sys.argv)>1:
    DIM = int(sys.argv[1])
assert DIM in (1,2)
fname = fname%DIM

qs='POS'
if len(sys.argv)>2:
    qs = sys.argv[2]
assert qs in ('POS','NEG')


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
EWK.append( ['MuonIsoSFUp','MuonIsoSFDown'] )
EWK.append( ['MuonTriggerSFPhi',] )
EWK.append( ['MuonIsoSFUp','MuonIsoSFDown'] )
EWK.append( ['JetResolUp','JetResolDown'] )
EWK.append( ['JetScaleUp','JetScaleDown'] )
EWK.append( ['JetNPVUp','JetNPVDown'] )
EWK.append( ['JetMUUp','JetMUDown'] )
EWK.append( ['ResoSoftTermsUp_ptHard','ResoSoftTermsDown_ptHard'] )
EWK.append( ['ScaleSoftTermsUp_ptHard','ScaleSoftTermsDown_ptHard'] )
EWK.append( ['WptSherpa',] )
EWK.append( ['PdfMSTW','PdfHERA','PdfNNPDF','PdfABM'] )
EWK.append( ['Nominal_ewk_xsecdown','Nominal_ewk_xsecup'] )
EWK.append( ['Nominal_unfoldPowhegJimmy', 'Nominal_unfoldMCNLO'] ) # caveat: ewk=5 (same)
# we use alpgen, reweighted to CT10 PDF, so no need to consider it separately.
#EWK.append( ['Nominal_ewk_alpgen'] )

# SIG systematics list. Include difference between three NLO generators? For now, just use Powheg+Pythia in the tables.
# Note that we also exclude EWK xsec uncertainy for the SIG numbers
SIG=EWK[:-2]

# QCD systematics list (no need to correlate them here!)
QCD = []
QCD = EWK[:-2]
QCD.append( ['Nominal_qcd_up','Nominal_qcd_down'])
#QCD.append( ['PowhegPythia'] ) # average of 3 generators!
#QCD.append( ['PowhegHerwig','McAtNlo'] )

def get(x,py=None):
    """ retrieves one histogram and makes sure it is valid """
    nm = '%s/%s'%(qs,x)
    tmp = f.Get(nm)
    if not tmp:
        print 'ERROR: cannot find histogram',nm
        os._exit(0)
    if py!=None:
        assert DIM==2,'Projection only works in 2D'
        # d2_abseta_lpt  x=abseta, y=lpt
        ipy = int(py)
        tmp2 = tmp.ProjectionX(tmp.GetName()+'_px_%d'%py,py,py,'e')
        return tmp2
    return tmp

def getStatSys(NSYS,SYS,py=None):
    """ returns both statistical and systematic histograms """
    nom = get(NSYS + '_' + SYS[0][0],py)
    sys = nom.Clone(nom.GetName()+'_sys')
    # zero out stat. error in sys, since it will contain systematic ONLY
    [sys.SetBinError(ii,0) for ii in xrange(0,sys.GetNbinsX()+2)]
    for hss in SYS[1:]:
        bdiffs = [[] for z in xrange(0,sys.GetNbinsX()+2)]
        # loop over systematics in this group
        for hs in hss:
            h = get(NSYS + '_' + hs,py)
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

def ptword(py=None):
    res = ''
    if py==None: res += 'inclusive $p_{T}$'
    elif py==7: res+= '$p_{T} > %.2f$\, GeV'%(ptbins[py-1])
    else: res+= '$%d < p_{T} < %d$\, GeV'%(ptbins[py-1],ptbins[py])
    return res

def lineXYZ(i,data,ewk,qcd,py=None):
    """
    $0.00..0.21$       & $11663 \pm 108$       & $420.7 \pm 8.4 \pm 22.0$      & $250.6 \pm 17.0 \pm 33.5$     & $0.6830 \pm 0.0069 \pm 0.0143$\\
    """
    assert type(data) != type([]), 'Data cannot be a tuple, since it has no systematic on it!'
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

def printDataEwkQcd(py=None):
    data = get(NDATA,py)
    ewk = getStatSys(NEWK,EWK,py)
    qcd = getStatSys(NQCD,QCD,py)
    for i in xrange(1,data.GetNbinsX()+1):
        lineXYZ(i,data,ewk,qcd,py)

def printDataBgsubSig(py=None):
    data = get(NDATA,py)
    ewk = getStatSys(NEWK,EWK,py)
    qcd = getStatSys(NQCD,QCD,py)
    sig = getStatSys(NSIG,SIG,py)
    bgsub = [ data.Clone('bgsub_stat') , data.Clone('bgsub_sys') ]
    for i in xrange(2):
        bgsub[i].Add(ewk[i],-1)
        bgsub[i].Add(qcd[i],-1)
    for i in xrange(1,data.GetNbinsX()+1):
        lineXYZ(i,data,bgsub,sig,py)

def printEventComposition(py=None):
    samples = ['data' , 'wmunu_PowhegPythia_Nominal' , 'qcd_Nominal' , 'all/zmumu_Nominal' , 'all/wtaunu_Nominal' , 'all/ttbar_stop_Nominal' , 'all/ztautau_Nominal' , 'all/diboson_Nominal']
    snames = ['Data','Signal','QCD','zmumu+DrellYan','wtaunu','ttbar+stop','ztautau','dibosons']
    hs = [ get(sample,py) for sample in samples]
    ntotal = hs[0].GetNbinsX()
    nfirst = int(ntotal/2)
    nsecond = ntotal - nfirst
    print '\\begin{table}'
    print '\\centering'
    for isub in (0,1):
        nbins = nfirst if isub == 0 else nsecond
        binloop = xrange( 1 if isub==0 else nfirst+1 , nbins+1 if isub==0 else ntotal+1)
        print '\\begin{tabular}{%s}'%('c'*(nbins+1))
        #/        &  0.00--0.20  &  0.20--0.40  &  0.40--0.60  &  0.60--0.80  &  1.00--1.20  \\
        print 'Sample/$|\eta|$    ',
        for ibin in binloop:
            low = '%.2f'%(hs[0].GetBinLowEdge(ibin))
            high = '%.2f'%(hs[0].GetBinLowEdge(ibin)+hs[0].GetBinWidth(ibin))
            print '&   %s--%s   '%(low,high),
        print '\\\\'
        print '\hline'
        for i,h in enumerate(hs):
            z = [ snames[i] ]
            for ibin in binloop:
                z.append( '%d'%h.GetBinContent(ibin) if i==0 else '%.1f'%h.GetBinContent(ibin) )
            print '  &  '.join(z) + '   \\\\'
        print '\end{tabular}'
        if isub==0:
            print '\hrule'
    print '\label{tab:Wmunu_bgcomp_%s_%s}'%(qs,py)
    print '\caption{Estimated backgrounds in \Wmunu%s\ channel in $|\eta|$ bins, '%('m' if qs=='NEG' else 'p'),
    print ptword(py) + '}'
    print '\\end{table}'

if __name__ == '__main__' and False:
    #print '======== DATA-EWK-QCD table ========'
    print ''
    printDataEwkQcd()
    #print '======== DATA-BGSUB-SIG table ========'
    print ''
    printDataBgsubSig()

if __name__ == '__main__' and True:
    #print '======== Event composition ========'
    if DIM==1:
        print ''
        print ''
        printEventComposition()
    else:
        for ipt in xrange(1,len(ptbins)):
            print ''
            print ''
            printEventComposition(ipt)

