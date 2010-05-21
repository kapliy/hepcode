#!/usr/bin/env python

_GOODLIST = ( 21677784,22180828,29555751,29874999 )
_GOODEVTS = ( 255497,261005,344577,347861 )
_DATA = True

_INPUTS = '/scratch/antonk/zmumu.root'
_INPUTS = '/scratch/antonk/wmunu.root'
_INPUTS = 'dcache:///pnfs/uct3/data/users/antonk/ANALYSIS/PLHC/153565/*root*'
#_LOAD_EFF = './efficiencies.root'

try:
    import psyco
    psyco.full()
    print 'Using psyco'
except:
    pass

from helpers import *
import math,sys,glob,re
import ROOT
ROOT.gROOT.SetBatch(1) #uncomment for interactive usage

from optparse import OptionParser
parser = OptionParser()
parser.add_option("-n", "--nevents",dest="nevents",
                  type="int", default=1000000,
                  help="Maximum number of events to process")
parser.add_option("-s", "--nskip",dest="nskip",
                  type="int", default=0,
                  help="Number of events to skip")
parser.add_option("-i", "--input",dest="input",
                  type="string", default=_INPUTS,
                  help="Full path to ntuple to process")
parser.add_option("-u", "--no-save",
                  action="store_true",dest="nosave",
                  help="Don't save any output on disk")
parser.add_option("-t", "--no-trigger",
                  action="store_true",dest="notrig",
                  help="Don't require trigger selection disk")

(opts, args) = parser.parse_args()

Histo("TW_vs_TMU_eta",etabins,etabins)
Histo("TNU_vs_MET_pt",ptbins,metbins)

Histo("RW_mT_p",wmassbins,color=ROOT.kRed)
Histo("RW_mT_m",wmassbins,color=ROOT.kBlue)
Histo("RZ_m",wmassbins,color=ROOT.kBlue)

Histo("TMU_eta_p",etabins,color=ROOT.kRed)
Histo("TMU_eta_m",etabins,color=ROOT.kBlue)
Histo("RMU_eta_p",etabins,color=ROOT.kRed)
Histo("RMU_eta_m",etabins,color=ROOT.kBlue)

Histo("TMU_pt_p",ptbins,color=ROOT.kRed)
Histo("TMU_pt_m",ptbins,color=ROOT.kBlue)
Histo("RMU_pt_p",ptbins,color=ROOT.kRed)
Histo("RMU_pt_m",ptbins,color=ROOT.kBlue)

# charge misidentification in accepted muons
Histo("QMS_vs_QID",qbins,qbins)
Histo("QMATCH_eta",etabins)
Histo("QMATCH_phi",etabins)
Histo("QMATCH_pt",ptbins)

t = ROOT.TChain('tree')
t.Add(opts.input)
nentries = t.GetEntries()

print 'Starting loop over',(opts.nevents if opts.nevents<nentries else nentries),'/',nentries,'entries'
ef = EventFlow()
for evt in _GOODEVTS:
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
                    h['TMU_pt_%c'%v.charge].Fill(mc_pt[m])
                    h['TMU_eta_%c'%v.charge].Fill(mc_eta[m])
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
            ef.TRUTH()
            continue

    # event cuts
    if True and _DATA:
        if t.bcid==0:
            ef.BCID()
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
            ef.VERTEX()
            continue

    # muon preselection
    if True:
        nmu,mu_author,mu_q,mu_pt,mu_eta,mu_phi,mu_ptcone20,mu_ptms,mu_qms,mu_qid,mu_z0 = t.nmu,t.mu_author,t.mu_q,t.mu_pt,t.mu_eta,t.mu_phi,t.mu_ptcone20,t.mu_ptms,t.mu_qms,t.mu_qid,t.mu_z0
        for m in range(nmu):
            #mu_q[m]!=0
            if (mu_author[m]&2!=0) and fabs(mu_eta[m])<2.4 and mu_pt[m]>15*GeV and mu_ptms[m]>10*GeV and fabs(mu_z0[m]-_zvertex)<10:
                v=ROOT.TLorentzVector()
                v.SetPtEtaPhiM(mu_pt[m],mu_eta[m],mu_phi[m],muMASS)
                v.charge='p' if mu_q[m]==1 else 'm'
                v.ptcone20 = mu_ptcone20[m]
                _Rmus.append(v)
    if len(_Rmus)==0:
        ef.MUON()
        continue

    # trigger
    if True:
        if t.trig_l1mu0==0:
            ef.TRIGGER()
            continue

    # met from met_topo + met_muon
    if True:
        et_topo,et_muon = t.met_topo,t.met_muon
        et = et_topo + et_muon
        if et<15*GeV:
            ef.MET()
            continue
        et_topo_phi,et_muon_phi = t.met_topo_phi,t.met_muon_phi
        et_x = et_topo*math.cos(et_topo_phi)+et_muon*math.cos(et_muon_phi)
        et_y = et_topo*math.sin(et_topo_phi)+et_muon*math.sin(et_muon_phi)
        _met=ROOT.TVector2(et_x,et_y)
        _met.Pt = _met.Mod  # define Pt() since TVector2 doesn't have it

        mu=_Rmus[0]
    #print t.event,_met.Pt(),_met.Phi(),mu.Pt(),mu.Eta(),mu.charge

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
                ef.ZCUT()
                continue
            else:
                mdiff,mZ,i=closest2(zMASS,cands)
                h["RZ_m"].Fill(mZ)
                
    # W candidates in W->munu decay
    if True:
        cands=[]  # [mu_index,mW]
        for i,mu in enumerate(_Rmus):
            if fabs(mu.Phi()-_met.Phi())>math.pi/2.0:  # dPhi cut on MET and muon
                mW=math.sqrt( 2*mu.Pt()*_met.Pt()*(1-math.cos(_met.Phi()-mu.Phi())) )
                cands.append((i,mW))
        if len(cands)==0:
            ef.METMUPHI()
            continue
        # choose the closest W candidate:
        mdiff,mW,i=closest(wMASS,cands)
        mu=_Rmus[cands[i][0]]
        h['RW_mT_%c'%mu.charge].Fill(mW)
        # reconstructed a good W:
        if (mW>wMIN and mW<wMAX):
            ef.OK()
            h['RMU_pt_%c'%mu.charge].Fill(mu.Pt())
            h['RMU_eta_%c'%mu.charge].Fill(mu.Eta())
            #ws=RecoW(mu.E(),mu.Px(),mu.Py(),mu.Pz(),_met.Px(),_met.Py())
            #_RWs+=ws
            # check with truth
        else:
            ef.W()
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
    out = ROOT.TFile("out.root","RECREATE")
    for hh in h.values():
        hh.Write()
        savePlot(hh)
    out.Close()
    ef.Print(open('./cuts.txt','w'))
    ef.Print(open('/dev/stdout','w'))
