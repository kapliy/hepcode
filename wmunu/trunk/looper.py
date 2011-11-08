#!/usr/bin/env python
import sys
print sys.argv

try:
    import psyco
    psyco.full()
except ImportError:
    pass

_INPUTS = '/atlas/uct3/data/users/antonk/NTUPLE/v1_27e_pdf/user.kapliy.mc11_7TeV.106044.PythiaWmunu_no_filter.merge.AOD.e815_s1272_s1274_r2730_r2700.ntuple.v1_27e.111105023134/user.kapliy.005352.flatntuple._00003.root'
_INPUTS = ','.join(['/atlas/uct3/data/users/antonk/NTUPLE/v1_27e_pdf/user.kapliy.mc11_7TeV.106044.PythiaWmunu_no_filter.merge.AOD.e815_s1272_s1274_r2730_r2700.ntuple.v1_27e.111105023134/user.kapliy.005352.flatntuple._000%02d.root'%j for j in range(1,21)])

from optparse import OptionParser
parser = OptionParser()
parser.add_option("-n", "--nevents",dest="nevents",
                  type="int", default=100000000,
                  help="Maximum number of events to process")
parser.add_option("-s", "--nskip",dest="nskip",
                  type="int", default=0,
                  help="Number of events to skip")
parser.add_option("-i", "--input",dest="input",
                  type="string", default=_INPUTS,
                  help="Comma-separated list of ntuples to process")
parser.add_option("-o", "--output",dest="output",
                  type="string", default="output",
                  help="Name of output dir for plots and cutflow")
parser.add_option("-x","--xml-grl",dest="grlxml",
                  type="string", default=None,
                  help="Comma-separated list of GRL xml files")
parser.add_option("--effmap",dest="effmap",
                  type="string", default=None,
                  help="Path to root file containing efficiency maps")
parser.add_option("--data", default=False,
                  action="store_true",dest="data",
                  help="Whether running over data (vs MC)")
parser.add_option("--make-plots",default=False,
                  action="store_true",dest="makeplots",
                  help="Save all plots on disk in the end of the run")
parser.add_option("--trigger",dest="trigger",
                  type='string', default='L1_MU10',
                  help="Choose L1 trigger, such as L1_MU6 or L1_MU10")
parser.add_option("--truthcuts",
                  action="store_true",dest="truthcuts",
                  help="Enable truth cut studies when running over MC")
parser.add_option("-p", "--pdfset",dest="pdfset",
                  type="int", default=0,
                  help="pdfset to reweight to")

(opts, args) = parser.parse_args()
_DATA = opts.data
_MC = not _DATA
_TRUTHCUTS = True
_PDFWEIGHT = True
trigger = opts.trigger
from helpers import *
try:
    ROOT.PyConfig.IgnoreCommandLineOptions = True
except:
    pass
ROOT.gROOT.SetBatch(1) #uncomment for interactive usage

# Set up GRL
PassRunLB = GoodRunsList.passRunLB__dummy
if opts.grlxml and _DATA:
    print 'Setting up GRL from these xml files:',opts.grlxml
    grl = GoodRunsList(opts.grlxml)
    PassRunLB=grl.passRunLB__real

# prepare output
plotdir=opts.output
if not os.path.isdir(plotdir):
    os.makedirs(plotdir)
out = AnaFile(os.path.join(plotdir,'./out.root'),mode="RECREATE")
out.MakeCandTree()

HistoQ("TD0_mu_eta",etabins)
HistoQ("TD0_mu_abseta",absetabins)
HistoQ("TD0_w_mt",mtbins)

