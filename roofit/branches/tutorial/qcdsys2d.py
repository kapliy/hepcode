#!/usr/bin/env python

"""
A small script that summarizes QCD systematic deviations
due to different signal MC or different fit variables.
THIS VERSION OPERATES ON 2D GRID: |eta| x pT
IT ALSO REPACKAGES A ROOT FILE
UPDATE SEP 6: it can alternatively handle 1D histograms now!
"""

memo = """
<pre>
<U>Summary of QCD systematics in 2D bins</U>:
</pre>
"""

import sys,os,re,math,copy
import antondb

# Define all systematics. Note that bgsig is NOT present here, since we save separate histograms for each bgsig
#'/iq%d/X%d/bgewk%d/bgsig%d/iso%s/ivar%s/ibin%s/ieta%d/ipt%s'%(iq,xsecerr,bgewk,bgsig,isofail,ivar,ibin,ieta,ipt)
msys = []
for xsecerr in [ (0,) ]:
    for bgewk in [ (5,),(2,) ]:
        for isofail in [ ('loose_isofail',),('isowind',) ] [:] :
            for ivar in [ ('met','50,0,80'),('met','50,0,90'),('wmt','50,40,90'),('wmt','50,35,100') ]:
                msys.append( xsecerr+bgewk+isofail+ivar )
msys_nom = (0,5,'loose_isofail','met','50,0,80')

etabins = [0.0,0.21,0.42,0.63,0.84,1.05,1.37,1.52,1.74,1.95,2.18,2.4] # N=12
ptbins = [20,25,30,35,40,45,50,120] # N=8
QMAP = {0:'mu+',1:'mu-',2:'both charges'}
QMAPN = {0:'POS',1:'NEG',2:'ALL'}
SIGMAP = { 1 : "MC@NLO", 2 : 'Alpgen+Her', 4 : "PowHeg+Herwig", 5 : "PowHeg+Pythia" }
FQNAMES = { 0 : 'POS_ewk5', 1 : 'NEG_ewk5' }

MODE = 2
if len(sys.argv)>=2 and sys.argv[1]=='1':
    MODE = 1
S = '&nbsp;'
PM = '&plusmn;'

db_name = 'DB_08272012_MC10'
fin_name = 'IN_08272012.v2.%dD.root'%MODE
fout_name = re.sub('IN_','OUT_',fin_name)
if os.path.exists(fin_name):
    print 'ROOT repackaging: %s -> %s'%(fin_name,fout_name)
    import common
    import ROOT
    
def mean(y):
    x = y
    if len(x)==0: return 0
    return sum(x)/len(x)
def rms(y):
    x = y
    if len(x)==0: return 0
    m = mean(x)*1.0
    return math.sqrt( sum([ (xx-m)**2 for xx in x])/len(x) )
def get(iq,bgsig,ieta,ipt):
    idxs = []
    fracs = []
    scales = []
    scalesE = []
    chindfs = []
    # these are for isoloose only!
    scalesL = []
    scalesLE = []
    idx = 0
    bla2=[]
    bla5=[]
    for xsecerr,bgewk,isofail,ivar,ibin in msys:
        key = '/iq%d/X%d/bgewk%d/bgsig%d/iso%s/ivar%s/ibin%s/ieta%s/ipt%s'%(iq,xsecerr,bgewk,bgsig,isofail,ivar,ibin,ieta,ipt)
        if key in R:
            fracs.append(R[key]['frac'])
            sc = R[key]['scales']
            scales.append(sc[0])
            scalesE.append(sc[1])
            if isofail=='loose_isofail':
                scalesL.append(sc[0])
                scalesLE.append(sc[1])
            chindfs.append( 1.0*sc[-3]/sc[-2] )
            if bgewk==2: bla2.append(fracs[-1])
            if bgewk==5: bla5.append(fracs[-1])
        idxs.append(idx)
        idx += 1
    return idxs,fracs,scales,scalesE,chindfs,scalesL,scalesLE

