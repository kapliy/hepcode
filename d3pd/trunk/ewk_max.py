#!/usr/bin/env python

""" Cutflow from d3pd
Backing up old version to validate electroweak cutflow spreadsheet.
Max's inclusie cutflow comes later!
"""

import sys,math
from math import cos,sin,sqrt
import ROOT
ROOT.gROOT.SetBatch(1)
ROOT.gROOT.ProcessLine('.L loader.C+')
true,false = True,False
GeV = 1000.0

ROOT.gSystem.Load('/home/antonk/TrigFTKAna/CommonAnalysis/PileupReweighting/StandAlone/libPileupReweighting.so')

pw = ROOT.Root.TPileupReweighting( "pw" )
pw.SetDefaultChannel(0);
pw.SetUnrepresentedDataAction(2);
pw.DisableWarnings(true);
mode = 0
if mode==0:
    pw.AddConfigFile("./PileupReweighting/MC11c.prw.root");
    pw.AddLumiCalcFile('./PileupReweighting/ilumicalc_histograms_None_179710-191933.root');
elif mode==1:
    pw.AddConfigFile("./PileupReweighting/MC11c.prw.2.root");
    pw.AddLumiCalcFile('./PileupReweighting/ilumicalc_histograms_None_179710-191933_OflLumi-7TeV-003.root');    
elif mode==2:
    pw.AddConfigFile("/home/antonk/TrigFTKAna/pileup_histos/mc11c_custom.root");
    pw.AddLumiCalcFile('/home/antonk/TrigFTKAna/pileup_histos/asym_data11_7TeV.pro10.DtoM.root')
else:
    assert False
pw.Initialize();

def xflatten(seq):
    """a generator to flatten a nested list"""
    for x in seq:
        if type(x) is list:
            for y in xflatten(x):
                yield y
        else:
            yield x
def SetTreeBranches(t):
    """ Sets branches for v11 peter ntuple """
    t.SetBranchStatus("*", 0)
    br = []
    br.append(['mc_event_weight','mc_channel_number','EventNumber','RunNumber','averageIntPerXing'])
    br.append(['MET_RefFinal*'])
    br.append(['vxp_*'])
    br.append(['EF_mu18_MG','EF_mu18_MG_medium'])
    br.append(['mu_staco*'])
    br.append(['jet_AntiKt4TopoEM*'])
    [t.SetBranchStatus(v,1) for v in xflatten(br)]


# configure METUtility package:
fname='/home/antonk/d3pd/MAX/mc11_7TeV.106083.McAtNloWplusmunu_no_filter.merge.NTUP_SMWZ.e878_s1310_s1300_r3043_r2993_p1035_tid00813122_00/NTUP_SMWZ.00813122._000101.root'
f = ROOT.TFile.Open(fname)
t = f.physics
t.ls()
SetTreeBranches(t)

nentries = t.GetEntries()
print nentries
nproc = nentries
#nproc = 10
OUT = []

evnum = -1
ntot,nvtx,ntrig,ntype,nmcp,nqual,npt,neta,niso,nveto,nmetclean,nlarhole,nmet,nmt = [0]*14

#import psyco
#psyco.full()

def passes_hits_mcp_blayer_v17(mu_nBLHits,mu_expectBLayerHit):
    return mu_nBLHits!=0 or not mu_expectBLayerHit
def passes_hits_mcp_holes_v17(mu_nPixHoles,mu_nSCTHoles):
    maxholes = 3
    return mu_nPixHoles+mu_nSCTHoles < maxholes
def passes_hits_mcp_sitrt_v17(mu_nTRTHits,mu_nTRTOutliers,mu_nPixHits,mu_nPixelDeadSensors,mu_nSCTHits,mu_nSCTDeadSensors,eta):
    ntrt = mu_nTRTHits+mu_nTRTOutliers
    passes_hits = ( mu_nPixHits + mu_nPixelDeadSensors > 1 ) and ( mu_nSCTHits+mu_nSCTDeadSensors >=6 ) and ((ntrt>=6 and mu_nTRTOutliers<0.9*ntrt) if abs(eta)<1.9 else (ntrt<6 or mu_nTRTOutliers<0.9*ntrt))
    return passes_hits;
