#!/usr/bin/env python

"""
./qcdsys3d.py 2 20   &&   ./qcdsys3d.py 1 20 && ./qcdsys3d.py 0 20   &&   ./qcdsys3d.py 1 25 && ./qcdsys3d.py 0 25

A small script that summarizes QCD systematic deviations
due to different signal MC or different fit variables.
"""

memo = """
v1: first version to be placed in common afs area. January 6 2013.
v2: added integrated measurement. January 7 2013.
v3: saving all background components. Added charge-summed integrated measurement. January 7 2013.
"""

import sys,os,re,math,copy
import antondb

# DIM=2: 2d in pt x |eta|
# DIM=1: 1d in |eta|
# DIM=0: 1d in |eta| (one bin 0-2.4)
DIM = 1
if len(sys.argv)>=2:
    DIM = int(sys.argv[1])  # 1d fits in eta only
assert DIM in (0,1,2), 'ERROR: unknown dimension %s'%DIM
print 'DIMENSION:',DIM
memo += ' DIM=%s'%DIM

# Minimum pt cut (20 or 25 GeV) - only for 1D and 2D cases
PT = 20
if len(sys.argv)>=3:
    PT = int(sys.argv[2])  # 1d fits in eta only
assert PT in (20,25), 'ERROR: unknown PT %s'%PT
if PT==25: assert DIM in (0,1), 'ERROR: cannot use PT=25 cut for 2D case'
print 'PT:',PT
memo += ' PT=%s'%PT

# eta vs |eta|
ETAMODE = 2
if len(sys.argv)>=4:
    ETAMODE = int(sys.argv[3])
assert ETAMODE in (2,1), 'ERROR: unknown eta mode: %d'%ETAMODE
print 'ETAMODE:',ETAMODE
eword = 'eta' if ETAMODE==1 else 'abseta'
memo += ' ETA=%s'%eword

QMAP = {0:'mu+',1:'mu-',2:'both charges'}
QMAPN = {0:'POS',1:'NEG',2:'ALL'}
SIGMAP = { 0: 'Pythia' , 1 : "MC@NLO", 2 : 'Alpgen+Her', 4 : "PowHeg+Herwig", 5 : "PowHeg+Pythia" }
FQNAMES = { 0 : 'POS_sig5_tau2_ewk5', 1 : 'NEG_sig5_tau2_ewk5' }

import binning
etabins = binning.setabins if ETAMODE==1 else binning.absetabins # N=12
ptbins = binning.ptbins # N=8

# from config.sh
bgqcd = 4
z = os.popen('source config.sh && echo $DB','r')
db_name = z.readline().strip()
z.close()
z = os.popen('source config.sh && echo $INDB','r')
fin_base = z.readline().strip()
z.close()
fin_name = '%s.%s.%dD.pt%d.root'%(fin_base,eword,DIM,PT)

fout_name = re.sub('IN_','OUT_',fin_name)
assert os.path.exists(fin_name),'ERROR: cannot find input file: %s'%fin_name
print 'ROOT repackaging: %s -> %s'%(fin_name,fout_name)
import common
import ROOT
    
def get(ieta,ipt):
    key = '%s_%s_%s_%s'%(iq,ETAMODE,ieta,ipt)
    if key not in R:
        print 'Missing key: %s'%key
        assert False,'ERROR: missing key not implemented yet. Please implement and update default return type'
        return [None,]*6
    return R[key]['MSYS'],R[key]['MGROUPS']
    
