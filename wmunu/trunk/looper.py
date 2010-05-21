#!/usr/bin/env python
try:
    import psyco
    psyco.full()
    print 'Using psyco'
except:
    pass
import math,sys,glob,re
import ROOT
ROOT.gROOT.SetBatch(1) #uncomment for interactive usage

#_GOODLIST = ( 21677784,22180828,29555751,29874999 )
#_GOODEVTS = ( 255497,261005,344577,347861 )
#INPUTS = 'dcache:///pnfs/uct3/data/users/antonk/ANALYSIS/PLHC/153565/*root*'

_BCIDs = (1,1786)
_DATA = False

_INPUTS = '/scratch/antonk/zmumu.root'
_INPUTS = '/scratch/antonk/wmunu.root'
_INPUTS = 'dcache:///pnfs/uct3/data/users/antonk/ANALYSIS/PLHC_MC/user10.AntonKapliy.mc09_7TeV.106044.PythiaWmunu_no_filter.merge.AOD.e468_s765_s767_r1207_r1210.ntuple.v1_7/user10.AntonKapliy.mc09_7TeV.106044.PythiaWmunu_no_filter.merge.AOD.e468_s765_s767_r1207_r1210.ntuple.v1_7.flatntuple._00001.root'
#_LOAD_EFF = './efficiencies.root'

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
                  help="Full path to ntuple to process")
parser.add_option("-o", "--output",dest="output",
                  type="string", default="output",
                  help="Name of output dir for plots and cutflow")
parser.add_option("-u", "--no-save",
                  action="store_true",dest="nosave",
                  help="Don't save any output on disk")
parser.add_option("-t", "--no-trigger",
                  action="store_true",dest="notrig",
                  help="Don't require trigger selection disk")
parser.add_option("-g", "--grl",dest="grl",
                  type="int", default=None,
                  help="Run number of MC or DATA dataset from GRL.py")

(opts, args) = parser.parse_args()
from GRL import mc09,plhc
if opts.grl!=None:
    rc = [r for r in mc09.runs if r.rnum==opts.grl]
    rc += [r for r in plhc.runs if r.rnum==opts.grl]
    if len(rc)==1:
        r = rc[0]
        opts.output = str(r.rnum)
        opts.input = r.path()
        print 'Expecting',r.nevents,'events'
    else:
        print "Couldn't find run",opts.grl
        sys.exit(0)
from helpers import *

plotdir=opts.output
# preselection
Histo("PRESEL_mu_pt",ptbins)
Histo("PRESEL_mu_eta",etabins)
Histo("PRESEL_mu_phi",phibins)
Histo("PRESEL_mu_ptiso",ptisobins)
Histo("PRESEL_met",metbins)
Histo("PRESEL_mu_pt_vs_met",ptbins,metbins)
# analysis level muons
Histo("ANA_mu_pt",ptbins)
Histo("ANA_mu_eta",etabins)
Histo("ANA_mu_phi",phibins)
Histo("ANA_mu_ptiso",ptisobins)
Histo("ANA_w_mt",mtbins)
Histo("ANA_met",metbins)

t = ROOT.TChain('tree')
print 'Adding files to TChain:'
print opts.input
t.Add(opts.input)
nentries = t.GetEntries()