if False:
    # truth
    HistoQ("TRUTH_mu_pt",ptbins)
    HistoQ("TRUTH_mu_eta",etabins)
    HistoQ("TRUTH_mu_phi",phibins)
    HistoQ("TRUTH_w_pt",ptbins)
    HistoQ("TRUTH_w_eta",etabins)
    HistoQ("TRUTH_w_phi",phibins)
    HistoQ("TRUTH_w_mt",phibins)
    # after event-quality cuts but before any muon / met cuts
    Histo("EVENT_njets",nobjbins)
    Histo("EVENT_nmus",nobjbins)
    Histo("EVENT_met",metbins)
    Histo('EVENT_mu_ptms_ptid',dptbins)
    # preselection
    HistoQ("PRESEL_mu_pt",ptbins)
    HistoQ("PRESEL_mu_eta",etabins)
    HistoQ("PRESEL_mu_etavar",etavarbins)
    HistoQ("PRESEL_mu_phi",phibins)
    HistoQ("PRESEL_mu_ptiso",ptisobins)
    HistoQ("PRESEL_met",metbins)
    HistoQ("PRESEL_mu_pt_vs_met",ptbins,metbins)
    HistoQ("PRESEL_mu_iso_vs_met",ptisobins,metbins)
    HistoQ("PRESEL_mu_eta_vs_phi",etabins,phibins)
    Histo("PRESEL_njets",nobjbins)
    # pre-analysis (before mT cut) and analysis (after all cuts)
    for lvl in ('PREANA','ANA'):
        HistoQ("%s_mu_pt"%lvl,ptbins)
        HistoQ("%s_mu_eta"%lvl,etabins)
        HistoQ("%s_mu_phi"%lvl,phibins)
        HistoQ("%s_mu_ptiso"%lvl,ptisobins)
        HistoQ("%s_met"%lvl,metbins)
        HistoQ("%s_mu_pt_vs_met"%lvl,ptbins,metbins)
        HistoQ("%s_mu_iso_vs_met"%lvl,ptisobins,metbins)
        HistoQ("%s_w_mt_vs_met"%lvl,mtbins,metbins)
        HistoQ("%s_w_mt"%lvl,mtbins)
        HistoQ("%s_w_pt"%lvl,ptbins)
        HistoQ("%s_mu_etavar"%lvl,etavarbins)
        Histo("%s_mu_q"%lvl,qbins)
        Histo("%s_njets"%lvl,nobjbins)

# prepare input
t = ROOT.TChain('tree')
print 'Adding files to TChain:'
print '['+opts.input+']'
inputs = opts.input.split(',')
totinputs = 0
for input in inputs:
    for file in glob.glob(input):
        t.Add(addDcachePrefix(file))
        totinputs+=1
print 'Added a total of %d files'%totinputs
SetTreeBranches_V27(t,doTruth=True,doReco=False)

# prepare PDF reweighting stuff
pdfsets = []
if _PDFWEIGHT:
    import lhapdf
    LHPDF,LHGRID=0,1
    pdfsets = ['HERAPDF10_EIG','MSTW2008nlo68cl','NNPDF21_100','cteq66','MRST2007lomod']
    pdfname_orig=pdfsets[3] #kristin (MC@NLO?)
    pdfname_orig=pdfsets[4] #pythia
    pdfname_reweight = pdfsets[1]
    lhapdf.initPDFSetByName(1,pdfname_orig,LHGRID);
    lhapdf.initPDFSetByName(2,pdfname_reweight,LHGRID);
    pdfnum1 = lhapdf.numberPDF(1)+1
    pdfnum2 = lhapdf.numberPDF(2)+1
    print 'Loaded PDF sets of length:',pdfnum1,pdfnum2
    pdfsets = [opts.pdfset,]
    for j in pdfsets:
        HistoQ("TW%d_mu_eta"%j,etabins)
        HistoQ("TW%d_mu_abseta"%j,absetabins)
        HistoQ("TW%d_w_mt"%j,mtbins)

# prepare cutflow
#cuts=['truthmet','truthmu','truthwmt','grl','bcid','trigger','vertex','jetclean','muonpresel','muonqual','muonpt','muoniso','met','zcut','wmt','ok']
cuts = ['mcevt','met','muon','wmt','ok']
ef = EventFlow(cuts)

