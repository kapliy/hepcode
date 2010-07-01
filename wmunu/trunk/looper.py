#!/usr/bin/env python
try:
    import psyco
    psyco.full()
    print 'Using psyco'
except ImportError:
    pass
import math,sys,glob,re
import ROOT
ROOT.gROOT.SetBatch(1) #uncomment for interactive usage

_BCIDs = (1,1786)
_INPUTS = '/pnfs/uct3/data/users/antonk/ANALYSIS/PLHC_MC/user10.AntonKapliy.mc09_7TeV.106044.PythiaWmunu_no_filter.merge.AOD.e468_s765_s767_r1207_r1210.ntuple.v1_7/user10.AntonKapliy.mc09_7TeV.106044.PythiaWmunu_no_filter.merge.AOD.e468_s765_s767_r1207_r1210.ntuple.v1_7.flatntuple._00001.root'

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
parser.add_option("--no-save",
                  action="store_true",dest="nosave",
                  help="Don't save any output on disk")
parser.add_option("--no-trigger",
                  action="store_true",dest="notrig",
                  help="Don't require trigger selection disk")
parser.add_option("--truth",
                  action="store_true",dest="truth",
                  help="Enable truth studies when running over MC")

(opts, args) = parser.parse_args()
_DATA = opts.data
_MC = not _DATA
_TRUTH = opts.truth
from helpers import *

# Set up GRL
PassRunLB = GoodRunsList.passRunLB__dummy
if opts.grlxml and _DATA:
    print 'Setting up GRL from these xml files:',opts.grlxml
    grl = GoodRunsList(opts.grlxml)
    PassRunLB=grl.passRunLB__real

plotdir=opts.output
if not os.path.isdir(plotdir):
    os.makedirs(plotdir)
candfile = open(os.path.join(plotdir,'cands.txt'),'w')

# truth
HistoQ("TRUTH_mu_pt",ptbins)
HistoQ("TRUTH_mu_eta",etabins)
HistoQ("TRUTH_mu_phi",phibins)
HistoQ("TRUTH_met",metbins)
# preselection
HistoQ("PRESEL_mu_pt",ptbins)
HistoQ("PRESEL_mu_eta",etabins)
HistoQ("PRESEL_mu_phi",phibins)
HistoQ("PRESEL_mu_ptiso",ptisobins)
HistoQ("PRESEL_met",metbins)
HistoQ("PRESEL_mu_pt_vs_met",ptbins,metbins)
# analysis level (after all cuts)
HistoQ("ANA_mu_pt",ptbins)
HistoQ("ANA_mu_eta",etabins)
HistoQ("ANA_mu_phi",phibins)
HistoQ("ANA_mu_ptiso",ptisobins)
HistoQ("ANA_met",metbins)
HistoQ("ANA_w_mt",mtbins)

t = ROOT.TChain('tree')
print 'Adding files to TChain:'
print opts.input
inputs = opts.input.split(',')
for input in inputs:
    for file in glob.glob(input):
        t.Add(addDcachePrefix(file))
