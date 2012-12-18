#!/usr/bin/env python

"""
./qcdsys3d.py 1 && ./qcdsys3d.py 2

A small script that summarizes QCD systematic deviations
due to different signal MC or different fit variables.
THIS VERSION OPERATES ON 2D GRID: |eta| x pT
IT ALSO REPACKAGES A ROOT FILE
UPDATE SEP 6: it can alternatively handle 1D histograms now!
"""

memo = """
New, conservative QCD estimate : including all systematics
"""

import sys,os,re,math,copy
import antondb

DIM = 1  # 2d fits in pt x eta
if len(sys.argv)>=2 and sys.argv[1]=='2':
    DIM = 2  # 1d fits in eta only
print 'DIMENSION:',DIM
ETAMODE = 2 # use |eta|
if len(sys.argv)>=3 and sys.argv[2]=='1':
    ETAMODE = 1 # use eta
print 'ETAMODE:',ETAMODE
eword = 'eta' if ETAMODE==1 else 'abseta'

QMAP = {0:'mu+',1:'mu-',2:'both charges'}
QMAPN = {0:'POS',1:'NEG',2:'ALL'}
SIGMAP = { 0: 'Pythia' , 1 : "MC@NLO", 2 : 'Alpgen+Her', 4 : "PowHeg+Herwig", 5 : "PowHeg+Pythia" }
FQNAMES = { 0 : 'POS_sig5_tau2_ewk5', 1 : 'NEG_sig5_tau2_ewk5' }

import binning
etabins = binning.setabins if ETAMODE==1 else binning.absetabins # N=12
ptbins = binning.ptbins # N=8

S = '&nbsp;'
PM = '&plusmn;'
COLH = 150
COLW = 95

bgqcd = 3
db_name = 'DB_11282012_ALL.v2'
fin_name = 'IN_11282012_ALL.v1.%s.%dD.root'%(eword,DIM)

bgqcd = 4
db_name = 'DB_12042012_ALL.v1'
fin_name = 'IN_12042012_ALL.v2.%s.%dD.root'%(eword,DIM)

# bgqcd = 4
# db_name = 'DB_12102012_PT25.v1'
# fin_name = 'IN_12102012_PT25.v2.%s.%dD.root'%(eword,DIM)

fout_name = re.sub('IN_','OUT_',fin_name)
if os.path.exists(fin_name):
    print 'ROOT repackaging: %s -> %s'%(fin_name,fout_name)
    import common
    import ROOT
    
def get(ieta,ipt):
    key = '%s_%s_%s_%s'%(iq,ETAMODE,ieta,ipt)
    if key not in R:
        print 'Missing key: %s'%key
        return [None,]*6
    return R[key]['data']
    
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

CACHE = {}
def make_qcd(name):
    if name in CACHE: return [CACHE[name],]
    out = qcd.Clone(name)
    ipts = range(1,len(ptbins)) if DIM==2 else ['ALL',]
    ietas = range(1,len(etabins))
    for ipt in ipts:
        for ieta in ietas:
            SVAL,SABS,SREL,SLAB , MQCD,FQCD = get(ieta,ipt)
            v = 0
            if MQCD:
                # if requesting a particular systematic, just take it directly
                if name in MQCD:
                    v = MQCD[name]
                elif name in ('qcd_up','qcd_down'):
                    nom = MQCD['Nominal']
                    # hardcoded list of indices that are not automatically propagated in EWUnfold (so we need to propagate them)
                    PROPi = [0,1]    # fit error; met fit range
                    err = math.sqrt( sum([ (SABS[i]*SABS[i]) for i in PROPi ]) )
                    v = nom+err if name=='qcd_up' else nom-err
                    if v<0:
                        print 'WARNING: negative qcd contribution:',ipt,ieta,nom,err
                else:
                    if ipt==ipts[0] and ieta==ietas[0]:
                        print 'Falling back to Nominal for systematic:',name
                    v = MQCD['Nominal']  # fall back to Nominal
            if DIM==1:
                out.SetBinContent(ieta,v)
            elif DIM==2:
                out.SetBinContent(ieta,ipt,v)
    CACHE[name] = out
    return [out,]