nentries = t.GetEntries()
niters = opts.nevents if opts.nevents<nentries else nentries
import SimpleProgressBar
bar = SimpleProgressBar.SimpleProgressBar(20,niters)
print 'Starting loop over',niters,'/',nentries,'entries'
t1 = time.time()
nstat=1000
sys.stderr.flush()
for evt in xrange(niters):
    if evt%nstat==0:
        if evt!=0:
            print "%s: event %6d, rate %.1f Hz"%(bar.show(evt),evt+opts.nskip,nstat*1.0/(time.time()-t1))
            sys.stdout.flush()
        t1 = time.time()
    t.GetEntry(evt+opts.nskip)

    # per-event arrays of objects
    _ivertex = None; _zvertex = None
    _Tmet = None
    _Tmus=[]
    _Tnus=[]
    _TWs=[]

    if _MC:
        # variables for PDF re-weighting
        nmcevt,amcevt_pdf_scale,amcevt_pdf_x1,amcevt_pdf_x2,amcevt_pdf_id1,amcevt_pdf_id2 = t.nmcevt,t.mcevt_pdf_scale,t.mcevt_pdf_x1,t.mcevt_pdf_x2,t.mcevt_pdf_id1,t.mcevt_pdf_id2
        if nmcevt<=0:
            ef.mcevt+=1
            continue
        mcevt_pdf_scale,mcevt_pdf_x1,mcevt_pdf_x2,mcevt_pdf_id1,mcevt_pdf_id2 = amcevt_pdf_scale[0],amcevt_pdf_x1[0],amcevt_pdf_x2[0],amcevt_pdf_id1[0],amcevt_pdf_id2[0]

        # truth-MET
        nmc,mc_status,mc_pdgid,mc_e,mc_pt,mc_eta,mc_phi,mc_parent = t.nmc,t.mc_status,t.mc_pdgid,t.mc_e,t.mc_pt,t.mc_eta,t.mc_phi,t.mc_parent
        met_truth,met_truth_phi = t.met_truth,t.met_truth_phi
        if met_truth<25*GeV:
            ef.met+=1
            continue
        _Tmet=ROOT.TVector2()
        _Tmet.SetMagPhi(met_truth,met_truth_phi)
        _Tmet.Pt = _Tmet.Mod  # define Pt() since TVector2 doesn't have it

        # truth particles
        for m in xrange(nmc):
            if mc_status[m]!=1:  #3
                continue
            # w's don't have a parent with mc_status==3
            if mc_pdgid[m] in (WPLUS,WMINUS):
                v = ROOT.TLorentzVector()
                v.SetPtEtaPhiE(mc_pt[m],mc_eta[m],mc_phi[m],mc_e[m])
                v.charge = charge = 'p' if mc_pdgid[m]==WPLUS else 'm'
                _TWs.append(v)
            mp = mc_parent[m]
            if False and mp:
                continue
            if mc_pdgid[m] in (MUON,-MUON): # and mc_pdgid[mp] in (WPLUS,WMINUS):
                if fabs(mc_pt[m])>=20.0*GeV and fabs(mc_eta[m])<2.4:
                    v = ROOT.TLorentzVector()
                    v.SetPtEtaPhiE(mc_pt[m],mc_eta[m],mc_phi[m],mc_e[m])
                    v.charge = charge = 'm' if mc_pdgid[m]==MUON else 'p'
                    _Tmus.append(v)
            if False and mp>0 and mc_status[m]==3 and mc_pdgid[m] in (NEUTRINOS) and mc_pdgid[mp] in (WPLUS,WMINUS):
                v = ROOT.TLorentzVector()
                v.SetPtEtaPhiE(mc_pt[m],mc_eta[m],mc_phi[m],mc_e[m])
                v.charge = charge = 'p' if mc_pdgid[m] in NEUTRINOS[3:] else 'm'
                _Tnus.append(v)

        # truth cuts
        if len(_Tmus)==0:
            ef.muon+=1
            continue
        _Tmus.sort(key=lambda p: p.Pt(),reverse=True)
        _TWs.sort(key=lambda p: p.Pt(),reverse=True)

        # only the highest-pt muon is used to construct W candidates:
        _Tmus = _Tmus[:1]
        cands=[]  # [mu_index,mW]
        for i,mu in enumerate(_Tmus):
            if True: #fabs(mu.Phi()-_Tmet.Phi())>math.pi/2.0:  # dPhi cut on MET and muon
                mW=sqrt( 2*mu.Pt()*_Tmet.Pt()*(1-cos(_Tmet.Phi()-mu.Phi())) )
                cands.append((i,mW))
        # choose the closest W candidate:
        mdiff,mtW,i=closest(wMASS,cands)
        mu,met =_Tmus[cands[i][0]],_Tmet.Pt()
        charge = mu.charge
        eta = mu.Eta()
        ptW = sqrt(mu.Pt()**2+met**2+2*mu.Pt()*met*cos(mu.Phi()-_Tmet.Phi()))

        # pdf reweighting
        if _PDFWEIGHT:
            weights = []
            partonID1 = mcevt_pdf_id1
            if partonID1==21:
                partonID1=0 # gluon is ==0 in LHAPDF, but in normal HepMC it is stored as 21 --> so redefine!
            partonID2 = mcevt_pdf_id2
            if partonID2==21:
                partonID2=0
            partonX1 = mcevt_pdf_x1
            partonX2 = mcevt_pdf_x2
            eventQ = mcevt_pdf_scale
            lhapdf.initPDF(1,0)
            p1density_orig = lhapdf.xfx(1,partonX1, eventQ, partonID1)
            p2density_orig = lhapdf.xfx(1,partonX2, eventQ, partonID2)
            for j in pdfsets:
                lhapdf.initPDF(2,j)
                p1density = lhapdf.xfx(2, partonX1, eventQ, partonID1);
                p2density = lhapdf.xfx(2, partonX2, eventQ, partonID2);
                pdfweight = (p1density*p2density)/(p1density_orig*p2density_orig);
                weights.append(pdfweight);
                FillQ1D("TW%d_mu_eta"%j,charge,eta,pdfweight)
                FillQ1D("TW%d_mu_abseta"%j,charge,fabs(eta),pdfweight)
                FillQ1D("TW%d_w_mt"%j,charge,mtW,pdfweight)
        # save histograms
        FillQ1D("TD0_mu_eta",charge,eta)
        FillQ1D("TD0_mu_abseta",charge,fabs(eta))
        FillQ1D("TD0_w_mt",charge,mtW)

    ef.ok += 1
    continue


    # RECO STUFF KEPT BELOW FOR HISTORICAL PURPOSES;
    # IT IS NOW RUN INSIDE TRIGFTKANA INSTEAD

    _met = None
    _Rmus=[]
    _RWs=[]

    # GRL cuts
    if _DATA and not PassRunLB(t.run,t.lb):
        ef.grl+=1
        continue
    
    # bcid cuts [implicitly applied through trigger]
    if _DATA:
        if t.bcid==-1:
            ef.bcid+=1
            continue

    if False:
        for v in t.trig_ef:
            if re.search('mu',v) or re.search('MU',v):
                print v
        for v in t.trig_l1:
            if re.search('mu',v) or re.search('MU',v):
                print v
        sys.exit(0)

    # trigger
    if True:
        if trigger not in t.trig_l1 and trigger not in t.trig_ef:
            ef.trigger+=1
            continue

   # find primary vertex (aka highest sum-pt) and apply some cuts to it
    if True:
        #nvx,vx_type,vx_ntracks,vx_sumpt,vx_z = t.nvx,t.vx_type,t.vx_ntracks,t.vx_sumpt,t.vx_z
        #sumpt,m=max((x,i) for i,x in enumerate([vx_sumpt[m] for m in xrange(nvx)])) if nvx>0 else (None,None)
        nvx,vx_type,vx_ntracks,vx_z = t.nvx,t.vx_type,t.vx_ntracks,t.vx_z
        _vfound = False
        for m in xrange(nvx):
            if vx_type[m]==1 and vx_ntracks[m]>=3 and fabs(vx_z[m])<150:
                _vfound = True
                break
        if _vfound:
            _ivertex = m
            _zvertex = t.vx_z[m]
        else:
            ef.vertex+=1
            continue

    # jet cleaning cut
    if _DATA:
        njet,jet_n90,jet_quality,jet_time,jet_emf,jet_hecf,jet_pt_em=t.njet,t.jet_n90,t.jet_quality,t.jet_time,t.jet_emf,t.jet_hecf,t.jet_pt_em
        failed=False
        for m in range(njet):
            if jet_pt_em[m]<10:
                continue
            if jet_quality[m]>0.8 and jet_emf[m]>0.95:
                failed=True
                break
            if jet_time[m]>50:
                failed=True
                break
            if jet_hecf[m]>0.8 and jet_n90[m]<=5:
                failed=True
                break
        if failed:
            ef.jetclean+=1
            continue

    # event-wide histograms
    Fill('EVENT_njets',t.njet)
    Fill('EVENT_nmus',t.nmu)
    Fill('EVENT_met',t.met_ichep)

    # muon preselection + W cuts
    #TODO - mu class!!!
    nmu,mu_author,mu_class,mu_q,mu_pt,mu_eta,mu_phi,mu_ptcone40,mu_ptms,mu_ptexms,mu_qms,mu_ptid,mu_qid,mu_z0 = t.nmu,t.mu_author,t.mu_class,t.mu_q,t.mu_pt,t.mu_eta,t.mu_phi,t.mu_ptcone40,t.mu_ptms,t.mu_ptexms,t.mu_qms,t.mu_ptid,t.mu_qid,t.mu_z0
    presel,quality,pt,iso=(True,)*4  # isFailed?
    for m in xrange(nmu):
        if mu_author[m]&2!=0 and mu_class[m]==1 and fabs(mu_eta[m])<=2.4 and mu_pt[m]>15.0*GeV and fabs(mu_z0[m]-_zvertex)<10.0*mm and mu_ptid[m]!=-1 and mu_q[m]!=0:
            presel = False
            Fill('EVENT_mu_ptms_ptid',mu_ptexms[m]-mu_ptid[m])
            if mu_ptexms[m]>10.0*GeV and fabs(mu_ptexms[m]-mu_ptid[m])<15.0*GeV:
                quality = False
                ptcone40 = mu_ptcone40[m]/mu_pt[m]
                charge = 'p' if mu_q[m]==1 else 'm'
                FillQ('PRESEL_mu_pt',charge,mu_pt[m])
                FillQ('PRESEL_mu_phi',charge,mu_phi[m])
                FillQ('PRESEL_mu_eta',charge,mu_eta[m])
                FillQ('PRESEL_mu_etavar',charge,mu_eta[m])
                FillQ('PRESEL_mu_ptiso',charge,ptcone40)
                FillQ('PRESEL_met',charge,t.met_ichep)
                FillQ('PRESEL_mu_pt_vs_met',charge,mu_pt[m],t.met_ichep)
                FillQ("PRESEL_mu_iso_vs_met",charge,ptcone40,t.met_ichep)
                FillQ('PRESEL_mu_eta_vs_phi',charge,mu_eta[m],mu_phi[m])
                if mu_pt[m]>20.0*GeV:
                    pt=False
                    if ptcone40<0.2:
                        iso=False
                        v=ROOT.TLorentzVector()
                        v.SetPtEtaPhiM(mu_pt[m],mu_eta[m],mu_phi[m],muMASS)
                        v.charge=charge
                        v.ptcone40 = ptcone40
                        _Rmus.append(v)
    _Rmus.sort(key=lambda p: p.Pt(),reverse=True)
    
    # muon preselection
    if presel:
        ef.muonpresel+=1
        continue
    Fill('PRESEL_njets',t.njet)
    # muon quality (MS-ID pt cuts)
    if quality:
        ef.muonqual+=1
        continue
    # muon pt
    if pt:
        ef.muonpt+=1
        continue
    # muon isolation
    if iso:
        ef.muoniso+=1
        continue

    # met from met_topo + met_muon
    if True:
        et_ichep,et_ichep_phi = t.met_ichep,t.met_ichep_phi
        _met=ROOT.TVector2()
        _met.SetMagPhi(et_ichep,et_ichep_phi)
        _met.Pt = _met.Mod  # define Pt() since TVector2 doesn't have it
        if _met.Pt()<25*GeV:
            ef.met+=1
            continue

    # explicitly remove events with a Z->mumu
    if False:
        if len(_Rmus)>=2:
            cands=[]  # [mZ]
            _fail=False
            for m1,m2 in pairs(_Rmus):
                M=(m1+m2).M()
                if M>zMIN and M<zMAX:
                    _fail=True
                    break
                cands.append(M)
            if _fail:
                ef.zcut+=1
                continue
            else:
                mdiff,mZ,i=closest2(zMASS,cands)

    # W candidates in W->munu decay
    if True:
        # only the highest-pt muon is used:
        _Rmus = _Rmus[:1]
        cands=[]  # [mu_index,mW]
        for i,mu in enumerate(_Rmus):
            if True: #fabs(mu.Phi()-_met.Phi())>math.pi/2.0:  # dPhi cut on MET and muon
                mW=sqrt( 2*mu.Pt()*_met.Pt()*(1-cos(_met.Phi()-mu.Phi())) )
                cands.append((i,mW))
        # choose the closest W candidate:
        mdiff,mtW,i=closest(wMASS,cands)
        mu,met =_Rmus[cands[i][0]],_met.Pt()
        charge = mu.charge
        ptW = sqrt(mu.Pt()**2+met**2+2*mu.Pt()*met*cos(mu.Phi()-_met.Phi()))
        if _DATA:
            out.AddCand(run=t.run,event=t.event,lb=t.lb,charge=chargemap[charge],mu_pt=mu.Pt(),mu_eta=mu.Eta(),mu_phi=mu.Phi(),mu_ptcone40=mu.ptcone40,met=met,w_mt=mtW,w_pt=ptW)
        FillQ('PREANA_w_mt',charge,mtW)
        FillQ('PREANA_w_pt',charge,ptW)
        FillQ('PREANA_mu_pt',charge,mu.Pt())
        FillQ('PREANA_mu_phi',charge,mu.Phi())
        FillQ('PREANA_mu_eta',charge,mu.Eta())
        FillQ('PREANA_mu_etavar',charge,mu.Eta())
        FillQ('PREANA_mu_ptiso',charge,mu.ptcone40)
        FillQ('PREANA_met',charge,met)
        FillQ("PREANA_mu_pt_vs_met",charge,mu.Pt(),met)
        FillQ("PREANA_mu_iso_vs_met",charge,mu.ptcone40,met)
        FillQ("PREANA_w_mt_vs_met",charge,mtW,met)
        # reconstructed a good W:
        if (mtW>wMIN and mtW<wMAX):
            ef.ok+=1
            FillQ('ANA_w_mt',charge,mtW)
            FillQ('ANA_w_pt',charge,ptW)
            FillQ('ANA_mu_pt',charge,mu.Pt())
            FillQ('ANA_mu_phi',charge,mu.Phi())
            Fill('ANA_mu_q',1 if charge=='p' else -1)
            FillQ('ANA_mu_eta',charge,mu.Eta())
            FillQ('ANA_mu_etavar',charge,mu.Eta())
            FillQ('ANA_mu_ptiso',charge,mu.ptcone40)
            FillQ('ANA_met',charge,met)
            FillQ("ANA_mu_pt_vs_met",charge,mu.Pt(),met)
            FillQ("ANA_mu_iso_vs_met",charge,mu.ptcone40,met)
            FillQ("ANA_w_mt_vs_met",charge,mtW,met)
            Fill('ANA_njets',t.njet)
            #ws=RecoW(mu.E(),mu.Px(),mu.Py(),mu.Pz(),_met.Px(),_met.Py())
        else:
            ef.wmt+=1
            continue

    if False:
        # Checking that W mass reconstruction algorithm really has fakes
        if len(_TWs)==1 and len(_Tmus)==1 and len(_Tnus)==1 and len(_RWs)>0:
            mu=_Tmus[0]
            nu=_Tnus[0]
            wmunu=mu+nu # 4vector sum
            print '-------------------------'
            print 'nupT/MET = %.2f/%.2f = %.2f'%(nu.Pt(),_met.Pt(),nu.Pt()/_met.Pt())
            for w in _TWs:
                Print4vec('TRUTH             ',w)
            Print4vec('TRUTH MU+NU->W    ',wmunu)
            for w in RecoW(mu.E(),mu.Px(),mu.Py(),mu.Pz(),nu.Px(),nu.Py(),_TWs[0].M()):
                Print4vec('TRECO(correct)    ',w)
            for w in RecoW(mu.E(),mu.Px(),mu.Py(),mu.Pz(),nu.Px(),nu.Py()):
                Print4vec('TRECO(wrong Mw)   ',w)
            for w in RecoW(mu.E(),mu.Px(),mu.Py(),mu.Pz(),_met.Px(),_met.Py(),_TWs[0].M()):
                Print4vec('TRECO(wrong MET)  ',w)
            for w in RecoW(mu.E(),mu.Px(),mu.Py(),mu.Pz(),_met.Px(),_met.Py()):
                Print4vec('TRECO(wrong both) ',w)
            for w in _RWs:
                Print4vec('RECO              ',w)


