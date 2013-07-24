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
ROOT.gROOT.SetBatch(True)
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
# MISSING: stat. uncertainty due to scale factors (done via sys. shift); PDF eigenvectors
EWK = []
EWK.append( ['Nominal'] )
EWK.append( ['MuonResMSUp','MuonResMSDown'] )
EWK.append( ['MuonResIDUp','MuonResIDDown'] )
EWK.append( ['MuonKScaleUp','MuonKScaleDown' ] )
EWK.append( ['MuonCScaleUp','MuonCScaleDown' ] )
EWK.append( ['MuonRecoSFUp','MuonRecoSFDown'] )
EWK.append( ['MuonRecoSFPhi',] )
EWK.append( ['MuonRecoStatSFUp','MuonRecoStatSFDown'] )
EWK.append( ['MuonIsoSFUp','MuonIsoSFDown'] )
EWK.append( ['MuonTriggerSFPhi',] )
EWK.append( ['MuonTriggerStatSFUp','MuonTriggerStatSFDown'] )
EWK.append( ['MuonIsoSFUp','MuonIsoSFDown'] )
EWK.append( ['JetResolUp','JetResolDown'] )
EWK.append( ['JetScaleUp','JetScaleDown'] )
EWK.append( ['JetNPVUp','JetNPVDown'] )
EWK.append( ['JetMUUp','JetMUDown'] )
EWK.append( ['PileupScale',] )
EWK.append( ['ResoSoftTermsUp_ptHard','ResoSoftTermsDown_ptHard'] )
EWK.append( ['ScaleSoftTermsUp_ptHard','ScaleSoftTermsDown_ptHard'] )
EWK.append( ['WptSherpa','WptPythia8'] )
#EWK.append( ['PdfMSTW','PdfHERA','PdfNNPDF','PdfABM'] )
EWK.append( ['Nominal_ewk_xsecdown','Nominal_ewk_xsecup'] )
EWK.append( ['unfoldPowhegJimmy', 'unfoldMCNLO'] ) # caveat: ewk=5 (same)
EWK.append( ['Nominal_qcd_up','Nominal_qcd_down'])

# a version of EWK that only includes uncorrelated component of QCD uncertainty
EWK_u = EWK[0:1] + [['Nominal_qcd_statup','Nominal_qcd_statdown']]

SAMPLES = ['all/wtaunu' , 'all/zmumu' , 'all/ttbar_stop' , 'all/ztautau' , 'all/diboson']  #+ [total_ewk,qcd]
SNAMES = ['$W \\rightarrow \\tau \\nu$','$Z \\rightarrow \mu\mu$ + DrellYan','$t \\bar{t}$ + single-top','$Z \\rightarrow \\tau \\tau$','Dibosons']
assert len(SAMPLES)==len(SNAMES)

SCOLORS = {}
SCOLORS[ SNAMES[0] ] = 41 #ROOT.kYellow - 9
SCOLORS[ SNAMES[1] ] = ROOT.kRed
SCOLORS[ SNAMES[2] ] = ROOT.kGreen + 1
SCOLORS[ SNAMES[3] ] = ROOT.kViolet
SCOLORS[ SNAMES[4] ] = ROOT.kOrange - 4
SCOLORS['Total EWK+top'] = ROOT.kBlue
SCOLORS['QCD'] = ROOT.kAzure - 9
SSTYLES = [20,21,22,23,33,34,22,29]
assert len(SSTYLES) >= len(SCOLORS)
SORDER = [6,5,4,3,2,1,0] #plot order

def getH(x):
    """ retrieves one histogram and makes sure it is valid
    TODO: re.search for slash (/) and if found, do not add qs manually
    """
    nm = '%s/%s'%(qs,x)
    tmp = f.Get(nm)
    if not tmp:
        assert False, 'ERROR: cannot find histogram %s'%nm
    return tmp

def get(xall,py=None,nm=''):
    """ retrieves one histogram and makes sure it is valid
    xall is a list: all histograms in the list are summed (e.g., all-ewk)
    py performs a projection on an axis
    """
    x = None
    assert isinstance(xall,list) or isinstance(xall,tuple), 'ERROR: get() now only accepts python lists or tuples'
    x = xall
    tmp = None
    for ix,curx in enumerate(x):
        if ix==0:
            tmpix = getH(curx)
            tmp = tmpix.Clone(common.rand_name()+'%s_%d'%(tmpix.GetName(),len(x))+nm)
        else:
            tmpix = getH(curx)
            tmp.Add(tmpix)
    if not tmp:
        assert False, 'ERROR: cannot find histogram %s'%nm
    if py!=None:
        assert DIM==2,'Projection only works in 2D'
        # d2_abseta_lpt  x=abseta, y=lpt
        ipy = int(py)
        tmp2 = tmp.ProjectionX(tmp.GetName()+('_px_%d'%py)+nm,py,py,'e')
        return tmp2.Clone(common.rand_name())
    return tmp.Clone(common.rand_name())