def scale_bin(h,ietabin,iptbin,v):
    etabin = etabins[ietabin]
    if iptbin=='ALL': # in eta only
        ibin = h.FindFixBin(etabin+1e-6)
    else:
        ptbin = ptbins[iptbin]
        ibin = h.FindFixBin(etabin+1e-6,ptbin+1e-6)
    h.SetBinContent(ibin, h.GetBinContent(ibin)*v)
    h.SetBinError(ibin, h.GetBinError(ibin)*v)
    return h

def make_total(csys,samples,cdir,title=None):
    """ Makes and saves total-bg histograms.
    Additionally, saved all components AND ewk-only AND qcd-only.
    Assumes QCD is always the last one under samples[] """
    # save subsamples
    if True:
        sdir = cdir.Get("all") if cdir.Get("all") else cdir.mkdir("all")
        sdir.cd()
        [ h.Write( h.GetTitle() , ROOT.TObject.kOverwrite ) for h in samples ]
        sdir.Write()
        cdir.cd()
    # save ewk (all minus QCD)
    if True:
        g = samples[0].Clone('ewk_'+csys)
        [ g.Add( sample ) for sample in samples[1:-1] ]
        g.SetTitle(g.GetName())
    # save totals
    if True:
        h = samples[0].Clone('totalbg_'+csys)
        #print 'Adding:',[ sample.GetName() for sample in samples[:] ]
        [ h.Add( sample ) for sample in samples[1:] ]
        h.SetTitle(title if title else h.GetName())
    return h