# merge per-charge histograms
MergeHistoQ()

if False:
    # plot distributions overlayed
    if hgood("TMU_eta_p"):
        PlotOverlayHistos(h["TMU_eta_p"],h["TMU_eta_m"],'True muon eta: red=mu+, blue=mu-','TMU_eta__overlay')
        PlotOverlayHistos(h["TMU_eta_p"],h["TMU_eta_m"],'True muon eta: red=mu+, blue=mu-','TMU_eta__norm',norm=True)
        PlotOverlayHistos(h["TMU_pt_p"],h["TMU_pt_m"],'True muon pt: red=mu+, blue=mu-','TMU_pt__overlay')
        PlotOverlayHistos(h["TMU_pt_p"],h["TMU_pt_m"],'True muon pt: red=mu+, blue=mu-','TMU_pt__norm',norm=True)
    if hgood("RMU_eta_p"):
        PlotOverlayHistos(h["RMU_eta_p"],h["RMU_eta_m"],'Reco accepted muon eta: red=mu+, blue=mu-','RMU_eta__overlay')
        PlotOverlayHistos(h["RMU_eta_p"],h["RMU_eta_m"],'Reco accepted muon eta: red=mu+, blue=mu-','RMU_eta__norm',norm=True)
        PlotOverlayHistos(h["RMU_pt_p"],h["RMU_pt_m"],'Reco accepted muon pt: red=mu+, blue=mu-','RMU_pt__overlay')
        PlotOverlayHistos(h["RMU_pt_p"],h["RMU_pt_m"],'Reco accepted muon pt: red=mu+, blue=mu-','RMU_pt__norm',norm=True)