CACHE = {}
def make_qcd(name):
    if name in CACHE: return [CACHE[name],]
    out = qcd.Clone(name)
    out.Reset()
    out.SetTitle(out.GetTitle()+'_'+name)
    ipts = range(1,len(ptbins)) if DIM==2 else ['ALL%d'%PT,]
    ietas = range(1,len(etabins))
    for ipt in ipts:
        for ieta in ietas:
            MSYS,MGROUPS = get(ieta,ipt)
            MSYS_FLAT = common.qcdfit_sys_flatten(MSYS,MGROUPS)
            v = 0
            # if requesting a particular systematic, just take it directly
            if name in MSYS_FLAT:
                v  = MSYS_FLAT[name][11]
                ve = MSYS_FLAT[name][12]
            elif name in ('Nominal_qcd_up','Nominal_qcd_down'):
                # Hardcoded list of QCD systematics not propagated through EWUnfold (so we need to add them in quadrature here)
                # That includes: fit error; met fit range  + choice of anti-isolation (previously missing)
                _ERRORS = ['fit_error','met_range','IsoWind20m','IsoWind40']
                devs = common.qcdfit_sys_deviations_subset(MSYS,MGROUPS,_ERRORS)
                dev2 = math.sqrt( sum([ xx*xx for xx in devs ]) )
                err  = 1.0*dev2 if name=='Nominal_qcd_up' else -1.0*dev2
                v  = MSYS_FLAT['Nominal'][11] + err
                ve = MSYS_FLAT['Nominal'][12]
                if v<0:
                    print 'WARNING: negative qcd contribution:',ipt,ieta,nom,err
            else:
                if ipt==ipts[0] and ieta==ietas[0]:
                    print 'Falling back to Nominal for systematic:',name
                v  = MSYS_FLAT['Nominal'][11]
                ve = MSYS_FLAT['Nominal'][12]
            if DIM in (0,1):
                out.SetBinContent(ieta,v)
                out.SetBinError(ieta,ve)
            elif DIM==2:
                out.SetBinContent(ieta,ipt,v)
                out.SetBinError(ieta,ipt,ve)
    CACHE[name] = out
    return [out,]