if __name__=='__main__':
    a = antondb.antondb(db_name)
    a.load()
    R = a.data
    f = open('index_%dd.html'%MODE,'w')
    fin,fout = None,None
    fout_D = []
    if os.path.exists(fin_name):
        fin = ROOT.TFile.Open(fin_name,"READ")
        fout = ROOT.TFile.Open(fout_name,"RECREATE")
        fout_D.append(fout.mkdir('POS'))
        fout_D.append(fout.mkdir('NEG'))
    print >>f,'<HTML><BODY>'
    print >>f,memo
    for iq in (0,1,):
        if iq!=0:
            print >>f,'<HR size="20" color="red">'
        samples,systems,qcd = None,None,None
        QCD = []
        if fin and fin.IsOpen():
            adir = fin.Get(FQNAMES[iq])
            adir.cd()
            assert adir,'Cannot access directory: %s'%(FQNAMES[iq])
            samples = [re.sub('_nominal','',key.GetName()) for key in adir.GetListOfKeys() if re.search('nominal',key.GetName())]
            systems = [re.sub('wmunu_','',key.GetName()) for key in adir.GetListOfKeys() if re.match('wmunu_',key.GetName())]
            qcd = adir.Get('qcd').Clone()
            # nominal, qcdup, qcddown[2]                    PowhegHerwig  MC@NLO[4]    MC-averaged    global-scale[6]  scale-factor[7]
            QCD = [ qcd.Clone(), qcd.Clone(), qcd.Clone() ,   qcd.Clone() , qcd.Clone() , qcd.Clone()  , qcd.Clone() ,   qcd.Clone() ]
            QCD[0].SetTitle('qcd_PowhegPythia')
            QCD[1].SetTitle('qcd_PowhegPythiaUp')
            QCD[2].SetTitle('qcd_PowhegPythiaDown')
            QCD[3].SetTitle('qcd_PowhegHerwig')
            QCD[4].SetTitle('qcd_McAtNlo')
            QCD[5].SetTitle('qcd_MCAverage')
            QCD[6].SetTitle('qcd_GlobalNorm')
            QCD[7].SetTitle('qcd_ScaleFactor')
            QCD[7].Reset()
        # first handle inclusive (in both pt and eta) bins
        scalesL = []
        for bgsig in (1,4,5):
            idx,frac,scale,scaleE,chindf,scaleL,scaleLE = get(iq,bgsig,'ALL','ALL')
            scalesL += scaleL
        QCD[6].Scale(mean(scalesL))
        # make a separate table for each pT bin
        ptrange = range(0 , len(ptbins)-1) if MODE==2 else ['ALL',]
        for ipt in ptrange:
            print 'Working on: ',QMAP[iq],'in pt bin',ipt
            print >>f,'<HR>'
            if MODE==2:
                print >>f,QMAP[iq],'%d&lt;pT&lt;%d'%(ptbins[ipt],ptbins[ipt+1])
            else:
                print >>f,QMAP[iq],'%d&lt;pT&lt;%d'%(ptbins[0],ptbins[-1])
            print >>f,'<HR>'
            print >>f,'<TABLE border="1" CELLPADDING="0" CELLSPACING="1">'
            #print >>f,'<TABLE border="1" CELLPADDING="0" CELLSPACING="1" width="1100">'
            # header first
            print >>f,'<TR>'
            print >>f,'<TD width="100">Info</TD>'
            for ieta in xrange(0,len(etabins)-1):
                print >>f, '<TD width="80">','%.2f&lt;|&eta;|&lt;%.2f'%(etabins[ieta],etabins[ieta+1]),"</TD>"
            print >>f,'</TR>'
            # actual table contents
            Aidxs,Afracs,Ascales,AscalesE,Achindfs,AscalesL,AscalesLE,ArelF = [],[],[],[],[],[],[] ,[]
            Afirst = True
            for bgsig in (5,4,1):
                print >>f,'<TR>'
                print >>f,'<TD colspan="%d" align="center">'%(len(etabins)+1),SIGMAP[bgsig],'</TD>'
                print >>f,'</TR>'
                idxs,fracs,scales,scalesE,chindfs,scalesL,scalesLE = [],[],[],[],[],[],[]
                for ieta in xrange(0,len(etabins)-1):
                    idx,frac,scale,scaleE,chindf,scaleL,scaleLE = get(iq,bgsig,ieta,ipt)
                    idxs.append(idx); fracs.append(frac); scales.append(scale); scalesE.append(scaleE); chindfs.append(chindf)
                    scalesL.append(scaleL),scalesLE.append(scaleLE)
                if Afirst:
                    Afirst = False
                    Aidxs = copy.copy(idxs)
                    Afracs = copy.copy(fracs)
                    Ascales = copy.copy(scales)
                    AscalesE = copy.copy(scalesE)
                    Achindfs = copy.copy(chindfs)
                    AscalesL = copy.copy(scalesL)
                    AscalesLE = copy.copy(scalesLE)
                else:
                    for ieta in xrange(0,len(etabins)-1):
                        Aidxs[ieta] += idxs[ieta]
                        Afracs[ieta] += fracs[ieta]
                        Ascales[ieta] += scales[ieta]
                        AscalesE[ieta] += scalesE[ieta]
                        Achindfs[ieta] += chindfs[ieta]
                        AscalesL[ieta] += scalesL[ieta]
                        AscalesLE[ieta] += scalesLE[ieta]
                print >>f,'<TR>'
                print >>f,'<TD>','QCD frac & error','</TD>'
                relF = [] # relative error on qcd fraction. Use it also as an uncertainty on scale!!!
                for ieta in xrange(0,len(etabins)-1):
                    relF.append( rms(fracs[ieta])/mean(fracs[ieta]) )
                    print >>f,'<TD>','%.1f %s %.1f %%'%(mean(fracs[ieta])*100.0,PM,relF[-1]*100.0),'</TD>'
                print >>f,'</TR>'
                print >>f,'<TR>'
                print >>f,'<TD>','Scale factor','</TD>'
                relS = [] # relative error on scale - using a subset of variations (loose_isofail). This is not actually used
                for ieta in xrange(0,len(etabins)-1):
                    relS.append( rms(scalesL[ieta])/mean(scalesL[ieta]) )
                    print >>f,'<TD>','%.3f'%(mean(scalesL[ieta])),'</TD>'
                    #print >>f,'<TD>','%.3f %s %.1f %%'%(mean(scalesL[ieta]),PM,relS[-1]*100.0),'</TD>'
                print >>f,'</TR>'
                # absolute uncertainty on QCD prediction
                print >>f,'<TR>'
                print >>f,'<TD>','Total QCD uncert.','</TD>'
                for ieta in xrange(0,len(etabins)-1):
                    print >>f,'<TD>','%.1f %%'%(relF[ieta]*mean(fracs[ieta])*100.0),'</TD>'
                print >>f,'</TR>'
                # NOW handle ROOT histograms
                if fin:
                    for ieta in xrange(0,len(etabins)-1):
                        if bgsig==5:
                            ibin = QCD[7].FindFixBin(ieta+1e-6,ipt+1e-6) if ipt!='ALL' else QCD[7].FindFixBin(ieta+1e-6)
                            QCD[7].SetBinContent(ibin,mean(scalesL[ieta]))
                            scale_bin(QCD[0],ieta,ipt,mean(scalesL[ieta]))
                            scale_bin(QCD[1],ieta,ipt,mean(scalesL[ieta])*(1.0+relS[ieta]))
                            msc = mean(scalesL[ieta])*(1.0-relS[ieta]) if relS[ieta]<1.0 else 0.0
                            scale_bin(QCD[2],ieta,ipt,msc)
                        if bgsig==4:
                            scale_bin(QCD[3],ieta,ipt,mean(scalesL[ieta]))
                        if bgsig==1:
                            scale_bin(QCD[4],ieta,ipt,mean(scalesL[ieta]))
            # absolute uncertainty on QCD prediction (including effect of different generators)
            # in other words, this is averaging across the three generators
            # first, handle ROOT histograms
            if fin:
                for ieta in xrange(0,len(etabins)-1):
                    scale_bin(QCD[5],ieta,ipt,mean(AscalesL[ieta]))
            print >>f,'<TR>'
            print >>f,'<TD colspan="%d" align="center">'%(len(etabins)+1),'Averaged across three generators','</TD>'
            print >>f,'</TR>'
            # Absolute QCD value
            print >>f,'<TR>'
            print >>f,'<TD>','QCD frac & error','</TD>'
            relF = [] # relative error on qcd fraction. Use it also as an uncertainty on scale!!!
            for ieta in xrange(0,len(etabins)-1):
                relF.append( rms(Afracs[ieta])/mean(Afracs[ieta]) )
                print >>f,'<TD>','%.1f %s %.1f %%'%(mean(Afracs[ieta])*100.0,PM,relF[-1]*100.0),'</TD>'
            print >>f,'</TR>'
            # relative error on scale - using a subset of variations (loose_isofail)
            print >>f,'<TR>'
            print >>f,'<TD>','Scale factor','</TD>'
            for ieta in xrange(0,len(etabins)-1):
                print >>f,'<TD>','%.3f'%(mean(AscalesL[ieta])),'</TD>'
            print >>f,'</TR>'
            # total qcd uncertainty (absolute)
            print >>f,'<TR>'
            print >>f,'<TD>','Total QCD uncert.','</TD>'
            for ieta in xrange(0,len(etabins)-1):
                relF = rms(Afracs[ieta])/mean(Afracs[ieta])
                print >>f,'<TD>','%.1f %%'%(relF*mean(Afracs[ieta])*100.0),'</TD>'
            print >>f,'</TR>'
            print >>f,'</TABLE>'
        if fin:
            # generate total histograms for detector systematics
            fout_D[iq].cd()
            if True:
                bgsig=5
                adir.Get('data').Write('data',ROOT.TObject.kOverwrite)
                for system in systems:
                    allsamples = [adir.Get(sample+'_'+system) for sample in samples if sample!='wmunu'] + [QCD[0],]
                    htot = make_total(system,allsamples,fout_D[iq])
                    htot.Write(htot.GetTitle(),ROOT.TObject.kOverwrite)
                    adir.Get('wmunu_'+system).Write('wmunu_PowhegPythia_'+system,ROOT.TObject.kOverwrite)
                # manually generate nominal histograms with qcd Up/Down variations
                system='nominal'
                allsamples = [adir.Get(sample+'_'+system) for sample in samples if sample!='wmunu'] + [QCD[1],]
                htot = make_total(system,allsamples,fout_D[iq])
                htot.SetTitle('totalbg_nominal_qcd_up')
                htot.Write(htot.GetTitle(),ROOT.TObject.kOverwrite)
                allsamples = [adir.Get(sample+'_'+system) for sample in samples if sample!='wmunu'] + [QCD[2],]
                htot = make_total(system,allsamples,fout_D[iq])
                htot.SetTitle('totalbg_nominal_qcd_down')
                htot.Write(htot.GetTitle(),ROOT.TObject.kOverwrite)
                # save QCD scale factor histogram
                QCD[7].SetTitle('qcd_scale_factor')
                QCD[7].Write(QCD[7].GetTitle(),ROOT.TObject.kOverwrite)
                # generate histograms for alpgen bg subtraction
                if True:
                    bdir = fin.Get('%s_ewk2'%QMAPN[iq])
                    assert bdir
                    allsamples = [bdir.Get(sample+'_'+system) for sample in samples if sample!='wmunu'] + [QCD[0],]
                    htot = make_total(system,allsamples,fout_D[iq])
                    htot.Write('totalbg_nominal_ewk_alpgen',ROOT.TObject.kOverwrite)
                # generate histograms for ewkbg xsec variations
                if True:
                    bdir = fin.Get('%s_ewk5_xsecup'%QMAPN[iq])
                    assert bdir
                    allsamples = [bdir.Get(sample+'_'+system) for sample in samples if sample!='wmunu'] + [QCD[0],]
                    htot = make_total(system,allsamples,fout_D[iq])
                    htot.Write('totalbg_nominal_ewk_xsecup',ROOT.TObject.kOverwrite)
                    bdir = fin.Get('%s_ewk5_xsecdown'%QMAPN[iq])
                    assert bdir
                    allsamples = [bdir.Get(sample+'_'+system) for sample in samples if sample!='wmunu'] + [QCD[0],]
                    htot = make_total(system,allsamples,fout_D[iq])
                    htot.Write('totalbg_nominal_ewk_xsecdown',ROOT.TObject.kOverwrite)
            if True:
                bgsig=4
                system='nominal'
                allsamples = [adir.Get(sample+'_'+system) for sample in samples if sample!='wmunu'] + [QCD[3],]
                htot = make_total(system,allsamples,fout_D[iq])
                htot.SetTitle('totalbg_nominal_unfoldPowhegJimmy')
                htot.Write(htot.GetTitle(),ROOT.TObject.kOverwrite)
            if True:
                bgsig=1
                system='nominal'
                allsamples = [adir.Get(sample+'_'+system) for sample in samples if sample!='wmunu'] + [QCD[4],]
                htot = make_total(system,allsamples,fout_D[iq])
                htot.SetTitle('totalbg_nominal_unfoldMCNLO')
                htot.Write(htot.GetTitle(),ROOT.TObject.kOverwrite)
            if True:
                bgsig=5
                system='nominal'
                allsamples = [adir.Get(sample+'_'+system) for sample in samples if sample!='wmunu'] + [QCD[6],]
                htot = make_total(system,allsamples,fout_D[iq])
                htot.SetTitle('totalbg_nominal_qcdaverage')
                htot.Write(htot.GetTitle(),ROOT.TObject.kOverwrite)
            
    if fin and fin.IsOpen():
        fin.Close()
    if fout and fout.IsOpen():
        fout.Close()
    print >>f,'</BODY></HTML>'