print 'Starting loop over',(opts.nevents if opts.nevents<nentries else nentries),'/',nentries,'entries'
ef = EventFlow()
for evt in range(nentries):
    if evt>=opts.nevents:
        break
    if evt%1000==0:
        print "Event",evt+opts.nskip
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
    if False and not _DATA:
        nmc,mc_status,mc_pdgid,mc_e,mc_pt,mc_eta,mc_phi = t.nmc,t.mc_status,t.mc_pdgid,t.mc_e,t.mc_pt,t.mc_eta,t.mc_phi
        for m in range(nmc):
            if mc_status[m]!=3:
                continue
            if mc_pdgid[m] in (MUON,-MUON):
                v = ROOT.TLorentzVector()
                v.SetPtEtaPhiE(mc_pt[m],mc_eta[m],mc_phi[m],mc_e[m])
                v.charge = 'm' if mc_pdgid[m]==MUON else 'p'
                if True or fabs(v.Pt())>=20.0*GeV and fabs(v.Eta())<2.4:
                    _Tmus.append(v)
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

    # truth cuts (only for testing, since it will bias the results!)
    if False and not _DATA:  # Wmunu truth cut
        if len(_Tmus)==0:
            ef.truth+=1
            continue

    # event cuts
    if True and _DATA:
        if t.bcid in _BCIDS:
            ef.bcid+=1
            continue

    # primary vertex cut
    if True:
        nvx,vx_type,vx_ntracks = t.nvx,t.vx_type,t.vx_ntracks
        for m in range(nvx):
            if vx_type[m]==1 and vx_ntracks[m]>=3 and fabs(t.vx_z[m])<150:
                _ivertex = m
                _zvertex = t.vx_z[m]
                break
        if _ivertex==None:
            ef.vertex+=1
            continue

    # muon preselection + W cuts
    nmu,mu_author,mu_q,mu_pt,mu_eta,mu_phi,mu_ptcone40,mu_ptms,mu_qms,mu_qid,mu_z0 = t.nmu,t.mu_author,t.mu_q,t.mu_pt,t.mu_eta,t.mu_phi,t.mu_ptcone40,t.mu_ptms,t.mu_qms,t.mu_qid,t.mu_z0
    presel,pt,iso=(True,)*3
    for m in range(nmu):
        #mu_q[m]!=0
        if (mu_author[m]&2!=0) and fabs(mu_eta[m])<2.4 and mu_pt[m]>15*GeV and mu_ptms[m]>10*GeV and fabs(mu_z0[m]-_zvertex)<10:
            ptcone40 = mu_ptcone40[m]/mu_pt[m]
            presel = False
            h['PRESEL_mu_pt'].Fill(mu_pt[m])
            h['PRESEL_mu_phi'].Fill(mu_phi[m])
            h['PRESEL_mu_eta'].Fill(mu_eta[m])
            h['PRESEL_mu_ptiso'].Fill(ptcone40)
            h['PRESEL_met'].Fill(t.met_topo+t.met_muonboy)
            h['PRESEL_mu_pt_vs_met'].Fill(mu_pt[m],t.met_topo+t.met_muonboy)
            if mu_pt[m]>20*GeV:
                pt=False
                if ptcone40<0.2:
                    iso=False
                    v=ROOT.TLorentzVector()
                    v.SetPtEtaPhiM(mu_pt[m],mu_eta[m],mu_phi[m],muMASS)
                    v.charge='p' if mu_q[m]==1 else 'm'
                    v.ptcone40 = ptcone40
                    _Rmus.append(v)

    # muon preselection
    if presel:
        ef.muonpresel+=1
        continue
    # trigger
    if True and not opts.notrig:
        if t.trig_l1mu0==0:
            ef.trigger+=1
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
        et_topo,et_muon = t.met_topo,t.met_muonboy
        et_topo_phi,et_muon_phi = t.met_topo_phi,t.met_muonboy_phi
        et_x = et_topo*math.cos(et_topo_phi)+et_muon*math.cos(et_muon_phi)
        et_y = et_topo*math.sin(et_topo_phi)+et_muon*math.sin(et_muon_phi)
        _met=ROOT.TVector2(et_x,et_y)
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
        h['ANA_w_mt'].Fill(mW)
        # reconstructed a good W:
        if (mW>wMIN and mW<wMAX):
            ef.ok+=1
            h['ANA_mu_pt'].Fill(mu.Pt())
            h['ANA_mu_phi'].Fill(mu.Phi())
            h['ANA_mu_eta'].Fill(mu.Eta())
            h['ANA_mu_ptiso'].Fill(mu.ptcone40)
            h['ANA_met'].Fill(_met.Pt())
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

if False:
    # plot distributions overlayed
    PlotOverlayHistos(h["TMU_eta_p"],h["TMU_eta_m"],'True muon eta: red=mu+, blue=mu-','TMU_eta__overlay')
    PlotOverlayHistos(h["TMU_eta_p"],h["TMU_eta_m"],'True muon eta: red=mu+, blue=mu-','TMU_eta__norm',norm=True)
    PlotOverlayHistos(h["RMU_eta_p"],h["RMU_eta_m"],'Reco accepted muon eta: red=mu+, blue=mu-','RMU_eta__overlay')
    PlotOverlayHistos(h["RMU_eta_p"],h["RMU_eta_m"],'Reco accepted muon eta: red=mu+, blue=mu-','RMU_eta__norm',norm=True)
    PlotOverlayHistos(h["TMU_pt_p"],h["TMU_pt_m"],'True muon pt: red=mu+, blue=mu-','TMU_pt__overlay')
    PlotOverlayHistos(h["TMU_pt_p"],h["TMU_pt_m"],'True muon pt: red=mu+, blue=mu-','TMU_pt__norm',norm=True)
    PlotOverlayHistos(h["RMU_pt_p"],h["RMU_pt_m"],'Reco accepted muon pt: red=mu+, blue=mu-','RMU_pt__overlay')
    PlotOverlayHistos(h["RMU_pt_p"],h["RMU_pt_m"],'Reco accepted muon pt: red=mu+, blue=mu-','RMU_pt__norm',norm=True)

