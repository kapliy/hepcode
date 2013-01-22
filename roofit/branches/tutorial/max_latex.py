#!/usr/bin/env python

# creates latex tables of event composition with systematics

import os,sys,re,math

fname = None
qs = 'POS'
ipt = None
if len(sys.argv)>=2:
    fname = sys.argv[1]
if len(sys.argv)>=3:
    qs = sys.argv[2]
if len(sys.argv)>=4:
    ipt = int(sys.argv[3])

assert os.path.isfile(fname),'ERROR: cannot locate file %s'%fname

DIM = 1 if re.search('1D',fname) else (2 if re.search('2D',fname) else 0)
pt  = 20 if re.search('pt20',fname) else 25

assert pt in (20,25)
assert DIM in (0,1,2)
assert qs in ('POS','NEG','ALL')
if DIM!=2:
    assert ipt==None
else:
    assert ipt

import common
import ROOT
import binning
etabins = binning.absetabins
ptbins = binning.ptbins

NDATA = 'data'
NQCD = 'qcd'
NEWK = 'ewk'
NBG = 'totalbg'
NSIG = 'wmunu'

f = ROOT.TFile.Open(fname,'READ')
assert f.IsOpen()

# EWK systematics list
# MISSING: stat. uncertainty due to scale factors; PDF eigenvectors
EWK = []
EWK.append( ['Nominal'] )
EWK.append( ['MuonResMSUp','MuonResMSDown'] )
EWK.append( ['MuonResIDUp','MuonResIDDown'] )
EWK.append( ['MuonKScaleUp','MuonKScaleDown' ] )
EWK.append( ['MuonCScaleUp','MuonCScaleDown' ] )
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
EWK.append( ['unfoldPowhegJimmy', 'unfoldMCNLO'] ) # caveat: ewk=5 (same)
EWK.append( ['Nominal_qcd_up','Nominal_qcd_down'])

def getH(x):
    """ retrieves one histogram and makes sure it is valid """
    nm = '%s/%s'%(qs,x)
    tmp = f.Get(nm)
    if not tmp:
        assert False, 'ERROR: cannot find histogram %s'%nm
    return tmp

def get(xall,py=None):
    """ retrieves one histogram and makes sure it is valid
    py performs a projection on an axis
    """
    x = None
    assert isinstance(xall,list) or isinstance(xall,tuple), 'ERROR: get() now only accepts python lists or tuples'
    x = xall
    tmp = None
    for ix,curx in enumerate(x):
        if ix==0:
            tmpix = getH(curx)
            tmp = tmpix.Clone('%s_%d'%(tmpix.GetName(),len(x)))
        else:
            tmpix = getH(curx)
            tmp.Add(tmpix)
    if not tmp:
        assert False, 'ERROR: cannot find histogram %s'%nm
    if py!=None:
        assert DIM==2,'Projection only works in 2D'
        # d2_abseta_lpt  x=abseta, y=lpt
        ipy = int(py)
        tmp2 = tmp.ProjectionX(tmp.GetName()+'_px_%d'%py,py,py,'e')
        return tmp2
    return tmp

def get2(NSYS,py=None,SYS=EWK):
    """ returns both statistical and systematic histograms """
    nom = get( [ isys + '_' + SYS[0][0] for isys in NSYS] ,py)
    sys = nom.Clone(nom.GetName()+'_sys')
    # zero out stat. error in sys, since it will contain systematic ONLY
    [sys.SetBinError(ii,0) for ii in xrange(0,sys.GetNbinsX()+2)]
    for hss in SYS[1:]:
        bdiffs = [[] for z in xrange(0,sys.GetNbinsX()+2)]
        # loop over systematics in this group
        for hs in hss:
            h = get( [ isys + '_' + hs for isys in NSYS] ,py)
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
    ewk = get2(NEWK,EWK,py)
    qcd = get2(NQCD,QCD,py)
    for i in xrange(1,data.GetNbinsX()+1):
        lineXYZ(i,data,ewk,qcd,py)

def printDataBgsubSig(py=None):
    data = get(NDATA,py)
    ewk = get2(NEWK,EWK,py)
    qcd = get2(NQCD,QCD,py)
    sig = get2(NSIG,SIG,py)
    bgsub = [ data.Clone('bgsub_stat') , data.Clone('bgsub_sys') ]
    for i in xrange(2):
        bgsub[i].Add(ewk[i],-1)
        bgsub[i].Add(qcd[i],-1)
    for i in xrange(1,data.GetNbinsX()+1):
        lineXYZ(i,data,bgsub,sig,py)