def make_total(csys,samples,cdir,prefix=None):
    """ Makes and saves total-bg histograms.
    Additionally, saved all components AND ewk-only AND qcd-only.
    Assumes QCD is always the last one under samples[] """
    h,g = None,None
    assert samples[0],'Missing first ewk background (please fix or allows this error to be recoverable)'
    assert samples[-1],'Missing qcd'
    # save subsamples (in a sub-directory)
    if True:
        sdir = cdir.Get("all") if cdir.Get("all") else cdir.mkdir("all")
        sdir.cd()
        [ h.Write( h.GetTitle() , ROOT.TObject.kOverwrite)  for h in samples if h]
        sdir.Write()
        cdir.cd()
    # save qcd (in root directory)
    if True:
        d = samples[-1].Clone('qcd_'+csys)
        if prefix:
            d.SetTitle('qcd_'+prefix)
        d.Write(d.GetTitle(),ROOT.TObject.kOverwrite)
    # save ewk (all minus QCD) (in root directory)
    if True:
        g = samples[0].Clone('ewk_'+csys)
        [ g.Add( sample ) for sample in samples[1:-1] if sample]
        if prefix:
            g.SetTitle('ewk_'+prefix)
        g.Write(g.GetTitle(),ROOT.TObject.kOverwrite)
    # save totals (including QCD) (in root directory)
    if True:
        h = samples[0].Clone('totalbg_'+csys)
        #print 'Adding:',[ sample.GetName() for sample in samples[:] ]
        [ h.Add( sample ) for sample in samples[1:] if sample]
        if prefix:
            h.SetTitle('totalbg_'+prefix)
        h.Write(h.GetTitle(),ROOT.TObject.kOverwrite)
    return h,g