if '_LOAD_EFF' in dir() and False:
    # load efficiency histograms from file
    f=ROOT.TFile(_LOAD_EFF)
    if not f.IsOpen():
        print "Ooops, couldn't open",_LOAD_EFF
        sys.exit(0)
    print 'Loading efficiency templates from',_LOAD_EFF
    ROOT.gROOT.cd() 
    h["EFF_MU_eta_p"]=f.Get("EFF_MU_eta_p").Clone()
    h["EFF_MU_eta_m"]=f.Get("EFF_MU_eta_m").Clone()
    h["EFF_MU_pt_p"]=f.Get("EFF_MU_pt_p").Clone()
    h["EFF_MU_pt_m"]=f.Get("EFF_MU_pt_m").Clone()
    f.Close()
elif False:
    # make efficiency histograms
    MakeEffHistos(h["RMU_eta_p"],h["TMU_eta_p"],'MU_eta_p')
    MakeEffHistos(h["RMU_pt_p"],h["TMU_pt_p"],'MU_pt_p')
    MakeEffHistos(h["RMU_eta_m"],h["TMU_eta_m"],'MU_eta_m')
    MakeEffHistos(h["RMU_pt_m"],h["TMU_pt_m"],'MU_pt_m')
    # plot efficiencies overlayed
    PlotOverlayHistos(h["EFF_MU_pt_p"],h["EFF_MU_pt_m"],'Reconstruction efficiency vs pt: red=mu+, blue=mu-','EFF_MU_pt__overlay')
    PlotOverlayHistos(h["EFF_MU_pt_p"],h["EFF_MU_pt_m"],'Reconstruction efficiency vs pt: red=mu+, blue=mu-','EFF_MU_pt__norm',norm=True)
    PlotOverlayHistos(h["EFF_MU_eta_p"],h["EFF_MU_eta_m"],'Reconstruction efficiency vs eta: red=mu+, blue=mu-','EFF_MU_eta__overlay')
    PlotOverlayHistos(h["EFF_MU_eta_p"],h["EFF_MU_eta_m"],'Reconstruction efficiency vs eta: red=mu+, blue=mu-','EFF_MU_eta__norm',norm=True)
    
# asymmetries, ratios, efficiencies
if False:
    MakeAsymmRatioHistos(h["TMU_eta_p"],h["TMU_eta_m"],'TMU_eta')
    MakeAsymmRatioHistos(h["TMU_pt_p"],h["TMU_pt_m"],'TMU_pt')
    # uncorrected for relative W+/W- reconstruction efficiency
    MakeAsymmRatioHistos(h["RMU_eta_p"],h["RMU_eta_m"],'RMU_eta__uncorrected')
    MakeAsymmRatioHistos(h["RMU_pt_p"],h["RMU_pt_m"],'RMU_pt__uncorrected')
    # corrected for relative W+/W- reconstruction efficiency (using efficiency histograms)
    h_eta_p = h["RMU_eta_p"].Clone(h["RMU_eta_p"].GetName() + "__corrected")
    h_eta_m = h["RMU_eta_m"].Clone(h["RMU_eta_m"].GetName() + "__corrected")
    h_pt_p = h["RMU_pt_p"].Clone(h["RMU_pt_p"].GetName() + "__corrected")
    h_pt_m = h["RMU_pt_m"].Clone(h["RMU_pt_m"].GetName() + "__corrected")
    h_eta_p.Divide(h['EFF_MU_eta_p'])
    h_eta_m.Divide(h['EFF_MU_eta_m'])
    h_pt_p.Divide(h['EFF_MU_pt_p'])
    h_pt_m.Divide(h['EFF_MU_pt_m'])
    MakeAsymmRatioHistos(h_eta_p,h_eta_m,'RMU_eta__corrected')
    MakeAsymmRatioHistos(h_pt_p,h_pt_m,'RMU_pt__corrected')

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