def printEventComposition(py=None , dorel=True):
    """ A version split across two tables """
    HLINES = []
    samples = ['all/zmumu' , 'all/wtaunu' , 'all/ttbar_stop' , 'all/ztautau' , 'all/diboson']
    snames = ['$Z \\rightarrow \mu\mu$ + DrellYan','$W \\rightarrow \\tau \\tau$','$t \\bar{t}$ + single-top','$Z \\rightarrow \\tau \\tau$','Dibosons']
    hs = [ get2([sample,],py) for sample in samples]
    HLINES.append( len(hs)-1 )
    # sum of ewk backgrounds
    hewk = get2(samples,py)
    hs.append(hewk)
    snames.append('Total EWK+top')
    # qcd
    hqcd = get2(['qcd',],py)
    hs.append(hqcd)
    snames.append('QCD')
    # sum of ALL backgrounds
    if False:
        HLINES.append( len(hs)-1 )
        hbg = get2(samples + ['qcd'],py)
        hs.append(hbg)
        snames.append('Total BG')
    # data (for normalization)
    hsig = get(['data',],py)
    ntotal = hs[0][0].GetNbinsX()
    nfirst = int(ntotal/2)
    nsecond = ntotal - nfirst
    for isub in (0,1):
        nbins = nfirst if isub == 0 else nsecond
        binloop = xrange( 1 if isub==0 else nfirst+1 , nbins+1 if isub==0 else ntotal+1)
        print '\\begin{tabular}{%s}'%('c'*(nbins+1))
        #/        &  0.00--0.20  &  0.20--0.40  &  0.40--0.60  &  0.60--0.80  &  1.00--1.20  \\
        print '\hline'
        print '\hline'
        print 'Process / $|\eta|$    ',
        for ibin in binloop:
            low = '%.2f'%(hs[0][0].GetBinLowEdge(ibin))
            high = '%.2f'%(hs[0][0].GetBinLowEdge(ibin)+hs[0][0].GetBinWidth(ibin))
            print '&   %s-%s   '%(low,high),
        print '\\\\'
        print '\hline'
        for i,h in enumerate(hs):
            z = [ snames[i] ]
            for ibin in binloop:
                v  = h[0].GetBinContent(ibin)
                e1 = h[0].GetBinError(ibin)
                e2 = h[1].GetBinError(ibin)
                if dorel:
                    d  = hsig.GetBinContent(ibin)
                    v = v/d*100.0
                    e1 = e1/d*100.0
                    e2 = e2/d*100.0
                z.append( '$%.2f \pm %.2f \pm %.2f$'%(v,e1,e2) )
            print ' & '.join(z) + '   \\\\'
            if i in HLINES:
                print '\hline'
        print '\hline'
        print '\hline'
        print '\end{tabular}'
        # this separates the lower table from the upper one
        if isub==0:
            for ibr in range(4):
                print r'\linebreak'

def printCombinedComposition(py=None , dorel=True):
    """ A combined Q+ / Q- / Q+- version """
    global qs
    hlines = []
    samples = ['all/zmumu' , 'all/wtaunu' , 'all/ttbar_stop' , 'all/ztautau' , 'all/diboson']
    snames = ['$Z \\rightarrow \mu\mu$ + DrellYan','$W \\rightarrow \\tau \\tau$','$t \\bar{t}$ + single-top','$Z \\rightarrow \\tau \\tau$','Dibosons']

    HS = []
    HSIG = []
    
    for qs in ['POS','NEG','ALL']:
        hs = [ get2([sample,],py) for sample in samples]
        if qs=='POS': hlines.append( len(hs)-1 )
        # sum of ewk backgrounds
        hs.append(get2(samples,py))
        if qs=='POS': snames.append('Total EWK+top')
        # qcd
        hs.append(get2(['qcd',],py))
        if qs=='POS': snames.append('QCD')
        # data (for normalization)
        hsig = get(['data',],py)
        # GLOBALIZE
        HS.append ( hs )
        HSIG.append( hsig )

    def oops(iq):
        v,e1,e2 = HS[iq][i][0].GetBinContent(ibin),HS[iq][i][0].GetBinError(ibin),HS[iq][i][1].GetBinError(ibin)
        if dorel:
            d  = HSIG[iq].GetBinContent(ibin); v = v/d*100.0; e1 = e1/d*100.0; e2 = e2/d*100.0
        return v,e1,e2
        
    if True:
        print '\\begin{tabular}{lccc}'
        print '\hline'
        print '\hline'
        print r'Process & $W^+$ & $W^-$ & $W^{\pm}$ \\'
        print '\hline'
        ibin = 1
        for i,h in enumerate(HS[0]):
            z = [ snames[i] ]
            v,e1,e2 = oops(0)
            z.append( '$%.2f \pm %.2f \pm %.2f$'%(v,e1,e2) )
            v,e1,e2 = oops(1)
            z.append( '$%.2f \pm %.2f \pm %.2f$'%(v,e1,e2) )
            v,e1,e2 = oops(2)
            z.append( '$%.2f \pm %.2f \pm %.2f$'%(v,e1,e2) )
            print ' & '.join(z) + '   \\\\'
            if i in hlines:
                print '\hline'
        print '\hline'
        print '\hline'

if __name__ == '__main__':
    if DIM==0:
        assert qs == 'ALL'
        printCombinedComposition(dorel=False)
    elif DIM==1:
        printEventComposition()
    elif DIM==2:
        for iipt in [ipt,]:
            printEventComposition(py=iipt)
    else:
        assert False