if opts.effmap:
    # load efficiency histograms from file
    f=ROOT.TFile(opts.effmap)
    if not f.IsOpen():
        print "Ooops, couldn't open",_LOAD_EFF
        sys.exit(0)
    print 'Loading efficiency templates from',_LOAD_EFF
    ROOT.gROOT.cd() 
    h["EFF_mu_eta_p"]=f.Get("EFF_mu_eta_p").Clone()
    h["EFF_mu_eta_m"]=f.Get("EFF_mu_eta_m").Clone()
    h["EFF_mu_pt_p"]=f.Get("EFF_mu_pt_p").Clone()
    h["EFF_mu_pt_m"]=f.Get("EFF_mu_pt_m").Clone()
    f.Close()
elif _TRUTHCUTS and _MC:
    # make efficiency histograms
    for var in 'eta','pt':
        for q in 'p','m':
            if hgood("PRESEL_mu_%s_%s"%(var,q)) and hgood("TRUTH_mu_%s_%s"%(var,q)):
                MakeEffHistos(h["PRESEL_mu_%s_%s"%(var,q)],h["TRUTH_mu_%s_%s"%(var,q)],'mu_%s_%s'%(var,q))
        try:
            PlotOverlayHistos(h["EFF_mu_%s_p"%var],h["EFF_mu_%s_m"%var],'Reconstruction efficiency vs %s: red=mu+, blue=mu-'%var,'EFF_mu_%s__overlay'%var,plotdir=plotdir)
            PlotOverlayHistos(h["EFF_mu_%s_p"%var],h["EFF_mu_%s_m"%var],'Reconstruction efficiency vs %s: red=mu+, blue=mu-'%var,'EFF_mu_%s__norm'%var,norm=True,plotdir=plotdir)
        except KeyError:
            continue

# asymmetries, ratios, efficiencies (uncorrected for relative W+/W- reconstruction efficiency)
if True:
    for var in 'eta','abseta':
        try:
            MakeAsymmRatioHistos(h["TD0_mu_%s_p"%var],h["TD0_mu_%s_m"%var],'TD0_mu_%s'%var)
        except KeyError:
            continue
        for j in pdfsets:
            try:
                MakeAsymmRatioHistos(h["TW%d_mu_%s_p"%(j,var)],h["TW%d_mu_%s_m"%(j,var)],'TW%d_mu_%s'%(j,var))
            except KeyError:
                continue

# save all output
ef.Print(open(os.path.join(plotdir,'./cuts.txt'),'w'))
ef.Print()
print 'Writing results to disk. Please wait...'
out.cd()
out.SaveCandTree()
out.SaveCutFlow(ef)
out.SaveHistos(h)
if opts.makeplots:
    out.PrintHistos(plotdir)
out.SaveMeta(nevents=niters,input=opts.input,grl=opts.grlxml if opts.grlxml else 'None',isdata='1' if opts.data else '0')

#done
out.Close()