def make_total(csys,samples,cdir,prefix,add=False):
    """ Makes and saves total-bg histograms.
    Additionally, saved all components AND ewk-only AND qcd-only.
    Assumes samples = [s1,s2,s3,...,QCD,SIG]. s1,QCD,SIG have to be non-empty
    """
    h,g = None,None
    assert samples[0],'Missing first ewk background (please fix or allow this error to be recoverable)'
    assert samples[-2],'Missing qcd'
    assert samples[-1],'Missing signal (wmunu)'
    def write_or_add(xdir,xnew,xtitle):
        xold = xdir.Get(xtitle)
        if add and xold:
            xold.Add(xnew)
            xold.Write( xtitle , ROOT.TObject.kOverwrite)
        else:
            xnew.Write( xtitle , ROOT.TObject.kOverwrite)
    # save all subsamples (in a sub-directory)
    if True:
        sdir = cdir.Get("all") if cdir.Get("all") else cdir.mkdir("all")
        sdir.cd()
        for h in samples:
            if not h: continue
            title = h.GetTitle()
            if not re.search(prefix,title):
                assert re.search('_Nominal',title),'ERROR: found histogram without _Nominal in its name: %s'%title
                title = re.sub('_Nominal','_'+prefix,title)
            assert re.search(prefix,title)
            #print 'Subsample: ',prefix,title
            write_or_add(sdir,h,title)
        sdir.Write()
        cdir.cd()
    # save signal (in root directory)
    if True:
        d = samples[-1].Clone('sig_'+csys)
        d.SetTitle('sig_'+prefix)
        write_or_add(cdir,d,d.GetTitle())
    # save qcd (in root directory)
    if True:
        d = samples[-2].Clone('qcd_'+csys)
        d.SetTitle('qcd_'+prefix)
        write_or_add(cdir,d,d.GetTitle())
    # save ewk (all minus QCD) (in root directory). Excludes signal.
    if True:
        g = samples[0].Clone('ewk_'+csys)
        [ g.Add( sample ) for sample in samples[1:-2] if sample]
        g.SetTitle('ewk_'+prefix)
        write_or_add(cdir,g,g.GetTitle())
    # save totals (including QCD) (in root directory). Excludes signal
    if True:
        h = samples[0].Clone('totalbg_'+csys)
        #print 'Adding:',[ sample.GetName() for sample in samples[:] ]
        [ h.Add( sample ) for sample in samples[1:-1] if sample]
        h.SetTitle('totalbg_'+prefix)
        write_or_add(cdir,h,h.GetTitle())
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
    fout_D.append(fout.mkdir('ALL'))
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
        # generate total histograms for detector systematics
        if True:
            bgsig=5
            fout_D[2].cd()
            hh = fout_D[2].Get('data')
            if hh:
                hh.Add(adir.Get('data'))
                hh.Write('data',ROOT.TObject.kOverwrite)
            else:
                adir.Get('data').Write('data',ROOT.TObject.kOverwrite)
            fout_D[iq].cd()
            adir.Get('data').Write('data',ROOT.TObject.kOverwrite)
            for system in systems: #including Nominal
                sigL = [adir.Get('wmunu_'+system),]
                allsamples = [adir.Get(sample+'_'+system) for sample in samples if sample!='wmunu'] + make_qcd(system) + sigL
                [ make_total(system,allsamples,fout_D[q],system,q==2 and iq==1) for q in (iq,2) ]
                #adir4.Get('wmunu_'+system).Write('wmunu_PowhegJimmy_'+system,ROOT.TObject.kOverwrite)
            # manually generate nominal histograms with qcd Up/Down variations
            # this only includes variations that are not correlated with the response matrix.
            system='Nominal'
            sigL = [adir.Get('wmunu_'+system),]
            allsamples = [adir.Get(sample+'_'+system) for sample in samples if sample!='wmunu'] + make_qcd('Nominal_qcd_up') + sigL
            [ make_total(system,allsamples,fout_D[q],'Nominal_qcd_up',q==2 and iq==1) for q in (iq,2) ]
            allsamples = [adir.Get(sample+'_'+system) for sample in samples if sample!='wmunu'] + make_qcd('Nominal_qcd_down') + sigL
            [ make_total(system,allsamples,fout_D[q],'Nominal_qcd_down',q==2 and iq==1) for q in (iq,2) ]
            # generate histograms for ewkbg xsec variations
            if True:
                bdir = fin.Get( '%s_xsecup'%FQNAMES[iq] )
                assert bdir
                sigL = [bdir.Get('wmunu_'+system),]
                allsamples = [bdir.Get(sample+'_'+system) for sample in samples if sample!='wmunu'] + make_qcd('Nominal') + sigL
                [ make_total(system,allsamples,fout_D[q],'Nominal_ewk_xsecup',q==2 and iq==1) for q in (iq,2) ]
                bdir = fin.Get( '%s_xsecdown'%FQNAMES[iq] )
                assert bdir
                allsamples = [bdir.Get(sample+'_'+system) for sample in samples if sample!='wmunu'] + make_qcd('Nominal') + sigL
                [ make_total(system,allsamples,fout_D[q],'Nominal_ewk_xsecdown',q==2 and iq==1) for q in (iq,2) ]
            # generate histograms for MC-based QCD
            if True:
                bdir = fin.Get('%s_qcdmc'%FQNAMES[iq])
                assert bdir
                sigL = [bdir.Get('wmunu_'+system),]
                sqcd = bdir.Get('qcd').Clone('qcd_Nominal_qcdmc'); sqcd.SetTitle('qcd_Nominal_qcdmc')
                allsamples = [bdir.Get(sample+'_'+system) for sample in samples if sample!='wmunu'] + [sqcd,] + sigL
                [ make_total(system,allsamples,fout_D[q],'Nominal_qcdmc',q==2 and iq==1) for q in (iq,2) ]
    fin.Close()
    fout.cd()
    sout = ROOT.TObjString(memo)
    sout.Write("memo")
    fout.Close()