nentries = t.GetEntries()
print 'Starting loop over',(opts.nevents if opts.nevents<nentries else nentries),'/',nentries,'entries'
ef = EventFlow()
t1 = time.time()
for evt in xrange(nentries):
    if evt>=opts.nevents:
        break
    if evt%1000==0:
        if evt!=0:
            print "Event %d (rate=%.1f Hz)"%(evt+opts.nskip,1000.0/(time.time()-t1))
        t1 = time.time()
    t.GetEntry(evt+opts.nskip)

    # per-event arrays of objects
    _ivertex = None; _zvertex = None
    _met = None
    _Tmus=[]
    _Tnus=[]
    _TWs=[]
    _Rmus=[]
    _RWs=[]

    # mc truth
    if _TRUTH and _MC:
        nmc,mc_status,mc_pdgid,mc_e,mc_pt,mc_eta,mc_phi = t.nmc,t.mc_status,t.mc_pdgid,t.mc_e,t.mc_pt,t.mc_eta,t.mc_phi
        if t.met_truth<25*GeV:
            ef.truthmet+=1
            continue
        for m in xrange(nmc):
            if mc_status[m]!=3:
                continue
            if mc_pdgid[m] in (MUON,-MUON):
                if fabs(mc_pt[m])>=20.0*GeV and fabs(mc_eta[m])<2.4:
                    charge = 'm' if mc_pdgid[m]==MUON else 'p'
                    v = ROOT.TLorentzVector()
                    v.SetPtEtaPhiE(mc_pt[m],mc_eta[m],mc_phi[m],mc_e[m])
                    v.charge = charge
                    _Tmus.append(v)
                    FillQ('TRUTH_mu_pt',charge,mc_pt[m])
                    FillQ('TRUTH_mu_phi',charge,mc_phi[m])
                    FillQ('TRUTH_mu_eta',charge,mc_eta[m])
                    FillQ('TRUTH_met',charge,t.met_truth)
            if mc_status[m]==3 and mc_pdgid[m] in (WPLUS,WMINUS):
                v = ROOT.TLorentzVector()
                v.SetPtEtaPhiE(mc_pt[m],mc_eta[m],mc_phi[m],mc_e[m])
                v.charge = 'p' if mc_pdgid[m]==WPLUS else 'm'
                _TWs.append(v)
            if mc_status[m]==3 and mc_pdgid[m] in (NEUTRINOS):
                v = ROOT.TLorentzVector()
                v.SetPtEtaPhiE(mc_pt[m],mc_eta[m],mc_phi[m],mc_e[m])
                v.charge = 'n'
                _Tnus.append(v)

    # truth cuts
    if _TRUTH and _MC:  # Wmunu truth cut
        if len(_Tmus)==0 or len(_TWs)==0:
            ef.truthmu+=1
            continue

    # GRL cuts
    if _DATA and not PassRunLB(t.run,t.lb):
        ef.grl+=1
        continue
    
    # bcid cuts [implicitly applied through trigger]
    if _DATA:
        if t.bcid==-1:
            ef.bcid+=1
            continue

    # jet cleaning cut
    if True:
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
            if jet_hecf[m]>0.8 and jet_n90[m]<6:
                failed=True
                break
        if failed:
            ef.jetclean+=1
            continue

    # trigger
    if True and not opts.notrig:
        if t.trig_l1mu0==0:
            ef.trigger+=1
            continue

   # primary vertex cut
    if True:
        nvx,vx_type,vx_ntracks = t.nvx,t.vx_type,t.vx_ntracks
        for m in xrange(nvx):
            if vx_type[m]==1 and vx_ntracks[m]>=3 and fabs(t.vx_z[m])<150:
                _ivertex = m
                _zvertex = t.vx_z[m]
                break
        if _ivertex==None:
            ef.vertex+=1
            continue

    # muon preselection + W cuts
    nmu,mu_author,mu_q,mu_pt,mu_eta,mu_phi,mu_ptcone40,mu_ptms,mu_ptexms,mu_qms,mu_ptid,mu_qid,mu_z0 = t.nmu,t.mu_author,t.mu_q,t.mu_pt,t.mu_eta,t.mu_phi,t.mu_ptcone40,t.mu_ptms,t.mu_ptexms,t.mu_qms,t.mu_ptid,t.mu_qid,t.mu_z0
    presel,quality,pt,iso=(True,)*4  # isFailed?
    for m in xrange(nmu):
        if mu_author[m]&2!=0 and mu_ptid[m]!=-1 and mu_q[m]!=0 and fabs(mu_eta[m])<2.4 and mu_pt[m]>15*GeV and fabs(mu_z0[m]-_zvertex)<10*mm:
            presel = False
            if mu_ptexms[m]>10*GeV and fabs(mu_ptexms[m]-mu_ptid[m])<15*GeV:
                quality = False
                ptcone40 = mu_ptcone40[m]/mu_pt[m]
                charge = 'p' if mu_q[m]==1 else 'm'
                FillQ('PRESEL_mu_pt',charge,mu_pt[m])
                FillQ('PRESEL_mu_phi',charge,mu_phi[m])
                FillQ('PRESEL_mu_eta',charge,mu_eta[m])
                FillQ('PRESEL_mu_ptiso',charge,ptcone40)
                FillQ('PRESEL_met',charge,t.met_ichep)
                FillQ('PRESEL_mu_pt_vs_met',charge,mu_pt[m],t.met_ichep)
                if mu_pt[m]>20*GeV:
                    pt=False
                    if ptcone40<0.2:
                        iso=False
                        v=ROOT.TLorentzVector()
                        v.SetPtEtaPhiM(mu_pt[m],mu_eta[m],mu_phi[m],muMASS)
                        v.charge=charge
                        v.ptcone40 = ptcone40
                        _Rmus.append(v)

    # muon preselection
    if presel:
        ef.muonpresel+=1
        continue
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
                if M>zMASS-10*GeV and M<zMASS+10*GeV:
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
        cands=[]  # [mu_index,mW]
        for i,mu in enumerate(_Rmus):
            if True: #fabs(mu.Phi()-_met.Phi())>math.pi/2.0:  # dPhi cut on MET and muon
                mW=math.sqrt( 2*mu.Pt()*_met.Pt()*(1-math.cos(_met.Phi()-mu.Phi())) )
                cands.append((i,mW))
        # choose the closest W candidate:
        mdiff,mW,i=closest(wMASS,cands)
        mu=_Rmus[cands[i][0]]
        FillQ('ANA_w_mt',mu.charge,mW)
        print >>candfile, '%d\t%d\t%d\t%s\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f '%(t.run,t.lb,t.event,mu.charge,mu.Pt(),mu.Eta(),mu.Phi(),mu.ptcone40,_met.Pt(),mW)
        # reconstructed a good W:
        if (mW>wMIN and mW<wMAX):
            ef.ok+=1
            FillQ('ANA_mu_pt',mu.charge,mu.Pt())
            FillQ('ANA_mu_phi',mu.charge,mu.Phi())
            FillQ('ANA_mu_eta',mu.charge,mu.Eta())
            FillQ('ANA_mu_ptiso',mu.charge,mu.ptcone40)
            FillQ('ANA_met',mu.charge,_met.Pt())
            #ws=RecoW(mu.E(),mu.Px(),mu.Py(),mu.Pz(),_met.Px(),_met.Py())
            #_RWs+=ws
            # check with truth
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
# write out candidate events
candfile.close()