if __name__=='__main__':
    a = antondb.antondb(db_name)
    assert a.load(),'ERROR: failed to load antondb %s'%db_name
    R = a.data
    fin,fout = None,None
    fout_D = []
    assert os.path.exists(fin_name)
    fin = ROOT.TFile.Open(fin_name,"READ")
    fout = ROOT.TFile.Open(fout_name,"RECREATE")
    fout_D.append(fout.mkdir('POS'))
    fout_D.append(fout.mkdir('NEG'))
    for iq in (0,1,):
        samples,systems,qcd = None,None,None
        assert fin and fin.IsOpen()
        adir = fin.Get(FQNAMES[iq])
        adir.cd()
        assert adir,'Cannot access directory: %s'%(FQNAMES[iq])
        samples = [re.sub('_Nominal','',key.GetName()) for key in adir.GetListOfKeys() if re.search('Nominal',key.GetName())]
        systems = [re.sub('wmunu_','',key.GetName()) for key in adir.GetListOfKeys() if re.match('wmunu_',key.GetName())]
        qcd = adir.Get('qcd').Clone()
        qcd.Reset()
        make_qcd('Nominal')
        # generate total histograms for detector systematics
        fout_D[iq].cd()
        if True:
            bgsig=5
            adir.Get('data').Write('data',ROOT.TObject.kOverwrite)
            # so in principle, we should use correlated ewk here, too (i.e. mcnlo or powheg-herwig)
            # but unfortunately there is not enough statistics. We have to use alpgen instead
            adir4 = fin.Get('%s_sig4_tau2_ewk4'%QMAPN[iq]); assert adir4
            adir1 = fin.Get('%s_sig1_tau2_ewk1'%QMAPN[iq]); assert adir1
            for system in systems: #including Nominal
                allsamples = [adir.Get(sample+'_'+system) for sample in samples if sample!='wmunu'] + make_qcd(system)
                make_total(system,allsamples,fout_D[iq],system)
                adir.Get('wmunu_'+system).Write('wmunu_PowhegPythia_'+system,ROOT.TObject.kOverwrite)
                adir4.Get('wmunu_'+system).Write('wmunu_PowhegJimmy_'+system,ROOT.TObject.kOverwrite)
                adir1.Get('wmunu_'+system).Write('wmunu_McAtNlo_'+system,ROOT.TObject.kOverwrite)
            # manually generate nominal histograms with qcd Up/Down variations
            system='Nominal'
            allsamples = [adir.Get(sample+'_'+system) for sample in samples if sample!='wmunu'] + make_qcd('qcd_up')
            make_total(system,allsamples,fout_D[iq],'Nominal_qcd_up')
            allsamples = [adir.Get(sample+'_'+system) for sample in samples if sample!='wmunu'] + make_qcd('qcd_down')
            make_total(system,allsamples,fout_D[iq],'Nominal_qcd_down')
            # compatibility: aliases [TODO: handle everything automatically through make_total]
            adir4.Get('wmunu_'+system).Write('wmunu_PowhegPythia_Nominal_PowhegJimmy',ROOT.TObject.kOverwrite)
            adir1.Get('wmunu_'+system).Write('wmunu_PowhegPythia_Nominal_MCNLO',ROOT.TObject.kOverwrite)
            # generate histograms for alpgen bg subtraction
            if True:
                bdir = fin.Get('%s_sig5_tau2_ewk5'%QMAPN[iq])
                assert bdir
                allsamples = [bdir.Get(sample+'_'+system) for sample in samples if sample!='wmunu'] + make_qcd('Nominal')
                make_total(system,allsamples,fout_D[iq],'Nominal_ewk_alpgen')
                bdir.Get('wmunu_'+system).Write('wmunu_PowhegPythia_Nominal_ewk_alpgen',ROOT.TObject.kOverwrite)
            # generate histograms for ewkbg xsec variations
            if True:
                bdir = fin.Get('%s_sig5_tau2_ewk5_xsecup'%QMAPN[iq])
                assert bdir
                allsamples = [bdir.Get(sample+'_'+system) for sample in samples if sample!='wmunu'] + make_qcd('Nominal')
                make_total(system,allsamples,fout_D[iq],'Nominal_ewk_xsecup')
                bdir.Get('wmunu_'+system).Write('wmunu_PowhegPythia_Nominal_ewk_xsecup',ROOT.TObject.kOverwrite)
                bdir = fin.Get('%s_sig5_tau2_ewk5_xsecdown'%QMAPN[iq])
                assert bdir
                allsamples = [bdir.Get(sample+'_'+system) for sample in samples if sample!='wmunu'] + make_qcd('Nominal')
                make_total(system,allsamples,fout_D[iq],'Nominal_ewk_xsecdown')
                bdir.Get('wmunu_'+system).Write('wmunu_PowhegPythia_Nominal_ewk_xsecdown',ROOT.TObject.kOverwrite)
            # generate histograms for MC-based QCD
            if True:
                bdir = fin.Get('%s_sig5_tau2_ewk5_qcdmc'%QMAPN[iq])
                assert bdir
                allsamples = [bdir.Get(sample+'_'+system) for sample in samples if sample!='wmunu'] + [bdir.Get('qcd').Clone(),]
                make_total(system,allsamples,fout_D[iq],'Nominal_qcdmc')
        # totalbg where QCD has been fit using only a particular EWK MC (to correlate QCD with unfolding)
        if True:
            bgsig=4
            system='Nominal'
            allsamples = [adir4.Get(sample+'_'+system) for sample in samples if sample!='wmunu'] + make_qcd('Nominal_PowhegJimmy')
            make_total(system,allsamples,fout_D[iq],'Nominal_PowhegJimmy')
        if True:
            bgsig=1
            system='Nominal'
            allsamples = [adir1.Get(sample+'_'+system) for sample in samples if sample!='wmunu'] + make_qcd('Nominal_MCNLO')
            make_total(system,allsamples,fout_D[iq],'Nominal_MCNLO')
        if False:
            bgsig=5
            system='Nominal'
            allsamples = [adir.Get(sample+'_'+system) for sample in samples if sample!='wmunu'] + make_qcd('MCAverage')
            make_total(system,allsamples,fout_D[iq],'Nominal_qcd_MCAverage')
        if False:
            bgsig=5
            system='Nominal'
            allsamples = [adir.Get(sample+'_'+system) for sample in samples if sample!='wmunu'] + make_qcd('inclusive')
            make_total(system,allsamples,fout_D[iq],'Nominal_qcdnorm_inclusive')
            
    fin.Close()
    fout.cd()
    sout = ROOT.TObjString(memo)
    sout.Write("memo")
    fout.Close()