def passes_hits_mcp_v17(t,i):
    passes_b_layer = passes_hits_mcp_blayer_v17(t.mu_staco_nBLHits[i],t.mu_staco_expectBLayerHit[i])
    passes_holes = passes_hits_mcp_holes_v17(t.mu_staco_nPixHoles[i],t.mu_staco_nSCTHoles[i])
    passes_hits = passes_hits_mcp_sitrt_v17(t.mu_staco_nTRTHits[i],t.mu_staco_nTRTOutliers[i],t.mu_staco_nPixHits[i],t.mu_staco_nPixelDeadSensors[i],t.mu_staco_nSCTHits[i],t.mu_staco_nSCTDeadSensors[i],t.mu_staco_eta[i])
    return passes_b_layer and passes_holes and passes_hits

def isBadJet_looser_v17(t,i):
    emf = t.jet_AntiKt4TopoEM_emfrac[i]
    hecf = t.jet_AntiKt4TopoEM_hecf[i]
    quality = t.jet_AntiKt4TopoEM_LArQuality[i]
    AvgLArQ = t.jet_AntiKt4TopoEM_AverageLArQF[i]
    HecQ = t.jet_AntiKt4TopoEM_HECQuality[i]
    sumpttrk = t.jet_AntiKt4TopoEM_sumPtTrk[i]
    eta = t.jet_AntiKt4TopoEM_emscale_eta[i]
    pt = t.jet_AntiKt4TopoEM_pt[i]
    fmax = t.jet_AntiKt4TopoEM_fracSamplingMax[i]
    NegE = t.jet_AntiKt4TopoEM_NegativeE[i]
    chf = sumpttrk/pt if pt!=0 else 0
    LArQmean = AvgLArQ/65335. ; 
    # HEC spike
    if( hecf>0.5 and abs(HecQ)>0.5 and LArQmean>0.8):         return true;
    if( abs(NegE)>60000.):                                  return true;
    # EM coherent noise
    if( emf>0.95 and abs(quality)>0.8 and LArQmean>0.8 and abs(eta)<2.8 ):   return true;
    # Cosmics and Beam background
    if( emf<0.05 and chf<0.05 and abs(eta)<2. ):             return true;
    if( emf<0.05 and abs(eta)>2. ):                         return true;
    if( fmax>0.99 and abs(eta)<2):                            return true;
    return false;

def IsLArHole(eta,phi, etamin = -0.1, etamax = 1.5, phimin = -0.9, phimax = -0.5):
    if (eta < etamin or eta > etamax): return false;
    if (phi < phimin or phi > phimax): return false;
    return true;
def simpleVeto(jet_pt, jet_eta,
               jet_phi,
               jet_BCH_CORR_JET,
               jet_BCH_CORR_CELL,
               threshold=40000.):
    if (not IsLArHole(jet_eta,jet_phi)): return false;
    pt = threshold;
    # Correct threshold cut by missing energy in LAr hole in data.
    # BCH_CORR_CELL is necessary to take into account dead tile module near LAr hole.
    pt = pt*(1. - jet_BCH_CORR_JET)/(1. - jet_BCH_CORR_CELL);
    if (jet_pt <=pt): return false;
    return true;
                          
def LArHole_v17(t):
    LArHoleEvent = false;
    isdata = False
    JetPt_threshold = 20
    #LARHOLE:  3431 183045 3084432 | 0 28.9864316406 0.515794336796 0.159949108958 0.00768974376842 True
    if evnum==3084432: print 'LARHOLE: ',evt,rnum,evnum,t.RunNumber
    if (    ((not isdata) and (t.RunNumber == 183003)) or   (isdata and (t.RunNumber >=180614) and (t.RunNumber <185352))    ):
        for i in xrange(0,t.jet_AntiKt4TopoEM_n):
            if evnum==3084432: print '|',i,t.jet_AntiKt4TopoEM_pt[i]/GeV, t.jet_AntiKt4TopoEM_eta[i],t.jet_AntiKt4TopoEM_BCH_CORR_JET[i], t.jet_AntiKt4TopoEM_BCH_CORR_CELL[i],
            if abs(t.jet_AntiKt4TopoEM_eta[i])>4.5: continue
            if t.jet_AntiKt4TopoEM_pt[i]<20*GeV: continue
            if (simpleVeto(t.jet_AntiKt4TopoEM_pt[i], t.jet_AntiKt4TopoEM_eta[i], t.jet_AntiKt4TopoEM_phi[i],
                           t.jet_AntiKt4TopoEM_BCH_CORR_JET[i], t.jet_AntiKt4TopoEM_BCH_CORR_CELL[i], JetPt_threshold)):
                LArHoleEvent = true
                break
    else:
        if evnum==3084432: print 'rnum says no need to apply LarHole'
    if evnum==3084432:
        print LArHoleEvent
    return LArHoleEvent