def get2(NSAMPLES,py=None,SYS=EWK,nm=''):
    """ returns both statistical and systematic histograms
    NSAMPLES is a vector of samples to add (eg, if we want all ewk, or to combine charges)
    """
    nom = get( [ isam + '_' + SYS[0][0] for isam in NSAMPLES] ,py , nm=nm)
    hsys = nom.Clone(nom.GetName()+'_sys'+common.rand_name())
    # zero out stat. error in sys, since it will contain systematic ONLY
    [hsys.SetBinError(ii,0) for ii in xrange(0,hsys.GetNbinsX()+2)]
    for hss in SYS[1:]:
        bdiffs = [[] for z in xrange(0,hsys.GetNbinsX()+2)]
        # loop over systematics in this group
        for hs in hss:
            h = get( [ isam + '_' + hs for isam in NSAMPLES] ,py)
            for ibin in xrange(0,hsys.GetNbinsX()+2):
                bdiffs[ibin].append ( abs(nom.GetBinContent(ibin)-h.GetBinContent(ibin)) )
        # loop over bins and add new error (max. deviation) in quadrature
        # in other words, we symmetrize up/down deviations here
        for ibin in xrange(0,hsys.GetNbinsX()+2):
            newerr = max(bdiffs[ibin]) if len(bdiffs[ibin])>0 else 0
            # sys
            olderr = hsys.GetBinError(ibin)
            hsys.SetBinError(ibin,1.0*math.sqrt(olderr*olderr + 1.0*newerr*newerr))
    return nom,hsys

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
    """ A version split across THREE tables """
    import SuData
    samples = SAMPLES[:]
    snames = SNAMES[:]
    HLINES = []
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
    assert( ntotal==11 ) # only works in |eta| due to hardcoded row boundaries
    nfirst = 4
    nsecond = 4
    nthird = 3
    # histograms
    assert len(hs)==len(snames)
    HP = [ SuData.SuSample.make_habseta('bgsummary%d_%s_pt%s'%(iz,qs,py)) for iz,z in enumerate(hs) ]
    # loop
    for isub in (0,1,2):
        nbins = nfirst if isub == 0 else (nsecond if isub==1 else nthird)
        binmin = 1 if isub==0 else (5 if isub==1 else 9)
        binmax = 4+1 if isub==0 else (8+1 if isub==1 else 12)
        binloop = xrange( binmin , binmax )
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
                HP[i].SetBinContent(ibin,v)
                HP[i].SetBinError(ibin, math.sqrt(e1*e1+e2*e2) )
            print ' & '.join(z) + '   \\\\'
            if i in HLINES:
                print '\hline'
        print '\hline'
        print '\hline'
        print '\end{tabular}'
        # this separates the lower tables from the upper ones
        if isub in (0,1):
            for ibr in range(4):
                print r'\linebreak'
    # make plots
    if True:
        from SuCanvas import SuCanvas,PlotOptions
        from binning import LABELMAP
        SuCanvas.savedir = './'
        SuCanvas.savetypes = ['png','pdf']
        SuCanvas.g_lin_ratio_y_title_offset = 1.7
        SuCanvas.g_marker_size = 0.9
        SuCanvas.g_legend_x1_ndc = 0.45
        SuCanvas.g_text_size = 18
        SuCanvas.g_legend_height_per_entry = 0.043
        SuCanvas.g_lin_ratio_y_title_offset = 2.0
        SuCanvas.g_lin_main_y_title_offset = 2.0
        SuCanvas.cgStyle = SuCanvas.ControlPlotStyle()
        SuCanvas._refLineMin = 0.99
        SuCanvas._refLineMax = 1.02
        M = PlotOptions()
        M.msize = 1.3
        size = SuCanvas.g_marker_size
        HOUT = []
        # separately prepare uncorrelated part of QCD
        hqcd_u = get2(['qcd',],py,SYS=EWK_u,nm='uncorr')
        Hqcd_u = SuData.SuSample.make_habseta('bgsummary%s_%s_pt%s'%('qcduncorr',qs,py))
        for ibin in xrange(1,12):
            v  = hqcd_u[0].GetBinContent(ibin)
            e1 = hqcd_u[0].GetBinError(ibin)
            e2 = hqcd_u[1].GetBinError(ibin)
            if dorel:
                d  = hsig.GetBinContent(ibin)
                v = v/d*100.0
                e1 = e1/d*100.0
                e2 = e2/d*100.0
            Hqcd_u.SetBinContent(ibin,v)
            Hqcd_u.SetBinError(ibin, math.sqrt(e1*e1+e2*e2) )
        M.add('QCD (uncorr)','QCD (uncorr)',size=size*1.1,style=SSTYLES[0],color=29,drawopt=' E4')
        # plot
        j=0
        for i in SORDER:
            nm2 = snames[i].replace('$','')
            if nm2=='QCD': nm2='QCD (total)'
            M.add(nm2,nm2,size=size*1.1,style=SSTYLES[j],color=SCOLORS[snames[i]])
            HOUT.append( HP[i] )
            j+=1
        c = SuCanvas('bgsummary_%s_pt%s'%(qs,pt if py==None else py))
        xaxis_info = LABELMAP['lepton_absetav'] + ['Background fraction','%']
        HOUT = [Hqcd_u,] + HOUT
        c.plotAny(HOUT,M=M,height=2.0,xaxis_info=xaxis_info)
        c.SaveSelf(silent=True)

def printCombinedComposition(py=None , dorel=True):
    """ A combined Q+ / Q- / Q+- version """
    global qs
    hlines = []
    samples = SAMPLES[:]
    snames = SNAMES[:]

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
        print '\end{tabular}'

if __name__ == '__main__':
    if DIM==0:
        assert qs == 'ALL'
        printCombinedComposition(dorel=True)
    elif DIM==1:
        printEventComposition()
    elif DIM==2:
        for iipt in [ipt,]:
            printEventComposition(py=iipt)
    else:
        assert False