if True:
    # plot distributions overlayed
    if "TMU_eta_p" in h:
        PlotOverlayHistos(h["TMU_eta_p"],h["TMU_eta_m"],'True muon eta: red=mu+, blue=mu-','TMU_eta__overlay')
        PlotOverlayHistos(h["TMU_eta_p"],h["TMU_eta_m"],'True muon eta: red=mu+, blue=mu-','TMU_eta__norm',norm=True)
        PlotOverlayHistos(h["TMU_pt_p"],h["TMU_pt_m"],'True muon pt: red=mu+, blue=mu-','TMU_pt__overlay')
        PlotOverlayHistos(h["TMU_pt_p"],h["TMU_pt_m"],'True muon pt: red=mu+, blue=mu-','TMU_pt__norm',norm=True)
    if "RMU_eta_p" in h:
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
elif _TRUTH and _MC:
    # make efficiency histograms
    for var in 'eta','pt':
        for q in 'p','m':
            try:
                MakeEffHistos(h["PRESEL_mu_%s_%s"%(var,q)],h["TRUTH_mu_%s_%s"%(var,q)],'mu_%s_%s'%(var,q))
            except KeyError:
                continue
        try:
            PlotOverlayHistos(h["EFF_mu_%s_p"%var],h["EFF_mu_%s_m"%var],'Reconstruction efficiency vs %s: red=mu+, blue=mu-'%var,'EFF_mu_%s__overlay'%var,plotdir=plotdir)
            PlotOverlayHistos(h["EFF_mu_%s_p"%var],h["EFF_mu_%s_m"%var],'Reconstruction efficiency vs %s: red=mu+, blue=mu-'%var,'EFF_mu_%s__norm'%var,norm=True,plotdir=plotdir)
        except KeyError:
            continue
    
# asymmetries, ratios, efficiencies (uncorrected for relative W+/W- reconstruction efficiency)
if False:
    for var in 'eta','pt':
        try:
            MakeAsymmRatioHistos(h["ANA_mu_%s_p"%var],h["ANA_mu_%s_m"%var],'ANA_%s__uncorrected'%var)
        except KeyError:
            continue

if not opts.nosave:
    if not os.path.isdir(plotdir):
        os.makedirs(plotdir)
    out = ROOT.TFile(os.path.join(plotdir,'./out.root'),"RECREATE")
    for hh in h.values():
        hh.Write()
        savePlot(hh,plotdir)
    out.Close()
    ef.Print(open(os.path.join(plotdir,'./cuts.txt'),'w'))
    ef.Print(open('/dev/stdout','w'))