for evt in xrange(nproc):
    if evt%100==0: print evt
    t.GetEntry(evt)
    w = int(t.mc_event_weight)
    pw.SetRandomSeed(314159 + t.mc_channel_number*2718 + t.EventNumber)
    evnum = t.EventNumber
    rnum = pw.GetRandomRunNumber(t.RunNumber);
    w2 = pw.GetCombinedWeight( t.RunNumber, t.mc_channel_number , t.averageIntPerXing )
    w = 1.0 * w2 * t.mc_event_weight   # ENABLE PILEUP REWEIGHTING FOR CUTFLOW!
    #print 'WEOGHTS',evnum,t.mc_event_weight, w, t.averageIntPerXing , w2
    ntot+=w
    if t.vxp_n>0 and t.vxp_nTracks[0]>=3:
        nvtx += w
        trig = t.EF_mu18_MG if rnum<186516 else t.EF_mu18_MG_medium
        if trig:
            ntrig += w
            # loop over muons
            MU_TYPE = []
            MU_MCP = []
            MU_QUAL = []
            MU_PT = []
            MU_ETA = []
            MU_ISO = []
            for imu in xrange(0,t.mu_staco_n):
                if t.mu_staco_isCombinedMuon[imu] and t.mu_staco_author[imu]==6:
                    MU_TYPE.append(imu)
                    if passes_hits_mcp_v17(t,imu):
                        MU_MCP.append(imu)
                        if abs(t.mu_staco_id_z0_exPV[imu])<10.0: # and t.mu_staco_id_z0[imu]>-100000:
                            MU_QUAL.append(imu)
                            if t.mu_staco_pt[imu]>20*GeV:
                                MU_PT.append(imu)
                                if abs(t.mu_staco_eta[imu])<2.4:
                                    MU_ETA.append(imu)
                                    if t.mu_staco_ptcone20[imu]/t.mu_staco_pt[imu]<0.1:
                                        MU_ISO.append(imu)
            if len(MU_TYPE)>0: ntype += w
            if len(MU_MCP)>0: nmcp += w
            if len(MU_QUAL)>0: nqual += w
            if len(MU_PT)>0: npt += w
            if len(MU_ETA)>0: neta += w
            if len(MU_ISO)>0: niso += w
            if len(MU_ISO)==0: continue
            if len(MU_ISO)==1:
                nveto += w
            else:
                continue
            # met cleaning
            BADJ = []
            for ijet in xrange(0,t.jet_AntiKt4TopoEM_n):
                if t.jet_AntiKt4TopoEM_pt[ijet]>20*GeV and abs(t.jet_AntiKt4TopoEM_eta[ijet])<4.5:
                    if isBadJet_looser_v17(t,ijet):
                        BADJ.append(ijet)
            if len(BADJ)==0: nmetclean += w
            else: continue
            # lar hole cleaning
            if LArHole_v17(t): continue
            OUT.append((rnum,evnum,int(w)))
            nlarhole += w
            # w boson construction (TODO)
            if t.MET_RefFinal_et > 25*GeV:
                nmet += w
                imu = MU_ISO[0]
                mW=sqrt( 2*t.mu_staco_pt[imu]*t.MET_RefFinal_et*(1-cos(t.MET_RefFinal_phi-t.mu_staco_phi[imu])) )
                if mW>40*GeV:
                    nmt += w


if len(OUT)>0:
    ff = open('events.txt','w')
    for ox in OUT:
        print >>ff,ox[0],ox[1],ox[2]
    ff.close()

print 'tot',ntot
print 'vtx',nvtx
print 'trig',ntrig
print '---------'
print 'type',ntype
print 'mcp',nmcp
print 'qual',nqual
print 'pt',npt
print 'eta',neta
print 'iso',niso
print '---------'
print 'veto',nveto
print 'metclean',nmetclean
print 'larhole',nlarhole
print 'met',nmet
print 'mt',nmt
